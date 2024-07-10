/******************************************************************************
 ** File Name:      ms_ref_diplay_trc.h                                         *
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
//trace_id:44
#ifndef _MS_REF_DIPLAY_TRC_H_
#define _MS_REF_DIPLAY_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define DIFFUSION_SCALE_IMAGE_190_112_2_18_1_24_44_0 "[_PreSmooth] the input parameters are invalidated"
#define DIFFUSION_SCALE_IMAGE_196_112_2_18_1_24_44_1 "[_PreSmooth] the src height is shorter than filter width, src_h = %d, filter_w = %d"
#define DIFFUSION_SCALE_IMAGE_667_112_2_18_1_24_45_2 "[GRAPH_ScaleAndDiffusionImage] scale failed"
#define DIFFUSION_SCALE_IMAGE_711_112_2_18_1_24_45_3 "[GRAPH_ScaleAndDiffusionImage] the first scale failed"
#define DIFFUSION_SCALE_IMAGE_739_112_2_18_1_24_45_4 "[GRAPH_ScaleAndDiffusionImage] the second scale failed"
#define DIFFUSION_SCALE_IMAGE_810_112_2_18_1_24_45_5 "[GRAPH_ScaleAndDiffusionImage]target buffer malloced failed"
#define DIFFUSION_SCALE_IMAGE_852_112_2_18_1_24_45_6 "[GRAPH_ScaleAndDiffusionImage] scale failed"
#define DIFFUSION_SCALE_IMAGE_927_112_2_18_1_24_45_7 "[GRAPH_ScaleAndDiffusionImage]over the capability of image scale: src_image: w = %d, h = %d, dst_img: w = %d, h = %d"
#define DIFFUSION_SCALE_IMAGE_971_112_2_18_1_24_45_8 "[GRAPH_ScaleAndDiffusionImage]target buffer malloced failed"
#define DIFFUSION_SCALE_IMAGE_1008_112_2_18_1_24_45_9 "[ScaleAndDiffusionImage_Ex]:image scale failed"
#define DRAW_POLYGON_1245_112_2_18_1_24_51_10 "[IMGREF_FillPolygon]: Line buf allocate failed "
#define DRAW_POLYGON_1282_112_2_18_1_24_51_11 "[GRAPH_FillPolygonWidthGradClr]: _AllocateLUT's lut tab allocate failed "
#define DRAW_POLYGON_1385_112_2_18_1_24_51_12 "_FillGradInit: fill_mode is invalidate"
#define DRAW_POLYGON_1425_112_2_18_1_24_51_13 "[IMGREF_FillPolygon]: _AllocateLUT failed"
#define DRAW_POLYGON_1457_112_2_18_1_24_51_14 "[IMGREF_FillPolygon]: the data format is not support!"
#define DRAW_POLYGON_1474_112_2_18_1_24_51_15 "[IMGREF_FillPolygon]: fill context memory allocate failed"
#define DRAW_POLYGON_1510_112_2_18_1_24_51_16 "[IMGREF_FillPolygon]: _CalcPolygen failed"
#define DRAW_POLYGON_1549_112_2_18_1_24_52_17 "[IMGREF_FillPolygon]: the data format is not support!"
#define DRAW_POLYGON_1579_112_2_18_1_24_52_18 "[IMGREF_FillPolygon]: fill context memory allocate failed"
#define DRAW_POLYGON_1622_112_2_18_1_24_52_19 "[IMGREF_FillPolygon]: _AllocateLUT failed"
#define DRAW_POLYGON_1632_112_2_18_1_24_52_20 "[IMGREF_FillPolygon]: _CalcPolygen failed"
#define DRAW_POLYGON_1666_112_2_18_1_24_52_21 "[IMGREF_FillPolygon]: the data format is not support!"
#define DRAW_POLYGON_1693_112_2_18_1_24_52_22 "[IMGREF_FillPolygon]: fill context memory allocate failed"
#define DRAW_POLYGON_1722_112_2_18_1_24_52_23 "[IMGREF_FillPolygon]: _FillGradInit failed"
#define DRAW_POLYGON_1730_112_2_18_1_24_52_24 "[IMGREF_FillPolygon]: _CalcPolygen failed"
#define DRAW_POLYGON_1744_112_2_18_1_24_52_25 "[IMGREF_FillPolygon]: _FillPureInit failed"
#define DRAW_POLYGON_1752_112_2_18_1_24_52_26 "[IMGREF_FillPolygon]: _CalcPolygen failed"
#define DRAW_ROUND_RECT_1514_112_2_18_1_24_55_27 "[_CheckAndSetParam] rect size is too small (%d X %d)"
#define DRAW_ROUND_RECT_1534_112_2_18_1_24_55_28 "[_CheckAndSetParam] bk format error (%d)"
#define DRAW_ROUND_RECT_1541_112_2_18_1_24_55_29 "[_CheckAndSetParam] background buffer size error, bk size (%d X %d), bk format = %d, bk buf size = %d"
#define DRAW_ROUND_RECT_1562_112_2_18_1_24_55_30 "[_CheckAndSetParam] target buffer size error, rect size (%d X %d), bk buf size = %d"
#define DRAW_ROUND_RECT_1584_112_2_18_1_24_55_31 "[_CheckAndSetParam] both bk and target buffer are NULL"
#define DRAW_ROUND_RECT_1612_112_2_18_1_24_55_32 "[GRAPH_DrawRoundRect] input/output parameter invalid"
#define DRAW_ROUND_RECT_1619_112_2_18_1_24_55_33 "[GRAPH_DrawRoundRect] _CheckAndSetParam failed"
#define DRAW_ROUND_RECT_1673_112_2_18_1_24_55_34 "[GRAPH_FillRoundRect] input/output parameter invalid"
#define DRAW_ROUND_RECT_1680_112_2_18_1_24_55_35 "[GRAPH_FillRoundRect] _CheckAndSetParam failed"
#define ROTATE_IMAGE_324_112_2_18_1_24_56_36 "[GRAPH_RotateImage] invalid pointer"
#define ROTATE_IMAGE_331_112_2_18_1_24_56_37 "[GRAPH_RotateImage] in size (%dX%d), format = %d, chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define ROTATE_IMAGE_335_112_2_18_1_24_56_38 "[GRAPH_RotateImage] rotate mode = %d, target buf (0x%x, %d)"
#define ROTATE_IMAGE_339_112_2_18_1_24_56_39 "[GRAPH_RotateImage] invalid source size"
#define ROTATE_IMAGE_345_112_2_18_1_24_56_40 "[GRAPH_RotateImage] invalid source size"
#define ROTATE_IMAGE_367_112_2_18_1_24_56_41 "[GRAPH_RotateImage] invalid source size, unalign %d X %d, align = %d, format = %d"
#define ROTATE_IMAGE_374_112_2_18_1_24_56_42 "[GRAPH_RotateImage] invalid rotate mode = %d"
#define ROTATE_IMAGE_385_112_2_18_1_24_56_43 "[GRAPH_RotateImage] get address failed"
#define ROTATE_IMAGE_415_112_2_18_1_24_57_44 "[GRAPH_RotateImage] out size (%dX%d), format = %d, chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define ROTATE_IMAGE_417_112_2_18_1_24_57_45 "[GRAPH_RotateImage] time = %d"
#define ROTATE_IMAGE_447_112_2_18_1_24_57_46 "[GRAPH_RotateArbitrary] angle = %d, src buf = (0x%x, %d), target buf = (0x%x, %d)"
#define ROTATE_IMAGE_453_112_2_18_1_24_57_47 "[GRAPH_RotateArbitrary] src size = (%d, %d), src center (%d, %d), target size = (%d, %d), target center (%d, %d)"
#define ROTATE_IMAGE_511_112_2_18_1_24_57_48 "[GRAPH_RotateArbitrary] allocate temp buffer failed"
#define ROTATE_IMAGE_519_112_2_18_1_24_57_49 "[GRAPH_RotateArbitrary] temp buffer = (0x%x, %d)"
#define ROTATE_IMAGE_532_112_2_18_1_24_57_50 "[GRAPH_RotateArbitrary] output size = (%d, %d), offset = (%d, %d)"
#define ROTATE_IMAGE_542_112_2_18_1_24_57_51 "[GRAPH_RotateArbitrary] ticks = %d"
#define ROTATE_IMAGE_571_112_2_18_1_24_57_52 "[GRAPH_RotateArbitrary and Blend] angle = %d, src buf = (0x%x, %d), background buf = (0x%x, %d)"
#define ROTATE_IMAGE_577_112_2_18_1_24_57_53 "[GRAPH_RotateArbitrary and Blend] src size = (%d, %d), src center (%d, %d), background size = (%d, %d), background center (%d, %d)"
#define ROTATE_IMAGE_634_112_2_18_1_24_57_54 "[GRAPH_RotateArbitrary and Blend] allocate temp buffer failed"
#define ROTATE_IMAGE_642_112_2_18_1_24_57_55 "[GRAPH_RotateArbitrary and Blend] temp buffer = (0x%x, %d)"
#define ROTATE_IMAGE_655_112_2_18_1_24_57_56 "[GRAPH_RotateArbitrary and Blend] output size = (%d, %d), offset = (%d, %d)"
#define ROTATE_IMAGE_665_112_2_18_1_24_57_57 "[GRAPH_RotateArbitrary and Blend] ticks = %d"
#define ROTATE_IMAGE_766_112_2_18_1_24_57_58 "[test_rotate_yuv422] scale failed"
#define ROTATE_IMAGE_776_112_2_18_1_24_57_59 "[test_rotate_yuv422] size (%dX%d), formate %d, chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define ROTATE_IMAGE_864_112_2_18_1_24_58_60 "[test_rotate_rgb565] scale failed"
#define ROTATE_IMAGE_874_112_2_18_1_24_58_61 "[test_rotate_rgb565] size (%dX%d), formate %d, chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define ROTATE_IMAGE_951_112_2_18_1_24_58_62 "[test_rotate_rgb565] scale failed"
#define ROTATE_IMAGE_961_112_2_18_1_24_58_63 "[test_rotate_rgb565] size (%dX%d), formate %d, chn0 (0x%x, %d), chn1 (0x%x, %d)"
#define S3D_ROTATE_SE_2415_112_2_18_1_25_13_64 "S3D_CubeAxisRotation eye start"
#define S3D_ROTATE_SE_2435_112_2_18_1_25_13_65 "S3D_CubeAxisRotation eye x = %d x= %d z =%d"
#define S3D_ROTATE_SE_2443_112_2_18_1_25_13_66 "S3D_CubeAxisRotation axis x = %d x= %d z =%d axis=%d"
#define S3D_ROTATE_SE_2450_112_2_18_1_25_13_67 "S3D_CubeAxisRotation 1"
#define S3D_ROTATE_SE_2457_112_2_18_1_25_13_68 "S3D_CubeAxisRotation 2"
#define S3D_ROTATE_SE_2486_112_2_18_1_25_13_69 "S3D_CubeAxisRotation vertex %d x= %d y =%d"
#define S3D_ROTATE_SE_2710_112_2_18_1_25_13_70 "S3D_CubeAxisRotation 3"
#define S3D_ROTATE_SE_3552_112_2_18_1_25_15_71 "S3D_CubeAxisRotation 1"
#define S3D_ROTATE_SE_3559_112_2_18_1_25_15_72 "S3D_CubeAxisRotation 2"
#define S3D_ROTATION_1654_112_2_18_1_25_22_73 "S3D_CubeAxisRotation eye start"
#define S3D_ROTATION_1674_112_2_18_1_25_22_74 "S3D_CubeAxisRotation eye x = %d x= %d z =%d"
#define S3D_ROTATION_1682_112_2_18_1_25_22_75 "S3D_CubeAxisRotation axis x = %d x= %d z =%d axis=%d"
#define S3D_ROTATION_1689_112_2_18_1_25_22_76 "S3D_CubeAxisRotation 1"
#define S3D_ROTATION_1696_112_2_18_1_25_22_77 "S3D_CubeAxisRotation 2"
#define S3D_ROTATION_1725_112_2_18_1_25_22_78 "S3D_CubeAxisRotation vertex %d x= %d y =%d"
#define S3D_ROTATION_1959_112_2_18_1_25_22_79 "S3D_CubeAxisRotation 3"
#define S3D_ROTATION_2293_112_2_18_1_25_23_80 "test_3D_Rotation 1 = %d\n"
#define S3D_ROTATION_SE_2542_112_2_18_1_25_30_81 "S3D_CubeAxisRotation eye start"
#define S3D_ROTATION_SE_2562_112_2_18_1_25_30_82 "S3D_CubeAxisRotation eye x = %d x= %d z =%d"
#define S3D_ROTATION_SE_2570_112_2_18_1_25_30_83 "S3D_CubeAxisRotation axis x = %d x= %d z =%d axis=%d"
#define S3D_ROTATION_SE_2577_112_2_18_1_25_30_84 "S3D_CubeAxisRotation 1"
#define S3D_ROTATION_SE_2584_112_2_18_1_25_30_85 "S3D_CubeAxisRotation 2"
#define S3D_ROTATION_SE_2613_112_2_18_1_25_30_86 "S3D_CubeAxisRotation vertex %d x= %d y =%d"
#define S3D_ROTATION_SE_2844_112_2_18_1_25_30_87 "S3D_CubeAxisRotation 3"
#define S3D_ROTATION_SE_3730_112_2_18_1_25_32_88 "S3D_CubeAxisRotation 1"
#define S3D_ROTATION_SE_3737_112_2_18_1_25_32_89 "S3D_CubeAxisRotation 2"
#define S3D_ROTATION_SE_11283_112_2_18_1_25_48_90 "s3d_MidRotation_Image: NO such color type blending in S3D_DATA_ARGB888! "
#define S3D_ROTATION_SE_11303_112_2_18_1_25_48_91 "s3d_MidRotation_Image: NO such color type blending in S3D_DATA_PAF_RGB888! "
#define S3D_ROTATION_SE_11309_112_2_18_1_25_48_92 "s3d_MidRotation_Image: NO such color type blending now! "
#define S3D_ROTATION_SE_11584_112_2_18_1_25_49_93 "s3d_MidRotation_Image: NO such color type blending in S3D_DATA_ARGB888! "
#define S3D_ROTATION_SE_11613_112_2_18_1_25_49_94 "s3d_MidRotation_Image: NO such color type blending in S3D_DATA_PAF_RGB888! "
#define S3D_ROTATION_SE_11621_112_2_18_1_25_49_95 "s3d_MidRotation_Image_shining: NO such color type blending now! "
#define S3D_ROTATION_SE_12061_112_2_18_1_25_50_96 "HTC_ALPHA, cost_time ONE frame = %d ms"
#define SCALE_IMAGE_96_112_2_18_1_25_50_97 "[_AsyncScaleCallback] ret = %d, async_param = 0x%x"
#define SCALE_IMAGE_168_112_2_18_1_25_51_98 "[GRAPH_ScaleImage] ARGB888 scaling not support async, change to sync mode"
#define IMGPROC_SCALE_IMAGE_351_112_2_18_1_25_53_99 "[_ScaleImageARGB888] src_alpha_buf (0x%x, %d), dst_alpha_buf (0x%x, %d), dst_rgb_buf (0x%x, %d)"
#define IMGPROC_SCALE_IMAGE_358_112_2_18_1_25_53_100 "[_ScaleImageARGB888] allocate alpha buffer failed"
#define IMGPROC_SCALE_IMAGE_376_112_2_18_1_25_53_101 "[_ScaleImageARGB888] get alpha time = %d"
#define IMGPROC_SCALE_IMAGE_398_112_2_18_1_25_53_102 "[_ScaleImageARGB888] _ScaleImage rgb888 failed"
#define IMGPROC_SCALE_IMAGE_404_112_2_18_1_25_53_103 "[_ScaleImageARGB888] scale alpha time = %d"
#define IMGPROC_SCALE_IMAGE_408_112_2_18_1_25_53_104 "[_ScaleImageARGB888] scale alpha done, out size (%d, %d), out addr (0x%x, %d), format (%d)"
#define IMGPROC_SCALE_IMAGE_430_112_2_18_1_25_53_105 "[_ScaleImageARGB888] _ScaleImage rgb888 failed"
#define IMGPROC_SCALE_IMAGE_436_112_2_18_1_25_53_106 "[_ScaleImageARGB888] scale rgb time = %d"
#define IMGPROC_SCALE_IMAGE_440_112_2_18_1_25_53_107 "[_ScaleImageARGB888] scale rgb done, out size (%d, %d), out addr (0x%x, %d), format (%d)"
#define IMGPROC_SCALE_IMAGE_451_112_2_18_1_25_53_108 "[_ScaleImageARGB888] merg alpha and rgb time = %d"
#define IMGPROC_SCALE_IMAGE_486_112_2_18_1_25_53_109 "[_ScaleImageARGB565] get alpha time = %d"
#define IMGPROC_SCALE_IMAGE_503_112_2_18_1_25_53_110 "[_ScaleImageARGB565] _ScaleImage rgb888 failed"
#define IMGPROC_SCALE_IMAGE_509_112_2_18_1_25_53_111 "[_ScaleImageARGB565] scale alpha time = %d"
#define IMGPROC_SCALE_IMAGE_513_112_2_18_1_25_53_112 "[_ScaleImageARGB565] scale alpha done, out size (%d, %d), out addr (0x%x, %d), format (%d)"
#define IMGPROC_SCALE_IMAGE_528_112_2_18_1_25_53_113 "[_ScaleImageARGB565] _ScaleImage rgb888 failed"
#define IMGPROC_SCALE_IMAGE_534_112_2_18_1_25_53_114 "[_ScaleImageARGB565] scale rgb time = %d"
#define IMGPROC_SCALE_IMAGE_538_112_2_18_1_25_53_115 "[_ScaleImageARGB565] scale rgb done, out size (%d, %d), out addr (0x%x, %d), format (%d)"
#define IMGPROC_SCALE_IMAGE_747_112_2_18_1_25_54_116 "[_SetScaleSize] output format = %d, dst chn0 (0x%x, %d), dst chn1 (0x%x, %d)"
#define IMGPROC_SCALE_IMAGE_876_112_2_18_1_25_54_117 "[_SetScaleSize] src size (%d, %d), src rect (%d, %d, %d, %d), target size (%d, %d)"
#define IMGPROC_SCALE_IMAGE_892_112_2_18_1_25_54_118 "[_SetScaleParam] _SetScaleInputChn failed"
#define IMGPROC_SCALE_IMAGE_898_112_2_18_1_25_54_119 "[_SetScaleParam] _SetScaleOutputChn failed"
#define IMGPROC_SCALE_IMAGE_904_112_2_18_1_25_54_120 "[_SetScaleParam] _SetScaleSize failed"
#define IMGPROC_SCALE_IMAGE_984_112_2_18_1_25_54_121 "[GRAPH_ScaleImage] input/output parameter invalid"
#define IMGPROC_SCALE_IMAGE_988_112_2_18_1_25_54_122 "[GRAPH_ScaleImage]"
#define IMGPROC_SCALE_IMAGE_992_112_2_18_1_25_54_123 "[GRAPH_ScaleImage] src size (%d, %d), src tirm rect (%d, %d, %d, %d),"
#define IMGPROC_SCALE_IMAGE_995_112_2_18_1_25_54_124 "[GRAPH_ScaleImage] src format (%d), src chn0 (0x%x, %d), chn1 (0x%x, %d),"
#define IMGPROC_SCALE_IMAGE_999_112_2_18_1_25_54_125 "[GRAPH_ScaleImage] target size (%d, %d), target format (%d), target buf (0x%x, %d),"
#define IMGPROC_SCALE_IMAGE_1011_112_2_18_1_25_54_126 "[GRAPH_ScaleImage] _SetScaleParam failed"
#define IMGPROC_SCALE_IMAGE_1020_112_2_18_1_25_54_127 "[GRAPH_ScaleImage] get scale func failed"
#define IMGPROC_SCALE_IMAGE_1034_112_2_18_1_25_54_128 "[GRAPH_ScaleImage] ret = %d, output size (%d, %d), output format (%d)"
#define IMGPROC_SCALE_IMAGE_1038_112_2_18_1_25_54_129 "[GRAPH_ScaleImage]output chn0 (0x%x, %d), output chn1 (0x%x, %d)"
#define IMGPROC_SCALE_IMAGE_1044_112_2_18_1_25_54_130 "[GRAPH_ScaleImage] spend time = %d"
#define BMP_IO_94_112_2_18_1_26_19_131 "Not support bmp format"
#define GIF_RES_DECODER_737_112_2_18_1_26_22_132 "----GIFRES_Dec point 0----"
#define GIF_RES_DECODER_858_112_2_18_1_26_23_133 "----GIFRES_Dec point 3----"
#define GIF_RES_DECODER_1903_112_2_18_1_26_25_134 "seek_next_image: return : %d"
#define GIF_RES_DECODER_1918_112_2_18_1_26_25_135 "seek_next_image: return "
#define IMG_RES_DECODER_154_112_2_18_1_26_27_136 "\r\nDC_IRAM_Disable:unable to unlock IRAM"
#define IMG_RES_DECODER_723_112_2_18_1_26_28_137 "IMG_RES_Free_Handler:filetype error!"
#define JPEG_RES_DECODER_119_112_2_18_1_26_31_138 "JPEG_Display_Img"
#define JPEG_RES_DECODER_148_112_2_18_1_26_31_139 "JPEG_Display_Img, jpeg size = (%d, %d), rect = (%d, %d, %d, %d)"
#define JPEG_RES_DECODER_152_112_2_18_1_26_31_140 "JPEG_Display_Img, disp size = (%d, %d), rect = (%d, %d, %d, %d)"
#define JPEG_RES_DECODER_155_112_2_18_1_26_31_141 "JPEG_Display_Img, img rect size = (%d, %d), disp rect size = (%d, %d)"
#define JPEG_RES_DECODER_197_112_2_18_1_26_31_142 "'[JPEG_Display_Img] allocate buffer failed!"
#define JPEG_RES_DECODER_202_112_2_18_1_26_31_143 "[JPEG_Display_Img] allocate buffer success! "
#define JPEG_RES_DECODER_204_112_2_18_1_26_31_144 "[JPEG_Display_Img] decode buf = 0x%x, size = %d, target buf = 0x%x, size = %d"
#define JPEG_RES_DECODER_244_112_2_18_1_26_31_145 "[JPEG_Display_Img] decode standard jpeg, stream buf = 0x%x, size = %d, "
#define JPEG_RES_DECODER_287_112_2_18_1_26_31_146 "[JPEG_Display_Img] display image, image buf = 0x%x, disp buf = 0x%x"
#define JPEG_RES_DECODER_305_112_2_18_1_26_31_147 "[JPEG_Display_Img] display image, image buf = 0x%x, disp buf = 0x%x"
#define JPEG_RES_DECODER_320_112_2_18_1_26_31_148 "[JPEG_Display_Img] display image end"
#define JPEG_RES_DECODER_324_112_2_18_1_26_31_149 "[JPEG_Display_Img] free decode buf = 0x%x"
#define JPEG_RES_DECODER_331_112_2_18_1_26_31_150 "[JPEG_Display_Img] free target buf = 0x%x"
#define JPEG_RES_DECODER_342_112_2_18_1_26_31_151 "[JPEG_Display_Img] out"
#define JPEG_RES_DECODER_394_112_2_18_1_26_31_152 "JPEG_Display_Img, img rect = (%d, %d, %d, %d), dsip rect (%d, %d, %d, %d), dst format = %d"
#define JPEG_RES_DECODER_441_112_2_18_1_26_32_153 "'[JPEG_Display_Img] allocate buffer failed!"
#define JPEG_RES_DECODER_543_112_2_18_1_26_32_154 "[test_jpeg], src = 0x%x, dst = 0x%x"
#define ROLL_1887_112_2_18_1_26_53_155 "[ROLL_GetFrame] +"
#define ROLL_1906_112_2_18_1_26_53_156 "[ROLL_GetFrame] _CalcParam %d"
#define ROLL_1912_112_2_18_1_26_53_157 "[ROLL_GetFrame] _DrawTop %d"
#define ROLL_1918_112_2_18_1_26_53_158 "[ROLL_GetFrame] _DrawSpindleTop %d"
#define ROLL_1924_112_2_18_1_26_53_159 "[ROLL_GetFrame] _DrawMiddle %d"
#define ROLL_1930_112_2_18_1_26_53_160 "[ROLL_GetFrame] _DrawSpindleBottom %d"
#define ROLL_1936_112_2_18_1_26_53_161 "[ROLL_GetFrame] _DrawBottom %d"
#define ROLL_1942_112_2_18_1_26_53_162 "[ROLL_GetFrame] _DestorySrcArray %d"
#define ROLL_1948_112_2_18_1_26_53_163 "[ROLL_GetFrame] -, %d"
#define SOLID_2633_112_2_18_1_26_59_164 "[SOLID_Render] ticks = %d"
#define SOLID_2801_112_2_18_1_26_59_165 "[sphere_test] frontage buf = 0x%x, backside buf = 0x%x"
#define SOLID_2870_112_2_18_1_26_59_166 "[sphere_test] ticks = %d"
#define SPECIAL_EFFECT_CURTAIN_537_112_2_18_1_27_3_167 "The frame rate is invalidated: should be larger than 1"
#define SPECIAL_EFFECT_CURTAIN_556_112_2_18_1_27_4_168 "the Radius is too large, Radius_Max = %d, Radius_Now = %d"
#define SPECIAL_EFFECT_CURTAIN_569_112_2_18_1_27_4_169 "[_SetContextCircleParam]: radius array memory allocate failed"
#define SPECIAL_EFFECT_CURTAIN_777_112_2_18_1_27_4_170 "[_GetUpdateRectFromIndex]:the index is invalidated "
#define SPECIAL_EFFECT_CURTAIN_940_112_2_18_1_27_4_171 "[Effect_CURTAIN_Create]: the input is invalidate"
#define SPECIAL_EFFECT_CURTAIN_947_112_2_18_1_27_4_172 "the data format does not support"
#define SPECIAL_EFFECT_CURTAIN_955_112_2_18_1_27_4_173 "[Effect_CURTAIN_Create]: SE_CURTAIN_HANLDE_T memory allocate failed"
#define SPECIAL_EFFECT_CURTAIN_965_112_2_18_1_27_4_174 "[Effect_CURTAIN_Create]: SE_CURTAIN_CONTEXT_T memory allocate failed"
#define SPECIAL_EFFECT_CURTAIN_993_112_2_18_1_27_4_175 "[Effect_CURTAIN_Create]: _SetContextGraphParam function failed"
#define SPECIAL_EFFECT_CURTAIN_1039_112_2_18_1_27_4_176 "the input param invalidate: input_ptr = %d, out_ptr = %d, handle = %d"
#define SPECIAL_EFFECT_CURTAIN_1162_112_2_18_1_27_5_177 "the input param invalidate: data_chn_ptr = %d, img_size_ptr = %d, dst_chn_ptr = %d, update_param_ptr = %d"
#define SPECIAL_EFFECT_CURTAIN_1169_112_2_18_1_27_5_178 "the data format does not support"
#define SPECIAL_EFFECT_CURTAIN_1186_112_2_18_1_27_5_179 "the Radius is too large, Radius_Max = %d, Radius_Now = %d"
#define SPECIAL_EFFECT_CURTAIN_1194_112_2_18_1_27_5_180 "Alpha Memory Malloc failed"
#define IMG_TEST_108_112_2_18_1_27_18_181 "load image failed\r\n"
#define IMG_TEST_115_112_2_18_1_27_18_182 "compress failed\r\n"
#define IMG_TEST_121_112_2_18_1_27_18_183 "save failed\r\n"
#define IMG_TEST_137_112_2_18_1_27_18_184 "load image failed\r\n"
#define IMG_TEST_144_112_2_18_1_27_18_185 "compress failed\r\n"
#define IMG_TEST_150_112_2_18_1_27_18_186 "save failed\r\n"
#define IMG_TEST_165_112_2_18_1_27_18_187 "load image failed\r\n"
#define IMG_TEST_172_112_2_18_1_27_18_188 "compress failed\r\n"
#define IMG_TEST_188_112_2_18_1_27_18_189 "compress failed\r\n"
#define IMG_TEST_194_112_2_18_1_27_18_190 "save failed\r\n"
#define IMG_TEST_213_112_2_18_1_27_18_191 "\r\n load failed"
#define IMG_TEST_220_112_2_18_1_27_18_192 "\r\n get info failed"
#define IMG_TEST_249_112_2_18_1_27_18_193 "\r\n disp failed"
#define IMG_TEST_266_112_2_18_1_27_18_194 "\r\n load failed"
#define IMG_TEST_273_112_2_18_1_27_18_195 "\r\n get info failed"
#define IMG_TEST_302_112_2_18_1_27_18_196 "\r\n disp failed"
#define IMG_TEST_378_112_2_18_1_27_19_197 "\r\n get info failed"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_DIPLAY_TRC)
TRACE_MSG(DIFFUSION_SCALE_IMAGE_190_112_2_18_1_24_44_0,"[_PreSmooth] the input parameters are invalidated")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_196_112_2_18_1_24_44_1,"[_PreSmooth] the src height is shorter than filter width, src_h = %d, filter_w = %d")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_667_112_2_18_1_24_45_2,"[GRAPH_ScaleAndDiffusionImage] scale failed")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_711_112_2_18_1_24_45_3,"[GRAPH_ScaleAndDiffusionImage] the first scale failed")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_739_112_2_18_1_24_45_4,"[GRAPH_ScaleAndDiffusionImage] the second scale failed")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_810_112_2_18_1_24_45_5,"[GRAPH_ScaleAndDiffusionImage]target buffer malloced failed")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_852_112_2_18_1_24_45_6,"[GRAPH_ScaleAndDiffusionImage] scale failed")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_927_112_2_18_1_24_45_7,"[GRAPH_ScaleAndDiffusionImage]over the capability of image scale: src_image: w = %d, h = %d, dst_img: w = %d, h = %d")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_971_112_2_18_1_24_45_8,"[GRAPH_ScaleAndDiffusionImage]target buffer malloced failed")
TRACE_MSG(DIFFUSION_SCALE_IMAGE_1008_112_2_18_1_24_45_9,"[ScaleAndDiffusionImage_Ex]:image scale failed")
TRACE_MSG(DRAW_POLYGON_1245_112_2_18_1_24_51_10,"[IMGREF_FillPolygon]: Line buf allocate failed ")
TRACE_MSG(DRAW_POLYGON_1282_112_2_18_1_24_51_11,"[GRAPH_FillPolygonWidthGradClr]: _AllocateLUT's lut tab allocate failed ")
TRACE_MSG(DRAW_POLYGON_1385_112_2_18_1_24_51_12,"_FillGradInit: fill_mode is invalidate")
TRACE_MSG(DRAW_POLYGON_1425_112_2_18_1_24_51_13,"[IMGREF_FillPolygon]: _AllocateLUT failed")
TRACE_MSG(DRAW_POLYGON_1457_112_2_18_1_24_51_14,"[IMGREF_FillPolygon]: the data format is not support!")
TRACE_MSG(DRAW_POLYGON_1474_112_2_18_1_24_51_15,"[IMGREF_FillPolygon]: fill context memory allocate failed")
TRACE_MSG(DRAW_POLYGON_1510_112_2_18_1_24_51_16,"[IMGREF_FillPolygon]: _CalcPolygen failed")
TRACE_MSG(DRAW_POLYGON_1549_112_2_18_1_24_52_17,"[IMGREF_FillPolygon]: the data format is not support!")
TRACE_MSG(DRAW_POLYGON_1579_112_2_18_1_24_52_18,"[IMGREF_FillPolygon]: fill context memory allocate failed")
TRACE_MSG(DRAW_POLYGON_1622_112_2_18_1_24_52_19,"[IMGREF_FillPolygon]: _AllocateLUT failed")
TRACE_MSG(DRAW_POLYGON_1632_112_2_18_1_24_52_20,"[IMGREF_FillPolygon]: _CalcPolygen failed")
TRACE_MSG(DRAW_POLYGON_1666_112_2_18_1_24_52_21,"[IMGREF_FillPolygon]: the data format is not support!")
TRACE_MSG(DRAW_POLYGON_1693_112_2_18_1_24_52_22,"[IMGREF_FillPolygon]: fill context memory allocate failed")
TRACE_MSG(DRAW_POLYGON_1722_112_2_18_1_24_52_23,"[IMGREF_FillPolygon]: _FillGradInit failed")
TRACE_MSG(DRAW_POLYGON_1730_112_2_18_1_24_52_24,"[IMGREF_FillPolygon]: _CalcPolygen failed")
TRACE_MSG(DRAW_POLYGON_1744_112_2_18_1_24_52_25,"[IMGREF_FillPolygon]: _FillPureInit failed")
TRACE_MSG(DRAW_POLYGON_1752_112_2_18_1_24_52_26,"[IMGREF_FillPolygon]: _CalcPolygen failed")
TRACE_MSG(DRAW_ROUND_RECT_1514_112_2_18_1_24_55_27,"[_CheckAndSetParam] rect size is too small (%d X %d)")
TRACE_MSG(DRAW_ROUND_RECT_1534_112_2_18_1_24_55_28,"[_CheckAndSetParam] bk format error (%d)")
TRACE_MSG(DRAW_ROUND_RECT_1541_112_2_18_1_24_55_29,"[_CheckAndSetParam] background buffer size error, bk size (%d X %d), bk format = %d, bk buf size = %d")
TRACE_MSG(DRAW_ROUND_RECT_1562_112_2_18_1_24_55_30,"[_CheckAndSetParam] target buffer size error, rect size (%d X %d), bk buf size = %d")
TRACE_MSG(DRAW_ROUND_RECT_1584_112_2_18_1_24_55_31,"[_CheckAndSetParam] both bk and target buffer are NULL")
TRACE_MSG(DRAW_ROUND_RECT_1612_112_2_18_1_24_55_32,"[GRAPH_DrawRoundRect] input/output parameter invalid")
TRACE_MSG(DRAW_ROUND_RECT_1619_112_2_18_1_24_55_33,"[GRAPH_DrawRoundRect] _CheckAndSetParam failed")
TRACE_MSG(DRAW_ROUND_RECT_1673_112_2_18_1_24_55_34,"[GRAPH_FillRoundRect] input/output parameter invalid")
TRACE_MSG(DRAW_ROUND_RECT_1680_112_2_18_1_24_55_35,"[GRAPH_FillRoundRect] _CheckAndSetParam failed")
TRACE_MSG(ROTATE_IMAGE_324_112_2_18_1_24_56_36,"[GRAPH_RotateImage] invalid pointer")
TRACE_MSG(ROTATE_IMAGE_331_112_2_18_1_24_56_37,"[GRAPH_RotateImage] in size (%dX%d), format = %d, chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_335_112_2_18_1_24_56_38,"[GRAPH_RotateImage] rotate mode = %d, target buf (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_339_112_2_18_1_24_56_39,"[GRAPH_RotateImage] invalid source size")
TRACE_MSG(ROTATE_IMAGE_345_112_2_18_1_24_56_40,"[GRAPH_RotateImage] invalid source size")
TRACE_MSG(ROTATE_IMAGE_367_112_2_18_1_24_56_41,"[GRAPH_RotateImage] invalid source size, unalign %d X %d, align = %d, format = %d")
TRACE_MSG(ROTATE_IMAGE_374_112_2_18_1_24_56_42,"[GRAPH_RotateImage] invalid rotate mode = %d")
TRACE_MSG(ROTATE_IMAGE_385_112_2_18_1_24_56_43,"[GRAPH_RotateImage] get address failed")
TRACE_MSG(ROTATE_IMAGE_415_112_2_18_1_24_57_44,"[GRAPH_RotateImage] out size (%dX%d), format = %d, chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_417_112_2_18_1_24_57_45,"[GRAPH_RotateImage] time = %d")
TRACE_MSG(ROTATE_IMAGE_447_112_2_18_1_24_57_46,"[GRAPH_RotateArbitrary] angle = %d, src buf = (0x%x, %d), target buf = (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_453_112_2_18_1_24_57_47,"[GRAPH_RotateArbitrary] src size = (%d, %d), src center (%d, %d), target size = (%d, %d), target center (%d, %d)")
TRACE_MSG(ROTATE_IMAGE_511_112_2_18_1_24_57_48,"[GRAPH_RotateArbitrary] allocate temp buffer failed")
TRACE_MSG(ROTATE_IMAGE_519_112_2_18_1_24_57_49,"[GRAPH_RotateArbitrary] temp buffer = (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_532_112_2_18_1_24_57_50,"[GRAPH_RotateArbitrary] output size = (%d, %d), offset = (%d, %d)")
TRACE_MSG(ROTATE_IMAGE_542_112_2_18_1_24_57_51,"[GRAPH_RotateArbitrary] ticks = %d")
TRACE_MSG(ROTATE_IMAGE_571_112_2_18_1_24_57_52,"[GRAPH_RotateArbitrary and Blend] angle = %d, src buf = (0x%x, %d), background buf = (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_577_112_2_18_1_24_57_53,"[GRAPH_RotateArbitrary and Blend] src size = (%d, %d), src center (%d, %d), background size = (%d, %d), background center (%d, %d)")
TRACE_MSG(ROTATE_IMAGE_634_112_2_18_1_24_57_54,"[GRAPH_RotateArbitrary and Blend] allocate temp buffer failed")
TRACE_MSG(ROTATE_IMAGE_642_112_2_18_1_24_57_55,"[GRAPH_RotateArbitrary and Blend] temp buffer = (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_655_112_2_18_1_24_57_56,"[GRAPH_RotateArbitrary and Blend] output size = (%d, %d), offset = (%d, %d)")
TRACE_MSG(ROTATE_IMAGE_665_112_2_18_1_24_57_57,"[GRAPH_RotateArbitrary and Blend] ticks = %d")
TRACE_MSG(ROTATE_IMAGE_766_112_2_18_1_24_57_58,"[test_rotate_yuv422] scale failed")
TRACE_MSG(ROTATE_IMAGE_776_112_2_18_1_24_57_59,"[test_rotate_yuv422] size (%dX%d), formate %d, chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_864_112_2_18_1_24_58_60,"[test_rotate_rgb565] scale failed")
TRACE_MSG(ROTATE_IMAGE_874_112_2_18_1_24_58_61,"[test_rotate_rgb565] size (%dX%d), formate %d, chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(ROTATE_IMAGE_951_112_2_18_1_24_58_62,"[test_rotate_rgb565] scale failed")
TRACE_MSG(ROTATE_IMAGE_961_112_2_18_1_24_58_63,"[test_rotate_rgb565] size (%dX%d), formate %d, chn0 (0x%x, %d), chn1 (0x%x, %d)")
TRACE_MSG(S3D_ROTATE_SE_2415_112_2_18_1_25_13_64,"S3D_CubeAxisRotation eye start")
TRACE_MSG(S3D_ROTATE_SE_2435_112_2_18_1_25_13_65,"S3D_CubeAxisRotation eye x = %d x= %d z =%d")
TRACE_MSG(S3D_ROTATE_SE_2443_112_2_18_1_25_13_66,"S3D_CubeAxisRotation axis x = %d x= %d z =%d axis=%d")
TRACE_MSG(S3D_ROTATE_SE_2450_112_2_18_1_25_13_67,"S3D_CubeAxisRotation 1")
TRACE_MSG(S3D_ROTATE_SE_2457_112_2_18_1_25_13_68,"S3D_CubeAxisRotation 2")
TRACE_MSG(S3D_ROTATE_SE_2486_112_2_18_1_25_13_69,"S3D_CubeAxisRotation vertex %d x= %d y =%d")
TRACE_MSG(S3D_ROTATE_SE_2710_112_2_18_1_25_13_70,"S3D_CubeAxisRotation 3")
TRACE_MSG(S3D_ROTATE_SE_3552_112_2_18_1_25_15_71,"S3D_CubeAxisRotation 1")
TRACE_MSG(S3D_ROTATE_SE_3559_112_2_18_1_25_15_72,"S3D_CubeAxisRotation 2")
TRACE_MSG(S3D_ROTATION_1654_112_2_18_1_25_22_73,"S3D_CubeAxisRotation eye start")
TRACE_MSG(S3D_ROTATION_1674_112_2_18_1_25_22_74,"S3D_CubeAxisRotation eye x = %d x= %d z =%d")
TRACE_MSG(S3D_ROTATION_1682_112_2_18_1_25_22_75,"S3D_CubeAxisRotation axis x = %d x= %d z =%d axis=%d")
TRACE_MSG(S3D_ROTATION_1689_112_2_18_1_25_22_76,"S3D_CubeAxisRotation 1")
TRACE_MSG(S3D_ROTATION_1696_112_2_18_1_25_22_77,"S3D_CubeAxisRotation 2")
TRACE_MSG(S3D_ROTATION_1725_112_2_18_1_25_22_78,"S3D_CubeAxisRotation vertex %d x= %d y =%d")
TRACE_MSG(S3D_ROTATION_1959_112_2_18_1_25_22_79,"S3D_CubeAxisRotation 3")
TRACE_MSG(S3D_ROTATION_2293_112_2_18_1_25_23_80,"test_3D_Rotation 1 = %d\n")
TRACE_MSG(S3D_ROTATION_SE_2542_112_2_18_1_25_30_81,"S3D_CubeAxisRotation eye start")
TRACE_MSG(S3D_ROTATION_SE_2562_112_2_18_1_25_30_82,"S3D_CubeAxisRotation eye x = %d x= %d z =%d")
TRACE_MSG(S3D_ROTATION_SE_2570_112_2_18_1_25_30_83,"S3D_CubeAxisRotation axis x = %d x= %d z =%d axis=%d")
TRACE_MSG(S3D_ROTATION_SE_2577_112_2_18_1_25_30_84,"S3D_CubeAxisRotation 1")
TRACE_MSG(S3D_ROTATION_SE_2584_112_2_18_1_25_30_85,"S3D_CubeAxisRotation 2")
TRACE_MSG(S3D_ROTATION_SE_2613_112_2_18_1_25_30_86,"S3D_CubeAxisRotation vertex %d x= %d y =%d")
TRACE_MSG(S3D_ROTATION_SE_2844_112_2_18_1_25_30_87,"S3D_CubeAxisRotation 3")
TRACE_MSG(S3D_ROTATION_SE_3730_112_2_18_1_25_32_88,"S3D_CubeAxisRotation 1")
TRACE_MSG(S3D_ROTATION_SE_3737_112_2_18_1_25_32_89,"S3D_CubeAxisRotation 2")
TRACE_MSG(S3D_ROTATION_SE_11283_112_2_18_1_25_48_90,"s3d_MidRotation_Image: NO such color type blending in S3D_DATA_ARGB888! ")
TRACE_MSG(S3D_ROTATION_SE_11303_112_2_18_1_25_48_91,"s3d_MidRotation_Image: NO such color type blending in S3D_DATA_PAF_RGB888! ")
TRACE_MSG(S3D_ROTATION_SE_11309_112_2_18_1_25_48_92,"s3d_MidRotation_Image: NO such color type blending now! ")
TRACE_MSG(S3D_ROTATION_SE_11584_112_2_18_1_25_49_93,"s3d_MidRotation_Image: NO such color type blending in S3D_DATA_ARGB888! ")
TRACE_MSG(S3D_ROTATION_SE_11613_112_2_18_1_25_49_94,"s3d_MidRotation_Image: NO such color type blending in S3D_DATA_PAF_RGB888! ")
TRACE_MSG(S3D_ROTATION_SE_11621_112_2_18_1_25_49_95,"s3d_MidRotation_Image_shining: NO such color type blending now! ")
TRACE_MSG(S3D_ROTATION_SE_12061_112_2_18_1_25_50_96,"HTC_ALPHA, cost_time ONE frame = %d ms")
TRACE_MSG(SCALE_IMAGE_96_112_2_18_1_25_50_97,"[_AsyncScaleCallback] ret = %d, async_param = 0x%x")
TRACE_MSG(SCALE_IMAGE_168_112_2_18_1_25_51_98,"[GRAPH_ScaleImage] ARGB888 scaling not support async, change to sync mode")
TRACE_MSG(IMGPROC_SCALE_IMAGE_351_112_2_18_1_25_53_99,"[_ScaleImageARGB888] src_alpha_buf (0x%x, %d), dst_alpha_buf (0x%x, %d), dst_rgb_buf (0x%x, %d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_358_112_2_18_1_25_53_100,"[_ScaleImageARGB888] allocate alpha buffer failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_376_112_2_18_1_25_53_101,"[_ScaleImageARGB888] get alpha time = %d")
TRACE_MSG(IMGPROC_SCALE_IMAGE_398_112_2_18_1_25_53_102,"[_ScaleImageARGB888] _ScaleImage rgb888 failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_404_112_2_18_1_25_53_103,"[_ScaleImageARGB888] scale alpha time = %d")
TRACE_MSG(IMGPROC_SCALE_IMAGE_408_112_2_18_1_25_53_104,"[_ScaleImageARGB888] scale alpha done, out size (%d, %d), out addr (0x%x, %d), format (%d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_430_112_2_18_1_25_53_105,"[_ScaleImageARGB888] _ScaleImage rgb888 failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_436_112_2_18_1_25_53_106,"[_ScaleImageARGB888] scale rgb time = %d")
TRACE_MSG(IMGPROC_SCALE_IMAGE_440_112_2_18_1_25_53_107,"[_ScaleImageARGB888] scale rgb done, out size (%d, %d), out addr (0x%x, %d), format (%d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_451_112_2_18_1_25_53_108,"[_ScaleImageARGB888] merg alpha and rgb time = %d")
TRACE_MSG(IMGPROC_SCALE_IMAGE_486_112_2_18_1_25_53_109,"[_ScaleImageARGB565] get alpha time = %d")
TRACE_MSG(IMGPROC_SCALE_IMAGE_503_112_2_18_1_25_53_110,"[_ScaleImageARGB565] _ScaleImage rgb888 failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_509_112_2_18_1_25_53_111,"[_ScaleImageARGB565] scale alpha time = %d")
TRACE_MSG(IMGPROC_SCALE_IMAGE_513_112_2_18_1_25_53_112,"[_ScaleImageARGB565] scale alpha done, out size (%d, %d), out addr (0x%x, %d), format (%d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_528_112_2_18_1_25_53_113,"[_ScaleImageARGB565] _ScaleImage rgb888 failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_534_112_2_18_1_25_53_114,"[_ScaleImageARGB565] scale rgb time = %d")
TRACE_MSG(IMGPROC_SCALE_IMAGE_538_112_2_18_1_25_53_115,"[_ScaleImageARGB565] scale rgb done, out size (%d, %d), out addr (0x%x, %d), format (%d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_747_112_2_18_1_25_54_116,"[_SetScaleSize] output format = %d, dst chn0 (0x%x, %d), dst chn1 (0x%x, %d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_876_112_2_18_1_25_54_117,"[_SetScaleSize] src size (%d, %d), src rect (%d, %d, %d, %d), target size (%d, %d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_892_112_2_18_1_25_54_118,"[_SetScaleParam] _SetScaleInputChn failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_898_112_2_18_1_25_54_119,"[_SetScaleParam] _SetScaleOutputChn failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_904_112_2_18_1_25_54_120,"[_SetScaleParam] _SetScaleSize failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_984_112_2_18_1_25_54_121,"[GRAPH_ScaleImage] input/output parameter invalid")
TRACE_MSG(IMGPROC_SCALE_IMAGE_988_112_2_18_1_25_54_122,"[GRAPH_ScaleImage]")
TRACE_MSG(IMGPROC_SCALE_IMAGE_992_112_2_18_1_25_54_123,"[GRAPH_ScaleImage] src size (%d, %d), src tirm rect (%d, %d, %d, %d),")
TRACE_MSG(IMGPROC_SCALE_IMAGE_995_112_2_18_1_25_54_124,"[GRAPH_ScaleImage] src format (%d), src chn0 (0x%x, %d), chn1 (0x%x, %d),")
TRACE_MSG(IMGPROC_SCALE_IMAGE_999_112_2_18_1_25_54_125,"[GRAPH_ScaleImage] target size (%d, %d), target format (%d), target buf (0x%x, %d),")
TRACE_MSG(IMGPROC_SCALE_IMAGE_1011_112_2_18_1_25_54_126,"[GRAPH_ScaleImage] _SetScaleParam failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_1020_112_2_18_1_25_54_127,"[GRAPH_ScaleImage] get scale func failed")
TRACE_MSG(IMGPROC_SCALE_IMAGE_1034_112_2_18_1_25_54_128,"[GRAPH_ScaleImage] ret = %d, output size (%d, %d), output format (%d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_1038_112_2_18_1_25_54_129,"[GRAPH_ScaleImage]output chn0 (0x%x, %d), output chn1 (0x%x, %d)")
TRACE_MSG(IMGPROC_SCALE_IMAGE_1044_112_2_18_1_25_54_130,"[GRAPH_ScaleImage] spend time = %d")
TRACE_MSG(BMP_IO_94_112_2_18_1_26_19_131,"Not support bmp format")
TRACE_MSG(GIF_RES_DECODER_737_112_2_18_1_26_22_132,"----GIFRES_Dec point 0----")
TRACE_MSG(GIF_RES_DECODER_858_112_2_18_1_26_23_133,"----GIFRES_Dec point 3----")
TRACE_MSG(GIF_RES_DECODER_1903_112_2_18_1_26_25_134,"seek_next_image: return : %d")
TRACE_MSG(GIF_RES_DECODER_1918_112_2_18_1_26_25_135,"seek_next_image: return ")
TRACE_MSG(IMG_RES_DECODER_154_112_2_18_1_26_27_136,"\r\nDC_IRAM_Disable:unable to unlock IRAM")
TRACE_MSG(IMG_RES_DECODER_723_112_2_18_1_26_28_137,"IMG_RES_Free_Handler:filetype error!")
TRACE_MSG(JPEG_RES_DECODER_119_112_2_18_1_26_31_138,"JPEG_Display_Img")
TRACE_MSG(JPEG_RES_DECODER_148_112_2_18_1_26_31_139,"JPEG_Display_Img, jpeg size = (%d, %d), rect = (%d, %d, %d, %d)")
TRACE_MSG(JPEG_RES_DECODER_152_112_2_18_1_26_31_140,"JPEG_Display_Img, disp size = (%d, %d), rect = (%d, %d, %d, %d)")
TRACE_MSG(JPEG_RES_DECODER_155_112_2_18_1_26_31_141,"JPEG_Display_Img, img rect size = (%d, %d), disp rect size = (%d, %d)")
TRACE_MSG(JPEG_RES_DECODER_197_112_2_18_1_26_31_142,"'[JPEG_Display_Img] allocate buffer failed!")
TRACE_MSG(JPEG_RES_DECODER_202_112_2_18_1_26_31_143,"[JPEG_Display_Img] allocate buffer success! ")
TRACE_MSG(JPEG_RES_DECODER_204_112_2_18_1_26_31_144,"[JPEG_Display_Img] decode buf = 0x%x, size = %d, target buf = 0x%x, size = %d")
TRACE_MSG(JPEG_RES_DECODER_244_112_2_18_1_26_31_145,"[JPEG_Display_Img] decode standard jpeg, stream buf = 0x%x, size = %d, ")
TRACE_MSG(JPEG_RES_DECODER_287_112_2_18_1_26_31_146,"[JPEG_Display_Img] display image, image buf = 0x%x, disp buf = 0x%x")
TRACE_MSG(JPEG_RES_DECODER_305_112_2_18_1_26_31_147,"[JPEG_Display_Img] display image, image buf = 0x%x, disp buf = 0x%x")
TRACE_MSG(JPEG_RES_DECODER_320_112_2_18_1_26_31_148,"[JPEG_Display_Img] display image end")
TRACE_MSG(JPEG_RES_DECODER_324_112_2_18_1_26_31_149,"[JPEG_Display_Img] free decode buf = 0x%x")
TRACE_MSG(JPEG_RES_DECODER_331_112_2_18_1_26_31_150,"[JPEG_Display_Img] free target buf = 0x%x")
TRACE_MSG(JPEG_RES_DECODER_342_112_2_18_1_26_31_151,"[JPEG_Display_Img] out")
TRACE_MSG(JPEG_RES_DECODER_394_112_2_18_1_26_31_152,"JPEG_Display_Img, img rect = (%d, %d, %d, %d), dsip rect (%d, %d, %d, %d), dst format = %d")
TRACE_MSG(JPEG_RES_DECODER_441_112_2_18_1_26_32_153,"'[JPEG_Display_Img] allocate buffer failed!")
TRACE_MSG(JPEG_RES_DECODER_543_112_2_18_1_26_32_154,"[test_jpeg], src = 0x%x, dst = 0x%x")
TRACE_MSG(ROLL_1887_112_2_18_1_26_53_155,"[ROLL_GetFrame] +")
TRACE_MSG(ROLL_1906_112_2_18_1_26_53_156,"[ROLL_GetFrame] _CalcParam %d")
TRACE_MSG(ROLL_1912_112_2_18_1_26_53_157,"[ROLL_GetFrame] _DrawTop %d")
TRACE_MSG(ROLL_1918_112_2_18_1_26_53_158,"[ROLL_GetFrame] _DrawSpindleTop %d")
TRACE_MSG(ROLL_1924_112_2_18_1_26_53_159,"[ROLL_GetFrame] _DrawMiddle %d")
TRACE_MSG(ROLL_1930_112_2_18_1_26_53_160,"[ROLL_GetFrame] _DrawSpindleBottom %d")
TRACE_MSG(ROLL_1936_112_2_18_1_26_53_161,"[ROLL_GetFrame] _DrawBottom %d")
TRACE_MSG(ROLL_1942_112_2_18_1_26_53_162,"[ROLL_GetFrame] _DestorySrcArray %d")
TRACE_MSG(ROLL_1948_112_2_18_1_26_53_163,"[ROLL_GetFrame] -, %d")
TRACE_MSG(SOLID_2633_112_2_18_1_26_59_164,"[SOLID_Render] ticks = %d")
TRACE_MSG(SOLID_2801_112_2_18_1_26_59_165,"[sphere_test] frontage buf = 0x%x, backside buf = 0x%x")
TRACE_MSG(SOLID_2870_112_2_18_1_26_59_166,"[sphere_test] ticks = %d")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_537_112_2_18_1_27_3_167,"The frame rate is invalidated: should be larger than 1")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_556_112_2_18_1_27_4_168,"the Radius is too large, Radius_Max = %d, Radius_Now = %d")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_569_112_2_18_1_27_4_169,"[_SetContextCircleParam]: radius array memory allocate failed")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_777_112_2_18_1_27_4_170,"[_GetUpdateRectFromIndex]:the index is invalidated ")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_940_112_2_18_1_27_4_171,"[Effect_CURTAIN_Create]: the input is invalidate")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_947_112_2_18_1_27_4_172,"the data format does not support")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_955_112_2_18_1_27_4_173,"[Effect_CURTAIN_Create]: SE_CURTAIN_HANLDE_T memory allocate failed")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_965_112_2_18_1_27_4_174,"[Effect_CURTAIN_Create]: SE_CURTAIN_CONTEXT_T memory allocate failed")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_993_112_2_18_1_27_4_175,"[Effect_CURTAIN_Create]: _SetContextGraphParam function failed")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_1039_112_2_18_1_27_4_176,"the input param invalidate: input_ptr = %d, out_ptr = %d, handle = %d")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_1162_112_2_18_1_27_5_177,"the input param invalidate: data_chn_ptr = %d, img_size_ptr = %d, dst_chn_ptr = %d, update_param_ptr = %d")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_1169_112_2_18_1_27_5_178,"the data format does not support")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_1186_112_2_18_1_27_5_179,"the Radius is too large, Radius_Max = %d, Radius_Now = %d")
TRACE_MSG(SPECIAL_EFFECT_CURTAIN_1194_112_2_18_1_27_5_180,"Alpha Memory Malloc failed")
TRACE_MSG(IMG_TEST_108_112_2_18_1_27_18_181,"load image failed\r\n")
TRACE_MSG(IMG_TEST_115_112_2_18_1_27_18_182,"compress failed\r\n")
TRACE_MSG(IMG_TEST_121_112_2_18_1_27_18_183,"save failed\r\n")
TRACE_MSG(IMG_TEST_137_112_2_18_1_27_18_184,"load image failed\r\n")
TRACE_MSG(IMG_TEST_144_112_2_18_1_27_18_185,"compress failed\r\n")
TRACE_MSG(IMG_TEST_150_112_2_18_1_27_18_186,"save failed\r\n")
TRACE_MSG(IMG_TEST_165_112_2_18_1_27_18_187,"load image failed\r\n")
TRACE_MSG(IMG_TEST_172_112_2_18_1_27_18_188,"compress failed\r\n")
TRACE_MSG(IMG_TEST_188_112_2_18_1_27_18_189,"compress failed\r\n")
TRACE_MSG(IMG_TEST_194_112_2_18_1_27_18_190,"save failed\r\n")
TRACE_MSG(IMG_TEST_213_112_2_18_1_27_18_191,"\r\n load failed")
TRACE_MSG(IMG_TEST_220_112_2_18_1_27_18_192,"\r\n get info failed")
TRACE_MSG(IMG_TEST_249_112_2_18_1_27_18_193,"\r\n disp failed")
TRACE_MSG(IMG_TEST_266_112_2_18_1_27_18_194,"\r\n load failed")
TRACE_MSG(IMG_TEST_273_112_2_18_1_27_18_195,"\r\n get info failed")
TRACE_MSG(IMG_TEST_302_112_2_18_1_27_18_196,"\r\n disp failed")
TRACE_MSG(IMG_TEST_378_112_2_18_1_27_19_197,"\r\n get info failed")
END_TRACE_MAP(MS_REF_DIPLAY_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_DIPLAY_TRC_H_

