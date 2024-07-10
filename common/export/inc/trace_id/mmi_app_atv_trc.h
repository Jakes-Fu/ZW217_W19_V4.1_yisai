/******************************************************************************
 ** File Name:      mmi_app_atv_trc.h                                         *
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
//trace_id:150
#ifndef _MMI_APP_ATV_TRC_H_
#define _MMI_APP_ATV_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIATV_DATA_91_112_2_18_2_3_6_0 "[MMIATV]: MMIATV_InitData"
#define MMIATV_DATA_94_112_2_18_2_3_6_1 "[MMIATV]: MMIATV_InitData s_data_ptr != PNULL"
#define MMIATV_DATA_101_112_2_18_2_3_6_2 "[MMIATV]:s_data_ptr alloc mem error!!!!!!"
#define MMIATV_DATA_304_112_2_18_2_3_7_3 "[MMIATV]: MMIATV_RestoreFactorySetting"
#define MMIATV_DATA_510_112_2_18_2_3_7_4 "[MMIATV]: MMIATV_ReadServiceListFile service_head_ptr not null! "
#define MMIATV_DATA_530_112_2_18_2_3_7_5 "[MMIATV]: MMIATV_ReadServiceListFile create file error! "
#define MMIATV_DATA_537_112_2_18_2_3_7_6 "[MMIATV]: MMIAPIFMM_ReadFile error! "
#define MMIATV_DATA_552_112_2_18_2_3_7_7 "[MMIATV]: ServiceListFile version error! "
#define MMIATV_DATA_561_112_2_18_2_3_7_8 "[MMIATV]: MMIAPIFMM_ReadFile error! "
#define MMIATV_DATA_565_112_2_18_2_3_7_9 "[MMIATV]: MMIATV_ReadServiceListFile total list num = %d"
#define MMIATV_DATA_576_112_2_18_2_3_7_10 "[MMIATV]: MMIAPIFMM_ReadFile error! "
#define MMIATV_DATA_615_112_2_18_2_3_8_11 "[MMIATV]: MMIATV_WriteServiceListFile create file error! "
#define MMIATV_DATA_655_112_2_18_2_3_8_12 "[MMIATV]: MMIATV_AddServiceNode node_ptr is null!"
#define MMIATV_DATA_666_112_2_18_2_3_8_13 "[MMIATV]: MMIATV_AddServiceNode alloc memory error!"
#define MMIATV_DATA_722_112_2_18_2_3_8_14 "[MMIATV]: MMIATV_GetServiceNodeViaIndex, index = %d"
#define MMIATV_DATA_726_112_2_18_2_3_8_15 "[MMIATV]: MMIATV_GetServiceNodeViaIndex PNULL==service_head_ptr"
#define MMIATV_DATA_738_112_2_18_2_3_8_16 "[MMIATV]: MMIATV_GetServiceNodeViaIndex fail!"
#define MMIATV_DATA_761_112_2_18_2_3_8_17 "[MMIATV]: MMIATV_ReadFavoriteServiceListFile service_head_ptr not null! "
#define MMIATV_DATA_782_112_2_18_2_3_8_18 "[MMIATV]: MMIATV_ReadFavoriteServiceListFile create file error! "
#define MMIATV_DATA_789_112_2_18_2_3_8_19 "[MMIATV]: FavoriteServiceListFile error! "
#define MMIATV_DATA_804_112_2_18_2_3_8_20 "[MMIATV]: FavoriteServiceListFile version error! "
#define MMIATV_DATA_813_112_2_18_2_3_8_21 "[MMIATV]: MMIAPIFMM_ReadFile error! "
#define MMIATV_DATA_817_112_2_18_2_3_8_22 "[MMIATV]: MMIATV_ReadFavoriteServiceListFile total list num = %d"
#define MMIATV_DATA_828_112_2_18_2_3_8_23 "[MMIATV]: MMIAPIFMM_ReadFile error! "
#define MMIATV_DATA_867_112_2_18_2_3_8_24 "[MMIATV]: MMIATV_WriteFavoriteServiceListFile create file error! "
#define MMIATV_DATA_907_112_2_18_2_3_8_25 "[MMIATV]: MMIATV_AddFavoriteServiceNode node_ptr is null!"
#define MMIATV_DATA_918_112_2_18_2_3_8_26 "[MMIATV]: MMIATV_AddFavoriteServiceNode alloc memory error!"
#define MMIATV_DATA_948_112_2_18_2_3_8_27 "[MMIATV]: MMIATV_DeleteFavoriteServiceNode node_ptr is null!"
#define MMIATV_DATA_954_112_2_18_2_3_8_28 "[MMIATV]: MMIATV_DeleteFavoriteServiceNode list is null!"
#define MMIATV_DATA_978_112_2_18_2_3_8_29 "[MMIATV]: MMIATV_DeleteFavoriteServiceNode find node error!"
#define MMIATV_DATA_1033_112_2_18_2_3_8_30 "[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex, index = %d"
#define MMIATV_DATA_1037_112_2_18_2_3_8_31 "[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex PNULL==service_head_ptr"
#define MMIATV_DATA_1049_112_2_18_2_3_8_32 "[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex fail!"
#define MMIATV_EXPORT_83_112_2_18_2_3_9_33 "[MMIATV]: MMIAPIATV_OpenATV"
#define MMIATV_EXPORT_86_112_2_18_2_3_9_34 "[MMIATV]: MMIAPIATV_OpenATV app is opened!"
#define MMIATV_EXPORT_103_112_2_18_2_3_9_35 "MMIAPIATV_OpenATV java running =%d"
#define MMIATV_EXPORT_112_112_2_18_2_3_9_36 "MMIAPIATV_OpenATV call running =%d"
#define MMIATV_MAIN_171_112_2_18_2_3_10_37 "[MMIATV]: MMIATV_Open  DRECORDER_SelectSrcDevice ret = %d"
#define MMIATV_MAIN_173_112_2_18_2_3_10_38 "[MMIATV]: MMIATV_Open  DRECORDER_Open ret = %d"
#define MMIATV_MAIN_177_112_2_18_2_3_10_39 "[MMIATV]: MMIATV_Open already opened!"
#define MMIATV_MAIN_210_112_2_18_2_3_10_40 "[MMIATV]: MMIATV_Close  DRECORDER_Close ret = %d"
#define MMIATV_MAIN_231_112_2_18_2_3_10_41 "[MMIATV]: MMIATV_StartSearchService play_status=%d"
#define MMIATV_MAIN_263_112_2_18_2_3_10_42 "[MMIATV]: MMIATV_StartSearchService total_num=%d"
#define MMIATV_MAIN_275_112_2_18_2_3_10_43 "[MMIATV]: DRECORDER_ATV_StartScanChn search_ret=%d"
#define MMIATV_MAIN_294_112_2_18_2_3_10_44 "[MMIATV]: MMIATV_StartSearchService  play_status error!"
#define MMIATV_MAIN_306_112_2_18_2_3_10_45 "[MMIATV]: MMIATV_StopSearchService play_status=%d"
#define MMIATV_MAIN_312_112_2_18_2_3_10_46 "[MMIATV]: StopSearch search_ret = %d"
#define MMIATV_MAIN_354_112_2_18_2_3_10_47 "[MMIATV]: MMIATV_PlayService play_status=%d service_id=%d"
#define MMIATV_MAIN_361_112_2_18_2_3_10_48 "[MMIATV]: MMIATV_PlayService play win is not focus!"
#define MMIATV_MAIN_366_112_2_18_2_3_10_49 "[MMIATV]: MMIATV_PlayService DRECORDER_ParamCtrl is not finished yet!"
#define MMIATV_MAIN_375_112_2_18_2_3_10_50 "[MMIATV]: MMIATV_PlayService DRECORDER_ParamCtrl MMIATV_STATUS_PLAYING ret=%d"
#define MMIATV_MAIN_412_112_2_18_2_3_10_51 "[MMIATV]: MMIATV_PlayService  error!!"
#define MMIATV_MAIN_421_112_2_18_2_3_10_52 "[MMIATV]: MMIATV_PlayService play_status error!"
#define MMIATV_MAIN_433_112_2_18_2_3_10_53 "[MMIATV]: StopPlayService play_status=%d"
#define MMIATV_MAIN_461_112_2_18_2_3_10_54 "[MMIATV]: MMIATV_HandleSysMsg error!"
#define MMIATV_MAIN_608_112_2_18_2_3_11_55 "[MMIATV]: GetPreviewParam screen_mode=%d, preview_type=%d"
#define MMIATV_MAIN_616_112_2_18_2_3_11_56 "[MMIATV]: GetPreviewParam cur_angle = %d"
#define MMIATV_MAIN_658_112_2_18_2_3_11_57 "[MMIATV]: GetPreviewParam screen_mode error"
#define MMIATV_MAIN_666_112_2_18_2_3_11_58 "[MMIATV]: GetPreviewParam , logic_coor.rect {%d %d %d %d} rotation %d"
#define MMIATV_MAIN_697_112_2_18_2_3_11_59 "[MMIATV]: GetPreviewParam , disp_rect {%d %d %d %d},target rect {%d %d %d %d},rotation %d,disp_mode %d"
#define MMIATV_MAIN_717_112_2_18_2_3_11_60 "[MMIATV]: HandleReportMsg  param_ptr error!!!!!!!!!!!!!!!!!! "
#define MMIATV_MAIN_723_112_2_18_2_3_11_61 "[MMIATV]: HandleReportMsg  ATV is not open! "
#define MMIATV_MAIN_731_112_2_18_2_3_11_62 "[MMIATV]: HandleReportMsg atv_chn_id=%d atv_chn_is_valid=%d"
#define MMIATV_MAIN_746_112_2_18_2_3_11_63 "[MMIATV]: HandleReportMsg report_type = %d"
#define MMIATV_MAIN_754_112_2_18_2_3_11_64 "[MMIATV]: HandleReportMsg RSSI changed, atv_rssi=%d"
#define MMIATV_MAIN_788_112_2_18_2_3_11_65 "[MMIATV]: HandleReportMsg report_type=%d error!!!!!!!!!!!!!"
#define MMIATV_MAIN_804_112_2_18_2_3_11_66 "[MMIATV]: AddServiceNode cur_num=%d service_id=%d"
#define MMIATV_MAIN_826_112_2_18_2_3_11_67 "[MMIATV]: CHN_ParamCtrlCallBack"
#define MMIATV_MAIN_839_112_2_18_2_3_11_68 "[MMIATV]: ParamCtrlCallBack"
#define MMIATV_MAIN_853_112_2_18_2_3_11_69 "[MMIATV]: ReportCallback  report_ptr is null !"
#define MMIATV_MAIN_868_112_2_18_2_3_11_70 "[MMIATV]: ReportCallback  report_type=%d"
#define MMIATV_MAIN_956_112_2_18_2_3_11_71 "[MMIATV]: RecordEndCallBack record end i_type = %d"
#define MMIATV_MAIN_999_112_2_18_2_3_11_72 "[MMIATV]:MMIATV_StartRecord video type=%d error!!!!!!!!!!!"
#define MMIATV_MAIN_1023_112_2_18_2_3_11_73 "[MMIATV]: MMIATV_StartRecord success!"
#define MMIATV_MAIN_1027_112_2_18_2_3_11_74 "[MMIATV]: MMIATV_StartRecord fail ret=%d!"
#define MMIATV_MAIN_1087_112_2_18_2_3_12_75 "[MMIATV]: MMIATV_StopRecord ret= %d"
#define MMIATV_MAIN_1102_112_2_18_2_3_12_76 "[MMIATV] MMIATV_SwitchPlayDevice is_a2dp_connected = %d, is_bt_connected = %d"
#define MMIATV_MAIN_1125_112_2_18_2_3_12_77 "[MMIATV]: BTEventNext enter"
#define MMIATV_MAIN_1135_112_2_18_2_3_12_78 "[MMIATV]: BTEventPre enter"
#define MMIATV_MAIN_1149_112_2_18_2_3_12_79 "[MMIATV]:AtvPlayCallBack event=%d"
#define MMIATV_MAIN_1160_112_2_18_2_3_12_80 "[MMIATV]:AtvPlayCallBack report=%d,data=%d"
#define MMIATV_WINTAB_948_112_2_18_2_3_15_81 "[MMIATV]: HandleATVRecordEnd i_type= %d"
#define MMIATV_WINTAB_1196_112_2_18_2_3_16_82 "[MMIATV]: HandleMainWinMsg  MSG_APP_CANCEL"
#define MMIATV_WINTAB_1320_112_2_18_2_3_16_83 "[MMIATV]:HandleTpMsg  point.x = %d, point.y = %d"
#define MMIATV_WINTAB_1624_112_2_18_2_3_16_84 "[MMIATV]:VolButtonCallback"
#define MMIATV_WINTAB_1643_112_2_18_2_3_16_85 "[MMIATV]:PreButtonCallback"
#define MMIATV_WINTAB_1662_112_2_18_2_3_16_86 "[MMIATV]:ScreenButtonCallback"
#define MMIATV_WINTAB_1681_112_2_18_2_3_17_87 "[MMIATV]:NextButtonCallback"
#define MMIATV_WINTAB_1700_112_2_18_2_3_17_88 "[MMIATV]:RecordButtonCallback"
#define MMIATV_WINTAB_1729_112_2_18_2_3_17_89 "[MMIATV]:OptionButtonCallback"
#define MMIATV_WINTAB_1748_112_2_18_2_3_17_90 "[MMIATV]:ReturnButtonCallback"
#define MMIATV_WINTAB_2065_112_2_18_2_3_17_91 "[MMIATV]: HandleMainOptWinMsg:menu_id 0x%x is error!"
#define MMIATV_WINTAB_2068_112_2_18_2_3_17_92 "[MMIATV]: HandleMainOptWinMsg:menu_id 0x%x is error!"
#define MMIATV_WINTAB_2277_112_2_18_2_3_18_93 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID"
#define MMIATV_WINTAB_2282_112_2_18_2_3_18_94 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
#define MMIATV_WINTAB_2291_112_2_18_2_3_18_95 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID"
#define MMIATV_WINTAB_2388_112_2_18_2_3_18_96 "[MMIATV]: HandleFavoriteServiceListOptWinMsg:menu_id 0x%x is error!"
#define MMIATV_WINTAB_2659_112_2_18_2_3_18_97 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID"
#define MMIATV_WINTAB_2664_112_2_18_2_3_18_98 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
#define MMIATV_WINTAB_2670_112_2_18_2_3_18_99 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID"
#define MMIATV_WINTAB_3402_112_2_18_2_3_20_100 "[MMIATV]: DisplayRecordTip media_time =%ld"
#define MMIATV_WINTAB_3964_112_2_18_2_3_21_101 "[MMIATV]: InitATV"
#define MMIATV_WINTAB_3967_112_2_18_2_3_21_102 "[MMIATV]: InitATV ATV is opened!"
#define MMIATV_WINTAB_4020_112_2_18_2_3_21_103 "[MMIATV]: InitAtv lastService=%d\n"
#define MMIATV_WINTAB_4052_112_2_18_2_3_21_104 "[MMIATV]: ReleaseATV"
#define MMIATV_WINTAB_4192_112_2_18_2_3_22_105 "[MMIATV]: PlayPreService current_index=%d, total_num=%d, play_status=%d"
#define MMIATV_WINTAB_4215_112_2_18_2_3_22_106 "[MMIATV]: PlayPreService DRECORDER_ParamCtrl is not finished yet!\n"
#define MMIATV_WINTAB_4235_112_2_18_2_3_22_107 "[MMIATV]: PlayNextService PNULL == node_ptr"
#define MMIATV_WINTAB_4249_112_2_18_2_3_22_108 "[MMIATV]: PlayPreService current_index=%d, total_num=%d, play_status=%d"
#define MMIATV_WINTAB_4272_112_2_18_2_3_22_109 "[MMIATV]: PlayNextService DRECORDER_ParamCtrl is not finished yet!\n"
#define MMIATV_WINTAB_4285_112_2_18_2_3_22_110 "[MMIATV]: PlayNextService PNULL == node_ptr"
#define MMIATV_WINTAB_4334_112_2_18_2_3_22_111 "[MMIATV]: HandleInterrupt screen_mode = %d, is_horizontal = %d"
#define MMIATV_WINTAB_4338_112_2_18_2_3_22_112 "[MMIATV]: HandleInterrupt  MMIATV_GET_FOCUS"
#define MMIATV_WINTAB_4351_112_2_18_2_3_22_113 "[MMIATV]: HandleInterrupt  set mode to normal"
#define MMIATV_WINTAB_4410_112_2_18_2_3_22_114 "[MMIATV]: HandleInterrupt  MMIATV_LOSE_FOCUS"
#define MMIATV_WINTAB_4433_112_2_18_2_3_22_115 "[MMIATV]: HandleInterrupt error mode type!"
#define MMIATV_WINTAB_4446_112_2_18_2_3_22_116 "[MMIATV]: HandleLcdSwitchMsg  play_status = %d, screen_mode = %d, is_horizontal = %d"
#define MMIATV_WINTAB_4450_112_2_18_2_3_22_117 "[MMIATV]: HandleLcdSwitchMsg  atv is not open!"
#define MMIATV_WINTAB_4456_112_2_18_2_3_22_118 "[MMIATV]: HandleLcdSwitchMsg  is inner lcd_switch_msg!"
#define MMIATV_WINTAB_4518_112_2_18_2_3_22_119 "[MMIATV]: SwitchScreenMode screen_mod = %d, is_horizontal = %d, play_status = %d"
#define MMIATV_WINTAB_4590_112_2_18_2_3_23_120 "[MMIATV]: SwitchScreenMode full to half!  screen_angle=%d win_angle=%d"
#define MMIATV_WINTAB_4622_112_2_18_2_3_23_121 "[MMIATV]: SwitchScreenMode error mode type!"
#define MMIATV_WINTAB_4632_112_2_18_2_3_23_122 "[MMIATV]: SetInnerLcdSwitchFlag  set to %d"
#define MMIATV_WINTAB_4661_112_2_18_2_3_23_123 "[MMIATV]: SetVolumeCallback  volume = %d"
#define MMIATV_WINTAB_4697_112_2_18_2_3_23_124 "[MMIATV]: MMIATV_TurnVolume play_status = %d, screen_mode = %d"
#define MMIATV_WINTAB_4753_112_2_18_2_3_23_125 "[MMIATV]: SetBrightnessCallback  brightness = %d"
#define MMIATV_WINTAB_4771_112_2_18_2_3_23_126 "[MMIATV]: TurnUpBrightness play_status = %d, screen_mode = %d"
#define MMIATV_WINTAB_4813_112_2_18_2_3_23_127 "[MMIATV]: TurnDownBrightness play_status = %d, screen_mode = %d"
#define MMIATV_WINTAB_5059_112_2_18_2_3_24_128 "[VP] HandleFavoriteServiceListLongOkOptMenuWinMsg msg_id = %x"
#define MMIATV_WINTAB_5099_112_2_18_2_3_24_129 "[VP] HandleFavoriteServiceListLongOkOptMenuWinMsg menu_id = %x"
#define MMIATV_WINTAB_5154_112_2_18_2_3_24_130 "[VP] HandleServiceListLongOkOptMenuWinMsg msg_id = %x"
#define MMIATV_WINTAB_5193_112_2_18_2_3_24_131 "[VP] HandleServiceListLongOkOptMenuWinMsg menu_id = %x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_ATV_TRC)
TRACE_MSG(MMIATV_DATA_91_112_2_18_2_3_6_0,"[MMIATV]: MMIATV_InitData")
TRACE_MSG(MMIATV_DATA_94_112_2_18_2_3_6_1,"[MMIATV]: MMIATV_InitData s_data_ptr != PNULL")
TRACE_MSG(MMIATV_DATA_101_112_2_18_2_3_6_2,"[MMIATV]:s_data_ptr alloc mem error!!!!!!")
TRACE_MSG(MMIATV_DATA_304_112_2_18_2_3_7_3,"[MMIATV]: MMIATV_RestoreFactorySetting")
TRACE_MSG(MMIATV_DATA_510_112_2_18_2_3_7_4,"[MMIATV]: MMIATV_ReadServiceListFile service_head_ptr not null! ")
TRACE_MSG(MMIATV_DATA_530_112_2_18_2_3_7_5,"[MMIATV]: MMIATV_ReadServiceListFile create file error! ")
TRACE_MSG(MMIATV_DATA_537_112_2_18_2_3_7_6,"[MMIATV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIATV_DATA_552_112_2_18_2_3_7_7,"[MMIATV]: ServiceListFile version error! ")
TRACE_MSG(MMIATV_DATA_561_112_2_18_2_3_7_8,"[MMIATV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIATV_DATA_565_112_2_18_2_3_7_9,"[MMIATV]: MMIATV_ReadServiceListFile total list num = %d")
TRACE_MSG(MMIATV_DATA_576_112_2_18_2_3_7_10,"[MMIATV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIATV_DATA_615_112_2_18_2_3_8_11,"[MMIATV]: MMIATV_WriteServiceListFile create file error! ")
TRACE_MSG(MMIATV_DATA_655_112_2_18_2_3_8_12,"[MMIATV]: MMIATV_AddServiceNode node_ptr is null!")
TRACE_MSG(MMIATV_DATA_666_112_2_18_2_3_8_13,"[MMIATV]: MMIATV_AddServiceNode alloc memory error!")
TRACE_MSG(MMIATV_DATA_722_112_2_18_2_3_8_14,"[MMIATV]: MMIATV_GetServiceNodeViaIndex, index = %d")
TRACE_MSG(MMIATV_DATA_726_112_2_18_2_3_8_15,"[MMIATV]: MMIATV_GetServiceNodeViaIndex PNULL==service_head_ptr")
TRACE_MSG(MMIATV_DATA_738_112_2_18_2_3_8_16,"[MMIATV]: MMIATV_GetServiceNodeViaIndex fail!")
TRACE_MSG(MMIATV_DATA_761_112_2_18_2_3_8_17,"[MMIATV]: MMIATV_ReadFavoriteServiceListFile service_head_ptr not null! ")
TRACE_MSG(MMIATV_DATA_782_112_2_18_2_3_8_18,"[MMIATV]: MMIATV_ReadFavoriteServiceListFile create file error! ")
TRACE_MSG(MMIATV_DATA_789_112_2_18_2_3_8_19,"[MMIATV]: FavoriteServiceListFile error! ")
TRACE_MSG(MMIATV_DATA_804_112_2_18_2_3_8_20,"[MMIATV]: FavoriteServiceListFile version error! ")
TRACE_MSG(MMIATV_DATA_813_112_2_18_2_3_8_21,"[MMIATV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIATV_DATA_817_112_2_18_2_3_8_22,"[MMIATV]: MMIATV_ReadFavoriteServiceListFile total list num = %d")
TRACE_MSG(MMIATV_DATA_828_112_2_18_2_3_8_23,"[MMIATV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIATV_DATA_867_112_2_18_2_3_8_24,"[MMIATV]: MMIATV_WriteFavoriteServiceListFile create file error! ")
TRACE_MSG(MMIATV_DATA_907_112_2_18_2_3_8_25,"[MMIATV]: MMIATV_AddFavoriteServiceNode node_ptr is null!")
TRACE_MSG(MMIATV_DATA_918_112_2_18_2_3_8_26,"[MMIATV]: MMIATV_AddFavoriteServiceNode alloc memory error!")
TRACE_MSG(MMIATV_DATA_948_112_2_18_2_3_8_27,"[MMIATV]: MMIATV_DeleteFavoriteServiceNode node_ptr is null!")
TRACE_MSG(MMIATV_DATA_954_112_2_18_2_3_8_28,"[MMIATV]: MMIATV_DeleteFavoriteServiceNode list is null!")
TRACE_MSG(MMIATV_DATA_978_112_2_18_2_3_8_29,"[MMIATV]: MMIATV_DeleteFavoriteServiceNode find node error!")
TRACE_MSG(MMIATV_DATA_1033_112_2_18_2_3_8_30,"[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex, index = %d")
TRACE_MSG(MMIATV_DATA_1037_112_2_18_2_3_8_31,"[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex PNULL==service_head_ptr")
TRACE_MSG(MMIATV_DATA_1049_112_2_18_2_3_8_32,"[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex fail!")
TRACE_MSG(MMIATV_EXPORT_83_112_2_18_2_3_9_33,"[MMIATV]: MMIAPIATV_OpenATV")
TRACE_MSG(MMIATV_EXPORT_86_112_2_18_2_3_9_34,"[MMIATV]: MMIAPIATV_OpenATV app is opened!")
TRACE_MSG(MMIATV_EXPORT_103_112_2_18_2_3_9_35,"MMIAPIATV_OpenATV java running =%d")
TRACE_MSG(MMIATV_EXPORT_112_112_2_18_2_3_9_36,"MMIAPIATV_OpenATV call running =%d")
TRACE_MSG(MMIATV_MAIN_171_112_2_18_2_3_10_37,"[MMIATV]: MMIATV_Open  DRECORDER_SelectSrcDevice ret = %d")
TRACE_MSG(MMIATV_MAIN_173_112_2_18_2_3_10_38,"[MMIATV]: MMIATV_Open  DRECORDER_Open ret = %d")
TRACE_MSG(MMIATV_MAIN_177_112_2_18_2_3_10_39,"[MMIATV]: MMIATV_Open already opened!")
TRACE_MSG(MMIATV_MAIN_210_112_2_18_2_3_10_40,"[MMIATV]: MMIATV_Close  DRECORDER_Close ret = %d")
TRACE_MSG(MMIATV_MAIN_231_112_2_18_2_3_10_41,"[MMIATV]: MMIATV_StartSearchService play_status=%d")
TRACE_MSG(MMIATV_MAIN_263_112_2_18_2_3_10_42,"[MMIATV]: MMIATV_StartSearchService total_num=%d")
TRACE_MSG(MMIATV_MAIN_275_112_2_18_2_3_10_43,"[MMIATV]: DRECORDER_ATV_StartScanChn search_ret=%d")
TRACE_MSG(MMIATV_MAIN_294_112_2_18_2_3_10_44,"[MMIATV]: MMIATV_StartSearchService  play_status error!")
TRACE_MSG(MMIATV_MAIN_306_112_2_18_2_3_10_45,"[MMIATV]: MMIATV_StopSearchService play_status=%d")
TRACE_MSG(MMIATV_MAIN_312_112_2_18_2_3_10_46,"[MMIATV]: StopSearch search_ret = %d")
TRACE_MSG(MMIATV_MAIN_354_112_2_18_2_3_10_47,"[MMIATV]: MMIATV_PlayService play_status=%d service_id=%d")
TRACE_MSG(MMIATV_MAIN_361_112_2_18_2_3_10_48,"[MMIATV]: MMIATV_PlayService play win is not focus!")
TRACE_MSG(MMIATV_MAIN_366_112_2_18_2_3_10_49,"[MMIATV]: MMIATV_PlayService DRECORDER_ParamCtrl is not finished yet!")
TRACE_MSG(MMIATV_MAIN_375_112_2_18_2_3_10_50,"[MMIATV]: MMIATV_PlayService DRECORDER_ParamCtrl MMIATV_STATUS_PLAYING ret=%d")
TRACE_MSG(MMIATV_MAIN_412_112_2_18_2_3_10_51,"[MMIATV]: MMIATV_PlayService  error!!")
TRACE_MSG(MMIATV_MAIN_421_112_2_18_2_3_10_52,"[MMIATV]: MMIATV_PlayService play_status error!")
TRACE_MSG(MMIATV_MAIN_433_112_2_18_2_3_10_53,"[MMIATV]: StopPlayService play_status=%d")
TRACE_MSG(MMIATV_MAIN_461_112_2_18_2_3_10_54,"[MMIATV]: MMIATV_HandleSysMsg error!")
TRACE_MSG(MMIATV_MAIN_608_112_2_18_2_3_11_55,"[MMIATV]: GetPreviewParam screen_mode=%d, preview_type=%d")
TRACE_MSG(MMIATV_MAIN_616_112_2_18_2_3_11_56,"[MMIATV]: GetPreviewParam cur_angle = %d")
TRACE_MSG(MMIATV_MAIN_658_112_2_18_2_3_11_57,"[MMIATV]: GetPreviewParam screen_mode error")
TRACE_MSG(MMIATV_MAIN_666_112_2_18_2_3_11_58,"[MMIATV]: GetPreviewParam , logic_coor.rect {%d %d %d %d} rotation %d")
TRACE_MSG(MMIATV_MAIN_697_112_2_18_2_3_11_59,"[MMIATV]: GetPreviewParam , disp_rect {%d %d %d %d},target rect {%d %d %d %d},rotation %d,disp_mode %d")
TRACE_MSG(MMIATV_MAIN_717_112_2_18_2_3_11_60,"[MMIATV]: HandleReportMsg  param_ptr error!!!!!!!!!!!!!!!!!! ")
TRACE_MSG(MMIATV_MAIN_723_112_2_18_2_3_11_61,"[MMIATV]: HandleReportMsg  ATV is not open! ")
TRACE_MSG(MMIATV_MAIN_731_112_2_18_2_3_11_62,"[MMIATV]: HandleReportMsg atv_chn_id=%d atv_chn_is_valid=%d")
TRACE_MSG(MMIATV_MAIN_746_112_2_18_2_3_11_63,"[MMIATV]: HandleReportMsg report_type = %d")
TRACE_MSG(MMIATV_MAIN_754_112_2_18_2_3_11_64,"[MMIATV]: HandleReportMsg RSSI changed, atv_rssi=%d")
TRACE_MSG(MMIATV_MAIN_788_112_2_18_2_3_11_65,"[MMIATV]: HandleReportMsg report_type=%d error!!!!!!!!!!!!!")
TRACE_MSG(MMIATV_MAIN_804_112_2_18_2_3_11_66,"[MMIATV]: AddServiceNode cur_num=%d service_id=%d")
TRACE_MSG(MMIATV_MAIN_826_112_2_18_2_3_11_67,"[MMIATV]: CHN_ParamCtrlCallBack")
TRACE_MSG(MMIATV_MAIN_839_112_2_18_2_3_11_68,"[MMIATV]: ParamCtrlCallBack")
TRACE_MSG(MMIATV_MAIN_853_112_2_18_2_3_11_69,"[MMIATV]: ReportCallback  report_ptr is null !")
TRACE_MSG(MMIATV_MAIN_868_112_2_18_2_3_11_70,"[MMIATV]: ReportCallback  report_type=%d")
TRACE_MSG(MMIATV_MAIN_956_112_2_18_2_3_11_71,"[MMIATV]: RecordEndCallBack record end i_type = %d")
TRACE_MSG(MMIATV_MAIN_999_112_2_18_2_3_11_72,"[MMIATV]:MMIATV_StartRecord video type=%d error!!!!!!!!!!!")
TRACE_MSG(MMIATV_MAIN_1023_112_2_18_2_3_11_73,"[MMIATV]: MMIATV_StartRecord success!")
TRACE_MSG(MMIATV_MAIN_1027_112_2_18_2_3_11_74,"[MMIATV]: MMIATV_StartRecord fail ret=%d!")
TRACE_MSG(MMIATV_MAIN_1087_112_2_18_2_3_12_75,"[MMIATV]: MMIATV_StopRecord ret= %d")
TRACE_MSG(MMIATV_MAIN_1102_112_2_18_2_3_12_76,"[MMIATV] MMIATV_SwitchPlayDevice is_a2dp_connected = %d, is_bt_connected = %d")
TRACE_MSG(MMIATV_MAIN_1125_112_2_18_2_3_12_77,"[MMIATV]: BTEventNext enter")
TRACE_MSG(MMIATV_MAIN_1135_112_2_18_2_3_12_78,"[MMIATV]: BTEventPre enter")
TRACE_MSG(MMIATV_MAIN_1149_112_2_18_2_3_12_79,"[MMIATV]:AtvPlayCallBack event=%d")
TRACE_MSG(MMIATV_MAIN_1160_112_2_18_2_3_12_80,"[MMIATV]:AtvPlayCallBack report=%d,data=%d")
TRACE_MSG(MMIATV_WINTAB_948_112_2_18_2_3_15_81,"[MMIATV]: HandleATVRecordEnd i_type= %d")
TRACE_MSG(MMIATV_WINTAB_1196_112_2_18_2_3_16_82,"[MMIATV]: HandleMainWinMsg  MSG_APP_CANCEL")
TRACE_MSG(MMIATV_WINTAB_1320_112_2_18_2_3_16_83,"[MMIATV]:HandleTpMsg  point.x = %d, point.y = %d")
TRACE_MSG(MMIATV_WINTAB_1624_112_2_18_2_3_16_84,"[MMIATV]:VolButtonCallback")
TRACE_MSG(MMIATV_WINTAB_1643_112_2_18_2_3_16_85,"[MMIATV]:PreButtonCallback")
TRACE_MSG(MMIATV_WINTAB_1662_112_2_18_2_3_16_86,"[MMIATV]:ScreenButtonCallback")
TRACE_MSG(MMIATV_WINTAB_1681_112_2_18_2_3_17_87,"[MMIATV]:NextButtonCallback")
TRACE_MSG(MMIATV_WINTAB_1700_112_2_18_2_3_17_88,"[MMIATV]:RecordButtonCallback")
TRACE_MSG(MMIATV_WINTAB_1729_112_2_18_2_3_17_89,"[MMIATV]:OptionButtonCallback")
TRACE_MSG(MMIATV_WINTAB_1748_112_2_18_2_3_17_90,"[MMIATV]:ReturnButtonCallback")
TRACE_MSG(MMIATV_WINTAB_2065_112_2_18_2_3_17_91,"[MMIATV]: HandleMainOptWinMsg:menu_id 0x%x is error!")
TRACE_MSG(MMIATV_WINTAB_2068_112_2_18_2_3_17_92,"[MMIATV]: HandleMainOptWinMsg:menu_id 0x%x is error!")
TRACE_MSG(MMIATV_WINTAB_2277_112_2_18_2_3_18_93,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID")
TRACE_MSG(MMIATV_WINTAB_2282_112_2_18_2_3_18_94,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID")
TRACE_MSG(MMIATV_WINTAB_2291_112_2_18_2_3_18_95,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID")
TRACE_MSG(MMIATV_WINTAB_2388_112_2_18_2_3_18_96,"[MMIATV]: HandleFavoriteServiceListOptWinMsg:menu_id 0x%x is error!")
TRACE_MSG(MMIATV_WINTAB_2659_112_2_18_2_3_18_97,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID")
TRACE_MSG(MMIATV_WINTAB_2664_112_2_18_2_3_18_98,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID")
TRACE_MSG(MMIATV_WINTAB_2670_112_2_18_2_3_18_99,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID")
TRACE_MSG(MMIATV_WINTAB_3402_112_2_18_2_3_20_100,"[MMIATV]: DisplayRecordTip media_time =%ld")
TRACE_MSG(MMIATV_WINTAB_3964_112_2_18_2_3_21_101,"[MMIATV]: InitATV")
TRACE_MSG(MMIATV_WINTAB_3967_112_2_18_2_3_21_102,"[MMIATV]: InitATV ATV is opened!")
TRACE_MSG(MMIATV_WINTAB_4020_112_2_18_2_3_21_103,"[MMIATV]: InitAtv lastService=%d\n")
TRACE_MSG(MMIATV_WINTAB_4052_112_2_18_2_3_21_104,"[MMIATV]: ReleaseATV")
TRACE_MSG(MMIATV_WINTAB_4192_112_2_18_2_3_22_105,"[MMIATV]: PlayPreService current_index=%d, total_num=%d, play_status=%d")
TRACE_MSG(MMIATV_WINTAB_4215_112_2_18_2_3_22_106,"[MMIATV]: PlayPreService DRECORDER_ParamCtrl is not finished yet!\n")
TRACE_MSG(MMIATV_WINTAB_4235_112_2_18_2_3_22_107,"[MMIATV]: PlayNextService PNULL == node_ptr")
TRACE_MSG(MMIATV_WINTAB_4249_112_2_18_2_3_22_108,"[MMIATV]: PlayPreService current_index=%d, total_num=%d, play_status=%d")
TRACE_MSG(MMIATV_WINTAB_4272_112_2_18_2_3_22_109,"[MMIATV]: PlayNextService DRECORDER_ParamCtrl is not finished yet!\n")
TRACE_MSG(MMIATV_WINTAB_4285_112_2_18_2_3_22_110,"[MMIATV]: PlayNextService PNULL == node_ptr")
TRACE_MSG(MMIATV_WINTAB_4334_112_2_18_2_3_22_111,"[MMIATV]: HandleInterrupt screen_mode = %d, is_horizontal = %d")
TRACE_MSG(MMIATV_WINTAB_4338_112_2_18_2_3_22_112,"[MMIATV]: HandleInterrupt  MMIATV_GET_FOCUS")
TRACE_MSG(MMIATV_WINTAB_4351_112_2_18_2_3_22_113,"[MMIATV]: HandleInterrupt  set mode to normal")
TRACE_MSG(MMIATV_WINTAB_4410_112_2_18_2_3_22_114,"[MMIATV]: HandleInterrupt  MMIATV_LOSE_FOCUS")
TRACE_MSG(MMIATV_WINTAB_4433_112_2_18_2_3_22_115,"[MMIATV]: HandleInterrupt error mode type!")
TRACE_MSG(MMIATV_WINTAB_4446_112_2_18_2_3_22_116,"[MMIATV]: HandleLcdSwitchMsg  play_status = %d, screen_mode = %d, is_horizontal = %d")
TRACE_MSG(MMIATV_WINTAB_4450_112_2_18_2_3_22_117,"[MMIATV]: HandleLcdSwitchMsg  atv is not open!")
TRACE_MSG(MMIATV_WINTAB_4456_112_2_18_2_3_22_118,"[MMIATV]: HandleLcdSwitchMsg  is inner lcd_switch_msg!")
TRACE_MSG(MMIATV_WINTAB_4518_112_2_18_2_3_22_119,"[MMIATV]: SwitchScreenMode screen_mod = %d, is_horizontal = %d, play_status = %d")
TRACE_MSG(MMIATV_WINTAB_4590_112_2_18_2_3_23_120,"[MMIATV]: SwitchScreenMode full to half!  screen_angle=%d win_angle=%d")
TRACE_MSG(MMIATV_WINTAB_4622_112_2_18_2_3_23_121,"[MMIATV]: SwitchScreenMode error mode type!")
TRACE_MSG(MMIATV_WINTAB_4632_112_2_18_2_3_23_122,"[MMIATV]: SetInnerLcdSwitchFlag  set to %d")
TRACE_MSG(MMIATV_WINTAB_4661_112_2_18_2_3_23_123,"[MMIATV]: SetVolumeCallback  volume = %d")
TRACE_MSG(MMIATV_WINTAB_4697_112_2_18_2_3_23_124,"[MMIATV]: MMIATV_TurnVolume play_status = %d, screen_mode = %d")
TRACE_MSG(MMIATV_WINTAB_4753_112_2_18_2_3_23_125,"[MMIATV]: SetBrightnessCallback  brightness = %d")
TRACE_MSG(MMIATV_WINTAB_4771_112_2_18_2_3_23_126,"[MMIATV]: TurnUpBrightness play_status = %d, screen_mode = %d")
TRACE_MSG(MMIATV_WINTAB_4813_112_2_18_2_3_23_127,"[MMIATV]: TurnDownBrightness play_status = %d, screen_mode = %d")
TRACE_MSG(MMIATV_WINTAB_5059_112_2_18_2_3_24_128,"[VP] HandleFavoriteServiceListLongOkOptMenuWinMsg msg_id = %x")
TRACE_MSG(MMIATV_WINTAB_5099_112_2_18_2_3_24_129,"[VP] HandleFavoriteServiceListLongOkOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIATV_WINTAB_5154_112_2_18_2_3_24_130,"[VP] HandleServiceListLongOkOptMenuWinMsg msg_id = %x")
TRACE_MSG(MMIATV_WINTAB_5193_112_2_18_2_3_24_131,"[VP] HandleServiceListLongOkOptMenuWinMsg menu_id = %x")
END_TRACE_MAP(MMI_APP_ATV_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_ATV_TRC_H_

