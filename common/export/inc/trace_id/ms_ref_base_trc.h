/******************************************************************************
 ** File Name:      ms_ref_base_trc.h                                         *
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
//trace_id:36
#ifndef _MS_REF_BASE_TRC_H_
#define _MS_REF_BASE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define CM_221_112_2_18_1_3_52_0 "[CM]rec cmd size error!,buf_countn=%d\r\n"
#define CM_293_112_2_18_1_3_52_1 "[CM]rec cmd size error!,msg_head->len=%d,size=%d\r\n"
#define DIAG_1441_112_2_18_1_3_56_2 "dISPOSEMSG"
#define DIAG_1786_112_2_18_1_3_57_3 "Command type %d error!"
#define DIAG_2166_112_2_18_1_3_58_4 "SIMULATOR KPD: generate down key, key code is %x"
#define DIAG_2184_112_2_18_1_3_58_5 "SIMULATOR KPD: generate up key, key code is %x"
#define DIAG_2215_112_2_18_1_3_58_6 "SIMULATOR KPD: generate down and up key, key code is %x"
#define DIAG_2531_112_2_18_1_3_59_7 "MCU Write Addr:%p  Mode:%d  Size:%d"
#define DIAG_2614_112_2_18_1_3_59_8 "MCU Read Addr:%p  Mode:%d  Bytes:%d"
#define DIAG_2692_112_2_18_1_3_59_9 "ERROR: MCU access Command is ERROR !!!"
#define DIAG_2757_112_2_18_1_3_59_10 "ERROR: This address cann't access in this mode, alignment is error."
#define DIAG_2763_112_2_18_1_3_59_11 "ERROR: Access size is 0 or too large(0< size(conver to bytes) < %d)."
#define DIAG_2818_112_2_18_1_3_59_12 "\"\"CHANGE MODE = %d"
#define DIAG_3053_112_2_18_1_4_0_13 "NVITEM_USB_AUTO_MODE = %d"
#define DIAG_3064_112_2_18_1_4_0_14 "ERROR: NVItem access type is ERROR !!!"
#define DIAG_3442_112_2_18_1_4_0_15 "ReadADC.( %x = %d)"
#define DOIDLEONPAGE_91_112_2_18_1_4_1_16 "DoIdle_RegisterCallback: func 0x%x has register !"
#define DOIDLEONPAGE_117_112_2_18_1_4_1_17 "DoIdle_RegisterCallback: return %d, func addr %d"
#define DOIDLEONPAGE_143_112_2_18_1_4_1_18 "DoIdle_RegisterCallback: return %d, func addr %d"
#define DOIDLEONPAGE_214_112_2_18_1_4_1_19 "%s(%d) : Create timer failed."
#define DUMMY_LAYER1_AUDIO_45_112_2_18_1_4_5_20 "LAYER1_PlaySpecialTone: tone_id = %d, b_uplink = %d"
#define DUMMY_LAYER1_AUDIO_63_112_2_18_1_4_5_21 "LAYER1_PlayGenericTone: freq1 = %d, freq2 = %d, freq3 = %d"
#define DUMMY_LAYER1_AUDIO_80_112_2_18_1_4_5_22 "LAYER1_StopTone: b_pause = %d"
#define DUMMY_LAYER1_AUDIO_97_112_2_18_1_4_5_23 "LAYER1_SetVolume: Mic = %d, Speaker = %d"
#define DUMMY_LAYER1_AUDIO_111_112_2_18_1_4_5_24 "LAYER1_MuteSwitch: b_mute = %d"
#define DUMMY_LAYER1_AUDIO_126_112_2_18_1_4_5_25 "LAYER1_OpenSoundDevice: Mic = %d, Speaker = %d"
#define DUMMY_LAYER1_AUDIO_141_112_2_18_1_4_5_26 "LAYER1_SwitchSoundDevice: Mic = %d, Speaker = %d"
#define DUMMY_LAYER1_AUDIO_153_112_2_18_1_4_5_27 "LAYER1_SwitchTestMode: is_test_mode = %d"
#define DUMMY_LAYER1_AUDIO_165_112_2_18_1_4_5_28 "LAYER1_PlaySideTone: tone_gain = %d"
#define DUMMY_LAYER1_AUDIO_179_112_2_18_1_4_5_29 "LAYER1_SwitchEchoCancel: is_echo_cancel_on = %d"
#define INIT_556_112_2_18_1_5_20_30 "\r\nSCI_InitRef: IRAM mgr fail to register"
#define INIT_793_112_2_18_1_5_20_31 "\r\n============Version Information=============\r\n"
#define INIT_803_112_2_18_1_5_20_32 "RTOS Compile Date : %s"
#define INIT_807_112_2_18_1_5_20_33 "REF Compile Date : %s"
#define INIT_809_112_2_18_1_5_20_34 "\r\n=============================================\r\n"
#define IRAM_MGR_1155_112_2_18_1_5_24_35 "[iram_thread] (%d), SignalCode = %d"
#define IRAM_MGR_1172_112_2_18_1_5_24_36 "[iram_thread:%d] area registered"
#define IRAM_MGR_1181_112_2_18_1_5_24_37 "[iram_thread:%d] area unregistered"
#define IRAM_MGR_1202_112_2_18_1_5_24_38 "[iram_thread:%d] lock area(lock = 0x%x)"
#define IRAM_MGR_1209_112_2_18_1_5_24_39 "[iram_thread:%d] unlock area"
#define IRAM_MGR_1216_112_2_18_1_5_24_40 "[iram_thread:%d] <WARNING> NOT support SignalCode"
#define IRAM_MGR_1232_112_2_18_1_5_24_41 "[HandleTestIram] subtype = %d, thread = %d"
#define IRAM_MGR_1242_112_2_18_1_5_24_42 "[HandleTestIram] g_iram = 0x%x"
#define KEYPAD_149_112_2_18_1_5_25_43 "KPD_SVR: Invalid Key: 0x%x"
#define KEYPAD_169_112_2_18_1_5_25_44 "KPD_SVR: Virtual Key: 0x%x is DOWN\n"
#define KEYPAD_178_112_2_18_1_5_25_45 "KPD_SVR: Virtual Key: 0x%x is UP\n"
#define MISC_TASK_101_112_2_18_1_5_25_46 "thread_misc_task_entry: Received signal: 0x%04x!"
#define MISC_TASK_392_112_2_18_1_5_26_47 "SCI_UpdateRtcCalibration: last_cal_time=%d, last_cal_result=%d,total_delta_seconds=%d, last_cal_numerator=%d"
#define POWER_118_112_2_18_1_5_27_48 "POWER_PowerOn !!!"
#define POWER_182_112_2_18_1_5_27_49 "POWER_PowerOff !!!"
#define POWER_260_112_2_18_1_5_27_50 "POWER_Reset !!!"
#define POWER_366_112_2_18_1_5_28_51 "POWER_GetRestartCondition: Geted value is %d"
#define POWER_380_112_2_18_1_5_28_52 "POWER_SetRestartCondition: Set to %d"
#define PRODUCTION_TEST_CURRENT_38_112_2_18_1_5_28_53 "ProductTest : CurrentTest (%d/%d)"
#define PRODUCTION_TEST_CURRENT_142_112_2_18_1_5_28_54 "production_test_current: Register Cmd Routine"
#define REF_OUTPORT_1524_112_2_18_1_5_32_55 "[REF] REF_RestoreFactorySetting() entry"
#define REF_PARAM_752_112_2_18_1_5_34_56 "[REF] REFPARAM_GetRefConfigValue() = 0x%X"
#define REF_PARAM_784_112_2_18_1_5_34_57 "[REF] REFPARAM_GetUsbAutoAssertRelModeFlag() OSPARAM_GetAssertMode = 0x%X"
#define REF_PARAM_807_112_2_18_1_5_34_58 "[REF] REFPARAM_GetUsbAutoModeFlag() UsbAutoModeFlag = %d"
#define REF_PARAM_836_112_2_18_1_5_34_59 "[REF] REFPARAM_SetUsbAutoModeFlag() the same mode = %d"
#define REF_PARAM_846_112_2_18_1_5_34_60 "[REF] REFPARAM_SetUsbAutoModeFlag() RefConfigValue = 0x%X, mode = %d"
#define SHELL_TEST_94_112_2_18_1_5_38_61 "SHELL_memLeakCheck!Total Memory Count=%d\n"
#define SHELL_TEST_101_112_2_18_1_5_38_62 "SHELL_memLeakCheck:Allocate mem failure!\n"
#define SHELL_TEST_119_112_2_18_1_5_38_63 "SHELL_memLeakCheck: Memory Offset=%d\r\n"
#define SHELL_TEST_126_112_2_18_1_5_38_64 "SHELL_memLeakCheck: Memory Get Error\r\n"
#define SHELL_TEST_142_112_2_18_1_5_38_65 "[SHELL] SHELL_memDump: argc=%d\n"
#define SHELL_TEST_146_112_2_18_1_5_38_66 "[SHELL] SHELL_memDump: argv[%d]=%s\n"
#define SHELL_TEST_238_112_2_18_1_5_38_67 "[SHELL] test_handler_play: argc=%d\n"
#define SHELL_TEST_242_112_2_18_1_5_38_68 "[SHELL] test_handler_play: argv[%d]=%s\n"
#define SHELL_TEST_300_112_2_18_1_5_38_69 "[SHELL] test_handler_play: argc=%d\n"
#define SHELL_TEST_304_112_2_18_1_5_38_70 "[SHELL] test_handler_play: argv[%d]=%s\n"
#define SHELL_TEST_336_112_2_18_1_5_38_71 "[SHELL] test_handler_1: argc=%d\n"
#define SHELL_TEST_340_112_2_18_1_5_38_72 "[SHELL] test_handler_1: argv[%d]=%s\n"
#define SHELL_TEST_347_112_2_18_1_5_38_73 "[SHELL] test_handler_2: argc=%d\n"
#define SHELL_TEST_351_112_2_18_1_5_38_74 "[SHELL] test_handler_2: argv[%d]=%s\n"
#define SHELL_TEST_359_112_2_18_1_5_38_75 "[SHELL] test_handler_play: argc=%d\n"
#define SHELL_TEST_363_112_2_18_1_5_38_76 "[SHELL] test_handler_play: argv[%d]=%s\n"
#define SHELL_TEST_375_112_2_18_1_5_38_77 "[SHELL] test_handler_pause: argc=%d\n"
#define SHELL_TEST_379_112_2_18_1_5_38_78 "[SHELL] test_handler_pause: argv[%d]=%s\n"
#define SHELL_TEST_388_112_2_18_1_5_38_79 "[SHELL] test_handler_resume: argc=%d\n"
#define SHELL_TEST_392_112_2_18_1_5_38_80 "[SHELL] test_handler_resume: argv[%d]=%s\n"
#define SHELL_TEST_401_112_2_18_1_5_38_81 "[SHELL] test_handler_stop: argc=%d\n"
#define SHELL_TEST_405_112_2_18_1_5_38_82 "[SHELL] test_handler_stop: argv[%d]=%s\n"
#define SHELL_TEST_595_112_2_18_1_5_39_83 "[SHELL] SHELL_stackInfoShow: argc=%d\n"
#define SHELL_TEST_601_112_2_18_1_5_39_84 "[SHELL] SHELL_taskInfoShow: argc=%d\n"
#define SHELL_TEST_658_112_2_18_1_5_39_85 "[SHELL] DIAG_TestDisposeShellCmd:\n"
#define SHELL_TEST_662_112_2_18_1_5_39_86 "[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n"
#define SHELL_TEST_665_112_2_18_1_5_39_87 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_669_112_2_18_1_5_39_88 "[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd1.\n"
#define SHELL_TEST_672_112_2_18_1_5_39_89 "[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
#define SHELL_TEST_675_112_2_18_1_5_39_90 "[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n"
#define SHELL_TEST_678_112_2_18_1_5_39_91 "[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
#define SHELL_TEST_685_112_2_18_1_5_39_92 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_696_112_2_18_1_5_39_93 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_707_112_2_18_1_5_39_94 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_717_112_2_18_1_5_39_95 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_725_112_2_18_1_5_39_96 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_734_112_2_18_1_5_39_97 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_742_112_2_18_1_5_39_98 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_750_112_2_18_1_5_39_99 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_760_112_2_18_1_5_39_100 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_772_112_2_18_1_5_39_101 "[SHELL] DIAG_TestDisposeShellCmd:\n"
#define SHELL_TEST_776_112_2_18_1_5_39_102 "[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n"
#define SHELL_TEST_779_112_2_18_1_5_39_103 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_784_112_2_18_1_5_39_104 "[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n"
#define SHELL_TEST_787_112_2_18_1_5_39_105 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_792_112_2_18_1_5_39_106 "[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd1.\n"
#define SHELL_TEST_795_112_2_18_1_5_39_107 "[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
#define SHELL_TEST_798_112_2_18_1_5_39_108 "[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n"
#define SHELL_TEST_801_112_2_18_1_5_39_109 "[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
#define SHELL_TEST_805_112_2_18_1_5_39_110 "[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n"
#define SHELL_TEST_808_112_2_18_1_5_39_111 "[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
#define SHELL_TEST_815_112_2_18_1_5_39_112 "[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
#define SHELL_TEST_825_112_2_18_1_5_39_113 "[SHELL] DIAG_TestDisposeShellCmd Parse finished and result=%d.\n"
#define SHELL_TEST_829_112_2_18_1_5_39_114 "[SHELL] DIAG_TestDisposeShellCmd: Parse finished and result=%d.\n"
#define SHELL_TEST_831_112_2_18_1_5_39_115 "[SHELL] DIAG_TestDisposeShellCmd: Unregister primary cmd.\n"
#define SHELL_TEST_834_112_2_18_1_5_39_116 "[SHELL] DIAG_TestDisposeShellCmd: Parsing mp3 cmd *****************.\n"
#define SHELL_TEST_841_112_2_18_1_5_39_117 "[SHELL] DIAG_TestDisposeShellCmd: Parsing error mp3 cmd *****************.\n"
#define SHELL_TEST_843_112_2_18_1_5_39_118 "[SHELL] DIAG_TestDisposeShellCmd: Parsing mp3 cmd end and parse_result=%d!! *****************.\n"
#define SYS_TIME_139_112_2_18_1_5_40_119 "TM_Init: Init system date & time."
#define SYS_TIME_198_112_2_18_1_5_41_120 "TM_SetSysDate: Set system date %04d-%02d-%02d !"
#define SYS_TIME_222_112_2_18_1_5_41_121 "TM_SetSysDate: day num %04d"
#define SYS_TIME_237_112_2_18_1_5_41_122 "TM_SetSysDate: System date is invalid !"
#define SYS_TIME_311_112_2_18_1_5_41_123 "TM_SetSysTime: Set system time %2d:%2d:%2d !"
#define SYS_TIME_338_112_2_18_1_5_41_124 "TM_SetSysTime: System time is invalid !"
#define SYS_TIME_815_112_2_18_1_5_42_125 "TM_SetAlarm: Set Alarm %4d-%2d-%2d  %2d:%2d:%2d !"
#define SYS_TIME_864_112_2_18_1_5_42_126 "TM_SetAlarm:rtc alarm value %4d %2d:%2d:%2d !"
#define SYS_TIME_872_112_2_18_1_5_42_127 "TM_SetAlarm: Alarm time is before current time !"
#define SYS_TIME_881_112_2_18_1_5_42_128 "TM_SetAlarm: Alarm time is invalid !"
#define SYS_TIME_900_112_2_18_1_5_42_129 "TM_DisableAlarm: Disable alarm (%4d-%02d-%02d  %02d:%02d:%02d)!"
#define SYS_TIME_1030_112_2_18_1_5_42_130 "TM_SetRtcIntType: type %d- value%d!"
#define SYS_TIME_1046_112_2_18_1_5_42_131 "TM_SetSysDate: System date is invalid !"
#define SYS_TIME_1062_112_2_18_1_5_42_132 "TM_GetSysDate: Read BeginDate from NV failed !"
#define TP_GESTURE_DRV_45_112_2_18_1_5_47_133 "gesture_type: TG_ROTATE_RIGHT\n"
#define TP_GESTURE_DRV_51_112_2_18_1_5_47_134 "gesture_type: TG_ROTATE_LEFT\n"
#define TP_GESTURE_DRV_61_112_2_18_1_5_47_135 "gesture_type: TG_ROTATE_RIGHT 1\n"
#define TP_GESTURE_DRV_67_112_2_18_1_5_47_136 "gesture_type: TG_ROTATE_LEFT 1 \n"
#define TP_GESTURE_DRV_93_112_2_18_1_5_47_137 "ges_ROTATE: rotat_mole = %d\n"
#define TP_GESTURE_DRV_96_112_2_18_1_5_47_138 "gesture_type:2 TG_ROTATE_RIGHT \n"
#define TP_GESTURE_DRV_101_112_2_18_1_5_47_139 "gesture_type:2 TG_ROTATE_LEFT \n"
#define TP_GESTURE_DRV_111_112_2_18_1_5_47_140 "gesture_type:1 TG_ROTATE_LEFT \n"
#define TP_GESTURE_DRV_116_112_2_18_1_5_47_141 "gesture_type:3 TG_ROTATE_RIGHT \n"
#define TP_GESTURE_DRV_124_112_2_18_1_5_47_142 "gesture_type:4 TG_ROTATE_RIGHT \n"
#define TP_GESTURE_DRV_129_112_2_18_1_5_47_143 "gesture_type:5 TG_ROTATE_LEFT \n"
#define TP_GESTURE_DRV_138_112_2_18_1_5_47_144 "gesture_type:6 TG_ROTATE_RIGHT \n"
#define TP_GESTURE_DRV_143_112_2_18_1_5_47_145 "gesture_type:7 TG_ROTATE_LEFT \n"
#define TP_GESTURE_DRV_151_112_2_18_1_5_47_146 "gesture_type:8 TG_ROTATE_LEFT \n"
#define TP_GESTURE_DRV_156_112_2_18_1_5_47_147 "gesture_type:9 TG_ROTATE_RIGHT \n"
#define TP_GESTURE_DRV_198_112_2_18_1_5_47_148 "ges_DOWN: num1 x = %d,y = %d\n"
#define TP_GESTURE_DRV_213_112_2_18_1_5_47_149 "ges_DOWN: num2 x = %d,y = %d,x1= %d, y2 = %d\n"
#define TP_GESTURE_DRV_233_112_2_18_1_5_47_150 "ges_MOVE: num1 x = %d,y = %d\n"
#define TP_GESTURE_DRV_248_112_2_18_1_5_47_151 "ges_MOVE: num2 x = %d,y = %d,x1= %d, y2 = %d\n"
#define TP_GESTURE_DRV_274_112_2_18_1_5_47_152 "GESTURE UP CLEAN!!!"
#define TP_SRV_350_112_2_18_1_5_48_153 "[TPSRV]_TP_CheckVirtualKey: (%d < x < %d) (%d < y <%d)"
#define TP_SRV_360_112_2_18_1_5_48_154 "[TPSRV]_TP_CheckVirtualKey: i = %d, keycode = 0x%X"
#define TP_SRV_398_112_2_18_1_5_48_155 "[TPSRV]TP_VirtualKeyHandle: TP= 0x%X (%d, %d) back(%d, %d) -> key = 0x%X"
#define TP_SRV_463_112_2_18_1_5_48_156 "[TPSRV]AdjustCoordinate: from (%d, %d) to (%d, %d)"
#define TP_SRV_528_112_2_18_1_5_49_157 "[TPSRV] state = 0x%X , (%d, %d)"
#define TP_SRV_550_112_2_18_1_5_49_158 "TPSRV: TP_DOWN Invalid coordinate"
#define TP_SRV_604_112_2_18_1_5_49_159 "[TPSRV: 0x%x, (x,y)=%d,%d]"
#define TP_SRV_630_112_2_18_1_5_49_160 "TPSRV Self-Adaptive Mode:%d\r\n"
#define TP_SRV_661_112_2_18_1_5_49_161 "[TPSRV] Invalid coordinate"
#define TP_SRV_678_112_2_18_1_5_49_162 "[TPSRV]: ----TP_MOVE_EXCEED_LIMIT ----continue.\n\r"
#define TP_SRV_680_112_2_18_1_5_49_163 "[TPSRV]: ----TP_MOVE_EXCEED_LIMIT ----**break**.\n\r"
#define TP_SRV_692_112_2_18_1_5_49_164 "[TPSRV]_TP_PushMsg:TP.cur.x=%d, TP.cur.y=%d.\n\r"
#define TP_SRV_700_112_2_18_1_5_49_165 "[TPSRV]TP_Not_Moving:TP.cur.x=%d, TP.cur.y=%d,TP.pre.x=%d,TP.pre.y=%d,"
#define TP_SRV_708_112_2_18_1_5_49_166 "[TPSRV: 0x%x, (x,y)=%d,%d]"
#define TP_SRV_743_112_2_18_1_5_49_167 "[TPSRV:] Error:TP_UP but without validate TP_DOWN. pre_state=0x%x"
#define TP_SRV_748_112_2_18_1_5_49_168 "[TPSRV: 0x%x, (x,y)=%d,%d]"
#define TP_SRV_773_112_2_18_1_5_49_169 "[TPSRV:TP_Enable]"
#define TP_SRV_783_112_2_18_1_5_49_170 "[TPSRV:TP_Disable]"
#define TP_SRV_790_112_2_18_1_5_49_171 "[TPSRV:TP_GetInfo]"
#define TP_SRV_813_112_2_18_1_5_49_172 "[TPSRV:TP_SetDataMode]"
#define TP_SRV_944_112_2_18_1_5_50_173 "[TPSRV:_callback] (%d,%d),(%d,%d),size=%d, Sigcode=0x%x"
#define TP_SRV_994_112_2_18_1_5_50_174 "[TPSRV:TP_Init]"
#define TP_SRV_1239_112_2_18_1_5_50_175 "SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d."
#define TP_SRV_ENTRY_89_112_2_18_1_5_51_176 "App_queue:\n"
#define TP_SRV_ENTRY_100_112_2_18_1_5_51_177 "queue info:enqueued:%d, store: %d, count: %d"
#define TP_SRV_V0_485_112_2_18_1_5_52_178 "TP_main: enter into TP_Init\n"
#define TP_SRV_V0_804_112_2_18_1_5_52_179 "TP_ADC_To_Coord: TP_Calculate, pen is UP.\n\r"
#define TP_SRV_V0_931_112_2_18_1_5_53_180 "_TP_DownIrqHandler: adc_x = %d"
#define TP_SRV_V0_961_112_2_18_1_5_53_181 "TP  DOWN IRQENTER :time = %d"
#define TP_SRV_V0_989_112_2_18_1_5_53_182 "TP UP IRQ ENTER"
#define TP_SRV_V0_1401_112_2_18_1_5_54_183 "Coordinate: x=%d, y=%d.\n\r"
#define TP_SRV_V0_1413_112_2_18_1_5_54_184 "Self-Adaptive Mode:%d\r\n"
#define TP_SRV_V0_1468_112_2_18_1_5_54_185 "TP_COORD_INVALID: Coordinate: x=%d, y=%d.\n\r"
#define TP_SRV_V0_1498_112_2_18_1_5_54_186 "TP_FIRSET_DOWN: Coordinate: TP.cur.x=%d, TP.cur.y=%d.\n\r"
#define TP_SRV_V0_1541_112_2_18_1_5_54_187 "TP_Not_Moving:TP.cur.x=%d, TP.cur.y=%d,TP.pre.x=%d,TP.pre.y=%d,"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_BASE_TRC)
TRACE_MSG(CM_221_112_2_18_1_3_52_0,"[CM]rec cmd size error!,buf_countn=%d\r\n")
TRACE_MSG(CM_293_112_2_18_1_3_52_1,"[CM]rec cmd size error!,msg_head->len=%d,size=%d\r\n")
TRACE_MSG(DIAG_1441_112_2_18_1_3_56_2,"dISPOSEMSG")
TRACE_MSG(DIAG_1786_112_2_18_1_3_57_3,"Command type %d error!")
TRACE_MSG(DIAG_2166_112_2_18_1_3_58_4,"SIMULATOR KPD: generate down key, key code is %x")
TRACE_MSG(DIAG_2184_112_2_18_1_3_58_5,"SIMULATOR KPD: generate up key, key code is %x")
TRACE_MSG(DIAG_2215_112_2_18_1_3_58_6,"SIMULATOR KPD: generate down and up key, key code is %x")
TRACE_MSG(DIAG_2531_112_2_18_1_3_59_7,"MCU Write Addr:%p  Mode:%d  Size:%d")
TRACE_MSG(DIAG_2614_112_2_18_1_3_59_8,"MCU Read Addr:%p  Mode:%d  Bytes:%d")
TRACE_MSG(DIAG_2692_112_2_18_1_3_59_9,"ERROR: MCU access Command is ERROR !!!")
TRACE_MSG(DIAG_2757_112_2_18_1_3_59_10,"ERROR: This address cann't access in this mode, alignment is error.")
TRACE_MSG(DIAG_2763_112_2_18_1_3_59_11,"ERROR: Access size is 0 or too large(0< size(conver to bytes) < %d).")
TRACE_MSG(DIAG_2818_112_2_18_1_3_59_12,"\"\"CHANGE MODE = %d")
TRACE_MSG(DIAG_3053_112_2_18_1_4_0_13,"NVITEM_USB_AUTO_MODE = %d")
TRACE_MSG(DIAG_3064_112_2_18_1_4_0_14,"ERROR: NVItem access type is ERROR !!!")
TRACE_MSG(DIAG_3442_112_2_18_1_4_0_15,"ReadADC.( %x = %d)")
TRACE_MSG(DOIDLEONPAGE_91_112_2_18_1_4_1_16,"DoIdle_RegisterCallback: func 0x%x has register !")
TRACE_MSG(DOIDLEONPAGE_117_112_2_18_1_4_1_17,"DoIdle_RegisterCallback: return %d, func addr %d")
TRACE_MSG(DOIDLEONPAGE_143_112_2_18_1_4_1_18,"DoIdle_RegisterCallback: return %d, func addr %d")
TRACE_MSG(DOIDLEONPAGE_214_112_2_18_1_4_1_19,"%s(%d) : Create timer failed.")
TRACE_MSG(DUMMY_LAYER1_AUDIO_45_112_2_18_1_4_5_20,"LAYER1_PlaySpecialTone: tone_id = %d, b_uplink = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_63_112_2_18_1_4_5_21,"LAYER1_PlayGenericTone: freq1 = %d, freq2 = %d, freq3 = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_80_112_2_18_1_4_5_22,"LAYER1_StopTone: b_pause = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_97_112_2_18_1_4_5_23,"LAYER1_SetVolume: Mic = %d, Speaker = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_111_112_2_18_1_4_5_24,"LAYER1_MuteSwitch: b_mute = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_126_112_2_18_1_4_5_25,"LAYER1_OpenSoundDevice: Mic = %d, Speaker = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_141_112_2_18_1_4_5_26,"LAYER1_SwitchSoundDevice: Mic = %d, Speaker = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_153_112_2_18_1_4_5_27,"LAYER1_SwitchTestMode: is_test_mode = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_165_112_2_18_1_4_5_28,"LAYER1_PlaySideTone: tone_gain = %d")
TRACE_MSG(DUMMY_LAYER1_AUDIO_179_112_2_18_1_4_5_29,"LAYER1_SwitchEchoCancel: is_echo_cancel_on = %d")
TRACE_MSG(INIT_556_112_2_18_1_5_20_30,"\r\nSCI_InitRef: IRAM mgr fail to register")
TRACE_MSG(INIT_793_112_2_18_1_5_20_31,"\r\n============Version Information=============\r\n")
TRACE_MSG(INIT_803_112_2_18_1_5_20_32,"RTOS Compile Date : %s")
TRACE_MSG(INIT_807_112_2_18_1_5_20_33,"REF Compile Date : %s")
TRACE_MSG(INIT_809_112_2_18_1_5_20_34,"\r\n=============================================\r\n")
TRACE_MSG(IRAM_MGR_1155_112_2_18_1_5_24_35,"[iram_thread] (%d), SignalCode = %d")
TRACE_MSG(IRAM_MGR_1172_112_2_18_1_5_24_36,"[iram_thread:%d] area registered")
TRACE_MSG(IRAM_MGR_1181_112_2_18_1_5_24_37,"[iram_thread:%d] area unregistered")
TRACE_MSG(IRAM_MGR_1202_112_2_18_1_5_24_38,"[iram_thread:%d] lock area(lock = 0x%x)")
TRACE_MSG(IRAM_MGR_1209_112_2_18_1_5_24_39,"[iram_thread:%d] unlock area")
TRACE_MSG(IRAM_MGR_1216_112_2_18_1_5_24_40,"[iram_thread:%d] <WARNING> NOT support SignalCode")
TRACE_MSG(IRAM_MGR_1232_112_2_18_1_5_24_41,"[HandleTestIram] subtype = %d, thread = %d")
TRACE_MSG(IRAM_MGR_1242_112_2_18_1_5_24_42,"[HandleTestIram] g_iram = 0x%x")
TRACE_MSG(KEYPAD_149_112_2_18_1_5_25_43,"KPD_SVR: Invalid Key: 0x%x")
TRACE_MSG(KEYPAD_169_112_2_18_1_5_25_44,"KPD_SVR: Virtual Key: 0x%x is DOWN\n")
TRACE_MSG(KEYPAD_178_112_2_18_1_5_25_45,"KPD_SVR: Virtual Key: 0x%x is UP\n")
TRACE_MSG(MISC_TASK_101_112_2_18_1_5_25_46,"thread_misc_task_entry: Received signal: 0x%04x!")
TRACE_MSG(MISC_TASK_392_112_2_18_1_5_26_47,"SCI_UpdateRtcCalibration: last_cal_time=%d, last_cal_result=%d,total_delta_seconds=%d, last_cal_numerator=%d")
TRACE_MSG(POWER_118_112_2_18_1_5_27_48,"POWER_PowerOn !!!")
TRACE_MSG(POWER_182_112_2_18_1_5_27_49,"POWER_PowerOff !!!")
TRACE_MSG(POWER_260_112_2_18_1_5_27_50,"POWER_Reset !!!")
TRACE_MSG(POWER_366_112_2_18_1_5_28_51,"POWER_GetRestartCondition: Geted value is %d")
TRACE_MSG(POWER_380_112_2_18_1_5_28_52,"POWER_SetRestartCondition: Set to %d")
TRACE_MSG(PRODUCTION_TEST_CURRENT_38_112_2_18_1_5_28_53,"ProductTest : CurrentTest (%d/%d)")
TRACE_MSG(PRODUCTION_TEST_CURRENT_142_112_2_18_1_5_28_54,"production_test_current: Register Cmd Routine")
TRACE_MSG(REF_OUTPORT_1524_112_2_18_1_5_32_55,"[REF] REF_RestoreFactorySetting() entry")
TRACE_MSG(REF_PARAM_752_112_2_18_1_5_34_56,"[REF] REFPARAM_GetRefConfigValue() = 0x%X")
TRACE_MSG(REF_PARAM_784_112_2_18_1_5_34_57,"[REF] REFPARAM_GetUsbAutoAssertRelModeFlag() OSPARAM_GetAssertMode = 0x%X")
TRACE_MSG(REF_PARAM_807_112_2_18_1_5_34_58,"[REF] REFPARAM_GetUsbAutoModeFlag() UsbAutoModeFlag = %d")
TRACE_MSG(REF_PARAM_836_112_2_18_1_5_34_59,"[REF] REFPARAM_SetUsbAutoModeFlag() the same mode = %d")
TRACE_MSG(REF_PARAM_846_112_2_18_1_5_34_60,"[REF] REFPARAM_SetUsbAutoModeFlag() RefConfigValue = 0x%X, mode = %d")
TRACE_MSG(SHELL_TEST_94_112_2_18_1_5_38_61,"SHELL_memLeakCheck!Total Memory Count=%d\n")
TRACE_MSG(SHELL_TEST_101_112_2_18_1_5_38_62,"SHELL_memLeakCheck:Allocate mem failure!\n")
TRACE_MSG(SHELL_TEST_119_112_2_18_1_5_38_63,"SHELL_memLeakCheck: Memory Offset=%d\r\n")
TRACE_MSG(SHELL_TEST_126_112_2_18_1_5_38_64,"SHELL_memLeakCheck: Memory Get Error\r\n")
TRACE_MSG(SHELL_TEST_142_112_2_18_1_5_38_65,"[SHELL] SHELL_memDump: argc=%d\n")
TRACE_MSG(SHELL_TEST_146_112_2_18_1_5_38_66,"[SHELL] SHELL_memDump: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_238_112_2_18_1_5_38_67,"[SHELL] test_handler_play: argc=%d\n")
TRACE_MSG(SHELL_TEST_242_112_2_18_1_5_38_68,"[SHELL] test_handler_play: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_300_112_2_18_1_5_38_69,"[SHELL] test_handler_play: argc=%d\n")
TRACE_MSG(SHELL_TEST_304_112_2_18_1_5_38_70,"[SHELL] test_handler_play: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_336_112_2_18_1_5_38_71,"[SHELL] test_handler_1: argc=%d\n")
TRACE_MSG(SHELL_TEST_340_112_2_18_1_5_38_72,"[SHELL] test_handler_1: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_347_112_2_18_1_5_38_73,"[SHELL] test_handler_2: argc=%d\n")
TRACE_MSG(SHELL_TEST_351_112_2_18_1_5_38_74,"[SHELL] test_handler_2: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_359_112_2_18_1_5_38_75,"[SHELL] test_handler_play: argc=%d\n")
TRACE_MSG(SHELL_TEST_363_112_2_18_1_5_38_76,"[SHELL] test_handler_play: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_375_112_2_18_1_5_38_77,"[SHELL] test_handler_pause: argc=%d\n")
TRACE_MSG(SHELL_TEST_379_112_2_18_1_5_38_78,"[SHELL] test_handler_pause: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_388_112_2_18_1_5_38_79,"[SHELL] test_handler_resume: argc=%d\n")
TRACE_MSG(SHELL_TEST_392_112_2_18_1_5_38_80,"[SHELL] test_handler_resume: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_401_112_2_18_1_5_38_81,"[SHELL] test_handler_stop: argc=%d\n")
TRACE_MSG(SHELL_TEST_405_112_2_18_1_5_38_82,"[SHELL] test_handler_stop: argv[%d]=%s\n")
TRACE_MSG(SHELL_TEST_595_112_2_18_1_5_39_83,"[SHELL] SHELL_stackInfoShow: argc=%d\n")
TRACE_MSG(SHELL_TEST_601_112_2_18_1_5_39_84,"[SHELL] SHELL_taskInfoShow: argc=%d\n")
TRACE_MSG(SHELL_TEST_658_112_2_18_1_5_39_85,"[SHELL] DIAG_TestDisposeShellCmd:\n")
TRACE_MSG(SHELL_TEST_662_112_2_18_1_5_39_86,"[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n")
TRACE_MSG(SHELL_TEST_665_112_2_18_1_5_39_87,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_669_112_2_18_1_5_39_88,"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd1.\n")
TRACE_MSG(SHELL_TEST_672_112_2_18_1_5_39_89,"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n")
TRACE_MSG(SHELL_TEST_675_112_2_18_1_5_39_90,"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n")
TRACE_MSG(SHELL_TEST_678_112_2_18_1_5_39_91,"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n")
TRACE_MSG(SHELL_TEST_685_112_2_18_1_5_39_92,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_696_112_2_18_1_5_39_93,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_707_112_2_18_1_5_39_94,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_717_112_2_18_1_5_39_95,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_725_112_2_18_1_5_39_96,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_734_112_2_18_1_5_39_97,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_742_112_2_18_1_5_39_98,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_750_112_2_18_1_5_39_99,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_760_112_2_18_1_5_39_100,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_772_112_2_18_1_5_39_101,"[SHELL] DIAG_TestDisposeShellCmd:\n")
TRACE_MSG(SHELL_TEST_776_112_2_18_1_5_39_102,"[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n")
TRACE_MSG(SHELL_TEST_779_112_2_18_1_5_39_103,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_784_112_2_18_1_5_39_104,"[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n")
TRACE_MSG(SHELL_TEST_787_112_2_18_1_5_39_105,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_792_112_2_18_1_5_39_106,"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd1.\n")
TRACE_MSG(SHELL_TEST_795_112_2_18_1_5_39_107,"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n")
TRACE_MSG(SHELL_TEST_798_112_2_18_1_5_39_108,"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n")
TRACE_MSG(SHELL_TEST_801_112_2_18_1_5_39_109,"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n")
TRACE_MSG(SHELL_TEST_805_112_2_18_1_5_39_110,"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n")
TRACE_MSG(SHELL_TEST_808_112_2_18_1_5_39_111,"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n")
TRACE_MSG(SHELL_TEST_815_112_2_18_1_5_39_112,"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n")
TRACE_MSG(SHELL_TEST_825_112_2_18_1_5_39_113,"[SHELL] DIAG_TestDisposeShellCmd Parse finished and result=%d.\n")
TRACE_MSG(SHELL_TEST_829_112_2_18_1_5_39_114,"[SHELL] DIAG_TestDisposeShellCmd: Parse finished and result=%d.\n")
TRACE_MSG(SHELL_TEST_831_112_2_18_1_5_39_115,"[SHELL] DIAG_TestDisposeShellCmd: Unregister primary cmd.\n")
TRACE_MSG(SHELL_TEST_834_112_2_18_1_5_39_116,"[SHELL] DIAG_TestDisposeShellCmd: Parsing mp3 cmd *****************.\n")
TRACE_MSG(SHELL_TEST_841_112_2_18_1_5_39_117,"[SHELL] DIAG_TestDisposeShellCmd: Parsing error mp3 cmd *****************.\n")
TRACE_MSG(SHELL_TEST_843_112_2_18_1_5_39_118,"[SHELL] DIAG_TestDisposeShellCmd: Parsing mp3 cmd end and parse_result=%d!! *****************.\n")
TRACE_MSG(SYS_TIME_139_112_2_18_1_5_40_119,"TM_Init: Init system date & time.")
TRACE_MSG(SYS_TIME_198_112_2_18_1_5_41_120,"TM_SetSysDate: Set system date %04d-%02d-%02d !")
TRACE_MSG(SYS_TIME_222_112_2_18_1_5_41_121,"TM_SetSysDate: day num %04d")
TRACE_MSG(SYS_TIME_237_112_2_18_1_5_41_122,"TM_SetSysDate: System date is invalid !")
TRACE_MSG(SYS_TIME_311_112_2_18_1_5_41_123,"TM_SetSysTime: Set system time %2d:%2d:%2d !")
TRACE_MSG(SYS_TIME_338_112_2_18_1_5_41_124,"TM_SetSysTime: System time is invalid !")
TRACE_MSG(SYS_TIME_815_112_2_18_1_5_42_125,"TM_SetAlarm: Set Alarm %4d-%2d-%2d  %2d:%2d:%2d !")
TRACE_MSG(SYS_TIME_864_112_2_18_1_5_42_126,"TM_SetAlarm:rtc alarm value %4d %2d:%2d:%2d !")
TRACE_MSG(SYS_TIME_872_112_2_18_1_5_42_127,"TM_SetAlarm: Alarm time is before current time !")
TRACE_MSG(SYS_TIME_881_112_2_18_1_5_42_128,"TM_SetAlarm: Alarm time is invalid !")
TRACE_MSG(SYS_TIME_900_112_2_18_1_5_42_129,"TM_DisableAlarm: Disable alarm (%4d-%02d-%02d  %02d:%02d:%02d)!")
TRACE_MSG(SYS_TIME_1030_112_2_18_1_5_42_130,"TM_SetRtcIntType: type %d- value%d!")
TRACE_MSG(SYS_TIME_1046_112_2_18_1_5_42_131,"TM_SetSysDate: System date is invalid !")
TRACE_MSG(SYS_TIME_1062_112_2_18_1_5_42_132,"TM_GetSysDate: Read BeginDate from NV failed !")
TRACE_MSG(TP_GESTURE_DRV_45_112_2_18_1_5_47_133,"gesture_type: TG_ROTATE_RIGHT\n")
TRACE_MSG(TP_GESTURE_DRV_51_112_2_18_1_5_47_134,"gesture_type: TG_ROTATE_LEFT\n")
TRACE_MSG(TP_GESTURE_DRV_61_112_2_18_1_5_47_135,"gesture_type: TG_ROTATE_RIGHT 1\n")
TRACE_MSG(TP_GESTURE_DRV_67_112_2_18_1_5_47_136,"gesture_type: TG_ROTATE_LEFT 1 \n")
TRACE_MSG(TP_GESTURE_DRV_93_112_2_18_1_5_47_137,"ges_ROTATE: rotat_mole = %d\n")
TRACE_MSG(TP_GESTURE_DRV_96_112_2_18_1_5_47_138,"gesture_type:2 TG_ROTATE_RIGHT \n")
TRACE_MSG(TP_GESTURE_DRV_101_112_2_18_1_5_47_139,"gesture_type:2 TG_ROTATE_LEFT \n")
TRACE_MSG(TP_GESTURE_DRV_111_112_2_18_1_5_47_140,"gesture_type:1 TG_ROTATE_LEFT \n")
TRACE_MSG(TP_GESTURE_DRV_116_112_2_18_1_5_47_141,"gesture_type:3 TG_ROTATE_RIGHT \n")
TRACE_MSG(TP_GESTURE_DRV_124_112_2_18_1_5_47_142,"gesture_type:4 TG_ROTATE_RIGHT \n")
TRACE_MSG(TP_GESTURE_DRV_129_112_2_18_1_5_47_143,"gesture_type:5 TG_ROTATE_LEFT \n")
TRACE_MSG(TP_GESTURE_DRV_138_112_2_18_1_5_47_144,"gesture_type:6 TG_ROTATE_RIGHT \n")
TRACE_MSG(TP_GESTURE_DRV_143_112_2_18_1_5_47_145,"gesture_type:7 TG_ROTATE_LEFT \n")
TRACE_MSG(TP_GESTURE_DRV_151_112_2_18_1_5_47_146,"gesture_type:8 TG_ROTATE_LEFT \n")
TRACE_MSG(TP_GESTURE_DRV_156_112_2_18_1_5_47_147,"gesture_type:9 TG_ROTATE_RIGHT \n")
TRACE_MSG(TP_GESTURE_DRV_198_112_2_18_1_5_47_148,"ges_DOWN: num1 x = %d,y = %d\n")
TRACE_MSG(TP_GESTURE_DRV_213_112_2_18_1_5_47_149,"ges_DOWN: num2 x = %d,y = %d,x1= %d, y2 = %d\n")
TRACE_MSG(TP_GESTURE_DRV_233_112_2_18_1_5_47_150,"ges_MOVE: num1 x = %d,y = %d\n")
TRACE_MSG(TP_GESTURE_DRV_248_112_2_18_1_5_47_151,"ges_MOVE: num2 x = %d,y = %d,x1= %d, y2 = %d\n")
TRACE_MSG(TP_GESTURE_DRV_274_112_2_18_1_5_47_152,"GESTURE UP CLEAN!!!")
TRACE_MSG(TP_SRV_350_112_2_18_1_5_48_153,"[TPSRV]_TP_CheckVirtualKey: (%d < x < %d) (%d < y <%d)")
TRACE_MSG(TP_SRV_360_112_2_18_1_5_48_154,"[TPSRV]_TP_CheckVirtualKey: i = %d, keycode = 0x%X")
TRACE_MSG(TP_SRV_398_112_2_18_1_5_48_155,"[TPSRV]TP_VirtualKeyHandle: TP= 0x%X (%d, %d) back(%d, %d) -> key = 0x%X")
TRACE_MSG(TP_SRV_463_112_2_18_1_5_48_156,"[TPSRV]AdjustCoordinate: from (%d, %d) to (%d, %d)")
TRACE_MSG(TP_SRV_528_112_2_18_1_5_49_157,"[TPSRV] state = 0x%X , (%d, %d)")
TRACE_MSG(TP_SRV_550_112_2_18_1_5_49_158,"TPSRV: TP_DOWN Invalid coordinate")
TRACE_MSG(TP_SRV_604_112_2_18_1_5_49_159,"[TPSRV: 0x%x, (x,y)=%d,%d]")
TRACE_MSG(TP_SRV_630_112_2_18_1_5_49_160,"TPSRV Self-Adaptive Mode:%d\r\n")
TRACE_MSG(TP_SRV_661_112_2_18_1_5_49_161,"[TPSRV] Invalid coordinate")
TRACE_MSG(TP_SRV_678_112_2_18_1_5_49_162,"[TPSRV]: ----TP_MOVE_EXCEED_LIMIT ----continue.\n\r")
TRACE_MSG(TP_SRV_680_112_2_18_1_5_49_163,"[TPSRV]: ----TP_MOVE_EXCEED_LIMIT ----**break**.\n\r")
TRACE_MSG(TP_SRV_692_112_2_18_1_5_49_164,"[TPSRV]_TP_PushMsg:TP.cur.x=%d, TP.cur.y=%d.\n\r")
TRACE_MSG(TP_SRV_700_112_2_18_1_5_49_165,"[TPSRV]TP_Not_Moving:TP.cur.x=%d, TP.cur.y=%d,TP.pre.x=%d,TP.pre.y=%d,")
TRACE_MSG(TP_SRV_708_112_2_18_1_5_49_166,"[TPSRV: 0x%x, (x,y)=%d,%d]")
TRACE_MSG(TP_SRV_743_112_2_18_1_5_49_167,"[TPSRV:] Error:TP_UP but without validate TP_DOWN. pre_state=0x%x")
TRACE_MSG(TP_SRV_748_112_2_18_1_5_49_168,"[TPSRV: 0x%x, (x,y)=%d,%d]")
TRACE_MSG(TP_SRV_773_112_2_18_1_5_49_169,"[TPSRV:TP_Enable]")
TRACE_MSG(TP_SRV_783_112_2_18_1_5_49_170,"[TPSRV:TP_Disable]")
TRACE_MSG(TP_SRV_790_112_2_18_1_5_49_171,"[TPSRV:TP_GetInfo]")
TRACE_MSG(TP_SRV_813_112_2_18_1_5_49_172,"[TPSRV:TP_SetDataMode]")
TRACE_MSG(TP_SRV_944_112_2_18_1_5_50_173,"[TPSRV:_callback] (%d,%d),(%d,%d),size=%d, Sigcode=0x%x")
TRACE_MSG(TP_SRV_994_112_2_18_1_5_50_174,"[TPSRV:TP_Init]")
TRACE_MSG(TP_SRV_1239_112_2_18_1_5_50_175,"SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d.")
TRACE_MSG(TP_SRV_ENTRY_89_112_2_18_1_5_51_176,"App_queue:\n")
TRACE_MSG(TP_SRV_ENTRY_100_112_2_18_1_5_51_177,"queue info:enqueued:%d, store: %d, count: %d")
TRACE_MSG(TP_SRV_V0_485_112_2_18_1_5_52_178,"TP_main: enter into TP_Init\n")
TRACE_MSG(TP_SRV_V0_804_112_2_18_1_5_52_179,"TP_ADC_To_Coord: TP_Calculate, pen is UP.\n\r")
TRACE_MSG(TP_SRV_V0_931_112_2_18_1_5_53_180,"_TP_DownIrqHandler: adc_x = %d")
TRACE_MSG(TP_SRV_V0_961_112_2_18_1_5_53_181,"TP  DOWN IRQENTER :time = %d")
TRACE_MSG(TP_SRV_V0_989_112_2_18_1_5_53_182,"TP UP IRQ ENTER")
TRACE_MSG(TP_SRV_V0_1401_112_2_18_1_5_54_183,"Coordinate: x=%d, y=%d.\n\r")
TRACE_MSG(TP_SRV_V0_1413_112_2_18_1_5_54_184,"Self-Adaptive Mode:%d\r\n")
TRACE_MSG(TP_SRV_V0_1468_112_2_18_1_5_54_185,"TP_COORD_INVALID: Coordinate: x=%d, y=%d.\n\r")
TRACE_MSG(TP_SRV_V0_1498_112_2_18_1_5_54_186,"TP_FIRSET_DOWN: Coordinate: TP.cur.x=%d, TP.cur.y=%d.\n\r")
TRACE_MSG(TP_SRV_V0_1541_112_2_18_1_5_54_187,"TP_Not_Moving:TP.cur.x=%d, TP.cur.y=%d,TP.pre.x=%d,TP.pre.y=%d,")
END_TRACE_MAP(MS_REF_BASE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_BASE_TRC_H_

