/******************************************************************************
 ** File Name:      ms_ref_bt_pal_csr_trc.h                                         *
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
//trace_id:38
#ifndef _MS_REF_BT_PAL_CSR_TRC_H_
#define _MS_REF_BT_PAL_CSR_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define BT_ABS_60_112_2_18_1_6_19_0 "[BT_Config]: no valid com %d"
#define BT_ABS_66_112_2_18_1_6_19_1 "[BT_Config] bd_addr = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x"
#define BT_ABS_67_112_2_18_1_6_19_2 "[BT_Config] port_num = %d"
#define BT_ABS_68_112_2_18_1_6_19_3 "[BT_Config] baud_rate = %d"
#define BT_ABS_75_112_2_18_1_6_19_4 "[BT]dir[%d]=0x%X\r\n"
#define BT_ABS_101_112_2_18_1_6_19_5 "[BT]BT_Init\r\n"
#define BT_ABS_153_112_2_18_1_6_19_6 "[BT]BT_Deinit\r\n"
#define BT_ABS_162_112_2_18_1_6_19_7 "[BT]BT_TestInit\r\n"
#define BT_ABS_189_112_2_18_1_6_19_8 "[BT]BT_Start\r\n"
#define BT_ABS_257_112_2_18_1_6_19_9 "[BT_SearchDevice] IN (service_type = 0x%x)"
#define BT_ABS_448_112_2_18_1_6_20_10 "[BT_GetDeviceType] IN (class of device = 0x%06x and "
#define BT_ABS_452_112_2_18_1_6_20_11 "[BT]<INFO> cod = DB_GetDeviceClass() = 0x%x"
#define BT_ABS_540_112_2_18_1_6_20_12 "[BT_GetPairedDeviceCount] IN (service_type = 0x%x)"
#define BT_ABS_595_112_2_18_1_6_20_13 "[BT_GetPairedDeviceInfo] IN (service_type = 0x%x, index = %d)"
#define BT_ABS_728_112_2_18_1_6_20_14 "[BT]<WARNING> %s L%d: NO avalable service"
#define BT_ABS_766_112_2_18_1_6_20_15 "[BT_GetPairedDeviceService] service = 0x%x"
#define BT_ABS_1026_112_2_18_1_6_21_16 "BT_SetFtpRootFolder path[0] %d, path[1] %d  mode %s"
#define BT_ABS_2220_112_2_18_1_6_23_17 "[BT]BT_SppConnect\n"
#define BT_ABS_2226_112_2_18_1_6_23_18 "[BT] Spp have connected with device, please disconnect it first!"
#define BT_ABS_2275_112_2_18_1_6_23_19 "[BT] Spp have connected with device, please disconnect it first!"
#define BT_ABS_2279_112_2_18_1_6_23_20 "[BT] BT_SppInitCallback!"
#define BT_ABS_2312_112_2_18_1_6_23_21 "[BT] BT_SppReleaseCallback!"
#define BT_ABS_2317_112_2_18_1_6_23_22 "[BT] Spp have bot connected with any device!"
#define BT_ABS_2347_112_2_18_1_6_23_23 "[BT]BT_SppDisconnect\n"
#define BT_ABS_2363_112_2_18_1_6_23_24 "[BT]BT_Spp_WriteData: src_buf = %s\n"
#define BT_ABS_2364_112_2_18_1_6_23_25 "[BT]BT_Spp_WriteData: data_len = %d\n"
#define BT_ABS_2365_112_2_18_1_6_23_26 "[BT]BT_Spp_WriteData: src_buf = %d\n"
#define BT_ABS_2377_112_2_18_1_6_23_27 "[BT]BT_Spp_WriteData: FIFO full\n"
#define BT_ABS_2382_112_2_18_1_6_23_28 "[BT]BT_Spp_WriteData: SPP have not connected!!\n"
#define BT_ABS_2406_112_2_18_1_6_23_29 "[BT]BT_Spp_WriteData: SPP have not connected!!\n"
#define BT_DUT_99_112_2_18_1_6_24_30 "[DUT_HandlePrim] prim_type = 0x%x"
#define BT_DUT_109_112_2_18_1_6_24_31 "CM_BCCMD_CFM"
#define BT_DUT_119_112_2_18_1_6_24_32 "CM_ENABLE_DUT_MODE_CFM: result = %d, stepNumber = %d"
#define BT_DUT_143_112_2_18_1_6_24_33 "[_dut_handler] subtype = %d"
#define BT_DUT_205_112_2_18_1_6_24_34 "[DRV_BT]<ASSERT_ERROR> _dut_handler"
#define BT_DUT_356_112_2_18_1_6_24_35 "[_start_radio_test] test_item=%d, channel=%d, payload_type=%d, packet_type=%d"
#define BT_DUT_400_112_2_18_1_6_24_36 "[_start_afc] channel = %d"
#define BT_DUT_417_112_2_18_1_6_24_37 "[_set_dac_value] dac = %d"
#define BT_DUT_426_112_2_18_1_6_24_38 "[_save_dac_value] dac = %d"
#define BT_DUT_435_112_2_18_1_6_24_39 "[_load_dac_value] dac = %d"
#define BT_FM_116_112_2_18_1_6_25_40 "[BT_FM] FM_SetStatus: new status = %d, old one = %d"
#define BT_FM_141_112_2_18_1_6_25_41 "[BT_FM] FM_Init"
#define BT_FM_145_112_2_18_1_6_25_42 "FM_Init: Don't need to Initailize again!!!"
#define BT_FM_162_112_2_18_1_6_25_43 "[BT_FM] FM_Init register cfm"
#define BT_FM_183_112_2_18_1_6_25_44 "[BT_FM] FM_Open,Version20080905"
#define BT_FM_200_112_2_18_1_6_25_45 "[BT_FM] FM_Open enble cfm"
#define BT_FM_217_112_2_18_1_6_25_46 "[BT_FM] FM_Open unmute cfm"
#define BT_FM_227_112_2_18_1_6_25_47 "[BT_FM]FM_Open: NO Actions! "
#define BT_FM_240_112_2_18_1_6_25_48 "[BT_FM] FM_Start: freq = %d"
#define BT_FM_267_112_2_18_1_6_25_49 "[BT_FM] FM_resume mute cfg"
#define BT_FM_286_112_2_18_1_6_25_50 "[BT_FM] FM_Start: set frequnency cfm"
#define BT_FM_298_112_2_18_1_6_25_51 "[BT_FM] FM_Start: NO Acitons!"
#define BT_FM_309_112_2_18_1_6_25_52 "[BT_FM] FM_Stop"
#define BT_FM_324_112_2_18_1_6_25_53 "[BT_FM] FM_Stop mute cfg"
#define BT_FM_335_112_2_18_1_6_25_54 "[BT_FM] FM_Stop NO Actions"
#define BT_FM_348_112_2_18_1_6_25_55 "[BT_FM] FM_Close"
#define BT_FM_352_112_2_18_1_6_25_56 "FM_Close: Don't need to Close"
#define BT_FM_367_112_2_18_1_6_25_57 "[BT_FM] FM_Close disable cfg"
#define BT_FM_391_112_2_18_1_6_25_58 "[BT_FM] FM_ManualSeek: freq = %d"
#define BT_FM_417_112_2_18_1_6_26_59 "[BT_FM] FM_ManualSeek: seek_dir error"
#define BT_FM_445_112_2_18_1_6_26_60 "[BT_FM] FM_AutoSeek: seek_dir = %d, mode = %d, freq = %d"
#define BT_FM_492_112_2_18_1_6_26_61 "[BT_FM] FM_AutoSeek: seek_dir error"
#define BT_FM_513_112_2_18_1_6_26_62 "[BT_FM] FM_SetVol: channel = %d, level = %d"
#define BT_FM_539_112_2_18_1_6_26_63 "[BT_FM] FM_SetVol: FM state error"
#define BT_FM_554_112_2_18_1_6_26_64 "[BT_FM] FM_SetScanLevel: level = %d"
#define BT_FM_578_112_2_18_1_6_26_65 "[BT_FM] FM_SetScanLevel: db = %d"
#define BT_FM_586_112_2_18_1_6_26_66 "[BT_FM] FM_SetScanLevel req "
#define BT_FM_621_112_2_18_1_6_26_67 "[BT_FM] FM_SetRegion: fm_region = %d"
#define BT_FM_642_112_2_18_1_6_26_68 "[BT_FM] FM_SetRegion req cfm"
#define BT_FM_675_112_2_18_1_6_26_69 "[FM_HandlePrim] IN"
#define BT_FM_684_112_2_18_1_6_26_70 "[BT_FM]FM_HandlePrim:type = 0x%X\r\n"
#define BT_FM_779_112_2_18_1_6_26_71 "[BT_FM]FM_HandlePrim:FM_AUTO_SEARCH_RESULT_IND frequency = %d"
#define BT_FM_788_112_2_18_1_6_26_72 "[BT_FM]FM_HandlePrim:FM_AUTO_SEARCH_RESULT_IND invalid station"
#define BCCMD_BOOT_STRAP_1471_112_2_18_1_6_30_73 "BootStrap: Baudrate set to: %d\n"
#define BCCMD_BOOT_STRAP_1480_112_2_18_1_6_30_74 " Can not set 0 as the address of bluetooth,set it default"
#define BCCMD_BOOT_STRAP_1487_112_2_18_1_6_30_75 "BootStrap: Bluetooth device address set to: %04x:%x:%x\n"
#define BCCMD_BOOT_STRAP_1493_112_2_18_1_6_30_76 "BootStrap: Crystal Frequency set to: %d\n"
#define BCCMD_BOOT_STRAP_1507_112_2_18_1_6_30_77 "BootStrap: Setting Friendly name to: %s\n"
#define BCCMD_BOOT_STRAP_1527_112_2_18_1_6_30_78 "[BT]BCCMD_SetBaudRate:User baud=0x%X\r\n"
#define BCCMD_BOOT_STRAP_1563_112_2_18_1_6_30_79 "[BT]BCCMD_SetBaudRate;baud=0x%X\r\n"
#define BCCMD_BOOT_STRAP_1567_112_2_18_1_6_30_80 "[BT]BCCMD_SetCrystalFreq;freq_khz=0x%X\r\n"
#define BCCMD_BOOT_STRAP_1577_112_2_18_1_6_30_81 " Can not set 0 as the address of bluetooth,set it default"
#define BCCMD_BOOT_STRAP_1584_112_2_18_1_6_30_82 "[BT]BCCMD_SetBtAddr;btNap=0x%X,btUap=0x%X,btLap=0x%X\r\n"
#define BCCMD_BOOT_STRAP_1589_112_2_18_1_6_30_83 "[BT]BCCMD_SetPcmConfig32:config=0x%X\r\n"
#define BCCMD_BOOT_STRAP_1595_112_2_18_1_6_30_84 "[BT]BCCMD_SetPcmFormat:format=0x%X\r\n"
#define BCCMD_BOOT_STRAP_1602_112_2_18_1_6_30_85 "[BT]BCCMD_SetPcmSlot:Slot=0x%X\r\n"
#define BCCMD_BOOT_STRAP_1608_112_2_18_1_6_30_86 "[BT] BCCMD_SetXtalFtrim: ftrim = %d"
#define BCCMD_BOOT_STRAP_1621_112_2_18_1_6_30_87 "[BT]reConfigureBaudRate:baudIn=0x%X\r\n"
#define BCCMD_BOOT_STRAP_1667_112_2_18_1_6_30_88 "error ,error error baudrate"
#define BCCMD_BOOT_STRAP_1725_112_2_18_1_6_30_89 "build_boot_strap_pskey_set key %04x data %04x len %04x\n"
#define BCCMD_BOOT_STRAP_1791_112_2_18_1_6_30_90 "build_boot_strap_bccmd varid %04x data %p len %04x\n"
#define BCCMD_BOOT_STRAP_1871_112_2_18_1_6_30_91 "BootStrap: State %d seq_no %d\n"
#define BCCMD_BOOT_STRAP_1904_112_2_18_1_6_30_92 "BootStrap: EARLY_PATCHES - BuildID %d (0x%04x)\n"
#define BCCMD_BOOT_STRAP_1908_112_2_18_1_6_30_93 "BootStrap: EARLY_PATCHES - Result status %02x%02x\n"
#define BCCMD_BOOT_STRAP_1914_112_2_18_1_6_31_94 "BootStrap: EARLY_PATCHES - Did not get result\n"
#define BCCMD_BOOT_STRAP_1931_112_2_18_1_6_31_95 "BootStrap:NO EARLY PATCHES NEEDED: NOP\n"
#define BCCMD_BOOT_STRAP_2275_112_2_18_1_6_31_96 "Apply key index %d len %d\n"
#define BCCMD_BOOT_STRAP_2290_112_2_18_1_6_31_97 "BootStrap:PATCHES_AND_KEYS: NOP\n"
#define BCCMD_BOOT_STRAP_2317_112_2_18_1_6_31_98 "BootStrap: Clearing Memory bank %d\n"
#define BLUECALL_205_112_2_18_1_6_32_99 "[BLUE_Play] IN"
#define BLUECALL_221_112_2_18_1_6_32_100 "[BLUE_Play] Have opened"
#define BLUECALL_284_112_2_18_1_6_32_101 "[BLUE_Stop] IN"
#define BLUECALL_329_112_2_18_1_6_32_102 "[BLUE_Stop] has closed"
#define BLUECALL_339_112_2_18_1_6_32_103 "[BT]BLUE_Pause\r\n"
#define BLUECALL_348_112_2_18_1_6_32_104 "[BT]BLUE_Resume\r\n"
#define BLUECALL_368_112_2_18_1_6_32_105 "[BLUE_StartRecord] IN"
#define BLUECALL_416_112_2_18_1_6_33_106 "[BLUE_StopRecord] IN"
#define BLUECALL_487_112_2_18_1_6_33_107 "[BT]BLUE_WritePCMData:Data Full:0x%X, 0x%X\r\n"
#define BLUECALL_512_112_2_18_1_6_33_108 "[BT]PCM:0x%X,0x%X,0x%X,0x%X\r\n"
#define BLUECALL_530_112_2_18_1_6_33_109 "[BT]BLUE_WritePCMData:track_occupied is zero\r\n"
#define BLUECALL_558_112_2_18_1_6_33_110 "[BT]Voice:0x%X,0x%X,0x%X,0x%X\r\n"
#define BLUECALL_582_112_2_18_1_6_33_111 "[BT]BLUE_WriteVoiceData:data_count=0x%X\r\n"
#define BLUECALL_643_112_2_18_1_6_33_112 ":%d\r\n"
#define BLUECALL_833_112_2_18_1_6_33_113 "[BLUE_Play] IN"
#define BLUECALL_853_112_2_18_1_6_33_114 "[BLUE_Play] Have opened"
#define BLUECALL_907_112_2_18_1_6_34_115 "[BLUE_Stop] IN"
#define BLUECALL_949_112_2_18_1_6_34_116 "[BLUE_Stop] has closed"
#define BLUECALL_959_112_2_18_1_6_34_117 "[BT]BLUE_Pause"
#define BLUECALL_968_112_2_18_1_6_34_118 "[BT]BLUE_Resume"
#define BLUECALL_985_112_2_18_1_6_34_119 "[BLUE_StartRecord] IN"
#define BLUECALL_1025_112_2_18_1_6_34_120 "[BLUE_StopRecord] IN"
#define BLUECALL_1064_112_2_18_1_6_34_121 "[BLUE_PauseRecord] IN"
#define BLUECALL_1073_112_2_18_1_6_34_122 "[BLUE_ResumeRecord] IN"
#define BLUECALL_1113_112_2_18_1_6_34_123 "[BT]BLUE_WritePCMData:Data Full:0x%X, 0x%X\r\n"
#define BLUECALL_1138_112_2_18_1_6_34_124 "[BT]PCM:0x%X,0x%X,0x%X,0x%X\r\n"
#define BLUECALL_1156_112_2_18_1_6_34_125 "[BT]BLUE_WritePCMData:track_occupied is zero\r\n"
#define BLUECALL_1184_112_2_18_1_6_34_126 "[BT]Voice:0x%X,0x%X,0x%X,0x%X\r\n"
#define BLUECALL_1207_112_2_18_1_6_34_127 "[BT]BLUE_WriteVoiceData:data_count=0x%X\r\n"
#define BLUECALL_1269_112_2_18_1_6_34_128 ":%d\r\n"
#define BLUECALL_UART_322_112_2_18_1_6_35_129 "[BLUE_Play] IN"
#define BLUECALL_UART_328_112_2_18_1_6_35_130 "[BLUE_Play] Have opened"
#define BLUECALL_UART_372_112_2_18_1_6_35_131 "[BLUE_Stop] IN"
#define BLUECALL_UART_403_112_2_18_1_6_35_132 "[BLUE_Stop] has closed"
#define BLUECALL_UART_415_112_2_18_1_6_35_133 "[BT]BLUE_Pause\r\n"
#define BLUECALL_UART_426_112_2_18_1_6_35_134 "[BT]BLUE_Resume\r\n"
#define BLUECALL_UART_438_112_2_18_1_6_35_135 "[BLUE_StartRecord] IN"
#define BLUECALL_UART_482_112_2_18_1_6_35_136 "[BLUE_StopRecord] IN"
#define BLUECALL_UART_495_112_2_18_1_6_36_137 "[BLUE_PauseRecord] IN"
#define BLUECALL_UART_508_112_2_18_1_6_36_138 "[BLUE_ResumeRecord] IN"
#define OS_PIN_7_112_2_18_1_6_37_139 "[BT]OS_Close_BlueCore\r\n"
#define OS_PIN_13_112_2_18_1_6_37_140 "[BT]OS_Start_BlueCore\r\n"
#define OS_UART_DMA_136_112_2_18_1_6_37_141 "[BT]uart0_timer_handler:last=0x%X, addr=0x%X\r\n"
#define OS_UART_DMA_176_112_2_18_1_6_37_142 "__FreeUartDmaChannel[port][tx], channel is empty\n"
#define OS_UART_DMA_187_112_2_18_1_6_37_143 "__FreeUartDmaChannel[port][tx], channel is empty\n"
#define OS_UART_DMA_253_112_2_18_1_6_37_144 "[BT]OSUartDrv_SetUartId:port:0x%X, tx_dma:0x%X, rx_dma:0x%X"
#define OS_UART_DMA_302_112_2_18_1_6_37_145 "[BT] critical error error error buffer over"
#define OS_UART_DMA_540_112_2_18_1_6_38_146 "[BT]EVENT_WRITE_COMPLETE:timer handle is empty\r\n"
#define OS_UART_DMA_655_112_2_18_1_6_38_147 "[BT]EVENT_DATA_TO_READ:timer handle is empty\r\n"
#define OS_UART_DMA_695_112_2_18_1_6_38_148 "[BT]OSSetUart_Configure baud_rate = 0x%X"
#define OS_UART_DMA_710_112_2_18_1_6_38_149 "[BT]OSUartDrv_Configure:baudrate = %d\r\n"
#define OS_UART_DMA_746_112_2_18_1_6_38_150 "[BT]OSUartDrv_Configure End g_uart_timer = %d\r\n"
#define OS_UART_DMA_819_112_2_18_1_6_38_151 "[BT]OSUartDrv_Reset"
#define OS_UART_DMA_843_112_2_18_1_6_39_152 "[BT]OSUartDrv_Stop"
#define OS_UART_DMA_867_112_2_18_1_6_39_153 "[BT]OSUartDrv_Start"
#define OS_UART_DMA_871_112_2_18_1_6_39_154 "[BT]OSUartDrv_Start: port= %d __GetUartDmaChannel Error\r\n"
#define OS_UART_DMA_902_112_2_18_1_6_39_155 "[BT]UART_GenerateBreakSignal %d ms"
#define PANIC_85_112_2_18_1_6_39_156 "[BT]sched_panic:%s, g_test_panicid=%d\r\n"
#define OS_MSG_160_112_2_18_1_6_42_157 "[BT]OS_Do_Put_External_Task_Message = 0x%X\r\n"
#define OS_PMALLOC_102_112_2_18_1_6_42_158 "BT_InitMem: Init successful!"
#define OS_PMALLOC_107_112_2_18_1_6_42_159 "BT_InitMem: already init !"
#define OS_PMALLOC_132_112_2_18_1_6_42_160 "BT_DeleteMem"
#define OS_TASK_45_112_2_18_1_6_43_161 "[BT]OS_SetSchedThreadPriority:priority=0x%X\r\n"
#define OS_TASK_51_112_2_18_1_6_43_162 "[BT]:OS_HandleStartSchedMsg\r\n"
#define OS_TASK_64_112_2_18_1_6_43_163 "[BT]:OS_HandleStartBtMsg\r\n"
#define OS_TASK_74_112_2_18_1_6_43_164 "[BT]:OS_HandleStopSchedMsg\r\n"
#define OS_TASK_75_112_2_18_1_6_43_165 "[BT]BT_Os_Sched_Task:time=%d"
#define OS_TASK_83_112_2_18_1_6_43_166 "[BT]:OS_HandleBtVisibleMsg\r\n"
#define OS_TASK_100_112_2_18_1_6_43_167 "[BT]:OS_Handle_Sched_Task_Msg:Should not start scheduler at here\r\n"
#define OS_TASK_122_112_2_18_1_6_43_168 "[BT]:OS_Handle_Sched_Task_Msg - OS_SCHED_TASK_MSG\r\n"
#define OS_TASK_128_112_2_18_1_6_43_169 "[BT]:OS_Handle_Sched_Task_Msg - OS_DEACTIVATED_SERVICE_OK_MSG\r\n"
#define OS_TASK_137_112_2_18_1_6_43_170 "[BT]:OS_Handle_Sched_Task_Msg - Begin to send message!\r\n"
#define OS_TASK_144_112_2_18_1_6_43_171 "[BT]:OS_Handle_Sched_Task_Msg - Undefine message:0x%X\r\n"
#define OS_TASK_154_112_2_18_1_6_43_172 "[BT]:BT_Os_Sched_Task\r\n"
#define OS_TASK_157_112_2_18_1_6_43_173 "[BT]BT_Os_Sched_Task = 0x%x waitting...... \r\n"
#define OS_TASK_164_112_2_18_1_6_43_174 "[BT]BT_Os_Sched_Task:Start Scheduler.... \r\n"
#define OS_TASK_165_112_2_18_1_6_43_175 "[BT]BT_Os_Sched_Task:time=%d"
#define OS_TASK_174_112_2_18_1_6_43_176 "[BT]BT_Os_Sched_Task: Invaild Signal = %s\r\n"
#define OS_TASK_198_112_2_18_1_6_43_177 "[BT]OS_StartThread:priority=0x%X\r\n"
#define OS_TASK_204_112_2_18_1_6_43_178 "[BT]OS_StartThread - s_bt_sched_thread_id = 0x%X\r\n"
#define OS_TIMER_23_112_2_18_1_6_44_179 "[BT]OS_StartTimeOut:Create Timer handle=0x%08X\r\n"
#define OS_TIMER_27_112_2_18_1_6_44_180 "[BT]OS_StartTimeOut: Active Timer handle=0x%08X\r\n"
#define OS_TIMER_37_112_2_18_1_6_44_181 "[BT]OS_IsTimeOutStart:s_bt_timeout_handle=0x%08X\r\n"
#define OS_TIMER_49_112_2_18_1_6_44_182 "[BT]OS_DeleteTimeOut:Timer handle=0x%08X\r\n"
#define OS_TIMER_60_112_2_18_1_6_44_183 "[BT]OS_StopTimeOut:s_bt_timeout_handle=0x%08X\r\n"
#define TASK_22_112_2_18_1_6_46_184 "ERROR NUMBER_OF_TASKS ID=0X%x\n\t"
#define A2DP_API_59_112_2_18_1_6_51_185 "[A2DPAdapt_SendMessageToApp] msg_id = 0x%x, isResume = %d, isSuspend = %d,body=%p,size=%d"
#define A2DP_API_111_112_2_18_1_6_51_186 "[DRV_BT]<WARNING> A2DP_InitDevice(): ERROR sample_rate = %d"
#define A2DP_API_155_112_2_18_1_6_51_187 "A2DP_OpenDevice: uiSampleRate = %d"
#define A2DP_API_171_112_2_18_1_6_51_188 "A2DP_CloseDevice"
#define A2DP_API_190_112_2_18_1_6_51_189 "A2DP_PauseDevice"
#define A2DP_API_205_112_2_18_1_6_51_190 "A2DP_ResumeDevice"
#define A2DP_API_224_112_2_18_1_6_51_191 "A2DP_StartStreamData"
#define A2DP_AS_109_112_2_18_1_6_52_192 "_A2DP_EnableDigitalGain add express:0x%x to codec:0x%x."
#define A2DP_AS_123_112_2_18_1_6_52_193 "_A2DP_EnableDigitalGain delete express:0x%x from codec:0x%x."
#define A2DP_AS_137_112_2_18_1_6_52_194 "A2DP_Play uiSamleRate = %d"
#define A2DP_AS_160_112_2_18_1_6_52_195 "A2DP_Stop"
#define A2DP_AS_173_112_2_18_1_6_52_196 "A2DP_Pause"
#define A2DP_AS_186_112_2_18_1_6_52_197 "A2DP_Resume uiSampleRate = %d"
#define A2DP_AS_209_112_2_18_1_6_52_198 "A2DP_SetSampleRate samle_rate = %d"
#define A2DP_AS_234_112_2_18_1_6_52_199 "a2dp_as.c,[A2DP_SetDeviceMode] set aud proc explugger! hAUDPROCExp:0x%x"
#define A2DP_AS_251_112_2_18_1_6_52_200 "a2dp_as.c,[A2DP_SetDeviceMode] set aud enha explugger! hAUDENHAExp:0x%x"
#define A2DP_AS_273_112_2_18_1_6_52_201 "A2DP_SetVolumeType vol:%d, appType:%d."
#define A2DP_AS_278_112_2_18_1_6_52_202 "A2DP_SetVolumeType volume level is too bit(%d), will be adjusted to be:%d."
#define A2DP_AS_285_112_2_18_1_6_52_203 "A2DP_SetVolumeType filed to get modeName:%d."
#define A2DP_AS_291_112_2_18_1_6_52_204 "A2DP_SetVolumeType, alloc fail, size: %d"
#define A2DP_AS_298_112_2_18_1_6_52_205 "A2DP_SetVolumeType failed to get mode param, mode:%d, modeName:%s."
#define A2DP_AS_304_112_2_18_1_6_52_206 "A2DP_SetVolumeType uiVol=%d, mode:%d, appType:%d, modeName:%s."
#define A2DP_AS_317_112_2_18_1_6_52_207 "A2DP_SetVolumeType gain:0x%4x, dg_level= 0x%4x"
#define A2DP_AS_342_112_2_18_1_6_52_208 "A2DP_SetDownlinkMute, mute:%d, vol:%d, appType:%d."
#define A2DP_AS_399_112_2_18_1_6_52_209 "peter: pcm_data_count is %d"
#define A2DP_AS_447_112_2_18_1_6_53_210 "peter:g_sbc_src_samplerate is %d"
#define PF_A2DP_283_112_2_18_1_6_54_211 "[BT][startDeactivate] IN"
#define PF_A2DP_292_112_2_18_1_6_54_212 "[BT][A2DP_ServiceClose] IN"
#define PF_A2DP_293_112_2_18_1_6_54_213 "[BT][A2DP_ServiceClose] status = %d"
#define PF_A2DP_294_112_2_18_1_6_54_214 "[BT][A2DP_ServiceClose] state = %d"
#define PF_A2DP_322_112_2_18_1_6_54_215 "[A2DP_TimeOutHandle] Time Out ,Acitive error"
#define PF_A2DP_335_112_2_18_1_6_54_216 "[startConnecting] IN"
#define PF_A2DP_336_112_2_18_1_6_54_217 "[startConnecting] isActivated = %d"
#define PF_A2DP_337_112_2_18_1_6_54_218 "[startConnecting] currentConnection = %d"
#define PF_A2DP_341_112_2_18_1_6_54_219 "[startConnecting] remoteDevAdddr = %04x:%02x:%06x"
#define PF_A2DP_362_112_2_18_1_6_54_220 "[startDisconnect] currentConnection = %d"
#define PF_A2DP_364_112_2_18_1_6_54_221 "[startDisconnect] connectionId = %d"
#define PF_A2DP_371_112_2_18_1_6_54_222 "[startStream] sample_freq = %d"
#define PF_A2DP_372_112_2_18_1_6_54_223 "[startStream] state = %d"
#define PF_A2DP_373_112_2_18_1_6_54_224 "[startStream] instData->sample_freq = %d"
#define PF_A2DP_374_112_2_18_1_6_54_225 "[startStream] ActiveConfig.sample_freq = %d"
#define PF_A2DP_375_112_2_18_1_6_54_226 "[startStream] currentConnection = %d"
#define PF_A2DP_377_112_2_18_1_6_54_227 "[startStream] connectionId = %d"
#define PF_A2DP_378_112_2_18_1_6_54_228 "[startStream] role = %d"
#define PF_A2DP_400_112_2_18_1_6_54_229 "[stopStream] state = %d"
#define PF_A2DP_401_112_2_18_1_6_54_230 "[stopStream] currentConnection = %d"
#define PF_A2DP_403_112_2_18_1_6_54_231 "[stopStream] streamHandle = %d"
#define PF_A2DP_411_112_2_18_1_6_54_232 "[suspendStream] state = %d"
#define PF_A2DP_412_112_2_18_1_6_54_233 "[suspendStream] currentConnection = %d"
#define PF_A2DP_414_112_2_18_1_6_54_234 "[suspendStream] streamHandle = %d"
#define PF_A2DP_425_112_2_18_1_6_54_235 "[startReconfigure] original sample freq = %d"
#define PF_A2DP_431_112_2_18_1_6_54_236 "[startReconfigure] buildServCap failed"
#define PF_A2DP_442_112_2_18_1_6_54_237 "[startReconfigure] target sample freq = %d"
#define PF_A2DP_447_112_2_18_1_6_54_238 "[startReconfigure] no need to reconfigure, start stream"
#define PF_A2DP_450_112_2_18_1_6_54_239 "[startReconfigure] do reconfigure"
#define PF_A2DP_460_112_2_18_1_6_54_240 "[startAbort] state = %d"
#define PF_A2DP_461_112_2_18_1_6_54_241 "[startAbort] currentConnection = %d"
#define PF_A2DP_463_112_2_18_1_6_54_242 "[startAbort] streamHandle = %d"
#define PF_A2DP_514_112_2_18_1_6_54_243 "[buildServCap]:support different sample:servcap:0x%x,sample:0x%x"
#define PF_A2DP_520_112_2_18_1_6_54_244 "[buildServCap]:support different sample:servcap:0x%x,sample:0x%x"
#define PF_A2DP_554_112_2_18_1_6_54_245 "[buildServCap] platform do not support the sample_freq"
#define PF_A2DP_561_112_2_18_1_6_54_246 "[buildServCap] sample_freq =%d,sbcCap[0]=%d"
#define PF_A2DP_575_112_2_18_1_6_54_247 "[buildServCap] Samling Frequency = 0x%02x"
#define PF_A2DP_617_112_2_18_1_6_54_248 "[buildServCap] candi = %d"
#define PF_A2DP_634_112_2_18_1_6_54_249 "[buildServCap] Channel Mode = 0x%02x"
#define PF_A2DP_647_112_2_18_1_6_54_250 "[buildServCap] Block Length = 0x%02x"
#define PF_A2DP_660_112_2_18_1_6_54_251 "[buildServCap] Subbands = 0x%02x"
#define PF_A2DP_673_112_2_18_1_6_54_252 "[buildServCap] Allocation Method = 0x%02x"
#define PF_A2DP_689_112_2_18_1_6_54_253 "[buildServCap] *(servCap + 6) = 0x%02x"
#define PF_A2DP_690_112_2_18_1_6_54_254 "[buildServCap] sbcCap[2] = 0x%02x"
#define PF_A2DP_692_112_2_18_1_6_54_255 "[buildServCap] *(servCap + 7) = 0x%02x"
#define PF_A2DP_693_112_2_18_1_6_54_256 "[buildServCap] sbcCap[3] = 0x%02x"
#define PF_A2DP_827_112_2_18_1_6_55_257 "dev_name:0x%04x\n"
#define PF_A2DP_832_112_2_18_1_6_55_258 "It's a Jabra hs.\n"
#define PF_A2DP_853_112_2_18_1_6_55_259 "[transmitAudioPacket] no connected_streaming connection"
#define PF_A2DP_867_112_2_18_1_6_55_260 "[transmitAudioPacket] (%d) no packet"
#define PF_A2DP_877_112_2_18_1_6_55_261 "[transmitAudioPacket] (%d) data = 0x%x, dataLen = %d, frames = %d"
#define PF_A2DP_923_112_2_18_1_6_55_262 "[audioSamplesHandler] IN (tick_diff = %d)"
#define PF_A2DP_927_112_2_18_1_6_55_263 "[audioSamplesHandler] frames = %d"
#define PF_A2DP_947_112_2_18_1_6_55_264 "[audioSamplesHandler] time_curr = %d, refStartTime = %d"
#define PF_A2DP_952_112_2_18_1_6_55_265 "[audioSamplesHandler] continue, time_curr = %d, packetRefTime = %d"
#define PF_A2DP_964_112_2_18_1_6_55_266 "[audioSamplesHandler] time_curr = %d, packetRefTime = %d, wait_time = %d"
#define PF_A2DP_1000_112_2_18_1_6_55_267 "[handleActivateCfm] result = %d"
#define PF_A2DP_1011_112_2_18_1_6_55_268 "[handleDeactivateCfm] result = %d"
#define PF_A2DP_1058_112_2_18_1_6_55_269 "[handleConnectInd] connectionId = %d"
#define PF_A2DP_1060_112_2_18_1_6_55_270 "[handleConnectInd] deviceAddr = %04x:%02x:%06x"
#define PF_A2DP_1092_112_2_18_1_6_55_271 "[handleConnectCfm] result = %d"
#define PF_A2DP_1101_112_2_18_1_6_55_272 "[handleConnectCfm] connectionId = %d"
#define PF_A2DP_1103_112_2_18_1_6_55_273 "[handleConnectCfm] deviceAddr = %04x:%02x:%06x"
#define PF_A2DP_1104_112_2_18_1_6_55_274 "[handleConnectCfm] maxFrameSize = %d"
#define PF_A2DP_1114_112_2_18_1_6_55_275 "instData->currentConnection].inUse = TRUE"
#define PF_A2DP_1152_112_2_18_1_6_55_276 "[handleConnectCfm] it should be an open cfm"
#define PF_A2DP_1183_112_2_18_1_6_55_277 "[handleDisconnectInd] connectionId = %d, result = %d"
#define PF_A2DP_1188_112_2_18_1_6_55_278 "[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore"
#define PF_A2DP_1243_112_2_18_1_6_55_279 "[handleConnectInd] deviceAddr = %02x:%02x:%02x:%02x:%02x:%02x"
#define PF_A2DP_1265_112_2_18_1_6_56_280 "[handleDiscoverCfm] result = %d"
#define PF_A2DP_1266_112_2_18_1_6_56_281 "[handleDiscoverCfm] connectionId = %d"
#define PF_A2DP_1267_112_2_18_1_6_56_282 "[handleDiscoverCfm] listLength = %d"
#define PF_A2DP_1268_112_2_18_1_6_56_283 "[handleDiscoverCfm] tLabel = %d"
#define PF_A2DP_1273_112_2_18_1_6_56_284 "[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore"
#define PF_A2DP_1293_112_2_18_1_6_56_285 "[handleDiscoverCfm] SEID %d"
#define PF_A2DP_1294_112_2_18_1_6_56_286 "acpSeid = %d"
#define PF_A2DP_1295_112_2_18_1_6_56_287 "inUse = %d"
#define PF_A2DP_1296_112_2_18_1_6_56_288 "mediaType = %d"
#define PF_A2DP_1297_112_2_18_1_6_56_289 "sepType = %d"
#define PF_A2DP_1319_112_2_18_1_6_56_290 "The remote device does not have available audio end-points\n"
#define PF_A2DP_1326_112_2_18_1_6_56_291 "Stream discovery rejected\n"
#define PF_A2DP_1339_112_2_18_1_6_56_292 "[handleDiscoverInd]"
#define PF_A2DP_1366_112_2_18_1_6_56_293 "[handleGetCapabilitiesCfm] result = %d"
#define PF_A2DP_1367_112_2_18_1_6_56_294 "[handleGetCapabilitiesCfm] connectionId = %d"
#define PF_A2DP_1368_112_2_18_1_6_56_295 "[handleGetCapabilitiesCfm] tLabel = %d"
#define PF_A2DP_1369_112_2_18_1_6_56_296 "[handleGetCapabilitiesCfm] servCapLen = %d"
#define PF_A2DP_1373_112_2_18_1_6_56_297 "Get capabilities rejected, error code 0x%X \n"
#define PF_A2DP_1388_112_2_18_1_6_56_298 "[handleGetCapabilitiesCfm] servCap is NULL"
#define PF_A2DP_1393_112_2_18_1_6_56_299 "[handleGetCapabilitiesCfm] buildServCap failed"
#define PF_A2DP_1435_112_2_18_1_6_56_300 "Get capabilities response does not indicate SBC audio support\n"
#define PF_A2DP_1451_112_2_18_1_6_56_301 "[handleGetCapabilitiesInd] connectionId = %d"
#define PF_A2DP_1452_112_2_18_1_6_56_302 "[handleGetCapabilitiesInd] tLabel = %d"
#define PF_A2DP_1453_112_2_18_1_6_56_303 "[handleGetCapabilitiesInd] acpSeid = %d"
#define PF_A2DP_1458_112_2_18_1_6_56_304 "[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore"
#define PF_A2DP_1463_112_2_18_1_6_56_305 "[handleGetCapabilitiesInd] localSeidIndex = %d"
#define PF_A2DP_1465_112_2_18_1_6_56_306 "address lap0x%x,uap0x%x,nap0x%x"
#define PF_A2DP_1495_112_2_18_1_6_56_307 "[handleGetCapabilitiesInd] Reject"
#define PF_A2DP_1505_112_2_18_1_6_56_308 "[handleSetConfigurationCfm] connectionId = %d"
#define PF_A2DP_1506_112_2_18_1_6_56_309 "[handleSetConfigurationCfm] tLabel = %d"
#define PF_A2DP_1507_112_2_18_1_6_56_310 "[handleSetConfigurationCfm] result = %d"
#define PF_A2DP_1508_112_2_18_1_6_56_311 "[handleSetConfigurationCfm] shandle = %d"
#define PF_A2DP_1509_112_2_18_1_6_56_312 "[handleSetConfigurationCfm] servCategory = %d"
#define PF_A2DP_1520_112_2_18_1_6_56_313 "[handleSetConfigurationCfm] AvOpenReqSend"
#define PF_A2DP_1524_112_2_18_1_6_56_314 "Unable to configure stream end-point\n"
#define PF_A2DP_1531_112_2_18_1_6_56_315 "Set configuration rejected, error code 0x%X\n"
#define PF_A2DP_1547_112_2_18_1_6_56_316 "[handleSetConfigurationInd] connectionId = %d"
#define PF_A2DP_1548_112_2_18_1_6_56_317 "[handleSetConfigurationInd] tLabel = %d"
#define PF_A2DP_1549_112_2_18_1_6_56_318 "[handleSetConfigurationInd] shandle = %d"
#define PF_A2DP_1550_112_2_18_1_6_56_319 "[handleSetConfigurationInd] acpSeid = %d"
#define PF_A2DP_1551_112_2_18_1_6_56_320 "[handleSetConfigurationInd] intSeid = %d"
#define PF_A2DP_1552_112_2_18_1_6_56_321 "[handleSetConfigurationInd] servCapLen %d"
#define PF_A2DP_1557_112_2_18_1_6_56_322 "[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore"
#define PF_A2DP_1661_112_2_18_1_6_56_323 "[handleOpenCfm] shandle = %d"
#define PF_A2DP_1662_112_2_18_1_6_56_324 "[handleOpenCfm] tLabel = %d"
#define PF_A2DP_1663_112_2_18_1_6_56_325 "[handleOpenCfm] result = %d"
#define PF_A2DP_1672_112_2_18_1_6_56_326 "Open stream rejected, error code 0x%X\n"
#define PF_A2DP_1684_112_2_18_1_6_56_327 "[handleOpenInd] shandle = %d"
#define PF_A2DP_1685_112_2_18_1_6_56_328 "[handleOpenInd] tLabel = %d"
#define PF_A2DP_1706_112_2_18_1_6_56_329 "[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore"
#define PF_A2DP_1715_112_2_18_1_6_56_330 "[handleStartCfm] tLabel = %d"
#define PF_A2DP_1716_112_2_18_1_6_56_331 "[handleStartCfm] result = %d"
#define PF_A2DP_1717_112_2_18_1_6_56_332 "[handleStartCfm] reject_shandle = %d"
#define PF_A2DP_1725_112_2_18_1_6_56_333 "Unable to configure local stream end-point\n"
#define PF_A2DP_1737_112_2_18_1_6_56_334 "Start stream rejected, error code 0x%x\n"
#define PF_A2DP_1751_112_2_18_1_6_56_335 "[handleStartInd] tLabel = %d"
#define PF_A2DP_1752_112_2_18_1_6_56_336 "[handleStartInd] listLength = %d"
#define PF_A2DP_1753_112_2_18_1_6_56_337 "[handleStartInd] first_shandle = %d"
#define PF_A2DP_1758_112_2_18_1_6_57_338 "[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore"
#define PF_A2DP_1775_112_2_18_1_6_57_339 "Unable to configure local stream end-point\n"
#define PF_A2DP_1824_112_2_18_1_6_57_340 "[handleCloseCfm] tLabel = %d"
#define PF_A2DP_1825_112_2_18_1_6_57_341 "[handleCloseCfm] result = %d"
#define PF_A2DP_1842_112_2_18_1_6_57_342 "Close stream rejected, error code 0x%x\n"
#define PF_A2DP_1854_112_2_18_1_6_57_343 "[handleCloseInd] shandle = %d"
#define PF_A2DP_1855_112_2_18_1_6_57_344 "[handleCloseInd] tLabel = %d"
#define PF_A2DP_1886_112_2_18_1_6_57_345 "[handleSuspendCfm] tLabel = %d"
#define PF_A2DP_1887_112_2_18_1_6_57_346 "[handleSuspendCfm] result = %d"
#define PF_A2DP_1888_112_2_18_1_6_57_347 "[handleSuspendCfm] reject_shandle = %d"
#define PF_A2DP_1906_112_2_18_1_6_57_348 "[DRV_BT]<ASSERT_ERROR>: handleSuspendCfm: invalid state"
#define PF_A2DP_1912_112_2_18_1_6_57_349 "Suspend stream rejected, error code 0x%X\n"
#define PF_A2DP_1925_112_2_18_1_6_57_350 "[handleSuspendInd] tLabel = %d"
#define PF_A2DP_1926_112_2_18_1_6_57_351 "[handleSuspendInd] listLength = %d"
#define PF_A2DP_1927_112_2_18_1_6_57_352 "[handleSuspendInd] first_shandle = %d"
#define PF_A2DP_1932_112_2_18_1_6_57_353 "[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore"
#define PF_A2DP_1980_112_2_18_1_6_57_354 "[handleQosInd] shandle = %d"
#define PF_A2DP_1981_112_2_18_1_6_57_355 "[handleQosInd] bufferStatus = %d"
#define PF_A2DP_1990_112_2_18_1_6_57_356 "[handleStreamMtuSizeInd] shandle = %d"
#define PF_A2DP_1991_112_2_18_1_6_57_357 "[handleStreamMtuSizeInd] remoteMtuSize = %d"
#define PF_A2DP_2017_112_2_18_1_6_57_358 "[handleReconfigureCfm] tLabel = %d"
#define PF_A2DP_2018_112_2_18_1_6_57_359 "[handleReconfigureCfm] result = %d"
#define PF_A2DP_2019_112_2_18_1_6_57_360 "[handleReconfigureCfm] servCategory = %d"
#define PF_A2DP_2037_112_2_18_1_6_57_361 "[handleReconfigureCfm] SBC_prepareSbc failed"
#define PF_A2DP_2057_112_2_18_1_6_57_362 "[handleReconfigureInd] shandle = %d"
#define PF_A2DP_2058_112_2_18_1_6_57_363 "[handleReconfigureInd] tLabel = %d"
#define PF_A2DP_2059_112_2_18_1_6_57_364 "[handleReconfigureInd] servCapLen = %D"
#define PF_A2DP_2072_112_2_18_1_6_57_365 "index = %d"
#define PF_A2DP_2073_112_2_18_1_6_57_366 "Service Category = 0x%x"
#define PF_A2DP_2074_112_2_18_1_6_57_367 "Media Type = 0x%x"
#define PF_A2DP_2075_112_2_18_1_6_57_368 "Media Code Type = 0x%x"
#define PF_A2DP_2112_112_2_18_1_6_57_369 "Sep not in use"
#define PF_A2DP_2131_112_2_18_1_6_57_370 "PrepareSbc fail"
#define PF_A2DP_2139_112_2_18_1_6_57_371 "Bad acp seid"
#define PF_A2DP_2157_112_2_18_1_6_57_372 "[handleGetConfigurationCfm] tLabel = %d"
#define PF_A2DP_2158_112_2_18_1_6_57_373 "[handleGetConfigurationCfm] result = %d"
#define PF_A2DP_2159_112_2_18_1_6_57_374 "[handleGetConfigurationCfm] servCapLen = %d"
#define PF_A2DP_2163_112_2_18_1_6_57_375 "[DRV_BT]<ASSERT_ERROR>: handleGetConfigurationCfm"
#define PF_A2DP_2173_112_2_18_1_6_57_376 "[handleGetConfigurationInd] shandle = %d"
#define PF_A2DP_2174_112_2_18_1_6_57_377 "[handleGetConfigurationInd] tLabel = %d"
#define PF_A2DP_2179_112_2_18_1_6_57_378 "[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore"
#define PF_A2DP_2218_112_2_18_1_6_57_379 "Bad acp seid"
#define PF_A2DP_2228_112_2_18_1_6_57_380 "[handleAbortCfm] shandle = %d"
#define PF_A2DP_2229_112_2_18_1_6_57_381 "[handleAbortCfm] tLabel = %d"
#define PF_A2DP_2267_112_2_18_1_6_58_382 "[handleAbortInd] shandle = %d"
#define PF_A2DP_2268_112_2_18_1_6_58_383 "[handleAbortInd] tLabel = %d"
#define PF_A2DP_2296_112_2_18_1_6_58_384 "[A2DP_Init] phandle = 0x%x"
#define PF_A2DP_2320_112_2_18_1_6_58_385 "[A2DP_Deinit] state = %s(%d), isAudioOpen = %d, audio_timer = %d"
#define PF_A2DP_2357_112_2_18_1_6_58_386 "[A2DP_setSbcCapabilites] sampleFreq = %d, quality = %d, transSamplerate = %d"
#define PF_A2DP_2379_112_2_18_1_6_58_387 "[DRV_BT]<ASSERT_ERROR>: A2DP_setSbcCapabilites"
#define PF_A2DP_2409_112_2_18_1_6_58_388 "[A2DP_startConnecting] addr = %04x:%02x:%06x"
#define PF_A2DP_2411_112_2_18_1_6_58_389 "[A2DP_startConnecting] state = %s (%d)"
#define PF_A2DP_2438_112_2_18_1_6_58_390 "[A2DP_startConnecting]addr =%x:%x:%x"
#define PF_A2DP_2459_112_2_18_1_6_58_391 "[A2DP_startDisconnect] state = %s (%d)"
#define PF_A2DP_2474_112_2_18_1_6_58_392 "[A2DP_startStream] sample_freq = %d"
#define PF_A2DP_2476_112_2_18_1_6_58_393 "[A2DP_startStream] state = %s (%d), role = %d"
#define PF_A2DP_2517_112_2_18_1_6_58_394 "[A2DP_stopStream] state = %s (%d)"
#define PF_A2DP_2536_112_2_18_1_6_58_395 "[A2DP_suspendStream] state = %s (%d)"
#define PF_A2DP_2572_112_2_18_1_6_58_396 "[A2DP_startAbort] state = %s (%d)"
#define PF_A2DP_2626_112_2_18_1_6_58_397 "[A2DP_handlePrim] state = %s (%d)"
#define PF_A2DP_2627_112_2_18_1_6_58_398 "[A2DP_handlePrim] primType = 0x%x --"
#define PF_A2DP_2631_112_2_18_1_6_58_399 "AV_ACTIVATE_CFM"
#define PF_A2DP_2637_112_2_18_1_6_58_400 "AV_DEACTIVATE_CFM"
#define PF_A2DP_2642_112_2_18_1_6_58_401 "AV_CONNECT_IND"
#define PF_A2DP_2647_112_2_18_1_6_58_402 "AV_CONNECT_CFM"
#define PF_A2DP_2652_112_2_18_1_6_58_403 "AV_DISCONNECT_IND"
#define PF_A2DP_2657_112_2_18_1_6_58_404 "AV_DISCOVER_CFM"
#define PF_A2DP_2662_112_2_18_1_6_58_405 "AV_DISCOVER_IND"
#define PF_A2DP_2667_112_2_18_1_6_58_406 "AV_GET_CAPABILITIES_CFM"
#define PF_A2DP_2672_112_2_18_1_6_58_407 "AV_GET_CAPABILITIES_IND"
#define PF_A2DP_2677_112_2_18_1_6_58_408 "AV_SET_CONFIGURATION_CFM"
#define PF_A2DP_2682_112_2_18_1_6_58_409 "AV_SET_CONFIGURATION_IND"
#define PF_A2DP_2687_112_2_18_1_6_58_410 "AV_OPEN_CFM"
#define PF_A2DP_2692_112_2_18_1_6_58_411 "AV_OPEN_IND"
#define PF_A2DP_2697_112_2_18_1_6_58_412 "AV_START_CFM"
#define PF_A2DP_2702_112_2_18_1_6_58_413 "AV_START_IND"
#define PF_A2DP_2707_112_2_18_1_6_58_414 "AV_CLOSE_CFM"
#define PF_A2DP_2712_112_2_18_1_6_58_415 "AV_CLOSE_IND"
#define PF_A2DP_2717_112_2_18_1_6_58_416 "AV_SUSPEND_CFM"
#define PF_A2DP_2722_112_2_18_1_6_58_417 "AV_SUSPEND_IND"
#define PF_A2DP_2727_112_2_18_1_6_58_418 "AV_QOS_IND"
#define PF_A2DP_2732_112_2_18_1_6_58_419 "AV_STREAM_MTU_SIZE_IND"
#define PF_A2DP_2737_112_2_18_1_6_58_420 "AV_RECONFIGURE_CFM"
#define PF_A2DP_2742_112_2_18_1_6_58_421 "AV_RECONFIGURE_IND"
#define PF_A2DP_2747_112_2_18_1_6_58_422 "AV_GET_CONFIGURATION_CFM"
#define PF_A2DP_2752_112_2_18_1_6_58_423 "AV_GET_CONFIGURATION_IND"
#define PF_A2DP_2757_112_2_18_1_6_58_424 "AV_ABORT_CFM"
#define PF_A2DP_2762_112_2_18_1_6_58_425 "AV_ABORT_IND"
#define PF_A2DP_2767_112_2_18_1_6_58_426 "AV_REGISTER_STREAM_HANDLE_CFM"
#define PF_A2DP_2771_112_2_18_1_6_58_427 "[DRV_BT]<ASSERT_ERROR> [A2DP_handlePrim] Unknown primType = 0x%x"
#define PF_A2DP_2794_112_2_18_1_6_59_428 "[AVRCP_handlePrim] primType = 0x%x --"
#define PF_A2DP_2799_112_2_18_1_6_59_429 "AVRCP_DISCONNECT_IND"
#define PF_A2DP_2809_112_2_18_1_6_59_430 "AVRCP_ACTIVATE_CFM"
#define PF_A2DP_2815_112_2_18_1_6_59_431 "[DRV_BT]<ASSERT_ERROR> AVRCP_CONNECT_CFM"
#define PF_A2DP_2826_112_2_18_1_6_59_432 "AVRCP_CONNECT_IND"
#define PF_A2DP_2828_112_2_18_1_6_59_433 "deviceAddr = %04x:%02x:%06x"
#define PF_A2DP_2843_112_2_18_1_6_59_434 "AVRCP_PASS_THROUGH_COMMAND_IND"
#define PF_A2DP_2844_112_2_18_1_6_59_435 "tLabel = %d"
#define PF_A2DP_2845_112_2_18_1_6_59_436 "invalidProfile = %d"
#define PF_A2DP_2846_112_2_18_1_6_59_437 "profileIdentifier = 0x%x"
#define PF_A2DP_2847_112_2_18_1_6_59_438 "cType = %d"
#define PF_A2DP_2848_112_2_18_1_6_59_439 "subunit_type = %d"
#define PF_A2DP_2849_112_2_18_1_6_59_440 "subunitId = %d"
#define PF_A2DP_2850_112_2_18_1_6_59_441 "dataLen = %d"
#define PF_A2DP_2862_112_2_18_1_6_59_442 "VOLUP_PASS_THROUGH"
#define PF_A2DP_2866_112_2_18_1_6_59_443 "VOLUP OFF"
#define PF_A2DP_2872_112_2_18_1_6_59_444 "VOLUP ON"
#define PF_A2DP_2879_112_2_18_1_6_59_445 "VOLDOWN_PASS_THROUGH"
#define PF_A2DP_2883_112_2_18_1_6_59_446 "VOLDOWN OFF"
#define PF_A2DP_2889_112_2_18_1_6_59_447 "VOLDOWN ON"
#define PF_A2DP_2897_112_2_18_1_6_59_448 "PLAY_PASS_THROUGH"
#define PF_A2DP_2901_112_2_18_1_6_59_449 "PLAY OFF"
#define PF_A2DP_2907_112_2_18_1_6_59_450 "PLAY ON"
#define PF_A2DP_2914_112_2_18_1_6_59_451 "STOP_PASS_THROUGH"
#define PF_A2DP_2918_112_2_18_1_6_59_452 "STOP OFF"
#define PF_A2DP_2924_112_2_18_1_6_59_453 "STOP ON"
#define PF_A2DP_2931_112_2_18_1_6_59_454 "PAUSE_PASS_THROUGH"
#define PF_A2DP_2935_112_2_18_1_6_59_455 "PAUSE OFF"
#define PF_A2DP_2941_112_2_18_1_6_59_456 "PAUSE ON"
#define PF_A2DP_2948_112_2_18_1_6_59_457 "FORWARD_PASS_THROUGH"
#define PF_A2DP_2952_112_2_18_1_6_59_458 "FORWARD OFF\n"
#define PF_A2DP_2958_112_2_18_1_6_59_459 "FORWARD ON\n"
#define PF_A2DP_2966_112_2_18_1_6_59_460 "BACKWARD_PASS_THROUGH"
#define PF_A2DP_2970_112_2_18_1_6_59_461 "BACKWARD OFF\n"
#define PF_A2DP_2976_112_2_18_1_6_59_462 "BACKWARD ON\n"
#define PF_A2DP_2983_112_2_18_1_6_59_463 "unknown key command - 0x%x\n"
#define PF_A2DP_3028_112_2_18_1_6_59_464 "AVRCP_PASS_THROUGH_COMMAND_CFM"
#define PF_A2DP_3040_112_2_18_1_6_59_465 "AVRCP_UNIT_INFO_COMMAND_CFM\n"
#define PF_A2DP_3046_112_2_18_1_6_59_466 "AVRCP_SUBUNIT_INFO_COMMAND_CFM\n"
#define PF_A2DP_3052_112_2_18_1_6_59_467 "AVRCP_UNIT_INFO_COMMAND_IND"
#define PF_A2DP_3075_112_2_18_1_6_59_468 "AVRCP_SUBUNIT_INFO_COMMAND_IND"
#define PF_A2DP_3097_112_2_18_1_6_59_469 "app -default -primtype = 0x%0x\n"
#define PF_A2DP_3109_112_2_18_1_6_59_470 "[A2DP_audioStart] sampleRate = %d"
#define PF_A2DP_3111_112_2_18_1_6_59_471 "[A2DP_audioStart] state = %d"
#define PF_A2DP_3112_112_2_18_1_6_59_472 "[A2DP_audioStart] isAudioOpen = %d"
#define PF_A2DP_3117_112_2_18_1_6_59_473 "[DRV_BT]<ASSERT_ERROR> isAudioOpen = %d"
#define PF_A2DP_3131_112_2_18_1_6_59_474 "[A2DP_audioClose] state = %d"
#define PF_A2DP_3132_112_2_18_1_6_59_475 "[A2DP_audioClose] isAudioOpen = %d"
#define PF_A2DP_3152_112_2_18_1_6_59_476 "[A2DP_handleDeinitPrim] primType = 0x%x --"
#define PF_A2DP_3193_112_2_18_1_6_59_477 "[AVRCP_handleDeinitPrim] primType = 0x%x --"
#define PF_A2DP_3221_112_2_18_1_6_59_478 "[A2DP_IsBusy] state = 0x%x --"
#define SBC_PAL_113_112_2_18_1_7_0_479 "[BT]SBC_SetTaskPriority:Priority=0x%X\r\n"
#define SBC_PAL_152_112_2_18_1_7_0_480 "[SBC_doEncode] IN"
#define SBC_PAL_161_112_2_18_1_7_0_481 "[SBC_doEncode] audio buf is NULL and replace with mute data. mute_buf_size=%d\n"
#define SBC_PAL_169_112_2_18_1_7_0_482 "[SBC_doEncode] Memory alloc err!\n"
#define SBC_PAL_174_112_2_18_1_7_0_483 "[SBC_doEncode] malloc ok!\n"
#define SBC_PAL_180_112_2_18_1_7_0_484 "[SBC_doEncode] memset ok!\n"
#define SBC_PAL_199_112_2_18_1_7_0_485 "payload_size= %d, frames_per_payload = %d, data_cnt_per_frame = %d"
#define SBC_PAL_202_112_2_18_1_7_0_486 "[SBC_doEncode] audio_buf->data_count = %d\ndata_in_pos=%d\ndata_out_pos=%d\ndata_per_encoding=%d\nbuffer_size=%d\nframes_per_payload=%d\nbytes_per_payload=%d"
#define SBC_PAL_269_112_2_18_1_7_0_487 "[SBC_doEncode] no enough data"
#define SBC_PAL_412_112_2_18_1_7_0_488 "[SBC_doEncode] sbcPacketCount = %d"
#define SBC_PAL_448_112_2_18_1_7_1_489 "sbc_pal.c:config.alloc_method is %d,config.bitPool is %d,config.channel_mode is %d,config.nrof_blocks is %d,config.sample_freq is %d"
#define SBC_PAL_454_112_2_18_1_7_1_490 "sbc_pal.c:sbc_encode_init result is %d"
#define SBC_PAL_470_112_2_18_1_7_1_491 "sbc_pal.c:sbc_encode_init result error  is %d"
#define SBC_PAL_566_112_2_18_1_7_1_492 "[BT]SBC_Init:s_sbc_task_priority=0x%X\r\n"
#define SBC_PAL_773_112_2_18_1_7_1_493 "[PrepareSbc] maxFrameSize = %i"
#define SBC_PAL_775_112_2_18_1_7_1_494 "[PrepareSbc] channel_mode = %i, sample_freq = %i, blocks = %i"
#define SBC_PAL_777_112_2_18_1_7_1_495 "[PrepareSbc] subbands = %i, alloc_method = %d, maxBitpool = %in"
#define SBC_PAL_787_112_2_18_1_7_1_496 "[PrepareSbc] bitPool = %d, bitPoolAlt = %d, togglePeriod = %d"
#define SBC_PAL_799_112_2_18_1_7_1_497 "[SBC_Config] bitPool = %d"
#define SBC_PAL_808_112_2_18_1_7_1_498 "[PrepareSbc] framesize = %d"
#define SBC_PAL_848_112_2_18_1_7_2_499 "[PrepareSbc] framesize = %d, bitPool = %d, frames_per_payload = %d, data_per_encoding = %d"
#define SBC_PAL_DSP_155_112_2_18_1_7_2_500 "[SBC_Config_Callback] frame_size=%d"
#define SBC_PAL_DSP_439_112_2_18_1_7_3_501 "[SBC_DSP_Continue_Encode] audio buf is NULL or no use trackbuf"
#define SBC_PAL_DSP_446_112_2_18_1_7_3_502 "[SBC_DSP_Continue_Encode] mp3 buffer data is not enough!"
#define SBC_PAL_DSP_452_112_2_18_1_7_3_503 "[SBC_DSP_Continue_Encode] SBC packet out buf is full ,so wait some time to encode sbc frame"
#define SBC_PAL_DSP_621_112_2_18_1_7_3_504 "SBC_getPacket: sbcPacketCount = %d"
#define SBC_PAL_DSP_780_112_2_18_1_7_3_505 "[PrepareSbc] maxFrameSize = %i"
#define SBC_PAL_DSP_782_112_2_18_1_7_3_506 "[PrepareSbc] channel_mode = %i, sample_freq = %i, blocks = %i"
#define SBC_PAL_DSP_784_112_2_18_1_7_3_507 "[PrepareSbc] subbands = %i, alloc_method = %d, maxBitpool = %in"
#define SBC_PAL_DSP_795_112_2_18_1_7_3_508 "[PrepareSbc] bitPool = %d, bitPoolAlt = %d, togglePeriod = %d"
#define SBC_PAL_DSP_809_112_2_18_1_7_3_509 "[PrepareSbc] framesize = %d"
#define SBC_PAL_DSP_847_112_2_18_1_7_3_510 "[PrepareSbc] framesize = %d, bitPool = %d, frames_per_payload = %d, data_per_encoding = %d"
#define SBC_PAL_OTHER_122_112_2_18_1_7_4_511 "[PrepareSbc] maxFrameSize = %i"
#define SBC_PAL_OTHER_124_112_2_18_1_7_4_512 "[PrepareSbc] channel_mode = %i, sample_freq = %i, blocks = %i"
#define SBC_PAL_OTHER_126_112_2_18_1_7_4_513 "[PrepareSbc] subbands = %i, alloc_method = %d, maxBitpool = %in"
#define SBC_PAL_OTHER_136_112_2_18_1_7_4_514 "[PrepareSbc] bitPool = %d, bitPoolAlt = %d, togglePeriod = %d"
#define SBC_PAL_OTHER_150_112_2_18_1_7_4_515 "[PrepareSbc] framesize = %d"
#define SBC_PAL_OTHER_188_112_2_18_1_7_4_516 "[PrepareSbc] framesize = %d, bitPool = %d, frames_per_payload = %d, data_per_encoding = %d"
#define PF_APP_130_112_2_18_1_7_11_517 "rand = %x"
#define PF_APP_182_112_2_18_1_7_11_518 "[BT]APP_BtConfig"
#define PF_APP_208_112_2_18_1_7_11_519 "[BT]:APP_Init:BT Task Priority=0x%X\r\n"
#define PF_APP_233_112_2_18_1_7_11_520 "[BT]APP_DeInitBt\r\n"
#define PF_APP_247_112_2_18_1_7_11_521 "[BT]APP_BchsBuildAck:Bchs Buid Fail\r\n"
#define PF_APP_263_112_2_18_1_7_11_522 "[BT]APP_CheckBchsLink\r\n"
#define PF_APP_286_112_2_18_1_7_11_523 "[BT]APP_BuildBchsLink:BCHS State=0x%X\r\n"
#define PF_APP_314_112_2_18_1_7_11_524 "[DRV_BT]<ASSERT_ERROR> APP_BuildBchsLink: invalid s_bt_bchs_state = %d"
#define PF_APP_321_112_2_18_1_7_11_525 "[BT]APP_BtStartAgain:link_ready=0x%X\r\n"
#define PF_APP_367_112_2_18_1_7_11_526 "[BT]<ERROR>%s L%d: APP_BtStartTimeOut()"
#define PF_APP_369_112_2_18_1_7_11_527 "[BT]:link=0x%X, state=0x%X"
#define PF_APP_393_112_2_18_1_7_12_528 "[BT]<ERROR> %s L%d: SCI_CreateTimer() fail"
#define PF_APP_411_112_2_18_1_7_12_529 "[BT] CSR BCHS VERSION: %d.%d.%d.%d.%d\r\n"
#define PF_APP_412_112_2_18_1_7_12_530 "[BT] CSR BCHS feature list: 0x%08X\r\n"
#define PF_APP_414_112_2_18_1_7_12_531 "[BT]:APP_BtStart:bchs link isReady:0x%X,state:0x%X\r\n"
#define PF_APP_462_112_2_18_1_7_12_532 "[BT]APP_BtStopCallBack:link_ready=0x%X, state=0x%X\r\n"
#define PF_APP_470_112_2_18_1_7_12_533 "[BT]Rebuild BCHS Link........\r\n"
#define PF_APP_483_112_2_18_1_7_12_534 "[BT]Could Not Build The Connection Between BB and BT Chip.........\r\n"
#define PF_APP_493_112_2_18_1_7_12_535 "[BT]PF_ActivateService, service_map=0x%08x\r\n"
#define PF_APP_501_112_2_18_1_7_12_536 "[BT]PF_DeactivateServiceMap, service_map=0x%08x\r\n"
#define PF_APP_525_112_2_18_1_7_12_537 "[BT]APP_StopTimeOut bt_activated_serv_map=%x\r\n"
#define PF_APP_532_112_2_18_1_7_12_538 "[BT]APP_BtStop\r\n"
#define PF_APP_597_112_2_18_1_7_12_539 "[pf_app]timed_event_in fail"
#define PF_APP_611_112_2_18_1_7_12_540 "APP_CloseRunningService: service=0x%X\r\n"
#define PF_APP_644_112_2_18_1_7_12_541 "[DRV_BT]<ASSERT_ERROR> APP_CloseRunningService"
#define PF_APP_670_112_2_18_1_7_12_542 "[BT]APP_SetServerType:type=0x%08X\r\n"
#define PF_APP_688_112_2_18_1_7_12_543 "[BT]APP_InitProfile\r\n"
#define PF_APP_717_112_2_18_1_7_12_544 "[BT]APP_DeInitProfile:Active=0x%X\r\n"
#define PF_APP_779_112_2_18_1_7_12_545 "[BT]App_ActiveProfile\r\n"
#define PF_APP_813_112_2_18_1_7_12_546 "[BT]APP_ActiveA2DPCfm\r\n"
#define PF_APP_846_112_2_18_1_7_12_547 "[BT]APP_RemovePairedDevice:ret = 0x%X\r\n"
#define PF_APP_868_112_2_18_1_7_13_548 "[BT]APP_RefreshPairedDeviceService\r\n"
#define PF_APP_902_112_2_18_1_7_13_549 "[BT]APP_CancelOppSendFile\r\n"
#define PF_APP_909_112_2_18_1_7_13_550 "[BT]APP_OppReceiveFile\r\n"
#define PF_APP_916_112_2_18_1_7_13_551 "[BT]APP_CancelOppReceiveFile\r\n"
#define PF_APP_924_112_2_18_1_7_13_552 "[BT]APP_FtpSendFile\r\n"
#define PF_APP_929_112_2_18_1_7_13_553 "[BT]BT_FtpReceiveFile\r\n"
#define PF_APP_934_112_2_18_1_7_13_554 "[BT]BT_CancelFtpReceiveFile\r\n"
#define PF_APP_957_112_2_18_1_7_13_555 "[BT]APP_SendMsgToMmi:Msg_Id=0x%X, status=0x%X, body=0x%X, size=0x%X\r\n"
#define PF_APP_961_112_2_18_1_7_13_556 "[BT]APP_SendMsgToMmi:Invail Message\r\n"
#define PF_APP_976_112_2_18_1_7_13_557 "[BT]APP_SendMsgToMmi:No CallBack Function\r\n"
#define PF_APP_995_112_2_18_1_7_13_558 "[BT]APP_HandleMsg:event=0x%X\r\n"
#define PF_APP_1017_112_2_18_1_7_13_559 "[DRV_BT]<ASSERT_ERROR> SDP_PRIM "
#define PF_APP_1113_112_2_18_1_7_13_560 "[SD_PRIM] prim_type = 0x%x"
#define PF_APP_1120_112_2_18_1_7_13_561 "[SD_PRIM] prim_type = 0x%x"
#define PF_APP_1199_112_2_18_1_7_13_562 "[DRV_BT]<ASSERT_ERROR> APP_HandleMsg: default"
#define PF_APP_1216_112_2_18_1_7_13_563 "[_ft_task] SignalCode = %d"
#define PF_APP_1276_112_2_18_1_7_13_564 "[DRV_BT]<ASSERT_ERROR> _ft_task : default"
#define PF_CM_93_112_2_18_1_7_14_565 "[BT]TimeOutExitHandle:Type=0x%X\r\n"
#define PF_CM_122_112_2_18_1_7_14_566 "[BT]CM_TimeOutHandle:Type=0x%X,%x\r\n"
#define PF_CM_147_112_2_18_1_7_14_567 "[BT]:CM_TimeOutHandle - addr:lap:0x%X, uap:0x%X, nap:0x%X\r\n"
#define PF_CM_161_112_2_18_1_7_14_568 "[BT]<ERROR>%s L%d: SC_DEBOND_CFM TimeOut"
#define PF_CM_202_112_2_18_1_7_14_569 "[BT]CM_TimeOutHandle:Type  error"
#define PF_CM_212_112_2_18_1_7_14_570 "[BT]CM_SC_Init\r\n"
#define PF_CM_228_112_2_18_1_7_14_571 "[BT]CM_StartInquiry:classOfDeviceMask=0x%X,state=0x%X\r\n"
#define PF_CM_241_112_2_18_1_7_14_572 "[HFG]CM_StartInquiry creat cm_search_time fial"
#define PF_CM_248_112_2_18_1_7_14_573 "[BT]CM_StartInquiry:BT is inquirying.......\r\n"
#define PF_CM_264_112_2_18_1_7_14_574 "[BT]PF_AppStopInquiryHs:state=0x%X\r\n"
#define PF_CM_277_112_2_18_1_7_14_575 "[BT]CM_StopInquiry cancel time"
#define PF_CM_288_112_2_18_1_7_14_576 "[BT]CM_StopInquiry:BT is not in inqirying!\r\n"
#define PF_CM_316_112_2_18_1_7_14_577 "[BT]CM_SetVisibleCfm\r\n"
#define PF_CM_329_112_2_18_1_7_15_578 "[HFG]timed_event_in fail"
#define PF_CM_354_112_2_18_1_7_15_579 "[BT]CM_SetVisible:scan=0x%X, inquiry=0x%X, page=0x%X\r\n"
#define PF_CM_378_112_2_18_1_7_15_580 "[BT]timed_event_in fail"
#define PF_CM_393_112_2_18_1_7_15_581 "[BT]CM_RestoreVisible\r\n"
#define PF_CM_405_112_2_18_1_7_15_582 "[BT]CM_SetVisibleAtPowerOn:isVisibleAtPowerOn=0x%X\r\n"
#define PF_CM_422_112_2_18_1_7_15_583 "[BT]CM_ScanReadEnable\r\n"
#define PF_CM_428_112_2_18_1_7_15_584 "[BT]CM_ScanWriteEnable:inuiry=0x%X,page=0x%X\r\n"
#define PF_CM_434_112_2_18_1_7_15_585 "[BT]CM_ReadLocalName\r\n"
#define PF_CM_441_112_2_18_1_7_15_586 "[BT]CM_SetLocalName:name_ptr=%s\r\n"
#define PF_CM_448_112_2_18_1_7_15_587 "[BT]CM_ReadLocalBdAddr\r\n"
#define PF_CM_457_112_2_18_1_7_15_588 "[BT]CM_DeBondReq:state=0x%X,addr lap:0x%x,uap:0x%x,nap:0x%xrn"
#define PF_CM_527_112_2_18_1_7_15_589 "Devclass have been added in list "
#define PF_CM_533_112_2_18_1_7_15_590 " not a dongle device"
#define PF_CM_537_112_2_18_1_7_15_591 "AddDevicelist device_info= 0x%x,name[0]=0x%x,name[1]=0x%x,name[2]=0x%x"
#define PF_CM_568_112_2_18_1_7_15_592 " error ,error ,error !!! device is paired full"
#define PF_CM_571_112_2_18_1_7_15_593 "devcice number =%d"
#define PF_CM_606_112_2_18_1_7_15_594 "not find the device in the list"
#define PF_CM_628_112_2_18_1_7_15_595 "[CM_GetDeviceClass] IN ( BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x: )"
#define PF_CM_639_112_2_18_1_7_15_596 "device_class=0x%x,i=%d"
#define PF_CM_666_112_2_18_1_7_15_597 "[CM_GetDeviceClass] IN ( BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x: )"
#define PF_CM_676_112_2_18_1_7_15_598 "device_class=0x%x,i=%d"
#define PF_CM_692_112_2_18_1_7_15_599 "[BT]SC_SetPairDeviceStatus: status = %d\n"
#define PF_CM_701_112_2_18_1_7_15_600 "[BT]SC_SetPairDeviceAddr: is_paired = %d\n"
#define PF_CM_712_112_2_18_1_7_16_601 "[BT]SC_GetPairDeviceAddr: is_paired = %d\n"
#define PF_CM_755_112_2_18_1_7_16_602 "SetDevicePaired device_class=0x%x,i=%d"
#define PF_CM_760_112_2_18_1_7_16_603 "error,error,device don't find"
#define PF_CM_782_112_2_18_1_7_16_604 "[BT]CM_MsgPrimHandle: type = 0x%X\r\n"
#define PF_CM_790_112_2_18_1_7_16_605 "[BT]:CM_DISCOVERY_RESULT_IND - addr:lap:0x%X, uap:0x%X, nap:0x%X\r\n"
#define PF_CM_794_112_2_18_1_7_16_606 "[BT]CM_DISCOVERY_RESULT_IND:Search a same device\r\n"
#define PF_CM_806_112_2_18_1_7_16_607 "[BT]CM_DISCOVERY_RESULT_IND:Search a empty device name\r\n"
#define PF_CM_848_112_2_18_1_7_16_608 "[BT] MsgPrimHandle CM_DISCOVERY_CFM"
#define PF_CM_853_112_2_18_1_7_16_609 "[BT] MsgPrimHandle cancel serach time"
#define PF_CM_862_112_2_18_1_7_16_610 "[BT]CM_CANCEL_DISCOVERY_CFM:state=%d\r\n"
#define PF_CM_897_112_2_18_1_7_16_611 "[BT]CM_CANCEL_DISCOVERY_CFM:error state\r\n"
#define PF_CM_906_112_2_18_1_7_16_612 "[BT]CM_MsgPrimHandle: name = %s\r\n"
#define PF_CM_930_112_2_18_1_7_16_613 "[BT]CM_READ_REMOTE_NAME_CFM:resul = 0x%X\r\n"
#define PF_CM_955_112_2_18_1_7_16_614 "[BT]CM_SET_LOCAL_NAME_CFM:result = 0x%X, start=0x%X\r\n"
#define PF_CM_988_112_2_18_1_7_16_615 "[BT]CM_READ_LOCAL_BD_ADDR_CFM:lap:%d, uap:%d,nap:%d\r\n"
#define PF_CM_997_112_2_18_1_7_16_616 "[BT]Scan Read result:= 0x%x, scanEnable:= 0x%x\r\n"
#define PF_CM_1044_112_2_18_1_7_16_617 "[BT]CM_READ_SCAN_ENABLE_CFM:Invaild Message\r\n"
#define PF_CM_1070_112_2_18_1_7_16_618 "[DRV_BT]<ASSERT_ERROR> PF_MsgCmPrimHandle:Unhandle\r\n"
#define PF_CM_1102_112_2_18_1_7_16_619 "[BT]_StartPairing: deviceAddr = %04X:%02X:%06X"
#define PF_CM_1126_112_2_18_1_7_16_620 "[BT]SC_StartPairing:state=0x%X\r\n"
#define PF_CM_1134_112_2_18_1_7_16_621 "[BT]SC_StartPairing:Cancel Discovery at first!\r\n"
#define PF_CM_1152_112_2_18_1_7_16_622 "[BT]SC_CancelPairing: state=0x%X, deviceAddr = %04X:%02X:%06X"
#define PF_CM_1194_112_2_18_1_7_17_623 "[BT]SC_CancelPairing: Empty Operate!\r\n"
#define PF_CM_1208_112_2_18_1_7_17_624 "[BT]SC_InputPairPin:lap=0x%X,uap=0x%X, nap=0x%X\r\n"
#define PF_CM_1236_112_2_18_1_7_17_625 "[BT]SC_MsgPrimHandle:type = 0x%X\r\n"
#define PF_CM_1245_112_2_18_1_7_17_626 "[BT]SC_PASSKEY_IND:lap=0x%X,uap=0x%X, nap=0x%X\r\n"
#define PF_CM_1295_112_2_18_1_7_17_627 "[BT]SC_BOND_CFM: result= 0x%X,cod=%x,class=%x\r\n"
#define PF_CM_1311_112_2_18_1_7_17_628 "[BT]<INFO> cod = 0x%X, it's a Audio Device"
#define PF_CM_1328_112_2_18_1_7_17_629 "[BT] state is idle"
#define PF_CM_1334_112_2_18_1_7_17_630 "[BT]BCHS_BONDING_CANCELLED SUCCESSFUL "
#define PF_CM_1351_112_2_18_1_7_17_631 "[BT]SC_BOND_IND:result= 0x%X\r\n"
#define PF_CM_1368_112_2_18_1_7_17_632 "[BT]<INFO> cod = 0x%X, it's a Audio Device"
#define PF_CM_1389_112_2_18_1_7_17_633 "[BT]SC_AUTHORISE_IND\r\n"
#define PF_CM_1401_112_2_18_1_7_17_634 "[BT]SC_DEBOND_CFM:result=0x%X\r\n"
#define PF_CM_1414_112_2_18_1_7_17_635 "[DRV_BT]<ASSERT_ERROR> default in scPrim handler %x,\n"
#define PF_FILE_56_112_2_18_1_7_19_636 "[BT]PF_fread length is zore!\r\n"
#define PF_FILE_80_112_2_18_1_7_19_637 "[BT]PF_fread length is zore!\r\n"
#define PF_FILE_101_112_2_18_1_7_19_638 "[BT]<ERROR> file_name invalid"
#define PF_FTP_CLIENT_258_112_2_18_1_7_20_639 "[BT]<FTC> ListFolder total_length = %d, data_length = %d, "
#define PF_FTP_CLIENT_266_112_2_18_1_7_20_640 "[BT]<FTC> ListFolder string = %s"
#define PF_FTP_CLIENT_288_112_2_18_1_7_20_641 "[BT]<FTC> ListFolder: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    "
#define PF_FTP_CLIENT_310_112_2_18_1_7_20_642 "[BT]<FTC> ListFolderBody data_length = %d, final = %d, data = 0x%x"
#define PF_FTP_CLIENT_319_112_2_18_1_7_21_643 "[BT]<FTC> ListFolder string = %s"
#define PF_FTP_CLIENT_341_112_2_18_1_7_21_644 "[BT]<FTC> ListFolder: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    "
#define PF_FTP_CLIENT_360_112_2_18_1_7_21_645 "[BT]<FTC> FTC_ChangeClk: frq_cnt = %d"
#define PF_FTP_CLIENT_377_112_2_18_1_7_21_646 "[BT]<FTC> FTC_RestoreClk: frq_cnt = %d"
#define PF_FTP_CLIENT_401_112_2_18_1_7_21_647 "[BT]<FTC> FTC_IsBusy: state=0x%X"
#define PF_FTP_CLIENT_417_112_2_18_1_7_21_648 "[BT]<FTC> FTC_Init state = %d"
#define PF_FTP_CLIENT_433_112_2_18_1_7_21_649 "[BT]<FTC> FTC_DeInit state = %d"
#define PF_FTP_CLIENT_466_112_2_18_1_7_21_650 "[BT]<FTC> FTC_ConnectServer: "
#define PF_FTP_CLIENT_507_112_2_18_1_7_21_651 "[BT]<FTC> FTC_CancelConnectServer "
#define PF_FTP_CLIENT_531_112_2_18_1_7_21_652 "[BT]<FTC> FTC_SetPath: state = %d"
#define PF_FTP_CLIENT_563_112_2_18_1_7_21_653 "[BT]<FTC> FTC_GoBackPath: state = %d"
#define PF_FTP_CLIENT_592_112_2_18_1_7_21_654 "[BT]<FTC> FTC_GoRootPath: state = %d"
#define PF_FTP_CLIENT_623_112_2_18_1_7_21_655 "[BT]<FTC> FTC_AddNewFolder: state = %d"
#define PF_FTP_CLIENT_661_112_2_18_1_7_21_656 "[BT]<FTC> FTC_SendFile state = %d"
#define PF_FTP_CLIENT_676_112_2_18_1_7_21_657 "[BT]<ERROR> %s L%d: openfile"
#define PF_FTP_CLIENT_683_112_2_18_1_7_21_658 "[BT]<FTC> FTC_SendFile file_name_len = %d, file_size = %d"
#define PF_FTP_CLIENT_726_112_2_18_1_7_21_659 "[BT]<FTC> FTC_ReceiveFile: state = %d"
#define PF_FTP_CLIENT_739_112_2_18_1_7_21_660 "[BT]<ERROR> %s L%d: openfile"
#define PF_FTP_CLIENT_745_112_2_18_1_7_21_661 "[BT]<FTC> FTC_ReceiveFile: file_name_len = %d, file_size = %d"
#define PF_FTP_CLIENT_786_112_2_18_1_7_21_662 "[BT]<FTC> FTC_Cancel: state = 0x%X\r\n"
#define PF_FTP_CLIENT_820_112_2_18_1_7_21_663 "[BT]<FTC> FTC_DeleteObject: state = %d"
#define PF_FTP_CLIENT_845_112_2_18_1_7_21_664 "[BT]<FTC> FTC_DisconnectServer: state = %d"
#define PF_FTP_CLIENT_871_112_2_18_1_7_22_665 "[BT]<FTC> result = 0x%x\r\n"
#define PF_FTP_CLIENT_907_112_2_18_1_7_22_666 "[BT]<FTC> state = 0x%X\r\n"
#define PF_FTP_CLIENT_950_112_2_18_1_7_22_667 "[BT]<FTC> SetFolder result = 0x%x\r\n"
#define PF_FTP_CLIENT_1099_112_2_18_1_7_22_668 "[BT]<FTC> PUT Header: result = 0x%X, bodysize = %d, state = %d, "
#define PF_FTP_CLIENT_1128_112_2_18_1_7_22_669 "[BT]<ERROR> %s L%d: read_len(%d) != send_len(%d)"
#define PF_FTP_CLIENT_1193_112_2_18_1_7_22_670 "[BT]<FTC> PUT BODY: result = 0x%X, bodysize = %d, "
#define PF_FTP_CLIENT_1244_112_2_18_1_7_22_671 "[BT]<ERROR> %s L%d: read_len(%d) != send_len(%d)"
#define PF_FTP_CLIENT_1308_112_2_18_1_7_22_672 "[BT]<FTC> GET Header: result = 0x%X, finalFlag = %d, "
#define PF_FTP_CLIENT_1324_112_2_18_1_7_22_673 "[BT]<ERROR> %s L%d: write_len(%d) != received_len(%d)"
#define PF_FTP_CLIENT_1404_112_2_18_1_7_23_674 "[BT]<FTC> GET BODY: result = 0x%X, finalFlag = %d, "
#define PF_FTP_CLIENT_1420_112_2_18_1_7_23_675 "[BT]<ERROR> %s L%d: write_len(%d) != received_len(%d)"
#define PF_FTP_CLIENT_1489_112_2_18_1_7_23_676 "[BT]<FTC> SetFolder result = 0x%x\r\n"
#define PF_FTP_CLIENT_1518_112_2_18_1_7_23_677 "[BT]<FTC> SetFolder result = 0x%x\r\n"
#define PF_FTP_CLIENT_1597_112_2_18_1_7_23_678 "[BT]<FTC> FTC_HandlePrim: type = 0x%x\r\n"
#define PF_FTP_CLIENT_1611_112_2_18_1_7_23_679 "[BT]<WARNING> g_ftc_handle[%d] is invalid, array size = %d"
#define PF_FTP_CLIENT_1616_112_2_18_1_7_23_680 "[BT]<FTC> FTC_HandlePrim: id = %d, %srn"
#define PF_FTP_SERVER_169_112_2_18_1_7_24_681 "[DRV_BT] FTS_Init_RES(): s_fts_dummy = 0x%X, g_ftp_tmp_path = 0x%X, "
#define PF_FTP_SERVER_178_112_2_18_1_7_24_682 "[DRV_BT] FTS_DeInit_RES(in): s_fts_dummy = 0x%X, g_ftp_tmp_path = 0x%X, "
#define PF_FTP_SERVER_210_112_2_18_1_7_24_683 "[DRV_BT] FTS_DeInit_RES(out): s_fts_dummy = 0x%X, g_ftp_tmp_path = 0x%X, "
#define PF_FTP_SERVER_232_112_2_18_1_7_24_684 "[BT]FTS_GetState error usr_addr "
#define PF_FTP_SERVER_240_112_2_18_1_7_24_685 "[BT]FTS_ChangeClk:Change System Clock;cnt =0x%X\r\n"
#define PF_FTP_SERVER_251_112_2_18_1_7_24_686 "[BT]FTS_RestoreClk:Restore System Clock=0x%X\r\n"
#define PF_FTP_SERVER_265_112_2_18_1_7_24_687 "[BT]FTS_Active\r\n"
#define PF_FTP_SERVER_275_112_2_18_1_7_24_688 "[BT]FTS_DeActive\r\n"
#define PF_FTP_SERVER_282_112_2_18_1_7_24_689 "[BT][FTS_ServiceClose] IN"
#define PF_FTP_SERVER_283_112_2_18_1_7_24_690 "[BT][FTS_ServiceClose] status = %d"
#define PF_FTP_SERVER_311_112_2_18_1_7_24_691 "[BT]FTS_SetRootFolder:state=0x%X oper_state=0x%Xrn"
#define PF_FTP_SERVER_316_112_2_18_1_7_24_692 "[BT]FTS_SetRootFolder:Should not reset root folder\r\n"
#define PF_FTP_SERVER_323_112_2_18_1_7_24_693 "[BT]FTS_SetRootFolder:Name[%d]=0x%X\r\n"
#define PF_FTP_SERVER_331_112_2_18_1_7_24_694 "[BT] FTS_Init:Root Folder is not exist\r\n"
#define PF_FTP_SERVER_382_112_2_18_1_7_24_695 "[BT]<ERROR>%s L%d: FTS_GetFolderInfoLength(): malloc() error"
#define PF_FTP_SERVER_402_112_2_18_1_7_24_696 "[BT] FTS_GetFolderInfoLength w_name_len=%d, utf8_len=%d"
#define PF_FTP_SERVER_443_112_2_18_1_7_24_697 "[BT] FTS_GetFolderInfoLength w_name_len=%d, utf8_len=%d"
#define PF_FTP_SERVER_482_112_2_18_1_7_24_698 "[BT]FTS_GetFolderInformation: The Folder is empty \r\n"
#define PF_FTP_SERVER_550_112_2_18_1_7_24_699 "[BT]FTS_GetListFolderLen:buf_len= 0x%X\r\n"
#define PF_FTP_SERVER_556_112_2_18_1_7_24_700 "[BT]FTS_GetListFolderLen:Is Empty\r\n"
#define PF_FTP_SERVER_577_112_2_18_1_7_24_701 "[BT]FTS_GetListFolderInfo:Memory allocate error!\n"
#define PF_FTP_SERVER_586_112_2_18_1_7_24_702 "[BT]<ERROR>%s L%d: FTS_GetListFolderInfo(): malloc() error"
#define PF_FTP_SERVER_640_112_2_18_1_7_25_703 "[BT]FTS_GetListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_671_112_2_18_1_7_25_704 "[BT]FTS_GetListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_710_112_2_18_1_7_25_705 "[BT]FTS_GetListFolderInfo:line=%d, bytes_allocated_len=%d,left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_719_112_2_18_1_7_25_706 "[BT]FTS_GetListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_750_112_2_18_1_7_25_707 "[BT]FTS_GetListFolderInfo:line=%d, bytes_allocated_len=%d,left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_758_112_2_18_1_7_25_708 "[BT]FTS_GetListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_774_112_2_18_1_7_25_709 "[BT]FTS_FolderInfoToXml: The Folder is empty \r\n"
#define PF_FTP_SERVER_790_112_2_18_1_7_25_710 "[BT]FTS_GetListFolderInfo:line=%d, left_buf_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_794_112_2_18_1_7_25_711 "[BT]FTS_GetListFolderInfo:left_len=%d, bytes_to_write=%d, fts_file_folder_count=%d\n"
#define PF_FTP_SERVER_816_112_2_18_1_7_25_712 "[BT]FTS_GetNextListFolderInfo:Memory allocate error!\n"
#define PF_FTP_SERVER_826_112_2_18_1_7_25_713 "[BT]FTS_GetNextListFolderInfo: There is no data left.!!!!!\n"
#define PF_FTP_SERVER_831_112_2_18_1_7_25_714 "[BT]FTS_GetNextListFolderInfo:allcated_len=%d, left_buf_len=%d, object_left_len=%d, fts_file_folder_count=%dn"
#define PF_FTP_SERVER_837_112_2_18_1_7_25_715 "[BT]<ERROR>%s L%d: malloc() error"
#define PF_FTP_SERVER_878_112_2_18_1_7_25_716 "[BT]FTS_GetNextListFolderInfo:line=%d, bytes_allocated_len=%d,left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_886_112_2_18_1_7_25_717 "[BT]FTS_GetNextListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_912_112_2_18_1_7_25_718 "[BT]FTS_GetNextListFolderInfo:line=%d, bytes_allocated_len=%d,left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_920_112_2_18_1_7_25_719 "[BT]FTS_GetNextListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_944_112_2_18_1_7_25_720 "[BT]FTS_GetNextListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_949_112_2_18_1_7_25_721 "[BT]FTS_GetNextListFolderInfo:left_len=%d, bytes_to_write=%d\n"
#define PF_FTP_SERVER_957_112_2_18_1_7_25_722 "[BT]FTS_IsBusy:state=0x%X\r\n"
#define PF_FTP_SERVER_966_112_2_18_1_7_25_723 "[BT]FTS_Init\r\n"
#define PF_FTP_SERVER_985_112_2_18_1_7_25_724 "[BT]FTS_Init:Name[%d]=0x%X\r\n"
#define PF_FTP_SERVER_996_112_2_18_1_7_25_725 "[BT]FTS_DeInit\r\n"
#define PF_FTP_SERVER_1038_112_2_18_1_7_25_726 "[BT]FTS_ReceiveFileFail\r\n"
#define PF_FTP_SERVER_1055_112_2_18_1_7_25_727 "[BT]FTS_ReceiveFileFinalPacket\r\n"
#define PF_FTP_SERVER_1105_112_2_18_1_7_26_728 "[BT]FTS_GET_OBJ_IND:Name[%d] = 0x%X\r\n"
#define PF_FTP_SERVER_1112_112_2_18_1_7_26_729 "[BT]FTS_GET_OBJ_IND:Failed to open the file \r\n"
#define PF_FTP_SERVER_1135_112_2_18_1_7_26_730 "[BT]FTS_GET_OBJ_IND:bytesToRead=%d \r\n"
#define PF_FTP_SERVER_1140_112_2_18_1_7_26_731 "[BT]FTS_GET_OBJ_IND:Failed to read the file \r\n"
#define PF_FTP_SERVER_1233_112_2_18_1_7_26_732 "[BT]FTS_ReceiveFile\r\n"
#define PF_FTP_SERVER_1249_112_2_18_1_7_26_733 "[BT]FTS_SetRootFolder:w_len=0x%X\r\n"
#define PF_FTP_SERVER_1389_112_2_18_1_7_26_734 "[bt] cancel ftp timer g_Timeoutcancel_timer=0x%x"
#define PF_FTP_SERVER_1406_112_2_18_1_7_26_735 "send putting the cancel command"
#define PF_FTP_SERVER_1422_112_2_18_1_7_26_736 "send getting the cancel command"
#define PF_FTP_SERVER_1438_112_2_18_1_7_26_737 "send opc the cancel command"
#define PF_FTP_SERVER_1480_112_2_18_1_7_26_738 "Handle_Timeout_Cancel_Msg start"
#define PF_FTP_SERVER_1489_112_2_18_1_7_26_739 "[BT_pf_ftp_server] SCI_GetEvent(s_bt_ftp_timeout_envet)="
#define PF_FTP_SERVER_1501_112_2_18_1_7_26_740 "put_file_handler release resourse successful"
#define PF_FTP_SERVER_1509_112_2_18_1_7_26_741 "get_file_handler release resourse successful"
#define PF_FTP_SERVER_1515_112_2_18_1_7_26_742 "OPC_CloseRES successful"
#define PF_FTP_SERVER_1520_112_2_18_1_7_26_743 "none operate out here"
#define PF_FTP_SERVER_1546_112_2_18_1_7_26_744 "g_Timeoutcancel_timer =0x%x"
#define PF_FTP_SERVER_1553_112_2_18_1_7_26_745 "ERROR: [BT_pf_ftp_server] s_bt_ftp_timeout_envet create error"
#define PF_FTP_SERVER_1557_112_2_18_1_7_26_746 "[BT_pf_ftp_server] s_bt_ftp_timeout_envet = 0x%x"
#define PF_FTP_SERVER_1602_112_2_18_1_7_27_747 "[BT]FTS_CancelPutFile:state=0x%X\r\n"
#define PF_FTP_SERVER_1632_112_2_18_1_7_27_748 "[BT]FTS_CancelPutFile:Invaild Cancel Received File\r\n"
#define PF_FTP_SERVER_1643_112_2_18_1_7_27_749 "[BT]FTS_CancelGetFile:state=0x%X\r\n"
#define PF_FTP_SERVER_1666_112_2_18_1_7_27_750 "[BT]FTS_CancelReceive:Invaild Cancel Received File\r\n"
#define PF_FTP_SERVER_1703_112_2_18_1_7_27_751 "[BT]FTS_SendFile: oper_state:0x%X\r\n"
#define PF_FTP_SERVER_1711_112_2_18_1_7_27_752 "[BT]FTS_SendFile: No operation\r\n"
#define PF_FTP_SERVER_1721_112_2_18_1_7_27_753 "[BT]FTS_ReceiveFile: oper_state:0x%X\r\n"
#define PF_FTP_SERVER_1729_112_2_18_1_7_27_754 "[BT]FTS_ReceiveFile: No operation\r\n"
#define PF_FTP_SERVER_1740_112_2_18_1_7_27_755 "[BT]FTS_CancelTransfer: oper_state:0x%X\r\n"
#define PF_FTP_SERVER_1752_112_2_18_1_7_27_756 "[BT]FTS_CancelReceive: No operation\r\n"
#define PF_FTP_SERVER_1764_112_2_18_1_7_27_757 "[BT]FTS_CreateNewFolder:is_enable=0x%X\r\n"
#define PF_FTP_SERVER_1779_112_2_18_1_7_27_758 "[BT]FTS_CreateNewFolder:Folder create successful\r\n"
#define PF_FTP_SERVER_1784_112_2_18_1_7_27_759 "[BT]FTS_CreateNewFolder:Folder Create Fail\r\n"
#define PF_FTP_SERVER_1791_112_2_18_1_7_27_760 "[BT]FTS_CreateNewFolder:Cancel Folder Create\r\n"
#define PF_FTP_SERVER_1805_112_2_18_1_7_27_761 "[BT]FTS_DeleteFile:is_enable = 0x%X, oper_state=0x%X\r\n"
#define PF_FTP_SERVER_1809_112_2_18_1_7_27_762 "[BT]FTS_DeleteFile:Error Delete\r\n"
#define PF_FTP_SERVER_1821_112_2_18_1_7_27_763 "[BT]FTS_DeleteFile:Name[%d]=0x%04X\r\n"
#define PF_FTP_SERVER_1830_112_2_18_1_7_27_764 "[BT]FTS_DeleteFile:Delete Fail\r\n"
#define PF_FTP_SERVER_1839_112_2_18_1_7_27_765 "[BT]FTS_DeleteFile: Cancel File Delete\r\n"
#define PF_FTP_SERVER_1863_112_2_18_1_7_27_766 "[BT]FTS_HandlePrim: type = 0x%X\r\n"
#define PF_FTP_SERVER_1873_112_2_18_1_7_27_767 "[BT]FTS_CONNECT_IND:state = 0x%X\r\n"
#define PF_FTP_SERVER_1881_112_2_18_1_7_27_768 "[BT]FTS_CONNECT_IND:Opp is busy now, could not respond connect request:is_have_disk =0x%X,w_len=%d\r\n"
#define PF_FTP_SERVER_1892_112_2_18_1_7_27_769 "[BT]FTS_CONNECT_IND:connect=0x%X\r\n"
#define PF_FTP_SERVER_1903_112_2_18_1_7_27_770 "[BT]reset root folder fail\r\n"
#define PF_FTP_SERVER_1923_112_2_18_1_7_27_771 "[BT]FTS_CONNECT_IND:no default root to set:is_have_disk =0x%Xrn"
#define PF_FTP_SERVER_1962_112_2_18_1_7_27_772 "[BT]FTS_HandlePrim:FTS_PUT_OBJ_IND-state = 0x%X, oper_state=0x%X, arribute =0x%X\r\n"
#define PF_FTP_SERVER_1966_112_2_18_1_7_27_773 "[BT]FTS_PUT_OBJ_IND:Opp is busy now, could not respond put object:is_have_disk =0x%X\r\n"
#define PF_FTP_SERVER_1984_112_2_18_1_7_27_774 "[BT]FTS_HandlePrim:Delete Operate is time out\r\n"
#define PF_FTP_SERVER_1989_112_2_18_1_7_27_775 "[BT]FTS_HandlePrim:FTS_PUT_OBJ_IND,One file is transmitting.......\r\n"
#define PF_FTP_SERVER_2013_112_2_18_1_7_27_776 "[BT]FTS_PUT_OBJ_IND:offset = 0x%X\r\n"
#define PF_FTP_SERVER_2046_112_2_18_1_7_27_777 "[BT] Name[%d]=0x%04x\r\n"
#define PF_FTP_SERVER_2051_112_2_18_1_7_27_778 "The Name is same as one folder\r\n"
#define PF_FTP_SERVER_2088_112_2_18_1_7_28_779 "The whole path name is too long:0x%X\r\n"
#define PF_FTP_SERVER_2104_112_2_18_1_7_28_780 "[BT]FTS_HandlePrim:FTS_PUT_OBJ_NEXT_IND-state = 0x%X, oper_state=0x%X\r\n"
#define PF_FTP_SERVER_2111_112_2_18_1_7_28_781 "[BT]FTS_HandlePrim:FTS_PUT_OBJ_NEXT_IND-handle = 0x%X, Cancel=0x%X\r\n"
#define PF_FTP_SERVER_2170_112_2_18_1_7_28_782 "[BT]Name[%d]=0x%X\r\n"
#define PF_FTP_SERVER_2221_112_2_18_1_7_28_783 "[BT]FTS_GET_OBJ_IND-state = 0x%X, oper_state=0x%X, Attribute=0x%X\r\n"
#define PF_FTP_SERVER_2224_112_2_18_1_7_28_784 "[BT]FTS_PUT_OBJ_IND:Opp is busy now, could not respond put object\r\n"
#define PF_FTP_SERVER_2237_112_2_18_1_7_28_785 "[BT]FTS_HandlePrim:FTS_GET_OBJ_IND,One file is transmitting.......\r\n"
#define PF_FTP_SERVER_2262_112_2_18_1_7_28_786 "[BT]FTS_GET_OBJ_IND:name[%d]=0x%X\r\n"
#define PF_FTP_SERVER_2303_112_2_18_1_7_28_787 "[BT]FTS_GET_OBJ_IND: File Open Error\r\n"
#define PF_FTP_SERVER_2314_112_2_18_1_7_28_788 "[BT]FTS_GET_OBJ_IND: File name is too long\r\n"
#define PF_FTP_SERVER_2338_112_2_18_1_7_28_789 "[BT]FTS_GET_OBJ_NEXT_IND-state = 0x%X, oper_state=0x%X\r\n"
#define PF_FTP_SERVER_2345_112_2_18_1_7_28_790 "[BT]FTS_GET_OBJ_NEXT_IND:Cancel=0x%X, handle=0x%X\r\n"
#define PF_FTP_SERVER_2365_112_2_18_1_7_28_791 "[BT]FTS_GET_OBJ_NEXT_IND:bytesToRead=%d\n"
#define PF_FTP_SERVER_2370_112_2_18_1_7_28_792 "[BT]FTS_GET_OBJ_NEXT_IND:File read error\r\n"
#define PF_FTP_SERVER_2456_112_2_18_1_7_28_793 "[DRV_BT]<ASSERT_ERROR>: FTS_AUTHENTICATE_IND "
#define PF_FTP_SERVER_2462_112_2_18_1_7_28_794 "[DRV_BT]<ASSERT_ERROR>: FTS_AUTHENTICATE_CFM"
#define PF_FTP_SERVER_2470_112_2_18_1_7_28_795 "[BT]FTS_ABORT_IND:state=0x%X\r\n"
#define PF_FTP_SERVER_2501_112_2_18_1_7_29_796 "[BT]FTS_ABORT_IND: Invail Operate\r\n"
#define PF_FTP_SERVER_2521_112_2_18_1_7_29_797 "[BT]FTS_DISCONNECT_IND:state=0x%X\r\n"
#define PF_FTP_SERVER_2555_112_2_18_1_7_29_798 "[BT]FTS_DISCONNECT_IND:ID_STATUS_FT_SERVER_ABORT_IND\r\n"
#define PF_FTP_SERVER_2582_112_2_18_1_7_29_799 "[BT]FTS_DEACTIVATE_CFM:Fts Server Deactivated...\n"
#define PF_FTP_SERVER_2594_112_2_18_1_7_29_800 "[BT]FTS_HandlePrim:FTS_DEL_OBJ_IND:state=0x%X\r\n"
#define PF_FTP_SERVER_2638_112_2_18_1_7_29_801 "[BT]FTS_DEL_OBJ_IND: Invaild operation\r\n"
#define PF_FTP_SERVER_2645_112_2_18_1_7_29_802 "[BT]FTS_DEL_OBJ_IND:is_del=0x%X, cancelputting=0x%X, cancelgetting=0x%X\r\n"
#define PF_FTP_SERVER_2660_112_2_18_1_7_29_803 "[BT]FTS_DEL_OBJ_IND: FTP Read Only!\r\n"
#define PF_FTP_SERVER_2693_112_2_18_1_7_29_804 "[BT]FTS_DEL_OBJ_IND: it is not an empty dir, can not del directly!\r\n"
#define PF_FTP_SERVER_2720_112_2_18_1_7_29_805 "[BT]FTS_DEL_OBJ_IND: File Not Exist!\r\n"
#define PF_FTP_SERVER_2744_112_2_18_1_7_29_806 "[BT]FTS_SET_FOLDER_IND\r\n"
#define PF_FTP_SERVER_2749_112_2_18_1_7_29_807 "w_len=%d,w_name_len=%d"
#define PF_FTP_SERVER_2754_112_2_18_1_7_29_808 "folder length  over 255"
#define PF_FTP_SERVER_2768_112_2_18_1_7_29_809 "[BT]FTS_SET_FOLDER_IND:The specified directory is getted\r\n"
#define PF_FTP_SERVER_2784_112_2_18_1_7_29_810 "w_len=%d folder length  over 256"
#define PF_FTP_SERVER_2795_112_2_18_1_7_29_811 "[BT]FTS_SET_FOLDER_IND:Name[%d]=0x%X\r\n"
#define PF_FTP_SERVER_2803_112_2_18_1_7_29_812 "[BT]FTS_SET_FOLDER_IND:No this path\r\n"
#define PF_FTP_SERVER_2825_112_2_18_1_7_29_813 "[BT]FTS_SET_BACK_FOLDER_IND:len=0x%X, root_len:0x%X\r\n"
#define PF_FTP_SERVER_2829_112_2_18_1_7_29_814 "[BT]FTS_SET_BACK_FOLDER_IND: w_len=%d,w_root_len=%d\r\n"
#define PF_FTP_SERVER_2864_112_2_18_1_7_29_815 "[BT]FTS_SET_BACK_FOLDER_IND:path[%d]=0x%X\r\n"
#define PF_FTP_SERVER_2871_112_2_18_1_7_29_816 "[BT]FTS_SET_BACK_FOLDER_IND: Not Found Folder\r\n"
#define PF_FTP_SERVER_2885_112_2_18_1_7_29_817 "w_len=:%d"
#define PF_FTP_SERVER_2888_112_2_18_1_7_29_818 "[DRV_BT]<ASSERT_ERROR> w_len exceeded !!!"
#define PF_FTP_SERVER_2897_112_2_18_1_7_29_819 "[BT:FTS] FTS_GET_LIST_FOLDER_IND:payload is NULL\r\n"
#define PF_FTP_SERVER_2905_112_2_18_1_7_29_820 "[BT:FTS] FTS_GET_LIST_FOLDER_IND:payload is not NULL\r\n"
#define PF_FTP_SERVER_2927_112_2_18_1_7_29_821 "[BT]FTS_GET_LIST_FOLDER_IND:len = %d maxPacketSize=%d byte_to_send = %d\r\n"
#define PF_FTP_SERVER_2950_112_2_18_1_7_29_822 "[BT]FTS_GET_LIST_FOLDER_IND:error path does not exists\n"
#define PF_FTP_SERVER_2972_112_2_18_1_7_29_823 "[BT]FTS_GET_LIST_FOLDER_NEXT_IND bytesToRead=%d folder_string_pos=%d\n"
#define PF_FTP_SERVER_2996_112_2_18_1_7_30_824 "[BT]FTS_GET_LIST_FOLDER_NEXT_IND:error path does not exists\n"
#define PF_FTP_SERVER_3004_112_2_18_1_7_30_825 "[BT]FTS_SET_ADD_FOLDER_IND\r\n"
#define PF_FTP_SERVER_3015_112_2_18_1_7_30_826 "[BT]FTS_SET_ADD_FOLDER_IND: FTP Read Only!\r\n"
#define PF_FTP_SERVER_3022_112_2_18_1_7_30_827 "[BT]FTS_SET_ADD_FOLDER_IND: Parent folder is not exist!\r\n"
#define PF_FTP_SERVER_3038_112_2_18_1_7_30_828 "[BT]FTS_SET_ADD_FOLDER_IND:Name[%d]=0x%04X\r\n"
#define PF_FTP_SERVER_3066_112_2_18_1_7_30_829 "[BT]FTS_SET_ADD_FOLDER_IND:Error, The Name is same as One File \r\n"
#define PF_FTP_SERVER_3072_112_2_18_1_7_30_830 "[BT]FTS_SET_ADD_FOLDER_IND: folder name is too long\r\n"
#define PF_FTP_SERVER_3100_112_2_18_1_7_30_831 "[BT]FTS_SET_ROOT_FOLDER_IND\r\n"
#define PF_FTP_SERVER_3108_112_2_18_1_7_30_832 "[BT]FTS_SET_ROOT_FOLDER_IND: Get File TimeOut: state=0x%X, operate_state=0x%X\r\n"
#define PF_FTP_SERVER_3128_112_2_18_1_7_30_833 "[DRV_BT]<ASSERT_ERROR>: FTS_HandlePrim: unkown type"
#define PF_FTP_SERVER_3235_112_2_18_1_7_30_834 "[DRV_BT]<ASSERT_ERROR>: FTS_DeInitHandlePrim: unkown type"
#define PF_HFG_208_112_2_18_1_7_31_835 "[BT]<INFO> HFG_AudioReqSend(): audio_on = %d, new_audio = %d "
#define PF_HFG_252_112_2_18_1_7_31_836 "[BT]<INFO> HFG_InitAudioConnection(): init audio link..."
#define PF_HFG_477_112_2_18_1_7_31_837 "[_HFG_HandleATCmd] token = 0x%x"
#define PF_HFG_503_112_2_18_1_7_31_838 "[ATD>] location = %s"
#define PF_HFG_512_112_2_18_1_7_31_839 "[ATD>] memory_indicator = %d"
#define PF_HFG_517_112_2_18_1_7_31_840 "[ATD] phone number = %s"
#define PF_HFG_533_112_2_18_1_7_32_841 "[ATD] phone_number = %s"
#define PF_HFG_546_112_2_18_1_7_32_842 "[CCWA_TOKEN] value = %d"
#define PF_HFG_556_112_2_18_1_7_32_843 "[VTS_TOKEN] value = 0x%x"
#define PF_HFG_567_112_2_18_1_7_32_844 "[VTS_TOKEN] dtmf = 0x%x"
#define PF_HFG_586_112_2_18_1_7_32_845 "[HFG_CNUM_IND] cnum = %s"
#define PF_HFG_607_112_2_18_1_7_32_846 "[HFG_Activate] IN"
#define PF_HFG_608_112_2_18_1_7_32_847 "[HFG_Activate] status = %d"
#define PF_HFG_613_112_2_18_1_7_32_848 "[DRV_BT]<ASSERT_ERROR>: HFG_Activate WRONG status"
#define PF_HFG_641_112_2_18_1_7_32_849 "[BT][HFG_ServiceClose] IN"
#define PF_HFG_642_112_2_18_1_7_32_850 "[BT][HFG_ServiceClose] status = %d"
#define PF_HFG_643_112_2_18_1_7_32_851 "[BT][HFG_ServiceClose] audio_on = %d"
#define PF_HFG_644_112_2_18_1_7_32_852 "[BT][HFG_ServiceClose] conn_id = %d"
#define PF_HFG_665_112_2_18_1_7_32_853 "[HFG_Deactivate] IN"
#define PF_HFG_666_112_2_18_1_7_32_854 "[HFG_Deactivate] status = %d"
#define PF_HFG_667_112_2_18_1_7_32_855 "[HFG_Deactivate] audio_on = %d"
#define PF_HFG_668_112_2_18_1_7_32_856 "[HFG_Deactivate] conn_id = %d"
#define PF_HFG_726_112_2_18_1_7_32_857 "[HFG_ConnectHf] Time Out ,Acitive error"
#define PF_HFG_755_112_2_18_1_7_32_858 "[HFG_ConnectHf] IN"
#define PF_HFG_756_112_2_18_1_7_32_859 "[HFG_ConnectHf] status = %d"
#define PF_HFG_757_112_2_18_1_7_32_860 "[HFG_ConnectHf] conn_id = 0x%x"
#define PF_HFG_762_112_2_18_1_7_32_861 "[HFG_ConnectHf] one headset has been activated"
#define PF_HFG_788_112_2_18_1_7_32_862 "[HFG]timed_event_in fail"
#define PF_HFG_797_112_2_18_1_7_32_863 "[HFG_CancelConnect] IN"
#define PF_HFG_798_112_2_18_1_7_32_864 "[HFG_CancelConnect] status = %d"
#define PF_HFG_814_112_2_18_1_7_32_865 "[HFG_DisconnectHf] IN"
#define PF_HFG_815_112_2_18_1_7_32_866 "[HFG_DisconnectHf] status = %d"
#define PF_HFG_816_112_2_18_1_7_32_867 "[HFG_DisconnectHf] conn_id = 0x%x"
#define PF_HFG_825_112_2_18_1_7_32_868 "[HFG_DisconnectHf] No activated headset"
#define PF_HFG_837_112_2_18_1_7_32_869 "[HFG]timed_event_in fail"
#define PF_HFG_847_112_2_18_1_7_32_870 "[HFG_GetConnectedHf] IN"
#define PF_HFG_848_112_2_18_1_7_32_871 "[HFG_GetConnectedHf] conn_id = 0x%x"
#define PF_HFG_849_112_2_18_1_7_32_872 "[HFG_GetConnectedHf] status = %d"
#define PF_HFG_868_112_2_18_1_7_32_873 "[HFG_Dialing] IN"
#define PF_HFG_869_112_2_18_1_7_32_874 "[HFG_Dialing] conn_id = 0x%x"
#define PF_HFG_870_112_2_18_1_7_32_875 "[HFG_Dialing] active_call_num = %d"
#define PF_HFG_871_112_2_18_1_7_32_876 "[HFG_Dialing] audio_on = %d, Reqsend = %d"
#define PF_HFG_872_112_2_18_1_7_32_877 "[HFG_Dialing] status = %d"
#define PF_HFG_894_112_2_18_1_7_32_878 "[HFG_Alerting] IN"
#define PF_HFG_895_112_2_18_1_7_32_879 "[HFG_Alerting] conn_id = 0x%x"
#define PF_HFG_896_112_2_18_1_7_32_880 "[HFG_Alerting] active_call_num = %d"
#define PF_HFG_897_112_2_18_1_7_32_881 "[HFG_Alerting] audio_on = %d, Reqsend=%d"
#define PF_HFG_898_112_2_18_1_7_32_882 "[HFG_Alerting] status = %d"
#define PF_HFG_911_112_2_18_1_7_32_883 "[HFG_Incoming] IN (phone_num = %s)"
#define PF_HFG_913_112_2_18_1_7_32_884 "[HFG_Incoming] conn_id = 0x%x"
#define PF_HFG_914_112_2_18_1_7_32_885 "[HFG_Incoming] ccwa_notification = %d"
#define PF_HFG_915_112_2_18_1_7_32_886 "[HFG_Incoming] active_call_num = %d"
#define PF_HFG_916_112_2_18_1_7_32_887 "[HFG_Incoming] status = %d"
#define PF_HFG_929_112_2_18_1_7_32_888 "[HFG_Incoming] The second call"
#define PF_HFG_934_112_2_18_1_7_32_889 "[HFG_Incoming] The headset supports three-way call, send CCWA"
#define PF_HFG_958_112_2_18_1_7_32_890 "[HFG_Incoming] The first call, send RING & CLIP"
#define PF_HFG_989_112_2_18_1_7_33_891 "[HFG_IndicatorSetupNoCall]:call_status:%d,%d"
#define PF_HFG_1006_112_2_18_1_7_33_892 "[HFG_CallConnected] IN"
#define PF_HFG_1007_112_2_18_1_7_33_893 "[HFG_CallConnected] conn_id = 0x%x"
#define PF_HFG_1008_112_2_18_1_7_33_894 "[HFG_CallConnected] active_call_num = %d"
#define PF_HFG_1009_112_2_18_1_7_33_895 "[HFG_CallConnected] audio_on = %d"
#define PF_HFG_1010_112_2_18_1_7_33_896 "[HFG_CallConnected] status = %d"
#define PF_HFG_1036_112_2_18_1_7_33_897 "[HFG_CallDisconnected] IN (call_status = %d)"
#define PF_HFG_1037_112_2_18_1_7_33_898 "[HFG_CallDisconnected] conn_id = 0x%x"
#define PF_HFG_1038_112_2_18_1_7_33_899 "[HFG_CallDisconnected] active_call_num = %d"
#define PF_HFG_1039_112_2_18_1_7_33_900 "[HFG_CallDisconnected] audio_on = %d"
#define PF_HFG_1040_112_2_18_1_7_33_901 "[HFG_CallDisconnected] status = %d"
#define PF_HFG_1093_112_2_18_1_7_33_902 "[HFG_SetSpeakerGain] status = %d"
#define PF_HFG_1110_112_2_18_1_7_33_903 "[HFG_SetSpeakerGain] sys_vol = %d, hfg_vol = %d"
#define PF_HFG_1118_112_2_18_1_7_33_904 "[HFG_SetMicGain] status = %d"
#define PF_HFG_1134_112_2_18_1_7_33_905 "[HFG_SetMicGain] sys_vol = %d, hfg_vol = %d"
#define PF_HFG_1142_112_2_18_1_7_33_906 "[HFG_GetSpeakerGain] IN"
#define PF_HFG_1143_112_2_18_1_7_33_907 "[HFG_GetSpeakerGain] status = %d"
#define PF_HFG_1144_112_2_18_1_7_33_908 "[HFG_GetSpeakerGain] spk_gain = %d"
#define PF_HFG_1152_112_2_18_1_7_33_909 "[HFG_GetMicGain] IN"
#define PF_HFG_1153_112_2_18_1_7_33_910 "[HFG_GetMicGain] status = %d"
#define PF_HFG_1154_112_2_18_1_7_33_911 "[HFG_GetMicGain] mic_gain = %d"
#define PF_HFG_1163_112_2_18_1_7_33_912 "[HFG_SwitchAudio] IN (on = %d)"
#define PF_HFG_1164_112_2_18_1_7_33_913 "[HFG_SwitchAudio] audio_on = %d"
#define PF_HFG_1165_112_2_18_1_7_33_914 "[HFG_SwitchAudio] status = %d"
#define PF_HFG_1166_112_2_18_1_7_33_915 "[HFG_SwitchAudio] conn_id = %d"
#define PF_HFG_1184_112_2_18_1_7_33_916 "[HFG_SwitchAudio] IN hf_entry.audio_on switch_debug"
#define PF_HFG_1205_112_2_18_1_7_33_917 "[HFG_SwitchAudio] here "
#define PF_HFG_1227_112_2_18_1_7_33_918 "[HFG]g_hfgtimerid exist: g_hfgtimerid = %x"
#define PF_HFG_1235_112_2_18_1_7_33_919 "[HFG]timed_event_in fail"
#define PF_HFG_1244_112_2_18_1_7_33_920 "[HFG_SwitchAudioFormusic] IN (on = %d)"
#define PF_HFG_1254_112_2_18_1_7_33_921 "[HFG_AudioStart] audioOn = %d"
#define PF_HFG_1255_112_2_18_1_7_33_922 "[HFG_AudioStart]conn_id=0x%X\r\n"
#define PF_HFG_1256_112_2_18_1_7_33_923 "[HFG_AudioStart]BluePlay=0x%X\r\n"
#define PF_HFG_1257_112_2_18_1_7_33_924 "[HFG_AudioStart]hBlueRec=0x%X\r\n"
#define PF_HFG_1271_112_2_18_1_7_33_925 "[HFG_AudioStart] audio is off and audio start fails!\n"
#define PF_HFG_1282_112_2_18_1_7_33_926 "[DRV_BT]<WARNING> HFG_AudioStart: buffer is NOT freed properly!\n"
#define PF_HFG_1295_112_2_18_1_7_33_927 "[DRV_BT]<WARNING> HFG_AudioStart: Handle is NOT freed properly!\n"
#define PF_HFG_1302_112_2_18_1_7_33_928 "[HFG_AudioStart]AUDIO_Play:result=0x%X\r\n"
#define PF_HFG_1309_112_2_18_1_7_33_929 "[HFG_AudioStart]AUDIO_StartRecord:result=0x%X\r\n"
#define PF_HFG_1328_112_2_18_1_7_33_930 "[BT]<INFO> HFG_AudioStop() conn_id = 0x%X, active_call_num = %d, "
#define PF_HFG_1330_112_2_18_1_7_33_931 "[BT]<INFO> HFG_AudioStop() hBluePlay= 0x%X, hBlueRec= 0x%X"
#define PF_HFG_1347_112_2_18_1_7_33_932 "[HFG_AudioStop] audio is on and audio stop fails!\n"
#define PF_HFG_1358_112_2_18_1_7_33_933 "[HFG_AudioStop]AUDIO_Stop:hBluePlay result=0x%X\r\n"
#define PF_HFG_1365_112_2_18_1_7_33_934 "[HFG_AudioStop]AUDIO_StopRecord:hBlueRec result=0x%X\r\n"
#define PF_HFG_1371_112_2_18_1_7_33_935 "[HFG_AudioStop]AUDIO_CloseHandle:hBluePlay result=0x%X\r\n"
#define PF_HFG_1377_112_2_18_1_7_33_936 "[HFG_AudioStop]AUDIO_CloseHandle:hBlueRec result=0x%X\r\n"
#define PF_HFG_1385_112_2_18_1_7_33_937 "[HFG_AudioStop]AUDIO_Stop:ret=  %x\r\n"
#define PF_HFG_1401_112_2_18_1_7_33_938 "[HFG_SetStatusIndicator] IN (service_status = %d, active_calls = %d, call_setup_status = %d)"
#define PF_HFG_1402_112_2_18_1_7_33_939 "[HFG_SetStatusIndicator] conn_id = %d"
#define PF_HFG_1403_112_2_18_1_7_33_940 "[HFG_SetStatusIndicator] audio_on = %d"
#define PF_HFG_1405_112_2_18_1_7_33_941 "[HFG_SetStatusIndicator] active_call_num = %d"
#define PF_HFG_1406_112_2_18_1_7_33_942 "[HFG_SetStatusIndicator] call_setup = %d"
#define PF_HFG_1407_112_2_18_1_7_33_943 "[HFG_SetStatusIndicator] status = %d"
#define PF_HFG_1408_112_2_18_1_7_33_944 "[HFG_SetStatusIndicator] hold_call = %d"
#define PF_HFG_1443_112_2_18_1_7_33_945 "[HFG_TO_A2DP_TimeOutHandle] hf.addr = %04x:%02x:%06x"
#define PF_HFG_1464_112_2_18_1_7_33_946 "[HFG_HandlePrim] IN"
#define PF_HFG_1473_112_2_18_1_7_33_947 "[HFG_HandlePrim] prim_type = 0x%x"
#define PF_HFG_1478_112_2_18_1_7_33_948 "[HFG_DEACTIVATE_CFM]"
#define PF_HFG_1479_112_2_18_1_7_33_949 "[HFG_DEACTIVATE_CFM] status = %d"
#define PF_HFG_1499_112_2_18_1_7_34_950 "[HFG_SERVICE_CONNECT_IND] result = %d"
#define PF_HFG_1520_112_2_18_1_7_34_951 "[HFG_SERVICE_CONNECT_IND] addr = %04x:%02x:%06x"
#define PF_HFG_1521_112_2_18_1_7_34_952 "[HFG_SERVICE_CONNECT_IND] supportedFeatures = 0x%x"
#define PF_HFG_1522_112_2_18_1_7_34_953 "[HFG_SERVICE_CONNECT_IND] serviceName = %s"
#define PF_HFG_1523_112_2_18_1_7_34_954 "[HFG_SERVICE_CONNECT_IND] connectionId = %d"
#define PF_HFG_1524_112_2_18_1_7_34_955 "[HFG_SERVICE_CONNECT_IND] connectionType = %d"
#define PF_HFG_1552_112_2_18_1_7_34_956 "[HFG]timed_event_in fail"
#define PF_HFG_1584_112_2_18_1_7_34_957 "[HFG_SERVICE_CONNECT_IND] one headset has been activated (conn_id = %d)"
#define PF_HFG_1585_112_2_18_1_7_34_958 "[HFG_SERVICE_CONNECT_IND] prim->connectionId = %d"
#define PF_HFG_1602_112_2_18_1_7_34_959 "[HFG_DISCONNECT_IND] result = %d"
#define PF_HFG_1603_112_2_18_1_7_34_960 "[HFG_DISCONNECT_IND] connectionId = %d"
#define PF_HFG_1656_112_2_18_1_7_34_961 "[HFG_DISCONNECT_IND] Deactivating HFG..."
#define PF_HFG_1690_112_2_18_1_7_34_962 "[HFG_AUDIO_IND] cancel_timed_event g_hfgtimerid = %d"
#define PF_HFG_1698_112_2_18_1_7_34_963 "[HFG_AUDIO_IND] audioOn = %d"
#define PF_HFG_1699_112_2_18_1_7_34_964 "[HFG_AUDIO_IND] scoHandle = 0x%x"
#define PF_HFG_1700_112_2_18_1_7_34_965 "[HFG_AUDIO_IND] result = %d"
#define PF_HFG_1701_112_2_18_1_7_34_966 "[HFG_AUDIO_IND] linkType = %d"
#define PF_HFG_1702_112_2_18_1_7_34_967 "[HFG_AUDIO_IND] g_hfgtimerid = %d"
#define PF_HFG_1724_112_2_18_1_7_34_968 "[BT]<WARNING> %s L%d: close BT_audioOn if no valid call"
#define PF_HFG_1745_112_2_18_1_7_34_969 "[HFG_AUDIO_IND] hfg_MusicOrVoice = 0x%x"
#define PF_HFG_1751_112_2_18_1_7_34_970 "[HFG_AUDIO_IND] hfg_MusicOrVoice = 0x%x"
#define PF_HFG_1778_112_2_18_1_7_34_971 "[BT]<WARNING> %s L%d: close BT_audioOn if no valid call"
#define PF_HFG_1816_112_2_18_1_7_34_972 "[HFG_REJECT_IND]"
#define PF_HFG_1825_112_2_18_1_7_34_973 "[HFG_ANSWER_IND]"
#define PF_HFG_1833_112_2_18_1_7_34_974 "[HFG_RING_CFM]"
#define PF_HFG_1842_112_2_18_1_7_34_975 "[HFG_STATUS_IND] statusEvent = %d"
#define PF_HFG_1852_112_2_18_1_7_34_976 "[HFG_SPEAKER_GAIN_IND] conn_id = %d, gain = %d"
#define PF_HFG_1877_112_2_18_1_7_34_977 "[HFG_MIC_GAIN_IND] conn_id = %d, gain = %d"
#define PF_HFG_1900_112_2_18_1_7_34_978 "[HFG_AT_CMD_IND] AT = %s"
#define PF_HFG_1916_112_2_18_1_7_34_979 "[HFG_CHLD_IND] value = %d"
#define PF_HFG_1917_112_2_18_1_7_34_980 "[HFG_CHLD_IND] index = %d"
#define PF_HFG_2000_112_2_18_1_7_34_981 "[HFG_CLCC_IND]"
#define PF_HFG_2023_112_2_18_1_7_35_982 "[HFG_COPS_IND] CopsStr = %s"
#define PF_HFG_2032_112_2_18_1_7_35_983 "[HFG_COPS_IND] CopsStr = %s"
#define PF_HFG_2061_112_2_18_1_7_35_984 "[HFG_CNUM_IND]"
#define PF_HFG_2072_112_2_18_1_7_35_985 "[HFG_CMEE_IND] command = %c"
#define PF_HFG_2083_112_2_18_1_7_35_986 "[HFG_BTRH_IND] command = 0x%x"
#define PF_HFG_2090_112_2_18_1_7_35_987 "[HFG_HandlePrim] un-handled command 0x%x"
#define PF_HFG_2115_112_2_18_1_7_35_988 "[HFG_DeinitPrim] prim_type = 0x%x"
#define PF_HFG_2136_112_2_18_1_7_35_989 "[HFG_GetCallStatus] active_call_num = %d"
#define PF_HFG_2138_112_2_18_1_7_35_990 "[HFG_GetCallStatus] call_setup = %d"
#define PF_HFG_2149_112_2_18_1_7_35_991 "[HFG_GetScoHandle] scoHandle = 0x%x"
#define PF_HFG_2181_112_2_18_1_7_35_992 "HFG_IndicatorCIEV Status:param=0x%x,%x"
#define PF_HFG_2206_112_2_18_1_7_35_993 "HFG_IndicatorCIEV Status End"
#define PF_HFG_2218_112_2_18_1_7_35_994 "idx=%d,dir=%d,status=%d,mode=%d,mpty=%d,number=%s"
#define PF_HFG_2238_112_2_18_1_7_35_995 "[HFG_SetNameOfNetOperator] net_operator_name = %s"
#define PF_HFG_2257_112_2_18_1_7_35_996 "[HFG_SetLocalNumber] local_number = %s"
#define PF_OPP_CLIENT_56_112_2_18_1_7_35_997 "[DRV_BT]OPC_CloseRES(in): fileHandle = 0x%X, filename_ptr = 0x%X"
#define PF_OPP_CLIENT_70_112_2_18_1_7_35_998 "[DRV_BT]OPC_CloseRES(out): fileHandle = 0x%X, filename_ptr = 0x%X"
#define PF_OPP_CLIENT_75_112_2_18_1_7_35_999 "[BT]OPC_ChangeClk:Change System Clock\r\n"
#define PF_OPP_CLIENT_83_112_2_18_1_7_35_1000 "[BT]OPC_RestoreClk:Restore System Clock=0x%X\r\n"
#define PF_OPP_CLIENT_103_112_2_18_1_7_35_1001 "[BT]OPC_ServiceClose:state=0x%X\r\n"
#define PF_OPP_CLIENT_118_112_2_18_1_7_35_1002 "[BT]OPC_IsBusy:state=0x%X\r\n"
#define PF_OPP_CLIENT_124_112_2_18_1_7_35_1003 "[BT]OPC_Init\r\n"
#define PF_OPP_CLIENT_130_112_2_18_1_7_35_1004 "[BT]OPC_DeInit\r\n"
#define PF_OPP_CLIENT_154_112_2_18_1_7_36_1005 "[BT]OPC_SendFile: Is Busy.....state:=0x%X,fileHandle=0x%xrn"
#define PF_OPP_CLIENT_160_112_2_18_1_7_36_1006 "[BT]OPC_SendFile\r\n"
#define PF_OPP_CLIENT_168_112_2_18_1_7_36_1007 "[BT]OPC_SendFile:PF_fopen Error\r\n"
#define PF_OPP_CLIENT_177_112_2_18_1_7_36_1008 "[DRV_BT]OPC_SendFile(): malloc filename_ptr = 0x%X"
#define PF_OPP_CLIENT_227_112_2_18_1_7_36_1009 "[BT]<INFO> %s L%d: BT_StartCancelTimer(): OPC Cancel"
#define PF_OPP_CLIENT_245_112_2_18_1_7_36_1010 "[BT]OPC_Cancel:state=0x%X\r\n"
#define PF_OPP_CLIENT_265_112_2_18_1_7_36_1011 "[DRV_BT]<ASSERT_ERROR> OPC_Cancel:Invail Cancel"
#define PF_OPP_CLIENT_321_112_2_18_1_7_36_1012 "[BT]OPC_HandlePrim type = 0x%X\r\n"
#define PF_OPP_CLIENT_341_112_2_18_1_7_36_1013 "[BT]OPC_HandlePrim:OPC_CONNECT_CFM-responseCode=0x%x\r\n"
#define PF_OPP_CLIENT_385_112_2_18_1_7_36_1014 "[BT]OPC_HandlePrim:OPP Disconnect:state = 0x%X\r\n"
#define PF_OPP_CLIENT_394_112_2_18_1_7_36_1015 "[BT]OPC_HandlePrim:Server Send Abort request\r\n"
#define PF_OPP_CLIENT_428_112_2_18_1_7_36_1016 "[BT]OPC_PUT_OBJECT_IND: state= 0x%X, fileHandle= 0x%X"
#define PF_OPP_CLIENT_431_112_2_18_1_7_36_1017 "[BT]OPC_PUT_OBJECT_IND:Cancel Send file\r\n"
#define PF_OPP_CLIENT_438_112_2_18_1_7_36_1018 "[BT]<ERROR>%s L%d: state or fileHandle error"
#define PF_OPP_CLIENT_456_112_2_18_1_7_36_1019 "[BT]<ERROR>%s L%d: read_len(%d) != "
#define PF_OPP_CLIENT_514_112_2_18_1_7_36_1020 "[BT]OPC_PUT_CFM:Cancel Send file\r\n"
#define PF_OPP_CLIENT_525_112_2_18_1_7_36_1021 "[BT]OPC_PUT_CFM:responseCode=0x%x\r\n"
#define PF_OPP_CLIENT_552_112_2_18_1_7_36_1022 "[BT]OPC_HandlePrim:OPP abort \r\n"
#define PF_OPP_CLIENT_570_112_2_18_1_7_36_1023 "[BT]<WARNING> OPC_HandlePrim: NOT supported type\r\n"
#define PF_OPP_SERVER_62_112_2_18_1_7_37_1024 "[DRV_BT]OPS_CloseRES(in): is_delete = %d, fileHandle = 0x%X, filename_ptr = 0x%X"
#define PF_OPP_SERVER_80_112_2_18_1_7_37_1025 "[DRV_BT]OPS_CloseRES(out): is_delete = %d, fileHandle = 0x%X, filename_ptr = 0x%X"
#define PF_OPP_SERVER_85_112_2_18_1_7_37_1026 "[BT]OPS_ChangeClk:Change System Clock:cnt=0x%X\r\n"
#define PF_OPP_SERVER_96_112_2_18_1_7_37_1027 "[BT]OPS_RestoreClk:Restore System Clock=0x%X\r\n"
#define PF_OPP_SERVER_110_112_2_18_1_7_37_1028 "[BT]OPS_Active\r\n"
#define PF_OPP_SERVER_119_112_2_18_1_7_37_1029 "[BT]OPS_DeActive\r\n"
#define PF_OPP_SERVER_129_112_2_18_1_7_37_1030 "[BT][OPS_ServiceClose] IN"
#define PF_OPP_SERVER_130_112_2_18_1_7_37_1031 "[BT][OPS_ServiceClose] status = %d"
#define PF_OPP_SERVER_148_112_2_18_1_7_37_1032 "[BT]OPS_IsBusy:state=0x%X\r\n"
#define PF_OPP_SERVER_154_112_2_18_1_7_37_1033 "[BT]OPS_Init\r\n"
#define PF_OPP_SERVER_160_112_2_18_1_7_37_1034 "[BT]OPS_DeInit\r\n"
#define PF_OPP_SERVER_180_112_2_18_1_7_37_1035 "[BT]OPS_Connect is_enable = %d"
#define PF_OPP_SERVER_199_112_2_18_1_7_37_1036 "[BT]OPS_CancelReceive:state = 0x%X\r\n"
#define PF_OPP_SERVER_224_112_2_18_1_7_37_1037 "[DRV_BT]<ASSERT_ERROR> OPS_CancelReceive:Invaild Cancel Receive\r\n"
#define PF_OPP_SERVER_233_112_2_18_1_7_37_1038 "[BT]OPS_ReceiveFileFail\r\n"
#define PF_OPP_SERVER_248_112_2_18_1_7_37_1039 "[BT]OPS_ReceiveFileFinalPacket\r\n"
#define PF_OPP_SERVER_278_112_2_18_1_7_37_1040 "[BT]OPS_ReceiveFile\r\n"
#define PF_OPP_SERVER_282_112_2_18_1_7_37_1041 "[BT] OPS_ReceiveFile error state=%d"
#define PF_OPP_SERVER_296_112_2_18_1_7_37_1042 "[BT]OPS_ReceiveFile:w_name_len =%d\r\n"
#define PF_OPP_SERVER_302_112_2_18_1_7_37_1043 "[BT]OPS_ReceiveFile:File Handle = 0x%X\r\n"
#define PF_OPP_SERVER_310_112_2_18_1_7_37_1044 "[DRV_BT]OPS_ReceiveFile(): malloc file_name_ptr = 0x%X"
#define PF_OPP_SERVER_318_112_2_18_1_7_37_1045 "[BT]OPS_ReceiveFile:File write error, source len=0x%X, write len=0x%X\r\n"
#define PF_OPP_SERVER_343_112_2_18_1_7_37_1046 "[BT]OPS_ReceiveFile: finaFlag=0x%X\r\n"
#define PF_OPP_SERVER_395_112_2_18_1_7_37_1047 "[BT]OPS_HandlePrim:type = 0x%X\r\n"
#define PF_OPP_SERVER_404_112_2_18_1_7_37_1048 "[BT]OPS_HandlePrim:opp client connected\r\n"
#define PF_OPP_SERVER_408_112_2_18_1_7_37_1049 "[BT]OPS_CONNECT_IND:Ftp is busy now, could not respond connect request\r\n"
#define PF_OPP_SERVER_447_112_2_18_1_7_37_1050 "[BT]OPS_CONNECT_IND:Received Too Many Connect Commands\r\n"
#define PF_OPP_SERVER_458_112_2_18_1_7_37_1051 "[BT]OPS_HandlePrim:OPS_PUT_IND\r\n"
#define PF_OPP_SERVER_462_112_2_18_1_7_37_1052 "[BT]OPS_CONNECT_IND:Ftp is busy now, could not respond put object\r\n"
#define PF_OPP_SERVER_475_112_2_18_1_7_37_1053 "[BT]OPS_HandlePrim:OPS_PUT_IND,One file is transmitting.......\r\n"
#define PF_OPP_SERVER_503_112_2_18_1_7_38_1054 "[BT]:bad w_name_len %d"
#define PF_OPP_SERVER_519_112_2_18_1_7_38_1055 "[BT]OPS_PUT_IND:name[%d]=0x%X\r\n"
#define PF_OPP_SERVER_571_112_2_18_1_7_38_1056 "[BT]OPS_HandlePrim:OPS_PUT_NEXT_IND: "
#define PF_OPP_SERVER_583_112_2_18_1_7_38_1057 "[BT]<ERROR>%s L%d: state or file_handler error"
#define PF_OPP_SERVER_661_112_2_18_1_7_38_1058 "[BT]OPS_ABORT_IND\r\n"
#define PF_OPP_SERVER_687_112_2_18_1_7_38_1059 "[BT]OPS_DISCONNECT_IND:state=0x%X\r\n"
#define PF_OPP_SERVER_712_112_2_18_1_7_38_1060 "[BT]OPS_DEACTIVATE_CFM\r\n"
#define PF_OPP_SERVER_730_112_2_18_1_7_38_1061 "[BT]PF_DeInitOpsEvent\r\n"
#define PF_OPP_SERVER_769_112_2_18_1_7_38_1062 "[DRV_BT]<ASSERT_ERROR>: OPS_DeInitHandlePrim: unkown type"
#define PF_SD_18_112_2_18_1_7_38_1063 "[BT]SD_ReadServiceTimeOutHandle:Time out\r\n"
#define PF_SD_29_112_2_18_1_7_38_1064 "[BT]SD_ReadAvailableServerReq\r\n"
#define PF_SD_67_112_2_18_1_7_38_1065 "[BT]SD_HandlePrim:type = 0x%X\r\n"
#define PF_SD_76_112_2_18_1_7_38_1066 "[BT]SD_READ_DEVICE_PROPERTIES_CFM:result = 0x%X\r\n"
#define PF_SD_101_112_2_18_1_7_38_1067 "[BT]SD_READ_AVAILABLE_SERVICES_CFM\r\n"
#define PF_SD_102_112_2_18_1_7_38_1068 "result                = %d\n"
#define PF_SD_103_112_2_18_1_7_38_1069 "remoteNameChanged     = %d\n"
#define PF_SD_107_112_2_18_1_7_38_1070 "deviceAddr            = %04x:%02x:%06x"
#define PF_SD_109_112_2_18_1_7_38_1071 "friendlyName          = %s"
#define PF_SD_111_112_2_18_1_7_38_1072 "classOfDevice         = 0x%x"
#define PF_SD_113_112_2_18_1_7_38_1073 "knownServices11_00_31 = 0x%08xn"
#define PF_SD_114_112_2_18_1_7_38_1074 "s_is_refresh_server   = 0x%x\n"
#define PF_SD_189_112_2_18_1_7_39_1075 "[DRV_BT]<ASSERT_ERROR>: SD_HandlePrim: unkown type"
#define DUN_GW_API_78_112_2_18_1_7_39_1076 "DG_SwitchUserPort phy_port = %d\n"
#define DUN_GW_API_258_112_2_18_1_7_39_1077 "[BT]DG_AcceptDunReq!\n"
#define DUN_GW_API_283_112_2_18_1_7_39_1078 "[BT]DG_RejectDunReq!\n"
#define DUN_GW_API_358_112_2_18_1_7_40_1079 "DG_PortBusy: port_busy = %d\n"
#define PF_DUN_GW_105_112_2_18_1_7_40_1080 "[BT]DG_Activate app_handle = %d\n"
#define PF_DUN_GW_129_112_2_18_1_7_40_1081 "[BT]DgDeactivateReqSend\n"
#define PF_DUN_GW_166_112_2_18_1_7_40_1082 "[BT]DG_Disconnect\n"
#define PF_DUN_GW_236_112_2_18_1_7_40_1083 "[BT]DG_dataResSend\n"
#define PF_DUN_GW_259_112_2_18_1_7_40_1084 "[BT]DG_dataReqSend thePayload = 0x%08x, PayloadLength = %d\n"
#define PF_DUN_GW_304_112_2_18_1_7_40_1085 "[BT]DG_controlReqSend\n"
#define PF_DUN_GW_330_112_2_18_1_7_40_1086 "result= 0x%02x\n"
#define PF_DUN_GW_332_112_2_18_1_7_41_1087 "muxId= 0x%04x\n"
#define PF_DUN_GW_333_112_2_18_1_7_41_1088 "serverChannel= 0x%02x\n"
#define PF_DUN_GW_334_112_2_18_1_7_41_1089 "profileMaxFrameSize= 0x%02x\n"
#define PF_DUN_GW_356_112_2_18_1_7_41_1090 "result= 0x%02x\n"
#define PF_DUN_GW_358_112_2_18_1_7_41_1091 "muxId= 0x%04x\n"
#define PF_DUN_GW_359_112_2_18_1_7_41_1092 "serverChannel= 0x%02x\n"
#define PF_DUN_GW_395_112_2_18_1_7_41_1093 "muxId= 0x%04x\n"
#define PF_DUN_GW_396_112_2_18_1_7_41_1094 "serverChannel= 0x%02x\n"
#define PF_DUN_GW_397_112_2_18_1_7_41_1095 "payloadLength= 0x%04x\n"
#define PF_DUN_GW_398_112_2_18_1_7_41_1096 "payload= 0x%08x\n"
#define PF_DUN_GW_431_112_2_18_1_7_41_1097 "[BT] DG_handleDataInd the DG COM has not opened when ecieve the DATA_IND.\n "
#define PF_DUN_GW_460_112_2_18_1_7_41_1098 "muxId= 0x%04x\n"
#define PF_DUN_GW_461_112_2_18_1_7_41_1099 "serverChannel= 0x%02x\n"
#define PF_DUN_GW_489_112_2_18_1_7_41_1100 "muxId= 0x%04x\n"
#define PF_DUN_GW_490_112_2_18_1_7_41_1101 "serverChannel= 0x%02x\n"
#define PF_DUN_GW_491_112_2_18_1_7_41_1102 "modemstatus= 0x%02x\n"
#define PF_DUN_GW_492_112_2_18_1_7_41_1103 "break_signal= 0x%02x\n"
#define PF_DUN_GW_493_112_2_18_1_7_41_1104 "tx_en= 0x%02x peer_dev_rx_ready= 0x%02x\n"
#define PF_DUN_GW_517_112_2_18_1_7_41_1105 "[BT]DG_handleControlInd. Peer device RTR and RTC is not ready!\n"
#define PF_DUN_GW_538_112_2_18_1_7_41_1106 "serverChannel= 0x%02x\n"
#define PF_DUN_GW_539_112_2_18_1_7_41_1107 "muxId= 0x%04x\n"
#define PF_DUN_GW_540_112_2_18_1_7_41_1108 "request= 0x%04x\n"
#define PF_DUN_GW_546_112_2_18_1_7_41_1109 "port_speed= 0x%04x, data_b= 0x%04x, stop_b= 0x%04x, "
#define PF_DUN_GW_587_112_2_18_1_7_41_1110 "muxId= 0x%04x\n"
#define PF_DUN_GW_588_112_2_18_1_7_41_1111 "serverChannel= 0x%02x\n"
#define PF_DUN_GW_589_112_2_18_1_7_41_1112 "connect= 0x%02x\n"
#define PF_DUN_GW_590_112_2_18_1_7_41_1113 "maxMsgSize= 0x%04x\n"
#define PF_DUN_GW_634_112_2_18_1_7_41_1114 "result= 0x%02x\n"
#define PF_DUN_GW_695_112_2_18_1_7_41_1115 "[BT]DG_handleDeinitPrim msg = 0x%08x\n"
#define PF_DUN_GW_730_112_2_18_1_7_41_1116 "[BT]DG_handlePrim type = 0x%X\n"
#define PF_DUN_GW_735_112_2_18_1_7_41_1117 "[BT]DG_CONNECT_IND\n"
#define PF_DUN_GW_740_112_2_18_1_7_41_1118 "[BT]DG_DATA_IND\n"
#define PF_DUN_GW_745_112_2_18_1_7_41_1119 "[BT]DG_DATA_CFM\n"
#define PF_DUN_GW_750_112_2_18_1_7_41_1120 "[BT]DG_CONTROL_IND\n"
#define PF_DUN_GW_755_112_2_18_1_7_41_1121 "[BT]DG_PORTNEG_IND\n"
#define PF_DUN_GW_760_112_2_18_1_7_41_1122 "[BT]DG_STATUS_IND\n"
#define PF_DUN_GW_765_112_2_18_1_7_41_1123 "[BT]DG_DISCONNECT_IND\n"
#define PF_DUN_GW_770_112_2_18_1_7_41_1124 "[BT]DG_DATA_PATH_STATUS_IND\n"
#define PF_DUN_GW_775_112_2_18_1_7_41_1125 "[BT]DG_DEACTIVATE_CFM\n"
#define PF_DUN_GW_780_112_2_18_1_7_41_1126 "[BT]DG_REGISTER_DATA_PATH_HANDLE_CFM\n"
#define PF_DUN_GW_838_112_2_18_1_7_41_1127 "[BT]DGCOM_Initialize\n"
#define PF_DUN_GW_879_112_2_18_1_7_42_1128 "[BT]DGCOM_Close\n"
#define PF_DUN_GW_908_112_2_18_1_7_42_1129 "[BT]DGCOM_GetTxFifoCnt, maxMsgSize = %d\n"
#define PF_DUN_GW_937_112_2_18_1_7_42_1130 "[BT]DGCOM_ReadData: port = %d, buffer_len = %d, payload_len=%dn"
#define PF_DUN_GW_948_112_2_18_1_7_42_1131 "[DRV_BT]<ASSERT_ERROR>DGCOM_ReadData: port NOT open !"
#define PF_DUN_GW_994_112_2_18_1_7_42_1132 "[BT]DGCOM_WriteData: port = %d, len = %d\n"
#define PF_DUN_GW_1005_112_2_18_1_7_42_1133 "[DRV_BT]<ASSERT_ERROR>DGCOM_ReadData: port NOT open !"
#define PF_DUN_GW_1049_112_2_18_1_7_42_1134 "[BT]DGCOM_DumpGetChar port_num = %d\n"
#define PF_DUN_GW_1068_112_2_18_1_7_42_1135 "[BT]DGCOM_DumpPutChar port_num = %d nchar = %c\n"
#define PF_DUN_GW_1088_112_2_18_1_7_42_1136 "[BT]DGCOM_Tx_Int_Enable is_enable = %d  open_flag = %dn"
#define PF_DUN_GW_1093_112_2_18_1_7_42_1137 "[BT]DGCOM_Tx_Int_Enable,The DGCOM has been enabled!\n "
#define PF_DUN_GW_1107_112_2_18_1_7_42_1138 "[BT]DGCOM_Tx_Int_Enable, Enable Tx but DGCOM or peerside not open.\n "
#define PF_DUN_GW_1134_112_2_18_1_7_42_1139 "[BT]DGCOM_ReadData port_num = %d,is_enable = %d\n"
#define DB_140_112_2_18_1_7_42_1140 "[_DB_SearchDevice] i = %d"
#define DB_146_112_2_18_1_7_42_1141 "[_DB_SearchDevice] NOT exist"
#define DB_209_112_2_18_1_7_43_1142 "[_DB_CheckDeviceCategory] IN (knownServices11_00_31 = 0x%x, classOfDevice = 0x%x)"
#define DB_253_112_2_18_1_7_43_1143 "device record:"
#define DB_255_112_2_18_1_7_43_1144 "deviceAddr = %04x:%02x:%06x"
#define DB_256_112_2_18_1_7_43_1145 "linkkeyValid = %d"
#define DB_257_112_2_18_1_7_43_1146 "linkkeyLen = %d"
#define DB_258_112_2_18_1_7_43_1147 "Linkkey = %s"
#define DB_259_112_2_18_1_7_43_1148 "remoteName = %s"
#define DB_260_112_2_18_1_7_43_1149 "classOfDevice = 0x%x"
#define DB_261_112_2_18_1_7_43_1150 "knownServices11_00_31 = 0x%x"
#define DB_262_112_2_18_1_7_43_1151 "knownServices11_32_63 = 0x%x"
#define DB_263_112_2_18_1_7_43_1152 "knownServices12_00_31 = 0x%x"
#define DB_264_112_2_18_1_7_43_1153 "knownServices13_00_31 = 0x%x"
#define DB_265_112_2_18_1_7_43_1154 "authorised = %d"
#define DB_270_112_2_18_1_7_43_1155 "addr = %04x:%02x:%06x"
#define DB_305_112_2_18_1_7_43_1156 "[DB_SetLocalDeviceStatus] IN (status = %d)"
#define DB_311_112_2_18_1_7_43_1157 "[DB_GetLocalDeviceStatus] IN"
#define DB_312_112_2_18_1_7_43_1158 "[DB_GetLocalDeviceStatus] status = %d"
#define DB_319_112_2_18_1_7_43_1159 "[DB_GetLocalName] IN"
#define DB_330_112_2_18_1_7_43_1160 "[DB_SetLocalName] IN"
#define DB_334_112_2_18_1_7_43_1161 "[DB_SetLocalName] len = %d"
#define DB_355_112_2_18_1_7_43_1162 "[DB_SetVisibility] IN (visibility = %d)"
#define DB_362_112_2_18_1_7_43_1163 "[DB_GetVisibility] IN"
#define DB_363_112_2_18_1_7_43_1164 "[DB_GetVisibility] visibility = %d"
#define DB_376_112_2_18_1_7_43_1165 "[DB_ReadRecord] IN"
#define DB_380_112_2_18_1_7_43_1166 "[DB_ReadRecord] The device is not found"
#define DB_384_112_2_18_1_7_43_1167 "[DB_ReadRecord] index = %d"
#define DB_407_112_2_18_1_7_43_1168 "[DB_WriteRecord] IN"
#define DB_417_112_2_18_1_7_43_1169 "do not write 0 address to record"
#define DB_423_112_2_18_1_7_43_1170 "[DB_WriteRecord] It is a new device, get a free entry for it"
#define DB_434_112_2_18_1_7_43_1171 "[DB_WriteRecord] dev_count = %d"
#define DB_435_112_2_18_1_7_43_1172 "[DB_WriteRecord] index = %d"
#define DB_452_112_2_18_1_7_43_1173 "[DB_WriteRecord] index = %d, strlen=%d, refresh_service=%dn"
#define DB_456_112_2_18_1_7_43_1174 "[DB_WriteRecord] Remote name has existed, and not change.\n"
#define DB_470_112_2_18_1_7_43_1175 "[DB_WriteRecord] Remote name reflesh.\n"
#define DB_491_112_2_18_1_7_43_1176 "[DB_WriteRecord] get the 0 class device"
#define DB_496_112_2_18_1_7_43_1177 "[DB_WriteRecord] reset Device class,get the wrong class device"
#define DB_515_112_2_18_1_7_43_1178 "[DB_DeleteRecord] IN"
#define DB_517_112_2_18_1_7_43_1179 "[DB_DeleteRecord] dev_count = %d"
#define DB_520_112_2_18_1_7_43_1180 "[DB_DeleteRecord] The device is not found"
#define DB_542_112_2_18_1_7_43_1181 "[DB_GetRecordCount] IN"
#define DB_543_112_2_18_1_7_43_1182 "[DB_GetRecordCount] dev_count = %d"
#define DB_553_112_2_18_1_7_43_1183 "[DB_ReadRecordWithIndex] IN (index = %d)"
#define DB_557_112_2_18_1_7_43_1184 "[DRV_BT]<ASSERT_ERROR> DB_ReadRecordWithIndex invalid index"
#define DB_573_112_2_18_1_7_43_1185 "[DRV_BT]<ASSERT_ERROR> DB_ReadRecordWithIndex invalid index"
#define DB_580_112_2_18_1_7_43_1186 "[DB_ReadFirstRecord] IN"
#define DB_595_112_2_18_1_7_43_1187 "[DB_ReadNextRecord] IN"
#define DB_614_112_2_18_1_7_43_1188 "[DB_SetDeviceName] IN"
#define DB_618_112_2_18_1_7_43_1189 "[DB_SetDeviceName] The device is not found"
#define DB_642_112_2_18_1_7_43_1190 "[DB_GetDeviceName] IN"
#define DB_646_112_2_18_1_7_43_1191 "[DB_GetDeviceName] The device is not found"
#define DB_667_112_2_18_1_7_43_1192 "[DB_GetDeivceCoD] IN"
#define DB_671_112_2_18_1_7_44_1193 "[DB_GetDeviceCoD] Device is not found"
#define DB_676_112_2_18_1_7_44_1194 "[DB_GetDeviceCoD] classOfDevice = 0x%x"
#define DB_686_112_2_18_1_7_44_1195 "[DB_SetDeviceCoD] IN"
#define DB_688_112_2_18_1_7_44_1196 "cod = 0x%x"
#define DB_691_112_2_18_1_7_44_1197 "[DB_SetDeviceCod] Device not found"
#define DB_708_112_2_18_1_7_44_1198 "[DB_SetDeviceConnStatus] IN"
#define DB_711_112_2_18_1_7_44_1199 "bimap_ind = %d, service_mask = 0x%x, status = %d"
#define DB_714_112_2_18_1_7_44_1200 "[DB_SetDeviceConnStatus] Device is not found"
#define DB_742_112_2_18_1_7_44_1201 "[DB_GetDeviceConnStatus] IN"
#define DB_744_112_2_18_1_7_44_1202 "bitmap_ind = %d, service_mask = 0x%x"
#define DB_747_112_2_18_1_7_44_1203 "[DB_GetDeviceConnStatus] Device is not found"
#define DB_767_112_2_18_1_7_44_1204 "[DB_GetDeviceService] IN"
#define DB_771_112_2_18_1_7_44_1205 "[DB_GetDeviceService] Device is not found"
#define DB_785_112_2_18_1_7_44_1206 "[DB_GetDeviceService] IN"
#define DB_789_112_2_18_1_7_44_1207 "[DB_GetDeviceService] Device is not found"
#define DB_802_112_2_18_1_7_44_1208 "[DB_CheckDeviceService] IN"
#define DB_806_112_2_18_1_7_44_1209 "[DB_CheckDeviceService] Device not found"
#define DB_819_112_2_18_1_7_44_1210 "[DB_GetRecordCountWithDeviceCategory] IN (category = %d)"
#define DB_833_112_2_18_1_7_44_1211 "[DB_GetRecordCountWithDeviceCategory] count = %d"
#define DB_844_112_2_18_1_7_44_1212 "[DB_ReadRecordWithDeviceCategory] IN (category = %d, index = %d)"
#define DB_869_112_2_18_1_7_44_1213 "[DRV_BT]<ASSERT_ERROR> DB_GetDeviceAuthorisation: BAD parameters "
#define DB_876_112_2_18_1_7_44_1214 "[DRV_BT]<ASSERT_ERROR> DB_GetDeviceAuthorisation: The device is not found"
#define DB_883_112_2_18_1_7_44_1215 "[DRV_BT]DB_SetDeviceAuthorisation is_authorised = %d"
#define DB_894_112_2_18_1_7_44_1216 "[DRV_BT]<ASSERT_ERROR> DB_GetDeviceAuthorisation: BAD parameters "
#define DB_901_112_2_18_1_7_44_1217 "[DRV_BT]<ASSERT_ERROR> DB_GetDeviceAuthorisation: The device is not found"
#define DB_905_112_2_18_1_7_44_1218 "[DRV_BT]DB_SetDeviceAuthorisation is_authorised = %d"
#define DB_916_112_2_18_1_7_44_1219 "[DB_Init] IN"
#define DB_922_112_2_18_1_7_44_1220 "[DB_Init] Read NV failed\r\n"
#define DB_947_112_2_18_1_7_44_1221 "[_DB_SaveToNV] IN\r\n"
#define DB_950_112_2_18_1_7_44_1222 "[_DB_SaveToNV] Write NV failed\r\n"
#define DB_961_112_2_18_1_7_44_1223 "[DB_SaveAddrToNV] IN\r\n"
#define DB_967_112_2_18_1_7_44_1224 "[DB_SaveAddrToNV] Write NV failed\r\n"
#define DB_984_112_2_18_1_7_44_1225 "[DB_Init] IN"
#define DB_998_112_2_18_1_7_44_1226 "[DB_Init] PF_mkdir failed"
#define DB_1009_112_2_18_1_7_44_1227 "[DB_Init] Open database file failed\r\n"
#define DB_1018_112_2_18_1_7_44_1228 "[DB_Init] Read signature failed"
#define DB_1021_112_2_18_1_7_44_1229 "[DB_Init] signature = 0x%x"
#define DB_1024_112_2_18_1_7_44_1230 "[DB_Init] Unknown signature"
#define DB_1030_112_2_18_1_7_44_1231 "[DB_Init] Read status failed"
#define DB_1034_112_2_18_1_7_44_1232 "[DB_Init] status = %d"
#define DB_1038_112_2_18_1_7_44_1233 "[DB_Init] Read visibility failed"
#define DB_1042_112_2_18_1_7_44_1234 "[DB_Init] visibility = %d"
#define DB_1046_112_2_18_1_7_44_1235 "[DB_Init] Read size of local name failed"
#define DB_1049_112_2_18_1_7_44_1236 "[DB_Init] size of local name = %d"
#define DB_1052_112_2_18_1_7_44_1237 "[DB_Init] Invalid count number"
#define DB_1058_112_2_18_1_7_44_1238 "[DB_Init] Read local name failed"
#define DB_1067_112_2_18_1_7_44_1239 "[DB_Init] Read count of device records failed"
#define DB_1070_112_2_18_1_7_44_1240 "[DB_Init] count of device records = %d"
#define DB_1073_112_2_18_1_7_44_1241 "[DB_Init] Invalid count number"
#define DB_1081_112_2_18_1_7_44_1242 "[DB_Init] Read %d th record failed"
#define DB_1089_112_2_18_1_7_44_1243 "[DB_Init] total %d records read"
#define DB_1111_112_2_18_1_7_44_1244 "[DB_Deinit] IN"
#define DB_1114_112_2_18_1_7_44_1245 "[DB_Deinit] create dir failed"
#define DB_1124_112_2_18_1_7_44_1246 "[DB_Deinit] Create database file failed"
#define DB_1133_112_2_18_1_7_44_1247 "[DB_Deinit] Write signature failed"
#define DB_1138_112_2_18_1_7_44_1248 "[DB_Deinit] status = %d"
#define DB_1140_112_2_18_1_7_44_1249 "[DB_Deinit] Write status failed"
#define DB_1145_112_2_18_1_7_44_1250 "[DB_Deinit] visibility = %d"
#define DB_1147_112_2_18_1_7_44_1251 "[DB_Deinit] Write visibility failed"
#define DB_1153_112_2_18_1_7_44_1252 "[DB_Deinit] size of local name = %d"
#define DB_1158_112_2_18_1_7_44_1253 "[DB_Deinit] Write size of local name failed"
#define DB_1164_112_2_18_1_7_44_1254 "[DB_Deinit] Write local name failed"
#define DB_1181_112_2_18_1_7_44_1255 "[DB_Deinit] count of device records = %d"
#define DB_1183_112_2_18_1_7_44_1256 "[DB_Deinit] Write count of device records failed"
#define DB_1192_112_2_18_1_7_44_1257 "[DB_Deinit] Write %d th device record failed"
#define DB_1200_112_2_18_1_7_44_1258 "[DB_Deinit] total %d records written"
#define PF_SPP_163_112_2_18_1_7_45_1259 "[BT]SPP_Activate app_handle = %d\n"
#define PF_SPP_172_112_2_18_1_7_45_1260 "[BT]SPP_Activate role = DCE\n"
#define PF_SPP_175_112_2_18_1_7_45_1261 "[BT]SPP_Activate role = DTE\n"
#define PF_SPP_249_112_2_18_1_7_45_1262 "[BT]SPP_Deinit\n"
#define PF_SPP_271_112_2_18_1_7_45_1263 "[BT]SPP_sendLinkStatus status=%d\n"
#define PF_SPP_295_112_2_18_1_7_45_1264 "[BT]SPP_Deactivate app_handle = %d\n"
#define PF_SPP_319_112_2_18_1_7_45_1265 "[BT]SPP_Connect\n"
#define PF_SPP_320_112_2_18_1_7_45_1266 "app_handle = 0x%08x\n"
#define PF_SPP_366_112_2_18_1_7_45_1267 "[BT]SPP_Disconnect\n"
#define PF_SPP_367_112_2_18_1_7_45_1268 "[BT]muxId = 0x%04x\n"
#define PF_SPP_368_112_2_18_1_7_45_1269 "[BT]serviceHandle = 0x%04x\n"
#define PF_SPP_391_112_2_18_1_7_46_1270 "[BT]SPP_DataSend\n"
#define PF_SPP_392_112_2_18_1_7_46_1271 "[BT]payload       = 0x%08x\n"
#define PF_SPP_393_112_2_18_1_7_46_1272 "[BT]payload_len   = 0x%04x\n"
#define PF_SPP_394_112_2_18_1_7_46_1273 "[BT]muxId         = 0x%04x\n"
#define PF_SPP_395_112_2_18_1_7_46_1274 "[BT]serviceHandle = 0x%04x\n"
#define PF_SPP_432_112_2_18_1_7_46_1275 "[BT]SPP_controlReqSend\n"
#define PF_SPP_459_112_2_18_1_7_46_1276 "[BT]SPP_serviceNameResSend\n"
#define PF_SPP_460_112_2_18_1_7_46_1277 "[BT]muxId = 0x%04x\n"
#define PF_SPP_461_112_2_18_1_7_46_1278 "[BT]serviceHandle = 0x%04x\n"
#define PF_SPP_484_112_2_18_1_7_46_1279 "[BT]SPP_getInstancesQidReq\n"
#define PF_SPP_485_112_2_18_1_7_46_1280 "[BT]app_handle = 0x%04x\n"
#define PF_SPP_510_112_2_18_1_7_46_1281 "SPP_handleActivateCfm\n"
#define PF_SPP_511_112_2_18_1_7_46_1282 "muxId         = 0x%04x\n"
#define PF_SPP_512_112_2_18_1_7_46_1283 "serverChannel = 0x%04x\n"
#define PF_SPP_581_112_2_18_1_7_46_1284 "SPP_handleDeactivateCfm\n"
#define PF_SPP_582_112_2_18_1_7_46_1285 "type         = 0x%04x\n"
#define PF_SPP_583_112_2_18_1_7_46_1286 "phandle      = 0x%04x\n"
#define PF_SPP_584_112_2_18_1_7_46_1287 "result       = %d\n"
#define PF_SPP_585_112_2_18_1_7_46_1288 "state        = %d\n"
#define PF_SPP_618_112_2_18_1_7_46_1289 "phandlesListSize = 0x%04x\n"
#define PF_SPP_622_112_2_18_1_7_46_1290 "phandles = 0x%04x\n"
#define PF_SPP_653_112_2_18_1_7_46_1291 "result = 0x%04x\n"
#define PF_SPP_655_112_2_18_1_7_46_1292 "muxId = 0x%04x\n"
#define PF_SPP_656_112_2_18_1_7_46_1293 "serverChannel = 0x%04x\n"
#define PF_SPP_657_112_2_18_1_7_46_1294 "profileMaxFrameSize = 0x%04x\n"
#define PF_SPP_658_112_2_18_1_7_46_1295 "validPortPar = 0x%04x\n"
#define PF_SPP_659_112_2_18_1_7_46_1296 "portPar->port_speed = 0x%02x\n"
#define PF_SPP_660_112_2_18_1_7_46_1297 "portPar->data_bits = 0x%02x\n"
#define PF_SPP_661_112_2_18_1_7_46_1298 "portPar->stop_bits = 0x%02x\n"
#define PF_SPP_662_112_2_18_1_7_46_1299 "portPar->parity = 0x%02x\n"
#define PF_SPP_663_112_2_18_1_7_46_1300 "portPar->parity_type = 0x%02x\n"
#define PF_SPP_664_112_2_18_1_7_46_1301 "portPar->flow_ctrl_mask = 0x%02x\n"
#define PF_SPP_665_112_2_18_1_7_46_1302 "portPar->xon = 0x%02x\n"
#define PF_SPP_666_112_2_18_1_7_46_1303 "portPar->xoff = 0x%02x\n"
#define PF_SPP_667_112_2_18_1_7_46_1304 "portPar->parameter_mask = 0x%04x\n"
#define PF_SPP_726_112_2_18_1_7_46_1305 "SPP_handleStatusInd: muxId = 0x%04x\n"
#define PF_SPP_727_112_2_18_1_7_46_1306 "SPP_handleStatusInd: serverChannel = 0x%02x\n"
#define PF_SPP_728_112_2_18_1_7_46_1307 "SPP_handleStatusInd: connect = 0x%02x\n"
#define PF_SPP_729_112_2_18_1_7_46_1308 "SPP_handleStatusInd: maxMsgSize = 0x%04x\n"
#define PF_SPP_804_112_2_18_1_7_47_1309 "muxId = 0x%04x\n"
#define PF_SPP_805_112_2_18_1_7_47_1310 "serverChannel = 0x%02x\n"
#define PF_SPP_806_112_2_18_1_7_47_1311 "modemstatus = 0x%04x\n"
#define PF_SPP_807_112_2_18_1_7_47_1312 "break_signal = 0x%02x\n"
#define PF_SPP_833_112_2_18_1_7_47_1313 "[BT]SPP_handleControlInd. Peer device RTR and RTC is not ready!\n"
#define PF_SPP_859_112_2_18_1_7_47_1314 "muxId = 0x%04x\n"
#define PF_SPP_860_112_2_18_1_7_47_1315 "serverChannel = 0x%02x\n"
#define PF_SPP_862_112_2_18_1_7_47_1316 "result = 0x%02x\n"
#define PF_SPP_863_112_2_18_1_7_47_1317 "app_handle = 0x%04x\n"
#define PF_SPP_868_112_2_18_1_7_47_1318 "[BT] spp_data.state == %d\n"
#define PF_SPP_869_112_2_18_1_7_47_1319 "[BT] SPP_handleDisconnectInd: SPP_Deactivate\n"
#define PF_SPP_905_112_2_18_1_7_47_1320 "muxId = 0x%04x\n"
#define PF_SPP_908_112_2_18_1_7_47_1321 "serviceName = %s\n"
#define PF_SPP_909_112_2_18_1_7_47_1322 "serviceHandle = 0x%08x\n"
#define PF_SPP_911_112_2_18_1_7_47_1323 "serviceNameListSize = 0x%08x\n"
#define PF_SPP_940_112_2_18_1_7_47_1324 "[BT]SPP_PORTNEG_IND : portneg_ind->request is true\n"
#define PF_SPP_950_112_2_18_1_7_47_1325 "[BT]SPP_PORTNEG_IND : portneg_ind->request is false\n"
#define PF_SPP_983_112_2_18_1_7_47_1326 "muxId         = 0x%04x\n"
#define PF_SPP_984_112_2_18_1_7_47_1327 "serverChannel = 0x%04x\n"
#define PF_SPP_995_112_2_18_1_7_47_1328 "Spp kill timer when cfm\n"
#define PF_SPP_1035_112_2_18_1_7_47_1329 "SPP_handleDataInd: muxId         = 0x%04x\n"
#define PF_SPP_1036_112_2_18_1_7_47_1330 "SPP_handleDataInd: serverChannel = 0x%04x\n"
#define PF_SPP_1037_112_2_18_1_7_47_1331 "SPP_handleDataInd: payloadLength = 0x%04x\n"
#define PF_SPP_1038_112_2_18_1_7_47_1332 "SPP_handleDataInd: payload       = 0x%08x\n"
#define PF_SPP_1039_112_2_18_1_7_47_1333 "SPP_handleDataInd: payload       = %s\n"
#define PF_SPP_1066_112_2_18_1_7_47_1334 "[BT]SPP_handleDataInd: rx buffer full, lost data!!!\n"
#define PF_SPP_1087_112_2_18_1_7_47_1335 "[BT]SPP_handleDataInd:the SPP COM has not opened.\n "
#define PF_SPP_1114_112_2_18_1_7_47_1336 "[BT]SPP_DATA_IND rx_fifo_buf = %s\n"
#define PF_SPP_1115_112_2_18_1_7_47_1337 "SPP_DATA_IND Tick = %d\n"
#define PF_SPP_1138_112_2_18_1_7_47_1338 "[BT]SPP_handlePrim msg = 0x%x\n"
#define PF_SPP_1145_112_2_18_1_7_47_1339 "[BT]SPP_ACTIVATE_CFM\n"
#define PF_SPP_1151_112_2_18_1_7_47_1340 "[BT]SPP_DEACTIVATE_CFM\n"
#define PF_SPP_1157_112_2_18_1_7_47_1341 "[BT]SPP_CONNECT_IND\n"
#define PF_SPP_1167_112_2_18_1_7_47_1342 "[BT]SPP_DATA_CFM\n"
#define PF_SPP_1174_112_2_18_1_7_47_1343 "[BT]SPP_DATA_IND\n"
#define PF_SPP_1181_112_2_18_1_7_47_1344 "[BT]SPP_CONTROL_IND\n"
#define PF_SPP_1187_112_2_18_1_7_47_1345 "[BT]SPP_DISCONNECT_IND\n"
#define PF_SPP_1193_112_2_18_1_7_47_1346 "[BT]SPP_PORTNEG_IND\n"
#define PF_SPP_1200_112_2_18_1_7_47_1347 "[BT]SPP_PORTNEG_CFM\n"
#define PF_SPP_1205_112_2_18_1_7_47_1348 "[BT]SPP_SERVICE_NAME_IND\n"
#define PF_SPP_1211_112_2_18_1_7_47_1349 "[BT]SPP_AUDIO_IND\n"
#define PF_SPP_1216_112_2_18_1_7_47_1350 "[BT]SPP_AUDIO_RELEASE_IND\n"
#define PF_SPP_1221_112_2_18_1_7_47_1351 "[BT]SPP_MODE_CHANGE_IND\n"
#define PF_SPP_1226_112_2_18_1_7_47_1352 "[BT]SPP_STATUS_IND\n"
#define PF_SPP_1232_112_2_18_1_7_47_1353 "[BT]SPP_REGISTER_DATA_PATH_HANDLE_CFM\n"
#define PF_SPP_1237_112_2_18_1_7_48_1354 "[BT]SPP_DATA_PATH_STATUS_IND\n"
#define PF_SPP_1242_112_2_18_1_7_48_1355 "[BT]SPP_GET_INSTANCES_QID_CFM\n"
#define PF_SPP_1248_112_2_18_1_7_48_1356 "[BT]SPP_AUDIO_RENEGOTIATE_CFM\n"
#define PF_SPP_1253_112_2_18_1_7_48_1357 "[BT]SPP_AUDIO_RENEGOTIATE_IND\n"
#define PF_SPP_1283_112_2_18_1_7_48_1358 "[BT]SPP_handleDeinitPrim msg = 0x%4x\n"
#define PF_SPP_1308_112_2_18_1_7_48_1359 "[BT] Spp have connected with device!"
#define PF_SPP_1313_112_2_18_1_7_48_1360 "[BT] Spp have not connected with device!"
#define PF_SPP_1323_112_2_18_1_7_48_1361 "[BT] Spp have connected with device, please disconnect it first!"
#define PF_SPP_1341_112_2_18_1_7_48_1362 "[BT] SPP_ReleaseCallback: releasing!"
#define PF_SPP_1346_112_2_18_1_7_48_1363 "[BT] SPP_ReleaseCallback: released!"
#define PF_SPP_1372_112_2_18_1_7_48_1364 "[BT]SPP_OpenVitualCOM func = 0x%08x\n"
#define PF_SPP_1398_112_2_18_1_7_48_1365 "[BT]SPP_CloseVitualCOM\n"
#define PF_SPP_1433_112_2_18_1_7_48_1366 "[BT]BT_SppClose\n"
#define PF_SPP_1463_112_2_18_1_7_48_1367 "[BT] spp_data.state == SPP_IDLE\n"
#define PF_SPP_1464_112_2_18_1_7_48_1368 "[BT] spp_data.state = SPP_DEACTIVING\n"
#define PF_SPP_1470_112_2_18_1_7_48_1369 "[BT] spp_data.state == SPP_CONNECTED\n"
#define PF_SPP_1471_112_2_18_1_7_48_1370 "[BT] spp_data.state = SPP_DEACTIVING\n"
#define PF_SPP_1494_112_2_18_1_7_48_1371 "[BT]SPP_GetTxFifoCnt, maxMsgSize = %d\n"
#define PF_SPP_1520_112_2_18_1_7_48_1372 "[BT]SPP_ReadData ,buffer = 0x%08x,read_max_len = %d real_len=%dn"
#define PF_SPP_1535_112_2_18_1_7_48_1373 "[BT]SPP_ReadData ,buffer = 0x%08x\n"
#define PF_SPP_1549_112_2_18_1_7_48_1374 "[BT]SPP_ReadData ,buffer = 0x%08x\n"
#define PF_SPP_1580_112_2_18_1_7_48_1375 "[BT]SPP_WriteData ,buffer = 0x%08x len = %d\n"
#define PF_SPP_1626_112_2_18_1_7_48_1376 "Spp kill timer when start timer\n"
#define PF_SPP_1630_112_2_18_1_7_48_1377 "Spp start timer\n"
#define PF_SPP_1656_112_2_18_1_7_48_1378 "[BT]SPP_Tx_Int_Enable,is_enable = %d\n"
#define PF_SPP_1657_112_2_18_1_7_48_1379 "[BT]SPP_Tx_Int_Enable,peer_dev_rx_ready = %d\n"
#define PF_SPP_1658_112_2_18_1_7_48_1380 "[BT]SPP_Tx_Int_Enable,open_flag = %d\n"
#define PF_SPP_1659_112_2_18_1_7_48_1381 "[BT]SPP_Tx_Int_Enable,sppcom_info.tx_en = %d\n"
#define PF_SPP_1664_112_2_18_1_7_48_1382 "[BT]SPP__Tx_Int_Enable,The SPPCOM has been enabled!\n "
#define PF_SPP_1679_112_2_18_1_7_48_1383 "[BT]SPP_Tx_Int_Enable, Enable Tx but peer device rx is not ready or the SPPCOM is not open.\n "
#define PF_SPP_1705_112_2_18_1_7_48_1384 "[BT]SPP_Rx_Int_Enable,is_enable = %d\n"
#define PF_SPP_1738_112_2_18_1_7_49_1385 "[BT]APP_RxDataTest,rec_num = %d\n"
#define PF_SPP_1753_112_2_18_1_7_49_1386 "SPP uart_callback: EVENT_DATA_TO_READ\n"
#define PF_SPP_1785_112_2_18_1_7_49_1387 "SPP uart_callback: EVENT_DATA_TO_WRITE\n"
#define PF_SPP_1792_112_2_18_1_7_49_1388 "SPP uart_callback: 1->tx_data_len = %d\n"
#define PF_SPP_1798_112_2_18_1_7_49_1389 "SPP uart_callback: 2->tx_data_len = %d\n"
#define PF_SPP_1801_112_2_18_1_7_49_1390 "SPP uart_callback: 3->tx_data_len = %d\n"
#define PF_SPP_1807_112_2_18_1_7_49_1391 "SPP uart_callback: 4->tx_data_len = %d\n"
#define PF_SPP_1809_112_2_18_1_7_49_1392 "SPP uart_callback: 5->tx_data_len = %d\n"
#define PF_SPP_1818_112_2_18_1_7_49_1393 "SPP uart_callback: 6->tx_data_len = %d\n"
#define PF_SPP_1845_112_2_18_1_7_49_1394 "[BT]BT_SppTest!\n"
#define PF_SPP_1884_112_2_18_1_7_49_1395 "Spp kill timer when timer expired\n"
#define PF_SPP_1905_112_2_18_1_7_49_1396 "[BT]SPP_SendMsgToApp:Msg_Id=0x%X, status=0x%X, body=0x%X, size=0x%X\r\n"
#define PF_SPP_1909_112_2_18_1_7_49_1397 "[BT]SPP_SendMsgToApp:Invail Message\r\n"
#define PF_SPP_1926_112_2_18_1_7_49_1398 "[BT]SPP_SendMsgToApp:No CallBack Function\r\n"
#define PF_SPP_1979_112_2_18_1_7_49_1399 "[BT]SPP_Read_FIFO: Recieve data length = %d\n"
#define PF_SPP_1980_112_2_18_1_7_49_1400 "[BT]SPP_Read_FIFO: Read data length = %d\n"
#define PF_SPP_1981_112_2_18_1_7_49_1401 "[BT]SPP_Read_FIFO: p_rx_read = %d\n"
#define PF_SPP_1982_112_2_18_1_7_49_1402 "[BT]SPP_Read_FIFO: p_rx_write = %d\n"
#define PF_SPP_2012_112_2_18_1_7_49_1403 "[BT]SPP_TX_FIFO_Empty: tx_data_len = %d\n"
#define PF_SPP_2034_112_2_18_1_7_49_1404 "[BT]SPP_Read_FIFO: p_rx_read = %d\n"
#define PF_SPP_2035_112_2_18_1_7_49_1405 "[BT]SPP_Read_FIFO: p_rx_write = %d\n"
#define PF_SPP_2104_112_2_18_1_7_49_1406 "[BT]SPP_StartTimeOut:Create Timer handle=0x%08X\r\n"
#define PF_SPP_2108_112_2_18_1_7_49_1407 "[BT]SPP_StartTimeOut: Active Timer handle=0x%08X\r\n"
#define PF_SPP_2125_112_2_18_1_7_49_1408 "[BT]SPP_IsTimeOutStart:time_handle = 0x%08X\r\n"
#define PF_SPP_2143_112_2_18_1_7_49_1409 "[BT]OS_DeleteTimeOut:Timer handle = 0x%08X\r\n"
#define PF_SPP_2162_112_2_18_1_7_49_1410 "[BT]SPP_StopTimeOut:time_handle = 0x%08X\r\n"
#define PF_SPP_2179_112_2_18_1_7_49_1411 "[BT]SPP_Connect_TimeExpiredHandle:time_handle = 0x%08X\r\n"
#define PF_CM_REDUCE_326_112_2_18_1_7_50_1412 "[BT] CSR btPalCmGetCacheIndex adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_333_112_2_18_1_7_50_1413 "[BT] CSR btPalCmGetCacheIndex return TRUE (index:0x%x)"
#define PF_CM_REDUCE_338_112_2_18_1_7_50_1414 "[BT] CSR btPalCmGetCacheIndex return FALSE"
#define PF_CM_REDUCE_364_112_2_18_1_7_50_1415 "[BT] CSR btPalCmGetCacheElement wrapping index to zero"
#define PF_CM_REDUCE_371_112_2_18_1_7_50_1416 "[BT] CSR btPalCmGetCacheElement index not found using index: 0x%x  instead [nextFreeIndex 0x%x]"
#define PF_CM_REDUCE_397_112_2_18_1_7_50_1417 "[BT]TimeOutExitHandle:Type=0x%X\r\n"
#define PF_CM_REDUCE_428_112_2_18_1_7_50_1418 "[BT]CM_TimeOutHandle:Type=0x%X,%x\r\n"
#define PF_CM_REDUCE_453_112_2_18_1_7_50_1419 "[BT]:CM_TimeOutHandle - addr:lap:0x%X, uap:0x%X, nap:0x%X\r\n"
#define PF_CM_REDUCE_465_112_2_18_1_7_51_1420 "[BT]<ERROR>%s L%d: SC_DEBOND_CFM TimeOut"
#define PF_CM_REDUCE_507_112_2_18_1_7_51_1421 "[BT]CM_TimeOutHandle:Type  error"
#define PF_CM_REDUCE_517_112_2_18_1_7_51_1422 "[BT]CM_SC_Init\r\n"
#define PF_CM_REDUCE_547_112_2_18_1_7_51_1423 "[BT]CM_StartInquiry:classOfDeviceMask=0x%X,state=0x%X\r\n"
#define PF_CM_REDUCE_558_112_2_18_1_7_51_1424 "[BT] CM_StartInquiry creat cm_search_time fial"
#define PF_CM_REDUCE_564_112_2_18_1_7_51_1425 "[BT]CM_StartInquiry:BT is inquirying.......\r\n"
#define PF_CM_REDUCE_574_112_2_18_1_7_51_1426 "[BT][CSR_USE_FAST_SEARCH]CM_StartInquiry filters 0x%x"
#define PF_CM_REDUCE_597_112_2_18_1_7_51_1427 "[BT] CM_StopInquiry: state=0x%X\r\n"
#define PF_CM_REDUCE_609_112_2_18_1_7_51_1428 "[BT]CM_StopInquiry cancel time"
#define PF_CM_REDUCE_620_112_2_18_1_7_51_1429 "[BT][CSR_USE_FAST_SEARCH]CM_StopInquiry: Reading Remote Name\r\n"
#define PF_CM_REDUCE_624_112_2_18_1_7_51_1430 "[BT][CSR_USE_FAST_SEARCH]CM_StopInquiry: CmCancelDiscoveryReqSend\r\n"
#define PF_CM_REDUCE_634_112_2_18_1_7_51_1431 "[BT]CM_StopInquiry:BT is not in inqirying!\r\n"
#define PF_CM_REDUCE_660_112_2_18_1_7_51_1432 "[BT]CM_SetVisibleCfm\r\n"
#define PF_CM_REDUCE_671_112_2_18_1_7_51_1433 "[BT] CM_SetVisibleCfm fail"
#define PF_CM_REDUCE_695_112_2_18_1_7_51_1434 "[BT]CM_SetVisible:scan=0x%X, inquiry=0x%X, page=0x%X\r\n"
#define PF_CM_REDUCE_717_112_2_18_1_7_51_1435 "[BT] CM_SetVisible fail"
#define PF_CM_REDUCE_732_112_2_18_1_7_51_1436 "[BT]CM_RestoreVisible\r\n"
#define PF_CM_REDUCE_745_112_2_18_1_7_51_1437 "[BT]CM_SetVisibleAtPowerOn:isVisibleAtPowerOn=0x%X\r\n"
#define PF_CM_REDUCE_761_112_2_18_1_7_51_1438 "[BT]CM_ScanReadEnable\r\n"
#define PF_CM_REDUCE_767_112_2_18_1_7_51_1439 "[BT]CM_ScanWriteEnable:inuiry=0x%X,page=0x%X\r\n"
#define PF_CM_REDUCE_773_112_2_18_1_7_51_1440 "[BT]CM_ReadLocalName\r\n"
#define PF_CM_REDUCE_780_112_2_18_1_7_51_1441 "[BT]CM_SetLocalName:name_ptr=%s\r\n"
#define PF_CM_REDUCE_788_112_2_18_1_7_51_1442 "[BT]CM_ReadLocalBdAddr\r\n"
#define PF_CM_REDUCE_797_112_2_18_1_7_51_1443 "[BT]CM_DeBondReq:state=0x%X,addr lap:0x%x,uap:0x%x,nap:0x%xrn"
#define PF_CM_REDUCE_869_112_2_18_1_7_51_1444 "Devclass have been added in list "
#define PF_CM_REDUCE_875_112_2_18_1_7_51_1445 " not a dongle device"
#define PF_CM_REDUCE_880_112_2_18_1_7_51_1446 "AddDevicelist device_info= 0x%x,name[0]=0x%x,name[1]=0x%x,name[2]=0x%x"
#define PF_CM_REDUCE_909_112_2_18_1_7_51_1447 " error ,error ,error !!! device is paired full"
#define PF_CM_REDUCE_912_112_2_18_1_7_51_1448 "devcice number =%d"
#define PF_CM_REDUCE_944_112_2_18_1_7_51_1449 "not find the device in the list"
#define PF_CM_REDUCE_965_112_2_18_1_7_52_1450 "[CM_GetDeviceClass] IN ( BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x: )"
#define PF_CM_REDUCE_972_112_2_18_1_7_52_1451 "device_class=0x%x,i=%d"
#define PF_CM_REDUCE_997_112_2_18_1_7_52_1452 "[CM_GetDeviceClass] IN ( BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x: )"
#define PF_CM_REDUCE_1004_112_2_18_1_7_52_1453 "device_class=0x%x,i=%d"
#define PF_CM_REDUCE_1019_112_2_18_1_7_52_1454 "[BT]SC_SetPairDeviceStatus: status = %d\n"
#define PF_CM_REDUCE_1028_112_2_18_1_7_52_1455 "[BT]SC_SetPairDeviceAddr: is_paired = %d\n"
#define PF_CM_REDUCE_1039_112_2_18_1_7_52_1456 "[BT]SC_GetPairDeviceAddr: is_paired = %d\n"
#define PF_CM_REDUCE_1079_112_2_18_1_7_52_1457 "SetDevicePaired device_class=0x%x,i=%d"
#define PF_CM_REDUCE_1084_112_2_18_1_7_52_1458 "error,error,device don't find"
#define PF_CM_REDUCE_1104_112_2_18_1_7_52_1459 "[BT]CM_MsgPrimHandle: type = 0x%X\r\n"
#define PF_CM_REDUCE_1119_112_2_18_1_7_52_1460 "[BT]:CM_DISCOVERY_RESULT_IND - addr:lap:0x%X, uap:0x%X, nap:0x%X\r\n"
#define PF_CM_REDUCE_1124_112_2_18_1_7_52_1461 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND: Normal Search\r\n"
#define PF_CM_REDUCE_1130_112_2_18_1_7_52_1462 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND: Normal Search - Found A Same Device\r\n"
#define PF_CM_REDUCE_1136_112_2_18_1_7_52_1463 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND: IsSameDevAddr\r\n"
#define PF_CM_REDUCE_1142_112_2_18_1_7_52_1464 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND: Normal Search - Sent to APP\r\n"
#define PF_CM_REDUCE_1151_112_2_18_1_7_52_1465 "[BT]CM_DISCOVERY_RESULT_IND cache adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1152_112_2_18_1_7_52_1466 "[BT]CM_DISCOVERY_RESULT_IND cache name %s"
#define PF_CM_REDUCE_1153_112_2_18_1_7_52_1467 "[BT]CM_DISCOVERY_RESULT_IND prim adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1154_112_2_18_1_7_52_1468 "[BT]CM_DISCOVERY_RESULT_IND prim name %s"
#define PF_CM_REDUCE_1179_112_2_18_1_7_52_1469 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND:found a same device in normal mode\r\n"
#define PF_CM_REDUCE_1193_112_2_18_1_7_52_1470 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND:found a new device cod: 0x%x 0x%xrn"
#define PF_CM_REDUCE_1197_112_2_18_1_7_52_1471 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND:found a same device\r\n"
#define PF_CM_REDUCE_1207_112_2_18_1_7_52_1472 "[BT]CM_DISCOVERY_RESULT_IND:Search a same device\r\n"
#define PF_CM_REDUCE_1219_112_2_18_1_7_52_1473 "[BT]CM_DISCOVERY_RESULT_IND:Search a empty device name\r\n"
#define PF_CM_REDUCE_1237_112_2_18_1_7_52_1474 "[BT]CM_DISCOVERY_RESULT_IND cache adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1238_112_2_18_1_7_52_1475 "[BT]CM_DISCOVERY_RESULT_IND cache name %s"
#define PF_CM_REDUCE_1239_112_2_18_1_7_52_1476 "[BT]CM_DISCOVERY_RESULT_IND prim adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1240_112_2_18_1_7_52_1477 "[BT]CM_DISCOVERY_RESULT_IND prim name %s"
#define PF_CM_REDUCE_1269_112_2_18_1_7_52_1478 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM BCHS_CANCEL_INQUIRYING cnt:%d"
#define PF_CM_REDUCE_1278_112_2_18_1_7_52_1479 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM All Discoveries Complete cnd:%d"
#define PF_CM_REDUCE_1283_112_2_18_1_7_52_1480 "[BT] MsgPrimHandle cancel serach time"
#define PF_CM_REDUCE_1296_112_2_18_1_7_52_1481 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM CmReadRemoteNameReqSend"
#define PF_CM_REDUCE_1304_112_2_18_1_7_52_1482 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM CmDiscoveryReqSend filters 0x%x No of: %d"
#define PF_CM_REDUCE_1310_112_2_18_1_7_52_1483 "[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM All Discoveries Complete cnd:%d"
#define PF_CM_REDUCE_1317_112_2_18_1_7_52_1484 "[BT] MsgPrimHandle CM_DISCOVERY_CFM"
#define PF_CM_REDUCE_1322_112_2_18_1_7_52_1485 "[BT] MsgPrimHandle cancel serach time"
#define PF_CM_REDUCE_1332_112_2_18_1_7_52_1486 "[BT]CM_CANCEL_DISCOVERY_CFM:state=%d\r\n"
#define PF_CM_REDUCE_1369_112_2_18_1_7_52_1487 "[BT]CM_CANCEL_DISCOVERY_CFM:error state\r\n"
#define PF_CM_REDUCE_1384_112_2_18_1_7_52_1488 "[BT]CM_MsgPrimHandle: name = %s\r\n"
#define PF_CM_REDUCE_1408_112_2_18_1_7_52_1489 "[BT]CM_READ_REMOTE_NAME_IND: name = %s\r\n"
#define PF_CM_REDUCE_1411_112_2_18_1_7_52_1490 "[BT]CM_READ_REMOTE_NAME_IND The Name has not been change "
#define PF_CM_REDUCE_1415_112_2_18_1_7_52_1491 "[BT]CM_READ_REMOTE_NAME_IND copying the name"
#define PF_CM_REDUCE_1432_112_2_18_1_7_52_1492 "[BT]CM_READ_REMOTE_NAME_CFM:resul = 0x%X\r\n"
#define PF_CM_REDUCE_1473_112_2_18_1_7_53_1493 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM BCHS_CANCEL_INQUIRYING"
#define PF_CM_REDUCE_1481_112_2_18_1_7_53_1494 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM Addr:lap:0x%X, uap:0x%X, nap:0x%X"
#define PF_CM_REDUCE_1487_112_2_18_1_7_53_1495 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM SUCCESS: %s"
#define PF_CM_REDUCE_1502_112_2_18_1_7_53_1496 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM SUCCESS cod: 0x%x\r\n"
#define PF_CM_REDUCE_1526_112_2_18_1_7_53_1497 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM FAIL: 0x%x Named Unknown"
#define PF_CM_REDUCE_1533_112_2_18_1_7_53_1498 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM MAX_DEVICE_DISCOVERY"
#define PF_CM_REDUCE_1543_112_2_18_1_7_53_1499 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM CmReadRemoteNameReqSend"
#define PF_CM_REDUCE_1551_112_2_18_1_7_53_1500 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM CmDiscoveryReqSend filters 0x%x"
#define PF_CM_REDUCE_1557_112_2_18_1_7_53_1501 "[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM All Discoveries Complete"
#define PF_CM_REDUCE_1621_112_2_18_1_7_53_1502 "[BT]CM_SET_LOCAL_NAME_CFM:result = 0x%X, start=0x%Xrn"
#define PF_CM_REDUCE_1633_112_2_18_1_7_53_1503 "[BT]CM_SET_LOCAL_NAME_CFM: allocating cache"
#define PF_CM_REDUCE_1657_112_2_18_1_7_53_1504 "[BT]CM_READ_LOCAL_BD_ADDR_CFM:lap:%d, uap:%d,nap:%d\r\n"
#define PF_CM_REDUCE_1667_112_2_18_1_7_53_1505 "[BT]Scan Read result:= 0x%x, scanEnable:= 0x%x\r\n"
#define PF_CM_REDUCE_1712_112_2_18_1_7_53_1506 "[BT]CM_READ_SCAN_ENABLE_CFM:Invaild Message\r\n"
#define PF_CM_REDUCE_1798_112_2_18_1_7_53_1507 "[BT]CM_SDC_SEARCH_IND Result - prim->service = 0x%x"
#define PF_CM_REDUCE_1802_112_2_18_1_7_53_1508 "knownServices11_00_31 = 0x%08x\n"
#define PF_CM_REDUCE_1807_112_2_18_1_7_53_1509 "knownServices11_32_63  = 0x%08x\n"
#define PF_CM_REDUCE_1812_112_2_18_1_7_53_1510 "knownServices12_00_31 = 0x%08x\n"
#define PF_CM_REDUCE_1817_112_2_18_1_7_53_1511 "knownServices13_00_31 = 0x%08x\n"
#define PF_CM_REDUCE_1821_112_2_18_1_7_53_1512 "The service is currently not supported "
#define PF_CM_REDUCE_1829_112_2_18_1_7_53_1513 "[BT]CM_SDC_SEARCH_CFM"
#define PF_CM_REDUCE_1840_112_2_18_1_7_53_1514 "[BT]CM_SDC_CLOSE_IND Result=0x%x "
#define PF_CM_REDUCE_1855_112_2_18_1_7_53_1515 "[BT] CSR remoteName   = %s\n"
#define PF_CM_REDUCE_1856_112_2_18_1_7_53_1516 "[BT] CSR BD adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1857_112_2_18_1_7_53_1517 "[BT] CSR (prim) BD adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1866_112_2_18_1_7_53_1518 "[BT] CSR remoteName   = %d\n"
#define PF_CM_REDUCE_1867_112_2_18_1_7_53_1519 "[BT] CSR BD adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1868_112_2_18_1_7_53_1520 "[BT] CSR (prim) BD adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1872_112_2_18_1_7_53_1521 "[BT] CSR cache look up failed\n"
#define PF_CM_REDUCE_1890_112_2_18_1_7_53_1522 "[BT] CSR EMULATED SD_READ_AVAILABLE_SERVICES_CFM\r\n"
#define PF_CM_REDUCE_1891_112_2_18_1_7_53_1523 "[BT] CSR result                = %d\n"
#define PF_CM_REDUCE_1892_112_2_18_1_7_53_1524 "[BT] CSR remoteNameChanged     = %d\n"
#define PF_CM_REDUCE_1893_112_2_18_1_7_53_1525 "[BT] CSR remoteName  = %s\n"
#define PF_CM_REDUCE_1894_112_2_18_1_7_53_1526 "[BT] CSR BD adress: 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_1895_112_2_18_1_7_53_1527 "[BT] CSR knownServices11_00_31 = 0x%08x [appServ 0x%08x]"
#define PF_CM_REDUCE_1896_112_2_18_1_7_53_1528 "[BT] CSR knownServices11_32_63 = 0x%08x [appServ 0x%08x]"
#define PF_CM_REDUCE_1897_112_2_18_1_7_53_1529 "[BT] CSR knownServices12_00_31 = 0x%08x [appServ 0x%08x]"
#define PF_CM_REDUCE_1898_112_2_18_1_7_53_1530 "[BT] CSR knownServices13_00_31 = 0x%08x [appServ 0x%08x]"
#define PF_CM_REDUCE_1899_112_2_18_1_7_53_1531 "[BT] CSR authorised = 0x%x classOfDevice=0x%x search found=0x%x]"
#define PF_CM_REDUCE_1912_112_2_18_1_7_53_1532 "[BT] CSR ID_STATUS_SD_READ_SERVICES_RES "
#define PF_CM_REDUCE_1925_112_2_18_1_7_53_1533 "[BT] CSR ID_STATUS_SC_PAIR OK "
#define PF_CM_REDUCE_1988_112_2_18_1_7_54_1534 "[BT]PF_MsgCmPrimHandle:Unhandle\r\n"
#define PF_CM_REDUCE_2004_112_2_18_1_7_54_1535 "[BT] MsgPrimHandle cancel serach time"
#define PF_CM_REDUCE_2040_112_2_18_1_7_54_1536 "[BT]_StartPairing: deviceAddr = %04X:%02X:%06X"
#define PF_CM_REDUCE_2062_112_2_18_1_7_54_1537 "[BT]SC_StartPairing:state=0x%X\r\n"
#define PF_CM_REDUCE_2070_112_2_18_1_7_54_1538 "[BT]SC_StartPairing:Cancel Discovery at first!\r\n"
#define PF_CM_REDUCE_2077_112_2_18_1_7_54_1539 "[BT][CSR_USE_FAST_SEARCH]SC_StartPairing: Reading Remote Name\r\n"
#define PF_CM_REDUCE_2081_112_2_18_1_7_54_1540 "[BT][CSR_USE_FAST_SEARCH]SC_StartPairing: CmCancelDiscoveryReqSend\r\n"
#define PF_CM_REDUCE_2100_112_2_18_1_7_54_1541 "[BT]SC_CancelPairing:state=0x%X\r\n"
#define PF_CM_REDUCE_2101_112_2_18_1_7_54_1542 "[BT]SC_CancelPairing - dev_addr:lap=%d,uap=%d,nap=%d\r\n"
#define PF_CM_REDUCE_2143_112_2_18_1_7_54_1543 "[BT]SC_CancelPairing: Empty Operate!\r\n"
#define PF_CM_REDUCE_2156_112_2_18_1_7_54_1544 "[BT]SC_InputPairPin:lap=0x%X,uap=0x%X, nap=0x%X\r\n"
#define PF_CM_REDUCE_2183_112_2_18_1_7_54_1545 "[BT]SC_MsgPrimHandle:type = 0x%X\r\n"
#define PF_CM_REDUCE_2192_112_2_18_1_7_54_1546 "[BT]SC_PASSKEY_IND:lap=0x%X,uap=0x%X, nap=0x%X\r\n"
#define PF_CM_REDUCE_2230_112_2_18_1_7_54_1547 "[BT]SC_BOND_CFM: result= 0x%X,cod=%x,class=%x\r\n"
#define PF_CM_REDUCE_2247_112_2_18_1_7_54_1548 "[BT]<INFO> cod = 0x%X, it's a Audio Device"
#define PF_CM_REDUCE_2261_112_2_18_1_7_54_1549 "[BT] CSR  CmSdcSearchReqSend  SC_BOND_CFM\r\n"
#define PF_CM_REDUCE_2271_112_2_18_1_7_54_1550 "[BT] CSR SD_ReadAvailableServerReq NOT EXECUTED uuid=0x%x service target addr : 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_2281_112_2_18_1_7_54_1551 "[BT] state is idle"
#define PF_CM_REDUCE_2287_112_2_18_1_7_54_1552 "[BT]BCHS_BONDING_CANCELLED SUCCESSFUL "
#define PF_CM_REDUCE_2305_112_2_18_1_7_54_1553 "[BT]SC_BOND_IND:result= 0x%X\r\n"
#define PF_CM_REDUCE_2322_112_2_18_1_7_54_1554 "[BT]<INFO> cod = 0x%X, it's a Audio Device"
#define PF_CM_REDUCE_2332_112_2_18_1_7_54_1555 "[BT] CSR  CmSdcSearchReqSend SC_BOND_IND \r\n"
#define PF_CM_REDUCE_2342_112_2_18_1_7_54_1556 "[BT] CSR SD_ReadAvailableServerReq NOT EXECUTED uuid=0x%x service target addr : 0x%x:0x%x:0x%x"
#define PF_CM_REDUCE_2362_112_2_18_1_7_54_1557 "[BT]SC_AUTHORISE_IND\r\n"
#define PF_CM_REDUCE_2375_112_2_18_1_7_54_1558 "[BT]SC_DEBOND_CFM:result=0x%X\r\n"
#define PF_CM_REDUCE_2388_112_2_18_1_7_55_1559 "\"\"\"# default in scPrim handler %x,\n"
#define PF_SD_REDUCE_27_112_2_18_1_7_55_1560 "[BT]SD_ReadServiceTimeOutHandle:Time out\r\n"
#define PF_SD_REDUCE_86_112_2_18_1_7_55_1561 " btPalEmulateSdCopyCacheTypeToDevicePropertiesType"
#define PF_SD_REDUCE_87_112_2_18_1_7_55_1562 "dest knownServices11_00_31 = 0x%08x\n"
#define PF_SD_REDUCE_88_112_2_18_1_7_55_1563 "dest knownServices11_32_63  = 0x%08x\n"
#define PF_SD_REDUCE_89_112_2_18_1_7_55_1564 "dest knownServices12_00_31 = 0x%08x\n"
#define PF_SD_REDUCE_90_112_2_18_1_7_55_1565 "dest knownServices13_00_31 = 0x%08x\n"
#define PF_SD_REDUCE_122_112_2_18_1_7_55_1566 " btPalEmulateSdReadDevicePropertiesReqSend cache entry found"
#define PF_SD_REDUCE_127_112_2_18_1_7_55_1567 " btPalEmulateSdReadDevicePropertiesReqSend cache entry NOT found!!!!"
#define PF_SD_REDUCE_135_112_2_18_1_7_55_1568 "[BT]SD_ReadAvailableServerReq\r\n"
#define PF_SD_REDUCE_152_112_2_18_1_7_55_1569 "[BT]SD_ReadAvailableServerReq NOT EXECUTED uuid=0x%x service target addr : 0x%x:0x%x:0x%x"
#define PF_SD_REDUCE_193_112_2_18_1_7_55_1570 "[BT]SD_HandlePrim:type = 0x%X\r\n"
#define PF_SD_REDUCE_206_112_2_18_1_7_55_1571 "[BT]SD_READ_DEVICE_PROPERTIES_CFM:result = 0x%X\r\n"
#define PF_SD_REDUCE_236_112_2_18_1_7_55_1572 "[BT]SD_READ_AVAILABLE_SERVICES_CFM\r\n"
#define PF_SD_REDUCE_237_112_2_18_1_7_55_1573 "result                = %d\n"
#define PF_SD_REDUCE_238_112_2_18_1_7_55_1574 "remoteNameChanged     = %d\n"
#define PF_SD_REDUCE_242_112_2_18_1_7_55_1575 "deviceAddr            = %04x:%02x:%06x"
#define PF_SD_REDUCE_244_112_2_18_1_7_55_1576 "friendlyName          = %s"
#define PF_SD_REDUCE_246_112_2_18_1_7_55_1577 "classOfDevice         = 0x%x"
#define PF_SD_REDUCE_248_112_2_18_1_7_55_1578 "knownServices11_00_31 = 0x%08xn"
#define PF_SD_REDUCE_249_112_2_18_1_7_55_1579 "s_is_refresh_server   = 0x%x\n"
#define PF_SD_REDUCE_323_112_2_18_1_7_55_1580 "[DRV_BT]<ASSERT_ERROR>: SD_HandlePrim: unkown type"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_BT_PAL_CSR_TRC)
TRACE_MSG(BT_ABS_60_112_2_18_1_6_19_0,"[BT_Config]: no valid com %d")
TRACE_MSG(BT_ABS_66_112_2_18_1_6_19_1,"[BT_Config] bd_addr = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x")
TRACE_MSG(BT_ABS_67_112_2_18_1_6_19_2,"[BT_Config] port_num = %d")
TRACE_MSG(BT_ABS_68_112_2_18_1_6_19_3,"[BT_Config] baud_rate = %d")
TRACE_MSG(BT_ABS_75_112_2_18_1_6_19_4,"[BT]dir[%d]=0x%X\r\n")
TRACE_MSG(BT_ABS_101_112_2_18_1_6_19_5,"[BT]BT_Init\r\n")
TRACE_MSG(BT_ABS_153_112_2_18_1_6_19_6,"[BT]BT_Deinit\r\n")
TRACE_MSG(BT_ABS_162_112_2_18_1_6_19_7,"[BT]BT_TestInit\r\n")
TRACE_MSG(BT_ABS_189_112_2_18_1_6_19_8,"[BT]BT_Start\r\n")
TRACE_MSG(BT_ABS_257_112_2_18_1_6_19_9,"[BT_SearchDevice] IN (service_type = 0x%x)")
TRACE_MSG(BT_ABS_448_112_2_18_1_6_20_10,"[BT_GetDeviceType] IN (class of device = 0x%06x and ")
TRACE_MSG(BT_ABS_452_112_2_18_1_6_20_11,"[BT]<INFO> cod = DB_GetDeviceClass() = 0x%x")
TRACE_MSG(BT_ABS_540_112_2_18_1_6_20_12,"[BT_GetPairedDeviceCount] IN (service_type = 0x%x)")
TRACE_MSG(BT_ABS_595_112_2_18_1_6_20_13,"[BT_GetPairedDeviceInfo] IN (service_type = 0x%x, index = %d)")
TRACE_MSG(BT_ABS_728_112_2_18_1_6_20_14,"[BT]<WARNING> %s L%d: NO avalable service")
TRACE_MSG(BT_ABS_766_112_2_18_1_6_20_15,"[BT_GetPairedDeviceService] service = 0x%x")
TRACE_MSG(BT_ABS_1026_112_2_18_1_6_21_16,"BT_SetFtpRootFolder path[0] %d, path[1] %d  mode %s")
TRACE_MSG(BT_ABS_2220_112_2_18_1_6_23_17,"[BT]BT_SppConnect\n")
TRACE_MSG(BT_ABS_2226_112_2_18_1_6_23_18,"[BT] Spp have connected with device, please disconnect it first!")
TRACE_MSG(BT_ABS_2275_112_2_18_1_6_23_19,"[BT] Spp have connected with device, please disconnect it first!")
TRACE_MSG(BT_ABS_2279_112_2_18_1_6_23_20,"[BT] BT_SppInitCallback!")
TRACE_MSG(BT_ABS_2312_112_2_18_1_6_23_21,"[BT] BT_SppReleaseCallback!")
TRACE_MSG(BT_ABS_2317_112_2_18_1_6_23_22,"[BT] Spp have bot connected with any device!")
TRACE_MSG(BT_ABS_2347_112_2_18_1_6_23_23,"[BT]BT_SppDisconnect\n")
TRACE_MSG(BT_ABS_2363_112_2_18_1_6_23_24,"[BT]BT_Spp_WriteData: src_buf = %s\n")
TRACE_MSG(BT_ABS_2364_112_2_18_1_6_23_25,"[BT]BT_Spp_WriteData: data_len = %d\n")
TRACE_MSG(BT_ABS_2365_112_2_18_1_6_23_26,"[BT]BT_Spp_WriteData: src_buf = %d\n")
TRACE_MSG(BT_ABS_2377_112_2_18_1_6_23_27,"[BT]BT_Spp_WriteData: FIFO full\n")
TRACE_MSG(BT_ABS_2382_112_2_18_1_6_23_28,"[BT]BT_Spp_WriteData: SPP have not connected!!\n")
TRACE_MSG(BT_ABS_2406_112_2_18_1_6_23_29,"[BT]BT_Spp_WriteData: SPP have not connected!!\n")
TRACE_MSG(BT_DUT_99_112_2_18_1_6_24_30,"[DUT_HandlePrim] prim_type = 0x%x")
TRACE_MSG(BT_DUT_109_112_2_18_1_6_24_31,"CM_BCCMD_CFM")
TRACE_MSG(BT_DUT_119_112_2_18_1_6_24_32,"CM_ENABLE_DUT_MODE_CFM: result = %d, stepNumber = %d")
TRACE_MSG(BT_DUT_143_112_2_18_1_6_24_33,"[_dut_handler] subtype = %d")
TRACE_MSG(BT_DUT_205_112_2_18_1_6_24_34,"[DRV_BT]<ASSERT_ERROR> _dut_handler")
TRACE_MSG(BT_DUT_356_112_2_18_1_6_24_35,"[_start_radio_test] test_item=%d, channel=%d, payload_type=%d, packet_type=%d")
TRACE_MSG(BT_DUT_400_112_2_18_1_6_24_36,"[_start_afc] channel = %d")
TRACE_MSG(BT_DUT_417_112_2_18_1_6_24_37,"[_set_dac_value] dac = %d")
TRACE_MSG(BT_DUT_426_112_2_18_1_6_24_38,"[_save_dac_value] dac = %d")
TRACE_MSG(BT_DUT_435_112_2_18_1_6_24_39,"[_load_dac_value] dac = %d")
TRACE_MSG(BT_FM_116_112_2_18_1_6_25_40,"[BT_FM] FM_SetStatus: new status = %d, old one = %d")
TRACE_MSG(BT_FM_141_112_2_18_1_6_25_41,"[BT_FM] FM_Init")
TRACE_MSG(BT_FM_145_112_2_18_1_6_25_42,"FM_Init: Don't need to Initailize again!!!")
TRACE_MSG(BT_FM_162_112_2_18_1_6_25_43,"[BT_FM] FM_Init register cfm")
TRACE_MSG(BT_FM_183_112_2_18_1_6_25_44,"[BT_FM] FM_Open,Version20080905")
TRACE_MSG(BT_FM_200_112_2_18_1_6_25_45,"[BT_FM] FM_Open enble cfm")
TRACE_MSG(BT_FM_217_112_2_18_1_6_25_46,"[BT_FM] FM_Open unmute cfm")
TRACE_MSG(BT_FM_227_112_2_18_1_6_25_47,"[BT_FM]FM_Open: NO Actions! ")
TRACE_MSG(BT_FM_240_112_2_18_1_6_25_48,"[BT_FM] FM_Start: freq = %d")
TRACE_MSG(BT_FM_267_112_2_18_1_6_25_49,"[BT_FM] FM_resume mute cfg")
TRACE_MSG(BT_FM_286_112_2_18_1_6_25_50,"[BT_FM] FM_Start: set frequnency cfm")
TRACE_MSG(BT_FM_298_112_2_18_1_6_25_51,"[BT_FM] FM_Start: NO Acitons!")
TRACE_MSG(BT_FM_309_112_2_18_1_6_25_52,"[BT_FM] FM_Stop")
TRACE_MSG(BT_FM_324_112_2_18_1_6_25_53,"[BT_FM] FM_Stop mute cfg")
TRACE_MSG(BT_FM_335_112_2_18_1_6_25_54,"[BT_FM] FM_Stop NO Actions")
TRACE_MSG(BT_FM_348_112_2_18_1_6_25_55,"[BT_FM] FM_Close")
TRACE_MSG(BT_FM_352_112_2_18_1_6_25_56,"FM_Close: Don't need to Close")
TRACE_MSG(BT_FM_367_112_2_18_1_6_25_57,"[BT_FM] FM_Close disable cfg")
TRACE_MSG(BT_FM_391_112_2_18_1_6_25_58,"[BT_FM] FM_ManualSeek: freq = %d")
TRACE_MSG(BT_FM_417_112_2_18_1_6_26_59,"[BT_FM] FM_ManualSeek: seek_dir error")
TRACE_MSG(BT_FM_445_112_2_18_1_6_26_60,"[BT_FM] FM_AutoSeek: seek_dir = %d, mode = %d, freq = %d")
TRACE_MSG(BT_FM_492_112_2_18_1_6_26_61,"[BT_FM] FM_AutoSeek: seek_dir error")
TRACE_MSG(BT_FM_513_112_2_18_1_6_26_62,"[BT_FM] FM_SetVol: channel = %d, level = %d")
TRACE_MSG(BT_FM_539_112_2_18_1_6_26_63,"[BT_FM] FM_SetVol: FM state error")
TRACE_MSG(BT_FM_554_112_2_18_1_6_26_64,"[BT_FM] FM_SetScanLevel: level = %d")
TRACE_MSG(BT_FM_578_112_2_18_1_6_26_65,"[BT_FM] FM_SetScanLevel: db = %d")
TRACE_MSG(BT_FM_586_112_2_18_1_6_26_66,"[BT_FM] FM_SetScanLevel req ")
TRACE_MSG(BT_FM_621_112_2_18_1_6_26_67,"[BT_FM] FM_SetRegion: fm_region = %d")
TRACE_MSG(BT_FM_642_112_2_18_1_6_26_68,"[BT_FM] FM_SetRegion req cfm")
TRACE_MSG(BT_FM_675_112_2_18_1_6_26_69,"[FM_HandlePrim] IN")
TRACE_MSG(BT_FM_684_112_2_18_1_6_26_70,"[BT_FM]FM_HandlePrim:type = 0x%X\r\n")
TRACE_MSG(BT_FM_779_112_2_18_1_6_26_71,"[BT_FM]FM_HandlePrim:FM_AUTO_SEARCH_RESULT_IND frequency = %d")
TRACE_MSG(BT_FM_788_112_2_18_1_6_26_72,"[BT_FM]FM_HandlePrim:FM_AUTO_SEARCH_RESULT_IND invalid station")
TRACE_MSG(BCCMD_BOOT_STRAP_1471_112_2_18_1_6_30_73,"BootStrap: Baudrate set to: %d\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1480_112_2_18_1_6_30_74," Can not set 0 as the address of bluetooth,set it default")
TRACE_MSG(BCCMD_BOOT_STRAP_1487_112_2_18_1_6_30_75,"BootStrap: Bluetooth device address set to: %04x:%x:%x\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1493_112_2_18_1_6_30_76,"BootStrap: Crystal Frequency set to: %d\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1507_112_2_18_1_6_30_77,"BootStrap: Setting Friendly name to: %s\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1527_112_2_18_1_6_30_78,"[BT]BCCMD_SetBaudRate:User baud=0x%X\r\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1563_112_2_18_1_6_30_79,"[BT]BCCMD_SetBaudRate;baud=0x%X\r\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1567_112_2_18_1_6_30_80,"[BT]BCCMD_SetCrystalFreq;freq_khz=0x%X\r\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1577_112_2_18_1_6_30_81," Can not set 0 as the address of bluetooth,set it default")
TRACE_MSG(BCCMD_BOOT_STRAP_1584_112_2_18_1_6_30_82,"[BT]BCCMD_SetBtAddr;btNap=0x%X,btUap=0x%X,btLap=0x%X\r\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1589_112_2_18_1_6_30_83,"[BT]BCCMD_SetPcmConfig32:config=0x%X\r\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1595_112_2_18_1_6_30_84,"[BT]BCCMD_SetPcmFormat:format=0x%X\r\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1602_112_2_18_1_6_30_85,"[BT]BCCMD_SetPcmSlot:Slot=0x%X\r\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1608_112_2_18_1_6_30_86,"[BT] BCCMD_SetXtalFtrim: ftrim = %d")
TRACE_MSG(BCCMD_BOOT_STRAP_1621_112_2_18_1_6_30_87,"[BT]reConfigureBaudRate:baudIn=0x%X\r\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1667_112_2_18_1_6_30_88,"error ,error error baudrate")
TRACE_MSG(BCCMD_BOOT_STRAP_1725_112_2_18_1_6_30_89,"build_boot_strap_pskey_set key %04x data %04x len %04x\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1791_112_2_18_1_6_30_90,"build_boot_strap_bccmd varid %04x data %p len %04x\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1871_112_2_18_1_6_30_91,"BootStrap: State %d seq_no %d\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1904_112_2_18_1_6_30_92,"BootStrap: EARLY_PATCHES - BuildID %d (0x%04x)\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1908_112_2_18_1_6_30_93,"BootStrap: EARLY_PATCHES - Result status %02x%02x\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1914_112_2_18_1_6_31_94,"BootStrap: EARLY_PATCHES - Did not get result\n")
TRACE_MSG(BCCMD_BOOT_STRAP_1931_112_2_18_1_6_31_95,"BootStrap:NO EARLY PATCHES NEEDED: NOP\n")
TRACE_MSG(BCCMD_BOOT_STRAP_2275_112_2_18_1_6_31_96,"Apply key index %d len %d\n")
TRACE_MSG(BCCMD_BOOT_STRAP_2290_112_2_18_1_6_31_97,"BootStrap:PATCHES_AND_KEYS: NOP\n")
TRACE_MSG(BCCMD_BOOT_STRAP_2317_112_2_18_1_6_31_98,"BootStrap: Clearing Memory bank %d\n")
TRACE_MSG(BLUECALL_205_112_2_18_1_6_32_99,"[BLUE_Play] IN")
TRACE_MSG(BLUECALL_221_112_2_18_1_6_32_100,"[BLUE_Play] Have opened")
TRACE_MSG(BLUECALL_284_112_2_18_1_6_32_101,"[BLUE_Stop] IN")
TRACE_MSG(BLUECALL_329_112_2_18_1_6_32_102,"[BLUE_Stop] has closed")
TRACE_MSG(BLUECALL_339_112_2_18_1_6_32_103,"[BT]BLUE_Pause\r\n")
TRACE_MSG(BLUECALL_348_112_2_18_1_6_32_104,"[BT]BLUE_Resume\r\n")
TRACE_MSG(BLUECALL_368_112_2_18_1_6_32_105,"[BLUE_StartRecord] IN")
TRACE_MSG(BLUECALL_416_112_2_18_1_6_33_106,"[BLUE_StopRecord] IN")
TRACE_MSG(BLUECALL_487_112_2_18_1_6_33_107,"[BT]BLUE_WritePCMData:Data Full:0x%X, 0x%X\r\n")
TRACE_MSG(BLUECALL_512_112_2_18_1_6_33_108,"[BT]PCM:0x%X,0x%X,0x%X,0x%X\r\n")
TRACE_MSG(BLUECALL_530_112_2_18_1_6_33_109,"[BT]BLUE_WritePCMData:track_occupied is zero\r\n")
TRACE_MSG(BLUECALL_558_112_2_18_1_6_33_110,"[BT]Voice:0x%X,0x%X,0x%X,0x%X\r\n")
TRACE_MSG(BLUECALL_582_112_2_18_1_6_33_111,"[BT]BLUE_WriteVoiceData:data_count=0x%X\r\n")
TRACE_MSG(BLUECALL_643_112_2_18_1_6_33_112,":%d\r\n")
TRACE_MSG(BLUECALL_833_112_2_18_1_6_33_113,"[BLUE_Play] IN")
TRACE_MSG(BLUECALL_853_112_2_18_1_6_33_114,"[BLUE_Play] Have opened")
TRACE_MSG(BLUECALL_907_112_2_18_1_6_34_115,"[BLUE_Stop] IN")
TRACE_MSG(BLUECALL_949_112_2_18_1_6_34_116,"[BLUE_Stop] has closed")
TRACE_MSG(BLUECALL_959_112_2_18_1_6_34_117,"[BT]BLUE_Pause")
TRACE_MSG(BLUECALL_968_112_2_18_1_6_34_118,"[BT]BLUE_Resume")
TRACE_MSG(BLUECALL_985_112_2_18_1_6_34_119,"[BLUE_StartRecord] IN")
TRACE_MSG(BLUECALL_1025_112_2_18_1_6_34_120,"[BLUE_StopRecord] IN")
TRACE_MSG(BLUECALL_1064_112_2_18_1_6_34_121,"[BLUE_PauseRecord] IN")
TRACE_MSG(BLUECALL_1073_112_2_18_1_6_34_122,"[BLUE_ResumeRecord] IN")
TRACE_MSG(BLUECALL_1113_112_2_18_1_6_34_123,"[BT]BLUE_WritePCMData:Data Full:0x%X, 0x%X\r\n")
TRACE_MSG(BLUECALL_1138_112_2_18_1_6_34_124,"[BT]PCM:0x%X,0x%X,0x%X,0x%X\r\n")
TRACE_MSG(BLUECALL_1156_112_2_18_1_6_34_125,"[BT]BLUE_WritePCMData:track_occupied is zero\r\n")
TRACE_MSG(BLUECALL_1184_112_2_18_1_6_34_126,"[BT]Voice:0x%X,0x%X,0x%X,0x%X\r\n")
TRACE_MSG(BLUECALL_1207_112_2_18_1_6_34_127,"[BT]BLUE_WriteVoiceData:data_count=0x%X\r\n")
TRACE_MSG(BLUECALL_1269_112_2_18_1_6_34_128,":%d\r\n")
TRACE_MSG(BLUECALL_UART_322_112_2_18_1_6_35_129,"[BLUE_Play] IN")
TRACE_MSG(BLUECALL_UART_328_112_2_18_1_6_35_130,"[BLUE_Play] Have opened")
TRACE_MSG(BLUECALL_UART_372_112_2_18_1_6_35_131,"[BLUE_Stop] IN")
TRACE_MSG(BLUECALL_UART_403_112_2_18_1_6_35_132,"[BLUE_Stop] has closed")
TRACE_MSG(BLUECALL_UART_415_112_2_18_1_6_35_133,"[BT]BLUE_Pause\r\n")
TRACE_MSG(BLUECALL_UART_426_112_2_18_1_6_35_134,"[BT]BLUE_Resume\r\n")
TRACE_MSG(BLUECALL_UART_438_112_2_18_1_6_35_135,"[BLUE_StartRecord] IN")
TRACE_MSG(BLUECALL_UART_482_112_2_18_1_6_35_136,"[BLUE_StopRecord] IN")
TRACE_MSG(BLUECALL_UART_495_112_2_18_1_6_36_137,"[BLUE_PauseRecord] IN")
TRACE_MSG(BLUECALL_UART_508_112_2_18_1_6_36_138,"[BLUE_ResumeRecord] IN")
TRACE_MSG(OS_PIN_7_112_2_18_1_6_37_139,"[BT]OS_Close_BlueCore\r\n")
TRACE_MSG(OS_PIN_13_112_2_18_1_6_37_140,"[BT]OS_Start_BlueCore\r\n")
TRACE_MSG(OS_UART_DMA_136_112_2_18_1_6_37_141,"[BT]uart0_timer_handler:last=0x%X, addr=0x%X\r\n")
TRACE_MSG(OS_UART_DMA_176_112_2_18_1_6_37_142,"__FreeUartDmaChannel[port][tx], channel is empty\n")
TRACE_MSG(OS_UART_DMA_187_112_2_18_1_6_37_143,"__FreeUartDmaChannel[port][tx], channel is empty\n")
TRACE_MSG(OS_UART_DMA_253_112_2_18_1_6_37_144,"[BT]OSUartDrv_SetUartId:port:0x%X, tx_dma:0x%X, rx_dma:0x%X")
TRACE_MSG(OS_UART_DMA_302_112_2_18_1_6_37_145,"[BT] critical error error error buffer over")
TRACE_MSG(OS_UART_DMA_540_112_2_18_1_6_38_146,"[BT]EVENT_WRITE_COMPLETE:timer handle is empty\r\n")
TRACE_MSG(OS_UART_DMA_655_112_2_18_1_6_38_147,"[BT]EVENT_DATA_TO_READ:timer handle is empty\r\n")
TRACE_MSG(OS_UART_DMA_695_112_2_18_1_6_38_148,"[BT]OSSetUart_Configure baud_rate = 0x%X")
TRACE_MSG(OS_UART_DMA_710_112_2_18_1_6_38_149,"[BT]OSUartDrv_Configure:baudrate = %d\r\n")
TRACE_MSG(OS_UART_DMA_746_112_2_18_1_6_38_150,"[BT]OSUartDrv_Configure End g_uart_timer = %d\r\n")
TRACE_MSG(OS_UART_DMA_819_112_2_18_1_6_38_151,"[BT]OSUartDrv_Reset")
TRACE_MSG(OS_UART_DMA_843_112_2_18_1_6_39_152,"[BT]OSUartDrv_Stop")
TRACE_MSG(OS_UART_DMA_867_112_2_18_1_6_39_153,"[BT]OSUartDrv_Start")
TRACE_MSG(OS_UART_DMA_871_112_2_18_1_6_39_154,"[BT]OSUartDrv_Start: port= %d __GetUartDmaChannel Error\r\n")
TRACE_MSG(OS_UART_DMA_902_112_2_18_1_6_39_155,"[BT]UART_GenerateBreakSignal %d ms")
TRACE_MSG(PANIC_85_112_2_18_1_6_39_156,"[BT]sched_panic:%s, g_test_panicid=%d\r\n")
TRACE_MSG(OS_MSG_160_112_2_18_1_6_42_157,"[BT]OS_Do_Put_External_Task_Message = 0x%X\r\n")
TRACE_MSG(OS_PMALLOC_102_112_2_18_1_6_42_158,"BT_InitMem: Init successful!")
TRACE_MSG(OS_PMALLOC_107_112_2_18_1_6_42_159,"BT_InitMem: already init !")
TRACE_MSG(OS_PMALLOC_132_112_2_18_1_6_42_160,"BT_DeleteMem")
TRACE_MSG(OS_TASK_45_112_2_18_1_6_43_161,"[BT]OS_SetSchedThreadPriority:priority=0x%X\r\n")
TRACE_MSG(OS_TASK_51_112_2_18_1_6_43_162,"[BT]:OS_HandleStartSchedMsg\r\n")
TRACE_MSG(OS_TASK_64_112_2_18_1_6_43_163,"[BT]:OS_HandleStartBtMsg\r\n")
TRACE_MSG(OS_TASK_74_112_2_18_1_6_43_164,"[BT]:OS_HandleStopSchedMsg\r\n")
TRACE_MSG(OS_TASK_75_112_2_18_1_6_43_165,"[BT]BT_Os_Sched_Task:time=%d")
TRACE_MSG(OS_TASK_83_112_2_18_1_6_43_166,"[BT]:OS_HandleBtVisibleMsg\r\n")
TRACE_MSG(OS_TASK_100_112_2_18_1_6_43_167,"[BT]:OS_Handle_Sched_Task_Msg:Should not start scheduler at here\r\n")
TRACE_MSG(OS_TASK_122_112_2_18_1_6_43_168,"[BT]:OS_Handle_Sched_Task_Msg - OS_SCHED_TASK_MSG\r\n")
TRACE_MSG(OS_TASK_128_112_2_18_1_6_43_169,"[BT]:OS_Handle_Sched_Task_Msg - OS_DEACTIVATED_SERVICE_OK_MSG\r\n")
TRACE_MSG(OS_TASK_137_112_2_18_1_6_43_170,"[BT]:OS_Handle_Sched_Task_Msg - Begin to send message!\r\n")
TRACE_MSG(OS_TASK_144_112_2_18_1_6_43_171,"[BT]:OS_Handle_Sched_Task_Msg - Undefine message:0x%X\r\n")
TRACE_MSG(OS_TASK_154_112_2_18_1_6_43_172,"[BT]:BT_Os_Sched_Task\r\n")
TRACE_MSG(OS_TASK_157_112_2_18_1_6_43_173,"[BT]BT_Os_Sched_Task = 0x%x waitting...... \r\n")
TRACE_MSG(OS_TASK_164_112_2_18_1_6_43_174,"[BT]BT_Os_Sched_Task:Start Scheduler.... \r\n")
TRACE_MSG(OS_TASK_165_112_2_18_1_6_43_175,"[BT]BT_Os_Sched_Task:time=%d")
TRACE_MSG(OS_TASK_174_112_2_18_1_6_43_176,"[BT]BT_Os_Sched_Task: Invaild Signal = %s\r\n")
TRACE_MSG(OS_TASK_198_112_2_18_1_6_43_177,"[BT]OS_StartThread:priority=0x%X\r\n")
TRACE_MSG(OS_TASK_204_112_2_18_1_6_43_178,"[BT]OS_StartThread - s_bt_sched_thread_id = 0x%X\r\n")
TRACE_MSG(OS_TIMER_23_112_2_18_1_6_44_179,"[BT]OS_StartTimeOut:Create Timer handle=0x%08X\r\n")
TRACE_MSG(OS_TIMER_27_112_2_18_1_6_44_180,"[BT]OS_StartTimeOut: Active Timer handle=0x%08X\r\n")
TRACE_MSG(OS_TIMER_37_112_2_18_1_6_44_181,"[BT]OS_IsTimeOutStart:s_bt_timeout_handle=0x%08X\r\n")
TRACE_MSG(OS_TIMER_49_112_2_18_1_6_44_182,"[BT]OS_DeleteTimeOut:Timer handle=0x%08X\r\n")
TRACE_MSG(OS_TIMER_60_112_2_18_1_6_44_183,"[BT]OS_StopTimeOut:s_bt_timeout_handle=0x%08X\r\n")
TRACE_MSG(TASK_22_112_2_18_1_6_46_184,"ERROR NUMBER_OF_TASKS ID=0X%x\n\t")
TRACE_MSG(A2DP_API_59_112_2_18_1_6_51_185,"[A2DPAdapt_SendMessageToApp] msg_id = 0x%x, isResume = %d, isSuspend = %d,body=%p,size=%d")
TRACE_MSG(A2DP_API_111_112_2_18_1_6_51_186,"[DRV_BT]<WARNING> A2DP_InitDevice(): ERROR sample_rate = %d")
TRACE_MSG(A2DP_API_155_112_2_18_1_6_51_187,"A2DP_OpenDevice: uiSampleRate = %d")
TRACE_MSG(A2DP_API_171_112_2_18_1_6_51_188,"A2DP_CloseDevice")
TRACE_MSG(A2DP_API_190_112_2_18_1_6_51_189,"A2DP_PauseDevice")
TRACE_MSG(A2DP_API_205_112_2_18_1_6_51_190,"A2DP_ResumeDevice")
TRACE_MSG(A2DP_API_224_112_2_18_1_6_51_191,"A2DP_StartStreamData")
TRACE_MSG(A2DP_AS_109_112_2_18_1_6_52_192,"_A2DP_EnableDigitalGain add express:0x%x to codec:0x%x.")
TRACE_MSG(A2DP_AS_123_112_2_18_1_6_52_193,"_A2DP_EnableDigitalGain delete express:0x%x from codec:0x%x.")
TRACE_MSG(A2DP_AS_137_112_2_18_1_6_52_194,"A2DP_Play uiSamleRate = %d")
TRACE_MSG(A2DP_AS_160_112_2_18_1_6_52_195,"A2DP_Stop")
TRACE_MSG(A2DP_AS_173_112_2_18_1_6_52_196,"A2DP_Pause")
TRACE_MSG(A2DP_AS_186_112_2_18_1_6_52_197,"A2DP_Resume uiSampleRate = %d")
TRACE_MSG(A2DP_AS_209_112_2_18_1_6_52_198,"A2DP_SetSampleRate samle_rate = %d")
TRACE_MSG(A2DP_AS_234_112_2_18_1_6_52_199,"a2dp_as.c,[A2DP_SetDeviceMode] set aud proc explugger! hAUDPROCExp:0x%x")
TRACE_MSG(A2DP_AS_251_112_2_18_1_6_52_200,"a2dp_as.c,[A2DP_SetDeviceMode] set aud enha explugger! hAUDENHAExp:0x%x")
TRACE_MSG(A2DP_AS_273_112_2_18_1_6_52_201,"A2DP_SetVolumeType vol:%d, appType:%d.")
TRACE_MSG(A2DP_AS_278_112_2_18_1_6_52_202,"A2DP_SetVolumeType volume level is too bit(%d), will be adjusted to be:%d.")
TRACE_MSG(A2DP_AS_285_112_2_18_1_6_52_203,"A2DP_SetVolumeType filed to get modeName:%d.")
TRACE_MSG(A2DP_AS_291_112_2_18_1_6_52_204,"A2DP_SetVolumeType, alloc fail, size: %d")
TRACE_MSG(A2DP_AS_298_112_2_18_1_6_52_205,"A2DP_SetVolumeType failed to get mode param, mode:%d, modeName:%s.")
TRACE_MSG(A2DP_AS_304_112_2_18_1_6_52_206,"A2DP_SetVolumeType uiVol=%d, mode:%d, appType:%d, modeName:%s.")
TRACE_MSG(A2DP_AS_317_112_2_18_1_6_52_207,"A2DP_SetVolumeType gain:0x%4x, dg_level= 0x%4x")
TRACE_MSG(A2DP_AS_342_112_2_18_1_6_52_208,"A2DP_SetDownlinkMute, mute:%d, vol:%d, appType:%d.")
TRACE_MSG(A2DP_AS_399_112_2_18_1_6_52_209,"peter: pcm_data_count is %d")
TRACE_MSG(A2DP_AS_447_112_2_18_1_6_53_210,"peter:g_sbc_src_samplerate is %d")
TRACE_MSG(PF_A2DP_283_112_2_18_1_6_54_211,"[BT][startDeactivate] IN")
TRACE_MSG(PF_A2DP_292_112_2_18_1_6_54_212,"[BT][A2DP_ServiceClose] IN")
TRACE_MSG(PF_A2DP_293_112_2_18_1_6_54_213,"[BT][A2DP_ServiceClose] status = %d")
TRACE_MSG(PF_A2DP_294_112_2_18_1_6_54_214,"[BT][A2DP_ServiceClose] state = %d")
TRACE_MSG(PF_A2DP_322_112_2_18_1_6_54_215,"[A2DP_TimeOutHandle] Time Out ,Acitive error")
TRACE_MSG(PF_A2DP_335_112_2_18_1_6_54_216,"[startConnecting] IN")
TRACE_MSG(PF_A2DP_336_112_2_18_1_6_54_217,"[startConnecting] isActivated = %d")
TRACE_MSG(PF_A2DP_337_112_2_18_1_6_54_218,"[startConnecting] currentConnection = %d")
TRACE_MSG(PF_A2DP_341_112_2_18_1_6_54_219,"[startConnecting] remoteDevAdddr = %04x:%02x:%06x")
TRACE_MSG(PF_A2DP_362_112_2_18_1_6_54_220,"[startDisconnect] currentConnection = %d")
TRACE_MSG(PF_A2DP_364_112_2_18_1_6_54_221,"[startDisconnect] connectionId = %d")
TRACE_MSG(PF_A2DP_371_112_2_18_1_6_54_222,"[startStream] sample_freq = %d")
TRACE_MSG(PF_A2DP_372_112_2_18_1_6_54_223,"[startStream] state = %d")
TRACE_MSG(PF_A2DP_373_112_2_18_1_6_54_224,"[startStream] instData->sample_freq = %d")
TRACE_MSG(PF_A2DP_374_112_2_18_1_6_54_225,"[startStream] ActiveConfig.sample_freq = %d")
TRACE_MSG(PF_A2DP_375_112_2_18_1_6_54_226,"[startStream] currentConnection = %d")
TRACE_MSG(PF_A2DP_377_112_2_18_1_6_54_227,"[startStream] connectionId = %d")
TRACE_MSG(PF_A2DP_378_112_2_18_1_6_54_228,"[startStream] role = %d")
TRACE_MSG(PF_A2DP_400_112_2_18_1_6_54_229,"[stopStream] state = %d")
TRACE_MSG(PF_A2DP_401_112_2_18_1_6_54_230,"[stopStream] currentConnection = %d")
TRACE_MSG(PF_A2DP_403_112_2_18_1_6_54_231,"[stopStream] streamHandle = %d")
TRACE_MSG(PF_A2DP_411_112_2_18_1_6_54_232,"[suspendStream] state = %d")
TRACE_MSG(PF_A2DP_412_112_2_18_1_6_54_233,"[suspendStream] currentConnection = %d")
TRACE_MSG(PF_A2DP_414_112_2_18_1_6_54_234,"[suspendStream] streamHandle = %d")
TRACE_MSG(PF_A2DP_425_112_2_18_1_6_54_235,"[startReconfigure] original sample freq = %d")
TRACE_MSG(PF_A2DP_431_112_2_18_1_6_54_236,"[startReconfigure] buildServCap failed")
TRACE_MSG(PF_A2DP_442_112_2_18_1_6_54_237,"[startReconfigure] target sample freq = %d")
TRACE_MSG(PF_A2DP_447_112_2_18_1_6_54_238,"[startReconfigure] no need to reconfigure, start stream")
TRACE_MSG(PF_A2DP_450_112_2_18_1_6_54_239,"[startReconfigure] do reconfigure")
TRACE_MSG(PF_A2DP_460_112_2_18_1_6_54_240,"[startAbort] state = %d")
TRACE_MSG(PF_A2DP_461_112_2_18_1_6_54_241,"[startAbort] currentConnection = %d")
TRACE_MSG(PF_A2DP_463_112_2_18_1_6_54_242,"[startAbort] streamHandle = %d")
TRACE_MSG(PF_A2DP_514_112_2_18_1_6_54_243,"[buildServCap]:support different sample:servcap:0x%x,sample:0x%x")
TRACE_MSG(PF_A2DP_520_112_2_18_1_6_54_244,"[buildServCap]:support different sample:servcap:0x%x,sample:0x%x")
TRACE_MSG(PF_A2DP_554_112_2_18_1_6_54_245,"[buildServCap] platform do not support the sample_freq")
TRACE_MSG(PF_A2DP_561_112_2_18_1_6_54_246,"[buildServCap] sample_freq =%d,sbcCap[0]=%d")
TRACE_MSG(PF_A2DP_575_112_2_18_1_6_54_247,"[buildServCap] Samling Frequency = 0x%02x")
TRACE_MSG(PF_A2DP_617_112_2_18_1_6_54_248,"[buildServCap] candi = %d")
TRACE_MSG(PF_A2DP_634_112_2_18_1_6_54_249,"[buildServCap] Channel Mode = 0x%02x")
TRACE_MSG(PF_A2DP_647_112_2_18_1_6_54_250,"[buildServCap] Block Length = 0x%02x")
TRACE_MSG(PF_A2DP_660_112_2_18_1_6_54_251,"[buildServCap] Subbands = 0x%02x")
TRACE_MSG(PF_A2DP_673_112_2_18_1_6_54_252,"[buildServCap] Allocation Method = 0x%02x")
TRACE_MSG(PF_A2DP_689_112_2_18_1_6_54_253,"[buildServCap] *(servCap + 6) = 0x%02x")
TRACE_MSG(PF_A2DP_690_112_2_18_1_6_54_254,"[buildServCap] sbcCap[2] = 0x%02x")
TRACE_MSG(PF_A2DP_692_112_2_18_1_6_54_255,"[buildServCap] *(servCap + 7) = 0x%02x")
TRACE_MSG(PF_A2DP_693_112_2_18_1_6_54_256,"[buildServCap] sbcCap[3] = 0x%02x")
TRACE_MSG(PF_A2DP_827_112_2_18_1_6_55_257,"dev_name:0x%04x\n")
TRACE_MSG(PF_A2DP_832_112_2_18_1_6_55_258,"It's a Jabra hs.\n")
TRACE_MSG(PF_A2DP_853_112_2_18_1_6_55_259,"[transmitAudioPacket] no connected_streaming connection")
TRACE_MSG(PF_A2DP_867_112_2_18_1_6_55_260,"[transmitAudioPacket] (%d) no packet")
TRACE_MSG(PF_A2DP_877_112_2_18_1_6_55_261,"[transmitAudioPacket] (%d) data = 0x%x, dataLen = %d, frames = %d")
TRACE_MSG(PF_A2DP_923_112_2_18_1_6_55_262,"[audioSamplesHandler] IN (tick_diff = %d)")
TRACE_MSG(PF_A2DP_927_112_2_18_1_6_55_263,"[audioSamplesHandler] frames = %d")
TRACE_MSG(PF_A2DP_947_112_2_18_1_6_55_264,"[audioSamplesHandler] time_curr = %d, refStartTime = %d")
TRACE_MSG(PF_A2DP_952_112_2_18_1_6_55_265,"[audioSamplesHandler] continue, time_curr = %d, packetRefTime = %d")
TRACE_MSG(PF_A2DP_964_112_2_18_1_6_55_266,"[audioSamplesHandler] time_curr = %d, packetRefTime = %d, wait_time = %d")
TRACE_MSG(PF_A2DP_1000_112_2_18_1_6_55_267,"[handleActivateCfm] result = %d")
TRACE_MSG(PF_A2DP_1011_112_2_18_1_6_55_268,"[handleDeactivateCfm] result = %d")
TRACE_MSG(PF_A2DP_1058_112_2_18_1_6_55_269,"[handleConnectInd] connectionId = %d")
TRACE_MSG(PF_A2DP_1060_112_2_18_1_6_55_270,"[handleConnectInd] deviceAddr = %04x:%02x:%06x")
TRACE_MSG(PF_A2DP_1092_112_2_18_1_6_55_271,"[handleConnectCfm] result = %d")
TRACE_MSG(PF_A2DP_1101_112_2_18_1_6_55_272,"[handleConnectCfm] connectionId = %d")
TRACE_MSG(PF_A2DP_1103_112_2_18_1_6_55_273,"[handleConnectCfm] deviceAddr = %04x:%02x:%06x")
TRACE_MSG(PF_A2DP_1104_112_2_18_1_6_55_274,"[handleConnectCfm] maxFrameSize = %d")
TRACE_MSG(PF_A2DP_1114_112_2_18_1_6_55_275,"instData->currentConnection].inUse = TRUE")
TRACE_MSG(PF_A2DP_1152_112_2_18_1_6_55_276,"[handleConnectCfm] it should be an open cfm")
TRACE_MSG(PF_A2DP_1183_112_2_18_1_6_55_277,"[handleDisconnectInd] connectionId = %d, result = %d")
TRACE_MSG(PF_A2DP_1188_112_2_18_1_6_55_278,"[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore")
TRACE_MSG(PF_A2DP_1243_112_2_18_1_6_55_279,"[handleConnectInd] deviceAddr = %02x:%02x:%02x:%02x:%02x:%02x")
TRACE_MSG(PF_A2DP_1265_112_2_18_1_6_56_280,"[handleDiscoverCfm] result = %d")
TRACE_MSG(PF_A2DP_1266_112_2_18_1_6_56_281,"[handleDiscoverCfm] connectionId = %d")
TRACE_MSG(PF_A2DP_1267_112_2_18_1_6_56_282,"[handleDiscoverCfm] listLength = %d")
TRACE_MSG(PF_A2DP_1268_112_2_18_1_6_56_283,"[handleDiscoverCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_1273_112_2_18_1_6_56_284,"[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore")
TRACE_MSG(PF_A2DP_1293_112_2_18_1_6_56_285,"[handleDiscoverCfm] SEID %d")
TRACE_MSG(PF_A2DP_1294_112_2_18_1_6_56_286,"acpSeid = %d")
TRACE_MSG(PF_A2DP_1295_112_2_18_1_6_56_287,"inUse = %d")
TRACE_MSG(PF_A2DP_1296_112_2_18_1_6_56_288,"mediaType = %d")
TRACE_MSG(PF_A2DP_1297_112_2_18_1_6_56_289,"sepType = %d")
TRACE_MSG(PF_A2DP_1319_112_2_18_1_6_56_290,"The remote device does not have available audio end-points\n")
TRACE_MSG(PF_A2DP_1326_112_2_18_1_6_56_291,"Stream discovery rejected\n")
TRACE_MSG(PF_A2DP_1339_112_2_18_1_6_56_292,"[handleDiscoverInd]")
TRACE_MSG(PF_A2DP_1366_112_2_18_1_6_56_293,"[handleGetCapabilitiesCfm] result = %d")
TRACE_MSG(PF_A2DP_1367_112_2_18_1_6_56_294,"[handleGetCapabilitiesCfm] connectionId = %d")
TRACE_MSG(PF_A2DP_1368_112_2_18_1_6_56_295,"[handleGetCapabilitiesCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_1369_112_2_18_1_6_56_296,"[handleGetCapabilitiesCfm] servCapLen = %d")
TRACE_MSG(PF_A2DP_1373_112_2_18_1_6_56_297,"Get capabilities rejected, error code 0x%X \n")
TRACE_MSG(PF_A2DP_1388_112_2_18_1_6_56_298,"[handleGetCapabilitiesCfm] servCap is NULL")
TRACE_MSG(PF_A2DP_1393_112_2_18_1_6_56_299,"[handleGetCapabilitiesCfm] buildServCap failed")
TRACE_MSG(PF_A2DP_1435_112_2_18_1_6_56_300,"Get capabilities response does not indicate SBC audio support\n")
TRACE_MSG(PF_A2DP_1451_112_2_18_1_6_56_301,"[handleGetCapabilitiesInd] connectionId = %d")
TRACE_MSG(PF_A2DP_1452_112_2_18_1_6_56_302,"[handleGetCapabilitiesInd] tLabel = %d")
TRACE_MSG(PF_A2DP_1453_112_2_18_1_6_56_303,"[handleGetCapabilitiesInd] acpSeid = %d")
TRACE_MSG(PF_A2DP_1458_112_2_18_1_6_56_304,"[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore")
TRACE_MSG(PF_A2DP_1463_112_2_18_1_6_56_305,"[handleGetCapabilitiesInd] localSeidIndex = %d")
TRACE_MSG(PF_A2DP_1465_112_2_18_1_6_56_306,"address lap0x%x,uap0x%x,nap0x%x")
TRACE_MSG(PF_A2DP_1495_112_2_18_1_6_56_307,"[handleGetCapabilitiesInd] Reject")
TRACE_MSG(PF_A2DP_1505_112_2_18_1_6_56_308,"[handleSetConfigurationCfm] connectionId = %d")
TRACE_MSG(PF_A2DP_1506_112_2_18_1_6_56_309,"[handleSetConfigurationCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_1507_112_2_18_1_6_56_310,"[handleSetConfigurationCfm] result = %d")
TRACE_MSG(PF_A2DP_1508_112_2_18_1_6_56_311,"[handleSetConfigurationCfm] shandle = %d")
TRACE_MSG(PF_A2DP_1509_112_2_18_1_6_56_312,"[handleSetConfigurationCfm] servCategory = %d")
TRACE_MSG(PF_A2DP_1520_112_2_18_1_6_56_313,"[handleSetConfigurationCfm] AvOpenReqSend")
TRACE_MSG(PF_A2DP_1524_112_2_18_1_6_56_314,"Unable to configure stream end-point\n")
TRACE_MSG(PF_A2DP_1531_112_2_18_1_6_56_315,"Set configuration rejected, error code 0x%X\n")
TRACE_MSG(PF_A2DP_1547_112_2_18_1_6_56_316,"[handleSetConfigurationInd] connectionId = %d")
TRACE_MSG(PF_A2DP_1548_112_2_18_1_6_56_317,"[handleSetConfigurationInd] tLabel = %d")
TRACE_MSG(PF_A2DP_1549_112_2_18_1_6_56_318,"[handleSetConfigurationInd] shandle = %d")
TRACE_MSG(PF_A2DP_1550_112_2_18_1_6_56_319,"[handleSetConfigurationInd] acpSeid = %d")
TRACE_MSG(PF_A2DP_1551_112_2_18_1_6_56_320,"[handleSetConfigurationInd] intSeid = %d")
TRACE_MSG(PF_A2DP_1552_112_2_18_1_6_56_321,"[handleSetConfigurationInd] servCapLen %d")
TRACE_MSG(PF_A2DP_1557_112_2_18_1_6_56_322,"[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore")
TRACE_MSG(PF_A2DP_1661_112_2_18_1_6_56_323,"[handleOpenCfm] shandle = %d")
TRACE_MSG(PF_A2DP_1662_112_2_18_1_6_56_324,"[handleOpenCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_1663_112_2_18_1_6_56_325,"[handleOpenCfm] result = %d")
TRACE_MSG(PF_A2DP_1672_112_2_18_1_6_56_326,"Open stream rejected, error code 0x%X\n")
TRACE_MSG(PF_A2DP_1684_112_2_18_1_6_56_327,"[handleOpenInd] shandle = %d")
TRACE_MSG(PF_A2DP_1685_112_2_18_1_6_56_328,"[handleOpenInd] tLabel = %d")
TRACE_MSG(PF_A2DP_1706_112_2_18_1_6_56_329,"[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore")
TRACE_MSG(PF_A2DP_1715_112_2_18_1_6_56_330,"[handleStartCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_1716_112_2_18_1_6_56_331,"[handleStartCfm] result = %d")
TRACE_MSG(PF_A2DP_1717_112_2_18_1_6_56_332,"[handleStartCfm] reject_shandle = %d")
TRACE_MSG(PF_A2DP_1725_112_2_18_1_6_56_333,"Unable to configure local stream end-point\n")
TRACE_MSG(PF_A2DP_1737_112_2_18_1_6_56_334,"Start stream rejected, error code 0x%x\n")
TRACE_MSG(PF_A2DP_1751_112_2_18_1_6_56_335,"[handleStartInd] tLabel = %d")
TRACE_MSG(PF_A2DP_1752_112_2_18_1_6_56_336,"[handleStartInd] listLength = %d")
TRACE_MSG(PF_A2DP_1753_112_2_18_1_6_56_337,"[handleStartInd] first_shandle = %d")
TRACE_MSG(PF_A2DP_1758_112_2_18_1_6_57_338,"[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore")
TRACE_MSG(PF_A2DP_1775_112_2_18_1_6_57_339,"Unable to configure local stream end-point\n")
TRACE_MSG(PF_A2DP_1824_112_2_18_1_6_57_340,"[handleCloseCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_1825_112_2_18_1_6_57_341,"[handleCloseCfm] result = %d")
TRACE_MSG(PF_A2DP_1842_112_2_18_1_6_57_342,"Close stream rejected, error code 0x%x\n")
TRACE_MSG(PF_A2DP_1854_112_2_18_1_6_57_343,"[handleCloseInd] shandle = %d")
TRACE_MSG(PF_A2DP_1855_112_2_18_1_6_57_344,"[handleCloseInd] tLabel = %d")
TRACE_MSG(PF_A2DP_1886_112_2_18_1_6_57_345,"[handleSuspendCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_1887_112_2_18_1_6_57_346,"[handleSuspendCfm] result = %d")
TRACE_MSG(PF_A2DP_1888_112_2_18_1_6_57_347,"[handleSuspendCfm] reject_shandle = %d")
TRACE_MSG(PF_A2DP_1906_112_2_18_1_6_57_348,"[DRV_BT]<ASSERT_ERROR>: handleSuspendCfm: invalid state")
TRACE_MSG(PF_A2DP_1912_112_2_18_1_6_57_349,"Suspend stream rejected, error code 0x%X\n")
TRACE_MSG(PF_A2DP_1925_112_2_18_1_6_57_350,"[handleSuspendInd] tLabel = %d")
TRACE_MSG(PF_A2DP_1926_112_2_18_1_6_57_351,"[handleSuspendInd] listLength = %d")
TRACE_MSG(PF_A2DP_1927_112_2_18_1_6_57_352,"[handleSuspendInd] first_shandle = %d")
TRACE_MSG(PF_A2DP_1932_112_2_18_1_6_57_353,"[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore")
TRACE_MSG(PF_A2DP_1980_112_2_18_1_6_57_354,"[handleQosInd] shandle = %d")
TRACE_MSG(PF_A2DP_1981_112_2_18_1_6_57_355,"[handleQosInd] bufferStatus = %d")
TRACE_MSG(PF_A2DP_1990_112_2_18_1_6_57_356,"[handleStreamMtuSizeInd] shandle = %d")
TRACE_MSG(PF_A2DP_1991_112_2_18_1_6_57_357,"[handleStreamMtuSizeInd] remoteMtuSize = %d")
TRACE_MSG(PF_A2DP_2017_112_2_18_1_6_57_358,"[handleReconfigureCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_2018_112_2_18_1_6_57_359,"[handleReconfigureCfm] result = %d")
TRACE_MSG(PF_A2DP_2019_112_2_18_1_6_57_360,"[handleReconfigureCfm] servCategory = %d")
TRACE_MSG(PF_A2DP_2037_112_2_18_1_6_57_361,"[handleReconfigureCfm] SBC_prepareSbc failed")
TRACE_MSG(PF_A2DP_2057_112_2_18_1_6_57_362,"[handleReconfigureInd] shandle = %d")
TRACE_MSG(PF_A2DP_2058_112_2_18_1_6_57_363,"[handleReconfigureInd] tLabel = %d")
TRACE_MSG(PF_A2DP_2059_112_2_18_1_6_57_364,"[handleReconfigureInd] servCapLen = %D")
TRACE_MSG(PF_A2DP_2072_112_2_18_1_6_57_365,"index = %d")
TRACE_MSG(PF_A2DP_2073_112_2_18_1_6_57_366,"Service Category = 0x%x")
TRACE_MSG(PF_A2DP_2074_112_2_18_1_6_57_367,"Media Type = 0x%x")
TRACE_MSG(PF_A2DP_2075_112_2_18_1_6_57_368,"Media Code Type = 0x%x")
TRACE_MSG(PF_A2DP_2112_112_2_18_1_6_57_369,"Sep not in use")
TRACE_MSG(PF_A2DP_2131_112_2_18_1_6_57_370,"PrepareSbc fail")
TRACE_MSG(PF_A2DP_2139_112_2_18_1_6_57_371,"Bad acp seid")
TRACE_MSG(PF_A2DP_2157_112_2_18_1_6_57_372,"[handleGetConfigurationCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_2158_112_2_18_1_6_57_373,"[handleGetConfigurationCfm] result = %d")
TRACE_MSG(PF_A2DP_2159_112_2_18_1_6_57_374,"[handleGetConfigurationCfm] servCapLen = %d")
TRACE_MSG(PF_A2DP_2163_112_2_18_1_6_57_375,"[DRV_BT]<ASSERT_ERROR>: handleGetConfigurationCfm")
TRACE_MSG(PF_A2DP_2173_112_2_18_1_6_57_376,"[handleGetConfigurationInd] shandle = %d")
TRACE_MSG(PF_A2DP_2174_112_2_18_1_6_57_377,"[handleGetConfigurationInd] tLabel = %d")
TRACE_MSG(PF_A2DP_2179_112_2_18_1_6_57_378,"[DRV_BT]<ASSERT_ERROR> Invalid conIndex, ignore")
TRACE_MSG(PF_A2DP_2218_112_2_18_1_6_57_379,"Bad acp seid")
TRACE_MSG(PF_A2DP_2228_112_2_18_1_6_57_380,"[handleAbortCfm] shandle = %d")
TRACE_MSG(PF_A2DP_2229_112_2_18_1_6_57_381,"[handleAbortCfm] tLabel = %d")
TRACE_MSG(PF_A2DP_2267_112_2_18_1_6_58_382,"[handleAbortInd] shandle = %d")
TRACE_MSG(PF_A2DP_2268_112_2_18_1_6_58_383,"[handleAbortInd] tLabel = %d")
TRACE_MSG(PF_A2DP_2296_112_2_18_1_6_58_384,"[A2DP_Init] phandle = 0x%x")
TRACE_MSG(PF_A2DP_2320_112_2_18_1_6_58_385,"[A2DP_Deinit] state = %s(%d), isAudioOpen = %d, audio_timer = %d")
TRACE_MSG(PF_A2DP_2357_112_2_18_1_6_58_386,"[A2DP_setSbcCapabilites] sampleFreq = %d, quality = %d, transSamplerate = %d")
TRACE_MSG(PF_A2DP_2379_112_2_18_1_6_58_387,"[DRV_BT]<ASSERT_ERROR>: A2DP_setSbcCapabilites")
TRACE_MSG(PF_A2DP_2409_112_2_18_1_6_58_388,"[A2DP_startConnecting] addr = %04x:%02x:%06x")
TRACE_MSG(PF_A2DP_2411_112_2_18_1_6_58_389,"[A2DP_startConnecting] state = %s (%d)")
TRACE_MSG(PF_A2DP_2438_112_2_18_1_6_58_390,"[A2DP_startConnecting]addr =%x:%x:%x")
TRACE_MSG(PF_A2DP_2459_112_2_18_1_6_58_391,"[A2DP_startDisconnect] state = %s (%d)")
TRACE_MSG(PF_A2DP_2474_112_2_18_1_6_58_392,"[A2DP_startStream] sample_freq = %d")
TRACE_MSG(PF_A2DP_2476_112_2_18_1_6_58_393,"[A2DP_startStream] state = %s (%d), role = %d")
TRACE_MSG(PF_A2DP_2517_112_2_18_1_6_58_394,"[A2DP_stopStream] state = %s (%d)")
TRACE_MSG(PF_A2DP_2536_112_2_18_1_6_58_395,"[A2DP_suspendStream] state = %s (%d)")
TRACE_MSG(PF_A2DP_2572_112_2_18_1_6_58_396,"[A2DP_startAbort] state = %s (%d)")
TRACE_MSG(PF_A2DP_2626_112_2_18_1_6_58_397,"[A2DP_handlePrim] state = %s (%d)")
TRACE_MSG(PF_A2DP_2627_112_2_18_1_6_58_398,"[A2DP_handlePrim] primType = 0x%x --")
TRACE_MSG(PF_A2DP_2631_112_2_18_1_6_58_399,"AV_ACTIVATE_CFM")
TRACE_MSG(PF_A2DP_2637_112_2_18_1_6_58_400,"AV_DEACTIVATE_CFM")
TRACE_MSG(PF_A2DP_2642_112_2_18_1_6_58_401,"AV_CONNECT_IND")
TRACE_MSG(PF_A2DP_2647_112_2_18_1_6_58_402,"AV_CONNECT_CFM")
TRACE_MSG(PF_A2DP_2652_112_2_18_1_6_58_403,"AV_DISCONNECT_IND")
TRACE_MSG(PF_A2DP_2657_112_2_18_1_6_58_404,"AV_DISCOVER_CFM")
TRACE_MSG(PF_A2DP_2662_112_2_18_1_6_58_405,"AV_DISCOVER_IND")
TRACE_MSG(PF_A2DP_2667_112_2_18_1_6_58_406,"AV_GET_CAPABILITIES_CFM")
TRACE_MSG(PF_A2DP_2672_112_2_18_1_6_58_407,"AV_GET_CAPABILITIES_IND")
TRACE_MSG(PF_A2DP_2677_112_2_18_1_6_58_408,"AV_SET_CONFIGURATION_CFM")
TRACE_MSG(PF_A2DP_2682_112_2_18_1_6_58_409,"AV_SET_CONFIGURATION_IND")
TRACE_MSG(PF_A2DP_2687_112_2_18_1_6_58_410,"AV_OPEN_CFM")
TRACE_MSG(PF_A2DP_2692_112_2_18_1_6_58_411,"AV_OPEN_IND")
TRACE_MSG(PF_A2DP_2697_112_2_18_1_6_58_412,"AV_START_CFM")
TRACE_MSG(PF_A2DP_2702_112_2_18_1_6_58_413,"AV_START_IND")
TRACE_MSG(PF_A2DP_2707_112_2_18_1_6_58_414,"AV_CLOSE_CFM")
TRACE_MSG(PF_A2DP_2712_112_2_18_1_6_58_415,"AV_CLOSE_IND")
TRACE_MSG(PF_A2DP_2717_112_2_18_1_6_58_416,"AV_SUSPEND_CFM")
TRACE_MSG(PF_A2DP_2722_112_2_18_1_6_58_417,"AV_SUSPEND_IND")
TRACE_MSG(PF_A2DP_2727_112_2_18_1_6_58_418,"AV_QOS_IND")
TRACE_MSG(PF_A2DP_2732_112_2_18_1_6_58_419,"AV_STREAM_MTU_SIZE_IND")
TRACE_MSG(PF_A2DP_2737_112_2_18_1_6_58_420,"AV_RECONFIGURE_CFM")
TRACE_MSG(PF_A2DP_2742_112_2_18_1_6_58_421,"AV_RECONFIGURE_IND")
TRACE_MSG(PF_A2DP_2747_112_2_18_1_6_58_422,"AV_GET_CONFIGURATION_CFM")
TRACE_MSG(PF_A2DP_2752_112_2_18_1_6_58_423,"AV_GET_CONFIGURATION_IND")
TRACE_MSG(PF_A2DP_2757_112_2_18_1_6_58_424,"AV_ABORT_CFM")
TRACE_MSG(PF_A2DP_2762_112_2_18_1_6_58_425,"AV_ABORT_IND")
TRACE_MSG(PF_A2DP_2767_112_2_18_1_6_58_426,"AV_REGISTER_STREAM_HANDLE_CFM")
TRACE_MSG(PF_A2DP_2771_112_2_18_1_6_58_427,"[DRV_BT]<ASSERT_ERROR> [A2DP_handlePrim] Unknown primType = 0x%x")
TRACE_MSG(PF_A2DP_2794_112_2_18_1_6_59_428,"[AVRCP_handlePrim] primType = 0x%x --")
TRACE_MSG(PF_A2DP_2799_112_2_18_1_6_59_429,"AVRCP_DISCONNECT_IND")
TRACE_MSG(PF_A2DP_2809_112_2_18_1_6_59_430,"AVRCP_ACTIVATE_CFM")
TRACE_MSG(PF_A2DP_2815_112_2_18_1_6_59_431,"[DRV_BT]<ASSERT_ERROR> AVRCP_CONNECT_CFM")
TRACE_MSG(PF_A2DP_2826_112_2_18_1_6_59_432,"AVRCP_CONNECT_IND")
TRACE_MSG(PF_A2DP_2828_112_2_18_1_6_59_433,"deviceAddr = %04x:%02x:%06x")
TRACE_MSG(PF_A2DP_2843_112_2_18_1_6_59_434,"AVRCP_PASS_THROUGH_COMMAND_IND")
TRACE_MSG(PF_A2DP_2844_112_2_18_1_6_59_435,"tLabel = %d")
TRACE_MSG(PF_A2DP_2845_112_2_18_1_6_59_436,"invalidProfile = %d")
TRACE_MSG(PF_A2DP_2846_112_2_18_1_6_59_437,"profileIdentifier = 0x%x")
TRACE_MSG(PF_A2DP_2847_112_2_18_1_6_59_438,"cType = %d")
TRACE_MSG(PF_A2DP_2848_112_2_18_1_6_59_439,"subunit_type = %d")
TRACE_MSG(PF_A2DP_2849_112_2_18_1_6_59_440,"subunitId = %d")
TRACE_MSG(PF_A2DP_2850_112_2_18_1_6_59_441,"dataLen = %d")
TRACE_MSG(PF_A2DP_2862_112_2_18_1_6_59_442,"VOLUP_PASS_THROUGH")
TRACE_MSG(PF_A2DP_2866_112_2_18_1_6_59_443,"VOLUP OFF")
TRACE_MSG(PF_A2DP_2872_112_2_18_1_6_59_444,"VOLUP ON")
TRACE_MSG(PF_A2DP_2879_112_2_18_1_6_59_445,"VOLDOWN_PASS_THROUGH")
TRACE_MSG(PF_A2DP_2883_112_2_18_1_6_59_446,"VOLDOWN OFF")
TRACE_MSG(PF_A2DP_2889_112_2_18_1_6_59_447,"VOLDOWN ON")
TRACE_MSG(PF_A2DP_2897_112_2_18_1_6_59_448,"PLAY_PASS_THROUGH")
TRACE_MSG(PF_A2DP_2901_112_2_18_1_6_59_449,"PLAY OFF")
TRACE_MSG(PF_A2DP_2907_112_2_18_1_6_59_450,"PLAY ON")
TRACE_MSG(PF_A2DP_2914_112_2_18_1_6_59_451,"STOP_PASS_THROUGH")
TRACE_MSG(PF_A2DP_2918_112_2_18_1_6_59_452,"STOP OFF")
TRACE_MSG(PF_A2DP_2924_112_2_18_1_6_59_453,"STOP ON")
TRACE_MSG(PF_A2DP_2931_112_2_18_1_6_59_454,"PAUSE_PASS_THROUGH")
TRACE_MSG(PF_A2DP_2935_112_2_18_1_6_59_455,"PAUSE OFF")
TRACE_MSG(PF_A2DP_2941_112_2_18_1_6_59_456,"PAUSE ON")
TRACE_MSG(PF_A2DP_2948_112_2_18_1_6_59_457,"FORWARD_PASS_THROUGH")
TRACE_MSG(PF_A2DP_2952_112_2_18_1_6_59_458,"FORWARD OFF\n")
TRACE_MSG(PF_A2DP_2958_112_2_18_1_6_59_459,"FORWARD ON\n")
TRACE_MSG(PF_A2DP_2966_112_2_18_1_6_59_460,"BACKWARD_PASS_THROUGH")
TRACE_MSG(PF_A2DP_2970_112_2_18_1_6_59_461,"BACKWARD OFF\n")
TRACE_MSG(PF_A2DP_2976_112_2_18_1_6_59_462,"BACKWARD ON\n")
TRACE_MSG(PF_A2DP_2983_112_2_18_1_6_59_463,"unknown key command - 0x%x\n")
TRACE_MSG(PF_A2DP_3028_112_2_18_1_6_59_464,"AVRCP_PASS_THROUGH_COMMAND_CFM")
TRACE_MSG(PF_A2DP_3040_112_2_18_1_6_59_465,"AVRCP_UNIT_INFO_COMMAND_CFM\n")
TRACE_MSG(PF_A2DP_3046_112_2_18_1_6_59_466,"AVRCP_SUBUNIT_INFO_COMMAND_CFM\n")
TRACE_MSG(PF_A2DP_3052_112_2_18_1_6_59_467,"AVRCP_UNIT_INFO_COMMAND_IND")
TRACE_MSG(PF_A2DP_3075_112_2_18_1_6_59_468,"AVRCP_SUBUNIT_INFO_COMMAND_IND")
TRACE_MSG(PF_A2DP_3097_112_2_18_1_6_59_469,"app -default -primtype = 0x%0x\n")
TRACE_MSG(PF_A2DP_3109_112_2_18_1_6_59_470,"[A2DP_audioStart] sampleRate = %d")
TRACE_MSG(PF_A2DP_3111_112_2_18_1_6_59_471,"[A2DP_audioStart] state = %d")
TRACE_MSG(PF_A2DP_3112_112_2_18_1_6_59_472,"[A2DP_audioStart] isAudioOpen = %d")
TRACE_MSG(PF_A2DP_3117_112_2_18_1_6_59_473,"[DRV_BT]<ASSERT_ERROR> isAudioOpen = %d")
TRACE_MSG(PF_A2DP_3131_112_2_18_1_6_59_474,"[A2DP_audioClose] state = %d")
TRACE_MSG(PF_A2DP_3132_112_2_18_1_6_59_475,"[A2DP_audioClose] isAudioOpen = %d")
TRACE_MSG(PF_A2DP_3152_112_2_18_1_6_59_476,"[A2DP_handleDeinitPrim] primType = 0x%x --")
TRACE_MSG(PF_A2DP_3193_112_2_18_1_6_59_477,"[AVRCP_handleDeinitPrim] primType = 0x%x --")
TRACE_MSG(PF_A2DP_3221_112_2_18_1_6_59_478,"[A2DP_IsBusy] state = 0x%x --")
TRACE_MSG(SBC_PAL_113_112_2_18_1_7_0_479,"[BT]SBC_SetTaskPriority:Priority=0x%X\r\n")
TRACE_MSG(SBC_PAL_152_112_2_18_1_7_0_480,"[SBC_doEncode] IN")
TRACE_MSG(SBC_PAL_161_112_2_18_1_7_0_481,"[SBC_doEncode] audio buf is NULL and replace with mute data. mute_buf_size=%d\n")
TRACE_MSG(SBC_PAL_169_112_2_18_1_7_0_482,"[SBC_doEncode] Memory alloc err!\n")
TRACE_MSG(SBC_PAL_174_112_2_18_1_7_0_483,"[SBC_doEncode] malloc ok!\n")
TRACE_MSG(SBC_PAL_180_112_2_18_1_7_0_484,"[SBC_doEncode] memset ok!\n")
TRACE_MSG(SBC_PAL_199_112_2_18_1_7_0_485,"payload_size= %d, frames_per_payload = %d, data_cnt_per_frame = %d")
TRACE_MSG(SBC_PAL_202_112_2_18_1_7_0_486,"[SBC_doEncode] audio_buf->data_count = %d\ndata_in_pos=%d\ndata_out_pos=%d\ndata_per_encoding=%d\nbuffer_size=%d\nframes_per_payload=%d\nbytes_per_payload=%d")
TRACE_MSG(SBC_PAL_269_112_2_18_1_7_0_487,"[SBC_doEncode] no enough data")
TRACE_MSG(SBC_PAL_412_112_2_18_1_7_0_488,"[SBC_doEncode] sbcPacketCount = %d")
TRACE_MSG(SBC_PAL_448_112_2_18_1_7_1_489,"sbc_pal.c:config.alloc_method is %d,config.bitPool is %d,config.channel_mode is %d,config.nrof_blocks is %d,config.sample_freq is %d")
TRACE_MSG(SBC_PAL_454_112_2_18_1_7_1_490,"sbc_pal.c:sbc_encode_init result is %d")
TRACE_MSG(SBC_PAL_470_112_2_18_1_7_1_491,"sbc_pal.c:sbc_encode_init result error  is %d")
TRACE_MSG(SBC_PAL_566_112_2_18_1_7_1_492,"[BT]SBC_Init:s_sbc_task_priority=0x%X\r\n")
TRACE_MSG(SBC_PAL_773_112_2_18_1_7_1_493,"[PrepareSbc] maxFrameSize = %i")
TRACE_MSG(SBC_PAL_775_112_2_18_1_7_1_494,"[PrepareSbc] channel_mode = %i, sample_freq = %i, blocks = %i")
TRACE_MSG(SBC_PAL_777_112_2_18_1_7_1_495,"[PrepareSbc] subbands = %i, alloc_method = %d, maxBitpool = %in")
TRACE_MSG(SBC_PAL_787_112_2_18_1_7_1_496,"[PrepareSbc] bitPool = %d, bitPoolAlt = %d, togglePeriod = %d")
TRACE_MSG(SBC_PAL_799_112_2_18_1_7_1_497,"[SBC_Config] bitPool = %d")
TRACE_MSG(SBC_PAL_808_112_2_18_1_7_1_498,"[PrepareSbc] framesize = %d")
TRACE_MSG(SBC_PAL_848_112_2_18_1_7_2_499,"[PrepareSbc] framesize = %d, bitPool = %d, frames_per_payload = %d, data_per_encoding = %d")
TRACE_MSG(SBC_PAL_DSP_155_112_2_18_1_7_2_500,"[SBC_Config_Callback] frame_size=%d")
TRACE_MSG(SBC_PAL_DSP_439_112_2_18_1_7_3_501,"[SBC_DSP_Continue_Encode] audio buf is NULL or no use trackbuf")
TRACE_MSG(SBC_PAL_DSP_446_112_2_18_1_7_3_502,"[SBC_DSP_Continue_Encode] mp3 buffer data is not enough!")
TRACE_MSG(SBC_PAL_DSP_452_112_2_18_1_7_3_503,"[SBC_DSP_Continue_Encode] SBC packet out buf is full ,so wait some time to encode sbc frame")
TRACE_MSG(SBC_PAL_DSP_621_112_2_18_1_7_3_504,"SBC_getPacket: sbcPacketCount = %d")
TRACE_MSG(SBC_PAL_DSP_780_112_2_18_1_7_3_505,"[PrepareSbc] maxFrameSize = %i")
TRACE_MSG(SBC_PAL_DSP_782_112_2_18_1_7_3_506,"[PrepareSbc] channel_mode = %i, sample_freq = %i, blocks = %i")
TRACE_MSG(SBC_PAL_DSP_784_112_2_18_1_7_3_507,"[PrepareSbc] subbands = %i, alloc_method = %d, maxBitpool = %in")
TRACE_MSG(SBC_PAL_DSP_795_112_2_18_1_7_3_508,"[PrepareSbc] bitPool = %d, bitPoolAlt = %d, togglePeriod = %d")
TRACE_MSG(SBC_PAL_DSP_809_112_2_18_1_7_3_509,"[PrepareSbc] framesize = %d")
TRACE_MSG(SBC_PAL_DSP_847_112_2_18_1_7_3_510,"[PrepareSbc] framesize = %d, bitPool = %d, frames_per_payload = %d, data_per_encoding = %d")
TRACE_MSG(SBC_PAL_OTHER_122_112_2_18_1_7_4_511,"[PrepareSbc] maxFrameSize = %i")
TRACE_MSG(SBC_PAL_OTHER_124_112_2_18_1_7_4_512,"[PrepareSbc] channel_mode = %i, sample_freq = %i, blocks = %i")
TRACE_MSG(SBC_PAL_OTHER_126_112_2_18_1_7_4_513,"[PrepareSbc] subbands = %i, alloc_method = %d, maxBitpool = %in")
TRACE_MSG(SBC_PAL_OTHER_136_112_2_18_1_7_4_514,"[PrepareSbc] bitPool = %d, bitPoolAlt = %d, togglePeriod = %d")
TRACE_MSG(SBC_PAL_OTHER_150_112_2_18_1_7_4_515,"[PrepareSbc] framesize = %d")
TRACE_MSG(SBC_PAL_OTHER_188_112_2_18_1_7_4_516,"[PrepareSbc] framesize = %d, bitPool = %d, frames_per_payload = %d, data_per_encoding = %d")
TRACE_MSG(PF_APP_130_112_2_18_1_7_11_517,"rand = %x")
TRACE_MSG(PF_APP_182_112_2_18_1_7_11_518,"[BT]APP_BtConfig")
TRACE_MSG(PF_APP_208_112_2_18_1_7_11_519,"[BT]:APP_Init:BT Task Priority=0x%X\r\n")
TRACE_MSG(PF_APP_233_112_2_18_1_7_11_520,"[BT]APP_DeInitBt\r\n")
TRACE_MSG(PF_APP_247_112_2_18_1_7_11_521,"[BT]APP_BchsBuildAck:Bchs Buid Fail\r\n")
TRACE_MSG(PF_APP_263_112_2_18_1_7_11_522,"[BT]APP_CheckBchsLink\r\n")
TRACE_MSG(PF_APP_286_112_2_18_1_7_11_523,"[BT]APP_BuildBchsLink:BCHS State=0x%X\r\n")
TRACE_MSG(PF_APP_314_112_2_18_1_7_11_524,"[DRV_BT]<ASSERT_ERROR> APP_BuildBchsLink: invalid s_bt_bchs_state = %d")
TRACE_MSG(PF_APP_321_112_2_18_1_7_11_525,"[BT]APP_BtStartAgain:link_ready=0x%X\r\n")
TRACE_MSG(PF_APP_367_112_2_18_1_7_11_526,"[BT]<ERROR>%s L%d: APP_BtStartTimeOut()")
TRACE_MSG(PF_APP_369_112_2_18_1_7_11_527,"[BT]:link=0x%X, state=0x%X")
TRACE_MSG(PF_APP_393_112_2_18_1_7_12_528,"[BT]<ERROR> %s L%d: SCI_CreateTimer() fail")
TRACE_MSG(PF_APP_411_112_2_18_1_7_12_529,"[BT] CSR BCHS VERSION: %d.%d.%d.%d.%d\r\n")
TRACE_MSG(PF_APP_412_112_2_18_1_7_12_530,"[BT] CSR BCHS feature list: 0x%08X\r\n")
TRACE_MSG(PF_APP_414_112_2_18_1_7_12_531,"[BT]:APP_BtStart:bchs link isReady:0x%X,state:0x%X\r\n")
TRACE_MSG(PF_APP_462_112_2_18_1_7_12_532,"[BT]APP_BtStopCallBack:link_ready=0x%X, state=0x%X\r\n")
TRACE_MSG(PF_APP_470_112_2_18_1_7_12_533,"[BT]Rebuild BCHS Link........\r\n")
TRACE_MSG(PF_APP_483_112_2_18_1_7_12_534,"[BT]Could Not Build The Connection Between BB and BT Chip.........\r\n")
TRACE_MSG(PF_APP_493_112_2_18_1_7_12_535,"[BT]PF_ActivateService, service_map=0x%08x\r\n")
TRACE_MSG(PF_APP_501_112_2_18_1_7_12_536,"[BT]PF_DeactivateServiceMap, service_map=0x%08x\r\n")
TRACE_MSG(PF_APP_525_112_2_18_1_7_12_537,"[BT]APP_StopTimeOut bt_activated_serv_map=%x\r\n")
TRACE_MSG(PF_APP_532_112_2_18_1_7_12_538,"[BT]APP_BtStop\r\n")
TRACE_MSG(PF_APP_597_112_2_18_1_7_12_539,"[pf_app]timed_event_in fail")
TRACE_MSG(PF_APP_611_112_2_18_1_7_12_540,"APP_CloseRunningService: service=0x%X\r\n")
TRACE_MSG(PF_APP_644_112_2_18_1_7_12_541,"[DRV_BT]<ASSERT_ERROR> APP_CloseRunningService")
TRACE_MSG(PF_APP_670_112_2_18_1_7_12_542,"[BT]APP_SetServerType:type=0x%08X\r\n")
TRACE_MSG(PF_APP_688_112_2_18_1_7_12_543,"[BT]APP_InitProfile\r\n")
TRACE_MSG(PF_APP_717_112_2_18_1_7_12_544,"[BT]APP_DeInitProfile:Active=0x%X\r\n")
TRACE_MSG(PF_APP_779_112_2_18_1_7_12_545,"[BT]App_ActiveProfile\r\n")
TRACE_MSG(PF_APP_813_112_2_18_1_7_12_546,"[BT]APP_ActiveA2DPCfm\r\n")
TRACE_MSG(PF_APP_846_112_2_18_1_7_12_547,"[BT]APP_RemovePairedDevice:ret = 0x%X\r\n")
TRACE_MSG(PF_APP_868_112_2_18_1_7_13_548,"[BT]APP_RefreshPairedDeviceService\r\n")
TRACE_MSG(PF_APP_902_112_2_18_1_7_13_549,"[BT]APP_CancelOppSendFile\r\n")
TRACE_MSG(PF_APP_909_112_2_18_1_7_13_550,"[BT]APP_OppReceiveFile\r\n")
TRACE_MSG(PF_APP_916_112_2_18_1_7_13_551,"[BT]APP_CancelOppReceiveFile\r\n")
TRACE_MSG(PF_APP_924_112_2_18_1_7_13_552,"[BT]APP_FtpSendFile\r\n")
TRACE_MSG(PF_APP_929_112_2_18_1_7_13_553,"[BT]BT_FtpReceiveFile\r\n")
TRACE_MSG(PF_APP_934_112_2_18_1_7_13_554,"[BT]BT_CancelFtpReceiveFile\r\n")
TRACE_MSG(PF_APP_957_112_2_18_1_7_13_555,"[BT]APP_SendMsgToMmi:Msg_Id=0x%X, status=0x%X, body=0x%X, size=0x%X\r\n")
TRACE_MSG(PF_APP_961_112_2_18_1_7_13_556,"[BT]APP_SendMsgToMmi:Invail Message\r\n")
TRACE_MSG(PF_APP_976_112_2_18_1_7_13_557,"[BT]APP_SendMsgToMmi:No CallBack Function\r\n")
TRACE_MSG(PF_APP_995_112_2_18_1_7_13_558,"[BT]APP_HandleMsg:event=0x%X\r\n")
TRACE_MSG(PF_APP_1017_112_2_18_1_7_13_559,"[DRV_BT]<ASSERT_ERROR> SDP_PRIM ")
TRACE_MSG(PF_APP_1113_112_2_18_1_7_13_560,"[SD_PRIM] prim_type = 0x%x")
TRACE_MSG(PF_APP_1120_112_2_18_1_7_13_561,"[SD_PRIM] prim_type = 0x%x")
TRACE_MSG(PF_APP_1199_112_2_18_1_7_13_562,"[DRV_BT]<ASSERT_ERROR> APP_HandleMsg: default")
TRACE_MSG(PF_APP_1216_112_2_18_1_7_13_563,"[_ft_task] SignalCode = %d")
TRACE_MSG(PF_APP_1276_112_2_18_1_7_13_564,"[DRV_BT]<ASSERT_ERROR> _ft_task : default")
TRACE_MSG(PF_CM_93_112_2_18_1_7_14_565,"[BT]TimeOutExitHandle:Type=0x%X\r\n")
TRACE_MSG(PF_CM_122_112_2_18_1_7_14_566,"[BT]CM_TimeOutHandle:Type=0x%X,%x\r\n")
TRACE_MSG(PF_CM_147_112_2_18_1_7_14_567,"[BT]:CM_TimeOutHandle - addr:lap:0x%X, uap:0x%X, nap:0x%X\r\n")
TRACE_MSG(PF_CM_161_112_2_18_1_7_14_568,"[BT]<ERROR>%s L%d: SC_DEBOND_CFM TimeOut")
TRACE_MSG(PF_CM_202_112_2_18_1_7_14_569,"[BT]CM_TimeOutHandle:Type  error")
TRACE_MSG(PF_CM_212_112_2_18_1_7_14_570,"[BT]CM_SC_Init\r\n")
TRACE_MSG(PF_CM_228_112_2_18_1_7_14_571,"[BT]CM_StartInquiry:classOfDeviceMask=0x%X,state=0x%X\r\n")
TRACE_MSG(PF_CM_241_112_2_18_1_7_14_572,"[HFG]CM_StartInquiry creat cm_search_time fial")
TRACE_MSG(PF_CM_248_112_2_18_1_7_14_573,"[BT]CM_StartInquiry:BT is inquirying.......\r\n")
TRACE_MSG(PF_CM_264_112_2_18_1_7_14_574,"[BT]PF_AppStopInquiryHs:state=0x%X\r\n")
TRACE_MSG(PF_CM_277_112_2_18_1_7_14_575,"[BT]CM_StopInquiry cancel time")
TRACE_MSG(PF_CM_288_112_2_18_1_7_14_576,"[BT]CM_StopInquiry:BT is not in inqirying!\r\n")
TRACE_MSG(PF_CM_316_112_2_18_1_7_14_577,"[BT]CM_SetVisibleCfm\r\n")
TRACE_MSG(PF_CM_329_112_2_18_1_7_15_578,"[HFG]timed_event_in fail")
TRACE_MSG(PF_CM_354_112_2_18_1_7_15_579,"[BT]CM_SetVisible:scan=0x%X, inquiry=0x%X, page=0x%X\r\n")
TRACE_MSG(PF_CM_378_112_2_18_1_7_15_580,"[BT]timed_event_in fail")
TRACE_MSG(PF_CM_393_112_2_18_1_7_15_581,"[BT]CM_RestoreVisible\r\n")
TRACE_MSG(PF_CM_405_112_2_18_1_7_15_582,"[BT]CM_SetVisibleAtPowerOn:isVisibleAtPowerOn=0x%X\r\n")
TRACE_MSG(PF_CM_422_112_2_18_1_7_15_583,"[BT]CM_ScanReadEnable\r\n")
TRACE_MSG(PF_CM_428_112_2_18_1_7_15_584,"[BT]CM_ScanWriteEnable:inuiry=0x%X,page=0x%X\r\n")
TRACE_MSG(PF_CM_434_112_2_18_1_7_15_585,"[BT]CM_ReadLocalName\r\n")
TRACE_MSG(PF_CM_441_112_2_18_1_7_15_586,"[BT]CM_SetLocalName:name_ptr=%s\r\n")
TRACE_MSG(PF_CM_448_112_2_18_1_7_15_587,"[BT]CM_ReadLocalBdAddr\r\n")
TRACE_MSG(PF_CM_457_112_2_18_1_7_15_588,"[BT]CM_DeBondReq:state=0x%X,addr lap:0x%x,uap:0x%x,nap:0x%xrn")
TRACE_MSG(PF_CM_527_112_2_18_1_7_15_589,"Devclass have been added in list ")
TRACE_MSG(PF_CM_533_112_2_18_1_7_15_590," not a dongle device")
TRACE_MSG(PF_CM_537_112_2_18_1_7_15_591,"AddDevicelist device_info= 0x%x,name[0]=0x%x,name[1]=0x%x,name[2]=0x%x")
TRACE_MSG(PF_CM_568_112_2_18_1_7_15_592," error ,error ,error !!! device is paired full")
TRACE_MSG(PF_CM_571_112_2_18_1_7_15_593,"devcice number =%d")
TRACE_MSG(PF_CM_606_112_2_18_1_7_15_594,"not find the device in the list")
TRACE_MSG(PF_CM_628_112_2_18_1_7_15_595,"[CM_GetDeviceClass] IN ( BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x: )")
TRACE_MSG(PF_CM_639_112_2_18_1_7_15_596,"device_class=0x%x,i=%d")
TRACE_MSG(PF_CM_666_112_2_18_1_7_15_597,"[CM_GetDeviceClass] IN ( BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x: )")
TRACE_MSG(PF_CM_676_112_2_18_1_7_15_598,"device_class=0x%x,i=%d")
TRACE_MSG(PF_CM_692_112_2_18_1_7_15_599,"[BT]SC_SetPairDeviceStatus: status = %d\n")
TRACE_MSG(PF_CM_701_112_2_18_1_7_15_600,"[BT]SC_SetPairDeviceAddr: is_paired = %d\n")
TRACE_MSG(PF_CM_712_112_2_18_1_7_16_601,"[BT]SC_GetPairDeviceAddr: is_paired = %d\n")
TRACE_MSG(PF_CM_755_112_2_18_1_7_16_602,"SetDevicePaired device_class=0x%x,i=%d")
TRACE_MSG(PF_CM_760_112_2_18_1_7_16_603,"error,error,device don't find")
TRACE_MSG(PF_CM_782_112_2_18_1_7_16_604,"[BT]CM_MsgPrimHandle: type = 0x%X\r\n")
TRACE_MSG(PF_CM_790_112_2_18_1_7_16_605,"[BT]:CM_DISCOVERY_RESULT_IND - addr:lap:0x%X, uap:0x%X, nap:0x%X\r\n")
TRACE_MSG(PF_CM_794_112_2_18_1_7_16_606,"[BT]CM_DISCOVERY_RESULT_IND:Search a same device\r\n")
TRACE_MSG(PF_CM_806_112_2_18_1_7_16_607,"[BT]CM_DISCOVERY_RESULT_IND:Search a empty device name\r\n")
TRACE_MSG(PF_CM_848_112_2_18_1_7_16_608,"[BT] MsgPrimHandle CM_DISCOVERY_CFM")
TRACE_MSG(PF_CM_853_112_2_18_1_7_16_609,"[BT] MsgPrimHandle cancel serach time")
TRACE_MSG(PF_CM_862_112_2_18_1_7_16_610,"[BT]CM_CANCEL_DISCOVERY_CFM:state=%d\r\n")
TRACE_MSG(PF_CM_897_112_2_18_1_7_16_611,"[BT]CM_CANCEL_DISCOVERY_CFM:error state\r\n")
TRACE_MSG(PF_CM_906_112_2_18_1_7_16_612,"[BT]CM_MsgPrimHandle: name = %s\r\n")
TRACE_MSG(PF_CM_930_112_2_18_1_7_16_613,"[BT]CM_READ_REMOTE_NAME_CFM:resul = 0x%X\r\n")
TRACE_MSG(PF_CM_955_112_2_18_1_7_16_614,"[BT]CM_SET_LOCAL_NAME_CFM:result = 0x%X, start=0x%X\r\n")
TRACE_MSG(PF_CM_988_112_2_18_1_7_16_615,"[BT]CM_READ_LOCAL_BD_ADDR_CFM:lap:%d, uap:%d,nap:%d\r\n")
TRACE_MSG(PF_CM_997_112_2_18_1_7_16_616,"[BT]Scan Read result:= 0x%x, scanEnable:= 0x%x\r\n")
TRACE_MSG(PF_CM_1044_112_2_18_1_7_16_617,"[BT]CM_READ_SCAN_ENABLE_CFM:Invaild Message\r\n")
TRACE_MSG(PF_CM_1070_112_2_18_1_7_16_618,"[DRV_BT]<ASSERT_ERROR> PF_MsgCmPrimHandle:Unhandle\r\n")
TRACE_MSG(PF_CM_1102_112_2_18_1_7_16_619,"[BT]_StartPairing: deviceAddr = %04X:%02X:%06X")
TRACE_MSG(PF_CM_1126_112_2_18_1_7_16_620,"[BT]SC_StartPairing:state=0x%X\r\n")
TRACE_MSG(PF_CM_1134_112_2_18_1_7_16_621,"[BT]SC_StartPairing:Cancel Discovery at first!\r\n")
TRACE_MSG(PF_CM_1152_112_2_18_1_7_16_622,"[BT]SC_CancelPairing: state=0x%X, deviceAddr = %04X:%02X:%06X")
TRACE_MSG(PF_CM_1194_112_2_18_1_7_17_623,"[BT]SC_CancelPairing: Empty Operate!\r\n")
TRACE_MSG(PF_CM_1208_112_2_18_1_7_17_624,"[BT]SC_InputPairPin:lap=0x%X,uap=0x%X, nap=0x%X\r\n")
TRACE_MSG(PF_CM_1236_112_2_18_1_7_17_625,"[BT]SC_MsgPrimHandle:type = 0x%X\r\n")
TRACE_MSG(PF_CM_1245_112_2_18_1_7_17_626,"[BT]SC_PASSKEY_IND:lap=0x%X,uap=0x%X, nap=0x%X\r\n")
TRACE_MSG(PF_CM_1295_112_2_18_1_7_17_627,"[BT]SC_BOND_CFM: result= 0x%X,cod=%x,class=%x\r\n")
TRACE_MSG(PF_CM_1311_112_2_18_1_7_17_628,"[BT]<INFO> cod = 0x%X, it's a Audio Device")
TRACE_MSG(PF_CM_1328_112_2_18_1_7_17_629,"[BT] state is idle")
TRACE_MSG(PF_CM_1334_112_2_18_1_7_17_630,"[BT]BCHS_BONDING_CANCELLED SUCCESSFUL ")
TRACE_MSG(PF_CM_1351_112_2_18_1_7_17_631,"[BT]SC_BOND_IND:result= 0x%X\r\n")
TRACE_MSG(PF_CM_1368_112_2_18_1_7_17_632,"[BT]<INFO> cod = 0x%X, it's a Audio Device")
TRACE_MSG(PF_CM_1389_112_2_18_1_7_17_633,"[BT]SC_AUTHORISE_IND\r\n")
TRACE_MSG(PF_CM_1401_112_2_18_1_7_17_634,"[BT]SC_DEBOND_CFM:result=0x%X\r\n")
TRACE_MSG(PF_CM_1414_112_2_18_1_7_17_635,"[DRV_BT]<ASSERT_ERROR> default in scPrim handler %x,\n")
TRACE_MSG(PF_FILE_56_112_2_18_1_7_19_636,"[BT]PF_fread length is zore!\r\n")
TRACE_MSG(PF_FILE_80_112_2_18_1_7_19_637,"[BT]PF_fread length is zore!\r\n")
TRACE_MSG(PF_FILE_101_112_2_18_1_7_19_638,"[BT]<ERROR> file_name invalid")
TRACE_MSG(PF_FTP_CLIENT_258_112_2_18_1_7_20_639,"[BT]<FTC> ListFolder total_length = %d, data_length = %d, ")
TRACE_MSG(PF_FTP_CLIENT_266_112_2_18_1_7_20_640,"[BT]<FTC> ListFolder string = %s")
TRACE_MSG(PF_FTP_CLIENT_288_112_2_18_1_7_20_641,"[BT]<FTC> ListFolder: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ")
TRACE_MSG(PF_FTP_CLIENT_310_112_2_18_1_7_20_642,"[BT]<FTC> ListFolderBody data_length = %d, final = %d, data = 0x%x")
TRACE_MSG(PF_FTP_CLIENT_319_112_2_18_1_7_21_643,"[BT]<FTC> ListFolder string = %s")
TRACE_MSG(PF_FTP_CLIENT_341_112_2_18_1_7_21_644,"[BT]<FTC> ListFolder: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ")
TRACE_MSG(PF_FTP_CLIENT_360_112_2_18_1_7_21_645,"[BT]<FTC> FTC_ChangeClk: frq_cnt = %d")
TRACE_MSG(PF_FTP_CLIENT_377_112_2_18_1_7_21_646,"[BT]<FTC> FTC_RestoreClk: frq_cnt = %d")
TRACE_MSG(PF_FTP_CLIENT_401_112_2_18_1_7_21_647,"[BT]<FTC> FTC_IsBusy: state=0x%X")
TRACE_MSG(PF_FTP_CLIENT_417_112_2_18_1_7_21_648,"[BT]<FTC> FTC_Init state = %d")
TRACE_MSG(PF_FTP_CLIENT_433_112_2_18_1_7_21_649,"[BT]<FTC> FTC_DeInit state = %d")
TRACE_MSG(PF_FTP_CLIENT_466_112_2_18_1_7_21_650,"[BT]<FTC> FTC_ConnectServer: ")
TRACE_MSG(PF_FTP_CLIENT_507_112_2_18_1_7_21_651,"[BT]<FTC> FTC_CancelConnectServer ")
TRACE_MSG(PF_FTP_CLIENT_531_112_2_18_1_7_21_652,"[BT]<FTC> FTC_SetPath: state = %d")
TRACE_MSG(PF_FTP_CLIENT_563_112_2_18_1_7_21_653,"[BT]<FTC> FTC_GoBackPath: state = %d")
TRACE_MSG(PF_FTP_CLIENT_592_112_2_18_1_7_21_654,"[BT]<FTC> FTC_GoRootPath: state = %d")
TRACE_MSG(PF_FTP_CLIENT_623_112_2_18_1_7_21_655,"[BT]<FTC> FTC_AddNewFolder: state = %d")
TRACE_MSG(PF_FTP_CLIENT_661_112_2_18_1_7_21_656,"[BT]<FTC> FTC_SendFile state = %d")
TRACE_MSG(PF_FTP_CLIENT_676_112_2_18_1_7_21_657,"[BT]<ERROR> %s L%d: openfile")
TRACE_MSG(PF_FTP_CLIENT_683_112_2_18_1_7_21_658,"[BT]<FTC> FTC_SendFile file_name_len = %d, file_size = %d")
TRACE_MSG(PF_FTP_CLIENT_726_112_2_18_1_7_21_659,"[BT]<FTC> FTC_ReceiveFile: state = %d")
TRACE_MSG(PF_FTP_CLIENT_739_112_2_18_1_7_21_660,"[BT]<ERROR> %s L%d: openfile")
TRACE_MSG(PF_FTP_CLIENT_745_112_2_18_1_7_21_661,"[BT]<FTC> FTC_ReceiveFile: file_name_len = %d, file_size = %d")
TRACE_MSG(PF_FTP_CLIENT_786_112_2_18_1_7_21_662,"[BT]<FTC> FTC_Cancel: state = 0x%X\r\n")
TRACE_MSG(PF_FTP_CLIENT_820_112_2_18_1_7_21_663,"[BT]<FTC> FTC_DeleteObject: state = %d")
TRACE_MSG(PF_FTP_CLIENT_845_112_2_18_1_7_21_664,"[BT]<FTC> FTC_DisconnectServer: state = %d")
TRACE_MSG(PF_FTP_CLIENT_871_112_2_18_1_7_22_665,"[BT]<FTC> result = 0x%x\r\n")
TRACE_MSG(PF_FTP_CLIENT_907_112_2_18_1_7_22_666,"[BT]<FTC> state = 0x%X\r\n")
TRACE_MSG(PF_FTP_CLIENT_950_112_2_18_1_7_22_667,"[BT]<FTC> SetFolder result = 0x%x\r\n")
TRACE_MSG(PF_FTP_CLIENT_1099_112_2_18_1_7_22_668,"[BT]<FTC> PUT Header: result = 0x%X, bodysize = %d, state = %d, ")
TRACE_MSG(PF_FTP_CLIENT_1128_112_2_18_1_7_22_669,"[BT]<ERROR> %s L%d: read_len(%d) != send_len(%d)")
TRACE_MSG(PF_FTP_CLIENT_1193_112_2_18_1_7_22_670,"[BT]<FTC> PUT BODY: result = 0x%X, bodysize = %d, ")
TRACE_MSG(PF_FTP_CLIENT_1244_112_2_18_1_7_22_671,"[BT]<ERROR> %s L%d: read_len(%d) != send_len(%d)")
TRACE_MSG(PF_FTP_CLIENT_1308_112_2_18_1_7_22_672,"[BT]<FTC> GET Header: result = 0x%X, finalFlag = %d, ")
TRACE_MSG(PF_FTP_CLIENT_1324_112_2_18_1_7_22_673,"[BT]<ERROR> %s L%d: write_len(%d) != received_len(%d)")
TRACE_MSG(PF_FTP_CLIENT_1404_112_2_18_1_7_23_674,"[BT]<FTC> GET BODY: result = 0x%X, finalFlag = %d, ")
TRACE_MSG(PF_FTP_CLIENT_1420_112_2_18_1_7_23_675,"[BT]<ERROR> %s L%d: write_len(%d) != received_len(%d)")
TRACE_MSG(PF_FTP_CLIENT_1489_112_2_18_1_7_23_676,"[BT]<FTC> SetFolder result = 0x%x\r\n")
TRACE_MSG(PF_FTP_CLIENT_1518_112_2_18_1_7_23_677,"[BT]<FTC> SetFolder result = 0x%x\r\n")
TRACE_MSG(PF_FTP_CLIENT_1597_112_2_18_1_7_23_678,"[BT]<FTC> FTC_HandlePrim: type = 0x%x\r\n")
TRACE_MSG(PF_FTP_CLIENT_1611_112_2_18_1_7_23_679,"[BT]<WARNING> g_ftc_handle[%d] is invalid, array size = %d")
TRACE_MSG(PF_FTP_CLIENT_1616_112_2_18_1_7_23_680,"[BT]<FTC> FTC_HandlePrim: id = %d, %srn")
TRACE_MSG(PF_FTP_SERVER_169_112_2_18_1_7_24_681,"[DRV_BT] FTS_Init_RES(): s_fts_dummy = 0x%X, g_ftp_tmp_path = 0x%X, ")
TRACE_MSG(PF_FTP_SERVER_178_112_2_18_1_7_24_682,"[DRV_BT] FTS_DeInit_RES(in): s_fts_dummy = 0x%X, g_ftp_tmp_path = 0x%X, ")
TRACE_MSG(PF_FTP_SERVER_210_112_2_18_1_7_24_683,"[DRV_BT] FTS_DeInit_RES(out): s_fts_dummy = 0x%X, g_ftp_tmp_path = 0x%X, ")
TRACE_MSG(PF_FTP_SERVER_232_112_2_18_1_7_24_684,"[BT]FTS_GetState error usr_addr ")
TRACE_MSG(PF_FTP_SERVER_240_112_2_18_1_7_24_685,"[BT]FTS_ChangeClk:Change System Clock;cnt =0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_251_112_2_18_1_7_24_686,"[BT]FTS_RestoreClk:Restore System Clock=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_265_112_2_18_1_7_24_687,"[BT]FTS_Active\r\n")
TRACE_MSG(PF_FTP_SERVER_275_112_2_18_1_7_24_688,"[BT]FTS_DeActive\r\n")
TRACE_MSG(PF_FTP_SERVER_282_112_2_18_1_7_24_689,"[BT][FTS_ServiceClose] IN")
TRACE_MSG(PF_FTP_SERVER_283_112_2_18_1_7_24_690,"[BT][FTS_ServiceClose] status = %d")
TRACE_MSG(PF_FTP_SERVER_311_112_2_18_1_7_24_691,"[BT]FTS_SetRootFolder:state=0x%X oper_state=0x%Xrn")
TRACE_MSG(PF_FTP_SERVER_316_112_2_18_1_7_24_692,"[BT]FTS_SetRootFolder:Should not reset root folder\r\n")
TRACE_MSG(PF_FTP_SERVER_323_112_2_18_1_7_24_693,"[BT]FTS_SetRootFolder:Name[%d]=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_331_112_2_18_1_7_24_694,"[BT] FTS_Init:Root Folder is not exist\r\n")
TRACE_MSG(PF_FTP_SERVER_382_112_2_18_1_7_24_695,"[BT]<ERROR>%s L%d: FTS_GetFolderInfoLength(): malloc() error")
TRACE_MSG(PF_FTP_SERVER_402_112_2_18_1_7_24_696,"[BT] FTS_GetFolderInfoLength w_name_len=%d, utf8_len=%d")
TRACE_MSG(PF_FTP_SERVER_443_112_2_18_1_7_24_697,"[BT] FTS_GetFolderInfoLength w_name_len=%d, utf8_len=%d")
TRACE_MSG(PF_FTP_SERVER_482_112_2_18_1_7_24_698,"[BT]FTS_GetFolderInformation: The Folder is empty \r\n")
TRACE_MSG(PF_FTP_SERVER_550_112_2_18_1_7_24_699,"[BT]FTS_GetListFolderLen:buf_len= 0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_556_112_2_18_1_7_24_700,"[BT]FTS_GetListFolderLen:Is Empty\r\n")
TRACE_MSG(PF_FTP_SERVER_577_112_2_18_1_7_24_701,"[BT]FTS_GetListFolderInfo:Memory allocate error!\n")
TRACE_MSG(PF_FTP_SERVER_586_112_2_18_1_7_24_702,"[BT]<ERROR>%s L%d: FTS_GetListFolderInfo(): malloc() error")
TRACE_MSG(PF_FTP_SERVER_640_112_2_18_1_7_25_703,"[BT]FTS_GetListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_671_112_2_18_1_7_25_704,"[BT]FTS_GetListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_710_112_2_18_1_7_25_705,"[BT]FTS_GetListFolderInfo:line=%d, bytes_allocated_len=%d,left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_719_112_2_18_1_7_25_706,"[BT]FTS_GetListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_750_112_2_18_1_7_25_707,"[BT]FTS_GetListFolderInfo:line=%d, bytes_allocated_len=%d,left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_758_112_2_18_1_7_25_708,"[BT]FTS_GetListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_774_112_2_18_1_7_25_709,"[BT]FTS_FolderInfoToXml: The Folder is empty \r\n")
TRACE_MSG(PF_FTP_SERVER_790_112_2_18_1_7_25_710,"[BT]FTS_GetListFolderInfo:line=%d, left_buf_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_794_112_2_18_1_7_25_711,"[BT]FTS_GetListFolderInfo:left_len=%d, bytes_to_write=%d, fts_file_folder_count=%d\n")
TRACE_MSG(PF_FTP_SERVER_816_112_2_18_1_7_25_712,"[BT]FTS_GetNextListFolderInfo:Memory allocate error!\n")
TRACE_MSG(PF_FTP_SERVER_826_112_2_18_1_7_25_713,"[BT]FTS_GetNextListFolderInfo: There is no data left.!!!!!\n")
TRACE_MSG(PF_FTP_SERVER_831_112_2_18_1_7_25_714,"[BT]FTS_GetNextListFolderInfo:allcated_len=%d, left_buf_len=%d, object_left_len=%d, fts_file_folder_count=%dn")
TRACE_MSG(PF_FTP_SERVER_837_112_2_18_1_7_25_715,"[BT]<ERROR>%s L%d: malloc() error")
TRACE_MSG(PF_FTP_SERVER_878_112_2_18_1_7_25_716,"[BT]FTS_GetNextListFolderInfo:line=%d, bytes_allocated_len=%d,left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_886_112_2_18_1_7_25_717,"[BT]FTS_GetNextListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_912_112_2_18_1_7_25_718,"[BT]FTS_GetNextListFolderInfo:line=%d, bytes_allocated_len=%d,left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_920_112_2_18_1_7_25_719,"[BT]FTS_GetNextListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_944_112_2_18_1_7_25_720,"[BT]FTS_GetNextListFolderInfo:line=%d, left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_949_112_2_18_1_7_25_721,"[BT]FTS_GetNextListFolderInfo:left_len=%d, bytes_to_write=%d\n")
TRACE_MSG(PF_FTP_SERVER_957_112_2_18_1_7_25_722,"[BT]FTS_IsBusy:state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_966_112_2_18_1_7_25_723,"[BT]FTS_Init\r\n")
TRACE_MSG(PF_FTP_SERVER_985_112_2_18_1_7_25_724,"[BT]FTS_Init:Name[%d]=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_996_112_2_18_1_7_25_725,"[BT]FTS_DeInit\r\n")
TRACE_MSG(PF_FTP_SERVER_1038_112_2_18_1_7_25_726,"[BT]FTS_ReceiveFileFail\r\n")
TRACE_MSG(PF_FTP_SERVER_1055_112_2_18_1_7_25_727,"[BT]FTS_ReceiveFileFinalPacket\r\n")
TRACE_MSG(PF_FTP_SERVER_1105_112_2_18_1_7_26_728,"[BT]FTS_GET_OBJ_IND:Name[%d] = 0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1112_112_2_18_1_7_26_729,"[BT]FTS_GET_OBJ_IND:Failed to open the file \r\n")
TRACE_MSG(PF_FTP_SERVER_1135_112_2_18_1_7_26_730,"[BT]FTS_GET_OBJ_IND:bytesToRead=%d \r\n")
TRACE_MSG(PF_FTP_SERVER_1140_112_2_18_1_7_26_731,"[BT]FTS_GET_OBJ_IND:Failed to read the file \r\n")
TRACE_MSG(PF_FTP_SERVER_1233_112_2_18_1_7_26_732,"[BT]FTS_ReceiveFile\r\n")
TRACE_MSG(PF_FTP_SERVER_1249_112_2_18_1_7_26_733,"[BT]FTS_SetRootFolder:w_len=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1389_112_2_18_1_7_26_734,"[bt] cancel ftp timer g_Timeoutcancel_timer=0x%x")
TRACE_MSG(PF_FTP_SERVER_1406_112_2_18_1_7_26_735,"send putting the cancel command")
TRACE_MSG(PF_FTP_SERVER_1422_112_2_18_1_7_26_736,"send getting the cancel command")
TRACE_MSG(PF_FTP_SERVER_1438_112_2_18_1_7_26_737,"send opc the cancel command")
TRACE_MSG(PF_FTP_SERVER_1480_112_2_18_1_7_26_738,"Handle_Timeout_Cancel_Msg start")
TRACE_MSG(PF_FTP_SERVER_1489_112_2_18_1_7_26_739,"[BT_pf_ftp_server] SCI_GetEvent(s_bt_ftp_timeout_envet)=")
TRACE_MSG(PF_FTP_SERVER_1501_112_2_18_1_7_26_740,"put_file_handler release resourse successful")
TRACE_MSG(PF_FTP_SERVER_1509_112_2_18_1_7_26_741,"get_file_handler release resourse successful")
TRACE_MSG(PF_FTP_SERVER_1515_112_2_18_1_7_26_742,"OPC_CloseRES successful")
TRACE_MSG(PF_FTP_SERVER_1520_112_2_18_1_7_26_743,"none operate out here")
TRACE_MSG(PF_FTP_SERVER_1546_112_2_18_1_7_26_744,"g_Timeoutcancel_timer =0x%x")
TRACE_MSG(PF_FTP_SERVER_1553_112_2_18_1_7_26_745,"ERROR: [BT_pf_ftp_server] s_bt_ftp_timeout_envet create error")
TRACE_MSG(PF_FTP_SERVER_1557_112_2_18_1_7_26_746,"[BT_pf_ftp_server] s_bt_ftp_timeout_envet = 0x%x")
TRACE_MSG(PF_FTP_SERVER_1602_112_2_18_1_7_27_747,"[BT]FTS_CancelPutFile:state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1632_112_2_18_1_7_27_748,"[BT]FTS_CancelPutFile:Invaild Cancel Received File\r\n")
TRACE_MSG(PF_FTP_SERVER_1643_112_2_18_1_7_27_749,"[BT]FTS_CancelGetFile:state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1666_112_2_18_1_7_27_750,"[BT]FTS_CancelReceive:Invaild Cancel Received File\r\n")
TRACE_MSG(PF_FTP_SERVER_1703_112_2_18_1_7_27_751,"[BT]FTS_SendFile: oper_state:0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1711_112_2_18_1_7_27_752,"[BT]FTS_SendFile: No operation\r\n")
TRACE_MSG(PF_FTP_SERVER_1721_112_2_18_1_7_27_753,"[BT]FTS_ReceiveFile: oper_state:0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1729_112_2_18_1_7_27_754,"[BT]FTS_ReceiveFile: No operation\r\n")
TRACE_MSG(PF_FTP_SERVER_1740_112_2_18_1_7_27_755,"[BT]FTS_CancelTransfer: oper_state:0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1752_112_2_18_1_7_27_756,"[BT]FTS_CancelReceive: No operation\r\n")
TRACE_MSG(PF_FTP_SERVER_1764_112_2_18_1_7_27_757,"[BT]FTS_CreateNewFolder:is_enable=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1779_112_2_18_1_7_27_758,"[BT]FTS_CreateNewFolder:Folder create successful\r\n")
TRACE_MSG(PF_FTP_SERVER_1784_112_2_18_1_7_27_759,"[BT]FTS_CreateNewFolder:Folder Create Fail\r\n")
TRACE_MSG(PF_FTP_SERVER_1791_112_2_18_1_7_27_760,"[BT]FTS_CreateNewFolder:Cancel Folder Create\r\n")
TRACE_MSG(PF_FTP_SERVER_1805_112_2_18_1_7_27_761,"[BT]FTS_DeleteFile:is_enable = 0x%X, oper_state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1809_112_2_18_1_7_27_762,"[BT]FTS_DeleteFile:Error Delete\r\n")
TRACE_MSG(PF_FTP_SERVER_1821_112_2_18_1_7_27_763,"[BT]FTS_DeleteFile:Name[%d]=0x%04X\r\n")
TRACE_MSG(PF_FTP_SERVER_1830_112_2_18_1_7_27_764,"[BT]FTS_DeleteFile:Delete Fail\r\n")
TRACE_MSG(PF_FTP_SERVER_1839_112_2_18_1_7_27_765,"[BT]FTS_DeleteFile: Cancel File Delete\r\n")
TRACE_MSG(PF_FTP_SERVER_1863_112_2_18_1_7_27_766,"[BT]FTS_HandlePrim: type = 0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1873_112_2_18_1_7_27_767,"[BT]FTS_CONNECT_IND:state = 0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1881_112_2_18_1_7_27_768,"[BT]FTS_CONNECT_IND:Opp is busy now, could not respond connect request:is_have_disk =0x%X,w_len=%d\r\n")
TRACE_MSG(PF_FTP_SERVER_1892_112_2_18_1_7_27_769,"[BT]FTS_CONNECT_IND:connect=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1903_112_2_18_1_7_27_770,"[BT]reset root folder fail\r\n")
TRACE_MSG(PF_FTP_SERVER_1923_112_2_18_1_7_27_771,"[BT]FTS_CONNECT_IND:no default root to set:is_have_disk =0x%Xrn")
TRACE_MSG(PF_FTP_SERVER_1962_112_2_18_1_7_27_772,"[BT]FTS_HandlePrim:FTS_PUT_OBJ_IND-state = 0x%X, oper_state=0x%X, arribute =0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1966_112_2_18_1_7_27_773,"[BT]FTS_PUT_OBJ_IND:Opp is busy now, could not respond put object:is_have_disk =0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_1984_112_2_18_1_7_27_774,"[BT]FTS_HandlePrim:Delete Operate is time out\r\n")
TRACE_MSG(PF_FTP_SERVER_1989_112_2_18_1_7_27_775,"[BT]FTS_HandlePrim:FTS_PUT_OBJ_IND,One file is transmitting.......\r\n")
TRACE_MSG(PF_FTP_SERVER_2013_112_2_18_1_7_27_776,"[BT]FTS_PUT_OBJ_IND:offset = 0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2046_112_2_18_1_7_27_777,"[BT] Name[%d]=0x%04x\r\n")
TRACE_MSG(PF_FTP_SERVER_2051_112_2_18_1_7_27_778,"The Name is same as one folder\r\n")
TRACE_MSG(PF_FTP_SERVER_2088_112_2_18_1_7_28_779,"The whole path name is too long:0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2104_112_2_18_1_7_28_780,"[BT]FTS_HandlePrim:FTS_PUT_OBJ_NEXT_IND-state = 0x%X, oper_state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2111_112_2_18_1_7_28_781,"[BT]FTS_HandlePrim:FTS_PUT_OBJ_NEXT_IND-handle = 0x%X, Cancel=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2170_112_2_18_1_7_28_782,"[BT]Name[%d]=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2221_112_2_18_1_7_28_783,"[BT]FTS_GET_OBJ_IND-state = 0x%X, oper_state=0x%X, Attribute=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2224_112_2_18_1_7_28_784,"[BT]FTS_PUT_OBJ_IND:Opp is busy now, could not respond put object\r\n")
TRACE_MSG(PF_FTP_SERVER_2237_112_2_18_1_7_28_785,"[BT]FTS_HandlePrim:FTS_GET_OBJ_IND,One file is transmitting.......\r\n")
TRACE_MSG(PF_FTP_SERVER_2262_112_2_18_1_7_28_786,"[BT]FTS_GET_OBJ_IND:name[%d]=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2303_112_2_18_1_7_28_787,"[BT]FTS_GET_OBJ_IND: File Open Error\r\n")
TRACE_MSG(PF_FTP_SERVER_2314_112_2_18_1_7_28_788,"[BT]FTS_GET_OBJ_IND: File name is too long\r\n")
TRACE_MSG(PF_FTP_SERVER_2338_112_2_18_1_7_28_789,"[BT]FTS_GET_OBJ_NEXT_IND-state = 0x%X, oper_state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2345_112_2_18_1_7_28_790,"[BT]FTS_GET_OBJ_NEXT_IND:Cancel=0x%X, handle=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2365_112_2_18_1_7_28_791,"[BT]FTS_GET_OBJ_NEXT_IND:bytesToRead=%d\n")
TRACE_MSG(PF_FTP_SERVER_2370_112_2_18_1_7_28_792,"[BT]FTS_GET_OBJ_NEXT_IND:File read error\r\n")
TRACE_MSG(PF_FTP_SERVER_2456_112_2_18_1_7_28_793,"[DRV_BT]<ASSERT_ERROR>: FTS_AUTHENTICATE_IND ")
TRACE_MSG(PF_FTP_SERVER_2462_112_2_18_1_7_28_794,"[DRV_BT]<ASSERT_ERROR>: FTS_AUTHENTICATE_CFM")
TRACE_MSG(PF_FTP_SERVER_2470_112_2_18_1_7_28_795,"[BT]FTS_ABORT_IND:state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2501_112_2_18_1_7_29_796,"[BT]FTS_ABORT_IND: Invail Operate\r\n")
TRACE_MSG(PF_FTP_SERVER_2521_112_2_18_1_7_29_797,"[BT]FTS_DISCONNECT_IND:state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2555_112_2_18_1_7_29_798,"[BT]FTS_DISCONNECT_IND:ID_STATUS_FT_SERVER_ABORT_IND\r\n")
TRACE_MSG(PF_FTP_SERVER_2582_112_2_18_1_7_29_799,"[BT]FTS_DEACTIVATE_CFM:Fts Server Deactivated...\n")
TRACE_MSG(PF_FTP_SERVER_2594_112_2_18_1_7_29_800,"[BT]FTS_HandlePrim:FTS_DEL_OBJ_IND:state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2638_112_2_18_1_7_29_801,"[BT]FTS_DEL_OBJ_IND: Invaild operation\r\n")
TRACE_MSG(PF_FTP_SERVER_2645_112_2_18_1_7_29_802,"[BT]FTS_DEL_OBJ_IND:is_del=0x%X, cancelputting=0x%X, cancelgetting=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2660_112_2_18_1_7_29_803,"[BT]FTS_DEL_OBJ_IND: FTP Read Only!\r\n")
TRACE_MSG(PF_FTP_SERVER_2693_112_2_18_1_7_29_804,"[BT]FTS_DEL_OBJ_IND: it is not an empty dir, can not del directly!\r\n")
TRACE_MSG(PF_FTP_SERVER_2720_112_2_18_1_7_29_805,"[BT]FTS_DEL_OBJ_IND: File Not Exist!\r\n")
TRACE_MSG(PF_FTP_SERVER_2744_112_2_18_1_7_29_806,"[BT]FTS_SET_FOLDER_IND\r\n")
TRACE_MSG(PF_FTP_SERVER_2749_112_2_18_1_7_29_807,"w_len=%d,w_name_len=%d")
TRACE_MSG(PF_FTP_SERVER_2754_112_2_18_1_7_29_808,"folder length  over 255")
TRACE_MSG(PF_FTP_SERVER_2768_112_2_18_1_7_29_809,"[BT]FTS_SET_FOLDER_IND:The specified directory is getted\r\n")
TRACE_MSG(PF_FTP_SERVER_2784_112_2_18_1_7_29_810,"w_len=%d folder length  over 256")
TRACE_MSG(PF_FTP_SERVER_2795_112_2_18_1_7_29_811,"[BT]FTS_SET_FOLDER_IND:Name[%d]=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2803_112_2_18_1_7_29_812,"[BT]FTS_SET_FOLDER_IND:No this path\r\n")
TRACE_MSG(PF_FTP_SERVER_2825_112_2_18_1_7_29_813,"[BT]FTS_SET_BACK_FOLDER_IND:len=0x%X, root_len:0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2829_112_2_18_1_7_29_814,"[BT]FTS_SET_BACK_FOLDER_IND: w_len=%d,w_root_len=%d\r\n")
TRACE_MSG(PF_FTP_SERVER_2864_112_2_18_1_7_29_815,"[BT]FTS_SET_BACK_FOLDER_IND:path[%d]=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_2871_112_2_18_1_7_29_816,"[BT]FTS_SET_BACK_FOLDER_IND: Not Found Folder\r\n")
TRACE_MSG(PF_FTP_SERVER_2885_112_2_18_1_7_29_817,"w_len=:%d")
TRACE_MSG(PF_FTP_SERVER_2888_112_2_18_1_7_29_818,"[DRV_BT]<ASSERT_ERROR> w_len exceeded !!!")
TRACE_MSG(PF_FTP_SERVER_2897_112_2_18_1_7_29_819,"[BT:FTS] FTS_GET_LIST_FOLDER_IND:payload is NULL\r\n")
TRACE_MSG(PF_FTP_SERVER_2905_112_2_18_1_7_29_820,"[BT:FTS] FTS_GET_LIST_FOLDER_IND:payload is not NULL\r\n")
TRACE_MSG(PF_FTP_SERVER_2927_112_2_18_1_7_29_821,"[BT]FTS_GET_LIST_FOLDER_IND:len = %d maxPacketSize=%d byte_to_send = %d\r\n")
TRACE_MSG(PF_FTP_SERVER_2950_112_2_18_1_7_29_822,"[BT]FTS_GET_LIST_FOLDER_IND:error path does not exists\n")
TRACE_MSG(PF_FTP_SERVER_2972_112_2_18_1_7_29_823,"[BT]FTS_GET_LIST_FOLDER_NEXT_IND bytesToRead=%d folder_string_pos=%d\n")
TRACE_MSG(PF_FTP_SERVER_2996_112_2_18_1_7_30_824,"[BT]FTS_GET_LIST_FOLDER_NEXT_IND:error path does not exists\n")
TRACE_MSG(PF_FTP_SERVER_3004_112_2_18_1_7_30_825,"[BT]FTS_SET_ADD_FOLDER_IND\r\n")
TRACE_MSG(PF_FTP_SERVER_3015_112_2_18_1_7_30_826,"[BT]FTS_SET_ADD_FOLDER_IND: FTP Read Only!\r\n")
TRACE_MSG(PF_FTP_SERVER_3022_112_2_18_1_7_30_827,"[BT]FTS_SET_ADD_FOLDER_IND: Parent folder is not exist!\r\n")
TRACE_MSG(PF_FTP_SERVER_3038_112_2_18_1_7_30_828,"[BT]FTS_SET_ADD_FOLDER_IND:Name[%d]=0x%04X\r\n")
TRACE_MSG(PF_FTP_SERVER_3066_112_2_18_1_7_30_829,"[BT]FTS_SET_ADD_FOLDER_IND:Error, The Name is same as One File \r\n")
TRACE_MSG(PF_FTP_SERVER_3072_112_2_18_1_7_30_830,"[BT]FTS_SET_ADD_FOLDER_IND: folder name is too long\r\n")
TRACE_MSG(PF_FTP_SERVER_3100_112_2_18_1_7_30_831,"[BT]FTS_SET_ROOT_FOLDER_IND\r\n")
TRACE_MSG(PF_FTP_SERVER_3108_112_2_18_1_7_30_832,"[BT]FTS_SET_ROOT_FOLDER_IND: Get File TimeOut: state=0x%X, operate_state=0x%X\r\n")
TRACE_MSG(PF_FTP_SERVER_3128_112_2_18_1_7_30_833,"[DRV_BT]<ASSERT_ERROR>: FTS_HandlePrim: unkown type")
TRACE_MSG(PF_FTP_SERVER_3235_112_2_18_1_7_30_834,"[DRV_BT]<ASSERT_ERROR>: FTS_DeInitHandlePrim: unkown type")
TRACE_MSG(PF_HFG_208_112_2_18_1_7_31_835,"[BT]<INFO> HFG_AudioReqSend(): audio_on = %d, new_audio = %d ")
TRACE_MSG(PF_HFG_252_112_2_18_1_7_31_836,"[BT]<INFO> HFG_InitAudioConnection(): init audio link...")
TRACE_MSG(PF_HFG_477_112_2_18_1_7_31_837,"[_HFG_HandleATCmd] token = 0x%x")
TRACE_MSG(PF_HFG_503_112_2_18_1_7_31_838,"[ATD>] location = %s")
TRACE_MSG(PF_HFG_512_112_2_18_1_7_31_839,"[ATD>] memory_indicator = %d")
TRACE_MSG(PF_HFG_517_112_2_18_1_7_31_840,"[ATD] phone number = %s")
TRACE_MSG(PF_HFG_533_112_2_18_1_7_32_841,"[ATD] phone_number = %s")
TRACE_MSG(PF_HFG_546_112_2_18_1_7_32_842,"[CCWA_TOKEN] value = %d")
TRACE_MSG(PF_HFG_556_112_2_18_1_7_32_843,"[VTS_TOKEN] value = 0x%x")
TRACE_MSG(PF_HFG_567_112_2_18_1_7_32_844,"[VTS_TOKEN] dtmf = 0x%x")
TRACE_MSG(PF_HFG_586_112_2_18_1_7_32_845,"[HFG_CNUM_IND] cnum = %s")
TRACE_MSG(PF_HFG_607_112_2_18_1_7_32_846,"[HFG_Activate] IN")
TRACE_MSG(PF_HFG_608_112_2_18_1_7_32_847,"[HFG_Activate] status = %d")
TRACE_MSG(PF_HFG_613_112_2_18_1_7_32_848,"[DRV_BT]<ASSERT_ERROR>: HFG_Activate WRONG status")
TRACE_MSG(PF_HFG_641_112_2_18_1_7_32_849,"[BT][HFG_ServiceClose] IN")
TRACE_MSG(PF_HFG_642_112_2_18_1_7_32_850,"[BT][HFG_ServiceClose] status = %d")
TRACE_MSG(PF_HFG_643_112_2_18_1_7_32_851,"[BT][HFG_ServiceClose] audio_on = %d")
TRACE_MSG(PF_HFG_644_112_2_18_1_7_32_852,"[BT][HFG_ServiceClose] conn_id = %d")
TRACE_MSG(PF_HFG_665_112_2_18_1_7_32_853,"[HFG_Deactivate] IN")
TRACE_MSG(PF_HFG_666_112_2_18_1_7_32_854,"[HFG_Deactivate] status = %d")
TRACE_MSG(PF_HFG_667_112_2_18_1_7_32_855,"[HFG_Deactivate] audio_on = %d")
TRACE_MSG(PF_HFG_668_112_2_18_1_7_32_856,"[HFG_Deactivate] conn_id = %d")
TRACE_MSG(PF_HFG_726_112_2_18_1_7_32_857,"[HFG_ConnectHf] Time Out ,Acitive error")
TRACE_MSG(PF_HFG_755_112_2_18_1_7_32_858,"[HFG_ConnectHf] IN")
TRACE_MSG(PF_HFG_756_112_2_18_1_7_32_859,"[HFG_ConnectHf] status = %d")
TRACE_MSG(PF_HFG_757_112_2_18_1_7_32_860,"[HFG_ConnectHf] conn_id = 0x%x")
TRACE_MSG(PF_HFG_762_112_2_18_1_7_32_861,"[HFG_ConnectHf] one headset has been activated")
TRACE_MSG(PF_HFG_788_112_2_18_1_7_32_862,"[HFG]timed_event_in fail")
TRACE_MSG(PF_HFG_797_112_2_18_1_7_32_863,"[HFG_CancelConnect] IN")
TRACE_MSG(PF_HFG_798_112_2_18_1_7_32_864,"[HFG_CancelConnect] status = %d")
TRACE_MSG(PF_HFG_814_112_2_18_1_7_32_865,"[HFG_DisconnectHf] IN")
TRACE_MSG(PF_HFG_815_112_2_18_1_7_32_866,"[HFG_DisconnectHf] status = %d")
TRACE_MSG(PF_HFG_816_112_2_18_1_7_32_867,"[HFG_DisconnectHf] conn_id = 0x%x")
TRACE_MSG(PF_HFG_825_112_2_18_1_7_32_868,"[HFG_DisconnectHf] No activated headset")
TRACE_MSG(PF_HFG_837_112_2_18_1_7_32_869,"[HFG]timed_event_in fail")
TRACE_MSG(PF_HFG_847_112_2_18_1_7_32_870,"[HFG_GetConnectedHf] IN")
TRACE_MSG(PF_HFG_848_112_2_18_1_7_32_871,"[HFG_GetConnectedHf] conn_id = 0x%x")
TRACE_MSG(PF_HFG_849_112_2_18_1_7_32_872,"[HFG_GetConnectedHf] status = %d")
TRACE_MSG(PF_HFG_868_112_2_18_1_7_32_873,"[HFG_Dialing] IN")
TRACE_MSG(PF_HFG_869_112_2_18_1_7_32_874,"[HFG_Dialing] conn_id = 0x%x")
TRACE_MSG(PF_HFG_870_112_2_18_1_7_32_875,"[HFG_Dialing] active_call_num = %d")
TRACE_MSG(PF_HFG_871_112_2_18_1_7_32_876,"[HFG_Dialing] audio_on = %d, Reqsend = %d")
TRACE_MSG(PF_HFG_872_112_2_18_1_7_32_877,"[HFG_Dialing] status = %d")
TRACE_MSG(PF_HFG_894_112_2_18_1_7_32_878,"[HFG_Alerting] IN")
TRACE_MSG(PF_HFG_895_112_2_18_1_7_32_879,"[HFG_Alerting] conn_id = 0x%x")
TRACE_MSG(PF_HFG_896_112_2_18_1_7_32_880,"[HFG_Alerting] active_call_num = %d")
TRACE_MSG(PF_HFG_897_112_2_18_1_7_32_881,"[HFG_Alerting] audio_on = %d, Reqsend=%d")
TRACE_MSG(PF_HFG_898_112_2_18_1_7_32_882,"[HFG_Alerting] status = %d")
TRACE_MSG(PF_HFG_911_112_2_18_1_7_32_883,"[HFG_Incoming] IN (phone_num = %s)")
TRACE_MSG(PF_HFG_913_112_2_18_1_7_32_884,"[HFG_Incoming] conn_id = 0x%x")
TRACE_MSG(PF_HFG_914_112_2_18_1_7_32_885,"[HFG_Incoming] ccwa_notification = %d")
TRACE_MSG(PF_HFG_915_112_2_18_1_7_32_886,"[HFG_Incoming] active_call_num = %d")
TRACE_MSG(PF_HFG_916_112_2_18_1_7_32_887,"[HFG_Incoming] status = %d")
TRACE_MSG(PF_HFG_929_112_2_18_1_7_32_888,"[HFG_Incoming] The second call")
TRACE_MSG(PF_HFG_934_112_2_18_1_7_32_889,"[HFG_Incoming] The headset supports three-way call, send CCWA")
TRACE_MSG(PF_HFG_958_112_2_18_1_7_32_890,"[HFG_Incoming] The first call, send RING & CLIP")
TRACE_MSG(PF_HFG_989_112_2_18_1_7_33_891,"[HFG_IndicatorSetupNoCall]:call_status:%d,%d")
TRACE_MSG(PF_HFG_1006_112_2_18_1_7_33_892,"[HFG_CallConnected] IN")
TRACE_MSG(PF_HFG_1007_112_2_18_1_7_33_893,"[HFG_CallConnected] conn_id = 0x%x")
TRACE_MSG(PF_HFG_1008_112_2_18_1_7_33_894,"[HFG_CallConnected] active_call_num = %d")
TRACE_MSG(PF_HFG_1009_112_2_18_1_7_33_895,"[HFG_CallConnected] audio_on = %d")
TRACE_MSG(PF_HFG_1010_112_2_18_1_7_33_896,"[HFG_CallConnected] status = %d")
TRACE_MSG(PF_HFG_1036_112_2_18_1_7_33_897,"[HFG_CallDisconnected] IN (call_status = %d)")
TRACE_MSG(PF_HFG_1037_112_2_18_1_7_33_898,"[HFG_CallDisconnected] conn_id = 0x%x")
TRACE_MSG(PF_HFG_1038_112_2_18_1_7_33_899,"[HFG_CallDisconnected] active_call_num = %d")
TRACE_MSG(PF_HFG_1039_112_2_18_1_7_33_900,"[HFG_CallDisconnected] audio_on = %d")
TRACE_MSG(PF_HFG_1040_112_2_18_1_7_33_901,"[HFG_CallDisconnected] status = %d")
TRACE_MSG(PF_HFG_1093_112_2_18_1_7_33_902,"[HFG_SetSpeakerGain] status = %d")
TRACE_MSG(PF_HFG_1110_112_2_18_1_7_33_903,"[HFG_SetSpeakerGain] sys_vol = %d, hfg_vol = %d")
TRACE_MSG(PF_HFG_1118_112_2_18_1_7_33_904,"[HFG_SetMicGain] status = %d")
TRACE_MSG(PF_HFG_1134_112_2_18_1_7_33_905,"[HFG_SetMicGain] sys_vol = %d, hfg_vol = %d")
TRACE_MSG(PF_HFG_1142_112_2_18_1_7_33_906,"[HFG_GetSpeakerGain] IN")
TRACE_MSG(PF_HFG_1143_112_2_18_1_7_33_907,"[HFG_GetSpeakerGain] status = %d")
TRACE_MSG(PF_HFG_1144_112_2_18_1_7_33_908,"[HFG_GetSpeakerGain] spk_gain = %d")
TRACE_MSG(PF_HFG_1152_112_2_18_1_7_33_909,"[HFG_GetMicGain] IN")
TRACE_MSG(PF_HFG_1153_112_2_18_1_7_33_910,"[HFG_GetMicGain] status = %d")
TRACE_MSG(PF_HFG_1154_112_2_18_1_7_33_911,"[HFG_GetMicGain] mic_gain = %d")
TRACE_MSG(PF_HFG_1163_112_2_18_1_7_33_912,"[HFG_SwitchAudio] IN (on = %d)")
TRACE_MSG(PF_HFG_1164_112_2_18_1_7_33_913,"[HFG_SwitchAudio] audio_on = %d")
TRACE_MSG(PF_HFG_1165_112_2_18_1_7_33_914,"[HFG_SwitchAudio] status = %d")
TRACE_MSG(PF_HFG_1166_112_2_18_1_7_33_915,"[HFG_SwitchAudio] conn_id = %d")
TRACE_MSG(PF_HFG_1184_112_2_18_1_7_33_916,"[HFG_SwitchAudio] IN hf_entry.audio_on switch_debug")
TRACE_MSG(PF_HFG_1205_112_2_18_1_7_33_917,"[HFG_SwitchAudio] here ")
TRACE_MSG(PF_HFG_1227_112_2_18_1_7_33_918,"[HFG]g_hfgtimerid exist: g_hfgtimerid = %x")
TRACE_MSG(PF_HFG_1235_112_2_18_1_7_33_919,"[HFG]timed_event_in fail")
TRACE_MSG(PF_HFG_1244_112_2_18_1_7_33_920,"[HFG_SwitchAudioFormusic] IN (on = %d)")
TRACE_MSG(PF_HFG_1254_112_2_18_1_7_33_921,"[HFG_AudioStart] audioOn = %d")
TRACE_MSG(PF_HFG_1255_112_2_18_1_7_33_922,"[HFG_AudioStart]conn_id=0x%X\r\n")
TRACE_MSG(PF_HFG_1256_112_2_18_1_7_33_923,"[HFG_AudioStart]BluePlay=0x%X\r\n")
TRACE_MSG(PF_HFG_1257_112_2_18_1_7_33_924,"[HFG_AudioStart]hBlueRec=0x%X\r\n")
TRACE_MSG(PF_HFG_1271_112_2_18_1_7_33_925,"[HFG_AudioStart] audio is off and audio start fails!\n")
TRACE_MSG(PF_HFG_1282_112_2_18_1_7_33_926,"[DRV_BT]<WARNING> HFG_AudioStart: buffer is NOT freed properly!\n")
TRACE_MSG(PF_HFG_1295_112_2_18_1_7_33_927,"[DRV_BT]<WARNING> HFG_AudioStart: Handle is NOT freed properly!\n")
TRACE_MSG(PF_HFG_1302_112_2_18_1_7_33_928,"[HFG_AudioStart]AUDIO_Play:result=0x%X\r\n")
TRACE_MSG(PF_HFG_1309_112_2_18_1_7_33_929,"[HFG_AudioStart]AUDIO_StartRecord:result=0x%X\r\n")
TRACE_MSG(PF_HFG_1328_112_2_18_1_7_33_930,"[BT]<INFO> HFG_AudioStop() conn_id = 0x%X, active_call_num = %d, ")
TRACE_MSG(PF_HFG_1330_112_2_18_1_7_33_931,"[BT]<INFO> HFG_AudioStop() hBluePlay= 0x%X, hBlueRec= 0x%X")
TRACE_MSG(PF_HFG_1347_112_2_18_1_7_33_932,"[HFG_AudioStop] audio is on and audio stop fails!\n")
TRACE_MSG(PF_HFG_1358_112_2_18_1_7_33_933,"[HFG_AudioStop]AUDIO_Stop:hBluePlay result=0x%X\r\n")
TRACE_MSG(PF_HFG_1365_112_2_18_1_7_33_934,"[HFG_AudioStop]AUDIO_StopRecord:hBlueRec result=0x%X\r\n")
TRACE_MSG(PF_HFG_1371_112_2_18_1_7_33_935,"[HFG_AudioStop]AUDIO_CloseHandle:hBluePlay result=0x%X\r\n")
TRACE_MSG(PF_HFG_1377_112_2_18_1_7_33_936,"[HFG_AudioStop]AUDIO_CloseHandle:hBlueRec result=0x%X\r\n")
TRACE_MSG(PF_HFG_1385_112_2_18_1_7_33_937,"[HFG_AudioStop]AUDIO_Stop:ret=  %x\r\n")
TRACE_MSG(PF_HFG_1401_112_2_18_1_7_33_938,"[HFG_SetStatusIndicator] IN (service_status = %d, active_calls = %d, call_setup_status = %d)")
TRACE_MSG(PF_HFG_1402_112_2_18_1_7_33_939,"[HFG_SetStatusIndicator] conn_id = %d")
TRACE_MSG(PF_HFG_1403_112_2_18_1_7_33_940,"[HFG_SetStatusIndicator] audio_on = %d")
TRACE_MSG(PF_HFG_1405_112_2_18_1_7_33_941,"[HFG_SetStatusIndicator] active_call_num = %d")
TRACE_MSG(PF_HFG_1406_112_2_18_1_7_33_942,"[HFG_SetStatusIndicator] call_setup = %d")
TRACE_MSG(PF_HFG_1407_112_2_18_1_7_33_943,"[HFG_SetStatusIndicator] status = %d")
TRACE_MSG(PF_HFG_1408_112_2_18_1_7_33_944,"[HFG_SetStatusIndicator] hold_call = %d")
TRACE_MSG(PF_HFG_1443_112_2_18_1_7_33_945,"[HFG_TO_A2DP_TimeOutHandle] hf.addr = %04x:%02x:%06x")
TRACE_MSG(PF_HFG_1464_112_2_18_1_7_33_946,"[HFG_HandlePrim] IN")
TRACE_MSG(PF_HFG_1473_112_2_18_1_7_33_947,"[HFG_HandlePrim] prim_type = 0x%x")
TRACE_MSG(PF_HFG_1478_112_2_18_1_7_33_948,"[HFG_DEACTIVATE_CFM]")
TRACE_MSG(PF_HFG_1479_112_2_18_1_7_33_949,"[HFG_DEACTIVATE_CFM] status = %d")
TRACE_MSG(PF_HFG_1499_112_2_18_1_7_34_950,"[HFG_SERVICE_CONNECT_IND] result = %d")
TRACE_MSG(PF_HFG_1520_112_2_18_1_7_34_951,"[HFG_SERVICE_CONNECT_IND] addr = %04x:%02x:%06x")
TRACE_MSG(PF_HFG_1521_112_2_18_1_7_34_952,"[HFG_SERVICE_CONNECT_IND] supportedFeatures = 0x%x")
TRACE_MSG(PF_HFG_1522_112_2_18_1_7_34_953,"[HFG_SERVICE_CONNECT_IND] serviceName = %s")
TRACE_MSG(PF_HFG_1523_112_2_18_1_7_34_954,"[HFG_SERVICE_CONNECT_IND] connectionId = %d")
TRACE_MSG(PF_HFG_1524_112_2_18_1_7_34_955,"[HFG_SERVICE_CONNECT_IND] connectionType = %d")
TRACE_MSG(PF_HFG_1552_112_2_18_1_7_34_956,"[HFG]timed_event_in fail")
TRACE_MSG(PF_HFG_1584_112_2_18_1_7_34_957,"[HFG_SERVICE_CONNECT_IND] one headset has been activated (conn_id = %d)")
TRACE_MSG(PF_HFG_1585_112_2_18_1_7_34_958,"[HFG_SERVICE_CONNECT_IND] prim->connectionId = %d")
TRACE_MSG(PF_HFG_1602_112_2_18_1_7_34_959,"[HFG_DISCONNECT_IND] result = %d")
TRACE_MSG(PF_HFG_1603_112_2_18_1_7_34_960,"[HFG_DISCONNECT_IND] connectionId = %d")
TRACE_MSG(PF_HFG_1656_112_2_18_1_7_34_961,"[HFG_DISCONNECT_IND] Deactivating HFG...")
TRACE_MSG(PF_HFG_1690_112_2_18_1_7_34_962,"[HFG_AUDIO_IND] cancel_timed_event g_hfgtimerid = %d")
TRACE_MSG(PF_HFG_1698_112_2_18_1_7_34_963,"[HFG_AUDIO_IND] audioOn = %d")
TRACE_MSG(PF_HFG_1699_112_2_18_1_7_34_964,"[HFG_AUDIO_IND] scoHandle = 0x%x")
TRACE_MSG(PF_HFG_1700_112_2_18_1_7_34_965,"[HFG_AUDIO_IND] result = %d")
TRACE_MSG(PF_HFG_1701_112_2_18_1_7_34_966,"[HFG_AUDIO_IND] linkType = %d")
TRACE_MSG(PF_HFG_1702_112_2_18_1_7_34_967,"[HFG_AUDIO_IND] g_hfgtimerid = %d")
TRACE_MSG(PF_HFG_1724_112_2_18_1_7_34_968,"[BT]<WARNING> %s L%d: close BT_audioOn if no valid call")
TRACE_MSG(PF_HFG_1745_112_2_18_1_7_34_969,"[HFG_AUDIO_IND] hfg_MusicOrVoice = 0x%x")
TRACE_MSG(PF_HFG_1751_112_2_18_1_7_34_970,"[HFG_AUDIO_IND] hfg_MusicOrVoice = 0x%x")
TRACE_MSG(PF_HFG_1778_112_2_18_1_7_34_971,"[BT]<WARNING> %s L%d: close BT_audioOn if no valid call")
TRACE_MSG(PF_HFG_1816_112_2_18_1_7_34_972,"[HFG_REJECT_IND]")
TRACE_MSG(PF_HFG_1825_112_2_18_1_7_34_973,"[HFG_ANSWER_IND]")
TRACE_MSG(PF_HFG_1833_112_2_18_1_7_34_974,"[HFG_RING_CFM]")
TRACE_MSG(PF_HFG_1842_112_2_18_1_7_34_975,"[HFG_STATUS_IND] statusEvent = %d")
TRACE_MSG(PF_HFG_1852_112_2_18_1_7_34_976,"[HFG_SPEAKER_GAIN_IND] conn_id = %d, gain = %d")
TRACE_MSG(PF_HFG_1877_112_2_18_1_7_34_977,"[HFG_MIC_GAIN_IND] conn_id = %d, gain = %d")
TRACE_MSG(PF_HFG_1900_112_2_18_1_7_34_978,"[HFG_AT_CMD_IND] AT = %s")
TRACE_MSG(PF_HFG_1916_112_2_18_1_7_34_979,"[HFG_CHLD_IND] value = %d")
TRACE_MSG(PF_HFG_1917_112_2_18_1_7_34_980,"[HFG_CHLD_IND] index = %d")
TRACE_MSG(PF_HFG_2000_112_2_18_1_7_34_981,"[HFG_CLCC_IND]")
TRACE_MSG(PF_HFG_2023_112_2_18_1_7_35_982,"[HFG_COPS_IND] CopsStr = %s")
TRACE_MSG(PF_HFG_2032_112_2_18_1_7_35_983,"[HFG_COPS_IND] CopsStr = %s")
TRACE_MSG(PF_HFG_2061_112_2_18_1_7_35_984,"[HFG_CNUM_IND]")
TRACE_MSG(PF_HFG_2072_112_2_18_1_7_35_985,"[HFG_CMEE_IND] command = %c")
TRACE_MSG(PF_HFG_2083_112_2_18_1_7_35_986,"[HFG_BTRH_IND] command = 0x%x")
TRACE_MSG(PF_HFG_2090_112_2_18_1_7_35_987,"[HFG_HandlePrim] un-handled command 0x%x")
TRACE_MSG(PF_HFG_2115_112_2_18_1_7_35_988,"[HFG_DeinitPrim] prim_type = 0x%x")
TRACE_MSG(PF_HFG_2136_112_2_18_1_7_35_989,"[HFG_GetCallStatus] active_call_num = %d")
TRACE_MSG(PF_HFG_2138_112_2_18_1_7_35_990,"[HFG_GetCallStatus] call_setup = %d")
TRACE_MSG(PF_HFG_2149_112_2_18_1_7_35_991,"[HFG_GetScoHandle] scoHandle = 0x%x")
TRACE_MSG(PF_HFG_2181_112_2_18_1_7_35_992,"HFG_IndicatorCIEV Status:param=0x%x,%x")
TRACE_MSG(PF_HFG_2206_112_2_18_1_7_35_993,"HFG_IndicatorCIEV Status End")
TRACE_MSG(PF_HFG_2218_112_2_18_1_7_35_994,"idx=%d,dir=%d,status=%d,mode=%d,mpty=%d,number=%s")
TRACE_MSG(PF_HFG_2238_112_2_18_1_7_35_995,"[HFG_SetNameOfNetOperator] net_operator_name = %s")
TRACE_MSG(PF_HFG_2257_112_2_18_1_7_35_996,"[HFG_SetLocalNumber] local_number = %s")
TRACE_MSG(PF_OPP_CLIENT_56_112_2_18_1_7_35_997,"[DRV_BT]OPC_CloseRES(in): fileHandle = 0x%X, filename_ptr = 0x%X")
TRACE_MSG(PF_OPP_CLIENT_70_112_2_18_1_7_35_998,"[DRV_BT]OPC_CloseRES(out): fileHandle = 0x%X, filename_ptr = 0x%X")
TRACE_MSG(PF_OPP_CLIENT_75_112_2_18_1_7_35_999,"[BT]OPC_ChangeClk:Change System Clock\r\n")
TRACE_MSG(PF_OPP_CLIENT_83_112_2_18_1_7_35_1000,"[BT]OPC_RestoreClk:Restore System Clock=0x%X\r\n")
TRACE_MSG(PF_OPP_CLIENT_103_112_2_18_1_7_35_1001,"[BT]OPC_ServiceClose:state=0x%X\r\n")
TRACE_MSG(PF_OPP_CLIENT_118_112_2_18_1_7_35_1002,"[BT]OPC_IsBusy:state=0x%X\r\n")
TRACE_MSG(PF_OPP_CLIENT_124_112_2_18_1_7_35_1003,"[BT]OPC_Init\r\n")
TRACE_MSG(PF_OPP_CLIENT_130_112_2_18_1_7_35_1004,"[BT]OPC_DeInit\r\n")
TRACE_MSG(PF_OPP_CLIENT_154_112_2_18_1_7_36_1005,"[BT]OPC_SendFile: Is Busy.....state:=0x%X,fileHandle=0x%xrn")
TRACE_MSG(PF_OPP_CLIENT_160_112_2_18_1_7_36_1006,"[BT]OPC_SendFile\r\n")
TRACE_MSG(PF_OPP_CLIENT_168_112_2_18_1_7_36_1007,"[BT]OPC_SendFile:PF_fopen Error\r\n")
TRACE_MSG(PF_OPP_CLIENT_177_112_2_18_1_7_36_1008,"[DRV_BT]OPC_SendFile(): malloc filename_ptr = 0x%X")
TRACE_MSG(PF_OPP_CLIENT_227_112_2_18_1_7_36_1009,"[BT]<INFO> %s L%d: BT_StartCancelTimer(): OPC Cancel")
TRACE_MSG(PF_OPP_CLIENT_245_112_2_18_1_7_36_1010,"[BT]OPC_Cancel:state=0x%X\r\n")
TRACE_MSG(PF_OPP_CLIENT_265_112_2_18_1_7_36_1011,"[DRV_BT]<ASSERT_ERROR> OPC_Cancel:Invail Cancel")
TRACE_MSG(PF_OPP_CLIENT_321_112_2_18_1_7_36_1012,"[BT]OPC_HandlePrim type = 0x%X\r\n")
TRACE_MSG(PF_OPP_CLIENT_341_112_2_18_1_7_36_1013,"[BT]OPC_HandlePrim:OPC_CONNECT_CFM-responseCode=0x%x\r\n")
TRACE_MSG(PF_OPP_CLIENT_385_112_2_18_1_7_36_1014,"[BT]OPC_HandlePrim:OPP Disconnect:state = 0x%X\r\n")
TRACE_MSG(PF_OPP_CLIENT_394_112_2_18_1_7_36_1015,"[BT]OPC_HandlePrim:Server Send Abort request\r\n")
TRACE_MSG(PF_OPP_CLIENT_428_112_2_18_1_7_36_1016,"[BT]OPC_PUT_OBJECT_IND: state= 0x%X, fileHandle= 0x%X")
TRACE_MSG(PF_OPP_CLIENT_431_112_2_18_1_7_36_1017,"[BT]OPC_PUT_OBJECT_IND:Cancel Send file\r\n")
TRACE_MSG(PF_OPP_CLIENT_438_112_2_18_1_7_36_1018,"[BT]<ERROR>%s L%d: state or fileHandle error")
TRACE_MSG(PF_OPP_CLIENT_456_112_2_18_1_7_36_1019,"[BT]<ERROR>%s L%d: read_len(%d) != ")
TRACE_MSG(PF_OPP_CLIENT_514_112_2_18_1_7_36_1020,"[BT]OPC_PUT_CFM:Cancel Send file\r\n")
TRACE_MSG(PF_OPP_CLIENT_525_112_2_18_1_7_36_1021,"[BT]OPC_PUT_CFM:responseCode=0x%x\r\n")
TRACE_MSG(PF_OPP_CLIENT_552_112_2_18_1_7_36_1022,"[BT]OPC_HandlePrim:OPP abort \r\n")
TRACE_MSG(PF_OPP_CLIENT_570_112_2_18_1_7_36_1023,"[BT]<WARNING> OPC_HandlePrim: NOT supported type\r\n")
TRACE_MSG(PF_OPP_SERVER_62_112_2_18_1_7_37_1024,"[DRV_BT]OPS_CloseRES(in): is_delete = %d, fileHandle = 0x%X, filename_ptr = 0x%X")
TRACE_MSG(PF_OPP_SERVER_80_112_2_18_1_7_37_1025,"[DRV_BT]OPS_CloseRES(out): is_delete = %d, fileHandle = 0x%X, filename_ptr = 0x%X")
TRACE_MSG(PF_OPP_SERVER_85_112_2_18_1_7_37_1026,"[BT]OPS_ChangeClk:Change System Clock:cnt=0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_96_112_2_18_1_7_37_1027,"[BT]OPS_RestoreClk:Restore System Clock=0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_110_112_2_18_1_7_37_1028,"[BT]OPS_Active\r\n")
TRACE_MSG(PF_OPP_SERVER_119_112_2_18_1_7_37_1029,"[BT]OPS_DeActive\r\n")
TRACE_MSG(PF_OPP_SERVER_129_112_2_18_1_7_37_1030,"[BT][OPS_ServiceClose] IN")
TRACE_MSG(PF_OPP_SERVER_130_112_2_18_1_7_37_1031,"[BT][OPS_ServiceClose] status = %d")
TRACE_MSG(PF_OPP_SERVER_148_112_2_18_1_7_37_1032,"[BT]OPS_IsBusy:state=0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_154_112_2_18_1_7_37_1033,"[BT]OPS_Init\r\n")
TRACE_MSG(PF_OPP_SERVER_160_112_2_18_1_7_37_1034,"[BT]OPS_DeInit\r\n")
TRACE_MSG(PF_OPP_SERVER_180_112_2_18_1_7_37_1035,"[BT]OPS_Connect is_enable = %d")
TRACE_MSG(PF_OPP_SERVER_199_112_2_18_1_7_37_1036,"[BT]OPS_CancelReceive:state = 0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_224_112_2_18_1_7_37_1037,"[DRV_BT]<ASSERT_ERROR> OPS_CancelReceive:Invaild Cancel Receive\r\n")
TRACE_MSG(PF_OPP_SERVER_233_112_2_18_1_7_37_1038,"[BT]OPS_ReceiveFileFail\r\n")
TRACE_MSG(PF_OPP_SERVER_248_112_2_18_1_7_37_1039,"[BT]OPS_ReceiveFileFinalPacket\r\n")
TRACE_MSG(PF_OPP_SERVER_278_112_2_18_1_7_37_1040,"[BT]OPS_ReceiveFile\r\n")
TRACE_MSG(PF_OPP_SERVER_282_112_2_18_1_7_37_1041,"[BT] OPS_ReceiveFile error state=%d")
TRACE_MSG(PF_OPP_SERVER_296_112_2_18_1_7_37_1042,"[BT]OPS_ReceiveFile:w_name_len =%d\r\n")
TRACE_MSG(PF_OPP_SERVER_302_112_2_18_1_7_37_1043,"[BT]OPS_ReceiveFile:File Handle = 0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_310_112_2_18_1_7_37_1044,"[DRV_BT]OPS_ReceiveFile(): malloc file_name_ptr = 0x%X")
TRACE_MSG(PF_OPP_SERVER_318_112_2_18_1_7_37_1045,"[BT]OPS_ReceiveFile:File write error, source len=0x%X, write len=0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_343_112_2_18_1_7_37_1046,"[BT]OPS_ReceiveFile: finaFlag=0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_395_112_2_18_1_7_37_1047,"[BT]OPS_HandlePrim:type = 0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_404_112_2_18_1_7_37_1048,"[BT]OPS_HandlePrim:opp client connected\r\n")
TRACE_MSG(PF_OPP_SERVER_408_112_2_18_1_7_37_1049,"[BT]OPS_CONNECT_IND:Ftp is busy now, could not respond connect request\r\n")
TRACE_MSG(PF_OPP_SERVER_447_112_2_18_1_7_37_1050,"[BT]OPS_CONNECT_IND:Received Too Many Connect Commands\r\n")
TRACE_MSG(PF_OPP_SERVER_458_112_2_18_1_7_37_1051,"[BT]OPS_HandlePrim:OPS_PUT_IND\r\n")
TRACE_MSG(PF_OPP_SERVER_462_112_2_18_1_7_37_1052,"[BT]OPS_CONNECT_IND:Ftp is busy now, could not respond put object\r\n")
TRACE_MSG(PF_OPP_SERVER_475_112_2_18_1_7_37_1053,"[BT]OPS_HandlePrim:OPS_PUT_IND,One file is transmitting.......\r\n")
TRACE_MSG(PF_OPP_SERVER_503_112_2_18_1_7_38_1054,"[BT]:bad w_name_len %d")
TRACE_MSG(PF_OPP_SERVER_519_112_2_18_1_7_38_1055,"[BT]OPS_PUT_IND:name[%d]=0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_571_112_2_18_1_7_38_1056,"[BT]OPS_HandlePrim:OPS_PUT_NEXT_IND: ")
TRACE_MSG(PF_OPP_SERVER_583_112_2_18_1_7_38_1057,"[BT]<ERROR>%s L%d: state or file_handler error")
TRACE_MSG(PF_OPP_SERVER_661_112_2_18_1_7_38_1058,"[BT]OPS_ABORT_IND\r\n")
TRACE_MSG(PF_OPP_SERVER_687_112_2_18_1_7_38_1059,"[BT]OPS_DISCONNECT_IND:state=0x%X\r\n")
TRACE_MSG(PF_OPP_SERVER_712_112_2_18_1_7_38_1060,"[BT]OPS_DEACTIVATE_CFM\r\n")
TRACE_MSG(PF_OPP_SERVER_730_112_2_18_1_7_38_1061,"[BT]PF_DeInitOpsEvent\r\n")
TRACE_MSG(PF_OPP_SERVER_769_112_2_18_1_7_38_1062,"[DRV_BT]<ASSERT_ERROR>: OPS_DeInitHandlePrim: unkown type")
TRACE_MSG(PF_SD_18_112_2_18_1_7_38_1063,"[BT]SD_ReadServiceTimeOutHandle:Time out\r\n")
TRACE_MSG(PF_SD_29_112_2_18_1_7_38_1064,"[BT]SD_ReadAvailableServerReq\r\n")
TRACE_MSG(PF_SD_67_112_2_18_1_7_38_1065,"[BT]SD_HandlePrim:type = 0x%X\r\n")
TRACE_MSG(PF_SD_76_112_2_18_1_7_38_1066,"[BT]SD_READ_DEVICE_PROPERTIES_CFM:result = 0x%X\r\n")
TRACE_MSG(PF_SD_101_112_2_18_1_7_38_1067,"[BT]SD_READ_AVAILABLE_SERVICES_CFM\r\n")
TRACE_MSG(PF_SD_102_112_2_18_1_7_38_1068,"result                = %d\n")
TRACE_MSG(PF_SD_103_112_2_18_1_7_38_1069,"remoteNameChanged     = %d\n")
TRACE_MSG(PF_SD_107_112_2_18_1_7_38_1070,"deviceAddr            = %04x:%02x:%06x")
TRACE_MSG(PF_SD_109_112_2_18_1_7_38_1071,"friendlyName          = %s")
TRACE_MSG(PF_SD_111_112_2_18_1_7_38_1072,"classOfDevice         = 0x%x")
TRACE_MSG(PF_SD_113_112_2_18_1_7_38_1073,"knownServices11_00_31 = 0x%08xn")
TRACE_MSG(PF_SD_114_112_2_18_1_7_38_1074,"s_is_refresh_server   = 0x%x\n")
TRACE_MSG(PF_SD_189_112_2_18_1_7_39_1075,"[DRV_BT]<ASSERT_ERROR>: SD_HandlePrim: unkown type")
TRACE_MSG(DUN_GW_API_78_112_2_18_1_7_39_1076,"DG_SwitchUserPort phy_port = %d\n")
TRACE_MSG(DUN_GW_API_258_112_2_18_1_7_39_1077,"[BT]DG_AcceptDunReq!\n")
TRACE_MSG(DUN_GW_API_283_112_2_18_1_7_39_1078,"[BT]DG_RejectDunReq!\n")
TRACE_MSG(DUN_GW_API_358_112_2_18_1_7_40_1079,"DG_PortBusy: port_busy = %d\n")
TRACE_MSG(PF_DUN_GW_105_112_2_18_1_7_40_1080,"[BT]DG_Activate app_handle = %d\n")
TRACE_MSG(PF_DUN_GW_129_112_2_18_1_7_40_1081,"[BT]DgDeactivateReqSend\n")
TRACE_MSG(PF_DUN_GW_166_112_2_18_1_7_40_1082,"[BT]DG_Disconnect\n")
TRACE_MSG(PF_DUN_GW_236_112_2_18_1_7_40_1083,"[BT]DG_dataResSend\n")
TRACE_MSG(PF_DUN_GW_259_112_2_18_1_7_40_1084,"[BT]DG_dataReqSend thePayload = 0x%08x, PayloadLength = %d\n")
TRACE_MSG(PF_DUN_GW_304_112_2_18_1_7_40_1085,"[BT]DG_controlReqSend\n")
TRACE_MSG(PF_DUN_GW_330_112_2_18_1_7_40_1086,"result= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_332_112_2_18_1_7_41_1087,"muxId= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_333_112_2_18_1_7_41_1088,"serverChannel= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_334_112_2_18_1_7_41_1089,"profileMaxFrameSize= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_356_112_2_18_1_7_41_1090,"result= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_358_112_2_18_1_7_41_1091,"muxId= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_359_112_2_18_1_7_41_1092,"serverChannel= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_395_112_2_18_1_7_41_1093,"muxId= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_396_112_2_18_1_7_41_1094,"serverChannel= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_397_112_2_18_1_7_41_1095,"payloadLength= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_398_112_2_18_1_7_41_1096,"payload= 0x%08x\n")
TRACE_MSG(PF_DUN_GW_431_112_2_18_1_7_41_1097,"[BT] DG_handleDataInd the DG COM has not opened when ecieve the DATA_IND.\n ")
TRACE_MSG(PF_DUN_GW_460_112_2_18_1_7_41_1098,"muxId= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_461_112_2_18_1_7_41_1099,"serverChannel= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_489_112_2_18_1_7_41_1100,"muxId= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_490_112_2_18_1_7_41_1101,"serverChannel= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_491_112_2_18_1_7_41_1102,"modemstatus= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_492_112_2_18_1_7_41_1103,"break_signal= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_493_112_2_18_1_7_41_1104,"tx_en= 0x%02x peer_dev_rx_ready= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_517_112_2_18_1_7_41_1105,"[BT]DG_handleControlInd. Peer device RTR and RTC is not ready!\n")
TRACE_MSG(PF_DUN_GW_538_112_2_18_1_7_41_1106,"serverChannel= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_539_112_2_18_1_7_41_1107,"muxId= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_540_112_2_18_1_7_41_1108,"request= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_546_112_2_18_1_7_41_1109,"port_speed= 0x%04x, data_b= 0x%04x, stop_b= 0x%04x, ")
TRACE_MSG(PF_DUN_GW_587_112_2_18_1_7_41_1110,"muxId= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_588_112_2_18_1_7_41_1111,"serverChannel= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_589_112_2_18_1_7_41_1112,"connect= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_590_112_2_18_1_7_41_1113,"maxMsgSize= 0x%04x\n")
TRACE_MSG(PF_DUN_GW_634_112_2_18_1_7_41_1114,"result= 0x%02x\n")
TRACE_MSG(PF_DUN_GW_695_112_2_18_1_7_41_1115,"[BT]DG_handleDeinitPrim msg = 0x%08x\n")
TRACE_MSG(PF_DUN_GW_730_112_2_18_1_7_41_1116,"[BT]DG_handlePrim type = 0x%X\n")
TRACE_MSG(PF_DUN_GW_735_112_2_18_1_7_41_1117,"[BT]DG_CONNECT_IND\n")
TRACE_MSG(PF_DUN_GW_740_112_2_18_1_7_41_1118,"[BT]DG_DATA_IND\n")
TRACE_MSG(PF_DUN_GW_745_112_2_18_1_7_41_1119,"[BT]DG_DATA_CFM\n")
TRACE_MSG(PF_DUN_GW_750_112_2_18_1_7_41_1120,"[BT]DG_CONTROL_IND\n")
TRACE_MSG(PF_DUN_GW_755_112_2_18_1_7_41_1121,"[BT]DG_PORTNEG_IND\n")
TRACE_MSG(PF_DUN_GW_760_112_2_18_1_7_41_1122,"[BT]DG_STATUS_IND\n")
TRACE_MSG(PF_DUN_GW_765_112_2_18_1_7_41_1123,"[BT]DG_DISCONNECT_IND\n")
TRACE_MSG(PF_DUN_GW_770_112_2_18_1_7_41_1124,"[BT]DG_DATA_PATH_STATUS_IND\n")
TRACE_MSG(PF_DUN_GW_775_112_2_18_1_7_41_1125,"[BT]DG_DEACTIVATE_CFM\n")
TRACE_MSG(PF_DUN_GW_780_112_2_18_1_7_41_1126,"[BT]DG_REGISTER_DATA_PATH_HANDLE_CFM\n")
TRACE_MSG(PF_DUN_GW_838_112_2_18_1_7_41_1127,"[BT]DGCOM_Initialize\n")
TRACE_MSG(PF_DUN_GW_879_112_2_18_1_7_42_1128,"[BT]DGCOM_Close\n")
TRACE_MSG(PF_DUN_GW_908_112_2_18_1_7_42_1129,"[BT]DGCOM_GetTxFifoCnt, maxMsgSize = %d\n")
TRACE_MSG(PF_DUN_GW_937_112_2_18_1_7_42_1130,"[BT]DGCOM_ReadData: port = %d, buffer_len = %d, payload_len=%dn")
TRACE_MSG(PF_DUN_GW_948_112_2_18_1_7_42_1131,"[DRV_BT]<ASSERT_ERROR>DGCOM_ReadData: port NOT open !")
TRACE_MSG(PF_DUN_GW_994_112_2_18_1_7_42_1132,"[BT]DGCOM_WriteData: port = %d, len = %d\n")
TRACE_MSG(PF_DUN_GW_1005_112_2_18_1_7_42_1133,"[DRV_BT]<ASSERT_ERROR>DGCOM_ReadData: port NOT open !")
TRACE_MSG(PF_DUN_GW_1049_112_2_18_1_7_42_1134,"[BT]DGCOM_DumpGetChar port_num = %d\n")
TRACE_MSG(PF_DUN_GW_1068_112_2_18_1_7_42_1135,"[BT]DGCOM_DumpPutChar port_num = %d nchar = %c\n")
TRACE_MSG(PF_DUN_GW_1088_112_2_18_1_7_42_1136,"[BT]DGCOM_Tx_Int_Enable is_enable = %d  open_flag = %dn")
TRACE_MSG(PF_DUN_GW_1093_112_2_18_1_7_42_1137,"[BT]DGCOM_Tx_Int_Enable,The DGCOM has been enabled!\n ")
TRACE_MSG(PF_DUN_GW_1107_112_2_18_1_7_42_1138,"[BT]DGCOM_Tx_Int_Enable, Enable Tx but DGCOM or peerside not open.\n ")
TRACE_MSG(PF_DUN_GW_1134_112_2_18_1_7_42_1139,"[BT]DGCOM_ReadData port_num = %d,is_enable = %d\n")
TRACE_MSG(DB_140_112_2_18_1_7_42_1140,"[_DB_SearchDevice] i = %d")
TRACE_MSG(DB_146_112_2_18_1_7_42_1141,"[_DB_SearchDevice] NOT exist")
TRACE_MSG(DB_209_112_2_18_1_7_43_1142,"[_DB_CheckDeviceCategory] IN (knownServices11_00_31 = 0x%x, classOfDevice = 0x%x)")
TRACE_MSG(DB_253_112_2_18_1_7_43_1143,"device record:")
TRACE_MSG(DB_255_112_2_18_1_7_43_1144,"deviceAddr = %04x:%02x:%06x")
TRACE_MSG(DB_256_112_2_18_1_7_43_1145,"linkkeyValid = %d")
TRACE_MSG(DB_257_112_2_18_1_7_43_1146,"linkkeyLen = %d")
TRACE_MSG(DB_258_112_2_18_1_7_43_1147,"Linkkey = %s")
TRACE_MSG(DB_259_112_2_18_1_7_43_1148,"remoteName = %s")
TRACE_MSG(DB_260_112_2_18_1_7_43_1149,"classOfDevice = 0x%x")
TRACE_MSG(DB_261_112_2_18_1_7_43_1150,"knownServices11_00_31 = 0x%x")
TRACE_MSG(DB_262_112_2_18_1_7_43_1151,"knownServices11_32_63 = 0x%x")
TRACE_MSG(DB_263_112_2_18_1_7_43_1152,"knownServices12_00_31 = 0x%x")
TRACE_MSG(DB_264_112_2_18_1_7_43_1153,"knownServices13_00_31 = 0x%x")
TRACE_MSG(DB_265_112_2_18_1_7_43_1154,"authorised = %d")
TRACE_MSG(DB_270_112_2_18_1_7_43_1155,"addr = %04x:%02x:%06x")
TRACE_MSG(DB_305_112_2_18_1_7_43_1156,"[DB_SetLocalDeviceStatus] IN (status = %d)")
TRACE_MSG(DB_311_112_2_18_1_7_43_1157,"[DB_GetLocalDeviceStatus] IN")
TRACE_MSG(DB_312_112_2_18_1_7_43_1158,"[DB_GetLocalDeviceStatus] status = %d")
TRACE_MSG(DB_319_112_2_18_1_7_43_1159,"[DB_GetLocalName] IN")
TRACE_MSG(DB_330_112_2_18_1_7_43_1160,"[DB_SetLocalName] IN")
TRACE_MSG(DB_334_112_2_18_1_7_43_1161,"[DB_SetLocalName] len = %d")
TRACE_MSG(DB_355_112_2_18_1_7_43_1162,"[DB_SetVisibility] IN (visibility = %d)")
TRACE_MSG(DB_362_112_2_18_1_7_43_1163,"[DB_GetVisibility] IN")
TRACE_MSG(DB_363_112_2_18_1_7_43_1164,"[DB_GetVisibility] visibility = %d")
TRACE_MSG(DB_376_112_2_18_1_7_43_1165,"[DB_ReadRecord] IN")
TRACE_MSG(DB_380_112_2_18_1_7_43_1166,"[DB_ReadRecord] The device is not found")
TRACE_MSG(DB_384_112_2_18_1_7_43_1167,"[DB_ReadRecord] index = %d")
TRACE_MSG(DB_407_112_2_18_1_7_43_1168,"[DB_WriteRecord] IN")
TRACE_MSG(DB_417_112_2_18_1_7_43_1169,"do not write 0 address to record")
TRACE_MSG(DB_423_112_2_18_1_7_43_1170,"[DB_WriteRecord] It is a new device, get a free entry for it")
TRACE_MSG(DB_434_112_2_18_1_7_43_1171,"[DB_WriteRecord] dev_count = %d")
TRACE_MSG(DB_435_112_2_18_1_7_43_1172,"[DB_WriteRecord] index = %d")
TRACE_MSG(DB_452_112_2_18_1_7_43_1173,"[DB_WriteRecord] index = %d, strlen=%d, refresh_service=%dn")
TRACE_MSG(DB_456_112_2_18_1_7_43_1174,"[DB_WriteRecord] Remote name has existed, and not change.\n")
TRACE_MSG(DB_470_112_2_18_1_7_43_1175,"[DB_WriteRecord] Remote name reflesh.\n")
TRACE_MSG(DB_491_112_2_18_1_7_43_1176,"[DB_WriteRecord] get the 0 class device")
TRACE_MSG(DB_496_112_2_18_1_7_43_1177,"[DB_WriteRecord] reset Device class,get the wrong class device")
TRACE_MSG(DB_515_112_2_18_1_7_43_1178,"[DB_DeleteRecord] IN")
TRACE_MSG(DB_517_112_2_18_1_7_43_1179,"[DB_DeleteRecord] dev_count = %d")
TRACE_MSG(DB_520_112_2_18_1_7_43_1180,"[DB_DeleteRecord] The device is not found")
TRACE_MSG(DB_542_112_2_18_1_7_43_1181,"[DB_GetRecordCount] IN")
TRACE_MSG(DB_543_112_2_18_1_7_43_1182,"[DB_GetRecordCount] dev_count = %d")
TRACE_MSG(DB_553_112_2_18_1_7_43_1183,"[DB_ReadRecordWithIndex] IN (index = %d)")
TRACE_MSG(DB_557_112_2_18_1_7_43_1184,"[DRV_BT]<ASSERT_ERROR> DB_ReadRecordWithIndex invalid index")
TRACE_MSG(DB_573_112_2_18_1_7_43_1185,"[DRV_BT]<ASSERT_ERROR> DB_ReadRecordWithIndex invalid index")
TRACE_MSG(DB_580_112_2_18_1_7_43_1186,"[DB_ReadFirstRecord] IN")
TRACE_MSG(DB_595_112_2_18_1_7_43_1187,"[DB_ReadNextRecord] IN")
TRACE_MSG(DB_614_112_2_18_1_7_43_1188,"[DB_SetDeviceName] IN")
TRACE_MSG(DB_618_112_2_18_1_7_43_1189,"[DB_SetDeviceName] The device is not found")
TRACE_MSG(DB_642_112_2_18_1_7_43_1190,"[DB_GetDeviceName] IN")
TRACE_MSG(DB_646_112_2_18_1_7_43_1191,"[DB_GetDeviceName] The device is not found")
TRACE_MSG(DB_667_112_2_18_1_7_43_1192,"[DB_GetDeivceCoD] IN")
TRACE_MSG(DB_671_112_2_18_1_7_44_1193,"[DB_GetDeviceCoD] Device is not found")
TRACE_MSG(DB_676_112_2_18_1_7_44_1194,"[DB_GetDeviceCoD] classOfDevice = 0x%x")
TRACE_MSG(DB_686_112_2_18_1_7_44_1195,"[DB_SetDeviceCoD] IN")
TRACE_MSG(DB_688_112_2_18_1_7_44_1196,"cod = 0x%x")
TRACE_MSG(DB_691_112_2_18_1_7_44_1197,"[DB_SetDeviceCod] Device not found")
TRACE_MSG(DB_708_112_2_18_1_7_44_1198,"[DB_SetDeviceConnStatus] IN")
TRACE_MSG(DB_711_112_2_18_1_7_44_1199,"bimap_ind = %d, service_mask = 0x%x, status = %d")
TRACE_MSG(DB_714_112_2_18_1_7_44_1200,"[DB_SetDeviceConnStatus] Device is not found")
TRACE_MSG(DB_742_112_2_18_1_7_44_1201,"[DB_GetDeviceConnStatus] IN")
TRACE_MSG(DB_744_112_2_18_1_7_44_1202,"bitmap_ind = %d, service_mask = 0x%x")
TRACE_MSG(DB_747_112_2_18_1_7_44_1203,"[DB_GetDeviceConnStatus] Device is not found")
TRACE_MSG(DB_767_112_2_18_1_7_44_1204,"[DB_GetDeviceService] IN")
TRACE_MSG(DB_771_112_2_18_1_7_44_1205,"[DB_GetDeviceService] Device is not found")
TRACE_MSG(DB_785_112_2_18_1_7_44_1206,"[DB_GetDeviceService] IN")
TRACE_MSG(DB_789_112_2_18_1_7_44_1207,"[DB_GetDeviceService] Device is not found")
TRACE_MSG(DB_802_112_2_18_1_7_44_1208,"[DB_CheckDeviceService] IN")
TRACE_MSG(DB_806_112_2_18_1_7_44_1209,"[DB_CheckDeviceService] Device not found")
TRACE_MSG(DB_819_112_2_18_1_7_44_1210,"[DB_GetRecordCountWithDeviceCategory] IN (category = %d)")
TRACE_MSG(DB_833_112_2_18_1_7_44_1211,"[DB_GetRecordCountWithDeviceCategory] count = %d")
TRACE_MSG(DB_844_112_2_18_1_7_44_1212,"[DB_ReadRecordWithDeviceCategory] IN (category = %d, index = %d)")
TRACE_MSG(DB_869_112_2_18_1_7_44_1213,"[DRV_BT]<ASSERT_ERROR> DB_GetDeviceAuthorisation: BAD parameters ")
TRACE_MSG(DB_876_112_2_18_1_7_44_1214,"[DRV_BT]<ASSERT_ERROR> DB_GetDeviceAuthorisation: The device is not found")
TRACE_MSG(DB_883_112_2_18_1_7_44_1215,"[DRV_BT]DB_SetDeviceAuthorisation is_authorised = %d")
TRACE_MSG(DB_894_112_2_18_1_7_44_1216,"[DRV_BT]<ASSERT_ERROR> DB_GetDeviceAuthorisation: BAD parameters ")
TRACE_MSG(DB_901_112_2_18_1_7_44_1217,"[DRV_BT]<ASSERT_ERROR> DB_GetDeviceAuthorisation: The device is not found")
TRACE_MSG(DB_905_112_2_18_1_7_44_1218,"[DRV_BT]DB_SetDeviceAuthorisation is_authorised = %d")
TRACE_MSG(DB_916_112_2_18_1_7_44_1219,"[DB_Init] IN")
TRACE_MSG(DB_922_112_2_18_1_7_44_1220,"[DB_Init] Read NV failed\r\n")
TRACE_MSG(DB_947_112_2_18_1_7_44_1221,"[_DB_SaveToNV] IN\r\n")
TRACE_MSG(DB_950_112_2_18_1_7_44_1222,"[_DB_SaveToNV] Write NV failed\r\n")
TRACE_MSG(DB_961_112_2_18_1_7_44_1223,"[DB_SaveAddrToNV] IN\r\n")
TRACE_MSG(DB_967_112_2_18_1_7_44_1224,"[DB_SaveAddrToNV] Write NV failed\r\n")
TRACE_MSG(DB_984_112_2_18_1_7_44_1225,"[DB_Init] IN")
TRACE_MSG(DB_998_112_2_18_1_7_44_1226,"[DB_Init] PF_mkdir failed")
TRACE_MSG(DB_1009_112_2_18_1_7_44_1227,"[DB_Init] Open database file failed\r\n")
TRACE_MSG(DB_1018_112_2_18_1_7_44_1228,"[DB_Init] Read signature failed")
TRACE_MSG(DB_1021_112_2_18_1_7_44_1229,"[DB_Init] signature = 0x%x")
TRACE_MSG(DB_1024_112_2_18_1_7_44_1230,"[DB_Init] Unknown signature")
TRACE_MSG(DB_1030_112_2_18_1_7_44_1231,"[DB_Init] Read status failed")
TRACE_MSG(DB_1034_112_2_18_1_7_44_1232,"[DB_Init] status = %d")
TRACE_MSG(DB_1038_112_2_18_1_7_44_1233,"[DB_Init] Read visibility failed")
TRACE_MSG(DB_1042_112_2_18_1_7_44_1234,"[DB_Init] visibility = %d")
TRACE_MSG(DB_1046_112_2_18_1_7_44_1235,"[DB_Init] Read size of local name failed")
TRACE_MSG(DB_1049_112_2_18_1_7_44_1236,"[DB_Init] size of local name = %d")
TRACE_MSG(DB_1052_112_2_18_1_7_44_1237,"[DB_Init] Invalid count number")
TRACE_MSG(DB_1058_112_2_18_1_7_44_1238,"[DB_Init] Read local name failed")
TRACE_MSG(DB_1067_112_2_18_1_7_44_1239,"[DB_Init] Read count of device records failed")
TRACE_MSG(DB_1070_112_2_18_1_7_44_1240,"[DB_Init] count of device records = %d")
TRACE_MSG(DB_1073_112_2_18_1_7_44_1241,"[DB_Init] Invalid count number")
TRACE_MSG(DB_1081_112_2_18_1_7_44_1242,"[DB_Init] Read %d th record failed")
TRACE_MSG(DB_1089_112_2_18_1_7_44_1243,"[DB_Init] total %d records read")
TRACE_MSG(DB_1111_112_2_18_1_7_44_1244,"[DB_Deinit] IN")
TRACE_MSG(DB_1114_112_2_18_1_7_44_1245,"[DB_Deinit] create dir failed")
TRACE_MSG(DB_1124_112_2_18_1_7_44_1246,"[DB_Deinit] Create database file failed")
TRACE_MSG(DB_1133_112_2_18_1_7_44_1247,"[DB_Deinit] Write signature failed")
TRACE_MSG(DB_1138_112_2_18_1_7_44_1248,"[DB_Deinit] status = %d")
TRACE_MSG(DB_1140_112_2_18_1_7_44_1249,"[DB_Deinit] Write status failed")
TRACE_MSG(DB_1145_112_2_18_1_7_44_1250,"[DB_Deinit] visibility = %d")
TRACE_MSG(DB_1147_112_2_18_1_7_44_1251,"[DB_Deinit] Write visibility failed")
TRACE_MSG(DB_1153_112_2_18_1_7_44_1252,"[DB_Deinit] size of local name = %d")
TRACE_MSG(DB_1158_112_2_18_1_7_44_1253,"[DB_Deinit] Write size of local name failed")
TRACE_MSG(DB_1164_112_2_18_1_7_44_1254,"[DB_Deinit] Write local name failed")
TRACE_MSG(DB_1181_112_2_18_1_7_44_1255,"[DB_Deinit] count of device records = %d")
TRACE_MSG(DB_1183_112_2_18_1_7_44_1256,"[DB_Deinit] Write count of device records failed")
TRACE_MSG(DB_1192_112_2_18_1_7_44_1257,"[DB_Deinit] Write %d th device record failed")
TRACE_MSG(DB_1200_112_2_18_1_7_44_1258,"[DB_Deinit] total %d records written")
TRACE_MSG(PF_SPP_163_112_2_18_1_7_45_1259,"[BT]SPP_Activate app_handle = %d\n")
TRACE_MSG(PF_SPP_172_112_2_18_1_7_45_1260,"[BT]SPP_Activate role = DCE\n")
TRACE_MSG(PF_SPP_175_112_2_18_1_7_45_1261,"[BT]SPP_Activate role = DTE\n")
TRACE_MSG(PF_SPP_249_112_2_18_1_7_45_1262,"[BT]SPP_Deinit\n")
TRACE_MSG(PF_SPP_271_112_2_18_1_7_45_1263,"[BT]SPP_sendLinkStatus status=%d\n")
TRACE_MSG(PF_SPP_295_112_2_18_1_7_45_1264,"[BT]SPP_Deactivate app_handle = %d\n")
TRACE_MSG(PF_SPP_319_112_2_18_1_7_45_1265,"[BT]SPP_Connect\n")
TRACE_MSG(PF_SPP_320_112_2_18_1_7_45_1266,"app_handle = 0x%08x\n")
TRACE_MSG(PF_SPP_366_112_2_18_1_7_45_1267,"[BT]SPP_Disconnect\n")
TRACE_MSG(PF_SPP_367_112_2_18_1_7_45_1268,"[BT]muxId = 0x%04x\n")
TRACE_MSG(PF_SPP_368_112_2_18_1_7_45_1269,"[BT]serviceHandle = 0x%04x\n")
TRACE_MSG(PF_SPP_391_112_2_18_1_7_46_1270,"[BT]SPP_DataSend\n")
TRACE_MSG(PF_SPP_392_112_2_18_1_7_46_1271,"[BT]payload       = 0x%08x\n")
TRACE_MSG(PF_SPP_393_112_2_18_1_7_46_1272,"[BT]payload_len   = 0x%04x\n")
TRACE_MSG(PF_SPP_394_112_2_18_1_7_46_1273,"[BT]muxId         = 0x%04x\n")
TRACE_MSG(PF_SPP_395_112_2_18_1_7_46_1274,"[BT]serviceHandle = 0x%04x\n")
TRACE_MSG(PF_SPP_432_112_2_18_1_7_46_1275,"[BT]SPP_controlReqSend\n")
TRACE_MSG(PF_SPP_459_112_2_18_1_7_46_1276,"[BT]SPP_serviceNameResSend\n")
TRACE_MSG(PF_SPP_460_112_2_18_1_7_46_1277,"[BT]muxId = 0x%04x\n")
TRACE_MSG(PF_SPP_461_112_2_18_1_7_46_1278,"[BT]serviceHandle = 0x%04x\n")
TRACE_MSG(PF_SPP_484_112_2_18_1_7_46_1279,"[BT]SPP_getInstancesQidReq\n")
TRACE_MSG(PF_SPP_485_112_2_18_1_7_46_1280,"[BT]app_handle = 0x%04x\n")
TRACE_MSG(PF_SPP_510_112_2_18_1_7_46_1281,"SPP_handleActivateCfm\n")
TRACE_MSG(PF_SPP_511_112_2_18_1_7_46_1282,"muxId         = 0x%04x\n")
TRACE_MSG(PF_SPP_512_112_2_18_1_7_46_1283,"serverChannel = 0x%04x\n")
TRACE_MSG(PF_SPP_581_112_2_18_1_7_46_1284,"SPP_handleDeactivateCfm\n")
TRACE_MSG(PF_SPP_582_112_2_18_1_7_46_1285,"type         = 0x%04x\n")
TRACE_MSG(PF_SPP_583_112_2_18_1_7_46_1286,"phandle      = 0x%04x\n")
TRACE_MSG(PF_SPP_584_112_2_18_1_7_46_1287,"result       = %d\n")
TRACE_MSG(PF_SPP_585_112_2_18_1_7_46_1288,"state        = %d\n")
TRACE_MSG(PF_SPP_618_112_2_18_1_7_46_1289,"phandlesListSize = 0x%04x\n")
TRACE_MSG(PF_SPP_622_112_2_18_1_7_46_1290,"phandles = 0x%04x\n")
TRACE_MSG(PF_SPP_653_112_2_18_1_7_46_1291,"result = 0x%04x\n")
TRACE_MSG(PF_SPP_655_112_2_18_1_7_46_1292,"muxId = 0x%04x\n")
TRACE_MSG(PF_SPP_656_112_2_18_1_7_46_1293,"serverChannel = 0x%04x\n")
TRACE_MSG(PF_SPP_657_112_2_18_1_7_46_1294,"profileMaxFrameSize = 0x%04x\n")
TRACE_MSG(PF_SPP_658_112_2_18_1_7_46_1295,"validPortPar = 0x%04x\n")
TRACE_MSG(PF_SPP_659_112_2_18_1_7_46_1296,"portPar->port_speed = 0x%02x\n")
TRACE_MSG(PF_SPP_660_112_2_18_1_7_46_1297,"portPar->data_bits = 0x%02x\n")
TRACE_MSG(PF_SPP_661_112_2_18_1_7_46_1298,"portPar->stop_bits = 0x%02x\n")
TRACE_MSG(PF_SPP_662_112_2_18_1_7_46_1299,"portPar->parity = 0x%02x\n")
TRACE_MSG(PF_SPP_663_112_2_18_1_7_46_1300,"portPar->parity_type = 0x%02x\n")
TRACE_MSG(PF_SPP_664_112_2_18_1_7_46_1301,"portPar->flow_ctrl_mask = 0x%02x\n")
TRACE_MSG(PF_SPP_665_112_2_18_1_7_46_1302,"portPar->xon = 0x%02x\n")
TRACE_MSG(PF_SPP_666_112_2_18_1_7_46_1303,"portPar->xoff = 0x%02x\n")
TRACE_MSG(PF_SPP_667_112_2_18_1_7_46_1304,"portPar->parameter_mask = 0x%04x\n")
TRACE_MSG(PF_SPP_726_112_2_18_1_7_46_1305,"SPP_handleStatusInd: muxId = 0x%04x\n")
TRACE_MSG(PF_SPP_727_112_2_18_1_7_46_1306,"SPP_handleStatusInd: serverChannel = 0x%02x\n")
TRACE_MSG(PF_SPP_728_112_2_18_1_7_46_1307,"SPP_handleStatusInd: connect = 0x%02x\n")
TRACE_MSG(PF_SPP_729_112_2_18_1_7_46_1308,"SPP_handleStatusInd: maxMsgSize = 0x%04x\n")
TRACE_MSG(PF_SPP_804_112_2_18_1_7_47_1309,"muxId = 0x%04x\n")
TRACE_MSG(PF_SPP_805_112_2_18_1_7_47_1310,"serverChannel = 0x%02x\n")
TRACE_MSG(PF_SPP_806_112_2_18_1_7_47_1311,"modemstatus = 0x%04x\n")
TRACE_MSG(PF_SPP_807_112_2_18_1_7_47_1312,"break_signal = 0x%02x\n")
TRACE_MSG(PF_SPP_833_112_2_18_1_7_47_1313,"[BT]SPP_handleControlInd. Peer device RTR and RTC is not ready!\n")
TRACE_MSG(PF_SPP_859_112_2_18_1_7_47_1314,"muxId = 0x%04x\n")
TRACE_MSG(PF_SPP_860_112_2_18_1_7_47_1315,"serverChannel = 0x%02x\n")
TRACE_MSG(PF_SPP_862_112_2_18_1_7_47_1316,"result = 0x%02x\n")
TRACE_MSG(PF_SPP_863_112_2_18_1_7_47_1317,"app_handle = 0x%04x\n")
TRACE_MSG(PF_SPP_868_112_2_18_1_7_47_1318,"[BT] spp_data.state == %d\n")
TRACE_MSG(PF_SPP_869_112_2_18_1_7_47_1319,"[BT] SPP_handleDisconnectInd: SPP_Deactivate\n")
TRACE_MSG(PF_SPP_905_112_2_18_1_7_47_1320,"muxId = 0x%04x\n")
TRACE_MSG(PF_SPP_908_112_2_18_1_7_47_1321,"serviceName = %s\n")
TRACE_MSG(PF_SPP_909_112_2_18_1_7_47_1322,"serviceHandle = 0x%08x\n")
TRACE_MSG(PF_SPP_911_112_2_18_1_7_47_1323,"serviceNameListSize = 0x%08x\n")
TRACE_MSG(PF_SPP_940_112_2_18_1_7_47_1324,"[BT]SPP_PORTNEG_IND : portneg_ind->request is true\n")
TRACE_MSG(PF_SPP_950_112_2_18_1_7_47_1325,"[BT]SPP_PORTNEG_IND : portneg_ind->request is false\n")
TRACE_MSG(PF_SPP_983_112_2_18_1_7_47_1326,"muxId         = 0x%04x\n")
TRACE_MSG(PF_SPP_984_112_2_18_1_7_47_1327,"serverChannel = 0x%04x\n")
TRACE_MSG(PF_SPP_995_112_2_18_1_7_47_1328,"Spp kill timer when cfm\n")
TRACE_MSG(PF_SPP_1035_112_2_18_1_7_47_1329,"SPP_handleDataInd: muxId         = 0x%04x\n")
TRACE_MSG(PF_SPP_1036_112_2_18_1_7_47_1330,"SPP_handleDataInd: serverChannel = 0x%04x\n")
TRACE_MSG(PF_SPP_1037_112_2_18_1_7_47_1331,"SPP_handleDataInd: payloadLength = 0x%04x\n")
TRACE_MSG(PF_SPP_1038_112_2_18_1_7_47_1332,"SPP_handleDataInd: payload       = 0x%08x\n")
TRACE_MSG(PF_SPP_1039_112_2_18_1_7_47_1333,"SPP_handleDataInd: payload       = %s\n")
TRACE_MSG(PF_SPP_1066_112_2_18_1_7_47_1334,"[BT]SPP_handleDataInd: rx buffer full, lost data!!!\n")
TRACE_MSG(PF_SPP_1087_112_2_18_1_7_47_1335,"[BT]SPP_handleDataInd:the SPP COM has not opened.\n ")
TRACE_MSG(PF_SPP_1114_112_2_18_1_7_47_1336,"[BT]SPP_DATA_IND rx_fifo_buf = %s\n")
TRACE_MSG(PF_SPP_1115_112_2_18_1_7_47_1337,"SPP_DATA_IND Tick = %d\n")
TRACE_MSG(PF_SPP_1138_112_2_18_1_7_47_1338,"[BT]SPP_handlePrim msg = 0x%x\n")
TRACE_MSG(PF_SPP_1145_112_2_18_1_7_47_1339,"[BT]SPP_ACTIVATE_CFM\n")
TRACE_MSG(PF_SPP_1151_112_2_18_1_7_47_1340,"[BT]SPP_DEACTIVATE_CFM\n")
TRACE_MSG(PF_SPP_1157_112_2_18_1_7_47_1341,"[BT]SPP_CONNECT_IND\n")
TRACE_MSG(PF_SPP_1167_112_2_18_1_7_47_1342,"[BT]SPP_DATA_CFM\n")
TRACE_MSG(PF_SPP_1174_112_2_18_1_7_47_1343,"[BT]SPP_DATA_IND\n")
TRACE_MSG(PF_SPP_1181_112_2_18_1_7_47_1344,"[BT]SPP_CONTROL_IND\n")
TRACE_MSG(PF_SPP_1187_112_2_18_1_7_47_1345,"[BT]SPP_DISCONNECT_IND\n")
TRACE_MSG(PF_SPP_1193_112_2_18_1_7_47_1346,"[BT]SPP_PORTNEG_IND\n")
TRACE_MSG(PF_SPP_1200_112_2_18_1_7_47_1347,"[BT]SPP_PORTNEG_CFM\n")
TRACE_MSG(PF_SPP_1205_112_2_18_1_7_47_1348,"[BT]SPP_SERVICE_NAME_IND\n")
TRACE_MSG(PF_SPP_1211_112_2_18_1_7_47_1349,"[BT]SPP_AUDIO_IND\n")
TRACE_MSG(PF_SPP_1216_112_2_18_1_7_47_1350,"[BT]SPP_AUDIO_RELEASE_IND\n")
TRACE_MSG(PF_SPP_1221_112_2_18_1_7_47_1351,"[BT]SPP_MODE_CHANGE_IND\n")
TRACE_MSG(PF_SPP_1226_112_2_18_1_7_47_1352,"[BT]SPP_STATUS_IND\n")
TRACE_MSG(PF_SPP_1232_112_2_18_1_7_47_1353,"[BT]SPP_REGISTER_DATA_PATH_HANDLE_CFM\n")
TRACE_MSG(PF_SPP_1237_112_2_18_1_7_48_1354,"[BT]SPP_DATA_PATH_STATUS_IND\n")
TRACE_MSG(PF_SPP_1242_112_2_18_1_7_48_1355,"[BT]SPP_GET_INSTANCES_QID_CFM\n")
TRACE_MSG(PF_SPP_1248_112_2_18_1_7_48_1356,"[BT]SPP_AUDIO_RENEGOTIATE_CFM\n")
TRACE_MSG(PF_SPP_1253_112_2_18_1_7_48_1357,"[BT]SPP_AUDIO_RENEGOTIATE_IND\n")
TRACE_MSG(PF_SPP_1283_112_2_18_1_7_48_1358,"[BT]SPP_handleDeinitPrim msg = 0x%4x\n")
TRACE_MSG(PF_SPP_1308_112_2_18_1_7_48_1359,"[BT] Spp have connected with device!")
TRACE_MSG(PF_SPP_1313_112_2_18_1_7_48_1360,"[BT] Spp have not connected with device!")
TRACE_MSG(PF_SPP_1323_112_2_18_1_7_48_1361,"[BT] Spp have connected with device, please disconnect it first!")
TRACE_MSG(PF_SPP_1341_112_2_18_1_7_48_1362,"[BT] SPP_ReleaseCallback: releasing!")
TRACE_MSG(PF_SPP_1346_112_2_18_1_7_48_1363,"[BT] SPP_ReleaseCallback: released!")
TRACE_MSG(PF_SPP_1372_112_2_18_1_7_48_1364,"[BT]SPP_OpenVitualCOM func = 0x%08x\n")
TRACE_MSG(PF_SPP_1398_112_2_18_1_7_48_1365,"[BT]SPP_CloseVitualCOM\n")
TRACE_MSG(PF_SPP_1433_112_2_18_1_7_48_1366,"[BT]BT_SppClose\n")
TRACE_MSG(PF_SPP_1463_112_2_18_1_7_48_1367,"[BT] spp_data.state == SPP_IDLE\n")
TRACE_MSG(PF_SPP_1464_112_2_18_1_7_48_1368,"[BT] spp_data.state = SPP_DEACTIVING\n")
TRACE_MSG(PF_SPP_1470_112_2_18_1_7_48_1369,"[BT] spp_data.state == SPP_CONNECTED\n")
TRACE_MSG(PF_SPP_1471_112_2_18_1_7_48_1370,"[BT] spp_data.state = SPP_DEACTIVING\n")
TRACE_MSG(PF_SPP_1494_112_2_18_1_7_48_1371,"[BT]SPP_GetTxFifoCnt, maxMsgSize = %d\n")
TRACE_MSG(PF_SPP_1520_112_2_18_1_7_48_1372,"[BT]SPP_ReadData ,buffer = 0x%08x,read_max_len = %d real_len=%dn")
TRACE_MSG(PF_SPP_1535_112_2_18_1_7_48_1373,"[BT]SPP_ReadData ,buffer = 0x%08x\n")
TRACE_MSG(PF_SPP_1549_112_2_18_1_7_48_1374,"[BT]SPP_ReadData ,buffer = 0x%08x\n")
TRACE_MSG(PF_SPP_1580_112_2_18_1_7_48_1375,"[BT]SPP_WriteData ,buffer = 0x%08x len = %d\n")
TRACE_MSG(PF_SPP_1626_112_2_18_1_7_48_1376,"Spp kill timer when start timer\n")
TRACE_MSG(PF_SPP_1630_112_2_18_1_7_48_1377,"Spp start timer\n")
TRACE_MSG(PF_SPP_1656_112_2_18_1_7_48_1378,"[BT]SPP_Tx_Int_Enable,is_enable = %d\n")
TRACE_MSG(PF_SPP_1657_112_2_18_1_7_48_1379,"[BT]SPP_Tx_Int_Enable,peer_dev_rx_ready = %d\n")
TRACE_MSG(PF_SPP_1658_112_2_18_1_7_48_1380,"[BT]SPP_Tx_Int_Enable,open_flag = %d\n")
TRACE_MSG(PF_SPP_1659_112_2_18_1_7_48_1381,"[BT]SPP_Tx_Int_Enable,sppcom_info.tx_en = %d\n")
TRACE_MSG(PF_SPP_1664_112_2_18_1_7_48_1382,"[BT]SPP__Tx_Int_Enable,The SPPCOM has been enabled!\n ")
TRACE_MSG(PF_SPP_1679_112_2_18_1_7_48_1383,"[BT]SPP_Tx_Int_Enable, Enable Tx but peer device rx is not ready or the SPPCOM is not open.\n ")
TRACE_MSG(PF_SPP_1705_112_2_18_1_7_48_1384,"[BT]SPP_Rx_Int_Enable,is_enable = %d\n")
TRACE_MSG(PF_SPP_1738_112_2_18_1_7_49_1385,"[BT]APP_RxDataTest,rec_num = %d\n")
TRACE_MSG(PF_SPP_1753_112_2_18_1_7_49_1386,"SPP uart_callback: EVENT_DATA_TO_READ\n")
TRACE_MSG(PF_SPP_1785_112_2_18_1_7_49_1387,"SPP uart_callback: EVENT_DATA_TO_WRITE\n")
TRACE_MSG(PF_SPP_1792_112_2_18_1_7_49_1388,"SPP uart_callback: 1->tx_data_len = %d\n")
TRACE_MSG(PF_SPP_1798_112_2_18_1_7_49_1389,"SPP uart_callback: 2->tx_data_len = %d\n")
TRACE_MSG(PF_SPP_1801_112_2_18_1_7_49_1390,"SPP uart_callback: 3->tx_data_len = %d\n")
TRACE_MSG(PF_SPP_1807_112_2_18_1_7_49_1391,"SPP uart_callback: 4->tx_data_len = %d\n")
TRACE_MSG(PF_SPP_1809_112_2_18_1_7_49_1392,"SPP uart_callback: 5->tx_data_len = %d\n")
TRACE_MSG(PF_SPP_1818_112_2_18_1_7_49_1393,"SPP uart_callback: 6->tx_data_len = %d\n")
TRACE_MSG(PF_SPP_1845_112_2_18_1_7_49_1394,"[BT]BT_SppTest!\n")
TRACE_MSG(PF_SPP_1884_112_2_18_1_7_49_1395,"Spp kill timer when timer expired\n")
TRACE_MSG(PF_SPP_1905_112_2_18_1_7_49_1396,"[BT]SPP_SendMsgToApp:Msg_Id=0x%X, status=0x%X, body=0x%X, size=0x%X\r\n")
TRACE_MSG(PF_SPP_1909_112_2_18_1_7_49_1397,"[BT]SPP_SendMsgToApp:Invail Message\r\n")
TRACE_MSG(PF_SPP_1926_112_2_18_1_7_49_1398,"[BT]SPP_SendMsgToApp:No CallBack Function\r\n")
TRACE_MSG(PF_SPP_1979_112_2_18_1_7_49_1399,"[BT]SPP_Read_FIFO: Recieve data length = %d\n")
TRACE_MSG(PF_SPP_1980_112_2_18_1_7_49_1400,"[BT]SPP_Read_FIFO: Read data length = %d\n")
TRACE_MSG(PF_SPP_1981_112_2_18_1_7_49_1401,"[BT]SPP_Read_FIFO: p_rx_read = %d\n")
TRACE_MSG(PF_SPP_1982_112_2_18_1_7_49_1402,"[BT]SPP_Read_FIFO: p_rx_write = %d\n")
TRACE_MSG(PF_SPP_2012_112_2_18_1_7_49_1403,"[BT]SPP_TX_FIFO_Empty: tx_data_len = %d\n")
TRACE_MSG(PF_SPP_2034_112_2_18_1_7_49_1404,"[BT]SPP_Read_FIFO: p_rx_read = %d\n")
TRACE_MSG(PF_SPP_2035_112_2_18_1_7_49_1405,"[BT]SPP_Read_FIFO: p_rx_write = %d\n")
TRACE_MSG(PF_SPP_2104_112_2_18_1_7_49_1406,"[BT]SPP_StartTimeOut:Create Timer handle=0x%08X\r\n")
TRACE_MSG(PF_SPP_2108_112_2_18_1_7_49_1407,"[BT]SPP_StartTimeOut: Active Timer handle=0x%08X\r\n")
TRACE_MSG(PF_SPP_2125_112_2_18_1_7_49_1408,"[BT]SPP_IsTimeOutStart:time_handle = 0x%08X\r\n")
TRACE_MSG(PF_SPP_2143_112_2_18_1_7_49_1409,"[BT]OS_DeleteTimeOut:Timer handle = 0x%08X\r\n")
TRACE_MSG(PF_SPP_2162_112_2_18_1_7_49_1410,"[BT]SPP_StopTimeOut:time_handle = 0x%08X\r\n")
TRACE_MSG(PF_SPP_2179_112_2_18_1_7_49_1411,"[BT]SPP_Connect_TimeExpiredHandle:time_handle = 0x%08X\r\n")
TRACE_MSG(PF_CM_REDUCE_326_112_2_18_1_7_50_1412,"[BT] CSR btPalCmGetCacheIndex adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_333_112_2_18_1_7_50_1413,"[BT] CSR btPalCmGetCacheIndex return TRUE (index:0x%x)")
TRACE_MSG(PF_CM_REDUCE_338_112_2_18_1_7_50_1414,"[BT] CSR btPalCmGetCacheIndex return FALSE")
TRACE_MSG(PF_CM_REDUCE_364_112_2_18_1_7_50_1415,"[BT] CSR btPalCmGetCacheElement wrapping index to zero")
TRACE_MSG(PF_CM_REDUCE_371_112_2_18_1_7_50_1416,"[BT] CSR btPalCmGetCacheElement index not found using index: 0x%x  instead [nextFreeIndex 0x%x]")
TRACE_MSG(PF_CM_REDUCE_397_112_2_18_1_7_50_1417,"[BT]TimeOutExitHandle:Type=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_428_112_2_18_1_7_50_1418,"[BT]CM_TimeOutHandle:Type=0x%X,%x\r\n")
TRACE_MSG(PF_CM_REDUCE_453_112_2_18_1_7_50_1419,"[BT]:CM_TimeOutHandle - addr:lap:0x%X, uap:0x%X, nap:0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_465_112_2_18_1_7_51_1420,"[BT]<ERROR>%s L%d: SC_DEBOND_CFM TimeOut")
TRACE_MSG(PF_CM_REDUCE_507_112_2_18_1_7_51_1421,"[BT]CM_TimeOutHandle:Type  error")
TRACE_MSG(PF_CM_REDUCE_517_112_2_18_1_7_51_1422,"[BT]CM_SC_Init\r\n")
TRACE_MSG(PF_CM_REDUCE_547_112_2_18_1_7_51_1423,"[BT]CM_StartInquiry:classOfDeviceMask=0x%X,state=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_558_112_2_18_1_7_51_1424,"[BT] CM_StartInquiry creat cm_search_time fial")
TRACE_MSG(PF_CM_REDUCE_564_112_2_18_1_7_51_1425,"[BT]CM_StartInquiry:BT is inquirying.......\r\n")
TRACE_MSG(PF_CM_REDUCE_574_112_2_18_1_7_51_1426,"[BT][CSR_USE_FAST_SEARCH]CM_StartInquiry filters 0x%x")
TRACE_MSG(PF_CM_REDUCE_597_112_2_18_1_7_51_1427,"[BT] CM_StopInquiry: state=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_609_112_2_18_1_7_51_1428,"[BT]CM_StopInquiry cancel time")
TRACE_MSG(PF_CM_REDUCE_620_112_2_18_1_7_51_1429,"[BT][CSR_USE_FAST_SEARCH]CM_StopInquiry: Reading Remote Name\r\n")
TRACE_MSG(PF_CM_REDUCE_624_112_2_18_1_7_51_1430,"[BT][CSR_USE_FAST_SEARCH]CM_StopInquiry: CmCancelDiscoveryReqSend\r\n")
TRACE_MSG(PF_CM_REDUCE_634_112_2_18_1_7_51_1431,"[BT]CM_StopInquiry:BT is not in inqirying!\r\n")
TRACE_MSG(PF_CM_REDUCE_660_112_2_18_1_7_51_1432,"[BT]CM_SetVisibleCfm\r\n")
TRACE_MSG(PF_CM_REDUCE_671_112_2_18_1_7_51_1433,"[BT] CM_SetVisibleCfm fail")
TRACE_MSG(PF_CM_REDUCE_695_112_2_18_1_7_51_1434,"[BT]CM_SetVisible:scan=0x%X, inquiry=0x%X, page=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_717_112_2_18_1_7_51_1435,"[BT] CM_SetVisible fail")
TRACE_MSG(PF_CM_REDUCE_732_112_2_18_1_7_51_1436,"[BT]CM_RestoreVisible\r\n")
TRACE_MSG(PF_CM_REDUCE_745_112_2_18_1_7_51_1437,"[BT]CM_SetVisibleAtPowerOn:isVisibleAtPowerOn=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_761_112_2_18_1_7_51_1438,"[BT]CM_ScanReadEnable\r\n")
TRACE_MSG(PF_CM_REDUCE_767_112_2_18_1_7_51_1439,"[BT]CM_ScanWriteEnable:inuiry=0x%X,page=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_773_112_2_18_1_7_51_1440,"[BT]CM_ReadLocalName\r\n")
TRACE_MSG(PF_CM_REDUCE_780_112_2_18_1_7_51_1441,"[BT]CM_SetLocalName:name_ptr=%s\r\n")
TRACE_MSG(PF_CM_REDUCE_788_112_2_18_1_7_51_1442,"[BT]CM_ReadLocalBdAddr\r\n")
TRACE_MSG(PF_CM_REDUCE_797_112_2_18_1_7_51_1443,"[BT]CM_DeBondReq:state=0x%X,addr lap:0x%x,uap:0x%x,nap:0x%xrn")
TRACE_MSG(PF_CM_REDUCE_869_112_2_18_1_7_51_1444,"Devclass have been added in list ")
TRACE_MSG(PF_CM_REDUCE_875_112_2_18_1_7_51_1445," not a dongle device")
TRACE_MSG(PF_CM_REDUCE_880_112_2_18_1_7_51_1446,"AddDevicelist device_info= 0x%x,name[0]=0x%x,name[1]=0x%x,name[2]=0x%x")
TRACE_MSG(PF_CM_REDUCE_909_112_2_18_1_7_51_1447," error ,error ,error !!! device is paired full")
TRACE_MSG(PF_CM_REDUCE_912_112_2_18_1_7_51_1448,"devcice number =%d")
TRACE_MSG(PF_CM_REDUCE_944_112_2_18_1_7_51_1449,"not find the device in the list")
TRACE_MSG(PF_CM_REDUCE_965_112_2_18_1_7_52_1450,"[CM_GetDeviceClass] IN ( BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x: )")
TRACE_MSG(PF_CM_REDUCE_972_112_2_18_1_7_52_1451,"device_class=0x%x,i=%d")
TRACE_MSG(PF_CM_REDUCE_997_112_2_18_1_7_52_1452,"[CM_GetDeviceClass] IN ( BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x: )")
TRACE_MSG(PF_CM_REDUCE_1004_112_2_18_1_7_52_1453,"device_class=0x%x,i=%d")
TRACE_MSG(PF_CM_REDUCE_1019_112_2_18_1_7_52_1454,"[BT]SC_SetPairDeviceStatus: status = %d\n")
TRACE_MSG(PF_CM_REDUCE_1028_112_2_18_1_7_52_1455,"[BT]SC_SetPairDeviceAddr: is_paired = %d\n")
TRACE_MSG(PF_CM_REDUCE_1039_112_2_18_1_7_52_1456,"[BT]SC_GetPairDeviceAddr: is_paired = %d\n")
TRACE_MSG(PF_CM_REDUCE_1079_112_2_18_1_7_52_1457,"SetDevicePaired device_class=0x%x,i=%d")
TRACE_MSG(PF_CM_REDUCE_1084_112_2_18_1_7_52_1458,"error,error,device don't find")
TRACE_MSG(PF_CM_REDUCE_1104_112_2_18_1_7_52_1459,"[BT]CM_MsgPrimHandle: type = 0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_1119_112_2_18_1_7_52_1460,"[BT]:CM_DISCOVERY_RESULT_IND - addr:lap:0x%X, uap:0x%X, nap:0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_1124_112_2_18_1_7_52_1461,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND: Normal Search\r\n")
TRACE_MSG(PF_CM_REDUCE_1130_112_2_18_1_7_52_1462,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND: Normal Search - Found A Same Device\r\n")
TRACE_MSG(PF_CM_REDUCE_1136_112_2_18_1_7_52_1463,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND: IsSameDevAddr\r\n")
TRACE_MSG(PF_CM_REDUCE_1142_112_2_18_1_7_52_1464,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND: Normal Search - Sent to APP\r\n")
TRACE_MSG(PF_CM_REDUCE_1151_112_2_18_1_7_52_1465,"[BT]CM_DISCOVERY_RESULT_IND cache adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1152_112_2_18_1_7_52_1466,"[BT]CM_DISCOVERY_RESULT_IND cache name %s")
TRACE_MSG(PF_CM_REDUCE_1153_112_2_18_1_7_52_1467,"[BT]CM_DISCOVERY_RESULT_IND prim adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1154_112_2_18_1_7_52_1468,"[BT]CM_DISCOVERY_RESULT_IND prim name %s")
TRACE_MSG(PF_CM_REDUCE_1179_112_2_18_1_7_52_1469,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND:found a same device in normal mode\r\n")
TRACE_MSG(PF_CM_REDUCE_1193_112_2_18_1_7_52_1470,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND:found a new device cod: 0x%x 0x%xrn")
TRACE_MSG(PF_CM_REDUCE_1197_112_2_18_1_7_52_1471,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_RESULT_IND:found a same device\r\n")
TRACE_MSG(PF_CM_REDUCE_1207_112_2_18_1_7_52_1472,"[BT]CM_DISCOVERY_RESULT_IND:Search a same device\r\n")
TRACE_MSG(PF_CM_REDUCE_1219_112_2_18_1_7_52_1473,"[BT]CM_DISCOVERY_RESULT_IND:Search a empty device name\r\n")
TRACE_MSG(PF_CM_REDUCE_1237_112_2_18_1_7_52_1474,"[BT]CM_DISCOVERY_RESULT_IND cache adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1238_112_2_18_1_7_52_1475,"[BT]CM_DISCOVERY_RESULT_IND cache name %s")
TRACE_MSG(PF_CM_REDUCE_1239_112_2_18_1_7_52_1476,"[BT]CM_DISCOVERY_RESULT_IND prim adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1240_112_2_18_1_7_52_1477,"[BT]CM_DISCOVERY_RESULT_IND prim name %s")
TRACE_MSG(PF_CM_REDUCE_1269_112_2_18_1_7_52_1478,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM BCHS_CANCEL_INQUIRYING cnt:%d")
TRACE_MSG(PF_CM_REDUCE_1278_112_2_18_1_7_52_1479,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM All Discoveries Complete cnd:%d")
TRACE_MSG(PF_CM_REDUCE_1283_112_2_18_1_7_52_1480,"[BT] MsgPrimHandle cancel serach time")
TRACE_MSG(PF_CM_REDUCE_1296_112_2_18_1_7_52_1481,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM CmReadRemoteNameReqSend")
TRACE_MSG(PF_CM_REDUCE_1304_112_2_18_1_7_52_1482,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM CmDiscoveryReqSend filters 0x%x No of: %d")
TRACE_MSG(PF_CM_REDUCE_1310_112_2_18_1_7_52_1483,"[BT][CSR_USE_FAST_SEARCH]CM_DISCOVERY_CFM All Discoveries Complete cnd:%d")
TRACE_MSG(PF_CM_REDUCE_1317_112_2_18_1_7_52_1484,"[BT] MsgPrimHandle CM_DISCOVERY_CFM")
TRACE_MSG(PF_CM_REDUCE_1322_112_2_18_1_7_52_1485,"[BT] MsgPrimHandle cancel serach time")
TRACE_MSG(PF_CM_REDUCE_1332_112_2_18_1_7_52_1486,"[BT]CM_CANCEL_DISCOVERY_CFM:state=%d\r\n")
TRACE_MSG(PF_CM_REDUCE_1369_112_2_18_1_7_52_1487,"[BT]CM_CANCEL_DISCOVERY_CFM:error state\r\n")
TRACE_MSG(PF_CM_REDUCE_1384_112_2_18_1_7_52_1488,"[BT]CM_MsgPrimHandle: name = %s\r\n")
TRACE_MSG(PF_CM_REDUCE_1408_112_2_18_1_7_52_1489,"[BT]CM_READ_REMOTE_NAME_IND: name = %s\r\n")
TRACE_MSG(PF_CM_REDUCE_1411_112_2_18_1_7_52_1490,"[BT]CM_READ_REMOTE_NAME_IND The Name has not been change ")
TRACE_MSG(PF_CM_REDUCE_1415_112_2_18_1_7_52_1491,"[BT]CM_READ_REMOTE_NAME_IND copying the name")
TRACE_MSG(PF_CM_REDUCE_1432_112_2_18_1_7_52_1492,"[BT]CM_READ_REMOTE_NAME_CFM:resul = 0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_1473_112_2_18_1_7_53_1493,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM BCHS_CANCEL_INQUIRYING")
TRACE_MSG(PF_CM_REDUCE_1481_112_2_18_1_7_53_1494,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM Addr:lap:0x%X, uap:0x%X, nap:0x%X")
TRACE_MSG(PF_CM_REDUCE_1487_112_2_18_1_7_53_1495,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM SUCCESS: %s")
TRACE_MSG(PF_CM_REDUCE_1502_112_2_18_1_7_53_1496,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM SUCCESS cod: 0x%x\r\n")
TRACE_MSG(PF_CM_REDUCE_1526_112_2_18_1_7_53_1497,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM FAIL: 0x%x Named Unknown")
TRACE_MSG(PF_CM_REDUCE_1533_112_2_18_1_7_53_1498,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM MAX_DEVICE_DISCOVERY")
TRACE_MSG(PF_CM_REDUCE_1543_112_2_18_1_7_53_1499,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM CmReadRemoteNameReqSend")
TRACE_MSG(PF_CM_REDUCE_1551_112_2_18_1_7_53_1500,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM CmDiscoveryReqSend filters 0x%x")
TRACE_MSG(PF_CM_REDUCE_1557_112_2_18_1_7_53_1501,"[BT][CSR_USE_FAST_SEARCH]CM_READ_REMOTE_NAME_CFM All Discoveries Complete")
TRACE_MSG(PF_CM_REDUCE_1621_112_2_18_1_7_53_1502,"[BT]CM_SET_LOCAL_NAME_CFM:result = 0x%X, start=0x%Xrn")
TRACE_MSG(PF_CM_REDUCE_1633_112_2_18_1_7_53_1503,"[BT]CM_SET_LOCAL_NAME_CFM: allocating cache")
TRACE_MSG(PF_CM_REDUCE_1657_112_2_18_1_7_53_1504,"[BT]CM_READ_LOCAL_BD_ADDR_CFM:lap:%d, uap:%d,nap:%d\r\n")
TRACE_MSG(PF_CM_REDUCE_1667_112_2_18_1_7_53_1505,"[BT]Scan Read result:= 0x%x, scanEnable:= 0x%x\r\n")
TRACE_MSG(PF_CM_REDUCE_1712_112_2_18_1_7_53_1506,"[BT]CM_READ_SCAN_ENABLE_CFM:Invaild Message\r\n")
TRACE_MSG(PF_CM_REDUCE_1798_112_2_18_1_7_53_1507,"[BT]CM_SDC_SEARCH_IND Result - prim->service = 0x%x")
TRACE_MSG(PF_CM_REDUCE_1802_112_2_18_1_7_53_1508,"knownServices11_00_31 = 0x%08x\n")
TRACE_MSG(PF_CM_REDUCE_1807_112_2_18_1_7_53_1509,"knownServices11_32_63  = 0x%08x\n")
TRACE_MSG(PF_CM_REDUCE_1812_112_2_18_1_7_53_1510,"knownServices12_00_31 = 0x%08x\n")
TRACE_MSG(PF_CM_REDUCE_1817_112_2_18_1_7_53_1511,"knownServices13_00_31 = 0x%08x\n")
TRACE_MSG(PF_CM_REDUCE_1821_112_2_18_1_7_53_1512,"The service is currently not supported ")
TRACE_MSG(PF_CM_REDUCE_1829_112_2_18_1_7_53_1513,"[BT]CM_SDC_SEARCH_CFM")
TRACE_MSG(PF_CM_REDUCE_1840_112_2_18_1_7_53_1514,"[BT]CM_SDC_CLOSE_IND Result=0x%x ")
TRACE_MSG(PF_CM_REDUCE_1855_112_2_18_1_7_53_1515,"[BT] CSR remoteName   = %s\n")
TRACE_MSG(PF_CM_REDUCE_1856_112_2_18_1_7_53_1516,"[BT] CSR BD adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1857_112_2_18_1_7_53_1517,"[BT] CSR (prim) BD adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1866_112_2_18_1_7_53_1518,"[BT] CSR remoteName   = %d\n")
TRACE_MSG(PF_CM_REDUCE_1867_112_2_18_1_7_53_1519,"[BT] CSR BD adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1868_112_2_18_1_7_53_1520,"[BT] CSR (prim) BD adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1872_112_2_18_1_7_53_1521,"[BT] CSR cache look up failed\n")
TRACE_MSG(PF_CM_REDUCE_1890_112_2_18_1_7_53_1522,"[BT] CSR EMULATED SD_READ_AVAILABLE_SERVICES_CFM\r\n")
TRACE_MSG(PF_CM_REDUCE_1891_112_2_18_1_7_53_1523,"[BT] CSR result                = %d\n")
TRACE_MSG(PF_CM_REDUCE_1892_112_2_18_1_7_53_1524,"[BT] CSR remoteNameChanged     = %d\n")
TRACE_MSG(PF_CM_REDUCE_1893_112_2_18_1_7_53_1525,"[BT] CSR remoteName  = %s\n")
TRACE_MSG(PF_CM_REDUCE_1894_112_2_18_1_7_53_1526,"[BT] CSR BD adress: 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_1895_112_2_18_1_7_53_1527,"[BT] CSR knownServices11_00_31 = 0x%08x [appServ 0x%08x]")
TRACE_MSG(PF_CM_REDUCE_1896_112_2_18_1_7_53_1528,"[BT] CSR knownServices11_32_63 = 0x%08x [appServ 0x%08x]")
TRACE_MSG(PF_CM_REDUCE_1897_112_2_18_1_7_53_1529,"[BT] CSR knownServices12_00_31 = 0x%08x [appServ 0x%08x]")
TRACE_MSG(PF_CM_REDUCE_1898_112_2_18_1_7_53_1530,"[BT] CSR knownServices13_00_31 = 0x%08x [appServ 0x%08x]")
TRACE_MSG(PF_CM_REDUCE_1899_112_2_18_1_7_53_1531,"[BT] CSR authorised = 0x%x classOfDevice=0x%x search found=0x%x]")
TRACE_MSG(PF_CM_REDUCE_1912_112_2_18_1_7_53_1532,"[BT] CSR ID_STATUS_SD_READ_SERVICES_RES ")
TRACE_MSG(PF_CM_REDUCE_1925_112_2_18_1_7_53_1533,"[BT] CSR ID_STATUS_SC_PAIR OK ")
TRACE_MSG(PF_CM_REDUCE_1988_112_2_18_1_7_54_1534,"[BT]PF_MsgCmPrimHandle:Unhandle\r\n")
TRACE_MSG(PF_CM_REDUCE_2004_112_2_18_1_7_54_1535,"[BT] MsgPrimHandle cancel serach time")
TRACE_MSG(PF_CM_REDUCE_2040_112_2_18_1_7_54_1536,"[BT]_StartPairing: deviceAddr = %04X:%02X:%06X")
TRACE_MSG(PF_CM_REDUCE_2062_112_2_18_1_7_54_1537,"[BT]SC_StartPairing:state=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_2070_112_2_18_1_7_54_1538,"[BT]SC_StartPairing:Cancel Discovery at first!\r\n")
TRACE_MSG(PF_CM_REDUCE_2077_112_2_18_1_7_54_1539,"[BT][CSR_USE_FAST_SEARCH]SC_StartPairing: Reading Remote Name\r\n")
TRACE_MSG(PF_CM_REDUCE_2081_112_2_18_1_7_54_1540,"[BT][CSR_USE_FAST_SEARCH]SC_StartPairing: CmCancelDiscoveryReqSend\r\n")
TRACE_MSG(PF_CM_REDUCE_2100_112_2_18_1_7_54_1541,"[BT]SC_CancelPairing:state=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_2101_112_2_18_1_7_54_1542,"[BT]SC_CancelPairing - dev_addr:lap=%d,uap=%d,nap=%d\r\n")
TRACE_MSG(PF_CM_REDUCE_2143_112_2_18_1_7_54_1543,"[BT]SC_CancelPairing: Empty Operate!\r\n")
TRACE_MSG(PF_CM_REDUCE_2156_112_2_18_1_7_54_1544,"[BT]SC_InputPairPin:lap=0x%X,uap=0x%X, nap=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_2183_112_2_18_1_7_54_1545,"[BT]SC_MsgPrimHandle:type = 0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_2192_112_2_18_1_7_54_1546,"[BT]SC_PASSKEY_IND:lap=0x%X,uap=0x%X, nap=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_2230_112_2_18_1_7_54_1547,"[BT]SC_BOND_CFM: result= 0x%X,cod=%x,class=%x\r\n")
TRACE_MSG(PF_CM_REDUCE_2247_112_2_18_1_7_54_1548,"[BT]<INFO> cod = 0x%X, it's a Audio Device")
TRACE_MSG(PF_CM_REDUCE_2261_112_2_18_1_7_54_1549,"[BT] CSR  CmSdcSearchReqSend  SC_BOND_CFM\r\n")
TRACE_MSG(PF_CM_REDUCE_2271_112_2_18_1_7_54_1550,"[BT] CSR SD_ReadAvailableServerReq NOT EXECUTED uuid=0x%x service target addr : 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_2281_112_2_18_1_7_54_1551,"[BT] state is idle")
TRACE_MSG(PF_CM_REDUCE_2287_112_2_18_1_7_54_1552,"[BT]BCHS_BONDING_CANCELLED SUCCESSFUL ")
TRACE_MSG(PF_CM_REDUCE_2305_112_2_18_1_7_54_1553,"[BT]SC_BOND_IND:result= 0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_2322_112_2_18_1_7_54_1554,"[BT]<INFO> cod = 0x%X, it's a Audio Device")
TRACE_MSG(PF_CM_REDUCE_2332_112_2_18_1_7_54_1555,"[BT] CSR  CmSdcSearchReqSend SC_BOND_IND \r\n")
TRACE_MSG(PF_CM_REDUCE_2342_112_2_18_1_7_54_1556,"[BT] CSR SD_ReadAvailableServerReq NOT EXECUTED uuid=0x%x service target addr : 0x%x:0x%x:0x%x")
TRACE_MSG(PF_CM_REDUCE_2362_112_2_18_1_7_54_1557,"[BT]SC_AUTHORISE_IND\r\n")
TRACE_MSG(PF_CM_REDUCE_2375_112_2_18_1_7_54_1558,"[BT]SC_DEBOND_CFM:result=0x%X\r\n")
TRACE_MSG(PF_CM_REDUCE_2388_112_2_18_1_7_55_1559,"\"\"\"# default in scPrim handler %x,\n")
TRACE_MSG(PF_SD_REDUCE_27_112_2_18_1_7_55_1560,"[BT]SD_ReadServiceTimeOutHandle:Time out\r\n")
TRACE_MSG(PF_SD_REDUCE_86_112_2_18_1_7_55_1561," btPalEmulateSdCopyCacheTypeToDevicePropertiesType")
TRACE_MSG(PF_SD_REDUCE_87_112_2_18_1_7_55_1562,"dest knownServices11_00_31 = 0x%08x\n")
TRACE_MSG(PF_SD_REDUCE_88_112_2_18_1_7_55_1563,"dest knownServices11_32_63  = 0x%08x\n")
TRACE_MSG(PF_SD_REDUCE_89_112_2_18_1_7_55_1564,"dest knownServices12_00_31 = 0x%08x\n")
TRACE_MSG(PF_SD_REDUCE_90_112_2_18_1_7_55_1565,"dest knownServices13_00_31 = 0x%08x\n")
TRACE_MSG(PF_SD_REDUCE_122_112_2_18_1_7_55_1566," btPalEmulateSdReadDevicePropertiesReqSend cache entry found")
TRACE_MSG(PF_SD_REDUCE_127_112_2_18_1_7_55_1567," btPalEmulateSdReadDevicePropertiesReqSend cache entry NOT found!!!!")
TRACE_MSG(PF_SD_REDUCE_135_112_2_18_1_7_55_1568,"[BT]SD_ReadAvailableServerReq\r\n")
TRACE_MSG(PF_SD_REDUCE_152_112_2_18_1_7_55_1569,"[BT]SD_ReadAvailableServerReq NOT EXECUTED uuid=0x%x service target addr : 0x%x:0x%x:0x%x")
TRACE_MSG(PF_SD_REDUCE_193_112_2_18_1_7_55_1570,"[BT]SD_HandlePrim:type = 0x%X\r\n")
TRACE_MSG(PF_SD_REDUCE_206_112_2_18_1_7_55_1571,"[BT]SD_READ_DEVICE_PROPERTIES_CFM:result = 0x%X\r\n")
TRACE_MSG(PF_SD_REDUCE_236_112_2_18_1_7_55_1572,"[BT]SD_READ_AVAILABLE_SERVICES_CFM\r\n")
TRACE_MSG(PF_SD_REDUCE_237_112_2_18_1_7_55_1573,"result                = %d\n")
TRACE_MSG(PF_SD_REDUCE_238_112_2_18_1_7_55_1574,"remoteNameChanged     = %d\n")
TRACE_MSG(PF_SD_REDUCE_242_112_2_18_1_7_55_1575,"deviceAddr            = %04x:%02x:%06x")
TRACE_MSG(PF_SD_REDUCE_244_112_2_18_1_7_55_1576,"friendlyName          = %s")
TRACE_MSG(PF_SD_REDUCE_246_112_2_18_1_7_55_1577,"classOfDevice         = 0x%x")
TRACE_MSG(PF_SD_REDUCE_248_112_2_18_1_7_55_1578,"knownServices11_00_31 = 0x%08xn")
TRACE_MSG(PF_SD_REDUCE_249_112_2_18_1_7_55_1579,"s_is_refresh_server   = 0x%x\n")
TRACE_MSG(PF_SD_REDUCE_323_112_2_18_1_7_55_1580,"[DRV_BT]<ASSERT_ERROR>: SD_HandlePrim: unkown type")
END_TRACE_MAP(MS_REF_BT_PAL_CSR_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_BT_PAL_CSR_TRC_H_

