/******************************************************************************
 ** File Name:      ms_ref_lcd_trc.h                                         *
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
//trace_id:56
#ifndef _MS_REF_LCD_TRC_H_
#define _MS_REF_LCD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define LAYER_BLEND_337_112_2_18_1_44_52_0 "[_SoftLayerBlend ERROR 0],dont supoort!"
#define LAYER_BLEND_356_112_2_18_1_44_52_1 "[_SoftLayerBlend ERROR 2],dont supoort!"
#define LAYER_BLEND_431_112_2_18_1_44_52_2 "[_SoftLayerBlend ERROR 0],dont supoort!"
#define LAYER_BLEND_448_112_2_18_1_44_52_3 "[_SoftLayerBlend ERROR 1],dont supoort!"
#define LAYER_BLEND_476_112_2_18_1_44_52_4 "[_SoftLayerBlend ERROR 2],dont supoort!"
#define LAYER_BLEND_575_112_2_18_1_44_53_5 "[_SoftLayerConvert ERROR],don't support RGB666!"
#define LAYER_BLEND_609_112_2_18_1_44_53_6 "[_SoftLayerConvert ERROR],don't support RGB888!"
#define LAYER_BLEND_614_112_2_18_1_44_53_7 "[_SoftLayerConvert ERROR],don't support output RGB666!"
#define LAYER_BLEND_620_112_2_18_1_44_53_8 "[_SoftLayerConvert ERROR],don't support RGB666 convertion!"
#define LAYER_BLEND_687_112_2_18_1_44_53_9 "[_SoftLayerConvert ERROR],don't support RGB666 convertion!"
#define LAYER_BLEND_751_112_2_18_1_44_53_10 "---_SoftInvalidateRectUsingLayer:malloc fial!---"
#define LAYER_BLEND_970_112_2_18_1_44_53_11 "covert_to_byte_num:result is 0,data_format is %d!"
#define LAYER_BLEND_1058_112_2_18_1_44_54_12 "[_ConfigLayer ERROR],layer position!"
#define LAYER_BLEND_1128_112_2_18_1_44_54_13 "_cap_data_type_convert:default rgb565,type=%d"
#define LAYER_BLEND_1233_112_2_18_1_44_54_14 "_HW_InvalidateRectUsingLayer:layer_num=%d"
#define LAYER_BLEND_1414_112_2_18_1_44_54_15 "--DISPLAY_BlendLayer:--array_size=%d--"
#define LAYER_BLEND_1483_112_2_18_1_44_55_16 "robert _SoftwareHandle  OSD_LAYER_BLEND"
#define LAYER_BLEND_1485_112_2_18_1_44_55_17 "robert _SoftwareHandle not OSD_LAYER_BLEND"
#define LAYER_BLEND_1522_112_2_18_1_44_55_18 "robert _HardwareHandle"
#define LAYER_BLEND_1526_112_2_18_1_44_55_19 "--DISPLAY_BlendLayer :--array_size=%d--"
#define LAYER_BLEND_1578_112_2_18_1_44_55_20 "_HardwareHandle,err_num=%d"
#define LAYER_BLEND_1621_112_2_18_1_44_55_21 "_HW_InvalidateRectUsingLayer"
#define LAYER_BLEND_1800_112_2_18_1_44_55_22 "DISPLAY_BlendLayer: Out"
#define LCD_169_112_2_18_1_44_56_23 "_lcd_data_format_convert is error! format = %d"
#define LCD_241_112_2_18_1_44_56_24 "LCD:Get Semaphore Fail!"
#define LCD_301_112_2_18_1_44_56_25 "LCD ERR:s_lcddone_callback is NULL!"
#define LCD_334_112_2_18_1_44_56_26 "[ProtectionOperation ERROR],lcd_id = %d"
#define LCD_349_112_2_18_1_44_56_27 "ProtectionOperation:err!"
#define LCD_358_112_2_18_1_44_56_28 "---ProtectionOperation: lcd operation is NUL!---"
#define LCD_401_112_2_18_1_44_56_29 "LCD:_AdjustRect:left=%d,right=%d"
#define LCD_820_112_2_18_1_44_57_30 "[LCD_GetInfo ERROR],lcd devsize is error! lcd_id=%d"
#define LCD_999_112_2_18_1_44_57_31 "LCD_Invalidate:Check State"
#define LCD_1914_112_2_18_1_44_59_32 "---LCD_UpdateTiming---: AHB Clock:%d"
#define LCD_CTRL_COMMON_134_112_2_18_1_45_0_33 "[_software_rotation ERROR],angle = %d"
#define LCD_CTRL_COMMON_198_112_2_18_1_45_1_34 "_rotation_by_hw:[ERROR]don't support format %d"
#define LCD_CTRL_COMMON_218_112_2_18_1_45_1_35 "LCDGUI_CTRL:Rotation_AppStart failed!"
#define LCD_CTRL_COMMON_243_112_2_18_1_45_1_36 "DISPLAY_ConvertCoordinate:rotation_angle=%d,left=%d,top=%d,right=%d,bottom=%d"
#define LCD_CTRL_COMMON_259_112_2_18_1_45_1_37 "_convert_coordinate:[ERROR] physical rect is error!"
#define LCD_CTRL_COMMON_334_112_2_18_1_45_1_38 "[LCDCTRL:]_convert_coordinate,rotaion_angle ERROR!"
#define LCD_CTRL_COMMON_341_112_2_18_1_45_1_39 "DISPLAY_ConvertCoordinate:out:angle=%d,left=%d,top=%d,right=%d,bottom=%d"
#define LCD_CTRL_COMMON_375_112_2_18_1_45_1_40 "_software_rotation_halfword:[ERROR]"
#define LCD_CTRL_COMMON_391_112_2_18_1_45_1_41 "_software_rotation_halfword:[ERROR]"
#define LCD_CTRL_COMMON_456_112_2_18_1_45_1_42 "_software_rotation_halfword:[ERROR]param is error! %d"
#define LCD_CTRL_COMMON_490_112_2_18_1_45_1_43 "_software_rotation_halfword:[ERROR]"
#define LCD_CTRL_COMMON_506_112_2_18_1_45_1_44 "_software_rotation_halfword:[ERROR]"
#define LCD_CTRL_COMMON_571_112_2_18_1_45_1_45 "_software_rotation_word:[ERROR]param is error! %d"
#define LCD_CTRL_COMMON_605_112_2_18_1_45_1_46 "_software_rotation_halfword:[ERROR]"
#define LCD_CTRL_COMMON_621_112_2_18_1_45_1_47 "_software_rotation_halfword:[ERROR]"
#define LCD_CTRL_COMMON_686_112_2_18_1_45_2_48 "_software_rotation_byte:[ERROR]param is error! %d"
#define LCD_CTRL_COMMON_724_112_2_18_1_45_2_49 "_software_rotation_with_crop:[ERROR]don't support format %d"
#define LCD_CTRL_COMMON_755_112_2_18_1_45_2_50 "_software_rotation_halfword:[ERROR]"
#define LCD_CTRL_COMMON_771_112_2_18_1_45_2_51 "[LCDCTRL:]_software_rotation_with_yuv,coordinate convert error!"
#define LCD_CTRL_COMMON_795_112_2_18_1_45_2_52 "[LCDCTRL:]_software_rotation_with_yuv,alloc fail!"
#define LCD_CTRL_COMMON_799_112_2_18_1_45_2_53 "[LCDCTRL:]_software_rotation_with_yuv,malloc addr=0x%x,size=%d"
#define LCD_CTRL_COMMON_875_112_2_18_1_45_2_54 "[LCDCTRL:],_software_rotation_with_yuv,convert ERROR!"
#define LCD_CTRL_COMMON_913_112_2_18_1_45_2_55 "_display_software_rotation:full copy"
#define LCD_CTRL_COMMON_926_112_2_18_1_45_2_56 "_display_software_rotation:crop copy"
#define LCD_CTRL_COMMON_951_112_2_18_1_45_2_57 "convert_angle: default!"
#define LCD_CTRL_COMMON_980_112_2_18_1_45_2_58 "convert_angle: default!"
#define LCD_CTRL_COMMON_1009_112_2_18_1_45_2_59 "LCDGUI_Ctrl:_invalidate_on_nor:dc mode"
#define LCD_CTRL_COMMON_1060_112_2_18_1_45_2_60 "_blend_and_rotate:param_ptr->rot_angle=%d,layer_num=%d"
#define LCD_CTRL_COMMON_1081_112_2_18_1_45_2_61 "[_blend_and_rotate ERROR]:angle is error!"
#define LCD_CTRL_COMMON_1176_112_2_18_1_45_3_62 "_display_software_rotation:[ERROR]"
#define LCD_CTRL_COMMON_1282_112_2_18_1_45_3_63 "dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d"
#define LCD_CTRL_COMMON_1344_112_2_18_1_45_3_64 "dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d"
#define LCD_CTRL_COMMON_1350_112_2_18_1_45_3_65 "[lcd_ctrl_common.c]:OSD copy don't support!"
#define LCD_CTRL_COMMON_1383_112_2_18_1_45_3_66 "param_ptr->rot_angle=%d,size=%d,layer_num=%d"
#define LCD_CTRL_COMMON_1407_112_2_18_1_45_3_67 "[DISPLAY] can alloc memory: addr: 0x%x, layer num: %d, total_size: %d, size: %d, type:%d "
#define LCD_CTRL_COMMON_1408_112_2_18_1_45_3_68 "[DISPLAY] lcd width: %d , lcd height %d "
#define LCD_CTRL_COMMON_1413_112_2_18_1_45_3_69 "[DISPLAY] can not alloc memory: layer num: %d, total_size: %d, size: %d, type:%d "
#define LCD_CTRL_COMMON_1414_112_2_18_1_45_3_70 "[DISPLAY] lcd width: %d , lcd height %d "
#define LCD_CTRL_COMMON_1437_112_2_18_1_45_3_71 "[_invalidate_on_nand:ERROR],_blend_and_rotate error!"
#define LCD_CTRL_COMMON_1458_112_2_18_1_45_3_72 "dst_ptr=0x%x,src_ptr=0x%x"
#define LCD_CTRL_COMMON_1463_112_2_18_1_45_3_73 "_display_software_rotation:[ERROR]"
#define LCD_CTRL_COMMON_1481_112_2_18_1_45_3_74 "dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d"
#define LCD_CTRL_COMMON_1616_112_2_18_1_45_3_75 "param_ptr->rot_angle=%d,size=%d"
#define LCD_CTRL_COMMON_1630_112_2_18_1_45_3_76 "dst_ptr=0x%x,src_ptr=0x%x"
#define LCD_CTRL_COMMON_1640_112_2_18_1_45_4_77 "blk_id=%d,time=%d"
#define LCD_CTRL_COMMON_1647_112_2_18_1_45_4_78 "_invalidate_on_nand: break,layer_num=%d,i=%d"
#define LCD_CTRL_COMMON_1786_112_2_18_1_45_4_79 "[_convert_cfg ERROR],display data type is error!"
#define LCD_CTRL_COMMON_1906_112_2_18_1_45_4_80 "covert_to_byte_num:result is 0,data_format is %d!"
#define LCD_CTRL_COMMON_1928_112_2_18_1_45_4_81 "_cap_data_type_convert:default rgb565,type=%d"
#define LCD_CTRL_COMMON_1943_112_2_18_1_45_4_82 "_invalidate_on_nand:%d,%d,0x%x"
#define LCD_CTRL_COMMON_1946_112_2_18_1_45_4_83 "_invalidate_on_nor:%d,%d"
#define LCD_FOR_FOTA_206_112_2_18_1_45_5_84 "ProtectionOperation:err!"
#define LCD_FOR_FOTA_633_112_2_18_1_45_6_85 "check lcdc state!"
#define LCD_FOR_FOTA_863_112_2_18_1_45_7_86 "LCD:Get Semaphore Fail!"
#define LCD_FOR_FOTA_962_112_2_18_1_45_7_87 "LCD_SetBrushMode Enter!mode:%d"
#define LCD_FOR_FOTA_1037_112_2_18_1_45_7_88 "LCD_SetBrushMode Exit!"
#define LCD_FOR_FOTA_1596_112_2_18_1_45_8_89 "----LCD_UpdateTiming-----"
#define LCD_FOR_FOTA_1602_112_2_18_1_45_8_90 "---LCD_UpdateTiming---: AHB Clock:%d"
#define LCD_FOR_FOTA_1621_112_2_18_1_45_8_91 "LCD_UpdateTiming:get semaphore 1"
#define LCD_FOR_FOTA_1672_112_2_18_1_45_8_92 "LCD_UpdateTiming:put semaphore 1"
#define LCD_FOTA_123_112_2_18_1_45_10_93 "_lcd_data_format_convert is error! format = %d"
#define LCD_FOTA_164_112_2_18_1_45_10_94 "ProtectionOperation:err!"
#define LCD_FOTA_171_112_2_18_1_45_10_95 "---ProtectionOperation: lcd operation is NUL!---"
#define LCD_FOTA_284_112_2_18_1_45_10_96 "LCD:Get Semaphore Fail!"
#define LCD_FOTA_634_112_2_18_1_45_11_97 "LCD_Invalidate:Check State"
#define LCD_FOTA_1190_112_2_18_1_45_12_98 "---LCD_UpdateTiming---: AHB Clock:%d"
#define LCD_INTERFACE_352_112_2_18_1_45_13_99 "DISPLAY_CopyToAuxMemory: invalid data type"
#define LCD_INTERFACE_503_112_2_18_1_45_13_100 "lcd_task blend: LCD_UPDATE_MODE_SYNC "
#define LCD_INTERFACE_515_112_2_18_1_45_13_101 "lcd_task invalidate: LCD_UPDATE_MODE_SYNC "
#define LCD_INTERFACE_532_112_2_18_1_45_13_102 "lcd_task blend: LCD_UPDATE_MODE_SYNC "
#define LCD_SC6600L_364_112_2_18_1_45_14_103 "LCD_Close() in tv on mode"
#define LCD_SC6600L_1615_112_2_18_1_45_17_104 "LCD:Get Semaphore Fail!"
#define LCD_SC6600L_1678_112_2_18_1_45_17_105 "LCD_SetBrushMode Enter!mode:%d"
#define LCD_SC8800H_259_112_2_18_1_45_18_106 "ProtectionOperation:err!"
#define LCD_SC8800H_722_112_2_18_1_45_19_107 "check lcdc state!"
#define LCD_SC8800H_1031_112_2_18_1_45_20_108 "LCD:Get Semaphore Fail!"
#define LCD_SC8800H_1127_112_2_18_1_45_20_109 "set event"
#define LCD_SC8800H_1186_112_2_18_1_45_20_110 "LCD ERR:s_lcddone_callback is NULL!"
#define LCD_SC8800H_1238_112_2_18_1_45_20_111 "LCD_SetBrushMode Enter!mode:%d"
#define LCD_SC8800H_1338_112_2_18_1_45_20_112 "LCD_SetBrushMode Exit!"
#define LCD_SC8800H_2458_112_2_18_1_45_23_113 "---lcdc_num=%d,lcd_sleep_num=%d"
#define LCD_SC8800H_2858_112_2_18_1_45_23_114 "----LCD_UpdateTiming-----"
#define LCD_SC8800H_2864_112_2_18_1_45_24_115 "---LCD_UpdateTiming---: AHB Clock:%d"
#define LCD_SC8800H_2883_112_2_18_1_45_24_116 "LCD_UpdateTiming:get semaphore 1"
#define LCD_SC8800H_2934_112_2_18_1_45_24_117 "LCD_UpdateTiming:put semaphore 1"
#define LCD_TASK_99_112_2_18_1_45_26_118 "lcd_task : LCD_WaitAndClearDoneStatus() "
#define LCD_TASK_111_112_2_18_1_45_26_119 "lcd_task : LCD_WaitBlendDoneStatus() "
#define LCD_TASK_133_112_2_18_1_45_26_120 "lcd_task:Get s_lcd_api_sm_ptr Semaphore Fail!--"
#define LCD_TASK_135_112_2_18_1_45_26_121 "lcd_task : LCD_GetAPISemaphore() "
#define LCD_TASK_154_112_2_18_1_45_27_122 "lcd_task:Put s_lcd_api_sm_ptr Semaphore Fail!--"
#define LCD_TASK_156_112_2_18_1_45_27_123 "lcd_task : LCD_PutAPISemaphore() "
#define LCD_TASK_174_112_2_18_1_45_27_124 "lcd_task:Get lcd_task_sm_ptr Semaphore Fail!--"
#define LCD_TASK_176_112_2_18_1_45_27_125 "lcd_task: LCD_GetTaskSemaphore() "
#define LCD_TASK_196_112_2_18_1_45_27_126 "lcd_task:Put lcd_task_sm_ptr  Semaphore Fail!--"
#define LCD_TASK_198_112_2_18_1_45_27_127 "lcd_task: LCD_PutTaskSemaphore() "
#define LCD_TASK_247_112_2_18_1_45_27_128 "lcd_task, lcd_Task_Open, task_id = %d"
#define LCD_TASK_251_112_2_18_1_45_27_129 "lcd_task, LCD TASK is existed,task_id = %d"
#define LCDC_406_112_2_18_1_45_28_130 "---------LCDC MCU INT-------"
#define LCDC_418_112_2_18_1_45_28_131 "---------LCDC TV INT-------"
#define LCDC_537_112_2_18_1_45_29_132 " LCDC_Init() !!"
#define LCDC_549_112_2_18_1_45_29_133 " the lcdc req has exist in the pll req lest already!"
#define LCDC_684_112_2_18_1_45_29_134 "Get LCDC Clock is %d!"
#define LCDC_1270_112_2_18_1_45_30_135 "----Don't permint close lcdc----"
#define LCDC_1278_112_2_18_1_45_30_136 "----Don't permint open lcdc again----"
#define LCDC_1290_112_2_18_1_45_30_137 " LCDC_Close(), close lcdc !"
#define LCDC_1305_112_2_18_1_45_30_138 " the lcdc req not in the pll req list !"
#define LCDC_1319_112_2_18_1_45_30_139 " LCDC_Close(), open lcdc !"
#define LCDC_1331_112_2_18_1_45_30_140 " the lcdc req has exist in the pll req lest already!"
#define LCDC_1353_112_2_18_1_45_30_141 " the lcdc req has exist in the pll req lest already!"
#define LCDC_1575_112_2_18_1_45_31_142 "*********Enter into ISP_config()*********"
#define LCDC_APP_111_112_2_18_1_45_32_143 "LCDC_APP:_LCDC_ISR1_LcmEof\r\n"
#define LCDC_APP_118_112_2_18_1_45_32_144 "LCDC_APP:_LCDC_ISR2_LcmSof\r\n"
#define LCDC_APP_129_112_2_18_1_45_32_145 "LCDC_APP:_LCDC_ISR3_Fmark\r\n"
#define LCDC_APP_156_112_2_18_1_45_32_146 "[_LCDC_GetDataCoeffParam ERROR],LCDC:img data format err format:%d"
#define LCDC_APP_196_112_2_18_1_45_32_147 "[_LCDC_GetAlginParam ERROR],LCDC:img data format err format:%d"
#define LCDC_APP_884_112_2_18_1_45_33_148 "_LCDC_SetCaptureParam:old addr = 0x%x"
#define LCDC_APP_903_112_2_18_1_45_34_149 "_LCDC_SetCaptureParam:new addr = 0x%x"
#define LCDC_APP_1129_112_2_18_1_45_34_150 "LCDC_AppConfigBlock, layer error, layer id: %d"
#define LCDC_DRV_496_112_2_18_1_45_36_151 "[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x"
#define LCDC_DRV_520_112_2_18_1_45_36_152 "[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x"
#define LCDC_DRV_575_112_2_18_1_45_36_153 "[LCDC_DrvSetEndian ERROR],layer %d, endian %d"
#define LCDC_DRV_621_112_2_18_1_45_36_154 "[LCDC_DrvSetDataFormat ERROR],layer %d, format %d"
#define LCDC_DRV_663_112_2_18_1_45_36_155 "[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d"
#define LCDC_DRV_717_112_2_18_1_45_36_156 "[LCDC_DrvSetPitch ERROR],layer %d, pitch %d"
#define LCDC_DRV_785_112_2_18_1_45_36_157 "[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)"
#define LCDC_DRV_831_112_2_18_1_45_37_158 "[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d"
#define LCDC_DRV_871_112_2_18_1_45_37_159 "[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d"
#define LCDC_DRV_911_112_2_18_1_45_37_160 "[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d"
#define LCDC_DRV_955_112_2_18_1_45_37_161 "[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d"
#define LCDC_DRV_993_112_2_18_1_45_37_162 "[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d"
#define LCDC_DRV_1058_112_2_18_1_45_37_163 "LCDC_DrvSetCapDataFormat:data format is error! format is %d"
#define LCDC_DRV_1205_112_2_18_1_45_37_164 "LCDC_DrvDisableLayer is error,layer_num=%d"
#define LCDC_DRV_1564_112_2_18_1_45_38_165 "LCDC_SetClock Fail,clock_type = %d"
#define LCDC_DRV_SC6530_370_112_2_18_1_45_39_166 "[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x"
#define LCDC_DRV_SC6530_391_112_2_18_1_45_39_167 "[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x"
#define LCDC_DRV_SC6530_433_112_2_18_1_45_39_168 "[LCDC_DrvSetEndian ERROR],layer %d, endian1 %d endian2 %d"
#define LCDC_DRV_SC6530_472_112_2_18_1_45_39_169 "[LCDC_DrvSetDataFormat ERROR],layer %d, format %d"
#define LCDC_DRV_SC6530_510_112_2_18_1_45_39_170 "[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d"
#define LCDC_DRV_SC6530_558_112_2_18_1_45_39_171 "[LCDC_DrvSetPitch ERROR],layer %d, pitch %d"
#define LCDC_DRV_SC6530_612_112_2_18_1_45_39_172 "[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)"
#define LCDC_DRV_SC6530_645_112_2_18_1_45_40_173 "[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d"
#define LCDC_DRV_SC6530_676_112_2_18_1_45_40_174 "[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d"
#define LCDC_DRV_SC6530_707_112_2_18_1_45_40_175 "[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d"
#define LCDC_DRV_SC6530_740_112_2_18_1_45_40_176 "[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d"
#define LCDC_DRV_SC6530_771_112_2_18_1_45_40_177 "[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d"
#define LCDC_DRV_SC6530_835_112_2_18_1_45_40_178 "LCDC_DrvSetCapDataFormat:data format is error! format is %d"
#define LCDC_DRV_SC6530_967_112_2_18_1_45_40_179 "LCDC_DrvDisableLayer is error,layer_num=%d"
#define LCDC_MCU_326_112_2_18_1_45_41_180 "lcdc_mcu_transcmd lcd_id>1"
#define LCDGUI_CTRL_55_112_2_18_1_45_43_181 "DISPLAY_ConvertCoordinate:logic_angle=%d,%d,%d,left=%d,top=%d,right=%d,bottom=%d"
#define LCDGUI_CTRL_206_112_2_18_1_45_43_182 "DISPLAY_ConvertCoordinate:out:angle=%d,left=%d,top=%d,right=%d,bottom=%d"
#define LCDGUI_CTRL_315_112_2_18_1_45_43_183 "DISPLAY_InvalidateRect:rect:%d,%d,%d,%d"
#define LCDREF_CTRL_90_112_2_18_1_45_44_184 "LCD_SetDisplayBuffer:lcd_id=%d,sum=%d "
#define LCDREF_CTRL_108_112_2_18_1_45_44_185 "LCD_SetDisplayBuffer:buf_array is NULL "
#define LCDREF_CTRL_144_112_2_18_1_45_44_186 "--LCDGUI:Get s_lcd_ctrl_sm_ptr Semaphore Fail!--"
#define LCDREF_CTRL_181_112_2_18_1_45_44_187 "--LCDGUI:Put s_lcd_ctrl_sm_ptr Semaphore Fail!--"
#define LCDREF_CTRL_213_112_2_18_1_45_44_188 "LCD_InvalidateRectEx:[error] dst_trim_rect.top=%d,dst_trim_rect.left=%d"
#define LCDREF_CTRL_288_112_2_18_1_45_44_189 "[DISPLAY] Free memory: addr: 0x%x "
#define OSD_RGB888_BLENDER_81_112_2_18_1_45_45_190 "OSD_Blend, _blend_layer, enter: %d"
#define OSD_RGB888_BLENDER_99_112_2_18_1_45_45_191 "OSD_Blend: Invalid rectangle happeps"
#define OSD_RGB888_BLENDER_104_112_2_18_1_45_45_192 "OSD_Blend: Invalid rectangle happeps"
#define OSD_RGB888_BLENDER_121_112_2_18_1_45_45_193 "OSD_Blend, _blend_layer ARGB888 width: %d, height:%d"
#define OSD_RGB888_BLENDER_148_112_2_18_1_45_45_194 "OSD_Blend, _blend_layer ARGB565 width: %d, height:%d"
#define OSD_RGB888_BLENDER_204_112_2_18_1_45_45_195 "OSD_Blend, _blend_layer, leave: %d"
#define OSD_RGB888_BLENDER_229_112_2_18_1_45_45_196 "_blend_frame() error i = 0"
#define OSD_RGB888_BLENDER_230_112_2_18_1_45_45_197 "_blend_frame() error x = %d, y = %d, width = %d, height = %d"
#define OSD_RGB888_BLENDER_231_112_2_18_1_45_45_198 "_blend_frame() error rect(%d,%d,%d,%d)"
#define OSD_RGB888_BLENDER_241_112_2_18_1_45_45_199 "_blend_frame() error i = %d"
#define OSD_RGB888_BLENDER_242_112_2_18_1_45_45_200 "_blend_frame() error x = %d, y = %d, width = %d, height = %d"
#define OSD_RGB888_BLENDER_243_112_2_18_1_45_45_201 "_blend_frame() error rect(%d,%d,%d,%d)"
#define OSD_RGB888_BLENDER_433_112_2_18_1_45_46_202 "OSD_Blend, num:%d, enter: %d"
#define OSD_RGB888_BLENDER_445_112_2_18_1_45_46_203 "OSD_Blend, leave: %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_LCD_TRC)
TRACE_MSG(LAYER_BLEND_337_112_2_18_1_44_52_0,"[_SoftLayerBlend ERROR 0],dont supoort!")
TRACE_MSG(LAYER_BLEND_356_112_2_18_1_44_52_1,"[_SoftLayerBlend ERROR 2],dont supoort!")
TRACE_MSG(LAYER_BLEND_431_112_2_18_1_44_52_2,"[_SoftLayerBlend ERROR 0],dont supoort!")
TRACE_MSG(LAYER_BLEND_448_112_2_18_1_44_52_3,"[_SoftLayerBlend ERROR 1],dont supoort!")
TRACE_MSG(LAYER_BLEND_476_112_2_18_1_44_52_4,"[_SoftLayerBlend ERROR 2],dont supoort!")
TRACE_MSG(LAYER_BLEND_575_112_2_18_1_44_53_5,"[_SoftLayerConvert ERROR],don't support RGB666!")
TRACE_MSG(LAYER_BLEND_609_112_2_18_1_44_53_6,"[_SoftLayerConvert ERROR],don't support RGB888!")
TRACE_MSG(LAYER_BLEND_614_112_2_18_1_44_53_7,"[_SoftLayerConvert ERROR],don't support output RGB666!")
TRACE_MSG(LAYER_BLEND_620_112_2_18_1_44_53_8,"[_SoftLayerConvert ERROR],don't support RGB666 convertion!")
TRACE_MSG(LAYER_BLEND_687_112_2_18_1_44_53_9,"[_SoftLayerConvert ERROR],don't support RGB666 convertion!")
TRACE_MSG(LAYER_BLEND_751_112_2_18_1_44_53_10,"---_SoftInvalidateRectUsingLayer:malloc fial!---")
TRACE_MSG(LAYER_BLEND_970_112_2_18_1_44_53_11,"covert_to_byte_num:result is 0,data_format is %d!")
TRACE_MSG(LAYER_BLEND_1058_112_2_18_1_44_54_12,"[_ConfigLayer ERROR],layer position!")
TRACE_MSG(LAYER_BLEND_1128_112_2_18_1_44_54_13,"_cap_data_type_convert:default rgb565,type=%d")
TRACE_MSG(LAYER_BLEND_1233_112_2_18_1_44_54_14,"_HW_InvalidateRectUsingLayer:layer_num=%d")
TRACE_MSG(LAYER_BLEND_1414_112_2_18_1_44_54_15,"--DISPLAY_BlendLayer:--array_size=%d--")
TRACE_MSG(LAYER_BLEND_1483_112_2_18_1_44_55_16,"robert _SoftwareHandle  OSD_LAYER_BLEND")
TRACE_MSG(LAYER_BLEND_1485_112_2_18_1_44_55_17,"robert _SoftwareHandle not OSD_LAYER_BLEND")
TRACE_MSG(LAYER_BLEND_1522_112_2_18_1_44_55_18,"robert _HardwareHandle")
TRACE_MSG(LAYER_BLEND_1526_112_2_18_1_44_55_19,"--DISPLAY_BlendLayer :--array_size=%d--")
TRACE_MSG(LAYER_BLEND_1578_112_2_18_1_44_55_20,"_HardwareHandle,err_num=%d")
TRACE_MSG(LAYER_BLEND_1621_112_2_18_1_44_55_21,"_HW_InvalidateRectUsingLayer")
TRACE_MSG(LAYER_BLEND_1800_112_2_18_1_44_55_22,"DISPLAY_BlendLayer: Out")
TRACE_MSG(LCD_169_112_2_18_1_44_56_23,"_lcd_data_format_convert is error! format = %d")
TRACE_MSG(LCD_241_112_2_18_1_44_56_24,"LCD:Get Semaphore Fail!")
TRACE_MSG(LCD_301_112_2_18_1_44_56_25,"LCD ERR:s_lcddone_callback is NULL!")
TRACE_MSG(LCD_334_112_2_18_1_44_56_26,"[ProtectionOperation ERROR],lcd_id = %d")
TRACE_MSG(LCD_349_112_2_18_1_44_56_27,"ProtectionOperation:err!")
TRACE_MSG(LCD_358_112_2_18_1_44_56_28,"---ProtectionOperation: lcd operation is NUL!---")
TRACE_MSG(LCD_401_112_2_18_1_44_56_29,"LCD:_AdjustRect:left=%d,right=%d")
TRACE_MSG(LCD_820_112_2_18_1_44_57_30,"[LCD_GetInfo ERROR],lcd devsize is error! lcd_id=%d")
TRACE_MSG(LCD_999_112_2_18_1_44_57_31,"LCD_Invalidate:Check State")
TRACE_MSG(LCD_1914_112_2_18_1_44_59_32,"---LCD_UpdateTiming---: AHB Clock:%d")
TRACE_MSG(LCD_CTRL_COMMON_134_112_2_18_1_45_0_33,"[_software_rotation ERROR],angle = %d")
TRACE_MSG(LCD_CTRL_COMMON_198_112_2_18_1_45_1_34,"_rotation_by_hw:[ERROR]don't support format %d")
TRACE_MSG(LCD_CTRL_COMMON_218_112_2_18_1_45_1_35,"LCDGUI_CTRL:Rotation_AppStart failed!")
TRACE_MSG(LCD_CTRL_COMMON_243_112_2_18_1_45_1_36,"DISPLAY_ConvertCoordinate:rotation_angle=%d,left=%d,top=%d,right=%d,bottom=%d")
TRACE_MSG(LCD_CTRL_COMMON_259_112_2_18_1_45_1_37,"_convert_coordinate:[ERROR] physical rect is error!")
TRACE_MSG(LCD_CTRL_COMMON_334_112_2_18_1_45_1_38,"[LCDCTRL:]_convert_coordinate,rotaion_angle ERROR!")
TRACE_MSG(LCD_CTRL_COMMON_341_112_2_18_1_45_1_39,"DISPLAY_ConvertCoordinate:out:angle=%d,left=%d,top=%d,right=%d,bottom=%d")
TRACE_MSG(LCD_CTRL_COMMON_375_112_2_18_1_45_1_40,"_software_rotation_halfword:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_391_112_2_18_1_45_1_41,"_software_rotation_halfword:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_456_112_2_18_1_45_1_42,"_software_rotation_halfword:[ERROR]param is error! %d")
TRACE_MSG(LCD_CTRL_COMMON_490_112_2_18_1_45_1_43,"_software_rotation_halfword:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_506_112_2_18_1_45_1_44,"_software_rotation_halfword:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_571_112_2_18_1_45_1_45,"_software_rotation_word:[ERROR]param is error! %d")
TRACE_MSG(LCD_CTRL_COMMON_605_112_2_18_1_45_1_46,"_software_rotation_halfword:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_621_112_2_18_1_45_1_47,"_software_rotation_halfword:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_686_112_2_18_1_45_2_48,"_software_rotation_byte:[ERROR]param is error! %d")
TRACE_MSG(LCD_CTRL_COMMON_724_112_2_18_1_45_2_49,"_software_rotation_with_crop:[ERROR]don't support format %d")
TRACE_MSG(LCD_CTRL_COMMON_755_112_2_18_1_45_2_50,"_software_rotation_halfword:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_771_112_2_18_1_45_2_51,"[LCDCTRL:]_software_rotation_with_yuv,coordinate convert error!")
TRACE_MSG(LCD_CTRL_COMMON_795_112_2_18_1_45_2_52,"[LCDCTRL:]_software_rotation_with_yuv,alloc fail!")
TRACE_MSG(LCD_CTRL_COMMON_799_112_2_18_1_45_2_53,"[LCDCTRL:]_software_rotation_with_yuv,malloc addr=0x%x,size=%d")
TRACE_MSG(LCD_CTRL_COMMON_875_112_2_18_1_45_2_54,"[LCDCTRL:],_software_rotation_with_yuv,convert ERROR!")
TRACE_MSG(LCD_CTRL_COMMON_913_112_2_18_1_45_2_55,"_display_software_rotation:full copy")
TRACE_MSG(LCD_CTRL_COMMON_926_112_2_18_1_45_2_56,"_display_software_rotation:crop copy")
TRACE_MSG(LCD_CTRL_COMMON_951_112_2_18_1_45_2_57,"convert_angle: default!")
TRACE_MSG(LCD_CTRL_COMMON_980_112_2_18_1_45_2_58,"convert_angle: default!")
TRACE_MSG(LCD_CTRL_COMMON_1009_112_2_18_1_45_2_59,"LCDGUI_Ctrl:_invalidate_on_nor:dc mode")
TRACE_MSG(LCD_CTRL_COMMON_1060_112_2_18_1_45_2_60,"_blend_and_rotate:param_ptr->rot_angle=%d,layer_num=%d")
TRACE_MSG(LCD_CTRL_COMMON_1081_112_2_18_1_45_2_61,"[_blend_and_rotate ERROR]:angle is error!")
TRACE_MSG(LCD_CTRL_COMMON_1176_112_2_18_1_45_3_62,"_display_software_rotation:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_1282_112_2_18_1_45_3_63,"dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d")
TRACE_MSG(LCD_CTRL_COMMON_1344_112_2_18_1_45_3_64,"dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d")
TRACE_MSG(LCD_CTRL_COMMON_1350_112_2_18_1_45_3_65,"[lcd_ctrl_common.c]:OSD copy don't support!")
TRACE_MSG(LCD_CTRL_COMMON_1383_112_2_18_1_45_3_66,"param_ptr->rot_angle=%d,size=%d,layer_num=%d")
TRACE_MSG(LCD_CTRL_COMMON_1407_112_2_18_1_45_3_67,"[DISPLAY] can alloc memory: addr: 0x%x, layer num: %d, total_size: %d, size: %d, type:%d ")
TRACE_MSG(LCD_CTRL_COMMON_1408_112_2_18_1_45_3_68,"[DISPLAY] lcd width: %d , lcd height %d ")
TRACE_MSG(LCD_CTRL_COMMON_1413_112_2_18_1_45_3_69,"[DISPLAY] can not alloc memory: layer num: %d, total_size: %d, size: %d, type:%d ")
TRACE_MSG(LCD_CTRL_COMMON_1414_112_2_18_1_45_3_70,"[DISPLAY] lcd width: %d , lcd height %d ")
TRACE_MSG(LCD_CTRL_COMMON_1437_112_2_18_1_45_3_71,"[_invalidate_on_nand:ERROR],_blend_and_rotate error!")
TRACE_MSG(LCD_CTRL_COMMON_1458_112_2_18_1_45_3_72,"dst_ptr=0x%x,src_ptr=0x%x")
TRACE_MSG(LCD_CTRL_COMMON_1463_112_2_18_1_45_3_73,"_display_software_rotation:[ERROR]")
TRACE_MSG(LCD_CTRL_COMMON_1481_112_2_18_1_45_3_74,"dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d")
TRACE_MSG(LCD_CTRL_COMMON_1616_112_2_18_1_45_3_75,"param_ptr->rot_angle=%d,size=%d")
TRACE_MSG(LCD_CTRL_COMMON_1630_112_2_18_1_45_3_76,"dst_ptr=0x%x,src_ptr=0x%x")
TRACE_MSG(LCD_CTRL_COMMON_1640_112_2_18_1_45_4_77,"blk_id=%d,time=%d")
TRACE_MSG(LCD_CTRL_COMMON_1647_112_2_18_1_45_4_78,"_invalidate_on_nand: break,layer_num=%d,i=%d")
TRACE_MSG(LCD_CTRL_COMMON_1786_112_2_18_1_45_4_79,"[_convert_cfg ERROR],display data type is error!")
TRACE_MSG(LCD_CTRL_COMMON_1906_112_2_18_1_45_4_80,"covert_to_byte_num:result is 0,data_format is %d!")
TRACE_MSG(LCD_CTRL_COMMON_1928_112_2_18_1_45_4_81,"_cap_data_type_convert:default rgb565,type=%d")
TRACE_MSG(LCD_CTRL_COMMON_1943_112_2_18_1_45_4_82,"_invalidate_on_nand:%d,%d,0x%x")
TRACE_MSG(LCD_CTRL_COMMON_1946_112_2_18_1_45_4_83,"_invalidate_on_nor:%d,%d")
TRACE_MSG(LCD_FOR_FOTA_206_112_2_18_1_45_5_84,"ProtectionOperation:err!")
TRACE_MSG(LCD_FOR_FOTA_633_112_2_18_1_45_6_85,"check lcdc state!")
TRACE_MSG(LCD_FOR_FOTA_863_112_2_18_1_45_7_86,"LCD:Get Semaphore Fail!")
TRACE_MSG(LCD_FOR_FOTA_962_112_2_18_1_45_7_87,"LCD_SetBrushMode Enter!mode:%d")
TRACE_MSG(LCD_FOR_FOTA_1037_112_2_18_1_45_7_88,"LCD_SetBrushMode Exit!")
TRACE_MSG(LCD_FOR_FOTA_1596_112_2_18_1_45_8_89,"----LCD_UpdateTiming-----")
TRACE_MSG(LCD_FOR_FOTA_1602_112_2_18_1_45_8_90,"---LCD_UpdateTiming---: AHB Clock:%d")
TRACE_MSG(LCD_FOR_FOTA_1621_112_2_18_1_45_8_91,"LCD_UpdateTiming:get semaphore 1")
TRACE_MSG(LCD_FOR_FOTA_1672_112_2_18_1_45_8_92,"LCD_UpdateTiming:put semaphore 1")
TRACE_MSG(LCD_FOTA_123_112_2_18_1_45_10_93,"_lcd_data_format_convert is error! format = %d")
TRACE_MSG(LCD_FOTA_164_112_2_18_1_45_10_94,"ProtectionOperation:err!")
TRACE_MSG(LCD_FOTA_171_112_2_18_1_45_10_95,"---ProtectionOperation: lcd operation is NUL!---")
TRACE_MSG(LCD_FOTA_284_112_2_18_1_45_10_96,"LCD:Get Semaphore Fail!")
TRACE_MSG(LCD_FOTA_634_112_2_18_1_45_11_97,"LCD_Invalidate:Check State")
TRACE_MSG(LCD_FOTA_1190_112_2_18_1_45_12_98,"---LCD_UpdateTiming---: AHB Clock:%d")
TRACE_MSG(LCD_INTERFACE_352_112_2_18_1_45_13_99,"DISPLAY_CopyToAuxMemory: invalid data type")
TRACE_MSG(LCD_INTERFACE_503_112_2_18_1_45_13_100,"lcd_task blend: LCD_UPDATE_MODE_SYNC ")
TRACE_MSG(LCD_INTERFACE_515_112_2_18_1_45_13_101,"lcd_task invalidate: LCD_UPDATE_MODE_SYNC ")
TRACE_MSG(LCD_INTERFACE_532_112_2_18_1_45_13_102,"lcd_task blend: LCD_UPDATE_MODE_SYNC ")
TRACE_MSG(LCD_SC6600L_364_112_2_18_1_45_14_103,"LCD_Close() in tv on mode")
TRACE_MSG(LCD_SC6600L_1615_112_2_18_1_45_17_104,"LCD:Get Semaphore Fail!")
TRACE_MSG(LCD_SC6600L_1678_112_2_18_1_45_17_105,"LCD_SetBrushMode Enter!mode:%d")
TRACE_MSG(LCD_SC8800H_259_112_2_18_1_45_18_106,"ProtectionOperation:err!")
TRACE_MSG(LCD_SC8800H_722_112_2_18_1_45_19_107,"check lcdc state!")
TRACE_MSG(LCD_SC8800H_1031_112_2_18_1_45_20_108,"LCD:Get Semaphore Fail!")
TRACE_MSG(LCD_SC8800H_1127_112_2_18_1_45_20_109,"set event")
TRACE_MSG(LCD_SC8800H_1186_112_2_18_1_45_20_110,"LCD ERR:s_lcddone_callback is NULL!")
TRACE_MSG(LCD_SC8800H_1238_112_2_18_1_45_20_111,"LCD_SetBrushMode Enter!mode:%d")
TRACE_MSG(LCD_SC8800H_1338_112_2_18_1_45_20_112,"LCD_SetBrushMode Exit!")
TRACE_MSG(LCD_SC8800H_2458_112_2_18_1_45_23_113,"---lcdc_num=%d,lcd_sleep_num=%d")
TRACE_MSG(LCD_SC8800H_2858_112_2_18_1_45_23_114,"----LCD_UpdateTiming-----")
TRACE_MSG(LCD_SC8800H_2864_112_2_18_1_45_24_115,"---LCD_UpdateTiming---: AHB Clock:%d")
TRACE_MSG(LCD_SC8800H_2883_112_2_18_1_45_24_116,"LCD_UpdateTiming:get semaphore 1")
TRACE_MSG(LCD_SC8800H_2934_112_2_18_1_45_24_117,"LCD_UpdateTiming:put semaphore 1")
TRACE_MSG(LCD_TASK_99_112_2_18_1_45_26_118,"lcd_task : LCD_WaitAndClearDoneStatus() ")
TRACE_MSG(LCD_TASK_111_112_2_18_1_45_26_119,"lcd_task : LCD_WaitBlendDoneStatus() ")
TRACE_MSG(LCD_TASK_133_112_2_18_1_45_26_120,"lcd_task:Get s_lcd_api_sm_ptr Semaphore Fail!--")
TRACE_MSG(LCD_TASK_135_112_2_18_1_45_26_121,"lcd_task : LCD_GetAPISemaphore() ")
TRACE_MSG(LCD_TASK_154_112_2_18_1_45_27_122,"lcd_task:Put s_lcd_api_sm_ptr Semaphore Fail!--")
TRACE_MSG(LCD_TASK_156_112_2_18_1_45_27_123,"lcd_task : LCD_PutAPISemaphore() ")
TRACE_MSG(LCD_TASK_174_112_2_18_1_45_27_124,"lcd_task:Get lcd_task_sm_ptr Semaphore Fail!--")
TRACE_MSG(LCD_TASK_176_112_2_18_1_45_27_125,"lcd_task: LCD_GetTaskSemaphore() ")
TRACE_MSG(LCD_TASK_196_112_2_18_1_45_27_126,"lcd_task:Put lcd_task_sm_ptr  Semaphore Fail!--")
TRACE_MSG(LCD_TASK_198_112_2_18_1_45_27_127,"lcd_task: LCD_PutTaskSemaphore() ")
TRACE_MSG(LCD_TASK_247_112_2_18_1_45_27_128,"lcd_task, lcd_Task_Open, task_id = %d")
TRACE_MSG(LCD_TASK_251_112_2_18_1_45_27_129,"lcd_task, LCD TASK is existed,task_id = %d")
TRACE_MSG(LCDC_406_112_2_18_1_45_28_130,"---------LCDC MCU INT-------")
TRACE_MSG(LCDC_418_112_2_18_1_45_28_131,"---------LCDC TV INT-------")
TRACE_MSG(LCDC_537_112_2_18_1_45_29_132," LCDC_Init() !!")
TRACE_MSG(LCDC_549_112_2_18_1_45_29_133," the lcdc req has exist in the pll req lest already!")
TRACE_MSG(LCDC_684_112_2_18_1_45_29_134,"Get LCDC Clock is %d!")
TRACE_MSG(LCDC_1270_112_2_18_1_45_30_135,"----Don't permint close lcdc----")
TRACE_MSG(LCDC_1278_112_2_18_1_45_30_136,"----Don't permint open lcdc again----")
TRACE_MSG(LCDC_1290_112_2_18_1_45_30_137," LCDC_Close(), close lcdc !")
TRACE_MSG(LCDC_1305_112_2_18_1_45_30_138," the lcdc req not in the pll req list !")
TRACE_MSG(LCDC_1319_112_2_18_1_45_30_139," LCDC_Close(), open lcdc !")
TRACE_MSG(LCDC_1331_112_2_18_1_45_30_140," the lcdc req has exist in the pll req lest already!")
TRACE_MSG(LCDC_1353_112_2_18_1_45_30_141," the lcdc req has exist in the pll req lest already!")
TRACE_MSG(LCDC_1575_112_2_18_1_45_31_142,"*********Enter into ISP_config()*********")
TRACE_MSG(LCDC_APP_111_112_2_18_1_45_32_143,"LCDC_APP:_LCDC_ISR1_LcmEof\r\n")
TRACE_MSG(LCDC_APP_118_112_2_18_1_45_32_144,"LCDC_APP:_LCDC_ISR2_LcmSof\r\n")
TRACE_MSG(LCDC_APP_129_112_2_18_1_45_32_145,"LCDC_APP:_LCDC_ISR3_Fmark\r\n")
TRACE_MSG(LCDC_APP_156_112_2_18_1_45_32_146,"[_LCDC_GetDataCoeffParam ERROR],LCDC:img data format err format:%d")
TRACE_MSG(LCDC_APP_196_112_2_18_1_45_32_147,"[_LCDC_GetAlginParam ERROR],LCDC:img data format err format:%d")
TRACE_MSG(LCDC_APP_884_112_2_18_1_45_33_148,"_LCDC_SetCaptureParam:old addr = 0x%x")
TRACE_MSG(LCDC_APP_903_112_2_18_1_45_34_149,"_LCDC_SetCaptureParam:new addr = 0x%x")
TRACE_MSG(LCDC_APP_1129_112_2_18_1_45_34_150,"LCDC_AppConfigBlock, layer error, layer id: %d")
TRACE_MSG(LCDC_DRV_496_112_2_18_1_45_36_151,"[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x")
TRACE_MSG(LCDC_DRV_520_112_2_18_1_45_36_152,"[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x")
TRACE_MSG(LCDC_DRV_575_112_2_18_1_45_36_153,"[LCDC_DrvSetEndian ERROR],layer %d, endian %d")
TRACE_MSG(LCDC_DRV_621_112_2_18_1_45_36_154,"[LCDC_DrvSetDataFormat ERROR],layer %d, format %d")
TRACE_MSG(LCDC_DRV_663_112_2_18_1_45_36_155,"[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d")
TRACE_MSG(LCDC_DRV_717_112_2_18_1_45_36_156,"[LCDC_DrvSetPitch ERROR],layer %d, pitch %d")
TRACE_MSG(LCDC_DRV_785_112_2_18_1_45_36_157,"[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)")
TRACE_MSG(LCDC_DRV_831_112_2_18_1_45_37_158,"[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d")
TRACE_MSG(LCDC_DRV_871_112_2_18_1_45_37_159,"[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d")
TRACE_MSG(LCDC_DRV_911_112_2_18_1_45_37_160,"[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d")
TRACE_MSG(LCDC_DRV_955_112_2_18_1_45_37_161,"[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d")
TRACE_MSG(LCDC_DRV_993_112_2_18_1_45_37_162,"[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d")
TRACE_MSG(LCDC_DRV_1058_112_2_18_1_45_37_163,"LCDC_DrvSetCapDataFormat:data format is error! format is %d")
TRACE_MSG(LCDC_DRV_1205_112_2_18_1_45_37_164,"LCDC_DrvDisableLayer is error,layer_num=%d")
TRACE_MSG(LCDC_DRV_1564_112_2_18_1_45_38_165,"LCDC_SetClock Fail,clock_type = %d")
TRACE_MSG(LCDC_DRV_SC6530_370_112_2_18_1_45_39_166,"[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x")
TRACE_MSG(LCDC_DRV_SC6530_391_112_2_18_1_45_39_167,"[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x")
TRACE_MSG(LCDC_DRV_SC6530_433_112_2_18_1_45_39_168,"[LCDC_DrvSetEndian ERROR],layer %d, endian1 %d endian2 %d")
TRACE_MSG(LCDC_DRV_SC6530_472_112_2_18_1_45_39_169,"[LCDC_DrvSetDataFormat ERROR],layer %d, format %d")
TRACE_MSG(LCDC_DRV_SC6530_510_112_2_18_1_45_39_170,"[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d")
TRACE_MSG(LCDC_DRV_SC6530_558_112_2_18_1_45_39_171,"[LCDC_DrvSetPitch ERROR],layer %d, pitch %d")
TRACE_MSG(LCDC_DRV_SC6530_612_112_2_18_1_45_39_172,"[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)")
TRACE_MSG(LCDC_DRV_SC6530_645_112_2_18_1_45_40_173,"[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d")
TRACE_MSG(LCDC_DRV_SC6530_676_112_2_18_1_45_40_174,"[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d")
TRACE_MSG(LCDC_DRV_SC6530_707_112_2_18_1_45_40_175,"[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d")
TRACE_MSG(LCDC_DRV_SC6530_740_112_2_18_1_45_40_176,"[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d")
TRACE_MSG(LCDC_DRV_SC6530_771_112_2_18_1_45_40_177,"[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d")
TRACE_MSG(LCDC_DRV_SC6530_835_112_2_18_1_45_40_178,"LCDC_DrvSetCapDataFormat:data format is error! format is %d")
TRACE_MSG(LCDC_DRV_SC6530_967_112_2_18_1_45_40_179,"LCDC_DrvDisableLayer is error,layer_num=%d")
TRACE_MSG(LCDC_MCU_326_112_2_18_1_45_41_180,"lcdc_mcu_transcmd lcd_id>1")
TRACE_MSG(LCDGUI_CTRL_55_112_2_18_1_45_43_181,"DISPLAY_ConvertCoordinate:logic_angle=%d,%d,%d,left=%d,top=%d,right=%d,bottom=%d")
TRACE_MSG(LCDGUI_CTRL_206_112_2_18_1_45_43_182,"DISPLAY_ConvertCoordinate:out:angle=%d,left=%d,top=%d,right=%d,bottom=%d")
TRACE_MSG(LCDGUI_CTRL_315_112_2_18_1_45_43_183,"DISPLAY_InvalidateRect:rect:%d,%d,%d,%d")
TRACE_MSG(LCDREF_CTRL_90_112_2_18_1_45_44_184,"LCD_SetDisplayBuffer:lcd_id=%d,sum=%d ")
TRACE_MSG(LCDREF_CTRL_108_112_2_18_1_45_44_185,"LCD_SetDisplayBuffer:buf_array is NULL ")
TRACE_MSG(LCDREF_CTRL_144_112_2_18_1_45_44_186,"--LCDGUI:Get s_lcd_ctrl_sm_ptr Semaphore Fail!--")
TRACE_MSG(LCDREF_CTRL_181_112_2_18_1_45_44_187,"--LCDGUI:Put s_lcd_ctrl_sm_ptr Semaphore Fail!--")
TRACE_MSG(LCDREF_CTRL_213_112_2_18_1_45_44_188,"LCD_InvalidateRectEx:[error] dst_trim_rect.top=%d,dst_trim_rect.left=%d")
TRACE_MSG(LCDREF_CTRL_288_112_2_18_1_45_44_189,"[DISPLAY] Free memory: addr: 0x%x ")
TRACE_MSG(OSD_RGB888_BLENDER_81_112_2_18_1_45_45_190,"OSD_Blend, _blend_layer, enter: %d")
TRACE_MSG(OSD_RGB888_BLENDER_99_112_2_18_1_45_45_191,"OSD_Blend: Invalid rectangle happeps")
TRACE_MSG(OSD_RGB888_BLENDER_104_112_2_18_1_45_45_192,"OSD_Blend: Invalid rectangle happeps")
TRACE_MSG(OSD_RGB888_BLENDER_121_112_2_18_1_45_45_193,"OSD_Blend, _blend_layer ARGB888 width: %d, height:%d")
TRACE_MSG(OSD_RGB888_BLENDER_148_112_2_18_1_45_45_194,"OSD_Blend, _blend_layer ARGB565 width: %d, height:%d")
TRACE_MSG(OSD_RGB888_BLENDER_204_112_2_18_1_45_45_195,"OSD_Blend, _blend_layer, leave: %d")
TRACE_MSG(OSD_RGB888_BLENDER_229_112_2_18_1_45_45_196,"_blend_frame() error i = 0")
TRACE_MSG(OSD_RGB888_BLENDER_230_112_2_18_1_45_45_197,"_blend_frame() error x = %d, y = %d, width = %d, height = %d")
TRACE_MSG(OSD_RGB888_BLENDER_231_112_2_18_1_45_45_198,"_blend_frame() error rect(%d,%d,%d,%d)")
TRACE_MSG(OSD_RGB888_BLENDER_241_112_2_18_1_45_45_199,"_blend_frame() error i = %d")
TRACE_MSG(OSD_RGB888_BLENDER_242_112_2_18_1_45_45_200,"_blend_frame() error x = %d, y = %d, width = %d, height = %d")
TRACE_MSG(OSD_RGB888_BLENDER_243_112_2_18_1_45_45_201,"_blend_frame() error rect(%d,%d,%d,%d)")
TRACE_MSG(OSD_RGB888_BLENDER_433_112_2_18_1_45_46_202,"OSD_Blend, num:%d, enter: %d")
TRACE_MSG(OSD_RGB888_BLENDER_445_112_2_18_1_45_46_203,"OSD_Blend, leave: %d")
END_TRACE_MAP(MS_REF_LCD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_LCD_TRC_H_

