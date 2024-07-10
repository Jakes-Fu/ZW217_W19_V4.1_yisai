/******************************************************************************
 ** File Name:      ms_ref_dc_trc.h                                         *
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
//trace_id:42
#ifndef _MS_REF_DC_TRC_H_
#define _MS_REF_DC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define DC_APP_152_112_2_18_1_23_11_0 "DC_FrameMgr_CreateAFrame() failed"
#define DC_APP_177_112_2_18_1_23_11_1 "_EncodeYYUVToJpeg(),jpeg encoding err,line %d"
#define DC_APP_198_112_2_18_1_23_11_2 "DCAM:_mjpeg_encode_callback, record_callback is null err"
#define DC_APP_259_112_2_18_1_23_11_3 "DCAM:_CheckMjpegRecParam target size:%d,%d err"
#define DC_APP_267_112_2_18_1_23_11_4 "DCAM:_CheckMjpegRecParam disply size:%d,%d err"
#define DC_APP_301_112_2_18_1_23_11_5 "DCAM:_SetMjpegRecParam caputre buf is null err!"
#define DC_APP_311_112_2_18_1_23_11_6 "DCAM:_SetMjpegRecParam display buf is null err!"
#define DC_APP_321_112_2_18_1_23_11_7 "DCAM:_SetMjpegRecParam scale buf is null err!"
#define DC_APP_331_112_2_18_1_23_11_8 "DCAM:_SetMjpegRecParam rotation buf is null err!"
#define DC_APP_472_112_2_18_1_23_11_9 "DCAM:_SetMjpegDecParam y/uv buf is null err!"
#define DC_APP_612_112_2_18_1_23_12_10 "_DCAMERA_ROT_Convertor: %d"
#define DC_APP_648_112_2_18_1_23_12_11 "_DCAMERA_ROT_Convertor: img: %d, ret: %d"
#define DC_APP_681_112_2_18_1_23_12_12 "DCAM:_DCAMERA_InitSensor img sensor init fail err!"
#define DC_APP_688_112_2_18_1_23_12_13 "_DCAMERA_InitSensor: before sensor_open, %d"
#define DC_APP_691_112_2_18_1_23_12_14 "DCAM:_DCAMERA_InitSensor img sensor open fail err!"
#define DC_APP_694_112_2_18_1_23_12_15 "_DCAMERA_InitSensor: after sensor_open, %d"
#define DC_APP_712_112_2_18_1_23_12_16 "DCAMERA_SetSensorType: %d"
#define DC_APP_746_112_2_18_1_23_12_17 "DCAM:DC_Open,time: %d"
#define DC_APP_788_112_2_18_1_23_12_18 "DCAM: enter, mode %d, time %d"
#define DC_APP_797_112_2_18_1_23_12_19 "---------DCAMERA_Open: USE fmark-------- "
#define DC_APP_815_112_2_18_1_23_12_20 "DCAM:DC lock isp error"
#define DC_APP_830_112_2_18_1_23_12_21 "DCAM, end:  time %d"
#define DC_APP_849_112_2_18_1_23_12_22 "DCAM:DCAMERA_Close start: %d"
#define DC_APP_893_112_2_18_1_23_12_23 "DCAM:DCAMERA_Close end: %d"
#define DC_APP_922_112_2_18_1_23_12_24 "DCAM:DCAMERA_StartPreview, start time = %d"
#define DC_APP_926_112_2_18_1_23_12_25 "DCAM:DCAMERA_StartPreview param_ptr is null"
#define DC_APP_975_112_2_18_1_23_13_26 "DCAM:DCAMERA_StartPreview end: %d"
#define DC_APP_994_112_2_18_1_23_13_27 "DCAM: DCAMERA_StopPreview, time %d"
#define DC_APP_1021_112_2_18_1_23_13_28 "DCAMERA_ATVAutoScanChn: In Time: %d"
#define DC_APP_1026_112_2_18_1_23_13_29 "DCAM:DCAMERA_StartPreview param_ptr is null"
#define DC_APP_1040_112_2_18_1_23_13_30 "DCAMERA_ATVAutoScanChn: Out Time: %d"
#define DC_APP_1058_112_2_18_1_23_13_31 "DCAM: DCAMERA_ATVStopScanChn: Time: %d"
#define DC_APP_1122_112_2_18_1_23_13_32 "DCAM: DCAMERA_DoSnapshot, time %d"
#define DC_APP_1127_112_2_18_1_23_13_33 "DCAM:DCAMERA_DoSnapshot param_ptr/return_param_ptr is null"
#define DC_APP_1144_112_2_18_1_23_13_34 "DCAM: capture asynchronous"
#define DC_APP_1149_112_2_18_1_23_13_35 "DCAM: capture synchronous"
#define DC_APP_1174_112_2_18_1_23_13_36 "DCAM: DCAMERA_DoBurstSnapshot, time %d"
#define DC_APP_1179_112_2_18_1_23_13_37 "DCAM:DCAMERA_DoSnapshot param_ptr/return_param_ptr is null err"
#define DC_APP_1185_112_2_18_1_23_13_38 "DCAM:DCAMERA_DoSnapshot burst_snapshot_callback is null err"
#define DC_APP_1220_112_2_18_1_23_13_39 "DCAM: DCAMERA_ReviewJpeg rot:%d"
#define DC_APP_1240_112_2_18_1_23_13_40 "DCAM: DCAMERA_ReviewJpeg param_ptr is null err"
#define DC_APP_1255_112_2_18_1_23_13_41 "DCAM: review asynchronous"
#define DC_APP_1260_112_2_18_1_23_13_42 "DCAM: review synchronous"
#define DC_APP_1288_112_2_18_1_23_13_43 "DCAM: DCAMERA_GetInfo start cmd:0x%x info:0x%x, %d"
#define DC_APP_1434_112_2_18_1_23_14_44 "DCAM: DCAMERA_GetInfo end cmd:0x%x info:0x%x, %d"
#define DC_APP_1454_112_2_18_1_23_14_45 "DCAM: DCAMERA_ParamCtrl  start: %d"
#define DC_APP_1467_112_2_18_1_23_14_46 "DCAM: DCAMERA_ParamCtrl end: %d"
#define DC_APP_1513_112_2_18_1_23_14_47 "DCAM: DC_SetUpccParam"
#define DC_APP_1517_112_2_18_1_23_14_48 "DCAM:DC_SetUpccParam pst_param is null err"
#define DC_APP_1552_112_2_18_1_23_14_49 "DCAM:DC_UpccStart param_ptr is null err"
#define DC_APP_1649_112_2_18_1_23_14_50 "DCAM:DC_SetMjpgRecParam pst_param is null err"
#define DC_APP_1677_112_2_18_1_23_14_51 "DC_StartMjpgRec(%lu,0x%lx)"
#define DC_APP_1681_112_2_18_1_23_14_52 "DCAM:DC_StartMjpgRec pf_callback is null err"
#define DC_APP_1821_112_2_18_1_23_14_53 "DCAM:DC_VideoDisplayInit"
#define DC_APP_1845_112_2_18_1_23_14_54 "DCAM:DC_VideoDisplayClose"
#define DC_APP_1880_112_2_18_1_23_14_55 "DCAM:DC_VideoSetDisplayParam"
#define DC_APP_1884_112_2_18_1_23_14_56 "DCAM:DC_VideoSetDisplayParam pst_param is null err"
#define DC_APP_1945_112_2_18_1_23_15_57 "DCAM:DC_DisplayAFrame p_frame is null err"
#define DC_APP_1954_112_2_18_1_23_15_58 "DCAM:DC lock isp error"
#define DC_APP_1998_112_2_18_1_23_15_59 "DCAM:DC_GetFrameData register func is null err"
#define DC_APP_2141_112_2_18_1_23_15_60 "DCM: _ExtSetGetDataParam jpg buf is not enough err"
#define DC_APP_2434_112_2_18_1_23_16_61 "DCM: DCAMERA_ExtGetData param is null or img sesor is not open"
#define DC_APP_2469_112_2_18_1_23_16_62 "DCM: DCAMERA_ExtGetData capture err"
#define DC_APP_2488_112_2_18_1_23_16_63 "DCM: DCAMERA_ExtGetData review err"
#define DC_APP_2500_112_2_18_1_23_16_64 "DCM: DCAMERA_ExtGetData encoder err"
#define DC_BUFMGR_236_112_2_18_1_23_24_65 "DCAM:MM_AllocRawBuffer: Buffer allaoc fail err"
#define DC_BUFMGR_255_112_2_18_1_23_24_66 "DCAM: MM_AllocRawBuffer: Buffer is allaoced"
#define DC_BUFMGR_1098_112_2_18_1_23_25_67 "DCAM: MM_Init, preview buf is not enough err!"
#define DC_BUFMGR_1240_112_2_18_1_23_26_68 "DCAM: MM_Init, capture buf is not enough err!"
#define DC_BUFMGR_1390_112_2_18_1_23_26_69 "DCAM: MM_Init, review buf is not enough err!"
#define DC_BUFMGR_1462_112_2_18_1_23_26_70 "DCAM: MM_Init, continue shot buf is not enough err!"
#define DC_BUFMGR_1533_112_2_18_1_23_26_71 "DCAM: MM_Init, dv review buf is not enough err!"
#define DC_BUFMGR_1558_112_2_18_1_23_27_72 "DCAM: MM_Init, dv review buf is not enough err!"
#define DC_BUFMGR_1567_112_2_18_1_23_27_73 "DCAM: Sorry!MM buffer MGR,case id does not exist error"
#define DC_BUFMGR_1631_112_2_18_1_23_27_74 "DCAM: MM_Init, all buf start addr=0x%x,buf_len=%dk"
#define DC_BUFMGR_1636_112_2_18_1_23_27_75 "DCAM: MM_Init alloc dc buf fail err!"
#define DC_BUFMGR_1696_112_2_18_1_23_27_76 "DCAM: MM_Malloc, pMM->buf_ptr is null %d line error"
#define DC_BUFMGR_1706_112_2_18_1_23_27_77 "DCAM: MM_Malloc, tbl_ptr is null %d line error"
#define DC_BUFMGR_1711_112_2_18_1_23_27_78 "DCAM: MM_Malloc, (group,block)=(%d,%d), addr range(0x%lx ~ 0x%lx) "
#define DC_BUFMGR_1732_112_2_18_1_23_27_79 "DCAM: MM_GetSize, pMM->buf_ptr is null %d line error"
#define DC_BUFMGR_1743_112_2_18_1_23_27_80 "DCAM: MM_GetSize, tbl_ptr is null %d line error"
#define DC_CODEC_94_112_2_18_1_23_27_81 "DCAM: _DC_SetMinatureInParam"
#define DC_CODEC_132_112_2_18_1_23_27_82 "DCAM: _DC_SetThumbnailMinatureInParam"
#define DC_CODEC_165_112_2_18_1_23_27_83 "DCAM: _DC_SetWriteEixfInParam"
#define DC_CODEC_215_112_2_18_1_23_28_84 "DCAM:DC_Get_HwSliceScaleLine: %d"
#define DC_CODEC_272_112_2_18_1_23_28_85 "DCAM:_GetSliceYUVData,photo_width=%d,get_slice_width=%d err"
#define DC_CODEC_278_112_2_18_1_23_28_86 "DCAM:_GetSliceYUVData,photo_height=%d,line_offsert=%d err"
#define DC_CODEC_415_112_2_18_1_23_28_87 "_EncodeYYUVToThumbnail(),thumbnail scale yuv buf err ,line %d"
#define DC_CODEC_487_112_2_18_1_23_28_88 "_EncodeYYUVToThumbnail(),jpeg encoding err ,line %d"
#define DC_CODEC_495_112_2_18_1_23_28_89 "DCAM: _EncodeYYUVToThumbnail, thumbnail_addr = 0x%x"
#define DC_CODEC_496_112_2_18_1_23_28_90 "DCAM: _EncodeYYUVToThumbnail, thumbnail_size = %d"
#define DC_CODEC_539_112_2_18_1_23_28_91 "DCAM: _DC_JpegToThumbnail, Greate thumbnail err ,line %d"
#define DC_CODEC_547_112_2_18_1_23_28_92 "DCAM: _DC_JpegToThumbnail, thumbnail_buf_addr = 0x%x"
#define DC_CODEC_548_112_2_18_1_23_28_93 "DCAM: _DC_JpegToThumbnail, thumbnail_size = %d"
#define DC_CODEC_582_112_2_18_1_23_28_94 "DCAM: DC_ExifGreate(),jpeg write exif error,line %d"
#define DC_CODEC_588_112_2_18_1_23_28_95 "DCAM: DC_ExifGreate(),jpeg_exif_buf_ptr = 0x%x"
#define DC_CODEC_589_112_2_18_1_23_28_96 "DCAM: DC_ExifGreate(),jpeg_exif_size = %d"
#define DC_CODEC_676_112_2_18_1_23_29_97 "_EncodeYYUVToJpeg(),jpeg encoding error,line %d"
#define DC_CODEC_685_112_2_18_1_23_29_98 "DCAM: _EncodeYYUVToJpeg(),jpeg_buf_ptr = 0x%x"
#define DC_CODEC_686_112_2_18_1_23_29_99 "DCAM: _EncodeYYUVToJpeg(),jpeg_size = %d"
#define DC_CODEC_745_112_2_18_1_23_29_100 "DCAM: _EncodeYYUVSliceToJpeg(),jpeg encoding err,line %d"
#define DC_CODEC_754_112_2_18_1_23_29_101 "DCAM: _EncodeYYUVSliceToJpeg(),jpeg_buf_ptr = 0x%x"
#define DC_CODEC_755_112_2_18_1_23_29_102 "DCAM: _EncodeYYUVSliceToJpeg(),jpeg_size = %d"
#define DC_CODEC_803_112_2_18_1_23_29_103 "DCAM: DC_JpegProcess,Greate Miniature err ,line %d"
#define DC_CODEC_817_112_2_18_1_23_29_104 "DCAM: DC_JpegProcess(),jpeg_buf_ptr = 0x%x"
#define DC_CODEC_818_112_2_18_1_23_29_105 "DCAM: DC_JpegProcess(),jpeg_size = %d"
#define DC_CODEC_882_112_2_18_1_23_29_106 "_DecodeJpegToYYUV(),jpeg decoding error,line %d"
#define DC_FRAMEMGR_62_112_2_18_1_23_30_107 "DC_FrameMgr_Init(),base_addr = %x, uplimited_addr = %x"
#define DC_FRAMEMGR_93_112_2_18_1_23_30_108 "DC DC_FrameMgr_C,ring buffer overflowed!,line %d"
#define DC_FRAMEMGR_103_112_2_18_1_23_30_109 "DC DC_FrameMgr_C,frame fifo overflowed!, line %d"
#define DC_FRAMEMGR_141_112_2_18_1_23_30_110 "DC_FrameMgr_CreateAFrame,fragment appears,len = %lu, line %d"
#define DC_FRAMEMGR_157_112_2_18_1_23_30_111 "DC_FrameMgr_CreateAFrame,fragment appears,len = %lu, line %d"
#define DC_FRAMEMGR_164_112_2_18_1_23_30_112 "DC_FrameMgr_CreateAFrame,remain ,len = %lu, line %d"
#define DC_FRAMEMGR_227_112_2_18_1_23_30_113 "DC_FrameMgr_F, pBufMgr->tail_fragment_len:%d, remain:%d"
#define DC_LCDC_81_112_2_18_1_23_30_114 "\nLCD:%d block:%d not supoort"
#define DC_LCDC_93_112_2_18_1_23_30_115 "\n The caller thread must get mutex first"
#define DC_LCDC_106_112_2_18_1_23_30_116 "LCDC_SetBlockBuffer, buf_ptr 0x%x"
#define DC_LCDC_119_112_2_18_1_23_31_117 "LCDC_EnableBlock"
#define DC_LCDC_130_112_2_18_1_23_31_118 "LCDC_DisableBlock"
#define DC_LCDC_160_112_2_18_1_23_31_119 "LCDC_ConfigBlock"
#define DC_LCDC_271_112_2_18_1_23_31_120 "LCDC_SetOsdBlockRect"
#define DC_LCDC_285_112_2_18_1_23_31_121 "LCDC_OsdBlockRectEnable"
#define DC_LCDC_297_112_2_18_1_23_31_122 "LCDC_OsdBlockRectDisable"
#define DC_MISC_449_112_2_18_1_23_32_123 "DCAM:DC_AdjRect_OnDistortion: disp_mode:%d, rot_mode:%d"
#define DC_MISC_450_112_2_18_1_23_32_124 "DCAM:DC_AdjRect_OnDistortion: src rect x:%d, y:%d,w:%d, h:%d"
#define DC_MISC_451_112_2_18_1_23_32_125 "DCAM:DC_AdjRect_OnDistortion: disp rect x:%d, y:%d,w:%d, h:%d"
#define DC_MISC_452_112_2_18_1_23_32_126 "DCAM:DC_AdjRect_OnDistortion: target rect x:%d, y:%d,w:%d, h:%d"
#define DC_MISC_566_112_2_18_1_23_32_127 "DCAM:DC_AdjRect_OnDistortion: modify src rect x:%d, y:%d,w:%d, h:%d"
#define DC_MISC_567_112_2_18_1_23_32_128 "DCAM:DC_AdjRect_OnDistortion: modify disp rect x:%d, y:%d,w:%d, h:%d"
#define DC_MISC_568_112_2_18_1_23_32_129 "DCAM:DC_AdjRect_OnDistortion: modify target rect x:%d, y:%d,w:%d, h:%d"
#define DC_MISC_587_112_2_18_1_23_32_130 "DCAM: DC_LcdPointMatchToImg: lcd rect x:%d, y:%d,w:%d, h:%d"
#define DC_MISC_658_112_2_18_1_23_32_131 "DCAM: DC_LcdPointMatchToImg: modify target rect x:%d, y:%d,w:%d, h:%d"
#define DC_MISC_1134_112_2_18_1_23_33_132 "DCAM: DC_Malloc, pContext->dc_buf_addr 0x%x"
#define DC_MISC_1135_112_2_18_1_23_33_133 "DCAM: DC_Malloc, pContext->thumbnail_buf_addr 0x%x, %dk"
#define DC_MISC_1136_112_2_18_1_23_33_134 "DCAM: DC_Malloc, pContext->exif_buf_addr 0x%x, %dk"
#define DC_MISC_1137_112_2_18_1_23_33_135 "DCAM: DC_Malloc, pContext->jpeg_buf_ptr 0x%x, %dk"
#define DC_MISC_1138_112_2_18_1_23_33_136 "DCAM: DC_Malloc, pContext->yyuv_buf.y_addr 0x%x, %dk"
#define DC_MISC_1139_112_2_18_1_23_33_137 "DCAM: DC_Malloc, pContext->quick_view_buf.y_addr 0x%x, %dk"
#define DC_MISC_1140_112_2_18_1_23_33_138 "DCAM: DC_Malloc, pContext->cx_buf_ptr 0x%x, %dk"
#define DC_MISC_1141_112_2_18_1_23_33_139 "DCAM: DC_Malloc, pContext->jpeg_fw_ptr 0x%x, %dk"
#define DC_MISC_1142_112_2_18_1_23_33_140 "DCAM: DC_Malloc, pContext->disp_buf_ptr 0x%x, %dk"
#define DC_MISC_1143_112_2_18_1_23_33_141 "DCAM: DC_Malloc, pContext->rot_buf_ptr 0x%x, %dk"
#define DC_MISC_1144_112_2_18_1_23_33_142 "DCAM: DC_Malloc, pContext->exif_swap_buf_addr 0x%x, %dk"
#define DC_MISC_1145_112_2_18_1_23_33_143 "DCAM: DC_Malloc, pContext->jpg_yuv_buf_addr 0x%x, %dk"
#define DC_MISC_1146_112_2_18_1_23_33_144 "DCAM: DC_Malloc, pContext->src_jpg_addr 0x%x, %dk"
#define DC_MISC_1147_112_2_18_1_23_33_145 "DCAM: DC_Malloc, pContext->minature_swap_buf_addr 0x%x, %dk"
#define DC_MISC_1148_112_2_18_1_23_33_146 "DCAM: DC_Malloc, pContext->total_buf_ptr 0x%x, %dk"
#define DC_MISC_1149_112_2_18_1_23_33_147 "DCAM: DC_Malloc, pContext->remain_buf_ptr 0x%x, %dk"
#define DC_MISC_1167_112_2_18_1_23_33_148 "dc_task,DC_SendMessage sig_code = %d"
#define DC_MISC_1197_112_2_18_1_23_34_149 "DCAM: DC_TaskEntry sig is null error"
#define DC_MISC_1203_112_2_18_1_23_34_150 "DCAM: DC_TaskEntry cmd % error"
#define DC_MISC_1219_112_2_18_1_23_34_151 "dc_task, DC_TASK_MSG_PREVIEW"
#define DC_MISC_1227_112_2_18_1_23_34_152 "DCAM: dc_task, DC_TASK_MSG_SNAPSHOT"
#define DC_MISC_1244_112_2_18_1_23_34_153 "dc_task, DC_TASK_MSG_DISPLAY"
#define DC_MISC_1267_112_2_18_1_23_34_154 "DCAM: dc_task, DCAM_MSG_BURST_SNAPSHOT"
#define DC_MISC_1275_112_2_18_1_23_34_155 "dc_task, DC_TASK_MSG_DECODE"
#define DC_MISC_1281_112_2_18_1_23_34_156 "dc_task, DC_TASK_MSG_MJPEG_CAPTURE"
#define DC_MISC_1287_112_2_18_1_23_34_157 "dc_task, DCAM_MSG_DISPLAY_DONE"
#define DC_MISC_1294_112_2_18_1_23_34_158 "dc_task, DC_TASK_MSG_OVERTIME"
#define DC_MISC_1300_112_2_18_1_23_34_159 "dc_task, DCAM_MSG_ZOOM_DONE"
#define DC_MISC_1314_112_2_18_1_23_34_160 "dc_task, invalid sig code to dc_task_Entry,line %d"
#define DC_MISC_1358_112_2_18_1_23_34_161 "DCAM_TaskOpen, task_id = %d"
#define DC_MISC_1366_112_2_18_1_23_34_162 "DCAM_TaskClose,task_id = %lu"
#define DC_MISC_1682_112_2_18_1_23_34_163 "DCAM: DC_GetCaptureMatchSize :%d"
#define DC_MISC_1740_112_2_18_1_23_35_164 "--DC_SetCaptureImgSensorMode:target_w =%d,pDcContext->snapshot_mode=%d,%d--"
#define DC_MISC_2182_112_2_18_1_23_35_165 "Hansen swap:%d"
#define DC_MISC_2185_112_2_18_1_23_35_166 "DCAM: GetYuvEncoderBufSize :sensor_w=%d, photo=%d, mem=%dk"
#define DC_MISC_2229_112_2_18_1_23_36_167 "DCAM: GetJpgEncoderBufSize :sensor_w=%d, photo=%d, mem=%dk"
#define DC_MISC_2269_112_2_18_1_23_36_168 "DCAM: DC_GetSensorSize mem total: %dk"
#define DC_MISC_2316_112_2_18_1_23_36_169 "Hansen: %d"
#define DC_MISC_2402_112_2_18_1_23_36_170 "DCAM: _DC_IsZoomSupported is not support !4P3"
#define DC_MISC_2411_112_2_18_1_23_36_171 "DCAM: _DC_IsZoomSupported is not support sensor:%d photo:%d"
#define DC_MISC_2609_112_2_18_1_23_36_172 "DCAM: DC_GetZoomMaxLevel start"
#define DC_MISC_2618_112_2_18_1_23_36_173 "DCAM: DC_GetZoomMaxLevel :%d"
#define DC_MISC_2644_112_2_18_1_23_36_174 "---DC_GetContinueShootMaxNum:%d,%d---"
#define DC_MISC_2712_112_2_18_1_23_36_175 "DCAM: DC_GetContinueShootMaxNum :%d"
#define DC_MISC_2876_112_2_18_1_23_37_176 "DCAM: DC_GetMemSize mode:%d"
#define DC_MISC_2979_112_2_18_1_23_37_177 "DCAM: DC_GetMemSize buf_size:0x%x, 0x%xk"
#define DC_PARAM_CTRL_159_112_2_18_1_23_37_178 "_SetSensorRot, Angle %d"
#define DC_PARAM_CTRL_215_112_2_18_1_23_38_179 "DCAM: _DCAMERA_SetPreviewEnviroment %d"
#define DC_PARAM_CTRL_219_112_2_18_1_23_38_180 "DCAM: _SetPreviewEnviroment %d error"
#define DC_PARAM_CTRL_241_112_2_18_1_23_38_181 "DCAM: _DCAMERA_SetPreviewBrightness %d"
#define DC_PARAM_CTRL_245_112_2_18_1_23_38_182 "DCAM: _DCAMERA_SetPreviewBrightness %d error"
#define DC_PARAM_CTRL_267_112_2_18_1_23_38_183 "DCAM: _DCAMERA_SetPreviewContrast %d"
#define DC_PARAM_CTRL_271_112_2_18_1_23_38_184 "DCAM: _DCAMERA_SetPreviewContrast %d error"
#define DC_PARAM_CTRL_293_112_2_18_1_23_38_185 "DCAM: _DCAMERA_SetPreviewSharpness %d"
#define DC_PARAM_CTRL_297_112_2_18_1_23_38_186 "DCAM: _SetPreviewSharpness %d error"
#define DC_PARAM_CTRL_319_112_2_18_1_23_38_187 "DCAM: _DCAMERA_SetPreviewSaturation %d"
#define DC_PARAM_CTRL_323_112_2_18_1_23_38_188 "DCAM: _SetPreviewSaturation %d error"
#define DC_PARAM_CTRL_345_112_2_18_1_23_38_189 "DCAM: _DCAMERA_SetPreviewEffect %d"
#define DC_PARAM_CTRL_349_112_2_18_1_23_38_190 "DCAM: _DCAMERA_SetPreviewEffect %d error"
#define DC_PARAM_CTRL_371_112_2_18_1_23_38_191 "DCAM: _DCAMERA_SetPreviewWBMode %d"
#define DC_PARAM_CTRL_375_112_2_18_1_23_38_192 "DCAM: _DCAMERA_SetPreviewWBMode %d error"
#define DC_PARAM_CTRL_397_112_2_18_1_23_38_193 "DCAM: _DCAMERA_SetPreviewVideoMode %d"
#define DC_PARAM_CTRL_401_112_2_18_1_23_38_194 "DCAM: _SetPreviewVideoMode %d error"
#define DC_PARAM_CTRL_425_112_2_18_1_23_38_195 "DCAM: _DCAMERA_AntiBandingFlicker %d"
#define DC_PARAM_CTRL_429_112_2_18_1_23_38_196 "DCAM: _DCAMERA_AntiBandingFlicker %d error"
#define DC_PARAM_CTRL_451_112_2_18_1_23_38_197 "DCAM: _DCAMERA_SetExposureCompensation %d"
#define DC_PARAM_CTRL_455_112_2_18_1_23_38_198 "DCAM: _SetExposureCompensation %d error"
#define DC_PARAM_CTRL_478_112_2_18_1_23_38_199 "DCAM: _zoom_callback %d"
#define DC_PARAM_CTRL_489_112_2_18_1_23_38_200 "DCAM: _zoom_callback error"
#define DC_PARAM_CTRL_507_112_2_18_1_23_38_201 "DCAM: _SetZoomLevel %d"
#define DC_PARAM_CTRL_520_112_2_18_1_23_38_202 "DCAM: _SetZoomLevel callback is pnull error!"
#define DC_PARAM_CTRL_585_112_2_18_1_23_38_203 "DCAM: _SetATVChn op_mode 0x%x, id 0x%x"
#define DC_PARAM_CTRL_623_112_2_18_1_23_38_204 "DCAM: _SetATVRegion 0x%x"
#define DC_PARAM_CTRL_644_112_2_18_1_23_38_205 "DCAM: _SetATVVolume 0x%x"
#define DC_PARAM_CTRL_712_112_2_18_1_23_39_206 "DCAM: _FlashCtrl 0x%x"
#define DC_PARAM_CTRL_756_112_2_18_1_23_39_207 "DCAM: _FocusCtrl : %d"
#define DC_PARAM_CTRL_760_112_2_18_1_23_39_208 "DCAM: _FocusCtrl param is null error"
#define DC_PARAM_CTRL_803_112_2_18_1_23_39_209 "DCAM: _ExposureCtrl : %d"
#define DC_PARAM_CTRL_807_112_2_18_1_23_39_210 "DCAM: _ExposureCtrl param is null error"
#define DC_PARAM_CTRL_849_112_2_18_1_23_39_211 "DCAM: _SetTargetSize %d"
#define DC_PARAM_CTRL_853_112_2_18_1_23_39_212 "DCAM: _SetTargetSize arg:%d error"
#define DC_PARAM_CTRL_859_112_2_18_1_23_39_213 "DCAM: _SetTargetSize - just record for not preview"
#define DC_PARAM_CTRL_871_112_2_18_1_23_39_214 "DCAM: _SetTargetSize - do nothing in preview"
#define DC_PARAM_CTRL_886_112_2_18_1_23_39_215 "DCAM: _SetTargetSize - need to change in preview"
#define DC_PARAM_CTRL_953_112_2_18_1_23_39_216 "DCAM: DC_GetParamCtrlFunc cmd %d error"
#define DC_PREVIEW_277_112_2_18_1_23_40_217 "DCAM: _stop_preview"
#define DC_PREVIEW_290_112_2_18_1_23_40_218 "DCAM: stop preview state err"
#define DC_PREVIEW_313_112_2_18_1_23_40_219 "DCAM: preview data format %x err!"
#define DC_PREVIEW_344_112_2_18_1_23_40_220 "DCAM: preview _get_isp_param display buf is null err!"
#define DC_PREVIEW_383_112_2_18_1_23_40_221 "DCAM: Panorama FourInOne preview distortion %d err!"
#define DC_REVIEW_203_112_2_18_1_23_41_222 "DCAM: pContext->review_param.disp_mode = %d"
#define DC_REVIEW_215_112_2_18_1_23_41_223 "DCAM: _adj_disp_mode - rotation mode: %d, disp size: %d, %d, %d, %d;  trim size: %d, %d, %d, %d"
#define DC_SNAPSHOT_140_112_2_18_1_23_41_224 "DCAM: %s, _check_status, DC is not open"
#define DC_SNAPSHOT_145_112_2_18_1_23_41_225 "DCAM: %s, _check_status, still preview, stop it"
#define DC_SNAPSHOT_170_112_2_18_1_23_41_226 "DCAM: capture file name is null err!"
#define DC_SNAPSHOT_203_112_2_18_1_23_41_227 "TIM_TEST: _DC_CalcPanoramaBuf 0x%x"
#define DC_SNAPSHOT_366_112_2_18_1_23_42_228 "DCAM: _set_parameters(),snap mode: 0x%x"
#define DC_SNAPSHOT_469_112_2_18_1_23_42_229 "DCAM: _DC_GetCaptureData(), eb=0x%x, callback:0x%x, line %d"
#define DC_SNAPSHOT_521_112_2_18_1_23_42_230 "_quick_review_jpeg(),jpeg decoding error,line %d"
#define DC_SNAPSHOT_635_112_2_18_1_23_42_231 "DCAM:_DC_QuickVeviewDirect"
#define DC_SNAPSHOT_665_112_2_18_1_23_42_232 "DCAM:_DC_SwitchPanoramaBuffer"
#define DC_SNAPSHOT_714_112_2_18_1_23_43_233 "DCAM:_DC_SetPanoramaParam the swap buf is not enough err!"
#define DC_SNAPSHOT_740_112_2_18_1_23_43_234 "DCAM:_DC_SetPanoramaEncoderParam err out_addr:0x%x, out_w:%d, out_h:%d"
#define DC_SNAPSHOT_746_112_2_18_1_23_43_235 "DCAM:_DC_SetPanoramaEncoder buf err quick_addr:0x%x, data_addr:0x%x"
#define DC_SNAPSHOT_834_112_2_18_1_23_43_236 "DCAM:_DC_PanoramaCapture"
#define DC_SNAPSHOT_874_112_2_18_1_23_43_237 "DCAM: Panorama capture call back is pnull err!"
#define DC_SNAPSHOT_951_112_2_18_1_23_43_238 "DCAM:_DC_SwitchFourInOneBuffer"
#define DC_SNAPSHOT_1004_112_2_18_1_23_43_239 "DCAM:_DC_SetFourInoneEncoder encoder err!"
#define DC_SNAPSHOT_1027_112_2_18_1_23_43_240 "trim rect x:%d, y:%d, w:%d, h:%d"
#define DC_SNAPSHOT_1028_112_2_18_1_23_43_241 "src_rect x:%d, y:%d, w:%d, h:%d"
#define DC_SNAPSHOT_1062_112_2_18_1_23_43_242 "DCAM: snapshot the data rect out of trim range"
#define DC_SNAPSHOT_1071_112_2_18_1_23_43_243 "dst_rect x:%d, y:%d, w:%d, h:%d"
#define DC_SNAPSHOT_1117_112_2_18_1_23_43_244 "DCAM: _DC_Proc_DispDate"
#define DC_SNAPSHOT_1121_112_2_18_1_23_43_245 "DCAM: org_dateimg_addr 0x%x, w: %d, h:%d"
#define DC_SNAPSHOT_1207_112_2_18_1_23_44_246 "DCAM: very org_dateimg_addr 0x%x, w: %d, h:%d"
#define DC_SNAPSHOT_1214_112_2_18_1_23_44_247 "DCAM: very target_img_addr 0x%x, w: %d, h:%d"
#define DC_SNAPSHOT_1262_112_2_18_1_23_44_248 "output date addr 0x%x, width %d, height %d"
#define DC_SNAPSHOT_1286_112_2_18_1_23_44_249 "cap date addr 0x%x, x %d y %dwidth %d, height %d"
#define DC_SNAPSHOT_1291_112_2_18_1_23_44_250 "brush date addr 0x%x, width %d, height %d"
#define DC_SNAPSHOT_1337_112_2_18_1_23_44_251 "scale datemark src zie x: %d,y: %d,w: %d,h: %d"
#define DC_SNAPSHOT_1382_112_2_18_1_23_44_252 "scale datemark dst zie x: %d,y: %d,w: %d,h: %d"
#define DC_SNAPSHOT_1405_112_2_18_1_23_44_253 "DCAM:_DC_FourInOneCapture"
#define DC_SNAPSHOT_1435_112_2_18_1_23_44_254 "DCAM: FourInOne capture combine call back is pnull err!"
#define DC_SNAPSHOT_1464_112_2_18_1_23_44_255 "DCAM:_do_snapshot capture type : %d!"
#define DC_SNAPSHOT_1468_112_2_18_1_23_44_256 "DCAM:_do_snapshot capture mode err!"
#define DC_SNAPSHOT_1475_112_2_18_1_23_44_257 "DCAM: capture before capture mode set err!"
#define DC_SNAPSHOT_1521_112_2_18_1_23_44_258 "DCAM: capture after capture mode set err!"
#define DC_SNAPSHOT_1540_112_2_18_1_23_44_259 "DCAM: _capture_normal"
#define DC_SNAPSHOT_1563_112_2_18_1_23_44_260 "DCAM: _capture_normal encoder err!"
#define DC_SNAPSHOT_1598_112_2_18_1_23_44_261 "DCAM: _capture_interpolation"
#define DC_SNAPSHOT_1611_112_2_18_1_23_44_262 "DCAM: _capture_interpolation encoder err!"
#define DC_SNAPSHOT_1626_112_2_18_1_23_44_263 "DCAM: _capture_jpeg"
#define DC_SNAPSHOT_1642_112_2_18_1_23_44_264 "DCAM: _do_capture_jpeg jpg stream is null err!"
#define DC_SNAPSHOT_1669_112_2_18_1_23_44_265 "DCAM: capture end jpg:0x%x, w:%d, h:%d, jpg_size:0x%x"
#define DC_SNAPSHOT_1677_112_2_18_1_23_44_266 "DCAM: capture end y:0x%x, uv:0x%x, w:%d, h:%d,"
#define DC_SNAPSHOT_1682_112_2_18_1_23_44_267 "DCAM: capture err!"
#define DC_SNAPSHOT_1716_112_2_18_1_23_45_268 "DCAM: DC_GetImageFromLCDC"
#define DC_SNAPSHOT_1789_112_2_18_1_23_45_269 "DCAM: _capture_personal"
#define DC_SNAPSHOT_1883_112_2_18_1_23_45_270 "DCAM: _capture_personal encoder err!"
#define DC_SNAPSHOT_1934_112_2_18_1_23_45_271 "DCAM: _capture_zoom encoder err!"
#define DC_SNAPSHOT_1945_112_2_18_1_23_45_272 "DCAM: _capture_zoom slice encoder err!"
#define DC_SNAPSHOT_2045_112_2_18_1_23_45_273 "DCAM: continue shot before capture mode set err!"
#define DC_SNAPSHOT_2068_112_2_18_1_23_45_274 "DCAM: _capture_burst capture err"
#define DC_SNAPSHOT_2086_112_2_18_1_23_45_275 "DCAM: _capture_burst quick view err"
#define DC_SNAPSHOT_2096_112_2_18_1_23_45_276 "DCAM: _capture_burst rotation err"
#define DC_SNAPSHOT_2143_112_2_18_1_23_45_277 "DCAM: _capture_burst encoder err!"
#define DC_SNAPSHOT_2166_112_2_18_1_23_45_278 "DCAM: continue shot after capture mode set err!"
#define DC_SNAPSHOT_2446_112_2_18_1_23_46_279 "DCAM: quick view target w: %d h: %d"
#define DC_SNAPSHOT_2518_112_2_18_1_23_46_280 "DCAM: _quick_review jpg sample format err!"
#define DC_SNAPSHOT_2547_112_2_18_1_23_46_281 "_capture_review"
#define DC_SNAPSHOT_2732_112_2_18_1_23_47_282 "DCAM: continue shot capture mode %d err!"
#define JPEG_71_112_2_18_1_23_47_283 "[JPEG_GetMCUSize] unsupport quality"
#define JPEG_122_112_2_18_1_23_47_284 "JPEG_EncodeJpeg ----start, %d"
#define JPEG_191_112_2_18_1_23_47_285 "JPEG_EncodeJpeg ----end :%d"
#define JPEG_214_112_2_18_1_23_47_286 "JPEG_DecodeJpeg ----start, %d"
#define JPEG_257_112_2_18_1_23_47_287 "JPEG_DecodeJpeg ----unsupported yuv format"
#define JPEG_327_112_2_18_1_23_48_288 "JPEG_DecodeJpeg ----unsupported yuv format"
#define JPEG_336_112_2_18_1_23_48_289 "JPEG_DecodeJpeg : y_chn_addr = 0x%x,u_chn_addr = 0x%x,v_chn_addr = 0x%x"
#define JPEG_338_112_2_18_1_23_48_290 "JPEG_DecodeJpeg ----end :%d"
#define JPEG_351_112_2_18_1_23_48_291 "DC: JPEG_ZoomJpeg -> start time = %d"
#define JPEG_380_112_2_18_1_23_48_292 "DC: JPEG_ZoomJpeg -> end time = %d, rVal = %d"
#define JPG_DECODE_245_112_2_18_1_23_50_293 "JPEG_Decode,g_JpegCodec.ratio[0].h_ratio = %d, g_JpegCodec.ratio[0].v_ratio = %d"
#define DC_CFG_183_112_2_18_1_23_57_294 "_DC_SetExifSpecificParameter %s"
#define DC_CFG_252_112_2_18_1_23_58_295 "DC_SetExifImageDataTime %s"
#define DC_CFG_266_112_2_18_1_23_58_296 "DC_GetExifParameter"
#define DC_COMMON_174_112_2_18_1_23_58_297 "DCAM: unsupport channel format"
#define DC_COMMON_211_112_2_18_1_23_58_298 "DCAM: DC_ArgbOverlayer src_buf_rect y=%d"
#define DC_COMMON_212_112_2_18_1_23_58_299 "DCAM: DC_ArgbOverlayer src_buf_rect h=%d"
#define DC_COMMON_213_112_2_18_1_23_58_300 "DCAM: DC_ArgbOverlayer overlay_rect.y=%d"
#define DC_COMMON_214_112_2_18_1_23_58_301 "DCAM: DC_ArgbOverlayer overlay_rect.h=%d"
#define DC_COMMON_220_112_2_18_1_23_58_302 "DCAM: DC_ArgbOverlayer error src_addr:x=%d, w=%d"
#define DC_COMMON_256_112_2_18_1_23_58_303 "DCAM: DC_ArgbOverlayer src_buf_rect x =%d y=%d"
#define DC_COMMON_257_112_2_18_1_23_58_304 "DCAM: DC_ArgbOverlayer src_buf_rect w =%d h=%d"
#define DC_COMMON_258_112_2_18_1_23_58_305 "DCAM: DC_ArgbOverlayer overlay_rect.x =%d y=%d"
#define DC_COMMON_259_112_2_18_1_23_58_306 "DCAM: DC_ArgbOverlayer overlay_rect.w =%d h=%d"
#define DC_COMMON_393_112_2_18_1_23_59_307 "DCAMERA_Convert_Coor,src x:%d, y:%d, w:%d, h:%d"
#define DC_COMMON_394_112_2_18_1_23_59_308 "DCAMERA_Convert_Coor,src degree:%d"
#define DC_COMMON_428_112_2_18_1_23_59_309 "DCAMERA_Convert_Coor,dst x:%d, y:%d, w:%d, h:%d"
#define DC_COMMON_429_112_2_18_1_23_59_310 "DCAMERA_Convert_Coor,dst degree:%d"
#define DCAMERA_APP_385_112_2_18_1_24_0_311 "DC: GetQueueAvailable queue info:enqueued:%d, store: %d, count: %d"
#define DCAMERA_APP_505_112_2_18_1_24_0_312 "DC: DCAMERA_GetMpegRect, size {%d %d} rect {%d %d %d %d}"
#define DCAMERA_APP_555_112_2_18_1_24_0_313 "_DCAMERA_GetMutex"
#define DCAMERA_APP_558_112_2_18_1_24_0_314 "_DCAMERA_GetMutex: Mutex is NULL!"
#define DCAMERA_APP_570_112_2_18_1_24_0_315 "_DCAMERA_PutMutex"
#define DCAMERA_APP_573_112_2_18_1_24_0_316 "_DCAMERA_PutMutex: Mutex is NULL!"
#define DCAMERA_APP_593_112_2_18_1_24_0_317 "_DCAMERA_DcamRect2ISPRect,src x y w h {%d %d %d %d},rotate_ang %d"
#define DCAMERA_APP_654_112_2_18_1_24_0_318 "_DCAMERA_DcamRect2ISPRect,wrong angle"
#define DCAMERA_APP_665_112_2_18_1_24_0_319 "_DCAMERA_DcamRect2ISPRect,dst x y w h {%d %d %d %d}"
#define DCAMERA_APP_683_112_2_18_1_24_0_320 "_DCamera_AdjustRect: fullview"
#define DCAMERA_APP_713_112_2_18_1_24_0_321 "_DCamera_AdjustRect: fullscreen "
#define DCAMERA_APP_753_112_2_18_1_24_0_322 "_DCamera_AdjustRect: any trimming and scaling"
#define DCAMERA_APP_763_112_2_18_1_24_0_323 "_DCamera_AdjustRect,trim rect x,y,w,h {%d %d %d %d}"
#define DCAMERA_APP_878_112_2_18_1_24_1_324 "DCAMERA_SetSensorType: %d"
#define DCAMERA_APP_928_112_2_18_1_24_1_325 "_DCAMERA_InitSensor, sensor %d is not exist"
#define DCAMERA_APP_933_112_2_18_1_24_1_326 "_DCAMERA_InitSensor, No sensor exist"
#define DCAMERA_APP_976_112_2_18_1_24_1_327 "DC: ISP output one frame in capture at %d"
#define DCAMERA_APP_981_112_2_18_1_24_1_328 "DC: yaddr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define DCAMERA_APP_993_112_2_18_1_24_1_329 "DC: Exit ISP CAPTURE FRAME callback at %d"
#define DCAMERA_APP_1008_112_2_18_1_24_1_330 "_DCAMERA_ISP_SCALE_CALLBACK: FLAG 0x%08x"
#define DCAMERA_APP_1081_112_2_18_1_24_1_331 "UPCC: JPEG ENCODE ERR!!"
#define DCAMERA_APP_1089_112_2_18_1_24_1_332 "UPCC_Camera.c->UPCC_GetAndSendFrame(): one frame lost."
#define DCAMERA_APP_1100_112_2_18_1_24_1_333 "DCAMERA_app.c->DCAMERA_RecordCallback()  NO call back !!"
#define DCAMERA_APP_1177_112_2_18_1_24_1_334 "DC: _DCAMERA_DoSnapShotExDecJpgFrame, snapshot_ex_yuv  0x%x  snapshot_ex_addi_mem 0x%x"
#define DCAMERA_APP_1183_112_2_18_1_24_1_335 "DC: _DCAMERA_DoSnapShotExDecJpgFrame, s_dc_info_ptr->snapshot_ex_yuv is NULL! "
#define DCAMERA_APP_1257_112_2_18_1_24_1_336 "DC: _DCAMERA_DoSnapShotDecJpgFrame Entry Time = %d"
#define DCAMERA_APP_1317_112_2_18_1_24_2_337 "DC: _DCAMERA_DoSnapShotDecJpgFrame End Time = %d"
#define DCAMERA_APP_1341_112_2_18_1_24_2_338 "DC _DCAMERA_DoSnapShotReview, start time %d  "
#define DCAMERA_APP_1393_112_2_18_1_24_2_339 "DC _DCAMERA_DoSnapShotReview , disp_rect {%d %d}, src {%d %d}"
#define DCAMERA_APP_1419_112_2_18_1_24_2_340 "DC _DCAMERA_DoSnapShotReview , zoom %d , trim {%d %d}"
#define DCAMERA_APP_1483_112_2_18_1_24_2_341 "DC _DCAMERA_DoSnapShotReview end, end time %d  "
#define DCAMERA_APP_1499_112_2_18_1_24_2_342 "DC _DCAMERA_DoSnapShotRotation, need rotation"
#define DCAMERA_APP_1559_112_2_18_1_24_2_343 "DC _DCAMERA_DoSnapShotRotation end, end time %d  "
#define DCAMERA_APP_1686_112_2_18_1_24_2_344 "DC: Call JPEG_EncodeJpeg, start time = %d"
#define DCAMERA_APP_1691_112_2_18_1_24_2_345 "y_addr = 0x%x,u_addr = 0x%x,v_addr = 0x%x,jpeg_buf_ptr = 0x%x"
#define DCAMERA_APP_1806_112_2_18_1_24_2_346 "DC: Call IMGJPEG_EncodeEx,Error"
#define DCAMERA_APP_1835_112_2_18_1_24_3_347 "DC: Call JPEG_EncodeJpeg, end time = %d"
#define DCAMERA_APP_1888_112_2_18_1_24_3_348 "DC: _DCAMERA_HandleSensorTargetSize -> Target width = %d, target height = %d"
#define DCAMERA_APP_1894_112_2_18_1_24_3_349 "DC: _DCAMERA_HandleSensorTargetSize -> Need Interpolation !!!!!! "
#define DCAMERA_APP_1905_112_2_18_1_24_3_350 "DC: _DCAMERA_HandleSensorTargetSize -> search_width = %d"
#define DCAMERA_APP_1913_112_2_18_1_24_3_351 "DC: _DCAMERA_HandleSensorTargetSize -> width = %d"
#define DCAMERA_APP_1936_112_2_18_1_24_3_352 "DC: _DCAMERA_HandleSensorTargetSize,can't find the right mode"
#define DCAMERA_APP_1943_112_2_18_1_24_3_353 "DC: _DCAMERA_HandleSensorTargetSize -> preview_m = %d, snapshot_m = %d"
#define DCAMERA_APP_1951_112_2_18_1_24_3_354 "DC: _DCAMERA_HandleSensorTargetSize -> snap_width = %d, snap_height = %d"
#define DCAMERA_APP_1963_112_2_18_1_24_3_355 "DC: _DCAMERA_HandleSensorTargetSize -> preview_width = %d, preview_height = %d"
#define DCAMERA_APP_1996_112_2_18_1_24_3_356 "DC: _DCAMERA_HandleSensorTargetSize ->(preview) zoom width step = %d, zoom height step = %d"
#define DCAMERA_APP_2000_112_2_18_1_24_3_357 "DC: _DCAMERA_HandleSensorTargetSize ->(snapshot) zoom width step = %d, zoom height step = %d"
#define DCAMERA_APP_2112_112_2_18_1_24_3_358 "_DCAMERA_SetPreviewMeterMOde: meter mode of sensor error,mode = %d"
#define DCAMERA_APP_2281_112_2_18_1_24_3_359 "_DCAMERA_SetSensorRot, Angle %d"
#define DCAMERA_APP_2314_112_2_18_1_24_3_360 "_DCAMERA_ROT_Convertor: %d"
#define DCAMERA_APP_2350_112_2_18_1_24_4_361 "_DCAMERA_ROT_Convertor: img: %d, ret: %d"
#define DCAMERA_APP_2441_112_2_18_1_24_4_362 "DCAM: _DCAMERA_CheckTargetSize, photo size: width = %d, height = %d"
#define DCAMERA_APP_2445_112_2_18_1_24_4_363 "DCAM: invalid input arg"
#define DCAMERA_APP_2518_112_2_18_1_24_4_364 "_DCAMERA_SetPreviewSubZoom last preview x = %d y = %d, w = %d h = %d"
#define DCAMERA_APP_2541_112_2_18_1_24_4_365 "_DCAMERA_SetPreviewZoom : sub zoom step level %d"
#define DCAMERA_APP_2604_112_2_18_1_24_4_366 "_DCAMERA_SetPreviewZoom : sub zoom step level %d"
#define DCAMERA_APP_2676_112_2_18_1_24_4_367 "_DCAMERA_SetVTZoom, zoom level setting ok"
#define DCAMERA_APP_2711_112_2_18_1_24_4_368 "_DCAMERA_SetVTZoom: level = %d, x = %d y = %d, w = %d h = %d"
#define DCAMERA_APP_2745_112_2_18_1_24_4_369 "_DCAMERA_SetMpegZoom, zoom level setting ok"
#define DCAMERA_APP_2753_112_2_18_1_24_4_370 "_DCAMERA_SetMpegZoom, Failed to get MPEG parameter"
#define DCAMERA_APP_2781_112_2_18_1_24_4_371 "_DCAMERA_SetMpegZoom: level = %d, x = %d y = %d, w = %d h = %d"
#define DCAMERA_APP_2823_112_2_18_1_24_4_372 "_DCAMERA_SetPreviewZoom, dc_info_ptr->preview_ctl_info.zoom_level = %d, zoom_level = %d"
#define DCAMERA_APP_2844_112_2_18_1_24_4_373 "_DCAMERA_SetPreviewZoom, wrong preview mode %d"
#define DCAMERA_APP_2899_112_2_18_1_24_5_374 "_DCAMERA_SetPreviewZoom, logical coordinate ,lcd_rect {%d %d %d %d},disp_range {%d %d %d %d}, rot %d"
#define DCAMERA_APP_2922_112_2_18_1_24_5_375 "_DCAMERA_SetPreviewZoom : disp_range = %d %d %d %d"
#define DCAMERA_APP_2958_112_2_18_1_24_5_376 "DC ZOOM: level = %d, x = %d y = %d, w = %d h = %d, rotation = %d"
#define DCAMERA_APP_3007_112_2_18_1_24_5_377 "_DCAMERA_SetPreviewEXZoom, dc_info_ptr->preview_ctl_info.zoom_level = %d, zoom_level = %d"
#define DCAMERA_APP_3122_112_2_18_1_24_5_378 "DCAM: _DCAMERA_SetATVChn op_mode 0x%x, id 0x%x"
#define DCAMERA_APP_3150_112_2_18_1_24_5_379 "DCAM: _DCAMERA_SetATVRegion 0x%x"
#define DCAMERA_APP_3170_112_2_18_1_24_5_380 "DCAM: _DCAMERA_SetATVVolume 0x%x"
#define DCAMERA_APP_3199_112_2_18_1_24_5_381 "DC: _DCAMERA_SetPreviewFlag -> is_preview = %d"
#define DCAMERA_APP_3227_112_2_18_1_24_5_382 "DC: _DCAMERA_SetPreviewExFlag -> is_preview = %d"
#define DCAMERA_APP_3401_112_2_18_1_24_6_383 "DCAMERA_CheckPreviewParam:: Don't need to check the %d cmd and %d arg"
#define DCAMERA_APP_3407_112_2_18_1_24_6_384 "DCAMERA_CheckPreviewParam:: Error"
#define DCAMERA_APP_3430_112_2_18_1_24_6_385 "DC: _DCAMERA_PreviewControl -> cmd = %d, arg = %d"
#define DCAMERA_APP_3519_112_2_18_1_24_6_386 "DC: _DCAMERA_PreviewControl: error cmd %d"
#define DCAMERA_APP_3523_112_2_18_1_24_6_387 "DC: _DCAMERA_PreviewControl, not in preview"
#define DCAMERA_APP_3677_112_2_18_1_24_6_388 "DC: _DCAMERA_PreviewControl: error cmd %d"
#define DCAMERA_APP_3713_112_2_18_1_24_6_389 "_DCAMERA_ImageYUV422ToYUV420 ,size_each_line %d, height %d"
#define DCAMERA_APP_3821_112_2_18_1_24_6_390 "_DCAMERA_ImageScale ,unsupported output format %d"
#define DCAMERA_APP_3861_112_2_18_1_24_7_391 "_DCAMERA_JpgAddExif"
#define DCAMERA_APP_3940_112_2_18_1_24_7_392 "_DCAMERA_JpgAddExif, Failed to encode thumbnail"
#define DCAMERA_APP_3970_112_2_18_1_24_7_393 "_DCAMERA_JpgAddExif, Failed to write exif information , %d"
#define DCAMERA_APP_3977_112_2_18_1_24_7_394 "_DCAMERA_JpgAddExif, jpg buffer 0x%x, lenght %d"
#define DCAMERA_APP_4020_112_2_18_1_24_7_395 "DC: DCAMERA_DoBurstSnapshot -> start time = %d,image_count = %d"
#define DCAMERA_APP_4044_112_2_18_1_24_7_396 "DC: DoBurstSnapshot -> save in buffer "
#define DCAMERA_APP_4073_112_2_18_1_24_7_397 "DC: DCAMERA_DoBurstSnapshot - > Snapshot single err %d"
#define DCAMERA_APP_4096_112_2_18_1_24_7_398 "DC: DCAMERA_DoBurstSnapshot - > interval time of frame %d"
#define DCAMERA_APP_4113_112_2_18_1_24_7_399 "DC: DCAMERA_DoBurstSnapshot -> end time = %d"
#define DCAMERA_APP_4326_112_2_18_1_24_7_400 "DC: _DCAMERA_TaskRoutine() don't handle %d command"
#define DCAMERA_APP_4479_112_2_18_1_24_8_401 "DC: _DCAMERA_RotateYUV420, rot_mode=%d,format=%d,img_size.w,h %d %d,"
#define DCAMERA_APP_4486_112_2_18_1_24_8_402 "DC: _DCAMERA_RotateYUV420,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
#define DCAMERA_APP_4494_112_2_18_1_24_8_403 "DC: _DCAMERA_RotateYUV420-Y, SUCCESS"
#define DCAMERA_APP_4499_112_2_18_1_24_8_404 "DC: _DCAMERA_RotateYUV420-Y, FAIL"
#define DCAMERA_APP_4525_112_2_18_1_24_8_405 "DC: _DCAMERA_RotateYUV422, rot_mode=%d,format=%d,img_size.w,h %d %d,"
#define DCAMERA_APP_4532_112_2_18_1_24_8_406 "DC: _DCAMERA_RotateYUV422,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
#define DCAMERA_APP_4540_112_2_18_1_24_8_407 "DC: _DCAMERA_RotateYUV422, SUCCESS"
#define DCAMERA_APP_4545_112_2_18_1_24_8_408 "DC: _DCAMERA_RotateYUV422, FAIL"
#define DCAMERA_APP_4579_112_2_18_1_24_8_409 "DC: _DCAMERA_RotateRGB565, rot_mode=%d,format=%d,img_size.w,h %d %d,"
#define DCAMERA_APP_4586_112_2_18_1_24_8_410 "DC: _DCAMERA_RotateRGB565,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
#define DCAMERA_APP_4591_112_2_18_1_24_8_411 "DC: _DCAMERA_RotateRGB565, SUCCESS"
#define DCAMERA_APP_4596_112_2_18_1_24_8_412 "DC: _DCAMERA_RotateRGB565, FAIL"
#define DCAMERA_APP_4704_112_2_18_1_24_8_413 "DC: few memory, reduce the output mode of sensor"
#define DCAMERA_APP_4727_112_2_18_1_24_8_414 "DC: few memory, scaling down the image size of one special output mode"
#define DCAMERA_APP_4764_112_2_18_1_24_8_415 "DC: _DCAMERA_GetCapability, photo size %d"
#define DCAMERA_APP_4908_112_2_18_1_24_9_416 "_DCAMERA_PreviewSensorSet:meter mode of sensor error,mode = %d"
#define DCAMERA_APP_5012_112_2_18_1_24_9_417 "_DCAMERA_PreviewSensorSet :: brightness %d, contrast %d, sharpness %d, sturation %d, effect %d, envir %d, wb_mode %d,zoom lel %d sensor_rot %d"
#define DCAMERA_APP_5061_112_2_18_1_24_9_418 "DCAMERA_Open, mode %d"
#define DCAMERA_APP_5068_112_2_18_1_24_9_419 "-----open fmark------"
#define DCAMERA_APP_5088_112_2_18_1_24_9_420 "DC: DCAMERA_Open, memory error!"
#define DCAMERA_APP_5174_112_2_18_1_24_9_421 "DCAMERA_Close"
#define DCAMERA_APP_5179_112_2_18_1_24_9_422 "-----close fmark------"
#define DCAMERA_APP_5214_112_2_18_1_24_9_423 "DCAMERA_Close"
#define DCAMERA_APP_5337_112_2_18_1_24_9_424 "DC: DCAMERA_GetInfo, sensor size 0x%x"
#define DCAMERA_APP_5614_112_2_18_1_24_10_425 "DC: _DCAMERA_FFSSave, p_data_buf 0x%x buf_len 0x%x"
#define DCAMERA_APP_5643_112_2_18_1_24_10_426 "DC: image_notice_callback, image count = %d, data_type %d, data_pattern %d ,width %d height %d"
#define DCAMERA_APP_5649_112_2_18_1_24_10_427 "DC: image_notice_callback, p_user 0x%x, address 0x%x,0x%x,0x%x"
#define DCAMERA_APP_5711_112_2_18_1_24_10_428 "DC: preview_ex_display_callback, rtn %d ,p_param %d"
#define DCAMERA_APP_5721_112_2_18_1_24_10_429 "Src 0x%x 0x%x {%d %d}, Dst 0x%x 0x%x {%d %d}"
#define DCAMERA_APP_5817_112_2_18_1_24_10_430 "DC: DCAMERA_StartPreview -> start time = %d"
#define DCAMERA_APP_5849_112_2_18_1_24_10_431 "DC: display region , x %d,y %d,w %d,h %d"
#define DCAMERA_APP_5943_112_2_18_1_24_11_432 "DC: DCAMERA_StartPreview -> end time = %d, return value %d"
#define DCAMERA_APP_5961_112_2_18_1_24_11_433 "DC: _DCAMERA_PreviewCheckParam disp_mode %d, rot %d, disp_rect {%d %d %d %d}"
#define DCAMERA_APP_5997_112_2_18_1_24_11_434 "DC: _DCAMERA_PreviewEXCheckParam image frame count %d"
#define DCAMERA_APP_6011_112_2_18_1_24_11_435 "DC: _DCAMERA_PreviewEXCheckParam image size errror w,h %d %d"
#define DCAMERA_APP_6021_112_2_18_1_24_11_436 "DC: _DCAMERA_PreviewEXCheckParam image data type errror %d"
#define DCAMERA_APP_6027_112_2_18_1_24_11_437 "DC: _DCAMERA_PreviewEXCheckParam slice scaling for YUV420 is unsupported"
#define DCAMERA_APP_6036_112_2_18_1_24_11_438 "DC: _DCAMERA_PreviewEXCheckParam image %d,width %d height %d,data type %d Rot %d"
#define DCAMERA_APP_6180_112_2_18_1_24_11_439 "_DCAMERA_PreviewDisplayCallback, frame 0x%x, yaddr 0x%x, uaddr 0x%x"
#define DCAMERA_APP_6185_112_2_18_1_24_11_440 "_DCAMERA_PreviewDisplayCallback, Source Frame locked! 0x%x"
#define DCAMERA_APP_6192_112_2_18_1_24_11_441 "_DCAMERA_PreviewDisplayCallback, Destination Frame locked! 0x%x"
#define DCAMERA_APP_6223_112_2_18_1_24_11_442 "_DCAMERA: drop the current frame"
#define DCAMERA_APP_6252_112_2_18_1_24_11_443 "DC: DCAMERA_StartPreviewEX -> start time = %d"
#define DCAMERA_APP_6282_112_2_18_1_24_11_444 "DC: DCAMERA_StartPreviewEX, preview parameter error"
#define DCAMERA_APP_6370_112_2_18_1_24_11_445 "DC: DCAMERA_StartPreviewEX, preview parameter, preview size %d %d"
#define DCAMERA_APP_6401_112_2_18_1_24_11_446 "DC: DCAMERA_StartPreviewEX, preview parameter, encode_size %d %d"
#define DCAMERA_APP_6507_112_2_18_1_24_12_447 "DC: DCAMERA_StartPreviewEX, preview parameter error"
#define DCAMERA_APP_6576_112_2_18_1_24_12_448 "DC: DCAMERA_StartPreviewEX, no memory!"
#define DCAMERA_APP_6583_112_2_18_1_24_12_449 "DC: DCAMERA_StartPreviewEX, p_preview_buf 0x%x, preview_buf_len %d"
#define DCAMERA_APP_6599_112_2_18_1_24_12_450 "DC: DCAMERA_StartPreviewEX, No memory! Total 0x%x"
#define DCAMERA_APP_6622_112_2_18_1_24_12_451 "DCAMERA_StartPreviewEX, disp src frame %d,0x%x, {0x%x 0x%x} ,pointer 0x%x 0x%x"
#define DCAMERA_APP_6646_112_2_18_1_24_12_452 "DCAMERA_StartPreviewEX, disp dst frame %d,0x%x, {0x%x 0x%x} ,pointer 0x%x 0x%x"
#define DCAMERA_APP_6677_112_2_18_1_24_12_453 "DC: DCAMERA_StartPreviewEX, ISP Error!"
#define DCAMERA_APP_6686_112_2_18_1_24_12_454 "DC: _DCAMERA_GetScaledImageData width height %d %d, addr 0x%x 0x%x"
#define DCAMERA_APP_6844_112_2_18_1_24_12_455 "_DCAMERA_DoImageScaleInVT, Scaling parameter ERROR!"
#define DCAMERA_APP_6860_112_2_18_1_24_12_456 "DC: DCAMERA_StopPreview"
#define DCAMERA_APP_6895_112_2_18_1_24_12_457 "DCAMERA_StopPreview,stop error,preview is already been stop"
#define DCAMERA_APP_6992_112_2_18_1_24_13_458 "_DCAMERA_YUV422DownSample2YUV420 ,size_each_line %d, height %d"
#define DCAMERA_APP_7102_112_2_18_1_24_13_459 "_DCAMERA_GetSpecificImage, JPEG buf 0x%x, length 0x%x mem_start  0x%x "
#define DCAMERA_APP_7123_112_2_18_1_24_13_460 "DC, Add exif error %d"
#define DCAMERA_APP_7185_112_2_18_1_24_13_461 "DC: Call JPEG_EncodeJpeg, start time = %d,jpeg_buf_ptr = 0x%x"
#define DCAMERA_APP_7233_112_2_18_1_24_13_462 "DC: Call JPEG_EncodeJpeg, end time = %d"
#define DCAMERA_APP_7322_112_2_18_1_24_13_463 "_DCAMERA_GetSpecificImage, YUV no transformed ,y 0x%x, u 0x%x "
#define DCAMERA_APP_7347_112_2_18_1_24_13_464 "_DCAMERA_SnapshotExFreeMem , free s_dc_info_ptr->snapshot_ex_yuv 0x%x, length %d"
#define DCAMERA_APP_7356_112_2_18_1_24_13_465 "_DCAMERA_SnapshotExFreeMem , free s_dc_info_ptr->snapshot_ex_addi_mem 0x%x, length %d"
#define DCAMERA_APP_7419_112_2_18_1_24_13_466 "DC _DCAMERA_DoSnapShotExReview, start time %d  "
#define DCAMERA_APP_7486_112_2_18_1_24_14_467 "DC _DCAMERA_DoSnapShotExReview end, end time %d  "
#define DCAMERA_APP_7522_112_2_18_1_24_14_468 "DC: _DCAMERA_DoSnapshotEx -> start time = %d,image_count = %d,exif_need %d"
#define DCAMERA_APP_7634_112_2_18_1_24_14_469 "DC: _DCAMERA_DoSnapshotEx  image %d,width %d height %d,data type %d, Rot %d, mem_size 0x%x"
#define DCAMERA_APP_7661_112_2_18_1_24_14_470 "DC: _DCAMERA_DoSnapshotEx, target image buffer 0x%x size %d"
#define DCAMERA_APP_7689_112_2_18_1_24_14_471 "DC: _DCAMERA_DoSnapshotEx, no need to alloc yuv source buffer!"
#define DCAMERA_APP_7716_112_2_18_1_24_14_472 "DC: _DCAMERA_DoSnapshotEx, YUV buffer for the middle image 0x%x, length 0x%x"
#define DCAMERA_APP_7735_112_2_18_1_24_14_473 "DC: DCAMERA_GetAFrameFromISP ERROR"
#define DCAMERA_APP_7810_112_2_18_1_24_14_474 "DC: _DCAMERA_DoSnapshotEx reuse_image_id %d width  height %d %d "
#define DCAMERA_APP_7849_112_2_18_1_24_14_475 "DC: _DCAMERA_DoSnapshotEx - > interval time of frame %d"
#define DCAMERA_APP_7883_112_2_18_1_24_14_476 "DC: _DCAMERA_DoSnapshotEx -> end time = %d, rtn_code %d"
#define DCAMERA_APP_7898_112_2_18_1_24_14_477 "DC: _DCAMERA_DoSnapshotEx,target frame count or snapshot number error"
#define DCAMERA_APP_7906_112_2_18_1_24_14_478 "DC: _DCAMERA_DoSnapshotEx,data type of %d-th target_info error"
#define DCAMERA_APP_7914_112_2_18_1_24_14_479 "DC: _DCAMERA_DoSnapshotEx,target size exceed the largest size output from sensor"
#define DCAMERA_APP_7921_112_2_18_1_24_14_480 "DC: _DCAMERA_DoSnapshotEx,target size must be aligned by 4 pixels"
#define DCAMERA_APP_7928_112_2_18_1_24_14_481 "DC: _DCAMERA_DoSnapshotEx,Rotation mode not supported for RAW data"
#define DCAMERA_APP_7967_112_2_18_1_24_15_482 "DC: DCAMERA_DoSnapshotEX,YUV420 slice scaling is not supported"
#define DCAMERA_APP_8069_112_2_18_1_24_15_483 "DCAMERA_ParamCtrl: dc is not open!"
#define DCAMERA_APP_8125_112_2_18_1_24_15_484 "DC DCAMERA_ReviewJpeg:start"
#define DCAMERA_APP_8139_112_2_18_1_24_15_485 "DC : DCAMERA_ReviewJpeg -> start time = %d"
#define DCAMERA_APP_8236_112_2_18_1_24_15_486 "DC : ISP_ServiceSetFrameMemory yaddr = 0x%x, uaddr = 0x%x,vaddr = 0x%x"
#define DCAMERA_APP_8279_112_2_18_1_24_16_487 "DC: review JPEG -> trimming x = %d, y = %d, w = %d, h = %d, rotation %d, distortion %d"
#define DCAMERA_APP_8326_112_2_18_1_24_16_488 "DC : DCAMERA_ReviewJpeg -> end time = %d"
#define DCAMERA_APP_8348_112_2_18_1_24_16_489 "DCAMERA_StartRecord: Start time %d"
#define DCAMERA_APP_8356_112_2_18_1_24_16_490 "DCAMERA_ParamCtrl: dc is not open!"
#define DCAMERA_APP_8564_112_2_18_1_24_16_491 "DCAMERA_StartRecord: End time %d, return value %d "
#define DCAMERA_APP_8579_112_2_18_1_24_16_492 "DC: DCAMERA_StopRecord"
#define DCAMERA_APP_8692_112_2_18_1_24_17_493 "DC_SetCurImgSensorId, sensor_id %d"
#define DCAMERA_APP_8730_112_2_18_1_24_17_494 "DC: DCAMERA_Open, memory error!"
#define DCAMERA_APP_8854_112_2_18_1_24_17_495 "DC: _DCAMERA_BackupPreviewbuf2ReviewBuf quick_view_size 0x%x, width %d blk_info.end_y %d blk_info.start_y %d"
#define DCAMERA_APP_8868_112_2_18_1_24_17_496 "DC: _DCAMERA_BackupPreviewbuf2ReviewBuf dst 0x%x, length %d, src y 0x%x"
#define DCAMERA_APP_8960_112_2_18_1_24_18_497 "_DCAMERA_CapMemAssign, captured size %d %d"
#define DCAMERA_APP_9040_112_2_18_1_24_18_498 "DC: few memory, reduce the output mode of sensor"
#define DCAMERA_APP_9151_112_2_18_1_24_18_499 "_DCAMERA_GetCaptureTrimRect, src {%d %d}, dst {%d %d},trim {%d %d %d %d},zoom_step {%d %d"
#define DCAMERA_APP_9166_112_2_18_1_24_18_500 "_DCAMERA_GetCaptureTrimRect,p_trim_rect {%d %d %d %d}"
#define DCAMERA_APP_9178_112_2_18_1_24_18_501 "_DCAMERA_GetScaledData, width %d, height %d, y addr 0x%x, uv addr 0x%x"
#define DCAMERA_APP_9222_112_2_18_1_24_18_502 "_DCAMERA_GetDateRectParam: trim rect x:%d, y:%d, w:%d, h:%d"
#define DCAMERA_APP_9223_112_2_18_1_24_18_503 "_DCAMERA_GetDateRectParam: src_rect x:%d, y:%d, w:%d, h:%d"
#define DCAMERA_APP_9295_112_2_18_1_24_18_504 "_DCAMERA_GetDateRectParam: snapshot the data rect out of trim range"
#define DCAMERA_APP_9304_112_2_18_1_24_18_505 "_DCAMERA_GetDateRectParam: dst_rect x:%d, y:%d, w:%d, h:%d"
#define DCAMERA_APP_9339_112_2_18_1_24_19_506 "_DCAMERA_getDateRectData, date mark rect info is not valid!"
#define DCAMERA_APP_9345_112_2_18_1_24_19_507 "_DCAMERA_getDateRectData, error source or dest date mark address !"
#define DCAMERA_APP_9405_112_2_18_1_24_19_508 "_DCAMERA_dateBlend: date addr 0x%x, x %d y %dwidth %d, height %d"
#define DCAMERA_APP_9417_112_2_18_1_24_19_509 "_DCAMERA_dateBlend: date addr 0x%x, x %d y %dwidth %d, height %d"
#define DCAMERA_APP_9452_112_2_18_1_24_19_510 "_DCAMERA_handleDateMark: the dst date mark rect is not valid!!"
#define DCAMERA_APP_9459_112_2_18_1_24_19_511 "_DCAMERA_handleDateMark: date mark memory alloc fail"
#define DCAMERA_APP_9468_112_2_18_1_24_19_512 "_DCAMERA_handleDateMark: date mark data duplicate error!"
#define DCAMERA_APP_9507_112_2_18_1_24_19_513 "_DCAMERA_handleScaleAndDate: start calculate the date mark scale coeff!"
#define DCAMERA_APP_9567_112_2_18_1_24_19_514 "_DCAMERA_handleScaleAndDate: the scale coeff(256x) is %d"
#define DCAMERA_APP_9636_112_2_18_1_24_19_515 "_DCAMERA_handleScaleAndDate: date mark scaling failed!"
#define DCAMERA_APP_9640_112_2_18_1_24_19_516 "_DCAMERA_handleScaleAndDate: date mark scaling successed!"
#define DCAMERA_APP_9651_112_2_18_1_24_19_517 "_DCAMERA_handleScaleAndDate: start blender the date mark!"
#define DCAMERA_APP_9722_112_2_18_1_24_19_518 "_DCAMERA_JpgEncGetSlice,photo_width =%d, get_slice_width=%d, line_offset %d, read_lines %d, yaddr 0x%x, uv 0x%x"
#define DCAMERA_APP_9729_112_2_18_1_24_19_519 "_DCAMERA_JpgEncGetSlice,photo_height=%d,line_offsert=%d"
#define DCAMERA_APP_9823_112_2_18_1_24_20_520 "_DCAMERA_JpgEncGetSlice,No source slice buffer, y 0x%x u 0x%x"
#define DCAMERA_APP_10013_112_2_18_1_24_20_521 "DC: _DBGDC_DoSnapShotJpgZoom flag"
#define DCAMERA_APP_10261_112_2_18_1_24_21_522 "DC: _DCAMERA_DoSnapshot -> start time = %d,rot %d"
#define DCAMERA_APP_10327_112_2_18_1_24_21_523 "DC: DCAMERA_GetAFrameFromIsp, start time %d  "
#define DCAMERA_APP_10341_112_2_18_1_24_21_524 "DC: DCAMERA_GetAFrameFromIsp ERROR"
#define DCAMERA_APP_10353_112_2_18_1_24_21_525 "DC DCAMERA_GetAFrameFromIsp, end time %d  "
#define DCAMERA_APP_10408_112_2_18_1_24_21_526 "DC: _DCAMERA_DoSnapShot -> flag"
#define DCAMERA_APP_10485_112_2_18_1_24_21_527 "DC: DCAMERA_DoSnapshot -> end time = %d, callback return %d, snapshot return %d"
#define DCAMERA_APP_10524_112_2_18_1_24_21_528 "DCAMERA_ATVAutoScanChn: In Time: %d"
#define DCAMERA_APP_10536_112_2_18_1_24_21_529 "DCAMERA_ATVAutoScanChn: Out Time: %d"
#define DCAMERA_APP_10546_112_2_18_1_24_21_530 "DCAM: DCAMERA_ATVStopScanChn: Time: %d"
#define DCAMERA_APP_10570_112_2_18_1_24_21_531 "DCAMERA_DisplayOneFrame"
#define DCAMERA_APP_10592_112_2_18_1_24_21_532 "DCAMERA_DisplayOneFrame,first frame,no need to free frame"
#define DCAMERA_APP_10597_112_2_18_1_24_21_533 "DCAMERA_DisplayOneFrame, free last frame, 0x%x"
#define DCAMERA_APP_10614_112_2_18_1_24_21_534 "_DCAMERA_OpenISP, Failed to Open ISP_Service"
#define DCAMERA_APP_10634_112_2_18_1_24_21_535 "_DCAMERA_CloseISP, Failed to Close ISP_Service"
#define DCAMERA_APP_10646_112_2_18_1_24_21_536 "DC: ISP output one frame in capture at %d"
#define DCAMERA_APP_10651_112_2_18_1_24_21_537 "DC: yaddr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define DCAMERA_APP_10663_112_2_18_1_24_21_538 "DC: Exit ISP CAPTURE FRAME callback at %d"
#define DCAMERA_APP_10717_112_2_18_1_24_21_539 "DC: _DCAMERA_GetFrameFromIspExt: not support sensor format!"
#define DCAMERA_APP_10731_112_2_18_1_24_22_540 "DC: _DCAMERA_GetFrameFromIspExt: not support dcam output format!"
#define DCAMERA_APP_10794_112_2_18_1_24_22_541 "DC: DCAMERA_ExtDisplay, config memory error!"
#define DCAMERA_APP_10830_112_2_18_1_24_22_542 "DC: DCAMERA_ExtDisplay, not enough memory error!"
#define DCAMERA_APP_10865_112_2_18_1_24_22_543 "DC: DCAMERA_ExtOpenSensor start!"
#define DCAMERA_APP_10905_112_2_18_1_24_22_544 "DC: DCAMERA_ExtOpenSensor accomplish!"
#define DCAMERA_APP_10919_112_2_18_1_24_22_545 "DC: DCAMERA_ExtCloseSensor start!"
#define DCAMERA_APP_10926_112_2_18_1_24_22_546 "DC: DCAMERA_ExtCloseSensor accomplish!"
#define DCAMERA_APP_10946_112_2_18_1_24_22_547 "DC: DCAMERA_ExtGetData start!"
#define DCAMERA_APP_10951_112_2_18_1_24_22_548 "DC: DCAMERA_ExtGetData: error input parameter!"
#define DCAMERA_APP_10961_112_2_18_1_24_22_549 "DC: DCAMERA_ExtGetData isp_service open  error quit!"
#define DCAMERA_APP_10968_112_2_18_1_24_22_550 "DC: DCAMERA_ExtGetData failed from sensor!"
#define DCAMERA_APP_10976_112_2_18_1_24_22_551 "DC: DCAMERA_ExtGetData isp_service close  error quit!"
#define DCAMERA_APP_10996_112_2_18_1_24_22_552 "DC: DCAMERA_ExtGetData accomplished!"
#define DCAMERA_APP_11029_112_2_18_1_24_22_553 "DC: _DCAMERA_BackupReviewbuf idth %d blk_info.end_y %d blk_info.start_y %d"
#define DCAMERA_APP_11036_112_2_18_1_24_22_554 "DC: _DCAMERA_BackupReviewbuf dst 0x%x, length %d, src y 0x%x"
#define DCAMERA_APP_11084_112_2_18_1_24_22_555 "DC: DCAMERA_ExtDisplay start!"
#define DCAMERA_APP_11107_112_2_18_1_24_22_556 "DC: DCAMERA_ExtDisplay: jpeg input, need to decoide at first!"
#define DCAMERA_APP_11147_112_2_18_1_24_22_557 "DC: DCAMERA_ExtDisplay decode jpeg error!"
#define DCAMERA_APP_11171_112_2_18_1_24_22_558 "DC: DCAMERA_ExtDisplay isp_service open error!"
#define DCAMERA_APP_11193_112_2_18_1_24_22_559 "DC: DCAMERA_ExtDisplay isp_service close error!"
#define DCAMERA_APP_11203_112_2_18_1_24_22_560 "DC: DCAMERA_ExtDisplay accomplished!"
#define DCAMERA_APP_11218_112_2_18_1_24_22_561 "DC: DC_VideoSetDisplayParam started!"
#define DCAMERA_APP_11255_112_2_18_1_24_23_562 "DC: DC_VideoSetDisplayParam accomplished!"
#define DCAMERA_APP_11271_112_2_18_1_24_23_563 "DC: DC_DisplayAFrame started!"
#define DCAMERA_APP_11320_112_2_18_1_24_23_564 "DC: DC_DisplayAFrame accomplished!"
#define DCAMERA_APP_SIMULATOR_1934_112_2_18_1_24_27_565 "[simulator_jpg_data] SaveJpeg successed! "
#define DCAMERA_MEM_118_112_2_18_1_24_29_566 "DCamera_MemConfig, NO memory reserved for DC"
#define DCAMERA_MEM_127_112_2_18_1_24_29_567 "DCamera_MemConfig, Memory not in one 64M Segement,try again"
#define DCAMERA_MEM_136_112_2_18_1_24_29_568 "DCamera_MemConfig, Number %d block mem freed"
#define DCAMERA_MEM_142_112_2_18_1_24_29_569 "DCamera_MemConfig, Memory successfully,total try times %d ,addr 0x%x"
#define DCAMERA_MEM_146_112_2_18_1_24_29_570 "DCamera_MemConfig, No memory reserved for DC"
#define DCAMERA_MEM_172_112_2_18_1_24_29_571 "DCamera_MemConfig, Memory alloc successfully base 0x%x,start 0x%x,size 0x%x"
#define DCAMERA_MEM_188_112_2_18_1_24_29_572 "DCamera_MemFree OK"
#define DCAMERA_MEM_269_112_2_18_1_24_29_573 "DCAMERA_PreviewGetMem, start 0x%x, size 0x%x"
#define DCAMERA_MEM_461_112_2_18_1_24_30_574 "DCAMERA_CaptureMemReq, not enough memory for rotation capture"
#define SENSOR_DRV_229_112_2_18_1_24_32_575 "SENSOR: I2C handle abnormal"
#define SENSOR_DRV_235_112_2_18_1_24_32_576 "SENSOR: I2C_no_close"
#define SENSOR_DRV_240_112_2_18_1_24_32_577 "SENSOR: I2C_close s_i2c_dev_handler=%d "
#define SENSOR_DRV_314_112_2_18_1_24_32_578 "_Sensor_IicHandlerInit, id %d freq %d bus %d slave_addr 0x%x reg_addr_num %d,ack %d no_stop %d"
#define SENSOR_DRV_320_112_2_18_1_24_32_579 "SENSOR_handler creat err first"
#define SENSOR_DRV_349_112_2_18_1_24_32_580 "SENSOR: Sensor_Reset -> reset_pulse_level = %d"
#define SENSOR_DRV_380_112_2_18_1_24_32_581 "SENSOR_DRV: set mclk parameter error, the MCLK is %d"
#define SENSOR_DRV_419_112_2_18_1_24_32_582 "SENSOR: Sensor_SetMCLK -> s_sensor_mclk = %dMHz, clk = %dMHz"
#define SENSOR_DRV_434_112_2_18_1_24_32_583 "SENSOR: Sensor_SetMCLK divd = %d"
#define SENSOR_DRV_440_112_2_18_1_24_32_584 "SENSOR: Sensor_SetMCLK -> Disable MCLK !!!"
#define SENSOR_DRV_444_112_2_18_1_24_32_585 "SENSOR: Sensor_SetMCLK -> Do nothing !! "
#define SENSOR_DRV_461_112_2_18_1_24_32_586 "SENSOR: Sensor_SetMCLK src = 0x%x,  divd = 0x%x"
#define SENSOR_DRV_481_112_2_18_1_24_32_587 "SENSOR: Sensor_SetMCLK -> Disable MCLK !!!"
#define SENSOR_DRV_485_112_2_18_1_24_32_588 "SENSOR: Sensor_SetMCLK -> Do nothing !! "
#define SENSOR_DRV_623_112_2_18_1_24_33_589 "SENSOR: Sensor_PowerOn -> power_on = %d, power_down_level = %d, avdd_val = %d"
#define SENSOR_DRV_668_112_2_18_1_24_33_590 "SENSOR: Sensor_PowerDown -> main, power_down %d, time %d"
#define SENSOR_DRV_687_112_2_18_1_24_33_591 "SENSOR: Sensor_PowerDown -> sub, power_down %d, time %d"
#define SENSOR_DRV_705_112_2_18_1_24_33_592 "SENSOR: Sensor_PowerDown -> atv %d"
#define SENSOR_DRV_710_112_2_18_1_24_33_593 "SENSOR: Sensor_PowerDown -> atv not found"
#define SENSOR_DRV_716_112_2_18_1_24_33_594 "SENSOR: Sensor_PowerDown -> atv not found"
#define SENSOR_DRV_791_112_2_18_1_24_33_595 "SENSOR: Sensor_CheckSensorInfo -> sensor name = %s"
#define SENSOR_DRV_807_112_2_18_1_24_33_596 "SENSOR: Sensor_SetExportInfo"
#define SENSOR_DRV_895_112_2_18_1_24_34_597 "SENSOR: SENSOR mode Info > mode = %d, width = %d, height = %d, format = %d"
#define SENSOR_DRV_927_112_2_18_1_24_34_598 "SENSOR: IIC write : reg:0x%04x, val:0x%04x error"
#define SENSOR_DRV_959_112_2_18_1_24_34_599 "SENSOR: s_i2c_dev_handler 0x%x"
#define SENSOR_DRV_969_112_2_18_1_24_34_600 "SENSOR: IIC write reg Error! 0x%04x, val:0x%04x"
#define SENSOR_DRV_974_112_2_18_1_24_34_601 "SENSOR: IIC write reg OK! 0x%04x, val:0x%04x "
#define SENSOR_DRV_990_112_2_18_1_24_34_602 "SENSOR: IIC write Delay %d ms"
#define SENSOR_DRV_1043_112_2_18_1_24_34_603 "SENSOR: s_i2c_dev_handler 0x%x"
#define SENSOR_DRV_1054_112_2_18_1_24_34_604 "SENSOR: IIC read reg, ERROR! %d"
#define SENSOR_DRV_1061_112_2_18_1_24_34_605 "SENSOR: IIC read reg OK:0x%04x, val:0x%04x"
#define SENSOR_DRV_1080_112_2_18_1_24_34_606 "SENSOR: Sensor_SendRegValueToSensor -> reg_count = %d start time = %d"
#define SENSOR_DRV_1085_112_2_18_1_24_34_607 "SENSOR: Sensor_SendRegTabToSensor tab is null error"
#define SENSOR_DRV_1097_112_2_18_1_24_34_608 "SENSOR: Sensor_SendRegValueToSensor -> end time = %d"
#define SENSOR_DRV_1192_112_2_18_1_24_34_609 "Sensor_SetCurId : %d, start time: %d"
#define SENSOR_DRV_1196_112_2_18_1_24_34_610 "Sensor_SetCurId : sensor_id %d unsupported"
#define SENSOR_DRV_1202_112_2_18_1_24_34_611 "Sensor_SetCurId: No such sensor! %d "
#define SENSOR_DRV_1228_112_2_18_1_24_34_612 "SENSOR:Sensor_SetCurId id:%d, num:%d, ptr: 0x%x, ptr: 0x%x"
#define SENSOR_DRV_1230_112_2_18_1_24_34_613 "Sensor_SetCurId : %d, end time: %d"
#define SENSOR_DRV_1257_112_2_18_1_24_34_614 "SENSOR: sensor identifing %d"
#define SENSOR_DRV_1262_112_2_18_1_24_34_615 "SENSOR: sensor identified"
#define SENSOR_DRV_1277_112_2_18_1_24_34_616 "SENSOR: %d info of Sensor_Init table %d is null"
#define SENSOR_DRV_1310_112_2_18_1_24_35_617 "SENSOR TYPE of %d indentify OK"
#define SENSOR_DRV_1314_112_2_18_1_24_35_618 "SENSOR TYPE of %d indentify FAILURE"
#define SENSOR_DRV_1326_112_2_18_1_24_35_619 "SENSOR: Sensor_Init start %d"
#define SENSOR_DRV_1330_112_2_18_1_24_35_620 "SENSOR: Sensor_Init is done\n"
#define SENSOR_DRV_1383_112_2_18_1_24_35_621 "SENSOR: Sensor_Init Main Success \n"
#define SENSOR_DRV_1393_112_2_18_1_24_35_622 "SENSOR: Sensor_Init Sub Success \n"
#define SENSOR_DRV_1400_112_2_18_1_24_35_623 "SENSOR: Sensor_Init Fail No Sensor err \n"
#define SENSOR_DRV_1403_112_2_18_1_24_35_624 "SENSOR: Sensor_Init: end: %d"
#define SENSOR_DRV_1437_112_2_18_1_24_35_625 "SENSOR: Sensor_powerUpHandle start %d"
#define SENSOR_DRV_1442_112_2_18_1_24_35_626 "SENSOR: Sensor_powerUpHandle error for sensor info of %d is null"
#define SENSOR_DRV_1473_112_2_18_1_24_35_627 "SENSOR: Sensor_sleep of id %d, time %d"
#define SENSOR_DRV_1493_112_2_18_1_24_35_628 "SENSOR: Sensor_Open the sensor identify fail %d error"
#define SENSOR_DRV_1511_112_2_18_1_24_35_629 "SENSOR: Sensor_Open the atv init fail %d error"
#define SENSOR_DRV_1535_112_2_18_1_24_35_630 "SENSOR: Sensor_Open sensor type: %d id: %d, time %d"
#define SENSOR_DRV_1539_112_2_18_1_24_35_631 "SENSOR: Sensor_Open -> sensor has not init"
#define SENSOR_DRV_1547_112_2_18_1_24_35_632 "SENSOR: Sensor_Open: start: %d"
#define SENSOR_DRV_1550_112_2_18_1_24_35_633 "SENSOR: Sensor_Open -> sensor has open"
#define SENSOR_DRV_1561_112_2_18_1_24_35_634 "SENSOR: Sensor_Open -> sensor main manual set"
#define SENSOR_DRV_1580_112_2_18_1_24_35_635 "SENSOR: ext func init %d error"
#define SENSOR_DRV_1619_112_2_18_1_24_35_636 "SENSOR: Sensor_Open the atv init fail %d error"
#define SENSOR_DRV_1629_112_2_18_1_24_35_637 "SENSOR: Sensor_Open end %d"
#define SENSOR_DRV_1643_112_2_18_1_24_35_638 "SENSOR: Sensor_SetMode -> mode = %d, time %d"
#define SENSOR_DRV_1647_112_2_18_1_24_35_639 "SENSOR: The sensor mode as before"
#define SENSOR_DRV_1653_112_2_18_1_24_35_640 "SENSOR: Sensor_SetResolution -> sensor has not init"
#define SENSOR_DRV_1659_112_2_18_1_24_35_641 "SENSOR: current sensor info is not available!"
#define SENSOR_DRV_1680_112_2_18_1_24_35_642 "SENSOR: Sensor_SetResolution -> No this resolution information !!!"
#define SENSOR_DRV_1699_112_2_18_1_24_35_643 "SENSOR: Sensor_Ioctl -> cmd = %d, arg = 0x%x"
#define SENSOR_DRV_1703_112_2_18_1_24_35_644 "SENSOR: Sensor_Ioctl -> cmd %d error"
#define SENSOR_DRV_1709_112_2_18_1_24_35_645 "SENSOR: Sensor_Ioctl -> sensor has not init error"
#define SENSOR_DRV_1715_112_2_18_1_24_35_646 "SENSOR: Sensor_Ioctl -> sensor has not open error"
#define SENSOR_DRV_1721_112_2_18_1_24_35_647 "SENSOR: Sensor_Ioctl -> sensor has not initial I2C handle"
#define SENSOR_DRV_1727_112_2_18_1_24_35_648 "SENSOR: Sensor_Ioctl - > can't access internal command error!"
#define SENSOR_DRV_1745_112_2_18_1_24_35_649 "SENSOR: Sensor_Ioctl -> the ioctl function has not register err!"
#define SENSOR_DRV_1759_112_2_18_1_24_35_650 "SENSOR: Sensor_GetInfo -> sensor has not init"
#define SENSOR_DRV_1771_112_2_18_1_24_35_651 "SENSOR: Sensor_close"
#define SENSOR_DRV_1901_112_2_18_1_24_36_652 "SENSOR: Sensor_SetSensorExifInfo the get_exif fun is null %d error"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_DC_TRC)
TRACE_MSG(DC_APP_152_112_2_18_1_23_11_0,"DC_FrameMgr_CreateAFrame() failed")
TRACE_MSG(DC_APP_177_112_2_18_1_23_11_1,"_EncodeYYUVToJpeg(),jpeg encoding err,line %d")
TRACE_MSG(DC_APP_198_112_2_18_1_23_11_2,"DCAM:_mjpeg_encode_callback, record_callback is null err")
TRACE_MSG(DC_APP_259_112_2_18_1_23_11_3,"DCAM:_CheckMjpegRecParam target size:%d,%d err")
TRACE_MSG(DC_APP_267_112_2_18_1_23_11_4,"DCAM:_CheckMjpegRecParam disply size:%d,%d err")
TRACE_MSG(DC_APP_301_112_2_18_1_23_11_5,"DCAM:_SetMjpegRecParam caputre buf is null err!")
TRACE_MSG(DC_APP_311_112_2_18_1_23_11_6,"DCAM:_SetMjpegRecParam display buf is null err!")
TRACE_MSG(DC_APP_321_112_2_18_1_23_11_7,"DCAM:_SetMjpegRecParam scale buf is null err!")
TRACE_MSG(DC_APP_331_112_2_18_1_23_11_8,"DCAM:_SetMjpegRecParam rotation buf is null err!")
TRACE_MSG(DC_APP_472_112_2_18_1_23_11_9,"DCAM:_SetMjpegDecParam y/uv buf is null err!")
TRACE_MSG(DC_APP_612_112_2_18_1_23_12_10,"_DCAMERA_ROT_Convertor: %d")
TRACE_MSG(DC_APP_648_112_2_18_1_23_12_11,"_DCAMERA_ROT_Convertor: img: %d, ret: %d")
TRACE_MSG(DC_APP_681_112_2_18_1_23_12_12,"DCAM:_DCAMERA_InitSensor img sensor init fail err!")
TRACE_MSG(DC_APP_688_112_2_18_1_23_12_13,"_DCAMERA_InitSensor: before sensor_open, %d")
TRACE_MSG(DC_APP_691_112_2_18_1_23_12_14,"DCAM:_DCAMERA_InitSensor img sensor open fail err!")
TRACE_MSG(DC_APP_694_112_2_18_1_23_12_15,"_DCAMERA_InitSensor: after sensor_open, %d")
TRACE_MSG(DC_APP_712_112_2_18_1_23_12_16,"DCAMERA_SetSensorType: %d")
TRACE_MSG(DC_APP_746_112_2_18_1_23_12_17,"DCAM:DC_Open,time: %d")
TRACE_MSG(DC_APP_788_112_2_18_1_23_12_18,"DCAM: enter, mode %d, time %d")
TRACE_MSG(DC_APP_797_112_2_18_1_23_12_19,"---------DCAMERA_Open: USE fmark-------- ")
TRACE_MSG(DC_APP_815_112_2_18_1_23_12_20,"DCAM:DC lock isp error")
TRACE_MSG(DC_APP_830_112_2_18_1_23_12_21,"DCAM, end:  time %d")
TRACE_MSG(DC_APP_849_112_2_18_1_23_12_22,"DCAM:DCAMERA_Close start: %d")
TRACE_MSG(DC_APP_893_112_2_18_1_23_12_23,"DCAM:DCAMERA_Close end: %d")
TRACE_MSG(DC_APP_922_112_2_18_1_23_12_24,"DCAM:DCAMERA_StartPreview, start time = %d")
TRACE_MSG(DC_APP_926_112_2_18_1_23_12_25,"DCAM:DCAMERA_StartPreview param_ptr is null")
TRACE_MSG(DC_APP_975_112_2_18_1_23_13_26,"DCAM:DCAMERA_StartPreview end: %d")
TRACE_MSG(DC_APP_994_112_2_18_1_23_13_27,"DCAM: DCAMERA_StopPreview, time %d")
TRACE_MSG(DC_APP_1021_112_2_18_1_23_13_28,"DCAMERA_ATVAutoScanChn: In Time: %d")
TRACE_MSG(DC_APP_1026_112_2_18_1_23_13_29,"DCAM:DCAMERA_StartPreview param_ptr is null")
TRACE_MSG(DC_APP_1040_112_2_18_1_23_13_30,"DCAMERA_ATVAutoScanChn: Out Time: %d")
TRACE_MSG(DC_APP_1058_112_2_18_1_23_13_31,"DCAM: DCAMERA_ATVStopScanChn: Time: %d")
TRACE_MSG(DC_APP_1122_112_2_18_1_23_13_32,"DCAM: DCAMERA_DoSnapshot, time %d")
TRACE_MSG(DC_APP_1127_112_2_18_1_23_13_33,"DCAM:DCAMERA_DoSnapshot param_ptr/return_param_ptr is null")
TRACE_MSG(DC_APP_1144_112_2_18_1_23_13_34,"DCAM: capture asynchronous")
TRACE_MSG(DC_APP_1149_112_2_18_1_23_13_35,"DCAM: capture synchronous")
TRACE_MSG(DC_APP_1174_112_2_18_1_23_13_36,"DCAM: DCAMERA_DoBurstSnapshot, time %d")
TRACE_MSG(DC_APP_1179_112_2_18_1_23_13_37,"DCAM:DCAMERA_DoSnapshot param_ptr/return_param_ptr is null err")
TRACE_MSG(DC_APP_1185_112_2_18_1_23_13_38,"DCAM:DCAMERA_DoSnapshot burst_snapshot_callback is null err")
TRACE_MSG(DC_APP_1220_112_2_18_1_23_13_39,"DCAM: DCAMERA_ReviewJpeg rot:%d")
TRACE_MSG(DC_APP_1240_112_2_18_1_23_13_40,"DCAM: DCAMERA_ReviewJpeg param_ptr is null err")
TRACE_MSG(DC_APP_1255_112_2_18_1_23_13_41,"DCAM: review asynchronous")
TRACE_MSG(DC_APP_1260_112_2_18_1_23_13_42,"DCAM: review synchronous")
TRACE_MSG(DC_APP_1288_112_2_18_1_23_13_43,"DCAM: DCAMERA_GetInfo start cmd:0x%x info:0x%x, %d")
TRACE_MSG(DC_APP_1434_112_2_18_1_23_14_44,"DCAM: DCAMERA_GetInfo end cmd:0x%x info:0x%x, %d")
TRACE_MSG(DC_APP_1454_112_2_18_1_23_14_45,"DCAM: DCAMERA_ParamCtrl  start: %d")
TRACE_MSG(DC_APP_1467_112_2_18_1_23_14_46,"DCAM: DCAMERA_ParamCtrl end: %d")
TRACE_MSG(DC_APP_1513_112_2_18_1_23_14_47,"DCAM: DC_SetUpccParam")
TRACE_MSG(DC_APP_1517_112_2_18_1_23_14_48,"DCAM:DC_SetUpccParam pst_param is null err")
TRACE_MSG(DC_APP_1552_112_2_18_1_23_14_49,"DCAM:DC_UpccStart param_ptr is null err")
TRACE_MSG(DC_APP_1649_112_2_18_1_23_14_50,"DCAM:DC_SetMjpgRecParam pst_param is null err")
TRACE_MSG(DC_APP_1677_112_2_18_1_23_14_51,"DC_StartMjpgRec(%lu,0x%lx)")
TRACE_MSG(DC_APP_1681_112_2_18_1_23_14_52,"DCAM:DC_StartMjpgRec pf_callback is null err")
TRACE_MSG(DC_APP_1821_112_2_18_1_23_14_53,"DCAM:DC_VideoDisplayInit")
TRACE_MSG(DC_APP_1845_112_2_18_1_23_14_54,"DCAM:DC_VideoDisplayClose")
TRACE_MSG(DC_APP_1880_112_2_18_1_23_14_55,"DCAM:DC_VideoSetDisplayParam")
TRACE_MSG(DC_APP_1884_112_2_18_1_23_14_56,"DCAM:DC_VideoSetDisplayParam pst_param is null err")
TRACE_MSG(DC_APP_1945_112_2_18_1_23_15_57,"DCAM:DC_DisplayAFrame p_frame is null err")
TRACE_MSG(DC_APP_1954_112_2_18_1_23_15_58,"DCAM:DC lock isp error")
TRACE_MSG(DC_APP_1998_112_2_18_1_23_15_59,"DCAM:DC_GetFrameData register func is null err")
TRACE_MSG(DC_APP_2141_112_2_18_1_23_15_60,"DCM: _ExtSetGetDataParam jpg buf is not enough err")
TRACE_MSG(DC_APP_2434_112_2_18_1_23_16_61,"DCM: DCAMERA_ExtGetData param is null or img sesor is not open")
TRACE_MSG(DC_APP_2469_112_2_18_1_23_16_62,"DCM: DCAMERA_ExtGetData capture err")
TRACE_MSG(DC_APP_2488_112_2_18_1_23_16_63,"DCM: DCAMERA_ExtGetData review err")
TRACE_MSG(DC_APP_2500_112_2_18_1_23_16_64,"DCM: DCAMERA_ExtGetData encoder err")
TRACE_MSG(DC_BUFMGR_236_112_2_18_1_23_24_65,"DCAM:MM_AllocRawBuffer: Buffer allaoc fail err")
TRACE_MSG(DC_BUFMGR_255_112_2_18_1_23_24_66,"DCAM: MM_AllocRawBuffer: Buffer is allaoced")
TRACE_MSG(DC_BUFMGR_1098_112_2_18_1_23_25_67,"DCAM: MM_Init, preview buf is not enough err!")
TRACE_MSG(DC_BUFMGR_1240_112_2_18_1_23_26_68,"DCAM: MM_Init, capture buf is not enough err!")
TRACE_MSG(DC_BUFMGR_1390_112_2_18_1_23_26_69,"DCAM: MM_Init, review buf is not enough err!")
TRACE_MSG(DC_BUFMGR_1462_112_2_18_1_23_26_70,"DCAM: MM_Init, continue shot buf is not enough err!")
TRACE_MSG(DC_BUFMGR_1533_112_2_18_1_23_26_71,"DCAM: MM_Init, dv review buf is not enough err!")
TRACE_MSG(DC_BUFMGR_1558_112_2_18_1_23_27_72,"DCAM: MM_Init, dv review buf is not enough err!")
TRACE_MSG(DC_BUFMGR_1567_112_2_18_1_23_27_73,"DCAM: Sorry!MM buffer MGR,case id does not exist error")
TRACE_MSG(DC_BUFMGR_1631_112_2_18_1_23_27_74,"DCAM: MM_Init, all buf start addr=0x%x,buf_len=%dk")
TRACE_MSG(DC_BUFMGR_1636_112_2_18_1_23_27_75,"DCAM: MM_Init alloc dc buf fail err!")
TRACE_MSG(DC_BUFMGR_1696_112_2_18_1_23_27_76,"DCAM: MM_Malloc, pMM->buf_ptr is null %d line error")
TRACE_MSG(DC_BUFMGR_1706_112_2_18_1_23_27_77,"DCAM: MM_Malloc, tbl_ptr is null %d line error")
TRACE_MSG(DC_BUFMGR_1711_112_2_18_1_23_27_78,"DCAM: MM_Malloc, (group,block)=(%d,%d), addr range(0x%lx ~ 0x%lx) ")
TRACE_MSG(DC_BUFMGR_1732_112_2_18_1_23_27_79,"DCAM: MM_GetSize, pMM->buf_ptr is null %d line error")
TRACE_MSG(DC_BUFMGR_1743_112_2_18_1_23_27_80,"DCAM: MM_GetSize, tbl_ptr is null %d line error")
TRACE_MSG(DC_CODEC_94_112_2_18_1_23_27_81,"DCAM: _DC_SetMinatureInParam")
TRACE_MSG(DC_CODEC_132_112_2_18_1_23_27_82,"DCAM: _DC_SetThumbnailMinatureInParam")
TRACE_MSG(DC_CODEC_165_112_2_18_1_23_27_83,"DCAM: _DC_SetWriteEixfInParam")
TRACE_MSG(DC_CODEC_215_112_2_18_1_23_28_84,"DCAM:DC_Get_HwSliceScaleLine: %d")
TRACE_MSG(DC_CODEC_272_112_2_18_1_23_28_85,"DCAM:_GetSliceYUVData,photo_width=%d,get_slice_width=%d err")
TRACE_MSG(DC_CODEC_278_112_2_18_1_23_28_86,"DCAM:_GetSliceYUVData,photo_height=%d,line_offsert=%d err")
TRACE_MSG(DC_CODEC_415_112_2_18_1_23_28_87,"_EncodeYYUVToThumbnail(),thumbnail scale yuv buf err ,line %d")
TRACE_MSG(DC_CODEC_487_112_2_18_1_23_28_88,"_EncodeYYUVToThumbnail(),jpeg encoding err ,line %d")
TRACE_MSG(DC_CODEC_495_112_2_18_1_23_28_89,"DCAM: _EncodeYYUVToThumbnail, thumbnail_addr = 0x%x")
TRACE_MSG(DC_CODEC_496_112_2_18_1_23_28_90,"DCAM: _EncodeYYUVToThumbnail, thumbnail_size = %d")
TRACE_MSG(DC_CODEC_539_112_2_18_1_23_28_91,"DCAM: _DC_JpegToThumbnail, Greate thumbnail err ,line %d")
TRACE_MSG(DC_CODEC_547_112_2_18_1_23_28_92,"DCAM: _DC_JpegToThumbnail, thumbnail_buf_addr = 0x%x")
TRACE_MSG(DC_CODEC_548_112_2_18_1_23_28_93,"DCAM: _DC_JpegToThumbnail, thumbnail_size = %d")
TRACE_MSG(DC_CODEC_582_112_2_18_1_23_28_94,"DCAM: DC_ExifGreate(),jpeg write exif error,line %d")
TRACE_MSG(DC_CODEC_588_112_2_18_1_23_28_95,"DCAM: DC_ExifGreate(),jpeg_exif_buf_ptr = 0x%x")
TRACE_MSG(DC_CODEC_589_112_2_18_1_23_28_96,"DCAM: DC_ExifGreate(),jpeg_exif_size = %d")
TRACE_MSG(DC_CODEC_676_112_2_18_1_23_29_97,"_EncodeYYUVToJpeg(),jpeg encoding error,line %d")
TRACE_MSG(DC_CODEC_685_112_2_18_1_23_29_98,"DCAM: _EncodeYYUVToJpeg(),jpeg_buf_ptr = 0x%x")
TRACE_MSG(DC_CODEC_686_112_2_18_1_23_29_99,"DCAM: _EncodeYYUVToJpeg(),jpeg_size = %d")
TRACE_MSG(DC_CODEC_745_112_2_18_1_23_29_100,"DCAM: _EncodeYYUVSliceToJpeg(),jpeg encoding err,line %d")
TRACE_MSG(DC_CODEC_754_112_2_18_1_23_29_101,"DCAM: _EncodeYYUVSliceToJpeg(),jpeg_buf_ptr = 0x%x")
TRACE_MSG(DC_CODEC_755_112_2_18_1_23_29_102,"DCAM: _EncodeYYUVSliceToJpeg(),jpeg_size = %d")
TRACE_MSG(DC_CODEC_803_112_2_18_1_23_29_103,"DCAM: DC_JpegProcess,Greate Miniature err ,line %d")
TRACE_MSG(DC_CODEC_817_112_2_18_1_23_29_104,"DCAM: DC_JpegProcess(),jpeg_buf_ptr = 0x%x")
TRACE_MSG(DC_CODEC_818_112_2_18_1_23_29_105,"DCAM: DC_JpegProcess(),jpeg_size = %d")
TRACE_MSG(DC_CODEC_882_112_2_18_1_23_29_106,"_DecodeJpegToYYUV(),jpeg decoding error,line %d")
TRACE_MSG(DC_FRAMEMGR_62_112_2_18_1_23_30_107,"DC_FrameMgr_Init(),base_addr = %x, uplimited_addr = %x")
TRACE_MSG(DC_FRAMEMGR_93_112_2_18_1_23_30_108,"DC DC_FrameMgr_C,ring buffer overflowed!,line %d")
TRACE_MSG(DC_FRAMEMGR_103_112_2_18_1_23_30_109,"DC DC_FrameMgr_C,frame fifo overflowed!, line %d")
TRACE_MSG(DC_FRAMEMGR_141_112_2_18_1_23_30_110,"DC_FrameMgr_CreateAFrame,fragment appears,len = %lu, line %d")
TRACE_MSG(DC_FRAMEMGR_157_112_2_18_1_23_30_111,"DC_FrameMgr_CreateAFrame,fragment appears,len = %lu, line %d")
TRACE_MSG(DC_FRAMEMGR_164_112_2_18_1_23_30_112,"DC_FrameMgr_CreateAFrame,remain ,len = %lu, line %d")
TRACE_MSG(DC_FRAMEMGR_227_112_2_18_1_23_30_113,"DC_FrameMgr_F, pBufMgr->tail_fragment_len:%d, remain:%d")
TRACE_MSG(DC_LCDC_81_112_2_18_1_23_30_114,"\nLCD:%d block:%d not supoort")
TRACE_MSG(DC_LCDC_93_112_2_18_1_23_30_115,"\n The caller thread must get mutex first")
TRACE_MSG(DC_LCDC_106_112_2_18_1_23_30_116,"LCDC_SetBlockBuffer, buf_ptr 0x%x")
TRACE_MSG(DC_LCDC_119_112_2_18_1_23_31_117,"LCDC_EnableBlock")
TRACE_MSG(DC_LCDC_130_112_2_18_1_23_31_118,"LCDC_DisableBlock")
TRACE_MSG(DC_LCDC_160_112_2_18_1_23_31_119,"LCDC_ConfigBlock")
TRACE_MSG(DC_LCDC_271_112_2_18_1_23_31_120,"LCDC_SetOsdBlockRect")
TRACE_MSG(DC_LCDC_285_112_2_18_1_23_31_121,"LCDC_OsdBlockRectEnable")
TRACE_MSG(DC_LCDC_297_112_2_18_1_23_31_122,"LCDC_OsdBlockRectDisable")
TRACE_MSG(DC_MISC_449_112_2_18_1_23_32_123,"DCAM:DC_AdjRect_OnDistortion: disp_mode:%d, rot_mode:%d")
TRACE_MSG(DC_MISC_450_112_2_18_1_23_32_124,"DCAM:DC_AdjRect_OnDistortion: src rect x:%d, y:%d,w:%d, h:%d")
TRACE_MSG(DC_MISC_451_112_2_18_1_23_32_125,"DCAM:DC_AdjRect_OnDistortion: disp rect x:%d, y:%d,w:%d, h:%d")
TRACE_MSG(DC_MISC_452_112_2_18_1_23_32_126,"DCAM:DC_AdjRect_OnDistortion: target rect x:%d, y:%d,w:%d, h:%d")
TRACE_MSG(DC_MISC_566_112_2_18_1_23_32_127,"DCAM:DC_AdjRect_OnDistortion: modify src rect x:%d, y:%d,w:%d, h:%d")
TRACE_MSG(DC_MISC_567_112_2_18_1_23_32_128,"DCAM:DC_AdjRect_OnDistortion: modify disp rect x:%d, y:%d,w:%d, h:%d")
TRACE_MSG(DC_MISC_568_112_2_18_1_23_32_129,"DCAM:DC_AdjRect_OnDistortion: modify target rect x:%d, y:%d,w:%d, h:%d")
TRACE_MSG(DC_MISC_587_112_2_18_1_23_32_130,"DCAM: DC_LcdPointMatchToImg: lcd rect x:%d, y:%d,w:%d, h:%d")
TRACE_MSG(DC_MISC_658_112_2_18_1_23_32_131,"DCAM: DC_LcdPointMatchToImg: modify target rect x:%d, y:%d,w:%d, h:%d")
TRACE_MSG(DC_MISC_1134_112_2_18_1_23_33_132,"DCAM: DC_Malloc, pContext->dc_buf_addr 0x%x")
TRACE_MSG(DC_MISC_1135_112_2_18_1_23_33_133,"DCAM: DC_Malloc, pContext->thumbnail_buf_addr 0x%x, %dk")
TRACE_MSG(DC_MISC_1136_112_2_18_1_23_33_134,"DCAM: DC_Malloc, pContext->exif_buf_addr 0x%x, %dk")
TRACE_MSG(DC_MISC_1137_112_2_18_1_23_33_135,"DCAM: DC_Malloc, pContext->jpeg_buf_ptr 0x%x, %dk")
TRACE_MSG(DC_MISC_1138_112_2_18_1_23_33_136,"DCAM: DC_Malloc, pContext->yyuv_buf.y_addr 0x%x, %dk")
TRACE_MSG(DC_MISC_1139_112_2_18_1_23_33_137,"DCAM: DC_Malloc, pContext->quick_view_buf.y_addr 0x%x, %dk")
TRACE_MSG(DC_MISC_1140_112_2_18_1_23_33_138,"DCAM: DC_Malloc, pContext->cx_buf_ptr 0x%x, %dk")
TRACE_MSG(DC_MISC_1141_112_2_18_1_23_33_139,"DCAM: DC_Malloc, pContext->jpeg_fw_ptr 0x%x, %dk")
TRACE_MSG(DC_MISC_1142_112_2_18_1_23_33_140,"DCAM: DC_Malloc, pContext->disp_buf_ptr 0x%x, %dk")
TRACE_MSG(DC_MISC_1143_112_2_18_1_23_33_141,"DCAM: DC_Malloc, pContext->rot_buf_ptr 0x%x, %dk")
TRACE_MSG(DC_MISC_1144_112_2_18_1_23_33_142,"DCAM: DC_Malloc, pContext->exif_swap_buf_addr 0x%x, %dk")
TRACE_MSG(DC_MISC_1145_112_2_18_1_23_33_143,"DCAM: DC_Malloc, pContext->jpg_yuv_buf_addr 0x%x, %dk")
TRACE_MSG(DC_MISC_1146_112_2_18_1_23_33_144,"DCAM: DC_Malloc, pContext->src_jpg_addr 0x%x, %dk")
TRACE_MSG(DC_MISC_1147_112_2_18_1_23_33_145,"DCAM: DC_Malloc, pContext->minature_swap_buf_addr 0x%x, %dk")
TRACE_MSG(DC_MISC_1148_112_2_18_1_23_33_146,"DCAM: DC_Malloc, pContext->total_buf_ptr 0x%x, %dk")
TRACE_MSG(DC_MISC_1149_112_2_18_1_23_33_147,"DCAM: DC_Malloc, pContext->remain_buf_ptr 0x%x, %dk")
TRACE_MSG(DC_MISC_1167_112_2_18_1_23_33_148,"dc_task,DC_SendMessage sig_code = %d")
TRACE_MSG(DC_MISC_1197_112_2_18_1_23_34_149,"DCAM: DC_TaskEntry sig is null error")
TRACE_MSG(DC_MISC_1203_112_2_18_1_23_34_150,"DCAM: DC_TaskEntry cmd % error")
TRACE_MSG(DC_MISC_1219_112_2_18_1_23_34_151,"dc_task, DC_TASK_MSG_PREVIEW")
TRACE_MSG(DC_MISC_1227_112_2_18_1_23_34_152,"DCAM: dc_task, DC_TASK_MSG_SNAPSHOT")
TRACE_MSG(DC_MISC_1244_112_2_18_1_23_34_153,"dc_task, DC_TASK_MSG_DISPLAY")
TRACE_MSG(DC_MISC_1267_112_2_18_1_23_34_154,"DCAM: dc_task, DCAM_MSG_BURST_SNAPSHOT")
TRACE_MSG(DC_MISC_1275_112_2_18_1_23_34_155,"dc_task, DC_TASK_MSG_DECODE")
TRACE_MSG(DC_MISC_1281_112_2_18_1_23_34_156,"dc_task, DC_TASK_MSG_MJPEG_CAPTURE")
TRACE_MSG(DC_MISC_1287_112_2_18_1_23_34_157,"dc_task, DCAM_MSG_DISPLAY_DONE")
TRACE_MSG(DC_MISC_1294_112_2_18_1_23_34_158,"dc_task, DC_TASK_MSG_OVERTIME")
TRACE_MSG(DC_MISC_1300_112_2_18_1_23_34_159,"dc_task, DCAM_MSG_ZOOM_DONE")
TRACE_MSG(DC_MISC_1314_112_2_18_1_23_34_160,"dc_task, invalid sig code to dc_task_Entry,line %d")
TRACE_MSG(DC_MISC_1358_112_2_18_1_23_34_161,"DCAM_TaskOpen, task_id = %d")
TRACE_MSG(DC_MISC_1366_112_2_18_1_23_34_162,"DCAM_TaskClose,task_id = %lu")
TRACE_MSG(DC_MISC_1682_112_2_18_1_23_34_163,"DCAM: DC_GetCaptureMatchSize :%d")
TRACE_MSG(DC_MISC_1740_112_2_18_1_23_35_164,"--DC_SetCaptureImgSensorMode:target_w =%d,pDcContext->snapshot_mode=%d,%d--")
TRACE_MSG(DC_MISC_2182_112_2_18_1_23_35_165,"Hansen swap:%d")
TRACE_MSG(DC_MISC_2185_112_2_18_1_23_35_166,"DCAM: GetYuvEncoderBufSize :sensor_w=%d, photo=%d, mem=%dk")
TRACE_MSG(DC_MISC_2229_112_2_18_1_23_36_167,"DCAM: GetJpgEncoderBufSize :sensor_w=%d, photo=%d, mem=%dk")
TRACE_MSG(DC_MISC_2269_112_2_18_1_23_36_168,"DCAM: DC_GetSensorSize mem total: %dk")
TRACE_MSG(DC_MISC_2316_112_2_18_1_23_36_169,"Hansen: %d")
TRACE_MSG(DC_MISC_2402_112_2_18_1_23_36_170,"DCAM: _DC_IsZoomSupported is not support !4P3")
TRACE_MSG(DC_MISC_2411_112_2_18_1_23_36_171,"DCAM: _DC_IsZoomSupported is not support sensor:%d photo:%d")
TRACE_MSG(DC_MISC_2609_112_2_18_1_23_36_172,"DCAM: DC_GetZoomMaxLevel start")
TRACE_MSG(DC_MISC_2618_112_2_18_1_23_36_173,"DCAM: DC_GetZoomMaxLevel :%d")
TRACE_MSG(DC_MISC_2644_112_2_18_1_23_36_174,"---DC_GetContinueShootMaxNum:%d,%d---")
TRACE_MSG(DC_MISC_2712_112_2_18_1_23_36_175,"DCAM: DC_GetContinueShootMaxNum :%d")
TRACE_MSG(DC_MISC_2876_112_2_18_1_23_37_176,"DCAM: DC_GetMemSize mode:%d")
TRACE_MSG(DC_MISC_2979_112_2_18_1_23_37_177,"DCAM: DC_GetMemSize buf_size:0x%x, 0x%xk")
TRACE_MSG(DC_PARAM_CTRL_159_112_2_18_1_23_37_178,"_SetSensorRot, Angle %d")
TRACE_MSG(DC_PARAM_CTRL_215_112_2_18_1_23_38_179,"DCAM: _DCAMERA_SetPreviewEnviroment %d")
TRACE_MSG(DC_PARAM_CTRL_219_112_2_18_1_23_38_180,"DCAM: _SetPreviewEnviroment %d error")
TRACE_MSG(DC_PARAM_CTRL_241_112_2_18_1_23_38_181,"DCAM: _DCAMERA_SetPreviewBrightness %d")
TRACE_MSG(DC_PARAM_CTRL_245_112_2_18_1_23_38_182,"DCAM: _DCAMERA_SetPreviewBrightness %d error")
TRACE_MSG(DC_PARAM_CTRL_267_112_2_18_1_23_38_183,"DCAM: _DCAMERA_SetPreviewContrast %d")
TRACE_MSG(DC_PARAM_CTRL_271_112_2_18_1_23_38_184,"DCAM: _DCAMERA_SetPreviewContrast %d error")
TRACE_MSG(DC_PARAM_CTRL_293_112_2_18_1_23_38_185,"DCAM: _DCAMERA_SetPreviewSharpness %d")
TRACE_MSG(DC_PARAM_CTRL_297_112_2_18_1_23_38_186,"DCAM: _SetPreviewSharpness %d error")
TRACE_MSG(DC_PARAM_CTRL_319_112_2_18_1_23_38_187,"DCAM: _DCAMERA_SetPreviewSaturation %d")
TRACE_MSG(DC_PARAM_CTRL_323_112_2_18_1_23_38_188,"DCAM: _SetPreviewSaturation %d error")
TRACE_MSG(DC_PARAM_CTRL_345_112_2_18_1_23_38_189,"DCAM: _DCAMERA_SetPreviewEffect %d")
TRACE_MSG(DC_PARAM_CTRL_349_112_2_18_1_23_38_190,"DCAM: _DCAMERA_SetPreviewEffect %d error")
TRACE_MSG(DC_PARAM_CTRL_371_112_2_18_1_23_38_191,"DCAM: _DCAMERA_SetPreviewWBMode %d")
TRACE_MSG(DC_PARAM_CTRL_375_112_2_18_1_23_38_192,"DCAM: _DCAMERA_SetPreviewWBMode %d error")
TRACE_MSG(DC_PARAM_CTRL_397_112_2_18_1_23_38_193,"DCAM: _DCAMERA_SetPreviewVideoMode %d")
TRACE_MSG(DC_PARAM_CTRL_401_112_2_18_1_23_38_194,"DCAM: _SetPreviewVideoMode %d error")
TRACE_MSG(DC_PARAM_CTRL_425_112_2_18_1_23_38_195,"DCAM: _DCAMERA_AntiBandingFlicker %d")
TRACE_MSG(DC_PARAM_CTRL_429_112_2_18_1_23_38_196,"DCAM: _DCAMERA_AntiBandingFlicker %d error")
TRACE_MSG(DC_PARAM_CTRL_451_112_2_18_1_23_38_197,"DCAM: _DCAMERA_SetExposureCompensation %d")
TRACE_MSG(DC_PARAM_CTRL_455_112_2_18_1_23_38_198,"DCAM: _SetExposureCompensation %d error")
TRACE_MSG(DC_PARAM_CTRL_478_112_2_18_1_23_38_199,"DCAM: _zoom_callback %d")
TRACE_MSG(DC_PARAM_CTRL_489_112_2_18_1_23_38_200,"DCAM: _zoom_callback error")
TRACE_MSG(DC_PARAM_CTRL_507_112_2_18_1_23_38_201,"DCAM: _SetZoomLevel %d")
TRACE_MSG(DC_PARAM_CTRL_520_112_2_18_1_23_38_202,"DCAM: _SetZoomLevel callback is pnull error!")
TRACE_MSG(DC_PARAM_CTRL_585_112_2_18_1_23_38_203,"DCAM: _SetATVChn op_mode 0x%x, id 0x%x")
TRACE_MSG(DC_PARAM_CTRL_623_112_2_18_1_23_38_204,"DCAM: _SetATVRegion 0x%x")
TRACE_MSG(DC_PARAM_CTRL_644_112_2_18_1_23_38_205,"DCAM: _SetATVVolume 0x%x")
TRACE_MSG(DC_PARAM_CTRL_712_112_2_18_1_23_39_206,"DCAM: _FlashCtrl 0x%x")
TRACE_MSG(DC_PARAM_CTRL_756_112_2_18_1_23_39_207,"DCAM: _FocusCtrl : %d")
TRACE_MSG(DC_PARAM_CTRL_760_112_2_18_1_23_39_208,"DCAM: _FocusCtrl param is null error")
TRACE_MSG(DC_PARAM_CTRL_803_112_2_18_1_23_39_209,"DCAM: _ExposureCtrl : %d")
TRACE_MSG(DC_PARAM_CTRL_807_112_2_18_1_23_39_210,"DCAM: _ExposureCtrl param is null error")
TRACE_MSG(DC_PARAM_CTRL_849_112_2_18_1_23_39_211,"DCAM: _SetTargetSize %d")
TRACE_MSG(DC_PARAM_CTRL_853_112_2_18_1_23_39_212,"DCAM: _SetTargetSize arg:%d error")
TRACE_MSG(DC_PARAM_CTRL_859_112_2_18_1_23_39_213,"DCAM: _SetTargetSize - just record for not preview")
TRACE_MSG(DC_PARAM_CTRL_871_112_2_18_1_23_39_214,"DCAM: _SetTargetSize - do nothing in preview")
TRACE_MSG(DC_PARAM_CTRL_886_112_2_18_1_23_39_215,"DCAM: _SetTargetSize - need to change in preview")
TRACE_MSG(DC_PARAM_CTRL_953_112_2_18_1_23_39_216,"DCAM: DC_GetParamCtrlFunc cmd %d error")
TRACE_MSG(DC_PREVIEW_277_112_2_18_1_23_40_217,"DCAM: _stop_preview")
TRACE_MSG(DC_PREVIEW_290_112_2_18_1_23_40_218,"DCAM: stop preview state err")
TRACE_MSG(DC_PREVIEW_313_112_2_18_1_23_40_219,"DCAM: preview data format %x err!")
TRACE_MSG(DC_PREVIEW_344_112_2_18_1_23_40_220,"DCAM: preview _get_isp_param display buf is null err!")
TRACE_MSG(DC_PREVIEW_383_112_2_18_1_23_40_221,"DCAM: Panorama FourInOne preview distortion %d err!")
TRACE_MSG(DC_REVIEW_203_112_2_18_1_23_41_222,"DCAM: pContext->review_param.disp_mode = %d")
TRACE_MSG(DC_REVIEW_215_112_2_18_1_23_41_223,"DCAM: _adj_disp_mode - rotation mode: %d, disp size: %d, %d, %d, %d;  trim size: %d, %d, %d, %d")
TRACE_MSG(DC_SNAPSHOT_140_112_2_18_1_23_41_224,"DCAM: %s, _check_status, DC is not open")
TRACE_MSG(DC_SNAPSHOT_145_112_2_18_1_23_41_225,"DCAM: %s, _check_status, still preview, stop it")
TRACE_MSG(DC_SNAPSHOT_170_112_2_18_1_23_41_226,"DCAM: capture file name is null err!")
TRACE_MSG(DC_SNAPSHOT_203_112_2_18_1_23_41_227,"TIM_TEST: _DC_CalcPanoramaBuf 0x%x")
TRACE_MSG(DC_SNAPSHOT_366_112_2_18_1_23_42_228,"DCAM: _set_parameters(),snap mode: 0x%x")
TRACE_MSG(DC_SNAPSHOT_469_112_2_18_1_23_42_229,"DCAM: _DC_GetCaptureData(), eb=0x%x, callback:0x%x, line %d")
TRACE_MSG(DC_SNAPSHOT_521_112_2_18_1_23_42_230,"_quick_review_jpeg(),jpeg decoding error,line %d")
TRACE_MSG(DC_SNAPSHOT_635_112_2_18_1_23_42_231,"DCAM:_DC_QuickVeviewDirect")
TRACE_MSG(DC_SNAPSHOT_665_112_2_18_1_23_42_232,"DCAM:_DC_SwitchPanoramaBuffer")
TRACE_MSG(DC_SNAPSHOT_714_112_2_18_1_23_43_233,"DCAM:_DC_SetPanoramaParam the swap buf is not enough err!")
TRACE_MSG(DC_SNAPSHOT_740_112_2_18_1_23_43_234,"DCAM:_DC_SetPanoramaEncoderParam err out_addr:0x%x, out_w:%d, out_h:%d")
TRACE_MSG(DC_SNAPSHOT_746_112_2_18_1_23_43_235,"DCAM:_DC_SetPanoramaEncoder buf err quick_addr:0x%x, data_addr:0x%x")
TRACE_MSG(DC_SNAPSHOT_834_112_2_18_1_23_43_236,"DCAM:_DC_PanoramaCapture")
TRACE_MSG(DC_SNAPSHOT_874_112_2_18_1_23_43_237,"DCAM: Panorama capture call back is pnull err!")
TRACE_MSG(DC_SNAPSHOT_951_112_2_18_1_23_43_238,"DCAM:_DC_SwitchFourInOneBuffer")
TRACE_MSG(DC_SNAPSHOT_1004_112_2_18_1_23_43_239,"DCAM:_DC_SetFourInoneEncoder encoder err!")
TRACE_MSG(DC_SNAPSHOT_1027_112_2_18_1_23_43_240,"trim rect x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(DC_SNAPSHOT_1028_112_2_18_1_23_43_241,"src_rect x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(DC_SNAPSHOT_1062_112_2_18_1_23_43_242,"DCAM: snapshot the data rect out of trim range")
TRACE_MSG(DC_SNAPSHOT_1071_112_2_18_1_23_43_243,"dst_rect x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(DC_SNAPSHOT_1117_112_2_18_1_23_43_244,"DCAM: _DC_Proc_DispDate")
TRACE_MSG(DC_SNAPSHOT_1121_112_2_18_1_23_43_245,"DCAM: org_dateimg_addr 0x%x, w: %d, h:%d")
TRACE_MSG(DC_SNAPSHOT_1207_112_2_18_1_23_44_246,"DCAM: very org_dateimg_addr 0x%x, w: %d, h:%d")
TRACE_MSG(DC_SNAPSHOT_1214_112_2_18_1_23_44_247,"DCAM: very target_img_addr 0x%x, w: %d, h:%d")
TRACE_MSG(DC_SNAPSHOT_1262_112_2_18_1_23_44_248,"output date addr 0x%x, width %d, height %d")
TRACE_MSG(DC_SNAPSHOT_1286_112_2_18_1_23_44_249,"cap date addr 0x%x, x %d y %dwidth %d, height %d")
TRACE_MSG(DC_SNAPSHOT_1291_112_2_18_1_23_44_250,"brush date addr 0x%x, width %d, height %d")
TRACE_MSG(DC_SNAPSHOT_1337_112_2_18_1_23_44_251,"scale datemark src zie x: %d,y: %d,w: %d,h: %d")
TRACE_MSG(DC_SNAPSHOT_1382_112_2_18_1_23_44_252,"scale datemark dst zie x: %d,y: %d,w: %d,h: %d")
TRACE_MSG(DC_SNAPSHOT_1405_112_2_18_1_23_44_253,"DCAM:_DC_FourInOneCapture")
TRACE_MSG(DC_SNAPSHOT_1435_112_2_18_1_23_44_254,"DCAM: FourInOne capture combine call back is pnull err!")
TRACE_MSG(DC_SNAPSHOT_1464_112_2_18_1_23_44_255,"DCAM:_do_snapshot capture type : %d!")
TRACE_MSG(DC_SNAPSHOT_1468_112_2_18_1_23_44_256,"DCAM:_do_snapshot capture mode err!")
TRACE_MSG(DC_SNAPSHOT_1475_112_2_18_1_23_44_257,"DCAM: capture before capture mode set err!")
TRACE_MSG(DC_SNAPSHOT_1521_112_2_18_1_23_44_258,"DCAM: capture after capture mode set err!")
TRACE_MSG(DC_SNAPSHOT_1540_112_2_18_1_23_44_259,"DCAM: _capture_normal")
TRACE_MSG(DC_SNAPSHOT_1563_112_2_18_1_23_44_260,"DCAM: _capture_normal encoder err!")
TRACE_MSG(DC_SNAPSHOT_1598_112_2_18_1_23_44_261,"DCAM: _capture_interpolation")
TRACE_MSG(DC_SNAPSHOT_1611_112_2_18_1_23_44_262,"DCAM: _capture_interpolation encoder err!")
TRACE_MSG(DC_SNAPSHOT_1626_112_2_18_1_23_44_263,"DCAM: _capture_jpeg")
TRACE_MSG(DC_SNAPSHOT_1642_112_2_18_1_23_44_264,"DCAM: _do_capture_jpeg jpg stream is null err!")
TRACE_MSG(DC_SNAPSHOT_1669_112_2_18_1_23_44_265,"DCAM: capture end jpg:0x%x, w:%d, h:%d, jpg_size:0x%x")
TRACE_MSG(DC_SNAPSHOT_1677_112_2_18_1_23_44_266,"DCAM: capture end y:0x%x, uv:0x%x, w:%d, h:%d,")
TRACE_MSG(DC_SNAPSHOT_1682_112_2_18_1_23_44_267,"DCAM: capture err!")
TRACE_MSG(DC_SNAPSHOT_1716_112_2_18_1_23_45_268,"DCAM: DC_GetImageFromLCDC")
TRACE_MSG(DC_SNAPSHOT_1789_112_2_18_1_23_45_269,"DCAM: _capture_personal")
TRACE_MSG(DC_SNAPSHOT_1883_112_2_18_1_23_45_270,"DCAM: _capture_personal encoder err!")
TRACE_MSG(DC_SNAPSHOT_1934_112_2_18_1_23_45_271,"DCAM: _capture_zoom encoder err!")
TRACE_MSG(DC_SNAPSHOT_1945_112_2_18_1_23_45_272,"DCAM: _capture_zoom slice encoder err!")
TRACE_MSG(DC_SNAPSHOT_2045_112_2_18_1_23_45_273,"DCAM: continue shot before capture mode set err!")
TRACE_MSG(DC_SNAPSHOT_2068_112_2_18_1_23_45_274,"DCAM: _capture_burst capture err")
TRACE_MSG(DC_SNAPSHOT_2086_112_2_18_1_23_45_275,"DCAM: _capture_burst quick view err")
TRACE_MSG(DC_SNAPSHOT_2096_112_2_18_1_23_45_276,"DCAM: _capture_burst rotation err")
TRACE_MSG(DC_SNAPSHOT_2143_112_2_18_1_23_45_277,"DCAM: _capture_burst encoder err!")
TRACE_MSG(DC_SNAPSHOT_2166_112_2_18_1_23_45_278,"DCAM: continue shot after capture mode set err!")
TRACE_MSG(DC_SNAPSHOT_2446_112_2_18_1_23_46_279,"DCAM: quick view target w: %d h: %d")
TRACE_MSG(DC_SNAPSHOT_2518_112_2_18_1_23_46_280,"DCAM: _quick_review jpg sample format err!")
TRACE_MSG(DC_SNAPSHOT_2547_112_2_18_1_23_46_281,"_capture_review")
TRACE_MSG(DC_SNAPSHOT_2732_112_2_18_1_23_47_282,"DCAM: continue shot capture mode %d err!")
TRACE_MSG(JPEG_71_112_2_18_1_23_47_283,"[JPEG_GetMCUSize] unsupport quality")
TRACE_MSG(JPEG_122_112_2_18_1_23_47_284,"JPEG_EncodeJpeg ----start, %d")
TRACE_MSG(JPEG_191_112_2_18_1_23_47_285,"JPEG_EncodeJpeg ----end :%d")
TRACE_MSG(JPEG_214_112_2_18_1_23_47_286,"JPEG_DecodeJpeg ----start, %d")
TRACE_MSG(JPEG_257_112_2_18_1_23_47_287,"JPEG_DecodeJpeg ----unsupported yuv format")
TRACE_MSG(JPEG_327_112_2_18_1_23_48_288,"JPEG_DecodeJpeg ----unsupported yuv format")
TRACE_MSG(JPEG_336_112_2_18_1_23_48_289,"JPEG_DecodeJpeg : y_chn_addr = 0x%x,u_chn_addr = 0x%x,v_chn_addr = 0x%x")
TRACE_MSG(JPEG_338_112_2_18_1_23_48_290,"JPEG_DecodeJpeg ----end :%d")
TRACE_MSG(JPEG_351_112_2_18_1_23_48_291,"DC: JPEG_ZoomJpeg -> start time = %d")
TRACE_MSG(JPEG_380_112_2_18_1_23_48_292,"DC: JPEG_ZoomJpeg -> end time = %d, rVal = %d")
TRACE_MSG(JPG_DECODE_245_112_2_18_1_23_50_293,"JPEG_Decode,g_JpegCodec.ratio[0].h_ratio = %d, g_JpegCodec.ratio[0].v_ratio = %d")
TRACE_MSG(DC_CFG_183_112_2_18_1_23_57_294,"_DC_SetExifSpecificParameter %s")
TRACE_MSG(DC_CFG_252_112_2_18_1_23_58_295,"DC_SetExifImageDataTime %s")
TRACE_MSG(DC_CFG_266_112_2_18_1_23_58_296,"DC_GetExifParameter")
TRACE_MSG(DC_COMMON_174_112_2_18_1_23_58_297,"DCAM: unsupport channel format")
TRACE_MSG(DC_COMMON_211_112_2_18_1_23_58_298,"DCAM: DC_ArgbOverlayer src_buf_rect y=%d")
TRACE_MSG(DC_COMMON_212_112_2_18_1_23_58_299,"DCAM: DC_ArgbOverlayer src_buf_rect h=%d")
TRACE_MSG(DC_COMMON_213_112_2_18_1_23_58_300,"DCAM: DC_ArgbOverlayer overlay_rect.y=%d")
TRACE_MSG(DC_COMMON_214_112_2_18_1_23_58_301,"DCAM: DC_ArgbOverlayer overlay_rect.h=%d")
TRACE_MSG(DC_COMMON_220_112_2_18_1_23_58_302,"DCAM: DC_ArgbOverlayer error src_addr:x=%d, w=%d")
TRACE_MSG(DC_COMMON_256_112_2_18_1_23_58_303,"DCAM: DC_ArgbOverlayer src_buf_rect x =%d y=%d")
TRACE_MSG(DC_COMMON_257_112_2_18_1_23_58_304,"DCAM: DC_ArgbOverlayer src_buf_rect w =%d h=%d")
TRACE_MSG(DC_COMMON_258_112_2_18_1_23_58_305,"DCAM: DC_ArgbOverlayer overlay_rect.x =%d y=%d")
TRACE_MSG(DC_COMMON_259_112_2_18_1_23_58_306,"DCAM: DC_ArgbOverlayer overlay_rect.w =%d h=%d")
TRACE_MSG(DC_COMMON_393_112_2_18_1_23_59_307,"DCAMERA_Convert_Coor,src x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(DC_COMMON_394_112_2_18_1_23_59_308,"DCAMERA_Convert_Coor,src degree:%d")
TRACE_MSG(DC_COMMON_428_112_2_18_1_23_59_309,"DCAMERA_Convert_Coor,dst x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(DC_COMMON_429_112_2_18_1_23_59_310,"DCAMERA_Convert_Coor,dst degree:%d")
TRACE_MSG(DCAMERA_APP_385_112_2_18_1_24_0_311,"DC: GetQueueAvailable queue info:enqueued:%d, store: %d, count: %d")
TRACE_MSG(DCAMERA_APP_505_112_2_18_1_24_0_312,"DC: DCAMERA_GetMpegRect, size {%d %d} rect {%d %d %d %d}")
TRACE_MSG(DCAMERA_APP_555_112_2_18_1_24_0_313,"_DCAMERA_GetMutex")
TRACE_MSG(DCAMERA_APP_558_112_2_18_1_24_0_314,"_DCAMERA_GetMutex: Mutex is NULL!")
TRACE_MSG(DCAMERA_APP_570_112_2_18_1_24_0_315,"_DCAMERA_PutMutex")
TRACE_MSG(DCAMERA_APP_573_112_2_18_1_24_0_316,"_DCAMERA_PutMutex: Mutex is NULL!")
TRACE_MSG(DCAMERA_APP_593_112_2_18_1_24_0_317,"_DCAMERA_DcamRect2ISPRect,src x y w h {%d %d %d %d},rotate_ang %d")
TRACE_MSG(DCAMERA_APP_654_112_2_18_1_24_0_318,"_DCAMERA_DcamRect2ISPRect,wrong angle")
TRACE_MSG(DCAMERA_APP_665_112_2_18_1_24_0_319,"_DCAMERA_DcamRect2ISPRect,dst x y w h {%d %d %d %d}")
TRACE_MSG(DCAMERA_APP_683_112_2_18_1_24_0_320,"_DCamera_AdjustRect: fullview")
TRACE_MSG(DCAMERA_APP_713_112_2_18_1_24_0_321,"_DCamera_AdjustRect: fullscreen ")
TRACE_MSG(DCAMERA_APP_753_112_2_18_1_24_0_322,"_DCamera_AdjustRect: any trimming and scaling")
TRACE_MSG(DCAMERA_APP_763_112_2_18_1_24_0_323,"_DCamera_AdjustRect,trim rect x,y,w,h {%d %d %d %d}")
TRACE_MSG(DCAMERA_APP_878_112_2_18_1_24_1_324,"DCAMERA_SetSensorType: %d")
TRACE_MSG(DCAMERA_APP_928_112_2_18_1_24_1_325,"_DCAMERA_InitSensor, sensor %d is not exist")
TRACE_MSG(DCAMERA_APP_933_112_2_18_1_24_1_326,"_DCAMERA_InitSensor, No sensor exist")
TRACE_MSG(DCAMERA_APP_976_112_2_18_1_24_1_327,"DC: ISP output one frame in capture at %d")
TRACE_MSG(DCAMERA_APP_981_112_2_18_1_24_1_328,"DC: yaddr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(DCAMERA_APP_993_112_2_18_1_24_1_329,"DC: Exit ISP CAPTURE FRAME callback at %d")
TRACE_MSG(DCAMERA_APP_1008_112_2_18_1_24_1_330,"_DCAMERA_ISP_SCALE_CALLBACK: FLAG 0x%08x")
TRACE_MSG(DCAMERA_APP_1081_112_2_18_1_24_1_331,"UPCC: JPEG ENCODE ERR!!")
TRACE_MSG(DCAMERA_APP_1089_112_2_18_1_24_1_332,"UPCC_Camera.c->UPCC_GetAndSendFrame(): one frame lost.")
TRACE_MSG(DCAMERA_APP_1100_112_2_18_1_24_1_333,"DCAMERA_app.c->DCAMERA_RecordCallback()  NO call back !!")
TRACE_MSG(DCAMERA_APP_1177_112_2_18_1_24_1_334,"DC: _DCAMERA_DoSnapShotExDecJpgFrame, snapshot_ex_yuv  0x%x  snapshot_ex_addi_mem 0x%x")
TRACE_MSG(DCAMERA_APP_1183_112_2_18_1_24_1_335,"DC: _DCAMERA_DoSnapShotExDecJpgFrame, s_dc_info_ptr->snapshot_ex_yuv is NULL! ")
TRACE_MSG(DCAMERA_APP_1257_112_2_18_1_24_1_336,"DC: _DCAMERA_DoSnapShotDecJpgFrame Entry Time = %d")
TRACE_MSG(DCAMERA_APP_1317_112_2_18_1_24_2_337,"DC: _DCAMERA_DoSnapShotDecJpgFrame End Time = %d")
TRACE_MSG(DCAMERA_APP_1341_112_2_18_1_24_2_338,"DC _DCAMERA_DoSnapShotReview, start time %d  ")
TRACE_MSG(DCAMERA_APP_1393_112_2_18_1_24_2_339,"DC _DCAMERA_DoSnapShotReview , disp_rect {%d %d}, src {%d %d}")
TRACE_MSG(DCAMERA_APP_1419_112_2_18_1_24_2_340,"DC _DCAMERA_DoSnapShotReview , zoom %d , trim {%d %d}")
TRACE_MSG(DCAMERA_APP_1483_112_2_18_1_24_2_341,"DC _DCAMERA_DoSnapShotReview end, end time %d  ")
TRACE_MSG(DCAMERA_APP_1499_112_2_18_1_24_2_342,"DC _DCAMERA_DoSnapShotRotation, need rotation")
TRACE_MSG(DCAMERA_APP_1559_112_2_18_1_24_2_343,"DC _DCAMERA_DoSnapShotRotation end, end time %d  ")
TRACE_MSG(DCAMERA_APP_1686_112_2_18_1_24_2_344,"DC: Call JPEG_EncodeJpeg, start time = %d")
TRACE_MSG(DCAMERA_APP_1691_112_2_18_1_24_2_345,"y_addr = 0x%x,u_addr = 0x%x,v_addr = 0x%x,jpeg_buf_ptr = 0x%x")
TRACE_MSG(DCAMERA_APP_1806_112_2_18_1_24_2_346,"DC: Call IMGJPEG_EncodeEx,Error")
TRACE_MSG(DCAMERA_APP_1835_112_2_18_1_24_3_347,"DC: Call JPEG_EncodeJpeg, end time = %d")
TRACE_MSG(DCAMERA_APP_1888_112_2_18_1_24_3_348,"DC: _DCAMERA_HandleSensorTargetSize -> Target width = %d, target height = %d")
TRACE_MSG(DCAMERA_APP_1894_112_2_18_1_24_3_349,"DC: _DCAMERA_HandleSensorTargetSize -> Need Interpolation !!!!!! ")
TRACE_MSG(DCAMERA_APP_1905_112_2_18_1_24_3_350,"DC: _DCAMERA_HandleSensorTargetSize -> search_width = %d")
TRACE_MSG(DCAMERA_APP_1913_112_2_18_1_24_3_351,"DC: _DCAMERA_HandleSensorTargetSize -> width = %d")
TRACE_MSG(DCAMERA_APP_1936_112_2_18_1_24_3_352,"DC: _DCAMERA_HandleSensorTargetSize,can't find the right mode")
TRACE_MSG(DCAMERA_APP_1943_112_2_18_1_24_3_353,"DC: _DCAMERA_HandleSensorTargetSize -> preview_m = %d, snapshot_m = %d")
TRACE_MSG(DCAMERA_APP_1951_112_2_18_1_24_3_354,"DC: _DCAMERA_HandleSensorTargetSize -> snap_width = %d, snap_height = %d")
TRACE_MSG(DCAMERA_APP_1963_112_2_18_1_24_3_355,"DC: _DCAMERA_HandleSensorTargetSize -> preview_width = %d, preview_height = %d")
TRACE_MSG(DCAMERA_APP_1996_112_2_18_1_24_3_356,"DC: _DCAMERA_HandleSensorTargetSize ->(preview) zoom width step = %d, zoom height step = %d")
TRACE_MSG(DCAMERA_APP_2000_112_2_18_1_24_3_357,"DC: _DCAMERA_HandleSensorTargetSize ->(snapshot) zoom width step = %d, zoom height step = %d")
TRACE_MSG(DCAMERA_APP_2112_112_2_18_1_24_3_358,"_DCAMERA_SetPreviewMeterMOde: meter mode of sensor error,mode = %d")
TRACE_MSG(DCAMERA_APP_2281_112_2_18_1_24_3_359,"_DCAMERA_SetSensorRot, Angle %d")
TRACE_MSG(DCAMERA_APP_2314_112_2_18_1_24_3_360,"_DCAMERA_ROT_Convertor: %d")
TRACE_MSG(DCAMERA_APP_2350_112_2_18_1_24_4_361,"_DCAMERA_ROT_Convertor: img: %d, ret: %d")
TRACE_MSG(DCAMERA_APP_2441_112_2_18_1_24_4_362,"DCAM: _DCAMERA_CheckTargetSize, photo size: width = %d, height = %d")
TRACE_MSG(DCAMERA_APP_2445_112_2_18_1_24_4_363,"DCAM: invalid input arg")
TRACE_MSG(DCAMERA_APP_2518_112_2_18_1_24_4_364,"_DCAMERA_SetPreviewSubZoom last preview x = %d y = %d, w = %d h = %d")
TRACE_MSG(DCAMERA_APP_2541_112_2_18_1_24_4_365,"_DCAMERA_SetPreviewZoom : sub zoom step level %d")
TRACE_MSG(DCAMERA_APP_2604_112_2_18_1_24_4_366,"_DCAMERA_SetPreviewZoom : sub zoom step level %d")
TRACE_MSG(DCAMERA_APP_2676_112_2_18_1_24_4_367,"_DCAMERA_SetVTZoom, zoom level setting ok")
TRACE_MSG(DCAMERA_APP_2711_112_2_18_1_24_4_368,"_DCAMERA_SetVTZoom: level = %d, x = %d y = %d, w = %d h = %d")
TRACE_MSG(DCAMERA_APP_2745_112_2_18_1_24_4_369,"_DCAMERA_SetMpegZoom, zoom level setting ok")
TRACE_MSG(DCAMERA_APP_2753_112_2_18_1_24_4_370,"_DCAMERA_SetMpegZoom, Failed to get MPEG parameter")
TRACE_MSG(DCAMERA_APP_2781_112_2_18_1_24_4_371,"_DCAMERA_SetMpegZoom: level = %d, x = %d y = %d, w = %d h = %d")
TRACE_MSG(DCAMERA_APP_2823_112_2_18_1_24_4_372,"_DCAMERA_SetPreviewZoom, dc_info_ptr->preview_ctl_info.zoom_level = %d, zoom_level = %d")
TRACE_MSG(DCAMERA_APP_2844_112_2_18_1_24_4_373,"_DCAMERA_SetPreviewZoom, wrong preview mode %d")
TRACE_MSG(DCAMERA_APP_2899_112_2_18_1_24_5_374,"_DCAMERA_SetPreviewZoom, logical coordinate ,lcd_rect {%d %d %d %d},disp_range {%d %d %d %d}, rot %d")
TRACE_MSG(DCAMERA_APP_2922_112_2_18_1_24_5_375,"_DCAMERA_SetPreviewZoom : disp_range = %d %d %d %d")
TRACE_MSG(DCAMERA_APP_2958_112_2_18_1_24_5_376,"DC ZOOM: level = %d, x = %d y = %d, w = %d h = %d, rotation = %d")
TRACE_MSG(DCAMERA_APP_3007_112_2_18_1_24_5_377,"_DCAMERA_SetPreviewEXZoom, dc_info_ptr->preview_ctl_info.zoom_level = %d, zoom_level = %d")
TRACE_MSG(DCAMERA_APP_3122_112_2_18_1_24_5_378,"DCAM: _DCAMERA_SetATVChn op_mode 0x%x, id 0x%x")
TRACE_MSG(DCAMERA_APP_3150_112_2_18_1_24_5_379,"DCAM: _DCAMERA_SetATVRegion 0x%x")
TRACE_MSG(DCAMERA_APP_3170_112_2_18_1_24_5_380,"DCAM: _DCAMERA_SetATVVolume 0x%x")
TRACE_MSG(DCAMERA_APP_3199_112_2_18_1_24_5_381,"DC: _DCAMERA_SetPreviewFlag -> is_preview = %d")
TRACE_MSG(DCAMERA_APP_3227_112_2_18_1_24_5_382,"DC: _DCAMERA_SetPreviewExFlag -> is_preview = %d")
TRACE_MSG(DCAMERA_APP_3401_112_2_18_1_24_6_383,"DCAMERA_CheckPreviewParam:: Don't need to check the %d cmd and %d arg")
TRACE_MSG(DCAMERA_APP_3407_112_2_18_1_24_6_384,"DCAMERA_CheckPreviewParam:: Error")
TRACE_MSG(DCAMERA_APP_3430_112_2_18_1_24_6_385,"DC: _DCAMERA_PreviewControl -> cmd = %d, arg = %d")
TRACE_MSG(DCAMERA_APP_3519_112_2_18_1_24_6_386,"DC: _DCAMERA_PreviewControl: error cmd %d")
TRACE_MSG(DCAMERA_APP_3523_112_2_18_1_24_6_387,"DC: _DCAMERA_PreviewControl, not in preview")
TRACE_MSG(DCAMERA_APP_3677_112_2_18_1_24_6_388,"DC: _DCAMERA_PreviewControl: error cmd %d")
TRACE_MSG(DCAMERA_APP_3713_112_2_18_1_24_6_389,"_DCAMERA_ImageYUV422ToYUV420 ,size_each_line %d, height %d")
TRACE_MSG(DCAMERA_APP_3821_112_2_18_1_24_6_390,"_DCAMERA_ImageScale ,unsupported output format %d")
TRACE_MSG(DCAMERA_APP_3861_112_2_18_1_24_7_391,"_DCAMERA_JpgAddExif")
TRACE_MSG(DCAMERA_APP_3940_112_2_18_1_24_7_392,"_DCAMERA_JpgAddExif, Failed to encode thumbnail")
TRACE_MSG(DCAMERA_APP_3970_112_2_18_1_24_7_393,"_DCAMERA_JpgAddExif, Failed to write exif information , %d")
TRACE_MSG(DCAMERA_APP_3977_112_2_18_1_24_7_394,"_DCAMERA_JpgAddExif, jpg buffer 0x%x, lenght %d")
TRACE_MSG(DCAMERA_APP_4020_112_2_18_1_24_7_395,"DC: DCAMERA_DoBurstSnapshot -> start time = %d,image_count = %d")
TRACE_MSG(DCAMERA_APP_4044_112_2_18_1_24_7_396,"DC: DoBurstSnapshot -> save in buffer ")
TRACE_MSG(DCAMERA_APP_4073_112_2_18_1_24_7_397,"DC: DCAMERA_DoBurstSnapshot - > Snapshot single err %d")
TRACE_MSG(DCAMERA_APP_4096_112_2_18_1_24_7_398,"DC: DCAMERA_DoBurstSnapshot - > interval time of frame %d")
TRACE_MSG(DCAMERA_APP_4113_112_2_18_1_24_7_399,"DC: DCAMERA_DoBurstSnapshot -> end time = %d")
TRACE_MSG(DCAMERA_APP_4326_112_2_18_1_24_7_400,"DC: _DCAMERA_TaskRoutine() don't handle %d command")
TRACE_MSG(DCAMERA_APP_4479_112_2_18_1_24_8_401,"DC: _DCAMERA_RotateYUV420, rot_mode=%d,format=%d,img_size.w,h %d %d,")
TRACE_MSG(DCAMERA_APP_4486_112_2_18_1_24_8_402,"DC: _DCAMERA_RotateYUV420,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x")
TRACE_MSG(DCAMERA_APP_4494_112_2_18_1_24_8_403,"DC: _DCAMERA_RotateYUV420-Y, SUCCESS")
TRACE_MSG(DCAMERA_APP_4499_112_2_18_1_24_8_404,"DC: _DCAMERA_RotateYUV420-Y, FAIL")
TRACE_MSG(DCAMERA_APP_4525_112_2_18_1_24_8_405,"DC: _DCAMERA_RotateYUV422, rot_mode=%d,format=%d,img_size.w,h %d %d,")
TRACE_MSG(DCAMERA_APP_4532_112_2_18_1_24_8_406,"DC: _DCAMERA_RotateYUV422,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x")
TRACE_MSG(DCAMERA_APP_4540_112_2_18_1_24_8_407,"DC: _DCAMERA_RotateYUV422, SUCCESS")
TRACE_MSG(DCAMERA_APP_4545_112_2_18_1_24_8_408,"DC: _DCAMERA_RotateYUV422, FAIL")
TRACE_MSG(DCAMERA_APP_4579_112_2_18_1_24_8_409,"DC: _DCAMERA_RotateRGB565, rot_mode=%d,format=%d,img_size.w,h %d %d,")
TRACE_MSG(DCAMERA_APP_4586_112_2_18_1_24_8_410,"DC: _DCAMERA_RotateRGB565,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x")
TRACE_MSG(DCAMERA_APP_4591_112_2_18_1_24_8_411,"DC: _DCAMERA_RotateRGB565, SUCCESS")
TRACE_MSG(DCAMERA_APP_4596_112_2_18_1_24_8_412,"DC: _DCAMERA_RotateRGB565, FAIL")
TRACE_MSG(DCAMERA_APP_4704_112_2_18_1_24_8_413,"DC: few memory, reduce the output mode of sensor")
TRACE_MSG(DCAMERA_APP_4727_112_2_18_1_24_8_414,"DC: few memory, scaling down the image size of one special output mode")
TRACE_MSG(DCAMERA_APP_4764_112_2_18_1_24_8_415,"DC: _DCAMERA_GetCapability, photo size %d")
TRACE_MSG(DCAMERA_APP_4908_112_2_18_1_24_9_416,"_DCAMERA_PreviewSensorSet:meter mode of sensor error,mode = %d")
TRACE_MSG(DCAMERA_APP_5012_112_2_18_1_24_9_417,"_DCAMERA_PreviewSensorSet :: brightness %d, contrast %d, sharpness %d, sturation %d, effect %d, envir %d, wb_mode %d,zoom lel %d sensor_rot %d")
TRACE_MSG(DCAMERA_APP_5061_112_2_18_1_24_9_418,"DCAMERA_Open, mode %d")
TRACE_MSG(DCAMERA_APP_5068_112_2_18_1_24_9_419,"-----open fmark------")
TRACE_MSG(DCAMERA_APP_5088_112_2_18_1_24_9_420,"DC: DCAMERA_Open, memory error!")
TRACE_MSG(DCAMERA_APP_5174_112_2_18_1_24_9_421,"DCAMERA_Close")
TRACE_MSG(DCAMERA_APP_5179_112_2_18_1_24_9_422,"-----close fmark------")
TRACE_MSG(DCAMERA_APP_5214_112_2_18_1_24_9_423,"DCAMERA_Close")
TRACE_MSG(DCAMERA_APP_5337_112_2_18_1_24_9_424,"DC: DCAMERA_GetInfo, sensor size 0x%x")
TRACE_MSG(DCAMERA_APP_5614_112_2_18_1_24_10_425,"DC: _DCAMERA_FFSSave, p_data_buf 0x%x buf_len 0x%x")
TRACE_MSG(DCAMERA_APP_5643_112_2_18_1_24_10_426,"DC: image_notice_callback, image count = %d, data_type %d, data_pattern %d ,width %d height %d")
TRACE_MSG(DCAMERA_APP_5649_112_2_18_1_24_10_427,"DC: image_notice_callback, p_user 0x%x, address 0x%x,0x%x,0x%x")
TRACE_MSG(DCAMERA_APP_5711_112_2_18_1_24_10_428,"DC: preview_ex_display_callback, rtn %d ,p_param %d")
TRACE_MSG(DCAMERA_APP_5721_112_2_18_1_24_10_429,"Src 0x%x 0x%x {%d %d}, Dst 0x%x 0x%x {%d %d}")
TRACE_MSG(DCAMERA_APP_5817_112_2_18_1_24_10_430,"DC: DCAMERA_StartPreview -> start time = %d")
TRACE_MSG(DCAMERA_APP_5849_112_2_18_1_24_10_431,"DC: display region , x %d,y %d,w %d,h %d")
TRACE_MSG(DCAMERA_APP_5943_112_2_18_1_24_11_432,"DC: DCAMERA_StartPreview -> end time = %d, return value %d")
TRACE_MSG(DCAMERA_APP_5961_112_2_18_1_24_11_433,"DC: _DCAMERA_PreviewCheckParam disp_mode %d, rot %d, disp_rect {%d %d %d %d}")
TRACE_MSG(DCAMERA_APP_5997_112_2_18_1_24_11_434,"DC: _DCAMERA_PreviewEXCheckParam image frame count %d")
TRACE_MSG(DCAMERA_APP_6011_112_2_18_1_24_11_435,"DC: _DCAMERA_PreviewEXCheckParam image size errror w,h %d %d")
TRACE_MSG(DCAMERA_APP_6021_112_2_18_1_24_11_436,"DC: _DCAMERA_PreviewEXCheckParam image data type errror %d")
TRACE_MSG(DCAMERA_APP_6027_112_2_18_1_24_11_437,"DC: _DCAMERA_PreviewEXCheckParam slice scaling for YUV420 is unsupported")
TRACE_MSG(DCAMERA_APP_6036_112_2_18_1_24_11_438,"DC: _DCAMERA_PreviewEXCheckParam image %d,width %d height %d,data type %d Rot %d")
TRACE_MSG(DCAMERA_APP_6180_112_2_18_1_24_11_439,"_DCAMERA_PreviewDisplayCallback, frame 0x%x, yaddr 0x%x, uaddr 0x%x")
TRACE_MSG(DCAMERA_APP_6185_112_2_18_1_24_11_440,"_DCAMERA_PreviewDisplayCallback, Source Frame locked! 0x%x")
TRACE_MSG(DCAMERA_APP_6192_112_2_18_1_24_11_441,"_DCAMERA_PreviewDisplayCallback, Destination Frame locked! 0x%x")
TRACE_MSG(DCAMERA_APP_6223_112_2_18_1_24_11_442,"_DCAMERA: drop the current frame")
TRACE_MSG(DCAMERA_APP_6252_112_2_18_1_24_11_443,"DC: DCAMERA_StartPreviewEX -> start time = %d")
TRACE_MSG(DCAMERA_APP_6282_112_2_18_1_24_11_444,"DC: DCAMERA_StartPreviewEX, preview parameter error")
TRACE_MSG(DCAMERA_APP_6370_112_2_18_1_24_11_445,"DC: DCAMERA_StartPreviewEX, preview parameter, preview size %d %d")
TRACE_MSG(DCAMERA_APP_6401_112_2_18_1_24_11_446,"DC: DCAMERA_StartPreviewEX, preview parameter, encode_size %d %d")
TRACE_MSG(DCAMERA_APP_6507_112_2_18_1_24_12_447,"DC: DCAMERA_StartPreviewEX, preview parameter error")
TRACE_MSG(DCAMERA_APP_6576_112_2_18_1_24_12_448,"DC: DCAMERA_StartPreviewEX, no memory!")
TRACE_MSG(DCAMERA_APP_6583_112_2_18_1_24_12_449,"DC: DCAMERA_StartPreviewEX, p_preview_buf 0x%x, preview_buf_len %d")
TRACE_MSG(DCAMERA_APP_6599_112_2_18_1_24_12_450,"DC: DCAMERA_StartPreviewEX, No memory! Total 0x%x")
TRACE_MSG(DCAMERA_APP_6622_112_2_18_1_24_12_451,"DCAMERA_StartPreviewEX, disp src frame %d,0x%x, {0x%x 0x%x} ,pointer 0x%x 0x%x")
TRACE_MSG(DCAMERA_APP_6646_112_2_18_1_24_12_452,"DCAMERA_StartPreviewEX, disp dst frame %d,0x%x, {0x%x 0x%x} ,pointer 0x%x 0x%x")
TRACE_MSG(DCAMERA_APP_6677_112_2_18_1_24_12_453,"DC: DCAMERA_StartPreviewEX, ISP Error!")
TRACE_MSG(DCAMERA_APP_6686_112_2_18_1_24_12_454,"DC: _DCAMERA_GetScaledImageData width height %d %d, addr 0x%x 0x%x")
TRACE_MSG(DCAMERA_APP_6844_112_2_18_1_24_12_455,"_DCAMERA_DoImageScaleInVT, Scaling parameter ERROR!")
TRACE_MSG(DCAMERA_APP_6860_112_2_18_1_24_12_456,"DC: DCAMERA_StopPreview")
TRACE_MSG(DCAMERA_APP_6895_112_2_18_1_24_12_457,"DCAMERA_StopPreview,stop error,preview is already been stop")
TRACE_MSG(DCAMERA_APP_6992_112_2_18_1_24_13_458,"_DCAMERA_YUV422DownSample2YUV420 ,size_each_line %d, height %d")
TRACE_MSG(DCAMERA_APP_7102_112_2_18_1_24_13_459,"_DCAMERA_GetSpecificImage, JPEG buf 0x%x, length 0x%x mem_start  0x%x ")
TRACE_MSG(DCAMERA_APP_7123_112_2_18_1_24_13_460,"DC, Add exif error %d")
TRACE_MSG(DCAMERA_APP_7185_112_2_18_1_24_13_461,"DC: Call JPEG_EncodeJpeg, start time = %d,jpeg_buf_ptr = 0x%x")
TRACE_MSG(DCAMERA_APP_7233_112_2_18_1_24_13_462,"DC: Call JPEG_EncodeJpeg, end time = %d")
TRACE_MSG(DCAMERA_APP_7322_112_2_18_1_24_13_463,"_DCAMERA_GetSpecificImage, YUV no transformed ,y 0x%x, u 0x%x ")
TRACE_MSG(DCAMERA_APP_7347_112_2_18_1_24_13_464,"_DCAMERA_SnapshotExFreeMem , free s_dc_info_ptr->snapshot_ex_yuv 0x%x, length %d")
TRACE_MSG(DCAMERA_APP_7356_112_2_18_1_24_13_465,"_DCAMERA_SnapshotExFreeMem , free s_dc_info_ptr->snapshot_ex_addi_mem 0x%x, length %d")
TRACE_MSG(DCAMERA_APP_7419_112_2_18_1_24_13_466,"DC _DCAMERA_DoSnapShotExReview, start time %d  ")
TRACE_MSG(DCAMERA_APP_7486_112_2_18_1_24_14_467,"DC _DCAMERA_DoSnapShotExReview end, end time %d  ")
TRACE_MSG(DCAMERA_APP_7522_112_2_18_1_24_14_468,"DC: _DCAMERA_DoSnapshotEx -> start time = %d,image_count = %d,exif_need %d")
TRACE_MSG(DCAMERA_APP_7634_112_2_18_1_24_14_469,"DC: _DCAMERA_DoSnapshotEx  image %d,width %d height %d,data type %d, Rot %d, mem_size 0x%x")
TRACE_MSG(DCAMERA_APP_7661_112_2_18_1_24_14_470,"DC: _DCAMERA_DoSnapshotEx, target image buffer 0x%x size %d")
TRACE_MSG(DCAMERA_APP_7689_112_2_18_1_24_14_471,"DC: _DCAMERA_DoSnapshotEx, no need to alloc yuv source buffer!")
TRACE_MSG(DCAMERA_APP_7716_112_2_18_1_24_14_472,"DC: _DCAMERA_DoSnapshotEx, YUV buffer for the middle image 0x%x, length 0x%x")
TRACE_MSG(DCAMERA_APP_7735_112_2_18_1_24_14_473,"DC: DCAMERA_GetAFrameFromISP ERROR")
TRACE_MSG(DCAMERA_APP_7810_112_2_18_1_24_14_474,"DC: _DCAMERA_DoSnapshotEx reuse_image_id %d width  height %d %d ")
TRACE_MSG(DCAMERA_APP_7849_112_2_18_1_24_14_475,"DC: _DCAMERA_DoSnapshotEx - > interval time of frame %d")
TRACE_MSG(DCAMERA_APP_7883_112_2_18_1_24_14_476,"DC: _DCAMERA_DoSnapshotEx -> end time = %d, rtn_code %d")
TRACE_MSG(DCAMERA_APP_7898_112_2_18_1_24_14_477,"DC: _DCAMERA_DoSnapshotEx,target frame count or snapshot number error")
TRACE_MSG(DCAMERA_APP_7906_112_2_18_1_24_14_478,"DC: _DCAMERA_DoSnapshotEx,data type of %d-th target_info error")
TRACE_MSG(DCAMERA_APP_7914_112_2_18_1_24_14_479,"DC: _DCAMERA_DoSnapshotEx,target size exceed the largest size output from sensor")
TRACE_MSG(DCAMERA_APP_7921_112_2_18_1_24_14_480,"DC: _DCAMERA_DoSnapshotEx,target size must be aligned by 4 pixels")
TRACE_MSG(DCAMERA_APP_7928_112_2_18_1_24_14_481,"DC: _DCAMERA_DoSnapshotEx,Rotation mode not supported for RAW data")
TRACE_MSG(DCAMERA_APP_7967_112_2_18_1_24_15_482,"DC: DCAMERA_DoSnapshotEX,YUV420 slice scaling is not supported")
TRACE_MSG(DCAMERA_APP_8069_112_2_18_1_24_15_483,"DCAMERA_ParamCtrl: dc is not open!")
TRACE_MSG(DCAMERA_APP_8125_112_2_18_1_24_15_484,"DC DCAMERA_ReviewJpeg:start")
TRACE_MSG(DCAMERA_APP_8139_112_2_18_1_24_15_485,"DC : DCAMERA_ReviewJpeg -> start time = %d")
TRACE_MSG(DCAMERA_APP_8236_112_2_18_1_24_15_486,"DC : ISP_ServiceSetFrameMemory yaddr = 0x%x, uaddr = 0x%x,vaddr = 0x%x")
TRACE_MSG(DCAMERA_APP_8279_112_2_18_1_24_16_487,"DC: review JPEG -> trimming x = %d, y = %d, w = %d, h = %d, rotation %d, distortion %d")
TRACE_MSG(DCAMERA_APP_8326_112_2_18_1_24_16_488,"DC : DCAMERA_ReviewJpeg -> end time = %d")
TRACE_MSG(DCAMERA_APP_8348_112_2_18_1_24_16_489,"DCAMERA_StartRecord: Start time %d")
TRACE_MSG(DCAMERA_APP_8356_112_2_18_1_24_16_490,"DCAMERA_ParamCtrl: dc is not open!")
TRACE_MSG(DCAMERA_APP_8564_112_2_18_1_24_16_491,"DCAMERA_StartRecord: End time %d, return value %d ")
TRACE_MSG(DCAMERA_APP_8579_112_2_18_1_24_16_492,"DC: DCAMERA_StopRecord")
TRACE_MSG(DCAMERA_APP_8692_112_2_18_1_24_17_493,"DC_SetCurImgSensorId, sensor_id %d")
TRACE_MSG(DCAMERA_APP_8730_112_2_18_1_24_17_494,"DC: DCAMERA_Open, memory error!")
TRACE_MSG(DCAMERA_APP_8854_112_2_18_1_24_17_495,"DC: _DCAMERA_BackupPreviewbuf2ReviewBuf quick_view_size 0x%x, width %d blk_info.end_y %d blk_info.start_y %d")
TRACE_MSG(DCAMERA_APP_8868_112_2_18_1_24_17_496,"DC: _DCAMERA_BackupPreviewbuf2ReviewBuf dst 0x%x, length %d, src y 0x%x")
TRACE_MSG(DCAMERA_APP_8960_112_2_18_1_24_18_497,"_DCAMERA_CapMemAssign, captured size %d %d")
TRACE_MSG(DCAMERA_APP_9040_112_2_18_1_24_18_498,"DC: few memory, reduce the output mode of sensor")
TRACE_MSG(DCAMERA_APP_9151_112_2_18_1_24_18_499,"_DCAMERA_GetCaptureTrimRect, src {%d %d}, dst {%d %d},trim {%d %d %d %d},zoom_step {%d %d")
TRACE_MSG(DCAMERA_APP_9166_112_2_18_1_24_18_500,"_DCAMERA_GetCaptureTrimRect,p_trim_rect {%d %d %d %d}")
TRACE_MSG(DCAMERA_APP_9178_112_2_18_1_24_18_501,"_DCAMERA_GetScaledData, width %d, height %d, y addr 0x%x, uv addr 0x%x")
TRACE_MSG(DCAMERA_APP_9222_112_2_18_1_24_18_502,"_DCAMERA_GetDateRectParam: trim rect x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(DCAMERA_APP_9223_112_2_18_1_24_18_503,"_DCAMERA_GetDateRectParam: src_rect x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(DCAMERA_APP_9295_112_2_18_1_24_18_504,"_DCAMERA_GetDateRectParam: snapshot the data rect out of trim range")
TRACE_MSG(DCAMERA_APP_9304_112_2_18_1_24_18_505,"_DCAMERA_GetDateRectParam: dst_rect x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(DCAMERA_APP_9339_112_2_18_1_24_19_506,"_DCAMERA_getDateRectData, date mark rect info is not valid!")
TRACE_MSG(DCAMERA_APP_9345_112_2_18_1_24_19_507,"_DCAMERA_getDateRectData, error source or dest date mark address !")
TRACE_MSG(DCAMERA_APP_9405_112_2_18_1_24_19_508,"_DCAMERA_dateBlend: date addr 0x%x, x %d y %dwidth %d, height %d")
TRACE_MSG(DCAMERA_APP_9417_112_2_18_1_24_19_509,"_DCAMERA_dateBlend: date addr 0x%x, x %d y %dwidth %d, height %d")
TRACE_MSG(DCAMERA_APP_9452_112_2_18_1_24_19_510,"_DCAMERA_handleDateMark: the dst date mark rect is not valid!!")
TRACE_MSG(DCAMERA_APP_9459_112_2_18_1_24_19_511,"_DCAMERA_handleDateMark: date mark memory alloc fail")
TRACE_MSG(DCAMERA_APP_9468_112_2_18_1_24_19_512,"_DCAMERA_handleDateMark: date mark data duplicate error!")
TRACE_MSG(DCAMERA_APP_9507_112_2_18_1_24_19_513,"_DCAMERA_handleScaleAndDate: start calculate the date mark scale coeff!")
TRACE_MSG(DCAMERA_APP_9567_112_2_18_1_24_19_514,"_DCAMERA_handleScaleAndDate: the scale coeff(256x) is %d")
TRACE_MSG(DCAMERA_APP_9636_112_2_18_1_24_19_515,"_DCAMERA_handleScaleAndDate: date mark scaling failed!")
TRACE_MSG(DCAMERA_APP_9640_112_2_18_1_24_19_516,"_DCAMERA_handleScaleAndDate: date mark scaling successed!")
TRACE_MSG(DCAMERA_APP_9651_112_2_18_1_24_19_517,"_DCAMERA_handleScaleAndDate: start blender the date mark!")
TRACE_MSG(DCAMERA_APP_9722_112_2_18_1_24_19_518,"_DCAMERA_JpgEncGetSlice,photo_width =%d, get_slice_width=%d, line_offset %d, read_lines %d, yaddr 0x%x, uv 0x%x")
TRACE_MSG(DCAMERA_APP_9729_112_2_18_1_24_19_519,"_DCAMERA_JpgEncGetSlice,photo_height=%d,line_offsert=%d")
TRACE_MSG(DCAMERA_APP_9823_112_2_18_1_24_20_520,"_DCAMERA_JpgEncGetSlice,No source slice buffer, y 0x%x u 0x%x")
TRACE_MSG(DCAMERA_APP_10013_112_2_18_1_24_20_521,"DC: _DBGDC_DoSnapShotJpgZoom flag")
TRACE_MSG(DCAMERA_APP_10261_112_2_18_1_24_21_522,"DC: _DCAMERA_DoSnapshot -> start time = %d,rot %d")
TRACE_MSG(DCAMERA_APP_10327_112_2_18_1_24_21_523,"DC: DCAMERA_GetAFrameFromIsp, start time %d  ")
TRACE_MSG(DCAMERA_APP_10341_112_2_18_1_24_21_524,"DC: DCAMERA_GetAFrameFromIsp ERROR")
TRACE_MSG(DCAMERA_APP_10353_112_2_18_1_24_21_525,"DC DCAMERA_GetAFrameFromIsp, end time %d  ")
TRACE_MSG(DCAMERA_APP_10408_112_2_18_1_24_21_526,"DC: _DCAMERA_DoSnapShot -> flag")
TRACE_MSG(DCAMERA_APP_10485_112_2_18_1_24_21_527,"DC: DCAMERA_DoSnapshot -> end time = %d, callback return %d, snapshot return %d")
TRACE_MSG(DCAMERA_APP_10524_112_2_18_1_24_21_528,"DCAMERA_ATVAutoScanChn: In Time: %d")
TRACE_MSG(DCAMERA_APP_10536_112_2_18_1_24_21_529,"DCAMERA_ATVAutoScanChn: Out Time: %d")
TRACE_MSG(DCAMERA_APP_10546_112_2_18_1_24_21_530,"DCAM: DCAMERA_ATVStopScanChn: Time: %d")
TRACE_MSG(DCAMERA_APP_10570_112_2_18_1_24_21_531,"DCAMERA_DisplayOneFrame")
TRACE_MSG(DCAMERA_APP_10592_112_2_18_1_24_21_532,"DCAMERA_DisplayOneFrame,first frame,no need to free frame")
TRACE_MSG(DCAMERA_APP_10597_112_2_18_1_24_21_533,"DCAMERA_DisplayOneFrame, free last frame, 0x%x")
TRACE_MSG(DCAMERA_APP_10614_112_2_18_1_24_21_534,"_DCAMERA_OpenISP, Failed to Open ISP_Service")
TRACE_MSG(DCAMERA_APP_10634_112_2_18_1_24_21_535,"_DCAMERA_CloseISP, Failed to Close ISP_Service")
TRACE_MSG(DCAMERA_APP_10646_112_2_18_1_24_21_536,"DC: ISP output one frame in capture at %d")
TRACE_MSG(DCAMERA_APP_10651_112_2_18_1_24_21_537,"DC: yaddr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(DCAMERA_APP_10663_112_2_18_1_24_21_538,"DC: Exit ISP CAPTURE FRAME callback at %d")
TRACE_MSG(DCAMERA_APP_10717_112_2_18_1_24_21_539,"DC: _DCAMERA_GetFrameFromIspExt: not support sensor format!")
TRACE_MSG(DCAMERA_APP_10731_112_2_18_1_24_22_540,"DC: _DCAMERA_GetFrameFromIspExt: not support dcam output format!")
TRACE_MSG(DCAMERA_APP_10794_112_2_18_1_24_22_541,"DC: DCAMERA_ExtDisplay, config memory error!")
TRACE_MSG(DCAMERA_APP_10830_112_2_18_1_24_22_542,"DC: DCAMERA_ExtDisplay, not enough memory error!")
TRACE_MSG(DCAMERA_APP_10865_112_2_18_1_24_22_543,"DC: DCAMERA_ExtOpenSensor start!")
TRACE_MSG(DCAMERA_APP_10905_112_2_18_1_24_22_544,"DC: DCAMERA_ExtOpenSensor accomplish!")
TRACE_MSG(DCAMERA_APP_10919_112_2_18_1_24_22_545,"DC: DCAMERA_ExtCloseSensor start!")
TRACE_MSG(DCAMERA_APP_10926_112_2_18_1_24_22_546,"DC: DCAMERA_ExtCloseSensor accomplish!")
TRACE_MSG(DCAMERA_APP_10946_112_2_18_1_24_22_547,"DC: DCAMERA_ExtGetData start!")
TRACE_MSG(DCAMERA_APP_10951_112_2_18_1_24_22_548,"DC: DCAMERA_ExtGetData: error input parameter!")
TRACE_MSG(DCAMERA_APP_10961_112_2_18_1_24_22_549,"DC: DCAMERA_ExtGetData isp_service open  error quit!")
TRACE_MSG(DCAMERA_APP_10968_112_2_18_1_24_22_550,"DC: DCAMERA_ExtGetData failed from sensor!")
TRACE_MSG(DCAMERA_APP_10976_112_2_18_1_24_22_551,"DC: DCAMERA_ExtGetData isp_service close  error quit!")
TRACE_MSG(DCAMERA_APP_10996_112_2_18_1_24_22_552,"DC: DCAMERA_ExtGetData accomplished!")
TRACE_MSG(DCAMERA_APP_11029_112_2_18_1_24_22_553,"DC: _DCAMERA_BackupReviewbuf idth %d blk_info.end_y %d blk_info.start_y %d")
TRACE_MSG(DCAMERA_APP_11036_112_2_18_1_24_22_554,"DC: _DCAMERA_BackupReviewbuf dst 0x%x, length %d, src y 0x%x")
TRACE_MSG(DCAMERA_APP_11084_112_2_18_1_24_22_555,"DC: DCAMERA_ExtDisplay start!")
TRACE_MSG(DCAMERA_APP_11107_112_2_18_1_24_22_556,"DC: DCAMERA_ExtDisplay: jpeg input, need to decoide at first!")
TRACE_MSG(DCAMERA_APP_11147_112_2_18_1_24_22_557,"DC: DCAMERA_ExtDisplay decode jpeg error!")
TRACE_MSG(DCAMERA_APP_11171_112_2_18_1_24_22_558,"DC: DCAMERA_ExtDisplay isp_service open error!")
TRACE_MSG(DCAMERA_APP_11193_112_2_18_1_24_22_559,"DC: DCAMERA_ExtDisplay isp_service close error!")
TRACE_MSG(DCAMERA_APP_11203_112_2_18_1_24_22_560,"DC: DCAMERA_ExtDisplay accomplished!")
TRACE_MSG(DCAMERA_APP_11218_112_2_18_1_24_22_561,"DC: DC_VideoSetDisplayParam started!")
TRACE_MSG(DCAMERA_APP_11255_112_2_18_1_24_23_562,"DC: DC_VideoSetDisplayParam accomplished!")
TRACE_MSG(DCAMERA_APP_11271_112_2_18_1_24_23_563,"DC: DC_DisplayAFrame started!")
TRACE_MSG(DCAMERA_APP_11320_112_2_18_1_24_23_564,"DC: DC_DisplayAFrame accomplished!")
TRACE_MSG(DCAMERA_APP_SIMULATOR_1934_112_2_18_1_24_27_565,"[simulator_jpg_data] SaveJpeg successed! ")
TRACE_MSG(DCAMERA_MEM_118_112_2_18_1_24_29_566,"DCamera_MemConfig, NO memory reserved for DC")
TRACE_MSG(DCAMERA_MEM_127_112_2_18_1_24_29_567,"DCamera_MemConfig, Memory not in one 64M Segement,try again")
TRACE_MSG(DCAMERA_MEM_136_112_2_18_1_24_29_568,"DCamera_MemConfig, Number %d block mem freed")
TRACE_MSG(DCAMERA_MEM_142_112_2_18_1_24_29_569,"DCamera_MemConfig, Memory successfully,total try times %d ,addr 0x%x")
TRACE_MSG(DCAMERA_MEM_146_112_2_18_1_24_29_570,"DCamera_MemConfig, No memory reserved for DC")
TRACE_MSG(DCAMERA_MEM_172_112_2_18_1_24_29_571,"DCamera_MemConfig, Memory alloc successfully base 0x%x,start 0x%x,size 0x%x")
TRACE_MSG(DCAMERA_MEM_188_112_2_18_1_24_29_572,"DCamera_MemFree OK")
TRACE_MSG(DCAMERA_MEM_269_112_2_18_1_24_29_573,"DCAMERA_PreviewGetMem, start 0x%x, size 0x%x")
TRACE_MSG(DCAMERA_MEM_461_112_2_18_1_24_30_574,"DCAMERA_CaptureMemReq, not enough memory for rotation capture")
TRACE_MSG(SENSOR_DRV_229_112_2_18_1_24_32_575,"SENSOR: I2C handle abnormal")
TRACE_MSG(SENSOR_DRV_235_112_2_18_1_24_32_576,"SENSOR: I2C_no_close")
TRACE_MSG(SENSOR_DRV_240_112_2_18_1_24_32_577,"SENSOR: I2C_close s_i2c_dev_handler=%d ")
TRACE_MSG(SENSOR_DRV_314_112_2_18_1_24_32_578,"_Sensor_IicHandlerInit, id %d freq %d bus %d slave_addr 0x%x reg_addr_num %d,ack %d no_stop %d")
TRACE_MSG(SENSOR_DRV_320_112_2_18_1_24_32_579,"SENSOR_handler creat err first")
TRACE_MSG(SENSOR_DRV_349_112_2_18_1_24_32_580,"SENSOR: Sensor_Reset -> reset_pulse_level = %d")
TRACE_MSG(SENSOR_DRV_380_112_2_18_1_24_32_581,"SENSOR_DRV: set mclk parameter error, the MCLK is %d")
TRACE_MSG(SENSOR_DRV_419_112_2_18_1_24_32_582,"SENSOR: Sensor_SetMCLK -> s_sensor_mclk = %dMHz, clk = %dMHz")
TRACE_MSG(SENSOR_DRV_434_112_2_18_1_24_32_583,"SENSOR: Sensor_SetMCLK divd = %d")
TRACE_MSG(SENSOR_DRV_440_112_2_18_1_24_32_584,"SENSOR: Sensor_SetMCLK -> Disable MCLK !!!")
TRACE_MSG(SENSOR_DRV_444_112_2_18_1_24_32_585,"SENSOR: Sensor_SetMCLK -> Do nothing !! ")
TRACE_MSG(SENSOR_DRV_461_112_2_18_1_24_32_586,"SENSOR: Sensor_SetMCLK src = 0x%x,  divd = 0x%x")
TRACE_MSG(SENSOR_DRV_481_112_2_18_1_24_32_587,"SENSOR: Sensor_SetMCLK -> Disable MCLK !!!")
TRACE_MSG(SENSOR_DRV_485_112_2_18_1_24_32_588,"SENSOR: Sensor_SetMCLK -> Do nothing !! ")
TRACE_MSG(SENSOR_DRV_623_112_2_18_1_24_33_589,"SENSOR: Sensor_PowerOn -> power_on = %d, power_down_level = %d, avdd_val = %d")
TRACE_MSG(SENSOR_DRV_668_112_2_18_1_24_33_590,"SENSOR: Sensor_PowerDown -> main, power_down %d, time %d")
TRACE_MSG(SENSOR_DRV_687_112_2_18_1_24_33_591,"SENSOR: Sensor_PowerDown -> sub, power_down %d, time %d")
TRACE_MSG(SENSOR_DRV_705_112_2_18_1_24_33_592,"SENSOR: Sensor_PowerDown -> atv %d")
TRACE_MSG(SENSOR_DRV_710_112_2_18_1_24_33_593,"SENSOR: Sensor_PowerDown -> atv not found")
TRACE_MSG(SENSOR_DRV_716_112_2_18_1_24_33_594,"SENSOR: Sensor_PowerDown -> atv not found")
TRACE_MSG(SENSOR_DRV_791_112_2_18_1_24_33_595,"SENSOR: Sensor_CheckSensorInfo -> sensor name = %s")
TRACE_MSG(SENSOR_DRV_807_112_2_18_1_24_33_596,"SENSOR: Sensor_SetExportInfo")
TRACE_MSG(SENSOR_DRV_895_112_2_18_1_24_34_597,"SENSOR: SENSOR mode Info > mode = %d, width = %d, height = %d, format = %d")
TRACE_MSG(SENSOR_DRV_927_112_2_18_1_24_34_598,"SENSOR: IIC write : reg:0x%04x, val:0x%04x error")
TRACE_MSG(SENSOR_DRV_959_112_2_18_1_24_34_599,"SENSOR: s_i2c_dev_handler 0x%x")
TRACE_MSG(SENSOR_DRV_969_112_2_18_1_24_34_600,"SENSOR: IIC write reg Error! 0x%04x, val:0x%04x")
TRACE_MSG(SENSOR_DRV_974_112_2_18_1_24_34_601,"SENSOR: IIC write reg OK! 0x%04x, val:0x%04x ")
TRACE_MSG(SENSOR_DRV_990_112_2_18_1_24_34_602,"SENSOR: IIC write Delay %d ms")
TRACE_MSG(SENSOR_DRV_1043_112_2_18_1_24_34_603,"SENSOR: s_i2c_dev_handler 0x%x")
TRACE_MSG(SENSOR_DRV_1054_112_2_18_1_24_34_604,"SENSOR: IIC read reg, ERROR! %d")
TRACE_MSG(SENSOR_DRV_1061_112_2_18_1_24_34_605,"SENSOR: IIC read reg OK:0x%04x, val:0x%04x")
TRACE_MSG(SENSOR_DRV_1080_112_2_18_1_24_34_606,"SENSOR: Sensor_SendRegValueToSensor -> reg_count = %d start time = %d")
TRACE_MSG(SENSOR_DRV_1085_112_2_18_1_24_34_607,"SENSOR: Sensor_SendRegTabToSensor tab is null error")
TRACE_MSG(SENSOR_DRV_1097_112_2_18_1_24_34_608,"SENSOR: Sensor_SendRegValueToSensor -> end time = %d")
TRACE_MSG(SENSOR_DRV_1192_112_2_18_1_24_34_609,"Sensor_SetCurId : %d, start time: %d")
TRACE_MSG(SENSOR_DRV_1196_112_2_18_1_24_34_610,"Sensor_SetCurId : sensor_id %d unsupported")
TRACE_MSG(SENSOR_DRV_1202_112_2_18_1_24_34_611,"Sensor_SetCurId: No such sensor! %d ")
TRACE_MSG(SENSOR_DRV_1228_112_2_18_1_24_34_612,"SENSOR:Sensor_SetCurId id:%d, num:%d, ptr: 0x%x, ptr: 0x%x")
TRACE_MSG(SENSOR_DRV_1230_112_2_18_1_24_34_613,"Sensor_SetCurId : %d, end time: %d")
TRACE_MSG(SENSOR_DRV_1257_112_2_18_1_24_34_614,"SENSOR: sensor identifing %d")
TRACE_MSG(SENSOR_DRV_1262_112_2_18_1_24_34_615,"SENSOR: sensor identified")
TRACE_MSG(SENSOR_DRV_1277_112_2_18_1_24_34_616,"SENSOR: %d info of Sensor_Init table %d is null")
TRACE_MSG(SENSOR_DRV_1310_112_2_18_1_24_35_617,"SENSOR TYPE of %d indentify OK")
TRACE_MSG(SENSOR_DRV_1314_112_2_18_1_24_35_618,"SENSOR TYPE of %d indentify FAILURE")
TRACE_MSG(SENSOR_DRV_1326_112_2_18_1_24_35_619,"SENSOR: Sensor_Init start %d")
TRACE_MSG(SENSOR_DRV_1330_112_2_18_1_24_35_620,"SENSOR: Sensor_Init is done\n")
TRACE_MSG(SENSOR_DRV_1383_112_2_18_1_24_35_621,"SENSOR: Sensor_Init Main Success \n")
TRACE_MSG(SENSOR_DRV_1393_112_2_18_1_24_35_622,"SENSOR: Sensor_Init Sub Success \n")
TRACE_MSG(SENSOR_DRV_1400_112_2_18_1_24_35_623,"SENSOR: Sensor_Init Fail No Sensor err \n")
TRACE_MSG(SENSOR_DRV_1403_112_2_18_1_24_35_624,"SENSOR: Sensor_Init: end: %d")
TRACE_MSG(SENSOR_DRV_1437_112_2_18_1_24_35_625,"SENSOR: Sensor_powerUpHandle start %d")
TRACE_MSG(SENSOR_DRV_1442_112_2_18_1_24_35_626,"SENSOR: Sensor_powerUpHandle error for sensor info of %d is null")
TRACE_MSG(SENSOR_DRV_1473_112_2_18_1_24_35_627,"SENSOR: Sensor_sleep of id %d, time %d")
TRACE_MSG(SENSOR_DRV_1493_112_2_18_1_24_35_628,"SENSOR: Sensor_Open the sensor identify fail %d error")
TRACE_MSG(SENSOR_DRV_1511_112_2_18_1_24_35_629,"SENSOR: Sensor_Open the atv init fail %d error")
TRACE_MSG(SENSOR_DRV_1535_112_2_18_1_24_35_630,"SENSOR: Sensor_Open sensor type: %d id: %d, time %d")
TRACE_MSG(SENSOR_DRV_1539_112_2_18_1_24_35_631,"SENSOR: Sensor_Open -> sensor has not init")
TRACE_MSG(SENSOR_DRV_1547_112_2_18_1_24_35_632,"SENSOR: Sensor_Open: start: %d")
TRACE_MSG(SENSOR_DRV_1550_112_2_18_1_24_35_633,"SENSOR: Sensor_Open -> sensor has open")
TRACE_MSG(SENSOR_DRV_1561_112_2_18_1_24_35_634,"SENSOR: Sensor_Open -> sensor main manual set")
TRACE_MSG(SENSOR_DRV_1580_112_2_18_1_24_35_635,"SENSOR: ext func init %d error")
TRACE_MSG(SENSOR_DRV_1619_112_2_18_1_24_35_636,"SENSOR: Sensor_Open the atv init fail %d error")
TRACE_MSG(SENSOR_DRV_1629_112_2_18_1_24_35_637,"SENSOR: Sensor_Open end %d")
TRACE_MSG(SENSOR_DRV_1643_112_2_18_1_24_35_638,"SENSOR: Sensor_SetMode -> mode = %d, time %d")
TRACE_MSG(SENSOR_DRV_1647_112_2_18_1_24_35_639,"SENSOR: The sensor mode as before")
TRACE_MSG(SENSOR_DRV_1653_112_2_18_1_24_35_640,"SENSOR: Sensor_SetResolution -> sensor has not init")
TRACE_MSG(SENSOR_DRV_1659_112_2_18_1_24_35_641,"SENSOR: current sensor info is not available!")
TRACE_MSG(SENSOR_DRV_1680_112_2_18_1_24_35_642,"SENSOR: Sensor_SetResolution -> No this resolution information !!!")
TRACE_MSG(SENSOR_DRV_1699_112_2_18_1_24_35_643,"SENSOR: Sensor_Ioctl -> cmd = %d, arg = 0x%x")
TRACE_MSG(SENSOR_DRV_1703_112_2_18_1_24_35_644,"SENSOR: Sensor_Ioctl -> cmd %d error")
TRACE_MSG(SENSOR_DRV_1709_112_2_18_1_24_35_645,"SENSOR: Sensor_Ioctl -> sensor has not init error")
TRACE_MSG(SENSOR_DRV_1715_112_2_18_1_24_35_646,"SENSOR: Sensor_Ioctl -> sensor has not open error")
TRACE_MSG(SENSOR_DRV_1721_112_2_18_1_24_35_647,"SENSOR: Sensor_Ioctl -> sensor has not initial I2C handle")
TRACE_MSG(SENSOR_DRV_1727_112_2_18_1_24_35_648,"SENSOR: Sensor_Ioctl - > can't access internal command error!")
TRACE_MSG(SENSOR_DRV_1745_112_2_18_1_24_35_649,"SENSOR: Sensor_Ioctl -> the ioctl function has not register err!")
TRACE_MSG(SENSOR_DRV_1759_112_2_18_1_24_35_650,"SENSOR: Sensor_GetInfo -> sensor has not init")
TRACE_MSG(SENSOR_DRV_1771_112_2_18_1_24_35_651,"SENSOR: Sensor_close")
TRACE_MSG(SENSOR_DRV_1901_112_2_18_1_24_36_652,"SENSOR: Sensor_SetSensorExifInfo the get_exif fun is null %d error")
END_TRACE_MAP(MS_REF_DC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_DC_TRC_H_

