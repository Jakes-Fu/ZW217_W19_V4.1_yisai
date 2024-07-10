/******************************************************************************
 ** File Name:      ms_ref_isp_service_trc.h                                         *
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
//trace_id:55
#ifndef _MS_REF_ISP_SERVICE_TRC_H_
#define _MS_REF_ISP_SERVICE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define FMARK_CTRL_134_112_2_18_1_43_34_0 "_UpdateLcd_SendSig: command=%d"
#define FMARK_CTRL_210_112_2_18_1_43_34_1 "LCD_InvalidateRectWithFmark:%d,%d,%d,%d,%d,%d,%d"
#define FMARK_CTRL_211_112_2_18_1_43_34_2 "handle LCD_InvalidateRectWithFmark time = %d"
#define FMARK_CTRL_236_112_2_18_1_43_34_3 "_UpdateLcdCtrl_SearchNextBuf:pos=%d,%d"
#define FMARK_CTRL_242_112_2_18_1_43_34_4 "s->save_display_data_info[temp][INFO_FLAG_INDEX] is %d"
#define FMARK_CTRL_273_112_2_18_1_43_34_5 "_UpdateLcdCtrl_Done:next buf index = %d"
#define FMARK_CTRL_285_112_2_18_1_43_34_6 "not saved data!"
#define FMARK_CTRL_315_112_2_18_1_43_34_7 "_UpdateLcd_MessageLoop:command=0x%x param_num=0x%x"
#define FMARK_CTRL_337_112_2_18_1_43_34_8 "---discard a frame data!--"
#define FMARK_CTRL_380_112_2_18_1_43_34_9 "set s->is_use_fmark 1"
#define FMARK_CTRL_395_112_2_18_1_43_34_10 "handle UPDATELCDCTRL_FMARK_SIG time = %d"
#define FMARK_CTRL_430_112_2_18_1_43_34_11 "UPDATELCDCTRL_UPDATE_DONE_SIG:callback_param is 0x%x,display_pos=%d,%d,%d"
#define FMARK_CTRL_437_112_2_18_1_43_34_12 "_UpdateLcd_MessageLoop command is error!"
#define FMARK_CTRL_455_112_2_18_1_43_34_13 "FMARK Int: time = %d"
#define FMARK_CTRL_472_112_2_18_1_43_34_14 "UpdateLcdCtrl_Init start!"
#define FMARK_CTRL_511_112_2_18_1_43_34_15 "UpdateLcdCtrl_Init end!"
#define ISP_SERVICE_275_112_2_18_1_43_35_16 "ISP_SERVICE:_ISP_UpdateCache default %d line error!"
#define ISP_SERVICE_299_112_2_18_1_43_35_17 "ISP_SERVICE:_ISP_AllocMjpegScaleCoeff "
#define ISP_SERVICE_310_112_2_18_1_43_35_18 "ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, capture_h_scale_coeff_ptr = 0x%x "
#define ISP_SERVICE_322_112_2_18_1_43_35_19 "ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, capture_v_scale_coeff_ptr = 0x%x "
#define ISP_SERVICE_334_112_2_18_1_43_35_20 "ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, review_h_scale_coeff_ptr = 0x%x "
#define ISP_SERVICE_346_112_2_18_1_43_35_21 "ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, review_v_scale_coeff_ptr = 0x%x "
#define ISP_SERVICE_358_112_2_18_1_43_35_22 "ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, jpeg_h_scale_coeff_ptr = 0x%x "
#define ISP_SERVICE_370_112_2_18_1_43_35_23 "ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, jpeg_v_scale_coeff_ptr = 0x%x "
#define ISP_SERVICE_391_112_2_18_1_43_35_24 "ISP_SERVICE:_ISP_AllocScaleCoeff "
#define ISP_SERVICE_401_112_2_18_1_43_35_25 "ISP_SERVICE:_ISP_AllocScaleCoeff, scale_vcoeff_tmp_ptr = 0x%x "
#define ISP_SERVICE_412_112_2_18_1_43_35_26 "ISP_SERVICE:_ISP_AllocScaleCoeff, scale_hcoeff_tmp_ptr = 0x%x "
#define ISP_SERVICE_424_112_2_18_1_43_35_27 "ISP_SERVICE:_ISP_AllocScaleCoeff, scale_tmp_ptr = 0x%x "
#define ISP_SERVICE_431_112_2_18_1_43_35_28 "ISP_SERVICE:_ISP_AllocScaleCoeff , h_ptr = 0x%x, v_ptr = 0x%x"
#define ISP_SERVICE_455_112_2_18_1_43_35_29 "ISP_SERVICE:_ISP_FreeScaleCoeff "
#define ISP_SERVICE_563_112_2_18_1_43_36_30 "ISP_SERVICE:_ISP_GetImgSize default %d line error!"
#define ISP_SERVICE_1043_112_2_18_1_43_37_31 "ISP_SERVICE:_ISP_InvalidateRect_Image default %d line error!"
#define ISP_SERVICE_1125_112_2_18_1_43_37_32 "ISP_SERVICE:_ISP_InvalidateRect_Image default %d line error!"
#define ISP_SERVICE_1180_112_2_18_1_43_37_33 "_ISP_DispImgCallback"
#define ISP_SERVICE_1209_112_2_18_1_43_37_34 "ISP_SERVICE:_ISP_DispImgUseCallback rotation angel error at line %d!"
#define ISP_SERVICE_1359_112_2_18_1_43_37_35 "NULL isp mutex, direct return"
#define ISP_SERVICE_1420_112_2_18_1_43_37_36 "NULL isp peek signal mutex, direct return"
#define ISP_SERVICE_1585_112_2_18_1_43_38_37 "ISP_SERVICE:_ISP_PeekSignal at %d"
#define ISP_SERVICE_1604_112_2_18_1_43_38_38 "ISP_SERVICE:_ISP_PeekSignal:%d"
#define ISP_SERVICE_1615_112_2_18_1_43_38_39 "ISP_SERVICE:_ISP_PeekSignal OK at %d"
#define ISP_SERVICE_1769_112_2_18_1_43_38_40 "ISP_SERVICE:_ISP_ISR_CapEndOfFrameLow default %d line error!"
#define ISP_SERVICE_1988_112_2_18_1_43_38_41 "ISP_SERVICE:_ISP_ISR_SensorLineErr\r\n"
#define ISP_SERVICE_2003_112_2_18_1_43_39_42 "ISP_SERVICE:_ISP_ISR_SensorLineErrHigh"
#define ISP_SERVICE_2024_112_2_18_1_43_39_43 "ISP_SERVICE:_ISP_ISR_PathFIFOOverflowed service: 0x%x "
#define ISP_SERVICE_2078_112_2_18_1_43_39_44 "ISP_SERVICE:_ISP_ISR_PathFIFOOverflowed service: 0x%x "
#define ISP_SERVICE_2095_112_2_18_1_43_39_45 "ISP_SERVICE:_ISP_ISR_SensorFrameErrLow"
#define ISP_SERVICE_2109_112_2_18_1_43_39_46 "ISP_SERVICE:_ISP_ISR_SensorFrameErrHigh"
#define ISP_SERVICE_2126_112_2_18_1_43_39_47 "ISP_SERVICE:_ISP_ISR_JpegBufferverflowedLow"
#define ISP_SERVICE_2155_112_2_18_1_43_39_48 "ISP_SERVICE:_ISP_ISR_JpegBufferverflowedHigh default %d line error!"
#define ISP_SERVICE_2159_112_2_18_1_43_39_49 "ISP_SERVICE:_ISP_ISR_JpegBufferverflowedHigh"
#define ISP_SERVICE_2175_112_2_18_1_43_39_50 "ISP_SERVICE:_ISP_TimerOutHandler service: %d , %dms"
#define ISP_SERVICE_2506_112_2_18_1_43_40_51 "[_ISP_CalculateScaleCoeff] shift_bits = %d, sub enable = %d, in size (%d, %d), out size = (%d, %d)"
#define ISP_SERVICE_2543_112_2_18_1_43_40_52 "ISP_SERVICE: ISP Scale coeff error ! in (%d X %d), out (%d X %d)"
#define ISP_SERVICE_2550_112_2_18_1_43_40_53 "ISP_SERVICE: ISP Scale coeff error ! in (%d X %d), out (%d X %d)"
#define ISP_SERVICE_2557_112_2_18_1_43_40_54 "ISP_SERVICE: GenScaleCoeff memory error !"
#define ISP_SERVICE_2565_112_2_18_1_43_40_55 "ISP_SERVICE: GenScaleCoeff failed !"
#define ISP_SERVICE_2573_112_2_18_1_43_40_56 "ISP_SERVICE: reset hor coef to scaling up coef"
#define ISP_SERVICE_2580_112_2_18_1_43_40_57 "ISP_SERVICE: reset ver coef to scaling up coef"
#define ISP_SERVICE_2584_112_2_18_1_43_40_58 "ISP_SERVICE: GenScaleCoeff in size (%d X %d), out size (%d X %d)"
#define ISP_SERVICE_2587_112_2_18_1_43_40_59 "ISP_SERVICE: GenScaleCoeff hor ptr = 0x%x, ver ptr = 0x%x, temp = 0x%x"
#define ISP_SERVICE_2693_112_2_18_1_43_40_60 "ISP_SERVICE:_ISP_ImgTerminalLineCopy default %d line error!"
#define ISP_SERVICE_2963_112_2_18_1_43_41_61 "ISP_SERVICE: _ISP_SetPreviewParam img format is %x  %d line error"
#define ISP_SERVICE_3167_112_2_18_1_43_41_62 "ISP_SERVICE: _ISP_StopPreview"
#define ISP_SERVICE_3395_112_2_18_1_43_41_63 "ISP_SERVICE:_ISP_InitializeBuf default %d line error!"
#define ISP_SERVICE_3429_112_2_18_1_43_42_64 "ISP_SERVICE: _ISP_SetJpgBufLimitSize img format : %d, %d, %d"
#define ISP_SERVICE_3470_112_2_18_1_43_42_65 "ISP_SERVICE: _ISP_SetCaptureParam img format is %x  %d line error"
#define ISP_SERVICE_3485_112_2_18_1_43_42_66 "ISP_SERVICE: _ISP_SetCaptureParam, ow:%d"
#define ISP_SERVICE_3486_112_2_18_1_43_42_67 "ISP_SERVICE: _ISP_SetCaptureParam, oh:%d"
#define ISP_SERVICE_3487_112_2_18_1_43_42_68 "ISP_SERVICE: _ISP_SetCaptureParam, zw:%d"
#define ISP_SERVICE_3488_112_2_18_1_43_42_69 "ISP_SERVICE: _ISP_SetCaptureParam, zh:%d"
#define ISP_SERVICE_3502_112_2_18_1_43_42_70 "ISP_SERVICE: factor1: %d"
#define ISP_SERVICE_3506_112_2_18_1_43_42_71 "ISP_SERVICE: _ISP_SetCaptureParam, cap trim 2"
#define ISP_SERVICE_3526_112_2_18_1_43_42_72 "ISP_SERVICE: _ISP_SetCaptureParam, cap trim 0"
#define ISP_SERVICE_3534_112_2_18_1_43_42_73 "ISP_SERVICE: _ISP_SetCaptureParam, cap trim 1"
#define ISP_SERVICE_3543_112_2_18_1_43_42_74 "ISP_SERVICE: _ISP_SetCaptureParam, cap trim 2"
#define ISP_SERVICE_4088_112_2_18_1_43_43_75 "ISP_SERVICE: _ISP_SetMjpegParam img format is %x  %d line error"
#define ISP_SERVICE_4154_112_2_18_1_43_43_76 "ISP_SERVICE: _ISP_CalcScaleParam failed"
#define ISP_SERVICE_4184_112_2_18_1_43_43_77 "ISP_SERVICE: _ISP_CalcScaleParam failed"
#define ISP_SERVICE_4239_112_2_18_1_43_43_78 "ISP_SERVICE: _ISP_CalcScaleParam failed"
#define ISP_SERVICE_4282_112_2_18_1_43_43_79 "ISP_SERVICE: _ISP_SetMjpegParam img format is %x  %d line error"
#define ISP_SERVICE_4461_112_2_18_1_43_44_80 "ISP_SERVICE: _ISP_StopMjpg"
#define ISP_SERVICE_4508_112_2_18_1_43_44_81 "ISP_SERVICE:Zoom array ptr=0x%x, input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_4637_112_2_18_1_43_44_82 "ISP_SERVICE: ISP_ServiceZoom call back 1"
#define ISP_SERVICE_4661_112_2_18_1_43_44_83 "ISP_Service: _ISP_StartDvZoom"
#define ISP_SERVICE_4673_112_2_18_1_43_44_84 "ISP_Service: _ISP_StartDvZoom, _ISP_CalcScaleParam failed"
#define ISP_SERVICE_4699_112_2_18_1_43_44_85 "ISP_Service: _ISP_StartDvZoom call back"
#define ISP_SERVICE_5039_112_2_18_1_43_45_86 "ISP_Service: _ISP_ServiceClose"
#define ISP_SERVICE_5301_112_2_18_1_43_46_87 "ISP_SERVICE: ISP_ServiceSetSensorParam2, 0x%x"
#define ISP_SERVICE_5316_112_2_18_1_43_46_88 "ISP_SERVICE: ISP_ServiceSetSensorParam2, 0x%x"
#define ISP_SERVICE_5340_112_2_18_1_43_46_89 "ISP_SERVICE: _ISP_SetSensorEofCtrl, 0x%x"
#define ISP_SERVICE_5377_112_2_18_1_43_46_90 "ISP_SERVICE: All width must be 4 pixel align rect x%d, inpur rect w%d, disp rect w%d"
#define ISP_SERVICE_5383_112_2_18_1_43_46_91 "ISP_SERVICE: Invalid lcd id %d"
#define ISP_SERVICE_5389_112_2_18_1_43_46_92 "ISP_SERVICE: Invalid zoom level %d"
#define ISP_SERVICE_5398_112_2_18_1_43_46_93 "ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
#define ISP_SERVICE_5408_112_2_18_1_43_46_94 "ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
#define ISP_SERVICE_5414_112_2_18_1_43_46_95 "ISP_SERVICE: the lcd ration param %d is error!"
#define ISP_SERVICE_5423_112_2_18_1_43_46_96 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_5432_112_2_18_1_43_46_97 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_5463_112_2_18_1_43_46_98 "ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
#define ISP_SERVICE_5472_112_2_18_1_43_46_99 "ISP_SERVICE: All width must be 4 pixel align, input rect w%d h%d, output w%d h%d"
#define ISP_SERVICE_5490_112_2_18_1_43_46_100 "ISP_SERVICE:The capture size w:%d h:%d ,the capture buffer 0x%x is not enough"
#define ISP_SERVICE_5500_112_2_18_1_43_46_101 "ISP_SERVICE: Jpg sensor output addr 0x%x is error"
#define ISP_SERVICE_5529_112_2_18_1_43_46_102 "ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
#define ISP_SERVICE_5540_112_2_18_1_43_46_103 "ISP_SERVICE: Review Paramter not align Input rect: x:%d, w:%d output w:%d"
#define ISP_SERVICE_5548_112_2_18_1_43_46_104 "ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
#define ISP_SERVICE_5557_112_2_18_1_43_46_105 "ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
#define ISP_SERVICE_5561_112_2_18_1_43_46_106 "ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
#define ISP_SERVICE_5573_112_2_18_1_43_46_107 "ISP_SERVICE: Input format %d is error"
#define ISP_SERVICE_5581_112_2_18_1_43_46_108 "ISP_SERVICE: Input data y addr 0x%x uv addr 0x%x isn't aligned!"
#define ISP_SERVICE_5591_112_2_18_1_43_46_109 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_5601_112_2_18_1_43_46_110 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_5615_112_2_18_1_43_46_111 "ISP_SERVICE:The src size w:%d h:%d ,The source review buffer 0x%x is not enough"
#define ISP_SERVICE_5624_112_2_18_1_43_46_112 "ISP_SERVICE:The src size w:%d h:%d ,The source review y buffer 0x%x is not enough"
#define ISP_SERVICE_5648_112_2_18_1_43_46_113 "ISP_SERVICE:The review data format %d error!"
#define ISP_SERVICE_5654_112_2_18_1_43_46_114 "ISP_SERVICE:The src size w:%d h:%d ,The source review uv buffer 0x%x is not enough"
#define ISP_SERVICE_5677_112_2_18_1_43_46_115 "ISP_SERVICE:The review disp size w:%d h:%d ,the review buffer 0x%x is not enough"
#define ISP_SERVICE_5707_112_2_18_1_43_47_116 "ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
#define ISP_SERVICE_5715_112_2_18_1_43_47_117 "ISP_SERVICE:Input rect w%d, h%d capture w%d, h%d ISP don't support scale up when Mjpeg capture!"
#define ISP_SERVICE_5721_112_2_18_1_43_47_118 "ISP_SERVICE: the lcd ration param %d is error!"
#define ISP_SERVICE_5730_112_2_18_1_43_47_119 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_5739_112_2_18_1_43_47_120 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_5749_112_2_18_1_43_47_121 "ISP_SERVICE:The mjpg capture size w:%d h:%d ,the mjpg capture buffer 0x%x is not enough"
#define ISP_SERVICE_5759_112_2_18_1_43_47_122 "ISP_SERVICE:The mjpg review disp size w:%d h:%d ,the review buffer 0x%x is not enough"
#define ISP_SERVICE_5765_112_2_18_1_43_47_123 "ISP_SERVICE: The mjpg get capture data callback is PNULL"
#define ISP_SERVICE_5792_112_2_18_1_43_47_124 "ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
#define ISP_SERVICE_5800_112_2_18_1_43_47_125 "ISP_SERVICE:Input rect w%d, h%d capture w%d, h%d ISP don't support scale up when Mjpeg capture!"
#define ISP_SERVICE_5807_112_2_18_1_43_47_126 "ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
#define ISP_SERVICE_5816_112_2_18_1_43_47_127 "ISP_SERVICE:The mjpg capture size w:%d h:%d ,the mjpg capture buffer 0x%x is not enough"
#define ISP_SERVICE_5822_112_2_18_1_43_47_128 "ISP_SERVICE: The mjpg get capture data callback is PNULL"
#define ISP_SERVICE_5897_112_2_18_1_43_47_129 "ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
#define ISP_SERVICE_5903_112_2_18_1_43_47_130 "ISP_SERVICE: Scale Paramter not align Input rect: x%d, w%d "
#define ISP_SERVICE_5911_112_2_18_1_43_47_131 "ISP_SERVICE: Scale Paramter not align  Output: w%d"
#define ISP_SERVICE_5919_112_2_18_1_43_47_132 "ISP_SERVICE: Scale Paramter not align  Output: w%d"
#define ISP_SERVICE_5929_112_2_18_1_43_47_133 "ISP_SERVICE: Scale multiply error input_rect: w%d, h%d output: w%d, h%d"
#define ISP_SERVICE_5935_112_2_18_1_43_47_134 "ISP_SERVICE: Scale mode error out_width: w%d, scale_mode:%d"
#define ISP_SERVICE_5941_112_2_18_1_43_47_135 "ISP_SERVICE: Slice scale callback error: get_ptr:0x=%x"
#define ISP_SERVICE_5948_112_2_18_1_43_47_136 "ISP_SERVICE:The input adr is null error"
#define ISP_SERVICE_5966_112_2_18_1_43_47_137 "ISP_SERVICE:The scale data format %d error!"
#define ISP_SERVICE_5977_112_2_18_1_43_47_138 "ISP_SERVICE: The mjpg get capture data callback is PNULL"
#define ISP_SERVICE_6071_112_2_18_1_43_47_139 "ISP_SERVICE:_ISP_PathFifoOverFlowHandle default %d line error!"
#define ISP_SERVICE_6129_112_2_18_1_43_47_140 "ISP_SERVICE:_ISP_ErrorHandle default %d line error!"
#define ISP_SERVICE_6151_112_2_18_1_43_48_141 "ISP Service:preview fram rate time: %dms"
#define ISP_SERVICE_6760_112_2_18_1_43_49_142 "ISP_SERVICE:_ISP_Service_GetIspPriority: %s, queue_name: %s, priority: %d "
#define ISP_SERVICE_6783_112_2_18_1_43_49_143 "ISP_SERVICE:_ISP_Service_Set_Priority, priority: %d"
#define ISP_SERVICE_6820_112_2_18_1_43_49_144 "ISP_SERVICE:_ISP_Service_GetCallerPriority: %s, queue_name: %s, priority: %d "
#define ISP_SERVICE_6860_112_2_18_1_43_49_145 "ISP_SERVICE: _ISP_ServiceHandleRoutine the SignalCode 0x%x, error"
#define ISP_SERVICE_6876_112_2_18_1_43_49_146 "ISP_SERVICE: Recieve Request, service=0x%x command=0x%x, cur time=%d"
#define ISP_SERVICE_7072_112_2_18_1_43_49_147 "ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!"
#define ISP_SERVICE_7152_112_2_18_1_43_50_148 "ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!"
#define ISP_SERVICE_7210_112_2_18_1_43_50_149 "ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!"
#define ISP_SERVICE_7256_112_2_18_1_43_50_150 "ISP_SERVICE: ISP_SERVICE_START_TIMEOUT_SIG, service=0x%x"
#define ISP_SERVICE_7266_112_2_18_1_43_50_151 "ISP_SERVICE: ISP_SERVICE_STOP_TIMEOUT_SIG, service=0x%x"
#define ISP_SERVICE_7272_112_2_18_1_43_50_152 "ISP_SERVICE: ISP_SERVICE_SENSOR_EOF_SIG, service=0x%x"
#define ISP_SERVICE_7277_112_2_18_1_43_50_153 "ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!"
#define ISP_SERVICE_7340_112_2_18_1_43_50_154 "[ISPLOCK: ISP_ServiceLock] owner = %d, cur owner = %d, sem_count = %d, wait option = %d"
#define ISP_SERVICE_7347_112_2_18_1_43_50_155 "[ISPLOCK: ISP_ServiceLock] lock done, owner = %d"
#define ISP_SERVICE_7351_112_2_18_1_43_50_156 "[ISPLOCK: ISP_ServiceLock] lock timeout, owner = %d"
#define ISP_SERVICE_7378_112_2_18_1_43_50_157 "[ISPLOCK: ISP_ServiceUnlock] owner = %d, cur owner = %d, sem_count = %d"
#define ISP_SERVICE_7385_112_2_18_1_43_50_158 "[ISPLOCK: ISP_ServiceUnlock] unlock done, owner = %d"
#define ISP_SERVICE_7410_112_2_18_1_43_50_159 "[ISPLOCK: ISP_ServiceIsIdle] sem_count = %d, susp_count = %d"
#define ISP_SERVICE_7522_112_2_18_1_43_50_160 "ISP_SERVICE:ISP_ServiceOpen"
#define ISP_SERVICE_7527_112_2_18_1_43_50_161 "ISP_SERVICE: _ISP_AllocScaleCoeff failed !"
#define ISP_SERVICE_7555_112_2_18_1_43_50_162 "ISP_SERVICE: ISP_ServiceClose, cur task: 0x%x"
#define ISP_SERVICE_7579_112_2_18_1_43_50_163 "ISP_SERVICE: ISP_ServiceClose not msg"
#define ISP_SERVICE_7640_112_2_18_1_43_51_164 "ISP_SERVICE: Register Function is PNULL"
#define ISP_SERVICE_7662_112_2_18_1_43_51_165 "ISP_SERVICE:ISP_ServiceRegister default %d line error!"
#define ISP_SERVICE_7681_112_2_18_1_43_51_166 "ISP_SERVICE:ISP_ServiceSetPreviewParam"
#define ISP_SERVICE_7685_112_2_18_1_43_51_167 "ISP_SERVICE: Preview Param Ptr is PNULL %d line error"
#define ISP_SERVICE_7692_112_2_18_1_43_51_168 "ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_7695_112_2_18_1_43_51_169 "ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d"
#define ISP_SERVICE_7698_112_2_18_1_43_51_170 "ISP_SERVICE:target_rect.x = %d,target_rect.y = %d,target_rect.w = %d,target_rect.h = %d"
#define ISP_SERVICE_7700_112_2_18_1_43_51_171 "ISP_SERVICE: param_ptr->init_zoom_level = %d"
#define ISP_SERVICE_7702_112_2_18_1_43_51_172 "ISP_SERVICE: param_ptr->disp_rotation preview = %d"
#define ISP_SERVICE_7731_112_2_18_1_43_51_173 "ISP_SERVICE:ISP_ServiceStartPreview"
#define ISP_SERVICE_7737_112_2_18_1_43_51_174 "ISP_SERVICE: The state is not idle before Preview"
#define ISP_SERVICE_7766_112_2_18_1_43_51_175 "ISP_ServiceStartPreview: preview start error"
#define ISP_SERVICE_7820_112_2_18_1_43_51_176 "ISP_SERVICE: Capture Param Ptr is PNULL %d line error"
#define ISP_SERVICE_7826_112_2_18_1_43_51_177 "ISP_SERVICE:ISP_ServiceSetCaptureParam"
#define ISP_SERVICE_7829_112_2_18_1_43_51_178 "ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_7831_112_2_18_1_43_51_179 "ISP_SERVICE:output.w = %d,output.h = %d"
#define ISP_SERVICE_7833_112_2_18_1_43_51_180 "ISP_SERVICE: skip num: %d, in_data_format: %d, out_data_format: %d"
#define ISP_SERVICE_7835_112_2_18_1_43_51_181 "ISP_SERVICE: cap_addr: 0x%x, size: 0x%x"
#define ISP_SERVICE_7862_112_2_18_1_43_51_182 "ISP_SERVICE:ISP_ServiceStartSyncCapture, in time: %d "
#define ISP_SERVICE_7868_112_2_18_1_43_51_183 "ISP_SERVICE: The state is not idle before Sync Capture"
#define ISP_SERVICE_7892_112_2_18_1_43_51_184 "ISP_SERVICE:ISP_ServiceStartSyncCapture time out"
#define ISP_SERVICE_7909_112_2_18_1_43_51_185 "ISP_SERVICE:ISP_ServiceStartSyncCapture, out time: %d "
#define ISP_SERVICE_7934_112_2_18_1_43_51_186 "ISP_SERVICE:ISP_ServiceStartSyncCapture, in time: %d "
#define ISP_SERVICE_7940_112_2_18_1_43_51_187 "ISP_SERVICE: The state is not idle before Sync Capture"
#define ISP_SERVICE_7966_112_2_18_1_43_51_188 "ISP_SERVICE:ISP_ServiceStartSyncCapture time out"
#define ISP_SERVICE_7974_112_2_18_1_43_51_189 "ISP_SERVICE:ISP_ServiceStartSyncCapture, out time: %d "
#define ISP_SERVICE_7998_112_2_18_1_43_51_190 "ISP_SERVICE:ISP_ServiceStartAsyncCapture"
#define ISP_SERVICE_8002_112_2_18_1_43_51_191 "ISP_SERVICE: The state is not idle before Async Capture"
#define ISP_SERVICE_8050_112_2_18_1_43_51_192 "ISP_SERVICE:ISP_ServiceSetReviewParam"
#define ISP_SERVICE_8054_112_2_18_1_43_51_193 "ISP_SERVICE: Review Param Ptr is PNULL %d line error"
#define ISP_SERVICE_8060_112_2_18_1_43_51_194 "ISP_SERVICE:input.w = %d, input.h = %d"
#define ISP_SERVICE_8063_112_2_18_1_43_51_195 "ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_8065_112_2_18_1_43_51_196 "ISP_SERVICE:display.x=%d,display.y=%d,display.w=%d,display.h=%d"
#define ISP_SERVICE_8067_112_2_18_1_43_51_197 "ISP_SERVICE:target.x=%d,target.y=%d,target.w=%d,target.h=%d"
#define ISP_SERVICE_8069_112_2_18_1_43_51_198 "ISP_SERVICE:scale line = %d"
#define ISP_SERVICE_8071_112_2_18_1_43_51_199 "ISP_SERVICE: param_ptr->disp_rotation = %d"
#define ISP_SERVICE_8072_112_2_18_1_43_51_200 "ISP_SERVICE: param_ptr->disp_format = %d"
#define ISP_SERVICE_8074_112_2_18_1_43_51_201 "ISP_SERVICE: src y_addr = 0x%x, uv_addr=0x%x"
#define ISP_SERVICE_8075_112_2_18_1_43_51_202 "ISP_SERVICE: rot y_addr = 0x%x, uv_addr=0x%x"
#define ISP_SERVICE_8076_112_2_18_1_43_51_203 "ISP_SERVICE: param_ptr->disp_addr = 0x%x"
#define ISP_SERVICE_8103_112_2_18_1_43_51_204 "ISP_SERVICE: ISP_ServiceStartReview: %dms"
#define ISP_SERVICE_8109_112_2_18_1_43_51_205 "ISP_SERVICE: The staet is idle defore Review"
#define ISP_SERVICE_8139_112_2_18_1_43_52_206 "ISP_SERVICE: ISP_ServiceStartReview end: %dms"
#define ISP_SERVICE_8160_112_2_18_1_43_52_207 "ISP_SERVICE:ISP_ServiceSetScaleParam"
#define ISP_SERVICE_8164_112_2_18_1_43_52_208 "ISP_SERVICE: Capture Param Ptr is PNULL %d line error"
#define ISP_SERVICE_8171_112_2_18_1_43_52_209 "ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_8173_112_2_18_1_43_52_210 "ISP_SERVICE:output.w = %d,output.h = %d"
#define ISP_SERVICE_8174_112_2_18_1_43_52_211 "ISP_SERVICE: in y_addr = 0x%x, uv_addr=0x%x"
#define ISP_SERVICE_8175_112_2_18_1_43_52_212 "ISP_SERVICE: out y_addr = 0x%x, uv_addr=0x%x"
#define ISP_SERVICE_8176_112_2_18_1_43_52_213 "ISP_SERVICE: slice line: %d, is_f:%d,is_l:%d"
#define ISP_SERVICE_8209_112_2_18_1_43_52_214 "ISP_SERVICE: ISP_ServiceScale"
#define ISP_SERVICE_8215_112_2_18_1_43_52_215 "ISP_SERVICE: The staet is idle before Scale"
#define ISP_SERVICE_8288_112_2_18_1_43_52_216 "ISP_SERVICE: Mjpg Param Ptr is PNULL %d line error"
#define ISP_SERVICE_8294_112_2_18_1_43_52_217 "ISP_SERVICE: ISP_ServiceSetMjpegParam"
#define ISP_SERVICE_8296_112_2_18_1_43_52_218 "ISP_SERVICE:input_size.w = %d,input_size.h = %d"
#define ISP_SERVICE_8299_112_2_18_1_43_52_219 "ISP_SERVICE:input_rect.x = %d, input_rect.y = %d,input_rect.w = %d, input_rect.h = %d"
#define ISP_SERVICE_8301_112_2_18_1_43_52_220 "ISP_SERVICE:capture_size.w = %d,capture_size.h = %d"
#define ISP_SERVICE_8304_112_2_18_1_43_52_221 "ISP_SERVICE:jpeg_target_rect.x = %d, jpeg_target_rect.y = %d,jpeg_target_rect.w = %d, jpeg_target_rect.h = %d"
#define ISP_SERVICE_8306_112_2_18_1_43_52_222 "ISP_SERVICE:jpeg_target_size.w = %d,jpeg_target_size.h = %d"
#define ISP_SERVICE_8309_112_2_18_1_43_52_223 "ISP_SERVICE:disp_rect.x = %d, disp_rect.y = %d,disp_rect.w = %d, disp_rect.h = %d"
#define ISP_SERVICE_8312_112_2_18_1_43_52_224 "ISP_SERVICE:target_src_rect.x = %d, target_src_rect.y = %d,target_src_rect.w = %d, target_src_rect.h = %d"
#define ISP_SERVICE_8315_112_2_18_1_43_52_225 "ISP_SERVICE:target_rect.x = %d, target_rect.y = %d,target_rect.w = %d, target_rect.h = %d"
#define ISP_SERVICE_8340_112_2_18_1_43_52_226 "ISP_SERVICE: ISP_ServiceStartMjpeg"
#define ISP_SERVICE_8344_112_2_18_1_43_52_227 "ISP_SERVICE: The state is not idle before StartMjpeg"
#define ISP_SERVICE_8391_112_2_18_1_43_52_228 "ISP_SERVICE: ISP_ServiceSetUpccParam"
#define ISP_SERVICE_8395_112_2_18_1_43_52_229 "ISP_SERVICE: Upcc Param Ptr is PNULL %d line error"
#define ISP_SERVICE_8402_112_2_18_1_43_52_230 "ISP_SERVICE:input_rect.x = %d, input_rect.y = %d,input_rect.w = %d, input_rect.h = %d"
#define ISP_SERVICE_8404_112_2_18_1_43_52_231 "ISP_SERVICE:capture_size.w = %d,capture_size.h = %d"
#define ISP_SERVICE_8429_112_2_18_1_43_52_232 "ISP_SERVICE: ISP_ServiceStartUPCC"
#define ISP_SERVICE_8433_112_2_18_1_43_52_233 "ISP_SERVICE: The state is not idle before upcc"
#define ISP_SERVICE_8475_112_2_18_1_43_52_234 "ISP_SERVICE: ISP_ServicePauseMjpeg"
#define ISP_SERVICE_8497_112_2_18_1_43_52_235 "ISP_SERVICE: ISP_ServiceResumeMjpeg"
#define ISP_SERVICE_8522_112_2_18_1_43_52_236 "ISP_SERVICE: get Sensor frame Ptr is PNULL %d line error"
#define ISP_SERVICE_8546_112_2_18_1_43_52_237 "ISP_SERVICE: get Sensor frame alloc %d line error"
#define ISP_SERVICE_8571_112_2_18_1_43_52_238 "ISP_SERVICE: ISP_ServiceFeatureStop : service:0x%x, task: 0x%x, %dms"
#define ISP_SERVICE_8584_112_2_18_1_43_52_239 "ISP_SERVICE: ISP_ServiceFeatureStop nothing end: %dms "
#define ISP_SERVICE_8627_112_2_18_1_43_53_240 "ISP_SERVICE: ISP_ServiceFeatureStop Stop Capture"
#define ISP_SERVICE_8636_112_2_18_1_43_53_241 "ISP_SERVICE: ISP_ServiceFeatureStop Stop Review"
#define ISP_SERVICE_8690_112_2_18_1_43_53_242 "ISP_SERVICE:ISP_ServiceFeatureStop default %d line error!"
#define ISP_SERVICE_8700_112_2_18_1_43_53_243 "ISP_SERVICE: ISP_ServiceFeatureStop TimeOut"
#define ISP_SERVICE_8709_112_2_18_1_43_53_244 "ISP_SERVICE: ISP_ServiceFeatureStop end: %dms "
#define ISP_SERVICE_8753_112_2_18_1_43_53_245 "ISP_SERVICE: ISP_ServiceZoom, zoom_level : %d"
#define ISP_SERVICE_8757_112_2_18_1_43_53_246 "ISP_SERVICE: The digital zoom callback is null"
#define ISP_SERVICE_8770_112_2_18_1_43_53_247 "ISP_SERVICE: ISP Service States is : %d"
#define ISP_SERVICE_8780_112_2_18_1_43_53_248 "ISP_SERVICE: ISP_ServiceZoom call back"
#define ISP_SERVICE_8820_112_2_18_1_43_53_249 "ISP_SERVICE: Sensor Param Ptr is PNULL %d line error"
#define ISP_SERVICE_8829_112_2_18_1_43_53_250 "ISP_SERVICE: ISP_ServiceSetSensorParam, 0x%x"
#define ISP_SERVICE_8906_112_2_18_1_43_53_251 "ISP_SERVICE: ISP_ServiceSetSensorSignalInfo: p_skip:%d, img_pattern:%d, v_sync:%d, h_sync:%d"
#define ISP_SERVICE_8907_112_2_18_1_43_53_252 "ISP_SERVICE: ISP_ServiceSetSensorSignalInfo: eb:%d, mode:%d, start:0x%x, end:0x%x"
#define ISP_SERVICE_8912_112_2_18_1_43_53_253 "ISP_SERVICE: ISP_ServiceSetSensorSignalInfo %d line error"
#define ISP_SERVICE_8937_112_2_18_1_43_53_254 "ISP_SERVICE: The fun of get frame data is pnull"
#define ISP_SERVICE_9140_112_2_18_1_43_54_255 "ISP_SERVICE:ISP_ServiceGetOutputWidthAlign default %d line error!"
#define ISP_SERVICE_9526_112_2_18_1_43_55_256 "ISP_SERVICE: _ISP_CheckScaleParam"
#define ISP_SERVICE_9532_112_2_18_1_43_55_257 "ISP_SERVICE: slice_height_algin = %d"
#define ISP_SERVICE_9537_112_2_18_1_43_55_258 "ISP_SERVICE: scaling factor can not support"
#define ISP_SERVICE_9543_112_2_18_1_43_55_259 "ISP_SERVICE: scaling factor can not support"
#define ISP_SERVICE_9550_112_2_18_1_43_55_260 "ISP_SERVICE: slice height is not aligned, %d"
#define ISP_SERVICE_SC6530_483_112_2_18_1_43_56_261 "ISP_SERVICE:ISP_ServiceOpen, service = %d, state %d, cur_time %d, wait_opt %d"
#define ISP_SERVICE_SC6530_490_112_2_18_1_43_56_262 "ISP_SERVICE:ISP_ServiceOpen, time out %d"
#define ISP_SERVICE_SC6530_496_112_2_18_1_43_56_263 "ISP_SERVICE:ISP_ServiceOpen, Got controller,cur_timr %d"
#define ISP_SERVICE_SC6530_508_112_2_18_1_43_56_264 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_525_112_2_18_1_43_56_265 "ISP_SERVICE:ISP_ServiceClose, service = %d, status %d"
#define ISP_SERVICE_SC6530_587_112_2_18_1_43_56_266 "ISP_SERVICE:ISP_ServiceSetPreviewParam"
#define ISP_SERVICE_SC6530_597_112_2_18_1_43_56_267 "ISP_SERVICE:input.x ,y,w,h {%d,%d,%d,%d} lcd rect {%d,%d,%d,%d}"
#define ISP_SERVICE_SC6530_606_112_2_18_1_43_56_268 "ISP_SERVICE:lcd_id %d,block_id %d, disp_range.x,y,w,h {%d,%d,%d,%d} rot mode %d"
#define ISP_SERVICE_SC6530_610_112_2_18_1_43_56_269 "ISP_SERVICE:skip_frames %d, deci_frames %d"
#define ISP_SERVICE_SC6530_640_112_2_18_1_43_56_270 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_681_112_2_18_1_43_56_271 "ISP_SERVICE:ISP_ServiceStartPreview"
#define ISP_SERVICE_SC6530_712_112_2_18_1_43_56_272 "ISP_SERVICE:ISP_ServiceStopPreview"
#define ISP_SERVICE_SC6530_748_112_2_18_1_43_56_273 "ISP_SERVICE:ISP_ServiceStartReview,wrong state"
#define ISP_SERVICE_SC6530_752_112_2_18_1_43_56_274 "ISP_SERVICE:ISP_ServiceStartReview"
#define ISP_SERVICE_SC6530_798_112_2_18_1_43_57_275 "ISP_SERVICE:ISP_ServiceReviewNext address 0x%x, {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC6530_836_112_2_18_1_43_57_276 "ISP_SERVICE:ISP_ServiceSetReviewParam"
#define ISP_SERVICE_SC6530_847_112_2_18_1_43_57_277 "ISP_SERVICE:input format = %d, input.x,y,w,h {%d %d %d %d} lcd rect {%d,%d,%d,%d}"
#define ISP_SERVICE_SC6530_856_112_2_18_1_43_57_278 "ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} rot mode %d"
#define ISP_SERVICE_SC6530_888_112_2_18_1_43_57_279 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_909_112_2_18_1_43_57_280 "ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam"
#define ISP_SERVICE_SC6530_915_112_2_18_1_43_57_281 "ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_921_112_2_18_1_43_57_282 "ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d"
#define ISP_SERVICE_SC6530_955_112_2_18_1_43_57_283 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_969_112_2_18_1_43_57_284 "ISP_SERVICE:ISP_ServiceStartCaptureJpeg"
#define ISP_SERVICE_SC6530_986_112_2_18_1_43_57_285 "ISP_SERVICE:ISP_ServiceSetCompoundJpegCaptureParam"
#define ISP_SERVICE_SC6530_994_112_2_18_1_43_57_286 "ISP_SERVICE:input format = %d,yuv_type = %d,output.w,h {%d %d}"
#define ISP_SERVICE_SC6530_1023_112_2_18_1_43_57_287 "ISP_SERVICE:ISP_ServiceSetMpegCaptureParam"
#define ISP_SERVICE_SC6530_1031_112_2_18_1_43_57_288 "ISP_SERVICE: input.x,y,w,h {%d %d %d %d},skip_frames %d, deci_frames %d"
#define ISP_SERVICE_SC6530_1040_112_2_18_1_43_57_289 "ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}, lcd rect {%d,%d,%d,%d}"
#define ISP_SERVICE_SC6530_1050_112_2_18_1_43_57_290 "ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} disp rot %d enc rot %d"
#define ISP_SERVICE_SC6530_1081_112_2_18_1_43_57_291 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_1100_112_2_18_1_43_57_292 "ISP_ServiceGetMpegCaptureParam, Not in MPEG mode"
#define ISP_SERVICE_SC6530_1157_112_2_18_1_43_57_293 "ISP_SERVICE:ISP_ServiceStartCaptureMpeg"
#define ISP_SERVICE_SC6530_1203_112_2_18_1_43_57_294 "ISP_SERVICE:ISP_ServiceIsMpegOn, service = %d"
#define ISP_SERVICE_SC6530_1221_112_2_18_1_43_57_295 "ISP_SERVICE:ISP_ServiceStopCapture"
#define ISP_SERVICE_SC6530_1254_112_2_18_1_43_57_296 "ISP_SERVICE:ISP_ServiceStopReview"
#define ISP_SERVICE_SC6530_1289_112_2_18_1_43_57_297 "ISP_SERVICE:ISP_ServiceSetScaleParam"
#define ISP_SERVICE_SC6530_1296_112_2_18_1_43_57_298 "ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_1301_112_2_18_1_43_57_299 "ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define ISP_SERVICE_SC6530_1306_112_2_18_1_43_57_300 "ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
#define ISP_SERVICE_SC6530_1339_112_2_18_1_43_57_301 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_1382_112_2_18_1_43_58_302 "ISP_SERVICE:ISP_ServiceSetScaleParam"
#define ISP_SERVICE_SC6530_1387_112_2_18_1_43_58_303 "ISP_SERVICE:input_format = %d,input.w,h {%d %d }"
#define ISP_SERVICE_SC6530_1392_112_2_18_1_43_58_304 "ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define ISP_SERVICE_SC6530_1397_112_2_18_1_43_58_305 "ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
#define ISP_SERVICE_SC6530_1429_112_2_18_1_43_58_306 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_1443_112_2_18_1_43_58_307 "ISP_SERVICE:_ISP_ServiceStartDenoise, %d"
#define ISP_SERVICE_SC6530_1519_112_2_18_1_43_58_308 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_1541_112_2_18_1_43_58_309 "ISP_SERVICE:ISP_ServiceStartDenoise,wrong state"
#define ISP_SERVICE_SC6530_1545_112_2_18_1_43_58_310 "ISP_SERVICE:ISP_ServiceStartDenoise"
#define ISP_SERVICE_SC6530_1577_112_2_18_1_43_58_311 "ISP_SERVICE: ISP_ServiceFreeFrameAddress, frame 0x%x"
#define ISP_SERVICE_SC6530_1603_112_2_18_1_43_58_312 "ISP_SERVICE:ISP_ServiceStartVT"
#define ISP_SERVICE_SC6530_1635_112_2_18_1_43_58_313 "ISP_SERVICE:ISP_ServiceStartVT, service = %d"
#define ISP_SERVICE_SC6530_1645_112_2_18_1_43_58_314 "ISP_SERVICE:_ISP_ServiceStartVTExtScale"
#define ISP_SERVICE_SC6530_1665_112_2_18_1_43_58_315 "ISP_SERVICE:ISP_ServiceStartVTExt"
#define ISP_SERVICE_SC6530_1698_112_2_18_1_43_58_316 "ISP_SERVICE:ISP_ServiceStartVTExt, service = %d"
#define ISP_SERVICE_SC6530_1719_112_2_18_1_43_58_317 "ISP_SERVICE:ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x"
#define ISP_SERVICE_SC6530_1726_112_2_18_1_43_58_318 "ISP_SERVICE:wait for path1 done"
#define ISP_SERVICE_SC6530_1730_112_2_18_1_43_58_319 "ISP_SERVICE:ISP_ServiceSetVTDecodeAddr"
#define ISP_SERVICE_SC6530_1757_112_2_18_1_43_58_320 "ISP_SERVICE:ISP_ServiceSetVTExtScaleParam"
#define ISP_SERVICE_SC6530_1763_112_2_18_1_43_58_321 "ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_1767_112_2_18_1_43_58_322 "ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define ISP_SERVICE_SC6530_1771_112_2_18_1_43_58_323 "ISP_SERVICE:output_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define ISP_SERVICE_SC6530_1775_112_2_18_1_43_58_324 "ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
#define ISP_SERVICE_SC6530_1811_112_2_18_1_43_58_325 "ISP_SERVICE:ISP_ServiceSetVTDecodeParam"
#define ISP_SERVICE_SC6530_1817_112_2_18_1_43_58_326 "ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_1825_112_2_18_1_43_58_327 "ISP_SERVICE:remote_disp_lcd_id=%d, remote_disp_block_id=%d,remote_disp_range.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_1831_112_2_18_1_43_58_328 "ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,uaddr = %d, vaddr = %d"
#define ISP_SERVICE_SC6530_1864_112_2_18_1_43_58_329 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_1878_112_2_18_1_43_58_330 "ISP_SERVICE:ISP_ServiceSetVTEncodeParam"
#define ISP_SERVICE_SC6530_1884_112_2_18_1_43_58_331 "ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_1891_112_2_18_1_43_58_332 "ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d, deci_frames %d"
#define ISP_SERVICE_SC6530_1899_112_2_18_1_43_58_333 "ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_1930_112_2_18_1_43_59_334 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_1945_112_2_18_1_43_59_335 "ISP_SERVICE:ISP_ServiceSetVTExtEncodeParam"
#define ISP_SERVICE_SC6530_1951_112_2_18_1_43_59_336 "ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_1956_112_2_18_1_43_59_337 "ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}"
#define ISP_SERVICE_SC6530_1964_112_2_18_1_43_59_338 "ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_1994_112_2_18_1_43_59_339 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC6530_2013_112_2_18_1_43_59_340 "ISP_ServiceGetVTEncodeParam, Not in VT mode"
#define ISP_SERVICE_SC6530_2078_112_2_18_1_43_59_341 "ISP_SERVICE:ISP_ServiceStopVT"
#define ISP_SERVICE_SC6530_2114_112_2_18_1_43_59_342 "ISP_SERVICE:ISP_ServiceStopVT, service = %d"
#define ISP_SERVICE_SC6530_2127_112_2_18_1_43_59_343 "ISP_SERVICE:ISP_ServiceStopVTExt"
#define ISP_SERVICE_SC6530_2157_112_2_18_1_43_59_344 "ISP_SERVICE:ISP_ServiceStopVTExt, service = %d"
#define ISP_SERVICE_SC6530_2255_112_2_18_1_43_59_345 "ISP_Service: _ISP_ServiceMainMessageLoop,command=0x%x param=0x%x SignalCode 0x%x sig_ptr->ptr 0x%x time %d"
#define ISP_SERVICE_SC6530_2335_112_2_18_1_43_59_346 "_ISP_ServiceOpenSignalHandler, Failed to open ISP driver"
#define ISP_SERVICE_SC6530_2384_112_2_18_1_43_59_347 "ISP_SERVICE: display one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC6530_2771_112_2_18_1_44_0_348 "_ISP_ServiceSendRequest, ISP Service Queue full"
#define ISP_SERVICE_SC6530_2832_112_2_18_1_44_0_349 "ISP_SERVICE: queue info:enqueued:%d, store: %d, count: %d"
#define ISP_SERVICE_SC6530_2929_112_2_18_1_44_0_350 "ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
#define ISP_SERVICE_SC6530_2943_112_2_18_1_44_0_351 "ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
#define ISP_SERVICE_SC6530_2953_112_2_18_1_44_1_352 "ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
#define ISP_SERVICE_SC6530_2969_112_2_18_1_44_1_353 "ISP_SERVICE: _ISP_ServiceCloseBlock disp_blk enable %d remote disp blk enable %d"
#define ISP_SERVICE_SC6530_3008_112_2_18_1_44_1_354 "ISP_SERVICE: _ISP_ServiceVTTimeout, to restart VT"
#define ISP_SERVICE_SC6530_3031_112_2_18_1_44_1_355 "ISP_SERVICE: performance_level = %d, watchdog_feeded = %d"
#define ISP_SERVICE_SC6530_3052_112_2_18_1_44_1_356 "ISP_SERVICE: _ISP_ServiceTimerExpireHandle,unsupported timeout mode"
#define ISP_SERVICE_SC6530_3079_112_2_18_1_44_1_357 "ISP_SERVICE: _ISP_ServiceStartTimer,: mode %d, expire value %d, ret_val, %d"
#define ISP_SERVICE_SC6530_3110_112_2_18_1_44_1_358 "_ISP_ServiceWaitForDone, before %d"
#define ISP_SERVICE_SC6530_3114_112_2_18_1_44_1_359 "_ISP_ServiceWaitForDone, after %d"
#define ISP_SERVICE_SC6530_3123_112_2_18_1_44_1_360 "_ISP_ServiceExitWait, %d"
#define ISP_SERVICE_SC6530_3148_112_2_18_1_44_1_361 "_ISP_ServiceDeleteTimer"
#define ISP_SERVICE_SC6530_3168_112_2_18_1_44_1_362 "ISP_SERVICE : _ISP_VTCreateTimer, ret_val %d"
#define ISP_SERVICE_SC6530_3185_112_2_18_1_44_1_363 "ISP_SERVICE: _ISP_ServiceCloseTimer"
#define ISP_SERVICE_SC6530_3236_112_2_18_1_44_1_364 "_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d"
#define ISP_SERVICE_SC6530_3285_112_2_18_1_44_1_365 "_ISP_ServiceConvertCoor,dst x y w h {%d %d %d %d}"
#define ISP_SERVICE_SC6530_3316_112_2_18_1_44_1_366 "ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}"
#define ISP_SERVICE_SC6530_3371_112_2_18_1_44_1_367 "ISP_SERVICE: block id %d blk_cfg.start.x,y end_x,y {%d,%d,%d,%d}, resolution %d"
#define ISP_SERVICE_SC6530_3405_112_2_18_1_44_1_368 "_ISP_ServiceDisplayOneFrame: frame addr 0x%x 0x%x"
#define ISP_SERVICE_SC6530_3469_112_2_18_1_44_1_369 "_ISP_ServiceDisplayOneFrame: Block %d has been enabled"
#define ISP_SERVICE_SC6530_3486_112_2_18_1_44_2_370 "ISP_SERVICE: block enable %d,p_block->lcd_id %d, p_block->block_id %d, re config %d "
#define ISP_SERVICE_SC6530_3540_112_2_18_1_44_2_371 "_ISP_ServiceDisplayOneFrame: s->is_first_frame = %d, time %d"
#define ISP_SERVICE_SC6530_3612_112_2_18_1_44_2_372 "ISP_SERVICE: _ISP_ServiceRotateImage-U, SUCCESS"
#define ISP_SERVICE_SC6530_3616_112_2_18_1_44_2_373 "ISP_SERVICE: _ISP_ServiceRotateImage-U, FAIL"
#define ISP_SERVICE_SC6530_3625_112_2_18_1_44_2_374 "ISP_SERVICE: _ISP_ServiceRotateImage-V, SUCCESS"
#define ISP_SERVICE_SC6530_3629_112_2_18_1_44_2_375 "ISP_SERVICE: _ISP_ServiceRotateImage-V, FAIL"
#define ISP_SERVICE_SC6530_3657_112_2_18_1_44_2_376 "ISP_SERVICE: _ISP_ServiceRotateImage, rot_mode=%d,format=%d,img_size.w,h %d %d,"
#define ISP_SERVICE_SC6530_3664_112_2_18_1_44_2_377 "src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
#define ISP_SERVICE_SC6530_3668_112_2_18_1_44_2_378 "ISP_SERVICE: _ISP_ServiceRotateImage, SUCCESS"
#define ISP_SERVICE_SC6530_3672_112_2_18_1_44_2_379 "ISP_SERVICE: _ISP_ServiceRotateImage, FAIL"
#define ISP_SERVICE_SC6530_3903_112_2_18_1_44_2_380 "ISP_SERVICE: _ISP_SetPreviewParam"
#define ISP_SERVICE_SC6530_3954_112_2_18_1_44_2_381 "ISP_SERVICE: _ISP_SetPreviewParam,input for scaling {%d,%d,%d,%d}, output from CAP {%d,%d}"
#define ISP_SERVICE_SC6530_3986_112_2_18_1_44_2_382 "ISP_SERVICE: _ISP_SetJpegParam"
#define ISP_SERVICE_SC6530_4051_112_2_18_1_44_3_383 "ISP_SERVICE: _ISP_SetMpegParam"
#define ISP_SERVICE_SC6530_4254_112_2_18_1_44_3_384 "ISP_SERVICE: _ISP_SetMpegParam, few memory, YUV format %d, enc buf %d, disp buf %d"
#define ISP_SERVICE_SC6530_4309_112_2_18_1_44_3_385 "ISP_SERVICE: _ISP_SetReviewParam"
#define ISP_SERVICE_SC6530_4369_112_2_18_1_44_3_386 "disp_block_ptr->width = %d,disp_block_ptr->height = %d"
#define ISP_SERVICE_SC6530_4411_112_2_18_1_44_3_387 "ISP_SERVICE: _ISP_SetScaleParam"
#define ISP_SERVICE_SC6530_4424_112_2_18_1_44_3_388 "ISP_SERVICE: _ISP_SetScaleParam, slice heght %d"
#define ISP_SERVICE_SC6530_4548_112_2_18_1_44_4_389 "ISP_SERVICE: _ISP_SetDenoiseParam"
#define ISP_SERVICE_SC6530_4595_112_2_18_1_44_4_390 "ISP_SERVICE: _ISP_ServiceSetVTScaleParam"
#define ISP_SERVICE_SC6530_4621_112_2_18_1_44_4_391 "Scale in VT mode, width of target image is too large"
#define ISP_SERVICE_SC6530_4648_112_2_18_1_44_4_392 "ISP_SERVICE: _ISP_ServiceEncodeFrameInit, frame 0x%x"
#define ISP_SERVICE_SC6530_4682_112_2_18_1_44_4_393 "ISP_SERVICE: _ISP_SetVTEncodeParam"
#define ISP_SERVICE_SC6530_4792_112_2_18_1_44_4_394 "_ISP_ServiceSetVTEncodeParam,display_enable = %d,review_enable = %d"
#define ISP_SERVICE_SC6530_4813_112_2_18_1_44_4_395 "ISP_SERVICE: _ISP_ServiceSetVTExtEncodeParam"
#define ISP_SERVICE_SC6530_4930_112_2_18_1_44_4_396 "_ISP_ServiceSetVTExtEncodeParam,display_enable = %d,review_enable = %d, skip num %d "
#define ISP_SERVICE_SC6530_4950_112_2_18_1_44_4_397 "ISP_SERVICE: _ISP_SetVTDecodeParam"
#define ISP_SERVICE_SC6530_4986_112_2_18_1_44_4_398 "_ISP_ServiceSetVTDecodeParam,display_enable = %d,review_enable = %d"
#define ISP_SERVICE_SC6530_5033_112_2_18_1_44_4_399 "ISP_SERVICE: _ISP_ServiceSetVTExtScaleParam"
#define ISP_SERVICE_SC6530_5066_112_2_18_1_44_5_400 "ISP_SERVICE: _ISP_CheckPreviewParam"
#define ISP_SERVICE_SC6530_5077_112_2_18_1_44_5_401 "ISP_SERVICE: All width must be 4 pixels aligned"
#define ISP_SERVICE_SC6530_5083_112_2_18_1_44_5_402 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC6530_5089_112_2_18_1_44_5_403 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC6530_5097_112_2_18_1_44_5_404 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC6530_5103_112_2_18_1_44_5_405 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC6530_5110_112_2_18_1_44_5_406 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_SC6530_5121_112_2_18_1_44_5_407 "ISP_SERVICE: _ISP_CheckJpegParam"
#define ISP_SERVICE_SC6530_5132_112_2_18_1_44_5_408 "ISP_SERVICE: All width must be 4 pixels align"
#define ISP_SERVICE_SC6530_5145_112_2_18_1_44_5_409 "ISP_SERVICE: Input range is out of input size, {%d %d} {%d %d %d %d}"
#define ISP_SERVICE_SC6530_5163_112_2_18_1_44_5_410 "ISP_SERVICE: _ISP_CheckMpegParam"
#define ISP_SERVICE_SC6530_5175_112_2_18_1_44_5_411 "ISP_SERVICE: All width must be 4 pixels align"
#define ISP_SERVICE_SC6530_5181_112_2_18_1_44_5_412 "ISP_SERVICE: Unsupported data format"
#define ISP_SERVICE_SC6530_5189_112_2_18_1_44_5_413 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC6530_5195_112_2_18_1_44_5_414 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC6530_5202_112_2_18_1_44_5_415 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC6530_5209_112_2_18_1_44_5_416 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC6530_5225_112_2_18_1_44_5_417 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_SC6530_5240_112_2_18_1_44_5_418 "ISP_SERVICE: _ISP_CheckReviewParam"
#define ISP_SERVICE_SC6530_5253_112_2_18_1_44_5_419 "ISP_SERVICE: input_format = %d"
#define ISP_SERVICE_SC6530_5261_112_2_18_1_44_5_420 "ISP_SERVICE: All width must be 4 pixels align"
#define ISP_SERVICE_SC6530_5273_112_2_18_1_44_5_421 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC6530_5282_112_2_18_1_44_5_422 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC6530_5289_112_2_18_1_44_5_423 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC6530_5296_112_2_18_1_44_5_424 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC6530_5301_112_2_18_1_44_5_425 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC6530_5307_112_2_18_1_44_5_426 "ISP_SERVICE: Width of display rect is out of threshold"
#define ISP_SERVICE_SC6530_5319_112_2_18_1_44_5_427 "ISP_SERVICE: _ISP_CheckScaleParam"
#define ISP_SERVICE_SC6530_5329_112_2_18_1_44_5_428 "ISP_SERVICE: slice mode sline height = 0"
#define ISP_SERVICE_SC6530_5342_112_2_18_1_44_5_429 "ISP_SERVICE: input_format = %d"
#define ISP_SERVICE_SC6530_5353_112_2_18_1_44_5_430 "ISP_SERVICE: output_format = %d"
#define ISP_SERVICE_SC6530_5361_112_2_18_1_44_5_431 "ISP_SERVICE: All width must be 4 pixel align"
#define ISP_SERVICE_SC6530_5367_112_2_18_1_44_5_432 "ISP_SERVICE: input start x must be 4 pixel align when scale"
#define ISP_SERVICE_SC6530_5374_112_2_18_1_44_5_433 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC6530_5387_112_2_18_1_44_5_434 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC6530_5396_112_2_18_1_44_5_435 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC6530_5405_112_2_18_1_44_5_436 "ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported"
#define ISP_SERVICE_SC6530_5418_112_2_18_1_44_5_437 "ISP_SERVICE: _ISP_CheckDenoiseParam"
#define ISP_SERVICE_SC6530_5427_112_2_18_1_44_5_438 "ISP_SERVICE: input_format = %d, not support denoise format"
#define ISP_SERVICE_SC6530_5433_112_2_18_1_44_5_439 "ISP_SERVICE: output_format = %d, not support denoise format"
#define ISP_SERVICE_SC6530_5440_112_2_18_1_44_5_440 "ISP_SERVICE: All width must be 4 pixel align"
#define ISP_SERVICE_SC6530_5447_112_2_18_1_44_5_441 "ISP_SERVICE: output size is not corresponding with input size"
#define ISP_SERVICE_SC6530_5456_112_2_18_1_44_5_442 "ISP_SERVICE: output rect now not supporting cropping"
#define ISP_SERVICE_SC6530_5462_112_2_18_1_44_5_443 "ISP_SERVICE: too large size for denoise function"
#define ISP_SERVICE_SC6530_5475_112_2_18_1_44_5_444 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC6530_5484_112_2_18_1_44_5_445 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC6530_5493_112_2_18_1_44_5_446 "ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported"
#define ISP_SERVICE_SC6530_5507_112_2_18_1_44_5_447 "ISP_SERVICE: _ISP_CheckVTEncodeParam"
#define ISP_SERVICE_SC6530_5519_112_2_18_1_44_5_448 "ISP_SERVICE: All width must be 4 pixels aligned"
#define ISP_SERVICE_SC6530_5530_112_2_18_1_44_5_449 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC6530_5536_112_2_18_1_44_5_450 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC6530_5544_112_2_18_1_44_5_451 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC6530_5550_112_2_18_1_44_5_452 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC6530_5556_112_2_18_1_44_5_453 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_SC6530_5567_112_2_18_1_44_5_454 "ISP_SERVICE: _ISP_ServiceCheckVTExtEncodeParam"
#define ISP_SERVICE_SC6530_5579_112_2_18_1_44_6_455 "ISP_SERVICE: All width must be 4 pixels aligned"
#define ISP_SERVICE_SC6530_5590_112_2_18_1_44_6_456 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC6530_5596_112_2_18_1_44_6_457 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC6530_5604_112_2_18_1_44_6_458 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC6530_5610_112_2_18_1_44_6_459 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC6530_5618_112_2_18_1_44_6_460 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_SC6530_5629_112_2_18_1_44_6_461 "ISP_SERVICE: _ISP_CheckVTDecodeParam"
#define ISP_SERVICE_SC6530_5640_112_2_18_1_44_6_462 "ISP_SERVICE: All width must be 4 pixel align"
#define ISP_SERVICE_SC6530_5646_112_2_18_1_44_6_463 "ISP_SERVICE: input start x must be 4 pixel align when review"
#define ISP_SERVICE_SC6530_5653_112_2_18_1_44_6_464 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC6530_5664_112_2_18_1_44_6_465 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC6530_5670_112_2_18_1_44_6_466 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC6530_5676_112_2_18_1_44_6_467 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC6530_5730_112_2_18_1_44_6_468 "ISP_SERVICE: To do image scaling"
#define ISP_SERVICE_SC6530_5734_112_2_18_1_44_6_469 "ISP_SERVICE: state error! state %d, service %d"
#define ISP_SERVICE_SC6530_5760_112_2_18_1_44_6_470 "ISP_SERVICE:transform the image frame, the flag of src and dst: 0x%x to 0x%x"
#define ISP_SERVICE_SC6530_5780_112_2_18_1_44_6_471 "ISP_SERVICE:_ISP_ServiceStartPreview"
#define ISP_SERVICE_SC6530_5867_112_2_18_1_44_6_472 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_5887_112_2_18_1_44_6_473 "ISP_SERVICE:_ISP_ServiceStartJpeg"
#define ISP_SERVICE_SC6530_5985_112_2_18_1_44_6_474 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_6177_112_2_18_1_44_7_475 "ISP_SERVICE: s->total_scale_out %d s->encoder_size.h %d"
#define ISP_SERVICE_SC6530_6304_112_2_18_1_44_7_476 "ISP_SERVICE:_ISP_ServiceInternScale, %d"
#define ISP_SERVICE_SC6530_6316_112_2_18_1_44_7_477 "_ISP_ServiceInternScale, src size {%d %d}, src trim {%d %d %d %d}, dst {%d %d}, src fmt %d, dst fmt %d"
#define ISP_SERVICE_SC6530_6322_112_2_18_1_44_7_478 "_ISP_ServiceInternScale, src addr {0x%x 0x%x}, dst addr {0x%x 0x%x}"
#define ISP_SERVICE_SC6530_6427_112_2_18_1_44_7_479 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_6449_112_2_18_1_44_7_480 "ISP_SERVICE:_ISP_ServiceCaptureOnce, time %d, output ftm %d, size {%d %d}"
#define ISP_SERVICE_SC6530_6548_112_2_18_1_44_7_481 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_6563_112_2_18_1_44_7_482 "ISP_SERVICE:_ISP_ServiceStartMpeg"
#define ISP_SERVICE_SC6530_6586_112_2_18_1_44_7_483 "ISP_SERVICE: _ISP_ServiceMpegHandler, p_frame->flags 0x%x "
#define ISP_SERVICE_SC6530_6590_112_2_18_1_44_7_484 "ISP_SERVICE: MPEG stopped"
#define ISP_SERVICE_SC6530_6599_112_2_18_1_44_7_485 "ISP_SERVICE: _ISP_ServiceMpegHandler, Got one capture frame, %d"
#define ISP_SERVICE_SC6530_6608_112_2_18_1_44_7_486 "ISP_SERVICE: _ISP_ServiceMpegHandler, encode frame not released"
#define ISP_SERVICE_SC6530_6647_112_2_18_1_44_8_487 "ISP_SERVICE: _ISP_ServiceMpegHandler, Got one encode frame, %d"
#define ISP_SERVICE_SC6530_6687_112_2_18_1_44_8_488 "ISP_SERVICE: _ISP_ServiceMpegHandler, encode frame 0x%x not released!"
#define ISP_SERVICE_SC6530_6719_112_2_18_1_44_8_489 "ISP_SERVICE: _ISP_ServiceMpegHandler, Got one review frame, %d"
#define ISP_SERVICE_SC6530_6760_112_2_18_1_44_8_490 "ISP_SERVICE:_ISP_ServiceStartReview, %d"
#define ISP_SERVICE_SC6530_6844_112_2_18_1_44_8_491 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_6863_112_2_18_1_44_8_492 "ISP_SERVICE:_ISP_ServiceContinueReviewing, %d"
#define ISP_SERVICE_SC6530_6944_112_2_18_1_44_8_493 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_6964_112_2_18_1_44_8_494 "ISP_SERVICE:_ISP_ServiceStartScale,input addr {0x%x 0x%x 0x%x}"
#define ISP_SERVICE_SC6530_7080_112_2_18_1_44_8_495 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_7104_112_2_18_1_44_8_496 "ISP_SERVICE:_ISP_ServiceContinueScaling, %d"
#define ISP_SERVICE_SC6530_7167_112_2_18_1_44_9_497 "ISP_SERVICE:_ISP_ServiceContinueScaling,input addr {0x%x, 0x%x}"
#define ISP_SERVICE_SC6530_7200_112_2_18_1_44_9_498 "ISP_SERVICE:_ISP_ServiceContinueScaling,encoder_addr {0x%x, 0x%x} ,total_slice_count %d"
#define ISP_SERVICE_SC6530_7219_112_2_18_1_44_9_499 "ISP_SERVICE:_ISP_ServiceContinueScaling,slice_height %d"
#define ISP_SERVICE_SC6530_7279_112_2_18_1_44_9_500 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC6530_7294_112_2_18_1_44_9_501 "ISP_SERVICE:_ISP_ServiceStartVTEncode, %d"
#define ISP_SERVICE_SC6530_7320_112_2_18_1_44_9_502 "ISP_SERVICE: _ISP_ServiceVTHandler, p_frame->type 0x%x , time %d"
#define ISP_SERVICE_SC6530_7324_112_2_18_1_44_9_503 "ISP_SERVICE: VP stopped"
#define ISP_SERVICE_SC6530_7333_112_2_18_1_44_9_504 "ISP_SERVICE: _ISP_ServiceVTHandler, Got one captured frame, %d"
#define ISP_SERVICE_SC6530_7397_112_2_18_1_44_9_505 "ISP_SERVICE: _ISP_ServiceVTHandler, Got one uplink frame, %d"
#define ISP_SERVICE_SC6530_7437_112_2_18_1_44_9_506 "ISP_SERVICE: _ISP_ServiceVTHandler, encode frame 0x%x not released!"
#define ISP_SERVICE_SC6530_7474_112_2_18_1_44_9_507 "ISP_SERVICE: _ISP_ServiceVTHandler, Got one local review frame, %d"
#define ISP_SERVICE_SC6530_7504_112_2_18_1_44_9_508 "ISP_SERVICE: _ISP_ServiceVTHandler, Got one remote review frame, %d"
#define ISP_SERVICE_SC6530_7505_112_2_18_1_44_9_509 "ISP_SERVICE: remote display %d"
#define ISP_SERVICE_SC6530_7530_112_2_18_1_44_9_510 "ISP_SERVICE:_ISP_ServiceStartVTExtEncode, %d"
#define ISP_SERVICE_SC6530_7552_112_2_18_1_44_9_511 "ISP_SERVICE: _ISP_ServiceVTExtHandler, p_frame->type 0x%x , time %d"
#define ISP_SERVICE_SC6530_7556_112_2_18_1_44_9_512 "ISP_SERVICE: VT extension mode stopped"
#define ISP_SERVICE_SC6530_7579_112_2_18_1_44_9_513 "ISP_SERVICE: _ISP_ServiceVTExtHandler,Frame unlocked"
#define ISP_SERVICE_SC6530_7595_112_2_18_1_44_9_514 "ISP_SERVICE: _ISP_ServiceVTExtHandler, Frame has not been unlocked"
#define ISP_SERVICE_SC6530_7787_112_2_18_1_44_10_515 "ISP_SERVICE:Cap fifo overflow, service = %d"
#define ISP_SERVICE_SC6530_7804_112_2_18_1_44_10_516 "ISP_SERVICE:Sensor line error, service = %d"
#define ISP_SERVICE_SC6530_7821_112_2_18_1_44_10_517 "ISP_SERVICE:Sensor Frame error, service = %d"
#define ISP_SERVICE_SC6530_7834_112_2_18_1_44_10_518 "ISP_SERVICE:Sensor Jpeg buffer error, service = %d"
#define ISP_SERVICE_SC6530_7882_112_2_18_1_44_10_519 "_ISP_ServiceGetDefaultMemSize, lcd {%d %d} mem_size 0x%x"
#define ISP_SERVICE_SC6530_7907_112_2_18_1_44_10_520 "_ISP_ServiceAllocFrame, Mem not given by caller, should alloc-ed here"
#define ISP_SERVICE_SC6530_7918_112_2_18_1_44_10_521 "_ISP_ServiceAllocFrame, NO memory, try again 0x%x"
#define ISP_SERVICE_SC6530_7924_112_2_18_1_44_10_522 "_ISP_ServiceAllocFrame, NO memory reserved for ISP, 0x%x"
#define ISP_SERVICE_SC6530_7946_112_2_18_1_44_10_523 "_ISP_ServiceAllocFrame, Mem given by caller, start 0x%x ,length 0x%x"
#define ISP_SERVICE_SC6530_8461_112_2_18_1_44_11_524 "_ISP_ServiceAllocFrame, line_buff , 0x%x swap_buff y,u, 0x%x,0x%x"
#define ISP_SERVICE_SC6530_8540_112_2_18_1_44_11_525 "_ISP_ServiceAllocFrame: the denoise target buffer already alloc by caller, y address %d"
#define ISP_SERVICE_SC6530_8763_112_2_18_1_44_12_526 "ISP_SERVICE:ISP_ServiceIsVTOn, service = %d"
#define ISP_SERVICE_SC6530_8975_112_2_18_1_44_12_527 "ISP_SERVICE: _ISP_CheckScaleParam"
#define ISP_SERVICE_SC6530_8984_112_2_18_1_44_12_528 "ISP_SERVICE: unsupported scaling down size"
#define ISP_SERVICE_SC6530_8990_112_2_18_1_44_12_529 "ISP_SERVICE: unsupported scaling down size"
#define ISP_SERVICE_SC6530_8997_112_2_18_1_44_12_530 "ISP_SERVICE: unsupported scaling up size"
#define ISP_SERVICE_SC6530_9004_112_2_18_1_44_12_531 "ISP_SERVICE: slice height is not aligned, %d"
#define ISP_SERVICE_SC6530_9011_112_2_18_1_44_12_532 "ISP_SERVICE: trim rect error"
#define ISP_SERVICE_SC6530_9017_112_2_18_1_44_12_533 "ISP_SERVICE: trim rect not aligned by 4 pixels"
#define ISP_SERVICE_SC6530_9023_112_2_18_1_44_12_534 "ISP_SERVICE: the width of output_size not aligned by 4 pixels"
#define ISP_SERVICE_SC6530_9029_112_2_18_1_44_12_535 "ISP_SERVICE: unsupported scaling mode"
#define ISP_SERVICE_SC6530_9035_112_2_18_1_44_12_536 "ISP_SERVICE: get_scale_data is NULL"
#define ISP_SERVICE_SC6530_9042_112_2_18_1_44_12_537 "ISP_SERVICE: the address of surce data is NULL"
#define ISP_SERVICE_SC6530_9060_112_2_18_1_44_12_538 "ISP_ServiceMemConfig, mem_ptr 0x%x, mem len 0x%x ,isp_status %d"
#define ISP_SERVICE_SC6530_9066_112_2_18_1_44_12_539 "ISP_ServiceMemConfig, mem_ptr 0x%x, mem len 0x%x"
#define ISP_SERVICE_SC6530_9151_112_2_18_1_44_12_540 "ISP_ServiceAsyncDisplay, 0x%x"
#define ISP_SERVICE_SC6530_9157_112_2_18_1_44_12_541 "ISP_ServiceAsyncDisplay, Wrong status or service %d %d"
#define ISP_SERVICE_SC6530_9166_112_2_18_1_44_12_542 "ISP_ServiceAsyncDisplay, frame_ptr 0x%x p_frame 0x%x"
#define ISP_SERVICE_SC8800G_475_112_2_18_1_44_14_543 "ISP_SERVICE:ISP_ServiceOpen, service = %d, state %d, cur_time %d, wait_opt %d"
#define ISP_SERVICE_SC8800G_482_112_2_18_1_44_14_544 "ISP_SERVICE:ISP_ServiceOpen, time out %d"
#define ISP_SERVICE_SC8800G_488_112_2_18_1_44_14_545 "ISP_SERVICE:ISP_ServiceOpen, Got controller,cur_timr %d"
#define ISP_SERVICE_SC8800G_508_112_2_18_1_44_14_546 "ISP_SERVICE:ISP_ServiceClose, service = %d, status %d"
#define ISP_SERVICE_SC8800G_566_112_2_18_1_44_14_547 "ISP_SERVICE:ISP_ServiceSetPreviewParam"
#define ISP_SERVICE_SC8800G_576_112_2_18_1_44_14_548 "ISP_SERVICE:input.x ,y,w,h {%d,%d,%d,%d} lcd rect {%d,%d,%d,%d}"
#define ISP_SERVICE_SC8800G_585_112_2_18_1_44_14_549 "ISP_SERVICE:lcd_id %d,block_id %d, disp_range.x,y,w,h {%d,%d,%d,%d} rot mode %d"
#define ISP_SERVICE_SC8800G_589_112_2_18_1_44_14_550 "ISP_SERVICE:skip_frames %d, deci_frames %d"
#define ISP_SERVICE_SC8800G_619_112_2_18_1_44_14_551 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC8800G_660_112_2_18_1_44_14_552 "ISP_SERVICE:ISP_ServiceStartPreview"
#define ISP_SERVICE_SC8800G_682_112_2_18_1_44_15_553 "ISP_SERVICE:ISP_ServiceStopPreview"
#define ISP_SERVICE_SC8800G_717_112_2_18_1_44_15_554 "ISP_SERVICE:ISP_ServiceStartReview,wrong state"
#define ISP_SERVICE_SC8800G_721_112_2_18_1_44_15_555 "ISP_SERVICE:ISP_ServiceStartReview"
#define ISP_SERVICE_SC8800G_773_112_2_18_1_44_15_556 "ISP_SERVICE:ISP_ServiceReviewNext address 0x%x, {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC8800G_811_112_2_18_1_44_15_557 "ISP_SERVICE:ISP_ServiceSetReviewParam"
#define ISP_SERVICE_SC8800G_822_112_2_18_1_44_15_558 "ISP_SERVICE:input format = %d, input.x,y,w,h {%d %d %d %d} lcd rect {%d,%d,%d,%d}"
#define ISP_SERVICE_SC8800G_831_112_2_18_1_44_15_559 "ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} rot mode %d"
#define ISP_SERVICE_SC8800G_863_112_2_18_1_44_15_560 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC8800G_886_112_2_18_1_44_15_561 "ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam"
#define ISP_SERVICE_SC8800G_892_112_2_18_1_44_15_562 "ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_898_112_2_18_1_44_15_563 "ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d"
#define ISP_SERVICE_SC8800G_932_112_2_18_1_44_15_564 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC8800G_944_112_2_18_1_44_15_565 "ISP_SERVICE:ISP_ServiceStartCaptureJpeg"
#define ISP_SERVICE_SC8800G_960_112_2_18_1_44_15_566 "ISP_SERVICE:ISP_ServiceSetCompoundJpegCaptureParam"
#define ISP_SERVICE_SC8800G_968_112_2_18_1_44_15_567 "ISP_SERVICE:input format = %d,yuv_type = %d,output.w,h {%d %d}"
#define ISP_SERVICE_SC8800G_997_112_2_18_1_44_15_568 "ISP_SERVICE:ISP_ServiceSetMpegCaptureParam"
#define ISP_SERVICE_SC8800G_1005_112_2_18_1_44_15_569 "ISP_SERVICE: input.x,y,w,h {%d %d %d %d},skip_frames %d, deci_frames %d"
#define ISP_SERVICE_SC8800G_1014_112_2_18_1_44_15_570 "ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}, lcd rect {%d,%d,%d,%d}"
#define ISP_SERVICE_SC8800G_1024_112_2_18_1_44_15_571 "ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} disp rot %d enc rot %d"
#define ISP_SERVICE_SC8800G_1055_112_2_18_1_44_15_572 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC8800G_1074_112_2_18_1_44_15_573 "ISP_ServiceGetMpegCaptureParam, Not in MPEG mode"
#define ISP_SERVICE_SC8800G_1136_112_2_18_1_44_15_574 "ISP_SERVICE:ISP_ServiceStartCaptureMpeg"
#define ISP_SERVICE_SC8800G_1168_112_2_18_1_44_16_575 "ISP_SERVICE:ISP_ServiceIsMpegOn, service = %d"
#define ISP_SERVICE_SC8800G_1186_112_2_18_1_44_16_576 "ISP_SERVICE:ISP_ServiceStopCapture"
#define ISP_SERVICE_SC8800G_1218_112_2_18_1_44_16_577 "ISP_SERVICE:ISP_ServiceStopReview"
#define ISP_SERVICE_SC8800G_1249_112_2_18_1_44_16_578 "ISP_SERVICE:ISP_ServiceSetScaleParam"
#define ISP_SERVICE_SC8800G_1256_112_2_18_1_44_16_579 "ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_1261_112_2_18_1_44_16_580 "ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define ISP_SERVICE_SC8800G_1266_112_2_18_1_44_16_581 "ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
#define ISP_SERVICE_SC8800G_1299_112_2_18_1_44_16_582 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC8800G_1336_112_2_18_1_44_16_583 "ISP_SERVICE: ISP_ServiceFreeFrameAddress, frame 0x%x"
#define ISP_SERVICE_SC8800G_1360_112_2_18_1_44_16_584 "ISP_SERVICE:ISP_ServiceStartVT"
#define ISP_SERVICE_SC8800G_1386_112_2_18_1_44_16_585 "ISP_SERVICE:ISP_ServiceStartVT, service = %d"
#define ISP_SERVICE_SC8800G_1396_112_2_18_1_44_16_586 "ISP_SERVICE:_ISP_ServiceStartVTExtScale"
#define ISP_SERVICE_SC8800G_1416_112_2_18_1_44_16_587 "ISP_SERVICE:ISP_ServiceStartVTExt"
#define ISP_SERVICE_SC8800G_1443_112_2_18_1_44_16_588 "ISP_SERVICE:ISP_ServiceStartVTExt, service = %d"
#define ISP_SERVICE_SC8800G_1464_112_2_18_1_44_16_589 "ISP_SERVICE:ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x"
#define ISP_SERVICE_SC8800G_1471_112_2_18_1_44_16_590 "ISP_SERVICE:wait for path1 done"
#define ISP_SERVICE_SC8800G_1475_112_2_18_1_44_16_591 "ISP_SERVICE:ISP_ServiceSetVTDecodeAddr"
#define ISP_SERVICE_SC8800G_1502_112_2_18_1_44_16_592 "ISP_SERVICE:ISP_ServiceSetVTExtScaleParam"
#define ISP_SERVICE_SC8800G_1508_112_2_18_1_44_16_593 "ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_1512_112_2_18_1_44_16_594 "ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define ISP_SERVICE_SC8800G_1516_112_2_18_1_44_16_595 "ISP_SERVICE:output_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
#define ISP_SERVICE_SC8800G_1520_112_2_18_1_44_16_596 "ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
#define ISP_SERVICE_SC8800G_1556_112_2_18_1_44_16_597 "ISP_SERVICE:ISP_ServiceSetVTDecodeParam"
#define ISP_SERVICE_SC8800G_1562_112_2_18_1_44_16_598 "ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_1570_112_2_18_1_44_16_599 "ISP_SERVICE:remote_disp_lcd_id=%d, remote_disp_block_id=%d,remote_disp_range.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_1576_112_2_18_1_44_16_600 "ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,uaddr = %d, vaddr = %d"
#define ISP_SERVICE_SC8800G_1609_112_2_18_1_44_16_601 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC8800G_1623_112_2_18_1_44_16_602 "ISP_SERVICE:ISP_ServiceSetVTEncodeParam"
#define ISP_SERVICE_SC8800G_1629_112_2_18_1_44_16_603 "ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_1636_112_2_18_1_44_16_604 "ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d, deci_frames %d"
#define ISP_SERVICE_SC8800G_1644_112_2_18_1_44_16_605 "ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_1675_112_2_18_1_44_16_606 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC8800G_1690_112_2_18_1_44_16_607 "ISP_SERVICE:ISP_ServiceSetVTExtEncodeParam"
#define ISP_SERVICE_SC8800G_1696_112_2_18_1_44_16_608 "ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_1701_112_2_18_1_44_16_609 "ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}"
#define ISP_SERVICE_SC8800G_1709_112_2_18_1_44_16_610 "ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_1739_112_2_18_1_44_17_611 "ISP_SERVICE: error code %d"
#define ISP_SERVICE_SC8800G_1758_112_2_18_1_44_17_612 "ISP_ServiceGetVTEncodeParam, Not in VT mode"
#define ISP_SERVICE_SC8800G_1828_112_2_18_1_44_17_613 "ISP_SERVICE:ISP_ServiceStopVT"
#define ISP_SERVICE_SC8800G_1865_112_2_18_1_44_17_614 "ISP_SERVICE:ISP_ServiceStopVT, service = %d"
#define ISP_SERVICE_SC8800G_1878_112_2_18_1_44_17_615 "ISP_SERVICE:ISP_ServiceStopVTExt"
#define ISP_SERVICE_SC8800G_1909_112_2_18_1_44_17_616 "ISP_SERVICE:ISP_ServiceStopVTExt, service = %d"
#define ISP_SERVICE_SC8800G_2007_112_2_18_1_44_17_617 "ISP_Service: _ISP_ServiceMainMessageLoop,command=0x%x param=0x%x SignalCode 0x%x sig_ptr->ptr 0x%x time %d"
#define ISP_SERVICE_SC8800G_2131_112_2_18_1_44_17_618 "ISP_SERVICE: display one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC8800G_2155_112_2_18_1_44_17_619 "ISP_SERVICE: vp get one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC8800G_2160_112_2_18_1_44_17_620 "ISP_SERVICE: VT Stopped, don't do local review"
#define ISP_SERVICE_SC8800G_2171_112_2_18_1_44_17_621 "ISP_SERVICE:transform the local frame from yuv422 to yuv420"
#define ISP_SERVICE_SC8800G_2177_112_2_18_1_44_17_622 "ISP_SERVICE: timeout, couldn't convert yuv422 to yuv420"
#define ISP_SERVICE_SC8800G_2209_112_2_18_1_44_18_623 "ISP_SERVICE: vp get one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC8800G_2222_112_2_18_1_44_18_624 "ISP_SERVICE:transform the local frame from yuv422 to rgb565"
#define ISP_SERVICE_SC8800G_2235_112_2_18_1_44_18_625 "ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565"
#define ISP_SERVICE_SC8800G_2391_112_2_18_1_44_18_626 "ISP_SERVICE: Encode frame not released by user, drop this frame"
#define ISP_SERVICE_SC8800G_2411_112_2_18_1_44_18_627 "ISP_SERVICE: flag %d"
#define ISP_SERVICE_SC8800G_2453_112_2_18_1_44_18_628 "ISP_SERVICE: unsupported frame flags"
#define ISP_SERVICE_SC8800G_2460_112_2_18_1_44_18_629 "ISP_SERVICE: flag %d"
#define ISP_SERVICE_SC8800G_2468_112_2_18_1_44_18_630 "ISP_SERVICE:vp transform to yuv420 done,p_frame = 0x%x,y,u,v {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC8800G_2500_112_2_18_1_44_18_631 "ISP_SERVICE: encode frame switched to next one 0x%x"
#define ISP_SERVICE_SC8800G_2528_112_2_18_1_44_18_632 "ISP_SERVICE: remote display %d"
#define ISP_SERVICE_SC8800G_2540_112_2_18_1_44_18_633 "ISP_SERVICE: unsupported frame flags"
#define ISP_SERVICE_SC8800G_2703_112_2_18_1_44_19_634 "ISP_SERVICE: s->total_scale_out %d s->encoder_size.h %d"
#define ISP_SERVICE_SC8800G_2718_112_2_18_1_44_19_635 "ISP_SERVICE: display one frame ,y,u,v {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC8800G_3005_112_2_18_1_44_19_636 "_ISP_ServiceSendRequest, ISP Service Queue full"
#define ISP_SERVICE_SC8800G_3077_112_2_18_1_44_19_637 "ISP_SERVICE: queue info:enqueued:%d, store: %d, count: %d"
#define ISP_SERVICE_SC8800G_3260_112_2_18_1_44_20_638 "_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d"
#define ISP_SERVICE_SC8800G_3309_112_2_18_1_44_20_639 "_ISP_ServiceConvertCoor,dst x y w h {%d %d %d %d}"
#define ISP_SERVICE_SC8800G_3355_112_2_18_1_44_20_640 "ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}"
#define ISP_SERVICE_SC8800G_3401_112_2_18_1_44_20_641 "ISP_SERVICE: block id %d blk_cfg.start.x,y end_x,y {%d,%d,%d,%d}, resolution %d"
#define ISP_SERVICE_SC8800G_3425_112_2_18_1_44_20_642 "ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
#define ISP_SERVICE_SC8800G_3433_112_2_18_1_44_20_643 "ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
#define ISP_SERVICE_SC8800G_3443_112_2_18_1_44_20_644 "ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
#define ISP_SERVICE_SC8800G_3463_112_2_18_1_44_20_645 "ISP_SERVICE: _ISP_ServiceCloseBlock disp_blk enable %d remote disp blk enable %d"
#define ISP_SERVICE_SC8800G_3606_112_2_18_1_44_20_646 "_ISP_ServiceDisplayOneFrame: Block %d has been enabled"
#define ISP_SERVICE_SC8800G_3612_112_2_18_1_44_20_647 "ISP_SERVICE: block enable %d,p_block->lcd_id %d, p_block->block_id %d "
#define ISP_SERVICE_SC8800G_3643_112_2_18_1_44_20_648 "_ISP_ServiceDisplayOneFrame: s->is_first_frame = %d"
#define ISP_SERVICE_SC8800G_3701_112_2_18_1_44_20_649 "ISP_SERVICE: _ISP_ServiceRotateImage, rot_mode=%d,format=%d,img_size.w,h %d %d,"
#define ISP_SERVICE_SC8800G_3708_112_2_18_1_44_20_650 "src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
#define ISP_SERVICE_SC8800G_3711_112_2_18_1_44_20_651 "ISP_SERVICE: _ISP_ServiceRotateImage, SUCCESS"
#define ISP_SERVICE_SC8800G_3713_112_2_18_1_44_20_652 "ISP_SERVICE: _ISP_ServiceRotateImage, FAIL"
#define ISP_SERVICE_SC8800G_3792_112_2_18_1_44_21_653 "restore ARMCLK"
#define ISP_SERVICE_SC8800G_3899_112_2_18_1_44_21_654 "ISP_SERVICE: _ISP_SetPreviewParam"
#define ISP_SERVICE_SC8800G_3960_112_2_18_1_44_21_655 "ISP_SERVICE: _ISP_SetPreviewParam,input for scaling {%d,%d,%d,%d}, output from CAP {%d,%d}"
#define ISP_SERVICE_SC8800G_3993_112_2_18_1_44_21_656 "ISP_SERVICE: _ISP_SetJpegParam"
#define ISP_SERVICE_SC8800G_4063_112_2_18_1_44_21_657 "ISP_SERVICE: _ISP_SetMpegParam"
#define ISP_SERVICE_SC8800G_4256_112_2_18_1_44_22_658 "ISP_SERVICE: _ISP_SetReviewParam"
#define ISP_SERVICE_SC8800G_4314_112_2_18_1_44_22_659 "disp_block_ptr->width = %d,disp_block_ptr->height = %d"
#define ISP_SERVICE_SC8800G_4352_112_2_18_1_44_22_660 "ISP_SERVICE: _ISP_SetScaleParam"
#define ISP_SERVICE_SC8800G_4365_112_2_18_1_44_22_661 "ISP_SERVICE: _ISP_SetScaleParam, slice heght %d"
#define ISP_SERVICE_SC8800G_4385_112_2_18_1_44_22_662 "_ISP_ServiceSetScaleParam, rgb_ex_height %d"
#define ISP_SERVICE_SC8800G_4551_112_2_18_1_44_22_663 "ISP_SERVICE: _ISP_ServiceSetVTScaleParam"
#define ISP_SERVICE_SC8800G_4577_112_2_18_1_44_22_664 "Scale in VT mode, width of target image is too large"
#define ISP_SERVICE_SC8800G_4608_112_2_18_1_44_22_665 "ISP_SERVICE: _ISP_ServiceEncodeFrameInit, frame 0x%x"
#define ISP_SERVICE_SC8800G_4632_112_2_18_1_44_22_666 "ISP_SERVICE: _ISP_SetVTEncodeParam"
#define ISP_SERVICE_SC8800G_4721_112_2_18_1_44_23_667 "_ISP_ServiceSetVTEncodeParam,display_enable = %d,review_enable = %d"
#define ISP_SERVICE_SC8800G_4742_112_2_18_1_44_23_668 "ISP_SERVICE: _ISP_ServiceSetVTExtEncodeParam"
#define ISP_SERVICE_SC8800G_4847_112_2_18_1_44_23_669 "_ISP_ServiceSetVTExtEncodeParam,display_enable = %d,review_enable = %d, skip num %d "
#define ISP_SERVICE_SC8800G_4867_112_2_18_1_44_23_670 "ISP_SERVICE: _ISP_SetVTDecodeParam"
#define ISP_SERVICE_SC8800G_4902_112_2_18_1_44_23_671 "_ISP_ServiceSetVTDecodeParam,display_enable = %d,review_enable = %d"
#define ISP_SERVICE_SC8800G_4949_112_2_18_1_44_23_672 "ISP_SERVICE: _ISP_ServiceSetVTExtScaleParam"
#define ISP_SERVICE_SC8800G_5010_112_2_18_1_44_23_673 "ISP_SERVICE: _ISP_CheckPreviewParam"
#define ISP_SERVICE_SC8800G_5021_112_2_18_1_44_23_674 "ISP_SERVICE: All width must be 4 pixels aligned"
#define ISP_SERVICE_SC8800G_5027_112_2_18_1_44_23_675 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800G_5033_112_2_18_1_44_23_676 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800G_5041_112_2_18_1_44_23_677 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800G_5047_112_2_18_1_44_23_678 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC8800G_5054_112_2_18_1_44_23_679 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_SC8800G_5065_112_2_18_1_44_23_680 "ISP_SERVICE: _ISP_CheckJpegParam"
#define ISP_SERVICE_SC8800G_5076_112_2_18_1_44_23_681 "ISP_SERVICE: All width must be 4 pixels align"
#define ISP_SERVICE_SC8800G_5083_112_2_18_1_44_23_682 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800G_5117_112_2_18_1_44_24_683 "ISP_SERVICE: _ISP_CheckMpegParam"
#define ISP_SERVICE_SC8800G_5129_112_2_18_1_44_24_684 "ISP_SERVICE: All width must be 4 pixels align"
#define ISP_SERVICE_SC8800G_5135_112_2_18_1_44_24_685 "ISP_SERVICE: Unsupported data format"
#define ISP_SERVICE_SC8800G_5143_112_2_18_1_44_24_686 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800G_5149_112_2_18_1_44_24_687 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800G_5156_112_2_18_1_44_24_688 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800G_5163_112_2_18_1_44_24_689 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC8800G_5179_112_2_18_1_44_24_690 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_SC8800G_5194_112_2_18_1_44_24_691 "ISP_SERVICE: _ISP_CheckReviewParam"
#define ISP_SERVICE_SC8800G_5207_112_2_18_1_44_24_692 "ISP_SERVICE: input_format = %d"
#define ISP_SERVICE_SC8800G_5215_112_2_18_1_44_24_693 "ISP_SERVICE: All width must be 4 pixels align"
#define ISP_SERVICE_SC8800G_5221_112_2_18_1_44_24_694 "ISP_SERVICE: input start x must be 4 pixels align when review"
#define ISP_SERVICE_SC8800G_5234_112_2_18_1_44_24_695 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC8800G_5243_112_2_18_1_44_24_696 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC8800G_5250_112_2_18_1_44_24_697 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800G_5255_112_2_18_1_44_24_698 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800G_5262_112_2_18_1_44_24_699 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800G_5267_112_2_18_1_44_24_700 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC8800G_5273_112_2_18_1_44_24_701 "ISP_SERVICE: Width of display rect is out of threshold"
#define ISP_SERVICE_SC8800G_5285_112_2_18_1_44_24_702 "ISP_SERVICE: _ISP_CheckScaleParam"
#define ISP_SERVICE_SC8800G_5295_112_2_18_1_44_24_703 "ISP_SERVICE: slice mode sline height = 0"
#define ISP_SERVICE_SC8800G_5308_112_2_18_1_44_24_704 "ISP_SERVICE: input_format = %d"
#define ISP_SERVICE_SC8800G_5317_112_2_18_1_44_24_705 "ISP_SERVICE: output_format = %d"
#define ISP_SERVICE_SC8800G_5325_112_2_18_1_44_24_706 "ISP_SERVICE: All width must be 4 pixel align"
#define ISP_SERVICE_SC8800G_5331_112_2_18_1_44_24_707 "ISP_SERVICE: input start x must be 4 pixel align when scale"
#define ISP_SERVICE_SC8800G_5338_112_2_18_1_44_24_708 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800G_5351_112_2_18_1_44_24_709 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC8800G_5360_112_2_18_1_44_24_710 "ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
#define ISP_SERVICE_SC8800G_5369_112_2_18_1_44_24_711 "ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported"
#define ISP_SERVICE_SC8800G_5385_112_2_18_1_44_24_712 "ISP_SERVICE: _ISP_CheckVTEncodeParam"
#define ISP_SERVICE_SC8800G_5397_112_2_18_1_44_24_713 "ISP_SERVICE: All width must be 4 pixels aligned"
#define ISP_SERVICE_SC8800G_5408_112_2_18_1_44_24_714 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800G_5414_112_2_18_1_44_24_715 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800G_5422_112_2_18_1_44_24_716 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800G_5428_112_2_18_1_44_24_717 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC8800G_5434_112_2_18_1_44_24_718 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_SC8800G_5445_112_2_18_1_44_24_719 "ISP_SERVICE: _ISP_ServiceCheckVTExtEncodeParam"
#define ISP_SERVICE_SC8800G_5457_112_2_18_1_44_24_720 "ISP_SERVICE: All width must be 4 pixels aligned"
#define ISP_SERVICE_SC8800G_5468_112_2_18_1_44_24_721 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800G_5474_112_2_18_1_44_24_722 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800G_5482_112_2_18_1_44_24_723 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800G_5488_112_2_18_1_44_24_724 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC8800G_5496_112_2_18_1_44_24_725 "ISP_SERVICE: Display size is larger than max lcd size"
#define ISP_SERVICE_SC8800G_5507_112_2_18_1_44_24_726 "ISP_SERVICE: _ISP_CheckVTDecodeParam"
#define ISP_SERVICE_SC8800G_5518_112_2_18_1_44_24_727 "ISP_SERVICE: All width must be 4 pixel align"
#define ISP_SERVICE_SC8800G_5524_112_2_18_1_44_24_728 "ISP_SERVICE: input start x must be 4 pixel align when review"
#define ISP_SERVICE_SC8800G_5531_112_2_18_1_44_24_729 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800G_5542_112_2_18_1_44_24_730 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800G_5548_112_2_18_1_44_24_731 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800G_5554_112_2_18_1_44_24_732 "ISP_SERVICE: Rotation angle error"
#define ISP_SERVICE_SC8800G_5658_112_2_18_1_44_25_733 "ISP_SERVICE:_ISP_ServiceStartVTScale, dithering enabled!"
#define ISP_SERVICE_SC8800G_5677_112_2_18_1_44_25_734 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_5696_112_2_18_1_44_25_735 "ISP_SERVICE:_ISP_ServiceStartVTReview ,review_from = %d, y,u,v = {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC8800G_5873_112_2_18_1_44_25_736 "ISP_SERVICE:_ISP_ServiceStartVTReview ,display_buffer 0x%x"
#define ISP_SERVICE_SC8800G_5913_112_2_18_1_44_25_737 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_5929_112_2_18_1_44_25_738 "ISP_SERVICE: To display remote frame,s->remote_review_enable = %d"
#define ISP_SERVICE_SC8800G_5935_112_2_18_1_44_25_739 "ISP_SERVICE:display the remote frame"
#define ISP_SERVICE_SC8800G_5943_112_2_18_1_44_25_740 "ISP_SERVICE: No mem for remote in buffer"
#define ISP_SERVICE_SC8800G_5951_112_2_18_1_44_25_741 "ISP_SERVICE: timeout, don't display the remote frame"
#define ISP_SERVICE_SC8800G_5969_112_2_18_1_44_25_742 "ISP_SERVICE: To do image scaling"
#define ISP_SERVICE_SC8800G_5973_112_2_18_1_44_25_743 "ISP_SERVICE: state error!"
#define ISP_SERVICE_SC8800G_5992_112_2_18_1_44_25_744 "ISP_SERVICE: NO memory for input!"
#define ISP_SERVICE_SC8800G_6020_112_2_18_1_44_25_745 "ISP_SERVICE:transform the image frame from %d to %d"
#define ISP_SERVICE_SC8800G_6036_112_2_18_1_44_25_746 "ISP_SERVICE: NO memory for output!"
#define ISP_SERVICE_SC8800G_6049_112_2_18_1_44_25_747 "ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565"
#define ISP_SERVICE_SC8800G_6063_112_2_18_1_44_25_748 "ISP_SERVICE:transform the local frame from yuv422 to rgb565"
#define ISP_SERVICE_SC8800G_6068_112_2_18_1_44_25_749 "ISP_SERVICE:p_frame = 0x%x,y,u,v {0x%x,0x%x,0x%x}"
#define ISP_SERVICE_SC8800G_6074_112_2_18_1_44_25_750 "ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565"
#define ISP_SERVICE_SC8800G_6088_112_2_18_1_44_25_751 "ISP_SERVICE: _ISP_ServiceVTTimeout, to restart VT"
#define ISP_SERVICE_SC8800G_6111_112_2_18_1_44_25_752 "ISP_SERVICE: performance_level = %d, watchdog_feeded = %d"
#define ISP_SERVICE_SC8800G_6132_112_2_18_1_44_25_753 "ISP_SERVICE: _ISP_ServiceTimerExpireHandle,unsupported timeout mode"
#define ISP_SERVICE_SC8800G_6155_112_2_18_1_44_26_754 "ISP_SERVICE : _ISP_VTCreateTimer, ret_val %d"
#define ISP_SERVICE_SC8800G_6172_112_2_18_1_44_26_755 "ISP_SERVICE: _ISP_ServiceCloseTimer"
#define ISP_SERVICE_SC8800G_6187_112_2_18_1_44_26_756 "_DCAMERA_DeleteTimer"
#define ISP_SERVICE_SC8800G_6212_112_2_18_1_44_26_757 "ISP_SERVICE: _ISP_ServiceStartTimer,: mode %d, expire value %d, ret_val, %d"
#define ISP_SERVICE_SC8800G_6225_112_2_18_1_44_26_758 "ISP_SERVICE:_ISP_ServiceStartPreview"
#define ISP_SERVICE_SC8800G_6396_112_2_18_1_44_26_759 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_6415_112_2_18_1_44_26_760 "ISP_SERVICE:_ISP_ServiceStartJpeg"
#define ISP_SERVICE_SC8800G_6555_112_2_18_1_44_26_761 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_6579_112_2_18_1_44_26_762 "ISP_SERVICE:_ISP_ServiceStartMpeg"
#define ISP_SERVICE_SC8800G_6746_112_2_18_1_44_27_763 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_6764_112_2_18_1_44_27_764 "ISP_SERVICE:_ISP_ServiceStartReview"
#define ISP_SERVICE_SC8800G_6850_112_2_18_1_44_27_765 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_6903_112_2_18_1_44_27_766 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_6923_112_2_18_1_44_27_767 "ISP_SERVICE:_ISP_ServiceStartScale,input addr {0x%x 0x%x 0x%x}"
#define ISP_SERVICE_SC8800G_7033_112_2_18_1_44_27_768 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_7056_112_2_18_1_44_28_769 "ISP_SERVICE:_ISP_ServiceContinueScaling"
#define ISP_SERVICE_SC8800G_7097_112_2_18_1_44_28_770 "ISP_SERVICE:_ISP_ServiceContinueScaling,input addr {0x%x, 0x%x}"
#define ISP_SERVICE_SC8800G_7121_112_2_18_1_44_28_771 "ISP_SERVICE:_ISP_ServiceContinueScaling,encoder_addr {0x%x, 0x%x} ,total_slice_count %d"
#define ISP_SERVICE_SC8800G_7133_112_2_18_1_44_28_772 "ISP_SERVICE:_ISP_ServiceContinueScaling,slice_height %d"
#define ISP_SERVICE_SC8800G_7154_112_2_18_1_44_28_773 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_7170_112_2_18_1_44_28_774 "ISP_SERVICE:_ISP_ServiceStartVTEncode"
#define ISP_SERVICE_SC8800G_7295_112_2_18_1_44_28_775 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_7312_112_2_18_1_44_28_776 "ISP_SERVICE:_ISP_ServiceStartVTExtEncode"
#define ISP_SERVICE_SC8800G_7429_112_2_18_1_44_28_777 "ISP_SERVICE:driver error code 0x%x"
#define ISP_SERVICE_SC8800G_7536_112_2_18_1_44_28_778 "ISP_SERVICE:Cap fifo overflow, service = %d"
#define ISP_SERVICE_SC8800G_7549_112_2_18_1_44_29_779 "ISP_SERVICE:Sensor line error, service = %d"
#define ISP_SERVICE_SC8800G_7562_112_2_18_1_44_29_780 "ISP_SERVICE:Sensor Frame error, service = %d"
#define ISP_SERVICE_SC8800G_7575_112_2_18_1_44_29_781 "ISP_SERVICE:Sensor Jpeg buffer error, service = %d"
#define ISP_SERVICE_SC8800G_7654_112_2_18_1_44_29_782 "_ISP_ServiceAllocFrame, Mem not given by caller, should alloc-ed here"
#define ISP_SERVICE_SC8800G_7667_112_2_18_1_44_29_783 "_ISP_ServiceAllocFrame, NO memory reserved for ISP"
#define ISP_SERVICE_SC8800G_7678_112_2_18_1_44_29_784 "_ISP_ServiceAllocFrame, Memory not in one 64M Segement,try again"
#define ISP_SERVICE_SC8800G_7687_112_2_18_1_44_29_785 "_ISP_ServiceAllocFrame, %d block mem freed"
#define ISP_SERVICE_SC8800G_7692_112_2_18_1_44_29_786 "_ISP_ServiceAllocFrame, Memory successfully,total try times %d ,addr 0x%x"
#define ISP_SERVICE_SC8800G_7708_112_2_18_1_44_29_787 "_ISP_ServiceAllocFrame, Mem given by caller, start 0x%x ,length 0x%x"
#define ISP_SERVICE_SC8800G_7987_112_2_18_1_44_29_788 "_ISP_ServiceAllocFrame, line_buff , 0x%x swap_buff y,u, 0x%x,0x%x"
#define ISP_SERVICE_SC8800G_8286_112_2_18_1_44_30_789 "ISP_SERVICE:ISP_ServiceIsVTOn, service = %d"
#define ISP_SERVICE_SC8800G_8494_112_2_18_1_44_31_790 "ISP_SERVICE: _ISP_CheckScaleParam"
#define ISP_SERVICE_SC8800G_8503_112_2_18_1_44_31_791 "ISP_SERVICE: unsupported scaling down size"
#define ISP_SERVICE_SC8800G_8509_112_2_18_1_44_31_792 "ISP_SERVICE: unsupported scaling down size"
#define ISP_SERVICE_SC8800G_8516_112_2_18_1_44_31_793 "ISP_SERVICE: unsupported scaling up size"
#define ISP_SERVICE_SC8800G_8523_112_2_18_1_44_31_794 "ISP_SERVICE: slice height is not aligned, %d"
#define ISP_SERVICE_SC8800G_8530_112_2_18_1_44_31_795 "ISP_SERVICE: trim rect error"
#define ISP_SERVICE_SC8800G_8536_112_2_18_1_44_31_796 "ISP_SERVICE: trim rect not aligned by 4 pixels"
#define ISP_SERVICE_SC8800G_8542_112_2_18_1_44_31_797 "ISP_SERVICE: the width of output_size not aligned by 4 pixels"
#define ISP_SERVICE_SC8800G_8548_112_2_18_1_44_31_798 "ISP_SERVICE: unsupported scaling mode"
#define ISP_SERVICE_SC8800G_8554_112_2_18_1_44_31_799 "ISP_SERVICE: get_scale_data is NULL"
#define ISP_SERVICE_SC8800G_8561_112_2_18_1_44_31_800 "ISP_SERVICE: the address of surce data is NULL"
#define ISP_SERVICE_SC8800G_8588_112_2_18_1_44_31_801 "_ISP_ServiceRGB888FromBigToLittle, addr 0x%x, pixel_num %d"
#define ISP_SERVICE_SC8800G_8647_112_2_18_1_44_31_802 "ISP_ServiceMemConfig, mem_ptr 0x%x, mnem len 0x%x ,isp_status %d"
#define ISP_SERVICE_SC8800G_8653_112_2_18_1_44_31_803 "ISP_ServiceMemConfig, mem_ptr 0x%x, mnem len 0x%x"
#define ISP_SERVICE_SC8800G_8715_112_2_18_1_44_31_804 "_ISP_ServiceCheckMemAddr, param error, p_addr 0x%x, input_size 0x%x"
#define ISP_SERVICE_SC8800G_8726_112_2_18_1_44_31_805 "_ISP_ServiceCheckMemAddr, param error, in_fmt %d, input_size {%d %d}"
#define ISP_SERVICE_SC8800G_8731_112_2_18_1_44_31_806 "_ISP_ServiceCheckMemAddr, in, p_addr, {0x%x 0x%x 0x%x}"
#define ISP_SERVICE_SC8800G_8744_112_2_18_1_44_31_807 "_ISP_ServiceCheckMemAddr, Need adjust the memory address, img_fmt %d"
#define ISP_SERVICE_SC8800G_8762_112_2_18_1_44_31_808 "_ISP_ServiceCheckMemAddr, No memory, work_mem_len 0x%x"
#define ISP_SERVICE_SC8800G_8813_112_2_18_1_44_31_809 "_ISP_ServiceCheckMemAddr, Need adjust the memory address, img_fmt %d"
#define ISP_SERVICE_SC8800G_8828_112_2_18_1_44_31_810 "_ISP_ServiceCheckMemAddr, No memory, work_mem_len 0x%x"
#define ISP_SERVICE_SC8800G_8848_112_2_18_1_44_31_811 "_ISP_ServiceCheckMemAddr, unsupported image fotmat %d"
#define ISP_SERVICE_SC8800G_8856_112_2_18_1_44_31_812 "_ISP_ServiceCheckMemAddr, out, p_addr, {0x%x 0x%x 0x%x}"
#define ISP_SERVICE_SC8800G_8874_112_2_18_1_44_31_813 "_ISP_ServiceSyncMemToOutput ,parameter error"
#define ISP_SERVICE_SC8800G_8883_112_2_18_1_44_31_814 "_ISP_ServiceSyncMemToOutput ,format error %d"
#define ISP_SERVICE_SC8800G_8893_112_2_18_1_44_31_815 "_ISP_ServiceSyncMemToOutput, in, p_in_addr, {0x%x 0x%x}, p_out_addr {0x%x 0x%x},fmt %d"
#define ISP_SERVICE_SC8800G_8897_112_2_18_1_44_31_816 "_ISP_ServiceSyncMemToOutput, No need to Sync memory"
#define ISP_SERVICE_SC8800H_420_112_2_18_1_44_38_817 "ISP_Service: Low performance ,skip this frame"
#define ISP_SERVICE_SC8800H_428_112_2_18_1_44_38_818 "ISP_Service: middle performance ,skip this frame"
#define ISP_SERVICE_SC8800H_504_112_2_18_1_44_39_819 "_ISP_ServiceOnPortB,flags = %d,p->rgbaddr = 0x%x"
#define ISP_SERVICE_SC8800H_974_112_2_18_1_44_40_820 "ISP: lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = 0x%x"
#define ISP_SERVICE_SC8800H_985_112_2_18_1_44_40_821 "ISP: lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = 0x%x"
#define ISP_SERVICE_SC8800H_989_112_2_18_1_44_40_822 "_ISP_Backuplcdblockinfo same do nothing"
#define ISP_SERVICE_SC8800H_1002_112_2_18_1_44_40_823 "ISP_SERVICE: _ISP_Restorelcdblockinfo, lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = %d"
#define ISP_SERVICE_SC8800H_1019_112_2_18_1_44_40_824 "ISP_SERVICE: _ISP_CheckPreviewParam"
#define ISP_SERVICE_SC8800H_1025_112_2_18_1_44_40_825 "ISP_SERVICE: All width must be 2 pixel align"
#define ISP_SERVICE_SC8800H_1031_112_2_18_1_44_40_826 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800H_1037_112_2_18_1_44_40_827 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800H_1045_112_2_18_1_44_40_828 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800H_1064_112_2_18_1_44_40_829 "ISP_SERVICE: _ISP_CheckJpegParam"
#define ISP_SERVICE_SC8800H_1070_112_2_18_1_44_40_830 "ISP_SERVICE: All width must be 2 pixel align"
#define ISP_SERVICE_SC8800H_1077_112_2_18_1_44_40_831 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800H_1094_112_2_18_1_44_40_832 "ISP_SERVICE: _ISP_CheckMpegParam"
#define ISP_SERVICE_SC8800H_1100_112_2_18_1_44_40_833 "ISP_SERVICE: All width must be 2 pixel align"
#define ISP_SERVICE_SC8800H_1108_112_2_18_1_44_40_834 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800H_1114_112_2_18_1_44_40_835 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800H_1121_112_2_18_1_44_40_836 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800H_1144_112_2_18_1_44_40_837 "ISP_SERVICE: _ISP_CheckReviewParam"
#define ISP_SERVICE_SC8800H_1151_112_2_18_1_44_40_838 "ISP_SERVICE: input_format = %d"
#define ISP_SERVICE_SC8800H_1157_112_2_18_1_44_40_839 "ISP_SERVICE: All width must be 2 pixel align"
#define ISP_SERVICE_SC8800H_1163_112_2_18_1_44_40_840 "ISP_SERVICE: input start x must be 4 pixel align when review"
#define ISP_SERVICE_SC8800H_1169_112_2_18_1_44_40_841 "ISP_SERVICE: Invalid lcd id"
#define ISP_SERVICE_SC8800H_1175_112_2_18_1_44_40_842 "ISP_SERVICE: Invalid lcd id or block id"
#define ISP_SERVICE_SC8800H_1182_112_2_18_1_44_40_843 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800H_1199_112_2_18_1_44_40_844 "ISP_SERVICE: _ISP_CheckScaleParam"
#define ISP_SERVICE_SC8800H_1206_112_2_18_1_44_40_845 "ISP_SERVICE: input_format = %d"
#define ISP_SERVICE_SC8800H_1213_112_2_18_1_44_40_846 "ISP_SERVICE: input_format = %d"
#define ISP_SERVICE_SC8800H_1219_112_2_18_1_44_40_847 "ISP_SERVICE: All width must be 2 pixel align"
#define ISP_SERVICE_SC8800H_1225_112_2_18_1_44_40_848 "ISP_SERVICE: input start x must be 4 pixel align when scale"
#define ISP_SERVICE_SC8800H_1232_112_2_18_1_44_40_849 "ISP_SERVICE: Input range is out of input size"
#define ISP_SERVICE_SC8800H_1255_112_2_18_1_44_40_850 "_ISP_SetPreviewParam"
#define ISP_SERVICE_SC8800H_1301_112_2_18_1_44_40_851 "ISP: _ISP_StartPreview"
#define ISP_SERVICE_SC8800H_1418_112_2_18_1_44_40_852 "_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d"
#define ISP_SERVICE_SC8800H_1469_112_2_18_1_44_41_853 "_ISP_ServiceConvertCoor,dst x y w h {%d %d %d %d}"
#define ISP_SERVICE_SC8800H_1557_112_2_18_1_44_41_854 "ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}"
#define ISP_SERVICE_SC8800H_1619_112_2_18_1_44_41_855 "_ISP_SetJpegParam"
#define ISP_SERVICE_SC8800H_1654_112_2_18_1_44_41_856 "_ISP_StartJpeg"
#define ISP_SERVICE_SC8800H_1830_112_2_18_1_44_41_857 "_ISP_SetScaleParam"
#define ISP_SERVICE_SC8800H_1841_112_2_18_1_44_41_858 "ISP_SERVICE: output_format = %d"
#define ISP_SERVICE_SC8800H_1884_112_2_18_1_44_41_859 "_ISP_StartScale"
#define ISP_SERVICE_SC8800H_2157_112_2_18_1_44_42_860 "_ISP_StartSliceScale, slice_id=%d"
#define ISP_SERVICE_SC8800H_2417_112_2_18_1_44_42_861 "_ISP_SetReviewParam"
#define ISP_SERVICE_SC8800H_2462_112_2_18_1_44_43_862 "_ISP_StartReview, cur time: %d"
#define ISP_SERVICE_SC8800H_2606_112_2_18_1_44_43_863 "_ISP_SetMpegParam"
#define ISP_SERVICE_SC8800H_2664_112_2_18_1_44_43_864 "_ISP_StartMpeg"
#define ISP_SERVICE_SC8800H_2824_112_2_18_1_44_43_865 "_ISP_SetVTEncodeParam"
#define ISP_SERVICE_SC8800H_2874_112_2_18_1_44_44_866 "_ISP_SetVTEncodeParam,s->local_display_enable = %d,s->local_review_enable = %d,s->display_rotation %d"
#define ISP_SERVICE_SC8800H_2926_112_2_18_1_44_44_867 "_ISP_SetVTDecodeParam,s->remote_display_enable = %d,s->remote_review_enable = %d,s->remote_display_rotation %d"
#define ISP_SERVICE_SC8800H_2944_112_2_18_1_44_44_868 "ISP_SERVICE: _ISP_StartVTEncode"
#define ISP_SERVICE_SC8800H_2950_112_2_18_1_44_44_869 "ISP_SERVICE: _ISP_StartVTEncode,Sleeping"
#define ISP_SERVICE_SC8800H_2960_112_2_18_1_44_44_870 "ISP_SERVICE: _ISP_StartVTEncode,start VT capture"
#define ISP_SERVICE_SC8800H_3147_112_2_18_1_44_44_871 "_ISP_StartVTDisplay, f->yaddr = 0x%x, f->uaddr = 0x%x, f->vaddr  = 0x%x,f->rgbaddr = 0x%x"
#define ISP_SERVICE_SC8800H_3194_112_2_18_1_44_45_872 "_ISP_StartVTDisplay, f->yaddr = 0x%x, f->uaddr = 0x%x, f->vaddr  = 0x%x,f->rgbaddr = 0x%x"
#define ISP_SERVICE_SC8800H_3251_112_2_18_1_44_45_873 "App_queue:\n"
#define ISP_SERVICE_SC8800H_3263_112_2_18_1_44_45_874 "queue info:enqueued:%d, store: %d, count: %d"
#define ISP_SERVICE_SC8800H_3316_112_2_18_1_44_45_875 "ISP_ServiceFreeFrameAddress,yaddr=0x%x"
#define ISP_SERVICE_SC8800H_3332_112_2_18_1_44_45_876 "ISP_SERVICE:ISP_ServiceSetReviewParam"
#define ISP_SERVICE_SC8800H_3335_112_2_18_1_44_45_877 "ISP_SERVICE:input format = %d,input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_SC8800H_3338_112_2_18_1_44_45_878 "ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d"
#define ISP_SERVICE_SC8800H_3370_112_2_18_1_44_45_879 "ISP_SERVICE:ISP_ServiceStartPlayback"
#define ISP_SERVICE_SC8800H_3445_112_2_18_1_44_45_880 "ISP_SERVICE:ISP_ServiceSetPreviewParam"
#define ISP_SERVICE_SC8800H_3448_112_2_18_1_44_45_881 "ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_SC8800H_3451_112_2_18_1_44_45_882 "ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d"
#define ISP_SERVICE_SC8800H_3505_112_2_18_1_44_45_883 "ISP_SERVICE:ISP_ServiceStartPreview"
#define ISP_SERVICE_SC8800H_3532_112_2_18_1_44_45_884 "ISP_SERVICE:ISP_ServiceStopPreview"
#define ISP_SERVICE_SC8800H_3572_112_2_18_1_44_45_885 "ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam"
#define ISP_SERVICE_SC8800H_3575_112_2_18_1_44_45_886 "ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_SC8800H_3578_112_2_18_1_44_45_887 "ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d"
#define ISP_SERVICE_SC8800H_3599_112_2_18_1_44_45_888 "ISP_SERVICE:ISP_ServiceStartCaptureJpeg"
#define ISP_SERVICE_SC8800H_3613_112_2_18_1_44_45_889 "ISP_SERVICE:ISP_ServiceSetCompoundJpegCaptureParam"
#define ISP_SERVICE_SC8800H_3618_112_2_18_1_44_46_890 "ISP_SERVICE:input format = %d,yuv_shift = %d,output.w = %d,output.h = %d"
#define ISP_SERVICE_SC8800H_3674_112_2_18_1_44_46_891 "ISP_SERVICE:ISP_ServiceSetMpegCaptureParam"
#define ISP_SERVICE_SC8800H_3677_112_2_18_1_44_46_892 "ISP_SERVICE: input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_SC8800H_3680_112_2_18_1_44_46_893 "ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d"
#define ISP_SERVICE_SC8800H_3683_112_2_18_1_44_46_894 "ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d"
#define ISP_SERVICE_SC8800H_3715_112_2_18_1_44_46_895 "ISP_SERVICE:ISP_ServiceStartCaptureMpeg"
#define ISP_SERVICE_SC8800H_3742_112_2_18_1_44_46_896 "ISP_SERVICE:ISP_ServiceStopCapture"
#define ISP_SERVICE_SC8800H_3782_112_2_18_1_44_46_897 "ISP_SERVICE:ISP_ServiceSetScaleParam"
#define ISP_SERVICE_SC8800H_3785_112_2_18_1_44_46_898 "ISP_SERVICE:input_format = %d,input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_SC8800H_3788_112_2_18_1_44_46_899 "ISP_SERVICE:input_y_addr = 0x%x, input_u_addr = 0x%x, input_v_addr = 0x%x"
#define ISP_SERVICE_SC8800H_3791_112_2_18_1_44_46_900 "ISP_SERVICE:output_format = %d, yuv_shift = %d,output.w = %d,output.h = %d"
#define ISP_SERVICE_SC8800H_3811_112_2_18_1_44_46_901 "ISP_SERVICE:ISP_ServiceSetVTEncodeParam"
#define ISP_SERVICE_SC8800H_3814_112_2_18_1_44_46_902 "ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_SC8800H_3817_112_2_18_1_44_46_903 "ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d"
#define ISP_SERVICE_SC8800H_3821_112_2_18_1_44_46_904 "ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x=%d, local_disp_range.y=%d, local_disp_range.w=%d, local_disp_range.h=%d"
#define ISP_SERVICE_SC8800H_3881_112_2_18_1_44_46_905 "ISP_SERVICE:ISP_ServiceSetVTDecodeParam"
#define ISP_SERVICE_SC8800H_3884_112_2_18_1_44_46_906 "ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
#define ISP_SERVICE_SC8800H_3888_112_2_18_1_44_46_907 "ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x=%d, local_disp_range.y=%d, local_disp_range.w=%d, local_disp_range.h=%d"
#define ISP_SERVICE_SC8800H_3891_112_2_18_1_44_46_908 "ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,u_input_addr = %d, v_input_addr = %d"
#define ISP_SERVICE_SC8800H_3910_112_2_18_1_44_46_909 "ISP_SERVICE: _ISP_ServiceVTTimeout"
#define ISP_SERVICE_SC8800H_3937_112_2_18_1_44_46_910 "ISP_SERVICE: _ISP_ServiceTimerExpireHandle,s_isp_performance = %d, s_b_vt_watchdog_feeded = %d"
#define ISP_SERVICE_SC8800H_3952_112_2_18_1_44_46_911 "ISP_SERVICE: _ISP_ServiceTimerExpireHandle,not supported timeout mode"
#define ISP_SERVICE_SC8800H_3976_112_2_18_1_44_46_912 "ISP_SERVICE : _ISP_VTCreateTimer, ret_val %d"
#define ISP_SERVICE_SC8800H_3991_112_2_18_1_44_46_913 "ISP_SERVICE: _ISP_ServiceCloseTimer"
#define ISP_SERVICE_SC8800H_4003_112_2_18_1_44_46_914 "_DCAMERA_DeleteTimer"
#define ISP_SERVICE_SC8800H_4029_112_2_18_1_44_46_915 "ISP_SERVICE: _ISP_ServiceStartTimer,: mode %d, expire value %d, ret_val, %d"
#define ISP_SERVICE_SC8800H_4047_112_2_18_1_44_46_916 "ISP_SERVICE:ISP_ServiceStartVT"
#define ISP_SERVICE_SC8800H_4076_112_2_18_1_44_46_917 "ISP_SERVICE:ISP_ServiceStopVT"
#define ISP_SERVICE_SC8800H_4142_112_2_18_1_44_47_918 "ISP_SERVICE:ISP_ServiceStartScale"
#define ISP_SERVICE_SC8800H_4170_112_2_18_1_44_47_919 "ISP_SERVICE: wait for first porta frame"
#define ISP_SERVICE_SC8800H_4176_112_2_18_1_44_47_920 "ISP_Service: Low performance ,skip the remote frame"
#define ISP_SERVICE_SC8800H_4185_112_2_18_1_44_47_921 "ISP_Service: middle performance ,skip the remote frame"
#define ISP_SERVICE_SC8800H_4191_112_2_18_1_44_47_922 "ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x"
#define ISP_SERVICE_SC8800H_4453_112_2_18_1_44_47_923 "ISP: mpeg get two frame already"
#define ISP_SERVICE_SC8800H_4503_112_2_18_1_44_47_924 "ISP_SERVICE: To display local frame, s->local_review_enable = %d,s->is_first_frame = %d"
#define ISP_SERVICE_SC8800H_4524_112_2_18_1_44_47_925 "ISP_SERVICE: display the local frame"
#define ISP_SERVICE_SC8800H_4530_112_2_18_1_44_47_926 "ISP_SERVICE: timeout, don't display the local frame"
#define ISP_SERVICE_SC8800H_4623_112_2_18_1_44_47_927 "ISP_SERVICE: _ISP_ServiceRotateRGBImage, input for Rotation_AppStart: {rotPara.rotation_mode=%d,rotPara.data_format=%d,rotPara.img_size.w=%d,rotPara.img_size.h=%d,rotPara.src_addr.y_addr=%d,rotPara.src_addr.uv_addr=%d,rotPara.src_addr.v_addr=%d}"
#define ISP_SERVICE_SC8800H_4626_112_2_18_1_44_47_928 "ISP_SERVICE: _ISP_ServiceRotateRGBImage, SUCCESS"
#define ISP_SERVICE_SC8800H_4628_112_2_18_1_44_47_929 "ISP_SERVICE: _ISP_ServiceRotateRGBImage, FAIL"
#define ISP_SERVICE_SC8800H_4689_112_2_18_1_44_48_930 "_ISP_ServiceDisplayOneRGBFrame ,rot_angle %d,rot_tmp %d,lcd_rect.w %d h %d"
#define ISP_SERVICE_SC8800H_4710_112_2_18_1_44_48_931 "_ISP_ServiceDisplayOneRGBFrame ,lcd %d,block %d,addr 0x%x,p_disp_block->width %d,height %d"
#define ISP_SERVICE_SC8800H_4764_112_2_18_1_44_48_932 "ISP_Service, _ISP_ServicePortBPreview, 0x%x"
#define ISP_SERVICE_SC8800H_4783_112_2_18_1_44_48_933 "ISP: display first frame"
#define ISP_SERVICE_SC8800H_4799_112_2_18_1_44_48_934 "ISP_SERVICE_PORTB_SIG before ,param = %d ,rgbaddr = 0x%x"
#define ISP_SERVICE_SC8800H_4813_112_2_18_1_44_48_935 "ISP: display first frame"
#define ISP_SERVICE_SC8800H_4900_112_2_18_1_44_48_936 "ISP_Service: display %d frame, 0x%x"
#define ISP_SERVICE_SC8800H_5006_112_2_18_1_44_48_937 "ISP: ISP_ServiceRecieveRequest,command=0x%x param=0x%x,ptr = 0x%x"
#define ISP_SERVICE_SC8800H_5017_112_2_18_1_44_48_938 " ISP_ServiceOpen() !!"
#define ISP_SERVICE_SC8800H_5128_112_2_18_1_44_48_939 "ISP_SERVICE: To display remote frame,s->remote_review_enable = %d"
#define ISP_SERVICE_SC8800H_5141_112_2_18_1_44_48_940 "ISP_SERVICE: timeout, don't display the remote frame"
#define ISP_SERVICE_SC8800H_5320_112_2_18_1_44_49_941 "ISP_ServiceSendRequest: curr_avliable = %d"
#define ISP_SERVICE_SC8800H_5327_112_2_18_1_44_49_942 "ISP_ServiceSendRequest: queue full not send the signal "
#define ISP_SERVICE_SC8800H_5379_112_2_18_1_44_49_943 "ISP_Service, Not closed since last opened"
#define ISP_SERVICE_SC8800H_5383_112_2_18_1_44_49_944 "ISP_ServiceOpen"
#define ISP_SERVICE_SC8800H_5411_112_2_18_1_44_49_945 "s_isp_chng_freq_req_handler = %x"
#define ISP_SERVICE_SC8800H_5448_112_2_18_1_44_49_946 "ISP_ServiceClose, service = %d"
#define ISP_SERVICE_SC8800H_5487_112_2_18_1_44_49_947 "restore ARMCLK"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_ISP_SERVICE_TRC)
TRACE_MSG(FMARK_CTRL_134_112_2_18_1_43_34_0,"_UpdateLcd_SendSig: command=%d")
TRACE_MSG(FMARK_CTRL_210_112_2_18_1_43_34_1,"LCD_InvalidateRectWithFmark:%d,%d,%d,%d,%d,%d,%d")
TRACE_MSG(FMARK_CTRL_211_112_2_18_1_43_34_2,"handle LCD_InvalidateRectWithFmark time = %d")
TRACE_MSG(FMARK_CTRL_236_112_2_18_1_43_34_3,"_UpdateLcdCtrl_SearchNextBuf:pos=%d,%d")
TRACE_MSG(FMARK_CTRL_242_112_2_18_1_43_34_4,"s->save_display_data_info[temp][INFO_FLAG_INDEX] is %d")
TRACE_MSG(FMARK_CTRL_273_112_2_18_1_43_34_5,"_UpdateLcdCtrl_Done:next buf index = %d")
TRACE_MSG(FMARK_CTRL_285_112_2_18_1_43_34_6,"not saved data!")
TRACE_MSG(FMARK_CTRL_315_112_2_18_1_43_34_7,"_UpdateLcd_MessageLoop:command=0x%x param_num=0x%x")
TRACE_MSG(FMARK_CTRL_337_112_2_18_1_43_34_8,"---discard a frame data!--")
TRACE_MSG(FMARK_CTRL_380_112_2_18_1_43_34_9,"set s->is_use_fmark 1")
TRACE_MSG(FMARK_CTRL_395_112_2_18_1_43_34_10,"handle UPDATELCDCTRL_FMARK_SIG time = %d")
TRACE_MSG(FMARK_CTRL_430_112_2_18_1_43_34_11,"UPDATELCDCTRL_UPDATE_DONE_SIG:callback_param is 0x%x,display_pos=%d,%d,%d")
TRACE_MSG(FMARK_CTRL_437_112_2_18_1_43_34_12,"_UpdateLcd_MessageLoop command is error!")
TRACE_MSG(FMARK_CTRL_455_112_2_18_1_43_34_13,"FMARK Int: time = %d")
TRACE_MSG(FMARK_CTRL_472_112_2_18_1_43_34_14,"UpdateLcdCtrl_Init start!")
TRACE_MSG(FMARK_CTRL_511_112_2_18_1_43_34_15,"UpdateLcdCtrl_Init end!")
TRACE_MSG(ISP_SERVICE_275_112_2_18_1_43_35_16,"ISP_SERVICE:_ISP_UpdateCache default %d line error!")
TRACE_MSG(ISP_SERVICE_299_112_2_18_1_43_35_17,"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff ")
TRACE_MSG(ISP_SERVICE_310_112_2_18_1_43_35_18,"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, capture_h_scale_coeff_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_322_112_2_18_1_43_35_19,"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, capture_v_scale_coeff_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_334_112_2_18_1_43_35_20,"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, review_h_scale_coeff_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_346_112_2_18_1_43_35_21,"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, review_v_scale_coeff_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_358_112_2_18_1_43_35_22,"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, jpeg_h_scale_coeff_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_370_112_2_18_1_43_35_23,"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, jpeg_v_scale_coeff_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_391_112_2_18_1_43_35_24,"ISP_SERVICE:_ISP_AllocScaleCoeff ")
TRACE_MSG(ISP_SERVICE_401_112_2_18_1_43_35_25,"ISP_SERVICE:_ISP_AllocScaleCoeff, scale_vcoeff_tmp_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_412_112_2_18_1_43_35_26,"ISP_SERVICE:_ISP_AllocScaleCoeff, scale_hcoeff_tmp_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_424_112_2_18_1_43_35_27,"ISP_SERVICE:_ISP_AllocScaleCoeff, scale_tmp_ptr = 0x%x ")
TRACE_MSG(ISP_SERVICE_431_112_2_18_1_43_35_28,"ISP_SERVICE:_ISP_AllocScaleCoeff , h_ptr = 0x%x, v_ptr = 0x%x")
TRACE_MSG(ISP_SERVICE_455_112_2_18_1_43_35_29,"ISP_SERVICE:_ISP_FreeScaleCoeff ")
TRACE_MSG(ISP_SERVICE_563_112_2_18_1_43_36_30,"ISP_SERVICE:_ISP_GetImgSize default %d line error!")
TRACE_MSG(ISP_SERVICE_1043_112_2_18_1_43_37_31,"ISP_SERVICE:_ISP_InvalidateRect_Image default %d line error!")
TRACE_MSG(ISP_SERVICE_1125_112_2_18_1_43_37_32,"ISP_SERVICE:_ISP_InvalidateRect_Image default %d line error!")
TRACE_MSG(ISP_SERVICE_1180_112_2_18_1_43_37_33,"_ISP_DispImgCallback")
TRACE_MSG(ISP_SERVICE_1209_112_2_18_1_43_37_34,"ISP_SERVICE:_ISP_DispImgUseCallback rotation angel error at line %d!")
TRACE_MSG(ISP_SERVICE_1359_112_2_18_1_43_37_35,"NULL isp mutex, direct return")
TRACE_MSG(ISP_SERVICE_1420_112_2_18_1_43_37_36,"NULL isp peek signal mutex, direct return")
TRACE_MSG(ISP_SERVICE_1585_112_2_18_1_43_38_37,"ISP_SERVICE:_ISP_PeekSignal at %d")
TRACE_MSG(ISP_SERVICE_1604_112_2_18_1_43_38_38,"ISP_SERVICE:_ISP_PeekSignal:%d")
TRACE_MSG(ISP_SERVICE_1615_112_2_18_1_43_38_39,"ISP_SERVICE:_ISP_PeekSignal OK at %d")
TRACE_MSG(ISP_SERVICE_1769_112_2_18_1_43_38_40,"ISP_SERVICE:_ISP_ISR_CapEndOfFrameLow default %d line error!")
TRACE_MSG(ISP_SERVICE_1988_112_2_18_1_43_38_41,"ISP_SERVICE:_ISP_ISR_SensorLineErr\r\n")
TRACE_MSG(ISP_SERVICE_2003_112_2_18_1_43_39_42,"ISP_SERVICE:_ISP_ISR_SensorLineErrHigh")
TRACE_MSG(ISP_SERVICE_2024_112_2_18_1_43_39_43,"ISP_SERVICE:_ISP_ISR_PathFIFOOverflowed service: 0x%x ")
TRACE_MSG(ISP_SERVICE_2078_112_2_18_1_43_39_44,"ISP_SERVICE:_ISP_ISR_PathFIFOOverflowed service: 0x%x ")
TRACE_MSG(ISP_SERVICE_2095_112_2_18_1_43_39_45,"ISP_SERVICE:_ISP_ISR_SensorFrameErrLow")
TRACE_MSG(ISP_SERVICE_2109_112_2_18_1_43_39_46,"ISP_SERVICE:_ISP_ISR_SensorFrameErrHigh")
TRACE_MSG(ISP_SERVICE_2126_112_2_18_1_43_39_47,"ISP_SERVICE:_ISP_ISR_JpegBufferverflowedLow")
TRACE_MSG(ISP_SERVICE_2155_112_2_18_1_43_39_48,"ISP_SERVICE:_ISP_ISR_JpegBufferverflowedHigh default %d line error!")
TRACE_MSG(ISP_SERVICE_2159_112_2_18_1_43_39_49,"ISP_SERVICE:_ISP_ISR_JpegBufferverflowedHigh")
TRACE_MSG(ISP_SERVICE_2175_112_2_18_1_43_39_50,"ISP_SERVICE:_ISP_TimerOutHandler service: %d , %dms")
TRACE_MSG(ISP_SERVICE_2506_112_2_18_1_43_40_51,"[_ISP_CalculateScaleCoeff] shift_bits = %d, sub enable = %d, in size (%d, %d), out size = (%d, %d)")
TRACE_MSG(ISP_SERVICE_2543_112_2_18_1_43_40_52,"ISP_SERVICE: ISP Scale coeff error ! in (%d X %d), out (%d X %d)")
TRACE_MSG(ISP_SERVICE_2550_112_2_18_1_43_40_53,"ISP_SERVICE: ISP Scale coeff error ! in (%d X %d), out (%d X %d)")
TRACE_MSG(ISP_SERVICE_2557_112_2_18_1_43_40_54,"ISP_SERVICE: GenScaleCoeff memory error !")
TRACE_MSG(ISP_SERVICE_2565_112_2_18_1_43_40_55,"ISP_SERVICE: GenScaleCoeff failed !")
TRACE_MSG(ISP_SERVICE_2573_112_2_18_1_43_40_56,"ISP_SERVICE: reset hor coef to scaling up coef")
TRACE_MSG(ISP_SERVICE_2580_112_2_18_1_43_40_57,"ISP_SERVICE: reset ver coef to scaling up coef")
TRACE_MSG(ISP_SERVICE_2584_112_2_18_1_43_40_58,"ISP_SERVICE: GenScaleCoeff in size (%d X %d), out size (%d X %d)")
TRACE_MSG(ISP_SERVICE_2587_112_2_18_1_43_40_59,"ISP_SERVICE: GenScaleCoeff hor ptr = 0x%x, ver ptr = 0x%x, temp = 0x%x")
TRACE_MSG(ISP_SERVICE_2693_112_2_18_1_43_40_60,"ISP_SERVICE:_ISP_ImgTerminalLineCopy default %d line error!")
TRACE_MSG(ISP_SERVICE_2963_112_2_18_1_43_41_61,"ISP_SERVICE: _ISP_SetPreviewParam img format is %x  %d line error")
TRACE_MSG(ISP_SERVICE_3167_112_2_18_1_43_41_62,"ISP_SERVICE: _ISP_StopPreview")
TRACE_MSG(ISP_SERVICE_3395_112_2_18_1_43_41_63,"ISP_SERVICE:_ISP_InitializeBuf default %d line error!")
TRACE_MSG(ISP_SERVICE_3429_112_2_18_1_43_42_64,"ISP_SERVICE: _ISP_SetJpgBufLimitSize img format : %d, %d, %d")
TRACE_MSG(ISP_SERVICE_3470_112_2_18_1_43_42_65,"ISP_SERVICE: _ISP_SetCaptureParam img format is %x  %d line error")
TRACE_MSG(ISP_SERVICE_3485_112_2_18_1_43_42_66,"ISP_SERVICE: _ISP_SetCaptureParam, ow:%d")
TRACE_MSG(ISP_SERVICE_3486_112_2_18_1_43_42_67,"ISP_SERVICE: _ISP_SetCaptureParam, oh:%d")
TRACE_MSG(ISP_SERVICE_3487_112_2_18_1_43_42_68,"ISP_SERVICE: _ISP_SetCaptureParam, zw:%d")
TRACE_MSG(ISP_SERVICE_3488_112_2_18_1_43_42_69,"ISP_SERVICE: _ISP_SetCaptureParam, zh:%d")
TRACE_MSG(ISP_SERVICE_3502_112_2_18_1_43_42_70,"ISP_SERVICE: factor1: %d")
TRACE_MSG(ISP_SERVICE_3506_112_2_18_1_43_42_71,"ISP_SERVICE: _ISP_SetCaptureParam, cap trim 2")
TRACE_MSG(ISP_SERVICE_3526_112_2_18_1_43_42_72,"ISP_SERVICE: _ISP_SetCaptureParam, cap trim 0")
TRACE_MSG(ISP_SERVICE_3534_112_2_18_1_43_42_73,"ISP_SERVICE: _ISP_SetCaptureParam, cap trim 1")
TRACE_MSG(ISP_SERVICE_3543_112_2_18_1_43_42_74,"ISP_SERVICE: _ISP_SetCaptureParam, cap trim 2")
TRACE_MSG(ISP_SERVICE_4088_112_2_18_1_43_43_75,"ISP_SERVICE: _ISP_SetMjpegParam img format is %x  %d line error")
TRACE_MSG(ISP_SERVICE_4154_112_2_18_1_43_43_76,"ISP_SERVICE: _ISP_CalcScaleParam failed")
TRACE_MSG(ISP_SERVICE_4184_112_2_18_1_43_43_77,"ISP_SERVICE: _ISP_CalcScaleParam failed")
TRACE_MSG(ISP_SERVICE_4239_112_2_18_1_43_43_78,"ISP_SERVICE: _ISP_CalcScaleParam failed")
TRACE_MSG(ISP_SERVICE_4282_112_2_18_1_43_43_79,"ISP_SERVICE: _ISP_SetMjpegParam img format is %x  %d line error")
TRACE_MSG(ISP_SERVICE_4461_112_2_18_1_43_44_80,"ISP_SERVICE: _ISP_StopMjpg")
TRACE_MSG(ISP_SERVICE_4508_112_2_18_1_43_44_81,"ISP_SERVICE:Zoom array ptr=0x%x, input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_4637_112_2_18_1_43_44_82,"ISP_SERVICE: ISP_ServiceZoom call back 1")
TRACE_MSG(ISP_SERVICE_4661_112_2_18_1_43_44_83,"ISP_Service: _ISP_StartDvZoom")
TRACE_MSG(ISP_SERVICE_4673_112_2_18_1_43_44_84,"ISP_Service: _ISP_StartDvZoom, _ISP_CalcScaleParam failed")
TRACE_MSG(ISP_SERVICE_4699_112_2_18_1_43_44_85,"ISP_Service: _ISP_StartDvZoom call back")
TRACE_MSG(ISP_SERVICE_5039_112_2_18_1_43_45_86,"ISP_Service: _ISP_ServiceClose")
TRACE_MSG(ISP_SERVICE_5301_112_2_18_1_43_46_87,"ISP_SERVICE: ISP_ServiceSetSensorParam2, 0x%x")
TRACE_MSG(ISP_SERVICE_5316_112_2_18_1_43_46_88,"ISP_SERVICE: ISP_ServiceSetSensorParam2, 0x%x")
TRACE_MSG(ISP_SERVICE_5340_112_2_18_1_43_46_89,"ISP_SERVICE: _ISP_SetSensorEofCtrl, 0x%x")
TRACE_MSG(ISP_SERVICE_5377_112_2_18_1_43_46_90,"ISP_SERVICE: All width must be 4 pixel align rect x%d, inpur rect w%d, disp rect w%d")
TRACE_MSG(ISP_SERVICE_5383_112_2_18_1_43_46_91,"ISP_SERVICE: Invalid lcd id %d")
TRACE_MSG(ISP_SERVICE_5389_112_2_18_1_43_46_92,"ISP_SERVICE: Invalid zoom level %d")
TRACE_MSG(ISP_SERVICE_5398_112_2_18_1_43_46_93,"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d")
TRACE_MSG(ISP_SERVICE_5408_112_2_18_1_43_46_94,"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!")
TRACE_MSG(ISP_SERVICE_5414_112_2_18_1_43_46_95,"ISP_SERVICE: the lcd ration param %d is error!")
TRACE_MSG(ISP_SERVICE_5423_112_2_18_1_43_46_96,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_5432_112_2_18_1_43_46_97,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_5463_112_2_18_1_43_46_98,"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d")
TRACE_MSG(ISP_SERVICE_5472_112_2_18_1_43_46_99,"ISP_SERVICE: All width must be 4 pixel align, input rect w%d h%d, output w%d h%d")
TRACE_MSG(ISP_SERVICE_5490_112_2_18_1_43_46_100,"ISP_SERVICE:The capture size w:%d h:%d ,the capture buffer 0x%x is not enough")
TRACE_MSG(ISP_SERVICE_5500_112_2_18_1_43_46_101,"ISP_SERVICE: Jpg sensor output addr 0x%x is error")
TRACE_MSG(ISP_SERVICE_5529_112_2_18_1_43_46_102,"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d")
TRACE_MSG(ISP_SERVICE_5540_112_2_18_1_43_46_103,"ISP_SERVICE: Review Paramter not align Input rect: x:%d, w:%d output w:%d")
TRACE_MSG(ISP_SERVICE_5548_112_2_18_1_43_46_104,"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!")
TRACE_MSG(ISP_SERVICE_5557_112_2_18_1_43_46_105,"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!")
TRACE_MSG(ISP_SERVICE_5561_112_2_18_1_43_46_106,"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!")
TRACE_MSG(ISP_SERVICE_5573_112_2_18_1_43_46_107,"ISP_SERVICE: Input format %d is error")
TRACE_MSG(ISP_SERVICE_5581_112_2_18_1_43_46_108,"ISP_SERVICE: Input data y addr 0x%x uv addr 0x%x isn't aligned!")
TRACE_MSG(ISP_SERVICE_5591_112_2_18_1_43_46_109,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_5601_112_2_18_1_43_46_110,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_5615_112_2_18_1_43_46_111,"ISP_SERVICE:The src size w:%d h:%d ,The source review buffer 0x%x is not enough")
TRACE_MSG(ISP_SERVICE_5624_112_2_18_1_43_46_112,"ISP_SERVICE:The src size w:%d h:%d ,The source review y buffer 0x%x is not enough")
TRACE_MSG(ISP_SERVICE_5648_112_2_18_1_43_46_113,"ISP_SERVICE:The review data format %d error!")
TRACE_MSG(ISP_SERVICE_5654_112_2_18_1_43_46_114,"ISP_SERVICE:The src size w:%d h:%d ,The source review uv buffer 0x%x is not enough")
TRACE_MSG(ISP_SERVICE_5677_112_2_18_1_43_46_115,"ISP_SERVICE:The review disp size w:%d h:%d ,the review buffer 0x%x is not enough")
TRACE_MSG(ISP_SERVICE_5707_112_2_18_1_43_47_116,"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d")
TRACE_MSG(ISP_SERVICE_5715_112_2_18_1_43_47_117,"ISP_SERVICE:Input rect w%d, h%d capture w%d, h%d ISP don't support scale up when Mjpeg capture!")
TRACE_MSG(ISP_SERVICE_5721_112_2_18_1_43_47_118,"ISP_SERVICE: the lcd ration param %d is error!")
TRACE_MSG(ISP_SERVICE_5730_112_2_18_1_43_47_119,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_5739_112_2_18_1_43_47_120,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_5749_112_2_18_1_43_47_121,"ISP_SERVICE:The mjpg capture size w:%d h:%d ,the mjpg capture buffer 0x%x is not enough")
TRACE_MSG(ISP_SERVICE_5759_112_2_18_1_43_47_122,"ISP_SERVICE:The mjpg review disp size w:%d h:%d ,the review buffer 0x%x is not enough")
TRACE_MSG(ISP_SERVICE_5765_112_2_18_1_43_47_123,"ISP_SERVICE: The mjpg get capture data callback is PNULL")
TRACE_MSG(ISP_SERVICE_5792_112_2_18_1_43_47_124,"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d")
TRACE_MSG(ISP_SERVICE_5800_112_2_18_1_43_47_125,"ISP_SERVICE:Input rect w%d, h%d capture w%d, h%d ISP don't support scale up when Mjpeg capture!")
TRACE_MSG(ISP_SERVICE_5807_112_2_18_1_43_47_126,"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!")
TRACE_MSG(ISP_SERVICE_5816_112_2_18_1_43_47_127,"ISP_SERVICE:The mjpg capture size w:%d h:%d ,the mjpg capture buffer 0x%x is not enough")
TRACE_MSG(ISP_SERVICE_5822_112_2_18_1_43_47_128,"ISP_SERVICE: The mjpg get capture data callback is PNULL")
TRACE_MSG(ISP_SERVICE_5897_112_2_18_1_43_47_129,"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d")
TRACE_MSG(ISP_SERVICE_5903_112_2_18_1_43_47_130,"ISP_SERVICE: Scale Paramter not align Input rect: x%d, w%d ")
TRACE_MSG(ISP_SERVICE_5911_112_2_18_1_43_47_131,"ISP_SERVICE: Scale Paramter not align  Output: w%d")
TRACE_MSG(ISP_SERVICE_5919_112_2_18_1_43_47_132,"ISP_SERVICE: Scale Paramter not align  Output: w%d")
TRACE_MSG(ISP_SERVICE_5929_112_2_18_1_43_47_133,"ISP_SERVICE: Scale multiply error input_rect: w%d, h%d output: w%d, h%d")
TRACE_MSG(ISP_SERVICE_5935_112_2_18_1_43_47_134,"ISP_SERVICE: Scale mode error out_width: w%d, scale_mode:%d")
TRACE_MSG(ISP_SERVICE_5941_112_2_18_1_43_47_135,"ISP_SERVICE: Slice scale callback error: get_ptr:0x=%x")
TRACE_MSG(ISP_SERVICE_5948_112_2_18_1_43_47_136,"ISP_SERVICE:The input adr is null error")
TRACE_MSG(ISP_SERVICE_5966_112_2_18_1_43_47_137,"ISP_SERVICE:The scale data format %d error!")
TRACE_MSG(ISP_SERVICE_5977_112_2_18_1_43_47_138,"ISP_SERVICE: The mjpg get capture data callback is PNULL")
TRACE_MSG(ISP_SERVICE_6071_112_2_18_1_43_47_139,"ISP_SERVICE:_ISP_PathFifoOverFlowHandle default %d line error!")
TRACE_MSG(ISP_SERVICE_6129_112_2_18_1_43_47_140,"ISP_SERVICE:_ISP_ErrorHandle default %d line error!")
TRACE_MSG(ISP_SERVICE_6151_112_2_18_1_43_48_141,"ISP Service:preview fram rate time: %dms")
TRACE_MSG(ISP_SERVICE_6760_112_2_18_1_43_49_142,"ISP_SERVICE:_ISP_Service_GetIspPriority: %s, queue_name: %s, priority: %d ")
TRACE_MSG(ISP_SERVICE_6783_112_2_18_1_43_49_143,"ISP_SERVICE:_ISP_Service_Set_Priority, priority: %d")
TRACE_MSG(ISP_SERVICE_6820_112_2_18_1_43_49_144,"ISP_SERVICE:_ISP_Service_GetCallerPriority: %s, queue_name: %s, priority: %d ")
TRACE_MSG(ISP_SERVICE_6860_112_2_18_1_43_49_145,"ISP_SERVICE: _ISP_ServiceHandleRoutine the SignalCode 0x%x, error")
TRACE_MSG(ISP_SERVICE_6876_112_2_18_1_43_49_146,"ISP_SERVICE: Recieve Request, service=0x%x command=0x%x, cur time=%d")
TRACE_MSG(ISP_SERVICE_7072_112_2_18_1_43_49_147,"ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!")
TRACE_MSG(ISP_SERVICE_7152_112_2_18_1_43_50_148,"ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!")
TRACE_MSG(ISP_SERVICE_7210_112_2_18_1_43_50_149,"ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!")
TRACE_MSG(ISP_SERVICE_7256_112_2_18_1_43_50_150,"ISP_SERVICE: ISP_SERVICE_START_TIMEOUT_SIG, service=0x%x")
TRACE_MSG(ISP_SERVICE_7266_112_2_18_1_43_50_151,"ISP_SERVICE: ISP_SERVICE_STOP_TIMEOUT_SIG, service=0x%x")
TRACE_MSG(ISP_SERVICE_7272_112_2_18_1_43_50_152,"ISP_SERVICE: ISP_SERVICE_SENSOR_EOF_SIG, service=0x%x")
TRACE_MSG(ISP_SERVICE_7277_112_2_18_1_43_50_153,"ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!")
TRACE_MSG(ISP_SERVICE_7340_112_2_18_1_43_50_154,"[ISPLOCK: ISP_ServiceLock] owner = %d, cur owner = %d, sem_count = %d, wait option = %d")
TRACE_MSG(ISP_SERVICE_7347_112_2_18_1_43_50_155,"[ISPLOCK: ISP_ServiceLock] lock done, owner = %d")
TRACE_MSG(ISP_SERVICE_7351_112_2_18_1_43_50_156,"[ISPLOCK: ISP_ServiceLock] lock timeout, owner = %d")
TRACE_MSG(ISP_SERVICE_7378_112_2_18_1_43_50_157,"[ISPLOCK: ISP_ServiceUnlock] owner = %d, cur owner = %d, sem_count = %d")
TRACE_MSG(ISP_SERVICE_7385_112_2_18_1_43_50_158,"[ISPLOCK: ISP_ServiceUnlock] unlock done, owner = %d")
TRACE_MSG(ISP_SERVICE_7410_112_2_18_1_43_50_159,"[ISPLOCK: ISP_ServiceIsIdle] sem_count = %d, susp_count = %d")
TRACE_MSG(ISP_SERVICE_7522_112_2_18_1_43_50_160,"ISP_SERVICE:ISP_ServiceOpen")
TRACE_MSG(ISP_SERVICE_7527_112_2_18_1_43_50_161,"ISP_SERVICE: _ISP_AllocScaleCoeff failed !")
TRACE_MSG(ISP_SERVICE_7555_112_2_18_1_43_50_162,"ISP_SERVICE: ISP_ServiceClose, cur task: 0x%x")
TRACE_MSG(ISP_SERVICE_7579_112_2_18_1_43_50_163,"ISP_SERVICE: ISP_ServiceClose not msg")
TRACE_MSG(ISP_SERVICE_7640_112_2_18_1_43_51_164,"ISP_SERVICE: Register Function is PNULL")
TRACE_MSG(ISP_SERVICE_7662_112_2_18_1_43_51_165,"ISP_SERVICE:ISP_ServiceRegister default %d line error!")
TRACE_MSG(ISP_SERVICE_7681_112_2_18_1_43_51_166,"ISP_SERVICE:ISP_ServiceSetPreviewParam")
TRACE_MSG(ISP_SERVICE_7685_112_2_18_1_43_51_167,"ISP_SERVICE: Preview Param Ptr is PNULL %d line error")
TRACE_MSG(ISP_SERVICE_7692_112_2_18_1_43_51_168,"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_7695_112_2_18_1_43_51_169,"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d")
TRACE_MSG(ISP_SERVICE_7698_112_2_18_1_43_51_170,"ISP_SERVICE:target_rect.x = %d,target_rect.y = %d,target_rect.w = %d,target_rect.h = %d")
TRACE_MSG(ISP_SERVICE_7700_112_2_18_1_43_51_171,"ISP_SERVICE: param_ptr->init_zoom_level = %d")
TRACE_MSG(ISP_SERVICE_7702_112_2_18_1_43_51_172,"ISP_SERVICE: param_ptr->disp_rotation preview = %d")
TRACE_MSG(ISP_SERVICE_7731_112_2_18_1_43_51_173,"ISP_SERVICE:ISP_ServiceStartPreview")
TRACE_MSG(ISP_SERVICE_7737_112_2_18_1_43_51_174,"ISP_SERVICE: The state is not idle before Preview")
TRACE_MSG(ISP_SERVICE_7766_112_2_18_1_43_51_175,"ISP_ServiceStartPreview: preview start error")
TRACE_MSG(ISP_SERVICE_7820_112_2_18_1_43_51_176,"ISP_SERVICE: Capture Param Ptr is PNULL %d line error")
TRACE_MSG(ISP_SERVICE_7826_112_2_18_1_43_51_177,"ISP_SERVICE:ISP_ServiceSetCaptureParam")
TRACE_MSG(ISP_SERVICE_7829_112_2_18_1_43_51_178,"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_7831_112_2_18_1_43_51_179,"ISP_SERVICE:output.w = %d,output.h = %d")
TRACE_MSG(ISP_SERVICE_7833_112_2_18_1_43_51_180,"ISP_SERVICE: skip num: %d, in_data_format: %d, out_data_format: %d")
TRACE_MSG(ISP_SERVICE_7835_112_2_18_1_43_51_181,"ISP_SERVICE: cap_addr: 0x%x, size: 0x%x")
TRACE_MSG(ISP_SERVICE_7862_112_2_18_1_43_51_182,"ISP_SERVICE:ISP_ServiceStartSyncCapture, in time: %d ")
TRACE_MSG(ISP_SERVICE_7868_112_2_18_1_43_51_183,"ISP_SERVICE: The state is not idle before Sync Capture")
TRACE_MSG(ISP_SERVICE_7892_112_2_18_1_43_51_184,"ISP_SERVICE:ISP_ServiceStartSyncCapture time out")
TRACE_MSG(ISP_SERVICE_7909_112_2_18_1_43_51_185,"ISP_SERVICE:ISP_ServiceStartSyncCapture, out time: %d ")
TRACE_MSG(ISP_SERVICE_7934_112_2_18_1_43_51_186,"ISP_SERVICE:ISP_ServiceStartSyncCapture, in time: %d ")
TRACE_MSG(ISP_SERVICE_7940_112_2_18_1_43_51_187,"ISP_SERVICE: The state is not idle before Sync Capture")
TRACE_MSG(ISP_SERVICE_7966_112_2_18_1_43_51_188,"ISP_SERVICE:ISP_ServiceStartSyncCapture time out")
TRACE_MSG(ISP_SERVICE_7974_112_2_18_1_43_51_189,"ISP_SERVICE:ISP_ServiceStartSyncCapture, out time: %d ")
TRACE_MSG(ISP_SERVICE_7998_112_2_18_1_43_51_190,"ISP_SERVICE:ISP_ServiceStartAsyncCapture")
TRACE_MSG(ISP_SERVICE_8002_112_2_18_1_43_51_191,"ISP_SERVICE: The state is not idle before Async Capture")
TRACE_MSG(ISP_SERVICE_8050_112_2_18_1_43_51_192,"ISP_SERVICE:ISP_ServiceSetReviewParam")
TRACE_MSG(ISP_SERVICE_8054_112_2_18_1_43_51_193,"ISP_SERVICE: Review Param Ptr is PNULL %d line error")
TRACE_MSG(ISP_SERVICE_8060_112_2_18_1_43_51_194,"ISP_SERVICE:input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_8063_112_2_18_1_43_51_195,"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_8065_112_2_18_1_43_51_196,"ISP_SERVICE:display.x=%d,display.y=%d,display.w=%d,display.h=%d")
TRACE_MSG(ISP_SERVICE_8067_112_2_18_1_43_51_197,"ISP_SERVICE:target.x=%d,target.y=%d,target.w=%d,target.h=%d")
TRACE_MSG(ISP_SERVICE_8069_112_2_18_1_43_51_198,"ISP_SERVICE:scale line = %d")
TRACE_MSG(ISP_SERVICE_8071_112_2_18_1_43_51_199,"ISP_SERVICE: param_ptr->disp_rotation = %d")
TRACE_MSG(ISP_SERVICE_8072_112_2_18_1_43_51_200,"ISP_SERVICE: param_ptr->disp_format = %d")
TRACE_MSG(ISP_SERVICE_8074_112_2_18_1_43_51_201,"ISP_SERVICE: src y_addr = 0x%x, uv_addr=0x%x")
TRACE_MSG(ISP_SERVICE_8075_112_2_18_1_43_51_202,"ISP_SERVICE: rot y_addr = 0x%x, uv_addr=0x%x")
TRACE_MSG(ISP_SERVICE_8076_112_2_18_1_43_51_203,"ISP_SERVICE: param_ptr->disp_addr = 0x%x")
TRACE_MSG(ISP_SERVICE_8103_112_2_18_1_43_51_204,"ISP_SERVICE: ISP_ServiceStartReview: %dms")
TRACE_MSG(ISP_SERVICE_8109_112_2_18_1_43_51_205,"ISP_SERVICE: The staet is idle defore Review")
TRACE_MSG(ISP_SERVICE_8139_112_2_18_1_43_52_206,"ISP_SERVICE: ISP_ServiceStartReview end: %dms")
TRACE_MSG(ISP_SERVICE_8160_112_2_18_1_43_52_207,"ISP_SERVICE:ISP_ServiceSetScaleParam")
TRACE_MSG(ISP_SERVICE_8164_112_2_18_1_43_52_208,"ISP_SERVICE: Capture Param Ptr is PNULL %d line error")
TRACE_MSG(ISP_SERVICE_8171_112_2_18_1_43_52_209,"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_8173_112_2_18_1_43_52_210,"ISP_SERVICE:output.w = %d,output.h = %d")
TRACE_MSG(ISP_SERVICE_8174_112_2_18_1_43_52_211,"ISP_SERVICE: in y_addr = 0x%x, uv_addr=0x%x")
TRACE_MSG(ISP_SERVICE_8175_112_2_18_1_43_52_212,"ISP_SERVICE: out y_addr = 0x%x, uv_addr=0x%x")
TRACE_MSG(ISP_SERVICE_8176_112_2_18_1_43_52_213,"ISP_SERVICE: slice line: %d, is_f:%d,is_l:%d")
TRACE_MSG(ISP_SERVICE_8209_112_2_18_1_43_52_214,"ISP_SERVICE: ISP_ServiceScale")
TRACE_MSG(ISP_SERVICE_8215_112_2_18_1_43_52_215,"ISP_SERVICE: The staet is idle before Scale")
TRACE_MSG(ISP_SERVICE_8288_112_2_18_1_43_52_216,"ISP_SERVICE: Mjpg Param Ptr is PNULL %d line error")
TRACE_MSG(ISP_SERVICE_8294_112_2_18_1_43_52_217,"ISP_SERVICE: ISP_ServiceSetMjpegParam")
TRACE_MSG(ISP_SERVICE_8296_112_2_18_1_43_52_218,"ISP_SERVICE:input_size.w = %d,input_size.h = %d")
TRACE_MSG(ISP_SERVICE_8299_112_2_18_1_43_52_219,"ISP_SERVICE:input_rect.x = %d, input_rect.y = %d,input_rect.w = %d, input_rect.h = %d")
TRACE_MSG(ISP_SERVICE_8301_112_2_18_1_43_52_220,"ISP_SERVICE:capture_size.w = %d,capture_size.h = %d")
TRACE_MSG(ISP_SERVICE_8304_112_2_18_1_43_52_221,"ISP_SERVICE:jpeg_target_rect.x = %d, jpeg_target_rect.y = %d,jpeg_target_rect.w = %d, jpeg_target_rect.h = %d")
TRACE_MSG(ISP_SERVICE_8306_112_2_18_1_43_52_222,"ISP_SERVICE:jpeg_target_size.w = %d,jpeg_target_size.h = %d")
TRACE_MSG(ISP_SERVICE_8309_112_2_18_1_43_52_223,"ISP_SERVICE:disp_rect.x = %d, disp_rect.y = %d,disp_rect.w = %d, disp_rect.h = %d")
TRACE_MSG(ISP_SERVICE_8312_112_2_18_1_43_52_224,"ISP_SERVICE:target_src_rect.x = %d, target_src_rect.y = %d,target_src_rect.w = %d, target_src_rect.h = %d")
TRACE_MSG(ISP_SERVICE_8315_112_2_18_1_43_52_225,"ISP_SERVICE:target_rect.x = %d, target_rect.y = %d,target_rect.w = %d, target_rect.h = %d")
TRACE_MSG(ISP_SERVICE_8340_112_2_18_1_43_52_226,"ISP_SERVICE: ISP_ServiceStartMjpeg")
TRACE_MSG(ISP_SERVICE_8344_112_2_18_1_43_52_227,"ISP_SERVICE: The state is not idle before StartMjpeg")
TRACE_MSG(ISP_SERVICE_8391_112_2_18_1_43_52_228,"ISP_SERVICE: ISP_ServiceSetUpccParam")
TRACE_MSG(ISP_SERVICE_8395_112_2_18_1_43_52_229,"ISP_SERVICE: Upcc Param Ptr is PNULL %d line error")
TRACE_MSG(ISP_SERVICE_8402_112_2_18_1_43_52_230,"ISP_SERVICE:input_rect.x = %d, input_rect.y = %d,input_rect.w = %d, input_rect.h = %d")
TRACE_MSG(ISP_SERVICE_8404_112_2_18_1_43_52_231,"ISP_SERVICE:capture_size.w = %d,capture_size.h = %d")
TRACE_MSG(ISP_SERVICE_8429_112_2_18_1_43_52_232,"ISP_SERVICE: ISP_ServiceStartUPCC")
TRACE_MSG(ISP_SERVICE_8433_112_2_18_1_43_52_233,"ISP_SERVICE: The state is not idle before upcc")
TRACE_MSG(ISP_SERVICE_8475_112_2_18_1_43_52_234,"ISP_SERVICE: ISP_ServicePauseMjpeg")
TRACE_MSG(ISP_SERVICE_8497_112_2_18_1_43_52_235,"ISP_SERVICE: ISP_ServiceResumeMjpeg")
TRACE_MSG(ISP_SERVICE_8522_112_2_18_1_43_52_236,"ISP_SERVICE: get Sensor frame Ptr is PNULL %d line error")
TRACE_MSG(ISP_SERVICE_8546_112_2_18_1_43_52_237,"ISP_SERVICE: get Sensor frame alloc %d line error")
TRACE_MSG(ISP_SERVICE_8571_112_2_18_1_43_52_238,"ISP_SERVICE: ISP_ServiceFeatureStop : service:0x%x, task: 0x%x, %dms")
TRACE_MSG(ISP_SERVICE_8584_112_2_18_1_43_52_239,"ISP_SERVICE: ISP_ServiceFeatureStop nothing end: %dms ")
TRACE_MSG(ISP_SERVICE_8627_112_2_18_1_43_53_240,"ISP_SERVICE: ISP_ServiceFeatureStop Stop Capture")
TRACE_MSG(ISP_SERVICE_8636_112_2_18_1_43_53_241,"ISP_SERVICE: ISP_ServiceFeatureStop Stop Review")
TRACE_MSG(ISP_SERVICE_8690_112_2_18_1_43_53_242,"ISP_SERVICE:ISP_ServiceFeatureStop default %d line error!")
TRACE_MSG(ISP_SERVICE_8700_112_2_18_1_43_53_243,"ISP_SERVICE: ISP_ServiceFeatureStop TimeOut")
TRACE_MSG(ISP_SERVICE_8709_112_2_18_1_43_53_244,"ISP_SERVICE: ISP_ServiceFeatureStop end: %dms ")
TRACE_MSG(ISP_SERVICE_8753_112_2_18_1_43_53_245,"ISP_SERVICE: ISP_ServiceZoom, zoom_level : %d")
TRACE_MSG(ISP_SERVICE_8757_112_2_18_1_43_53_246,"ISP_SERVICE: The digital zoom callback is null")
TRACE_MSG(ISP_SERVICE_8770_112_2_18_1_43_53_247,"ISP_SERVICE: ISP Service States is : %d")
TRACE_MSG(ISP_SERVICE_8780_112_2_18_1_43_53_248,"ISP_SERVICE: ISP_ServiceZoom call back")
TRACE_MSG(ISP_SERVICE_8820_112_2_18_1_43_53_249,"ISP_SERVICE: Sensor Param Ptr is PNULL %d line error")
TRACE_MSG(ISP_SERVICE_8829_112_2_18_1_43_53_250,"ISP_SERVICE: ISP_ServiceSetSensorParam, 0x%x")
TRACE_MSG(ISP_SERVICE_8906_112_2_18_1_43_53_251,"ISP_SERVICE: ISP_ServiceSetSensorSignalInfo: p_skip:%d, img_pattern:%d, v_sync:%d, h_sync:%d")
TRACE_MSG(ISP_SERVICE_8907_112_2_18_1_43_53_252,"ISP_SERVICE: ISP_ServiceSetSensorSignalInfo: eb:%d, mode:%d, start:0x%x, end:0x%x")
TRACE_MSG(ISP_SERVICE_8912_112_2_18_1_43_53_253,"ISP_SERVICE: ISP_ServiceSetSensorSignalInfo %d line error")
TRACE_MSG(ISP_SERVICE_8937_112_2_18_1_43_53_254,"ISP_SERVICE: The fun of get frame data is pnull")
TRACE_MSG(ISP_SERVICE_9140_112_2_18_1_43_54_255,"ISP_SERVICE:ISP_ServiceGetOutputWidthAlign default %d line error!")
TRACE_MSG(ISP_SERVICE_9526_112_2_18_1_43_55_256,"ISP_SERVICE: _ISP_CheckScaleParam")
TRACE_MSG(ISP_SERVICE_9532_112_2_18_1_43_55_257,"ISP_SERVICE: slice_height_algin = %d")
TRACE_MSG(ISP_SERVICE_9537_112_2_18_1_43_55_258,"ISP_SERVICE: scaling factor can not support")
TRACE_MSG(ISP_SERVICE_9543_112_2_18_1_43_55_259,"ISP_SERVICE: scaling factor can not support")
TRACE_MSG(ISP_SERVICE_9550_112_2_18_1_43_55_260,"ISP_SERVICE: slice height is not aligned, %d")
TRACE_MSG(ISP_SERVICE_SC6530_483_112_2_18_1_43_56_261,"ISP_SERVICE:ISP_ServiceOpen, service = %d, state %d, cur_time %d, wait_opt %d")
TRACE_MSG(ISP_SERVICE_SC6530_490_112_2_18_1_43_56_262,"ISP_SERVICE:ISP_ServiceOpen, time out %d")
TRACE_MSG(ISP_SERVICE_SC6530_496_112_2_18_1_43_56_263,"ISP_SERVICE:ISP_ServiceOpen, Got controller,cur_timr %d")
TRACE_MSG(ISP_SERVICE_SC6530_508_112_2_18_1_43_56_264,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_525_112_2_18_1_43_56_265,"ISP_SERVICE:ISP_ServiceClose, service = %d, status %d")
TRACE_MSG(ISP_SERVICE_SC6530_587_112_2_18_1_43_56_266,"ISP_SERVICE:ISP_ServiceSetPreviewParam")
TRACE_MSG(ISP_SERVICE_SC6530_597_112_2_18_1_43_56_267,"ISP_SERVICE:input.x ,y,w,h {%d,%d,%d,%d} lcd rect {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC6530_606_112_2_18_1_43_56_268,"ISP_SERVICE:lcd_id %d,block_id %d, disp_range.x,y,w,h {%d,%d,%d,%d} rot mode %d")
TRACE_MSG(ISP_SERVICE_SC6530_610_112_2_18_1_43_56_269,"ISP_SERVICE:skip_frames %d, deci_frames %d")
TRACE_MSG(ISP_SERVICE_SC6530_640_112_2_18_1_43_56_270,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_681_112_2_18_1_43_56_271,"ISP_SERVICE:ISP_ServiceStartPreview")
TRACE_MSG(ISP_SERVICE_SC6530_712_112_2_18_1_43_56_272,"ISP_SERVICE:ISP_ServiceStopPreview")
TRACE_MSG(ISP_SERVICE_SC6530_748_112_2_18_1_43_56_273,"ISP_SERVICE:ISP_ServiceStartReview,wrong state")
TRACE_MSG(ISP_SERVICE_SC6530_752_112_2_18_1_43_56_274,"ISP_SERVICE:ISP_ServiceStartReview")
TRACE_MSG(ISP_SERVICE_SC6530_798_112_2_18_1_43_57_275,"ISP_SERVICE:ISP_ServiceReviewNext address 0x%x, {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_836_112_2_18_1_43_57_276,"ISP_SERVICE:ISP_ServiceSetReviewParam")
TRACE_MSG(ISP_SERVICE_SC6530_847_112_2_18_1_43_57_277,"ISP_SERVICE:input format = %d, input.x,y,w,h {%d %d %d %d} lcd rect {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC6530_856_112_2_18_1_43_57_278,"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} rot mode %d")
TRACE_MSG(ISP_SERVICE_SC6530_888_112_2_18_1_43_57_279,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_909_112_2_18_1_43_57_280,"ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam")
TRACE_MSG(ISP_SERVICE_SC6530_915_112_2_18_1_43_57_281,"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_921_112_2_18_1_43_57_282,"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d")
TRACE_MSG(ISP_SERVICE_SC6530_955_112_2_18_1_43_57_283,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_969_112_2_18_1_43_57_284,"ISP_SERVICE:ISP_ServiceStartCaptureJpeg")
TRACE_MSG(ISP_SERVICE_SC6530_986_112_2_18_1_43_57_285,"ISP_SERVICE:ISP_ServiceSetCompoundJpegCaptureParam")
TRACE_MSG(ISP_SERVICE_SC6530_994_112_2_18_1_43_57_286,"ISP_SERVICE:input format = %d,yuv_type = %d,output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1023_112_2_18_1_43_57_287,"ISP_SERVICE:ISP_ServiceSetMpegCaptureParam")
TRACE_MSG(ISP_SERVICE_SC6530_1031_112_2_18_1_43_57_288,"ISP_SERVICE: input.x,y,w,h {%d %d %d %d},skip_frames %d, deci_frames %d")
TRACE_MSG(ISP_SERVICE_SC6530_1040_112_2_18_1_43_57_289,"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}, lcd rect {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC6530_1050_112_2_18_1_43_57_290,"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} disp rot %d enc rot %d")
TRACE_MSG(ISP_SERVICE_SC6530_1081_112_2_18_1_43_57_291,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_1100_112_2_18_1_43_57_292,"ISP_ServiceGetMpegCaptureParam, Not in MPEG mode")
TRACE_MSG(ISP_SERVICE_SC6530_1157_112_2_18_1_43_57_293,"ISP_SERVICE:ISP_ServiceStartCaptureMpeg")
TRACE_MSG(ISP_SERVICE_SC6530_1203_112_2_18_1_43_57_294,"ISP_SERVICE:ISP_ServiceIsMpegOn, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_1221_112_2_18_1_43_57_295,"ISP_SERVICE:ISP_ServiceStopCapture")
TRACE_MSG(ISP_SERVICE_SC6530_1254_112_2_18_1_43_57_296,"ISP_SERVICE:ISP_ServiceStopReview")
TRACE_MSG(ISP_SERVICE_SC6530_1289_112_2_18_1_43_57_297,"ISP_SERVICE:ISP_ServiceSetScaleParam")
TRACE_MSG(ISP_SERVICE_SC6530_1296_112_2_18_1_43_57_298,"ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1301_112_2_18_1_43_57_299,"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_1306_112_2_18_1_43_57_300,"ISP_SERVICE:output_format = %d, output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1339_112_2_18_1_43_57_301,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_1382_112_2_18_1_43_58_302,"ISP_SERVICE:ISP_ServiceSetScaleParam")
TRACE_MSG(ISP_SERVICE_SC6530_1387_112_2_18_1_43_58_303,"ISP_SERVICE:input_format = %d,input.w,h {%d %d }")
TRACE_MSG(ISP_SERVICE_SC6530_1392_112_2_18_1_43_58_304,"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_1397_112_2_18_1_43_58_305,"ISP_SERVICE:output_format = %d, output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1429_112_2_18_1_43_58_306,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_1443_112_2_18_1_43_58_307,"ISP_SERVICE:_ISP_ServiceStartDenoise, %d")
TRACE_MSG(ISP_SERVICE_SC6530_1519_112_2_18_1_43_58_308,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_1541_112_2_18_1_43_58_309,"ISP_SERVICE:ISP_ServiceStartDenoise,wrong state")
TRACE_MSG(ISP_SERVICE_SC6530_1545_112_2_18_1_43_58_310,"ISP_SERVICE:ISP_ServiceStartDenoise")
TRACE_MSG(ISP_SERVICE_SC6530_1577_112_2_18_1_43_58_311,"ISP_SERVICE: ISP_ServiceFreeFrameAddress, frame 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_1603_112_2_18_1_43_58_312,"ISP_SERVICE:ISP_ServiceStartVT")
TRACE_MSG(ISP_SERVICE_SC6530_1635_112_2_18_1_43_58_313,"ISP_SERVICE:ISP_ServiceStartVT, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_1645_112_2_18_1_43_58_314,"ISP_SERVICE:_ISP_ServiceStartVTExtScale")
TRACE_MSG(ISP_SERVICE_SC6530_1665_112_2_18_1_43_58_315,"ISP_SERVICE:ISP_ServiceStartVTExt")
TRACE_MSG(ISP_SERVICE_SC6530_1698_112_2_18_1_43_58_316,"ISP_SERVICE:ISP_ServiceStartVTExt, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_1719_112_2_18_1_43_58_317,"ISP_SERVICE:ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_1726_112_2_18_1_43_58_318,"ISP_SERVICE:wait for path1 done")
TRACE_MSG(ISP_SERVICE_SC6530_1730_112_2_18_1_43_58_319,"ISP_SERVICE:ISP_ServiceSetVTDecodeAddr")
TRACE_MSG(ISP_SERVICE_SC6530_1757_112_2_18_1_43_58_320,"ISP_SERVICE:ISP_ServiceSetVTExtScaleParam")
TRACE_MSG(ISP_SERVICE_SC6530_1763_112_2_18_1_43_58_321,"ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1767_112_2_18_1_43_58_322,"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_1771_112_2_18_1_43_58_323,"ISP_SERVICE:output_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_1775_112_2_18_1_43_58_324,"ISP_SERVICE:output_format = %d, output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1811_112_2_18_1_43_58_325,"ISP_SERVICE:ISP_ServiceSetVTDecodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_1817_112_2_18_1_43_58_326,"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1825_112_2_18_1_43_58_327,"ISP_SERVICE:remote_disp_lcd_id=%d, remote_disp_block_id=%d,remote_disp_range.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1831_112_2_18_1_43_58_328,"ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,uaddr = %d, vaddr = %d")
TRACE_MSG(ISP_SERVICE_SC6530_1864_112_2_18_1_43_58_329,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_1878_112_2_18_1_43_58_330,"ISP_SERVICE:ISP_ServiceSetVTEncodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_1884_112_2_18_1_43_58_331,"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1891_112_2_18_1_43_58_332,"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d, deci_frames %d")
TRACE_MSG(ISP_SERVICE_SC6530_1899_112_2_18_1_43_58_333,"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1930_112_2_18_1_43_59_334,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_1945_112_2_18_1_43_59_335,"ISP_SERVICE:ISP_ServiceSetVTExtEncodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_1951_112_2_18_1_43_59_336,"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1956_112_2_18_1_43_59_337,"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1964_112_2_18_1_43_59_338,"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_1994_112_2_18_1_43_59_339,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC6530_2013_112_2_18_1_43_59_340,"ISP_ServiceGetVTEncodeParam, Not in VT mode")
TRACE_MSG(ISP_SERVICE_SC6530_2078_112_2_18_1_43_59_341,"ISP_SERVICE:ISP_ServiceStopVT")
TRACE_MSG(ISP_SERVICE_SC6530_2114_112_2_18_1_43_59_342,"ISP_SERVICE:ISP_ServiceStopVT, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_2127_112_2_18_1_43_59_343,"ISP_SERVICE:ISP_ServiceStopVTExt")
TRACE_MSG(ISP_SERVICE_SC6530_2157_112_2_18_1_43_59_344,"ISP_SERVICE:ISP_ServiceStopVTExt, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_2255_112_2_18_1_43_59_345,"ISP_Service: _ISP_ServiceMainMessageLoop,command=0x%x param=0x%x SignalCode 0x%x sig_ptr->ptr 0x%x time %d")
TRACE_MSG(ISP_SERVICE_SC6530_2335_112_2_18_1_43_59_346,"_ISP_ServiceOpenSignalHandler, Failed to open ISP driver")
TRACE_MSG(ISP_SERVICE_SC6530_2384_112_2_18_1_43_59_347,"ISP_SERVICE: display one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_2771_112_2_18_1_44_0_348,"_ISP_ServiceSendRequest, ISP Service Queue full")
TRACE_MSG(ISP_SERVICE_SC6530_2832_112_2_18_1_44_0_349,"ISP_SERVICE: queue info:enqueued:%d, store: %d, count: %d")
TRACE_MSG(ISP_SERVICE_SC6530_2929_112_2_18_1_44_0_350,"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_2943_112_2_18_1_44_0_351,"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_2953_112_2_18_1_44_1_352,"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_2969_112_2_18_1_44_1_353,"ISP_SERVICE: _ISP_ServiceCloseBlock disp_blk enable %d remote disp blk enable %d")
TRACE_MSG(ISP_SERVICE_SC6530_3008_112_2_18_1_44_1_354,"ISP_SERVICE: _ISP_ServiceVTTimeout, to restart VT")
TRACE_MSG(ISP_SERVICE_SC6530_3031_112_2_18_1_44_1_355,"ISP_SERVICE: performance_level = %d, watchdog_feeded = %d")
TRACE_MSG(ISP_SERVICE_SC6530_3052_112_2_18_1_44_1_356,"ISP_SERVICE: _ISP_ServiceTimerExpireHandle,unsupported timeout mode")
TRACE_MSG(ISP_SERVICE_SC6530_3079_112_2_18_1_44_1_357,"ISP_SERVICE: _ISP_ServiceStartTimer,: mode %d, expire value %d, ret_val, %d")
TRACE_MSG(ISP_SERVICE_SC6530_3110_112_2_18_1_44_1_358,"_ISP_ServiceWaitForDone, before %d")
TRACE_MSG(ISP_SERVICE_SC6530_3114_112_2_18_1_44_1_359,"_ISP_ServiceWaitForDone, after %d")
TRACE_MSG(ISP_SERVICE_SC6530_3123_112_2_18_1_44_1_360,"_ISP_ServiceExitWait, %d")
TRACE_MSG(ISP_SERVICE_SC6530_3148_112_2_18_1_44_1_361,"_ISP_ServiceDeleteTimer")
TRACE_MSG(ISP_SERVICE_SC6530_3168_112_2_18_1_44_1_362,"ISP_SERVICE : _ISP_VTCreateTimer, ret_val %d")
TRACE_MSG(ISP_SERVICE_SC6530_3185_112_2_18_1_44_1_363,"ISP_SERVICE: _ISP_ServiceCloseTimer")
TRACE_MSG(ISP_SERVICE_SC6530_3236_112_2_18_1_44_1_364,"_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d")
TRACE_MSG(ISP_SERVICE_SC6530_3285_112_2_18_1_44_1_365,"_ISP_ServiceConvertCoor,dst x y w h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_3316_112_2_18_1_44_1_366,"ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC6530_3371_112_2_18_1_44_1_367,"ISP_SERVICE: block id %d blk_cfg.start.x,y end_x,y {%d,%d,%d,%d}, resolution %d")
TRACE_MSG(ISP_SERVICE_SC6530_3405_112_2_18_1_44_1_368,"_ISP_ServiceDisplayOneFrame: frame addr 0x%x 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_3469_112_2_18_1_44_1_369,"_ISP_ServiceDisplayOneFrame: Block %d has been enabled")
TRACE_MSG(ISP_SERVICE_SC6530_3486_112_2_18_1_44_2_370,"ISP_SERVICE: block enable %d,p_block->lcd_id %d, p_block->block_id %d, re config %d ")
TRACE_MSG(ISP_SERVICE_SC6530_3540_112_2_18_1_44_2_371,"_ISP_ServiceDisplayOneFrame: s->is_first_frame = %d, time %d")
TRACE_MSG(ISP_SERVICE_SC6530_3612_112_2_18_1_44_2_372,"ISP_SERVICE: _ISP_ServiceRotateImage-U, SUCCESS")
TRACE_MSG(ISP_SERVICE_SC6530_3616_112_2_18_1_44_2_373,"ISP_SERVICE: _ISP_ServiceRotateImage-U, FAIL")
TRACE_MSG(ISP_SERVICE_SC6530_3625_112_2_18_1_44_2_374,"ISP_SERVICE: _ISP_ServiceRotateImage-V, SUCCESS")
TRACE_MSG(ISP_SERVICE_SC6530_3629_112_2_18_1_44_2_375,"ISP_SERVICE: _ISP_ServiceRotateImage-V, FAIL")
TRACE_MSG(ISP_SERVICE_SC6530_3657_112_2_18_1_44_2_376,"ISP_SERVICE: _ISP_ServiceRotateImage, rot_mode=%d,format=%d,img_size.w,h %d %d,")
TRACE_MSG(ISP_SERVICE_SC6530_3664_112_2_18_1_44_2_377,"src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_3668_112_2_18_1_44_2_378,"ISP_SERVICE: _ISP_ServiceRotateImage, SUCCESS")
TRACE_MSG(ISP_SERVICE_SC6530_3672_112_2_18_1_44_2_379,"ISP_SERVICE: _ISP_ServiceRotateImage, FAIL")
TRACE_MSG(ISP_SERVICE_SC6530_3903_112_2_18_1_44_2_380,"ISP_SERVICE: _ISP_SetPreviewParam")
TRACE_MSG(ISP_SERVICE_SC6530_3954_112_2_18_1_44_2_381,"ISP_SERVICE: _ISP_SetPreviewParam,input for scaling {%d,%d,%d,%d}, output from CAP {%d,%d}")
TRACE_MSG(ISP_SERVICE_SC6530_3986_112_2_18_1_44_2_382,"ISP_SERVICE: _ISP_SetJpegParam")
TRACE_MSG(ISP_SERVICE_SC6530_4051_112_2_18_1_44_3_383,"ISP_SERVICE: _ISP_SetMpegParam")
TRACE_MSG(ISP_SERVICE_SC6530_4254_112_2_18_1_44_3_384,"ISP_SERVICE: _ISP_SetMpegParam, few memory, YUV format %d, enc buf %d, disp buf %d")
TRACE_MSG(ISP_SERVICE_SC6530_4309_112_2_18_1_44_3_385,"ISP_SERVICE: _ISP_SetReviewParam")
TRACE_MSG(ISP_SERVICE_SC6530_4369_112_2_18_1_44_3_386,"disp_block_ptr->width = %d,disp_block_ptr->height = %d")
TRACE_MSG(ISP_SERVICE_SC6530_4411_112_2_18_1_44_3_387,"ISP_SERVICE: _ISP_SetScaleParam")
TRACE_MSG(ISP_SERVICE_SC6530_4424_112_2_18_1_44_3_388,"ISP_SERVICE: _ISP_SetScaleParam, slice heght %d")
TRACE_MSG(ISP_SERVICE_SC6530_4548_112_2_18_1_44_4_389,"ISP_SERVICE: _ISP_SetDenoiseParam")
TRACE_MSG(ISP_SERVICE_SC6530_4595_112_2_18_1_44_4_390,"ISP_SERVICE: _ISP_ServiceSetVTScaleParam")
TRACE_MSG(ISP_SERVICE_SC6530_4621_112_2_18_1_44_4_391,"Scale in VT mode, width of target image is too large")
TRACE_MSG(ISP_SERVICE_SC6530_4648_112_2_18_1_44_4_392,"ISP_SERVICE: _ISP_ServiceEncodeFrameInit, frame 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_4682_112_2_18_1_44_4_393,"ISP_SERVICE: _ISP_SetVTEncodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_4792_112_2_18_1_44_4_394,"_ISP_ServiceSetVTEncodeParam,display_enable = %d,review_enable = %d")
TRACE_MSG(ISP_SERVICE_SC6530_4813_112_2_18_1_44_4_395,"ISP_SERVICE: _ISP_ServiceSetVTExtEncodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_4930_112_2_18_1_44_4_396,"_ISP_ServiceSetVTExtEncodeParam,display_enable = %d,review_enable = %d, skip num %d ")
TRACE_MSG(ISP_SERVICE_SC6530_4950_112_2_18_1_44_4_397,"ISP_SERVICE: _ISP_SetVTDecodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_4986_112_2_18_1_44_4_398,"_ISP_ServiceSetVTDecodeParam,display_enable = %d,review_enable = %d")
TRACE_MSG(ISP_SERVICE_SC6530_5033_112_2_18_1_44_4_399,"ISP_SERVICE: _ISP_ServiceSetVTExtScaleParam")
TRACE_MSG(ISP_SERVICE_SC6530_5066_112_2_18_1_44_5_400,"ISP_SERVICE: _ISP_CheckPreviewParam")
TRACE_MSG(ISP_SERVICE_SC6530_5077_112_2_18_1_44_5_401,"ISP_SERVICE: All width must be 4 pixels aligned")
TRACE_MSG(ISP_SERVICE_SC6530_5083_112_2_18_1_44_5_402,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC6530_5089_112_2_18_1_44_5_403,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC6530_5097_112_2_18_1_44_5_404,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC6530_5103_112_2_18_1_44_5_405,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC6530_5110_112_2_18_1_44_5_406,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_SC6530_5121_112_2_18_1_44_5_407,"ISP_SERVICE: _ISP_CheckJpegParam")
TRACE_MSG(ISP_SERVICE_SC6530_5132_112_2_18_1_44_5_408,"ISP_SERVICE: All width must be 4 pixels align")
TRACE_MSG(ISP_SERVICE_SC6530_5145_112_2_18_1_44_5_409,"ISP_SERVICE: Input range is out of input size, {%d %d} {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_5163_112_2_18_1_44_5_410,"ISP_SERVICE: _ISP_CheckMpegParam")
TRACE_MSG(ISP_SERVICE_SC6530_5175_112_2_18_1_44_5_411,"ISP_SERVICE: All width must be 4 pixels align")
TRACE_MSG(ISP_SERVICE_SC6530_5181_112_2_18_1_44_5_412,"ISP_SERVICE: Unsupported data format")
TRACE_MSG(ISP_SERVICE_SC6530_5189_112_2_18_1_44_5_413,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC6530_5195_112_2_18_1_44_5_414,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC6530_5202_112_2_18_1_44_5_415,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC6530_5209_112_2_18_1_44_5_416,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC6530_5225_112_2_18_1_44_5_417,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_SC6530_5240_112_2_18_1_44_5_418,"ISP_SERVICE: _ISP_CheckReviewParam")
TRACE_MSG(ISP_SERVICE_SC6530_5253_112_2_18_1_44_5_419,"ISP_SERVICE: input_format = %d")
TRACE_MSG(ISP_SERVICE_SC6530_5261_112_2_18_1_44_5_420,"ISP_SERVICE: All width must be 4 pixels align")
TRACE_MSG(ISP_SERVICE_SC6530_5273_112_2_18_1_44_5_421,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_5282_112_2_18_1_44_5_422,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_5289_112_2_18_1_44_5_423,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC6530_5296_112_2_18_1_44_5_424,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC6530_5301_112_2_18_1_44_5_425,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC6530_5307_112_2_18_1_44_5_426,"ISP_SERVICE: Width of display rect is out of threshold")
TRACE_MSG(ISP_SERVICE_SC6530_5319_112_2_18_1_44_5_427,"ISP_SERVICE: _ISP_CheckScaleParam")
TRACE_MSG(ISP_SERVICE_SC6530_5329_112_2_18_1_44_5_428,"ISP_SERVICE: slice mode sline height = 0")
TRACE_MSG(ISP_SERVICE_SC6530_5342_112_2_18_1_44_5_429,"ISP_SERVICE: input_format = %d")
TRACE_MSG(ISP_SERVICE_SC6530_5353_112_2_18_1_44_5_430,"ISP_SERVICE: output_format = %d")
TRACE_MSG(ISP_SERVICE_SC6530_5361_112_2_18_1_44_5_431,"ISP_SERVICE: All width must be 4 pixel align")
TRACE_MSG(ISP_SERVICE_SC6530_5367_112_2_18_1_44_5_432,"ISP_SERVICE: input start x must be 4 pixel align when scale")
TRACE_MSG(ISP_SERVICE_SC6530_5374_112_2_18_1_44_5_433,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC6530_5387_112_2_18_1_44_5_434,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_5396_112_2_18_1_44_5_435,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_5405_112_2_18_1_44_5_436,"ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported")
TRACE_MSG(ISP_SERVICE_SC6530_5418_112_2_18_1_44_5_437,"ISP_SERVICE: _ISP_CheckDenoiseParam")
TRACE_MSG(ISP_SERVICE_SC6530_5427_112_2_18_1_44_5_438,"ISP_SERVICE: input_format = %d, not support denoise format")
TRACE_MSG(ISP_SERVICE_SC6530_5433_112_2_18_1_44_5_439,"ISP_SERVICE: output_format = %d, not support denoise format")
TRACE_MSG(ISP_SERVICE_SC6530_5440_112_2_18_1_44_5_440,"ISP_SERVICE: All width must be 4 pixel align")
TRACE_MSG(ISP_SERVICE_SC6530_5447_112_2_18_1_44_5_441,"ISP_SERVICE: output size is not corresponding with input size")
TRACE_MSG(ISP_SERVICE_SC6530_5456_112_2_18_1_44_5_442,"ISP_SERVICE: output rect now not supporting cropping")
TRACE_MSG(ISP_SERVICE_SC6530_5462_112_2_18_1_44_5_443,"ISP_SERVICE: too large size for denoise function")
TRACE_MSG(ISP_SERVICE_SC6530_5475_112_2_18_1_44_5_444,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_5484_112_2_18_1_44_5_445,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_5493_112_2_18_1_44_5_446,"ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported")
TRACE_MSG(ISP_SERVICE_SC6530_5507_112_2_18_1_44_5_447,"ISP_SERVICE: _ISP_CheckVTEncodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_5519_112_2_18_1_44_5_448,"ISP_SERVICE: All width must be 4 pixels aligned")
TRACE_MSG(ISP_SERVICE_SC6530_5530_112_2_18_1_44_5_449,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC6530_5536_112_2_18_1_44_5_450,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC6530_5544_112_2_18_1_44_5_451,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC6530_5550_112_2_18_1_44_5_452,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC6530_5556_112_2_18_1_44_5_453,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_SC6530_5567_112_2_18_1_44_5_454,"ISP_SERVICE: _ISP_ServiceCheckVTExtEncodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_5579_112_2_18_1_44_6_455,"ISP_SERVICE: All width must be 4 pixels aligned")
TRACE_MSG(ISP_SERVICE_SC6530_5590_112_2_18_1_44_6_456,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC6530_5596_112_2_18_1_44_6_457,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC6530_5604_112_2_18_1_44_6_458,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC6530_5610_112_2_18_1_44_6_459,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC6530_5618_112_2_18_1_44_6_460,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_SC6530_5629_112_2_18_1_44_6_461,"ISP_SERVICE: _ISP_CheckVTDecodeParam")
TRACE_MSG(ISP_SERVICE_SC6530_5640_112_2_18_1_44_6_462,"ISP_SERVICE: All width must be 4 pixel align")
TRACE_MSG(ISP_SERVICE_SC6530_5646_112_2_18_1_44_6_463,"ISP_SERVICE: input start x must be 4 pixel align when review")
TRACE_MSG(ISP_SERVICE_SC6530_5653_112_2_18_1_44_6_464,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC6530_5664_112_2_18_1_44_6_465,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC6530_5670_112_2_18_1_44_6_466,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC6530_5676_112_2_18_1_44_6_467,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC6530_5730_112_2_18_1_44_6_468,"ISP_SERVICE: To do image scaling")
TRACE_MSG(ISP_SERVICE_SC6530_5734_112_2_18_1_44_6_469,"ISP_SERVICE: state error! state %d, service %d")
TRACE_MSG(ISP_SERVICE_SC6530_5760_112_2_18_1_44_6_470,"ISP_SERVICE:transform the image frame, the flag of src and dst: 0x%x to 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_5780_112_2_18_1_44_6_471,"ISP_SERVICE:_ISP_ServiceStartPreview")
TRACE_MSG(ISP_SERVICE_SC6530_5867_112_2_18_1_44_6_472,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_5887_112_2_18_1_44_6_473,"ISP_SERVICE:_ISP_ServiceStartJpeg")
TRACE_MSG(ISP_SERVICE_SC6530_5985_112_2_18_1_44_6_474,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_6177_112_2_18_1_44_7_475,"ISP_SERVICE: s->total_scale_out %d s->encoder_size.h %d")
TRACE_MSG(ISP_SERVICE_SC6530_6304_112_2_18_1_44_7_476,"ISP_SERVICE:_ISP_ServiceInternScale, %d")
TRACE_MSG(ISP_SERVICE_SC6530_6316_112_2_18_1_44_7_477,"_ISP_ServiceInternScale, src size {%d %d}, src trim {%d %d %d %d}, dst {%d %d}, src fmt %d, dst fmt %d")
TRACE_MSG(ISP_SERVICE_SC6530_6322_112_2_18_1_44_7_478,"_ISP_ServiceInternScale, src addr {0x%x 0x%x}, dst addr {0x%x 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_6427_112_2_18_1_44_7_479,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_6449_112_2_18_1_44_7_480,"ISP_SERVICE:_ISP_ServiceCaptureOnce, time %d, output ftm %d, size {%d %d}")
TRACE_MSG(ISP_SERVICE_SC6530_6548_112_2_18_1_44_7_481,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_6563_112_2_18_1_44_7_482,"ISP_SERVICE:_ISP_ServiceStartMpeg")
TRACE_MSG(ISP_SERVICE_SC6530_6586_112_2_18_1_44_7_483,"ISP_SERVICE: _ISP_ServiceMpegHandler, p_frame->flags 0x%x ")
TRACE_MSG(ISP_SERVICE_SC6530_6590_112_2_18_1_44_7_484,"ISP_SERVICE: MPEG stopped")
TRACE_MSG(ISP_SERVICE_SC6530_6599_112_2_18_1_44_7_485,"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one capture frame, %d")
TRACE_MSG(ISP_SERVICE_SC6530_6608_112_2_18_1_44_7_486,"ISP_SERVICE: _ISP_ServiceMpegHandler, encode frame not released")
TRACE_MSG(ISP_SERVICE_SC6530_6647_112_2_18_1_44_8_487,"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one encode frame, %d")
TRACE_MSG(ISP_SERVICE_SC6530_6687_112_2_18_1_44_8_488,"ISP_SERVICE: _ISP_ServiceMpegHandler, encode frame 0x%x not released!")
TRACE_MSG(ISP_SERVICE_SC6530_6719_112_2_18_1_44_8_489,"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one review frame, %d")
TRACE_MSG(ISP_SERVICE_SC6530_6760_112_2_18_1_44_8_490,"ISP_SERVICE:_ISP_ServiceStartReview, %d")
TRACE_MSG(ISP_SERVICE_SC6530_6844_112_2_18_1_44_8_491,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_6863_112_2_18_1_44_8_492,"ISP_SERVICE:_ISP_ServiceContinueReviewing, %d")
TRACE_MSG(ISP_SERVICE_SC6530_6944_112_2_18_1_44_8_493,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_6964_112_2_18_1_44_8_494,"ISP_SERVICE:_ISP_ServiceStartScale,input addr {0x%x 0x%x 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_7080_112_2_18_1_44_8_495,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_7104_112_2_18_1_44_8_496,"ISP_SERVICE:_ISP_ServiceContinueScaling, %d")
TRACE_MSG(ISP_SERVICE_SC6530_7167_112_2_18_1_44_9_497,"ISP_SERVICE:_ISP_ServiceContinueScaling,input addr {0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC6530_7200_112_2_18_1_44_9_498,"ISP_SERVICE:_ISP_ServiceContinueScaling,encoder_addr {0x%x, 0x%x} ,total_slice_count %d")
TRACE_MSG(ISP_SERVICE_SC6530_7219_112_2_18_1_44_9_499,"ISP_SERVICE:_ISP_ServiceContinueScaling,slice_height %d")
TRACE_MSG(ISP_SERVICE_SC6530_7279_112_2_18_1_44_9_500,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_7294_112_2_18_1_44_9_501,"ISP_SERVICE:_ISP_ServiceStartVTEncode, %d")
TRACE_MSG(ISP_SERVICE_SC6530_7320_112_2_18_1_44_9_502,"ISP_SERVICE: _ISP_ServiceVTHandler, p_frame->type 0x%x , time %d")
TRACE_MSG(ISP_SERVICE_SC6530_7324_112_2_18_1_44_9_503,"ISP_SERVICE: VP stopped")
TRACE_MSG(ISP_SERVICE_SC6530_7333_112_2_18_1_44_9_504,"ISP_SERVICE: _ISP_ServiceVTHandler, Got one captured frame, %d")
TRACE_MSG(ISP_SERVICE_SC6530_7397_112_2_18_1_44_9_505,"ISP_SERVICE: _ISP_ServiceVTHandler, Got one uplink frame, %d")
TRACE_MSG(ISP_SERVICE_SC6530_7437_112_2_18_1_44_9_506,"ISP_SERVICE: _ISP_ServiceVTHandler, encode frame 0x%x not released!")
TRACE_MSG(ISP_SERVICE_SC6530_7474_112_2_18_1_44_9_507,"ISP_SERVICE: _ISP_ServiceVTHandler, Got one local review frame, %d")
TRACE_MSG(ISP_SERVICE_SC6530_7504_112_2_18_1_44_9_508,"ISP_SERVICE: _ISP_ServiceVTHandler, Got one remote review frame, %d")
TRACE_MSG(ISP_SERVICE_SC6530_7505_112_2_18_1_44_9_509,"ISP_SERVICE: remote display %d")
TRACE_MSG(ISP_SERVICE_SC6530_7530_112_2_18_1_44_9_510,"ISP_SERVICE:_ISP_ServiceStartVTExtEncode, %d")
TRACE_MSG(ISP_SERVICE_SC6530_7552_112_2_18_1_44_9_511,"ISP_SERVICE: _ISP_ServiceVTExtHandler, p_frame->type 0x%x , time %d")
TRACE_MSG(ISP_SERVICE_SC6530_7556_112_2_18_1_44_9_512,"ISP_SERVICE: VT extension mode stopped")
TRACE_MSG(ISP_SERVICE_SC6530_7579_112_2_18_1_44_9_513,"ISP_SERVICE: _ISP_ServiceVTExtHandler,Frame unlocked")
TRACE_MSG(ISP_SERVICE_SC6530_7595_112_2_18_1_44_9_514,"ISP_SERVICE: _ISP_ServiceVTExtHandler, Frame has not been unlocked")
TRACE_MSG(ISP_SERVICE_SC6530_7787_112_2_18_1_44_10_515,"ISP_SERVICE:Cap fifo overflow, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_7804_112_2_18_1_44_10_516,"ISP_SERVICE:Sensor line error, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_7821_112_2_18_1_44_10_517,"ISP_SERVICE:Sensor Frame error, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_7834_112_2_18_1_44_10_518,"ISP_SERVICE:Sensor Jpeg buffer error, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_7882_112_2_18_1_44_10_519,"_ISP_ServiceGetDefaultMemSize, lcd {%d %d} mem_size 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_7907_112_2_18_1_44_10_520,"_ISP_ServiceAllocFrame, Mem not given by caller, should alloc-ed here")
TRACE_MSG(ISP_SERVICE_SC6530_7918_112_2_18_1_44_10_521,"_ISP_ServiceAllocFrame, NO memory, try again 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_7924_112_2_18_1_44_10_522,"_ISP_ServiceAllocFrame, NO memory reserved for ISP, 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_7946_112_2_18_1_44_10_523,"_ISP_ServiceAllocFrame, Mem given by caller, start 0x%x ,length 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_8461_112_2_18_1_44_11_524,"_ISP_ServiceAllocFrame, line_buff , 0x%x swap_buff y,u, 0x%x,0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_8540_112_2_18_1_44_11_525,"_ISP_ServiceAllocFrame: the denoise target buffer already alloc by caller, y address %d")
TRACE_MSG(ISP_SERVICE_SC6530_8763_112_2_18_1_44_12_526,"ISP_SERVICE:ISP_ServiceIsVTOn, service = %d")
TRACE_MSG(ISP_SERVICE_SC6530_8975_112_2_18_1_44_12_527,"ISP_SERVICE: _ISP_CheckScaleParam")
TRACE_MSG(ISP_SERVICE_SC6530_8984_112_2_18_1_44_12_528,"ISP_SERVICE: unsupported scaling down size")
TRACE_MSG(ISP_SERVICE_SC6530_8990_112_2_18_1_44_12_529,"ISP_SERVICE: unsupported scaling down size")
TRACE_MSG(ISP_SERVICE_SC6530_8997_112_2_18_1_44_12_530,"ISP_SERVICE: unsupported scaling up size")
TRACE_MSG(ISP_SERVICE_SC6530_9004_112_2_18_1_44_12_531,"ISP_SERVICE: slice height is not aligned, %d")
TRACE_MSG(ISP_SERVICE_SC6530_9011_112_2_18_1_44_12_532,"ISP_SERVICE: trim rect error")
TRACE_MSG(ISP_SERVICE_SC6530_9017_112_2_18_1_44_12_533,"ISP_SERVICE: trim rect not aligned by 4 pixels")
TRACE_MSG(ISP_SERVICE_SC6530_9023_112_2_18_1_44_12_534,"ISP_SERVICE: the width of output_size not aligned by 4 pixels")
TRACE_MSG(ISP_SERVICE_SC6530_9029_112_2_18_1_44_12_535,"ISP_SERVICE: unsupported scaling mode")
TRACE_MSG(ISP_SERVICE_SC6530_9035_112_2_18_1_44_12_536,"ISP_SERVICE: get_scale_data is NULL")
TRACE_MSG(ISP_SERVICE_SC6530_9042_112_2_18_1_44_12_537,"ISP_SERVICE: the address of surce data is NULL")
TRACE_MSG(ISP_SERVICE_SC6530_9060_112_2_18_1_44_12_538,"ISP_ServiceMemConfig, mem_ptr 0x%x, mem len 0x%x ,isp_status %d")
TRACE_MSG(ISP_SERVICE_SC6530_9066_112_2_18_1_44_12_539,"ISP_ServiceMemConfig, mem_ptr 0x%x, mem len 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_9151_112_2_18_1_44_12_540,"ISP_ServiceAsyncDisplay, 0x%x")
TRACE_MSG(ISP_SERVICE_SC6530_9157_112_2_18_1_44_12_541,"ISP_ServiceAsyncDisplay, Wrong status or service %d %d")
TRACE_MSG(ISP_SERVICE_SC6530_9166_112_2_18_1_44_12_542,"ISP_ServiceAsyncDisplay, frame_ptr 0x%x p_frame 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_475_112_2_18_1_44_14_543,"ISP_SERVICE:ISP_ServiceOpen, service = %d, state %d, cur_time %d, wait_opt %d")
TRACE_MSG(ISP_SERVICE_SC8800G_482_112_2_18_1_44_14_544,"ISP_SERVICE:ISP_ServiceOpen, time out %d")
TRACE_MSG(ISP_SERVICE_SC8800G_488_112_2_18_1_44_14_545,"ISP_SERVICE:ISP_ServiceOpen, Got controller,cur_timr %d")
TRACE_MSG(ISP_SERVICE_SC8800G_508_112_2_18_1_44_14_546,"ISP_SERVICE:ISP_ServiceClose, service = %d, status %d")
TRACE_MSG(ISP_SERVICE_SC8800G_566_112_2_18_1_44_14_547,"ISP_SERVICE:ISP_ServiceSetPreviewParam")
TRACE_MSG(ISP_SERVICE_SC8800G_576_112_2_18_1_44_14_548,"ISP_SERVICE:input.x ,y,w,h {%d,%d,%d,%d} lcd rect {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC8800G_585_112_2_18_1_44_14_549,"ISP_SERVICE:lcd_id %d,block_id %d, disp_range.x,y,w,h {%d,%d,%d,%d} rot mode %d")
TRACE_MSG(ISP_SERVICE_SC8800G_589_112_2_18_1_44_14_550,"ISP_SERVICE:skip_frames %d, deci_frames %d")
TRACE_MSG(ISP_SERVICE_SC8800G_619_112_2_18_1_44_14_551,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC8800G_660_112_2_18_1_44_14_552,"ISP_SERVICE:ISP_ServiceStartPreview")
TRACE_MSG(ISP_SERVICE_SC8800G_682_112_2_18_1_44_15_553,"ISP_SERVICE:ISP_ServiceStopPreview")
TRACE_MSG(ISP_SERVICE_SC8800G_717_112_2_18_1_44_15_554,"ISP_SERVICE:ISP_ServiceStartReview,wrong state")
TRACE_MSG(ISP_SERVICE_SC8800G_721_112_2_18_1_44_15_555,"ISP_SERVICE:ISP_ServiceStartReview")
TRACE_MSG(ISP_SERVICE_SC8800G_773_112_2_18_1_44_15_556,"ISP_SERVICE:ISP_ServiceReviewNext address 0x%x, {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_811_112_2_18_1_44_15_557,"ISP_SERVICE:ISP_ServiceSetReviewParam")
TRACE_MSG(ISP_SERVICE_SC8800G_822_112_2_18_1_44_15_558,"ISP_SERVICE:input format = %d, input.x,y,w,h {%d %d %d %d} lcd rect {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC8800G_831_112_2_18_1_44_15_559,"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} rot mode %d")
TRACE_MSG(ISP_SERVICE_SC8800G_863_112_2_18_1_44_15_560,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC8800G_886_112_2_18_1_44_15_561,"ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam")
TRACE_MSG(ISP_SERVICE_SC8800G_892_112_2_18_1_44_15_562,"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_898_112_2_18_1_44_15_563,"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d")
TRACE_MSG(ISP_SERVICE_SC8800G_932_112_2_18_1_44_15_564,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC8800G_944_112_2_18_1_44_15_565,"ISP_SERVICE:ISP_ServiceStartCaptureJpeg")
TRACE_MSG(ISP_SERVICE_SC8800G_960_112_2_18_1_44_15_566,"ISP_SERVICE:ISP_ServiceSetCompoundJpegCaptureParam")
TRACE_MSG(ISP_SERVICE_SC8800G_968_112_2_18_1_44_15_567,"ISP_SERVICE:input format = %d,yuv_type = %d,output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_997_112_2_18_1_44_15_568,"ISP_SERVICE:ISP_ServiceSetMpegCaptureParam")
TRACE_MSG(ISP_SERVICE_SC8800G_1005_112_2_18_1_44_15_569,"ISP_SERVICE: input.x,y,w,h {%d %d %d %d},skip_frames %d, deci_frames %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1014_112_2_18_1_44_15_570,"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}, lcd rect {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1024_112_2_18_1_44_15_571,"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} disp rot %d enc rot %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1055_112_2_18_1_44_15_572,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1074_112_2_18_1_44_15_573,"ISP_ServiceGetMpegCaptureParam, Not in MPEG mode")
TRACE_MSG(ISP_SERVICE_SC8800G_1136_112_2_18_1_44_15_574,"ISP_SERVICE:ISP_ServiceStartCaptureMpeg")
TRACE_MSG(ISP_SERVICE_SC8800G_1168_112_2_18_1_44_16_575,"ISP_SERVICE:ISP_ServiceIsMpegOn, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1186_112_2_18_1_44_16_576,"ISP_SERVICE:ISP_ServiceStopCapture")
TRACE_MSG(ISP_SERVICE_SC8800G_1218_112_2_18_1_44_16_577,"ISP_SERVICE:ISP_ServiceStopReview")
TRACE_MSG(ISP_SERVICE_SC8800G_1249_112_2_18_1_44_16_578,"ISP_SERVICE:ISP_ServiceSetScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800G_1256_112_2_18_1_44_16_579,"ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1261_112_2_18_1_44_16_580,"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_1266_112_2_18_1_44_16_581,"ISP_SERVICE:output_format = %d, output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1299_112_2_18_1_44_16_582,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1336_112_2_18_1_44_16_583,"ISP_SERVICE: ISP_ServiceFreeFrameAddress, frame 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_1360_112_2_18_1_44_16_584,"ISP_SERVICE:ISP_ServiceStartVT")
TRACE_MSG(ISP_SERVICE_SC8800G_1386_112_2_18_1_44_16_585,"ISP_SERVICE:ISP_ServiceStartVT, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1396_112_2_18_1_44_16_586,"ISP_SERVICE:_ISP_ServiceStartVTExtScale")
TRACE_MSG(ISP_SERVICE_SC8800G_1416_112_2_18_1_44_16_587,"ISP_SERVICE:ISP_ServiceStartVTExt")
TRACE_MSG(ISP_SERVICE_SC8800G_1443_112_2_18_1_44_16_588,"ISP_SERVICE:ISP_ServiceStartVTExt, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1464_112_2_18_1_44_16_589,"ISP_SERVICE:ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_1471_112_2_18_1_44_16_590,"ISP_SERVICE:wait for path1 done")
TRACE_MSG(ISP_SERVICE_SC8800G_1475_112_2_18_1_44_16_591,"ISP_SERVICE:ISP_ServiceSetVTDecodeAddr")
TRACE_MSG(ISP_SERVICE_SC8800G_1502_112_2_18_1_44_16_592,"ISP_SERVICE:ISP_ServiceSetVTExtScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800G_1508_112_2_18_1_44_16_593,"ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1512_112_2_18_1_44_16_594,"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_1516_112_2_18_1_44_16_595,"ISP_SERVICE:output_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_1520_112_2_18_1_44_16_596,"ISP_SERVICE:output_format = %d, output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1556_112_2_18_1_44_16_597,"ISP_SERVICE:ISP_ServiceSetVTDecodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_1562_112_2_18_1_44_16_598,"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1570_112_2_18_1_44_16_599,"ISP_SERVICE:remote_disp_lcd_id=%d, remote_disp_block_id=%d,remote_disp_range.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1576_112_2_18_1_44_16_600,"ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,uaddr = %d, vaddr = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1609_112_2_18_1_44_16_601,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1623_112_2_18_1_44_16_602,"ISP_SERVICE:ISP_ServiceSetVTEncodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_1629_112_2_18_1_44_16_603,"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1636_112_2_18_1_44_16_604,"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d, deci_frames %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1644_112_2_18_1_44_16_605,"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1675_112_2_18_1_44_16_606,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1690_112_2_18_1_44_16_607,"ISP_SERVICE:ISP_ServiceSetVTExtEncodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_1696_112_2_18_1_44_16_608,"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1701_112_2_18_1_44_16_609,"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1709_112_2_18_1_44_16_610,"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_1739_112_2_18_1_44_17_611,"ISP_SERVICE: error code %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1758_112_2_18_1_44_17_612,"ISP_ServiceGetVTEncodeParam, Not in VT mode")
TRACE_MSG(ISP_SERVICE_SC8800G_1828_112_2_18_1_44_17_613,"ISP_SERVICE:ISP_ServiceStopVT")
TRACE_MSG(ISP_SERVICE_SC8800G_1865_112_2_18_1_44_17_614,"ISP_SERVICE:ISP_ServiceStopVT, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_1878_112_2_18_1_44_17_615,"ISP_SERVICE:ISP_ServiceStopVTExt")
TRACE_MSG(ISP_SERVICE_SC8800G_1909_112_2_18_1_44_17_616,"ISP_SERVICE:ISP_ServiceStopVTExt, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_2007_112_2_18_1_44_17_617,"ISP_Service: _ISP_ServiceMainMessageLoop,command=0x%x param=0x%x SignalCode 0x%x sig_ptr->ptr 0x%x time %d")
TRACE_MSG(ISP_SERVICE_SC8800G_2131_112_2_18_1_44_17_618,"ISP_SERVICE: display one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_2155_112_2_18_1_44_17_619,"ISP_SERVICE: vp get one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_2160_112_2_18_1_44_17_620,"ISP_SERVICE: VT Stopped, don't do local review")
TRACE_MSG(ISP_SERVICE_SC8800G_2171_112_2_18_1_44_17_621,"ISP_SERVICE:transform the local frame from yuv422 to yuv420")
TRACE_MSG(ISP_SERVICE_SC8800G_2177_112_2_18_1_44_17_622,"ISP_SERVICE: timeout, couldn't convert yuv422 to yuv420")
TRACE_MSG(ISP_SERVICE_SC8800G_2209_112_2_18_1_44_18_623,"ISP_SERVICE: vp get one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_2222_112_2_18_1_44_18_624,"ISP_SERVICE:transform the local frame from yuv422 to rgb565")
TRACE_MSG(ISP_SERVICE_SC8800G_2235_112_2_18_1_44_18_625,"ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565")
TRACE_MSG(ISP_SERVICE_SC8800G_2391_112_2_18_1_44_18_626,"ISP_SERVICE: Encode frame not released by user, drop this frame")
TRACE_MSG(ISP_SERVICE_SC8800G_2411_112_2_18_1_44_18_627,"ISP_SERVICE: flag %d")
TRACE_MSG(ISP_SERVICE_SC8800G_2453_112_2_18_1_44_18_628,"ISP_SERVICE: unsupported frame flags")
TRACE_MSG(ISP_SERVICE_SC8800G_2460_112_2_18_1_44_18_629,"ISP_SERVICE: flag %d")
TRACE_MSG(ISP_SERVICE_SC8800G_2468_112_2_18_1_44_18_630,"ISP_SERVICE:vp transform to yuv420 done,p_frame = 0x%x,y,u,v {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_2500_112_2_18_1_44_18_631,"ISP_SERVICE: encode frame switched to next one 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_2528_112_2_18_1_44_18_632,"ISP_SERVICE: remote display %d")
TRACE_MSG(ISP_SERVICE_SC8800G_2540_112_2_18_1_44_18_633,"ISP_SERVICE: unsupported frame flags")
TRACE_MSG(ISP_SERVICE_SC8800G_2703_112_2_18_1_44_19_634,"ISP_SERVICE: s->total_scale_out %d s->encoder_size.h %d")
TRACE_MSG(ISP_SERVICE_SC8800G_2718_112_2_18_1_44_19_635,"ISP_SERVICE: display one frame ,y,u,v {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_3005_112_2_18_1_44_19_636,"_ISP_ServiceSendRequest, ISP Service Queue full")
TRACE_MSG(ISP_SERVICE_SC8800G_3077_112_2_18_1_44_19_637,"ISP_SERVICE: queue info:enqueued:%d, store: %d, count: %d")
TRACE_MSG(ISP_SERVICE_SC8800G_3260_112_2_18_1_44_20_638,"_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d")
TRACE_MSG(ISP_SERVICE_SC8800G_3309_112_2_18_1_44_20_639,"_ISP_ServiceConvertCoor,dst x y w h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_3355_112_2_18_1_44_20_640,"ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC8800G_3401_112_2_18_1_44_20_641,"ISP_SERVICE: block id %d blk_cfg.start.x,y end_x,y {%d,%d,%d,%d}, resolution %d")
TRACE_MSG(ISP_SERVICE_SC8800G_3425_112_2_18_1_44_20_642,"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_3433_112_2_18_1_44_20_643,"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_3443_112_2_18_1_44_20_644,"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_3463_112_2_18_1_44_20_645,"ISP_SERVICE: _ISP_ServiceCloseBlock disp_blk enable %d remote disp blk enable %d")
TRACE_MSG(ISP_SERVICE_SC8800G_3606_112_2_18_1_44_20_646,"_ISP_ServiceDisplayOneFrame: Block %d has been enabled")
TRACE_MSG(ISP_SERVICE_SC8800G_3612_112_2_18_1_44_20_647,"ISP_SERVICE: block enable %d,p_block->lcd_id %d, p_block->block_id %d ")
TRACE_MSG(ISP_SERVICE_SC8800G_3643_112_2_18_1_44_20_648,"_ISP_ServiceDisplayOneFrame: s->is_first_frame = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_3701_112_2_18_1_44_20_649,"ISP_SERVICE: _ISP_ServiceRotateImage, rot_mode=%d,format=%d,img_size.w,h %d %d,")
TRACE_MSG(ISP_SERVICE_SC8800G_3708_112_2_18_1_44_20_650,"src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_3711_112_2_18_1_44_20_651,"ISP_SERVICE: _ISP_ServiceRotateImage, SUCCESS")
TRACE_MSG(ISP_SERVICE_SC8800G_3713_112_2_18_1_44_20_652,"ISP_SERVICE: _ISP_ServiceRotateImage, FAIL")
TRACE_MSG(ISP_SERVICE_SC8800G_3792_112_2_18_1_44_21_653,"restore ARMCLK")
TRACE_MSG(ISP_SERVICE_SC8800G_3899_112_2_18_1_44_21_654,"ISP_SERVICE: _ISP_SetPreviewParam")
TRACE_MSG(ISP_SERVICE_SC8800G_3960_112_2_18_1_44_21_655,"ISP_SERVICE: _ISP_SetPreviewParam,input for scaling {%d,%d,%d,%d}, output from CAP {%d,%d}")
TRACE_MSG(ISP_SERVICE_SC8800G_3993_112_2_18_1_44_21_656,"ISP_SERVICE: _ISP_SetJpegParam")
TRACE_MSG(ISP_SERVICE_SC8800G_4063_112_2_18_1_44_21_657,"ISP_SERVICE: _ISP_SetMpegParam")
TRACE_MSG(ISP_SERVICE_SC8800G_4256_112_2_18_1_44_22_658,"ISP_SERVICE: _ISP_SetReviewParam")
TRACE_MSG(ISP_SERVICE_SC8800G_4314_112_2_18_1_44_22_659,"disp_block_ptr->width = %d,disp_block_ptr->height = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_4352_112_2_18_1_44_22_660,"ISP_SERVICE: _ISP_SetScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800G_4365_112_2_18_1_44_22_661,"ISP_SERVICE: _ISP_SetScaleParam, slice heght %d")
TRACE_MSG(ISP_SERVICE_SC8800G_4385_112_2_18_1_44_22_662,"_ISP_ServiceSetScaleParam, rgb_ex_height %d")
TRACE_MSG(ISP_SERVICE_SC8800G_4551_112_2_18_1_44_22_663,"ISP_SERVICE: _ISP_ServiceSetVTScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800G_4577_112_2_18_1_44_22_664,"Scale in VT mode, width of target image is too large")
TRACE_MSG(ISP_SERVICE_SC8800G_4608_112_2_18_1_44_22_665,"ISP_SERVICE: _ISP_ServiceEncodeFrameInit, frame 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_4632_112_2_18_1_44_22_666,"ISP_SERVICE: _ISP_SetVTEncodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_4721_112_2_18_1_44_23_667,"_ISP_ServiceSetVTEncodeParam,display_enable = %d,review_enable = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_4742_112_2_18_1_44_23_668,"ISP_SERVICE: _ISP_ServiceSetVTExtEncodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_4847_112_2_18_1_44_23_669,"_ISP_ServiceSetVTExtEncodeParam,display_enable = %d,review_enable = %d, skip num %d ")
TRACE_MSG(ISP_SERVICE_SC8800G_4867_112_2_18_1_44_23_670,"ISP_SERVICE: _ISP_SetVTDecodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_4902_112_2_18_1_44_23_671,"_ISP_ServiceSetVTDecodeParam,display_enable = %d,review_enable = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_4949_112_2_18_1_44_23_672,"ISP_SERVICE: _ISP_ServiceSetVTExtScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5010_112_2_18_1_44_23_673,"ISP_SERVICE: _ISP_CheckPreviewParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5021_112_2_18_1_44_23_674,"ISP_SERVICE: All width must be 4 pixels aligned")
TRACE_MSG(ISP_SERVICE_SC8800G_5027_112_2_18_1_44_23_675,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800G_5033_112_2_18_1_44_23_676,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800G_5041_112_2_18_1_44_23_677,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800G_5047_112_2_18_1_44_23_678,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC8800G_5054_112_2_18_1_44_23_679,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_SC8800G_5065_112_2_18_1_44_23_680,"ISP_SERVICE: _ISP_CheckJpegParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5076_112_2_18_1_44_23_681,"ISP_SERVICE: All width must be 4 pixels align")
TRACE_MSG(ISP_SERVICE_SC8800G_5083_112_2_18_1_44_23_682,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800G_5117_112_2_18_1_44_24_683,"ISP_SERVICE: _ISP_CheckMpegParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5129_112_2_18_1_44_24_684,"ISP_SERVICE: All width must be 4 pixels align")
TRACE_MSG(ISP_SERVICE_SC8800G_5135_112_2_18_1_44_24_685,"ISP_SERVICE: Unsupported data format")
TRACE_MSG(ISP_SERVICE_SC8800G_5143_112_2_18_1_44_24_686,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800G_5149_112_2_18_1_44_24_687,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800G_5156_112_2_18_1_44_24_688,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800G_5163_112_2_18_1_44_24_689,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC8800G_5179_112_2_18_1_44_24_690,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_SC8800G_5194_112_2_18_1_44_24_691,"ISP_SERVICE: _ISP_CheckReviewParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5207_112_2_18_1_44_24_692,"ISP_SERVICE: input_format = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_5215_112_2_18_1_44_24_693,"ISP_SERVICE: All width must be 4 pixels align")
TRACE_MSG(ISP_SERVICE_SC8800G_5221_112_2_18_1_44_24_694,"ISP_SERVICE: input start x must be 4 pixels align when review")
TRACE_MSG(ISP_SERVICE_SC8800G_5234_112_2_18_1_44_24_695,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_5243_112_2_18_1_44_24_696,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_5250_112_2_18_1_44_24_697,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800G_5255_112_2_18_1_44_24_698,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800G_5262_112_2_18_1_44_24_699,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800G_5267_112_2_18_1_44_24_700,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC8800G_5273_112_2_18_1_44_24_701,"ISP_SERVICE: Width of display rect is out of threshold")
TRACE_MSG(ISP_SERVICE_SC8800G_5285_112_2_18_1_44_24_702,"ISP_SERVICE: _ISP_CheckScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5295_112_2_18_1_44_24_703,"ISP_SERVICE: slice mode sline height = 0")
TRACE_MSG(ISP_SERVICE_SC8800G_5308_112_2_18_1_44_24_704,"ISP_SERVICE: input_format = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_5317_112_2_18_1_44_24_705,"ISP_SERVICE: output_format = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_5325_112_2_18_1_44_24_706,"ISP_SERVICE: All width must be 4 pixel align")
TRACE_MSG(ISP_SERVICE_SC8800G_5331_112_2_18_1_44_24_707,"ISP_SERVICE: input start x must be 4 pixel align when scale")
TRACE_MSG(ISP_SERVICE_SC8800G_5338_112_2_18_1_44_24_708,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800G_5351_112_2_18_1_44_24_709,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_5360_112_2_18_1_44_24_710,"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_5369_112_2_18_1_44_24_711,"ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported")
TRACE_MSG(ISP_SERVICE_SC8800G_5385_112_2_18_1_44_24_712,"ISP_SERVICE: _ISP_CheckVTEncodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5397_112_2_18_1_44_24_713,"ISP_SERVICE: All width must be 4 pixels aligned")
TRACE_MSG(ISP_SERVICE_SC8800G_5408_112_2_18_1_44_24_714,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800G_5414_112_2_18_1_44_24_715,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800G_5422_112_2_18_1_44_24_716,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800G_5428_112_2_18_1_44_24_717,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC8800G_5434_112_2_18_1_44_24_718,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_SC8800G_5445_112_2_18_1_44_24_719,"ISP_SERVICE: _ISP_ServiceCheckVTExtEncodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5457_112_2_18_1_44_24_720,"ISP_SERVICE: All width must be 4 pixels aligned")
TRACE_MSG(ISP_SERVICE_SC8800G_5468_112_2_18_1_44_24_721,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800G_5474_112_2_18_1_44_24_722,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800G_5482_112_2_18_1_44_24_723,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800G_5488_112_2_18_1_44_24_724,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC8800G_5496_112_2_18_1_44_24_725,"ISP_SERVICE: Display size is larger than max lcd size")
TRACE_MSG(ISP_SERVICE_SC8800G_5507_112_2_18_1_44_24_726,"ISP_SERVICE: _ISP_CheckVTDecodeParam")
TRACE_MSG(ISP_SERVICE_SC8800G_5518_112_2_18_1_44_24_727,"ISP_SERVICE: All width must be 4 pixel align")
TRACE_MSG(ISP_SERVICE_SC8800G_5524_112_2_18_1_44_24_728,"ISP_SERVICE: input start x must be 4 pixel align when review")
TRACE_MSG(ISP_SERVICE_SC8800G_5531_112_2_18_1_44_24_729,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800G_5542_112_2_18_1_44_24_730,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800G_5548_112_2_18_1_44_24_731,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800G_5554_112_2_18_1_44_24_732,"ISP_SERVICE: Rotation angle error")
TRACE_MSG(ISP_SERVICE_SC8800G_5658_112_2_18_1_44_25_733,"ISP_SERVICE:_ISP_ServiceStartVTScale, dithering enabled!")
TRACE_MSG(ISP_SERVICE_SC8800G_5677_112_2_18_1_44_25_734,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_5696_112_2_18_1_44_25_735,"ISP_SERVICE:_ISP_ServiceStartVTReview ,review_from = %d, y,u,v = {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_5873_112_2_18_1_44_25_736,"ISP_SERVICE:_ISP_ServiceStartVTReview ,display_buffer 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_5913_112_2_18_1_44_25_737,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_5929_112_2_18_1_44_25_738,"ISP_SERVICE: To display remote frame,s->remote_review_enable = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_5935_112_2_18_1_44_25_739,"ISP_SERVICE:display the remote frame")
TRACE_MSG(ISP_SERVICE_SC8800G_5943_112_2_18_1_44_25_740,"ISP_SERVICE: No mem for remote in buffer")
TRACE_MSG(ISP_SERVICE_SC8800G_5951_112_2_18_1_44_25_741,"ISP_SERVICE: timeout, don't display the remote frame")
TRACE_MSG(ISP_SERVICE_SC8800G_5969_112_2_18_1_44_25_742,"ISP_SERVICE: To do image scaling")
TRACE_MSG(ISP_SERVICE_SC8800G_5973_112_2_18_1_44_25_743,"ISP_SERVICE: state error!")
TRACE_MSG(ISP_SERVICE_SC8800G_5992_112_2_18_1_44_25_744,"ISP_SERVICE: NO memory for input!")
TRACE_MSG(ISP_SERVICE_SC8800G_6020_112_2_18_1_44_25_745,"ISP_SERVICE:transform the image frame from %d to %d")
TRACE_MSG(ISP_SERVICE_SC8800G_6036_112_2_18_1_44_25_746,"ISP_SERVICE: NO memory for output!")
TRACE_MSG(ISP_SERVICE_SC8800G_6049_112_2_18_1_44_25_747,"ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565")
TRACE_MSG(ISP_SERVICE_SC8800G_6063_112_2_18_1_44_25_748,"ISP_SERVICE:transform the local frame from yuv422 to rgb565")
TRACE_MSG(ISP_SERVICE_SC8800G_6068_112_2_18_1_44_25_749,"ISP_SERVICE:p_frame = 0x%x,y,u,v {0x%x,0x%x,0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_6074_112_2_18_1_44_25_750,"ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565")
TRACE_MSG(ISP_SERVICE_SC8800G_6088_112_2_18_1_44_25_751,"ISP_SERVICE: _ISP_ServiceVTTimeout, to restart VT")
TRACE_MSG(ISP_SERVICE_SC8800G_6111_112_2_18_1_44_25_752,"ISP_SERVICE: performance_level = %d, watchdog_feeded = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_6132_112_2_18_1_44_25_753,"ISP_SERVICE: _ISP_ServiceTimerExpireHandle,unsupported timeout mode")
TRACE_MSG(ISP_SERVICE_SC8800G_6155_112_2_18_1_44_26_754,"ISP_SERVICE : _ISP_VTCreateTimer, ret_val %d")
TRACE_MSG(ISP_SERVICE_SC8800G_6172_112_2_18_1_44_26_755,"ISP_SERVICE: _ISP_ServiceCloseTimer")
TRACE_MSG(ISP_SERVICE_SC8800G_6187_112_2_18_1_44_26_756,"_DCAMERA_DeleteTimer")
TRACE_MSG(ISP_SERVICE_SC8800G_6212_112_2_18_1_44_26_757,"ISP_SERVICE: _ISP_ServiceStartTimer,: mode %d, expire value %d, ret_val, %d")
TRACE_MSG(ISP_SERVICE_SC8800G_6225_112_2_18_1_44_26_758,"ISP_SERVICE:_ISP_ServiceStartPreview")
TRACE_MSG(ISP_SERVICE_SC8800G_6396_112_2_18_1_44_26_759,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_6415_112_2_18_1_44_26_760,"ISP_SERVICE:_ISP_ServiceStartJpeg")
TRACE_MSG(ISP_SERVICE_SC8800G_6555_112_2_18_1_44_26_761,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_6579_112_2_18_1_44_26_762,"ISP_SERVICE:_ISP_ServiceStartMpeg")
TRACE_MSG(ISP_SERVICE_SC8800G_6746_112_2_18_1_44_27_763,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_6764_112_2_18_1_44_27_764,"ISP_SERVICE:_ISP_ServiceStartReview")
TRACE_MSG(ISP_SERVICE_SC8800G_6850_112_2_18_1_44_27_765,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_6903_112_2_18_1_44_27_766,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_6923_112_2_18_1_44_27_767,"ISP_SERVICE:_ISP_ServiceStartScale,input addr {0x%x 0x%x 0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_7033_112_2_18_1_44_27_768,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_7056_112_2_18_1_44_28_769,"ISP_SERVICE:_ISP_ServiceContinueScaling")
TRACE_MSG(ISP_SERVICE_SC8800G_7097_112_2_18_1_44_28_770,"ISP_SERVICE:_ISP_ServiceContinueScaling,input addr {0x%x, 0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_7121_112_2_18_1_44_28_771,"ISP_SERVICE:_ISP_ServiceContinueScaling,encoder_addr {0x%x, 0x%x} ,total_slice_count %d")
TRACE_MSG(ISP_SERVICE_SC8800G_7133_112_2_18_1_44_28_772,"ISP_SERVICE:_ISP_ServiceContinueScaling,slice_height %d")
TRACE_MSG(ISP_SERVICE_SC8800G_7154_112_2_18_1_44_28_773,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_7170_112_2_18_1_44_28_774,"ISP_SERVICE:_ISP_ServiceStartVTEncode")
TRACE_MSG(ISP_SERVICE_SC8800G_7295_112_2_18_1_44_28_775,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_7312_112_2_18_1_44_28_776,"ISP_SERVICE:_ISP_ServiceStartVTExtEncode")
TRACE_MSG(ISP_SERVICE_SC8800G_7429_112_2_18_1_44_28_777,"ISP_SERVICE:driver error code 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_7536_112_2_18_1_44_28_778,"ISP_SERVICE:Cap fifo overflow, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_7549_112_2_18_1_44_29_779,"ISP_SERVICE:Sensor line error, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_7562_112_2_18_1_44_29_780,"ISP_SERVICE:Sensor Frame error, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_7575_112_2_18_1_44_29_781,"ISP_SERVICE:Sensor Jpeg buffer error, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_7654_112_2_18_1_44_29_782,"_ISP_ServiceAllocFrame, Mem not given by caller, should alloc-ed here")
TRACE_MSG(ISP_SERVICE_SC8800G_7667_112_2_18_1_44_29_783,"_ISP_ServiceAllocFrame, NO memory reserved for ISP")
TRACE_MSG(ISP_SERVICE_SC8800G_7678_112_2_18_1_44_29_784,"_ISP_ServiceAllocFrame, Memory not in one 64M Segement,try again")
TRACE_MSG(ISP_SERVICE_SC8800G_7687_112_2_18_1_44_29_785,"_ISP_ServiceAllocFrame, %d block mem freed")
TRACE_MSG(ISP_SERVICE_SC8800G_7692_112_2_18_1_44_29_786,"_ISP_ServiceAllocFrame, Memory successfully,total try times %d ,addr 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_7708_112_2_18_1_44_29_787,"_ISP_ServiceAllocFrame, Mem given by caller, start 0x%x ,length 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_7987_112_2_18_1_44_29_788,"_ISP_ServiceAllocFrame, line_buff , 0x%x swap_buff y,u, 0x%x,0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_8286_112_2_18_1_44_30_789,"ISP_SERVICE:ISP_ServiceIsVTOn, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8494_112_2_18_1_44_31_790,"ISP_SERVICE: _ISP_CheckScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800G_8503_112_2_18_1_44_31_791,"ISP_SERVICE: unsupported scaling down size")
TRACE_MSG(ISP_SERVICE_SC8800G_8509_112_2_18_1_44_31_792,"ISP_SERVICE: unsupported scaling down size")
TRACE_MSG(ISP_SERVICE_SC8800G_8516_112_2_18_1_44_31_793,"ISP_SERVICE: unsupported scaling up size")
TRACE_MSG(ISP_SERVICE_SC8800G_8523_112_2_18_1_44_31_794,"ISP_SERVICE: slice height is not aligned, %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8530_112_2_18_1_44_31_795,"ISP_SERVICE: trim rect error")
TRACE_MSG(ISP_SERVICE_SC8800G_8536_112_2_18_1_44_31_796,"ISP_SERVICE: trim rect not aligned by 4 pixels")
TRACE_MSG(ISP_SERVICE_SC8800G_8542_112_2_18_1_44_31_797,"ISP_SERVICE: the width of output_size not aligned by 4 pixels")
TRACE_MSG(ISP_SERVICE_SC8800G_8548_112_2_18_1_44_31_798,"ISP_SERVICE: unsupported scaling mode")
TRACE_MSG(ISP_SERVICE_SC8800G_8554_112_2_18_1_44_31_799,"ISP_SERVICE: get_scale_data is NULL")
TRACE_MSG(ISP_SERVICE_SC8800G_8561_112_2_18_1_44_31_800,"ISP_SERVICE: the address of surce data is NULL")
TRACE_MSG(ISP_SERVICE_SC8800G_8588_112_2_18_1_44_31_801,"_ISP_ServiceRGB888FromBigToLittle, addr 0x%x, pixel_num %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8647_112_2_18_1_44_31_802,"ISP_ServiceMemConfig, mem_ptr 0x%x, mnem len 0x%x ,isp_status %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8653_112_2_18_1_44_31_803,"ISP_ServiceMemConfig, mem_ptr 0x%x, mnem len 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_8715_112_2_18_1_44_31_804,"_ISP_ServiceCheckMemAddr, param error, p_addr 0x%x, input_size 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_8726_112_2_18_1_44_31_805,"_ISP_ServiceCheckMemAddr, param error, in_fmt %d, input_size {%d %d}")
TRACE_MSG(ISP_SERVICE_SC8800G_8731_112_2_18_1_44_31_806,"_ISP_ServiceCheckMemAddr, in, p_addr, {0x%x 0x%x 0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_8744_112_2_18_1_44_31_807,"_ISP_ServiceCheckMemAddr, Need adjust the memory address, img_fmt %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8762_112_2_18_1_44_31_808,"_ISP_ServiceCheckMemAddr, No memory, work_mem_len 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_8813_112_2_18_1_44_31_809,"_ISP_ServiceCheckMemAddr, Need adjust the memory address, img_fmt %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8828_112_2_18_1_44_31_810,"_ISP_ServiceCheckMemAddr, No memory, work_mem_len 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800G_8848_112_2_18_1_44_31_811,"_ISP_ServiceCheckMemAddr, unsupported image fotmat %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8856_112_2_18_1_44_31_812,"_ISP_ServiceCheckMemAddr, out, p_addr, {0x%x 0x%x 0x%x}")
TRACE_MSG(ISP_SERVICE_SC8800G_8874_112_2_18_1_44_31_813,"_ISP_ServiceSyncMemToOutput ,parameter error")
TRACE_MSG(ISP_SERVICE_SC8800G_8883_112_2_18_1_44_31_814,"_ISP_ServiceSyncMemToOutput ,format error %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8893_112_2_18_1_44_31_815,"_ISP_ServiceSyncMemToOutput, in, p_in_addr, {0x%x 0x%x}, p_out_addr {0x%x 0x%x},fmt %d")
TRACE_MSG(ISP_SERVICE_SC8800G_8897_112_2_18_1_44_31_816,"_ISP_ServiceSyncMemToOutput, No need to Sync memory")
TRACE_MSG(ISP_SERVICE_SC8800H_420_112_2_18_1_44_38_817,"ISP_Service: Low performance ,skip this frame")
TRACE_MSG(ISP_SERVICE_SC8800H_428_112_2_18_1_44_38_818,"ISP_Service: middle performance ,skip this frame")
TRACE_MSG(ISP_SERVICE_SC8800H_504_112_2_18_1_44_39_819,"_ISP_ServiceOnPortB,flags = %d,p->rgbaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_974_112_2_18_1_44_40_820,"ISP: lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_985_112_2_18_1_44_40_821,"ISP: lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_989_112_2_18_1_44_40_822,"_ISP_Backuplcdblockinfo same do nothing")
TRACE_MSG(ISP_SERVICE_SC8800H_1002_112_2_18_1_44_40_823,"ISP_SERVICE: _ISP_Restorelcdblockinfo, lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_1019_112_2_18_1_44_40_824,"ISP_SERVICE: _ISP_CheckPreviewParam")
TRACE_MSG(ISP_SERVICE_SC8800H_1025_112_2_18_1_44_40_825,"ISP_SERVICE: All width must be 2 pixel align")
TRACE_MSG(ISP_SERVICE_SC8800H_1031_112_2_18_1_44_40_826,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800H_1037_112_2_18_1_44_40_827,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800H_1045_112_2_18_1_44_40_828,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800H_1064_112_2_18_1_44_40_829,"ISP_SERVICE: _ISP_CheckJpegParam")
TRACE_MSG(ISP_SERVICE_SC8800H_1070_112_2_18_1_44_40_830,"ISP_SERVICE: All width must be 2 pixel align")
TRACE_MSG(ISP_SERVICE_SC8800H_1077_112_2_18_1_44_40_831,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800H_1094_112_2_18_1_44_40_832,"ISP_SERVICE: _ISP_CheckMpegParam")
TRACE_MSG(ISP_SERVICE_SC8800H_1100_112_2_18_1_44_40_833,"ISP_SERVICE: All width must be 2 pixel align")
TRACE_MSG(ISP_SERVICE_SC8800H_1108_112_2_18_1_44_40_834,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800H_1114_112_2_18_1_44_40_835,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800H_1121_112_2_18_1_44_40_836,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800H_1144_112_2_18_1_44_40_837,"ISP_SERVICE: _ISP_CheckReviewParam")
TRACE_MSG(ISP_SERVICE_SC8800H_1151_112_2_18_1_44_40_838,"ISP_SERVICE: input_format = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_1157_112_2_18_1_44_40_839,"ISP_SERVICE: All width must be 2 pixel align")
TRACE_MSG(ISP_SERVICE_SC8800H_1163_112_2_18_1_44_40_840,"ISP_SERVICE: input start x must be 4 pixel align when review")
TRACE_MSG(ISP_SERVICE_SC8800H_1169_112_2_18_1_44_40_841,"ISP_SERVICE: Invalid lcd id")
TRACE_MSG(ISP_SERVICE_SC8800H_1175_112_2_18_1_44_40_842,"ISP_SERVICE: Invalid lcd id or block id")
TRACE_MSG(ISP_SERVICE_SC8800H_1182_112_2_18_1_44_40_843,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800H_1199_112_2_18_1_44_40_844,"ISP_SERVICE: _ISP_CheckScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800H_1206_112_2_18_1_44_40_845,"ISP_SERVICE: input_format = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_1213_112_2_18_1_44_40_846,"ISP_SERVICE: input_format = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_1219_112_2_18_1_44_40_847,"ISP_SERVICE: All width must be 2 pixel align")
TRACE_MSG(ISP_SERVICE_SC8800H_1225_112_2_18_1_44_40_848,"ISP_SERVICE: input start x must be 4 pixel align when scale")
TRACE_MSG(ISP_SERVICE_SC8800H_1232_112_2_18_1_44_40_849,"ISP_SERVICE: Input range is out of input size")
TRACE_MSG(ISP_SERVICE_SC8800H_1255_112_2_18_1_44_40_850,"_ISP_SetPreviewParam")
TRACE_MSG(ISP_SERVICE_SC8800H_1301_112_2_18_1_44_40_851,"ISP: _ISP_StartPreview")
TRACE_MSG(ISP_SERVICE_SC8800H_1418_112_2_18_1_44_40_852,"_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d")
TRACE_MSG(ISP_SERVICE_SC8800H_1469_112_2_18_1_44_41_853,"_ISP_ServiceConvertCoor,dst x y w h {%d %d %d %d}")
TRACE_MSG(ISP_SERVICE_SC8800H_1557_112_2_18_1_44_41_854,"ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}")
TRACE_MSG(ISP_SERVICE_SC8800H_1619_112_2_18_1_44_41_855,"_ISP_SetJpegParam")
TRACE_MSG(ISP_SERVICE_SC8800H_1654_112_2_18_1_44_41_856,"_ISP_StartJpeg")
TRACE_MSG(ISP_SERVICE_SC8800H_1830_112_2_18_1_44_41_857,"_ISP_SetScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800H_1841_112_2_18_1_44_41_858,"ISP_SERVICE: output_format = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_1884_112_2_18_1_44_41_859,"_ISP_StartScale")
TRACE_MSG(ISP_SERVICE_SC8800H_2157_112_2_18_1_44_42_860,"_ISP_StartSliceScale, slice_id=%d")
TRACE_MSG(ISP_SERVICE_SC8800H_2417_112_2_18_1_44_42_861,"_ISP_SetReviewParam")
TRACE_MSG(ISP_SERVICE_SC8800H_2462_112_2_18_1_44_43_862,"_ISP_StartReview, cur time: %d")
TRACE_MSG(ISP_SERVICE_SC8800H_2606_112_2_18_1_44_43_863,"_ISP_SetMpegParam")
TRACE_MSG(ISP_SERVICE_SC8800H_2664_112_2_18_1_44_43_864,"_ISP_StartMpeg")
TRACE_MSG(ISP_SERVICE_SC8800H_2824_112_2_18_1_44_43_865,"_ISP_SetVTEncodeParam")
TRACE_MSG(ISP_SERVICE_SC8800H_2874_112_2_18_1_44_44_866,"_ISP_SetVTEncodeParam,s->local_display_enable = %d,s->local_review_enable = %d,s->display_rotation %d")
TRACE_MSG(ISP_SERVICE_SC8800H_2926_112_2_18_1_44_44_867,"_ISP_SetVTDecodeParam,s->remote_display_enable = %d,s->remote_review_enable = %d,s->remote_display_rotation %d")
TRACE_MSG(ISP_SERVICE_SC8800H_2944_112_2_18_1_44_44_868,"ISP_SERVICE: _ISP_StartVTEncode")
TRACE_MSG(ISP_SERVICE_SC8800H_2950_112_2_18_1_44_44_869,"ISP_SERVICE: _ISP_StartVTEncode,Sleeping")
TRACE_MSG(ISP_SERVICE_SC8800H_2960_112_2_18_1_44_44_870,"ISP_SERVICE: _ISP_StartVTEncode,start VT capture")
TRACE_MSG(ISP_SERVICE_SC8800H_3147_112_2_18_1_44_44_871,"_ISP_StartVTDisplay, f->yaddr = 0x%x, f->uaddr = 0x%x, f->vaddr  = 0x%x,f->rgbaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_3194_112_2_18_1_44_45_872,"_ISP_StartVTDisplay, f->yaddr = 0x%x, f->uaddr = 0x%x, f->vaddr  = 0x%x,f->rgbaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_3251_112_2_18_1_44_45_873,"App_queue:\n")
TRACE_MSG(ISP_SERVICE_SC8800H_3263_112_2_18_1_44_45_874,"queue info:enqueued:%d, store: %d, count: %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3316_112_2_18_1_44_45_875,"ISP_ServiceFreeFrameAddress,yaddr=0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_3332_112_2_18_1_44_45_876,"ISP_SERVICE:ISP_ServiceSetReviewParam")
TRACE_MSG(ISP_SERVICE_SC8800H_3335_112_2_18_1_44_45_877,"ISP_SERVICE:input format = %d,input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3338_112_2_18_1_44_45_878,"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3370_112_2_18_1_44_45_879,"ISP_SERVICE:ISP_ServiceStartPlayback")
TRACE_MSG(ISP_SERVICE_SC8800H_3445_112_2_18_1_44_45_880,"ISP_SERVICE:ISP_ServiceSetPreviewParam")
TRACE_MSG(ISP_SERVICE_SC8800H_3448_112_2_18_1_44_45_881,"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3451_112_2_18_1_44_45_882,"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3505_112_2_18_1_44_45_883,"ISP_SERVICE:ISP_ServiceStartPreview")
TRACE_MSG(ISP_SERVICE_SC8800H_3532_112_2_18_1_44_45_884,"ISP_SERVICE:ISP_ServiceStopPreview")
TRACE_MSG(ISP_SERVICE_SC8800H_3572_112_2_18_1_44_45_885,"ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam")
TRACE_MSG(ISP_SERVICE_SC8800H_3575_112_2_18_1_44_45_886,"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3578_112_2_18_1_44_45_887,"ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3599_112_2_18_1_44_45_888,"ISP_SERVICE:ISP_ServiceStartCaptureJpeg")
TRACE_MSG(ISP_SERVICE_SC8800H_3613_112_2_18_1_44_45_889,"ISP_SERVICE:ISP_ServiceSetCompoundJpegCaptureParam")
TRACE_MSG(ISP_SERVICE_SC8800H_3618_112_2_18_1_44_46_890,"ISP_SERVICE:input format = %d,yuv_shift = %d,output.w = %d,output.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3674_112_2_18_1_44_46_891,"ISP_SERVICE:ISP_ServiceSetMpegCaptureParam")
TRACE_MSG(ISP_SERVICE_SC8800H_3677_112_2_18_1_44_46_892,"ISP_SERVICE: input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3680_112_2_18_1_44_46_893,"ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3683_112_2_18_1_44_46_894,"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3715_112_2_18_1_44_46_895,"ISP_SERVICE:ISP_ServiceStartCaptureMpeg")
TRACE_MSG(ISP_SERVICE_SC8800H_3742_112_2_18_1_44_46_896,"ISP_SERVICE:ISP_ServiceStopCapture")
TRACE_MSG(ISP_SERVICE_SC8800H_3782_112_2_18_1_44_46_897,"ISP_SERVICE:ISP_ServiceSetScaleParam")
TRACE_MSG(ISP_SERVICE_SC8800H_3785_112_2_18_1_44_46_898,"ISP_SERVICE:input_format = %d,input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3788_112_2_18_1_44_46_899,"ISP_SERVICE:input_y_addr = 0x%x, input_u_addr = 0x%x, input_v_addr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_3791_112_2_18_1_44_46_900,"ISP_SERVICE:output_format = %d, yuv_shift = %d,output.w = %d,output.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3811_112_2_18_1_44_46_901,"ISP_SERVICE:ISP_ServiceSetVTEncodeParam")
TRACE_MSG(ISP_SERVICE_SC8800H_3814_112_2_18_1_44_46_902,"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3817_112_2_18_1_44_46_903,"ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3821_112_2_18_1_44_46_904,"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x=%d, local_disp_range.y=%d, local_disp_range.w=%d, local_disp_range.h=%d")
TRACE_MSG(ISP_SERVICE_SC8800H_3881_112_2_18_1_44_46_905,"ISP_SERVICE:ISP_ServiceSetVTDecodeParam")
TRACE_MSG(ISP_SERVICE_SC8800H_3884_112_2_18_1_44_46_906,"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3888_112_2_18_1_44_46_907,"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x=%d, local_disp_range.y=%d, local_disp_range.w=%d, local_disp_range.h=%d")
TRACE_MSG(ISP_SERVICE_SC8800H_3891_112_2_18_1_44_46_908,"ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,u_input_addr = %d, v_input_addr = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3910_112_2_18_1_44_46_909,"ISP_SERVICE: _ISP_ServiceVTTimeout")
TRACE_MSG(ISP_SERVICE_SC8800H_3937_112_2_18_1_44_46_910,"ISP_SERVICE: _ISP_ServiceTimerExpireHandle,s_isp_performance = %d, s_b_vt_watchdog_feeded = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3952_112_2_18_1_44_46_911,"ISP_SERVICE: _ISP_ServiceTimerExpireHandle,not supported timeout mode")
TRACE_MSG(ISP_SERVICE_SC8800H_3976_112_2_18_1_44_46_912,"ISP_SERVICE : _ISP_VTCreateTimer, ret_val %d")
TRACE_MSG(ISP_SERVICE_SC8800H_3991_112_2_18_1_44_46_913,"ISP_SERVICE: _ISP_ServiceCloseTimer")
TRACE_MSG(ISP_SERVICE_SC8800H_4003_112_2_18_1_44_46_914,"_DCAMERA_DeleteTimer")
TRACE_MSG(ISP_SERVICE_SC8800H_4029_112_2_18_1_44_46_915,"ISP_SERVICE: _ISP_ServiceStartTimer,: mode %d, expire value %d, ret_val, %d")
TRACE_MSG(ISP_SERVICE_SC8800H_4047_112_2_18_1_44_46_916,"ISP_SERVICE:ISP_ServiceStartVT")
TRACE_MSG(ISP_SERVICE_SC8800H_4076_112_2_18_1_44_46_917,"ISP_SERVICE:ISP_ServiceStopVT")
TRACE_MSG(ISP_SERVICE_SC8800H_4142_112_2_18_1_44_47_918,"ISP_SERVICE:ISP_ServiceStartScale")
TRACE_MSG(ISP_SERVICE_SC8800H_4170_112_2_18_1_44_47_919,"ISP_SERVICE: wait for first porta frame")
TRACE_MSG(ISP_SERVICE_SC8800H_4176_112_2_18_1_44_47_920,"ISP_Service: Low performance ,skip the remote frame")
TRACE_MSG(ISP_SERVICE_SC8800H_4185_112_2_18_1_44_47_921,"ISP_Service: middle performance ,skip the remote frame")
TRACE_MSG(ISP_SERVICE_SC8800H_4191_112_2_18_1_44_47_922,"ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_4453_112_2_18_1_44_47_923,"ISP: mpeg get two frame already")
TRACE_MSG(ISP_SERVICE_SC8800H_4503_112_2_18_1_44_47_924,"ISP_SERVICE: To display local frame, s->local_review_enable = %d,s->is_first_frame = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_4524_112_2_18_1_44_47_925,"ISP_SERVICE: display the local frame")
TRACE_MSG(ISP_SERVICE_SC8800H_4530_112_2_18_1_44_47_926,"ISP_SERVICE: timeout, don't display the local frame")
TRACE_MSG(ISP_SERVICE_SC8800H_4623_112_2_18_1_44_47_927,"ISP_SERVICE: _ISP_ServiceRotateRGBImage, input for Rotation_AppStart: {rotPara.rotation_mode=%d,rotPara.data_format=%d,rotPara.img_size.w=%d,rotPara.img_size.h=%d,rotPara.src_addr.y_addr=%d,rotPara.src_addr.uv_addr=%d,rotPara.src_addr.v_addr=%d}")
TRACE_MSG(ISP_SERVICE_SC8800H_4626_112_2_18_1_44_47_928,"ISP_SERVICE: _ISP_ServiceRotateRGBImage, SUCCESS")
TRACE_MSG(ISP_SERVICE_SC8800H_4628_112_2_18_1_44_47_929,"ISP_SERVICE: _ISP_ServiceRotateRGBImage, FAIL")
TRACE_MSG(ISP_SERVICE_SC8800H_4689_112_2_18_1_44_48_930,"_ISP_ServiceDisplayOneRGBFrame ,rot_angle %d,rot_tmp %d,lcd_rect.w %d h %d")
TRACE_MSG(ISP_SERVICE_SC8800H_4710_112_2_18_1_44_48_931,"_ISP_ServiceDisplayOneRGBFrame ,lcd %d,block %d,addr 0x%x,p_disp_block->width %d,height %d")
TRACE_MSG(ISP_SERVICE_SC8800H_4764_112_2_18_1_44_48_932,"ISP_Service, _ISP_ServicePortBPreview, 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_4783_112_2_18_1_44_48_933,"ISP: display first frame")
TRACE_MSG(ISP_SERVICE_SC8800H_4799_112_2_18_1_44_48_934,"ISP_SERVICE_PORTB_SIG before ,param = %d ,rgbaddr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_4813_112_2_18_1_44_48_935,"ISP: display first frame")
TRACE_MSG(ISP_SERVICE_SC8800H_4900_112_2_18_1_44_48_936,"ISP_Service: display %d frame, 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_5006_112_2_18_1_44_48_937,"ISP: ISP_ServiceRecieveRequest,command=0x%x param=0x%x,ptr = 0x%x")
TRACE_MSG(ISP_SERVICE_SC8800H_5017_112_2_18_1_44_48_938," ISP_ServiceOpen() !!")
TRACE_MSG(ISP_SERVICE_SC8800H_5128_112_2_18_1_44_48_939,"ISP_SERVICE: To display remote frame,s->remote_review_enable = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_5141_112_2_18_1_44_48_940,"ISP_SERVICE: timeout, don't display the remote frame")
TRACE_MSG(ISP_SERVICE_SC8800H_5320_112_2_18_1_44_49_941,"ISP_ServiceSendRequest: curr_avliable = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_5327_112_2_18_1_44_49_942,"ISP_ServiceSendRequest: queue full not send the signal ")
TRACE_MSG(ISP_SERVICE_SC8800H_5379_112_2_18_1_44_49_943,"ISP_Service, Not closed since last opened")
TRACE_MSG(ISP_SERVICE_SC8800H_5383_112_2_18_1_44_49_944,"ISP_ServiceOpen")
TRACE_MSG(ISP_SERVICE_SC8800H_5411_112_2_18_1_44_49_945,"s_isp_chng_freq_req_handler = %x")
TRACE_MSG(ISP_SERVICE_SC8800H_5448_112_2_18_1_44_49_946,"ISP_ServiceClose, service = %d")
TRACE_MSG(ISP_SERVICE_SC8800H_5487_112_2_18_1_44_49_947,"restore ARMCLK")
END_TRACE_MAP(MS_REF_ISP_SERVICE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_ISP_SERVICE_TRC_H_

