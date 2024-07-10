/******************************************************************************
 ** File Name:      mmi_app_ia_java_trc.h                                         *
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
//trace_id:182
#ifndef _MMI_APP_IA_JAVA_TRC_H_
#define _MMI_APP_IA_JAVA_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define AUDIOCLIENT_342_112_2_18_2_21_58_0 "AudioClient_player_notify() entry, handle=0x%x, param=0x%x"
#define AUDIOCLIENT_350_112_2_18_2_21_58_1 "AudioClient_player_notify(), report_ptr->report=%d"
#define AUDIOCLIENT_370_112_2_18_2_21_58_2 "AudioClient_player_notify() entry, handle=0x%x, param=0x%x"
#define AUDIOCLIENT_378_112_2_18_2_21_58_3 "AudioClient_tone_notify(), report_ptr->report=%d"
#define AUDIOCLIENT_909_112_2_18_2_21_59_4 "java nCurPlayTime ac->_usecL %d"
#define JAS_GFX_FONT_711_112_2_18_2_22_1_5 "java jas_gfx_font jas_GfxGetCharCount end, loop = %d time=%d"
#define MMIJAVA_FUNC_1574_112_2_18_2_22_5_6 " MMIJAVA_Start_initializeResource leave"
#define MMIJAVA_FUNC_2078_112_2_18_2_22_6_7 "MMIDEFAULT_FlashBackLight:input param is invalid"
#define MMIJAVA_FUNC_2108_112_2_18_2_22_6_8 "MMIDEFAULT_FlashBackLight:create timer failed"
#define MMIJAVA_FUNC_2381_112_2_18_2_22_6_9 " MMIJAVA_Exit_FinalizeResource leave"
#define MMIJAVA_FUNC_2397_112_2_18_2_22_6_10 "MMIAPIJAVA_SendSignal sig_id = %x"
#define MMIJAVA_FUNC_2453_112_2_18_2_22_6_11 "IsJavaPort() port is %d"
#define MMIJAVA_FUNC_2459_112_2_18_2_22_6_12 "IsJavaPort() port is %d, cb is %d"
#define MMIJAVA_WINTAB_1251_112_2_18_2_22_10_13 "MMIJAVA_AddSuiteToMainmenu enter suite id=%d,device_type =%d"
#define MMIJAVA_WINTAB_1262_112_2_18_2_22_10_14 "MMIJAVA_AddSuiteToMainmenu suiteList_ptr ALLOC FAILED"
#define MMIJAVA_WINTAB_1270_112_2_18_2_22_10_15 "MMIJAVA_AddSuiteToMainmenu ajms_ab_getSuiteList FAILED"
#define MMIJAVA_WINTAB_1291_112_2_18_2_22_10_16 "MMIJAVA_AddSuiteToMainmenu data_info_ptr failed"
#define MMIJAVA_WINTAB_1297_112_2_18_2_22_10_17 "Test_Add_Java_item get the first midlet icons_suiteList"
#define MMIJAVA_WINTAB_1345_112_2_18_2_22_10_18 "MMIJAVA_DeleteSuiteRecordInMainmenu suite_id = %d"
#define MMIJAVA_WINTAB_1360_112_2_18_2_22_10_19 "MMIJAVA_DeleteSuiteRecordInMainmenu suiteList_ptr ALLOC FAILED"
#define MMIJAVA_WINTAB_1368_112_2_18_2_22_10_20 "MMIJAVA_DeleteSuiteRecordInMainmenu ajms_ab_getSuiteList FAILED"
#define MMIJAVA_WINTAB_1411_112_2_18_2_22_10_21 "MMIJAVA_OpenJAVAFromMainMenu:param1_ptr g_active_suite_id=%s,%d"
#define MMIJAVA_WINTAB_1420_112_2_18_2_22_10_22 "MMIJAVA_OpenJAVAFromMainMenu iNumOfSuite= %d"
#define MMIJAVA_WINTAB_1444_112_2_18_2_22_10_23 "MMIJAVA_OpenJAVAFromMainMenu suiteList_ptr ALLOC FAILED"
#define MMIJAVA_WINTAB_1460_112_2_18_2_22_11_24 "MMIJAVA_OpenJAVAFromMainMenu ajms_ab_getSuiteList FAILED"
#define MMIJAVA_WINTAB_1471_112_2_18_2_22_11_25 "MMIJAVA_OpenJAVAFromMainMenu run_suite_index= %d"
#define MMIJAVA_WINTAB_1477_112_2_18_2_22_11_26 "MMIJAVA_OpenJAVAFromMainMenu active_suite_index = %d"
#define MMIJAVA_WINTAB_1495_112_2_18_2_22_11_27 "MMIJAVA_OpenJAVAFromMainMenu ajms_ab_getSuiteList FAILED"
#define MMIJAVA_WINTAB_1523_112_2_18_2_22_11_28 "MMIJAVA_OpenJAVAFromMainMenu active_suite_index < 0"
#define MMIJAVA_WINTAB_1552_112_2_18_2_22_11_29 "MMIJAVA_OpenJAVAFromMainMenu prompt_content_ptr alloc FAILED"
#define MMIJAVA_WINTAB_1622_112_2_18_2_22_11_30 "MMIJAVA_DeleteSuiteFromMainmenu:param1_ptr =%s"
#define MMIJAVA_WINTAB_1632_112_2_18_2_22_11_31 "MMIJAVA_DeleteSuiteFromMainmenu iNumOfSuite= %d"
#define MMIJAVA_WINTAB_1656_112_2_18_2_22_11_32 "MMIJAVA_DeleteSuiteFromMainmenu suiteList_ptr ALLOC FAILED"
#define MMIJAVA_WINTAB_1672_112_2_18_2_22_11_33 "MMIJAVA_DeleteSuiteFromMainmenu ajms_ab_getSuiteList FAILED"
#define MMIJAVA_WINTAB_1682_112_2_18_2_22_11_34 "MMIJAVA_DeleteSuiteFromMainmenu suite_index= %d"
#define MMIJAVA_WINTAB_1697_112_2_18_2_22_11_35 "MMIJAVA_DeleteSuiteFromMainmenu suite not exist"
#define MMIJAVA_WINTAB_1715_112_2_18_2_22_11_36 "MMIJAVA_DeleteSuiteFromMainmenu suite AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED"
#define MMIJAVA_WINTAB_1775_112_2_18_2_22_11_37 "JAVA_Install_ErrorHandle otaCode=%d"
#define MMIJAVA_WINTAB_2020_112_2_18_2_22_12_38 "SetSelectNetFormParam ALLOCA FAILED 2"
#define MMIJAVA_WINTAB_2140_112_2_18_2_22_12_39 "HandleJavaNetSelectWinMsg"
#define MMIJAVA_WINTAB_2362_112_2_18_2_22_12_40 "HandleSelectWinMsg msg_id = %x"
#define MMIJAVA_WINTAB_2390_112_2_18_2_22_12_41 "HandleSelectWinMsg len = %d"
#define MMIJAVA_WINTAB_2436_112_2_18_2_22_12_42 "HandleJavaAMSMidletListWinMsg, msg_id = 0x%x"
#define MMIJAVA_WINTAB_2453_112_2_18_2_22_13_43 "HandleJavaAMSMidletListWinMsg GUILIST_GetCurItemIndex %d"
#define MMIJAVA_WINTAB_2503_112_2_18_2_22_13_44 "HandleJavaAMSWinMsg, msg_id = 0x%02x"
#define MMIJAVA_WINTAB_2511_112_2_18_2_22_13_45 "HandleJavaAMSWinMsg, s_suiteList ALLOC failed"
#define MMIJAVA_WINTAB_2532_112_2_18_2_22_13_46 "HandleJavaAMSWinMsg cursuitid = %d"
#define MMIJAVA_WINTAB_2584_112_2_18_2_22_13_47 "HandleJavaAMSWinMsg cursuitid = %d"
#define MMIJAVA_WINTAB_2647_112_2_18_2_22_13_48 "HandleJavaAMSWinMsg cursuitid = %d"
#define MMIJAVA_WINTAB_2678_112_2_18_2_22_13_49 "HandleJavaDownLoadEnter Enter type =%d"
#define MMIJAVA_WINTAB_2714_112_2_18_2_22_13_50 "HandleJavaDownLoadEnter exit"
#define MMIJAVA_WINTAB_2743_112_2_18_2_22_13_51 "MMIJAVA_ResetSystem enter!"
#define MMIJAVA_WINTAB_2748_112_2_18_2_22_13_52 "path_sytem_e_nor error = %d"
#define MMIJAVA_WINTAB_2751_112_2_18_2_22_13_53 "path_sytem_f_nor error = %d"
#define MMIJAVA_WINTAB_2758_112_2_18_2_22_13_54 "path_sytem_f error = %d"
#define MMIJAVA_WINTAB_2762_112_2_18_2_22_13_55 "path_sytem_e error = %d"
#define MMIJAVA_WINTAB_2765_112_2_18_2_22_13_56 "path_sytem_d error = %d"
#define MMIJAVA_WINTAB_2768_112_2_18_2_22_13_57 "path_sytem_c error = %d"
#define MMIJAVA_WINTAB_2775_112_2_18_2_22_13_58 "MMIJAVA_ResetSystem success!"
#define MMIJAVA_WINTAB_2793_112_2_18_2_22_13_59 "HandleJavaResetSystemPromptWinMsg,msg_id =%d"
#define MMIJAVA_WINTAB_2845_112_2_18_2_22_13_60 "HandleJavaDeletePromptWinMsg win_info_ptr == PNULL !"
#define MMIJAVA_WINTAB_2913_112_2_18_2_22_14_61 "HandleJavaAMSOptionWinMsg,msg_id =%d"
#define MMIJAVA_WINTAB_3019_112_2_18_2_22_14_62 "HandleJavaAMSOptionWinMsg,msg_id =%d"
#define MMIJAVA_WINTAB_3188_112_2_18_2_22_14_63 "AddOneDetailItem enter "
#define MMIJAVA_WINTAB_3218_112_2_18_2_22_14_64 "AddOneDetailItem fail "
#define MMIJAVA_WINTAB_3322_112_2_18_2_22_15_65 "SetJavaInfoDetail s_suiteList[cursuitid].domainId = %d"
#define MMIJAVA_WINTAB_3399_112_2_18_2_22_15_66 "enter MMIJAVA_CreateNetworkSetting, suitId= %d"
#define MMIJAVA_WINTAB_3406_112_2_18_2_22_15_67 "MMIJAVA_CreateNetworkSetting setting_file_ascii= %s"
#define MMIJAVA_WINTAB_3416_112_2_18_2_22_15_68 "MMIJAVA_CreateNetworkSetting setting_handle= %d"
#define MMIJAVA_WINTAB_3421_112_2_18_2_22_15_69 "MMIJAVA_CreateNetworkSetting bytes_written= %d"
#define MMIJAVA_WINTAB_3461_112_2_18_2_22_15_70 "MMIJAVA_GetNetSettingInfo file name = %s"
#define MMIJAVA_WINTAB_3464_112_2_18_2_22_15_71 "MMIJAVA_GetNetSettingInfo setting file handle = %d"
#define MMIJAVA_WINTAB_3501_112_2_18_2_22_15_72 "MMIJAVA_GetNetSettingIndex enter"
#define MMIJAVA_WINTAB_3515_112_2_18_2_22_15_73 "MMIJAVA_GetNetSettingIndex file name = %s"
#define MMIJAVA_WINTAB_3518_112_2_18_2_22_15_74 "MMIJAVA_GetNetSettingIndex setting file handle = %d"
#define MMIJAVA_WINTAB_3551_112_2_18_2_22_15_75 "MMIJAVA_SetNetSettingInfo setting file:%s"
#define MMIJAVA_WINTAB_3554_112_2_18_2_22_15_76 "MMIJAVA_SetNetSettingInfo setting file handle:%d"
#define MMIJAVA_WINTAB_3559_112_2_18_2_22_15_77 "MMIJAVA_SetNetSettingInfo bytes_written=%d"
#define MMIJAVA_WINTAB_3717_112_2_18_2_22_15_78 "InitPBFolder enter"
#define MMIJAVA_WINTAB_3743_112_2_18_2_22_15_79 "MMIAPIFMM_SetAttr result %d"
#define MMIJAVA_WINTAB_3748_112_2_18_2_22_15_80 "InitPBFolder result %d"
#define MMIJAVA_WINTAB_3782_112_2_18_2_22_15_81 "Java_LocalInstall_To current_item=%d"
#define MMIJAVA_WINTAB_3783_112_2_18_2_22_15_82 "Java_LocalInstall_To current_item=%d %d"
#define MMIJAVA_WINTAB_3784_112_2_18_2_22_15_83 "Java_LocalInstall_To jar_data_size = %d  cur_item=%d,disk_name=%d"
#define MMIJAVA_WINTAB_3794_112_2_18_2_22_16_84 "Java_LocalInstall_To drm = %s"
#define MMIJAVA_WINTAB_3836_112_2_18_2_22_16_85 "Java_LocalInstall_To  current_item=%d"
#define MMIJAVA_WINTAB_3874_112_2_18_2_22_16_86 "Java_LocalInstall_To url=%s"
#define MMIJAVA_WINTAB_3876_112_2_18_2_22_16_87 "Java_LocalInstall_To jadinstalltoPath = %s"
#define MMIJAVA_WINTAB_3964_112_2_18_2_22_16_88 "Java_LocalInstall_To TRUE"
#define MMIJAVA_WINTAB_3969_112_2_18_2_22_16_89 "Java_LocalInstall_To FLASE result = %d"
#define MMIJAVA_WINTAB_3990_112_2_18_2_22_16_90 "HandleJavaSelectInstallDiskWinMsg, msg_id = 0x%02x"
#define MMIJAVA_WINTAB_4030_112_2_18_2_22_16_91 "HandleJavaSelectInstallDiskWinMsg cursuitid = %d"
#define MMIJAVA_WINTAB_4040_112_2_18_2_22_16_92 "HandleJavaSelectInstallDiskWinMsg cursuitid = %d"
#define MMIJAVA_WINTAB_4078_112_2_18_2_22_16_93 "HandleInstallInfoWinMsg msg_id =%x "
#define MMIJAVA_WINTAB_4121_112_2_18_2_22_16_94 "HandleInstallInfoWinMsg jar_len = %d"
#define MMIJAVA_WINTAB_4241_112_2_18_2_22_16_95 "mmijava_wintab.c, OpenJarFile() enter!"
#define MMIJAVA_WINTAB_4251_112_2_18_2_22_16_96 "mmijava_wintab.c, OpenJarFile(), FALSE 1"
#define MMIJAVA_WINTAB_4256_112_2_18_2_22_16_97 "mmijava_wintab.c, OpenJarFile(), file_len = %d"
#define MMIJAVA_WINTAB_4261_112_2_18_2_22_16_98 "mmijava_wintab.c, OpenJarFile(), FALSE 2"
#define MMIJAVA_WINTAB_4269_112_2_18_2_22_16_99 "mmijava_wintab.c, OpenJarFile(), FALSE 2"
#define MMIJAVA_WINTAB_4279_112_2_18_2_22_16_100 "OpenJarFile file_info_ptr->file_content_ptr = %x"
#define MMIJAVA_WINTAB_4283_112_2_18_2_22_16_101 "mmijava_wintab.c, OpenJarFile(), FALSE 3"
#define MMIJAVA_WINTAB_4304_112_2_18_2_22_17_102 "mmijava_wintab.c, OpenJarFile(), FALSE 4, error = %d"
#define MMIJAVA_WINTAB_4311_112_2_18_2_22_17_103 "mmijava_wintab.c, OpenJarFile(), TRUE 5"
#define MMIJAVA_WINTAB_4376_112_2_18_2_22_17_104 "HandleCheckWinMsg msg_id = %x, "
#define MMIJAVA_WINTAB_4502_112_2_18_2_22_17_105 "HandleLocalCheckResult s_local_check_result.oldId = %d"
#define MMIJAVA_WINTAB_4534_112_2_18_2_22_17_106 "HandleLocalCheckResult otacode = %d"
#define MMIJAVA_WINTAB_4608_112_2_18_2_22_17_107 "\"\"\"\"\" MMIJAVA_jblendia_start\"\"\"\"\""
#define MMIJAVA_WINTAB_4631_112_2_18_2_22_17_108 "HandleJavaSubWinMsg launch error"
#define MMIJAVA_WINTAB_4651_112_2_18_2_22_17_109 "HandleJavaSubWinMsg launch error"
#define MMIJAVA_WINTAB_4671_112_2_18_2_22_17_110 "HandleJavaSubWinMsg MMIJAVA_jwe_run ret =%d"
#define MMIJAVA_WINTAB_4686_112_2_18_2_22_17_111 "HandleJavaSubWinMsg MMIJAVA_jwe_run leave"
#define MMIJAVA_WINTAB_4710_112_2_18_2_22_17_112 "\"\"\"\"\" tck test start \"\"\"\"\""
#define MMIJAVA_WINTAB_4718_112_2_18_2_22_17_113 "\"\"\"\"\"\"\"\"# tck test url: %s"
#define MMIJAVA_WINTAB_4739_112_2_18_2_22_17_114 "HandleJavaSubWinMsg launch error"
#define MMIJAVA_WINTAB_4756_112_2_18_2_22_17_115 "HandleJavaSubWinMsg launch error"
#define MMIJAVA_WINTAB_4786_112_2_18_2_22_17_116 "MMIJAVA_VirtualKeyboardTPToKJKey:active_id = %x"
#define MMIJAVA_WINTAB_4856_112_2_18_2_22_18_117 "MMIJAVA_VirtualKeyboardTPToKJKey:key_type,key_code =%d,%d"
#define MMIJAVA_WINTAB_4914_112_2_18_2_22_18_118 "rect(%d,%d,%d,%d)"
#define MMIJAVA_WINTAB_5040_112_2_18_2_22_18_119 "JVM has been runing...."
#define MMIJAVA_WINTAB_5046_112_2_18_2_22_18_120 "JVM start step1,try to allocate memory ahead"
#define MMIJAVA_WINTAB_5051_112_2_18_2_22_18_121 "JVM start step1,allocate failed, exit"
#define MMIJAVA_WINTAB_5086_112_2_18_2_22_18_122 "native key(red) is 0x%x"
#define MMIJAVA_WINTAB_5211_112_2_18_2_22_18_123 "native key(no red) is 0x%x"
#define MMIJAVA_WINTAB_5248_112_2_18_2_22_18_124 "HandleJavaSubWinMsg this is no acordding key code"
#define MMIJAVA_WINTAB_5270_112_2_18_2_22_18_125 "HandleJavaSubWinMsg  -- MSG_CLOSE_WINDOW!"
#define MMIJAVA_WINTAB_5291_112_2_18_2_22_18_126 "HandleJavaSubWinMsg  -- MSG_LOSE_FOCUS!"
#define MMIJAVA_WINTAB_5334_112_2_18_2_22_19_127 "HandleJavaSubWinMsg  -- MSG_GET_FOCUS!"
#define MMIJAVA_WINTAB_5499_112_2_18_2_22_19_128 "CreateInstallDiskListBox "
#define MMIJAVA_WINTAB_5504_112_2_18_2_22_19_129 "CreateInstallDiskListBox C"
#define MMIJAVA_WINTAB_5586_112_2_18_2_22_19_130 "ACJ->ajms_ab_getSutieListCount return applet = %d"
#define MMIJAVA_WINTAB_5602_112_2_18_2_22_19_131 "no midlet or more size"
#define MMIJAVA_WINTAB_5625_112_2_18_2_22_19_132 "--error: could not get the installed midlet list."
#define MMIJAVA_WINTAB_5629_112_2_18_2_22_19_133 "CreateSuitListBox s_preinstall_id = %d"
#define MMIJAVA_WINTAB_5637_112_2_18_2_22_19_134 "CreateSuitListBox j = %d"
#define MMIJAVA_WINTAB_5756_112_2_18_2_22_19_135 "DrawListItemWithIconData item_index:%d, content_index: %d, icon size:%d"
#define MMIJAVA_WINTAB_5762_112_2_18_2_22_19_136 "DrawListItemWithIconData data_info_ptr  failed"
#define MMIJAVA_WINTAB_5772_112_2_18_2_22_19_137 "DrawListItemWithIconData get the first midlet icon"
#define MMIJAVA_WINTAB_5850_112_2_18_2_22_20_138 "CreateMidletListBox suit id is %d,"
#define MMIJAVA_WINTAB_5855_112_2_18_2_22_20_139 "-- number of midlet(s) is %d."
#define MMIJAVA_WINTAB_5887_112_2_18_2_22_20_140 "--error: failed to get midlet list."
#define MMIJAVA_WINTAB_5960_112_2_18_2_22_20_141 "jblendia_setMidletWinTitle iNumOfSuite = %d"
#define MMIJAVA_WINTAB_5964_112_2_18_2_22_20_142 "jblendia_setMidletWinTitle iNumOfSuite < 0"
#define MMIJAVA_WINTAB_5971_112_2_18_2_22_20_143 "jblendia_setMidletWinTitle suiteList_ptr ALLOC FAILED"
#define MMIJAVA_WINTAB_5979_112_2_18_2_22_20_144 "jblendia_setMidletWinTitle ajms_ab_getSuiteList FAILED"
#define MMIJAVA_WINTAB_6021_112_2_18_2_22_20_145 "C:\\delrpt.txt not exist"
#define MMIJAVA_WINTAB_6026_112_2_18_2_22_20_146 "jblendia_send_delete_report file name = %s"
#define MMIJAVA_WINTAB_6029_112_2_18_2_22_20_147 "jblendia_send_delete_report setting file handle = %d"
#define MMIJAVA_WINTAB_6439_112_2_18_2_22_21_148 "JavaDeleteSuite jadSize = %d"
#define MMIJAVA_WINTAB_6448_112_2_18_2_22_21_149 "delete notify failed, can't get jad file, return %d.\n"
#define MMIJAVA_WINTAB_6470_112_2_18_2_22_21_150 "delete notify failed, can't get property \"MIDlet-Delete-Notify\", return %d.\n"
#define MMIJAVA_WINTAB_6484_112_2_18_2_22_21_151 "JavaDeleteSuite flag = %d"
#define MMIJAVA_WINTAB_6612_112_2_18_2_22_21_152 "HandleJavaPermitSetWinMsg: msg_id = %d"
#define MMIJAVA_WINTAB_6694_112_2_18_2_22_21_153 "HandleJavaPermitOptWinMsg: msg_id = %d"
#define MMIJAVA_WINTAB_6702_112_2_18_2_22_21_154 "cur_group = %d"
#define MMIJAVA_WINTAB_6788_112_2_18_2_22_21_155 "HandleJavaPermitOptWinMsg:result =%d"
#define MMIJAVA_WINTAB_7238_112_2_18_2_22_22_156 "MMIAPIJAVA_StartLocalInstall(): jad"
#define MMIJAVA_WINTAB_7416_112_2_18_2_22_23_157 "MMIAPIJAVA_JavaGameDirectStart(): jar"
#define MMIJAVA_WINTAB_7439_112_2_18_2_22_23_158 "MMIAPIJAVA_StartLocalInstall(): OPEN jar error"
#define MMIJAVA_WINTAB_7479_112_2_18_2_22_23_159 "MMIAPIJAVA_StartLocalInstall(): OPEN jar error"
#define MMIJAVA_WINTAB_7499_112_2_18_2_22_23_160 "s_install_data_info_ptr alloc failed!"
#define MMIJAVA_WINTAB_7532_112_2_18_2_22_23_161 "jar_len=%d"
#define MMIJAVA_WINTAB_7533_112_2_18_2_22_23_162 "jad_len=%d"
#define MMIJAVA_WINTAB_7534_112_2_18_2_22_23_163 "jar_file_path=%d"
#define MMIJAVA_WINTAB_7600_112_2_18_2_22_23_164 "StartLocalInstall(): out!"
#define MMIJAVA_WINTAB_7611_112_2_18_2_22_23_165 "MMIJAVA_ReturnToApplicationMenu ENTER"
#define MMIJAVA_WINTAB_7626_112_2_18_2_22_23_166 "PauseDialg_OnBTN, btn_type = %d"
#define MMIJAVA_WINTAB_7676_112_2_18_2_22_23_167 "HandleJavaPauseDialgWinMsg, msg_id = 0x%x"
#define MMIJAVA_WINTAB_7696_112_2_18_2_22_23_168 "HandleJavaPauseDialgWinMsg GUILIST_GetCurItemIndex %d"
#define MMIJAVA_WINTAB_7755_112_2_18_2_22_23_169 "ResumeDialg_OnBTN, btn_type = %d"
#define MMIJAVA_WINTAB_7797_112_2_18_2_22_23_170 "HandleJavaResumeDialgWinMsg, msg_id = 0x%x"
#define MMIJAVA_WINTAB_7816_112_2_18_2_22_24_171 "HandleJavaAMSMidletListWinMsg GUILIST_GetCurItemIndex %d"
#define MMIJAVA_WINTAB_7870_112_2_18_2_22_24_172 "mmijava_wintab.c InstallJadDataReader, size = %d"
#define MMIJAVA_WINTAB_7903_112_2_18_2_22_24_173 "mmijava_wintab.c InstallJarDataReader, size = %d"
#define MMIJAVA_WINTAB_7915_112_2_18_2_22_24_174 "mmijava_wintab.c InstallJarDataReader, readcount = %d"
#define MMIJAVA_WINTAB_7940_112_2_18_2_22_24_175 "point 0"
#define MMIJAVA_WINTAB_7953_112_2_18_2_22_24_176 "point 1 %d"
#define MMIJAVA_WINTAB_7964_112_2_18_2_22_24_177 "point 2 %d"
#define MMIJAVA_WINTAB_7972_112_2_18_2_22_24_178 "point 3"
#define MMIJAVA_WINTAB_8035_112_2_18_2_22_24_179 "LocalCheck data_size = %d,data_addr = %x"
#define MMIJAVA_WINTAB_8074_112_2_18_2_22_24_180 "Java file type is jad"
#define MMIJAVA_WINTAB_8079_112_2_18_2_22_24_181 "Java file type is jar"
#define MMIJAVA_WINTAB_8119_112_2_18_2_22_24_182 "LocalCheck result = %d"
#define MMIJAVA_WINTAB_8123_112_2_18_2_22_24_183 "LocalCheck TRUE"
#define MMIJAVA_WINTAB_8128_112_2_18_2_22_24_184 "LocalCheck FALSE"
#define MMIJAVA_WINTAB_8147_112_2_18_2_22_24_185 "Java_LocalCheckCallBack step = %d,total = %d"
#define MMIJAVA_WINTAB_8155_112_2_18_2_22_24_186 "LocalCheck Success"
#define MMIJAVA_WINTAB_8160_112_2_18_2_22_24_187 "LocalCheck Waiting...."
#define MMIJAVA_WINTAB_8171_112_2_18_2_22_24_188 "MMIJAVA_LocalCheck Fail"
#define MMIJAVA_WINTAB_8198_112_2_18_2_22_24_189 "Java_LocalInstall jar_data_size = %d"
#define MMIJAVA_WINTAB_8240_112_2_18_2_22_24_190 "Java_LocalInstall TRUE"
#define MMIJAVA_WINTAB_8245_112_2_18_2_22_24_191 "Java_LocalInstall FLASE result = %d"
#define MMIJAVA_WINTAB_8267_112_2_18_2_22_24_192 "MMIJAVA_LaunchMidletDirectAfterInstallOk install_result.suiteId = %d"
#define MMIJAVA_WINTAB_8325_112_2_18_2_22_25_193 "HandleInstallWinMsg msg_id =%x "
#define MMIJAVA_WINTAB_8342_112_2_18_2_22_25_194 "Java_LocalInstall jar_len = %d"
#define MMIJAVA_WINTAB_8343_112_2_18_2_22_25_195 "s_local_check_result minimumInstallSize = %d"
#define MMIJAVA_WINTAB_8364_112_2_18_2_22_25_196 "Java_LocalInstall Fail"
#define MMIJAVA_WINTAB_8380_112_2_18_2_22_25_197 "HandleInstallWinMsg s_install_data_info_ptr = %x, s_install_data_info_ptr->is_ota = %d"
#define MMIJAVA_WINTAB_8383_112_2_18_2_22_25_198 "HandleInstallWinMsg s_local_check_result.installNotifyUrl = %s"
#define MMIJAVA_WINTAB_8418_112_2_18_2_22_25_199 "s_local_check_result.oldId = %d"
#define MMIJAVA_WINTAB_8476_112_2_18_2_22_25_200 "Java_LocalInstall Fail"
#define MMIJAVA_WINTAB_8587_112_2_18_2_22_25_201 "HandleProcessWinMsg  msg_id %x"
#define MMIJAVA_WINTAB_8632_112_2_18_2_22_25_202 "Java_LocalInstallCallBack s_preinstall_id = %d, s_local_check_result.oldId = %d"
#define MMIJAVA_WINTAB_8637_112_2_18_2_22_25_203 "Java_LocalInstallCallBack s_preinstall_id = %d"
#define MMIJAVA_WINTAB_8680_112_2_18_2_22_25_204 "[param = %d], function success, read %d bytes data in buffer\n"
#define MMIJAVA_WINTAB_8696_112_2_18_2_22_25_205 "[param = %d], reading... %d bytes read\n"
#define MMIJAVA_WINTAB_8701_112_2_18_2_22_25_206 "[param = %d], writing... %d bytes write\n"
#define MMIJAVA_WINTAB_8706_112_2_18_2_22_25_207 "[param = %d], error occurred in download files, error value = %d\n"
#define MMIJAVA_WINTAB_8723_112_2_18_2_22_25_208 "[param = %d], Authentication failed, set name / password.\n"
#define MMIJAVA_WINTAB_8745_112_2_18_2_22_26_209 "jwe_ota_notify enter statusCode = %d, urlLength = %d "
#define MMIJAVA_WINTAB_8753_112_2_18_2_22_26_210 "jwe_ota_notify.., url = %s\n"
#define MMIJAVA_WINTAB_8756_112_2_18_2_22_26_211 "StatusCode[i].code: %d.\n"
#define MMIJAVA_WINTAB_8763_112_2_18_2_22_26_212 "t: %s.\n"
#define MMIJAVA_WINTAB_8766_112_2_18_2_22_26_213 "error: t==NULL.\n"
#define MMIJAVA_WINTAB_8773_112_2_18_2_22_26_214 "JARBuffer alloc failed"
#define MMIJAVA_WINTAB_8782_112_2_18_2_22_26_215 "ignoreBuf alloc failed"
#define MMIJAVA_WINTAB_8797_112_2_18_2_22_26_216 "error: ajsc_http_sendRequest.\n"
#define MMIJAVA_WINTAB_8818_112_2_18_2_22_26_217 "notify start.., status = %d.\n"
#define MMIJAVA_WINTAB_8932_112_2_18_2_22_26_218 "MMIAPIJAVA_OTAInstallDrm in param error null"
#define MMIJAVA_WINTAB_8939_112_2_18_2_22_26_219 "MMIAPIJAVA_OTAInstallDrm in param error len is 0"
#define MMIJAVA_WINTAB_8975_112_2_18_2_22_26_220 "MMIAPIJAVA_OTAInstallDrm s_install_data_info_ptr is not null"
#define MMIJAVA_WINTAB_9014_112_2_18_2_22_26_221 "MMIAPIJAVA_StartLocalInstall(): OPEN jar error"
#define MMIJAVA_WINTAB_9089_112_2_18_2_22_27_222 "MMIJAVA_OTADownloadFailHandle  downresult =%d, s_local_check_result.installNotifyUrl lenth = %d"
#define MMIJAVA_WINTAB_9176_112_2_18_2_22_27_223 "OTA_DELETE_JAD=%d OTA_DELETE_JAR=%d"
#define MMIJAVA_WINTAB_9189_112_2_18_2_22_27_224 "OTA_DELETE_JAD ret=%d"
#define MMIJAVA_WINTAB_9204_112_2_18_2_22_27_225 "OTA_JAR_FileName_ptr ret=%d"
#define MMIJAVA_WINTAB_9567_112_2_18_2_22_28_226 "MMIJAVA_IsJavaRuning result is %d"
#define MMIJAVA_WINTAB_9583_112_2_18_2_22_28_227 "MMIJAVA_OTAInstallJad_CallBackFunction enter "
#define MMIJAVA_WINTAB_9588_112_2_18_2_22_28_228 "MMIJAVA_OTAInstallJad_CallBackFunction,MMIAPIJAVA_IsJavaInstalling true exit "
#define MMIJAVA_WINTAB_9708_112_2_18_2_22_28_229 "MMIJAVA_OTAInstallJar_CallBackFunction,MMIAPIJAVA_IsJavaInstalling true exit "
#define MMIJAVA_WINTAB_9807_112_2_18_2_22_29_230 "MMIJAVA_OTAInstallJad  11"
#define MMIJAVA_WINTAB_10181_112_2_18_2_22_30_231 "hanker MMIAPIJAVA_OpenWapWin enter "
#define MMIJAVA_WINTAB_10185_112_2_18_2_22_30_232 "MMIAPIJAVA_OpenWapWin hanker enter s_local_check_result.url=%s"
#define MMIJAVA_WINTAB_10206_112_2_18_2_22_30_233 "MMIAPIJAVA_OpenWapWin hanker enter g_java_wap_info.url=%s"
#define MMIJAVA_WINTAB_10221_112_2_18_2_22_30_234 "MMIAPIJAVA_OpenWapWin url is null"
#define MMIJAVA_WINTAB_10266_112_2_18_2_22_30_235 "MMIJAVA_SuiteUpdate,suite_id =%d"
#define MMIJAVA_WINTAB_10271_112_2_18_2_22_30_236 "MMIJAVA_SuiteUpdate,erorcode =%d"
#define MMIJAVA_WINTAB_10276_112_2_18_2_22_30_237 "MMIJAVA_SuiteUpdate,jadurl =%s"
#define MMIJAVA_WINTAB_10325_112_2_18_2_22_30_238 "MMIJAVA_initPreinstallID return applet = %d"
#define MMIJAVA_WINTAB_10350_112_2_18_2_22_30_239 "MMIJAVA_initPreinstallID s_preinstall_id = %d"
#define MMIJAVA_WINTAB_10378_112_2_18_2_22_30_240 "MMIJAVA_initPreinstallID return applet = %d"
#define MMIJAVA_WINTAB_10574_112_2_18_2_22_31_241 "MMIAPIJAVA_IsJavaBackgroundRuning result is %d"
#define MMIJAVA_WINTAB_10591_112_2_18_2_22_31_242 "MMIJAVA_IsJavaRuning result is %d"
#define MMIJAVA_WINTAB_10704_112_2_18_2_22_31_243 "MMIJAVA_StartMIDletByNameVendor cursuitid = %d"
#define MMIJAVA_WINTAB_10729_112_2_18_2_22_31_244 "JavaGameDirectStart(): enter!"
#define MMIJAVA_WINTAB_10750_112_2_18_2_22_31_245 "JavaGameDirectStart(): jad"
#define MMIJAVA_WINTAB_10774_112_2_18_2_22_31_246 "JavaGameDirectStart(): jar"
#define MMIJAVA_WINTAB_10786_112_2_18_2_22_31_247 "JavaGameDirectStart: ignore this file"
#define MMIJAVA_WINTAB_10811_112_2_18_2_22_31_248 "JavaGameDirectStart(): out!"
#define MMIJAVA_WINTAB_10895_112_2_18_2_22_31_249 "MMIAPIJAVA_ResumeBackgroundJVM g_jvm_is_running_by_mmi =%d"
#define MMIJAVA_WINTAB_10904_112_2_18_2_22_31_250 "MMIAPIJAVA_ResumeBackgroundJVM ajms_arc_resumeApp fail!!!"
#define MMIJAVA_WINTAB_10909_112_2_18_2_22_31_251 "MMIAPIJAVA_ResumeBackgroundJVM do nothing exit"
#define MMIJAVA_WINTAB_10921_112_2_18_2_22_31_252 "MMIAPIJAVA_PauseBackgroundJVM g_jvm_is_running_by_mmi =%d"
#define MMIJAVA_WINTAB_10931_112_2_18_2_22_31_253 "MMIAPIJAVA_PauseBackgroundJVM ajms_arc_suspendApp fail!!!"
#define MMIJAVA_WINTAB_10936_112_2_18_2_22_31_254 "MMIAPIJAVA_PauseBackgroundJVM do nothing exit"
#define MMIJAVA_WINTAB_11039_112_2_18_2_22_32_255 "MMIAPIJAVA_PlatformRequest enter"
#define MMIJAVA_WINTAB_11043_112_2_18_2_22_32_256 "MMIAPIJAVA_PlatformRequest enter g_java_call_info_ptr = %s"
#define MMIJAVA_WINTAB_11058_112_2_18_2_22_32_257 "MMIAPIJAVA_PlatformRequest type = %d "
#define MMIJAVA_WINTAB_11184_112_2_18_2_22_32_258 "mmi JavaApplet_HandleEvent msg_id = 0x%04x"
#define MMIJAVA_WINTAB_11268_112_2_18_2_22_32_259 "MMIAPIJAVA_StartApplet"
#define MMIJAVA_WINTAB_11287_112_2_18_2_22_32_260 "HandleJavaAppletWinMsg:msg_id=%d"
#define JGURTOOLKIT_117_112_2_18_2_22_34_261 "commandmenuPaint"
#define JKPIM_22_112_2_18_2_22_37_262 "JkPIMGetListNames enter!"
#define JKPIM_23_112_2_18_2_22_37_263 "JkPIMGetListNames,enter"
#define JKPIM_38_112_2_18_2_22_37_264 "leave"
#define JKPIM_47_112_2_18_2_22_37_265 "JkPIMGetDefaultListName:enter"
#define JKPIM_61_112_2_18_2_22_37_266 "JkPIMGetDefaultListName,leave"
#define JKPIM_70_112_2_18_2_22_37_267 "JkPIMOpenConnection:enter"
#define JKPIM_84_112_2_18_2_22_37_268 "JkPIMOpenConnection,leave"
#define JKPIM_92_112_2_18_2_22_37_269 "JkPIMCloseConnection:enter"
#define JKPIM_105_112_2_18_2_22_37_270 "JkPIMCloseConnection,leave"
#define JKPIM_114_112_2_18_2_22_37_271 "JkPIMGetSupportedFields:enter"
#define JKPIM_127_112_2_18_2_22_37_272 "JkPIMGetSupportedFields,leave"
#define JKPIM_136_112_2_18_2_22_37_273 "JkPIMGetSupportedAttributes:enter"
#define JKPIM_149_112_2_18_2_22_37_274 "JkPIMGetSupportedAttributes,leave"
#define JKPIM_159_112_2_18_2_22_37_275 "JkPIMReadItem:enter"
#define JKPIM_173_112_2_18_2_22_37_276 "JkPIMReadItem,leave"
#define JKPIM_182_112_2_18_2_22_37_277 "JkPIMAddItem:enter"
#define JKPIM_196_112_2_18_2_22_38_278 "JkPIMAddItem,leave"
#define JKPIM_205_112_2_18_2_22_38_279 "JkPIMSetItem:enter"
#define JKPIM_219_112_2_18_2_22_38_280 "JkPIMSetItem,leave"
#define JKPIM_227_112_2_18_2_22_38_281 "JkPIMDeleteItem:enter"
#define JKPIM_241_112_2_18_2_22_38_282 "JkPIMDeleteItem,leave"
#define JKPIM_249_112_2_18_2_22_38_283 "JkPIMIsCategorySupported:enter"
#define JKPIM_263_112_2_18_2_22_38_284 "JkPIMIsCategorySupported,leave"
#define JKPIM_271_112_2_18_2_22_38_285 "JkPIMGetSupportedCategories:enter"
#define JKPIM_285_112_2_18_2_22_38_286 "JkPIMGetSupportedCategories,leave"
#define JKPIM_292_112_2_18_2_22_38_287 "JkPIMIsCategory:enter"
#define JKPIM_307_112_2_18_2_22_38_288 "JkPIMIsCategory,leave"
#define JKPIM_314_112_2_18_2_22_38_289 "JkPIMAddCategory:enter"
#define JKPIM_329_112_2_18_2_22_38_290 "JkPIMAddCategory,leave"
#define JKPIM_336_112_2_18_2_22_38_291 "JkPIMDeleteCategory:enter"
#define JKPIM_351_112_2_18_2_22_38_292 "JkPIMDeleteCategory,leave"
#define JKPIM_358_112_2_18_2_22_38_293 "JkPIMRenameCategory:enter"
#define JKPIM_373_112_2_18_2_22_38_294 "JkPIMRenameCategory,leave"
#define JKPIM_380_112_2_18_2_22_38_295 "JkPIMAddCategoryToItem:enter"
#define JKPIM_395_112_2_18_2_22_38_296 "JkPIMAddCategoryToItem,leave"
#define JKPIM_402_112_2_18_2_22_38_297 "JkPIMMoveToNextItem:enter"
#define JKPIM_417_112_2_18_2_22_38_298 "JkPIMMoveToNextItem,leave"
#define JKPIM_424_112_2_18_2_22_38_299 "JkPIMGetResultSetByAll:enter"
#define JKPIM_439_112_2_18_2_22_38_300 "JkPIMGetResultSetByAll,leave"
#define JKPIM_447_112_2_18_2_22_38_301 "JkPIMGetResultSetByItem:enter"
#define JKPIM_461_112_2_18_2_22_38_302 "JkPIMGetResultSetByAll,leave"
#define JKPIM_469_112_2_18_2_22_38_303 "JkPIMGetResultSetByString:enter"
#define JKPIM_483_112_2_18_2_22_38_304 "JkPIMGetResultSetByString,leave"
#define JKPIM_491_112_2_18_2_22_38_305 "JkPIMGetResultSetByCategory:enter"
#define JKPIM_505_112_2_18_2_22_38_306 "JkPIMGetResultSetByCategory,leave"
#define JKPIM_513_112_2_18_2_22_38_307 "JkPIMGetResultSetForEvent:enter"
#define JKPIM_522_112_2_18_2_22_38_308 "JkPIMGetResultSetForEvent,leave"
#define JKPIM_530_112_2_18_2_22_38_309 "JkPIMGetResultSetForToDoDate:enter"
#define JKPIM_539_112_2_18_2_22_38_310 "JkPIMGetResultSetForToDoDate,leave"
#define JKPIM_547_112_2_18_2_22_38_311 "JkPIMGetResultSetForToDoCalendar:enter"
#define JKPIM_556_112_2_18_2_22_39_312 "JkPIMGetResultSetForToDoCalendar,leave"
#define JKPIM_564_112_2_18_2_22_39_313 "JkPIMCloseResultSet:enter"
#define JKPIM_578_112_2_18_2_22_39_314 "JkPIMCloseResultSet,leave"
#define JKPIM_584_112_2_18_2_22_39_315 "JkPIMAllocPointer enter!"
#define JKPIM_590_112_2_18_2_22_39_316 "JkPIMFreePointer enter!"
#define JKPIM_601_112_2_18_2_22_39_317 "JkPIMGetSupportedSerialFormats:enter"
#define JKPIM_607_112_2_18_2_22_39_318 "JkPIMStartFromSerialFormat:enter"
#define JKPIM_613_112_2_18_2_22_39_319 "JkPIMEndFromSerialFormat:enter"
#define JKPIM_619_112_2_18_2_22_39_320 "JkPIMReceiveSerialByte:enter"
#define JKPIM_625_112_2_18_2_22_39_321 "JkPIMReceiveSerialBytes:enter"
#define JKPIM_631_112_2_18_2_22_39_322 "JkPIMGetSerialFormatHandlerClassName:enter"
#define JKPIM_637_112_2_18_2_22_39_323 "JkPIMExecuteToSerialFormat:enter"
#define JKPIM_643_112_2_18_2_22_39_324 "JkPIMExecuteFromSerialFormat:enter"
#define JKPIM_650_112_2_18_2_22_39_325 "JkPIMGetSupportedRepeatRuleFields:enter"
#define JKPIM_657_112_2_18_2_22_39_326 "JkPIMImportItem:enter"
#define JKSOFTKEY_115_112_2_18_2_22_39_327 "[JkSoftkeySetLabel] (label, length, location) = (0x%08X,%d,%d)"
#define JKSOFTKEY_207_112_2_18_2_22_39_328 "[JkSoftkeyResetLabels] enter."
#define JKSOFTKEY_223_112_2_18_2_22_39_329 "[JkSoftkeySetScrollState] (state) = (%d) "
#define JKSOFTKEY_242_112_2_18_2_22_39_330 "[JkSoftkeySetFocusState] (focusstate) = (%d) "
#define JKSOFTKEY_252_112_2_18_2_22_39_331 "[JkSoftkeyGetFocusState] focusState = (%d)"
#define JKSOFTKEY_274_112_2_18_2_22_39_332 "[JkSoftkeyPaint] enter."
#define JKSOFTKEY_398_112_2_18_2_22_40_333 "[JkVideoDrawPermissionDialog] (g, index, keyCode, titleStr, titleSize, messageStr, messageSize) (0x%08X, %d, %d, 0x%08X, %d, 0x%08X, %d)"
#define JKSOFTKEY_413_112_2_18_2_22_40_334 "[JkSoftkeyPaint-drawScrollArrow] enter."
#define JKUDP_307_112_2_18_2_22_41_335 "JkUdpConnect pclink set interface error"
#define JKVMBACKGROUND_21_112_2_18_2_22_42_336 "[JkSetVmBackground] enter vmId=%d"
#define JKVMBACKGROUND_25_112_2_18_2_22_42_337 "[JkSetVmForeground] enter vmId=%d"
#define JKVMBACKGROUND_31_112_2_18_2_22_42_338 "[JkGetVmBgFgState] JK_BG_VM_STATE_BACKGROUND"
#define JKVMBACKGROUND_35_112_2_18_2_22_42_339 "[JkGetVmBgFgState] JK_BG_VM_STATE_FOREGROUND"
#define JKVMBACKGROUND_44_112_2_18_2_22_42_340 "[JkNotifyPopupDialog] = message= %s "
#define JKVMBACKGROUND_51_112_2_18_2_22_43_341 "[JkIsVmOnRealScreen]"
#define JKVMTIMER_29_112_2_18_2_22_43_342 "[JkSetVmTimerHandler] handler!=null."
#define JKVMTIMER_36_112_2_18_2_22_43_343 "[JkSetVmTimerHandler] handler!==null."
#define JWE_83_112_2_18_2_23_4_344 "Ajava_Initiliztion_ia b_initializeJAVA = %d"
#define JWE_99_112_2_18_2_23_4_345 "Ajava_Initiliztion_ia yuyu log failed"
#define JWE_102_112_2_18_2_23_4_346 "Ajava_Initiliztion_ia yuyu log success"
#define JWE_175_112_2_18_2_23_4_347 "Ajava_Initiliztion_Special b_initializeJAVA = %d"
#define JWE_199_112_2_18_2_23_4_348 "IsDefaultJavaFolder delete suite.idx"
#define JWE_201_112_2_18_2_23_4_349 "Ajava_Initiliztion_Special ajms_ret = %d"
#define JWE_EIME_128_112_2_18_2_23_5_350 "MMIAPIJAVA_OpenTextEditWindow cur_inputmethod = %x"
#define KJAVA_ADV_FS_68_112_2_18_2_23_7_351 "kjava_fs_setAttr enter"
#define KJAVA_ADV_FS_695_112_2_18_2_23_8_352 "kjava_fs_setRootChangeNotification enter"
#define KJAVA_SYS_CORE_762_112_2_18_2_23_11_353 "first=%d,second =%d"
#define KJAVA_SYS_CORE_763_112_2_18_2_23_11_354 "kjava_core_allocateHugeMemory-------allocate memory size=%d"
#define KJAVA_SYS_CORE_778_112_2_18_2_23_11_355 "FREE ENTER"
#define KJAVA_SYS_CORE_781_112_2_18_2_23_11_356 "kjava_core_freeHugeMemory status = %d"
#define KJAVA_SYS_CORE_788_112_2_18_2_23_11_357 "kjava_core_freeHugeMemory2 status = %d"
#define KJAVA_SYS_CORE_799_112_2_18_2_23_11_358 "FREE OK"
#define KJAVA_SYS_CORE_814_112_2_18_2_23_11_359 "kjava_core_getProperty (key)=(%s)"
#define KJAVA_SYS_CORE_840_112_2_18_2_23_11_360 "kjava_core_getProperty return property.value=(%s)"
#define KJAVA_SYS_CORE_855_112_2_18_2_23_11_361 "kjava_core_getProperty return property=(%s)"
#define KJAVA_SYS_CORE_864_112_2_18_2_23_11_362 "kjava_core_getProperty return null"
#define KJAVA_SYS_CORE_890_112_2_18_2_23_11_363 "return string = %s."
#define KJAVA_SYS_CORE_946_112_2_18_2_23_11_364 "[kjava_core_getOption]22 (m_option[%d].option)=(%s)"
#define KJAVA_SYS_CORE_1114_112_2_18_2_23_12_365 "CB_Start_RequestConn call error, prev call has't ended"
#define KJAVA_SYS_CORE_1126_112_2_18_2_23_12_366 "CALL OK"
#define KJAVA_SYS_CORE_1135_112_2_18_2_23_12_367 "CB_Start_RequestConn call error, prev call has't ended"
#define KJAVA_SYS_CORE_1147_112_2_18_2_23_12_368 "CALL OK"
#define KJAVA_SYS_CORE_1151_112_2_18_2_23_12_369 "HTTP ENTER"
#define KJAVA_SYS_CORE_1162_112_2_18_2_23_12_370 "CB_Start_RequestConn para = %s"
#define KJAVA_SYS_CORE_1163_112_2_18_2_23_12_371 "CB_Start_RequestConn g_java_wap_info.url = %s"
#define KJAVA_SYS_CORE_1164_112_2_18_2_23_12_372 "HTTP OK"
#define KJAVA_SYS_GFX_151_112_2_18_2_23_18_373 "kjava_sys_gfx_loadCacheFont enter isDefaultFontCached=%d"
#define KJAVA_SYS_GFX_240_112_2_18_2_23_18_374 "JVM_UpdateGameCanvasScreenTimer Enter"
#define KJAVA_SYS_GFX_258_112_2_18_2_23_18_375 "JVM_UpdateGameCanvasScreenTimer AmNotifyRepaintEvent"
#define KJAVA_SYS_GFX_263_112_2_18_2_23_18_376 "JVM_UpdateGameCanvasScreenTimer Leave"
#define KJAVA_SYS_GFX_273_112_2_18_2_23_18_377 "JkVideoDrawOffScreenDirectly timerCount = %d, m_timeCount = %d"
#define KJAVA_SYS_GFX_291_112_2_18_2_23_18_378 "JkVideoDrawOffScreenDirectly Leave"
#define KJAVA_SYS_GFX_296_112_2_18_2_23_18_379 "JkVideoIsDrawOffScreenDirectlyPending Enter"
#define KJAVA_SYS_GFX_298_112_2_18_2_23_18_380 "JkVideoIsDrawOffScreenDirectlyPending Leave"
#define KJAVA_SYS_GFX_317_112_2_18_2_23_18_381 "java is not running,just return"
#define KJAVA_SYS_GFX_380_112_2_18_2_23_19_382 "MMIAPIJAVA_CreateWinPaintTimer %d"
#define KJAVA_SYS_GFX_413_112_2_18_2_23_19_383 "MMIJAVA_gfx_update enter  s_is_display_logo=%d  s_is_still_display_logo=%d"
#define KJAVA_SYS_GFX_417_112_2_18_2_23_19_384 "java is not running,just return"
#define KJAVA_SYS_GFX_473_112_2_18_2_23_19_385 "VIRTUAL_KEY_BOARD size = %d x %d"
#define KJAVA_SYS_GFX_480_112_2_18_2_23_19_386 "screen size = %d x %d"
#define KJAVA_SYS_GFX_486_112_2_18_2_23_19_387 "kjava_virtualScreenPixels alloc failed"
#define KJAVA_SYS_GFX_599_112_2_18_2_23_19_388 "kjava_gfx_fini enter"
#define KJAVA_SYS_GFX_686_112_2_18_2_23_19_389 "kjava_gfx_getSuitableFont size=%d style=%d face=%d defaultFont=0x%x"
#define KJAVA_SYS_GFX_1671_112_2_18_2_23_21_390 "[kjava_gfx_imageAnalyze] (srcBuf, srcSize, width, height) = (0x%08X, %d, %d, %d)"
#define KJAVA_SYS_GFX_1747_112_2_18_2_23_21_391 "kjava_gfx_imageDecode,(srcBuf, srcSize, argbData, dataLength, processAlpha) = (0x%08X, %d, 0x%08X, %d, 0x%08X)"
#define KJAVA_SYS_GFX_1758_112_2_18_2_23_21_392 "kjava_gfx_imageDecode type == png"
#define KJAVA_SYS_GFX_1763_112_2_18_2_23_21_393 "kjava_gfx_imageDecode alphaData = NULL SCI_ALLOC_APP failed!!!!OutOfMemory!"
#define KJAVA_SYS_GFX_1783_112_2_18_2_23_21_394 "kjava_gfx_imageDecode time start = %d"
#define KJAVA_SYS_GFX_1794_112_2_18_2_23_21_395 "kjava_gfx_imageDecode type == png, is_handle_alpha = %d"
#define KJAVA_SYS_GFX_1800_112_2_18_2_23_21_396 "kjava_gfx_imageDecode time end = %d"
#define KJAVA_SYS_GFX_1805_112_2_18_2_23_21_397 "kjava_gfx_imageDecode failed"
#define KJAVA_SYS_LOG_OUT_49_112_2_18_2_23_23_398 "%s"
#define KJAVA_SYS_SOCK_330_112_2_18_2_23_26_399 "kjava_sock_connect pclink set interface is wrong"
#define KJAVA_SYS_SOCK_1444_112_2_18_2_23_29_400 "[MMIJAVA]:MMIJAVAHandlePDPMsg msg_ptr->result=%d"
#define KJAVA_SYS_SOCK_1460_112_2_18_2_23_29_401 "MMIJAVAHandlePDPMsg: MMIPDP_DEACTIVE_IND"
#define KJAVA_SYS_WMA_166_112_2_18_2_23_29_402 "kjava_wma_open port = %d"
#define KJAVA_SYS_WMA_189_112_2_18_2_23_29_403 "kjava_wma_open gen port true"
#define KJAVA_SYS_WMA_295_112_2_18_2_23_30_404 "IAJavaSMSSendCnfCallback !"
#define KJAVA_SYS_WMA_302_112_2_18_2_23_30_405 "java:IAJavaSMSSendCnfCallback java_cb = PNULL"
#define KJAVA_SYS_WMA_366_112_2_18_2_23_30_406 "kjava_wma_sendMessage i = %d"
#define KJAVA_SYS_WMA_370_112_2_18_2_23_30_407 "kjava_wma_sendMessage message->url = %s,message->urlLength = %d"
#define KJAVA_SYS_WMA_374_112_2_18_2_23_30_408 "kjava_wma_sendMessage parse url 0"
#define KJAVA_SYS_WMA_388_112_2_18_2_23_30_409 "kjava_wma_sendMessage pcAddr = %s"
#define KJAVA_SYS_WMA_430_112_2_18_2_23_30_410 "sms_makeAndSendMessage s_connList[i].port = %d"
#define KJAVA_SYS_WMA_431_112_2_18_2_23_30_411 "kjava_wma_sendMessage sendRet = %d"
#define KJAVA_SYS_WMA_536_112_2_18_2_23_30_412 "AppsmsCallback i = %d, event = %d, argv->dstPort = %d"
#define KJAVA_SYS_WMA_552_112_2_18_2_23_30_413 "AppsmsCallback mode is KJAVA_WMA_CONNECTION_MODE_CLIENT"
#define KJAVA_SYS_WMA_607_112_2_18_2_23_30_414 "Add2SMSReceiveQue Data type MN_SMS_8_BIT_ALPHBET"
#define KJAVA_SYS_WMA_616_112_2_18_2_23_30_415 "Add2SMSReceiveQue Data type MN_SMS_UCS2_ALPHABET"
#define KJAVA_SYS_WMA_785_112_2_18_2_23_31_416 "MMISMS_RegisterCallback()"
#define KJAVA_SYS_WMA_787_112_2_18_2_23_31_417 "MMISMS_RegisterCallback() cb is %d, port is %d"
#define KJAVA_SYS_WMA_816_112_2_18_2_23_31_418 "MMISMS_UnRegisterCallback() port is %d"
#define KJAVA_SYS_WMA_827_112_2_18_2_23_31_419 "MMISMS_UnRegisterCallback() port found and clear"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_IA_JAVA_TRC)
TRACE_MSG(AUDIOCLIENT_342_112_2_18_2_21_58_0,"AudioClient_player_notify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(AUDIOCLIENT_350_112_2_18_2_21_58_1,"AudioClient_player_notify(), report_ptr->report=%d")
TRACE_MSG(AUDIOCLIENT_370_112_2_18_2_21_58_2,"AudioClient_player_notify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(AUDIOCLIENT_378_112_2_18_2_21_58_3,"AudioClient_tone_notify(), report_ptr->report=%d")
TRACE_MSG(AUDIOCLIENT_909_112_2_18_2_21_59_4,"java nCurPlayTime ac->_usecL %d")
TRACE_MSG(JAS_GFX_FONT_711_112_2_18_2_22_1_5,"java jas_gfx_font jas_GfxGetCharCount end, loop = %d time=%d")
TRACE_MSG(MMIJAVA_FUNC_1574_112_2_18_2_22_5_6," MMIJAVA_Start_initializeResource leave")
TRACE_MSG(MMIJAVA_FUNC_2078_112_2_18_2_22_6_7,"MMIDEFAULT_FlashBackLight:input param is invalid")
TRACE_MSG(MMIJAVA_FUNC_2108_112_2_18_2_22_6_8,"MMIDEFAULT_FlashBackLight:create timer failed")
TRACE_MSG(MMIJAVA_FUNC_2381_112_2_18_2_22_6_9," MMIJAVA_Exit_FinalizeResource leave")
TRACE_MSG(MMIJAVA_FUNC_2397_112_2_18_2_22_6_10,"MMIAPIJAVA_SendSignal sig_id = %x")
TRACE_MSG(MMIJAVA_FUNC_2453_112_2_18_2_22_6_11,"IsJavaPort() port is %d")
TRACE_MSG(MMIJAVA_FUNC_2459_112_2_18_2_22_6_12,"IsJavaPort() port is %d, cb is %d")
TRACE_MSG(MMIJAVA_WINTAB_1251_112_2_18_2_22_10_13,"MMIJAVA_AddSuiteToMainmenu enter suite id=%d,device_type =%d")
TRACE_MSG(MMIJAVA_WINTAB_1262_112_2_18_2_22_10_14,"MMIJAVA_AddSuiteToMainmenu suiteList_ptr ALLOC FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1270_112_2_18_2_22_10_15,"MMIJAVA_AddSuiteToMainmenu ajms_ab_getSuiteList FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1291_112_2_18_2_22_10_16,"MMIJAVA_AddSuiteToMainmenu data_info_ptr failed")
TRACE_MSG(MMIJAVA_WINTAB_1297_112_2_18_2_22_10_17,"Test_Add_Java_item get the first midlet icons_suiteList")
TRACE_MSG(MMIJAVA_WINTAB_1345_112_2_18_2_22_10_18,"MMIJAVA_DeleteSuiteRecordInMainmenu suite_id = %d")
TRACE_MSG(MMIJAVA_WINTAB_1360_112_2_18_2_22_10_19,"MMIJAVA_DeleteSuiteRecordInMainmenu suiteList_ptr ALLOC FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1368_112_2_18_2_22_10_20,"MMIJAVA_DeleteSuiteRecordInMainmenu ajms_ab_getSuiteList FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1411_112_2_18_2_22_10_21,"MMIJAVA_OpenJAVAFromMainMenu:param1_ptr g_active_suite_id=%s,%d")
TRACE_MSG(MMIJAVA_WINTAB_1420_112_2_18_2_22_10_22,"MMIJAVA_OpenJAVAFromMainMenu iNumOfSuite= %d")
TRACE_MSG(MMIJAVA_WINTAB_1444_112_2_18_2_22_10_23,"MMIJAVA_OpenJAVAFromMainMenu suiteList_ptr ALLOC FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1460_112_2_18_2_22_11_24,"MMIJAVA_OpenJAVAFromMainMenu ajms_ab_getSuiteList FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1471_112_2_18_2_22_11_25,"MMIJAVA_OpenJAVAFromMainMenu run_suite_index= %d")
TRACE_MSG(MMIJAVA_WINTAB_1477_112_2_18_2_22_11_26,"MMIJAVA_OpenJAVAFromMainMenu active_suite_index = %d")
TRACE_MSG(MMIJAVA_WINTAB_1495_112_2_18_2_22_11_27,"MMIJAVA_OpenJAVAFromMainMenu ajms_ab_getSuiteList FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1523_112_2_18_2_22_11_28,"MMIJAVA_OpenJAVAFromMainMenu active_suite_index < 0")
TRACE_MSG(MMIJAVA_WINTAB_1552_112_2_18_2_22_11_29,"MMIJAVA_OpenJAVAFromMainMenu prompt_content_ptr alloc FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1622_112_2_18_2_22_11_30,"MMIJAVA_DeleteSuiteFromMainmenu:param1_ptr =%s")
TRACE_MSG(MMIJAVA_WINTAB_1632_112_2_18_2_22_11_31,"MMIJAVA_DeleteSuiteFromMainmenu iNumOfSuite= %d")
TRACE_MSG(MMIJAVA_WINTAB_1656_112_2_18_2_22_11_32,"MMIJAVA_DeleteSuiteFromMainmenu suiteList_ptr ALLOC FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1672_112_2_18_2_22_11_33,"MMIJAVA_DeleteSuiteFromMainmenu ajms_ab_getSuiteList FAILED")
TRACE_MSG(MMIJAVA_WINTAB_1682_112_2_18_2_22_11_34,"MMIJAVA_DeleteSuiteFromMainmenu suite_index= %d")
TRACE_MSG(MMIJAVA_WINTAB_1697_112_2_18_2_22_11_35,"MMIJAVA_DeleteSuiteFromMainmenu suite not exist")
TRACE_MSG(MMIJAVA_WINTAB_1715_112_2_18_2_22_11_36,"MMIJAVA_DeleteSuiteFromMainmenu suite AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED")
TRACE_MSG(MMIJAVA_WINTAB_1775_112_2_18_2_22_11_37,"JAVA_Install_ErrorHandle otaCode=%d")
TRACE_MSG(MMIJAVA_WINTAB_2020_112_2_18_2_22_12_38,"SetSelectNetFormParam ALLOCA FAILED 2")
TRACE_MSG(MMIJAVA_WINTAB_2140_112_2_18_2_22_12_39,"HandleJavaNetSelectWinMsg")
TRACE_MSG(MMIJAVA_WINTAB_2362_112_2_18_2_22_12_40,"HandleSelectWinMsg msg_id = %x")
TRACE_MSG(MMIJAVA_WINTAB_2390_112_2_18_2_22_12_41,"HandleSelectWinMsg len = %d")
TRACE_MSG(MMIJAVA_WINTAB_2436_112_2_18_2_22_12_42,"HandleJavaAMSMidletListWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIJAVA_WINTAB_2453_112_2_18_2_22_13_43,"HandleJavaAMSMidletListWinMsg GUILIST_GetCurItemIndex %d")
TRACE_MSG(MMIJAVA_WINTAB_2503_112_2_18_2_22_13_44,"HandleJavaAMSWinMsg, msg_id = 0x%02x")
TRACE_MSG(MMIJAVA_WINTAB_2511_112_2_18_2_22_13_45,"HandleJavaAMSWinMsg, s_suiteList ALLOC failed")
TRACE_MSG(MMIJAVA_WINTAB_2532_112_2_18_2_22_13_46,"HandleJavaAMSWinMsg cursuitid = %d")
TRACE_MSG(MMIJAVA_WINTAB_2584_112_2_18_2_22_13_47,"HandleJavaAMSWinMsg cursuitid = %d")
TRACE_MSG(MMIJAVA_WINTAB_2647_112_2_18_2_22_13_48,"HandleJavaAMSWinMsg cursuitid = %d")
TRACE_MSG(MMIJAVA_WINTAB_2678_112_2_18_2_22_13_49,"HandleJavaDownLoadEnter Enter type =%d")
TRACE_MSG(MMIJAVA_WINTAB_2714_112_2_18_2_22_13_50,"HandleJavaDownLoadEnter exit")
TRACE_MSG(MMIJAVA_WINTAB_2743_112_2_18_2_22_13_51,"MMIJAVA_ResetSystem enter!")
TRACE_MSG(MMIJAVA_WINTAB_2748_112_2_18_2_22_13_52,"path_sytem_e_nor error = %d")
TRACE_MSG(MMIJAVA_WINTAB_2751_112_2_18_2_22_13_53,"path_sytem_f_nor error = %d")
TRACE_MSG(MMIJAVA_WINTAB_2758_112_2_18_2_22_13_54,"path_sytem_f error = %d")
TRACE_MSG(MMIJAVA_WINTAB_2762_112_2_18_2_22_13_55,"path_sytem_e error = %d")
TRACE_MSG(MMIJAVA_WINTAB_2765_112_2_18_2_22_13_56,"path_sytem_d error = %d")
TRACE_MSG(MMIJAVA_WINTAB_2768_112_2_18_2_22_13_57,"path_sytem_c error = %d")
TRACE_MSG(MMIJAVA_WINTAB_2775_112_2_18_2_22_13_58,"MMIJAVA_ResetSystem success!")
TRACE_MSG(MMIJAVA_WINTAB_2793_112_2_18_2_22_13_59,"HandleJavaResetSystemPromptWinMsg,msg_id =%d")
TRACE_MSG(MMIJAVA_WINTAB_2845_112_2_18_2_22_13_60,"HandleJavaDeletePromptWinMsg win_info_ptr == PNULL !")
TRACE_MSG(MMIJAVA_WINTAB_2913_112_2_18_2_22_14_61,"HandleJavaAMSOptionWinMsg,msg_id =%d")
TRACE_MSG(MMIJAVA_WINTAB_3019_112_2_18_2_22_14_62,"HandleJavaAMSOptionWinMsg,msg_id =%d")
TRACE_MSG(MMIJAVA_WINTAB_3188_112_2_18_2_22_14_63,"AddOneDetailItem enter ")
TRACE_MSG(MMIJAVA_WINTAB_3218_112_2_18_2_22_14_64,"AddOneDetailItem fail ")
TRACE_MSG(MMIJAVA_WINTAB_3322_112_2_18_2_22_15_65,"SetJavaInfoDetail s_suiteList[cursuitid].domainId = %d")
TRACE_MSG(MMIJAVA_WINTAB_3399_112_2_18_2_22_15_66,"enter MMIJAVA_CreateNetworkSetting, suitId= %d")
TRACE_MSG(MMIJAVA_WINTAB_3406_112_2_18_2_22_15_67,"MMIJAVA_CreateNetworkSetting setting_file_ascii= %s")
TRACE_MSG(MMIJAVA_WINTAB_3416_112_2_18_2_22_15_68,"MMIJAVA_CreateNetworkSetting setting_handle= %d")
TRACE_MSG(MMIJAVA_WINTAB_3421_112_2_18_2_22_15_69,"MMIJAVA_CreateNetworkSetting bytes_written= %d")
TRACE_MSG(MMIJAVA_WINTAB_3461_112_2_18_2_22_15_70,"MMIJAVA_GetNetSettingInfo file name = %s")
TRACE_MSG(MMIJAVA_WINTAB_3464_112_2_18_2_22_15_71,"MMIJAVA_GetNetSettingInfo setting file handle = %d")
TRACE_MSG(MMIJAVA_WINTAB_3501_112_2_18_2_22_15_72,"MMIJAVA_GetNetSettingIndex enter")
TRACE_MSG(MMIJAVA_WINTAB_3515_112_2_18_2_22_15_73,"MMIJAVA_GetNetSettingIndex file name = %s")
TRACE_MSG(MMIJAVA_WINTAB_3518_112_2_18_2_22_15_74,"MMIJAVA_GetNetSettingIndex setting file handle = %d")
TRACE_MSG(MMIJAVA_WINTAB_3551_112_2_18_2_22_15_75,"MMIJAVA_SetNetSettingInfo setting file:%s")
TRACE_MSG(MMIJAVA_WINTAB_3554_112_2_18_2_22_15_76,"MMIJAVA_SetNetSettingInfo setting file handle:%d")
TRACE_MSG(MMIJAVA_WINTAB_3559_112_2_18_2_22_15_77,"MMIJAVA_SetNetSettingInfo bytes_written=%d")
TRACE_MSG(MMIJAVA_WINTAB_3717_112_2_18_2_22_15_78,"InitPBFolder enter")
TRACE_MSG(MMIJAVA_WINTAB_3743_112_2_18_2_22_15_79,"MMIAPIFMM_SetAttr result %d")
TRACE_MSG(MMIJAVA_WINTAB_3748_112_2_18_2_22_15_80,"InitPBFolder result %d")
TRACE_MSG(MMIJAVA_WINTAB_3782_112_2_18_2_22_15_81,"Java_LocalInstall_To current_item=%d")
TRACE_MSG(MMIJAVA_WINTAB_3783_112_2_18_2_22_15_82,"Java_LocalInstall_To current_item=%d %d")
TRACE_MSG(MMIJAVA_WINTAB_3784_112_2_18_2_22_15_83,"Java_LocalInstall_To jar_data_size = %d  cur_item=%d,disk_name=%d")
TRACE_MSG(MMIJAVA_WINTAB_3794_112_2_18_2_22_16_84,"Java_LocalInstall_To drm = %s")
TRACE_MSG(MMIJAVA_WINTAB_3836_112_2_18_2_22_16_85,"Java_LocalInstall_To  current_item=%d")
TRACE_MSG(MMIJAVA_WINTAB_3874_112_2_18_2_22_16_86,"Java_LocalInstall_To url=%s")
TRACE_MSG(MMIJAVA_WINTAB_3876_112_2_18_2_22_16_87,"Java_LocalInstall_To jadinstalltoPath = %s")
TRACE_MSG(MMIJAVA_WINTAB_3964_112_2_18_2_22_16_88,"Java_LocalInstall_To TRUE")
TRACE_MSG(MMIJAVA_WINTAB_3969_112_2_18_2_22_16_89,"Java_LocalInstall_To FLASE result = %d")
TRACE_MSG(MMIJAVA_WINTAB_3990_112_2_18_2_22_16_90,"HandleJavaSelectInstallDiskWinMsg, msg_id = 0x%02x")
TRACE_MSG(MMIJAVA_WINTAB_4030_112_2_18_2_22_16_91,"HandleJavaSelectInstallDiskWinMsg cursuitid = %d")
TRACE_MSG(MMIJAVA_WINTAB_4040_112_2_18_2_22_16_92,"HandleJavaSelectInstallDiskWinMsg cursuitid = %d")
TRACE_MSG(MMIJAVA_WINTAB_4078_112_2_18_2_22_16_93,"HandleInstallInfoWinMsg msg_id =%x ")
TRACE_MSG(MMIJAVA_WINTAB_4121_112_2_18_2_22_16_94,"HandleInstallInfoWinMsg jar_len = %d")
TRACE_MSG(MMIJAVA_WINTAB_4241_112_2_18_2_22_16_95,"mmijava_wintab.c, OpenJarFile() enter!")
TRACE_MSG(MMIJAVA_WINTAB_4251_112_2_18_2_22_16_96,"mmijava_wintab.c, OpenJarFile(), FALSE 1")
TRACE_MSG(MMIJAVA_WINTAB_4256_112_2_18_2_22_16_97,"mmijava_wintab.c, OpenJarFile(), file_len = %d")
TRACE_MSG(MMIJAVA_WINTAB_4261_112_2_18_2_22_16_98,"mmijava_wintab.c, OpenJarFile(), FALSE 2")
TRACE_MSG(MMIJAVA_WINTAB_4269_112_2_18_2_22_16_99,"mmijava_wintab.c, OpenJarFile(), FALSE 2")
TRACE_MSG(MMIJAVA_WINTAB_4279_112_2_18_2_22_16_100,"OpenJarFile file_info_ptr->file_content_ptr = %x")
TRACE_MSG(MMIJAVA_WINTAB_4283_112_2_18_2_22_16_101,"mmijava_wintab.c, OpenJarFile(), FALSE 3")
TRACE_MSG(MMIJAVA_WINTAB_4304_112_2_18_2_22_17_102,"mmijava_wintab.c, OpenJarFile(), FALSE 4, error = %d")
TRACE_MSG(MMIJAVA_WINTAB_4311_112_2_18_2_22_17_103,"mmijava_wintab.c, OpenJarFile(), TRUE 5")
TRACE_MSG(MMIJAVA_WINTAB_4376_112_2_18_2_22_17_104,"HandleCheckWinMsg msg_id = %x, ")
TRACE_MSG(MMIJAVA_WINTAB_4502_112_2_18_2_22_17_105,"HandleLocalCheckResult s_local_check_result.oldId = %d")
TRACE_MSG(MMIJAVA_WINTAB_4534_112_2_18_2_22_17_106,"HandleLocalCheckResult otacode = %d")
TRACE_MSG(MMIJAVA_WINTAB_4608_112_2_18_2_22_17_107,"\"\"\"\"\" MMIJAVA_jblendia_start\"\"\"\"\"")
TRACE_MSG(MMIJAVA_WINTAB_4631_112_2_18_2_22_17_108,"HandleJavaSubWinMsg launch error")
TRACE_MSG(MMIJAVA_WINTAB_4651_112_2_18_2_22_17_109,"HandleJavaSubWinMsg launch error")
TRACE_MSG(MMIJAVA_WINTAB_4671_112_2_18_2_22_17_110,"HandleJavaSubWinMsg MMIJAVA_jwe_run ret =%d")
TRACE_MSG(MMIJAVA_WINTAB_4686_112_2_18_2_22_17_111,"HandleJavaSubWinMsg MMIJAVA_jwe_run leave")
TRACE_MSG(MMIJAVA_WINTAB_4710_112_2_18_2_22_17_112,"\"\"\"\"\" tck test start \"\"\"\"\"")
TRACE_MSG(MMIJAVA_WINTAB_4718_112_2_18_2_22_17_113,"\"\"\"\"\"\"\"\"# tck test url: %s")
TRACE_MSG(MMIJAVA_WINTAB_4739_112_2_18_2_22_17_114,"HandleJavaSubWinMsg launch error")
TRACE_MSG(MMIJAVA_WINTAB_4756_112_2_18_2_22_17_115,"HandleJavaSubWinMsg launch error")
TRACE_MSG(MMIJAVA_WINTAB_4786_112_2_18_2_22_17_116,"MMIJAVA_VirtualKeyboardTPToKJKey:active_id = %x")
TRACE_MSG(MMIJAVA_WINTAB_4856_112_2_18_2_22_18_117,"MMIJAVA_VirtualKeyboardTPToKJKey:key_type,key_code =%d,%d")
TRACE_MSG(MMIJAVA_WINTAB_4914_112_2_18_2_22_18_118,"rect(%d,%d,%d,%d)")
TRACE_MSG(MMIJAVA_WINTAB_5040_112_2_18_2_22_18_119,"JVM has been runing....")
TRACE_MSG(MMIJAVA_WINTAB_5046_112_2_18_2_22_18_120,"JVM start step1,try to allocate memory ahead")
TRACE_MSG(MMIJAVA_WINTAB_5051_112_2_18_2_22_18_121,"JVM start step1,allocate failed, exit")
TRACE_MSG(MMIJAVA_WINTAB_5086_112_2_18_2_22_18_122,"native key(red) is 0x%x")
TRACE_MSG(MMIJAVA_WINTAB_5211_112_2_18_2_22_18_123,"native key(no red) is 0x%x")
TRACE_MSG(MMIJAVA_WINTAB_5248_112_2_18_2_22_18_124,"HandleJavaSubWinMsg this is no acordding key code")
TRACE_MSG(MMIJAVA_WINTAB_5270_112_2_18_2_22_18_125,"HandleJavaSubWinMsg  -- MSG_CLOSE_WINDOW!")
TRACE_MSG(MMIJAVA_WINTAB_5291_112_2_18_2_22_18_126,"HandleJavaSubWinMsg  -- MSG_LOSE_FOCUS!")
TRACE_MSG(MMIJAVA_WINTAB_5334_112_2_18_2_22_19_127,"HandleJavaSubWinMsg  -- MSG_GET_FOCUS!")
TRACE_MSG(MMIJAVA_WINTAB_5499_112_2_18_2_22_19_128,"CreateInstallDiskListBox ")
TRACE_MSG(MMIJAVA_WINTAB_5504_112_2_18_2_22_19_129,"CreateInstallDiskListBox C")
TRACE_MSG(MMIJAVA_WINTAB_5586_112_2_18_2_22_19_130,"ACJ->ajms_ab_getSutieListCount return applet = %d")
TRACE_MSG(MMIJAVA_WINTAB_5602_112_2_18_2_22_19_131,"no midlet or more size")
TRACE_MSG(MMIJAVA_WINTAB_5625_112_2_18_2_22_19_132,"--error: could not get the installed midlet list.")
TRACE_MSG(MMIJAVA_WINTAB_5629_112_2_18_2_22_19_133,"CreateSuitListBox s_preinstall_id = %d")
TRACE_MSG(MMIJAVA_WINTAB_5637_112_2_18_2_22_19_134,"CreateSuitListBox j = %d")
TRACE_MSG(MMIJAVA_WINTAB_5756_112_2_18_2_22_19_135,"DrawListItemWithIconData item_index:%d, content_index: %d, icon size:%d")
TRACE_MSG(MMIJAVA_WINTAB_5762_112_2_18_2_22_19_136,"DrawListItemWithIconData data_info_ptr  failed")
TRACE_MSG(MMIJAVA_WINTAB_5772_112_2_18_2_22_19_137,"DrawListItemWithIconData get the first midlet icon")
TRACE_MSG(MMIJAVA_WINTAB_5850_112_2_18_2_22_20_138,"CreateMidletListBox suit id is %d,")
TRACE_MSG(MMIJAVA_WINTAB_5855_112_2_18_2_22_20_139,"-- number of midlet(s) is %d.")
TRACE_MSG(MMIJAVA_WINTAB_5887_112_2_18_2_22_20_140,"--error: failed to get midlet list.")
TRACE_MSG(MMIJAVA_WINTAB_5960_112_2_18_2_22_20_141,"jblendia_setMidletWinTitle iNumOfSuite = %d")
TRACE_MSG(MMIJAVA_WINTAB_5964_112_2_18_2_22_20_142,"jblendia_setMidletWinTitle iNumOfSuite < 0")
TRACE_MSG(MMIJAVA_WINTAB_5971_112_2_18_2_22_20_143,"jblendia_setMidletWinTitle suiteList_ptr ALLOC FAILED")
TRACE_MSG(MMIJAVA_WINTAB_5979_112_2_18_2_22_20_144,"jblendia_setMidletWinTitle ajms_ab_getSuiteList FAILED")
TRACE_MSG(MMIJAVA_WINTAB_6021_112_2_18_2_22_20_145,"C:\\delrpt.txt not exist")
TRACE_MSG(MMIJAVA_WINTAB_6026_112_2_18_2_22_20_146,"jblendia_send_delete_report file name = %s")
TRACE_MSG(MMIJAVA_WINTAB_6029_112_2_18_2_22_20_147,"jblendia_send_delete_report setting file handle = %d")
TRACE_MSG(MMIJAVA_WINTAB_6439_112_2_18_2_22_21_148,"JavaDeleteSuite jadSize = %d")
TRACE_MSG(MMIJAVA_WINTAB_6448_112_2_18_2_22_21_149,"delete notify failed, can't get jad file, return %d.\n")
TRACE_MSG(MMIJAVA_WINTAB_6470_112_2_18_2_22_21_150,"delete notify failed, can't get property \"MIDlet-Delete-Notify\", return %d.\n")
TRACE_MSG(MMIJAVA_WINTAB_6484_112_2_18_2_22_21_151,"JavaDeleteSuite flag = %d")
TRACE_MSG(MMIJAVA_WINTAB_6612_112_2_18_2_22_21_152,"HandleJavaPermitSetWinMsg: msg_id = %d")
TRACE_MSG(MMIJAVA_WINTAB_6694_112_2_18_2_22_21_153,"HandleJavaPermitOptWinMsg: msg_id = %d")
TRACE_MSG(MMIJAVA_WINTAB_6702_112_2_18_2_22_21_154,"cur_group = %d")
TRACE_MSG(MMIJAVA_WINTAB_6788_112_2_18_2_22_21_155,"HandleJavaPermitOptWinMsg:result =%d")
TRACE_MSG(MMIJAVA_WINTAB_7238_112_2_18_2_22_22_156,"MMIAPIJAVA_StartLocalInstall(): jad")
TRACE_MSG(MMIJAVA_WINTAB_7416_112_2_18_2_22_23_157,"MMIAPIJAVA_JavaGameDirectStart(): jar")
TRACE_MSG(MMIJAVA_WINTAB_7439_112_2_18_2_22_23_158,"MMIAPIJAVA_StartLocalInstall(): OPEN jar error")
TRACE_MSG(MMIJAVA_WINTAB_7479_112_2_18_2_22_23_159,"MMIAPIJAVA_StartLocalInstall(): OPEN jar error")
TRACE_MSG(MMIJAVA_WINTAB_7499_112_2_18_2_22_23_160,"s_install_data_info_ptr alloc failed!")
TRACE_MSG(MMIJAVA_WINTAB_7532_112_2_18_2_22_23_161,"jar_len=%d")
TRACE_MSG(MMIJAVA_WINTAB_7533_112_2_18_2_22_23_162,"jad_len=%d")
TRACE_MSG(MMIJAVA_WINTAB_7534_112_2_18_2_22_23_163,"jar_file_path=%d")
TRACE_MSG(MMIJAVA_WINTAB_7600_112_2_18_2_22_23_164,"StartLocalInstall(): out!")
TRACE_MSG(MMIJAVA_WINTAB_7611_112_2_18_2_22_23_165,"MMIJAVA_ReturnToApplicationMenu ENTER")
TRACE_MSG(MMIJAVA_WINTAB_7626_112_2_18_2_22_23_166,"PauseDialg_OnBTN, btn_type = %d")
TRACE_MSG(MMIJAVA_WINTAB_7676_112_2_18_2_22_23_167,"HandleJavaPauseDialgWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIJAVA_WINTAB_7696_112_2_18_2_22_23_168,"HandleJavaPauseDialgWinMsg GUILIST_GetCurItemIndex %d")
TRACE_MSG(MMIJAVA_WINTAB_7755_112_2_18_2_22_23_169,"ResumeDialg_OnBTN, btn_type = %d")
TRACE_MSG(MMIJAVA_WINTAB_7797_112_2_18_2_22_23_170,"HandleJavaResumeDialgWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIJAVA_WINTAB_7816_112_2_18_2_22_24_171,"HandleJavaAMSMidletListWinMsg GUILIST_GetCurItemIndex %d")
TRACE_MSG(MMIJAVA_WINTAB_7870_112_2_18_2_22_24_172,"mmijava_wintab.c InstallJadDataReader, size = %d")
TRACE_MSG(MMIJAVA_WINTAB_7903_112_2_18_2_22_24_173,"mmijava_wintab.c InstallJarDataReader, size = %d")
TRACE_MSG(MMIJAVA_WINTAB_7915_112_2_18_2_22_24_174,"mmijava_wintab.c InstallJarDataReader, readcount = %d")
TRACE_MSG(MMIJAVA_WINTAB_7940_112_2_18_2_22_24_175,"point 0")
TRACE_MSG(MMIJAVA_WINTAB_7953_112_2_18_2_22_24_176,"point 1 %d")
TRACE_MSG(MMIJAVA_WINTAB_7964_112_2_18_2_22_24_177,"point 2 %d")
TRACE_MSG(MMIJAVA_WINTAB_7972_112_2_18_2_22_24_178,"point 3")
TRACE_MSG(MMIJAVA_WINTAB_8035_112_2_18_2_22_24_179,"LocalCheck data_size = %d,data_addr = %x")
TRACE_MSG(MMIJAVA_WINTAB_8074_112_2_18_2_22_24_180,"Java file type is jad")
TRACE_MSG(MMIJAVA_WINTAB_8079_112_2_18_2_22_24_181,"Java file type is jar")
TRACE_MSG(MMIJAVA_WINTAB_8119_112_2_18_2_22_24_182,"LocalCheck result = %d")
TRACE_MSG(MMIJAVA_WINTAB_8123_112_2_18_2_22_24_183,"LocalCheck TRUE")
TRACE_MSG(MMIJAVA_WINTAB_8128_112_2_18_2_22_24_184,"LocalCheck FALSE")
TRACE_MSG(MMIJAVA_WINTAB_8147_112_2_18_2_22_24_185,"Java_LocalCheckCallBack step = %d,total = %d")
TRACE_MSG(MMIJAVA_WINTAB_8155_112_2_18_2_22_24_186,"LocalCheck Success")
TRACE_MSG(MMIJAVA_WINTAB_8160_112_2_18_2_22_24_187,"LocalCheck Waiting....")
TRACE_MSG(MMIJAVA_WINTAB_8171_112_2_18_2_22_24_188,"MMIJAVA_LocalCheck Fail")
TRACE_MSG(MMIJAVA_WINTAB_8198_112_2_18_2_22_24_189,"Java_LocalInstall jar_data_size = %d")
TRACE_MSG(MMIJAVA_WINTAB_8240_112_2_18_2_22_24_190,"Java_LocalInstall TRUE")
TRACE_MSG(MMIJAVA_WINTAB_8245_112_2_18_2_22_24_191,"Java_LocalInstall FLASE result = %d")
TRACE_MSG(MMIJAVA_WINTAB_8267_112_2_18_2_22_24_192,"MMIJAVA_LaunchMidletDirectAfterInstallOk install_result.suiteId = %d")
TRACE_MSG(MMIJAVA_WINTAB_8325_112_2_18_2_22_25_193,"HandleInstallWinMsg msg_id =%x ")
TRACE_MSG(MMIJAVA_WINTAB_8342_112_2_18_2_22_25_194,"Java_LocalInstall jar_len = %d")
TRACE_MSG(MMIJAVA_WINTAB_8343_112_2_18_2_22_25_195,"s_local_check_result minimumInstallSize = %d")
TRACE_MSG(MMIJAVA_WINTAB_8364_112_2_18_2_22_25_196,"Java_LocalInstall Fail")
TRACE_MSG(MMIJAVA_WINTAB_8380_112_2_18_2_22_25_197,"HandleInstallWinMsg s_install_data_info_ptr = %x, s_install_data_info_ptr->is_ota = %d")
TRACE_MSG(MMIJAVA_WINTAB_8383_112_2_18_2_22_25_198,"HandleInstallWinMsg s_local_check_result.installNotifyUrl = %s")
TRACE_MSG(MMIJAVA_WINTAB_8418_112_2_18_2_22_25_199,"s_local_check_result.oldId = %d")
TRACE_MSG(MMIJAVA_WINTAB_8476_112_2_18_2_22_25_200,"Java_LocalInstall Fail")
TRACE_MSG(MMIJAVA_WINTAB_8587_112_2_18_2_22_25_201,"HandleProcessWinMsg  msg_id %x")
TRACE_MSG(MMIJAVA_WINTAB_8632_112_2_18_2_22_25_202,"Java_LocalInstallCallBack s_preinstall_id = %d, s_local_check_result.oldId = %d")
TRACE_MSG(MMIJAVA_WINTAB_8637_112_2_18_2_22_25_203,"Java_LocalInstallCallBack s_preinstall_id = %d")
TRACE_MSG(MMIJAVA_WINTAB_8680_112_2_18_2_22_25_204,"[param = %d], function success, read %d bytes data in buffer\n")
TRACE_MSG(MMIJAVA_WINTAB_8696_112_2_18_2_22_25_205,"[param = %d], reading... %d bytes read\n")
TRACE_MSG(MMIJAVA_WINTAB_8701_112_2_18_2_22_25_206,"[param = %d], writing... %d bytes write\n")
TRACE_MSG(MMIJAVA_WINTAB_8706_112_2_18_2_22_25_207,"[param = %d], error occurred in download files, error value = %d\n")
TRACE_MSG(MMIJAVA_WINTAB_8723_112_2_18_2_22_25_208,"[param = %d], Authentication failed, set name / password.\n")
TRACE_MSG(MMIJAVA_WINTAB_8745_112_2_18_2_22_26_209,"jwe_ota_notify enter statusCode = %d, urlLength = %d ")
TRACE_MSG(MMIJAVA_WINTAB_8753_112_2_18_2_22_26_210,"jwe_ota_notify.., url = %s\n")
TRACE_MSG(MMIJAVA_WINTAB_8756_112_2_18_2_22_26_211,"StatusCode[i].code: %d.\n")
TRACE_MSG(MMIJAVA_WINTAB_8763_112_2_18_2_22_26_212,"t: %s.\n")
TRACE_MSG(MMIJAVA_WINTAB_8766_112_2_18_2_22_26_213,"error: t==NULL.\n")
TRACE_MSG(MMIJAVA_WINTAB_8773_112_2_18_2_22_26_214,"JARBuffer alloc failed")
TRACE_MSG(MMIJAVA_WINTAB_8782_112_2_18_2_22_26_215,"ignoreBuf alloc failed")
TRACE_MSG(MMIJAVA_WINTAB_8797_112_2_18_2_22_26_216,"error: ajsc_http_sendRequest.\n")
TRACE_MSG(MMIJAVA_WINTAB_8818_112_2_18_2_22_26_217,"notify start.., status = %d.\n")
TRACE_MSG(MMIJAVA_WINTAB_8932_112_2_18_2_22_26_218,"MMIAPIJAVA_OTAInstallDrm in param error null")
TRACE_MSG(MMIJAVA_WINTAB_8939_112_2_18_2_22_26_219,"MMIAPIJAVA_OTAInstallDrm in param error len is 0")
TRACE_MSG(MMIJAVA_WINTAB_8975_112_2_18_2_22_26_220,"MMIAPIJAVA_OTAInstallDrm s_install_data_info_ptr is not null")
TRACE_MSG(MMIJAVA_WINTAB_9014_112_2_18_2_22_26_221,"MMIAPIJAVA_StartLocalInstall(): OPEN jar error")
TRACE_MSG(MMIJAVA_WINTAB_9089_112_2_18_2_22_27_222,"MMIJAVA_OTADownloadFailHandle  downresult =%d, s_local_check_result.installNotifyUrl lenth = %d")
TRACE_MSG(MMIJAVA_WINTAB_9176_112_2_18_2_22_27_223,"OTA_DELETE_JAD=%d OTA_DELETE_JAR=%d")
TRACE_MSG(MMIJAVA_WINTAB_9189_112_2_18_2_22_27_224,"OTA_DELETE_JAD ret=%d")
TRACE_MSG(MMIJAVA_WINTAB_9204_112_2_18_2_22_27_225,"OTA_JAR_FileName_ptr ret=%d")
TRACE_MSG(MMIJAVA_WINTAB_9567_112_2_18_2_22_28_226,"MMIJAVA_IsJavaRuning result is %d")
TRACE_MSG(MMIJAVA_WINTAB_9583_112_2_18_2_22_28_227,"MMIJAVA_OTAInstallJad_CallBackFunction enter ")
TRACE_MSG(MMIJAVA_WINTAB_9588_112_2_18_2_22_28_228,"MMIJAVA_OTAInstallJad_CallBackFunction,MMIAPIJAVA_IsJavaInstalling true exit ")
TRACE_MSG(MMIJAVA_WINTAB_9708_112_2_18_2_22_28_229,"MMIJAVA_OTAInstallJar_CallBackFunction,MMIAPIJAVA_IsJavaInstalling true exit ")
TRACE_MSG(MMIJAVA_WINTAB_9807_112_2_18_2_22_29_230,"MMIJAVA_OTAInstallJad  11")
TRACE_MSG(MMIJAVA_WINTAB_10181_112_2_18_2_22_30_231,"hanker MMIAPIJAVA_OpenWapWin enter ")
TRACE_MSG(MMIJAVA_WINTAB_10185_112_2_18_2_22_30_232,"MMIAPIJAVA_OpenWapWin hanker enter s_local_check_result.url=%s")
TRACE_MSG(MMIJAVA_WINTAB_10206_112_2_18_2_22_30_233,"MMIAPIJAVA_OpenWapWin hanker enter g_java_wap_info.url=%s")
TRACE_MSG(MMIJAVA_WINTAB_10221_112_2_18_2_22_30_234,"MMIAPIJAVA_OpenWapWin url is null")
TRACE_MSG(MMIJAVA_WINTAB_10266_112_2_18_2_22_30_235,"MMIJAVA_SuiteUpdate,suite_id =%d")
TRACE_MSG(MMIJAVA_WINTAB_10271_112_2_18_2_22_30_236,"MMIJAVA_SuiteUpdate,erorcode =%d")
TRACE_MSG(MMIJAVA_WINTAB_10276_112_2_18_2_22_30_237,"MMIJAVA_SuiteUpdate,jadurl =%s")
TRACE_MSG(MMIJAVA_WINTAB_10325_112_2_18_2_22_30_238,"MMIJAVA_initPreinstallID return applet = %d")
TRACE_MSG(MMIJAVA_WINTAB_10350_112_2_18_2_22_30_239,"MMIJAVA_initPreinstallID s_preinstall_id = %d")
TRACE_MSG(MMIJAVA_WINTAB_10378_112_2_18_2_22_30_240,"MMIJAVA_initPreinstallID return applet = %d")
TRACE_MSG(MMIJAVA_WINTAB_10574_112_2_18_2_22_31_241,"MMIAPIJAVA_IsJavaBackgroundRuning result is %d")
TRACE_MSG(MMIJAVA_WINTAB_10591_112_2_18_2_22_31_242,"MMIJAVA_IsJavaRuning result is %d")
TRACE_MSG(MMIJAVA_WINTAB_10704_112_2_18_2_22_31_243,"MMIJAVA_StartMIDletByNameVendor cursuitid = %d")
TRACE_MSG(MMIJAVA_WINTAB_10729_112_2_18_2_22_31_244,"JavaGameDirectStart(): enter!")
TRACE_MSG(MMIJAVA_WINTAB_10750_112_2_18_2_22_31_245,"JavaGameDirectStart(): jad")
TRACE_MSG(MMIJAVA_WINTAB_10774_112_2_18_2_22_31_246,"JavaGameDirectStart(): jar")
TRACE_MSG(MMIJAVA_WINTAB_10786_112_2_18_2_22_31_247,"JavaGameDirectStart: ignore this file")
TRACE_MSG(MMIJAVA_WINTAB_10811_112_2_18_2_22_31_248,"JavaGameDirectStart(): out!")
TRACE_MSG(MMIJAVA_WINTAB_10895_112_2_18_2_22_31_249,"MMIAPIJAVA_ResumeBackgroundJVM g_jvm_is_running_by_mmi =%d")
TRACE_MSG(MMIJAVA_WINTAB_10904_112_2_18_2_22_31_250,"MMIAPIJAVA_ResumeBackgroundJVM ajms_arc_resumeApp fail!!!")
TRACE_MSG(MMIJAVA_WINTAB_10909_112_2_18_2_22_31_251,"MMIAPIJAVA_ResumeBackgroundJVM do nothing exit")
TRACE_MSG(MMIJAVA_WINTAB_10921_112_2_18_2_22_31_252,"MMIAPIJAVA_PauseBackgroundJVM g_jvm_is_running_by_mmi =%d")
TRACE_MSG(MMIJAVA_WINTAB_10931_112_2_18_2_22_31_253,"MMIAPIJAVA_PauseBackgroundJVM ajms_arc_suspendApp fail!!!")
TRACE_MSG(MMIJAVA_WINTAB_10936_112_2_18_2_22_31_254,"MMIAPIJAVA_PauseBackgroundJVM do nothing exit")
TRACE_MSG(MMIJAVA_WINTAB_11039_112_2_18_2_22_32_255,"MMIAPIJAVA_PlatformRequest enter")
TRACE_MSG(MMIJAVA_WINTAB_11043_112_2_18_2_22_32_256,"MMIAPIJAVA_PlatformRequest enter g_java_call_info_ptr = %s")
TRACE_MSG(MMIJAVA_WINTAB_11058_112_2_18_2_22_32_257,"MMIAPIJAVA_PlatformRequest type = %d ")
TRACE_MSG(MMIJAVA_WINTAB_11184_112_2_18_2_22_32_258,"mmi JavaApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIJAVA_WINTAB_11268_112_2_18_2_22_32_259,"MMIAPIJAVA_StartApplet")
TRACE_MSG(MMIJAVA_WINTAB_11287_112_2_18_2_22_32_260,"HandleJavaAppletWinMsg:msg_id=%d")
TRACE_MSG(JGURTOOLKIT_117_112_2_18_2_22_34_261,"commandmenuPaint")
TRACE_MSG(JKPIM_22_112_2_18_2_22_37_262,"JkPIMGetListNames enter!")
TRACE_MSG(JKPIM_23_112_2_18_2_22_37_263,"JkPIMGetListNames,enter")
TRACE_MSG(JKPIM_38_112_2_18_2_22_37_264,"leave")
TRACE_MSG(JKPIM_47_112_2_18_2_22_37_265,"JkPIMGetDefaultListName:enter")
TRACE_MSG(JKPIM_61_112_2_18_2_22_37_266,"JkPIMGetDefaultListName,leave")
TRACE_MSG(JKPIM_70_112_2_18_2_22_37_267,"JkPIMOpenConnection:enter")
TRACE_MSG(JKPIM_84_112_2_18_2_22_37_268,"JkPIMOpenConnection,leave")
TRACE_MSG(JKPIM_92_112_2_18_2_22_37_269,"JkPIMCloseConnection:enter")
TRACE_MSG(JKPIM_105_112_2_18_2_22_37_270,"JkPIMCloseConnection,leave")
TRACE_MSG(JKPIM_114_112_2_18_2_22_37_271,"JkPIMGetSupportedFields:enter")
TRACE_MSG(JKPIM_127_112_2_18_2_22_37_272,"JkPIMGetSupportedFields,leave")
TRACE_MSG(JKPIM_136_112_2_18_2_22_37_273,"JkPIMGetSupportedAttributes:enter")
TRACE_MSG(JKPIM_149_112_2_18_2_22_37_274,"JkPIMGetSupportedAttributes,leave")
TRACE_MSG(JKPIM_159_112_2_18_2_22_37_275,"JkPIMReadItem:enter")
TRACE_MSG(JKPIM_173_112_2_18_2_22_37_276,"JkPIMReadItem,leave")
TRACE_MSG(JKPIM_182_112_2_18_2_22_37_277,"JkPIMAddItem:enter")
TRACE_MSG(JKPIM_196_112_2_18_2_22_38_278,"JkPIMAddItem,leave")
TRACE_MSG(JKPIM_205_112_2_18_2_22_38_279,"JkPIMSetItem:enter")
TRACE_MSG(JKPIM_219_112_2_18_2_22_38_280,"JkPIMSetItem,leave")
TRACE_MSG(JKPIM_227_112_2_18_2_22_38_281,"JkPIMDeleteItem:enter")
TRACE_MSG(JKPIM_241_112_2_18_2_22_38_282,"JkPIMDeleteItem,leave")
TRACE_MSG(JKPIM_249_112_2_18_2_22_38_283,"JkPIMIsCategorySupported:enter")
TRACE_MSG(JKPIM_263_112_2_18_2_22_38_284,"JkPIMIsCategorySupported,leave")
TRACE_MSG(JKPIM_271_112_2_18_2_22_38_285,"JkPIMGetSupportedCategories:enter")
TRACE_MSG(JKPIM_285_112_2_18_2_22_38_286,"JkPIMGetSupportedCategories,leave")
TRACE_MSG(JKPIM_292_112_2_18_2_22_38_287,"JkPIMIsCategory:enter")
TRACE_MSG(JKPIM_307_112_2_18_2_22_38_288,"JkPIMIsCategory,leave")
TRACE_MSG(JKPIM_314_112_2_18_2_22_38_289,"JkPIMAddCategory:enter")
TRACE_MSG(JKPIM_329_112_2_18_2_22_38_290,"JkPIMAddCategory,leave")
TRACE_MSG(JKPIM_336_112_2_18_2_22_38_291,"JkPIMDeleteCategory:enter")
TRACE_MSG(JKPIM_351_112_2_18_2_22_38_292,"JkPIMDeleteCategory,leave")
TRACE_MSG(JKPIM_358_112_2_18_2_22_38_293,"JkPIMRenameCategory:enter")
TRACE_MSG(JKPIM_373_112_2_18_2_22_38_294,"JkPIMRenameCategory,leave")
TRACE_MSG(JKPIM_380_112_2_18_2_22_38_295,"JkPIMAddCategoryToItem:enter")
TRACE_MSG(JKPIM_395_112_2_18_2_22_38_296,"JkPIMAddCategoryToItem,leave")
TRACE_MSG(JKPIM_402_112_2_18_2_22_38_297,"JkPIMMoveToNextItem:enter")
TRACE_MSG(JKPIM_417_112_2_18_2_22_38_298,"JkPIMMoveToNextItem,leave")
TRACE_MSG(JKPIM_424_112_2_18_2_22_38_299,"JkPIMGetResultSetByAll:enter")
TRACE_MSG(JKPIM_439_112_2_18_2_22_38_300,"JkPIMGetResultSetByAll,leave")
TRACE_MSG(JKPIM_447_112_2_18_2_22_38_301,"JkPIMGetResultSetByItem:enter")
TRACE_MSG(JKPIM_461_112_2_18_2_22_38_302,"JkPIMGetResultSetByAll,leave")
TRACE_MSG(JKPIM_469_112_2_18_2_22_38_303,"JkPIMGetResultSetByString:enter")
TRACE_MSG(JKPIM_483_112_2_18_2_22_38_304,"JkPIMGetResultSetByString,leave")
TRACE_MSG(JKPIM_491_112_2_18_2_22_38_305,"JkPIMGetResultSetByCategory:enter")
TRACE_MSG(JKPIM_505_112_2_18_2_22_38_306,"JkPIMGetResultSetByCategory,leave")
TRACE_MSG(JKPIM_513_112_2_18_2_22_38_307,"JkPIMGetResultSetForEvent:enter")
TRACE_MSG(JKPIM_522_112_2_18_2_22_38_308,"JkPIMGetResultSetForEvent,leave")
TRACE_MSG(JKPIM_530_112_2_18_2_22_38_309,"JkPIMGetResultSetForToDoDate:enter")
TRACE_MSG(JKPIM_539_112_2_18_2_22_38_310,"JkPIMGetResultSetForToDoDate,leave")
TRACE_MSG(JKPIM_547_112_2_18_2_22_38_311,"JkPIMGetResultSetForToDoCalendar:enter")
TRACE_MSG(JKPIM_556_112_2_18_2_22_39_312,"JkPIMGetResultSetForToDoCalendar,leave")
TRACE_MSG(JKPIM_564_112_2_18_2_22_39_313,"JkPIMCloseResultSet:enter")
TRACE_MSG(JKPIM_578_112_2_18_2_22_39_314,"JkPIMCloseResultSet,leave")
TRACE_MSG(JKPIM_584_112_2_18_2_22_39_315,"JkPIMAllocPointer enter!")
TRACE_MSG(JKPIM_590_112_2_18_2_22_39_316,"JkPIMFreePointer enter!")
TRACE_MSG(JKPIM_601_112_2_18_2_22_39_317,"JkPIMGetSupportedSerialFormats:enter")
TRACE_MSG(JKPIM_607_112_2_18_2_22_39_318,"JkPIMStartFromSerialFormat:enter")
TRACE_MSG(JKPIM_613_112_2_18_2_22_39_319,"JkPIMEndFromSerialFormat:enter")
TRACE_MSG(JKPIM_619_112_2_18_2_22_39_320,"JkPIMReceiveSerialByte:enter")
TRACE_MSG(JKPIM_625_112_2_18_2_22_39_321,"JkPIMReceiveSerialBytes:enter")
TRACE_MSG(JKPIM_631_112_2_18_2_22_39_322,"JkPIMGetSerialFormatHandlerClassName:enter")
TRACE_MSG(JKPIM_637_112_2_18_2_22_39_323,"JkPIMExecuteToSerialFormat:enter")
TRACE_MSG(JKPIM_643_112_2_18_2_22_39_324,"JkPIMExecuteFromSerialFormat:enter")
TRACE_MSG(JKPIM_650_112_2_18_2_22_39_325,"JkPIMGetSupportedRepeatRuleFields:enter")
TRACE_MSG(JKPIM_657_112_2_18_2_22_39_326,"JkPIMImportItem:enter")
TRACE_MSG(JKSOFTKEY_115_112_2_18_2_22_39_327,"[JkSoftkeySetLabel] (label, length, location) = (0x%08X,%d,%d)")
TRACE_MSG(JKSOFTKEY_207_112_2_18_2_22_39_328,"[JkSoftkeyResetLabels] enter.")
TRACE_MSG(JKSOFTKEY_223_112_2_18_2_22_39_329,"[JkSoftkeySetScrollState] (state) = (%d) ")
TRACE_MSG(JKSOFTKEY_242_112_2_18_2_22_39_330,"[JkSoftkeySetFocusState] (focusstate) = (%d) ")
TRACE_MSG(JKSOFTKEY_252_112_2_18_2_22_39_331,"[JkSoftkeyGetFocusState] focusState = (%d)")
TRACE_MSG(JKSOFTKEY_274_112_2_18_2_22_39_332,"[JkSoftkeyPaint] enter.")
TRACE_MSG(JKSOFTKEY_398_112_2_18_2_22_40_333,"[JkVideoDrawPermissionDialog] (g, index, keyCode, titleStr, titleSize, messageStr, messageSize) (0x%08X, %d, %d, 0x%08X, %d, 0x%08X, %d)")
TRACE_MSG(JKSOFTKEY_413_112_2_18_2_22_40_334,"[JkSoftkeyPaint-drawScrollArrow] enter.")
TRACE_MSG(JKUDP_307_112_2_18_2_22_41_335,"JkUdpConnect pclink set interface error")
TRACE_MSG(JKVMBACKGROUND_21_112_2_18_2_22_42_336,"[JkSetVmBackground] enter vmId=%d")
TRACE_MSG(JKVMBACKGROUND_25_112_2_18_2_22_42_337,"[JkSetVmForeground] enter vmId=%d")
TRACE_MSG(JKVMBACKGROUND_31_112_2_18_2_22_42_338,"[JkGetVmBgFgState] JK_BG_VM_STATE_BACKGROUND")
TRACE_MSG(JKVMBACKGROUND_35_112_2_18_2_22_42_339,"[JkGetVmBgFgState] JK_BG_VM_STATE_FOREGROUND")
TRACE_MSG(JKVMBACKGROUND_44_112_2_18_2_22_42_340,"[JkNotifyPopupDialog] = message= %s ")
TRACE_MSG(JKVMBACKGROUND_51_112_2_18_2_22_43_341,"[JkIsVmOnRealScreen]")
TRACE_MSG(JKVMTIMER_29_112_2_18_2_22_43_342,"[JkSetVmTimerHandler] handler!=null.")
TRACE_MSG(JKVMTIMER_36_112_2_18_2_22_43_343,"[JkSetVmTimerHandler] handler!==null.")
TRACE_MSG(JWE_83_112_2_18_2_23_4_344,"Ajava_Initiliztion_ia b_initializeJAVA = %d")
TRACE_MSG(JWE_99_112_2_18_2_23_4_345,"Ajava_Initiliztion_ia yuyu log failed")
TRACE_MSG(JWE_102_112_2_18_2_23_4_346,"Ajava_Initiliztion_ia yuyu log success")
TRACE_MSG(JWE_175_112_2_18_2_23_4_347,"Ajava_Initiliztion_Special b_initializeJAVA = %d")
TRACE_MSG(JWE_199_112_2_18_2_23_4_348,"IsDefaultJavaFolder delete suite.idx")
TRACE_MSG(JWE_201_112_2_18_2_23_4_349,"Ajava_Initiliztion_Special ajms_ret = %d")
TRACE_MSG(JWE_EIME_128_112_2_18_2_23_5_350,"MMIAPIJAVA_OpenTextEditWindow cur_inputmethod = %x")
TRACE_MSG(KJAVA_ADV_FS_68_112_2_18_2_23_7_351,"kjava_fs_setAttr enter")
TRACE_MSG(KJAVA_ADV_FS_695_112_2_18_2_23_8_352,"kjava_fs_setRootChangeNotification enter")
TRACE_MSG(KJAVA_SYS_CORE_762_112_2_18_2_23_11_353,"first=%d,second =%d")
TRACE_MSG(KJAVA_SYS_CORE_763_112_2_18_2_23_11_354,"kjava_core_allocateHugeMemory-------allocate memory size=%d")
TRACE_MSG(KJAVA_SYS_CORE_778_112_2_18_2_23_11_355,"FREE ENTER")
TRACE_MSG(KJAVA_SYS_CORE_781_112_2_18_2_23_11_356,"kjava_core_freeHugeMemory status = %d")
TRACE_MSG(KJAVA_SYS_CORE_788_112_2_18_2_23_11_357,"kjava_core_freeHugeMemory2 status = %d")
TRACE_MSG(KJAVA_SYS_CORE_799_112_2_18_2_23_11_358,"FREE OK")
TRACE_MSG(KJAVA_SYS_CORE_814_112_2_18_2_23_11_359,"kjava_core_getProperty (key)=(%s)")
TRACE_MSG(KJAVA_SYS_CORE_840_112_2_18_2_23_11_360,"kjava_core_getProperty return property.value=(%s)")
TRACE_MSG(KJAVA_SYS_CORE_855_112_2_18_2_23_11_361,"kjava_core_getProperty return property=(%s)")
TRACE_MSG(KJAVA_SYS_CORE_864_112_2_18_2_23_11_362,"kjava_core_getProperty return null")
TRACE_MSG(KJAVA_SYS_CORE_890_112_2_18_2_23_11_363,"return string = %s.")
TRACE_MSG(KJAVA_SYS_CORE_946_112_2_18_2_23_11_364,"[kjava_core_getOption]22 (m_option[%d].option)=(%s)")
TRACE_MSG(KJAVA_SYS_CORE_1114_112_2_18_2_23_12_365,"CB_Start_RequestConn call error, prev call has't ended")
TRACE_MSG(KJAVA_SYS_CORE_1126_112_2_18_2_23_12_366,"CALL OK")
TRACE_MSG(KJAVA_SYS_CORE_1135_112_2_18_2_23_12_367,"CB_Start_RequestConn call error, prev call has't ended")
TRACE_MSG(KJAVA_SYS_CORE_1147_112_2_18_2_23_12_368,"CALL OK")
TRACE_MSG(KJAVA_SYS_CORE_1151_112_2_18_2_23_12_369,"HTTP ENTER")
TRACE_MSG(KJAVA_SYS_CORE_1162_112_2_18_2_23_12_370,"CB_Start_RequestConn para = %s")
TRACE_MSG(KJAVA_SYS_CORE_1163_112_2_18_2_23_12_371,"CB_Start_RequestConn g_java_wap_info.url = %s")
TRACE_MSG(KJAVA_SYS_CORE_1164_112_2_18_2_23_12_372,"HTTP OK")
TRACE_MSG(KJAVA_SYS_GFX_151_112_2_18_2_23_18_373,"kjava_sys_gfx_loadCacheFont enter isDefaultFontCached=%d")
TRACE_MSG(KJAVA_SYS_GFX_240_112_2_18_2_23_18_374,"JVM_UpdateGameCanvasScreenTimer Enter")
TRACE_MSG(KJAVA_SYS_GFX_258_112_2_18_2_23_18_375,"JVM_UpdateGameCanvasScreenTimer AmNotifyRepaintEvent")
TRACE_MSG(KJAVA_SYS_GFX_263_112_2_18_2_23_18_376,"JVM_UpdateGameCanvasScreenTimer Leave")
TRACE_MSG(KJAVA_SYS_GFX_273_112_2_18_2_23_18_377,"JkVideoDrawOffScreenDirectly timerCount = %d, m_timeCount = %d")
TRACE_MSG(KJAVA_SYS_GFX_291_112_2_18_2_23_18_378,"JkVideoDrawOffScreenDirectly Leave")
TRACE_MSG(KJAVA_SYS_GFX_296_112_2_18_2_23_18_379,"JkVideoIsDrawOffScreenDirectlyPending Enter")
TRACE_MSG(KJAVA_SYS_GFX_298_112_2_18_2_23_18_380,"JkVideoIsDrawOffScreenDirectlyPending Leave")
TRACE_MSG(KJAVA_SYS_GFX_317_112_2_18_2_23_18_381,"java is not running,just return")
TRACE_MSG(KJAVA_SYS_GFX_380_112_2_18_2_23_19_382,"MMIAPIJAVA_CreateWinPaintTimer %d")
TRACE_MSG(KJAVA_SYS_GFX_413_112_2_18_2_23_19_383,"MMIJAVA_gfx_update enter  s_is_display_logo=%d  s_is_still_display_logo=%d")
TRACE_MSG(KJAVA_SYS_GFX_417_112_2_18_2_23_19_384,"java is not running,just return")
TRACE_MSG(KJAVA_SYS_GFX_473_112_2_18_2_23_19_385,"VIRTUAL_KEY_BOARD size = %d x %d")
TRACE_MSG(KJAVA_SYS_GFX_480_112_2_18_2_23_19_386,"screen size = %d x %d")
TRACE_MSG(KJAVA_SYS_GFX_486_112_2_18_2_23_19_387,"kjava_virtualScreenPixels alloc failed")
TRACE_MSG(KJAVA_SYS_GFX_599_112_2_18_2_23_19_388,"kjava_gfx_fini enter")
TRACE_MSG(KJAVA_SYS_GFX_686_112_2_18_2_23_19_389,"kjava_gfx_getSuitableFont size=%d style=%d face=%d defaultFont=0x%x")
TRACE_MSG(KJAVA_SYS_GFX_1671_112_2_18_2_23_21_390,"[kjava_gfx_imageAnalyze] (srcBuf, srcSize, width, height) = (0x%08X, %d, %d, %d)")
TRACE_MSG(KJAVA_SYS_GFX_1747_112_2_18_2_23_21_391,"kjava_gfx_imageDecode,(srcBuf, srcSize, argbData, dataLength, processAlpha) = (0x%08X, %d, 0x%08X, %d, 0x%08X)")
TRACE_MSG(KJAVA_SYS_GFX_1758_112_2_18_2_23_21_392,"kjava_gfx_imageDecode type == png")
TRACE_MSG(KJAVA_SYS_GFX_1763_112_2_18_2_23_21_393,"kjava_gfx_imageDecode alphaData = NULL SCI_ALLOC_APP failed!!!!OutOfMemory!")
TRACE_MSG(KJAVA_SYS_GFX_1783_112_2_18_2_23_21_394,"kjava_gfx_imageDecode time start = %d")
TRACE_MSG(KJAVA_SYS_GFX_1794_112_2_18_2_23_21_395,"kjava_gfx_imageDecode type == png, is_handle_alpha = %d")
TRACE_MSG(KJAVA_SYS_GFX_1800_112_2_18_2_23_21_396,"kjava_gfx_imageDecode time end = %d")
TRACE_MSG(KJAVA_SYS_GFX_1805_112_2_18_2_23_21_397,"kjava_gfx_imageDecode failed")
TRACE_MSG(KJAVA_SYS_LOG_OUT_49_112_2_18_2_23_23_398,"%s")
TRACE_MSG(KJAVA_SYS_SOCK_330_112_2_18_2_23_26_399,"kjava_sock_connect pclink set interface is wrong")
TRACE_MSG(KJAVA_SYS_SOCK_1444_112_2_18_2_23_29_400,"[MMIJAVA]:MMIJAVAHandlePDPMsg msg_ptr->result=%d")
TRACE_MSG(KJAVA_SYS_SOCK_1460_112_2_18_2_23_29_401,"MMIJAVAHandlePDPMsg: MMIPDP_DEACTIVE_IND")
TRACE_MSG(KJAVA_SYS_WMA_166_112_2_18_2_23_29_402,"kjava_wma_open port = %d")
TRACE_MSG(KJAVA_SYS_WMA_189_112_2_18_2_23_29_403,"kjava_wma_open gen port true")
TRACE_MSG(KJAVA_SYS_WMA_295_112_2_18_2_23_30_404,"IAJavaSMSSendCnfCallback !")
TRACE_MSG(KJAVA_SYS_WMA_302_112_2_18_2_23_30_405,"java:IAJavaSMSSendCnfCallback java_cb = PNULL")
TRACE_MSG(KJAVA_SYS_WMA_366_112_2_18_2_23_30_406,"kjava_wma_sendMessage i = %d")
TRACE_MSG(KJAVA_SYS_WMA_370_112_2_18_2_23_30_407,"kjava_wma_sendMessage message->url = %s,message->urlLength = %d")
TRACE_MSG(KJAVA_SYS_WMA_374_112_2_18_2_23_30_408,"kjava_wma_sendMessage parse url 0")
TRACE_MSG(KJAVA_SYS_WMA_388_112_2_18_2_23_30_409,"kjava_wma_sendMessage pcAddr = %s")
TRACE_MSG(KJAVA_SYS_WMA_430_112_2_18_2_23_30_410,"sms_makeAndSendMessage s_connList[i].port = %d")
TRACE_MSG(KJAVA_SYS_WMA_431_112_2_18_2_23_30_411,"kjava_wma_sendMessage sendRet = %d")
TRACE_MSG(KJAVA_SYS_WMA_536_112_2_18_2_23_30_412,"AppsmsCallback i = %d, event = %d, argv->dstPort = %d")
TRACE_MSG(KJAVA_SYS_WMA_552_112_2_18_2_23_30_413,"AppsmsCallback mode is KJAVA_WMA_CONNECTION_MODE_CLIENT")
TRACE_MSG(KJAVA_SYS_WMA_607_112_2_18_2_23_30_414,"Add2SMSReceiveQue Data type MN_SMS_8_BIT_ALPHBET")
TRACE_MSG(KJAVA_SYS_WMA_616_112_2_18_2_23_30_415,"Add2SMSReceiveQue Data type MN_SMS_UCS2_ALPHABET")
TRACE_MSG(KJAVA_SYS_WMA_785_112_2_18_2_23_31_416,"MMISMS_RegisterCallback()")
TRACE_MSG(KJAVA_SYS_WMA_787_112_2_18_2_23_31_417,"MMISMS_RegisterCallback() cb is %d, port is %d")
TRACE_MSG(KJAVA_SYS_WMA_816_112_2_18_2_23_31_418,"MMISMS_UnRegisterCallback() port is %d")
TRACE_MSG(KJAVA_SYS_WMA_827_112_2_18_2_23_31_419,"MMISMS_UnRegisterCallback() port found and clear")
END_TRACE_MAP(MMI_APP_IA_JAVA_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_IA_JAVA_TRC_H_

