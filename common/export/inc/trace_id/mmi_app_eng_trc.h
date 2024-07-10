/******************************************************************************
 ** File Name:      mmi_app_eng_trc.h                                         *
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
//trace_id:173
#ifndef _MMI_APP_ENG_TRC_H_
#define _MMI_APP_ENG_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIENG_MAIN_793_112_2_18_2_17_11_0 "mmieng_main.c MMIAPIENG_ParseEngPhoneInfoString str_str == null"
#define MMIENG_MAIN_843_112_2_18_2_17_11_1 "MMIENG_MAIN.C MMIAPIENG_SetSensorMode sensor_mode is %d"
#define MMIENG_MAIN_861_112_2_18_2_17_11_2 "MMIENG_MAIN.C MMIAPIENG_SetChipTestMode test_mode is %d"
#define MMIENG_MAIN_890_112_2_18_2_17_11_3 "MMIENG_MAIN.C MMIAPIENG_GetSensorMode sensor_mode is %d"
#define MMIENG_MAIN_915_112_2_18_2_17_11_4 "MMIENG_MAIN.C MMIAPIENG_GetChipTestMode test_mode is %d"
#define MMIENG_MAIN_928_112_2_18_2_17_11_5 "MMIENG_MAIN.C MMIAPIENG_SetDVFrameFre dv_frame_frequency is %d"
#define MMIENG_MAIN_956_112_2_18_2_17_11_6 "MMIENG_MAIN.C MMIAPIENG_GetDVFrameFre dv_frame_frequency is %d"
#define MMIENG_MAIN_982_112_2_18_2_17_11_7 "mmieng_main.c AppHandleEngMsg app_ptr == null"
#define MMIENG_MAIN_985_112_2_18_2_17_11_8 ">>>>>>>>>  enter AppHandleEngMsg: msg_id = 0x%04X"
#define MMIENG_MAIN_1044_112_2_18_2_17_11_9 "mmieng pclink: ping result not for ours 0x%x, received 0x%x"
#define MMIENG_MAIN_2570_112_2_18_2_17_14_10 "mmieng_main.c MMIAPIENG_SetIQMode iq_mode == null"
#define MMIENG_MAIN_2984_112_2_18_2_17_15_11 "Open file cmmbtest.ini , SFS_HANDLE = %d!"
#define MMIENG_MAIN_2987_112_2_18_2_17_15_12 "Open file d:cmmbtest.ini fail!"
#define MMIENG_MAIN_2992_112_2_18_2_17_15_13 "Open file e:cmmbtest.ini fail!"
#define MMIENG_MAIN_3000_112_2_18_2_17_15_14 "Get file size error!"
#define MMIENG_MAIN_3010_112_2_18_2_17_15_15 "Read file error!"
#define MMIENG_MAIN_3021_112_2_18_2_17_15_16 "CmmbTest_ParseIni parse network_id = %d "
#define MMIENG_MAIN_3026_112_2_18_2_17_15_17 "parse network_id error "
#define MMIENG_MAIN_3037_112_2_18_2_17_15_18 "CmmbTest_ParseIni parse service_id = %d "
#define MMIENG_MAIN_3042_112_2_18_2_17_15_19 "parse service_id error "
#define MMIENG_MAIN_3054_112_2_18_2_17_15_20 "CmmbTest_ParseIni parse rf_search_freq = %d "
#define MMIENG_MAIN_3059_112_2_18_2_17_15_21 "parse search_freq error "
#define MMIENG_MAIN_3071_112_2_18_2_17_15_22 "CmmbTest_ParseIni parse bler_err_blk = %d "
#define MMIENG_MAIN_3076_112_2_18_2_17_15_23 "parse bler_err_blk error "
#define MMIENG_MAIN_3087_112_2_18_2_17_15_24 "CmmbTest_ParseIni parse bler_num_blk = %d "
#define MMIENG_MAIN_3092_112_2_18_2_17_15_25 "parse bler_num_blk error "
#define MMIENG_MAIN_3103_112_2_18_2_17_15_26 "CmmbTest_ParseIni parse rssi_min = %d "
#define MMIENG_MAIN_3108_112_2_18_2_17_15_27 "parse rssi_min error "
#define MMIENG_MAIN_3119_112_2_18_2_17_15_28 "CmmbTest_ParseIni parse rssi_max = %d "
#define MMIENG_MAIN_3124_112_2_18_2_17_15_29 "parse rssi_max error "
#define MMIENG_MAIN_3145_112_2_18_2_17_15_30 "MTV: MBBMS_SERVICE_MANUAL_SG"
#define MMIENG_MAIN_3157_112_2_18_2_17_15_31 "MTV MBBMS_SERVICE_STOP_MANUAL_SG"
#define MMIENG_MAIN_3163_112_2_18_2_17_15_32 "MTV received MBBMS_SERVICE_PLAY"
#define MMIENG_MAIN_3174_112_2_18_2_17_15_33 "MTV received MBBMS_SERVICE_STOP"
#define MMIENG_MAIN_3200_112_2_18_2_17_15_34 "MMIAPIENG_InitMTV start"
#define MMIENG_MAIN_3290_112_2_18_2_17_16_35 "MMIAPIENG_InitMTV end"
#define MMIENG_MAIN_3302_112_2_18_2_17_16_36 "MMIAPIENG_ReleaseMTV"
#define MMIENG_MAIN_3334_112_2_18_2_17_16_37 "MMIAPIENG_StopMTVFlow curr flow status=%d,curr_flow type =%d"
#define MMIENG_MAIN_3354_112_2_18_2_17_16_38 "MMIAPIENG_StopMTVFlow MTVSERVICE_StopParseServiceGuide ret = %d"
#define MMIENG_MAIN_3369_112_2_18_2_17_16_39 "MMIAPIENG_StopMTVFlow MTVSERVICE_Stop ret = %d"
#define MMIENG_MAIN_3395_112_2_18_2_17_16_40 "MMIAPIENG_PlayMTVService  net_id=%d, service_id=%d"
#define MMIENG_MAIN_3412_112_2_18_2_17_16_41 "MMIAPIENG_PlayMTVService  MTVSERVICE_Play return = %d"
#define MMIENG_MAIN_3432_112_2_18_2_17_16_42 "MMIAPIENG_MTVRFTest  net_frequency=%d"
#define MMIENG_MAIN_3451_112_2_18_2_17_16_43 "MMIAPIENG_MTVRFTest  MTVSERVICE_Play return = %d"
#define MMIENG_MAIN_3904_112_2_18_2_17_17_44 "SCI_GetArmLogFlag, retrun = %d"
#define MMIENG_MAIN_3934_112_2_18_2_17_17_45 "SCI_SetArmLogFlag, index = %d"
#define MMIENG_MAIN_3991_112_2_18_2_17_17_46 "REFPARAM_GetDspDebugPortPhyNo, retrun = %d"
#define MMIENG_MAIN_4018_112_2_18_2_17_17_47 "REFPARAM_SetDspDebugPortPhyNo, index = %d"
#define MMIENG_MAIN_4654_112_2_18_2_17_18_48 "MNNV_GetNVPlmnSupportFlagEx: sim index = %d, flag = %d, return=%d"
#define MMIENG_MAIN_4699_112_2_18_2_17_19_49 "MNNV_SetNVPlmnSupportFlagEx: sim index = %d, flag = %d, return=%d"
#define MMIENG_MAIN_4867_112_2_18_2_17_19_50 "[mmieng]GET cmcc test flag = %d"
#define MMIENG_MAIN_4887_112_2_18_2_17_19_51 "mmieng_main.c MMIAPIENG_SetCMCCTestFlag pTestFlag == null"
#define MMIENG_MAIN_4893_112_2_18_2_17_19_52 "[mmieng]SET cmcc test flag = %d"
#define MMIENG_MAIN_5212_112_2_18_2_17_20_53 "mmieng_main.c MMIAPIENG_ParseIdleSetLanguageTypeNumString language_type_ptr == null"
#define MMIENG_MAIN_5282_112_2_18_2_17_20_54 "mmieng_main.c MMIAPIENG_ParseIdleDialNumString opt_type_ptr == null"
#define MMIENG_MAIN_5288_112_2_18_2_17_20_55 "mmieng_main.c MMIAPIENG_ParseIdleDialNumString ctrl_code_ptr == null"
#define MMIENG_MAIN_5326_112_2_18_2_17_20_56 "s_mmieng_idle_dial_num_app_tab size = %d"
#define MMIENG_MAIN_5357_112_2_18_2_17_20_57 "MMIENG_MAIN.C MMIAPIENG_DoIdleDialNumOpt index is %d"
#define MMIENG_UITESTWIN_1404_112_2_18_2_17_26_58 "mmieng_uitestwin.c MiniTimerCallBack input == null"
#define MMIENG_UITESTWIN_1432_112_2_18_2_17_26_59 "mmieng_uitestwin.c MMIENG_MiniTestTimerCallback signal_ptr == null"
#define MMIENG_UITESTWIN_1781_112_2_18_2_17_27_60 "zhaohui Pause3DMMI"
#define MMIENG_UITESTWIN_1803_112_2_18_2_17_27_61 "zhaohui Resume3DMMI"
#define MMIENG_UITESTWIN_1908_112_2_18_2_17_27_62 "TestKeyboard key_msg_id = %d"
#define MMIENG_UITESTWIN_2091_112_2_18_2_17_27_63 "UITestKeyWinHandleMsg msg_id = %x"
#define MMIENG_UITESTWIN_3484_112_2_18_2_17_30_64 "MMIENG DC GetSensorID sensor_id=%d"
#define MMIENG_UITESTWIN_3503_112_2_18_2_17_30_65 "MMIENG DC GetSensorNumber sensor_number=%d"
#define MMIENG_UITESTWIN_3628_112_2_18_2_17_31_66 "[MMIDC] ENG_DC_Preview SENSOR ret=%d"
#define MMIENG_UITESTWIN_3661_112_2_18_2_17_31_67 "[MMIDC] ENG_DC_Preview StartPreview ret=%d"
#define MMIENG_UITESTWIN_3714_112_2_18_2_17_31_68 "[MMIDC]: MMIAPIDC_ForEngWinMsg, msg_id = 0x%.2x"
#define MMIENG_UITESTWIN_4407_112_2_18_2_17_32_69 "MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_OPEN_WINDOW "
#define MMIENG_UITESTWIN_4469_112_2_18_2_17_32_70 "MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_GET_FOCUS "
#define MMIENG_UITESTWIN_4481_112_2_18_2_17_32_71 "MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_LOSE_FOCUS "
#define MMIENG_UITESTWIN_4546_112_2_18_2_17_32_72 "UITestLoopbackSetUlPA: Not support %d mode"
#define MMIENG_UITESTWIN_4972_112_2_18_2_17_33_73 "mmieng_uitestwin.c UITestCOMWinHandleMsg COM0 ret_ptr == null"
#define MMIENG_UITESTWIN_4999_112_2_18_2_17_33_74 "mmieng_uitestwin.c UITestCOMWinHandleMsg COM1 ret_ptr == null"
#define MMIENG_UITESTWIN_5027_112_2_18_2_17_33_75 "mmieng_uitestwin.c UITestCOMWinHandleMsg COM2 ret_ptr == null"
#define MMIENG_UITESTWIN_5381_112_2_18_2_17_34_76 "mmieng_uitestwin.c SetGPSText gps_test_ptr == null"
#define MMIENG_UITESTWIN_5401_112_2_18_2_17_34_77 "mmieng_uitestwin.c SetGPSText gps_info_ptr == null"
#define MMIENG_UITESTWIN_5969_112_2_18_2_17_35_78 "mmieng_uitestwin DRECORDER_SelectSrcDevice ATV result = %d"
#define MMIENG_UITESTWIN_5974_112_2_18_2_17_35_79 "mmieng_uitestwin DRECORDER_Open before, result = %d"
#define MMIENG_UITESTWIN_5976_112_2_18_2_17_35_80 "mmieng_uitestwin DRECORDER_Open result = %d"
#define MMIENG_UITESTWIN_5984_112_2_18_2_17_35_81 "mmieng_uitestwin DCAMERA_ParamCtrl ATV_REGION_CHINA = %d"
#define MMIENG_UITESTWIN_5992_112_2_18_2_17_35_82 "mmieng_uitestwin DCAMERA_ParamCtrl result = %d,ch = %d"
#define MMIENG_UITESTWIN_6021_112_2_18_2_17_35_83 "mmieng_uitestwin DCAMERA_GetInfo, result = %d,ch = %d"
#define MMIENG_UITESTWIN_6463_112_2_18_2_17_36_84 "ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
#define MMIENG_UITESTWIN_6471_112_2_18_2_17_36_85 "ENGVideoDemoWinHandleMsg MSG_GET_FOCUS s_video_cur_state = %d,menu_id=%d"
#define MMIENG_UITESTWIN_6491_112_2_18_2_17_36_86 "ENGVideoDemoWinHandleMsg MSG_GET_FOCUS s_video_cur_state = %d"
#define MMIENG_UITESTWIN_6529_112_2_18_2_17_36_87 "ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
#define MMIENG_UITESTWIN_6558_112_2_18_2_17_37_88 "ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
#define MMIENG_UITESTWIN_6669_112_2_18_2_17_37_89 "ENGAudioDemoWinHandleMsg MSG_APP_UP s_video_cur_state = %d"
#define MMIENG_UITESTWIN_6675_112_2_18_2_17_37_90 "ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_video_cur_state = %d"
#define MMIENG_UITESTWIN_6707_112_2_18_2_17_37_91 "ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_6713_112_2_18_2_17_37_92 "ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_6808_112_2_18_2_17_37_93 "ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_7201_112_2_18_2_17_38_94 "ENGAudioDemoWinHandleMsg MSG_APP_UP s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_7205_112_2_18_2_17_38_95 "ENGAudioDemoWinHandleMsg MSG_APP_UP menu_id = %d"
#define MMIENG_UITESTWIN_7229_112_2_18_2_17_38_96 "ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_7235_112_2_18_2_17_38_97 "ENGAudioDemoWinHandleMsg MSG_APP_DOWN menu_id = %d"
#define MMIENG_UITESTWIN_7262_112_2_18_2_17_38_98 "ENGAudioDemoWinHandleMsg MSG_MEDIA_DEMO_SHOW_DATE MSG_APP_DOWN menu_id = %d"
#define MMIENG_UITESTWIN_7310_112_2_18_2_17_38_99 "ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
#define MMIENG_UITESTWIN_7317_112_2_18_2_17_38_100 "ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
#define MMIENG_UITESTWIN_7435_112_2_18_2_17_38_101 "ENGAudioDemoWinHandleMsg MSG_APP_UP s_video_cur_state = %d"
#define MMIENG_UITESTWIN_7440_112_2_18_2_17_38_102 "ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_video_cur_state = %d"
#define MMIENG_UITESTWIN_7469_112_2_18_2_17_38_103 "ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_7478_112_2_18_2_17_38_104 "ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_7862_112_2_18_2_17_39_105 "ENGAudioDemoWinHandleMsg MSG_APP_UP s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_7868_112_2_18_2_17_39_106 "ENGAudioDemoWinHandleMsg MSG_APP_UP menu_id = %d"
#define MMIENG_UITESTWIN_7890_112_2_18_2_17_39_107 "ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_audio_cur_state = %d"
#define MMIENG_UITESTWIN_7896_112_2_18_2_17_39_108 "ENGAudioDemoWinHandleMsg MSG_APP_DOWN menu_id = %d"
#define MMIENG_UITESTWIN_7921_112_2_18_2_17_39_109 "ENGAudioDemoWinHandleMsg MSG_MEDIA_DEMO_SHOW_DATE MSG_APP_DOWN menu_id = %d"
#define MMIENG_UITESTWIN_8134_112_2_18_2_17_40_110 "MMIENG_UITESTWIN ReadUITestResultNV  return_code = %d"
#define MMIENG_UITESTWIN_8139_112_2_18_2_17_40_111 "MMIENG_UITESTWIN ReadUITestResultNV : is_tested =  %x,  result = %x"
#define MMIENG_UITESTWIN_8176_112_2_18_2_17_40_112 "MMIENG_UITESTWIN WriteUITestResultNV return_code =  %d"
#define MMIENG_UITESTWIN_8186_112_2_18_2_17_40_113 "MMIENG_UITESTWIN WriteUITestResultNV success: is_tested = %x, result = %x"
#define MMIENG_UITESTWIN_8191_112_2_18_2_17_40_114 "MMIENG_UITESTWIN WriteUITestResultNV do nothing: is_tested = %x, result = %x"
#define MMIENG_UITESTWIN_8220_112_2_18_2_17_40_115 " SetTestResult : i = %d,  result = %d"
#define MMIENG_UITESTWIN_8221_112_2_18_2_17_40_116 " SetTestResult :s_is_tested = %x,  s_test_result = %x"
#define MMIENG_UITESTWIN_8236_112_2_18_2_17_40_117 "MMIENG_UITESTWIN SaveTestResult : fulltest = %d, result = %d"
#define MMIENG_UITESTWIN_8292_112_2_18_2_17_40_118 "SetMMITestResultBit :  j = %d, result = 0"
#define MMIENG_UITESTWIN_8299_112_2_18_2_17_40_119 "SetMMITestResultBit :  j = %d, result = 0"
#define MMIENG_UITESTWIN_8305_112_2_18_2_17_40_120 "SetMMITestResultBit :  j = %d, result = 1"
#define MMIENG_UITESTWIN_8326_112_2_18_2_17_40_121 "REFPARAM_GetCaliMarkerValue %x"
#define MMIENG_UITESTWIN_8332_112_2_18_2_17_40_122 "MMIENG_UITESTWIN SetADC7MMITestBit : result = %d,  return_code = %d"
#define MMIENG_UITESTWIN_8448_112_2_18_2_17_40_123 "MSensor_ioctl sensor_data_ex =%d"
#define MMIENG_UITESTWIN_8457_112_2_18_2_17_40_124 "MSensor_ioctl X = %d, Y = %d"
#define MMIENG_UITESTWIN_8462_112_2_18_2_17_40_125 "MSensor_ioctl Z = %d"
#define MMIENG_WIN_3108_112_2_18_2_17_47_126 "EngShowLayer1MonitorWinHandleMsg text.wstr_len is %d"
#define MMIENG_WIN_3317_112_2_18_2_17_47_127 "mmieng_win.c GetSCNetInfoStr net_buf == null"
#define MMIENG_WIN_3321_112_2_18_2_17_47_128 "mmieng_win.c GetSCNetInfoStr buf_len is %d"
#define MMIENG_WIN_3385_112_2_18_2_17_48_129 "mmieng_win.c GetNCNetInfoStr net_buf == null"
#define MMIENG_WIN_3389_112_2_18_2_17_48_130 "mmieng_win.c GetNCNetInfoStr buf_len is %d"
#define MMIENG_WIN_4039_112_2_18_2_17_49_131 "mmieng_win.c GetPhoneInfoStr phone_buf == null"
#define MMIENG_WIN_4043_112_2_18_2_17_49_132 "mmieng_win.c GetPhoneInfoStr buf_len is %d"
#define MMIENG_WIN_4342_112_2_18_2_17_49_133 "mmieng_win.c GetAdcCalibrateInfoStr adc_info_buf == null"
#define MMIENG_WIN_4346_112_2_18_2_17_49_134 "mmieng_win.c GetAdcCalibrateInfoStr buf_len is %d"
#define MMIENG_WIN_4357_112_2_18_2_17_50_135 "GetAdcCalibrateInfoStr %x"
#define MMIENG_WIN_4371_112_2_18_2_17_50_136 "mmieng_win.c GetAdcCalibrateInfoStr array definition error"
#define MMIENG_WIN_4674_112_2_18_2_17_50_137 ">>>>>>>>>  enter EngForbidPLMNWinHandleMsg, msg_id = 0x%02X"
#define MMIENG_WIN_4808_112_2_18_2_17_50_138 "mmieng_win.c GetTCVInfoStr tcv_buf == null"
#define MMIENG_WIN_4811_112_2_18_2_17_50_139 "mmieng_win.c GetTCVInfoStr buf_len is %d"
#define MMIENG_WIN_4882_112_2_18_2_17_51_140 ">>>>>>>>>  enter EngTCVWinHandleMsg, msg_id = 0x%02X"
#define MMIENG_WIN_5156_112_2_18_2_17_51_141 "EngAFCWinHandleMsg: g_mmieng_afc_value is %d"
#define MMIENG_WIN_5231_112_2_18_2_17_51_142 "EngAFCWinHandleMsg: g_mmieng_afc_value is %d"
#define MMIENG_WIN_5662_112_2_18_2_17_52_143 "mmieng_win.c GetAGCInfoStr agc_buf == null"
#define MMIENG_WIN_5665_112_2_18_2_17_52_144 "mmieng_win.c GetAGCInfoStr buf_len is %d"
#define MMIENG_WIN_5701_112_2_18_2_17_52_145 ">>>>>>>>>  enter EngAGCWinHandleMsg, msg_id = 0x%02X"
#define MMIENG_WIN_5831_112_2_18_2_17_53_146 "mmieng_win.c GetErrRateInfoStr rate_buf == null"
#define MMIENG_WIN_5834_112_2_18_2_17_53_147 "mmieng_win.c GetErrRateInfoStr buf_len is %d"
#define MMIENG_WIN_5887_112_2_18_2_17_53_148 ">>>>>>>>>  enter EngErrRateWinHandleMsg, msg_id = 0x%02X"
#define MMIENG_WIN_6032_112_2_18_2_17_53_149 "EngAudioRead:%d"
#define MMIENG_WIN_6090_112_2_18_2_17_53_150 "EngAudioRead"
#define MMIENG_WIN_6111_112_2_18_2_17_53_151 "EngAudioRead:log"
#define MMIENG_WIN_6135_112_2_18_2_17_53_152 "EngAudioRead:end"
#define MMIENG_WIN_7674_112_2_18_2_17_56_153 "mmieng_win.c GetProductionSNStr pd_buf == null"
#define MMIENG_WIN_7677_112_2_18_2_17_56_154 "mmieng_win.c GetProductionSNStr buf_len is %d"
#define MMIENG_WIN_8076_112_2_18_2_17_57_155 "is_on %d"
#define MMIENG_WIN_8115_112_2_18_2_17_57_156 "is_on_close %d"
#define MMIENG_WIN_8126_112_2_18_2_17_57_157 "IS sdcard %d"
#define MMIENG_WIN_8144_112_2_18_2_17_57_158 "is_on_open %d"
#define MMIENG_WIN_8194_112_2_18_2_17_57_159 "MMIENG_WIN.C EngSensorModeWinHandleMsg mode is %d,item_total_num is %d"
#define MMIENG_WIN_8217_112_2_18_2_17_57_160 "MMIENG_WIN.C EngSensorModeWinHandleMsg index is %d,item_total_num is %d"
#define MMIENG_WIN_8286_112_2_18_2_17_57_161 "[mmieng] list_id = %d"
#define MMIENG_WIN_8339_112_2_18_2_17_58_162 "MMIENG_WIN.C EngDVCovFrameFre2ListItemPos dv_frame_frequency is %d"
#define MMIENG_WIN_8384_112_2_18_2_17_58_163 "MMIENG_WIN.C EngDVCovListItemPos2FrameFre item_pos is %d"
#define MMIENG_WIN_8590_112_2_18_2_17_58_164 "MMIENG_WIN.C EngBandSelectWinHandleMsg index is %d"
#define MMIENG_WIN_8604_112_2_18_2_17_58_165 "MMIENG_WIN.C,EngBandSelectWinHandleMsg,MNPHONE_SelectBandEx, sim%d, result = %d"
#define MMIENG_WIN_8909_112_2_18_2_17_59_166 "mmieng_win.c GetRFCellInfo info == null"
#define MMIENG_WIN_9001_112_2_18_2_17_59_167 "MMIENG_WIN.C GetRFCellInfo cell is %d"
#define MMIENG_WIN_9005_112_2_18_2_17_59_168 "MMIENG_MAIN.C GetRFCellInfo cell_index is %d"
#define MMIENG_WIN_9026_112_2_18_2_17_59_169 "MMIENG_WIN.C GetRFCellInfo cell is %d"
#define MMIENG_WIN_9134_112_2_18_2_17_59_170 "MMIENG_WIN.C DrawRFWin index is %d"
#define MMIENG_WIN_9290_112_2_18_2_17_59_171 "MMIENG_WIN.C DrawRFWin cell is %d"
#define MMIENG_WIN_9621_112_2_18_2_18_0_172 "mmieng_win.c SetGPSText text_ptr == null"
#define MMIENG_WIN_9653_112_2_18_2_18_0_173 "mmieng_win.c SetGPSText gps_test_ptr == null"
#define MMIENG_WIN_9662_112_2_18_2_18_0_174 "mmieng_win.c SetGPSText GPS_GetGpsInfo gps_info_ptr == null"
#define MMIENG_WIN_9841_112_2_18_2_18_1_175 "EngShowGPSWinHandleMsg msg_id = 0x%x"
#define MMIENG_WIN_9964_112_2_18_2_18_1_176 " MMIAPIENG_CreateCMMBChannelStatusWin"
#define MMIENG_WIN_10336_112_2_18_2_18_2_177 "HandleChipTestMenuWinMsg(), msg_id = %x"
#define MMIENG_WIN_10403_112_2_18_2_18_2_178 "HandleCMMBTestWaitWinMsg msg MSG_ENG_MTV_PLAY_CNF"
#define MMIENG_WIN_10419_112_2_18_2_18_2_179 "HandleCMMBTestWaitWinMsg msg MSG_ENG_MTV_SEARCH_CNF"
#define MMIENG_WIN_10677_112_2_18_2_18_3_180 "MTVSERVICE_GetCmmbSn ok SN:%s"
#define MMIENG_WIN_10884_112_2_18_2_18_3_181 "[CMMB ENG]: CMMB Channel info Heartbeat:%d  Block count:%d  Error block count:%d"
#define MMIENG_WIN_10887_112_2_18_2_18_3_182 "[CMMB ENG]: CMMB Channel info Frequency offset:%d  RSSI:%d  Padding:%d"
#define MMIENG_WIN_11016_112_2_18_2_18_4_183 "[MMIMTV]: HandleCMMBDisplayWinMsg MSG_OPEN_WINDOW"
#define MMIENG_WIN_11018_112_2_18_2_18_4_184 "[MMIMTV]: MTVSERVICE_GetChannelStatus, return = %d"
#define MMIENG_WIN_11029_112_2_18_2_18_4_185 "[MMIMTV]: MTVSERVICE_GetChannelStatus, return = %d"
#define MMIENG_WIN_11037_112_2_18_2_18_4_186 "[MMIMTV]: HandleCMMBDisplayWinMsg MSG_APP_CANCEL"
#define MMIENG_WIN_11577_112_2_18_2_18_5_187 "HandleChipTestSettingWinMsg(), msg_id = %x"
#define MMIENG_WIN_11620_112_2_18_2_18_5_188 "MMIENG_WIN.C HandleChipTestSettingWinMsg index is %d"
#define MMIENG_WIN_11759_112_2_18_2_18_5_189 "MMIAPIENG Read Pac file: file_len=0x%x"
#define MMIENG_WIN_11809_112_2_18_2_18_5_190 "MMIAPIENG Read Pac file: file_len=0x%x"
#define MMIENG_WIN_11873_112_2_18_2_18_5_191 "MMIAPIENG_ReadBootFile: file_len=0x%x"
#define MMIENG_WIN_11912_112_2_18_2_18_5_192 "MMIAPIENG_CopyBootFile: tf_exec_addr=0x%x"
#define MMIENG_WIN_11943_112_2_18_2_18_5_193 "HandleTFUpWarnWindow: msg_id=0x%x"
#define MMIENG_WIN_12022_112_2_18_2_18_6_194 "MMIENG_WIN HandleTFLoadWinMsg: msg_id=0x%x"
#define MMIENG_WIN_12105_112_2_18_2_18_6_195 "file update file and jump to sram execute"
#define MMIENG_WIN_12119_112_2_18_2_18_6_196 "read boot file  fail "
#define MMIENG_WIN_12125_112_2_18_2_18_6_197 "MMIENG_WIN HandleTFLoadWinMsg: MSG_KEYDOWN_CANCEL"
#define MMIENG_WIN_12131_112_2_18_2_18_6_198 "MMIENG_WIN HandleTFLoadWinMsg: MSG_CLOSE_WINDOW"
#define MMIENG_WIN_12824_112_2_18_2_18_7_199 "MMIENG_WIN.C MMIAPIENG_OpenIQDataWin IQ Mode Is Disable!"
#define MMIENG_WIN_13003_112_2_18_2_18_8_200 "MMIENG_WIN.C HandleIQDataModeSettingWinMsg menu_id=%d"
#define MMIENG_WIN_14287_112_2_18_2_18_10_201 "MMIENG_MAIN.C HandleDCParamScreenSizeWinMsg mode is %d,item_total_num is %d"
#define MMIENG_WIN_14310_112_2_18_2_18_10_202 "MMIENG_MAIN.C HandleDCParamScreenSizeWinMsg mode is %d,item_total_num is %d"
#define MMIENG_WIN_14534_112_2_18_2_18_11_203 "mmieng HandleDCParamSensorAngleWinMsg s_dc_sensor_id=%d"
#define MMIENG_WIN_15604_112_2_18_2_18_13_204 "mmieng_win.c GetTDNCNetInfoStr net_buf == null"
#define MMIENG_WIN_15607_112_2_18_2_18_13_205 "mmieng_win.c GetTDNCNetInfoStr buf_len is %d"
#define MMIENG_WIN_15713_112_2_18_2_18_13_206 "mmieng_win.c MMIENG_Int2Str buf == null"
#define MMIENG_WIN_15716_112_2_18_2_18_13_207 "mmieng_win.c MMIENG_Int2Str buf_len is %d"
#define MMIENG_WIN_15786_112_2_18_2_18_13_208 "GetAdcCalibrateInfoStr %x"
#define MMIENG_WIN_16069_112_2_18_2_18_14_209 "WIFI:info:MMI: HandleWlanRfSetWinMsg WIFISUPP_Command cmd=0x%x, para1=%d"
#define MMIENG_WIN_16075_112_2_18_2_18_14_210 "WIFI:info:MMI: HandleWlanRfSetWinMsg WIFISUPP_Command cmd=0x%x, para1=%d"
#define MMIENG_WIN_16729_112_2_18_2_18_15_211 "standby_start:%lu standby_time:%lu\n"
#define MMIENG_WIN_16742_112_2_18_2_18_15_212 "s_d:%u s_h:%u s_m:%u s_s:%u t_h:%lu t_m:%un"
#define MMIENG_WIN_16783_112_2_18_2_18_15_213 "MMIENG_WIN.C MMIENG_RecordStandbyInit MMI_ReadNVItem = %d"
#define MMIENG_WIN_16784_112_2_18_2_18_15_214 "MMIENG_WIN.C MMIENG_RecordStandbyInit ENALBE = %d"
#define MMIENG_WIN_16808_112_2_18_2_18_15_215 "MMIENG_WIN.C MMI_CalculateStandbyTime2File MMI_ReadNVItem = %d"
#define MMIENG_WIN_16992_112_2_18_2_18_16_216 "MMIENG_WIN ReadIMEINum EFS_NvitemRead error = %d"
#define MMIENG_WIN_17002_112_2_18_2_18_16_217 "MMIENG_WIN ReadIMEINum EFS_NvitemRead error = %d"
#define AAC_STREAM_DEMO_111_112_2_18_2_18_17_218 "aac_dec,[AACLC_InitAACStreamHead] IN! "
#define AAC_STREAM_DEMO_121_112_2_18_2_18_17_219 "aac_dec,[AACLC_InitAACStreamHead] read head info error=! stream_source_size=%d"
#define AAC_STREAM_DEMO_128_112_2_18_2_18_17_220 "aac_dec,[AACLC_InitAACStreamHead]:m4a format"
#define AAC_STREAM_DEMO_135_112_2_18_2_18_17_221 "aac_dec,[AACLC_InitAACStreamHead]:adif format cannot support!"
#define AAC_STREAM_DEMO_141_112_2_18_2_18_17_222 "aac_dec,[AACLC_InitAACStreamHead]:adts format!"
#define AAC_STREAM_DEMO_149_112_2_18_2_18_17_223 "aac_dec,[AACLC_InitAACStreamHead]:aac format adts"
#define AAC_STREAM_DEMO_159_112_2_18_2_18_17_224 "aac_dec,[AACLC_InitAACStreamHead]:the syncword is err!"
#define AAC_STREAM_DEMO_168_112_2_18_2_18_17_225 "aac_dec,[AACLC_InitAACStreamHead]:not AAC-LC,profile=%d !"
#define AAC_STREAM_DEMO_180_112_2_18_2_18_17_226 "aac_dec,[AACLC_InitAACStreamHead]:sr_index > 11, sr_index=%d"
#define AAC_STREAM_DEMO_184_112_2_18_2_18_17_227 "aac_dec,[AACLC_InitAACStreamHead]: sr_index=%d"
#define AAC_STREAM_DEMO_190_112_2_18_2_18_17_228 "aac_dec,[AACLC_InitAACStreamHead]: channel_configuration is err =%d"
#define AAC_STREAM_DEMO_194_112_2_18_2_18_17_229 "aac_dec,[AACLC_InitAACStreamHead]: channel_configuration=%d"
#define AAC_STREAM_DEMO_211_112_2_18_2_18_17_230 "aac_dec,[AACLC_InitAACStreamHead], aac_frame_len=0x%X."
#define AAC_STREAM_DEMO_220_112_2_18_2_18_17_231 "aac_dec,[AACLC_InitAACStreamHead]:aac  m4a format cannot support!"
#define AAC_STREAM_DEMO_244_112_2_18_2_18_17_232 "aac_dec, callback"
#define AAC_STREAM_DEMO_250_112_2_18_2_18_17_233 "aac_dec, callback, not in palying status."
#define AAC_STREAM_DEMO_265_112_2_18_2_18_17_234 "aac_dec, callback, AACLC_InitAACStreamHead= %d"
#define AAC_STREAM_DEMO_269_112_2_18_2_18_17_235 "aac_dec, callback, long_offset_source=%d."
#define AAC_STREAM_DEMO_270_112_2_18_2_18_17_236 "aac_dec, callback, puiDataLength=%d."
#define AAC_STREAM_DEMO_303_112_2_18_2_18_18_237 "aac_dec, aac_dec_dummy_callbak"
#define AAC_STREAM_DEMO_313_112_2_18_2_18_18_238 "aac_dec, start"
#define AAC_STREAM_DEMO_334_112_2_18_2_18_18_239 "aac_dec, open"
#define AAC_STREAM_DEMO_338_112_2_18_2_18_18_240 "aac_dec, open, last operation first not stop"
#define AAC_STREAM_DEMO_346_112_2_18_2_18_18_241 "aac_dec, AAC_Play_Open, AACLC_InitAACStreamHead= %d"
#define AAC_STREAM_DEMO_367_112_2_18_2_18_18_242 "aac_dec, open(), create handle fail!"
#define AAC_STREAM_DEMO_379_112_2_18_2_18_18_243 "aac_dec, start"
#define AAC_STREAM_DEMO_387_112_2_18_2_18_18_244 "aac_dec,source_len= %d"
#define AAC_STREAM_DEMO_399_112_2_18_2_18_18_245 "aac_dec, close"
#define AAC_STREAM_DEMO_420_112_2_18_2_18_18_246 "aac_dec, pause"
#define AAC_STREAM_DEMO_434_112_2_18_2_18_18_247 "aac_dec, resume"
#define AMR_RECORD_DEMO_98_112_2_18_2_18_18_248 "amr_dec, callback"
#define AMR_RECORD_DEMO_106_112_2_18_2_18_18_249 "amr_dec, callback, not in palying status."
#define AMR_RECORD_DEMO_123_112_2_18_2_18_18_250 "amr_dec, amr_count=%d"
#define AMR_RECORD_DEMO_145_112_2_18_2_18_18_251 "amr_dec, *puiDataLength=%d,amr_count=%d"
#define AMR_RECORD_DEMO_171_112_2_18_2_18_18_252 "wav_dec, start"
#define AMR_RECORD_DEMO_190_112_2_18_2_18_18_253 "amr_dec, open"
#define AMR_RECORD_DEMO_194_112_2_18_2_18_18_254 "amr_dec, open, last operation first not stop"
#define AMR_RECORD_DEMO_212_112_2_18_2_18_19_255 "wav_dec, open(), create handle fail!"
#define AMR_RECORD_DEMO_360_112_2_18_2_18_19_256 "AMR_Record_Test: start reoced"
#define AMR_RECORD_DEMO_388_112_2_18_2_18_19_257 "AMR_Record_Test: stop reoced"
#define AMR_RECORD_DEMO_419_112_2_18_2_18_19_258 "amr_dec, start"
#define AMR_RECORD_DEMO_426_112_2_18_2_18_19_259 "amr_dec,source_len= %d"
#define AMR_RECORD_DEMO_438_112_2_18_2_18_19_260 "amr_dec, close"
#define AMR_STREAM_DEMO_73_112_2_18_2_18_19_261 "amr_dec, callback"
#define AMR_STREAM_DEMO_80_112_2_18_2_18_19_262 "amr_dec, callback, not in palying status."
#define AMR_STREAM_DEMO_96_112_2_18_2_18_19_263 "amr_dec, long_offset_source=%d"
#define AMR_STREAM_DEMO_117_112_2_18_2_18_19_264 "amr_dec, *puiDataLength=%d,long_offset_source=%d"
#define AMR_STREAM_DEMO_140_112_2_18_2_18_19_265 "wav_dec, start"
#define AMR_STREAM_DEMO_158_112_2_18_2_18_20_266 "amr_dec, open"
#define AMR_STREAM_DEMO_162_112_2_18_2_18_20_267 "amr_dec, open, last operation first not stop"
#define AMR_STREAM_DEMO_180_112_2_18_2_18_20_268 "wav_dec, open(), create handle fail!"
#define AMR_STREAM_DEMO_192_112_2_18_2_18_20_269 "amr_dec, start"
#define AMR_STREAM_DEMO_200_112_2_18_2_18_20_270 "amr_dec,source_len= %d"
#define AMR_STREAM_DEMO_211_112_2_18_2_18_20_271 "amr_dec, close"
#define AMR_STREAM_DEMO_234_112_2_18_2_18_20_272 "amr_dec, pause"
#define AMR_STREAM_DEMO_249_112_2_18_2_18_20_273 "amr_dec, resume"
#define AUDIO_EXPRESS_DEMO_112_112_2_18_2_18_20_274 "[audio_express_demo]: EXPRESS_DEMO_Process *puiDestCount=%d"
#define AUDIO_EXPRESS_DEMO_131_112_2_18_2_18_20_275 "[audio_express_demo]: EXPRESS_DEMO_SetPara express_process_type=%d "
#define AUDIO_EXPRESS_DEMO_137_112_2_18_2_18_20_276 "[audio_express_demo]: EXPRESS_DEMO_SetPara is err! para=%d "
#define AUDIO_EXPRESS_DEMO_218_112_2_18_2_18_20_277 "[audio_express_demo]: AUDIO_Express_Demo_RegExp"
#define AUDIO_EXPRESS_DEMO_236_112_2_18_2_18_20_278 "[audio_express_demo]: AUDIO_Express_Demo_UnRegExp"
#define AUDIO_EXPRESS_DEMO_251_112_2_18_2_18_20_279 "[audio_express_demo]: AUDIO_Express_Demo_UnRegExp Err,demo_state had INIT!"
#define AUDIO_EXPRESS_DEMO_262_112_2_18_2_18_20_280 "[audio_express_demo]: AUDIO_Express_Demo_Start"
#define AUDIO_EXPRESS_DEMO_279_112_2_18_2_18_20_281 "[audio_express_demo]: AUDIO_Express_Demo_Start Err,demo_state had start!"
#define AUDIO_EXPRESS_DEMO_291_112_2_18_2_18_20_282 "[audio_express_demo]: AUDIO_Express_Demo_Stop"
#define AUDIO_EXPRESS_DEMO_306_112_2_18_2_18_20_283 "[audio_express_demo]: AUDIO_Express_Demo_Stop Err,demo_state had stop!"
#define AUDIO_EXPRESS_DEMO_318_112_2_18_2_18_20_284 "[audio_express_demo]: AUDIO_Express_Demo_SetPar aud_para=%d"
#define AUDIO_EXPRESS_DEMO_339_112_2_18_2_18_21_285 "[audio_express_demo]: AUDIO_Express_Demo_SetPar Err!"
#define AUDIO_EXPRESS_DEMO_361_112_2_18_2_18_21_286 "YM %s"
#define AUDIO_EXPRESS_DEMO_382_112_2_18_2_18_21_287 "file is not exit!"
#define MIC_SAMPLE_DEMO_64_112_2_18_2_18_23_288 "mic_sample_demo, thread entry"
#define MIC_SAMPLE_DEMO_81_112_2_18_2_18_23_289 "[MicSample Demo]: get_time=%d, sample type=%d, get_db=%d"
#define MIC_SAMPLE_DEMO_107_112_2_18_2_18_24_290 "mic_sample_demo, start in"
#define MIC_SAMPLE_DEMO_111_112_2_18_2_18_24_291 "mic_sample_demo have started"
#define MIC_SAMPLE_DEMO_146_112_2_18_2_18_24_292 "mic_sample_demo, start end"
#define MIC_SAMPLE_DEMO_156_112_2_18_2_18_24_293 "mic_sample_demo, close in"
#define MIC_SAMPLE_DEMO_160_112_2_18_2_18_24_294 "mic_sample_demo have clsoed"
#define MIC_SAMPLE_DEMO_179_112_2_18_2_18_24_295 "mic_sample_demo, close end"
#define MP3_STREAM_DEMO_92_112_2_18_2_18_24_296 "mp3_dec, callback"
#define MP3_STREAM_DEMO_98_112_2_18_2_18_24_297 "mp3_dec, callback, not in palying status."
#define MP3_STREAM_DEMO_148_112_2_18_2_18_24_298 "mp3_dec, start"
#define MP3_STREAM_DEMO_165_112_2_18_2_18_24_299 "mp3_dec, open"
#define MP3_STREAM_DEMO_169_112_2_18_2_18_24_300 "mp3_dec, open, last operation first not stop"
#define MP3_STREAM_DEMO_174_112_2_18_2_18_24_301 "mp3_dec, open MP3_RESOUCE_MONE %d"
#define MP3_STREAM_DEMO_178_112_2_18_2_18_24_302 "mp3_dec, open MP3_RESOUCE_MONE %d"
#define MP3_STREAM_DEMO_215_112_2_18_2_18_24_303 "mp3_dec, open(), create handle fail!"
#define MP3_STREAM_DEMO_229_112_2_18_2_18_24_304 "mp3_dec, start"
#define MP3_STREAM_DEMO_237_112_2_18_2_18_24_305 "mp3_dec, source_len= %d"
#define MP3_STREAM_DEMO_248_112_2_18_2_18_24_306 "mp3_dec, close"
#define MP3_STREAM_DEMO_270_112_2_18_2_18_25_307 "mp3_dec, pause"
#define MP3_STREAM_DEMO_285_112_2_18_2_18_25_308 "mp3_dec, resume"
#define WAV_STREAM_DEMO_85_112_2_18_2_18_25_309 "wav_dec, callback"
#define WAV_STREAM_DEMO_92_112_2_18_2_18_25_310 "wav_dec, callback, not in palying status."
#define WAV_STREAM_DEMO_134_112_2_18_2_18_25_311 "wav_dec, i_len %d, offset=%d"
#define WAV_STREAM_DEMO_155_112_2_18_2_18_25_312 "wav_dec, start"
#define WAV_STREAM_DEMO_174_112_2_18_2_18_25_313 "wav_dec, open"
#define WAV_STREAM_DEMO_178_112_2_18_2_18_25_314 "wav_dec, open, last operation first not stop"
#define WAV_STREAM_DEMO_203_112_2_18_2_18_25_315 "wav_dec, open, uiBitPerSample[%d],uiBlockAlign[%d],uiChannelNum[%d],uiDataSize[%d],uiSampleRate[%d],eSubtype[%d]"
#define WAV_STREAM_DEMO_219_112_2_18_2_18_25_316 "wav_dec, open(), create handle fail!"
#define WAV_STREAM_DEMO_231_112_2_18_2_18_25_317 "wav_dec, start"
#define WAV_STREAM_DEMO_242_112_2_18_2_18_25_318 "wav_dec,source_len= %d"
#define WAV_STREAM_DEMO_253_112_2_18_2_18_25_319 "wav_dec, close"
#define WAV_STREAM_DEMO_276_112_2_18_2_18_25_320 "wav_dec, pause"
#define WAV_STREAM_DEMO_291_112_2_18_2_18_25_321 "wav_dec, resume"
#define DC_VIDEO_DEMO_233_112_2_18_2_18_26_322 "[dc_video_demo]: GetData size=%d"
#define DC_VIDEO_DEMO_252_112_2_18_2_18_26_323 "[dc_video_demo]: DCVIDEODEMO_PlayConstruct"
#define DC_VIDEO_DEMO_272_112_2_18_2_18_26_324 "[dc_video_demo]: DCVIDEODEMO_PlayConstruct create  dynamic task!"
#define DC_VIDEO_DEMO_276_112_2_18_2_18_26_325 "[dc_video_demo]: DCVIDEODEMO_PlayConstruct can not create  dynamic task!"
#define DC_VIDEO_DEMO_289_112_2_18_2_18_26_326 "[dc_video_demo]:DCVIDEODEMO_StopDeconstruct CatMv_demo hdcvideo_demo_thread=%d"
#define DC_VIDEO_DEMO_334_112_2_18_2_18_26_327 "[dc_video_demo]: DCVIDEODEMO_StopDeconstruct hdcvideo_demo_thread=%d"
#define DC_VIDEO_DEMO_344_112_2_18_2_18_26_328 "[dc_video_demo]:DCVIDEODEMO_Play hdcvideo_demo_thread=%d"
#define DC_VIDEO_DEMO_365_112_2_18_2_18_26_329 "[dc_video_demo]: DCVIDEODEMO_Stop, hdcvideo_demo_thread=%d"
#define DC_VIDEO_DEMO_369_112_2_18_2_18_26_330 "[dc_video_demo]: DCVIDEODEMO_Stop, error"
#define DC_VIDEO_DEMO_375_112_2_18_2_18_26_331 "[dc_video_demo]: DCVIDEODEMO_Stop, result=%d"
#define DC_VIDEO_DEMO_406_112_2_18_2_18_26_332 "[dc_video_demo]:  DCVIDEODEMO_SendSignal-1- alloc failed!"
#define DC_VIDEO_DEMO_422_112_2_18_2_18_26_333 "[dc_video_demo]: DCVIDEODEMO_SendSignal-2-  alloc failed!"
#define DC_VIDEO_DEMO_461_112_2_18_2_18_26_334 "[dc_video_demo]: DCVIDEODEMO_Thread_Entry"
#define DC_VIDEO_DEMO_475_112_2_18_2_18_26_335 "[dc_video_demo]: DCVIDEODEMO_Thread_Entry,DCVIDEODEMO_PLAY_REQ"
#define DC_VIDEO_DEMO_482_112_2_18_2_18_26_336 "[dc_video_demo]: DCVIDEODEMO_Thread_Entry,DCVIDEODEMO_STOP_REQ"
#define DC_VIDEO_DEMO_496_112_2_18_2_18_26_337 "[dc_video_demo]: DCVIDEODEMO_Thread_Entry,other invalid signal code!"
#define DC_VIDEO_DEMO_512_112_2_18_2_18_26_338 "[dc_video_demo]: DCVIDEO_Demo_Start, s_dec_ctx.demo_state=%d"
#define DC_VIDEO_DEMO_533_112_2_18_2_18_27_339 "[dc_video_demo]: DCVIDEO_Demo_Stop, s_dec_ctx.demo_state=%d"
#define DC_VIDEO_DEMO_552_112_2_18_2_18_27_340 "[dc_video_demo]: DCVIDEO_Demo_Stop out"
#define DISPLAY_YUV_DEMO_233_112_2_18_2_18_27_341 "demo: Video_Demo_yuv_Start"
#define DISPLAY_YUV_DEMO_240_112_2_18_2_18_27_342 "demo: alloc err y_buf_addr[0x%x]"
#define DISPLAY_YUV_DEMO_244_112_2_18_2_18_27_343 "demo: alloc ok y_buf_addr[0x%x]"
#define DISPLAY_YUV_DEMO_250_112_2_18_2_18_27_344 "demo: alloc err u_buf_addr[0x%x]"
#define DISPLAY_YUV_DEMO_254_112_2_18_2_18_27_345 "demo: alloc ok u_buf_addr[0x%x]"
#define DISPLAY_YUV_DEMO_260_112_2_18_2_18_27_346 "demo: alloc err v_buf_addr[0x%x]"
#define DISPLAY_YUV_DEMO_264_112_2_18_2_18_27_347 "demo: alloc ok v_buf_addr[0x%x]"
#define DISPLAY_YUV_DEMO_302_112_2_18_2_18_28_348 "demo:update_display_param,DC_VideoSetDisplayParam fail"
#define DISPLAY_YUV_DEMO_309_112_2_18_2_18_28_349 "demo:, DC_DisplayAFrame fail"
#define DISPLAY_YUV_DEMO_314_112_2_18_2_18_28_350 "demo:cost time [%d]ms"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_ENG_TRC)
TRACE_MSG(MMIENG_MAIN_793_112_2_18_2_17_11_0,"mmieng_main.c MMIAPIENG_ParseEngPhoneInfoString str_str == null")
TRACE_MSG(MMIENG_MAIN_843_112_2_18_2_17_11_1,"MMIENG_MAIN.C MMIAPIENG_SetSensorMode sensor_mode is %d")
TRACE_MSG(MMIENG_MAIN_861_112_2_18_2_17_11_2,"MMIENG_MAIN.C MMIAPIENG_SetChipTestMode test_mode is %d")
TRACE_MSG(MMIENG_MAIN_890_112_2_18_2_17_11_3,"MMIENG_MAIN.C MMIAPIENG_GetSensorMode sensor_mode is %d")
TRACE_MSG(MMIENG_MAIN_915_112_2_18_2_17_11_4,"MMIENG_MAIN.C MMIAPIENG_GetChipTestMode test_mode is %d")
TRACE_MSG(MMIENG_MAIN_928_112_2_18_2_17_11_5,"MMIENG_MAIN.C MMIAPIENG_SetDVFrameFre dv_frame_frequency is %d")
TRACE_MSG(MMIENG_MAIN_956_112_2_18_2_17_11_6,"MMIENG_MAIN.C MMIAPIENG_GetDVFrameFre dv_frame_frequency is %d")
TRACE_MSG(MMIENG_MAIN_982_112_2_18_2_17_11_7,"mmieng_main.c AppHandleEngMsg app_ptr == null")
TRACE_MSG(MMIENG_MAIN_985_112_2_18_2_17_11_8,">>>>>>>>>  enter AppHandleEngMsg: msg_id = 0x%04X")
TRACE_MSG(MMIENG_MAIN_1044_112_2_18_2_17_11_9,"mmieng pclink: ping result not for ours 0x%x, received 0x%x")
TRACE_MSG(MMIENG_MAIN_2570_112_2_18_2_17_14_10,"mmieng_main.c MMIAPIENG_SetIQMode iq_mode == null")
TRACE_MSG(MMIENG_MAIN_2984_112_2_18_2_17_15_11,"Open file cmmbtest.ini , SFS_HANDLE = %d!")
TRACE_MSG(MMIENG_MAIN_2987_112_2_18_2_17_15_12,"Open file d:cmmbtest.ini fail!")
TRACE_MSG(MMIENG_MAIN_2992_112_2_18_2_17_15_13,"Open file e:cmmbtest.ini fail!")
TRACE_MSG(MMIENG_MAIN_3000_112_2_18_2_17_15_14,"Get file size error!")
TRACE_MSG(MMIENG_MAIN_3010_112_2_18_2_17_15_15,"Read file error!")
TRACE_MSG(MMIENG_MAIN_3021_112_2_18_2_17_15_16,"CmmbTest_ParseIni parse network_id = %d ")
TRACE_MSG(MMIENG_MAIN_3026_112_2_18_2_17_15_17,"parse network_id error ")
TRACE_MSG(MMIENG_MAIN_3037_112_2_18_2_17_15_18,"CmmbTest_ParseIni parse service_id = %d ")
TRACE_MSG(MMIENG_MAIN_3042_112_2_18_2_17_15_19,"parse service_id error ")
TRACE_MSG(MMIENG_MAIN_3054_112_2_18_2_17_15_20,"CmmbTest_ParseIni parse rf_search_freq = %d ")
TRACE_MSG(MMIENG_MAIN_3059_112_2_18_2_17_15_21,"parse search_freq error ")
TRACE_MSG(MMIENG_MAIN_3071_112_2_18_2_17_15_22,"CmmbTest_ParseIni parse bler_err_blk = %d ")
TRACE_MSG(MMIENG_MAIN_3076_112_2_18_2_17_15_23,"parse bler_err_blk error ")
TRACE_MSG(MMIENG_MAIN_3087_112_2_18_2_17_15_24,"CmmbTest_ParseIni parse bler_num_blk = %d ")
TRACE_MSG(MMIENG_MAIN_3092_112_2_18_2_17_15_25,"parse bler_num_blk error ")
TRACE_MSG(MMIENG_MAIN_3103_112_2_18_2_17_15_26,"CmmbTest_ParseIni parse rssi_min = %d ")
TRACE_MSG(MMIENG_MAIN_3108_112_2_18_2_17_15_27,"parse rssi_min error ")
TRACE_MSG(MMIENG_MAIN_3119_112_2_18_2_17_15_28,"CmmbTest_ParseIni parse rssi_max = %d ")
TRACE_MSG(MMIENG_MAIN_3124_112_2_18_2_17_15_29,"parse rssi_max error ")
TRACE_MSG(MMIENG_MAIN_3145_112_2_18_2_17_15_30,"MTV: MBBMS_SERVICE_MANUAL_SG")
TRACE_MSG(MMIENG_MAIN_3157_112_2_18_2_17_15_31,"MTV MBBMS_SERVICE_STOP_MANUAL_SG")
TRACE_MSG(MMIENG_MAIN_3163_112_2_18_2_17_15_32,"MTV received MBBMS_SERVICE_PLAY")
TRACE_MSG(MMIENG_MAIN_3174_112_2_18_2_17_15_33,"MTV received MBBMS_SERVICE_STOP")
TRACE_MSG(MMIENG_MAIN_3200_112_2_18_2_17_15_34,"MMIAPIENG_InitMTV start")
TRACE_MSG(MMIENG_MAIN_3290_112_2_18_2_17_16_35,"MMIAPIENG_InitMTV end")
TRACE_MSG(MMIENG_MAIN_3302_112_2_18_2_17_16_36,"MMIAPIENG_ReleaseMTV")
TRACE_MSG(MMIENG_MAIN_3334_112_2_18_2_17_16_37,"MMIAPIENG_StopMTVFlow curr flow status=%d,curr_flow type =%d")
TRACE_MSG(MMIENG_MAIN_3354_112_2_18_2_17_16_38,"MMIAPIENG_StopMTVFlow MTVSERVICE_StopParseServiceGuide ret = %d")
TRACE_MSG(MMIENG_MAIN_3369_112_2_18_2_17_16_39,"MMIAPIENG_StopMTVFlow MTVSERVICE_Stop ret = %d")
TRACE_MSG(MMIENG_MAIN_3395_112_2_18_2_17_16_40,"MMIAPIENG_PlayMTVService  net_id=%d, service_id=%d")
TRACE_MSG(MMIENG_MAIN_3412_112_2_18_2_17_16_41,"MMIAPIENG_PlayMTVService  MTVSERVICE_Play return = %d")
TRACE_MSG(MMIENG_MAIN_3432_112_2_18_2_17_16_42,"MMIAPIENG_MTVRFTest  net_frequency=%d")
TRACE_MSG(MMIENG_MAIN_3451_112_2_18_2_17_16_43,"MMIAPIENG_MTVRFTest  MTVSERVICE_Play return = %d")
TRACE_MSG(MMIENG_MAIN_3904_112_2_18_2_17_17_44,"SCI_GetArmLogFlag, retrun = %d")
TRACE_MSG(MMIENG_MAIN_3934_112_2_18_2_17_17_45,"SCI_SetArmLogFlag, index = %d")
TRACE_MSG(MMIENG_MAIN_3991_112_2_18_2_17_17_46,"REFPARAM_GetDspDebugPortPhyNo, retrun = %d")
TRACE_MSG(MMIENG_MAIN_4018_112_2_18_2_17_17_47,"REFPARAM_SetDspDebugPortPhyNo, index = %d")
TRACE_MSG(MMIENG_MAIN_4654_112_2_18_2_17_18_48,"MNNV_GetNVPlmnSupportFlagEx: sim index = %d, flag = %d, return=%d")
TRACE_MSG(MMIENG_MAIN_4699_112_2_18_2_17_19_49,"MNNV_SetNVPlmnSupportFlagEx: sim index = %d, flag = %d, return=%d")
TRACE_MSG(MMIENG_MAIN_4867_112_2_18_2_17_19_50,"[mmieng]GET cmcc test flag = %d")
TRACE_MSG(MMIENG_MAIN_4887_112_2_18_2_17_19_51,"mmieng_main.c MMIAPIENG_SetCMCCTestFlag pTestFlag == null")
TRACE_MSG(MMIENG_MAIN_4893_112_2_18_2_17_19_52,"[mmieng]SET cmcc test flag = %d")
TRACE_MSG(MMIENG_MAIN_5212_112_2_18_2_17_20_53,"mmieng_main.c MMIAPIENG_ParseIdleSetLanguageTypeNumString language_type_ptr == null")
TRACE_MSG(MMIENG_MAIN_5282_112_2_18_2_17_20_54,"mmieng_main.c MMIAPIENG_ParseIdleDialNumString opt_type_ptr == null")
TRACE_MSG(MMIENG_MAIN_5288_112_2_18_2_17_20_55,"mmieng_main.c MMIAPIENG_ParseIdleDialNumString ctrl_code_ptr == null")
TRACE_MSG(MMIENG_MAIN_5326_112_2_18_2_17_20_56,"s_mmieng_idle_dial_num_app_tab size = %d")
TRACE_MSG(MMIENG_MAIN_5357_112_2_18_2_17_20_57,"MMIENG_MAIN.C MMIAPIENG_DoIdleDialNumOpt index is %d")
TRACE_MSG(MMIENG_UITESTWIN_1404_112_2_18_2_17_26_58,"mmieng_uitestwin.c MiniTimerCallBack input == null")
TRACE_MSG(MMIENG_UITESTWIN_1432_112_2_18_2_17_26_59,"mmieng_uitestwin.c MMIENG_MiniTestTimerCallback signal_ptr == null")
TRACE_MSG(MMIENG_UITESTWIN_1781_112_2_18_2_17_27_60,"zhaohui Pause3DMMI")
TRACE_MSG(MMIENG_UITESTWIN_1803_112_2_18_2_17_27_61,"zhaohui Resume3DMMI")
TRACE_MSG(MMIENG_UITESTWIN_1908_112_2_18_2_17_27_62,"TestKeyboard key_msg_id = %d")
TRACE_MSG(MMIENG_UITESTWIN_2091_112_2_18_2_17_27_63,"UITestKeyWinHandleMsg msg_id = %x")
TRACE_MSG(MMIENG_UITESTWIN_3484_112_2_18_2_17_30_64,"MMIENG DC GetSensorID sensor_id=%d")
TRACE_MSG(MMIENG_UITESTWIN_3503_112_2_18_2_17_30_65,"MMIENG DC GetSensorNumber sensor_number=%d")
TRACE_MSG(MMIENG_UITESTWIN_3628_112_2_18_2_17_31_66,"[MMIDC] ENG_DC_Preview SENSOR ret=%d")
TRACE_MSG(MMIENG_UITESTWIN_3661_112_2_18_2_17_31_67,"[MMIDC] ENG_DC_Preview StartPreview ret=%d")
TRACE_MSG(MMIENG_UITESTWIN_3714_112_2_18_2_17_31_68,"[MMIDC]: MMIAPIDC_ForEngWinMsg, msg_id = 0x%.2x")
TRACE_MSG(MMIENG_UITESTWIN_4407_112_2_18_2_17_32_69,"MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_OPEN_WINDOW ")
TRACE_MSG(MMIENG_UITESTWIN_4469_112_2_18_2_17_32_70,"MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_GET_FOCUS ")
TRACE_MSG(MMIENG_UITESTWIN_4481_112_2_18_2_17_32_71,"MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_LOSE_FOCUS ")
TRACE_MSG(MMIENG_UITESTWIN_4546_112_2_18_2_17_32_72,"UITestLoopbackSetUlPA: Not support %d mode")
TRACE_MSG(MMIENG_UITESTWIN_4972_112_2_18_2_17_33_73,"mmieng_uitestwin.c UITestCOMWinHandleMsg COM0 ret_ptr == null")
TRACE_MSG(MMIENG_UITESTWIN_4999_112_2_18_2_17_33_74,"mmieng_uitestwin.c UITestCOMWinHandleMsg COM1 ret_ptr == null")
TRACE_MSG(MMIENG_UITESTWIN_5027_112_2_18_2_17_33_75,"mmieng_uitestwin.c UITestCOMWinHandleMsg COM2 ret_ptr == null")
TRACE_MSG(MMIENG_UITESTWIN_5381_112_2_18_2_17_34_76,"mmieng_uitestwin.c SetGPSText gps_test_ptr == null")
TRACE_MSG(MMIENG_UITESTWIN_5401_112_2_18_2_17_34_77,"mmieng_uitestwin.c SetGPSText gps_info_ptr == null")
TRACE_MSG(MMIENG_UITESTWIN_5969_112_2_18_2_17_35_78,"mmieng_uitestwin DRECORDER_SelectSrcDevice ATV result = %d")
TRACE_MSG(MMIENG_UITESTWIN_5974_112_2_18_2_17_35_79,"mmieng_uitestwin DRECORDER_Open before, result = %d")
TRACE_MSG(MMIENG_UITESTWIN_5976_112_2_18_2_17_35_80,"mmieng_uitestwin DRECORDER_Open result = %d")
TRACE_MSG(MMIENG_UITESTWIN_5984_112_2_18_2_17_35_81,"mmieng_uitestwin DCAMERA_ParamCtrl ATV_REGION_CHINA = %d")
TRACE_MSG(MMIENG_UITESTWIN_5992_112_2_18_2_17_35_82,"mmieng_uitestwin DCAMERA_ParamCtrl result = %d,ch = %d")
TRACE_MSG(MMIENG_UITESTWIN_6021_112_2_18_2_17_35_83,"mmieng_uitestwin DCAMERA_GetInfo, result = %d,ch = %d")
TRACE_MSG(MMIENG_UITESTWIN_6463_112_2_18_2_17_36_84,"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_6471_112_2_18_2_17_36_85,"ENGVideoDemoWinHandleMsg MSG_GET_FOCUS s_video_cur_state = %d,menu_id=%d")
TRACE_MSG(MMIENG_UITESTWIN_6491_112_2_18_2_17_36_86,"ENGVideoDemoWinHandleMsg MSG_GET_FOCUS s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_6529_112_2_18_2_17_36_87,"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_6558_112_2_18_2_17_37_88,"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_6669_112_2_18_2_17_37_89,"ENGAudioDemoWinHandleMsg MSG_APP_UP s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_6675_112_2_18_2_17_37_90,"ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_6707_112_2_18_2_17_37_91,"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_6713_112_2_18_2_17_37_92,"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_6808_112_2_18_2_17_37_93,"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7201_112_2_18_2_17_38_94,"ENGAudioDemoWinHandleMsg MSG_APP_UP s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7205_112_2_18_2_17_38_95,"ENGAudioDemoWinHandleMsg MSG_APP_UP menu_id = %d")
TRACE_MSG(MMIENG_UITESTWIN_7229_112_2_18_2_17_38_96,"ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7235_112_2_18_2_17_38_97,"ENGAudioDemoWinHandleMsg MSG_APP_DOWN menu_id = %d")
TRACE_MSG(MMIENG_UITESTWIN_7262_112_2_18_2_17_38_98,"ENGAudioDemoWinHandleMsg MSG_MEDIA_DEMO_SHOW_DATE MSG_APP_DOWN menu_id = %d")
TRACE_MSG(MMIENG_UITESTWIN_7310_112_2_18_2_17_38_99,"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7317_112_2_18_2_17_38_100,"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7435_112_2_18_2_17_38_101,"ENGAudioDemoWinHandleMsg MSG_APP_UP s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7440_112_2_18_2_17_38_102,"ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_video_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7469_112_2_18_2_17_38_103,"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7478_112_2_18_2_17_38_104,"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7862_112_2_18_2_17_39_105,"ENGAudioDemoWinHandleMsg MSG_APP_UP s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7868_112_2_18_2_17_39_106,"ENGAudioDemoWinHandleMsg MSG_APP_UP menu_id = %d")
TRACE_MSG(MMIENG_UITESTWIN_7890_112_2_18_2_17_39_107,"ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_audio_cur_state = %d")
TRACE_MSG(MMIENG_UITESTWIN_7896_112_2_18_2_17_39_108,"ENGAudioDemoWinHandleMsg MSG_APP_DOWN menu_id = %d")
TRACE_MSG(MMIENG_UITESTWIN_7921_112_2_18_2_17_39_109,"ENGAudioDemoWinHandleMsg MSG_MEDIA_DEMO_SHOW_DATE MSG_APP_DOWN menu_id = %d")
TRACE_MSG(MMIENG_UITESTWIN_8134_112_2_18_2_17_40_110,"MMIENG_UITESTWIN ReadUITestResultNV  return_code = %d")
TRACE_MSG(MMIENG_UITESTWIN_8139_112_2_18_2_17_40_111,"MMIENG_UITESTWIN ReadUITestResultNV : is_tested =  %x,  result = %x")
TRACE_MSG(MMIENG_UITESTWIN_8176_112_2_18_2_17_40_112,"MMIENG_UITESTWIN WriteUITestResultNV return_code =  %d")
TRACE_MSG(MMIENG_UITESTWIN_8186_112_2_18_2_17_40_113,"MMIENG_UITESTWIN WriteUITestResultNV success: is_tested = %x, result = %x")
TRACE_MSG(MMIENG_UITESTWIN_8191_112_2_18_2_17_40_114,"MMIENG_UITESTWIN WriteUITestResultNV do nothing: is_tested = %x, result = %x")
TRACE_MSG(MMIENG_UITESTWIN_8220_112_2_18_2_17_40_115," SetTestResult : i = %d,  result = %d")
TRACE_MSG(MMIENG_UITESTWIN_8221_112_2_18_2_17_40_116," SetTestResult :s_is_tested = %x,  s_test_result = %x")
TRACE_MSG(MMIENG_UITESTWIN_8236_112_2_18_2_17_40_117,"MMIENG_UITESTWIN SaveTestResult : fulltest = %d, result = %d")
TRACE_MSG(MMIENG_UITESTWIN_8292_112_2_18_2_17_40_118,"SetMMITestResultBit :  j = %d, result = 0")
TRACE_MSG(MMIENG_UITESTWIN_8299_112_2_18_2_17_40_119,"SetMMITestResultBit :  j = %d, result = 0")
TRACE_MSG(MMIENG_UITESTWIN_8305_112_2_18_2_17_40_120,"SetMMITestResultBit :  j = %d, result = 1")
TRACE_MSG(MMIENG_UITESTWIN_8326_112_2_18_2_17_40_121,"REFPARAM_GetCaliMarkerValue %x")
TRACE_MSG(MMIENG_UITESTWIN_8332_112_2_18_2_17_40_122,"MMIENG_UITESTWIN SetADC7MMITestBit : result = %d,  return_code = %d")
TRACE_MSG(MMIENG_UITESTWIN_8448_112_2_18_2_17_40_123,"MSensor_ioctl sensor_data_ex =%d")
TRACE_MSG(MMIENG_UITESTWIN_8457_112_2_18_2_17_40_124,"MSensor_ioctl X = %d, Y = %d")
TRACE_MSG(MMIENG_UITESTWIN_8462_112_2_18_2_17_40_125,"MSensor_ioctl Z = %d")
TRACE_MSG(MMIENG_WIN_3108_112_2_18_2_17_47_126,"EngShowLayer1MonitorWinHandleMsg text.wstr_len is %d")
TRACE_MSG(MMIENG_WIN_3317_112_2_18_2_17_47_127,"mmieng_win.c GetSCNetInfoStr net_buf == null")
TRACE_MSG(MMIENG_WIN_3321_112_2_18_2_17_47_128,"mmieng_win.c GetSCNetInfoStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_3385_112_2_18_2_17_48_129,"mmieng_win.c GetNCNetInfoStr net_buf == null")
TRACE_MSG(MMIENG_WIN_3389_112_2_18_2_17_48_130,"mmieng_win.c GetNCNetInfoStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_4039_112_2_18_2_17_49_131,"mmieng_win.c GetPhoneInfoStr phone_buf == null")
TRACE_MSG(MMIENG_WIN_4043_112_2_18_2_17_49_132,"mmieng_win.c GetPhoneInfoStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_4342_112_2_18_2_17_49_133,"mmieng_win.c GetAdcCalibrateInfoStr adc_info_buf == null")
TRACE_MSG(MMIENG_WIN_4346_112_2_18_2_17_49_134,"mmieng_win.c GetAdcCalibrateInfoStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_4357_112_2_18_2_17_50_135,"GetAdcCalibrateInfoStr %x")
TRACE_MSG(MMIENG_WIN_4371_112_2_18_2_17_50_136,"mmieng_win.c GetAdcCalibrateInfoStr array definition error")
TRACE_MSG(MMIENG_WIN_4674_112_2_18_2_17_50_137,">>>>>>>>>  enter EngForbidPLMNWinHandleMsg, msg_id = 0x%02X")
TRACE_MSG(MMIENG_WIN_4808_112_2_18_2_17_50_138,"mmieng_win.c GetTCVInfoStr tcv_buf == null")
TRACE_MSG(MMIENG_WIN_4811_112_2_18_2_17_50_139,"mmieng_win.c GetTCVInfoStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_4882_112_2_18_2_17_51_140,">>>>>>>>>  enter EngTCVWinHandleMsg, msg_id = 0x%02X")
TRACE_MSG(MMIENG_WIN_5156_112_2_18_2_17_51_141,"EngAFCWinHandleMsg: g_mmieng_afc_value is %d")
TRACE_MSG(MMIENG_WIN_5231_112_2_18_2_17_51_142,"EngAFCWinHandleMsg: g_mmieng_afc_value is %d")
TRACE_MSG(MMIENG_WIN_5662_112_2_18_2_17_52_143,"mmieng_win.c GetAGCInfoStr agc_buf == null")
TRACE_MSG(MMIENG_WIN_5665_112_2_18_2_17_52_144,"mmieng_win.c GetAGCInfoStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_5701_112_2_18_2_17_52_145,">>>>>>>>>  enter EngAGCWinHandleMsg, msg_id = 0x%02X")
TRACE_MSG(MMIENG_WIN_5831_112_2_18_2_17_53_146,"mmieng_win.c GetErrRateInfoStr rate_buf == null")
TRACE_MSG(MMIENG_WIN_5834_112_2_18_2_17_53_147,"mmieng_win.c GetErrRateInfoStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_5887_112_2_18_2_17_53_148,">>>>>>>>>  enter EngErrRateWinHandleMsg, msg_id = 0x%02X")
TRACE_MSG(MMIENG_WIN_6032_112_2_18_2_17_53_149,"EngAudioRead:%d")
TRACE_MSG(MMIENG_WIN_6090_112_2_18_2_17_53_150,"EngAudioRead")
TRACE_MSG(MMIENG_WIN_6111_112_2_18_2_17_53_151,"EngAudioRead:log")
TRACE_MSG(MMIENG_WIN_6135_112_2_18_2_17_53_152,"EngAudioRead:end")
TRACE_MSG(MMIENG_WIN_7674_112_2_18_2_17_56_153,"mmieng_win.c GetProductionSNStr pd_buf == null")
TRACE_MSG(MMIENG_WIN_7677_112_2_18_2_17_56_154,"mmieng_win.c GetProductionSNStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_8076_112_2_18_2_17_57_155,"is_on %d")
TRACE_MSG(MMIENG_WIN_8115_112_2_18_2_17_57_156,"is_on_close %d")
TRACE_MSG(MMIENG_WIN_8126_112_2_18_2_17_57_157,"IS sdcard %d")
TRACE_MSG(MMIENG_WIN_8144_112_2_18_2_17_57_158,"is_on_open %d")
TRACE_MSG(MMIENG_WIN_8194_112_2_18_2_17_57_159,"MMIENG_WIN.C EngSensorModeWinHandleMsg mode is %d,item_total_num is %d")
TRACE_MSG(MMIENG_WIN_8217_112_2_18_2_17_57_160,"MMIENG_WIN.C EngSensorModeWinHandleMsg index is %d,item_total_num is %d")
TRACE_MSG(MMIENG_WIN_8286_112_2_18_2_17_57_161,"[mmieng] list_id = %d")
TRACE_MSG(MMIENG_WIN_8339_112_2_18_2_17_58_162,"MMIENG_WIN.C EngDVCovFrameFre2ListItemPos dv_frame_frequency is %d")
TRACE_MSG(MMIENG_WIN_8384_112_2_18_2_17_58_163,"MMIENG_WIN.C EngDVCovListItemPos2FrameFre item_pos is %d")
TRACE_MSG(MMIENG_WIN_8590_112_2_18_2_17_58_164,"MMIENG_WIN.C EngBandSelectWinHandleMsg index is %d")
TRACE_MSG(MMIENG_WIN_8604_112_2_18_2_17_58_165,"MMIENG_WIN.C,EngBandSelectWinHandleMsg,MNPHONE_SelectBandEx, sim%d, result = %d")
TRACE_MSG(MMIENG_WIN_8909_112_2_18_2_17_59_166,"mmieng_win.c GetRFCellInfo info == null")
TRACE_MSG(MMIENG_WIN_9001_112_2_18_2_17_59_167,"MMIENG_WIN.C GetRFCellInfo cell is %d")
TRACE_MSG(MMIENG_WIN_9005_112_2_18_2_17_59_168,"MMIENG_MAIN.C GetRFCellInfo cell_index is %d")
TRACE_MSG(MMIENG_WIN_9026_112_2_18_2_17_59_169,"MMIENG_WIN.C GetRFCellInfo cell is %d")
TRACE_MSG(MMIENG_WIN_9134_112_2_18_2_17_59_170,"MMIENG_WIN.C DrawRFWin index is %d")
TRACE_MSG(MMIENG_WIN_9290_112_2_18_2_17_59_171,"MMIENG_WIN.C DrawRFWin cell is %d")
TRACE_MSG(MMIENG_WIN_9621_112_2_18_2_18_0_172,"mmieng_win.c SetGPSText text_ptr == null")
TRACE_MSG(MMIENG_WIN_9653_112_2_18_2_18_0_173,"mmieng_win.c SetGPSText gps_test_ptr == null")
TRACE_MSG(MMIENG_WIN_9662_112_2_18_2_18_0_174,"mmieng_win.c SetGPSText GPS_GetGpsInfo gps_info_ptr == null")
TRACE_MSG(MMIENG_WIN_9841_112_2_18_2_18_1_175,"EngShowGPSWinHandleMsg msg_id = 0x%x")
TRACE_MSG(MMIENG_WIN_9964_112_2_18_2_18_1_176," MMIAPIENG_CreateCMMBChannelStatusWin")
TRACE_MSG(MMIENG_WIN_10336_112_2_18_2_18_2_177,"HandleChipTestMenuWinMsg(), msg_id = %x")
TRACE_MSG(MMIENG_WIN_10403_112_2_18_2_18_2_178,"HandleCMMBTestWaitWinMsg msg MSG_ENG_MTV_PLAY_CNF")
TRACE_MSG(MMIENG_WIN_10419_112_2_18_2_18_2_179,"HandleCMMBTestWaitWinMsg msg MSG_ENG_MTV_SEARCH_CNF")
TRACE_MSG(MMIENG_WIN_10677_112_2_18_2_18_3_180,"MTVSERVICE_GetCmmbSn ok SN:%s")
TRACE_MSG(MMIENG_WIN_10884_112_2_18_2_18_3_181,"[CMMB ENG]: CMMB Channel info Heartbeat:%d  Block count:%d  Error block count:%d")
TRACE_MSG(MMIENG_WIN_10887_112_2_18_2_18_3_182,"[CMMB ENG]: CMMB Channel info Frequency offset:%d  RSSI:%d  Padding:%d")
TRACE_MSG(MMIENG_WIN_11016_112_2_18_2_18_4_183,"[MMIMTV]: HandleCMMBDisplayWinMsg MSG_OPEN_WINDOW")
TRACE_MSG(MMIENG_WIN_11018_112_2_18_2_18_4_184,"[MMIMTV]: MTVSERVICE_GetChannelStatus, return = %d")
TRACE_MSG(MMIENG_WIN_11029_112_2_18_2_18_4_185,"[MMIMTV]: MTVSERVICE_GetChannelStatus, return = %d")
TRACE_MSG(MMIENG_WIN_11037_112_2_18_2_18_4_186,"[MMIMTV]: HandleCMMBDisplayWinMsg MSG_APP_CANCEL")
TRACE_MSG(MMIENG_WIN_11577_112_2_18_2_18_5_187,"HandleChipTestSettingWinMsg(), msg_id = %x")
TRACE_MSG(MMIENG_WIN_11620_112_2_18_2_18_5_188,"MMIENG_WIN.C HandleChipTestSettingWinMsg index is %d")
TRACE_MSG(MMIENG_WIN_11759_112_2_18_2_18_5_189,"MMIAPIENG Read Pac file: file_len=0x%x")
TRACE_MSG(MMIENG_WIN_11809_112_2_18_2_18_5_190,"MMIAPIENG Read Pac file: file_len=0x%x")
TRACE_MSG(MMIENG_WIN_11873_112_2_18_2_18_5_191,"MMIAPIENG_ReadBootFile: file_len=0x%x")
TRACE_MSG(MMIENG_WIN_11912_112_2_18_2_18_5_192,"MMIAPIENG_CopyBootFile: tf_exec_addr=0x%x")
TRACE_MSG(MMIENG_WIN_11943_112_2_18_2_18_5_193,"HandleTFUpWarnWindow: msg_id=0x%x")
TRACE_MSG(MMIENG_WIN_12022_112_2_18_2_18_6_194,"MMIENG_WIN HandleTFLoadWinMsg: msg_id=0x%x")
TRACE_MSG(MMIENG_WIN_12105_112_2_18_2_18_6_195,"file update file and jump to sram execute")
TRACE_MSG(MMIENG_WIN_12119_112_2_18_2_18_6_196,"read boot file  fail ")
TRACE_MSG(MMIENG_WIN_12125_112_2_18_2_18_6_197,"MMIENG_WIN HandleTFLoadWinMsg: MSG_KEYDOWN_CANCEL")
TRACE_MSG(MMIENG_WIN_12131_112_2_18_2_18_6_198,"MMIENG_WIN HandleTFLoadWinMsg: MSG_CLOSE_WINDOW")
TRACE_MSG(MMIENG_WIN_12824_112_2_18_2_18_7_199,"MMIENG_WIN.C MMIAPIENG_OpenIQDataWin IQ Mode Is Disable!")
TRACE_MSG(MMIENG_WIN_13003_112_2_18_2_18_8_200,"MMIENG_WIN.C HandleIQDataModeSettingWinMsg menu_id=%d")
TRACE_MSG(MMIENG_WIN_14287_112_2_18_2_18_10_201,"MMIENG_MAIN.C HandleDCParamScreenSizeWinMsg mode is %d,item_total_num is %d")
TRACE_MSG(MMIENG_WIN_14310_112_2_18_2_18_10_202,"MMIENG_MAIN.C HandleDCParamScreenSizeWinMsg mode is %d,item_total_num is %d")
TRACE_MSG(MMIENG_WIN_14534_112_2_18_2_18_11_203,"mmieng HandleDCParamSensorAngleWinMsg s_dc_sensor_id=%d")
TRACE_MSG(MMIENG_WIN_15604_112_2_18_2_18_13_204,"mmieng_win.c GetTDNCNetInfoStr net_buf == null")
TRACE_MSG(MMIENG_WIN_15607_112_2_18_2_18_13_205,"mmieng_win.c GetTDNCNetInfoStr buf_len is %d")
TRACE_MSG(MMIENG_WIN_15713_112_2_18_2_18_13_206,"mmieng_win.c MMIENG_Int2Str buf == null")
TRACE_MSG(MMIENG_WIN_15716_112_2_18_2_18_13_207,"mmieng_win.c MMIENG_Int2Str buf_len is %d")
TRACE_MSG(MMIENG_WIN_15786_112_2_18_2_18_13_208,"GetAdcCalibrateInfoStr %x")
TRACE_MSG(MMIENG_WIN_16069_112_2_18_2_18_14_209,"WIFI:info:MMI: HandleWlanRfSetWinMsg WIFISUPP_Command cmd=0x%x, para1=%d")
TRACE_MSG(MMIENG_WIN_16075_112_2_18_2_18_14_210,"WIFI:info:MMI: HandleWlanRfSetWinMsg WIFISUPP_Command cmd=0x%x, para1=%d")
TRACE_MSG(MMIENG_WIN_16729_112_2_18_2_18_15_211,"standby_start:%lu standby_time:%lu\n")
TRACE_MSG(MMIENG_WIN_16742_112_2_18_2_18_15_212,"s_d:%u s_h:%u s_m:%u s_s:%u t_h:%lu t_m:%un")
TRACE_MSG(MMIENG_WIN_16783_112_2_18_2_18_15_213,"MMIENG_WIN.C MMIENG_RecordStandbyInit MMI_ReadNVItem = %d")
TRACE_MSG(MMIENG_WIN_16784_112_2_18_2_18_15_214,"MMIENG_WIN.C MMIENG_RecordStandbyInit ENALBE = %d")
TRACE_MSG(MMIENG_WIN_16808_112_2_18_2_18_15_215,"MMIENG_WIN.C MMI_CalculateStandbyTime2File MMI_ReadNVItem = %d")
TRACE_MSG(MMIENG_WIN_16992_112_2_18_2_18_16_216,"MMIENG_WIN ReadIMEINum EFS_NvitemRead error = %d")
TRACE_MSG(MMIENG_WIN_17002_112_2_18_2_18_16_217,"MMIENG_WIN ReadIMEINum EFS_NvitemRead error = %d")
TRACE_MSG(AAC_STREAM_DEMO_111_112_2_18_2_18_17_218,"aac_dec,[AACLC_InitAACStreamHead] IN! ")
TRACE_MSG(AAC_STREAM_DEMO_121_112_2_18_2_18_17_219,"aac_dec,[AACLC_InitAACStreamHead] read head info error=! stream_source_size=%d")
TRACE_MSG(AAC_STREAM_DEMO_128_112_2_18_2_18_17_220,"aac_dec,[AACLC_InitAACStreamHead]:m4a format")
TRACE_MSG(AAC_STREAM_DEMO_135_112_2_18_2_18_17_221,"aac_dec,[AACLC_InitAACStreamHead]:adif format cannot support!")
TRACE_MSG(AAC_STREAM_DEMO_141_112_2_18_2_18_17_222,"aac_dec,[AACLC_InitAACStreamHead]:adts format!")
TRACE_MSG(AAC_STREAM_DEMO_149_112_2_18_2_18_17_223,"aac_dec,[AACLC_InitAACStreamHead]:aac format adts")
TRACE_MSG(AAC_STREAM_DEMO_159_112_2_18_2_18_17_224,"aac_dec,[AACLC_InitAACStreamHead]:the syncword is err!")
TRACE_MSG(AAC_STREAM_DEMO_168_112_2_18_2_18_17_225,"aac_dec,[AACLC_InitAACStreamHead]:not AAC-LC,profile=%d !")
TRACE_MSG(AAC_STREAM_DEMO_180_112_2_18_2_18_17_226,"aac_dec,[AACLC_InitAACStreamHead]:sr_index > 11, sr_index=%d")
TRACE_MSG(AAC_STREAM_DEMO_184_112_2_18_2_18_17_227,"aac_dec,[AACLC_InitAACStreamHead]: sr_index=%d")
TRACE_MSG(AAC_STREAM_DEMO_190_112_2_18_2_18_17_228,"aac_dec,[AACLC_InitAACStreamHead]: channel_configuration is err =%d")
TRACE_MSG(AAC_STREAM_DEMO_194_112_2_18_2_18_17_229,"aac_dec,[AACLC_InitAACStreamHead]: channel_configuration=%d")
TRACE_MSG(AAC_STREAM_DEMO_211_112_2_18_2_18_17_230,"aac_dec,[AACLC_InitAACStreamHead], aac_frame_len=0x%X.")
TRACE_MSG(AAC_STREAM_DEMO_220_112_2_18_2_18_17_231,"aac_dec,[AACLC_InitAACStreamHead]:aac  m4a format cannot support!")
TRACE_MSG(AAC_STREAM_DEMO_244_112_2_18_2_18_17_232,"aac_dec, callback")
TRACE_MSG(AAC_STREAM_DEMO_250_112_2_18_2_18_17_233,"aac_dec, callback, not in palying status.")
TRACE_MSG(AAC_STREAM_DEMO_265_112_2_18_2_18_17_234,"aac_dec, callback, AACLC_InitAACStreamHead= %d")
TRACE_MSG(AAC_STREAM_DEMO_269_112_2_18_2_18_17_235,"aac_dec, callback, long_offset_source=%d.")
TRACE_MSG(AAC_STREAM_DEMO_270_112_2_18_2_18_17_236,"aac_dec, callback, puiDataLength=%d.")
TRACE_MSG(AAC_STREAM_DEMO_303_112_2_18_2_18_18_237,"aac_dec, aac_dec_dummy_callbak")
TRACE_MSG(AAC_STREAM_DEMO_313_112_2_18_2_18_18_238,"aac_dec, start")
TRACE_MSG(AAC_STREAM_DEMO_334_112_2_18_2_18_18_239,"aac_dec, open")
TRACE_MSG(AAC_STREAM_DEMO_338_112_2_18_2_18_18_240,"aac_dec, open, last operation first not stop")
TRACE_MSG(AAC_STREAM_DEMO_346_112_2_18_2_18_18_241,"aac_dec, AAC_Play_Open, AACLC_InitAACStreamHead= %d")
TRACE_MSG(AAC_STREAM_DEMO_367_112_2_18_2_18_18_242,"aac_dec, open(), create handle fail!")
TRACE_MSG(AAC_STREAM_DEMO_379_112_2_18_2_18_18_243,"aac_dec, start")
TRACE_MSG(AAC_STREAM_DEMO_387_112_2_18_2_18_18_244,"aac_dec,source_len= %d")
TRACE_MSG(AAC_STREAM_DEMO_399_112_2_18_2_18_18_245,"aac_dec, close")
TRACE_MSG(AAC_STREAM_DEMO_420_112_2_18_2_18_18_246,"aac_dec, pause")
TRACE_MSG(AAC_STREAM_DEMO_434_112_2_18_2_18_18_247,"aac_dec, resume")
TRACE_MSG(AMR_RECORD_DEMO_98_112_2_18_2_18_18_248,"amr_dec, callback")
TRACE_MSG(AMR_RECORD_DEMO_106_112_2_18_2_18_18_249,"amr_dec, callback, not in palying status.")
TRACE_MSG(AMR_RECORD_DEMO_123_112_2_18_2_18_18_250,"amr_dec, amr_count=%d")
TRACE_MSG(AMR_RECORD_DEMO_145_112_2_18_2_18_18_251,"amr_dec, *puiDataLength=%d,amr_count=%d")
TRACE_MSG(AMR_RECORD_DEMO_171_112_2_18_2_18_18_252,"wav_dec, start")
TRACE_MSG(AMR_RECORD_DEMO_190_112_2_18_2_18_18_253,"amr_dec, open")
TRACE_MSG(AMR_RECORD_DEMO_194_112_2_18_2_18_18_254,"amr_dec, open, last operation first not stop")
TRACE_MSG(AMR_RECORD_DEMO_212_112_2_18_2_18_19_255,"wav_dec, open(), create handle fail!")
TRACE_MSG(AMR_RECORD_DEMO_360_112_2_18_2_18_19_256,"AMR_Record_Test: start reoced")
TRACE_MSG(AMR_RECORD_DEMO_388_112_2_18_2_18_19_257,"AMR_Record_Test: stop reoced")
TRACE_MSG(AMR_RECORD_DEMO_419_112_2_18_2_18_19_258,"amr_dec, start")
TRACE_MSG(AMR_RECORD_DEMO_426_112_2_18_2_18_19_259,"amr_dec,source_len= %d")
TRACE_MSG(AMR_RECORD_DEMO_438_112_2_18_2_18_19_260,"amr_dec, close")
TRACE_MSG(AMR_STREAM_DEMO_73_112_2_18_2_18_19_261,"amr_dec, callback")
TRACE_MSG(AMR_STREAM_DEMO_80_112_2_18_2_18_19_262,"amr_dec, callback, not in palying status.")
TRACE_MSG(AMR_STREAM_DEMO_96_112_2_18_2_18_19_263,"amr_dec, long_offset_source=%d")
TRACE_MSG(AMR_STREAM_DEMO_117_112_2_18_2_18_19_264,"amr_dec, *puiDataLength=%d,long_offset_source=%d")
TRACE_MSG(AMR_STREAM_DEMO_140_112_2_18_2_18_19_265,"wav_dec, start")
TRACE_MSG(AMR_STREAM_DEMO_158_112_2_18_2_18_20_266,"amr_dec, open")
TRACE_MSG(AMR_STREAM_DEMO_162_112_2_18_2_18_20_267,"amr_dec, open, last operation first not stop")
TRACE_MSG(AMR_STREAM_DEMO_180_112_2_18_2_18_20_268,"wav_dec, open(), create handle fail!")
TRACE_MSG(AMR_STREAM_DEMO_192_112_2_18_2_18_20_269,"amr_dec, start")
TRACE_MSG(AMR_STREAM_DEMO_200_112_2_18_2_18_20_270,"amr_dec,source_len= %d")
TRACE_MSG(AMR_STREAM_DEMO_211_112_2_18_2_18_20_271,"amr_dec, close")
TRACE_MSG(AMR_STREAM_DEMO_234_112_2_18_2_18_20_272,"amr_dec, pause")
TRACE_MSG(AMR_STREAM_DEMO_249_112_2_18_2_18_20_273,"amr_dec, resume")
TRACE_MSG(AUDIO_EXPRESS_DEMO_112_112_2_18_2_18_20_274,"[audio_express_demo]: EXPRESS_DEMO_Process *puiDestCount=%d")
TRACE_MSG(AUDIO_EXPRESS_DEMO_131_112_2_18_2_18_20_275,"[audio_express_demo]: EXPRESS_DEMO_SetPara express_process_type=%d ")
TRACE_MSG(AUDIO_EXPRESS_DEMO_137_112_2_18_2_18_20_276,"[audio_express_demo]: EXPRESS_DEMO_SetPara is err! para=%d ")
TRACE_MSG(AUDIO_EXPRESS_DEMO_218_112_2_18_2_18_20_277,"[audio_express_demo]: AUDIO_Express_Demo_RegExp")
TRACE_MSG(AUDIO_EXPRESS_DEMO_236_112_2_18_2_18_20_278,"[audio_express_demo]: AUDIO_Express_Demo_UnRegExp")
TRACE_MSG(AUDIO_EXPRESS_DEMO_251_112_2_18_2_18_20_279,"[audio_express_demo]: AUDIO_Express_Demo_UnRegExp Err,demo_state had INIT!")
TRACE_MSG(AUDIO_EXPRESS_DEMO_262_112_2_18_2_18_20_280,"[audio_express_demo]: AUDIO_Express_Demo_Start")
TRACE_MSG(AUDIO_EXPRESS_DEMO_279_112_2_18_2_18_20_281,"[audio_express_demo]: AUDIO_Express_Demo_Start Err,demo_state had start!")
TRACE_MSG(AUDIO_EXPRESS_DEMO_291_112_2_18_2_18_20_282,"[audio_express_demo]: AUDIO_Express_Demo_Stop")
TRACE_MSG(AUDIO_EXPRESS_DEMO_306_112_2_18_2_18_20_283,"[audio_express_demo]: AUDIO_Express_Demo_Stop Err,demo_state had stop!")
TRACE_MSG(AUDIO_EXPRESS_DEMO_318_112_2_18_2_18_20_284,"[audio_express_demo]: AUDIO_Express_Demo_SetPar aud_para=%d")
TRACE_MSG(AUDIO_EXPRESS_DEMO_339_112_2_18_2_18_21_285,"[audio_express_demo]: AUDIO_Express_Demo_SetPar Err!")
TRACE_MSG(AUDIO_EXPRESS_DEMO_361_112_2_18_2_18_21_286,"YM %s")
TRACE_MSG(AUDIO_EXPRESS_DEMO_382_112_2_18_2_18_21_287,"file is not exit!")
TRACE_MSG(MIC_SAMPLE_DEMO_64_112_2_18_2_18_23_288,"mic_sample_demo, thread entry")
TRACE_MSG(MIC_SAMPLE_DEMO_81_112_2_18_2_18_23_289,"[MicSample Demo]: get_time=%d, sample type=%d, get_db=%d")
TRACE_MSG(MIC_SAMPLE_DEMO_107_112_2_18_2_18_24_290,"mic_sample_demo, start in")
TRACE_MSG(MIC_SAMPLE_DEMO_111_112_2_18_2_18_24_291,"mic_sample_demo have started")
TRACE_MSG(MIC_SAMPLE_DEMO_146_112_2_18_2_18_24_292,"mic_sample_demo, start end")
TRACE_MSG(MIC_SAMPLE_DEMO_156_112_2_18_2_18_24_293,"mic_sample_demo, close in")
TRACE_MSG(MIC_SAMPLE_DEMO_160_112_2_18_2_18_24_294,"mic_sample_demo have clsoed")
TRACE_MSG(MIC_SAMPLE_DEMO_179_112_2_18_2_18_24_295,"mic_sample_demo, close end")
TRACE_MSG(MP3_STREAM_DEMO_92_112_2_18_2_18_24_296,"mp3_dec, callback")
TRACE_MSG(MP3_STREAM_DEMO_98_112_2_18_2_18_24_297,"mp3_dec, callback, not in palying status.")
TRACE_MSG(MP3_STREAM_DEMO_148_112_2_18_2_18_24_298,"mp3_dec, start")
TRACE_MSG(MP3_STREAM_DEMO_165_112_2_18_2_18_24_299,"mp3_dec, open")
TRACE_MSG(MP3_STREAM_DEMO_169_112_2_18_2_18_24_300,"mp3_dec, open, last operation first not stop")
TRACE_MSG(MP3_STREAM_DEMO_174_112_2_18_2_18_24_301,"mp3_dec, open MP3_RESOUCE_MONE %d")
TRACE_MSG(MP3_STREAM_DEMO_178_112_2_18_2_18_24_302,"mp3_dec, open MP3_RESOUCE_MONE %d")
TRACE_MSG(MP3_STREAM_DEMO_215_112_2_18_2_18_24_303,"mp3_dec, open(), create handle fail!")
TRACE_MSG(MP3_STREAM_DEMO_229_112_2_18_2_18_24_304,"mp3_dec, start")
TRACE_MSG(MP3_STREAM_DEMO_237_112_2_18_2_18_24_305,"mp3_dec, source_len= %d")
TRACE_MSG(MP3_STREAM_DEMO_248_112_2_18_2_18_24_306,"mp3_dec, close")
TRACE_MSG(MP3_STREAM_DEMO_270_112_2_18_2_18_25_307,"mp3_dec, pause")
TRACE_MSG(MP3_STREAM_DEMO_285_112_2_18_2_18_25_308,"mp3_dec, resume")
TRACE_MSG(WAV_STREAM_DEMO_85_112_2_18_2_18_25_309,"wav_dec, callback")
TRACE_MSG(WAV_STREAM_DEMO_92_112_2_18_2_18_25_310,"wav_dec, callback, not in palying status.")
TRACE_MSG(WAV_STREAM_DEMO_134_112_2_18_2_18_25_311,"wav_dec, i_len %d, offset=%d")
TRACE_MSG(WAV_STREAM_DEMO_155_112_2_18_2_18_25_312,"wav_dec, start")
TRACE_MSG(WAV_STREAM_DEMO_174_112_2_18_2_18_25_313,"wav_dec, open")
TRACE_MSG(WAV_STREAM_DEMO_178_112_2_18_2_18_25_314,"wav_dec, open, last operation first not stop")
TRACE_MSG(WAV_STREAM_DEMO_203_112_2_18_2_18_25_315,"wav_dec, open, uiBitPerSample[%d],uiBlockAlign[%d],uiChannelNum[%d],uiDataSize[%d],uiSampleRate[%d],eSubtype[%d]")
TRACE_MSG(WAV_STREAM_DEMO_219_112_2_18_2_18_25_316,"wav_dec, open(), create handle fail!")
TRACE_MSG(WAV_STREAM_DEMO_231_112_2_18_2_18_25_317,"wav_dec, start")
TRACE_MSG(WAV_STREAM_DEMO_242_112_2_18_2_18_25_318,"wav_dec,source_len= %d")
TRACE_MSG(WAV_STREAM_DEMO_253_112_2_18_2_18_25_319,"wav_dec, close")
TRACE_MSG(WAV_STREAM_DEMO_276_112_2_18_2_18_25_320,"wav_dec, pause")
TRACE_MSG(WAV_STREAM_DEMO_291_112_2_18_2_18_25_321,"wav_dec, resume")
TRACE_MSG(DC_VIDEO_DEMO_233_112_2_18_2_18_26_322,"[dc_video_demo]: GetData size=%d")
TRACE_MSG(DC_VIDEO_DEMO_252_112_2_18_2_18_26_323,"[dc_video_demo]: DCVIDEODEMO_PlayConstruct")
TRACE_MSG(DC_VIDEO_DEMO_272_112_2_18_2_18_26_324,"[dc_video_demo]: DCVIDEODEMO_PlayConstruct create  dynamic task!")
TRACE_MSG(DC_VIDEO_DEMO_276_112_2_18_2_18_26_325,"[dc_video_demo]: DCVIDEODEMO_PlayConstruct can not create  dynamic task!")
TRACE_MSG(DC_VIDEO_DEMO_289_112_2_18_2_18_26_326,"[dc_video_demo]:DCVIDEODEMO_StopDeconstruct CatMv_demo hdcvideo_demo_thread=%d")
TRACE_MSG(DC_VIDEO_DEMO_334_112_2_18_2_18_26_327,"[dc_video_demo]: DCVIDEODEMO_StopDeconstruct hdcvideo_demo_thread=%d")
TRACE_MSG(DC_VIDEO_DEMO_344_112_2_18_2_18_26_328,"[dc_video_demo]:DCVIDEODEMO_Play hdcvideo_demo_thread=%d")
TRACE_MSG(DC_VIDEO_DEMO_365_112_2_18_2_18_26_329,"[dc_video_demo]: DCVIDEODEMO_Stop, hdcvideo_demo_thread=%d")
TRACE_MSG(DC_VIDEO_DEMO_369_112_2_18_2_18_26_330,"[dc_video_demo]: DCVIDEODEMO_Stop, error")
TRACE_MSG(DC_VIDEO_DEMO_375_112_2_18_2_18_26_331,"[dc_video_demo]: DCVIDEODEMO_Stop, result=%d")
TRACE_MSG(DC_VIDEO_DEMO_406_112_2_18_2_18_26_332,"[dc_video_demo]:  DCVIDEODEMO_SendSignal-1- alloc failed!")
TRACE_MSG(DC_VIDEO_DEMO_422_112_2_18_2_18_26_333,"[dc_video_demo]: DCVIDEODEMO_SendSignal-2-  alloc failed!")
TRACE_MSG(DC_VIDEO_DEMO_461_112_2_18_2_18_26_334,"[dc_video_demo]: DCVIDEODEMO_Thread_Entry")
TRACE_MSG(DC_VIDEO_DEMO_475_112_2_18_2_18_26_335,"[dc_video_demo]: DCVIDEODEMO_Thread_Entry,DCVIDEODEMO_PLAY_REQ")
TRACE_MSG(DC_VIDEO_DEMO_482_112_2_18_2_18_26_336,"[dc_video_demo]: DCVIDEODEMO_Thread_Entry,DCVIDEODEMO_STOP_REQ")
TRACE_MSG(DC_VIDEO_DEMO_496_112_2_18_2_18_26_337,"[dc_video_demo]: DCVIDEODEMO_Thread_Entry,other invalid signal code!")
TRACE_MSG(DC_VIDEO_DEMO_512_112_2_18_2_18_26_338,"[dc_video_demo]: DCVIDEO_Demo_Start, s_dec_ctx.demo_state=%d")
TRACE_MSG(DC_VIDEO_DEMO_533_112_2_18_2_18_27_339,"[dc_video_demo]: DCVIDEO_Demo_Stop, s_dec_ctx.demo_state=%d")
TRACE_MSG(DC_VIDEO_DEMO_552_112_2_18_2_18_27_340,"[dc_video_demo]: DCVIDEO_Demo_Stop out")
TRACE_MSG(DISPLAY_YUV_DEMO_233_112_2_18_2_18_27_341,"demo: Video_Demo_yuv_Start")
TRACE_MSG(DISPLAY_YUV_DEMO_240_112_2_18_2_18_27_342,"demo: alloc err y_buf_addr[0x%x]")
TRACE_MSG(DISPLAY_YUV_DEMO_244_112_2_18_2_18_27_343,"demo: alloc ok y_buf_addr[0x%x]")
TRACE_MSG(DISPLAY_YUV_DEMO_250_112_2_18_2_18_27_344,"demo: alloc err u_buf_addr[0x%x]")
TRACE_MSG(DISPLAY_YUV_DEMO_254_112_2_18_2_18_27_345,"demo: alloc ok u_buf_addr[0x%x]")
TRACE_MSG(DISPLAY_YUV_DEMO_260_112_2_18_2_18_27_346,"demo: alloc err v_buf_addr[0x%x]")
TRACE_MSG(DISPLAY_YUV_DEMO_264_112_2_18_2_18_27_347,"demo: alloc ok v_buf_addr[0x%x]")
TRACE_MSG(DISPLAY_YUV_DEMO_302_112_2_18_2_18_28_348,"demo:update_display_param,DC_VideoSetDisplayParam fail")
TRACE_MSG(DISPLAY_YUV_DEMO_309_112_2_18_2_18_28_349,"demo:, DC_DisplayAFrame fail")
TRACE_MSG(DISPLAY_YUV_DEMO_314_112_2_18_2_18_28_350,"demo:cost time [%d]ms")
END_TRACE_MAP(MMI_APP_ENG_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_ENG_TRC_H_

