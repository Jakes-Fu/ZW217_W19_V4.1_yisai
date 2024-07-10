/******************************************************************************
 ** File Name:      mmi_app_camera_trc.h                                         *
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
//trace_id:158
#ifndef _MMI_APP_CAMERA_TRC_H_
#define _MMI_APP_CAMERA_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMICAMERA_MANAGER_40_112_2_18_2_8_25_0 "[MMICamera]: DCAMERA_Open error"
#define MMICAMERA_MANAGER_57_112_2_18_2_8_25_1 "[MMICamera]: OpenDC ret = %d, s_dc_is_open = %d"
#define MMICAMERA_MANAGER_73_112_2_18_2_8_25_2 "[MMICamera]: StartDCPreview disp w = %d, disp h = %d"
#define MMICAMERA_MANAGER_74_112_2_18_2_8_25_3 "[MMICamera]: StartDCPreview target w = %d, target h = %d"
#define MMICAMERA_MANAGER_79_112_2_18_2_8_25_4 "[MMICamera]: DCAMERA_StartPreview ret = %d"
#define MMICAMERA_MANAGER_84_112_2_18_2_8_25_5 "[MMICamera]: StartDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d"
#define MMICAMERA_MANAGER_103_112_2_18_2_8_25_6 "[MMICamera]: MMICamera_InitDCPreviewParam enter"
#define MMICAMERA_MANAGER_107_112_2_18_2_8_25_7 "[MMICamera]:  cur_angle = %d"
#define MMICAMERA_MANAGER_114_112_2_18_2_8_25_8 "[MMICamera]:  GUILCD_GetLogicWidthHeight, result = %d"
#define MMICAMERA_MANAGER_125_112_2_18_2_8_25_9 "[MMICamera]:  GUILCD_ConvertLogicToPhysicalCoordinate, result = %d"
#define MMICAMERA_MANAGER_141_112_2_18_2_8_25_10 "[MMICamera]:  s_preview_param.disp_rect :x = %d, y = %d, w = %d, h = %d"
#define MMICAMERA_MANAGER_163_112_2_18_2_8_25_11 "[MMICamera]: StopDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d"
#define MMICAMERA_MANAGER_183_112_2_18_2_8_25_12 "[MMICamera]: CloseDC ret = %d, s_dc_is_open = %d"
#define MMICAMERA_MANAGER_207_112_2_18_2_8_25_13 "[MMICamera]: SnapShot return: %d"
#define MMICAMERA_MANAGER_265_112_2_18_2_8_25_14 "[MMICamera]: SetPhotoSize return: %d"
#define MMIDC_EFFECT360_95_112_2_18_2_8_26_15 "[MMIDC] GetCurEffect360CommonType type=%d"
#define MMIDC_EFFECT360_129_112_2_18_2_8_26_16 "[MMIDC] IsEffect360CommonType resut=%d"
#define MMIDC_EFFECT360_177_112_2_18_2_8_26_17 "[MMIDC] GetEffect360CommonHandle handle=%d"
#define MMIDC_EFFECT360_233_112_2_18_2_8_26_18 "[MMIDC] CreateEffect360CommonHandle ret=%d,handle=%d"
#define MMIDC_EFFECT360_285_112_2_18_2_8_26_19 "[MMIDC] DestroyEffect360CommonHandle ret=%d"
#define MMIDC_EFFECT360_332_112_2_18_2_8_26_20 "[MMIDC] CreateCommonPhotoInfo buf_ptr=0x%X, buf_size=%d"
#define MMIDC_EFFECT360_335_112_2_18_2_8_26_21 "[MMIDC] CreateCommonPhotoInfo ret=%d"
#define MMIDC_EFFECT360_363_112_2_18_2_8_26_22 "[MMIDC] DestroyCommonPhotoInfo"
#define MMIDC_EFFECT360_442_112_2_18_2_8_26_23 "[MMIDC] MMIDC_OpenEffect360 ret=%d"
#define MMIDC_EFFECT360_485_112_2_18_2_8_27_24 "[MMIDC] MMIDC_IsEffect360Enable is_enable=%d"
#define MMIDC_EFFECT360_509_112_2_18_2_8_27_25 "[MMIDC] Effect360CommonProcessCallBack"
#define MMIDC_EFFECT360_538_112_2_18_2_8_27_26 "[MMIDC] Effect360CommonProcessCallBack"
#define MMIDC_EFFECT360_680_112_2_18_2_8_27_27 "[MMIDC] GetEffect360CommonImageProcessParam capture"
#define MMIDC_EFFECT360_684_112_2_18_2_8_27_28 "[MMIDC] GetEffect360CommonImageProcessParam"
#define MMIDC_EFFECT360_712_112_2_18_2_8_27_29 "[MMIDC] MMIDC_GetEffect360ImageProcessParam"
#define MMIDC_EXPORT_243_112_2_18_2_8_28_30 "[MMIDC] MMIAPIDC_GetFileIDString id >= id_max"
#define MMIDC_EXPORT_270_112_2_18_2_8_28_31 "[MMIDC] MMIAPIDC_CombinePathName path = PNULL"
#define MMIDC_FLOW_405_112_2_18_2_8_29_32 "[MMIDC] SetReviewOSDCallback"
#define MMIDC_FLOW_513_112_2_18_2_8_29_33 "[MMIDC] GetDCPreviewParam preview_param_ptr is PNULL !!!"
#define MMIDC_FLOW_525_112_2_18_2_8_29_34 "[MMIDC] GetDCPreviewParam lcd_width = %d, lcd_height = %d"
#define MMIDC_FLOW_531_112_2_18_2_8_29_35 "[MMIDC] GetDCPreviewParam rotation = %d, display mode = %d"
#define MMIDC_FLOW_556_112_2_18_2_8_29_36 "[MMIDC] Display_Callback ret=%d"
#define MMIDC_FLOW_560_112_2_18_2_8_29_37 "[MMIDC] Display_Callback p_disp_src=0x%X,p_disp_dst=0x%X"
#define MMIDC_FLOW_572_112_2_18_2_8_29_38 "[MMIDC] Display_Callback is brushing return"
#define MMIDC_FLOW_582_112_2_18_2_8_29_39 "[MMIDC] Display_Callback is doing return"
#define MMIDC_FLOW_587_112_2_18_2_8_29_40 "[MMIDC] Display_Callback start"
#define MMIDC_FLOW_596_112_2_18_2_8_29_41 "[MMIDC Display_Callback src type=%d,pattern=%d"
#define MMIDC_FLOW_599_112_2_18_2_8_29_42 "[MMIDC Display_Callback dst yaddr=0x%X,uaddr=0x%X"
#define MMIDC_FLOW_621_112_2_18_2_8_29_43 "[MMIDC] GetDCPreviewParamEx preview_param_ptr is PNULL !!!"
#define MMIDC_FLOW_633_112_2_18_2_8_29_44 "[MMIDC] GetDCPreviewParamEx lcd_width = %d, lcd_height = %d"
#define MMIDC_FLOW_639_112_2_18_2_8_29_45 "[MMIDC] GetDCPreviewParamEx rotation = %d, display mode = %d"
#define MMIDC_FLOW_663_112_2_18_2_8_29_46 "[MMIDC]: SetDCAllPreviewParam s_dc_is_open = %d"
#define MMIDC_FLOW_710_112_2_18_2_8_29_47 "[MMIDC]: DCAMERA_StartPreview ret = %d"
#define MMIDC_FLOW_716_112_2_18_2_8_29_48 "[MMIDC]: MMIDC_GetPhotoSize() = %d"
#define MMIDC_FLOW_717_112_2_18_2_8_29_49 "[MMIDC]: MMIDC_GePhotoSizetVerLcd() = %d"
#define MMIDC_FLOW_726_112_2_18_2_8_29_50 "[MMIDC]: StartDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d"
#define MMIDC_FLOW_745_112_2_18_2_8_29_51 "[MMIDC]: StopDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d"
#define MMIDC_FLOW_772_112_2_18_2_8_29_52 "[MMIDC]: CloseDC ret = %d, s_dc_is_open = %d"
#define MMIDC_FLOW_787_112_2_18_2_8_30_53 "[MMIDC]: MMIDC_IsDCOpen s_dc_is_open = %d"
#define MMIDC_FLOW_797_112_2_18_2_8_30_54 "[MMIDC]: MMIDC_IsDVOpen s_dv_is_open = %d"
#define MMIDC_FLOW_835_112_2_18_2_8_30_55 "[MMIDC]: OpenDC ret = %d"
#define MMIDC_FLOW_914_112_2_18_2_8_30_56 "MMIDC_GetMultiShootEnum = %d"
#define MMIDC_FLOW_927_112_2_18_2_8_30_57 "[MMIDC] SendCaptureFailMsg ret==%d"
#define MMIDC_FLOW_930_112_2_18_2_8_30_58 "[MMIDC] SendCaptureFailMsg param_ptr=0x%X, param_size=%d"
#define MMIDC_FLOW_952_112_2_18_2_8_30_59 "[MMIDC] SendCaptureSuccessMsg ret==%d"
#define MMIDC_FLOW_955_112_2_18_2_8_30_60 "[MMIDC] SendCaptureSuccessMsg param_ptr=0x%X, param_size=%d"
#define MMIDC_FLOW_980_112_2_18_2_8_30_61 "[MMIDC] SaveDataToFile "
#define MMIDC_FLOW_997_112_2_18_2_8_30_62 "[MMIDC] SaveDataToFile ret=%d"
#define MMIDC_FLOW_1023_112_2_18_2_8_30_63 "[MMIDC] PlayCaptureRing is_play_ring = %d"
#define MMIDC_FLOW_1041_112_2_18_2_8_30_64 "[MMIDC] PlayCaptureRing"
#define MMIDC_FLOW_1053_112_2_18_2_8_30_65 "[MMIDC]: CaptureCallBack, ret = %d, data_buffer = %d, data_len = %d"
#define MMIDC_FLOW_1058_112_2_18_2_8_30_66 "[MMIDC]: CaptureCallBack !s_is_capture"
#define MMIDC_FLOW_1066_112_2_18_2_8_30_67 "[MMIDC]: CaptureCallBack s_is_cancel_multishot=%d, is_cancel_by_user=%d"
#define MMIDC_FLOW_1103_112_2_18_2_8_30_68 "[MMIDC]: DCAdap_CaptureCallBack, index = %d, size = %d"
#define MMIDC_FLOW_1110_112_2_18_2_8_30_69 "[MMIDC]: photo_id = %d, MMIDC_GetMulitShotNumber = %d"
#define MMIDC_FLOW_1130_112_2_18_2_8_30_70 "[MMIDC]: CaptureCallBack  captured_photo_num = %d"
#define MMIDC_FLOW_1147_112_2_18_2_8_30_71 "[MMIDC] CaptureCallBack LAST !photos_validate"
#define MMIDC_FLOW_1167_112_2_18_2_8_30_72 "[MMIDC]: MMIDC_GetCapturedPhotoNumber captured_photo_num = %d"
#define MMIDC_FLOW_1194_112_2_18_2_8_30_73 "[MMIDC]: Capture_Ex_Callback, ret = %d"
#define MMIDC_FLOW_1200_112_2_18_2_8_30_74 "[MMIDC]: Capture_Ex_Callback !s_is_capture"
#define MMIDC_FLOW_1206_112_2_18_2_8_30_75 "[MMIDC]: Capture_Ex_Callback t1 = %d"
#define MMIDC_FLOW_1213_112_2_18_2_8_30_76 "[MMIDC]: Capture_Ex_Callback s_is_cancel_multishot=%d, is_cancel_by_user=%d"
#define MMIDC_FLOW_1267_112_2_18_2_8_30_77 "[MMIDC] Capture_Ex_Callback process_result=%d"
#define MMIDC_FLOW_1272_112_2_18_2_8_30_78 "[MMIDC] Capture_Ex_Callback data_size.w=%d, data_size.h=%d"
#define MMIDC_FLOW_1305_112_2_18_2_8_31_79 "[MMIDC] Capture_Ex_Callback process_result=%d"
#define MMIDC_FLOW_1323_112_2_18_2_8_31_80 "[MMIDC] Capture_Ex_Callback end t2=%d, t2-t1=%d"
#define MMIDC_FLOW_1379_112_2_18_2_8_31_81 "[MMIDC] GetSnapshotExParam target_size.w =%d, h=%d"
#define MMIDC_FLOW_1381_112_2_18_2_8_31_82 "[MMIDC] GetSnapshotExParam"
#define MMIDC_FLOW_1444_112_2_18_2_8_31_83 "[MMIDC] SnapShot Quality = PNULL"
#define MMIDC_FLOW_1464_112_2_18_2_8_31_84 "[MMIDC]: MMIDC_GetPhotoSize() = %d"
#define MMIDC_FLOW_1465_112_2_18_2_8_31_85 "[MMIDC]: MMIDC_GePhotoSizetVerLcd() = %d"
#define MMIDC_FLOW_1475_112_2_18_2_8_31_86 "[MMIDC]: SnapShot with frame"
#define MMIDC_FLOW_1491_112_2_18_2_8_31_87 "[MMIDC]: SnapShot with frame buffer NULL!!!"
#define MMIDC_FLOW_1496_112_2_18_2_8_31_88 "[MMIDC]: SnapShot rotation_mode=%d"
#define MMIDC_FLOW_1512_112_2_18_2_8_31_89 "[MMIDC]: SnapShot return: %d"
#define MMIDC_FLOW_1523_112_2_18_2_8_31_90 "[MMIDC]: ReviewPhotoCallBack +, ret_value = %d"
#define MMIDC_FLOW_1562_112_2_18_2_8_31_91 "[MMIDC]: ReviewSinglePhotoFromBuf DC_REVIEW_JPEG"
#define MMIDC_FLOW_1635_112_2_18_2_8_31_92 "[MMIDC]: is_real_hor_lcd= %d"
#define MMIDC_FLOW_1637_112_2_18_2_8_31_93 "[MMIDC]: disp rect w= %d, h = %d"
#define MMIDC_FLOW_1638_112_2_18_2_8_31_94 "[MMIDC]: target rect w = %d, h = %d"
#define MMIDC_FLOW_1639_112_2_18_2_8_31_95 "[MMIDC]: rotation_mode = %d, disp_mode = %d"
#define MMIDC_FLOW_1641_112_2_18_2_8_31_96 "[MMIDC]: jpeg_buffer = %d, jpeg_len = %d"
#define MMIDC_FLOW_1645_112_2_18_2_8_31_97 "[MMIDC]: DCAMERA_ReviewJpeg return %d"
#define MMIDC_FLOW_1659_112_2_18_2_8_31_98 "[MMIDC]: MMIDC_ReviewPhotos %d, flow_mode = %d"
#define MMIDC_FLOW_1708_112_2_18_2_8_31_99 "[MMIDC]: RecordEndNotifyCallBack record end! i_type = %d"
#define MMIDC_FLOW_1747_112_2_18_2_8_31_100 "[MMIDC]: RecordEndNotifyCallBack end_type= %d"
#define MMIDC_FLOW_1783_112_2_18_2_8_31_101 "[MMIDC] ConvertImageData dst_ptr=0x%X,dst_size=%d,dst_width=%d,dst_height=%d"
#define MMIDC_FLOW_1790_112_2_18_2_8_32_102 "[MMIDC] ConvertImageData src_ptr=0x%X,src_size=%d,src_width=%d,src_height=%d"
#define MMIDC_FLOW_1796_112_2_18_2_8_32_103 "[MMIDC] ConvertImageData type=%d,angle=%d,is_around_color=%d"
#define MMIDC_FLOW_1818_112_2_18_2_8_32_104 "[MMIDC] ConvertImageData PARAM error!!!"
#define MMIDC_FLOW_1842_112_2_18_2_8_32_105 "[MMIDC] ConvertImageData size overflow!!!"
#define MMIDC_FLOW_1879_112_2_18_2_8_32_106 "[MMIDC] ConvertImageData size overflow!!!"
#define MMIDC_FLOW_1911_112_2_18_2_8_32_107 "[MMIDC] ConvertImageData size overflow!!!"
#define MMIDC_FLOW_1943_112_2_18_2_8_32_108 "[MMIDC] ConvertImageData size overflow!!!"
#define MMIDC_FLOW_1963_112_2_18_2_8_32_109 "[MMIDC] ConvertImageData offset_bytes=%d,offset_size=%d"
#define MMIDC_FLOW_1965_112_2_18_2_8_32_110 "[MMIDC] ConvertImageData ret=%d"
#define MMIDC_FLOW_2045_112_2_18_2_8_32_111 "[MMIDC] GetLastFrameCallback data_type = %d !!!"
#define MMIDC_FLOW_2061_112_2_18_2_8_32_112 "[MMIDC] lcd_buffer_size = %d"
#define MMIDC_FLOW_2066_112_2_18_2_8_32_113 "[MMIDC] main_buffer_ptr  is PNULL = 0x%lX"
#define MMIDC_FLOW_2079_112_2_18_2_8_32_114 "[MMIDC] s_lastframe_ptr = 0x%lX"
#define MMIDC_FLOW_2083_112_2_18_2_8_32_115 "[MMIDC] s_lastframe_ptr is PNULL"
#define MMIDC_FLOW_2094_112_2_18_2_8_32_116 "[MMIDC] dst_angle_ptr = 0x%lX"
#define MMIDC_FLOW_2096_112_2_18_2_8_32_117 "[MMIDC] src_angle_ptr = 0x%lX, data_len = %d"
#define MMIDC_FLOW_2138_112_2_18_2_8_32_118 "[MMIDC] width = %d, height = %d"
#define MMIDC_FLOW_2140_112_2_18_2_8_32_119 "[MMIDC] logic_width = %d, logic_height = %d, logic_angle=%d"
#define MMIDC_FLOW_2142_112_2_18_2_8_32_120 "[MMIDC] absolute_angle=%d"
#define MMIDC_FLOW_2145_112_2_18_2_8_32_121 "[MMIDC] is_keep_dst_logic_screen=%d"
#define MMIDC_FLOW_2149_112_2_18_2_8_32_122 "[MMIDC] MAINLCD_LOGIC_ANGLE_0"
#define MMIDC_FLOW_2249_112_2_18_2_8_32_123 "[MMIDC] MAINLCD_LOGIC_ANGLE_90"
#define MMIDC_FLOW_2310_112_2_18_2_8_33_124 "[MMIDC]: StartRecord"
#define MMIDC_FLOW_2355_112_2_18_2_8_33_125 "[MMIDC] StartRecord NO TYPE"
#define MMIDC_FLOW_2394_112_2_18_2_8_33_126 "[MMIDC] StartRecord recorder_start_t.audio_info_ptr = PNULL"
#define MMIDC_FLOW_2399_112_2_18_2_8_33_127 "[MMIDC]StartRecord recorder_start_t.video_info_ptr = PNULL"
#define MMIDC_FLOW_2404_112_2_18_2_8_33_128 "[MMIDC] StartRecord recorder_start_t.file_name_ptr = PNULL"
#define MMIDC_FLOW_2410_112_2_18_2_8_33_129 "[MMIDC]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d"
#define MMIDC_FLOW_2414_112_2_18_2_8_33_130 "[MMIDC] DRECORDER_RegisterGetLastFrameCallback ret = %d"
#define MMIDC_FLOW_2417_112_2_18_2_8_33_131 "[MMIDC]: DRECORDER_StartRecord = %d"
#define MMIDC_FLOW_2443_112_2_18_2_8_33_132 "[MMIDC]: StartRecord - return success !"
#define MMIDC_FLOW_2460_112_2_18_2_8_33_133 "[MMIDC]: SetDVAllPreviewParam = %d"
#define MMIDC_FLOW_2485_112_2_18_2_8_33_134 "[MMIDC]: SetDVAllPreviewParam ret= %d"
#define MMIDC_FLOW_2504_112_2_18_2_8_33_135 "[MMIDC]: StartDVPreview = %d, = %d"
#define MMIDC_FLOW_2529_112_2_18_2_8_33_136 "[MMIDC]: StartDVPreview lcd_width= %d, lcd_height= %d, angle = %d"
#define MMIDC_FLOW_2536_112_2_18_2_8_33_137 "[MMIDC]: DRECORDER_StartPreview = %d"
#define MMIDC_FLOW_2565_112_2_18_2_8_33_138 "[MMIDC]: StopPreview = %d, = %d"
#define MMIDC_FLOW_2572_112_2_18_2_8_33_139 "[MMIDC]: DRECORDER_StopPreview = %d"
#define MMIDC_FLOW_2597_112_2_18_2_8_33_140 "[MMIDC]: OpenDV = %d"
#define MMIDC_FLOW_2642_112_2_18_2_8_33_141 "[MMIDC] MMIDC_CopyLastFrame s_lastframe_ptr = 0x%lX, buffer_size = %d"
#define MMIDC_FLOW_2644_112_2_18_2_8_33_142 "[MMIDC] main_buffer_ptr = 0x%lX"
#define MMIDC_FLOW_2668_112_2_18_2_8_33_143 "[MMIDC]: CloseDV = %d"
#define MMIDC_FLOW_2675_112_2_18_2_8_33_144 "[MMIDC]: DRECORDER_Close = %d"
#define MMIDC_FLOW_2708_112_2_18_2_8_33_145 "[MMIDC]: BurstShot %d"
#define MMIDC_FLOW_2714_112_2_18_2_8_33_146 "[MMIDC] BurstShot shot_num OVER 9"
#define MMIDC_FLOW_2757_112_2_18_2_8_33_147 "[MMIDC] BurstShot Quality"
#define MMIDC_FLOW_2765_112_2_18_2_8_33_148 "[MMIDC]: BurstShot rotaiton_mode: %d"
#define MMIDC_FLOW_2767_112_2_18_2_8_33_149 "[MMIDC]: DCAMERA_DoBurstSnapshot rotation_mode=%d"
#define MMIDC_FLOW_2770_112_2_18_2_8_33_150 "[MMIDC]: DCAMERA_DoBurstSnapshot return: %d"
#define MMIDC_FLOW_2787_112_2_18_2_8_33_151 "[MMIDC]: DRECORDER_StopRecord return: %d"
#define MMIDC_FLOW_3179_112_2_18_2_8_34_152 "[MMIDC]: MMIDC_FlowExit"
#define MMIDC_FLOW_3223_112_2_18_2_8_34_153 "[MMIDC] MMIDC_FlowFunction op=%d,flow_mode=%d"
#define MMIDC_FLOW_3276_112_2_18_2_8_34_154 "[MMIDC]: MMIDC_FlowStart fail"
#define MMIDC_FLOW_3297_112_2_18_2_8_35_155 "[MMIDC]: MMIDC_FlowStart fail"
#define MMIDC_FLOW_3313_112_2_18_2_8_35_156 "[MMIDC]: MMIDC_FlowStart res=%d,is_first_preview=%d"
#define MMIDC_FLOW_3345_112_2_18_2_8_35_157 "[MMIDC]: MMIAPIDC_IsPreviewing dc preview = %d, dv preview = %d"
#define MMIDC_FLOW_3355_112_2_18_2_8_35_158 "[MMIDC]: MMIDC_IsReviewing %d"
#define MMIDC_FLOW_3365_112_2_18_2_8_35_159 "[MMIDC]: MMIDC_IsCapturing %d"
#define MMIDC_FLOW_3415_112_2_18_2_8_35_160 "[MMIDC]MMIDC_CheckSpace: UDISK free spaced high value is = %d free space low value is = %d"
#define MMIDC_FLOW_3425_112_2_18_2_8_35_161 "[MMIDC]MMIDC_CheckSpace sfs_sd_ret = %d, sfs_udisk_ret = %d,space_result = %d"
#define MMIDC_FLOW_3453_112_2_18_2_8_35_162 "[MMIDC]MMIDC_CheckSpace dc_dev = %d, sfs_result = %d"
#define MMIDC_FLOW_3472_112_2_18_2_8_35_163 "[MMIDC] MMIDC_ShowErrorMsg fs_err = %d,dev = %d"
#define MMIDC_FLOW_3579_112_2_18_2_8_35_164 "[MMIDC] MMIDC_SetBlock"
#define MMIDC_FLOW_3670_112_2_18_2_8_35_165 "[MMIDC] MMIDC_GetOSDFlagBeforePreview s_is_osd_flag_before_preview=%d"
#define MMIDC_FLOW_3695_112_2_18_2_8_35_166 "[MMIDC]: MMIDC_OpenDC is_dc_open = %d"
#define MMIDC_FLOW_3709_112_2_18_2_8_35_167 "[MMIDC]: MMIDC_OpenDC ret = %d"
#define MMIDC_FLOW_3723_112_2_18_2_8_35_168 "[MMIDC]: MMIDC_OpenDC is_dc_open 2= %d"
#define MMIDC_FLOW_3739_112_2_18_2_8_35_169 "[MMIDC]: MMIDC_OpenDV is_dv_open= %d"
#define MMIDC_FLOW_3744_112_2_18_2_8_35_170 "[MMIDC]: DRECORDER_SelectSrcDevice = %d"
#define MMIDC_FLOW_3746_112_2_18_2_8_35_171 "[MMIDC]: DRECORDER_Open = %d"
#define MMIDC_FLOW_3761_112_2_18_2_8_35_172 "[MMIDC]: MMIDC_OpenDV is_dv_open2= %d"
#define MMIDC_FLOW_3793_112_2_18_2_8_36_173 "[MMIDC]: MMIDC_StartPreviewForSwitchSize ret=%d"
#define MMIDC_FLOW_3809_112_2_18_2_8_36_174 "[MMIDC] MMIDC_StopPreviewForSwitchSize ret=%d"
#define MMIDC_FLOW_3834_112_2_18_2_8_36_175 "[MMIDC] MMIDC_CreateMainOSDLayer block_id=%d"
#define MMIDC_FLOW_3843_112_2_18_2_8_36_176 "[MMIDC] MMIDC_CreateMainOSDLayer width=%d, height=%d"
#define MMIDC_FLOW_3878_112_2_18_2_8_36_177 "[MMIDC] MMIDC_CreateMainOSDLayer result=%d,dev_info.block_id=%d"
#define MMIDC_FLOW_3939_112_2_18_2_8_36_178 "[MMIDC] MMIDC_ClearMainOSDLayer block_id=%d"
#define MMIDC_FLOW_3958_112_2_18_2_8_36_179 "[MMIDC] MMIDC_PDAReviewPhotosForEditPic"
#define MMIDC_FLOW_3993_112_2_18_2_8_36_180 "[MMIDC] MMIDC_GetA888LastFrame src_ptr=0x%lX,width=%d,height=%d"
#define MMIDC_FLOW_4009_112_2_18_2_8_36_181 "[MMIDC] MMIDC_GetA888LastFrame s_pda_a888_lastframe_ptr=0x%lX"
#define MMIDC_FLOW_4069_112_2_18_2_8_36_182 "[MMIDC] MMIDC_ReCreateMainOSDLayer ret=%d, result=%d"
#define MMIDC_FLOW_4116_112_2_18_2_8_36_183 "[MMIDC] MMIDC_AllocMemoryForReview addr=0x%X"
#define MMIDC_FLOW_4121_112_2_18_2_8_36_184 "[MMIDC] MMIDC_AllocMemoryForReview ret=%d"
#define MMIDC_FLOW_4158_112_2_18_2_8_36_185 "[MMIDC] MMIDC_AllocMemoryForReview addr_ptr=0x%X"
#define MMIDC_FLOW_4241_112_2_18_2_8_37_186 "[MMIDC] MMIDC_GetCurrentFileBuffer ret=%d, buf_ptr=0x%X, buffer_size=%d"
#define MMIDC_FLOW_4289_112_2_18_2_8_37_187 "test_imageprocess handle=%d"
#define MMIDC_FLOW_4317_112_2_18_2_8_37_188 "test_imageprocess width=%d, height=%d"
#define MMIDC_FLOW_4320_112_2_18_2_8_37_189 "test_imageprocess src chn0_ptr=0x%X, chn0_size=%d"
#define MMIDC_FLOW_4323_112_2_18_2_8_37_190 "test_imageprocess src chn1_ptr=0x%X, chn1_size=%d"
#define MMIDC_FLOW_4326_112_2_18_2_8_37_191 "test_imageprocess dst chn0_ptr=0x%X, chn1_ptr=0x%X"
#define MMIDC_FLOW_4421_112_2_18_2_8_37_192 "test_imageprocess IMGPROC_DoProcess ret=%d"
#define MMIDC_FLOW_4455_112_2_18_2_8_37_193 "[MMIDC] MMIDC_DisplayPreviewFrame t1=%d"
#define MMIDC_FLOW_4499_112_2_18_2_8_37_194 "[MMIDC] MMIDC_DisplayPreviewFrame ret=%d"
#define MMIDC_FLOW_4502_112_2_18_2_8_37_195 "[MMIDC] MMIDC_DisplayPreviewFrame is_skip_image_process=%d"
#define MMIDC_FLOW_4521_112_2_18_2_8_37_196 "[MMIDC] MMIDC_DisplayPreviewFrame src width=%d,height=%d"
#define MMIDC_FLOW_4525_112_2_18_2_8_37_197 "[MMIDC] MMIDC_DisplayPreviewFrame src type=%d,pattern=%d"
#define MMIDC_FLOW_4528_112_2_18_2_8_37_198 "[MMIDC] MMIDC_DisplayPreviewFrame src yaddr=0x%X,uaddr=0x%X"
#define MMIDC_FLOW_4532_112_2_18_2_8_37_199 "[MMIDC] MMIDC_DisplayPreviewFrame dst width=%d,height=%d"
#define MMIDC_FLOW_4536_112_2_18_2_8_37_200 "[MMIDC] MMIDC_DisplayPreviewFrame dst type=%d,pattern=%d"
#define MMIDC_FLOW_4539_112_2_18_2_8_37_201 "[MMIDC] MMIDC_DisplayPreviewFrame dst yaddr=0x%X,uaddr=0x%X"
#define MMIDC_FLOW_4543_112_2_18_2_8_37_202 "[MMIDC] MMIDC_DisplayPreviewFrame s_dc_is_preview=%d"
#define MMIDC_FLOW_4555_112_2_18_2_8_37_203 "[MMIDC] MMIDC_DisplayPreviewFrame t2=%d, t2-t1=%d"
#define MMIDC_FLOW_4585_112_2_18_2_8_37_204 "[MMIDC] MMIDC_SaveDataToFile ret =%d"
#define MMIDC_FLOW_4599_112_2_18_2_8_37_205 "[MMIDC] MMIDC_SendCaptureFailMsg ret==%d"
#define MMIDC_FULL_OSD_DISPLAY_143_112_2_18_2_8_38_206 "[MMIDC]: MMIDC_CompleteDisplayOSD"
#define MMIDC_FULL_OSD_DISPLAY_178_112_2_18_2_8_38_207 "[MMIDC]: ShowSoftKey layout_style = %d"
#define MMIDC_FULL_OSD_DISPLAY_372_112_2_18_2_8_38_208 "[MMIDC]: MMIDC_DisplaySoftKey mode = %d"
#define MMIDC_FULL_OSD_DISPLAY_1014_112_2_18_2_8_40_209 "[MMIDC] MMIDC_GetFrameDisplayPos id"
#define MMIDC_FULL_OSD_DISPLAY_1153_112_2_18_2_8_40_210 "[MMIDC] MMIDC_DisplayFrame frame index"
#define MMIDC_FULL_OSD_DISPLAY_1289_112_2_18_2_8_40_211 "[MMIDC] MMIDC_DisplayCurrentFileName NAME = PNULL"
#define MMIDC_FULL_OSD_DISPLAY_1412_112_2_18_2_8_40_212 "[MMIDC] MMIDC_DisplayCurrentFileName NAME = PNULL"
#define MMIDC_FULL_OSD_DISPLAY_2091_112_2_18_2_8_42_213 "[MMIDC] MMIDC_ShowAllOSD cur_mode = %d, show_state = %d"
#define MMIDC_FULL_OSD_DISPLAY_2128_112_2_18_2_8_42_214 "[MMIDC] MMIDC_ShowAllOSD is_show = %d"
#define MMIDC_FULL_OSD_DISPLAY_2332_112_2_18_2_8_42_215 "[MMIDC] MMIDC_DisplayLastFrame width = %d, height = %d"
#define MMIDC_FULL_OSD_DISPLAY_2335_112_2_18_2_8_42_216 "[MMIDC] MMIDC_DisplayLastFrame frame_ptr = 0x%lX"
#define MMIDC_FULL_OSD_DISPLAY_2346_112_2_18_2_8_42_217 "[MMIDC] MMIDC_DisplayLastFrame layer_ptr = 0x%lX, block_id=%d"
#define MMIDC_FULL_OSD_DISPLAY_2448_112_2_18_2_8_42_218 "[MMIDC] MMIDC_DisplayDateOSDStringByFont BUFFER NULL!!!"
#define MMIDC_GUI_286_112_2_18_2_8_43_219 "[MMIDC] MenuSetItemCallback index is overflow !!!"
#define MMIDC_GUI_319_112_2_18_2_8_43_220 "[MMIDC] MenuSetItemText index is overflow !!!"
#define MMIDC_GUI_1311_112_2_18_2_8_46_221 "[MMIDC] IconSetImage index is overflow !!!"
#define MMIDC_GUI_1330_112_2_18_2_8_46_222 "[MMIDC] IconCallBackFocusIcon is PNULL !!!"
#define MMIDC_GUI_1497_112_2_18_2_8_46_223 "[MMIDC] IconFocus index is overflow !!!"
#define MMIDC_GUI_1517_112_2_18_2_8_46_224 "[MMIDC] IconSetCallback index is overflow"
#define MMIDC_GUI_1532_112_2_18_2_8_46_225 "[MMIDC] IconSetOKCallback index is overflow !!!"
#define MMIDC_GUI_1867_112_2_18_2_8_47_226 "[MMIDC]: MMIAPIDC_FreeGUI"
#define MMIDC_GUIICONBAR_287_112_2_18_2_8_48_227 "[MMIDC] GetIconImageList ICON_IMG_MAX != total_img_number !!!"
#define MMIDC_GUIICONBAR_1411_112_2_18_2_8_50_228 "[MMIDC] MMIDC_DisplayIconDesktop is_open =%d"
#define MMIDC_GUIICONBAR_1425_112_2_18_2_8_50_229 "[MMIDC] MMIDC_DisplayIconDesktop ICON_IMG_MAX != total_img_number !!!"
#define MMIDC_GUIICONBAR_1610_112_2_18_2_8_50_230 "[MMIDC] MMIDC_DisplayIconDesktop tag ptr NULL !!!"
#define MMIDC_GUIICONBAR_1746_112_2_18_2_8_51_231 "[MMIDC] MMIDC_GetIconDesktopRectList max_number=%d, total_num=%d"
#define MMIDC_GUIICONBAR_1770_112_2_18_2_8_51_232 "[MMIDC] MMIDC_ShortcutForCapture mode=%d"
#define MMIDC_GUIICONBAR_1786_112_2_18_2_8_51_233 "[MMIDC] MMIDC_ShortcutForRecorder mode=%d"
#define MMIDC_GUIICONBAR_1802_112_2_18_2_8_51_234 "[MMIDC] MMIDC_ShortcutForCameraMode mode=%d"
#define MMIDC_GUIICONBAR_1842_112_2_18_2_8_51_235 "[MMIDC] MMIDC_ShortcutForSize mode=%d"
#define MMIDC_GUIICONBAR_1865_112_2_18_2_8_51_236 "[MMIDC] MMIDC_ShortcutForSize lcd_size=%d,screen_mode=%d"
#define MMIDC_GUIICONBAR_1939_112_2_18_2_8_51_237 "[MMIDC] MMIDC_ShortcutForSize total_size_list_number=%d, total_func_list_number=%d,cur_size=%d"
#define MMIDC_GUIICONBAR_1940_112_2_18_2_8_51_238 "[MMIDC] MMIDC_ShortcutForSize i=%d, next_size=%d"
#define MMIDC_GUIICONBAR_1960_112_2_18_2_8_51_239 "[MMIDC] MMIDC_ShortcutForEnvironment env=%d"
#define MMIDC_GUIICONBAR_1990_112_2_18_2_8_51_240 "[MMIDC] MMIDC_ShortcutForEffect ef=%d"
#define MMIDC_GUIICONBAR_2020_112_2_18_2_8_51_241 "[MMIDC] MMIDC_ShortcutForWhiteBalance wb=%d"
#define MMIDC_GUIICONBAR_2060_112_2_18_2_8_51_242 "[MMIDC] MMIDC_ShortcutForDelayTime self_time=%d"
#define MMIDC_GUIICONBAR_2100_112_2_18_2_8_51_243 "[MMIDC] MMIDC_ShortcutForStore dc_dev=%d"
#define MMIDC_GUIICONBAR_2149_112_2_18_2_8_51_244 "[MMIDC] MMIDC_ShortcutForStore dc_dev =%d, "
#define MMIDC_GUIICONBAR_2193_112_2_18_2_8_52_245 "[MMIDC] MMIDC_ShortcutForBrightnessToHigh brighness=%d, mode=%d"
#define MMIDC_GUIICONBAR_2225_112_2_18_2_8_52_246 "[MMIDC] MMIDC_ShortcutForBrightnessToLow brighness=%d, mode=%d"
#define MMIDC_GUIICONBAR_2256_112_2_18_2_8_52_247 "[MMIDC] MMIDC_ShortcutForContrastToHigh contrast=%d, mode=%d"
#define MMIDC_GUIICONBAR_2288_112_2_18_2_8_52_248 "[MMIDC] MMIDC_ShortcutForContrastToLow brighness=%d, mode=%d"
#define MMIDC_GUIICONBAR_2330_112_2_18_2_8_52_249 "[MMIDC] MMIDC_ShortcutForMultiShot multi=%d"
#define MMIDC_GUIICONBAR_2374_112_2_18_2_8_52_250 "[MMIDC] MMIDC_ClearIconDesktopRectList"
#define MMIDC_GUIICONBAR_2386_112_2_18_2_8_52_251 "[MMIDC] MMIDC_CloseGUIIconDesktop"
#define MMIDC_GUIPDA_1658_112_2_18_2_8_56_252 "[MMIDC] CreateDesktopPDA ret=%d"
#define MMIDC_GUIPDA_1693_112_2_18_2_8_56_253 "[MMIDC] DestroyDesktopPDA"
#define MMIDC_GUIPDA_1790_112_2_18_2_8_56_254 "[MMIDC] DisplayListCtrl is_ctrl_item=%d"
#define MMIDC_GUIPDA_2278_112_2_18_2_8_57_255 "[MMIDC] ProcessExpandBtn is_expand_state=%d, btn_list_focus=%d, is_attr_press=%d"
#define MMIDC_GUIPDA_2294_112_2_18_2_8_57_256 "[MMIDC] ProcessCmd1Btn"
#define MMIDC_GUIPDA_2310_112_2_18_2_8_57_257 "[MMIDC] ProcessCmd2Btn"
#define MMIDC_GUIPDA_2326_112_2_18_2_8_57_258 "[MMIDC] ProcessCmd3Btn"
#define MMIDC_GUIPDA_2349_112_2_18_2_8_57_259 "[MMIDC] ProcessCaptureBtn ret=%d"
#define MMIDC_GUIPDA_2381_112_2_18_2_8_57_260 "[MMIDC] ProcessCaptureKey ret=%d"
#define MMIDC_GUIPDA_2493_112_2_18_2_8_57_261 "[MMIDC] ProcessButtonList ctrl_id=%d"
#define MMIDC_GUIPDA_2553_112_2_18_2_8_57_262 "[MMIDC] MMIDC_SetDCIconTypeList icon_id=%d, cur_index=%d"
#define MMIDC_GUIPDA_2674_112_2_18_2_8_58_263 "[MMIDC] MMIDC_SetDCIconSettingTypeList ctrl_id=%d,cur_index=%d,icon_id=%d"
#define MMIDC_GUIPDA_2700_112_2_18_2_8_58_264 "[MMIDC] ProcessPicBtn"
#define MMIDC_GUIPDA_2719_112_2_18_2_8_58_265 "[MMIDC] ProcessSensorBtn"
#define MMIDC_GUIPDA_2760_112_2_18_2_8_58_266 "[MMIDC] ProcessZoomBtn ret=%d"
#define MMIDC_GUIPDA_2784_112_2_18_2_8_58_267 "[MMIDC] ProcessArrowLeftBtn ret=%d, is_visible=%D"
#define MMIDC_GUIPDA_2808_112_2_18_2_8_58_268 "[MMIDC] ProcessArrowRightBtn ret=%d, is_visible=%D"
#define MMIDC_GUIPDA_2826_112_2_18_2_8_58_269 "[MMIDC] ProcessUCameraBtn is_press=%d"
#define MMIDC_GUIPDA_2930_112_2_18_2_8_58_270 "[MMIDC] HandleTPPenOK ctrl_id=%d,is_update=%d, ret=%d"
#define MMIDC_GUIPDA_3032_112_2_18_2_8_58_271 "[MMIDC] HandleTpMove ret=%d, point.x=%d,point.y=%d"
#define MMIDC_GUIPDA_3108_112_2_18_2_8_59_272 "[MMIDC] IsContract is_press=%d,is_contract=%d"
#define MMIDC_GUIPDA_3131_112_2_18_2_8_59_273 "[MMIDC] HandleContractExpandBtnTpDown is_press=%d"
#define MMIDC_GUIPDA_3134_112_2_18_2_8_59_274 "[MMIDC] HandleContractExpandBtnTpDown ret=%d"
#define MMIDC_GUIPDA_3166_112_2_18_2_8_59_275 "[MMIDC] HandleContractExpandBtnTpUp is_press=%d, is_contract=%d"
#define MMIDC_GUIPDA_3169_112_2_18_2_8_59_276 "[MMIDC] HandleContractExpandBtnTpUp ret=%d"
#define MMIDC_GUIPDA_3337_112_2_18_2_8_59_277 "[MMIDC] DesktopPDAProcessMsg ret=%d, msg_id=0x%lX"
#define MMIDC_GUIPDA_3511_112_2_18_2_8_59_278 "[MMIDC] SetAllCmdBtnsVisible is_visible=%d"
#define MMIDC_GUIPDA_3914_112_2_18_2_9_0_279 "[MMIDC]: CreateAllCmdBtns mode = %d"
#define MMIDC_GUIPDA_5680_112_2_18_2_9_4_280 "[MMIDC] DisplayPDAAllItems cur_mode = %d, show_state = %d"
#define MMIDC_GUIPDA_5708_112_2_18_2_9_4_281 "[MMIDC] DisplayPDAAllItems is_show = %d"
#define MMIDC_GUIPDA_6121_112_2_18_2_9_4_282 "[MMIDC] FindFirstImgFiles param error!"
#define MMIDC_GUIPDA_6129_112_2_18_2_9_4_283 "[MMIDC]:FindFirstImgFiles error:PNULL == find_data_ptr"
#define MMIDC_GUIPDA_6137_112_2_18_2_9_5_284 "[MMIDC]:FindFirstImgFiles error:PNULL == find"
#define MMIDC_GUIPDA_6178_112_2_18_2_9_5_285 "[MMIDC] FindFirstImgFile ret=%d"
#define MMIDC_GUIPDA_6307_112_2_18_2_9_5_286 "[MMIDC] GetImgDecodedData Prame NULL !!!"
#define MMIDC_GUIPDA_6340_112_2_18_2_9_5_287 "[MMIDC] GetImgDecodedData no memory"
#define MMIDC_GUIPDA_6369_112_2_18_2_9_5_288 "[MMIDC] GetImgDecodedData no memory"
#define MMIDC_GUIPDA_6393_112_2_18_2_9_5_289 "[MMIDC] GetImgDecodedData result = %d,img_ptr=0x%lX,img_width=%d,img_height=%d"
#define MMIDC_GUIPDA_7082_112_2_18_2_9_6_290 "[MMIDC] PDAUpdateZoomValue ret=%d, cur_value=%d"
#define MMIDC_GUIPDA_7120_112_2_18_2_9_6_291 "[MMIDC] UpdateZoomValue cur_step=%d,total_step=%d,total_value=%d,cur_value=%d"
#define MMIDC_GUIPDA_7147_112_2_18_2_9_6_292 "[MMIDC] HandleZoomSeekBarTimer ret=%d"
#define MMIDC_GUIPDA_7250_112_2_18_2_9_7_293 "[MMIDC] HandleZoomSeekBarTPMove y_step=%d,move_value=%d,cur_value=%d"
#define MMIDC_GUIPDA_7256_112_2_18_2_9_7_294 "[MMIDC] HandleZoomSeekBarTPMove ret=%d,is_down=%d"
#define MMIDC_GUIPDA_7314_112_2_18_2_9_7_295 "[MMIDC] HandleZoomSeekBarTpDown ret=%d"
#define MMIDC_GUIPDA_7353_112_2_18_2_9_7_296 "[MMIDC] HandleZoomSeekBarTpUp update_result=%d"
#define MMIDC_GUIPDA_7393_112_2_18_2_9_7_297 "[MMIDC] HandleZoomSeekBarTpUp ret=%d,is_down=%d"
#define MMIDC_GUIPDA_7429_112_2_18_2_9_7_298 "[MMIDC] CreateImageAttrInfo attr_ptr=0x%lX"
#define MMIDC_GUIPDA_7437_112_2_18_2_9_7_299 "[MMIDC] CreateImageAttrInfo ret=%d"
#define MMIDC_GUIPDA_7449_112_2_18_2_9_7_300 "[MMIDC] DestroyImageAttrInfo attr_ptr=0x%lX"
#define MMIDC_GUIPDA_7897_112_2_18_2_9_8_301 "[MMIDC] PDAUpdateImageAttrValue ret=%d, cur_value=%d,cur_attr=%d"
#define MMIDC_GUIPDA_7944_112_2_18_2_9_8_302 "[MMIDC] UpdateImageAttrValue cur_step=%d,total_step =%d,total_value=%d,cur_value=%d"
#define MMIDC_GUIPDA_7974_112_2_18_2_9_8_303 "[MMIDC] HandleImageAttrSeekBarTimer ret=%d"
#define MMIDC_GUIPDA_8061_112_2_18_2_9_8_304 "[MMIDC] HandleImageAttrSeekBarTpMove y_step=%d,move_value=%d,cur_value=%d"
#define MMIDC_GUIPDA_8066_112_2_18_2_9_8_305 "[MMIDC] HandleImageAttrSeekBarTpMove ret=%d,is_down=%d"
#define MMIDC_GUIPDA_8118_112_2_18_2_9_8_306 "[MMIDC] HandleImageAttrSeekBarTpDown ret=%d, is_attr_press=%d"
#define MMIDC_GUIPDA_8159_112_2_18_2_9_8_307 "[MMIDC] HandleImageAttrSeekBarTpUp update_result=%d"
#define MMIDC_GUIPDA_8184_112_2_18_2_9_9_308 "[MMIDC] HandleImageAttrSeekBarTpUp cur_value=%d,total_value=%d,cur_step=%d,total_step"
#define MMIDC_GUIPDA_8190_112_2_18_2_9_9_309 "[MMIDC] HandleImageAttrSeekBarTpUp ret=%d,point.x=%d,pointl.y=%d, is_down=%d"
#define MMIDC_GUIPDA_8440_112_2_18_2_9_9_310 "[MMIDC] HandlePDAKey is_btn_expand_press=%d"
#define MMIDC_GUIPDA_8542_112_2_18_2_9_9_311 "[MMIDC] HandlePDAKey msg_id=0x%lX, ret=%d, key=%d"
#define MMIDC_GUIPDA_8651_112_2_18_2_9_9_312 "[MMIDC] RestorePDADesktop is_zoom_press=%d"
#define MMIDC_GUIPDA_8960_112_2_18_2_9_10_313 "[MMIDC] HandleGlideExpandTpUp is_visible =%d, is_glide=%d"
#define MMIDC_GUIPDA_8963_112_2_18_2_9_10_314 "[MMIDC] HandleGlideExpandTpUp ret =%d, is_move_panel=%d"
#define MMIDC_GUIPDA_9257_112_2_18_2_9_11_315 "[MMIDC MMIDC_StartPreviewAnim step=%d"
#define MMIDC_GUIPDA_9305_112_2_18_2_9_11_316 "[MMIDC] MMIDC_StartPreviewAnim start anim time=%d"
#define MMIDC_GUIPDA_9381_112_2_18_2_9_11_317 "[MMIDC] MMIDC_StartPreviewAnim end_time=%d"
#define MMIDC_GUIPDA_9859_112_2_18_2_9_12_318 "[MMIDC] ProcessEffect360LeftBtn ret=%d, is_visible=%D"
#define MMIDC_GUIPDA_9884_112_2_18_2_9_12_319 "[MMIDC] ProcessEffect360RightBtn ret=%d, is_visible=%D"
#define MMIDC_IMAGEPROCESS_172_112_2_18_2_9_12_320 "[MMDIC] GetImageProcessDirectory ret=%d"
#define MMIDC_IMAGEPROCESS_201_112_2_18_2_9_13_321 "[MMIDC] OpenImageProc ret=%d"
#define MMIDC_IMAGEPROCESS_270_112_2_18_2_9_13_322 "[MMIDC] MMIDC_OpenImgProc ret=%d"
#define MMIDC_IMAGEPROCESS_271_112_2_18_2_9_13_323 "[MMIDC] MMIDC_OpenImgProc err=%d"
#define MMIDC_IMAGEPROCESS_328_112_2_18_2_9_13_324 "[MMIDC] MMIDC_CloseUCamera"
#define MMIDC_IMAGEPROCESS_346_112_2_18_2_9_13_325 "[MMIDC] MMIDC_IsUCameraEnable is_enable=%d"
#define MMIDC_IMAGEPROCESS_363_112_2_18_2_9_13_326 "trace_param cmd=%d"
#define MMIDC_IMAGEPROCESS_366_112_2_18_2_9_13_327 "trace_param src =0x%X"
#define MMIDC_IMAGEPROCESS_367_112_2_18_2_9_13_328 "trace_param src format=%d"
#define MMIDC_IMAGEPROCESS_369_112_2_18_2_9_13_329 "trace_param src format=%d"
#define MMIDC_IMAGEPROCESS_372_112_2_18_2_9_13_330 "trace_param src chn0=0x%X,chn0_len=%d"
#define MMIDC_IMAGEPROCESS_374_112_2_18_2_9_13_331 "trace_param src chn1=0x%X,chn1_len=%d"
#define MMIDC_IMAGEPROCESS_376_112_2_18_2_9_13_332 "trace_param proc_mode=0x%X"
#define MMIDC_IMAGEPROCESS_381_112_2_18_2_9_13_333 "trace_param proc_img w=%d, h=%d"
#define MMIDC_IMAGEPROCESS_384_112_2_18_2_9_13_334 "trace_param proc_img format=%d"
#define MMIDC_IMAGEPROCESS_387_112_2_18_2_9_13_335 "trace_param proc_img ctrl_callback=0x%X"
#define MMIDC_IMAGEPROCESS_390_112_2_18_2_9_13_336 "trace_param proc_img size w=%d h=%d"
#define MMIDC_IMAGEPROCESS_393_112_2_18_2_9_13_337 "trace_param proc_img chn0 =0x%X chn0_len=%d"
#define MMIDC_IMAGEPROCESS_396_112_2_18_2_9_13_338 "trace_param proc_img chn1 =0x%X chn1_len=%d"
#define MMIDC_IMAGEPROCESS_401_112_2_18_2_9_13_339 "trace_param before 0 w=%d, h=%d"
#define MMIDC_IMAGEPROCESS_404_112_2_18_2_9_13_340 "trace_param before 0 format=%d"
#define MMIDC_IMAGEPROCESS_407_112_2_18_2_9_13_341 "trace_param before 0 ctrl_callback=0x%X"
#define MMIDC_IMAGEPROCESS_410_112_2_18_2_9_13_342 "trace_param before 0 size w=%d h=%d"
#define MMIDC_IMAGEPROCESS_413_112_2_18_2_9_13_343 "trace_param before 0 chn0 =0x%X chn0_len=%d"
#define MMIDC_IMAGEPROCESS_416_112_2_18_2_9_13_344 "trace_param before 0 chn1 =0x%X chn1_len=%d"
#define MMIDC_IMAGEPROCESS_519_112_2_18_2_9_13_345 "[MMIDC] DoImageProcess src w=%d,h=%d"
#define MMIDC_IMAGEPROCESS_523_112_2_18_2_9_13_346 "[MMIDC] DoImageProcess src chn1_ptr=0x%X, chn1_size=%d"
#define MMIDC_IMAGEPROCESS_526_112_2_18_2_9_13_347 "[MMIDC] DoImageProcess src chn2_ptr=0x%X,chn2_size=%d"
#define MMIDC_IMAGEPROCESS_529_112_2_18_2_9_13_348 "[MMIDC] DoImageProcess src chn3_ptr=0x%X,chn3_size=%d"
#define MMIDC_IMAGEPROCESS_597_112_2_18_2_9_13_349 "[MMIDC] DoImageProcess img_proc_ret=%d"
#define MMIDC_IMAGEPROCESS_601_112_2_18_2_9_13_350 "[MMIDC] DoImageProcess t2=%d, t2-t1=%d"
#define MMIDC_IMAGEPROCESS_605_112_2_18_2_9_13_351 "[MMIDC] DoImageProcess rtn chn0=0x%X,chn0_size=%d"
#define MMIDC_IMAGEPROCESS_608_112_2_18_2_9_13_352 "[MMIDC] DoImageProcess rtn chn1=0x%X,chn1_size=%d"
#define MMIDC_IMAGEPROCESS_612_112_2_18_2_9_13_353 "[MMIDC] DoImageProcess dst chn1=0x%X,chn1_size=%d"
#define MMIDC_IMAGEPROCESS_615_112_2_18_2_9_13_354 "[MMIDC] DoImageProcess dst chn2=0x%X,chn2_size=%d"
#define MMIDC_IMAGEPROCESS_673_112_2_18_2_9_14_355 "[MMIDC] MMIDC_ProcessImage get src data img_proc_ret=%d"
#define MMIDC_IMAGEPROCESS_726_112_2_18_2_9_14_356 "[MMIDC] MMIDC_ProcessImage is_skip_image_process=%d"
#define MMIDC_IMAGEPROCESS_729_112_2_18_2_9_14_357 "[MMIDC] MMIDC_ProcessImage img_proc_ret=%d"
#define MMIDC_IMAGEPROCESS_751_112_2_18_2_9_14_358 "[MMIDC] MMIDC_SetImageProcessType type=%d"
#define MMIDC_IMAGEPROCESS_769_112_2_18_2_9_14_359 "[MMIDC] MMIDC_GetImageProcessType type=%d"
#define MMIDC_IMAGEPROCESS_873_112_2_18_2_9_14_360 "[MMIDC] MMIDC_GetDataSizeByImgProcFormat data_size=%d"
#define MMIDC_IMAGEPROCESS_893_112_2_18_2_9_14_361 "[MMIDC] MMIDC_IsPhotoData ret=%d"
#define MMIDC_IMAGEPROCESS_911_112_2_18_2_9_14_362 "[MMIDC] MMIDC_SetPhotoDataFlag is_enable=%d"
#define MMIDC_IMAGEPROCESS_929_112_2_18_2_9_14_363 "[MMIDC] MMIDC_GetUCameraStatus status=%d"
#define MMIDC_IMAGEPROCESS_947_112_2_18_2_9_14_364 "[MMIDC] MMIDC_SetUCameraStatus status=%d"
#define MMIDC_IMAGEPROCESS_967_112_2_18_2_9_14_365 "[MMIDC] MMIDC_IsUCameraDoing ret=%d"
#define MMIDC_IMAGEPROCESS_1037_112_2_18_2_9_14_366 "[MMIDC] MMIDC_UCameraASyncCallbackToSavePhotoFile format=%d"
#define MMIDC_IMAGEPROCESS_1040_112_2_18_2_9_14_367 "[MMIDC] MMIDC_UCameraASyncCallbackToSavePhotoFile chn0=0x%X,chn0_len=%d"
#define MMIDC_IMAGEPROCESS_1074_112_2_18_2_9_14_368 "[MMIDC] MMIDC_UCameraASyncCallbackSendMsgToApp format=%d"
#define MMIDC_IMAGEPROCESS_1077_112_2_18_2_9_14_369 "[MMIDC] MMIDC_UCameraASyncCallbackSendMsgToApp chn0=0x%X,chn0_len=%d"
#define MMIDC_IMAGEPROCESS_1120_112_2_18_2_9_14_370 "[MMIDC] MMIDC_GetDataSizeForJPG width=%d, height=%d"
#define MMIDC_IMAGEPROCESS_1123_112_2_18_2_9_14_371 "[MMIDC] MMIDC_GetDataSizeForJPG data_size=%d"
#define MMIDC_IMAGEPROCESS_1157_112_2_18_2_9_14_372 "[MMIDC] MMIDC_OpenUCameraCurTypeFunction type=%d"
#define MMIDC_IMAGEPROCESS_1160_112_2_18_2_9_14_373 "[MMIDC] MMIDC_OpenUCameraCurTypeFunction ret=%d"
#define MMIDC_IMAGEPROCESS_1193_112_2_18_2_9_15_374 "[MMIDC] MMIDC_DestroyUCameraCurTypeFunction type=%d"
#define MMIDC_IMAGEPROCESS_1219_112_2_18_2_9_15_375 "[MMIDC] MMIDC_SetUCameraPhotoData buf_ptr=0x%X,buf_size = %d"
#define MMIDC_IMAGEPROCESS_1222_112_2_18_2_9_15_376 "[MMIDC] MMIDC_SetUCameraPhotoData is_enable = %d"
#define MMIDC_IMAGEPROCESS_1250_112_2_18_2_9_15_377 "[MMIDC] MMIDC_GetUCameraPhotoData is_enable = %d"
#define MMIDC_MAIN_WINTAB_476_112_2_18_2_9_16_378 "[MMI DC]: HandleHelpWinMsg, msg_id = %x"
#define MMIDC_MAIN_WINTAB_547_112_2_18_2_9_16_379 "[MMIDC] MMIDC_SetLcdForRotate set = %d"
#define MMIDC_MAIN_WINTAB_548_112_2_18_2_9_16_380 "[MMIDC] MMIDC_SetLcdForRotate platform_angle = %d"
#define MMIDC_MAIN_WINTAB_549_112_2_18_2_9_16_381 "[MMIDC] MMIDC_SetLcdForRotate current_angle = %d"
#define MMIDC_MAIN_WINTAB_580_112_2_18_2_9_16_382 "[MMIDC]: HandleOSDKey key = %d"
#define MMIDC_MAIN_WINTAB_745_112_2_18_2_9_16_383 "StartSwitchOSDMenuItemsKeyTimer: the key timer has started!"
#define MMIDC_MAIN_WINTAB_763_112_2_18_2_9_16_384 "StopSwitchOSDMenuItemsKeyTimer: the key timer has stop!"
#define MMIDC_MAIN_WINTAB_779_112_2_18_2_9_16_385 "MMIDC_StartTextScrollTimer: the key timer has started!"
#define MMIDC_MAIN_WINTAB_796_112_2_18_2_9_16_386 "MMIDC_StopTextScrollTimer: the key timer has stop!"
#define MMIDC_MAIN_WINTAB_841_112_2_18_2_9_17_387 "[MMIDC] MMIAPIDC_OpenPhotoWin java running =%d"
#define MMIDC_MAIN_WINTAB_897_112_2_18_2_9_17_388 "[MMIDC] MMIAPIDC_OpenPhotoWin"
#define MMIDC_MAIN_WINTAB_924_112_2_18_2_9_17_389 "[MMIDC] MMIAPIDC_OpenUCameraPhotoWin"
#define MMIDC_MAIN_WINTAB_968_112_2_18_2_9_17_390 "[MMIDC] MMIAPIDC_OpenVideoWin"
#define MMIDC_MAIN_WINTAB_1886_112_2_18_2_9_19_391 "[MMIDC] PauseOrResume s_is_dv_pause = %d"
#define MMIDC_MAIN_WINTAB_1951_112_2_18_2_9_19_392 "[MMIDC]KeyFunction key >= DC_KEY_MAX"
#define MMIDC_MAIN_WINTAB_1957_112_2_18_2_9_19_393 "[MMIDC] KeyFunction key=%d,cur_mode=%d"
#define MMIDC_MAIN_WINTAB_1975_112_2_18_2_9_19_394 "[MMIDC]: MMIDC_HandleDeletePubWinBG"
#define MMIDC_MAIN_WINTAB_2036_112_2_18_2_9_19_395 "[MMIDC] HandleTPForMultiPhoto ret=%d"
#define MMIDC_MAIN_WINTAB_2058_112_2_18_2_9_19_396 "[MMIDC]: GUI_PointIsInRect rs_rect"
#define MMIDC_MAIN_WINTAB_2064_112_2_18_2_9_19_397 "[MMIDC]: GUI_PointIsInRect mid_rect"
#define MMIDC_MAIN_WINTAB_2069_112_2_18_2_9_19_398 "[MMIDC]: GUI_PointIsInRect ls_rect"
#define MMIDC_MAIN_WINTAB_2077_112_2_18_2_9_19_399 "[MMIDC] HandleTPForSoftkey ret=%d"
#define MMIDC_MAIN_WINTAB_2090_112_2_18_2_9_19_400 "[MMIDC]: HandleDCWinTpPressDown %d, %d"
#define MMIDC_MAIN_WINTAB_2236_112_2_18_2_9_20_401 "[MMIDC]: MMIDC_HandleGetFocus mode = %d"
#define MMIDC_MAIN_WINTAB_2315_112_2_18_2_9_20_402 "[MMIDC]: MSG_TIMER s_3seconds_timer"
#define MMIDC_MAIN_WINTAB_2330_112_2_18_2_9_20_403 "[MMIDC]: MSG_TIMER s_timer_delay"
#define MMIDC_MAIN_WINTAB_2383_112_2_18_2_9_20_404 "[MMIDC]: MSG_TIMER s_updown_tip_timer"
#define MMIDC_MAIN_WINTAB_2404_112_2_18_2_9_20_405 "[MMIDC]: MSG_TIMER s_switch_osd_key_timer_id last_key=%d"
#define MMIDC_MAIN_WINTAB_2472_112_2_18_2_9_20_406 "[MMIDC]: MSG_TIMER s_record_timer"
#define MMIDC_MAIN_WINTAB_2486_112_2_18_2_9_20_407 "[MMIDC]: MSG_TIMER s_text_scroll_timer"
#define MMIDC_MAIN_WINTAB_2504_112_2_18_2_9_20_408 "[MMIDC] s_hit_timer exec"
#define MMIDC_MAIN_WINTAB_2720_112_2_18_2_9_20_409 "[MMIDC] StartDCActivity audio_handle=%d"
#define MMIDC_MAIN_WINTAB_2809_112_2_18_2_9_21_410 "[MMIDC]: HandleDCWaitWinMsg, msg_id = 0x%.2x"
#define MMIDC_MAIN_WINTAB_2815_112_2_18_2_9_21_411 "[MMIDC] HandleDCWaitWinMsg start_time=%d"
#define MMIDC_MAIN_WINTAB_2820_112_2_18_2_9_21_412 "[MMIDC] HandleDCWaitWinMsg mode=%d"
#define MMIDC_MAIN_WINTAB_2893_112_2_18_2_9_21_413 "[MMIDC] ProcessRecordEnd is_normal= %d"
#define MMIDC_MAIN_WINTAB_2914_112_2_18_2_9_21_414 "[MMIDC]: HandleCameraWinMsg, msg_id = 0x%.2x"
#define MMIDC_MAIN_WINTAB_2919_112_2_18_2_9_21_415 "[MMIDC]: MSG_OPEN_WINDOW"
#define MMIDC_MAIN_WINTAB_2966_112_2_18_2_9_21_416 "[MMIDC] HandleCameraWinMsg flip_status = %d"
#define MMIDC_MAIN_WINTAB_3017_112_2_18_2_9_21_417 "[MMIDC]: HandleCameraWinMsg, MSG_LOSE_FOCUS"
#define MMIDC_MAIN_WINTAB_3029_112_2_18_2_9_21_418 "[MMIDC]: HandleCameraWinMsg, MSG_GET_FOCUS"
#define MMIDC_MAIN_WINTAB_3069_112_2_18_2_9_21_419 "[MMIDC]: MSG_DC_CAPTURE_SUCCESS_MSG"
#define MMIDC_MAIN_WINTAB_3115_112_2_18_2_9_21_420 "[MMIDC]: MSG_DC_REVIEW_SUCCESS_MSG"
#define MMIDC_MAIN_WINTAB_3119_112_2_18_2_9_21_421 "[MMIDC]: MSG_DC_CAPTURE_FAILE_MSG"
#define MMIDC_MAIN_WINTAB_3187_112_2_18_2_9_21_422 "[MMIDC]: MSG_APP_WEB"
#define MMIDC_MAIN_WINTAB_3222_112_2_18_2_9_21_423 "[MMIDC]: MSG_APP_OK"
#define MMIDC_MAIN_WINTAB_3249_112_2_18_2_9_22_424 "[MMIDC]: MSG_APP_CANCEL"
#define MMIDC_MAIN_WINTAB_3266_112_2_18_2_9_22_425 "[MMIDC]: MSG_APP_LEFT"
#define MMIDC_MAIN_WINTAB_3291_112_2_18_2_9_22_426 "[MMIDC]: MSG_APP_RIGHT"
#define MMIDC_MAIN_WINTAB_3325_112_2_18_2_9_22_427 "[MMIDC]: MSG_KEYUP"
#define MMIDC_MAIN_WINTAB_3332_112_2_18_2_9_22_428 "[MMIDC]: MSG_APP_UP"
#define MMIDC_MAIN_WINTAB_3358_112_2_18_2_9_22_429 "[MMIDC]: MSG_APP_DOWN"
#define MMIDC_MAIN_WINTAB_3403_112_2_18_2_9_22_430 "[MMIDC]: MSG_CLOSE_WINDOW"
#define MMIDC_MAIN_WINTAB_3410_112_2_18_2_9_22_431 "[MMIDC] HandleCameraWinMsg: MSG_KEYDOWN_FLIP!"
#define MMIDC_MAIN_WINTAB_3429_112_2_18_2_9_22_432 "[MMIDC] HandleCameraWinMsg: MSG_KEYUP_FLIP! %d %d"
#define MMIDC_MAIN_WINTAB_3436_112_2_18_2_9_22_433 "[MMIDC] Go on preview!"
#define MMIDC_MAIN_WINTAB_3446_112_2_18_2_9_22_434 "[MMIDC] Display picture!"
#define MMIDC_MAIN_WINTAB_3466_112_2_18_2_9_22_435 "[MMIDC] HandleCameraWinMsg: MSG_LCD_SWITCH"
#define MMIDC_MAIN_WINTAB_3494_112_2_18_2_9_22_436 "[MMIDC]: HandleCameraWinMsg msg_id = 0x%.2x"
#define MMIDC_MAIN_WINTAB_3593_112_2_18_2_9_22_437 "[MMIDC]: MMIAPIDC_Exit"
#define MMIDC_MAIN_WINTAB_3634_112_2_18_2_9_22_438 "[MMIDC]: MMIAPIDC_Interrupt"
#define MMIDC_MAIN_WINTAB_3690_112_2_18_2_9_22_439 "mmidc_wintab.c,  HandleSendWinMsg(), msg_id = %x"
#define MMIDC_MAIN_WINTAB_3902_112_2_18_2_9_23_440 "[MMIDC] ProcessChipTestStep step=%d"
#define MMIDC_MAIN_WINTAB_4008_112_2_18_2_9_23_441 "[MMIDC] RestoreChipTestLastVideoSize video_size =%d"
#define MMIDC_MAIN_WINTAB_4027_112_2_18_2_9_23_442 "[MMIDC] RestoreChipTestLastAutoSave autosave=%d"
#define MMIDC_MAIN_WINTAB_4044_112_2_18_2_9_23_443 "[MMIDC] HandleShortcutKey msg_id = %d, mode=%d"
#define MMIDC_MAIN_WINTAB_4217_112_2_18_2_9_23_444 "[MMIDC] HandleTPForIconBar is_desktop_open=%d"
#define MMIDC_MAIN_WINTAB_4239_112_2_18_2_9_23_445 "[MMIDC] HandleTPForIconBar rect_index=%d"
#define MMIDC_MAIN_WINTAB_4301_112_2_18_2_9_24_446 "[MMIDC] HandleTPForIconBar msg_id=%d, ret=%d"
#define MMIDC_MAIN_WINTAB_4337_112_2_18_2_9_24_447 "[MMIDC] HandleTPForIconBar rect_index=%d"
#define MMIDC_MAIN_WINTAB_4604_112_2_18_2_9_24_448 "[MMIDC] MMIDC_PDACmd key=%d,cur_mode=%d"
#define MMIDC_MAIN_WINTAB_4608_112_2_18_2_9_24_449 "[MMIDC]MMIDC_PDACmd key >= PDA_KEY_MAX"
#define MMIDC_MAIN_WINTAB_4663_112_2_18_2_9_24_450 "[MMIDC] DCApplet_HandleEvent msg_id = 0x%04x"
#define MMIDC_MAIN_WINTAB_4677_112_2_18_2_9_24_451 "[MMIDC] DCApplet_HandleEvent MSG_APPLET_START"
#define MMIDC_MAIN_WINTAB_4682_112_2_18_2_9_24_452 "[MMIDC] DCApplet_HandleEvent MSG_APPLET_STOP"
#define MMIDC_MAIN_WINTAB_4716_112_2_18_2_9_24_453 "[MMIDC] CloseDCApplet applet_ptr=0x%lX"
#define MMIDC_MAIN_WINTAB_4755_112_2_18_2_9_25_454 "[MMIDC] StartDCApplet ret = %d,applet_ptr=0x%lX"
#define MMIDC_MAIN_WINTAB_4773_112_2_18_2_9_25_455 "[MMIDC] MMIDC_CreateWinByApplet applet_ptr=0x%lX"
#define MMIDC_MAIN_WINTAB_4803_112_2_18_2_9_25_456 "[MMIDC] MMIDC_GetFirstPreview ret=%d"
#define MMIDC_MAIN_WINTAB_4823_112_2_18_2_9_25_457 "[MMIDC] MMIDC_SetFirstPreview is_first_preview=%d"
#define MMIDC_MAIN_WINTAB_4843_112_2_18_2_9_25_458 "[MMIDC] MMIDC_GetDCModuleRunningFlag ret=%d"
#define MMIDC_MAIN_WINTAB_4862_112_2_18_2_9_25_459 "[MMIDC] MMIDC_SetDCModuleRunningFlag is_running=%d"
#define MMIDC_MAIN_WINTAB_4921_112_2_18_2_9_25_460 "[MMIDC] MMIDC_ProcessMsgCmd cmd=%d"
#define MMIDC_NINONE_106_112_2_18_2_9_25_461 "[MMIDC] GetNInOneHandle handle=%d"
#define MMIDC_NINONE_133_112_2_18_2_9_25_462 "[MMIDC] CreateNInOneHandle ret=%d,handle=%d"
#define MMIDC_NINONE_157_112_2_18_2_9_25_463 "[MMIDC] DestroyNInOneHandle ret=%d"
#define MMIDC_NINONE_214_112_2_18_2_9_25_464 "[MMIDC] CreateNInOneOSDInfo img_size.w=%d, h=%d"
#define MMIDC_NINONE_217_112_2_18_2_9_25_465 "[MMIDC] CreateNInOneOSDInfo ret=%d"
#define MMIDC_NINONE_246_112_2_18_2_9_26_466 "[MMIDC] DestroyNInOneOSDInfo"
#define MMIDC_NINONE_310_112_2_18_2_9_26_467 "[MMIDC] CreateNInOnePhotoInfo buf_ptr=0x%X, buf_size=%d"
#define MMIDC_NINONE_313_112_2_18_2_9_26_468 "[MMIDC] CreateNInOnePhotoInfo ret=%d"
#define MMIDC_NINONE_339_112_2_18_2_9_26_469 "[MMIDC] DestroyNInOnePhotoInfo"
#define MMIDC_NINONE_426_112_2_18_2_9_26_470 "[MMIDC] MMIDC_OpenNInOne ret=%d"
#define MMIDC_NINONE_474_112_2_18_2_9_26_471 "[MMIDC] MMIDC_IsNInOneEnable is_enable=%d"
#define MMIDC_NINONE_786_112_2_18_2_9_27_472 "[MMIDC] ProcessPhotoImageCallback cur_index=%d"
#define MMIDC_NINONE_837_112_2_18_2_9_27_473 "[MMIDC] ViewOsdImageCallback buffer_ptr=0x%X,buffer_size=%d"
#define MMIDC_NINONE_840_112_2_18_2_9_27_474 "[MMIDC] ViewOsdImageCallback W=%d, h=%d"
#define MMIDC_NINONE_982_112_2_18_2_9_27_475 "[MMIDC] MMIDC_GetNInOneImageProcessParam"
#define MMIDC_NINONE_1036_112_2_18_2_9_27_476 "[MMIDC] MMIDC_GetNInOneImageProcessParam ret=%d"
#define MMIDC_NINONE_1039_112_2_18_2_9_27_477 "[MMIDC] MMIDC_GetNInOneImageProcessParam is_skip_image_process=%d"
#define MMIDC_OSD_OPTION_550_112_2_18_2_9_29_478 "[MMIDC] OpenDCModeMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_720_112_2_18_2_9_29_479 "[MMIDC] SetPhotoSizeState photo_size = %d"
#define MMIDC_OSD_OPTION_740_112_2_18_2_9_29_480 "[MMIDC] SetPhotoSizeState screen mode = %d"
#define MMIDC_OSD_OPTION_1139_112_2_18_2_9_30_481 "[MMIDC] GetPhonePhotoSizeList index=%d"
#define MMIDC_OSD_OPTION_1180_112_2_18_2_9_30_482 "[MMIDC] GetPhonePhotoSizeFunctionList callback array"
#define MMIDC_OSD_OPTION_1190_112_2_18_2_9_30_483 "[MMIDC] GetPhonePhotoSizeFunctionList copy_count=%d"
#define MMIDC_OSD_OPTION_1538_112_2_18_2_9_31_484 "[MMIDC] MMIDC_GetPhoneVideoSizeList total_item=%d,count=%d"
#define MMIDC_OSD_OPTION_1588_112_2_18_2_9_31_485 "[MMIDC] MMIDC_GetPhoneVideoSizeFunctionList copy_count=%d, total_item=%d"
#define MMIDC_OSD_OPTION_1845_112_2_18_2_9_31_486 "[MMIDC] MMIDC_OpenVideoOption osd_menu = PNULL"
#define MMIDC_OSD_OPTION_1972_112_2_18_2_9_32_487 "[MMIDC] MMIDC_MoveCurReviewPhotoID() KEY"
#define MMIDC_OSD_OPTION_1991_112_2_18_2_9_32_488 "[MMIDC] MMIDC_OpenVideoReviewOption osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2072_112_2_18_2_9_32_489 "[MMIDC] MMIDC_ShortcutForSwitchSensor sensor_id=%d"
#define MMIDC_OSD_OPTION_2304_112_2_18_2_9_32_490 "[MMIDC] OpenMultiShotMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2359_112_2_18_2_9_32_491 "[MMIDC] OpenPhotoEffectMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2400_112_2_18_2_9_32_492 "[MMIDC] OpenPhotoEnvironmentMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2434_112_2_18_2_9_32_493 "[MMIDC] OpenPhotoFrameMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2599_112_2_18_2_9_33_494 "[MMIDC] OpenSelfDelayMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2630_112_2_18_2_9_33_495 "[MMIDC] OpenDCModeMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2673_112_2_18_2_9_33_496 "[MMIDC] OpenVideoEffectMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2729_112_2_18_2_9_33_497 "[MMIDC] OpenVideoSizeMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2742_112_2_18_2_9_33_498 "[MMIDC] OpenVideoSizeMenu cur_size=%d"
#define MMIDC_OSD_OPTION_2750_112_2_18_2_9_33_499 "[MMIDC] OpenVideoSizeMenu total_size_number=%d, total_func_number=%d error!!!"
#define MMIDC_OSD_OPTION_2786_112_2_18_2_9_33_500 "[MMIDC] OpenWhiteBalanceMenu osd_menu = PNULL"
#define MMIDC_OSD_OPTION_2826_112_2_18_2_9_33_501 "[MMIDC] OpenWhiteBalanceMenu() TYPE"
#define MMIDC_OSD_OPTION_2867_112_2_18_2_9_33_502 "[MMIDC] OpenPhotoEnvironmentMenu osd_icon = PNULL"
#define MMIDC_OSD_OPTION_3774_112_2_18_2_9_35_503 "[MMIDC] MMIDC_OpenPhotoOption osd_menu = PNULL"
#define MMIDC_OSD_OPTION_4072_112_2_18_2_9_36_504 "[MMIDC] MMIDC_GetDVSettingTypeListItems cur_size=%d"
#define MMIDC_OSD_OPTION_4080_112_2_18_2_9_36_505 "[MMIDC] MMIDC_GetDVSettingTypeListItems total_size_number=%d, total_func_number=%d error!!!"
#define MMIDC_PHOTODATE_96_112_2_18_2_9_37_506 "[MMIDC] SetPhotoDateLayerID id=%d"
#define MMIDC_PHOTODATE_155_112_2_18_2_9_37_507 "[MMIDC] GetDateOSDInitAttribute photo_size=%d, total_items=%d"
#define MMIDC_PHOTODATE_445_112_2_18_2_9_38_508 "[MMIDC] DisplayPhotoDateString date_width=%d,time_width=%d,interval_width=%d"
#define MMIDC_PHOTODATE_449_112_2_18_2_9_38_509 "[MMIDC] DisplayPhotoDateString text_width_max=%d,text_height_max=%d"
#define MMIDC_PHOTODATE_516_112_2_18_2_9_38_510 "[MMIDC] DisplayPhotoDateString photo_size=%d,x=%d, y=%d"
#define MMIDC_PHOTODATE_535_112_2_18_2_9_38_511 "[MMIDC] GetPhotoDateOSDRectWidthAndHeight width=%d, height=%d"
#define MMIDC_PHOTODATE_604_112_2_18_2_9_38_512 "[MMIDC] GetPhotoDateLayerInfo ret=%d,id=%d"
#define MMIDC_PHOTODATE_669_112_2_18_2_9_39_513 "[MMIDC] MMIDC_DestoryPhotoDateOSDLayer id=%d,block_id=%d"
#define MMIDC_PHOTODATE_701_112_2_18_2_9_39_514 "[MMIDC] CreateOSDLayer id=%d,block_id=%d"
#define MMIDC_PHOTODATE_723_112_2_18_2_9_39_515 "[MMIDC] CreateOSDLayer ret=%d"
#define MMIDC_PHOTODATE_745_112_2_18_2_9_39_516 "[MMIDC] CreateOSDLayer block_id=%d, buf_ptr=0x%X"
#define MMIDC_PHOTODATE_750_112_2_18_2_9_39_517 "[MMIDC] CreateOSDLayer width=%d, height=%d"
#define MMIDC_PHOTODATE_756_112_2_18_2_9_39_518 "[MMIDC] CreateOSDLayer result=%d"
#define MMIDC_PHOTODATE_808_112_2_18_2_9_39_519 "[MMIDC] CreatePhotoDateLayers ret=%d"
#define MMIDC_PHOTODATE_828_112_2_18_2_9_39_520 "[MMIDC] MMIDC_CreatePhotoDateLayers ret=%d"
#define MMIDC_PHOTODATE_888_112_2_18_2_9_39_521 "[MMIDC] GetDisplayOSDLayerInfo ret=%d, width=%d, height=%d"
#define MMIDC_PHOTODATE_899_112_2_18_2_9_39_522 "[MMIDC] UpdatePhotoDateCallback"
#define MMIDC_PHOTODATE_918_112_2_18_2_9_39_523 "[MMIDC] MMIDC_CreatePhotoDateData s_photo_date_info_ptr=0x%lX"
#define MMIDC_PHOTODATE_925_112_2_18_2_9_39_524 "[MMIDC] MMIDC_CreatePhotoDateData ALLOC s_photo_date_info_ptr=0x%lX"
#define MMIDC_PHOTODATE_941_112_2_18_2_9_39_525 "[MMIDC] MMIDC_CreatePhotoDateData 2 s_photo_date_info_ptr=0x%lX"
#define MMIDC_PHOTODATE_953_112_2_18_2_9_39_526 "[MMIDC] MMIDC_ReleasePhotoDateData s_photo_date_info_ptr=0x%lX"
#define MMIDC_PHOTODATE_1039_112_2_18_2_9_39_527 "[MMIDC] GetPhotoDateParameter is_enable=%d"
#define MMIDC_SAVE_82_112_2_18_2_9_40_528 "[MMIDC] MMIDC_NewSaveData s_save_data_ptr is PNULL !!!"
#define MMIDC_SAVE_95_112_2_18_2_9_40_529 "[MMIDC]: MMIDC_DeleteSaveData"
#define MMIDC_SAVE_136_112_2_18_2_9_40_530 "[MMIDC] MMIDC_SendByMMS file_type=%d"
#define MMIDC_SAVE_239_112_2_18_2_9_40_531 "[MMIDC] MMIDC_SendByBt Path = PNULL"
#define MMIDC_SAVE_330_112_2_18_2_9_40_532 "[MMIDC] MMIDC_SendByBt file_size=%d"
#define MMIDC_SAVE_375_112_2_18_2_9_40_533 "MMIDC_SetCurrentPhotoAsWallpaper is_success =%d"
#define MMIDC_SAVE_406_112_2_18_2_9_40_534 "[MMIDC] CheckDevRoomSufficient ret=%d"
#define MMIDC_SAVE_426_112_2_18_2_9_40_535 "[MMIDC]: RealCreateOnePicName photo_index=%d, file_dev=%d"
#define MMIDC_SAVE_447_112_2_18_2_9_40_536 "[MMIDC] RealCreateOnePicName create file ret=%d"
#define MMIDC_SAVE_486_112_2_18_2_9_41_537 "[MMIDC] CreateOnePicName %d"
#define MMIDC_SAVE_489_112_2_18_2_9_41_538 "[MMIDC] CreateOnePicName dc infor = PNULL"
#define MMIDC_SAVE_495_112_2_18_2_9_41_539 "[MMIDC] CreateOnePicName dc infor = PNULL"
#define MMIDC_SAVE_514_112_2_18_2_9_41_540 "[MMIDC] CreateOnePicName ret=%d, user_sel_dev=%d,suitable_dev=%d"
#define MMIDC_SAVE_554_112_2_18_2_9_41_541 "[MMIDC]: CreateOnePicName success"
#define MMIDC_SAVE_563_112_2_18_2_9_41_542 "[MMIDC]: CreateOnePicName fail"
#define MMIDC_SAVE_590_112_2_18_2_9_41_543 "[MMIDC]: CreateOnePicName i=%d"
#define MMIDC_SAVE_593_112_2_18_2_9_41_544 "[MMIDC]: CreateOnePicName count_dev_num=%d, old_file_dev=%d,cur_file_dev=%d"
#define MMIDC_SAVE_595_112_2_18_2_9_41_545 "[MMIDC]: CreateOnePicName ret=%d"
#define MMIDC_SAVE_616_112_2_18_2_9_41_546 "[MMIDC]: WriteToFile success"
#define MMIDC_SAVE_625_112_2_18_2_9_41_547 "[MMIDC]: WriteToFile file_ret=%d, write_ret=%d"
#define MMIDC_SAVE_641_112_2_18_2_9_41_548 "[MMIDC]MMIDC_SavePhotos ret=%d, photo_id=%d"
#define MMIDC_SAVE_657_112_2_18_2_9_41_549 "[MMIDC]: GetVideoSubfix DV_VIDEOEXT_MP4 -"
#define MMIDC_SAVE_662_112_2_18_2_9_41_550 "[MMIDC]: GetVideoSubfix DV_VIDEOEXT_3GP -"
#define MMIDC_SAVE_667_112_2_18_2_9_41_551 "[MMIDC]: GetVideoSubfix VIDEO_FILE_TYPE_MJPEG -"
#define MMIDC_SAVE_673_112_2_18_2_9_41_552 "[MMIDC] GetVideoSubfix TYPE"
#define MMIDC_SAVE_703_112_2_18_2_9_41_553 "[MMIDC] CheckVideoSavePath is_folder_exist=%d, ret=%d"
#define MMIDC_SAVE_735_112_2_18_2_9_41_554 "[MMIDC] CreateVideoName subfix = PNULL"
#define MMIDC_SAVE_740_112_2_18_2_9_41_555 "[MMIDC] CreateVideoName path = PNULL"
#define MMIDC_SAVE_744_112_2_18_2_9_41_556 "[MMIDC]: CreateVideoName"
#define MMIDC_SAVE_747_112_2_18_2_9_41_557 "[MMIDC] CreateVideoName ucs2_name = PNULL"
#define MMIDC_SAVE_756_112_2_18_2_9_41_558 "[MMIDC] UDISK MEM NO DISK"
#define MMIDC_SAVE_772_112_2_18_2_9_41_559 "[MMIDC] CreateVideoName DV error_ret=%d, user_sel_dev=%d,suitable_dev=%d"
#define MMIDC_SAVE_873_112_2_18_2_9_41_560 "[MMIDC] CreateVideoName user_sel_dev=%d, old_sel_dev=%d, i =%d"
#define MMIDC_SAVE_876_112_2_18_2_9_41_561 "[MMIDC] CreateVideoName error_ret=%d, count_dev_num=%d"
#define MMIDC_SAVE_902_112_2_18_2_9_41_562 "[MMIDC]: MMIDC_CreateVideoName +"
#define MMIDC_SAVE_908_112_2_18_2_9_41_563 "[MMIDC]: MMIDC_CreateVideoName - create dv name fail!"
#define MMIDC_SAVE_916_112_2_18_2_9_41_564 "[MMIDC]: MMIDC_CreateVideoName -"
#define MMIDC_SAVE_926_112_2_18_2_9_42_565 "[MMIDC]: MMIDC_DeleteNoSavedFile +-"
#define MMIDC_SAVE_1024_112_2_18_2_9_42_566 "[MMIDC] MMIDC_OpenPictureWin path = PNULL"
#define MMIDC_SAVE_1048_112_2_18_2_9_42_567 "[MMIDC] MMIDC_OpenMovieWin path = PNULL"
#define MMIDC_SAVE_1053_112_2_18_2_9_42_568 "[MMIDC] MMIDC_OpenMovieWin name = PNULL"
#define MMIDC_SAVE_1091_112_2_18_2_9_42_569 "[MMIDC] MMIAPIDC_GetSavePhotoIndex array = PNULL"
#define MMIDC_SAVE_1178_112_2_18_2_9_42_570 "[MMIDC] MMIDC_GetCurrentPhotoInfo is_enable=%d"
#define MMIDC_SAVE_1280_112_2_18_2_9_42_571 "[MMIDC]: MMIDC_SaveImageFile"
#define MMIDC_SAVE_1320_112_2_18_2_9_42_572 "[MMIDC]: MMIDC_SaveImageFile ret=%d"
#define MMIDC_SAVE_1337_112_2_18_2_9_42_573 "[MMIDC] MMIDC_SetMaxPhotoFileSize file_size=%d"
#define MMIDC_SAVE_1354_112_2_18_2_9_42_574 "[MMIDC] MMIDC_GetMaxPhotoFileSize file_size=%d"
#define MMIDC_SAVE_1382_112_2_18_2_9_42_575 "[MMIDC]: GeneratePhotoFileName %d"
#define MMIDC_SAVE_1387_112_2_18_2_9_42_576 "[MMIDC] GeneratePhotoFileName name info = PNULL"
#define MMIDC_SAVE_1468_112_2_18_2_9_43_577 "[MMIFILE] SetAttr NO memory !"
#define MMIDC_SAVE_1531_112_2_18_2_9_43_578 "[MMIDC]: MMIDC_SetPhotoFileHidden file_ret=%d,is_enable=%d"
#define MMIDC_SAVE_1593_112_2_18_2_9_43_579 "[MMIDC]: MMIDC_RenamePhotoFile photo_index=%d"
#define MMIDC_SAVE_1608_112_2_18_2_9_43_580 "[MMIDC]: MMIDC_RenamePhotoFile ret=%d,result=%d,is_enable=%d"
#define MMIDC_SAVE_1702_112_2_18_2_9_43_581 "[MMIDC] MMIDC_SetMaxImageFileSize file_size=%d"
#define MMIDC_SAVE_1721_112_2_18_2_9_43_582 "[MMIDC] MMIDC_GetMaxImageFileSize file_size=%d"
#define MMIDC_SAVE_1737_112_2_18_2_9_43_583 "[MMIDC] MMIDC_SetTempFileNameID name_id=%d"
#define MMIDC_SAVE_1754_112_2_18_2_9_43_584 "[MMIDC] MMIDC_GetTempFileNameID name_id=%d"
#define MMIDC_SAVE_1805_112_2_18_2_9_43_585 "[MMIDC] MMIDC_DeleteImageFile ret=%d"
#define MMIDC_SAVE_1825_112_2_18_2_9_43_586 "[MMIDC] MMIDC_DeleteAllImageFile"
#define MMIDC_SAVE_1841_112_2_18_2_9_43_587 "[MMIDC] MMIDC_DeleteAllDiskImageFile"
#define MMIDC_SAVE_1872_112_2_18_2_9_44_588 "[MMIDC] MMIDC_DeleteAllSavedImageFile ret=%d,index=%d"
#define MMIDC_SAVE_1892_112_2_18_2_9_44_589 "[MMIDC] MMIDC_DeleteAllSavedImageFile ret=%d"
#define MMIDC_SETTING_167_112_2_18_2_9_44_590 "[MMIDC]: MMIAPIDC_AllocSettingMemory s_setting_data_ptr = 0x%x"
#define MMIDC_SETTING_176_112_2_18_2_9_44_591 "[MMIDC]: MMIAPIDC_FreeSettingMemory s_setting_data_ptr = 0x%x"
#define MMIDC_SETTING_224_112_2_18_2_9_44_592 "[MMIDC]: MMIDC_SaveSettings is_save=%d"
#define MMIDC_SETTING_342_112_2_18_2_9_45_593 "[MMIDC] InitSizeArray PHOTO_SIZE_MAX != total_num !!!"
#define MMIDC_SETTING_350_112_2_18_2_9_45_594 "[MMIDC] InitSizeArray VIDEO_SIZE_MAX != total_num !!!"
#define MMIDC_SETTING_410_112_2_18_2_9_45_595 "[MMIDC] CheckVideoSize file_type = %d, video_size=%d"
#define MMIDC_SETTING_462_112_2_18_2_9_45_596 "[MMIDC] CheckVideoSize video_size = %d"
#define MMIDC_SETTING_479_112_2_18_2_9_45_597 "[MMIDC] InitVideoSizeToDefault video_size = %d"
#define MMIDC_SETTING_488_112_2_18_2_9_45_598 "[MMIDC]: MMIDC_InitNVSettingDefaultValue"
#define MMIDC_SETTING_518_112_2_18_2_9_45_599 "[MMIDC] MMIDC_InitNVSettingDefaultValue ,screen_mode=%d"
#define MMIDC_SETTING_591_112_2_18_2_9_45_600 "[MMIDC]: MMIDC_Setting_GetNVDefaultValue"
#define MMIDC_SETTING_636_112_2_18_2_9_45_601 "[MMIDC] MMIDC_Setting_InitDefaultValue ret = %d"
#define MMIDC_SETTING_665_112_2_18_2_9_45_602 "[MMIDC]: MMIDC_Setting_GetNVDefaultValue ret=%d"
#define MMIDC_SETTING_678_112_2_18_2_9_45_603 "[MMIDC]: MMIDC_Setting_InitDefaultValue"
#define MMIDC_SETTING_712_112_2_18_2_9_46_604 "[MMIDC]: MMIAPIDC_Setting_SetFactoryValue"
#define MMIDC_SETTING_775_112_2_18_2_9_46_605 "[MMIDC] MMIAPIDC_Setting_SetFactoryValue sensor_angle 1 = %d,screen_mode=%d"
#define MMIDC_SETTING_823_112_2_18_2_9_46_606 "[MMIDC]: MMIDC_CloseCallbackTimer"
#define MMIDC_SETTING_847_112_2_18_2_9_46_607 "[MMIDC]: ParamCtrlCallBack"
#define MMIDC_SETTING_933_112_2_18_2_9_46_608 "[MMIDC] MMIDC_SetScreenMode mode"
#define MMIDC_SETTING_966_112_2_18_2_9_46_609 "[MMIDC] MMIAPIDC_SetPhotoSize size = %d"
#define MMIDC_SETTING_977_112_2_18_2_9_46_610 "[MMIDC] MMIAPIDC_SetPhotoSize size = %d"
#define MMIDC_SETTING_1002_112_2_18_2_9_46_611 "[MMIDC] MMIAPIDC_SetPhotoSize ret = %d"
#define MMIDC_SETTING_1065_112_2_18_2_9_46_612 "[MMIDC] MMIAPIDC_SetPhotoSize shoot_enum = %d"
#define MMIDC_SETTING_1086_112_2_18_2_9_46_613 "[MMIDC] MMIDC_SetFrameIndex frame"
#define MMIDC_SETTING_1098_112_2_18_2_9_46_614 "[MMIDC] MMIDC_SetFrameIndex frame=%d"
#define MMIDC_SETTING_1136_112_2_18_2_9_46_615 "[MMIDC] MMIDC_SetFrameIndex is_preview=%d"
#define MMIDC_SETTING_1156_112_2_18_2_9_46_616 "[MMIDC] MMIDC_SetShootMode mode"
#define MMIDC_SETTING_1197_112_2_18_2_9_46_617 "[MMIDC] MMIDC_SetMultiShootEnum shoot=%d"
#define MMIDC_SETTING_1219_112_2_18_2_9_47_618 "[MMIDC] MMIDC_SetMultiShootEnum photo_size=%d"
#define MMIDC_SETTING_1239_112_2_18_2_9_47_619 "[MMIDC] MMIDC_SetPhotoStorageDevice dev"
#define MMIDC_SETTING_1270_112_2_18_2_9_47_620 "[MMIDC] MMIDC_SetPhotoQuality quality"
#define MMIDC_SETTING_1293_112_2_18_2_9_47_621 "[MMIDC] MMIDC_SetAutoSave promt"
#define MMIDC_SETTING_1326_112_2_18_2_9_47_622 "[MMIDC] MMIDC_SetRecordFileType type"
#define MMIDC_SETTING_1364_112_2_18_2_9_47_623 "[MMIDC] MMIDC_SetVideoSize size"
#define MMIDC_SETTING_1390_112_2_18_2_9_47_624 "[MMIDC] MMIDC_SetVideoSize size=%d"
#define MMIDC_SETTING_1410_112_2_18_2_9_47_625 "[MMIDC] MMIDC_SetVideoStorageDevice dev"
#define MMIDC_SETTING_1448_112_2_18_2_9_47_626 "[MMIDC]: MMIDC_UpdatePhotoZoomValue modify multi shot!"
#define MMIDC_SETTING_1460_112_2_18_2_9_47_627 "[MMIDC]: MMIDC_SetDCZoomValue return: %d, %d"
#define MMIDC_SETTING_1492_112_2_18_2_9_47_628 "[MMIDC] MMIDC_SetShootScene scene"
#define MMIDC_SETTING_1533_112_2_18_2_9_47_629 "[MMIDC] MMIDC_SetShootScene 0"
#define MMIDC_SETTING_1548_112_2_18_2_9_47_630 "[MMIDC] MMIDC_SetFlashLamp flash"
#define MMIDC_SETTING_1572_112_2_18_2_9_47_631 "[MMIDC] MMIDC_SetExposalCompensate exposal_compensate"
#define MMIDC_SETTING_1588_112_2_18_2_9_47_632 "[MMIDC] MMIDC_SetExposalCompensate setting count > 0 !!!"
#define MMIDC_SETTING_1597_112_2_18_2_9_47_633 "[MMIDC]: MMIDC_SetExposalCompensate compentsate return %d, %d"
#define MMIDC_SETTING_1629_112_2_18_2_9_47_634 "[MMIDC] MMIDC_SetSelfShootDelayTime time"
#define MMIDC_SETTING_1752_112_2_18_2_9_48_635 "[MMIDC] MMIDC_SetPhotoEffect effect"
#define MMIDC_SETTING_1768_112_2_18_2_9_48_636 "[MMIDC] MMIDC_SetPhotoEffect setting count > 0 !!!"
#define MMIDC_SETTING_1778_112_2_18_2_9_48_637 "[MMIDC]: MMIDC_SetPhotoEffect return: %d, effect = %d"
#define MMIDC_SETTING_1822_112_2_18_2_9_48_638 "[MMIDC] MMIDC_SetVideoEffect effect"
#define MMIDC_SETTING_1838_112_2_18_2_9_48_639 "[MMIDC] MMIDC_SetVideoEffect setting count > 0 !!!"
#define MMIDC_SETTING_1848_112_2_18_2_9_48_640 "[MMIDC]: MMIDC_SetVideoEffect return: %d, %d"
#define MMIDC_SETTING_1871_112_2_18_2_9_48_641 "[MMIDC] MMIDC_SetShutterVoice voice"
#define MMIDC_SETTING_1905_112_2_18_2_9_48_642 "[MMIDC] MMIDC_SetPhotoEnvironment environment"
#define MMIDC_SETTING_1921_112_2_18_2_9_48_643 "[MMIDC] MMIDC_SetPhotoEnvironment setting count > 0 !!!"
#define MMIDC_SETTING_1942_112_2_18_2_9_48_644 "[MMIDC]: SetphotoEnvironment return %d, %d"
#define MMIDC_SETTING_1977_112_2_18_2_9_48_645 "[MMIDC] MMIDC_SetVideoEnvironment environment"
#define MMIDC_SETTING_1993_112_2_18_2_9_48_646 "[MMIDC] MMIDC_SetVideoEnvironment setting count > 0 !!!"
#define MMIDC_SETTING_2015_112_2_18_2_9_48_647 "[MMIDC]: SetvideoEnvironment return %d, %d"
#define MMIDC_SETTING_2049_112_2_18_2_9_48_648 "[MMIDC] MMIDC_SetContrast contrast"
#define MMIDC_SETTING_2065_112_2_18_2_9_48_649 "[MMIDC] MMIDC_SetContrast setting count > 0 !!!"
#define MMIDC_SETTING_2074_112_2_18_2_9_48_650 "[MMIDC]: SetphotoContrast return %d, %d"
#define MMIDC_SETTING_2099_112_2_18_2_9_48_651 "[MMIDC] MMIDC_SetVideoContrast contrast"
#define MMIDC_SETTING_2115_112_2_18_2_9_48_652 "[MMIDC] MMIDC_SetVideoContrast setting count > 0 !!!"
#define MMIDC_SETTING_2124_112_2_18_2_9_48_653 "[MMIDC]: MMIDC_SetVideoContrast return %d, %d"
#define MMIDC_SETTING_2176_112_2_18_2_9_48_654 "[MMIDC] MMIDC_SetPhotoFlicker cor"
#define MMIDC_SETTING_2192_112_2_18_2_9_48_655 "[MMIDC] MMIDC_SetPhotoFlicker setting count > 0 !!!"
#define MMIDC_SETTING_2200_112_2_18_2_9_48_656 "[MMIDC]: MMIDC_SetPhotoFlicker return %d, %d"
#define MMIDC_SETTING_2234_112_2_18_2_9_49_657 "[MMIDC] MMIDC_SetVideoFlicker cor"
#define MMIDC_SETTING_2250_112_2_18_2_9_49_658 "[MMIDC] MMIDC_SetVideoFlicker setting count > 0 !!!"
#define MMIDC_SETTING_2258_112_2_18_2_9_49_659 "[MMIDC]: MMIDC_SetVideoFlicker return %d, %d"
#define MMIDC_SETTING_2290_112_2_18_2_9_49_660 "[MMIDC] MMIDC_SetISO iso"
#define MMIDC_SETTING_2324_112_2_18_2_9_49_661 "[MMIDC] MMIDC_SetPhotoWhiteBalance setting count > 0 !!!"
#define MMIDC_SETTING_2333_112_2_18_2_9_49_662 "[MMIDC]: SetphotoWhiteBalance return %d, %d"
#define MMIDC_SETTING_2365_112_2_18_2_9_49_663 "[MMIDC] MMIDC_SetCameraMode mode"
#define MMIDC_SETTING_2386_112_2_18_2_9_49_664 "[MMIDC]: SetVideoZoomValue return %d, %d"
#define MMIDC_SETTING_2406_112_2_18_2_9_49_665 "[MMIDC]: SetvideoWhiteBalance return %d"
#define MMIDC_SETTING_2429_112_2_18_2_9_49_666 "[MMIDC] MMIDC_SetPhotoBrightness photo_brightness"
#define MMIDC_SETTING_2445_112_2_18_2_9_49_667 "[MMIDC] MMIDC_SetPhotoBrightness setting count > 0 !!!"
#define MMIDC_SETTING_2454_112_2_18_2_9_49_668 "[MMIDC]: MMIDC_SetPhotoBrightness return %d, %d"
#define MMIDC_SETTING_2488_112_2_18_2_9_49_669 "[MMIDC] MMIDC_SetVideoBrightness video_brightness"
#define MMIDC_SETTING_2503_112_2_18_2_9_49_670 "[MMIDC] MMIDC_SetVideoBrightness setting count > 0 !!!"
#define MMIDC_SETTING_2512_112_2_18_2_9_49_671 "[MMIDC]: MMIDC_SetvideoBrightness return: %d, %d"
#define MMIDC_SETTING_2579_112_2_18_2_9_49_672 "[MMIDC] MMIDC_IncreaseZoomValue U CAMERA NO SUPPORT"
#define MMIDC_SETTING_2610_112_2_18_2_9_49_673 "[MMIDC] MMIDC_IncreaseZoomValue U CAMERA NO SUPPORT"
#define MMIDC_SETTING_2707_112_2_18_2_9_49_674 "[MMIDC]: MMIDC_IsSettingParamCtrl %d"
#define MMIDC_SETTING_3109_112_2_18_2_9_50_675 "[MMIDC] MMIDC_InitInfo platform_angle = %d"
#define MMIDC_SETTING_3110_112_2_18_2_9_50_676 "[MMIDC] MMIDC_InitInfo lcd_size = %d"
#define MMIDC_SETTING_3112_112_2_18_2_9_50_677 "[MMIDC] MMIDC_InitInfo physical width = %d, height = %d"
#define MMIDC_SETTING_3194_112_2_18_2_9_50_678 "[MMIDC] lcd_id = %d, width = %d, height = %d"
#define MMIDC_SETTING_3287_112_2_18_2_9_51_679 "[MMIDC] MMIDC_GetMaxZoomLevel zoom_max = 0x%04lX"
#define MMIDC_SETTING_3292_112_2_18_2_9_51_680 "[MMIDC] MMIDC_GetMaxZoomLevel FAIL"
#define MMIDC_SETTING_3296_112_2_18_2_9_51_681 "[MMIDC] MMIDC_GetMaxZoomLevel out zoom_max = %d"
#define MMIDC_SETTING_3334_112_2_18_2_9_51_682 "[MMIDC] MMIDC_GetPhysicalLCDWidthAndHeight logic_angle =%d"
#define MMIDC_SETTING_3336_112_2_18_2_9_51_683 "[MMIDC] width = %d,height = %d"
#define MMIDC_SETTING_3338_112_2_18_2_9_51_684 "[MMIDC] rect_right = %d,rect_bottom = %d"
#define MMIDC_SETTING_3346_112_2_18_2_9_51_685 "[MMIDC] out rect_right = %d,rect_bottom = %d"
#define MMIDC_SETTING_3347_112_2_18_2_9_51_686 "[MMIDC] out angle = %d"
#define MMIDC_SETTING_3356_112_2_18_2_9_51_687 "[MMIDC] *angle = %d"
#define MMIDC_SETTING_3419_112_2_18_2_9_51_688 "[MMIDC] MMIAPIDC_GetDCModuleSensorAngle angle=%d,id=%d"
#define MMIDC_SETTING_3435_112_2_18_2_9_51_689 "[MMIDC] MMIAPIDC_SetDCModuleSensorAngle angle=%d,id=%d"
#define MMIDC_SETTING_3463_112_2_18_2_9_51_690 "[MMIDC] MMIDC_IsSilentMode mode=%d"
#define MMIDC_SETTING_3516_112_2_18_2_9_51_691 "[MMIDC] MMIDC_ConvertDCameraSizeToPhotoSize total_item !!!"
#define MMIDC_SETTING_3529_112_2_18_2_9_51_692 "[MMIDC] MMIDC_ConvertDCameraSizeToPhotoSize photo_size = %d,dcamera_size=%d"
#define MMIDC_SETTING_3565_112_2_18_2_9_51_693 "[MMIDC] MMIDC_ConvertPhotoSizeToDCameraSize total_item !!!"
#define MMIDC_SETTING_3579_112_2_18_2_9_51_694 "[MMIDC] MMIDC_ConvertPhotoSizeToDCameraSize photo_size = %d,dcamera_size= %d"
#define MMIDC_SETTING_3603_112_2_18_2_9_51_695 "[MMIDC] MMIDC_GetSupportSizeArray size_bit = 0x%08lX"
#define MMIDC_SETTING_3696_112_2_18_2_9_51_696 "[MMIDC] MMIDC_SetDCameraSensorID sensor_id = %d"
#define MMIDC_SETTING_3729_112_2_18_2_9_51_697 "[MMIDC] MMIDC_SetDCameraSensorID ret = %d"
#define MMIDC_SETTING_3743_112_2_18_2_9_52_698 "[MMIDC] MMIDC_SetDCameraSensorID sensor_id = %d"
#define MMIDC_SETTING_3762_112_2_18_2_9_52_699 "[MMIDC] MMIDC_GetDCameraSensorNumber FAIL"
#define MMIDC_SETTING_3766_112_2_18_2_9_52_700 "[MMIDC] MMIDC_GetDCameraSensorNumber out sensor_number = %d"
#define MMIDC_SETTING_3805_112_2_18_2_9_52_701 "[MMIDC] MMIDC_GetBurstNumberByPhotoSize num = %d"
#define MMIDC_SETTING_3843_112_2_18_2_9_52_702 "[MMIDC]: MMIDC_InitParamCtrl"
#define MMIDC_SETTING_3924_112_2_18_2_9_52_703 "[MMIDC] MMIAPIDC_GetDCModulSensorType sensor_type=%d"
#define MMIDC_SETTING_3950_112_2_18_2_9_52_704 "[MMIDC] MMIAPIDC_GetDCModulSensorType sensor_type 2=%d"
#define MMIDC_SETTING_3968_112_2_18_2_9_52_705 "[MMIDC] MMIDC_SetBackupFrameIndex frame_index=%d"
#define MMIDC_SETTING_4020_112_2_18_2_9_52_706 "[MMIDC] MMIDC_SetNVShutterVoice voice"
#define MMIDC_SETTING_4024_112_2_18_2_9_52_707 "[MMIDC] MMIDC_SetNVShutterVoice voice=%d"
#define MMIDC_SETTING_4090_112_2_18_2_9_52_708 "[MMIDC] MMIDC_GetThumbnailPhotoSize photo_size=%d"
#define MMIDC_SETTING_4107_112_2_18_2_9_52_709 "[MMIDC] MMIAPIDC_GetDCModuleSensorNumber sensor_number=%d"
#define MMIDC_SETTING_4133_112_2_18_2_9_52_710 "[MMIDC] MMIAPIDC_GetDCModuleSensorNumber sensor_number 2=%d"
#define MMIDC_SETTING_4150_112_2_18_2_9_52_711 "[MMIDC] MMIDC_GetSoftwareRotationAngle angle=%d"
#define MMIDC_SETTING_4164_112_2_18_2_9_52_712 "[MMIDC] MMIDC_SetSoftwareRotationAngle angle=%d"
#define MMIDC_SETTING_4186_112_2_18_2_9_52_713 "[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle dc_ret=%d"
#define MMIDC_SETTING_4196_112_2_18_2_9_52_714 "[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle dv_ret=%d"
#define MMIDC_SETTING_4205_112_2_18_2_9_52_715 "[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle ret=%d"
#define MMIDC_SETTING_4226_112_2_18_2_9_52_716 "[MMIDC] MMIDC_GetDVMaxZoomLevel FAIL ret=%d"
#define MMIDC_SETTING_4230_112_2_18_2_9_52_717 "[MMIDC] MMIDC_GetDVMaxZoomLevel out zoom_max = %d"
#define MMIDC_SETTING_4272_112_2_18_2_9_53_718 "[MMIDC]: MMIDC_UpdateDVZoomValue ret=%d, zoom_param=%d"
#define MMIDC_SETTING_4273_112_2_18_2_9_53_719 "[MMIDC]: MMIDC_UpdateDVZoomValue zoom=%d"
#define MMIDC_SETTING_4318_112_2_18_2_9_53_720 "[MMIDC] MMIDC_IncreaseDVZoomValue ret=%d"
#define MMIDC_SETTING_4340_112_2_18_2_9_53_721 "[MMIDC] MMIDC_DecreaseDVZoomValue ret=%d"
#define MMIDC_SETTING_4365_112_2_18_2_9_53_722 "[MMIDC] MMIDC_GetScreenPhotoSize screen_size=%d"
#define MMIDC_SETTING_4414_112_2_18_2_9_53_723 "[MMIDC] MMIDC_GetMaxVideoSize max_video_size= %d, file_type=%d"
#define MMIDC_SETTING_4442_112_2_18_2_9_53_724 "[MMIDC] MMIDC_GetMaxVideoSize min_video_size= %d"
#define MMIDC_SETTING_4533_112_2_18_2_9_53_725 "[MMIDC] MMIDC_GetSupportVideoSizeArray copy_count=%d, total_item=%d, video_size_ptr=%d"
#define MMIDC_SETTING_4570_112_2_18_2_9_53_726 "[MMIDC] MMIDC_GetDataFormatInfo data_format=%d, data_unit=%d"
#define MMIDC_SETTING_4626_112_2_18_2_9_53_727 "[MMIDC] MMIDC_SetMMIBrushStatus status=%d"
#define MMIDC_SETTING_4641_112_2_18_2_9_53_728 "[MMIDC] MMIDC_GetMMIBrushStatus status=%d"
#define MMIDC_SETTING_4662_112_2_18_2_9_53_729 "[MMIDC] MMIDC_IsMMIBrushing ret=%d"
#define MMIDC_SETTING_4676_112_2_18_2_9_53_730 "[MMIDC] MMIDC_SetMMIFocusStatus status=%d"
#define MMIDC_SETTING_4691_112_2_18_2_9_53_731 "[MMIDC] MMIDC_GetMMIFocusStatus status=%d"
#define MMIDC_SETTING_4705_112_2_18_2_9_53_732 "[MMIDC] MMIDC_PushPhotoSize photo_size=%d"
#define MMIDC_SETTING_4720_112_2_18_2_9_53_733 "[MMIDC] MMIDC_PullPhotoSize photo_size=%d"
#define MMIDC_SETTING_4734_112_2_18_2_9_53_734 "[MMIDC] MMIDC_PushVisualMode visual_mode=%d"
#define MMIDC_SETTING_4749_112_2_18_2_9_53_735 "[MMIDC] MMIDC_PullVisualMode visual_mode=%d"
#define MMIDC_SMILEAUTOPHOTO_86_112_2_18_2_9_54_736 "[MMIDC] GetSmileAutoPhotoHandle handle=%d"
#define MMIDC_SMILEAUTOPHOTO_113_112_2_18_2_9_54_737 "[MMIDC] CreateSmileAutoPhotoHandle ret=%d,handle=%d"
#define MMIDC_SMILEAUTOPHOTO_137_112_2_18_2_9_54_738 "[MMIDC] DestroySmileAutoPhotoHandle ret=%d"
#define MMIDC_SMILEAUTOPHOTO_171_112_2_18_2_9_54_739 "[MMIDC] MMIDC_ClearSmileAutoPhotoOSDLayer block_id=%d"
#define MMIDC_SMILEAUTOPHOTO_175_112_2_18_2_9_54_740 "[MMIDC] MMIDC_ClearSmileAutoPhotoOSDLayer layer clear"
#define MMIDC_SMILEAUTOPHOTO_231_112_2_18_2_9_54_741 "[MMIDC] MMIDC_CreateSmileAutoPhotoOSDLayer width=%d, height=%d"
#define MMIDC_SMILEAUTOPHOTO_267_112_2_18_2_9_54_742 "[MMIDC] MMIDC_CreateSmileAutoPhotoOSDLayer result=%d,dev_info.block_id=%d"
#define MMIDC_SMILEAUTOPHOTO_329_112_2_18_2_9_54_743 "[MMIDC] MMIDC_OpenSmileAutoPhoto ret=%d"
#define MMIDC_SMILEAUTOPHOTO_363_112_2_18_2_9_55_744 "[MMIDC] MMIDC_IsSmileAutoPhotoEnable is_enable=%d"
#define MMIDC_SMILEAUTOPHOTO_475_112_2_18_2_9_55_745 "[MMIDC] ConvertVerRectToHorRect lcd_height = %d"
#define MMIDC_SMILEAUTOPHOTO_513_112_2_18_2_9_55_746 "[MMIDC] DisplaySmileAutoPhotoOsdRect pram 0 num = %d"
#define MMIDC_SMILEAUTOPHOTO_518_112_2_18_2_9_55_747 "[MMIDC] DisplaySmileAutoPhotoOsdRect dect_pram_ptr->num == 0,return "
#define MMIDC_SMILEAUTOPHOTO_527_112_2_18_2_9_55_748 "[MMIDC] DisplaySmileAutoPhotoOsdRect dc_disp_rect x = %d,y = %d,w = %d,h = %d"
#define MMIDC_SMILEAUTOPHOTO_533_112_2_18_2_9_55_749 "[MMIDC] DisplaySmileAutoPhotoOsdRect pram i=%d,x = %d,y = %d,w = %d,h = %d"
#define MMIDC_SMILEAUTOPHOTO_553_112_2_18_2_9_55_750 "[MMIDC] DisplaySmileAutoPhotoOsdRect osd rect  left = %d,top = %d,right = %d,bottom = %d"
#define MMIDC_SMILEAUTOPHOTO_558_112_2_18_2_9_55_751 "[MMIDC] DisplaySmileAutoPhotoOsdRect SCI_SUCCESS! draw Rect "
#define MMIDC_SMILEAUTOPHOTO_573_112_2_18_2_9_55_752 "[MMIDC] DisplaySmileAutoPhotoOsdRect  Draw Rect "
#define MMIDC_SMILEAUTOPHOTO_621_112_2_18_2_9_55_753 "[MMIDC] IsSmileAutoPhotoAllSmile result = %d"
#define MMIDC_SMILEAUTOPHOTO_657_112_2_18_2_9_55_754 "[MMIDC] SmileAutoPhotoImageProcessCallBack"
#define MMIDC_SMILEAUTOPHOTO_757_112_2_18_2_9_55_755 "[MMIDC] MMIDC_GetSmileAutoPhotoImageProcessParam ret=%d"
#define MMIDC_UCAMERA_WINTAB_281_112_2_18_2_9_56_756 "[MMIDC] ProcessIconEffect proc_param.cmd=%d"
#define MMIDC_UCAMERA_WINTAB_282_112_2_18_2_9_56_757 "[MMIDC] ProcessIconEffect proc_param.size.w=%d"
#define MMIDC_UCAMERA_WINTAB_283_112_2_18_2_9_56_758 "[MMIDC] ProcessIconEffect proc_param.size.h=%d"
#define MMIDC_UCAMERA_WINTAB_285_112_2_18_2_9_56_759 "[MMIDC] ProcessIconEffect proc_param.input_addr.chn0=0x%x, proc_param.input_addr.chn0_len = %d"
#define MMIDC_UCAMERA_WINTAB_287_112_2_18_2_9_56_760 "[MMIDC] ProcessIconEffect proc_param.output_addr.chn0=0x%x, proc_param.output_addr.chn0_len = %d"
#define MMIDC_UCAMERA_WINTAB_291_112_2_18_2_9_56_761 "[MMIDC] ProcessIconEffect rtn_ret=%d"
#define MMIDC_UCAMERA_WINTAB_341_112_2_18_2_9_56_762 "[MMIDC] InitIconEffect iconlist_num=%d"
#define MMIDC_UCAMERA_WINTAB_344_112_2_18_2_9_56_763 "InitIconEffect handle=%d"
#define MMIDC_UCAMERA_WINTAB_373_112_2_18_2_9_56_764 "[MMIDC] InitIconEffect src_rgb_ptr is null"
#define MMIDC_UCAMERA_WINTAB_387_112_2_18_2_9_56_765 "[MMIDC] InitIconEffect buffer_ptr is null"
#define MMIDC_UCAMERA_WINTAB_406_112_2_18_2_9_56_766 "[MMIDC] InitIconEffect GUIRES_CopyImgBuf error 2"
#define MMIDC_UCAMERA_WINTAB_422_112_2_18_2_9_56_767 "[MMIDC] InitIconEffect ProcessIconEffect error"
#define MMIDC_UCAMERA_WINTAB_457_112_2_18_2_9_56_768 "[MMIDC] InitIconEffect end"
#define MMIDC_UCAMERA_WINTAB_547_112_2_18_2_9_57_769 "[MMIDC] SelectEffectItem select_type=%d"
#define MMIDC_UCAMERA_WINTAB_611_112_2_18_2_9_57_770 "[MMIDC] SelectEffectItem ret=%d"
#define MMIDC_UCAMERA_WINTAB_663_112_2_18_2_9_57_771 "[MMIDC] HandleUCameraWinMsg InitUCameraWin fail"
#define MMIDC_WINDOW_OPTION_290_112_2_18_2_9_58_772 "[MMIDC]: HandlePhotoReviewOptionWinMsg(), msg_id = %x"
#define MMIDC_WINDOW_OPTION_420_112_2_18_2_9_58_773 "[MMIDC] HandleDcStorageOptWinMsg udisk=%d,sd=%d"
#define MMIDC_WINDOW_OPTION_535_112_2_18_2_9_59_774 "[MMIDC] GetStorageDeviceList count_dev_num=%d"
#define MMIDC_WINDOW_OPTION_588_112_2_18_2_9_59_775 "[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
#define MMIDC_WINDOW_OPTION_672_112_2_18_2_9_59_776 "[MMIDC] HandleDcStorageOptWinMsg OK max_items=%d"
#define MMIDC_WINDOW_OPTION_680_112_2_18_2_9_59_777 "[MMIDC] HandleDcStorageOptWinMsg OK sys_dev=%d,mode=%d,cur_item=%d"
#define MMIDC_WINDOW_OPTION_1030_112_2_18_2_10_0_778 "[MMIDC] AddItemToSetlist max_dev_number=%d, cur_dev_item=%d, dc_dev=%d"
#define MMIDC_WORKMODE_75_112_2_18_2_10_2_779 "[MMIDC] AllocWorkInfo s_dc_work_info_ptr=0x%X"
#define MMIDC_WORKMODE_87_112_2_18_2_10_2_780 "[MMIDC] FreeWorkInfo s_dc_work_info_ptr=0x%X"
#define MMIDC_WORKMODE_144_112_2_18_2_10_2_781 "[MMIDC] InitWorkInfoParam ret=%d"
#define MMIDC_WORKMODE_160_112_2_18_2_10_2_782 "[MMIDC] MMIDC_ReleaseWorkMode"
#define MMIDC_WORKMODE_203_112_2_18_2_10_2_783 "[MMIDC] MMIAPIDC_StartWork ret=%d"
#define MMIDC_WORKMODE_222_112_2_18_2_10_2_784 "[MMIDC] MMIDC_IsWorkMode ret=%d"
#define MMIDC_WORKMODE_350_112_2_18_2_10_3_785 "[MMIDC] ConvertWorkPhotoSizeToDCPhotoSize photo_size=%d"
#define MMIDC_WORKMODE_369_112_2_18_2_10_3_786 "[MMIDC] MMIDC_GetPhotoSizeOfWorkMode photo_size=%d"
#define MMIDC_WORKMODE_469_112_2_18_2_10_3_787 "[MMIDC] ConvertWorkVideoSizeToDCPhotoSize video_size=%d"
#define MMIDC_WORKMODE_488_112_2_18_2_10_3_788 "[MMIDC] MMIDC_GetVideoSizeOfWorkMode video_size=%d"
#define MMIDC_WORKMODE_554_112_2_18_2_10_3_789 "[MMIDC] MMIDC_GetMaxPhotoSizeOfWorkMode max_photo_size=%d"
#define MMIDC_WORKMODE_621_112_2_18_2_10_3_790 "[MMIDC] MMIDC_SaveWorkFile is_succes=%d"
#define MMIDC_WORKMODE_654_112_2_18_2_10_3_791 "[MMIDC] MMIDC_WorkCallback ret=%d"
#define MMIDC_WORKMODE_674_112_2_18_2_10_3_792 "[MMIDC] MMIDC_GetMaxVideoSizeOfWorkMode max_video_size=%d"
#define MMIDC_WORKMODE_694_112_2_18_2_10_3_793 "[MMIDC] MMIDC_GetLimitedVideoFileSizeOfWorkMode file_size=%d"
#define MMIDV_MANAGER_87_112_2_18_2_10_4_794 "[MMIRecord]: OpenDV = %d"
#define MMIDV_MANAGER_91_112_2_18_2_10_4_795 "[MMIRecord]: DRECORDER_SelectSrcDevice = %d"
#define MMIDV_MANAGER_93_112_2_18_2_10_4_796 "[MMIRecord]: DRECORDER_Open = %d"
#define MMIDV_MANAGER_126_112_2_18_2_10_4_797 "[MMIRecord]: StartDVPreview = %d, = %d"
#define MMIDV_MANAGER_133_112_2_18_2_10_4_798 "[MMIRecord]: DRECORDER_StartPreview = %d"
#define MMIDV_MANAGER_161_112_2_18_2_10_4_799 "[MMIRecord]: InitDVPreviewParam record_param_ptr is PNULL !!!"
#define MMIDV_MANAGER_173_112_2_18_2_10_4_800 "[MMIdv_manager]:  cur_angle = %d"
#define MMIDV_MANAGER_179_112_2_18_2_10_4_801 "[MMIdv_manager]:  GUILCD_GetLogicWidthHeight, result = %d"
#define MMIDV_MANAGER_191_112_2_18_2_10_4_802 "[MMIdv_manager]:  GUILCD_ConvertLogicToPhysicalCoordinate, result = %d"
#define MMIDV_MANAGER_209_112_2_18_2_10_4_803 "[MMIdv_manager]:  record_param_ptr->disp_rect :x = %d, y = %d, w = %d, h = %d"
#define MMIDV_MANAGER_216_112_2_18_2_10_4_804 "[MMIdv_manager]: InitDVPreviewParam"
#define MMIDV_MANAGER_229_112_2_18_2_10_4_805 "[MMIRecord]: StopPreview = %d, = %d"
#define MMIDV_MANAGER_234_112_2_18_2_10_4_806 "[MMIRecord]: DRECORDER_StopPreview = %d"
#define MMIDV_MANAGER_252_112_2_18_2_10_4_807 "[MMIRecord]: CloseDV = %d"
#define MMIDV_MANAGER_259_112_2_18_2_10_4_808 "[MMIRecord]: DRECORDER_Close = %d"
#define MMIDV_MANAGER_287_112_2_18_2_10_4_809 "[MMIRecord]: StartRecord start"
#define MMIDV_MANAGER_291_112_2_18_2_10_4_810 "[MMIRecord]: StartRecord start filename is PNULL !!!"
#define MMIDV_MANAGER_326_112_2_18_2_10_4_811 "[MMIRecord]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d"
#define MMIDV_MANAGER_331_112_2_18_2_10_4_812 "[MMIRecord]: DRECORDER_StartRecord = %d"
#define MMIDV_MANAGER_339_112_2_18_2_10_4_813 "[MMIRecord]: StartRecord - return success !"
#define MMIDV_MANAGER_392_112_2_18_2_10_4_814 "[MMIRecord]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d"
#define MMIDV_MANAGER_407_112_2_18_2_10_4_815 "[MMIRecord]: DRECORDER_StopRecord return: %d"
#define MMIDV_MANAGER_437_112_2_18_2_10_4_816 "[MMIRecord]: DRECORDER_StopRecord return: %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_CAMERA_TRC)
TRACE_MSG(MMICAMERA_MANAGER_40_112_2_18_2_8_25_0,"[MMICamera]: DCAMERA_Open error")
TRACE_MSG(MMICAMERA_MANAGER_57_112_2_18_2_8_25_1,"[MMICamera]: OpenDC ret = %d, s_dc_is_open = %d")
TRACE_MSG(MMICAMERA_MANAGER_73_112_2_18_2_8_25_2,"[MMICamera]: StartDCPreview disp w = %d, disp h = %d")
TRACE_MSG(MMICAMERA_MANAGER_74_112_2_18_2_8_25_3,"[MMICamera]: StartDCPreview target w = %d, target h = %d")
TRACE_MSG(MMICAMERA_MANAGER_79_112_2_18_2_8_25_4,"[MMICamera]: DCAMERA_StartPreview ret = %d")
TRACE_MSG(MMICAMERA_MANAGER_84_112_2_18_2_8_25_5,"[MMICamera]: StartDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d")
TRACE_MSG(MMICAMERA_MANAGER_103_112_2_18_2_8_25_6,"[MMICamera]: MMICamera_InitDCPreviewParam enter")
TRACE_MSG(MMICAMERA_MANAGER_107_112_2_18_2_8_25_7,"[MMICamera]:  cur_angle = %d")
TRACE_MSG(MMICAMERA_MANAGER_114_112_2_18_2_8_25_8,"[MMICamera]:  GUILCD_GetLogicWidthHeight, result = %d")
TRACE_MSG(MMICAMERA_MANAGER_125_112_2_18_2_8_25_9,"[MMICamera]:  GUILCD_ConvertLogicToPhysicalCoordinate, result = %d")
TRACE_MSG(MMICAMERA_MANAGER_141_112_2_18_2_8_25_10,"[MMICamera]:  s_preview_param.disp_rect :x = %d, y = %d, w = %d, h = %d")
TRACE_MSG(MMICAMERA_MANAGER_163_112_2_18_2_8_25_11,"[MMICamera]: StopDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d")
TRACE_MSG(MMICAMERA_MANAGER_183_112_2_18_2_8_25_12,"[MMICamera]: CloseDC ret = %d, s_dc_is_open = %d")
TRACE_MSG(MMICAMERA_MANAGER_207_112_2_18_2_8_25_13,"[MMICamera]: SnapShot return: %d")
TRACE_MSG(MMICAMERA_MANAGER_265_112_2_18_2_8_25_14,"[MMICamera]: SetPhotoSize return: %d")
TRACE_MSG(MMIDC_EFFECT360_95_112_2_18_2_8_26_15,"[MMIDC] GetCurEffect360CommonType type=%d")
TRACE_MSG(MMIDC_EFFECT360_129_112_2_18_2_8_26_16,"[MMIDC] IsEffect360CommonType resut=%d")
TRACE_MSG(MMIDC_EFFECT360_177_112_2_18_2_8_26_17,"[MMIDC] GetEffect360CommonHandle handle=%d")
TRACE_MSG(MMIDC_EFFECT360_233_112_2_18_2_8_26_18,"[MMIDC] CreateEffect360CommonHandle ret=%d,handle=%d")
TRACE_MSG(MMIDC_EFFECT360_285_112_2_18_2_8_26_19,"[MMIDC] DestroyEffect360CommonHandle ret=%d")
TRACE_MSG(MMIDC_EFFECT360_332_112_2_18_2_8_26_20,"[MMIDC] CreateCommonPhotoInfo buf_ptr=0x%X, buf_size=%d")
TRACE_MSG(MMIDC_EFFECT360_335_112_2_18_2_8_26_21,"[MMIDC] CreateCommonPhotoInfo ret=%d")
TRACE_MSG(MMIDC_EFFECT360_363_112_2_18_2_8_26_22,"[MMIDC] DestroyCommonPhotoInfo")
TRACE_MSG(MMIDC_EFFECT360_442_112_2_18_2_8_26_23,"[MMIDC] MMIDC_OpenEffect360 ret=%d")
TRACE_MSG(MMIDC_EFFECT360_485_112_2_18_2_8_27_24,"[MMIDC] MMIDC_IsEffect360Enable is_enable=%d")
TRACE_MSG(MMIDC_EFFECT360_509_112_2_18_2_8_27_25,"[MMIDC] Effect360CommonProcessCallBack")
TRACE_MSG(MMIDC_EFFECT360_538_112_2_18_2_8_27_26,"[MMIDC] Effect360CommonProcessCallBack")
TRACE_MSG(MMIDC_EFFECT360_680_112_2_18_2_8_27_27,"[MMIDC] GetEffect360CommonImageProcessParam capture")
TRACE_MSG(MMIDC_EFFECT360_684_112_2_18_2_8_27_28,"[MMIDC] GetEffect360CommonImageProcessParam")
TRACE_MSG(MMIDC_EFFECT360_712_112_2_18_2_8_27_29,"[MMIDC] MMIDC_GetEffect360ImageProcessParam")
TRACE_MSG(MMIDC_EXPORT_243_112_2_18_2_8_28_30,"[MMIDC] MMIAPIDC_GetFileIDString id >= id_max")
TRACE_MSG(MMIDC_EXPORT_270_112_2_18_2_8_28_31,"[MMIDC] MMIAPIDC_CombinePathName path = PNULL")
TRACE_MSG(MMIDC_FLOW_405_112_2_18_2_8_29_32,"[MMIDC] SetReviewOSDCallback")
TRACE_MSG(MMIDC_FLOW_513_112_2_18_2_8_29_33,"[MMIDC] GetDCPreviewParam preview_param_ptr is PNULL !!!")
TRACE_MSG(MMIDC_FLOW_525_112_2_18_2_8_29_34,"[MMIDC] GetDCPreviewParam lcd_width = %d, lcd_height = %d")
TRACE_MSG(MMIDC_FLOW_531_112_2_18_2_8_29_35,"[MMIDC] GetDCPreviewParam rotation = %d, display mode = %d")
TRACE_MSG(MMIDC_FLOW_556_112_2_18_2_8_29_36,"[MMIDC] Display_Callback ret=%d")
TRACE_MSG(MMIDC_FLOW_560_112_2_18_2_8_29_37,"[MMIDC] Display_Callback p_disp_src=0x%X,p_disp_dst=0x%X")
TRACE_MSG(MMIDC_FLOW_572_112_2_18_2_8_29_38,"[MMIDC] Display_Callback is brushing return")
TRACE_MSG(MMIDC_FLOW_582_112_2_18_2_8_29_39,"[MMIDC] Display_Callback is doing return")
TRACE_MSG(MMIDC_FLOW_587_112_2_18_2_8_29_40,"[MMIDC] Display_Callback start")
TRACE_MSG(MMIDC_FLOW_596_112_2_18_2_8_29_41,"[MMIDC Display_Callback src type=%d,pattern=%d")
TRACE_MSG(MMIDC_FLOW_599_112_2_18_2_8_29_42,"[MMIDC Display_Callback dst yaddr=0x%X,uaddr=0x%X")
TRACE_MSG(MMIDC_FLOW_621_112_2_18_2_8_29_43,"[MMIDC] GetDCPreviewParamEx preview_param_ptr is PNULL !!!")
TRACE_MSG(MMIDC_FLOW_633_112_2_18_2_8_29_44,"[MMIDC] GetDCPreviewParamEx lcd_width = %d, lcd_height = %d")
TRACE_MSG(MMIDC_FLOW_639_112_2_18_2_8_29_45,"[MMIDC] GetDCPreviewParamEx rotation = %d, display mode = %d")
TRACE_MSG(MMIDC_FLOW_663_112_2_18_2_8_29_46,"[MMIDC]: SetDCAllPreviewParam s_dc_is_open = %d")
TRACE_MSG(MMIDC_FLOW_710_112_2_18_2_8_29_47,"[MMIDC]: DCAMERA_StartPreview ret = %d")
TRACE_MSG(MMIDC_FLOW_716_112_2_18_2_8_29_48,"[MMIDC]: MMIDC_GetPhotoSize() = %d")
TRACE_MSG(MMIDC_FLOW_717_112_2_18_2_8_29_49,"[MMIDC]: MMIDC_GePhotoSizetVerLcd() = %d")
TRACE_MSG(MMIDC_FLOW_726_112_2_18_2_8_29_50,"[MMIDC]: StartDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d")
TRACE_MSG(MMIDC_FLOW_745_112_2_18_2_8_29_51,"[MMIDC]: StopDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d")
TRACE_MSG(MMIDC_FLOW_772_112_2_18_2_8_29_52,"[MMIDC]: CloseDC ret = %d, s_dc_is_open = %d")
TRACE_MSG(MMIDC_FLOW_787_112_2_18_2_8_30_53,"[MMIDC]: MMIDC_IsDCOpen s_dc_is_open = %d")
TRACE_MSG(MMIDC_FLOW_797_112_2_18_2_8_30_54,"[MMIDC]: MMIDC_IsDVOpen s_dv_is_open = %d")
TRACE_MSG(MMIDC_FLOW_835_112_2_18_2_8_30_55,"[MMIDC]: OpenDC ret = %d")
TRACE_MSG(MMIDC_FLOW_914_112_2_18_2_8_30_56,"MMIDC_GetMultiShootEnum = %d")
TRACE_MSG(MMIDC_FLOW_927_112_2_18_2_8_30_57,"[MMIDC] SendCaptureFailMsg ret==%d")
TRACE_MSG(MMIDC_FLOW_930_112_2_18_2_8_30_58,"[MMIDC] SendCaptureFailMsg param_ptr=0x%X, param_size=%d")
TRACE_MSG(MMIDC_FLOW_952_112_2_18_2_8_30_59,"[MMIDC] SendCaptureSuccessMsg ret==%d")
TRACE_MSG(MMIDC_FLOW_955_112_2_18_2_8_30_60,"[MMIDC] SendCaptureSuccessMsg param_ptr=0x%X, param_size=%d")
TRACE_MSG(MMIDC_FLOW_980_112_2_18_2_8_30_61,"[MMIDC] SaveDataToFile ")
TRACE_MSG(MMIDC_FLOW_997_112_2_18_2_8_30_62,"[MMIDC] SaveDataToFile ret=%d")
TRACE_MSG(MMIDC_FLOW_1023_112_2_18_2_8_30_63,"[MMIDC] PlayCaptureRing is_play_ring = %d")
TRACE_MSG(MMIDC_FLOW_1041_112_2_18_2_8_30_64,"[MMIDC] PlayCaptureRing")
TRACE_MSG(MMIDC_FLOW_1053_112_2_18_2_8_30_65,"[MMIDC]: CaptureCallBack, ret = %d, data_buffer = %d, data_len = %d")
TRACE_MSG(MMIDC_FLOW_1058_112_2_18_2_8_30_66,"[MMIDC]: CaptureCallBack !s_is_capture")
TRACE_MSG(MMIDC_FLOW_1066_112_2_18_2_8_30_67,"[MMIDC]: CaptureCallBack s_is_cancel_multishot=%d, is_cancel_by_user=%d")
TRACE_MSG(MMIDC_FLOW_1103_112_2_18_2_8_30_68,"[MMIDC]: DCAdap_CaptureCallBack, index = %d, size = %d")
TRACE_MSG(MMIDC_FLOW_1110_112_2_18_2_8_30_69,"[MMIDC]: photo_id = %d, MMIDC_GetMulitShotNumber = %d")
TRACE_MSG(MMIDC_FLOW_1130_112_2_18_2_8_30_70,"[MMIDC]: CaptureCallBack  captured_photo_num = %d")
TRACE_MSG(MMIDC_FLOW_1147_112_2_18_2_8_30_71,"[MMIDC] CaptureCallBack LAST !photos_validate")
TRACE_MSG(MMIDC_FLOW_1167_112_2_18_2_8_30_72,"[MMIDC]: MMIDC_GetCapturedPhotoNumber captured_photo_num = %d")
TRACE_MSG(MMIDC_FLOW_1194_112_2_18_2_8_30_73,"[MMIDC]: Capture_Ex_Callback, ret = %d")
TRACE_MSG(MMIDC_FLOW_1200_112_2_18_2_8_30_74,"[MMIDC]: Capture_Ex_Callback !s_is_capture")
TRACE_MSG(MMIDC_FLOW_1206_112_2_18_2_8_30_75,"[MMIDC]: Capture_Ex_Callback t1 = %d")
TRACE_MSG(MMIDC_FLOW_1213_112_2_18_2_8_30_76,"[MMIDC]: Capture_Ex_Callback s_is_cancel_multishot=%d, is_cancel_by_user=%d")
TRACE_MSG(MMIDC_FLOW_1267_112_2_18_2_8_30_77,"[MMIDC] Capture_Ex_Callback process_result=%d")
TRACE_MSG(MMIDC_FLOW_1272_112_2_18_2_8_30_78,"[MMIDC] Capture_Ex_Callback data_size.w=%d, data_size.h=%d")
TRACE_MSG(MMIDC_FLOW_1305_112_2_18_2_8_31_79,"[MMIDC] Capture_Ex_Callback process_result=%d")
TRACE_MSG(MMIDC_FLOW_1323_112_2_18_2_8_31_80,"[MMIDC] Capture_Ex_Callback end t2=%d, t2-t1=%d")
TRACE_MSG(MMIDC_FLOW_1379_112_2_18_2_8_31_81,"[MMIDC] GetSnapshotExParam target_size.w =%d, h=%d")
TRACE_MSG(MMIDC_FLOW_1381_112_2_18_2_8_31_82,"[MMIDC] GetSnapshotExParam")
TRACE_MSG(MMIDC_FLOW_1444_112_2_18_2_8_31_83,"[MMIDC] SnapShot Quality = PNULL")
TRACE_MSG(MMIDC_FLOW_1464_112_2_18_2_8_31_84,"[MMIDC]: MMIDC_GetPhotoSize() = %d")
TRACE_MSG(MMIDC_FLOW_1465_112_2_18_2_8_31_85,"[MMIDC]: MMIDC_GePhotoSizetVerLcd() = %d")
TRACE_MSG(MMIDC_FLOW_1475_112_2_18_2_8_31_86,"[MMIDC]: SnapShot with frame")
TRACE_MSG(MMIDC_FLOW_1491_112_2_18_2_8_31_87,"[MMIDC]: SnapShot with frame buffer NULL!!!")
TRACE_MSG(MMIDC_FLOW_1496_112_2_18_2_8_31_88,"[MMIDC]: SnapShot rotation_mode=%d")
TRACE_MSG(MMIDC_FLOW_1512_112_2_18_2_8_31_89,"[MMIDC]: SnapShot return: %d")
TRACE_MSG(MMIDC_FLOW_1523_112_2_18_2_8_31_90,"[MMIDC]: ReviewPhotoCallBack +, ret_value = %d")
TRACE_MSG(MMIDC_FLOW_1562_112_2_18_2_8_31_91,"[MMIDC]: ReviewSinglePhotoFromBuf DC_REVIEW_JPEG")
TRACE_MSG(MMIDC_FLOW_1635_112_2_18_2_8_31_92,"[MMIDC]: is_real_hor_lcd= %d")
TRACE_MSG(MMIDC_FLOW_1637_112_2_18_2_8_31_93,"[MMIDC]: disp rect w= %d, h = %d")
TRACE_MSG(MMIDC_FLOW_1638_112_2_18_2_8_31_94,"[MMIDC]: target rect w = %d, h = %d")
TRACE_MSG(MMIDC_FLOW_1639_112_2_18_2_8_31_95,"[MMIDC]: rotation_mode = %d, disp_mode = %d")
TRACE_MSG(MMIDC_FLOW_1641_112_2_18_2_8_31_96,"[MMIDC]: jpeg_buffer = %d, jpeg_len = %d")
TRACE_MSG(MMIDC_FLOW_1645_112_2_18_2_8_31_97,"[MMIDC]: DCAMERA_ReviewJpeg return %d")
TRACE_MSG(MMIDC_FLOW_1659_112_2_18_2_8_31_98,"[MMIDC]: MMIDC_ReviewPhotos %d, flow_mode = %d")
TRACE_MSG(MMIDC_FLOW_1708_112_2_18_2_8_31_99,"[MMIDC]: RecordEndNotifyCallBack record end! i_type = %d")
TRACE_MSG(MMIDC_FLOW_1747_112_2_18_2_8_31_100,"[MMIDC]: RecordEndNotifyCallBack end_type= %d")
TRACE_MSG(MMIDC_FLOW_1783_112_2_18_2_8_31_101,"[MMIDC] ConvertImageData dst_ptr=0x%X,dst_size=%d,dst_width=%d,dst_height=%d")
TRACE_MSG(MMIDC_FLOW_1790_112_2_18_2_8_32_102,"[MMIDC] ConvertImageData src_ptr=0x%X,src_size=%d,src_width=%d,src_height=%d")
TRACE_MSG(MMIDC_FLOW_1796_112_2_18_2_8_32_103,"[MMIDC] ConvertImageData type=%d,angle=%d,is_around_color=%d")
TRACE_MSG(MMIDC_FLOW_1818_112_2_18_2_8_32_104,"[MMIDC] ConvertImageData PARAM error!!!")
TRACE_MSG(MMIDC_FLOW_1842_112_2_18_2_8_32_105,"[MMIDC] ConvertImageData size overflow!!!")
TRACE_MSG(MMIDC_FLOW_1879_112_2_18_2_8_32_106,"[MMIDC] ConvertImageData size overflow!!!")
TRACE_MSG(MMIDC_FLOW_1911_112_2_18_2_8_32_107,"[MMIDC] ConvertImageData size overflow!!!")
TRACE_MSG(MMIDC_FLOW_1943_112_2_18_2_8_32_108,"[MMIDC] ConvertImageData size overflow!!!")
TRACE_MSG(MMIDC_FLOW_1963_112_2_18_2_8_32_109,"[MMIDC] ConvertImageData offset_bytes=%d,offset_size=%d")
TRACE_MSG(MMIDC_FLOW_1965_112_2_18_2_8_32_110,"[MMIDC] ConvertImageData ret=%d")
TRACE_MSG(MMIDC_FLOW_2045_112_2_18_2_8_32_111,"[MMIDC] GetLastFrameCallback data_type = %d !!!")
TRACE_MSG(MMIDC_FLOW_2061_112_2_18_2_8_32_112,"[MMIDC] lcd_buffer_size = %d")
TRACE_MSG(MMIDC_FLOW_2066_112_2_18_2_8_32_113,"[MMIDC] main_buffer_ptr  is PNULL = 0x%lX")
TRACE_MSG(MMIDC_FLOW_2079_112_2_18_2_8_32_114,"[MMIDC] s_lastframe_ptr = 0x%lX")
TRACE_MSG(MMIDC_FLOW_2083_112_2_18_2_8_32_115,"[MMIDC] s_lastframe_ptr is PNULL")
TRACE_MSG(MMIDC_FLOW_2094_112_2_18_2_8_32_116,"[MMIDC] dst_angle_ptr = 0x%lX")
TRACE_MSG(MMIDC_FLOW_2096_112_2_18_2_8_32_117,"[MMIDC] src_angle_ptr = 0x%lX, data_len = %d")
TRACE_MSG(MMIDC_FLOW_2138_112_2_18_2_8_32_118,"[MMIDC] width = %d, height = %d")
TRACE_MSG(MMIDC_FLOW_2140_112_2_18_2_8_32_119,"[MMIDC] logic_width = %d, logic_height = %d, logic_angle=%d")
TRACE_MSG(MMIDC_FLOW_2142_112_2_18_2_8_32_120,"[MMIDC] absolute_angle=%d")
TRACE_MSG(MMIDC_FLOW_2145_112_2_18_2_8_32_121,"[MMIDC] is_keep_dst_logic_screen=%d")
TRACE_MSG(MMIDC_FLOW_2149_112_2_18_2_8_32_122,"[MMIDC] MAINLCD_LOGIC_ANGLE_0")
TRACE_MSG(MMIDC_FLOW_2249_112_2_18_2_8_32_123,"[MMIDC] MAINLCD_LOGIC_ANGLE_90")
TRACE_MSG(MMIDC_FLOW_2310_112_2_18_2_8_33_124,"[MMIDC]: StartRecord")
TRACE_MSG(MMIDC_FLOW_2355_112_2_18_2_8_33_125,"[MMIDC] StartRecord NO TYPE")
TRACE_MSG(MMIDC_FLOW_2394_112_2_18_2_8_33_126,"[MMIDC] StartRecord recorder_start_t.audio_info_ptr = PNULL")
TRACE_MSG(MMIDC_FLOW_2399_112_2_18_2_8_33_127,"[MMIDC]StartRecord recorder_start_t.video_info_ptr = PNULL")
TRACE_MSG(MMIDC_FLOW_2404_112_2_18_2_8_33_128,"[MMIDC] StartRecord recorder_start_t.file_name_ptr = PNULL")
TRACE_MSG(MMIDC_FLOW_2410_112_2_18_2_8_33_129,"[MMIDC]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d")
TRACE_MSG(MMIDC_FLOW_2414_112_2_18_2_8_33_130,"[MMIDC] DRECORDER_RegisterGetLastFrameCallback ret = %d")
TRACE_MSG(MMIDC_FLOW_2417_112_2_18_2_8_33_131,"[MMIDC]: DRECORDER_StartRecord = %d")
TRACE_MSG(MMIDC_FLOW_2443_112_2_18_2_8_33_132,"[MMIDC]: StartRecord - return success !")
TRACE_MSG(MMIDC_FLOW_2460_112_2_18_2_8_33_133,"[MMIDC]: SetDVAllPreviewParam = %d")
TRACE_MSG(MMIDC_FLOW_2485_112_2_18_2_8_33_134,"[MMIDC]: SetDVAllPreviewParam ret= %d")
TRACE_MSG(MMIDC_FLOW_2504_112_2_18_2_8_33_135,"[MMIDC]: StartDVPreview = %d, = %d")
TRACE_MSG(MMIDC_FLOW_2529_112_2_18_2_8_33_136,"[MMIDC]: StartDVPreview lcd_width= %d, lcd_height= %d, angle = %d")
TRACE_MSG(MMIDC_FLOW_2536_112_2_18_2_8_33_137,"[MMIDC]: DRECORDER_StartPreview = %d")
TRACE_MSG(MMIDC_FLOW_2565_112_2_18_2_8_33_138,"[MMIDC]: StopPreview = %d, = %d")
TRACE_MSG(MMIDC_FLOW_2572_112_2_18_2_8_33_139,"[MMIDC]: DRECORDER_StopPreview = %d")
TRACE_MSG(MMIDC_FLOW_2597_112_2_18_2_8_33_140,"[MMIDC]: OpenDV = %d")
TRACE_MSG(MMIDC_FLOW_2642_112_2_18_2_8_33_141,"[MMIDC] MMIDC_CopyLastFrame s_lastframe_ptr = 0x%lX, buffer_size = %d")
TRACE_MSG(MMIDC_FLOW_2644_112_2_18_2_8_33_142,"[MMIDC] main_buffer_ptr = 0x%lX")
TRACE_MSG(MMIDC_FLOW_2668_112_2_18_2_8_33_143,"[MMIDC]: CloseDV = %d")
TRACE_MSG(MMIDC_FLOW_2675_112_2_18_2_8_33_144,"[MMIDC]: DRECORDER_Close = %d")
TRACE_MSG(MMIDC_FLOW_2708_112_2_18_2_8_33_145,"[MMIDC]: BurstShot %d")
TRACE_MSG(MMIDC_FLOW_2714_112_2_18_2_8_33_146,"[MMIDC] BurstShot shot_num OVER 9")
TRACE_MSG(MMIDC_FLOW_2757_112_2_18_2_8_33_147,"[MMIDC] BurstShot Quality")
TRACE_MSG(MMIDC_FLOW_2765_112_2_18_2_8_33_148,"[MMIDC]: BurstShot rotaiton_mode: %d")
TRACE_MSG(MMIDC_FLOW_2767_112_2_18_2_8_33_149,"[MMIDC]: DCAMERA_DoBurstSnapshot rotation_mode=%d")
TRACE_MSG(MMIDC_FLOW_2770_112_2_18_2_8_33_150,"[MMIDC]: DCAMERA_DoBurstSnapshot return: %d")
TRACE_MSG(MMIDC_FLOW_2787_112_2_18_2_8_33_151,"[MMIDC]: DRECORDER_StopRecord return: %d")
TRACE_MSG(MMIDC_FLOW_3179_112_2_18_2_8_34_152,"[MMIDC]: MMIDC_FlowExit")
TRACE_MSG(MMIDC_FLOW_3223_112_2_18_2_8_34_153,"[MMIDC] MMIDC_FlowFunction op=%d,flow_mode=%d")
TRACE_MSG(MMIDC_FLOW_3276_112_2_18_2_8_34_154,"[MMIDC]: MMIDC_FlowStart fail")
TRACE_MSG(MMIDC_FLOW_3297_112_2_18_2_8_35_155,"[MMIDC]: MMIDC_FlowStart fail")
TRACE_MSG(MMIDC_FLOW_3313_112_2_18_2_8_35_156,"[MMIDC]: MMIDC_FlowStart res=%d,is_first_preview=%d")
TRACE_MSG(MMIDC_FLOW_3345_112_2_18_2_8_35_157,"[MMIDC]: MMIAPIDC_IsPreviewing dc preview = %d, dv preview = %d")
TRACE_MSG(MMIDC_FLOW_3355_112_2_18_2_8_35_158,"[MMIDC]: MMIDC_IsReviewing %d")
TRACE_MSG(MMIDC_FLOW_3365_112_2_18_2_8_35_159,"[MMIDC]: MMIDC_IsCapturing %d")
TRACE_MSG(MMIDC_FLOW_3415_112_2_18_2_8_35_160,"[MMIDC]MMIDC_CheckSpace: UDISK free spaced high value is = %d free space low value is = %d")
TRACE_MSG(MMIDC_FLOW_3425_112_2_18_2_8_35_161,"[MMIDC]MMIDC_CheckSpace sfs_sd_ret = %d, sfs_udisk_ret = %d,space_result = %d")
TRACE_MSG(MMIDC_FLOW_3453_112_2_18_2_8_35_162,"[MMIDC]MMIDC_CheckSpace dc_dev = %d, sfs_result = %d")
TRACE_MSG(MMIDC_FLOW_3472_112_2_18_2_8_35_163,"[MMIDC] MMIDC_ShowErrorMsg fs_err = %d,dev = %d")
TRACE_MSG(MMIDC_FLOW_3579_112_2_18_2_8_35_164,"[MMIDC] MMIDC_SetBlock")
TRACE_MSG(MMIDC_FLOW_3670_112_2_18_2_8_35_165,"[MMIDC] MMIDC_GetOSDFlagBeforePreview s_is_osd_flag_before_preview=%d")
TRACE_MSG(MMIDC_FLOW_3695_112_2_18_2_8_35_166,"[MMIDC]: MMIDC_OpenDC is_dc_open = %d")
TRACE_MSG(MMIDC_FLOW_3709_112_2_18_2_8_35_167,"[MMIDC]: MMIDC_OpenDC ret = %d")
TRACE_MSG(MMIDC_FLOW_3723_112_2_18_2_8_35_168,"[MMIDC]: MMIDC_OpenDC is_dc_open 2= %d")
TRACE_MSG(MMIDC_FLOW_3739_112_2_18_2_8_35_169,"[MMIDC]: MMIDC_OpenDV is_dv_open= %d")
TRACE_MSG(MMIDC_FLOW_3744_112_2_18_2_8_35_170,"[MMIDC]: DRECORDER_SelectSrcDevice = %d")
TRACE_MSG(MMIDC_FLOW_3746_112_2_18_2_8_35_171,"[MMIDC]: DRECORDER_Open = %d")
TRACE_MSG(MMIDC_FLOW_3761_112_2_18_2_8_35_172,"[MMIDC]: MMIDC_OpenDV is_dv_open2= %d")
TRACE_MSG(MMIDC_FLOW_3793_112_2_18_2_8_36_173,"[MMIDC]: MMIDC_StartPreviewForSwitchSize ret=%d")
TRACE_MSG(MMIDC_FLOW_3809_112_2_18_2_8_36_174,"[MMIDC] MMIDC_StopPreviewForSwitchSize ret=%d")
TRACE_MSG(MMIDC_FLOW_3834_112_2_18_2_8_36_175,"[MMIDC] MMIDC_CreateMainOSDLayer block_id=%d")
TRACE_MSG(MMIDC_FLOW_3843_112_2_18_2_8_36_176,"[MMIDC] MMIDC_CreateMainOSDLayer width=%d, height=%d")
TRACE_MSG(MMIDC_FLOW_3878_112_2_18_2_8_36_177,"[MMIDC] MMIDC_CreateMainOSDLayer result=%d,dev_info.block_id=%d")
TRACE_MSG(MMIDC_FLOW_3939_112_2_18_2_8_36_178,"[MMIDC] MMIDC_ClearMainOSDLayer block_id=%d")
TRACE_MSG(MMIDC_FLOW_3958_112_2_18_2_8_36_179,"[MMIDC] MMIDC_PDAReviewPhotosForEditPic")
TRACE_MSG(MMIDC_FLOW_3993_112_2_18_2_8_36_180,"[MMIDC] MMIDC_GetA888LastFrame src_ptr=0x%lX,width=%d,height=%d")
TRACE_MSG(MMIDC_FLOW_4009_112_2_18_2_8_36_181,"[MMIDC] MMIDC_GetA888LastFrame s_pda_a888_lastframe_ptr=0x%lX")
TRACE_MSG(MMIDC_FLOW_4069_112_2_18_2_8_36_182,"[MMIDC] MMIDC_ReCreateMainOSDLayer ret=%d, result=%d")
TRACE_MSG(MMIDC_FLOW_4116_112_2_18_2_8_36_183,"[MMIDC] MMIDC_AllocMemoryForReview addr=0x%X")
TRACE_MSG(MMIDC_FLOW_4121_112_2_18_2_8_36_184,"[MMIDC] MMIDC_AllocMemoryForReview ret=%d")
TRACE_MSG(MMIDC_FLOW_4158_112_2_18_2_8_36_185,"[MMIDC] MMIDC_AllocMemoryForReview addr_ptr=0x%X")
TRACE_MSG(MMIDC_FLOW_4241_112_2_18_2_8_37_186,"[MMIDC] MMIDC_GetCurrentFileBuffer ret=%d, buf_ptr=0x%X, buffer_size=%d")
TRACE_MSG(MMIDC_FLOW_4289_112_2_18_2_8_37_187,"test_imageprocess handle=%d")
TRACE_MSG(MMIDC_FLOW_4317_112_2_18_2_8_37_188,"test_imageprocess width=%d, height=%d")
TRACE_MSG(MMIDC_FLOW_4320_112_2_18_2_8_37_189,"test_imageprocess src chn0_ptr=0x%X, chn0_size=%d")
TRACE_MSG(MMIDC_FLOW_4323_112_2_18_2_8_37_190,"test_imageprocess src chn1_ptr=0x%X, chn1_size=%d")
TRACE_MSG(MMIDC_FLOW_4326_112_2_18_2_8_37_191,"test_imageprocess dst chn0_ptr=0x%X, chn1_ptr=0x%X")
TRACE_MSG(MMIDC_FLOW_4421_112_2_18_2_8_37_192,"test_imageprocess IMGPROC_DoProcess ret=%d")
TRACE_MSG(MMIDC_FLOW_4455_112_2_18_2_8_37_193,"[MMIDC] MMIDC_DisplayPreviewFrame t1=%d")
TRACE_MSG(MMIDC_FLOW_4499_112_2_18_2_8_37_194,"[MMIDC] MMIDC_DisplayPreviewFrame ret=%d")
TRACE_MSG(MMIDC_FLOW_4502_112_2_18_2_8_37_195,"[MMIDC] MMIDC_DisplayPreviewFrame is_skip_image_process=%d")
TRACE_MSG(MMIDC_FLOW_4521_112_2_18_2_8_37_196,"[MMIDC] MMIDC_DisplayPreviewFrame src width=%d,height=%d")
TRACE_MSG(MMIDC_FLOW_4525_112_2_18_2_8_37_197,"[MMIDC] MMIDC_DisplayPreviewFrame src type=%d,pattern=%d")
TRACE_MSG(MMIDC_FLOW_4528_112_2_18_2_8_37_198,"[MMIDC] MMIDC_DisplayPreviewFrame src yaddr=0x%X,uaddr=0x%X")
TRACE_MSG(MMIDC_FLOW_4532_112_2_18_2_8_37_199,"[MMIDC] MMIDC_DisplayPreviewFrame dst width=%d,height=%d")
TRACE_MSG(MMIDC_FLOW_4536_112_2_18_2_8_37_200,"[MMIDC] MMIDC_DisplayPreviewFrame dst type=%d,pattern=%d")
TRACE_MSG(MMIDC_FLOW_4539_112_2_18_2_8_37_201,"[MMIDC] MMIDC_DisplayPreviewFrame dst yaddr=0x%X,uaddr=0x%X")
TRACE_MSG(MMIDC_FLOW_4543_112_2_18_2_8_37_202,"[MMIDC] MMIDC_DisplayPreviewFrame s_dc_is_preview=%d")
TRACE_MSG(MMIDC_FLOW_4555_112_2_18_2_8_37_203,"[MMIDC] MMIDC_DisplayPreviewFrame t2=%d, t2-t1=%d")
TRACE_MSG(MMIDC_FLOW_4585_112_2_18_2_8_37_204,"[MMIDC] MMIDC_SaveDataToFile ret =%d")
TRACE_MSG(MMIDC_FLOW_4599_112_2_18_2_8_37_205,"[MMIDC] MMIDC_SendCaptureFailMsg ret==%d")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_143_112_2_18_2_8_38_206,"[MMIDC]: MMIDC_CompleteDisplayOSD")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_178_112_2_18_2_8_38_207,"[MMIDC]: ShowSoftKey layout_style = %d")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_372_112_2_18_2_8_38_208,"[MMIDC]: MMIDC_DisplaySoftKey mode = %d")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_1014_112_2_18_2_8_40_209,"[MMIDC] MMIDC_GetFrameDisplayPos id")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_1153_112_2_18_2_8_40_210,"[MMIDC] MMIDC_DisplayFrame frame index")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_1289_112_2_18_2_8_40_211,"[MMIDC] MMIDC_DisplayCurrentFileName NAME = PNULL")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_1412_112_2_18_2_8_40_212,"[MMIDC] MMIDC_DisplayCurrentFileName NAME = PNULL")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_2091_112_2_18_2_8_42_213,"[MMIDC] MMIDC_ShowAllOSD cur_mode = %d, show_state = %d")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_2128_112_2_18_2_8_42_214,"[MMIDC] MMIDC_ShowAllOSD is_show = %d")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_2332_112_2_18_2_8_42_215,"[MMIDC] MMIDC_DisplayLastFrame width = %d, height = %d")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_2335_112_2_18_2_8_42_216,"[MMIDC] MMIDC_DisplayLastFrame frame_ptr = 0x%lX")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_2346_112_2_18_2_8_42_217,"[MMIDC] MMIDC_DisplayLastFrame layer_ptr = 0x%lX, block_id=%d")
TRACE_MSG(MMIDC_FULL_OSD_DISPLAY_2448_112_2_18_2_8_42_218,"[MMIDC] MMIDC_DisplayDateOSDStringByFont BUFFER NULL!!!")
TRACE_MSG(MMIDC_GUI_286_112_2_18_2_8_43_219,"[MMIDC] MenuSetItemCallback index is overflow !!!")
TRACE_MSG(MMIDC_GUI_319_112_2_18_2_8_43_220,"[MMIDC] MenuSetItemText index is overflow !!!")
TRACE_MSG(MMIDC_GUI_1311_112_2_18_2_8_46_221,"[MMIDC] IconSetImage index is overflow !!!")
TRACE_MSG(MMIDC_GUI_1330_112_2_18_2_8_46_222,"[MMIDC] IconCallBackFocusIcon is PNULL !!!")
TRACE_MSG(MMIDC_GUI_1497_112_2_18_2_8_46_223,"[MMIDC] IconFocus index is overflow !!!")
TRACE_MSG(MMIDC_GUI_1517_112_2_18_2_8_46_224,"[MMIDC] IconSetCallback index is overflow")
TRACE_MSG(MMIDC_GUI_1532_112_2_18_2_8_46_225,"[MMIDC] IconSetOKCallback index is overflow !!!")
TRACE_MSG(MMIDC_GUI_1867_112_2_18_2_8_47_226,"[MMIDC]: MMIAPIDC_FreeGUI")
TRACE_MSG(MMIDC_GUIICONBAR_287_112_2_18_2_8_48_227,"[MMIDC] GetIconImageList ICON_IMG_MAX != total_img_number !!!")
TRACE_MSG(MMIDC_GUIICONBAR_1411_112_2_18_2_8_50_228,"[MMIDC] MMIDC_DisplayIconDesktop is_open =%d")
TRACE_MSG(MMIDC_GUIICONBAR_1425_112_2_18_2_8_50_229,"[MMIDC] MMIDC_DisplayIconDesktop ICON_IMG_MAX != total_img_number !!!")
TRACE_MSG(MMIDC_GUIICONBAR_1610_112_2_18_2_8_50_230,"[MMIDC] MMIDC_DisplayIconDesktop tag ptr NULL !!!")
TRACE_MSG(MMIDC_GUIICONBAR_1746_112_2_18_2_8_51_231,"[MMIDC] MMIDC_GetIconDesktopRectList max_number=%d, total_num=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1770_112_2_18_2_8_51_232,"[MMIDC] MMIDC_ShortcutForCapture mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1786_112_2_18_2_8_51_233,"[MMIDC] MMIDC_ShortcutForRecorder mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1802_112_2_18_2_8_51_234,"[MMIDC] MMIDC_ShortcutForCameraMode mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1842_112_2_18_2_8_51_235,"[MMIDC] MMIDC_ShortcutForSize mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1865_112_2_18_2_8_51_236,"[MMIDC] MMIDC_ShortcutForSize lcd_size=%d,screen_mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1939_112_2_18_2_8_51_237,"[MMIDC] MMIDC_ShortcutForSize total_size_list_number=%d, total_func_list_number=%d,cur_size=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1940_112_2_18_2_8_51_238,"[MMIDC] MMIDC_ShortcutForSize i=%d, next_size=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1960_112_2_18_2_8_51_239,"[MMIDC] MMIDC_ShortcutForEnvironment env=%d")
TRACE_MSG(MMIDC_GUIICONBAR_1990_112_2_18_2_8_51_240,"[MMIDC] MMIDC_ShortcutForEffect ef=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2020_112_2_18_2_8_51_241,"[MMIDC] MMIDC_ShortcutForWhiteBalance wb=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2060_112_2_18_2_8_51_242,"[MMIDC] MMIDC_ShortcutForDelayTime self_time=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2100_112_2_18_2_8_51_243,"[MMIDC] MMIDC_ShortcutForStore dc_dev=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2149_112_2_18_2_8_51_244,"[MMIDC] MMIDC_ShortcutForStore dc_dev =%d, ")
TRACE_MSG(MMIDC_GUIICONBAR_2193_112_2_18_2_8_52_245,"[MMIDC] MMIDC_ShortcutForBrightnessToHigh brighness=%d, mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2225_112_2_18_2_8_52_246,"[MMIDC] MMIDC_ShortcutForBrightnessToLow brighness=%d, mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2256_112_2_18_2_8_52_247,"[MMIDC] MMIDC_ShortcutForContrastToHigh contrast=%d, mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2288_112_2_18_2_8_52_248,"[MMIDC] MMIDC_ShortcutForContrastToLow brighness=%d, mode=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2330_112_2_18_2_8_52_249,"[MMIDC] MMIDC_ShortcutForMultiShot multi=%d")
TRACE_MSG(MMIDC_GUIICONBAR_2374_112_2_18_2_8_52_250,"[MMIDC] MMIDC_ClearIconDesktopRectList")
TRACE_MSG(MMIDC_GUIICONBAR_2386_112_2_18_2_8_52_251,"[MMIDC] MMIDC_CloseGUIIconDesktop")
TRACE_MSG(MMIDC_GUIPDA_1658_112_2_18_2_8_56_252,"[MMIDC] CreateDesktopPDA ret=%d")
TRACE_MSG(MMIDC_GUIPDA_1693_112_2_18_2_8_56_253,"[MMIDC] DestroyDesktopPDA")
TRACE_MSG(MMIDC_GUIPDA_1790_112_2_18_2_8_56_254,"[MMIDC] DisplayListCtrl is_ctrl_item=%d")
TRACE_MSG(MMIDC_GUIPDA_2278_112_2_18_2_8_57_255,"[MMIDC] ProcessExpandBtn is_expand_state=%d, btn_list_focus=%d, is_attr_press=%d")
TRACE_MSG(MMIDC_GUIPDA_2294_112_2_18_2_8_57_256,"[MMIDC] ProcessCmd1Btn")
TRACE_MSG(MMIDC_GUIPDA_2310_112_2_18_2_8_57_257,"[MMIDC] ProcessCmd2Btn")
TRACE_MSG(MMIDC_GUIPDA_2326_112_2_18_2_8_57_258,"[MMIDC] ProcessCmd3Btn")
TRACE_MSG(MMIDC_GUIPDA_2349_112_2_18_2_8_57_259,"[MMIDC] ProcessCaptureBtn ret=%d")
TRACE_MSG(MMIDC_GUIPDA_2381_112_2_18_2_8_57_260,"[MMIDC] ProcessCaptureKey ret=%d")
TRACE_MSG(MMIDC_GUIPDA_2493_112_2_18_2_8_57_261,"[MMIDC] ProcessButtonList ctrl_id=%d")
TRACE_MSG(MMIDC_GUIPDA_2553_112_2_18_2_8_57_262,"[MMIDC] MMIDC_SetDCIconTypeList icon_id=%d, cur_index=%d")
TRACE_MSG(MMIDC_GUIPDA_2674_112_2_18_2_8_58_263,"[MMIDC] MMIDC_SetDCIconSettingTypeList ctrl_id=%d,cur_index=%d,icon_id=%d")
TRACE_MSG(MMIDC_GUIPDA_2700_112_2_18_2_8_58_264,"[MMIDC] ProcessPicBtn")
TRACE_MSG(MMIDC_GUIPDA_2719_112_2_18_2_8_58_265,"[MMIDC] ProcessSensorBtn")
TRACE_MSG(MMIDC_GUIPDA_2760_112_2_18_2_8_58_266,"[MMIDC] ProcessZoomBtn ret=%d")
TRACE_MSG(MMIDC_GUIPDA_2784_112_2_18_2_8_58_267,"[MMIDC] ProcessArrowLeftBtn ret=%d, is_visible=%D")
TRACE_MSG(MMIDC_GUIPDA_2808_112_2_18_2_8_58_268,"[MMIDC] ProcessArrowRightBtn ret=%d, is_visible=%D")
TRACE_MSG(MMIDC_GUIPDA_2826_112_2_18_2_8_58_269,"[MMIDC] ProcessUCameraBtn is_press=%d")
TRACE_MSG(MMIDC_GUIPDA_2930_112_2_18_2_8_58_270,"[MMIDC] HandleTPPenOK ctrl_id=%d,is_update=%d, ret=%d")
TRACE_MSG(MMIDC_GUIPDA_3032_112_2_18_2_8_58_271,"[MMIDC] HandleTpMove ret=%d, point.x=%d,point.y=%d")
TRACE_MSG(MMIDC_GUIPDA_3108_112_2_18_2_8_59_272,"[MMIDC] IsContract is_press=%d,is_contract=%d")
TRACE_MSG(MMIDC_GUIPDA_3131_112_2_18_2_8_59_273,"[MMIDC] HandleContractExpandBtnTpDown is_press=%d")
TRACE_MSG(MMIDC_GUIPDA_3134_112_2_18_2_8_59_274,"[MMIDC] HandleContractExpandBtnTpDown ret=%d")
TRACE_MSG(MMIDC_GUIPDA_3166_112_2_18_2_8_59_275,"[MMIDC] HandleContractExpandBtnTpUp is_press=%d, is_contract=%d")
TRACE_MSG(MMIDC_GUIPDA_3169_112_2_18_2_8_59_276,"[MMIDC] HandleContractExpandBtnTpUp ret=%d")
TRACE_MSG(MMIDC_GUIPDA_3337_112_2_18_2_8_59_277,"[MMIDC] DesktopPDAProcessMsg ret=%d, msg_id=0x%lX")
TRACE_MSG(MMIDC_GUIPDA_3511_112_2_18_2_8_59_278,"[MMIDC] SetAllCmdBtnsVisible is_visible=%d")
TRACE_MSG(MMIDC_GUIPDA_3914_112_2_18_2_9_0_279,"[MMIDC]: CreateAllCmdBtns mode = %d")
TRACE_MSG(MMIDC_GUIPDA_5680_112_2_18_2_9_4_280,"[MMIDC] DisplayPDAAllItems cur_mode = %d, show_state = %d")
TRACE_MSG(MMIDC_GUIPDA_5708_112_2_18_2_9_4_281,"[MMIDC] DisplayPDAAllItems is_show = %d")
TRACE_MSG(MMIDC_GUIPDA_6121_112_2_18_2_9_4_282,"[MMIDC] FindFirstImgFiles param error!")
TRACE_MSG(MMIDC_GUIPDA_6129_112_2_18_2_9_4_283,"[MMIDC]:FindFirstImgFiles error:PNULL == find_data_ptr")
TRACE_MSG(MMIDC_GUIPDA_6137_112_2_18_2_9_5_284,"[MMIDC]:FindFirstImgFiles error:PNULL == find")
TRACE_MSG(MMIDC_GUIPDA_6178_112_2_18_2_9_5_285,"[MMIDC] FindFirstImgFile ret=%d")
TRACE_MSG(MMIDC_GUIPDA_6307_112_2_18_2_9_5_286,"[MMIDC] GetImgDecodedData Prame NULL !!!")
TRACE_MSG(MMIDC_GUIPDA_6340_112_2_18_2_9_5_287,"[MMIDC] GetImgDecodedData no memory")
TRACE_MSG(MMIDC_GUIPDA_6369_112_2_18_2_9_5_288,"[MMIDC] GetImgDecodedData no memory")
TRACE_MSG(MMIDC_GUIPDA_6393_112_2_18_2_9_5_289,"[MMIDC] GetImgDecodedData result = %d,img_ptr=0x%lX,img_width=%d,img_height=%d")
TRACE_MSG(MMIDC_GUIPDA_7082_112_2_18_2_9_6_290,"[MMIDC] PDAUpdateZoomValue ret=%d, cur_value=%d")
TRACE_MSG(MMIDC_GUIPDA_7120_112_2_18_2_9_6_291,"[MMIDC] UpdateZoomValue cur_step=%d,total_step=%d,total_value=%d,cur_value=%d")
TRACE_MSG(MMIDC_GUIPDA_7147_112_2_18_2_9_6_292,"[MMIDC] HandleZoomSeekBarTimer ret=%d")
TRACE_MSG(MMIDC_GUIPDA_7250_112_2_18_2_9_7_293,"[MMIDC] HandleZoomSeekBarTPMove y_step=%d,move_value=%d,cur_value=%d")
TRACE_MSG(MMIDC_GUIPDA_7256_112_2_18_2_9_7_294,"[MMIDC] HandleZoomSeekBarTPMove ret=%d,is_down=%d")
TRACE_MSG(MMIDC_GUIPDA_7314_112_2_18_2_9_7_295,"[MMIDC] HandleZoomSeekBarTpDown ret=%d")
TRACE_MSG(MMIDC_GUIPDA_7353_112_2_18_2_9_7_296,"[MMIDC] HandleZoomSeekBarTpUp update_result=%d")
TRACE_MSG(MMIDC_GUIPDA_7393_112_2_18_2_9_7_297,"[MMIDC] HandleZoomSeekBarTpUp ret=%d,is_down=%d")
TRACE_MSG(MMIDC_GUIPDA_7429_112_2_18_2_9_7_298,"[MMIDC] CreateImageAttrInfo attr_ptr=0x%lX")
TRACE_MSG(MMIDC_GUIPDA_7437_112_2_18_2_9_7_299,"[MMIDC] CreateImageAttrInfo ret=%d")
TRACE_MSG(MMIDC_GUIPDA_7449_112_2_18_2_9_7_300,"[MMIDC] DestroyImageAttrInfo attr_ptr=0x%lX")
TRACE_MSG(MMIDC_GUIPDA_7897_112_2_18_2_9_8_301,"[MMIDC] PDAUpdateImageAttrValue ret=%d, cur_value=%d,cur_attr=%d")
TRACE_MSG(MMIDC_GUIPDA_7944_112_2_18_2_9_8_302,"[MMIDC] UpdateImageAttrValue cur_step=%d,total_step =%d,total_value=%d,cur_value=%d")
TRACE_MSG(MMIDC_GUIPDA_7974_112_2_18_2_9_8_303,"[MMIDC] HandleImageAttrSeekBarTimer ret=%d")
TRACE_MSG(MMIDC_GUIPDA_8061_112_2_18_2_9_8_304,"[MMIDC] HandleImageAttrSeekBarTpMove y_step=%d,move_value=%d,cur_value=%d")
TRACE_MSG(MMIDC_GUIPDA_8066_112_2_18_2_9_8_305,"[MMIDC] HandleImageAttrSeekBarTpMove ret=%d,is_down=%d")
TRACE_MSG(MMIDC_GUIPDA_8118_112_2_18_2_9_8_306,"[MMIDC] HandleImageAttrSeekBarTpDown ret=%d, is_attr_press=%d")
TRACE_MSG(MMIDC_GUIPDA_8159_112_2_18_2_9_8_307,"[MMIDC] HandleImageAttrSeekBarTpUp update_result=%d")
TRACE_MSG(MMIDC_GUIPDA_8184_112_2_18_2_9_9_308,"[MMIDC] HandleImageAttrSeekBarTpUp cur_value=%d,total_value=%d,cur_step=%d,total_step")
TRACE_MSG(MMIDC_GUIPDA_8190_112_2_18_2_9_9_309,"[MMIDC] HandleImageAttrSeekBarTpUp ret=%d,point.x=%d,pointl.y=%d, is_down=%d")
TRACE_MSG(MMIDC_GUIPDA_8440_112_2_18_2_9_9_310,"[MMIDC] HandlePDAKey is_btn_expand_press=%d")
TRACE_MSG(MMIDC_GUIPDA_8542_112_2_18_2_9_9_311,"[MMIDC] HandlePDAKey msg_id=0x%lX, ret=%d, key=%d")
TRACE_MSG(MMIDC_GUIPDA_8651_112_2_18_2_9_9_312,"[MMIDC] RestorePDADesktop is_zoom_press=%d")
TRACE_MSG(MMIDC_GUIPDA_8960_112_2_18_2_9_10_313,"[MMIDC] HandleGlideExpandTpUp is_visible =%d, is_glide=%d")
TRACE_MSG(MMIDC_GUIPDA_8963_112_2_18_2_9_10_314,"[MMIDC] HandleGlideExpandTpUp ret =%d, is_move_panel=%d")
TRACE_MSG(MMIDC_GUIPDA_9257_112_2_18_2_9_11_315,"[MMIDC MMIDC_StartPreviewAnim step=%d")
TRACE_MSG(MMIDC_GUIPDA_9305_112_2_18_2_9_11_316,"[MMIDC] MMIDC_StartPreviewAnim start anim time=%d")
TRACE_MSG(MMIDC_GUIPDA_9381_112_2_18_2_9_11_317,"[MMIDC] MMIDC_StartPreviewAnim end_time=%d")
TRACE_MSG(MMIDC_GUIPDA_9859_112_2_18_2_9_12_318,"[MMIDC] ProcessEffect360LeftBtn ret=%d, is_visible=%D")
TRACE_MSG(MMIDC_GUIPDA_9884_112_2_18_2_9_12_319,"[MMIDC] ProcessEffect360RightBtn ret=%d, is_visible=%D")
TRACE_MSG(MMIDC_IMAGEPROCESS_172_112_2_18_2_9_12_320,"[MMDIC] GetImageProcessDirectory ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_201_112_2_18_2_9_13_321,"[MMIDC] OpenImageProc ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_270_112_2_18_2_9_13_322,"[MMIDC] MMIDC_OpenImgProc ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_271_112_2_18_2_9_13_323,"[MMIDC] MMIDC_OpenImgProc err=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_328_112_2_18_2_9_13_324,"[MMIDC] MMIDC_CloseUCamera")
TRACE_MSG(MMIDC_IMAGEPROCESS_346_112_2_18_2_9_13_325,"[MMIDC] MMIDC_IsUCameraEnable is_enable=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_363_112_2_18_2_9_13_326,"trace_param cmd=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_366_112_2_18_2_9_13_327,"trace_param src =0x%X")
TRACE_MSG(MMIDC_IMAGEPROCESS_367_112_2_18_2_9_13_328,"trace_param src format=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_369_112_2_18_2_9_13_329,"trace_param src format=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_372_112_2_18_2_9_13_330,"trace_param src chn0=0x%X,chn0_len=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_374_112_2_18_2_9_13_331,"trace_param src chn1=0x%X,chn1_len=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_376_112_2_18_2_9_13_332,"trace_param proc_mode=0x%X")
TRACE_MSG(MMIDC_IMAGEPROCESS_381_112_2_18_2_9_13_333,"trace_param proc_img w=%d, h=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_384_112_2_18_2_9_13_334,"trace_param proc_img format=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_387_112_2_18_2_9_13_335,"trace_param proc_img ctrl_callback=0x%X")
TRACE_MSG(MMIDC_IMAGEPROCESS_390_112_2_18_2_9_13_336,"trace_param proc_img size w=%d h=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_393_112_2_18_2_9_13_337,"trace_param proc_img chn0 =0x%X chn0_len=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_396_112_2_18_2_9_13_338,"trace_param proc_img chn1 =0x%X chn1_len=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_401_112_2_18_2_9_13_339,"trace_param before 0 w=%d, h=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_404_112_2_18_2_9_13_340,"trace_param before 0 format=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_407_112_2_18_2_9_13_341,"trace_param before 0 ctrl_callback=0x%X")
TRACE_MSG(MMIDC_IMAGEPROCESS_410_112_2_18_2_9_13_342,"trace_param before 0 size w=%d h=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_413_112_2_18_2_9_13_343,"trace_param before 0 chn0 =0x%X chn0_len=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_416_112_2_18_2_9_13_344,"trace_param before 0 chn1 =0x%X chn1_len=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_519_112_2_18_2_9_13_345,"[MMIDC] DoImageProcess src w=%d,h=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_523_112_2_18_2_9_13_346,"[MMIDC] DoImageProcess src chn1_ptr=0x%X, chn1_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_526_112_2_18_2_9_13_347,"[MMIDC] DoImageProcess src chn2_ptr=0x%X,chn2_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_529_112_2_18_2_9_13_348,"[MMIDC] DoImageProcess src chn3_ptr=0x%X,chn3_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_597_112_2_18_2_9_13_349,"[MMIDC] DoImageProcess img_proc_ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_601_112_2_18_2_9_13_350,"[MMIDC] DoImageProcess t2=%d, t2-t1=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_605_112_2_18_2_9_13_351,"[MMIDC] DoImageProcess rtn chn0=0x%X,chn0_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_608_112_2_18_2_9_13_352,"[MMIDC] DoImageProcess rtn chn1=0x%X,chn1_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_612_112_2_18_2_9_13_353,"[MMIDC] DoImageProcess dst chn1=0x%X,chn1_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_615_112_2_18_2_9_13_354,"[MMIDC] DoImageProcess dst chn2=0x%X,chn2_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_673_112_2_18_2_9_14_355,"[MMIDC] MMIDC_ProcessImage get src data img_proc_ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_726_112_2_18_2_9_14_356,"[MMIDC] MMIDC_ProcessImage is_skip_image_process=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_729_112_2_18_2_9_14_357,"[MMIDC] MMIDC_ProcessImage img_proc_ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_751_112_2_18_2_9_14_358,"[MMIDC] MMIDC_SetImageProcessType type=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_769_112_2_18_2_9_14_359,"[MMIDC] MMIDC_GetImageProcessType type=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_873_112_2_18_2_9_14_360,"[MMIDC] MMIDC_GetDataSizeByImgProcFormat data_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_893_112_2_18_2_9_14_361,"[MMIDC] MMIDC_IsPhotoData ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_911_112_2_18_2_9_14_362,"[MMIDC] MMIDC_SetPhotoDataFlag is_enable=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_929_112_2_18_2_9_14_363,"[MMIDC] MMIDC_GetUCameraStatus status=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_947_112_2_18_2_9_14_364,"[MMIDC] MMIDC_SetUCameraStatus status=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_967_112_2_18_2_9_14_365,"[MMIDC] MMIDC_IsUCameraDoing ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1037_112_2_18_2_9_14_366,"[MMIDC] MMIDC_UCameraASyncCallbackToSavePhotoFile format=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1040_112_2_18_2_9_14_367,"[MMIDC] MMIDC_UCameraASyncCallbackToSavePhotoFile chn0=0x%X,chn0_len=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1074_112_2_18_2_9_14_368,"[MMIDC] MMIDC_UCameraASyncCallbackSendMsgToApp format=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1077_112_2_18_2_9_14_369,"[MMIDC] MMIDC_UCameraASyncCallbackSendMsgToApp chn0=0x%X,chn0_len=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1120_112_2_18_2_9_14_370,"[MMIDC] MMIDC_GetDataSizeForJPG width=%d, height=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1123_112_2_18_2_9_14_371,"[MMIDC] MMIDC_GetDataSizeForJPG data_size=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1157_112_2_18_2_9_14_372,"[MMIDC] MMIDC_OpenUCameraCurTypeFunction type=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1160_112_2_18_2_9_14_373,"[MMIDC] MMIDC_OpenUCameraCurTypeFunction ret=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1193_112_2_18_2_9_15_374,"[MMIDC] MMIDC_DestroyUCameraCurTypeFunction type=%d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1219_112_2_18_2_9_15_375,"[MMIDC] MMIDC_SetUCameraPhotoData buf_ptr=0x%X,buf_size = %d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1222_112_2_18_2_9_15_376,"[MMIDC] MMIDC_SetUCameraPhotoData is_enable = %d")
TRACE_MSG(MMIDC_IMAGEPROCESS_1250_112_2_18_2_9_15_377,"[MMIDC] MMIDC_GetUCameraPhotoData is_enable = %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_476_112_2_18_2_9_16_378,"[MMI DC]: HandleHelpWinMsg, msg_id = %x")
TRACE_MSG(MMIDC_MAIN_WINTAB_547_112_2_18_2_9_16_379,"[MMIDC] MMIDC_SetLcdForRotate set = %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_548_112_2_18_2_9_16_380,"[MMIDC] MMIDC_SetLcdForRotate platform_angle = %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_549_112_2_18_2_9_16_381,"[MMIDC] MMIDC_SetLcdForRotate current_angle = %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_580_112_2_18_2_9_16_382,"[MMIDC]: HandleOSDKey key = %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_745_112_2_18_2_9_16_383,"StartSwitchOSDMenuItemsKeyTimer: the key timer has started!")
TRACE_MSG(MMIDC_MAIN_WINTAB_763_112_2_18_2_9_16_384,"StopSwitchOSDMenuItemsKeyTimer: the key timer has stop!")
TRACE_MSG(MMIDC_MAIN_WINTAB_779_112_2_18_2_9_16_385,"MMIDC_StartTextScrollTimer: the key timer has started!")
TRACE_MSG(MMIDC_MAIN_WINTAB_796_112_2_18_2_9_16_386,"MMIDC_StopTextScrollTimer: the key timer has stop!")
TRACE_MSG(MMIDC_MAIN_WINTAB_841_112_2_18_2_9_17_387,"[MMIDC] MMIAPIDC_OpenPhotoWin java running =%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_897_112_2_18_2_9_17_388,"[MMIDC] MMIAPIDC_OpenPhotoWin")
TRACE_MSG(MMIDC_MAIN_WINTAB_924_112_2_18_2_9_17_389,"[MMIDC] MMIAPIDC_OpenUCameraPhotoWin")
TRACE_MSG(MMIDC_MAIN_WINTAB_968_112_2_18_2_9_17_390,"[MMIDC] MMIAPIDC_OpenVideoWin")
TRACE_MSG(MMIDC_MAIN_WINTAB_1886_112_2_18_2_9_19_391,"[MMIDC] PauseOrResume s_is_dv_pause = %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_1951_112_2_18_2_9_19_392,"[MMIDC]KeyFunction key >= DC_KEY_MAX")
TRACE_MSG(MMIDC_MAIN_WINTAB_1957_112_2_18_2_9_19_393,"[MMIDC] KeyFunction key=%d,cur_mode=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_1975_112_2_18_2_9_19_394,"[MMIDC]: MMIDC_HandleDeletePubWinBG")
TRACE_MSG(MMIDC_MAIN_WINTAB_2036_112_2_18_2_9_19_395,"[MMIDC] HandleTPForMultiPhoto ret=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2058_112_2_18_2_9_19_396,"[MMIDC]: GUI_PointIsInRect rs_rect")
TRACE_MSG(MMIDC_MAIN_WINTAB_2064_112_2_18_2_9_19_397,"[MMIDC]: GUI_PointIsInRect mid_rect")
TRACE_MSG(MMIDC_MAIN_WINTAB_2069_112_2_18_2_9_19_398,"[MMIDC]: GUI_PointIsInRect ls_rect")
TRACE_MSG(MMIDC_MAIN_WINTAB_2077_112_2_18_2_9_19_399,"[MMIDC] HandleTPForSoftkey ret=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2090_112_2_18_2_9_19_400,"[MMIDC]: HandleDCWinTpPressDown %d, %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2236_112_2_18_2_9_20_401,"[MMIDC]: MMIDC_HandleGetFocus mode = %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2315_112_2_18_2_9_20_402,"[MMIDC]: MSG_TIMER s_3seconds_timer")
TRACE_MSG(MMIDC_MAIN_WINTAB_2330_112_2_18_2_9_20_403,"[MMIDC]: MSG_TIMER s_timer_delay")
TRACE_MSG(MMIDC_MAIN_WINTAB_2383_112_2_18_2_9_20_404,"[MMIDC]: MSG_TIMER s_updown_tip_timer")
TRACE_MSG(MMIDC_MAIN_WINTAB_2404_112_2_18_2_9_20_405,"[MMIDC]: MSG_TIMER s_switch_osd_key_timer_id last_key=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2472_112_2_18_2_9_20_406,"[MMIDC]: MSG_TIMER s_record_timer")
TRACE_MSG(MMIDC_MAIN_WINTAB_2486_112_2_18_2_9_20_407,"[MMIDC]: MSG_TIMER s_text_scroll_timer")
TRACE_MSG(MMIDC_MAIN_WINTAB_2504_112_2_18_2_9_20_408,"[MMIDC] s_hit_timer exec")
TRACE_MSG(MMIDC_MAIN_WINTAB_2720_112_2_18_2_9_20_409,"[MMIDC] StartDCActivity audio_handle=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2809_112_2_18_2_9_21_410,"[MMIDC]: HandleDCWaitWinMsg, msg_id = 0x%.2x")
TRACE_MSG(MMIDC_MAIN_WINTAB_2815_112_2_18_2_9_21_411,"[MMIDC] HandleDCWaitWinMsg start_time=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2820_112_2_18_2_9_21_412,"[MMIDC] HandleDCWaitWinMsg mode=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2893_112_2_18_2_9_21_413,"[MMIDC] ProcessRecordEnd is_normal= %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_2914_112_2_18_2_9_21_414,"[MMIDC]: HandleCameraWinMsg, msg_id = 0x%.2x")
TRACE_MSG(MMIDC_MAIN_WINTAB_2919_112_2_18_2_9_21_415,"[MMIDC]: MSG_OPEN_WINDOW")
TRACE_MSG(MMIDC_MAIN_WINTAB_2966_112_2_18_2_9_21_416,"[MMIDC] HandleCameraWinMsg flip_status = %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_3017_112_2_18_2_9_21_417,"[MMIDC]: HandleCameraWinMsg, MSG_LOSE_FOCUS")
TRACE_MSG(MMIDC_MAIN_WINTAB_3029_112_2_18_2_9_21_418,"[MMIDC]: HandleCameraWinMsg, MSG_GET_FOCUS")
TRACE_MSG(MMIDC_MAIN_WINTAB_3069_112_2_18_2_9_21_419,"[MMIDC]: MSG_DC_CAPTURE_SUCCESS_MSG")
TRACE_MSG(MMIDC_MAIN_WINTAB_3115_112_2_18_2_9_21_420,"[MMIDC]: MSG_DC_REVIEW_SUCCESS_MSG")
TRACE_MSG(MMIDC_MAIN_WINTAB_3119_112_2_18_2_9_21_421,"[MMIDC]: MSG_DC_CAPTURE_FAILE_MSG")
TRACE_MSG(MMIDC_MAIN_WINTAB_3187_112_2_18_2_9_21_422,"[MMIDC]: MSG_APP_WEB")
TRACE_MSG(MMIDC_MAIN_WINTAB_3222_112_2_18_2_9_21_423,"[MMIDC]: MSG_APP_OK")
TRACE_MSG(MMIDC_MAIN_WINTAB_3249_112_2_18_2_9_22_424,"[MMIDC]: MSG_APP_CANCEL")
TRACE_MSG(MMIDC_MAIN_WINTAB_3266_112_2_18_2_9_22_425,"[MMIDC]: MSG_APP_LEFT")
TRACE_MSG(MMIDC_MAIN_WINTAB_3291_112_2_18_2_9_22_426,"[MMIDC]: MSG_APP_RIGHT")
TRACE_MSG(MMIDC_MAIN_WINTAB_3325_112_2_18_2_9_22_427,"[MMIDC]: MSG_KEYUP")
TRACE_MSG(MMIDC_MAIN_WINTAB_3332_112_2_18_2_9_22_428,"[MMIDC]: MSG_APP_UP")
TRACE_MSG(MMIDC_MAIN_WINTAB_3358_112_2_18_2_9_22_429,"[MMIDC]: MSG_APP_DOWN")
TRACE_MSG(MMIDC_MAIN_WINTAB_3403_112_2_18_2_9_22_430,"[MMIDC]: MSG_CLOSE_WINDOW")
TRACE_MSG(MMIDC_MAIN_WINTAB_3410_112_2_18_2_9_22_431,"[MMIDC] HandleCameraWinMsg: MSG_KEYDOWN_FLIP!")
TRACE_MSG(MMIDC_MAIN_WINTAB_3429_112_2_18_2_9_22_432,"[MMIDC] HandleCameraWinMsg: MSG_KEYUP_FLIP! %d %d")
TRACE_MSG(MMIDC_MAIN_WINTAB_3436_112_2_18_2_9_22_433,"[MMIDC] Go on preview!")
TRACE_MSG(MMIDC_MAIN_WINTAB_3446_112_2_18_2_9_22_434,"[MMIDC] Display picture!")
TRACE_MSG(MMIDC_MAIN_WINTAB_3466_112_2_18_2_9_22_435,"[MMIDC] HandleCameraWinMsg: MSG_LCD_SWITCH")
TRACE_MSG(MMIDC_MAIN_WINTAB_3494_112_2_18_2_9_22_436,"[MMIDC]: HandleCameraWinMsg msg_id = 0x%.2x")
TRACE_MSG(MMIDC_MAIN_WINTAB_3593_112_2_18_2_9_22_437,"[MMIDC]: MMIAPIDC_Exit")
TRACE_MSG(MMIDC_MAIN_WINTAB_3634_112_2_18_2_9_22_438,"[MMIDC]: MMIAPIDC_Interrupt")
TRACE_MSG(MMIDC_MAIN_WINTAB_3690_112_2_18_2_9_22_439,"mmidc_wintab.c,  HandleSendWinMsg(), msg_id = %x")
TRACE_MSG(MMIDC_MAIN_WINTAB_3902_112_2_18_2_9_23_440,"[MMIDC] ProcessChipTestStep step=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4008_112_2_18_2_9_23_441,"[MMIDC] RestoreChipTestLastVideoSize video_size =%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4027_112_2_18_2_9_23_442,"[MMIDC] RestoreChipTestLastAutoSave autosave=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4044_112_2_18_2_9_23_443,"[MMIDC] HandleShortcutKey msg_id = %d, mode=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4217_112_2_18_2_9_23_444,"[MMIDC] HandleTPForIconBar is_desktop_open=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4239_112_2_18_2_9_23_445,"[MMIDC] HandleTPForIconBar rect_index=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4301_112_2_18_2_9_24_446,"[MMIDC] HandleTPForIconBar msg_id=%d, ret=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4337_112_2_18_2_9_24_447,"[MMIDC] HandleTPForIconBar rect_index=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4604_112_2_18_2_9_24_448,"[MMIDC] MMIDC_PDACmd key=%d,cur_mode=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4608_112_2_18_2_9_24_449,"[MMIDC]MMIDC_PDACmd key >= PDA_KEY_MAX")
TRACE_MSG(MMIDC_MAIN_WINTAB_4663_112_2_18_2_9_24_450,"[MMIDC] DCApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIDC_MAIN_WINTAB_4677_112_2_18_2_9_24_451,"[MMIDC] DCApplet_HandleEvent MSG_APPLET_START")
TRACE_MSG(MMIDC_MAIN_WINTAB_4682_112_2_18_2_9_24_452,"[MMIDC] DCApplet_HandleEvent MSG_APPLET_STOP")
TRACE_MSG(MMIDC_MAIN_WINTAB_4716_112_2_18_2_9_24_453,"[MMIDC] CloseDCApplet applet_ptr=0x%lX")
TRACE_MSG(MMIDC_MAIN_WINTAB_4755_112_2_18_2_9_25_454,"[MMIDC] StartDCApplet ret = %d,applet_ptr=0x%lX")
TRACE_MSG(MMIDC_MAIN_WINTAB_4773_112_2_18_2_9_25_455,"[MMIDC] MMIDC_CreateWinByApplet applet_ptr=0x%lX")
TRACE_MSG(MMIDC_MAIN_WINTAB_4803_112_2_18_2_9_25_456,"[MMIDC] MMIDC_GetFirstPreview ret=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4823_112_2_18_2_9_25_457,"[MMIDC] MMIDC_SetFirstPreview is_first_preview=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4843_112_2_18_2_9_25_458,"[MMIDC] MMIDC_GetDCModuleRunningFlag ret=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4862_112_2_18_2_9_25_459,"[MMIDC] MMIDC_SetDCModuleRunningFlag is_running=%d")
TRACE_MSG(MMIDC_MAIN_WINTAB_4921_112_2_18_2_9_25_460,"[MMIDC] MMIDC_ProcessMsgCmd cmd=%d")
TRACE_MSG(MMIDC_NINONE_106_112_2_18_2_9_25_461,"[MMIDC] GetNInOneHandle handle=%d")
TRACE_MSG(MMIDC_NINONE_133_112_2_18_2_9_25_462,"[MMIDC] CreateNInOneHandle ret=%d,handle=%d")
TRACE_MSG(MMIDC_NINONE_157_112_2_18_2_9_25_463,"[MMIDC] DestroyNInOneHandle ret=%d")
TRACE_MSG(MMIDC_NINONE_214_112_2_18_2_9_25_464,"[MMIDC] CreateNInOneOSDInfo img_size.w=%d, h=%d")
TRACE_MSG(MMIDC_NINONE_217_112_2_18_2_9_25_465,"[MMIDC] CreateNInOneOSDInfo ret=%d")
TRACE_MSG(MMIDC_NINONE_246_112_2_18_2_9_26_466,"[MMIDC] DestroyNInOneOSDInfo")
TRACE_MSG(MMIDC_NINONE_310_112_2_18_2_9_26_467,"[MMIDC] CreateNInOnePhotoInfo buf_ptr=0x%X, buf_size=%d")
TRACE_MSG(MMIDC_NINONE_313_112_2_18_2_9_26_468,"[MMIDC] CreateNInOnePhotoInfo ret=%d")
TRACE_MSG(MMIDC_NINONE_339_112_2_18_2_9_26_469,"[MMIDC] DestroyNInOnePhotoInfo")
TRACE_MSG(MMIDC_NINONE_426_112_2_18_2_9_26_470,"[MMIDC] MMIDC_OpenNInOne ret=%d")
TRACE_MSG(MMIDC_NINONE_474_112_2_18_2_9_26_471,"[MMIDC] MMIDC_IsNInOneEnable is_enable=%d")
TRACE_MSG(MMIDC_NINONE_786_112_2_18_2_9_27_472,"[MMIDC] ProcessPhotoImageCallback cur_index=%d")
TRACE_MSG(MMIDC_NINONE_837_112_2_18_2_9_27_473,"[MMIDC] ViewOsdImageCallback buffer_ptr=0x%X,buffer_size=%d")
TRACE_MSG(MMIDC_NINONE_840_112_2_18_2_9_27_474,"[MMIDC] ViewOsdImageCallback W=%d, h=%d")
TRACE_MSG(MMIDC_NINONE_982_112_2_18_2_9_27_475,"[MMIDC] MMIDC_GetNInOneImageProcessParam")
TRACE_MSG(MMIDC_NINONE_1036_112_2_18_2_9_27_476,"[MMIDC] MMIDC_GetNInOneImageProcessParam ret=%d")
TRACE_MSG(MMIDC_NINONE_1039_112_2_18_2_9_27_477,"[MMIDC] MMIDC_GetNInOneImageProcessParam is_skip_image_process=%d")
TRACE_MSG(MMIDC_OSD_OPTION_550_112_2_18_2_9_29_478,"[MMIDC] OpenDCModeMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_720_112_2_18_2_9_29_479,"[MMIDC] SetPhotoSizeState photo_size = %d")
TRACE_MSG(MMIDC_OSD_OPTION_740_112_2_18_2_9_29_480,"[MMIDC] SetPhotoSizeState screen mode = %d")
TRACE_MSG(MMIDC_OSD_OPTION_1139_112_2_18_2_9_30_481,"[MMIDC] GetPhonePhotoSizeList index=%d")
TRACE_MSG(MMIDC_OSD_OPTION_1180_112_2_18_2_9_30_482,"[MMIDC] GetPhonePhotoSizeFunctionList callback array")
TRACE_MSG(MMIDC_OSD_OPTION_1190_112_2_18_2_9_30_483,"[MMIDC] GetPhonePhotoSizeFunctionList copy_count=%d")
TRACE_MSG(MMIDC_OSD_OPTION_1538_112_2_18_2_9_31_484,"[MMIDC] MMIDC_GetPhoneVideoSizeList total_item=%d,count=%d")
TRACE_MSG(MMIDC_OSD_OPTION_1588_112_2_18_2_9_31_485,"[MMIDC] MMIDC_GetPhoneVideoSizeFunctionList copy_count=%d, total_item=%d")
TRACE_MSG(MMIDC_OSD_OPTION_1845_112_2_18_2_9_31_486,"[MMIDC] MMIDC_OpenVideoOption osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_1972_112_2_18_2_9_32_487,"[MMIDC] MMIDC_MoveCurReviewPhotoID() KEY")
TRACE_MSG(MMIDC_OSD_OPTION_1991_112_2_18_2_9_32_488,"[MMIDC] MMIDC_OpenVideoReviewOption osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2072_112_2_18_2_9_32_489,"[MMIDC] MMIDC_ShortcutForSwitchSensor sensor_id=%d")
TRACE_MSG(MMIDC_OSD_OPTION_2304_112_2_18_2_9_32_490,"[MMIDC] OpenMultiShotMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2359_112_2_18_2_9_32_491,"[MMIDC] OpenPhotoEffectMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2400_112_2_18_2_9_32_492,"[MMIDC] OpenPhotoEnvironmentMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2434_112_2_18_2_9_32_493,"[MMIDC] OpenPhotoFrameMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2599_112_2_18_2_9_33_494,"[MMIDC] OpenSelfDelayMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2630_112_2_18_2_9_33_495,"[MMIDC] OpenDCModeMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2673_112_2_18_2_9_33_496,"[MMIDC] OpenVideoEffectMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2729_112_2_18_2_9_33_497,"[MMIDC] OpenVideoSizeMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2742_112_2_18_2_9_33_498,"[MMIDC] OpenVideoSizeMenu cur_size=%d")
TRACE_MSG(MMIDC_OSD_OPTION_2750_112_2_18_2_9_33_499,"[MMIDC] OpenVideoSizeMenu total_size_number=%d, total_func_number=%d error!!!")
TRACE_MSG(MMIDC_OSD_OPTION_2786_112_2_18_2_9_33_500,"[MMIDC] OpenWhiteBalanceMenu osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_2826_112_2_18_2_9_33_501,"[MMIDC] OpenWhiteBalanceMenu() TYPE")
TRACE_MSG(MMIDC_OSD_OPTION_2867_112_2_18_2_9_33_502,"[MMIDC] OpenPhotoEnvironmentMenu osd_icon = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_3774_112_2_18_2_9_35_503,"[MMIDC] MMIDC_OpenPhotoOption osd_menu = PNULL")
TRACE_MSG(MMIDC_OSD_OPTION_4072_112_2_18_2_9_36_504,"[MMIDC] MMIDC_GetDVSettingTypeListItems cur_size=%d")
TRACE_MSG(MMIDC_OSD_OPTION_4080_112_2_18_2_9_36_505,"[MMIDC] MMIDC_GetDVSettingTypeListItems total_size_number=%d, total_func_number=%d error!!!")
TRACE_MSG(MMIDC_PHOTODATE_96_112_2_18_2_9_37_506,"[MMIDC] SetPhotoDateLayerID id=%d")
TRACE_MSG(MMIDC_PHOTODATE_155_112_2_18_2_9_37_507,"[MMIDC] GetDateOSDInitAttribute photo_size=%d, total_items=%d")
TRACE_MSG(MMIDC_PHOTODATE_445_112_2_18_2_9_38_508,"[MMIDC] DisplayPhotoDateString date_width=%d,time_width=%d,interval_width=%d")
TRACE_MSG(MMIDC_PHOTODATE_449_112_2_18_2_9_38_509,"[MMIDC] DisplayPhotoDateString text_width_max=%d,text_height_max=%d")
TRACE_MSG(MMIDC_PHOTODATE_516_112_2_18_2_9_38_510,"[MMIDC] DisplayPhotoDateString photo_size=%d,x=%d, y=%d")
TRACE_MSG(MMIDC_PHOTODATE_535_112_2_18_2_9_38_511,"[MMIDC] GetPhotoDateOSDRectWidthAndHeight width=%d, height=%d")
TRACE_MSG(MMIDC_PHOTODATE_604_112_2_18_2_9_38_512,"[MMIDC] GetPhotoDateLayerInfo ret=%d,id=%d")
TRACE_MSG(MMIDC_PHOTODATE_669_112_2_18_2_9_39_513,"[MMIDC] MMIDC_DestoryPhotoDateOSDLayer id=%d,block_id=%d")
TRACE_MSG(MMIDC_PHOTODATE_701_112_2_18_2_9_39_514,"[MMIDC] CreateOSDLayer id=%d,block_id=%d")
TRACE_MSG(MMIDC_PHOTODATE_723_112_2_18_2_9_39_515,"[MMIDC] CreateOSDLayer ret=%d")
TRACE_MSG(MMIDC_PHOTODATE_745_112_2_18_2_9_39_516,"[MMIDC] CreateOSDLayer block_id=%d, buf_ptr=0x%X")
TRACE_MSG(MMIDC_PHOTODATE_750_112_2_18_2_9_39_517,"[MMIDC] CreateOSDLayer width=%d, height=%d")
TRACE_MSG(MMIDC_PHOTODATE_756_112_2_18_2_9_39_518,"[MMIDC] CreateOSDLayer result=%d")
TRACE_MSG(MMIDC_PHOTODATE_808_112_2_18_2_9_39_519,"[MMIDC] CreatePhotoDateLayers ret=%d")
TRACE_MSG(MMIDC_PHOTODATE_828_112_2_18_2_9_39_520,"[MMIDC] MMIDC_CreatePhotoDateLayers ret=%d")
TRACE_MSG(MMIDC_PHOTODATE_888_112_2_18_2_9_39_521,"[MMIDC] GetDisplayOSDLayerInfo ret=%d, width=%d, height=%d")
TRACE_MSG(MMIDC_PHOTODATE_899_112_2_18_2_9_39_522,"[MMIDC] UpdatePhotoDateCallback")
TRACE_MSG(MMIDC_PHOTODATE_918_112_2_18_2_9_39_523,"[MMIDC] MMIDC_CreatePhotoDateData s_photo_date_info_ptr=0x%lX")
TRACE_MSG(MMIDC_PHOTODATE_925_112_2_18_2_9_39_524,"[MMIDC] MMIDC_CreatePhotoDateData ALLOC s_photo_date_info_ptr=0x%lX")
TRACE_MSG(MMIDC_PHOTODATE_941_112_2_18_2_9_39_525,"[MMIDC] MMIDC_CreatePhotoDateData 2 s_photo_date_info_ptr=0x%lX")
TRACE_MSG(MMIDC_PHOTODATE_953_112_2_18_2_9_39_526,"[MMIDC] MMIDC_ReleasePhotoDateData s_photo_date_info_ptr=0x%lX")
TRACE_MSG(MMIDC_PHOTODATE_1039_112_2_18_2_9_39_527,"[MMIDC] GetPhotoDateParameter is_enable=%d")
TRACE_MSG(MMIDC_SAVE_82_112_2_18_2_9_40_528,"[MMIDC] MMIDC_NewSaveData s_save_data_ptr is PNULL !!!")
TRACE_MSG(MMIDC_SAVE_95_112_2_18_2_9_40_529,"[MMIDC]: MMIDC_DeleteSaveData")
TRACE_MSG(MMIDC_SAVE_136_112_2_18_2_9_40_530,"[MMIDC] MMIDC_SendByMMS file_type=%d")
TRACE_MSG(MMIDC_SAVE_239_112_2_18_2_9_40_531,"[MMIDC] MMIDC_SendByBt Path = PNULL")
TRACE_MSG(MMIDC_SAVE_330_112_2_18_2_9_40_532,"[MMIDC] MMIDC_SendByBt file_size=%d")
TRACE_MSG(MMIDC_SAVE_375_112_2_18_2_9_40_533,"MMIDC_SetCurrentPhotoAsWallpaper is_success =%d")
TRACE_MSG(MMIDC_SAVE_406_112_2_18_2_9_40_534,"[MMIDC] CheckDevRoomSufficient ret=%d")
TRACE_MSG(MMIDC_SAVE_426_112_2_18_2_9_40_535,"[MMIDC]: RealCreateOnePicName photo_index=%d, file_dev=%d")
TRACE_MSG(MMIDC_SAVE_447_112_2_18_2_9_40_536,"[MMIDC] RealCreateOnePicName create file ret=%d")
TRACE_MSG(MMIDC_SAVE_486_112_2_18_2_9_41_537,"[MMIDC] CreateOnePicName %d")
TRACE_MSG(MMIDC_SAVE_489_112_2_18_2_9_41_538,"[MMIDC] CreateOnePicName dc infor = PNULL")
TRACE_MSG(MMIDC_SAVE_495_112_2_18_2_9_41_539,"[MMIDC] CreateOnePicName dc infor = PNULL")
TRACE_MSG(MMIDC_SAVE_514_112_2_18_2_9_41_540,"[MMIDC] CreateOnePicName ret=%d, user_sel_dev=%d,suitable_dev=%d")
TRACE_MSG(MMIDC_SAVE_554_112_2_18_2_9_41_541,"[MMIDC]: CreateOnePicName success")
TRACE_MSG(MMIDC_SAVE_563_112_2_18_2_9_41_542,"[MMIDC]: CreateOnePicName fail")
TRACE_MSG(MMIDC_SAVE_590_112_2_18_2_9_41_543,"[MMIDC]: CreateOnePicName i=%d")
TRACE_MSG(MMIDC_SAVE_593_112_2_18_2_9_41_544,"[MMIDC]: CreateOnePicName count_dev_num=%d, old_file_dev=%d,cur_file_dev=%d")
TRACE_MSG(MMIDC_SAVE_595_112_2_18_2_9_41_545,"[MMIDC]: CreateOnePicName ret=%d")
TRACE_MSG(MMIDC_SAVE_616_112_2_18_2_9_41_546,"[MMIDC]: WriteToFile success")
TRACE_MSG(MMIDC_SAVE_625_112_2_18_2_9_41_547,"[MMIDC]: WriteToFile file_ret=%d, write_ret=%d")
TRACE_MSG(MMIDC_SAVE_641_112_2_18_2_9_41_548,"[MMIDC]MMIDC_SavePhotos ret=%d, photo_id=%d")
TRACE_MSG(MMIDC_SAVE_657_112_2_18_2_9_41_549,"[MMIDC]: GetVideoSubfix DV_VIDEOEXT_MP4 -")
TRACE_MSG(MMIDC_SAVE_662_112_2_18_2_9_41_550,"[MMIDC]: GetVideoSubfix DV_VIDEOEXT_3GP -")
TRACE_MSG(MMIDC_SAVE_667_112_2_18_2_9_41_551,"[MMIDC]: GetVideoSubfix VIDEO_FILE_TYPE_MJPEG -")
TRACE_MSG(MMIDC_SAVE_673_112_2_18_2_9_41_552,"[MMIDC] GetVideoSubfix TYPE")
TRACE_MSG(MMIDC_SAVE_703_112_2_18_2_9_41_553,"[MMIDC] CheckVideoSavePath is_folder_exist=%d, ret=%d")
TRACE_MSG(MMIDC_SAVE_735_112_2_18_2_9_41_554,"[MMIDC] CreateVideoName subfix = PNULL")
TRACE_MSG(MMIDC_SAVE_740_112_2_18_2_9_41_555,"[MMIDC] CreateVideoName path = PNULL")
TRACE_MSG(MMIDC_SAVE_744_112_2_18_2_9_41_556,"[MMIDC]: CreateVideoName")
TRACE_MSG(MMIDC_SAVE_747_112_2_18_2_9_41_557,"[MMIDC] CreateVideoName ucs2_name = PNULL")
TRACE_MSG(MMIDC_SAVE_756_112_2_18_2_9_41_558,"[MMIDC] UDISK MEM NO DISK")
TRACE_MSG(MMIDC_SAVE_772_112_2_18_2_9_41_559,"[MMIDC] CreateVideoName DV error_ret=%d, user_sel_dev=%d,suitable_dev=%d")
TRACE_MSG(MMIDC_SAVE_873_112_2_18_2_9_41_560,"[MMIDC] CreateVideoName user_sel_dev=%d, old_sel_dev=%d, i =%d")
TRACE_MSG(MMIDC_SAVE_876_112_2_18_2_9_41_561,"[MMIDC] CreateVideoName error_ret=%d, count_dev_num=%d")
TRACE_MSG(MMIDC_SAVE_902_112_2_18_2_9_41_562,"[MMIDC]: MMIDC_CreateVideoName +")
TRACE_MSG(MMIDC_SAVE_908_112_2_18_2_9_41_563,"[MMIDC]: MMIDC_CreateVideoName - create dv name fail!")
TRACE_MSG(MMIDC_SAVE_916_112_2_18_2_9_41_564,"[MMIDC]: MMIDC_CreateVideoName -")
TRACE_MSG(MMIDC_SAVE_926_112_2_18_2_9_42_565,"[MMIDC]: MMIDC_DeleteNoSavedFile +-")
TRACE_MSG(MMIDC_SAVE_1024_112_2_18_2_9_42_566,"[MMIDC] MMIDC_OpenPictureWin path = PNULL")
TRACE_MSG(MMIDC_SAVE_1048_112_2_18_2_9_42_567,"[MMIDC] MMIDC_OpenMovieWin path = PNULL")
TRACE_MSG(MMIDC_SAVE_1053_112_2_18_2_9_42_568,"[MMIDC] MMIDC_OpenMovieWin name = PNULL")
TRACE_MSG(MMIDC_SAVE_1091_112_2_18_2_9_42_569,"[MMIDC] MMIAPIDC_GetSavePhotoIndex array = PNULL")
TRACE_MSG(MMIDC_SAVE_1178_112_2_18_2_9_42_570,"[MMIDC] MMIDC_GetCurrentPhotoInfo is_enable=%d")
TRACE_MSG(MMIDC_SAVE_1280_112_2_18_2_9_42_571,"[MMIDC]: MMIDC_SaveImageFile")
TRACE_MSG(MMIDC_SAVE_1320_112_2_18_2_9_42_572,"[MMIDC]: MMIDC_SaveImageFile ret=%d")
TRACE_MSG(MMIDC_SAVE_1337_112_2_18_2_9_42_573,"[MMIDC] MMIDC_SetMaxPhotoFileSize file_size=%d")
TRACE_MSG(MMIDC_SAVE_1354_112_2_18_2_9_42_574,"[MMIDC] MMIDC_GetMaxPhotoFileSize file_size=%d")
TRACE_MSG(MMIDC_SAVE_1382_112_2_18_2_9_42_575,"[MMIDC]: GeneratePhotoFileName %d")
TRACE_MSG(MMIDC_SAVE_1387_112_2_18_2_9_42_576,"[MMIDC] GeneratePhotoFileName name info = PNULL")
TRACE_MSG(MMIDC_SAVE_1468_112_2_18_2_9_43_577,"[MMIFILE] SetAttr NO memory !")
TRACE_MSG(MMIDC_SAVE_1531_112_2_18_2_9_43_578,"[MMIDC]: MMIDC_SetPhotoFileHidden file_ret=%d,is_enable=%d")
TRACE_MSG(MMIDC_SAVE_1593_112_2_18_2_9_43_579,"[MMIDC]: MMIDC_RenamePhotoFile photo_index=%d")
TRACE_MSG(MMIDC_SAVE_1608_112_2_18_2_9_43_580,"[MMIDC]: MMIDC_RenamePhotoFile ret=%d,result=%d,is_enable=%d")
TRACE_MSG(MMIDC_SAVE_1702_112_2_18_2_9_43_581,"[MMIDC] MMIDC_SetMaxImageFileSize file_size=%d")
TRACE_MSG(MMIDC_SAVE_1721_112_2_18_2_9_43_582,"[MMIDC] MMIDC_GetMaxImageFileSize file_size=%d")
TRACE_MSG(MMIDC_SAVE_1737_112_2_18_2_9_43_583,"[MMIDC] MMIDC_SetTempFileNameID name_id=%d")
TRACE_MSG(MMIDC_SAVE_1754_112_2_18_2_9_43_584,"[MMIDC] MMIDC_GetTempFileNameID name_id=%d")
TRACE_MSG(MMIDC_SAVE_1805_112_2_18_2_9_43_585,"[MMIDC] MMIDC_DeleteImageFile ret=%d")
TRACE_MSG(MMIDC_SAVE_1825_112_2_18_2_9_43_586,"[MMIDC] MMIDC_DeleteAllImageFile")
TRACE_MSG(MMIDC_SAVE_1841_112_2_18_2_9_43_587,"[MMIDC] MMIDC_DeleteAllDiskImageFile")
TRACE_MSG(MMIDC_SAVE_1872_112_2_18_2_9_44_588,"[MMIDC] MMIDC_DeleteAllSavedImageFile ret=%d,index=%d")
TRACE_MSG(MMIDC_SAVE_1892_112_2_18_2_9_44_589,"[MMIDC] MMIDC_DeleteAllSavedImageFile ret=%d")
TRACE_MSG(MMIDC_SETTING_167_112_2_18_2_9_44_590,"[MMIDC]: MMIAPIDC_AllocSettingMemory s_setting_data_ptr = 0x%x")
TRACE_MSG(MMIDC_SETTING_176_112_2_18_2_9_44_591,"[MMIDC]: MMIAPIDC_FreeSettingMemory s_setting_data_ptr = 0x%x")
TRACE_MSG(MMIDC_SETTING_224_112_2_18_2_9_44_592,"[MMIDC]: MMIDC_SaveSettings is_save=%d")
TRACE_MSG(MMIDC_SETTING_342_112_2_18_2_9_45_593,"[MMIDC] InitSizeArray PHOTO_SIZE_MAX != total_num !!!")
TRACE_MSG(MMIDC_SETTING_350_112_2_18_2_9_45_594,"[MMIDC] InitSizeArray VIDEO_SIZE_MAX != total_num !!!")
TRACE_MSG(MMIDC_SETTING_410_112_2_18_2_9_45_595,"[MMIDC] CheckVideoSize file_type = %d, video_size=%d")
TRACE_MSG(MMIDC_SETTING_462_112_2_18_2_9_45_596,"[MMIDC] CheckVideoSize video_size = %d")
TRACE_MSG(MMIDC_SETTING_479_112_2_18_2_9_45_597,"[MMIDC] InitVideoSizeToDefault video_size = %d")
TRACE_MSG(MMIDC_SETTING_488_112_2_18_2_9_45_598,"[MMIDC]: MMIDC_InitNVSettingDefaultValue")
TRACE_MSG(MMIDC_SETTING_518_112_2_18_2_9_45_599,"[MMIDC] MMIDC_InitNVSettingDefaultValue ,screen_mode=%d")
TRACE_MSG(MMIDC_SETTING_591_112_2_18_2_9_45_600,"[MMIDC]: MMIDC_Setting_GetNVDefaultValue")
TRACE_MSG(MMIDC_SETTING_636_112_2_18_2_9_45_601,"[MMIDC] MMIDC_Setting_InitDefaultValue ret = %d")
TRACE_MSG(MMIDC_SETTING_665_112_2_18_2_9_45_602,"[MMIDC]: MMIDC_Setting_GetNVDefaultValue ret=%d")
TRACE_MSG(MMIDC_SETTING_678_112_2_18_2_9_45_603,"[MMIDC]: MMIDC_Setting_InitDefaultValue")
TRACE_MSG(MMIDC_SETTING_712_112_2_18_2_9_46_604,"[MMIDC]: MMIAPIDC_Setting_SetFactoryValue")
TRACE_MSG(MMIDC_SETTING_775_112_2_18_2_9_46_605,"[MMIDC] MMIAPIDC_Setting_SetFactoryValue sensor_angle 1 = %d,screen_mode=%d")
TRACE_MSG(MMIDC_SETTING_823_112_2_18_2_9_46_606,"[MMIDC]: MMIDC_CloseCallbackTimer")
TRACE_MSG(MMIDC_SETTING_847_112_2_18_2_9_46_607,"[MMIDC]: ParamCtrlCallBack")
TRACE_MSG(MMIDC_SETTING_933_112_2_18_2_9_46_608,"[MMIDC] MMIDC_SetScreenMode mode")
TRACE_MSG(MMIDC_SETTING_966_112_2_18_2_9_46_609,"[MMIDC] MMIAPIDC_SetPhotoSize size = %d")
TRACE_MSG(MMIDC_SETTING_977_112_2_18_2_9_46_610,"[MMIDC] MMIAPIDC_SetPhotoSize size = %d")
TRACE_MSG(MMIDC_SETTING_1002_112_2_18_2_9_46_611,"[MMIDC] MMIAPIDC_SetPhotoSize ret = %d")
TRACE_MSG(MMIDC_SETTING_1065_112_2_18_2_9_46_612,"[MMIDC] MMIAPIDC_SetPhotoSize shoot_enum = %d")
TRACE_MSG(MMIDC_SETTING_1086_112_2_18_2_9_46_613,"[MMIDC] MMIDC_SetFrameIndex frame")
TRACE_MSG(MMIDC_SETTING_1098_112_2_18_2_9_46_614,"[MMIDC] MMIDC_SetFrameIndex frame=%d")
TRACE_MSG(MMIDC_SETTING_1136_112_2_18_2_9_46_615,"[MMIDC] MMIDC_SetFrameIndex is_preview=%d")
TRACE_MSG(MMIDC_SETTING_1156_112_2_18_2_9_46_616,"[MMIDC] MMIDC_SetShootMode mode")
TRACE_MSG(MMIDC_SETTING_1197_112_2_18_2_9_46_617,"[MMIDC] MMIDC_SetMultiShootEnum shoot=%d")
TRACE_MSG(MMIDC_SETTING_1219_112_2_18_2_9_47_618,"[MMIDC] MMIDC_SetMultiShootEnum photo_size=%d")
TRACE_MSG(MMIDC_SETTING_1239_112_2_18_2_9_47_619,"[MMIDC] MMIDC_SetPhotoStorageDevice dev")
TRACE_MSG(MMIDC_SETTING_1270_112_2_18_2_9_47_620,"[MMIDC] MMIDC_SetPhotoQuality quality")
TRACE_MSG(MMIDC_SETTING_1293_112_2_18_2_9_47_621,"[MMIDC] MMIDC_SetAutoSave promt")
TRACE_MSG(MMIDC_SETTING_1326_112_2_18_2_9_47_622,"[MMIDC] MMIDC_SetRecordFileType type")
TRACE_MSG(MMIDC_SETTING_1364_112_2_18_2_9_47_623,"[MMIDC] MMIDC_SetVideoSize size")
TRACE_MSG(MMIDC_SETTING_1390_112_2_18_2_9_47_624,"[MMIDC] MMIDC_SetVideoSize size=%d")
TRACE_MSG(MMIDC_SETTING_1410_112_2_18_2_9_47_625,"[MMIDC] MMIDC_SetVideoStorageDevice dev")
TRACE_MSG(MMIDC_SETTING_1448_112_2_18_2_9_47_626,"[MMIDC]: MMIDC_UpdatePhotoZoomValue modify multi shot!")
TRACE_MSG(MMIDC_SETTING_1460_112_2_18_2_9_47_627,"[MMIDC]: MMIDC_SetDCZoomValue return: %d, %d")
TRACE_MSG(MMIDC_SETTING_1492_112_2_18_2_9_47_628,"[MMIDC] MMIDC_SetShootScene scene")
TRACE_MSG(MMIDC_SETTING_1533_112_2_18_2_9_47_629,"[MMIDC] MMIDC_SetShootScene 0")
TRACE_MSG(MMIDC_SETTING_1548_112_2_18_2_9_47_630,"[MMIDC] MMIDC_SetFlashLamp flash")
TRACE_MSG(MMIDC_SETTING_1572_112_2_18_2_9_47_631,"[MMIDC] MMIDC_SetExposalCompensate exposal_compensate")
TRACE_MSG(MMIDC_SETTING_1588_112_2_18_2_9_47_632,"[MMIDC] MMIDC_SetExposalCompensate setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_1597_112_2_18_2_9_47_633,"[MMIDC]: MMIDC_SetExposalCompensate compentsate return %d, %d")
TRACE_MSG(MMIDC_SETTING_1629_112_2_18_2_9_47_634,"[MMIDC] MMIDC_SetSelfShootDelayTime time")
TRACE_MSG(MMIDC_SETTING_1752_112_2_18_2_9_48_635,"[MMIDC] MMIDC_SetPhotoEffect effect")
TRACE_MSG(MMIDC_SETTING_1768_112_2_18_2_9_48_636,"[MMIDC] MMIDC_SetPhotoEffect setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_1778_112_2_18_2_9_48_637,"[MMIDC]: MMIDC_SetPhotoEffect return: %d, effect = %d")
TRACE_MSG(MMIDC_SETTING_1822_112_2_18_2_9_48_638,"[MMIDC] MMIDC_SetVideoEffect effect")
TRACE_MSG(MMIDC_SETTING_1838_112_2_18_2_9_48_639,"[MMIDC] MMIDC_SetVideoEffect setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_1848_112_2_18_2_9_48_640,"[MMIDC]: MMIDC_SetVideoEffect return: %d, %d")
TRACE_MSG(MMIDC_SETTING_1871_112_2_18_2_9_48_641,"[MMIDC] MMIDC_SetShutterVoice voice")
TRACE_MSG(MMIDC_SETTING_1905_112_2_18_2_9_48_642,"[MMIDC] MMIDC_SetPhotoEnvironment environment")
TRACE_MSG(MMIDC_SETTING_1921_112_2_18_2_9_48_643,"[MMIDC] MMIDC_SetPhotoEnvironment setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_1942_112_2_18_2_9_48_644,"[MMIDC]: SetphotoEnvironment return %d, %d")
TRACE_MSG(MMIDC_SETTING_1977_112_2_18_2_9_48_645,"[MMIDC] MMIDC_SetVideoEnvironment environment")
TRACE_MSG(MMIDC_SETTING_1993_112_2_18_2_9_48_646,"[MMIDC] MMIDC_SetVideoEnvironment setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_2015_112_2_18_2_9_48_647,"[MMIDC]: SetvideoEnvironment return %d, %d")
TRACE_MSG(MMIDC_SETTING_2049_112_2_18_2_9_48_648,"[MMIDC] MMIDC_SetContrast contrast")
TRACE_MSG(MMIDC_SETTING_2065_112_2_18_2_9_48_649,"[MMIDC] MMIDC_SetContrast setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_2074_112_2_18_2_9_48_650,"[MMIDC]: SetphotoContrast return %d, %d")
TRACE_MSG(MMIDC_SETTING_2099_112_2_18_2_9_48_651,"[MMIDC] MMIDC_SetVideoContrast contrast")
TRACE_MSG(MMIDC_SETTING_2115_112_2_18_2_9_48_652,"[MMIDC] MMIDC_SetVideoContrast setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_2124_112_2_18_2_9_48_653,"[MMIDC]: MMIDC_SetVideoContrast return %d, %d")
TRACE_MSG(MMIDC_SETTING_2176_112_2_18_2_9_48_654,"[MMIDC] MMIDC_SetPhotoFlicker cor")
TRACE_MSG(MMIDC_SETTING_2192_112_2_18_2_9_48_655,"[MMIDC] MMIDC_SetPhotoFlicker setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_2200_112_2_18_2_9_48_656,"[MMIDC]: MMIDC_SetPhotoFlicker return %d, %d")
TRACE_MSG(MMIDC_SETTING_2234_112_2_18_2_9_49_657,"[MMIDC] MMIDC_SetVideoFlicker cor")
TRACE_MSG(MMIDC_SETTING_2250_112_2_18_2_9_49_658,"[MMIDC] MMIDC_SetVideoFlicker setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_2258_112_2_18_2_9_49_659,"[MMIDC]: MMIDC_SetVideoFlicker return %d, %d")
TRACE_MSG(MMIDC_SETTING_2290_112_2_18_2_9_49_660,"[MMIDC] MMIDC_SetISO iso")
TRACE_MSG(MMIDC_SETTING_2324_112_2_18_2_9_49_661,"[MMIDC] MMIDC_SetPhotoWhiteBalance setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_2333_112_2_18_2_9_49_662,"[MMIDC]: SetphotoWhiteBalance return %d, %d")
TRACE_MSG(MMIDC_SETTING_2365_112_2_18_2_9_49_663,"[MMIDC] MMIDC_SetCameraMode mode")
TRACE_MSG(MMIDC_SETTING_2386_112_2_18_2_9_49_664,"[MMIDC]: SetVideoZoomValue return %d, %d")
TRACE_MSG(MMIDC_SETTING_2406_112_2_18_2_9_49_665,"[MMIDC]: SetvideoWhiteBalance return %d")
TRACE_MSG(MMIDC_SETTING_2429_112_2_18_2_9_49_666,"[MMIDC] MMIDC_SetPhotoBrightness photo_brightness")
TRACE_MSG(MMIDC_SETTING_2445_112_2_18_2_9_49_667,"[MMIDC] MMIDC_SetPhotoBrightness setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_2454_112_2_18_2_9_49_668,"[MMIDC]: MMIDC_SetPhotoBrightness return %d, %d")
TRACE_MSG(MMIDC_SETTING_2488_112_2_18_2_9_49_669,"[MMIDC] MMIDC_SetVideoBrightness video_brightness")
TRACE_MSG(MMIDC_SETTING_2503_112_2_18_2_9_49_670,"[MMIDC] MMIDC_SetVideoBrightness setting count > 0 !!!")
TRACE_MSG(MMIDC_SETTING_2512_112_2_18_2_9_49_671,"[MMIDC]: MMIDC_SetvideoBrightness return: %d, %d")
TRACE_MSG(MMIDC_SETTING_2579_112_2_18_2_9_49_672,"[MMIDC] MMIDC_IncreaseZoomValue U CAMERA NO SUPPORT")
TRACE_MSG(MMIDC_SETTING_2610_112_2_18_2_9_49_673,"[MMIDC] MMIDC_IncreaseZoomValue U CAMERA NO SUPPORT")
TRACE_MSG(MMIDC_SETTING_2707_112_2_18_2_9_49_674,"[MMIDC]: MMIDC_IsSettingParamCtrl %d")
TRACE_MSG(MMIDC_SETTING_3109_112_2_18_2_9_50_675,"[MMIDC] MMIDC_InitInfo platform_angle = %d")
TRACE_MSG(MMIDC_SETTING_3110_112_2_18_2_9_50_676,"[MMIDC] MMIDC_InitInfo lcd_size = %d")
TRACE_MSG(MMIDC_SETTING_3112_112_2_18_2_9_50_677,"[MMIDC] MMIDC_InitInfo physical width = %d, height = %d")
TRACE_MSG(MMIDC_SETTING_3194_112_2_18_2_9_50_678,"[MMIDC] lcd_id = %d, width = %d, height = %d")
TRACE_MSG(MMIDC_SETTING_3287_112_2_18_2_9_51_679,"[MMIDC] MMIDC_GetMaxZoomLevel zoom_max = 0x%04lX")
TRACE_MSG(MMIDC_SETTING_3292_112_2_18_2_9_51_680,"[MMIDC] MMIDC_GetMaxZoomLevel FAIL")
TRACE_MSG(MMIDC_SETTING_3296_112_2_18_2_9_51_681,"[MMIDC] MMIDC_GetMaxZoomLevel out zoom_max = %d")
TRACE_MSG(MMIDC_SETTING_3334_112_2_18_2_9_51_682,"[MMIDC] MMIDC_GetPhysicalLCDWidthAndHeight logic_angle =%d")
TRACE_MSG(MMIDC_SETTING_3336_112_2_18_2_9_51_683,"[MMIDC] width = %d,height = %d")
TRACE_MSG(MMIDC_SETTING_3338_112_2_18_2_9_51_684,"[MMIDC] rect_right = %d,rect_bottom = %d")
TRACE_MSG(MMIDC_SETTING_3346_112_2_18_2_9_51_685,"[MMIDC] out rect_right = %d,rect_bottom = %d")
TRACE_MSG(MMIDC_SETTING_3347_112_2_18_2_9_51_686,"[MMIDC] out angle = %d")
TRACE_MSG(MMIDC_SETTING_3356_112_2_18_2_9_51_687,"[MMIDC] *angle = %d")
TRACE_MSG(MMIDC_SETTING_3419_112_2_18_2_9_51_688,"[MMIDC] MMIAPIDC_GetDCModuleSensorAngle angle=%d,id=%d")
TRACE_MSG(MMIDC_SETTING_3435_112_2_18_2_9_51_689,"[MMIDC] MMIAPIDC_SetDCModuleSensorAngle angle=%d,id=%d")
TRACE_MSG(MMIDC_SETTING_3463_112_2_18_2_9_51_690,"[MMIDC] MMIDC_IsSilentMode mode=%d")
TRACE_MSG(MMIDC_SETTING_3516_112_2_18_2_9_51_691,"[MMIDC] MMIDC_ConvertDCameraSizeToPhotoSize total_item !!!")
TRACE_MSG(MMIDC_SETTING_3529_112_2_18_2_9_51_692,"[MMIDC] MMIDC_ConvertDCameraSizeToPhotoSize photo_size = %d,dcamera_size=%d")
TRACE_MSG(MMIDC_SETTING_3565_112_2_18_2_9_51_693,"[MMIDC] MMIDC_ConvertPhotoSizeToDCameraSize total_item !!!")
TRACE_MSG(MMIDC_SETTING_3579_112_2_18_2_9_51_694,"[MMIDC] MMIDC_ConvertPhotoSizeToDCameraSize photo_size = %d,dcamera_size= %d")
TRACE_MSG(MMIDC_SETTING_3603_112_2_18_2_9_51_695,"[MMIDC] MMIDC_GetSupportSizeArray size_bit = 0x%08lX")
TRACE_MSG(MMIDC_SETTING_3696_112_2_18_2_9_51_696,"[MMIDC] MMIDC_SetDCameraSensorID sensor_id = %d")
TRACE_MSG(MMIDC_SETTING_3729_112_2_18_2_9_51_697,"[MMIDC] MMIDC_SetDCameraSensorID ret = %d")
TRACE_MSG(MMIDC_SETTING_3743_112_2_18_2_9_52_698,"[MMIDC] MMIDC_SetDCameraSensorID sensor_id = %d")
TRACE_MSG(MMIDC_SETTING_3762_112_2_18_2_9_52_699,"[MMIDC] MMIDC_GetDCameraSensorNumber FAIL")
TRACE_MSG(MMIDC_SETTING_3766_112_2_18_2_9_52_700,"[MMIDC] MMIDC_GetDCameraSensorNumber out sensor_number = %d")
TRACE_MSG(MMIDC_SETTING_3805_112_2_18_2_9_52_701,"[MMIDC] MMIDC_GetBurstNumberByPhotoSize num = %d")
TRACE_MSG(MMIDC_SETTING_3843_112_2_18_2_9_52_702,"[MMIDC]: MMIDC_InitParamCtrl")
TRACE_MSG(MMIDC_SETTING_3924_112_2_18_2_9_52_703,"[MMIDC] MMIAPIDC_GetDCModulSensorType sensor_type=%d")
TRACE_MSG(MMIDC_SETTING_3950_112_2_18_2_9_52_704,"[MMIDC] MMIAPIDC_GetDCModulSensorType sensor_type 2=%d")
TRACE_MSG(MMIDC_SETTING_3968_112_2_18_2_9_52_705,"[MMIDC] MMIDC_SetBackupFrameIndex frame_index=%d")
TRACE_MSG(MMIDC_SETTING_4020_112_2_18_2_9_52_706,"[MMIDC] MMIDC_SetNVShutterVoice voice")
TRACE_MSG(MMIDC_SETTING_4024_112_2_18_2_9_52_707,"[MMIDC] MMIDC_SetNVShutterVoice voice=%d")
TRACE_MSG(MMIDC_SETTING_4090_112_2_18_2_9_52_708,"[MMIDC] MMIDC_GetThumbnailPhotoSize photo_size=%d")
TRACE_MSG(MMIDC_SETTING_4107_112_2_18_2_9_52_709,"[MMIDC] MMIAPIDC_GetDCModuleSensorNumber sensor_number=%d")
TRACE_MSG(MMIDC_SETTING_4133_112_2_18_2_9_52_710,"[MMIDC] MMIAPIDC_GetDCModuleSensorNumber sensor_number 2=%d")
TRACE_MSG(MMIDC_SETTING_4150_112_2_18_2_9_52_711,"[MMIDC] MMIDC_GetSoftwareRotationAngle angle=%d")
TRACE_MSG(MMIDC_SETTING_4164_112_2_18_2_9_52_712,"[MMIDC] MMIDC_SetSoftwareRotationAngle angle=%d")
TRACE_MSG(MMIDC_SETTING_4186_112_2_18_2_9_52_713,"[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle dc_ret=%d")
TRACE_MSG(MMIDC_SETTING_4196_112_2_18_2_9_52_714,"[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle dv_ret=%d")
TRACE_MSG(MMIDC_SETTING_4205_112_2_18_2_9_52_715,"[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle ret=%d")
TRACE_MSG(MMIDC_SETTING_4226_112_2_18_2_9_52_716,"[MMIDC] MMIDC_GetDVMaxZoomLevel FAIL ret=%d")
TRACE_MSG(MMIDC_SETTING_4230_112_2_18_2_9_52_717,"[MMIDC] MMIDC_GetDVMaxZoomLevel out zoom_max = %d")
TRACE_MSG(MMIDC_SETTING_4272_112_2_18_2_9_53_718,"[MMIDC]: MMIDC_UpdateDVZoomValue ret=%d, zoom_param=%d")
TRACE_MSG(MMIDC_SETTING_4273_112_2_18_2_9_53_719,"[MMIDC]: MMIDC_UpdateDVZoomValue zoom=%d")
TRACE_MSG(MMIDC_SETTING_4318_112_2_18_2_9_53_720,"[MMIDC] MMIDC_IncreaseDVZoomValue ret=%d")
TRACE_MSG(MMIDC_SETTING_4340_112_2_18_2_9_53_721,"[MMIDC] MMIDC_DecreaseDVZoomValue ret=%d")
TRACE_MSG(MMIDC_SETTING_4365_112_2_18_2_9_53_722,"[MMIDC] MMIDC_GetScreenPhotoSize screen_size=%d")
TRACE_MSG(MMIDC_SETTING_4414_112_2_18_2_9_53_723,"[MMIDC] MMIDC_GetMaxVideoSize max_video_size= %d, file_type=%d")
TRACE_MSG(MMIDC_SETTING_4442_112_2_18_2_9_53_724,"[MMIDC] MMIDC_GetMaxVideoSize min_video_size= %d")
TRACE_MSG(MMIDC_SETTING_4533_112_2_18_2_9_53_725,"[MMIDC] MMIDC_GetSupportVideoSizeArray copy_count=%d, total_item=%d, video_size_ptr=%d")
TRACE_MSG(MMIDC_SETTING_4570_112_2_18_2_9_53_726,"[MMIDC] MMIDC_GetDataFormatInfo data_format=%d, data_unit=%d")
TRACE_MSG(MMIDC_SETTING_4626_112_2_18_2_9_53_727,"[MMIDC] MMIDC_SetMMIBrushStatus status=%d")
TRACE_MSG(MMIDC_SETTING_4641_112_2_18_2_9_53_728,"[MMIDC] MMIDC_GetMMIBrushStatus status=%d")
TRACE_MSG(MMIDC_SETTING_4662_112_2_18_2_9_53_729,"[MMIDC] MMIDC_IsMMIBrushing ret=%d")
TRACE_MSG(MMIDC_SETTING_4676_112_2_18_2_9_53_730,"[MMIDC] MMIDC_SetMMIFocusStatus status=%d")
TRACE_MSG(MMIDC_SETTING_4691_112_2_18_2_9_53_731,"[MMIDC] MMIDC_GetMMIFocusStatus status=%d")
TRACE_MSG(MMIDC_SETTING_4705_112_2_18_2_9_53_732,"[MMIDC] MMIDC_PushPhotoSize photo_size=%d")
TRACE_MSG(MMIDC_SETTING_4720_112_2_18_2_9_53_733,"[MMIDC] MMIDC_PullPhotoSize photo_size=%d")
TRACE_MSG(MMIDC_SETTING_4734_112_2_18_2_9_53_734,"[MMIDC] MMIDC_PushVisualMode visual_mode=%d")
TRACE_MSG(MMIDC_SETTING_4749_112_2_18_2_9_53_735,"[MMIDC] MMIDC_PullVisualMode visual_mode=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_86_112_2_18_2_9_54_736,"[MMIDC] GetSmileAutoPhotoHandle handle=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_113_112_2_18_2_9_54_737,"[MMIDC] CreateSmileAutoPhotoHandle ret=%d,handle=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_137_112_2_18_2_9_54_738,"[MMIDC] DestroySmileAutoPhotoHandle ret=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_171_112_2_18_2_9_54_739,"[MMIDC] MMIDC_ClearSmileAutoPhotoOSDLayer block_id=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_175_112_2_18_2_9_54_740,"[MMIDC] MMIDC_ClearSmileAutoPhotoOSDLayer layer clear")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_231_112_2_18_2_9_54_741,"[MMIDC] MMIDC_CreateSmileAutoPhotoOSDLayer width=%d, height=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_267_112_2_18_2_9_54_742,"[MMIDC] MMIDC_CreateSmileAutoPhotoOSDLayer result=%d,dev_info.block_id=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_329_112_2_18_2_9_54_743,"[MMIDC] MMIDC_OpenSmileAutoPhoto ret=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_363_112_2_18_2_9_55_744,"[MMIDC] MMIDC_IsSmileAutoPhotoEnable is_enable=%d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_475_112_2_18_2_9_55_745,"[MMIDC] ConvertVerRectToHorRect lcd_height = %d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_513_112_2_18_2_9_55_746,"[MMIDC] DisplaySmileAutoPhotoOsdRect pram 0 num = %d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_518_112_2_18_2_9_55_747,"[MMIDC] DisplaySmileAutoPhotoOsdRect dect_pram_ptr->num == 0,return ")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_527_112_2_18_2_9_55_748,"[MMIDC] DisplaySmileAutoPhotoOsdRect dc_disp_rect x = %d,y = %d,w = %d,h = %d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_533_112_2_18_2_9_55_749,"[MMIDC] DisplaySmileAutoPhotoOsdRect pram i=%d,x = %d,y = %d,w = %d,h = %d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_553_112_2_18_2_9_55_750,"[MMIDC] DisplaySmileAutoPhotoOsdRect osd rect  left = %d,top = %d,right = %d,bottom = %d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_558_112_2_18_2_9_55_751,"[MMIDC] DisplaySmileAutoPhotoOsdRect SCI_SUCCESS! draw Rect ")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_573_112_2_18_2_9_55_752,"[MMIDC] DisplaySmileAutoPhotoOsdRect  Draw Rect ")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_621_112_2_18_2_9_55_753,"[MMIDC] IsSmileAutoPhotoAllSmile result = %d")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_657_112_2_18_2_9_55_754,"[MMIDC] SmileAutoPhotoImageProcessCallBack")
TRACE_MSG(MMIDC_SMILEAUTOPHOTO_757_112_2_18_2_9_55_755,"[MMIDC] MMIDC_GetSmileAutoPhotoImageProcessParam ret=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_281_112_2_18_2_9_56_756,"[MMIDC] ProcessIconEffect proc_param.cmd=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_282_112_2_18_2_9_56_757,"[MMIDC] ProcessIconEffect proc_param.size.w=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_283_112_2_18_2_9_56_758,"[MMIDC] ProcessIconEffect proc_param.size.h=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_285_112_2_18_2_9_56_759,"[MMIDC] ProcessIconEffect proc_param.input_addr.chn0=0x%x, proc_param.input_addr.chn0_len = %d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_287_112_2_18_2_9_56_760,"[MMIDC] ProcessIconEffect proc_param.output_addr.chn0=0x%x, proc_param.output_addr.chn0_len = %d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_291_112_2_18_2_9_56_761,"[MMIDC] ProcessIconEffect rtn_ret=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_341_112_2_18_2_9_56_762,"[MMIDC] InitIconEffect iconlist_num=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_344_112_2_18_2_9_56_763,"InitIconEffect handle=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_373_112_2_18_2_9_56_764,"[MMIDC] InitIconEffect src_rgb_ptr is null")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_387_112_2_18_2_9_56_765,"[MMIDC] InitIconEffect buffer_ptr is null")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_406_112_2_18_2_9_56_766,"[MMIDC] InitIconEffect GUIRES_CopyImgBuf error 2")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_422_112_2_18_2_9_56_767,"[MMIDC] InitIconEffect ProcessIconEffect error")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_457_112_2_18_2_9_56_768,"[MMIDC] InitIconEffect end")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_547_112_2_18_2_9_57_769,"[MMIDC] SelectEffectItem select_type=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_611_112_2_18_2_9_57_770,"[MMIDC] SelectEffectItem ret=%d")
TRACE_MSG(MMIDC_UCAMERA_WINTAB_663_112_2_18_2_9_57_771,"[MMIDC] HandleUCameraWinMsg InitUCameraWin fail")
TRACE_MSG(MMIDC_WINDOW_OPTION_290_112_2_18_2_9_58_772,"[MMIDC]: HandlePhotoReviewOptionWinMsg(), msg_id = %x")
TRACE_MSG(MMIDC_WINDOW_OPTION_420_112_2_18_2_9_58_773,"[MMIDC] HandleDcStorageOptWinMsg udisk=%d,sd=%d")
TRACE_MSG(MMIDC_WINDOW_OPTION_535_112_2_18_2_9_59_774,"[MMIDC] GetStorageDeviceList count_dev_num=%d")
TRACE_MSG(MMIDC_WINDOW_OPTION_588_112_2_18_2_9_59_775,"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d")
TRACE_MSG(MMIDC_WINDOW_OPTION_672_112_2_18_2_9_59_776,"[MMIDC] HandleDcStorageOptWinMsg OK max_items=%d")
TRACE_MSG(MMIDC_WINDOW_OPTION_680_112_2_18_2_9_59_777,"[MMIDC] HandleDcStorageOptWinMsg OK sys_dev=%d,mode=%d,cur_item=%d")
TRACE_MSG(MMIDC_WINDOW_OPTION_1030_112_2_18_2_10_0_778,"[MMIDC] AddItemToSetlist max_dev_number=%d, cur_dev_item=%d, dc_dev=%d")
TRACE_MSG(MMIDC_WORKMODE_75_112_2_18_2_10_2_779,"[MMIDC] AllocWorkInfo s_dc_work_info_ptr=0x%X")
TRACE_MSG(MMIDC_WORKMODE_87_112_2_18_2_10_2_780,"[MMIDC] FreeWorkInfo s_dc_work_info_ptr=0x%X")
TRACE_MSG(MMIDC_WORKMODE_144_112_2_18_2_10_2_781,"[MMIDC] InitWorkInfoParam ret=%d")
TRACE_MSG(MMIDC_WORKMODE_160_112_2_18_2_10_2_782,"[MMIDC] MMIDC_ReleaseWorkMode")
TRACE_MSG(MMIDC_WORKMODE_203_112_2_18_2_10_2_783,"[MMIDC] MMIAPIDC_StartWork ret=%d")
TRACE_MSG(MMIDC_WORKMODE_222_112_2_18_2_10_2_784,"[MMIDC] MMIDC_IsWorkMode ret=%d")
TRACE_MSG(MMIDC_WORKMODE_350_112_2_18_2_10_3_785,"[MMIDC] ConvertWorkPhotoSizeToDCPhotoSize photo_size=%d")
TRACE_MSG(MMIDC_WORKMODE_369_112_2_18_2_10_3_786,"[MMIDC] MMIDC_GetPhotoSizeOfWorkMode photo_size=%d")
TRACE_MSG(MMIDC_WORKMODE_469_112_2_18_2_10_3_787,"[MMIDC] ConvertWorkVideoSizeToDCPhotoSize video_size=%d")
TRACE_MSG(MMIDC_WORKMODE_488_112_2_18_2_10_3_788,"[MMIDC] MMIDC_GetVideoSizeOfWorkMode video_size=%d")
TRACE_MSG(MMIDC_WORKMODE_554_112_2_18_2_10_3_789,"[MMIDC] MMIDC_GetMaxPhotoSizeOfWorkMode max_photo_size=%d")
TRACE_MSG(MMIDC_WORKMODE_621_112_2_18_2_10_3_790,"[MMIDC] MMIDC_SaveWorkFile is_succes=%d")
TRACE_MSG(MMIDC_WORKMODE_654_112_2_18_2_10_3_791,"[MMIDC] MMIDC_WorkCallback ret=%d")
TRACE_MSG(MMIDC_WORKMODE_674_112_2_18_2_10_3_792,"[MMIDC] MMIDC_GetMaxVideoSizeOfWorkMode max_video_size=%d")
TRACE_MSG(MMIDC_WORKMODE_694_112_2_18_2_10_3_793,"[MMIDC] MMIDC_GetLimitedVideoFileSizeOfWorkMode file_size=%d")
TRACE_MSG(MMIDV_MANAGER_87_112_2_18_2_10_4_794,"[MMIRecord]: OpenDV = %d")
TRACE_MSG(MMIDV_MANAGER_91_112_2_18_2_10_4_795,"[MMIRecord]: DRECORDER_SelectSrcDevice = %d")
TRACE_MSG(MMIDV_MANAGER_93_112_2_18_2_10_4_796,"[MMIRecord]: DRECORDER_Open = %d")
TRACE_MSG(MMIDV_MANAGER_126_112_2_18_2_10_4_797,"[MMIRecord]: StartDVPreview = %d, = %d")
TRACE_MSG(MMIDV_MANAGER_133_112_2_18_2_10_4_798,"[MMIRecord]: DRECORDER_StartPreview = %d")
TRACE_MSG(MMIDV_MANAGER_161_112_2_18_2_10_4_799,"[MMIRecord]: InitDVPreviewParam record_param_ptr is PNULL !!!")
TRACE_MSG(MMIDV_MANAGER_173_112_2_18_2_10_4_800,"[MMIdv_manager]:  cur_angle = %d")
TRACE_MSG(MMIDV_MANAGER_179_112_2_18_2_10_4_801,"[MMIdv_manager]:  GUILCD_GetLogicWidthHeight, result = %d")
TRACE_MSG(MMIDV_MANAGER_191_112_2_18_2_10_4_802,"[MMIdv_manager]:  GUILCD_ConvertLogicToPhysicalCoordinate, result = %d")
TRACE_MSG(MMIDV_MANAGER_209_112_2_18_2_10_4_803,"[MMIdv_manager]:  record_param_ptr->disp_rect :x = %d, y = %d, w = %d, h = %d")
TRACE_MSG(MMIDV_MANAGER_216_112_2_18_2_10_4_804,"[MMIdv_manager]: InitDVPreviewParam")
TRACE_MSG(MMIDV_MANAGER_229_112_2_18_2_10_4_805,"[MMIRecord]: StopPreview = %d, = %d")
TRACE_MSG(MMIDV_MANAGER_234_112_2_18_2_10_4_806,"[MMIRecord]: DRECORDER_StopPreview = %d")
TRACE_MSG(MMIDV_MANAGER_252_112_2_18_2_10_4_807,"[MMIRecord]: CloseDV = %d")
TRACE_MSG(MMIDV_MANAGER_259_112_2_18_2_10_4_808,"[MMIRecord]: DRECORDER_Close = %d")
TRACE_MSG(MMIDV_MANAGER_287_112_2_18_2_10_4_809,"[MMIRecord]: StartRecord start")
TRACE_MSG(MMIDV_MANAGER_291_112_2_18_2_10_4_810,"[MMIRecord]: StartRecord start filename is PNULL !!!")
TRACE_MSG(MMIDV_MANAGER_326_112_2_18_2_10_4_811,"[MMIRecord]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d")
TRACE_MSG(MMIDV_MANAGER_331_112_2_18_2_10_4_812,"[MMIRecord]: DRECORDER_StartRecord = %d")
TRACE_MSG(MMIDV_MANAGER_339_112_2_18_2_10_4_813,"[MMIRecord]: StartRecord - return success !")
TRACE_MSG(MMIDV_MANAGER_392_112_2_18_2_10_4_814,"[MMIRecord]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d")
TRACE_MSG(MMIDV_MANAGER_407_112_2_18_2_10_4_815,"[MMIRecord]: DRECORDER_StopRecord return: %d")
TRACE_MSG(MMIDV_MANAGER_437_112_2_18_2_10_4_816,"[MMIRecord]: DRECORDER_StopRecord return: %d")
END_TRACE_MAP(MMI_APP_CAMERA_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_CAMERA_TRC_H_

