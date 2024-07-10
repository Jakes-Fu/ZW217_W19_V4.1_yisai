/******************************************************************************
 ** File Name:      mmi_app_phone_trc.h                                         *
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
//trace_id:208
#ifndef _MMI_APP_PHONE_TRC_H_
#define _MMI_APP_PHONE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPHONE_2039_112_2_18_2_45_3_0 "VirWin_HandleMsg THE msg_id = 0x%04x"
#define MMIPHONE_2044_112_2_18_2_45_3_1 "HandlePsAndRefMsg:receive SD 0x%x"
#define MMIPHONE_2090_112_2_18_2_45_3_2 "VirWin_HandleMsg: received MSG_ALM_ALERTING_END"
#define MMIPHONE_2103_112_2_18_2_45_3_3 "VirWin_HandleMsg: received MSG_MP3_PLAY_CALL_BACK"
#define MMIPHONE_2109_112_2_18_2_45_3_4 "VirWin_HandleMsg: received MSG_MP3_FATAL_ERROR"
#define MMIPHONE_2114_112_2_18_2_45_3_5 "VirWin_HandleMsg: received [MMIMP3] MSG_MP3_START_LIRIC"
#define MMIPHONE_2119_112_2_18_2_45_3_6 "VirWin_HandleMsg: received [MMIMP3] MSG_MP3_FOLDER_FULL"
#define MMIPHONE_2124_112_2_18_2_45_3_7 "VirWin_HandleMsg: received MSG_KUR_PLAY_CALL_BACK"
#define MMIPHONE_2129_112_2_18_2_45_3_8 "VirWin_HandleMsg: received MSG_KUR_FATAL_ERROR"
#define MMIPHONE_2134_112_2_18_2_45_3_9 "VirWin_HandleMsg: received MSG_MPEG4_DELETE_ONE"
#define MMIPHONE_2138_112_2_18_2_45_3_10 "VirWin_HandleMsg: MMIAPIMMS_ActiveRecvSendModule"
#define MMIPHONE_2302_112_2_18_2_45_4_11 "virwin_MSG_TTS_AUDIO_PLAY"
#define MMIPHONE_2306_112_2_18_2_45_4_12 "virwin_MSG_TTS_AUDIO_STOP"
#define MMIPHONE_2311_112_2_18_2_45_4_13 "virwin_MSG_TTS_OVER"
#define MMIPHONE_2318_112_2_18_2_45_4_14 "virwin_MSG_TTS_AUDIO_PLAY"
#define MMIPHONE_2430_112_2_18_2_45_4_15 "HandlePsAndRefMsg:receive APP_MN_NETWORK_STATUS_IND"
#define MMIPHONE_2438_112_2_18_2_45_4_16 "HandlePsAndRefMsg:receive APP_MN_NETWORK_INFO_IND"
#define MMIPHONE_2446_112_2_18_2_45_4_17 "HandlePsAndRefMsg:receive APP_MN_PREFER_RAT_RSP"
#define MMIPHONE_2451_112_2_18_2_45_4_18 "HandlePsAndRefMsg:receive APP_MN_PLMN_LIST_CNF"
#define MMIPHONE_2458_112_2_18_2_45_4_19 "HandlePsAndRefMsg:receive APP_MN_PLMN_SELECT_CNF"
#define MMIPHONE_2466_112_2_18_2_45_4_20 "HandlePsAndRefMsg:receive APP_MN_DISPLAY_PLMN_IND"
#define MMIPHONE_2475_112_2_18_2_45_4_21 "HandlePsAndRefMsg:receive APP_MN_SIM_PRESENT_IND"
#define MMIPHONE_2480_112_2_18_2_45_4_22 "HandlePsAndRefMsg:receive APP_MN_SIM_PLUG_IN_IND"
#define MMIPHONE_2486_112_2_18_2_45_4_23 "HandlePsAndRefMsg:receive APP_MN_LND_READY_IND"
#define MMIPHONE_2490_112_2_18_2_45_4_24 "HandlePsAndRefMsg:receive APP_MN_LND_UPDATE_CNF"
#define MMIPHONE_2496_112_2_18_2_45_4_25 "HandlePsAndRefMsg:receive APP_MN_SET_BAND_CNF"
#define MMIPHONE_2504_112_2_18_2_45_4_26 "HandlePsAndRefMsg:receive APP_MN_SIM_GET_PIN_IND"
#define MMIPHONE_2514_112_2_18_2_45_4_27 "HandlePsAndRefMsg:dual_sys=%d"
#define MMIPHONE_2533_112_2_18_2_45_4_28 "HandlePsAndRefMsg:receive APP_MN_SIM_PIN_FUNC_CNF"
#define MMIPHONE_2539_112_2_18_2_45_4_29 "HandlePsAndRefMsg:receive APP_MN_SIM_READY_IND"
#define MMIPHONE_2565_112_2_18_2_45_4_30 "HandlePsAndRefMsg:receive APP_MN_FDN_SERVICE_IND"
#define MMIPHONE_2570_112_2_18_2_45_4_31 "HandlePsAndRefMsg:receive APP_MN_PS_READY_IND"
#define MMIPHONE_2577_112_2_18_2_45_4_32 "HandlePsAndRefMsg:receive APP_MN_SIM_NOT_READY_IND"
#define MMIPHONE_2589_112_2_18_2_45_5_33 "HandlePsAndRefMsg:receive APP_MN_SCELL_RSSI_IND"
#define MMIPHONE_2594_112_2_18_2_45_5_34 "HandlePsAndRefMsg:receive APP_MN_PS_POWER_ON_CNF"
#define MMIPHONE_2601_112_2_18_2_45_5_35 "HandlePsAndRefMsg:receive APP_MN_PS_POWER_OFF_CNF"
#define MMIPHONE_2606_112_2_18_2_45_5_36 "HandlePsAndRefMsg:receive APP_MN_DEACTIVE_PS_CNF"
#define MMIPHONE_2611_112_2_18_2_45_5_37 "HandlePsAndRefMsg:receive APP_MN_SIM_POWER_ON_CNF"
#define MMIPHONE_2622_112_2_18_2_45_5_38 "HandlePsAndRefMsg:receive APP_MN_SIM_POWER_OFF_CNF"
#define MMIPHONE_2629_112_2_18_2_45_5_39 "HandlePsAndRefMsg:receive APP_MN_PREFER_PLMN_CHANGE_CNF"
#define MMIPHONE_2637_112_2_18_2_45_5_40 "HandlePuctInfoCnf: receive APP_MN_PUCT_INFO_CNF"
#define MMIPHONE_2642_112_2_18_2_45_5_41 "None Handle: receive APP_MN_AOC_MODIFIED_IND"
#define MMIPHONE_2671_112_2_18_2_45_5_42 "HandlePsAndRefMsg:receive MISC_MSG_RTC_ALARM"
#define MMIPHONE_2682_112_2_18_2_45_5_43 "HandlePsAndRefMsg:receive UPM_PLUGIN_IND"
#define MMIPHONE_2688_112_2_18_2_45_5_44 "HandlePsAndRefMsg:receive UPM_PLUGOUT_IND"
#define MMIPHONE_2722_112_2_18_2_45_5_45 "HandlePsAndRefMsg:receive UPM_READY_IND"
#define MMIPHONE_2729_112_2_18_2_45_5_46 "HandlePsAndRefMsg:receive UPM_ERROR_IND"
#define MMIPHONE_2735_112_2_18_2_45_5_47 "HandlePsAndRefMsg:receive APP_AUDIO_END_IND"
#define MMIPHONE_2741_112_2_18_2_45_5_48 "HandlePsAndRefMsg:receive APP_WRE_AUDIO_END_IND"
#define MMIPHONE_2759_112_2_18_2_45_5_49 "HandlePsAndRefMsg(),receive APP_JAVA_MP4_DISPLAY_IND!"
#define MMIPHONE_2764_112_2_18_2_45_5_50 "HandlePsAndRefMsg(),receive APP_JAVA_MP4_END_IND!"
#define MMIPHONE_2768_112_2_18_2_45_5_51 "HandlePsAndRefMsg(),receive APP_JAVA_SNAPSHOT_END_IND!"
#define MMIPHONE_2790_112_2_18_2_45_5_52 "apple phone param = %x"
#define MMIPHONE_2794_112_2_18_2_45_5_53 "HandlePsAndRefMsg msg_id = %x"
#define MMIPHONE_2798_112_2_18_2_45_5_54 "HandlePsAndRefMsg MSG_JAVA_CLOSE_APP_WIN param = %d, %x"
#define MMIPHONE_2805_112_2_18_2_45_5_55 "HandlePsAndRefMsg MSG_JAVA_AUTORUNAPP param = %x"
#define MMIPHONE_2809_112_2_18_2_45_5_56 "HandlePsAndRefMsg MSG_JAVA_SEND_TIMER param = %d %d"
#define MMIPHONE_2813_112_2_18_2_45_5_57 "HandlePsAndRefMsg MSG_JAVA_LINK_TIMER param = %d %d"
#define MMIPHONE_2817_112_2_18_2_45_5_58 "HandlePsAndRefMsg MSG_JAVA_CONNECT_TIMER param = %d %d"
#define MMIPHONE_2821_112_2_18_2_45_5_59 "HandlePsAndRefMsg MSG_JAVA_RECEIVE_TIMER param = %d %d"
#define MMIPHONE_2825_112_2_18_2_45_5_60 "HandlePsAndRefMsg MSG_JAVA_STOP_SOCKET_TIMER param = %d %d"
#define MMIPHONE_2840_112_2_18_2_45_5_61 "HandlePsAndRefMsg MSG_JAVA_VM_SUSPENDED_IND param = %d %d"
#define MMIPHONE_2844_112_2_18_2_45_5_62 "HandlePsAndRefMsg MSG_JAVA_MEDIA_STOPTIME_TIMER param = %d %d"
#define MMIPHONE_2848_112_2_18_2_45_5_63 "HandlePsAndRefMsg MSG_JAVA_DEACTIVE_PDP param = %d %d"
#define MMIPHONE_2852_112_2_18_2_45_5_64 "HandlePsAndRefMsg MSG_JAVA_PLATFORM_REQUEST param = %d %d"
#define MMIPHONE_2856_112_2_18_2_45_5_65 "HandlePsAndRefMsg MSG_JAVA_AUDIO_PLAY_AGAIN param = %d %d"
#define MMIPHONE_2931_112_2_18_2_45_5_66 "flash_printf:@@@@@MSG_FLASH_UPDATE_WINDOW  0  ,received the message."
#define MMIPHONE_2934_112_2_18_2_45_5_67 "flash_printf:@@@@@MSG_FLASH_UPDATE_WINDOW  1  ,received the message."
#define MMIPHONE_2941_112_2_18_2_45_5_68 "HandlePsAndRefMsg:receive SNTP_SIG_GET_TIME_CNF"
#define MMIPHONE_2946_112_2_18_2_45_5_69 "HandlePsAndRefMsg:msg_id = 0x%04x"
#define MMIPHONE_2992_112_2_18_2_45_5_70 "MMIAPIPHONE_InterrogateCfuForIdle dual_sys=%d,cfu_dual_sys=%d"
#define MMIPHONE_3012_112_2_18_2_45_6_71 "MMIAPIPHONE_InterrogateCfuForIdle is_allow_do_cfu=%d"
#define MMIPHONE_3145_112_2_18_2_45_6_72 "MMIAPIPHONE_SetIdleGprsState err dual_sys: %d"
#define MMIPHONE_3149_112_2_18_2_45_6_73 "[dual mode] MMIAPIPHONE_SetIdleGprsState: rat = %d"
#define MMIPHONE_3201_112_2_18_2_45_6_74 "MMIAPIPHONE_SetIdleGprsState dual_sys=%d"
#define MMIPHONE_3205_112_2_18_2_45_6_75 "MMIAPIPHONE_SetIdleGprsState err dual_sys: %d"
#define MMIPHONE_3261_112_2_18_2_45_6_76 "StoreNetworkStatus error dual_sys = %d"
#define MMIPHONE_3307_112_2_18_2_45_6_77 "NetworkStatus g:%d, e:%d, h:%d"
#define MMIPHONE_3407_112_2_18_2_45_6_78 "MMIAPIPHONE_GenPLMNDisplay plmn_ptr=%d, display_str=%d, display_str_len=%d"
#define MMIPHONE_3412_112_2_18_2_45_6_79 "MMIAPIPHONE_GenPLMNDisplay mcc=%d, mnc=%d, mnc_digit_num=%d, str_len=%d, is_display_digit_format=%d,rat=%d"
#define MMIPHONE_3474_112_2_18_2_45_6_80 "MMIAPIPHONE_GenPLMNDisplay is_digit_output=%d, network_name_id=%x"
#define MMIPHONE_3508_112_2_18_2_45_7_81 "HandlePlmnListCnf error dual_sys = %d"
#define MMIPHONE_3634_112_2_18_2_45_7_82 "HandlePowerOnSimCnf:dual_sys=%d,is_ok=%d"
#define MMIPHONE_3638_112_2_18_2_45_7_83 "HandlePowerOnSimCnf error dual_sys = %d"
#define MMIPHONE_3759_112_2_18_2_45_7_84 "HandlePowerOnSimCnf: Idle is not open"
#define MMIPHONE_3840_112_2_18_2_45_7_85 "HandlePowerOnSimCnf,dual_sys=%d,is_prefer_set=%d"
#define MMIPHONE_3874_112_2_18_2_45_7_86 "HandlePowerOffSimCnf:dual_sys=%d,is_ok=%d"
#define MMIPHONE_3915_112_2_18_2_45_7_87 "HandlePowerOffSimCnf:dual_sys=%d,is_ok=%d"
#define MMIPHONE_3939_112_2_18_2_45_7_88 "HandleSelectPlmnCnf error dual_sys = %d"
#define MMIPHONE_3984_112_2_18_2_45_8_89 "HandleSelectBandCnf band_select_cnf.is_ok = %d"
#define MMIPHONE_4016_112_2_18_2_45_8_90 "GetNetWorkNameId: the network plmn_ptr is error! %d"
#define MMIPHONE_4057_112_2_18_2_45_8_91 "GetNetWorkNameId: the network status is error! %d"
#define MMIPHONE_4084_112_2_18_2_45_8_92 "HandleSimPinInd:error dual_sys=%d"
#define MMIPHONE_4267_112_2_18_2_45_8_93 "HandleSimPinCnf:pin_operate_result=%d, pin_num=%d, pin_function_type=%d"
#define MMIPHONE_4715_112_2_18_2_45_9_94 "StoreSimReadyStatus:error dual_sys=%d"
#define MMIPHONE_4786_112_2_18_2_45_9_95 "StoreSimNotReadyStatus:error dual_sys=%d"
#define MMIPHONE_5110_112_2_18_2_45_10_96 "HandleScellRssiInd:error dual_sys=%d"
#define MMIPHONE_5199_112_2_18_2_45_10_97 "MMIAPIPHONE_PowerOff:restart_condition=%d"
#define MMIPHONE_5209_112_2_18_2_45_10_98 "MMIPHONE_PowerOff PHONE_SIM_LOCK_COUNTDOWN_WIN_ID open"
#define MMIPHONE_5220_112_2_18_2_45_10_99 "MMIPHONE_PowerOff MMIAPIMTV_IsMTVRunning delay poweroff"
#define MMIPHONE_5298_112_2_18_2_45_10_100 "MMIAPIPHONE_GetNetworkName err dual_sys: %d"
#define MMIPHONE_5561_112_2_18_2_45_11_101 "MMIAPIPHONE_GetRoamingStatus:error dual_sys=%d"
#define MMIPHONE_5583_112_2_18_2_45_11_102 "roaming status: %d, net isroaming: %d, net mcc: %d, sim mcc: %d "
#define MMIPHONE_5652_112_2_18_2_45_11_103 "GetOpnString dual_sys = %d OR opn_str_ptr is NULL"
#define MMIPHONE_5736_112_2_18_2_45_11_104 "GetOpnString dual_sys = %d OR spn_str_ptr is NULL"
#define MMIPHONE_5905_112_2_18_2_45_12_105 "MMIAPIPHONE_IsNetworkSelecting dual_sys = %d"
#define MMIPHONE_5911_112_2_18_2_45_12_106 "MMIAPIPHONE_IsNetworkSelecting plmn_status=%d"
#define MMIPHONE_5936_112_2_18_2_45_12_107 "GetSimAndServiceString: the network_str_ptr || network_str_ptr->wstr_ptr ||dual_sys"
#define MMIPHONE_5942_112_2_18_2_45_12_108 "MMIAPIPHONE_GetSimAndServiceString s_sim_status[%d].simStatus %d,is_opn_need=%d,opn_len=%d,is_opn_exist=%d,is_spn_need=%d,spn_len=%d"
#define MMIPHONE_6032_112_2_18_2_45_12_109 "MMIAPIPHONE_GetSimStatus err dual sys: %d"
#define MMIPHONE_6045_112_2_18_2_45_12_110 "MMIPHONE_SetSimStatus:dual sys = %d, dual status = %d"
#define MMIPHONE_6053_112_2_18_2_45_12_111 "MMIPHONE_SetSimStatus err dual sys: %d"
#define MMIPHONE_6066_112_2_18_2_45_12_112 "MMIAPIPHONE_SetSimExistedStatus:dual sys = %d, is_existed = %d"
#define MMIPHONE_6081_112_2_18_2_45_12_113 "MMIAPIPHONE_SetSimExistedStatus:err dual sys = %d"
#define MMIPHONE_6120_112_2_18_2_45_12_114 "MMIAPIPHONE_IsSimOk err dual sys: %d"
#define MMIPHONE_6136_112_2_18_2_45_12_115 "MMIAPIPHONE_IsSimCardOk err dual sys: %d"
#define MMIPHONE_6153_112_2_18_2_45_12_116 "MMIAPIPHONE_IsSimAvailable err dual sys: %d"
#define MMIPHONE_6302_112_2_18_2_45_13_117 "GetSysOkNum: the p=%d || idx_ptr = %d"
#define MMIPHONE_6359_112_2_18_2_45_13_118 "MMIAPIPHONE_AlertSimNorOKStatus:error simStatus=%d"
#define MMIPHONE_6365_112_2_18_2_45_13_119 "MMIAPIPHONE_AlertSimNorOKStatus:error dual_sys=%d"
#define MMIPHONE_6474_112_2_18_2_45_13_120 "MMIAPIPHONE_AlertSimNorOKStatus err sim status: %d"
#define MMIPHONE_6488_112_2_18_2_45_13_121 "MMIAPIPHONE_IsPin2RequiredACM err dual sys: %d"
#define MMIPHONE_6502_112_2_18_2_45_13_122 "MMIAPIPHONE_IsPin2RequiredACMMax err dual sys: %d"
#define MMIPHONE_6517_112_2_18_2_45_13_123 "MMIAPIPHONE_IsPin2RequiredPuct err dual sys: %d"
#define MMIPHONE_6532_112_2_18_2_45_13_124 "MMIAPIPHONE_IsAocSupport err dual sys: %d"
#define MMIPHONE_6542_112_2_18_2_45_13_125 "MMIAPIPHONE_IsAocSupport aoc_is_support=%d"
#define MMIPHONE_6559_112_2_18_2_45_13_126 "MMIAPIPHONE_GetPlmnStatus err dual sys: %d"
#define MMIPHONE_6576_112_2_18_2_45_13_127 "mmiphone:CreateInputPinWindow dual_sys=%d"
#define MMIPHONE_6636_112_2_18_2_45_13_128 "GetSimAndServiceString: the is_sim1_ok_ptr=%d || is_sim2_ok_ptr =%d "
#define MMIPHONE_6656_112_2_18_2_45_13_129 "MMIAPIPHONE_GetDualSimStatus, is_sim1_ok=%d, is_sim2_ok=%d"
#define MMIPHONE_6695_112_2_18_2_45_13_130 "MMIAPIENG_GetFtaFlag() = %d"
#define MMIPHONE_6802_112_2_18_2_45_14_131 "MMIAPIPHONE_SetGPRSDetach e_dualsys_setting = %d"
#define MMIPHONE_6823_112_2_18_2_45_14_132 "MMIAPIPHONE_SetGPRSDetachEx dual_sys = %d"
#define MMIPHONE_6854_112_2_18_2_45_14_133 "MMIAPIPHONE_StartUpPsAccordingToSetting, start power on sim"
#define MMIPHONE_6888_112_2_18_2_45_14_134 "MMIAPIPHONE_StartUpPsAccordingToSetting, e_dualsys_setting=%d"
#define MMIPHONE_6968_112_2_18_2_45_14_135 "PowerOnPsForEmerge, s_ps_ready[%d]=%d"
#define MMIPHONE_6977_112_2_18_2_45_14_136 "PowerOnPsForEmerge Fly mode!"
#define MMIPHONE_7000_112_2_18_2_45_14_137 "MMIAPIPHONE_PowerOnPs, dual_sys=%d ps is already"
#define MMIPHONE_7009_112_2_18_2_45_14_138 "MMIAPIPHONE_PowerOnPs Fly mode!"
#define MMIPHONE_7030_112_2_18_2_45_14_139 "MMIAPIPHONE_PowerOffPs, dual_sys=%d,s_ps_ready=%d"
#define MMIPHONE_7060_112_2_18_2_45_14_140 "MMIAPIPHONE_SetIsPsDeactiveCnf err dual sys: %d"
#define MMIPHONE_7075_112_2_18_2_45_14_141 "MMIAPIPHONE_GetIsPsDeactiveCnf err dual sys: %d"
#define MMIPHONE_7090_112_2_18_2_45_14_142 "MMIAPIPHONE_SetIsPsReady err dual sys: %d"
#define MMIPHONE_7105_112_2_18_2_45_14_143 "MMIAPIPHONE_GetIsPsReady err dual sys: %d"
#define MMIPHONE_7118_112_2_18_2_45_14_144 "MMIAPIPHONE_GetCurResponsePinSimID:sim_id=%d"
#define MMIPHONE_7129_112_2_18_2_45_14_145 "MMIAPIPHONE_GetCurHandlingPinSimID:sim_id=%d"
#define MMIPHONE_7152_112_2_18_2_45_14_146 "MMIAPIPHONE_InitTDOrGsm configue_rat=%d"
#define MMIPHONE_7263_112_2_18_2_45_14_147 "HandleSimPresentInd:error dual_sys=%d"
#define MMIPHONE_7267_112_2_18_2_45_14_148 "HandleSimPresentInd:dual_sys:%d, sim_sms_num:%d"
#define MMIPHONE_7292_112_2_18_2_45_15_149 "HandleLdnReadyInd:dual_sys:%d,is_lnd_ok=%d"
#define MMIPHONE_7313_112_2_18_2_45_15_150 "HandleSimPlugInInd:error dual_sys=%d"
#define MMIPHONE_7316_112_2_18_2_45_15_151 "HandleSimPlugInInd:dual_sys:%d,"
#define MMIPHONE_7381_112_2_18_2_45_15_152 "NotifySIMPlugInd dual_sys=%d, notify_event=%d"
#define MMIPHONE_7390_112_2_18_2_45_15_153 "NotifySIMPlugInd Intial not start, donot handle!"
#define MMIPHONE_7515_112_2_18_2_45_15_154 "MMIAPIPHONE_PowerOnSIM, start power on sim dual_sys=%d"
#define MMIPHONE_7533_112_2_18_2_45_15_155 "MMIAPIPHONE_DeactivePS, dual_sys=%d,s_ps_ready=%d"
#define MMIPHONE_7595_112_2_18_2_45_15_156 "HandleSetStandbyMdoeWaitingWindow msg_id = %d"
#define MMIPHONE_7662_112_2_18_2_45_15_157 "ImplementStandByModeAsync s_plug_sim_id=%d, s_plug_event=%d"
#define MMIPHONE_7698_112_2_18_2_45_15_158 "MMIAPIPHONE_SetStandByMode dual_sys=%d, plug_event=%d, e_sys_setting=%d"
#define MMIPHONE_7851_112_2_18_2_45_16_159 "MMIAPIPHONE_GetDataServiceType: %d"
#define MMIPHONE_7865_112_2_18_2_45_16_160 "NotifyRxLevelChange error dual_sys: %d"
#define MMIPHONE_7870_112_2_18_2_45_16_161 "NotifyRxLevelChange rx_level=%d, s_notify_rx_level=%d, PlmnStatus=%d"
#define MMIPHONE_7917_112_2_18_2_45_16_162 "NotifyPlmnStatusChange plmn_status=%d, s_notify_plmn_status=%d, rssiLevel=%d"
#define MMIPHONE_8028_112_2_18_2_45_16_163 "NotifyServiceChange dual_sys=%d, notify_event=%d"
#define MMIPHONE_8157_112_2_18_2_45_17_164 "MMIAPIPHONE_IsPSAvail error dual_sys: %d"
#define MMIPHONE_8195_112_2_18_2_45_17_165 "MMIAPIPHONE_PowerOnPsEx s_is_power_on_sim_cnf[%d]=%d"
#define MMIPHONE_8260_112_2_18_2_45_17_166 "MMIAPIPHONE_IsRegNetSuccess err dual sys: %d"
#define MMIPHONE_8276_112_2_18_2_45_17_167 "MMIAPIPHONE_IsAttachGprsSuccess err dual sys: %d"
#define MMIPHONE_8303_112_2_18_2_45_17_168 "MMIAPIPHONE_OpeartorTextID param error"
#define MMIPHONE_8423_112_2_18_2_45_17_169 "mmiphone:CreateInputSimLockWindow opreate_sim_lock_ptr = NULL"
#define MMIPHONE_8427_112_2_18_2_45_17_170 "mmiphone:CreateInputSimLockWindow dual_sys=%d"
#define MMIPHONE_8534_112_2_18_2_45_18_171 "MMIAPIPHONE_AppendPlmnList,index=%d"
#define MMIPHONE_8589_112_2_18_2_45_18_172 "MMIAPIPHONE_AppendPreferPlmnList,index=%d"
#define MMIPHONE_8643_112_2_18_2_45_18_173 "GetSimAndServiceString: the prefer_network =%d "
#define MMIPHONE_8647_112_2_18_2_45_18_174 "MMIAPIPHONE_GetPlmnInfoByIndex,index=%d"
#define MMIPHONE_8683_112_2_18_2_45_18_175 "ccapp.c HandleLDNUpdateCnf,type= %d, result =%d, dual_sys= %d"
#define MMIPHONE_8760_112_2_18_2_45_18_176 "[mmiphone]: MMIAPIPHONE_GetAllSimStateOKCount, active_sim_count=%d"
#define MMIPHONE_CHARGE_307_112_2_18_2_45_19_177 "MMIAPIPHONE_ChargeInit,is_charge =%d,dbat_info->module_state=%d"
#define MMIPHONE_CHARGE_346_112_2_18_2_45_19_178 "HandlePsAndRefMsg:receive DBAT_MSG_UPDATE_STATE_IND"
#define MMIPHONE_CHARGE_351_112_2_18_2_45_19_179 "HandlePsAndRefMsg:receive DBAT_MSG_BAT_PLUGIN_IND"
#define MMIPHONE_CHARGE_355_112_2_18_2_45_19_180 "HandlePsAndRefMsg:receive DBAT_MSG_BAT_PLUGOUT_IND"
#define MMIPHONE_CHARGE_360_112_2_18_2_45_19_181 "HandlePsAndRefMsg:receive DBAT_MSG_SWITCH_BAT_IND"
#define MMIPHONE_CHARGE_363_112_2_18_2_45_19_182 "HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_START_IND"
#define MMIPHONE_CHARGE_368_112_2_18_2_45_19_183 "HandlePsAndRefMsg:receive DBAT_MSG_WARNING_IND"
#define MMIPHONE_CHARGE_372_112_2_18_2_45_19_184 "HandlePsAndRefMsg:receive DBAT_MSG_SHUTDOWN_IND"
#define MMIPHONE_CHARGE_396_112_2_18_2_45_19_185 "HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_FINISH"
#define MMIPHONE_CHARGE_400_112_2_18_2_45_19_186 "HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_DISCONNECT"
#define MMIPHONE_CHARGE_405_112_2_18_2_45_19_187 "HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_FAULT"
#define MMIPHONE_CHARGE_410_112_2_18_2_45_19_188 "HandlePsAndRefMsg:receive CHR_MSG_CAP_IND"
#define MMIPHONE_CHARGE_416_112_2_18_2_45_19_189 "HandlePsAndRefMsg:receive CHR_MSG_CHARGE_START_IND"
#define MMIPHONE_CHARGE_431_112_2_18_2_45_19_190 "HandlePsAndRefMsg:receive CHR_MSG_CHARGE_END_IND"
#define MMIPHONE_CHARGE_446_112_2_18_2_45_19_191 "HandlePsAndRefMsg:receive CHR_MSG_WARNING_IND"
#define MMIPHONE_CHARGE_452_112_2_18_2_45_19_192 "HandlePsAndRefMsg:receive CHR_MSG_SHUTDOWN_IND"
#define MMIPHONE_CHARGE_480_112_2_18_2_45_19_193 "HandlePsAndRefMsg:receive CHR_MSG_CHARGE_FINISH_IND"
#define MMIPHONE_CHARGE_486_112_2_18_2_45_19_194 "HandlePsAndRefMsg:receive CHR_MSG_CHARGE_DISCONNECT_IND"
#define MMIPHONE_CHARGE_501_112_2_18_2_45_19_195 "HandlePsAndRefMsg:receive CHR_MSG_CHARGE_FAULT_IND"
#define MMIPHONE_CHARGE_765_112_2_18_2_45_20_196 "HandleChrWarningInd:s_startup_condition = STARTUP_CHARGE"
#define MMIPHONE_CHARGE_1130_112_2_18_2_45_21_197 "MMIAPIPHONE_GetBatLevel:bat_level=%d,level=%d"
#define MMIPHONE_CHARGE_1151_112_2_18_2_45_21_198 "MMIAPIPHONE_GetBatCapacity bat_level = %d"
#define MMIPHONE_CHARGE_1176_112_2_18_2_45_21_199 "MMIPHONE_GetChargeStatus,result=%d"
#define MMIPHONE_CHARGE_1193_112_2_18_2_45_21_200 "[MMIPHONE]: HandleLowPowerRingWin, msg=%x"
#define MMIPHONE_CHARGE_1883_112_2_18_2_45_22_201 "mmi DisplayBtryCapacityStr exceed str len max"
#define MMIPHONE_CHARGE_1924_112_2_18_2_45_22_202 "mmi DisplayBtryCapacityStr exceed str len max"
#define MMIPHONE_CHARGE_1954_112_2_18_2_45_22_203 "mmi DisplayBtryCapacityStr exceed str len max"
#define MMIPHONE_CHARGE_2007_112_2_18_2_45_23_204 "BtryCapacityCtrlCallbackFun,ERROR!"
#define MMIPHONE_CHARGE_2012_112_2_18_2_45_23_205 "BtryCapacityCtrlCallbackFun,bat_level=%d"
#define MMIPHONE_NITZ_93_112_2_18_2_45_23_206 "[MMIPHONE] DrmAutoUpdateNetworkTime: The phone has update network time after leaving factory."
#define MMIPHONE_NITZ_128_112_2_18_2_45_23_207 "[MMIPHONE]: SetNetworkInfoTimeToSystem , ut_lt_zone_flag =%d daylight_saving_time_flag =%d ,ut_lt_zone.time_zone =%d, local_time_zone= %d"
#define MMIPHONE_NITZ_155_112_2_18_2_45_23_208 "[MMIPHONE]: SetNetworkInfoTimeToSystem , net_second =%d"
#define MMIPHONE_NITZ_169_112_2_18_2_45_24_209 "[MMIPHONE]: SetNetworkInfoTimeToSystem , temp_second2 =%d"
#define MMIPHONE_NITZ_172_112_2_18_2_45_24_210 "[MMIPHONE]: SetNetworkInfoTimeToSystem , net_second2 =%d"
#define MMIPHONE_NITZ_177_112_2_18_2_45_24_211 "[MMIPHONE]: SetNetworkInfoTimeToSystem , DT:%04d-%02d-%02d %02d-%02d-%02d"
#define MMIPHONE_NITZ_195_112_2_18_2_45_24_212 "[MMIPHONE]: SetNetworkInfoTimeToSystem , date time is the same"
#define MMIPHONE_NITZ_206_112_2_18_2_45_24_213 "[MMIPHONE]: SetNetworkInfoTimeToSystem , set time:%04d-%02d-%02d %02d-%02d-%02d"
#define MMIPHONE_NITZ_221_112_2_18_2_45_24_214 "[MMIPHONE]: SetNetworkInfoTimeToSystem, SetTime:time_hour is %d,time_minute is %d is invalid!"
#define MMIPHONE_NITZ_229_112_2_18_2_45_24_215 "[MMIPHONE]: SetNetworkInfoTimeToSystem , TM_SetSysTime status [%d] "
#define MMIPHONE_NITZ_251_112_2_18_2_45_24_216 "[MMIPHONE] :SetNetworkInfoTimeToSystem, SetDate:year is %d,month is %d and day is %d is invalid!"
#define MMIPHONE_NITZ_257_112_2_18_2_45_24_217 "[MMIPHONE]: SetNetworkInfoTimeToSystem , ut_lt_zone_flagis_network_time_exist %d"
#define MMIPHONE_NITZ_277_112_2_18_2_45_24_218 "[MMIPHONE]: ParseSysTimeFromNetworkInfo success , net_time_zone [%d] daylight_saving_time[%d]"
#define MMIPHONE_NITZ_319_112_2_18_2_45_24_219 "[MMIPHONE]: ParseSysTimeFromNetworkInfo , local_time_zone_flag [%d] ut_lt_zone_flag[%d],daylight_saving_time_flag [%d]"
#define MMIPHONE_NITZ_326_112_2_18_2_45_24_220 "[MMIPHONE]: ParseSysTimeFromNetworkInfo , net_time_zone=%d, MMIAPIACC_GetLocalTimeZone() =%f"
#define MMIPHONE_NITZ_343_112_2_18_2_45_24_221 "[MMIPHONE]: AutoUpdateTimeHandleNetworkInfoInd SIM [%d] ut_lt_zone_flag[%d] "
#define MMIPHONE_NITZ_357_112_2_18_2_45_24_222 "[MMIPHONE]:  net no support time and timezone"
#define MMIPHONE_ONOFF_381_112_2_18_2_45_25_223 "[POWERONOFF]:MMIAPIPHONE_IsNetworkSelecting e_start_cond = %d"
#define MMIPHONE_ONOFF_421_112_2_18_2_45_25_224 "[POWERONOFF]:HandleNormalStartupWindow type=%d"
#define MMIPHONE_ONOFF_970_112_2_18_2_45_26_225 "[POWERONOFF]:PlayPowerOnOffMP4:is_power_on=%d"
#define MMIPHONE_ONOFF_1086_112_2_18_2_45_27_226 "[POWERONOFF]:PlayPowerOnOffMP4:anim_type=%d"
#define MMIPHONE_ONOFF_1101_112_2_18_2_45_27_227 "mmiphone_onoff.c, PlayPowerOnMP4CallBack result = %d"
#define MMIPHONE_ONOFF_1185_112_2_18_2_45_27_228 "[POWERONOFF]:HandleVideoPowerOnOffMsg msg_ptr->msg_id = %x"
#define MMIPHONE_ONOFF_1285_112_2_18_2_45_27_229 "[POWERONOFF]:PlayDefaultPowerOnOffAnimation active_mode_value=%d is_power_on_off=%d"
#define MMIPHONE_WINTAB_1209_112_2_18_2_45_30_230 "HandlePinInputingString pin_ptr is NULL"
#define MMIPHONE_WINTAB_1899_112_2_18_2_45_32_231 "HandlePinInputingString pin_ptr=%d ||puk_ptr=%d"
#define MMIPHONE_WINTAB_2208_112_2_18_2_45_32_232 "HandlePinInputingString pin_ptr "
#define MMIPHONE_WINTAB_2222_112_2_18_2_45_32_233 "HandleOperatePin:operate_type is %d,pin_num is %d "
#define MMIPHONE_WINTAB_2865_112_2_18_2_45_34_234 "HandleOperatePukAndNewPin pin_ptr=%d, puk_ptr= %d , operate_pin_info=%d"
#define MMIPHONE_WINTAB_2914_112_2_18_2_45_34_235 "HandleOperatePinAndNewPin pin_ptr=%d, puk_ptr= %d , operate_pin_info=%d"
#define MMIPHONE_WINTAB_2967_112_2_18_2_45_34_236 "HandleOperatePukAndNewPin  pin_puk_str_ptr is NULL"
#define MMIPHONE_WINTAB_2973_112_2_18_2_45_34_237 "MMI_PHONE: pin_puk_remain=%d"
#define MMIPHONE_WINTAB_3035_112_2_18_2_45_34_238 "MMIPHONE_OpenQueryPoweroff=%d"
#define MMIPHONE_WINTAB_3115_112_2_18_2_45_34_239 "AppendIMEIDetail Read MN_DUAL_SYS_%d MNNV_GetIMEISVNEx Error!"
#define MMIPHONE_WINTAB_3153_112_2_18_2_45_35_240 "AppendIMEIDetail SVN Not Exist!"
#define MMIPHONE_WINTAB_3158_112_2_18_2_45_35_241 "AppendIMEIDetail Read MN_DUAL_SYS_1MNNV_GetIMEISVNEx Error!"
#define MMIPHONE_WINTAB_3378_112_2_18_2_45_35_242 "MMIAPIPHONE_SetScrDelCallBack OVERFLOW"
#define MMIPHONE_WINTAB_3699_112_2_18_2_45_36_243 "MMIPHONE:GetSIMLockRemainStr  max_num_trials=%d,time_interval= %d,time_multiplier= %d,cck_trials=%d,nck_trials=%d,nsck_trials=%d,pck_trials=%d,spck_trials=%d"
#define MMIPHONE_WINTAB_3735_112_2_18_2_45_36_244 "[MMIPHONE]: GetSIMLockInputPasswordTitle unlock_title = NULL"
#define MMIPHONE_WINTAB_3791_112_2_18_2_45_36_245 "MMIPHONE:MMIAPIPHONE_GetSIMLockRemainTrialsTimer  max_num_trials=%d,time_interval= %d,time_multiplier= %d,cck_trials=%d,nck_trials=%d,nsck_trials=%d,pck_trials=%d,spck_trials=%d"
#define MMIPHONE_WINTAB_3898_112_2_18_2_45_36_246 "SIMUnlockInputWinSoftkey operate_sim_lock_ptr is NULL"
#define MMIPHONE_WINTAB_3904_112_2_18_2_45_36_247 "SIMUnlockInputWinSoftkey str_ptr || text_id_ptr is NULL"
#define MMIPHONE_WINTAB_3978_112_2_18_2_45_36_248 "HandleSIMUnlockingAlertErrorWindow win_info_ptr =NULL ||win_info_ptr->user_data = NULL"
#define MMIPHONE_WINTAB_4082_112_2_18_2_45_36_249 "HandleSIMUnlockingInputingString sim_unlocking_ptr||sim_unlocking_ptr->wstr_ptr|operate_sim_lock_ptr||sim_unlock_datais null"
#define MMIPHONE_WINTAB_4204_112_2_18_2_45_37_250 "[MMIPHONE] sim_lock_ptr alloc memory fail"
#define MMIPHONE_WINTAB_4228_112_2_18_2_45_37_251 "HandleSIMUnlockingInputingString operate_sim_lock is null"
#define MMIPHONE_WINTAB_4367_112_2_18_2_45_37_252 "[MMIPHONE] get MMI_DUAL_SYS_MAX <= dual_sys fail"
#define MMIPHONE_WINTAB_4381_112_2_18_2_45_37_253 "[MMIPHONE] set dual_sys = %d is_need_restart=%d"
#define MMIPHONE_WINTAB_4586_112_2_18_2_45_37_254 "[MMIPHONE] SetCountdownStartTime result=%d,hour%2d,min%2d,sec%2d"
#define MMIPHONE_WINTAB_4709_112_2_18_2_45_38_255 "CountdownHandleTimer timer_id = %d"
#define MMIPHONE_WINTAB_4801_112_2_18_2_45_38_256 "[MMIPHONE] MMIAPIPHONE_HandleSIMLockSettingMenuFlow opreate_sim_lock_ptr alloc fail!"
#define MMIPHONE_WINTAB_5307_112_2_18_2_45_39_257 "SelectSIM COM WIN,dual_sys[%d]>=MMI_DUAL_SYS_MAX/MMI_DUAL_SYS_MAX+1 in wifi "
#define MMIPHONE_WINTAB_5366_112_2_18_2_45_39_258 "MMIAPIPHONE_OpenSelectSimWin:select sim_input_ptr couldn't be PNULL !"
#define MMIPHONE_WINTAB_5375_112_2_18_2_45_39_259 "MMIAPIPHONE_OpenSelectSimWin:win_id_ptr or  handle_func couldn't be PNULL !"
#define MMIPHONE_WINTAB_5488_112_2_18_2_45_39_260 "MMISET_OpenSelectSimWin:win_id %d is already open!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PHONE_TRC)
TRACE_MSG(MMIPHONE_2039_112_2_18_2_45_3_0,"VirWin_HandleMsg THE msg_id = 0x%04x")
TRACE_MSG(MMIPHONE_2044_112_2_18_2_45_3_1,"HandlePsAndRefMsg:receive SD 0x%x")
TRACE_MSG(MMIPHONE_2090_112_2_18_2_45_3_2,"VirWin_HandleMsg: received MSG_ALM_ALERTING_END")
TRACE_MSG(MMIPHONE_2103_112_2_18_2_45_3_3,"VirWin_HandleMsg: received MSG_MP3_PLAY_CALL_BACK")
TRACE_MSG(MMIPHONE_2109_112_2_18_2_45_3_4,"VirWin_HandleMsg: received MSG_MP3_FATAL_ERROR")
TRACE_MSG(MMIPHONE_2114_112_2_18_2_45_3_5,"VirWin_HandleMsg: received [MMIMP3] MSG_MP3_START_LIRIC")
TRACE_MSG(MMIPHONE_2119_112_2_18_2_45_3_6,"VirWin_HandleMsg: received [MMIMP3] MSG_MP3_FOLDER_FULL")
TRACE_MSG(MMIPHONE_2124_112_2_18_2_45_3_7,"VirWin_HandleMsg: received MSG_KUR_PLAY_CALL_BACK")
TRACE_MSG(MMIPHONE_2129_112_2_18_2_45_3_8,"VirWin_HandleMsg: received MSG_KUR_FATAL_ERROR")
TRACE_MSG(MMIPHONE_2134_112_2_18_2_45_3_9,"VirWin_HandleMsg: received MSG_MPEG4_DELETE_ONE")
TRACE_MSG(MMIPHONE_2138_112_2_18_2_45_3_10,"VirWin_HandleMsg: MMIAPIMMS_ActiveRecvSendModule")
TRACE_MSG(MMIPHONE_2302_112_2_18_2_45_4_11,"virwin_MSG_TTS_AUDIO_PLAY")
TRACE_MSG(MMIPHONE_2306_112_2_18_2_45_4_12,"virwin_MSG_TTS_AUDIO_STOP")
TRACE_MSG(MMIPHONE_2311_112_2_18_2_45_4_13,"virwin_MSG_TTS_OVER")
TRACE_MSG(MMIPHONE_2318_112_2_18_2_45_4_14,"virwin_MSG_TTS_AUDIO_PLAY")
TRACE_MSG(MMIPHONE_2430_112_2_18_2_45_4_15,"HandlePsAndRefMsg:receive APP_MN_NETWORK_STATUS_IND")
TRACE_MSG(MMIPHONE_2438_112_2_18_2_45_4_16,"HandlePsAndRefMsg:receive APP_MN_NETWORK_INFO_IND")
TRACE_MSG(MMIPHONE_2446_112_2_18_2_45_4_17,"HandlePsAndRefMsg:receive APP_MN_PREFER_RAT_RSP")
TRACE_MSG(MMIPHONE_2451_112_2_18_2_45_4_18,"HandlePsAndRefMsg:receive APP_MN_PLMN_LIST_CNF")
TRACE_MSG(MMIPHONE_2458_112_2_18_2_45_4_19,"HandlePsAndRefMsg:receive APP_MN_PLMN_SELECT_CNF")
TRACE_MSG(MMIPHONE_2466_112_2_18_2_45_4_20,"HandlePsAndRefMsg:receive APP_MN_DISPLAY_PLMN_IND")
TRACE_MSG(MMIPHONE_2475_112_2_18_2_45_4_21,"HandlePsAndRefMsg:receive APP_MN_SIM_PRESENT_IND")
TRACE_MSG(MMIPHONE_2480_112_2_18_2_45_4_22,"HandlePsAndRefMsg:receive APP_MN_SIM_PLUG_IN_IND")
TRACE_MSG(MMIPHONE_2486_112_2_18_2_45_4_23,"HandlePsAndRefMsg:receive APP_MN_LND_READY_IND")
TRACE_MSG(MMIPHONE_2490_112_2_18_2_45_4_24,"HandlePsAndRefMsg:receive APP_MN_LND_UPDATE_CNF")
TRACE_MSG(MMIPHONE_2496_112_2_18_2_45_4_25,"HandlePsAndRefMsg:receive APP_MN_SET_BAND_CNF")
TRACE_MSG(MMIPHONE_2504_112_2_18_2_45_4_26,"HandlePsAndRefMsg:receive APP_MN_SIM_GET_PIN_IND")
TRACE_MSG(MMIPHONE_2514_112_2_18_2_45_4_27,"HandlePsAndRefMsg:dual_sys=%d")
TRACE_MSG(MMIPHONE_2533_112_2_18_2_45_4_28,"HandlePsAndRefMsg:receive APP_MN_SIM_PIN_FUNC_CNF")
TRACE_MSG(MMIPHONE_2539_112_2_18_2_45_4_29,"HandlePsAndRefMsg:receive APP_MN_SIM_READY_IND")
TRACE_MSG(MMIPHONE_2565_112_2_18_2_45_4_30,"HandlePsAndRefMsg:receive APP_MN_FDN_SERVICE_IND")
TRACE_MSG(MMIPHONE_2570_112_2_18_2_45_4_31,"HandlePsAndRefMsg:receive APP_MN_PS_READY_IND")
TRACE_MSG(MMIPHONE_2577_112_2_18_2_45_4_32,"HandlePsAndRefMsg:receive APP_MN_SIM_NOT_READY_IND")
TRACE_MSG(MMIPHONE_2589_112_2_18_2_45_5_33,"HandlePsAndRefMsg:receive APP_MN_SCELL_RSSI_IND")
TRACE_MSG(MMIPHONE_2594_112_2_18_2_45_5_34,"HandlePsAndRefMsg:receive APP_MN_PS_POWER_ON_CNF")
TRACE_MSG(MMIPHONE_2601_112_2_18_2_45_5_35,"HandlePsAndRefMsg:receive APP_MN_PS_POWER_OFF_CNF")
TRACE_MSG(MMIPHONE_2606_112_2_18_2_45_5_36,"HandlePsAndRefMsg:receive APP_MN_DEACTIVE_PS_CNF")
TRACE_MSG(MMIPHONE_2611_112_2_18_2_45_5_37,"HandlePsAndRefMsg:receive APP_MN_SIM_POWER_ON_CNF")
TRACE_MSG(MMIPHONE_2622_112_2_18_2_45_5_38,"HandlePsAndRefMsg:receive APP_MN_SIM_POWER_OFF_CNF")
TRACE_MSG(MMIPHONE_2629_112_2_18_2_45_5_39,"HandlePsAndRefMsg:receive APP_MN_PREFER_PLMN_CHANGE_CNF")
TRACE_MSG(MMIPHONE_2637_112_2_18_2_45_5_40,"HandlePuctInfoCnf: receive APP_MN_PUCT_INFO_CNF")
TRACE_MSG(MMIPHONE_2642_112_2_18_2_45_5_41,"None Handle: receive APP_MN_AOC_MODIFIED_IND")
TRACE_MSG(MMIPHONE_2671_112_2_18_2_45_5_42,"HandlePsAndRefMsg:receive MISC_MSG_RTC_ALARM")
TRACE_MSG(MMIPHONE_2682_112_2_18_2_45_5_43,"HandlePsAndRefMsg:receive UPM_PLUGIN_IND")
TRACE_MSG(MMIPHONE_2688_112_2_18_2_45_5_44,"HandlePsAndRefMsg:receive UPM_PLUGOUT_IND")
TRACE_MSG(MMIPHONE_2722_112_2_18_2_45_5_45,"HandlePsAndRefMsg:receive UPM_READY_IND")
TRACE_MSG(MMIPHONE_2729_112_2_18_2_45_5_46,"HandlePsAndRefMsg:receive UPM_ERROR_IND")
TRACE_MSG(MMIPHONE_2735_112_2_18_2_45_5_47,"HandlePsAndRefMsg:receive APP_AUDIO_END_IND")
TRACE_MSG(MMIPHONE_2741_112_2_18_2_45_5_48,"HandlePsAndRefMsg:receive APP_WRE_AUDIO_END_IND")
TRACE_MSG(MMIPHONE_2759_112_2_18_2_45_5_49,"HandlePsAndRefMsg(),receive APP_JAVA_MP4_DISPLAY_IND!")
TRACE_MSG(MMIPHONE_2764_112_2_18_2_45_5_50,"HandlePsAndRefMsg(),receive APP_JAVA_MP4_END_IND!")
TRACE_MSG(MMIPHONE_2768_112_2_18_2_45_5_51,"HandlePsAndRefMsg(),receive APP_JAVA_SNAPSHOT_END_IND!")
TRACE_MSG(MMIPHONE_2790_112_2_18_2_45_5_52,"apple phone param = %x")
TRACE_MSG(MMIPHONE_2794_112_2_18_2_45_5_53,"HandlePsAndRefMsg msg_id = %x")
TRACE_MSG(MMIPHONE_2798_112_2_18_2_45_5_54,"HandlePsAndRefMsg MSG_JAVA_CLOSE_APP_WIN param = %d, %x")
TRACE_MSG(MMIPHONE_2805_112_2_18_2_45_5_55,"HandlePsAndRefMsg MSG_JAVA_AUTORUNAPP param = %x")
TRACE_MSG(MMIPHONE_2809_112_2_18_2_45_5_56,"HandlePsAndRefMsg MSG_JAVA_SEND_TIMER param = %d %d")
TRACE_MSG(MMIPHONE_2813_112_2_18_2_45_5_57,"HandlePsAndRefMsg MSG_JAVA_LINK_TIMER param = %d %d")
TRACE_MSG(MMIPHONE_2817_112_2_18_2_45_5_58,"HandlePsAndRefMsg MSG_JAVA_CONNECT_TIMER param = %d %d")
TRACE_MSG(MMIPHONE_2821_112_2_18_2_45_5_59,"HandlePsAndRefMsg MSG_JAVA_RECEIVE_TIMER param = %d %d")
TRACE_MSG(MMIPHONE_2825_112_2_18_2_45_5_60,"HandlePsAndRefMsg MSG_JAVA_STOP_SOCKET_TIMER param = %d %d")
TRACE_MSG(MMIPHONE_2840_112_2_18_2_45_5_61,"HandlePsAndRefMsg MSG_JAVA_VM_SUSPENDED_IND param = %d %d")
TRACE_MSG(MMIPHONE_2844_112_2_18_2_45_5_62,"HandlePsAndRefMsg MSG_JAVA_MEDIA_STOPTIME_TIMER param = %d %d")
TRACE_MSG(MMIPHONE_2848_112_2_18_2_45_5_63,"HandlePsAndRefMsg MSG_JAVA_DEACTIVE_PDP param = %d %d")
TRACE_MSG(MMIPHONE_2852_112_2_18_2_45_5_64,"HandlePsAndRefMsg MSG_JAVA_PLATFORM_REQUEST param = %d %d")
TRACE_MSG(MMIPHONE_2856_112_2_18_2_45_5_65,"HandlePsAndRefMsg MSG_JAVA_AUDIO_PLAY_AGAIN param = %d %d")
TRACE_MSG(MMIPHONE_2931_112_2_18_2_45_5_66,"flash_printf:@@@@@MSG_FLASH_UPDATE_WINDOW  0  ,received the message.")
TRACE_MSG(MMIPHONE_2934_112_2_18_2_45_5_67,"flash_printf:@@@@@MSG_FLASH_UPDATE_WINDOW  1  ,received the message.")
TRACE_MSG(MMIPHONE_2941_112_2_18_2_45_5_68,"HandlePsAndRefMsg:receive SNTP_SIG_GET_TIME_CNF")
TRACE_MSG(MMIPHONE_2946_112_2_18_2_45_5_69,"HandlePsAndRefMsg:msg_id = 0x%04x")
TRACE_MSG(MMIPHONE_2992_112_2_18_2_45_5_70,"MMIAPIPHONE_InterrogateCfuForIdle dual_sys=%d,cfu_dual_sys=%d")
TRACE_MSG(MMIPHONE_3012_112_2_18_2_45_6_71,"MMIAPIPHONE_InterrogateCfuForIdle is_allow_do_cfu=%d")
TRACE_MSG(MMIPHONE_3145_112_2_18_2_45_6_72,"MMIAPIPHONE_SetIdleGprsState err dual_sys: %d")
TRACE_MSG(MMIPHONE_3149_112_2_18_2_45_6_73,"[dual mode] MMIAPIPHONE_SetIdleGprsState: rat = %d")
TRACE_MSG(MMIPHONE_3201_112_2_18_2_45_6_74,"MMIAPIPHONE_SetIdleGprsState dual_sys=%d")
TRACE_MSG(MMIPHONE_3205_112_2_18_2_45_6_75,"MMIAPIPHONE_SetIdleGprsState err dual_sys: %d")
TRACE_MSG(MMIPHONE_3261_112_2_18_2_45_6_76,"StoreNetworkStatus error dual_sys = %d")
TRACE_MSG(MMIPHONE_3307_112_2_18_2_45_6_77,"NetworkStatus g:%d, e:%d, h:%d")
TRACE_MSG(MMIPHONE_3407_112_2_18_2_45_6_78,"MMIAPIPHONE_GenPLMNDisplay plmn_ptr=%d, display_str=%d, display_str_len=%d")
TRACE_MSG(MMIPHONE_3412_112_2_18_2_45_6_79,"MMIAPIPHONE_GenPLMNDisplay mcc=%d, mnc=%d, mnc_digit_num=%d, str_len=%d, is_display_digit_format=%d,rat=%d")
TRACE_MSG(MMIPHONE_3474_112_2_18_2_45_6_80,"MMIAPIPHONE_GenPLMNDisplay is_digit_output=%d, network_name_id=%x")
TRACE_MSG(MMIPHONE_3508_112_2_18_2_45_7_81,"HandlePlmnListCnf error dual_sys = %d")
TRACE_MSG(MMIPHONE_3634_112_2_18_2_45_7_82,"HandlePowerOnSimCnf:dual_sys=%d,is_ok=%d")
TRACE_MSG(MMIPHONE_3638_112_2_18_2_45_7_83,"HandlePowerOnSimCnf error dual_sys = %d")
TRACE_MSG(MMIPHONE_3759_112_2_18_2_45_7_84,"HandlePowerOnSimCnf: Idle is not open")
TRACE_MSG(MMIPHONE_3840_112_2_18_2_45_7_85,"HandlePowerOnSimCnf,dual_sys=%d,is_prefer_set=%d")
TRACE_MSG(MMIPHONE_3874_112_2_18_2_45_7_86,"HandlePowerOffSimCnf:dual_sys=%d,is_ok=%d")
TRACE_MSG(MMIPHONE_3915_112_2_18_2_45_7_87,"HandlePowerOffSimCnf:dual_sys=%d,is_ok=%d")
TRACE_MSG(MMIPHONE_3939_112_2_18_2_45_7_88,"HandleSelectPlmnCnf error dual_sys = %d")
TRACE_MSG(MMIPHONE_3984_112_2_18_2_45_8_89,"HandleSelectBandCnf band_select_cnf.is_ok = %d")
TRACE_MSG(MMIPHONE_4016_112_2_18_2_45_8_90,"GetNetWorkNameId: the network plmn_ptr is error! %d")
TRACE_MSG(MMIPHONE_4057_112_2_18_2_45_8_91,"GetNetWorkNameId: the network status is error! %d")
TRACE_MSG(MMIPHONE_4084_112_2_18_2_45_8_92,"HandleSimPinInd:error dual_sys=%d")
TRACE_MSG(MMIPHONE_4267_112_2_18_2_45_8_93,"HandleSimPinCnf:pin_operate_result=%d, pin_num=%d, pin_function_type=%d")
TRACE_MSG(MMIPHONE_4715_112_2_18_2_45_9_94,"StoreSimReadyStatus:error dual_sys=%d")
TRACE_MSG(MMIPHONE_4786_112_2_18_2_45_9_95,"StoreSimNotReadyStatus:error dual_sys=%d")
TRACE_MSG(MMIPHONE_5110_112_2_18_2_45_10_96,"HandleScellRssiInd:error dual_sys=%d")
TRACE_MSG(MMIPHONE_5199_112_2_18_2_45_10_97,"MMIAPIPHONE_PowerOff:restart_condition=%d")
TRACE_MSG(MMIPHONE_5209_112_2_18_2_45_10_98,"MMIPHONE_PowerOff PHONE_SIM_LOCK_COUNTDOWN_WIN_ID open")
TRACE_MSG(MMIPHONE_5220_112_2_18_2_45_10_99,"MMIPHONE_PowerOff MMIAPIMTV_IsMTVRunning delay poweroff")
TRACE_MSG(MMIPHONE_5298_112_2_18_2_45_10_100,"MMIAPIPHONE_GetNetworkName err dual_sys: %d")
TRACE_MSG(MMIPHONE_5561_112_2_18_2_45_11_101,"MMIAPIPHONE_GetRoamingStatus:error dual_sys=%d")
TRACE_MSG(MMIPHONE_5583_112_2_18_2_45_11_102,"roaming status: %d, net isroaming: %d, net mcc: %d, sim mcc: %d ")
TRACE_MSG(MMIPHONE_5652_112_2_18_2_45_11_103,"GetOpnString dual_sys = %d OR opn_str_ptr is NULL")
TRACE_MSG(MMIPHONE_5736_112_2_18_2_45_11_104,"GetOpnString dual_sys = %d OR spn_str_ptr is NULL")
TRACE_MSG(MMIPHONE_5905_112_2_18_2_45_12_105,"MMIAPIPHONE_IsNetworkSelecting dual_sys = %d")
TRACE_MSG(MMIPHONE_5911_112_2_18_2_45_12_106,"MMIAPIPHONE_IsNetworkSelecting plmn_status=%d")
TRACE_MSG(MMIPHONE_5936_112_2_18_2_45_12_107,"GetSimAndServiceString: the network_str_ptr || network_str_ptr->wstr_ptr ||dual_sys")
TRACE_MSG(MMIPHONE_5942_112_2_18_2_45_12_108,"MMIAPIPHONE_GetSimAndServiceString s_sim_status[%d].simStatus %d,is_opn_need=%d,opn_len=%d,is_opn_exist=%d,is_spn_need=%d,spn_len=%d")
TRACE_MSG(MMIPHONE_6032_112_2_18_2_45_12_109,"MMIAPIPHONE_GetSimStatus err dual sys: %d")
TRACE_MSG(MMIPHONE_6045_112_2_18_2_45_12_110,"MMIPHONE_SetSimStatus:dual sys = %d, dual status = %d")
TRACE_MSG(MMIPHONE_6053_112_2_18_2_45_12_111,"MMIPHONE_SetSimStatus err dual sys: %d")
TRACE_MSG(MMIPHONE_6066_112_2_18_2_45_12_112,"MMIAPIPHONE_SetSimExistedStatus:dual sys = %d, is_existed = %d")
TRACE_MSG(MMIPHONE_6081_112_2_18_2_45_12_113,"MMIAPIPHONE_SetSimExistedStatus:err dual sys = %d")
TRACE_MSG(MMIPHONE_6120_112_2_18_2_45_12_114,"MMIAPIPHONE_IsSimOk err dual sys: %d")
TRACE_MSG(MMIPHONE_6136_112_2_18_2_45_12_115,"MMIAPIPHONE_IsSimCardOk err dual sys: %d")
TRACE_MSG(MMIPHONE_6153_112_2_18_2_45_12_116,"MMIAPIPHONE_IsSimAvailable err dual sys: %d")
TRACE_MSG(MMIPHONE_6302_112_2_18_2_45_13_117,"GetSysOkNum: the p=%d || idx_ptr = %d")
TRACE_MSG(MMIPHONE_6359_112_2_18_2_45_13_118,"MMIAPIPHONE_AlertSimNorOKStatus:error simStatus=%d")
TRACE_MSG(MMIPHONE_6365_112_2_18_2_45_13_119,"MMIAPIPHONE_AlertSimNorOKStatus:error dual_sys=%d")
TRACE_MSG(MMIPHONE_6474_112_2_18_2_45_13_120,"MMIAPIPHONE_AlertSimNorOKStatus err sim status: %d")
TRACE_MSG(MMIPHONE_6488_112_2_18_2_45_13_121,"MMIAPIPHONE_IsPin2RequiredACM err dual sys: %d")
TRACE_MSG(MMIPHONE_6502_112_2_18_2_45_13_122,"MMIAPIPHONE_IsPin2RequiredACMMax err dual sys: %d")
TRACE_MSG(MMIPHONE_6517_112_2_18_2_45_13_123,"MMIAPIPHONE_IsPin2RequiredPuct err dual sys: %d")
TRACE_MSG(MMIPHONE_6532_112_2_18_2_45_13_124,"MMIAPIPHONE_IsAocSupport err dual sys: %d")
TRACE_MSG(MMIPHONE_6542_112_2_18_2_45_13_125,"MMIAPIPHONE_IsAocSupport aoc_is_support=%d")
TRACE_MSG(MMIPHONE_6559_112_2_18_2_45_13_126,"MMIAPIPHONE_GetPlmnStatus err dual sys: %d")
TRACE_MSG(MMIPHONE_6576_112_2_18_2_45_13_127,"mmiphone:CreateInputPinWindow dual_sys=%d")
TRACE_MSG(MMIPHONE_6636_112_2_18_2_45_13_128,"GetSimAndServiceString: the is_sim1_ok_ptr=%d || is_sim2_ok_ptr =%d ")
TRACE_MSG(MMIPHONE_6656_112_2_18_2_45_13_129,"MMIAPIPHONE_GetDualSimStatus, is_sim1_ok=%d, is_sim2_ok=%d")
TRACE_MSG(MMIPHONE_6695_112_2_18_2_45_13_130,"MMIAPIENG_GetFtaFlag() = %d")
TRACE_MSG(MMIPHONE_6802_112_2_18_2_45_14_131,"MMIAPIPHONE_SetGPRSDetach e_dualsys_setting = %d")
TRACE_MSG(MMIPHONE_6823_112_2_18_2_45_14_132,"MMIAPIPHONE_SetGPRSDetachEx dual_sys = %d")
TRACE_MSG(MMIPHONE_6854_112_2_18_2_45_14_133,"MMIAPIPHONE_StartUpPsAccordingToSetting, start power on sim")
TRACE_MSG(MMIPHONE_6888_112_2_18_2_45_14_134,"MMIAPIPHONE_StartUpPsAccordingToSetting, e_dualsys_setting=%d")
TRACE_MSG(MMIPHONE_6968_112_2_18_2_45_14_135,"PowerOnPsForEmerge, s_ps_ready[%d]=%d")
TRACE_MSG(MMIPHONE_6977_112_2_18_2_45_14_136,"PowerOnPsForEmerge Fly mode!")
TRACE_MSG(MMIPHONE_7000_112_2_18_2_45_14_137,"MMIAPIPHONE_PowerOnPs, dual_sys=%d ps is already")
TRACE_MSG(MMIPHONE_7009_112_2_18_2_45_14_138,"MMIAPIPHONE_PowerOnPs Fly mode!")
TRACE_MSG(MMIPHONE_7030_112_2_18_2_45_14_139,"MMIAPIPHONE_PowerOffPs, dual_sys=%d,s_ps_ready=%d")
TRACE_MSG(MMIPHONE_7060_112_2_18_2_45_14_140,"MMIAPIPHONE_SetIsPsDeactiveCnf err dual sys: %d")
TRACE_MSG(MMIPHONE_7075_112_2_18_2_45_14_141,"MMIAPIPHONE_GetIsPsDeactiveCnf err dual sys: %d")
TRACE_MSG(MMIPHONE_7090_112_2_18_2_45_14_142,"MMIAPIPHONE_SetIsPsReady err dual sys: %d")
TRACE_MSG(MMIPHONE_7105_112_2_18_2_45_14_143,"MMIAPIPHONE_GetIsPsReady err dual sys: %d")
TRACE_MSG(MMIPHONE_7118_112_2_18_2_45_14_144,"MMIAPIPHONE_GetCurResponsePinSimID:sim_id=%d")
TRACE_MSG(MMIPHONE_7129_112_2_18_2_45_14_145,"MMIAPIPHONE_GetCurHandlingPinSimID:sim_id=%d")
TRACE_MSG(MMIPHONE_7152_112_2_18_2_45_14_146,"MMIAPIPHONE_InitTDOrGsm configue_rat=%d")
TRACE_MSG(MMIPHONE_7263_112_2_18_2_45_14_147,"HandleSimPresentInd:error dual_sys=%d")
TRACE_MSG(MMIPHONE_7267_112_2_18_2_45_14_148,"HandleSimPresentInd:dual_sys:%d, sim_sms_num:%d")
TRACE_MSG(MMIPHONE_7292_112_2_18_2_45_15_149,"HandleLdnReadyInd:dual_sys:%d,is_lnd_ok=%d")
TRACE_MSG(MMIPHONE_7313_112_2_18_2_45_15_150,"HandleSimPlugInInd:error dual_sys=%d")
TRACE_MSG(MMIPHONE_7316_112_2_18_2_45_15_151,"HandleSimPlugInInd:dual_sys:%d,")
TRACE_MSG(MMIPHONE_7381_112_2_18_2_45_15_152,"NotifySIMPlugInd dual_sys=%d, notify_event=%d")
TRACE_MSG(MMIPHONE_7390_112_2_18_2_45_15_153,"NotifySIMPlugInd Intial not start, donot handle!")
TRACE_MSG(MMIPHONE_7515_112_2_18_2_45_15_154,"MMIAPIPHONE_PowerOnSIM, start power on sim dual_sys=%d")
TRACE_MSG(MMIPHONE_7533_112_2_18_2_45_15_155,"MMIAPIPHONE_DeactivePS, dual_sys=%d,s_ps_ready=%d")
TRACE_MSG(MMIPHONE_7595_112_2_18_2_45_15_156,"HandleSetStandbyMdoeWaitingWindow msg_id = %d")
TRACE_MSG(MMIPHONE_7662_112_2_18_2_45_15_157,"ImplementStandByModeAsync s_plug_sim_id=%d, s_plug_event=%d")
TRACE_MSG(MMIPHONE_7698_112_2_18_2_45_15_158,"MMIAPIPHONE_SetStandByMode dual_sys=%d, plug_event=%d, e_sys_setting=%d")
TRACE_MSG(MMIPHONE_7851_112_2_18_2_45_16_159,"MMIAPIPHONE_GetDataServiceType: %d")
TRACE_MSG(MMIPHONE_7865_112_2_18_2_45_16_160,"NotifyRxLevelChange error dual_sys: %d")
TRACE_MSG(MMIPHONE_7870_112_2_18_2_45_16_161,"NotifyRxLevelChange rx_level=%d, s_notify_rx_level=%d, PlmnStatus=%d")
TRACE_MSG(MMIPHONE_7917_112_2_18_2_45_16_162,"NotifyPlmnStatusChange plmn_status=%d, s_notify_plmn_status=%d, rssiLevel=%d")
TRACE_MSG(MMIPHONE_8028_112_2_18_2_45_16_163,"NotifyServiceChange dual_sys=%d, notify_event=%d")
TRACE_MSG(MMIPHONE_8157_112_2_18_2_45_17_164,"MMIAPIPHONE_IsPSAvail error dual_sys: %d")
TRACE_MSG(MMIPHONE_8195_112_2_18_2_45_17_165,"MMIAPIPHONE_PowerOnPsEx s_is_power_on_sim_cnf[%d]=%d")
TRACE_MSG(MMIPHONE_8260_112_2_18_2_45_17_166,"MMIAPIPHONE_IsRegNetSuccess err dual sys: %d")
TRACE_MSG(MMIPHONE_8276_112_2_18_2_45_17_167,"MMIAPIPHONE_IsAttachGprsSuccess err dual sys: %d")
TRACE_MSG(MMIPHONE_8303_112_2_18_2_45_17_168,"MMIAPIPHONE_OpeartorTextID param error")
TRACE_MSG(MMIPHONE_8423_112_2_18_2_45_17_169,"mmiphone:CreateInputSimLockWindow opreate_sim_lock_ptr = NULL")
TRACE_MSG(MMIPHONE_8427_112_2_18_2_45_17_170,"mmiphone:CreateInputSimLockWindow dual_sys=%d")
TRACE_MSG(MMIPHONE_8534_112_2_18_2_45_18_171,"MMIAPIPHONE_AppendPlmnList,index=%d")
TRACE_MSG(MMIPHONE_8589_112_2_18_2_45_18_172,"MMIAPIPHONE_AppendPreferPlmnList,index=%d")
TRACE_MSG(MMIPHONE_8643_112_2_18_2_45_18_173,"GetSimAndServiceString: the prefer_network =%d ")
TRACE_MSG(MMIPHONE_8647_112_2_18_2_45_18_174,"MMIAPIPHONE_GetPlmnInfoByIndex,index=%d")
TRACE_MSG(MMIPHONE_8683_112_2_18_2_45_18_175,"ccapp.c HandleLDNUpdateCnf,type= %d, result =%d, dual_sys= %d")
TRACE_MSG(MMIPHONE_8760_112_2_18_2_45_18_176,"[mmiphone]: MMIAPIPHONE_GetAllSimStateOKCount, active_sim_count=%d")
TRACE_MSG(MMIPHONE_CHARGE_307_112_2_18_2_45_19_177,"MMIAPIPHONE_ChargeInit,is_charge =%d,dbat_info->module_state=%d")
TRACE_MSG(MMIPHONE_CHARGE_346_112_2_18_2_45_19_178,"HandlePsAndRefMsg:receive DBAT_MSG_UPDATE_STATE_IND")
TRACE_MSG(MMIPHONE_CHARGE_351_112_2_18_2_45_19_179,"HandlePsAndRefMsg:receive DBAT_MSG_BAT_PLUGIN_IND")
TRACE_MSG(MMIPHONE_CHARGE_355_112_2_18_2_45_19_180,"HandlePsAndRefMsg:receive DBAT_MSG_BAT_PLUGOUT_IND")
TRACE_MSG(MMIPHONE_CHARGE_360_112_2_18_2_45_19_181,"HandlePsAndRefMsg:receive DBAT_MSG_SWITCH_BAT_IND")
TRACE_MSG(MMIPHONE_CHARGE_363_112_2_18_2_45_19_182,"HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_START_IND")
TRACE_MSG(MMIPHONE_CHARGE_368_112_2_18_2_45_19_183,"HandlePsAndRefMsg:receive DBAT_MSG_WARNING_IND")
TRACE_MSG(MMIPHONE_CHARGE_372_112_2_18_2_45_19_184,"HandlePsAndRefMsg:receive DBAT_MSG_SHUTDOWN_IND")
TRACE_MSG(MMIPHONE_CHARGE_396_112_2_18_2_45_19_185,"HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_FINISH")
TRACE_MSG(MMIPHONE_CHARGE_400_112_2_18_2_45_19_186,"HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_DISCONNECT")
TRACE_MSG(MMIPHONE_CHARGE_405_112_2_18_2_45_19_187,"HandlePsAndRefMsg:receive DBAT_MSG_CHARGE_FAULT")
TRACE_MSG(MMIPHONE_CHARGE_410_112_2_18_2_45_19_188,"HandlePsAndRefMsg:receive CHR_MSG_CAP_IND")
TRACE_MSG(MMIPHONE_CHARGE_416_112_2_18_2_45_19_189,"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_START_IND")
TRACE_MSG(MMIPHONE_CHARGE_431_112_2_18_2_45_19_190,"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_END_IND")
TRACE_MSG(MMIPHONE_CHARGE_446_112_2_18_2_45_19_191,"HandlePsAndRefMsg:receive CHR_MSG_WARNING_IND")
TRACE_MSG(MMIPHONE_CHARGE_452_112_2_18_2_45_19_192,"HandlePsAndRefMsg:receive CHR_MSG_SHUTDOWN_IND")
TRACE_MSG(MMIPHONE_CHARGE_480_112_2_18_2_45_19_193,"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_FINISH_IND")
TRACE_MSG(MMIPHONE_CHARGE_486_112_2_18_2_45_19_194,"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_DISCONNECT_IND")
TRACE_MSG(MMIPHONE_CHARGE_501_112_2_18_2_45_19_195,"HandlePsAndRefMsg:receive CHR_MSG_CHARGE_FAULT_IND")
TRACE_MSG(MMIPHONE_CHARGE_765_112_2_18_2_45_20_196,"HandleChrWarningInd:s_startup_condition = STARTUP_CHARGE")
TRACE_MSG(MMIPHONE_CHARGE_1130_112_2_18_2_45_21_197,"MMIAPIPHONE_GetBatLevel:bat_level=%d,level=%d")
TRACE_MSG(MMIPHONE_CHARGE_1151_112_2_18_2_45_21_198,"MMIAPIPHONE_GetBatCapacity bat_level = %d")
TRACE_MSG(MMIPHONE_CHARGE_1176_112_2_18_2_45_21_199,"MMIPHONE_GetChargeStatus,result=%d")
TRACE_MSG(MMIPHONE_CHARGE_1193_112_2_18_2_45_21_200,"[MMIPHONE]: HandleLowPowerRingWin, msg=%x")
TRACE_MSG(MMIPHONE_CHARGE_1883_112_2_18_2_45_22_201,"mmi DisplayBtryCapacityStr exceed str len max")
TRACE_MSG(MMIPHONE_CHARGE_1924_112_2_18_2_45_22_202,"mmi DisplayBtryCapacityStr exceed str len max")
TRACE_MSG(MMIPHONE_CHARGE_1954_112_2_18_2_45_22_203,"mmi DisplayBtryCapacityStr exceed str len max")
TRACE_MSG(MMIPHONE_CHARGE_2007_112_2_18_2_45_23_204,"BtryCapacityCtrlCallbackFun,ERROR!")
TRACE_MSG(MMIPHONE_CHARGE_2012_112_2_18_2_45_23_205,"BtryCapacityCtrlCallbackFun,bat_level=%d")
TRACE_MSG(MMIPHONE_NITZ_93_112_2_18_2_45_23_206,"[MMIPHONE] DrmAutoUpdateNetworkTime: The phone has update network time after leaving factory.")
TRACE_MSG(MMIPHONE_NITZ_128_112_2_18_2_45_23_207,"[MMIPHONE]: SetNetworkInfoTimeToSystem , ut_lt_zone_flag =%d daylight_saving_time_flag =%d ,ut_lt_zone.time_zone =%d, local_time_zone= %d")
TRACE_MSG(MMIPHONE_NITZ_155_112_2_18_2_45_23_208,"[MMIPHONE]: SetNetworkInfoTimeToSystem , net_second =%d")
TRACE_MSG(MMIPHONE_NITZ_169_112_2_18_2_45_24_209,"[MMIPHONE]: SetNetworkInfoTimeToSystem , temp_second2 =%d")
TRACE_MSG(MMIPHONE_NITZ_172_112_2_18_2_45_24_210,"[MMIPHONE]: SetNetworkInfoTimeToSystem , net_second2 =%d")
TRACE_MSG(MMIPHONE_NITZ_177_112_2_18_2_45_24_211,"[MMIPHONE]: SetNetworkInfoTimeToSystem , DT:%04d-%02d-%02d %02d-%02d-%02d")
TRACE_MSG(MMIPHONE_NITZ_195_112_2_18_2_45_24_212,"[MMIPHONE]: SetNetworkInfoTimeToSystem , date time is the same")
TRACE_MSG(MMIPHONE_NITZ_206_112_2_18_2_45_24_213,"[MMIPHONE]: SetNetworkInfoTimeToSystem , set time:%04d-%02d-%02d %02d-%02d-%02d")
TRACE_MSG(MMIPHONE_NITZ_221_112_2_18_2_45_24_214,"[MMIPHONE]: SetNetworkInfoTimeToSystem, SetTime:time_hour is %d,time_minute is %d is invalid!")
TRACE_MSG(MMIPHONE_NITZ_229_112_2_18_2_45_24_215,"[MMIPHONE]: SetNetworkInfoTimeToSystem , TM_SetSysTime status [%d] ")
TRACE_MSG(MMIPHONE_NITZ_251_112_2_18_2_45_24_216,"[MMIPHONE] :SetNetworkInfoTimeToSystem, SetDate:year is %d,month is %d and day is %d is invalid!")
TRACE_MSG(MMIPHONE_NITZ_257_112_2_18_2_45_24_217,"[MMIPHONE]: SetNetworkInfoTimeToSystem , ut_lt_zone_flagis_network_time_exist %d")
TRACE_MSG(MMIPHONE_NITZ_277_112_2_18_2_45_24_218,"[MMIPHONE]: ParseSysTimeFromNetworkInfo success , net_time_zone [%d] daylight_saving_time[%d]")
TRACE_MSG(MMIPHONE_NITZ_319_112_2_18_2_45_24_219,"[MMIPHONE]: ParseSysTimeFromNetworkInfo , local_time_zone_flag [%d] ut_lt_zone_flag[%d],daylight_saving_time_flag [%d]")
TRACE_MSG(MMIPHONE_NITZ_326_112_2_18_2_45_24_220,"[MMIPHONE]: ParseSysTimeFromNetworkInfo , net_time_zone=%d, MMIAPIACC_GetLocalTimeZone() =%f")
TRACE_MSG(MMIPHONE_NITZ_343_112_2_18_2_45_24_221,"[MMIPHONE]: AutoUpdateTimeHandleNetworkInfoInd SIM [%d] ut_lt_zone_flag[%d] ")
TRACE_MSG(MMIPHONE_NITZ_357_112_2_18_2_45_24_222,"[MMIPHONE]:  net no support time and timezone")
TRACE_MSG(MMIPHONE_ONOFF_381_112_2_18_2_45_25_223,"[POWERONOFF]:MMIAPIPHONE_IsNetworkSelecting e_start_cond = %d")
TRACE_MSG(MMIPHONE_ONOFF_421_112_2_18_2_45_25_224,"[POWERONOFF]:HandleNormalStartupWindow type=%d")
TRACE_MSG(MMIPHONE_ONOFF_970_112_2_18_2_45_26_225,"[POWERONOFF]:PlayPowerOnOffMP4:is_power_on=%d")
TRACE_MSG(MMIPHONE_ONOFF_1086_112_2_18_2_45_27_226,"[POWERONOFF]:PlayPowerOnOffMP4:anim_type=%d")
TRACE_MSG(MMIPHONE_ONOFF_1101_112_2_18_2_45_27_227,"mmiphone_onoff.c, PlayPowerOnMP4CallBack result = %d")
TRACE_MSG(MMIPHONE_ONOFF_1185_112_2_18_2_45_27_228,"[POWERONOFF]:HandleVideoPowerOnOffMsg msg_ptr->msg_id = %x")
TRACE_MSG(MMIPHONE_ONOFF_1285_112_2_18_2_45_27_229,"[POWERONOFF]:PlayDefaultPowerOnOffAnimation active_mode_value=%d is_power_on_off=%d")
TRACE_MSG(MMIPHONE_WINTAB_1209_112_2_18_2_45_30_230,"HandlePinInputingString pin_ptr is NULL")
TRACE_MSG(MMIPHONE_WINTAB_1899_112_2_18_2_45_32_231,"HandlePinInputingString pin_ptr=%d ||puk_ptr=%d")
TRACE_MSG(MMIPHONE_WINTAB_2208_112_2_18_2_45_32_232,"HandlePinInputingString pin_ptr ")
TRACE_MSG(MMIPHONE_WINTAB_2222_112_2_18_2_45_32_233,"HandleOperatePin:operate_type is %d,pin_num is %d ")
TRACE_MSG(MMIPHONE_WINTAB_2865_112_2_18_2_45_34_234,"HandleOperatePukAndNewPin pin_ptr=%d, puk_ptr= %d , operate_pin_info=%d")
TRACE_MSG(MMIPHONE_WINTAB_2914_112_2_18_2_45_34_235,"HandleOperatePinAndNewPin pin_ptr=%d, puk_ptr= %d , operate_pin_info=%d")
TRACE_MSG(MMIPHONE_WINTAB_2967_112_2_18_2_45_34_236,"HandleOperatePukAndNewPin  pin_puk_str_ptr is NULL")
TRACE_MSG(MMIPHONE_WINTAB_2973_112_2_18_2_45_34_237,"MMI_PHONE: pin_puk_remain=%d")
TRACE_MSG(MMIPHONE_WINTAB_3035_112_2_18_2_45_34_238,"MMIPHONE_OpenQueryPoweroff=%d")
TRACE_MSG(MMIPHONE_WINTAB_3115_112_2_18_2_45_34_239,"AppendIMEIDetail Read MN_DUAL_SYS_%d MNNV_GetIMEISVNEx Error!")
TRACE_MSG(MMIPHONE_WINTAB_3153_112_2_18_2_45_35_240,"AppendIMEIDetail SVN Not Exist!")
TRACE_MSG(MMIPHONE_WINTAB_3158_112_2_18_2_45_35_241,"AppendIMEIDetail Read MN_DUAL_SYS_1MNNV_GetIMEISVNEx Error!")
TRACE_MSG(MMIPHONE_WINTAB_3378_112_2_18_2_45_35_242,"MMIAPIPHONE_SetScrDelCallBack OVERFLOW")
TRACE_MSG(MMIPHONE_WINTAB_3699_112_2_18_2_45_36_243,"MMIPHONE:GetSIMLockRemainStr  max_num_trials=%d,time_interval= %d,time_multiplier= %d,cck_trials=%d,nck_trials=%d,nsck_trials=%d,pck_trials=%d,spck_trials=%d")
TRACE_MSG(MMIPHONE_WINTAB_3735_112_2_18_2_45_36_244,"[MMIPHONE]: GetSIMLockInputPasswordTitle unlock_title = NULL")
TRACE_MSG(MMIPHONE_WINTAB_3791_112_2_18_2_45_36_245,"MMIPHONE:MMIAPIPHONE_GetSIMLockRemainTrialsTimer  max_num_trials=%d,time_interval= %d,time_multiplier= %d,cck_trials=%d,nck_trials=%d,nsck_trials=%d,pck_trials=%d,spck_trials=%d")
TRACE_MSG(MMIPHONE_WINTAB_3898_112_2_18_2_45_36_246,"SIMUnlockInputWinSoftkey operate_sim_lock_ptr is NULL")
TRACE_MSG(MMIPHONE_WINTAB_3904_112_2_18_2_45_36_247,"SIMUnlockInputWinSoftkey str_ptr || text_id_ptr is NULL")
TRACE_MSG(MMIPHONE_WINTAB_3978_112_2_18_2_45_36_248,"HandleSIMUnlockingAlertErrorWindow win_info_ptr =NULL ||win_info_ptr->user_data = NULL")
TRACE_MSG(MMIPHONE_WINTAB_4082_112_2_18_2_45_36_249,"HandleSIMUnlockingInputingString sim_unlocking_ptr||sim_unlocking_ptr->wstr_ptr|operate_sim_lock_ptr||sim_unlock_datais null")
TRACE_MSG(MMIPHONE_WINTAB_4204_112_2_18_2_45_37_250,"[MMIPHONE] sim_lock_ptr alloc memory fail")
TRACE_MSG(MMIPHONE_WINTAB_4228_112_2_18_2_45_37_251,"HandleSIMUnlockingInputingString operate_sim_lock is null")
TRACE_MSG(MMIPHONE_WINTAB_4367_112_2_18_2_45_37_252,"[MMIPHONE] get MMI_DUAL_SYS_MAX <= dual_sys fail")
TRACE_MSG(MMIPHONE_WINTAB_4381_112_2_18_2_45_37_253,"[MMIPHONE] set dual_sys = %d is_need_restart=%d")
TRACE_MSG(MMIPHONE_WINTAB_4586_112_2_18_2_45_37_254,"[MMIPHONE] SetCountdownStartTime result=%d,hour%2d,min%2d,sec%2d")
TRACE_MSG(MMIPHONE_WINTAB_4709_112_2_18_2_45_38_255,"CountdownHandleTimer timer_id = %d")
TRACE_MSG(MMIPHONE_WINTAB_4801_112_2_18_2_45_38_256,"[MMIPHONE] MMIAPIPHONE_HandleSIMLockSettingMenuFlow opreate_sim_lock_ptr alloc fail!")
TRACE_MSG(MMIPHONE_WINTAB_5307_112_2_18_2_45_39_257,"SelectSIM COM WIN,dual_sys[%d]>=MMI_DUAL_SYS_MAX/MMI_DUAL_SYS_MAX+1 in wifi ")
TRACE_MSG(MMIPHONE_WINTAB_5366_112_2_18_2_45_39_258,"MMIAPIPHONE_OpenSelectSimWin:select sim_input_ptr couldn't be PNULL !")
TRACE_MSG(MMIPHONE_WINTAB_5375_112_2_18_2_45_39_259,"MMIAPIPHONE_OpenSelectSimWin:win_id_ptr or  handle_func couldn't be PNULL !")
TRACE_MSG(MMIPHONE_WINTAB_5488_112_2_18_2_45_39_260,"MMISET_OpenSelectSimWin:win_id %d is already open!")
END_TRACE_MAP(MMI_APP_PHONE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PHONE_TRC_H_

