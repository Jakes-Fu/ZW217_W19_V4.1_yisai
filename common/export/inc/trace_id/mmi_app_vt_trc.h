/******************************************************************************
 ** File Name:      mmi_app_vt_trc.h                                         *
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
//trace_id:236
#ifndef _MMI_APP_VT_TRC_H_
#define _MMI_APP_VT_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIVT_KERNEL_324ADAPTER_111_112_2_18_3_6_2_0 "mmivt.c CallbackMsg() i_msg = %04x"
#define MMIVT_KERNEL_324ADAPTER_123_112_2_18_3_6_2_1 "mmivt.c: CallbackMsg() -> MPLH324M_MSG_CLOSE_IND argc = %d"
#define MMIVT_KERNEL_324ADAPTER_149_112_2_18_3_6_2_2 "mmivt.c CallbackMsg() i_msg = MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC"
#define MMIVT_KERNEL_324ADAPTER_191_112_2_18_3_6_3_3 "mmivt MMIVT_Open() b_mo[%d]"
#define MMIVT_KERNEL_324ADAPTER_199_112_2_18_3_6_3_4 "mmivt.c MMIVT_Open() fail error_code = %d"
#define MMIVT_KERNEL_324ADAPTER_214_112_2_18_3_6_3_5 "mmivt.c MMIVT_324_Close"
#define MMIVT_KERNEL_324ADAPTER_226_112_2_18_3_6_3_6 "mmivt.c MMIVT_MakeCall"
#define MMIVT_KERNEL_324ADAPTER_234_112_2_18_3_6_3_7 "videcall.c MMIVT_MakeCall() fail error_code = %d"
#define MMIVT_KERNEL_324ADAPTER_250_112_2_18_3_6_3_8 "mmivt.c MMIVT_324_ConnectReq"
#define MMIVT_KERNEL_324ADAPTER_257_112_2_18_3_6_3_9 "videcall.c MMIVT_ListenForCall() fail error_code = %d"
#define MMIVT_KERNEL_324ADAPTER_274_112_2_18_3_6_3_10 "mmivt.c MMIVT_324_HangUp"
#define MMIVT_KERNEL_324ADAPTER_281_112_2_18_3_6_3_11 "videcall.c MMIVT_HangUp() fail error_code = %d"
#define MMIVT_KERNEL_324ADAPTER_319_112_2_18_3_6_3_12 "mmivt send user input wrong %d"
#define MMIVT_KERNEL_324ADAPTER_338_112_2_18_3_6_3_13 "mmivt MMIVT_324_EnableCamera fail code = %d"
#define MMIVT_KERNEL_PROCESS_348_112_2_18_3_6_4_14 "mmivt null param"
#define MMIVT_KERNEL_PROCESS_376_112_2_18_3_6_4_15 "mmivt:enter MMIVT_HandleExtMsg(), msg_id = %d"
#define MMIVT_KERNEL_PROCESS_540_112_2_18_3_6_4_16 "mmivt: ext msg: %d"
#define MMIVT_KERNEL_PROCESS_559_112_2_18_3_6_4_17 "mmivt cur_state:%d, 324 state:%d, ps state:%d"
#define MMIVT_KERNEL_PROCESS_607_112_2_18_3_6_4_18 "mmivt wrong msg: %d"
#define MMIVT_KERNEL_PROCESS_626_112_2_18_3_6_4_19 "mmivt HandleUserDial"
#define MMIVT_KERNEL_PROCESS_632_112_2_18_3_6_4_20 "mmivt err state: %d"
#define MMIVT_KERNEL_PROCESS_695_112_2_18_3_6_4_21 "mmivt HandleSetupComplete"
#define MMIVT_KERNEL_PROCESS_702_112_2_18_3_6_4_22 "mmivt err state:%d type:%d"
#define MMIVT_KERNEL_PROCESS_730_112_2_18_3_6_4_23 "mmivt HandleStartMedia"
#define MMIVT_KERNEL_PROCESS_735_112_2_18_3_6_4_24 "mmivt HandleStartMedia no exe"
#define MMIVT_KERNEL_PROCESS_743_112_2_18_3_6_5_25 "mmivt err state:%d"
#define MMIVT_KERNEL_PROCESS_775_112_2_18_3_6_5_26 "mmivt err type:%"
#define MMIVT_KERNEL_PROCESS_796_112_2_18_3_6_5_27 "mmivt HandleStartMedia err result: %d"
#define MMIVT_KERNEL_PROCESS_812_112_2_18_3_6_5_28 "mmivt HandleUserHangup"
#define MMIVT_KERNEL_PROCESS_858_112_2_18_3_6_5_29 "mmivt err ps state:%d, 324 state:%d"
#define MMIVT_KERNEL_PROCESS_885_112_2_18_3_6_5_30 "mmivt HandleUserHangup err state: %d"
#define MMIVT_KERNEL_PROCESS_901_112_2_18_3_6_5_31 "mmivt Handle324ClosedInd"
#define MMIVT_KERNEL_PROCESS_906_112_2_18_3_6_5_32 "mmivt err 324 state"
#define MMIVT_KERNEL_PROCESS_945_112_2_18_3_6_5_33 "mmivt Handle324ClosedInd err state: %d"
#define MMIVT_KERNEL_PROCESS_960_112_2_18_3_6_5_34 "mmivt Handle324CloseCnf"
#define MMIVT_KERNEL_PROCESS_965_112_2_18_3_6_5_35 "mmivt err state: %d"
#define MMIVT_KERNEL_PROCESS_977_112_2_18_3_6_5_36 "mmivt err 324 state: %d"
#define MMIVT_KERNEL_PROCESS_1027_112_2_18_3_6_5_37 "mmivt err ps state:%d"
#define MMIVT_KERNEL_PROCESS_1056_112_2_18_3_6_5_38 "mmivt HandleDisconnectedInd cause %d"
#define MMIVT_KERNEL_PROCESS_1067_112_2_18_3_6_5_39 "mmivt err ps state:%d"
#define MMIVT_KERNEL_PROCESS_1129_112_2_18_3_6_5_40 "mmivt err 324 state:%d"
#define MMIVT_KERNEL_PROCESS_1184_112_2_18_3_6_6_41 "mmivt err state:%d"
#define MMIVT_KERNEL_PROCESS_1203_112_2_18_3_6_6_42 "mmivt HandleIncomingCall"
#define MMIVT_KERNEL_PROCESS_1207_112_2_18_3_6_6_43 "mmivt param null"
#define MMIVT_KERNEL_PROCESS_1217_112_2_18_3_6_6_44 "mmivt disc incoming vt res:%d"
#define MMIVT_KERNEL_PROCESS_1230_112_2_18_3_6_6_45 "mmivt err state"
#define MMIVT_KERNEL_PROCESS_1272_112_2_18_3_6_6_46 "mmivt HandleAlertingInd"
#define MMIVT_KERNEL_PROCESS_1278_112_2_18_3_6_6_47 "mmivt err state:%d, type:%d"
#define MMIVT_KERNEL_PROCESS_1315_112_2_18_3_6_6_48 "mmivt HandleConnectCall"
#define MMIVT_KERNEL_PROCESS_1322_112_2_18_3_6_6_49 "mmivt err state:%d, type:%d"
#define MMIVT_KERNEL_PROCESS_1355_112_2_18_3_6_6_50 "mmivt HandleConnectInd"
#define MMIVT_KERNEL_PROCESS_1362_112_2_18_3_6_6_51 "mmivt err state:%d, type:%d"
#define MMIVT_KERNEL_PROCESS_1403_112_2_18_3_6_6_52 "mmivt MakeVtCall"
#define MMIVT_KERNEL_PROCESS_1437_112_2_18_3_6_6_53 "mmivt genPartyNum error"
#define MMIVT_KERNEL_PROCESS_1486_112_2_18_3_6_6_54 "SetAddressInfoByBCDNum bcd_num is NULL"
#define MMIVT_KERNEL_PROCESS_1600_112_2_18_3_6_6_55 "MMIAPIVT_GetCallName out_name_str_ptr is NULL"
#define MMIVT_KERNEL_PROCESS_1654_112_2_18_3_6_6_56 "MMIVT_GetCallBCDNumber bcd_number_ptr is NULL"
#define MMIVT_KERNEL_PROCESS_1677_112_2_18_3_6_7_57 "MMIVT_GetCallNormalNumber bcd_number_ptr is NULL"
#define MMIVT_KERNEL_PROCESS_1776_112_2_18_3_6_7_58 "mmivt is fall back req:%d"
#define MMIVT_KERNEL_PROCESS_1815_112_2_18_3_6_7_59 "SaveToMostUsedList::is_valid_number = %d"
#define MMIVT_KERNEL_PROCESS_1900_112_2_18_3_6_7_60 "mmivt: userinput is %s %x"
#define MMIVT_KERNEL_PROCESS_1904_112_2_18_3_6_7_61 "mmivt HandleUserInput err state:%d"
#define MMIVT_KERNEL_PROCESS_1910_112_2_18_3_6_7_62 "mmivt quit mmr"
#define MMIVT_KERNEL_PROCESS_1921_112_2_18_3_6_7_63 "mmivt enter mmr"
#define MMIVT_KERNEL_REFADAPTER_114_112_2_18_3_6_7_64 "mmivt ErrorNotifyCallback %d"
#define MMIVT_KERNEL_REFADAPTER_133_112_2_18_3_6_7_65 "mmivt: ref has opened"
#define MMIVT_KERNEL_REFADAPTER_152_112_2_18_3_6_8_66 "mmivt: only one sensor"
#define MMIVT_KERNEL_REFADAPTER_175_112_2_18_3_6_8_67 "mmivt: ref not open"
#define MMIVT_KERNEL_REFADAPTER_196_112_2_18_3_6_8_68 "MMIVT_REF_StartPreview already preview "
#define MMIVT_KERNEL_REFADAPTER_224_112_2_18_3_6_8_69 "MMIVT_REF_StopPreview not previewing "
#define MMIVT_KERNEL_REFADAPTER_245_112_2_18_3_6_8_70 "MMIVT_REF_StartTalk already talk"
#define MMIVT_KERNEL_REFADAPTER_271_112_2_18_3_6_8_71 "MMIVT_REF_StopTalk not talking "
#define MMIVT_KERNEL_REFADAPTER_406_112_2_18_3_6_8_72 "MMIVT_REF_ReplaceStart err type:%d"
#define MMIVT_KERNEL_REFADAPTER_410_112_2_18_3_6_8_73 "MMIVT_REF_ReplaceStart replace_type:%d, is_default:%d"
#define MMIVT_KERNEL_REFADAPTER_504_112_2_18_3_6_8_74 "MMIVT_REF_ReplaceStop replace_type:%d"
#define MMIVT_KERNEL_REFADAPTER_507_112_2_18_3_6_8_75 "MMIVT_REF_ReplaceStop err"
#define MMIVT_KERNEL_REFADAPTER_540_112_2_18_3_6_8_76 "MMIVT_REF_RecordStart record_type:%d"
#define MMIVT_KERNEL_REFADAPTER_543_112_2_18_3_6_8_77 "MMIVT_REF_RecordStart err type:%d"
#define MMIVT_KERNEL_REFADAPTER_590_112_2_18_3_6_8_78 "mmivt record start result:%d"
#define MMIVT_KERNEL_REFADAPTER_621_112_2_18_3_6_8_79 "mmivt ref bright max:%d"
#define MMIVT_KERNEL_REFADAPTER_639_112_2_18_3_6_8_80 "mmivt ref contrast max:%d"
#define MMIVT_KERNEL_REFADAPTER_659_112_2_18_3_6_9_81 "MMIVT_REF_SetLcdBrightness:%d"
#define MMIVT_KERNEL_REFADAPTER_673_112_2_18_3_6_9_82 "MMIVT_REF_SetContrast:%d"
#define MMIVT_KERNEL_REFADAPTER_690_112_2_18_3_6_9_83 "MMIVT_REF_SetBrightness:%d"
#define MMIVT_KERNEL_REFADAPTER_766_112_2_18_3_6_9_84 "MMIVT_REF_SwitchCamera:%d"
#define MMIVT_MAIN_150_112_2_18_3_6_9_85 "mmi vtApplet_HandleEvent msg_id = 0x%04x"
#define MMIVT_MAIN_334_112_2_18_3_6_10_86 "mmivt : is vt call being"
#define MMIVT_MAIN_357_112_2_18_3_6_10_87 "mmivt : is vt call being"
#define MMIVT_MAIN_366_112_2_18_3_6_10_88 "mmivt: null applet!"
#define MMIVT_MAIN_380_112_2_18_3_6_10_89 "MMIVT_Initialize"
#define MMIVT_MAIN_428_112_2_18_3_6_10_90 "MMIVT_Finalize"
#define MMIVT_MAIN_457_112_2_18_3_6_10_91 "mmivt StartVTApplet"
#define MMIVT_MAIN_473_112_2_18_3_6_10_92 "mmivt StopVT"
#define MMIVT_MAIN_493_112_2_18_3_6_10_93 "MMIAPIVT_SendMsgToApplet msg:%d"
#define MMIVT_MAIN_521_112_2_18_3_6_10_94 "MMIAPIVT_ActiveVT result:%d"
#define MMIVT_UI_1267_112_2_18_3_6_13_95 "mmivt HandleUIAction state:%d"
#define MMIVT_UI_1382_112_2_18_3_6_14_96 "mmivt HandleUIDial"
#define MMIVT_UI_1399_112_2_18_3_6_14_97 "mmivt err already have applet"
#define MMIVT_UI_1441_112_2_18_3_6_14_98 "mmivt HandleUIRemoteConnect"
#define MMIVT_UI_1444_112_2_18_3_6_14_99 "mmivt null applet"
#define MMIVT_UI_1450_112_2_18_3_6_14_100 "mmivt err state:%d"
#define MMIVT_UI_1470_112_2_18_3_6_14_101 "mmivt HandleUIRemoteIncoming"
#define MMIVT_UI_1473_112_2_18_3_6_14_102 "mmivt err already have applet"
#define MMIVT_UI_1497_112_2_18_3_6_14_103 "mmivt HandleUIConnect"
#define MMIVT_UI_1500_112_2_18_3_6_14_104 "mmivt null applet"
#define MMIVT_UI_1506_112_2_18_3_6_14_105 "mmivt err state:%d"
#define MMIVT_UI_1530_112_2_18_3_6_14_106 "mmivt HandleUIConnectEnd"
#define MMIVT_UI_1533_112_2_18_3_6_14_107 "mmivt null applet"
#define MMIVT_UI_1539_112_2_18_3_6_14_108 "mmivt err state:%d"
#define MMIVT_UI_1565_112_2_18_3_6_14_109 "mmivt null applet"
#define MMIVT_UI_1568_112_2_18_3_6_14_110 "mmivt HandleUIHangup"
#define MMIVT_UI_1596_112_2_18_3_6_14_111 "mmivt HandleUIHangup err state:%d"
#define MMIVT_UI_1627_112_2_18_3_6_14_112 "mmivt HandleUIDisconnect"
#define MMIVT_UI_1630_112_2_18_3_6_14_113 "mmivt null applet"
#define MMIVT_UI_1662_112_2_18_3_6_14_114 "mmivt: HandleUIDisconnect err state:%d"
#define MMIVT_UI_1679_112_2_18_3_6_14_115 "mmivt HandleUIDiscEnd"
#define MMIVT_UI_1682_112_2_18_3_6_14_116 "mmivt null applet"
#define MMIVT_UI_1702_112_2_18_3_6_14_117 "mmivt close MMIVT_DISCONNECTING_WIN_ID"
#define MMIVT_UI_1704_112_2_18_3_6_14_118 "mmivt close end"
#define MMIVT_UI_1769_112_2_18_3_6_15_119 "mmivt err state:%d"
#define MMIVT_UI_1787_112_2_18_3_6_15_120 "mmivt HandleUIRemoteFBReq"
#define MMIVT_UI_1790_112_2_18_3_6_15_121 "mmivt null applet"
#define MMIVT_UI_1826_112_2_18_3_6_15_122 "mmivt err fallback mode:%d"
#define MMIVT_UI_1837_112_2_18_3_6_15_123 "mmivt err state:%d"
#define MMIVT_UI_1854_112_2_18_3_6_15_124 "mmivt HandleUIFBReq"
#define MMIVT_UI_1857_112_2_18_3_6_15_125 "mmivt null applet"
#define MMIVT_UI_1878_112_2_18_3_6_15_126 "mmivt err state:%d"
#define MMIVT_UI_1894_112_2_18_3_6_15_127 "mmivt HandleUIQuit"
#define MMIVT_UI_1897_112_2_18_3_6_15_128 "mmivt null applet"
#define MMIVT_UI_1916_112_2_18_3_6_15_129 "mmivt HandleUIRemoteAlert"
#define MMIVT_UI_1919_112_2_18_3_6_15_130 "mmivt null applet"
#define MMIVT_UI_1925_112_2_18_3_6_15_131 "mmivt err state:%d"
#define MMIVT_UI_1950_112_2_18_3_6_15_132 "mmivt HandleUIEnterMMR"
#define MMIVT_UI_1953_112_2_18_3_6_15_133 "mmivt null applet"
#define MMIVT_UI_1959_112_2_18_3_6_15_134 "mmivt err state:%d"
#define MMIVT_UI_1991_112_2_18_3_6_15_135 "mmivt HandleUIEnterMMR"
#define MMIVT_UI_1994_112_2_18_3_6_15_136 "mmivt null applet"
#define MMIVT_UI_2000_112_2_18_3_6_15_137 "mmivt err state:%d sub state:%d"
#define MMIVT_UI_2055_112_2_18_3_6_15_138 "mmivt HandleUIRedial"
#define MMIVT_UI_2059_112_2_18_3_6_15_139 "mmivt null applet"
#define MMIVT_UI_2104_112_2_18_3_6_15_140 "mmivt HandleUIRefErrorInd %d"
#define MMIVT_UI_2118_112_2_18_3_6_15_141 "mmivt HandleUIRemoteCameraOff"
#define MMIVT_UI_2122_112_2_18_3_6_15_142 "mmivt null applet"
#define MMIVT_UI_2128_112_2_18_3_6_15_143 "mmivt err state"
#define MMIVT_UI_2155_112_2_18_3_6_15_144 "mmivt HandleUIRemoteCameraOn"
#define MMIVT_UI_2159_112_2_18_3_6_15_145 "mmivt null applet"
#define MMIVT_UI_2165_112_2_18_3_6_15_146 "mmivt err state"
#define MMIVT_UI_2190_112_2_18_3_6_15_147 "mmivt HandleUIUpdateAudioDevice"
#define MMIVT_UI_2194_112_2_18_3_6_15_148 "mmivt null applet"
#define MMIVT_UI_2223_112_2_18_3_6_15_149 "mmivt null applet"
#define MMIVT_UI_2271_112_2_18_3_6_16_150 "mmivt null applet"
#define MMIVT_UI_2294_112_2_18_3_6_16_151 "mmivt null applet"
#define MMIVT_UI_2317_112_2_18_3_6_16_152 "mmivt null applet"
#define MMIVT_UI_2340_112_2_18_3_6_16_153 "mmivt null applet"
#define MMIVT_UI_2364_112_2_18_3_6_16_154 "mmivt null applet"
#define MMIVT_UI_2390_112_2_18_3_6_16_155 "mmivt null applet"
#define MMIVT_UI_2413_112_2_18_3_6_16_156 "mmivt null applet"
#define MMIVT_UI_2438_112_2_18_3_6_16_157 "mmivt null applet"
#define MMIVT_UI_2462_112_2_18_3_6_16_158 "mmivt null applet"
#define MMIVT_UI_2500_112_2_18_3_6_16_159 "mmivt null applet"
#define MMIVT_UI_2533_112_2_18_3_6_16_160 "mmivt null applet"
#define MMIVT_UI_2566_112_2_18_3_6_16_161 "mmivt null applet"
#define MMIVT_UI_2590_112_2_18_3_6_16_162 "mmivt null applet"
#define MMIVT_UI_2898_112_2_18_3_6_17_163 "mmivt auto accept mt"
#define MMIVT_UI_3422_112_2_18_3_6_18_164 "mmivt HandleVTConnectedWinUpAndDownKey mode: %d"
#define MMIVT_UI_3460_112_2_18_3_6_18_165 "mmivt HandleVTConnectedWinLeftAndRightKey mode: %d"
#define MMIVT_UI_3515_112_2_18_3_6_18_166 "mmivt wrong focus %d"
#define MMIVT_UI_3544_112_2_18_3_6_18_167 "mmivt HandleVTConnectedWinPhoneNumKey mode: %d"
#define MMIVT_UI_3584_112_2_18_3_6_18_168 "mmivt HandleVTConnectedWinWebKey mode: %d"
#define MMIVT_UI_3638_112_2_18_3_6_18_169 "mmivt wrong focus %d"
#define MMIVT_UI_3683_112_2_18_3_6_18_170 "mmivt HandleVTConnectedWinCancelKey mode: %d"
#define MMIVT_UI_3824_112_2_18_3_6_19_171 "mmivt wrong focus %d"
#define MMIVT_UI_3880_112_2_18_3_6_19_172 "mmivt HandleVTConnectedWinPenOk err button_id:%d"
#define MMIVT_UI_5333_112_2_18_3_6_22_173 "SetHeadAndContentInRText index is NULL"
#define MMIVT_UI_5444_112_2_18_3_6_22_174 "GetBigHeadPhotoByCalledNum call_number_ptr=%d, photo_name_ptr =%d, photo_len_ptr =%d"
#define MMIVT_UI_5470_112_2_18_3_6_22_175 "mmivt SwitchVideoRotateFocus"
#define MMIVT_UI_5498_112_2_18_3_6_22_176 "mmivt SwitchVideoOptionFocus focus:%d"
#define MMIVT_UI_5537_112_2_18_3_6_22_177 "mmivt SwitchMainOptionFocus focus:%d"
#define MMIVT_UI_5576_112_2_18_3_6_22_178 "mmivt UpdateLcdBright %d"
#define MMIVT_UI_5621_112_2_18_3_6_22_179 "mmivt UpdateZoom level:%d zoom_out:%d"
#define MMIVT_UI_5663_112_2_18_3_6_22_180 "mmivt UpdateContrast level:%d contrast_add:%d"
#define MMIVT_UI_5707_112_2_18_3_6_23_181 "mmivt UpdateBrightness level:%d brightness_add:%d"
#define MMIVT_UI_5757_112_2_18_3_6_23_182 "mmivt UpdateRotateOption mode:%d, status:%d"
#define MMIVT_UI_5806_112_2_18_3_6_23_183 "mmivt UpdateAudioOption handfree_show:%d"
#define MMIVT_UI_5839_112_2_18_3_6_23_184 "mmivt UpdateCameraOption id:%d"
#define MMIVT_UI_5843_112_2_18_3_6_23_185 "mmivt UpdateCameraOption replacing not switch camera"
#define MMIVT_UI_5877_112_2_18_3_6_23_186 "mmivt UpdateDisplayModeOption mode:%d"
#define MMIVT_UI_5915_112_2_18_3_6_23_187 "mmivt EnterMainOptionMode"
#define MMIVT_UI_5930_112_2_18_3_6_23_188 "mmivt QuitMainOptionMode"
#define MMIVT_UI_5992_112_2_18_3_6_23_189 "mmivt:wrong focus %d"
#define MMIVT_UI_6045_112_2_18_3_6_23_190 "mmivt EnterVideoOptionMode"
#define MMIVT_UI_6060_112_2_18_3_6_23_191 "mmivt QuitVideoOptionMode"
#define MMIVT_UI_6082_112_2_18_3_6_23_192 "mmivt DisplayVideoOption focus %d"
#define MMIVT_UI_6106_112_2_18_3_6_24_193 "mmivt DisplayVideoOption wrong sub focus"
#define MMIVT_UI_6139_112_2_18_3_6_24_194 "mmivt EnterVideoParamMode"
#define MMIVT_UI_6167_112_2_18_3_6_24_195 "mmivt EnterVideoParamMode error ctrl"
#define MMIVT_UI_6186_112_2_18_3_6_24_196 "mmivt QuitVideoParamMode"
#define MMIVT_UI_6202_112_2_18_3_6_24_197 "mmivt DisplayVideoParam video option:%d"
#define MMIVT_UI_6222_112_2_18_3_6_24_198 "mmivt DisplayVideoParam err rotate mode:%d"
#define MMIVT_UI_6256_112_2_18_3_6_24_199 "mmivt wrong video option %d"
#define MMIVT_UI_6316_112_2_18_3_6_24_200 "mmivt EnterUserInputMode"
#define MMIVT_UI_6402_112_2_18_3_6_24_201 "mmivt QuitUserInputMode"
#define MMIVT_UI_6453_112_2_18_3_6_24_202 "mmivt EnterVolumeMode"
#define MMIVT_UI_6470_112_2_18_3_6_24_203 "mmivt QuitVolumeMode"
#define MMIVT_UI_6591_112_2_18_3_6_25_204 "mmivt.c HandleUser user_input_char: %c"
#define MMIVT_UI_6601_112_2_18_3_6_25_205 "mmivt send userinput error state:%d"
#define MMIVT_UI_6666_112_2_18_3_6_25_206 "mmivt null applet"
#define MMIVT_UI_6766_112_2_18_3_6_25_207 "mmivt HandleEnableCamera %d"
#define MMIVT_UI_6879_112_2_18_3_6_25_208 "MMIVT StartRecord record_type:%d"
#define MMIVT_UI_6882_112_2_18_3_6_25_209 "MMIVT StartRecord err type:%d"
#define MMIVT_UI_6889_112_2_18_3_6_25_210 "MMIVT StartRecord bt in use"
#define MMIVT_UI_6919_112_2_18_3_6_25_211 "mmivt no enough space to record"
#define MMIVT_UI_6976_112_2_18_3_6_25_212 "mmivt start record create file failed"
#define MMIVT_UI_7023_112_2_18_3_6_25_213 "MMIVT StopRecord"
#define MMIVT_UI_7026_112_2_18_3_6_25_214 "mmivt null applet"
#define MMIVT_UI_7031_112_2_18_3_6_25_215 "MMIVT not recording"
#define MMIVT_UI_7055_112_2_18_3_6_26_216 "mmivt errorNotify %d"
#define MMIVT_UI_7151_112_2_18_3_6_26_217 "mmivt DisableGsensorRotate set = %d, current_angle = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_VT_TRC)
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_111_112_2_18_3_6_2_0,"mmivt.c CallbackMsg() i_msg = %04x")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_123_112_2_18_3_6_2_1,"mmivt.c: CallbackMsg() -> MPLH324M_MSG_CLOSE_IND argc = %d")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_149_112_2_18_3_6_2_2,"mmivt.c CallbackMsg() i_msg = MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_191_112_2_18_3_6_3_3,"mmivt MMIVT_Open() b_mo[%d]")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_199_112_2_18_3_6_3_4,"mmivt.c MMIVT_Open() fail error_code = %d")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_214_112_2_18_3_6_3_5,"mmivt.c MMIVT_324_Close")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_226_112_2_18_3_6_3_6,"mmivt.c MMIVT_MakeCall")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_234_112_2_18_3_6_3_7,"videcall.c MMIVT_MakeCall() fail error_code = %d")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_250_112_2_18_3_6_3_8,"mmivt.c MMIVT_324_ConnectReq")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_257_112_2_18_3_6_3_9,"videcall.c MMIVT_ListenForCall() fail error_code = %d")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_274_112_2_18_3_6_3_10,"mmivt.c MMIVT_324_HangUp")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_281_112_2_18_3_6_3_11,"videcall.c MMIVT_HangUp() fail error_code = %d")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_319_112_2_18_3_6_3_12,"mmivt send user input wrong %d")
TRACE_MSG(MMIVT_KERNEL_324ADAPTER_338_112_2_18_3_6_3_13,"mmivt MMIVT_324_EnableCamera fail code = %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_348_112_2_18_3_6_4_14,"mmivt null param")
TRACE_MSG(MMIVT_KERNEL_PROCESS_376_112_2_18_3_6_4_15,"mmivt:enter MMIVT_HandleExtMsg(), msg_id = %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_540_112_2_18_3_6_4_16,"mmivt: ext msg: %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_559_112_2_18_3_6_4_17,"mmivt cur_state:%d, 324 state:%d, ps state:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_607_112_2_18_3_6_4_18,"mmivt wrong msg: %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_626_112_2_18_3_6_4_19,"mmivt HandleUserDial")
TRACE_MSG(MMIVT_KERNEL_PROCESS_632_112_2_18_3_6_4_20,"mmivt err state: %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_695_112_2_18_3_6_4_21,"mmivt HandleSetupComplete")
TRACE_MSG(MMIVT_KERNEL_PROCESS_702_112_2_18_3_6_4_22,"mmivt err state:%d type:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_730_112_2_18_3_6_4_23,"mmivt HandleStartMedia")
TRACE_MSG(MMIVT_KERNEL_PROCESS_735_112_2_18_3_6_4_24,"mmivt HandleStartMedia no exe")
TRACE_MSG(MMIVT_KERNEL_PROCESS_743_112_2_18_3_6_5_25,"mmivt err state:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_775_112_2_18_3_6_5_26,"mmivt err type:%")
TRACE_MSG(MMIVT_KERNEL_PROCESS_796_112_2_18_3_6_5_27,"mmivt HandleStartMedia err result: %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_812_112_2_18_3_6_5_28,"mmivt HandleUserHangup")
TRACE_MSG(MMIVT_KERNEL_PROCESS_858_112_2_18_3_6_5_29,"mmivt err ps state:%d, 324 state:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_885_112_2_18_3_6_5_30,"mmivt HandleUserHangup err state: %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_901_112_2_18_3_6_5_31,"mmivt Handle324ClosedInd")
TRACE_MSG(MMIVT_KERNEL_PROCESS_906_112_2_18_3_6_5_32,"mmivt err 324 state")
TRACE_MSG(MMIVT_KERNEL_PROCESS_945_112_2_18_3_6_5_33,"mmivt Handle324ClosedInd err state: %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_960_112_2_18_3_6_5_34,"mmivt Handle324CloseCnf")
TRACE_MSG(MMIVT_KERNEL_PROCESS_965_112_2_18_3_6_5_35,"mmivt err state: %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_977_112_2_18_3_6_5_36,"mmivt err 324 state: %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1027_112_2_18_3_6_5_37,"mmivt err ps state:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1056_112_2_18_3_6_5_38,"mmivt HandleDisconnectedInd cause %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1067_112_2_18_3_6_5_39,"mmivt err ps state:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1129_112_2_18_3_6_5_40,"mmivt err 324 state:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1184_112_2_18_3_6_6_41,"mmivt err state:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1203_112_2_18_3_6_6_42,"mmivt HandleIncomingCall")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1207_112_2_18_3_6_6_43,"mmivt param null")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1217_112_2_18_3_6_6_44,"mmivt disc incoming vt res:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1230_112_2_18_3_6_6_45,"mmivt err state")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1272_112_2_18_3_6_6_46,"mmivt HandleAlertingInd")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1278_112_2_18_3_6_6_47,"mmivt err state:%d, type:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1315_112_2_18_3_6_6_48,"mmivt HandleConnectCall")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1322_112_2_18_3_6_6_49,"mmivt err state:%d, type:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1355_112_2_18_3_6_6_50,"mmivt HandleConnectInd")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1362_112_2_18_3_6_6_51,"mmivt err state:%d, type:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1403_112_2_18_3_6_6_52,"mmivt MakeVtCall")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1437_112_2_18_3_6_6_53,"mmivt genPartyNum error")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1486_112_2_18_3_6_6_54,"SetAddressInfoByBCDNum bcd_num is NULL")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1600_112_2_18_3_6_6_55,"MMIAPIVT_GetCallName out_name_str_ptr is NULL")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1654_112_2_18_3_6_6_56,"MMIVT_GetCallBCDNumber bcd_number_ptr is NULL")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1677_112_2_18_3_6_7_57,"MMIVT_GetCallNormalNumber bcd_number_ptr is NULL")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1776_112_2_18_3_6_7_58,"mmivt is fall back req:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1815_112_2_18_3_6_7_59,"SaveToMostUsedList::is_valid_number = %d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1900_112_2_18_3_6_7_60,"mmivt: userinput is %s %x")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1904_112_2_18_3_6_7_61,"mmivt HandleUserInput err state:%d")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1910_112_2_18_3_6_7_62,"mmivt quit mmr")
TRACE_MSG(MMIVT_KERNEL_PROCESS_1921_112_2_18_3_6_7_63,"mmivt enter mmr")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_114_112_2_18_3_6_7_64,"mmivt ErrorNotifyCallback %d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_133_112_2_18_3_6_7_65,"mmivt: ref has opened")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_152_112_2_18_3_6_8_66,"mmivt: only one sensor")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_175_112_2_18_3_6_8_67,"mmivt: ref not open")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_196_112_2_18_3_6_8_68,"MMIVT_REF_StartPreview already preview ")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_224_112_2_18_3_6_8_69,"MMIVT_REF_StopPreview not previewing ")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_245_112_2_18_3_6_8_70,"MMIVT_REF_StartTalk already talk")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_271_112_2_18_3_6_8_71,"MMIVT_REF_StopTalk not talking ")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_406_112_2_18_3_6_8_72,"MMIVT_REF_ReplaceStart err type:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_410_112_2_18_3_6_8_73,"MMIVT_REF_ReplaceStart replace_type:%d, is_default:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_504_112_2_18_3_6_8_74,"MMIVT_REF_ReplaceStop replace_type:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_507_112_2_18_3_6_8_75,"MMIVT_REF_ReplaceStop err")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_540_112_2_18_3_6_8_76,"MMIVT_REF_RecordStart record_type:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_543_112_2_18_3_6_8_77,"MMIVT_REF_RecordStart err type:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_590_112_2_18_3_6_8_78,"mmivt record start result:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_621_112_2_18_3_6_8_79,"mmivt ref bright max:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_639_112_2_18_3_6_8_80,"mmivt ref contrast max:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_659_112_2_18_3_6_9_81,"MMIVT_REF_SetLcdBrightness:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_673_112_2_18_3_6_9_82,"MMIVT_REF_SetContrast:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_690_112_2_18_3_6_9_83,"MMIVT_REF_SetBrightness:%d")
TRACE_MSG(MMIVT_KERNEL_REFADAPTER_766_112_2_18_3_6_9_84,"MMIVT_REF_SwitchCamera:%d")
TRACE_MSG(MMIVT_MAIN_150_112_2_18_3_6_9_85,"mmi vtApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIVT_MAIN_334_112_2_18_3_6_10_86,"mmivt : is vt call being")
TRACE_MSG(MMIVT_MAIN_357_112_2_18_3_6_10_87,"mmivt : is vt call being")
TRACE_MSG(MMIVT_MAIN_366_112_2_18_3_6_10_88,"mmivt: null applet!")
TRACE_MSG(MMIVT_MAIN_380_112_2_18_3_6_10_89,"MMIVT_Initialize")
TRACE_MSG(MMIVT_MAIN_428_112_2_18_3_6_10_90,"MMIVT_Finalize")
TRACE_MSG(MMIVT_MAIN_457_112_2_18_3_6_10_91,"mmivt StartVTApplet")
TRACE_MSG(MMIVT_MAIN_473_112_2_18_3_6_10_92,"mmivt StopVT")
TRACE_MSG(MMIVT_MAIN_493_112_2_18_3_6_10_93,"MMIAPIVT_SendMsgToApplet msg:%d")
TRACE_MSG(MMIVT_MAIN_521_112_2_18_3_6_10_94,"MMIAPIVT_ActiveVT result:%d")
TRACE_MSG(MMIVT_UI_1267_112_2_18_3_6_13_95,"mmivt HandleUIAction state:%d")
TRACE_MSG(MMIVT_UI_1382_112_2_18_3_6_14_96,"mmivt HandleUIDial")
TRACE_MSG(MMIVT_UI_1399_112_2_18_3_6_14_97,"mmivt err already have applet")
TRACE_MSG(MMIVT_UI_1441_112_2_18_3_6_14_98,"mmivt HandleUIRemoteConnect")
TRACE_MSG(MMIVT_UI_1444_112_2_18_3_6_14_99,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1450_112_2_18_3_6_14_100,"mmivt err state:%d")
TRACE_MSG(MMIVT_UI_1470_112_2_18_3_6_14_101,"mmivt HandleUIRemoteIncoming")
TRACE_MSG(MMIVT_UI_1473_112_2_18_3_6_14_102,"mmivt err already have applet")
TRACE_MSG(MMIVT_UI_1497_112_2_18_3_6_14_103,"mmivt HandleUIConnect")
TRACE_MSG(MMIVT_UI_1500_112_2_18_3_6_14_104,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1506_112_2_18_3_6_14_105,"mmivt err state:%d")
TRACE_MSG(MMIVT_UI_1530_112_2_18_3_6_14_106,"mmivt HandleUIConnectEnd")
TRACE_MSG(MMIVT_UI_1533_112_2_18_3_6_14_107,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1539_112_2_18_3_6_14_108,"mmivt err state:%d")
TRACE_MSG(MMIVT_UI_1565_112_2_18_3_6_14_109,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1568_112_2_18_3_6_14_110,"mmivt HandleUIHangup")
TRACE_MSG(MMIVT_UI_1596_112_2_18_3_6_14_111,"mmivt HandleUIHangup err state:%d")
TRACE_MSG(MMIVT_UI_1627_112_2_18_3_6_14_112,"mmivt HandleUIDisconnect")
TRACE_MSG(MMIVT_UI_1630_112_2_18_3_6_14_113,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1662_112_2_18_3_6_14_114,"mmivt: HandleUIDisconnect err state:%d")
TRACE_MSG(MMIVT_UI_1679_112_2_18_3_6_14_115,"mmivt HandleUIDiscEnd")
TRACE_MSG(MMIVT_UI_1682_112_2_18_3_6_14_116,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1702_112_2_18_3_6_14_117,"mmivt close MMIVT_DISCONNECTING_WIN_ID")
TRACE_MSG(MMIVT_UI_1704_112_2_18_3_6_14_118,"mmivt close end")
TRACE_MSG(MMIVT_UI_1769_112_2_18_3_6_15_119,"mmivt err state:%d")
TRACE_MSG(MMIVT_UI_1787_112_2_18_3_6_15_120,"mmivt HandleUIRemoteFBReq")
TRACE_MSG(MMIVT_UI_1790_112_2_18_3_6_15_121,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1826_112_2_18_3_6_15_122,"mmivt err fallback mode:%d")
TRACE_MSG(MMIVT_UI_1837_112_2_18_3_6_15_123,"mmivt err state:%d")
TRACE_MSG(MMIVT_UI_1854_112_2_18_3_6_15_124,"mmivt HandleUIFBReq")
TRACE_MSG(MMIVT_UI_1857_112_2_18_3_6_15_125,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1878_112_2_18_3_6_15_126,"mmivt err state:%d")
TRACE_MSG(MMIVT_UI_1894_112_2_18_3_6_15_127,"mmivt HandleUIQuit")
TRACE_MSG(MMIVT_UI_1897_112_2_18_3_6_15_128,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1916_112_2_18_3_6_15_129,"mmivt HandleUIRemoteAlert")
TRACE_MSG(MMIVT_UI_1919_112_2_18_3_6_15_130,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1925_112_2_18_3_6_15_131,"mmivt err state:%d")
TRACE_MSG(MMIVT_UI_1950_112_2_18_3_6_15_132,"mmivt HandleUIEnterMMR")
TRACE_MSG(MMIVT_UI_1953_112_2_18_3_6_15_133,"mmivt null applet")
TRACE_MSG(MMIVT_UI_1959_112_2_18_3_6_15_134,"mmivt err state:%d")
TRACE_MSG(MMIVT_UI_1991_112_2_18_3_6_15_135,"mmivt HandleUIEnterMMR")
TRACE_MSG(MMIVT_UI_1994_112_2_18_3_6_15_136,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2000_112_2_18_3_6_15_137,"mmivt err state:%d sub state:%d")
TRACE_MSG(MMIVT_UI_2055_112_2_18_3_6_15_138,"mmivt HandleUIRedial")
TRACE_MSG(MMIVT_UI_2059_112_2_18_3_6_15_139,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2104_112_2_18_3_6_15_140,"mmivt HandleUIRefErrorInd %d")
TRACE_MSG(MMIVT_UI_2118_112_2_18_3_6_15_141,"mmivt HandleUIRemoteCameraOff")
TRACE_MSG(MMIVT_UI_2122_112_2_18_3_6_15_142,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2128_112_2_18_3_6_15_143,"mmivt err state")
TRACE_MSG(MMIVT_UI_2155_112_2_18_3_6_15_144,"mmivt HandleUIRemoteCameraOn")
TRACE_MSG(MMIVT_UI_2159_112_2_18_3_6_15_145,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2165_112_2_18_3_6_15_146,"mmivt err state")
TRACE_MSG(MMIVT_UI_2190_112_2_18_3_6_15_147,"mmivt HandleUIUpdateAudioDevice")
TRACE_MSG(MMIVT_UI_2194_112_2_18_3_6_15_148,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2223_112_2_18_3_6_15_149,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2271_112_2_18_3_6_16_150,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2294_112_2_18_3_6_16_151,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2317_112_2_18_3_6_16_152,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2340_112_2_18_3_6_16_153,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2364_112_2_18_3_6_16_154,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2390_112_2_18_3_6_16_155,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2413_112_2_18_3_6_16_156,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2438_112_2_18_3_6_16_157,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2462_112_2_18_3_6_16_158,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2500_112_2_18_3_6_16_159,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2533_112_2_18_3_6_16_160,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2566_112_2_18_3_6_16_161,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2590_112_2_18_3_6_16_162,"mmivt null applet")
TRACE_MSG(MMIVT_UI_2898_112_2_18_3_6_17_163,"mmivt auto accept mt")
TRACE_MSG(MMIVT_UI_3422_112_2_18_3_6_18_164,"mmivt HandleVTConnectedWinUpAndDownKey mode: %d")
TRACE_MSG(MMIVT_UI_3460_112_2_18_3_6_18_165,"mmivt HandleVTConnectedWinLeftAndRightKey mode: %d")
TRACE_MSG(MMIVT_UI_3515_112_2_18_3_6_18_166,"mmivt wrong focus %d")
TRACE_MSG(MMIVT_UI_3544_112_2_18_3_6_18_167,"mmivt HandleVTConnectedWinPhoneNumKey mode: %d")
TRACE_MSG(MMIVT_UI_3584_112_2_18_3_6_18_168,"mmivt HandleVTConnectedWinWebKey mode: %d")
TRACE_MSG(MMIVT_UI_3638_112_2_18_3_6_18_169,"mmivt wrong focus %d")
TRACE_MSG(MMIVT_UI_3683_112_2_18_3_6_18_170,"mmivt HandleVTConnectedWinCancelKey mode: %d")
TRACE_MSG(MMIVT_UI_3824_112_2_18_3_6_19_171,"mmivt wrong focus %d")
TRACE_MSG(MMIVT_UI_3880_112_2_18_3_6_19_172,"mmivt HandleVTConnectedWinPenOk err button_id:%d")
TRACE_MSG(MMIVT_UI_5333_112_2_18_3_6_22_173,"SetHeadAndContentInRText index is NULL")
TRACE_MSG(MMIVT_UI_5444_112_2_18_3_6_22_174,"GetBigHeadPhotoByCalledNum call_number_ptr=%d, photo_name_ptr =%d, photo_len_ptr =%d")
TRACE_MSG(MMIVT_UI_5470_112_2_18_3_6_22_175,"mmivt SwitchVideoRotateFocus")
TRACE_MSG(MMIVT_UI_5498_112_2_18_3_6_22_176,"mmivt SwitchVideoOptionFocus focus:%d")
TRACE_MSG(MMIVT_UI_5537_112_2_18_3_6_22_177,"mmivt SwitchMainOptionFocus focus:%d")
TRACE_MSG(MMIVT_UI_5576_112_2_18_3_6_22_178,"mmivt UpdateLcdBright %d")
TRACE_MSG(MMIVT_UI_5621_112_2_18_3_6_22_179,"mmivt UpdateZoom level:%d zoom_out:%d")
TRACE_MSG(MMIVT_UI_5663_112_2_18_3_6_22_180,"mmivt UpdateContrast level:%d contrast_add:%d")
TRACE_MSG(MMIVT_UI_5707_112_2_18_3_6_23_181,"mmivt UpdateBrightness level:%d brightness_add:%d")
TRACE_MSG(MMIVT_UI_5757_112_2_18_3_6_23_182,"mmivt UpdateRotateOption mode:%d, status:%d")
TRACE_MSG(MMIVT_UI_5806_112_2_18_3_6_23_183,"mmivt UpdateAudioOption handfree_show:%d")
TRACE_MSG(MMIVT_UI_5839_112_2_18_3_6_23_184,"mmivt UpdateCameraOption id:%d")
TRACE_MSG(MMIVT_UI_5843_112_2_18_3_6_23_185,"mmivt UpdateCameraOption replacing not switch camera")
TRACE_MSG(MMIVT_UI_5877_112_2_18_3_6_23_186,"mmivt UpdateDisplayModeOption mode:%d")
TRACE_MSG(MMIVT_UI_5915_112_2_18_3_6_23_187,"mmivt EnterMainOptionMode")
TRACE_MSG(MMIVT_UI_5930_112_2_18_3_6_23_188,"mmivt QuitMainOptionMode")
TRACE_MSG(MMIVT_UI_5992_112_2_18_3_6_23_189,"mmivt:wrong focus %d")
TRACE_MSG(MMIVT_UI_6045_112_2_18_3_6_23_190,"mmivt EnterVideoOptionMode")
TRACE_MSG(MMIVT_UI_6060_112_2_18_3_6_23_191,"mmivt QuitVideoOptionMode")
TRACE_MSG(MMIVT_UI_6082_112_2_18_3_6_23_192,"mmivt DisplayVideoOption focus %d")
TRACE_MSG(MMIVT_UI_6106_112_2_18_3_6_24_193,"mmivt DisplayVideoOption wrong sub focus")
TRACE_MSG(MMIVT_UI_6139_112_2_18_3_6_24_194,"mmivt EnterVideoParamMode")
TRACE_MSG(MMIVT_UI_6167_112_2_18_3_6_24_195,"mmivt EnterVideoParamMode error ctrl")
TRACE_MSG(MMIVT_UI_6186_112_2_18_3_6_24_196,"mmivt QuitVideoParamMode")
TRACE_MSG(MMIVT_UI_6202_112_2_18_3_6_24_197,"mmivt DisplayVideoParam video option:%d")
TRACE_MSG(MMIVT_UI_6222_112_2_18_3_6_24_198,"mmivt DisplayVideoParam err rotate mode:%d")
TRACE_MSG(MMIVT_UI_6256_112_2_18_3_6_24_199,"mmivt wrong video option %d")
TRACE_MSG(MMIVT_UI_6316_112_2_18_3_6_24_200,"mmivt EnterUserInputMode")
TRACE_MSG(MMIVT_UI_6402_112_2_18_3_6_24_201,"mmivt QuitUserInputMode")
TRACE_MSG(MMIVT_UI_6453_112_2_18_3_6_24_202,"mmivt EnterVolumeMode")
TRACE_MSG(MMIVT_UI_6470_112_2_18_3_6_24_203,"mmivt QuitVolumeMode")
TRACE_MSG(MMIVT_UI_6591_112_2_18_3_6_25_204,"mmivt.c HandleUser user_input_char: %c")
TRACE_MSG(MMIVT_UI_6601_112_2_18_3_6_25_205,"mmivt send userinput error state:%d")
TRACE_MSG(MMIVT_UI_6666_112_2_18_3_6_25_206,"mmivt null applet")
TRACE_MSG(MMIVT_UI_6766_112_2_18_3_6_25_207,"mmivt HandleEnableCamera %d")
TRACE_MSG(MMIVT_UI_6879_112_2_18_3_6_25_208,"MMIVT StartRecord record_type:%d")
TRACE_MSG(MMIVT_UI_6882_112_2_18_3_6_25_209,"MMIVT StartRecord err type:%d")
TRACE_MSG(MMIVT_UI_6889_112_2_18_3_6_25_210,"MMIVT StartRecord bt in use")
TRACE_MSG(MMIVT_UI_6919_112_2_18_3_6_25_211,"mmivt no enough space to record")
TRACE_MSG(MMIVT_UI_6976_112_2_18_3_6_25_212,"mmivt start record create file failed")
TRACE_MSG(MMIVT_UI_7023_112_2_18_3_6_25_213,"MMIVT StopRecord")
TRACE_MSG(MMIVT_UI_7026_112_2_18_3_6_25_214,"mmivt null applet")
TRACE_MSG(MMIVT_UI_7031_112_2_18_3_6_25_215,"MMIVT not recording")
TRACE_MSG(MMIVT_UI_7055_112_2_18_3_6_26_216,"mmivt errorNotify %d")
TRACE_MSG(MMIVT_UI_7151_112_2_18_3_6_26_217,"mmivt DisableGsensorRotate set = %d, current_angle = %d")
END_TRACE_MAP(MMI_APP_VT_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_VT_TRC_H_

