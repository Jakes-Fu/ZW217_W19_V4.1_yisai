/******************************************************************************
 ** File Name:      ms_ref_image_proc_trc.h                                         *
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
//trace_id:53
#ifndef _MS_REF_IMAGE_PROC_TRC_H_
#define _MS_REF_IMAGE_PROC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define BMP_APP_224_112_2_18_1_34_9_0 "bmp_rotate_180:\n"
#define BMP_APP_267_112_2_18_1_34_9_1 "BMP Decode: bmp_param_check:\n"
#define BMP_APP_628_112_2_18_1_34_9_2 "BMPAPP_BmpDecode:rect top %d, bottom %d, left %d,right %d"
#define BMP_APP_629_112_2_18_1_34_9_3 "BMPAPP_BmpDecode:cropwidth %d, cropheight %d"
#define BMP_APP_706_112_2_18_1_34_10_4 "BMPAPP_BmpDecode:p_input_info.input_width %d, p_input_info.input_height  %d"
#define BMP_APP_768_112_2_18_1_34_10_5 "BMPAPP_BmpDecode: Decoding Succeed"
#define BMP_APP_769_112_2_18_1_34_10_6 "BMPAPP_BmpDecode: End Target W: %d, H:%d  "
#define BMP_APP_770_112_2_18_1_34_10_7 "BMPAPP_BmpDecode: End Src W: %d, H:%d  "
#define BMP_APP_771_112_2_18_1_34_10_8 "BMPAPP_BmpDecode: End bit W: %d,"
#define BMP_APP_775_112_2_18_1_34_10_9 "BMPAPP_BmpDecode: Decoding Fail,Bad file"
#define BMP_DECODE_2010_112_2_18_1_34_16_10 "BMP_Decoder_Init:\n"
#define BMP_DECODE_2247_112_2_18_1_34_16_11 "destroy_bmp_decoder:\n"
#define BMP_DECODE_2282_112_2_18_1_34_16_12 "BMP: BMP_Get_No_Scaling_Data in rgb_ptr:%d , line count: %d"
#define BMP_DECODE_2289_112_2_18_1_34_16_13 "BMP: BMP_Get_No_Scaling_Data bad BMP out"
#define BMP_DECODE_2339_112_2_18_1_34_16_14 "BMP_Get_Data:\n"
#define BMP_DECODE_2353_112_2_18_1_34_17_15 "Error happen"
#define BMP_TASK_372_112_2_18_1_34_18_16 "BMP_DEC_TaskClose1"
#define BMP_TASK_401_112_2_18_1_34_18_17 "BMP_DEC_TaskClose3: %d"
#define BMP_TASK_404_112_2_18_1_34_18_18 "BMP_DEC_TaskClose4: %d"
#define BMP_TASK_482_112_2_18_1_34_18_19 "_BMP_DECODER_SetEvent+"
#define BMP_TASK_488_112_2_18_1_34_18_20 "_BMP_DECODER_SetEvent-"
#define BMP_TASK_518_112_2_18_1_34_18_21 "BMPAPP_GetFrame mid +"
#define BMP_TASK_535_112_2_18_1_34_18_22 "BMPAPP_GetFrame mid"
#define BMP_TASK_556_112_2_18_1_34_18_23 "BMPAPP_GetFrame -"
#define BMP_TASK_874_112_2_18_1_34_19_24 "[IMGDECbmp_Dec] bytes_written = %d"
#define BMP_TASK_878_112_2_18_1_34_19_25 "[IMGDECbmp_Dec] create file failed"
#define BMP_TASK_882_112_2_18_1_34_19_26 "IMGDECbmp_Dec, target_ptr= %x, jpeg_size= %d"
#define BMP_TASK_883_112_2_18_1_34_19_27 "IMGDECbmp_Dec, target_width = %d, target_height = %d"
#define BMP_TASK_884_112_2_18_1_34_19_28 "IMGDECbmp_Dec,out_param.real_width = %d, out_param.real_height = %d"
#define BMP_TASK_994_112_2_18_1_34_19_29 "_BMP_DEC_Task_Routine bmp_handle: %d +1"
#define BMP_TASK_1004_112_2_18_1_34_19_30 "_BMP_DEC_Task_Routine bmp_handle: %d -1"
#define BMP_TASK_1023_112_2_18_1_34_19_31 "BMP_DECODER_Task_Init: s_bmp_decoder_thread_id = %d+"
#define BMP_TASK_1051_112_2_18_1_34_19_32 "BMP_DECODER_Task_Init: s_bmp_decoder_thread_id = %d  ddd"
#define BMP_TASK_1061_112_2_18_1_34_19_33 "BMP_DECODER_Task_Init: s_bmp_decoder_thread_id = %d-"
#define BMP_TASK_1090_112_2_18_1_34_19_34 "BMPAPP_WBMP_GetInfo: width header_count:%d"
#define BMP_TASK_1108_112_2_18_1_34_20_35 "BMPAPP_WBMP_GetInfo: height header_count:%d"
#define WBMP_DECODE_43_112_2_18_1_34_20_36 "wbmp_init:\n"
#define WBMP_DECODE_136_112_2_18_1_34_20_37 "wbmp_init: width header_count:%d"
#define WBMP_DECODE_154_112_2_18_1_34_20_38 "wbmp_init: height header_count:%d"
#define WBMP_DECODE_178_112_2_18_1_34_20_39 "wbmp_init: imgsize:%d, img_width:%d, img_height:%d"
#define WBMP_DECODE_429_112_2_18_1_34_21_40 "WBMPDecode:line_count: %d\n"
#define WBMP_DECODE_451_112_2_18_1_34_21_41 "WBMPDecode:line_count: %d\n"
#define WBMP_DECODE_555_112_2_18_1_34_21_42 "WBMPDecode_NoScaling:line_count: %d\n"
#define IMG_CASCADE_126_112_2_18_1_34_21_43 "TIM_TEST: _Get_MatchedPoint  %d, %d, %d"
#define IMG_CASCADE_187_112_2_18_1_34_22_44 "TIM_TEST: _Img_Combine offset :%d, %d, %d, %d"
#define IMG_CASCADE_295_112_2_18_1_34_22_45 "TIM_TEST: _Img_PanoramaJointTwoImg addr :0x%x, 0x%x"
#define IMG_CASCADE_345_112_2_18_1_34_22_46 "TIM_TEST: _Img_PanoramaJointTwoImg y  %d"
#define IMG_CASCADE_376_112_2_18_1_34_22_47 "TIM_TEST: _Img_PanoramaJointTwoImg x :%d"
#define IMG_CASCADE_410_112_2_18_1_34_22_48 "TIM_TEST: _Img_PanoramaJointTwoImg y :%d"
#define IMG_CASCADE_582_112_2_18_1_34_23_49 "TIM_TEST: Img_PanoramaMosaic output :0x%x, 0x%x, %d, %d, %d"
#define BALLOON_106_112_2_18_1_34_23_50 "bd_vel.x: %d, bd_vel.y:%d"
#define BALLOON_170_112_2_18_1_34_23_51 "draw circle body pos coordinate, p.x:%f, p.y: %f"
#define BALLOON_265_112_2_18_1_34_23_52 "draw circle body pos coordinate, p.x:%f, p.y: %f"
#define DRAWSPACE_577_112_2_18_1_35_9_53 "drawObject, rot x:%d, y: %d"
#define DRAWSPACE_597_112_2_18_1_35_9_54 "drawPolyShape%d:x%d,y%d"
#define DRAWSPACE_615_112_2_18_1_35_9_55 "drawPolyShape:max_x: %d"
#define PLINK_360_112_2_18_1_35_17_56 "chipmunk: 0x%x"
#define IMAGE_ALGORITHM_145_112_2_18_1_35_42_57 "DC: JIALIAN rotation 270"
#define IMAGE_ALGORITHM_208_112_2_18_1_35_42_58 "DC: JIALIAN angle%d"
#define IMAGE_ALGORITHM_232_112_2_18_1_35_42_59 "[Img_RotateYUV422_Y_UV:run default of switch!]"
#define DC_ALGORITHM_447_112_2_18_1_35_44_60 "TIM:_Get_JpegSliceEncoderIram,ilock handle 0x%x,0x%x,0x%x"
#define DC_ALGORITHM_677_112_2_18_1_35_44_61 "DCAM: DC_ArgbOverlayer src_buf_rect y=%d"
#define DC_ALGORITHM_678_112_2_18_1_35_44_62 "DCAM: DC_ArgbOverlayer src_buf_rect h=%d"
#define DC_ALGORITHM_679_112_2_18_1_35_44_63 "DCAM: DC_ArgbOverlayer overlay_rect.y=%d"
#define DC_ALGORITHM_680_112_2_18_1_35_44_64 "DCAM: DC_ArgbOverlayer overlay_rect.h=%d"
#define DC_ALGORITHM_687_112_2_18_1_35_44_65 "DCAM: DC_ArgbOverlayer error src_addr:x=%d, w=%d"
#define DC_ALGORITHM_714_112_2_18_1_35_44_66 "DCAM: DC_ArgbOverlayer src_buf_rect x =%d y=%d"
#define DC_ALGORITHM_715_112_2_18_1_35_44_67 "DCAM: DC_ArgbOverlayer src_buf_rect w =%d h=%d"
#define DC_ALGORITHM_716_112_2_18_1_35_44_68 "DCAM: DC_ArgbOverlayer overlay_rect.x =%d y=%d"
#define DC_ALGORITHM_717_112_2_18_1_35_44_69 "DCAM: DC_ArgbOverlayer overlay_rect.w =%d h=%d"
#define DC_ALGORITHM_956_112_2_18_1_35_45_70 "DCAM: ROTATION IS NOT SUPPORTTED!"
#define DC_ALGORITHM_1028_112_2_18_1_35_45_71 "DCAM capture rotation buf is not enough need_buf=0x%x,rot_buf=0x%x"
#define EXIF_APP_337_112_2_18_1_35_46_72 "SplitExif_app: pThumboffset= %d, ret: %d"
#define EXIF_APP_338_112_2_18_1_35_46_73 "SplitExif_app 1 = 0x%x"
#define EXIF_APP_339_112_2_18_1_35_46_74 "SplitExif_app 2 = 0x%x"
#define EXIF_APP_340_112_2_18_1_35_46_75 "SplitExif_app 2 = 0x%x"
#define EXIFPARSER_442_112_2_18_1_35_52_76 "_JP_ExifAPPnParse1_  :find thumbnail"
#define EXIFPARSER_480_112_2_18_1_35_52_77 "_JP_ExifAPPnParse1_: err = %d"
#define EXIFPARSER_485_112_2_18_1_35_52_78 "_JP_ExifAPPnParse1_  Memcpy Thumbnail"
#define EXIFPARSER_1153_112_2_18_1_35_53_79 " _JP_ExifSOFParse: : isThumb: %d ,pdata[0x0B] = %d"
#define EXIFPARSER_1470_112_2_18_1_35_54_80 "EXIF _API_ExifParamParse Next AppLen = %d"
#define EXIFPARSER_1486_112_2_18_1_35_54_81 "EXIF _API_ExifParamParse Next ADDR = %d"
#define GIF_APP_CTRL_248_112_2_18_1_35_55_82 "data format is invailid: format is %d"
#define GIF_APP_CTRL_357_112_2_18_1_35_55_83 "_GIFDEC_ReadFile:read file error!"
#define GIF_APP_CTRL_382_112_2_18_1_35_55_84 "s_is_continue_decode_flag=%d,time=%d"
#define GIF_APP_CTRL_708_112_2_18_1_35_56_85 "GIFDEC: Recieve Sig=0x%x, param addr=0x%x , cur time=%d"
#define GIF_APP_CTRL_770_112_2_18_1_35_56_86 "[IMGDECGIF_Dec] bytes_written = %d"
#define GIF_APP_CTRL_774_112_2_18_1_35_56_87 "[IMGDECGIF_Dec] create file failed"
#define GIF_APP_CTRL_779_112_2_18_1_35_56_88 "IMGDECGIF_Dec, target_ptr = %x, jpeg_size = %d"
#define GIF_APP_CTRL_780_112_2_18_1_35_56_89 "IMGDECGIF_Dec, target_width = %d, target_height = %d"
#define GIF_APP_CTRL_781_112_2_18_1_35_56_90 "IMGDECGIF_Dec,out_param.real_width = %d, out_param.real_height = %d"
#define GIF_APP_CTRL_812_112_2_18_1_35_56_91 "gif dec finish a file time=%d,handle=0x%x"
#define GIF_APP_CTRL_843_112_2_18_1_35_56_92 "--_GIFDEC_Decoder:is_exist_background=%d---"
#define GIF_APP_CTRL_1014_112_2_18_1_35_57_93 "IMGDECGIF_Create,time=%d"
#define GIF_APP_CTRL_1063_112_2_18_1_35_57_94 "IMGDECGIF_Create,out_time=%d,handle=0x%x"
#define GIF_APP_CTRL_1085_112_2_18_1_35_57_95 "IMGDECGIF_Destroy handle=0x%x,s_gif_handle=0x%x,time=%d"
#define GIF_APP_CTRL_1155_112_2_18_1_35_57_96 "IMGDECGIF_Destroy out time=%d"
#define GIF_APP_CTRL_1184_112_2_18_1_35_57_97 "IMGDECGIF_GetFrame handle=0x%x,callback=0x%x,time=%d,is_exit_back=%d"
#define GIF_APP_CTRL_1191_112_2_18_1_35_57_98 "IMGDECGIF_GetFrame:data format is error! %d"
#define GIF_APP_CTRL_1276_112_2_18_1_35_57_99 "IMGDECGIF_GetFrame handle=0x%x,out time=%d"
#define GIF_DECODE_APP_58_112_2_18_1_35_58_100 "GIFDECODEAPP_FreeDecodeRes in"
#define GIF_DECODE_APP_86_112_2_18_1_35_58_101 "GIFDECODEAPP_FreeDecodeRes Out"
#define GIF_DECODE_APP_104_112_2_18_1_35_58_102 "GIFDECODEAPP_FreeScaleRes"
#define GIF_DECODE_APP_463_112_2_18_1_35_59_103 "Error file"
#define GIF_DECODE_APP_476_112_2_18_1_35_59_104 "Error file"
#define GIF_DECODE_APP_608_112_2_18_1_35_59_105 "get_gif_no_scaling_frame: No Scaling line_total =  %d"
#define GIF_DECODE_APP_857_112_2_18_1_35_59_106 "gif crop parameter err crop_width = %d"
#define GIF_DECODE_APP_864_112_2_18_1_35_59_107 "gif crop parameter err crop_height = %d"
#define GIF_DECODE_APP_886_112_2_18_1_36_0_108 "gif_size_handle: first frame sample %d, %d"
#define GIF_DECODE_APP_1013_112_2_18_1_36_0_109 "gif crop parameter err crop_width = %d"
#define GIF_DECODE_APP_1020_112_2_18_1_36_0_110 "gif crop parameter err crop_height = %d"
#define GIF_DECODE_APP_1042_112_2_18_1_36_0_111 "gif_size_handle_non_first_frame: first frame sample %d, %d"
#define GIF_DECODE_APP_1126_112_2_18_1_36_0_112 "format is error!"
#define GIF_DECODE_APP_1132_112_2_18_1_36_0_113 "--_get_gif_frame_with_scale:get first frame!---"
#define GIF_DECODE_APP_1179_112_2_18_1_36_0_114 "_get_gif_frame_with_scale: target %d, %d, src %d, %d"
#define GIF_DECODE_APP_1255_112_2_18_1_36_0_115 "_get_gif_frame_with_scale:target change target %d, %d, src %d, %d"
#define GIF_DECODE_APP_1305_112_2_18_1_36_0_116 "_get_gif_frame_with_scale: target %d, %d, src %d, %d"
#define GIF_DECODE_APP_1329_112_2_18_1_36_1_117 "_get_gif_frame_with_scale:start decode! "
#define GIF_DECODER_67_112_2_18_1_36_1_118 "--_set_current_decoder_handle:0x%x--"
#define GIF_DECODER_673_112_2_18_1_36_2_119 "gif_data_compound: in!"
#define GIF_DECODER_806_112_2_18_1_36_3_120 "gif_data_compound: out!"
#define GIF_DECODER_825_112_2_18_1_36_3_121 "gif_data_compound_argb_noscale: in!"
#define GIF_DECODER_955_112_2_18_1_36_3_122 "gif_data_compound_argb_noscale: out!"
#define GIF_DECODER_987_112_2_18_1_36_3_123 "gif_data_compound: in!"
#define GIF_DECODER_1110_112_2_18_1_36_3_124 "gif_data_compound: out!"
#define GIF_DECODER_1429_112_2_18_1_36_4_125 "out_line interleave:%d,%d"
#define GIF_DECODER_1962_112_2_18_1_36_5_126 "seek_next_image: return "
#define GIF_DECODER_2056_112_2_18_1_36_5_127 "IMG_PROC_gif_decode_init:src %d,%d"
#define GIF_DECODER_2088_112_2_18_1_36_5_128 "GIF decode buf size too small!"
#define GIF_DECODER_2096_112_2_18_1_36_5_129 "GIF decode buf size too small!"
#define GIF_DECODER_2164_112_2_18_1_36_5_130 "===IMG_PROC_gif_decode_init:is_exist_background=%d===="
#define GIF_DECODER_2251_112_2_18_1_36_5_131 "get_gif_data image %d,%d,%d,%d"
#define GIF_DECODER_2260_112_2_18_1_36_6_132 "No Support this file !"
#define GIF_DECODER_2341_112_2_18_1_36_6_133 "get_gif_data bad code"
#define GIF_DECODER_2502_112_2_18_1_36_6_134 "IMG_PROC_GIF_Decode_Get_Data return line%d"
#define GIF_DECODER_2599_112_2_18_1_36_6_135 "[IMG_PROC_GIF_Decode_Get_Data:state err!]"
#define GIF_DECODER_2636_112_2_18_1_36_6_136 "IMG_PROC_GIF_Decode_Get_Data return line%d"
#define GIF_REENTRANT_DECODER_454_112_2_18_1_36_7_137 "----GIFRES_Dec point 0----"
#define GIF_REENTRANT_DECODER_478_112_2_18_1_36_7_138 "----Error file---"
#define GIF_REENTRANT_DECODER_534_112_2_18_1_36_8_139 "----GIFRES_Dec point 3----"
#define GIF_REENTRANT_DECODER_571_112_2_18_1_36_8_140 "----Error file---"
#define GIF_REENTRANT_DECODER_631_112_2_18_1_36_8_141 "---DEC_MALLOC_FAIL---"
#define GIF_REENTRANT_DECODER_1416_112_2_18_1_36_9_142 "seek_next_image: return : %d"
#define GIF_REENTRANT_DECODER_1431_112_2_18_1_36_9_143 "seek_next_image: return "
#define IMG_GZIP_DEFLATE_634_112_2_18_1_36_12_144 "deflate_fast:%c"
#define IMG_GZIP_DEFLATE_736_112_2_18_1_36_12_145 "deflate: %c"
#define IMG_GZIP_GZIP_559_112_2_18_1_36_13_146 "unknown method %d"
#define IMG_GZIP_GZIP_568_112_2_18_1_36_13_147 "encrypted -- get newer version of gzip!"
#define IMG_GZIP_GZIP_574_112_2_18_1_36_13_148 "multi-part gzip file -- get newer version of gzip"
#define IMG_GZIP_GZIP_581_112_2_18_1_36_13_149 "get newer version of gzip"
#define IMG_GZIP_GZIP_612_112_2_18_1_36_13_150 "extra field of %u bytes ignored"
#define IMG_GZIP_GZIP_690_112_2_18_1_36_13_151 "not in gzip format"
#define IMG_GZIP_GZIP_694_112_2_18_1_36_13_152 "decompression OK, trailing garbage ignored"
#define IMG_GZIP_LZW_21_112_2_18_1_36_16_153 "output in compress .Z format not supported"
#define IMG_GZIP_UNLZW_232_112_2_18_1_36_20_154 "warning, unknown flags 0x%x"
#define IMG_GZIP_UNLZW_244_112_2_18_1_36_20_155 "compressed with %d bits, can only handle %d bits"
#define IMG_GZIP_UNZIP_145_112_2_18_1_36_22_156 "unzip:len %ld, siz %d"
#define IMG_GZIP_UNZIP_211_112_2_18_1_36_22_157 "has more than one entry--rest ignored"
#define IMG_GZIP_UNZIP_217_112_2_18_1_36_22_158 "has more than one entry -- unchanged"
#define IMG_GZIP_UTIL_372_112_2_18_1_36_23_159 "error %s"
#define IMG_GZIP_UTIL_384_112_2_18_1_36_23_160 "warning: %s%s"
#define IMG_GZIP_UTIL_397_112_2_18_1_36_23_161 "unexpected end of file"
#define ICO_DECODE_366_112_2_18_1_36_24_162 "IMG_DEC_GetInfo: Can not open file "
#define IMG_DEC_HAL_85_112_2_18_1_36_27_163 "_IMG_DEC_Get_Caller_Priority: %s, queue_name: %s, priority: %d "
#define IMG_DEC_HAL_111_112_2_18_1_36_28_164 "_IMG_DEC_Get_Caller_Priority: %s, queue_name: %s, priority: %d "
#define IMG_DEC_HAL_121_112_2_18_1_36_28_165 "IMG_DEC_ALLOC size = %d, addr = 0x%x"
#define IMG_DEC_HAL_135_112_2_18_1_36_28_166 "IMG_DEC_FREE data_ptr = 0x%x"
#define IMG_DEC_HAL_230_112_2_18_1_36_28_167 "IMG_DEC_GetInfo_Detail: Unkown image type"
#define IMG_DEC_INTERFACE_154_112_2_18_1_36_29_168 "IMG_DEC_GetInfo: Can not open file "
#define IMG_DEC_INTERFACE_166_112_2_18_1_36_29_169 "IMG_DEC_GetInfo: unknown image type "
#define IMG_DEC_INTERFACE_237_112_2_18_1_36_29_170 "[IMG_DEC_Create +]"
#define IMG_DEC_INTERFACE_245_112_2_18_1_36_29_171 "[IMG_DEC_Create -] handle = 0x%x"
#define IMG_DEC_INTERFACE_265_112_2_18_1_36_29_172 "[IMG_DEC_GetFrame +], handle = 0x%x, callback = 0x%x"
#define IMG_DEC_INTERFACE_297_112_2_18_1_36_29_173 "[IMG_DEC_GetFrame -], handle = 0x%x, callback = 0x%x , app_param_ptr = 0x%x  app_param_size=%d"
#define IMG_DEC_INTERFACE_312_112_2_18_1_36_29_174 "[IMG_DEC_Destroy +] handle = 0x%x"
#define IMG_DEC_INTERFACE_328_112_2_18_1_36_29_175 "[IMG_DEC_Destroy -] handle = 0x%x"
#define IMG_DEC_INTERFACE_474_112_2_18_1_36_29_176 "0framecallback"
#define IMG_DEC_TASK_148_112_2_18_1_36_30_177 "_IMG_DEC_Set_Priority, priority: %d"
#define IMG_DEC_TASK_228_112_2_18_1_36_30_178 "_IMG_DEC_Get_Caller_Priority: %s, queue_name: %s, priority: %d "
#define IMG_DEC_TASK_253_112_2_18_1_36_30_179 "_IMG_DEC_Get_Caller_Priority: %s, queue_name: %s, priority: %d "
#define IMG_DEC_TASK_281_112_2_18_1_36_30_180 "[_IMG_DEC_Dec_Command_Reg +] handle = 0x%x"
#define IMG_DEC_TASK_307_112_2_18_1_36_31_181 "[_IMG_DEC_Dec_Command_Reg]: can not find handle = 0x%x"
#define IMG_DEC_TASK_316_112_2_18_1_36_31_182 "_IMG_DEC_Dec_Command_Reg: Running CMD++, handle: 0x%x(For runing handler,  not permit to set runing again)"
#define IMG_DEC_TASK_367_112_2_18_1_36_31_183 "[_IMG_DEC_Dec_Command_Reg -] handle = 0x%x"
#define IMG_DEC_TASK_384_112_2_18_1_36_31_184 "[_IMG_DEC_Dec_Handler_Reg +] handle = 0x%x"
#define IMG_DEC_TASK_430_112_2_18_1_36_31_185 "[_IMG_DEC_Dec_Handler_Reg -] handle = 0x%x"
#define IMG_DEC_TASK_431_112_2_18_1_36_31_186 "[_IMG_DEC_Dec_Handler_Reg -] app_handle = 0x%x"
#define IMG_DEC_TASK_453_112_2_18_1_36_31_187 "[_IMG_DEC_Find_Ready_Command +]"
#define IMG_DEC_TASK_457_112_2_18_1_36_31_188 "[_IMG_DEC_Find_Ready_Command] running_cmd_ptr is not null: Running app Handle:0x%x"
#define IMG_DEC_TASK_473_112_2_18_1_36_31_189 "[_IMG_DEC_Find_Ready_Command], find handle = 0x%x"
#define IMG_DEC_TASK_482_112_2_18_1_36_31_190 "_IMG_DEC_Find_Ready_Command: set running_cmd_ptr to 0x%x"
#define IMG_DEC_TASK_512_112_2_18_1_36_31_191 "_IMG_DEC_Find_Ready_Command: set running_cmd_ptr to PNULL"
#define IMG_DEC_TASK_523_112_2_18_1_36_31_192 "[_IMG_DEC_Find_Ready_Command], get frame, handle = 0x%x callback = 0x%x"
#define IMG_DEC_TASK_530_112_2_18_1_36_31_193 "[_IMG_DEC_Find_Ready_Command], get frame, handle = 0x%x callback = 0x%x"
#define IMG_DEC_TASK_551_112_2_18_1_36_31_194 "[_IMG_DEC_Find_Ready_Command -]"
#define IMG_DEC_TASK_564_112_2_18_1_36_31_195 "[_IMG_DEC_Callback], handle = 0x%x, frame index = %d"
#define IMG_DEC_TASK_595_112_2_18_1_36_31_196 "[_IMG_DEC_End_Dec +] handle = 0x%x"
#define IMG_DEC_TASK_606_112_2_18_1_36_31_197 "_IMG_DEC_End_Dec: set running_cmd_ptr to PNULL"
#define IMG_DEC_TASK_627_112_2_18_1_36_31_198 "[_IMG_DEC_End_Dec -] handle = 0x%x frame index = %d"
#define IMG_DEC_TASK_631_112_2_18_1_36_31_199 "[_IMG_DEC_End_Dec -] ERROR!"
#define IMG_DEC_TASK_636_112_2_18_1_36_31_200 "[_IMG_DEC_End_Dec -] running_cmd_ptr is PNULL"
#define IMG_DEC_TASK_658_112_2_18_1_36_31_201 "[_IMG_DEC_Remove_Command +] handle = 0x%x"
#define IMG_DEC_TASK_693_112_2_18_1_36_31_202 "_IMG_DEC_Remove_Command: set running_cmd_ptr to PNULL"
#define IMG_DEC_TASK_741_112_2_18_1_36_32_203 "[_IMG_DEC_Remove_Command -] handle = 0x%x"
#define IMG_DEC_TASK_786_112_2_18_1_36_32_204 "_IMG_DEC_Init_Handle: open file failed "
#define IMG_DEC_TASK_804_112_2_18_1_36_32_205 "_IMG_DEC_Init_Handle: get image info failed "
#define IMG_DEC_TASK_835_112_2_18_1_36_32_206 "[_GetMsg +] sig = 0x%x\n"
#define IMG_DEC_TASK_841_112_2_18_1_36_32_207 "[SIG: IMG_DEC_CMD_END +]"
#define IMG_DEC_TASK_846_112_2_18_1_36_32_208 "[SIG: IMG_DEC_CMD_END -]"
#define IMG_DEC_TASK_857_112_2_18_1_36_32_209 "[SIG: IMG_DEC_CMD_HALT +]"
#define IMG_DEC_TASK_859_112_2_18_1_36_32_210 "[SIG: IMG_DEC_CMD_HALT -]"
#define IMG_DEC_TASK_874_112_2_18_1_36_32_211 "[SIG: IMG_DEC_CMD_REG_CREATE_CMD +]"
#define IMG_DEC_TASK_884_112_2_18_1_36_32_212 "[SIG: IMG_DEC_CMD_REG_CREATE_CMD:malloc handle error!]"
#define IMG_DEC_TASK_894_112_2_18_1_36_32_213 "[SIG: IMG_DEC_CMD_REG_CREATE_CMD -]"
#define IMG_DEC_TASK_906_112_2_18_1_36_32_214 "[SIG: IMG_DEC_CMD_REG_DECODE_CMD +]"
#define IMG_DEC_TASK_932_112_2_18_1_36_32_215 "[SIG: IMG_DEC_CMD_REG_DECODE_CMD -]"
#define IMG_DEC_TASK_940_112_2_18_1_36_32_216 "[IMG DEC:invalid cmd]"
#define IMG_DEC_TASK_946_112_2_18_1_36_32_217 "_IMG_DEC_Task_Routine: command += %d"
#define IMG_DEC_TASK_959_112_2_18_1_36_32_218 "[_GetMsg -] sig = 0x%x\n"
#define IMG_DEC_TASK_995_112_2_18_1_36_32_219 "_IMG_DEC_SendMsg: sig_code = %d"
#define IMG_DEC_TASK_1035_112_2_18_1_36_32_220 "_IMG_DEC_Callback_SendMsg: sig_code = %d"
#define IMG_JPEG_DECODER_138_112_2_18_1_36_33_221 "[	JPEGDEC: _SetDecEvent] %d"
#define IMG_JPEG_DECODER_152_112_2_18_1_36_33_222 "[	JPEGDEC: _GetDecEvent] + , %d"
#define IMG_JPEG_DECODER_161_112_2_18_1_36_33_223 "[	JPEGDEC: _GetDecEvent] -, actual event = %d, status = %d"
#define IMG_JPEG_DECODER_181_112_2_18_1_36_33_224 "_GetResMutex +"
#define IMG_JPEG_DECODER_192_112_2_18_1_36_33_225 "_PutResMutex -"
#define IMG_JPEG_DECODER_265_112_2_18_1_36_33_226 "	ResMutex: _FreeAllDecMemory: free all decode buf and file"
#define IMG_JPEG_DECODER_306_112_2_18_1_36_33_227 "[	JPEGDEC: _SendMsg], sig_code = %d"
#define IMG_JPEG_DECODER_406_112_2_18_1_36_34_228 "JPEGDEC: _GetInfo s_jpeg_begin_offset = %d"
#define IMG_JPEG_DECODER_420_112_2_18_1_36_34_229 "	ResMutex: _GetInfo: alloc read buf"
#define IMG_JPEG_DECODER_433_112_2_18_1_36_34_230 "	ResMutex: _GetInfo: open file"
#define IMG_JPEG_DECODER_518_112_2_18_1_36_34_231 "JPEGDEC: unsupport data format"
#define IMG_JPEG_DECODER_532_112_2_18_1_36_34_232 "	JPEGDEC: _GetDecBufSize =%d"
#define IMG_JPEG_DECODER_578_112_2_18_1_36_34_233 "	ResMutex: _Decode: alloc dec buf"
#define IMG_JPEG_DECODER_593_112_2_18_1_36_34_234 "JPEGDEC: decode thumbnail jpeg"
#define IMG_JPEG_DECODER_611_112_2_18_1_36_34_235 "	ResMutex: _Decode: open file"
#define IMG_JPEG_DECODER_635_112_2_18_1_36_34_236 "	ResMutex: _Decode: alloc read buf"
#define IMG_JPEG_DECODER_710_112_2_18_1_36_34_237 "JPEGDEC: unsupport data format"
#define IMG_JPEG_DECODER_717_112_2_18_1_36_34_238 "JPEGDEC: _Decod s_jpeg_begin_offset = %d"
#define IMG_JPEG_DECODER_781_112_2_18_1_36_35_239 "JPEGDEC: unsupport data format"
#define IMG_JPEG_DECODER_801_112_2_18_1_36_35_240 "[	JPEGDEC: _JpegDecode +] handle = 0x%x"
#define IMG_JPEG_DECODER_820_112_2_18_1_36_35_241 "[	JPEGDEC: _JpegDecode -] handle = 0x%x"
#define IMG_JPEG_DECODER_853_112_2_18_1_36_35_242 "	ResMutex: _CreateMiniature: alloc dec buf"
#define IMG_JPEG_DECODER_866_112_2_18_1_36_35_243 "ResMutex: _CreateMiniature: open file"
#define IMG_JPEG_DECODER_890_112_2_18_1_36_35_244 "	ResMutex: _Decode: alloc read buf"
#define IMG_JPEG_DECODER_917_112_2_18_1_36_35_245 "JPEGDEC: _CreateMiniature read progressive jpeg to memory"
#define IMG_JPEG_DECODER_943_112_2_18_1_36_35_246 "JPEGDEC: _CreateMiniature s_jpeg_begin_offset = %d"
#define IMG_JPEG_DECODER_982_112_2_18_1_36_35_247 "[	JPEGDEC: _JpegCreateMiniature +] handle = 0x%x"
#define IMG_JPEG_DECODER_1002_112_2_18_1_36_35_248 "[	JPEGDEC: _JpegCreateMiniature -] handle = 0x%x"
#define IMG_JPEG_DECODER_1034_112_2_18_1_36_35_249 "[IMG_JPEG_DECODER: _TaskEntry], sig=0x%x,sig_code = %d"
#define IMG_JPEG_DECODER_1039_112_2_18_1_36_35_250 "[	JPEGDED: _GetMsg +] sig_code = %d, handle = 0x%x"
#define IMG_JPEG_DECODER_1057_112_2_18_1_36_35_251 "[_TaskEntry]+ SIG_DECODE, handle = 0x%x"
#define IMG_JPEG_DECODER_1059_112_2_18_1_36_35_252 "[_TaskEntry]- SIG_DECODE, handle = 0x%x, s_decode_count"
#define IMG_JPEG_DECODER_1063_112_2_18_1_36_35_253 "[_TaskEntry]+ SIG_CREATE_MINI, handle = 0x%x"
#define IMG_JPEG_DECODER_1065_112_2_18_1_36_35_254 "[_TaskEntry]- SIG_CREATE_MINI, handle = 0x%x"
#define IMG_JPEG_DECODER_1069_112_2_18_1_36_35_255 "[_TaskEntry] invalid signal"
#define IMG_JPEG_DECODER_1079_112_2_18_1_36_35_256 "[	JPEGDED: _GetMsg -] sig done"
#define IMG_JPEG_DECODER_1110_112_2_18_1_36_35_257 "[	JPEGDEC: _CreateTask +]: handle = 0x%x, create task, priority = %d"
#define IMG_JPEG_DECODER_1121_112_2_18_1_36_35_258 "[	JPEGDEC: _CreateTask +]: handle = 0x%x, task exist, priority = %d"
#define IMG_JPEG_DECODER_1135_112_2_18_1_36_35_259 "[	JPEGDEC: _CloseTask +]: handle = 0x%x"
#define IMG_JPEG_DECODER_1164_112_2_18_1_36_35_260 "[	JPEGDEC: _CloseTask -]: handle = 0x%x"
#define IMG_JPEG_DECODER_1173_112_2_18_1_36_35_261 "[	JPEGDEC: _SuspendTask +]: handle = 0x%x"
#define IMG_JPEG_DECODER_1184_112_2_18_1_36_35_262 "[	JPEGDEC: _SuspendTask -]: handle = 0x%x"
#define IMG_JPEG_DECODER_1195_112_2_18_1_36_35_263 "[	JPEGDEC_Init]"
#define IMG_JPEG_DECODER_1216_112_2_18_1_36_35_264 "[	JPEGDEC_CreateHandle +]"
#define IMG_JPEG_DECODER_1222_112_2_18_1_36_35_265 "[JPGDEC_CreateHandle] Create handle failed"
#define IMG_JPEG_DECODER_1237_112_2_18_1_36_35_266 "[	JPEGDEC_CreateHandle -] handle = 0x%x"
#define IMG_JPEG_DECODER_1259_112_2_18_1_36_35_267 "[	JPEGDEC_GetFrame +] handle = 0x%x, callback = 0x%x"
#define IMG_JPEG_DECODER_1263_112_2_18_1_36_35_268 "[JPGDEC_GetFrame] invalid handle"
#define IMG_JPEG_DECODER_1305_112_2_18_1_36_36_269 "[	JPEGDEC_GetFrame -] handle = 0x%x, callback = 0x%x"
#define IMG_JPEG_DECODER_1320_112_2_18_1_36_36_270 "[	JPEGDEC_DestoryHandle +] handle = 0x%x, callback = 0x%x, exit_type = %d"
#define IMG_JPEG_DECODER_1344_112_2_18_1_36_36_271 "[	JPEGDEC_DestoryHandle -] handle = 0x%x, callback = 0x%x, exit_type = %d"
#define IMG_ALG_COMMON_226_112_2_18_1_36_37_272 "IMG_PROC: _IMGPROC_GetChnLen format 0x%x"
#define IMG_ALG_COMMON_265_112_2_18_1_36_37_273 "IMG_PROC: _IMGPROC_GetChnLen format 0x%x"
#define IMG_ALG_FRAME_1225_112_2_18_1_36_43_274 "[SimpleFrmInit] the line is just a point"
#define IMG_ALG_FRAME_1266_112_2_18_1_36_43_275 "[SubFrame_Alloc] SubFrame Malloc Failed"
#define IMG_ALG_FRAME_1306_112_2_18_1_36_43_276 "[MaskFrameAlloc] MaskFrame Malloc Failed"
#define IMG_ALG_FRAME_1360_112_2_18_1_36_43_277 "[SimpleFrame Alloc] SimpleFrame Malloc Failed"
#define IMG_ALG_FRAME_1806_112_2_18_1_36_44_278 "[Frame_Blend_Mask] _BlendMask's input parameter is invalidate"
#define IMG_ALG_FRAME_3002_112_2_18_1_36_46_279 "[Frame_Blend_Simple_Create] input parameter is invalidate"
#define IMG_ALG_FRAME_3010_112_2_18_1_36_46_280 "[Frame_Blend_Simple_Create] this image data format is not support"
#define IMG_ALG_FRAME_3082_112_2_18_1_36_47_281 "[Frame_Blend_Simple_Blend] input parameter is invalidate"
#define IMG_ALG_FRAME_3087_112_2_18_1_36_47_282 "[Frame_Blend_Simple_Blend] image's width must be even"
#define IMG_ALG_FRAME_3157_112_2_18_1_36_47_283 "[Frame_Blend_Simple_Blend] input parameter is invalidate"
#define IMG_ALG_FRAME_3168_112_2_18_1_36_47_284 "[Frame_Blend_Simple_Blend] image's width must be even"
#define IMG_ALG_FRAME_3296_112_2_18_1_36_47_285 "[Frame_Blend_Simple_Free] input parameter is invalidate"
#define IMG_ALG_FRAME_3329_112_2_18_1_36_47_286 "[Frame_Blend_Mask_Create] input parameter is invalidate"
#define IMG_ALG_FRAME_3338_112_2_18_1_36_47_287 "[Frame_Blend_Mask_Create] this image data format is not support"
#define IMG_ALG_FRAME_3359_112_2_18_1_36_47_288 "[Frame_Blend_Mask] mask source address does not align"
#define IMG_ALG_FRAME_3406_112_2_18_1_36_47_289 "[Frame_Blend_Mask_Create: Mask Frame Rotate] Temp Bufs are not enough"
#define IMG_ALG_FRAME_3463_112_2_18_1_36_47_290 "Frame_MaskCreate: Scale temp buf failed"
#define IMG_ALG_FRAME_3496_112_2_18_1_36_47_291 "[Frame_Blend_Mask] input parameter is invalidate"
#define IMG_ALG_FRAME_3502_112_2_18_1_36_47_292 "[Frame_Blend_Mask] image's width must be even"
#define IMG_ALG_FRAME_3562_112_2_18_1_36_48_293 "[Frame_Blend_Mask_Create: Mask Frame Scale] Temp Bufs are not enough"
#define IMG_ALG_FRAME_3575_112_2_18_1_36_48_294 "[Frame_Blend_Mask] scale failed"
#define IMG_ALG_FRAME_3597_112_2_18_1_36_48_295 "[Frame_Blend_Mask] _BlendMask failed"
#define IMG_ALG_FRAME_3633_112_2_18_1_36_48_296 "[Frame_MaskBlendForPreview]: input parameter is invalidate"
#define IMG_ALG_FRAME_3644_112_2_18_1_36_48_297 "[Frame_MaskBlendForPreview]: image's width must be even"
#define IMG_ALG_FRAME_3714_112_2_18_1_36_48_298 "[Frame_MaskBlendForPreview]: scale failed"
#define IMG_ALG_FRAME_3776_112_2_18_1_36_48_299 "[Frame_Blend_Mask_Create] input parameter is invalidate"
#define IMG_ALG_FRAME_3784_112_2_18_1_36_48_300 "[Frame_Blend_Mask_Create] this image data format is not support"
#define IMG_ALG_FRAME_3834_112_2_18_1_36_48_301 "[Frame_Blend_Mixed_Blend] input parameter is invalidate"
#define IMG_ALG_FRAME_3840_112_2_18_1_36_48_302 "[Frame_Blend_Mixed_Blend] image's width must be even"
#define IMG_ALG_FRAME_3918_112_2_18_1_36_48_303 "[Frame_Blend_Mixed_Blend] input parameter is invalidate"
#define IMG_ALG_FRAME_3929_112_2_18_1_36_48_304 "[Frame_Blend_Mixed_Blend] image's width must be even"
#define IMG_ALG_FRAME_4007_112_2_18_1_36_48_305 "[Frame_Blend_Mixed_Free] input parameter is invalidate"
#define IMG_ALG_FRAME_4057_112_2_18_1_36_49_306 "[Frame_HandleCreate] Simple Frame create resource failed"
#define IMG_ALG_FRAME_4067_112_2_18_1_36_49_307 "[Frame_HandleCreate] Mask create resource failed"
#define IMG_ALG_FRAME_4077_112_2_18_1_36_49_308 "[Frame_HandleCreate] MixedFrame create resource failed"
#define IMG_ALG_FRAME_4108_112_2_18_1_36_49_309 "[Frame_Blend] input parameter is invalidate"
#define IMG_ALG_FRAME_4124_112_2_18_1_36_49_310 "[Frame_Blend] Simple Blend failed"
#define IMG_ALG_FRAME_4134_112_2_18_1_36_49_311 "[Frame_Blend] Mask Frame Blend failed"
#define IMG_ALG_FRAME_4144_112_2_18_1_36_49_312 "[Frame_Blend] Mixed Frame Blend failed"
#define IMG_ALG_FRAME_4162_112_2_18_1_36_49_313 "Preview is not support slice mode "
#define IMG_ALG_FRAME_4173_112_2_18_1_36_49_314 "[Frame_Blend] Simple Blend failed"
#define IMG_ALG_FRAME_4183_112_2_18_1_36_49_315 "[Frame_Blend] Mask Frame Blend failed"
#define IMG_ALG_FRAME_4193_112_2_18_1_36_49_316 "[Frame_Blend] Mixed Frame Blend failed"
#define IMG_ALG_FRAME_4227_112_2_18_1_36_49_317 "[Frame_HandleDestroy] input parameter is invalidate"
#define IMG_ALG_FRAME_4238_112_2_18_1_36_49_318 "[Frame_HandleDestroy] Simple destroy the resources failed"
#define IMG_ALG_FRAME_4248_112_2_18_1_36_49_319 "[Frame_HandleDestroy] Mask frame destroy the resources failed"
#define IMG_ALG_FRAME_4258_112_2_18_1_36_49_320 "[Frame_HandleDestroy] Mixed frame destroy the resources failed"
#define IMG_ALG_FRAME_4477_112_2_18_1_36_49_321 "IMG_PROC: _ImgFrame_Start context null"
#define IMG_ALG_FRAME_4483_112_2_18_1_36_49_322 "IMG_PROC: _ImgFrame_Start input param null"
#define IMG_ALG_FRAME_4510_112_2_18_1_36_49_323 "IMG_PROC: _ImgFrame_Start calling mode error"
#define IMG_ALG_PENCILSKETCH_598_112_2_18_1_36_51_324 "IMG_PROC: _pencilsketch: the src img addr 0x%x, img size w %d, h %d, format=%d"
#define IMG_ALG_PENCILSKETCH_612_112_2_18_1_36_51_325 "PencialSketch : dst format: %d"
#define IMG_ALG_PENCILSKETCH_613_112_2_18_1_36_51_326 "PencialSketch: src:%d,dst:%d"
#define IMG_PROC_408_112_2_18_1_36_53_327 "IMG_PROC: event _IMGPROC_SetEvent "
#define IMG_PROC_432_112_2_18_1_36_53_328 "IMG_PROC: event _IMGPROC_GetEvent "
#define IMG_PROC_538_112_2_18_1_36_53_329 "IMG_PROC: _IMGPROC_GetProcThreadState cur_handle_ptr null, %d line error"
#define IMG_PROC_559_112_2_18_1_36_53_330 "IMG_PROC: _IMGPROC_AllocBlock cur_handle_ptr null, %d line error"
#define IMG_PROC_605_112_2_18_1_36_53_331 "IMG_PROC: _IMGPROC_FreeBlock del buf num, %d line error"
#define IMG_PROC_861_112_2_18_1_36_54_332 "IMG_PROC: _IMGPROC_GetAlgCtrlFun alg exp info null, %d line error"
#define IMG_PROC_866_112_2_18_1_36_54_333 "IMG_PROC: _IMGPROC_GetAlgCtrlFun alg fun no exit, %d line error"
#define IMG_PROC_1020_112_2_18_1_36_54_334 "IMG_PROC: _IMGPROC_HandleInit is init"
#define IMG_PROC_1077_112_2_18_1_36_54_335 "IMG_PROC: _IMGPROC_CreateHandle alloc handle fail %d line error"
#define IMG_PROC_1656_112_2_18_1_36_55_336 "IMG_PROC: IMGPROC_DestroyHandle handle 0x%d is invalid"
#define IMG_PROC_1694_112_2_18_1_36_55_337 "IMG_PROC: _IMGPROC_AllHandleDestroy handle no exit"
#define IMG_PROC_1967_112_2_18_1_36_56_338 "IMG_PROC: _IMGPROC_SetProcessParam handle 0x%x"
#define IMG_PROC_2109_112_2_18_1_36_56_339 "IMG_PROC: _IMGPROC_AlgInit handle 0x%x"
#define IMG_PROC_2272_112_2_18_1_36_57_340 "IMG_PROC: _IMGPROC_ViewBeforeProcess handle 0x%x"
#define IMG_PROC_2316_112_2_18_1_36_57_341 "IMG_PROC: _IMGPROC_ViewBeforeProcess handle 0x%x"
#define IMG_PROC_2361_112_2_18_1_36_57_342 "IMG_PROC: _IMGPROC_ViewAfterProcess handle 0x%x"
#define IMG_PROC_2406_112_2_18_1_36_57_343 "IMG_PROC: _IMGPROC_AlgStart handle 0x%x"
#define IMG_PROC_2425_112_2_18_1_36_57_344 "IMG_PROC: _IMGPROC_AlgStart proc time :%d ms"
#define IMG_PROC_2434_112_2_18_1_36_57_345 "IMG_PROC: alg end rtn y_addr:0x%x, uv_addr:0x%x,"
#define IMG_PROC_2435_112_2_18_1_36_57_346 "IMG_PROC: alg end rtn w:%d, h:%d,"
#define IMG_PROC_2436_112_2_18_1_36_57_347 "IMG_PROC: alg end rtn format:0x%x,"
#define IMG_PROC_2478_112_2_18_1_36_57_348 "IMG_PROC: _IMGPROC_AlgDestroy handle 0x%x, force:%d"
#define IMG_PROC_2504_112_2_18_1_36_57_349 "IMG_PROC: _IMGPROC_CtrlStartProcess handle 0x%x"
#define IMG_PROC_2560_112_2_18_1_36_57_350 "IMG_PROC: _IMGPROC_ProcStartProcess handle: 0x%x"
#define IMG_PROC_2575_112_2_18_1_36_57_351 "IMG_PROC: _IMGPROC_ProcStartProcess proc_mode 0x%x "
#define IMG_PROC_2577_112_2_18_1_36_57_352 "IMG_PROC: _IMGPROC_ProcStartProcess ViewBefore0"
#define IMG_PROC_2610_112_2_18_1_36_57_353 "IMG_PROC: _IMGPROC_ProcStartProcess ViewBefore1"
#define IMG_PROC_2643_112_2_18_1_36_57_354 "IMG_PROC: _IMGPROC_ProcStartProcess AlgStart"
#define IMG_PROC_2698_112_2_18_1_36_57_355 "IMG_PROC: _IMGPROC_ProcStartProcess ViewAfter"
#define IMG_PROC_2732_112_2_18_1_36_58_356 "IMG_PROC: _IMGPROC_ProcStartProcess end"
#define IMG_PROC_2766_112_2_18_1_36_58_357 "IMG_PROC: _IMGPROC_CtrlEndProcess handle, 0x%x syn: 0x%x"
#define IMG_PROC_2798_112_2_18_1_36_58_358 "IMG_PROC: _IMGPROC_CtrlErrorProcess handle, 0x%x syn: 0x%x"
#define IMG_PROC_2832_112_2_18_1_36_58_359 "IMG_PROC: _IMGPROC_CtrlOpen"
#define IMG_PROC_2837_112_2_18_1_36_58_360 "IMG_PROC: IMGPROC_Open param null %d line error "
#define IMG_PROC_2861_112_2_18_1_36_58_361 "IMG_PROC: IMGPROC_Open alloc temp directory null %d line error "
#define IMG_PROC_2867_112_2_18_1_36_58_362 "IMG_PROC: IMGPROC_Open temp directory null %d line error "
#define IMG_PROC_2893_112_2_18_1_36_58_363 "IMG_PROC: _IMGPROC_CtrlCreateHandle"
#define IMG_PROC_2898_112_2_18_1_36_58_364 "IMG_PROC: _IMGPROC_CreateHandle param null %d line error "
#define IMG_PROC_2917_112_2_18_1_36_58_365 "IMG_PROC: _IMGPROC_CtrlDestroyHandle handle: 0x%x"
#define IMG_PROC_2936_112_2_18_1_36_58_366 "IMG_PROC: _IMGPROC_SendCtrlMsg sig_cmd=0x%x, start %dms"
#define IMG_PROC_2940_112_2_18_1_36_58_367 "IMG_PROC: _IMGPROC_SendCtrlMsg not thread %dms"
#define IMG_PROC_2965_112_2_18_1_36_58_368 "IMG_PROC: _IMGPROC_SendCtrlMsg end %dms"
#define IMG_PROC_3010_112_2_18_1_36_58_369 "IMG_PROC: _IMGPROC_CtrlThreadEntry sig is null %d line error"
#define IMG_PROC_3023_112_2_18_1_36_58_370 "IMG_PROC: _IMGPROC_CtrlThreadEntry enter: %dms, cmd: %d, Priority:%d, 0x%x"
#define IMG_PROC_3036_112_2_18_1_36_58_371 "IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlOpen rtn ptr null %d line error"
#define IMG_PROC_3050_112_2_18_1_36_58_372 "IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlClose rtn ptr null %d line error"
#define IMG_PROC_3064_112_2_18_1_36_58_373 "IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlCreateHandle rtn ptr null %d line error"
#define IMG_PROC_3078_112_2_18_1_36_58_374 "IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlDestroyHandle rtn ptr null %d line error"
#define IMG_PROC_3093_112_2_18_1_36_58_375 "IMG_PROC: _IMGPROC_CtrlThreadEntry StartProcess rtn ptr null %d line error"
#define IMG_PROC_3105_112_2_18_1_36_58_376 "IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlStartProcess % line error"
#define IMG_PROC_3114_112_2_18_1_36_58_377 "IMG_PROC: _IMGPROC_CtrlThreadEntry _IMGPROC_CtrlEndProcess % line error"
#define IMG_PROC_3123_112_2_18_1_36_58_378 "IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlErrorProcess % line error"
#define IMG_PROC_3129_112_2_18_1_36_58_379 "IMG_PROC: _IMGPROC_CtrlThreadEntry, invalid sig ,line %d"
#define IMG_PROC_3136_112_2_18_1_36_58_380 "IMG_PROC: _IMGPROC_CtrlThreadEntry end: %dms, 0x%x"
#define IMG_PROC_3149_112_2_18_1_36_58_381 "IMG_PROC: _IMGPROC_CtrlThreadCreat enter %d"
#define IMG_PROC_3166_112_2_18_1_36_58_382 "IMG_PROC: _IMGPROC_CtrlThreadCreat, fail %d line error"
#define IMG_PROC_3173_112_2_18_1_36_58_383 "IMG_PROC: _IMGPROC_CtrlThreadCreat, thread already exist"
#define IMG_PROC_3176_112_2_18_1_36_58_384 "IMG_PROC: _IMGPROC_CtrlThreadCreat, task_id=0x%x"
#define IMG_PROC_3192_112_2_18_1_36_58_385 "IMG_PROC: _IMGPROC_SendProcessMsg sig_cmd=0x%x, start %dms"
#define IMG_PROC_3196_112_2_18_1_36_59_386 "IMG_PROC: _IMGPROC_SendProcessMsg not thread %dms"
#define IMG_PROC_3221_112_2_18_1_36_59_387 "IMG_PROC: _IMGPROC_SendProcessMsg end %dms"
#define IMG_PROC_3265_112_2_18_1_36_59_388 "IMG_PROC: _IMGPROC_ProcessThreadEntry sig is null %d line error"
#define IMG_PROC_3277_112_2_18_1_36_59_389 "IMG_PROC: _IMGPROC_ProcessThreadEntry enter: %dms, cmd: %d, Priority: %d"
#define IMG_PROC_3294_112_2_18_1_36_59_390 "IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlStartProcess % line error"
#define IMG_PROC_3300_112_2_18_1_36_59_391 "IMG_PROC: _IMGPROC_ProcessThreadEntry, invalid sig ,line %d"
#define IMG_PROC_3309_112_2_18_1_36_59_392 "IMG_PROC: _IMGPROC_ProcessThreadEntry end: %dms"
#define IMG_PROC_3321_112_2_18_1_36_59_393 "IMG_PROC: _IMGPROC_ProcessThreadCreat enter %d"
#define IMG_PROC_3338_112_2_18_1_36_59_394 "IMG_PROC: _IMGPROC_ProcessThreadCreat, fail %d line error"
#define IMG_PROC_3344_112_2_18_1_36_59_395 "IMG_PROC: _IMGPROC_ProcessThreadCreat, thread already exist"
#define IMG_PROC_3347_112_2_18_1_36_59_396 "IMG_PROC: _IMGPROC_ProcessThreadCreat, task_id=0x%x"
#define IMG_PROC_3358_112_2_18_1_36_59_397 "REFIMG: IMG_ProcessAppTaskClose,task_id=0x%x"
#define IMG_PROC_3406_112_2_18_1_36_59_398 "IMG_PROC: =R=I= IMGPROC_Init"
#define IMG_PROC_3442_112_2_18_1_36_59_399 "IMG_PROC: =R=I= IMGPROC_Open cur task: 0x%x "
#define IMG_PROC_3476_112_2_18_1_36_59_400 "IMG_PROC: =R=I= IMGPROC_Close"
#define IMG_PROC_3510_112_2_18_1_36_59_401 "IMG_PROC: =R=I= IMGPROC_CreateHandle"
#define IMG_PROC_3527_112_2_18_1_36_59_402 "IMG_PROC: IMGPROC_CreateHandle handle: 0x%x"
#define IMG_PROC_3544_112_2_18_1_36_59_403 "IMG_PROC: =R=I= IMGPROC_DestroyHandle handle: 0x%x"
#define IMG_PROC_3572_112_2_18_1_36_59_404 "IMG_PROC: src mode:0x%x, num:%d,"
#define IMG_PROC_3573_112_2_18_1_36_59_405 "IMG_PROC: src y_addr:0x%x, uv_addr:0x%x,"
#define IMG_PROC_3574_112_2_18_1_36_59_406 "IMG_PROC: src format:0x%x,"
#define IMG_PROC_3577_112_2_18_1_36_59_407 "IMG_PROC: before 0 y_addr:0x%x, uv_addr:0x%x,"
#define IMG_PROC_3578_112_2_18_1_36_59_408 "IMG_PROC: before 0 w:%d, h:%d,"
#define IMG_PROC_3579_112_2_18_1_36_59_409 "IMG_PROC: before 0 format:0x%x,"
#define IMG_PROC_3583_112_2_18_1_36_59_410 "IMG_PROC: before 1 y_addr:0x%x, uv_addr:0x%x,"
#define IMG_PROC_3584_112_2_18_1_36_59_411 "IMG_PROC: before 1 w:%d, h:%d,"
#define IMG_PROC_3585_112_2_18_1_36_59_412 "IMG_PROC: before 1 format:0x%x,"
#define IMG_PROC_3589_112_2_18_1_36_59_413 "IMG_PROC: proc y_addr:0x%x, uv_addr:0x%x,"
#define IMG_PROC_3590_112_2_18_1_36_59_414 "IMG_PROC: proc rect x:%d, y:%d, w:%d, h:%d"
#define IMG_PROC_3591_112_2_18_1_36_59_415 "IMG_PROC: proc w:%d, h:%d,"
#define IMG_PROC_3592_112_2_18_1_36_59_416 "IMG_PROC: proc format:0x%x,"
#define IMG_PROC_3596_112_2_18_1_36_59_417 "IMG_PROC: after y_addr:0x%x, uv_addr:0x%x,"
#define IMG_PROC_3597_112_2_18_1_36_59_418 "IMG_PROC: after w:%d, h:%d,"
#define IMG_PROC_3598_112_2_18_1_36_59_419 "IMG_PROC: after format:0x%x,"
#define IMG_PROC_3614_112_2_18_1_36_59_420 "IMG_PROC: =R=I= IMGPROC_DoProcess handle: 0x%x , cmd 0x%x, caller:%d"
#define IMG_PROC_3618_112_2_18_1_36_59_421 "IMG_PROC: IMGPROC_DoProcess FIRST"
#define IMG_PROC_3622_112_2_18_1_36_59_422 "IMG_PROC: IMGPROC_DoProcess PROCESS"
#define IMG_PROC_3626_112_2_18_1_36_59_423 "IMG_PROC: IMGPROC_DoProcess BEFORE0"
#define IMG_PROC_3630_112_2_18_1_36_59_424 "IMG_PROC: IMGPROC_DoProcess BEFORE"
#define IMG_PROC_3634_112_2_18_1_36_59_425 "IMG_PROC: IMGPROC_DoProcess AFTER"
#define IMG_PROC_3638_112_2_18_1_36_59_426 "IMG_PROC: IMGPROC_DoProcess HISTORY"
#define IMG_PROC_3642_112_2_18_1_36_59_427 "IMG_PROC: IMGPROC_DoProcess UPDAT_ALG"
#define IMG_PROC_3646_112_2_18_1_36_59_428 "IMG_PROC: IMGPROC_DoProcess UPDAT_SRC"
#define IMG_PROC_3650_112_2_18_1_36_59_429 "IMG_PROC: IMGPROC_DoProcess UPDAT_DST"
#define IMG_PROC_3654_112_2_18_1_36_59_430 "IMG_PROC: IMGPROC_DoProcess END"
#define IMG_PROC_3730_112_2_18_1_37_0_431 "IMG_PROC: =R=I= IMGPROC_DoProcessIcon handle: 0x%x"
#define IMG_PROC_3858_112_2_18_1_37_0_432 "IMG_PROC: =R=I= IMGPROC_AbortProcess handle: 0x%x"
#define IMG_PROC_3881_112_2_18_1_37_0_433 "IMG_PROC: =R=I= IMGPROC_GetProcessedFrame handle: 0x%x"
#define IMG_PROC_3951_112_2_18_1_37_0_434 "IMG_PROC: IMGPROC_SetProcImgDstBuf handle 0x%x"
#define IMG_PROC_4069_112_2_18_1_37_0_435 "IMG_PROC: IMGPROC_SetProcImgDstFs handle 0x%x"
#define IMG_PROC_4095_112_2_18_1_37_0_436 "IMG_PROC: IMGPROC_BufEncoder handle 0x%x"
#define IMG_PROC_4133_112_2_18_1_37_0_437 "IMG_PROC: IMGPROC_BufEncoder y_addr:0x%x, uv_addr:0x%x,"
#define IMG_PROC_4134_112_2_18_1_37_0_438 "IMG_PROC: IMGPROC_BufEncoder w:%d, h:%d,"
#define IMG_PROC_4135_112_2_18_1_37_0_439 "IMG_PROC: IMGPROC_BufEncoder format:0x%x,"
#define IMG_PROC_4137_112_2_18_1_37_0_440 "IMG_PROC: IMGPROC_BufEncoder1 y_addr:0x%x, uv_addr:0x%x,"
#define IMG_PROC_4138_112_2_18_1_37_0_441 "IMG_PROC: IMGPROC_BufEncoder1 w:%d, h:%d,"
#define IMG_PROC_4139_112_2_18_1_37_0_442 "IMG_PROC: IMGPROC_BufEncoder1 format:0x%x,"
#define IMG_PROC_4177_112_2_18_1_37_0_443 "IMG_PROC: IMGPROC_ALLOCA %dk fail %d line error "
#define IMG_PROC_ALG_EXPOSAL_149_112_2_18_1_37_1_444 "IMG_PROC: _EXPOSAL_GetContextPtr id zero, %d line error"
#define IMG_PROC_ALG_EXPOSAL_158_112_2_18_1_37_1_445 "IMG_PROC: _EXPOSAL_GetContextPtr check id %d line error"
#define IMG_PROC_ALG_HDR_90_112_2_18_1_37_2_446 "IMG_PROC: _HDR_GetContextPtr id zero, %d line error"
#define IMG_PROC_ALG_HDR_99_112_2_18_1_37_2_447 "IMG_PROC: _HDR_GetContextPtr check id %d line error"
#define IMG_PROC_ALG_HDR_446_112_2_18_1_37_3_448 "IMG_PROC: _HDR_Start h:%d, complete_line:%d"
#define IMG_PROC_ALG_INFRARED_400_112_2_18_1_37_4_449 "IMG_PROC: _INFRARED_GetContextPtr id zero, %d line error"
#define IMG_PROC_ALG_INFRARED_409_112_2_18_1_37_4_450 "IMG_PROC: _INFRARED_GetContextPtr check id %d line error"
#define IMG_PROC_ALG_N_IN_ONE_198_112_2_18_1_37_5_451 "IMG_PROC: _NInOne_GetContextNum num zero, %d line error"
#define IMG_PROC_ALG_N_IN_ONE_206_112_2_18_1_37_5_452 "IMG_PROC: _NInOne_GetContextNum context head no exit, %d line error"
#define IMG_PROC_ALG_N_IN_ONE_222_112_2_18_1_37_5_453 "IMG_PROC: _NInOne_GetContextNum num no exit, %d line error"
#define IMG_PROC_ALG_NEGATIVE_141_112_2_18_1_37_7_454 "IMG_PROC: _NEGATIVE_GetContextPtr id zero, %d line error"
#define IMG_PROC_ALG_NEGATIVE_150_112_2_18_1_37_7_455 "IMG_PROC: _NEGATIVE_GetContextPtr check id %d line error"
#define IMG_PROC_ALG_XRAY_141_112_2_18_1_37_8_456 "IMG_PROC: _XRAY_GetContextPtr id zero, %d line error"
#define IMG_PROC_ALG_XRAY_150_112_2_18_1_37_8_457 "IMG_PROC: _XRAY_GetContextPtr check id %d line error"
#define IMG_PROC_FACE_DECT_1958_112_2_18_1_37_13_458 "IMG_PROC: _FaceDect_DelContextNum, chn0: 0x%x"
#define IMG_PROC_FACE_DECT_1959_112_2_18_1_37_13_459 "IMG_PROC: _FaceDect_DelContextNum, chn1: 0x%x"
#define IMG_PROC_FACE_DECT_1960_112_2_18_1_37_13_460 "IMG_PROC: _FaceDect_DelContextNum, sw_buf_ptr: 0x%x"
#define IMG_PROC_FACE_DECT_1984_112_2_18_1_37_13_461 "IMG_PROC: _FaceDect_GetContextNum num zero, %d line error"
#define IMG_PROC_FACE_DECT_1992_112_2_18_1_37_13_462 "IMG_PROC: _FaceDect_GetContextNum context head no exit, %d line error"
#define IMG_PROC_FACE_DECT_2008_112_2_18_1_37_13_463 "IMG_PROC: _FaceDect_GetContextNum num no exit, %d line error"
#define IMG_PROC_FACE_DECT_2029_112_2_18_1_37_13_464 "IMG_PROC: _FaceDect_SetInitParam id: 0x%x"
#define IMG_PROC_FACE_DECT_2192_112_2_18_1_37_13_465 "IMG_PROC: _FaceDect_Init "
#define IMG_PROC_FACE_DECT_2232_112_2_18_1_37_13_466 "IMG_PROC: _FaceDect_Update "
#define IMG_PROC_FACE_DECT_2325_112_2_18_1_37_14_467 "IMG_PROC: _FaceDect_Start "
#define IMG_PROC_FACE_DECT_2356_112_2_18_1_37_14_468 "IMG_PROC: IMG_FaceDection, y:0x%x, uv:0x%x"
#define IMG_PROC_FACE_DECT_2357_112_2_18_1_37_14_469 "IMG_PROC: IMG_FaceDection, w:%d, h:%d"
#define IMG_PROC_FACE_DECT_2358_112_2_18_1_37_14_470 "IMG_PROC: IMG_FaceDection, x:%d, y:%d, w:%d, h:%d"
#define IMG_PROC_FACE_DECT_2364_112_2_18_1_37_14_471 "IMG_PROC: IMG_FaceDection num ------%d---------"
#define IMG_PROC_FACE_DECT_2367_112_2_18_1_37_14_472 "IMG_PROC: IMG_FaceDection ------%d---------"
#define IMG_PROC_FACE_DECT_2368_112_2_18_1_37_14_473 "IMG_PROC: IMG_FaceDection rect, x:%d, y:%d, w:%d, h:%d"
#define IMG_PROC_FACE_DECT_2369_112_2_18_1_37_14_474 "IMG_PROC: IMG_FaceDection simle:%d"
#define IMG_PROC_FACE_DECT_2388_112_2_18_1_37_14_475 "IMG_PROC: _FaceDect_Start 0x%x end"
#define IMG_PROC_FACE_DECT_2399_112_2_18_1_37_14_476 "IMG_PROC: _FaceDect_Destroy id: 0x%x"
#define RGB_YUV_CONVERT_69_112_2_18_1_37_16_477 "YUVConvertJPG malloc is fail!---"
#define RGB_YUV_CONVERT_81_112_2_18_1_37_16_478 "[pe_process.c] SaveJpeg successed! "
#define SCALE_HARDWARE_169_112_2_18_1_37_16_479 "[_OpenIspService] lock isp service failed"
#define SCALE_HARDWARE_179_112_2_18_1_37_16_480 "[_OpenIspService] open isp failed"
#define SCALE_HARDWARE_358_112_2_18_1_37_17_481 "[_ScaleSetSliceBuf] allocate slice in buffer, chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define SCALE_HARDWARE_402_112_2_18_1_37_17_482 "[_ScaleSetSliceBuf] allocate slice out buffer, chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define SCALE_HARDWARE_431_112_2_18_1_37_17_483 "[_ReadSlice] src chn (0x%x, 0x%x), dst chn (0x%x, 0x%x), src size (%d, %d)"
#define SCALE_HARDWARE_435_112_2_18_1_37_17_484 "[_ReadSlice] src line chn size (%d, %d), dst size (%d, %d)"
#define SCALE_HARDWARE_447_112_2_18_1_37_17_485 "[_ReadSlice] yuv420 format, src chn size = %d, dst chn size = %d, lines = %d"
#define SCALE_HARDWARE_492_112_2_18_1_37_17_486 "[_ReadSlice] read_lines = %d, chn size = %d, slice_bytes = %d"
#define SCALE_HARDWARE_510_112_2_18_1_37_17_487 "[_ReadSlice] update src chn (0x%x, 0x%x)"
#define SCALE_HARDWARE_539_112_2_18_1_37_17_488 "[_WriteSlice] src chn (0x%x, 0x%x), dst chn (0x%x, 0x%x), src w = %d, dst w = %d"
#define SCALE_HARDWARE_628_112_2_18_1_37_17_489 "[_SetIspParam] isp param: output buf size = %d"
#define SCALE_HARDWARE_633_112_2_18_1_37_17_490 "[_SetIspParam] isp param: src size (%d, %d), src rect (%d, %d, %d, %d), src format = %d"
#define SCALE_HARDWARE_637_112_2_18_1_37_17_491 "[_SetIspParam] isp param: trg size (%d, %d), trg format = %d"
#define SCALE_HARDWARE_643_112_2_18_1_37_17_492 "[_SetIspParam] isp param: src addr (0x%x, 0x%x), trg addr = (0x%x, 0x%x)"
#define SCALE_HARDWARE_675_112_2_18_1_37_17_493 "[_SetScaleIspBuf] allocate isp buffer failed"
#define SCALE_HARDWARE_692_112_2_18_1_37_17_494 "[_SetScaleIspBuf] line buf (0x%x, %d), swap buf (0x%x, %d)"
#define SCALE_HARDWARE_714_112_2_18_1_37_17_495 "[_ScaleDone] output addr (0x%x, 0x%x), size (%d, %d)"
#define SCALE_HARDWARE_719_112_2_18_1_37_18_496 "[_ScaleDone] scale failed, NULL addr"
#define SCALE_HARDWARE_727_112_2_18_1_37_18_497 "[_ScaleDone] scale failed, unenough output lines"
#define SCALE_HARDWARE_737_112_2_18_1_37_18_498 "[_ScaleDone] scale failed, unenough output lines"
#define SCALE_HARDWARE_784_112_2_18_1_37_18_499 "[_SliceScaleDone] output addr (0x%x, 0x%x), size (%d, %d)"
#define SCALE_HARDWARE_789_112_2_18_1_37_18_500 "[_SliceScaleDone] scale failed, y addr is NULL"
#define SCALE_HARDWARE_797_112_2_18_1_37_18_501 "[_SliceScaleDone] scale failed, uv addr is NULL"
#define SCALE_HARDWARE_805_112_2_18_1_37_18_502 "[_SliceScaleDone] scale failed, unenough output lines"
#define SCALE_HARDWARE_815_112_2_18_1_37_18_503 "[_SliceScaleDone] scale failed, unenough output lines"
#define SCALE_HARDWARE_834_112_2_18_1_37_18_504 "[_SliceScaleDone] scale out size (%d, %d), total size = %d"
#define SCALE_HARDWARE_870_112_2_18_1_37_18_505 "[_IsNormalMode] input size - width is not 4 aligned !"
#define SCALE_HARDWARE_877_112_2_18_1_37_18_506 "[_IsNormalMode] input chn0 ptr is not 4 aligned !"
#define SCALE_HARDWARE_884_112_2_18_1_37_18_507 "[_IsNormalMode] input chn0 size is not 32 aligned !"
#define SCALE_HARDWARE_892_112_2_18_1_37_18_508 "[_IsNormalMode] input chn1 ptr is not 4 aligned !"
#define SCALE_HARDWARE_900_112_2_18_1_37_18_509 "[_IsNormalMode] input chn1 size is not 32 aligned !"
#define SCALE_HARDWARE_907_112_2_18_1_37_18_510 "[_IsNormalMode] output chn0 ptr is not 4 aligned !"
#define SCALE_HARDWARE_914_112_2_18_1_37_18_511 "[_IsNormalMode] output chn0 size is not 32 aligned !"
#define SCALE_HARDWARE_922_112_2_18_1_37_18_512 "[_IsNormalMode] output chn1 ptr is not 4 aligned !"
#define SCALE_HARDWARE_930_112_2_18_1_37_18_513 "[_IsNormalMode] output chn1 size is not 32 aligned !"
#define SCALE_HARDWARE_974_112_2_18_1_37_18_514 "[_SetScaleSliceHeight] slice height = %d"
#define SCALE_HARDWARE_1018_112_2_18_1_37_18_515 "[_ScaleSliceMode]"
#define SCALE_HARDWARE_1023_112_2_18_1_37_18_516 "[_ScaleSliceMode] _SetIspParam failed"
#define SCALE_HARDWARE_1036_112_2_18_1_37_18_517 "[_ScaleNormalMode] async_param_ptr = 0x%x, callback = 0x%x, param = 0x%x"
#define SCALE_HARDWARE_1048_112_2_18_1_37_18_518 "[_ScaleSliceMode] _SetScaleIspBuf failed"
#define SCALE_HARDWARE_1056_112_2_18_1_37_18_519 "[_ScaleSliceMode] _ScaleSetSliceBuf failed"
#define SCALE_HARDWARE_1080_112_2_18_1_37_18_520 "[_ScaleSliceMode] src size (%d, %d), src chn (0x%x, 0x%x), src line size (%d, %d)"
#define SCALE_HARDWARE_1083_112_2_18_1_37_18_521 "[_ScaleSliceMode] src skip lines = %d, src total lines = %d"
#define SCALE_HARDWARE_1095_112_2_18_1_37_18_522 "[_ScaleSliceMode] slice in size (%d, %d), src in chn (0x%x, 0x%x), src in line size (%d, %d)"
#define SCALE_HARDWARE_1102_112_2_18_1_37_18_523 "[_ScaleSliceMode] dst size (%d, %d), dst chn (0x%x, 0x%x)"
#define SCALE_HARDWARE_1114_112_2_18_1_37_18_524 "[_ScaleSliceMode] lase sline height = %d"
#define SCALE_HARDWARE_1122_112_2_18_1_37_18_525 "[_ScaleSliceMode] _ReadSlice failed !"
#define SCALE_HARDWARE_1129_112_2_18_1_37_18_526 "[_ScaleSliceMode] ISP_ServiceSetScaleParam failed !"
#define SCALE_HARDWARE_1143_112_2_18_1_37_18_527 "[_ScaleSliceMode] ISP_ServiceStartScale failed !"
#define SCALE_HARDWARE_1156_112_2_18_1_37_18_528 "[_ScaleSliceMode] slice output lines overflow %d, reset to 0x%x"
#define SCALE_HARDWARE_1164_112_2_18_1_37_18_529 "[_ScaleSliceMode] _WriteSlice failed !"
#define SCALE_HARDWARE_1177_112_2_18_1_37_18_530 "[_ScaleSliceMode] scale output done scale out lines = %d, total write lines = %d"
#define SCALE_HARDWARE_1211_112_2_18_1_37_19_531 "[_ScaleNormalMode]"
#define SCALE_HARDWARE_1229_112_2_18_1_37_19_532 "[_ScaleNormalMode] async_param_ptr = 0x%x, callback = 0x%x, param = 0x%x"
#define SCALE_HARDWARE_1269_112_2_18_1_37_19_533 "[_FreeBuffer] isp buf = 0x%x, out slice buf = 0x%x, in slice buf = 0x%x"
#define SCALE_HARDWARE_1364_112_2_18_1_37_19_534 "[_CheckScaleSize] trim rect size is valid"
#define SCALE_HARDWARE_1372_112_2_18_1_37_19_535 "[_CheckScaleSize] trim rect size is valid"
#define SCALE_HARDWARE_1378_112_2_18_1_37_19_536 "[_CheckScaleSize] trim rect size is valid"
#define SCALE_HARDWARE_1395_112_2_18_1_37_19_537 "[_CheckScaleSize] scale mode is SCALE_MODE_NO_DISTORT, reset the target size"
#define SCALE_HARDWARE_1400_112_2_18_1_37_19_538 "[_CheckScaleSize] the size is out of range to do SCALE_MODE_DISTORT scale"
#define SCALE_HARDWARE_1470_112_2_18_1_37_19_539 "[_IsHardwareSupport] src size (%d, %d), src rect (%d, %d, %d, %d), target size (%d, %d)"
#define SCALE_HARDWARE_1524_112_2_18_1_37_19_540 "[_CheckScaleSize] trim rect size is valid"
#define SCALE_HARDWARE_1541_112_2_18_1_37_19_541 "[_CheckScaleSize] scale mode is SCALE_MODE_NO_DISTORT, reset the target size"
#define SCALE_HARDWARE_1546_112_2_18_1_37_19_542 "[_CheckScaleSize] the size is out of range to do SCALE_MODE_DISTORT scale"
#define SCALE_HARDWARE_1619_112_2_18_1_37_19_543 "[_SetScaleSize] src size (%d, %d), src rect (%d, %d, %d, %d), target size (%d, %d)"
#define SCALE_HARDWARE_1644_112_2_18_1_37_19_544 "[_ScaleImage] src chn0 (0x%x, %d), src chn1 (0x%x, %d), dst chn0 (0x%x, %d), dst chn1 (0x%x, %d)"
#define SCALE_HARDWARE_1689_112_2_18_1_37_20_545 "[_ScaleImage] hardware scale unsupport"
#define SCALE_HARDWARE_1704_112_2_18_1_37_20_546 "[_ScaleImage] hardware scale unsupport"
#define SCALE_HARDWARE_1722_112_2_18_1_37_20_547 "[_ScaleImage] hardware scale unsupport"
#define SCALE_IMAGE_105_112_2_18_1_37_20_548 "[_IsScaleBySoftware] change to software scale"
#define SCALE_IMAGE_486_112_2_18_1_37_21_549 "[_ScaleImageARGB888] src_alpha_buf (0x%x, %d, %d), dst_alpha_buf (0x%x, %d, %d), dst_rgb_buf (0x%x, %d, %d)"
#define SCALE_IMAGE_493_112_2_18_1_37_21_550 "[_ScaleImageARGB888] allocate alpha buffer failed"
#define SCALE_IMAGE_511_112_2_18_1_37_21_551 "[_ScaleImageARGB888] get alpha time = %d"
#define SCALE_IMAGE_534_112_2_18_1_37_21_552 "[_ScaleImageARGB888] _ScaleImage rgb888 failed"
#define SCALE_IMAGE_540_112_2_18_1_37_21_553 "[_ScaleImageARGB888] scale alpha time = %d"
#define SCALE_IMAGE_544_112_2_18_1_37_21_554 "[_ScaleImageARGB888] scale alpha done, out size (%d, %d), out addr (0x%x, %d), format (%d)"
#define SCALE_IMAGE_567_112_2_18_1_37_21_555 "[_ScaleImageARGB888] _ScaleImage rgb888 failed"
#define SCALE_IMAGE_573_112_2_18_1_37_21_556 "[_ScaleImageARGB888] scale rgb time = %d"
#define SCALE_IMAGE_577_112_2_18_1_37_21_557 "[_ScaleImageARGB888] scale rgb done, out size (%d, %d), out addr (0x%x, %d), format (%d)"
#define SCALE_IMAGE_588_112_2_18_1_37_21_558 "[_ScaleImageARGB888] merg alpha and rgb time = %d"
#define SCALE_IMAGE_625_112_2_18_1_37_21_559 "[_ScaleImageARGB565] get alpha time = %d"
#define SCALE_IMAGE_643_112_2_18_1_37_21_560 "[_ScaleImageARGB565] _ScaleImage rgb888 failed"
#define SCALE_IMAGE_649_112_2_18_1_37_21_561 "[_ScaleImageARGB565] scale alpha time = %d"
#define SCALE_IMAGE_653_112_2_18_1_37_21_562 "[_ScaleImageARGB565] scale alpha done, out size (%d, %d), out addr (0x%x, %d), format (%d)"
#define SCALE_IMAGE_669_112_2_18_1_37_21_563 "[_ScaleImageARGB565] _ScaleImage rgb888 failed"
#define SCALE_IMAGE_675_112_2_18_1_37_21_564 "[_ScaleImageARGB565] scale rgb time = %d"
#define SCALE_IMAGE_679_112_2_18_1_37_21_565 "[_ScaleImageARGB565] scale rgb done, out size (%d, %d), out addr (0x%x, %d), format (%d)"
#define SCALE_IMAGE_720_112_2_18_1_37_21_566 "IMGPROC_SCALE: _ScaleEnterCritical"
#define SCALE_IMAGE_735_112_2_18_1_37_22_567 "IMGPROC_SCALE: _ScaleExitCritical"
#define SCALE_IMAGE_892_112_2_18_1_37_22_568 "[_CheckScaleOutputChn] output buffer is enough for align"
#define SCALE_IMAGE_901_112_2_18_1_37_22_569 "[_CheckScaleOutputChn] output buffer is not enough for align"
#define SCALE_IMAGE_923_112_2_18_1_37_22_570 "[_SetScaleSize] output format = %d, dst chn0 (0x%x, %d), dst chn1 (0x%x, %d)"
#define SCALE_IMAGE_943_112_2_18_1_37_22_571 "[_SetScaleParam] _SetScaleInputChn failed"
#define SCALE_IMAGE_950_112_2_18_1_37_22_572 "[_SetScaleParam] _SetScaleOutputChn failed"
#define SCALE_IMAGE_961_112_2_18_1_37_22_573 "[_SetScaleParam] scale function invalid"
#define SCALE_IMAGE_975_112_2_18_1_37_22_574 "[_SetScaleParam] scale function invalid"
#define SCALE_IMAGE_981_112_2_18_1_37_22_575 "[_SetScaleParam] scale not support"
#define SCALE_IMAGE_987_112_2_18_1_37_22_576 "[_SetScaleParam] _SetScaleSize failed"
#define SCALE_IMAGE_991_112_2_18_1_37_22_577 "[_SetScaleParam] scale_by_software = %d"
#define SCALE_IMAGE_1084_112_2_18_1_37_22_578 "[IMGPROC_SCALE: _DoScale] output buffer is invalid"
#define SCALE_IMAGE_1091_112_2_18_1_37_22_579 "[IMGPROC_SCALE: _DoScale] get scale func failed"
#define SCALE_IMAGE_1108_112_2_18_1_37_22_580 "[IMGPROC_SCALE: _DoScale] ret = %d, output size (%d, %d), output format (%d)"
#define SCALE_IMAGE_1148_112_2_18_1_37_22_581 "[IMGPROC_SCALE: _SetScaleEvent] %d"
#define SCALE_IMAGE_1162_112_2_18_1_37_22_582 "[IMGPROC_SCALE: _GetScaleEvent] + , %d"
#define SCALE_IMAGE_1171_112_2_18_1_37_22_583 "[IMGPROC_SCALE: _GetScaleEvent] -, actual event = %d, status = %d"
#define SCALE_IMAGE_1184_112_2_18_1_37_22_584 "[IMGPROC_SCALE: _PostScaleStart] scale_cxt_ptr = 0x%x"
#define SCALE_IMAGE_1190_112_2_18_1_37_22_585 "[IMGPROC_SCALE: _PostScaleStart] is called = %d"
#define SCALE_IMAGE_1218_112_2_18_1_37_22_586 "[IMGPROC_SCALE: _TaskEntry], sig=0x%x,sig_code = %d"
#define SCALE_IMAGE_1237_112_2_18_1_37_23_587 "[IMGPROC_SCALE: _TaskEntry] is_async = %d, context_ptr = 0x%x"
#define SCALE_IMAGE_1258_112_2_18_1_37_23_588 "[_TaskEntry] invalid signal"
#define SCALE_IMAGE_1263_112_2_18_1_37_23_589 "[IMGPROC_SCALE: _TaskEntry] scale done, is_async = %d, cxt_ptr = 0x%x"
#define SCALE_IMAGE_1332_112_2_18_1_37_23_590 "[IMGPROC_SCALE: _OpenTask] create task, s_task_id = 0x%x, priority = %d"
#define SCALE_IMAGE_1343_112_2_18_1_37_23_591 "[IMGPROC_SCALE: _OpenTask] change task priority from %d to %d"
#define SCALE_IMAGE_1359_112_2_18_1_37_23_592 "[IMGPROC_SCALE: _CloseTask] s_task_id = 0x%x"
#define SCALE_IMAGE_1391_112_2_18_1_37_23_593 "[_SuspendTask] s_task_id = 0x%x"
#define SCALE_IMAGE_1415_112_2_18_1_37_23_594 "[IMGPROC_SCALE: _SendMsg], sig_code = %d"
#define SCALE_IMAGE_1510_112_2_18_1_37_23_595 "[IMGPROC_SCALE: _CopySrcImage] src buf chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define SCALE_IMAGE_1512_112_2_18_1_37_23_596 "[IMGPROC_SCALE: _CopySrcImage] src data chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define SCALE_IMAGE_1543_112_2_18_1_37_23_597 "[IMGPROC_SCALE: _StartScale] enter, is_async = %d, cxt_ptr = 0x%x"
#define SCALE_IMAGE_1581_112_2_18_1_37_23_598 "[IMGPROC_SCALE: _StartScale] exit, is_async = %d, ret_value = %d"
#define SCALE_IMAGE_1595_112_2_18_1_37_23_599 "[scale_image: _ScaleArgbCallBack], the output addr 0x%x, width %d, height %d"
#define SCALE_IMAGE_1645_112_2_18_1_37_23_600 "[Scale_image: _StartScaleArgb888]software scale, handle the argb888 scale directly!"
#define SCALE_IMAGE_1650_112_2_18_1_37_23_601 "[Scale_image: _StartScaleArgb888] accomplish software scale argb888 time is %d"
#define SCALE_IMAGE_1673_112_2_18_1_37_23_602 "[Scale_image: _StartScaleArgb888] src_alpha_buf (0x%x, %d, %d), dst_alpha_buf (0x%x, %d, %d)"
#define SCALE_IMAGE_1679_112_2_18_1_37_23_603 "[Scale_image: _StartScaleArgb888] allocate alpha buffer failed"
#define SCALE_IMAGE_1694_112_2_18_1_37_23_604 "[Scale_image: _StartScaleArgb888] start scaling RGB888 time is %d "
#define SCALE_IMAGE_1703_112_2_18_1_37_23_605 "[Scale_image: _StartScaleArgb888] scale argb888 error!! scaling result is %d"
#define SCALE_IMAGE_1709_112_2_18_1_37_23_606 "[Scale_image: _StartScaleArgb888] get alpha start time is %d, async mode is %d "
#define SCALE_IMAGE_1721_112_2_18_1_37_23_607 "[Scale_image: _StartScaleArgb888] get alpha accomplish time is %d"
#define SCALE_IMAGE_1731_112_2_18_1_37_23_608 "[Scale_image: _StartScaleArgb888] scale rgb888 error!! thx stat is %d, scaling result is %d"
#define SCALE_IMAGE_1738_112_2_18_1_37_24_609 "[Scale_image: _StartScaleArgb888] scale rgb accompish time is %d"
#define SCALE_IMAGE_1767_112_2_18_1_37_24_610 "[Scale_image: _StartScaleArgb888] start scaling alpha time is %d "
#define SCALE_IMAGE_1784_112_2_18_1_37_24_611 "[Scale_image: _StartScaleArgb888] scale alpha error!! scaling result is %d"
#define SCALE_IMAGE_1800_112_2_18_1_37_24_612 "[Scale_image: _StartScaleArgb888] alpha scale error, spend too much time!"
#define SCALE_IMAGE_1809_112_2_18_1_37_24_613 "[Scale_image: _StartScaleArgb888] set alpha start time is %d, async mode is %d "
#define SCALE_IMAGE_1822_112_2_18_1_37_24_614 "[Scale_image: _StartScaleArgb888] Set alpha accomplish time is %d"
#define SCALE_IMAGE_1833_112_2_18_1_37_24_615 "[Scale_image: _StartScaleArgb888] scale alpha error!! thx stat is %d, scaling result is %d"
#define SCALE_IMAGE_1852_112_2_18_1_37_24_616 "[Scale_image: _StartScaleArgb888] alpha checksum after not corresponding!!"
#define SCALE_IMAGE_1857_112_2_18_1_37_24_617 "[Scale_image: _StartScaleArgb888] accomplish scale alpha time is %d"
#define SCALE_IMAGE_1893_112_2_18_1_37_24_618 "[IMGPROC_ScaleImage] input/output parameter invalid"
#define SCALE_IMAGE_1899_112_2_18_1_37_24_619 "[IMGPROC_ScaleImage] src size (%d, %d), src tirm rect (%d, %d, %d, %d),"
#define SCALE_IMAGE_1902_112_2_18_1_37_24_620 "[IMGPROC_ScaleImage] src format (%d), src chn0 (0x%x, %d), chn1 (0x%x, %d),"
#define SCALE_IMAGE_1906_112_2_18_1_37_24_621 "[IMGPROC_ScaleImage] target size (%d, %d), target format (%d), target buf (0x%x, %d),"
#define SCALE_IMAGE_1912_112_2_18_1_37_24_622 "[IMGPROC_ScaleImage] scale by software = %d"
#define SCALE_IMAGE_1918_112_2_18_1_37_24_623 "[IMGPROC_ScaleImage] revise src width to %d for YUV422 format"
#define SCALE_IMAGE_1926_112_2_18_1_37_24_624 "[IMGPROC_ScaleImage] revise src size to %dX%d for YUV420 format"
#define SCALE_IMAGE_1936_112_2_18_1_37_24_625 "[IMGPROC_ScaleImage] ret = %d, _SetScaleParam failed"
#define SCALE_IMAGE_1955_112_2_18_1_37_24_626 "[IMGPROC_ScaleImage] ret = %d, output size (%d, %d), output format (%d)"
#define SCALE_IMAGE_1959_112_2_18_1_37_24_627 "[IMGPROC_ScaleImage]output chn0 (0x%x, %d), output chn1 (0x%x, %d)"
#define SCALE_IMAGE_1963_112_2_18_1_37_24_628 "[IMGPROC_ScaleImage] spend time = %d"
#define SCALE_SOFTWARE_371_112_2_18_1_37_25_629 "[_CheckScaleSize] trim rect size is valid"
#define SCALE_SOFTWARE_391_112_2_18_1_37_25_630 "[_CheckScaleSize] scale mode is SCALE_MODE_NO_DISTORT, reset the target size"
#define SCALE_SOFTWARE_431_112_2_18_1_37_25_631 "[_CheckScaleSize] src size (%d, %d), src rect (%d, %d, %d, %d), target size (%d, %d)"
#define SCALE_SOFTWARE_462_112_2_18_1_37_25_632 "[_ScaleImage by software]"
#define ISP_DRV_6X00_577_112_2_18_1_37_39_633 "ISP_SERVICE: ISP_SetVerDownTap the ver dwen top %d error"
#define ISP_DRV_6X00_699_112_2_18_1_37_40_634 "ISP_SERVICE: ISP_SetMode the mode %d error"
#define ISP_DRV_6X00_732_112_2_18_1_37_40_635 "ISP_SERVICE: ISP_SwichtClkDomain the domain %d error"
#define ISP_DRV_6X00_791_112_2_18_1_37_40_636 "ISP_SERVICE: ISP_SetCAPInputFormat the format %d error"
#define ISP_DRV_6X00_834_112_2_18_1_37_40_637 "ISP_SERVICE: ISP_SetSrcSize the x %d, y %y error"
#define ISP_DRV_6X00_897_112_2_18_1_37_40_638 "ISP_SERVICE: ISP_SetScaleInputFormat the format %d error"
#define ISP_DRV_SC6530_223_112_2_18_1_37_42_639 "ISP_Drv, _ISP_DriverModuleEnable, ahb_ctrl_addr 0x%x"
#define ISP_DRV_SC6530_236_112_2_18_1_37_42_640 "ISP_Drv, _ISP_DriverGetIVSPController, got_rights %d"
#define ISP_DRV_SC6530_256_112_2_18_1_37_42_641 "ISP_Drv, _ISP_DriverReleaseIVSPController, got_rights %d"
#define ISP_DRV_SC6530_307_112_2_18_1_37_42_642 "ISP_Drv, _ISP_DriverModuleEnable, ahb_ctrl_addr 0x%x"
#define ISP_DRV_SC6530_328_112_2_18_1_37_42_643 "ISP_Drv, ISP_DriverDeInit, ahb_ctrl_addr 0x%x"
#define ISP_DRV_SC6530_410_112_2_18_1_37_42_644 "ISP_Drv, ISP_DriverStart, time %d"
#define ISP_DRV_SC6530_438_112_2_18_1_37_43_645 "ISP_DRV: ISP_DriverStart, output_format %d"
#define ISP_DRV_SC6530_494_112_2_18_1_37_43_646 "ISP_Drv, ISP_DriverStop, time %d"
#define ISP_DRV_SC6530_618_112_2_18_1_37_43_647 "ISP_DRV: ISP_DriverFrameLock,f = 0x%x, time %d"
#define ISP_DRV_SC6530_633_112_2_18_1_37_43_648 "ISP_DRV: ISP_DriverFrameUnLock,f = 0x%x, time %d"
#define ISP_DRV_SC6530_652_112_2_18_1_37_43_649 "ISP_DRV: ISP_DriverFrameIsLocked,f = 0x%x, rtn %d"
#define ISP_DRV_SC6530_692_112_2_18_1_37_43_650 "ISP_DRV: ISP_DriverCapConfig, base_addr 0x%x, id 0x%x, param 0x%x "
#define ISP_DRV_SC6530_948_112_2_18_1_37_44_651 "ISP_DRV: ISP_DriverCapConfig, rtn %d"
#define ISP_DRV_SC6530_961_112_2_18_1_37_44_652 "ISP_DRV: ISP_DriverCapGetInfo, base_addr 0x%x, id 0x%x, param 0x%x "
#define ISP_DRV_SC6530_1472_112_2_18_1_37_45_653 "ISP_DRV: _ISP_ISRSensorStartOfFrame user_func 0x%x"
#define ISP_DRV_SC6530_1485_112_2_18_1_37_45_654 "ISP_DRV: _ISP_ISRSensorEndOfFrame user_func 0x%x"
#define ISP_DRV_SC6530_1501_112_2_18_1_37_45_655 "ISP_DRV: _ISP_ISRCapStartOfFrame,time = %d"
#define ISP_DRV_SC6530_1511_112_2_18_1_37_45_656 "ISP_DRV: _ISP_ISRCapStartOfFrame, wait for the next frame unlocked"
#define ISP_DRV_SC6530_1538_112_2_18_1_37_45_657 "ISP_DRV: _ISP_ISRCapEndOfFrame user_func 0x%x"
#define ISP_DRV_SC6530_1555_112_2_18_1_37_45_658 "ISP_DRV: _ISP_ISRTxDone,time = %d"
#define ISP_DRV_SC6530_1561_112_2_18_1_37_45_659 "ISP_DRV: _ISP_ISRTxDone,Has been stopped"
#define ISP_DRV_SC6530_1586_112_2_18_1_37_45_660 "ISP_DRV: _ISP_ISRTxDone,frame_curr = 0x%x, y,u,v = {0x%x,0x%x,0x%x}, fmt %d"
#define ISP_DRV_SC6530_1620_112_2_18_1_37_45_661 "ISP_DRV: _ISP_ISRTxDone,frame_curr = 0x%x, y,u,v = {0x%x,0x%x,0x%x}, w,h {%d,%d}, fmt %d"
#define ISP_DRV_SC6530_1764_112_2_18_1_37_45_662 "ISP_Drv, _ISP_DriverSetFrameAddr, time %d"
#define ISP_DRV_SC6530_1866_112_2_18_1_37_45_663 "ISP_DRV: _ISP_DriverGenScxCoeff i_w/i_h/o_w/o_h = {%d, %d, %d, %d,}"
#define ISP_DRV_SC6530_1877_112_2_18_1_37_45_664 "ISP_DRV: _ISP_DriverGenScxCoeff GenScaleCoeff error!"
#define ISP_DRV_SC8800G_455_112_2_18_1_37_47_665 "ISP_DRV: ISP_DriverStart , s_isp_mod.isp_mode = %d ,time = %d"
#define ISP_DRV_SC8800G_477_112_2_18_1_37_47_666 "ISP_DRV: ISP_DriverStart , p_path2->input_format %d ,p_path2->output_format %d"
#define ISP_DRV_SC8800G_585_112_2_18_1_37_47_667 "ISP_DRV: ISP_DriverExtStart , s_isp_mod.isp_mode = %d ,time = %d"
#define ISP_DRV_SC8800G_1661_112_2_18_1_37_49_668 "ISP_DRV: ISP_DriverFrameLock,f = 0x%x"
#define ISP_DRV_SC8800G_1676_112_2_18_1_37_49_669 "ISP_DRV: ISP_DriverFrameUnLock,f = 0x%x"
#define ISP_DRV_SC8800G_1695_112_2_18_1_37_49_670 "ISP_DRV: ISP_DriverFrameIsLocked,f = 0x%x, rtn %d"
#define ISP_DRV_SC8800G_1754_112_2_18_1_37_49_671 "ISP_DRV: _ISP_DriverHISRRoot, *pData 0x%x"
#define ISP_DRV_SC8800G_1758_112_2_18_1_37_49_672 "ISP_DRV: _ISP_DriverHISRRoot, irq_num 0x%x"
#define ISP_DRV_SC8800G_1787_112_2_18_1_37_49_673 "ISP_DRV: _ISP_DriverISRRoot, base_addr 0x%x irq_line 0x%x"
#define ISP_DRV_SC8800G_1799_112_2_18_1_37_49_674 "ISP_DRV: _ISP_ISRSensorStartOfFrame user_func 0x%x"
#define ISP_DRV_SC8800G_1812_112_2_18_1_37_49_675 "ISP_DRV: _ISP_ISRSensorEndOfFrame user_func 0x%x"
#define ISP_DRV_SC8800G_1828_112_2_18_1_37_49_676 "ISP_DRV: _ISP_ISRCapStartOfFrame,time = %d"
#define ISP_DRV_SC8800G_1846_112_2_18_1_37_49_677 "ISP_DRV: _ISP_ISRCapStartOfFrame, wait for the path1 next frame unlocked"
#define ISP_DRV_SC8800G_1867_112_2_18_1_37_49_678 "ISP_DRV: _ISP_ISRCapStartOfFrame, wait for the next frame unlocked"
#define ISP_DRV_SC8800G_1889_112_2_18_1_37_50_679 "ISP_DRV: _ISP_ISRCapEndOfFrame user_func 0x%x"
#define ISP_DRV_SC8800G_1905_112_2_18_1_37_50_680 "ISP_DRV: _ISP_ISRPath1Done,time = %d"
#define ISP_DRV_SC8800G_1915_112_2_18_1_37_50_681 "ISP_DRV: _ISP_ISRPath1Done,frame_curr = 0x%x, y,u,v = {0x%x,0x%x,0x%x}"
#define ISP_DRV_SC8800G_2007_112_2_18_1_37_50_682 "ISP_DRV: _ISP_ISRPath2Done the next frame has not been unlocked, skip this frame"
#define ISP_DRV_SC8800G_2046_112_2_18_1_37_50_683 "ISP_DRV: _ISP_ISRPath2Done y,u,v = {0x%x,0x%x,0x%x} ,width,height = {%d, %d},time = %d"
#define ISP_DRV_SC8800G_2464_112_2_18_1_37_51_684 "ISP_DRV: _ISP_DriverGenScxCoeff GenScaleCoeff error!"
#define ISP_DRV_SC8800G_2593_112_2_18_1_37_51_685 "ISP_DRV: _ISP_DriverGenScxCoeff i_w/i_h/o_w/o_h = {%d, %d, %d, %d,}"
#define ISP_DRV_SC8800G_2604_112_2_18_1_37_51_686 "ISP_DRV: _ISP_DriverGenScxCoeff GenScaleCoeff error!"
#define ISP_DRV_SC8800G_2718_112_2_18_1_37_51_687 "ISP_DRV: _ISP_DriverGenScxCoeff i_w/i_h/o_w/o_h = {%d, %d, %d, %d,}"
#define ISP_DRV_SC8800G_2729_112_2_18_1_37_51_688 "ISP_DRV: _ISP_DriverGenScxCoeff GenScaleCoeff error!"
#define ISP_DRV_SC8800H_483_112_2_18_1_37_52_689 "\r\n+++++++++++ISP:Start Auto Focus!+++++++++"
#define ISP_DRV_SC8800H_512_112_2_18_1_37_53_690 "\r\n++++++++ISP:Stop AF+++++++"
#define ISP_DRV_SC8800H_526_112_2_18_1_37_53_691 "rnISP_BeforeGetdataFromLCDC: addr %d,x %d, y %d, w %d, h %d"
#define ISP_DRV_SC8800H_665_112_2_18_1_37_53_692 "\r\nISP:_ISP_AutoCopyAll"
#define ISP_DRV_SC8800H_889_112_2_18_1_37_53_693 "\r\n_ISP_SetLNCCenter:cx=%d,cy=%d"
#define ISP_DRV_SC8800H_900_112_2_18_1_37_53_694 "\r\n_ISP_SetLNCAlpha:r=%d,g=%d,b=%d,shift=%d"
#define ISP_DRV_SC8800H_910_112_2_18_1_37_53_695 "\r\n_ISP_SetLNCSqareSize:rsize=%d"
#define ISP_DRV_SC8800H_976_112_2_18_1_37_53_696 "\r\n_ISP_DumpCceMatrix: %d"
#define ISP_DRV_SC8800H_1024_112_2_18_1_37_54_697 "\r\n_ISP_SetPortBAddress,ADDR=0x%x"
#define ISP_DRV_SC8800H_1066_112_2_18_1_37_54_698 "\r\n_ISP_DumpGammaTable: %d"
#define ISP_DRV_SC8800H_2940_112_2_18_1_37_58_699 "ISP_BeforeGetdataFromLCDC: addr %d,x %d, y %d, w %d, h %d"
#define ISP_DRV_SC8800H_3769_112_2_18_1_37_59_700 "\r\nISP_DRV: film.w = %d, film.h = %d"
#define ISP_DRV_SC8800H_3771_112_2_18_1_37_59_701 "rnISP_DRV: interface_in.x= %d, interface_ice_in.y= %d, interface_in.w= %d,interface_in.h= %d"
#define ISP_DRV_SC8800H_3773_112_2_18_1_37_59_702 "rnISP_DRV: s1_in.x= %d, s1_in.y= %d, s1_in.w= %d,s1_in.h= %d"
#define ISP_DRV_SC8800H_3774_112_2_18_1_37_59_703 "\r\nISP_DRV: s1_miniout.w= %d, s1_miniout.h = %d"
#define ISP_DRV_SC8800H_3775_112_2_18_1_37_59_704 "\r\nISP_DRV: s1_realout.w= %d, s1_realout.h = %d"
#define ISP_DRV_SC8800H_3777_112_2_18_1_37_59_705 "rnISP_DRV: s2_in.x= %d, s2_in.y= %d, s2_in.w= %d,s2_in.h= %d"
#define ISP_DRV_SC8800H_3778_112_2_18_1_37_59_706 "\r\nISP_DRV: s2_miniout.w= %d, s2_miniout.h = %d"
#define ISP_DRV_SC8800H_3779_112_2_18_1_37_59_707 "\r\nISP_DRV: s2_realout.w= %d, s2_realout.h = %d"
#define ISP_DRV_SC8800H_3781_112_2_18_1_37_59_708 "rnISP_DRV: display.x= %d, display.y= %d, display.w= %d,display.h= %d"
#define ISP_DRV_SC8800H_3783_112_2_18_1_37_59_709 "\r\nISP_DRV: CAP_START = 0x%x, CAP_END = 0x%x"
#define ISP_DRV_SC8800H_3785_112_2_18_1_37_59_710 "\r\nISP_DRV: ISP_SRC_SIZE = 0x%x"
#define ISP_DRV_SC8800H_3787_112_2_18_1_37_59_711 "\r\nISP_DRV: ISP_SC2_START = 0x%x, ISP_SC2_SIZE = 0x%x"
#define ISP_DRV_SC8800H_3789_112_2_18_1_37_59_712 "\r\nISP_DRV: ISP_DISP_START = 0x%x, ISP_DISP_SIZE = 0x%x"
#define ISP_DRV_SC8800H_3791_112_2_18_1_37_59_713 "\r\nISP_DRV: ISP_ENC_START = 0x%x, ISP_ENC_SIZE = 0x%x"
#define ISP_DRV_SC8800H_3793_112_2_18_1_37_59_714 "\r\nISP_DRV: SC1_CONF = 0x%x, SC2_CONF = 0x%x"
#define ISP_DRV_SC8800H_4040_112_2_18_1_38_0_715 "\r\nISP_DRV: film.w = %d, film.h = %d"
#define ISP_DRV_SC8800H_4042_112_2_18_1_38_0_716 "rnISP_DRV: interface_in.x= %d, interface_in.y= %d, interface_in.w= %d,interface_in.h= %d"
#define ISP_DRV_SC8800H_4044_112_2_18_1_38_0_717 "rnISP_DRV: s1_in.x= %d, s1_in.y= %d, s1_in.w= %d,s1_in.h= %d"
#define ISP_DRV_SC8800H_4045_112_2_18_1_38_0_718 "\r\nISP_DRV: s1_miniout.w= %d, s1_miniout.h = %d"
#define ISP_DRV_SC8800H_4046_112_2_18_1_38_0_719 "\r\nISP_DRV: s1_realout.w= %d, s1_realout.h = %d"
#define ISP_DRV_SC8800H_4048_112_2_18_1_38_0_720 "rnISP_DRV: s2_in.x= %d, s2_in.y= %d, s2_in.w= %d,s2_in.h= %d"
#define ISP_DRV_SC8800H_4049_112_2_18_1_38_0_721 "\r\nISP_DRV: s2_miniout.w= %d, s2_miniout.h = %d"
#define ISP_DRV_SC8800H_4050_112_2_18_1_38_0_722 "\r\nISP_DRV: s2_realout.w= %d, s2_realout.h = %d"
#define ISP_DRV_SC8800H_4052_112_2_18_1_38_0_723 "rnISP_DRV: display.x= %d, display.y= %d, display.w= %d,display.h= %d"
#define ISP_DRV_SC8800H_4054_112_2_18_1_38_0_724 "\r\nISP_DRV: CAP_START = 0x%x, CAP_END = 0x%x"
#define ISP_DRV_SC8800H_4056_112_2_18_1_38_0_725 "\r\nISP_DRV: ISP_SRC_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4058_112_2_18_1_38_0_726 "\r\nISP_DRV: ISP_SC2_START = 0x%x, ISP_SC2_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4060_112_2_18_1_38_0_727 "\r\nISP_DRV: ISP_DISP_START = 0x%x, ISP_DISP_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4062_112_2_18_1_38_0_728 "\r\nISP_DRV: ISP_ENC_START = 0x%x, ISP_ENC_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4064_112_2_18_1_38_0_729 "\r\nISP_DRV: SC1_CONF = 0x%x, SC2_CONF = 0x%x"
#define ISP_DRV_SC8800H_4220_112_2_18_1_38_0_730 "rnISP_DRV: s2_in.x= %d, s2_in.y= %d, s2_in.w= %d,s2_in.h= %d"
#define ISP_DRV_SC8800H_4221_112_2_18_1_38_0_731 "\r\nISP_DRV: s2_miniout.w= %d, s2_miniout.h = %d"
#define ISP_DRV_SC8800H_4222_112_2_18_1_38_0_732 "\r\nISP_DRV: s2_realout.w= %d, s2_realout.h = %d"
#define ISP_DRV_SC8800H_4224_112_2_18_1_38_0_733 "rnISP_DRV: display.x= %d, display.y= %d, display.w= %d,display.h= %d"
#define ISP_DRV_SC8800H_4227_112_2_18_1_38_0_734 "rnISP_DRV: ISP_PORTC_CURR_YBA = 0x%x,ISP_PORTC_CURR_UBA = 0x%x,ISP_PORTC_CURR_VBA = 0x%x "
#define ISP_DRV_SC8800H_4229_112_2_18_1_38_0_735 "\r\nISP_DRV: SC1_CONF = 0x%x, SC2_CONF = 0x%x"
#define ISP_DRV_SC8800H_4231_112_2_18_1_38_0_736 "\r\nISP_DRV: ISP_SC2_START = 0x%x, ISP_SC2_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4233_112_2_18_1_38_0_737 "\r\nISP_DRV: ISP_DISP_START = 0x%x, ISP_DISP_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4235_112_2_18_1_38_0_738 "\r\nISP_DRV: VP_REVIEW_SIZE = %d"
#define ISP_DRV_SC8800H_4237_112_2_18_1_38_0_739 "\r\nISP_DRV: ISP_PORTC_SKIP = %d"
#define ISP_DRV_SC8800H_4239_112_2_18_1_38_0_740 "\r\nISP_DRV: ISP_SELF_RATE = %d"
#define ISP_DRV_SC8800H_4243_112_2_18_1_38_0_741 "ISP_DRV: vt review start, curr_time = %d"
#define ISP_DRV_SC8800H_4246_112_2_18_1_38_0_742 "rnISP_DRV: ISP_PORTC_CURR_YBA = 0x%x,ISP_PORTC_CURR_UBA = 0x%x,ISP_PORTC_CURR_VBA = 0x%x "
#define ISP_DRV_SC8800H_4247_112_2_18_1_38_0_743 "ISP_DRV start PORTC, curr_time=%d "
#define ISP_DRV_SC8800H_4602_112_2_18_1_38_1_744 "rnISP_DRV: interface_in.x= %d, interface_in.y= %d, interface_in.w= %d,interface_in.h= %d"
#define ISP_DRV_SC8800H_4604_112_2_18_1_38_1_745 "rnISP_DRV: s1_in.x= %d, s1_in.y= %d, s1_in.w= %d,s1_in.h= %d"
#define ISP_DRV_SC8800H_4605_112_2_18_1_38_1_746 "\r\nISP_DRV: s1_miniout.w= %d, s1_miniout.h = %d"
#define ISP_DRV_SC8800H_4606_112_2_18_1_38_1_747 "\r\nISP_DRV: s1_realout.w= %d, s1_realout.h = %d"
#define ISP_DRV_SC8800H_4608_112_2_18_1_38_1_748 "rnISP_DRV: s2_in.x= %d, s2_in.y= %d, s2_in.w= %d,s2_in.h= %d"
#define ISP_DRV_SC8800H_4609_112_2_18_1_38_1_749 "\r\nISP_DRV: s2_miniout.w= %d, s2_miniout.h = %d"
#define ISP_DRV_SC8800H_4610_112_2_18_1_38_1_750 "\r\nISP_DRV: s2_realout.w= %d, s2_realout.h = %d"
#define ISP_DRV_SC8800H_4612_112_2_18_1_38_1_751 "rnISP_DRV: display.x= %d, display.y= %d, display.w= %d,display.h= %d"
#define ISP_DRV_SC8800H_4615_112_2_18_1_38_1_752 "\r\nISP_DRV: ISP_SRC_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4617_112_2_18_1_38_1_753 "\r\nISP_DRV: SC1_CONF = 0x%x"
#define ISP_DRV_SC8800H_4619_112_2_18_1_38_1_754 "\r\nISP_DRV: ISP_SC2_START = 0x%x, ISP_SC2_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4621_112_2_18_1_38_1_755 "\r\nISP_DRV: ISP_DISP_START = 0x%x, ISP_DISP_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4623_112_2_18_1_38_1_756 "\r\nISP_DRV: ISP_ENC_START = 0x%x, ISP_ENC_SIZE = 0x%x"
#define ISP_DRV_SC8800H_4626_112_2_18_1_38_1_757 "rnISP_DRV: ISP_PORTA_NEXT_YBA = 0x%x, ISP_PORTA_NEXT_UBA = 0x%x, ISP_PORTA_NEXT_VBA = 0x%x"
#define ISP_DRV_SC8800H_4644_112_2_18_1_38_1_758 "ISP drv: ISP_StartFromRawRGBBypassMode"
#define ISP_DRV_SC8800H_4827_112_2_18_1_38_1_759 "ISP_DRV: ISP_PORTC_SKIP = %d"
#define ISP_DRV_SC8800H_4829_112_2_18_1_38_1_760 "ISP_DRV: ISP_SELF_RATE = %d"
#define ISP_DRV_SC8800H_4831_112_2_18_1_38_1_761 "ISP_DRV: start portc = %d"
#define ISP_DRV_SC8800H_4915_112_2_18_1_38_1_762 "ISP:ISP_ISR1CapStartOfFrame curr_time=%d"
#define ISP_DRV_SC8800H_4942_112_2_18_1_38_2_763 "ISP_DRV:Review enabled"
#define ISP_DRV_SC8800H_5049_112_2_18_1_38_2_764 "ISP:ISP_ISR2CapEndOfFrame curr_time = %d"
#define ISP_DRV_SC8800H_5117_112_2_18_1_38_2_765 "ISP_ISR3EncoderEndOfFrame,curr_yaddr=0x%x ,cutt_time = %d,time_interval=%d\r\n"
#define ISP_DRV_SC8800H_5197_112_2_18_1_38_2_766 "ISP:ISP_ISR4DisplayEndOfFrame,curr_addr=0x%x, curr_time=%d "
#define ISP_DRV_SC8800H_5254_112_2_18_1_38_2_767 "ISP:ISP_ISR5PortAFIFOOverflowed,curr_time=%d "
#define ISP_DRV_SC8800H_5260_112_2_18_1_38_2_768 "ISP CURR REGISTER: ISP_CONF=0x%x, ISP_SUBM_MODE=0x%x, ISP_SELF_RATE=0x%x,CAP_CNTRL=0x%x,CAP_START=0x%x, CAP_END=0x%x"
#define ISP_DRV_SC8800H_5265_112_2_18_1_38_2_769 "ISP CURR REGISTER: ISP_SRC_SIZE=0x%x, ISP_SC2_SIZE=0x%x, ISP_SC2_START=0x%x, ISP_SC2_START=0x%x,ISP_DISP_SIZE=0x%x,ISP_DISP_START=0x%x,ISP_ENC_SIZE=0x%x,ISP_ENC_START=0x%x"
#define ISP_DRV_SC8800H_5301_112_2_18_1_38_2_770 "ISP:ISP_ISR6PortBFIFOOverflowed,curr_time=%d "
#define ISP_DRV_SC8800H_5311_112_2_18_1_38_2_771 "ISP CURR REGISTER: ISP_CONF=0x%x, ISP_SUBM_MODE=0x%x, ISP_SELF_RATE=0x%x,CAP_CNTRL=0x%x,CAP_START=0x%x, CAP_END=0x%x"
#define ISP_DRV_SC8800H_5317_112_2_18_1_38_2_772 "ISP CURR REGISTER: ISP_SRC_SIZE=0x%x, ISP_SC2_SIZE=0x%x, ISP_SC2_START=0x%x, ISP_SC2_START=0x%x,ISP_DISP_SIZE=0x%x,ISP_DISP_START=0x%x,ISP_ENC_SIZE=0x%x,ISP_ENC_START=0x%x"
#define ISP_DRV_SC8800H_5344_112_2_18_1_38_2_773 "ISP: ISP_ISR6PortBFIFOOverflowed ,blank_per_line = %x,pixel_per_clk = %d,portb_threshold=%d"
#define ISP_DRV_SC8800H_5375_112_2_18_1_38_2_774 "ISP:ISP_ISR7SC1UpFIFOOverflowed"
#define ISP_DRV_SC8800H_5386_112_2_18_1_38_2_775 "ISP:ISP_ISR8CapFIFOOverflowed"
#define ISP_DRV_SC8800H_5397_112_2_18_1_38_2_776 "\r\nISP:ISP_ISR9AutoFocusEndOfFrame"
#define ISP_DRV_SC8800H_5406_112_2_18_1_38_2_777 "\r\nISP:ISP_ISRALCD2SC2StartOfFrame"
#define ISP_DRV_SC8800H_5417_112_2_18_1_38_2_778 "\r\nISP:ISP_ISRBPortCEndOfFrame"
#define ISP_DRV_SC8800H_5721_112_2_18_1_38_3_779 "ISP drv: ISP_OnPause"
#define ISP_DRV_SC8800H_5732_112_2_18_1_38_3_780 "ISP drv: ISP_OnPause, Reset ISP"
#define ISP_DRV_SC8800H_6080_112_2_18_1_38_4_781 "ISP DRV:s->pb_current = 0x%x"
#define ISP_DRV_SC8800H_6081_112_2_18_1_38_4_782 "ISP DRV:s->pb_frame = 0x%x"
#define ISP_DRV_SC8800H_6082_112_2_18_1_38_4_783 "ISP DRV:s->display.w = 0x%x,s->display.h = 0x%x"
#define IVSP_DRV_6X00_467_112_2_18_1_38_16_784 "[_IVsp_Open] ISR_RegHandler_Ex ret = %d"
#define IVSP_DRV_6X00_542_112_2_18_1_38_16_785 "IVsp_ModeSet: IVsp_Stop mode:%d"
#define IVSP_DRV_6X00_615_112_2_18_1_38_16_786 "ISP_SERVICE: IVsp_RegisterIntFunc interrupt num is invalid %d error"
#define IVSP_DRV_6X00_669_112_2_18_1_38_16_787 "ISP_SERVICE: IVsp_RegisterIntFuncEx interrupt num is invalid %d error"
#define IVSP_DRV_6X00_689_112_2_18_1_38_16_788 "ISP_SERVICE: IVsp_OpenInt interrupt num is invalid %d error"
#define IVSP_DRV_6X00_708_112_2_18_1_38_16_789 "ISP_SERVICE: IVsp_OpenInt interrupt num is invalid %d error"
#define IVSP_DRV_6X00_724_112_2_18_1_38_16_790 "ISP_SERVICE: IVsp_OpenInt interrupt num is invalid %d error"
#define IVSP_DRV_8800H5_478_112_2_18_1_38_17_791 "[_IVsp_Open] ISR_RegHandler_Ex ret = %d"
#define IVSP_DRV_8800H5_518_112_2_18_1_38_17_792 "IVsp_ModeSet: IVsp_Stop mode:%d"
#define IVSP_DRV_8800H5_549_112_2_18_1_38_18_793 "IVsp_SwitchMode: s_ivsp_mode %d, mode %d, is_enter %d"
#define IVSP_DRV_8800H5_558_112_2_18_1_38_18_794 "IVsp_SwitchMode: get mutex task id=0x%x"
#define IVSP_DRV_8800H5_564_112_2_18_1_38_18_795 "IVsp_SwitchMode: cur close task id=0x%x"
#define IVSP_DRV_8800H5_568_112_2_18_1_38_18_796 "IVsp_SwitchMode: put mutex task id=0x%x"
#define IVSP_DRV_SC6530_120_112_2_18_1_38_18_797 "IVsp_Open, thread id %d, mode %d, option %d, time %d"
#define IVSP_DRV_SC6530_124_112_2_18_1_38_18_798 "IVsp_Open, upsupported IVSP MODE!"
#define IVSP_DRV_SC6530_133_112_2_18_1_38_18_799 "IVsp_Open, Semaphore created!, 0x%x, time %d"
#define IVSP_DRV_SC6530_142_112_2_18_1_38_18_800 "IVsp_Open, time out %d"
#define IVSP_DRV_SC6530_149_112_2_18_1_38_18_801 "IVsp_Open, Got semaphore!, thread id %d, time %d"
#define IVSP_DRV_SC6530_156_112_2_18_1_38_18_802 "IVsp_Open, ivsp_mod.ivsp_sema 0x%x, time %d"
#define IVSP_DRV_SC6530_186_112_2_18_1_38_18_803 "IVsp_Close, thread id %d, mode %d, time %d"
#define IVSP_DRV_SC6530_193_112_2_18_1_38_18_804 "IVsp_Close, wrong ivsp mode %d, current working mode %d"
#define IVSP_DRV_SC6530_215_112_2_18_1_38_18_805 "IVsp_RegisterIntFunc, irq number %d, isr_func 0x%x"
#define IVSP_DRV_SC6530_254_112_2_18_1_38_18_806 "IVsp_UnRegisterIntFunc, irq number %d "
#define IVSP_DRV_SC6530_301_112_2_18_1_38_19_807 "_IVSP_LowISR, irq_line 0x%x"
#define IVSP_DRV_SC6530_319_112_2_18_1_38_19_808 "_IVSP_HighISR, irq_num 0x%x, data 0x%x"
#define JAPP_618_112_2_18_1_38_22_809 "[JPG_CreateMiniature] dec_buf_size = %d, used_size = %d"
#define JAPP_620_112_2_18_1_38_22_810 "[JPG_CreateMiniature] enc_buf_size = %d, used_size = %d"
#define JAPP_697_112_2_18_1_38_22_811 "[JPG_EncodeYUV444] size = (%d, %d), encode buf = 0x%x, size = %d"
#define JAPP_976_112_2_18_1_38_22_812 "[JPG_EncodeYUV422] size = (%d, %d), encode buf = 0x%x, size = %d"
#define JAPP_1149_112_2_18_1_38_23_813 "[JPG_DecodeToRGB] target buf = 0x%x, size = %d, target size = (%d, %d), src rect = (%x, %x, %d, %d)"
#define JAPP_1417_112_2_18_1_38_23_814 "[JPG_DecodeToRGB] scaled rect = (%x, %x, %d, %d)"
#define JAPP_1426_112_2_18_1_38_23_815 "[JPG_DecodeToRGB] output rect = (%x, %x, %d, %d)"
#define JAPP_1603_112_2_18_1_38_24_816 "[JPG_DecodeToRGB] scaled rect = (%x, %x, %d, %d)"
#define JAPP_1611_112_2_18_1_38_24_817 "[JPG_DecodeToRGB] output rect = (%x, %x, %d, %d)"
#define JAPP_1699_112_2_18_1_38_24_818 "[JPG_DecodeToRgb] decode to RGB OK! buf = 0x%x, w = %d, h = %d"
#define JAPP_1707_112_2_18_1_38_24_819 "[JPG_DecodeToRgb] dec_buf_size = %d, used_size = %d"
#define JAPP_2065_112_2_18_1_38_25_820 "[JPG_DecodeToYUV] dec_buf_size = %d, used_size = %d"
#define JPEGDEC_FRAME_301_112_2_18_1_39_21_821 "[START_HW_DECODE] flush failed, i = %d"
#define JPEGDEC_FRAME_1138_112_2_18_1_39_38_822 "Invalid raw_height, must be interger times of 8 or 16...\n"
#define JPEGDEC_FRAME_1172_112_2_18_1_39_38_823 "[JPEG_HWDecStartMCUSynchro] decode error, slice = %d"
#define JPEGDEC_FRAME_1178_112_2_18_1_39_38_824 "[JPEG_HWDecStartMCUSynchro] decode timeout, slice = %d"
#define JPEGDEC_FRAME_1209_112_2_18_1_39_38_825 "[JPEG_HWDecStartMCUSynchro] decode error, last slice"
#define JPEGDEC_FRAME_1215_112_2_18_1_39_38_826 "[JPEG_HWDecStartMCUSynchro] decode timeout, last slice"
#define JPEGDEC_FRAME_1225_112_2_18_1_39_38_827 "[JPEG_HWDecStartMCUSynchro] JPEG_HWWaitingEnd failed"
#define JPEGDEC_PARSE_998_112_2_18_1_39_43_828 "[JPEG_HWParseHead] find the SOI marker error: %x"
#define JPEGDEC_STREAM_197_112_2_18_1_39_44_829 "[JPEGFW_GetC] g_jpeg_fw_bsm_current_buffer_index = %d"
#define JPEGDEC_STREAM_203_112_2_18_1_39_44_830 "[JPEGFW_GetC] no more bytes, return error, cmd1 = 0x%x, total_read_size = %d"
#define JPEGDEC_STREAM_240_112_2_18_1_39_44_831 "[JPEGFW_GetC] g_jpeg_fw_bsm_current_buffer_index = %d"
#define JPEGDEC_STREAM_245_112_2_18_1_39_44_832 "[JPEGFW_GetC] no more bytes, return error, cmd0 = 0x%x, total_read_size = %d"
#define JPEGDEC_STREAM_328_112_2_18_1_39_44_833 "[JPEGFW_GetW] g_jpeg_fw_bsm_current_buffer_index = %d"
#define JPEGDEC_STREAM_333_112_2_18_1_39_44_834 "[JPEGFW_GetW] no more bytes, return error, cmd1 = 0x%x, total_read_size = %d"
#define JPEGDEC_FRAME_479_112_2_18_1_39_57_835 "[START_HW_DECODE] flush failed, i = %d"
#define JPEGDEC_FRAME_310_112_2_18_1_40_16_836 "[START_HW_DECODE] flush failed, i = %d"
#define JPEGDEC_FRAME_307_112_2_18_1_40_35_837 "[START_HW_DECODE] flush failed, i = %d"
#define EXIF_READER_2370_112_2_18_1_40_54_838 "[Jpeg_ParseExif] EXIF_READER_SUPPORT is not defined!"
#define EXIF_WRITER_2971_112_2_18_1_41_0_839 "[IMGJPEG_WriteExif] EXIF_WRITER_SUPPORT is not defined!"
#define EXIF_WRITER_3000_112_2_18_1_41_0_840 "[IMGJPEG_WriteExif] EXIF_WRITER_SUPPORT is not defined!"
#define JPEG_ALGORITHM_1528_112_2_18_1_41_3_841 "[YUVAlign] unsupport data format"
#define JPEG_EXIF_50_112_2_18_1_41_5_842 "JPEG_PaserExifInfo : in"
#define JPEG_EXIF_54_112_2_18_1_41_5_843 "JPEG_PaserExifInfo(PNULL, xxx, xxx)"
#define JPEG_EXIF_75_112_2_18_1_41_5_844 "JPEG_PaserExifInfo : width: %d, height: %d"
#define JPEG_EXIF_102_112_2_18_1_41_5_845 "JPEG_PaserExifInfo : out"
#define JPEG_HAL_44_112_2_18_1_41_8_846 "[JPEG_IsSoftwareScalingDown] need scaling down"
#define JPEG_HAL_49_112_2_18_1_41_8_847 "[JPEG_IsSoftwareScalingDown] do not need scaling down"
#define JPEG_HAL_78_112_2_18_1_41_8_848 "[JPEG_IsHardwarecalingDown], src_size = (%d, %d), src trim rect (%d, %d, %d, %d), target size = (%d, %d)"
#define JPEG_HAL_81_112_2_18_1_41_8_849 "[JPEG_IsHardwarecalingDown] target data = %d, src yuv = %d, target yuv = %d"
#define JPEG_HAL_104_112_2_18_1_41_8_850 "[JPEG_IsHardwareScalingDown] is_scaling = %d"
#define JPEG_HAL_159_112_2_18_1_41_8_851 "[JPEG_GetMCUSize] unsupport quality"
#define JPEG_HAL_341_112_2_18_1_41_9_852 "[JPEG_ParseRes] unsupport quality"
#define JPEG_HAL_368_112_2_18_1_41_9_853 "[JPEG_ParseRes] unsupport quality"
#define JPEG_HAL_408_112_2_18_1_41_9_854 "[JPEG_ParseStdRes], allocate dqt failed"
#define JPEG_HAL_423_112_2_18_1_41_9_855 "[JPEG_ParseStdRes], Jpeg_parser failed, return = %d"
#define JPEG_HAL_438_112_2_18_1_41_9_856 "[JPEG_ParseStdRes], parse quality failed"
#define JPEG_HAL_451_112_2_18_1_41_9_857 "[JPEG_ParseStdRes] w = %d, h = %d, format = %d, quality = %d, head size = %d"
#define JPEG_HAL_472_112_2_18_1_41_9_858 "[JPEG_ResParse] jpeg stream size smaller than jpeg head size, not res jpeg!"
#define JPEG_HAL_479_112_2_18_1_41_9_859 "[JPEG_ParseRes] jpeg res ID mismatch"
#define JPEG_HAL_514_112_2_18_1_41_9_860 "[JPEG_ParseRes] unsupport quality"
#define JPEG_HAL_541_112_2_18_1_41_9_861 "[JPEG_ParseRes] unsupport quality"
#define JPEG_HAL_550_112_2_18_1_41_9_862 "[JPEG_ParseRes] w = %d, h = %d, format = %d, quality = %d, head size = %d"
#define JPEG_HAL_571_112_2_18_1_41_9_863 "[JPEG_ParseSJPGRes] input pointer invalid, 0x%x, 0x%x"
#define JPEG_HAL_721_112_2_18_1_41_9_864 "[JPEG_ParseRes] warning, unsupporte format, use format"
#define JPEG_HEAD_INFO_50_112_2_18_1_41_10_865 "[Jpeg_ParseSOF] SOF, SOF length error, %d"
#define JPEG_HEAD_INFO_79_112_2_18_1_41_10_866 "[Jpeg_ParseSOF] SOF, the first component is not Y component, return error"
#define JPEG_HEAD_INFO_113_112_2_18_1_41_10_867 "[Jpeg_ParseSOF] SOF, sample factor of Y component error, %d"
#define JPEG_HEAD_INFO_119_112_2_18_1_41_10_868 "[Jpeg_ParseSOF] SOF, component error, %d"
#define JPEG_HEAD_INFO_207_112_2_18_1_41_10_869 "[Jpeg_ParseDQT] skipped marker failed, marker = 0x%x, size = %d"
#define JPEG_HEAD_INFO_283_112_2_18_1_41_10_870 "[Jpeg_ParseDHT] skipped marker failed, marker = 0x%x, size = %d"
#define JPEG_HEAD_INFO_390_112_2_18_1_41_10_871 "[Jpeg_ParseDRI] skipped marker failed,  size = %d"
#define JPEG_HEAD_INFO_440_112_2_18_1_41_10_872 "[Jpeg_ParseSOS] skipped marker failed, marker = 0x%x, size = %d"
#define JPEG_HEAD_INFO_505_112_2_18_1_41_11_873 "[Jpeg_SkipMarker] skipped marker failed, size = %d"
#define JPEG_HEAD_INFO_511_112_2_18_1_41_11_874 "[Jpeg_SkipMarker] skipped marker failed,  size = %d"
#define JPEG_HEAD_INFO_569_112_2_18_1_41_11_875 "[Jpeg_ParseMarker] the first marker is not SOI, marker = 0x%x"
#define JPEG_HEAD_INFO_577_112_2_18_1_41_11_876 "[Jpeg_ParseMarker] the first marker is not SOI, marker = 0x%x"
#define JPEG_HEAD_INFO_598_112_2_18_1_41_11_877 "[Jpeg_ParseMarker] find marker M_EOI, return error"
#define JPEG_HEAD_INFO_626_112_2_18_1_41_11_878 "[Jpeg_ParseMarker] an error occurred while parsing APP1 marker"
#define JPEG_HEAD_INFO_668_112_2_18_1_41_11_879 "[Jpeg_ParseMarker] unsupported marker, marker = 0x%x"
#define JPEG_HEAD_INFO_774_112_2_18_1_41_11_880 "[Jpeg_ParseHead] no source found"
#define JPEG_INTERFACE_289_112_2_18_1_41_12_881 "IMGJPEG_FreeRes, line %d, state = %d"
#define JPEG_INTERFACE_295_112_2_18_1_41_12_882 "IMGJPEG_FreeRes, state is free, do nothing!"
#define JPEG_INTERFACE_304_112_2_18_1_41_12_883 "IMGJPEG_FreeRes, no task, line %d"
#define JPEG_INTERFACE_313_112_2_18_1_41_12_884 "IMGJPEG_FreeRes, wait task stopped,line %d"
#define JPEG_INTERFACE_318_112_2_18_1_41_12_885 "IMGJPEG_FreeRes, working by software"
#define JPEG_INTERFACE_349_112_2_18_1_41_12_886 " IMGJPEG_GetInfo(), line %d, info_ptr = 0x%x"
#define JPEG_INTERFACE_387_112_2_18_1_41_12_887 "[IMGJPEG_GetInfo] stream in file = 0x%x, read buf = 0x%x, size = %d"
#define JPEG_INTERFACE_394_112_2_18_1_41_12_888 "[IMGJPEG_GetInfo] stream in buffer = 0x%x, size = %d"
#define JPEG_INTERFACE_401_112_2_18_1_41_12_889 "[IMGJPEG_GetInfo], Jpeg_parser failed, return = %d"
#define JPEG_INTERFACE_420_112_2_18_1_41_12_890 "[IMGJPEG_GetInfo] width = %d, height = %d, jpeg type = %d, sample format = %d"
#define JPEG_INTERFACE_453_112_2_18_1_41_12_891 "[IMGJPEG_GetInfo] get sjpg information"
#define JPEG_INTERFACE_474_112_2_18_1_41_12_892 "[IMGJPEG_GetInfo], parse sjpg failed, return = %d"
#define JPEG_INTERFACE_510_112_2_18_1_41_13_893 "[IMGJPEG_CreateMiniature] state = %d, target buf = 0x%x, size = %d, decode buf = 0x%x, size = %d,"
#define JPEG_INTERFACE_512_112_2_18_1_41_13_894 "[IMGJPEG_CreateMiniature] jpeg buf = 0x%x, size = %d"
#define JPEG_INTERFACE_529_112_2_18_1_41_13_895 "[IMGJPEG] software decode, nRet = %d"
#define JPEG_INTERFACE_543_112_2_18_1_41_13_896 "[IMGJPEG_CreateMiniature] unsupport format, re-decode it by software"
#define JPEG_INTERFACE_547_112_2_18_1_41_13_897 "[IMGJPEG_CreateMiniature] hardware decode, nRet = %d"
#define JPEG_INTERFACE_550_112_2_18_1_41_13_898 "[IMGJPEG_CreateMiniature] neither HW or SW decoder/encoder defined !"
#define JPEG_INTERFACE_554_112_2_18_1_41_13_899 "[IMGJPEG_CreateMiniature ] time = %d"
#define JPEG_INTERFACE_559_112_2_18_1_41_13_900 "[IMGJPEG_CreateMiniature] miniature disabled"
#define JPEG_INTERFACE_563_112_2_18_1_41_13_901 "[IMGJPEG_CreateMiniature] JPG_MINIATURE_SUPPORT not defined !"
#define JPEG_INTERFACE_643_112_2_18_1_41_13_902 "[IMGJPEG_Encode] state = %d, w = %d, h = %d, sample format = %d, quality = %d"
#define JPEG_INTERFACE_647_112_2_18_1_41_13_903 "[IMGJPEG_Encode] target buf = 0x%x, size = %d, encode buf = 0x%x, size = %d,"
#define JPEG_INTERFACE_697_112_2_18_1_41_13_904 "[IMGJPEG_Encode] temp buf, addr = 0x%x, size  = %d, used size = %d"
#define JPEG_INTERFACE_699_112_2_18_1_41_13_905 "[IMGJPEG_Encode] hardware encode, ret = %d"
#define JPEG_INTERFACE_703_112_2_18_1_41_13_906 "[IMGJPEG] hardware encode, unsupport format,"
#define JPEG_INTERFACE_712_112_2_18_1_41_13_907 "[IMGJPEG] software encode, ret = %d,"
#define JPEG_INTERFACE_716_112_2_18_1_41_13_908 "[IMGJPEG] software encode, unsupport format,"
#define JPEG_INTERFACE_724_112_2_18_1_41_13_909 "[IMGJPEG] Exit IMGJPEG_Encode, time = %d"
#define JPEG_INTERFACE_798_112_2_18_1_41_13_910 "[IMGJPEG_Decode] target buf = 0x%x, size = %d, decode buf = 0x%x, size = %d,"
#define JPEG_INTERFACE_802_112_2_18_1_41_13_911 "[IMGJPEG_Decode] target size = (%d, %d), target_data_type = %d, jpeg buf = 0x%x, size = %d"
#define JPEG_INTERFACE_837_112_2_18_1_41_13_912 "[IMGJPEG_GetInfo], Jpeg_parser failed, return = %d"
#define JPEG_INTERFACE_849_112_2_18_1_41_13_913 "[IMGJPEG_Decode] YUV422R JPEG, convert to YUV422 on platform 6600l"
#define JPEG_INTERFACE_854_112_2_18_1_41_13_914 "[IMGJPEG_Decode] jpeg info: w = %d, h = %d, format = %d, is rotated_yuv422 = %d"
#define JPEG_INTERFACE_863_112_2_18_1_41_13_915 "[IMGJPEG_Decode] hardware decode baseline jpeg"
#define JPEG_INTERFACE_872_112_2_18_1_41_13_916 "[IMGJPEG_Decode] software decode progressive jpeg, jpeg buf = %d, read file func = 0x%x"
#define JPEG_INTERFACE_882_112_2_18_1_41_13_917 "[IMGJPEG_Decode] size of progressive jpeg bigger than allowed !"
#define JPEG_INTERFACE_886_112_2_18_1_41_13_918 "[IMGJPEG_Decode] JPG_DEC_TYPE_SUPPORT_BASELINE_PROGRESSIVE undefined !"
#define JPEG_INTERFACE_900_112_2_18_1_41_13_919 "[IMGJPEG_Decode] software decode baseline jpeg, jpeg buf = %d, read file func = 0x%x"
#define JPEG_INTERFACE_910_112_2_18_1_41_13_920 "[IMGJPEG_Decode] software decode progressive jpeg, jpeg buf = %d, read file func = 0x%x"
#define JPEG_INTERFACE_919_112_2_18_1_41_13_921 "[IMGJPEG_Decode] size of progressive jpeg bigger than allowed !"
#define JPEG_INTERFACE_923_112_2_18_1_41_13_922 "[IMGJPEG_Decode] JPG_DEC_TYPE_SUPPORT_BASELINE_PROGRESSIVE undefined !"
#define JPEG_INTERFACE_937_112_2_18_1_41_13_923 "[IMGJPEG_Decode] Exit IMGJPEG_Decode. nRet = %d, time = %d"
#define JPEG_INTERFACE_1012_112_2_18_1_41_13_924 "IMGJPEG_Decode_Res(), line %d, state = %d, in_param_ptr = 0x%x, out_param_ptr = 0x%x"
#define JPEG_INTERFACE_1044_112_2_18_1_41_14_925 "[_DecodeRes] convert STD JPG to SJPG"
#define JPEG_INTERFACE_1053_112_2_18_1_41_14_926 "[_DecodeRes] convert std jpeg to sjpg failed!"
#define JPEG_INTERFACE_1067_112_2_18_1_41_14_927 "[_DecodeRes] jpeg buf = 0x%x, jpeg_size = %d"
#define JPEG_INTERFACE_1108_112_2_18_1_41_14_928 "[IMGJPEG] Exit IMGJPEG_Decode_Res. nRet = %d, time = %d"
#define JPEG_INTERFACE_1175_112_2_18_1_41_14_929 "[IMGJPEG_EnableMiniature] enable = %d"
#define JPEG_INTERFACE_1178_112_2_18_1_41_14_930 "[IMGJPEG_EnableMiniature] JPG_MINIATURE_SUPPORT not defined"
#define JPEG_ISR_157_112_2_18_1_41_15_931 "[JPEG_BSM_intHandler] encode drop the BSM INT after VLC done"
#define JPEG_MIDWARE_171_112_2_18_1_41_17_932 "[JPEG_SetCaller] caller_flag = %d"
#define JPEG_MIDWARE_192_112_2_18_1_41_17_933 "[JPEG_IspLock] lock isp failed"
#define JPEG_MIDWARE_233_112_2_18_1_41_17_934 "[JPEG_IspOpen] ISP_ServiceOpen timeout"
#define JPEG_MIDWARE_267_112_2_18_1_41_17_935 "[JPEG_OpenIspService] is_open = %d"
#define JPEG_MIDWARE_298_112_2_18_1_41_17_936 "[JPEG_CloseIspService] is_open = %d"
#define JPEG_MIDWARE_429_112_2_18_1_41_17_937 "[JPEG_DecInitScaleSliceBuf], allcate scale buffer failed"
#define JPEG_MIDWARE_446_112_2_18_1_41_17_938 "[JPEG_DecInitScaleSliceBuf], allcate scale swap buffer failed"
#define JPEG_MIDWARE_462_112_2_18_1_41_17_939 "[JPEG_DecInitScaleSliceBuf], allcate scale swap buffer failed"
#define JPEG_MIDWARE_546_112_2_18_1_41_18_940 "[JPEG_DecScaleInit], allcate line buffer and scale mem failed"
#define JPEG_MIDWARE_575_112_2_18_1_41_18_941 "[JPEG_DecScaleInit] YUV data type not supported"
#define JPEG_MIDWARE_633_112_2_18_1_41_18_942 "[JPEG_InitSoftwareScaleBuf] YUV data type not supported"
#define JPEG_MIDWARE_654_112_2_18_1_41_18_943 "[JPEG_DecInitScaleSliceBuf], allcate scale y buffer failed"
#define JPEG_MIDWARE_660_112_2_18_1_41_18_944 "[JPEG_DecInitScaleSliceBuf], allcate scale u buffer failed"
#define JPEG_MIDWARE_734_112_2_18_1_41_18_945 "[JPEG_DecSubsampleSlice] unaligned slice height %d"
#define JPEG_MIDWARE_746_112_2_18_1_41_18_946 "[JPEG_DecSubsampleSlice] unaligned slice height %d"
#define JPEG_MIDWARE_757_112_2_18_1_41_18_947 "[JPEG_DecSubsampleSlice] unsupport subsample factor = %d"
#define JPEG_MIDWARE_917_112_2_18_1_41_18_948 "[JPEG_DecWriteYUV] unsupport YUV format, %d"
#define JPEG_MIDWARE_1030_112_2_18_1_41_19_949 "[JPEG_DecYUVToRgb565] unsupporte target sample format"
#define JPEG_MIDWARE_1045_112_2_18_1_41_19_950 "[JPEG_DecYUVToRgb565] error! yuv2rgb failed !target_sample_format = %d, actual write lines = %d"
#define JPEG_MIDWARE_1114_112_2_18_1_41_19_951 "[JPEG_DecYUVToArgb8888] unsupporte target sample format"
#define JPEG_MIDWARE_1130_112_2_18_1_41_19_952 "[JPEG_DecYUVToArgb8888] error! yuv2rgb failed !target_sample_format = %d, actual write lines = %d"
#define JPEG_MIDWARE_1179_112_2_18_1_41_19_953 "[JPEG_DecGetSubsampleFactor] unsupport sample format %d"
#define JPEG_MIDWARE_1231_112_2_18_1_41_19_954 "[JPEG_DecGetSubsampleFactor] revise scaling shift = %d"
#define JPEG_MIDWARE_1306_112_2_18_1_41_19_955 "[JPEG_DecGetSoftSubsampleFactor] warning !! total_subsample_factor (%d) < hardware_subsample_factor (%d)"
#define JPEG_MIDWARE_1386_112_2_18_1_41_19_956 "[JPEG_GetOutDataType] yuv data type not supported!"
#define JPEG_MIDWARE_1578_112_2_18_1_41_20_957 "[JPEG_CalcSliceHeight] error return, memory not enough"
#define JPEG_MIDWARE_1601_112_2_18_1_41_20_958 "[JPEG_CalcSliceHeight] error, memory not enough"
#define JPEG_MIDWARE_1611_112_2_18_1_41_20_959 "[JPEG_CalcSliceHeight] slice_height = %d"
#define JPEG_MIDWARE_1629_112_2_18_1_41_20_960 "[JPEG_DecScaleCallback], scale failed, scaled_output_lines = %d"
#define JPEG_MIDWARE_1638_112_2_18_1_41_20_961 "[JPEG_DecScaleCallback], scale failed, scaled_output_lines = %d"
#define JPEG_MIDWARE_1688_112_2_18_1_41_20_962 "[JPEG_DecSetHardwareScaleParam] do not need scaling down"
#define JPEG_MIDWARE_1695_112_2_18_1_41_20_963 "[JPEG_DecSetHardwareScaleParam] init hardware scale failed"
#define JPEG_MIDWARE_1732_112_2_18_1_41_20_964 "[JPEG_DecSetHardwareScaleParam] resize the dst size = (%d X %d)"
#define JPEG_MIDWARE_1741_112_2_18_1_41_20_965 "[JPEG_DecSetHardwareScaleParam] error dst size (%d, %d)"
#define JPEG_MIDWARE_1762_112_2_18_1_41_20_966 "[JPEG_DecSetHardwareScaleParam] unsupported input data format %d"
#define JPEG_MIDWARE_1809_112_2_18_1_41_20_967 "[JPEG_DecSetHardwareScaleParam] error scaling factor, src (%d, %d), dst (%d, %d)"
#define JPEG_MIDWARE_1850_112_2_18_1_41_20_968 "[JPEG_DecCalculateParam], JPEG_DecScaleInit failed = %d"
#define JPEG_MIDWARE_1894_112_2_18_1_41_20_969 "[JPEG_DecCalculateParam] need soft scale down, output size = (%d, %d)"
#define JPEG_MIDWARE_1917_112_2_18_1_41_21_970 "[JPEG_DecSetSoftwareScaleParam] unsupport sample format %d"
#define JPEG_MIDWARE_1946_112_2_18_1_41_21_971 "[JPEG_DecSetSoftwareScaleParam] invalid crop size, src rect (%dX%d), crop size (%dX%d)"
#define JPEG_MIDWARE_2075_112_2_18_1_41_21_972 "[JPEG_DecSetWorkMode] work mode = %d"
#define JPEG_MIDWARE_2145_112_2_18_1_41_21_973 "[JPEG_DecCalculateParam] unsupport scaling down factor"
#define JPEG_MIDWARE_2162_112_2_18_1_41_21_974 "[JPEG_DecCalculateParam] total factor = %d, hard factor = %d, soft factor = %d, mode = %d"
#define JPEG_MIDWARE_2247_112_2_18_1_41_21_975 "[JPEG_DecCalculateParam] subsampled slice height is not enough, %d"
#define JPEG_MIDWARE_2310_112_2_18_1_41_21_976 "yuv format %d -> unsupported input format"
#define JPEG_MIDWARE_2324_112_2_18_1_41_21_977 "[JPEG_InitSubsampledBuffer] alloc subsampled yuv buffer failed"
#define JPEG_MIDWARE_2358_112_2_18_1_41_21_978 "[JPEG_DecInitSliceStreamBuffer] alloc stream pingpong buffer failed"
#define JPEG_MIDWARE_2373_112_2_18_1_41_21_979 "[JPEG_DecInitSliceStreamBuffer] alloc stream pingpong buffer failed"
#define JPEG_MIDWARE_2390_112_2_18_1_41_21_980 "[JPEG_Decode_SC6600L] alloc stream pingpong buffer failed"
#define JPEG_MIDWARE_2404_112_2_18_1_41_22_981 "[JPEG_DecInitSliceStreamBuffer], file in buffer, buf = 0x%x, size = %d"
#define JPEG_MIDWARE_2459_112_2_18_1_41_22_982 "yuv format %d -> unsupported input format"
#define JPEG_MIDWARE_2483_112_2_18_1_41_22_983 "[JPEG_DecInitSliceYUVBuffer] alloc yuv pingpong buffer failed"
#define JPEG_MIDWARE_2502_112_2_18_1_41_22_984 "[JPEG_DecInitSliceYUVBuffer] alloc yuv pingpong buffer failed"
#define JPEG_MIDWARE_2540_112_2_18_1_41_22_985 "[JPEG_DecInitDecBuffer] alloc dec buffer failed"
#define JPEG_MIDWARE_2570_112_2_18_1_41_22_986 "[JPEG_DecInitTargetBuffer] alloc convert slice buffer failed"
#define JPEG_MIDWARE_2594_112_2_18_1_41_22_987 "[JPEG_DecInitTargetBuffer] unsupport target rgb type"
#define JPEG_MIDWARE_2654_112_2_18_1_41_22_988 "[JPEG_DecInitTargetBuffer] alloc target rgb565 buffer failed"
#define JPEG_MIDWARE_2678_112_2_18_1_41_22_989 "[JPEG_DecInitTargetBuffer] alloc rgb565 slice buffer failed"
#define JPEG_MIDWARE_2732_112_2_18_1_41_22_990 "[JPEG_DecInitTargetBuffer] alloc target y buffer failed"
#define JPEG_MIDWARE_2739_112_2_18_1_41_22_991 "[JPEG_DecInitTargetBuffer] alloc target uv buffer failed"
#define JPEG_MIDWARE_2756_112_2_18_1_41_22_992 "[JPEG_DecInitTargetBuffer] is_frame_mode = %d"
#define JPEG_MIDWARE_2767_112_2_18_1_41_22_993 "[JPEG_DecInitTargetBuffer] alloc convert slice buffer failed"
#define JPEG_MIDWARE_2779_112_2_18_1_41_22_994 "[JPEG_DecInitTargetBuffer] unsupport target data type"
#define JPEG_MIDWARE_2817_112_2_18_1_41_22_995 "[JPEG_DecInitFwParam] unsupport scaling down factor, use default"
#define JPEG_MIDWARE_2896_112_2_18_1_41_22_996 "[JPEG_Decode_SC6600L] decode success, out_w = %d, out_h = %d, rgb buf = 0x%x"
#define JPEG_MIDWARE_2910_112_2_18_1_41_23_997 "[JPEG_Decode_SC6600L] decode success, format = %d, out_w = %d, out_h = %d, out_y_addr = 0x%x, out_u_addr = 0x%x"
#define JPEG_MIDWARE_2930_112_2_18_1_41_23_998 "[JPEG_InitializeHardware]"
#define JPEG_MIDWARE_2942_112_2_18_1_41_23_999 "[JPEG_ReleaseHardware] s_is_vsp_open = %d"
#define JPEG_MIDWARE_3063_112_2_18_1_41_23_1000 "JPEG_SyncHalDecode failed = %d"
#define JPEG_MIDWARE_3101_112_2_18_1_41_23_1001 "[JPEG_StartDecode] DecodeInit/DecEncSwitchInit failed = %d"
#define JPEG_MIDWARE_3119_112_2_18_1_41_23_1002 "[JPEG_StartDecode] JPEG_HWParseHead failed = %d"
#define JPEG_MIDWARE_3286_112_2_18_1_41_23_1003 "[JPEG_DecodeWritePingPongBuffer] no stream in stream buffer, read size = %d, decoded size = %d, buf_id = %d"
#define JPEG_MIDWARE_3322_112_2_18_1_41_23_1004 "[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed"
#define JPEG_MIDWARE_3358_112_2_18_1_41_23_1005 "[JPEG_HardwareScaleYUV] scaled height reaches the output height. %d"
#define JPEG_MIDWARE_3388_112_2_18_1_41_24_1006 "[JPEG_HardwareScaleYUV] unsupport input yuv format"
#define JPEG_MIDWARE_3436_112_2_18_1_41_24_1007 "[JPEG_HardwareScaleYUV] error: input slice line is zero, return"
#define JPEG_MIDWARE_3479_112_2_18_1_41_24_1008 "[JPEG_IsHardwareScaleSupport] hardware scaling unsupported!in size (%d, %d), out size (%d, %d)"
#define JPEG_MIDWARE_3801_112_2_18_1_41_24_1009 "[JPEG_OutputYUV]unsupport sample format %d, use default value"
#define JPEG_MIDWARE_3894_112_2_18_1_41_25_1010 "[JPEG_DecProcYUV] JPEG_ScaleYUV failed!ouput size (%d, %d), format = %d "
#define JPEG_MIDWARE_4005_112_2_18_1_41_25_1011 "[JPEG_DecodeErrorRestart] error restart, offset = %d, error_counter = %d, switch count = %d, dec_stream_size = %d"
#define JPEG_MIDWARE_4037_112_2_18_1_41_25_1012 "[IMGJPEG_Decode] img size error, w = %d, h = %d"
#define JPEG_MIDWARE_4045_112_2_18_1_41_25_1013 "[IMGJPEG_Decode] target img size error, w = %d, h = %d"
#define JPEG_MIDWARE_4076_112_2_18_1_41_25_1014 "[JPEG_HardwareDecode] change the src size to the mcu aligned size"
#define JPEG_MIDWARE_4152_112_2_18_1_41_25_1015 "[IMGJPEG_Decode] unsupported target data type !"
#define JPEG_MIDWARE_4162_112_2_18_1_41_25_1016 "[JPEG_HardwareDecode] temp buf, addr = 0x%x, size  = %d, used size = %d"
#define JPEG_MIDWARE_4263_112_2_18_1_41_25_1017 "[JPEG_HalGetDecodeBufSize] unsupport subsample factor = %d"
#define JPEG_MIDWARE_4304_112_2_18_1_41_26_1018 "[JPEG_HalGetDecodeBufSize] src size = (%d, %d), crop rect = (%d, %d, %d, %d), mcu_height = %d"
#define JPEG_MIDWARE_4308_112_2_18_1_41_26_1019 "[JPEG_HalGetDecodeBufSize] slice height = %d, aligned size = (%d, %d), subsample_factor = %d, hal_subsample_factor = %d, soft_subsample_factor = %d"
#define JPEG_MIDWARE_4345_112_2_18_1_41_26_1020 "[JPEG_HalGetDecodeBufSize], temp_buf_size = %d"
#define JPEG_MIDWARE_4356_112_2_18_1_41_26_1021 "[JPEG_HalGetDecodeBufSize] encode temp buffer = %d"
#define JPEG_MIDWARE_4367_112_2_18_1_41_26_1022 "[JPEG_HalGetDecodeBufSize] temp buf size = %d, ret = %d"
#define JPEG_MIDWARE_4481_112_2_18_1_41_26_1023 "JPEG_HWDecInit failed = %d"
#define JPEG_MIDWARE_4613_112_2_18_1_41_26_1024 "[JPEG_EncReadYUVEx] read yuv return 0, total read lines = %d"
#define JPEG_MIDWARE_4796_112_2_18_1_41_27_1025 "[JPEG_EncReadYUV] unsupport YUV format, %d, use default value"
#define JPEG_MIDWARE_4938_112_2_18_1_41_27_1026 "[JPEG_EncSetReadFunc] change read func to JPEG_EncReadYUVEx"
#define JPEG_MIDWARE_5048_112_2_18_1_41_27_1027 "[JPEG_EncInitParam] unsupport encode size (%d X %d)"
#define JPEG_MIDWARE_5068_112_2_18_1_41_27_1028 "[JPEG_EncInitParam] src size (%d, %d), mcu aligned size (%d, %d), slice height = %d, mode = %d, work_mode = %d"
#define JPEG_MIDWARE_5091_112_2_18_1_41_27_1029 "[JPEG_EncInitAlignBuffer] allocate yuv align buffer failed!"
#define JPEG_MIDWARE_5133_112_2_18_1_41_27_1030 "[JPEG_EncInitReadYUVBuffer] allocate yuv pingpong buffer failed!"
#define JPEG_MIDWARE_5207_112_2_18_1_41_27_1031 "[JPEG_EncInitSliceYUVBuffer] alloc yuv pingpong buffer failed"
#define JPEG_MIDWARE_5225_112_2_18_1_41_27_1032 "[JPEG_EncInitSliceYUVBuffer] alloc yuv pingpong buffer failed"
#define JPEG_MIDWARE_5259_112_2_18_1_41_27_1033 "[JPEG_EncInitTargetBuffer] allocate target buffer failed, encode memory not enough"
#define JPEG_MIDWARE_5333_112_2_18_1_41_28_1034 "[JPEG_EncInitSliceStreamBuffer] allocate stream pingpong buffer failed, encode memory not enough"
#define JPEG_MIDWARE_5349_112_2_18_1_41_28_1035 "[JPEG_EncInitSliceStreamBuffer] need copy to target = %d"
#define JPEG_MIDWARE_5513_112_2_18_1_41_28_1036 "[JPEG_HalEncode]src_width = %d, src_height = %d, quality = %d"
#define JPEG_MIDWARE_5517_112_2_18_1_41_28_1037 "[JPEG_HalEncode] src yuv pattern = %d, src yuv format = %d"
#define JPEG_MIDWARE_5522_112_2_18_1_41_28_1038 "[JPEG_HalEncode]src y buf = 0x%x, src u buf size = 0x%x, src v buf size = 0x%x"
#define JPEG_MIDWARE_5531_112_2_18_1_41_28_1039 "JPEG_SyncHalEncode failed = %d"
#define JPEG_MIDWARE_5542_112_2_18_1_41_28_1040 "[JPEG_Encode_SC6600L] output jpeg buf = 0x%x, size = %d"
#define JPEG_MIDWARE_5720_112_2_18_1_41_28_1041 "JPEG_HWEncInit failed = %d"
#define JPEG_MIDWARE_5729_112_2_18_1_41_28_1042 "JPEG_HWWriteHead failed = %d"
#define JPEG_MIDWARE_5816_112_2_18_1_41_29_1043 "[JPEG_StartEncode] JPEG_EncodeInit failed!"
#define JPEG_MIDWARE_5908_112_2_18_1_41_29_1044 "[JPEG_StopEncode] jpeg stream buffer is not enough, stream size = %d, target buffer size = %d"
#define JPEG_MIDWARE_5921_112_2_18_1_41_29_1045 "[JPEG_StopEncode] last size(%d) > stream buffer size(%d)"
#define JPEG_MIDWARE_6063_112_2_18_1_41_29_1046 "[JPEG_EncodeReadPingPongBuffer] jpeg stream buffer is not enough"
#define JPEG_MIDWARE_6114_112_2_18_1_41_29_1047 "[JPEG_SwitchWriteYUV] error! backup lines is not ZERO (%d)"
#define JPEG_MIDWARE_6195_112_2_18_1_41_29_1048 "[JPEG_SwitchEncReadYUV] unsupport sample format, %d, use default value"
#define JPEG_MIDWARE_6227_112_2_18_1_41_29_1049 "[JPEG_SwitchReadYUV] no more lines"
#define JPEG_MIDWARE_6387_112_2_18_1_41_30_1050 "JPEG_SyncHalDecode failed = %d"
#define JPEG_MIDWARE_6398_112_2_18_1_41_30_1051 "[JPEG_HalDecEncSwitch] create miniature success, output buf = 0x%x, size = %d, w = %d, h = %d"
#define JPEG_MIDWARE_6473_112_2_18_1_41_30_1052 "[IMGJPEG_GetInfo] stream in file = 0x%x, read buf = 0x%x, size = %d"
#define JPEG_MIDWARE_6480_112_2_18_1_41_30_1053 "[IMGJPEG_GetInfo] stream in buffer = 0x%x, size = %d"
#define JPEG_MIDWARE_6488_112_2_18_1_41_30_1054 "[JPEG_CreateMiniature], Jpeg_parser failed, return = %d"
#define JPEG_MIDWARE_6496_112_2_18_1_41_30_1055 "[JPEG_CreateMiniature] progressive file"
#define JPEG_MIDWARE_6504_112_2_18_1_41_30_1056 "[JPEG_CreateMiniature] img size error, w = %d, h = %d"
#define JPEG_MIDWARE_6513_112_2_18_1_41_30_1057 "[JPEG_CreateMiniature] target img size error, w = %d, h = %d"
#define JPEG_MIDWARE_6599_112_2_18_1_41_30_1058 "[JPEG_CreateMiniature] memory not enough, enc temp buf(%d) >= dec temp buf (%d)"
#define JPEG_MIDWARE_6625_112_2_18_1_41_30_1059 "[JPEG_HalDecEncSwitch] dec temp buf, addr = 0x%x, size  = %d, used size = %d"
#define JPEG_MIDWARE_6627_112_2_18_1_41_30_1060 "[JPEG_HalDecEncSwitch] enc temp buf, addr = 0x%x, size  = %d, used size = %d"
#define JPEG_MIDWARE_6657_112_2_18_1_41_30_1061 "[JPEG_YUV2RGBCallback], addr.y_addr = %d, size = %dX%d"
#define JPEG_MIDWARE_6692_112_2_18_1_41_30_1062 "[JPEG_YUV2RGBHardware] invalid input parameters"
#define JPEG_MIDWARE_6702_112_2_18_1_41_30_1063 "[JPEG_YUV2RGBHardware] invalid input parameters"
#define JPEG_MIDWARE_6712_112_2_18_1_41_30_1064 "[JPEG_YUV2RGBHardware] trim rect invalid (%d, %d)"
#define JPEG_MIDWARE_6771_112_2_18_1_41_30_1065 "[JPEG_YUV2RGBHardware], dst format error %d"
#define JPEG_MIDWARE_6778_112_2_18_1_41_30_1066 "[JPEG_YUV2RGBHardware], dst buffer size is not enough, %d, %d"
#define JPEG_MIDWARE_6788_112_2_18_1_41_30_1067 "[JPEG_YUV2RGBHardware] open isp failed!"
#define JPEG_MIDWARE_6813_112_2_18_1_41_30_1068 "[JPEG_YUV2RGBHardware] convert rgb888 to argb888, argb888 buf = 0x%x, size (%d, %d)"
#define JPEG_MIDWARE_6823_112_2_18_1_41_30_1069 "[JPEG_YUV2RGBHardware] convert rgb565 to argb888, argb888 buf = 0x%x, rgb565 buf = 0x%x, size (%d, %d)"
#define JPEG_MIDWARE_7050_112_2_18_1_41_31_1070 "[JPEG_Decode_Res] unsupported sample format %d"
#define JPEG_MIDWARE_7142_112_2_18_1_41_31_1071 "[JPEG_Decode_Res_SC6600L] alloc target y, and v buffer failed"
#define JPEG_MIDWARE_7161_112_2_18_1_41_31_1072 "[JPEG_Decode_Res_SC6600L] alloc target RGB buffer failed"
#define JPEG_MIDWARE_7185_112_2_18_1_41_31_1073 "JPEG_HWParseHead failed = %d"
#define JPEG_MIDWARE_7196_112_2_18_1_41_31_1074 "[JPEG_Decode_Res_SC6600L] JPEG_HWDecInit failed, %d"
#define JPEG_MIDWARE_7219_112_2_18_1_41_31_1075 "[JPEG_Decode_Res_SC6600L], eRet = %d, output_ptr = 0x%x, size = %d, output_width = %d, output_height = %d, actual_width = %d, actual_height = %d"
#define JPEG_MIDWARE_7223_112_2_18_1_41_31_1076 "[JPEG_Decode_Res_SC6600L] JPEG_HWDecStartSynchro, %d"
#define JPEG_MIDWARE_SC6600L_136_112_2_18_1_41_32_1077 "[JPEG_DecScaleInit], allcate line buffer and scale mem failed"
#define JPEG_MIDWARE_SC6600L_345_112_2_18_1_41_32_1078 "[JPEG_DecInitScaleSliceBuf], allcate scale y buffer failed"
#define JPEG_MIDWARE_SC6600L_351_112_2_18_1_41_32_1079 "[JPEG_DecInitScaleSliceBuf], allcate scale u buffer failed"
#define JPEG_MIDWARE_SC6600L_530_112_2_18_1_41_33_1080 "[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed"
#define JPEG_MIDWARE_SC6600L_589_112_2_18_1_41_33_1081 "[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed"
#define JPEG_MIDWARE_SC6600L_778_112_2_18_1_41_33_1082 "[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed"
#define JPEG_MIDWARE_SC6600L_837_112_2_18_1_41_33_1083 "[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed"
#define JPEG_MIDWARE_SC6600L_1244_112_2_18_1_41_34_1084 "[JPEG_DecGetSubsampleFactor] revise scaling shift = %d"
#define JPEG_MIDWARE_SC6600L_1514_112_2_18_1_41_34_1085 "[JPEG_EncReadYUVSlice] read lines = %d, src_height = %d, src_align_height"
#define JPEG_MIDWARE_SC6600L_1530_112_2_18_1_41_35_1086 "[JPEG_EncReadYUVSlice] read lines = %d, fill up the dummy lines = %d"
#define JPEG_MIDWARE_SC6600L_1679_112_2_18_1_41_35_1087 "[JPEG_EncInitSliceYUVBuffer] allocate yuv pingpong buffer failed!"
#define JPEG_MIDWARE_SC6600L_1699_112_2_18_1_41_35_1088 "[JPEG_EncInitSliceYUVBuffer] allocate yuv read buffer failed!"
#define JPEG_MIDWARE_SC6600L_1741_112_2_18_1_41_35_1089 "[JPEG_EncInitTargetBuffer] allocate target buffer failed, encode memory not enough"
#define JPEG_MIDWARE_SC6600L_1803_112_2_18_1_41_35_1090 "[JPEG_EncInitSliceStreamBuffer] allocate stream pingpong buffer failed, encode memory not enough"
#define JPEG_MIDWARE_SC6600L_1960_112_2_18_1_41_35_1091 "[JPEG_CalcSliceHeight] error return, temp buf size (%d) < used_buf_size (%d)"
#define JPEG_MIDWARE_SC6600L_1998_112_2_18_1_41_35_1092 "[JPEG_CalcSliceHeight] slice_height = %d"
#define JPEG_MIDWARE_SC6600L_2006_112_2_18_1_41_35_1093 "[JPEG_CalcSliceHeight] error return, memory not enough"
#define JPEG_MIDWARE_SC6600L_2064_112_2_18_1_41_36_1094 "[JPEG_DecCalculateParam] unsupport scaling down factor"
#define JPEG_MIDWARE_SC6600L_2078_112_2_18_1_41_36_1095 "[JPEG_DecCalculateParam] total factor = %d, hard factor = %d, soft factor = %d, mode = %d"
#define JPEG_MIDWARE_SC6600L_2158_112_2_18_1_41_36_1096 "[JPEG_DecCalculateParam], JPEG_DecScaleInit failed = %d"
#define JPEG_MIDWARE_SC6600L_2202_112_2_18_1_41_36_1097 "[JPEG_DecCalculateParam] need soft scale down, output size = (%d, %d)"
#define JPEG_MIDWARE_SC6600L_2278_112_2_18_1_41_36_1098 "[JPEG_DecCalculateParam] subsampled slice height is not enough, %d"
#define JPEG_MIDWARE_SC6600L_2346_112_2_18_1_41_36_1099 "[JPEG_InitSubsampledBuffer] alloc subsampled yuv buffer failed"
#define JPEG_MIDWARE_SC6600L_2376_112_2_18_1_41_36_1100 "[JPEG_DecInitSliceStreamBuffer] alloc stream pingpong buffer failed"
#define JPEG_MIDWARE_SC6600L_2391_112_2_18_1_41_36_1101 "[JPEG_DecInitSliceStreamBuffer] alloc stream pingpong buffer failed"
#define JPEG_MIDWARE_SC6600L_2408_112_2_18_1_41_36_1102 "[JPEG_Decode_SC6600L] alloc stream pingpong buffer failed"
#define JPEG_MIDWARE_SC6600L_2422_112_2_18_1_41_36_1103 "[JPEG_DecInitSliceStreamBuffer], file in buffer, buf = 0x%x, size = %d"
#define JPEG_MIDWARE_SC6600L_2439_112_2_18_1_41_36_1104 "[JPEG_DecInitSliceStreamBuffer], empty jpeg, no stream read in ping buffer, exit"
#define JPEG_MIDWARE_SC6600L_2447_112_2_18_1_41_36_1105 "[JPEG_DecInitSliceStreamBuffer] jpeg stream size less than jpeg stream buffer ,reset the stream buffer size to %d"
#define JPEG_MIDWARE_SC6600L_2465_112_2_18_1_41_36_1106 "[JPEG_DecInitSliceStreamBuffer], no stream read in pong buffer, total read size = %d"
#define JPEG_MIDWARE_SC6600L_2526_112_2_18_1_41_36_1107 "[JPEG_DecInitSliceYUVBuffer] alloc y pingpong buffer failed"
#define JPEG_MIDWARE_SC6600L_2542_112_2_18_1_41_36_1108 "[JPEG_DecInitSliceYUVBuffer] alloc uv pingpong buffer failed"
#define JPEG_MIDWARE_SC6600L_2559_112_2_18_1_41_37_1109 "[JPEG_DecInitSliceYUVBuffer] alloc rotated yuv422 buffer failed"
#define JPEG_MIDWARE_SC6600L_2594_112_2_18_1_41_37_1110 "[JPEG_DecInitDecBuffer] alloc dec buffer failed"
#define JPEG_MIDWARE_SC6600L_2655_112_2_18_1_41_37_1111 "[JPEG_DecInitTargetBuffer] alloc target rgb565 buffer failed"
#define JPEG_MIDWARE_SC6600L_2669_112_2_18_1_41_37_1112 "[JPEG_DecInitTargetBuffer] alloc rgb565 slice buffer failed"
#define JPEG_MIDWARE_SC6600L_2720_112_2_18_1_41_37_1113 "[JPEG_DecInitTargetBuffer] alloc target y buffer failed"
#define JPEG_MIDWARE_SC6600L_2727_112_2_18_1_41_37_1114 "[JPEG_DecInitTargetBuffer] alloc target uv buffer failed"
#define JPEG_MIDWARE_SC6600L_2911_112_2_18_1_41_37_1115 "JPEG_SyncHalEncode failed = %d"
#define JPEG_MIDWARE_SC6600L_2922_112_2_18_1_41_37_1116 "[JPEG_Encode_SC6600L] output jpeg buf = 0x%x, size = %d"
#define JPEG_MIDWARE_SC6600L_2997_112_2_18_1_41_37_1117 "JPEG_SyncHalDecode failed = %d"
#define JPEG_MIDWARE_SC6600L_3095_112_2_18_1_41_38_1118 "[JPEG_Decode_Res_SC6600L] alloc target RGB buffer failed"
#define JPEG_MIDWARE_SC6600L_3164_112_2_18_1_41_38_1119 "[JPEG_Decode_Res] slice height = %d"
#define JPEG_MIDWARE_SC6600L_3183_112_2_18_1_41_38_1120 "[JPEG_Decode_Res_SC6600L] JPEG_HWDecStartSynchro, %d"
#define JPEG_MIDWARE_SC6600L_3214_112_2_18_1_41_38_1121 "JPEG_HWEncInit failed = %d"
#define JPEG_MIDWARE_SC6600L_3221_112_2_18_1_41_38_1122 "JPEG_HWWriteHead failed = %d"
#define JPEG_MIDWARE_SC6600L_3273_112_2_18_1_41_38_1123 "[JPEG_StopEncode] jpeg stream buffer is not enough, stream size = %d, target buffer size = %d"
#define JPEG_MIDWARE_SC6600L_3485_112_2_18_1_41_38_1124 "[JPEG_EncodeReadPingPongBufferFirst] jpeg stream buffer is not enough"
#define JPEG_MIDWARE_SC6600L_3516_112_2_18_1_41_38_1125 "[JPEG_EncodeReadPingPongBuffer] jpeg stream buffer is not enough"
#define JPEG_MIDWARE_SC6600L_3587_112_2_18_1_41_39_1126 "JPEG_HWDecInit failed = %d"
#define JPEG_MIDWARE_SC6600L_3594_112_2_18_1_41_39_1127 "JPEG_HWParseHead failed = %d"
#define JPEG_MIDWARE_SC6600L_3757_112_2_18_1_41_39_1128 "[JPEG_DecodeWritePingPongBufferFirst] read ping first"
#define JPEG_MIDWARE_SC6600L_3763_112_2_18_1_41_39_1129 "[JPEG_DecodeWritePingPongBufferFirst] read pong first"
#define JPEG_MIDWARE_SC6600L_3795_112_2_18_1_41_39_1130 "[JPEG_DecodeWritePingPongBufferFirst] no stream in stream buffer, read size = %d, decoded size = %d"
#define JPEG_MIDWARE_SC6600L_3852_112_2_18_1_41_39_1131 "[JPEG_DecodeWritePingPongBuffer] no stream in stream buffer, read size = %d, decoded size = %d, buf_id = %d"
#define JPEG_MIDWARE_SC6600L_3988_112_2_18_1_41_39_1132 "[IMGJPEG_Decode] img size error, w = %d, h = %d"
#define JPEG_MIDWARE_SC6600L_3996_112_2_18_1_41_39_1133 "[IMGJPEG_Decode] target img size error, w = %d, h = %d"
#define JPEG_MIDWARE_SC6600L_4108_112_2_18_1_41_40_1134 "[JPEG_CreateMiniature] dec buff = 0x%x, size = %d, target buf = 0x%x, size = %d"
#define JPEG_MIDWARE_SC6600L_4113_112_2_18_1_41_40_1135 "[JPEG_CreateMiniature] decode buffer not enough, %d"
#define JPEG_MIDWARE_SC6600L_4135_112_2_18_1_41_40_1136 "[JPEG_CreateMiniature] decode to yuv failed !"
#define JPEG_MIDWARE_SC6600L_4152_112_2_18_1_41_40_1137 "[JPEG_CreateMiniature] encode to miniature failed !"
#define JPEG_MIDWARE_SC6600L_4286_112_2_18_1_41_40_1138 "[JPEG_HalGetDecodeBufSize] src size = (%d, %d), crop rect = (%d, %d, %d, %d), mcu_height = %d"
#define JPEG_MIDWARE_SC6600L_4290_112_2_18_1_41_40_1139 "[JPEG_HalGetDecodeBufSize] slice height = %d, aligned size = (%d, %d), subsample_factor = %d, hal_subsample_factor = %d, soft_subsample_factor = %d"
#define JPEG_MIDWARE_SC6600L_4332_112_2_18_1_41_40_1140 "[JPEG_HalGetDecodeBufSize] temp buf size = %d, ret = %d"
#define JPEG_SCALING_INIT_20_112_2_18_1_41_43_1141 "Can not allocate enough memory!\n"
#define JPEG_SCALING_INIT_37_112_2_18_1_41_43_1142 "[SCALE_MALLOC]Can not allocate enough memory!\n"
#define JPEG_SCALING_INIT_132_112_2_18_1_41_43_1143 "unknown data type!\n"
#define JPEG_SCALING_INIT_170_112_2_18_1_41_43_1144 "unknown data type!\n"
#define JPEG_SCALING_INIT_237_112_2_18_1_41_43_1145 "does not support!\n"
#define JPEG_SCALING_INIT_357_112_2_18_1_41_44_1146 "Warning: The times of scaling is more than 7, I am sorry WinSinc method can not support now!\n"
#define JPEG_SCALING_INIT_358_112_2_18_1_41_44_1147 "You can choose the Box average method to scaling it! Thanks!\n"
#define JPEG_SCALING_INIT_390_112_2_18_1_41_44_1148 "[Scale_Initialize]in w = %d, in h = %d, out w = %d, out h = %d"
#define JPEG_SCALING_INIT_423_112_2_18_1_41_44_1149 "the ratio of width and height is not proper"
#define JPEG_SCALING_INIT_459_112_2_18_1_41_44_1150 "Scale factor is %d/%d\n\n"
#define JPEG_SCALING_INIT_498_112_2_18_1_41_44_1151 "[Scale_Initialize], N = %d or M = %d > BASE_COFF = %d "
#define JPEG_SCALING_INIT_504_112_2_18_1_41_44_1152 "[Scale_Initialize], Init_Data_Type_adv failed "
#define JPEG_SCALING_INIT_509_112_2_18_1_41_44_1153 "[Scale_Initialize], Init_COEEF_Table_adv failed "
#define JPEG_SCALING_INIT_514_112_2_18_1_41_44_1154 "[Scale_Initialize], Init_Scale_Fun_adv failed "
#define JPEG_SCALING_INIT_529_112_2_18_1_41_44_1155 "[Scale_Initialize], allocate slice_ctrl.copy_line_ptr failed "
#define JPEG_SCALING_INIT_539_112_2_18_1_41_44_1156 "[Scale_Initialize], allocate rgb_buf failed "
#define JPEG_SCALING_INIT_551_112_2_18_1_41_44_1157 "[Scale_Initialize], allocate uv buffer failed "
#define JPEG_SCALING_INIT_597_112_2_18_1_41_44_1158 "[Scale_GetScalingDownSize], src size (%d, %d), target size (%d, %d), mode = %d"
#define JPEG_SCALING_INIT_653_112_2_18_1_41_44_1159 "[Scale_GetScalingDownSize], output size (%d, %d), N / M = %d / %d"
#define JPEG_STREAM_40_112_2_18_1_41_46_1160 "[Jpeg_ReadStream] warning ! stream_left_size = %d !"
#define JPEG_STREAM_60_112_2_18_1_41_46_1161 "[Jpeg_ReadStream] stream in file, no more bytes"
#define JPEG_STREAM_77_112_2_18_1_41_46_1162 "[Jpeg_ReadStream] stream in buffer, no more bytes"
#define JPEG_STREAM_81_112_2_18_1_41_46_1163 "[Jpeg_ReadStream] stream in buffer, stream_ptr = 0x%x, stream_left_size = %d"
#define JPEG_STREAM_87_112_2_18_1_41_46_1164 "[Jpeg_ReadStream] no source found !"
#define JPEG_STREAM_132_112_2_18_1_41_46_1165 "[Jpeg_GetStreamPos] error of stream ptr"
#define JPEG_STREAM_163_112_2_18_1_41_46_1166 "[Jpeg_SetStreamPos] error of stream ptr"
#define JPEG_STREAM_235_112_2_18_1_41_47_1167 "[Jpeg_GetC] no more bytes !"
#define JPEG_STREAM_262_112_2_18_1_41_47_1168 "[Jpegd_GetW] no more bytes !"
#define JPEG_STREAM_290_112_2_18_1_41_47_1169 "[Jpegd_GetW] no more bytes !"
#define JPEG_STREAM_321_112_2_18_1_41_47_1170 "[Jpegd_GetUINT32] no more bytes !"
#define JPEG_STREAM_354_112_2_18_1_41_47_1171 "[Jpegd_GetUINT32] no more bytes !"
#define JPEG_TASK_129_112_2_18_1_41_48_1172 "jpeg_task, JPEG_Task_Open, task_id = %d"
#define JPEG_TASK_133_112_2_18_1_41_48_1173 "jpeg_task, JPEG TASK is existed,task_id = %d"
#define JPEG_TASK_175_112_2_18_1_41_48_1174 "jpeg_task, JPEG_SIG_DEC_START"
#define JPEG_TASK_178_112_2_18_1_41_48_1175 "jpeg_task, JPEG_StartDecode error"
#define JPEG_TASK_186_112_2_18_1_41_48_1176 "jpeg_task, JPEG_SIG_DEC_END"
#define JPEG_TASK_209_112_2_18_1_41_48_1177 "jpeg_task, JPEG_SIG_ENC_START"
#define JPEG_TASK_212_112_2_18_1_41_48_1178 "jpeg_task, JPEG_StartEncode error"
#define JPEG_TASK_220_112_2_18_1_41_48_1179 "jpeg_task, JPEG_SIG_ENC_END"
#define JPEG_TASK_252_112_2_18_1_41_48_1180 "jpeg_task, JPEG_SIG_WRITE_PING_PONG_BUF_FIRST error"
#define JPEG_TASK_273_112_2_18_1_41_48_1181 "jpeg_task, JPEG_SIG_READ_PING_PONG_BUF_FIRST error"
#define JPEG_TASK_294_112_2_18_1_41_48_1182 "jpeg_task, JPEG_SIG_WRITE_PING_PONG_BUF error"
#define JPEG_TASK_315_112_2_18_1_41_48_1183 "jpeg_task, JPEG_SIG_READ_PING_PONG_BUF error"
#define JPEG_TASK_324_112_2_18_1_41_48_1184 "jpeg_task, JPEG_SIG_ERROR_RESTART"
#define JPEG_TASK_332_112_2_18_1_41_48_1185 "jpeg_task, JPEG_SIG_ERROR_RESTART error"
#define JPEG_TASK_341_112_2_18_1_41_48_1186 "jpeg_task, JPEG_SIG_OVERTIME"
#define JPEG_TASK_352_112_2_18_1_41_49_1187 "jpeg_task, JPEG_SIG_ERROR"
#define JPEG_TASK_364_112_2_18_1_41_49_1188 "jpeg_task, sig JPEG_SIG_ACTIVE,line %d"
#define JPEG_TASK_395_112_2_18_1_41_49_1189 "jpeg_task, forced to stop,line %d, freed signal = %d"
#define JPEG_TASK_402_112_2_18_1_41_49_1190 "jpeg_task, invalid sig code to JPEG_Task_Entry,line %d"
#define JPEG_TASK_520_112_2_18_1_41_49_1191 "jpeg_task, decoding event error, event flag = 0x%x"
#define JPEG_TASK_564_112_2_18_1_41_49_1192 "jpeg_task, decoding event error, event flag = 0x%x"
#define JPEG_TASK_606_112_2_18_1_41_49_1193 "jpeg_task, decoding event error, event flag = 0x%x"
#define JPEG_TASK_609_112_2_18_1_41_49_1194 "jpeg_task, after SCI_GetEvent(),event_flag=%lu,line %d"
#define JPEG_TASK_631_112_2_18_1_41_49_1195 "JPEG_WaitTaskStopped, g_jpeg_event_ptr is null"
#define JPEG_TASK_649_112_2_18_1_41_49_1196 "JPEG_WaitTaskStopped, wait task stopped time out"
#define JPEG_TASK_652_112_2_18_1_41_49_1197 "JPEG_WaitTaskStopped, after SCI_GetEvent(),event_flag=%lu,line %d"
#define JPEG_TASK_749_112_2_18_1_41_49_1198 "[JPEG_TimeOutHandler] jpeg time out but task no-active"
#define JPEG_TASK_762_112_2_18_1_41_49_1199 "jpeg_task,JPEG_Task_Close,task_id = %lu"
#define JPEG_TASK_794_112_2_18_1_41_49_1200 "jpeg_task,JPEG_Task_Close failed, g_jpeg_task_id = 0"
#define DCAMERA_JPG_CODEC_70_112_2_18_1_41_52_1201 "Jpeg_IRAM_Disable:failed to unlock IRAM"
#define DCAMERA_JPG_CODEC_196_112_2_18_1_41_52_1202 "--%s--,line %d"
#define DCAMERA_JPG_CODEC_297_112_2_18_1_41_52_1203 "--%s--,line %d"
#define JPEG_119_112_2_18_1_41_53_1204 "dcam: IMG_JPEG_DecodeJpeg - data type %d"
#define PE_PROCESS_3362_112_2_18_1_42_12_1205 "[pe_data_sub_sample], width = %d, subsample = %d, num_of_line = %d"
#define PE_PROCESS_3427_112_2_18_1_42_12_1206 "[pe_get_data], width = %d, height = %d, line = %d, src buf = 0x%x"
#define PE_PROCESS_3502_112_2_18_1_42_12_1207 "[PE_Scalingdown] src size =(%d, %d), target size = (%d, %d)"
#define PE_PROCESS_3570_112_2_18_1_42_12_1208 "[PE_Scalingdown] is_sub_sample = %d, sub_sample = %d"
#define PE_PROCESS_3593_112_2_18_1_42_13_1209 "[PE_Scalingdown] scaling input width = %d, height = %d"
#define PE_PROCESS_3731_112_2_18_1_42_13_1210 "[PE_SaveJpeg], size = (%d, %d), process buf = 0x%x, size = %d"
#define PE_PROCESS_3732_112_2_18_1_42_13_1211 "[PE_SaveJpeg], convert buf = 0x%x, convert_buf_size = %d"
#define PE_PROCESS_3760_112_2_18_1_42_13_1212 "[pe_process.c] SaveJpeg successed! "
#define PE_PROCESS_3770_112_2_18_1_42_13_1213 "[pe_process.c] process buf is not enough, %d! "
#define PE_PROCESS_3839_112_2_18_1_42_13_1214 "---PEPROCESS_ImageProcess:input image width or height is 0---"
#define PE_PROCESS_4152_112_2_18_1_42_14_1215 "PE Process type is default!"
#define PE_PROCESS_4156_112_2_18_1_42_14_1216 "------PE PROCESS TIME:%d------"
#define INFLATE_790_112_2_18_1_42_18_1217 "[PNG]img_inflate:run default of switch!"
#define PNG_65_112_2_18_1_42_20_1218 "---PNG ERR INFO---Too many bytes for PNG signature."
#define PNG_117_112_2_18_1_42_20_1219 "---PNG WARNING--Potential overflow in png_zalloc()"
#define PNG_297_112_2_18_1_42_20_1220 "---PNG WARNING--Unknown freer parameter in png_data_freer_0."
#define PNG_APP_261_112_2_18_1_42_22_1221 "png 0 target %d, %d, src %d, %d, sample %d, %d"
#define PNG_APP_399_112_2_18_1_42_22_1222 "PNGAPP_Decode: scrwidth: %d scrheight: %d target width :%d target height: %d"
#define PNG_APP_455_112_2_18_1_42_22_1223 "image_orgin_width:%d,%d,%d,%d"
#define PNG_APP_CTRL_284_112_2_18_1_42_23_1224 "PNGDEC: Receive Sig=0x%x, param addr=0x%x , cur time=%d"
#define PNG_APP_CTRL_330_112_2_18_1_42_23_1225 "_PNGDEC_HandleRoutine:Png decode is finish time=%d"
#define PNG_APP_CTRL_370_112_2_18_1_42_23_1226 "_PNGDEC_ReadFile:read file error!"
#define PNG_APP_CTRL_443_112_2_18_1_42_24_1227 "PNGDEC_FormatConvert:data format is invailid,%d"
#define PNG_APP_CTRL_678_112_2_18_1_42_24_1228 "[IMGDECpng_Dec] bytes_written = %d"
#define PNG_APP_CTRL_682_112_2_18_1_42_24_1229 "[IMGDECpng_Dec] create file failed"
#define PNG_APP_CTRL_685_112_2_18_1_42_24_1230 "IMGDECpng_Dec, target_ptr= %x, jpeg_size= %d"
#define PNG_APP_CTRL_686_112_2_18_1_42_24_1231 "IMGDECpng_Dec, target_width = %d, target_height = %d"
#define PNG_APP_CTRL_687_112_2_18_1_42_24_1232 "IMGDECpng_Dec,out_param.real_width = %d, out_param.real_height = %d"
#define PNG_APP_CTRL_891_112_2_18_1_42_24_1233 "IMGDECPNG_GetFrame:handle=0x%x,callback=0x%x,time=%d"
#define PNG_APP_CTRL_897_112_2_18_1_42_24_1234 "IMGDECPNG_GetFrame:data format is error! %d"
#define PNG_APP_CTRL_991_112_2_18_1_42_25_1235 "IMGDECPNG_GetFrame,handle=0x%x,out_time=%d"
#define PNG_APP_CTRL_1014_112_2_18_1_42_25_1236 "IMGDECPNG_Destroy:handle=0x%x,time = %d"
#define PNG_DECODE_184_112_2_18_1_42_26_1237 "Failed to create PNG structure"
#define PNG_DECODE_193_112_2_18_1_42_26_1238 "Failed to initialize PNG info structure"
#define PNG_DECODE_258_112_2_18_1_42_26_1239 "[PNG DEC:alpha buffer is small!]"
#define PNG_DECODE_285_112_2_18_1_42_26_1240 "img_png_decode_destroy: enter into destroy \n"
#define PNG_DECODE_335_112_2_18_1_42_26_1241 "img_png_decode_destroy: s_png_ptr is PNULL \n"
#define PNG_DECODE_341_112_2_18_1_42_26_1242 "img_png_decode_destroy: enter out destroy \n"
#define PNG_DECODE_532_112_2_18_1_42_26_1243 "[_noscale_row_frame_palette: ERROR],color_table_ptr is NULL!"
#define PNG_DECODE_791_112_2_18_1_42_27_1244 "[_noscale_row_frame_palette: ERROR],color_table_ptr is NULL!"
#define PNG_DECODE_1330_112_2_18_1_42_28_1245 "Do not support format"
#define PNG_DECODE_1434_112_2_18_1_42_28_1246 "Do not support format"
#define PNG_DECODE_2210_112_2_18_1_42_30_1247 "Do not support format"
#define PNG_DECODE_2318_112_2_18_1_42_30_1248 "Do not support format"
#define PNG_DECODE_2593_112_2_18_1_42_30_1249 "[_noscale_rowgroup_palette ERROR]:color_table_ptr is NULL!"
#define PNG_DECODE_2612_112_2_18_1_42_30_1250 "s_png_ptr->row_info.pixel_depth %d"
#define PNG_DECODE_2879_112_2_18_1_42_31_1251 "[_noscale_rowgroup_palette ERROR]:color_table_ptr is NULL!"
#define PNG_DECODE_2899_112_2_18_1_42_31_1252 "s_png_ptr->row_info.pixel_depth %d"
#define PNG_DECODE_3584_112_2_18_1_42_32_1253 "Do not support format"
#define PNG_DECODE_3687_112_2_18_1_42_33_1254 "Do not support format"
#define PNG_DECODE_3929_112_2_18_1_42_33_1255 "out into _decode_row_group "
#define PNG_DECODE_4121_112_2_18_1_42_33_1256 "[_row_group_palette_argb ERROR]:color_table_ptr is NULL!"
#define PNG_DECODE_4626_112_2_18_1_42_34_1257 "Do not support format"
#define PNG_DECODE_4745_112_2_18_1_42_35_1258 "Do not support format"
#define PNG_DECODE_4957_112_2_18_1_42_35_1259 "out into _decode_row_group "
#define PNG_DECODE_5247_112_2_18_1_42_36_1260 "test Get Data "
#define PNG_DECODE_5251_112_2_18_1_42_36_1261 "-----PNG Get Data Time :%d------"
#define PNGGET_515_112_2_18_1_42_37_1262 "---PNG ERR INFO---Invalid bit depth"
#define PNGGET_521_112_2_18_1_42_37_1263 "---PNG ERR INFO---Invalid color type"
#define PNGGET_534_112_2_18_1_42_37_1264 "---PNG ERR INFO---Invalid image width"
#define PNGGET_539_112_2_18_1_42_37_1265 "---PNG ERR INFO---Invalid image height"
#define PNGGET_549_112_2_18_1_42_38_1266 "---PNG WARNING---Width too large for libpng to process image data."
#define PNGMEM_167_112_2_18_1_42_39_1267 "---PNG ERR INFO---Overflow in png_memcpy_check."
#define PNGMEM_182_112_2_18_1_42_39_1268 "---PNG ERR INFO---Overflow in png_memset_check."
#define PNGPREAD_156_112_2_18_1_42_39_1269 "---PNG ERR INFO---Not a PNG file"
#define PNGPREAD_161_112_2_18_1_42_39_1270 "---PNG ERR INFO---PNG file corrupted by ASCII conversion"
#define PNGPREAD_292_112_2_18_1_42_39_1271 "---PNG ERR INFO---Missing IHDR before IDAT"
#define PNGPREAD_298_112_2_18_1_42_39_1272 "---PNG ERR INFO---Missing PLTE before IDAT"
#define PNGPREAD_321_112_2_18_1_42_40_1273 "---PNG ERR INFO---Missing IHDR before IDAT"
#define PNGPREAD_327_112_2_18_1_42_40_1274 "---PNG ERR INFO---Missing PLTE before IDAT"
#define PNGPREAD_338_112_2_18_1_42_40_1275 "---PNG ERR INFO---Too many IDAT's found"
#define PNGPREAD_660_112_2_18_1_42_40_1276 "---PNG ERR INFO---Potential overflow of save_buffer"
#define PNGPREAD_722_112_2_18_1_42_40_1277 "---PNG ERR INFO---Not enough compressed data"
#define PNGPREAD_738_112_2_18_1_42_40_1278 "---PNG ERR INFO---save_size overflowed in pngpread"
#define PNGPREAD_761_112_2_18_1_42_40_1279 "---PNG ERR INFO---save_size overflowed in pngpread"
#define PNGPREAD_798_112_2_18_1_42_40_1280 "---PNG ERR INFO---Extra compression data"
#define PNGPREAD_813_112_2_18_1_42_40_1281 "---PNG ERR INFO---Extra compressed data"
#define PNGPREAD_829_112_2_18_1_42_41_1282 "---PNG ERR INFO---Decompression Error"
#define PNGPREAD_844_112_2_18_1_42_41_1283 "---PNG WARNING---Too much data in IDAT chunks"
#define PNGPREAD_1019_112_2_18_1_42_41_1284 "[img_png_push_process_row:run default of swith]"
#define PNGPREAD_1115_112_2_18_1_42_41_1285 "---PNG ERR INFO---Out of place tEXt"
#define PNGPREAD_1126_112_2_18_1_42_41_1286 "---PNG WARNING---tEXt chunk too large to fit in memory"
#define PNGPREAD_1199_112_2_18_1_42_41_1287 "---PNG WARNING---Insufficient memory to store text chunk."
#define PNGPREAD_1210_112_2_18_1_42_41_1288 "---PNG ERR INFO---Out of place zTXt"
#define PNGPREAD_1223_112_2_18_1_42_41_1289 "---PNG WARNING---zTXt chunk too large to fit in memory"
#define PNGPREAD_1389_112_2_18_1_42_42_1290 "---PNG WARNING---Insufficient memory to store text chunk."
#define PNGPREAD_1401_112_2_18_1_42_42_1291 "---PNG ERR INFO---Out of place iTXt"
#define PNGPREAD_1412_112_2_18_1_42_42_1292 "---PNG WARNING---iTXt chunk too large to fit in memory"
#define PNGPREAD_1500_112_2_18_1_42_42_1293 "---PNG WARNING---Insufficient memory to store iTXt chunk."
#define PNGPREAD_1525_112_2_18_1_42_42_1294 "---PNG ERR INFO---unknown critical chunk"
#define PNGPREAD_1541_112_2_18_1_42_42_1295 "---PNG WARNING---unknown chunk too large to fit in memory"
#define PNGPREAD_1561_112_2_18_1_42_42_1296 "---PNG ERR INFO---unknown critical chunk"
#define PNGREAD_144_112_2_18_1_42_43_1297 "--PNG ERR Info--Incompatible libpng version in application and library"
#define PNGREAD_161_112_2_18_1_42_43_1298 "---PNG ERR INFO---zlib memory error"
#define PNGREAD_165_112_2_18_1_42_43_1299 "---PNG ERR INFO---zlib version error"
#define PNGREAD_170_112_2_18_1_42_43_1300 "---PNG ERR INFO---Unknown zlib error"
#define PNGREAD_227_112_2_18_1_42_43_1301 "---PNG ERR INFO---The png struct allocated by the application for reading is too small."
#define PNGREAD_236_112_2_18_1_42_43_1302 "---PNG ERR INFO---The info struct allocated by application for reading is too small."
#define PNGREAD_261_112_2_18_1_42_43_1303 "---PNG WARNING---Application uses deprecated png_read_init_0() and should be recompiled."
#define PNGREAD_308_112_2_18_1_42_43_1304 "---PNG ERR INFO---zlib memory"
#define PNGREAD_312_112_2_18_1_42_43_1305 "---PNG ERR INFO---zlib version"
#define PNGREAD_317_112_2_18_1_42_43_1306 "---PNG ERR INFO---Unknown zlib error"
#define PNGREAD_354_112_2_18_1_42_43_1307 "---PNG ERR INFO---Not a PNG file"
#define PNGREAD_359_112_2_18_1_42_43_1308 "---PNG ERR INFO---PNG file corrupted by ASCII conversion"
#define PNGREAD_490_112_2_18_1_42_43_1309 "---PNG ERR INFO---Missing IHDR before IDAT"
#define PNGREAD_497_112_2_18_1_42_43_1310 "---PNG ERR INFO---Missing PLTE before IDAT"
#define PNGREAD_511_112_2_18_1_42_43_1311 "---PNG ERR INFO---Missing IHDR before IDAT"
#define PNGREAD_518_112_2_18_1_42_43_1312 "---PNG ERR INFO---Missing PLTE before IDAT"
#define PNGREAD_607_112_2_18_1_42_43_1313 "---PNG WARNING---Ignoring extra png_read_update_info() call; row buffer not reallocated"
#define PNGREAD_638_112_2_18_1_42_44_1314 "---PNG WARNING---PNG_READ_INVERT_SUPPORTED is not defined."
#define PNGREAD_642_112_2_18_1_42_44_1315 "---PNG WARNING---PNG_READ_FILLER_SUPPORTED is not defined."
#define PNGREAD_646_112_2_18_1_42_44_1316 "---PNG WARNING---PNG_READ_PACKSWAP_SUPPORTED is not defined."
#define PNGREAD_650_112_2_18_1_42_44_1317 "---PNG WARNING---PNG_READ_PACK_SUPPORTED is not defined."
#define PNGREAD_654_112_2_18_1_42_44_1318 "---PNG WARNING---PNG_READ_SHIFT_SUPPORTED is not defined."
#define PNGREAD_658_112_2_18_1_42_44_1319 "---PNG WARNING---PNG_READ_BGR_SUPPORTED is not defined."
#define PNGREAD_662_112_2_18_1_42_44_1320 "---PNG WARNING---PNG_READ_SWAP_SUPPORTED is not defined."
#define PNGREAD_740_112_2_18_1_42_44_1321 "[img_png_read_row:run default of switch!]"
#define PNGREAD_748_112_2_18_1_42_44_1322 "---PNG ERR INFO---Invalid attempt to read row data"
#define PNGREAD_769_112_2_18_1_42_44_1323 "---PNG ERR INFO---Not enough image data"
#define PNGREAD_787_112_2_18_1_42_44_1324 "---PNG ERR INFO---Extra compressed data"
#define PNGREAD_797_112_2_18_1_42_44_1325 "--PNG ERR Info--Decompression error"
#define PNGREAD_953_112_2_18_1_42_44_1326 "---PNG ERR INFO---Cannot read interlaced image -- interlace handler disabled."
#define PNGREAD_1089_112_2_18_1_42_44_1327 "---PNG ERR INFO---Too many IDAT's found"
#define PNGREAD_1107_112_2_18_1_42_44_1328 "---PNG ERR INFO---Too many IDAT's found"
#define PNGREAD_1426_112_2_18_1_42_45_1329 "---PNG ERR INFO---Image is too high to process with png_read_png()"
#define PNGRIO_65_112_2_18_1_42_46_1330 "[img_png_read_data:ERR0]"
#define PNGRIO_72_112_2_18_1_42_46_1331 "[img_png_read_data:END0]"
#define PNGRIO_83_112_2_18_1_42_46_1332 "[img_png_read_data:ERR1]"
#define PNGRIO_89_112_2_18_1_42_46_1333 "[img_png_read_data:END1]"
#define PNGRIO_118_112_2_18_1_42_46_1334 "[img_png_read_data:ERR2]"
#define PNGRIO_124_112_2_18_1_42_46_1335 "[img_png_read_data:END2]"
#define PNGRIO_134_112_2_18_1_42_46_1336 "[img_png_read_data:ERR3]"
#define PNGRIO_140_112_2_18_1_42_46_1337 "[img_png_read_data:END3]"
#define PNGRIO_211_112_2_18_1_42_46_1338 "---PNG WARNING---It's an error to set both read_data_fn and write_data_fn in the "
#define PNGRIO_212_112_2_18_1_42_46_1339 "---PNG WARNING---same structure.  Resetting write_data_fn to NULL."
#define PNGRTRAN_38_112_2_18_1_42_46_1340 "---PNG WARNING---Can't discard critical data on CRC error."
#define PNGRTRAN_80_112_2_18_1_42_46_1341 "---PNG WARNING---Application must supply a known background gamma"
#define PNGRTRAN_611_112_2_18_1_42_48_1342 "[img_png_set_rgb_to_gray_fixed_0:run default of switch!]"
#define PNGRTRAN_619_112_2_18_1_42_48_1343 "---PNG WARNING---Cannot do RGB_TO_GRAY without EXPAND_SUPPORTED."
#define PNGRTRAN_637_112_2_18_1_42_48_1344 "---PNG WARNING---ignoring out of range rgb_to_gray coefficients"
#define PNGRTRAN_661_112_2_18_1_42_48_1345 "---PNG WARNING---This version of libpng does not support user transforms"
#define PNGRTRAN_1158_112_2_18_1_42_49_1346 "---PNG ERR INFO---NULL row buffer"
#define PNGRTRAN_1199_112_2_18_1_42_49_1347 "---PNG WARNING---png_do_rgb_to_gray found nongray pixel"
#define PNGRTRAN_1202_112_2_18_1_42_49_1348 "---PNG ERR INFO---png_do_rgb_to_gray found nongray pixel"
#define PNGRTRAN_1287_112_2_18_1_42_49_1349 "---PNG ERR INFO---png_do_dither returned rowbytes=0"
#define PNGRTRAN_1458_112_2_18_1_42_50_1350 "[img_png_do_unpack:run default of switch!]"
#define PNGRTRAN_1572_112_2_18_1_42_50_1351 "[img_png_do_unshift:run default of switch!]"
#define PNGRTRAN_3427_112_2_18_1_42_54_1352 "[img_png_do_gamma:run default of switch!]"
#define PNGRTRAN_3522_112_2_18_1_42_54_1353 "[img_png_do_expand_palette:run default of switch!]"
#define PNGRTRAN_3576_112_2_18_1_42_54_1354 "[img_png_do_expand_palette:run default of switch!]"
#define PNGRTRAN_3676_112_2_18_1_42_54_1355 "[img_png_do_expand:run default of switch!]"
#define PNGRUTIL_77_112_2_18_1_42_55_1356 "---PNG ERR INFO---PNG unsigned integer out of range.\n"
#define PNGRUTIL_146_112_2_18_1_42_56_1357 "---PNG ERR INFO---CRC error"
#define PNGRUTIL_150_112_2_18_1_42_56_1358 "---PNG ERR INFO---CRC error"
#define PNGRUTIL_233_112_2_18_1_42_56_1359 "---PNG ERR INFO---Not enough memory to decompress chunk"
#define PNGRUTIL_260_112_2_18_1_42_56_1360 "---PNG ERR INFO---Not enough memory to decompress chunk."
#define PNGRUTIL_278_112_2_18_1_42_56_1361 "---PNG ERR INFO---Not enough memory to decompress chunk.."
#define PNGRUTIL_309_112_2_18_1_42_56_1362 "---PNG WARNING---Incomplete compressed datastream in chunk other than IDAT"
#define PNGRUTIL_318_112_2_18_1_42_56_1363 "---PNG ERR INFO---Not enough memory for text."
#define PNGRUTIL_337_112_2_18_1_42_56_1364 "--PNG WARNING--Unknown zTXt compression type"
#define PNGRUTIL_359_112_2_18_1_42_56_1365 "---PNG ERR INFO---Out of place IHDR"
#define PNGRUTIL_366_112_2_18_1_42_56_1366 "---PNG ERR INFO---Invalid IHDR chunk"
#define PNGRUTIL_411_112_2_18_1_42_56_1367 "[img_png_handle_IHDR:run default of switch!]"
#define PNGRUTIL_439_112_2_18_1_42_56_1368 "---PNG ERR INFO---Missing IHDR before PLTE"
#define PNGRUTIL_444_112_2_18_1_42_56_1369 "---PNG WARNING---Invalid PLTE after IDAT"
#define PNGRUTIL_450_112_2_18_1_42_56_1370 "---PNG ERR INFO---Duplicate PLTE chunk"
#define PNGRUTIL_458_112_2_18_1_42_56_1371 "---PNG WARNING---Ignoring PLTE chunk in grayscale PNG"
#define PNGRUTIL_474_112_2_18_1_42_56_1372 "---PNG WARNING---Invalid palette chunk"
#define PNGRUTIL_480_112_2_18_1_42_56_1373 "---PNG ERR INFO---Invalid palette chunk"
#define PNGRUTIL_529_112_2_18_1_42_56_1374 "---PNG ERR INFO---CRC error"
#define PNGRUTIL_533_112_2_18_1_42_56_1375 "---PNG WARNING---CRC error"
#define PNGRUTIL_540_112_2_18_1_42_56_1376 "---PNG WARNING---CRC error"
#define PNGRUTIL_554_112_2_18_1_42_56_1377 "---PNG WARNING---Truncating incorrect tRNS chunk length"
#define PNGRUTIL_559_112_2_18_1_42_56_1378 "---PNG WARNING---Truncating incorrect info tRNS chunk length"
#define PNGRUTIL_574_112_2_18_1_42_56_1379 "---PNG ERR INFO---No image in file"
#define PNGRUTIL_582_112_2_18_1_42_56_1380 "---PNG WARNING---Incorrect IEND chunk length"
#define PNGRUTIL_603_112_2_18_1_42_57_1381 "---PNG ERR INFO---Missing IHDR before gAMA"
#define PNGRUTIL_608_112_2_18_1_42_57_1382 "---PNG WARNING---Invalid gAMA after IDAT"
#define PNGRUTIL_615_112_2_18_1_42_57_1383 "---PNG WARNING--Out of place gAMA chunk"
#define PNGRUTIL_624_112_2_18_1_42_57_1384 "---PNG WARNING---Duplicate gAMA chunk"
#define PNGRUTIL_631_112_2_18_1_42_57_1385 "---PNG WRANING---Incorrect gAMA chunk length"
#define PNGRUTIL_644_112_2_18_1_42_57_1386 "---PNG WARNING---Ignoring gAMA chunk with gamma=0"
#define PNGRUTIL_652_112_2_18_1_42_57_1387 "---PNG WARNING---Ignoring incorrect gAMA value when sRGB is also present"
#define PNGRUTIL_685_112_2_18_1_42_57_1388 "---PNG ERR INFO---Missing IHDR before sBIT"
#define PNGRUTIL_690_112_2_18_1_42_57_1389 "---PNG WARNING---Invalid sBIT after IDAT"
#define PNGRUTIL_697_112_2_18_1_42_57_1390 "---PNG WARNING---Out of place sBIT chunk"
#define PNGRUTIL_701_112_2_18_1_42_57_1391 "---PNG WARNING---Duplicate sBIT chunk"
#define PNGRUTIL_713_112_2_18_1_42_57_1392 "---PNG WARNING---Incorrect sBIT chunk length"
#define PNGRUTIL_757_112_2_18_1_42_57_1393 "---PNG ERR INFO---Missing IHDR before cHRM"
#define PNGRUTIL_762_112_2_18_1_42_57_1394 "---PNG WARNING---Invalid cHRM after IDAT"
#define PNGRUTIL_769_112_2_18_1_42_57_1395 "---PNG WARNING---Missing PLTE before cHRM"
#define PNGRUTIL_778_112_2_18_1_42_57_1396 "---PNG WARNING---Duplicate cHRM chunk"
#define PNGRUTIL_785_112_2_18_1_42_57_1397 "---PNG WARNING---Incorrect cHRM chunk length"
#define PNGRUTIL_799_112_2_18_1_42_57_1398 "---PNG WARNING---Invalid cHRM white point"
#define PNGRUTIL_815_112_2_18_1_42_57_1399 "---PNG WARNING---Invalid cHRM red point"
#define PNGRUTIL_831_112_2_18_1_42_57_1400 "---PNG WARNING---Invalid cHRM green point"
#define PNGRUTIL_847_112_2_18_1_42_57_1401 "---PNG WARNING---Invalid cHRM blue point"
#define PNGRUTIL_878_112_2_18_1_42_57_1402 "---PNG WARNING---Ignoring incorrect cHRM value when sRGB is also present"
#define PNGRUTIL_922_112_2_18_1_42_57_1403 "--PNG ERR Info--Missing IHDR before sRGB"
#define PNGRUTIL_926_112_2_18_1_42_57_1404 "---PNG WARNING---Invalid sRGB after IDAT"
#define PNGRUTIL_933_112_2_18_1_42_57_1405 "---PNG WARNING---Out of place sRGB chunk"
#define PNGRUTIL_938_112_2_18_1_42_57_1406 "---PNG WARNING---Duplicate sRGB chunk"
#define PNGRUTIL_945_112_2_18_1_42_57_1407 "---PNG WARNING---Incorrect sRGB chunk length"
#define PNGRUTIL_958_112_2_18_1_42_57_1408 "---PNG WARNING---Unknown sRGB intent"
#define PNGRUTIL_975_112_2_18_1_42_57_1409 "---PNG WARNING---Ignoring incorrect gAMA value when sRGB is also present"
#define PNGRUTIL_1001_112_2_18_1_42_57_1410 "---PNG WARNING---Ignoring incorrect cHRM value when sRGB is also present"
#define PNGRUTIL_1025_112_2_18_1_42_57_1411 "---PNG ERR INFO---Missing IHDR before iCCP"
#define PNGRUTIL_1030_112_2_18_1_42_57_1412 "---PNG WARNING---Invalid iCCP after IDAT"
#define PNGRUTIL_1037_112_2_18_1_42_57_1413 "---PNG WARNING---Out of place iCCP chunk"
#define PNGRUTIL_1041_112_2_18_1_42_58_1414 "---PNG WARNING---Duplicate iCCP chunk"
#define PNGRUTIL_1049_112_2_18_1_42_58_1415 "---PNG WARNING---iCCP chunk too large to fit in memory"
#define PNGRUTIL_1077_112_2_18_1_42_58_1416 "---PNG WARNING---Malformed iCCP chunk"
#define PNGRUTIL_1085_112_2_18_1_42_58_1417 "---PNG WARNING---Ignoring nonzero compression type in iCCP chunk"
#define PNGRUTIL_1099_112_2_18_1_42_58_1418 "---PNG WARNING---Profile size field missing from iCCP chunk"
#define PNGRUTIL_1116_112_2_18_1_42_58_1419 "---PNG WARNING---Ignoring truncated iCCP profile.\n"
#define PNGRUTIL_1144_112_2_18_1_42_58_1420 "--PNG ERR Info--Missing IHDR before sPLT"
#define PNGRUTIL_1148_112_2_18_1_42_58_1421 "---PNG WARNING---Invalid sPLT after IDAT"
#define PNGRUTIL_1156_112_2_18_1_42_58_1422 "---PNG WARNING---sPLT chunk too large to fit in memory"
#define PNGRUTIL_1182_112_2_18_1_42_58_1423 "---PNG WARNING---malformed sPLT chunk"
#define PNGRUTIL_1194_112_2_18_1_42_58_1424 "---PNG WARNING---sPLT chunk has bad length"
#define PNGRUTIL_1202_112_2_18_1_42_58_1425 "---PNG WARNING---sPLT chunk too long"
#define PNGRUTIL_1209_112_2_18_1_42_58_1426 "---PNG WARNING---sPLT chunk requires too much memory"
#define PNGRUTIL_1277_112_2_18_1_42_58_1427 "---PNG ERR INFO---Missing IHDR before tRNS"
#define PNGRUTIL_1282_112_2_18_1_42_58_1428 "---PNG WARNING---Invalid tRNS after IDAT"
#define PNGRUTIL_1288_112_2_18_1_42_58_1429 "---PNG WARNING---Duplicate tRNS chunk"
#define PNGRUTIL_1297_112_2_18_1_42_58_1430 "---PNG WARNING---Incorrect tRNS chunk length"
#define PNGRUTIL_1313_112_2_18_1_42_58_1431 "---PNG WARNING---Incorrect tRNS chunk length"
#define PNGRUTIL_1329_112_2_18_1_42_58_1432 "---PNG WARNING---Missing PLTE before tRNS"
#define PNGRUTIL_1334_112_2_18_1_42_58_1433 "---PNG WARNING---Incorrect tRNS chunk length"
#define PNGRUTIL_1340_112_2_18_1_42_58_1434 "---PNG WARNING---Zero length tRNS chunk"
#define PNGRUTIL_1349_112_2_18_1_42_58_1435 "---PNG WARNING---tRNS chunk not allowed with alpha channel"
#define PNGRUTIL_1372_112_2_18_1_42_58_1436 "---PNG ERR INFO---Missing IHDR before bKGD"
#define PNGRUTIL_1377_112_2_18_1_42_58_1437 "---PNG WARNING---Invalid bKGD after IDAT"
#define PNGRUTIL_1384_112_2_18_1_42_58_1438 "---PNG WARNING---Missing PLTE before bKGD"
#define PNGRUTIL_1390_112_2_18_1_42_58_1439 "---PNG WARNING---Duplicate bKGD chunk"
#define PNGRUTIL_1404_112_2_18_1_42_58_1440 "---PNG WARNING---Incorrect bKGD chunk length"
#define PNGRUTIL_1424_112_2_18_1_42_58_1441 "---PNG WARNING---Incorrect bKGD chunk index value"
#define PNGRUTIL_1463_112_2_18_1_42_58_1442 "--PNG ERR Info--Missing IHDR before hIST"
#define PNGRUTIL_1467_112_2_18_1_42_58_1443 "---PNG WARNING---Invalid hIST after IDAT"
#define PNGRUTIL_1473_112_2_18_1_42_58_1444 "---PNG WARNING---Missing PLTE before hIST"
#define PNGRUTIL_1479_112_2_18_1_42_58_1445 "---PNG WARNING---Duplicate hIST chunk"
#define PNGRUTIL_1488_112_2_18_1_42_58_1446 "---PNG WARNING---Incorrect hIST chunk length"
#define PNGRUTIL_1519_112_2_18_1_42_58_1447 "---PNG ERR INFO---Missing IHDR before pHYs"
#define PNGRUTIL_1524_112_2_18_1_42_59_1448 "---PNG WARNING---Invalid pHYs after IDAT"
#define PNGRUTIL_1530_112_2_18_1_42_59_1449 "---PNG_WARNING---Duplicate pHYs chunk"
#define PNGRUTIL_1537_112_2_18_1_42_59_1450 "---PNG WARNING---Incorrect pHYs chunk length"
#define PNGRUTIL_1564_112_2_18_1_42_59_1451 "--PNG ERR Info--Missing IHDR before oFFs"
#define PNGRUTIL_1568_112_2_18_1_42_59_1452 "---PNG WARNING---Invalid oFFs after IDAT"
#define PNGRUTIL_1574_112_2_18_1_42_59_1453 "---PNG WARNING---Duplicate oFFs chunk"
#define PNGRUTIL_1581_112_2_18_1_42_59_1454 "---PNG WARNING---Incorrect oFFs chunk length"
#define PNGRUTIL_1612_112_2_18_1_42_59_1455 "--PNG ERR Info--Missing IHDR before pCAL"
#define PNGRUTIL_1616_112_2_18_1_42_59_1456 "---PNG WARNING---Invalid pCAL after IDAT"
#define PNGRUTIL_1622_112_2_18_1_42_59_1457 "---PNG WARNING---Duplicate pCAL chunk"
#define PNGRUTIL_1632_112_2_18_1_42_59_1458 "---PNG WARNING---No memory for pCAL purpose."
#define PNGRUTIL_1656_112_2_18_1_42_59_1459 "---PNG WARNING---Invalid pCAL data"
#define PNGRUTIL_1676_112_2_18_1_42_59_1460 "---PNG WARNING---Invalid pCAL parameters for equation type"
#define PNGRUTIL_1682_112_2_18_1_42_59_1461 "---PNG WARNING---Unrecognized equation type for pCAL chunk"
#define PNGRUTIL_1694_112_2_18_1_42_59_1462 "---PNG WARNING---No memory for pCAL params."
#define PNGRUTIL_1710_112_2_18_1_42_59_1463 "---PNG WARNING---Invalid pCAL data"
#define PNGRUTIL_1743_112_2_18_1_42_59_1464 "--PNG ERR Info--Missing IHDR before sCAL"
#define PNGRUTIL_1747_112_2_18_1_42_59_1465 "---PNG WARNING---Invalid sCAL after IDAT"
#define PNGRUTIL_1753_112_2_18_1_42_59_1466 "---PNG WARNING---Duplicate sCAL chunk"
#define PNGRUTIL_1763_112_2_18_1_42_59_1467 "---PNG WARNING---Out of memory while processing sCAL chunk"
#define PNGRUTIL_1783_112_2_18_1_42_59_1468 "---PNG WARNING---malformed width string in sCAL chunk"
#define PNGRUTIL_1792_112_2_18_1_42_59_1469 "---PNG WARNING---Out of memory while processing sCAL chunk width"
#define PNGRUTIL_1807_112_2_18_1_42_59_1470 "---PNG WARNING---malformed height string in sCAL chunk"
#define PNGRUTIL_1815_112_2_18_1_42_59_1471 "---PNG WARNING---Out of memory while processing sCAL chunk height"
#define PNGRUTIL_1828_112_2_18_1_42_59_1472 "---PNG WARNING---Invalid sCAL data"
#define PNGRUTIL_1864_112_2_18_1_42_59_1473 "--PNG ERR Info--Out of place tIME chunk"
#define PNGRUTIL_1868_112_2_18_1_42_59_1474 "---PNG WARNING---Duplicate tIME chunk"
#define PNGRUTIL_1878_112_2_18_1_42_59_1475 "---PNG WARNING---Incorrect tIME chunk length"
#define PNGRUTIL_1912_112_2_18_1_42_59_1476 "--PNG ERR Info--Missing IHDR before tEXt"
#define PNGRUTIL_1921_112_2_18_1_42_59_1477 "---PNG WARNING---tEXt chunk too large to fit in memory"
#define PNGRUTIL_1930_112_2_18_1_42_59_1478 "---PNG WARNING---No memory to process text chunk."
#define PNGRUTIL_1954_112_2_18_1_42_59_1479 "---PNG WARNING---Not enough memory to process text chunk."
#define PNGRUTIL_1974_112_2_18_1_42_59_1480 "---PNG WARNING---Insufficient memory to process text chunk."
#define PNGRUTIL_1992_112_2_18_1_43_0_1481 "---PNG ERR INFO---Missing IHDR before zTXt"
#define PNGRUTIL_2003_112_2_18_1_43_0_1482 "---PNG WARNING---zTXt chunk too large to fit in memory"
#define PNGRUTIL_2012_112_2_18_1_43_0_1483 "---PNG WARNING---Out of memory processing zTXt chunk."
#define PNGRUTIL_2032_112_2_18_1_43_0_1484 "---PNG WARNING---Zero length zTXt chunk"
#define PNGRUTIL_2039_112_2_18_1_43_0_1485 "---PNG WARNING---Unknown compression type in zTXt chunk"
#define PNGRUTIL_2053_112_2_18_1_43_0_1486 "---PNG WARNING---Not enough memory to process zTXt chunk."
#define PNGRUTIL_2073_112_2_18_1_43_0_1487 "--PNG ERR Info--Insufficient memory to store zTXt chunk."
#define PNGRUTIL_2093_112_2_18_1_43_0_1488 "--PNG ERR Info--Missing IHDR before iTXt"
#define PNGRUTIL_2104_112_2_18_1_43_0_1489 "---PNG WARNING---iTXt chunk too large to fit in memory"
#define PNGRUTIL_2113_112_2_18_1_43_0_1490 "---PNG WARNING---No memory to process iTXt chunk."
#define PNGRUTIL_2137_112_2_18_1_43_0_1491 "---PNG WARNING---Zero length iTXt chunk"
#define PNGRUTIL_2165_112_2_18_1_43_0_1492 "---PNG WARNING---Not enough memory to process iTXt chunk."
#define PNGRUTIL_2183_112_2_18_1_43_0_1493 "--PNG ERR Info--Insufficient memory to store iTXt chunk."
#define PNGRUTIL_2222_112_2_18_1_43_0_1494 "---PNG ERR INFO---unknown critical chunk"
#define PNGRUTIL_2235_112_2_18_1_43_0_1495 "---PNG WARNING---unknown chunk too large to fit in memory"
#define PNGRUTIL_2255_112_2_18_1_43_0_1496 "---PNG ERR INFO---unknown critical chunk"
#define PNGRUTIL_2291_112_2_18_1_43_0_1497 "---PNG ERR INFO---invalid chunk type"
#define PNGRUTIL_2847_112_2_18_1_43_1_1498 "---PNG WARNING---Ignoring bad adaptive filter type"
#define PNGRUTIL_2940_112_2_18_1_43_1_1499 "---PNG ERR INFO---Not enough image data"
#define PNGRUTIL_2957_112_2_18_1_43_1_1500 "---PNG WARNING---Extra compressed data"
#define PNGRUTIL_2966_112_2_18_1_43_2_1501 "--PNG ERR Info--Decompression Error"
#define PNGRUTIL_2975_112_2_18_1_43_2_1502 "---PNG WARNING---Extra compressed data."
#define PNGRUTIL_2986_112_2_18_1_43_2_1503 "---PNG WARNING---Extra compression data"
#define PNGRUTIL_3040_112_2_18_1_43_2_1504 "--PNG ERR Info--Rowbytes overflow in png_read_start_row"
#define PNGRUTIL_3160_112_2_18_1_43_2_1505 "--PNG ERR Info--This image requires a row greater than 64KB"
#define PNGRUTIL_3171_112_2_18_1_43_2_1506 "img_png_read_start_row: cano not allocate png_ptr->big_row_buf "
#define PNGRUTIL_3182_112_2_18_1_43_2_1507 "--PNG ERR Info--This image requires a row greater than 64KB"
#define PNGRUTIL_3187_112_2_18_1_43_2_1508 "--PNG ERR Info--Row has too many bytes to allocate in memory."
#define PNGSET_46_112_2_18_1_43_2_1509 "---PNG WARNING---Ignoring attempt to set negative chromaticity value"
#define PNGSET_54_112_2_18_1_43_2_1510 "---PNG WARNING---Ignoring attempt to set chromaticity value exceeding 21474.83"
#define PNGSET_94_112_2_18_1_43_2_1511 "---PNG WARNING---Ignoring attempt to set negative chromaticity value"
#define PNGSET_114_112_2_18_1_43_2_1512 "---PNG WARNING---Ignoring attempt to set chromaticity value exceeding 21474.83"
#define PNGSET_152_112_2_18_1_43_3_1513 "---PNG WARNING---Limiting gamma to 21474.83"
#define PNGSET_163_112_2_18_1_43_3_1514 "---PNG WARNING---Setting gamma=0"
#define PNGSET_177_112_2_18_1_43_3_1515 "---PNG WARNING---Limiting gamma to 21474.83"
#define PNGSET_184_112_2_18_1_43_3_1516 "---PNG WARNING---Setting negative gamma to zero"
#define PNGSET_198_112_2_18_1_43_3_1517 "---PNG WARNING---Setting gamma=0"
#define PNGSET_212_112_2_18_1_43_3_1518 "---PNG WARNING---Palette size 0, hIST allocation skipped."
#define PNGSET_224_112_2_18_1_43_3_1519 "---PNG WARNING---Insufficient memory for hIST chunk data."
#define PNGSET_253_112_2_18_1_43_3_1520 "--PNG ERR Info--Image width or height is zero in IHDR"
#define PNGSET_258_112_2_18_1_43_3_1521 "--PNG ERR Info--image size exceeds user limits in IHDR"
#define PNGSET_263_112_2_18_1_43_3_1522 "--PNG ERR Info--image size exceeds user limits in IHDR"
#define PNGSET_268_112_2_18_1_43_3_1523 "--PNG ERR Info--Invalid image size in IHDR"
#define PNGSET_276_112_2_18_1_43_3_1524 "---PNG WARNING---Width is too large for libpng to process pixels"
#define PNGSET_282_112_2_18_1_43_3_1525 "--PNG ERR Info--Invalid bit depth in IHDR"
#define PNGSET_288_112_2_18_1_43_3_1526 "--PNG ERR Info--Invalid color type in IHDR"
#define PNGSET_296_112_2_18_1_43_3_1527 "--PNG ERR Info--Invalid color type/bit depth combination in IHDR"
#define PNGSET_301_112_2_18_1_43_3_1528 "--PNG ERR Info--Unknown interlace method in IHDR"
#define PNGSET_306_112_2_18_1_43_3_1529 "--PNG ERR Info--Unknown compression method in IHDR"
#define PNGSET_321_112_2_18_1_43_3_1530 "---PNG WARNING---MNG features are not allowed in a PNG datastream\n"
#define PNGSET_331_112_2_18_1_43_3_1531 "--PNG ERR Info--Unknown filter method in IHDR"
#define PNGSET_335_112_2_18_1_43_3_1532 "---PNG WARNING---Invalid filter method in IHDR"
#define PNGSET_341_112_2_18_1_43_3_1533 "--PNG ERR Info--Unknown filter method in IHDR"
#define PNGSET_406_112_2_18_1_43_3_1534 "---PNG WARNING---Insufficient memory for pCAL purpose."
#define PNGSET_422_112_2_18_1_43_3_1535 "---PNG WARNING---Insufficient memory for pCAL units."
#define PNGSET_431_112_2_18_1_43_3_1536 "---PNG WARNING---Insufficient memory for pCAL params."
#define PNGSET_444_112_2_18_1_43_3_1537 "---PNG WARNING---Insufficient memory for pCAL parameter."
#define PNGSET_490_112_2_18_1_43_3_1538 "---PNG WARNING---Memory allocation failed while processing sCAL."
#define PNGSET_500_112_2_18_1_43_3_1539 "---PNG WARNING---Memory allocation failed while processing sCAL."
#define PNGSET_672_112_2_18_1_43_4_1540 "---PNG WARNING---Insufficient memory to process iCCP chunk."
#define PNGSET_680_112_2_18_1_43_4_1541 "---PNG WARNING---Insufficient memory to process iCCP profile."
#define PNGSET_707_112_2_18_1_43_4_1542 "--PNG ERR INFO--Insufficient memory to store text"
#define PNGSET_791_112_2_18_1_43_4_1543 "---PNG WARNING---iTXt chunk not supported."
#define PNGSET_937_112_2_18_1_43_4_1544 "---PNG WARNING---No memory for sPLT palettes."
#define PNGSET_990_112_2_18_1_43_4_1545 "---PNG WARNING---Out of memory while processing unknown chunk."
#define PNGSET_1008_112_2_18_1_43_4_1546 "---PNG WARNING---Out of memory processing unknown chunk."
#define PNGTRANS_596_112_2_18_1_43_6_1547 "---PNG WARNING---This version of libpng does not support user transform info"
#define SCALING_DOWN_CUS_124_112_2_18_1_43_8_1548 "SPRD_IMG_Scaling_cust: Init_Scale_Info_cus"
#define SCALING_DOWN_CUS_134_112_2_18_1_43_8_1549 "SPRD_IMG_Scaling_cust : CX_SCALING_CUS"
#define SCALING_DOWN_CUS_141_112_2_18_1_43_8_1550 "test SPRD_IMG_Scaling_cust; After CX_SCALING_CUS"
#define SCALING_DOWN_CUS_491_112_2_18_1_43_8_1551 " Init_Scale_Info_cus: Info->input_width= %d , input_height = %d"
#define SCALING_DOWN_CUS_496_112_2_18_1_43_8_1552 "Init_Scale_Info_cus: h_ratio -%d, v_ratio = %d"
#define SCALING_DOWN_CUS_508_112_2_18_1_43_8_1553 "Init_Scale_Info_cus: N= %d"
#define IMG_SHARPEN_107_112_2_18_1_43_11_1554 "sharpen src addr: %d\n"
#define IMG_SHARPEN_108_112_2_18_1_43_11_1555 "sharpen dst addr: %d\n"
#define IMG_SHARPEN_123_112_2_18_1_43_11_1556 "Sharpen_Test"
#define IMG_SHARPEN_128_112_2_18_1_43_11_1557 "--img_Sharpen_App:malloc fails!----"
#define UI_SPECIAL_EFFECT_933_112_2_18_1_43_15_1558 "[UISE: SE_GetEffectFrameAmount] undefined effect id"
#define UI_SPECIAL_EFFECT_967_112_2_18_1_43_15_1559 "[UISE: SE_DesktopRotate]: Enter"
#define UI_SPECIAL_EFFECT_981_112_2_18_1_43_15_1560 "[UISE: SE_DesktopRotate]: Use table 0"
#define UI_SPECIAL_EFFECT_987_112_2_18_1_43_15_1561 "[UISE: SE_DesktopRotate]: Use table 1"
#define UI_SPECIAL_EFFECT_991_112_2_18_1_43_15_1562 "[UISE: SE_DesktopRotate]: Exit error, no proper table"
#define UI_SPECIAL_EFFECT_1032_112_2_18_1_43_15_1563 "[UISE: SE_DesktopRotate] undefined direction"
#define UI_SPECIAL_EFFECT_1051_112_2_18_1_43_15_1564 "[UISE: SE_DesktopRotate] frame = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1052_112_2_18_1_43_15_1565 "[UISE: SE_DesktopRotate]: Exit"
#define UI_SPECIAL_EFFECT_1085_112_2_18_1_43_15_1566 "[UISE: SE_MenuItemMove]: Enter"
#define UI_SPECIAL_EFFECT_1134_112_2_18_1_43_15_1567 "[UISE: SE_MenuItemMove] undefined direction"
#define UI_SPECIAL_EFFECT_1150_112_2_18_1_43_15_1568 "[UISE: SE_MenuItemMove] frame = %d, direction = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1152_112_2_18_1_43_15_1569 "[UISE: SE_MenuItemMove]: Exit"
#define UI_SPECIAL_EFFECT_1178_112_2_18_1_43_15_1570 "[UISE: SE_MagicMove]: Enter"
#define UI_SPECIAL_EFFECT_1204_112_2_18_1_43_15_1571 "[UISE: SE_MagicMove]: undefined alpha value"
#define UI_SPECIAL_EFFECT_1227_112_2_18_1_43_15_1572 "[UISE: SE_MagicMove] frame = %d, direction = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1228_112_2_18_1_43_15_1573 "[UISE: SE_MagicMove]: Exit"
#define UI_SPECIAL_EFFECT_1256_112_2_18_1_43_15_1574 "[UISE: SE_SlideMove]: Enter"
#define UI_SPECIAL_EFFECT_1318_112_2_18_1_43_15_1575 "[UISE: SE_SlideMove] undefined direction"
#define UI_SPECIAL_EFFECT_1335_112_2_18_1_43_15_1576 "[UISE: SE_SlideMove] frame = %d, direction = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1336_112_2_18_1_43_15_1577 "[UISE: SE_SlideMove]: Exit"
#define UI_SPECIAL_EFFECT_1355_112_2_18_1_43_15_1578 "[UISE: SE_RotateMove3D] Enter"
#define UI_SPECIAL_EFFECT_1378_112_2_18_1_43_15_1579 "[UISE: SE_RotateMove3D] undefined direction"
#define UI_SPECIAL_EFFECT_1385_112_2_18_1_43_15_1580 "[UISE: SE_RotateMove3D] frame = %d, direction = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1386_112_2_18_1_43_15_1581 "[UISE: SE_RotateMove3D] Exit"
#define UI_SPECIAL_EFFECT_1416_112_2_18_1_43_16_1582 "[UISE: SE_RotateMove]: Enter"
#define UI_SPECIAL_EFFECT_1465_112_2_18_1_43_16_1583 "[UISE: SE_RotateMove] undefined rotate angle"
#define UI_SPECIAL_EFFECT_1497_112_2_18_1_43_16_1584 "[UISE: SE_RotateMove] undefined rotate angle"
#define UI_SPECIAL_EFFECT_1505_112_2_18_1_43_16_1585 "[UISE: SE_RotateMove] undefined direction"
#define UI_SPECIAL_EFFECT_1538_112_2_18_1_43_16_1586 "[UISE: SE_RotateMove] frame = %d, direction = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1540_112_2_18_1_43_16_1587 "[UISE: SE_RotateMove]: Exit"
#define UI_SPECIAL_EFFECT_1559_112_2_18_1_43_16_1588 "[UISE: SE_DistortMove]: Enter"
#define UI_SPECIAL_EFFECT_1584_112_2_18_1_43_16_1589 "[UISE: SE_DistortMove] undefined direction"
#define UI_SPECIAL_EFFECT_1591_112_2_18_1_43_16_1590 "[UISE: SE_DistortMove] frame = %d, direction = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1593_112_2_18_1_43_16_1591 "[UISE: SE_DistortMove]: Exit"
#define UI_SPECIAL_EFFECT_1628_112_2_18_1_43_16_1592 "[UISE: SE_LuckyBoard]: Enter"
#define UI_SPECIAL_EFFECT_1781_112_2_18_1_43_16_1593 "[UISE: SE_LuckyBoard] undefined direction"
#define UI_SPECIAL_EFFECT_1793_112_2_18_1_43_16_1594 "[UISE: SE_LuckyBoard] frame = %d, direction = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1795_112_2_18_1_43_16_1595 "[UISE: SE_LuckyBoard] icon_width = %d, icon_height = %d, output_width = %d, output_height = %d"
#define UI_SPECIAL_EFFECT_1797_112_2_18_1_43_16_1596 "[UISE: SE_LuckyBoard]: Exit, output_rect = (%d, %d, %d, %d)"
#define UI_SPECIAL_EFFECT_1816_112_2_18_1_43_16_1597 "[UISE: SE_WrappedMove], Enter"
#define UI_SPECIAL_EFFECT_1840_112_2_18_1_43_17_1598 "[UISE: SE_WrappedMove] undefined direction"
#define UI_SPECIAL_EFFECT_1847_112_2_18_1_43_17_1599 "[UISE: SE_WrappedMove] frame = %d, direction = %d, ticks = %d"
#define UI_SPECIAL_EFFECT_1849_112_2_18_1_43_17_1600 "[UISE: SE_WrappedMove], Exit"
#define UI_SPECIAL_EFFECT_1925_112_2_18_1_43_17_1601 "[UISE: SE_CheckInputParam] input_param_ptr: effect_id = %d, direction = %d, frame_index = %d"
#define UI_SPECIAL_EFFECT_1931_112_2_18_1_43_17_1602 "[UISE: SE_CheckInputParam] input_param_ptr: selected_rect = (%d, %d, %d, %d), next_rect = (%d, %d, %d, %d)"
#define UI_SPECIAL_EFFECT_1935_112_2_18_1_43_17_1603 "[UISE: SE_CheckInputParam] input_param_ptr: menu_rect = (%d, %d, %d, %d)"
#define UI_SPECIAL_EFFECT_2836_112_2_18_1_43_19_1604 "[UISE: SE_QuickRotateImage] undefined angle"
#define UI_SPECIAL_EFFECT_3163_112_2_18_1_43_19_1605 "[UISE: SE_QuickHorzTrapezaWithZoom] undefined zoom level"
#define UI_SPECIAL_EFFECT_3414_112_2_18_1_43_20_1606 "[UISE: SE_QuickHorzTrapezaWithZoom] undefined zoom level"
#define UI_SPECIAL_EFFECT_3702_112_2_18_1_43_20_1607 "[UISE: SE_QuickVertTrapezaWithZoom] undefined zoom level"
#define UI_SPECIAL_EFFECT_3935_112_2_18_1_43_21_1608 "[UISE: SE_QuickVertTrapezaWithZoom] undefined zoom level"
#define UI_SPECIAL_EFFECT_6399_112_2_18_1_43_26_1609 "[UISE: SE_CalcSlideMoveParam] undefined direction"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_IMAGE_PROC_TRC)
TRACE_MSG(BMP_APP_224_112_2_18_1_34_9_0,"bmp_rotate_180:\n")
TRACE_MSG(BMP_APP_267_112_2_18_1_34_9_1,"BMP Decode: bmp_param_check:\n")
TRACE_MSG(BMP_APP_628_112_2_18_1_34_9_2,"BMPAPP_BmpDecode:rect top %d, bottom %d, left %d,right %d")
TRACE_MSG(BMP_APP_629_112_2_18_1_34_9_3,"BMPAPP_BmpDecode:cropwidth %d, cropheight %d")
TRACE_MSG(BMP_APP_706_112_2_18_1_34_10_4,"BMPAPP_BmpDecode:p_input_info.input_width %d, p_input_info.input_height  %d")
TRACE_MSG(BMP_APP_768_112_2_18_1_34_10_5,"BMPAPP_BmpDecode: Decoding Succeed")
TRACE_MSG(BMP_APP_769_112_2_18_1_34_10_6,"BMPAPP_BmpDecode: End Target W: %d, H:%d  ")
TRACE_MSG(BMP_APP_770_112_2_18_1_34_10_7,"BMPAPP_BmpDecode: End Src W: %d, H:%d  ")
TRACE_MSG(BMP_APP_771_112_2_18_1_34_10_8,"BMPAPP_BmpDecode: End bit W: %d,")
TRACE_MSG(BMP_APP_775_112_2_18_1_34_10_9,"BMPAPP_BmpDecode: Decoding Fail,Bad file")
TRACE_MSG(BMP_DECODE_2010_112_2_18_1_34_16_10,"BMP_Decoder_Init:\n")
TRACE_MSG(BMP_DECODE_2247_112_2_18_1_34_16_11,"destroy_bmp_decoder:\n")
TRACE_MSG(BMP_DECODE_2282_112_2_18_1_34_16_12,"BMP: BMP_Get_No_Scaling_Data in rgb_ptr:%d , line count: %d")
TRACE_MSG(BMP_DECODE_2289_112_2_18_1_34_16_13,"BMP: BMP_Get_No_Scaling_Data bad BMP out")
TRACE_MSG(BMP_DECODE_2339_112_2_18_1_34_16_14,"BMP_Get_Data:\n")
TRACE_MSG(BMP_DECODE_2353_112_2_18_1_34_17_15,"Error happen")
TRACE_MSG(BMP_TASK_372_112_2_18_1_34_18_16,"BMP_DEC_TaskClose1")
TRACE_MSG(BMP_TASK_401_112_2_18_1_34_18_17,"BMP_DEC_TaskClose3: %d")
TRACE_MSG(BMP_TASK_404_112_2_18_1_34_18_18,"BMP_DEC_TaskClose4: %d")
TRACE_MSG(BMP_TASK_482_112_2_18_1_34_18_19,"_BMP_DECODER_SetEvent+")
TRACE_MSG(BMP_TASK_488_112_2_18_1_34_18_20,"_BMP_DECODER_SetEvent-")
TRACE_MSG(BMP_TASK_518_112_2_18_1_34_18_21,"BMPAPP_GetFrame mid +")
TRACE_MSG(BMP_TASK_535_112_2_18_1_34_18_22,"BMPAPP_GetFrame mid")
TRACE_MSG(BMP_TASK_556_112_2_18_1_34_18_23,"BMPAPP_GetFrame -")
TRACE_MSG(BMP_TASK_874_112_2_18_1_34_19_24,"[IMGDECbmp_Dec] bytes_written = %d")
TRACE_MSG(BMP_TASK_878_112_2_18_1_34_19_25,"[IMGDECbmp_Dec] create file failed")
TRACE_MSG(BMP_TASK_882_112_2_18_1_34_19_26,"IMGDECbmp_Dec, target_ptr= %x, jpeg_size= %d")
TRACE_MSG(BMP_TASK_883_112_2_18_1_34_19_27,"IMGDECbmp_Dec, target_width = %d, target_height = %d")
TRACE_MSG(BMP_TASK_884_112_2_18_1_34_19_28,"IMGDECbmp_Dec,out_param.real_width = %d, out_param.real_height = %d")
TRACE_MSG(BMP_TASK_994_112_2_18_1_34_19_29,"_BMP_DEC_Task_Routine bmp_handle: %d +1")
TRACE_MSG(BMP_TASK_1004_112_2_18_1_34_19_30,"_BMP_DEC_Task_Routine bmp_handle: %d -1")
TRACE_MSG(BMP_TASK_1023_112_2_18_1_34_19_31,"BMP_DECODER_Task_Init: s_bmp_decoder_thread_id = %d+")
TRACE_MSG(BMP_TASK_1051_112_2_18_1_34_19_32,"BMP_DECODER_Task_Init: s_bmp_decoder_thread_id = %d  ddd")
TRACE_MSG(BMP_TASK_1061_112_2_18_1_34_19_33,"BMP_DECODER_Task_Init: s_bmp_decoder_thread_id = %d-")
TRACE_MSG(BMP_TASK_1090_112_2_18_1_34_19_34,"BMPAPP_WBMP_GetInfo: width header_count:%d")
TRACE_MSG(BMP_TASK_1108_112_2_18_1_34_20_35,"BMPAPP_WBMP_GetInfo: height header_count:%d")
TRACE_MSG(WBMP_DECODE_43_112_2_18_1_34_20_36,"wbmp_init:\n")
TRACE_MSG(WBMP_DECODE_136_112_2_18_1_34_20_37,"wbmp_init: width header_count:%d")
TRACE_MSG(WBMP_DECODE_154_112_2_18_1_34_20_38,"wbmp_init: height header_count:%d")
TRACE_MSG(WBMP_DECODE_178_112_2_18_1_34_20_39,"wbmp_init: imgsize:%d, img_width:%d, img_height:%d")
TRACE_MSG(WBMP_DECODE_429_112_2_18_1_34_21_40,"WBMPDecode:line_count: %d\n")
TRACE_MSG(WBMP_DECODE_451_112_2_18_1_34_21_41,"WBMPDecode:line_count: %d\n")
TRACE_MSG(WBMP_DECODE_555_112_2_18_1_34_21_42,"WBMPDecode_NoScaling:line_count: %d\n")
TRACE_MSG(IMG_CASCADE_126_112_2_18_1_34_21_43,"TIM_TEST: _Get_MatchedPoint  %d, %d, %d")
TRACE_MSG(IMG_CASCADE_187_112_2_18_1_34_22_44,"TIM_TEST: _Img_Combine offset :%d, %d, %d, %d")
TRACE_MSG(IMG_CASCADE_295_112_2_18_1_34_22_45,"TIM_TEST: _Img_PanoramaJointTwoImg addr :0x%x, 0x%x")
TRACE_MSG(IMG_CASCADE_345_112_2_18_1_34_22_46,"TIM_TEST: _Img_PanoramaJointTwoImg y  %d")
TRACE_MSG(IMG_CASCADE_376_112_2_18_1_34_22_47,"TIM_TEST: _Img_PanoramaJointTwoImg x :%d")
TRACE_MSG(IMG_CASCADE_410_112_2_18_1_34_22_48,"TIM_TEST: _Img_PanoramaJointTwoImg y :%d")
TRACE_MSG(IMG_CASCADE_582_112_2_18_1_34_23_49,"TIM_TEST: Img_PanoramaMosaic output :0x%x, 0x%x, %d, %d, %d")
TRACE_MSG(BALLOON_106_112_2_18_1_34_23_50,"bd_vel.x: %d, bd_vel.y:%d")
TRACE_MSG(BALLOON_170_112_2_18_1_34_23_51,"draw circle body pos coordinate, p.x:%f, p.y: %f")
TRACE_MSG(BALLOON_265_112_2_18_1_34_23_52,"draw circle body pos coordinate, p.x:%f, p.y: %f")
TRACE_MSG(DRAWSPACE_577_112_2_18_1_35_9_53,"drawObject, rot x:%d, y: %d")
TRACE_MSG(DRAWSPACE_597_112_2_18_1_35_9_54,"drawPolyShape%d:x%d,y%d")
TRACE_MSG(DRAWSPACE_615_112_2_18_1_35_9_55,"drawPolyShape:max_x: %d")
TRACE_MSG(PLINK_360_112_2_18_1_35_17_56,"chipmunk: 0x%x")
TRACE_MSG(IMAGE_ALGORITHM_145_112_2_18_1_35_42_57,"DC: JIALIAN rotation 270")
TRACE_MSG(IMAGE_ALGORITHM_208_112_2_18_1_35_42_58,"DC: JIALIAN angle%d")
TRACE_MSG(IMAGE_ALGORITHM_232_112_2_18_1_35_42_59,"[Img_RotateYUV422_Y_UV:run default of switch!]")
TRACE_MSG(DC_ALGORITHM_447_112_2_18_1_35_44_60,"TIM:_Get_JpegSliceEncoderIram,ilock handle 0x%x,0x%x,0x%x")
TRACE_MSG(DC_ALGORITHM_677_112_2_18_1_35_44_61,"DCAM: DC_ArgbOverlayer src_buf_rect y=%d")
TRACE_MSG(DC_ALGORITHM_678_112_2_18_1_35_44_62,"DCAM: DC_ArgbOverlayer src_buf_rect h=%d")
TRACE_MSG(DC_ALGORITHM_679_112_2_18_1_35_44_63,"DCAM: DC_ArgbOverlayer overlay_rect.y=%d")
TRACE_MSG(DC_ALGORITHM_680_112_2_18_1_35_44_64,"DCAM: DC_ArgbOverlayer overlay_rect.h=%d")
TRACE_MSG(DC_ALGORITHM_687_112_2_18_1_35_44_65,"DCAM: DC_ArgbOverlayer error src_addr:x=%d, w=%d")
TRACE_MSG(DC_ALGORITHM_714_112_2_18_1_35_44_66,"DCAM: DC_ArgbOverlayer src_buf_rect x =%d y=%d")
TRACE_MSG(DC_ALGORITHM_715_112_2_18_1_35_44_67,"DCAM: DC_ArgbOverlayer src_buf_rect w =%d h=%d")
TRACE_MSG(DC_ALGORITHM_716_112_2_18_1_35_44_68,"DCAM: DC_ArgbOverlayer overlay_rect.x =%d y=%d")
TRACE_MSG(DC_ALGORITHM_717_112_2_18_1_35_44_69,"DCAM: DC_ArgbOverlayer overlay_rect.w =%d h=%d")
TRACE_MSG(DC_ALGORITHM_956_112_2_18_1_35_45_70,"DCAM: ROTATION IS NOT SUPPORTTED!")
TRACE_MSG(DC_ALGORITHM_1028_112_2_18_1_35_45_71,"DCAM capture rotation buf is not enough need_buf=0x%x,rot_buf=0x%x")
TRACE_MSG(EXIF_APP_337_112_2_18_1_35_46_72,"SplitExif_app: pThumboffset= %d, ret: %d")
TRACE_MSG(EXIF_APP_338_112_2_18_1_35_46_73,"SplitExif_app 1 = 0x%x")
TRACE_MSG(EXIF_APP_339_112_2_18_1_35_46_74,"SplitExif_app 2 = 0x%x")
TRACE_MSG(EXIF_APP_340_112_2_18_1_35_46_75,"SplitExif_app 2 = 0x%x")
TRACE_MSG(EXIFPARSER_442_112_2_18_1_35_52_76,"_JP_ExifAPPnParse1_  :find thumbnail")
TRACE_MSG(EXIFPARSER_480_112_2_18_1_35_52_77,"_JP_ExifAPPnParse1_: err = %d")
TRACE_MSG(EXIFPARSER_485_112_2_18_1_35_52_78,"_JP_ExifAPPnParse1_  Memcpy Thumbnail")
TRACE_MSG(EXIFPARSER_1153_112_2_18_1_35_53_79," _JP_ExifSOFParse: : isThumb: %d ,pdata[0x0B] = %d")
TRACE_MSG(EXIFPARSER_1470_112_2_18_1_35_54_80,"EXIF _API_ExifParamParse Next AppLen = %d")
TRACE_MSG(EXIFPARSER_1486_112_2_18_1_35_54_81,"EXIF _API_ExifParamParse Next ADDR = %d")
TRACE_MSG(GIF_APP_CTRL_248_112_2_18_1_35_55_82,"data format is invailid: format is %d")
TRACE_MSG(GIF_APP_CTRL_357_112_2_18_1_35_55_83,"_GIFDEC_ReadFile:read file error!")
TRACE_MSG(GIF_APP_CTRL_382_112_2_18_1_35_55_84,"s_is_continue_decode_flag=%d,time=%d")
TRACE_MSG(GIF_APP_CTRL_708_112_2_18_1_35_56_85,"GIFDEC: Recieve Sig=0x%x, param addr=0x%x , cur time=%d")
TRACE_MSG(GIF_APP_CTRL_770_112_2_18_1_35_56_86,"[IMGDECGIF_Dec] bytes_written = %d")
TRACE_MSG(GIF_APP_CTRL_774_112_2_18_1_35_56_87,"[IMGDECGIF_Dec] create file failed")
TRACE_MSG(GIF_APP_CTRL_779_112_2_18_1_35_56_88,"IMGDECGIF_Dec, target_ptr = %x, jpeg_size = %d")
TRACE_MSG(GIF_APP_CTRL_780_112_2_18_1_35_56_89,"IMGDECGIF_Dec, target_width = %d, target_height = %d")
TRACE_MSG(GIF_APP_CTRL_781_112_2_18_1_35_56_90,"IMGDECGIF_Dec,out_param.real_width = %d, out_param.real_height = %d")
TRACE_MSG(GIF_APP_CTRL_812_112_2_18_1_35_56_91,"gif dec finish a file time=%d,handle=0x%x")
TRACE_MSG(GIF_APP_CTRL_843_112_2_18_1_35_56_92,"--_GIFDEC_Decoder:is_exist_background=%d---")
TRACE_MSG(GIF_APP_CTRL_1014_112_2_18_1_35_57_93,"IMGDECGIF_Create,time=%d")
TRACE_MSG(GIF_APP_CTRL_1063_112_2_18_1_35_57_94,"IMGDECGIF_Create,out_time=%d,handle=0x%x")
TRACE_MSG(GIF_APP_CTRL_1085_112_2_18_1_35_57_95,"IMGDECGIF_Destroy handle=0x%x,s_gif_handle=0x%x,time=%d")
TRACE_MSG(GIF_APP_CTRL_1155_112_2_18_1_35_57_96,"IMGDECGIF_Destroy out time=%d")
TRACE_MSG(GIF_APP_CTRL_1184_112_2_18_1_35_57_97,"IMGDECGIF_GetFrame handle=0x%x,callback=0x%x,time=%d,is_exit_back=%d")
TRACE_MSG(GIF_APP_CTRL_1191_112_2_18_1_35_57_98,"IMGDECGIF_GetFrame:data format is error! %d")
TRACE_MSG(GIF_APP_CTRL_1276_112_2_18_1_35_57_99,"IMGDECGIF_GetFrame handle=0x%x,out time=%d")
TRACE_MSG(GIF_DECODE_APP_58_112_2_18_1_35_58_100,"GIFDECODEAPP_FreeDecodeRes in")
TRACE_MSG(GIF_DECODE_APP_86_112_2_18_1_35_58_101,"GIFDECODEAPP_FreeDecodeRes Out")
TRACE_MSG(GIF_DECODE_APP_104_112_2_18_1_35_58_102,"GIFDECODEAPP_FreeScaleRes")
TRACE_MSG(GIF_DECODE_APP_463_112_2_18_1_35_59_103,"Error file")
TRACE_MSG(GIF_DECODE_APP_476_112_2_18_1_35_59_104,"Error file")
TRACE_MSG(GIF_DECODE_APP_608_112_2_18_1_35_59_105,"get_gif_no_scaling_frame: No Scaling line_total =  %d")
TRACE_MSG(GIF_DECODE_APP_857_112_2_18_1_35_59_106,"gif crop parameter err crop_width = %d")
TRACE_MSG(GIF_DECODE_APP_864_112_2_18_1_35_59_107,"gif crop parameter err crop_height = %d")
TRACE_MSG(GIF_DECODE_APP_886_112_2_18_1_36_0_108,"gif_size_handle: first frame sample %d, %d")
TRACE_MSG(GIF_DECODE_APP_1013_112_2_18_1_36_0_109,"gif crop parameter err crop_width = %d")
TRACE_MSG(GIF_DECODE_APP_1020_112_2_18_1_36_0_110,"gif crop parameter err crop_height = %d")
TRACE_MSG(GIF_DECODE_APP_1042_112_2_18_1_36_0_111,"gif_size_handle_non_first_frame: first frame sample %d, %d")
TRACE_MSG(GIF_DECODE_APP_1126_112_2_18_1_36_0_112,"format is error!")
TRACE_MSG(GIF_DECODE_APP_1132_112_2_18_1_36_0_113,"--_get_gif_frame_with_scale:get first frame!---")
TRACE_MSG(GIF_DECODE_APP_1179_112_2_18_1_36_0_114,"_get_gif_frame_with_scale: target %d, %d, src %d, %d")
TRACE_MSG(GIF_DECODE_APP_1255_112_2_18_1_36_0_115,"_get_gif_frame_with_scale:target change target %d, %d, src %d, %d")
TRACE_MSG(GIF_DECODE_APP_1305_112_2_18_1_36_0_116,"_get_gif_frame_with_scale: target %d, %d, src %d, %d")
TRACE_MSG(GIF_DECODE_APP_1329_112_2_18_1_36_1_117,"_get_gif_frame_with_scale:start decode! ")
TRACE_MSG(GIF_DECODER_67_112_2_18_1_36_1_118,"--_set_current_decoder_handle:0x%x--")
TRACE_MSG(GIF_DECODER_673_112_2_18_1_36_2_119,"gif_data_compound: in!")
TRACE_MSG(GIF_DECODER_806_112_2_18_1_36_3_120,"gif_data_compound: out!")
TRACE_MSG(GIF_DECODER_825_112_2_18_1_36_3_121,"gif_data_compound_argb_noscale: in!")
TRACE_MSG(GIF_DECODER_955_112_2_18_1_36_3_122,"gif_data_compound_argb_noscale: out!")
TRACE_MSG(GIF_DECODER_987_112_2_18_1_36_3_123,"gif_data_compound: in!")
TRACE_MSG(GIF_DECODER_1110_112_2_18_1_36_3_124,"gif_data_compound: out!")
TRACE_MSG(GIF_DECODER_1429_112_2_18_1_36_4_125,"out_line interleave:%d,%d")
TRACE_MSG(GIF_DECODER_1962_112_2_18_1_36_5_126,"seek_next_image: return ")
TRACE_MSG(GIF_DECODER_2056_112_2_18_1_36_5_127,"IMG_PROC_gif_decode_init:src %d,%d")
TRACE_MSG(GIF_DECODER_2088_112_2_18_1_36_5_128,"GIF decode buf size too small!")
TRACE_MSG(GIF_DECODER_2096_112_2_18_1_36_5_129,"GIF decode buf size too small!")
TRACE_MSG(GIF_DECODER_2164_112_2_18_1_36_5_130,"===IMG_PROC_gif_decode_init:is_exist_background=%d====")
TRACE_MSG(GIF_DECODER_2251_112_2_18_1_36_5_131,"get_gif_data image %d,%d,%d,%d")
TRACE_MSG(GIF_DECODER_2260_112_2_18_1_36_6_132,"No Support this file !")
TRACE_MSG(GIF_DECODER_2341_112_2_18_1_36_6_133,"get_gif_data bad code")
TRACE_MSG(GIF_DECODER_2502_112_2_18_1_36_6_134,"IMG_PROC_GIF_Decode_Get_Data return line%d")
TRACE_MSG(GIF_DECODER_2599_112_2_18_1_36_6_135,"[IMG_PROC_GIF_Decode_Get_Data:state err!]")
TRACE_MSG(GIF_DECODER_2636_112_2_18_1_36_6_136,"IMG_PROC_GIF_Decode_Get_Data return line%d")
TRACE_MSG(GIF_REENTRANT_DECODER_454_112_2_18_1_36_7_137,"----GIFRES_Dec point 0----")
TRACE_MSG(GIF_REENTRANT_DECODER_478_112_2_18_1_36_7_138,"----Error file---")
TRACE_MSG(GIF_REENTRANT_DECODER_534_112_2_18_1_36_8_139,"----GIFRES_Dec point 3----")
TRACE_MSG(GIF_REENTRANT_DECODER_571_112_2_18_1_36_8_140,"----Error file---")
TRACE_MSG(GIF_REENTRANT_DECODER_631_112_2_18_1_36_8_141,"---DEC_MALLOC_FAIL---")
TRACE_MSG(GIF_REENTRANT_DECODER_1416_112_2_18_1_36_9_142,"seek_next_image: return : %d")
TRACE_MSG(GIF_REENTRANT_DECODER_1431_112_2_18_1_36_9_143,"seek_next_image: return ")
TRACE_MSG(IMG_GZIP_DEFLATE_634_112_2_18_1_36_12_144,"deflate_fast:%c")
TRACE_MSG(IMG_GZIP_DEFLATE_736_112_2_18_1_36_12_145,"deflate: %c")
TRACE_MSG(IMG_GZIP_GZIP_559_112_2_18_1_36_13_146,"unknown method %d")
TRACE_MSG(IMG_GZIP_GZIP_568_112_2_18_1_36_13_147,"encrypted -- get newer version of gzip!")
TRACE_MSG(IMG_GZIP_GZIP_574_112_2_18_1_36_13_148,"multi-part gzip file -- get newer version of gzip")
TRACE_MSG(IMG_GZIP_GZIP_581_112_2_18_1_36_13_149,"get newer version of gzip")
TRACE_MSG(IMG_GZIP_GZIP_612_112_2_18_1_36_13_150,"extra field of %u bytes ignored")
TRACE_MSG(IMG_GZIP_GZIP_690_112_2_18_1_36_13_151,"not in gzip format")
TRACE_MSG(IMG_GZIP_GZIP_694_112_2_18_1_36_13_152,"decompression OK, trailing garbage ignored")
TRACE_MSG(IMG_GZIP_LZW_21_112_2_18_1_36_16_153,"output in compress .Z format not supported")
TRACE_MSG(IMG_GZIP_UNLZW_232_112_2_18_1_36_20_154,"warning, unknown flags 0x%x")
TRACE_MSG(IMG_GZIP_UNLZW_244_112_2_18_1_36_20_155,"compressed with %d bits, can only handle %d bits")
TRACE_MSG(IMG_GZIP_UNZIP_145_112_2_18_1_36_22_156,"unzip:len %ld, siz %d")
TRACE_MSG(IMG_GZIP_UNZIP_211_112_2_18_1_36_22_157,"has more than one entry--rest ignored")
TRACE_MSG(IMG_GZIP_UNZIP_217_112_2_18_1_36_22_158,"has more than one entry -- unchanged")
TRACE_MSG(IMG_GZIP_UTIL_372_112_2_18_1_36_23_159,"error %s")
TRACE_MSG(IMG_GZIP_UTIL_384_112_2_18_1_36_23_160,"warning: %s%s")
TRACE_MSG(IMG_GZIP_UTIL_397_112_2_18_1_36_23_161,"unexpected end of file")
TRACE_MSG(ICO_DECODE_366_112_2_18_1_36_24_162,"IMG_DEC_GetInfo: Can not open file ")
TRACE_MSG(IMG_DEC_HAL_85_112_2_18_1_36_27_163,"_IMG_DEC_Get_Caller_Priority: %s, queue_name: %s, priority: %d ")
TRACE_MSG(IMG_DEC_HAL_111_112_2_18_1_36_28_164,"_IMG_DEC_Get_Caller_Priority: %s, queue_name: %s, priority: %d ")
TRACE_MSG(IMG_DEC_HAL_121_112_2_18_1_36_28_165,"IMG_DEC_ALLOC size = %d, addr = 0x%x")
TRACE_MSG(IMG_DEC_HAL_135_112_2_18_1_36_28_166,"IMG_DEC_FREE data_ptr = 0x%x")
TRACE_MSG(IMG_DEC_HAL_230_112_2_18_1_36_28_167,"IMG_DEC_GetInfo_Detail: Unkown image type")
TRACE_MSG(IMG_DEC_INTERFACE_154_112_2_18_1_36_29_168,"IMG_DEC_GetInfo: Can not open file ")
TRACE_MSG(IMG_DEC_INTERFACE_166_112_2_18_1_36_29_169,"IMG_DEC_GetInfo: unknown image type ")
TRACE_MSG(IMG_DEC_INTERFACE_237_112_2_18_1_36_29_170,"[IMG_DEC_Create +]")
TRACE_MSG(IMG_DEC_INTERFACE_245_112_2_18_1_36_29_171,"[IMG_DEC_Create -] handle = 0x%x")
TRACE_MSG(IMG_DEC_INTERFACE_265_112_2_18_1_36_29_172,"[IMG_DEC_GetFrame +], handle = 0x%x, callback = 0x%x")
TRACE_MSG(IMG_DEC_INTERFACE_297_112_2_18_1_36_29_173,"[IMG_DEC_GetFrame -], handle = 0x%x, callback = 0x%x , app_param_ptr = 0x%x  app_param_size=%d")
TRACE_MSG(IMG_DEC_INTERFACE_312_112_2_18_1_36_29_174,"[IMG_DEC_Destroy +] handle = 0x%x")
TRACE_MSG(IMG_DEC_INTERFACE_328_112_2_18_1_36_29_175,"[IMG_DEC_Destroy -] handle = 0x%x")
TRACE_MSG(IMG_DEC_INTERFACE_474_112_2_18_1_36_29_176,"0framecallback")
TRACE_MSG(IMG_DEC_TASK_148_112_2_18_1_36_30_177,"_IMG_DEC_Set_Priority, priority: %d")
TRACE_MSG(IMG_DEC_TASK_228_112_2_18_1_36_30_178,"_IMG_DEC_Get_Caller_Priority: %s, queue_name: %s, priority: %d ")
TRACE_MSG(IMG_DEC_TASK_253_112_2_18_1_36_30_179,"_IMG_DEC_Get_Caller_Priority: %s, queue_name: %s, priority: %d ")
TRACE_MSG(IMG_DEC_TASK_281_112_2_18_1_36_30_180,"[_IMG_DEC_Dec_Command_Reg +] handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_307_112_2_18_1_36_31_181,"[_IMG_DEC_Dec_Command_Reg]: can not find handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_316_112_2_18_1_36_31_182,"_IMG_DEC_Dec_Command_Reg: Running CMD++, handle: 0x%x(For runing handler,  not permit to set runing again)")
TRACE_MSG(IMG_DEC_TASK_367_112_2_18_1_36_31_183,"[_IMG_DEC_Dec_Command_Reg -] handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_384_112_2_18_1_36_31_184,"[_IMG_DEC_Dec_Handler_Reg +] handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_430_112_2_18_1_36_31_185,"[_IMG_DEC_Dec_Handler_Reg -] handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_431_112_2_18_1_36_31_186,"[_IMG_DEC_Dec_Handler_Reg -] app_handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_453_112_2_18_1_36_31_187,"[_IMG_DEC_Find_Ready_Command +]")
TRACE_MSG(IMG_DEC_TASK_457_112_2_18_1_36_31_188,"[_IMG_DEC_Find_Ready_Command] running_cmd_ptr is not null: Running app Handle:0x%x")
TRACE_MSG(IMG_DEC_TASK_473_112_2_18_1_36_31_189,"[_IMG_DEC_Find_Ready_Command], find handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_482_112_2_18_1_36_31_190,"_IMG_DEC_Find_Ready_Command: set running_cmd_ptr to 0x%x")
TRACE_MSG(IMG_DEC_TASK_512_112_2_18_1_36_31_191,"_IMG_DEC_Find_Ready_Command: set running_cmd_ptr to PNULL")
TRACE_MSG(IMG_DEC_TASK_523_112_2_18_1_36_31_192,"[_IMG_DEC_Find_Ready_Command], get frame, handle = 0x%x callback = 0x%x")
TRACE_MSG(IMG_DEC_TASK_530_112_2_18_1_36_31_193,"[_IMG_DEC_Find_Ready_Command], get frame, handle = 0x%x callback = 0x%x")
TRACE_MSG(IMG_DEC_TASK_551_112_2_18_1_36_31_194,"[_IMG_DEC_Find_Ready_Command -]")
TRACE_MSG(IMG_DEC_TASK_564_112_2_18_1_36_31_195,"[_IMG_DEC_Callback], handle = 0x%x, frame index = %d")
TRACE_MSG(IMG_DEC_TASK_595_112_2_18_1_36_31_196,"[_IMG_DEC_End_Dec +] handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_606_112_2_18_1_36_31_197,"_IMG_DEC_End_Dec: set running_cmd_ptr to PNULL")
TRACE_MSG(IMG_DEC_TASK_627_112_2_18_1_36_31_198,"[_IMG_DEC_End_Dec -] handle = 0x%x frame index = %d")
TRACE_MSG(IMG_DEC_TASK_631_112_2_18_1_36_31_199,"[_IMG_DEC_End_Dec -] ERROR!")
TRACE_MSG(IMG_DEC_TASK_636_112_2_18_1_36_31_200,"[_IMG_DEC_End_Dec -] running_cmd_ptr is PNULL")
TRACE_MSG(IMG_DEC_TASK_658_112_2_18_1_36_31_201,"[_IMG_DEC_Remove_Command +] handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_693_112_2_18_1_36_31_202,"_IMG_DEC_Remove_Command: set running_cmd_ptr to PNULL")
TRACE_MSG(IMG_DEC_TASK_741_112_2_18_1_36_32_203,"[_IMG_DEC_Remove_Command -] handle = 0x%x")
TRACE_MSG(IMG_DEC_TASK_786_112_2_18_1_36_32_204,"_IMG_DEC_Init_Handle: open file failed ")
TRACE_MSG(IMG_DEC_TASK_804_112_2_18_1_36_32_205,"_IMG_DEC_Init_Handle: get image info failed ")
TRACE_MSG(IMG_DEC_TASK_835_112_2_18_1_36_32_206,"[_GetMsg +] sig = 0x%x\n")
TRACE_MSG(IMG_DEC_TASK_841_112_2_18_1_36_32_207,"[SIG: IMG_DEC_CMD_END +]")
TRACE_MSG(IMG_DEC_TASK_846_112_2_18_1_36_32_208,"[SIG: IMG_DEC_CMD_END -]")
TRACE_MSG(IMG_DEC_TASK_857_112_2_18_1_36_32_209,"[SIG: IMG_DEC_CMD_HALT +]")
TRACE_MSG(IMG_DEC_TASK_859_112_2_18_1_36_32_210,"[SIG: IMG_DEC_CMD_HALT -]")
TRACE_MSG(IMG_DEC_TASK_874_112_2_18_1_36_32_211,"[SIG: IMG_DEC_CMD_REG_CREATE_CMD +]")
TRACE_MSG(IMG_DEC_TASK_884_112_2_18_1_36_32_212,"[SIG: IMG_DEC_CMD_REG_CREATE_CMD:malloc handle error!]")
TRACE_MSG(IMG_DEC_TASK_894_112_2_18_1_36_32_213,"[SIG: IMG_DEC_CMD_REG_CREATE_CMD -]")
TRACE_MSG(IMG_DEC_TASK_906_112_2_18_1_36_32_214,"[SIG: IMG_DEC_CMD_REG_DECODE_CMD +]")
TRACE_MSG(IMG_DEC_TASK_932_112_2_18_1_36_32_215,"[SIG: IMG_DEC_CMD_REG_DECODE_CMD -]")
TRACE_MSG(IMG_DEC_TASK_940_112_2_18_1_36_32_216,"[IMG DEC:invalid cmd]")
TRACE_MSG(IMG_DEC_TASK_946_112_2_18_1_36_32_217,"_IMG_DEC_Task_Routine: command += %d")
TRACE_MSG(IMG_DEC_TASK_959_112_2_18_1_36_32_218,"[_GetMsg -] sig = 0x%x\n")
TRACE_MSG(IMG_DEC_TASK_995_112_2_18_1_36_32_219,"_IMG_DEC_SendMsg: sig_code = %d")
TRACE_MSG(IMG_DEC_TASK_1035_112_2_18_1_36_32_220,"_IMG_DEC_Callback_SendMsg: sig_code = %d")
TRACE_MSG(IMG_JPEG_DECODER_138_112_2_18_1_36_33_221,"[	JPEGDEC: _SetDecEvent] %d")
TRACE_MSG(IMG_JPEG_DECODER_152_112_2_18_1_36_33_222,"[	JPEGDEC: _GetDecEvent] + , %d")
TRACE_MSG(IMG_JPEG_DECODER_161_112_2_18_1_36_33_223,"[	JPEGDEC: _GetDecEvent] -, actual event = %d, status = %d")
TRACE_MSG(IMG_JPEG_DECODER_181_112_2_18_1_36_33_224,"_GetResMutex +")
TRACE_MSG(IMG_JPEG_DECODER_192_112_2_18_1_36_33_225,"_PutResMutex -")
TRACE_MSG(IMG_JPEG_DECODER_265_112_2_18_1_36_33_226,"	ResMutex: _FreeAllDecMemory: free all decode buf and file")
TRACE_MSG(IMG_JPEG_DECODER_306_112_2_18_1_36_33_227,"[	JPEGDEC: _SendMsg], sig_code = %d")
TRACE_MSG(IMG_JPEG_DECODER_406_112_2_18_1_36_34_228,"JPEGDEC: _GetInfo s_jpeg_begin_offset = %d")
TRACE_MSG(IMG_JPEG_DECODER_420_112_2_18_1_36_34_229,"	ResMutex: _GetInfo: alloc read buf")
TRACE_MSG(IMG_JPEG_DECODER_433_112_2_18_1_36_34_230,"	ResMutex: _GetInfo: open file")
TRACE_MSG(IMG_JPEG_DECODER_518_112_2_18_1_36_34_231,"JPEGDEC: unsupport data format")
TRACE_MSG(IMG_JPEG_DECODER_532_112_2_18_1_36_34_232,"	JPEGDEC: _GetDecBufSize =%d")
TRACE_MSG(IMG_JPEG_DECODER_578_112_2_18_1_36_34_233,"	ResMutex: _Decode: alloc dec buf")
TRACE_MSG(IMG_JPEG_DECODER_593_112_2_18_1_36_34_234,"JPEGDEC: decode thumbnail jpeg")
TRACE_MSG(IMG_JPEG_DECODER_611_112_2_18_1_36_34_235,"	ResMutex: _Decode: open file")
TRACE_MSG(IMG_JPEG_DECODER_635_112_2_18_1_36_34_236,"	ResMutex: _Decode: alloc read buf")
TRACE_MSG(IMG_JPEG_DECODER_710_112_2_18_1_36_34_237,"JPEGDEC: unsupport data format")
TRACE_MSG(IMG_JPEG_DECODER_717_112_2_18_1_36_34_238,"JPEGDEC: _Decod s_jpeg_begin_offset = %d")
TRACE_MSG(IMG_JPEG_DECODER_781_112_2_18_1_36_35_239,"JPEGDEC: unsupport data format")
TRACE_MSG(IMG_JPEG_DECODER_801_112_2_18_1_36_35_240,"[	JPEGDEC: _JpegDecode +] handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_820_112_2_18_1_36_35_241,"[	JPEGDEC: _JpegDecode -] handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_853_112_2_18_1_36_35_242,"	ResMutex: _CreateMiniature: alloc dec buf")
TRACE_MSG(IMG_JPEG_DECODER_866_112_2_18_1_36_35_243,"ResMutex: _CreateMiniature: open file")
TRACE_MSG(IMG_JPEG_DECODER_890_112_2_18_1_36_35_244,"	ResMutex: _Decode: alloc read buf")
TRACE_MSG(IMG_JPEG_DECODER_917_112_2_18_1_36_35_245,"JPEGDEC: _CreateMiniature read progressive jpeg to memory")
TRACE_MSG(IMG_JPEG_DECODER_943_112_2_18_1_36_35_246,"JPEGDEC: _CreateMiniature s_jpeg_begin_offset = %d")
TRACE_MSG(IMG_JPEG_DECODER_982_112_2_18_1_36_35_247,"[	JPEGDEC: _JpegCreateMiniature +] handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1002_112_2_18_1_36_35_248,"[	JPEGDEC: _JpegCreateMiniature -] handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1034_112_2_18_1_36_35_249,"[IMG_JPEG_DECODER: _TaskEntry], sig=0x%x,sig_code = %d")
TRACE_MSG(IMG_JPEG_DECODER_1039_112_2_18_1_36_35_250,"[	JPEGDED: _GetMsg +] sig_code = %d, handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1057_112_2_18_1_36_35_251,"[_TaskEntry]+ SIG_DECODE, handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1059_112_2_18_1_36_35_252,"[_TaskEntry]- SIG_DECODE, handle = 0x%x, s_decode_count")
TRACE_MSG(IMG_JPEG_DECODER_1063_112_2_18_1_36_35_253,"[_TaskEntry]+ SIG_CREATE_MINI, handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1065_112_2_18_1_36_35_254,"[_TaskEntry]- SIG_CREATE_MINI, handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1069_112_2_18_1_36_35_255,"[_TaskEntry] invalid signal")
TRACE_MSG(IMG_JPEG_DECODER_1079_112_2_18_1_36_35_256,"[	JPEGDED: _GetMsg -] sig done")
TRACE_MSG(IMG_JPEG_DECODER_1110_112_2_18_1_36_35_257,"[	JPEGDEC: _CreateTask +]: handle = 0x%x, create task, priority = %d")
TRACE_MSG(IMG_JPEG_DECODER_1121_112_2_18_1_36_35_258,"[	JPEGDEC: _CreateTask +]: handle = 0x%x, task exist, priority = %d")
TRACE_MSG(IMG_JPEG_DECODER_1135_112_2_18_1_36_35_259,"[	JPEGDEC: _CloseTask +]: handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1164_112_2_18_1_36_35_260,"[	JPEGDEC: _CloseTask -]: handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1173_112_2_18_1_36_35_261,"[	JPEGDEC: _SuspendTask +]: handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1184_112_2_18_1_36_35_262,"[	JPEGDEC: _SuspendTask -]: handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1195_112_2_18_1_36_35_263,"[	JPEGDEC_Init]")
TRACE_MSG(IMG_JPEG_DECODER_1216_112_2_18_1_36_35_264,"[	JPEGDEC_CreateHandle +]")
TRACE_MSG(IMG_JPEG_DECODER_1222_112_2_18_1_36_35_265,"[JPGDEC_CreateHandle] Create handle failed")
TRACE_MSG(IMG_JPEG_DECODER_1237_112_2_18_1_36_35_266,"[	JPEGDEC_CreateHandle -] handle = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1259_112_2_18_1_36_35_267,"[	JPEGDEC_GetFrame +] handle = 0x%x, callback = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1263_112_2_18_1_36_35_268,"[JPGDEC_GetFrame] invalid handle")
TRACE_MSG(IMG_JPEG_DECODER_1305_112_2_18_1_36_36_269,"[	JPEGDEC_GetFrame -] handle = 0x%x, callback = 0x%x")
TRACE_MSG(IMG_JPEG_DECODER_1320_112_2_18_1_36_36_270,"[	JPEGDEC_DestoryHandle +] handle = 0x%x, callback = 0x%x, exit_type = %d")
TRACE_MSG(IMG_JPEG_DECODER_1344_112_2_18_1_36_36_271,"[	JPEGDEC_DestoryHandle -] handle = 0x%x, callback = 0x%x, exit_type = %d")
TRACE_MSG(IMG_ALG_COMMON_226_112_2_18_1_36_37_272,"IMG_PROC: _IMGPROC_GetChnLen format 0x%x")
TRACE_MSG(IMG_ALG_COMMON_265_112_2_18_1_36_37_273,"IMG_PROC: _IMGPROC_GetChnLen format 0x%x")
TRACE_MSG(IMG_ALG_FRAME_1225_112_2_18_1_36_43_274,"[SimpleFrmInit] the line is just a point")
TRACE_MSG(IMG_ALG_FRAME_1266_112_2_18_1_36_43_275,"[SubFrame_Alloc] SubFrame Malloc Failed")
TRACE_MSG(IMG_ALG_FRAME_1306_112_2_18_1_36_43_276,"[MaskFrameAlloc] MaskFrame Malloc Failed")
TRACE_MSG(IMG_ALG_FRAME_1360_112_2_18_1_36_43_277,"[SimpleFrame Alloc] SimpleFrame Malloc Failed")
TRACE_MSG(IMG_ALG_FRAME_1806_112_2_18_1_36_44_278,"[Frame_Blend_Mask] _BlendMask's input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3002_112_2_18_1_36_46_279,"[Frame_Blend_Simple_Create] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3010_112_2_18_1_36_46_280,"[Frame_Blend_Simple_Create] this image data format is not support")
TRACE_MSG(IMG_ALG_FRAME_3082_112_2_18_1_36_47_281,"[Frame_Blend_Simple_Blend] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3087_112_2_18_1_36_47_282,"[Frame_Blend_Simple_Blend] image's width must be even")
TRACE_MSG(IMG_ALG_FRAME_3157_112_2_18_1_36_47_283,"[Frame_Blend_Simple_Blend] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3168_112_2_18_1_36_47_284,"[Frame_Blend_Simple_Blend] image's width must be even")
TRACE_MSG(IMG_ALG_FRAME_3296_112_2_18_1_36_47_285,"[Frame_Blend_Simple_Free] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3329_112_2_18_1_36_47_286,"[Frame_Blend_Mask_Create] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3338_112_2_18_1_36_47_287,"[Frame_Blend_Mask_Create] this image data format is not support")
TRACE_MSG(IMG_ALG_FRAME_3359_112_2_18_1_36_47_288,"[Frame_Blend_Mask] mask source address does not align")
TRACE_MSG(IMG_ALG_FRAME_3406_112_2_18_1_36_47_289,"[Frame_Blend_Mask_Create: Mask Frame Rotate] Temp Bufs are not enough")
TRACE_MSG(IMG_ALG_FRAME_3463_112_2_18_1_36_47_290,"Frame_MaskCreate: Scale temp buf failed")
TRACE_MSG(IMG_ALG_FRAME_3496_112_2_18_1_36_47_291,"[Frame_Blend_Mask] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3502_112_2_18_1_36_47_292,"[Frame_Blend_Mask] image's width must be even")
TRACE_MSG(IMG_ALG_FRAME_3562_112_2_18_1_36_48_293,"[Frame_Blend_Mask_Create: Mask Frame Scale] Temp Bufs are not enough")
TRACE_MSG(IMG_ALG_FRAME_3575_112_2_18_1_36_48_294,"[Frame_Blend_Mask] scale failed")
TRACE_MSG(IMG_ALG_FRAME_3597_112_2_18_1_36_48_295,"[Frame_Blend_Mask] _BlendMask failed")
TRACE_MSG(IMG_ALG_FRAME_3633_112_2_18_1_36_48_296,"[Frame_MaskBlendForPreview]: input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3644_112_2_18_1_36_48_297,"[Frame_MaskBlendForPreview]: image's width must be even")
TRACE_MSG(IMG_ALG_FRAME_3714_112_2_18_1_36_48_298,"[Frame_MaskBlendForPreview]: scale failed")
TRACE_MSG(IMG_ALG_FRAME_3776_112_2_18_1_36_48_299,"[Frame_Blend_Mask_Create] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3784_112_2_18_1_36_48_300,"[Frame_Blend_Mask_Create] this image data format is not support")
TRACE_MSG(IMG_ALG_FRAME_3834_112_2_18_1_36_48_301,"[Frame_Blend_Mixed_Blend] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3840_112_2_18_1_36_48_302,"[Frame_Blend_Mixed_Blend] image's width must be even")
TRACE_MSG(IMG_ALG_FRAME_3918_112_2_18_1_36_48_303,"[Frame_Blend_Mixed_Blend] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_3929_112_2_18_1_36_48_304,"[Frame_Blend_Mixed_Blend] image's width must be even")
TRACE_MSG(IMG_ALG_FRAME_4007_112_2_18_1_36_48_305,"[Frame_Blend_Mixed_Free] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_4057_112_2_18_1_36_49_306,"[Frame_HandleCreate] Simple Frame create resource failed")
TRACE_MSG(IMG_ALG_FRAME_4067_112_2_18_1_36_49_307,"[Frame_HandleCreate] Mask create resource failed")
TRACE_MSG(IMG_ALG_FRAME_4077_112_2_18_1_36_49_308,"[Frame_HandleCreate] MixedFrame create resource failed")
TRACE_MSG(IMG_ALG_FRAME_4108_112_2_18_1_36_49_309,"[Frame_Blend] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_4124_112_2_18_1_36_49_310,"[Frame_Blend] Simple Blend failed")
TRACE_MSG(IMG_ALG_FRAME_4134_112_2_18_1_36_49_311,"[Frame_Blend] Mask Frame Blend failed")
TRACE_MSG(IMG_ALG_FRAME_4144_112_2_18_1_36_49_312,"[Frame_Blend] Mixed Frame Blend failed")
TRACE_MSG(IMG_ALG_FRAME_4162_112_2_18_1_36_49_313,"Preview is not support slice mode ")
TRACE_MSG(IMG_ALG_FRAME_4173_112_2_18_1_36_49_314,"[Frame_Blend] Simple Blend failed")
TRACE_MSG(IMG_ALG_FRAME_4183_112_2_18_1_36_49_315,"[Frame_Blend] Mask Frame Blend failed")
TRACE_MSG(IMG_ALG_FRAME_4193_112_2_18_1_36_49_316,"[Frame_Blend] Mixed Frame Blend failed")
TRACE_MSG(IMG_ALG_FRAME_4227_112_2_18_1_36_49_317,"[Frame_HandleDestroy] input parameter is invalidate")
TRACE_MSG(IMG_ALG_FRAME_4238_112_2_18_1_36_49_318,"[Frame_HandleDestroy] Simple destroy the resources failed")
TRACE_MSG(IMG_ALG_FRAME_4248_112_2_18_1_36_49_319,"[Frame_HandleDestroy] Mask frame destroy the resources failed")
TRACE_MSG(IMG_ALG_FRAME_4258_112_2_18_1_36_49_320,"[Frame_HandleDestroy] Mixed frame destroy the resources failed")
TRACE_MSG(IMG_ALG_FRAME_4477_112_2_18_1_36_49_321,"IMG_PROC: _ImgFrame_Start context null")
TRACE_MSG(IMG_ALG_FRAME_4483_112_2_18_1_36_49_322,"IMG_PROC: _ImgFrame_Start input param null")
TRACE_MSG(IMG_ALG_FRAME_4510_112_2_18_1_36_49_323,"IMG_PROC: _ImgFrame_Start calling mode error")
TRACE_MSG(IMG_ALG_PENCILSKETCH_598_112_2_18_1_36_51_324,"IMG_PROC: _pencilsketch: the src img addr 0x%x, img size w %d, h %d, format=%d")
TRACE_MSG(IMG_ALG_PENCILSKETCH_612_112_2_18_1_36_51_325,"PencialSketch : dst format: %d")
TRACE_MSG(IMG_ALG_PENCILSKETCH_613_112_2_18_1_36_51_326,"PencialSketch: src:%d,dst:%d")
TRACE_MSG(IMG_PROC_408_112_2_18_1_36_53_327,"IMG_PROC: event _IMGPROC_SetEvent ")
TRACE_MSG(IMG_PROC_432_112_2_18_1_36_53_328,"IMG_PROC: event _IMGPROC_GetEvent ")
TRACE_MSG(IMG_PROC_538_112_2_18_1_36_53_329,"IMG_PROC: _IMGPROC_GetProcThreadState cur_handle_ptr null, %d line error")
TRACE_MSG(IMG_PROC_559_112_2_18_1_36_53_330,"IMG_PROC: _IMGPROC_AllocBlock cur_handle_ptr null, %d line error")
TRACE_MSG(IMG_PROC_605_112_2_18_1_36_53_331,"IMG_PROC: _IMGPROC_FreeBlock del buf num, %d line error")
TRACE_MSG(IMG_PROC_861_112_2_18_1_36_54_332,"IMG_PROC: _IMGPROC_GetAlgCtrlFun alg exp info null, %d line error")
TRACE_MSG(IMG_PROC_866_112_2_18_1_36_54_333,"IMG_PROC: _IMGPROC_GetAlgCtrlFun alg fun no exit, %d line error")
TRACE_MSG(IMG_PROC_1020_112_2_18_1_36_54_334,"IMG_PROC: _IMGPROC_HandleInit is init")
TRACE_MSG(IMG_PROC_1077_112_2_18_1_36_54_335,"IMG_PROC: _IMGPROC_CreateHandle alloc handle fail %d line error")
TRACE_MSG(IMG_PROC_1656_112_2_18_1_36_55_336,"IMG_PROC: IMGPROC_DestroyHandle handle 0x%d is invalid")
TRACE_MSG(IMG_PROC_1694_112_2_18_1_36_55_337,"IMG_PROC: _IMGPROC_AllHandleDestroy handle no exit")
TRACE_MSG(IMG_PROC_1967_112_2_18_1_36_56_338,"IMG_PROC: _IMGPROC_SetProcessParam handle 0x%x")
TRACE_MSG(IMG_PROC_2109_112_2_18_1_36_56_339,"IMG_PROC: _IMGPROC_AlgInit handle 0x%x")
TRACE_MSG(IMG_PROC_2272_112_2_18_1_36_57_340,"IMG_PROC: _IMGPROC_ViewBeforeProcess handle 0x%x")
TRACE_MSG(IMG_PROC_2316_112_2_18_1_36_57_341,"IMG_PROC: _IMGPROC_ViewBeforeProcess handle 0x%x")
TRACE_MSG(IMG_PROC_2361_112_2_18_1_36_57_342,"IMG_PROC: _IMGPROC_ViewAfterProcess handle 0x%x")
TRACE_MSG(IMG_PROC_2406_112_2_18_1_36_57_343,"IMG_PROC: _IMGPROC_AlgStart handle 0x%x")
TRACE_MSG(IMG_PROC_2425_112_2_18_1_36_57_344,"IMG_PROC: _IMGPROC_AlgStart proc time :%d ms")
TRACE_MSG(IMG_PROC_2434_112_2_18_1_36_57_345,"IMG_PROC: alg end rtn y_addr:0x%x, uv_addr:0x%x,")
TRACE_MSG(IMG_PROC_2435_112_2_18_1_36_57_346,"IMG_PROC: alg end rtn w:%d, h:%d,")
TRACE_MSG(IMG_PROC_2436_112_2_18_1_36_57_347,"IMG_PROC: alg end rtn format:0x%x,")
TRACE_MSG(IMG_PROC_2478_112_2_18_1_36_57_348,"IMG_PROC: _IMGPROC_AlgDestroy handle 0x%x, force:%d")
TRACE_MSG(IMG_PROC_2504_112_2_18_1_36_57_349,"IMG_PROC: _IMGPROC_CtrlStartProcess handle 0x%x")
TRACE_MSG(IMG_PROC_2560_112_2_18_1_36_57_350,"IMG_PROC: _IMGPROC_ProcStartProcess handle: 0x%x")
TRACE_MSG(IMG_PROC_2575_112_2_18_1_36_57_351,"IMG_PROC: _IMGPROC_ProcStartProcess proc_mode 0x%x ")
TRACE_MSG(IMG_PROC_2577_112_2_18_1_36_57_352,"IMG_PROC: _IMGPROC_ProcStartProcess ViewBefore0")
TRACE_MSG(IMG_PROC_2610_112_2_18_1_36_57_353,"IMG_PROC: _IMGPROC_ProcStartProcess ViewBefore1")
TRACE_MSG(IMG_PROC_2643_112_2_18_1_36_57_354,"IMG_PROC: _IMGPROC_ProcStartProcess AlgStart")
TRACE_MSG(IMG_PROC_2698_112_2_18_1_36_57_355,"IMG_PROC: _IMGPROC_ProcStartProcess ViewAfter")
TRACE_MSG(IMG_PROC_2732_112_2_18_1_36_58_356,"IMG_PROC: _IMGPROC_ProcStartProcess end")
TRACE_MSG(IMG_PROC_2766_112_2_18_1_36_58_357,"IMG_PROC: _IMGPROC_CtrlEndProcess handle, 0x%x syn: 0x%x")
TRACE_MSG(IMG_PROC_2798_112_2_18_1_36_58_358,"IMG_PROC: _IMGPROC_CtrlErrorProcess handle, 0x%x syn: 0x%x")
TRACE_MSG(IMG_PROC_2832_112_2_18_1_36_58_359,"IMG_PROC: _IMGPROC_CtrlOpen")
TRACE_MSG(IMG_PROC_2837_112_2_18_1_36_58_360,"IMG_PROC: IMGPROC_Open param null %d line error ")
TRACE_MSG(IMG_PROC_2861_112_2_18_1_36_58_361,"IMG_PROC: IMGPROC_Open alloc temp directory null %d line error ")
TRACE_MSG(IMG_PROC_2867_112_2_18_1_36_58_362,"IMG_PROC: IMGPROC_Open temp directory null %d line error ")
TRACE_MSG(IMG_PROC_2893_112_2_18_1_36_58_363,"IMG_PROC: _IMGPROC_CtrlCreateHandle")
TRACE_MSG(IMG_PROC_2898_112_2_18_1_36_58_364,"IMG_PROC: _IMGPROC_CreateHandle param null %d line error ")
TRACE_MSG(IMG_PROC_2917_112_2_18_1_36_58_365,"IMG_PROC: _IMGPROC_CtrlDestroyHandle handle: 0x%x")
TRACE_MSG(IMG_PROC_2936_112_2_18_1_36_58_366,"IMG_PROC: _IMGPROC_SendCtrlMsg sig_cmd=0x%x, start %dms")
TRACE_MSG(IMG_PROC_2940_112_2_18_1_36_58_367,"IMG_PROC: _IMGPROC_SendCtrlMsg not thread %dms")
TRACE_MSG(IMG_PROC_2965_112_2_18_1_36_58_368,"IMG_PROC: _IMGPROC_SendCtrlMsg end %dms")
TRACE_MSG(IMG_PROC_3010_112_2_18_1_36_58_369,"IMG_PROC: _IMGPROC_CtrlThreadEntry sig is null %d line error")
TRACE_MSG(IMG_PROC_3023_112_2_18_1_36_58_370,"IMG_PROC: _IMGPROC_CtrlThreadEntry enter: %dms, cmd: %d, Priority:%d, 0x%x")
TRACE_MSG(IMG_PROC_3036_112_2_18_1_36_58_371,"IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlOpen rtn ptr null %d line error")
TRACE_MSG(IMG_PROC_3050_112_2_18_1_36_58_372,"IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlClose rtn ptr null %d line error")
TRACE_MSG(IMG_PROC_3064_112_2_18_1_36_58_373,"IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlCreateHandle rtn ptr null %d line error")
TRACE_MSG(IMG_PROC_3078_112_2_18_1_36_58_374,"IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlDestroyHandle rtn ptr null %d line error")
TRACE_MSG(IMG_PROC_3093_112_2_18_1_36_58_375,"IMG_PROC: _IMGPROC_CtrlThreadEntry StartProcess rtn ptr null %d line error")
TRACE_MSG(IMG_PROC_3105_112_2_18_1_36_58_376,"IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlStartProcess % line error")
TRACE_MSG(IMG_PROC_3114_112_2_18_1_36_58_377,"IMG_PROC: _IMGPROC_CtrlThreadEntry _IMGPROC_CtrlEndProcess % line error")
TRACE_MSG(IMG_PROC_3123_112_2_18_1_36_58_378,"IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlErrorProcess % line error")
TRACE_MSG(IMG_PROC_3129_112_2_18_1_36_58_379,"IMG_PROC: _IMGPROC_CtrlThreadEntry, invalid sig ,line %d")
TRACE_MSG(IMG_PROC_3136_112_2_18_1_36_58_380,"IMG_PROC: _IMGPROC_CtrlThreadEntry end: %dms, 0x%x")
TRACE_MSG(IMG_PROC_3149_112_2_18_1_36_58_381,"IMG_PROC: _IMGPROC_CtrlThreadCreat enter %d")
TRACE_MSG(IMG_PROC_3166_112_2_18_1_36_58_382,"IMG_PROC: _IMGPROC_CtrlThreadCreat, fail %d line error")
TRACE_MSG(IMG_PROC_3173_112_2_18_1_36_58_383,"IMG_PROC: _IMGPROC_CtrlThreadCreat, thread already exist")
TRACE_MSG(IMG_PROC_3176_112_2_18_1_36_58_384,"IMG_PROC: _IMGPROC_CtrlThreadCreat, task_id=0x%x")
TRACE_MSG(IMG_PROC_3192_112_2_18_1_36_58_385,"IMG_PROC: _IMGPROC_SendProcessMsg sig_cmd=0x%x, start %dms")
TRACE_MSG(IMG_PROC_3196_112_2_18_1_36_59_386,"IMG_PROC: _IMGPROC_SendProcessMsg not thread %dms")
TRACE_MSG(IMG_PROC_3221_112_2_18_1_36_59_387,"IMG_PROC: _IMGPROC_SendProcessMsg end %dms")
TRACE_MSG(IMG_PROC_3265_112_2_18_1_36_59_388,"IMG_PROC: _IMGPROC_ProcessThreadEntry sig is null %d line error")
TRACE_MSG(IMG_PROC_3277_112_2_18_1_36_59_389,"IMG_PROC: _IMGPROC_ProcessThreadEntry enter: %dms, cmd: %d, Priority: %d")
TRACE_MSG(IMG_PROC_3294_112_2_18_1_36_59_390,"IMG_PROC: _IMGPROC_CtrlThreadEntry CtrlStartProcess % line error")
TRACE_MSG(IMG_PROC_3300_112_2_18_1_36_59_391,"IMG_PROC: _IMGPROC_ProcessThreadEntry, invalid sig ,line %d")
TRACE_MSG(IMG_PROC_3309_112_2_18_1_36_59_392,"IMG_PROC: _IMGPROC_ProcessThreadEntry end: %dms")
TRACE_MSG(IMG_PROC_3321_112_2_18_1_36_59_393,"IMG_PROC: _IMGPROC_ProcessThreadCreat enter %d")
TRACE_MSG(IMG_PROC_3338_112_2_18_1_36_59_394,"IMG_PROC: _IMGPROC_ProcessThreadCreat, fail %d line error")
TRACE_MSG(IMG_PROC_3344_112_2_18_1_36_59_395,"IMG_PROC: _IMGPROC_ProcessThreadCreat, thread already exist")
TRACE_MSG(IMG_PROC_3347_112_2_18_1_36_59_396,"IMG_PROC: _IMGPROC_ProcessThreadCreat, task_id=0x%x")
TRACE_MSG(IMG_PROC_3358_112_2_18_1_36_59_397,"REFIMG: IMG_ProcessAppTaskClose,task_id=0x%x")
TRACE_MSG(IMG_PROC_3406_112_2_18_1_36_59_398,"IMG_PROC: =R=I= IMGPROC_Init")
TRACE_MSG(IMG_PROC_3442_112_2_18_1_36_59_399,"IMG_PROC: =R=I= IMGPROC_Open cur task: 0x%x ")
TRACE_MSG(IMG_PROC_3476_112_2_18_1_36_59_400,"IMG_PROC: =R=I= IMGPROC_Close")
TRACE_MSG(IMG_PROC_3510_112_2_18_1_36_59_401,"IMG_PROC: =R=I= IMGPROC_CreateHandle")
TRACE_MSG(IMG_PROC_3527_112_2_18_1_36_59_402,"IMG_PROC: IMGPROC_CreateHandle handle: 0x%x")
TRACE_MSG(IMG_PROC_3544_112_2_18_1_36_59_403,"IMG_PROC: =R=I= IMGPROC_DestroyHandle handle: 0x%x")
TRACE_MSG(IMG_PROC_3572_112_2_18_1_36_59_404,"IMG_PROC: src mode:0x%x, num:%d,")
TRACE_MSG(IMG_PROC_3573_112_2_18_1_36_59_405,"IMG_PROC: src y_addr:0x%x, uv_addr:0x%x,")
TRACE_MSG(IMG_PROC_3574_112_2_18_1_36_59_406,"IMG_PROC: src format:0x%x,")
TRACE_MSG(IMG_PROC_3577_112_2_18_1_36_59_407,"IMG_PROC: before 0 y_addr:0x%x, uv_addr:0x%x,")
TRACE_MSG(IMG_PROC_3578_112_2_18_1_36_59_408,"IMG_PROC: before 0 w:%d, h:%d,")
TRACE_MSG(IMG_PROC_3579_112_2_18_1_36_59_409,"IMG_PROC: before 0 format:0x%x,")
TRACE_MSG(IMG_PROC_3583_112_2_18_1_36_59_410,"IMG_PROC: before 1 y_addr:0x%x, uv_addr:0x%x,")
TRACE_MSG(IMG_PROC_3584_112_2_18_1_36_59_411,"IMG_PROC: before 1 w:%d, h:%d,")
TRACE_MSG(IMG_PROC_3585_112_2_18_1_36_59_412,"IMG_PROC: before 1 format:0x%x,")
TRACE_MSG(IMG_PROC_3589_112_2_18_1_36_59_413,"IMG_PROC: proc y_addr:0x%x, uv_addr:0x%x,")
TRACE_MSG(IMG_PROC_3590_112_2_18_1_36_59_414,"IMG_PROC: proc rect x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(IMG_PROC_3591_112_2_18_1_36_59_415,"IMG_PROC: proc w:%d, h:%d,")
TRACE_MSG(IMG_PROC_3592_112_2_18_1_36_59_416,"IMG_PROC: proc format:0x%x,")
TRACE_MSG(IMG_PROC_3596_112_2_18_1_36_59_417,"IMG_PROC: after y_addr:0x%x, uv_addr:0x%x,")
TRACE_MSG(IMG_PROC_3597_112_2_18_1_36_59_418,"IMG_PROC: after w:%d, h:%d,")
TRACE_MSG(IMG_PROC_3598_112_2_18_1_36_59_419,"IMG_PROC: after format:0x%x,")
TRACE_MSG(IMG_PROC_3614_112_2_18_1_36_59_420,"IMG_PROC: =R=I= IMGPROC_DoProcess handle: 0x%x , cmd 0x%x, caller:%d")
TRACE_MSG(IMG_PROC_3618_112_2_18_1_36_59_421,"IMG_PROC: IMGPROC_DoProcess FIRST")
TRACE_MSG(IMG_PROC_3622_112_2_18_1_36_59_422,"IMG_PROC: IMGPROC_DoProcess PROCESS")
TRACE_MSG(IMG_PROC_3626_112_2_18_1_36_59_423,"IMG_PROC: IMGPROC_DoProcess BEFORE0")
TRACE_MSG(IMG_PROC_3630_112_2_18_1_36_59_424,"IMG_PROC: IMGPROC_DoProcess BEFORE")
TRACE_MSG(IMG_PROC_3634_112_2_18_1_36_59_425,"IMG_PROC: IMGPROC_DoProcess AFTER")
TRACE_MSG(IMG_PROC_3638_112_2_18_1_36_59_426,"IMG_PROC: IMGPROC_DoProcess HISTORY")
TRACE_MSG(IMG_PROC_3642_112_2_18_1_36_59_427,"IMG_PROC: IMGPROC_DoProcess UPDAT_ALG")
TRACE_MSG(IMG_PROC_3646_112_2_18_1_36_59_428,"IMG_PROC: IMGPROC_DoProcess UPDAT_SRC")
TRACE_MSG(IMG_PROC_3650_112_2_18_1_36_59_429,"IMG_PROC: IMGPROC_DoProcess UPDAT_DST")
TRACE_MSG(IMG_PROC_3654_112_2_18_1_36_59_430,"IMG_PROC: IMGPROC_DoProcess END")
TRACE_MSG(IMG_PROC_3730_112_2_18_1_37_0_431,"IMG_PROC: =R=I= IMGPROC_DoProcessIcon handle: 0x%x")
TRACE_MSG(IMG_PROC_3858_112_2_18_1_37_0_432,"IMG_PROC: =R=I= IMGPROC_AbortProcess handle: 0x%x")
TRACE_MSG(IMG_PROC_3881_112_2_18_1_37_0_433,"IMG_PROC: =R=I= IMGPROC_GetProcessedFrame handle: 0x%x")
TRACE_MSG(IMG_PROC_3951_112_2_18_1_37_0_434,"IMG_PROC: IMGPROC_SetProcImgDstBuf handle 0x%x")
TRACE_MSG(IMG_PROC_4069_112_2_18_1_37_0_435,"IMG_PROC: IMGPROC_SetProcImgDstFs handle 0x%x")
TRACE_MSG(IMG_PROC_4095_112_2_18_1_37_0_436,"IMG_PROC: IMGPROC_BufEncoder handle 0x%x")
TRACE_MSG(IMG_PROC_4133_112_2_18_1_37_0_437,"IMG_PROC: IMGPROC_BufEncoder y_addr:0x%x, uv_addr:0x%x,")
TRACE_MSG(IMG_PROC_4134_112_2_18_1_37_0_438,"IMG_PROC: IMGPROC_BufEncoder w:%d, h:%d,")
TRACE_MSG(IMG_PROC_4135_112_2_18_1_37_0_439,"IMG_PROC: IMGPROC_BufEncoder format:0x%x,")
TRACE_MSG(IMG_PROC_4137_112_2_18_1_37_0_440,"IMG_PROC: IMGPROC_BufEncoder1 y_addr:0x%x, uv_addr:0x%x,")
TRACE_MSG(IMG_PROC_4138_112_2_18_1_37_0_441,"IMG_PROC: IMGPROC_BufEncoder1 w:%d, h:%d,")
TRACE_MSG(IMG_PROC_4139_112_2_18_1_37_0_442,"IMG_PROC: IMGPROC_BufEncoder1 format:0x%x,")
TRACE_MSG(IMG_PROC_4177_112_2_18_1_37_0_443,"IMG_PROC: IMGPROC_ALLOCA %dk fail %d line error ")
TRACE_MSG(IMG_PROC_ALG_EXPOSAL_149_112_2_18_1_37_1_444,"IMG_PROC: _EXPOSAL_GetContextPtr id zero, %d line error")
TRACE_MSG(IMG_PROC_ALG_EXPOSAL_158_112_2_18_1_37_1_445,"IMG_PROC: _EXPOSAL_GetContextPtr check id %d line error")
TRACE_MSG(IMG_PROC_ALG_HDR_90_112_2_18_1_37_2_446,"IMG_PROC: _HDR_GetContextPtr id zero, %d line error")
TRACE_MSG(IMG_PROC_ALG_HDR_99_112_2_18_1_37_2_447,"IMG_PROC: _HDR_GetContextPtr check id %d line error")
TRACE_MSG(IMG_PROC_ALG_HDR_446_112_2_18_1_37_3_448,"IMG_PROC: _HDR_Start h:%d, complete_line:%d")
TRACE_MSG(IMG_PROC_ALG_INFRARED_400_112_2_18_1_37_4_449,"IMG_PROC: _INFRARED_GetContextPtr id zero, %d line error")
TRACE_MSG(IMG_PROC_ALG_INFRARED_409_112_2_18_1_37_4_450,"IMG_PROC: _INFRARED_GetContextPtr check id %d line error")
TRACE_MSG(IMG_PROC_ALG_N_IN_ONE_198_112_2_18_1_37_5_451,"IMG_PROC: _NInOne_GetContextNum num zero, %d line error")
TRACE_MSG(IMG_PROC_ALG_N_IN_ONE_206_112_2_18_1_37_5_452,"IMG_PROC: _NInOne_GetContextNum context head no exit, %d line error")
TRACE_MSG(IMG_PROC_ALG_N_IN_ONE_222_112_2_18_1_37_5_453,"IMG_PROC: _NInOne_GetContextNum num no exit, %d line error")
TRACE_MSG(IMG_PROC_ALG_NEGATIVE_141_112_2_18_1_37_7_454,"IMG_PROC: _NEGATIVE_GetContextPtr id zero, %d line error")
TRACE_MSG(IMG_PROC_ALG_NEGATIVE_150_112_2_18_1_37_7_455,"IMG_PROC: _NEGATIVE_GetContextPtr check id %d line error")
TRACE_MSG(IMG_PROC_ALG_XRAY_141_112_2_18_1_37_8_456,"IMG_PROC: _XRAY_GetContextPtr id zero, %d line error")
TRACE_MSG(IMG_PROC_ALG_XRAY_150_112_2_18_1_37_8_457,"IMG_PROC: _XRAY_GetContextPtr check id %d line error")
TRACE_MSG(IMG_PROC_FACE_DECT_1958_112_2_18_1_37_13_458,"IMG_PROC: _FaceDect_DelContextNum, chn0: 0x%x")
TRACE_MSG(IMG_PROC_FACE_DECT_1959_112_2_18_1_37_13_459,"IMG_PROC: _FaceDect_DelContextNum, chn1: 0x%x")
TRACE_MSG(IMG_PROC_FACE_DECT_1960_112_2_18_1_37_13_460,"IMG_PROC: _FaceDect_DelContextNum, sw_buf_ptr: 0x%x")
TRACE_MSG(IMG_PROC_FACE_DECT_1984_112_2_18_1_37_13_461,"IMG_PROC: _FaceDect_GetContextNum num zero, %d line error")
TRACE_MSG(IMG_PROC_FACE_DECT_1992_112_2_18_1_37_13_462,"IMG_PROC: _FaceDect_GetContextNum context head no exit, %d line error")
TRACE_MSG(IMG_PROC_FACE_DECT_2008_112_2_18_1_37_13_463,"IMG_PROC: _FaceDect_GetContextNum num no exit, %d line error")
TRACE_MSG(IMG_PROC_FACE_DECT_2029_112_2_18_1_37_13_464,"IMG_PROC: _FaceDect_SetInitParam id: 0x%x")
TRACE_MSG(IMG_PROC_FACE_DECT_2192_112_2_18_1_37_13_465,"IMG_PROC: _FaceDect_Init ")
TRACE_MSG(IMG_PROC_FACE_DECT_2232_112_2_18_1_37_13_466,"IMG_PROC: _FaceDect_Update ")
TRACE_MSG(IMG_PROC_FACE_DECT_2325_112_2_18_1_37_14_467,"IMG_PROC: _FaceDect_Start ")
TRACE_MSG(IMG_PROC_FACE_DECT_2356_112_2_18_1_37_14_468,"IMG_PROC: IMG_FaceDection, y:0x%x, uv:0x%x")
TRACE_MSG(IMG_PROC_FACE_DECT_2357_112_2_18_1_37_14_469,"IMG_PROC: IMG_FaceDection, w:%d, h:%d")
TRACE_MSG(IMG_PROC_FACE_DECT_2358_112_2_18_1_37_14_470,"IMG_PROC: IMG_FaceDection, x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(IMG_PROC_FACE_DECT_2364_112_2_18_1_37_14_471,"IMG_PROC: IMG_FaceDection num ------%d---------")
TRACE_MSG(IMG_PROC_FACE_DECT_2367_112_2_18_1_37_14_472,"IMG_PROC: IMG_FaceDection ------%d---------")
TRACE_MSG(IMG_PROC_FACE_DECT_2368_112_2_18_1_37_14_473,"IMG_PROC: IMG_FaceDection rect, x:%d, y:%d, w:%d, h:%d")
TRACE_MSG(IMG_PROC_FACE_DECT_2369_112_2_18_1_37_14_474,"IMG_PROC: IMG_FaceDection simle:%d")
TRACE_MSG(IMG_PROC_FACE_DECT_2388_112_2_18_1_37_14_475,"IMG_PROC: _FaceDect_Start 0x%x end")
TRACE_MSG(IMG_PROC_FACE_DECT_2399_112_2_18_1_37_14_476,"IMG_PROC: _FaceDect_Destroy id: 0x%x")
TRACE_MSG(RGB_YUV_CONVERT_69_112_2_18_1_37_16_477,"YUVConvertJPG malloc is fail!---")
TRACE_MSG(RGB_YUV_CONVERT_81_112_2_18_1_37_16_478,"[pe_process.c] SaveJpeg successed! ")
TRACE_MSG(SCALE_HARDWARE_169_112_2_18_1_37_16_479,"[_OpenIspService] lock isp service failed")
TRACE_MSG(SCALE_HARDWARE_179_112_2_18_1_37_16_480,"[_OpenIspService] open isp failed")
TRACE_MSG(SCALE_HARDWARE_358_112_2_18_1_37_17_481,"[_ScaleSetSliceBuf] allocate slice in buffer, chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(SCALE_HARDWARE_402_112_2_18_1_37_17_482,"[_ScaleSetSliceBuf] allocate slice out buffer, chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(SCALE_HARDWARE_431_112_2_18_1_37_17_483,"[_ReadSlice] src chn (0x%x, 0x%x), dst chn (0x%x, 0x%x), src size (%d, %d)")
TRACE_MSG(SCALE_HARDWARE_435_112_2_18_1_37_17_484,"[_ReadSlice] src line chn size (%d, %d), dst size (%d, %d)")
TRACE_MSG(SCALE_HARDWARE_447_112_2_18_1_37_17_485,"[_ReadSlice] yuv420 format, src chn size = %d, dst chn size = %d, lines = %d")
TRACE_MSG(SCALE_HARDWARE_492_112_2_18_1_37_17_486,"[_ReadSlice] read_lines = %d, chn size = %d, slice_bytes = %d")
TRACE_MSG(SCALE_HARDWARE_510_112_2_18_1_37_17_487,"[_ReadSlice] update src chn (0x%x, 0x%x)")
TRACE_MSG(SCALE_HARDWARE_539_112_2_18_1_37_17_488,"[_WriteSlice] src chn (0x%x, 0x%x), dst chn (0x%x, 0x%x), src w = %d, dst w = %d")
TRACE_MSG(SCALE_HARDWARE_628_112_2_18_1_37_17_489,"[_SetIspParam] isp param: output buf size = %d")
TRACE_MSG(SCALE_HARDWARE_633_112_2_18_1_37_17_490,"[_SetIspParam] isp param: src size (%d, %d), src rect (%d, %d, %d, %d), src format = %d")
TRACE_MSG(SCALE_HARDWARE_637_112_2_18_1_37_17_491,"[_SetIspParam] isp param: trg size (%d, %d), trg format = %d")
TRACE_MSG(SCALE_HARDWARE_643_112_2_18_1_37_17_492,"[_SetIspParam] isp param: src addr (0x%x, 0x%x), trg addr = (0x%x, 0x%x)")
TRACE_MSG(SCALE_HARDWARE_675_112_2_18_1_37_17_493,"[_SetScaleIspBuf] allocate isp buffer failed")
TRACE_MSG(SCALE_HARDWARE_692_112_2_18_1_37_17_494,"[_SetScaleIspBuf] line buf (0x%x, %d), swap buf (0x%x, %d)")
TRACE_MSG(SCALE_HARDWARE_714_112_2_18_1_37_17_495,"[_ScaleDone] output addr (0x%x, 0x%x), size (%d, %d)")
TRACE_MSG(SCALE_HARDWARE_719_112_2_18_1_37_18_496,"[_ScaleDone] scale failed, NULL addr")
TRACE_MSG(SCALE_HARDWARE_727_112_2_18_1_37_18_497,"[_ScaleDone] scale failed, unenough output lines")
TRACE_MSG(SCALE_HARDWARE_737_112_2_18_1_37_18_498,"[_ScaleDone] scale failed, unenough output lines")
TRACE_MSG(SCALE_HARDWARE_784_112_2_18_1_37_18_499,"[_SliceScaleDone] output addr (0x%x, 0x%x), size (%d, %d)")
TRACE_MSG(SCALE_HARDWARE_789_112_2_18_1_37_18_500,"[_SliceScaleDone] scale failed, y addr is NULL")
TRACE_MSG(SCALE_HARDWARE_797_112_2_18_1_37_18_501,"[_SliceScaleDone] scale failed, uv addr is NULL")
TRACE_MSG(SCALE_HARDWARE_805_112_2_18_1_37_18_502,"[_SliceScaleDone] scale failed, unenough output lines")
TRACE_MSG(SCALE_HARDWARE_815_112_2_18_1_37_18_503,"[_SliceScaleDone] scale failed, unenough output lines")
TRACE_MSG(SCALE_HARDWARE_834_112_2_18_1_37_18_504,"[_SliceScaleDone] scale out size (%d, %d), total size = %d")
TRACE_MSG(SCALE_HARDWARE_870_112_2_18_1_37_18_505,"[_IsNormalMode] input size - width is not 4 aligned !")
TRACE_MSG(SCALE_HARDWARE_877_112_2_18_1_37_18_506,"[_IsNormalMode] input chn0 ptr is not 4 aligned !")
TRACE_MSG(SCALE_HARDWARE_884_112_2_18_1_37_18_507,"[_IsNormalMode] input chn0 size is not 32 aligned !")
TRACE_MSG(SCALE_HARDWARE_892_112_2_18_1_37_18_508,"[_IsNormalMode] input chn1 ptr is not 4 aligned !")
TRACE_MSG(SCALE_HARDWARE_900_112_2_18_1_37_18_509,"[_IsNormalMode] input chn1 size is not 32 aligned !")
TRACE_MSG(SCALE_HARDWARE_907_112_2_18_1_37_18_510,"[_IsNormalMode] output chn0 ptr is not 4 aligned !")
TRACE_MSG(SCALE_HARDWARE_914_112_2_18_1_37_18_511,"[_IsNormalMode] output chn0 size is not 32 aligned !")
TRACE_MSG(SCALE_HARDWARE_922_112_2_18_1_37_18_512,"[_IsNormalMode] output chn1 ptr is not 4 aligned !")
TRACE_MSG(SCALE_HARDWARE_930_112_2_18_1_37_18_513,"[_IsNormalMode] output chn1 size is not 32 aligned !")
TRACE_MSG(SCALE_HARDWARE_974_112_2_18_1_37_18_514,"[_SetScaleSliceHeight] slice height = %d")
TRACE_MSG(SCALE_HARDWARE_1018_112_2_18_1_37_18_515,"[_ScaleSliceMode]")
TRACE_MSG(SCALE_HARDWARE_1023_112_2_18_1_37_18_516,"[_ScaleSliceMode] _SetIspParam failed")
TRACE_MSG(SCALE_HARDWARE_1036_112_2_18_1_37_18_517,"[_ScaleNormalMode] async_param_ptr = 0x%x, callback = 0x%x, param = 0x%x")
TRACE_MSG(SCALE_HARDWARE_1048_112_2_18_1_37_18_518,"[_ScaleSliceMode] _SetScaleIspBuf failed")
TRACE_MSG(SCALE_HARDWARE_1056_112_2_18_1_37_18_519,"[_ScaleSliceMode] _ScaleSetSliceBuf failed")
TRACE_MSG(SCALE_HARDWARE_1080_112_2_18_1_37_18_520,"[_ScaleSliceMode] src size (%d, %d), src chn (0x%x, 0x%x), src line size (%d, %d)")
TRACE_MSG(SCALE_HARDWARE_1083_112_2_18_1_37_18_521,"[_ScaleSliceMode] src skip lines = %d, src total lines = %d")
TRACE_MSG(SCALE_HARDWARE_1095_112_2_18_1_37_18_522,"[_ScaleSliceMode] slice in size (%d, %d), src in chn (0x%x, 0x%x), src in line size (%d, %d)")
TRACE_MSG(SCALE_HARDWARE_1102_112_2_18_1_37_18_523,"[_ScaleSliceMode] dst size (%d, %d), dst chn (0x%x, 0x%x)")
TRACE_MSG(SCALE_HARDWARE_1114_112_2_18_1_37_18_524,"[_ScaleSliceMode] lase sline height = %d")
TRACE_MSG(SCALE_HARDWARE_1122_112_2_18_1_37_18_525,"[_ScaleSliceMode] _ReadSlice failed !")
TRACE_MSG(SCALE_HARDWARE_1129_112_2_18_1_37_18_526,"[_ScaleSliceMode] ISP_ServiceSetScaleParam failed !")
TRACE_MSG(SCALE_HARDWARE_1143_112_2_18_1_37_18_527,"[_ScaleSliceMode] ISP_ServiceStartScale failed !")
TRACE_MSG(SCALE_HARDWARE_1156_112_2_18_1_37_18_528,"[_ScaleSliceMode] slice output lines overflow %d, reset to 0x%x")
TRACE_MSG(SCALE_HARDWARE_1164_112_2_18_1_37_18_529,"[_ScaleSliceMode] _WriteSlice failed !")
TRACE_MSG(SCALE_HARDWARE_1177_112_2_18_1_37_18_530,"[_ScaleSliceMode] scale output done scale out lines = %d, total write lines = %d")
TRACE_MSG(SCALE_HARDWARE_1211_112_2_18_1_37_19_531,"[_ScaleNormalMode]")
TRACE_MSG(SCALE_HARDWARE_1229_112_2_18_1_37_19_532,"[_ScaleNormalMode] async_param_ptr = 0x%x, callback = 0x%x, param = 0x%x")
TRACE_MSG(SCALE_HARDWARE_1269_112_2_18_1_37_19_533,"[_FreeBuffer] isp buf = 0x%x, out slice buf = 0x%x, in slice buf = 0x%x")
TRACE_MSG(SCALE_HARDWARE_1364_112_2_18_1_37_19_534,"[_CheckScaleSize] trim rect size is valid")
TRACE_MSG(SCALE_HARDWARE_1372_112_2_18_1_37_19_535,"[_CheckScaleSize] trim rect size is valid")
TRACE_MSG(SCALE_HARDWARE_1378_112_2_18_1_37_19_536,"[_CheckScaleSize] trim rect size is valid")
TRACE_MSG(SCALE_HARDWARE_1395_112_2_18_1_37_19_537,"[_CheckScaleSize] scale mode is SCALE_MODE_NO_DISTORT, reset the target size")
TRACE_MSG(SCALE_HARDWARE_1400_112_2_18_1_37_19_538,"[_CheckScaleSize] the size is out of range to do SCALE_MODE_DISTORT scale")
TRACE_MSG(SCALE_HARDWARE_1470_112_2_18_1_37_19_539,"[_IsHardwareSupport] src size (%d, %d), src rect (%d, %d, %d, %d), target size (%d, %d)")
TRACE_MSG(SCALE_HARDWARE_1524_112_2_18_1_37_19_540,"[_CheckScaleSize] trim rect size is valid")
TRACE_MSG(SCALE_HARDWARE_1541_112_2_18_1_37_19_541,"[_CheckScaleSize] scale mode is SCALE_MODE_NO_DISTORT, reset the target size")
TRACE_MSG(SCALE_HARDWARE_1546_112_2_18_1_37_19_542,"[_CheckScaleSize] the size is out of range to do SCALE_MODE_DISTORT scale")
TRACE_MSG(SCALE_HARDWARE_1619_112_2_18_1_37_19_543,"[_SetScaleSize] src size (%d, %d), src rect (%d, %d, %d, %d), target size (%d, %d)")
TRACE_MSG(SCALE_HARDWARE_1644_112_2_18_1_37_19_544,"[_ScaleImage] src chn0 (0x%x, %d), src chn1 (0x%x, %d), dst chn0 (0x%x, %d), dst chn1 (0x%x, %d)")
TRACE_MSG(SCALE_HARDWARE_1689_112_2_18_1_37_20_545,"[_ScaleImage] hardware scale unsupport")
TRACE_MSG(SCALE_HARDWARE_1704_112_2_18_1_37_20_546,"[_ScaleImage] hardware scale unsupport")
TRACE_MSG(SCALE_HARDWARE_1722_112_2_18_1_37_20_547,"[_ScaleImage] hardware scale unsupport")
TRACE_MSG(SCALE_IMAGE_105_112_2_18_1_37_20_548,"[_IsScaleBySoftware] change to software scale")
TRACE_MSG(SCALE_IMAGE_486_112_2_18_1_37_21_549,"[_ScaleImageARGB888] src_alpha_buf (0x%x, %d, %d), dst_alpha_buf (0x%x, %d, %d), dst_rgb_buf (0x%x, %d, %d)")
TRACE_MSG(SCALE_IMAGE_493_112_2_18_1_37_21_550,"[_ScaleImageARGB888] allocate alpha buffer failed")
TRACE_MSG(SCALE_IMAGE_511_112_2_18_1_37_21_551,"[_ScaleImageARGB888] get alpha time = %d")
TRACE_MSG(SCALE_IMAGE_534_112_2_18_1_37_21_552,"[_ScaleImageARGB888] _ScaleImage rgb888 failed")
TRACE_MSG(SCALE_IMAGE_540_112_2_18_1_37_21_553,"[_ScaleImageARGB888] scale alpha time = %d")
TRACE_MSG(SCALE_IMAGE_544_112_2_18_1_37_21_554,"[_ScaleImageARGB888] scale alpha done, out size (%d, %d), out addr (0x%x, %d), format (%d)")
TRACE_MSG(SCALE_IMAGE_567_112_2_18_1_37_21_555,"[_ScaleImageARGB888] _ScaleImage rgb888 failed")
TRACE_MSG(SCALE_IMAGE_573_112_2_18_1_37_21_556,"[_ScaleImageARGB888] scale rgb time = %d")
TRACE_MSG(SCALE_IMAGE_577_112_2_18_1_37_21_557,"[_ScaleImageARGB888] scale rgb done, out size (%d, %d), out addr (0x%x, %d), format (%d)")
TRACE_MSG(SCALE_IMAGE_588_112_2_18_1_37_21_558,"[_ScaleImageARGB888] merg alpha and rgb time = %d")
TRACE_MSG(SCALE_IMAGE_625_112_2_18_1_37_21_559,"[_ScaleImageARGB565] get alpha time = %d")
TRACE_MSG(SCALE_IMAGE_643_112_2_18_1_37_21_560,"[_ScaleImageARGB565] _ScaleImage rgb888 failed")
TRACE_MSG(SCALE_IMAGE_649_112_2_18_1_37_21_561,"[_ScaleImageARGB565] scale alpha time = %d")
TRACE_MSG(SCALE_IMAGE_653_112_2_18_1_37_21_562,"[_ScaleImageARGB565] scale alpha done, out size (%d, %d), out addr (0x%x, %d), format (%d)")
TRACE_MSG(SCALE_IMAGE_669_112_2_18_1_37_21_563,"[_ScaleImageARGB565] _ScaleImage rgb888 failed")
TRACE_MSG(SCALE_IMAGE_675_112_2_18_1_37_21_564,"[_ScaleImageARGB565] scale rgb time = %d")
TRACE_MSG(SCALE_IMAGE_679_112_2_18_1_37_21_565,"[_ScaleImageARGB565] scale rgb done, out size (%d, %d), out addr (0x%x, %d), format (%d)")
TRACE_MSG(SCALE_IMAGE_720_112_2_18_1_37_21_566,"IMGPROC_SCALE: _ScaleEnterCritical")
TRACE_MSG(SCALE_IMAGE_735_112_2_18_1_37_22_567,"IMGPROC_SCALE: _ScaleExitCritical")
TRACE_MSG(SCALE_IMAGE_892_112_2_18_1_37_22_568,"[_CheckScaleOutputChn] output buffer is enough for align")
TRACE_MSG(SCALE_IMAGE_901_112_2_18_1_37_22_569,"[_CheckScaleOutputChn] output buffer is not enough for align")
TRACE_MSG(SCALE_IMAGE_923_112_2_18_1_37_22_570,"[_SetScaleSize] output format = %d, dst chn0 (0x%x, %d), dst chn1 (0x%x, %d)")
TRACE_MSG(SCALE_IMAGE_943_112_2_18_1_37_22_571,"[_SetScaleParam] _SetScaleInputChn failed")
TRACE_MSG(SCALE_IMAGE_950_112_2_18_1_37_22_572,"[_SetScaleParam] _SetScaleOutputChn failed")
TRACE_MSG(SCALE_IMAGE_961_112_2_18_1_37_22_573,"[_SetScaleParam] scale function invalid")
TRACE_MSG(SCALE_IMAGE_975_112_2_18_1_37_22_574,"[_SetScaleParam] scale function invalid")
TRACE_MSG(SCALE_IMAGE_981_112_2_18_1_37_22_575,"[_SetScaleParam] scale not support")
TRACE_MSG(SCALE_IMAGE_987_112_2_18_1_37_22_576,"[_SetScaleParam] _SetScaleSize failed")
TRACE_MSG(SCALE_IMAGE_991_112_2_18_1_37_22_577,"[_SetScaleParam] scale_by_software = %d")
TRACE_MSG(SCALE_IMAGE_1084_112_2_18_1_37_22_578,"[IMGPROC_SCALE: _DoScale] output buffer is invalid")
TRACE_MSG(SCALE_IMAGE_1091_112_2_18_1_37_22_579,"[IMGPROC_SCALE: _DoScale] get scale func failed")
TRACE_MSG(SCALE_IMAGE_1108_112_2_18_1_37_22_580,"[IMGPROC_SCALE: _DoScale] ret = %d, output size (%d, %d), output format (%d)")
TRACE_MSG(SCALE_IMAGE_1148_112_2_18_1_37_22_581,"[IMGPROC_SCALE: _SetScaleEvent] %d")
TRACE_MSG(SCALE_IMAGE_1162_112_2_18_1_37_22_582,"[IMGPROC_SCALE: _GetScaleEvent] + , %d")
TRACE_MSG(SCALE_IMAGE_1171_112_2_18_1_37_22_583,"[IMGPROC_SCALE: _GetScaleEvent] -, actual event = %d, status = %d")
TRACE_MSG(SCALE_IMAGE_1184_112_2_18_1_37_22_584,"[IMGPROC_SCALE: _PostScaleStart] scale_cxt_ptr = 0x%x")
TRACE_MSG(SCALE_IMAGE_1190_112_2_18_1_37_22_585,"[IMGPROC_SCALE: _PostScaleStart] is called = %d")
TRACE_MSG(SCALE_IMAGE_1218_112_2_18_1_37_22_586,"[IMGPROC_SCALE: _TaskEntry], sig=0x%x,sig_code = %d")
TRACE_MSG(SCALE_IMAGE_1237_112_2_18_1_37_23_587,"[IMGPROC_SCALE: _TaskEntry] is_async = %d, context_ptr = 0x%x")
TRACE_MSG(SCALE_IMAGE_1258_112_2_18_1_37_23_588,"[_TaskEntry] invalid signal")
TRACE_MSG(SCALE_IMAGE_1263_112_2_18_1_37_23_589,"[IMGPROC_SCALE: _TaskEntry] scale done, is_async = %d, cxt_ptr = 0x%x")
TRACE_MSG(SCALE_IMAGE_1332_112_2_18_1_37_23_590,"[IMGPROC_SCALE: _OpenTask] create task, s_task_id = 0x%x, priority = %d")
TRACE_MSG(SCALE_IMAGE_1343_112_2_18_1_37_23_591,"[IMGPROC_SCALE: _OpenTask] change task priority from %d to %d")
TRACE_MSG(SCALE_IMAGE_1359_112_2_18_1_37_23_592,"[IMGPROC_SCALE: _CloseTask] s_task_id = 0x%x")
TRACE_MSG(SCALE_IMAGE_1391_112_2_18_1_37_23_593,"[_SuspendTask] s_task_id = 0x%x")
TRACE_MSG(SCALE_IMAGE_1415_112_2_18_1_37_23_594,"[IMGPROC_SCALE: _SendMsg], sig_code = %d")
TRACE_MSG(SCALE_IMAGE_1510_112_2_18_1_37_23_595,"[IMGPROC_SCALE: _CopySrcImage] src buf chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(SCALE_IMAGE_1512_112_2_18_1_37_23_596,"[IMGPROC_SCALE: _CopySrcImage] src data chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(SCALE_IMAGE_1543_112_2_18_1_37_23_597,"[IMGPROC_SCALE: _StartScale] enter, is_async = %d, cxt_ptr = 0x%x")
TRACE_MSG(SCALE_IMAGE_1581_112_2_18_1_37_23_598,"[IMGPROC_SCALE: _StartScale] exit, is_async = %d, ret_value = %d")
TRACE_MSG(SCALE_IMAGE_1595_112_2_18_1_37_23_599,"[scale_image: _ScaleArgbCallBack], the output addr 0x%x, width %d, height %d")
TRACE_MSG(SCALE_IMAGE_1645_112_2_18_1_37_23_600,"[Scale_image: _StartScaleArgb888]software scale, handle the argb888 scale directly!")
TRACE_MSG(SCALE_IMAGE_1650_112_2_18_1_37_23_601,"[Scale_image: _StartScaleArgb888] accomplish software scale argb888 time is %d")
TRACE_MSG(SCALE_IMAGE_1673_112_2_18_1_37_23_602,"[Scale_image: _StartScaleArgb888] src_alpha_buf (0x%x, %d, %d), dst_alpha_buf (0x%x, %d, %d)")
TRACE_MSG(SCALE_IMAGE_1679_112_2_18_1_37_23_603,"[Scale_image: _StartScaleArgb888] allocate alpha buffer failed")
TRACE_MSG(SCALE_IMAGE_1694_112_2_18_1_37_23_604,"[Scale_image: _StartScaleArgb888] start scaling RGB888 time is %d ")
TRACE_MSG(SCALE_IMAGE_1703_112_2_18_1_37_23_605,"[Scale_image: _StartScaleArgb888] scale argb888 error!! scaling result is %d")
TRACE_MSG(SCALE_IMAGE_1709_112_2_18_1_37_23_606,"[Scale_image: _StartScaleArgb888] get alpha start time is %d, async mode is %d ")
TRACE_MSG(SCALE_IMAGE_1721_112_2_18_1_37_23_607,"[Scale_image: _StartScaleArgb888] get alpha accomplish time is %d")
TRACE_MSG(SCALE_IMAGE_1731_112_2_18_1_37_23_608,"[Scale_image: _StartScaleArgb888] scale rgb888 error!! thx stat is %d, scaling result is %d")
TRACE_MSG(SCALE_IMAGE_1738_112_2_18_1_37_24_609,"[Scale_image: _StartScaleArgb888] scale rgb accompish time is %d")
TRACE_MSG(SCALE_IMAGE_1767_112_2_18_1_37_24_610,"[Scale_image: _StartScaleArgb888] start scaling alpha time is %d ")
TRACE_MSG(SCALE_IMAGE_1784_112_2_18_1_37_24_611,"[Scale_image: _StartScaleArgb888] scale alpha error!! scaling result is %d")
TRACE_MSG(SCALE_IMAGE_1800_112_2_18_1_37_24_612,"[Scale_image: _StartScaleArgb888] alpha scale error, spend too much time!")
TRACE_MSG(SCALE_IMAGE_1809_112_2_18_1_37_24_613,"[Scale_image: _StartScaleArgb888] set alpha start time is %d, async mode is %d ")
TRACE_MSG(SCALE_IMAGE_1822_112_2_18_1_37_24_614,"[Scale_image: _StartScaleArgb888] Set alpha accomplish time is %d")
TRACE_MSG(SCALE_IMAGE_1833_112_2_18_1_37_24_615,"[Scale_image: _StartScaleArgb888] scale alpha error!! thx stat is %d, scaling result is %d")
TRACE_MSG(SCALE_IMAGE_1852_112_2_18_1_37_24_616,"[Scale_image: _StartScaleArgb888] alpha checksum after not corresponding!!")
TRACE_MSG(SCALE_IMAGE_1857_112_2_18_1_37_24_617,"[Scale_image: _StartScaleArgb888] accomplish scale alpha time is %d")
TRACE_MSG(SCALE_IMAGE_1893_112_2_18_1_37_24_618,"[IMGPROC_ScaleImage] input/output parameter invalid")
TRACE_MSG(SCALE_IMAGE_1899_112_2_18_1_37_24_619,"[IMGPROC_ScaleImage] src size (%d, %d), src tirm rect (%d, %d, %d, %d),")
TRACE_MSG(SCALE_IMAGE_1902_112_2_18_1_37_24_620,"[IMGPROC_ScaleImage] src format (%d), src chn0 (0x%x, %d), chn1 (0x%x, %d),")
TRACE_MSG(SCALE_IMAGE_1906_112_2_18_1_37_24_621,"[IMGPROC_ScaleImage] target size (%d, %d), target format (%d), target buf (0x%x, %d),")
TRACE_MSG(SCALE_IMAGE_1912_112_2_18_1_37_24_622,"[IMGPROC_ScaleImage] scale by software = %d")
TRACE_MSG(SCALE_IMAGE_1918_112_2_18_1_37_24_623,"[IMGPROC_ScaleImage] revise src width to %d for YUV422 format")
TRACE_MSG(SCALE_IMAGE_1926_112_2_18_1_37_24_624,"[IMGPROC_ScaleImage] revise src size to %dX%d for YUV420 format")
TRACE_MSG(SCALE_IMAGE_1936_112_2_18_1_37_24_625,"[IMGPROC_ScaleImage] ret = %d, _SetScaleParam failed")
TRACE_MSG(SCALE_IMAGE_1955_112_2_18_1_37_24_626,"[IMGPROC_ScaleImage] ret = %d, output size (%d, %d), output format (%d)")
TRACE_MSG(SCALE_IMAGE_1959_112_2_18_1_37_24_627,"[IMGPROC_ScaleImage]output chn0 (0x%x, %d), output chn1 (0x%x, %d)")
TRACE_MSG(SCALE_IMAGE_1963_112_2_18_1_37_24_628,"[IMGPROC_ScaleImage] spend time = %d")
TRACE_MSG(SCALE_SOFTWARE_371_112_2_18_1_37_25_629,"[_CheckScaleSize] trim rect size is valid")
TRACE_MSG(SCALE_SOFTWARE_391_112_2_18_1_37_25_630,"[_CheckScaleSize] scale mode is SCALE_MODE_NO_DISTORT, reset the target size")
TRACE_MSG(SCALE_SOFTWARE_431_112_2_18_1_37_25_631,"[_CheckScaleSize] src size (%d, %d), src rect (%d, %d, %d, %d), target size (%d, %d)")
TRACE_MSG(SCALE_SOFTWARE_462_112_2_18_1_37_25_632,"[_ScaleImage by software]")
TRACE_MSG(ISP_DRV_6X00_577_112_2_18_1_37_39_633,"ISP_SERVICE: ISP_SetVerDownTap the ver dwen top %d error")
TRACE_MSG(ISP_DRV_6X00_699_112_2_18_1_37_40_634,"ISP_SERVICE: ISP_SetMode the mode %d error")
TRACE_MSG(ISP_DRV_6X00_732_112_2_18_1_37_40_635,"ISP_SERVICE: ISP_SwichtClkDomain the domain %d error")
TRACE_MSG(ISP_DRV_6X00_791_112_2_18_1_37_40_636,"ISP_SERVICE: ISP_SetCAPInputFormat the format %d error")
TRACE_MSG(ISP_DRV_6X00_834_112_2_18_1_37_40_637,"ISP_SERVICE: ISP_SetSrcSize the x %d, y %y error")
TRACE_MSG(ISP_DRV_6X00_897_112_2_18_1_37_40_638,"ISP_SERVICE: ISP_SetScaleInputFormat the format %d error")
TRACE_MSG(ISP_DRV_SC6530_223_112_2_18_1_37_42_639,"ISP_Drv, _ISP_DriverModuleEnable, ahb_ctrl_addr 0x%x")
TRACE_MSG(ISP_DRV_SC6530_236_112_2_18_1_37_42_640,"ISP_Drv, _ISP_DriverGetIVSPController, got_rights %d")
TRACE_MSG(ISP_DRV_SC6530_256_112_2_18_1_37_42_641,"ISP_Drv, _ISP_DriverReleaseIVSPController, got_rights %d")
TRACE_MSG(ISP_DRV_SC6530_307_112_2_18_1_37_42_642,"ISP_Drv, _ISP_DriverModuleEnable, ahb_ctrl_addr 0x%x")
TRACE_MSG(ISP_DRV_SC6530_328_112_2_18_1_37_42_643,"ISP_Drv, ISP_DriverDeInit, ahb_ctrl_addr 0x%x")
TRACE_MSG(ISP_DRV_SC6530_410_112_2_18_1_37_42_644,"ISP_Drv, ISP_DriverStart, time %d")
TRACE_MSG(ISP_DRV_SC6530_438_112_2_18_1_37_43_645,"ISP_DRV: ISP_DriverStart, output_format %d")
TRACE_MSG(ISP_DRV_SC6530_494_112_2_18_1_37_43_646,"ISP_Drv, ISP_DriverStop, time %d")
TRACE_MSG(ISP_DRV_SC6530_618_112_2_18_1_37_43_647,"ISP_DRV: ISP_DriverFrameLock,f = 0x%x, time %d")
TRACE_MSG(ISP_DRV_SC6530_633_112_2_18_1_37_43_648,"ISP_DRV: ISP_DriverFrameUnLock,f = 0x%x, time %d")
TRACE_MSG(ISP_DRV_SC6530_652_112_2_18_1_37_43_649,"ISP_DRV: ISP_DriverFrameIsLocked,f = 0x%x, rtn %d")
TRACE_MSG(ISP_DRV_SC6530_692_112_2_18_1_37_43_650,"ISP_DRV: ISP_DriverCapConfig, base_addr 0x%x, id 0x%x, param 0x%x ")
TRACE_MSG(ISP_DRV_SC6530_948_112_2_18_1_37_44_651,"ISP_DRV: ISP_DriverCapConfig, rtn %d")
TRACE_MSG(ISP_DRV_SC6530_961_112_2_18_1_37_44_652,"ISP_DRV: ISP_DriverCapGetInfo, base_addr 0x%x, id 0x%x, param 0x%x ")
TRACE_MSG(ISP_DRV_SC6530_1472_112_2_18_1_37_45_653,"ISP_DRV: _ISP_ISRSensorStartOfFrame user_func 0x%x")
TRACE_MSG(ISP_DRV_SC6530_1485_112_2_18_1_37_45_654,"ISP_DRV: _ISP_ISRSensorEndOfFrame user_func 0x%x")
TRACE_MSG(ISP_DRV_SC6530_1501_112_2_18_1_37_45_655,"ISP_DRV: _ISP_ISRCapStartOfFrame,time = %d")
TRACE_MSG(ISP_DRV_SC6530_1511_112_2_18_1_37_45_656,"ISP_DRV: _ISP_ISRCapStartOfFrame, wait for the next frame unlocked")
TRACE_MSG(ISP_DRV_SC6530_1538_112_2_18_1_37_45_657,"ISP_DRV: _ISP_ISRCapEndOfFrame user_func 0x%x")
TRACE_MSG(ISP_DRV_SC6530_1555_112_2_18_1_37_45_658,"ISP_DRV: _ISP_ISRTxDone,time = %d")
TRACE_MSG(ISP_DRV_SC6530_1561_112_2_18_1_37_45_659,"ISP_DRV: _ISP_ISRTxDone,Has been stopped")
TRACE_MSG(ISP_DRV_SC6530_1586_112_2_18_1_37_45_660,"ISP_DRV: _ISP_ISRTxDone,frame_curr = 0x%x, y,u,v = {0x%x,0x%x,0x%x}, fmt %d")
TRACE_MSG(ISP_DRV_SC6530_1620_112_2_18_1_37_45_661,"ISP_DRV: _ISP_ISRTxDone,frame_curr = 0x%x, y,u,v = {0x%x,0x%x,0x%x}, w,h {%d,%d}, fmt %d")
TRACE_MSG(ISP_DRV_SC6530_1764_112_2_18_1_37_45_662,"ISP_Drv, _ISP_DriverSetFrameAddr, time %d")
TRACE_MSG(ISP_DRV_SC6530_1866_112_2_18_1_37_45_663,"ISP_DRV: _ISP_DriverGenScxCoeff i_w/i_h/o_w/o_h = {%d, %d, %d, %d,}")
TRACE_MSG(ISP_DRV_SC6530_1877_112_2_18_1_37_45_664,"ISP_DRV: _ISP_DriverGenScxCoeff GenScaleCoeff error!")
TRACE_MSG(ISP_DRV_SC8800G_455_112_2_18_1_37_47_665,"ISP_DRV: ISP_DriverStart , s_isp_mod.isp_mode = %d ,time = %d")
TRACE_MSG(ISP_DRV_SC8800G_477_112_2_18_1_37_47_666,"ISP_DRV: ISP_DriverStart , p_path2->input_format %d ,p_path2->output_format %d")
TRACE_MSG(ISP_DRV_SC8800G_585_112_2_18_1_37_47_667,"ISP_DRV: ISP_DriverExtStart , s_isp_mod.isp_mode = %d ,time = %d")
TRACE_MSG(ISP_DRV_SC8800G_1661_112_2_18_1_37_49_668,"ISP_DRV: ISP_DriverFrameLock,f = 0x%x")
TRACE_MSG(ISP_DRV_SC8800G_1676_112_2_18_1_37_49_669,"ISP_DRV: ISP_DriverFrameUnLock,f = 0x%x")
TRACE_MSG(ISP_DRV_SC8800G_1695_112_2_18_1_37_49_670,"ISP_DRV: ISP_DriverFrameIsLocked,f = 0x%x, rtn %d")
TRACE_MSG(ISP_DRV_SC8800G_1754_112_2_18_1_37_49_671,"ISP_DRV: _ISP_DriverHISRRoot, *pData 0x%x")
TRACE_MSG(ISP_DRV_SC8800G_1758_112_2_18_1_37_49_672,"ISP_DRV: _ISP_DriverHISRRoot, irq_num 0x%x")
TRACE_MSG(ISP_DRV_SC8800G_1787_112_2_18_1_37_49_673,"ISP_DRV: _ISP_DriverISRRoot, base_addr 0x%x irq_line 0x%x")
TRACE_MSG(ISP_DRV_SC8800G_1799_112_2_18_1_37_49_674,"ISP_DRV: _ISP_ISRSensorStartOfFrame user_func 0x%x")
TRACE_MSG(ISP_DRV_SC8800G_1812_112_2_18_1_37_49_675,"ISP_DRV: _ISP_ISRSensorEndOfFrame user_func 0x%x")
TRACE_MSG(ISP_DRV_SC8800G_1828_112_2_18_1_37_49_676,"ISP_DRV: _ISP_ISRCapStartOfFrame,time = %d")
TRACE_MSG(ISP_DRV_SC8800G_1846_112_2_18_1_37_49_677,"ISP_DRV: _ISP_ISRCapStartOfFrame, wait for the path1 next frame unlocked")
TRACE_MSG(ISP_DRV_SC8800G_1867_112_2_18_1_37_49_678,"ISP_DRV: _ISP_ISRCapStartOfFrame, wait for the next frame unlocked")
TRACE_MSG(ISP_DRV_SC8800G_1889_112_2_18_1_37_50_679,"ISP_DRV: _ISP_ISRCapEndOfFrame user_func 0x%x")
TRACE_MSG(ISP_DRV_SC8800G_1905_112_2_18_1_37_50_680,"ISP_DRV: _ISP_ISRPath1Done,time = %d")
TRACE_MSG(ISP_DRV_SC8800G_1915_112_2_18_1_37_50_681,"ISP_DRV: _ISP_ISRPath1Done,frame_curr = 0x%x, y,u,v = {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_DRV_SC8800G_2007_112_2_18_1_37_50_682,"ISP_DRV: _ISP_ISRPath2Done the next frame has not been unlocked, skip this frame")
TRACE_MSG(ISP_DRV_SC8800G_2046_112_2_18_1_37_50_683,"ISP_DRV: _ISP_ISRPath2Done y,u,v = {0x%x,0x%x,0x%x} ,width,height = {%d, %d},time = %d")
TRACE_MSG(ISP_DRV_SC8800G_2464_112_2_18_1_37_51_684,"ISP_DRV: _ISP_DriverGenScxCoeff GenScaleCoeff error!")
TRACE_MSG(ISP_DRV_SC8800G_2593_112_2_18_1_37_51_685,"ISP_DRV: _ISP_DriverGenScxCoeff i_w/i_h/o_w/o_h = {%d, %d, %d, %d,}")
TRACE_MSG(ISP_DRV_SC8800G_2604_112_2_18_1_37_51_686,"ISP_DRV: _ISP_DriverGenScxCoeff GenScaleCoeff error!")
TRACE_MSG(ISP_DRV_SC8800G_2718_112_2_18_1_37_51_687,"ISP_DRV: _ISP_DriverGenScxCoeff i_w/i_h/o_w/o_h = {%d, %d, %d, %d,}")
TRACE_MSG(ISP_DRV_SC8800G_2729_112_2_18_1_37_51_688,"ISP_DRV: _ISP_DriverGenScxCoeff GenScaleCoeff error!")
TRACE_MSG(ISP_DRV_SC8800H_483_112_2_18_1_37_52_689,"\r\n+++++++++++ISP:Start Auto Focus!+++++++++")
TRACE_MSG(ISP_DRV_SC8800H_512_112_2_18_1_37_53_690,"\r\n++++++++ISP:Stop AF+++++++")
TRACE_MSG(ISP_DRV_SC8800H_526_112_2_18_1_37_53_691,"rnISP_BeforeGetdataFromLCDC: addr %d,x %d, y %d, w %d, h %d")
TRACE_MSG(ISP_DRV_SC8800H_665_112_2_18_1_37_53_692,"\r\nISP:_ISP_AutoCopyAll")
TRACE_MSG(ISP_DRV_SC8800H_889_112_2_18_1_37_53_693,"\r\n_ISP_SetLNCCenter:cx=%d,cy=%d")
TRACE_MSG(ISP_DRV_SC8800H_900_112_2_18_1_37_53_694,"\r\n_ISP_SetLNCAlpha:r=%d,g=%d,b=%d,shift=%d")
TRACE_MSG(ISP_DRV_SC8800H_910_112_2_18_1_37_53_695,"\r\n_ISP_SetLNCSqareSize:rsize=%d")
TRACE_MSG(ISP_DRV_SC8800H_976_112_2_18_1_37_53_696,"\r\n_ISP_DumpCceMatrix: %d")
TRACE_MSG(ISP_DRV_SC8800H_1024_112_2_18_1_37_54_697,"\r\n_ISP_SetPortBAddress,ADDR=0x%x")
TRACE_MSG(ISP_DRV_SC8800H_1066_112_2_18_1_37_54_698,"\r\n_ISP_DumpGammaTable: %d")
TRACE_MSG(ISP_DRV_SC8800H_2940_112_2_18_1_37_58_699,"ISP_BeforeGetdataFromLCDC: addr %d,x %d, y %d, w %d, h %d")
TRACE_MSG(ISP_DRV_SC8800H_3769_112_2_18_1_37_59_700,"\r\nISP_DRV: film.w = %d, film.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_3771_112_2_18_1_37_59_701,"rnISP_DRV: interface_in.x= %d, interface_ice_in.y= %d, interface_in.w= %d,interface_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_3773_112_2_18_1_37_59_702,"rnISP_DRV: s1_in.x= %d, s1_in.y= %d, s1_in.w= %d,s1_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_3774_112_2_18_1_37_59_703,"\r\nISP_DRV: s1_miniout.w= %d, s1_miniout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_3775_112_2_18_1_37_59_704,"\r\nISP_DRV: s1_realout.w= %d, s1_realout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_3777_112_2_18_1_37_59_705,"rnISP_DRV: s2_in.x= %d, s2_in.y= %d, s2_in.w= %d,s2_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_3778_112_2_18_1_37_59_706,"\r\nISP_DRV: s2_miniout.w= %d, s2_miniout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_3779_112_2_18_1_37_59_707,"\r\nISP_DRV: s2_realout.w= %d, s2_realout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_3781_112_2_18_1_37_59_708,"rnISP_DRV: display.x= %d, display.y= %d, display.w= %d,display.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_3783_112_2_18_1_37_59_709,"\r\nISP_DRV: CAP_START = 0x%x, CAP_END = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_3785_112_2_18_1_37_59_710,"\r\nISP_DRV: ISP_SRC_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_3787_112_2_18_1_37_59_711,"\r\nISP_DRV: ISP_SC2_START = 0x%x, ISP_SC2_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_3789_112_2_18_1_37_59_712,"\r\nISP_DRV: ISP_DISP_START = 0x%x, ISP_DISP_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_3791_112_2_18_1_37_59_713,"\r\nISP_DRV: ISP_ENC_START = 0x%x, ISP_ENC_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_3793_112_2_18_1_37_59_714,"\r\nISP_DRV: SC1_CONF = 0x%x, SC2_CONF = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4040_112_2_18_1_38_0_715,"\r\nISP_DRV: film.w = %d, film.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4042_112_2_18_1_38_0_716,"rnISP_DRV: interface_in.x= %d, interface_in.y= %d, interface_in.w= %d,interface_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4044_112_2_18_1_38_0_717,"rnISP_DRV: s1_in.x= %d, s1_in.y= %d, s1_in.w= %d,s1_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4045_112_2_18_1_38_0_718,"\r\nISP_DRV: s1_miniout.w= %d, s1_miniout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4046_112_2_18_1_38_0_719,"\r\nISP_DRV: s1_realout.w= %d, s1_realout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4048_112_2_18_1_38_0_720,"rnISP_DRV: s2_in.x= %d, s2_in.y= %d, s2_in.w= %d,s2_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4049_112_2_18_1_38_0_721,"\r\nISP_DRV: s2_miniout.w= %d, s2_miniout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4050_112_2_18_1_38_0_722,"\r\nISP_DRV: s2_realout.w= %d, s2_realout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4052_112_2_18_1_38_0_723,"rnISP_DRV: display.x= %d, display.y= %d, display.w= %d,display.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4054_112_2_18_1_38_0_724,"\r\nISP_DRV: CAP_START = 0x%x, CAP_END = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4056_112_2_18_1_38_0_725,"\r\nISP_DRV: ISP_SRC_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4058_112_2_18_1_38_0_726,"\r\nISP_DRV: ISP_SC2_START = 0x%x, ISP_SC2_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4060_112_2_18_1_38_0_727,"\r\nISP_DRV: ISP_DISP_START = 0x%x, ISP_DISP_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4062_112_2_18_1_38_0_728,"\r\nISP_DRV: ISP_ENC_START = 0x%x, ISP_ENC_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4064_112_2_18_1_38_0_729,"\r\nISP_DRV: SC1_CONF = 0x%x, SC2_CONF = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4220_112_2_18_1_38_0_730,"rnISP_DRV: s2_in.x= %d, s2_in.y= %d, s2_in.w= %d,s2_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4221_112_2_18_1_38_0_731,"\r\nISP_DRV: s2_miniout.w= %d, s2_miniout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4222_112_2_18_1_38_0_732,"\r\nISP_DRV: s2_realout.w= %d, s2_realout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4224_112_2_18_1_38_0_733,"rnISP_DRV: display.x= %d, display.y= %d, display.w= %d,display.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4227_112_2_18_1_38_0_734,"rnISP_DRV: ISP_PORTC_CURR_YBA = 0x%x,ISP_PORTC_CURR_UBA = 0x%x,ISP_PORTC_CURR_VBA = 0x%x ")
TRACE_MSG(ISP_DRV_SC8800H_4229_112_2_18_1_38_0_735,"\r\nISP_DRV: SC1_CONF = 0x%x, SC2_CONF = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4231_112_2_18_1_38_0_736,"\r\nISP_DRV: ISP_SC2_START = 0x%x, ISP_SC2_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4233_112_2_18_1_38_0_737,"\r\nISP_DRV: ISP_DISP_START = 0x%x, ISP_DISP_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4235_112_2_18_1_38_0_738,"\r\nISP_DRV: VP_REVIEW_SIZE = %d")
TRACE_MSG(ISP_DRV_SC8800H_4237_112_2_18_1_38_0_739,"\r\nISP_DRV: ISP_PORTC_SKIP = %d")
TRACE_MSG(ISP_DRV_SC8800H_4239_112_2_18_1_38_0_740,"\r\nISP_DRV: ISP_SELF_RATE = %d")
TRACE_MSG(ISP_DRV_SC8800H_4243_112_2_18_1_38_0_741,"ISP_DRV: vt review start, curr_time = %d")
TRACE_MSG(ISP_DRV_SC8800H_4246_112_2_18_1_38_0_742,"rnISP_DRV: ISP_PORTC_CURR_YBA = 0x%x,ISP_PORTC_CURR_UBA = 0x%x,ISP_PORTC_CURR_VBA = 0x%x ")
TRACE_MSG(ISP_DRV_SC8800H_4247_112_2_18_1_38_0_743,"ISP_DRV start PORTC, curr_time=%d ")
TRACE_MSG(ISP_DRV_SC8800H_4602_112_2_18_1_38_1_744,"rnISP_DRV: interface_in.x= %d, interface_in.y= %d, interface_in.w= %d,interface_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4604_112_2_18_1_38_1_745,"rnISP_DRV: s1_in.x= %d, s1_in.y= %d, s1_in.w= %d,s1_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4605_112_2_18_1_38_1_746,"\r\nISP_DRV: s1_miniout.w= %d, s1_miniout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4606_112_2_18_1_38_1_747,"\r\nISP_DRV: s1_realout.w= %d, s1_realout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4608_112_2_18_1_38_1_748,"rnISP_DRV: s2_in.x= %d, s2_in.y= %d, s2_in.w= %d,s2_in.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4609_112_2_18_1_38_1_749,"\r\nISP_DRV: s2_miniout.w= %d, s2_miniout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4610_112_2_18_1_38_1_750,"\r\nISP_DRV: s2_realout.w= %d, s2_realout.h = %d")
TRACE_MSG(ISP_DRV_SC8800H_4612_112_2_18_1_38_1_751,"rnISP_DRV: display.x= %d, display.y= %d, display.w= %d,display.h= %d")
TRACE_MSG(ISP_DRV_SC8800H_4615_112_2_18_1_38_1_752,"\r\nISP_DRV: ISP_SRC_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4617_112_2_18_1_38_1_753,"\r\nISP_DRV: SC1_CONF = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4619_112_2_18_1_38_1_754,"\r\nISP_DRV: ISP_SC2_START = 0x%x, ISP_SC2_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4621_112_2_18_1_38_1_755,"\r\nISP_DRV: ISP_DISP_START = 0x%x, ISP_DISP_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4623_112_2_18_1_38_1_756,"\r\nISP_DRV: ISP_ENC_START = 0x%x, ISP_ENC_SIZE = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4626_112_2_18_1_38_1_757,"rnISP_DRV: ISP_PORTA_NEXT_YBA = 0x%x, ISP_PORTA_NEXT_UBA = 0x%x, ISP_PORTA_NEXT_VBA = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_4644_112_2_18_1_38_1_758,"ISP drv: ISP_StartFromRawRGBBypassMode")
TRACE_MSG(ISP_DRV_SC8800H_4827_112_2_18_1_38_1_759,"ISP_DRV: ISP_PORTC_SKIP = %d")
TRACE_MSG(ISP_DRV_SC8800H_4829_112_2_18_1_38_1_760,"ISP_DRV: ISP_SELF_RATE = %d")
TRACE_MSG(ISP_DRV_SC8800H_4831_112_2_18_1_38_1_761,"ISP_DRV: start portc = %d")
TRACE_MSG(ISP_DRV_SC8800H_4915_112_2_18_1_38_1_762,"ISP:ISP_ISR1CapStartOfFrame curr_time=%d")
TRACE_MSG(ISP_DRV_SC8800H_4942_112_2_18_1_38_2_763,"ISP_DRV:Review enabled")
TRACE_MSG(ISP_DRV_SC8800H_5049_112_2_18_1_38_2_764,"ISP:ISP_ISR2CapEndOfFrame curr_time = %d")
TRACE_MSG(ISP_DRV_SC8800H_5117_112_2_18_1_38_2_765,"ISP_ISR3EncoderEndOfFrame,curr_yaddr=0x%x ,cutt_time = %d,time_interval=%d\r\n")
TRACE_MSG(ISP_DRV_SC8800H_5197_112_2_18_1_38_2_766,"ISP:ISP_ISR4DisplayEndOfFrame,curr_addr=0x%x, curr_time=%d ")
TRACE_MSG(ISP_DRV_SC8800H_5254_112_2_18_1_38_2_767,"ISP:ISP_ISR5PortAFIFOOverflowed,curr_time=%d ")
TRACE_MSG(ISP_DRV_SC8800H_5260_112_2_18_1_38_2_768,"ISP CURR REGISTER: ISP_CONF=0x%x, ISP_SUBM_MODE=0x%x, ISP_SELF_RATE=0x%x,CAP_CNTRL=0x%x,CAP_START=0x%x, CAP_END=0x%x")
TRACE_MSG(ISP_DRV_SC8800H_5265_112_2_18_1_38_2_769,"ISP CURR REGISTER: ISP_SRC_SIZE=0x%x, ISP_SC2_SIZE=0x%x, ISP_SC2_START=0x%x, ISP_SC2_START=0x%x,ISP_DISP_SIZE=0x%x,ISP_DISP_START=0x%x,ISP_ENC_SIZE=0x%x,ISP_ENC_START=0x%x")
TRACE_MSG(ISP_DRV_SC8800H_5301_112_2_18_1_38_2_770,"ISP:ISP_ISR6PortBFIFOOverflowed,curr_time=%d ")
TRACE_MSG(ISP_DRV_SC8800H_5311_112_2_18_1_38_2_771,"ISP CURR REGISTER: ISP_CONF=0x%x, ISP_SUBM_MODE=0x%x, ISP_SELF_RATE=0x%x,CAP_CNTRL=0x%x,CAP_START=0x%x, CAP_END=0x%x")
TRACE_MSG(ISP_DRV_SC8800H_5317_112_2_18_1_38_2_772,"ISP CURR REGISTER: ISP_SRC_SIZE=0x%x, ISP_SC2_SIZE=0x%x, ISP_SC2_START=0x%x, ISP_SC2_START=0x%x,ISP_DISP_SIZE=0x%x,ISP_DISP_START=0x%x,ISP_ENC_SIZE=0x%x,ISP_ENC_START=0x%x")
TRACE_MSG(ISP_DRV_SC8800H_5344_112_2_18_1_38_2_773,"ISP: ISP_ISR6PortBFIFOOverflowed ,blank_per_line = %x,pixel_per_clk = %d,portb_threshold=%d")
TRACE_MSG(ISP_DRV_SC8800H_5375_112_2_18_1_38_2_774,"ISP:ISP_ISR7SC1UpFIFOOverflowed")
TRACE_MSG(ISP_DRV_SC8800H_5386_112_2_18_1_38_2_775,"ISP:ISP_ISR8CapFIFOOverflowed")
TRACE_MSG(ISP_DRV_SC8800H_5397_112_2_18_1_38_2_776,"\r\nISP:ISP_ISR9AutoFocusEndOfFrame")
TRACE_MSG(ISP_DRV_SC8800H_5406_112_2_18_1_38_2_777,"\r\nISP:ISP_ISRALCD2SC2StartOfFrame")
TRACE_MSG(ISP_DRV_SC8800H_5417_112_2_18_1_38_2_778,"\r\nISP:ISP_ISRBPortCEndOfFrame")
TRACE_MSG(ISP_DRV_SC8800H_5721_112_2_18_1_38_3_779,"ISP drv: ISP_OnPause")
TRACE_MSG(ISP_DRV_SC8800H_5732_112_2_18_1_38_3_780,"ISP drv: ISP_OnPause, Reset ISP")
TRACE_MSG(ISP_DRV_SC8800H_6080_112_2_18_1_38_4_781,"ISP DRV:s->pb_current = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_6081_112_2_18_1_38_4_782,"ISP DRV:s->pb_frame = 0x%x")
TRACE_MSG(ISP_DRV_SC8800H_6082_112_2_18_1_38_4_783,"ISP DRV:s->display.w = 0x%x,s->display.h = 0x%x")
TRACE_MSG(IVSP_DRV_6X00_467_112_2_18_1_38_16_784,"[_IVsp_Open] ISR_RegHandler_Ex ret = %d")
TRACE_MSG(IVSP_DRV_6X00_542_112_2_18_1_38_16_785,"IVsp_ModeSet: IVsp_Stop mode:%d")
TRACE_MSG(IVSP_DRV_6X00_615_112_2_18_1_38_16_786,"ISP_SERVICE: IVsp_RegisterIntFunc interrupt num is invalid %d error")
TRACE_MSG(IVSP_DRV_6X00_669_112_2_18_1_38_16_787,"ISP_SERVICE: IVsp_RegisterIntFuncEx interrupt num is invalid %d error")
TRACE_MSG(IVSP_DRV_6X00_689_112_2_18_1_38_16_788,"ISP_SERVICE: IVsp_OpenInt interrupt num is invalid %d error")
TRACE_MSG(IVSP_DRV_6X00_708_112_2_18_1_38_16_789,"ISP_SERVICE: IVsp_OpenInt interrupt num is invalid %d error")
TRACE_MSG(IVSP_DRV_6X00_724_112_2_18_1_38_16_790,"ISP_SERVICE: IVsp_OpenInt interrupt num is invalid %d error")
TRACE_MSG(IVSP_DRV_8800H5_478_112_2_18_1_38_17_791,"[_IVsp_Open] ISR_RegHandler_Ex ret = %d")
TRACE_MSG(IVSP_DRV_8800H5_518_112_2_18_1_38_17_792,"IVsp_ModeSet: IVsp_Stop mode:%d")
TRACE_MSG(IVSP_DRV_8800H5_549_112_2_18_1_38_18_793,"IVsp_SwitchMode: s_ivsp_mode %d, mode %d, is_enter %d")
TRACE_MSG(IVSP_DRV_8800H5_558_112_2_18_1_38_18_794,"IVsp_SwitchMode: get mutex task id=0x%x")
TRACE_MSG(IVSP_DRV_8800H5_564_112_2_18_1_38_18_795,"IVsp_SwitchMode: cur close task id=0x%x")
TRACE_MSG(IVSP_DRV_8800H5_568_112_2_18_1_38_18_796,"IVsp_SwitchMode: put mutex task id=0x%x")
TRACE_MSG(IVSP_DRV_SC6530_120_112_2_18_1_38_18_797,"IVsp_Open, thread id %d, mode %d, option %d, time %d")
TRACE_MSG(IVSP_DRV_SC6530_124_112_2_18_1_38_18_798,"IVsp_Open, upsupported IVSP MODE!")
TRACE_MSG(IVSP_DRV_SC6530_133_112_2_18_1_38_18_799,"IVsp_Open, Semaphore created!, 0x%x, time %d")
TRACE_MSG(IVSP_DRV_SC6530_142_112_2_18_1_38_18_800,"IVsp_Open, time out %d")
TRACE_MSG(IVSP_DRV_SC6530_149_112_2_18_1_38_18_801,"IVsp_Open, Got semaphore!, thread id %d, time %d")
TRACE_MSG(IVSP_DRV_SC6530_156_112_2_18_1_38_18_802,"IVsp_Open, ivsp_mod.ivsp_sema 0x%x, time %d")
TRACE_MSG(IVSP_DRV_SC6530_186_112_2_18_1_38_18_803,"IVsp_Close, thread id %d, mode %d, time %d")
TRACE_MSG(IVSP_DRV_SC6530_193_112_2_18_1_38_18_804,"IVsp_Close, wrong ivsp mode %d, current working mode %d")
TRACE_MSG(IVSP_DRV_SC6530_215_112_2_18_1_38_18_805,"IVsp_RegisterIntFunc, irq number %d, isr_func 0x%x")
TRACE_MSG(IVSP_DRV_SC6530_254_112_2_18_1_38_18_806,"IVsp_UnRegisterIntFunc, irq number %d ")
TRACE_MSG(IVSP_DRV_SC6530_301_112_2_18_1_38_19_807,"_IVSP_LowISR, irq_line 0x%x")
TRACE_MSG(IVSP_DRV_SC6530_319_112_2_18_1_38_19_808,"_IVSP_HighISR, irq_num 0x%x, data 0x%x")
TRACE_MSG(JAPP_618_112_2_18_1_38_22_809,"[JPG_CreateMiniature] dec_buf_size = %d, used_size = %d")
TRACE_MSG(JAPP_620_112_2_18_1_38_22_810,"[JPG_CreateMiniature] enc_buf_size = %d, used_size = %d")
TRACE_MSG(JAPP_697_112_2_18_1_38_22_811,"[JPG_EncodeYUV444] size = (%d, %d), encode buf = 0x%x, size = %d")
TRACE_MSG(JAPP_976_112_2_18_1_38_22_812,"[JPG_EncodeYUV422] size = (%d, %d), encode buf = 0x%x, size = %d")
TRACE_MSG(JAPP_1149_112_2_18_1_38_23_813,"[JPG_DecodeToRGB] target buf = 0x%x, size = %d, target size = (%d, %d), src rect = (%x, %x, %d, %d)")
TRACE_MSG(JAPP_1417_112_2_18_1_38_23_814,"[JPG_DecodeToRGB] scaled rect = (%x, %x, %d, %d)")
TRACE_MSG(JAPP_1426_112_2_18_1_38_23_815,"[JPG_DecodeToRGB] output rect = (%x, %x, %d, %d)")
TRACE_MSG(JAPP_1603_112_2_18_1_38_24_816,"[JPG_DecodeToRGB] scaled rect = (%x, %x, %d, %d)")
TRACE_MSG(JAPP_1611_112_2_18_1_38_24_817,"[JPG_DecodeToRGB] output rect = (%x, %x, %d, %d)")
TRACE_MSG(JAPP_1699_112_2_18_1_38_24_818,"[JPG_DecodeToRgb] decode to RGB OK! buf = 0x%x, w = %d, h = %d")
TRACE_MSG(JAPP_1707_112_2_18_1_38_24_819,"[JPG_DecodeToRgb] dec_buf_size = %d, used_size = %d")
TRACE_MSG(JAPP_2065_112_2_18_1_38_25_820,"[JPG_DecodeToYUV] dec_buf_size = %d, used_size = %d")
TRACE_MSG(JPEGDEC_FRAME_301_112_2_18_1_39_21_821,"[START_HW_DECODE] flush failed, i = %d")
TRACE_MSG(JPEGDEC_FRAME_1138_112_2_18_1_39_38_822,"Invalid raw_height, must be interger times of 8 or 16...\n")
TRACE_MSG(JPEGDEC_FRAME_1172_112_2_18_1_39_38_823,"[JPEG_HWDecStartMCUSynchro] decode error, slice = %d")
TRACE_MSG(JPEGDEC_FRAME_1178_112_2_18_1_39_38_824,"[JPEG_HWDecStartMCUSynchro] decode timeout, slice = %d")
TRACE_MSG(JPEGDEC_FRAME_1209_112_2_18_1_39_38_825,"[JPEG_HWDecStartMCUSynchro] decode error, last slice")
TRACE_MSG(JPEGDEC_FRAME_1215_112_2_18_1_39_38_826,"[JPEG_HWDecStartMCUSynchro] decode timeout, last slice")
TRACE_MSG(JPEGDEC_FRAME_1225_112_2_18_1_39_38_827,"[JPEG_HWDecStartMCUSynchro] JPEG_HWWaitingEnd failed")
TRACE_MSG(JPEGDEC_PARSE_998_112_2_18_1_39_43_828,"[JPEG_HWParseHead] find the SOI marker error: %x")
TRACE_MSG(JPEGDEC_STREAM_197_112_2_18_1_39_44_829,"[JPEGFW_GetC] g_jpeg_fw_bsm_current_buffer_index = %d")
TRACE_MSG(JPEGDEC_STREAM_203_112_2_18_1_39_44_830,"[JPEGFW_GetC] no more bytes, return error, cmd1 = 0x%x, total_read_size = %d")
TRACE_MSG(JPEGDEC_STREAM_240_112_2_18_1_39_44_831,"[JPEGFW_GetC] g_jpeg_fw_bsm_current_buffer_index = %d")
TRACE_MSG(JPEGDEC_STREAM_245_112_2_18_1_39_44_832,"[JPEGFW_GetC] no more bytes, return error, cmd0 = 0x%x, total_read_size = %d")
TRACE_MSG(JPEGDEC_STREAM_328_112_2_18_1_39_44_833,"[JPEGFW_GetW] g_jpeg_fw_bsm_current_buffer_index = %d")
TRACE_MSG(JPEGDEC_STREAM_333_112_2_18_1_39_44_834,"[JPEGFW_GetW] no more bytes, return error, cmd1 = 0x%x, total_read_size = %d")
TRACE_MSG(JPEGDEC_FRAME_479_112_2_18_1_39_57_835,"[START_HW_DECODE] flush failed, i = %d")
TRACE_MSG(JPEGDEC_FRAME_310_112_2_18_1_40_16_836,"[START_HW_DECODE] flush failed, i = %d")
TRACE_MSG(JPEGDEC_FRAME_307_112_2_18_1_40_35_837,"[START_HW_DECODE] flush failed, i = %d")
TRACE_MSG(EXIF_READER_2370_112_2_18_1_40_54_838,"[Jpeg_ParseExif] EXIF_READER_SUPPORT is not defined!")
TRACE_MSG(EXIF_WRITER_2971_112_2_18_1_41_0_839,"[IMGJPEG_WriteExif] EXIF_WRITER_SUPPORT is not defined!")
TRACE_MSG(EXIF_WRITER_3000_112_2_18_1_41_0_840,"[IMGJPEG_WriteExif] EXIF_WRITER_SUPPORT is not defined!")
TRACE_MSG(JPEG_ALGORITHM_1528_112_2_18_1_41_3_841,"[YUVAlign] unsupport data format")
TRACE_MSG(JPEG_EXIF_50_112_2_18_1_41_5_842,"JPEG_PaserExifInfo : in")
TRACE_MSG(JPEG_EXIF_54_112_2_18_1_41_5_843,"JPEG_PaserExifInfo(PNULL, xxx, xxx)")
TRACE_MSG(JPEG_EXIF_75_112_2_18_1_41_5_844,"JPEG_PaserExifInfo : width: %d, height: %d")
TRACE_MSG(JPEG_EXIF_102_112_2_18_1_41_5_845,"JPEG_PaserExifInfo : out")
TRACE_MSG(JPEG_HAL_44_112_2_18_1_41_8_846,"[JPEG_IsSoftwareScalingDown] need scaling down")
TRACE_MSG(JPEG_HAL_49_112_2_18_1_41_8_847,"[JPEG_IsSoftwareScalingDown] do not need scaling down")
TRACE_MSG(JPEG_HAL_78_112_2_18_1_41_8_848,"[JPEG_IsHardwarecalingDown], src_size = (%d, %d), src trim rect (%d, %d, %d, %d), target size = (%d, %d)")
TRACE_MSG(JPEG_HAL_81_112_2_18_1_41_8_849,"[JPEG_IsHardwarecalingDown] target data = %d, src yuv = %d, target yuv = %d")
TRACE_MSG(JPEG_HAL_104_112_2_18_1_41_8_850,"[JPEG_IsHardwareScalingDown] is_scaling = %d")
TRACE_MSG(JPEG_HAL_159_112_2_18_1_41_8_851,"[JPEG_GetMCUSize] unsupport quality")
TRACE_MSG(JPEG_HAL_341_112_2_18_1_41_9_852,"[JPEG_ParseRes] unsupport quality")
TRACE_MSG(JPEG_HAL_368_112_2_18_1_41_9_853,"[JPEG_ParseRes] unsupport quality")
TRACE_MSG(JPEG_HAL_408_112_2_18_1_41_9_854,"[JPEG_ParseStdRes], allocate dqt failed")
TRACE_MSG(JPEG_HAL_423_112_2_18_1_41_9_855,"[JPEG_ParseStdRes], Jpeg_parser failed, return = %d")
TRACE_MSG(JPEG_HAL_438_112_2_18_1_41_9_856,"[JPEG_ParseStdRes], parse quality failed")
TRACE_MSG(JPEG_HAL_451_112_2_18_1_41_9_857,"[JPEG_ParseStdRes] w = %d, h = %d, format = %d, quality = %d, head size = %d")
TRACE_MSG(JPEG_HAL_472_112_2_18_1_41_9_858,"[JPEG_ResParse] jpeg stream size smaller than jpeg head size, not res jpeg!")
TRACE_MSG(JPEG_HAL_479_112_2_18_1_41_9_859,"[JPEG_ParseRes] jpeg res ID mismatch")
TRACE_MSG(JPEG_HAL_514_112_2_18_1_41_9_860,"[JPEG_ParseRes] unsupport quality")
TRACE_MSG(JPEG_HAL_541_112_2_18_1_41_9_861,"[JPEG_ParseRes] unsupport quality")
TRACE_MSG(JPEG_HAL_550_112_2_18_1_41_9_862,"[JPEG_ParseRes] w = %d, h = %d, format = %d, quality = %d, head size = %d")
TRACE_MSG(JPEG_HAL_571_112_2_18_1_41_9_863,"[JPEG_ParseSJPGRes] input pointer invalid, 0x%x, 0x%x")
TRACE_MSG(JPEG_HAL_721_112_2_18_1_41_9_864,"[JPEG_ParseRes] warning, unsupporte format, use format")
TRACE_MSG(JPEG_HEAD_INFO_50_112_2_18_1_41_10_865,"[Jpeg_ParseSOF] SOF, SOF length error, %d")
TRACE_MSG(JPEG_HEAD_INFO_79_112_2_18_1_41_10_866,"[Jpeg_ParseSOF] SOF, the first component is not Y component, return error")
TRACE_MSG(JPEG_HEAD_INFO_113_112_2_18_1_41_10_867,"[Jpeg_ParseSOF] SOF, sample factor of Y component error, %d")
TRACE_MSG(JPEG_HEAD_INFO_119_112_2_18_1_41_10_868,"[Jpeg_ParseSOF] SOF, component error, %d")
TRACE_MSG(JPEG_HEAD_INFO_207_112_2_18_1_41_10_869,"[Jpeg_ParseDQT] skipped marker failed, marker = 0x%x, size = %d")
TRACE_MSG(JPEG_HEAD_INFO_283_112_2_18_1_41_10_870,"[Jpeg_ParseDHT] skipped marker failed, marker = 0x%x, size = %d")
TRACE_MSG(JPEG_HEAD_INFO_390_112_2_18_1_41_10_871,"[Jpeg_ParseDRI] skipped marker failed,  size = %d")
TRACE_MSG(JPEG_HEAD_INFO_440_112_2_18_1_41_10_872,"[Jpeg_ParseSOS] skipped marker failed, marker = 0x%x, size = %d")
TRACE_MSG(JPEG_HEAD_INFO_505_112_2_18_1_41_11_873,"[Jpeg_SkipMarker] skipped marker failed, size = %d")
TRACE_MSG(JPEG_HEAD_INFO_511_112_2_18_1_41_11_874,"[Jpeg_SkipMarker] skipped marker failed,  size = %d")
TRACE_MSG(JPEG_HEAD_INFO_569_112_2_18_1_41_11_875,"[Jpeg_ParseMarker] the first marker is not SOI, marker = 0x%x")
TRACE_MSG(JPEG_HEAD_INFO_577_112_2_18_1_41_11_876,"[Jpeg_ParseMarker] the first marker is not SOI, marker = 0x%x")
TRACE_MSG(JPEG_HEAD_INFO_598_112_2_18_1_41_11_877,"[Jpeg_ParseMarker] find marker M_EOI, return error")
TRACE_MSG(JPEG_HEAD_INFO_626_112_2_18_1_41_11_878,"[Jpeg_ParseMarker] an error occurred while parsing APP1 marker")
TRACE_MSG(JPEG_HEAD_INFO_668_112_2_18_1_41_11_879,"[Jpeg_ParseMarker] unsupported marker, marker = 0x%x")
TRACE_MSG(JPEG_HEAD_INFO_774_112_2_18_1_41_11_880,"[Jpeg_ParseHead] no source found")
TRACE_MSG(JPEG_INTERFACE_289_112_2_18_1_41_12_881,"IMGJPEG_FreeRes, line %d, state = %d")
TRACE_MSG(JPEG_INTERFACE_295_112_2_18_1_41_12_882,"IMGJPEG_FreeRes, state is free, do nothing!")
TRACE_MSG(JPEG_INTERFACE_304_112_2_18_1_41_12_883,"IMGJPEG_FreeRes, no task, line %d")
TRACE_MSG(JPEG_INTERFACE_313_112_2_18_1_41_12_884,"IMGJPEG_FreeRes, wait task stopped,line %d")
TRACE_MSG(JPEG_INTERFACE_318_112_2_18_1_41_12_885,"IMGJPEG_FreeRes, working by software")
TRACE_MSG(JPEG_INTERFACE_349_112_2_18_1_41_12_886," IMGJPEG_GetInfo(), line %d, info_ptr = 0x%x")
TRACE_MSG(JPEG_INTERFACE_387_112_2_18_1_41_12_887,"[IMGJPEG_GetInfo] stream in file = 0x%x, read buf = 0x%x, size = %d")
TRACE_MSG(JPEG_INTERFACE_394_112_2_18_1_41_12_888,"[IMGJPEG_GetInfo] stream in buffer = 0x%x, size = %d")
TRACE_MSG(JPEG_INTERFACE_401_112_2_18_1_41_12_889,"[IMGJPEG_GetInfo], Jpeg_parser failed, return = %d")
TRACE_MSG(JPEG_INTERFACE_420_112_2_18_1_41_12_890,"[IMGJPEG_GetInfo] width = %d, height = %d, jpeg type = %d, sample format = %d")
TRACE_MSG(JPEG_INTERFACE_453_112_2_18_1_41_12_891,"[IMGJPEG_GetInfo] get sjpg information")
TRACE_MSG(JPEG_INTERFACE_474_112_2_18_1_41_12_892,"[IMGJPEG_GetInfo], parse sjpg failed, return = %d")
TRACE_MSG(JPEG_INTERFACE_510_112_2_18_1_41_13_893,"[IMGJPEG_CreateMiniature] state = %d, target buf = 0x%x, size = %d, decode buf = 0x%x, size = %d,")
TRACE_MSG(JPEG_INTERFACE_512_112_2_18_1_41_13_894,"[IMGJPEG_CreateMiniature] jpeg buf = 0x%x, size = %d")
TRACE_MSG(JPEG_INTERFACE_529_112_2_18_1_41_13_895,"[IMGJPEG] software decode, nRet = %d")
TRACE_MSG(JPEG_INTERFACE_543_112_2_18_1_41_13_896,"[IMGJPEG_CreateMiniature] unsupport format, re-decode it by software")
TRACE_MSG(JPEG_INTERFACE_547_112_2_18_1_41_13_897,"[IMGJPEG_CreateMiniature] hardware decode, nRet = %d")
TRACE_MSG(JPEG_INTERFACE_550_112_2_18_1_41_13_898,"[IMGJPEG_CreateMiniature] neither HW or SW decoder/encoder defined !")
TRACE_MSG(JPEG_INTERFACE_554_112_2_18_1_41_13_899,"[IMGJPEG_CreateMiniature ] time = %d")
TRACE_MSG(JPEG_INTERFACE_559_112_2_18_1_41_13_900,"[IMGJPEG_CreateMiniature] miniature disabled")
TRACE_MSG(JPEG_INTERFACE_563_112_2_18_1_41_13_901,"[IMGJPEG_CreateMiniature] JPG_MINIATURE_SUPPORT not defined !")
TRACE_MSG(JPEG_INTERFACE_643_112_2_18_1_41_13_902,"[IMGJPEG_Encode] state = %d, w = %d, h = %d, sample format = %d, quality = %d")
TRACE_MSG(JPEG_INTERFACE_647_112_2_18_1_41_13_903,"[IMGJPEG_Encode] target buf = 0x%x, size = %d, encode buf = 0x%x, size = %d,")
TRACE_MSG(JPEG_INTERFACE_697_112_2_18_1_41_13_904,"[IMGJPEG_Encode] temp buf, addr = 0x%x, size  = %d, used size = %d")
TRACE_MSG(JPEG_INTERFACE_699_112_2_18_1_41_13_905,"[IMGJPEG_Encode] hardware encode, ret = %d")
TRACE_MSG(JPEG_INTERFACE_703_112_2_18_1_41_13_906,"[IMGJPEG] hardware encode, unsupport format,")
TRACE_MSG(JPEG_INTERFACE_712_112_2_18_1_41_13_907,"[IMGJPEG] software encode, ret = %d,")
TRACE_MSG(JPEG_INTERFACE_716_112_2_18_1_41_13_908,"[IMGJPEG] software encode, unsupport format,")
TRACE_MSG(JPEG_INTERFACE_724_112_2_18_1_41_13_909,"[IMGJPEG] Exit IMGJPEG_Encode, time = %d")
TRACE_MSG(JPEG_INTERFACE_798_112_2_18_1_41_13_910,"[IMGJPEG_Decode] target buf = 0x%x, size = %d, decode buf = 0x%x, size = %d,")
TRACE_MSG(JPEG_INTERFACE_802_112_2_18_1_41_13_911,"[IMGJPEG_Decode] target size = (%d, %d), target_data_type = %d, jpeg buf = 0x%x, size = %d")
TRACE_MSG(JPEG_INTERFACE_837_112_2_18_1_41_13_912,"[IMGJPEG_GetInfo], Jpeg_parser failed, return = %d")
TRACE_MSG(JPEG_INTERFACE_849_112_2_18_1_41_13_913,"[IMGJPEG_Decode] YUV422R JPEG, convert to YUV422 on platform 6600l")
TRACE_MSG(JPEG_INTERFACE_854_112_2_18_1_41_13_914,"[IMGJPEG_Decode] jpeg info: w = %d, h = %d, format = %d, is rotated_yuv422 = %d")
TRACE_MSG(JPEG_INTERFACE_863_112_2_18_1_41_13_915,"[IMGJPEG_Decode] hardware decode baseline jpeg")
TRACE_MSG(JPEG_INTERFACE_872_112_2_18_1_41_13_916,"[IMGJPEG_Decode] software decode progressive jpeg, jpeg buf = %d, read file func = 0x%x")
TRACE_MSG(JPEG_INTERFACE_882_112_2_18_1_41_13_917,"[IMGJPEG_Decode] size of progressive jpeg bigger than allowed !")
TRACE_MSG(JPEG_INTERFACE_886_112_2_18_1_41_13_918,"[IMGJPEG_Decode] JPG_DEC_TYPE_SUPPORT_BASELINE_PROGRESSIVE undefined !")
TRACE_MSG(JPEG_INTERFACE_900_112_2_18_1_41_13_919,"[IMGJPEG_Decode] software decode baseline jpeg, jpeg buf = %d, read file func = 0x%x")
TRACE_MSG(JPEG_INTERFACE_910_112_2_18_1_41_13_920,"[IMGJPEG_Decode] software decode progressive jpeg, jpeg buf = %d, read file func = 0x%x")
TRACE_MSG(JPEG_INTERFACE_919_112_2_18_1_41_13_921,"[IMGJPEG_Decode] size of progressive jpeg bigger than allowed !")
TRACE_MSG(JPEG_INTERFACE_923_112_2_18_1_41_13_922,"[IMGJPEG_Decode] JPG_DEC_TYPE_SUPPORT_BASELINE_PROGRESSIVE undefined !")
TRACE_MSG(JPEG_INTERFACE_937_112_2_18_1_41_13_923,"[IMGJPEG_Decode] Exit IMGJPEG_Decode. nRet = %d, time = %d")
TRACE_MSG(JPEG_INTERFACE_1012_112_2_18_1_41_13_924,"IMGJPEG_Decode_Res(), line %d, state = %d, in_param_ptr = 0x%x, out_param_ptr = 0x%x")
TRACE_MSG(JPEG_INTERFACE_1044_112_2_18_1_41_14_925,"[_DecodeRes] convert STD JPG to SJPG")
TRACE_MSG(JPEG_INTERFACE_1053_112_2_18_1_41_14_926,"[_DecodeRes] convert std jpeg to sjpg failed!")
TRACE_MSG(JPEG_INTERFACE_1067_112_2_18_1_41_14_927,"[_DecodeRes] jpeg buf = 0x%x, jpeg_size = %d")
TRACE_MSG(JPEG_INTERFACE_1108_112_2_18_1_41_14_928,"[IMGJPEG] Exit IMGJPEG_Decode_Res. nRet = %d, time = %d")
TRACE_MSG(JPEG_INTERFACE_1175_112_2_18_1_41_14_929,"[IMGJPEG_EnableMiniature] enable = %d")
TRACE_MSG(JPEG_INTERFACE_1178_112_2_18_1_41_14_930,"[IMGJPEG_EnableMiniature] JPG_MINIATURE_SUPPORT not defined")
TRACE_MSG(JPEG_ISR_157_112_2_18_1_41_15_931,"[JPEG_BSM_intHandler] encode drop the BSM INT after VLC done")
TRACE_MSG(JPEG_MIDWARE_171_112_2_18_1_41_17_932,"[JPEG_SetCaller] caller_flag = %d")
TRACE_MSG(JPEG_MIDWARE_192_112_2_18_1_41_17_933,"[JPEG_IspLock] lock isp failed")
TRACE_MSG(JPEG_MIDWARE_233_112_2_18_1_41_17_934,"[JPEG_IspOpen] ISP_ServiceOpen timeout")
TRACE_MSG(JPEG_MIDWARE_267_112_2_18_1_41_17_935,"[JPEG_OpenIspService] is_open = %d")
TRACE_MSG(JPEG_MIDWARE_298_112_2_18_1_41_17_936,"[JPEG_CloseIspService] is_open = %d")
TRACE_MSG(JPEG_MIDWARE_429_112_2_18_1_41_17_937,"[JPEG_DecInitScaleSliceBuf], allcate scale buffer failed")
TRACE_MSG(JPEG_MIDWARE_446_112_2_18_1_41_17_938,"[JPEG_DecInitScaleSliceBuf], allcate scale swap buffer failed")
TRACE_MSG(JPEG_MIDWARE_462_112_2_18_1_41_17_939,"[JPEG_DecInitScaleSliceBuf], allcate scale swap buffer failed")
TRACE_MSG(JPEG_MIDWARE_546_112_2_18_1_41_18_940,"[JPEG_DecScaleInit], allcate line buffer and scale mem failed")
TRACE_MSG(JPEG_MIDWARE_575_112_2_18_1_41_18_941,"[JPEG_DecScaleInit] YUV data type not supported")
TRACE_MSG(JPEG_MIDWARE_633_112_2_18_1_41_18_942,"[JPEG_InitSoftwareScaleBuf] YUV data type not supported")
TRACE_MSG(JPEG_MIDWARE_654_112_2_18_1_41_18_943,"[JPEG_DecInitScaleSliceBuf], allcate scale y buffer failed")
TRACE_MSG(JPEG_MIDWARE_660_112_2_18_1_41_18_944,"[JPEG_DecInitScaleSliceBuf], allcate scale u buffer failed")
TRACE_MSG(JPEG_MIDWARE_734_112_2_18_1_41_18_945,"[JPEG_DecSubsampleSlice] unaligned slice height %d")
TRACE_MSG(JPEG_MIDWARE_746_112_2_18_1_41_18_946,"[JPEG_DecSubsampleSlice] unaligned slice height %d")
TRACE_MSG(JPEG_MIDWARE_757_112_2_18_1_41_18_947,"[JPEG_DecSubsampleSlice] unsupport subsample factor = %d")
TRACE_MSG(JPEG_MIDWARE_917_112_2_18_1_41_18_948,"[JPEG_DecWriteYUV] unsupport YUV format, %d")
TRACE_MSG(JPEG_MIDWARE_1030_112_2_18_1_41_19_949,"[JPEG_DecYUVToRgb565] unsupporte target sample format")
TRACE_MSG(JPEG_MIDWARE_1045_112_2_18_1_41_19_950,"[JPEG_DecYUVToRgb565] error! yuv2rgb failed !target_sample_format = %d, actual write lines = %d")
TRACE_MSG(JPEG_MIDWARE_1114_112_2_18_1_41_19_951,"[JPEG_DecYUVToArgb8888] unsupporte target sample format")
TRACE_MSG(JPEG_MIDWARE_1130_112_2_18_1_41_19_952,"[JPEG_DecYUVToArgb8888] error! yuv2rgb failed !target_sample_format = %d, actual write lines = %d")
TRACE_MSG(JPEG_MIDWARE_1179_112_2_18_1_41_19_953,"[JPEG_DecGetSubsampleFactor] unsupport sample format %d")
TRACE_MSG(JPEG_MIDWARE_1231_112_2_18_1_41_19_954,"[JPEG_DecGetSubsampleFactor] revise scaling shift = %d")
TRACE_MSG(JPEG_MIDWARE_1306_112_2_18_1_41_19_955,"[JPEG_DecGetSoftSubsampleFactor] warning !! total_subsample_factor (%d) < hardware_subsample_factor (%d)")
TRACE_MSG(JPEG_MIDWARE_1386_112_2_18_1_41_19_956,"[JPEG_GetOutDataType] yuv data type not supported!")
TRACE_MSG(JPEG_MIDWARE_1578_112_2_18_1_41_20_957,"[JPEG_CalcSliceHeight] error return, memory not enough")
TRACE_MSG(JPEG_MIDWARE_1601_112_2_18_1_41_20_958,"[JPEG_CalcSliceHeight] error, memory not enough")
TRACE_MSG(JPEG_MIDWARE_1611_112_2_18_1_41_20_959,"[JPEG_CalcSliceHeight] slice_height = %d")
TRACE_MSG(JPEG_MIDWARE_1629_112_2_18_1_41_20_960,"[JPEG_DecScaleCallback], scale failed, scaled_output_lines = %d")
TRACE_MSG(JPEG_MIDWARE_1638_112_2_18_1_41_20_961,"[JPEG_DecScaleCallback], scale failed, scaled_output_lines = %d")
TRACE_MSG(JPEG_MIDWARE_1688_112_2_18_1_41_20_962,"[JPEG_DecSetHardwareScaleParam] do not need scaling down")
TRACE_MSG(JPEG_MIDWARE_1695_112_2_18_1_41_20_963,"[JPEG_DecSetHardwareScaleParam] init hardware scale failed")
TRACE_MSG(JPEG_MIDWARE_1732_112_2_18_1_41_20_964,"[JPEG_DecSetHardwareScaleParam] resize the dst size = (%d X %d)")
TRACE_MSG(JPEG_MIDWARE_1741_112_2_18_1_41_20_965,"[JPEG_DecSetHardwareScaleParam] error dst size (%d, %d)")
TRACE_MSG(JPEG_MIDWARE_1762_112_2_18_1_41_20_966,"[JPEG_DecSetHardwareScaleParam] unsupported input data format %d")
TRACE_MSG(JPEG_MIDWARE_1809_112_2_18_1_41_20_967,"[JPEG_DecSetHardwareScaleParam] error scaling factor, src (%d, %d), dst (%d, %d)")
TRACE_MSG(JPEG_MIDWARE_1850_112_2_18_1_41_20_968,"[JPEG_DecCalculateParam], JPEG_DecScaleInit failed = %d")
TRACE_MSG(JPEG_MIDWARE_1894_112_2_18_1_41_20_969,"[JPEG_DecCalculateParam] need soft scale down, output size = (%d, %d)")
TRACE_MSG(JPEG_MIDWARE_1917_112_2_18_1_41_21_970,"[JPEG_DecSetSoftwareScaleParam] unsupport sample format %d")
TRACE_MSG(JPEG_MIDWARE_1946_112_2_18_1_41_21_971,"[JPEG_DecSetSoftwareScaleParam] invalid crop size, src rect (%dX%d), crop size (%dX%d)")
TRACE_MSG(JPEG_MIDWARE_2075_112_2_18_1_41_21_972,"[JPEG_DecSetWorkMode] work mode = %d")
TRACE_MSG(JPEG_MIDWARE_2145_112_2_18_1_41_21_973,"[JPEG_DecCalculateParam] unsupport scaling down factor")
TRACE_MSG(JPEG_MIDWARE_2162_112_2_18_1_41_21_974,"[JPEG_DecCalculateParam] total factor = %d, hard factor = %d, soft factor = %d, mode = %d")
TRACE_MSG(JPEG_MIDWARE_2247_112_2_18_1_41_21_975,"[JPEG_DecCalculateParam] subsampled slice height is not enough, %d")
TRACE_MSG(JPEG_MIDWARE_2310_112_2_18_1_41_21_976,"yuv format %d -> unsupported input format")
TRACE_MSG(JPEG_MIDWARE_2324_112_2_18_1_41_21_977,"[JPEG_InitSubsampledBuffer] alloc subsampled yuv buffer failed")
TRACE_MSG(JPEG_MIDWARE_2358_112_2_18_1_41_21_978,"[JPEG_DecInitSliceStreamBuffer] alloc stream pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_2373_112_2_18_1_41_21_979,"[JPEG_DecInitSliceStreamBuffer] alloc stream pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_2390_112_2_18_1_41_21_980,"[JPEG_Decode_SC6600L] alloc stream pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_2404_112_2_18_1_41_22_981,"[JPEG_DecInitSliceStreamBuffer], file in buffer, buf = 0x%x, size = %d")
TRACE_MSG(JPEG_MIDWARE_2459_112_2_18_1_41_22_982,"yuv format %d -> unsupported input format")
TRACE_MSG(JPEG_MIDWARE_2483_112_2_18_1_41_22_983,"[JPEG_DecInitSliceYUVBuffer] alloc yuv pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_2502_112_2_18_1_41_22_984,"[JPEG_DecInitSliceYUVBuffer] alloc yuv pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_2540_112_2_18_1_41_22_985,"[JPEG_DecInitDecBuffer] alloc dec buffer failed")
TRACE_MSG(JPEG_MIDWARE_2570_112_2_18_1_41_22_986,"[JPEG_DecInitTargetBuffer] alloc convert slice buffer failed")
TRACE_MSG(JPEG_MIDWARE_2594_112_2_18_1_41_22_987,"[JPEG_DecInitTargetBuffer] unsupport target rgb type")
TRACE_MSG(JPEG_MIDWARE_2654_112_2_18_1_41_22_988,"[JPEG_DecInitTargetBuffer] alloc target rgb565 buffer failed")
TRACE_MSG(JPEG_MIDWARE_2678_112_2_18_1_41_22_989,"[JPEG_DecInitTargetBuffer] alloc rgb565 slice buffer failed")
TRACE_MSG(JPEG_MIDWARE_2732_112_2_18_1_41_22_990,"[JPEG_DecInitTargetBuffer] alloc target y buffer failed")
TRACE_MSG(JPEG_MIDWARE_2739_112_2_18_1_41_22_991,"[JPEG_DecInitTargetBuffer] alloc target uv buffer failed")
TRACE_MSG(JPEG_MIDWARE_2756_112_2_18_1_41_22_992,"[JPEG_DecInitTargetBuffer] is_frame_mode = %d")
TRACE_MSG(JPEG_MIDWARE_2767_112_2_18_1_41_22_993,"[JPEG_DecInitTargetBuffer] alloc convert slice buffer failed")
TRACE_MSG(JPEG_MIDWARE_2779_112_2_18_1_41_22_994,"[JPEG_DecInitTargetBuffer] unsupport target data type")
TRACE_MSG(JPEG_MIDWARE_2817_112_2_18_1_41_22_995,"[JPEG_DecInitFwParam] unsupport scaling down factor, use default")
TRACE_MSG(JPEG_MIDWARE_2896_112_2_18_1_41_22_996,"[JPEG_Decode_SC6600L] decode success, out_w = %d, out_h = %d, rgb buf = 0x%x")
TRACE_MSG(JPEG_MIDWARE_2910_112_2_18_1_41_23_997,"[JPEG_Decode_SC6600L] decode success, format = %d, out_w = %d, out_h = %d, out_y_addr = 0x%x, out_u_addr = 0x%x")
TRACE_MSG(JPEG_MIDWARE_2930_112_2_18_1_41_23_998,"[JPEG_InitializeHardware]")
TRACE_MSG(JPEG_MIDWARE_2942_112_2_18_1_41_23_999,"[JPEG_ReleaseHardware] s_is_vsp_open = %d")
TRACE_MSG(JPEG_MIDWARE_3063_112_2_18_1_41_23_1000,"JPEG_SyncHalDecode failed = %d")
TRACE_MSG(JPEG_MIDWARE_3101_112_2_18_1_41_23_1001,"[JPEG_StartDecode] DecodeInit/DecEncSwitchInit failed = %d")
TRACE_MSG(JPEG_MIDWARE_3119_112_2_18_1_41_23_1002,"[JPEG_StartDecode] JPEG_HWParseHead failed = %d")
TRACE_MSG(JPEG_MIDWARE_3286_112_2_18_1_41_23_1003,"[JPEG_DecodeWritePingPongBuffer] no stream in stream buffer, read size = %d, decoded size = %d, buf_id = %d")
TRACE_MSG(JPEG_MIDWARE_3322_112_2_18_1_41_23_1004,"[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed")
TRACE_MSG(JPEG_MIDWARE_3358_112_2_18_1_41_23_1005,"[JPEG_HardwareScaleYUV] scaled height reaches the output height. %d")
TRACE_MSG(JPEG_MIDWARE_3388_112_2_18_1_41_24_1006,"[JPEG_HardwareScaleYUV] unsupport input yuv format")
TRACE_MSG(JPEG_MIDWARE_3436_112_2_18_1_41_24_1007,"[JPEG_HardwareScaleYUV] error: input slice line is zero, return")
TRACE_MSG(JPEG_MIDWARE_3479_112_2_18_1_41_24_1008,"[JPEG_IsHardwareScaleSupport] hardware scaling unsupported!in size (%d, %d), out size (%d, %d)")
TRACE_MSG(JPEG_MIDWARE_3801_112_2_18_1_41_24_1009,"[JPEG_OutputYUV]unsupport sample format %d, use default value")
TRACE_MSG(JPEG_MIDWARE_3894_112_2_18_1_41_25_1010,"[JPEG_DecProcYUV] JPEG_ScaleYUV failed!ouput size (%d, %d), format = %d ")
TRACE_MSG(JPEG_MIDWARE_4005_112_2_18_1_41_25_1011,"[JPEG_DecodeErrorRestart] error restart, offset = %d, error_counter = %d, switch count = %d, dec_stream_size = %d")
TRACE_MSG(JPEG_MIDWARE_4037_112_2_18_1_41_25_1012,"[IMGJPEG_Decode] img size error, w = %d, h = %d")
TRACE_MSG(JPEG_MIDWARE_4045_112_2_18_1_41_25_1013,"[IMGJPEG_Decode] target img size error, w = %d, h = %d")
TRACE_MSG(JPEG_MIDWARE_4076_112_2_18_1_41_25_1014,"[JPEG_HardwareDecode] change the src size to the mcu aligned size")
TRACE_MSG(JPEG_MIDWARE_4152_112_2_18_1_41_25_1015,"[IMGJPEG_Decode] unsupported target data type !")
TRACE_MSG(JPEG_MIDWARE_4162_112_2_18_1_41_25_1016,"[JPEG_HardwareDecode] temp buf, addr = 0x%x, size  = %d, used size = %d")
TRACE_MSG(JPEG_MIDWARE_4263_112_2_18_1_41_25_1017,"[JPEG_HalGetDecodeBufSize] unsupport subsample factor = %d")
TRACE_MSG(JPEG_MIDWARE_4304_112_2_18_1_41_26_1018,"[JPEG_HalGetDecodeBufSize] src size = (%d, %d), crop rect = (%d, %d, %d, %d), mcu_height = %d")
TRACE_MSG(JPEG_MIDWARE_4308_112_2_18_1_41_26_1019,"[JPEG_HalGetDecodeBufSize] slice height = %d, aligned size = (%d, %d), subsample_factor = %d, hal_subsample_factor = %d, soft_subsample_factor = %d")
TRACE_MSG(JPEG_MIDWARE_4345_112_2_18_1_41_26_1020,"[JPEG_HalGetDecodeBufSize], temp_buf_size = %d")
TRACE_MSG(JPEG_MIDWARE_4356_112_2_18_1_41_26_1021,"[JPEG_HalGetDecodeBufSize] encode temp buffer = %d")
TRACE_MSG(JPEG_MIDWARE_4367_112_2_18_1_41_26_1022,"[JPEG_HalGetDecodeBufSize] temp buf size = %d, ret = %d")
TRACE_MSG(JPEG_MIDWARE_4481_112_2_18_1_41_26_1023,"JPEG_HWDecInit failed = %d")
TRACE_MSG(JPEG_MIDWARE_4613_112_2_18_1_41_26_1024,"[JPEG_EncReadYUVEx] read yuv return 0, total read lines = %d")
TRACE_MSG(JPEG_MIDWARE_4796_112_2_18_1_41_27_1025,"[JPEG_EncReadYUV] unsupport YUV format, %d, use default value")
TRACE_MSG(JPEG_MIDWARE_4938_112_2_18_1_41_27_1026,"[JPEG_EncSetReadFunc] change read func to JPEG_EncReadYUVEx")
TRACE_MSG(JPEG_MIDWARE_5048_112_2_18_1_41_27_1027,"[JPEG_EncInitParam] unsupport encode size (%d X %d)")
TRACE_MSG(JPEG_MIDWARE_5068_112_2_18_1_41_27_1028,"[JPEG_EncInitParam] src size (%d, %d), mcu aligned size (%d, %d), slice height = %d, mode = %d, work_mode = %d")
TRACE_MSG(JPEG_MIDWARE_5091_112_2_18_1_41_27_1029,"[JPEG_EncInitAlignBuffer] allocate yuv align buffer failed!")
TRACE_MSG(JPEG_MIDWARE_5133_112_2_18_1_41_27_1030,"[JPEG_EncInitReadYUVBuffer] allocate yuv pingpong buffer failed!")
TRACE_MSG(JPEG_MIDWARE_5207_112_2_18_1_41_27_1031,"[JPEG_EncInitSliceYUVBuffer] alloc yuv pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_5225_112_2_18_1_41_27_1032,"[JPEG_EncInitSliceYUVBuffer] alloc yuv pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_5259_112_2_18_1_41_27_1033,"[JPEG_EncInitTargetBuffer] allocate target buffer failed, encode memory not enough")
TRACE_MSG(JPEG_MIDWARE_5333_112_2_18_1_41_28_1034,"[JPEG_EncInitSliceStreamBuffer] allocate stream pingpong buffer failed, encode memory not enough")
TRACE_MSG(JPEG_MIDWARE_5349_112_2_18_1_41_28_1035,"[JPEG_EncInitSliceStreamBuffer] need copy to target = %d")
TRACE_MSG(JPEG_MIDWARE_5513_112_2_18_1_41_28_1036,"[JPEG_HalEncode]src_width = %d, src_height = %d, quality = %d")
TRACE_MSG(JPEG_MIDWARE_5517_112_2_18_1_41_28_1037,"[JPEG_HalEncode] src yuv pattern = %d, src yuv format = %d")
TRACE_MSG(JPEG_MIDWARE_5522_112_2_18_1_41_28_1038,"[JPEG_HalEncode]src y buf = 0x%x, src u buf size = 0x%x, src v buf size = 0x%x")
TRACE_MSG(JPEG_MIDWARE_5531_112_2_18_1_41_28_1039,"JPEG_SyncHalEncode failed = %d")
TRACE_MSG(JPEG_MIDWARE_5542_112_2_18_1_41_28_1040,"[JPEG_Encode_SC6600L] output jpeg buf = 0x%x, size = %d")
TRACE_MSG(JPEG_MIDWARE_5720_112_2_18_1_41_28_1041,"JPEG_HWEncInit failed = %d")
TRACE_MSG(JPEG_MIDWARE_5729_112_2_18_1_41_28_1042,"JPEG_HWWriteHead failed = %d")
TRACE_MSG(JPEG_MIDWARE_5816_112_2_18_1_41_29_1043,"[JPEG_StartEncode] JPEG_EncodeInit failed!")
TRACE_MSG(JPEG_MIDWARE_5908_112_2_18_1_41_29_1044,"[JPEG_StopEncode] jpeg stream buffer is not enough, stream size = %d, target buffer size = %d")
TRACE_MSG(JPEG_MIDWARE_5921_112_2_18_1_41_29_1045,"[JPEG_StopEncode] last size(%d) > stream buffer size(%d)")
TRACE_MSG(JPEG_MIDWARE_6063_112_2_18_1_41_29_1046,"[JPEG_EncodeReadPingPongBuffer] jpeg stream buffer is not enough")
TRACE_MSG(JPEG_MIDWARE_6114_112_2_18_1_41_29_1047,"[JPEG_SwitchWriteYUV] error! backup lines is not ZERO (%d)")
TRACE_MSG(JPEG_MIDWARE_6195_112_2_18_1_41_29_1048,"[JPEG_SwitchEncReadYUV] unsupport sample format, %d, use default value")
TRACE_MSG(JPEG_MIDWARE_6227_112_2_18_1_41_29_1049,"[JPEG_SwitchReadYUV] no more lines")
TRACE_MSG(JPEG_MIDWARE_6387_112_2_18_1_41_30_1050,"JPEG_SyncHalDecode failed = %d")
TRACE_MSG(JPEG_MIDWARE_6398_112_2_18_1_41_30_1051,"[JPEG_HalDecEncSwitch] create miniature success, output buf = 0x%x, size = %d, w = %d, h = %d")
TRACE_MSG(JPEG_MIDWARE_6473_112_2_18_1_41_30_1052,"[IMGJPEG_GetInfo] stream in file = 0x%x, read buf = 0x%x, size = %d")
TRACE_MSG(JPEG_MIDWARE_6480_112_2_18_1_41_30_1053,"[IMGJPEG_GetInfo] stream in buffer = 0x%x, size = %d")
TRACE_MSG(JPEG_MIDWARE_6488_112_2_18_1_41_30_1054,"[JPEG_CreateMiniature], Jpeg_parser failed, return = %d")
TRACE_MSG(JPEG_MIDWARE_6496_112_2_18_1_41_30_1055,"[JPEG_CreateMiniature] progressive file")
TRACE_MSG(JPEG_MIDWARE_6504_112_2_18_1_41_30_1056,"[JPEG_CreateMiniature] img size error, w = %d, h = %d")
TRACE_MSG(JPEG_MIDWARE_6513_112_2_18_1_41_30_1057,"[JPEG_CreateMiniature] target img size error, w = %d, h = %d")
TRACE_MSG(JPEG_MIDWARE_6599_112_2_18_1_41_30_1058,"[JPEG_CreateMiniature] memory not enough, enc temp buf(%d) >= dec temp buf (%d)")
TRACE_MSG(JPEG_MIDWARE_6625_112_2_18_1_41_30_1059,"[JPEG_HalDecEncSwitch] dec temp buf, addr = 0x%x, size  = %d, used size = %d")
TRACE_MSG(JPEG_MIDWARE_6627_112_2_18_1_41_30_1060,"[JPEG_HalDecEncSwitch] enc temp buf, addr = 0x%x, size  = %d, used size = %d")
TRACE_MSG(JPEG_MIDWARE_6657_112_2_18_1_41_30_1061,"[JPEG_YUV2RGBCallback], addr.y_addr = %d, size = %dX%d")
TRACE_MSG(JPEG_MIDWARE_6692_112_2_18_1_41_30_1062,"[JPEG_YUV2RGBHardware] invalid input parameters")
TRACE_MSG(JPEG_MIDWARE_6702_112_2_18_1_41_30_1063,"[JPEG_YUV2RGBHardware] invalid input parameters")
TRACE_MSG(JPEG_MIDWARE_6712_112_2_18_1_41_30_1064,"[JPEG_YUV2RGBHardware] trim rect invalid (%d, %d)")
TRACE_MSG(JPEG_MIDWARE_6771_112_2_18_1_41_30_1065,"[JPEG_YUV2RGBHardware], dst format error %d")
TRACE_MSG(JPEG_MIDWARE_6778_112_2_18_1_41_30_1066,"[JPEG_YUV2RGBHardware], dst buffer size is not enough, %d, %d")
TRACE_MSG(JPEG_MIDWARE_6788_112_2_18_1_41_30_1067,"[JPEG_YUV2RGBHardware] open isp failed!")
TRACE_MSG(JPEG_MIDWARE_6813_112_2_18_1_41_30_1068,"[JPEG_YUV2RGBHardware] convert rgb888 to argb888, argb888 buf = 0x%x, size (%d, %d)")
TRACE_MSG(JPEG_MIDWARE_6823_112_2_18_1_41_30_1069,"[JPEG_YUV2RGBHardware] convert rgb565 to argb888, argb888 buf = 0x%x, rgb565 buf = 0x%x, size (%d, %d)")
TRACE_MSG(JPEG_MIDWARE_7050_112_2_18_1_41_31_1070,"[JPEG_Decode_Res] unsupported sample format %d")
TRACE_MSG(JPEG_MIDWARE_7142_112_2_18_1_41_31_1071,"[JPEG_Decode_Res_SC6600L] alloc target y, and v buffer failed")
TRACE_MSG(JPEG_MIDWARE_7161_112_2_18_1_41_31_1072,"[JPEG_Decode_Res_SC6600L] alloc target RGB buffer failed")
TRACE_MSG(JPEG_MIDWARE_7185_112_2_18_1_41_31_1073,"JPEG_HWParseHead failed = %d")
TRACE_MSG(JPEG_MIDWARE_7196_112_2_18_1_41_31_1074,"[JPEG_Decode_Res_SC6600L] JPEG_HWDecInit failed, %d")
TRACE_MSG(JPEG_MIDWARE_7219_112_2_18_1_41_31_1075,"[JPEG_Decode_Res_SC6600L], eRet = %d, output_ptr = 0x%x, size = %d, output_width = %d, output_height = %d, actual_width = %d, actual_height = %d")
TRACE_MSG(JPEG_MIDWARE_7223_112_2_18_1_41_31_1076,"[JPEG_Decode_Res_SC6600L] JPEG_HWDecStartSynchro, %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_136_112_2_18_1_41_32_1077,"[JPEG_DecScaleInit], allcate line buffer and scale mem failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_345_112_2_18_1_41_32_1078,"[JPEG_DecInitScaleSliceBuf], allcate scale y buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_351_112_2_18_1_41_32_1079,"[JPEG_DecInitScaleSliceBuf], allcate scale u buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_530_112_2_18_1_41_33_1080,"[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_589_112_2_18_1_41_33_1081,"[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_778_112_2_18_1_41_33_1082,"[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_837_112_2_18_1_41_33_1083,"[JPEG_DecSubsampleWriteYUVSlice] subsample slice failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1244_112_2_18_1_41_34_1084,"[JPEG_DecGetSubsampleFactor] revise scaling shift = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1514_112_2_18_1_41_34_1085,"[JPEG_EncReadYUVSlice] read lines = %d, src_height = %d, src_align_height")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1530_112_2_18_1_41_35_1086,"[JPEG_EncReadYUVSlice] read lines = %d, fill up the dummy lines = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1679_112_2_18_1_41_35_1087,"[JPEG_EncInitSliceYUVBuffer] allocate yuv pingpong buffer failed!")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1699_112_2_18_1_41_35_1088,"[JPEG_EncInitSliceYUVBuffer] allocate yuv read buffer failed!")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1741_112_2_18_1_41_35_1089,"[JPEG_EncInitTargetBuffer] allocate target buffer failed, encode memory not enough")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1803_112_2_18_1_41_35_1090,"[JPEG_EncInitSliceStreamBuffer] allocate stream pingpong buffer failed, encode memory not enough")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1960_112_2_18_1_41_35_1091,"[JPEG_CalcSliceHeight] error return, temp buf size (%d) < used_buf_size (%d)")
TRACE_MSG(JPEG_MIDWARE_SC6600L_1998_112_2_18_1_41_35_1092,"[JPEG_CalcSliceHeight] slice_height = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2006_112_2_18_1_41_35_1093,"[JPEG_CalcSliceHeight] error return, memory not enough")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2064_112_2_18_1_41_36_1094,"[JPEG_DecCalculateParam] unsupport scaling down factor")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2078_112_2_18_1_41_36_1095,"[JPEG_DecCalculateParam] total factor = %d, hard factor = %d, soft factor = %d, mode = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2158_112_2_18_1_41_36_1096,"[JPEG_DecCalculateParam], JPEG_DecScaleInit failed = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2202_112_2_18_1_41_36_1097,"[JPEG_DecCalculateParam] need soft scale down, output size = (%d, %d)")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2278_112_2_18_1_41_36_1098,"[JPEG_DecCalculateParam] subsampled slice height is not enough, %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2346_112_2_18_1_41_36_1099,"[JPEG_InitSubsampledBuffer] alloc subsampled yuv buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2376_112_2_18_1_41_36_1100,"[JPEG_DecInitSliceStreamBuffer] alloc stream pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2391_112_2_18_1_41_36_1101,"[JPEG_DecInitSliceStreamBuffer] alloc stream pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2408_112_2_18_1_41_36_1102,"[JPEG_Decode_SC6600L] alloc stream pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2422_112_2_18_1_41_36_1103,"[JPEG_DecInitSliceStreamBuffer], file in buffer, buf = 0x%x, size = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2439_112_2_18_1_41_36_1104,"[JPEG_DecInitSliceStreamBuffer], empty jpeg, no stream read in ping buffer, exit")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2447_112_2_18_1_41_36_1105,"[JPEG_DecInitSliceStreamBuffer] jpeg stream size less than jpeg stream buffer ,reset the stream buffer size to %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2465_112_2_18_1_41_36_1106,"[JPEG_DecInitSliceStreamBuffer], no stream read in pong buffer, total read size = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2526_112_2_18_1_41_36_1107,"[JPEG_DecInitSliceYUVBuffer] alloc y pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2542_112_2_18_1_41_36_1108,"[JPEG_DecInitSliceYUVBuffer] alloc uv pingpong buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2559_112_2_18_1_41_37_1109,"[JPEG_DecInitSliceYUVBuffer] alloc rotated yuv422 buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2594_112_2_18_1_41_37_1110,"[JPEG_DecInitDecBuffer] alloc dec buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2655_112_2_18_1_41_37_1111,"[JPEG_DecInitTargetBuffer] alloc target rgb565 buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2669_112_2_18_1_41_37_1112,"[JPEG_DecInitTargetBuffer] alloc rgb565 slice buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2720_112_2_18_1_41_37_1113,"[JPEG_DecInitTargetBuffer] alloc target y buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2727_112_2_18_1_41_37_1114,"[JPEG_DecInitTargetBuffer] alloc target uv buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2911_112_2_18_1_41_37_1115,"JPEG_SyncHalEncode failed = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2922_112_2_18_1_41_37_1116,"[JPEG_Encode_SC6600L] output jpeg buf = 0x%x, size = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_2997_112_2_18_1_41_37_1117,"JPEG_SyncHalDecode failed = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3095_112_2_18_1_41_38_1118,"[JPEG_Decode_Res_SC6600L] alloc target RGB buffer failed")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3164_112_2_18_1_41_38_1119,"[JPEG_Decode_Res] slice height = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3183_112_2_18_1_41_38_1120,"[JPEG_Decode_Res_SC6600L] JPEG_HWDecStartSynchro, %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3214_112_2_18_1_41_38_1121,"JPEG_HWEncInit failed = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3221_112_2_18_1_41_38_1122,"JPEG_HWWriteHead failed = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3273_112_2_18_1_41_38_1123,"[JPEG_StopEncode] jpeg stream buffer is not enough, stream size = %d, target buffer size = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3485_112_2_18_1_41_38_1124,"[JPEG_EncodeReadPingPongBufferFirst] jpeg stream buffer is not enough")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3516_112_2_18_1_41_38_1125,"[JPEG_EncodeReadPingPongBuffer] jpeg stream buffer is not enough")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3587_112_2_18_1_41_39_1126,"JPEG_HWDecInit failed = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3594_112_2_18_1_41_39_1127,"JPEG_HWParseHead failed = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3757_112_2_18_1_41_39_1128,"[JPEG_DecodeWritePingPongBufferFirst] read ping first")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3763_112_2_18_1_41_39_1129,"[JPEG_DecodeWritePingPongBufferFirst] read pong first")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3795_112_2_18_1_41_39_1130,"[JPEG_DecodeWritePingPongBufferFirst] no stream in stream buffer, read size = %d, decoded size = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3852_112_2_18_1_41_39_1131,"[JPEG_DecodeWritePingPongBuffer] no stream in stream buffer, read size = %d, decoded size = %d, buf_id = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3988_112_2_18_1_41_39_1132,"[IMGJPEG_Decode] img size error, w = %d, h = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_3996_112_2_18_1_41_39_1133,"[IMGJPEG_Decode] target img size error, w = %d, h = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_4108_112_2_18_1_41_40_1134,"[JPEG_CreateMiniature] dec buff = 0x%x, size = %d, target buf = 0x%x, size = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_4113_112_2_18_1_41_40_1135,"[JPEG_CreateMiniature] decode buffer not enough, %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_4135_112_2_18_1_41_40_1136,"[JPEG_CreateMiniature] decode to yuv failed !")
TRACE_MSG(JPEG_MIDWARE_SC6600L_4152_112_2_18_1_41_40_1137,"[JPEG_CreateMiniature] encode to miniature failed !")
TRACE_MSG(JPEG_MIDWARE_SC6600L_4286_112_2_18_1_41_40_1138,"[JPEG_HalGetDecodeBufSize] src size = (%d, %d), crop rect = (%d, %d, %d, %d), mcu_height = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_4290_112_2_18_1_41_40_1139,"[JPEG_HalGetDecodeBufSize] slice height = %d, aligned size = (%d, %d), subsample_factor = %d, hal_subsample_factor = %d, soft_subsample_factor = %d")
TRACE_MSG(JPEG_MIDWARE_SC6600L_4332_112_2_18_1_41_40_1140,"[JPEG_HalGetDecodeBufSize] temp buf size = %d, ret = %d")
TRACE_MSG(JPEG_SCALING_INIT_20_112_2_18_1_41_43_1141,"Can not allocate enough memory!\n")
TRACE_MSG(JPEG_SCALING_INIT_37_112_2_18_1_41_43_1142,"[SCALE_MALLOC]Can not allocate enough memory!\n")
TRACE_MSG(JPEG_SCALING_INIT_132_112_2_18_1_41_43_1143,"unknown data type!\n")
TRACE_MSG(JPEG_SCALING_INIT_170_112_2_18_1_41_43_1144,"unknown data type!\n")
TRACE_MSG(JPEG_SCALING_INIT_237_112_2_18_1_41_43_1145,"does not support!\n")
TRACE_MSG(JPEG_SCALING_INIT_357_112_2_18_1_41_44_1146,"Warning: The times of scaling is more than 7, I am sorry WinSinc method can not support now!\n")
TRACE_MSG(JPEG_SCALING_INIT_358_112_2_18_1_41_44_1147,"You can choose the Box average method to scaling it! Thanks!\n")
TRACE_MSG(JPEG_SCALING_INIT_390_112_2_18_1_41_44_1148,"[Scale_Initialize]in w = %d, in h = %d, out w = %d, out h = %d")
TRACE_MSG(JPEG_SCALING_INIT_423_112_2_18_1_41_44_1149,"the ratio of width and height is not proper")
TRACE_MSG(JPEG_SCALING_INIT_459_112_2_18_1_41_44_1150,"Scale factor is %d/%d\n\n")
TRACE_MSG(JPEG_SCALING_INIT_498_112_2_18_1_41_44_1151,"[Scale_Initialize], N = %d or M = %d > BASE_COFF = %d ")
TRACE_MSG(JPEG_SCALING_INIT_504_112_2_18_1_41_44_1152,"[Scale_Initialize], Init_Data_Type_adv failed ")
TRACE_MSG(JPEG_SCALING_INIT_509_112_2_18_1_41_44_1153,"[Scale_Initialize], Init_COEEF_Table_adv failed ")
TRACE_MSG(JPEG_SCALING_INIT_514_112_2_18_1_41_44_1154,"[Scale_Initialize], Init_Scale_Fun_adv failed ")
TRACE_MSG(JPEG_SCALING_INIT_529_112_2_18_1_41_44_1155,"[Scale_Initialize], allocate slice_ctrl.copy_line_ptr failed ")
TRACE_MSG(JPEG_SCALING_INIT_539_112_2_18_1_41_44_1156,"[Scale_Initialize], allocate rgb_buf failed ")
TRACE_MSG(JPEG_SCALING_INIT_551_112_2_18_1_41_44_1157,"[Scale_Initialize], allocate uv buffer failed ")
TRACE_MSG(JPEG_SCALING_INIT_597_112_2_18_1_41_44_1158,"[Scale_GetScalingDownSize], src size (%d, %d), target size (%d, %d), mode = %d")
TRACE_MSG(JPEG_SCALING_INIT_653_112_2_18_1_41_44_1159,"[Scale_GetScalingDownSize], output size (%d, %d), N / M = %d / %d")
TRACE_MSG(JPEG_STREAM_40_112_2_18_1_41_46_1160,"[Jpeg_ReadStream] warning ! stream_left_size = %d !")
TRACE_MSG(JPEG_STREAM_60_112_2_18_1_41_46_1161,"[Jpeg_ReadStream] stream in file, no more bytes")
TRACE_MSG(JPEG_STREAM_77_112_2_18_1_41_46_1162,"[Jpeg_ReadStream] stream in buffer, no more bytes")
TRACE_MSG(JPEG_STREAM_81_112_2_18_1_41_46_1163,"[Jpeg_ReadStream] stream in buffer, stream_ptr = 0x%x, stream_left_size = %d")
TRACE_MSG(JPEG_STREAM_87_112_2_18_1_41_46_1164,"[Jpeg_ReadStream] no source found !")
TRACE_MSG(JPEG_STREAM_132_112_2_18_1_41_46_1165,"[Jpeg_GetStreamPos] error of stream ptr")
TRACE_MSG(JPEG_STREAM_163_112_2_18_1_41_46_1166,"[Jpeg_SetStreamPos] error of stream ptr")
TRACE_MSG(JPEG_STREAM_235_112_2_18_1_41_47_1167,"[Jpeg_GetC] no more bytes !")
TRACE_MSG(JPEG_STREAM_262_112_2_18_1_41_47_1168,"[Jpegd_GetW] no more bytes !")
TRACE_MSG(JPEG_STREAM_290_112_2_18_1_41_47_1169,"[Jpegd_GetW] no more bytes !")
TRACE_MSG(JPEG_STREAM_321_112_2_18_1_41_47_1170,"[Jpegd_GetUINT32] no more bytes !")
TRACE_MSG(JPEG_STREAM_354_112_2_18_1_41_47_1171,"[Jpegd_GetUINT32] no more bytes !")
TRACE_MSG(JPEG_TASK_129_112_2_18_1_41_48_1172,"jpeg_task, JPEG_Task_Open, task_id = %d")
TRACE_MSG(JPEG_TASK_133_112_2_18_1_41_48_1173,"jpeg_task, JPEG TASK is existed,task_id = %d")
TRACE_MSG(JPEG_TASK_175_112_2_18_1_41_48_1174,"jpeg_task, JPEG_SIG_DEC_START")
TRACE_MSG(JPEG_TASK_178_112_2_18_1_41_48_1175,"jpeg_task, JPEG_StartDecode error")
TRACE_MSG(JPEG_TASK_186_112_2_18_1_41_48_1176,"jpeg_task, JPEG_SIG_DEC_END")
TRACE_MSG(JPEG_TASK_209_112_2_18_1_41_48_1177,"jpeg_task, JPEG_SIG_ENC_START")
TRACE_MSG(JPEG_TASK_212_112_2_18_1_41_48_1178,"jpeg_task, JPEG_StartEncode error")
TRACE_MSG(JPEG_TASK_220_112_2_18_1_41_48_1179,"jpeg_task, JPEG_SIG_ENC_END")
TRACE_MSG(JPEG_TASK_252_112_2_18_1_41_48_1180,"jpeg_task, JPEG_SIG_WRITE_PING_PONG_BUF_FIRST error")
TRACE_MSG(JPEG_TASK_273_112_2_18_1_41_48_1181,"jpeg_task, JPEG_SIG_READ_PING_PONG_BUF_FIRST error")
TRACE_MSG(JPEG_TASK_294_112_2_18_1_41_48_1182,"jpeg_task, JPEG_SIG_WRITE_PING_PONG_BUF error")
TRACE_MSG(JPEG_TASK_315_112_2_18_1_41_48_1183,"jpeg_task, JPEG_SIG_READ_PING_PONG_BUF error")
TRACE_MSG(JPEG_TASK_324_112_2_18_1_41_48_1184,"jpeg_task, JPEG_SIG_ERROR_RESTART")
TRACE_MSG(JPEG_TASK_332_112_2_18_1_41_48_1185,"jpeg_task, JPEG_SIG_ERROR_RESTART error")
TRACE_MSG(JPEG_TASK_341_112_2_18_1_41_48_1186,"jpeg_task, JPEG_SIG_OVERTIME")
TRACE_MSG(JPEG_TASK_352_112_2_18_1_41_49_1187,"jpeg_task, JPEG_SIG_ERROR")
TRACE_MSG(JPEG_TASK_364_112_2_18_1_41_49_1188,"jpeg_task, sig JPEG_SIG_ACTIVE,line %d")
TRACE_MSG(JPEG_TASK_395_112_2_18_1_41_49_1189,"jpeg_task, forced to stop,line %d, freed signal = %d")
TRACE_MSG(JPEG_TASK_402_112_2_18_1_41_49_1190,"jpeg_task, invalid sig code to JPEG_Task_Entry,line %d")
TRACE_MSG(JPEG_TASK_520_112_2_18_1_41_49_1191,"jpeg_task, decoding event error, event flag = 0x%x")
TRACE_MSG(JPEG_TASK_564_112_2_18_1_41_49_1192,"jpeg_task, decoding event error, event flag = 0x%x")
TRACE_MSG(JPEG_TASK_606_112_2_18_1_41_49_1193,"jpeg_task, decoding event error, event flag = 0x%x")
TRACE_MSG(JPEG_TASK_609_112_2_18_1_41_49_1194,"jpeg_task, after SCI_GetEvent(),event_flag=%lu,line %d")
TRACE_MSG(JPEG_TASK_631_112_2_18_1_41_49_1195,"JPEG_WaitTaskStopped, g_jpeg_event_ptr is null")
TRACE_MSG(JPEG_TASK_649_112_2_18_1_41_49_1196,"JPEG_WaitTaskStopped, wait task stopped time out")
TRACE_MSG(JPEG_TASK_652_112_2_18_1_41_49_1197,"JPEG_WaitTaskStopped, after SCI_GetEvent(),event_flag=%lu,line %d")
TRACE_MSG(JPEG_TASK_749_112_2_18_1_41_49_1198,"[JPEG_TimeOutHandler] jpeg time out but task no-active")
TRACE_MSG(JPEG_TASK_762_112_2_18_1_41_49_1199,"jpeg_task,JPEG_Task_Close,task_id = %lu")
TRACE_MSG(JPEG_TASK_794_112_2_18_1_41_49_1200,"jpeg_task,JPEG_Task_Close failed, g_jpeg_task_id = 0")
TRACE_MSG(DCAMERA_JPG_CODEC_70_112_2_18_1_41_52_1201,"Jpeg_IRAM_Disable:failed to unlock IRAM")
TRACE_MSG(DCAMERA_JPG_CODEC_196_112_2_18_1_41_52_1202,"--%s--,line %d")
TRACE_MSG(DCAMERA_JPG_CODEC_297_112_2_18_1_41_52_1203,"--%s--,line %d")
TRACE_MSG(JPEG_119_112_2_18_1_41_53_1204,"dcam: IMG_JPEG_DecodeJpeg - data type %d")
TRACE_MSG(PE_PROCESS_3362_112_2_18_1_42_12_1205,"[pe_data_sub_sample], width = %d, subsample = %d, num_of_line = %d")
TRACE_MSG(PE_PROCESS_3427_112_2_18_1_42_12_1206,"[pe_get_data], width = %d, height = %d, line = %d, src buf = 0x%x")
TRACE_MSG(PE_PROCESS_3502_112_2_18_1_42_12_1207,"[PE_Scalingdown] src size =(%d, %d), target size = (%d, %d)")
TRACE_MSG(PE_PROCESS_3570_112_2_18_1_42_12_1208,"[PE_Scalingdown] is_sub_sample = %d, sub_sample = %d")
TRACE_MSG(PE_PROCESS_3593_112_2_18_1_42_13_1209,"[PE_Scalingdown] scaling input width = %d, height = %d")
TRACE_MSG(PE_PROCESS_3731_112_2_18_1_42_13_1210,"[PE_SaveJpeg], size = (%d, %d), process buf = 0x%x, size = %d")
TRACE_MSG(PE_PROCESS_3732_112_2_18_1_42_13_1211,"[PE_SaveJpeg], convert buf = 0x%x, convert_buf_size = %d")
TRACE_MSG(PE_PROCESS_3760_112_2_18_1_42_13_1212,"[pe_process.c] SaveJpeg successed! ")
TRACE_MSG(PE_PROCESS_3770_112_2_18_1_42_13_1213,"[pe_process.c] process buf is not enough, %d! ")
TRACE_MSG(PE_PROCESS_3839_112_2_18_1_42_13_1214,"---PEPROCESS_ImageProcess:input image width or height is 0---")
TRACE_MSG(PE_PROCESS_4152_112_2_18_1_42_14_1215,"PE Process type is default!")
TRACE_MSG(PE_PROCESS_4156_112_2_18_1_42_14_1216,"------PE PROCESS TIME:%d------")
TRACE_MSG(INFLATE_790_112_2_18_1_42_18_1217,"[PNG]img_inflate:run default of switch!")
TRACE_MSG(PNG_65_112_2_18_1_42_20_1218,"---PNG ERR INFO---Too many bytes for PNG signature.")
TRACE_MSG(PNG_117_112_2_18_1_42_20_1219,"---PNG WARNING--Potential overflow in png_zalloc()")
TRACE_MSG(PNG_297_112_2_18_1_42_20_1220,"---PNG WARNING--Unknown freer parameter in png_data_freer_0.")
TRACE_MSG(PNG_APP_261_112_2_18_1_42_22_1221,"png 0 target %d, %d, src %d, %d, sample %d, %d")
TRACE_MSG(PNG_APP_399_112_2_18_1_42_22_1222,"PNGAPP_Decode: scrwidth: %d scrheight: %d target width :%d target height: %d")
TRACE_MSG(PNG_APP_455_112_2_18_1_42_22_1223,"image_orgin_width:%d,%d,%d,%d")
TRACE_MSG(PNG_APP_CTRL_284_112_2_18_1_42_23_1224,"PNGDEC: Receive Sig=0x%x, param addr=0x%x , cur time=%d")
TRACE_MSG(PNG_APP_CTRL_330_112_2_18_1_42_23_1225,"_PNGDEC_HandleRoutine:Png decode is finish time=%d")
TRACE_MSG(PNG_APP_CTRL_370_112_2_18_1_42_23_1226,"_PNGDEC_ReadFile:read file error!")
TRACE_MSG(PNG_APP_CTRL_443_112_2_18_1_42_24_1227,"PNGDEC_FormatConvert:data format is invailid,%d")
TRACE_MSG(PNG_APP_CTRL_678_112_2_18_1_42_24_1228,"[IMGDECpng_Dec] bytes_written = %d")
TRACE_MSG(PNG_APP_CTRL_682_112_2_18_1_42_24_1229,"[IMGDECpng_Dec] create file failed")
TRACE_MSG(PNG_APP_CTRL_685_112_2_18_1_42_24_1230,"IMGDECpng_Dec, target_ptr= %x, jpeg_size= %d")
TRACE_MSG(PNG_APP_CTRL_686_112_2_18_1_42_24_1231,"IMGDECpng_Dec, target_width = %d, target_height = %d")
TRACE_MSG(PNG_APP_CTRL_687_112_2_18_1_42_24_1232,"IMGDECpng_Dec,out_param.real_width = %d, out_param.real_height = %d")
TRACE_MSG(PNG_APP_CTRL_891_112_2_18_1_42_24_1233,"IMGDECPNG_GetFrame:handle=0x%x,callback=0x%x,time=%d")
TRACE_MSG(PNG_APP_CTRL_897_112_2_18_1_42_24_1234,"IMGDECPNG_GetFrame:data format is error! %d")
TRACE_MSG(PNG_APP_CTRL_991_112_2_18_1_42_25_1235,"IMGDECPNG_GetFrame,handle=0x%x,out_time=%d")
TRACE_MSG(PNG_APP_CTRL_1014_112_2_18_1_42_25_1236,"IMGDECPNG_Destroy:handle=0x%x,time = %d")
TRACE_MSG(PNG_DECODE_184_112_2_18_1_42_26_1237,"Failed to create PNG structure")
TRACE_MSG(PNG_DECODE_193_112_2_18_1_42_26_1238,"Failed to initialize PNG info structure")
TRACE_MSG(PNG_DECODE_258_112_2_18_1_42_26_1239,"[PNG DEC:alpha buffer is small!]")
TRACE_MSG(PNG_DECODE_285_112_2_18_1_42_26_1240,"img_png_decode_destroy: enter into destroy \n")
TRACE_MSG(PNG_DECODE_335_112_2_18_1_42_26_1241,"img_png_decode_destroy: s_png_ptr is PNULL \n")
TRACE_MSG(PNG_DECODE_341_112_2_18_1_42_26_1242,"img_png_decode_destroy: enter out destroy \n")
TRACE_MSG(PNG_DECODE_532_112_2_18_1_42_26_1243,"[_noscale_row_frame_palette: ERROR],color_table_ptr is NULL!")
TRACE_MSG(PNG_DECODE_791_112_2_18_1_42_27_1244,"[_noscale_row_frame_palette: ERROR],color_table_ptr is NULL!")
TRACE_MSG(PNG_DECODE_1330_112_2_18_1_42_28_1245,"Do not support format")
TRACE_MSG(PNG_DECODE_1434_112_2_18_1_42_28_1246,"Do not support format")
TRACE_MSG(PNG_DECODE_2210_112_2_18_1_42_30_1247,"Do not support format")
TRACE_MSG(PNG_DECODE_2318_112_2_18_1_42_30_1248,"Do not support format")
TRACE_MSG(PNG_DECODE_2593_112_2_18_1_42_30_1249,"[_noscale_rowgroup_palette ERROR]:color_table_ptr is NULL!")
TRACE_MSG(PNG_DECODE_2612_112_2_18_1_42_30_1250,"s_png_ptr->row_info.pixel_depth %d")
TRACE_MSG(PNG_DECODE_2879_112_2_18_1_42_31_1251,"[_noscale_rowgroup_palette ERROR]:color_table_ptr is NULL!")
TRACE_MSG(PNG_DECODE_2899_112_2_18_1_42_31_1252,"s_png_ptr->row_info.pixel_depth %d")
TRACE_MSG(PNG_DECODE_3584_112_2_18_1_42_32_1253,"Do not support format")
TRACE_MSG(PNG_DECODE_3687_112_2_18_1_42_33_1254,"Do not support format")
TRACE_MSG(PNG_DECODE_3929_112_2_18_1_42_33_1255,"out into _decode_row_group ")
TRACE_MSG(PNG_DECODE_4121_112_2_18_1_42_33_1256,"[_row_group_palette_argb ERROR]:color_table_ptr is NULL!")
TRACE_MSG(PNG_DECODE_4626_112_2_18_1_42_34_1257,"Do not support format")
TRACE_MSG(PNG_DECODE_4745_112_2_18_1_42_35_1258,"Do not support format")
TRACE_MSG(PNG_DECODE_4957_112_2_18_1_42_35_1259,"out into _decode_row_group ")
TRACE_MSG(PNG_DECODE_5247_112_2_18_1_42_36_1260,"test Get Data ")
TRACE_MSG(PNG_DECODE_5251_112_2_18_1_42_36_1261,"-----PNG Get Data Time :%d------")
TRACE_MSG(PNGGET_515_112_2_18_1_42_37_1262,"---PNG ERR INFO---Invalid bit depth")
TRACE_MSG(PNGGET_521_112_2_18_1_42_37_1263,"---PNG ERR INFO---Invalid color type")
TRACE_MSG(PNGGET_534_112_2_18_1_42_37_1264,"---PNG ERR INFO---Invalid image width")
TRACE_MSG(PNGGET_539_112_2_18_1_42_37_1265,"---PNG ERR INFO---Invalid image height")
TRACE_MSG(PNGGET_549_112_2_18_1_42_38_1266,"---PNG WARNING---Width too large for libpng to process image data.")
TRACE_MSG(PNGMEM_167_112_2_18_1_42_39_1267,"---PNG ERR INFO---Overflow in png_memcpy_check.")
TRACE_MSG(PNGMEM_182_112_2_18_1_42_39_1268,"---PNG ERR INFO---Overflow in png_memset_check.")
TRACE_MSG(PNGPREAD_156_112_2_18_1_42_39_1269,"---PNG ERR INFO---Not a PNG file")
TRACE_MSG(PNGPREAD_161_112_2_18_1_42_39_1270,"---PNG ERR INFO---PNG file corrupted by ASCII conversion")
TRACE_MSG(PNGPREAD_292_112_2_18_1_42_39_1271,"---PNG ERR INFO---Missing IHDR before IDAT")
TRACE_MSG(PNGPREAD_298_112_2_18_1_42_39_1272,"---PNG ERR INFO---Missing PLTE before IDAT")
TRACE_MSG(PNGPREAD_321_112_2_18_1_42_40_1273,"---PNG ERR INFO---Missing IHDR before IDAT")
TRACE_MSG(PNGPREAD_327_112_2_18_1_42_40_1274,"---PNG ERR INFO---Missing PLTE before IDAT")
TRACE_MSG(PNGPREAD_338_112_2_18_1_42_40_1275,"---PNG ERR INFO---Too many IDAT's found")
TRACE_MSG(PNGPREAD_660_112_2_18_1_42_40_1276,"---PNG ERR INFO---Potential overflow of save_buffer")
TRACE_MSG(PNGPREAD_722_112_2_18_1_42_40_1277,"---PNG ERR INFO---Not enough compressed data")
TRACE_MSG(PNGPREAD_738_112_2_18_1_42_40_1278,"---PNG ERR INFO---save_size overflowed in pngpread")
TRACE_MSG(PNGPREAD_761_112_2_18_1_42_40_1279,"---PNG ERR INFO---save_size overflowed in pngpread")
TRACE_MSG(PNGPREAD_798_112_2_18_1_42_40_1280,"---PNG ERR INFO---Extra compression data")
TRACE_MSG(PNGPREAD_813_112_2_18_1_42_40_1281,"---PNG ERR INFO---Extra compressed data")
TRACE_MSG(PNGPREAD_829_112_2_18_1_42_41_1282,"---PNG ERR INFO---Decompression Error")
TRACE_MSG(PNGPREAD_844_112_2_18_1_42_41_1283,"---PNG WARNING---Too much data in IDAT chunks")
TRACE_MSG(PNGPREAD_1019_112_2_18_1_42_41_1284,"[img_png_push_process_row:run default of swith]")
TRACE_MSG(PNGPREAD_1115_112_2_18_1_42_41_1285,"---PNG ERR INFO---Out of place tEXt")
TRACE_MSG(PNGPREAD_1126_112_2_18_1_42_41_1286,"---PNG WARNING---tEXt chunk too large to fit in memory")
TRACE_MSG(PNGPREAD_1199_112_2_18_1_42_41_1287,"---PNG WARNING---Insufficient memory to store text chunk.")
TRACE_MSG(PNGPREAD_1210_112_2_18_1_42_41_1288,"---PNG ERR INFO---Out of place zTXt")
TRACE_MSG(PNGPREAD_1223_112_2_18_1_42_41_1289,"---PNG WARNING---zTXt chunk too large to fit in memory")
TRACE_MSG(PNGPREAD_1389_112_2_18_1_42_42_1290,"---PNG WARNING---Insufficient memory to store text chunk.")
TRACE_MSG(PNGPREAD_1401_112_2_18_1_42_42_1291,"---PNG ERR INFO---Out of place iTXt")
TRACE_MSG(PNGPREAD_1412_112_2_18_1_42_42_1292,"---PNG WARNING---iTXt chunk too large to fit in memory")
TRACE_MSG(PNGPREAD_1500_112_2_18_1_42_42_1293,"---PNG WARNING---Insufficient memory to store iTXt chunk.")
TRACE_MSG(PNGPREAD_1525_112_2_18_1_42_42_1294,"---PNG ERR INFO---unknown critical chunk")
TRACE_MSG(PNGPREAD_1541_112_2_18_1_42_42_1295,"---PNG WARNING---unknown chunk too large to fit in memory")
TRACE_MSG(PNGPREAD_1561_112_2_18_1_42_42_1296,"---PNG ERR INFO---unknown critical chunk")
TRACE_MSG(PNGREAD_144_112_2_18_1_42_43_1297,"--PNG ERR Info--Incompatible libpng version in application and library")
TRACE_MSG(PNGREAD_161_112_2_18_1_42_43_1298,"---PNG ERR INFO---zlib memory error")
TRACE_MSG(PNGREAD_165_112_2_18_1_42_43_1299,"---PNG ERR INFO---zlib version error")
TRACE_MSG(PNGREAD_170_112_2_18_1_42_43_1300,"---PNG ERR INFO---Unknown zlib error")
TRACE_MSG(PNGREAD_227_112_2_18_1_42_43_1301,"---PNG ERR INFO---The png struct allocated by the application for reading is too small.")
TRACE_MSG(PNGREAD_236_112_2_18_1_42_43_1302,"---PNG ERR INFO---The info struct allocated by application for reading is too small.")
TRACE_MSG(PNGREAD_261_112_2_18_1_42_43_1303,"---PNG WARNING---Application uses deprecated png_read_init_0() and should be recompiled.")
TRACE_MSG(PNGREAD_308_112_2_18_1_42_43_1304,"---PNG ERR INFO---zlib memory")
TRACE_MSG(PNGREAD_312_112_2_18_1_42_43_1305,"---PNG ERR INFO---zlib version")
TRACE_MSG(PNGREAD_317_112_2_18_1_42_43_1306,"---PNG ERR INFO---Unknown zlib error")
TRACE_MSG(PNGREAD_354_112_2_18_1_42_43_1307,"---PNG ERR INFO---Not a PNG file")
TRACE_MSG(PNGREAD_359_112_2_18_1_42_43_1308,"---PNG ERR INFO---PNG file corrupted by ASCII conversion")
TRACE_MSG(PNGREAD_490_112_2_18_1_42_43_1309,"---PNG ERR INFO---Missing IHDR before IDAT")
TRACE_MSG(PNGREAD_497_112_2_18_1_42_43_1310,"---PNG ERR INFO---Missing PLTE before IDAT")
TRACE_MSG(PNGREAD_511_112_2_18_1_42_43_1311,"---PNG ERR INFO---Missing IHDR before IDAT")
TRACE_MSG(PNGREAD_518_112_2_18_1_42_43_1312,"---PNG ERR INFO---Missing PLTE before IDAT")
TRACE_MSG(PNGREAD_607_112_2_18_1_42_43_1313,"---PNG WARNING---Ignoring extra png_read_update_info() call; row buffer not reallocated")
TRACE_MSG(PNGREAD_638_112_2_18_1_42_44_1314,"---PNG WARNING---PNG_READ_INVERT_SUPPORTED is not defined.")
TRACE_MSG(PNGREAD_642_112_2_18_1_42_44_1315,"---PNG WARNING---PNG_READ_FILLER_SUPPORTED is not defined.")
TRACE_MSG(PNGREAD_646_112_2_18_1_42_44_1316,"---PNG WARNING---PNG_READ_PACKSWAP_SUPPORTED is not defined.")
TRACE_MSG(PNGREAD_650_112_2_18_1_42_44_1317,"---PNG WARNING---PNG_READ_PACK_SUPPORTED is not defined.")
TRACE_MSG(PNGREAD_654_112_2_18_1_42_44_1318,"---PNG WARNING---PNG_READ_SHIFT_SUPPORTED is not defined.")
TRACE_MSG(PNGREAD_658_112_2_18_1_42_44_1319,"---PNG WARNING---PNG_READ_BGR_SUPPORTED is not defined.")
TRACE_MSG(PNGREAD_662_112_2_18_1_42_44_1320,"---PNG WARNING---PNG_READ_SWAP_SUPPORTED is not defined.")
TRACE_MSG(PNGREAD_740_112_2_18_1_42_44_1321,"[img_png_read_row:run default of switch!]")
TRACE_MSG(PNGREAD_748_112_2_18_1_42_44_1322,"---PNG ERR INFO---Invalid attempt to read row data")
TRACE_MSG(PNGREAD_769_112_2_18_1_42_44_1323,"---PNG ERR INFO---Not enough image data")
TRACE_MSG(PNGREAD_787_112_2_18_1_42_44_1324,"---PNG ERR INFO---Extra compressed data")
TRACE_MSG(PNGREAD_797_112_2_18_1_42_44_1325,"--PNG ERR Info--Decompression error")
TRACE_MSG(PNGREAD_953_112_2_18_1_42_44_1326,"---PNG ERR INFO---Cannot read interlaced image -- interlace handler disabled.")
TRACE_MSG(PNGREAD_1089_112_2_18_1_42_44_1327,"---PNG ERR INFO---Too many IDAT's found")
TRACE_MSG(PNGREAD_1107_112_2_18_1_42_44_1328,"---PNG ERR INFO---Too many IDAT's found")
TRACE_MSG(PNGREAD_1426_112_2_18_1_42_45_1329,"---PNG ERR INFO---Image is too high to process with png_read_png()")
TRACE_MSG(PNGRIO_65_112_2_18_1_42_46_1330,"[img_png_read_data:ERR0]")
TRACE_MSG(PNGRIO_72_112_2_18_1_42_46_1331,"[img_png_read_data:END0]")
TRACE_MSG(PNGRIO_83_112_2_18_1_42_46_1332,"[img_png_read_data:ERR1]")
TRACE_MSG(PNGRIO_89_112_2_18_1_42_46_1333,"[img_png_read_data:END1]")
TRACE_MSG(PNGRIO_118_112_2_18_1_42_46_1334,"[img_png_read_data:ERR2]")
TRACE_MSG(PNGRIO_124_112_2_18_1_42_46_1335,"[img_png_read_data:END2]")
TRACE_MSG(PNGRIO_134_112_2_18_1_42_46_1336,"[img_png_read_data:ERR3]")
TRACE_MSG(PNGRIO_140_112_2_18_1_42_46_1337,"[img_png_read_data:END3]")
TRACE_MSG(PNGRIO_211_112_2_18_1_42_46_1338,"---PNG WARNING---It's an error to set both read_data_fn and write_data_fn in the ")
TRACE_MSG(PNGRIO_212_112_2_18_1_42_46_1339,"---PNG WARNING---same structure.  Resetting write_data_fn to NULL.")
TRACE_MSG(PNGRTRAN_38_112_2_18_1_42_46_1340,"---PNG WARNING---Can't discard critical data on CRC error.")
TRACE_MSG(PNGRTRAN_80_112_2_18_1_42_46_1341,"---PNG WARNING---Application must supply a known background gamma")
TRACE_MSG(PNGRTRAN_611_112_2_18_1_42_48_1342,"[img_png_set_rgb_to_gray_fixed_0:run default of switch!]")
TRACE_MSG(PNGRTRAN_619_112_2_18_1_42_48_1343,"---PNG WARNING---Cannot do RGB_TO_GRAY without EXPAND_SUPPORTED.")
TRACE_MSG(PNGRTRAN_637_112_2_18_1_42_48_1344,"---PNG WARNING---ignoring out of range rgb_to_gray coefficients")
TRACE_MSG(PNGRTRAN_661_112_2_18_1_42_48_1345,"---PNG WARNING---This version of libpng does not support user transforms")
TRACE_MSG(PNGRTRAN_1158_112_2_18_1_42_49_1346,"---PNG ERR INFO---NULL row buffer")
TRACE_MSG(PNGRTRAN_1199_112_2_18_1_42_49_1347,"---PNG WARNING---png_do_rgb_to_gray found nongray pixel")
TRACE_MSG(PNGRTRAN_1202_112_2_18_1_42_49_1348,"---PNG ERR INFO---png_do_rgb_to_gray found nongray pixel")
TRACE_MSG(PNGRTRAN_1287_112_2_18_1_42_49_1349,"---PNG ERR INFO---png_do_dither returned rowbytes=0")
TRACE_MSG(PNGRTRAN_1458_112_2_18_1_42_50_1350,"[img_png_do_unpack:run default of switch!]")
TRACE_MSG(PNGRTRAN_1572_112_2_18_1_42_50_1351,"[img_png_do_unshift:run default of switch!]")
TRACE_MSG(PNGRTRAN_3427_112_2_18_1_42_54_1352,"[img_png_do_gamma:run default of switch!]")
TRACE_MSG(PNGRTRAN_3522_112_2_18_1_42_54_1353,"[img_png_do_expand_palette:run default of switch!]")
TRACE_MSG(PNGRTRAN_3576_112_2_18_1_42_54_1354,"[img_png_do_expand_palette:run default of switch!]")
TRACE_MSG(PNGRTRAN_3676_112_2_18_1_42_54_1355,"[img_png_do_expand:run default of switch!]")
TRACE_MSG(PNGRUTIL_77_112_2_18_1_42_55_1356,"---PNG ERR INFO---PNG unsigned integer out of range.\n")
TRACE_MSG(PNGRUTIL_146_112_2_18_1_42_56_1357,"---PNG ERR INFO---CRC error")
TRACE_MSG(PNGRUTIL_150_112_2_18_1_42_56_1358,"---PNG ERR INFO---CRC error")
TRACE_MSG(PNGRUTIL_233_112_2_18_1_42_56_1359,"---PNG ERR INFO---Not enough memory to decompress chunk")
TRACE_MSG(PNGRUTIL_260_112_2_18_1_42_56_1360,"---PNG ERR INFO---Not enough memory to decompress chunk.")
TRACE_MSG(PNGRUTIL_278_112_2_18_1_42_56_1361,"---PNG ERR INFO---Not enough memory to decompress chunk..")
TRACE_MSG(PNGRUTIL_309_112_2_18_1_42_56_1362,"---PNG WARNING---Incomplete compressed datastream in chunk other than IDAT")
TRACE_MSG(PNGRUTIL_318_112_2_18_1_42_56_1363,"---PNG ERR INFO---Not enough memory for text.")
TRACE_MSG(PNGRUTIL_337_112_2_18_1_42_56_1364,"--PNG WARNING--Unknown zTXt compression type")
TRACE_MSG(PNGRUTIL_359_112_2_18_1_42_56_1365,"---PNG ERR INFO---Out of place IHDR")
TRACE_MSG(PNGRUTIL_366_112_2_18_1_42_56_1366,"---PNG ERR INFO---Invalid IHDR chunk")
TRACE_MSG(PNGRUTIL_411_112_2_18_1_42_56_1367,"[img_png_handle_IHDR:run default of switch!]")
TRACE_MSG(PNGRUTIL_439_112_2_18_1_42_56_1368,"---PNG ERR INFO---Missing IHDR before PLTE")
TRACE_MSG(PNGRUTIL_444_112_2_18_1_42_56_1369,"---PNG WARNING---Invalid PLTE after IDAT")
TRACE_MSG(PNGRUTIL_450_112_2_18_1_42_56_1370,"---PNG ERR INFO---Duplicate PLTE chunk")
TRACE_MSG(PNGRUTIL_458_112_2_18_1_42_56_1371,"---PNG WARNING---Ignoring PLTE chunk in grayscale PNG")
TRACE_MSG(PNGRUTIL_474_112_2_18_1_42_56_1372,"---PNG WARNING---Invalid palette chunk")
TRACE_MSG(PNGRUTIL_480_112_2_18_1_42_56_1373,"---PNG ERR INFO---Invalid palette chunk")
TRACE_MSG(PNGRUTIL_529_112_2_18_1_42_56_1374,"---PNG ERR INFO---CRC error")
TRACE_MSG(PNGRUTIL_533_112_2_18_1_42_56_1375,"---PNG WARNING---CRC error")
TRACE_MSG(PNGRUTIL_540_112_2_18_1_42_56_1376,"---PNG WARNING---CRC error")
TRACE_MSG(PNGRUTIL_554_112_2_18_1_42_56_1377,"---PNG WARNING---Truncating incorrect tRNS chunk length")
TRACE_MSG(PNGRUTIL_559_112_2_18_1_42_56_1378,"---PNG WARNING---Truncating incorrect info tRNS chunk length")
TRACE_MSG(PNGRUTIL_574_112_2_18_1_42_56_1379,"---PNG ERR INFO---No image in file")
TRACE_MSG(PNGRUTIL_582_112_2_18_1_42_56_1380,"---PNG WARNING---Incorrect IEND chunk length")
TRACE_MSG(PNGRUTIL_603_112_2_18_1_42_57_1381,"---PNG ERR INFO---Missing IHDR before gAMA")
TRACE_MSG(PNGRUTIL_608_112_2_18_1_42_57_1382,"---PNG WARNING---Invalid gAMA after IDAT")
TRACE_MSG(PNGRUTIL_615_112_2_18_1_42_57_1383,"---PNG WARNING--Out of place gAMA chunk")
TRACE_MSG(PNGRUTIL_624_112_2_18_1_42_57_1384,"---PNG WARNING---Duplicate gAMA chunk")
TRACE_MSG(PNGRUTIL_631_112_2_18_1_42_57_1385,"---PNG WRANING---Incorrect gAMA chunk length")
TRACE_MSG(PNGRUTIL_644_112_2_18_1_42_57_1386,"---PNG WARNING---Ignoring gAMA chunk with gamma=0")
TRACE_MSG(PNGRUTIL_652_112_2_18_1_42_57_1387,"---PNG WARNING---Ignoring incorrect gAMA value when sRGB is also present")
TRACE_MSG(PNGRUTIL_685_112_2_18_1_42_57_1388,"---PNG ERR INFO---Missing IHDR before sBIT")
TRACE_MSG(PNGRUTIL_690_112_2_18_1_42_57_1389,"---PNG WARNING---Invalid sBIT after IDAT")
TRACE_MSG(PNGRUTIL_697_112_2_18_1_42_57_1390,"---PNG WARNING---Out of place sBIT chunk")
TRACE_MSG(PNGRUTIL_701_112_2_18_1_42_57_1391,"---PNG WARNING---Duplicate sBIT chunk")
TRACE_MSG(PNGRUTIL_713_112_2_18_1_42_57_1392,"---PNG WARNING---Incorrect sBIT chunk length")
TRACE_MSG(PNGRUTIL_757_112_2_18_1_42_57_1393,"---PNG ERR INFO---Missing IHDR before cHRM")
TRACE_MSG(PNGRUTIL_762_112_2_18_1_42_57_1394,"---PNG WARNING---Invalid cHRM after IDAT")
TRACE_MSG(PNGRUTIL_769_112_2_18_1_42_57_1395,"---PNG WARNING---Missing PLTE before cHRM")
TRACE_MSG(PNGRUTIL_778_112_2_18_1_42_57_1396,"---PNG WARNING---Duplicate cHRM chunk")
TRACE_MSG(PNGRUTIL_785_112_2_18_1_42_57_1397,"---PNG WARNING---Incorrect cHRM chunk length")
TRACE_MSG(PNGRUTIL_799_112_2_18_1_42_57_1398,"---PNG WARNING---Invalid cHRM white point")
TRACE_MSG(PNGRUTIL_815_112_2_18_1_42_57_1399,"---PNG WARNING---Invalid cHRM red point")
TRACE_MSG(PNGRUTIL_831_112_2_18_1_42_57_1400,"---PNG WARNING---Invalid cHRM green point")
TRACE_MSG(PNGRUTIL_847_112_2_18_1_42_57_1401,"---PNG WARNING---Invalid cHRM blue point")
TRACE_MSG(PNGRUTIL_878_112_2_18_1_42_57_1402,"---PNG WARNING---Ignoring incorrect cHRM value when sRGB is also present")
TRACE_MSG(PNGRUTIL_922_112_2_18_1_42_57_1403,"--PNG ERR Info--Missing IHDR before sRGB")
TRACE_MSG(PNGRUTIL_926_112_2_18_1_42_57_1404,"---PNG WARNING---Invalid sRGB after IDAT")
TRACE_MSG(PNGRUTIL_933_112_2_18_1_42_57_1405,"---PNG WARNING---Out of place sRGB chunk")
TRACE_MSG(PNGRUTIL_938_112_2_18_1_42_57_1406,"---PNG WARNING---Duplicate sRGB chunk")
TRACE_MSG(PNGRUTIL_945_112_2_18_1_42_57_1407,"---PNG WARNING---Incorrect sRGB chunk length")
TRACE_MSG(PNGRUTIL_958_112_2_18_1_42_57_1408,"---PNG WARNING---Unknown sRGB intent")
TRACE_MSG(PNGRUTIL_975_112_2_18_1_42_57_1409,"---PNG WARNING---Ignoring incorrect gAMA value when sRGB is also present")
TRACE_MSG(PNGRUTIL_1001_112_2_18_1_42_57_1410,"---PNG WARNING---Ignoring incorrect cHRM value when sRGB is also present")
TRACE_MSG(PNGRUTIL_1025_112_2_18_1_42_57_1411,"---PNG ERR INFO---Missing IHDR before iCCP")
TRACE_MSG(PNGRUTIL_1030_112_2_18_1_42_57_1412,"---PNG WARNING---Invalid iCCP after IDAT")
TRACE_MSG(PNGRUTIL_1037_112_2_18_1_42_57_1413,"---PNG WARNING---Out of place iCCP chunk")
TRACE_MSG(PNGRUTIL_1041_112_2_18_1_42_58_1414,"---PNG WARNING---Duplicate iCCP chunk")
TRACE_MSG(PNGRUTIL_1049_112_2_18_1_42_58_1415,"---PNG WARNING---iCCP chunk too large to fit in memory")
TRACE_MSG(PNGRUTIL_1077_112_2_18_1_42_58_1416,"---PNG WARNING---Malformed iCCP chunk")
TRACE_MSG(PNGRUTIL_1085_112_2_18_1_42_58_1417,"---PNG WARNING---Ignoring nonzero compression type in iCCP chunk")
TRACE_MSG(PNGRUTIL_1099_112_2_18_1_42_58_1418,"---PNG WARNING---Profile size field missing from iCCP chunk")
TRACE_MSG(PNGRUTIL_1116_112_2_18_1_42_58_1419,"---PNG WARNING---Ignoring truncated iCCP profile.\n")
TRACE_MSG(PNGRUTIL_1144_112_2_18_1_42_58_1420,"--PNG ERR Info--Missing IHDR before sPLT")
TRACE_MSG(PNGRUTIL_1148_112_2_18_1_42_58_1421,"---PNG WARNING---Invalid sPLT after IDAT")
TRACE_MSG(PNGRUTIL_1156_112_2_18_1_42_58_1422,"---PNG WARNING---sPLT chunk too large to fit in memory")
TRACE_MSG(PNGRUTIL_1182_112_2_18_1_42_58_1423,"---PNG WARNING---malformed sPLT chunk")
TRACE_MSG(PNGRUTIL_1194_112_2_18_1_42_58_1424,"---PNG WARNING---sPLT chunk has bad length")
TRACE_MSG(PNGRUTIL_1202_112_2_18_1_42_58_1425,"---PNG WARNING---sPLT chunk too long")
TRACE_MSG(PNGRUTIL_1209_112_2_18_1_42_58_1426,"---PNG WARNING---sPLT chunk requires too much memory")
TRACE_MSG(PNGRUTIL_1277_112_2_18_1_42_58_1427,"---PNG ERR INFO---Missing IHDR before tRNS")
TRACE_MSG(PNGRUTIL_1282_112_2_18_1_42_58_1428,"---PNG WARNING---Invalid tRNS after IDAT")
TRACE_MSG(PNGRUTIL_1288_112_2_18_1_42_58_1429,"---PNG WARNING---Duplicate tRNS chunk")
TRACE_MSG(PNGRUTIL_1297_112_2_18_1_42_58_1430,"---PNG WARNING---Incorrect tRNS chunk length")
TRACE_MSG(PNGRUTIL_1313_112_2_18_1_42_58_1431,"---PNG WARNING---Incorrect tRNS chunk length")
TRACE_MSG(PNGRUTIL_1329_112_2_18_1_42_58_1432,"---PNG WARNING---Missing PLTE before tRNS")
TRACE_MSG(PNGRUTIL_1334_112_2_18_1_42_58_1433,"---PNG WARNING---Incorrect tRNS chunk length")
TRACE_MSG(PNGRUTIL_1340_112_2_18_1_42_58_1434,"---PNG WARNING---Zero length tRNS chunk")
TRACE_MSG(PNGRUTIL_1349_112_2_18_1_42_58_1435,"---PNG WARNING---tRNS chunk not allowed with alpha channel")
TRACE_MSG(PNGRUTIL_1372_112_2_18_1_42_58_1436,"---PNG ERR INFO---Missing IHDR before bKGD")
TRACE_MSG(PNGRUTIL_1377_112_2_18_1_42_58_1437,"---PNG WARNING---Invalid bKGD after IDAT")
TRACE_MSG(PNGRUTIL_1384_112_2_18_1_42_58_1438,"---PNG WARNING---Missing PLTE before bKGD")
TRACE_MSG(PNGRUTIL_1390_112_2_18_1_42_58_1439,"---PNG WARNING---Duplicate bKGD chunk")
TRACE_MSG(PNGRUTIL_1404_112_2_18_1_42_58_1440,"---PNG WARNING---Incorrect bKGD chunk length")
TRACE_MSG(PNGRUTIL_1424_112_2_18_1_42_58_1441,"---PNG WARNING---Incorrect bKGD chunk index value")
TRACE_MSG(PNGRUTIL_1463_112_2_18_1_42_58_1442,"--PNG ERR Info--Missing IHDR before hIST")
TRACE_MSG(PNGRUTIL_1467_112_2_18_1_42_58_1443,"---PNG WARNING---Invalid hIST after IDAT")
TRACE_MSG(PNGRUTIL_1473_112_2_18_1_42_58_1444,"---PNG WARNING---Missing PLTE before hIST")
TRACE_MSG(PNGRUTIL_1479_112_2_18_1_42_58_1445,"---PNG WARNING---Duplicate hIST chunk")
TRACE_MSG(PNGRUTIL_1488_112_2_18_1_42_58_1446,"---PNG WARNING---Incorrect hIST chunk length")
TRACE_MSG(PNGRUTIL_1519_112_2_18_1_42_58_1447,"---PNG ERR INFO---Missing IHDR before pHYs")
TRACE_MSG(PNGRUTIL_1524_112_2_18_1_42_59_1448,"---PNG WARNING---Invalid pHYs after IDAT")
TRACE_MSG(PNGRUTIL_1530_112_2_18_1_42_59_1449,"---PNG_WARNING---Duplicate pHYs chunk")
TRACE_MSG(PNGRUTIL_1537_112_2_18_1_42_59_1450,"---PNG WARNING---Incorrect pHYs chunk length")
TRACE_MSG(PNGRUTIL_1564_112_2_18_1_42_59_1451,"--PNG ERR Info--Missing IHDR before oFFs")
TRACE_MSG(PNGRUTIL_1568_112_2_18_1_42_59_1452,"---PNG WARNING---Invalid oFFs after IDAT")
TRACE_MSG(PNGRUTIL_1574_112_2_18_1_42_59_1453,"---PNG WARNING---Duplicate oFFs chunk")
TRACE_MSG(PNGRUTIL_1581_112_2_18_1_42_59_1454,"---PNG WARNING---Incorrect oFFs chunk length")
TRACE_MSG(PNGRUTIL_1612_112_2_18_1_42_59_1455,"--PNG ERR Info--Missing IHDR before pCAL")
TRACE_MSG(PNGRUTIL_1616_112_2_18_1_42_59_1456,"---PNG WARNING---Invalid pCAL after IDAT")
TRACE_MSG(PNGRUTIL_1622_112_2_18_1_42_59_1457,"---PNG WARNING---Duplicate pCAL chunk")
TRACE_MSG(PNGRUTIL_1632_112_2_18_1_42_59_1458,"---PNG WARNING---No memory for pCAL purpose.")
TRACE_MSG(PNGRUTIL_1656_112_2_18_1_42_59_1459,"---PNG WARNING---Invalid pCAL data")
TRACE_MSG(PNGRUTIL_1676_112_2_18_1_42_59_1460,"---PNG WARNING---Invalid pCAL parameters for equation type")
TRACE_MSG(PNGRUTIL_1682_112_2_18_1_42_59_1461,"---PNG WARNING---Unrecognized equation type for pCAL chunk")
TRACE_MSG(PNGRUTIL_1694_112_2_18_1_42_59_1462,"---PNG WARNING---No memory for pCAL params.")
TRACE_MSG(PNGRUTIL_1710_112_2_18_1_42_59_1463,"---PNG WARNING---Invalid pCAL data")
TRACE_MSG(PNGRUTIL_1743_112_2_18_1_42_59_1464,"--PNG ERR Info--Missing IHDR before sCAL")
TRACE_MSG(PNGRUTIL_1747_112_2_18_1_42_59_1465,"---PNG WARNING---Invalid sCAL after IDAT")
TRACE_MSG(PNGRUTIL_1753_112_2_18_1_42_59_1466,"---PNG WARNING---Duplicate sCAL chunk")
TRACE_MSG(PNGRUTIL_1763_112_2_18_1_42_59_1467,"---PNG WARNING---Out of memory while processing sCAL chunk")
TRACE_MSG(PNGRUTIL_1783_112_2_18_1_42_59_1468,"---PNG WARNING---malformed width string in sCAL chunk")
TRACE_MSG(PNGRUTIL_1792_112_2_18_1_42_59_1469,"---PNG WARNING---Out of memory while processing sCAL chunk width")
TRACE_MSG(PNGRUTIL_1807_112_2_18_1_42_59_1470,"---PNG WARNING---malformed height string in sCAL chunk")
TRACE_MSG(PNGRUTIL_1815_112_2_18_1_42_59_1471,"---PNG WARNING---Out of memory while processing sCAL chunk height")
TRACE_MSG(PNGRUTIL_1828_112_2_18_1_42_59_1472,"---PNG WARNING---Invalid sCAL data")
TRACE_MSG(PNGRUTIL_1864_112_2_18_1_42_59_1473,"--PNG ERR Info--Out of place tIME chunk")
TRACE_MSG(PNGRUTIL_1868_112_2_18_1_42_59_1474,"---PNG WARNING---Duplicate tIME chunk")
TRACE_MSG(PNGRUTIL_1878_112_2_18_1_42_59_1475,"---PNG WARNING---Incorrect tIME chunk length")
TRACE_MSG(PNGRUTIL_1912_112_2_18_1_42_59_1476,"--PNG ERR Info--Missing IHDR before tEXt")
TRACE_MSG(PNGRUTIL_1921_112_2_18_1_42_59_1477,"---PNG WARNING---tEXt chunk too large to fit in memory")
TRACE_MSG(PNGRUTIL_1930_112_2_18_1_42_59_1478,"---PNG WARNING---No memory to process text chunk.")
TRACE_MSG(PNGRUTIL_1954_112_2_18_1_42_59_1479,"---PNG WARNING---Not enough memory to process text chunk.")
TRACE_MSG(PNGRUTIL_1974_112_2_18_1_42_59_1480,"---PNG WARNING---Insufficient memory to process text chunk.")
TRACE_MSG(PNGRUTIL_1992_112_2_18_1_43_0_1481,"---PNG ERR INFO---Missing IHDR before zTXt")
TRACE_MSG(PNGRUTIL_2003_112_2_18_1_43_0_1482,"---PNG WARNING---zTXt chunk too large to fit in memory")
TRACE_MSG(PNGRUTIL_2012_112_2_18_1_43_0_1483,"---PNG WARNING---Out of memory processing zTXt chunk.")
TRACE_MSG(PNGRUTIL_2032_112_2_18_1_43_0_1484,"---PNG WARNING---Zero length zTXt chunk")
TRACE_MSG(PNGRUTIL_2039_112_2_18_1_43_0_1485,"---PNG WARNING---Unknown compression type in zTXt chunk")
TRACE_MSG(PNGRUTIL_2053_112_2_18_1_43_0_1486,"---PNG WARNING---Not enough memory to process zTXt chunk.")
TRACE_MSG(PNGRUTIL_2073_112_2_18_1_43_0_1487,"--PNG ERR Info--Insufficient memory to store zTXt chunk.")
TRACE_MSG(PNGRUTIL_2093_112_2_18_1_43_0_1488,"--PNG ERR Info--Missing IHDR before iTXt")
TRACE_MSG(PNGRUTIL_2104_112_2_18_1_43_0_1489,"---PNG WARNING---iTXt chunk too large to fit in memory")
TRACE_MSG(PNGRUTIL_2113_112_2_18_1_43_0_1490,"---PNG WARNING---No memory to process iTXt chunk.")
TRACE_MSG(PNGRUTIL_2137_112_2_18_1_43_0_1491,"---PNG WARNING---Zero length iTXt chunk")
TRACE_MSG(PNGRUTIL_2165_112_2_18_1_43_0_1492,"---PNG WARNING---Not enough memory to process iTXt chunk.")
TRACE_MSG(PNGRUTIL_2183_112_2_18_1_43_0_1493,"--PNG ERR Info--Insufficient memory to store iTXt chunk.")
TRACE_MSG(PNGRUTIL_2222_112_2_18_1_43_0_1494,"---PNG ERR INFO---unknown critical chunk")
TRACE_MSG(PNGRUTIL_2235_112_2_18_1_43_0_1495,"---PNG WARNING---unknown chunk too large to fit in memory")
TRACE_MSG(PNGRUTIL_2255_112_2_18_1_43_0_1496,"---PNG ERR INFO---unknown critical chunk")
TRACE_MSG(PNGRUTIL_2291_112_2_18_1_43_0_1497,"---PNG ERR INFO---invalid chunk type")
TRACE_MSG(PNGRUTIL_2847_112_2_18_1_43_1_1498,"---PNG WARNING---Ignoring bad adaptive filter type")
TRACE_MSG(PNGRUTIL_2940_112_2_18_1_43_1_1499,"---PNG ERR INFO---Not enough image data")
TRACE_MSG(PNGRUTIL_2957_112_2_18_1_43_1_1500,"---PNG WARNING---Extra compressed data")
TRACE_MSG(PNGRUTIL_2966_112_2_18_1_43_2_1501,"--PNG ERR Info--Decompression Error")
TRACE_MSG(PNGRUTIL_2975_112_2_18_1_43_2_1502,"---PNG WARNING---Extra compressed data.")
TRACE_MSG(PNGRUTIL_2986_112_2_18_1_43_2_1503,"---PNG WARNING---Extra compression data")
TRACE_MSG(PNGRUTIL_3040_112_2_18_1_43_2_1504,"--PNG ERR Info--Rowbytes overflow in png_read_start_row")
TRACE_MSG(PNGRUTIL_3160_112_2_18_1_43_2_1505,"--PNG ERR Info--This image requires a row greater than 64KB")
TRACE_MSG(PNGRUTIL_3171_112_2_18_1_43_2_1506,"img_png_read_start_row: cano not allocate png_ptr->big_row_buf ")
TRACE_MSG(PNGRUTIL_3182_112_2_18_1_43_2_1507,"--PNG ERR Info--This image requires a row greater than 64KB")
TRACE_MSG(PNGRUTIL_3187_112_2_18_1_43_2_1508,"--PNG ERR Info--Row has too many bytes to allocate in memory.")
TRACE_MSG(PNGSET_46_112_2_18_1_43_2_1509,"---PNG WARNING---Ignoring attempt to set negative chromaticity value")
TRACE_MSG(PNGSET_54_112_2_18_1_43_2_1510,"---PNG WARNING---Ignoring attempt to set chromaticity value exceeding 21474.83")
TRACE_MSG(PNGSET_94_112_2_18_1_43_2_1511,"---PNG WARNING---Ignoring attempt to set negative chromaticity value")
TRACE_MSG(PNGSET_114_112_2_18_1_43_2_1512,"---PNG WARNING---Ignoring attempt to set chromaticity value exceeding 21474.83")
TRACE_MSG(PNGSET_152_112_2_18_1_43_3_1513,"---PNG WARNING---Limiting gamma to 21474.83")
TRACE_MSG(PNGSET_163_112_2_18_1_43_3_1514,"---PNG WARNING---Setting gamma=0")
TRACE_MSG(PNGSET_177_112_2_18_1_43_3_1515,"---PNG WARNING---Limiting gamma to 21474.83")
TRACE_MSG(PNGSET_184_112_2_18_1_43_3_1516,"---PNG WARNING---Setting negative gamma to zero")
TRACE_MSG(PNGSET_198_112_2_18_1_43_3_1517,"---PNG WARNING---Setting gamma=0")
TRACE_MSG(PNGSET_212_112_2_18_1_43_3_1518,"---PNG WARNING---Palette size 0, hIST allocation skipped.")
TRACE_MSG(PNGSET_224_112_2_18_1_43_3_1519,"---PNG WARNING---Insufficient memory for hIST chunk data.")
TRACE_MSG(PNGSET_253_112_2_18_1_43_3_1520,"--PNG ERR Info--Image width or height is zero in IHDR")
TRACE_MSG(PNGSET_258_112_2_18_1_43_3_1521,"--PNG ERR Info--image size exceeds user limits in IHDR")
TRACE_MSG(PNGSET_263_112_2_18_1_43_3_1522,"--PNG ERR Info--image size exceeds user limits in IHDR")
TRACE_MSG(PNGSET_268_112_2_18_1_43_3_1523,"--PNG ERR Info--Invalid image size in IHDR")
TRACE_MSG(PNGSET_276_112_2_18_1_43_3_1524,"---PNG WARNING---Width is too large for libpng to process pixels")
TRACE_MSG(PNGSET_282_112_2_18_1_43_3_1525,"--PNG ERR Info--Invalid bit depth in IHDR")
TRACE_MSG(PNGSET_288_112_2_18_1_43_3_1526,"--PNG ERR Info--Invalid color type in IHDR")
TRACE_MSG(PNGSET_296_112_2_18_1_43_3_1527,"--PNG ERR Info--Invalid color type/bit depth combination in IHDR")
TRACE_MSG(PNGSET_301_112_2_18_1_43_3_1528,"--PNG ERR Info--Unknown interlace method in IHDR")
TRACE_MSG(PNGSET_306_112_2_18_1_43_3_1529,"--PNG ERR Info--Unknown compression method in IHDR")
TRACE_MSG(PNGSET_321_112_2_18_1_43_3_1530,"---PNG WARNING---MNG features are not allowed in a PNG datastream\n")
TRACE_MSG(PNGSET_331_112_2_18_1_43_3_1531,"--PNG ERR Info--Unknown filter method in IHDR")
TRACE_MSG(PNGSET_335_112_2_18_1_43_3_1532,"---PNG WARNING---Invalid filter method in IHDR")
TRACE_MSG(PNGSET_341_112_2_18_1_43_3_1533,"--PNG ERR Info--Unknown filter method in IHDR")
TRACE_MSG(PNGSET_406_112_2_18_1_43_3_1534,"---PNG WARNING---Insufficient memory for pCAL purpose.")
TRACE_MSG(PNGSET_422_112_2_18_1_43_3_1535,"---PNG WARNING---Insufficient memory for pCAL units.")
TRACE_MSG(PNGSET_431_112_2_18_1_43_3_1536,"---PNG WARNING---Insufficient memory for pCAL params.")
TRACE_MSG(PNGSET_444_112_2_18_1_43_3_1537,"---PNG WARNING---Insufficient memory for pCAL parameter.")
TRACE_MSG(PNGSET_490_112_2_18_1_43_3_1538,"---PNG WARNING---Memory allocation failed while processing sCAL.")
TRACE_MSG(PNGSET_500_112_2_18_1_43_3_1539,"---PNG WARNING---Memory allocation failed while processing sCAL.")
TRACE_MSG(PNGSET_672_112_2_18_1_43_4_1540,"---PNG WARNING---Insufficient memory to process iCCP chunk.")
TRACE_MSG(PNGSET_680_112_2_18_1_43_4_1541,"---PNG WARNING---Insufficient memory to process iCCP profile.")
TRACE_MSG(PNGSET_707_112_2_18_1_43_4_1542,"--PNG ERR INFO--Insufficient memory to store text")
TRACE_MSG(PNGSET_791_112_2_18_1_43_4_1543,"---PNG WARNING---iTXt chunk not supported.")
TRACE_MSG(PNGSET_937_112_2_18_1_43_4_1544,"---PNG WARNING---No memory for sPLT palettes.")
TRACE_MSG(PNGSET_990_112_2_18_1_43_4_1545,"---PNG WARNING---Out of memory while processing unknown chunk.")
TRACE_MSG(PNGSET_1008_112_2_18_1_43_4_1546,"---PNG WARNING---Out of memory processing unknown chunk.")
TRACE_MSG(PNGTRANS_596_112_2_18_1_43_6_1547,"---PNG WARNING---This version of libpng does not support user transform info")
TRACE_MSG(SCALING_DOWN_CUS_124_112_2_18_1_43_8_1548,"SPRD_IMG_Scaling_cust: Init_Scale_Info_cus")
TRACE_MSG(SCALING_DOWN_CUS_134_112_2_18_1_43_8_1549,"SPRD_IMG_Scaling_cust : CX_SCALING_CUS")
TRACE_MSG(SCALING_DOWN_CUS_141_112_2_18_1_43_8_1550,"test SPRD_IMG_Scaling_cust; After CX_SCALING_CUS")
TRACE_MSG(SCALING_DOWN_CUS_491_112_2_18_1_43_8_1551," Init_Scale_Info_cus: Info->input_width= %d , input_height = %d")
TRACE_MSG(SCALING_DOWN_CUS_496_112_2_18_1_43_8_1552,"Init_Scale_Info_cus: h_ratio -%d, v_ratio = %d")
TRACE_MSG(SCALING_DOWN_CUS_508_112_2_18_1_43_8_1553,"Init_Scale_Info_cus: N= %d")
TRACE_MSG(IMG_SHARPEN_107_112_2_18_1_43_11_1554,"sharpen src addr: %d\n")
TRACE_MSG(IMG_SHARPEN_108_112_2_18_1_43_11_1555,"sharpen dst addr: %d\n")
TRACE_MSG(IMG_SHARPEN_123_112_2_18_1_43_11_1556,"Sharpen_Test")
TRACE_MSG(IMG_SHARPEN_128_112_2_18_1_43_11_1557,"--img_Sharpen_App:malloc fails!----")
TRACE_MSG(UI_SPECIAL_EFFECT_933_112_2_18_1_43_15_1558,"[UISE: SE_GetEffectFrameAmount] undefined effect id")
TRACE_MSG(UI_SPECIAL_EFFECT_967_112_2_18_1_43_15_1559,"[UISE: SE_DesktopRotate]: Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_981_112_2_18_1_43_15_1560,"[UISE: SE_DesktopRotate]: Use table 0")
TRACE_MSG(UI_SPECIAL_EFFECT_987_112_2_18_1_43_15_1561,"[UISE: SE_DesktopRotate]: Use table 1")
TRACE_MSG(UI_SPECIAL_EFFECT_991_112_2_18_1_43_15_1562,"[UISE: SE_DesktopRotate]: Exit error, no proper table")
TRACE_MSG(UI_SPECIAL_EFFECT_1032_112_2_18_1_43_15_1563,"[UISE: SE_DesktopRotate] undefined direction")
TRACE_MSG(UI_SPECIAL_EFFECT_1051_112_2_18_1_43_15_1564,"[UISE: SE_DesktopRotate] frame = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1052_112_2_18_1_43_15_1565,"[UISE: SE_DesktopRotate]: Exit")
TRACE_MSG(UI_SPECIAL_EFFECT_1085_112_2_18_1_43_15_1566,"[UISE: SE_MenuItemMove]: Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_1134_112_2_18_1_43_15_1567,"[UISE: SE_MenuItemMove] undefined direction")
TRACE_MSG(UI_SPECIAL_EFFECT_1150_112_2_18_1_43_15_1568,"[UISE: SE_MenuItemMove] frame = %d, direction = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1152_112_2_18_1_43_15_1569,"[UISE: SE_MenuItemMove]: Exit")
TRACE_MSG(UI_SPECIAL_EFFECT_1178_112_2_18_1_43_15_1570,"[UISE: SE_MagicMove]: Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_1204_112_2_18_1_43_15_1571,"[UISE: SE_MagicMove]: undefined alpha value")
TRACE_MSG(UI_SPECIAL_EFFECT_1227_112_2_18_1_43_15_1572,"[UISE: SE_MagicMove] frame = %d, direction = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1228_112_2_18_1_43_15_1573,"[UISE: SE_MagicMove]: Exit")
TRACE_MSG(UI_SPECIAL_EFFECT_1256_112_2_18_1_43_15_1574,"[UISE: SE_SlideMove]: Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_1318_112_2_18_1_43_15_1575,"[UISE: SE_SlideMove] undefined direction")
TRACE_MSG(UI_SPECIAL_EFFECT_1335_112_2_18_1_43_15_1576,"[UISE: SE_SlideMove] frame = %d, direction = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1336_112_2_18_1_43_15_1577,"[UISE: SE_SlideMove]: Exit")
TRACE_MSG(UI_SPECIAL_EFFECT_1355_112_2_18_1_43_15_1578,"[UISE: SE_RotateMove3D] Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_1378_112_2_18_1_43_15_1579,"[UISE: SE_RotateMove3D] undefined direction")
TRACE_MSG(UI_SPECIAL_EFFECT_1385_112_2_18_1_43_15_1580,"[UISE: SE_RotateMove3D] frame = %d, direction = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1386_112_2_18_1_43_15_1581,"[UISE: SE_RotateMove3D] Exit")
TRACE_MSG(UI_SPECIAL_EFFECT_1416_112_2_18_1_43_16_1582,"[UISE: SE_RotateMove]: Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_1465_112_2_18_1_43_16_1583,"[UISE: SE_RotateMove] undefined rotate angle")
TRACE_MSG(UI_SPECIAL_EFFECT_1497_112_2_18_1_43_16_1584,"[UISE: SE_RotateMove] undefined rotate angle")
TRACE_MSG(UI_SPECIAL_EFFECT_1505_112_2_18_1_43_16_1585,"[UISE: SE_RotateMove] undefined direction")
TRACE_MSG(UI_SPECIAL_EFFECT_1538_112_2_18_1_43_16_1586,"[UISE: SE_RotateMove] frame = %d, direction = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1540_112_2_18_1_43_16_1587,"[UISE: SE_RotateMove]: Exit")
TRACE_MSG(UI_SPECIAL_EFFECT_1559_112_2_18_1_43_16_1588,"[UISE: SE_DistortMove]: Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_1584_112_2_18_1_43_16_1589,"[UISE: SE_DistortMove] undefined direction")
TRACE_MSG(UI_SPECIAL_EFFECT_1591_112_2_18_1_43_16_1590,"[UISE: SE_DistortMove] frame = %d, direction = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1593_112_2_18_1_43_16_1591,"[UISE: SE_DistortMove]: Exit")
TRACE_MSG(UI_SPECIAL_EFFECT_1628_112_2_18_1_43_16_1592,"[UISE: SE_LuckyBoard]: Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_1781_112_2_18_1_43_16_1593,"[UISE: SE_LuckyBoard] undefined direction")
TRACE_MSG(UI_SPECIAL_EFFECT_1793_112_2_18_1_43_16_1594,"[UISE: SE_LuckyBoard] frame = %d, direction = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1795_112_2_18_1_43_16_1595,"[UISE: SE_LuckyBoard] icon_width = %d, icon_height = %d, output_width = %d, output_height = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1797_112_2_18_1_43_16_1596,"[UISE: SE_LuckyBoard]: Exit, output_rect = (%d, %d, %d, %d)")
TRACE_MSG(UI_SPECIAL_EFFECT_1816_112_2_18_1_43_16_1597,"[UISE: SE_WrappedMove], Enter")
TRACE_MSG(UI_SPECIAL_EFFECT_1840_112_2_18_1_43_17_1598,"[UISE: SE_WrappedMove] undefined direction")
TRACE_MSG(UI_SPECIAL_EFFECT_1847_112_2_18_1_43_17_1599,"[UISE: SE_WrappedMove] frame = %d, direction = %d, ticks = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1849_112_2_18_1_43_17_1600,"[UISE: SE_WrappedMove], Exit")
TRACE_MSG(UI_SPECIAL_EFFECT_1925_112_2_18_1_43_17_1601,"[UISE: SE_CheckInputParam] input_param_ptr: effect_id = %d, direction = %d, frame_index = %d")
TRACE_MSG(UI_SPECIAL_EFFECT_1931_112_2_18_1_43_17_1602,"[UISE: SE_CheckInputParam] input_param_ptr: selected_rect = (%d, %d, %d, %d), next_rect = (%d, %d, %d, %d)")
TRACE_MSG(UI_SPECIAL_EFFECT_1935_112_2_18_1_43_17_1603,"[UISE: SE_CheckInputParam] input_param_ptr: menu_rect = (%d, %d, %d, %d)")
TRACE_MSG(UI_SPECIAL_EFFECT_2836_112_2_18_1_43_19_1604,"[UISE: SE_QuickRotateImage] undefined angle")
TRACE_MSG(UI_SPECIAL_EFFECT_3163_112_2_18_1_43_19_1605,"[UISE: SE_QuickHorzTrapezaWithZoom] undefined zoom level")
TRACE_MSG(UI_SPECIAL_EFFECT_3414_112_2_18_1_43_20_1606,"[UISE: SE_QuickHorzTrapezaWithZoom] undefined zoom level")
TRACE_MSG(UI_SPECIAL_EFFECT_3702_112_2_18_1_43_20_1607,"[UISE: SE_QuickVertTrapezaWithZoom] undefined zoom level")
TRACE_MSG(UI_SPECIAL_EFFECT_3935_112_2_18_1_43_21_1608,"[UISE: SE_QuickVertTrapezaWithZoom] undefined zoom level")
TRACE_MSG(UI_SPECIAL_EFFECT_6399_112_2_18_1_43_26_1609,"[UISE: SE_CalcSlideMoveParam] undefined direction")
END_TRACE_MAP(MS_REF_IMAGE_PROC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_IMAGE_PROC_TRC_H_

