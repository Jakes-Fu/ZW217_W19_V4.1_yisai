/******************************************************************************
 ** File Name:      mmi_app_setting_trc.h                                         *
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
//trace_id:221
#ifndef _MMI_APP_SETTING_TRC_H_
#define _MMI_APP_SETTING_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMISET_CALL_516_112_2_18_2_50_4_0 "[MMISET]: HandleSIMPlugNotifySetFunc dual_sys=%d, notify_event=%d"
#define MMISET_CALL_702_112_2_18_2_50_4_1 "HandlePsMsg:receive APP_MN_RELEASE_SS_IND (%d, %d, %d)"
#define MMISET_CALL_735_112_2_18_2_50_4_2 "HandlePsMsg:receive APP_MN_REGISTER_SS_CNF"
#define MMISET_CALL_755_112_2_18_2_50_4_3 "HandlePsMsg:receive APP_MN_ERASE_SS_CNF"
#define MMISET_CALL_774_112_2_18_2_50_4_4 "HandlePsMsg:receive APP_MN_ACTIVATE_SS_CNF"
#define MMISET_CALL_797_112_2_18_2_50_4_5 "HandlePsMsg:receive APP_MN_DEACTIVATE_SS_CNF"
#define MMISET_CALL_820_112_2_18_2_50_5_6 "HandlePsMsg:receive APP_MN_INTERROGATE_CNF (%d, %d, %d)"
#define MMISET_CALL_887_112_2_18_2_50_5_7 "HandleGetPasswordInd:guidance_info is %d!"
#define MMISET_CALL_891_112_2_18_2_50_5_8 "connect win is closed"
#define MMISET_CALL_1000_112_2_18_2_50_5_9 "HandleReleaseSsInd: error_present is %d and error_code is %d, %d, %d"
#define MMISET_CALL_1116_112_2_18_2_50_5_10 "UpdateCfuStatuaAfterOperate ss_code=%d,service_type=%d"
#define MMISET_CALL_1213_112_2_18_2_50_5_11 "HandleOperateSsCnf: result_type is %d,list num is = %d"
#define MMISET_CALL_1252_112_2_18_2_50_6_12 "HandleOperateSsCnf fail_num=%d, list_num=%d"
#define MMISET_CALL_1303_112_2_18_2_50_6_13 "HandleOperateSsCnf:error_code is %d"
#define MMISET_CALL_1451_112_2_18_2_50_6_14 "HandleInterrogateSsCnf: result_type is %d"
#define MMISET_CALL_1472_112_2_18_2_50_6_15 "HandleInterrogateSsCnf: get the status for cfu icon"
#define MMISET_CALL_1577_112_2_18_2_50_6_16 "GenInterrogateReport:list_num = %d"
#define MMISET_CALL_1590_112_2_18_2_50_6_17 "GenInterrogateReport:list_num = %d"
#define MMISET_CALL_1654_112_2_18_2_50_6_18 "GenDcsStatusReport:list_num = %d,service_code=%d"
#define MMISET_CALL_1671_112_2_18_2_50_6_19 "GenDcsStatusReport:service_type = %d,service_code=%d"
#define MMISET_CALL_1727_112_2_18_2_50_6_20 "GenInterrogateReport:list_num = %d"
#define MMISET_CALL_1826_112_2_18_2_50_7_21 "GenInterrogateReport:list_num = %d"
#define MMISET_CALL_1934_112_2_18_2_50_7_22 "GenInterrogateStatusReport:list_num = %d,service_code=%d"
#define MMISET_CALL_1950_112_2_18_2_50_7_23 "GenInterrogateStatusReport: info_list_ptr[0].ss_status_present is 0"
#define MMISET_CALL_1974_112_2_18_2_50_7_24 "GenInterrogateStatusReport: info_list_ptr[i].ss_status_present is 0"
#define MMISET_CALL_1990_112_2_18_2_50_7_25 "GenInterrogateStatusReport: no value"
#define MMISET_CALL_2017_112_2_18_2_50_7_26 "GenInterrogateVTReport:list_num = %d"
#define MMISET_CALL_2031_112_2_18_2_50_7_27 "GenInterrogateVTStatusReport: info_list_ptr[0].ss_status_present is 0"
#define MMISET_CALL_2044_112_2_18_2_50_7_28 "GenInterrogateVTStatusReport: i=%d, type=%d,code=%d"
#define MMISET_CALL_2054_112_2_18_2_50_7_29 "GenInterrogateVTStatusReport: info_list_ptr[i].ss_status_present is 0"
#define MMISET_CALL_2070_112_2_18_2_50_7_30 "GenInterrogateVTStatusReport: no value"
#define MMISET_CALL_2130_112_2_18_2_50_7_31 "GenInterrogateDetailReport:ss_code is 0x04%x, ss_status is 0x04%x"
#define MMISET_CALL_2815_112_2_18_2_50_9_32 "%d"
#define MMISET_CALL_2816_112_2_18_2_50_9_33 "SetSsParam: called_fuc_id is %d a, ss code = %d"
#define MMISET_CALL_2896_112_2_18_2_50_9_34 "GetCfuSimId cfu_dual_sys=%d"
#define MMISET_CALL_2951_112_2_18_2_50_9_35 "MMIAPISET_InterrogateCfuStatusFromPs:only one sim card"
#define MMISET_CALL_2956_112_2_18_2_50_9_36 "MMIAPISET_InterrogateCfuStatusFromPs:err_code=0x%x,cff1=%d,cff2=%d"
#define MMISET_CALL_3002_112_2_18_2_50_9_37 "MMIAPISET_InterrogateCfuStatusFromPs:get the cfu status from the network, ss_err_code=%d"
#define MMISET_CALL_3007_112_2_18_2_50_9_38 "MMIAPISET_InterrogateCfuStatusFromPs:has query already. do nothing"
#define MMISET_CALL_3079_112_2_18_2_50_9_39 "MMISET_SaveLineInfoToNV: the line type is %d"
#define MMISET_CALL_3085_112_2_18_2_50_9_40 "MMISET_SaveLineInfoToNV: read the line type is %d, the return value is %d"
#define MMISET_CALL_3491_112_2_18_2_50_10_41 "mmiset: query all cf start"
#define MMISET_CALL_3492_112_2_18_2_50_10_42 "mmiset: query all cfu"
#define MMISET_CALL_3556_112_2_18_2_50_10_43 "MMISET_CloseAllCallForwarding: group_id is %d and called_subfuc_id is %d"
#define MMISET_CALL_3819_112_2_18_2_50_11_44 "MMISET_CloseAllCallBar: group_id is %d and called_subfuc_id is %d"
#define MMISET_CALL_3905_112_2_18_2_50_11_45 "MMIAPISET_GetActiveIPNumber error len = %d num_len = %d"
#define MMISET_CALL_4081_112_2_18_2_50_11_46 "mmiset:MMIAPISET_ParsSsStringProc len=%d, str=%s"
#define MMISET_CALL_5099_112_2_18_2_50_13_47 "VirWin_HandleMsg: received SS repeat timer"
#define MMISET_CALL_5149_112_2_18_2_50_14_48 "MMIAPISET_BgInterrogateStatus: interrogate service"
#define MMISET_CALL_5175_112_2_18_2_50_14_49 "MMIAPISET_BgSetIgStatusParam bg_snd = %d, ss_snd = %d, g_is_for_cfu_icon = %d"
#define MMISET_CALL_5180_112_2_18_2_50_14_50 "MMIAPISET_BgSetIgStatusParam:isnot MMISET_DAULSYS_ALL_SIM_ON"
#define MMISET_CALL_5448_112_2_18_2_50_14_51 "mmiset:MMISET_RegisterDualsysCFNRC dual_sys = %d"
#define MMISET_CALL_5520_112_2_18_2_50_14_52 "mmiset: query all cfb"
#define MMISET_CALL_5531_112_2_18_2_50_14_53 "mmiset: query all cfnry"
#define MMISET_CALL_5542_112_2_18_2_50_14_54 "mmiset: query all cfnrc"
#define MMISET_CALL_5554_112_2_18_2_50_14_55 "mmiset: query all cf end"
#define MMISET_CALL_5576_112_2_18_2_50_14_56 "s_cf_info_ptr is null"
#define MMISET_CALL_5789_112_2_18_2_50_15_57 "mmivt GetFallbackHandleMode :%d ret:%d"
#define MMISET_CALL_5857_112_2_18_2_50_15_58 "MMISET_SetVTRecordRVDAPath error record_path %d"
#define MMISET_CALL_5893_112_2_18_2_50_15_59 "MMISET_SetVTRecordDAPath error device %d"
#define MMISET_CALL_5929_112_2_18_2_50_15_60 "MMISET_SetVTRecordRVPath error device %d"
#define MMISET_CALL_6032_112_2_18_2_50_15_61 "MMISET_IsSSProcessing g_is_background_search_send=%d, g_is_ss_user_query=%d, g_is_for_cfu_icon=%d,is_network_connecting=%d"
#define MMISET_CALL_6049_112_2_18_2_50_15_62 "MMIAPISET_SetSSStatus dual_sys=%d, ss_code=%d, service_type=%d, service_code=%d"
#define MMISET_CALL_6098_112_2_18_2_50_15_63 "MMISET_SetIsCfuActived dual_sys=%d, ss_type=%d, is_actived=%d"
#define MMISET_CALL_6133_112_2_18_2_50_15_64 "MMISET_GetIsCfuActived dual_sys=%d, ss_type=%d, is_ss_cfu_actived=%d"
#define MMISET_CALL_6201_112_2_18_2_50_16_65 "[MMISET]:MMIAPISET_GetMagicVoiceRes dat_ptr= 0x%x, data_len = %d"
#define MMISET_CALLWIN_3086_112_2_18_2_50_22_66 "mmisetting_assert ImplementCTAStandbyChanged"
#define MMISET_CALLWIN_3138_112_2_18_2_50_22_67 "mmisetting_assert ImplementCTASendSIMNotReadInd"
#define MMISET_CALLWIN_3766_112_2_18_2_50_23_68 "MMISET_SetDivertBarWatingHideidCtrlId ctrl_id = %d"
#define MMISET_CALLWIN_3777_112_2_18_2_50_23_69 "MMISET_GetDivertBarWatingHideidCtrlId ctrl_id = %d"
#define MMISET_CALLWIN_4491_112_2_18_2_50_25_70 "HandleGetVoiMessAddrCnf:enter divert voice box!"
#define MMISET_CALLWIN_4556_112_2_18_2_50_25_71 "HandleBarPasswordWindow s_call_bar_operation=%d"
#define MMISET_CALLWIN_4923_112_2_18_2_50_26_72 "HandleSelectNetworkWindow: select network mode is %d error!"
#define MMISET_CALLWIN_5100_112_2_18_2_50_26_73 "mmisetting_assert AppendIpNumberItem pos %d"
#define MMISET_CALLWIN_6023_112_2_18_2_50_28_74 "mmisetting_assert HandleVTReplaceImageSettingsWindow"
#define MMISET_CALLWIN_6045_112_2_18_2_50_28_75 "mmisetting_assert HandleVTReplaceImageSettingsWindow list_index %d"
#define MMISET_CALLWIN_6103_112_2_18_2_50_28_76 "IsReplaceImageValid : null"
#define MMISET_CALLWIN_6108_112_2_18_2_50_28_77 "IsReplaceImageValid width:%d height:%d size:%d"
#define MMISET_CALLWIN_6687_112_2_18_2_50_29_78 "HandleSelectNetworkMethodWindow: select network mode is %d error!"
#define MMISET_CALLWIN_6717_112_2_18_2_50_29_79 "HandleSelectNetworkMethodWindow: MNPHONE_SelectPLMN fail!"
#define MMISET_CALLWIN_6748_112_2_18_2_50_29_80 "HandleSelectNetworkMethodWindow: MNPHONE_ListPLMN fail!"
#define MMISET_CALLWIN_6869_112_2_18_2_50_29_81 "mmisetting_assert HandleSetAutoRedialWindow cur_selection %d"
#define MMISET_CALLWIN_6974_112_2_18_2_50_30_82 "mmisetting_assert SetSpeedDialDelay cur_selection %d"
#define MMISET_CALLWIN_7090_112_2_18_2_50_30_83 "MMIAPISET_SetTTSInfo cur_selection = %d"
#define MMISET_CALLWIN_8587_112_2_18_2_50_34_84 "HandleSetAnykeyRejectWindow: selected item is %d error!"
#define MMISET_CALLWIN_8908_112_2_18_2_50_35_85 "mmisetting_assert HandleSetFlipAnswerWindow cur_selection %d"
#define MMISET_CALLWIN_9243_112_2_18_2_50_36_86 "mmisetting_assert ImplementStandbyChanged"
#define MMISET_CALLWIN_9968_112_2_18_2_50_38_87 "mmisetting_assert HandleSetMajorSimWhenAllSimOnWindow e_major_sim_func %d"
#define MMISET_CALLWIN_10058_112_2_18_2_50_38_88 "mmisetting_assert HandleSetMajorSimWhenAllSimOnWindow e_major_sim_func %d"
#define MMISET_CALLWIN_10069_112_2_18_2_50_38_89 "mmiset:HandleSetMajorSimWhenAllSimOnWindow, cur_select=%d, e_major_sim_func=%d, e_major_sim_type = %d"
#define MMISET_CALLWIN_10088_112_2_18_2_50_38_90 "HandleSetMajorSimWhenAllSimOnWindow,dual_sys[%d]>=MMI_DUAL_SYS_MAX"
#define MMISET_CALLWIN_10453_112_2_18_2_50_39_91 "mmisetting_assert HandleEditSimNameWindow %d"
#define MMISET_CALLWIN_10546_112_2_18_2_50_39_92 "mmiset: HandleMultiSimSettingWindow wap"
#define MMISET_CALLWIN_10802_112_2_18_2_50_40_93 "mmiset_wintab.c:DispatchBySelectType, dual_sys = %d, select_type = %d"
#define MMISET_CALLWIN_11324_112_2_18_2_50_41_94 "mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin,True"
#define MMISET_CALLWIN_11330_112_2_18_2_50_41_95 "mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin:s_dualsys_cfnrc_win_close_time = %d"
#define MMISET_CALLWIN_11332_112_2_18_2_50_41_96 "mmiset:CloseDualsysCFNRCWin, s_dualsys_cfnrc_win_close_time=%d, action_result = %d"
#define MMISET_CALLWIN_11353_112_2_18_2_50_41_97 "MMISET_CloseDualsysCFNRCWin: is not dual sys, do not handle!"
#define MMISET_CALLWIN_11467_112_2_18_2_50_41_98 "MMISET_GetNetworkType gmmreg_rat is %d"
#define MMISET_CALLWIN_11507_112_2_18_2_50_41_99 "MMIAPISET_SetNetworkType rat=%d"
#define MMISET_CALLWIN_11553_112_2_18_2_50_41_100 "SetNetworkType cur_selection_netwoktype is %d, only_write_nv is %d"
#define MMISET_CALLWIN_11598_112_2_18_2_50_41_101 "SetNetworkType %d %d"
#define MMISET_CALLWIN_11802_112_2_18_2_50_42_102 "ResultDisp success: %d"
#define MMISET_CALLWIN_11807_112_2_18_2_50_42_103 "ResultDisp fail: %d"
#define MMISET_CALLWIN_11827_112_2_18_2_50_42_104 "ResultDisp success: %d"
#define MMISET_CALLWIN_11832_112_2_18_2_50_42_105 "ResultDisp fail: %d"
#define MMISET_CALLWIN_11914_112_2_18_2_50_42_106 "MMIAPISET_HandlePreferRatRsp rsp_result=%d"
#define MMISET_CALLWIN_11964_112_2_18_2_50_42_107 "IsinSwitchRatState is TRUE"
#define MMISET_CALLWIN_11969_112_2_18_2_50_42_108 "IsinSwitchRatState is FALSE"
#define MMISET_CALLWIN_12182_112_2_18_2_50_43_109 "HandleNetworkSettingWindow,msg_id=0x%x"
#define MMISET_CALLWIN_12247_112_2_18_2_50_43_110 "HandleDispPreferNetworkListWinMsg,msg=0x%x"
#define MMISET_CALLWIN_12272_112_2_18_2_50_43_111 "HandleDispPreferNetworkListWinMsg,max_list_num"
#define MMISET_CALLWIN_12304_112_2_18_2_50_43_112 "HandleDispPreferNetworkListWinMsg,total_item_num=%d "
#define MMISET_CALLWIN_12325_112_2_18_2_50_43_113 "HandleDispPreferNetworkListWinMsg,total_item_num=%d "
#define MMISET_CALLWIN_12387_112_2_18_2_50_43_114 "HandleDeletePreferPlmnQueryWindow,OK_Delete_Item= %d"
#define MMISET_CALLWIN_12411_112_2_18_2_50_43_115 "HandleDeletePreferPlmnQueryWindow,Delete Plmn: mcc=%d,mnc=%d,mnc_len=%d"
#define MMISET_CALLWIN_12449_112_2_18_2_50_43_116 "HandlePreferNetworkOptionMsg,msg_id=0x%x"
#define MMISET_CALLWIN_12457_112_2_18_2_50_43_117 "HandlePreferNetworkOptionMsg,total_list_num=%d,plmn_max_num=%d"
#define MMISET_CALLWIN_12497_112_2_18_2_50_43_118 "HandlePreferNetworkOptionMsg,total_list_num=%d,plmn_max_num=%d"
#define MMISET_CALLWIN_12603_112_2_18_2_50_44_119 "HandlePlmnListDispWinMsg,msg_id=0x%x"
#define MMISET_CALLWIN_12668_112_2_18_2_50_44_120 "HandlePlmnListDispWinMsg,Add Plmn: mcc=%d,mnc=%d,mnc_len=%d"
#define MMISET_CALLWIN_12702_112_2_18_2_50_44_121 "HandleCreatePlmnItemWinMsg,msg_id = 0x%x"
#define MMISET_CALLWIN_12842_112_2_18_2_50_44_122 "HandleCreatePlmnItemWinMsg,Add Plmn: mcc=%d,mnc=%d,mnc_len=%d"
#define MMISET_CALLWIN_12879_112_2_18_2_50_44_123 "HandlePreferNetworkPriorityChangeWinMsg,msg = 0x%x,cur_index=%d"
#define MMISET_CALLWIN_12955_112_2_18_2_50_44_124 "HandlePreferNetworkPriorityChangeWinMsg,index_priority=%d,cur_index=%d"
#define MMISET_CALLWIN_13026_112_2_18_2_50_45_125 "HandlePreferNetChangeWaitingWinMsg,msg=0x%x"
#define MMISET_CALLWIN_13131_112_2_18_2_50_45_126 "LoadMNPreferNetworkList,sim=%d,prefer_valid_num=%d,prefer_max_num=%d"
#define MMISET_CALLWIN_13171_112_2_18_2_50_45_127 "MMIAPISET_SavePreferNetworkList ERROR ! s_update_prefer_list_ptr == PNULL !"
#define MMISET_CALLWIN_13188_112_2_18_2_50_45_128 "MMIAPISET_SavePreferNetworkList,dual_sys=%d,prefer_valid_num=%d,prefer_max_num=%d"
#define MMISET_CALLWIN_13207_112_2_18_2_50_45_129 "MMIAPISET_HandlePreferNetworkListCnf,sim=%d,update_sucess=%d"
#define MMISET_DATETIME_642_112_2_18_2_50_47_130 "DisplaySystemDate sys_date.year = %d"
#define MMISET_DATETIME_1392_112_2_18_2_50_48_131 "Waring, UpdateStbDispDate: month = %d"
#define MMISET_DATETIME_1397_112_2_18_2_50_48_132 "Waring,UpdateStbDispDate: date = %d"
#define MMISET_DATETIME_1995_112_2_18_2_50_49_133 "SetTime:hour is %d,minute is %d is invalid!"
#define MMISET_DATETIME_2215_112_2_18_2_50_50_134 "DisplaySystemDate:date_type %d is error!"
#define MMISET_DATETIME_2233_112_2_18_2_50_50_135 "DisplaySystemDate sys_date.year = %d"
#define MMISET_DATETIME_2287_112_2_18_2_50_50_136 "SetDate sys_date.year = %d"
#define MMISET_DATETIME_2294_112_2_18_2_50_50_137 "SetDate:year is %d,month is %d and day is %d is invalid!"
#define MMISET_DISPLAY_644_112_2_18_2_50_52_138 "mmisetting_assert MMIAPISET_GetWallpaperSetInfo"
#define MMISET_DISPLAY_742_112_2_18_2_50_52_139 "mmisetting_assert MMIAPISET_GetBackLightInfo"
#define MMISET_DISPLAY_812_112_2_18_2_50_52_140 "MMIAPISET_UpdateLCDContrast:s_real_contrast = %d"
#define MMISET_DISPLAY_990_112_2_18_2_50_53_141 "mmisetting_assert MMIAPISET_SetTVOutMmode cur_selection %d"
#define MMISET_DISPLAY_1126_112_2_18_2_50_53_142 "MMIAPISET_SetTTSInfo cur_selection = %d"
#define MMISET_DISPLAY_1230_112_2_18_2_50_53_143 "mmisetting_assert MMIAPISET_SetMainmenuStyle"
#define MMISET_DISPLAY_1891_112_2_18_2_50_55_144 "MMISET_AppendBacklightListItem:backlight time is %d error!"
#define MMISET_DISPLAY_1905_112_2_18_2_50_55_145 "MMISET_AppendBacklightListItem:backlight type is %d error!"
#define MMISET_DISPLAY_1966_112_2_18_2_50_55_146 "MMISET_SetIdleDisplayMode: selected item is %d error!"
#define MMISET_DISPLAY_2003_112_2_18_2_50_55_147 "mmisetting_assert MMISET_SetBackLight cur_selection %d"
#define MMISET_DISPLAY_2167_112_2_18_2_50_55_148 "MMISET_TuneUpConstrast:msg_id %d is error!"
#define MMISET_DISPLAY_2170_112_2_18_2_50_55_149 "MMISET_TuneUpConstrast:s_cur_contrast is %d!"
#define MMISET_DISPLAY_2221_112_2_18_2_50_55_150 "mmisetting_assert MMIAPISET_GetScreensaverInfo"
#define MMISET_DISPLAY_2414_112_2_18_2_50_56_151 "mmisetting_assert MMISET_Set3DStyleMainmenu cur_selection %d"
#define MMISET_DISPLAY_2455_112_2_18_2_50_56_152 "mmisetting_assert MMISET_Set3DStyleScreen cur_selection %d"
#define MMISET_DISPLAY_2558_112_2_18_2_50_56_153 "mmisetting_assert MMISET_SetGsensorSwitchStyle g_sensor_anim %d"
#define MMISET_DISPLAY_2610_112_2_18_2_50_56_154 "mmisetting_assert Get3DStyle"
#define MMISET_DISPLAY_2638_112_2_18_2_50_56_155 "mmisetting_assert Set3DStyle"
#define MMISET_DISPLAY_2682_112_2_18_2_50_56_156 "mmisetting_assert MMISET_SetIdleStyle idle_style %d"
#define MMISET_DISPLAY_2873_112_2_18_2_50_57_157 "Waring, MMIAPISET_FormatDateStrByDateStyle: month = %d"
#define MMISET_DISPLAY_2878_112_2_18_2_50_57_158 "Waring,MMIAPISET_FormatDateStrByDateStyle: date = %d"
#define MMISET_DISPLAY_2883_112_2_18_2_50_57_159 "Waring,MMIAPISET_FormatDateStrByDateStyle: in len = %d,min len=%d"
#define MMISET_DISPLAY_2929_112_2_18_2_50_57_160 "Waring, MMIAPISET_FormatDateStrByDateEx: month = %d"
#define MMISET_DISPLAY_2934_112_2_18_2_50_57_161 "Waring,MMIAPISET_FormatDateStrByDateEx: date = %d"
#define MMISET_DISPLAY_2939_112_2_18_2_50_57_162 "Waring,MMIAPISET_FormatDateStrByDateEx: in len = %d,min len=%d"
#define MMISET_DISPLAY_3131_112_2_18_2_50_57_163 "SetSlideOpenType slide_open_type = %d"
#define MMISET_DISPLAY_3188_112_2_18_2_50_57_164 "SetSlideCloseType slide_close_type = %d"
#define MMISET_DISPLAY_3245_112_2_18_2_50_57_165 "SetKeylockSwitchType keylock_switch_type = %d"
#define MMISET_DISPLAY_3297_112_2_18_2_50_57_166 "mmisetting_assert IsValid3DStyle"
#define MMISET_DISPLAY_3307_112_2_18_2_50_58_167 "IsValid3DStyle() NV abnormal!"
#define MMISET_DISPLAYWIN_739_112_2_18_2_51_0_168 "SetAnimLabelParam"
#define MMISET_DISPLAYWIN_745_112_2_18_2_51_0_169 "SetThemeParam"
#define MMISET_DISPLAYWIN_862_112_2_18_2_51_0_170 "HandleSetDisplayWindow ctrl_id = %d"
#define MMISET_DISPLAYWIN_1257_112_2_18_2_51_1_171 "AppendMainmenuStyle:mainmenu_style %d is error!"
#define MMISET_DISPLAYWIN_1366_112_2_18_2_51_1_172 "HandleIdleMsg:idle_style %d is error!"
#define MMISET_DISPLAYWIN_1486_112_2_18_2_51_1_173 "MMISET_AppendBacklightListItem:backlight time is %d error!"
#define MMISET_DISPLAYWIN_1500_112_2_18_2_51_1_174 "MMISET_AppendBacklightListItem:backlight type is %d error!"
#define MMISET_DISPLAYWIN_1744_112_2_18_2_51_2_175 "HandleSetDisplayAnimationWindow ctrl_id = %d"
#define MMISET_DISPLAYWIN_2609_112_2_18_2_51_4_176 "mmisetting_assert MMIAPISET_GetPowerOnOffAnimInfo"
#define MMISET_DISPLAYWIN_3150_112_2_18_2_51_5_177 "HandleSetConstrastWindow cur_constrast = %d"
#define MMISET_DISPLAYWIN_3155_112_2_18_2_51_5_178 "HandleSetConstrastWindow cur_constrast = %d"
#define MMISET_DISPLAYWIN_3679_112_2_18_2_51_6_179 "SetMainmenuStyle:cur_selection %d is error!"
#define MMISET_DISPLAYWIN_3747_112_2_18_2_51_6_180 "SetIdleStyle:cur_index %d is error!"
#define MMISET_FUNC_953_112_2_18_2_51_8_181 "mmisetting_assert InitPrivacyProtectPwd %d"
#define MMISET_FUNC_981_112_2_18_2_51_8_182 "mmisetting_assert InitPhonePwd %d"
#define MMISET_FUNC_1008_112_2_18_2_51_8_183 "mmisetting_assert MMIAPISET_SetByOpenCloseList cur_selection %d"
#define MMISET_FUNC_1498_112_2_18_2_51_9_184 "mmisetting_assert MMIAPISET_GetLanguageType"
#define MMISET_FUNC_1518_112_2_18_2_51_9_185 "MMIAPISET_SetLanguageType language_type=%d, not exist"
#define MMISET_FUNC_1598_112_2_18_2_51_9_186 "mmisetting_assert MMIAPISET_GetKeyBoardLightTime"
#define MMISET_FUNC_1661_112_2_18_2_51_9_187 "MMISET_CleanUserData: enter."
#define MMISET_FUNC_1699_112_2_18_2_51_9_188 "MMISET_CleanUserData delete result = %d"
#define MMISET_FUNC_1707_112_2_18_2_51_10_189 "MMISET_CleanUserData: Success Exit."
#define MMISET_FUNC_2183_112_2_18_2_51_10_190 "[MMISET] MMIAPISET_SetWaitFlag"
#define MMISET_FUNC_2193_112_2_18_2_51_10_191 "[MMISET] MMIAPISET_FuncFinishedInd"
#define MMISET_FUNC_2207_112_2_18_2_51_11_192 "mmisetting_assert GetValidIdxByLangIdx %d"
#define MMISET_FUNC_2236_112_2_18_2_51_11_193 "mmisetting_assert MMIAPISET_IsSelectedLanguageValid language_type %d"
#define MMISET_FUNC_2329_112_2_18_2_51_11_194 "mmisetting_assert MMISET_AppendLanguageListItem language_type %d"
#define MMISET_FUNC_2339_112_2_18_2_51_11_195 "mmisetting_assert MMISET_AppendLanguageListItem language_type %d"
#define MMISET_FUNC_2391_112_2_18_2_51_11_196 "mmisetting_assert MMIAPISET_AppendInputLanguageListItem language_type %d"
#define MMISET_FUNC_2460_112_2_18_2_51_11_197 "mmisetting_assert MMIAPISET_GetInputLanguageString language_type %d"
#define MMISET_FUNC_2467_112_2_18_2_51_11_198 "mmisetting_assert MMIAPISET_GetInputLanguageString 2"
#define MMISET_FUNC_2549_112_2_18_2_51_11_199 "mmisetting_assert MMISET_SetKeyboardLight cur_selection %d"
#define MMISET_FUNC_2600_112_2_18_2_51_11_200 "MMISET_SetKeyboardLight: cur_selection %d is error!"
#define MMISET_FUNC_2645_112_2_18_2_51_11_201 "mmisetting_assert MMISET_SetMoveStyle cur_selection %d"
#define MMISET_FUNC_2666_112_2_18_2_51_11_202 "MMISET_SetMoveStyle: cur_selection %d is error!"
#define MMISET_FUNC_2696_112_2_18_2_51_11_203 "MMISET_AppendSelectNetworkListItem:select network mode is %d!,dual_sys = %d"
#define MMISET_FUNC_2749_112_2_18_2_51_12_204 "mmisetting_assert MMISET_SetSelectNetworkMode cur_selection %d"
#define MMISET_FUNC_2781_112_2_18_2_51_12_205 "MMISET_SetSelectNetworkMode: cur_selection %d is error!"
#define MMISET_FUNC_2803_112_2_18_2_51_12_206 "mmisetting_assert MMIAPISET_IsOpenPrivacyProtect protect_type %d"
#define MMISET_FUNC_3052_112_2_18_2_51_12_207 "mmisetting_assert MMIAPISET_OpenPlmnListWin"
#define MMISET_FUNC_3082_112_2_18_2_51_12_208 "mmisetting_assert AppendPlmnListItem"
#define MMISET_FUNC_3094_112_2_18_2_51_12_209 "mmisetting_assert AppendPlmnListItem"
#define MMISET_FUNC_3336_112_2_18_2_51_13_210 "MMISET_SetAnykeyRejectMode: selected item is error!"
#define MMISET_FUNC_3362_112_2_18_2_51_13_211 "mmisetting_assert MMISET_SelectNetwork cur_selection %d"
#define MMISET_FUNC_3370_112_2_18_2_51_13_212 "MMISET_SelectNetwork: mcc = %d, mnc = %d, mnc_digit_num = %d, select dual sys = %d"
#define MMISET_FUNC_3379_112_2_18_2_51_13_213 "MMISET_SelectNetwork: MNPHONE_SelectPLMN fail!"
#define MMISET_FUNC_3476_112_2_18_2_51_13_214 "mmisetting_assert MMISET_SetSelectBandType cur_selection %d"
#define MMISET_FUNC_3490_112_2_18_2_51_13_215 "MMISET_SetSelectBandType: cur_selection %d is error!"
#define MMISET_FUNC_3702_112_2_18_2_51_13_216 "MMIAPISET_GetFlyModeSetPowerOn is_fly_mode_power_on=%d"
#define MMISET_FUNC_3796_112_2_18_2_51_14_217 "MMIAPISET_IsNeedResetDateTime is_rtcAbnormal = %d"
#define MMISET_FUNC_3827_112_2_18_2_51_14_218 "GetMulNVConfigInfo fail! reslut = %d"
#define MMISET_FUNC_3871_112_2_18_2_51_14_219 "MMIAPISET_CheckLanguageStatus language_type = %d,result=%d"
#define MMISET_FUNC_3950_112_2_18_2_51_14_220 "mmisetting_assert MMISET_AppendShowLanguageItem language_type %d"
#define MMISET_FUNC_3978_112_2_18_2_51_14_221 "mmisetting_assert MMISET_AppendInputLanguageItem language_type %d"
#define MMISET_FUNC_4226_112_2_18_2_51_15_222 "MMISET_GetInputLanguageContent"
#define MMISET_FUNC_4307_112_2_18_2_51_15_223 "MMIAPISET_RestoreDataTimeFactorySetting"
#define MMISET_FUNC_4779_112_2_18_2_51_16_224 "MMISET_AllocAndInitLanguageInfo valid language is %d"
#define MMISET_FUNC_4897_112_2_18_2_51_16_225 "MMIAPISET_ConvertNVLangToSysType(set_lang = %d) BEGIN:dual_sys = %d"
#define MMISET_FUNC_4905_112_2_18_2_51_16_226 "MMIAPISET_ConvertNVLangToSysType,get auto_lang = %d"
#define MMISET_FUNC_4928_112_2_18_2_51_16_227 "MMIAPISET_ConvertNVLangToSysType END: cur_lang = %d,nv_lang=%d"
#define MMISET_FUNC_4956_112_2_18_2_51_16_228 "MMIAPISET_GetPreferLanguageTypeFromSim Begin :sim=%d, [elp_lst_isvalid=%d and num=%d],[lp_isvalid=%d and num=%d],[elp_2nd_valid= %d and num=%d]"
#define MMISET_FUNC_4971_112_2_18_2_51_16_229 "GetPreferedLanguage 1st_elp:i=%d,j=%d,mmi_type=%d,sim_code=[0x%x,0x%x]"
#define MMISET_FUNC_4994_112_2_18_2_51_16_230 "GetPreferedLanguage lp :i=%d,j=%d,mmi_type=%d,sim_code=0x%x"
#define MMISET_FUNC_5017_112_2_18_2_51_16_231 "GetPreferedLanguage 2nd_elp:i=%d,j=%d,mmi_type=%d,sim_code=[0x%x,0x%x]"
#define MMISET_LOCK_EFFECT_335_112_2_18_2_51_17_232 "HandleScreenLockTypeSetWinMsg, msg_id = 0x%x"
#define MMISET_LOCK_EFFECT_1049_112_2_18_2_51_19_233 "MMIAPISET_GetKeyLockStyle: style =%d, is invalid!!!"
#define MMISET_LONGPOWER_114_112_2_18_2_51_19_234 "HandleBlacklistOptWinMsg, msg_id = 0x%x"
#define MMISET_MAINMENU_521_112_2_18_2_51_21_235 "GetMainmenuStyleItemIndex error type = %d"
#define MMISET_MAINMENU_597_112_2_18_2_51_21_236 "GetMainmenuStyleByIndex error item_index = %d"
#define MMISET_MAINMENU_671_112_2_18_2_51_21_237 "mmisetting_assert MoveSetingPreview FALSE"
#define MMISET_MAINMENU_753_112_2_18_2_51_21_238 "mmisetting_assert ResumeSetingPreview 0"
#define MMISET_MAINMENU_854_112_2_18_2_51_21_239 "HandleMainMenuTypeSetWinMsg, msg_id = 0x%x"
#define MMISET_MAINMENU_927_112_2_18_2_51_22_240 "HandleMainMenuTypeSetWinMsg, msg_id = 0x%x"
#define MMISET_MAINMENU_1124_112_2_18_2_51_22_241 "HandleMainMenuTypePreviewWinMsg, msg_id = 0x%x"
#define MMISET_MAINMENU_1357_112_2_18_2_51_23_242 "GetMainmenuStyleItemIndex error type = %d"
#define MMISET_MAINMENU_1476_112_2_18_2_51_23_243 "GetMainmenuStyleByIndex error item_index = %d"
#define MMISET_MAINMENU_1530_112_2_18_2_51_23_244 "HandleGOMenuTypeSetWinMsg, msg_id = 0x%x"
#define MMISET_MULTISIM_260_112_2_18_2_51_26_245 "MMIAPISET_GetSimName error dual_sys = %d"
#define MMISET_MULTISIM_377_112_2_18_2_51_26_246 "MMIAPISET_GetSimColorID error dual_sys = %d"
#define MMISET_MULTISIM_404_112_2_18_2_51_26_247 "MMIAPISET_GetSimColor error dual_sys = %d"
#define MMISET_MULTISIM_500_112_2_18_2_51_27_248 "MMIAPISET_GetSimColor error dual_sys = %d"
#define MMISET_MULTISIM_561_112_2_18_2_51_27_249 "MMISET_SetMajorSimSelType  MMISET_SetMajorSimSelType =%d, e_sim_sel_type= %d"
#define MMISET_MULTISIM_970_112_2_18_2_51_28_250 "MMIAPISET_GetAnswerWithOriginalSimType   e_func_type=%d"
#define MMISET_MULTISIM_989_112_2_18_2_51_28_251 "MMISET_SetMultiSysSetting   e_dual_sys=%d"
#define MMISET_MULTISIM_1027_112_2_18_2_51_28_252 "MMIAPISET_GetMultiSysSetting e_dual_sys=%d"
#define MMISET_MULTISIM_1139_112_2_18_2_51_28_253 "mmiset:MMIAPISET_GetMajorSim e_major_sim_type=%d"
#define MMISET_MULTISIM_1190_112_2_18_2_51_28_254 "mmiset:MMIAPISET_GetMajorSim e_major_sim_type=%d"
#define MMISET_PHONEWIN_1126_112_2_18_2_51_33_255 "HandleSetSlideOpenTypeWindow cur_selection = %d"
#define MMISET_PHONEWIN_1191_112_2_18_2_51_33_256 "HandleSetSlideCloseTypeWindow cur_selection = %d"
#define MMISET_PHONEWIN_1255_112_2_18_2_51_33_257 "HandleSetKeylockSwitchTypeWindow cur_selection = %d"
#define MMISET_PHONEWIN_1823_112_2_18_2_51_34_258 "HandleSetSensorWindow sensor_data = %d,is_sensor_mute = %d,is_sensor_change_wallpaper = %d"
#define MMISET_PHONEWIN_2393_112_2_18_2_51_35_259 "MMIAPISET_EnterPhoneSettingWin:mainmenu_style %d is error!"
#define MMISET_PHONEWIN_2550_112_2_18_2_51_35_260 "SetPowerSaveParam PNULL == mtext_str.wstr_ptr"
#define MMISET_PHONEWIN_2579_112_2_18_2_51_35_261 "SetPowerSaveParam PNULL == mtext_str.wstr_ptr"
#define MMISET_PHONEWIN_2603_112_2_18_2_51_35_262 "SetPowerSaveParam SCI_ALLOCA error!"
#define MMISET_PHONEWIN_2715_112_2_18_2_51_36_263 "MMIAPISET_GetPowerSavingSetting nv_item = %d"
#define MMISET_PHONEWIN_2850_112_2_18_2_51_36_264 "HandlePowerSaveSettingWindow list1 index = %d"
#define MMISET_PHONEWIN_2887_112_2_18_2_51_36_265 "HandlePowerSaveSettingWindow list2 index = %d"
#define MMISET_PHONEWIN_3163_112_2_18_2_51_37_266 "MMIAPISET_IsProgramRunning bt is open."
#define MMISET_PHONEWIN_3173_112_2_18_2_51_37_267 "MMIAPISET_IsProgramRunning wifi is open."
#define MMISET_PHONEWIN_3182_112_2_18_2_51_37_268 "MMIAPISET_IsProgramRunning java is open."
#define MMISET_PHONEWIN_3191_112_2_18_2_51_37_269 "MMIAPISET_IsProgramRunning browser is open."
#define MMISET_RING_349_112_2_18_2_51_38_270 "[MMISET]:MMIAPISET_GetMultimVolume VOL=%d"
#define MMISET_RING_377_112_2_18_2_51_38_271 "[MMISET]:MMIAPISET_SetMultimVolume error!! VOL=%d"
#define MMISET_RING_523_112_2_18_2_51_39_272 "MMIAPISET_SetVibratorEx: is_open = %d, ring_type = %d,is_period = %d, duration = %d"
#define MMISET_RING_625_112_2_18_2_51_39_273 "MMIAPISET_PlayRing:NO PERMITPLAYRING"
#define MMISET_RING_664_112_2_18_2_51_39_274 "MMIAPISET_PlayRing:NO PERMITPLAYRING"
#define MMISET_RING_689_112_2_18_2_51_39_275 "[MMISET]:PlayFixedRing, ring_type = 0x%x"
#define MMISET_RING_699_112_2_18_2_51_39_276 "PlayFixedRing ring_id = %d is error!"
#define MMISET_RING_776_112_2_18_2_51_40_277 "[MMISET]:PlayFixedRing, ring_type = 0x%x"
#define MMISET_RING_780_112_2_18_2_51_40_278 "PlayFixedRing ring_id = %d is error!"
#define MMISET_RING_888_112_2_18_2_51_40_279 "mmisetting_assert PlayRing CALL"
#define MMISET_RING_907_112_2_18_2_51_40_280 "mmisetting_assert PlayRing MSG"
#define MMISET_RING_984_112_2_18_2_51_40_281 "[MMISET]:MMI_APISET_LivePaperPlayRing, not PermitPlayRing"
#define MMISET_RING_1036_112_2_18_2_51_40_282 "mmisetting_assert MMIAPISET_GetDeviceMode"
#define MMISET_RING_1088_112_2_18_2_51_40_283 "[MMISET]: MMIAPISET_StopRing enter--ring_type is %d and g_is_playing_ring_type is %d,s_cur_audio_htype = %d"
#define MMISET_RING_1159_112_2_18_2_51_41_284 "MMIAPISET_StopAllRing s_cur_audio_htype= %d"
#define MMISET_RING_1256_112_2_18_2_51_41_285 "MMISET_SetCallRingType:call_ring_type is %d!"
#define MMISET_RING_2084_112_2_18_2_51_43_286 "MMIAPISET_GetCallRingInfo enter"
#define MMISET_RING_2096_112_2_18_2_51_43_287 "MMIAPISET_GetCallRingInfo: the more call ring is not exited"
#define MMISET_RING_2101_112_2_18_2_51_43_288 "MMIAPISET_GetCallRingInfo out"
#define MMISET_RING_2327_112_2_18_2_51_43_289 "mmisetting_assert GetRingInfo"
#define MMISET_RING_2495_112_2_18_2_51_44_290 "GetRingInfo:ring_type is error!"
#define MMISET_RING_2927_112_2_18_2_51_45_291 "MMIAPISET_PlayMusicFile param error!"
#define MMISET_RING_3015_112_2_18_2_51_45_292 "MMIAPISET_PlayMusicFile, ring_type= %d, play_times = %d, vol = %d, call_back = 0x%x,music_type = %d, result = %d"
#define MMISET_RING_3051_112_2_18_2_51_45_293 "MMIAPISET_PlayMusicFile param error!"
#define MMISET_RING_3132_112_2_18_2_51_45_294 "MMIAPISET_PlayMusicFile, ring_type= %d, play_times = %d, vol = %d, call_back = 0x%x,music_type = %d, result = %d"
#define MMISET_RING_3178_112_2_18_2_51_45_295 "[MMISET]:HandleReceiverMode, ring_type = %d, volume = %d, is_through_receiver = %d"
#define MMISET_RING_3247_112_2_18_2_51_45_296 "[MMISET]:MMIAPISET_IsThroughReceiver, ring_type = %d"
#define MMISET_RING_3274_112_2_18_2_51_46_297 "[MMISET]: MMIAPISET_InitAudioDeviceExt: ring_type = %d,volume=%d"
#define MMISET_RING_3305_112_2_18_2_51_46_298 "[MMISET]: MMIAPISET_InitBtAudioDevice: is_bt_play = %d, volume=%d"
#define MMISET_RING_3345_112_2_18_2_51_46_299 "[MMISET]: MMIAPISET_InitAudioDevice: volume=%d"
#define MMISET_RING_3594_112_2_18_2_51_46_300 "MMIAPISET_IsPermitPlayRing: ring_type=0x%x, g_is_playing_ring_type = 0x%x,s_is_vibrate=0x%x, s_is_playing_vibrate_type=0x%x,result= 0x%x"
#define MMISET_RING_3777_112_2_18_2_51_47_301 "PlayGenericTone:is_dsp_play_ring is FALSE"
#define MMISET_RING_4086_112_2_18_2_51_47_302 "[MMISET]:MMIAPISET_SetCurRingType, ring_type = %d"
#define MMISET_RING_4096_112_2_18_2_51_47_303 "[MMISET]:MMIAPISET_ClearCurRingType, ring_type = %d, g_is_playing_ring_type = %d"
#define MMISET_RING_4120_112_2_18_2_51_47_304 "[MMISET]:MMIAPISET_IsCurRingType, ring_type=%d, g_ring=%d, s_cur_app_handle=0x%x"
#define MMISET_RING_4136_112_2_18_2_51_47_305 "[MMISET]:MMIAPISET_IsCurRingType, ring_type = %d, g_ring = %d, s_audio = %d, result = %d"
#define MMISET_RING_4195_112_2_18_2_51_47_306 "[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d"
#define MMISET_RING_4406_112_2_18_2_51_48_307 "[MMISET]: HandleAudioCallBack() entry, handle=0x%x, param=0x%x"
#define MMISET_RING_4414_112_2_18_2_51_48_308 "[MMISRV]: HandleNotify(), report_ptr->report=%d"
#define MMISET_SCREENSAVER_422_112_2_18_2_51_49_309 "mmisetting_assert HandleScreensaverPreviewWindow sel_item_index %d"
#define MMISET_SECURITY_1955_112_2_18_2_51_54_310 "mmiset cur_selection = %d exceed MMISET_KEYLOCK_MAX_ITEM "
#define MMISET_SECURITY_3523_112_2_18_2_51_57_311 "AppendSecuritySetListItem:mode=%d"
#define MMISET_SECURITY_3731_112_2_18_2_51_58_312 "mmiset pwd_ptr == PNULL"
#define MMISET_SECURITY_3761_112_2_18_2_51_58_313 "mmiset pwd_ptr == PNULL"
#define MMISET_SECURITY_3879_112_2_18_2_51_58_314 "mmiset pwd_ptr== PNULL"
#define MMISET_SECURITY_4159_112_2_18_2_51_58_315 "[MMIPHONE]InitSIMLockMenuControl get sim lock data fail!!"
#define MMISET_SECURITY_4249_112_2_18_2_51_59_316 "[MMIPHONE]UpdateSIMLockEditControl get sim lock data fail!!"
#define MMISET_SECURITY_4720_112_2_18_2_52_0_317 "AppendAutoKeylockTimeListItem:keylock type is %d error!"
#define MMISET_SECURITY_4747_112_2_18_2_52_0_318 "mmisetting_assert SetAudoKeyLockTime cur_selection %d"
#define MMISET_SECURITY_5569_112_2_18_2_52_1_319 "start PHONEBOOK_SetFDNService true"
#define MMISET_SECURITY_5574_112_2_18_2_52_1_320 "start PHONEBOOK_SetFDNService false"
#define MMISET_SECURITY_5581_112_2_18_2_52_1_321 "HandleSetFDNStateWindow err_code=%d"
#define MMISET_SECURITY_5712_112_2_18_2_52_2_322 "start PHONEBOOK_SetFDNService true"
#define MMISET_SECURITY_5717_112_2_18_2_52_2_323 "start PHONEBOOK_SetFDNService false"
#define MMISET_SECURITY_5724_112_2_18_2_52_2_324 "HandleSetFDNStateWindow err_code=%d"
#define MMISET_SECURITY_5869_112_2_18_2_52_2_325 "start PHONEBOOK_SetFDNService true"
#define MMISET_SECURITY_5874_112_2_18_2_52_2_326 "start PHONEBOOK_SetFDNService false"
#define MMISET_SECURITY_5883_112_2_18_2_52_2_327 "HandleSetFDNStateWindow err_code=%d"
#define MMISET_SECURITY_6277_112_2_18_2_52_3_328 "OperatePowerOnPwd: operate_type %d is error!"
#define MMISET_SECURITY_7496_112_2_18_2_52_5_329 "HandleBlacklistOptWinMsg, msg_id = 0x%x"
#define MMISET_SECURITY_8514_112_2_18_2_52_7_330 "MMISET: MMIAPISET_AddBlacklistFromExt,pb_name_ptr ||pb_num_ptr is NULL"
#define MMISET_SHORTCUT_766_112_2_18_2_52_10_331 "mmisetting_assert ResetDirectionKeyInfo"
#define MMISET_SHORTCUT_795_112_2_18_2_52_10_332 "mmisetting_assert GetDirectionKeyInfo"
#define MMISET_SHORTCUT_820_112_2_18_2_52_10_333 "mmisetting_assert GetDirectionKey index %d"
#define MMISET_SHORTCUT_847_112_2_18_2_52_10_334 "mmisetting_assert SetDirectionKey index %d"
#define MMISET_SHORTCUT_874_112_2_18_2_52_10_335 "mmisetting_assert SetIdleShortcutInfo index %d"
#define MMISET_SHORTCUT_932_112_2_18_2_52_10_336 "mmisetting_assert ResetIdleShortcutInfo"
#define MMISET_SHORTCUT_986_112_2_18_2_52_10_337 "mmisetting_assert GetIdleShortcutInfo"
#define MMISET_SHORTCUT_1015_112_2_18_2_52_10_338 "mmisetting_assert GetEnabledFuncKeyShortcutInfo"
#define MMISET_SHORTCUT_1048_112_2_18_2_52_10_339 "mmisetting_assert SetFuncKeyShortcutInfo index %d, type %d"
#define MMISET_SHORTCUT_1070_112_2_18_2_52_10_340 "mmisetting_assert GetFunctionKey index %d"
#define MMISET_SHORTCUT_1094_112_2_18_2_52_10_341 "MMIAPISET_OpenDirectionKeyWin, dcd is open and skip the key msg!"
#define MMISET_SHORTCUT_1100_112_2_18_2_52_10_342 "MMIAPISET_OpenDirectionKeyWin selected = %d, index = %d"
#define MMISET_SHORTCUT_1280_112_2_18_2_52_11_343 "MMIAPISET_OpenFuncKeyWin "
#define MMISET_SHORTCUT_1491_112_2_18_2_52_11_344 "mmisetting_assert GetShorcutActionTypeAndOffset"
#define MMISET_THEME_143_112_2_18_2_52_13_345 "mmisetting_assert AppendFileTheme"
#define MMISET_THEME_256_112_2_18_2_52_13_346 "mmisetting_assert MMIAPISET_IniCurrentTheme"
#define MMISET_VIDEOWALLPAPER_383_112_2_18_2_52_15_347 "[MMISETVWP]:GetNextVideo total_num ERROR!!!!!!!"
#define MMISET_VIDEOWALLPAPER_407_112_2_18_2_52_15_348 "[MMISETVWP]:GetNextVideo h=%d,w=%d,v_support=%d,video_type=%d,index=%d"
#define MMISET_VIDEOWALLPAPER_441_112_2_18_2_52_15_349 "[MMISETVWP]:GetNextVideo h=%d,w=%d,v_support=%d,video_type=%d,index=%d"
#define MMISET_VIDEOWALLPAPER_463_112_2_18_2_52_15_350 "[MMISETVWP]:GetNextVideo no support video!"
#define MMISET_VIDEOWALLPAPER_815_112_2_18_2_52_16_351 "[MMISETVWP]: HandleVideoListWinMsg need_item_data_ptr is null"
#define MMISET_VIDEOWALLPAPER_1024_112_2_18_2_52_16_352 "[MMISETVWP]: VideoList_AddItem"
#define MMISET_VIDEOWALLPAPER_1272_112_2_18_2_52_17_353 "[MMISETVWP]:VideoListWriteFile FAIL, fetal error!"
#define MMISET_VIDEOWALLPAPER_1284_112_2_18_2_52_17_354 "[MMISETVWP]:VideoListWriteFile ERROR!!!!!!!"
#define MMISET_VIDEOWALLPAPER_1356_112_2_18_2_52_17_355 "[MMISETVWP]:VideoListReadFile ERROR!!!!!!!"
#define MMISET_VIDEOWALLPAPER_1363_112_2_18_2_52_17_356 "[MMISETVWP]:VideoListReadFile empty!"
#define MMISET_VIDEOWALLPAPER_1518_112_2_18_2_52_17_357 "[MMISETVWP]: GetVideoItemByIndex fail!"
#define MMISET_VIDEOWALLPAPER_1567_112_2_18_2_52_17_358 "[MMISETVWP]:SetVideoRandomSetting is_random=%d"
#define MMISET_VIDEOWALLPAPER_1578_112_2_18_2_52_17_359 "[MMISETVWP]:GetVideoAudioSetting is_audio_support=%d"
#define MMISET_VIDEOWALLPAPER_1588_112_2_18_2_52_17_360 "[MMISETVWP]:SetVideoAudioSetting is_audio_support=%d"
#define MMISET_VIDEOWALLPAPER_1600_112_2_18_2_52_17_361 "[MMISETVWP]:CheckAllVideoIsInValid entry"
#define MMISET_VIDEOWALLPAPER_1627_112_2_18_2_52_17_362 "[MMISETVWP] HandleDynWallpaperPlayMsg msg_ptr->msg_id = %d"
#define MMISET_VIDEOWALLPAPER_1633_112_2_18_2_52_17_363 "[MMISETVWP] HandleDynWallpaperPlayMsg play_end_result = %d"
#define MMISET_VIDEOWALLPAPER_1693_112_2_18_2_52_17_364 "[MMISETVWP]:GetDisplayParam KEEP WIDTH AND HEIGHT"
#define MMISET_VIDEOWALLPAPER_1697_112_2_18_2_52_17_365 "[MMISETVWP]:GetDisplayParam CHANGE dy=%d"
#define MMISET_VIDEOWALLPAPER_1709_112_2_18_2_52_17_366 "[MMISETVWP]:GetDisplayParam LCD_ANGLE_0"
#define MMISET_VIDEOWALLPAPER_1714_112_2_18_2_52_17_367 "[MMISETVWP]:GetDisplayParam LCD_ANGLE_90"
#define MMISET_VIDEOWALLPAPER_1738_112_2_18_2_52_17_368 "[MMISETVWP]:PlayNextVideoWallpaper() UdiskIsRun!!"
#define MMISET_VIDEOWALLPAPER_1779_112_2_18_2_52_18_369 "[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play!"
#define MMISET_VIDEOWALLPAPER_1782_112_2_18_2_52_18_370 "[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play fail!"
#define MMISET_VIDEOWALLPAPER_1788_112_2_18_2_52_18_371 "[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play TRUE!"
#define MMISET_VIDEOWALLPAPER_1815_112_2_18_2_52_18_372 "[MMISETVWP]:PlayNextVideoWallpaper() not found!"
#define MMISET_VIDEOWALLPAPER_2038_112_2_18_2_52_18_373 "[MMIAPISETWP]: LCD SWITCH"
#define MMISET_VIDEOWALLPAPER_2089_112_2_18_2_52_18_374 "[MMIAPISETWP] AdjustVolCallBack volue = %d"
#define MMISET_WALLPAPER_399_112_2_18_2_52_20_375 "pda HandleSetWallpaperWindow unknow index"
#define MMISET_WALLPAPER_970_112_2_18_2_52_21_376 "mmisetting_assert HandlePreviewWallpaperWindow g_cur_wallpaper_index %d"
#define MMISET_WINTAB_614_112_2_18_2_52_23_377 "mmisetting_assert MMIAPISET_OpenFixCallRingWin"
#define MMISET_WINTAB_754_112_2_18_2_52_23_378 "mmisetting_assert HandleSelectFixedCallRingWindow"
#define MMISET_WINTAB_757_112_2_18_2_52_23_379 "[MMISET]:HandleSelectFixedCallRingWindow, msg = 0x%x"
#define MMISET_WINTAB_1129_112_2_18_2_52_24_380 "EnterRingSettingMenu:mainmenu_style %d is error!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_SETTING_TRC)
TRACE_MSG(MMISET_CALL_516_112_2_18_2_50_4_0,"[MMISET]: HandleSIMPlugNotifySetFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMISET_CALL_702_112_2_18_2_50_4_1,"HandlePsMsg:receive APP_MN_RELEASE_SS_IND (%d, %d, %d)")
TRACE_MSG(MMISET_CALL_735_112_2_18_2_50_4_2,"HandlePsMsg:receive APP_MN_REGISTER_SS_CNF")
TRACE_MSG(MMISET_CALL_755_112_2_18_2_50_4_3,"HandlePsMsg:receive APP_MN_ERASE_SS_CNF")
TRACE_MSG(MMISET_CALL_774_112_2_18_2_50_4_4,"HandlePsMsg:receive APP_MN_ACTIVATE_SS_CNF")
TRACE_MSG(MMISET_CALL_797_112_2_18_2_50_4_5,"HandlePsMsg:receive APP_MN_DEACTIVATE_SS_CNF")
TRACE_MSG(MMISET_CALL_820_112_2_18_2_50_5_6,"HandlePsMsg:receive APP_MN_INTERROGATE_CNF (%d, %d, %d)")
TRACE_MSG(MMISET_CALL_887_112_2_18_2_50_5_7,"HandleGetPasswordInd:guidance_info is %d!")
TRACE_MSG(MMISET_CALL_891_112_2_18_2_50_5_8,"connect win is closed")
TRACE_MSG(MMISET_CALL_1000_112_2_18_2_50_5_9,"HandleReleaseSsInd: error_present is %d and error_code is %d, %d, %d")
TRACE_MSG(MMISET_CALL_1116_112_2_18_2_50_5_10,"UpdateCfuStatuaAfterOperate ss_code=%d,service_type=%d")
TRACE_MSG(MMISET_CALL_1213_112_2_18_2_50_5_11,"HandleOperateSsCnf: result_type is %d,list num is = %d")
TRACE_MSG(MMISET_CALL_1252_112_2_18_2_50_6_12,"HandleOperateSsCnf fail_num=%d, list_num=%d")
TRACE_MSG(MMISET_CALL_1303_112_2_18_2_50_6_13,"HandleOperateSsCnf:error_code is %d")
TRACE_MSG(MMISET_CALL_1451_112_2_18_2_50_6_14,"HandleInterrogateSsCnf: result_type is %d")
TRACE_MSG(MMISET_CALL_1472_112_2_18_2_50_6_15,"HandleInterrogateSsCnf: get the status for cfu icon")
TRACE_MSG(MMISET_CALL_1577_112_2_18_2_50_6_16,"GenInterrogateReport:list_num = %d")
TRACE_MSG(MMISET_CALL_1590_112_2_18_2_50_6_17,"GenInterrogateReport:list_num = %d")
TRACE_MSG(MMISET_CALL_1654_112_2_18_2_50_6_18,"GenDcsStatusReport:list_num = %d,service_code=%d")
TRACE_MSG(MMISET_CALL_1671_112_2_18_2_50_6_19,"GenDcsStatusReport:service_type = %d,service_code=%d")
TRACE_MSG(MMISET_CALL_1727_112_2_18_2_50_6_20,"GenInterrogateReport:list_num = %d")
TRACE_MSG(MMISET_CALL_1826_112_2_18_2_50_7_21,"GenInterrogateReport:list_num = %d")
TRACE_MSG(MMISET_CALL_1934_112_2_18_2_50_7_22,"GenInterrogateStatusReport:list_num = %d,service_code=%d")
TRACE_MSG(MMISET_CALL_1950_112_2_18_2_50_7_23,"GenInterrogateStatusReport: info_list_ptr[0].ss_status_present is 0")
TRACE_MSG(MMISET_CALL_1974_112_2_18_2_50_7_24,"GenInterrogateStatusReport: info_list_ptr[i].ss_status_present is 0")
TRACE_MSG(MMISET_CALL_1990_112_2_18_2_50_7_25,"GenInterrogateStatusReport: no value")
TRACE_MSG(MMISET_CALL_2017_112_2_18_2_50_7_26,"GenInterrogateVTReport:list_num = %d")
TRACE_MSG(MMISET_CALL_2031_112_2_18_2_50_7_27,"GenInterrogateVTStatusReport: info_list_ptr[0].ss_status_present is 0")
TRACE_MSG(MMISET_CALL_2044_112_2_18_2_50_7_28,"GenInterrogateVTStatusReport: i=%d, type=%d,code=%d")
TRACE_MSG(MMISET_CALL_2054_112_2_18_2_50_7_29,"GenInterrogateVTStatusReport: info_list_ptr[i].ss_status_present is 0")
TRACE_MSG(MMISET_CALL_2070_112_2_18_2_50_7_30,"GenInterrogateVTStatusReport: no value")
TRACE_MSG(MMISET_CALL_2130_112_2_18_2_50_7_31,"GenInterrogateDetailReport:ss_code is 0x04%x, ss_status is 0x04%x")
TRACE_MSG(MMISET_CALL_2815_112_2_18_2_50_9_32,"%d")
TRACE_MSG(MMISET_CALL_2816_112_2_18_2_50_9_33,"SetSsParam: called_fuc_id is %d a, ss code = %d")
TRACE_MSG(MMISET_CALL_2896_112_2_18_2_50_9_34,"GetCfuSimId cfu_dual_sys=%d")
TRACE_MSG(MMISET_CALL_2951_112_2_18_2_50_9_35,"MMIAPISET_InterrogateCfuStatusFromPs:only one sim card")
TRACE_MSG(MMISET_CALL_2956_112_2_18_2_50_9_36,"MMIAPISET_InterrogateCfuStatusFromPs:err_code=0x%x,cff1=%d,cff2=%d")
TRACE_MSG(MMISET_CALL_3002_112_2_18_2_50_9_37,"MMIAPISET_InterrogateCfuStatusFromPs:get the cfu status from the network, ss_err_code=%d")
TRACE_MSG(MMISET_CALL_3007_112_2_18_2_50_9_38,"MMIAPISET_InterrogateCfuStatusFromPs:has query already. do nothing")
TRACE_MSG(MMISET_CALL_3079_112_2_18_2_50_9_39,"MMISET_SaveLineInfoToNV: the line type is %d")
TRACE_MSG(MMISET_CALL_3085_112_2_18_2_50_9_40,"MMISET_SaveLineInfoToNV: read the line type is %d, the return value is %d")
TRACE_MSG(MMISET_CALL_3491_112_2_18_2_50_10_41,"mmiset: query all cf start")
TRACE_MSG(MMISET_CALL_3492_112_2_18_2_50_10_42,"mmiset: query all cfu")
TRACE_MSG(MMISET_CALL_3556_112_2_18_2_50_10_43,"MMISET_CloseAllCallForwarding: group_id is %d and called_subfuc_id is %d")
TRACE_MSG(MMISET_CALL_3819_112_2_18_2_50_11_44,"MMISET_CloseAllCallBar: group_id is %d and called_subfuc_id is %d")
TRACE_MSG(MMISET_CALL_3905_112_2_18_2_50_11_45,"MMIAPISET_GetActiveIPNumber error len = %d num_len = %d")
TRACE_MSG(MMISET_CALL_4081_112_2_18_2_50_11_46,"mmiset:MMIAPISET_ParsSsStringProc len=%d, str=%s")
TRACE_MSG(MMISET_CALL_5099_112_2_18_2_50_13_47,"VirWin_HandleMsg: received SS repeat timer")
TRACE_MSG(MMISET_CALL_5149_112_2_18_2_50_14_48,"MMIAPISET_BgInterrogateStatus: interrogate service")
TRACE_MSG(MMISET_CALL_5175_112_2_18_2_50_14_49,"MMIAPISET_BgSetIgStatusParam bg_snd = %d, ss_snd = %d, g_is_for_cfu_icon = %d")
TRACE_MSG(MMISET_CALL_5180_112_2_18_2_50_14_50,"MMIAPISET_BgSetIgStatusParam:isnot MMISET_DAULSYS_ALL_SIM_ON")
TRACE_MSG(MMISET_CALL_5448_112_2_18_2_50_14_51,"mmiset:MMISET_RegisterDualsysCFNRC dual_sys = %d")
TRACE_MSG(MMISET_CALL_5520_112_2_18_2_50_14_52,"mmiset: query all cfb")
TRACE_MSG(MMISET_CALL_5531_112_2_18_2_50_14_53,"mmiset: query all cfnry")
TRACE_MSG(MMISET_CALL_5542_112_2_18_2_50_14_54,"mmiset: query all cfnrc")
TRACE_MSG(MMISET_CALL_5554_112_2_18_2_50_14_55,"mmiset: query all cf end")
TRACE_MSG(MMISET_CALL_5576_112_2_18_2_50_14_56,"s_cf_info_ptr is null")
TRACE_MSG(MMISET_CALL_5789_112_2_18_2_50_15_57,"mmivt GetFallbackHandleMode :%d ret:%d")
TRACE_MSG(MMISET_CALL_5857_112_2_18_2_50_15_58,"MMISET_SetVTRecordRVDAPath error record_path %d")
TRACE_MSG(MMISET_CALL_5893_112_2_18_2_50_15_59,"MMISET_SetVTRecordDAPath error device %d")
TRACE_MSG(MMISET_CALL_5929_112_2_18_2_50_15_60,"MMISET_SetVTRecordRVPath error device %d")
TRACE_MSG(MMISET_CALL_6032_112_2_18_2_50_15_61,"MMISET_IsSSProcessing g_is_background_search_send=%d, g_is_ss_user_query=%d, g_is_for_cfu_icon=%d,is_network_connecting=%d")
TRACE_MSG(MMISET_CALL_6049_112_2_18_2_50_15_62,"MMIAPISET_SetSSStatus dual_sys=%d, ss_code=%d, service_type=%d, service_code=%d")
TRACE_MSG(MMISET_CALL_6098_112_2_18_2_50_15_63,"MMISET_SetIsCfuActived dual_sys=%d, ss_type=%d, is_actived=%d")
TRACE_MSG(MMISET_CALL_6133_112_2_18_2_50_15_64,"MMISET_GetIsCfuActived dual_sys=%d, ss_type=%d, is_ss_cfu_actived=%d")
TRACE_MSG(MMISET_CALL_6201_112_2_18_2_50_16_65,"[MMISET]:MMIAPISET_GetMagicVoiceRes dat_ptr= 0x%x, data_len = %d")
TRACE_MSG(MMISET_CALLWIN_3086_112_2_18_2_50_22_66,"mmisetting_assert ImplementCTAStandbyChanged")
TRACE_MSG(MMISET_CALLWIN_3138_112_2_18_2_50_22_67,"mmisetting_assert ImplementCTASendSIMNotReadInd")
TRACE_MSG(MMISET_CALLWIN_3766_112_2_18_2_50_23_68,"MMISET_SetDivertBarWatingHideidCtrlId ctrl_id = %d")
TRACE_MSG(MMISET_CALLWIN_3777_112_2_18_2_50_23_69,"MMISET_GetDivertBarWatingHideidCtrlId ctrl_id = %d")
TRACE_MSG(MMISET_CALLWIN_4491_112_2_18_2_50_25_70,"HandleGetVoiMessAddrCnf:enter divert voice box!")
TRACE_MSG(MMISET_CALLWIN_4556_112_2_18_2_50_25_71,"HandleBarPasswordWindow s_call_bar_operation=%d")
TRACE_MSG(MMISET_CALLWIN_4923_112_2_18_2_50_26_72,"HandleSelectNetworkWindow: select network mode is %d error!")
TRACE_MSG(MMISET_CALLWIN_5100_112_2_18_2_50_26_73,"mmisetting_assert AppendIpNumberItem pos %d")
TRACE_MSG(MMISET_CALLWIN_6023_112_2_18_2_50_28_74,"mmisetting_assert HandleVTReplaceImageSettingsWindow")
TRACE_MSG(MMISET_CALLWIN_6045_112_2_18_2_50_28_75,"mmisetting_assert HandleVTReplaceImageSettingsWindow list_index %d")
TRACE_MSG(MMISET_CALLWIN_6103_112_2_18_2_50_28_76,"IsReplaceImageValid : null")
TRACE_MSG(MMISET_CALLWIN_6108_112_2_18_2_50_28_77,"IsReplaceImageValid width:%d height:%d size:%d")
TRACE_MSG(MMISET_CALLWIN_6687_112_2_18_2_50_29_78,"HandleSelectNetworkMethodWindow: select network mode is %d error!")
TRACE_MSG(MMISET_CALLWIN_6717_112_2_18_2_50_29_79,"HandleSelectNetworkMethodWindow: MNPHONE_SelectPLMN fail!")
TRACE_MSG(MMISET_CALLWIN_6748_112_2_18_2_50_29_80,"HandleSelectNetworkMethodWindow: MNPHONE_ListPLMN fail!")
TRACE_MSG(MMISET_CALLWIN_6869_112_2_18_2_50_29_81,"mmisetting_assert HandleSetAutoRedialWindow cur_selection %d")
TRACE_MSG(MMISET_CALLWIN_6974_112_2_18_2_50_30_82,"mmisetting_assert SetSpeedDialDelay cur_selection %d")
TRACE_MSG(MMISET_CALLWIN_7090_112_2_18_2_50_30_83,"MMIAPISET_SetTTSInfo cur_selection = %d")
TRACE_MSG(MMISET_CALLWIN_8587_112_2_18_2_50_34_84,"HandleSetAnykeyRejectWindow: selected item is %d error!")
TRACE_MSG(MMISET_CALLWIN_8908_112_2_18_2_50_35_85,"mmisetting_assert HandleSetFlipAnswerWindow cur_selection %d")
TRACE_MSG(MMISET_CALLWIN_9243_112_2_18_2_50_36_86,"mmisetting_assert ImplementStandbyChanged")
TRACE_MSG(MMISET_CALLWIN_9968_112_2_18_2_50_38_87,"mmisetting_assert HandleSetMajorSimWhenAllSimOnWindow e_major_sim_func %d")
TRACE_MSG(MMISET_CALLWIN_10058_112_2_18_2_50_38_88,"mmisetting_assert HandleSetMajorSimWhenAllSimOnWindow e_major_sim_func %d")
TRACE_MSG(MMISET_CALLWIN_10069_112_2_18_2_50_38_89,"mmiset:HandleSetMajorSimWhenAllSimOnWindow, cur_select=%d, e_major_sim_func=%d, e_major_sim_type = %d")
TRACE_MSG(MMISET_CALLWIN_10088_112_2_18_2_50_38_90,"HandleSetMajorSimWhenAllSimOnWindow,dual_sys[%d]>=MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISET_CALLWIN_10453_112_2_18_2_50_39_91,"mmisetting_assert HandleEditSimNameWindow %d")
TRACE_MSG(MMISET_CALLWIN_10546_112_2_18_2_50_39_92,"mmiset: HandleMultiSimSettingWindow wap")
TRACE_MSG(MMISET_CALLWIN_10802_112_2_18_2_50_40_93,"mmiset_wintab.c:DispatchBySelectType, dual_sys = %d, select_type = %d")
TRACE_MSG(MMISET_CALLWIN_11324_112_2_18_2_50_41_94,"mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin,True")
TRACE_MSG(MMISET_CALLWIN_11330_112_2_18_2_50_41_95,"mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin:s_dualsys_cfnrc_win_close_time = %d")
TRACE_MSG(MMISET_CALLWIN_11332_112_2_18_2_50_41_96,"mmiset:CloseDualsysCFNRCWin, s_dualsys_cfnrc_win_close_time=%d, action_result = %d")
TRACE_MSG(MMISET_CALLWIN_11353_112_2_18_2_50_41_97,"MMISET_CloseDualsysCFNRCWin: is not dual sys, do not handle!")
TRACE_MSG(MMISET_CALLWIN_11467_112_2_18_2_50_41_98,"MMISET_GetNetworkType gmmreg_rat is %d")
TRACE_MSG(MMISET_CALLWIN_11507_112_2_18_2_50_41_99,"MMIAPISET_SetNetworkType rat=%d")
TRACE_MSG(MMISET_CALLWIN_11553_112_2_18_2_50_41_100,"SetNetworkType cur_selection_netwoktype is %d, only_write_nv is %d")
TRACE_MSG(MMISET_CALLWIN_11598_112_2_18_2_50_41_101,"SetNetworkType %d %d")
TRACE_MSG(MMISET_CALLWIN_11802_112_2_18_2_50_42_102,"ResultDisp success: %d")
TRACE_MSG(MMISET_CALLWIN_11807_112_2_18_2_50_42_103,"ResultDisp fail: %d")
TRACE_MSG(MMISET_CALLWIN_11827_112_2_18_2_50_42_104,"ResultDisp success: %d")
TRACE_MSG(MMISET_CALLWIN_11832_112_2_18_2_50_42_105,"ResultDisp fail: %d")
TRACE_MSG(MMISET_CALLWIN_11914_112_2_18_2_50_42_106,"MMIAPISET_HandlePreferRatRsp rsp_result=%d")
TRACE_MSG(MMISET_CALLWIN_11964_112_2_18_2_50_42_107,"IsinSwitchRatState is TRUE")
TRACE_MSG(MMISET_CALLWIN_11969_112_2_18_2_50_42_108,"IsinSwitchRatState is FALSE")
TRACE_MSG(MMISET_CALLWIN_12182_112_2_18_2_50_43_109,"HandleNetworkSettingWindow,msg_id=0x%x")
TRACE_MSG(MMISET_CALLWIN_12247_112_2_18_2_50_43_110,"HandleDispPreferNetworkListWinMsg,msg=0x%x")
TRACE_MSG(MMISET_CALLWIN_12272_112_2_18_2_50_43_111,"HandleDispPreferNetworkListWinMsg,max_list_num")
TRACE_MSG(MMISET_CALLWIN_12304_112_2_18_2_50_43_112,"HandleDispPreferNetworkListWinMsg,total_item_num=%d ")
TRACE_MSG(MMISET_CALLWIN_12325_112_2_18_2_50_43_113,"HandleDispPreferNetworkListWinMsg,total_item_num=%d ")
TRACE_MSG(MMISET_CALLWIN_12387_112_2_18_2_50_43_114,"HandleDeletePreferPlmnQueryWindow,OK_Delete_Item= %d")
TRACE_MSG(MMISET_CALLWIN_12411_112_2_18_2_50_43_115,"HandleDeletePreferPlmnQueryWindow,Delete Plmn: mcc=%d,mnc=%d,mnc_len=%d")
TRACE_MSG(MMISET_CALLWIN_12449_112_2_18_2_50_43_116,"HandlePreferNetworkOptionMsg,msg_id=0x%x")
TRACE_MSG(MMISET_CALLWIN_12457_112_2_18_2_50_43_117,"HandlePreferNetworkOptionMsg,total_list_num=%d,plmn_max_num=%d")
TRACE_MSG(MMISET_CALLWIN_12497_112_2_18_2_50_43_118,"HandlePreferNetworkOptionMsg,total_list_num=%d,plmn_max_num=%d")
TRACE_MSG(MMISET_CALLWIN_12603_112_2_18_2_50_44_119,"HandlePlmnListDispWinMsg,msg_id=0x%x")
TRACE_MSG(MMISET_CALLWIN_12668_112_2_18_2_50_44_120,"HandlePlmnListDispWinMsg,Add Plmn: mcc=%d,mnc=%d,mnc_len=%d")
TRACE_MSG(MMISET_CALLWIN_12702_112_2_18_2_50_44_121,"HandleCreatePlmnItemWinMsg,msg_id = 0x%x")
TRACE_MSG(MMISET_CALLWIN_12842_112_2_18_2_50_44_122,"HandleCreatePlmnItemWinMsg,Add Plmn: mcc=%d,mnc=%d,mnc_len=%d")
TRACE_MSG(MMISET_CALLWIN_12879_112_2_18_2_50_44_123,"HandlePreferNetworkPriorityChangeWinMsg,msg = 0x%x,cur_index=%d")
TRACE_MSG(MMISET_CALLWIN_12955_112_2_18_2_50_44_124,"HandlePreferNetworkPriorityChangeWinMsg,index_priority=%d,cur_index=%d")
TRACE_MSG(MMISET_CALLWIN_13026_112_2_18_2_50_45_125,"HandlePreferNetChangeWaitingWinMsg,msg=0x%x")
TRACE_MSG(MMISET_CALLWIN_13131_112_2_18_2_50_45_126,"LoadMNPreferNetworkList,sim=%d,prefer_valid_num=%d,prefer_max_num=%d")
TRACE_MSG(MMISET_CALLWIN_13171_112_2_18_2_50_45_127,"MMIAPISET_SavePreferNetworkList ERROR ! s_update_prefer_list_ptr == PNULL !")
TRACE_MSG(MMISET_CALLWIN_13188_112_2_18_2_50_45_128,"MMIAPISET_SavePreferNetworkList,dual_sys=%d,prefer_valid_num=%d,prefer_max_num=%d")
TRACE_MSG(MMISET_CALLWIN_13207_112_2_18_2_50_45_129,"MMIAPISET_HandlePreferNetworkListCnf,sim=%d,update_sucess=%d")
TRACE_MSG(MMISET_DATETIME_642_112_2_18_2_50_47_130,"DisplaySystemDate sys_date.year = %d")
TRACE_MSG(MMISET_DATETIME_1392_112_2_18_2_50_48_131,"Waring, UpdateStbDispDate: month = %d")
TRACE_MSG(MMISET_DATETIME_1397_112_2_18_2_50_48_132,"Waring,UpdateStbDispDate: date = %d")
TRACE_MSG(MMISET_DATETIME_1995_112_2_18_2_50_49_133,"SetTime:hour is %d,minute is %d is invalid!")
TRACE_MSG(MMISET_DATETIME_2215_112_2_18_2_50_50_134,"DisplaySystemDate:date_type %d is error!")
TRACE_MSG(MMISET_DATETIME_2233_112_2_18_2_50_50_135,"DisplaySystemDate sys_date.year = %d")
TRACE_MSG(MMISET_DATETIME_2287_112_2_18_2_50_50_136,"SetDate sys_date.year = %d")
TRACE_MSG(MMISET_DATETIME_2294_112_2_18_2_50_50_137,"SetDate:year is %d,month is %d and day is %d is invalid!")
TRACE_MSG(MMISET_DISPLAY_644_112_2_18_2_50_52_138,"mmisetting_assert MMIAPISET_GetWallpaperSetInfo")
TRACE_MSG(MMISET_DISPLAY_742_112_2_18_2_50_52_139,"mmisetting_assert MMIAPISET_GetBackLightInfo")
TRACE_MSG(MMISET_DISPLAY_812_112_2_18_2_50_52_140,"MMIAPISET_UpdateLCDContrast:s_real_contrast = %d")
TRACE_MSG(MMISET_DISPLAY_990_112_2_18_2_50_53_141,"mmisetting_assert MMIAPISET_SetTVOutMmode cur_selection %d")
TRACE_MSG(MMISET_DISPLAY_1126_112_2_18_2_50_53_142,"MMIAPISET_SetTTSInfo cur_selection = %d")
TRACE_MSG(MMISET_DISPLAY_1230_112_2_18_2_50_53_143,"mmisetting_assert MMIAPISET_SetMainmenuStyle")
TRACE_MSG(MMISET_DISPLAY_1891_112_2_18_2_50_55_144,"MMISET_AppendBacklightListItem:backlight time is %d error!")
TRACE_MSG(MMISET_DISPLAY_1905_112_2_18_2_50_55_145,"MMISET_AppendBacklightListItem:backlight type is %d error!")
TRACE_MSG(MMISET_DISPLAY_1966_112_2_18_2_50_55_146,"MMISET_SetIdleDisplayMode: selected item is %d error!")
TRACE_MSG(MMISET_DISPLAY_2003_112_2_18_2_50_55_147,"mmisetting_assert MMISET_SetBackLight cur_selection %d")
TRACE_MSG(MMISET_DISPLAY_2167_112_2_18_2_50_55_148,"MMISET_TuneUpConstrast:msg_id %d is error!")
TRACE_MSG(MMISET_DISPLAY_2170_112_2_18_2_50_55_149,"MMISET_TuneUpConstrast:s_cur_contrast is %d!")
TRACE_MSG(MMISET_DISPLAY_2221_112_2_18_2_50_55_150,"mmisetting_assert MMIAPISET_GetScreensaverInfo")
TRACE_MSG(MMISET_DISPLAY_2414_112_2_18_2_50_56_151,"mmisetting_assert MMISET_Set3DStyleMainmenu cur_selection %d")
TRACE_MSG(MMISET_DISPLAY_2455_112_2_18_2_50_56_152,"mmisetting_assert MMISET_Set3DStyleScreen cur_selection %d")
TRACE_MSG(MMISET_DISPLAY_2558_112_2_18_2_50_56_153,"mmisetting_assert MMISET_SetGsensorSwitchStyle g_sensor_anim %d")
TRACE_MSG(MMISET_DISPLAY_2610_112_2_18_2_50_56_154,"mmisetting_assert Get3DStyle")
TRACE_MSG(MMISET_DISPLAY_2638_112_2_18_2_50_56_155,"mmisetting_assert Set3DStyle")
TRACE_MSG(MMISET_DISPLAY_2682_112_2_18_2_50_56_156,"mmisetting_assert MMISET_SetIdleStyle idle_style %d")
TRACE_MSG(MMISET_DISPLAY_2873_112_2_18_2_50_57_157,"Waring, MMIAPISET_FormatDateStrByDateStyle: month = %d")
TRACE_MSG(MMISET_DISPLAY_2878_112_2_18_2_50_57_158,"Waring,MMIAPISET_FormatDateStrByDateStyle: date = %d")
TRACE_MSG(MMISET_DISPLAY_2883_112_2_18_2_50_57_159,"Waring,MMIAPISET_FormatDateStrByDateStyle: in len = %d,min len=%d")
TRACE_MSG(MMISET_DISPLAY_2929_112_2_18_2_50_57_160,"Waring, MMIAPISET_FormatDateStrByDateEx: month = %d")
TRACE_MSG(MMISET_DISPLAY_2934_112_2_18_2_50_57_161,"Waring,MMIAPISET_FormatDateStrByDateEx: date = %d")
TRACE_MSG(MMISET_DISPLAY_2939_112_2_18_2_50_57_162,"Waring,MMIAPISET_FormatDateStrByDateEx: in len = %d,min len=%d")
TRACE_MSG(MMISET_DISPLAY_3131_112_2_18_2_50_57_163,"SetSlideOpenType slide_open_type = %d")
TRACE_MSG(MMISET_DISPLAY_3188_112_2_18_2_50_57_164,"SetSlideCloseType slide_close_type = %d")
TRACE_MSG(MMISET_DISPLAY_3245_112_2_18_2_50_57_165,"SetKeylockSwitchType keylock_switch_type = %d")
TRACE_MSG(MMISET_DISPLAY_3297_112_2_18_2_50_57_166,"mmisetting_assert IsValid3DStyle")
TRACE_MSG(MMISET_DISPLAY_3307_112_2_18_2_50_58_167,"IsValid3DStyle() NV abnormal!")
TRACE_MSG(MMISET_DISPLAYWIN_739_112_2_18_2_51_0_168,"SetAnimLabelParam")
TRACE_MSG(MMISET_DISPLAYWIN_745_112_2_18_2_51_0_169,"SetThemeParam")
TRACE_MSG(MMISET_DISPLAYWIN_862_112_2_18_2_51_0_170,"HandleSetDisplayWindow ctrl_id = %d")
TRACE_MSG(MMISET_DISPLAYWIN_1257_112_2_18_2_51_1_171,"AppendMainmenuStyle:mainmenu_style %d is error!")
TRACE_MSG(MMISET_DISPLAYWIN_1366_112_2_18_2_51_1_172,"HandleIdleMsg:idle_style %d is error!")
TRACE_MSG(MMISET_DISPLAYWIN_1486_112_2_18_2_51_1_173,"MMISET_AppendBacklightListItem:backlight time is %d error!")
TRACE_MSG(MMISET_DISPLAYWIN_1500_112_2_18_2_51_1_174,"MMISET_AppendBacklightListItem:backlight type is %d error!")
TRACE_MSG(MMISET_DISPLAYWIN_1744_112_2_18_2_51_2_175,"HandleSetDisplayAnimationWindow ctrl_id = %d")
TRACE_MSG(MMISET_DISPLAYWIN_2609_112_2_18_2_51_4_176,"mmisetting_assert MMIAPISET_GetPowerOnOffAnimInfo")
TRACE_MSG(MMISET_DISPLAYWIN_3150_112_2_18_2_51_5_177,"HandleSetConstrastWindow cur_constrast = %d")
TRACE_MSG(MMISET_DISPLAYWIN_3155_112_2_18_2_51_5_178,"HandleSetConstrastWindow cur_constrast = %d")
TRACE_MSG(MMISET_DISPLAYWIN_3679_112_2_18_2_51_6_179,"SetMainmenuStyle:cur_selection %d is error!")
TRACE_MSG(MMISET_DISPLAYWIN_3747_112_2_18_2_51_6_180,"SetIdleStyle:cur_index %d is error!")
TRACE_MSG(MMISET_FUNC_953_112_2_18_2_51_8_181,"mmisetting_assert InitPrivacyProtectPwd %d")
TRACE_MSG(MMISET_FUNC_981_112_2_18_2_51_8_182,"mmisetting_assert InitPhonePwd %d")
TRACE_MSG(MMISET_FUNC_1008_112_2_18_2_51_8_183,"mmisetting_assert MMIAPISET_SetByOpenCloseList cur_selection %d")
TRACE_MSG(MMISET_FUNC_1498_112_2_18_2_51_9_184,"mmisetting_assert MMIAPISET_GetLanguageType")
TRACE_MSG(MMISET_FUNC_1518_112_2_18_2_51_9_185,"MMIAPISET_SetLanguageType language_type=%d, not exist")
TRACE_MSG(MMISET_FUNC_1598_112_2_18_2_51_9_186,"mmisetting_assert MMIAPISET_GetKeyBoardLightTime")
TRACE_MSG(MMISET_FUNC_1661_112_2_18_2_51_9_187,"MMISET_CleanUserData: enter.")
TRACE_MSG(MMISET_FUNC_1699_112_2_18_2_51_9_188,"MMISET_CleanUserData delete result = %d")
TRACE_MSG(MMISET_FUNC_1707_112_2_18_2_51_10_189,"MMISET_CleanUserData: Success Exit.")
TRACE_MSG(MMISET_FUNC_2183_112_2_18_2_51_10_190,"[MMISET] MMIAPISET_SetWaitFlag")
TRACE_MSG(MMISET_FUNC_2193_112_2_18_2_51_10_191,"[MMISET] MMIAPISET_FuncFinishedInd")
TRACE_MSG(MMISET_FUNC_2207_112_2_18_2_51_11_192,"mmisetting_assert GetValidIdxByLangIdx %d")
TRACE_MSG(MMISET_FUNC_2236_112_2_18_2_51_11_193,"mmisetting_assert MMIAPISET_IsSelectedLanguageValid language_type %d")
TRACE_MSG(MMISET_FUNC_2329_112_2_18_2_51_11_194,"mmisetting_assert MMISET_AppendLanguageListItem language_type %d")
TRACE_MSG(MMISET_FUNC_2339_112_2_18_2_51_11_195,"mmisetting_assert MMISET_AppendLanguageListItem language_type %d")
TRACE_MSG(MMISET_FUNC_2391_112_2_18_2_51_11_196,"mmisetting_assert MMIAPISET_AppendInputLanguageListItem language_type %d")
TRACE_MSG(MMISET_FUNC_2460_112_2_18_2_51_11_197,"mmisetting_assert MMIAPISET_GetInputLanguageString language_type %d")
TRACE_MSG(MMISET_FUNC_2467_112_2_18_2_51_11_198,"mmisetting_assert MMIAPISET_GetInputLanguageString 2")
TRACE_MSG(MMISET_FUNC_2549_112_2_18_2_51_11_199,"mmisetting_assert MMISET_SetKeyboardLight cur_selection %d")
TRACE_MSG(MMISET_FUNC_2600_112_2_18_2_51_11_200,"MMISET_SetKeyboardLight: cur_selection %d is error!")
TRACE_MSG(MMISET_FUNC_2645_112_2_18_2_51_11_201,"mmisetting_assert MMISET_SetMoveStyle cur_selection %d")
TRACE_MSG(MMISET_FUNC_2666_112_2_18_2_51_11_202,"MMISET_SetMoveStyle: cur_selection %d is error!")
TRACE_MSG(MMISET_FUNC_2696_112_2_18_2_51_11_203,"MMISET_AppendSelectNetworkListItem:select network mode is %d!,dual_sys = %d")
TRACE_MSG(MMISET_FUNC_2749_112_2_18_2_51_12_204,"mmisetting_assert MMISET_SetSelectNetworkMode cur_selection %d")
TRACE_MSG(MMISET_FUNC_2781_112_2_18_2_51_12_205,"MMISET_SetSelectNetworkMode: cur_selection %d is error!")
TRACE_MSG(MMISET_FUNC_2803_112_2_18_2_51_12_206,"mmisetting_assert MMIAPISET_IsOpenPrivacyProtect protect_type %d")
TRACE_MSG(MMISET_FUNC_3052_112_2_18_2_51_12_207,"mmisetting_assert MMIAPISET_OpenPlmnListWin")
TRACE_MSG(MMISET_FUNC_3082_112_2_18_2_51_12_208,"mmisetting_assert AppendPlmnListItem")
TRACE_MSG(MMISET_FUNC_3094_112_2_18_2_51_12_209,"mmisetting_assert AppendPlmnListItem")
TRACE_MSG(MMISET_FUNC_3336_112_2_18_2_51_13_210,"MMISET_SetAnykeyRejectMode: selected item is error!")
TRACE_MSG(MMISET_FUNC_3362_112_2_18_2_51_13_211,"mmisetting_assert MMISET_SelectNetwork cur_selection %d")
TRACE_MSG(MMISET_FUNC_3370_112_2_18_2_51_13_212,"MMISET_SelectNetwork: mcc = %d, mnc = %d, mnc_digit_num = %d, select dual sys = %d")
TRACE_MSG(MMISET_FUNC_3379_112_2_18_2_51_13_213,"MMISET_SelectNetwork: MNPHONE_SelectPLMN fail!")
TRACE_MSG(MMISET_FUNC_3476_112_2_18_2_51_13_214,"mmisetting_assert MMISET_SetSelectBandType cur_selection %d")
TRACE_MSG(MMISET_FUNC_3490_112_2_18_2_51_13_215,"MMISET_SetSelectBandType: cur_selection %d is error!")
TRACE_MSG(MMISET_FUNC_3702_112_2_18_2_51_13_216,"MMIAPISET_GetFlyModeSetPowerOn is_fly_mode_power_on=%d")
TRACE_MSG(MMISET_FUNC_3796_112_2_18_2_51_14_217,"MMIAPISET_IsNeedResetDateTime is_rtcAbnormal = %d")
TRACE_MSG(MMISET_FUNC_3827_112_2_18_2_51_14_218,"GetMulNVConfigInfo fail! reslut = %d")
TRACE_MSG(MMISET_FUNC_3871_112_2_18_2_51_14_219,"MMIAPISET_CheckLanguageStatus language_type = %d,result=%d")
TRACE_MSG(MMISET_FUNC_3950_112_2_18_2_51_14_220,"mmisetting_assert MMISET_AppendShowLanguageItem language_type %d")
TRACE_MSG(MMISET_FUNC_3978_112_2_18_2_51_14_221,"mmisetting_assert MMISET_AppendInputLanguageItem language_type %d")
TRACE_MSG(MMISET_FUNC_4226_112_2_18_2_51_15_222,"MMISET_GetInputLanguageContent")
TRACE_MSG(MMISET_FUNC_4307_112_2_18_2_51_15_223,"MMIAPISET_RestoreDataTimeFactorySetting")
TRACE_MSG(MMISET_FUNC_4779_112_2_18_2_51_16_224,"MMISET_AllocAndInitLanguageInfo valid language is %d")
TRACE_MSG(MMISET_FUNC_4897_112_2_18_2_51_16_225,"MMIAPISET_ConvertNVLangToSysType(set_lang = %d) BEGIN:dual_sys = %d")
TRACE_MSG(MMISET_FUNC_4905_112_2_18_2_51_16_226,"MMIAPISET_ConvertNVLangToSysType,get auto_lang = %d")
TRACE_MSG(MMISET_FUNC_4928_112_2_18_2_51_16_227,"MMIAPISET_ConvertNVLangToSysType END: cur_lang = %d,nv_lang=%d")
TRACE_MSG(MMISET_FUNC_4956_112_2_18_2_51_16_228,"MMIAPISET_GetPreferLanguageTypeFromSim Begin :sim=%d, [elp_lst_isvalid=%d and num=%d],[lp_isvalid=%d and num=%d],[elp_2nd_valid= %d and num=%d]")
TRACE_MSG(MMISET_FUNC_4971_112_2_18_2_51_16_229,"GetPreferedLanguage 1st_elp:i=%d,j=%d,mmi_type=%d,sim_code=[0x%x,0x%x]")
TRACE_MSG(MMISET_FUNC_4994_112_2_18_2_51_16_230,"GetPreferedLanguage lp :i=%d,j=%d,mmi_type=%d,sim_code=0x%x")
TRACE_MSG(MMISET_FUNC_5017_112_2_18_2_51_16_231,"GetPreferedLanguage 2nd_elp:i=%d,j=%d,mmi_type=%d,sim_code=[0x%x,0x%x]")
TRACE_MSG(MMISET_LOCK_EFFECT_335_112_2_18_2_51_17_232,"HandleScreenLockTypeSetWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISET_LOCK_EFFECT_1049_112_2_18_2_51_19_233,"MMIAPISET_GetKeyLockStyle: style =%d, is invalid!!!")
TRACE_MSG(MMISET_LONGPOWER_114_112_2_18_2_51_19_234,"HandleBlacklistOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISET_MAINMENU_521_112_2_18_2_51_21_235,"GetMainmenuStyleItemIndex error type = %d")
TRACE_MSG(MMISET_MAINMENU_597_112_2_18_2_51_21_236,"GetMainmenuStyleByIndex error item_index = %d")
TRACE_MSG(MMISET_MAINMENU_671_112_2_18_2_51_21_237,"mmisetting_assert MoveSetingPreview FALSE")
TRACE_MSG(MMISET_MAINMENU_753_112_2_18_2_51_21_238,"mmisetting_assert ResumeSetingPreview 0")
TRACE_MSG(MMISET_MAINMENU_854_112_2_18_2_51_21_239,"HandleMainMenuTypeSetWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISET_MAINMENU_927_112_2_18_2_51_22_240,"HandleMainMenuTypeSetWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISET_MAINMENU_1124_112_2_18_2_51_22_241,"HandleMainMenuTypePreviewWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISET_MAINMENU_1357_112_2_18_2_51_23_242,"GetMainmenuStyleItemIndex error type = %d")
TRACE_MSG(MMISET_MAINMENU_1476_112_2_18_2_51_23_243,"GetMainmenuStyleByIndex error item_index = %d")
TRACE_MSG(MMISET_MAINMENU_1530_112_2_18_2_51_23_244,"HandleGOMenuTypeSetWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISET_MULTISIM_260_112_2_18_2_51_26_245,"MMIAPISET_GetSimName error dual_sys = %d")
TRACE_MSG(MMISET_MULTISIM_377_112_2_18_2_51_26_246,"MMIAPISET_GetSimColorID error dual_sys = %d")
TRACE_MSG(MMISET_MULTISIM_404_112_2_18_2_51_26_247,"MMIAPISET_GetSimColor error dual_sys = %d")
TRACE_MSG(MMISET_MULTISIM_500_112_2_18_2_51_27_248,"MMIAPISET_GetSimColor error dual_sys = %d")
TRACE_MSG(MMISET_MULTISIM_561_112_2_18_2_51_27_249,"MMISET_SetMajorSimSelType  MMISET_SetMajorSimSelType =%d, e_sim_sel_type= %d")
TRACE_MSG(MMISET_MULTISIM_970_112_2_18_2_51_28_250,"MMIAPISET_GetAnswerWithOriginalSimType   e_func_type=%d")
TRACE_MSG(MMISET_MULTISIM_989_112_2_18_2_51_28_251,"MMISET_SetMultiSysSetting   e_dual_sys=%d")
TRACE_MSG(MMISET_MULTISIM_1027_112_2_18_2_51_28_252,"MMIAPISET_GetMultiSysSetting e_dual_sys=%d")
TRACE_MSG(MMISET_MULTISIM_1139_112_2_18_2_51_28_253,"mmiset:MMIAPISET_GetMajorSim e_major_sim_type=%d")
TRACE_MSG(MMISET_MULTISIM_1190_112_2_18_2_51_28_254,"mmiset:MMIAPISET_GetMajorSim e_major_sim_type=%d")
TRACE_MSG(MMISET_PHONEWIN_1126_112_2_18_2_51_33_255,"HandleSetSlideOpenTypeWindow cur_selection = %d")
TRACE_MSG(MMISET_PHONEWIN_1191_112_2_18_2_51_33_256,"HandleSetSlideCloseTypeWindow cur_selection = %d")
TRACE_MSG(MMISET_PHONEWIN_1255_112_2_18_2_51_33_257,"HandleSetKeylockSwitchTypeWindow cur_selection = %d")
TRACE_MSG(MMISET_PHONEWIN_1823_112_2_18_2_51_34_258,"HandleSetSensorWindow sensor_data = %d,is_sensor_mute = %d,is_sensor_change_wallpaper = %d")
TRACE_MSG(MMISET_PHONEWIN_2393_112_2_18_2_51_35_259,"MMIAPISET_EnterPhoneSettingWin:mainmenu_style %d is error!")
TRACE_MSG(MMISET_PHONEWIN_2550_112_2_18_2_51_35_260,"SetPowerSaveParam PNULL == mtext_str.wstr_ptr")
TRACE_MSG(MMISET_PHONEWIN_2579_112_2_18_2_51_35_261,"SetPowerSaveParam PNULL == mtext_str.wstr_ptr")
TRACE_MSG(MMISET_PHONEWIN_2603_112_2_18_2_51_35_262,"SetPowerSaveParam SCI_ALLOCA error!")
TRACE_MSG(MMISET_PHONEWIN_2715_112_2_18_2_51_36_263,"MMIAPISET_GetPowerSavingSetting nv_item = %d")
TRACE_MSG(MMISET_PHONEWIN_2850_112_2_18_2_51_36_264,"HandlePowerSaveSettingWindow list1 index = %d")
TRACE_MSG(MMISET_PHONEWIN_2887_112_2_18_2_51_36_265,"HandlePowerSaveSettingWindow list2 index = %d")
TRACE_MSG(MMISET_PHONEWIN_3163_112_2_18_2_51_37_266,"MMIAPISET_IsProgramRunning bt is open.")
TRACE_MSG(MMISET_PHONEWIN_3173_112_2_18_2_51_37_267,"MMIAPISET_IsProgramRunning wifi is open.")
TRACE_MSG(MMISET_PHONEWIN_3182_112_2_18_2_51_37_268,"MMIAPISET_IsProgramRunning java is open.")
TRACE_MSG(MMISET_PHONEWIN_3191_112_2_18_2_51_37_269,"MMIAPISET_IsProgramRunning browser is open.")
TRACE_MSG(MMISET_RING_349_112_2_18_2_51_38_270,"[MMISET]:MMIAPISET_GetMultimVolume VOL=%d")
TRACE_MSG(MMISET_RING_377_112_2_18_2_51_38_271,"[MMISET]:MMIAPISET_SetMultimVolume error!! VOL=%d")
TRACE_MSG(MMISET_RING_523_112_2_18_2_51_39_272,"MMIAPISET_SetVibratorEx: is_open = %d, ring_type = %d,is_period = %d, duration = %d")
TRACE_MSG(MMISET_RING_625_112_2_18_2_51_39_273,"MMIAPISET_PlayRing:NO PERMITPLAYRING")
TRACE_MSG(MMISET_RING_664_112_2_18_2_51_39_274,"MMIAPISET_PlayRing:NO PERMITPLAYRING")
TRACE_MSG(MMISET_RING_689_112_2_18_2_51_39_275,"[MMISET]:PlayFixedRing, ring_type = 0x%x")
TRACE_MSG(MMISET_RING_699_112_2_18_2_51_39_276,"PlayFixedRing ring_id = %d is error!")
TRACE_MSG(MMISET_RING_776_112_2_18_2_51_40_277,"[MMISET]:PlayFixedRing, ring_type = 0x%x")
TRACE_MSG(MMISET_RING_780_112_2_18_2_51_40_278,"PlayFixedRing ring_id = %d is error!")
TRACE_MSG(MMISET_RING_888_112_2_18_2_51_40_279,"mmisetting_assert PlayRing CALL")
TRACE_MSG(MMISET_RING_907_112_2_18_2_51_40_280,"mmisetting_assert PlayRing MSG")
TRACE_MSG(MMISET_RING_984_112_2_18_2_51_40_281,"[MMISET]:MMI_APISET_LivePaperPlayRing, not PermitPlayRing")
TRACE_MSG(MMISET_RING_1036_112_2_18_2_51_40_282,"mmisetting_assert MMIAPISET_GetDeviceMode")
TRACE_MSG(MMISET_RING_1088_112_2_18_2_51_40_283,"[MMISET]: MMIAPISET_StopRing enter--ring_type is %d and g_is_playing_ring_type is %d,s_cur_audio_htype = %d")
TRACE_MSG(MMISET_RING_1159_112_2_18_2_51_41_284,"MMIAPISET_StopAllRing s_cur_audio_htype= %d")
TRACE_MSG(MMISET_RING_1256_112_2_18_2_51_41_285,"MMISET_SetCallRingType:call_ring_type is %d!")
TRACE_MSG(MMISET_RING_2084_112_2_18_2_51_43_286,"MMIAPISET_GetCallRingInfo enter")
TRACE_MSG(MMISET_RING_2096_112_2_18_2_51_43_287,"MMIAPISET_GetCallRingInfo: the more call ring is not exited")
TRACE_MSG(MMISET_RING_2101_112_2_18_2_51_43_288,"MMIAPISET_GetCallRingInfo out")
TRACE_MSG(MMISET_RING_2327_112_2_18_2_51_43_289,"mmisetting_assert GetRingInfo")
TRACE_MSG(MMISET_RING_2495_112_2_18_2_51_44_290,"GetRingInfo:ring_type is error!")
TRACE_MSG(MMISET_RING_2927_112_2_18_2_51_45_291,"MMIAPISET_PlayMusicFile param error!")
TRACE_MSG(MMISET_RING_3015_112_2_18_2_51_45_292,"MMIAPISET_PlayMusicFile, ring_type= %d, play_times = %d, vol = %d, call_back = 0x%x,music_type = %d, result = %d")
TRACE_MSG(MMISET_RING_3051_112_2_18_2_51_45_293,"MMIAPISET_PlayMusicFile param error!")
TRACE_MSG(MMISET_RING_3132_112_2_18_2_51_45_294,"MMIAPISET_PlayMusicFile, ring_type= %d, play_times = %d, vol = %d, call_back = 0x%x,music_type = %d, result = %d")
TRACE_MSG(MMISET_RING_3178_112_2_18_2_51_45_295,"[MMISET]:HandleReceiverMode, ring_type = %d, volume = %d, is_through_receiver = %d")
TRACE_MSG(MMISET_RING_3247_112_2_18_2_51_45_296,"[MMISET]:MMIAPISET_IsThroughReceiver, ring_type = %d")
TRACE_MSG(MMISET_RING_3274_112_2_18_2_51_46_297,"[MMISET]: MMIAPISET_InitAudioDeviceExt: ring_type = %d,volume=%d")
TRACE_MSG(MMISET_RING_3305_112_2_18_2_51_46_298,"[MMISET]: MMIAPISET_InitBtAudioDevice: is_bt_play = %d, volume=%d")
TRACE_MSG(MMISET_RING_3345_112_2_18_2_51_46_299,"[MMISET]: MMIAPISET_InitAudioDevice: volume=%d")
TRACE_MSG(MMISET_RING_3594_112_2_18_2_51_46_300,"MMIAPISET_IsPermitPlayRing: ring_type=0x%x, g_is_playing_ring_type = 0x%x,s_is_vibrate=0x%x, s_is_playing_vibrate_type=0x%x,result= 0x%x")
TRACE_MSG(MMISET_RING_3777_112_2_18_2_51_47_301,"PlayGenericTone:is_dsp_play_ring is FALSE")
TRACE_MSG(MMISET_RING_4086_112_2_18_2_51_47_302,"[MMISET]:MMIAPISET_SetCurRingType, ring_type = %d")
TRACE_MSG(MMISET_RING_4096_112_2_18_2_51_47_303,"[MMISET]:MMIAPISET_ClearCurRingType, ring_type = %d, g_is_playing_ring_type = %d")
TRACE_MSG(MMISET_RING_4120_112_2_18_2_51_47_304,"[MMISET]:MMIAPISET_IsCurRingType, ring_type=%d, g_ring=%d, s_cur_app_handle=0x%x")
TRACE_MSG(MMISET_RING_4136_112_2_18_2_51_47_305,"[MMISET]:MMIAPISET_IsCurRingType, ring_type = %d, g_ring = %d, s_audio = %d, result = %d")
TRACE_MSG(MMISET_RING_4195_112_2_18_2_51_47_306,"[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d")
TRACE_MSG(MMISET_RING_4406_112_2_18_2_51_48_307,"[MMISET]: HandleAudioCallBack() entry, handle=0x%x, param=0x%x")
TRACE_MSG(MMISET_RING_4414_112_2_18_2_51_48_308,"[MMISRV]: HandleNotify(), report_ptr->report=%d")
TRACE_MSG(MMISET_SCREENSAVER_422_112_2_18_2_51_49_309,"mmisetting_assert HandleScreensaverPreviewWindow sel_item_index %d")
TRACE_MSG(MMISET_SECURITY_1955_112_2_18_2_51_54_310,"mmiset cur_selection = %d exceed MMISET_KEYLOCK_MAX_ITEM ")
TRACE_MSG(MMISET_SECURITY_3523_112_2_18_2_51_57_311,"AppendSecuritySetListItem:mode=%d")
TRACE_MSG(MMISET_SECURITY_3731_112_2_18_2_51_58_312,"mmiset pwd_ptr == PNULL")
TRACE_MSG(MMISET_SECURITY_3761_112_2_18_2_51_58_313,"mmiset pwd_ptr == PNULL")
TRACE_MSG(MMISET_SECURITY_3879_112_2_18_2_51_58_314,"mmiset pwd_ptr== PNULL")
TRACE_MSG(MMISET_SECURITY_4159_112_2_18_2_51_58_315,"[MMIPHONE]InitSIMLockMenuControl get sim lock data fail!!")
TRACE_MSG(MMISET_SECURITY_4249_112_2_18_2_51_59_316,"[MMIPHONE]UpdateSIMLockEditControl get sim lock data fail!!")
TRACE_MSG(MMISET_SECURITY_4720_112_2_18_2_52_0_317,"AppendAutoKeylockTimeListItem:keylock type is %d error!")
TRACE_MSG(MMISET_SECURITY_4747_112_2_18_2_52_0_318,"mmisetting_assert SetAudoKeyLockTime cur_selection %d")
TRACE_MSG(MMISET_SECURITY_5569_112_2_18_2_52_1_319,"start PHONEBOOK_SetFDNService true")
TRACE_MSG(MMISET_SECURITY_5574_112_2_18_2_52_1_320,"start PHONEBOOK_SetFDNService false")
TRACE_MSG(MMISET_SECURITY_5581_112_2_18_2_52_1_321,"HandleSetFDNStateWindow err_code=%d")
TRACE_MSG(MMISET_SECURITY_5712_112_2_18_2_52_2_322,"start PHONEBOOK_SetFDNService true")
TRACE_MSG(MMISET_SECURITY_5717_112_2_18_2_52_2_323,"start PHONEBOOK_SetFDNService false")
TRACE_MSG(MMISET_SECURITY_5724_112_2_18_2_52_2_324,"HandleSetFDNStateWindow err_code=%d")
TRACE_MSG(MMISET_SECURITY_5869_112_2_18_2_52_2_325,"start PHONEBOOK_SetFDNService true")
TRACE_MSG(MMISET_SECURITY_5874_112_2_18_2_52_2_326,"start PHONEBOOK_SetFDNService false")
TRACE_MSG(MMISET_SECURITY_5883_112_2_18_2_52_2_327,"HandleSetFDNStateWindow err_code=%d")
TRACE_MSG(MMISET_SECURITY_6277_112_2_18_2_52_3_328,"OperatePowerOnPwd: operate_type %d is error!")
TRACE_MSG(MMISET_SECURITY_7496_112_2_18_2_52_5_329,"HandleBlacklistOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISET_SECURITY_8514_112_2_18_2_52_7_330,"MMISET: MMIAPISET_AddBlacklistFromExt,pb_name_ptr ||pb_num_ptr is NULL")
TRACE_MSG(MMISET_SHORTCUT_766_112_2_18_2_52_10_331,"mmisetting_assert ResetDirectionKeyInfo")
TRACE_MSG(MMISET_SHORTCUT_795_112_2_18_2_52_10_332,"mmisetting_assert GetDirectionKeyInfo")
TRACE_MSG(MMISET_SHORTCUT_820_112_2_18_2_52_10_333,"mmisetting_assert GetDirectionKey index %d")
TRACE_MSG(MMISET_SHORTCUT_847_112_2_18_2_52_10_334,"mmisetting_assert SetDirectionKey index %d")
TRACE_MSG(MMISET_SHORTCUT_874_112_2_18_2_52_10_335,"mmisetting_assert SetIdleShortcutInfo index %d")
TRACE_MSG(MMISET_SHORTCUT_932_112_2_18_2_52_10_336,"mmisetting_assert ResetIdleShortcutInfo")
TRACE_MSG(MMISET_SHORTCUT_986_112_2_18_2_52_10_337,"mmisetting_assert GetIdleShortcutInfo")
TRACE_MSG(MMISET_SHORTCUT_1015_112_2_18_2_52_10_338,"mmisetting_assert GetEnabledFuncKeyShortcutInfo")
TRACE_MSG(MMISET_SHORTCUT_1048_112_2_18_2_52_10_339,"mmisetting_assert SetFuncKeyShortcutInfo index %d, type %d")
TRACE_MSG(MMISET_SHORTCUT_1070_112_2_18_2_52_10_340,"mmisetting_assert GetFunctionKey index %d")
TRACE_MSG(MMISET_SHORTCUT_1094_112_2_18_2_52_10_341,"MMIAPISET_OpenDirectionKeyWin, dcd is open and skip the key msg!")
TRACE_MSG(MMISET_SHORTCUT_1100_112_2_18_2_52_10_342,"MMIAPISET_OpenDirectionKeyWin selected = %d, index = %d")
TRACE_MSG(MMISET_SHORTCUT_1280_112_2_18_2_52_11_343,"MMIAPISET_OpenFuncKeyWin ")
TRACE_MSG(MMISET_SHORTCUT_1491_112_2_18_2_52_11_344,"mmisetting_assert GetShorcutActionTypeAndOffset")
TRACE_MSG(MMISET_THEME_143_112_2_18_2_52_13_345,"mmisetting_assert AppendFileTheme")
TRACE_MSG(MMISET_THEME_256_112_2_18_2_52_13_346,"mmisetting_assert MMIAPISET_IniCurrentTheme")
TRACE_MSG(MMISET_VIDEOWALLPAPER_383_112_2_18_2_52_15_347,"[MMISETVWP]:GetNextVideo total_num ERROR!!!!!!!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_407_112_2_18_2_52_15_348,"[MMISETVWP]:GetNextVideo h=%d,w=%d,v_support=%d,video_type=%d,index=%d")
TRACE_MSG(MMISET_VIDEOWALLPAPER_441_112_2_18_2_52_15_349,"[MMISETVWP]:GetNextVideo h=%d,w=%d,v_support=%d,video_type=%d,index=%d")
TRACE_MSG(MMISET_VIDEOWALLPAPER_463_112_2_18_2_52_15_350,"[MMISETVWP]:GetNextVideo no support video!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_815_112_2_18_2_52_16_351,"[MMISETVWP]: HandleVideoListWinMsg need_item_data_ptr is null")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1024_112_2_18_2_52_16_352,"[MMISETVWP]: VideoList_AddItem")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1272_112_2_18_2_52_17_353,"[MMISETVWP]:VideoListWriteFile FAIL, fetal error!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1284_112_2_18_2_52_17_354,"[MMISETVWP]:VideoListWriteFile ERROR!!!!!!!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1356_112_2_18_2_52_17_355,"[MMISETVWP]:VideoListReadFile ERROR!!!!!!!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1363_112_2_18_2_52_17_356,"[MMISETVWP]:VideoListReadFile empty!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1518_112_2_18_2_52_17_357,"[MMISETVWP]: GetVideoItemByIndex fail!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1567_112_2_18_2_52_17_358,"[MMISETVWP]:SetVideoRandomSetting is_random=%d")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1578_112_2_18_2_52_17_359,"[MMISETVWP]:GetVideoAudioSetting is_audio_support=%d")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1588_112_2_18_2_52_17_360,"[MMISETVWP]:SetVideoAudioSetting is_audio_support=%d")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1600_112_2_18_2_52_17_361,"[MMISETVWP]:CheckAllVideoIsInValid entry")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1627_112_2_18_2_52_17_362,"[MMISETVWP] HandleDynWallpaperPlayMsg msg_ptr->msg_id = %d")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1633_112_2_18_2_52_17_363,"[MMISETVWP] HandleDynWallpaperPlayMsg play_end_result = %d")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1693_112_2_18_2_52_17_364,"[MMISETVWP]:GetDisplayParam KEEP WIDTH AND HEIGHT")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1697_112_2_18_2_52_17_365,"[MMISETVWP]:GetDisplayParam CHANGE dy=%d")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1709_112_2_18_2_52_17_366,"[MMISETVWP]:GetDisplayParam LCD_ANGLE_0")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1714_112_2_18_2_52_17_367,"[MMISETVWP]:GetDisplayParam LCD_ANGLE_90")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1738_112_2_18_2_52_17_368,"[MMISETVWP]:PlayNextVideoWallpaper() UdiskIsRun!!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1779_112_2_18_2_52_18_369,"[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1782_112_2_18_2_52_18_370,"[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play fail!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1788_112_2_18_2_52_18_371,"[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play TRUE!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_1815_112_2_18_2_52_18_372,"[MMISETVWP]:PlayNextVideoWallpaper() not found!")
TRACE_MSG(MMISET_VIDEOWALLPAPER_2038_112_2_18_2_52_18_373,"[MMIAPISETWP]: LCD SWITCH")
TRACE_MSG(MMISET_VIDEOWALLPAPER_2089_112_2_18_2_52_18_374,"[MMIAPISETWP] AdjustVolCallBack volue = %d")
TRACE_MSG(MMISET_WALLPAPER_399_112_2_18_2_52_20_375,"pda HandleSetWallpaperWindow unknow index")
TRACE_MSG(MMISET_WALLPAPER_970_112_2_18_2_52_21_376,"mmisetting_assert HandlePreviewWallpaperWindow g_cur_wallpaper_index %d")
TRACE_MSG(MMISET_WINTAB_614_112_2_18_2_52_23_377,"mmisetting_assert MMIAPISET_OpenFixCallRingWin")
TRACE_MSG(MMISET_WINTAB_754_112_2_18_2_52_23_378,"mmisetting_assert HandleSelectFixedCallRingWindow")
TRACE_MSG(MMISET_WINTAB_757_112_2_18_2_52_23_379,"[MMISET]:HandleSelectFixedCallRingWindow, msg = 0x%x")
TRACE_MSG(MMISET_WINTAB_1129_112_2_18_2_52_24_380,"EnterRingSettingMenu:mainmenu_style %d is error!")
END_TRACE_MAP(MMI_APP_SETTING_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_SETTING_TRC_H_

