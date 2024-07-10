/******************************************************************************
 ** File Name:      caf_trc.h                                         *
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
//trace_id:3
#ifndef _CAF_TRC_H_
#define _CAF_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define CAFMEDIA_737_112_2_17_23_0_51_0 ">>>>>>>>>>%s,%s"
#define CAFMEDIA_1493_112_2_17_23_0_53_1 ">>>>>>>>>>>>>>>haud fail"
#define CAFSOCKET_1826_112_2_17_23_1_1_2 "CAFSOCKET_RegAsynSocketEvent regevent = %lx"
#define CAFSOCKET_1970_112_2_17_23_1_1_3 "CAFSOCKET_ASYNTask_Proc signalCode=%lx, sig_ptr=%lx, msg_id=%lx"
#define CAFSOCKET_1976_112_2_17_23_1_1_4 "CAFSOCKET_ASYNTask_Proc exit! sig_ptr = %lx"
#define CAFSOCKET_2039_112_2_17_23_1_1_5 "Socket failed\r\n"
#define CAFSOCKET_2083_112_2_17_23_1_1_6 "CAFSOCKET_HandlePsMsg, signal_ptr->msg_id = %lx"
#define CAFTAPI_634_112_2_17_23_1_6_7 "caftapi.c:CAFITAPI_IsDataServiceReady, s_gprs_state = %d"
#define CAFTAPI_689_112_2_17_23_1_6_8 "[CAFTAPI]:HandleMMSPDPMsg msg_id=%d, result=%d"
#define CAFTAPI_729_112_2_17_23_1_6_9 "Reg item service ptrs is %d"
#define CAFTAPI_734_112_2_17_23_1_6_10 "Every service msg nums is %d"
#define CAFTAPI_753_112_2_17_23_1_6_11 "Reg item service ptrs is %d"
#define CAFTAPI_757_112_2_17_23_1_6_12 "Every service msg nums is %d"
#define CAFTAPI_897_112_2_17_23_1_6_13 "ConvertCAFToMN_DUAL_E,ERROR:caf_dual_sys[%d],mn_dual_sys[%d]"
#define CAFTAPI_915_112_2_17_23_1_6_14 "ConvertMNToCAF_DUAL_E,ERROR:caf_dual_sys_mn[%d],tapi_dual_sys[%d]"
#define CAFTAPI_1585_112_2_17_23_1_8_15 "status is %d"
#define CAFTAPI_1722_112_2_17_23_1_8_16 "ConvertMNToCAF_READSMS_CNF_T is_mms is %d"
#define CAFTAPI_2559_112_2_17_23_1_10_17 "SMS: Get_Current_Seq_Num =%d"
#define CAFTAPI_2591_112_2_17_23_1_10_18 "MNSMS_AppSendSmsEx, result =%d"
#define CAFTAPI_2901_112_2_17_23_1_10_19 "CAFITAPI_SMS_HandleSMSMTInd is_mms is %d"
#define CAFTAPI_3110_112_2_17_23_1_11_20 "caftapi.c:enter function CAFITAPI_HandlePrePsMsg(), msg_id = %lx"
#define CAFTAPI_3116_112_2_17_23_1_11_21 "APP_MN_SIM_READY_IND %d"
#define CAFTAPI_3120_112_2_17_23_1_11_22 "APP_MN_SIM_NOT_READY_IND %d"
#define CAFTAPI_3174_112_2_17_23_1_11_23 "APP_MN_SEND_SMS_CNF send cause =%d"
#define CAFTAPI_3182_112_2_17_23_1_11_24 "APP_MN_SEND_SMS_CNF exist next sms"
#define CAFTAPI_3234_112_2_17_23_1_11_25 "APP_MN_WRITE_SMS_CNF exist next sms"
#define CAFTAPI_3465_112_2_17_23_1_12_26 "msg APP_MN_CALL_ERR_IND, err is %d"
#define CAFTHREAD_384_112_2_17_23_1_12_27 "CAFTHREAD_PostSignalToThread , signal_code is %x"
#define CAFTHREAD_417_112_2_17_23_1_13_28 "CAFTHREAD_PostSignalToApplet :invalid applet_handle ,signal not post out! "
#define ANIMSAMPLE_128_112_2_17_23_1_13_29 "[app]:createinstance"
#define ANIMSAMPLE_136_112_2_17_23_1_13_30 "[app]:createinstance success"
#define ANIMSAMPLE_279_112_2_17_23_1_13_31 "Test-anim:IGUIANIM_AddRef:2:%d\r\n"
#define ANIMSAMPLE_286_112_2_17_23_1_13_32 "Test-anim:IGUIANIM_Release:1:%d\r\n"
#define ANIMSAMPLE_294_112_2_17_23_1_13_33 "Test-anim:IGUIANIM_GetCtrlGuid:%d\r\n"
#define ANIMSAMPLE_302_112_2_17_23_1_13_34 "Test-anim:IGUIANIM_GetCtrlHandle:%d\r\n"
#define ANIMSAMPLE_459_112_2_17_23_1_14_35 "TestCase_1:IGUIANIM_AddRef:2:%d\r\n"
#define ANIMSAMPLE_466_112_2_17_23_1_14_36 "TestCase_1:IGUIANIM_Release:1:%d\r\n"
#define ANIMSAMPLE_474_112_2_17_23_1_14_37 "TestCase_1:IGUIANIM_GetCtrlGuid:%d\r\n"
#define ANIMSAMPLE_482_112_2_17_23_1_14_38 "TestCase_1:IGUIANIM_GetCtrlHandle:%d\r\n"
#define BUTTONSAMPLE_128_112_2_17_23_1_16_39 "[app]:createinstance"
#define BUTTONSAMPLE_136_112_2_17_23_1_16_40 "[app]:createinstance success"
#define BUTTONSAMPLE_198_112_2_17_23_1_16_41 "Test-button:MSG_TP_PRESS_LONG:OK!\r\n"
#define BUTTONSAMPLE_202_112_2_17_23_1_16_42 "Test-button:MSG_NOTIFY_PENLONGOK:OK!\r\n"
#define BUTTONSAMPLE_283_112_2_17_23_1_16_43 "Test-button:IGUIBUTTON_AddRef:2:%d\r\n"
#define BUTTONSAMPLE_290_112_2_17_23_1_16_44 "Test-button:IGUIBUTTON_Release:1:%d\r\n"
#define BUTTONSAMPLE_298_112_2_17_23_1_16_45 "Test-button:IGUIBUTTON_GetCtrlGuid:%d\r\n"
#define BUTTONSAMPLE_306_112_2_17_23_1_16_46 "Test-button:IGUIBUTTON_GetCtrlHandle:%d\r\n"
#define BUTTONSAMPLE_459_112_2_17_23_1_17_47 "TestCase_1:IGUIBUTTON_AddRef:2:%d\r\n"
#define BUTTONSAMPLE_466_112_2_17_23_1_17_48 "TestCase_1:IGUIBUTTON_Release:1:%d\r\n"
#define BUTTONSAMPLE_474_112_2_17_23_1_17_49 "TestCase_1:IGUIBUTTON_GetCtrlGuid:%d\r\n"
#define BUTTONSAMPLE_482_112_2_17_23_1_17_50 "TestCase_1:IGUIBUTTON_GetCtrlHandle:%d\r\n"
#define CAFCALL_175_112_2_17_23_1_19_51 "[app]:applet start"
#define CAFCALL_317_112_2_17_23_1_20_52 "[app]:createinstance"
#define CAFCALL_325_112_2_17_23_1_20_53 "[app]:createinstance success"
#define CAFCALL_331_112_2_17_23_1_20_54 "create tapi failure!\r\n"
#define CAFCALL_335_112_2_17_23_1_20_55 "create heap failure!\r\n"
#define CAFCALL_378_112_2_17_23_1_20_56 "start call error! status =%d"
#define CAFCALL_470_112_2_17_23_1_20_57 "disconnected call error! status =%d"
#define CAFCALL_573_112_2_17_23_1_20_58 "disconnected call error! message =%d"
#define CAFCALL_585_112_2_17_23_1_20_59 "connected call error! status =%d"
#define CAFCALL_593_112_2_17_23_1_20_60 "disconnected call error! status =%d"
#define CAFCALL_610_112_2_17_23_1_20_61 "disconnected call error! status =%d"
#define CAFPHONE_180_112_2_17_23_1_22_62 "[app]:applet start"
#define CAFPHONE_341_112_2_17_23_1_23_63 "[app]:createinstance"
#define CAFPHONE_349_112_2_17_23_1_23_64 "[app]:createinstance success"
#define CAFPHONE_355_112_2_17_23_1_23_65 "create tapi failure!\r\n"
#define CAFPHONE_359_112_2_17_23_1_23_66 "create heap failure!\r\n"
#define CAFSMS_257_112_2_17_23_1_25_67 "[app]:applet start"
#define CAFSMS_438_112_2_17_23_1_25_68 "[app]:createinstance"
#define CAFSMS_446_112_2_17_23_1_25_69 "[app]:createinstance success"
#define CAFSMS_452_112_2_17_23_1_25_70 "create tapi failure!\r\n"
#define CAFSMS_456_112_2_17_23_1_25_71 "create heap failure!\r\n"
#define CAFSMS_608_112_2_17_23_1_26_72 "send sms cause =%d"
#define CAFSMS_652_112_2_17_23_1_26_73 "send sms error!"
#define CAFSMS_729_112_2_17_23_1_26_74 "read sms error!"
#define CAFSMS_962_112_2_17_23_1_26_75 "write sms error!"
#define CAFSMS_1021_112_2_17_23_1_26_76 "SmsCenterAddr len =%d, party_num =%s"
#define CAFSMS_1132_112_2_17_23_1_27_77 "Recv func stop!"
#define CAFSMS_1139_112_2_17_23_1_27_78 "Recv func resume!"
#define CAFSMS_1198_112_2_17_23_1_27_79 "SIM GetSmsNum max = %d, used = %d"
#define CAFSMS_1210_112_2_17_23_1_27_80 "ME GetSmsNum max = %d, used = %d"
#define CAFSMS_1229_112_2_17_23_1_27_81 "SIM GetSmsNum2 max = %d, used = %d"
#define CAFSMS_1240_112_2_17_23_1_27_82 "ME GetSmsNum2 max = %d, used = %d"
#define CAFSMS1_268_112_2_17_23_1_28_83 "[app]:applet start"
#define CAFSMS1_449_112_2_17_23_1_28_84 "[app]:createinstance"
#define CAFSMS1_457_112_2_17_23_1_28_85 "[app]:createinstance success"
#define CAFSMS1_463_112_2_17_23_1_28_86 "create tapi failure!\r\n"
#define CAFSMS1_467_112_2_17_23_1_28_87 "create heap failure!\r\n"
#define CAFSMS1_517_112_2_17_23_1_28_88 "receive sms id = %d, cause = %d, dual_sys = %d,storage = %d"
#define CAFSMS1_518_112_2_17_23_1_28_89 "number len = %d, number = %s"
#define CAFSMS1_519_112_2_17_23_1_28_90 "content len = %d, content = %s"
#define CAFSMS1_685_112_2_17_23_1_29_91 "read sms error!"
#define CAFSMS1_723_112_2_17_23_1_29_92 "write sms error!"
#define CAFSMS1_736_112_2_17_23_1_29_93 "SmsCenterAddr len =%d, party_num =%s"
#define CAFSMS1_775_112_2_17_23_1_29_94 "send sms error!"
#define CAFSMS1_786_112_2_17_23_1_29_95 "recv func error!"
#define CAFSMS1_796_112_2_17_23_1_29_96 "SetSmsReceiveFunc cause =%d, is_receive_sms =%d"
#define CAFSMS1_799_112_2_17_23_1_29_97 "SetSmsReceiveFunc ok"
#define CAFSMS1_811_112_2_17_23_1_29_98 "send sms ok"
#define CAFSMS1_815_112_2_17_23_1_29_99 "send sms failure"
#define CAFSMS1_839_112_2_17_23_1_29_100 "SmsCenterAddr len =%d, party_num =%s"
#define CAFSMS1_890_112_2_17_23_1_29_101 "read sms error!"
#define CAFSMS1_902_112_2_17_23_1_29_102 "delete sms id is %d"
#define CAFSMS1_1184_112_2_17_23_1_30_103 "Recv func stop!"
#define CAFSMS1_1191_112_2_17_23_1_30_104 "Recv func resume!"
#define CAFSMS1_1201_112_2_17_23_1_30_105 "SetSmsReceiveFunc cause =%d, is_receive_sms =%d"
#define CAFSMS1_1204_112_2_17_23_1_30_106 "SetSmsReceiveFunc ok"
#define CAFSMS1_1273_112_2_17_23_1_30_107 "write sms error!"
#define CAFSMS1_1332_112_2_17_23_1_30_108 "SmsCenterAddr len =%d, party_num =%s"
#define CAFSMS1_1445_112_2_17_23_1_30_109 "SIM GetSmsNum max = %d, used = %d"
#define CAFSMS1_1454_112_2_17_23_1_30_110 "ME GetSmsNum max = %d, used = %d"
#define CHECKBOX_250_112_2_17_23_1_32_111 "[app]:createinstance"
#define CHECKBOX_258_112_2_17_23_1_32_112 "[app]:createinstance success"
#define CHECKBOXCTRL_89_112_2_17_23_1_34_113 "[app]:createinstance"
#define DROPDOWNLISTSAMPLE_125_112_2_17_23_1_36_114 "[app]:createinstance"
#define DROPDOWNLISTSAMPLE_133_112_2_17_23_1_37_115 "[app]:createinstance success"
#define DROPDOWNLISTSAMPLE_284_112_2_17_23_1_37_116 "Test-dropdownlist:IGUIDROPDOWNLIST_AddRef:2:%d\r\n"
#define DROPDOWNLISTSAMPLE_291_112_2_17_23_1_37_117 "Test-dropdownlist:IGUIDROPDOWNLIST_Release:1:%d\r\n"
#define DROPDOWNLISTSAMPLE_299_112_2_17_23_1_37_118 "Test-dropdownlist:IGUIDROPDOWNLIST_GetCtrlGuid:%d\r\n"
#define DROPDOWNLISTSAMPLE_307_112_2_17_23_1_37_119 "Test-dropdownlist:IGUIDROPDOWNLIST_GetCtrlHandle:%d\r\n"
#define DROPDOWNLISTSAMPLE_459_112_2_17_23_1_37_120 "TestCase_1:IGUIDROPDOWNLIST_AddRef:2:%d\r\n"
#define DROPDOWNLISTSAMPLE_466_112_2_17_23_1_37_121 "TestCase_1:IGUIDROPDOWNLIST_Release:1:%d\r\n"
#define DROPDOWNLISTSAMPLE_474_112_2_17_23_1_37_122 "TestCase_1:IGUIDROPDOWNLIST_GetCtrlGuid:%d\r\n"
#define DROPDOWNLISTSAMPLE_482_112_2_17_23_1_37_123 "TestCase_1:IGUIDROPDOWNLIST_GetCtrlHandle:%d\r\n"
#define EDITSAMPLE_116_112_2_17_23_1_39_124 "[app]:createinstance"
#define EDITSAMPLE_124_112_2_17_23_1_39_125 "[app]:createinstance success"
#define EDITSAMPLE_305_112_2_17_23_1_40_126 "Test-edit:IGUIEDIT_AddRef:2:%d\r\n"
#define EDITSAMPLE_312_112_2_17_23_1_40_127 "Test-edit:IGUIEDIT_Release:1:%d\r\n"
#define EDITSAMPLE_320_112_2_17_23_1_40_128 "Test-edit:IGUIEDIT_GetCtrlGuid:%d\r\n"
#define EDITSAMPLE_328_112_2_17_23_1_40_129 "Test-edit:IGUIEDIT_GetCtrlHandle:%d\r\n"
#define FILESYSSAMPLE_132_112_2_17_23_1_42_130 "[app]:createinstance"
#define FILESYSSAMPLE_140_112_2_17_23_1_42_131 "[app]:createinstance success"
#define FILESYSSAMPLE_356_112_2_17_23_1_42_132 "Test-Filesys:IFILE_AddRef:2:%d\r\n"
#define FILESYSSAMPLE_363_112_2_17_23_1_42_133 "Test-Filesys:IFILE_Release:1:%d\r\n"
#define FILESYSSAMPLE_381_112_2_17_23_1_42_134 "Test-Filesys:IFILE_GetDiskFreeSpace:%d:%d\r\n"
#define FILESYSSAMPLE_396_112_2_17_23_1_42_135 "Test-Filesys:IFILE_GetDiskUsedSpace:%d:%d\r\n"
#define FILESYSSAMPLE_606_112_2_17_23_1_43_136 "TestCase_1:ifile_ptr=%d\r\n"
#define FILESYSSAMPLE_612_112_2_17_23_1_43_137 "TestCase_1:IFILE_AddRef=%d\r\n"
#define FILESYSSAMPLE_619_112_2_17_23_1_43_138 "TestCase_1:IFILE_Release=%d\r\n"
#define FILESYSSAMPLE_637_112_2_17_23_1_43_139 "TestCase_1:IFILE_GetDiskFreeSpace:low_word=%d;high_word=%d\r\n"
#define FILESYSSAMPLE_652_112_2_17_23_1_43_140 "TestCase_1:IFILE_GetDiskUsedSpace:low_word=%d;high_word=%d\r\n"
#define ICONLISTSAMPLE_118_112_2_17_23_1_45_141 "[app]:createinstance"
#define ICONLISTSAMPLE_126_112_2_17_23_1_45_142 "[app]:createinstance success"
#define ICONLISTSAMPLE_271_112_2_17_23_1_45_143 "Test-iconlist:IGUIICONLIST_AddRef:2:%d\r\n"
#define ICONLISTSAMPLE_278_112_2_17_23_1_45_144 "Test-iconlist:IGUIICONLIST_Release:1:%d\r\n"
#define ICONLISTSAMPLE_286_112_2_17_23_1_45_145 "Test-iconlist:IGUIICONLIST_GetCtrlGuid:%d\r\n"
#define ICONLISTSAMPLE_294_112_2_17_23_1_46_146 "Test-iconlist:IGUIICONLIST_GetCtrlHandle:%d\r\n"
#define IDC_103_112_2_17_23_1_48_147 "[app]:applet start"
#define IDC_198_112_2_17_23_1_48_148 "[app]:createinstance"
#define IDC_206_112_2_17_23_1_48_149 "[app]:createinstance success"
#define IDC_292_112_2_17_23_1_48_150 "old font is %d \n "
#define IDC_297_112_2_17_23_1_48_151 "font_new_type  is %d \n "
#define IDC_300_112_2_17_23_1_48_152 "after set font fornt type  is %d \n "
#define IDC_312_112_2_17_23_1_48_153 "font_old_effect is %d \n "
#define IDC_316_112_2_17_23_1_48_154 "font_new_effect  is %d \n "
#define IDC_320_112_2_17_23_1_48_155 "after set font effect is %d \n "
#define IDC_332_112_2_17_23_1_48_156 "font_old_color is %d \n "
#define IDC_337_112_2_17_23_1_48_157 "font_new_color  is %d \n "
#define IDC_341_112_2_17_23_1_48_158 "after set font color is %d \n "
#define IDC_356_112_2_17_23_1_48_159 "font_aling is %d \n "
#define IDC_383_112_2_17_23_1_48_160 "pen_color is %d \n "
#define IDC_385_112_2_17_23_1_48_161 "pen_size is %d \n "
#define IDC_393_112_2_17_23_1_48_162 "test_point_color is %d \n "
#define IDC_552_112_2_17_23_1_49_163 "IDC_GetFontWidth is %d \n "
#define IDC_555_112_2_17_23_1_49_164 "IDC_GetFontWidth is %d \n "
#define IDC_561_112_2_17_23_1_49_165 "IDC_GetStringWidth is %d \n "
#define IDC_564_112_2_17_23_1_49_166 "IDC_GetStringHeight is %d \n "
#define IMAGESAMPLE_126_112_2_17_23_1_50_167 "[app]:createinstance"
#define IMAGESAMPLE_134_112_2_17_23_1_50_168 "[app]:createinstance success"
#define IMAGESAMPLE_239_112_2_17_23_1_50_169 "Test-IMAGE:IIMAGE_AddRef:2:%d\r\n"
#define IMAGESAMPLE_246_112_2_17_23_1_50_170 "Test-IMAGE:IIMAGE_Release:1:%d\r\n"
#define IMAGESAMPLE_341_112_2_17_23_1_51_171 "TestCase_1:IIMAGE_AddRef:2:%d\r\n"
#define IMAGESAMPLE_348_112_2_17_23_1_51_172 "TestCase_1:IIMAGE_Release:1:%d\r\n"
#define ISHELL_103_112_2_17_23_1_52_173 "[app]:applet start"
#define ISHELL_197_112_2_17_23_1_53_174 "[app]:createinstance"
#define ISHELL_205_112_2_17_23_1_53_175 "[app]:createinstance success"
#define ISHELL_294_112_2_17_23_1_53_176 "screen_info: width  is  %d ;  high is %d \n "
#define ISHELL_312_112_2_17_23_1_53_177 "disk d  Memory   info free_high is   %d ;  free_low is %d ;used_high is %d ; used_low is %d  n "
#define ITHREAD_103_112_2_17_23_1_54_178 "[app]:applet start"
#define ITHREAD_130_112_2_17_23_1_54_179 "HandleEvent MSG_ITHREAD_TEST1 data is : %s!\n "
#define ITHREAD_210_112_2_17_23_1_54_180 "[app]:createinstance"
#define ITHREAD_218_112_2_17_23_1_54_181 "[app]:createinstance success"
#define ITHREAD_240_112_2_17_23_1_54_182 "thread_test fun : 1"
#define ITHREAD_242_112_2_17_23_1_54_183 "thread_test fun : 2"
#define ITHREAD_251_112_2_17_23_1_54_184 "thread_test fun : 3"
#define ITHREAD_253_112_2_17_23_1_54_185 "thread_test fun : 4"
#define ITHREAD_260_112_2_17_23_1_54_186 "thread_test fun : 5"
#define ITHREAD_262_112_2_17_23_1_55_187 "thread_test fun : 6"
#define ITHREAD_270_112_2_17_23_1_55_188 "thread_test fun : 7"
#define ITHREAD_273_112_2_17_23_1_55_189 "thread_test fun : 8"
#define ITHREAD_289_112_2_17_23_1_55_190 "ITHREAD_test_Proc fun : 1"
#define ITHREAD_293_112_2_17_23_1_55_191 "ITHREAD_test_Proc fun : 2"
#define ITHREAD_297_112_2_17_23_1_55_192 "ITHREAD_test_Proc fun : 3"
#define ITHREAD_303_112_2_17_23_1_55_193 "ITHREAD_test_Proc  get SIG_ITHREAD_TEST1 data is : %s!\n "
#define ITHREAD_308_112_2_17_23_1_55_194 "ITHREAD_test_Proc : get unknow msg !\n "
#define LABELSAMPLE_129_112_2_17_23_1_56_195 "[app]:createinstance"
#define LABELSAMPLE_137_112_2_17_23_1_56_196 "[app]:createinstance success"
#define LABELSAMPLE_283_112_2_17_23_1_56_197 "Test-label:IGUILABEL_AddRef:2:%d\r\n"
#define LABELSAMPLE_290_112_2_17_23_1_56_198 "Test-label:IGUILABEL_Release:1:%d\r\n"
#define LABELSAMPLE_298_112_2_17_23_1_56_199 "Test-label:IGUILABEL_GetCtrlGuid:%d\r\n"
#define LABELSAMPLE_306_112_2_17_23_1_56_200 "Test-label:IGUILABEL_GetCtrlHandle:%d\r\n"
#define LABELSAMPLE_470_112_2_17_23_1_57_201 "TestCase_1:IGUILABEL_AddRef:2:%d\r\n"
#define LABELSAMPLE_477_112_2_17_23_1_57_202 "TestCase_1:IGUILABEL_Release:1:%d\r\n"
#define LABELSAMPLE_485_112_2_17_23_1_57_203 "TestCase_1:IGUILABEL_GetCtrlGuid:%d\r\n"
#define LABELSAMPLE_493_112_2_17_23_1_57_204 "TestCase_1:IGUILABEL_GetCtrlHandle:%d\r\n"
#define LISTSAMPLE_125_112_2_17_23_1_59_205 "[app]:createinstance"
#define LISTSAMPLE_133_112_2_17_23_1_59_206 "[app]:createinstance success"
#define LISTSAMPLE_302_112_2_17_23_1_59_207 "Test-list:IGUILIST_AddRef:2:%d\r\n"
#define LISTSAMPLE_309_112_2_17_23_1_59_208 "Test-list:IGUILIST_Release:1:%d\r\n"
#define LISTSAMPLE_317_112_2_17_23_1_59_209 "Test-list:IGUILIST_GetCtrlGuid:%d\r\n"
#define LISTSAMPLE_325_112_2_17_23_1_59_210 "Test-list:IGUILIST_GetCtrlHandle:%d\r\n"
#define LISTSAMPLE_551_112_2_17_23_2_0_211 "Test-list:IGUILIST_GetCurItemIndex:%d\r\n"
#define LISTSAMPLE_559_112_2_17_23_2_0_212 "Test-list:IGUILIST_GetTotalItemNum:%d\r\n"
#define LISTSAMPLE_579_112_2_17_23_2_0_213 "TestCase_1:IGUILIST_AddRef:2:%d\r\n"
#define LISTSAMPLE_586_112_2_17_23_2_0_214 "TestCase_1:IGUILIST_Release:1:%d\r\n"
#define LISTSAMPLE_594_112_2_17_23_2_0_215 "TestCase_1:IGUILIST_GetCtrlGuid:%d\r\n"
#define LISTSAMPLE_602_112_2_17_23_2_0_216 "TestCase_1:IGUILIST_GetCtrlHandle:%d\r\n"
#define LISTSAMPLE_901_112_2_17_23_2_0_217 "Test-list:IGUILIST_GetCurItemIndex:%d\r\n"
#define LISTSAMPLE_909_112_2_17_23_2_0_218 "Test-list:IGUILIST_GetTotalItemNum:%d\r\n"
#define MEDIATEST_150_112_2_17_23_2_5_219 "[app]:applet start"
#define MEDIATEST_164_112_2_17_23_2_5_220 "[app]:ISHELL_CreateInstance - media fail! "
#define MEDIATEST_229_112_2_17_23_2_5_221 "[app]:createinstance"
#define MEDIATEST_237_112_2_17_23_2_5_222 "[app]:createinstance success"
#define MEDIATEST_297_112_2_17_23_2_5_223 "[app]:IMEDIA_SetMediaData -  fail! "
#define MEDIATEST_313_112_2_17_23_2_5_224 "[app]:>>>>>>IMEDIA_control -  %d! "
#define MEDIATEST_408_112_2_17_23_2_6_225 "[app]:IMEDIA_SetMediaData -  fail! "
#define MEDIATEST_431_112_2_17_23_2_6_226 "[app]:>>>>>>IMEDIA_control -  %d! "
#define MEDIATEST_521_112_2_17_23_2_6_227 "[app]:IMEDIA_SetMediaData -  fail! "
#define MEDIATEST_537_112_2_17_23_2_6_228 "[app]:>>>>>>IMEDIA_control -  %d! "
#define MEDIATEST_631_112_2_17_23_2_6_229 "[app]:IMEDIA_SetMediaData -  fail! "
#define MEDIATEST_647_112_2_17_23_2_6_230 "[app]:>>>>>>IMEDIA_control -  %d! "
#define POPMENUSAMPLE_85_112_2_17_23_2_8_231 "[app]:applet start"
#define POPMENUSAMPLE_192_112_2_17_23_2_8_232 "[app]:createinstance"
#define POPMENUSAMPLE_200_112_2_17_23_2_8_233 "[app]:createinstance success"
#define POPMENUSAMPLE_249_112_2_17_23_2_8_234 "node_id = %d\r\n"
#define ABC_227_112_2_17_23_2_9_235 "[socket]:applet start"
#define ABC_270_112_2_17_23_2_9_236 "[testclient]:createinstance"
#define ABC_278_112_2_17_23_2_9_237 "[socket]:createinstance success"
#define ABC_282_112_2_17_23_2_9_238 "create heap failure!\r\n"
#define ABC_286_112_2_17_23_2_9_239 "create tapi failure!\r\n"
#define ABC_290_112_2_17_23_2_9_240 "dial failure!\r\n"
#define ABC_673_112_2_17_23_2_10_241 "HandleAsyncliWindow msg id is %lx\r\n"
#define ABC_695_112_2_17_23_2_10_242 "create netmgr failure!\r\n"
#define ABC_708_112_2_17_23_2_10_243 "domain address error!\r\n"
#define ABC_716_112_2_17_23_2_10_244 "address:%s\r\n"
#define ABC_753_112_2_17_23_2_10_245 "Address %x  Port %x\r\n"
#define ABC_781_112_2_17_23_2_10_246 "MSG_ASYN_SOCKET_CLOSE_IND, server disconnect"
#define ABC_796_112_2_17_23_2_10_247 "MSG_USER_START, user exit"
#define ABC_915_112_2_17_23_2_10_248 "ITestSocket_TCLI_ColloquyCase state =%d"
#define ABC_964_112_2_17_23_2_10_249 "rev %s\r\n"
#define ABC_977_112_2_17_23_2_10_250 "send %s\r\n"
#define ABC_1009_112_2_17_23_2_10_251 "rev %s\r\n"
#define ABC_1023_112_2_17_23_2_10_252 "send %s\r\n"
#define CAFMODULE_GEN_206_112_2_17_23_2_12_253 "[app]:applet end"
#define TESTSOCKET_72_112_2_17_23_2_12_254 "ITestSocket_TestClient Start state =%d"
#define TESTSOCKET_81_112_2_17_23_2_12_255 "create netmgr failure!\r\n"
#define TESTSOCKET_93_112_2_17_23_2_12_256 "domain address error!\r\n"
#define TESTSOCKET_101_112_2_17_23_2_12_257 "address:%s\r\n"
#define TESTSOCKET_123_112_2_17_23_2_12_258 "Address %x  Port %x\r\n"
#define TESTSOCKET_128_112_2_17_23_2_12_259 "Connect failure!\r\n"
#define TESTSOCKET_131_112_2_17_23_2_12_260 "socket connect ok"
#define TESTSOCKET_172_112_2_17_23_2_12_261 "select error!\r\n"
#define TESTSOCKET_201_112_2_17_23_2_12_262 "rev %s\r\n"
#define TESTSOCKET_202_112_2_17_23_2_12_263 "send %s\r\n"
#define TESTSOCKET_219_112_2_17_23_2_12_264 "ITestSocket_TCLI_ColloquyCase state =%d"
#define TESTSOCKET_329_112_2_17_23_2_13_265 "create tapi failure!\r\n"
#define TESTSOCKET_339_112_2_17_23_2_13_266 "domain address error!\r\n"
#define TESTSOCKET_347_112_2_17_23_2_13_267 "address:%s\r\n"
#define TESTSOCKET_367_112_2_17_23_2_13_268 "Address %x  Port %x\r\n"
#define TESTSOCKET_372_112_2_17_23_2_13_269 "Bind failure!\r\n"
#define TESTSOCKET_378_112_2_17_23_2_13_270 "listen error!\r\n"
#define TESTSOCKET_398_112_2_17_23_2_13_271 "accept error!\r\n"
#define TESTSOCKET_421_112_2_17_23_2_13_272 "select error!\r\n"
#define TESTSOCKET_455_112_2_17_23_2_13_273 "rev %s\r\n"
#define TESTSOCKET_456_112_2_17_23_2_13_274 "send %s\r\n"
#define TESTSOCKET_531_112_2_17_23_2_13_275 "create tapi failure!\r\n"
#define TESTSOCKET_541_112_2_17_23_2_13_276 "domain address error!\r\n"
#define TESTSOCKET_549_112_2_17_23_2_13_277 "address:%s\r\n"
#define TESTSOCKET_562_112_2_17_23_2_13_278 "Address %x  addr str %s\r\n"
#define TESTSOCKET_567_112_2_17_23_2_13_279 "Address %x  Port %x\r\n"
#define TESTSOCKET_572_112_2_17_23_2_13_280 "Connect failure!\r\n"
#define TESTSOCKET_611_112_2_17_23_2_13_281 "select error!\r\n"
#define TESTSOCKET_761_112_2_17_23_2_13_282 "create tapi failure!\r\n"
#define TESTSOCKET_771_112_2_17_23_2_13_283 "domain address error!\r\n"
#define TESTSOCKET_779_112_2_17_23_2_13_284 "address:%s\r\n"
#define TESTSOCKET_792_112_2_17_23_2_13_285 "Address %x  Port %x\r\n"
#define TESTSOCKET_797_112_2_17_23_2_13_286 "Bind failure!\r\n"
#define TESTSOCKET_980_112_2_17_23_2_14_287 "CAF_MEMCMP result is %d"
#define TESTSOCKET_983_112_2_17_23_2_14_288 "CAF_STRCMP result is %d"
#define TESTSOCKET_986_112_2_17_23_2_14_289 "CAF_STRNCMP result is %d"
#define TESTSOCKET_989_112_2_17_23_2_14_290 "CAF_STRCOLL result is %d"
#define TESTSOCKET_992_112_2_17_23_2_14_291 "CAF_STRXFRM result is %s, size is %d"
#define TESTSOCKET_998_112_2_17_23_2_14_292 "CAF_ATOI result is %d"
#define TESTSOCKET_1005_112_2_17_23_2_14_293 "CAF_WSTRTOSTR result is %s"
#define TESTSOCKET_1022_112_2_17_23_2_14_294 "CAF_ALLOC CAF_STRCPY is %s"
#define TESTSOCKET_1027_112_2_17_23_2_14_295 "Address %x  addr str %s\r\n"
#define SOUNDTEST_119_112_2_17_23_2_15_296 "[app]:applet start"
#define SOUNDTEST_132_112_2_17_23_2_15_297 "[app]:ISHELL_CreateInstance - sound fail! "
#define SOUNDTEST_234_112_2_17_23_2_16_298 "[app]:createinstance"
#define SOUNDTEST_242_112_2_17_23_2_16_299 "[app]:createinstance success"
#define TESTCONTROL_113_112_2_17_23_2_17_300 "[app]:applet start"
#define TESTCONTROL_263_112_2_17_23_2_18_301 "[app]:createinstance"
#define TESTCONTROL_271_112_2_17_23_2_18_302 "[app]:createinstance success"
#define TESTDLL_111_112_2_17_23_2_19_303 "[app]:applet start"
#define TESTDLL_275_112_2_17_23_2_19_304 "[app]:createinstance"
#define TESTDLL_283_112_2_17_23_2_19_305 "[app]:createinstance success"
#define TEXTBOXSAMPLE_125_112_2_17_23_2_21_306 "[app]:createinstance"
#define TEXTBOXSAMPLE_133_112_2_17_23_2_21_307 "[app]:createinstance success"
#define TEXTBOXSAMPLE_284_112_2_17_23_2_21_308 "Test-textbox:IGUITEXTBOX_AddRef:2:%d\r\n"
#define TEXTBOXSAMPLE_291_112_2_17_23_2_21_309 "Test-textbox:IGUITEXTBOX_Release:1:%d\r\n"
#define TEXTBOXSAMPLE_299_112_2_17_23_2_21_310 "Test-textbox:IGUITEXTBOX_GetCtrlGuid:%d\r\n"
#define TEXTBOXSAMPLE_307_112_2_17_23_2_21_311 "Test-textbox:IGUITEXTBOX_GetCtrlHandle:%d\r\n"
#define TEXTBOXSAMPLE_470_112_2_17_23_2_22_312 "TestCase_1:IGUITEXTBOX_AddRef:2:%d\r\n"
#define TEXTBOXSAMPLE_477_112_2_17_23_2_22_313 "TestCase_1:IGUITEXTBOX_Release:1:%d\r\n"
#define TEXTBOXSAMPLE_485_112_2_17_23_2_22_314 "TestCase_1:IGUITEXTBOX_GetCtrlGuid:%d\r\n"
#define TEXTBOXSAMPLE_493_112_2_17_23_2_22_315 "TestCase_1:IGUITEXTBOX_GetCtrlHandle:%d\r\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(CAF_TRC)
TRACE_MSG(CAFMEDIA_737_112_2_17_23_0_51_0,">>>>>>>>>>%s,%s")
TRACE_MSG(CAFMEDIA_1493_112_2_17_23_0_53_1,">>>>>>>>>>>>>>>haud fail")
TRACE_MSG(CAFSOCKET_1826_112_2_17_23_1_1_2,"CAFSOCKET_RegAsynSocketEvent regevent = %lx")
TRACE_MSG(CAFSOCKET_1970_112_2_17_23_1_1_3,"CAFSOCKET_ASYNTask_Proc signalCode=%lx, sig_ptr=%lx, msg_id=%lx")
TRACE_MSG(CAFSOCKET_1976_112_2_17_23_1_1_4,"CAFSOCKET_ASYNTask_Proc exit! sig_ptr = %lx")
TRACE_MSG(CAFSOCKET_2039_112_2_17_23_1_1_5,"Socket failed\r\n")
TRACE_MSG(CAFSOCKET_2083_112_2_17_23_1_1_6,"CAFSOCKET_HandlePsMsg, signal_ptr->msg_id = %lx")
TRACE_MSG(CAFTAPI_634_112_2_17_23_1_6_7,"caftapi.c:CAFITAPI_IsDataServiceReady, s_gprs_state = %d")
TRACE_MSG(CAFTAPI_689_112_2_17_23_1_6_8,"[CAFTAPI]:HandleMMSPDPMsg msg_id=%d, result=%d")
TRACE_MSG(CAFTAPI_729_112_2_17_23_1_6_9,"Reg item service ptrs is %d")
TRACE_MSG(CAFTAPI_734_112_2_17_23_1_6_10,"Every service msg nums is %d")
TRACE_MSG(CAFTAPI_753_112_2_17_23_1_6_11,"Reg item service ptrs is %d")
TRACE_MSG(CAFTAPI_757_112_2_17_23_1_6_12,"Every service msg nums is %d")
TRACE_MSG(CAFTAPI_897_112_2_17_23_1_6_13,"ConvertCAFToMN_DUAL_E,ERROR:caf_dual_sys[%d],mn_dual_sys[%d]")
TRACE_MSG(CAFTAPI_915_112_2_17_23_1_6_14,"ConvertMNToCAF_DUAL_E,ERROR:caf_dual_sys_mn[%d],tapi_dual_sys[%d]")
TRACE_MSG(CAFTAPI_1585_112_2_17_23_1_8_15,"status is %d")
TRACE_MSG(CAFTAPI_1722_112_2_17_23_1_8_16,"ConvertMNToCAF_READSMS_CNF_T is_mms is %d")
TRACE_MSG(CAFTAPI_2559_112_2_17_23_1_10_17,"SMS: Get_Current_Seq_Num =%d")
TRACE_MSG(CAFTAPI_2591_112_2_17_23_1_10_18,"MNSMS_AppSendSmsEx, result =%d")
TRACE_MSG(CAFTAPI_2901_112_2_17_23_1_10_19,"CAFITAPI_SMS_HandleSMSMTInd is_mms is %d")
TRACE_MSG(CAFTAPI_3110_112_2_17_23_1_11_20,"caftapi.c:enter function CAFITAPI_HandlePrePsMsg(), msg_id = %lx")
TRACE_MSG(CAFTAPI_3116_112_2_17_23_1_11_21,"APP_MN_SIM_READY_IND %d")
TRACE_MSG(CAFTAPI_3120_112_2_17_23_1_11_22,"APP_MN_SIM_NOT_READY_IND %d")
TRACE_MSG(CAFTAPI_3174_112_2_17_23_1_11_23,"APP_MN_SEND_SMS_CNF send cause =%d")
TRACE_MSG(CAFTAPI_3182_112_2_17_23_1_11_24,"APP_MN_SEND_SMS_CNF exist next sms")
TRACE_MSG(CAFTAPI_3234_112_2_17_23_1_11_25,"APP_MN_WRITE_SMS_CNF exist next sms")
TRACE_MSG(CAFTAPI_3465_112_2_17_23_1_12_26,"msg APP_MN_CALL_ERR_IND, err is %d")
TRACE_MSG(CAFTHREAD_384_112_2_17_23_1_12_27,"CAFTHREAD_PostSignalToThread , signal_code is %x")
TRACE_MSG(CAFTHREAD_417_112_2_17_23_1_13_28,"CAFTHREAD_PostSignalToApplet :invalid applet_handle ,signal not post out! ")
TRACE_MSG(ANIMSAMPLE_128_112_2_17_23_1_13_29,"[app]:createinstance")
TRACE_MSG(ANIMSAMPLE_136_112_2_17_23_1_13_30,"[app]:createinstance success")
TRACE_MSG(ANIMSAMPLE_279_112_2_17_23_1_13_31,"Test-anim:IGUIANIM_AddRef:2:%d\r\n")
TRACE_MSG(ANIMSAMPLE_286_112_2_17_23_1_13_32,"Test-anim:IGUIANIM_Release:1:%d\r\n")
TRACE_MSG(ANIMSAMPLE_294_112_2_17_23_1_13_33,"Test-anim:IGUIANIM_GetCtrlGuid:%d\r\n")
TRACE_MSG(ANIMSAMPLE_302_112_2_17_23_1_13_34,"Test-anim:IGUIANIM_GetCtrlHandle:%d\r\n")
TRACE_MSG(ANIMSAMPLE_459_112_2_17_23_1_14_35,"TestCase_1:IGUIANIM_AddRef:2:%d\r\n")
TRACE_MSG(ANIMSAMPLE_466_112_2_17_23_1_14_36,"TestCase_1:IGUIANIM_Release:1:%d\r\n")
TRACE_MSG(ANIMSAMPLE_474_112_2_17_23_1_14_37,"TestCase_1:IGUIANIM_GetCtrlGuid:%d\r\n")
TRACE_MSG(ANIMSAMPLE_482_112_2_17_23_1_14_38,"TestCase_1:IGUIANIM_GetCtrlHandle:%d\r\n")
TRACE_MSG(BUTTONSAMPLE_128_112_2_17_23_1_16_39,"[app]:createinstance")
TRACE_MSG(BUTTONSAMPLE_136_112_2_17_23_1_16_40,"[app]:createinstance success")
TRACE_MSG(BUTTONSAMPLE_198_112_2_17_23_1_16_41,"Test-button:MSG_TP_PRESS_LONG:OK!\r\n")
TRACE_MSG(BUTTONSAMPLE_202_112_2_17_23_1_16_42,"Test-button:MSG_NOTIFY_PENLONGOK:OK!\r\n")
TRACE_MSG(BUTTONSAMPLE_283_112_2_17_23_1_16_43,"Test-button:IGUIBUTTON_AddRef:2:%d\r\n")
TRACE_MSG(BUTTONSAMPLE_290_112_2_17_23_1_16_44,"Test-button:IGUIBUTTON_Release:1:%d\r\n")
TRACE_MSG(BUTTONSAMPLE_298_112_2_17_23_1_16_45,"Test-button:IGUIBUTTON_GetCtrlGuid:%d\r\n")
TRACE_MSG(BUTTONSAMPLE_306_112_2_17_23_1_16_46,"Test-button:IGUIBUTTON_GetCtrlHandle:%d\r\n")
TRACE_MSG(BUTTONSAMPLE_459_112_2_17_23_1_17_47,"TestCase_1:IGUIBUTTON_AddRef:2:%d\r\n")
TRACE_MSG(BUTTONSAMPLE_466_112_2_17_23_1_17_48,"TestCase_1:IGUIBUTTON_Release:1:%d\r\n")
TRACE_MSG(BUTTONSAMPLE_474_112_2_17_23_1_17_49,"TestCase_1:IGUIBUTTON_GetCtrlGuid:%d\r\n")
TRACE_MSG(BUTTONSAMPLE_482_112_2_17_23_1_17_50,"TestCase_1:IGUIBUTTON_GetCtrlHandle:%d\r\n")
TRACE_MSG(CAFCALL_175_112_2_17_23_1_19_51,"[app]:applet start")
TRACE_MSG(CAFCALL_317_112_2_17_23_1_20_52,"[app]:createinstance")
TRACE_MSG(CAFCALL_325_112_2_17_23_1_20_53,"[app]:createinstance success")
TRACE_MSG(CAFCALL_331_112_2_17_23_1_20_54,"create tapi failure!\r\n")
TRACE_MSG(CAFCALL_335_112_2_17_23_1_20_55,"create heap failure!\r\n")
TRACE_MSG(CAFCALL_378_112_2_17_23_1_20_56,"start call error! status =%d")
TRACE_MSG(CAFCALL_470_112_2_17_23_1_20_57,"disconnected call error! status =%d")
TRACE_MSG(CAFCALL_573_112_2_17_23_1_20_58,"disconnected call error! message =%d")
TRACE_MSG(CAFCALL_585_112_2_17_23_1_20_59,"connected call error! status =%d")
TRACE_MSG(CAFCALL_593_112_2_17_23_1_20_60,"disconnected call error! status =%d")
TRACE_MSG(CAFCALL_610_112_2_17_23_1_20_61,"disconnected call error! status =%d")
TRACE_MSG(CAFPHONE_180_112_2_17_23_1_22_62,"[app]:applet start")
TRACE_MSG(CAFPHONE_341_112_2_17_23_1_23_63,"[app]:createinstance")
TRACE_MSG(CAFPHONE_349_112_2_17_23_1_23_64,"[app]:createinstance success")
TRACE_MSG(CAFPHONE_355_112_2_17_23_1_23_65,"create tapi failure!\r\n")
TRACE_MSG(CAFPHONE_359_112_2_17_23_1_23_66,"create heap failure!\r\n")
TRACE_MSG(CAFSMS_257_112_2_17_23_1_25_67,"[app]:applet start")
TRACE_MSG(CAFSMS_438_112_2_17_23_1_25_68,"[app]:createinstance")
TRACE_MSG(CAFSMS_446_112_2_17_23_1_25_69,"[app]:createinstance success")
TRACE_MSG(CAFSMS_452_112_2_17_23_1_25_70,"create tapi failure!\r\n")
TRACE_MSG(CAFSMS_456_112_2_17_23_1_25_71,"create heap failure!\r\n")
TRACE_MSG(CAFSMS_608_112_2_17_23_1_26_72,"send sms cause =%d")
TRACE_MSG(CAFSMS_652_112_2_17_23_1_26_73,"send sms error!")
TRACE_MSG(CAFSMS_729_112_2_17_23_1_26_74,"read sms error!")
TRACE_MSG(CAFSMS_962_112_2_17_23_1_26_75,"write sms error!")
TRACE_MSG(CAFSMS_1021_112_2_17_23_1_26_76,"SmsCenterAddr len =%d, party_num =%s")
TRACE_MSG(CAFSMS_1132_112_2_17_23_1_27_77,"Recv func stop!")
TRACE_MSG(CAFSMS_1139_112_2_17_23_1_27_78,"Recv func resume!")
TRACE_MSG(CAFSMS_1198_112_2_17_23_1_27_79,"SIM GetSmsNum max = %d, used = %d")
TRACE_MSG(CAFSMS_1210_112_2_17_23_1_27_80,"ME GetSmsNum max = %d, used = %d")
TRACE_MSG(CAFSMS_1229_112_2_17_23_1_27_81,"SIM GetSmsNum2 max = %d, used = %d")
TRACE_MSG(CAFSMS_1240_112_2_17_23_1_27_82,"ME GetSmsNum2 max = %d, used = %d")
TRACE_MSG(CAFSMS1_268_112_2_17_23_1_28_83,"[app]:applet start")
TRACE_MSG(CAFSMS1_449_112_2_17_23_1_28_84,"[app]:createinstance")
TRACE_MSG(CAFSMS1_457_112_2_17_23_1_28_85,"[app]:createinstance success")
TRACE_MSG(CAFSMS1_463_112_2_17_23_1_28_86,"create tapi failure!\r\n")
TRACE_MSG(CAFSMS1_467_112_2_17_23_1_28_87,"create heap failure!\r\n")
TRACE_MSG(CAFSMS1_517_112_2_17_23_1_28_88,"receive sms id = %d, cause = %d, dual_sys = %d,storage = %d")
TRACE_MSG(CAFSMS1_518_112_2_17_23_1_28_89,"number len = %d, number = %s")
TRACE_MSG(CAFSMS1_519_112_2_17_23_1_28_90,"content len = %d, content = %s")
TRACE_MSG(CAFSMS1_685_112_2_17_23_1_29_91,"read sms error!")
TRACE_MSG(CAFSMS1_723_112_2_17_23_1_29_92,"write sms error!")
TRACE_MSG(CAFSMS1_736_112_2_17_23_1_29_93,"SmsCenterAddr len =%d, party_num =%s")
TRACE_MSG(CAFSMS1_775_112_2_17_23_1_29_94,"send sms error!")
TRACE_MSG(CAFSMS1_786_112_2_17_23_1_29_95,"recv func error!")
TRACE_MSG(CAFSMS1_796_112_2_17_23_1_29_96,"SetSmsReceiveFunc cause =%d, is_receive_sms =%d")
TRACE_MSG(CAFSMS1_799_112_2_17_23_1_29_97,"SetSmsReceiveFunc ok")
TRACE_MSG(CAFSMS1_811_112_2_17_23_1_29_98,"send sms ok")
TRACE_MSG(CAFSMS1_815_112_2_17_23_1_29_99,"send sms failure")
TRACE_MSG(CAFSMS1_839_112_2_17_23_1_29_100,"SmsCenterAddr len =%d, party_num =%s")
TRACE_MSG(CAFSMS1_890_112_2_17_23_1_29_101,"read sms error!")
TRACE_MSG(CAFSMS1_902_112_2_17_23_1_29_102,"delete sms id is %d")
TRACE_MSG(CAFSMS1_1184_112_2_17_23_1_30_103,"Recv func stop!")
TRACE_MSG(CAFSMS1_1191_112_2_17_23_1_30_104,"Recv func resume!")
TRACE_MSG(CAFSMS1_1201_112_2_17_23_1_30_105,"SetSmsReceiveFunc cause =%d, is_receive_sms =%d")
TRACE_MSG(CAFSMS1_1204_112_2_17_23_1_30_106,"SetSmsReceiveFunc ok")
TRACE_MSG(CAFSMS1_1273_112_2_17_23_1_30_107,"write sms error!")
TRACE_MSG(CAFSMS1_1332_112_2_17_23_1_30_108,"SmsCenterAddr len =%d, party_num =%s")
TRACE_MSG(CAFSMS1_1445_112_2_17_23_1_30_109,"SIM GetSmsNum max = %d, used = %d")
TRACE_MSG(CAFSMS1_1454_112_2_17_23_1_30_110,"ME GetSmsNum max = %d, used = %d")
TRACE_MSG(CHECKBOX_250_112_2_17_23_1_32_111,"[app]:createinstance")
TRACE_MSG(CHECKBOX_258_112_2_17_23_1_32_112,"[app]:createinstance success")
TRACE_MSG(CHECKBOXCTRL_89_112_2_17_23_1_34_113,"[app]:createinstance")
TRACE_MSG(DROPDOWNLISTSAMPLE_125_112_2_17_23_1_36_114,"[app]:createinstance")
TRACE_MSG(DROPDOWNLISTSAMPLE_133_112_2_17_23_1_37_115,"[app]:createinstance success")
TRACE_MSG(DROPDOWNLISTSAMPLE_284_112_2_17_23_1_37_116,"Test-dropdownlist:IGUIDROPDOWNLIST_AddRef:2:%d\r\n")
TRACE_MSG(DROPDOWNLISTSAMPLE_291_112_2_17_23_1_37_117,"Test-dropdownlist:IGUIDROPDOWNLIST_Release:1:%d\r\n")
TRACE_MSG(DROPDOWNLISTSAMPLE_299_112_2_17_23_1_37_118,"Test-dropdownlist:IGUIDROPDOWNLIST_GetCtrlGuid:%d\r\n")
TRACE_MSG(DROPDOWNLISTSAMPLE_307_112_2_17_23_1_37_119,"Test-dropdownlist:IGUIDROPDOWNLIST_GetCtrlHandle:%d\r\n")
TRACE_MSG(DROPDOWNLISTSAMPLE_459_112_2_17_23_1_37_120,"TestCase_1:IGUIDROPDOWNLIST_AddRef:2:%d\r\n")
TRACE_MSG(DROPDOWNLISTSAMPLE_466_112_2_17_23_1_37_121,"TestCase_1:IGUIDROPDOWNLIST_Release:1:%d\r\n")
TRACE_MSG(DROPDOWNLISTSAMPLE_474_112_2_17_23_1_37_122,"TestCase_1:IGUIDROPDOWNLIST_GetCtrlGuid:%d\r\n")
TRACE_MSG(DROPDOWNLISTSAMPLE_482_112_2_17_23_1_37_123,"TestCase_1:IGUIDROPDOWNLIST_GetCtrlHandle:%d\r\n")
TRACE_MSG(EDITSAMPLE_116_112_2_17_23_1_39_124,"[app]:createinstance")
TRACE_MSG(EDITSAMPLE_124_112_2_17_23_1_39_125,"[app]:createinstance success")
TRACE_MSG(EDITSAMPLE_305_112_2_17_23_1_40_126,"Test-edit:IGUIEDIT_AddRef:2:%d\r\n")
TRACE_MSG(EDITSAMPLE_312_112_2_17_23_1_40_127,"Test-edit:IGUIEDIT_Release:1:%d\r\n")
TRACE_MSG(EDITSAMPLE_320_112_2_17_23_1_40_128,"Test-edit:IGUIEDIT_GetCtrlGuid:%d\r\n")
TRACE_MSG(EDITSAMPLE_328_112_2_17_23_1_40_129,"Test-edit:IGUIEDIT_GetCtrlHandle:%d\r\n")
TRACE_MSG(FILESYSSAMPLE_132_112_2_17_23_1_42_130,"[app]:createinstance")
TRACE_MSG(FILESYSSAMPLE_140_112_2_17_23_1_42_131,"[app]:createinstance success")
TRACE_MSG(FILESYSSAMPLE_356_112_2_17_23_1_42_132,"Test-Filesys:IFILE_AddRef:2:%d\r\n")
TRACE_MSG(FILESYSSAMPLE_363_112_2_17_23_1_42_133,"Test-Filesys:IFILE_Release:1:%d\r\n")
TRACE_MSG(FILESYSSAMPLE_381_112_2_17_23_1_42_134,"Test-Filesys:IFILE_GetDiskFreeSpace:%d:%d\r\n")
TRACE_MSG(FILESYSSAMPLE_396_112_2_17_23_1_42_135,"Test-Filesys:IFILE_GetDiskUsedSpace:%d:%d\r\n")
TRACE_MSG(FILESYSSAMPLE_606_112_2_17_23_1_43_136,"TestCase_1:ifile_ptr=%d\r\n")
TRACE_MSG(FILESYSSAMPLE_612_112_2_17_23_1_43_137,"TestCase_1:IFILE_AddRef=%d\r\n")
TRACE_MSG(FILESYSSAMPLE_619_112_2_17_23_1_43_138,"TestCase_1:IFILE_Release=%d\r\n")
TRACE_MSG(FILESYSSAMPLE_637_112_2_17_23_1_43_139,"TestCase_1:IFILE_GetDiskFreeSpace:low_word=%d;high_word=%d\r\n")
TRACE_MSG(FILESYSSAMPLE_652_112_2_17_23_1_43_140,"TestCase_1:IFILE_GetDiskUsedSpace:low_word=%d;high_word=%d\r\n")
TRACE_MSG(ICONLISTSAMPLE_118_112_2_17_23_1_45_141,"[app]:createinstance")
TRACE_MSG(ICONLISTSAMPLE_126_112_2_17_23_1_45_142,"[app]:createinstance success")
TRACE_MSG(ICONLISTSAMPLE_271_112_2_17_23_1_45_143,"Test-iconlist:IGUIICONLIST_AddRef:2:%d\r\n")
TRACE_MSG(ICONLISTSAMPLE_278_112_2_17_23_1_45_144,"Test-iconlist:IGUIICONLIST_Release:1:%d\r\n")
TRACE_MSG(ICONLISTSAMPLE_286_112_2_17_23_1_45_145,"Test-iconlist:IGUIICONLIST_GetCtrlGuid:%d\r\n")
TRACE_MSG(ICONLISTSAMPLE_294_112_2_17_23_1_46_146,"Test-iconlist:IGUIICONLIST_GetCtrlHandle:%d\r\n")
TRACE_MSG(IDC_103_112_2_17_23_1_48_147,"[app]:applet start")
TRACE_MSG(IDC_198_112_2_17_23_1_48_148,"[app]:createinstance")
TRACE_MSG(IDC_206_112_2_17_23_1_48_149,"[app]:createinstance success")
TRACE_MSG(IDC_292_112_2_17_23_1_48_150,"old font is %d \n ")
TRACE_MSG(IDC_297_112_2_17_23_1_48_151,"font_new_type  is %d \n ")
TRACE_MSG(IDC_300_112_2_17_23_1_48_152,"after set font fornt type  is %d \n ")
TRACE_MSG(IDC_312_112_2_17_23_1_48_153,"font_old_effect is %d \n ")
TRACE_MSG(IDC_316_112_2_17_23_1_48_154,"font_new_effect  is %d \n ")
TRACE_MSG(IDC_320_112_2_17_23_1_48_155,"after set font effect is %d \n ")
TRACE_MSG(IDC_332_112_2_17_23_1_48_156,"font_old_color is %d \n ")
TRACE_MSG(IDC_337_112_2_17_23_1_48_157,"font_new_color  is %d \n ")
TRACE_MSG(IDC_341_112_2_17_23_1_48_158,"after set font color is %d \n ")
TRACE_MSG(IDC_356_112_2_17_23_1_48_159,"font_aling is %d \n ")
TRACE_MSG(IDC_383_112_2_17_23_1_48_160,"pen_color is %d \n ")
TRACE_MSG(IDC_385_112_2_17_23_1_48_161,"pen_size is %d \n ")
TRACE_MSG(IDC_393_112_2_17_23_1_48_162,"test_point_color is %d \n ")
TRACE_MSG(IDC_552_112_2_17_23_1_49_163,"IDC_GetFontWidth is %d \n ")
TRACE_MSG(IDC_555_112_2_17_23_1_49_164,"IDC_GetFontWidth is %d \n ")
TRACE_MSG(IDC_561_112_2_17_23_1_49_165,"IDC_GetStringWidth is %d \n ")
TRACE_MSG(IDC_564_112_2_17_23_1_49_166,"IDC_GetStringHeight is %d \n ")
TRACE_MSG(IMAGESAMPLE_126_112_2_17_23_1_50_167,"[app]:createinstance")
TRACE_MSG(IMAGESAMPLE_134_112_2_17_23_1_50_168,"[app]:createinstance success")
TRACE_MSG(IMAGESAMPLE_239_112_2_17_23_1_50_169,"Test-IMAGE:IIMAGE_AddRef:2:%d\r\n")
TRACE_MSG(IMAGESAMPLE_246_112_2_17_23_1_50_170,"Test-IMAGE:IIMAGE_Release:1:%d\r\n")
TRACE_MSG(IMAGESAMPLE_341_112_2_17_23_1_51_171,"TestCase_1:IIMAGE_AddRef:2:%d\r\n")
TRACE_MSG(IMAGESAMPLE_348_112_2_17_23_1_51_172,"TestCase_1:IIMAGE_Release:1:%d\r\n")
TRACE_MSG(ISHELL_103_112_2_17_23_1_52_173,"[app]:applet start")
TRACE_MSG(ISHELL_197_112_2_17_23_1_53_174,"[app]:createinstance")
TRACE_MSG(ISHELL_205_112_2_17_23_1_53_175,"[app]:createinstance success")
TRACE_MSG(ISHELL_294_112_2_17_23_1_53_176,"screen_info: width  is  %d ;  high is %d \n ")
TRACE_MSG(ISHELL_312_112_2_17_23_1_53_177,"disk d  Memory   info free_high is   %d ;  free_low is %d ;used_high is %d ; used_low is %d  n ")
TRACE_MSG(ITHREAD_103_112_2_17_23_1_54_178,"[app]:applet start")
TRACE_MSG(ITHREAD_130_112_2_17_23_1_54_179,"HandleEvent MSG_ITHREAD_TEST1 data is : %s!\n ")
TRACE_MSG(ITHREAD_210_112_2_17_23_1_54_180,"[app]:createinstance")
TRACE_MSG(ITHREAD_218_112_2_17_23_1_54_181,"[app]:createinstance success")
TRACE_MSG(ITHREAD_240_112_2_17_23_1_54_182,"thread_test fun : 1")
TRACE_MSG(ITHREAD_242_112_2_17_23_1_54_183,"thread_test fun : 2")
TRACE_MSG(ITHREAD_251_112_2_17_23_1_54_184,"thread_test fun : 3")
TRACE_MSG(ITHREAD_253_112_2_17_23_1_54_185,"thread_test fun : 4")
TRACE_MSG(ITHREAD_260_112_2_17_23_1_54_186,"thread_test fun : 5")
TRACE_MSG(ITHREAD_262_112_2_17_23_1_55_187,"thread_test fun : 6")
TRACE_MSG(ITHREAD_270_112_2_17_23_1_55_188,"thread_test fun : 7")
TRACE_MSG(ITHREAD_273_112_2_17_23_1_55_189,"thread_test fun : 8")
TRACE_MSG(ITHREAD_289_112_2_17_23_1_55_190,"ITHREAD_test_Proc fun : 1")
TRACE_MSG(ITHREAD_293_112_2_17_23_1_55_191,"ITHREAD_test_Proc fun : 2")
TRACE_MSG(ITHREAD_297_112_2_17_23_1_55_192,"ITHREAD_test_Proc fun : 3")
TRACE_MSG(ITHREAD_303_112_2_17_23_1_55_193,"ITHREAD_test_Proc  get SIG_ITHREAD_TEST1 data is : %s!\n ")
TRACE_MSG(ITHREAD_308_112_2_17_23_1_55_194,"ITHREAD_test_Proc : get unknow msg !\n ")
TRACE_MSG(LABELSAMPLE_129_112_2_17_23_1_56_195,"[app]:createinstance")
TRACE_MSG(LABELSAMPLE_137_112_2_17_23_1_56_196,"[app]:createinstance success")
TRACE_MSG(LABELSAMPLE_283_112_2_17_23_1_56_197,"Test-label:IGUILABEL_AddRef:2:%d\r\n")
TRACE_MSG(LABELSAMPLE_290_112_2_17_23_1_56_198,"Test-label:IGUILABEL_Release:1:%d\r\n")
TRACE_MSG(LABELSAMPLE_298_112_2_17_23_1_56_199,"Test-label:IGUILABEL_GetCtrlGuid:%d\r\n")
TRACE_MSG(LABELSAMPLE_306_112_2_17_23_1_56_200,"Test-label:IGUILABEL_GetCtrlHandle:%d\r\n")
TRACE_MSG(LABELSAMPLE_470_112_2_17_23_1_57_201,"TestCase_1:IGUILABEL_AddRef:2:%d\r\n")
TRACE_MSG(LABELSAMPLE_477_112_2_17_23_1_57_202,"TestCase_1:IGUILABEL_Release:1:%d\r\n")
TRACE_MSG(LABELSAMPLE_485_112_2_17_23_1_57_203,"TestCase_1:IGUILABEL_GetCtrlGuid:%d\r\n")
TRACE_MSG(LABELSAMPLE_493_112_2_17_23_1_57_204,"TestCase_1:IGUILABEL_GetCtrlHandle:%d\r\n")
TRACE_MSG(LISTSAMPLE_125_112_2_17_23_1_59_205,"[app]:createinstance")
TRACE_MSG(LISTSAMPLE_133_112_2_17_23_1_59_206,"[app]:createinstance success")
TRACE_MSG(LISTSAMPLE_302_112_2_17_23_1_59_207,"Test-list:IGUILIST_AddRef:2:%d\r\n")
TRACE_MSG(LISTSAMPLE_309_112_2_17_23_1_59_208,"Test-list:IGUILIST_Release:1:%d\r\n")
TRACE_MSG(LISTSAMPLE_317_112_2_17_23_1_59_209,"Test-list:IGUILIST_GetCtrlGuid:%d\r\n")
TRACE_MSG(LISTSAMPLE_325_112_2_17_23_1_59_210,"Test-list:IGUILIST_GetCtrlHandle:%d\r\n")
TRACE_MSG(LISTSAMPLE_551_112_2_17_23_2_0_211,"Test-list:IGUILIST_GetCurItemIndex:%d\r\n")
TRACE_MSG(LISTSAMPLE_559_112_2_17_23_2_0_212,"Test-list:IGUILIST_GetTotalItemNum:%d\r\n")
TRACE_MSG(LISTSAMPLE_579_112_2_17_23_2_0_213,"TestCase_1:IGUILIST_AddRef:2:%d\r\n")
TRACE_MSG(LISTSAMPLE_586_112_2_17_23_2_0_214,"TestCase_1:IGUILIST_Release:1:%d\r\n")
TRACE_MSG(LISTSAMPLE_594_112_2_17_23_2_0_215,"TestCase_1:IGUILIST_GetCtrlGuid:%d\r\n")
TRACE_MSG(LISTSAMPLE_602_112_2_17_23_2_0_216,"TestCase_1:IGUILIST_GetCtrlHandle:%d\r\n")
TRACE_MSG(LISTSAMPLE_901_112_2_17_23_2_0_217,"Test-list:IGUILIST_GetCurItemIndex:%d\r\n")
TRACE_MSG(LISTSAMPLE_909_112_2_17_23_2_0_218,"Test-list:IGUILIST_GetTotalItemNum:%d\r\n")
TRACE_MSG(MEDIATEST_150_112_2_17_23_2_5_219,"[app]:applet start")
TRACE_MSG(MEDIATEST_164_112_2_17_23_2_5_220,"[app]:ISHELL_CreateInstance - media fail! ")
TRACE_MSG(MEDIATEST_229_112_2_17_23_2_5_221,"[app]:createinstance")
TRACE_MSG(MEDIATEST_237_112_2_17_23_2_5_222,"[app]:createinstance success")
TRACE_MSG(MEDIATEST_297_112_2_17_23_2_5_223,"[app]:IMEDIA_SetMediaData -  fail! ")
TRACE_MSG(MEDIATEST_313_112_2_17_23_2_5_224,"[app]:>>>>>>IMEDIA_control -  %d! ")
TRACE_MSG(MEDIATEST_408_112_2_17_23_2_6_225,"[app]:IMEDIA_SetMediaData -  fail! ")
TRACE_MSG(MEDIATEST_431_112_2_17_23_2_6_226,"[app]:>>>>>>IMEDIA_control -  %d! ")
TRACE_MSG(MEDIATEST_521_112_2_17_23_2_6_227,"[app]:IMEDIA_SetMediaData -  fail! ")
TRACE_MSG(MEDIATEST_537_112_2_17_23_2_6_228,"[app]:>>>>>>IMEDIA_control -  %d! ")
TRACE_MSG(MEDIATEST_631_112_2_17_23_2_6_229,"[app]:IMEDIA_SetMediaData -  fail! ")
TRACE_MSG(MEDIATEST_647_112_2_17_23_2_6_230,"[app]:>>>>>>IMEDIA_control -  %d! ")
TRACE_MSG(POPMENUSAMPLE_85_112_2_17_23_2_8_231,"[app]:applet start")
TRACE_MSG(POPMENUSAMPLE_192_112_2_17_23_2_8_232,"[app]:createinstance")
TRACE_MSG(POPMENUSAMPLE_200_112_2_17_23_2_8_233,"[app]:createinstance success")
TRACE_MSG(POPMENUSAMPLE_249_112_2_17_23_2_8_234,"node_id = %d\r\n")
TRACE_MSG(ABC_227_112_2_17_23_2_9_235,"[socket]:applet start")
TRACE_MSG(ABC_270_112_2_17_23_2_9_236,"[testclient]:createinstance")
TRACE_MSG(ABC_278_112_2_17_23_2_9_237,"[socket]:createinstance success")
TRACE_MSG(ABC_282_112_2_17_23_2_9_238,"create heap failure!\r\n")
TRACE_MSG(ABC_286_112_2_17_23_2_9_239,"create tapi failure!\r\n")
TRACE_MSG(ABC_290_112_2_17_23_2_9_240,"dial failure!\r\n")
TRACE_MSG(ABC_673_112_2_17_23_2_10_241,"HandleAsyncliWindow msg id is %lx\r\n")
TRACE_MSG(ABC_695_112_2_17_23_2_10_242,"create netmgr failure!\r\n")
TRACE_MSG(ABC_708_112_2_17_23_2_10_243,"domain address error!\r\n")
TRACE_MSG(ABC_716_112_2_17_23_2_10_244,"address:%s\r\n")
TRACE_MSG(ABC_753_112_2_17_23_2_10_245,"Address %x  Port %x\r\n")
TRACE_MSG(ABC_781_112_2_17_23_2_10_246,"MSG_ASYN_SOCKET_CLOSE_IND, server disconnect")
TRACE_MSG(ABC_796_112_2_17_23_2_10_247,"MSG_USER_START, user exit")
TRACE_MSG(ABC_915_112_2_17_23_2_10_248,"ITestSocket_TCLI_ColloquyCase state =%d")
TRACE_MSG(ABC_964_112_2_17_23_2_10_249,"rev %s\r\n")
TRACE_MSG(ABC_977_112_2_17_23_2_10_250,"send %s\r\n")
TRACE_MSG(ABC_1009_112_2_17_23_2_10_251,"rev %s\r\n")
TRACE_MSG(ABC_1023_112_2_17_23_2_10_252,"send %s\r\n")
TRACE_MSG(CAFMODULE_GEN_206_112_2_17_23_2_12_253,"[app]:applet end")
TRACE_MSG(TESTSOCKET_72_112_2_17_23_2_12_254,"ITestSocket_TestClient Start state =%d")
TRACE_MSG(TESTSOCKET_81_112_2_17_23_2_12_255,"create netmgr failure!\r\n")
TRACE_MSG(TESTSOCKET_93_112_2_17_23_2_12_256,"domain address error!\r\n")
TRACE_MSG(TESTSOCKET_101_112_2_17_23_2_12_257,"address:%s\r\n")
TRACE_MSG(TESTSOCKET_123_112_2_17_23_2_12_258,"Address %x  Port %x\r\n")
TRACE_MSG(TESTSOCKET_128_112_2_17_23_2_12_259,"Connect failure!\r\n")
TRACE_MSG(TESTSOCKET_131_112_2_17_23_2_12_260,"socket connect ok")
TRACE_MSG(TESTSOCKET_172_112_2_17_23_2_12_261,"select error!\r\n")
TRACE_MSG(TESTSOCKET_201_112_2_17_23_2_12_262,"rev %s\r\n")
TRACE_MSG(TESTSOCKET_202_112_2_17_23_2_12_263,"send %s\r\n")
TRACE_MSG(TESTSOCKET_219_112_2_17_23_2_12_264,"ITestSocket_TCLI_ColloquyCase state =%d")
TRACE_MSG(TESTSOCKET_329_112_2_17_23_2_13_265,"create tapi failure!\r\n")
TRACE_MSG(TESTSOCKET_339_112_2_17_23_2_13_266,"domain address error!\r\n")
TRACE_MSG(TESTSOCKET_347_112_2_17_23_2_13_267,"address:%s\r\n")
TRACE_MSG(TESTSOCKET_367_112_2_17_23_2_13_268,"Address %x  Port %x\r\n")
TRACE_MSG(TESTSOCKET_372_112_2_17_23_2_13_269,"Bind failure!\r\n")
TRACE_MSG(TESTSOCKET_378_112_2_17_23_2_13_270,"listen error!\r\n")
TRACE_MSG(TESTSOCKET_398_112_2_17_23_2_13_271,"accept error!\r\n")
TRACE_MSG(TESTSOCKET_421_112_2_17_23_2_13_272,"select error!\r\n")
TRACE_MSG(TESTSOCKET_455_112_2_17_23_2_13_273,"rev %s\r\n")
TRACE_MSG(TESTSOCKET_456_112_2_17_23_2_13_274,"send %s\r\n")
TRACE_MSG(TESTSOCKET_531_112_2_17_23_2_13_275,"create tapi failure!\r\n")
TRACE_MSG(TESTSOCKET_541_112_2_17_23_2_13_276,"domain address error!\r\n")
TRACE_MSG(TESTSOCKET_549_112_2_17_23_2_13_277,"address:%s\r\n")
TRACE_MSG(TESTSOCKET_562_112_2_17_23_2_13_278,"Address %x  addr str %s\r\n")
TRACE_MSG(TESTSOCKET_567_112_2_17_23_2_13_279,"Address %x  Port %x\r\n")
TRACE_MSG(TESTSOCKET_572_112_2_17_23_2_13_280,"Connect failure!\r\n")
TRACE_MSG(TESTSOCKET_611_112_2_17_23_2_13_281,"select error!\r\n")
TRACE_MSG(TESTSOCKET_761_112_2_17_23_2_13_282,"create tapi failure!\r\n")
TRACE_MSG(TESTSOCKET_771_112_2_17_23_2_13_283,"domain address error!\r\n")
TRACE_MSG(TESTSOCKET_779_112_2_17_23_2_13_284,"address:%s\r\n")
TRACE_MSG(TESTSOCKET_792_112_2_17_23_2_13_285,"Address %x  Port %x\r\n")
TRACE_MSG(TESTSOCKET_797_112_2_17_23_2_13_286,"Bind failure!\r\n")
TRACE_MSG(TESTSOCKET_980_112_2_17_23_2_14_287,"CAF_MEMCMP result is %d")
TRACE_MSG(TESTSOCKET_983_112_2_17_23_2_14_288,"CAF_STRCMP result is %d")
TRACE_MSG(TESTSOCKET_986_112_2_17_23_2_14_289,"CAF_STRNCMP result is %d")
TRACE_MSG(TESTSOCKET_989_112_2_17_23_2_14_290,"CAF_STRCOLL result is %d")
TRACE_MSG(TESTSOCKET_992_112_2_17_23_2_14_291,"CAF_STRXFRM result is %s, size is %d")
TRACE_MSG(TESTSOCKET_998_112_2_17_23_2_14_292,"CAF_ATOI result is %d")
TRACE_MSG(TESTSOCKET_1005_112_2_17_23_2_14_293,"CAF_WSTRTOSTR result is %s")
TRACE_MSG(TESTSOCKET_1022_112_2_17_23_2_14_294,"CAF_ALLOC CAF_STRCPY is %s")
TRACE_MSG(TESTSOCKET_1027_112_2_17_23_2_14_295,"Address %x  addr str %s\r\n")
TRACE_MSG(SOUNDTEST_119_112_2_17_23_2_15_296,"[app]:applet start")
TRACE_MSG(SOUNDTEST_132_112_2_17_23_2_15_297,"[app]:ISHELL_CreateInstance - sound fail! ")
TRACE_MSG(SOUNDTEST_234_112_2_17_23_2_16_298,"[app]:createinstance")
TRACE_MSG(SOUNDTEST_242_112_2_17_23_2_16_299,"[app]:createinstance success")
TRACE_MSG(TESTCONTROL_113_112_2_17_23_2_17_300,"[app]:applet start")
TRACE_MSG(TESTCONTROL_263_112_2_17_23_2_18_301,"[app]:createinstance")
TRACE_MSG(TESTCONTROL_271_112_2_17_23_2_18_302,"[app]:createinstance success")
TRACE_MSG(TESTDLL_111_112_2_17_23_2_19_303,"[app]:applet start")
TRACE_MSG(TESTDLL_275_112_2_17_23_2_19_304,"[app]:createinstance")
TRACE_MSG(TESTDLL_283_112_2_17_23_2_19_305,"[app]:createinstance success")
TRACE_MSG(TEXTBOXSAMPLE_125_112_2_17_23_2_21_306,"[app]:createinstance")
TRACE_MSG(TEXTBOXSAMPLE_133_112_2_17_23_2_21_307,"[app]:createinstance success")
TRACE_MSG(TEXTBOXSAMPLE_284_112_2_17_23_2_21_308,"Test-textbox:IGUITEXTBOX_AddRef:2:%d\r\n")
TRACE_MSG(TEXTBOXSAMPLE_291_112_2_17_23_2_21_309,"Test-textbox:IGUITEXTBOX_Release:1:%d\r\n")
TRACE_MSG(TEXTBOXSAMPLE_299_112_2_17_23_2_21_310,"Test-textbox:IGUITEXTBOX_GetCtrlGuid:%d\r\n")
TRACE_MSG(TEXTBOXSAMPLE_307_112_2_17_23_2_21_311,"Test-textbox:IGUITEXTBOX_GetCtrlHandle:%d\r\n")
TRACE_MSG(TEXTBOXSAMPLE_470_112_2_17_23_2_22_312,"TestCase_1:IGUITEXTBOX_AddRef:2:%d\r\n")
TRACE_MSG(TEXTBOXSAMPLE_477_112_2_17_23_2_22_313,"TestCase_1:IGUITEXTBOX_Release:1:%d\r\n")
TRACE_MSG(TEXTBOXSAMPLE_485_112_2_17_23_2_22_314,"TestCase_1:IGUITEXTBOX_GetCtrlGuid:%d\r\n")
TRACE_MSG(TEXTBOXSAMPLE_493_112_2_17_23_2_22_315,"TestCase_1:IGUITEXTBOX_GetCtrlHandle:%d\r\n")
END_TRACE_MAP(CAF_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _CAF_TRC_H_

