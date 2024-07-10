/******************************************************************************
 ** File Name:      fdl_bootloader_trc.h                                         *
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
//trace_id:16
#ifndef _FDL_BOOTLOADER_TRC_H_
#define _FDL_BOOTLOADER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define NAND_CONTROLLER_161_112_2_18_1_0_7_0 "NFC timeout happened!"
#define NAND_CONTROLLER_881_112_2_18_1_0_8_1 "Set NandParameter Failed!\n\r"
#define NAND_CONTROLLER_1120_112_2_18_1_0_9_2 "NFC timeout happened!"
#define WATCHDOG_PHY_V0_41_112_2_18_1_0_26_3 "Watch Dog Trace: Watch Dog Value 0x%8.8x"
#define WATCHDOG_PHY_V0_78_112_2_18_1_0_26_4 "Watch Dog Trace: Watch Dog Control 0x%8.8x"
#define WATCHDOG_PHY_V0_79_112_2_18_1_0_26_5 "Watch Dog Trace: Watch Dog LOAD    0x%8.8x"
#define WATCHDOG_PHY_V3_98_112_2_18_1_0_27_6 "Watch Dog Trace: Watch Dog Control 0x%8.8x"
#define WATCHDOG_HAL_44_112_2_18_1_0_27_7 "Watch Dog Trace: WDG_TimerStop"
#define WATCHDOG_HAL_77_112_2_18_1_0_27_8 "Watch Dog Trace: WDG_TimerStart"
#define WATCHDOG_HAL_134_112_2_18_1_0_27_9 "Watch Dog Trace: WDG_ResetMCU"
#define WATCHDOG_HAL_150_112_2_18_1_0_27_10 "Watch Dog Trace: WDG_TimerLoad %d"
#define TF_DRV_NAND_176_112_2_18_1_1_28_11 "NFC timeout happened!"
#define TF_DRV_NAND_903_112_2_18_1_1_29_12 "Set NandParameter Failed!\n\r"
#define TF_DRV_NAND_1142_112_2_18_1_1_30_13 "NFC timeout happened!"
#define TF_LCD_175_112_2_18_1_1_34_14 "_lcd_data_format_convert is error! format = %d"
#define TF_LCD_247_112_2_18_1_1_34_15 "LCD:Get Semaphore Fail!"
#define TF_LCD_307_112_2_18_1_1_34_16 "LCD ERR:s_lcddone_callback is NULL!"
#define TF_LCD_340_112_2_18_1_1_34_17 "[ProtectionOperation ERROR],lcd_id = %d"
#define TF_LCD_355_112_2_18_1_1_34_18 "ProtectionOperation:err!"
#define TF_LCD_364_112_2_18_1_1_34_19 "---ProtectionOperation: lcd operation is NUL!---"
#define TF_LCD_817_112_2_18_1_1_35_20 "[LCD_GetInfo ERROR],lcd devsize is error! lcd_id=%d"
#define TF_LCD_912_112_2_18_1_1_35_21 "LCD_Invalidate:Check State"
#define TF_LCD_1793_112_2_18_1_1_37_22 "---LCD_UpdateTiming---: AHB Clock:%d"
#define TF_LCDC_APP_109_112_2_18_1_1_39_23 "LCDC_APP:_LCDC_ISR1_LcmEof\r\n"
#define TF_LCDC_APP_116_112_2_18_1_1_39_24 "LCDC_APP:_LCDC_ISR2_LcmSof\r\n"
#define TF_LCDC_APP_127_112_2_18_1_1_39_25 "LCDC_APP:_LCDC_ISR3_Fmark\r\n"
#define TF_LCDC_APP_154_112_2_18_1_1_39_26 "[_LCDC_GetDataCoeffParam ERROR],LCDC:img data format err format:%d"
#define TF_LCDC_APP_194_112_2_18_1_1_39_27 "[_LCDC_GetAlginParam ERROR],LCDC:img data format err format:%d"
#define TF_LCDC_APP_820_112_2_18_1_1_40_28 "LCDC_AppConfigBlock, layer error, layer id: %d"
#define TF_LCDC_DRV_470_112_2_18_1_1_42_29 "[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x"
#define TF_LCDC_DRV_494_112_2_18_1_1_42_30 "[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x"
#define TF_LCDC_DRV_549_112_2_18_1_1_42_31 "[LCDC_DrvSetEndian ERROR],layer %d, endian %d"
#define TF_LCDC_DRV_595_112_2_18_1_1_42_32 "[LCDC_DrvSetDataFormat ERROR],layer %d, format %d"
#define TF_LCDC_DRV_637_112_2_18_1_1_42_33 "[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d"
#define TF_LCDC_DRV_691_112_2_18_1_1_42_34 "[LCDC_DrvSetPitch ERROR],layer %d, pitch %d"
#define TF_LCDC_DRV_759_112_2_18_1_1_43_35 "[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)"
#define TF_LCDC_DRV_805_112_2_18_1_1_43_36 "[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d"
#define TF_LCDC_DRV_845_112_2_18_1_1_43_37 "[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d"
#define TF_LCDC_DRV_885_112_2_18_1_1_43_38 "[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d"
#define TF_LCDC_DRV_929_112_2_18_1_1_43_39 "[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d"
#define TF_LCDC_DRV_967_112_2_18_1_1_43_40 "[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d"
#define TF_LCDC_DRV_1032_112_2_18_1_1_43_41 "LCDC_DrvSetCapDataFormat:data format is error! format is %d"
#define TF_LCDC_DRV_1179_112_2_18_1_1_43_42 "LCDC_DrvDisableLayer is error,layer_num=%d"
#define TF_LCDC_DRV_1523_112_2_18_1_1_44_43 "LCDC_SetClock Fail,clock_type = %d"
#define TF_LCDC_DRV_SC6530_381_112_2_18_1_1_45_44 "[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x"
#define TF_LCDC_DRV_SC6530_402_112_2_18_1_1_45_45 "[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x"
#define TF_LCDC_DRV_SC6530_444_112_2_18_1_1_45_46 "[LCDC_DrvSetEndian ERROR],layer %d, endian1 %d endian2 %d"
#define TF_LCDC_DRV_SC6530_483_112_2_18_1_1_45_47 "[LCDC_DrvSetDataFormat ERROR],layer %d, format %d"
#define TF_LCDC_DRV_SC6530_521_112_2_18_1_1_45_48 "[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d"
#define TF_LCDC_DRV_SC6530_569_112_2_18_1_1_46_49 "[LCDC_DrvSetPitch ERROR],layer %d, pitch %d"
#define TF_LCDC_DRV_SC6530_623_112_2_18_1_1_46_50 "[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)"
#define TF_LCDC_DRV_SC6530_656_112_2_18_1_1_46_51 "[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d"
#define TF_LCDC_DRV_SC6530_687_112_2_18_1_1_46_52 "[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d"
#define TF_LCDC_DRV_SC6530_718_112_2_18_1_1_46_53 "[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d"
#define TF_LCDC_DRV_SC6530_751_112_2_18_1_1_46_54 "[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d"
#define TF_LCDC_DRV_SC6530_782_112_2_18_1_1_46_55 "[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d"
#define TF_LCDC_DRV_SC6530_846_112_2_18_1_1_46_56 "LCDC_DrvSetCapDataFormat:data format is error! format is %d"
#define TF_LCDC_DRV_SC6530_978_112_2_18_1_1_46_57 "LCDC_DrvDisableLayer is error,layer_num=%d"
#define TF_LCMPROD_124_112_2_18_1_1_48_58 "_LCM_ReadID: lcd_id=%d ,lcm_dev_id = 0x%x!"
#define TF_LCMPROD_200_112_2_18_1_1_48_59 "_LCM_Identify: already done!"
#define TF_LCMPROD_210_112_2_18_1_1_48_60 "TF_LcmIdentify:s_lcm_index[i=0x%x] =0x%x"
#define TF_LCMPROD_215_112_2_18_1_1_48_61 "TF_LcmIdentify: %d LCM used default config info"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(FDL_BOOTLOADER_TRC)
TRACE_MSG(NAND_CONTROLLER_161_112_2_18_1_0_7_0,"NFC timeout happened!")
TRACE_MSG(NAND_CONTROLLER_881_112_2_18_1_0_8_1,"Set NandParameter Failed!\n\r")
TRACE_MSG(NAND_CONTROLLER_1120_112_2_18_1_0_9_2,"NFC timeout happened!")
TRACE_MSG(WATCHDOG_PHY_V0_41_112_2_18_1_0_26_3,"Watch Dog Trace: Watch Dog Value 0x%8.8x")
TRACE_MSG(WATCHDOG_PHY_V0_78_112_2_18_1_0_26_4,"Watch Dog Trace: Watch Dog Control 0x%8.8x")
TRACE_MSG(WATCHDOG_PHY_V0_79_112_2_18_1_0_26_5,"Watch Dog Trace: Watch Dog LOAD    0x%8.8x")
TRACE_MSG(WATCHDOG_PHY_V3_98_112_2_18_1_0_27_6,"Watch Dog Trace: Watch Dog Control 0x%8.8x")
TRACE_MSG(WATCHDOG_HAL_44_112_2_18_1_0_27_7,"Watch Dog Trace: WDG_TimerStop")
TRACE_MSG(WATCHDOG_HAL_77_112_2_18_1_0_27_8,"Watch Dog Trace: WDG_TimerStart")
TRACE_MSG(WATCHDOG_HAL_134_112_2_18_1_0_27_9,"Watch Dog Trace: WDG_ResetMCU")
TRACE_MSG(WATCHDOG_HAL_150_112_2_18_1_0_27_10,"Watch Dog Trace: WDG_TimerLoad %d")
TRACE_MSG(TF_DRV_NAND_176_112_2_18_1_1_28_11,"NFC timeout happened!")
TRACE_MSG(TF_DRV_NAND_903_112_2_18_1_1_29_12,"Set NandParameter Failed!\n\r")
TRACE_MSG(TF_DRV_NAND_1142_112_2_18_1_1_30_13,"NFC timeout happened!")
TRACE_MSG(TF_LCD_175_112_2_18_1_1_34_14,"_lcd_data_format_convert is error! format = %d")
TRACE_MSG(TF_LCD_247_112_2_18_1_1_34_15,"LCD:Get Semaphore Fail!")
TRACE_MSG(TF_LCD_307_112_2_18_1_1_34_16,"LCD ERR:s_lcddone_callback is NULL!")
TRACE_MSG(TF_LCD_340_112_2_18_1_1_34_17,"[ProtectionOperation ERROR],lcd_id = %d")
TRACE_MSG(TF_LCD_355_112_2_18_1_1_34_18,"ProtectionOperation:err!")
TRACE_MSG(TF_LCD_364_112_2_18_1_1_34_19,"---ProtectionOperation: lcd operation is NUL!---")
TRACE_MSG(TF_LCD_817_112_2_18_1_1_35_20,"[LCD_GetInfo ERROR],lcd devsize is error! lcd_id=%d")
TRACE_MSG(TF_LCD_912_112_2_18_1_1_35_21,"LCD_Invalidate:Check State")
TRACE_MSG(TF_LCD_1793_112_2_18_1_1_37_22,"---LCD_UpdateTiming---: AHB Clock:%d")
TRACE_MSG(TF_LCDC_APP_109_112_2_18_1_1_39_23,"LCDC_APP:_LCDC_ISR1_LcmEof\r\n")
TRACE_MSG(TF_LCDC_APP_116_112_2_18_1_1_39_24,"LCDC_APP:_LCDC_ISR2_LcmSof\r\n")
TRACE_MSG(TF_LCDC_APP_127_112_2_18_1_1_39_25,"LCDC_APP:_LCDC_ISR3_Fmark\r\n")
TRACE_MSG(TF_LCDC_APP_154_112_2_18_1_1_39_26,"[_LCDC_GetDataCoeffParam ERROR],LCDC:img data format err format:%d")
TRACE_MSG(TF_LCDC_APP_194_112_2_18_1_1_39_27,"[_LCDC_GetAlginParam ERROR],LCDC:img data format err format:%d")
TRACE_MSG(TF_LCDC_APP_820_112_2_18_1_1_40_28,"LCDC_AppConfigBlock, layer error, layer id: %d")
TRACE_MSG(TF_LCDC_DRV_470_112_2_18_1_1_42_29,"[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x")
TRACE_MSG(TF_LCDC_DRV_494_112_2_18_1_1_42_30,"[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x")
TRACE_MSG(TF_LCDC_DRV_549_112_2_18_1_1_42_31,"[LCDC_DrvSetEndian ERROR],layer %d, endian %d")
TRACE_MSG(TF_LCDC_DRV_595_112_2_18_1_1_42_32,"[LCDC_DrvSetDataFormat ERROR],layer %d, format %d")
TRACE_MSG(TF_LCDC_DRV_637_112_2_18_1_1_42_33,"[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d")
TRACE_MSG(TF_LCDC_DRV_691_112_2_18_1_1_42_34,"[LCDC_DrvSetPitch ERROR],layer %d, pitch %d")
TRACE_MSG(TF_LCDC_DRV_759_112_2_18_1_1_43_35,"[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)")
TRACE_MSG(TF_LCDC_DRV_805_112_2_18_1_1_43_36,"[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d")
TRACE_MSG(TF_LCDC_DRV_845_112_2_18_1_1_43_37,"[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d")
TRACE_MSG(TF_LCDC_DRV_885_112_2_18_1_1_43_38,"[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d")
TRACE_MSG(TF_LCDC_DRV_929_112_2_18_1_1_43_39,"[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d")
TRACE_MSG(TF_LCDC_DRV_967_112_2_18_1_1_43_40,"[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d")
TRACE_MSG(TF_LCDC_DRV_1032_112_2_18_1_1_43_41,"LCDC_DrvSetCapDataFormat:data format is error! format is %d")
TRACE_MSG(TF_LCDC_DRV_1179_112_2_18_1_1_43_42,"LCDC_DrvDisableLayer is error,layer_num=%d")
TRACE_MSG(TF_LCDC_DRV_1523_112_2_18_1_1_44_43,"LCDC_SetClock Fail,clock_type = %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_381_112_2_18_1_1_45_44,"[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x")
TRACE_MSG(TF_LCDC_DRV_SC6530_402_112_2_18_1_1_45_45,"[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x")
TRACE_MSG(TF_LCDC_DRV_SC6530_444_112_2_18_1_1_45_46,"[LCDC_DrvSetEndian ERROR],layer %d, endian1 %d endian2 %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_483_112_2_18_1_1_45_47,"[LCDC_DrvSetDataFormat ERROR],layer %d, format %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_521_112_2_18_1_1_45_48,"[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_569_112_2_18_1_1_46_49,"[LCDC_DrvSetPitch ERROR],layer %d, pitch %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_623_112_2_18_1_1_46_50,"[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)")
TRACE_MSG(TF_LCDC_DRV_SC6530_656_112_2_18_1_1_46_51,"[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_687_112_2_18_1_1_46_52,"[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_718_112_2_18_1_1_46_53,"[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_751_112_2_18_1_1_46_54,"[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_782_112_2_18_1_1_46_55,"[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_846_112_2_18_1_1_46_56,"LCDC_DrvSetCapDataFormat:data format is error! format is %d")
TRACE_MSG(TF_LCDC_DRV_SC6530_978_112_2_18_1_1_46_57,"LCDC_DrvDisableLayer is error,layer_num=%d")
TRACE_MSG(TF_LCMPROD_124_112_2_18_1_1_48_58,"_LCM_ReadID: lcd_id=%d ,lcm_dev_id = 0x%x!")
TRACE_MSG(TF_LCMPROD_200_112_2_18_1_1_48_59,"_LCM_Identify: already done!")
TRACE_MSG(TF_LCMPROD_210_112_2_18_1_1_48_60,"TF_LcmIdentify:s_lcm_index[i=0x%x] =0x%x")
TRACE_MSG(TF_LCMPROD_215_112_2_18_1_1_48_61,"TF_LcmIdentify: %d LCM used default config info")
END_TRACE_MAP(FDL_BOOTLOADER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _FDL_BOOTLOADER_TRC_H_

