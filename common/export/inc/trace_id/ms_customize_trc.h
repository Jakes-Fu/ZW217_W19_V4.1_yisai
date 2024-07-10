/******************************************************************************
 ** File Name:      ms_customize_trc.h                                         *
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
//trace_id:12
#ifndef _MS_CUSTOMIZE_TRC_H_
#define _MS_CUSTOMIZE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define ADC_PARAMETER_81_112_2_18_0_26_13_0 "ENVIRONMENT temper_adcenv %d, Temper %d\r\n"
#define LCM_PROD_129_112_2_18_0_26_14_1 "ADC: %d[%d]"
#define LCM_PROD_135_112_2_18_0_26_14_2 "_LCM_GetAdcVal: ADC Val -> 0x%x"
#define LCM_PROD_187_112_2_18_0_26_14_3 "_LCM_AdcIdentify: index %d, adc %d, min %d, max %d"
#define LCM_PROD_194_112_2_18_0_26_14_4 "_LCM_AdcIdentify: Match succ!"
#define LCM_PROD_228_112_2_18_0_26_14_5 "_LCM_ReadID: lcd_id=%d ,lcm_dev_id = 0x%x!"
#define LCM_PROD_323_112_2_18_0_26_14_6 "_LCM_Identify: already done!"
#define LCM_PROD_354_112_2_18_0_26_14_7 "_LCM_Identify: %d LCM unused"
#define LCM_PROD_360_112_2_18_0_26_14_8 "_LCM_Identify: %d LCM used default config info"
#define PROD_PARAM_483_112_2_18_0_26_17_9 "[chr_drv] REF_GetAdcCalibrationParaEx adc_ptr alloc fail!!"
#define PROD_PARAM_534_112_2_18_0_26_17_10 "[chr_drv] PRODINIT: battery0 = 0x%X, battery1 = 0x%X, "
#define PROD_PARAM_584_112_2_18_0_26_17_11 "[chr_drv] PRODINIT2: battery0 = 0x%X, battery1 = 0x%X, "
#define PROD_PARAM_663_112_2_18_0_26_17_12 "[chr_drv] slope_m =%d,slope_aux =%d"
#define PROD_PARAM_722_112_2_18_0_26_17_13 "PROD_SetChargeNVParam need't update !"
#define PROD_PARAM_726_112_2_18_0_26_17_14 "PROD_SetChargeNVParam update point = 0x%x"
#define PROD_PARAM_800_112_2_18_0_26_17_15 "PROD_Init() Enter !"
#define PROD_PARAM_911_112_2_18_0_26_18_16 "PROD_Init() Exit !"
#define PROD_PARAM_963_112_2_18_0_26_18_17 "PROD_InitProdParam has been called !"
#define PROD_PARAM_1032_112_2_18_0_26_18_18 "PROD_InitProdParam Exit !"
#define PROD_PARAM_1122_112_2_18_0_26_18_19 "PROD_WriteAudioParam: mode_name = %s, is_flush = %d, start_time = %d"
#define PROD_PARAM_1158_112_2_18_0_26_18_20 "PROD_WriteAudioParam: Write to Cache, end time = %d"
#define PROD_PARAM_1196_112_2_18_0_26_18_21 "PROD_WriteAudioParam: Write to Nv,end time = %d"
#define PROD_PARAM_1415_112_2_18_0_26_19_22 "PROD_ReadEngModeTestResult: nv_len %d, buf_len %d"
#define PROD_PARAM_1442_112_2_18_0_26_19_23 "PROD_WriteEngModeTestResult: nv_len %d, buf_len %d"
#define SENSOR_PROD_80_112_2_18_0_26_19_24 "[sensor_prod.c:_check_msg: sensor_id error,sensor_id = %d]"
#define SENSOR_PROD_88_112_2_18_0_26_19_25 "[sensor_prod.c: sensor id error!]"
#define SENSOR_PROD_95_112_2_18_0_26_19_26 "[sensor_prod.c:_check_msg:set sensor info table fail]"
#define SENSOR_PROD_743_112_2_18_0_26_21_27 "[sensor_prod.c:SensorTool_Init ERROR!]"
#define SENSOR_PROD_767_112_2_18_0_26_21_28 "sensor_prod.c:_SensorTool_CmdRoutine:"
#define SENSOR_PROD_771_112_2_18_0_26_21_29 "[sensor_prod.c:_SensorTool_CmdRoutine:type error!,type = %d]"
#define SENSOR_PROD_777_112_2_18_0_26_21_30 "[sensor_prod.c:_SensorTool_CmdRoutine:Init faile!]"
#define SENSOR_PROD_813_112_2_18_0_26_21_31 "[sensor_prod.c:_SensorTool_CmdRoutine:TOOL_SENSOR_GET_SENSOR_SUM]"
#define SENSOR_PROD_817_112_2_18_0_26_21_32 "[sensor_prod.c:_SensorTool_CmdRoutine subtype error,subtype=%d]"
#define FM_CFG_94_112_2_18_0_26_25_33 "FM_GetCtrlPort %x"
#define GPIO_CFG_192_112_2_18_0_26_25_34 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_265_112_2_18_0_26_25_35 "GPIO_CustomizeInit() End !"
#define PROD_OUTPORT_70_112_2_18_0_26_31_36 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define PROD_OUTPORT_82_112_2_18_0_26_31_37 "Production test dummy function: Register_Current_Test_CmdRoutine"
#define SCM_CONFIG_145_112_2_18_0_26_32_38 "SCM: DIAG, slot_name = %d, msg = %d"
#define SCM_CONFIG_194_112_2_18_0_26_32_39 "SCM: ATest Mode slot_name not support!"
#define SCM_CONFIG_202_112_2_18_0_26_32_40 "SCM: ATest Mode MSG error!"
#define SCM_CONFIG_219_112_2_18_0_26_32_41 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_220_112_2_18_0_26_32_42 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_221_112_2_18_0_26_32_43 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_222_112_2_18_0_26_32_44 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_223_112_2_18_0_26_32_45 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_224_112_2_18_0_26_32_46 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_225_112_2_18_0_26_32_47 "[Init]SCM_APB_CLK_CFG0 = 0x%x"
#define SCM_CONFIG_226_112_2_18_0_26_32_48 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_254_112_2_18_0_26_32_49 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_259_112_2_18_0_26_32_50 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_296_112_2_18_0_26_32_51 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_322_112_2_18_0_26_32_52 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_330_112_2_18_0_26_32_53 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_353_112_2_18_0_26_32_54 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_361_112_2_18_0_26_32_55 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_633_112_2_18_0_26_33_56 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_659_112_2_18_0_26_33_57 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_670_112_2_18_0_26_33_58 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_671_112_2_18_0_26_33_59 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_672_112_2_18_0_26_33_60 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_673_112_2_18_0_26_33_61 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_674_112_2_18_0_26_33_62 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_675_112_2_18_0_26_33_63 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_676_112_2_18_0_26_33_64 "[ErrProc]SCM_APB_CLK_CFG0 = 0x%x"
#define SCM_CONFIG_677_112_2_18_0_26_33_65 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_681_112_2_18_0_26_33_66 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_882_112_2_18_0_26_33_67 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_888_112_2_18_0_26_33_68 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_957_112_2_18_0_26_34_69 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_962_112_2_18_0_26_34_70 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_1182_112_2_18_0_26_34_71 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_1208_112_2_18_0_26_34_72 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_1221_112_2_18_0_26_34_73 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1408_112_2_18_0_26_35_74 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_1414_112_2_18_0_26_35_75 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1526_112_2_18_0_26_35_76 "SCM Read fail"
#define SCM_CONFIG_1551_112_2_18_0_26_35_77 "SCM Write fail"
#define SCM_CONFIG_1574_112_2_18_0_26_35_78 "SCM Erase fail"
#define SCM_CONFIG_1590_112_2_18_0_26_35_79 "SCM GetCap fail"
#define SCM_CONFIG_1617_112_2_18_0_26_35_80 "SCM Read fail"
#define SCM_CONFIG_1642_112_2_18_0_26_35_81 "SCM Write fail"
#define SCM_CONFIG_1665_112_2_18_0_26_35_82 "SCM Erase fail"
#define SCM_CONFIG_1681_112_2_18_0_26_35_83 "SCM GetCap fail"
#define SCM_CONFIG_1693_112_2_18_0_26_35_84 "SCM_BSD_REG"
#define SCM_CONFIG_1699_112_2_18_0_26_35_85 "SCM slot0 status not normal! Error!"
#define SCM_CONFIG_1706_112_2_18_0_26_35_86 "SCM slot0 capacity is 0! Error!"
#define SCM_CONFIG_1733_112_2_18_0_26_35_87 "SCM slot1 status not normal! Error!"
#define SCM_CONFIG_1740_112_2_18_0_26_35_88 "SCM slot1 capacity is 0! Error!"
#define SCM_CONFIG_1765_112_2_18_0_26_35_89 "SCM slot name error!"
#define SCM_CONFIG_1774_112_2_18_0_26_35_90 "SCM_BSD_UNREG"
#define SCM_CONFIG_1794_112_2_18_0_26_35_91 "SCM slot name error!"
#define SENSOR_CFG_144_112_2_18_0_26_36_92 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define TP_MULTI_CFG_82_112_2_18_0_26_38_93 "TPM_GetCtrlPort %x"
#define FM_CFG_94_112_2_18_0_26_41_94 "FM_GetCtrlPort %x"
#define GPIO_CFG_189_112_2_18_0_26_42_95 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_262_112_2_18_0_26_42_96 "GPIO_CustomizeInit() End !"
#define PROD_OUTPORT_70_112_2_18_0_26_49_97 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define PROD_OUTPORT_82_112_2_18_0_26_49_98 "Production test dummy function: Register_Current_Test_CmdRoutine"
#define SCM_CONFIG_145_112_2_18_0_26_49_99 "SCM: DIAG, slot_name = %d, msg = %d"
#define SCM_CONFIG_194_112_2_18_0_26_49_100 "SCM: ATest Mode slot_name not support!"
#define SCM_CONFIG_202_112_2_18_0_26_49_101 "SCM: ATest Mode MSG error!"
#define SCM_CONFIG_219_112_2_18_0_26_49_102 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_220_112_2_18_0_26_49_103 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_221_112_2_18_0_26_49_104 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_222_112_2_18_0_26_49_105 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_223_112_2_18_0_26_49_106 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_224_112_2_18_0_26_49_107 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_225_112_2_18_0_26_49_108 "[Init]SCM_APB_CLK_CFG0 = 0x%x"
#define SCM_CONFIG_226_112_2_18_0_26_49_109 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_254_112_2_18_0_26_49_110 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_259_112_2_18_0_26_49_111 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_296_112_2_18_0_26_50_112 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_322_112_2_18_0_26_50_113 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_330_112_2_18_0_26_50_114 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_353_112_2_18_0_26_50_115 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_361_112_2_18_0_26_50_116 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_633_112_2_18_0_26_50_117 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_659_112_2_18_0_26_50_118 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_670_112_2_18_0_26_50_119 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_671_112_2_18_0_26_50_120 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_672_112_2_18_0_26_50_121 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_673_112_2_18_0_26_50_122 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_674_112_2_18_0_26_50_123 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_675_112_2_18_0_26_50_124 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_676_112_2_18_0_26_50_125 "[ErrProc]SCM_APB_CLK_CFG0 = 0x%x"
#define SCM_CONFIG_677_112_2_18_0_26_50_126 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_681_112_2_18_0_26_50_127 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_882_112_2_18_0_26_51_128 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_888_112_2_18_0_26_51_129 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_957_112_2_18_0_26_51_130 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_962_112_2_18_0_26_51_131 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_1182_112_2_18_0_26_52_132 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_1208_112_2_18_0_26_52_133 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_1221_112_2_18_0_26_52_134 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1408_112_2_18_0_26_52_135 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_1414_112_2_18_0_26_52_136 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1526_112_2_18_0_26_52_137 "SCM Read fail"
#define SCM_CONFIG_1551_112_2_18_0_26_52_138 "SCM Write fail"
#define SCM_CONFIG_1574_112_2_18_0_26_52_139 "SCM Erase fail"
#define SCM_CONFIG_1590_112_2_18_0_26_52_140 "SCM GetCap fail"
#define SCM_CONFIG_1617_112_2_18_0_26_52_141 "SCM Read fail"
#define SCM_CONFIG_1642_112_2_18_0_26_52_142 "SCM Write fail"
#define SCM_CONFIG_1665_112_2_18_0_26_52_143 "SCM Erase fail"
#define SCM_CONFIG_1681_112_2_18_0_26_52_144 "SCM GetCap fail"
#define SCM_CONFIG_1693_112_2_18_0_26_53_145 "SCM_BSD_REG"
#define SCM_CONFIG_1699_112_2_18_0_26_53_146 "SCM slot0 status not normal! Error!"
#define SCM_CONFIG_1706_112_2_18_0_26_53_147 "SCM slot0 capacity is 0! Error!"
#define SCM_CONFIG_1733_112_2_18_0_26_53_148 "SCM slot1 status not normal! Error!"
#define SCM_CONFIG_1740_112_2_18_0_26_53_149 "SCM slot1 capacity is 0! Error!"
#define SCM_CONFIG_1765_112_2_18_0_26_53_150 "SCM slot name error!"
#define SCM_CONFIG_1774_112_2_18_0_26_53_151 "SCM_BSD_UNREG"
#define SCM_CONFIG_1794_112_2_18_0_26_53_152 "SCM slot name error!"
#define SENSOR_CFG_144_112_2_18_0_26_53_153 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define FM_CFG_94_112_2_18_0_27_1_154 "FM_GetCtrlPort %x"
#define GPIO_CFG_197_112_2_18_0_27_1_155 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_270_112_2_18_0_27_1_156 "GPIO_CustomizeInit() End !"
#define PROD_OUTPORT_70_112_2_18_0_27_8_157 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define PROD_OUTPORT_82_112_2_18_0_27_8_158 "Production test dummy function: Register_Current_Test_CmdRoutine"
#define SCM_CONFIG_145_112_2_18_0_27_8_159 "SCM: DIAG, slot_name = %d, msg = %d"
#define SCM_CONFIG_194_112_2_18_0_27_8_160 "SCM: ATest Mode slot_name not support!"
#define SCM_CONFIG_202_112_2_18_0_27_8_161 "SCM: ATest Mode MSG error!"
#define SCM_CONFIG_219_112_2_18_0_27_8_162 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_220_112_2_18_0_27_8_163 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_221_112_2_18_0_27_8_164 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_222_112_2_18_0_27_8_165 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_223_112_2_18_0_27_8_166 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_224_112_2_18_0_27_8_167 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_225_112_2_18_0_27_8_168 "[Init]SCM_APB_CLK_CFG0 = 0x%x"
#define SCM_CONFIG_226_112_2_18_0_27_8_169 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_254_112_2_18_0_27_9_170 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_259_112_2_18_0_27_9_171 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_296_112_2_18_0_27_9_172 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_322_112_2_18_0_27_9_173 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_330_112_2_18_0_27_9_174 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_353_112_2_18_0_27_9_175 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_361_112_2_18_0_27_9_176 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_633_112_2_18_0_27_9_177 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_659_112_2_18_0_27_9_178 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_670_112_2_18_0_27_9_179 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_671_112_2_18_0_27_9_180 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_672_112_2_18_0_27_9_181 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_673_112_2_18_0_27_9_182 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_674_112_2_18_0_27_9_183 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_675_112_2_18_0_27_9_184 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_676_112_2_18_0_27_9_185 "[ErrProc]SCM_APB_CLK_CFG0 = 0x%x"
#define SCM_CONFIG_677_112_2_18_0_27_9_186 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_681_112_2_18_0_27_9_187 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_882_112_2_18_0_27_10_188 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_888_112_2_18_0_27_10_189 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_957_112_2_18_0_27_10_190 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_962_112_2_18_0_27_10_191 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_1182_112_2_18_0_27_10_192 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_1208_112_2_18_0_27_11_193 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_1221_112_2_18_0_27_11_194 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1408_112_2_18_0_27_11_195 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_1414_112_2_18_0_27_11_196 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1526_112_2_18_0_27_11_197 "SCM Read fail"
#define SCM_CONFIG_1551_112_2_18_0_27_11_198 "SCM Write fail"
#define SCM_CONFIG_1574_112_2_18_0_27_11_199 "SCM Erase fail"
#define SCM_CONFIG_1590_112_2_18_0_27_11_200 "SCM GetCap fail"
#define SCM_CONFIG_1617_112_2_18_0_27_12_201 "SCM Read fail"
#define SCM_CONFIG_1642_112_2_18_0_27_12_202 "SCM Write fail"
#define SCM_CONFIG_1665_112_2_18_0_27_12_203 "SCM Erase fail"
#define SCM_CONFIG_1681_112_2_18_0_27_12_204 "SCM GetCap fail"
#define SCM_CONFIG_1693_112_2_18_0_27_12_205 "SCM_BSD_REG"
#define SCM_CONFIG_1699_112_2_18_0_27_12_206 "SCM slot0 status not normal! Error!"
#define SCM_CONFIG_1706_112_2_18_0_27_12_207 "SCM slot0 capacity is 0! Error!"
#define SCM_CONFIG_1733_112_2_18_0_27_12_208 "SCM slot1 status not normal! Error!"
#define SCM_CONFIG_1740_112_2_18_0_27_12_209 "SCM slot1 capacity is 0! Error!"
#define SCM_CONFIG_1765_112_2_18_0_27_12_210 "SCM slot name error!"
#define SCM_CONFIG_1774_112_2_18_0_27_12_211 "SCM_BSD_UNREG"
#define SCM_CONFIG_1794_112_2_18_0_27_12_212 "SCM slot name error!"
#define SENSOR_CFG_144_112_2_18_0_27_12_213 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define TP_MULTI_CFG_82_112_2_18_0_27_15_214 "TPM_GetCtrlPort %x"
#define FM_CFG_82_112_2_18_0_27_20_215 "FM_GetCtrlPort %x"
#define FM_CFG_106_112_2_18_0_27_20_216 "FM_ConfigClk open = %d, ret = 0x%X"
#define GPIO_CFG_188_112_2_18_0_27_21_217 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_263_112_2_18_0_27_21_218 "GPIO_CustomizeInit() End !"
#define GPIO_CFG_269_112_2_18_0_27_21_219 "GPIO_SetSensorPwdn: d,b_on is %d"
#define GPIO_CFG_320_112_2_18_0_27_21_220 "GPIO_GetSwitchDefaultValue: gpio switch default value is wifi"
#define GPIO_CFG_333_112_2_18_0_27_21_221 "GPIO_GetSwitchWiFiValue: gpio_switch_wifi_value is %d"
#define GPIO_CFG_346_112_2_18_0_27_21_222 "GPIO_GetSwitchCMMBValue: gpio_switch_cmmb_value is %d"
#define LCM_CFG_INFO_105_112_2_18_0_27_22_223 "enter Lcm_GetLcmInfo size = %d"
#define MTV_CFG_122_112_2_18_0_27_25_224 "MTVCFG_Open_26MClock: %x"
#define MTV_CFG_162_112_2_18_0_27_25_225 "MTV: (lcd id) index = %d, dev_id = 0x%x, dev_id_in_tab = 0x%x"
#define PROD_OUTPORT_77_112_2_18_0_27_29_226 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define SCM_CONFIG_107_112_2_18_0_27_29_227 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_108_112_2_18_0_27_29_228 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_109_112_2_18_0_27_29_229 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_110_112_2_18_0_27_29_230 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_111_112_2_18_0_27_29_231 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_112_112_2_18_0_27_29_232 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_113_112_2_18_0_27_29_233 "[Init]SCM_GR_CLK_GEN5 = 0x%x"
#define SCM_CONFIG_114_112_2_18_0_27_29_234 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_384_112_2_18_0_27_30_235 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_410_112_2_18_0_27_30_236 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_421_112_2_18_0_27_30_237 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_422_112_2_18_0_27_30_238 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_423_112_2_18_0_27_30_239 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_424_112_2_18_0_27_30_240 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_425_112_2_18_0_27_30_241 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_426_112_2_18_0_27_30_242 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_427_112_2_18_0_27_30_243 "[ErrProc]SCM_GR_CLK_GEN5 = 0x%x"
#define SCM_CONFIG_428_112_2_18_0_27_30_244 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_432_112_2_18_0_27_30_245 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_641_112_2_18_0_27_30_246 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_716_112_2_18_0_27_30_247 "SCM Read fail"
#define SCM_CONFIG_741_112_2_18_0_27_30_248 "SCM Write fail"
#define SCM_CONFIG_764_112_2_18_0_27_30_249 "SCM Erase fail"
#define SCM_CONFIG_780_112_2_18_0_27_30_250 "SCM GetCap fail"
#define SCM_CONFIG_792_112_2_18_0_27_30_251 "SCM_BSD_REG"
#define SCM_CONFIG_828_112_2_18_0_27_30_252 "SCM_BSD_UNREG"
#define SDUA_CONFIG_81_112_2_18_0_27_31_253 "sd is mbbms ca card"
#define SDUA_CONFIG_110_112_2_18_0_27_31_254 "[DRV_SDUA]<ERROR> SDUACFG_IOCTL is wrong\r\n"
#define SENSOR_CFG_125_112_2_18_0_27_31_255 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define FM_CFG_82_112_2_18_0_27_34_256 "FM_GetCtrlPort %x"
#define FM_CFG_106_112_2_18_0_27_34_257 "FM_ConfigClk open = %d, ret = 0x%X"
#define GPIO_CFG_178_112_2_18_0_27_35_258 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_251_112_2_18_0_27_35_259 "GPIO_CustomizeInit() End !"
#define PROD_OUTPORT_70_112_2_18_0_27_41_260 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define PROD_OUTPORT_82_112_2_18_0_27_41_261 "Production test dummy function: Register_Current_Test_CmdRoutine"
#define SCM_CONFIG_145_112_2_18_0_27_42_262 "SCM: DIAG, slot_name = %d, msg = %d"
#define SCM_CONFIG_194_112_2_18_0_27_42_263 "SCM: ATest Mode slot_name not support!"
#define SCM_CONFIG_202_112_2_18_0_27_42_264 "SCM: ATest Mode MSG error!"
#define SCM_CONFIG_219_112_2_18_0_27_42_265 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_220_112_2_18_0_27_42_266 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_221_112_2_18_0_27_42_267 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_222_112_2_18_0_27_42_268 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_223_112_2_18_0_27_42_269 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_224_112_2_18_0_27_42_270 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_225_112_2_18_0_27_42_271 "[Init]SCM_AHB_CLK_CFG1 = 0x%x"
#define SCM_CONFIG_226_112_2_18_0_27_42_272 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_254_112_2_18_0_27_42_273 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_259_112_2_18_0_27_42_274 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_296_112_2_18_0_27_42_275 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_322_112_2_18_0_27_42_276 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_330_112_2_18_0_27_42_277 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_353_112_2_18_0_27_42_278 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_361_112_2_18_0_27_42_279 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_589_112_2_18_0_27_43_280 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_615_112_2_18_0_27_43_281 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_626_112_2_18_0_27_43_282 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_627_112_2_18_0_27_43_283 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_628_112_2_18_0_27_43_284 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_629_112_2_18_0_27_43_285 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_630_112_2_18_0_27_43_286 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_631_112_2_18_0_27_43_287 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_632_112_2_18_0_27_43_288 "[ErrProc]SCM_AHB_CLK_CFG1 = 0x%x"
#define SCM_CONFIG_633_112_2_18_0_27_43_289 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_637_112_2_18_0_27_43_290 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_824_112_2_18_0_27_43_291 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_830_112_2_18_0_27_43_292 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_899_112_2_18_0_27_44_293 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_904_112_2_18_0_27_44_294 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_1124_112_2_18_0_27_44_295 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_1150_112_2_18_0_27_44_296 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_1163_112_2_18_0_27_44_297 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1350_112_2_18_0_27_45_298 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_1356_112_2_18_0_27_45_299 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1468_112_2_18_0_27_45_300 "SCM Read fail"
#define SCM_CONFIG_1493_112_2_18_0_27_45_301 "SCM Write fail"
#define SCM_CONFIG_1516_112_2_18_0_27_45_302 "SCM Erase fail"
#define SCM_CONFIG_1532_112_2_18_0_27_45_303 "SCM GetCap fail"
#define SCM_CONFIG_1559_112_2_18_0_27_45_304 "SCM Read fail"
#define SCM_CONFIG_1584_112_2_18_0_27_45_305 "SCM Write fail"
#define SCM_CONFIG_1607_112_2_18_0_27_45_306 "SCM Erase fail"
#define SCM_CONFIG_1623_112_2_18_0_27_45_307 "SCM GetCap fail"
#define SCM_CONFIG_1635_112_2_18_0_27_45_308 "SCM_BSD_REG"
#define SCM_CONFIG_1641_112_2_18_0_27_45_309 "SCM slot0 status not normal! Error!"
#define SCM_CONFIG_1648_112_2_18_0_27_45_310 "SCM slot0 capacity is 0! Error!"
#define SCM_CONFIG_1675_112_2_18_0_27_45_311 "SCM slot1 status not normal! Error!"
#define SCM_CONFIG_1682_112_2_18_0_27_45_312 "SCM slot1 capacity is 0! Error!"
#define SCM_CONFIG_1707_112_2_18_0_27_45_313 "SCM slot name error!"
#define SCM_CONFIG_1716_112_2_18_0_27_45_314 "SCM_BSD_UNREG"
#define SCM_CONFIG_1736_112_2_18_0_27_46_315 "SCM slot name error!"
#define SENSOR_CFG_136_112_2_18_0_27_46_316 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define FM_CFG_93_112_2_18_0_27_51_317 "FM_GetCtrlPort %x"
#define FM_CFG_117_112_2_18_0_27_51_318 "FM_ConfigClk open = %d, ret = 0x%X"
#define GPIO_CFG_200_112_2_18_0_27_51_319 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_316_112_2_18_0_27_52_320 "GPIO_CustomizeInit() End !"
#define PROD_OUTPORT_70_112_2_18_0_27_57_321 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define PROD_OUTPORT_82_112_2_18_0_27_57_322 "Production test dummy function: Register_Current_Test_CmdRoutine"
#define SCM_CONFIG_148_112_2_18_0_27_58_323 "SCM: DIAG, slot_name = %d, msg = %d"
#define SCM_CONFIG_197_112_2_18_0_27_58_324 "SCM: ATest Mode slot_name not support!"
#define SCM_CONFIG_205_112_2_18_0_27_58_325 "SCM: ATest Mode MSG error!"
#define SCM_CONFIG_222_112_2_18_0_27_58_326 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_223_112_2_18_0_27_58_327 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_224_112_2_18_0_27_58_328 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_225_112_2_18_0_27_58_329 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_226_112_2_18_0_27_58_330 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_227_112_2_18_0_27_58_331 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_228_112_2_18_0_27_58_332 "[Init]SCM_AHB_CLK_CFG1 = 0x%x"
#define SCM_CONFIG_229_112_2_18_0_27_58_333 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_259_112_2_18_0_27_58_334 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_264_112_2_18_0_27_58_335 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_303_112_2_18_0_27_58_336 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_329_112_2_18_0_27_58_337 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_337_112_2_18_0_27_58_338 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_360_112_2_18_0_27_58_339 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_368_112_2_18_0_27_58_340 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_593_112_2_18_0_27_59_341 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_619_112_2_18_0_27_59_342 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_630_112_2_18_0_27_59_343 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_631_112_2_18_0_27_59_344 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_632_112_2_18_0_27_59_345 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_633_112_2_18_0_27_59_346 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_634_112_2_18_0_27_59_347 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_635_112_2_18_0_27_59_348 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_636_112_2_18_0_27_59_349 "[ErrProc]SCM_AHB_CLK_CFG1 = 0x%x"
#define SCM_CONFIG_637_112_2_18_0_27_59_350 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_641_112_2_18_0_27_59_351 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_828_112_2_18_0_27_59_352 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_834_112_2_18_0_27_59_353 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_903_112_2_18_0_27_59_354 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_908_112_2_18_0_27_59_355 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_1128_112_2_18_0_28_0_356 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_1154_112_2_18_0_28_0_357 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_1167_112_2_18_0_28_0_358 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1354_112_2_18_0_28_0_359 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_1360_112_2_18_0_28_0_360 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1472_112_2_18_0_28_0_361 "SCM Read fail"
#define SCM_CONFIG_1497_112_2_18_0_28_0_362 "SCM Write fail"
#define SCM_CONFIG_1520_112_2_18_0_28_1_363 "SCM Erase fail"
#define SCM_CONFIG_1536_112_2_18_0_28_1_364 "SCM GetCap fail"
#define SCM_CONFIG_1563_112_2_18_0_28_1_365 "SCM Read fail"
#define SCM_CONFIG_1588_112_2_18_0_28_1_366 "SCM Write fail"
#define SCM_CONFIG_1611_112_2_18_0_28_1_367 "SCM Erase fail"
#define SCM_CONFIG_1627_112_2_18_0_28_1_368 "SCM GetCap fail"
#define SCM_CONFIG_1639_112_2_18_0_28_1_369 "SCM_BSD_REG"
#define SCM_CONFIG_1645_112_2_18_0_28_1_370 "SCM slot0 status not normal! Error!"
#define SCM_CONFIG_1652_112_2_18_0_28_1_371 "SCM slot0 capacity is 0! Error!"
#define SCM_CONFIG_1679_112_2_18_0_28_1_372 "SCM slot1 status not normal! Error!"
#define SCM_CONFIG_1686_112_2_18_0_28_1_373 "SCM slot1 capacity is 0! Error!"
#define SCM_CONFIG_1711_112_2_18_0_28_1_374 "SCM slot name error!"
#define SCM_CONFIG_1720_112_2_18_0_28_1_375 "SCM_BSD_UNREG"
#define SCM_CONFIG_1740_112_2_18_0_28_1_376 "SCM slot name error!"
#define SENSOR_CFG_117_112_2_18_0_28_1_377 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define TP_MULTI_CFG_82_112_2_18_0_28_3_378 "TPM_GetCtrlPort %x"
#define FM_CFG_84_112_2_18_0_28_6_379 "FM_GetCtrlPort %x"
#define FM_CFG_108_112_2_18_0_28_6_380 "FM_ConfigClk open = %d, ret = 0x%X"
#define GPIO_CFG_178_112_2_18_0_28_6_381 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_251_112_2_18_0_28_7_382 "GPIO_CustomizeInit() End !"
#define PROD_OUTPORT_70_112_2_18_0_28_12_383 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define PROD_OUTPORT_82_112_2_18_0_28_12_384 "Production test dummy function: Register_Current_Test_CmdRoutine"
#define SCM_CONFIG_145_112_2_18_0_28_13_385 "SCM: DIAG, slot_name = %d, msg = %d"
#define SCM_CONFIG_194_112_2_18_0_28_13_386 "SCM: ATest Mode slot_name not support!"
#define SCM_CONFIG_202_112_2_18_0_28_13_387 "SCM: ATest Mode MSG error!"
#define SCM_CONFIG_219_112_2_18_0_28_13_388 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_220_112_2_18_0_28_13_389 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_221_112_2_18_0_28_13_390 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_222_112_2_18_0_28_13_391 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_223_112_2_18_0_28_13_392 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_224_112_2_18_0_28_13_393 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_225_112_2_18_0_28_13_394 "[Init]SCM_AHB_CLK_CFG1 = 0x%x"
#define SCM_CONFIG_226_112_2_18_0_28_13_395 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_254_112_2_18_0_28_13_396 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_259_112_2_18_0_28_13_397 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_296_112_2_18_0_28_13_398 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_322_112_2_18_0_28_13_399 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_330_112_2_18_0_28_13_400 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_353_112_2_18_0_28_13_401 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_361_112_2_18_0_28_13_402 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_589_112_2_18_0_28_13_403 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_615_112_2_18_0_28_14_404 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_626_112_2_18_0_28_14_405 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_627_112_2_18_0_28_14_406 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_628_112_2_18_0_28_14_407 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_629_112_2_18_0_28_14_408 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_630_112_2_18_0_28_14_409 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_631_112_2_18_0_28_14_410 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_632_112_2_18_0_28_14_411 "[ErrProc]SCM_AHB_CLK_CFG1 = 0x%x"
#define SCM_CONFIG_633_112_2_18_0_28_14_412 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_637_112_2_18_0_28_14_413 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_824_112_2_18_0_28_14_414 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_830_112_2_18_0_28_14_415 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_899_112_2_18_0_28_14_416 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_904_112_2_18_0_28_14_417 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_1124_112_2_18_0_28_15_418 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_1150_112_2_18_0_28_15_419 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_1163_112_2_18_0_28_15_420 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1350_112_2_18_0_28_15_421 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_1356_112_2_18_0_28_15_422 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1468_112_2_18_0_28_15_423 "SCM Read fail"
#define SCM_CONFIG_1493_112_2_18_0_28_15_424 "SCM Write fail"
#define SCM_CONFIG_1516_112_2_18_0_28_15_425 "SCM Erase fail"
#define SCM_CONFIG_1532_112_2_18_0_28_15_426 "SCM GetCap fail"
#define SCM_CONFIG_1559_112_2_18_0_28_15_427 "SCM Read fail"
#define SCM_CONFIG_1584_112_2_18_0_28_15_428 "SCM Write fail"
#define SCM_CONFIG_1607_112_2_18_0_28_16_429 "SCM Erase fail"
#define SCM_CONFIG_1623_112_2_18_0_28_16_430 "SCM GetCap fail"
#define SCM_CONFIG_1635_112_2_18_0_28_16_431 "SCM_BSD_REG"
#define SCM_CONFIG_1641_112_2_18_0_28_16_432 "SCM slot0 status not normal! Error!"
#define SCM_CONFIG_1648_112_2_18_0_28_16_433 "SCM slot0 capacity is 0! Error!"
#define SCM_CONFIG_1675_112_2_18_0_28_16_434 "SCM slot1 status not normal! Error!"
#define SCM_CONFIG_1682_112_2_18_0_28_16_435 "SCM slot1 capacity is 0! Error!"
#define SCM_CONFIG_1707_112_2_18_0_28_16_436 "SCM slot name error!"
#define SCM_CONFIG_1716_112_2_18_0_28_16_437 "SCM_BSD_UNREG"
#define SCM_CONFIG_1736_112_2_18_0_28_16_438 "SCM slot name error!"
#define SENSOR_CFG_129_112_2_18_0_28_16_439 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define FM_CFG_95_112_2_18_0_28_20_440 "FM_GetCtrlPort %x"
#define GPIO_CFG_270_112_2_18_0_28_21_441 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_352_112_2_18_0_28_21_442 "GPIO_CustomizeInit() End !"
#define PROD_OUTPORT_70_112_2_18_0_28_27_443 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define PROD_OUTPORT_82_112_2_18_0_28_27_444 "Production test dummy function: Register_Current_Test_CmdRoutine"
#define SCM_CONFIG_148_112_2_18_0_28_27_445 "SCM: DIAG, slot_name = %d, msg = %d"
#define SCM_CONFIG_197_112_2_18_0_28_27_446 "SCM: ATest Mode slot_name not support!"
#define SCM_CONFIG_205_112_2_18_0_28_27_447 "SCM: ATest Mode MSG error!"
#define SCM_CONFIG_224_112_2_18_0_28_28_448 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_225_112_2_18_0_28_28_449 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_226_112_2_18_0_28_28_450 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_227_112_2_18_0_28_28_451 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_228_112_2_18_0_28_28_452 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_229_112_2_18_0_28_28_453 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_230_112_2_18_0_28_28_454 "[Init]SCM_AHB_CLK_CFG1 = 0x%x"
#define SCM_CONFIG_231_112_2_18_0_28_28_455 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_259_112_2_18_0_28_28_456 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_264_112_2_18_0_28_28_457 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_302_112_2_18_0_28_28_458 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_328_112_2_18_0_28_28_459 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_336_112_2_18_0_28_28_460 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_359_112_2_18_0_28_28_461 "[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
#define SCM_CONFIG_367_112_2_18_0_28_28_462 "[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
#define SCM_CONFIG_592_112_2_18_0_28_28_463 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_618_112_2_18_0_28_28_464 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_629_112_2_18_0_28_28_465 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_630_112_2_18_0_28_28_466 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_631_112_2_18_0_28_28_467 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_632_112_2_18_0_28_28_468 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_633_112_2_18_0_28_28_469 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_634_112_2_18_0_28_28_470 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_635_112_2_18_0_28_28_471 "[ErrProc]SCM_AHB_CLK_CFG1 = 0x%x"
#define SCM_CONFIG_636_112_2_18_0_28_28_472 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_640_112_2_18_0_28_28_473 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_821_112_2_18_0_28_29_474 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_827_112_2_18_0_28_29_475 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_896_112_2_18_0_28_29_476 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_901_112_2_18_0_28_29_477 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_1121_112_2_18_0_28_29_478 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_1147_112_2_18_0_28_29_479 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_1160_112_2_18_0_28_29_480 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1347_112_2_18_0_28_30_481 "[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
#define SCM_CONFIG_1353_112_2_18_0_28_30_482 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_1465_112_2_18_0_28_30_483 "SCM Read fail"
#define SCM_CONFIG_1490_112_2_18_0_28_30_484 "SCM Write fail"
#define SCM_CONFIG_1513_112_2_18_0_28_30_485 "SCM Erase fail"
#define SCM_CONFIG_1529_112_2_18_0_28_30_486 "SCM GetCap fail"
#define SCM_CONFIG_1556_112_2_18_0_28_30_487 "SCM Read fail"
#define SCM_CONFIG_1581_112_2_18_0_28_30_488 "SCM Write fail"
#define SCM_CONFIG_1604_112_2_18_0_28_30_489 "SCM Erase fail"
#define SCM_CONFIG_1620_112_2_18_0_28_30_490 "SCM GetCap fail"
#define SCM_CONFIG_1632_112_2_18_0_28_30_491 "SCM_BSD_REG"
#define SCM_CONFIG_1638_112_2_18_0_28_30_492 "SCM slot0 status not normal! Error!"
#define SCM_CONFIG_1645_112_2_18_0_28_30_493 "SCM slot0 capacity is 0! Error!"
#define SCM_CONFIG_1672_112_2_18_0_28_30_494 "SCM slot1 status not normal! Error!"
#define SCM_CONFIG_1679_112_2_18_0_28_30_495 "SCM slot1 capacity is 0! Error!"
#define SCM_CONFIG_1704_112_2_18_0_28_30_496 "SCM slot name error!"
#define SCM_CONFIG_1713_112_2_18_0_28_30_497 "SCM_BSD_UNREG"
#define SCM_CONFIG_1733_112_2_18_0_28_31_498 "SCM slot name error!"
#define SENSOR_CFG_123_112_2_18_0_28_31_499 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define TP_MULTI_CFG_82_112_2_18_0_28_32_500 "TPM_GetCtrlPort %x"
#define FM_CFG_82_112_2_18_0_28_34_501 "FM_GetCtrlPort %x"
#define FM_CFG_106_112_2_18_0_28_34_502 "FM_ConfigClk open = %d, ret = 0x%X"
#define GPIO_CFG_165_112_2_18_0_28_35_503 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_230_112_2_18_0_28_35_504 "GPIO_CustomizeInit() End !"
#define GPIO_CFG_236_112_2_18_0_28_35_505 "GPIO_SetSensorPwdn: d,b_on is %d"
#define MTV_CFG_122_112_2_18_0_28_38_506 "MTVCFG_Open_26MClock: %x"
#define MTV_CFG_162_112_2_18_0_28_39_507 "MTV: (lcd id) index = %d, dev_id = 0x%x, dev_id_in_tab = 0x%x"
#define PROD_OUTPORT_77_112_2_18_0_28_43_508 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define SCM_CONFIG_144_112_2_18_0_28_43_509 "SCM: DIAG, slot_name = %d, msg = %d"
#define SCM_CONFIG_193_112_2_18_0_28_43_510 "SCM: ATest Mode slot_name not support!"
#define SCM_CONFIG_201_112_2_18_0_28_43_511 "SCM: ATest Mode MSG error!"
#define SCM_CONFIG_219_112_2_18_0_28_43_512 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_220_112_2_18_0_28_43_513 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_221_112_2_18_0_28_43_514 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_222_112_2_18_0_28_43_515 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_223_112_2_18_0_28_43_516 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_224_112_2_18_0_28_43_517 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_225_112_2_18_0_28_43_518 "[Init]SCM_GR_CLK_GEN5 = 0x%x"
#define SCM_CONFIG_226_112_2_18_0_28_43_519 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_265_112_2_18_0_28_44_520 "ATest Mode PO -- Plug out"
#define SCM_CONFIG_270_112_2_18_0_28_44_521 "ATest Mode PI -- Plug in"
#define SCM_CONFIG_524_112_2_18_0_28_44_522 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_550_112_2_18_0_28_44_523 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_561_112_2_18_0_28_44_524 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_562_112_2_18_0_28_44_525 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_563_112_2_18_0_28_44_526 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_564_112_2_18_0_28_44_527 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_565_112_2_18_0_28_44_528 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_566_112_2_18_0_28_44_529 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_567_112_2_18_0_28_44_530 "[ErrProc]SCM_GR_CLK_GEN5 = 0x%x"
#define SCM_CONFIG_568_112_2_18_0_28_44_531 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_572_112_2_18_0_28_44_532 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_781_112_2_18_0_28_45_533 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_856_112_2_18_0_28_45_534 "SCM Read fail"
#define SCM_CONFIG_881_112_2_18_0_28_45_535 "SCM Write fail"
#define SCM_CONFIG_904_112_2_18_0_28_45_536 "SCM Erase fail"
#define SCM_CONFIG_920_112_2_18_0_28_45_537 "SCM GetCap fail"
#define SCM_CONFIG_932_112_2_18_0_28_45_538 "SCM_BSD_REG"
#define SCM_CONFIG_968_112_2_18_0_28_45_539 "SCM_BSD_UNREG"
#define SDUA_CONFIG_81_112_2_18_0_28_45_540 "sd is mbbms ca card"
#define SDUA_CONFIG_110_112_2_18_0_28_45_541 "[DRV_SDUA]<ERROR> SDUACFG_IOCTL is wrong\r\n"
#define SENSOR_CFG_114_112_2_18_0_28_46_542 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define GPIO_CFG_161_112_2_18_0_28_49_543 "GPIO_CustomizeInit() Start !"
#define GPIO_CFG_224_112_2_18_0_28_49_544 "GPIO_CustomizeInit() End !"
#define GPIO_CFG_230_112_2_18_0_28_49_545 "GPIO_SetSensorPwdn: d,b_on is %d"
#define MTV_CFG_122_112_2_18_0_28_52_546 "MTVCFG_Open_26MClock: %x"
#define MTV_CFG_162_112_2_18_0_28_52_547 "MTV: (lcd id) index = %d, dev_id = 0x%x, dev_id_in_tab = 0x%x"
#define PROD_OUTPORT_77_112_2_18_0_28_56_548 "Production test dummy function: Register_MMI_Test_CmdRoutine"
#define SCM_CONFIG_107_112_2_18_0_28_57_549 "[Init]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_108_112_2_18_0_28_57_550 "[Init]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_109_112_2_18_0_28_57_551 "[Init]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_110_112_2_18_0_28_57_552 "[Init]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_111_112_2_18_0_28_57_553 "[Init]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_112_112_2_18_0_28_57_554 "[Init]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_113_112_2_18_0_28_57_555 "[Init]SCM_GR_CLK_GEN5 = 0x%x"
#define SCM_CONFIG_114_112_2_18_0_28_57_556 "[Init]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_376_112_2_18_0_28_57_557 "SCM ErrProc too many in HighSpeed mode! Use normal mode!"
#define SCM_CONFIG_402_112_2_18_0_28_57_558 "SCM ErrProc too many in normal mode! FALSE!"
#define SCM_CONFIG_413_112_2_18_0_28_57_559 "[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
#define SCM_CONFIG_414_112_2_18_0_28_57_560 "[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
#define SCM_CONFIG_415_112_2_18_0_28_57_561 "[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
#define SCM_CONFIG_416_112_2_18_0_28_57_562 "[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
#define SCM_CONFIG_417_112_2_18_0_28_57_563 "[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
#define SCM_CONFIG_418_112_2_18_0_28_57_564 "[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
#define SCM_CONFIG_419_112_2_18_0_28_57_565 "[ErrProc]SCM_GR_CLK_GEN5 = 0x%x"
#define SCM_CONFIG_420_112_2_18_0_28_57_566 "[ErrProc]SCM_GR_MPLL_MN = 0x%x"
#define SCM_CONFIG_424_112_2_18_0_28_57_567 "SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_633_112_2_18_0_28_58_568 "SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
#define SCM_CONFIG_708_112_2_18_0_28_58_569 "SCM Read fail"
#define SCM_CONFIG_733_112_2_18_0_28_58_570 "SCM Write fail"
#define SCM_CONFIG_756_112_2_18_0_28_58_571 "SCM Erase fail"
#define SCM_CONFIG_772_112_2_18_0_28_58_572 "SCM GetCap fail"
#define SCM_CONFIG_784_112_2_18_0_28_58_573 "SCM_BSD_REG"
#define SCM_CONFIG_820_112_2_18_0_28_58_574 "SCM_BSD_UNREG"
#define SENSOR_CFG_112_112_2_18_0_28_59_575 "Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d"
#define AF_CFG_92_112_2_18_0_29_0_576 "AF Is beding:%d"
#define AF_CFG_209_112_2_18_0_29_0_577 "Set Preview auto focus state"
#define AF_NFMW_MICRON_3M_216_112_2_18_0_29_2_578 "AF: index  = %d,  value = %d"
#define AF_NFMW_MICRON_3M_257_112_2_18_0_29_2_579 "AF: index_1  = %d MaxFoucus Index ' %d"
#define AF_NFMW_MICRON_3M_261_112_2_18_0_29_2_580 "AF: index  = %d"
#define AF_NFMW_MICRON_3M_336_112_2_18_0_29_2_581 "AF SDY: check--  stable: MaxFocusIndex: %d ; FineFocusindex = %d"
#define AF_NFMW_MICRON_3M_376_112_2_18_0_29_2_582 "AF SDY: check--  stable: MaxFocusIndex: %d ; FineMaxFocusindex = %d"
#define AF_NFMW_MICRON_3M_389_112_2_18_0_29_2_583 "Focusstate = %d"
#define BT_PROD_210_112_2_18_0_29_4_584 "[BT]BTI_RestoreSystemClk:type = 0x%X\r\n"
#define BT_PROD_220_112_2_18_0_29_4_585 "[BT]BTI_RestoreSystemClk:Idle restore successful\r\n"
#define BT_PROD_224_112_2_18_0_29_4_586 "[BT]BTI_RestoreSystemClk:Idle restore error\r\n"
#define BT_PROD_235_112_2_18_0_29_4_587 "[BT]BTI_RestoreSystemClk:FTP restore successful\r\n"
#define BT_PROD_239_112_2_18_0_29_4_588 "[BT]BTI_RestoreSystemClk:FTP restore error\r\n"
#define BT_PROD_250_112_2_18_0_29_4_589 "[BT]BTI_RestoreSystemClk:HFG restore successful\r\n"
#define BT_PROD_254_112_2_18_0_29_4_590 "[BT]BTI_RestoreSystemClk:HFG restore error\r\n"
#define BT_PROD_265_112_2_18_0_29_4_591 "[BT]BTI_RestoreSystemClk:A2DP restore successful\r\n"
#define BT_PROD_269_112_2_18_0_29_4_592 "[BT]BTI_RestoreSystemClk:A2DP restore error\r\n"
#define BT_PROD_275_112_2_18_0_29_4_593 "[BT]<ERROR> BTI_RestoreSystemClk: NOT supported type\r\n"
#define BT_PROD_286_112_2_18_0_29_4_594 "[BT]BTI_ChangeSystemClk:type = 0x%X\r\n"
#define BT_PROD_295_112_2_18_0_29_4_595 "[BT]BTI_ChangeSystemClk:IDLE's clolck change successful\r\n"
#define BT_PROD_299_112_2_18_0_29_4_596 "[BT]BTI_ChangeSystemClk:IDLE changed clolck error\r\n"
#define BT_PROD_308_112_2_18_0_29_4_597 "[BT]BTI_ChangeSystemClk:FTP's clolck change successful\r\n"
#define BT_PROD_312_112_2_18_0_29_4_598 "[BT]BTI_ChangeSystemClk:FTP changed clolck error\r\n"
#define BT_PROD_321_112_2_18_0_29_4_599 "[BT]BTI_ChangeSystemClk:HFG's clolck change successful\r\n"
#define BT_PROD_325_112_2_18_0_29_4_600 "[BT]BTI_ChangeSystemClk:HFG changed clolck error\r\n"
#define BT_PROD_334_112_2_18_0_29_4_601 "[BT]BTI_ChangeSystemClk:A2DP's clolck change successful\r\n"
#define BT_PROD_338_112_2_18_0_29_4_602 "[BT]BTI_ChangeSystemClk:A2DP changed clolck error\r\n"
#define BT_PROD_343_112_2_18_0_29_4_603 "[BT]<ERROR> BTI_ChangeSystemClk: NOT supported type\r\n"
#define BT_RDA_32_112_2_18_0_29_5_604 "rda_InitGpioI2C"
#define BT_RDA_50_112_2_18_0_29_5_605 "rda_DeinitGpioI2C"
#define BT_RDA_201_112_2_18_0_29_5_606 "\"# bt i2c write failed "
#define BT_RDA_446_112_2_18_0_29_6_607 "The chip id of RDA5868+ is 0x%08x"
#define BT_RDA_453_112_2_18_0_29_6_608 " BTI_StartupChip\r\n"
#define FM_NMI601_133_112_2_18_0_29_7_609 "[FM_DRV]:_nmi600_FM_PowerOn %d"
#define FM_NMI601_164_112_2_18_0_29_7_610 "[FM_DRV]NMI600_Initial()"
#define FM_NMI601_181_112_2_18_0_29_8_611 "[FM_DRV]NMI600_PlayStart() ,%d"
#define FM_NMI601_198_112_2_18_0_29_8_612 "[FM_DRV]NMI600_PlayStop()"
#define FM_NMI601_218_112_2_18_0_29_8_613 "[FM_DRV]NMI600_Open "
#define FM_NMI601_229_112_2_18_0_29_8_614 "[FM_DRV]NMI600_Close "
#define FM_NMI601_248_112_2_18_0_29_8_615 "[FM_DRV]NMI600_Pause "
#define FM_NMI601_261_112_2_18_0_29_8_616 "[FM_DRV]NMI600_Resume "
#define FM_NMI601_287_112_2_18_0_29_8_617 "[FM_DRV]NMI600_ManualSeek , rssi %d"
#define FM_NMI601_370_112_2_18_0_29_8_618 "[FM_DRV]NMI600_AutoSeek: ret = %d,  state = %d, freq = %d"
#define FM_NMI601_387_112_2_18_0_29_8_619 "NMI600_SetVol  ,%d"
#define FM_NMI601_404_112_2_18_0_29_8_620 "NMI600_SetFMRegion "
#define FM_NMI601_420_112_2_18_0_29_8_621 "NMI600_SetScanLevel "
#define NMIDRV_ATV_39_112_2_18_0_29_34_622 "nmidrv_atv: nmi atv power on failed!!!"
#define NMIDRV_ATV_539_112_2_18_0_29_35_623 "atv_nmi_init_scan_param region = %d,  scan_start_chn = %d,  scan_end_chn= %d \r\n"
#define NMIDRV_ATV_547_112_2_18_0_29_35_624 "atv_nmi_scan_chn scan_chn_index = %d \r\n"
#define NMIPORT_56_112_2_18_0_29_36_625 "%s"
#define NMIPORT_300_112_2_18_0_29_37_626 "Failed, bus write...\n"
#define NMIPORT_306_112_2_18_0_29_37_627 "Failed, bus read...\n"
#define NMIPORT_310_112_2_18_0_29_37_628 "nmi600 adr(0x%08x),val(0x%08x)\n\r"
#define NMIPORT_347_112_2_18_0_29_37_629 "NMIAPP_InitVideoSize fullscreen:%d"
#define NMIPORT_366_112_2_18_0_29_37_630 "%s"
#define ATV_NMI601_253_112_2_18_0_29_38_631 "SENSOR: _nmi600_InitChannel"
#define ATV_NMI601_358_112_2_18_0_29_38_632 "GPIO_SetAnalogTVPowerOn,power = %d"
#define ATV_NMI601_369_112_2_18_0_29_38_633 "ATV: _nmi600_Power_On(1:on, 0:off): %d"
#define ATV_NMI601_392_112_2_18_0_29_38_634 "ATV: _nmi600_Power_On(1:on, 0:off) end : %d"
#define ATV_NMI601_405_112_2_18_0_29_38_635 "ATV: _nmi600_Identify"
#define ATV_NMI601_409_112_2_18_0_29_38_636 "ATV: the atv is not nmi600"
#define ATV_NMI601_414_112_2_18_0_29_38_637 "ATV: the atv is nmi600"
#define ATV_NMI601_429_112_2_18_0_29_38_638 "ATV:_nmi600_Init"
#define ATV_NMI601_445_112_2_18_0_29_38_639 "ATV:_nmi600_Sleep"
#define ATV_NMI601_482_112_2_18_0_29_38_640 "ATV:_nmi600_ScanChn"
#define ATV_NMI601_506_112_2_18_0_29_38_641 "ATV:_nmi600_SetChn : 0x%x"
#define ATV_NMI601_535_112_2_18_0_29_38_642 "ATV:_nmi600_GetAllChnNum"
#define ATV_NMI601_551_112_2_18_0_29_38_643 "ATV:_nmi600_SetVolume : 0x0%x"
#define ATV_NMI601_593_112_2_18_0_29_38_644 "ATV:_nmi600_SetRegion"
#define ATV_NMI601_694_112_2_18_0_29_39_645 "ATV:_nmi600_GetRssi"
#define ATV_NMI601_746_112_2_18_0_29_39_646 "ATV:_nmi600_IsNTSCMode"
#define ATV_NMI601_794_112_2_18_0_29_39_647 "ATV:_nmi600_Close"
#define ATV_NMI601_812_112_2_18_0_29_39_648 "ATV: nmi600_WriteReg reg/value(0x%x,0x%x) !!"
#define ATV_NMI601_825_112_2_18_0_29_39_649 "ATV: nmi600_ReadReg reg/value(0x%x,0x%x) !!"
#define QMADRIVER_465_112_2_18_0_29_44_650 "QMA:Chip ID = %x\n"
#define QMADRIVER_585_112_2_18_0_29_44_651 "ATV:QN:QMA_SetRegion param[%d]"
#define QMADRIVER_1322_112_2_18_0_29_45_652 "ATV:atv_qma_scan_chn got id[%d]"
#define QMADRIVER_1331_112_2_18_0_29_45_653 "ATV: FREQ=%d,qma->chCount=%d,rgn=%d,freq=%d,rgnChTatol=%d,chlist=%d"
#define QN8610I2C_20_112_2_18_0_29_51_654 "ATV:QN:QMA_WriteReg check :Fail Write[%d] Read[%d]"
#define ATV_QN8610_260_112_2_18_0_29_52_655 "ATV:QN:_qn8610_PowerOnSequence power_on[%d]"
#define ATV_QN8610_293_112_2_18_0_29_52_656 "ATV:QN:_qn8610_Identify "
#define ATV_QN8610_301_112_2_18_0_29_52_657 "ATV: the atv is QN6810"
#define ATV_QN8610_305_112_2_18_0_29_52_658 "ATV: the atv is not QN6810"
#define ATV_QN8610_320_112_2_18_0_29_52_659 "ATV:QN:_qn8610_Init param[%d]"
#define ATV_QN8610_337_112_2_18_0_29_52_660 "ATV:QN:_qn8610_Sleep sleep_enter[%d]"
#define ATV_QN8610_358_112_2_18_0_29_52_661 "ATV:QN:_qn8610_ScanChnInit param[%d]"
#define ATV_QN8610_379_112_2_18_0_29_52_662 "ATV:_qn8610_ScanChn chn_id[%d]"
#define ATV_QN8610_386_112_2_18_0_29_52_663 "ATV:_qn8610_ScanChn not complete rtn[%d]"
#define ATV_QN8610_402_112_2_18_0_29_52_664 "ATV:QN:_qn8610_SetChn param[%d]"
#define ATV_QN8610_443_112_2_18_0_29_52_665 "ATV:QN:_qn8610_GetAllChnNum total_num[%d]"
#define ATV_QN8610_457_112_2_18_0_29_52_666 "ATV:QN:_qn8610_SetVolume param[%d]"
#define ATV_QN8610_494_112_2_18_0_29_52_667 "ATV:QN:_qn8610_SetRegion param[%d]"
#define ATV_QN8610_641_112_2_18_0_29_53_668 "ATV:QN:_qn8610_IsNTSCMode Is_NTSCMode[%d]"
#define ATV_QN8610_669_112_2_18_0_29_53_669 "ATV:QN:_qn8610_Close"
#define ATV_QN8610_685_112_2_18_0_29_53_670 "ATV:QN: qn8610_WriteReg reg/value(%x,%x) !!"
#define ATV_QN8610_698_112_2_18_0_29_53_671 "ATV:QN: qn8610_ReadReg reg/value(%x,%x) !!"
#define TLG1120CUSTOMIZE_49_112_2_18_0_30_25_672 "TLGAPP_InitVideoSize is ntsc mode : %d"
#define ATV_TLG1120_260_112_2_18_0_30_28_673 "SENSOR: _TLG1120_InitChannel"
#define ATV_TLG1120_386_112_2_18_0_30_28_674 "SENSOR: _tlg1120_Power_On(1:on, 0:off): %d"
#define ATV_TLG1120_425_112_2_18_0_30_29_675 "ATV: _TLG1120_Identify"
#define ATV_TLG1120_431_112_2_18_0_30_29_676 "ATV: the atv is TLG1120"
#define ATV_TLG1120_435_112_2_18_0_30_29_677 "ATV: the atv is not TLG1120"
#define ATV_TLG1120_450_112_2_18_0_30_29_678 "ATV:_TLG1120_Init"
#define ATV_TLG1120_466_112_2_18_0_30_29_679 "ATV:_TLG1120_Sleep"
#define ATV_TLG1120_490_112_2_18_0_30_29_680 "ATV:_TLG1120_ScanChnInit"
#define ATV_TLG1120_522_112_2_18_0_30_29_681 "ATV:_TLG1120_ScanChn"
#define ATV_TLG1120_547_112_2_18_0_30_29_682 "ATV:_TLG1120_SetChn : 0x%x"
#define ATV_TLG1120_576_112_2_18_0_30_29_683 "ATV:_TLG1120_GetAllChnNum"
#define ATV_TLG1120_595_112_2_18_0_30_29_684 "ATV:_TLG1120_SetVolume : 0x0%x"
#define ATV_TLG1120_635_112_2_18_0_30_29_685 "ATV:_TLG1120_SetRegion"
#define ATV_TLG1120_737_112_2_18_0_30_29_686 "ATV:_TLG1120_GetRssi"
#define ATV_TLG1120_788_112_2_18_0_30_29_687 "ATV:_TLG1120_IsNTSCMode"
#define ATV_TLG1120_802_112_2_18_0_30_29_688 "ATV:_TLG1120_is NTSC"
#define ATV_TLG1120_807_112_2_18_0_30_29_689 "ATV:_TLG1120_is PAL"
#define ATV_TLG1120_838_112_2_18_0_30_29_690 "ATV:_TLG1120_Close"
#define ATV_TLG1120_854_112_2_18_0_30_30_691 "ATV: tlg1120_WriteReg reg/value(0x%x,0x%x) !!"
#define ATV_TLG1120_867_112_2_18_0_30_30_692 "ATV: tlg1120_ReadReg reg/value(0x%x,0x%x) !!"
#define SENSOR_BF3703_546_112_2_18_0_30_31_693 "SENSOR: BF3703_WriteReg reg/value(%x,%x) !!"
#define SENSOR_BF3703_554_112_2_18_0_30_31_694 "BF3703_ReadReg: subaddr:%x,value:%x"
#define SENSOR_BF3703_570_112_2_18_0_30_31_695 "BF3703_Identify: start"
#define SENSOR_BF3703_595_112_2_18_0_30_31_696 "BF3703_Identify: it is BF3703"
#define SENSOR_BF3703_610_112_2_18_0_30_31_697 "BF3703_Identify: ret = %d"
#define SENSOR_BF3703_633_112_2_18_0_30_31_698 "SENSOR: set_ae_enable: enable = %d"
#define SENSOR_BF3703_665_112_2_18_0_30_31_699 "set_BF3703_ae_awb_enable: set_ae_awb_enable: ae=%d awb=%d"
#define SENSOR_BF3703_739_112_2_18_0_30_31_700 "set_BF3703_ev: set_ev: level = %d"
#define SENSOR_BF3703_769_112_2_18_0_30_31_701 "set_BF3703_anti_flicker: set_banding_mode: mode = %d"
#define SENSOR_BF3703_811_112_2_18_0_30_31_702 "SENSOR: set_BF3703_video_mode: mode = %d"
#define SENSOR_BF3703_901_112_2_18_0_30_32_703 "set_BF3703_awb: mode = %d"
#define SENSOR_BF3703_938_112_2_18_0_30_32_704 "BF3703 set_brightness: level = %d"
#define SENSOR_BF3703_971_112_2_18_0_30_32_705 "BF3703: set_contrast: level = %d"
#define SENSOR_BF3703_999_112_2_18_0_30_32_706 "BF3703 set_preview_mode: preview_mode = %d"
#define SENSOR_BF3703_1072_112_2_18_0_30_32_707 "BF3703 set_image_effect: effect_type = %d"
#define SENSOR_BF3703_1185_112_2_18_0_30_32_708 "BF3703 set_work_mode: mode = %d"
#define SENSOR_BF3703_1226_112_2_18_0_30_32_709 "SENSOR: _BF3703_Power_On(1:on, 0:off): %d"
#define SENSOR_BF3903_413_112_2_18_0_30_33_710 "SENSOR: BF3903_WriteReg reg/value(%x,%x) !!"
#define SENSOR_BF3903_421_112_2_18_0_30_33_711 "BF3903_ReadReg: subaddr:%x,value:%x"
#define SENSOR_BF3903_436_112_2_18_0_30_33_712 "BF3903_Identify: start"
#define SENSOR_BF3903_460_112_2_18_0_30_33_713 "BF3903_Identify: it is BF3903"
#define SENSOR_BF3903_482_112_2_18_0_30_33_714 "SENSOR: set_ae_enable: enable = %d"
#define SENSOR_BF3903_514_112_2_18_0_30_34_715 "set_BF3903_ae_awb_enable: set_ae_awb_enable: ae=%d awb=%d"
#define SENSOR_BF3903_584_112_2_18_0_30_34_716 "set_BF3903_ev: set_ev: level = %d"
#define SENSOR_BF3903_633_112_2_18_0_30_34_717 "set_BF3903_anti_flicker: set_banding_mode: mode = %d"
#define SENSOR_BF3903_675_112_2_18_0_30_34_718 "SENSOR: set_BF3903_video_mode: mode = %d"
#define SENSOR_BF3903_760_112_2_18_0_30_34_719 "set_BF3903_awb: mode = %d"
#define SENSOR_BF3903_796_112_2_18_0_30_34_720 "BF3903 set_brightness: level = %d"
#define SENSOR_BF3903_830_112_2_18_0_30_34_721 "BF3903: set_contrast: level = %d"
#define SENSOR_BF3903_858_112_2_18_0_30_34_722 "BF3903 set_preview_mode: preview_mode = %d"
#define SENSOR_BF3903_984_112_2_18_0_30_34_723 "BF3903 set_image_effect: effect_type = %d"
#define SENSOR_BF3903_1095_112_2_18_0_30_35_724 "BF3903 set_work_mode: mode = %d"
#define SENSOR_BF3903_1128_112_2_18_0_30_35_725 "SENSOR: _BF3903_Power_On(1:on, 0:off): %d"
#define SENSOR_BG0323_544_112_2_18_0_30_36_726 "SENSOR: BG0323_WriteReg reg/value(%x,%x) !!"
#define SENSOR_BG0323_560_112_2_18_0_30_36_727 "SENSOR: BG0323_ReadReg reg/value(%x,%x) !!"
#define SENSOR_BG0323_598_112_2_18_0_30_36_728 "BG0323_Identify: it is BG0323"
#define SENSOR_BG0323_636_112_2_18_0_30_36_729 "SENSOR: _bg0323_Power_On(1:on, 0:off): %d"
#define SENSOR_BG0323_659_112_2_18_0_30_36_730 "set_ae_enable: enable = %d"
#define SENSOR_BG0323_697_112_2_18_0_30_36_731 "SENSOR: set_ev: level = %d"
#define SENSOR_BG0323_765_112_2_18_0_30_36_732 "SENSOR: set_video_mode: mode = %d"
#define SENSOR_BG0323_801_112_2_18_0_30_37_733 "set_ae_awb_enable: ae=%d awb=%d"
#define SENSOR_BG0323_814_112_2_18_0_30_37_734 "set_bg0323_hmirror_enable: enable = %d"
#define SENSOR_BG0323_826_112_2_18_0_30_37_735 "set_bg0323_vmirror_enable: enable = %d"
#define SENSOR_BG0323_862_112_2_18_0_30_37_736 "bg0323_set_brightness: level = %d"
#define SENSOR_BG0323_894_112_2_18_0_30_37_737 "bg0323_set_contrast: level = %d"
#define SENSOR_BG0323_922_112_2_18_0_30_37_738 "set_bg0323_preview_mode: preview_mode = %d"
#define SENSOR_BG0323_1117_112_2_18_0_30_37_739 "bg0323_set_image_effect: effect_type = %d"
#define SENSOR_BG0323_1267_112_2_18_0_30_37_740 "set_work_mode: mode = %d"
#define SENSOR_BG0323_1373_112_2_18_0_30_38_741 "BG0323_set_whitebalance_mode: mode = %d"
#define SENSOR_GC0306_418_112_2_18_0_30_39_742 "SENSOR: GC0306_WriteReg reg/value(%x,%x) !!"
#define SENSOR_GC0306_434_112_2_18_0_30_39_743 "SENSOR: GC0306_ReadReg reg/value(%x,%x) !!"
#define SENSOR_GC0306_454_112_2_18_0_30_39_744 "GC0306_Identify"
#define SENSOR_GC0306_464_112_2_18_0_30_39_745 "It is not GC0306"
#define SENSOR_GC0306_479_112_2_18_0_30_39_746 "GC0306_Identify: it is GC0306"
#define SENSOR_GC0306_486_112_2_18_0_30_39_747 "set_gc0306_ae_enable: enable = %d"
#define SENSOR_GC0306_494_112_2_18_0_30_39_748 "set_hmirror_enable: enable = %d"
#define SENSOR_GC0306_503_112_2_18_0_30_39_749 "set_vmirror_enable: enable = %d"
#define SENSOR_GC0306_564_112_2_18_0_30_39_750 "set_brightness: level = %d"
#define SENSOR_GC0306_705_112_2_18_0_30_39_751 "SENSOR: set_ev: level = %d"
#define SENSOR_GC0306_734_112_2_18_0_30_39_752 "SENSOR: set_banding_mode: mode = %d"
#define SENSOR_GC0306_780_112_2_18_0_30_39_753 "SENSOR: set_video_mode: mode = %d"
#define SENSOR_GC0306_865_112_2_18_0_30_40_754 "SENSOR: set_gc0306_awb: mode = %d"
#define SENSOR_GC0306_913_112_2_18_0_30_40_755 "set_gc0306_awb: not supported "
#define SENSOR_GC0306_946_112_2_18_0_30_40_756 "set_contrast: level = %d"
#define SENSOR_GC0306_973_112_2_18_0_30_40_757 "set_preview_mode: preview_mode = %d"
#define SENSOR_GC0306_1137_112_2_18_0_30_40_758 "set_image_effect: not supported!"
#define SENSOR_GC0306_1167_112_2_18_0_30_40_759 "GC0306_BeforeSnapshot "
#define SENSOR_GC0306_1274_112_2_18_0_30_40_760 "set_work_mode: mode = %d"
#define SENSOR_GC0307_931_112_2_18_0_30_42_761 "SENSOR: GC0307_WriteReg reg/value(%x,%x) !!"
#define SENSOR_GC0307_968_112_2_18_0_30_43_762 "GC0307_Identify-START"
#define SENSOR_GC0307_979_112_2_18_0_30_43_763 "GC0307 read ret is %X"
#define SENSOR_GC0307_986_112_2_18_0_30_43_764 "GC0307 Fail"
#define SENSOR_GC0307_1004_112_2_18_0_30_43_765 "GC0307 succ"
#define SENSOR_GC0307_1006_112_2_18_0_30_43_766 "GC0307_Identify-END"
#define SENSOR_GC0307_1099_112_2_18_0_30_43_767 "set_brightness: level = %d"
#define SENSOR_GC0307_1155_112_2_18_0_30_43_768 "set_contrast: level = %d"
#define SENSOR_GC0307_1195_112_2_18_0_30_43_769 "set preview mod %d"
#define SENSOR_GC0307_1401_112_2_18_0_30_44_770 "-----------set_image_effect: effect_type = %d------------"
#define SENSOR_GC0307_1454_112_2_18_0_30_44_771 "SENSOR_GC0307: Before Snapshot"
#define SENSOR_GC0307_1470_112_2_18_0_30_44_772 "SENSOR: After Snapshot"
#define SENSOR_GC0307_1563_112_2_18_0_30_44_773 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_GC0307_1585_112_2_18_0_30_44_774 "SENSOR: set_ae_enable: enable = %d"
#define SENSOR_GC0307_1681_112_2_18_0_30_44_775 "SENSOR: set_video_mode: mode = %d,bl_GC_50Hz_GC0307=%d"
#define SENSOR_GC0307_1717_112_2_18_0_30_44_776 "SENSOR: set_video_mode: mode = %d"
#define SENSOR_GC0307_1750_112_2_18_0_30_44_777 "SENSOR: set_ev: level = %d"
#define SENSOR_GC0307_1806_112_2_18_0_30_44_778 "SENSOR: set_GC0307_AntiFlicker: mode = %d"
#define SENSOR_GC0309_690_112_2_18_0_30_46_779 "SENSOR: GC0309_WriteReg reg/value(%x,%x) !!"
#define SENSOR_GC0309_706_112_2_18_0_30_46_780 "SENSOR: GC0309_ReadReg reg/value(%x,%x) !!"
#define SENSOR_GC0309_725_112_2_18_0_30_46_781 "GC0309_Identify"
#define SENSOR_GC0309_735_112_2_18_0_30_46_782 "It is not GC0309"
#define SENSOR_GC0309_753_112_2_18_0_30_46_783 "GC0309_Identify: it is GC0309"
#define SENSOR_GC0309_760_112_2_18_0_30_46_784 "set_gc0309_ae_enable: enable = %d"
#define SENSOR_GC0309_783_112_2_18_0_30_46_785 "set_gc0309_hmirror_enable: enable = %d"
#define SENSOR_GC0309_808_112_2_18_0_30_46_786 "set_gc0309_vmirror_enable: enable = %d"
#define SENSOR_GC0309_863_112_2_18_0_30_47_787 "set_brightness: level = %d"
#define SENSOR_GC0309_895_112_2_18_0_30_47_788 "SENSOR: set_ev: level = %d"
#define SENSOR_GC0309_923_112_2_18_0_30_47_789 "DEBUG_gc0309 anti flick 50HZ!!!!"
#define SENSOR_GC0309_940_112_2_18_0_30_47_790 "DEBUG_gc0309 anti flick 60HZ!!!!"
#define SENSOR_GC0309_991_112_2_18_0_30_47_791 "SENSOR: set_video_mode: mode = %d"
#define SENSOR_GC0309_1159_112_2_18_0_30_47_792 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_GC0309_1211_112_2_18_0_30_47_793 "set_contrast: level = %d"
#define SENSOR_GC0309_1238_112_2_18_0_30_47_794 "set_preview_mode: preview_mode = %d"
#define SENSOR_GC0309_1256_112_2_18_0_30_47_795 "set_GC0309_preview_mode: level = %d"
#define SENSOR_GC0309_1335_112_2_18_0_30_47_796 "-----------set_image_effect: effect_type = %d------------"
#define SENSOR_GC0309_1362_112_2_18_0_30_48_797 "GC0309_BeforeSnapshot "
#define SENSOR_GC0309_1469_112_2_18_0_30_48_798 "set_work_mode: mode = %d"
#define SENSOR_GC2015_530_112_2_18_0_30_49_799 "set_GC2015_Power_On"
#define SENSOR_GC2015_554_112_2_18_0_30_49_800 "GC2015_Identify-id_h_value %d"
#define SENSOR_GC2015_555_112_2_18_0_30_49_801 "GC2015_Identify-id_l_value %d"
#define SENSOR_GC2015_560_112_2_18_0_30_49_802 "It Is GC2015 Sensor !"
#define SENSOR_GC2015_588_112_2_18_0_30_49_803 "GC0309 WriteReg reg/value(%x,%x) "
#define SENSOR_GC2015_609_112_2_18_0_30_49_804 "GC0309 ReadReg reg/value(%x,%x) "
#define SENSOR_GC2015_704_112_2_18_0_30_49_805 "set_GC2015_brightness: level = %d"
#define SENSOR_GC2015_734_112_2_18_0_30_49_806 "set_GC0309_contrast: level = %d"
#define SENSOR_GC2015_769_112_2_18_0_30_49_807 "set_GC2015_preview_mode: level = %d"
#define SENSOR_GC2015_811_112_2_18_0_30_49_808 "set_GC2015_image_effect: effect_type = %d"
#define SENSOR_GC2015_832_112_2_18_0_30_49_809 "Capture VGA Size"
#define SENSOR_GC2015_840_112_2_18_0_30_49_810 "Capture 1.3M&2M Size"
#define SENSOR_GC2015_855_112_2_18_0_30_49_811 "SENSOR_GC2015: Before Snapshot mode = sensor_snapshot_mode"
#define SENSOR_GC2015_881_112_2_18_0_30_50_812 "SENSOR_GC2015: After Snapshot"
#define SENSOR_GC2015_970_112_2_18_0_30_50_813 "set_GC2015_awb_mode: mode = %d"
#define SENSOR_GC2015_1002_112_2_18_0_30_50_814 "set_GC2015_ev: level = %d"
#define SENSOR_GC2015_1230_112_2_18_0_30_50_815 "set_GC2015_anti_flicker-mode=%d"
#define SENSOR_GC2015_1263_112_2_18_0_30_50_816 "set_GC2015_video_mode=%d"
#define SENSOR_GC2015_1356_112_2_18_0_30_50_817 "SENSOR: set_ae_enable: ae_enable = %d"
#define SENSOR_GC2015_1380_112_2_18_0_30_50_818 "SENSOR: set_awb_enable: awb_enable = %d"
#define SENSOR_GC6113_517_112_2_18_0_30_52_819 "GC6113_WriteReg reg/value(%x,%x) !!"
#define SENSOR_GC6113_525_112_2_18_0_30_52_820 "GC6113_ReadReg: subaddr:%x,value:%x"
#define SENSOR_GC6113_620_112_2_18_0_30_52_821 "SENSOR: GC6113 Read reg0x00 = %x"
#define SENSOR_GC6113_627_112_2_18_0_30_52_822 "GC6113 Fail"
#define SENSOR_GC6113_640_112_2_18_0_30_52_823 "GC6113 Identify"
#define SENSOR_GC6113_709_112_2_18_0_30_52_824 "set_GC6113_brightness: level = %d"
#define SENSOR_GC6113_756_112_2_18_0_30_52_825 "set_GC6113_contrast: level = %d"
#define SENSOR_GC6113_807_112_2_18_0_30_52_826 "set_GC6113_image_effect: effect_type = %d"
#define SENSOR_GC6113_835_112_2_18_0_30_52_827 "set_GC6113_ev: level = %d"
#define SENSOR_GC6113_902_112_2_18_0_30_52_828 "set_GC6113_anti_flicker-mode=%d"
#define SENSOR_GC6113_932_112_2_18_0_30_53_829 "set_GC6113_preview_mode: level = %d"
#define SENSOR_GC6113_1004_112_2_18_0_30_53_830 "set_GC6113_awb_mode: mode = %d"
#define SENSOR_GC6113_1036_112_2_18_0_30_53_831 "SENSOR: set_ae_awb_enable: ae_enable = %d  awb_enable = %d"
#define SENSOR_GC6113_1065_112_2_18_0_30_53_832 "SENSOR_GC6113: Before Snapshot"
#define SENSOR_GC6113_1078_112_2_18_0_30_53_833 "SENSOR_GC6113: After Snapshot"
#define SENSOR_GC6113_1142_112_2_18_0_30_53_834 "set_GC6113_video_mode=%d"
#define SENSOR_GT2005_879_112_2_18_0_30_55_835 "GT2005 WriteReg reg/value(%x,%x) "
#define SENSOR_GT2005_887_112_2_18_0_30_55_836 "GT2005 ReadReg reg/value(%x,%x) "
#define SENSOR_GT2005_927_112_2_18_0_30_55_837 "set_GT2005_Power_On"
#define SENSOR_GT2005_997_112_2_18_0_30_55_838 "set_GT2005_preview_mode: level = %d"
#define SENSOR_GT2005_1011_112_2_18_0_30_56_839 "GT2005_Identify-id_h_value %d"
#define SENSOR_GT2005_1014_112_2_18_0_30_56_840 "GT2005_Identify-id_l_value %d"
#define SENSOR_GT2005_1020_112_2_18_0_30_56_841 "It Is GT2005 Sensor !"
#define SENSOR_GT2005_1061_112_2_18_0_30_56_842 "set_GT2005_brightness: level = %d"
#define SENSOR_GT2005_1088_112_2_18_0_30_56_843 "set_GT2005_contrast: level = %d"
#define SENSOR_GT2005_1124_112_2_18_0_30_56_844 "set_GT2005_image_effect: effect_type = %d"
#define SENSOR_GT2005_1150_112_2_18_0_30_56_845 "set_GT2005_ev: level = %d"
#define SENSOR_GT2005_1188_112_2_18_0_30_56_846 "set_GT2005_anti_flicker-mode=%d"
#define SENSOR_GT2005_1310_112_2_18_0_30_56_847 "set_GT2005_awb_mode: mode = %d"
#define SENSOR_GT2005_1360_112_2_18_0_30_56_848 "SENSOR_GT2005: Before Snapshot"
#define SENSOR_GT2005_1375_112_2_18_0_30_56_849 "SENSOR_GT2005: After Snapshot"
#define SENSOR_GT2005_1450_112_2_18_0_30_56_850 "set_GT2005_video_mode=%d"
#define SENSOR_GT2005_1457_112_2_18_0_30_57_851 "SENSOR: GT2005_InitExifInfo"
#define SENSOR_HI253_1677_112_2_18_0_31_0_852 "SENSOR: HI253_WriteReg reg/value(%x,%x) !!"
#define SENSOR_HI253_1696_112_2_18_0_31_0_853 "SENSOR: HI253_ReadReg reg/value(%x,%x) !!"
#define SENSOR_HI253_1737_112_2_18_0_31_0_854 "HI253_Identify: it is HI253"
#define SENSOR_HI253_1777_112_2_18_0_31_0_855 "SENSOR: _hi253_Power_On(1:on, 0:off): %d"
#define SENSOR_HI253_1802_112_2_18_0_31_0_856 "set_ae_enable: enable = %d"
#define SENSOR_HI253_1833_112_2_18_0_31_0_857 "set_ae_awb_enable: ae=%d awb=%d"
#define SENSOR_HI253_1903_112_2_18_0_31_0_858 "set_hi253_hmirror_enable: enable = %d"
#define SENSOR_HI253_1984_112_2_18_0_31_1_859 "set_hi253_vmirror_enable: enable = %d"
#define SENSOR_HI253_2049_112_2_18_0_31_1_860 "set_brightness: level = %d"
#define SENSOR_HI253_2138_112_2_18_0_31_1_861 "set_hi253_preview_mode: preview_mode = %d"
#define SENSOR_HI253_2268_112_2_18_0_31_1_862 "HI253_set_image_effect: effect_type = %d"
#define SENSOR_HI253_2278_112_2_18_0_31_1_863 "HI253_After_Snapshot "
#define SENSOR_HI253_2294_112_2_18_0_31_1_864 "sensor:HI253_Before_Snapshot"
#define SENSOR_HI253_2306_112_2_18_0_31_1_865 "HI253_Before_Snapshot:dont set any"
#define SENSOR_HI253_2309_112_2_18_0_31_1_866 "sensor:HI253_Before_Snapshot2"
#define SENSOR_HI253_2397_112_2_18_0_31_1_867 "sensor:HI253_set_work_mode: mode = %d"
#define SENSOR_HI253_2489_112_2_18_0_31_2_868 "HI253_set_whitebalance_mode: mode = %d"
#define SENSOR_HI253_2653_112_2_18_0_31_2_869 "SENSOR: set_hi253_video_mode: mode = %d"
#define SENSOR_HI253_2720_112_2_18_0_31_2_870 "SENSOR: set_hi253_ev: level = %d"
#define SENSOR_MT9D112_567_112_2_18_0_31_3_871 "set_mt9d112_mode: level = %d"
#define SENSOR_MT9D112_627_112_2_18_0_31_3_872 "set_brightness: level = %d"
#define SENSOR_MT9D112_1260_112_2_18_0_31_5_873 "set_mt9d112_contrast: level = %d"
#define SENSOR_MT9D112_1373_112_2_18_0_31_5_874 "set_mt9d112_image_effect: effect_type = %d"
#define SENSOR_MT9D112_1459_112_2_18_0_31_5_875 "DCAM: sensor exposure value: 0x%x"
#define SENSOR_MT9D112_1460_112_2_18_0_31_5_876 "DCAM: sensor reg_val_gain1 value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1461_112_2_18_0_31_5_877 "DCAM: sensor reg_val_gain2 value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1462_112_2_18_0_31_5_878 "DCAM: sensor reg_val_gain3 value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1463_112_2_18_0_31_5_879 "DCAM: sensor reg_val_gain4 value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1519_112_2_18_0_31_5_880 "DCAM: sensor close_reg value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1531_112_2_18_0_31_5_881 "DCAM: sensor exp_reg value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1547_112_2_18_0_31_5_882 "DCAM: sensor exp_reg value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1672_112_2_18_0_31_6_883 "DCAM: sensor reg_val_gain1 value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1673_112_2_18_0_31_6_884 "DCAM: sensor reg_val_gain2 value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1674_112_2_18_0_31_6_885 "DCAM: sensor reg_val_gain3 value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1675_112_2_18_0_31_6_886 "DCAM: sensor reg_val_gain1 value: 0x%x, 0x%x"
#define SENSOR_MT9D112_1694_112_2_18_0_31_6_887 "mt9d112: DCAMERA_WB_MODE_INCANDESCENCE"
#define SENSOR_MT9D112_1704_112_2_18_0_31_6_888 "mt9d112: DCAMERA_WB_MODE_FLUORESCENT"
#define SENSOR_MT9D112_1714_112_2_18_0_31_6_889 "mt9d112: DCAMERA_WB_MODE_SUN"
#define SENSOR_MT9D112_1725_112_2_18_0_31_6_890 "mt9d112: DCAMERA_WB_MODE_CLOUD"
#define SENSOR_MT9D112_1736_112_2_18_0_31_6_891 "mt9d112: DCAMERA_WB_MODE_AUTO"
#define SENSOR_MT9D112_1977_112_2_18_0_31_6_892 "MT9D112_Identify: I2C error id = %d"
#define SENSOR_MT9D112_1996_112_2_18_0_31_6_893 "MT9D112_Identify: Succ"
#define SENSOR_MT9D112_2001_112_2_18_0_31_6_894 "MT9D112_Identify: Fail reg_val[0] = 0x%X, [1] = 0x%X"
#define SENSOR_MT9M112_285_112_2_18_0_31_7_895 "reg: 0x%x, write_val: 0x%x"
#define SENSOR_MT9M112_298_112_2_18_0_31_7_896 "SENSOR: Delay %d ms"
#define SENSOR_MT9M112_303_112_2_18_0_31_7_897 "reg: 0x%x, write_val: 0x%x"
#define SENSOR_MT9M112_321_112_2_18_0_31_7_898 "ERROR reg: 0x%x, write: 0x%x, read: 0x%x, temp[0]: 0x%x, temp[1]: 0x%x"
#define SENSOR_MT9M112_341_112_2_18_0_31_7_899 "MT9M112 read_reg reg: 0x%x, val: 0x%x"
#define SENSOR_MT9M112_350_112_2_18_0_31_7_900 "set_ae_enable: enable = %d"
#define SENSOR_MT9M112_357_112_2_18_0_31_7_901 "set_hmirror_enable: enable = %d"
#define SENSOR_MT9M112_364_112_2_18_0_31_7_902 "set_vmirror_enable: enable = %d"
#define SENSOR_MT9M112_377_112_2_18_0_31_7_903 "read_ae_value: %x"
#define SENSOR_MT9M112_383_112_2_18_0_31_7_904 "write_ae_value: %x"
#define SENSOR_MT9M112_392_112_2_18_0_31_7_905 "read_gain_value: "
#define SENSOR_MT9M112_398_112_2_18_0_31_7_906 "write_gain_value: %x"
#define SENSOR_MT9M112_410_112_2_18_0_31_7_907 "set_preview_mode: preview_mode = %d"
#define SENSOR_MT9M112_462_112_2_18_0_31_7_908 "set_brightness: level = %d"
#define SENSOR_MT9M112_469_112_2_18_0_31_7_909 "set_contrast: level = %d"
#define SENSOR_MT9M112_476_112_2_18_0_31_7_910 "set_sharpness: level = %d"
#define SENSOR_MT9M112_483_112_2_18_0_31_7_911 "set_saturation: level = %d"
#define SENSOR_MT9M112_490_112_2_18_0_31_7_912 "-----------set_image_effect: effect_type = %d------------"
#define SENSOR_MT9M112_497_112_2_18_0_31_7_913 "set_frame_rate: param = %d"
#define SENSOR_MT9M112_518_112_2_18_0_31_8_914 "this sensor is MT9M112"
#define SENSOR_MT9M112_523_112_2_18_0_31_8_915 "this sensor is not MT9M112"
#define SENSOR_MT9M112_530_112_2_18_0_31_8_916 "Sensor_Set_ChangeMega_MT9M112"
#define SENSOR_MT9M112_537_112_2_18_0_31_8_917 "MT9M112_BeforeSnapshot: param = %d"
#define SENSOR_NOON010PC20_528_112_2_18_0_31_15_918 "SENSOR: NOON010PC20_WriteReg 0x%x=0x%x"
#define SENSOR_NOON010PC20_543_112_2_18_0_31_15_919 "SENSOR: NOON010PC20_WriteReg 0x%x=0x%x"
#define SENSOR_NOON010PC20_554_112_2_18_0_31_15_920 "NOON010PC20_Identify: it is NOON010PC20"
#define SENSOR_NOON010PC20_562_112_2_18_0_31_15_921 "This is not NOON010PC20 sensor"
#define SENSOR_NOON010PC20_566_112_2_18_0_31_15_922 "NOON010PC20_Identify: it is NOON010PC20"
#define SENSOR_NOON010PC20_585_112_2_18_0_31_15_923 "set_hmirror_enable: enable = %d"
#define SENSOR_NOON010PC20_597_112_2_18_0_31_15_924 "set_vmirror_enable: enable = %d"
#define SENSOR_NOON010PC20_633_112_2_18_0_31_15_925 "set_brightness: level = %d"
#define SENSOR_NOON010PC20_665_112_2_18_0_31_15_926 "set_contrast: level = %d"
#define SENSOR_NOON010PC20_693_112_2_18_0_31_15_927 "set_preview_mode: preview_mode = %d"
#define SENSOR_NOON010PC20_787_112_2_18_0_31_15_928 "set_image_effect: effect_type = %d"
#define SENSOR_NOON010PC20_898_112_2_18_0_31_16_929 "noon010pc20_set_iso: iso_type = %d"
#define SENSOR_NOON010PC20_953_112_2_18_0_31_16_930 "OV2640_set_wb_mode: wb_mode = %d"
#define SENSOR_NOON010PC20_987_112_2_18_0_31_16_931 "noon010pc20_set_flicker_elimination_mode: flicker_mode = %d"
#define SENSOR_NOON010PC20_1021_112_2_18_0_31_16_932 "noon010pc20_ev_compensation: ev_mode = %d"
#define SENSOR_NOON010PC20_1057_112_2_18_0_31_16_933 "set_work_mode: mode = %d"
#define SENSOR_NOON010PC30L_596_112_2_18_0_31_17_934 "SENSOR: _noon010pc30l_Power_On  power_on =0x%x"
#define SENSOR_NOON010PC30L_652_112_2_18_0_31_18_935 "SENSOR: NOON010PC30L_WriteReg 0x%x=0x%x"
#define SENSOR_NOON010PC30L_667_112_2_18_0_31_18_936 "SENSOR: noon010pc30l_ReadReg 0x%x=0x%x"
#define SENSOR_NOON010PC30L_680_112_2_18_0_31_18_937 "NOON010PC30L_Identify: it is NOON010PC30L ret=%d"
#define SENSOR_NOON010PC30L_685_112_2_18_0_31_18_938 "NOON010PC30L_Identify: it is NOON010PC30L 0x%x after read reg"
#define SENSOR_NOON010PC30L_689_112_2_18_0_31_18_939 "This is not NOON010PC30L sensor"
#define SENSOR_NOON010PC30L_693_112_2_18_0_31_18_940 "NOON010PC30L_Identify: it is NOON010PC30L"
#define SENSOR_NOON010PC30L_712_112_2_18_0_31_18_941 "set_hmirror_enable: enable = %d"
#define SENSOR_NOON010PC30L_724_112_2_18_0_31_18_942 "set_vmirror_enable: enable = %d"
#define SENSOR_NOON010PC30L_785_112_2_18_0_31_18_943 "set_brightness: level = %d"
#define SENSOR_NOON010PC30L_817_112_2_18_0_31_18_944 "set_contrast: level = %d"
#define SENSOR_NOON010PC30L_845_112_2_18_0_31_18_945 "set_preview_mode: preview_mode = %d"
#define SENSOR_NOON010PC30L_958_112_2_18_0_31_18_946 "set_image_effect: effect_type = %d"
#define SENSOR_NOON010PC30L_1069_112_2_18_0_31_18_947 "noon010pc30l_set_iso: iso_type = %d"
#define SENSOR_NOON010PC30L_1174_112_2_18_0_31_19_948 "OV2640_set_wb_mode: wb_mode = %d"
#define SENSOR_NOON010PC30L_1208_112_2_18_0_31_19_949 "noon010pc30l_set_flicker_elimination_mode: flicker_mode = %d"
#define SENSOR_NOON010PC30L_1242_112_2_18_0_31_19_950 "noon010pc30l_ev_compensation: ev_mode = %d"
#define SENSOR_NOON010PC30L_1282_112_2_18_0_31_19_951 "noon010pc30l_set_meter_mode: mode = %d, rect(%d,%d,%d,%d)"
#define SENSOR_NOON010PC30L_1319_112_2_18_0_31_19_952 "set_work_mode: mode = %d"
#define SENSOR_NT99040_579_112_2_18_0_31_20_953 "SENSOR: set_NT99040_flicker: 0x%x"
#define SENSOR_NT99040_678_112_2_18_0_31_21_954 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_NT99040_717_112_2_18_0_31_21_955 "SENSOR: set_ev: level = %d"
#define SENSOR_NT99040_778_112_2_18_0_31_21_956 "set_preview_mode: preview_mode = %d"
#define SENSOR_NT99040_909_112_2_18_0_31_21_957 "Reg0x3012=0x%x"
#define SENSOR_NT99040_910_112_2_18_0_31_21_958 "Reg0x3013=0x%x"
#define SENSOR_NT99040_1058_112_2_18_0_31_21_959 "Reg0x3000=%d"
#define SENSOR_NT99040_1059_112_2_18_0_31_21_960 "g_PV_Shutter=%d"
#define SENSOR_NT99040_1060_112_2_18_0_31_22_961 "g_PV_Extra_Lines=%d"
#define SENSOR_NT99040_1061_112_2_18_0_31_22_962 "PV_Line_Width=%d"
#define SENSOR_NT99040_1062_112_2_18_0_31_22_963 "Capture_Line_Width=%d"
#define SENSOR_NT99040_1095_112_2_18_0_31_22_964 "PV_Line_Width=%d"
#define SENSOR_NT99040_1096_112_2_18_0_31_22_965 "Capture_Line_Width=%d"
#define SENSOR_NT99040_1097_112_2_18_0_31_22_966 "Capture_Maximum_Shutter=%d"
#define SENSOR_NT99040_1098_112_2_18_0_31_22_967 "Capture_Banding_Filter=%d"
#define SENSOR_NT99040_1099_112_2_18_0_31_22_968 "g_PV_Gain16=%d"
#define SENSOR_NT99040_1100_112_2_18_0_31_22_969 "Gain_Exposure=%d"
#define SENSOR_NT99040_1106_112_2_18_0_31_22_970 "if (Gain_Exposure < Capture_Banding_Filter * 16)"
#define SENSOR_NT99040_1113_112_2_18_0_31_22_971 "Gain_Exposure<16 Capture_Gain16=%d"
#define SENSOR_NT99040_1120_112_2_18_0_31_22_972 "Gain_Exposure<16 else---- Capture_Gain16=%d"
#define SENSOR_NT99040_1126_112_2_18_0_31_22_973 "if (Gain_Exposure < Capture_Banding_Filter * 16)  else--------"
#define SENSOR_NT99040_1143_112_2_18_0_31_22_974 "Gain_Exposure<16 Capture_Gain16=%d"
#define SENSOR_NT99040_1152_112_2_18_0_31_22_975 "Gain_Exposure<16 else---- Capture_Gain16=%d"
#define SENSOR_NT99040_1216_112_2_18_0_31_22_976 "set_sharpness: level = %d"
#define SENSOR_NT99040_1266_112_2_18_0_31_22_977 "set_saturation: level = %d"
#define SENSOR_NT99040_1346_112_2_18_0_31_22_978 "set_image_effect: effect_type = %d"
#define SENSOR_NT99040_1399_112_2_18_0_31_22_979 "set_brightness: reg3391 = %d"
#define SENSOR_NT99040_1410_112_2_18_0_31_22_980 "set_brightness: level = %d"
#define SENSOR_NT99040_1459_112_2_18_0_31_22_981 "set_contrast: reg3391 = %d"
#define SENSOR_NT99040_1469_112_2_18_0_31_22_982 "set_contrast: level = %d"
#define SENSOR_NT99040_1517_112_2_18_0_31_23_983 "set_work_mode: mode = %d"
#define SENSOR_NT99250_669_112_2_18_0_31_24_984 "SENSOR: set_NT99250_flicker: 0x%x"
#define SENSOR_NT99250_768_112_2_18_0_31_25_985 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_NT99250_807_112_2_18_0_31_25_986 "SENSOR: set_ev: level = %d"
#define SENSOR_NT99250_868_112_2_18_0_31_25_987 "set_preview_mode: preview_mode = %d"
#define SENSOR_NT99250_999_112_2_18_0_31_25_988 "Reg0x3012=0x%x"
#define SENSOR_NT99250_1000_112_2_18_0_31_25_989 "Reg0x3013=0x%x"
#define SENSOR_NT99250_1148_112_2_18_0_31_25_990 "Reg0x3000=%d"
#define SENSOR_NT99250_1149_112_2_18_0_31_25_991 "g_PV_Shutter=%d"
#define SENSOR_NT99250_1150_112_2_18_0_31_25_992 "g_PV_Extra_Lines=%d"
#define SENSOR_NT99250_1151_112_2_18_0_31_25_993 "PV_Line_Width=%d"
#define SENSOR_NT99250_1152_112_2_18_0_31_25_994 "Capture_Line_Width=%d"
#define SENSOR_NT99250_1185_112_2_18_0_31_25_995 "PV_Line_Width=%d"
#define SENSOR_NT99250_1186_112_2_18_0_31_25_996 "Capture_Line_Width=%d"
#define SENSOR_NT99250_1187_112_2_18_0_31_25_997 "Capture_Maximum_Shutter=%d"
#define SENSOR_NT99250_1188_112_2_18_0_31_25_998 "Capture_Banding_Filter=%d"
#define SENSOR_NT99250_1189_112_2_18_0_31_25_999 "g_PV_Gain16=%d"
#define SENSOR_NT99250_1190_112_2_18_0_31_25_1000 "Gain_Exposure=%d"
#define SENSOR_NT99250_1196_112_2_18_0_31_25_1001 "if (Gain_Exposure < Capture_Banding_Filter * 16)"
#define SENSOR_NT99250_1203_112_2_18_0_31_25_1002 "Gain_Exposure<16 Capture_Gain16=%d"
#define SENSOR_NT99250_1210_112_2_18_0_31_25_1003 "Gain_Exposure<16 else---- Capture_Gain16=%d"
#define SENSOR_NT99250_1216_112_2_18_0_31_25_1004 "if (Gain_Exposure < Capture_Banding_Filter * 16)  else--------"
#define SENSOR_NT99250_1233_112_2_18_0_31_25_1005 "Gain_Exposure<16 Capture_Gain16=%d"
#define SENSOR_NT99250_1242_112_2_18_0_31_25_1006 "Gain_Exposure<16 else---- Capture_Gain16=%d"
#define SENSOR_NT99250_1306_112_2_18_0_31_26_1007 "set_sharpness: level = %d"
#define SENSOR_NT99250_1356_112_2_18_0_31_26_1008 "set_saturation: level = %d"
#define SENSOR_NT99250_1436_112_2_18_0_31_26_1009 "set_image_effect: effect_type = %d"
#define SENSOR_NT99250_1489_112_2_18_0_31_26_1010 "set_brightness: reg3391 = %d"
#define SENSOR_NT99250_1500_112_2_18_0_31_26_1011 "set_brightness: level = %d"
#define SENSOR_NT99250_1549_112_2_18_0_31_26_1012 "set_contrast: reg3391 = %d"
#define SENSOR_NT99250_1559_112_2_18_0_31_26_1013 "set_contrast: level = %d"
#define SENSOR_NT99250_1607_112_2_18_0_31_26_1014 "set_work_mode: mode = %d"
#define SENSOR_OV2640_664_112_2_18_0_31_28_1015 "OV2640_set_ae_enable: enable = %d"
#define SENSOR_OV2640_703_112_2_18_0_31_28_1016 "SENSOR: set_ov2640_flicker: 0x%x"
#define SENSOR_OV2640_960_112_2_18_0_31_28_1017 "SENSOR: set_vodeo_mode: mode : %d"
#define SENSOR_OV2640_1057_112_2_18_0_31_29_1018 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_OV2640_1115_112_2_18_0_31_29_1019 "SENSOR: set_ev: level = %d"
#define SENSOR_OV2640_1181_112_2_18_0_31_29_1020 "set_preview_mode: preview_mode = %d"
#define SENSOR_OV2640_1239_112_2_18_0_31_29_1021 "That is OV2640 sensor !"
#define SENSOR_OV2640_1243_112_2_18_0_31_29_1022 "OV2640_Identify: That is OV%x%x sensor !"
#define SENSOR_OV2640_1249_112_2_18_0_31_29_1023 "OV2640_Identify: PID = %x, VER = %x"
#define SENSOR_OV2640_1287_112_2_18_0_31_29_1024 "ov2640: prvGain %d, prvExp %d"
#define SENSOR_OV2640_1310_112_2_18_0_31_29_1025 "ov2640: PrvGain %d, TgtExp %d"
#define SENSOR_OV2640_1454_112_2_18_0_31_29_1026 "set_brightness: level = %d"
#define SENSOR_OV2640_1561_112_2_18_0_31_30_1027 "set_contrast: level = %d"
#define SENSOR_OV2640_1593_112_2_18_0_31_30_1028 "set_sharpness: level = %d"
#define SENSOR_OV2640_1653_112_2_18_0_31_30_1029 "set_saturation: level = %d"
#define SENSOR_OV2640_1715_112_2_18_0_31_30_1030 "set_image_effect: effect_type = %d"
#define SENSOR_OV2640_1764_112_2_18_0_31_30_1031 "set_work_mode: mode = %d"
#define SENSOR_OV2640_1825_112_2_18_0_31_30_1032 "ov3640: jpeg capture head: 0x%x, 0x%x"
#define SENSOR_OV2640_1842_112_2_18_0_31_30_1033 "ov3640: Found the jpeg tail at %d: 0x%x 0x%x"
#define SENSOR_OV2640_1845_112_2_18_0_31_30_1034 "ov3640: can not find the jpeg tail: %d"
#define SENSOR_OV2640_2026_112_2_18_0_31_30_1035 "OV2640_zoom: mode=%d,level = %d"
#define SENSOR_OV2655_645_112_2_18_0_31_32_1036 "OV2655_set_ae_Rot: %d"
#define SENSOR_OV2655_703_112_2_18_0_31_32_1037 "SENSOR: set_ov2655_flicker: 0x%x"
#define SENSOR_OV2655_797_112_2_18_0_31_32_1038 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_OV2655_831_112_2_18_0_31_32_1039 "SENSOR: set_ev: level = %d"
#define SENSOR_OV2655_847_112_2_18_0_31_32_1040 "OV2655_set_hmirror_enable: enable = %d"
#define SENSOR_OV2655_918_112_2_18_0_31_32_1041 "OV2655_set_hmirror_enable: enable = %d"
#define SENSOR_OV2655_984_112_2_18_0_31_33_1042 "set_preview_mode: preview_mode = %d"
#define SENSOR_OV2655_1038_112_2_18_0_31_33_1043 "OV2655_Identify: ret = %d"
#define SENSOR_OV2655_1111_112_2_18_0_31_33_1044 "Reg0x3002=0x%x"
#define SENSOR_OV2655_1112_112_2_18_0_31_33_1045 "Reg0x3003=0x%x"
#define SENSOR_OV2655_1229_112_2_18_0_31_33_1046 "set_sharpness: level = %d"
#define SENSOR_OV2655_1279_112_2_18_0_31_33_1047 "set_saturation: level = %d"
#define SENSOR_OV2655_1359_112_2_18_0_31_33_1048 "set_image_effect: effect_type = %d"
#define SENSOR_OV2655_1413_112_2_18_0_31_33_1049 "set_brightness: reg3391 = %d"
#define SENSOR_OV2655_1437_112_2_18_0_31_33_1050 "set_brightness: level = %d"
#define SENSOR_OV2655_1486_112_2_18_0_31_34_1051 "set_contrast: reg3391 = %d"
#define SENSOR_OV2655_1503_112_2_18_0_31_34_1052 "set_contrast: level = %d"
#define SENSOR_OV2655_1587_112_2_18_0_31_34_1053 "set_work_mode: mode = %d"
#define SENSOR_OV2655_1596_112_2_18_0_31_34_1054 "SENSOR: OV2655_after_snapshot =%d"
#define SENSOR_OV2655_1650_112_2_18_0_31_34_1055 "SENSOR: OV2655_InitExifInfo"
#define SENSOR_OV2659_619_112_2_18_0_31_36_1056 "SENSOR: set_ov2659_flicker: 0x%x"
#define SENSOR_OV2659_692_112_2_18_0_31_36_1057 "SENSOR: set_awb_mode: mode = %d,ret = %d"
#define SENSOR_OV2659_727_112_2_18_0_31_36_1058 "SENSOR: set_ev: level = %d"
#define SENSOR_OV2659_792_112_2_18_0_31_36_1059 "set_preview_mode: preview_mode = %d"
#define SENSOR_OV2659_850_112_2_18_0_31_36_1060 "OV2659_Identify: ret = %d"
#define SENSOR_OV2659_907_112_2_18_0_31_36_1061 "OV2659Core_Get_ExposureValue()\r\n"
#define SENSOR_OV2659_918_112_2_18_0_31_36_1062 "linetp = %ld\r\n"
#define SENSOR_OV2659_931_112_2_18_0_31_36_1063 "shutter = %ld\r\n"
#define SENSOR_OV2659_940_112_2_18_0_31_36_1064 "gain16 = %d\r\n"
#define SENSOR_OV2659_949_112_2_18_0_31_36_1065 "m_dbExposure = %f\r\n"
#define SENSOR_OV2659_961_112_2_18_0_31_36_1066 "OV2659Core_Set_ExposureValue()\r\n"
#define SENSOR_OV2659_975_112_2_18_0_31_36_1067 "linetp = %d\r\n"
#define SENSOR_OV2659_989_112_2_18_0_31_36_1068 "framelines = %d\r\n"
#define SENSOR_OV2659_992_112_2_18_0_31_36_1069 "m_dbExposure = %lf\r\n"
#define SENSOR_OV2659_996_112_2_18_0_31_36_1070 "exposuretp = %lf, before adjust\r\n"
#define SENSOR_OV2659_997_112_2_18_0_31_37_1071 "exposuretp = %lf, after adjust, m_gain_preview=%x\r\n"
#define SENSOR_OV2659_1018_112_2_18_0_31_37_1072 "bandinglines = %d\r\n"
#define SENSOR_OV2659_1019_112_2_18_0_31_37_1073 "lightfreq = %d\r\n"
#define SENSOR_OV2659_1023_112_2_18_0_31_37_1074 "smaller than 1 line\r\n"
#define SENSOR_OV2659_1030_112_2_18_0_31_37_1075 "smaller than 1 banding, bandinglines=%d\r\n"
#define SENSOR_OV2659_1036_112_2_18_0_31_37_1076 "smaller than 1 frame\r\n"
#define SENSOR_OV2659_1043_112_2_18_0_31_37_1077 "larger than 1 frame\r\n"
#define SENSOR_OV2659_1048_112_2_18_0_31_37_1078 "larger than maxim gain & frame\r\n"
#define SENSOR_OV2659_1055_112_2_18_0_31_37_1079 "shutter = %ld\r\n"
#define SENSOR_OV2659_1056_112_2_18_0_31_37_1080 "gain16 = %d\r\n"
#define SENSOR_OV2659_1059_112_2_18_0_31_37_1081 "m_gain_still = %x\r\n"
#define SENSOR_OV2659_1194_112_2_18_0_31_37_1082 "set_saturation: level = %d,ret = %d"
#define SENSOR_OV2659_1287_112_2_18_0_31_37_1083 "set_image_effect: effect_type = %d,ret = %d"
#define SENSOR_OV2659_1430_112_2_18_0_31_37_1084 "set_contrast: level = %d,ret = %d"
#define SENSOR_OV2659_1489_112_2_18_0_31_38_1085 "set_work_mode: mode = %d"
#define SENSOR_OV2659_1547_112_2_18_0_31_38_1086 "SENSOR: OV2659_InitExifInfo"
#define SENSOR_OV3640_3797_112_2_18_0_31_46_1087 "set_preview_mode: preview_mode = %d"
#define SENSOR_OV3640_3841_112_2_18_0_31_46_1088 "ov3640_Identify---------------\n"
#define SENSOR_OV3640_3845_112_2_18_0_31_46_1089 "ov3640_Identify: PID = %x, "
#define SENSOR_OV3640_3848_112_2_18_0_31_46_1090 "ov3640_Identify: VER = %x, "
#define SENSOR_OV3640_3851_112_2_18_0_31_46_1091 "It's 36xx sensor."
#define SENSOR_OV3640_3854_112_2_18_0_31_46_1092 "ov3640_Identify: That is OV%x%x sensor !"
#define SENSOR_OV3640_3902_112_2_18_0_31_46_1093 "SENSOR: OV3640_set_brightness = 0x%02x"
#define SENSOR_OV3640_3948_112_2_18_0_31_46_1094 "SENSOR: OV3640_set_contrast = 0x%02x"
#define SENSOR_OV3640_4003_112_2_18_0_31_46_1095 "SENSOR: OV3640_set_saturation = 0x%02x"
#define SENSOR_OV3640_4060_112_2_18_0_31_47_1096 "SENSOR: OV3640_set_image_effect = 0x%02x"
#define SENSOR_OV3640_4106_112_2_18_0_31_47_1097 "SENSOR: OV3640_set_ev = 0x%02x"
#define SENSOR_OV3640_4135_112_2_18_0_31_47_1098 "SENSOR: OV3640_set_anti_flicker = 0x%02x"
#define SENSOR_OV3640_4432_112_2_18_0_31_47_1099 "SENSOR: OV3640_set_video_mode = 0x%02x"
#define SENSOR_OV3640_4486_112_2_18_0_31_47_1100 "SENSOR: OV3640_set_awb = 0x%02x"
#define SENSOR_OV3640_4529_112_2_18_0_31_47_1101 "set_work_mode: mode = %d"
#define SENSOR_OV3640_4633_112_2_18_0_31_48_1102 "ov3640: jpeg capture head: 0x%x, 0x%x"
#define SENSOR_OV3640_4650_112_2_18_0_31_48_1103 "ov3640: Found the jpeg tail at %d: 0x%x 0x%x"
#define SENSOR_OV3640_4653_112_2_18_0_31_48_1104 "ov3640: can not find the jpeg tail: %d"
#define SENSOR_OV3640_4723_112_2_18_0_31_48_1105 "step1 reg0x3013 0x%x"
#define SENSOR_OV3640_4726_112_2_18_0_31_48_1106 "step1 reg0x3013 0x%x"
#define SENSOR_OV3640_4732_112_2_18_0_31_48_1107 "step2 reg0x3002 0x%x  0x3003=0x%x shutter=0x%x"
#define SENSOR_OV3640_4739_112_2_18_0_31_48_1108 "step3 reg0x302d=0x%x; 0x302e=0x%x"
#define SENSOR_OV3640_4740_112_2_18_0_31_48_1109 "step3 extra_lines=%d; preview_exposure=%d"
#define SENSOR_OV3640_4745_112_2_18_0_31_48_1110 "step4 reg0x3001=0x%x Preview_Gain16=%d"
#define SENSOR_OV3640_4752_112_2_18_0_31_48_1111 "step5 0x3028=0x%x reg0x3029=0x%x"
#define SENSOR_OV3640_4753_112_2_18_0_31_48_1112 "step5 Preview_dummy_pixels=%d"
#define SENSOR_OV3640_4763_112_2_18_0_31_48_1113 "step5 preview=%d  capture=%d"
#define SENSOR_OV3640_4768_112_2_18_0_31_48_1114 "step6 capture_max_gain=%d  Capture_max_gain16=%d"
#define SENSOR_OV3640_4773_112_2_18_0_31_48_1115 "step7 Preview_line_width=%d"
#define SENSOR_OV3640_4776_112_2_18_0_31_48_1116 "step 7 Capture_line_width=%d"
#define SENSOR_OV3640_4779_112_2_18_0_31_48_1117 "step8 Capture_Exposure=%d"
#define SENSOR_OV3640_4785_112_2_18_0_31_48_1118 "step9 Capture_banding_Filter=%d"
#define SENSOR_OV3640_4789_112_2_18_0_31_48_1119 "step9 max_shutter=%d"
#define SENSOR_OV3640_4793_112_2_18_0_31_48_1120 "step10 Gain_Exposure=%d"
#define SENSOR_OV3640_4798_112_2_18_0_31_48_1121 "step11 Capture_Exposure=%d  Capture_Gain16=%d"
#define SENSOR_OV3640_4805_112_2_18_0_31_48_1122 "step12 Capture_Exposure=%d  Capture_Gain16=%d"
#define SENSOR_OV3640_4814_112_2_18_0_31_48_1123 "step13 Capture_Exposure=%d  Capture_Gain16=%d"
#define SENSOR_OV3640_4822_112_2_18_0_31_48_1124 "step14 Capture_Exposure=%d  Capture_Gain16=%d"
#define SENSOR_OV3640_4842_112_2_18_0_31_48_1125 "step15 reg0x3029=0x%x"
#define SENSOR_OV3640_4846_112_2_18_0_31_48_1126 "step16 reg0x3028=0x%x"
#define SENSOR_OV3640_4856_112_2_18_0_31_48_1127 "step17 0x302a=0x%x  0x302b=0x%x"
#define SENSOR_OV3640_4867_112_2_18_0_31_48_1128 "step18 shutter=%d  extra_lines=%d"
#define SENSOR_OV3640_4873_112_2_18_0_31_48_1129 "step19 0x3003=0x%x  0x3002=0x%x"
#define SENSOR_OV3640_4880_112_2_18_0_31_48_1130 "step19 0x302d=0x%x  0x302e=0x%x"
#define SENSOR_OV3640_4909_112_2_18_0_31_48_1131 "step20 0x3001=0x%x"
#define SENSOR_OV3640_4912_112_2_18_0_31_48_1132 "step20 0x3001=0x%x"
#define SENSOR_OV5640_4427_112_2_18_0_31_57_1133 "SENSOR: _ov5640_InitExifInfo"
#define SENSOR_OV5640_4544_112_2_18_0_31_57_1134 "SENSOR: _ov5640_Power_On(1:on, 0:off): %d"
#define SENSOR_OV5640_4571_112_2_18_0_31_57_1135 "SENSOR: ov5640_Identify: PID = %x, VER = %x"
#define SENSOR_OV5640_4576_112_2_18_0_31_57_1136 "SENSOR: this is ov5640 sensor !"
#define SENSOR_OV5640_4580_112_2_18_0_31_57_1137 "SENSOR: ov5640_Identify this is OV%x%x sensor !"
#define SENSOR_OV5640_4631_112_2_18_0_31_58_1138 "SENSOR: _ov5640_set_brightness = 0x%02x"
#define SENSOR_OV5640_4679_112_2_18_0_31_58_1139 "SENSOR: _ov5640_set_contrast = 0x%02x"
#define SENSOR_OV5640_4734_112_2_18_0_31_58_1140 "SENSOR: _ov5640_set_saturation = 0x%02x"
#define SENSOR_OV5640_4791_112_2_18_0_31_58_1141 "SENSOR: _ov5640_set_image_effect = 0x%02x"
#define SENSOR_OV5640_4837_112_2_18_0_31_58_1142 "SENSOR: _ov5640_set_ev = 0x%02x"
#define SENSOR_OV5640_4866_112_2_18_0_31_58_1143 "SENSOR: _ov5640_set_anti_flicker = 0x%02x"
#define SENSOR_OV5640_5079_112_2_18_0_31_58_1144 "SENSOR: _ov5640_set_video_mode = 0x%02x"
#define SENSOR_OV5640_5135_112_2_18_0_31_59_1145 "SENSOR: _ov5640_set_awb = 0x%02x"
#define SENSOR_OV5640_5179_112_2_18_0_31_59_1146 "SENSOR: _ov5640_set_work_mode: mode = %d"
#define SENSOR_OV5640_5303_112_2_18_0_31_59_1147 "SENSOR: ov5640 jpeg capture head: 0x%x, 0x%x"
#define SENSOR_OV5640_5323_112_2_18_0_31_59_1148 "SENSOR: ov5640 Found the jpeg tail at %d: 0x%x 0x%x"
#define SENSOR_OV5640_5327_112_2_18_0_31_59_1149 "SENSOR: ov5640 can not find the jpeg tail: %d"
#define SENSOR_OV5640_5346_112_2_18_0_31_59_1150 "SENSOR: ov5640  chang_image_format  YUV422"
#define SENSOR_OV5640_5351_112_2_18_0_31_59_1151 "SENSOR: ov5640  chang_image_format  jpg"
#define SENSOR_OV5640_5369_112_2_18_0_31_59_1152 "SENSOR: _ov5640_after_snapshot =%d"
#define SENSOR_OV5640_5392_112_2_18_0_31_59_1153 "SENSOR: ov5640_after_snapshot =%d"
#define SENSOR_OV5640_5446_112_2_18_0_31_59_1154 "SENSOR: _ov5640_LoadAfFirmware i2c close error"
#define SENSOR_OV5640_5451_112_2_18_0_31_59_1155 "SENSOR: _ov5640_LoadAfFirmware i2c close success"
#define SENSOR_OV5640_5456_112_2_18_0_31_59_1156 "SENSOR: _ov5640_LoadAfFirmware i2c init error"
#define SENSOR_OV5640_5519_112_2_18_0_31_59_1157 "SENSOR: _ov5640_MatchZone, w:%d, h:%d error"
#define SENSOR_OV5640_5523_112_2_18_0_31_59_1158 "SENSOR: _ov5640_MatchZone, x:%d, y:%d"
#define SENSOR_OV5640_5539_112_2_18_0_31_59_1159 "SENSOR: _ov5640_AutoFocusTrig"
#define SENSOR_OV5640_5548_112_2_18_0_31_59_1160 "SENSOR: _ov5640_AutoFocusTrig error!"
#define SENSOR_OV5640_5562_112_2_18_0_31_59_1161 "SENSOR: _ov5640_AutoFocusTrig fail!"
#define SENSOR_OV5640_5582_112_2_18_0_31_59_1162 "SENSOR: _ov5640_AutoFocusZone: %d, %d"
#define SENSOR_OV5640_5603_112_2_18_0_32_0_1163 "SENSOR: _ov5640_AutoFocusZone error!"
#define SENSOR_OV5640_5627_112_2_18_0_32_0_1164 "SENSOR: _ov5640_StartAutoFocus param =%d"
#define SENSOR_OV5640_5657_112_2_18_0_32_0_1165 "SENSOR: _ov5640_ExposureAuto"
#define SENSOR_OV5640_5675_112_2_18_0_32_0_1166 "SENSOR: _ov5640_ExposureZone: %d, %d"
#define SENSOR_OV5640_5711_112_2_18_0_32_0_1167 "SENSOR: _ov5640_ExposureZone: %d, %d, %d, %d"
#define SENSOR_OV5640_5750_112_2_18_0_32_0_1168 "SENSOR: _ov5640_StartExposure param =%d"
#define SENSOR_OV5640_5779_112_2_18_0_32_0_1169 "SENSOR: _ov5640_ExtFunc cmd:0x%x "
#define SENSOR_OV7670_703_112_2_18_0_32_2_1170 "SENSOR: OV7670_Identify: it is OV7670"
#define SENSOR_OV7670_718_112_2_18_0_32_2_1171 "SENSOR: _ov7670_InitExifInfo"
#define SENSOR_OV7670_851_112_2_18_0_32_2_1172 "SENSOR: _ov7670_Power_On(1:on, 0:off): %d"
#define SENSOR_OV7670_873_112_2_18_0_32_2_1173 "SENSOR: set_ae_enable: enable = %d"
#define SENSOR_OV7670_905_112_2_18_0_32_2_1174 "SENSOR: set_ae_awb_enable: ae=%d awb=%d"
#define SENSOR_OV7670_915_112_2_18_0_32_2_1175 "set_hmirror_enable: enable = %d"
#define SENSOR_OV7670_924_112_2_18_0_32_2_1176 "set_vmirror_enable: enable = %d"
#define SENSOR_OV7670_954_112_2_18_0_32_2_1177 "SENSOR: set_ev: level = %d"
#define SENSOR_OV7670_985_112_2_18_0_32_2_1178 "SENSOR: set_banding_mode: mode = %d"
#define SENSOR_OV7670_1070_112_2_18_0_32_2_1179 "SENSOR: set_video_mode: mode = %d"
#define SENSOR_OV7670_1170_112_2_18_0_32_2_1180 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_OV7670_1204_112_2_18_0_32_3_1181 "SENSOR: set_brightness: level = %d"
#define SENSOR_OV7670_1238_112_2_18_0_32_3_1182 "SENSOR: set_contrast: level = %d"
#define SENSOR_OV7670_1266_112_2_18_0_32_3_1183 "SENSOR: set_preview_mode: preview_mode = %d"
#define SENSOR_OV7670_1355_112_2_18_0_32_3_1184 "SENSOR: set_image_effect: effect_type = %d"
#define SENSOR_OV7670_1363_112_2_18_0_32_3_1185 "SENSOR: OV7670_BeforeSnapshot: OV7670_After_Snapshot "
#define SENSOR_OV7670_1409_112_2_18_0_32_3_1186 "SENSOR: OV7670_BeforeSnapshot: OV7670_BeforeSnapshot "
#define SENSOR_OV7670_1421_112_2_18_0_32_3_1187 "unknown Camera mode!"
#define SENSOR_OV7670_1424_112_2_18_0_32_3_1188 "OV7670_BeforeSnapshot max_line= %d"
#define SENSOR_OV7670_1481_112_2_18_0_32_3_1189 "OV7670_BeforeSnapshot exposal= %d"
#define SENSOR_OV7670_1620_112_2_18_0_32_3_1190 "SENSOR: set_work_mode: mode = %d"
#define SENSOR_OV7670_1639_112_2_18_0_32_3_1191 "OV7670_set_ae_Rot: %d"
#define SENSOR_OV7675_701_112_2_18_0_32_5_1192 "SENSOR: OV7675_Identify: it is OV7675"
#define SENSOR_OV7675_714_112_2_18_0_32_5_1193 "SENSOR: _ov7675_InitExifInfo"
#define SENSOR_OV7675_818_112_2_18_0_32_5_1194 "SENSOR: _ov7675_Power_On(1:on, 0:off): %d"
#define SENSOR_OV7675_841_112_2_18_0_32_5_1195 "SENSOR: set_ae_enable: enable = %d"
#define SENSOR_OV7675_873_112_2_18_0_32_5_1196 "SENSOR: set_ae_awb_enable: ae=%d awb=%d"
#define SENSOR_OV7675_883_112_2_18_0_32_5_1197 "set_hmirror_enable: enable = %d"
#define SENSOR_OV7675_892_112_2_18_0_32_5_1198 "set_vmirror_enable: enable = %d"
#define SENSOR_OV7675_923_112_2_18_0_32_6_1199 "SENSOR: set_ev: level = %d"
#define SENSOR_OV7675_954_112_2_18_0_32_6_1200 "SENSOR: set_banding_mode: mode = %d"
#define SENSOR_OV7675_1036_112_2_18_0_32_6_1201 "SENSOR: set_video_mode: mode = %d"
#define SENSOR_OV7675_1136_112_2_18_0_32_6_1202 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_OV7675_1170_112_2_18_0_32_6_1203 "SENSOR: set_brightness: level = %d"
#define SENSOR_OV7675_1203_112_2_18_0_32_6_1204 "SENSOR: set_contrast: level = %d"
#define SENSOR_OV7675_1231_112_2_18_0_32_6_1205 "SENSOR: set_preview_mode: preview_mode = %d"
#define SENSOR_OV7675_1320_112_2_18_0_32_6_1206 "SENSOR: set_image_effect: effect_type = %d"
#define SENSOR_OV7675_1328_112_2_18_0_32_6_1207 "SENSOR: OV7675_BeforeSnapshot: OV7675_After_Snapshot "
#define SENSOR_OV7675_1374_112_2_18_0_32_6_1208 "SENSOR: OV7675_BeforeSnapshot: OV7675_BeforeSnapshot "
#define SENSOR_OV7675_1386_112_2_18_0_32_6_1209 "unknown Camera mode!"
#define SENSOR_OV7675_1389_112_2_18_0_32_6_1210 "OV7675_BeforeSnapshot max_line= %d"
#define SENSOR_OV7675_1440_112_2_18_0_32_7_1211 "OV7675_BeforeSnapshot exposal= %d"
#define SENSOR_OV7675_1586_112_2_18_0_32_7_1212 "SENSOR: set_work_mode: mode = %d"
#define SENSOR_OV7690_402_112_2_18_0_32_8_1213 "SENSOR: OV7690_WriteReg reg/value(%x,%x) !!"
#define SENSOR_OV7690_418_112_2_18_0_32_8_1214 "SENSOR: OV7690_ReadReg reg/value(%x,%x) !!"
#define SENSOR_OV7690_464_112_2_18_0_32_8_1215 "OV7690_Identify: it is OV7690"
#define SENSOR_OV7690_500_112_2_18_0_32_8_1216 "SENSOR: _OV7690_Power_On(1:on, 0:off): %d"
#define SENSOR_OV7690_523_112_2_18_0_32_8_1217 "set_ae_enable: enable = %d"
#define SENSOR_OV7690_555_112_2_18_0_32_8_1218 "set_ae_awb_enable: ae=%d awb=%d"
#define SENSOR_OV7690_567_112_2_18_0_32_8_1219 "set_OV7690_rot: %d"
#define SENSOR_OV7690_596_112_2_18_0_32_8_1220 "set_OV7690_hmirror_enable: enable = %d"
#define SENSOR_OV7690_651_112_2_18_0_32_8_1221 "set_OV7690_vmirror_enable: enable = %d"
#define SENSOR_OV7690_720_112_2_18_0_32_8_1222 "OV7690_set_brightness: level = %d"
#define SENSOR_OV7690_725_112_2_18_0_32_8_1223 "OV7690_set_brightness: 0x0c = %d"
#define SENSOR_OV7690_850_112_2_18_0_32_9_1224 "OV7690_set_contrast: level = %d"
#define SENSOR_OV7690_982_112_2_18_0_32_9_1225 "set_OV7690_preview_mode: preview_mode = %d"
#define SENSOR_OV7690_1220_112_2_18_0_32_9_1226 "OV7690_set_image_effect: effect_type = %d"
#define SENSOR_OV7690_1230_112_2_18_0_32_9_1227 "OV7690_BeforeSnapshot: OV7690_After_Snapshot "
#define SENSOR_OV7690_1313_112_2_18_0_32_9_1228 "OV7690_set_work_mode: mode = %d"
#define SENSOR_OV7690_1414_112_2_18_0_32_10_1229 "OV7690_set_whitebalance_mode: mode = %d"
#define SENSOR_OV7690_1522_112_2_18_0_32_10_1230 "SENSOR: set_ov7690_video_mode: mode = %d"
#define SENSOR_OV7690_1549_112_2_18_0_32_10_1231 "SENSOR: set_ov7690_ev: level = %d"
#define SENSOR_OV9655_549_112_2_18_0_32_11_1232 "set_ae_enable: enable = %d"
#define SENSOR_OV9655_565_112_2_18_0_32_11_1233 "set_hmirror_enable: enable = %d"
#define SENSOR_OV9655_582_112_2_18_0_32_11_1234 "set_vmirror_enable: enable = %d"
#define SENSOR_OV9655_606_112_2_18_0_32_11_1235 "read_ae_value: %x"
#define SENSOR_OV9655_616_112_2_18_0_32_11_1236 "write_ae_value: %x"
#define SENSOR_OV9655_655_112_2_18_0_32_12_1237 "read_gain_value: %x"
#define SENSOR_OV9655_665_112_2_18_0_32_12_1238 "write_gain_value: %x"
#define SENSOR_OV9655_719_112_2_18_0_32_12_1239 "set_preview_mode: preview_mode = %d"
#define SENSOR_OV9655_985_112_2_18_0_32_12_1240 "set_brightness: level = %d"
#define SENSOR_OV9655_1004_112_2_18_0_32_12_1241 "set_contrast: level = %d"
#define SENSOR_OV9655_1025_112_2_18_0_32_12_1242 "set_sharpness: level = %d"
#define SENSOR_OV9655_1046_112_2_18_0_32_12_1243 "set_saturation: level = %d"
#define SENSOR_OV9655_1066_112_2_18_0_32_12_1244 "-----------set_image_effect: effect_type = %d------------"
#define SENSOR_OV9655_1075_112_2_18_0_32_12_1245 "set_frame_rate: param = %d"
#define SENSOR_OV9655_1113_112_2_18_0_32_13_1246 "That is ov9655 sensor !"
#define SENSOR_OV9655_1117_112_2_18_0_32_13_1247 "ov9655_Identify: That is OV%x%x sensor !"
#define SENSOR_OV9655_1123_112_2_18_0_32_13_1248 "ov9655_Identify: PID = %x, VER = %x"
#define SENSOR_OV9655_1142_112_2_18_0_32_13_1249 "SENSOR ov9655 : caculate_expo PREVIEW MODE %d"
#define SENSOR_OV9655_1267_112_2_18_0_32_13_1250 "ov9655_BeforeSnapshot: param = %d"
#define SENSOR_OV9660_542_112_2_18_0_32_14_1251 "OV9660_set_ae_enable: enable = %d"
#define SENSOR_OV9660_581_112_2_18_0_32_14_1252 "SENSOR: set_OV9660_flicker: 0x%x"
#define SENSOR_OV9660_673_112_2_18_0_32_14_1253 "SENSOR: set_vodeo_mode: %d "
#define SENSOR_OV9660_763_112_2_18_0_32_15_1254 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_OV9660_801_112_2_18_0_32_15_1255 "SENSOR: set_ev: level = %d"
#define SENSOR_OV9660_867_112_2_18_0_32_15_1256 "set_preview_mode: preview_mode = %d"
#define SENSOR_OV9660_934_112_2_18_0_32_15_1257 "That is OV9660 sensor !"
#define SENSOR_OV9660_938_112_2_18_0_32_15_1258 "OV9660_Identify: That is OV%x%x sensor !"
#define SENSOR_OV9660_944_112_2_18_0_32_15_1259 "OV9660_Identify: PID = %x, VER = %x"
#define SENSOR_OV9660_987_112_2_18_0_32_15_1260 "OV9660: prvGain %d, prvExp %d"
#define SENSOR_OV9660_1020_112_2_18_0_32_15_1261 "OV9660: PrvGain %d, TgtExp %d"
#define SENSOR_OV9660_1083_112_2_18_0_32_15_1262 "set_brightness: level = %d"
#define SENSOR_OV9660_1114_112_2_18_0_32_15_1263 "set_contrast: level = %d"
#define SENSOR_OV9660_1146_112_2_18_0_32_15_1264 "set_sharpness: level = %d"
#define SENSOR_OV9660_1206_112_2_18_0_32_15_1265 "set_saturation: level = %d"
#define SENSOR_OV9660_1276_112_2_18_0_32_16_1266 "set_image_effect: effect_type = %d"
#define SENSOR_OV9660_1329_112_2_18_0_32_16_1267 "set_work_mode: mode = %d"
#define SENSOR_S5K5CAGX_3878_112_2_18_0_32_27_1268 "SENSOR: _s5k5cagx_InitExifInfo"
#define SENSOR_S5K5CAGX_3971_112_2_18_0_32_27_1269 "SENSOR: _s5k5cagx_PowerOn(1:on, 0:off): Entry"
#define SENSOR_S5K5CAGX_3975_112_2_18_0_32_27_1270 "SENSOR: _s5k5cagx_PowerOn"
#define SENSOR_S5K5CAGX_3978_112_2_18_0_32_27_1271 "SENSOR: _s5k5cagx_PowerOn dvdd/avdd/iovdd = {%d/%d/%d}"
#define SENSOR_S5K5CAGX_3997_112_2_18_0_32_27_1272 "SENSOR: _s5k5cagx_PowerDown"
#define SENSOR_S5K5CAGX_4011_112_2_18_0_32_27_1273 "SENSOR: _s5k5cagx_Power_On(1:on, 0:off): %d"
#define SENSOR_S5K5CAGX_4012_112_2_18_0_32_27_1274 "SENSOR: _s5k5cagx_PowerOn(1:on, 0:off): End"
#define SENSOR_S5K5CAGX_4036_112_2_18_0_32_27_1275 "s5k5cagx_Identify Entry"
#define SENSOR_S5K5CAGX_4038_112_2_18_0_32_27_1276 "s5k5cagx_Identify 0xD0001006 PID = 0x%x"
#define SENSOR_S5K5CAGX_4043_112_2_18_0_32_27_1277 "s5k5cagx_Identify Entry"
#define SENSOR_S5K5CAGX_4045_112_2_18_0_32_27_1278 "s5k5cagx_Identify 0x00000040 PID = 0x%x"
#define SENSOR_S5K5CAGX_4049_112_2_18_0_32_27_1279 "That is not s5k5cagx sensor ! error!"
#define SENSOR_S5K5CAGX_4057_112_2_18_0_32_27_1280 "s5k5cagx_Identify: PID = %x, VER = %x"
#define SENSOR_S5K5CAGX_4062_112_2_18_0_32_27_1281 "That is s5k5cagx sensor !"
#define SENSOR_S5K5CAGX_4066_112_2_18_0_32_27_1282 "s5k5cagx_Identify: That is OV%x%x sensor !"
#define SENSOR_S5K5CAGX_4071_112_2_18_0_32_27_1283 "That is s5k5cagx sensor !"
#define SENSOR_S5K5CAGX_4072_112_2_18_0_32_27_1284 "s5k5cagx_Identify End"
#define SENSOR_S5K5CAGX_4120_112_2_18_0_32_27_1285 "SENSOR: s5k5cagx_set_brightness = 0x%02x"
#define SENSOR_S5K5CAGX_4168_112_2_18_0_32_28_1286 "SENSOR: s5k5cagx_set_contrast = 0x%02x"
#define SENSOR_S5K5CAGX_4216_112_2_18_0_32_28_1287 "SENSOR: s5k5cagx_set_sharpness = 0x%02x"
#define SENSOR_S5K5CAGX_4261_112_2_18_0_32_28_1288 "SENSOR: s5k5cagx_set_saturation = 0x%02x"
#define SENSOR_S5K5CAGX_4422_112_2_18_0_32_28_1289 "SENSOR: s5k5cagx_set_image_effect = 0x%02x"
#define SENSOR_S5K5CAGX_4469_112_2_18_0_32_28_1290 "SENSOR: s5k5cagx_set_ev = 0x%02x"
#define SENSOR_S5K5CAGX_4553_112_2_18_0_32_28_1291 "SENSOR: s5k5cagx_set_awb = 0x%02x"
#define SENSOR_S5K5CAGX_4617_112_2_18_0_32_28_1292 "SENSOR: set_work_mode: mode = %d"
#define SENSOR_S5K5CAGX_4629_112_2_18_0_32_28_1293 "_s5k5cagx_BeforeSnapshot =%d"
#define SENSOR_S5K5CAGX_4655_112_2_18_0_32_29_1294 "s5k5cagx only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d"
#define SENSOR_S5K5CAGX_4674_112_2_18_0_32_29_1295 "s5k5cagx: jpeg capture head: 0x%x, 0x%x"
#define SENSOR_S5K5CAGX_4695_112_2_18_0_32_29_1296 "s5k5cagx: Found the jpeg tail at %d: 0x%x 0x%x"
#define SENSOR_S5K5CAGX_4699_112_2_18_0_32_29_1297 "s5k5cagx: can not find the jpeg tail: %d"
#define SENSOR_S5K5CAGX_4719_112_2_18_0_32_29_1298 "s5k5cagx  chang_image_format  YUV422"
#define SENSOR_S5K5CAGX_4724_112_2_18_0_32_29_1299 "s5k5cagx  chang_image_format  jpg"
#define SENSOR_S5K5CAGX_4787_112_2_18_0_32_29_1300 "_s5k5cagx_after_snapshot =%d"
#define SENSOR_S5K5CAGX_4817_112_2_18_0_32_29_1301 "SENSOR: _s5k5cagx_InitExt"
#define SENSOR_S5K5CAGX_4832_112_2_18_0_32_29_1302 "SENSOR: _s5k5cagx_InitExt, i2c handler creat err !"
#define SENSOR_S5K5CAGX_4885_112_2_18_0_32_29_1303 "SENSOR: _s5k5cagx_InitExt, i2c write once error"
#define SENSOR_S5K5CAGX_4892_112_2_18_0_32_29_1304 "SENSOR: _s5k5cagx_InitExt, i2c write once from %d {0x%x 0x%x}, total %d registers {0x%x 0x%x},time %d"
#define SENSOR_S5K5CAGX_4897_112_2_18_0_32_29_1305 "SENSOR: _s5k5cagx_InitExt, reg,{0x%x 0x%x} val {0x%x 0x%x} {0x%x 0x%x} {0x%x 0x%x}"
#define SENSOR_S5K5CAGX_4907_112_2_18_0_32_29_1306 "SENSOR: _s5k5cagx_InitExt, i2c close error"
#define SENSOR_S5K5CAGX_4912_112_2_18_0_32_29_1307 "SENSOR: _s5k5cagx_InitExt, i2c close success"
#define SENSOR_S5K5CAGX_4915_112_2_18_0_32_29_1308 "SENSOR: _s5k5cagx_InitExt, success"
#define SENSOR_S5K5CAGX_4932_112_2_18_0_32_29_1309 "SENSOR: _s5k5cagx_ExtFunc cmd:0x%x "
#define SENSOR_S5K5CAGX_4944_112_2_18_0_32_29_1310 "SENSOR: _s5k5cagx_ExtFunc unsupported command"
#define SENSOR_S5KA3DFX_844_112_2_18_0_32_31_1311 "SENSOR: _s5ka3dfx_Power_On  power_on =0x%x"
#define SENSOR_S5KA3DFX_896_112_2_18_0_32_31_1312 "SENSOR: S5KA3DFX_WriteReg 0x%x=0x%x"
#define SENSOR_S5KA3DFX_911_112_2_18_0_32_31_1313 "SENSOR: S5KA3DFX_WriteReg 0x%x=0x%x"
#define SENSOR_S5KA3DFX_923_112_2_18_0_32_31_1314 "S5KA3DFX_Identify: it is S5KA3DFX"
#define SENSOR_S5KA3DFX_928_112_2_18_0_32_31_1315 "S5KA3DFX_Identify: it is S5KA3DFX 0x%x after read reg"
#define SENSOR_S5KA3DFX_932_112_2_18_0_32_31_1316 "This is not S5KA3DFX sensor"
#define SENSOR_S5KA3DFX_936_112_2_18_0_32_31_1317 "S5KA3DFX_Identify: it is S5KA3DFX"
#define SENSOR_S5KA3DFX_955_112_2_18_0_32_31_1318 "set_hmirror_enable: enable = %d"
#define SENSOR_S5KA3DFX_967_112_2_18_0_32_31_1319 "set_vmirror_enable: enable = %d"
#define SENSOR_S5KA3DFX_1051_112_2_18_0_32_32_1320 "set_brightness: level = %d"
#define SENSOR_S5KA3DFX_1083_112_2_18_0_32_32_1321 "set_contrast: level = %d"
#define SENSOR_S5KA3DFX_1111_112_2_18_0_32_32_1322 "set_preview_mode: preview_mode = %d"
#define SENSOR_S5KA3DFX_1227_112_2_18_0_32_32_1323 "set_image_effect: effect_type = %d"
#define SENSOR_S5KA3DFX_1338_112_2_18_0_32_32_1324 "s5ka3dfx_set_iso: iso_type = %d"
#define SENSOR_S5KA3DFX_1423_112_2_18_0_32_32_1325 "OV2640_set_wb_mode: wb_mode = %d"
#define SENSOR_S5KA3DFX_1457_112_2_18_0_32_32_1326 "s5ka3dfx_set_flicker_elimination_mode: flicker_mode = %d"
#define SENSOR_S5KA3DFX_1491_112_2_18_0_32_33_1327 "s5ka3dfx_ev_compensation: ev_mode = %d"
#define SENSOR_S5KA3DFX_1530_112_2_18_0_32_33_1328 "s5ka3dfx_set_meter_mode: mode = %d, rect(%d,%d,%d,%d)"
#define SENSOR_S5KA3DFX_1684_112_2_18_0_32_33_1329 "set_work_mode: mode = %d"
#define SENSOR_SIV100A_204_112_2_18_0_32_33_1330 "SENSOR: SIV100A_WriteReg reg/value(%x,%x) !!"
#define SENSOR_SIV100A_247_112_2_18_0_32_33_1331 "SIV100A read ret is %X"
#define SENSOR_SIV100A_254_112_2_18_0_32_33_1332 "SIV100A Fail"
#define SENSOR_SIV100A_269_112_2_18_0_32_33_1333 "SIV100A succ"
#define SENSOR_SIV100A_323_112_2_18_0_32_34_1334 "set_brightness: level = %d"
#define SENSOR_SIV100A_379_112_2_18_0_32_34_1335 "set_contrast: level = %d"
#define SENSOR_SIV100A_412_112_2_18_0_32_34_1336 "set_preview_mode: preview_mode = %d"
#define SENSOR_SIV100A_476_112_2_18_0_32_34_1337 "-----------set_image_effect: effect_type = %d------------"
#define SENSOR_SIV100B_206_112_2_18_0_32_35_1338 "SENSOR: SIV100B_WriteReg reg/value(%x,%x) !!"
#define SENSOR_SIV100B_219_112_2_18_0_32_35_1339 "SIV100B read ret is %X=%X"
#define SENSOR_SIV100B_249_112_2_18_0_32_35_1340 "SIV100B read ret is %X"
#define SENSOR_SIV100B_256_112_2_18_0_32_35_1341 "SIV100B Fail"
#define SENSOR_SIV100B_271_112_2_18_0_32_35_1342 "SIV100B succ"
#define SENSOR_SIV100B_339_112_2_18_0_32_35_1343 "set_brightness: level = %d"
#define SENSOR_SIV100B_393_112_2_18_0_32_35_1344 "set_contrast: level = %d"
#define SENSOR_SIV100B_491_112_2_18_0_32_35_1345 "set_contrast: level = %d"
#define SENSOR_SIV100B_517_112_2_18_0_32_35_1346 "SIV100B: DCAMERA_WB_MODE_INCANDESCENCE"
#define SENSOR_SIV100B_526_112_2_18_0_32_35_1347 "SIV100B: DCAMERA_WB_MODE_FLUORESCENT"
#define SENSOR_SIV100B_535_112_2_18_0_32_35_1348 "SIV100B: DCAMERA_WB_MODE_SUN"
#define SENSOR_SIV100B_545_112_2_18_0_32_35_1349 "SIV100B: DCAMERA_WB_MODE_CLOUD"
#define SENSOR_SIV100B_555_112_2_18_0_32_35_1350 "SIV100B: DCAMERA_WB_MODE_AUTO"
#define SENSOR_SIV100B_603_112_2_18_0_32_35_1351 "siv100b_set_video_mode 1"
#define SENSOR_SIV100B_615_112_2_18_0_32_35_1352 "siv100b_set_video_mode 0"
#define SENSOR_SIV100B_639_112_2_18_0_32_35_1353 "set_preview_mode 1"
#define SENSOR_SIV100B_651_112_2_18_0_32_35_1354 "set_preview_mode 0"
#define SENSOR_SIV100B_804_112_2_18_0_32_36_1355 "-----------set_image_effect: effect_type = %d------------"
#define SENSOR_SIV120B_385_112_2_18_0_32_37_1356 "SENSOR: siv120b_WriteReg reg/value(%x,%x) !!"
#define SENSOR_SIV120B_421_112_2_18_0_32_37_1357 "siv120b_Identify"
#define SENSOR_SIV120B_433_112_2_18_0_32_37_1358 "siv120b read ret is %X"
#define SENSOR_SIV120B_440_112_2_18_0_32_37_1359 "siv120b Fail"
#define SENSOR_SIV120B_455_112_2_18_0_32_37_1360 "siv120b succ"
#define SENSOR_SIV120B_531_112_2_18_0_32_37_1361 "set_brightness: level = %d"
#define SENSOR_SIV120B_595_112_2_18_0_32_37_1362 "set_contrast: level = %d"
#define SENSOR_SIV120B_693_112_2_18_0_32_38_1363 "set_ev: level = %d"
#define SENSOR_SIV120B_741_112_2_18_0_32_38_1364 "siv120b: DCAMERA_WB_MODE_INCANDESCENCE"
#define SENSOR_SIV120B_750_112_2_18_0_32_38_1365 "siv120b: DCAMERA_WB_MODE_FLUORESCENT"
#define SENSOR_SIV120B_759_112_2_18_0_32_38_1366 "siv120b: DCAMERA_WB_MODE_SUN"
#define SENSOR_SIV120B_769_112_2_18_0_32_38_1367 "siv120b: DCAMERA_WB_MODE_CLOUD"
#define SENSOR_SIV120B_780_112_2_18_0_32_38_1368 "siv120b: DCAMERA_WB_MODE_AUTO"
#define SENSOR_SIV120B_1022_112_2_18_0_32_38_1369 "-----------set_image_effect: effect_type = %d------------"
#define SENSOR_SIV120B_1079_112_2_18_0_32_39_1370 "siv120b_before_snapshot"
#define SENSOR_SIV120B_1210_112_2_18_0_32_39_1371 " siv120b_before_snapshot 0x2C = %0X"
#define SENSOR_SIV120B_1211_112_2_18_0_32_39_1372 " siv120b_before_snapshot 0x2D = %0X"
#define SENSOR_SIV120B_1212_112_2_18_0_32_39_1373 " siv120b_before_snapshot 0x2E = %0X"
#define SENSOR_SIV120B_1213_112_2_18_0_32_39_1374 " siv120b_before_snapshot 0x2F = %0X"
#define SENSOR_SP0838_469_112_2_18_0_32_40_1375 "SENSOR: SP0838_WriteReg reg/value(%x,%x) !!"
#define SENSOR_SP0838_485_112_2_18_0_32_40_1376 "SENSOR: SP0838_ReadReg reg/value(%x,%x) !!"
#define SENSOR_SP0838_506_112_2_18_0_32_40_1377 "SP0838_Identify"
#define SENSOR_SP0838_516_112_2_18_0_32_40_1378 "It is not SP0838"
#define SENSOR_SP0838_531_112_2_18_0_32_40_1379 "SP0838_Identify: it is SP0838"
#define SENSOR_SP0838_544_112_2_18_0_32_40_1380 "SP0838_Identify: ret = %d"
#define SENSOR_SP0838_552_112_2_18_0_32_40_1381 "set_SP0838_ae_enable: enable = %d"
#define SENSOR_SP0838_560_112_2_18_0_32_40_1382 "set_hmirror_enable: enable = %d"
#define SENSOR_SP0838_569_112_2_18_0_32_40_1383 "set_vmirror_enable: enable = %d"
#define SENSOR_SP0838_644_112_2_18_0_32_41_1384 "set_brightness: level = %d"
#define SENSOR_SP0838_724_112_2_18_0_32_41_1385 "SENSOR: set_ev: level = %d"
#define SENSOR_SP0838_1119_112_2_18_0_32_42_1386 "SENSOR: set_awb_mode: mode = %d"
#define SENSOR_SP0838_1193_112_2_18_0_32_42_1387 "set_contrast: level = %d"
#define SENSOR_SP0838_1220_112_2_18_0_32_42_1388 "set_preview_mode: preview_mode = %d"
#define SENSOR_SP0838_1477_112_2_18_0_32_42_1389 "-----------set_image_effect: effect_type = %d------------"
#define SENSOR_SP0838_1504_112_2_18_0_32_42_1390 "SP0838_BeforeSnapshot "
#define SENSOR_SP0838_1632_112_2_18_0_32_43_1391 "set_work_mode: mode = %d"
#define FM_DRV_108_112_2_18_0_32_43_1392 "FM_SetStatus: status = %d, s_fm_status = %d"
#define FM_DRV_150_112_2_18_0_32_44_1393 "FM_Entersleep"
#define FM_DRV_179_112_2_18_0_32_44_1394 "FM_Init"
#define FM_DRV_184_112_2_18_0_32_44_1395 "FM_Init: Don't need to Initailize again!!!"
#define FM_DRV_218_112_2_18_0_32_44_1396 "FM_Init2: Fail to init FM!!"
#define FM_DRV_237_112_2_18_0_32_44_1397 "FM_Init1: Fail to init FM!!"
#define FM_DRV_253_112_2_18_0_32_44_1398 "FM_Open"
#define FM_DRV_256_112_2_18_0_32_44_1399 "FM not init !!"
#define FM_DRV_276_112_2_18_0_32_44_1400 "FM_Start: freq = %d"
#define FM_DRV_280_112_2_18_0_32_44_1401 "FM not init !!"
#define FM_DRV_300_112_2_18_0_32_44_1402 "FM_PlayStop"
#define FM_DRV_303_112_2_18_0_32_44_1403 "FM_PlayStop: not init, Don't need to stop"
#define FM_DRV_320_112_2_18_0_32_44_1404 "FM_Close"
#define FM_DRV_325_112_2_18_0_32_44_1405 "FM_Close: not init, Don't need to Close"
#define FM_DRV_353_112_2_18_0_32_44_1406 "FM_ManualSeek: freq = %d"
#define FM_DRV_357_112_2_18_0_32_44_1407 "FM not init !!"
#define FM_DRV_378_112_2_18_0_32_44_1408 "FM_AutoSeek: seek_dir = %d, mode = %d, freq = %d"
#define FM_DRV_382_112_2_18_0_32_44_1409 "FM not init !!"
#define FM_DRV_400_112_2_18_0_32_44_1410 "FM_SetScanLevel: level = %d"
#define FM_DRV_403_112_2_18_0_32_44_1411 "FM not init !!"
#define FM_DRV_435_112_2_18_0_32_44_1412 "FM not init !!"
#define FM_CL6012X_145_112_2_18_0_32_45_1413 "[FM_DRV]CL6012X Reg[%d]= 0x%02x"
#define FM_CL6012X_157_112_2_18_0_32_45_1414 "CL6012X_Config"
#define FM_CL6012X_188_112_2_18_0_32_45_1415 "----CL6012X_Initial----"
#define FM_CL6012X_194_112_2_18_0_32_45_1416 "FM::==> open IIC failed!\r\n"
#define FM_CL6012X_252_112_2_18_0_32_45_1417 "CL6012X_PlayStop()"
#define FM_CL6012X_279_112_2_18_0_32_45_1418 "CL6012X_Close()"
#define FM_CL6012X_342_112_2_18_0_32_46_1419 "CL6012X_ManualSeek enter:freq = %d"
#define FM_CL6012X_360_112_2_18_0_32_46_1420 "[FM_DRV]CL6012X_ManualSeek: freq = %d, signal_strength = %d, "
#define FM_CL6012X_449_112_2_18_0_32_46_1421 "[FM_DRV]CL6012X_AutoSeek: stc= %d, sff= %d, "
#define FM_CL6012X_482_112_2_18_0_32_46_1422 "--CL6012X_SetVol:vol_level=%d---"
#define FM_KT0812G_128_112_2_18_0_32_47_1423 "[DRV_FM]==> open IIC failed!\r\n"
#define FM_KT0812G_160_112_2_18_0_32_47_1424 "[DRV_FM] _Write_One_Reg: [0x%02X] = 0x%04X "
#define FM_KT0812G_180_112_2_18_0_32_47_1425 "[DRV_FM] _Read_One_Reg: [0x%02X] = 0x%04X "
#define FM_KT0812G_199_112_2_18_0_32_47_1426 "[DRV_FM] KT0812G_ReadAllReg: [0x%02X]=0x%04X "
#define FM_KT0812G_217_112_2_18_0_32_47_1427 "[DRV_FM] KT0812G_Initial: i2c port = %d"
#define FM_KT0812G_243_112_2_18_0_32_47_1428 "[DRV_FM] KT0812G_FMTune: STATUSA = 0x%04X, i = %d"
#define FM_KT0812G_280_112_2_18_0_32_47_1429 "[DRV_FM] KT0812G_Open"
#define FM_KT0812G_293_112_2_18_0_32_47_1430 "[DRV_FM] KT0812G_Close"
#define FM_KT0812G_329_112_2_18_0_32_47_1431 "[DRV_FM] KT0812G_FMTune: at TUNE = 0x%04X "
#define FM_KT0812G_334_112_2_18_0_32_47_1432 "[DRV_FM] KT0812G_FMTune: After TUNE = 0x%04X "
#define FM_KT0812G_335_112_2_18_0_32_47_1433 "[DRV_FM] KT0812G_FMTune: STATUSA = 0x%04X "
#define FM_KT0812G_336_112_2_18_0_32_47_1434 "[DRV_FM] KT0812G_FMTune: READ CHAN= 0x%04X "
#define FM_KT0812G_354_112_2_18_0_32_47_1435 "[DRV_FM] KT0812G_GetTuneFreq: CHAN= 0x%04X, Freq = %d "
#define FM_KT0812G_411_112_2_18_0_32_47_1436 "[DRV_FM] KT0812G_PlayStart: Freq = %d "
#define FM_KT0812G_426_112_2_18_0_32_47_1437 "[DRV_FM] KT0812G_PlayStop"
#define FM_KT0812G_453_112_2_18_0_32_47_1438 "[DRV_FM]KT0812G_ManualSeek: freq = %d, rssi = %d, stereo = %d"
#define FM_KT0812G_519_112_2_18_0_32_47_1439 "[DRV_FM]KT0812G_AutoSeek: seek_dir = %d, seek_end= %d, seek_fail= %d, "
#define FM_KT0812G_536_112_2_18_0_32_47_1440 "[DRV_FM] KT0812G_SetScanLevel"
#define FM_KT0812G_571_112_2_18_0_32_47_1441 "[DRV_FM] KT0812G_SetVol = %d, s_cur_vol = %d, RegVol = %02X"
#define FM_KT0812G_588_112_2_18_0_32_47_1442 "[DRV_FM] KT0812G_SetFMRegion"
#define FM_KT0812G_598_112_2_18_0_32_47_1443 "[DRV_FM] KT0812G_SetStereo = %d"
#define FM_LV2400X_273_112_2_18_0_32_48_1444 "FM: SetScanLevel %d"
#define LV2400X_225_112_2_18_0_32_49_1445 "Set_IF_Freq 0x%x,g_wLastMsr=0X%x\n"
#define LV2400X_229_112_2_18_0_32_49_1446 "InitTuningRfCapOsc 0x%x\n"
#define LV2400X_234_112_2_18_0_32_49_1447 "SetRegion 0x%x\n"
#define LV2400X_238_112_2_18_0_32_49_1448 "Set_SD_Freq 0x%x,g_wLastMsr=0x%x\n"
#define LV2400X_286_112_2_18_0_32_49_1449 "Set_IF_Freq 0x%x,g_wLastMsr=0X%x\n"
#define LV2400X_291_112_2_18_0_32_49_1450 "InitTuningRfCapOsc 0x%x\n"
#define LV2400X_296_112_2_18_0_32_49_1451 "SetRegion 0x%x\n"
#define LV2400X_300_112_2_18_0_32_49_1452 "Set_SD_Freq 0x%x,g_wLastMsr=0x%x\n"
#define LV2400X_305_112_2_18_0_32_49_1453 "InitLv2400xChip: chip id -> %x"
#define LV2400X_2012_112_2_18_0_32_53_1454 "chip id1 = 0x%x"
#define LV2400X_2035_112_2_18_0_32_53_1455 "chip id = 0x%x"
#define RADIOSUB_352_112_2_18_0_32_55_1456 "scan set freq error = %d\r\n"
#define RADIOSUB_361_112_2_18_0_32_55_1457 "scan set freq ok = %d\r\n"
#define RADIOSUB_460_112_2_18_0_32_55_1458 "field strength = %d\r\n"
#define RADIOSUB_461_112_2_18_0_32_55_1459 "g_wLastMsrRF = %d\r\n"
#define TEA5761_135_112_2_18_0_32_55_1460 "TEA5761_Calculate_Frequency()  return=%d"
#define TEA5761_147_112_2_18_0_32_55_1461 "    TEA5761_Calculate_PLL_Value(freq=%d)  return = 0x%04x    "
#define TEA5761_208_112_2_18_0_32_55_1462 "---TEA5761_WaitLDSet(): time out error---"
#define TEA5761_211_112_2_18_0_32_55_1463 "---TEA5761_WaitLDSet(): return ok---"
#define TEA5761_219_112_2_18_0_32_55_1464 "----TEA5761_CorrectFreq: freq=%d----"
#define TEA5761_233_112_2_18_0_32_56_1465 "--TEA5761_CorrectFreq(return freq=%d)---"
#define TEA5761_286_112_2_18_0_32_56_1466 "----TEA5761_Preset(%d)----"
#define TEA5761_356_112_2_18_0_32_56_1467 "--if_count=%d----"
#define TEA5761_394_112_2_18_0_32_56_1468 "----TEA5761_Initial(freq=%d)----"
#define TEA5761_433_112_2_18_0_32_56_1469 "---Tea5761_search(%d)---"
#define TEA5761_466_112_2_18_0_32_56_1470 "---if_count[1]=%d----"
#define TEA5761_477_112_2_18_0_32_56_1471 "---if_count[2]=%d----"
#define TEA5761_499_112_2_18_0_32_56_1472 "---Tea5761_Search...return----"
#define TEA5761_505_112_2_18_0_32_56_1473 "TEA5761_palystart()"
#define TEA5761_517_112_2_18_0_32_56_1474 "TEA5761_palystop()"
#define TEA5761_530_112_2_18_0_32_56_1475 "TEA5761_Open()"
#define TEA5761_537_112_2_18_0_32_56_1476 "TEA5761_close()"
#define TUNEALG_308_112_2_18_0_32_57_1477 "SetUpChipMode() return 0x%x\n"
#define TUNEALG_312_112_2_18_0_32_57_1478 "GetRfAtCapOsc() g_wHwRfLow 0x%x\n"
#define TUNEALG_315_112_2_18_0_32_57_1479 "GetRfAtCapOsc() g_wHwRfHigh 0x%x\n"
#define TUNEALG_327_112_2_18_0_32_57_1480 "Qssf1.CoefLo 0x%x,Qssf1.CoefHi 0x%x\n"
#define TUNEALG_328_112_2_18_0_32_57_1481 "Qssf2.CoefLo 0x%x,Qssf2.CoefHi 0x%x\n"
#define GPIO_EXT_DRV_189_112_2_18_0_32_58_1482 "_GPIO_EXT_ReadReg: ic %d hasn't existed"
#define GPIO_EXT_DRV_218_112_2_18_0_32_58_1483 "_GPIO_EXT_WriteReg: ic %d hasn't existed"
#define GPIO_EXT_DRV_232_112_2_18_0_32_58_1484 "_GPIO_EXT_UpdateInputValue: ic_num = %d"
#define GPIO_EXT_DRV_236_112_2_18_0_32_58_1485 "port0 = %02x, port1 = %02x"
#define GPIO_EXT_DRV_268_112_2_18_0_32_58_1486 "GPIO_EXT_Init: Extend gpio diasble"
#define GPIO_EXT_DRV_274_112_2_18_0_32_58_1487 "GPIO_EXT_Init: Extend gpio enable"
#define GPIO_EXT_DRV_284_112_2_18_0_32_58_1488 "GPIO EXT: GPIO_TOTAL_CNT = % d, GPIO_EXT_DEV_MAX_CNT = %d, gpio_cnt_in_one_ic = %d, s_gpio_ext_int_pin = %d"
#define GPIO_EXT_DRV_289_112_2_18_0_32_58_1489 "working-gpio table is over !!!"
#define GPIO_EXT_DRV_319_112_2_18_0_32_58_1490 "EXT GPIO: working table %d line has been used !!"
#define GPIO_EXT_DRV_324_112_2_18_0_32_58_1491 "EXT GPIO : cus cfg table has not end flag, or larger than working table!!"
#define GPIO_EXT_DRV_369_112_2_18_0_32_58_1492 "GPIO_EXT_Init: Start time = %d"
#define GPIO_EXT_DRV_383_112_2_18_0_32_59_1493 "----- ic -- port -- gpio -- dir -- value -- callback addr"
#define GPIO_EXT_DRV_405_112_2_18_0_32_59_1494 "      %02d  -- %02d -- %02d -- %d  -- %d -- %08x"
#define GPIO_EXT_DRV_416_112_2_18_0_32_59_1495 " *    %02d  -- %02d -- %02d -- %d  -- %d -- %08x"
#define GPIO_EXT_DRV_422_112_2_18_0_32_59_1496 "GPIO_EXT_Init: ERR ic_num %d, gpio_num %d"
#define GPIO_EXT_DRV_466_112_2_18_0_32_59_1497 "------------------------- END -------------------------"
#define GPIO_EXT_DRV_501_112_2_18_0_32_59_1498 "GPIO_EXT_Init: End time = %d"
#define GPIO_EXT_DRV_512_112_2_18_0_32_59_1499 "GPIO_EXT_IsInit: Not Init!!"
#define GPIO_EXT_DRV_553_112_2_18_0_32_59_1500 "GPIO_EXT_DeShaking: Start Time = %d"
#define GPIO_EXT_DRV_583_112_2_18_0_32_59_1501 "GPIO_EXT_DeShaking: ( %02d - %02d - %02x )"
#define GPIO_EXT_DRV_599_112_2_18_0_32_59_1502 "GPIO_EXT_DeShaking: gpio_num = %04x"
#define GPIO_EXT_DRV_618_112_2_18_0_32_59_1503 "GPIO_EXT_DeShaking: End Time = %d"
#define GPIO_EXT_DRV_640_112_2_18_0_32_59_1504 "GPIO_EXT_SetDirection: gpio_num = %02x, value = %d"
#define GPIO_EXT_DRV_686_112_2_18_0_32_59_1505 "GPIO_EXT_SetValue: Don't need to Set extend gpio(%02x) direction(%02x)"
#define GPIO_EXT_DRV_717_112_2_18_0_32_59_1506 "[DRV_GPIO_EXT_DRV]<ERROR> ic > GPIO_EXT_DEV_MAX_CNT\r\n"
#define GPIO_EXT_DRV_843_112_2_18_0_32_59_1507 "GPIO_EXT_GetValue: gpio_num = %04x"
#define GPIO_EXT_DRV_883_112_2_18_0_32_59_1508 "GPIO_EXT_GetValue: gpio_num = %02x, value = %d"
#define GPIO_EXT_DRV_922_112_2_18_0_33_0_1509 "GPIO_EXT_SetPortValue:(before setting) all_gpio_value %0x"
#define GPIO_EXT_DRV_949_112_2_18_0_33_0_1510 "GPIO_EXT_SetPortValue:(after setting) all_gpio_value %0x"
#define GPIO_EXT_DRV_997_112_2_18_0_33_0_1511 "extend GPIO %04x be used"
#define GPIO_EXT_DRV_1024_112_2_18_0_33_0_1512 "[DRV_GPIO_EXT_DRV]<WARNING> ic > GPIO_EXT_DEV_MAX_CNT\r\n"
#define GPIO_EXT_DRV_1036_112_2_18_0_33_0_1513 "[DRV_GPIO_EXT_DRV]<ERROR> func:GPIO_EXT_RegGpio() ic > GPIO_EXT_DEV_MAX_CNT\r\n"
#define GPIO_EXT_DRV_1073_112_2_18_0_33_0_1514 "GPIO_EXT_UnRegGpio: The extend GPIO is not used!!"
#define GPIO_EXT_DRV_1102_112_2_18_0_33_0_1515 "GPIO_EXT_Control_Entry: start time = %d"
#define GPIO_EXT_DRV_1107_112_2_18_0_33_0_1516 "GPIO_EXT_Control_Entry: end time = %d"
#define GPIO_EXT_DRV_1135_112_2_18_0_33_0_1517 "GPIO_EXT_task_init"
#define GPIO_EXT_DRV_1154_112_2_18_0_33_0_1518 "GPIO_EXT_ISR_3_12_Test: gpio_ext_state = %d"
#define GPIO_EXT_DRV_1158_112_2_18_0_33_0_1519 "GPIO_EXT_Test"
#define GPIO_EXT_DRV_1165_112_2_18_0_33_0_1520 "GPIO_EXT_Test: g_function = %d"
#define GPIO_PROD_461_112_2_18_0_33_1_1521 "[DBAT]GPIO_DBAT_AuxBatDetectIntHandler: Aux battery plug OUT !"
#define GPIO_PROD_467_112_2_18_0_33_1_1522 "[DBAT]GPIO_DBAT_AuxBatDetectIntHandler: Aux battery plug IN !"
#define GPIO_PROD_485_112_2_18_0_33_1_1523 "[DBAT]GPIO_DBAT_MainBatDetectIntHandler: Main battery plug OUT !"
#define GPIO_PROD_491_112_2_18_0_33_1_1524 "[DBAT]GPIO_DBAT_MainBatDetectIntHandler: Main battery plug IN !"
#define GPIO_PROD_935_112_2_18_0_33_2_1525 "GPIO_WiFiIrqHander()"
#define GPIO_PROD_1122_112_2_18_0_33_3_1526 "GPIO_PROD_RegGpio - INT type error!"
#define GPIO_PROD_1173_112_2_18_0_33_3_1527 "_GPIO_PROD_RegGpio : type %d - invalid"
#define GPIO_PROD_1190_112_2_18_0_33_3_1528 "_GPIO_PROD_InitCfgTable"
#define GPIO_PROD_1279_112_2_18_0_33_3_1529 "GPIO_PROD_SetCfgInfo: Gpio prod id %d be Modified"
#define GPIO_PROD_1283_112_2_18_0_33_3_1530 "GPIO_PROD_SetCfgInfo: Gpio prod id %d Create"
#define GPIO_PROD_1299_112_2_18_0_33_3_1531 "GPIO_SetKeyPadBackLight: %d"
#define GPIO_PROD_1313_112_2_18_0_33_3_1532 "GPIO_SetLcdBackLight: %d"
#define GPIO_PROD_1350_112_2_18_0_33_3_1533 "GPIO_SetBackLight: %d"
#define GPIO_PROD_1364_112_2_18_0_33_3_1534 "GPIO_SetVibrator: %d"
#define GPIO_PROD_1390_112_2_18_0_33_3_1535 "GPIO_SetVibrator_Ext: on = %d, period = %d, stress = %d"
#define GPIO_PROD_1435_112_2_18_0_33_3_1536 "GPIO_GetFlipState: %d"
#define GPIO_PROD_1455_112_2_18_0_33_3_1537 "GPIO_CheckHeadsetStatus: %d"
#define GPIO_PROD_1473_112_2_18_0_33_3_1538 "GPIO_CheckSDCardStatus: %d"
#define GPIO_PROD_1718_112_2_18_0_33_4_1539 "GPIO_SelectSPI2CS: cs_num = %d"
#define GPIO_PROD_1812_112_2_18_0_33_4_1540 "GPIO_WiFiIrqHander()"
#define GPIO_PROD_1997_112_2_18_0_33_5_1541 "GPIO_SIMIntHandler %d %d. %d %d %d %d"
#define GPIO_PROD_2113_112_2_18_0_33_5_1542 "GPIO_HookKeyIntHandler gpio_state(%d)"
#define GPIO_PROD_2145_112_2_18_0_33_5_1543 "GPIO_EnableHookInt.........%d...."
#define GPIO_PROD_2155_112_2_18_0_33_5_1544 "GPIO_EnableHandFreeILed.........%d...."
#define GPIO_PROD_2250_112_2_18_0_33_5_1545 "GPIO_SetSPISwitch: WIFI ON"
#define GPIO_PROD_2257_112_2_18_0_33_5_1546 "GPIO_SetSPISwitch: CMMB ON"
#define GPIO_PROD_2264_112_2_18_0_33_5_1547 "GPIO_SetSPISwitch: SWITCH DEFAULT"
#define GPIO_PROD_2271_112_2_18_0_33_5_1548 "GPIO_SetSPISwitch: PARAMETERS ERROR"
#define GPS_COM_351_112_2_18_0_33_6_1549 "\r\nGPS_ComInit.BaudRate=%d"
#define GPS_COM_423_112_2_18_0_33_6_1550 "\r\nGPS_ComClose."
#define GPS_DIAG_78_112_2_18_0_33_6_1551 "_GPS_DIAG_CmdRoutine() begin: cmd=%d, num=%d, val_list=%s"
#define GPS_DIAG_159_112_2_18_0_33_7_1552 "GPSCalCmd end: cmd=%d, num=%d, val_list=%s"
#define GPS_DIAG_172_112_2_18_0_33_7_1553 "GPS: GPS_DIAG_RegDiagCmdRoutine"
#define GPS_DRV_171_112_2_18_0_33_7_1554 "_GPS_ValidationCallback: high_cipher 0x%08x, low_cipher 0x%08x"
#define GPS_DRV_192_112_2_18_0_33_7_1555 "_GPS_SetMode: s_gps_mode %d, mode %d"
#define GPS_DRV_255_112_2_18_0_33_7_1556 "GPS_Init: Identify fail !! status %d"
#define GPS_DRV_261_112_2_18_0_33_7_1557 "GPS_Init: Identify successful !!"
#define GPS_DRV_509_112_2_18_0_33_8_1558 "GPS_GetValidationCipher: input_serial 0x%08x, start time %d"
#define GPS_DRV_525_112_2_18_0_33_8_1559 "GPS_GetValidationCipher OK  end time %d!"
#define GPS_DRV_532_112_2_18_0_33_8_1560 "GPS_GetValidationCipher timer out, end time %d!"
#define GPS_NMEA_258_112_2_18_0_33_8_1561 "Unknown NMEA FRAME 0x%x"
#define GPS_NMEA_287_112_2_18_0_33_8_1562 "NMEA: Queue full, index %d"
#define GPS_NMEA_301_112_2_18_0_33_8_1563 "[DRV_GPS_NMEA]<WARNING> _NMEA_AddFrame2Queue: status is invalid\r\n"
#define GPS_NMEA_354_112_2_18_0_33_8_1564 "[DRV_GPS_NMEA]<WARNING> _NMEA_DelFrameFromQueue: status is invalid\r\n"
#define GPS_NMEA_368_112_2_18_0_33_9_1565 "NMEA: Queue Empty!"
#define GPS_NMEA_416_112_2_18_0_33_9_1566 "  < NMEA: %s > "
#define GPS_NMEA_425_112_2_18_0_33_9_1567 "NMEA: Drop frame - end error"
#define GPS_NMEA_437_112_2_18_0_33_9_1568 "NMEA: Drop frame - data error"
#define GPS_NMEA_459_112_2_18_0_33_9_1569 "NMEA: no end"
#define GPS_NMEA_467_112_2_18_0_33_9_1570 "NMEA: no start"
#define GPS_NMEA_593_112_2_18_0_33_9_1571 "[DRV_NMEA]<WARNING> _NMEA_DecodeGPGGA: field_cnt is invalid\r\n"
#define GPS_NMEA_643_112_2_18_0_33_9_1572 "[DRV_NMEA]<ERROR> _NMEA_DecodeGPGGA: field_cnt is invalid\r\n"
#define GPS_NMEA_788_112_2_18_0_33_10_1573 "[DRV_NMEA]<WARNING> _NMEA_DecodeGPGSV:field_cnt is invalid\r\n "
#define GPS_NMEA_859_112_2_18_0_33_10_1574 "[DRV_NMEA]<ERROR> _NMEA_DecodeGPGSV:field_cnt is invalid\r\n "
#define GPS_NMEA_915_112_2_18_0_33_10_1575 "NMEA_Init() !"
#define GPS_NMEA_926_112_2_18_0_33_10_1576 "NMEA_Close() !"
#define GPS_NMEA_947_112_2_18_0_33_10_1577 "NMEA NOT INIT !!!"
#define GPS_NMEA_953_112_2_18_0_33_10_1578 "No data in NMEA Stream !!"
#define GPS_NMEA_957_112_2_18_0_33_10_1579 "*** NMEA_SnatchFramesFromStream START, data len %d"
#define GPS_NMEA_961_112_2_18_0_33_10_1580 "NMEA: Split frame ! cur data %d"
#define GPS_NMEA_1065_112_2_18_0_33_10_1581 "*** NMEA_SnatchFramesFromStream END "
#define GPS_NMEA_1137_112_2_18_0_33_10_1582 "*******  _NMEA_Test_Task Start *******"
#define GPS_NMEA_1143_112_2_18_0_33_10_1583 "-- NMEA_Test_Task Loop --"
#define GPS_APP_210_112_2_18_0_33_11_1584 "%s"
#define GPS_APP_219_112_2_18_0_33_11_1585 "APP: NULL debug Message."
#define GPS_APP_223_112_2_18_0_33_11_1586 "%s"
#define GPS_APP_234_112_2_18_0_33_11_1587 "APP: INFO: got ref loc"
#define GPS_APP_244_112_2_18_0_33_11_1588 "APP: ERROR: SIRF_PAL_OS_SEMAPHORE_Release() error: 0x%X\n"
#define GPS_APP_255_112_2_18_0_33_11_1589 "App: INFO: got all (%d/%d) fixes"
#define GPS_APP_259_112_2_18_0_33_11_1590 "APP: INFO: got a fix (%d/%d)"
#define GPS_APP_413_112_2_18_0_33_11_1591 "APP: gpioInitialization() error: 0x%X\n"
#define GPS_APP_419_112_2_18_0_33_11_1592 "APP: ERROR: Init cannot set LSM Common configuration data"
#define GPS_APP_423_112_2_18_0_33_11_1593 "APP: INFO: LSM version is %s"
#define GPS_APP_427_112_2_18_0_33_11_1594 "APP: ERROR: Init failed calling LSM_Init"
#define GPS_APP_440_112_2_18_0_33_11_1595 "APP: ERROR: Deinit failed calling LSM_Deinit"
#define GPS_APP_446_112_2_18_0_33_11_1596 "gpioDeInitialization() error: 0x%X\n"
#define GPS_APP_472_112_2_18_0_33_11_1597 "APP: Semaphore (semPosition) Creation Failed!"
#define GPS_APP_478_112_2_18_0_33_11_1598 "APP: ERROR: Open cannot obtain session cfg data"
#define GPS_APP_487_112_2_18_0_33_11_1599 "APP: ERROR: Unable to get SET ID: %d"
#define GPS_APP_517_112_2_18_0_33_12_1600 "APP: ERROR: Open failed calling LSM_Start"
#define GPS_APP_529_112_2_18_0_33_12_1601 "APP: ERROR: Close failed calling LSM_Stop!"
#define GPS_APP_549_112_2_18_0_33_12_1602 "Failed to open On_Off port in Main \n"
#define GPS_APP_555_112_2_18_0_33_12_1603 "Failed to open Reset port in Main \n"
#define GPS_APP_631_112_2_18_0_33_12_1604 "start gps init"
#define GPS_APP_636_112_2_18_0_33_12_1605 "end gps init"
#define GPS_APP_645_112_2_18_0_33_12_1606 "[GPS]StartGpsThread:priority=0x%X\r\n"
#define GPS_APP_651_112_2_18_0_33_12_1607 "[GPS]OS_StartThread - s_gps_sched_thread_id = 0x%X\r\n"
#define GPS_APP_663_112_2_18_0_33_12_1608 "APP: ============ SiRFNavIV LSM Spreadtrum Demo App ============="
#define GPS_APP_669_112_2_18_0_33_12_1609 "ERROR: SIRF_PAL_Init Failed %d"
#define GPS_APP_678_112_2_18_0_33_12_1610 "APP: LPL Init Failed: 0x%x"
#define GPS_APP_683_112_2_18_0_33_12_1611 "ERROR: SIRF_PAL_Destroy Failed %d"
#define GPS_APP_691_112_2_18_0_33_12_1612 "APP: Session: %d of %d"
#define GPS_APP_695_112_2_18_0_33_12_1613 "APP: LPL Session Open Failed"
#define GPS_APP_706_112_2_18_0_33_12_1614 "ERROR: LPL_Close Failed %d"
#define GPS_APP_711_112_2_18_0_33_12_1615 "APP: LPL Session Closed."
#define GPS_APP_718_112_2_18_0_33_12_1616 "ERROR: LPL_Deinit Failed %d"
#define GPS_APP_721_112_2_18_0_33_12_1617 "APP: All Sessions completed"
#define GPS_APP_727_112_2_18_0_33_12_1618 "ERROR: SIRF_PAL_Destroy Failed %d"
#define GPS_CONFIG_58_112_2_18_0_33_12_1619 "GPIO_GPS_PowerOn: %d"
#define GPS_CONFIG_71_112_2_18_0_33_12_1620 "GPIO_GPS_On: %d"
#define GPS_CONFIG_85_112_2_18_0_33_12_1621 "GPIO_GPS_BootInter: %d"
#define GPS_CONFIG_98_112_2_18_0_33_12_1622 "GPIO_GPS_ResetLow: %d"
#define GPS_CONFIG_111_112_2_18_0_33_12_1623 "GPIO_GPS_Wakeup: %d"
#define GPS_CONFIG_130_112_2_18_0_33_12_1624 "Srf_ReadData: buf=%x, len=%d"
#define GPS_CONFIG_145_112_2_18_0_33_12_1625 "Srf_WriteData: buf=%x, len=%d"
#define GPS_CONFIG_159_112_2_18_0_33_12_1626 "Srf_Identify"
#define SIRF_PAL_TCPIP_81_112_2_18_0_33_19_1627 "CSR Andy SIRF_Security_Send_Data Error data_len %d, data_ptr 0x%x"
#define SIRF_PAL_TCPIP_87_112_2_18_0_33_19_1628 "CSR Andy SIRF_Security_Send_Data data_len %d"
#define SIRF_PAL_TCPIP_94_112_2_18_0_33_19_1629 "CSR Andy SIRF_Security_Send_Data sci_sock_send"
#define SIRF_PAL_TCPIP_99_112_2_18_0_33_19_1630 "CSR Andy SIRF_Security_Send_Data sci_sock_send successfully with bytes %d"
#define SIRF_PAL_TCPIP_103_112_2_18_0_33_19_1631 "CSR Andy SIRF_Security_Send_Data Warning sci_sock_send data not all sent"
#define SIRF_PAL_TCPIP_115_112_2_18_0_33_19_1632 "CSR Andy SIRF_Security_Send_Data SIRF_PAL_NET_Select"
#define SIRF_PAL_TCPIP_119_112_2_18_0_33_19_1633 "CSR Andy SIRF_Security_Send_Data Error SIRF_PAL_NET_Select"
#define SIRF_PAL_TCPIP_129_112_2_18_0_33_19_1634 "CSR Andy SIRF_Security_Send_Data sci_sock_recv data with bytes %d"
#define SIRF_PAL_TCPIP_130_112_2_18_0_33_19_1635 "CSR Andy SIRF_Security_Send_Data SSL_DecryptPasser Start"
#define SIRF_PAL_TCPIP_132_112_2_18_0_33_19_1636 "CSR Andy SIRF_Security_Send_Data SSL_DecryptPasser End"
#define SIRF_PAL_TCPIP_136_112_2_18_0_33_19_1637 "CSR Andy SIRF_Security_Send_Data Error sci_sock_recv"
#define SIRF_PAL_TCPIP_142_112_2_18_0_33_19_1638 "CSR Andy SIRF_Security_Send_Data sci_sock_errno for sci_sock_recv"
#define SIRF_PAL_TCPIP_146_112_2_18_0_33_19_1639 "CSR Andy SIRF_Security_Send_Data sci_sock_errno Pending for sci_sock_recv"
#define SIRF_PAL_TCPIP_150_112_2_18_0_33_19_1640 "CSR Andy SIRF_Security_Send_Data Error sci_sock_errno err %d for sci_sock_recv"
#define SIRF_PAL_TCPIP_156_112_2_18_0_33_19_1641 "CSR Andy SIRF_Security_Send_Data Error sci_sock_send"
#define SIRF_PAL_TCPIP_162_112_2_18_0_33_19_1642 "CSR Andy SIRF_Security_Send_Data sci_sock_errno for sci_sock_send"
#define SIRF_PAL_TCPIP_166_112_2_18_0_33_19_1643 "CSR Andy SIRF_Security_Send_Data sci_sock_errno Pending for sci_sock_send"
#define SIRF_PAL_TCPIP_170_112_2_18_0_33_19_1644 "CSR Andy SIRF_Security_Send_Data Error sci_sock_errno err %d for sci_sock_send"
#define SIRF_PAL_TCPIP_176_112_2_18_0_33_19_1645 "CSR Andy SIRF_Security_Send_Data CsrMemCpy"
#define SIRF_PAL_TCPIP_190_112_2_18_0_33_19_1646 "CSR Andy SIRF_Security_Send_Data Error data_len %d > max %d"
#define SIRF_PAL_TCPIP_202_112_2_18_0_33_19_1647 "CSR Andy SIRF_Security_Receive_Data data_len %d"
#define SIRF_PAL_TCPIP_209_112_2_18_0_33_19_1648 "CSR Andy SIRF_Security_Receive_Data Error data_len %d > max %d"
#define SIRF_PAL_TCPIP_222_112_2_18_0_33_19_1649 "CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_HANDSHAKE_SUCC"
#define SIRF_PAL_TCPIP_227_112_2_18_0_33_19_1650 "CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_FAIL"
#define SIRF_PAL_TCPIP_232_112_2_18_0_33_19_1651 "CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_CLOSE_BY_SERVER"
#define SIRF_PAL_TCPIP_237_112_2_18_0_33_19_1652 "CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_CANCLED_BY_USER"
#define SIRF_PAL_TCPIP_242_112_2_18_0_33_20_1653 "CSR Andy SIRF_Security_Post_Message msg_id %d"
#define SIRF_PAL_TCPIP_250_112_2_18_0_33_20_1654 "CSR Andy SIRF_Security_Show_Cert_Info"
#define SIRF_PAL_TCPIP_258_112_2_18_0_33_20_1655 "CSR Andy SIRF_PAL_NET_Init"
#define SIRF_PAL_TCPIP_428_112_2_18_0_33_20_1656 "CSR Andy SIRF_PAL_NET_Connect sci_sock_connect"
#define SIRF_PAL_TCPIP_435_112_2_18_0_33_20_1657 "CSR Andy SIRF_PAL_NET_Connect sci_sock_connect sci_sock_errno"
#define SIRF_PAL_TCPIP_439_112_2_18_0_33_20_1658 "CSR Andy SIRF_PAL_NET_Connect sci_sock_connect Pending"
#define SIRF_PAL_TCPIP_444_112_2_18_0_33_20_1659 "CSR Andy SIRF_PAL_NET_Connect Error sci_sock_connect %d"
#define SIRF_PAL_TCPIP_454_112_2_18_0_33_20_1660 "CSR Andy SIRF_PAL_NET_Connect SSL sci_sock_connect"
#define SIRF_PAL_TCPIP_467_112_2_18_0_33_20_1661 "CSR Andy SIRF_PAL_NET_Connect SSL SIRF_PAL_NET_Select"
#define SIRF_PAL_TCPIP_471_112_2_18_0_33_20_1662 "CSR Andy SIRF_PAL_NET_Connect SSL Error SIRF_PAL_NET_Select"
#define SIRF_PAL_TCPIP_476_112_2_18_0_33_20_1663 "CSR Andy SIRF_PAL_NET_Connect SSL_Create"
#define SIRF_PAL_TCPIP_478_112_2_18_0_33_20_1664 "CSR Andy SIRF_PAL_NET_Connect SSL_Create ssl_handle_csr %d"
#define SIRF_PAL_TCPIP_482_112_2_18_0_33_20_1665 "CSR Andy SIRF_PAL_NET_Connect SSL_Create Fail"
#define SIRF_PAL_TCPIP_489_112_2_18_0_33_20_1666 "CSR Andy SIRF_PAL_NET_Connect SSL_HandShake"
#define SIRF_PAL_TCPIP_490_112_2_18_0_33_20_1667 "CSR Andy SIRF_PAL_NET_Connect SSL_HandShake %s"
#define SIRF_PAL_TCPIP_495_112_2_18_0_33_20_1668 "CSR Andy SIRF_PAL_NET_Connect SSL_HandShake Successfully"
#define SIRF_PAL_TCPIP_504_112_2_18_0_33_20_1669 "CSR Andy SIRF_PAL_NET_Connect SSL_HandShake count: %d, ret %d"
#define SIRF_PAL_TCPIP_790_112_2_18_0_33_21_1670 "CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser "
#define SIRF_PAL_TCPIP_795_112_2_18_0_33_21_1671 "CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser Waiting"
#define SIRF_PAL_TCPIP_802_112_2_18_0_33_21_1672 "CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser Done with decrypted_data_len %d"
#define SIRF_PAL_TCPIP_808_112_2_18_0_33_21_1673 "CSR Andy SIRF_PAL_NET_Read SSL Error SSL_DecryptPasser Done with decrypted_data_len %d (buffer size %d)"
#define SIRF_PAL_TCPIP_857_112_2_18_0_33_21_1674 "CSR Andy SIRF_PAL_NET_Write SSL"
#define SIRF_PAL_TCPIP_865_112_2_18_0_33_21_1675 "CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser"
#define SIRF_PAL_TCPIP_870_112_2_18_0_33_21_1676 "CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser Waiting"
#define SIRF_PAL_TCPIP_875_112_2_18_0_33_21_1677 "CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser Done with encrypted_data_len_written %d"
#define SIRF_PAL_TCPIP_879_112_2_18_0_33_21_1678 "CSR Andy SIRF_PAL_NET_Write SSL sci_sock_send"
#define SIRF_PAL_TCPIP_884_112_2_18_0_33_21_1679 "CSR Andy SIRF_PAL_NET_Write SSL Error with encrypted_data_len_written 0"
#define SIRF_PAL_TCPIP_890_112_2_18_0_33_21_1680 "CSR Andy SIRF_PAL_NET_Write SSL sci_sock_send successfully with bytes %d"
#define SIRF_PAL_TCPIP_894_112_2_18_0_33_21_1681 "CSR Andy SIRF_PAL_NET_Write SSL bytes left %d, new writing position %d"
#define SIRF_PAL_TCPIP_898_112_2_18_0_33_21_1682 "CSR Andy SIRF_PAL_NET_Write SSL A set of encrption data done "
#define SIRF_PAL_TCPIP_910_112_2_18_0_33_21_1683 "CSR Andy SIRF_PAL_NET_Write SSL Error sci_sock_send"
#define SIRF_PAL_TCPIP_919_112_2_18_0_33_21_1684 "CSR Andy SIRF_PAL_NET_Write SSL sci_sock_errno"
#define SIRF_PAL_TCPIP_924_112_2_18_0_33_21_1685 "CSR Andy SIRF_PAL_NET_Write SSL sci_sock_errno Pending"
#define SIRF_PAL_TCPIP_929_112_2_18_0_33_21_1686 "CSR Andy SIRF_PAL_NET_Write SSL Error sci_sock_errno err %d"
#define GPS_SIRF_60_112_2_18_0_33_21_1687 "GPIO_GPS_PowerOn: %d"
#define GPS_SIRF_73_112_2_18_0_33_21_1688 "GPIO_GPS_On: %d"
#define GPS_SIRF_86_112_2_18_0_33_21_1689 "GPIO_GPS_BootInter: %d"
#define GPS_SIRF_99_112_2_18_0_33_21_1690 "GPIO_GPS_ResetLow: %d"
#define GPS_SIRF_112_112_2_18_0_33_21_1691 "GPIO_GPS_Wakeup: %d"
#define GPS_SIRF_127_112_2_18_0_33_22_1692 "GPIO_GPS_UART2GPIO: %d"
#define GPS_SIRF_158_112_2_18_0_33_22_1693 "Srf_Init"
#define GPS_SIRF_160_112_2_18_0_33_22_1694 "config Uart1 to GPIO and pull them down for saving power"
#define GPS_SIRF_178_112_2_18_0_33_22_1695 "Srf_Open: mode=%d"
#define GPS_SIRF_191_112_2_18_0_33_22_1696 "Srf_Open: reflash!"
#define GPS_SIRF_245_112_2_18_0_33_22_1697 "Srf_Close"
#define GPS_SIRF_265_112_2_18_0_33_22_1698 "[GPIO_GPS_PowerOn] OFF"
#define GPS_SIRF_279_112_2_18_0_33_22_1699 "Srf_Sleep: is_sleep=%d"
#define GPS_SIRF_295_112_2_18_0_33_22_1700 "Srf_Reflash"
#define GPS_SIRF_313_112_2_18_0_33_22_1701 "Srf_ReadData: buf=%x, len=%d"
#define GPS_SIRF_328_112_2_18_0_33_22_1702 "Srf_WriteData: buf=%x, len=%d"
#define GPS_SIRF_342_112_2_18_0_33_22_1703 "Srf_Identify"
#define GPS_SIRF_355_112_2_18_0_33_22_1704 "Srf_Test"
#define SRF_FUNC_341_112_2_18_0_33_23_1705 "%s"
#define SRF_FUNC_359_112_2_18_0_33_23_1706 "%s"
#define SRF_FUNC_387_112_2_18_0_33_23_1707 "GpsTTFFSrf, begin time %d"
#define SRF_FUNC_393_112_2_18_0_33_23_1708 "GpsTTFFSrf, response 0x%x"
#define SRF_FUNC_406_112_2_18_0_33_23_1709 "GpsTTFFSrf, end time %d"
#define SRF_FUNC_439_112_2_18_0_33_23_1710 "GPS test mode 4:"
#define SRF_FUNC_459_112_2_18_0_33_23_1711 "GpsTest4Srf: reset No ack."
#define SRF_FUNC_467_112_2_18_0_33_23_1712 "GpsTest4Srf, begin %d"
#define SRF_FUNC_472_112_2_18_0_33_23_1713 "GpsTest4Srf, response 0x%x"
#define SRF_FUNC_479_112_2_18_0_33_23_1714 "GpsTest4Srf, end %d"
#define SRF_FUNC_528_112_2_18_0_33_23_1715 "GPS: write command %d"
#define SRF_FUNC_540_112_2_18_0_33_23_1716 "GPS: NMEA, %s"
#define SRF_FUNC_553_112_2_18_0_33_23_1717 "GpsTestSrf loop"
#define SRF_FUNC_558_112_2_18_0_33_23_1718 "GPS: id=%x"
#define SRF_FUNC_567_112_2_18_0_33_23_1719 "GPS: skip"
#define SRF_FUNC_582_112_2_18_0_33_23_1720 "GpsTestSrf, TTFF time %d"
#define SRF_FUNC_586_112_2_18_0_33_23_1721 "GpsTestSrf, GpsTTFFSrf error %d"
#define SRF_FUNC_603_112_2_18_0_33_23_1722 "GpsTestSrf, skip %d"
#define SRF_FUNC_611_112_2_18_0_33_23_1723 "\"#GPS, GpsTest4Srf"
#define SRF_FUNC_616_112_2_18_0_33_23_1724 "GpsTest4Srf, error %d"
#define SRF_FUNC_623_112_2_18_0_33_23_1725 "GpsTest4Srf, response 0x%x"
#define UARTCOM_SAMPLE_280_112_2_18_0_33_24_1726 "[DRV_GPS_COM]<WARNING> uart1_callback: event > COM_MAX_EVENT\r\n"
#define UARTCOM_SAMPLE_350_112_2_18_0_33_24_1727 "\r\nCOM1_Init."
#define UARTCOM_SAMPLE_384_112_2_18_0_33_24_1728 "\r\nCOM0_Close."
#define LCD_BACKLIGHT_144_112_2_18_0_33_24_1729 "_LCD_GetBackLightType: addr 0x%x"
#define LCD_BACKLIGHT_147_112_2_18_0_33_24_1730 "_LCD_GetBackLightType: type %d, param %d"
#define LCD_BACKLIGHT_446_112_2_18_0_33_25_1731 "LCD_SetBackLightBrightness: BackLight type %d, Brightness %d"
#define LCD_BACKLIGHT_459_112_2_18_0_33_25_1732 "LCD_SetBackLightBrightness: BackLight type %d, backlight_ness %d"
#define LCD_DIAG_151_112_2_18_0_33_25_1733 "PE_RGBToYUV:src=0x%x,dst=0x%x,width=%d,height=%d"
#define LCD_DIAG_184_112_2_18_0_33_25_1734 "---PE_PROCESS:YUVToJPG malloc is fail!---"
#define LCD_DIAG_196_112_2_18_0_33_26_1735 "[pe_process.c] SaveJpeg successed! "
#define LCD_DIAG_241_112_2_18_0_33_26_1736 "---HandleReadMainBuffer in---time =%d-"
#define LCD_DIAG_253_112_2_18_0_33_26_1737 "HandleReadMainBuffer:malloc failed!"
#define LCD_DIAG_265_112_2_18_0_33_26_1738 "HandleReadMainBuffer:malloc failed1!"
#define LCD_DIAG_277_112_2_18_0_33_26_1739 "HandleReadMainBuffer:size=%d"
#define LCD_DIAG_325_112_2_18_0_33_26_1740 "---HandleReadMainBuffer out---time =%d-"
#define LCD_DIAG_337_112_2_18_0_33_26_1741 "LCD: LCD_DIAG_RegDiagCmdRoutine"
#define TFT_HX8312A_165_112_2_18_0_33_26_1742 "HX8312 initialize!"
#define TFT_HX8312A_459_112_2_18_0_33_27_1743 "HX8312_EnterSleep,%d"
#define TFT_HX8340B_159_112_2_18_0_33_28_1744 "qinss LCD: in HX8340B_reset"
#define TFT_HX8340B_168_112_2_18_0_33_28_1745 "\"\"\"#LCD: in HX8340B_Init ID(0x93)=,0x%x"
#define TFT_HX8340B_255_112_2_18_0_33_28_1746 "qinss LCD: in HX8340B_EnterSleep, is_sleep = %d"
#define TFT_HX8340B_299_112_2_18_0_33_28_1747 "qinss LCD: in HX8340B_Close"
#define TFT_HX8340B_451_112_2_18_0_33_28_1748 "qinss LCD: in HX8340B_GetOperations"
#define TFT_HX8346_181_112_2_18_0_33_29_1749 "qinss LCD: in HX8346_reset"
#define TFT_HX8346_344_112_2_18_0_33_29_1750 "qinss LCD: in HX8346_reset end."
#define TFT_HX8346_357_112_2_18_0_33_29_1751 "qinss LCD: in HX8346_EnterSleep, is_sleep = %d"
#define TFT_HX8346_392_112_2_18_0_33_29_1752 "qinss LCD: in HX8346_Close"
#define TFT_HX8346_541_112_2_18_0_33_29_1753 "qinss LCD: in HX8346_GetOperations"
#define TFT_HX8347_199_112_2_18_0_33_30_1754 "qinss LCD: in HX8347_reset"
#define TFT_HX8347_334_112_2_18_0_33_30_1755 "qinss LCD: in HX8347_reset end."
#define TFT_HX8347_347_112_2_18_0_33_30_1756 "qinss LCD: in HX8347_EnterSleep, is_sleep = %d"
#define TFT_HX8347_382_112_2_18_0_33_30_1757 "qinss LCD: in HX8347_Close"
#define TFT_HX8347_525_112_2_18_0_33_31_1758 "qinss LCD: in HX8347_GetOperations"
#define TFT_HX8347G_193_112_2_18_0_33_33_1759 "HX8347G set direction,direct_type=%d"
#define TFT_HX8347G_232_112_2_18_0_33_33_1760 "HX8347G set dis window,s_lcd_direct=%d"
#define TFT_HX8347G_313_112_2_18_0_33_33_1761 "qinss LCD: in HX8347G_reset"
#define TFT_HX8347G_321_112_2_18_0_33_33_1762 "\"\"\"#LCD: in HX8347G_Init ID(0x0)=,0x%x"
#define TFT_HX8347G_487_112_2_18_0_33_33_1763 "qinss LCD: in HX8347G_reset end."
#define TFT_HX8347G_499_112_2_18_0_33_33_1764 "HX8347G_Init"
#define TFT_HX8347G_514_112_2_18_0_33_33_1765 "qinss LCD: in HX8347G_EnterSleep, is_sleep = %d"
#define TFT_HX8347G_541_112_2_18_0_33_33_1766 "qinss LCD: in HX8347G_Close"
#define TFT_HX8347G_692_112_2_18_0_33_34_1767 "qinss LCD: in HX8347G_GetOperations"
#define TFT_HX8347G_SPI_191_112_2_18_0_33_34_1768 "[REF_LCD] HX8347G new left = %d, top = %d, right = %d, bottom = %d"
#define TFT_HX8352C_393_112_2_18_0_33_36_1769 "\r\n"
#define TFT_HX8357_241_112_2_18_0_33_37_1770 "\r\n"
#define TFT_HX8357_710_112_2_18_0_33_38_1771 "\r\n"
#define TFT_ILI9225_393_112_2_18_0_33_40_1772 "qinss LCD: in ILI9225B_EnterSleep, is_sleep = %d"
#define TFT_ILI9225_417_112_2_18_0_33_40_1773 "qinss LCD: in ILI9225B_Close"
#define TFT_ILI9225_567_112_2_18_0_33_40_1774 "qinss LCD: in ILI9225B_GetOperations"
#define TFT_ILI9225B_397_112_2_18_0_33_41_1775 "qinss LCD: in ILI9225B_EnterSleep, is_sleep = %d"
#define TFT_ILI9225B_421_112_2_18_0_33_41_1776 "qinss LCD: in ILI9225B_Close"
#define TFT_ILI9225B_571_112_2_18_0_33_41_1777 "qinss LCD: in ILI9225B_GetOperations"
#define TFT_ILI9320_233_112_2_18_0_33_42_1778 "qinss LCD: in ILI9320_reset"
#define TFT_ILI9320_404_112_2_18_0_33_43_1779 "qinss LCD: in ILI9320_reset end."
#define TFT_ILI9320_418_112_2_18_0_33_43_1780 "qinss LCD: in ILI9320_EnterSleep, is_sleep = %d"
#define TFT_ILI9320_498_112_2_18_0_33_43_1781 "qinss LCD: in ILI9320_Close"
#define TFT_ILI9320_648_112_2_18_0_33_43_1782 "qinss LCD: in ILI9320_GetOperations"
#define TFT_ILI9325_317_112_2_18_0_33_44_1783 "qinss LCD: in ILI9325_EnterSleep, is_sleep = %d"
#define TFT_ILI9325_346_112_2_18_0_33_44_1784 "qinss LCD: in ILI9325_Close"
#define TFT_ILI9325_498_112_2_18_0_33_44_1785 "qinss LCD: in ILI9325_GetOperations"
#define TFT_ILI9325B8_305_112_2_18_0_33_45_1786 "qinss LCD: in ILI9325_EnterSleep, is_sleep = %d"
#define TFT_ILI9325B8_334_112_2_18_0_33_45_1787 "qinss LCD: in ILI9325B8_Close"
#define TFT_ILI9325B8_486_112_2_18_0_33_45_1788 "qinss LCD: in ILI9325B8_GetOperations"
#define TFT_ILI9326_325_112_2_18_0_33_46_1789 "qinss LCD: in ILI9326_EnterSleep, is_sleep = %d"
#define TFT_ILI9326_366_112_2_18_0_33_46_1790 "qinss LCD: in ILI9326_Close"
#define TFT_ILI9326_517_112_2_18_0_33_47_1791 "qinss LCD: in ILI9326_GetOperations"
#define TFT_ILI9328_250_112_2_18_0_33_47_1792 "qinss LCD: in ILI9328_reset"
#define TFT_ILI9328_343_112_2_18_0_33_48_1793 "qinss LCD: in ILI9328_EnterSleep, is_sleep = %d"
#define TFT_ILI9328_375_112_2_18_0_33_48_1794 "qinss LCD: in ILI9328_Close"
#define TFT_ILI9328_527_112_2_18_0_33_48_1795 "qinss LCD: in ILI9328_GetOperations"
#define TFT_ILI9328_765_112_2_18_0_33_48_1796 "qinss LCD: in ILI9328_reset"
#define TFT_ILI9328_858_112_2_18_0_33_49_1797 "qinss LCD: in ILI9328_EnterSleep, is_sleep = %d"
#define TFT_ILI9328_890_112_2_18_0_33_49_1798 "qinss LCD: in ILI9328_Close"
#define TFT_ILI9328_1042_112_2_18_0_33_49_1799 "qinss LCD: in ILI9328_GetOperations"
#define TFT_ILI9335_252_112_2_18_0_33_50_1800 "qinss LCD: in ILI9335_reset"
#define TFT_ILI9335_328_112_2_18_0_33_50_1801 "ILI9335_Init"
#define TFT_ILI9335_347_112_2_18_0_33_50_1802 "qinss LCD: in ILI9335_Clear end"
#define TFT_ILI9335_360_112_2_18_0_33_50_1803 "qinss LCD: in ILI9335_EnterSleep, is_sleep = %d"
#define TFT_ILI9335_392_112_2_18_0_33_50_1804 "qinss LCD: in ILI9335_Close"
#define TFT_ILI9335_544_112_2_18_0_33_51_1805 "qinss LCD: in ILI9335_GetOperations"
#define TFT_ILI9338B_152_112_2_18_0_33_51_1806 "ILI9338B set direction,direct_type=%d"
#define TFT_ILI9338B_169_112_2_18_0_33_51_1807 "ILI9338B set dis window,s_lcd_direct=%d"
#define TFT_ILI9338B_209_112_2_18_0_33_51_1808 "qinss LCD: in ILI9338B_reset"
#define TFT_ILI9338B_217_112_2_18_0_33_51_1809 "\"\"\"#LCD: in ILI9338B_Init ID(0x0)=,0x%x"
#define TFT_ILI9338B_383_112_2_18_0_33_52_1810 "qinss LCD: in ILI9338B_reset end."
#define TFT_ILI9338B_395_112_2_18_0_33_52_1811 "ILI9338B_Init"
#define TFT_ILI9338B_410_112_2_18_0_33_52_1812 "qinss LCD: in ILI9338B_EnterSleep, is_sleep = %d"
#define TFT_ILI9338B_432_112_2_18_0_33_52_1813 "qinss LCD: in ILI9338B_Close"
#define TFT_ILI9338B_583_112_2_18_0_33_52_1814 "qinss LCD: in ILI9338B_GetOperations"
#define TFT_ILI9342_177_112_2_18_0_33_53_1815 "Direction LCD: ILI9342 Direction=%x"
#define TFT_ILI9342_195_112_2_18_0_33_53_1816 "ILI9342 window:left=%x,right=%x,top=%x,bottom=%x"
#define TFT_ILI9342_262_112_2_18_0_33_53_1817 " ILI9342_driver"
#define TFT_ILI9342_350_112_2_18_0_33_53_1818 " ILI9342_driver end "
#define TFT_ILI9342_358_112_2_18_0_33_53_1819 "ILI9342_Init"
#define TFT_ILI9342_374_112_2_18_0_33_53_1820 "qinss LCD: in ILI9342_Clear end"
#define TFT_ILI9342_386_112_2_18_0_33_53_1821 "qinss LCD: in ILI9342_EnterSleep, is_sleep = %d"
#define TFT_ILI9342_410_112_2_18_0_33_53_1822 "qinss LCD: in ILI9342_Close"
#define TFT_ILI9342_581_112_2_18_0_33_54_1823 "qinss LCD: in ILI9342_GetOperations"
#define TFT_ILI9481_244_112_2_18_0_33_54_1824 "ILI9481_Init"
#define TFT_ILI9481_437_112_2_18_0_33_55_1825 "ILI9481_IDCheck dev_id[1]=%x, dev_id[2]=%x, dev_id[3]=%x"
#define TFT_ILI9486_170_112_2_18_0_33_55_1826 "ILI9486_Init"
#define TFT_ILI9486_417_112_2_18_0_33_56_1827 "ILI9486_IDCheck dev_id[1]=%x, dev_id[2]=%x, dev_id[3]=%x"
#define TFT_LGDP4531_241_112_2_18_0_33_56_1828 "LCD: in LGDP4531_reset"
#define TFT_LGDP4531_308_112_2_18_0_33_56_1829 "LCD: in LGDP4531_reset end."
#define TFT_LGDP4531_323_112_2_18_0_33_56_1830 " LCD: in LGDP4531_EnterSleep, is_sleep = %d"
#define TFT_LGDP4531_373_112_2_18_0_33_57_1831 "LCD: in LGDP4531_Close"
#define TFT_LGDP4531_523_112_2_18_0_33_57_1832 "LCD: in LGDP4531_GetOperations"
#define TFT_LGDP4532_243_112_2_18_0_33_58_1833 "LCD: in LGDP4532_reset"
#define TFT_LGDP4532_310_112_2_18_0_33_58_1834 "LCD: in LGDP4532_reset end."
#define TFT_LGDP4532_322_112_2_18_0_33_58_1835 "LCD: in LGDP4532_EnterSleep, is_sleep = %d"
#define TFT_LGDP4532_374_112_2_18_0_33_58_1836 " LCD: in LGDP4532_Close"
#define TFT_LGDP4532_524_112_2_18_0_33_58_1837 "LCD: in LGDP4532_GetOperations"
#define TFT_NT3911_275_112_2_18_0_33_59_1838 "NT3911_EnterSleep,%d"
#define TFT_NT3911_353_112_2_18_0_33_59_1839 "NT3911_SetDisplayWindow:%d,%d,%d,%d"
#define TFT_NT3911_362_112_2_18_0_33_59_1840 " NT3911_SetDisplayWindow wrong: right > left or bottom > top"
#define TFT_NT39118_286_112_2_18_0_34_0_1841 "NT39118_EnterSleep,%d"
#define TFT_NT39118_324_112_2_18_0_34_0_1842 "NT39118_SetDisplayWindow:%d,%d,%d,%d"
#define TFT_NT39118_333_112_2_18_0_34_0_1843 " NT39118_SetDisplayWindow wrong: right > left or bottom > top"
#define TFT_R61505U_325_112_2_18_0_34_1_1844 "R61505U_EnterSleep,%d"
#define TFT_R61505U_390_112_2_18_0_34_1_1845 "R61505U_SetDisplayWindow:%d,%d,%d,%d"
#define TFT_R61505U_399_112_2_18_0_34_1_1846 " R61505U_SetDisplayWindow wrong: right > left or bottom > top"
#define TFT_R61509V_237_112_2_18_0_34_2_1847 "qinss LCD: in R61509V_reset"
#define TFT_R61509V_352_112_2_18_0_34_2_1848 "qinss LCD: in R61509V_EnterSleep, is_sleep = %d"
#define TFT_R61509V_375_112_2_18_0_34_2_1849 "qinss LCD: in R61509V_Close"
#define TFT_R61509V_524_112_2_18_0_34_2_1850 "qinss LCD: in R61509V_GetOperations"
#define TFT_R61529_179_112_2_18_0_34_3_1851 "qinss LCD: in R61529_reset"
#define TFT_R61529_364_112_2_18_0_34_3_1852 "qinss LCD: in R61529_EnterSleep, is_sleep = %d"
#define TFT_R61529_385_112_2_18_0_34_3_1853 "qinss LCD: in R61529_Close"
#define TFT_R61529_604_112_2_18_0_34_4_1854 "qinss LCD: in R61529_GetOperations"
#define TFT_R61581_181_112_2_18_0_34_4_1855 "R61581 initialize!"
#define TFT_RM68040_112_112_2_18_0_34_5_1856 "RM68040_Init\r\n"
#define TFT_RM68040_248_112_2_18_0_34_5_1857 "rm68040_set_window\n"
#define TFT_S6D0139_207_112_2_18_0_34_6_1858 "S6D0139 initialize!"
#define TFT_S6D04H0A_199_112_2_18_0_34_8_1859 "S6D04H0A initialize!"
#define TFT_SPFD5408_236_112_2_18_0_34_9_1860 "SPDF5408 LCD: in SPDF5408_reset"
#define TFT_SPFD5408_341_112_2_18_0_34_9_1861 "qinss LCD: in SPDF5408_reset end."
#define TFT_SPFD5408_354_112_2_18_0_34_9_1862 "qinss LCD: in SPDF5408_EnterSleep, is_sleep = %d"
#define TFT_SPFD54124A_325_112_2_18_0_34_10_1863 "SPFD54124A_EnterSleep,%d"
#define TFT_SPFD54124A_361_112_2_18_0_34_10_1864 "SPFD54124A_SetDisplayWindow:%d,%d,%d,%d"
#define TFT_SPFD54124A_370_112_2_18_0_34_10_1865 " SPFD54124A_SetDisplayWindow wrong: right > left or bottom > top"
#define TFT_SPFD5420A_242_112_2_18_0_34_11_1866 "LCD: in SPFD5420A_reset"
#define TFT_SPFD5420A_333_112_2_18_0_34_12_1867 "LCD: in SPFD5420A_reset end."
#define TFT_SPFD5420A_345_112_2_18_0_34_12_1868 "LCD: in SPFD5420A_EnterSleep, is_sleep = %d"
#define TFT_SPFD5420A_370_112_2_18_0_34_12_1869 "LCD: in SPFD5420A_Close"
#define TFT_SPFD5420A_521_112_2_18_0_34_12_1870 "qinss LCD: in SPFD5420A_GetOperations"
#define TFT_SSD1228_199_112_2_18_0_34_13_1871 "SSD1288_set_display_window***********"
#define TFT_SSD1228_254_112_2_18_0_34_13_1872 "SSD1288_InvalidateRect***********"
#define TFT_SSD1228_283_112_2_18_0_34_13_1873 "SSD1288_EnterSleep,%d"
#define TFT_SSD1228_314_112_2_18_0_34_13_1874 "SSD1288_SetDisplayWindow:%d,%d,%d,%d"
#define TFT_SSD1228_323_112_2_18_0_34_13_1875 " SSD1288_SetDisplayWindow wrong: right > left or bottom > top"
#define TFT_ST7781RB8_178_112_2_18_0_34_13_1876 "ST7781R_Init"
#define TFT_TP057A_VT_176_112_2_18_0_34_14_1877 "LCD ID ERROR!!!HDr@@@@@@@@@@@@ Lcd_ver:%x"
#define TFT_TP057A_VT_312_112_2_18_0_34_15_1878 "LCD ID ERROR!!!HDr@@@@@@@@@@@@ Lcd_ver:%x"
#define MSENSOR_ADXL346_205_112_2_18_0_34_16_1879 "[REF_MSENSOR] %s : id = 0x%X"
#define MSENSOR_ADXL346_357_112_2_18_0_34_16_1880 "MSENSOR: MOCOR_COOR: axis_x = %5d, axis_y = %5d, axis_z = %5d,"
#define MSENSOR_ADXL346_608_112_2_18_0_34_17_1881 "[REF_MSENSOR] %s :  shake: %8d"
#define MSENSOR_ADXL346_616_112_2_18_0_34_17_1882 "[REF_MSENSOR] %s :  rotate: %8d"
#define MSENSOR_DTOS_110_112_2_18_0_34_17_1883 "[DRV_MSENSOR] open IIC failed!\r\n"
#define MSENSOR_DTOS_130_112_2_18_0_34_17_1884 "[DRV_MSENSOR] open IIC failed!\r\n"
#define MSENSOR_DTOS_288_112_2_18_0_34_17_1885 "[DRV_MSENSOR] x = %d(%d), y = %d(%d), status = 0x%X, "
#define MSENSOR_DTOS_324_112_2_18_0_34_18_1886 "[DRV_MSENSOR] or_index=%d, "
#define MSENSOR_DTOS_353_112_2_18_0_34_18_1887 "[DRV_MSENSOR] x = %d, y = %d, status= 0x%xrn"
#define MSENSOR_DTOS_360_112_2_18_0_34_18_1888 "[DRV_MSENSOR] lcd_face = %d, pin1 = %d"
#define MSENSOR_DTOS_428_112_2_18_0_34_18_1889 "[DRV_MSENSOR] MOCOR_COOR: x = %d, y = %d, "
#define MSENSOR_DTOS_456_112_2_18_0_34_18_1890 "[DRV_MSENSOR] open IIC failed!\r\n"
#define MSENSOR_DTOS_470_112_2_18_0_34_18_1891 "[DRV_MSENSOR]DETECTION = 0x%X, sensi_val = %d rn"
#define MSENSOR_DTOS_524_112_2_18_0_34_18_1892 "[DRV_MSENSOR] cmd = %d \r\n"
#define MSENSOR_LIS35DE_160_112_2_18_0_34_19_1893 "MSENSOR:: ==> open IIC failed!\r\n"
#define MSENSOR_LIS35DE_173_112_2_18_0_34_19_1894 "MSENSOR:: ==> SUCCEESSFULLY!"
#define MSENSOR_LIS35DE_215_112_2_18_0_34_19_1895 "MSENSOR::==> open IIC failed!\r\n"
#define MSENSOR_LIS35DE_248_112_2_18_0_34_19_1896 "MSENSOR:: ==> SUCCEESSFULLY!"
#define MSENSOR_LIS35DE_311_112_2_18_0_34_19_1897 "MSENSOR: placement:  lcd_face = %d, pin1 = %d"
#define MSENSOR_LIS35DE_313_112_2_18_0_34_19_1898 "MSENSOR: SENSOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,"
#define MSENSOR_LIS35DE_385_112_2_18_0_34_19_1899 "MSENSOR: MOCOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,"
#define MSENSOR_LIS35DE_503_112_2_18_0_34_19_1900 "MSENSOR:: s_last_motion_type= 0x%08X, "
#define MSENSOR_LIS35DE_586_112_2_18_0_34_19_1901 "MSENSOR:: cmd = %d \r\n"
#define MSENSOR_LIS3DH_123_112_2_18_0_34_20_1902 "[REF_MSENSOR] %s : open IIC failed!"
#define MSENSOR_LIS3DH_137_112_2_18_0_34_20_1903 "[REF_MSENSOR] %s : addr: 0x%02X = 0x%02X  "
#define MSENSOR_LIS3DH_156_112_2_18_0_34_20_1904 "[REF_MSENSOR] %s : data: 0x%X%X, 0x%X%X, 0x%X%X"
#define MSENSOR_LIS3DH_172_112_2_18_0_34_20_1905 "[REF_MSENSOR] %s : open IIC failed!"
#define MSENSOR_LIS3DH_194_112_2_18_0_34_20_1906 "[REF_MSENSOR] %s : id = 0x%X"
#define MSENSOR_LIS3DH_220_112_2_18_0_34_20_1907 "[REF_MSENSOR] LIS3DH 0x%X = 0x%2X, 0x%2X, 0x%2X, 0x%2X,"
#define MSENSOR_LIS3DH_235_112_2_18_0_34_20_1908 "[REF_MSENSOR] %s 1: id = 0x%X"
#define MSENSOR_LIS3DH_243_112_2_18_0_34_20_1909 "[REF_MSENSOR] %s 2: CTRL1 = 0x%X"
#define MSENSOR_LIS3DH_249_112_2_18_0_34_20_1910 "[REF_MSENSOR] %s 3: id = 0x%X"
#define MSENSOR_LIS3DH_340_112_2_18_0_34_20_1911 "MSENSOR: placement:  lcd_face = %d, pin1 = %d"
#define MSENSOR_LIS3DH_342_112_2_18_0_34_20_1912 "MSENSOR: SENSOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,"
#define MSENSOR_LIS3DH_414_112_2_18_0_34_20_1913 "MSENSOR: MOCOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,"
#define MSENSOR_LIS3DH_529_112_2_18_0_34_21_1914 "MSENSOR::"
#define MSENSOR_LIS3DH_613_112_2_18_0_34_21_1915 "[REF_MSENSOR] %s : cmd = %d"
#define MSENSOR_DRV_100_112_2_18_0_34_22_1916 "_MSensor_CreateEvent : event =0x%x"
#define MSENSOR_DRV_146_112_2_18_0_34_22_1917 "_MSensor_SendMSG : msg = %d, para = %d, is_sync = %d"
#define MSENSOR_DRV_151_112_2_18_0_34_22_1918 "MSensor_TimerHandler: msensor thread Q Full !!"
#define MSENSOR_DRV_205_112_2_18_0_34_22_1919 "DRV_MSENSOR : init: name=%s type = %d init failed!rn"
#define MSENSOR_DRV_231_112_2_18_0_34_22_1920 "DRV_MSENSOR : HandleRoutine: msg_id = %d, sensor_type = %drn"
#define MSENSOR_DRV_248_112_2_18_0_34_22_1921 "DRV_MSENSOR : HandleRoutine: invalid open()"
#define MSENSOR_DRV_274_112_2_18_0_34_22_1922 "DRV_MSENSOR : HandleRoutine: event = 0x%08X\r\n"
#define MSENSOR_DRV_304_112_2_18_0_34_22_1923 "MSensor:  _MSensor_CreateTask priority=%d\n"
#define MSENSOR_DRV_319_112_2_18_0_34_22_1924 "_MSensor_CreateTask : task id 0x%x"
#define MSENSOR_DRV_327_112_2_18_0_34_22_1925 "MSensor:  _MSensor_ChangeTask new Priority = %dn"
#define MSENSOR_DRV_336_112_2_18_0_34_22_1926 "MSensor:  _MSensor_ChangeTask old Priority = %dn"
#define MSENSOR_DRV_339_112_2_18_0_34_22_1927 "MSensor:  _MSensor_ChangeTaskpPriority SCI_INVALID_BLOCK_ID"
#define MSENSOR_DRV_350_112_2_18_0_34_22_1928 "Msensor: _MSensor_DefaultServiceCB: id = %d, argc = %d argv = %d"
#define MSENSOR_DRV_375_112_2_18_0_34_22_1929 "Msensor: DIAG, type = %d, event = 0x%08X"
#define MSENSOR_DRV_431_112_2_18_0_34_23_1930 "MSensor_Open : failed, sensor_type %d not support"
#define MSENSOR_DRV_461_112_2_18_0_34_23_1931 "MSensor_Open : s_msensor_tab[%d]->name= %s, status = %d, "
#define MSENSOR_DRV_478_112_2_18_0_34_23_1932 "MSensor_Close : failed, sensor_type %d not support"
#define MSENSOR_DRV_500_112_2_18_0_34_23_1933 "MSensor_Close : s_msensor_tab[%d]->name= %s, status = %d, "
#define MSENSOR_DRV_512_112_2_18_0_34_23_1934 "MSensor_IntHandler: The thread is NOT running!"
#define MSENSOR_DRV_541_112_2_18_0_34_23_1935 "MSensor_TimerHandler: The thread is NOT running!"
#define MSENSOR_DRV_565_112_2_18_0_34_23_1936 "MSensor_ioctl : failed, sensor_type %d not support"
#define MSENSOR_TRULY_A320_135_112_2_18_0_34_23_1937 "[DRV_OFN] open IIC failed!\r\n"
#define MSENSOR_TRULY_A320_227_112_2_18_0_34_23_1938 "[DRV_OFN] open IIC failed!\r\n"
#define MSENSOR_TRULY_A320_348_112_2_18_0_34_24_1939 "[DRV_OFN] cmd = %d \r\n"
#define MSENSOR_LTR501_134_112_2_18_0_34_24_1940 "[DRV_OPTICAL] LTR501_init_i2c_handler  open IIC failed!"
#define MSENSOR_LTR501_338_112_2_18_0_34_24_1941 "[DRV_OPTICAL] LTR502_init"
#define MSENSOR_LTR501_351_112_2_18_0_34_25_1942 "[DRV_OPTICAL] LTR501_open"
#define MSENSOR_LTR501_444_112_2_18_0_34_25_1943 "[DRV_OPTICAL]als_data %5x"
#define MSENSOR_SCM013_148_112_2_18_0_34_25_1944 "[Kui Wang]Work_Adc=%d\t,Ambent_Adc=%d\t,Work_Adc - Ambent_Adc=%d\t,cur_status=%d"
#define MSENSOR_MEYE_YANGXUN_135_112_2_18_0_34_26_1945 "GSensor_Open: Open fail !!"
#define MSENSOR_MEYE_YANGXUN_165_112_2_18_0_34_26_1946 "GSensor_Close !!"
#define MSENSOR_MEYE_YANGXUN_183_112_2_18_0_34_26_1947 "_Camera_GetImage : Gsensor is not open !!"
#define MSENSOR_MEYE_YANGXUN_207_112_2_18_0_34_26_1948 "_Camera_GetImage: _GSensor_GetCaptureParam %d,%d,0x%x"
#define MSENSOR_MEYE_YANGXUN_213_112_2_18_0_34_26_1949 "_Camera_GetImage: Fail !!"
#define MSENSOR_MEYE_YANGXUN_217_112_2_18_0_34_26_1950 "_Camera_GetImage: success."
#define MSENSOR_MEYE_YANGXUN_233_112_2_18_0_34_26_1951 "met_cam_camera_open: MSensor_GetImageAddr is NULL"
#define MSENSOR_MEYE_YANGXUN_255_112_2_18_0_34_26_1952 "_met_camera_close()"
#define MSENSOR_MEYE_YANGXUN_309_112_2_18_0_34_26_1953 "MET_CMOTION_Init ok"
#define MSENSOR_MEYE_YANGXUN_335_112_2_18_0_34_26_1954 "met_cam_cmotion_close"
#define MSENSOR_MEYE_YANGXUN_358_112_2_18_0_34_26_1955 "met_cam_cmotion_judge_motion: \" Judge time = %d"
#define MSENSOR_MEYE_YANGXUN_362_112_2_18_0_34_26_1956 "MET_CAM_CMOTION MOVED! \""
#define MSENSOR_MEYE_YANGXUN_393_112_2_18_0_34_26_1957 "met_cam_alloc_ext_mem : ptr 0x%x, size %d"
#define MSENSOR_MEYE_YANGXUN_404_112_2_18_0_34_26_1958 "met_cam_free_ext_mem:prt 0x%x"
#define MAGICVOICE_APP_115_112_2_18_0_34_26_1959 " MV_DEMO_Open"
#define MAGICVOICE_APP_136_112_2_18_0_34_27_1960 " MV_DEMO_Close"
#define MAGICVOICE_APP_167_112_2_18_0_34_27_1961 "[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_CHANGE\r\n"
#define MAGICVOICE_APP_181_112_2_18_0_34_27_1962 "[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_ROLE\r\n"
#define MAGICVOICE_APP_195_112_2_18_0_34_27_1963 "[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_EFFECT\r\n"
#define MAGICVOICE_APP_209_112_2_18_0_34_27_1964 "[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_EQID\r\n"
#define MAGICVOICE_APP_223_112_2_18_0_34_27_1965 "[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_BGID\r\n"
#define MAGICVOICE_APP_237_112_2_18_0_34_27_1966 "[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_BGVolume\r\n"
#define MAGICVOICE_APP_251_112_2_18_0_34_27_1967 "[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_MVolume\r\n"
#define MAGICVOICE_APP_258_112_2_18_0_34_27_1968 "[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:paramid is invalid\r\n"
#define MAGICVOICE_APP_312_112_2_18_0_34_27_1969 "MV_DEMO_Open_Resource: open res fail or no card"
#define MAGICVOICE_APP_323_112_2_18_0_34_27_1970 "read bytes_read = %d"
#define MAGICVOICE_APP_351_112_2_18_0_34_27_1971 "MV_DEMO_Open_Resource: open log fail or no card"
#define MAGICVOICE_APP_442_112_2_18_0_34_27_1972 "s_mv_thread_id = 0x%X\r\n"
#define INTEL_M18_DRV_706_112_2_18_0_34_32_1973 "INTEL_Write:Error Address or Length!"
#define INTEL_M18_DRV_778_112_2_18_0_34_32_1974 "[DRV_INTEL_M18_DRV]<WARNING> addr is invalid\r\n"
#define OCR_DRIVER_216_112_2_18_0_34_44_1975 "OCR_Init"
#define OCR_DRIVER_221_112_2_18_0_34_44_1976 "OCR_Init: ocr has been init before or hasn't been closed"
#define OCR_DRIVER_268_112_2_18_0_34_45_1977 "OCR_Preview"
#define OCR_DRIVER_273_112_2_18_0_34_45_1978 "OCR_Preview: ocr has NOT been opened, 0x%x"
#define OCR_DRIVER_281_112_2_18_0_34_45_1979 "OCR_Preview: OCR setting parameter error"
#define OCR_DRIVER_289_112_2_18_0_34_45_1980 "OCR_Preview: OCR start preview error"
#define OCR_DRIVER_310_112_2_18_0_34_45_1981 "OCR_SetPreviewParam"
#define OCR_DRIVER_317_112_2_18_0_34_45_1982 "OCR_SetPreviewParam: ocr has NOT been opened, 0x%x"
#define OCR_DRIVER_338_112_2_18_0_34_45_1983 "OCR_SetPreviewParam: OCR setting parameter error"
#define OCR_DRIVER_359_112_2_18_0_34_45_1984 "OCR_StartPreview"
#define OCR_DRIVER_364_112_2_18_0_34_45_1985 "OCR_StartPreview: ocr has NOT been opened, 0x%x"
#define OCR_DRIVER_371_112_2_18_0_34_45_1986 "OCR_StartPreview: ocr is previewing"
#define OCR_DRIVER_379_112_2_18_0_34_45_1987 "OCR_StartPreview: ocr start preview error"
#define OCR_DRIVER_400_112_2_18_0_34_45_1988 "OCR_StopPreview"
#define OCR_DRIVER_405_112_2_18_0_34_45_1989 "OCR_StopPreview: ocr has NOT been opened"
#define OCR_DRIVER_412_112_2_18_0_34_45_1990 "OCR_StopPreview: ocr is not previewing"
#define OCR_DRIVER_420_112_2_18_0_34_45_1991 "OCR_StopPreview: ocr stop preview error"
#define OCR_DRIVER_441_112_2_18_0_34_45_1992 "OCR_DoFocus"
#define OCR_DRIVER_446_112_2_18_0_34_45_1993 "OCR_DoFocus: ocr has NOT been opened"
#define OCR_DRIVER_453_112_2_18_0_34_45_1994 "OCR_DoFocus: ocr is not previewing"
#define OCR_DRIVER_476_112_2_18_0_34_45_1995 "OCR_Capture"
#define OCR_DRIVER_481_112_2_18_0_34_45_1996 "OCR_Capture: ocr has NOT been opened : 0x%x"
#define OCR_DRIVER_489_112_2_18_0_34_45_1997 "OCR_Capture: OCR set capture parameter error"
#define OCR_DRIVER_497_112_2_18_0_34_45_1998 "OCR_Capture: OCR caputre error"
#define OCR_DRIVER_519_112_2_18_0_34_45_1999 "OCR_SetCaptureParam"
#define OCR_DRIVER_526_112_2_18_0_34_45_2000 "OCR_SetCaptureParams: ocr has NOT been opened : 0x%x"
#define OCR_DRIVER_547_112_2_18_0_34_45_2001 "OCR_SetCaptureParams: set parameter error"
#define OCR_DRIVER_564_112_2_18_0_34_45_2002 "OCR_DoCapture"
#define OCR_DRIVER_569_112_2_18_0_34_45_2003 "OCR_DoCapture: ocr has NOT been opened : 0x%x"
#define OCR_DRIVER_577_112_2_18_0_34_45_2004 "OCR_DoCapture: capture error"
#define OCR_DRIVER_596_112_2_18_0_34_45_2005 "OCR_GetYUVbuf: %x, %d"
#define OCR_DRIVER_630_112_2_18_0_34_45_2006 "OCR_SetReviewParam: %x"
#define OCR_DRIVER_637_112_2_18_0_34_45_2007 "OCR_SetReviewParam: ocr has NOT been opened"
#define OCR_DRIVER_668_112_2_18_0_34_45_2008 "OCR_SetReviewParam: set parameter error"
#define OCR_DRIVER_699_112_2_18_0_34_46_2009 "OCR_DoReview"
#define OCR_DRIVER_736_112_2_18_0_34_46_2010 "OCR_Close"
#define OCR_DRIVER_741_112_2_18_0_34_46_2011 "OCR_Close: ocr has NOT been opened"
#define OCR_DRIVER_771_112_2_18_0_34_46_2012 "OCR_SetOsdZoneMode: set ZONE mode error"
#define OCR_DRIVER_801_112_2_18_0_34_46_2013 "OCR_SetOsdImage: set osd image error"
#define OCR_DRIVER_827_112_2_18_0_34_46_2014 "OCR_SetOsdColorPalette: set osd color palette error"
#define OCR_DRIVER_858_112_2_18_0_34_46_2015 "OCR_ModifyOsdImage: modify osd image error"
#define OCR_DRIVER_879_112_2_18_0_34_46_2016 "OCR_SetOsdEnableMode: modify osd image error"
#define OCR_DRIVER_900_112_2_18_0_34_46_2017 "s_st_ocr_context.p_buf = %x"
#define OCR_DRIVER_956_112_2_18_0_34_46_2018 "OCR_GetLayoutRegion_lay %d: %d, %d, %d, %d"
#define OCR_DRIVER_980_112_2_18_0_34_46_2019 "OCR_GetLayoutRegion_disp %d: %d, %d, %d, %d"
#define OCR_DRIVER_1090_112_2_18_0_34_46_2020 "OCR_LCD_InvalidateImage: %d, %d, %d, %d, color_id:%d"
#define THOCR_LIB_207_112_2_18_0_34_47_2021 "OCRlib_ThreadLayoutEntry: OCRlib_Layout......"
#define THOCR_LIB_209_112_2_18_0_34_47_2022 "OCRlib_ThreadLayoutEntry: OCRlib_Layout done."
#define THOCR_LIB_248_112_2_18_0_34_47_2023 "ocr_layout_task_open,task_id = %d"
#define THOCR_LIB_264_112_2_18_0_34_47_2024 "ocr_layout_task_close,task_id = %d"
#define THOCR_LIB_270_112_2_18_0_34_47_2025 "ocr lib being init, can't stop task now, wait."
#define THOCR_LIB_277_112_2_18_0_34_47_2026 "ocr lib is open, ready to stop, wait index=%d."
#define THOCR_LIB_338_112_2_18_0_34_47_2027 "OCRlib_ThreadAutoRecogEntry: OCRlib_AutoRecog done."
#define THOCR_LIB_377_112_2_18_0_34_47_2028 "ocr_autorecog_task_open,task_id = %d"
#define THOCR_LIB_392_112_2_18_0_34_47_2029 "ocr_autorecog_task_close,task_id = %d"
#define THOCR_LIB_435_112_2_18_0_34_47_2030 "OCR:OCR_hSendMsg sig_code = %d"
#define THOCR_LIB_550_112_2_18_0_34_48_2031 "OCRlib_Layout: OCRlib_Initialize....."
#define THOCR_LIB_554_112_2_18_0_34_48_2032 "OCRlib_Layout: OCRlib_Initialize done."
#define THOCR_LIB_558_112_2_18_0_34_48_2033 "OCRlib_Layout: ARM clock = %d"
#define THOCR_LIB_564_112_2_18_0_34_48_2034 "OCRlib_Layout: running time = %d"
#define THOCR_LIB_566_112_2_18_0_34_48_2035 "OCRlib_Layout: Num of Regions: %d."
#define THOCR_LIB_572_112_2_18_0_34_48_2036 "ocr layout failed, error code = %d"
#define THOCR_LIB_598_112_2_18_0_34_48_2037 "OCRlib_AutoRecog: ARM clock = %d"
#define THOCR_LIB_603_112_2_18_0_34_48_2038 "OCRlib_AutoRecog: region %d ..."
#define THOCR_LIB_607_112_2_18_0_34_48_2039 "OCRlib_AutoRecog: left=%d top=%d right=%d bottom=%d"
#define THOCR_LIB_730_112_2_18_0_34_48_2040 "OCRlib_AutoRecog: UNICODE (first 10) %d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
#define THOCR_LIB_732_112_2_18_0_34_48_2041 "OCRlib_AutoRecog: Class %d"
#define THOCR_LIB_734_112_2_18_0_34_48_2042 "OCRlib_AutoRecog: region %d done."
#define THOCR_LIB_736_112_2_18_0_34_48_2043 "OCRlib_AutoRecog: running time = %d"
#define THOCR_LIB_792_112_2_18_0_34_48_2044 "OCRlib_CallBackFunc: nState=%d, nInfo=%d\n"
#define THOCR_LIB_868_112_2_18_0_34_48_2045 "OCRlib_Initialize: dumping y buffer..."
#define THOCR_LIB_871_112_2_18_0_34_48_2046 "OCRlib_Initialize: dumping y buffer done."
#define THOCR_LIB_875_112_2_18_0_34_48_2047 "OCRlib_Initialize: dumping y buffer failed."
#define THOCR_LIB_880_112_2_18_0_34_48_2048 "OCRlib_Initialize: OCR LIB VER %d"
#define THOCR_LIB_892_112_2_18_0_34_48_2049 "ocr initialize failed, error code = %d"
#define THOCR_LIB_905_112_2_18_0_34_48_2050 "ocr set image failed, error code = %d"
#define THOCR_LIB_973_112_2_18_0_34_49_2051 "OCRlib_RecogOneRegion: TH_RecogOneRegion..."
#define THOCR_LIB_975_112_2_18_0_34_49_2052 "OCRlib_RecogOneRegion: TH_RecogOneRegion done."
#define THOCR_LIB_978_112_2_18_0_34_49_2053 "ocr recognition failed, error code = %d"
#define THOCR_LIB_990_112_2_18_0_34_49_2054 "OCRlib_RecogOneRegion: TH_ClassifyOneRegionByKonwClass..."
#define THOCR_LIB_998_112_2_18_0_34_49_2055 "OCRlib_RecogOneRegion: TH_ClassifyOneRegionByKonwClass done."
#define THOCR_LIB_1001_112_2_18_0_34_49_2056 "ocr classification failed, error code = %d"
#define HX8347_SEINE_DRV_287_112_2_18_0_34_50_2057 "INDEX %u, LEN %u"
#define HX8347_SEINE_DRV_290_112_2_18_0_34_50_2058 "Total len %u, index len %u"
#define ILI9325_SEINE_DRV_289_112_2_18_0_34_51_2059 "INDEX %u, LEN %u"
#define ILI9325_SEINE_DRV_292_112_2_18_0_34_51_2060 "Total len %u, index len %u"
#define LGDP4531_SEINE_DRV_345_112_2_18_0_34_52_2061 "INDEX %u, LEN %u"
#define LGDP4531_SEINE_DRV_348_112_2_18_0_34_52_2062 "Total len %u, index len %u"
#define R61509V_SEINE_DRV_286_112_2_18_0_34_52_2063 "INDEX %u, LEN %u"
#define R61509V_SEINE_DRV_289_112_2_18_0_34_52_2064 "Total len %u, index len %u"
#define R61581_SEINE_DRV_337_112_2_18_0_34_53_2065 "INDEX %u, LEN %u"
#define R61581_SEINE_DRV_340_112_2_18_0_34_53_2066 "Total len %u, index len %u"
#define SPFD5420A_SEINE_DRV_305_112_2_18_0_34_54_2067 "INDEX %u, LEN %u"
#define SPFD5420A_SEINE_DRV_308_112_2_18_0_34_54_2068 "Total len %u, index len %u"
#define TP_FT5206_114_112_2_18_0_35_2_2069 "TP:: open I2C failed!\r\n"
#define TP_FT5206_122_112_2_18_0_35_2_2070 "TP:: open I2C success!\r\n"
#define TP_FT5206_241_112_2_18_0_35_2_2071 "[TSP] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n"
#define TP_FT5206_259_112_2_18_0_35_2_2072 "[TSP] Step 4: erase. \n"
#define TP_FT5206_263_112_2_18_0_35_2_2073 "[TSP] Step 5: start upgrade. \n"
#define TP_FT5206_287_112_2_18_0_35_2_2074 "[TSP] upgrade the 0x%x th byte.\n"
#define TP_FT5206_331_112_2_18_0_35_3_2075 "[TSP] Step 6:  ecc read 0x%x, new firmware 0x%x. \n"
#define TP_FT5206_367_112_2_18_0_35_3_2076 "FW FIRMWARE_ID = 0x%x\n"
#define TP_FT5206_378_112_2_18_0_35_3_2077 "TPC UPDATE FIRMWARE SUCCESS\n"
#define TP_FT5206_382_112_2_18_0_35_3_2078 "TPC UPDATE FIRMWARE FAILED\n"
#define TP_FT5206_427_112_2_18_0_35_3_2079 "TP:: XY_Coordinate Finger[%d] x=%d, y=%d, finger_id = 0x%x!rn"
#define TP_FT5206_443_112_2_18_0_35_3_2080 "FT5206 Reg[0x%x]=0x%x \r\n"
#define TP_FT5206_449_112_2_18_0_35_3_2081 "FT5206 Reg[0x80+%x]=0x%x \r\n"
#define TP_FT5206_455_112_2_18_0_35_3_2082 "FT5206 Reg[0x92+%x]=0x%x \r\n"
#define TP_FT5206_461_112_2_18_0_35_3_2083 "FT5206 Reg[0x9c+%x]=0x%x \r\n"
#define TP_FT5206_467_112_2_18_0_35_3_2084 "FT5206 Reg[0xa6+%x]=0x%x \r\n"
#define TP_FT5206_689_112_2_18_0_35_3_2085 "value freq_cycle time = 0x%x\n"
#define TP_FT5206_698_112_2_18_0_35_3_2086 "value zoom_distance = 0x%x\n"
#define TP_FT5206_748_112_2_18_0_35_3_2087 "TP: FT5206_Resume Retry Count =0x%x,value=0x%x \r\n"
#define TP_MULTI_DRV_95_112_2_18_0_35_4_2088 "TPC LEN 0x%x\r\n"
#define TP_MULTI_DRV_241_112_2_18_0_35_4_2089 "TPC: <Error>data is null\n"
#define TP_MULTI_DRV_281_112_2_18_0_35_4_2090 "TP_DRIVER: enter into TP_Multi_MountGPIOHandler\n"
#define TP_MULTI_DRV_451_112_2_18_0_35_5_2091 " TPC:SIMULATOR, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d."
#define TP_MULTI_SRV_97_112_2_18_0_35_5_2092 "SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d."
#define TP_SRV_86_112_2_18_0_35_5_2093 "App_queue:\n"
#define TPD_DRV_352_112_2_18_0_35_6_2094 "TP_DRIVER: enter into TP_DInit\n"
#define TPD_DRV_357_112_2_18_0_35_6_2095 "********TP_DInit***********\r\n"
#define TPD_DRV_392_112_2_18_0_35_7_2096 "*******TP_DInit finish********"
#define TPD_DRV_531_112_2_18_0_35_7_2097 "TP_DRV: TP UP filter begin, %d!\n\r"
#define TPD_DRV_566_112_2_18_0_35_7_2098 "g_adc_lt[%d].x=%d, g_adc_lt[%d].y=%d"
#define TPD_DRV_579_112_2_18_0_35_7_2099 "TP_DRV: TP UP filter middle %d!\n\r"
#define TPD_DRV_646_112_2_18_0_35_7_2100 "TP_DRV: TP UP filter finish!\n\r"
#define TPD_DRV_662_112_2_18_0_35_7_2101 "TP_DRV: IsGpioMessage\n\r"
#define TPD_DRV_678_112_2_18_0_35_7_2102 "TP_DRV: XL_state = %d\n\r"
#define TPD_DRV_682_112_2_18_0_35_7_2103 "TP_DRV: Start to ADC\n\r"
#define TPD_DRV_734_112_2_18_0_35_7_2104 "temp1.x = %d, temp1.y = %d\n\r"
#define TPD_DRV_735_112_2_18_0_35_7_2105 "temp2.x = %d, temp2.y = %d\n\r"
#define TPD_DRV_736_112_2_18_0_35_7_2106 "temp3.x = %d, temp3.y = %d\n\r"
#define TPD_DRV_737_112_2_18_0_35_7_2107 "delta_x1 = %d, delta_y1 = %d\n\r"
#define TPD_DRV_738_112_2_18_0_35_7_2108 "delta_x2 = %d, delta_y2= %d\n\r"
#define TPD_DRV_739_112_2_18_0_35_7_2109 "delta_x3 = %d, delta_y3= %d\n\r"
#define TPD_DRV_756_112_2_18_0_35_7_2110 "TP_DRV: ADC detected\n\r"
#define TPD_DRV_762_112_2_18_0_35_7_2111 "TP_DRV: DOWN message, x=%d, y=%d\n\r"
#define TPD_DRV_780_112_2_18_0_35_7_2112 "TP_DRV: MOVE message, x=%d, y=%d, time=%dms\n\r"
#define TPD_DRV_790_112_2_18_0_35_7_2113 "TPDRV: UP action happened!  \n\r"
#define TPD_DRV_796_112_2_18_0_35_7_2114 "TPDRV: UP message, x=%d, y=%d\n\r"
#define TPD_DRV_817_112_2_18_0_35_7_2115 "TP_DRV: XL_state = %d\n\r"
#define TPD_DRV_867_112_2_18_0_35_7_2116 "TP_UpdateCalibration:x_top_factor = %d,x_bottom_factor=%d,y_left_factor = %d,y_right_factor=%d "
#define TPD_DRV_884_112_2_18_0_35_8_2117 "TP_DRIVER: enter into TP_MountGPIOHandler\n"
#define TPD_DRV_895_112_2_18_0_35_8_2118 "TP_DRIVER: TP_MountGPIOHandler SUCCES\n"
#define TPD_DRV_918_112_2_18_0_35_8_2119 "TP_DRIVER: enter into TP_GPIOHandler\n"
#define TPD_DRV_937_112_2_18_0_35_8_2120 "TP_DRIVER: PIN detect error in INT\n"
#define TPD_DRV_1009_112_2_18_0_35_8_2121 "TPDRV: ADC x=%d, y=%d.\n\r"
#define TPD_DRV_1017_112_2_18_0_35_8_2122 "TPDRV: Coordinate: x=%d, y=%d.\n\r"
#define TPD_DRV_1066_112_2_18_0_35_8_2123 "TPDRV: Coordinate: x=%d, y=%d.\n\r"
#define TPD_DRV_1069_112_2_18_0_35_8_2124 "TPDRV: TP_Calculate, pen is UP.\n\r"
#define TPD_DRV_1240_112_2_18_0_35_8_2125 "SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d."
#define TPD_DRV_1248_112_2_18_0_35_8_2126 "SC6800: Register_Sensor_Tool_CmdRoutine"
#define ATHR_WIFI_ADAPTER_707_112_2_18_0_35_11_2127 "WIFI:MAC ADDR =   %02X:%02X:%02X:%02X:%02X:%02Xn"
#define ATHR_WIFI_ADAPTER_920_112_2_18_0_35_11_2128 ":%s(%s)\n"
#define ATHR_WIFI_ADAPTER_929_112_2_18_0_35_11_2129 ":%s(%s)\n"
#define ATHR_WIFI_ADAPTER_950_112_2_18_0_35_11_2130 ":%s(%s)\n"
#define ATHR_WIFI_ADAPTER_1058_112_2_18_0_35_11_2131 "length not right, should be %d\n"
#define ATHR_WIFI_ADAPTER_1073_112_2_18_0_35_11_2132 "length not right, should be %d\n"
#define WIFISUPP_EUT_AR6302_104_112_2_18_0_35_12_2133 "WIFISUPP_EUTSetTxFactor pcl=%d, tx_factor=%d"
#define CSR_IP_ETHER_SEF_373_112_2_18_0_35_13_2134 "STACK: current stack ptr: 0x%08X | stack_size = %d"
#define CSR_SPI_MASTER_105_112_2_18_0_35_15_2135 "CSR: %s - No ISR handlers registered"
#define CSR_SPI_MASTER_120_112_2_18_0_35_15_2136 "CsrSpiMasterInitialise"
#define CSR_SPI_MASTER_180_112_2_18_0_35_15_2137 "Failed to Open SPI Port"
#define CSR_SPI_MASTER_189_112_2_18_0_35_15_2138 "Failed to Init SPI Port"
#define CSR_SPI_MASTER_238_112_2_18_0_35_15_2139 "SPI Port initialised"
#define CSR_SPI_MASTER_246_112_2_18_0_35_15_2140 "CsrSpiMasterDeinitialise"
#define CSR_SPI_MASTER_347_112_2_18_0_35_15_2141 "%s r %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:"
#define CSR_SPI_MASTER_358_112_2_18_0_35_15_2142 "%s w %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:"
#define CSR_SPI_MASTER_373_112_2_18_0_35_15_2143 "%s r %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:"
#define CSR_SPI_MASTER_383_112_2_18_0_35_15_2144 "%s w %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:"
#define CSR_SPI_MASTER_593_112_2_18_0_35_16_2145 "CSR: %s - NULL data pointer!"
#define CSR_SPI_MASTER_760_112_2_18_0_35_16_2146 "CsrSpiMasterBusClockFrequencySet: %d"
#define CSR_SPI_MASTER_779_112_2_18_0_35_16_2147 "CsrSpiMasterCallbackRegister"
#define CSR_SPI_MASTER_790_112_2_18_0_35_16_2148 "CsrSpiMasterCallbackRegister End"
#define SDIO_TEST_54_112_2_18_0_35_21_2149 "%s"
#define SDIO_TEST_57_112_2_18_0_35_21_2150 "%02X:"
#define SDIO_TEST_59_112_2_18_0_35_21_2151 "\n"
#define SDIO_TEST_118_112_2_18_0_35_22_2152 "Waiting for SDIO functions to register: %d\n"
#define SDIO_TEST_126_112_2_18_0_35_22_2153 "Device information:\n"
#define SDIO_TEST_127_112_2_18_0_35_22_2154 "Manufacturer: 0x%04X\n"
#define SDIO_TEST_128_112_2_18_0_35_22_2155 "Card: 0x%04X\n"
#define SDIO_TEST_129_112_2_18_0_35_22_2156 "Function: %d\n"
#define SDIO_TEST_130_112_2_18_0_35_22_2157 "Standard Interface: %d\n"
#define SDIO_TEST_131_112_2_18_0_35_22_2158 "Blocksize: %d\n"
#define SDIO_TEST_132_112_2_18_0_35_22_2159 "Features:0x%08X\n"
#define SDIO_TEST_141_112_2_18_0_35_22_2160 "Testing enable function %d\n"
#define SDIO_TEST_153_112_2_18_0_35_22_2161 "Enable function is verified\n"
#define SDIO_TEST_161_112_2_18_0_35_22_2162 "Testing disable function %d\n"
#define SDIO_TEST_173_112_2_18_0_35_22_2163 "Disable function is verified\n"
#define SDIO_TEST_180_112_2_18_0_35_22_2164 "Testing set frequency, function %d, frequency %d\n"
#define SDIO_TEST_192_112_2_18_0_35_22_2165 "validateBuffer failed at byte %d\n"
#define SDIO_TEST_216_112_2_18_0_35_22_2166 "Testing CsrSdioRead/Write length %d, totalTestCount: %d\n"
#define SDIO_TEST_227_112_2_18_0_35_22_2167 "Writing remainder, which is %d bytes\n"
#define SDIO_TEST_236_112_2_18_0_35_22_2168 "Reading remainder, which is %d bytes\n"
#define WIFISUPP_EUT_CSR6027_75_112_2_18_0_35_58_2169 "Error, %d, NOT-SUPPORTED\n"
#define WIFISUPP_EUT_CSR6027_162_112_2_18_0_35_58_2170 "+++++ 0x%X, Result=%d +++++\n"
#define WIFISUPP_EUT_CSR6027_176_112_2_18_0_35_58_2171 "PtestChipID: 0x%04X\n"
#define WIFISUPP_EUT_CSR6027_186_112_2_18_0_35_58_2172 "PtestFwBuildId: %s\n"
#define WIFISUPP_EUT_CSR6027_481_112_2_18_0_35_59_2173 "last rx-result info\r\n"
#define WIFISUPP_EUT_CSR6027_482_112_2_18_0_35_59_2174 "  good:  %d\r\n"
#define WIFISUPP_EUT_CSR6027_483_112_2_18_0_35_59_2175 "   bad: %d\r\n"
#define WIFISUPP_EUT_CSR6027_484_112_2_18_0_35_59_2176 "total:    %d\r\n"
#define WIFISUPP_EUT_CSR6027_486_112_2_18_0_35_59_2177 "   PER:  %d %%\r\n"
#define WIFISUPP_EUT_CSR6027_525_112_2_18_0_35_59_2178 "<WIFISUPP_EUTSetChannel = %d>\r\n"
#define WIFISUPP_EUT_CSR6027_539_112_2_18_0_35_59_2179 "<WIFISUPP_EUTSetDataRatio, gTestRate = %d (long ratio=%d)>\r\n"
#define WIFISUPP_EUT_CSR6027_562_112_2_18_0_35_59_2180 "<WIFISUPP_EUTSetMode = %d>\r\n"
#define WIFISUPP_EUT_CSR6027_576_112_2_18_0_35_59_2181 "<WIFISUPP_EUTSetTxFactor, pcl = %d, tx_factor = %d>\r\n"
#define WIFISUPP_EUT_CSR6027_589_112_2_18_0_35_59_2182 "<WIFISUPP_EUTSetPCL, pcl = %d\r\n"
#define WIFISUPP_EUT_CSR6027_602_112_2_18_0_35_59_2183 "<WIFISUPP_EUTSetTx, is_tx_on = %d, tx_packet_mode = %d\r\n"
#define WIFISUPP_EUT_CSR6027_629_112_2_18_0_35_59_2184 "<WIFISUPP_EUTSetRx, rx_command = %d\r\n"
#define WIFISUPP_EUT_CSR6027_712_112_2_18_0_35_59_2185 "CSR:WIFI_GetVersion - %s"
#define CSR_IP_ETHER_SEF_270_112_2_18_0_36_0_2186 "STACK: current stack ptr: 0x%08X | stack_size = %d"
#define CSR_SPI_MASTER_88_112_2_18_0_36_1_2187 "CsrSpiMasterInitialise"
#define CSR_SPI_MASTER_111_112_2_18_0_36_1_2188 "Failed to Open SPI Port"
#define CSR_SPI_MASTER_124_112_2_18_0_36_2_2189 "SPI Port initialised"
#define CSR_SPI_MASTER_130_112_2_18_0_36_2_2190 "CsrSpiMasterDeinitialise"
#define CSR_SPI_MASTER_194_112_2_18_0_36_2_2191 "CsrSpiMasterReadAsync"
#define CSR_SPI_MASTER_195_112_2_18_0_36_2_2192 "Not implemented"
#define CSR_SPI_MASTER_207_112_2_18_0_36_2_2193 "CsrSpiMasterWriteAsync"
#define CSR_SPI_MASTER_208_112_2_18_0_36_2_2194 "Not implemented"
#define CSR_SPI_MASTER_230_112_2_18_0_36_2_2195 "CsrSpiMasterBusClockFrequencySet: %d"
#define CSR_SPI_MASTER_248_112_2_18_0_36_2_2196 "CsrSpiMasterCallbackRegister"
#define CSR_SPI_MASTER_286_112_2_18_0_36_2_2197 "CsrSpiMasterCallbackRegister End"
#define WIFISUPP_EUT_CSR6030_75_112_2_18_0_36_14_2198 "Error, %d, NOT-SUPPORTED\n"
#define WIFISUPP_EUT_CSR6030_162_112_2_18_0_36_15_2199 "+++++ 0x%X, Result=%d +++++\n"
#define WIFISUPP_EUT_CSR6030_176_112_2_18_0_36_15_2200 "PtestChipID: 0x%04X\n"
#define WIFISUPP_EUT_CSR6030_186_112_2_18_0_36_15_2201 "PtestFwBuildId: %s\n"
#define WIFISUPP_EUT_CSR6030_488_112_2_18_0_36_15_2202 "last rx-result info\r\n"
#define WIFISUPP_EUT_CSR6030_489_112_2_18_0_36_15_2203 "  good:  %d\r\n"
#define WIFISUPP_EUT_CSR6030_490_112_2_18_0_36_15_2204 "   bad: %d\r\n"
#define WIFISUPP_EUT_CSR6030_491_112_2_18_0_36_15_2205 "total:    %d\r\n"
#define WIFISUPP_EUT_CSR6030_493_112_2_18_0_36_15_2206 "   PER:  %d %%\r\n"
#define WIFISUPP_EUT_CSR6030_532_112_2_18_0_36_16_2207 "<WIFISUPP_EUTSetChannel = %d>\r\n"
#define WIFISUPP_EUT_CSR6030_546_112_2_18_0_36_16_2208 "<WIFISUPP_EUTSetDataRatio, gTestRate = %d (long ratio=%d)>\r\n"
#define WIFISUPP_EUT_CSR6030_569_112_2_18_0_36_16_2209 "<WIFISUPP_EUTSetMode = %d>\r\n"
#define WIFISUPP_EUT_CSR6030_583_112_2_18_0_36_16_2210 "<WIFISUPP_EUTSetTxFactor, pcl = %d, tx_factor = %d>\r\n"
#define WIFISUPP_EUT_CSR6030_596_112_2_18_0_36_16_2211 "<WIFISUPP_EUTSetPCL, pcl = %d\r\n"
#define WIFISUPP_EUT_CSR6030_609_112_2_18_0_36_16_2212 "<WIFISUPP_EUTSetTx, is_tx_on = %d, tx_packet_mode = %d\r\n"
#define WIFISUPP_EUT_CSR6030_636_112_2_18_0_36_16_2213 "<WIFISUPP_EUTSetRx, rx_command = %d\r\n"
#define WIFISUPP_EUT_CSR6030_719_112_2_18_0_36_16_2214 "CSR:WIFI_GetVersion - %s"
#define RTL8723_WIFI_ADAPTER_291_112_2_18_0_36_17_2215 "&&&&&&&&rtl_createThread(): priority is %d\n"
#define RTL8723_WIFI_ADAPTER_470_112_2_18_0_36_17_2216 "MAC ADDR =   %02X:%02X:%02X:%02X:%02X:%02Xn"
#define RTL8723_WIFI_ADAPTER_541_112_2_18_0_36_17_2217 "======>rtl8723_supp_notifyOnCnf result=%d, pResult->net_id is 0x%x"
#define RTL8723_WIFI_ADAPTER_566_112_2_18_0_36_17_2218 "<=======rtl8723_supp_notifyOnCnf\n"
#define RTL8723_WIFI_ADAPTER_577_112_2_18_0_36_17_2219 "WIFI para_supp_notifyOffCnf result=%d"
#define RTL8723_WIFI_ADAPTER_593_112_2_18_0_36_17_2220 "WIFI para_supp_notifyConnectCnf result=%d"
#define RTL8723_WIFI_ADAPTER_617_112_2_18_0_36_17_2221 "WIFI para_supp_notifyScanInd"
#define RTL8723_WIFI_ADAPTER_646_112_2_18_0_36_17_2222 "WIFI param_supp_notifyScanCnf"
#define RTL8723_WIFI_ADAPTER_661_112_2_18_0_36_17_2223 "WIFI param_supp_notifyDisconnectCnf result=%d"
#define RTL8723_WIFI_ADAPTER_680_112_2_18_0_36_17_2224 "WIFI param_supp_notifyDisconnectInd"
#define RTL8723_WIFI_ADAPTER_748_112_2_18_0_36_17_2225 "wifi: para_GetMAC is %x-%x-%x-%x-%x-%x"
#define RTL8723_WIFI_ADAPTER_789_112_2_18_0_36_17_2226 "wifi: para_GetMAC is %x-%x-%x-%x-%x-%x, ret=%d"
#define RTL8723_WIFI_ADAPTER_805_112_2_18_0_36_18_2227 "&&&&&&&&&rtl8723_wifi_supplicant_turn_on()\n"
#define RTL8723_WIFI_ADAPTER_809_112_2_18_0_36_18_2228 "&&&&&&&&&&&&Failed to Open SPI Port\n"
#define RTL8723_WIFI_ADAPTER_817_112_2_18_0_36_18_2229 "rtl8723_wifi_supplicant_turn_on wifi_hw_open_func fail\n"
#define RTL8723_WIFI_ADAPTER_822_112_2_18_0_36_18_2230 "&&&&&rtl8723_wifi_supplicant_turn_on(): netid is 0x%x"
#define RTL8723_WIFI_ADAPTER_900_112_2_18_0_36_18_2231 "&&&&&&&&&rtl8723_wifi_on()\n"
#define RTL8723_WIFI_ADAPTER_908_112_2_18_0_36_18_2232 "&&&&&&&&&rtl8723_wifi_off()\n"
#define RTL8723_WIFI_ADAPTER_923_112_2_18_0_36_18_2233 "&&&&&&&&&rtl8723_wifi_connect()\n"
#define RTL8723_WIFI_ADAPTER_929_112_2_18_0_36_18_2234 "&&&&&&&&&rtl8723_wifi_disconnect()\n"
#define RTL8723_WIFI_ADAPTER_935_112_2_18_0_36_18_2235 "&&&&&&&&&rtl8723_handle_cus_msg()\n"
#define RTL8723_WIFI_ADAPTER_940_112_2_18_0_36_18_2236 "&&&&&&&&&rtl8723_get_connection_info()\n"
#define RTL8723_WIFI_ADAPTER_946_112_2_18_0_36_18_2237 "&&&&&&&&&para_getThread_Identify()\n"
#define RTL8723_WIFI_ADAPTER_993_112_2_18_0_36_18_2238 ":%s(%d) str = %s \n"
#define RTL8723_WIFI_ADAPTER_1005_112_2_18_0_36_18_2239 ":%s(%d) str = %s\n"
#define RTL8723_WIFI_ADAPTER_1017_112_2_18_0_36_18_2240 ":%s(%d) str = %s\n"
#define RTL8723_WIFI_ADAPTER_1050_112_2_18_0_36_18_2241 ":%s(%s)\n"
#define RTL8723_WIFI_ADAPTER_1055_112_2_18_0_36_18_2242 ":%s(%s)\n"
#define RTL8723_WIFI_ADAPTER_1075_112_2_18_0_36_18_2243 ":%s(%s)\n"
#define RTL8723_WIFI_ADAPTER_1081_112_2_18_0_36_18_2244 ":%s(%s)\n"
#define RTL8723_WIFI_ADAPTER_1087_112_2_18_0_36_18_2245 ":%s(%s)\n"
#define RTL8723_WIFI_ADAPTER_1136_112_2_18_0_36_18_2246 "length not right, should be %d\n"
#define RTL8723_WIFI_ADAPTER_1151_112_2_18_0_36_18_2247 "length not right, should be %d\n"
#define RTL8723_WIFI_ADAPTER_1216_112_2_18_0_36_18_2248 "rtl8723_set_eut_mode enter =====>\n"
#define RTL8723_WIFI_ADAPTER_1224_112_2_18_0_36_18_2249 "rtl8723_set_eut_mode leave =====>\n"
#define RTL8723_WIFI_ADAPTER_1237_112_2_18_0_36_18_2250 "&&&&&&&&&&&&&WIFI_GetVersion: is %s\n"
#define WIFISUPP_ADAP_REALTEK_116_112_2_18_0_36_19_2251 "&&&&&&&&&Rtl8723WifiOn()\n"
#define WIFISUPP_ADAP_REALTEK_126_112_2_18_0_36_19_2252 "&&&&&&&&&Rtl8723WifiOff()\n"
#define WIFISUPP_ADAP_REALTEK_136_112_2_18_0_36_19_2253 "&&&&&&&&&Rtl8723WifiScan()\n"
#define WIFISUPP_ADAP_REALTEK_146_112_2_18_0_36_19_2254 "&&&&&&&&&Rtl8723WifiConnect()\n"
#define WIFISUPP_ADAP_REALTEK_156_112_2_18_0_36_19_2255 "&&&&&&&&&Rtl8723WifiDisconnect()\n"
#define WIFISUPP_ADAP_REALTEK_166_112_2_18_0_36_19_2256 "&&&&&&&&&Rtl8723HandleCusmsg()\n"
#define WIFISUPP_ADAP_REALTEK_179_112_2_18_0_36_19_2257 "&&&&&&&&&Rtl8723WifiGetConnectionInfo()\n"
#define WIFISUPP_EUT_REALTEK_58_112_2_18_0_36_19_2258 "WIFISUPP_EUTEnterMode %d\n"
#define WIFISUPP_EUT_REALTEK_107_112_2_18_0_36_19_2259 "WIFISUPP_EUTSetTxFactor pcl=%d, tx_factor=%d"
#define WIFISUPP_EUT_REALTEK_201_112_2_18_0_36_19_2260 "WIFISUPP_EUTCusData\n"
#define WIFI_DRV_101_112_2_18_0_36_20_2261 "_WIFI_SPI_Open(...)"
#define WIFI_DRV_137_112_2_18_0_36_20_2262 "_WIFI_SPI_Open Open failed!!"
#define WIFI_DRV_157_112_2_18_0_36_20_2263 "_WIFI_SPI_Read: port_num = %d, buf_ptr = 0x%x, lenght = %d"
#define WIFI_DRV_167_112_2_18_0_36_20_2264 "WIFI_SPI_1 read failed!!"
#define WIFI_DRV_187_112_2_18_0_36_20_2265 "_WIFI_SPI_Write: port_num = %d, buf_ptr = 0x%x, lenght = %d"
#define WIFI_DRV_197_112_2_18_0_36_20_2266 "WIFI_SPI_1 write failed!!"
#define WIFI_DRV_215_112_2_18_0_36_20_2267 "_WIFI_SPI_ReadWrite: port_num = %d, buf_ptr_in = 0x%x, buf_ptr_out = 0x%x, lenght = %d"
#define WIFI_DRV_221_112_2_18_0_36_20_2268 "WIFI_SPI_1 write failed!!"
#define WIFI_DRV_238_112_2_18_0_36_20_2269 "_WIFI_SPI_Write_Asyn: port_num = %d, buf_ptr = 0x%x, lenght = %d,read_call_func=%p"
#define WIFI_DRV_244_112_2_18_0_36_20_2270 "WIFI_SPI_1 write failed!!"
#define WIFI_DRV_261_112_2_18_0_36_20_2271 "_WIFI_SPI_Read_Asyn: port_num = %d, buf_ptr = 0x%x, lenght = %d"
#define WIFI_DRV_267_112_2_18_0_36_20_2272 "WIFI_SPI_1 write failed!!"
#define WIFI_DRV_280_112_2_18_0_36_21_2273 "_WIFI_SPI_Close: port_num = %d"
#define WIFI_DRV_526_112_2_18_0_36_21_2274 "s_wifi_port.WIFI_Read_Asyn"
#define WIFI_DRV_534_112_2_18_0_36_21_2275 "WIFI_Read_Asyn"
#define WIFI_DRV_555_112_2_18_0_36_21_2276 "s_wifi_port.write_asyn"
#define WIFI_DRV_563_112_2_18_0_36_21_2277 "WIFI_RET_STATUS_ERR"
#define WIFI_DRV_739_112_2_18_0_36_22_2278 "wifi WIFI_GetNvParam:%x%x%x%x%x%x"
#define WIFISUPP_API_85_112_2_18_0_36_22_2279 "WIFISUPP WIFISUPP_OnReq, invalid thread id!"
#define WIFISUPP_API_92_112_2_18_0_36_22_2280 "WIFISUPP WIFISUPP_OnReq, alloc sig_ptr fail!"
#define WIFISUPP_API_114_112_2_18_0_36_22_2281 "WIFISUPP WIFISUPP_OffReq, alloc sig_ptr fail!"
#define WIFISUPP_API_132_112_2_18_0_36_22_2282 "WIFI:error:SUPP:%s scan_info_ptr is PNULL!"
#define WIFISUPP_API_140_112_2_18_0_36_22_2283 "WIFI:error:SUPP:%s alloc sig_ptr fail!"
#define WIFISUPP_API_164_112_2_18_0_36_22_2284 "WIFISUPP WIFISUPP_ConnectReq, invalid input!"
#define WIFISUPP_API_171_112_2_18_0_36_22_2285 "WIFISUPP WIFISUPP_ConnectReq, alloc sig_ptr fail!"
#define WIFISUPP_API_194_112_2_18_0_36_22_2286 "WIFISUPP WIFISUPP_DisconnectReq, invalid input!"
#define WIFISUPP_API_201_112_2_18_0_36_22_2287 "WIFISUPP WIFISUPP_DisconnectReq, alloc sig_ptr fail!"
#define WIFISUPP_API_255_112_2_18_0_36_22_2288 "WIFISUPP SerializeConnectReq tls using buffer alloc fail"
#define WIFISUPP_API_275_112_2_18_0_36_22_2289 "WIFISUPP SerializeConnectReq tls using file alloc fail"
#define WIFISUPP_API_294_112_2_18_0_36_22_2290 "WIFISUPP SerializeConnectReq fast using buffer alloc fail"
#define WIFISUPP_API_317_112_2_18_0_36_23_2291 "WIFISUPP SerializeConnectReq fast using file alloc fail"
#define WIFISUPP_API_330_112_2_18_0_36_23_2292 "WIFISUPP SerializeConnectReq other eap alloc fail"
#define WIFISUPP_API_343_112_2_18_0_36_23_2293 "WIFISUPP SerializeConnectReq wpa psk alloc fail"
#define WIFISUPP_API_362_112_2_18_0_36_23_2294 "WIFI:error:SUPP:%s as using buffer alloc fail"
#define WIFISUPP_API_384_112_2_18_0_36_23_2295 "WIFISUPP SerializeConnectReq wep alloc fail"
#define WIFISUPP_API_407_112_2_18_0_36_23_2296 "WIFI:error:Supp:GetMac failed %d!"
#define WIFISUPP_API_414_112_2_18_0_36_23_2297 "WIFI:info:Supp:GetMAC is %x-%x-%x-%x-%x-%x"
#define WIFISUPP_EUT_256_112_2_18_0_36_23_2298 "req_ptr->len=%d\n"
#define WIFISUPP_EUT_259_112_2_18_0_36_23_2299 "WIFISUPP_EUTHandleReq req_ptr->len > WIFISUPP_COMMAND_MAX_LEN"
#define WIFISUPP_EUT_267_112_2_18_0_36_23_2300 "wifi: WIFISUPP_EUTHandleReq command_code = %d"
#define WIFISUPP_EUT_281_112_2_18_0_36_23_2301 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_ENTER_MODE bEnterOrLeave=%d"
#define WIFISUPP_EUT_319_112_2_18_0_36_23_2302 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_CHANNEL channel=%x"
#define WIFISUPP_EUT_333_112_2_18_0_36_23_2303 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_DATA_RATIO ratio=%x"
#define WIFISUPP_EUT_346_112_2_18_0_36_23_2304 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_MODE mode=%x"
#define WIFISUPP_EUT_360_112_2_18_0_36_23_2305 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_PCL pcl=%x"
#define WIFISUPP_EUT_376_112_2_18_0_36_24_2306 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_TX tx_on_off=%x"
#define WIFISUPP_EUT_377_112_2_18_0_36_24_2307 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_TX tx_packet_mode=%x"
#define WIFISUPP_EUT_391_112_2_18_0_36_24_2308 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_RX rx_command=%x"
#define WIFISUPP_EUT_397_112_2_18_0_36_24_2309 "WIFISUPP_EUT_CMD_READ_RX_VAL"
#define WIFISUPP_EUT_419_112_2_18_0_36_24_2310 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_TXFACTOR tx_factor=%x"
#define WIFISUPP_EUT_428_112_2_18_0_36_24_2311 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_CUSTOM_DATA:  len is %d\n"
#define WIFISUPP_EUT_447_112_2_18_0_36_24_2312 "WIFISUPP_EUT_CMD_READ_CALDATA"
#define WIFISUPP_EUT_455_112_2_18_0_36_24_2313 "WIFISUPP_EUT_CMD_READ_CALDATA"
#define WIFISUPP_EUT_484_112_2_18_0_36_24_2314 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_READ_REGISTER addr=%x my_to_read=%x"
#define WIFISUPP_EUT_492_112_2_18_0_36_24_2315 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_READ_REGISTER failed"
#define WIFISUPP_EUT_513_112_2_18_0_36_24_2316 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_WRITE_REGISTER addr1=%x write_temp=%x"
#define WIFISUPP_EUT_524_112_2_18_0_36_24_2317 "wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_WRITE_REGISTER failed %d"
#define WIFISUPP_EUT_567_112_2_18_0_36_24_2318 "WIFISUPP_CheckStatus context_ptr == PNULL"
#define WIFISUPP_EUT_833_112_2_18_0_36_24_2319 ":%x\n"
#define WIFISUPP_EUT_863_112_2_18_0_36_24_2320 "WIFISUPP_EUTSendCnf command_code=%d"
#define WIFISUPP_EUT_888_112_2_18_0_36_25_2321 "WIFISUPP_EUTSendCnf command_code=%d"
#define WIFISUPP_EUT_906_112_2_18_0_36_25_2322 "WIFISUPP_EUTSendCnf command_code=%d"
#define WIFISUPP_EUT_954_112_2_18_0_36_25_2323 "WIFISUPP_EUTSendCnf command_code=%d"
#define WIFISUPP_EUT_1037_112_2_18_0_36_25_2324 "WIFISUPP_EUTSyncFinished context_ptr == PNULL"
#define WIFISUPP_EUT_1129_112_2_18_0_36_25_2325 "WIFISUPP_EUTReadRxValCnf context_ptr == PNULL"
#define WIFISUPP_EUT_1154_112_2_18_0_36_25_2326 "WIFISUPP_EUTReadCalDataCnf context_ptr == PNULL"
#define WIFISUPP_EUT_1174_112_2_18_0_36_25_2327 "WIFISUPP_EUTReadRxValCnf context_ptr == PNULL"
#define WIFISUPP_MAIN_104_112_2_18_0_36_25_2328 "WIFISUPP creat thread: %x"
#define WIFISUPP_MAIN_123_112_2_18_0_36_25_2329 "WIFISUPP SUPPADAP_SendSig msg_info_ptr==0!"
#define WIFISUPP_MAIN_128_112_2_18_0_36_25_2330 "WIFISUPP SUPPADAP_SendSig send_to_thread_id==0!"
#define WIFISUPP_MAIN_134_112_2_18_0_36_25_2331 "SUPPMAIN_NotifyApp WIFISUPP_SIG_ON_CNF"
#define WIFISUPP_MAIN_142_112_2_18_0_36_25_2332 "SUPPMAIN_NotifyApp WIFISUPP_SIG_OFF_CNF"
#define WIFISUPP_MAIN_150_112_2_18_0_36_25_2333 "SUPPMAIN_NotifyApp WIFISUPP_SIG_SCAN_IND"
#define WIFISUPP_MAIN_158_112_2_18_0_36_25_2334 "SUPPMAIN_NotifyApp WIFISUPP_SIG_SCAN_CNF"
#define WIFISUPP_MAIN_166_112_2_18_0_36_25_2335 "SUPPMAIN_NotifyApp WIFISUPP_SIG_CONNECT_CNF"
#define WIFISUPP_MAIN_174_112_2_18_0_36_25_2336 "SUPPMAIN_NotifyApp WIFISUPP_SIG_DISCONNECT_CNF"
#define WIFISUPP_MAIN_182_112_2_18_0_36_26_2337 "SUPPMAIN_NotifyApp WIFISUPP_SIG_DISCONNECT_IND"
#define WIFISUPP_MAIN_190_112_2_18_0_36_26_2338 "SUPPMAIN_NotifyApp WIFISUPP_SIG_BSSID_CHANGED_IND"
#define WIFISUPP_MAIN_199_112_2_18_0_36_26_2339 "SUPPMAIN_NotifyApp WIFISUPP_SIG_RSSI_CHANGED_IND"
#define WIFISUPP_MAIN_207_112_2_18_0_36_26_2340 "WIFISUPP CreatWifiMsg, invalid msg id!"
#define WIFISUPP_MAIN_246_112_2_18_0_36_26_2341 "WifiSuppTaskEntry, id:%x"
#define WIFISUPP_MAIN_252_112_2_18_0_36_26_2342 "WIFISUPP task entry: Customized signal received,id:%x"
#define WIFISUPP_MAIN_264_112_2_18_0_36_26_2343 "WIFISUPP task entry: WIFISUPP_SIG_ON_REQ,app:%d"
#define WIFISUPP_MAIN_269_112_2_18_0_36_26_2344 "WIFISUPP task entry: WIFISUPP_SIG_OFF_REQ"
#define WIFISUPP_MAIN_273_112_2_18_0_36_26_2345 "WIFISUPP task entry: WIFISUPP_SIG_SCAN_REQ"
#define WIFISUPP_MAIN_286_112_2_18_0_36_26_2346 "WIFISUPP task entry: WIFISUPP_SIG_CONNECT_REQ"
#define WIFISUPP_MAIN_295_112_2_18_0_36_26_2347 "WIFISUPP task entry: WIFISUPP_SIG_DISCONNECT_REQ"
#define WIFISUPP_MAIN_300_112_2_18_0_36_26_2348 "WIFISUPP task entry: unwanted signal received!"
#define WIFISUPP_MAIN_314_112_2_18_0_36_26_2349 "WifiSupp_main.c::WifiOn called"
#define WIFISUPP_MAIN_318_112_2_18_0_36_26_2350 "WIFISUPP WifiOn, custom supplicant info NOT available!"
#define WIFISUPP_MAIN_323_112_2_18_0_36_26_2351 "WIFISUPP WifiOn, app thread is null!"
#define WIFISUPP_MAIN_328_112_2_18_0_36_26_2352 "WIFISUPP WifiOn, wifi_on_func is null!"
#define WIFISUPP_MAIN_344_112_2_18_0_36_26_2353 "WIFISUPP WifiOff, custom supplicant info NOT available!"
#define WIFISUPP_MAIN_349_112_2_18_0_36_26_2354 "WIFISUPP WifiOff, wifi_off_func is null!"
#define WIFISUPP_MAIN_365_112_2_18_0_36_26_2355 "WIFI:info:SUPP:%s invalid input!"
#define WIFISUPP_MAIN_369_112_2_18_0_36_26_2356 "WIFI:info:SUPP:%s ssid_len=%d"
#define WIFISUPP_MAIN_371_112_2_18_0_36_26_2357 "WIFI:info:SUPP:%s ESSID =%s"
#define WIFISUPP_MAIN_375_112_2_18_0_36_26_2358 "WIFI:error:SUPP:%s custom supplicant info NOT available!"
#define WIFISUPP_MAIN_381_112_2_18_0_36_26_2359 "WIFI:error:SUPP:%s wifi_scan_func is null!"
#define WIFISUPP_MAIN_398_112_2_18_0_36_26_2360 "WIFISUPP WifiConnect, invalid input!"
#define WIFISUPP_MAIN_403_112_2_18_0_36_26_2361 "WifiConnect: ESSID =%s"
#define WIFISUPP_MAIN_405_112_2_18_0_36_26_2362 "WifiConnect: BSSID: %02X:%02X:%02X:%02X:%02X:%02X"
#define WIFISUPP_MAIN_406_112_2_18_0_36_26_2363 "WifiConnect encryp type =%d"
#define WIFISUPP_MAIN_407_112_2_18_0_36_26_2364 "WifiConnect network_mode =%d"
#define WIFISUPP_MAIN_408_112_2_18_0_36_26_2365 "WifiConnect chanel =%d"
#define WIFISUPP_MAIN_409_112_2_18_0_36_26_2366 "WifiConnect pairwise_cipher =%d"
#define WIFISUPP_MAIN_410_112_2_18_0_36_26_2367 "WifiConnect group_cipher =%d"
#define WIFISUPP_MAIN_413_112_2_18_0_36_26_2368 "WifiConnect WEP type =%d"
#define WIFISUPP_MAIN_417_112_2_18_0_36_26_2369 "WifiConnect WEP 64bit key 1 = %x,%x,%x,%x,%x"
#define WIFISUPP_MAIN_420_112_2_18_0_36_26_2370 "WifiConnect WEP 64bit key 2 = %x,%x,%x,%x,%x"
#define WIFISUPP_MAIN_423_112_2_18_0_36_26_2371 "WifiConnect WEP 64bit key 3 = %x,%x,%x,%x,%x"
#define WIFISUPP_MAIN_426_112_2_18_0_36_26_2372 "WifiConnect WEP 64bit key 4 = %x,%x,%x,%x,%x"
#define WIFISUPP_MAIN_434_112_2_18_0_36_26_2373 "WifiConnect WEP 128bit key 1 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
#define WIFISUPP_MAIN_440_112_2_18_0_36_26_2374 "WifiConnect WEP 128bit key 2 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
#define WIFISUPP_MAIN_446_112_2_18_0_36_26_2375 "WifiConnect WEP 128bit key 3 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
#define WIFISUPP_MAIN_452_112_2_18_0_36_26_2376 "WifiConnect WEP 128bit key 4 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
#define WIFISUPP_MAIN_457_112_2_18_0_36_26_2377 "WifiConnect wpa type=%d"
#define WIFISUPP_MAIN_458_112_2_18_0_36_26_2378 "WifiConnect wpa =%s"
#define WIFISUPP_MAIN_462_112_2_18_0_36_26_2379 "WifiConnect wpa type=%d"
#define WIFISUPP_MAIN_463_112_2_18_0_36_26_2380 "WifiConnect wpa =%s"
#define WIFISUPP_MAIN_467_112_2_18_0_36_26_2381 "WifiConnect WAPI type is %d"
#define WIFISUPP_MAIN_470_112_2_18_0_36_26_2382 "WifiConnect WAPI psk = %s"
#define WIFISUPP_MAIN_480_112_2_18_0_36_26_2383 "WIFISUPP WifiConnect, custom supplicant info NOT available!"
#define WIFISUPP_MAIN_485_112_2_18_0_36_26_2384 "WIFISUPP WifiConnect, wifi_connect_func is null!"
#define WIFISUPP_MAIN_499_112_2_18_0_36_26_2385 "WIFISUPP WifiDisconnect, invalid input!"
#define WIFISUPP_MAIN_505_112_2_18_0_36_26_2386 "WIFISUPP WifiDisconnect, custom supplicant info NOT available!"
#define WIFISUPP_MAIN_510_112_2_18_0_36_26_2387 "WIFISUPP WifiDisconnect, wifi_connect_func is null!"
#define WIFISUPP_MAIN_525_112_2_18_0_36_26_2388 "WIFISUPP WifiHandleCusMsg, custom supplicant info NOT available!"
#define WIFISUPP_MAIN_530_112_2_18_0_36_26_2389 "WIFISUPP WifiHandleCusMsg, wifi_handle_cus_msg_func is null!"
#define WIFISUPP_MAIN_544_112_2_18_0_36_26_2390 "WIFI:error:SUPP:%s command_info_ptr is PNULL!"
#define WIFISUPP_MAIN_550_112_2_18_0_36_26_2391 "WIFI:error:SUPP:%s s_suppadap_custom_info_ptr is PNULL!"
#define WIFISUPP_MAIN_556_112_2_18_0_36_26_2392 "WIFI:error:SUPP:%s wifi_command_func is PNULL!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_CUSTOMIZE_TRC)
TRACE_MSG(ADC_PARAMETER_81_112_2_18_0_26_13_0,"ENVIRONMENT temper_adcenv %d, Temper %d\r\n")
TRACE_MSG(LCM_PROD_129_112_2_18_0_26_14_1,"ADC: %d[%d]")
TRACE_MSG(LCM_PROD_135_112_2_18_0_26_14_2,"_LCM_GetAdcVal: ADC Val -> 0x%x")
TRACE_MSG(LCM_PROD_187_112_2_18_0_26_14_3,"_LCM_AdcIdentify: index %d, adc %d, min %d, max %d")
TRACE_MSG(LCM_PROD_194_112_2_18_0_26_14_4,"_LCM_AdcIdentify: Match succ!")
TRACE_MSG(LCM_PROD_228_112_2_18_0_26_14_5,"_LCM_ReadID: lcd_id=%d ,lcm_dev_id = 0x%x!")
TRACE_MSG(LCM_PROD_323_112_2_18_0_26_14_6,"_LCM_Identify: already done!")
TRACE_MSG(LCM_PROD_354_112_2_18_0_26_14_7,"_LCM_Identify: %d LCM unused")
TRACE_MSG(LCM_PROD_360_112_2_18_0_26_14_8,"_LCM_Identify: %d LCM used default config info")
TRACE_MSG(PROD_PARAM_483_112_2_18_0_26_17_9,"[chr_drv] REF_GetAdcCalibrationParaEx adc_ptr alloc fail!!")
TRACE_MSG(PROD_PARAM_534_112_2_18_0_26_17_10,"[chr_drv] PRODINIT: battery0 = 0x%X, battery1 = 0x%X, ")
TRACE_MSG(PROD_PARAM_584_112_2_18_0_26_17_11,"[chr_drv] PRODINIT2: battery0 = 0x%X, battery1 = 0x%X, ")
TRACE_MSG(PROD_PARAM_663_112_2_18_0_26_17_12,"[chr_drv] slope_m =%d,slope_aux =%d")
TRACE_MSG(PROD_PARAM_722_112_2_18_0_26_17_13,"PROD_SetChargeNVParam need't update !")
TRACE_MSG(PROD_PARAM_726_112_2_18_0_26_17_14,"PROD_SetChargeNVParam update point = 0x%x")
TRACE_MSG(PROD_PARAM_800_112_2_18_0_26_17_15,"PROD_Init() Enter !")
TRACE_MSG(PROD_PARAM_911_112_2_18_0_26_18_16,"PROD_Init() Exit !")
TRACE_MSG(PROD_PARAM_963_112_2_18_0_26_18_17,"PROD_InitProdParam has been called !")
TRACE_MSG(PROD_PARAM_1032_112_2_18_0_26_18_18,"PROD_InitProdParam Exit !")
TRACE_MSG(PROD_PARAM_1122_112_2_18_0_26_18_19,"PROD_WriteAudioParam: mode_name = %s, is_flush = %d, start_time = %d")
TRACE_MSG(PROD_PARAM_1158_112_2_18_0_26_18_20,"PROD_WriteAudioParam: Write to Cache, end time = %d")
TRACE_MSG(PROD_PARAM_1196_112_2_18_0_26_18_21,"PROD_WriteAudioParam: Write to Nv,end time = %d")
TRACE_MSG(PROD_PARAM_1415_112_2_18_0_26_19_22,"PROD_ReadEngModeTestResult: nv_len %d, buf_len %d")
TRACE_MSG(PROD_PARAM_1442_112_2_18_0_26_19_23,"PROD_WriteEngModeTestResult: nv_len %d, buf_len %d")
TRACE_MSG(SENSOR_PROD_80_112_2_18_0_26_19_24,"[sensor_prod.c:_check_msg: sensor_id error,sensor_id = %d]")
TRACE_MSG(SENSOR_PROD_88_112_2_18_0_26_19_25,"[sensor_prod.c: sensor id error!]")
TRACE_MSG(SENSOR_PROD_95_112_2_18_0_26_19_26,"[sensor_prod.c:_check_msg:set sensor info table fail]")
TRACE_MSG(SENSOR_PROD_743_112_2_18_0_26_21_27,"[sensor_prod.c:SensorTool_Init ERROR!]")
TRACE_MSG(SENSOR_PROD_767_112_2_18_0_26_21_28,"sensor_prod.c:_SensorTool_CmdRoutine:")
TRACE_MSG(SENSOR_PROD_771_112_2_18_0_26_21_29,"[sensor_prod.c:_SensorTool_CmdRoutine:type error!,type = %d]")
TRACE_MSG(SENSOR_PROD_777_112_2_18_0_26_21_30,"[sensor_prod.c:_SensorTool_CmdRoutine:Init faile!]")
TRACE_MSG(SENSOR_PROD_813_112_2_18_0_26_21_31,"[sensor_prod.c:_SensorTool_CmdRoutine:TOOL_SENSOR_GET_SENSOR_SUM]")
TRACE_MSG(SENSOR_PROD_817_112_2_18_0_26_21_32,"[sensor_prod.c:_SensorTool_CmdRoutine subtype error,subtype=%d]")
TRACE_MSG(FM_CFG_94_112_2_18_0_26_25_33,"FM_GetCtrlPort %x")
TRACE_MSG(GPIO_CFG_192_112_2_18_0_26_25_34,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_265_112_2_18_0_26_25_35,"GPIO_CustomizeInit() End !")
TRACE_MSG(PROD_OUTPORT_70_112_2_18_0_26_31_36,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(PROD_OUTPORT_82_112_2_18_0_26_31_37,"Production test dummy function: Register_Current_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_145_112_2_18_0_26_32_38,"SCM: DIAG, slot_name = %d, msg = %d")
TRACE_MSG(SCM_CONFIG_194_112_2_18_0_26_32_39,"SCM: ATest Mode slot_name not support!")
TRACE_MSG(SCM_CONFIG_202_112_2_18_0_26_32_40,"SCM: ATest Mode MSG error!")
TRACE_MSG(SCM_CONFIG_219_112_2_18_0_26_32_41,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_220_112_2_18_0_26_32_42,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_221_112_2_18_0_26_32_43,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_222_112_2_18_0_26_32_44,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_223_112_2_18_0_26_32_45,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_224_112_2_18_0_26_32_46,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_225_112_2_18_0_26_32_47,"[Init]SCM_APB_CLK_CFG0 = 0x%x")
TRACE_MSG(SCM_CONFIG_226_112_2_18_0_26_32_48,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_254_112_2_18_0_26_32_49,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_259_112_2_18_0_26_32_50,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_296_112_2_18_0_26_32_51,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_322_112_2_18_0_26_32_52,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_330_112_2_18_0_26_32_53,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_353_112_2_18_0_26_32_54,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_361_112_2_18_0_26_32_55,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_633_112_2_18_0_26_33_56,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_659_112_2_18_0_26_33_57,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_670_112_2_18_0_26_33_58,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_671_112_2_18_0_26_33_59,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_672_112_2_18_0_26_33_60,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_673_112_2_18_0_26_33_61,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_674_112_2_18_0_26_33_62,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_675_112_2_18_0_26_33_63,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_676_112_2_18_0_26_33_64,"[ErrProc]SCM_APB_CLK_CFG0 = 0x%x")
TRACE_MSG(SCM_CONFIG_677_112_2_18_0_26_33_65,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_681_112_2_18_0_26_33_66,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_882_112_2_18_0_26_33_67,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_888_112_2_18_0_26_33_68,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_957_112_2_18_0_26_34_69,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_962_112_2_18_0_26_34_70,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_1182_112_2_18_0_26_34_71,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_1208_112_2_18_0_26_34_72,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_1221_112_2_18_0_26_34_73,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1408_112_2_18_0_26_35_74,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_1414_112_2_18_0_26_35_75,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1526_112_2_18_0_26_35_76,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1551_112_2_18_0_26_35_77,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1574_112_2_18_0_26_35_78,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1590_112_2_18_0_26_35_79,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1617_112_2_18_0_26_35_80,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1642_112_2_18_0_26_35_81,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1665_112_2_18_0_26_35_82,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1681_112_2_18_0_26_35_83,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1693_112_2_18_0_26_35_84,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_1699_112_2_18_0_26_35_85,"SCM slot0 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1706_112_2_18_0_26_35_86,"SCM slot0 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1733_112_2_18_0_26_35_87,"SCM slot1 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1740_112_2_18_0_26_35_88,"SCM slot1 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1765_112_2_18_0_26_35_89,"SCM slot name error!")
TRACE_MSG(SCM_CONFIG_1774_112_2_18_0_26_35_90,"SCM_BSD_UNREG")
TRACE_MSG(SCM_CONFIG_1794_112_2_18_0_26_35_91,"SCM slot name error!")
TRACE_MSG(SENSOR_CFG_144_112_2_18_0_26_36_92,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(TP_MULTI_CFG_82_112_2_18_0_26_38_93,"TPM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_94_112_2_18_0_26_41_94,"FM_GetCtrlPort %x")
TRACE_MSG(GPIO_CFG_189_112_2_18_0_26_42_95,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_262_112_2_18_0_26_42_96,"GPIO_CustomizeInit() End !")
TRACE_MSG(PROD_OUTPORT_70_112_2_18_0_26_49_97,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(PROD_OUTPORT_82_112_2_18_0_26_49_98,"Production test dummy function: Register_Current_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_145_112_2_18_0_26_49_99,"SCM: DIAG, slot_name = %d, msg = %d")
TRACE_MSG(SCM_CONFIG_194_112_2_18_0_26_49_100,"SCM: ATest Mode slot_name not support!")
TRACE_MSG(SCM_CONFIG_202_112_2_18_0_26_49_101,"SCM: ATest Mode MSG error!")
TRACE_MSG(SCM_CONFIG_219_112_2_18_0_26_49_102,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_220_112_2_18_0_26_49_103,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_221_112_2_18_0_26_49_104,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_222_112_2_18_0_26_49_105,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_223_112_2_18_0_26_49_106,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_224_112_2_18_0_26_49_107,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_225_112_2_18_0_26_49_108,"[Init]SCM_APB_CLK_CFG0 = 0x%x")
TRACE_MSG(SCM_CONFIG_226_112_2_18_0_26_49_109,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_254_112_2_18_0_26_49_110,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_259_112_2_18_0_26_49_111,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_296_112_2_18_0_26_50_112,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_322_112_2_18_0_26_50_113,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_330_112_2_18_0_26_50_114,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_353_112_2_18_0_26_50_115,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_361_112_2_18_0_26_50_116,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_633_112_2_18_0_26_50_117,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_659_112_2_18_0_26_50_118,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_670_112_2_18_0_26_50_119,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_671_112_2_18_0_26_50_120,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_672_112_2_18_0_26_50_121,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_673_112_2_18_0_26_50_122,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_674_112_2_18_0_26_50_123,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_675_112_2_18_0_26_50_124,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_676_112_2_18_0_26_50_125,"[ErrProc]SCM_APB_CLK_CFG0 = 0x%x")
TRACE_MSG(SCM_CONFIG_677_112_2_18_0_26_50_126,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_681_112_2_18_0_26_50_127,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_882_112_2_18_0_26_51_128,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_888_112_2_18_0_26_51_129,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_957_112_2_18_0_26_51_130,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_962_112_2_18_0_26_51_131,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_1182_112_2_18_0_26_52_132,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_1208_112_2_18_0_26_52_133,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_1221_112_2_18_0_26_52_134,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1408_112_2_18_0_26_52_135,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_1414_112_2_18_0_26_52_136,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1526_112_2_18_0_26_52_137,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1551_112_2_18_0_26_52_138,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1574_112_2_18_0_26_52_139,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1590_112_2_18_0_26_52_140,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1617_112_2_18_0_26_52_141,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1642_112_2_18_0_26_52_142,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1665_112_2_18_0_26_52_143,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1681_112_2_18_0_26_52_144,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1693_112_2_18_0_26_53_145,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_1699_112_2_18_0_26_53_146,"SCM slot0 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1706_112_2_18_0_26_53_147,"SCM slot0 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1733_112_2_18_0_26_53_148,"SCM slot1 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1740_112_2_18_0_26_53_149,"SCM slot1 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1765_112_2_18_0_26_53_150,"SCM slot name error!")
TRACE_MSG(SCM_CONFIG_1774_112_2_18_0_26_53_151,"SCM_BSD_UNREG")
TRACE_MSG(SCM_CONFIG_1794_112_2_18_0_26_53_152,"SCM slot name error!")
TRACE_MSG(SENSOR_CFG_144_112_2_18_0_26_53_153,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(FM_CFG_94_112_2_18_0_27_1_154,"FM_GetCtrlPort %x")
TRACE_MSG(GPIO_CFG_197_112_2_18_0_27_1_155,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_270_112_2_18_0_27_1_156,"GPIO_CustomizeInit() End !")
TRACE_MSG(PROD_OUTPORT_70_112_2_18_0_27_8_157,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(PROD_OUTPORT_82_112_2_18_0_27_8_158,"Production test dummy function: Register_Current_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_145_112_2_18_0_27_8_159,"SCM: DIAG, slot_name = %d, msg = %d")
TRACE_MSG(SCM_CONFIG_194_112_2_18_0_27_8_160,"SCM: ATest Mode slot_name not support!")
TRACE_MSG(SCM_CONFIG_202_112_2_18_0_27_8_161,"SCM: ATest Mode MSG error!")
TRACE_MSG(SCM_CONFIG_219_112_2_18_0_27_8_162,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_220_112_2_18_0_27_8_163,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_221_112_2_18_0_27_8_164,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_222_112_2_18_0_27_8_165,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_223_112_2_18_0_27_8_166,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_224_112_2_18_0_27_8_167,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_225_112_2_18_0_27_8_168,"[Init]SCM_APB_CLK_CFG0 = 0x%x")
TRACE_MSG(SCM_CONFIG_226_112_2_18_0_27_8_169,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_254_112_2_18_0_27_9_170,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_259_112_2_18_0_27_9_171,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_296_112_2_18_0_27_9_172,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_322_112_2_18_0_27_9_173,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_330_112_2_18_0_27_9_174,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_353_112_2_18_0_27_9_175,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_361_112_2_18_0_27_9_176,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_633_112_2_18_0_27_9_177,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_659_112_2_18_0_27_9_178,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_670_112_2_18_0_27_9_179,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_671_112_2_18_0_27_9_180,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_672_112_2_18_0_27_9_181,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_673_112_2_18_0_27_9_182,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_674_112_2_18_0_27_9_183,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_675_112_2_18_0_27_9_184,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_676_112_2_18_0_27_9_185,"[ErrProc]SCM_APB_CLK_CFG0 = 0x%x")
TRACE_MSG(SCM_CONFIG_677_112_2_18_0_27_9_186,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_681_112_2_18_0_27_9_187,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_882_112_2_18_0_27_10_188,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_888_112_2_18_0_27_10_189,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_957_112_2_18_0_27_10_190,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_962_112_2_18_0_27_10_191,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_1182_112_2_18_0_27_10_192,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_1208_112_2_18_0_27_11_193,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_1221_112_2_18_0_27_11_194,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1408_112_2_18_0_27_11_195,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_1414_112_2_18_0_27_11_196,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1526_112_2_18_0_27_11_197,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1551_112_2_18_0_27_11_198,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1574_112_2_18_0_27_11_199,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1590_112_2_18_0_27_11_200,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1617_112_2_18_0_27_12_201,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1642_112_2_18_0_27_12_202,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1665_112_2_18_0_27_12_203,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1681_112_2_18_0_27_12_204,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1693_112_2_18_0_27_12_205,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_1699_112_2_18_0_27_12_206,"SCM slot0 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1706_112_2_18_0_27_12_207,"SCM slot0 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1733_112_2_18_0_27_12_208,"SCM slot1 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1740_112_2_18_0_27_12_209,"SCM slot1 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1765_112_2_18_0_27_12_210,"SCM slot name error!")
TRACE_MSG(SCM_CONFIG_1774_112_2_18_0_27_12_211,"SCM_BSD_UNREG")
TRACE_MSG(SCM_CONFIG_1794_112_2_18_0_27_12_212,"SCM slot name error!")
TRACE_MSG(SENSOR_CFG_144_112_2_18_0_27_12_213,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(TP_MULTI_CFG_82_112_2_18_0_27_15_214,"TPM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_82_112_2_18_0_27_20_215,"FM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_106_112_2_18_0_27_20_216,"FM_ConfigClk open = %d, ret = 0x%X")
TRACE_MSG(GPIO_CFG_188_112_2_18_0_27_21_217,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_263_112_2_18_0_27_21_218,"GPIO_CustomizeInit() End !")
TRACE_MSG(GPIO_CFG_269_112_2_18_0_27_21_219,"GPIO_SetSensorPwdn: d,b_on is %d")
TRACE_MSG(GPIO_CFG_320_112_2_18_0_27_21_220,"GPIO_GetSwitchDefaultValue: gpio switch default value is wifi")
TRACE_MSG(GPIO_CFG_333_112_2_18_0_27_21_221,"GPIO_GetSwitchWiFiValue: gpio_switch_wifi_value is %d")
TRACE_MSG(GPIO_CFG_346_112_2_18_0_27_21_222,"GPIO_GetSwitchCMMBValue: gpio_switch_cmmb_value is %d")
TRACE_MSG(LCM_CFG_INFO_105_112_2_18_0_27_22_223,"enter Lcm_GetLcmInfo size = %d")
TRACE_MSG(MTV_CFG_122_112_2_18_0_27_25_224,"MTVCFG_Open_26MClock: %x")
TRACE_MSG(MTV_CFG_162_112_2_18_0_27_25_225,"MTV: (lcd id) index = %d, dev_id = 0x%x, dev_id_in_tab = 0x%x")
TRACE_MSG(PROD_OUTPORT_77_112_2_18_0_27_29_226,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_107_112_2_18_0_27_29_227,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_108_112_2_18_0_27_29_228,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_109_112_2_18_0_27_29_229,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_110_112_2_18_0_27_29_230,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_111_112_2_18_0_27_29_231,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_112_112_2_18_0_27_29_232,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_113_112_2_18_0_27_29_233,"[Init]SCM_GR_CLK_GEN5 = 0x%x")
TRACE_MSG(SCM_CONFIG_114_112_2_18_0_27_29_234,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_384_112_2_18_0_27_30_235,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_410_112_2_18_0_27_30_236,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_421_112_2_18_0_27_30_237,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_422_112_2_18_0_27_30_238,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_423_112_2_18_0_27_30_239,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_424_112_2_18_0_27_30_240,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_425_112_2_18_0_27_30_241,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_426_112_2_18_0_27_30_242,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_427_112_2_18_0_27_30_243,"[ErrProc]SCM_GR_CLK_GEN5 = 0x%x")
TRACE_MSG(SCM_CONFIG_428_112_2_18_0_27_30_244,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_432_112_2_18_0_27_30_245,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_641_112_2_18_0_27_30_246,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_716_112_2_18_0_27_30_247,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_741_112_2_18_0_27_30_248,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_764_112_2_18_0_27_30_249,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_780_112_2_18_0_27_30_250,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_792_112_2_18_0_27_30_251,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_828_112_2_18_0_27_30_252,"SCM_BSD_UNREG")
TRACE_MSG(SDUA_CONFIG_81_112_2_18_0_27_31_253,"sd is mbbms ca card")
TRACE_MSG(SDUA_CONFIG_110_112_2_18_0_27_31_254,"[DRV_SDUA]<ERROR> SDUACFG_IOCTL is wrong\r\n")
TRACE_MSG(SENSOR_CFG_125_112_2_18_0_27_31_255,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(FM_CFG_82_112_2_18_0_27_34_256,"FM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_106_112_2_18_0_27_34_257,"FM_ConfigClk open = %d, ret = 0x%X")
TRACE_MSG(GPIO_CFG_178_112_2_18_0_27_35_258,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_251_112_2_18_0_27_35_259,"GPIO_CustomizeInit() End !")
TRACE_MSG(PROD_OUTPORT_70_112_2_18_0_27_41_260,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(PROD_OUTPORT_82_112_2_18_0_27_41_261,"Production test dummy function: Register_Current_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_145_112_2_18_0_27_42_262,"SCM: DIAG, slot_name = %d, msg = %d")
TRACE_MSG(SCM_CONFIG_194_112_2_18_0_27_42_263,"SCM: ATest Mode slot_name not support!")
TRACE_MSG(SCM_CONFIG_202_112_2_18_0_27_42_264,"SCM: ATest Mode MSG error!")
TRACE_MSG(SCM_CONFIG_219_112_2_18_0_27_42_265,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_220_112_2_18_0_27_42_266,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_221_112_2_18_0_27_42_267,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_222_112_2_18_0_27_42_268,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_223_112_2_18_0_27_42_269,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_224_112_2_18_0_27_42_270,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_225_112_2_18_0_27_42_271,"[Init]SCM_AHB_CLK_CFG1 = 0x%x")
TRACE_MSG(SCM_CONFIG_226_112_2_18_0_27_42_272,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_254_112_2_18_0_27_42_273,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_259_112_2_18_0_27_42_274,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_296_112_2_18_0_27_42_275,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_322_112_2_18_0_27_42_276,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_330_112_2_18_0_27_42_277,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_353_112_2_18_0_27_42_278,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_361_112_2_18_0_27_42_279,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_589_112_2_18_0_27_43_280,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_615_112_2_18_0_27_43_281,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_626_112_2_18_0_27_43_282,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_627_112_2_18_0_27_43_283,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_628_112_2_18_0_27_43_284,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_629_112_2_18_0_27_43_285,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_630_112_2_18_0_27_43_286,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_631_112_2_18_0_27_43_287,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_632_112_2_18_0_27_43_288,"[ErrProc]SCM_AHB_CLK_CFG1 = 0x%x")
TRACE_MSG(SCM_CONFIG_633_112_2_18_0_27_43_289,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_637_112_2_18_0_27_43_290,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_824_112_2_18_0_27_43_291,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_830_112_2_18_0_27_43_292,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_899_112_2_18_0_27_44_293,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_904_112_2_18_0_27_44_294,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_1124_112_2_18_0_27_44_295,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_1150_112_2_18_0_27_44_296,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_1163_112_2_18_0_27_44_297,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1350_112_2_18_0_27_45_298,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_1356_112_2_18_0_27_45_299,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1468_112_2_18_0_27_45_300,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1493_112_2_18_0_27_45_301,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1516_112_2_18_0_27_45_302,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1532_112_2_18_0_27_45_303,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1559_112_2_18_0_27_45_304,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1584_112_2_18_0_27_45_305,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1607_112_2_18_0_27_45_306,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1623_112_2_18_0_27_45_307,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1635_112_2_18_0_27_45_308,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_1641_112_2_18_0_27_45_309,"SCM slot0 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1648_112_2_18_0_27_45_310,"SCM slot0 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1675_112_2_18_0_27_45_311,"SCM slot1 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1682_112_2_18_0_27_45_312,"SCM slot1 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1707_112_2_18_0_27_45_313,"SCM slot name error!")
TRACE_MSG(SCM_CONFIG_1716_112_2_18_0_27_45_314,"SCM_BSD_UNREG")
TRACE_MSG(SCM_CONFIG_1736_112_2_18_0_27_46_315,"SCM slot name error!")
TRACE_MSG(SENSOR_CFG_136_112_2_18_0_27_46_316,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(FM_CFG_93_112_2_18_0_27_51_317,"FM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_117_112_2_18_0_27_51_318,"FM_ConfigClk open = %d, ret = 0x%X")
TRACE_MSG(GPIO_CFG_200_112_2_18_0_27_51_319,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_316_112_2_18_0_27_52_320,"GPIO_CustomizeInit() End !")
TRACE_MSG(PROD_OUTPORT_70_112_2_18_0_27_57_321,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(PROD_OUTPORT_82_112_2_18_0_27_57_322,"Production test dummy function: Register_Current_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_148_112_2_18_0_27_58_323,"SCM: DIAG, slot_name = %d, msg = %d")
TRACE_MSG(SCM_CONFIG_197_112_2_18_0_27_58_324,"SCM: ATest Mode slot_name not support!")
TRACE_MSG(SCM_CONFIG_205_112_2_18_0_27_58_325,"SCM: ATest Mode MSG error!")
TRACE_MSG(SCM_CONFIG_222_112_2_18_0_27_58_326,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_223_112_2_18_0_27_58_327,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_224_112_2_18_0_27_58_328,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_225_112_2_18_0_27_58_329,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_226_112_2_18_0_27_58_330,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_227_112_2_18_0_27_58_331,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_228_112_2_18_0_27_58_332,"[Init]SCM_AHB_CLK_CFG1 = 0x%x")
TRACE_MSG(SCM_CONFIG_229_112_2_18_0_27_58_333,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_259_112_2_18_0_27_58_334,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_264_112_2_18_0_27_58_335,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_303_112_2_18_0_27_58_336,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_329_112_2_18_0_27_58_337,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_337_112_2_18_0_27_58_338,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_360_112_2_18_0_27_58_339,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_368_112_2_18_0_27_58_340,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_593_112_2_18_0_27_59_341,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_619_112_2_18_0_27_59_342,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_630_112_2_18_0_27_59_343,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_631_112_2_18_0_27_59_344,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_632_112_2_18_0_27_59_345,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_633_112_2_18_0_27_59_346,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_634_112_2_18_0_27_59_347,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_635_112_2_18_0_27_59_348,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_636_112_2_18_0_27_59_349,"[ErrProc]SCM_AHB_CLK_CFG1 = 0x%x")
TRACE_MSG(SCM_CONFIG_637_112_2_18_0_27_59_350,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_641_112_2_18_0_27_59_351,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_828_112_2_18_0_27_59_352,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_834_112_2_18_0_27_59_353,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_903_112_2_18_0_27_59_354,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_908_112_2_18_0_27_59_355,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_1128_112_2_18_0_28_0_356,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_1154_112_2_18_0_28_0_357,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_1167_112_2_18_0_28_0_358,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1354_112_2_18_0_28_0_359,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_1360_112_2_18_0_28_0_360,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1472_112_2_18_0_28_0_361,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1497_112_2_18_0_28_0_362,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1520_112_2_18_0_28_1_363,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1536_112_2_18_0_28_1_364,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1563_112_2_18_0_28_1_365,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1588_112_2_18_0_28_1_366,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1611_112_2_18_0_28_1_367,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1627_112_2_18_0_28_1_368,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1639_112_2_18_0_28_1_369,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_1645_112_2_18_0_28_1_370,"SCM slot0 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1652_112_2_18_0_28_1_371,"SCM slot0 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1679_112_2_18_0_28_1_372,"SCM slot1 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1686_112_2_18_0_28_1_373,"SCM slot1 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1711_112_2_18_0_28_1_374,"SCM slot name error!")
TRACE_MSG(SCM_CONFIG_1720_112_2_18_0_28_1_375,"SCM_BSD_UNREG")
TRACE_MSG(SCM_CONFIG_1740_112_2_18_0_28_1_376,"SCM slot name error!")
TRACE_MSG(SENSOR_CFG_117_112_2_18_0_28_1_377,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(TP_MULTI_CFG_82_112_2_18_0_28_3_378,"TPM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_84_112_2_18_0_28_6_379,"FM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_108_112_2_18_0_28_6_380,"FM_ConfigClk open = %d, ret = 0x%X")
TRACE_MSG(GPIO_CFG_178_112_2_18_0_28_6_381,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_251_112_2_18_0_28_7_382,"GPIO_CustomizeInit() End !")
TRACE_MSG(PROD_OUTPORT_70_112_2_18_0_28_12_383,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(PROD_OUTPORT_82_112_2_18_0_28_12_384,"Production test dummy function: Register_Current_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_145_112_2_18_0_28_13_385,"SCM: DIAG, slot_name = %d, msg = %d")
TRACE_MSG(SCM_CONFIG_194_112_2_18_0_28_13_386,"SCM: ATest Mode slot_name not support!")
TRACE_MSG(SCM_CONFIG_202_112_2_18_0_28_13_387,"SCM: ATest Mode MSG error!")
TRACE_MSG(SCM_CONFIG_219_112_2_18_0_28_13_388,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_220_112_2_18_0_28_13_389,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_221_112_2_18_0_28_13_390,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_222_112_2_18_0_28_13_391,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_223_112_2_18_0_28_13_392,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_224_112_2_18_0_28_13_393,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_225_112_2_18_0_28_13_394,"[Init]SCM_AHB_CLK_CFG1 = 0x%x")
TRACE_MSG(SCM_CONFIG_226_112_2_18_0_28_13_395,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_254_112_2_18_0_28_13_396,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_259_112_2_18_0_28_13_397,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_296_112_2_18_0_28_13_398,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_322_112_2_18_0_28_13_399,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_330_112_2_18_0_28_13_400,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_353_112_2_18_0_28_13_401,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_361_112_2_18_0_28_13_402,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_589_112_2_18_0_28_13_403,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_615_112_2_18_0_28_14_404,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_626_112_2_18_0_28_14_405,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_627_112_2_18_0_28_14_406,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_628_112_2_18_0_28_14_407,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_629_112_2_18_0_28_14_408,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_630_112_2_18_0_28_14_409,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_631_112_2_18_0_28_14_410,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_632_112_2_18_0_28_14_411,"[ErrProc]SCM_AHB_CLK_CFG1 = 0x%x")
TRACE_MSG(SCM_CONFIG_633_112_2_18_0_28_14_412,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_637_112_2_18_0_28_14_413,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_824_112_2_18_0_28_14_414,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_830_112_2_18_0_28_14_415,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_899_112_2_18_0_28_14_416,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_904_112_2_18_0_28_14_417,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_1124_112_2_18_0_28_15_418,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_1150_112_2_18_0_28_15_419,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_1163_112_2_18_0_28_15_420,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1350_112_2_18_0_28_15_421,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_1356_112_2_18_0_28_15_422,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1468_112_2_18_0_28_15_423,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1493_112_2_18_0_28_15_424,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1516_112_2_18_0_28_15_425,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1532_112_2_18_0_28_15_426,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1559_112_2_18_0_28_15_427,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1584_112_2_18_0_28_15_428,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1607_112_2_18_0_28_16_429,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1623_112_2_18_0_28_16_430,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1635_112_2_18_0_28_16_431,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_1641_112_2_18_0_28_16_432,"SCM slot0 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1648_112_2_18_0_28_16_433,"SCM slot0 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1675_112_2_18_0_28_16_434,"SCM slot1 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1682_112_2_18_0_28_16_435,"SCM slot1 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1707_112_2_18_0_28_16_436,"SCM slot name error!")
TRACE_MSG(SCM_CONFIG_1716_112_2_18_0_28_16_437,"SCM_BSD_UNREG")
TRACE_MSG(SCM_CONFIG_1736_112_2_18_0_28_16_438,"SCM slot name error!")
TRACE_MSG(SENSOR_CFG_129_112_2_18_0_28_16_439,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(FM_CFG_95_112_2_18_0_28_20_440,"FM_GetCtrlPort %x")
TRACE_MSG(GPIO_CFG_270_112_2_18_0_28_21_441,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_352_112_2_18_0_28_21_442,"GPIO_CustomizeInit() End !")
TRACE_MSG(PROD_OUTPORT_70_112_2_18_0_28_27_443,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(PROD_OUTPORT_82_112_2_18_0_28_27_444,"Production test dummy function: Register_Current_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_148_112_2_18_0_28_27_445,"SCM: DIAG, slot_name = %d, msg = %d")
TRACE_MSG(SCM_CONFIG_197_112_2_18_0_28_27_446,"SCM: ATest Mode slot_name not support!")
TRACE_MSG(SCM_CONFIG_205_112_2_18_0_28_27_447,"SCM: ATest Mode MSG error!")
TRACE_MSG(SCM_CONFIG_224_112_2_18_0_28_28_448,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_225_112_2_18_0_28_28_449,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_226_112_2_18_0_28_28_450,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_227_112_2_18_0_28_28_451,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_228_112_2_18_0_28_28_452,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_229_112_2_18_0_28_28_453,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_230_112_2_18_0_28_28_454,"[Init]SCM_AHB_CLK_CFG1 = 0x%x")
TRACE_MSG(SCM_CONFIG_231_112_2_18_0_28_28_455,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_259_112_2_18_0_28_28_456,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_264_112_2_18_0_28_28_457,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_302_112_2_18_0_28_28_458,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_328_112_2_18_0_28_28_459,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_336_112_2_18_0_28_28_460,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_359_112_2_18_0_28_28_461,"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n")
TRACE_MSG(SCM_CONFIG_367_112_2_18_0_28_28_462,"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n")
TRACE_MSG(SCM_CONFIG_592_112_2_18_0_28_28_463,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_618_112_2_18_0_28_28_464,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_629_112_2_18_0_28_28_465,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_630_112_2_18_0_28_28_466,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_631_112_2_18_0_28_28_467,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_632_112_2_18_0_28_28_468,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_633_112_2_18_0_28_28_469,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_634_112_2_18_0_28_28_470,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_635_112_2_18_0_28_28_471,"[ErrProc]SCM_AHB_CLK_CFG1 = 0x%x")
TRACE_MSG(SCM_CONFIG_636_112_2_18_0_28_28_472,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_640_112_2_18_0_28_28_473,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_821_112_2_18_0_28_29_474,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_827_112_2_18_0_28_29_475,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_896_112_2_18_0_28_29_476,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_901_112_2_18_0_28_29_477,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_1121_112_2_18_0_28_29_478,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_1147_112_2_18_0_28_29_479,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_1160_112_2_18_0_28_29_480,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1347_112_2_18_0_28_30_481,"[DRV_SCM]<ERROR> val:cmd is wrong\r\n")
TRACE_MSG(SCM_CONFIG_1353_112_2_18_0_28_30_482,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_1465_112_2_18_0_28_30_483,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1490_112_2_18_0_28_30_484,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1513_112_2_18_0_28_30_485,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1529_112_2_18_0_28_30_486,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1556_112_2_18_0_28_30_487,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_1581_112_2_18_0_28_30_488,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_1604_112_2_18_0_28_30_489,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_1620_112_2_18_0_28_30_490,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_1632_112_2_18_0_28_30_491,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_1638_112_2_18_0_28_30_492,"SCM slot0 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1645_112_2_18_0_28_30_493,"SCM slot0 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1672_112_2_18_0_28_30_494,"SCM slot1 status not normal! Error!")
TRACE_MSG(SCM_CONFIG_1679_112_2_18_0_28_30_495,"SCM slot1 capacity is 0! Error!")
TRACE_MSG(SCM_CONFIG_1704_112_2_18_0_28_30_496,"SCM slot name error!")
TRACE_MSG(SCM_CONFIG_1713_112_2_18_0_28_30_497,"SCM_BSD_UNREG")
TRACE_MSG(SCM_CONFIG_1733_112_2_18_0_28_31_498,"SCM slot name error!")
TRACE_MSG(SENSOR_CFG_123_112_2_18_0_28_31_499,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(TP_MULTI_CFG_82_112_2_18_0_28_32_500,"TPM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_82_112_2_18_0_28_34_501,"FM_GetCtrlPort %x")
TRACE_MSG(FM_CFG_106_112_2_18_0_28_34_502,"FM_ConfigClk open = %d, ret = 0x%X")
TRACE_MSG(GPIO_CFG_165_112_2_18_0_28_35_503,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_230_112_2_18_0_28_35_504,"GPIO_CustomizeInit() End !")
TRACE_MSG(GPIO_CFG_236_112_2_18_0_28_35_505,"GPIO_SetSensorPwdn: d,b_on is %d")
TRACE_MSG(MTV_CFG_122_112_2_18_0_28_38_506,"MTVCFG_Open_26MClock: %x")
TRACE_MSG(MTV_CFG_162_112_2_18_0_28_39_507,"MTV: (lcd id) index = %d, dev_id = 0x%x, dev_id_in_tab = 0x%x")
TRACE_MSG(PROD_OUTPORT_77_112_2_18_0_28_43_508,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_144_112_2_18_0_28_43_509,"SCM: DIAG, slot_name = %d, msg = %d")
TRACE_MSG(SCM_CONFIG_193_112_2_18_0_28_43_510,"SCM: ATest Mode slot_name not support!")
TRACE_MSG(SCM_CONFIG_201_112_2_18_0_28_43_511,"SCM: ATest Mode MSG error!")
TRACE_MSG(SCM_CONFIG_219_112_2_18_0_28_43_512,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_220_112_2_18_0_28_43_513,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_221_112_2_18_0_28_43_514,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_222_112_2_18_0_28_43_515,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_223_112_2_18_0_28_43_516,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_224_112_2_18_0_28_43_517,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_225_112_2_18_0_28_43_518,"[Init]SCM_GR_CLK_GEN5 = 0x%x")
TRACE_MSG(SCM_CONFIG_226_112_2_18_0_28_43_519,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_265_112_2_18_0_28_44_520,"ATest Mode PO -- Plug out")
TRACE_MSG(SCM_CONFIG_270_112_2_18_0_28_44_521,"ATest Mode PI -- Plug in")
TRACE_MSG(SCM_CONFIG_524_112_2_18_0_28_44_522,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_550_112_2_18_0_28_44_523,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_561_112_2_18_0_28_44_524,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_562_112_2_18_0_28_44_525,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_563_112_2_18_0_28_44_526,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_564_112_2_18_0_28_44_527,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_565_112_2_18_0_28_44_528,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_566_112_2_18_0_28_44_529,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_567_112_2_18_0_28_44_530,"[ErrProc]SCM_GR_CLK_GEN5 = 0x%x")
TRACE_MSG(SCM_CONFIG_568_112_2_18_0_28_44_531,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_572_112_2_18_0_28_44_532,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_781_112_2_18_0_28_45_533,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_856_112_2_18_0_28_45_534,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_881_112_2_18_0_28_45_535,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_904_112_2_18_0_28_45_536,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_920_112_2_18_0_28_45_537,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_932_112_2_18_0_28_45_538,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_968_112_2_18_0_28_45_539,"SCM_BSD_UNREG")
TRACE_MSG(SDUA_CONFIG_81_112_2_18_0_28_45_540,"sd is mbbms ca card")
TRACE_MSG(SDUA_CONFIG_110_112_2_18_0_28_45_541,"[DRV_SDUA]<ERROR> SDUACFG_IOCTL is wrong\r\n")
TRACE_MSG(SENSOR_CFG_114_112_2_18_0_28_46_542,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(GPIO_CFG_161_112_2_18_0_28_49_543,"GPIO_CustomizeInit() Start !")
TRACE_MSG(GPIO_CFG_224_112_2_18_0_28_49_544,"GPIO_CustomizeInit() End !")
TRACE_MSG(GPIO_CFG_230_112_2_18_0_28_49_545,"GPIO_SetSensorPwdn: d,b_on is %d")
TRACE_MSG(MTV_CFG_122_112_2_18_0_28_52_546,"MTVCFG_Open_26MClock: %x")
TRACE_MSG(MTV_CFG_162_112_2_18_0_28_52_547,"MTV: (lcd id) index = %d, dev_id = 0x%x, dev_id_in_tab = 0x%x")
TRACE_MSG(PROD_OUTPORT_77_112_2_18_0_28_56_548,"Production test dummy function: Register_MMI_Test_CmdRoutine")
TRACE_MSG(SCM_CONFIG_107_112_2_18_0_28_57_549,"[Init]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_108_112_2_18_0_28_57_550,"[Init]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_109_112_2_18_0_28_57_551,"[Init]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_110_112_2_18_0_28_57_552,"[Init]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_111_112_2_18_0_28_57_553,"[Init]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_112_112_2_18_0_28_57_554,"[Init]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_113_112_2_18_0_28_57_555,"[Init]SCM_GR_CLK_GEN5 = 0x%x")
TRACE_MSG(SCM_CONFIG_114_112_2_18_0_28_57_556,"[Init]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_376_112_2_18_0_28_57_557,"SCM ErrProc too many in HighSpeed mode! Use normal mode!")
TRACE_MSG(SCM_CONFIG_402_112_2_18_0_28_57_558,"SCM ErrProc too many in normal mode! FALSE!")
TRACE_MSG(SCM_CONFIG_413_112_2_18_0_28_57_559,"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x")
TRACE_MSG(SCM_CONFIG_414_112_2_18_0_28_57_560,"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x")
TRACE_MSG(SCM_CONFIG_415_112_2_18_0_28_57_561,"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x")
TRACE_MSG(SCM_CONFIG_416_112_2_18_0_28_57_562,"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x")
TRACE_MSG(SCM_CONFIG_417_112_2_18_0_28_57_563,"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x")
TRACE_MSG(SCM_CONFIG_418_112_2_18_0_28_57_564,"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x")
TRACE_MSG(SCM_CONFIG_419_112_2_18_0_28_57_565,"[ErrProc]SCM_GR_CLK_GEN5 = 0x%x")
TRACE_MSG(SCM_CONFIG_420_112_2_18_0_28_57_566,"[ErrProc]SCM_GR_MPLL_MN = 0x%x")
TRACE_MSG(SCM_CONFIG_424_112_2_18_0_28_57_567,"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_633_112_2_18_0_28_58_568,"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x")
TRACE_MSG(SCM_CONFIG_708_112_2_18_0_28_58_569,"SCM Read fail")
TRACE_MSG(SCM_CONFIG_733_112_2_18_0_28_58_570,"SCM Write fail")
TRACE_MSG(SCM_CONFIG_756_112_2_18_0_28_58_571,"SCM Erase fail")
TRACE_MSG(SCM_CONFIG_772_112_2_18_0_28_58_572,"SCM GetCap fail")
TRACE_MSG(SCM_CONFIG_784_112_2_18_0_28_58_573,"SCM_BSD_REG")
TRACE_MSG(SCM_CONFIG_820_112_2_18_0_28_58_574,"SCM_BSD_UNREG")
TRACE_MSG(SENSOR_CFG_112_112_2_18_0_28_59_575,"Sensor_SelectSensorFormat: image_format %d, s_sensor_image_format = %d")
TRACE_MSG(AF_CFG_92_112_2_18_0_29_0_576,"AF Is beding:%d")
TRACE_MSG(AF_CFG_209_112_2_18_0_29_0_577,"Set Preview auto focus state")
TRACE_MSG(AF_NFMW_MICRON_3M_216_112_2_18_0_29_2_578,"AF: index  = %d,  value = %d")
TRACE_MSG(AF_NFMW_MICRON_3M_257_112_2_18_0_29_2_579,"AF: index_1  = %d MaxFoucus Index ' %d")
TRACE_MSG(AF_NFMW_MICRON_3M_261_112_2_18_0_29_2_580,"AF: index  = %d")
TRACE_MSG(AF_NFMW_MICRON_3M_336_112_2_18_0_29_2_581,"AF SDY: check--  stable: MaxFocusIndex: %d ; FineFocusindex = %d")
TRACE_MSG(AF_NFMW_MICRON_3M_376_112_2_18_0_29_2_582,"AF SDY: check--  stable: MaxFocusIndex: %d ; FineMaxFocusindex = %d")
TRACE_MSG(AF_NFMW_MICRON_3M_389_112_2_18_0_29_2_583,"Focusstate = %d")
TRACE_MSG(BT_PROD_210_112_2_18_0_29_4_584,"[BT]BTI_RestoreSystemClk:type = 0x%X\r\n")
TRACE_MSG(BT_PROD_220_112_2_18_0_29_4_585,"[BT]BTI_RestoreSystemClk:Idle restore successful\r\n")
TRACE_MSG(BT_PROD_224_112_2_18_0_29_4_586,"[BT]BTI_RestoreSystemClk:Idle restore error\r\n")
TRACE_MSG(BT_PROD_235_112_2_18_0_29_4_587,"[BT]BTI_RestoreSystemClk:FTP restore successful\r\n")
TRACE_MSG(BT_PROD_239_112_2_18_0_29_4_588,"[BT]BTI_RestoreSystemClk:FTP restore error\r\n")
TRACE_MSG(BT_PROD_250_112_2_18_0_29_4_589,"[BT]BTI_RestoreSystemClk:HFG restore successful\r\n")
TRACE_MSG(BT_PROD_254_112_2_18_0_29_4_590,"[BT]BTI_RestoreSystemClk:HFG restore error\r\n")
TRACE_MSG(BT_PROD_265_112_2_18_0_29_4_591,"[BT]BTI_RestoreSystemClk:A2DP restore successful\r\n")
TRACE_MSG(BT_PROD_269_112_2_18_0_29_4_592,"[BT]BTI_RestoreSystemClk:A2DP restore error\r\n")
TRACE_MSG(BT_PROD_275_112_2_18_0_29_4_593,"[BT]<ERROR> BTI_RestoreSystemClk: NOT supported type\r\n")
TRACE_MSG(BT_PROD_286_112_2_18_0_29_4_594,"[BT]BTI_ChangeSystemClk:type = 0x%X\r\n")
TRACE_MSG(BT_PROD_295_112_2_18_0_29_4_595,"[BT]BTI_ChangeSystemClk:IDLE's clolck change successful\r\n")
TRACE_MSG(BT_PROD_299_112_2_18_0_29_4_596,"[BT]BTI_ChangeSystemClk:IDLE changed clolck error\r\n")
TRACE_MSG(BT_PROD_308_112_2_18_0_29_4_597,"[BT]BTI_ChangeSystemClk:FTP's clolck change successful\r\n")
TRACE_MSG(BT_PROD_312_112_2_18_0_29_4_598,"[BT]BTI_ChangeSystemClk:FTP changed clolck error\r\n")
TRACE_MSG(BT_PROD_321_112_2_18_0_29_4_599,"[BT]BTI_ChangeSystemClk:HFG's clolck change successful\r\n")
TRACE_MSG(BT_PROD_325_112_2_18_0_29_4_600,"[BT]BTI_ChangeSystemClk:HFG changed clolck error\r\n")
TRACE_MSG(BT_PROD_334_112_2_18_0_29_4_601,"[BT]BTI_ChangeSystemClk:A2DP's clolck change successful\r\n")
TRACE_MSG(BT_PROD_338_112_2_18_0_29_4_602,"[BT]BTI_ChangeSystemClk:A2DP changed clolck error\r\n")
TRACE_MSG(BT_PROD_343_112_2_18_0_29_4_603,"[BT]<ERROR> BTI_ChangeSystemClk: NOT supported type\r\n")
TRACE_MSG(BT_RDA_32_112_2_18_0_29_5_604,"rda_InitGpioI2C")
TRACE_MSG(BT_RDA_50_112_2_18_0_29_5_605,"rda_DeinitGpioI2C")
TRACE_MSG(BT_RDA_201_112_2_18_0_29_5_606,"\"# bt i2c write failed ")
TRACE_MSG(BT_RDA_446_112_2_18_0_29_6_607,"The chip id of RDA5868+ is 0x%08x")
TRACE_MSG(BT_RDA_453_112_2_18_0_29_6_608," BTI_StartupChip\r\n")
TRACE_MSG(FM_NMI601_133_112_2_18_0_29_7_609,"[FM_DRV]:_nmi600_FM_PowerOn %d")
TRACE_MSG(FM_NMI601_164_112_2_18_0_29_7_610,"[FM_DRV]NMI600_Initial()")
TRACE_MSG(FM_NMI601_181_112_2_18_0_29_8_611,"[FM_DRV]NMI600_PlayStart() ,%d")
TRACE_MSG(FM_NMI601_198_112_2_18_0_29_8_612,"[FM_DRV]NMI600_PlayStop()")
TRACE_MSG(FM_NMI601_218_112_2_18_0_29_8_613,"[FM_DRV]NMI600_Open ")
TRACE_MSG(FM_NMI601_229_112_2_18_0_29_8_614,"[FM_DRV]NMI600_Close ")
TRACE_MSG(FM_NMI601_248_112_2_18_0_29_8_615,"[FM_DRV]NMI600_Pause ")
TRACE_MSG(FM_NMI601_261_112_2_18_0_29_8_616,"[FM_DRV]NMI600_Resume ")
TRACE_MSG(FM_NMI601_287_112_2_18_0_29_8_617,"[FM_DRV]NMI600_ManualSeek , rssi %d")
TRACE_MSG(FM_NMI601_370_112_2_18_0_29_8_618,"[FM_DRV]NMI600_AutoSeek: ret = %d,  state = %d, freq = %d")
TRACE_MSG(FM_NMI601_387_112_2_18_0_29_8_619,"NMI600_SetVol  ,%d")
TRACE_MSG(FM_NMI601_404_112_2_18_0_29_8_620,"NMI600_SetFMRegion ")
TRACE_MSG(FM_NMI601_420_112_2_18_0_29_8_621,"NMI600_SetScanLevel ")
TRACE_MSG(NMIDRV_ATV_39_112_2_18_0_29_34_622,"nmidrv_atv: nmi atv power on failed!!!")
TRACE_MSG(NMIDRV_ATV_539_112_2_18_0_29_35_623,"atv_nmi_init_scan_param region = %d,  scan_start_chn = %d,  scan_end_chn= %d \r\n")
TRACE_MSG(NMIDRV_ATV_547_112_2_18_0_29_35_624,"atv_nmi_scan_chn scan_chn_index = %d \r\n")
TRACE_MSG(NMIPORT_56_112_2_18_0_29_36_625,"%s")
TRACE_MSG(NMIPORT_300_112_2_18_0_29_37_626,"Failed, bus write...\n")
TRACE_MSG(NMIPORT_306_112_2_18_0_29_37_627,"Failed, bus read...\n")
TRACE_MSG(NMIPORT_310_112_2_18_0_29_37_628,"nmi600 adr(0x%08x),val(0x%08x)\n\r")
TRACE_MSG(NMIPORT_347_112_2_18_0_29_37_629,"NMIAPP_InitVideoSize fullscreen:%d")
TRACE_MSG(NMIPORT_366_112_2_18_0_29_37_630,"%s")
TRACE_MSG(ATV_NMI601_253_112_2_18_0_29_38_631,"SENSOR: _nmi600_InitChannel")
TRACE_MSG(ATV_NMI601_358_112_2_18_0_29_38_632,"GPIO_SetAnalogTVPowerOn,power = %d")
TRACE_MSG(ATV_NMI601_369_112_2_18_0_29_38_633,"ATV: _nmi600_Power_On(1:on, 0:off): %d")
TRACE_MSG(ATV_NMI601_392_112_2_18_0_29_38_634,"ATV: _nmi600_Power_On(1:on, 0:off) end : %d")
TRACE_MSG(ATV_NMI601_405_112_2_18_0_29_38_635,"ATV: _nmi600_Identify")
TRACE_MSG(ATV_NMI601_409_112_2_18_0_29_38_636,"ATV: the atv is not nmi600")
TRACE_MSG(ATV_NMI601_414_112_2_18_0_29_38_637,"ATV: the atv is nmi600")
TRACE_MSG(ATV_NMI601_429_112_2_18_0_29_38_638,"ATV:_nmi600_Init")
TRACE_MSG(ATV_NMI601_445_112_2_18_0_29_38_639,"ATV:_nmi600_Sleep")
TRACE_MSG(ATV_NMI601_482_112_2_18_0_29_38_640,"ATV:_nmi600_ScanChn")
TRACE_MSG(ATV_NMI601_506_112_2_18_0_29_38_641,"ATV:_nmi600_SetChn : 0x%x")
TRACE_MSG(ATV_NMI601_535_112_2_18_0_29_38_642,"ATV:_nmi600_GetAllChnNum")
TRACE_MSG(ATV_NMI601_551_112_2_18_0_29_38_643,"ATV:_nmi600_SetVolume : 0x0%x")
TRACE_MSG(ATV_NMI601_593_112_2_18_0_29_38_644,"ATV:_nmi600_SetRegion")
TRACE_MSG(ATV_NMI601_694_112_2_18_0_29_39_645,"ATV:_nmi600_GetRssi")
TRACE_MSG(ATV_NMI601_746_112_2_18_0_29_39_646,"ATV:_nmi600_IsNTSCMode")
TRACE_MSG(ATV_NMI601_794_112_2_18_0_29_39_647,"ATV:_nmi600_Close")
TRACE_MSG(ATV_NMI601_812_112_2_18_0_29_39_648,"ATV: nmi600_WriteReg reg/value(0x%x,0x%x) !!")
TRACE_MSG(ATV_NMI601_825_112_2_18_0_29_39_649,"ATV: nmi600_ReadReg reg/value(0x%x,0x%x) !!")
TRACE_MSG(QMADRIVER_465_112_2_18_0_29_44_650,"QMA:Chip ID = %x\n")
TRACE_MSG(QMADRIVER_585_112_2_18_0_29_44_651,"ATV:QN:QMA_SetRegion param[%d]")
TRACE_MSG(QMADRIVER_1322_112_2_18_0_29_45_652,"ATV:atv_qma_scan_chn got id[%d]")
TRACE_MSG(QMADRIVER_1331_112_2_18_0_29_45_653,"ATV: FREQ=%d,qma->chCount=%d,rgn=%d,freq=%d,rgnChTatol=%d,chlist=%d")
TRACE_MSG(QN8610I2C_20_112_2_18_0_29_51_654,"ATV:QN:QMA_WriteReg check :Fail Write[%d] Read[%d]")
TRACE_MSG(ATV_QN8610_260_112_2_18_0_29_52_655,"ATV:QN:_qn8610_PowerOnSequence power_on[%d]")
TRACE_MSG(ATV_QN8610_293_112_2_18_0_29_52_656,"ATV:QN:_qn8610_Identify ")
TRACE_MSG(ATV_QN8610_301_112_2_18_0_29_52_657,"ATV: the atv is QN6810")
TRACE_MSG(ATV_QN8610_305_112_2_18_0_29_52_658,"ATV: the atv is not QN6810")
TRACE_MSG(ATV_QN8610_320_112_2_18_0_29_52_659,"ATV:QN:_qn8610_Init param[%d]")
TRACE_MSG(ATV_QN8610_337_112_2_18_0_29_52_660,"ATV:QN:_qn8610_Sleep sleep_enter[%d]")
TRACE_MSG(ATV_QN8610_358_112_2_18_0_29_52_661,"ATV:QN:_qn8610_ScanChnInit param[%d]")
TRACE_MSG(ATV_QN8610_379_112_2_18_0_29_52_662,"ATV:_qn8610_ScanChn chn_id[%d]")
TRACE_MSG(ATV_QN8610_386_112_2_18_0_29_52_663,"ATV:_qn8610_ScanChn not complete rtn[%d]")
TRACE_MSG(ATV_QN8610_402_112_2_18_0_29_52_664,"ATV:QN:_qn8610_SetChn param[%d]")
TRACE_MSG(ATV_QN8610_443_112_2_18_0_29_52_665,"ATV:QN:_qn8610_GetAllChnNum total_num[%d]")
TRACE_MSG(ATV_QN8610_457_112_2_18_0_29_52_666,"ATV:QN:_qn8610_SetVolume param[%d]")
TRACE_MSG(ATV_QN8610_494_112_2_18_0_29_52_667,"ATV:QN:_qn8610_SetRegion param[%d]")
TRACE_MSG(ATV_QN8610_641_112_2_18_0_29_53_668,"ATV:QN:_qn8610_IsNTSCMode Is_NTSCMode[%d]")
TRACE_MSG(ATV_QN8610_669_112_2_18_0_29_53_669,"ATV:QN:_qn8610_Close")
TRACE_MSG(ATV_QN8610_685_112_2_18_0_29_53_670,"ATV:QN: qn8610_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(ATV_QN8610_698_112_2_18_0_29_53_671,"ATV:QN: qn8610_ReadReg reg/value(%x,%x) !!")
TRACE_MSG(TLG1120CUSTOMIZE_49_112_2_18_0_30_25_672,"TLGAPP_InitVideoSize is ntsc mode : %d")
TRACE_MSG(ATV_TLG1120_260_112_2_18_0_30_28_673,"SENSOR: _TLG1120_InitChannel")
TRACE_MSG(ATV_TLG1120_386_112_2_18_0_30_28_674,"SENSOR: _tlg1120_Power_On(1:on, 0:off): %d")
TRACE_MSG(ATV_TLG1120_425_112_2_18_0_30_29_675,"ATV: _TLG1120_Identify")
TRACE_MSG(ATV_TLG1120_431_112_2_18_0_30_29_676,"ATV: the atv is TLG1120")
TRACE_MSG(ATV_TLG1120_435_112_2_18_0_30_29_677,"ATV: the atv is not TLG1120")
TRACE_MSG(ATV_TLG1120_450_112_2_18_0_30_29_678,"ATV:_TLG1120_Init")
TRACE_MSG(ATV_TLG1120_466_112_2_18_0_30_29_679,"ATV:_TLG1120_Sleep")
TRACE_MSG(ATV_TLG1120_490_112_2_18_0_30_29_680,"ATV:_TLG1120_ScanChnInit")
TRACE_MSG(ATV_TLG1120_522_112_2_18_0_30_29_681,"ATV:_TLG1120_ScanChn")
TRACE_MSG(ATV_TLG1120_547_112_2_18_0_30_29_682,"ATV:_TLG1120_SetChn : 0x%x")
TRACE_MSG(ATV_TLG1120_576_112_2_18_0_30_29_683,"ATV:_TLG1120_GetAllChnNum")
TRACE_MSG(ATV_TLG1120_595_112_2_18_0_30_29_684,"ATV:_TLG1120_SetVolume : 0x0%x")
TRACE_MSG(ATV_TLG1120_635_112_2_18_0_30_29_685,"ATV:_TLG1120_SetRegion")
TRACE_MSG(ATV_TLG1120_737_112_2_18_0_30_29_686,"ATV:_TLG1120_GetRssi")
TRACE_MSG(ATV_TLG1120_788_112_2_18_0_30_29_687,"ATV:_TLG1120_IsNTSCMode")
TRACE_MSG(ATV_TLG1120_802_112_2_18_0_30_29_688,"ATV:_TLG1120_is NTSC")
TRACE_MSG(ATV_TLG1120_807_112_2_18_0_30_29_689,"ATV:_TLG1120_is PAL")
TRACE_MSG(ATV_TLG1120_838_112_2_18_0_30_29_690,"ATV:_TLG1120_Close")
TRACE_MSG(ATV_TLG1120_854_112_2_18_0_30_30_691,"ATV: tlg1120_WriteReg reg/value(0x%x,0x%x) !!")
TRACE_MSG(ATV_TLG1120_867_112_2_18_0_30_30_692,"ATV: tlg1120_ReadReg reg/value(0x%x,0x%x) !!")
TRACE_MSG(SENSOR_BF3703_546_112_2_18_0_30_31_693,"SENSOR: BF3703_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_BF3703_554_112_2_18_0_30_31_694,"BF3703_ReadReg: subaddr:%x,value:%x")
TRACE_MSG(SENSOR_BF3703_570_112_2_18_0_30_31_695,"BF3703_Identify: start")
TRACE_MSG(SENSOR_BF3703_595_112_2_18_0_30_31_696,"BF3703_Identify: it is BF3703")
TRACE_MSG(SENSOR_BF3703_610_112_2_18_0_30_31_697,"BF3703_Identify: ret = %d")
TRACE_MSG(SENSOR_BF3703_633_112_2_18_0_30_31_698,"SENSOR: set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_BF3703_665_112_2_18_0_30_31_699,"set_BF3703_ae_awb_enable: set_ae_awb_enable: ae=%d awb=%d")
TRACE_MSG(SENSOR_BF3703_739_112_2_18_0_30_31_700,"set_BF3703_ev: set_ev: level = %d")
TRACE_MSG(SENSOR_BF3703_769_112_2_18_0_30_31_701,"set_BF3703_anti_flicker: set_banding_mode: mode = %d")
TRACE_MSG(SENSOR_BF3703_811_112_2_18_0_30_31_702,"SENSOR: set_BF3703_video_mode: mode = %d")
TRACE_MSG(SENSOR_BF3703_901_112_2_18_0_30_32_703,"set_BF3703_awb: mode = %d")
TRACE_MSG(SENSOR_BF3703_938_112_2_18_0_30_32_704,"BF3703 set_brightness: level = %d")
TRACE_MSG(SENSOR_BF3703_971_112_2_18_0_30_32_705,"BF3703: set_contrast: level = %d")
TRACE_MSG(SENSOR_BF3703_999_112_2_18_0_30_32_706,"BF3703 set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_BF3703_1072_112_2_18_0_30_32_707,"BF3703 set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_BF3703_1185_112_2_18_0_30_32_708,"BF3703 set_work_mode: mode = %d")
TRACE_MSG(SENSOR_BF3703_1226_112_2_18_0_30_32_709,"SENSOR: _BF3703_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_BF3903_413_112_2_18_0_30_33_710,"SENSOR: BF3903_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_BF3903_421_112_2_18_0_30_33_711,"BF3903_ReadReg: subaddr:%x,value:%x")
TRACE_MSG(SENSOR_BF3903_436_112_2_18_0_30_33_712,"BF3903_Identify: start")
TRACE_MSG(SENSOR_BF3903_460_112_2_18_0_30_33_713,"BF3903_Identify: it is BF3903")
TRACE_MSG(SENSOR_BF3903_482_112_2_18_0_30_33_714,"SENSOR: set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_BF3903_514_112_2_18_0_30_34_715,"set_BF3903_ae_awb_enable: set_ae_awb_enable: ae=%d awb=%d")
TRACE_MSG(SENSOR_BF3903_584_112_2_18_0_30_34_716,"set_BF3903_ev: set_ev: level = %d")
TRACE_MSG(SENSOR_BF3903_633_112_2_18_0_30_34_717,"set_BF3903_anti_flicker: set_banding_mode: mode = %d")
TRACE_MSG(SENSOR_BF3903_675_112_2_18_0_30_34_718,"SENSOR: set_BF3903_video_mode: mode = %d")
TRACE_MSG(SENSOR_BF3903_760_112_2_18_0_30_34_719,"set_BF3903_awb: mode = %d")
TRACE_MSG(SENSOR_BF3903_796_112_2_18_0_30_34_720,"BF3903 set_brightness: level = %d")
TRACE_MSG(SENSOR_BF3903_830_112_2_18_0_30_34_721,"BF3903: set_contrast: level = %d")
TRACE_MSG(SENSOR_BF3903_858_112_2_18_0_30_34_722,"BF3903 set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_BF3903_984_112_2_18_0_30_34_723,"BF3903 set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_BF3903_1095_112_2_18_0_30_35_724,"BF3903 set_work_mode: mode = %d")
TRACE_MSG(SENSOR_BF3903_1128_112_2_18_0_30_35_725,"SENSOR: _BF3903_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_BG0323_544_112_2_18_0_30_36_726,"SENSOR: BG0323_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_BG0323_560_112_2_18_0_30_36_727,"SENSOR: BG0323_ReadReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_BG0323_598_112_2_18_0_30_36_728,"BG0323_Identify: it is BG0323")
TRACE_MSG(SENSOR_BG0323_636_112_2_18_0_30_36_729,"SENSOR: _bg0323_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_BG0323_659_112_2_18_0_30_36_730,"set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_BG0323_697_112_2_18_0_30_36_731,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_BG0323_765_112_2_18_0_30_36_732,"SENSOR: set_video_mode: mode = %d")
TRACE_MSG(SENSOR_BG0323_801_112_2_18_0_30_37_733,"set_ae_awb_enable: ae=%d awb=%d")
TRACE_MSG(SENSOR_BG0323_814_112_2_18_0_30_37_734,"set_bg0323_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_BG0323_826_112_2_18_0_30_37_735,"set_bg0323_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_BG0323_862_112_2_18_0_30_37_736,"bg0323_set_brightness: level = %d")
TRACE_MSG(SENSOR_BG0323_894_112_2_18_0_30_37_737,"bg0323_set_contrast: level = %d")
TRACE_MSG(SENSOR_BG0323_922_112_2_18_0_30_37_738,"set_bg0323_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_BG0323_1117_112_2_18_0_30_37_739,"bg0323_set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_BG0323_1267_112_2_18_0_30_37_740,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_BG0323_1373_112_2_18_0_30_38_741,"BG0323_set_whitebalance_mode: mode = %d")
TRACE_MSG(SENSOR_GC0306_418_112_2_18_0_30_39_742,"SENSOR: GC0306_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_GC0306_434_112_2_18_0_30_39_743,"SENSOR: GC0306_ReadReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_GC0306_454_112_2_18_0_30_39_744,"GC0306_Identify")
TRACE_MSG(SENSOR_GC0306_464_112_2_18_0_30_39_745,"It is not GC0306")
TRACE_MSG(SENSOR_GC0306_479_112_2_18_0_30_39_746,"GC0306_Identify: it is GC0306")
TRACE_MSG(SENSOR_GC0306_486_112_2_18_0_30_39_747,"set_gc0306_ae_enable: enable = %d")
TRACE_MSG(SENSOR_GC0306_494_112_2_18_0_30_39_748,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_GC0306_503_112_2_18_0_30_39_749,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_GC0306_564_112_2_18_0_30_39_750,"set_brightness: level = %d")
TRACE_MSG(SENSOR_GC0306_705_112_2_18_0_30_39_751,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_GC0306_734_112_2_18_0_30_39_752,"SENSOR: set_banding_mode: mode = %d")
TRACE_MSG(SENSOR_GC0306_780_112_2_18_0_30_39_753,"SENSOR: set_video_mode: mode = %d")
TRACE_MSG(SENSOR_GC0306_865_112_2_18_0_30_40_754,"SENSOR: set_gc0306_awb: mode = %d")
TRACE_MSG(SENSOR_GC0306_913_112_2_18_0_30_40_755,"set_gc0306_awb: not supported ")
TRACE_MSG(SENSOR_GC0306_946_112_2_18_0_30_40_756,"set_contrast: level = %d")
TRACE_MSG(SENSOR_GC0306_973_112_2_18_0_30_40_757,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_GC0306_1137_112_2_18_0_30_40_758,"set_image_effect: not supported!")
TRACE_MSG(SENSOR_GC0306_1167_112_2_18_0_30_40_759,"GC0306_BeforeSnapshot ")
TRACE_MSG(SENSOR_GC0306_1274_112_2_18_0_30_40_760,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_GC0307_931_112_2_18_0_30_42_761,"SENSOR: GC0307_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_GC0307_968_112_2_18_0_30_43_762,"GC0307_Identify-START")
TRACE_MSG(SENSOR_GC0307_979_112_2_18_0_30_43_763,"GC0307 read ret is %X")
TRACE_MSG(SENSOR_GC0307_986_112_2_18_0_30_43_764,"GC0307 Fail")
TRACE_MSG(SENSOR_GC0307_1004_112_2_18_0_30_43_765,"GC0307 succ")
TRACE_MSG(SENSOR_GC0307_1006_112_2_18_0_30_43_766,"GC0307_Identify-END")
TRACE_MSG(SENSOR_GC0307_1099_112_2_18_0_30_43_767,"set_brightness: level = %d")
TRACE_MSG(SENSOR_GC0307_1155_112_2_18_0_30_43_768,"set_contrast: level = %d")
TRACE_MSG(SENSOR_GC0307_1195_112_2_18_0_30_43_769,"set preview mod %d")
TRACE_MSG(SENSOR_GC0307_1401_112_2_18_0_30_44_770,"-----------set_image_effect: effect_type = %d------------")
TRACE_MSG(SENSOR_GC0307_1454_112_2_18_0_30_44_771,"SENSOR_GC0307: Before Snapshot")
TRACE_MSG(SENSOR_GC0307_1470_112_2_18_0_30_44_772,"SENSOR: After Snapshot")
TRACE_MSG(SENSOR_GC0307_1563_112_2_18_0_30_44_773,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_GC0307_1585_112_2_18_0_30_44_774,"SENSOR: set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_GC0307_1681_112_2_18_0_30_44_775,"SENSOR: set_video_mode: mode = %d,bl_GC_50Hz_GC0307=%d")
TRACE_MSG(SENSOR_GC0307_1717_112_2_18_0_30_44_776,"SENSOR: set_video_mode: mode = %d")
TRACE_MSG(SENSOR_GC0307_1750_112_2_18_0_30_44_777,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_GC0307_1806_112_2_18_0_30_44_778,"SENSOR: set_GC0307_AntiFlicker: mode = %d")
TRACE_MSG(SENSOR_GC0309_690_112_2_18_0_30_46_779,"SENSOR: GC0309_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_GC0309_706_112_2_18_0_30_46_780,"SENSOR: GC0309_ReadReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_GC0309_725_112_2_18_0_30_46_781,"GC0309_Identify")
TRACE_MSG(SENSOR_GC0309_735_112_2_18_0_30_46_782,"It is not GC0309")
TRACE_MSG(SENSOR_GC0309_753_112_2_18_0_30_46_783,"GC0309_Identify: it is GC0309")
TRACE_MSG(SENSOR_GC0309_760_112_2_18_0_30_46_784,"set_gc0309_ae_enable: enable = %d")
TRACE_MSG(SENSOR_GC0309_783_112_2_18_0_30_46_785,"set_gc0309_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_GC0309_808_112_2_18_0_30_46_786,"set_gc0309_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_GC0309_863_112_2_18_0_30_47_787,"set_brightness: level = %d")
TRACE_MSG(SENSOR_GC0309_895_112_2_18_0_30_47_788,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_GC0309_923_112_2_18_0_30_47_789,"DEBUG_gc0309 anti flick 50HZ!!!!")
TRACE_MSG(SENSOR_GC0309_940_112_2_18_0_30_47_790,"DEBUG_gc0309 anti flick 60HZ!!!!")
TRACE_MSG(SENSOR_GC0309_991_112_2_18_0_30_47_791,"SENSOR: set_video_mode: mode = %d")
TRACE_MSG(SENSOR_GC0309_1159_112_2_18_0_30_47_792,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_GC0309_1211_112_2_18_0_30_47_793,"set_contrast: level = %d")
TRACE_MSG(SENSOR_GC0309_1238_112_2_18_0_30_47_794,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_GC0309_1256_112_2_18_0_30_47_795,"set_GC0309_preview_mode: level = %d")
TRACE_MSG(SENSOR_GC0309_1335_112_2_18_0_30_47_796,"-----------set_image_effect: effect_type = %d------------")
TRACE_MSG(SENSOR_GC0309_1362_112_2_18_0_30_48_797,"GC0309_BeforeSnapshot ")
TRACE_MSG(SENSOR_GC0309_1469_112_2_18_0_30_48_798,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_GC2015_530_112_2_18_0_30_49_799,"set_GC2015_Power_On")
TRACE_MSG(SENSOR_GC2015_554_112_2_18_0_30_49_800,"GC2015_Identify-id_h_value %d")
TRACE_MSG(SENSOR_GC2015_555_112_2_18_0_30_49_801,"GC2015_Identify-id_l_value %d")
TRACE_MSG(SENSOR_GC2015_560_112_2_18_0_30_49_802,"It Is GC2015 Sensor !")
TRACE_MSG(SENSOR_GC2015_588_112_2_18_0_30_49_803,"GC0309 WriteReg reg/value(%x,%x) ")
TRACE_MSG(SENSOR_GC2015_609_112_2_18_0_30_49_804,"GC0309 ReadReg reg/value(%x,%x) ")
TRACE_MSG(SENSOR_GC2015_704_112_2_18_0_30_49_805,"set_GC2015_brightness: level = %d")
TRACE_MSG(SENSOR_GC2015_734_112_2_18_0_30_49_806,"set_GC0309_contrast: level = %d")
TRACE_MSG(SENSOR_GC2015_769_112_2_18_0_30_49_807,"set_GC2015_preview_mode: level = %d")
TRACE_MSG(SENSOR_GC2015_811_112_2_18_0_30_49_808,"set_GC2015_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_GC2015_832_112_2_18_0_30_49_809,"Capture VGA Size")
TRACE_MSG(SENSOR_GC2015_840_112_2_18_0_30_49_810,"Capture 1.3M&2M Size")
TRACE_MSG(SENSOR_GC2015_855_112_2_18_0_30_49_811,"SENSOR_GC2015: Before Snapshot mode = sensor_snapshot_mode")
TRACE_MSG(SENSOR_GC2015_881_112_2_18_0_30_50_812,"SENSOR_GC2015: After Snapshot")
TRACE_MSG(SENSOR_GC2015_970_112_2_18_0_30_50_813,"set_GC2015_awb_mode: mode = %d")
TRACE_MSG(SENSOR_GC2015_1002_112_2_18_0_30_50_814,"set_GC2015_ev: level = %d")
TRACE_MSG(SENSOR_GC2015_1230_112_2_18_0_30_50_815,"set_GC2015_anti_flicker-mode=%d")
TRACE_MSG(SENSOR_GC2015_1263_112_2_18_0_30_50_816,"set_GC2015_video_mode=%d")
TRACE_MSG(SENSOR_GC2015_1356_112_2_18_0_30_50_817,"SENSOR: set_ae_enable: ae_enable = %d")
TRACE_MSG(SENSOR_GC2015_1380_112_2_18_0_30_50_818,"SENSOR: set_awb_enable: awb_enable = %d")
TRACE_MSG(SENSOR_GC6113_517_112_2_18_0_30_52_819,"GC6113_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_GC6113_525_112_2_18_0_30_52_820,"GC6113_ReadReg: subaddr:%x,value:%x")
TRACE_MSG(SENSOR_GC6113_620_112_2_18_0_30_52_821,"SENSOR: GC6113 Read reg0x00 = %x")
TRACE_MSG(SENSOR_GC6113_627_112_2_18_0_30_52_822,"GC6113 Fail")
TRACE_MSG(SENSOR_GC6113_640_112_2_18_0_30_52_823,"GC6113 Identify")
TRACE_MSG(SENSOR_GC6113_709_112_2_18_0_30_52_824,"set_GC6113_brightness: level = %d")
TRACE_MSG(SENSOR_GC6113_756_112_2_18_0_30_52_825,"set_GC6113_contrast: level = %d")
TRACE_MSG(SENSOR_GC6113_807_112_2_18_0_30_52_826,"set_GC6113_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_GC6113_835_112_2_18_0_30_52_827,"set_GC6113_ev: level = %d")
TRACE_MSG(SENSOR_GC6113_902_112_2_18_0_30_52_828,"set_GC6113_anti_flicker-mode=%d")
TRACE_MSG(SENSOR_GC6113_932_112_2_18_0_30_53_829,"set_GC6113_preview_mode: level = %d")
TRACE_MSG(SENSOR_GC6113_1004_112_2_18_0_30_53_830,"set_GC6113_awb_mode: mode = %d")
TRACE_MSG(SENSOR_GC6113_1036_112_2_18_0_30_53_831,"SENSOR: set_ae_awb_enable: ae_enable = %d  awb_enable = %d")
TRACE_MSG(SENSOR_GC6113_1065_112_2_18_0_30_53_832,"SENSOR_GC6113: Before Snapshot")
TRACE_MSG(SENSOR_GC6113_1078_112_2_18_0_30_53_833,"SENSOR_GC6113: After Snapshot")
TRACE_MSG(SENSOR_GC6113_1142_112_2_18_0_30_53_834,"set_GC6113_video_mode=%d")
TRACE_MSG(SENSOR_GT2005_879_112_2_18_0_30_55_835,"GT2005 WriteReg reg/value(%x,%x) ")
TRACE_MSG(SENSOR_GT2005_887_112_2_18_0_30_55_836,"GT2005 ReadReg reg/value(%x,%x) ")
TRACE_MSG(SENSOR_GT2005_927_112_2_18_0_30_55_837,"set_GT2005_Power_On")
TRACE_MSG(SENSOR_GT2005_997_112_2_18_0_30_55_838,"set_GT2005_preview_mode: level = %d")
TRACE_MSG(SENSOR_GT2005_1011_112_2_18_0_30_56_839,"GT2005_Identify-id_h_value %d")
TRACE_MSG(SENSOR_GT2005_1014_112_2_18_0_30_56_840,"GT2005_Identify-id_l_value %d")
TRACE_MSG(SENSOR_GT2005_1020_112_2_18_0_30_56_841,"It Is GT2005 Sensor !")
TRACE_MSG(SENSOR_GT2005_1061_112_2_18_0_30_56_842,"set_GT2005_brightness: level = %d")
TRACE_MSG(SENSOR_GT2005_1088_112_2_18_0_30_56_843,"set_GT2005_contrast: level = %d")
TRACE_MSG(SENSOR_GT2005_1124_112_2_18_0_30_56_844,"set_GT2005_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_GT2005_1150_112_2_18_0_30_56_845,"set_GT2005_ev: level = %d")
TRACE_MSG(SENSOR_GT2005_1188_112_2_18_0_30_56_846,"set_GT2005_anti_flicker-mode=%d")
TRACE_MSG(SENSOR_GT2005_1310_112_2_18_0_30_56_847,"set_GT2005_awb_mode: mode = %d")
TRACE_MSG(SENSOR_GT2005_1360_112_2_18_0_30_56_848,"SENSOR_GT2005: Before Snapshot")
TRACE_MSG(SENSOR_GT2005_1375_112_2_18_0_30_56_849,"SENSOR_GT2005: After Snapshot")
TRACE_MSG(SENSOR_GT2005_1450_112_2_18_0_30_56_850,"set_GT2005_video_mode=%d")
TRACE_MSG(SENSOR_GT2005_1457_112_2_18_0_30_57_851,"SENSOR: GT2005_InitExifInfo")
TRACE_MSG(SENSOR_HI253_1677_112_2_18_0_31_0_852,"SENSOR: HI253_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_HI253_1696_112_2_18_0_31_0_853,"SENSOR: HI253_ReadReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_HI253_1737_112_2_18_0_31_0_854,"HI253_Identify: it is HI253")
TRACE_MSG(SENSOR_HI253_1777_112_2_18_0_31_0_855,"SENSOR: _hi253_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_HI253_1802_112_2_18_0_31_0_856,"set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_HI253_1833_112_2_18_0_31_0_857,"set_ae_awb_enable: ae=%d awb=%d")
TRACE_MSG(SENSOR_HI253_1903_112_2_18_0_31_0_858,"set_hi253_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_HI253_1984_112_2_18_0_31_1_859,"set_hi253_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_HI253_2049_112_2_18_0_31_1_860,"set_brightness: level = %d")
TRACE_MSG(SENSOR_HI253_2138_112_2_18_0_31_1_861,"set_hi253_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_HI253_2268_112_2_18_0_31_1_862,"HI253_set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_HI253_2278_112_2_18_0_31_1_863,"HI253_After_Snapshot ")
TRACE_MSG(SENSOR_HI253_2294_112_2_18_0_31_1_864,"sensor:HI253_Before_Snapshot")
TRACE_MSG(SENSOR_HI253_2306_112_2_18_0_31_1_865,"HI253_Before_Snapshot:dont set any")
TRACE_MSG(SENSOR_HI253_2309_112_2_18_0_31_1_866,"sensor:HI253_Before_Snapshot2")
TRACE_MSG(SENSOR_HI253_2397_112_2_18_0_31_1_867,"sensor:HI253_set_work_mode: mode = %d")
TRACE_MSG(SENSOR_HI253_2489_112_2_18_0_31_2_868,"HI253_set_whitebalance_mode: mode = %d")
TRACE_MSG(SENSOR_HI253_2653_112_2_18_0_31_2_869,"SENSOR: set_hi253_video_mode: mode = %d")
TRACE_MSG(SENSOR_HI253_2720_112_2_18_0_31_2_870,"SENSOR: set_hi253_ev: level = %d")
TRACE_MSG(SENSOR_MT9D112_567_112_2_18_0_31_3_871,"set_mt9d112_mode: level = %d")
TRACE_MSG(SENSOR_MT9D112_627_112_2_18_0_31_3_872,"set_brightness: level = %d")
TRACE_MSG(SENSOR_MT9D112_1260_112_2_18_0_31_5_873,"set_mt9d112_contrast: level = %d")
TRACE_MSG(SENSOR_MT9D112_1373_112_2_18_0_31_5_874,"set_mt9d112_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_MT9D112_1459_112_2_18_0_31_5_875,"DCAM: sensor exposure value: 0x%x")
TRACE_MSG(SENSOR_MT9D112_1460_112_2_18_0_31_5_876,"DCAM: sensor reg_val_gain1 value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1461_112_2_18_0_31_5_877,"DCAM: sensor reg_val_gain2 value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1462_112_2_18_0_31_5_878,"DCAM: sensor reg_val_gain3 value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1463_112_2_18_0_31_5_879,"DCAM: sensor reg_val_gain4 value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1519_112_2_18_0_31_5_880,"DCAM: sensor close_reg value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1531_112_2_18_0_31_5_881,"DCAM: sensor exp_reg value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1547_112_2_18_0_31_5_882,"DCAM: sensor exp_reg value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1672_112_2_18_0_31_6_883,"DCAM: sensor reg_val_gain1 value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1673_112_2_18_0_31_6_884,"DCAM: sensor reg_val_gain2 value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1674_112_2_18_0_31_6_885,"DCAM: sensor reg_val_gain3 value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1675_112_2_18_0_31_6_886,"DCAM: sensor reg_val_gain1 value: 0x%x, 0x%x")
TRACE_MSG(SENSOR_MT9D112_1694_112_2_18_0_31_6_887,"mt9d112: DCAMERA_WB_MODE_INCANDESCENCE")
TRACE_MSG(SENSOR_MT9D112_1704_112_2_18_0_31_6_888,"mt9d112: DCAMERA_WB_MODE_FLUORESCENT")
TRACE_MSG(SENSOR_MT9D112_1714_112_2_18_0_31_6_889,"mt9d112: DCAMERA_WB_MODE_SUN")
TRACE_MSG(SENSOR_MT9D112_1725_112_2_18_0_31_6_890,"mt9d112: DCAMERA_WB_MODE_CLOUD")
TRACE_MSG(SENSOR_MT9D112_1736_112_2_18_0_31_6_891,"mt9d112: DCAMERA_WB_MODE_AUTO")
TRACE_MSG(SENSOR_MT9D112_1977_112_2_18_0_31_6_892,"MT9D112_Identify: I2C error id = %d")
TRACE_MSG(SENSOR_MT9D112_1996_112_2_18_0_31_6_893,"MT9D112_Identify: Succ")
TRACE_MSG(SENSOR_MT9D112_2001_112_2_18_0_31_6_894,"MT9D112_Identify: Fail reg_val[0] = 0x%X, [1] = 0x%X")
TRACE_MSG(SENSOR_MT9M112_285_112_2_18_0_31_7_895,"reg: 0x%x, write_val: 0x%x")
TRACE_MSG(SENSOR_MT9M112_298_112_2_18_0_31_7_896,"SENSOR: Delay %d ms")
TRACE_MSG(SENSOR_MT9M112_303_112_2_18_0_31_7_897,"reg: 0x%x, write_val: 0x%x")
TRACE_MSG(SENSOR_MT9M112_321_112_2_18_0_31_7_898,"ERROR reg: 0x%x, write: 0x%x, read: 0x%x, temp[0]: 0x%x, temp[1]: 0x%x")
TRACE_MSG(SENSOR_MT9M112_341_112_2_18_0_31_7_899,"MT9M112 read_reg reg: 0x%x, val: 0x%x")
TRACE_MSG(SENSOR_MT9M112_350_112_2_18_0_31_7_900,"set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_MT9M112_357_112_2_18_0_31_7_901,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_MT9M112_364_112_2_18_0_31_7_902,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_MT9M112_377_112_2_18_0_31_7_903,"read_ae_value: %x")
TRACE_MSG(SENSOR_MT9M112_383_112_2_18_0_31_7_904,"write_ae_value: %x")
TRACE_MSG(SENSOR_MT9M112_392_112_2_18_0_31_7_905,"read_gain_value: ")
TRACE_MSG(SENSOR_MT9M112_398_112_2_18_0_31_7_906,"write_gain_value: %x")
TRACE_MSG(SENSOR_MT9M112_410_112_2_18_0_31_7_907,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_MT9M112_462_112_2_18_0_31_7_908,"set_brightness: level = %d")
TRACE_MSG(SENSOR_MT9M112_469_112_2_18_0_31_7_909,"set_contrast: level = %d")
TRACE_MSG(SENSOR_MT9M112_476_112_2_18_0_31_7_910,"set_sharpness: level = %d")
TRACE_MSG(SENSOR_MT9M112_483_112_2_18_0_31_7_911,"set_saturation: level = %d")
TRACE_MSG(SENSOR_MT9M112_490_112_2_18_0_31_7_912,"-----------set_image_effect: effect_type = %d------------")
TRACE_MSG(SENSOR_MT9M112_497_112_2_18_0_31_7_913,"set_frame_rate: param = %d")
TRACE_MSG(SENSOR_MT9M112_518_112_2_18_0_31_8_914,"this sensor is MT9M112")
TRACE_MSG(SENSOR_MT9M112_523_112_2_18_0_31_8_915,"this sensor is not MT9M112")
TRACE_MSG(SENSOR_MT9M112_530_112_2_18_0_31_8_916,"Sensor_Set_ChangeMega_MT9M112")
TRACE_MSG(SENSOR_MT9M112_537_112_2_18_0_31_8_917,"MT9M112_BeforeSnapshot: param = %d")
TRACE_MSG(SENSOR_NOON010PC20_528_112_2_18_0_31_15_918,"SENSOR: NOON010PC20_WriteReg 0x%x=0x%x")
TRACE_MSG(SENSOR_NOON010PC20_543_112_2_18_0_31_15_919,"SENSOR: NOON010PC20_WriteReg 0x%x=0x%x")
TRACE_MSG(SENSOR_NOON010PC20_554_112_2_18_0_31_15_920,"NOON010PC20_Identify: it is NOON010PC20")
TRACE_MSG(SENSOR_NOON010PC20_562_112_2_18_0_31_15_921,"This is not NOON010PC20 sensor")
TRACE_MSG(SENSOR_NOON010PC20_566_112_2_18_0_31_15_922,"NOON010PC20_Identify: it is NOON010PC20")
TRACE_MSG(SENSOR_NOON010PC20_585_112_2_18_0_31_15_923,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_NOON010PC20_597_112_2_18_0_31_15_924,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_NOON010PC20_633_112_2_18_0_31_15_925,"set_brightness: level = %d")
TRACE_MSG(SENSOR_NOON010PC20_665_112_2_18_0_31_15_926,"set_contrast: level = %d")
TRACE_MSG(SENSOR_NOON010PC20_693_112_2_18_0_31_15_927,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_NOON010PC20_787_112_2_18_0_31_15_928,"set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_NOON010PC20_898_112_2_18_0_31_16_929,"noon010pc20_set_iso: iso_type = %d")
TRACE_MSG(SENSOR_NOON010PC20_953_112_2_18_0_31_16_930,"OV2640_set_wb_mode: wb_mode = %d")
TRACE_MSG(SENSOR_NOON010PC20_987_112_2_18_0_31_16_931,"noon010pc20_set_flicker_elimination_mode: flicker_mode = %d")
TRACE_MSG(SENSOR_NOON010PC20_1021_112_2_18_0_31_16_932,"noon010pc20_ev_compensation: ev_mode = %d")
TRACE_MSG(SENSOR_NOON010PC20_1057_112_2_18_0_31_16_933,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_NOON010PC30L_596_112_2_18_0_31_17_934,"SENSOR: _noon010pc30l_Power_On  power_on =0x%x")
TRACE_MSG(SENSOR_NOON010PC30L_652_112_2_18_0_31_18_935,"SENSOR: NOON010PC30L_WriteReg 0x%x=0x%x")
TRACE_MSG(SENSOR_NOON010PC30L_667_112_2_18_0_31_18_936,"SENSOR: noon010pc30l_ReadReg 0x%x=0x%x")
TRACE_MSG(SENSOR_NOON010PC30L_680_112_2_18_0_31_18_937,"NOON010PC30L_Identify: it is NOON010PC30L ret=%d")
TRACE_MSG(SENSOR_NOON010PC30L_685_112_2_18_0_31_18_938,"NOON010PC30L_Identify: it is NOON010PC30L 0x%x after read reg")
TRACE_MSG(SENSOR_NOON010PC30L_689_112_2_18_0_31_18_939,"This is not NOON010PC30L sensor")
TRACE_MSG(SENSOR_NOON010PC30L_693_112_2_18_0_31_18_940,"NOON010PC30L_Identify: it is NOON010PC30L")
TRACE_MSG(SENSOR_NOON010PC30L_712_112_2_18_0_31_18_941,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_NOON010PC30L_724_112_2_18_0_31_18_942,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_NOON010PC30L_785_112_2_18_0_31_18_943,"set_brightness: level = %d")
TRACE_MSG(SENSOR_NOON010PC30L_817_112_2_18_0_31_18_944,"set_contrast: level = %d")
TRACE_MSG(SENSOR_NOON010PC30L_845_112_2_18_0_31_18_945,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_NOON010PC30L_958_112_2_18_0_31_18_946,"set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_NOON010PC30L_1069_112_2_18_0_31_18_947,"noon010pc30l_set_iso: iso_type = %d")
TRACE_MSG(SENSOR_NOON010PC30L_1174_112_2_18_0_31_19_948,"OV2640_set_wb_mode: wb_mode = %d")
TRACE_MSG(SENSOR_NOON010PC30L_1208_112_2_18_0_31_19_949,"noon010pc30l_set_flicker_elimination_mode: flicker_mode = %d")
TRACE_MSG(SENSOR_NOON010PC30L_1242_112_2_18_0_31_19_950,"noon010pc30l_ev_compensation: ev_mode = %d")
TRACE_MSG(SENSOR_NOON010PC30L_1282_112_2_18_0_31_19_951,"noon010pc30l_set_meter_mode: mode = %d, rect(%d,%d,%d,%d)")
TRACE_MSG(SENSOR_NOON010PC30L_1319_112_2_18_0_31_19_952,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_NT99040_579_112_2_18_0_31_20_953,"SENSOR: set_NT99040_flicker: 0x%x")
TRACE_MSG(SENSOR_NT99040_678_112_2_18_0_31_21_954,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_NT99040_717_112_2_18_0_31_21_955,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_NT99040_778_112_2_18_0_31_21_956,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_NT99040_909_112_2_18_0_31_21_957,"Reg0x3012=0x%x")
TRACE_MSG(SENSOR_NT99040_910_112_2_18_0_31_21_958,"Reg0x3013=0x%x")
TRACE_MSG(SENSOR_NT99040_1058_112_2_18_0_31_21_959,"Reg0x3000=%d")
TRACE_MSG(SENSOR_NT99040_1059_112_2_18_0_31_21_960,"g_PV_Shutter=%d")
TRACE_MSG(SENSOR_NT99040_1060_112_2_18_0_31_22_961,"g_PV_Extra_Lines=%d")
TRACE_MSG(SENSOR_NT99040_1061_112_2_18_0_31_22_962,"PV_Line_Width=%d")
TRACE_MSG(SENSOR_NT99040_1062_112_2_18_0_31_22_963,"Capture_Line_Width=%d")
TRACE_MSG(SENSOR_NT99040_1095_112_2_18_0_31_22_964,"PV_Line_Width=%d")
TRACE_MSG(SENSOR_NT99040_1096_112_2_18_0_31_22_965,"Capture_Line_Width=%d")
TRACE_MSG(SENSOR_NT99040_1097_112_2_18_0_31_22_966,"Capture_Maximum_Shutter=%d")
TRACE_MSG(SENSOR_NT99040_1098_112_2_18_0_31_22_967,"Capture_Banding_Filter=%d")
TRACE_MSG(SENSOR_NT99040_1099_112_2_18_0_31_22_968,"g_PV_Gain16=%d")
TRACE_MSG(SENSOR_NT99040_1100_112_2_18_0_31_22_969,"Gain_Exposure=%d")
TRACE_MSG(SENSOR_NT99040_1106_112_2_18_0_31_22_970,"if (Gain_Exposure < Capture_Banding_Filter * 16)")
TRACE_MSG(SENSOR_NT99040_1113_112_2_18_0_31_22_971,"Gain_Exposure<16 Capture_Gain16=%d")
TRACE_MSG(SENSOR_NT99040_1120_112_2_18_0_31_22_972,"Gain_Exposure<16 else---- Capture_Gain16=%d")
TRACE_MSG(SENSOR_NT99040_1126_112_2_18_0_31_22_973,"if (Gain_Exposure < Capture_Banding_Filter * 16)  else--------")
TRACE_MSG(SENSOR_NT99040_1143_112_2_18_0_31_22_974,"Gain_Exposure<16 Capture_Gain16=%d")
TRACE_MSG(SENSOR_NT99040_1152_112_2_18_0_31_22_975,"Gain_Exposure<16 else---- Capture_Gain16=%d")
TRACE_MSG(SENSOR_NT99040_1216_112_2_18_0_31_22_976,"set_sharpness: level = %d")
TRACE_MSG(SENSOR_NT99040_1266_112_2_18_0_31_22_977,"set_saturation: level = %d")
TRACE_MSG(SENSOR_NT99040_1346_112_2_18_0_31_22_978,"set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_NT99040_1399_112_2_18_0_31_22_979,"set_brightness: reg3391 = %d")
TRACE_MSG(SENSOR_NT99040_1410_112_2_18_0_31_22_980,"set_brightness: level = %d")
TRACE_MSG(SENSOR_NT99040_1459_112_2_18_0_31_22_981,"set_contrast: reg3391 = %d")
TRACE_MSG(SENSOR_NT99040_1469_112_2_18_0_31_22_982,"set_contrast: level = %d")
TRACE_MSG(SENSOR_NT99040_1517_112_2_18_0_31_23_983,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_NT99250_669_112_2_18_0_31_24_984,"SENSOR: set_NT99250_flicker: 0x%x")
TRACE_MSG(SENSOR_NT99250_768_112_2_18_0_31_25_985,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_NT99250_807_112_2_18_0_31_25_986,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_NT99250_868_112_2_18_0_31_25_987,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_NT99250_999_112_2_18_0_31_25_988,"Reg0x3012=0x%x")
TRACE_MSG(SENSOR_NT99250_1000_112_2_18_0_31_25_989,"Reg0x3013=0x%x")
TRACE_MSG(SENSOR_NT99250_1148_112_2_18_0_31_25_990,"Reg0x3000=%d")
TRACE_MSG(SENSOR_NT99250_1149_112_2_18_0_31_25_991,"g_PV_Shutter=%d")
TRACE_MSG(SENSOR_NT99250_1150_112_2_18_0_31_25_992,"g_PV_Extra_Lines=%d")
TRACE_MSG(SENSOR_NT99250_1151_112_2_18_0_31_25_993,"PV_Line_Width=%d")
TRACE_MSG(SENSOR_NT99250_1152_112_2_18_0_31_25_994,"Capture_Line_Width=%d")
TRACE_MSG(SENSOR_NT99250_1185_112_2_18_0_31_25_995,"PV_Line_Width=%d")
TRACE_MSG(SENSOR_NT99250_1186_112_2_18_0_31_25_996,"Capture_Line_Width=%d")
TRACE_MSG(SENSOR_NT99250_1187_112_2_18_0_31_25_997,"Capture_Maximum_Shutter=%d")
TRACE_MSG(SENSOR_NT99250_1188_112_2_18_0_31_25_998,"Capture_Banding_Filter=%d")
TRACE_MSG(SENSOR_NT99250_1189_112_2_18_0_31_25_999,"g_PV_Gain16=%d")
TRACE_MSG(SENSOR_NT99250_1190_112_2_18_0_31_25_1000,"Gain_Exposure=%d")
TRACE_MSG(SENSOR_NT99250_1196_112_2_18_0_31_25_1001,"if (Gain_Exposure < Capture_Banding_Filter * 16)")
TRACE_MSG(SENSOR_NT99250_1203_112_2_18_0_31_25_1002,"Gain_Exposure<16 Capture_Gain16=%d")
TRACE_MSG(SENSOR_NT99250_1210_112_2_18_0_31_25_1003,"Gain_Exposure<16 else---- Capture_Gain16=%d")
TRACE_MSG(SENSOR_NT99250_1216_112_2_18_0_31_25_1004,"if (Gain_Exposure < Capture_Banding_Filter * 16)  else--------")
TRACE_MSG(SENSOR_NT99250_1233_112_2_18_0_31_25_1005,"Gain_Exposure<16 Capture_Gain16=%d")
TRACE_MSG(SENSOR_NT99250_1242_112_2_18_0_31_25_1006,"Gain_Exposure<16 else---- Capture_Gain16=%d")
TRACE_MSG(SENSOR_NT99250_1306_112_2_18_0_31_26_1007,"set_sharpness: level = %d")
TRACE_MSG(SENSOR_NT99250_1356_112_2_18_0_31_26_1008,"set_saturation: level = %d")
TRACE_MSG(SENSOR_NT99250_1436_112_2_18_0_31_26_1009,"set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_NT99250_1489_112_2_18_0_31_26_1010,"set_brightness: reg3391 = %d")
TRACE_MSG(SENSOR_NT99250_1500_112_2_18_0_31_26_1011,"set_brightness: level = %d")
TRACE_MSG(SENSOR_NT99250_1549_112_2_18_0_31_26_1012,"set_contrast: reg3391 = %d")
TRACE_MSG(SENSOR_NT99250_1559_112_2_18_0_31_26_1013,"set_contrast: level = %d")
TRACE_MSG(SENSOR_NT99250_1607_112_2_18_0_31_26_1014,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV2640_664_112_2_18_0_31_28_1015,"OV2640_set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_OV2640_703_112_2_18_0_31_28_1016,"SENSOR: set_ov2640_flicker: 0x%x")
TRACE_MSG(SENSOR_OV2640_960_112_2_18_0_31_28_1017,"SENSOR: set_vodeo_mode: mode : %d")
TRACE_MSG(SENSOR_OV2640_1057_112_2_18_0_31_29_1018,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_OV2640_1115_112_2_18_0_31_29_1019,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_OV2640_1181_112_2_18_0_31_29_1020,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV2640_1239_112_2_18_0_31_29_1021,"That is OV2640 sensor !")
TRACE_MSG(SENSOR_OV2640_1243_112_2_18_0_31_29_1022,"OV2640_Identify: That is OV%x%x sensor !")
TRACE_MSG(SENSOR_OV2640_1249_112_2_18_0_31_29_1023,"OV2640_Identify: PID = %x, VER = %x")
TRACE_MSG(SENSOR_OV2640_1287_112_2_18_0_31_29_1024,"ov2640: prvGain %d, prvExp %d")
TRACE_MSG(SENSOR_OV2640_1310_112_2_18_0_31_29_1025,"ov2640: PrvGain %d, TgtExp %d")
TRACE_MSG(SENSOR_OV2640_1454_112_2_18_0_31_29_1026,"set_brightness: level = %d")
TRACE_MSG(SENSOR_OV2640_1561_112_2_18_0_31_30_1027,"set_contrast: level = %d")
TRACE_MSG(SENSOR_OV2640_1593_112_2_18_0_31_30_1028,"set_sharpness: level = %d")
TRACE_MSG(SENSOR_OV2640_1653_112_2_18_0_31_30_1029,"set_saturation: level = %d")
TRACE_MSG(SENSOR_OV2640_1715_112_2_18_0_31_30_1030,"set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_OV2640_1764_112_2_18_0_31_30_1031,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV2640_1825_112_2_18_0_31_30_1032,"ov3640: jpeg capture head: 0x%x, 0x%x")
TRACE_MSG(SENSOR_OV2640_1842_112_2_18_0_31_30_1033,"ov3640: Found the jpeg tail at %d: 0x%x 0x%x")
TRACE_MSG(SENSOR_OV2640_1845_112_2_18_0_31_30_1034,"ov3640: can not find the jpeg tail: %d")
TRACE_MSG(SENSOR_OV2640_2026_112_2_18_0_31_30_1035,"OV2640_zoom: mode=%d,level = %d")
TRACE_MSG(SENSOR_OV2655_645_112_2_18_0_31_32_1036,"OV2655_set_ae_Rot: %d")
TRACE_MSG(SENSOR_OV2655_703_112_2_18_0_31_32_1037,"SENSOR: set_ov2655_flicker: 0x%x")
TRACE_MSG(SENSOR_OV2655_797_112_2_18_0_31_32_1038,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_OV2655_831_112_2_18_0_31_32_1039,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_OV2655_847_112_2_18_0_31_32_1040,"OV2655_set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV2655_918_112_2_18_0_31_32_1041,"OV2655_set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV2655_984_112_2_18_0_31_33_1042,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV2655_1038_112_2_18_0_31_33_1043,"OV2655_Identify: ret = %d")
TRACE_MSG(SENSOR_OV2655_1111_112_2_18_0_31_33_1044,"Reg0x3002=0x%x")
TRACE_MSG(SENSOR_OV2655_1112_112_2_18_0_31_33_1045,"Reg0x3003=0x%x")
TRACE_MSG(SENSOR_OV2655_1229_112_2_18_0_31_33_1046,"set_sharpness: level = %d")
TRACE_MSG(SENSOR_OV2655_1279_112_2_18_0_31_33_1047,"set_saturation: level = %d")
TRACE_MSG(SENSOR_OV2655_1359_112_2_18_0_31_33_1048,"set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_OV2655_1413_112_2_18_0_31_33_1049,"set_brightness: reg3391 = %d")
TRACE_MSG(SENSOR_OV2655_1437_112_2_18_0_31_33_1050,"set_brightness: level = %d")
TRACE_MSG(SENSOR_OV2655_1486_112_2_18_0_31_34_1051,"set_contrast: reg3391 = %d")
TRACE_MSG(SENSOR_OV2655_1503_112_2_18_0_31_34_1052,"set_contrast: level = %d")
TRACE_MSG(SENSOR_OV2655_1587_112_2_18_0_31_34_1053,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV2655_1596_112_2_18_0_31_34_1054,"SENSOR: OV2655_after_snapshot =%d")
TRACE_MSG(SENSOR_OV2655_1650_112_2_18_0_31_34_1055,"SENSOR: OV2655_InitExifInfo")
TRACE_MSG(SENSOR_OV2659_619_112_2_18_0_31_36_1056,"SENSOR: set_ov2659_flicker: 0x%x")
TRACE_MSG(SENSOR_OV2659_692_112_2_18_0_31_36_1057,"SENSOR: set_awb_mode: mode = %d,ret = %d")
TRACE_MSG(SENSOR_OV2659_727_112_2_18_0_31_36_1058,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_OV2659_792_112_2_18_0_31_36_1059,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV2659_850_112_2_18_0_31_36_1060,"OV2659_Identify: ret = %d")
TRACE_MSG(SENSOR_OV2659_907_112_2_18_0_31_36_1061,"OV2659Core_Get_ExposureValue()\r\n")
TRACE_MSG(SENSOR_OV2659_918_112_2_18_0_31_36_1062,"linetp = %ld\r\n")
TRACE_MSG(SENSOR_OV2659_931_112_2_18_0_31_36_1063,"shutter = %ld\r\n")
TRACE_MSG(SENSOR_OV2659_940_112_2_18_0_31_36_1064,"gain16 = %d\r\n")
TRACE_MSG(SENSOR_OV2659_949_112_2_18_0_31_36_1065,"m_dbExposure = %f\r\n")
TRACE_MSG(SENSOR_OV2659_961_112_2_18_0_31_36_1066,"OV2659Core_Set_ExposureValue()\r\n")
TRACE_MSG(SENSOR_OV2659_975_112_2_18_0_31_36_1067,"linetp = %d\r\n")
TRACE_MSG(SENSOR_OV2659_989_112_2_18_0_31_36_1068,"framelines = %d\r\n")
TRACE_MSG(SENSOR_OV2659_992_112_2_18_0_31_36_1069,"m_dbExposure = %lf\r\n")
TRACE_MSG(SENSOR_OV2659_996_112_2_18_0_31_36_1070,"exposuretp = %lf, before adjust\r\n")
TRACE_MSG(SENSOR_OV2659_997_112_2_18_0_31_37_1071,"exposuretp = %lf, after adjust, m_gain_preview=%x\r\n")
TRACE_MSG(SENSOR_OV2659_1018_112_2_18_0_31_37_1072,"bandinglines = %d\r\n")
TRACE_MSG(SENSOR_OV2659_1019_112_2_18_0_31_37_1073,"lightfreq = %d\r\n")
TRACE_MSG(SENSOR_OV2659_1023_112_2_18_0_31_37_1074,"smaller than 1 line\r\n")
TRACE_MSG(SENSOR_OV2659_1030_112_2_18_0_31_37_1075,"smaller than 1 banding, bandinglines=%d\r\n")
TRACE_MSG(SENSOR_OV2659_1036_112_2_18_0_31_37_1076,"smaller than 1 frame\r\n")
TRACE_MSG(SENSOR_OV2659_1043_112_2_18_0_31_37_1077,"larger than 1 frame\r\n")
TRACE_MSG(SENSOR_OV2659_1048_112_2_18_0_31_37_1078,"larger than maxim gain & frame\r\n")
TRACE_MSG(SENSOR_OV2659_1055_112_2_18_0_31_37_1079,"shutter = %ld\r\n")
TRACE_MSG(SENSOR_OV2659_1056_112_2_18_0_31_37_1080,"gain16 = %d\r\n")
TRACE_MSG(SENSOR_OV2659_1059_112_2_18_0_31_37_1081,"m_gain_still = %x\r\n")
TRACE_MSG(SENSOR_OV2659_1194_112_2_18_0_31_37_1082,"set_saturation: level = %d,ret = %d")
TRACE_MSG(SENSOR_OV2659_1287_112_2_18_0_31_37_1083,"set_image_effect: effect_type = %d,ret = %d")
TRACE_MSG(SENSOR_OV2659_1430_112_2_18_0_31_37_1084,"set_contrast: level = %d,ret = %d")
TRACE_MSG(SENSOR_OV2659_1489_112_2_18_0_31_38_1085,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV2659_1547_112_2_18_0_31_38_1086,"SENSOR: OV2659_InitExifInfo")
TRACE_MSG(SENSOR_OV3640_3797_112_2_18_0_31_46_1087,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV3640_3841_112_2_18_0_31_46_1088,"ov3640_Identify---------------\n")
TRACE_MSG(SENSOR_OV3640_3845_112_2_18_0_31_46_1089,"ov3640_Identify: PID = %x, ")
TRACE_MSG(SENSOR_OV3640_3848_112_2_18_0_31_46_1090,"ov3640_Identify: VER = %x, ")
TRACE_MSG(SENSOR_OV3640_3851_112_2_18_0_31_46_1091,"It's 36xx sensor.")
TRACE_MSG(SENSOR_OV3640_3854_112_2_18_0_31_46_1092,"ov3640_Identify: That is OV%x%x sensor !")
TRACE_MSG(SENSOR_OV3640_3902_112_2_18_0_31_46_1093,"SENSOR: OV3640_set_brightness = 0x%02x")
TRACE_MSG(SENSOR_OV3640_3948_112_2_18_0_31_46_1094,"SENSOR: OV3640_set_contrast = 0x%02x")
TRACE_MSG(SENSOR_OV3640_4003_112_2_18_0_31_46_1095,"SENSOR: OV3640_set_saturation = 0x%02x")
TRACE_MSG(SENSOR_OV3640_4060_112_2_18_0_31_47_1096,"SENSOR: OV3640_set_image_effect = 0x%02x")
TRACE_MSG(SENSOR_OV3640_4106_112_2_18_0_31_47_1097,"SENSOR: OV3640_set_ev = 0x%02x")
TRACE_MSG(SENSOR_OV3640_4135_112_2_18_0_31_47_1098,"SENSOR: OV3640_set_anti_flicker = 0x%02x")
TRACE_MSG(SENSOR_OV3640_4432_112_2_18_0_31_47_1099,"SENSOR: OV3640_set_video_mode = 0x%02x")
TRACE_MSG(SENSOR_OV3640_4486_112_2_18_0_31_47_1100,"SENSOR: OV3640_set_awb = 0x%02x")
TRACE_MSG(SENSOR_OV3640_4529_112_2_18_0_31_47_1101,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV3640_4633_112_2_18_0_31_48_1102,"ov3640: jpeg capture head: 0x%x, 0x%x")
TRACE_MSG(SENSOR_OV3640_4650_112_2_18_0_31_48_1103,"ov3640: Found the jpeg tail at %d: 0x%x 0x%x")
TRACE_MSG(SENSOR_OV3640_4653_112_2_18_0_31_48_1104,"ov3640: can not find the jpeg tail: %d")
TRACE_MSG(SENSOR_OV3640_4723_112_2_18_0_31_48_1105,"step1 reg0x3013 0x%x")
TRACE_MSG(SENSOR_OV3640_4726_112_2_18_0_31_48_1106,"step1 reg0x3013 0x%x")
TRACE_MSG(SENSOR_OV3640_4732_112_2_18_0_31_48_1107,"step2 reg0x3002 0x%x  0x3003=0x%x shutter=0x%x")
TRACE_MSG(SENSOR_OV3640_4739_112_2_18_0_31_48_1108,"step3 reg0x302d=0x%x; 0x302e=0x%x")
TRACE_MSG(SENSOR_OV3640_4740_112_2_18_0_31_48_1109,"step3 extra_lines=%d; preview_exposure=%d")
TRACE_MSG(SENSOR_OV3640_4745_112_2_18_0_31_48_1110,"step4 reg0x3001=0x%x Preview_Gain16=%d")
TRACE_MSG(SENSOR_OV3640_4752_112_2_18_0_31_48_1111,"step5 0x3028=0x%x reg0x3029=0x%x")
TRACE_MSG(SENSOR_OV3640_4753_112_2_18_0_31_48_1112,"step5 Preview_dummy_pixels=%d")
TRACE_MSG(SENSOR_OV3640_4763_112_2_18_0_31_48_1113,"step5 preview=%d  capture=%d")
TRACE_MSG(SENSOR_OV3640_4768_112_2_18_0_31_48_1114,"step6 capture_max_gain=%d  Capture_max_gain16=%d")
TRACE_MSG(SENSOR_OV3640_4773_112_2_18_0_31_48_1115,"step7 Preview_line_width=%d")
TRACE_MSG(SENSOR_OV3640_4776_112_2_18_0_31_48_1116,"step 7 Capture_line_width=%d")
TRACE_MSG(SENSOR_OV3640_4779_112_2_18_0_31_48_1117,"step8 Capture_Exposure=%d")
TRACE_MSG(SENSOR_OV3640_4785_112_2_18_0_31_48_1118,"step9 Capture_banding_Filter=%d")
TRACE_MSG(SENSOR_OV3640_4789_112_2_18_0_31_48_1119,"step9 max_shutter=%d")
TRACE_MSG(SENSOR_OV3640_4793_112_2_18_0_31_48_1120,"step10 Gain_Exposure=%d")
TRACE_MSG(SENSOR_OV3640_4798_112_2_18_0_31_48_1121,"step11 Capture_Exposure=%d  Capture_Gain16=%d")
TRACE_MSG(SENSOR_OV3640_4805_112_2_18_0_31_48_1122,"step12 Capture_Exposure=%d  Capture_Gain16=%d")
TRACE_MSG(SENSOR_OV3640_4814_112_2_18_0_31_48_1123,"step13 Capture_Exposure=%d  Capture_Gain16=%d")
TRACE_MSG(SENSOR_OV3640_4822_112_2_18_0_31_48_1124,"step14 Capture_Exposure=%d  Capture_Gain16=%d")
TRACE_MSG(SENSOR_OV3640_4842_112_2_18_0_31_48_1125,"step15 reg0x3029=0x%x")
TRACE_MSG(SENSOR_OV3640_4846_112_2_18_0_31_48_1126,"step16 reg0x3028=0x%x")
TRACE_MSG(SENSOR_OV3640_4856_112_2_18_0_31_48_1127,"step17 0x302a=0x%x  0x302b=0x%x")
TRACE_MSG(SENSOR_OV3640_4867_112_2_18_0_31_48_1128,"step18 shutter=%d  extra_lines=%d")
TRACE_MSG(SENSOR_OV3640_4873_112_2_18_0_31_48_1129,"step19 0x3003=0x%x  0x3002=0x%x")
TRACE_MSG(SENSOR_OV3640_4880_112_2_18_0_31_48_1130,"step19 0x302d=0x%x  0x302e=0x%x")
TRACE_MSG(SENSOR_OV3640_4909_112_2_18_0_31_48_1131,"step20 0x3001=0x%x")
TRACE_MSG(SENSOR_OV3640_4912_112_2_18_0_31_48_1132,"step20 0x3001=0x%x")
TRACE_MSG(SENSOR_OV5640_4427_112_2_18_0_31_57_1133,"SENSOR: _ov5640_InitExifInfo")
TRACE_MSG(SENSOR_OV5640_4544_112_2_18_0_31_57_1134,"SENSOR: _ov5640_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_OV5640_4571_112_2_18_0_31_57_1135,"SENSOR: ov5640_Identify: PID = %x, VER = %x")
TRACE_MSG(SENSOR_OV5640_4576_112_2_18_0_31_57_1136,"SENSOR: this is ov5640 sensor !")
TRACE_MSG(SENSOR_OV5640_4580_112_2_18_0_31_57_1137,"SENSOR: ov5640_Identify this is OV%x%x sensor !")
TRACE_MSG(SENSOR_OV5640_4631_112_2_18_0_31_58_1138,"SENSOR: _ov5640_set_brightness = 0x%02x")
TRACE_MSG(SENSOR_OV5640_4679_112_2_18_0_31_58_1139,"SENSOR: _ov5640_set_contrast = 0x%02x")
TRACE_MSG(SENSOR_OV5640_4734_112_2_18_0_31_58_1140,"SENSOR: _ov5640_set_saturation = 0x%02x")
TRACE_MSG(SENSOR_OV5640_4791_112_2_18_0_31_58_1141,"SENSOR: _ov5640_set_image_effect = 0x%02x")
TRACE_MSG(SENSOR_OV5640_4837_112_2_18_0_31_58_1142,"SENSOR: _ov5640_set_ev = 0x%02x")
TRACE_MSG(SENSOR_OV5640_4866_112_2_18_0_31_58_1143,"SENSOR: _ov5640_set_anti_flicker = 0x%02x")
TRACE_MSG(SENSOR_OV5640_5079_112_2_18_0_31_58_1144,"SENSOR: _ov5640_set_video_mode = 0x%02x")
TRACE_MSG(SENSOR_OV5640_5135_112_2_18_0_31_59_1145,"SENSOR: _ov5640_set_awb = 0x%02x")
TRACE_MSG(SENSOR_OV5640_5179_112_2_18_0_31_59_1146,"SENSOR: _ov5640_set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV5640_5303_112_2_18_0_31_59_1147,"SENSOR: ov5640 jpeg capture head: 0x%x, 0x%x")
TRACE_MSG(SENSOR_OV5640_5323_112_2_18_0_31_59_1148,"SENSOR: ov5640 Found the jpeg tail at %d: 0x%x 0x%x")
TRACE_MSG(SENSOR_OV5640_5327_112_2_18_0_31_59_1149,"SENSOR: ov5640 can not find the jpeg tail: %d")
TRACE_MSG(SENSOR_OV5640_5346_112_2_18_0_31_59_1150,"SENSOR: ov5640  chang_image_format  YUV422")
TRACE_MSG(SENSOR_OV5640_5351_112_2_18_0_31_59_1151,"SENSOR: ov5640  chang_image_format  jpg")
TRACE_MSG(SENSOR_OV5640_5369_112_2_18_0_31_59_1152,"SENSOR: _ov5640_after_snapshot =%d")
TRACE_MSG(SENSOR_OV5640_5392_112_2_18_0_31_59_1153,"SENSOR: ov5640_after_snapshot =%d")
TRACE_MSG(SENSOR_OV5640_5446_112_2_18_0_31_59_1154,"SENSOR: _ov5640_LoadAfFirmware i2c close error")
TRACE_MSG(SENSOR_OV5640_5451_112_2_18_0_31_59_1155,"SENSOR: _ov5640_LoadAfFirmware i2c close success")
TRACE_MSG(SENSOR_OV5640_5456_112_2_18_0_31_59_1156,"SENSOR: _ov5640_LoadAfFirmware i2c init error")
TRACE_MSG(SENSOR_OV5640_5519_112_2_18_0_31_59_1157,"SENSOR: _ov5640_MatchZone, w:%d, h:%d error")
TRACE_MSG(SENSOR_OV5640_5523_112_2_18_0_31_59_1158,"SENSOR: _ov5640_MatchZone, x:%d, y:%d")
TRACE_MSG(SENSOR_OV5640_5539_112_2_18_0_31_59_1159,"SENSOR: _ov5640_AutoFocusTrig")
TRACE_MSG(SENSOR_OV5640_5548_112_2_18_0_31_59_1160,"SENSOR: _ov5640_AutoFocusTrig error!")
TRACE_MSG(SENSOR_OV5640_5562_112_2_18_0_31_59_1161,"SENSOR: _ov5640_AutoFocusTrig fail!")
TRACE_MSG(SENSOR_OV5640_5582_112_2_18_0_31_59_1162,"SENSOR: _ov5640_AutoFocusZone: %d, %d")
TRACE_MSG(SENSOR_OV5640_5603_112_2_18_0_32_0_1163,"SENSOR: _ov5640_AutoFocusZone error!")
TRACE_MSG(SENSOR_OV5640_5627_112_2_18_0_32_0_1164,"SENSOR: _ov5640_StartAutoFocus param =%d")
TRACE_MSG(SENSOR_OV5640_5657_112_2_18_0_32_0_1165,"SENSOR: _ov5640_ExposureAuto")
TRACE_MSG(SENSOR_OV5640_5675_112_2_18_0_32_0_1166,"SENSOR: _ov5640_ExposureZone: %d, %d")
TRACE_MSG(SENSOR_OV5640_5711_112_2_18_0_32_0_1167,"SENSOR: _ov5640_ExposureZone: %d, %d, %d, %d")
TRACE_MSG(SENSOR_OV5640_5750_112_2_18_0_32_0_1168,"SENSOR: _ov5640_StartExposure param =%d")
TRACE_MSG(SENSOR_OV5640_5779_112_2_18_0_32_0_1169,"SENSOR: _ov5640_ExtFunc cmd:0x%x ")
TRACE_MSG(SENSOR_OV7670_703_112_2_18_0_32_2_1170,"SENSOR: OV7670_Identify: it is OV7670")
TRACE_MSG(SENSOR_OV7670_718_112_2_18_0_32_2_1171,"SENSOR: _ov7670_InitExifInfo")
TRACE_MSG(SENSOR_OV7670_851_112_2_18_0_32_2_1172,"SENSOR: _ov7670_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_OV7670_873_112_2_18_0_32_2_1173,"SENSOR: set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_OV7670_905_112_2_18_0_32_2_1174,"SENSOR: set_ae_awb_enable: ae=%d awb=%d")
TRACE_MSG(SENSOR_OV7670_915_112_2_18_0_32_2_1175,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV7670_924_112_2_18_0_32_2_1176,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV7670_954_112_2_18_0_32_2_1177,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_OV7670_985_112_2_18_0_32_2_1178,"SENSOR: set_banding_mode: mode = %d")
TRACE_MSG(SENSOR_OV7670_1070_112_2_18_0_32_2_1179,"SENSOR: set_video_mode: mode = %d")
TRACE_MSG(SENSOR_OV7670_1170_112_2_18_0_32_2_1180,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_OV7670_1204_112_2_18_0_32_3_1181,"SENSOR: set_brightness: level = %d")
TRACE_MSG(SENSOR_OV7670_1238_112_2_18_0_32_3_1182,"SENSOR: set_contrast: level = %d")
TRACE_MSG(SENSOR_OV7670_1266_112_2_18_0_32_3_1183,"SENSOR: set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV7670_1355_112_2_18_0_32_3_1184,"SENSOR: set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_OV7670_1363_112_2_18_0_32_3_1185,"SENSOR: OV7670_BeforeSnapshot: OV7670_After_Snapshot ")
TRACE_MSG(SENSOR_OV7670_1409_112_2_18_0_32_3_1186,"SENSOR: OV7670_BeforeSnapshot: OV7670_BeforeSnapshot ")
TRACE_MSG(SENSOR_OV7670_1421_112_2_18_0_32_3_1187,"unknown Camera mode!")
TRACE_MSG(SENSOR_OV7670_1424_112_2_18_0_32_3_1188,"OV7670_BeforeSnapshot max_line= %d")
TRACE_MSG(SENSOR_OV7670_1481_112_2_18_0_32_3_1189,"OV7670_BeforeSnapshot exposal= %d")
TRACE_MSG(SENSOR_OV7670_1620_112_2_18_0_32_3_1190,"SENSOR: set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV7670_1639_112_2_18_0_32_3_1191,"OV7670_set_ae_Rot: %d")
TRACE_MSG(SENSOR_OV7675_701_112_2_18_0_32_5_1192,"SENSOR: OV7675_Identify: it is OV7675")
TRACE_MSG(SENSOR_OV7675_714_112_2_18_0_32_5_1193,"SENSOR: _ov7675_InitExifInfo")
TRACE_MSG(SENSOR_OV7675_818_112_2_18_0_32_5_1194,"SENSOR: _ov7675_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_OV7675_841_112_2_18_0_32_5_1195,"SENSOR: set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_OV7675_873_112_2_18_0_32_5_1196,"SENSOR: set_ae_awb_enable: ae=%d awb=%d")
TRACE_MSG(SENSOR_OV7675_883_112_2_18_0_32_5_1197,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV7675_892_112_2_18_0_32_5_1198,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV7675_923_112_2_18_0_32_6_1199,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_OV7675_954_112_2_18_0_32_6_1200,"SENSOR: set_banding_mode: mode = %d")
TRACE_MSG(SENSOR_OV7675_1036_112_2_18_0_32_6_1201,"SENSOR: set_video_mode: mode = %d")
TRACE_MSG(SENSOR_OV7675_1136_112_2_18_0_32_6_1202,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_OV7675_1170_112_2_18_0_32_6_1203,"SENSOR: set_brightness: level = %d")
TRACE_MSG(SENSOR_OV7675_1203_112_2_18_0_32_6_1204,"SENSOR: set_contrast: level = %d")
TRACE_MSG(SENSOR_OV7675_1231_112_2_18_0_32_6_1205,"SENSOR: set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV7675_1320_112_2_18_0_32_6_1206,"SENSOR: set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_OV7675_1328_112_2_18_0_32_6_1207,"SENSOR: OV7675_BeforeSnapshot: OV7675_After_Snapshot ")
TRACE_MSG(SENSOR_OV7675_1374_112_2_18_0_32_6_1208,"SENSOR: OV7675_BeforeSnapshot: OV7675_BeforeSnapshot ")
TRACE_MSG(SENSOR_OV7675_1386_112_2_18_0_32_6_1209,"unknown Camera mode!")
TRACE_MSG(SENSOR_OV7675_1389_112_2_18_0_32_6_1210,"OV7675_BeforeSnapshot max_line= %d")
TRACE_MSG(SENSOR_OV7675_1440_112_2_18_0_32_7_1211,"OV7675_BeforeSnapshot exposal= %d")
TRACE_MSG(SENSOR_OV7675_1586_112_2_18_0_32_7_1212,"SENSOR: set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV7690_402_112_2_18_0_32_8_1213,"SENSOR: OV7690_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_OV7690_418_112_2_18_0_32_8_1214,"SENSOR: OV7690_ReadReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_OV7690_464_112_2_18_0_32_8_1215,"OV7690_Identify: it is OV7690")
TRACE_MSG(SENSOR_OV7690_500_112_2_18_0_32_8_1216,"SENSOR: _OV7690_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_OV7690_523_112_2_18_0_32_8_1217,"set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_OV7690_555_112_2_18_0_32_8_1218,"set_ae_awb_enable: ae=%d awb=%d")
TRACE_MSG(SENSOR_OV7690_567_112_2_18_0_32_8_1219,"set_OV7690_rot: %d")
TRACE_MSG(SENSOR_OV7690_596_112_2_18_0_32_8_1220,"set_OV7690_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV7690_651_112_2_18_0_32_8_1221,"set_OV7690_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV7690_720_112_2_18_0_32_8_1222,"OV7690_set_brightness: level = %d")
TRACE_MSG(SENSOR_OV7690_725_112_2_18_0_32_8_1223,"OV7690_set_brightness: 0x0c = %d")
TRACE_MSG(SENSOR_OV7690_850_112_2_18_0_32_9_1224,"OV7690_set_contrast: level = %d")
TRACE_MSG(SENSOR_OV7690_982_112_2_18_0_32_9_1225,"set_OV7690_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV7690_1220_112_2_18_0_32_9_1226,"OV7690_set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_OV7690_1230_112_2_18_0_32_9_1227,"OV7690_BeforeSnapshot: OV7690_After_Snapshot ")
TRACE_MSG(SENSOR_OV7690_1313_112_2_18_0_32_9_1228,"OV7690_set_work_mode: mode = %d")
TRACE_MSG(SENSOR_OV7690_1414_112_2_18_0_32_10_1229,"OV7690_set_whitebalance_mode: mode = %d")
TRACE_MSG(SENSOR_OV7690_1522_112_2_18_0_32_10_1230,"SENSOR: set_ov7690_video_mode: mode = %d")
TRACE_MSG(SENSOR_OV7690_1549_112_2_18_0_32_10_1231,"SENSOR: set_ov7690_ev: level = %d")
TRACE_MSG(SENSOR_OV9655_549_112_2_18_0_32_11_1232,"set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_OV9655_565_112_2_18_0_32_11_1233,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV9655_582_112_2_18_0_32_11_1234,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_OV9655_606_112_2_18_0_32_11_1235,"read_ae_value: %x")
TRACE_MSG(SENSOR_OV9655_616_112_2_18_0_32_11_1236,"write_ae_value: %x")
TRACE_MSG(SENSOR_OV9655_655_112_2_18_0_32_12_1237,"read_gain_value: %x")
TRACE_MSG(SENSOR_OV9655_665_112_2_18_0_32_12_1238,"write_gain_value: %x")
TRACE_MSG(SENSOR_OV9655_719_112_2_18_0_32_12_1239,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV9655_985_112_2_18_0_32_12_1240,"set_brightness: level = %d")
TRACE_MSG(SENSOR_OV9655_1004_112_2_18_0_32_12_1241,"set_contrast: level = %d")
TRACE_MSG(SENSOR_OV9655_1025_112_2_18_0_32_12_1242,"set_sharpness: level = %d")
TRACE_MSG(SENSOR_OV9655_1046_112_2_18_0_32_12_1243,"set_saturation: level = %d")
TRACE_MSG(SENSOR_OV9655_1066_112_2_18_0_32_12_1244,"-----------set_image_effect: effect_type = %d------------")
TRACE_MSG(SENSOR_OV9655_1075_112_2_18_0_32_12_1245,"set_frame_rate: param = %d")
TRACE_MSG(SENSOR_OV9655_1113_112_2_18_0_32_13_1246,"That is ov9655 sensor !")
TRACE_MSG(SENSOR_OV9655_1117_112_2_18_0_32_13_1247,"ov9655_Identify: That is OV%x%x sensor !")
TRACE_MSG(SENSOR_OV9655_1123_112_2_18_0_32_13_1248,"ov9655_Identify: PID = %x, VER = %x")
TRACE_MSG(SENSOR_OV9655_1142_112_2_18_0_32_13_1249,"SENSOR ov9655 : caculate_expo PREVIEW MODE %d")
TRACE_MSG(SENSOR_OV9655_1267_112_2_18_0_32_13_1250,"ov9655_BeforeSnapshot: param = %d")
TRACE_MSG(SENSOR_OV9660_542_112_2_18_0_32_14_1251,"OV9660_set_ae_enable: enable = %d")
TRACE_MSG(SENSOR_OV9660_581_112_2_18_0_32_14_1252,"SENSOR: set_OV9660_flicker: 0x%x")
TRACE_MSG(SENSOR_OV9660_673_112_2_18_0_32_14_1253,"SENSOR: set_vodeo_mode: %d ")
TRACE_MSG(SENSOR_OV9660_763_112_2_18_0_32_15_1254,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_OV9660_801_112_2_18_0_32_15_1255,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_OV9660_867_112_2_18_0_32_15_1256,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_OV9660_934_112_2_18_0_32_15_1257,"That is OV9660 sensor !")
TRACE_MSG(SENSOR_OV9660_938_112_2_18_0_32_15_1258,"OV9660_Identify: That is OV%x%x sensor !")
TRACE_MSG(SENSOR_OV9660_944_112_2_18_0_32_15_1259,"OV9660_Identify: PID = %x, VER = %x")
TRACE_MSG(SENSOR_OV9660_987_112_2_18_0_32_15_1260,"OV9660: prvGain %d, prvExp %d")
TRACE_MSG(SENSOR_OV9660_1020_112_2_18_0_32_15_1261,"OV9660: PrvGain %d, TgtExp %d")
TRACE_MSG(SENSOR_OV9660_1083_112_2_18_0_32_15_1262,"set_brightness: level = %d")
TRACE_MSG(SENSOR_OV9660_1114_112_2_18_0_32_15_1263,"set_contrast: level = %d")
TRACE_MSG(SENSOR_OV9660_1146_112_2_18_0_32_15_1264,"set_sharpness: level = %d")
TRACE_MSG(SENSOR_OV9660_1206_112_2_18_0_32_15_1265,"set_saturation: level = %d")
TRACE_MSG(SENSOR_OV9660_1276_112_2_18_0_32_16_1266,"set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_OV9660_1329_112_2_18_0_32_16_1267,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_S5K5CAGX_3878_112_2_18_0_32_27_1268,"SENSOR: _s5k5cagx_InitExifInfo")
TRACE_MSG(SENSOR_S5K5CAGX_3971_112_2_18_0_32_27_1269,"SENSOR: _s5k5cagx_PowerOn(1:on, 0:off): Entry")
TRACE_MSG(SENSOR_S5K5CAGX_3975_112_2_18_0_32_27_1270,"SENSOR: _s5k5cagx_PowerOn")
TRACE_MSG(SENSOR_S5K5CAGX_3978_112_2_18_0_32_27_1271,"SENSOR: _s5k5cagx_PowerOn dvdd/avdd/iovdd = {%d/%d/%d}")
TRACE_MSG(SENSOR_S5K5CAGX_3997_112_2_18_0_32_27_1272,"SENSOR: _s5k5cagx_PowerDown")
TRACE_MSG(SENSOR_S5K5CAGX_4011_112_2_18_0_32_27_1273,"SENSOR: _s5k5cagx_Power_On(1:on, 0:off): %d")
TRACE_MSG(SENSOR_S5K5CAGX_4012_112_2_18_0_32_27_1274,"SENSOR: _s5k5cagx_PowerOn(1:on, 0:off): End")
TRACE_MSG(SENSOR_S5K5CAGX_4036_112_2_18_0_32_27_1275,"s5k5cagx_Identify Entry")
TRACE_MSG(SENSOR_S5K5CAGX_4038_112_2_18_0_32_27_1276,"s5k5cagx_Identify 0xD0001006 PID = 0x%x")
TRACE_MSG(SENSOR_S5K5CAGX_4043_112_2_18_0_32_27_1277,"s5k5cagx_Identify Entry")
TRACE_MSG(SENSOR_S5K5CAGX_4045_112_2_18_0_32_27_1278,"s5k5cagx_Identify 0x00000040 PID = 0x%x")
TRACE_MSG(SENSOR_S5K5CAGX_4049_112_2_18_0_32_27_1279,"That is not s5k5cagx sensor ! error!")
TRACE_MSG(SENSOR_S5K5CAGX_4057_112_2_18_0_32_27_1280,"s5k5cagx_Identify: PID = %x, VER = %x")
TRACE_MSG(SENSOR_S5K5CAGX_4062_112_2_18_0_32_27_1281,"That is s5k5cagx sensor !")
TRACE_MSG(SENSOR_S5K5CAGX_4066_112_2_18_0_32_27_1282,"s5k5cagx_Identify: That is OV%x%x sensor !")
TRACE_MSG(SENSOR_S5K5CAGX_4071_112_2_18_0_32_27_1283,"That is s5k5cagx sensor !")
TRACE_MSG(SENSOR_S5K5CAGX_4072_112_2_18_0_32_27_1284,"s5k5cagx_Identify End")
TRACE_MSG(SENSOR_S5K5CAGX_4120_112_2_18_0_32_27_1285,"SENSOR: s5k5cagx_set_brightness = 0x%02x")
TRACE_MSG(SENSOR_S5K5CAGX_4168_112_2_18_0_32_28_1286,"SENSOR: s5k5cagx_set_contrast = 0x%02x")
TRACE_MSG(SENSOR_S5K5CAGX_4216_112_2_18_0_32_28_1287,"SENSOR: s5k5cagx_set_sharpness = 0x%02x")
TRACE_MSG(SENSOR_S5K5CAGX_4261_112_2_18_0_32_28_1288,"SENSOR: s5k5cagx_set_saturation = 0x%02x")
TRACE_MSG(SENSOR_S5K5CAGX_4422_112_2_18_0_32_28_1289,"SENSOR: s5k5cagx_set_image_effect = 0x%02x")
TRACE_MSG(SENSOR_S5K5CAGX_4469_112_2_18_0_32_28_1290,"SENSOR: s5k5cagx_set_ev = 0x%02x")
TRACE_MSG(SENSOR_S5K5CAGX_4553_112_2_18_0_32_28_1291,"SENSOR: s5k5cagx_set_awb = 0x%02x")
TRACE_MSG(SENSOR_S5K5CAGX_4617_112_2_18_0_32_28_1292,"SENSOR: set_work_mode: mode = %d")
TRACE_MSG(SENSOR_S5K5CAGX_4629_112_2_18_0_32_28_1293,"_s5k5cagx_BeforeSnapshot =%d")
TRACE_MSG(SENSOR_S5K5CAGX_4655_112_2_18_0_32_29_1294,"s5k5cagx only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d")
TRACE_MSG(SENSOR_S5K5CAGX_4674_112_2_18_0_32_29_1295,"s5k5cagx: jpeg capture head: 0x%x, 0x%x")
TRACE_MSG(SENSOR_S5K5CAGX_4695_112_2_18_0_32_29_1296,"s5k5cagx: Found the jpeg tail at %d: 0x%x 0x%x")
TRACE_MSG(SENSOR_S5K5CAGX_4699_112_2_18_0_32_29_1297,"s5k5cagx: can not find the jpeg tail: %d")
TRACE_MSG(SENSOR_S5K5CAGX_4719_112_2_18_0_32_29_1298,"s5k5cagx  chang_image_format  YUV422")
TRACE_MSG(SENSOR_S5K5CAGX_4724_112_2_18_0_32_29_1299,"s5k5cagx  chang_image_format  jpg")
TRACE_MSG(SENSOR_S5K5CAGX_4787_112_2_18_0_32_29_1300,"_s5k5cagx_after_snapshot =%d")
TRACE_MSG(SENSOR_S5K5CAGX_4817_112_2_18_0_32_29_1301,"SENSOR: _s5k5cagx_InitExt")
TRACE_MSG(SENSOR_S5K5CAGX_4832_112_2_18_0_32_29_1302,"SENSOR: _s5k5cagx_InitExt, i2c handler creat err !")
TRACE_MSG(SENSOR_S5K5CAGX_4885_112_2_18_0_32_29_1303,"SENSOR: _s5k5cagx_InitExt, i2c write once error")
TRACE_MSG(SENSOR_S5K5CAGX_4892_112_2_18_0_32_29_1304,"SENSOR: _s5k5cagx_InitExt, i2c write once from %d {0x%x 0x%x}, total %d registers {0x%x 0x%x},time %d")
TRACE_MSG(SENSOR_S5K5CAGX_4897_112_2_18_0_32_29_1305,"SENSOR: _s5k5cagx_InitExt, reg,{0x%x 0x%x} val {0x%x 0x%x} {0x%x 0x%x} {0x%x 0x%x}")
TRACE_MSG(SENSOR_S5K5CAGX_4907_112_2_18_0_32_29_1306,"SENSOR: _s5k5cagx_InitExt, i2c close error")
TRACE_MSG(SENSOR_S5K5CAGX_4912_112_2_18_0_32_29_1307,"SENSOR: _s5k5cagx_InitExt, i2c close success")
TRACE_MSG(SENSOR_S5K5CAGX_4915_112_2_18_0_32_29_1308,"SENSOR: _s5k5cagx_InitExt, success")
TRACE_MSG(SENSOR_S5K5CAGX_4932_112_2_18_0_32_29_1309,"SENSOR: _s5k5cagx_ExtFunc cmd:0x%x ")
TRACE_MSG(SENSOR_S5K5CAGX_4944_112_2_18_0_32_29_1310,"SENSOR: _s5k5cagx_ExtFunc unsupported command")
TRACE_MSG(SENSOR_S5KA3DFX_844_112_2_18_0_32_31_1311,"SENSOR: _s5ka3dfx_Power_On  power_on =0x%x")
TRACE_MSG(SENSOR_S5KA3DFX_896_112_2_18_0_32_31_1312,"SENSOR: S5KA3DFX_WriteReg 0x%x=0x%x")
TRACE_MSG(SENSOR_S5KA3DFX_911_112_2_18_0_32_31_1313,"SENSOR: S5KA3DFX_WriteReg 0x%x=0x%x")
TRACE_MSG(SENSOR_S5KA3DFX_923_112_2_18_0_32_31_1314,"S5KA3DFX_Identify: it is S5KA3DFX")
TRACE_MSG(SENSOR_S5KA3DFX_928_112_2_18_0_32_31_1315,"S5KA3DFX_Identify: it is S5KA3DFX 0x%x after read reg")
TRACE_MSG(SENSOR_S5KA3DFX_932_112_2_18_0_32_31_1316,"This is not S5KA3DFX sensor")
TRACE_MSG(SENSOR_S5KA3DFX_936_112_2_18_0_32_31_1317,"S5KA3DFX_Identify: it is S5KA3DFX")
TRACE_MSG(SENSOR_S5KA3DFX_955_112_2_18_0_32_31_1318,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_S5KA3DFX_967_112_2_18_0_32_31_1319,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1051_112_2_18_0_32_32_1320,"set_brightness: level = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1083_112_2_18_0_32_32_1321,"set_contrast: level = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1111_112_2_18_0_32_32_1322,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1227_112_2_18_0_32_32_1323,"set_image_effect: effect_type = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1338_112_2_18_0_32_32_1324,"s5ka3dfx_set_iso: iso_type = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1423_112_2_18_0_32_32_1325,"OV2640_set_wb_mode: wb_mode = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1457_112_2_18_0_32_32_1326,"s5ka3dfx_set_flicker_elimination_mode: flicker_mode = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1491_112_2_18_0_32_33_1327,"s5ka3dfx_ev_compensation: ev_mode = %d")
TRACE_MSG(SENSOR_S5KA3DFX_1530_112_2_18_0_32_33_1328,"s5ka3dfx_set_meter_mode: mode = %d, rect(%d,%d,%d,%d)")
TRACE_MSG(SENSOR_S5KA3DFX_1684_112_2_18_0_32_33_1329,"set_work_mode: mode = %d")
TRACE_MSG(SENSOR_SIV100A_204_112_2_18_0_32_33_1330,"SENSOR: SIV100A_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_SIV100A_247_112_2_18_0_32_33_1331,"SIV100A read ret is %X")
TRACE_MSG(SENSOR_SIV100A_254_112_2_18_0_32_33_1332,"SIV100A Fail")
TRACE_MSG(SENSOR_SIV100A_269_112_2_18_0_32_33_1333,"SIV100A succ")
TRACE_MSG(SENSOR_SIV100A_323_112_2_18_0_32_34_1334,"set_brightness: level = %d")
TRACE_MSG(SENSOR_SIV100A_379_112_2_18_0_32_34_1335,"set_contrast: level = %d")
TRACE_MSG(SENSOR_SIV100A_412_112_2_18_0_32_34_1336,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_SIV100A_476_112_2_18_0_32_34_1337,"-----------set_image_effect: effect_type = %d------------")
TRACE_MSG(SENSOR_SIV100B_206_112_2_18_0_32_35_1338,"SENSOR: SIV100B_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_SIV100B_219_112_2_18_0_32_35_1339,"SIV100B read ret is %X=%X")
TRACE_MSG(SENSOR_SIV100B_249_112_2_18_0_32_35_1340,"SIV100B read ret is %X")
TRACE_MSG(SENSOR_SIV100B_256_112_2_18_0_32_35_1341,"SIV100B Fail")
TRACE_MSG(SENSOR_SIV100B_271_112_2_18_0_32_35_1342,"SIV100B succ")
TRACE_MSG(SENSOR_SIV100B_339_112_2_18_0_32_35_1343,"set_brightness: level = %d")
TRACE_MSG(SENSOR_SIV100B_393_112_2_18_0_32_35_1344,"set_contrast: level = %d")
TRACE_MSG(SENSOR_SIV100B_491_112_2_18_0_32_35_1345,"set_contrast: level = %d")
TRACE_MSG(SENSOR_SIV100B_517_112_2_18_0_32_35_1346,"SIV100B: DCAMERA_WB_MODE_INCANDESCENCE")
TRACE_MSG(SENSOR_SIV100B_526_112_2_18_0_32_35_1347,"SIV100B: DCAMERA_WB_MODE_FLUORESCENT")
TRACE_MSG(SENSOR_SIV100B_535_112_2_18_0_32_35_1348,"SIV100B: DCAMERA_WB_MODE_SUN")
TRACE_MSG(SENSOR_SIV100B_545_112_2_18_0_32_35_1349,"SIV100B: DCAMERA_WB_MODE_CLOUD")
TRACE_MSG(SENSOR_SIV100B_555_112_2_18_0_32_35_1350,"SIV100B: DCAMERA_WB_MODE_AUTO")
TRACE_MSG(SENSOR_SIV100B_603_112_2_18_0_32_35_1351,"siv100b_set_video_mode 1")
TRACE_MSG(SENSOR_SIV100B_615_112_2_18_0_32_35_1352,"siv100b_set_video_mode 0")
TRACE_MSG(SENSOR_SIV100B_639_112_2_18_0_32_35_1353,"set_preview_mode 1")
TRACE_MSG(SENSOR_SIV100B_651_112_2_18_0_32_35_1354,"set_preview_mode 0")
TRACE_MSG(SENSOR_SIV100B_804_112_2_18_0_32_36_1355,"-----------set_image_effect: effect_type = %d------------")
TRACE_MSG(SENSOR_SIV120B_385_112_2_18_0_32_37_1356,"SENSOR: siv120b_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_SIV120B_421_112_2_18_0_32_37_1357,"siv120b_Identify")
TRACE_MSG(SENSOR_SIV120B_433_112_2_18_0_32_37_1358,"siv120b read ret is %X")
TRACE_MSG(SENSOR_SIV120B_440_112_2_18_0_32_37_1359,"siv120b Fail")
TRACE_MSG(SENSOR_SIV120B_455_112_2_18_0_32_37_1360,"siv120b succ")
TRACE_MSG(SENSOR_SIV120B_531_112_2_18_0_32_37_1361,"set_brightness: level = %d")
TRACE_MSG(SENSOR_SIV120B_595_112_2_18_0_32_37_1362,"set_contrast: level = %d")
TRACE_MSG(SENSOR_SIV120B_693_112_2_18_0_32_38_1363,"set_ev: level = %d")
TRACE_MSG(SENSOR_SIV120B_741_112_2_18_0_32_38_1364,"siv120b: DCAMERA_WB_MODE_INCANDESCENCE")
TRACE_MSG(SENSOR_SIV120B_750_112_2_18_0_32_38_1365,"siv120b: DCAMERA_WB_MODE_FLUORESCENT")
TRACE_MSG(SENSOR_SIV120B_759_112_2_18_0_32_38_1366,"siv120b: DCAMERA_WB_MODE_SUN")
TRACE_MSG(SENSOR_SIV120B_769_112_2_18_0_32_38_1367,"siv120b: DCAMERA_WB_MODE_CLOUD")
TRACE_MSG(SENSOR_SIV120B_780_112_2_18_0_32_38_1368,"siv120b: DCAMERA_WB_MODE_AUTO")
TRACE_MSG(SENSOR_SIV120B_1022_112_2_18_0_32_38_1369,"-----------set_image_effect: effect_type = %d------------")
TRACE_MSG(SENSOR_SIV120B_1079_112_2_18_0_32_39_1370,"siv120b_before_snapshot")
TRACE_MSG(SENSOR_SIV120B_1210_112_2_18_0_32_39_1371," siv120b_before_snapshot 0x2C = %0X")
TRACE_MSG(SENSOR_SIV120B_1211_112_2_18_0_32_39_1372," siv120b_before_snapshot 0x2D = %0X")
TRACE_MSG(SENSOR_SIV120B_1212_112_2_18_0_32_39_1373," siv120b_before_snapshot 0x2E = %0X")
TRACE_MSG(SENSOR_SIV120B_1213_112_2_18_0_32_39_1374," siv120b_before_snapshot 0x2F = %0X")
TRACE_MSG(SENSOR_SP0838_469_112_2_18_0_32_40_1375,"SENSOR: SP0838_WriteReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_SP0838_485_112_2_18_0_32_40_1376,"SENSOR: SP0838_ReadReg reg/value(%x,%x) !!")
TRACE_MSG(SENSOR_SP0838_506_112_2_18_0_32_40_1377,"SP0838_Identify")
TRACE_MSG(SENSOR_SP0838_516_112_2_18_0_32_40_1378,"It is not SP0838")
TRACE_MSG(SENSOR_SP0838_531_112_2_18_0_32_40_1379,"SP0838_Identify: it is SP0838")
TRACE_MSG(SENSOR_SP0838_544_112_2_18_0_32_40_1380,"SP0838_Identify: ret = %d")
TRACE_MSG(SENSOR_SP0838_552_112_2_18_0_32_40_1381,"set_SP0838_ae_enable: enable = %d")
TRACE_MSG(SENSOR_SP0838_560_112_2_18_0_32_40_1382,"set_hmirror_enable: enable = %d")
TRACE_MSG(SENSOR_SP0838_569_112_2_18_0_32_40_1383,"set_vmirror_enable: enable = %d")
TRACE_MSG(SENSOR_SP0838_644_112_2_18_0_32_41_1384,"set_brightness: level = %d")
TRACE_MSG(SENSOR_SP0838_724_112_2_18_0_32_41_1385,"SENSOR: set_ev: level = %d")
TRACE_MSG(SENSOR_SP0838_1119_112_2_18_0_32_42_1386,"SENSOR: set_awb_mode: mode = %d")
TRACE_MSG(SENSOR_SP0838_1193_112_2_18_0_32_42_1387,"set_contrast: level = %d")
TRACE_MSG(SENSOR_SP0838_1220_112_2_18_0_32_42_1388,"set_preview_mode: preview_mode = %d")
TRACE_MSG(SENSOR_SP0838_1477_112_2_18_0_32_42_1389,"-----------set_image_effect: effect_type = %d------------")
TRACE_MSG(SENSOR_SP0838_1504_112_2_18_0_32_42_1390,"SP0838_BeforeSnapshot ")
TRACE_MSG(SENSOR_SP0838_1632_112_2_18_0_32_43_1391,"set_work_mode: mode = %d")
TRACE_MSG(FM_DRV_108_112_2_18_0_32_43_1392,"FM_SetStatus: status = %d, s_fm_status = %d")
TRACE_MSG(FM_DRV_150_112_2_18_0_32_44_1393,"FM_Entersleep")
TRACE_MSG(FM_DRV_179_112_2_18_0_32_44_1394,"FM_Init")
TRACE_MSG(FM_DRV_184_112_2_18_0_32_44_1395,"FM_Init: Don't need to Initailize again!!!")
TRACE_MSG(FM_DRV_218_112_2_18_0_32_44_1396,"FM_Init2: Fail to init FM!!")
TRACE_MSG(FM_DRV_237_112_2_18_0_32_44_1397,"FM_Init1: Fail to init FM!!")
TRACE_MSG(FM_DRV_253_112_2_18_0_32_44_1398,"FM_Open")
TRACE_MSG(FM_DRV_256_112_2_18_0_32_44_1399,"FM not init !!")
TRACE_MSG(FM_DRV_276_112_2_18_0_32_44_1400,"FM_Start: freq = %d")
TRACE_MSG(FM_DRV_280_112_2_18_0_32_44_1401,"FM not init !!")
TRACE_MSG(FM_DRV_300_112_2_18_0_32_44_1402,"FM_PlayStop")
TRACE_MSG(FM_DRV_303_112_2_18_0_32_44_1403,"FM_PlayStop: not init, Don't need to stop")
TRACE_MSG(FM_DRV_320_112_2_18_0_32_44_1404,"FM_Close")
TRACE_MSG(FM_DRV_325_112_2_18_0_32_44_1405,"FM_Close: not init, Don't need to Close")
TRACE_MSG(FM_DRV_353_112_2_18_0_32_44_1406,"FM_ManualSeek: freq = %d")
TRACE_MSG(FM_DRV_357_112_2_18_0_32_44_1407,"FM not init !!")
TRACE_MSG(FM_DRV_378_112_2_18_0_32_44_1408,"FM_AutoSeek: seek_dir = %d, mode = %d, freq = %d")
TRACE_MSG(FM_DRV_382_112_2_18_0_32_44_1409,"FM not init !!")
TRACE_MSG(FM_DRV_400_112_2_18_0_32_44_1410,"FM_SetScanLevel: level = %d")
TRACE_MSG(FM_DRV_403_112_2_18_0_32_44_1411,"FM not init !!")
TRACE_MSG(FM_DRV_435_112_2_18_0_32_44_1412,"FM not init !!")
TRACE_MSG(FM_CL6012X_145_112_2_18_0_32_45_1413,"[FM_DRV]CL6012X Reg[%d]= 0x%02x")
TRACE_MSG(FM_CL6012X_157_112_2_18_0_32_45_1414,"CL6012X_Config")
TRACE_MSG(FM_CL6012X_188_112_2_18_0_32_45_1415,"----CL6012X_Initial----")
TRACE_MSG(FM_CL6012X_194_112_2_18_0_32_45_1416,"FM::==> open IIC failed!\r\n")
TRACE_MSG(FM_CL6012X_252_112_2_18_0_32_45_1417,"CL6012X_PlayStop()")
TRACE_MSG(FM_CL6012X_279_112_2_18_0_32_45_1418,"CL6012X_Close()")
TRACE_MSG(FM_CL6012X_342_112_2_18_0_32_46_1419,"CL6012X_ManualSeek enter:freq = %d")
TRACE_MSG(FM_CL6012X_360_112_2_18_0_32_46_1420,"[FM_DRV]CL6012X_ManualSeek: freq = %d, signal_strength = %d, ")
TRACE_MSG(FM_CL6012X_449_112_2_18_0_32_46_1421,"[FM_DRV]CL6012X_AutoSeek: stc= %d, sff= %d, ")
TRACE_MSG(FM_CL6012X_482_112_2_18_0_32_46_1422,"--CL6012X_SetVol:vol_level=%d---")
TRACE_MSG(FM_KT0812G_128_112_2_18_0_32_47_1423,"[DRV_FM]==> open IIC failed!\r\n")
TRACE_MSG(FM_KT0812G_160_112_2_18_0_32_47_1424,"[DRV_FM] _Write_One_Reg: [0x%02X] = 0x%04X ")
TRACE_MSG(FM_KT0812G_180_112_2_18_0_32_47_1425,"[DRV_FM] _Read_One_Reg: [0x%02X] = 0x%04X ")
TRACE_MSG(FM_KT0812G_199_112_2_18_0_32_47_1426,"[DRV_FM] KT0812G_ReadAllReg: [0x%02X]=0x%04X ")
TRACE_MSG(FM_KT0812G_217_112_2_18_0_32_47_1427,"[DRV_FM] KT0812G_Initial: i2c port = %d")
TRACE_MSG(FM_KT0812G_243_112_2_18_0_32_47_1428,"[DRV_FM] KT0812G_FMTune: STATUSA = 0x%04X, i = %d")
TRACE_MSG(FM_KT0812G_280_112_2_18_0_32_47_1429,"[DRV_FM] KT0812G_Open")
TRACE_MSG(FM_KT0812G_293_112_2_18_0_32_47_1430,"[DRV_FM] KT0812G_Close")
TRACE_MSG(FM_KT0812G_329_112_2_18_0_32_47_1431,"[DRV_FM] KT0812G_FMTune: at TUNE = 0x%04X ")
TRACE_MSG(FM_KT0812G_334_112_2_18_0_32_47_1432,"[DRV_FM] KT0812G_FMTune: After TUNE = 0x%04X ")
TRACE_MSG(FM_KT0812G_335_112_2_18_0_32_47_1433,"[DRV_FM] KT0812G_FMTune: STATUSA = 0x%04X ")
TRACE_MSG(FM_KT0812G_336_112_2_18_0_32_47_1434,"[DRV_FM] KT0812G_FMTune: READ CHAN= 0x%04X ")
TRACE_MSG(FM_KT0812G_354_112_2_18_0_32_47_1435,"[DRV_FM] KT0812G_GetTuneFreq: CHAN= 0x%04X, Freq = %d ")
TRACE_MSG(FM_KT0812G_411_112_2_18_0_32_47_1436,"[DRV_FM] KT0812G_PlayStart: Freq = %d ")
TRACE_MSG(FM_KT0812G_426_112_2_18_0_32_47_1437,"[DRV_FM] KT0812G_PlayStop")
TRACE_MSG(FM_KT0812G_453_112_2_18_0_32_47_1438,"[DRV_FM]KT0812G_ManualSeek: freq = %d, rssi = %d, stereo = %d")
TRACE_MSG(FM_KT0812G_519_112_2_18_0_32_47_1439,"[DRV_FM]KT0812G_AutoSeek: seek_dir = %d, seek_end= %d, seek_fail= %d, ")
TRACE_MSG(FM_KT0812G_536_112_2_18_0_32_47_1440,"[DRV_FM] KT0812G_SetScanLevel")
TRACE_MSG(FM_KT0812G_571_112_2_18_0_32_47_1441,"[DRV_FM] KT0812G_SetVol = %d, s_cur_vol = %d, RegVol = %02X")
TRACE_MSG(FM_KT0812G_588_112_2_18_0_32_47_1442,"[DRV_FM] KT0812G_SetFMRegion")
TRACE_MSG(FM_KT0812G_598_112_2_18_0_32_47_1443,"[DRV_FM] KT0812G_SetStereo = %d")
TRACE_MSG(FM_LV2400X_273_112_2_18_0_32_48_1444,"FM: SetScanLevel %d")
TRACE_MSG(LV2400X_225_112_2_18_0_32_49_1445,"Set_IF_Freq 0x%x,g_wLastMsr=0X%x\n")
TRACE_MSG(LV2400X_229_112_2_18_0_32_49_1446,"InitTuningRfCapOsc 0x%x\n")
TRACE_MSG(LV2400X_234_112_2_18_0_32_49_1447,"SetRegion 0x%x\n")
TRACE_MSG(LV2400X_238_112_2_18_0_32_49_1448,"Set_SD_Freq 0x%x,g_wLastMsr=0x%x\n")
TRACE_MSG(LV2400X_286_112_2_18_0_32_49_1449,"Set_IF_Freq 0x%x,g_wLastMsr=0X%x\n")
TRACE_MSG(LV2400X_291_112_2_18_0_32_49_1450,"InitTuningRfCapOsc 0x%x\n")
TRACE_MSG(LV2400X_296_112_2_18_0_32_49_1451,"SetRegion 0x%x\n")
TRACE_MSG(LV2400X_300_112_2_18_0_32_49_1452,"Set_SD_Freq 0x%x,g_wLastMsr=0x%x\n")
TRACE_MSG(LV2400X_305_112_2_18_0_32_49_1453,"InitLv2400xChip: chip id -> %x")
TRACE_MSG(LV2400X_2012_112_2_18_0_32_53_1454,"chip id1 = 0x%x")
TRACE_MSG(LV2400X_2035_112_2_18_0_32_53_1455,"chip id = 0x%x")
TRACE_MSG(RADIOSUB_352_112_2_18_0_32_55_1456,"scan set freq error = %d\r\n")
TRACE_MSG(RADIOSUB_361_112_2_18_0_32_55_1457,"scan set freq ok = %d\r\n")
TRACE_MSG(RADIOSUB_460_112_2_18_0_32_55_1458,"field strength = %d\r\n")
TRACE_MSG(RADIOSUB_461_112_2_18_0_32_55_1459,"g_wLastMsrRF = %d\r\n")
TRACE_MSG(TEA5761_135_112_2_18_0_32_55_1460,"TEA5761_Calculate_Frequency()  return=%d")
TRACE_MSG(TEA5761_147_112_2_18_0_32_55_1461,"    TEA5761_Calculate_PLL_Value(freq=%d)  return = 0x%04x    ")
TRACE_MSG(TEA5761_208_112_2_18_0_32_55_1462,"---TEA5761_WaitLDSet(): time out error---")
TRACE_MSG(TEA5761_211_112_2_18_0_32_55_1463,"---TEA5761_WaitLDSet(): return ok---")
TRACE_MSG(TEA5761_219_112_2_18_0_32_55_1464,"----TEA5761_CorrectFreq: freq=%d----")
TRACE_MSG(TEA5761_233_112_2_18_0_32_56_1465,"--TEA5761_CorrectFreq(return freq=%d)---")
TRACE_MSG(TEA5761_286_112_2_18_0_32_56_1466,"----TEA5761_Preset(%d)----")
TRACE_MSG(TEA5761_356_112_2_18_0_32_56_1467,"--if_count=%d----")
TRACE_MSG(TEA5761_394_112_2_18_0_32_56_1468,"----TEA5761_Initial(freq=%d)----")
TRACE_MSG(TEA5761_433_112_2_18_0_32_56_1469,"---Tea5761_search(%d)---")
TRACE_MSG(TEA5761_466_112_2_18_0_32_56_1470,"---if_count[1]=%d----")
TRACE_MSG(TEA5761_477_112_2_18_0_32_56_1471,"---if_count[2]=%d----")
TRACE_MSG(TEA5761_499_112_2_18_0_32_56_1472,"---Tea5761_Search...return----")
TRACE_MSG(TEA5761_505_112_2_18_0_32_56_1473,"TEA5761_palystart()")
TRACE_MSG(TEA5761_517_112_2_18_0_32_56_1474,"TEA5761_palystop()")
TRACE_MSG(TEA5761_530_112_2_18_0_32_56_1475,"TEA5761_Open()")
TRACE_MSG(TEA5761_537_112_2_18_0_32_56_1476,"TEA5761_close()")
TRACE_MSG(TUNEALG_308_112_2_18_0_32_57_1477,"SetUpChipMode() return 0x%x\n")
TRACE_MSG(TUNEALG_312_112_2_18_0_32_57_1478,"GetRfAtCapOsc() g_wHwRfLow 0x%x\n")
TRACE_MSG(TUNEALG_315_112_2_18_0_32_57_1479,"GetRfAtCapOsc() g_wHwRfHigh 0x%x\n")
TRACE_MSG(TUNEALG_327_112_2_18_0_32_57_1480,"Qssf1.CoefLo 0x%x,Qssf1.CoefHi 0x%x\n")
TRACE_MSG(TUNEALG_328_112_2_18_0_32_57_1481,"Qssf2.CoefLo 0x%x,Qssf2.CoefHi 0x%x\n")
TRACE_MSG(GPIO_EXT_DRV_189_112_2_18_0_32_58_1482,"_GPIO_EXT_ReadReg: ic %d hasn't existed")
TRACE_MSG(GPIO_EXT_DRV_218_112_2_18_0_32_58_1483,"_GPIO_EXT_WriteReg: ic %d hasn't existed")
TRACE_MSG(GPIO_EXT_DRV_232_112_2_18_0_32_58_1484,"_GPIO_EXT_UpdateInputValue: ic_num = %d")
TRACE_MSG(GPIO_EXT_DRV_236_112_2_18_0_32_58_1485,"port0 = %02x, port1 = %02x")
TRACE_MSG(GPIO_EXT_DRV_268_112_2_18_0_32_58_1486,"GPIO_EXT_Init: Extend gpio diasble")
TRACE_MSG(GPIO_EXT_DRV_274_112_2_18_0_32_58_1487,"GPIO_EXT_Init: Extend gpio enable")
TRACE_MSG(GPIO_EXT_DRV_284_112_2_18_0_32_58_1488,"GPIO EXT: GPIO_TOTAL_CNT = % d, GPIO_EXT_DEV_MAX_CNT = %d, gpio_cnt_in_one_ic = %d, s_gpio_ext_int_pin = %d")
TRACE_MSG(GPIO_EXT_DRV_289_112_2_18_0_32_58_1489,"working-gpio table is over !!!")
TRACE_MSG(GPIO_EXT_DRV_319_112_2_18_0_32_58_1490,"EXT GPIO: working table %d line has been used !!")
TRACE_MSG(GPIO_EXT_DRV_324_112_2_18_0_32_58_1491,"EXT GPIO : cus cfg table has not end flag, or larger than working table!!")
TRACE_MSG(GPIO_EXT_DRV_369_112_2_18_0_32_58_1492,"GPIO_EXT_Init: Start time = %d")
TRACE_MSG(GPIO_EXT_DRV_383_112_2_18_0_32_59_1493,"----- ic -- port -- gpio -- dir -- value -- callback addr")
TRACE_MSG(GPIO_EXT_DRV_405_112_2_18_0_32_59_1494,"      %02d  -- %02d -- %02d -- %d  -- %d -- %08x")
TRACE_MSG(GPIO_EXT_DRV_416_112_2_18_0_32_59_1495," *    %02d  -- %02d -- %02d -- %d  -- %d -- %08x")
TRACE_MSG(GPIO_EXT_DRV_422_112_2_18_0_32_59_1496,"GPIO_EXT_Init: ERR ic_num %d, gpio_num %d")
TRACE_MSG(GPIO_EXT_DRV_466_112_2_18_0_32_59_1497,"------------------------- END -------------------------")
TRACE_MSG(GPIO_EXT_DRV_501_112_2_18_0_32_59_1498,"GPIO_EXT_Init: End time = %d")
TRACE_MSG(GPIO_EXT_DRV_512_112_2_18_0_32_59_1499,"GPIO_EXT_IsInit: Not Init!!")
TRACE_MSG(GPIO_EXT_DRV_553_112_2_18_0_32_59_1500,"GPIO_EXT_DeShaking: Start Time = %d")
TRACE_MSG(GPIO_EXT_DRV_583_112_2_18_0_32_59_1501,"GPIO_EXT_DeShaking: ( %02d - %02d - %02x )")
TRACE_MSG(GPIO_EXT_DRV_599_112_2_18_0_32_59_1502,"GPIO_EXT_DeShaking: gpio_num = %04x")
TRACE_MSG(GPIO_EXT_DRV_618_112_2_18_0_32_59_1503,"GPIO_EXT_DeShaking: End Time = %d")
TRACE_MSG(GPIO_EXT_DRV_640_112_2_18_0_32_59_1504,"GPIO_EXT_SetDirection: gpio_num = %02x, value = %d")
TRACE_MSG(GPIO_EXT_DRV_686_112_2_18_0_32_59_1505,"GPIO_EXT_SetValue: Don't need to Set extend gpio(%02x) direction(%02x)")
TRACE_MSG(GPIO_EXT_DRV_717_112_2_18_0_32_59_1506,"[DRV_GPIO_EXT_DRV]<ERROR> ic > GPIO_EXT_DEV_MAX_CNT\r\n")
TRACE_MSG(GPIO_EXT_DRV_843_112_2_18_0_32_59_1507,"GPIO_EXT_GetValue: gpio_num = %04x")
TRACE_MSG(GPIO_EXT_DRV_883_112_2_18_0_32_59_1508,"GPIO_EXT_GetValue: gpio_num = %02x, value = %d")
TRACE_MSG(GPIO_EXT_DRV_922_112_2_18_0_33_0_1509,"GPIO_EXT_SetPortValue:(before setting) all_gpio_value %0x")
TRACE_MSG(GPIO_EXT_DRV_949_112_2_18_0_33_0_1510,"GPIO_EXT_SetPortValue:(after setting) all_gpio_value %0x")
TRACE_MSG(GPIO_EXT_DRV_997_112_2_18_0_33_0_1511,"extend GPIO %04x be used")
TRACE_MSG(GPIO_EXT_DRV_1024_112_2_18_0_33_0_1512,"[DRV_GPIO_EXT_DRV]<WARNING> ic > GPIO_EXT_DEV_MAX_CNT\r\n")
TRACE_MSG(GPIO_EXT_DRV_1036_112_2_18_0_33_0_1513,"[DRV_GPIO_EXT_DRV]<ERROR> func:GPIO_EXT_RegGpio() ic > GPIO_EXT_DEV_MAX_CNT\r\n")
TRACE_MSG(GPIO_EXT_DRV_1073_112_2_18_0_33_0_1514,"GPIO_EXT_UnRegGpio: The extend GPIO is not used!!")
TRACE_MSG(GPIO_EXT_DRV_1102_112_2_18_0_33_0_1515,"GPIO_EXT_Control_Entry: start time = %d")
TRACE_MSG(GPIO_EXT_DRV_1107_112_2_18_0_33_0_1516,"GPIO_EXT_Control_Entry: end time = %d")
TRACE_MSG(GPIO_EXT_DRV_1135_112_2_18_0_33_0_1517,"GPIO_EXT_task_init")
TRACE_MSG(GPIO_EXT_DRV_1154_112_2_18_0_33_0_1518,"GPIO_EXT_ISR_3_12_Test: gpio_ext_state = %d")
TRACE_MSG(GPIO_EXT_DRV_1158_112_2_18_0_33_0_1519,"GPIO_EXT_Test")
TRACE_MSG(GPIO_EXT_DRV_1165_112_2_18_0_33_0_1520,"GPIO_EXT_Test: g_function = %d")
TRACE_MSG(GPIO_PROD_461_112_2_18_0_33_1_1521,"[DBAT]GPIO_DBAT_AuxBatDetectIntHandler: Aux battery plug OUT !")
TRACE_MSG(GPIO_PROD_467_112_2_18_0_33_1_1522,"[DBAT]GPIO_DBAT_AuxBatDetectIntHandler: Aux battery plug IN !")
TRACE_MSG(GPIO_PROD_485_112_2_18_0_33_1_1523,"[DBAT]GPIO_DBAT_MainBatDetectIntHandler: Main battery plug OUT !")
TRACE_MSG(GPIO_PROD_491_112_2_18_0_33_1_1524,"[DBAT]GPIO_DBAT_MainBatDetectIntHandler: Main battery plug IN !")
TRACE_MSG(GPIO_PROD_935_112_2_18_0_33_2_1525,"GPIO_WiFiIrqHander()")
TRACE_MSG(GPIO_PROD_1122_112_2_18_0_33_3_1526,"GPIO_PROD_RegGpio - INT type error!")
TRACE_MSG(GPIO_PROD_1173_112_2_18_0_33_3_1527,"_GPIO_PROD_RegGpio : type %d - invalid")
TRACE_MSG(GPIO_PROD_1190_112_2_18_0_33_3_1528,"_GPIO_PROD_InitCfgTable")
TRACE_MSG(GPIO_PROD_1279_112_2_18_0_33_3_1529,"GPIO_PROD_SetCfgInfo: Gpio prod id %d be Modified")
TRACE_MSG(GPIO_PROD_1283_112_2_18_0_33_3_1530,"GPIO_PROD_SetCfgInfo: Gpio prod id %d Create")
TRACE_MSG(GPIO_PROD_1299_112_2_18_0_33_3_1531,"GPIO_SetKeyPadBackLight: %d")
TRACE_MSG(GPIO_PROD_1313_112_2_18_0_33_3_1532,"GPIO_SetLcdBackLight: %d")
TRACE_MSG(GPIO_PROD_1350_112_2_18_0_33_3_1533,"GPIO_SetBackLight: %d")
TRACE_MSG(GPIO_PROD_1364_112_2_18_0_33_3_1534,"GPIO_SetVibrator: %d")
TRACE_MSG(GPIO_PROD_1390_112_2_18_0_33_3_1535,"GPIO_SetVibrator_Ext: on = %d, period = %d, stress = %d")
TRACE_MSG(GPIO_PROD_1435_112_2_18_0_33_3_1536,"GPIO_GetFlipState: %d")
TRACE_MSG(GPIO_PROD_1455_112_2_18_0_33_3_1537,"GPIO_CheckHeadsetStatus: %d")
TRACE_MSG(GPIO_PROD_1473_112_2_18_0_33_3_1538,"GPIO_CheckSDCardStatus: %d")
TRACE_MSG(GPIO_PROD_1718_112_2_18_0_33_4_1539,"GPIO_SelectSPI2CS: cs_num = %d")
TRACE_MSG(GPIO_PROD_1812_112_2_18_0_33_4_1540,"GPIO_WiFiIrqHander()")
TRACE_MSG(GPIO_PROD_1997_112_2_18_0_33_5_1541,"GPIO_SIMIntHandler %d %d. %d %d %d %d")
TRACE_MSG(GPIO_PROD_2113_112_2_18_0_33_5_1542,"GPIO_HookKeyIntHandler gpio_state(%d)")
TRACE_MSG(GPIO_PROD_2145_112_2_18_0_33_5_1543,"GPIO_EnableHookInt.........%d....")
TRACE_MSG(GPIO_PROD_2155_112_2_18_0_33_5_1544,"GPIO_EnableHandFreeILed.........%d....")
TRACE_MSG(GPIO_PROD_2250_112_2_18_0_33_5_1545,"GPIO_SetSPISwitch: WIFI ON")
TRACE_MSG(GPIO_PROD_2257_112_2_18_0_33_5_1546,"GPIO_SetSPISwitch: CMMB ON")
TRACE_MSG(GPIO_PROD_2264_112_2_18_0_33_5_1547,"GPIO_SetSPISwitch: SWITCH DEFAULT")
TRACE_MSG(GPIO_PROD_2271_112_2_18_0_33_5_1548,"GPIO_SetSPISwitch: PARAMETERS ERROR")
TRACE_MSG(GPS_COM_351_112_2_18_0_33_6_1549,"\r\nGPS_ComInit.BaudRate=%d")
TRACE_MSG(GPS_COM_423_112_2_18_0_33_6_1550,"\r\nGPS_ComClose.")
TRACE_MSG(GPS_DIAG_78_112_2_18_0_33_6_1551,"_GPS_DIAG_CmdRoutine() begin: cmd=%d, num=%d, val_list=%s")
TRACE_MSG(GPS_DIAG_159_112_2_18_0_33_7_1552,"GPSCalCmd end: cmd=%d, num=%d, val_list=%s")
TRACE_MSG(GPS_DIAG_172_112_2_18_0_33_7_1553,"GPS: GPS_DIAG_RegDiagCmdRoutine")
TRACE_MSG(GPS_DRV_171_112_2_18_0_33_7_1554,"_GPS_ValidationCallback: high_cipher 0x%08x, low_cipher 0x%08x")
TRACE_MSG(GPS_DRV_192_112_2_18_0_33_7_1555,"_GPS_SetMode: s_gps_mode %d, mode %d")
TRACE_MSG(GPS_DRV_255_112_2_18_0_33_7_1556,"GPS_Init: Identify fail !! status %d")
TRACE_MSG(GPS_DRV_261_112_2_18_0_33_7_1557,"GPS_Init: Identify successful !!")
TRACE_MSG(GPS_DRV_509_112_2_18_0_33_8_1558,"GPS_GetValidationCipher: input_serial 0x%08x, start time %d")
TRACE_MSG(GPS_DRV_525_112_2_18_0_33_8_1559,"GPS_GetValidationCipher OK  end time %d!")
TRACE_MSG(GPS_DRV_532_112_2_18_0_33_8_1560,"GPS_GetValidationCipher timer out, end time %d!")
TRACE_MSG(GPS_NMEA_258_112_2_18_0_33_8_1561,"Unknown NMEA FRAME 0x%x")
TRACE_MSG(GPS_NMEA_287_112_2_18_0_33_8_1562,"NMEA: Queue full, index %d")
TRACE_MSG(GPS_NMEA_301_112_2_18_0_33_8_1563,"[DRV_GPS_NMEA]<WARNING> _NMEA_AddFrame2Queue: status is invalid\r\n")
TRACE_MSG(GPS_NMEA_354_112_2_18_0_33_8_1564,"[DRV_GPS_NMEA]<WARNING> _NMEA_DelFrameFromQueue: status is invalid\r\n")
TRACE_MSG(GPS_NMEA_368_112_2_18_0_33_9_1565,"NMEA: Queue Empty!")
TRACE_MSG(GPS_NMEA_416_112_2_18_0_33_9_1566,"  < NMEA: %s > ")
TRACE_MSG(GPS_NMEA_425_112_2_18_0_33_9_1567,"NMEA: Drop frame - end error")
TRACE_MSG(GPS_NMEA_437_112_2_18_0_33_9_1568,"NMEA: Drop frame - data error")
TRACE_MSG(GPS_NMEA_459_112_2_18_0_33_9_1569,"NMEA: no end")
TRACE_MSG(GPS_NMEA_467_112_2_18_0_33_9_1570,"NMEA: no start")
TRACE_MSG(GPS_NMEA_593_112_2_18_0_33_9_1571,"[DRV_NMEA]<WARNING> _NMEA_DecodeGPGGA: field_cnt is invalid\r\n")
TRACE_MSG(GPS_NMEA_643_112_2_18_0_33_9_1572,"[DRV_NMEA]<ERROR> _NMEA_DecodeGPGGA: field_cnt is invalid\r\n")
TRACE_MSG(GPS_NMEA_788_112_2_18_0_33_10_1573,"[DRV_NMEA]<WARNING> _NMEA_DecodeGPGSV:field_cnt is invalid\r\n ")
TRACE_MSG(GPS_NMEA_859_112_2_18_0_33_10_1574,"[DRV_NMEA]<ERROR> _NMEA_DecodeGPGSV:field_cnt is invalid\r\n ")
TRACE_MSG(GPS_NMEA_915_112_2_18_0_33_10_1575,"NMEA_Init() !")
TRACE_MSG(GPS_NMEA_926_112_2_18_0_33_10_1576,"NMEA_Close() !")
TRACE_MSG(GPS_NMEA_947_112_2_18_0_33_10_1577,"NMEA NOT INIT !!!")
TRACE_MSG(GPS_NMEA_953_112_2_18_0_33_10_1578,"No data in NMEA Stream !!")
TRACE_MSG(GPS_NMEA_957_112_2_18_0_33_10_1579,"*** NMEA_SnatchFramesFromStream START, data len %d")
TRACE_MSG(GPS_NMEA_961_112_2_18_0_33_10_1580,"NMEA: Split frame ! cur data %d")
TRACE_MSG(GPS_NMEA_1065_112_2_18_0_33_10_1581,"*** NMEA_SnatchFramesFromStream END ")
TRACE_MSG(GPS_NMEA_1137_112_2_18_0_33_10_1582,"*******  _NMEA_Test_Task Start *******")
TRACE_MSG(GPS_NMEA_1143_112_2_18_0_33_10_1583,"-- NMEA_Test_Task Loop --")
TRACE_MSG(GPS_APP_210_112_2_18_0_33_11_1584,"%s")
TRACE_MSG(GPS_APP_219_112_2_18_0_33_11_1585,"APP: NULL debug Message.")
TRACE_MSG(GPS_APP_223_112_2_18_0_33_11_1586,"%s")
TRACE_MSG(GPS_APP_234_112_2_18_0_33_11_1587,"APP: INFO: got ref loc")
TRACE_MSG(GPS_APP_244_112_2_18_0_33_11_1588,"APP: ERROR: SIRF_PAL_OS_SEMAPHORE_Release() error: 0x%X\n")
TRACE_MSG(GPS_APP_255_112_2_18_0_33_11_1589,"App: INFO: got all (%d/%d) fixes")
TRACE_MSG(GPS_APP_259_112_2_18_0_33_11_1590,"APP: INFO: got a fix (%d/%d)")
TRACE_MSG(GPS_APP_413_112_2_18_0_33_11_1591,"APP: gpioInitialization() error: 0x%X\n")
TRACE_MSG(GPS_APP_419_112_2_18_0_33_11_1592,"APP: ERROR: Init cannot set LSM Common configuration data")
TRACE_MSG(GPS_APP_423_112_2_18_0_33_11_1593,"APP: INFO: LSM version is %s")
TRACE_MSG(GPS_APP_427_112_2_18_0_33_11_1594,"APP: ERROR: Init failed calling LSM_Init")
TRACE_MSG(GPS_APP_440_112_2_18_0_33_11_1595,"APP: ERROR: Deinit failed calling LSM_Deinit")
TRACE_MSG(GPS_APP_446_112_2_18_0_33_11_1596,"gpioDeInitialization() error: 0x%X\n")
TRACE_MSG(GPS_APP_472_112_2_18_0_33_11_1597,"APP: Semaphore (semPosition) Creation Failed!")
TRACE_MSG(GPS_APP_478_112_2_18_0_33_11_1598,"APP: ERROR: Open cannot obtain session cfg data")
TRACE_MSG(GPS_APP_487_112_2_18_0_33_11_1599,"APP: ERROR: Unable to get SET ID: %d")
TRACE_MSG(GPS_APP_517_112_2_18_0_33_12_1600,"APP: ERROR: Open failed calling LSM_Start")
TRACE_MSG(GPS_APP_529_112_2_18_0_33_12_1601,"APP: ERROR: Close failed calling LSM_Stop!")
TRACE_MSG(GPS_APP_549_112_2_18_0_33_12_1602,"Failed to open On_Off port in Main \n")
TRACE_MSG(GPS_APP_555_112_2_18_0_33_12_1603,"Failed to open Reset port in Main \n")
TRACE_MSG(GPS_APP_631_112_2_18_0_33_12_1604,"start gps init")
TRACE_MSG(GPS_APP_636_112_2_18_0_33_12_1605,"end gps init")
TRACE_MSG(GPS_APP_645_112_2_18_0_33_12_1606,"[GPS]StartGpsThread:priority=0x%X\r\n")
TRACE_MSG(GPS_APP_651_112_2_18_0_33_12_1607,"[GPS]OS_StartThread - s_gps_sched_thread_id = 0x%X\r\n")
TRACE_MSG(GPS_APP_663_112_2_18_0_33_12_1608,"APP: ============ SiRFNavIV LSM Spreadtrum Demo App =============")
TRACE_MSG(GPS_APP_669_112_2_18_0_33_12_1609,"ERROR: SIRF_PAL_Init Failed %d")
TRACE_MSG(GPS_APP_678_112_2_18_0_33_12_1610,"APP: LPL Init Failed: 0x%x")
TRACE_MSG(GPS_APP_683_112_2_18_0_33_12_1611,"ERROR: SIRF_PAL_Destroy Failed %d")
TRACE_MSG(GPS_APP_691_112_2_18_0_33_12_1612,"APP: Session: %d of %d")
TRACE_MSG(GPS_APP_695_112_2_18_0_33_12_1613,"APP: LPL Session Open Failed")
TRACE_MSG(GPS_APP_706_112_2_18_0_33_12_1614,"ERROR: LPL_Close Failed %d")
TRACE_MSG(GPS_APP_711_112_2_18_0_33_12_1615,"APP: LPL Session Closed.")
TRACE_MSG(GPS_APP_718_112_2_18_0_33_12_1616,"ERROR: LPL_Deinit Failed %d")
TRACE_MSG(GPS_APP_721_112_2_18_0_33_12_1617,"APP: All Sessions completed")
TRACE_MSG(GPS_APP_727_112_2_18_0_33_12_1618,"ERROR: SIRF_PAL_Destroy Failed %d")
TRACE_MSG(GPS_CONFIG_58_112_2_18_0_33_12_1619,"GPIO_GPS_PowerOn: %d")
TRACE_MSG(GPS_CONFIG_71_112_2_18_0_33_12_1620,"GPIO_GPS_On: %d")
TRACE_MSG(GPS_CONFIG_85_112_2_18_0_33_12_1621,"GPIO_GPS_BootInter: %d")
TRACE_MSG(GPS_CONFIG_98_112_2_18_0_33_12_1622,"GPIO_GPS_ResetLow: %d")
TRACE_MSG(GPS_CONFIG_111_112_2_18_0_33_12_1623,"GPIO_GPS_Wakeup: %d")
TRACE_MSG(GPS_CONFIG_130_112_2_18_0_33_12_1624,"Srf_ReadData: buf=%x, len=%d")
TRACE_MSG(GPS_CONFIG_145_112_2_18_0_33_12_1625,"Srf_WriteData: buf=%x, len=%d")
TRACE_MSG(GPS_CONFIG_159_112_2_18_0_33_12_1626,"Srf_Identify")
TRACE_MSG(SIRF_PAL_TCPIP_81_112_2_18_0_33_19_1627,"CSR Andy SIRF_Security_Send_Data Error data_len %d, data_ptr 0x%x")
TRACE_MSG(SIRF_PAL_TCPIP_87_112_2_18_0_33_19_1628,"CSR Andy SIRF_Security_Send_Data data_len %d")
TRACE_MSG(SIRF_PAL_TCPIP_94_112_2_18_0_33_19_1629,"CSR Andy SIRF_Security_Send_Data sci_sock_send")
TRACE_MSG(SIRF_PAL_TCPIP_99_112_2_18_0_33_19_1630,"CSR Andy SIRF_Security_Send_Data sci_sock_send successfully with bytes %d")
TRACE_MSG(SIRF_PAL_TCPIP_103_112_2_18_0_33_19_1631,"CSR Andy SIRF_Security_Send_Data Warning sci_sock_send data not all sent")
TRACE_MSG(SIRF_PAL_TCPIP_115_112_2_18_0_33_19_1632,"CSR Andy SIRF_Security_Send_Data SIRF_PAL_NET_Select")
TRACE_MSG(SIRF_PAL_TCPIP_119_112_2_18_0_33_19_1633,"CSR Andy SIRF_Security_Send_Data Error SIRF_PAL_NET_Select")
TRACE_MSG(SIRF_PAL_TCPIP_129_112_2_18_0_33_19_1634,"CSR Andy SIRF_Security_Send_Data sci_sock_recv data with bytes %d")
TRACE_MSG(SIRF_PAL_TCPIP_130_112_2_18_0_33_19_1635,"CSR Andy SIRF_Security_Send_Data SSL_DecryptPasser Start")
TRACE_MSG(SIRF_PAL_TCPIP_132_112_2_18_0_33_19_1636,"CSR Andy SIRF_Security_Send_Data SSL_DecryptPasser End")
TRACE_MSG(SIRF_PAL_TCPIP_136_112_2_18_0_33_19_1637,"CSR Andy SIRF_Security_Send_Data Error sci_sock_recv")
TRACE_MSG(SIRF_PAL_TCPIP_142_112_2_18_0_33_19_1638,"CSR Andy SIRF_Security_Send_Data sci_sock_errno for sci_sock_recv")
TRACE_MSG(SIRF_PAL_TCPIP_146_112_2_18_0_33_19_1639,"CSR Andy SIRF_Security_Send_Data sci_sock_errno Pending for sci_sock_recv")
TRACE_MSG(SIRF_PAL_TCPIP_150_112_2_18_0_33_19_1640,"CSR Andy SIRF_Security_Send_Data Error sci_sock_errno err %d for sci_sock_recv")
TRACE_MSG(SIRF_PAL_TCPIP_156_112_2_18_0_33_19_1641,"CSR Andy SIRF_Security_Send_Data Error sci_sock_send")
TRACE_MSG(SIRF_PAL_TCPIP_162_112_2_18_0_33_19_1642,"CSR Andy SIRF_Security_Send_Data sci_sock_errno for sci_sock_send")
TRACE_MSG(SIRF_PAL_TCPIP_166_112_2_18_0_33_19_1643,"CSR Andy SIRF_Security_Send_Data sci_sock_errno Pending for sci_sock_send")
TRACE_MSG(SIRF_PAL_TCPIP_170_112_2_18_0_33_19_1644,"CSR Andy SIRF_Security_Send_Data Error sci_sock_errno err %d for sci_sock_send")
TRACE_MSG(SIRF_PAL_TCPIP_176_112_2_18_0_33_19_1645,"CSR Andy SIRF_Security_Send_Data CsrMemCpy")
TRACE_MSG(SIRF_PAL_TCPIP_190_112_2_18_0_33_19_1646,"CSR Andy SIRF_Security_Send_Data Error data_len %d > max %d")
TRACE_MSG(SIRF_PAL_TCPIP_202_112_2_18_0_33_19_1647,"CSR Andy SIRF_Security_Receive_Data data_len %d")
TRACE_MSG(SIRF_PAL_TCPIP_209_112_2_18_0_33_19_1648,"CSR Andy SIRF_Security_Receive_Data Error data_len %d > max %d")
TRACE_MSG(SIRF_PAL_TCPIP_222_112_2_18_0_33_19_1649,"CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_HANDSHAKE_SUCC")
TRACE_MSG(SIRF_PAL_TCPIP_227_112_2_18_0_33_19_1650,"CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_FAIL")
TRACE_MSG(SIRF_PAL_TCPIP_232_112_2_18_0_33_19_1651,"CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_CLOSE_BY_SERVER")
TRACE_MSG(SIRF_PAL_TCPIP_237_112_2_18_0_33_19_1652,"CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_CANCLED_BY_USER")
TRACE_MSG(SIRF_PAL_TCPIP_242_112_2_18_0_33_20_1653,"CSR Andy SIRF_Security_Post_Message msg_id %d")
TRACE_MSG(SIRF_PAL_TCPIP_250_112_2_18_0_33_20_1654,"CSR Andy SIRF_Security_Show_Cert_Info")
TRACE_MSG(SIRF_PAL_TCPIP_258_112_2_18_0_33_20_1655,"CSR Andy SIRF_PAL_NET_Init")
TRACE_MSG(SIRF_PAL_TCPIP_428_112_2_18_0_33_20_1656,"CSR Andy SIRF_PAL_NET_Connect sci_sock_connect")
TRACE_MSG(SIRF_PAL_TCPIP_435_112_2_18_0_33_20_1657,"CSR Andy SIRF_PAL_NET_Connect sci_sock_connect sci_sock_errno")
TRACE_MSG(SIRF_PAL_TCPIP_439_112_2_18_0_33_20_1658,"CSR Andy SIRF_PAL_NET_Connect sci_sock_connect Pending")
TRACE_MSG(SIRF_PAL_TCPIP_444_112_2_18_0_33_20_1659,"CSR Andy SIRF_PAL_NET_Connect Error sci_sock_connect %d")
TRACE_MSG(SIRF_PAL_TCPIP_454_112_2_18_0_33_20_1660,"CSR Andy SIRF_PAL_NET_Connect SSL sci_sock_connect")
TRACE_MSG(SIRF_PAL_TCPIP_467_112_2_18_0_33_20_1661,"CSR Andy SIRF_PAL_NET_Connect SSL SIRF_PAL_NET_Select")
TRACE_MSG(SIRF_PAL_TCPIP_471_112_2_18_0_33_20_1662,"CSR Andy SIRF_PAL_NET_Connect SSL Error SIRF_PAL_NET_Select")
TRACE_MSG(SIRF_PAL_TCPIP_476_112_2_18_0_33_20_1663,"CSR Andy SIRF_PAL_NET_Connect SSL_Create")
TRACE_MSG(SIRF_PAL_TCPIP_478_112_2_18_0_33_20_1664,"CSR Andy SIRF_PAL_NET_Connect SSL_Create ssl_handle_csr %d")
TRACE_MSG(SIRF_PAL_TCPIP_482_112_2_18_0_33_20_1665,"CSR Andy SIRF_PAL_NET_Connect SSL_Create Fail")
TRACE_MSG(SIRF_PAL_TCPIP_489_112_2_18_0_33_20_1666,"CSR Andy SIRF_PAL_NET_Connect SSL_HandShake")
TRACE_MSG(SIRF_PAL_TCPIP_490_112_2_18_0_33_20_1667,"CSR Andy SIRF_PAL_NET_Connect SSL_HandShake %s")
TRACE_MSG(SIRF_PAL_TCPIP_495_112_2_18_0_33_20_1668,"CSR Andy SIRF_PAL_NET_Connect SSL_HandShake Successfully")
TRACE_MSG(SIRF_PAL_TCPIP_504_112_2_18_0_33_20_1669,"CSR Andy SIRF_PAL_NET_Connect SSL_HandShake count: %d, ret %d")
TRACE_MSG(SIRF_PAL_TCPIP_790_112_2_18_0_33_21_1670,"CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser ")
TRACE_MSG(SIRF_PAL_TCPIP_795_112_2_18_0_33_21_1671,"CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser Waiting")
TRACE_MSG(SIRF_PAL_TCPIP_802_112_2_18_0_33_21_1672,"CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser Done with decrypted_data_len %d")
TRACE_MSG(SIRF_PAL_TCPIP_808_112_2_18_0_33_21_1673,"CSR Andy SIRF_PAL_NET_Read SSL Error SSL_DecryptPasser Done with decrypted_data_len %d (buffer size %d)")
TRACE_MSG(SIRF_PAL_TCPIP_857_112_2_18_0_33_21_1674,"CSR Andy SIRF_PAL_NET_Write SSL")
TRACE_MSG(SIRF_PAL_TCPIP_865_112_2_18_0_33_21_1675,"CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser")
TRACE_MSG(SIRF_PAL_TCPIP_870_112_2_18_0_33_21_1676,"CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser Waiting")
TRACE_MSG(SIRF_PAL_TCPIP_875_112_2_18_0_33_21_1677,"CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser Done with encrypted_data_len_written %d")
TRACE_MSG(SIRF_PAL_TCPIP_879_112_2_18_0_33_21_1678,"CSR Andy SIRF_PAL_NET_Write SSL sci_sock_send")
TRACE_MSG(SIRF_PAL_TCPIP_884_112_2_18_0_33_21_1679,"CSR Andy SIRF_PAL_NET_Write SSL Error with encrypted_data_len_written 0")
TRACE_MSG(SIRF_PAL_TCPIP_890_112_2_18_0_33_21_1680,"CSR Andy SIRF_PAL_NET_Write SSL sci_sock_send successfully with bytes %d")
TRACE_MSG(SIRF_PAL_TCPIP_894_112_2_18_0_33_21_1681,"CSR Andy SIRF_PAL_NET_Write SSL bytes left %d, new writing position %d")
TRACE_MSG(SIRF_PAL_TCPIP_898_112_2_18_0_33_21_1682,"CSR Andy SIRF_PAL_NET_Write SSL A set of encrption data done ")
TRACE_MSG(SIRF_PAL_TCPIP_910_112_2_18_0_33_21_1683,"CSR Andy SIRF_PAL_NET_Write SSL Error sci_sock_send")
TRACE_MSG(SIRF_PAL_TCPIP_919_112_2_18_0_33_21_1684,"CSR Andy SIRF_PAL_NET_Write SSL sci_sock_errno")
TRACE_MSG(SIRF_PAL_TCPIP_924_112_2_18_0_33_21_1685,"CSR Andy SIRF_PAL_NET_Write SSL sci_sock_errno Pending")
TRACE_MSG(SIRF_PAL_TCPIP_929_112_2_18_0_33_21_1686,"CSR Andy SIRF_PAL_NET_Write SSL Error sci_sock_errno err %d")
TRACE_MSG(GPS_SIRF_60_112_2_18_0_33_21_1687,"GPIO_GPS_PowerOn: %d")
TRACE_MSG(GPS_SIRF_73_112_2_18_0_33_21_1688,"GPIO_GPS_On: %d")
TRACE_MSG(GPS_SIRF_86_112_2_18_0_33_21_1689,"GPIO_GPS_BootInter: %d")
TRACE_MSG(GPS_SIRF_99_112_2_18_0_33_21_1690,"GPIO_GPS_ResetLow: %d")
TRACE_MSG(GPS_SIRF_112_112_2_18_0_33_21_1691,"GPIO_GPS_Wakeup: %d")
TRACE_MSG(GPS_SIRF_127_112_2_18_0_33_22_1692,"GPIO_GPS_UART2GPIO: %d")
TRACE_MSG(GPS_SIRF_158_112_2_18_0_33_22_1693,"Srf_Init")
TRACE_MSG(GPS_SIRF_160_112_2_18_0_33_22_1694,"config Uart1 to GPIO and pull them down for saving power")
TRACE_MSG(GPS_SIRF_178_112_2_18_0_33_22_1695,"Srf_Open: mode=%d")
TRACE_MSG(GPS_SIRF_191_112_2_18_0_33_22_1696,"Srf_Open: reflash!")
TRACE_MSG(GPS_SIRF_245_112_2_18_0_33_22_1697,"Srf_Close")
TRACE_MSG(GPS_SIRF_265_112_2_18_0_33_22_1698,"[GPIO_GPS_PowerOn] OFF")
TRACE_MSG(GPS_SIRF_279_112_2_18_0_33_22_1699,"Srf_Sleep: is_sleep=%d")
TRACE_MSG(GPS_SIRF_295_112_2_18_0_33_22_1700,"Srf_Reflash")
TRACE_MSG(GPS_SIRF_313_112_2_18_0_33_22_1701,"Srf_ReadData: buf=%x, len=%d")
TRACE_MSG(GPS_SIRF_328_112_2_18_0_33_22_1702,"Srf_WriteData: buf=%x, len=%d")
TRACE_MSG(GPS_SIRF_342_112_2_18_0_33_22_1703,"Srf_Identify")
TRACE_MSG(GPS_SIRF_355_112_2_18_0_33_22_1704,"Srf_Test")
TRACE_MSG(SRF_FUNC_341_112_2_18_0_33_23_1705,"%s")
TRACE_MSG(SRF_FUNC_359_112_2_18_0_33_23_1706,"%s")
TRACE_MSG(SRF_FUNC_387_112_2_18_0_33_23_1707,"GpsTTFFSrf, begin time %d")
TRACE_MSG(SRF_FUNC_393_112_2_18_0_33_23_1708,"GpsTTFFSrf, response 0x%x")
TRACE_MSG(SRF_FUNC_406_112_2_18_0_33_23_1709,"GpsTTFFSrf, end time %d")
TRACE_MSG(SRF_FUNC_439_112_2_18_0_33_23_1710,"GPS test mode 4:")
TRACE_MSG(SRF_FUNC_459_112_2_18_0_33_23_1711,"GpsTest4Srf: reset No ack.")
TRACE_MSG(SRF_FUNC_467_112_2_18_0_33_23_1712,"GpsTest4Srf, begin %d")
TRACE_MSG(SRF_FUNC_472_112_2_18_0_33_23_1713,"GpsTest4Srf, response 0x%x")
TRACE_MSG(SRF_FUNC_479_112_2_18_0_33_23_1714,"GpsTest4Srf, end %d")
TRACE_MSG(SRF_FUNC_528_112_2_18_0_33_23_1715,"GPS: write command %d")
TRACE_MSG(SRF_FUNC_540_112_2_18_0_33_23_1716,"GPS: NMEA, %s")
TRACE_MSG(SRF_FUNC_553_112_2_18_0_33_23_1717,"GpsTestSrf loop")
TRACE_MSG(SRF_FUNC_558_112_2_18_0_33_23_1718,"GPS: id=%x")
TRACE_MSG(SRF_FUNC_567_112_2_18_0_33_23_1719,"GPS: skip")
TRACE_MSG(SRF_FUNC_582_112_2_18_0_33_23_1720,"GpsTestSrf, TTFF time %d")
TRACE_MSG(SRF_FUNC_586_112_2_18_0_33_23_1721,"GpsTestSrf, GpsTTFFSrf error %d")
TRACE_MSG(SRF_FUNC_603_112_2_18_0_33_23_1722,"GpsTestSrf, skip %d")
TRACE_MSG(SRF_FUNC_611_112_2_18_0_33_23_1723,"\"#GPS, GpsTest4Srf")
TRACE_MSG(SRF_FUNC_616_112_2_18_0_33_23_1724,"GpsTest4Srf, error %d")
TRACE_MSG(SRF_FUNC_623_112_2_18_0_33_23_1725,"GpsTest4Srf, response 0x%x")
TRACE_MSG(UARTCOM_SAMPLE_280_112_2_18_0_33_24_1726,"[DRV_GPS_COM]<WARNING> uart1_callback: event > COM_MAX_EVENT\r\n")
TRACE_MSG(UARTCOM_SAMPLE_350_112_2_18_0_33_24_1727,"\r\nCOM1_Init.")
TRACE_MSG(UARTCOM_SAMPLE_384_112_2_18_0_33_24_1728,"\r\nCOM0_Close.")
TRACE_MSG(LCD_BACKLIGHT_144_112_2_18_0_33_24_1729,"_LCD_GetBackLightType: addr 0x%x")
TRACE_MSG(LCD_BACKLIGHT_147_112_2_18_0_33_24_1730,"_LCD_GetBackLightType: type %d, param %d")
TRACE_MSG(LCD_BACKLIGHT_446_112_2_18_0_33_25_1731,"LCD_SetBackLightBrightness: BackLight type %d, Brightness %d")
TRACE_MSG(LCD_BACKLIGHT_459_112_2_18_0_33_25_1732,"LCD_SetBackLightBrightness: BackLight type %d, backlight_ness %d")
TRACE_MSG(LCD_DIAG_151_112_2_18_0_33_25_1733,"PE_RGBToYUV:src=0x%x,dst=0x%x,width=%d,height=%d")
TRACE_MSG(LCD_DIAG_184_112_2_18_0_33_25_1734,"---PE_PROCESS:YUVToJPG malloc is fail!---")
TRACE_MSG(LCD_DIAG_196_112_2_18_0_33_26_1735,"[pe_process.c] SaveJpeg successed! ")
TRACE_MSG(LCD_DIAG_241_112_2_18_0_33_26_1736,"---HandleReadMainBuffer in---time =%d-")
TRACE_MSG(LCD_DIAG_253_112_2_18_0_33_26_1737,"HandleReadMainBuffer:malloc failed!")
TRACE_MSG(LCD_DIAG_265_112_2_18_0_33_26_1738,"HandleReadMainBuffer:malloc failed1!")
TRACE_MSG(LCD_DIAG_277_112_2_18_0_33_26_1739,"HandleReadMainBuffer:size=%d")
TRACE_MSG(LCD_DIAG_325_112_2_18_0_33_26_1740,"---HandleReadMainBuffer out---time =%d-")
TRACE_MSG(LCD_DIAG_337_112_2_18_0_33_26_1741,"LCD: LCD_DIAG_RegDiagCmdRoutine")
TRACE_MSG(TFT_HX8312A_165_112_2_18_0_33_26_1742,"HX8312 initialize!")
TRACE_MSG(TFT_HX8312A_459_112_2_18_0_33_27_1743,"HX8312_EnterSleep,%d")
TRACE_MSG(TFT_HX8340B_159_112_2_18_0_33_28_1744,"qinss LCD: in HX8340B_reset")
TRACE_MSG(TFT_HX8340B_168_112_2_18_0_33_28_1745,"\"\"\"#LCD: in HX8340B_Init ID(0x93)=,0x%x")
TRACE_MSG(TFT_HX8340B_255_112_2_18_0_33_28_1746,"qinss LCD: in HX8340B_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_HX8340B_299_112_2_18_0_33_28_1747,"qinss LCD: in HX8340B_Close")
TRACE_MSG(TFT_HX8340B_451_112_2_18_0_33_28_1748,"qinss LCD: in HX8340B_GetOperations")
TRACE_MSG(TFT_HX8346_181_112_2_18_0_33_29_1749,"qinss LCD: in HX8346_reset")
TRACE_MSG(TFT_HX8346_344_112_2_18_0_33_29_1750,"qinss LCD: in HX8346_reset end.")
TRACE_MSG(TFT_HX8346_357_112_2_18_0_33_29_1751,"qinss LCD: in HX8346_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_HX8346_392_112_2_18_0_33_29_1752,"qinss LCD: in HX8346_Close")
TRACE_MSG(TFT_HX8346_541_112_2_18_0_33_29_1753,"qinss LCD: in HX8346_GetOperations")
TRACE_MSG(TFT_HX8347_199_112_2_18_0_33_30_1754,"qinss LCD: in HX8347_reset")
TRACE_MSG(TFT_HX8347_334_112_2_18_0_33_30_1755,"qinss LCD: in HX8347_reset end.")
TRACE_MSG(TFT_HX8347_347_112_2_18_0_33_30_1756,"qinss LCD: in HX8347_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_HX8347_382_112_2_18_0_33_30_1757,"qinss LCD: in HX8347_Close")
TRACE_MSG(TFT_HX8347_525_112_2_18_0_33_31_1758,"qinss LCD: in HX8347_GetOperations")
TRACE_MSG(TFT_HX8347G_193_112_2_18_0_33_33_1759,"HX8347G set direction,direct_type=%d")
TRACE_MSG(TFT_HX8347G_232_112_2_18_0_33_33_1760,"HX8347G set dis window,s_lcd_direct=%d")
TRACE_MSG(TFT_HX8347G_313_112_2_18_0_33_33_1761,"qinss LCD: in HX8347G_reset")
TRACE_MSG(TFT_HX8347G_321_112_2_18_0_33_33_1762,"\"\"\"#LCD: in HX8347G_Init ID(0x0)=,0x%x")
TRACE_MSG(TFT_HX8347G_487_112_2_18_0_33_33_1763,"qinss LCD: in HX8347G_reset end.")
TRACE_MSG(TFT_HX8347G_499_112_2_18_0_33_33_1764,"HX8347G_Init")
TRACE_MSG(TFT_HX8347G_514_112_2_18_0_33_33_1765,"qinss LCD: in HX8347G_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_HX8347G_541_112_2_18_0_33_33_1766,"qinss LCD: in HX8347G_Close")
TRACE_MSG(TFT_HX8347G_692_112_2_18_0_33_34_1767,"qinss LCD: in HX8347G_GetOperations")
TRACE_MSG(TFT_HX8347G_SPI_191_112_2_18_0_33_34_1768,"[REF_LCD] HX8347G new left = %d, top = %d, right = %d, bottom = %d")
TRACE_MSG(TFT_HX8352C_393_112_2_18_0_33_36_1769,"\r\n")
TRACE_MSG(TFT_HX8357_241_112_2_18_0_33_37_1770,"\r\n")
TRACE_MSG(TFT_HX8357_710_112_2_18_0_33_38_1771,"\r\n")
TRACE_MSG(TFT_ILI9225_393_112_2_18_0_33_40_1772,"qinss LCD: in ILI9225B_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9225_417_112_2_18_0_33_40_1773,"qinss LCD: in ILI9225B_Close")
TRACE_MSG(TFT_ILI9225_567_112_2_18_0_33_40_1774,"qinss LCD: in ILI9225B_GetOperations")
TRACE_MSG(TFT_ILI9225B_397_112_2_18_0_33_41_1775,"qinss LCD: in ILI9225B_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9225B_421_112_2_18_0_33_41_1776,"qinss LCD: in ILI9225B_Close")
TRACE_MSG(TFT_ILI9225B_571_112_2_18_0_33_41_1777,"qinss LCD: in ILI9225B_GetOperations")
TRACE_MSG(TFT_ILI9320_233_112_2_18_0_33_42_1778,"qinss LCD: in ILI9320_reset")
TRACE_MSG(TFT_ILI9320_404_112_2_18_0_33_43_1779,"qinss LCD: in ILI9320_reset end.")
TRACE_MSG(TFT_ILI9320_418_112_2_18_0_33_43_1780,"qinss LCD: in ILI9320_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9320_498_112_2_18_0_33_43_1781,"qinss LCD: in ILI9320_Close")
TRACE_MSG(TFT_ILI9320_648_112_2_18_0_33_43_1782,"qinss LCD: in ILI9320_GetOperations")
TRACE_MSG(TFT_ILI9325_317_112_2_18_0_33_44_1783,"qinss LCD: in ILI9325_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9325_346_112_2_18_0_33_44_1784,"qinss LCD: in ILI9325_Close")
TRACE_MSG(TFT_ILI9325_498_112_2_18_0_33_44_1785,"qinss LCD: in ILI9325_GetOperations")
TRACE_MSG(TFT_ILI9325B8_305_112_2_18_0_33_45_1786,"qinss LCD: in ILI9325_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9325B8_334_112_2_18_0_33_45_1787,"qinss LCD: in ILI9325B8_Close")
TRACE_MSG(TFT_ILI9325B8_486_112_2_18_0_33_45_1788,"qinss LCD: in ILI9325B8_GetOperations")
TRACE_MSG(TFT_ILI9326_325_112_2_18_0_33_46_1789,"qinss LCD: in ILI9326_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9326_366_112_2_18_0_33_46_1790,"qinss LCD: in ILI9326_Close")
TRACE_MSG(TFT_ILI9326_517_112_2_18_0_33_47_1791,"qinss LCD: in ILI9326_GetOperations")
TRACE_MSG(TFT_ILI9328_250_112_2_18_0_33_47_1792,"qinss LCD: in ILI9328_reset")
TRACE_MSG(TFT_ILI9328_343_112_2_18_0_33_48_1793,"qinss LCD: in ILI9328_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9328_375_112_2_18_0_33_48_1794,"qinss LCD: in ILI9328_Close")
TRACE_MSG(TFT_ILI9328_527_112_2_18_0_33_48_1795,"qinss LCD: in ILI9328_GetOperations")
TRACE_MSG(TFT_ILI9328_765_112_2_18_0_33_48_1796,"qinss LCD: in ILI9328_reset")
TRACE_MSG(TFT_ILI9328_858_112_2_18_0_33_49_1797,"qinss LCD: in ILI9328_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9328_890_112_2_18_0_33_49_1798,"qinss LCD: in ILI9328_Close")
TRACE_MSG(TFT_ILI9328_1042_112_2_18_0_33_49_1799,"qinss LCD: in ILI9328_GetOperations")
TRACE_MSG(TFT_ILI9335_252_112_2_18_0_33_50_1800,"qinss LCD: in ILI9335_reset")
TRACE_MSG(TFT_ILI9335_328_112_2_18_0_33_50_1801,"ILI9335_Init")
TRACE_MSG(TFT_ILI9335_347_112_2_18_0_33_50_1802,"qinss LCD: in ILI9335_Clear end")
TRACE_MSG(TFT_ILI9335_360_112_2_18_0_33_50_1803,"qinss LCD: in ILI9335_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9335_392_112_2_18_0_33_50_1804,"qinss LCD: in ILI9335_Close")
TRACE_MSG(TFT_ILI9335_544_112_2_18_0_33_51_1805,"qinss LCD: in ILI9335_GetOperations")
TRACE_MSG(TFT_ILI9338B_152_112_2_18_0_33_51_1806,"ILI9338B set direction,direct_type=%d")
TRACE_MSG(TFT_ILI9338B_169_112_2_18_0_33_51_1807,"ILI9338B set dis window,s_lcd_direct=%d")
TRACE_MSG(TFT_ILI9338B_209_112_2_18_0_33_51_1808,"qinss LCD: in ILI9338B_reset")
TRACE_MSG(TFT_ILI9338B_217_112_2_18_0_33_51_1809,"\"\"\"#LCD: in ILI9338B_Init ID(0x0)=,0x%x")
TRACE_MSG(TFT_ILI9338B_383_112_2_18_0_33_52_1810,"qinss LCD: in ILI9338B_reset end.")
TRACE_MSG(TFT_ILI9338B_395_112_2_18_0_33_52_1811,"ILI9338B_Init")
TRACE_MSG(TFT_ILI9338B_410_112_2_18_0_33_52_1812,"qinss LCD: in ILI9338B_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9338B_432_112_2_18_0_33_52_1813,"qinss LCD: in ILI9338B_Close")
TRACE_MSG(TFT_ILI9338B_583_112_2_18_0_33_52_1814,"qinss LCD: in ILI9338B_GetOperations")
TRACE_MSG(TFT_ILI9342_177_112_2_18_0_33_53_1815,"Direction LCD: ILI9342 Direction=%x")
TRACE_MSG(TFT_ILI9342_195_112_2_18_0_33_53_1816,"ILI9342 window:left=%x,right=%x,top=%x,bottom=%x")
TRACE_MSG(TFT_ILI9342_262_112_2_18_0_33_53_1817," ILI9342_driver")
TRACE_MSG(TFT_ILI9342_350_112_2_18_0_33_53_1818," ILI9342_driver end ")
TRACE_MSG(TFT_ILI9342_358_112_2_18_0_33_53_1819,"ILI9342_Init")
TRACE_MSG(TFT_ILI9342_374_112_2_18_0_33_53_1820,"qinss LCD: in ILI9342_Clear end")
TRACE_MSG(TFT_ILI9342_386_112_2_18_0_33_53_1821,"qinss LCD: in ILI9342_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_ILI9342_410_112_2_18_0_33_53_1822,"qinss LCD: in ILI9342_Close")
TRACE_MSG(TFT_ILI9342_581_112_2_18_0_33_54_1823,"qinss LCD: in ILI9342_GetOperations")
TRACE_MSG(TFT_ILI9481_244_112_2_18_0_33_54_1824,"ILI9481_Init")
TRACE_MSG(TFT_ILI9481_437_112_2_18_0_33_55_1825,"ILI9481_IDCheck dev_id[1]=%x, dev_id[2]=%x, dev_id[3]=%x")
TRACE_MSG(TFT_ILI9486_170_112_2_18_0_33_55_1826,"ILI9486_Init")
TRACE_MSG(TFT_ILI9486_417_112_2_18_0_33_56_1827,"ILI9486_IDCheck dev_id[1]=%x, dev_id[2]=%x, dev_id[3]=%x")
TRACE_MSG(TFT_LGDP4531_241_112_2_18_0_33_56_1828,"LCD: in LGDP4531_reset")
TRACE_MSG(TFT_LGDP4531_308_112_2_18_0_33_56_1829,"LCD: in LGDP4531_reset end.")
TRACE_MSG(TFT_LGDP4531_323_112_2_18_0_33_56_1830," LCD: in LGDP4531_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_LGDP4531_373_112_2_18_0_33_57_1831,"LCD: in LGDP4531_Close")
TRACE_MSG(TFT_LGDP4531_523_112_2_18_0_33_57_1832,"LCD: in LGDP4531_GetOperations")
TRACE_MSG(TFT_LGDP4532_243_112_2_18_0_33_58_1833,"LCD: in LGDP4532_reset")
TRACE_MSG(TFT_LGDP4532_310_112_2_18_0_33_58_1834,"LCD: in LGDP4532_reset end.")
TRACE_MSG(TFT_LGDP4532_322_112_2_18_0_33_58_1835,"LCD: in LGDP4532_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_LGDP4532_374_112_2_18_0_33_58_1836," LCD: in LGDP4532_Close")
TRACE_MSG(TFT_LGDP4532_524_112_2_18_0_33_58_1837,"LCD: in LGDP4532_GetOperations")
TRACE_MSG(TFT_NT3911_275_112_2_18_0_33_59_1838,"NT3911_EnterSleep,%d")
TRACE_MSG(TFT_NT3911_353_112_2_18_0_33_59_1839,"NT3911_SetDisplayWindow:%d,%d,%d,%d")
TRACE_MSG(TFT_NT3911_362_112_2_18_0_33_59_1840," NT3911_SetDisplayWindow wrong: right > left or bottom > top")
TRACE_MSG(TFT_NT39118_286_112_2_18_0_34_0_1841,"NT39118_EnterSleep,%d")
TRACE_MSG(TFT_NT39118_324_112_2_18_0_34_0_1842,"NT39118_SetDisplayWindow:%d,%d,%d,%d")
TRACE_MSG(TFT_NT39118_333_112_2_18_0_34_0_1843," NT39118_SetDisplayWindow wrong: right > left or bottom > top")
TRACE_MSG(TFT_R61505U_325_112_2_18_0_34_1_1844,"R61505U_EnterSleep,%d")
TRACE_MSG(TFT_R61505U_390_112_2_18_0_34_1_1845,"R61505U_SetDisplayWindow:%d,%d,%d,%d")
TRACE_MSG(TFT_R61505U_399_112_2_18_0_34_1_1846," R61505U_SetDisplayWindow wrong: right > left or bottom > top")
TRACE_MSG(TFT_R61509V_237_112_2_18_0_34_2_1847,"qinss LCD: in R61509V_reset")
TRACE_MSG(TFT_R61509V_352_112_2_18_0_34_2_1848,"qinss LCD: in R61509V_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_R61509V_375_112_2_18_0_34_2_1849,"qinss LCD: in R61509V_Close")
TRACE_MSG(TFT_R61509V_524_112_2_18_0_34_2_1850,"qinss LCD: in R61509V_GetOperations")
TRACE_MSG(TFT_R61529_179_112_2_18_0_34_3_1851,"qinss LCD: in R61529_reset")
TRACE_MSG(TFT_R61529_364_112_2_18_0_34_3_1852,"qinss LCD: in R61529_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_R61529_385_112_2_18_0_34_3_1853,"qinss LCD: in R61529_Close")
TRACE_MSG(TFT_R61529_604_112_2_18_0_34_4_1854,"qinss LCD: in R61529_GetOperations")
TRACE_MSG(TFT_R61581_181_112_2_18_0_34_4_1855,"R61581 initialize!")
TRACE_MSG(TFT_RM68040_112_112_2_18_0_34_5_1856,"RM68040_Init\r\n")
TRACE_MSG(TFT_RM68040_248_112_2_18_0_34_5_1857,"rm68040_set_window\n")
TRACE_MSG(TFT_S6D0139_207_112_2_18_0_34_6_1858,"S6D0139 initialize!")
TRACE_MSG(TFT_S6D04H0A_199_112_2_18_0_34_8_1859,"S6D04H0A initialize!")
TRACE_MSG(TFT_SPFD5408_236_112_2_18_0_34_9_1860,"SPDF5408 LCD: in SPDF5408_reset")
TRACE_MSG(TFT_SPFD5408_341_112_2_18_0_34_9_1861,"qinss LCD: in SPDF5408_reset end.")
TRACE_MSG(TFT_SPFD5408_354_112_2_18_0_34_9_1862,"qinss LCD: in SPDF5408_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_SPFD54124A_325_112_2_18_0_34_10_1863,"SPFD54124A_EnterSleep,%d")
TRACE_MSG(TFT_SPFD54124A_361_112_2_18_0_34_10_1864,"SPFD54124A_SetDisplayWindow:%d,%d,%d,%d")
TRACE_MSG(TFT_SPFD54124A_370_112_2_18_0_34_10_1865," SPFD54124A_SetDisplayWindow wrong: right > left or bottom > top")
TRACE_MSG(TFT_SPFD5420A_242_112_2_18_0_34_11_1866,"LCD: in SPFD5420A_reset")
TRACE_MSG(TFT_SPFD5420A_333_112_2_18_0_34_12_1867,"LCD: in SPFD5420A_reset end.")
TRACE_MSG(TFT_SPFD5420A_345_112_2_18_0_34_12_1868,"LCD: in SPFD5420A_EnterSleep, is_sleep = %d")
TRACE_MSG(TFT_SPFD5420A_370_112_2_18_0_34_12_1869,"LCD: in SPFD5420A_Close")
TRACE_MSG(TFT_SPFD5420A_521_112_2_18_0_34_12_1870,"qinss LCD: in SPFD5420A_GetOperations")
TRACE_MSG(TFT_SSD1228_199_112_2_18_0_34_13_1871,"SSD1288_set_display_window***********")
TRACE_MSG(TFT_SSD1228_254_112_2_18_0_34_13_1872,"SSD1288_InvalidateRect***********")
TRACE_MSG(TFT_SSD1228_283_112_2_18_0_34_13_1873,"SSD1288_EnterSleep,%d")
TRACE_MSG(TFT_SSD1228_314_112_2_18_0_34_13_1874,"SSD1288_SetDisplayWindow:%d,%d,%d,%d")
TRACE_MSG(TFT_SSD1228_323_112_2_18_0_34_13_1875," SSD1288_SetDisplayWindow wrong: right > left or bottom > top")
TRACE_MSG(TFT_ST7781RB8_178_112_2_18_0_34_13_1876,"ST7781R_Init")
TRACE_MSG(TFT_TP057A_VT_176_112_2_18_0_34_14_1877,"LCD ID ERROR!!!HDr@@@@@@@@@@@@ Lcd_ver:%x")
TRACE_MSG(TFT_TP057A_VT_312_112_2_18_0_34_15_1878,"LCD ID ERROR!!!HDr@@@@@@@@@@@@ Lcd_ver:%x")
TRACE_MSG(MSENSOR_ADXL346_205_112_2_18_0_34_16_1879,"[REF_MSENSOR] %s : id = 0x%X")
TRACE_MSG(MSENSOR_ADXL346_357_112_2_18_0_34_16_1880,"MSENSOR: MOCOR_COOR: axis_x = %5d, axis_y = %5d, axis_z = %5d,")
TRACE_MSG(MSENSOR_ADXL346_608_112_2_18_0_34_17_1881,"[REF_MSENSOR] %s :  shake: %8d")
TRACE_MSG(MSENSOR_ADXL346_616_112_2_18_0_34_17_1882,"[REF_MSENSOR] %s :  rotate: %8d")
TRACE_MSG(MSENSOR_DTOS_110_112_2_18_0_34_17_1883,"[DRV_MSENSOR] open IIC failed!\r\n")
TRACE_MSG(MSENSOR_DTOS_130_112_2_18_0_34_17_1884,"[DRV_MSENSOR] open IIC failed!\r\n")
TRACE_MSG(MSENSOR_DTOS_288_112_2_18_0_34_17_1885,"[DRV_MSENSOR] x = %d(%d), y = %d(%d), status = 0x%X, ")
TRACE_MSG(MSENSOR_DTOS_324_112_2_18_0_34_18_1886,"[DRV_MSENSOR] or_index=%d, ")
TRACE_MSG(MSENSOR_DTOS_353_112_2_18_0_34_18_1887,"[DRV_MSENSOR] x = %d, y = %d, status= 0x%xrn")
TRACE_MSG(MSENSOR_DTOS_360_112_2_18_0_34_18_1888,"[DRV_MSENSOR] lcd_face = %d, pin1 = %d")
TRACE_MSG(MSENSOR_DTOS_428_112_2_18_0_34_18_1889,"[DRV_MSENSOR] MOCOR_COOR: x = %d, y = %d, ")
TRACE_MSG(MSENSOR_DTOS_456_112_2_18_0_34_18_1890,"[DRV_MSENSOR] open IIC failed!\r\n")
TRACE_MSG(MSENSOR_DTOS_470_112_2_18_0_34_18_1891,"[DRV_MSENSOR]DETECTION = 0x%X, sensi_val = %d rn")
TRACE_MSG(MSENSOR_DTOS_524_112_2_18_0_34_18_1892,"[DRV_MSENSOR] cmd = %d \r\n")
TRACE_MSG(MSENSOR_LIS35DE_160_112_2_18_0_34_19_1893,"MSENSOR:: ==> open IIC failed!\r\n")
TRACE_MSG(MSENSOR_LIS35DE_173_112_2_18_0_34_19_1894,"MSENSOR:: ==> SUCCEESSFULLY!")
TRACE_MSG(MSENSOR_LIS35DE_215_112_2_18_0_34_19_1895,"MSENSOR::==> open IIC failed!\r\n")
TRACE_MSG(MSENSOR_LIS35DE_248_112_2_18_0_34_19_1896,"MSENSOR:: ==> SUCCEESSFULLY!")
TRACE_MSG(MSENSOR_LIS35DE_311_112_2_18_0_34_19_1897,"MSENSOR: placement:  lcd_face = %d, pin1 = %d")
TRACE_MSG(MSENSOR_LIS35DE_313_112_2_18_0_34_19_1898,"MSENSOR: SENSOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,")
TRACE_MSG(MSENSOR_LIS35DE_385_112_2_18_0_34_19_1899,"MSENSOR: MOCOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,")
TRACE_MSG(MSENSOR_LIS35DE_503_112_2_18_0_34_19_1900,"MSENSOR:: s_last_motion_type= 0x%08X, ")
TRACE_MSG(MSENSOR_LIS35DE_586_112_2_18_0_34_19_1901,"MSENSOR:: cmd = %d \r\n")
TRACE_MSG(MSENSOR_LIS3DH_123_112_2_18_0_34_20_1902,"[REF_MSENSOR] %s : open IIC failed!")
TRACE_MSG(MSENSOR_LIS3DH_137_112_2_18_0_34_20_1903,"[REF_MSENSOR] %s : addr: 0x%02X = 0x%02X  ")
TRACE_MSG(MSENSOR_LIS3DH_156_112_2_18_0_34_20_1904,"[REF_MSENSOR] %s : data: 0x%X%X, 0x%X%X, 0x%X%X")
TRACE_MSG(MSENSOR_LIS3DH_172_112_2_18_0_34_20_1905,"[REF_MSENSOR] %s : open IIC failed!")
TRACE_MSG(MSENSOR_LIS3DH_194_112_2_18_0_34_20_1906,"[REF_MSENSOR] %s : id = 0x%X")
TRACE_MSG(MSENSOR_LIS3DH_220_112_2_18_0_34_20_1907,"[REF_MSENSOR] LIS3DH 0x%X = 0x%2X, 0x%2X, 0x%2X, 0x%2X,")
TRACE_MSG(MSENSOR_LIS3DH_235_112_2_18_0_34_20_1908,"[REF_MSENSOR] %s 1: id = 0x%X")
TRACE_MSG(MSENSOR_LIS3DH_243_112_2_18_0_34_20_1909,"[REF_MSENSOR] %s 2: CTRL1 = 0x%X")
TRACE_MSG(MSENSOR_LIS3DH_249_112_2_18_0_34_20_1910,"[REF_MSENSOR] %s 3: id = 0x%X")
TRACE_MSG(MSENSOR_LIS3DH_340_112_2_18_0_34_20_1911,"MSENSOR: placement:  lcd_face = %d, pin1 = %d")
TRACE_MSG(MSENSOR_LIS3DH_342_112_2_18_0_34_20_1912,"MSENSOR: SENSOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,")
TRACE_MSG(MSENSOR_LIS3DH_414_112_2_18_0_34_20_1913,"MSENSOR: MOCOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,")
TRACE_MSG(MSENSOR_LIS3DH_529_112_2_18_0_34_21_1914,"MSENSOR::")
TRACE_MSG(MSENSOR_LIS3DH_613_112_2_18_0_34_21_1915,"[REF_MSENSOR] %s : cmd = %d")
TRACE_MSG(MSENSOR_DRV_100_112_2_18_0_34_22_1916,"_MSensor_CreateEvent : event =0x%x")
TRACE_MSG(MSENSOR_DRV_146_112_2_18_0_34_22_1917,"_MSensor_SendMSG : msg = %d, para = %d, is_sync = %d")
TRACE_MSG(MSENSOR_DRV_151_112_2_18_0_34_22_1918,"MSensor_TimerHandler: msensor thread Q Full !!")
TRACE_MSG(MSENSOR_DRV_205_112_2_18_0_34_22_1919,"DRV_MSENSOR : init: name=%s type = %d init failed!rn")
TRACE_MSG(MSENSOR_DRV_231_112_2_18_0_34_22_1920,"DRV_MSENSOR : HandleRoutine: msg_id = %d, sensor_type = %drn")
TRACE_MSG(MSENSOR_DRV_248_112_2_18_0_34_22_1921,"DRV_MSENSOR : HandleRoutine: invalid open()")
TRACE_MSG(MSENSOR_DRV_274_112_2_18_0_34_22_1922,"DRV_MSENSOR : HandleRoutine: event = 0x%08X\r\n")
TRACE_MSG(MSENSOR_DRV_304_112_2_18_0_34_22_1923,"MSensor:  _MSensor_CreateTask priority=%d\n")
TRACE_MSG(MSENSOR_DRV_319_112_2_18_0_34_22_1924,"_MSensor_CreateTask : task id 0x%x")
TRACE_MSG(MSENSOR_DRV_327_112_2_18_0_34_22_1925,"MSensor:  _MSensor_ChangeTask new Priority = %dn")
TRACE_MSG(MSENSOR_DRV_336_112_2_18_0_34_22_1926,"MSensor:  _MSensor_ChangeTask old Priority = %dn")
TRACE_MSG(MSENSOR_DRV_339_112_2_18_0_34_22_1927,"MSensor:  _MSensor_ChangeTaskpPriority SCI_INVALID_BLOCK_ID")
TRACE_MSG(MSENSOR_DRV_350_112_2_18_0_34_22_1928,"Msensor: _MSensor_DefaultServiceCB: id = %d, argc = %d argv = %d")
TRACE_MSG(MSENSOR_DRV_375_112_2_18_0_34_22_1929,"Msensor: DIAG, type = %d, event = 0x%08X")
TRACE_MSG(MSENSOR_DRV_431_112_2_18_0_34_23_1930,"MSensor_Open : failed, sensor_type %d not support")
TRACE_MSG(MSENSOR_DRV_461_112_2_18_0_34_23_1931,"MSensor_Open : s_msensor_tab[%d]->name= %s, status = %d, ")
TRACE_MSG(MSENSOR_DRV_478_112_2_18_0_34_23_1932,"MSensor_Close : failed, sensor_type %d not support")
TRACE_MSG(MSENSOR_DRV_500_112_2_18_0_34_23_1933,"MSensor_Close : s_msensor_tab[%d]->name= %s, status = %d, ")
TRACE_MSG(MSENSOR_DRV_512_112_2_18_0_34_23_1934,"MSensor_IntHandler: The thread is NOT running!")
TRACE_MSG(MSENSOR_DRV_541_112_2_18_0_34_23_1935,"MSensor_TimerHandler: The thread is NOT running!")
TRACE_MSG(MSENSOR_DRV_565_112_2_18_0_34_23_1936,"MSensor_ioctl : failed, sensor_type %d not support")
TRACE_MSG(MSENSOR_TRULY_A320_135_112_2_18_0_34_23_1937,"[DRV_OFN] open IIC failed!\r\n")
TRACE_MSG(MSENSOR_TRULY_A320_227_112_2_18_0_34_23_1938,"[DRV_OFN] open IIC failed!\r\n")
TRACE_MSG(MSENSOR_TRULY_A320_348_112_2_18_0_34_24_1939,"[DRV_OFN] cmd = %d \r\n")
TRACE_MSG(MSENSOR_LTR501_134_112_2_18_0_34_24_1940,"[DRV_OPTICAL] LTR501_init_i2c_handler  open IIC failed!")
TRACE_MSG(MSENSOR_LTR501_338_112_2_18_0_34_24_1941,"[DRV_OPTICAL] LTR502_init")
TRACE_MSG(MSENSOR_LTR501_351_112_2_18_0_34_25_1942,"[DRV_OPTICAL] LTR501_open")
TRACE_MSG(MSENSOR_LTR501_444_112_2_18_0_34_25_1943,"[DRV_OPTICAL]als_data %5x")
TRACE_MSG(MSENSOR_SCM013_148_112_2_18_0_34_25_1944,"[Kui Wang]Work_Adc=%d\t,Ambent_Adc=%d\t,Work_Adc - Ambent_Adc=%d\t,cur_status=%d")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_135_112_2_18_0_34_26_1945,"GSensor_Open: Open fail !!")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_165_112_2_18_0_34_26_1946,"GSensor_Close !!")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_183_112_2_18_0_34_26_1947,"_Camera_GetImage : Gsensor is not open !!")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_207_112_2_18_0_34_26_1948,"_Camera_GetImage: _GSensor_GetCaptureParam %d,%d,0x%x")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_213_112_2_18_0_34_26_1949,"_Camera_GetImage: Fail !!")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_217_112_2_18_0_34_26_1950,"_Camera_GetImage: success.")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_233_112_2_18_0_34_26_1951,"met_cam_camera_open: MSensor_GetImageAddr is NULL")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_255_112_2_18_0_34_26_1952,"_met_camera_close()")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_309_112_2_18_0_34_26_1953,"MET_CMOTION_Init ok")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_335_112_2_18_0_34_26_1954,"met_cam_cmotion_close")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_358_112_2_18_0_34_26_1955,"met_cam_cmotion_judge_motion: \" Judge time = %d")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_362_112_2_18_0_34_26_1956,"MET_CAM_CMOTION MOVED! \"")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_393_112_2_18_0_34_26_1957,"met_cam_alloc_ext_mem : ptr 0x%x, size %d")
TRACE_MSG(MSENSOR_MEYE_YANGXUN_404_112_2_18_0_34_26_1958,"met_cam_free_ext_mem:prt 0x%x")
TRACE_MSG(MAGICVOICE_APP_115_112_2_18_0_34_26_1959," MV_DEMO_Open")
TRACE_MSG(MAGICVOICE_APP_136_112_2_18_0_34_27_1960," MV_DEMO_Close")
TRACE_MSG(MAGICVOICE_APP_167_112_2_18_0_34_27_1961,"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_CHANGE\r\n")
TRACE_MSG(MAGICVOICE_APP_181_112_2_18_0_34_27_1962,"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_ROLE\r\n")
TRACE_MSG(MAGICVOICE_APP_195_112_2_18_0_34_27_1963,"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_EFFECT\r\n")
TRACE_MSG(MAGICVOICE_APP_209_112_2_18_0_34_27_1964,"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_EQID\r\n")
TRACE_MSG(MAGICVOICE_APP_223_112_2_18_0_34_27_1965,"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_BGID\r\n")
TRACE_MSG(MAGICVOICE_APP_237_112_2_18_0_34_27_1966,"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_BGVolume\r\n")
TRACE_MSG(MAGICVOICE_APP_251_112_2_18_0_34_27_1967,"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_MVolume\r\n")
TRACE_MSG(MAGICVOICE_APP_258_112_2_18_0_34_27_1968,"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:paramid is invalid\r\n")
TRACE_MSG(MAGICVOICE_APP_312_112_2_18_0_34_27_1969,"MV_DEMO_Open_Resource: open res fail or no card")
TRACE_MSG(MAGICVOICE_APP_323_112_2_18_0_34_27_1970,"read bytes_read = %d")
TRACE_MSG(MAGICVOICE_APP_351_112_2_18_0_34_27_1971,"MV_DEMO_Open_Resource: open log fail or no card")
TRACE_MSG(MAGICVOICE_APP_442_112_2_18_0_34_27_1972,"s_mv_thread_id = 0x%X\r\n")
TRACE_MSG(INTEL_M18_DRV_706_112_2_18_0_34_32_1973,"INTEL_Write:Error Address or Length!")
TRACE_MSG(INTEL_M18_DRV_778_112_2_18_0_34_32_1974,"[DRV_INTEL_M18_DRV]<WARNING> addr is invalid\r\n")
TRACE_MSG(OCR_DRIVER_216_112_2_18_0_34_44_1975,"OCR_Init")
TRACE_MSG(OCR_DRIVER_221_112_2_18_0_34_44_1976,"OCR_Init: ocr has been init before or hasn't been closed")
TRACE_MSG(OCR_DRIVER_268_112_2_18_0_34_45_1977,"OCR_Preview")
TRACE_MSG(OCR_DRIVER_273_112_2_18_0_34_45_1978,"OCR_Preview: ocr has NOT been opened, 0x%x")
TRACE_MSG(OCR_DRIVER_281_112_2_18_0_34_45_1979,"OCR_Preview: OCR setting parameter error")
TRACE_MSG(OCR_DRIVER_289_112_2_18_0_34_45_1980,"OCR_Preview: OCR start preview error")
TRACE_MSG(OCR_DRIVER_310_112_2_18_0_34_45_1981,"OCR_SetPreviewParam")
TRACE_MSG(OCR_DRIVER_317_112_2_18_0_34_45_1982,"OCR_SetPreviewParam: ocr has NOT been opened, 0x%x")
TRACE_MSG(OCR_DRIVER_338_112_2_18_0_34_45_1983,"OCR_SetPreviewParam: OCR setting parameter error")
TRACE_MSG(OCR_DRIVER_359_112_2_18_0_34_45_1984,"OCR_StartPreview")
TRACE_MSG(OCR_DRIVER_364_112_2_18_0_34_45_1985,"OCR_StartPreview: ocr has NOT been opened, 0x%x")
TRACE_MSG(OCR_DRIVER_371_112_2_18_0_34_45_1986,"OCR_StartPreview: ocr is previewing")
TRACE_MSG(OCR_DRIVER_379_112_2_18_0_34_45_1987,"OCR_StartPreview: ocr start preview error")
TRACE_MSG(OCR_DRIVER_400_112_2_18_0_34_45_1988,"OCR_StopPreview")
TRACE_MSG(OCR_DRIVER_405_112_2_18_0_34_45_1989,"OCR_StopPreview: ocr has NOT been opened")
TRACE_MSG(OCR_DRIVER_412_112_2_18_0_34_45_1990,"OCR_StopPreview: ocr is not previewing")
TRACE_MSG(OCR_DRIVER_420_112_2_18_0_34_45_1991,"OCR_StopPreview: ocr stop preview error")
TRACE_MSG(OCR_DRIVER_441_112_2_18_0_34_45_1992,"OCR_DoFocus")
TRACE_MSG(OCR_DRIVER_446_112_2_18_0_34_45_1993,"OCR_DoFocus: ocr has NOT been opened")
TRACE_MSG(OCR_DRIVER_453_112_2_18_0_34_45_1994,"OCR_DoFocus: ocr is not previewing")
TRACE_MSG(OCR_DRIVER_476_112_2_18_0_34_45_1995,"OCR_Capture")
TRACE_MSG(OCR_DRIVER_481_112_2_18_0_34_45_1996,"OCR_Capture: ocr has NOT been opened : 0x%x")
TRACE_MSG(OCR_DRIVER_489_112_2_18_0_34_45_1997,"OCR_Capture: OCR set capture parameter error")
TRACE_MSG(OCR_DRIVER_497_112_2_18_0_34_45_1998,"OCR_Capture: OCR caputre error")
TRACE_MSG(OCR_DRIVER_519_112_2_18_0_34_45_1999,"OCR_SetCaptureParam")
TRACE_MSG(OCR_DRIVER_526_112_2_18_0_34_45_2000,"OCR_SetCaptureParams: ocr has NOT been opened : 0x%x")
TRACE_MSG(OCR_DRIVER_547_112_2_18_0_34_45_2001,"OCR_SetCaptureParams: set parameter error")
TRACE_MSG(OCR_DRIVER_564_112_2_18_0_34_45_2002,"OCR_DoCapture")
TRACE_MSG(OCR_DRIVER_569_112_2_18_0_34_45_2003,"OCR_DoCapture: ocr has NOT been opened : 0x%x")
TRACE_MSG(OCR_DRIVER_577_112_2_18_0_34_45_2004,"OCR_DoCapture: capture error")
TRACE_MSG(OCR_DRIVER_596_112_2_18_0_34_45_2005,"OCR_GetYUVbuf: %x, %d")
TRACE_MSG(OCR_DRIVER_630_112_2_18_0_34_45_2006,"OCR_SetReviewParam: %x")
TRACE_MSG(OCR_DRIVER_637_112_2_18_0_34_45_2007,"OCR_SetReviewParam: ocr has NOT been opened")
TRACE_MSG(OCR_DRIVER_668_112_2_18_0_34_45_2008,"OCR_SetReviewParam: set parameter error")
TRACE_MSG(OCR_DRIVER_699_112_2_18_0_34_46_2009,"OCR_DoReview")
TRACE_MSG(OCR_DRIVER_736_112_2_18_0_34_46_2010,"OCR_Close")
TRACE_MSG(OCR_DRIVER_741_112_2_18_0_34_46_2011,"OCR_Close: ocr has NOT been opened")
TRACE_MSG(OCR_DRIVER_771_112_2_18_0_34_46_2012,"OCR_SetOsdZoneMode: set ZONE mode error")
TRACE_MSG(OCR_DRIVER_801_112_2_18_0_34_46_2013,"OCR_SetOsdImage: set osd image error")
TRACE_MSG(OCR_DRIVER_827_112_2_18_0_34_46_2014,"OCR_SetOsdColorPalette: set osd color palette error")
TRACE_MSG(OCR_DRIVER_858_112_2_18_0_34_46_2015,"OCR_ModifyOsdImage: modify osd image error")
TRACE_MSG(OCR_DRIVER_879_112_2_18_0_34_46_2016,"OCR_SetOsdEnableMode: modify osd image error")
TRACE_MSG(OCR_DRIVER_900_112_2_18_0_34_46_2017,"s_st_ocr_context.p_buf = %x")
TRACE_MSG(OCR_DRIVER_956_112_2_18_0_34_46_2018,"OCR_GetLayoutRegion_lay %d: %d, %d, %d, %d")
TRACE_MSG(OCR_DRIVER_980_112_2_18_0_34_46_2019,"OCR_GetLayoutRegion_disp %d: %d, %d, %d, %d")
TRACE_MSG(OCR_DRIVER_1090_112_2_18_0_34_46_2020,"OCR_LCD_InvalidateImage: %d, %d, %d, %d, color_id:%d")
TRACE_MSG(THOCR_LIB_207_112_2_18_0_34_47_2021,"OCRlib_ThreadLayoutEntry: OCRlib_Layout......")
TRACE_MSG(THOCR_LIB_209_112_2_18_0_34_47_2022,"OCRlib_ThreadLayoutEntry: OCRlib_Layout done.")
TRACE_MSG(THOCR_LIB_248_112_2_18_0_34_47_2023,"ocr_layout_task_open,task_id = %d")
TRACE_MSG(THOCR_LIB_264_112_2_18_0_34_47_2024,"ocr_layout_task_close,task_id = %d")
TRACE_MSG(THOCR_LIB_270_112_2_18_0_34_47_2025,"ocr lib being init, can't stop task now, wait.")
TRACE_MSG(THOCR_LIB_277_112_2_18_0_34_47_2026,"ocr lib is open, ready to stop, wait index=%d.")
TRACE_MSG(THOCR_LIB_338_112_2_18_0_34_47_2027,"OCRlib_ThreadAutoRecogEntry: OCRlib_AutoRecog done.")
TRACE_MSG(THOCR_LIB_377_112_2_18_0_34_47_2028,"ocr_autorecog_task_open,task_id = %d")
TRACE_MSG(THOCR_LIB_392_112_2_18_0_34_47_2029,"ocr_autorecog_task_close,task_id = %d")
TRACE_MSG(THOCR_LIB_435_112_2_18_0_34_47_2030,"OCR:OCR_hSendMsg sig_code = %d")
TRACE_MSG(THOCR_LIB_550_112_2_18_0_34_48_2031,"OCRlib_Layout: OCRlib_Initialize.....")
TRACE_MSG(THOCR_LIB_554_112_2_18_0_34_48_2032,"OCRlib_Layout: OCRlib_Initialize done.")
TRACE_MSG(THOCR_LIB_558_112_2_18_0_34_48_2033,"OCRlib_Layout: ARM clock = %d")
TRACE_MSG(THOCR_LIB_564_112_2_18_0_34_48_2034,"OCRlib_Layout: running time = %d")
TRACE_MSG(THOCR_LIB_566_112_2_18_0_34_48_2035,"OCRlib_Layout: Num of Regions: %d.")
TRACE_MSG(THOCR_LIB_572_112_2_18_0_34_48_2036,"ocr layout failed, error code = %d")
TRACE_MSG(THOCR_LIB_598_112_2_18_0_34_48_2037,"OCRlib_AutoRecog: ARM clock = %d")
TRACE_MSG(THOCR_LIB_603_112_2_18_0_34_48_2038,"OCRlib_AutoRecog: region %d ...")
TRACE_MSG(THOCR_LIB_607_112_2_18_0_34_48_2039,"OCRlib_AutoRecog: left=%d top=%d right=%d bottom=%d")
TRACE_MSG(THOCR_LIB_730_112_2_18_0_34_48_2040,"OCRlib_AutoRecog: UNICODE (first 10) %d,%d,%d,%d,%d,%d,%d,%d,%d,%d")
TRACE_MSG(THOCR_LIB_732_112_2_18_0_34_48_2041,"OCRlib_AutoRecog: Class %d")
TRACE_MSG(THOCR_LIB_734_112_2_18_0_34_48_2042,"OCRlib_AutoRecog: region %d done.")
TRACE_MSG(THOCR_LIB_736_112_2_18_0_34_48_2043,"OCRlib_AutoRecog: running time = %d")
TRACE_MSG(THOCR_LIB_792_112_2_18_0_34_48_2044,"OCRlib_CallBackFunc: nState=%d, nInfo=%d\n")
TRACE_MSG(THOCR_LIB_868_112_2_18_0_34_48_2045,"OCRlib_Initialize: dumping y buffer...")
TRACE_MSG(THOCR_LIB_871_112_2_18_0_34_48_2046,"OCRlib_Initialize: dumping y buffer done.")
TRACE_MSG(THOCR_LIB_875_112_2_18_0_34_48_2047,"OCRlib_Initialize: dumping y buffer failed.")
TRACE_MSG(THOCR_LIB_880_112_2_18_0_34_48_2048,"OCRlib_Initialize: OCR LIB VER %d")
TRACE_MSG(THOCR_LIB_892_112_2_18_0_34_48_2049,"ocr initialize failed, error code = %d")
TRACE_MSG(THOCR_LIB_905_112_2_18_0_34_48_2050,"ocr set image failed, error code = %d")
TRACE_MSG(THOCR_LIB_973_112_2_18_0_34_49_2051,"OCRlib_RecogOneRegion: TH_RecogOneRegion...")
TRACE_MSG(THOCR_LIB_975_112_2_18_0_34_49_2052,"OCRlib_RecogOneRegion: TH_RecogOneRegion done.")
TRACE_MSG(THOCR_LIB_978_112_2_18_0_34_49_2053,"ocr recognition failed, error code = %d")
TRACE_MSG(THOCR_LIB_990_112_2_18_0_34_49_2054,"OCRlib_RecogOneRegion: TH_ClassifyOneRegionByKonwClass...")
TRACE_MSG(THOCR_LIB_998_112_2_18_0_34_49_2055,"OCRlib_RecogOneRegion: TH_ClassifyOneRegionByKonwClass done.")
TRACE_MSG(THOCR_LIB_1001_112_2_18_0_34_49_2056,"ocr classification failed, error code = %d")
TRACE_MSG(HX8347_SEINE_DRV_287_112_2_18_0_34_50_2057,"INDEX %u, LEN %u")
TRACE_MSG(HX8347_SEINE_DRV_290_112_2_18_0_34_50_2058,"Total len %u, index len %u")
TRACE_MSG(ILI9325_SEINE_DRV_289_112_2_18_0_34_51_2059,"INDEX %u, LEN %u")
TRACE_MSG(ILI9325_SEINE_DRV_292_112_2_18_0_34_51_2060,"Total len %u, index len %u")
TRACE_MSG(LGDP4531_SEINE_DRV_345_112_2_18_0_34_52_2061,"INDEX %u, LEN %u")
TRACE_MSG(LGDP4531_SEINE_DRV_348_112_2_18_0_34_52_2062,"Total len %u, index len %u")
TRACE_MSG(R61509V_SEINE_DRV_286_112_2_18_0_34_52_2063,"INDEX %u, LEN %u")
TRACE_MSG(R61509V_SEINE_DRV_289_112_2_18_0_34_52_2064,"Total len %u, index len %u")
TRACE_MSG(R61581_SEINE_DRV_337_112_2_18_0_34_53_2065,"INDEX %u, LEN %u")
TRACE_MSG(R61581_SEINE_DRV_340_112_2_18_0_34_53_2066,"Total len %u, index len %u")
TRACE_MSG(SPFD5420A_SEINE_DRV_305_112_2_18_0_34_54_2067,"INDEX %u, LEN %u")
TRACE_MSG(SPFD5420A_SEINE_DRV_308_112_2_18_0_34_54_2068,"Total len %u, index len %u")
TRACE_MSG(TP_FT5206_114_112_2_18_0_35_2_2069,"TP:: open I2C failed!\r\n")
TRACE_MSG(TP_FT5206_122_112_2_18_0_35_2_2070,"TP:: open I2C success!\r\n")
TRACE_MSG(TP_FT5206_241_112_2_18_0_35_2_2071,"[TSP] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n")
TRACE_MSG(TP_FT5206_259_112_2_18_0_35_2_2072,"[TSP] Step 4: erase. \n")
TRACE_MSG(TP_FT5206_263_112_2_18_0_35_2_2073,"[TSP] Step 5: start upgrade. \n")
TRACE_MSG(TP_FT5206_287_112_2_18_0_35_2_2074,"[TSP] upgrade the 0x%x th byte.\n")
TRACE_MSG(TP_FT5206_331_112_2_18_0_35_3_2075,"[TSP] Step 6:  ecc read 0x%x, new firmware 0x%x. \n")
TRACE_MSG(TP_FT5206_367_112_2_18_0_35_3_2076,"FW FIRMWARE_ID = 0x%x\n")
TRACE_MSG(TP_FT5206_378_112_2_18_0_35_3_2077,"TPC UPDATE FIRMWARE SUCCESS\n")
TRACE_MSG(TP_FT5206_382_112_2_18_0_35_3_2078,"TPC UPDATE FIRMWARE FAILED\n")
TRACE_MSG(TP_FT5206_427_112_2_18_0_35_3_2079,"TP:: XY_Coordinate Finger[%d] x=%d, y=%d, finger_id = 0x%x!rn")
TRACE_MSG(TP_FT5206_443_112_2_18_0_35_3_2080,"FT5206 Reg[0x%x]=0x%x \r\n")
TRACE_MSG(TP_FT5206_449_112_2_18_0_35_3_2081,"FT5206 Reg[0x80+%x]=0x%x \r\n")
TRACE_MSG(TP_FT5206_455_112_2_18_0_35_3_2082,"FT5206 Reg[0x92+%x]=0x%x \r\n")
TRACE_MSG(TP_FT5206_461_112_2_18_0_35_3_2083,"FT5206 Reg[0x9c+%x]=0x%x \r\n")
TRACE_MSG(TP_FT5206_467_112_2_18_0_35_3_2084,"FT5206 Reg[0xa6+%x]=0x%x \r\n")
TRACE_MSG(TP_FT5206_689_112_2_18_0_35_3_2085,"value freq_cycle time = 0x%x\n")
TRACE_MSG(TP_FT5206_698_112_2_18_0_35_3_2086,"value zoom_distance = 0x%x\n")
TRACE_MSG(TP_FT5206_748_112_2_18_0_35_3_2087,"TP: FT5206_Resume Retry Count =0x%x,value=0x%x \r\n")
TRACE_MSG(TP_MULTI_DRV_95_112_2_18_0_35_4_2088,"TPC LEN 0x%x\r\n")
TRACE_MSG(TP_MULTI_DRV_241_112_2_18_0_35_4_2089,"TPC: <Error>data is null\n")
TRACE_MSG(TP_MULTI_DRV_281_112_2_18_0_35_4_2090,"TP_DRIVER: enter into TP_Multi_MountGPIOHandler\n")
TRACE_MSG(TP_MULTI_DRV_451_112_2_18_0_35_5_2091," TPC:SIMULATOR, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d.")
TRACE_MSG(TP_MULTI_SRV_97_112_2_18_0_35_5_2092,"SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d.")
TRACE_MSG(TP_SRV_86_112_2_18_0_35_5_2093,"App_queue:\n")
TRACE_MSG(TPD_DRV_352_112_2_18_0_35_6_2094,"TP_DRIVER: enter into TP_DInit\n")
TRACE_MSG(TPD_DRV_357_112_2_18_0_35_6_2095,"********TP_DInit***********\r\n")
TRACE_MSG(TPD_DRV_392_112_2_18_0_35_7_2096,"*******TP_DInit finish********")
TRACE_MSG(TPD_DRV_531_112_2_18_0_35_7_2097,"TP_DRV: TP UP filter begin, %d!\n\r")
TRACE_MSG(TPD_DRV_566_112_2_18_0_35_7_2098,"g_adc_lt[%d].x=%d, g_adc_lt[%d].y=%d")
TRACE_MSG(TPD_DRV_579_112_2_18_0_35_7_2099,"TP_DRV: TP UP filter middle %d!\n\r")
TRACE_MSG(TPD_DRV_646_112_2_18_0_35_7_2100,"TP_DRV: TP UP filter finish!\n\r")
TRACE_MSG(TPD_DRV_662_112_2_18_0_35_7_2101,"TP_DRV: IsGpioMessage\n\r")
TRACE_MSG(TPD_DRV_678_112_2_18_0_35_7_2102,"TP_DRV: XL_state = %d\n\r")
TRACE_MSG(TPD_DRV_682_112_2_18_0_35_7_2103,"TP_DRV: Start to ADC\n\r")
TRACE_MSG(TPD_DRV_734_112_2_18_0_35_7_2104,"temp1.x = %d, temp1.y = %d\n\r")
TRACE_MSG(TPD_DRV_735_112_2_18_0_35_7_2105,"temp2.x = %d, temp2.y = %d\n\r")
TRACE_MSG(TPD_DRV_736_112_2_18_0_35_7_2106,"temp3.x = %d, temp3.y = %d\n\r")
TRACE_MSG(TPD_DRV_737_112_2_18_0_35_7_2107,"delta_x1 = %d, delta_y1 = %d\n\r")
TRACE_MSG(TPD_DRV_738_112_2_18_0_35_7_2108,"delta_x2 = %d, delta_y2= %d\n\r")
TRACE_MSG(TPD_DRV_739_112_2_18_0_35_7_2109,"delta_x3 = %d, delta_y3= %d\n\r")
TRACE_MSG(TPD_DRV_756_112_2_18_0_35_7_2110,"TP_DRV: ADC detected\n\r")
TRACE_MSG(TPD_DRV_762_112_2_18_0_35_7_2111,"TP_DRV: DOWN message, x=%d, y=%d\n\r")
TRACE_MSG(TPD_DRV_780_112_2_18_0_35_7_2112,"TP_DRV: MOVE message, x=%d, y=%d, time=%dms\n\r")
TRACE_MSG(TPD_DRV_790_112_2_18_0_35_7_2113,"TPDRV: UP action happened!  \n\r")
TRACE_MSG(TPD_DRV_796_112_2_18_0_35_7_2114,"TPDRV: UP message, x=%d, y=%d\n\r")
TRACE_MSG(TPD_DRV_817_112_2_18_0_35_7_2115,"TP_DRV: XL_state = %d\n\r")
TRACE_MSG(TPD_DRV_867_112_2_18_0_35_7_2116,"TP_UpdateCalibration:x_top_factor = %d,x_bottom_factor=%d,y_left_factor = %d,y_right_factor=%d ")
TRACE_MSG(TPD_DRV_884_112_2_18_0_35_8_2117,"TP_DRIVER: enter into TP_MountGPIOHandler\n")
TRACE_MSG(TPD_DRV_895_112_2_18_0_35_8_2118,"TP_DRIVER: TP_MountGPIOHandler SUCCES\n")
TRACE_MSG(TPD_DRV_918_112_2_18_0_35_8_2119,"TP_DRIVER: enter into TP_GPIOHandler\n")
TRACE_MSG(TPD_DRV_937_112_2_18_0_35_8_2120,"TP_DRIVER: PIN detect error in INT\n")
TRACE_MSG(TPD_DRV_1009_112_2_18_0_35_8_2121,"TPDRV: ADC x=%d, y=%d.\n\r")
TRACE_MSG(TPD_DRV_1017_112_2_18_0_35_8_2122,"TPDRV: Coordinate: x=%d, y=%d.\n\r")
TRACE_MSG(TPD_DRV_1066_112_2_18_0_35_8_2123,"TPDRV: Coordinate: x=%d, y=%d.\n\r")
TRACE_MSG(TPD_DRV_1069_112_2_18_0_35_8_2124,"TPDRV: TP_Calculate, pen is UP.\n\r")
TRACE_MSG(TPD_DRV_1240_112_2_18_0_35_8_2125,"SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d.")
TRACE_MSG(TPD_DRV_1248_112_2_18_0_35_8_2126,"SC6800: Register_Sensor_Tool_CmdRoutine")
TRACE_MSG(ATHR_WIFI_ADAPTER_707_112_2_18_0_35_11_2127,"WIFI:MAC ADDR =   %02X:%02X:%02X:%02X:%02X:%02Xn")
TRACE_MSG(ATHR_WIFI_ADAPTER_920_112_2_18_0_35_11_2128,":%s(%s)\n")
TRACE_MSG(ATHR_WIFI_ADAPTER_929_112_2_18_0_35_11_2129,":%s(%s)\n")
TRACE_MSG(ATHR_WIFI_ADAPTER_950_112_2_18_0_35_11_2130,":%s(%s)\n")
TRACE_MSG(ATHR_WIFI_ADAPTER_1058_112_2_18_0_35_11_2131,"length not right, should be %d\n")
TRACE_MSG(ATHR_WIFI_ADAPTER_1073_112_2_18_0_35_11_2132,"length not right, should be %d\n")
TRACE_MSG(WIFISUPP_EUT_AR6302_104_112_2_18_0_35_12_2133,"WIFISUPP_EUTSetTxFactor pcl=%d, tx_factor=%d")
TRACE_MSG(CSR_IP_ETHER_SEF_373_112_2_18_0_35_13_2134,"STACK: current stack ptr: 0x%08X | stack_size = %d")
TRACE_MSG(CSR_SPI_MASTER_105_112_2_18_0_35_15_2135,"CSR: %s - No ISR handlers registered")
TRACE_MSG(CSR_SPI_MASTER_120_112_2_18_0_35_15_2136,"CsrSpiMasterInitialise")
TRACE_MSG(CSR_SPI_MASTER_180_112_2_18_0_35_15_2137,"Failed to Open SPI Port")
TRACE_MSG(CSR_SPI_MASTER_189_112_2_18_0_35_15_2138,"Failed to Init SPI Port")
TRACE_MSG(CSR_SPI_MASTER_238_112_2_18_0_35_15_2139,"SPI Port initialised")
TRACE_MSG(CSR_SPI_MASTER_246_112_2_18_0_35_15_2140,"CsrSpiMasterDeinitialise")
TRACE_MSG(CSR_SPI_MASTER_347_112_2_18_0_35_15_2141,"%s r %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:")
TRACE_MSG(CSR_SPI_MASTER_358_112_2_18_0_35_15_2142,"%s w %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:")
TRACE_MSG(CSR_SPI_MASTER_373_112_2_18_0_35_15_2143,"%s r %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:")
TRACE_MSG(CSR_SPI_MASTER_383_112_2_18_0_35_15_2144,"%s w %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:")
TRACE_MSG(CSR_SPI_MASTER_593_112_2_18_0_35_16_2145,"CSR: %s - NULL data pointer!")
TRACE_MSG(CSR_SPI_MASTER_760_112_2_18_0_35_16_2146,"CsrSpiMasterBusClockFrequencySet: %d")
TRACE_MSG(CSR_SPI_MASTER_779_112_2_18_0_35_16_2147,"CsrSpiMasterCallbackRegister")
TRACE_MSG(CSR_SPI_MASTER_790_112_2_18_0_35_16_2148,"CsrSpiMasterCallbackRegister End")
TRACE_MSG(SDIO_TEST_54_112_2_18_0_35_21_2149,"%s")
TRACE_MSG(SDIO_TEST_57_112_2_18_0_35_21_2150,"%02X:")
TRACE_MSG(SDIO_TEST_59_112_2_18_0_35_21_2151,"\n")
TRACE_MSG(SDIO_TEST_118_112_2_18_0_35_22_2152,"Waiting for SDIO functions to register: %d\n")
TRACE_MSG(SDIO_TEST_126_112_2_18_0_35_22_2153,"Device information:\n")
TRACE_MSG(SDIO_TEST_127_112_2_18_0_35_22_2154,"Manufacturer: 0x%04X\n")
TRACE_MSG(SDIO_TEST_128_112_2_18_0_35_22_2155,"Card: 0x%04X\n")
TRACE_MSG(SDIO_TEST_129_112_2_18_0_35_22_2156,"Function: %d\n")
TRACE_MSG(SDIO_TEST_130_112_2_18_0_35_22_2157,"Standard Interface: %d\n")
TRACE_MSG(SDIO_TEST_131_112_2_18_0_35_22_2158,"Blocksize: %d\n")
TRACE_MSG(SDIO_TEST_132_112_2_18_0_35_22_2159,"Features:0x%08X\n")
TRACE_MSG(SDIO_TEST_141_112_2_18_0_35_22_2160,"Testing enable function %d\n")
TRACE_MSG(SDIO_TEST_153_112_2_18_0_35_22_2161,"Enable function is verified\n")
TRACE_MSG(SDIO_TEST_161_112_2_18_0_35_22_2162,"Testing disable function %d\n")
TRACE_MSG(SDIO_TEST_173_112_2_18_0_35_22_2163,"Disable function is verified\n")
TRACE_MSG(SDIO_TEST_180_112_2_18_0_35_22_2164,"Testing set frequency, function %d, frequency %d\n")
TRACE_MSG(SDIO_TEST_192_112_2_18_0_35_22_2165,"validateBuffer failed at byte %d\n")
TRACE_MSG(SDIO_TEST_216_112_2_18_0_35_22_2166,"Testing CsrSdioRead/Write length %d, totalTestCount: %d\n")
TRACE_MSG(SDIO_TEST_227_112_2_18_0_35_22_2167,"Writing remainder, which is %d bytes\n")
TRACE_MSG(SDIO_TEST_236_112_2_18_0_35_22_2168,"Reading remainder, which is %d bytes\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_75_112_2_18_0_35_58_2169,"Error, %d, NOT-SUPPORTED\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_162_112_2_18_0_35_58_2170,"+++++ 0x%X, Result=%d +++++\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_176_112_2_18_0_35_58_2171,"PtestChipID: 0x%04X\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_186_112_2_18_0_35_58_2172,"PtestFwBuildId: %s\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_481_112_2_18_0_35_59_2173,"last rx-result info\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_482_112_2_18_0_35_59_2174,"  good:  %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_483_112_2_18_0_35_59_2175,"   bad: %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_484_112_2_18_0_35_59_2176,"total:    %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_486_112_2_18_0_35_59_2177,"   PER:  %d %%\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_525_112_2_18_0_35_59_2178,"<WIFISUPP_EUTSetChannel = %d>\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_539_112_2_18_0_35_59_2179,"<WIFISUPP_EUTSetDataRatio, gTestRate = %d (long ratio=%d)>\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_562_112_2_18_0_35_59_2180,"<WIFISUPP_EUTSetMode = %d>\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_576_112_2_18_0_35_59_2181,"<WIFISUPP_EUTSetTxFactor, pcl = %d, tx_factor = %d>\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_589_112_2_18_0_35_59_2182,"<WIFISUPP_EUTSetPCL, pcl = %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_602_112_2_18_0_35_59_2183,"<WIFISUPP_EUTSetTx, is_tx_on = %d, tx_packet_mode = %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_629_112_2_18_0_35_59_2184,"<WIFISUPP_EUTSetRx, rx_command = %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6027_712_112_2_18_0_35_59_2185,"CSR:WIFI_GetVersion - %s")
TRACE_MSG(CSR_IP_ETHER_SEF_270_112_2_18_0_36_0_2186,"STACK: current stack ptr: 0x%08X | stack_size = %d")
TRACE_MSG(CSR_SPI_MASTER_88_112_2_18_0_36_1_2187,"CsrSpiMasterInitialise")
TRACE_MSG(CSR_SPI_MASTER_111_112_2_18_0_36_1_2188,"Failed to Open SPI Port")
TRACE_MSG(CSR_SPI_MASTER_124_112_2_18_0_36_2_2189,"SPI Port initialised")
TRACE_MSG(CSR_SPI_MASTER_130_112_2_18_0_36_2_2190,"CsrSpiMasterDeinitialise")
TRACE_MSG(CSR_SPI_MASTER_194_112_2_18_0_36_2_2191,"CsrSpiMasterReadAsync")
TRACE_MSG(CSR_SPI_MASTER_195_112_2_18_0_36_2_2192,"Not implemented")
TRACE_MSG(CSR_SPI_MASTER_207_112_2_18_0_36_2_2193,"CsrSpiMasterWriteAsync")
TRACE_MSG(CSR_SPI_MASTER_208_112_2_18_0_36_2_2194,"Not implemented")
TRACE_MSG(CSR_SPI_MASTER_230_112_2_18_0_36_2_2195,"CsrSpiMasterBusClockFrequencySet: %d")
TRACE_MSG(CSR_SPI_MASTER_248_112_2_18_0_36_2_2196,"CsrSpiMasterCallbackRegister")
TRACE_MSG(CSR_SPI_MASTER_286_112_2_18_0_36_2_2197,"CsrSpiMasterCallbackRegister End")
TRACE_MSG(WIFISUPP_EUT_CSR6030_75_112_2_18_0_36_14_2198,"Error, %d, NOT-SUPPORTED\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_162_112_2_18_0_36_15_2199,"+++++ 0x%X, Result=%d +++++\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_176_112_2_18_0_36_15_2200,"PtestChipID: 0x%04X\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_186_112_2_18_0_36_15_2201,"PtestFwBuildId: %s\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_488_112_2_18_0_36_15_2202,"last rx-result info\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_489_112_2_18_0_36_15_2203,"  good:  %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_490_112_2_18_0_36_15_2204,"   bad: %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_491_112_2_18_0_36_15_2205,"total:    %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_493_112_2_18_0_36_15_2206,"   PER:  %d %%\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_532_112_2_18_0_36_16_2207,"<WIFISUPP_EUTSetChannel = %d>\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_546_112_2_18_0_36_16_2208,"<WIFISUPP_EUTSetDataRatio, gTestRate = %d (long ratio=%d)>\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_569_112_2_18_0_36_16_2209,"<WIFISUPP_EUTSetMode = %d>\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_583_112_2_18_0_36_16_2210,"<WIFISUPP_EUTSetTxFactor, pcl = %d, tx_factor = %d>\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_596_112_2_18_0_36_16_2211,"<WIFISUPP_EUTSetPCL, pcl = %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_609_112_2_18_0_36_16_2212,"<WIFISUPP_EUTSetTx, is_tx_on = %d, tx_packet_mode = %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_636_112_2_18_0_36_16_2213,"<WIFISUPP_EUTSetRx, rx_command = %d\r\n")
TRACE_MSG(WIFISUPP_EUT_CSR6030_719_112_2_18_0_36_16_2214,"CSR:WIFI_GetVersion - %s")
TRACE_MSG(RTL8723_WIFI_ADAPTER_291_112_2_18_0_36_17_2215,"&&&&&&&&rtl_createThread(): priority is %d\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_470_112_2_18_0_36_17_2216,"MAC ADDR =   %02X:%02X:%02X:%02X:%02X:%02Xn")
TRACE_MSG(RTL8723_WIFI_ADAPTER_541_112_2_18_0_36_17_2217,"======>rtl8723_supp_notifyOnCnf result=%d, pResult->net_id is 0x%x")
TRACE_MSG(RTL8723_WIFI_ADAPTER_566_112_2_18_0_36_17_2218,"<=======rtl8723_supp_notifyOnCnf\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_577_112_2_18_0_36_17_2219,"WIFI para_supp_notifyOffCnf result=%d")
TRACE_MSG(RTL8723_WIFI_ADAPTER_593_112_2_18_0_36_17_2220,"WIFI para_supp_notifyConnectCnf result=%d")
TRACE_MSG(RTL8723_WIFI_ADAPTER_617_112_2_18_0_36_17_2221,"WIFI para_supp_notifyScanInd")
TRACE_MSG(RTL8723_WIFI_ADAPTER_646_112_2_18_0_36_17_2222,"WIFI param_supp_notifyScanCnf")
TRACE_MSG(RTL8723_WIFI_ADAPTER_661_112_2_18_0_36_17_2223,"WIFI param_supp_notifyDisconnectCnf result=%d")
TRACE_MSG(RTL8723_WIFI_ADAPTER_680_112_2_18_0_36_17_2224,"WIFI param_supp_notifyDisconnectInd")
TRACE_MSG(RTL8723_WIFI_ADAPTER_748_112_2_18_0_36_17_2225,"wifi: para_GetMAC is %x-%x-%x-%x-%x-%x")
TRACE_MSG(RTL8723_WIFI_ADAPTER_789_112_2_18_0_36_17_2226,"wifi: para_GetMAC is %x-%x-%x-%x-%x-%x, ret=%d")
TRACE_MSG(RTL8723_WIFI_ADAPTER_805_112_2_18_0_36_18_2227,"&&&&&&&&&rtl8723_wifi_supplicant_turn_on()\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_809_112_2_18_0_36_18_2228,"&&&&&&&&&&&&Failed to Open SPI Port\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_817_112_2_18_0_36_18_2229,"rtl8723_wifi_supplicant_turn_on wifi_hw_open_func fail\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_822_112_2_18_0_36_18_2230,"&&&&&rtl8723_wifi_supplicant_turn_on(): netid is 0x%x")
TRACE_MSG(RTL8723_WIFI_ADAPTER_900_112_2_18_0_36_18_2231,"&&&&&&&&&rtl8723_wifi_on()\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_908_112_2_18_0_36_18_2232,"&&&&&&&&&rtl8723_wifi_off()\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_923_112_2_18_0_36_18_2233,"&&&&&&&&&rtl8723_wifi_connect()\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_929_112_2_18_0_36_18_2234,"&&&&&&&&&rtl8723_wifi_disconnect()\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_935_112_2_18_0_36_18_2235,"&&&&&&&&&rtl8723_handle_cus_msg()\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_940_112_2_18_0_36_18_2236,"&&&&&&&&&rtl8723_get_connection_info()\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_946_112_2_18_0_36_18_2237,"&&&&&&&&&para_getThread_Identify()\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_993_112_2_18_0_36_18_2238,":%s(%d) str = %s \n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1005_112_2_18_0_36_18_2239,":%s(%d) str = %s\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1017_112_2_18_0_36_18_2240,":%s(%d) str = %s\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1050_112_2_18_0_36_18_2241,":%s(%s)\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1055_112_2_18_0_36_18_2242,":%s(%s)\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1075_112_2_18_0_36_18_2243,":%s(%s)\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1081_112_2_18_0_36_18_2244,":%s(%s)\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1087_112_2_18_0_36_18_2245,":%s(%s)\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1136_112_2_18_0_36_18_2246,"length not right, should be %d\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1151_112_2_18_0_36_18_2247,"length not right, should be %d\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1216_112_2_18_0_36_18_2248,"rtl8723_set_eut_mode enter =====>\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1224_112_2_18_0_36_18_2249,"rtl8723_set_eut_mode leave =====>\n")
TRACE_MSG(RTL8723_WIFI_ADAPTER_1237_112_2_18_0_36_18_2250,"&&&&&&&&&&&&&WIFI_GetVersion: is %s\n")
TRACE_MSG(WIFISUPP_ADAP_REALTEK_116_112_2_18_0_36_19_2251,"&&&&&&&&&Rtl8723WifiOn()\n")
TRACE_MSG(WIFISUPP_ADAP_REALTEK_126_112_2_18_0_36_19_2252,"&&&&&&&&&Rtl8723WifiOff()\n")
TRACE_MSG(WIFISUPP_ADAP_REALTEK_136_112_2_18_0_36_19_2253,"&&&&&&&&&Rtl8723WifiScan()\n")
TRACE_MSG(WIFISUPP_ADAP_REALTEK_146_112_2_18_0_36_19_2254,"&&&&&&&&&Rtl8723WifiConnect()\n")
TRACE_MSG(WIFISUPP_ADAP_REALTEK_156_112_2_18_0_36_19_2255,"&&&&&&&&&Rtl8723WifiDisconnect()\n")
TRACE_MSG(WIFISUPP_ADAP_REALTEK_166_112_2_18_0_36_19_2256,"&&&&&&&&&Rtl8723HandleCusmsg()\n")
TRACE_MSG(WIFISUPP_ADAP_REALTEK_179_112_2_18_0_36_19_2257,"&&&&&&&&&Rtl8723WifiGetConnectionInfo()\n")
TRACE_MSG(WIFISUPP_EUT_REALTEK_58_112_2_18_0_36_19_2258,"WIFISUPP_EUTEnterMode %d\n")
TRACE_MSG(WIFISUPP_EUT_REALTEK_107_112_2_18_0_36_19_2259,"WIFISUPP_EUTSetTxFactor pcl=%d, tx_factor=%d")
TRACE_MSG(WIFISUPP_EUT_REALTEK_201_112_2_18_0_36_19_2260,"WIFISUPP_EUTCusData\n")
TRACE_MSG(WIFI_DRV_101_112_2_18_0_36_20_2261,"_WIFI_SPI_Open(...)")
TRACE_MSG(WIFI_DRV_137_112_2_18_0_36_20_2262,"_WIFI_SPI_Open Open failed!!")
TRACE_MSG(WIFI_DRV_157_112_2_18_0_36_20_2263,"_WIFI_SPI_Read: port_num = %d, buf_ptr = 0x%x, lenght = %d")
TRACE_MSG(WIFI_DRV_167_112_2_18_0_36_20_2264,"WIFI_SPI_1 read failed!!")
TRACE_MSG(WIFI_DRV_187_112_2_18_0_36_20_2265,"_WIFI_SPI_Write: port_num = %d, buf_ptr = 0x%x, lenght = %d")
TRACE_MSG(WIFI_DRV_197_112_2_18_0_36_20_2266,"WIFI_SPI_1 write failed!!")
TRACE_MSG(WIFI_DRV_215_112_2_18_0_36_20_2267,"_WIFI_SPI_ReadWrite: port_num = %d, buf_ptr_in = 0x%x, buf_ptr_out = 0x%x, lenght = %d")
TRACE_MSG(WIFI_DRV_221_112_2_18_0_36_20_2268,"WIFI_SPI_1 write failed!!")
TRACE_MSG(WIFI_DRV_238_112_2_18_0_36_20_2269,"_WIFI_SPI_Write_Asyn: port_num = %d, buf_ptr = 0x%x, lenght = %d,read_call_func=%p")
TRACE_MSG(WIFI_DRV_244_112_2_18_0_36_20_2270,"WIFI_SPI_1 write failed!!")
TRACE_MSG(WIFI_DRV_261_112_2_18_0_36_20_2271,"_WIFI_SPI_Read_Asyn: port_num = %d, buf_ptr = 0x%x, lenght = %d")
TRACE_MSG(WIFI_DRV_267_112_2_18_0_36_20_2272,"WIFI_SPI_1 write failed!!")
TRACE_MSG(WIFI_DRV_280_112_2_18_0_36_21_2273,"_WIFI_SPI_Close: port_num = %d")
TRACE_MSG(WIFI_DRV_526_112_2_18_0_36_21_2274,"s_wifi_port.WIFI_Read_Asyn")
TRACE_MSG(WIFI_DRV_534_112_2_18_0_36_21_2275,"WIFI_Read_Asyn")
TRACE_MSG(WIFI_DRV_555_112_2_18_0_36_21_2276,"s_wifi_port.write_asyn")
TRACE_MSG(WIFI_DRV_563_112_2_18_0_36_21_2277,"WIFI_RET_STATUS_ERR")
TRACE_MSG(WIFI_DRV_739_112_2_18_0_36_22_2278,"wifi WIFI_GetNvParam:%x%x%x%x%x%x")
TRACE_MSG(WIFISUPP_API_85_112_2_18_0_36_22_2279,"WIFISUPP WIFISUPP_OnReq, invalid thread id!")
TRACE_MSG(WIFISUPP_API_92_112_2_18_0_36_22_2280,"WIFISUPP WIFISUPP_OnReq, alloc sig_ptr fail!")
TRACE_MSG(WIFISUPP_API_114_112_2_18_0_36_22_2281,"WIFISUPP WIFISUPP_OffReq, alloc sig_ptr fail!")
TRACE_MSG(WIFISUPP_API_132_112_2_18_0_36_22_2282,"WIFI:error:SUPP:%s scan_info_ptr is PNULL!")
TRACE_MSG(WIFISUPP_API_140_112_2_18_0_36_22_2283,"WIFI:error:SUPP:%s alloc sig_ptr fail!")
TRACE_MSG(WIFISUPP_API_164_112_2_18_0_36_22_2284,"WIFISUPP WIFISUPP_ConnectReq, invalid input!")
TRACE_MSG(WIFISUPP_API_171_112_2_18_0_36_22_2285,"WIFISUPP WIFISUPP_ConnectReq, alloc sig_ptr fail!")
TRACE_MSG(WIFISUPP_API_194_112_2_18_0_36_22_2286,"WIFISUPP WIFISUPP_DisconnectReq, invalid input!")
TRACE_MSG(WIFISUPP_API_201_112_2_18_0_36_22_2287,"WIFISUPP WIFISUPP_DisconnectReq, alloc sig_ptr fail!")
TRACE_MSG(WIFISUPP_API_255_112_2_18_0_36_22_2288,"WIFISUPP SerializeConnectReq tls using buffer alloc fail")
TRACE_MSG(WIFISUPP_API_275_112_2_18_0_36_22_2289,"WIFISUPP SerializeConnectReq tls using file alloc fail")
TRACE_MSG(WIFISUPP_API_294_112_2_18_0_36_22_2290,"WIFISUPP SerializeConnectReq fast using buffer alloc fail")
TRACE_MSG(WIFISUPP_API_317_112_2_18_0_36_23_2291,"WIFISUPP SerializeConnectReq fast using file alloc fail")
TRACE_MSG(WIFISUPP_API_330_112_2_18_0_36_23_2292,"WIFISUPP SerializeConnectReq other eap alloc fail")
TRACE_MSG(WIFISUPP_API_343_112_2_18_0_36_23_2293,"WIFISUPP SerializeConnectReq wpa psk alloc fail")
TRACE_MSG(WIFISUPP_API_362_112_2_18_0_36_23_2294,"WIFI:error:SUPP:%s as using buffer alloc fail")
TRACE_MSG(WIFISUPP_API_384_112_2_18_0_36_23_2295,"WIFISUPP SerializeConnectReq wep alloc fail")
TRACE_MSG(WIFISUPP_API_407_112_2_18_0_36_23_2296,"WIFI:error:Supp:GetMac failed %d!")
TRACE_MSG(WIFISUPP_API_414_112_2_18_0_36_23_2297,"WIFI:info:Supp:GetMAC is %x-%x-%x-%x-%x-%x")
TRACE_MSG(WIFISUPP_EUT_256_112_2_18_0_36_23_2298,"req_ptr->len=%d\n")
TRACE_MSG(WIFISUPP_EUT_259_112_2_18_0_36_23_2299,"WIFISUPP_EUTHandleReq req_ptr->len > WIFISUPP_COMMAND_MAX_LEN")
TRACE_MSG(WIFISUPP_EUT_267_112_2_18_0_36_23_2300,"wifi: WIFISUPP_EUTHandleReq command_code = %d")
TRACE_MSG(WIFISUPP_EUT_281_112_2_18_0_36_23_2301,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_ENTER_MODE bEnterOrLeave=%d")
TRACE_MSG(WIFISUPP_EUT_319_112_2_18_0_36_23_2302,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_CHANNEL channel=%x")
TRACE_MSG(WIFISUPP_EUT_333_112_2_18_0_36_23_2303,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_DATA_RATIO ratio=%x")
TRACE_MSG(WIFISUPP_EUT_346_112_2_18_0_36_23_2304,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_MODE mode=%x")
TRACE_MSG(WIFISUPP_EUT_360_112_2_18_0_36_23_2305,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_PCL pcl=%x")
TRACE_MSG(WIFISUPP_EUT_376_112_2_18_0_36_24_2306,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_TX tx_on_off=%x")
TRACE_MSG(WIFISUPP_EUT_377_112_2_18_0_36_24_2307,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_TX tx_packet_mode=%x")
TRACE_MSG(WIFISUPP_EUT_391_112_2_18_0_36_24_2308,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_RX rx_command=%x")
TRACE_MSG(WIFISUPP_EUT_397_112_2_18_0_36_24_2309,"WIFISUPP_EUT_CMD_READ_RX_VAL")
TRACE_MSG(WIFISUPP_EUT_419_112_2_18_0_36_24_2310,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_TXFACTOR tx_factor=%x")
TRACE_MSG(WIFISUPP_EUT_428_112_2_18_0_36_24_2311,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_CUSTOM_DATA:  len is %d\n")
TRACE_MSG(WIFISUPP_EUT_447_112_2_18_0_36_24_2312,"WIFISUPP_EUT_CMD_READ_CALDATA")
TRACE_MSG(WIFISUPP_EUT_455_112_2_18_0_36_24_2313,"WIFISUPP_EUT_CMD_READ_CALDATA")
TRACE_MSG(WIFISUPP_EUT_484_112_2_18_0_36_24_2314,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_READ_REGISTER addr=%x my_to_read=%x")
TRACE_MSG(WIFISUPP_EUT_492_112_2_18_0_36_24_2315,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_READ_REGISTER failed")
TRACE_MSG(WIFISUPP_EUT_513_112_2_18_0_36_24_2316,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_WRITE_REGISTER addr1=%x write_temp=%x")
TRACE_MSG(WIFISUPP_EUT_524_112_2_18_0_36_24_2317,"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_WRITE_REGISTER failed %d")
TRACE_MSG(WIFISUPP_EUT_567_112_2_18_0_36_24_2318,"WIFISUPP_CheckStatus context_ptr == PNULL")
TRACE_MSG(WIFISUPP_EUT_833_112_2_18_0_36_24_2319,":%x\n")
TRACE_MSG(WIFISUPP_EUT_863_112_2_18_0_36_24_2320,"WIFISUPP_EUTSendCnf command_code=%d")
TRACE_MSG(WIFISUPP_EUT_888_112_2_18_0_36_25_2321,"WIFISUPP_EUTSendCnf command_code=%d")
TRACE_MSG(WIFISUPP_EUT_906_112_2_18_0_36_25_2322,"WIFISUPP_EUTSendCnf command_code=%d")
TRACE_MSG(WIFISUPP_EUT_954_112_2_18_0_36_25_2323,"WIFISUPP_EUTSendCnf command_code=%d")
TRACE_MSG(WIFISUPP_EUT_1037_112_2_18_0_36_25_2324,"WIFISUPP_EUTSyncFinished context_ptr == PNULL")
TRACE_MSG(WIFISUPP_EUT_1129_112_2_18_0_36_25_2325,"WIFISUPP_EUTReadRxValCnf context_ptr == PNULL")
TRACE_MSG(WIFISUPP_EUT_1154_112_2_18_0_36_25_2326,"WIFISUPP_EUTReadCalDataCnf context_ptr == PNULL")
TRACE_MSG(WIFISUPP_EUT_1174_112_2_18_0_36_25_2327,"WIFISUPP_EUTReadRxValCnf context_ptr == PNULL")
TRACE_MSG(WIFISUPP_MAIN_104_112_2_18_0_36_25_2328,"WIFISUPP creat thread: %x")
TRACE_MSG(WIFISUPP_MAIN_123_112_2_18_0_36_25_2329,"WIFISUPP SUPPADAP_SendSig msg_info_ptr==0!")
TRACE_MSG(WIFISUPP_MAIN_128_112_2_18_0_36_25_2330,"WIFISUPP SUPPADAP_SendSig send_to_thread_id==0!")
TRACE_MSG(WIFISUPP_MAIN_134_112_2_18_0_36_25_2331,"SUPPMAIN_NotifyApp WIFISUPP_SIG_ON_CNF")
TRACE_MSG(WIFISUPP_MAIN_142_112_2_18_0_36_25_2332,"SUPPMAIN_NotifyApp WIFISUPP_SIG_OFF_CNF")
TRACE_MSG(WIFISUPP_MAIN_150_112_2_18_0_36_25_2333,"SUPPMAIN_NotifyApp WIFISUPP_SIG_SCAN_IND")
TRACE_MSG(WIFISUPP_MAIN_158_112_2_18_0_36_25_2334,"SUPPMAIN_NotifyApp WIFISUPP_SIG_SCAN_CNF")
TRACE_MSG(WIFISUPP_MAIN_166_112_2_18_0_36_25_2335,"SUPPMAIN_NotifyApp WIFISUPP_SIG_CONNECT_CNF")
TRACE_MSG(WIFISUPP_MAIN_174_112_2_18_0_36_25_2336,"SUPPMAIN_NotifyApp WIFISUPP_SIG_DISCONNECT_CNF")
TRACE_MSG(WIFISUPP_MAIN_182_112_2_18_0_36_26_2337,"SUPPMAIN_NotifyApp WIFISUPP_SIG_DISCONNECT_IND")
TRACE_MSG(WIFISUPP_MAIN_190_112_2_18_0_36_26_2338,"SUPPMAIN_NotifyApp WIFISUPP_SIG_BSSID_CHANGED_IND")
TRACE_MSG(WIFISUPP_MAIN_199_112_2_18_0_36_26_2339,"SUPPMAIN_NotifyApp WIFISUPP_SIG_RSSI_CHANGED_IND")
TRACE_MSG(WIFISUPP_MAIN_207_112_2_18_0_36_26_2340,"WIFISUPP CreatWifiMsg, invalid msg id!")
TRACE_MSG(WIFISUPP_MAIN_246_112_2_18_0_36_26_2341,"WifiSuppTaskEntry, id:%x")
TRACE_MSG(WIFISUPP_MAIN_252_112_2_18_0_36_26_2342,"WIFISUPP task entry: Customized signal received,id:%x")
TRACE_MSG(WIFISUPP_MAIN_264_112_2_18_0_36_26_2343,"WIFISUPP task entry: WIFISUPP_SIG_ON_REQ,app:%d")
TRACE_MSG(WIFISUPP_MAIN_269_112_2_18_0_36_26_2344,"WIFISUPP task entry: WIFISUPP_SIG_OFF_REQ")
TRACE_MSG(WIFISUPP_MAIN_273_112_2_18_0_36_26_2345,"WIFISUPP task entry: WIFISUPP_SIG_SCAN_REQ")
TRACE_MSG(WIFISUPP_MAIN_286_112_2_18_0_36_26_2346,"WIFISUPP task entry: WIFISUPP_SIG_CONNECT_REQ")
TRACE_MSG(WIFISUPP_MAIN_295_112_2_18_0_36_26_2347,"WIFISUPP task entry: WIFISUPP_SIG_DISCONNECT_REQ")
TRACE_MSG(WIFISUPP_MAIN_300_112_2_18_0_36_26_2348,"WIFISUPP task entry: unwanted signal received!")
TRACE_MSG(WIFISUPP_MAIN_314_112_2_18_0_36_26_2349,"WifiSupp_main.c::WifiOn called")
TRACE_MSG(WIFISUPP_MAIN_318_112_2_18_0_36_26_2350,"WIFISUPP WifiOn, custom supplicant info NOT available!")
TRACE_MSG(WIFISUPP_MAIN_323_112_2_18_0_36_26_2351,"WIFISUPP WifiOn, app thread is null!")
TRACE_MSG(WIFISUPP_MAIN_328_112_2_18_0_36_26_2352,"WIFISUPP WifiOn, wifi_on_func is null!")
TRACE_MSG(WIFISUPP_MAIN_344_112_2_18_0_36_26_2353,"WIFISUPP WifiOff, custom supplicant info NOT available!")
TRACE_MSG(WIFISUPP_MAIN_349_112_2_18_0_36_26_2354,"WIFISUPP WifiOff, wifi_off_func is null!")
TRACE_MSG(WIFISUPP_MAIN_365_112_2_18_0_36_26_2355,"WIFI:info:SUPP:%s invalid input!")
TRACE_MSG(WIFISUPP_MAIN_369_112_2_18_0_36_26_2356,"WIFI:info:SUPP:%s ssid_len=%d")
TRACE_MSG(WIFISUPP_MAIN_371_112_2_18_0_36_26_2357,"WIFI:info:SUPP:%s ESSID =%s")
TRACE_MSG(WIFISUPP_MAIN_375_112_2_18_0_36_26_2358,"WIFI:error:SUPP:%s custom supplicant info NOT available!")
TRACE_MSG(WIFISUPP_MAIN_381_112_2_18_0_36_26_2359,"WIFI:error:SUPP:%s wifi_scan_func is null!")
TRACE_MSG(WIFISUPP_MAIN_398_112_2_18_0_36_26_2360,"WIFISUPP WifiConnect, invalid input!")
TRACE_MSG(WIFISUPP_MAIN_403_112_2_18_0_36_26_2361,"WifiConnect: ESSID =%s")
TRACE_MSG(WIFISUPP_MAIN_405_112_2_18_0_36_26_2362,"WifiConnect: BSSID: %02X:%02X:%02X:%02X:%02X:%02X")
TRACE_MSG(WIFISUPP_MAIN_406_112_2_18_0_36_26_2363,"WifiConnect encryp type =%d")
TRACE_MSG(WIFISUPP_MAIN_407_112_2_18_0_36_26_2364,"WifiConnect network_mode =%d")
TRACE_MSG(WIFISUPP_MAIN_408_112_2_18_0_36_26_2365,"WifiConnect chanel =%d")
TRACE_MSG(WIFISUPP_MAIN_409_112_2_18_0_36_26_2366,"WifiConnect pairwise_cipher =%d")
TRACE_MSG(WIFISUPP_MAIN_410_112_2_18_0_36_26_2367,"WifiConnect group_cipher =%d")
TRACE_MSG(WIFISUPP_MAIN_413_112_2_18_0_36_26_2368,"WifiConnect WEP type =%d")
TRACE_MSG(WIFISUPP_MAIN_417_112_2_18_0_36_26_2369,"WifiConnect WEP 64bit key 1 = %x,%x,%x,%x,%x")
TRACE_MSG(WIFISUPP_MAIN_420_112_2_18_0_36_26_2370,"WifiConnect WEP 64bit key 2 = %x,%x,%x,%x,%x")
TRACE_MSG(WIFISUPP_MAIN_423_112_2_18_0_36_26_2371,"WifiConnect WEP 64bit key 3 = %x,%x,%x,%x,%x")
TRACE_MSG(WIFISUPP_MAIN_426_112_2_18_0_36_26_2372,"WifiConnect WEP 64bit key 4 = %x,%x,%x,%x,%x")
TRACE_MSG(WIFISUPP_MAIN_434_112_2_18_0_36_26_2373,"WifiConnect WEP 128bit key 1 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(WIFISUPP_MAIN_440_112_2_18_0_36_26_2374,"WifiConnect WEP 128bit key 2 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(WIFISUPP_MAIN_446_112_2_18_0_36_26_2375,"WifiConnect WEP 128bit key 3 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(WIFISUPP_MAIN_452_112_2_18_0_36_26_2376,"WifiConnect WEP 128bit key 4 = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(WIFISUPP_MAIN_457_112_2_18_0_36_26_2377,"WifiConnect wpa type=%d")
TRACE_MSG(WIFISUPP_MAIN_458_112_2_18_0_36_26_2378,"WifiConnect wpa =%s")
TRACE_MSG(WIFISUPP_MAIN_462_112_2_18_0_36_26_2379,"WifiConnect wpa type=%d")
TRACE_MSG(WIFISUPP_MAIN_463_112_2_18_0_36_26_2380,"WifiConnect wpa =%s")
TRACE_MSG(WIFISUPP_MAIN_467_112_2_18_0_36_26_2381,"WifiConnect WAPI type is %d")
TRACE_MSG(WIFISUPP_MAIN_470_112_2_18_0_36_26_2382,"WifiConnect WAPI psk = %s")
TRACE_MSG(WIFISUPP_MAIN_480_112_2_18_0_36_26_2383,"WIFISUPP WifiConnect, custom supplicant info NOT available!")
TRACE_MSG(WIFISUPP_MAIN_485_112_2_18_0_36_26_2384,"WIFISUPP WifiConnect, wifi_connect_func is null!")
TRACE_MSG(WIFISUPP_MAIN_499_112_2_18_0_36_26_2385,"WIFISUPP WifiDisconnect, invalid input!")
TRACE_MSG(WIFISUPP_MAIN_505_112_2_18_0_36_26_2386,"WIFISUPP WifiDisconnect, custom supplicant info NOT available!")
TRACE_MSG(WIFISUPP_MAIN_510_112_2_18_0_36_26_2387,"WIFISUPP WifiDisconnect, wifi_connect_func is null!")
TRACE_MSG(WIFISUPP_MAIN_525_112_2_18_0_36_26_2388,"WIFISUPP WifiHandleCusMsg, custom supplicant info NOT available!")
TRACE_MSG(WIFISUPP_MAIN_530_112_2_18_0_36_26_2389,"WIFISUPP WifiHandleCusMsg, wifi_handle_cus_msg_func is null!")
TRACE_MSG(WIFISUPP_MAIN_544_112_2_18_0_36_26_2390,"WIFI:error:SUPP:%s command_info_ptr is PNULL!")
TRACE_MSG(WIFISUPP_MAIN_550_112_2_18_0_36_26_2391,"WIFI:error:SUPP:%s s_suppadap_custom_info_ptr is PNULL!")
TRACE_MSG(WIFISUPP_MAIN_556_112_2_18_0_36_26_2392,"WIFI:error:SUPP:%s wifi_command_func is PNULL!")
END_TRACE_MAP(MS_CUSTOMIZE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_CUSTOMIZE_TRC_H_

