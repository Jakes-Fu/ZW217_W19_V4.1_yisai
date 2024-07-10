/******************************************************************************
 ** File Name:      mmi_app_idle_trc.h                                         *
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
//trace_id:183
#ifndef _MMI_APP_IDLE_TRC_H_
#define _MMI_APP_IDLE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MAINAPP_490_112_2_18_2_23_33_0 "MAIN_HandleSysMsg: begin MISC_MSG_RTC_MIN! back light on!"
#define MAINAPP_508_112_2_18_2_23_33_1 "MAIN_HandleSysMsg: MISC_MSG_RTC_MIN! back light off!"
#define MAINAPP_528_112_2_18_2_23_34_2 "MAIN_HandleSysMsg: begin MISC_MSG_RTC_HOUR! back light on!"
#define MAINAPP_585_112_2_18_2_23_34_3 "[idle] IdleWin_HandleMsg msg_id 0x%x"
#define MAINAPP_1073_112_2_18_2_23_35_4 "IdleWin_HandleMsg case MSG_SET_VALIDATE_PRIVACY_PWD_RETURN PNULL == result_info_ptr"
#define MAINAPP_1385_112_2_18_2_23_35_5 "IDLEWIN_HandleWinMsg: handle headset MSG!"
#define MAINAPP_1396_112_2_18_2_23_35_6 "IdleWin_HandleMsg:received timer msg!"
#define MAINAPP_1403_112_2_18_2_23_35_7 "IdleWin_HandleMsg: handle the anim wallpater timer!"
#define MAINAPP_1553_112_2_18_2_23_36_8 "IdleWin_HandleMsg MSG_KEYLONG_HASH!"
#define MAINAPP_1710_112_2_18_2_23_36_9 "MMAPIIDLE_init:call_ring_type is %d!"
#define MAINAPP_1732_112_2_18_2_23_36_10 "MMIAPIIDLE_init: call_ring_type is error!"
#define MAINAPP_1948_112_2_18_2_23_36_11 "HandleIdleMsg:idle_style %d is error!"
#define MMI_SUBWINTAB_607_112_2_18_2_23_38_12 "GetSubWinTextStyle ERROR! PNULL == style_ptr"
#define MMI_SUBWINTAB_706_112_2_18_2_23_38_13 "DisplaySubLcdTime 12: Xpos=%d Ypos=%d"
#define MMI_SUBWINTAB_743_112_2_18_2_23_38_14 "DisplaySubLcdTime 24: Xpos=%d Ypos=%d"
#define MMI_SUBWINTAB_783_112_2_18_2_23_38_15 "DisplaySubLcdDateOrStr 0 == s_sub_lcd_info.content_num"
#define MMI_SUBWINTAB_910_112_2_18_2_23_39_16 "DisplaySubLcdDate: pos=%d"
#define MMI_SUBWINTAB_1054_112_2_18_2_23_39_17 "MMISUB_UpdateDisplaySubLcd error! num=%d"
#define MMI_SUBWINTAB_1207_112_2_18_2_23_39_18 "MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_ALARM, PNULL == str_ptr"
#define MMI_SUBWINTAB_1225_112_2_18_2_23_39_19 "MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_CALL, PNULL == str_ptr"
#define MMI_SUBWINTAB_1237_112_2_18_2_23_39_20 "MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_MISSED_CALL, PNULL == str_ptr"
#define MMIIDLE_CSTYLE_988_112_2_18_2_23_42_21 "MMIIDLE OutputIdleWinCountedTime PNULL == disp_str.wstr_ptr"
#define MMIIDLE_DIAL_2247_112_2_18_2_23_48_22 "mmiidle_dial.c HandleDialWinMsg edit_ctrl_id is not active ctrl, can't handle the message!!!"
#define MMIIDLE_DIAL_3233_112_2_18_2_23_50_23 "HandleDialPenLongOkMsg:button_ctrl_id %d don't handle!"
#define MMIIDLE_DIAL_3496_112_2_18_2_23_51_24 "mmiidle_dial.c HandleDialWinMsg edit_ctrl_id is not active ctrl, can't handle the message!!!"
#define MMIIDLE_DIAL_5070_112_2_18_2_23_54_25 "UpdateSearchResult err para"
#define MMIIDLE_DIAL_5223_112_2_18_2_23_54_26 "AppendMatchingInfo err append idx:%d, total idx: %d"
#define MMIIDLE_DIAL_5255_112_2_18_2_23_54_27 "AppendMatchingInfo err num is invalid"
#define MMIIDLE_DIAL_5403_112_2_18_2_23_55_28 "AppendMatchingInfo err num is invalid"
#define MMIIDLE_DIAL_6280_112_2_18_2_23_56_29 "mainapp: CheckSIMADNInput adn_entry_ptr No Memory"
#define MMIIDLE_DIAL_6293_112_2_18_2_23_56_30 "mainapp: CheckSIMADNInput err_code=%x"
#define MMIIDLE_DISPLAY_966_112_2_18_2_23_59_31 "GetDisplayRect ERROR! tp_type = %d"
#define MMIIDLE_DISPLAY_986_112_2_18_2_23_59_32 "SetDisplayRect ERROR! tp_type = %d"
#define MMIIDLE_DISPLAY_1117_112_2_18_2_23_59_33 "DisplayDate ERROR! data_wstr_len = %d"
#define MMIIDLE_DISPLAY_1397_112_2_18_2_24_0_34 "GetLineType ERROR! line_index = %d"
#define MMIIDLE_DISPLAY_1776_112_2_18_2_24_1_35 "DisplayNetname ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_1929_112_2_18_2_24_1_36 "DisplayLineAndSelImg ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_1935_112_2_18_2_24_1_37 "DisplayLineAndSelImg ERROR! PNULL == lcd_dev_ptr"
#define MMIIDLE_DISPLAY_2005_112_2_18_2_24_1_38 "GetCurItemPos:prompt_pos %d is error!"
#define MMIIDLE_DISPLAY_2063_112_2_18_2_24_1_39 "DisplaySchedule ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_2135_112_2_18_2_24_1_40 "DisplayCountDown ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_2280_112_2_18_2_24_2_41 "DisplayMp3Name ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_2393_112_2_18_2_24_2_42 "DisplayFmName:fm_status %d is error!"
#define MMIIDLE_DISPLAY_2411_112_2_18_2_24_2_43 "DisplayFmName ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_2513_112_2_18_2_24_2_44 "DisplayWords ERROR! PNULL == words_ptr"
#define MMIIDLE_DISPLAY_2538_112_2_18_2_24_2_45 "DisplayWords ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_2628_112_2_18_2_24_2_46 "DisplayDcd ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_2678_112_2_18_2_24_3_47 "Display Wlan ERROR! line_index =%d"
#define MMIIDLE_DISPLAY_3093_112_2_18_2_24_3_48 "OpenFocusItemWin:focus_line_type %d is error!"
#define MMIIDLE_DISPLAY_3148_112_2_18_2_24_3_49 "OpenFocusItemWin:focus_line_type %d is error!"
#define MMIIDLE_DISPLAY_3489_112_2_18_2_24_4_50 "OpenPromptWin:focus_line_type %d is error!"
#define MMIIDLE_DISPLAY_3588_112_2_18_2_24_4_51 "SmartIdleHandleTpDown:tp_type %d is error!"
#define MMIIDLE_DISPLAY_3717_112_2_18_2_24_5_52 "SmartIdleHandleTpUp:tp_type %d is error!"
#define MMIIDLE_DISPLAY_3931_112_2_18_2_24_5_53 "UpdateShortcutOrLine:don't update line_type = %d"
#define MMIIDLE_FUNC_138_112_2_18_2_24_7_54 "MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_ptr"
#define MMIIDLE_FUNC_164_112_2_18_2_24_7_55 "MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_str.wstr_ptr"
#define MMIIDLE_FUNC_176_112_2_18_2_24_7_56 "MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_str.wstr_ptr"
#define MMIIDLE_FUNC_219_112_2_18_2_24_7_57 "MMIIDLE:MMIIDLE_DisplaySchedule PNULL == event_str_ptr"
#define MMIIDLE_FUNC_244_112_2_18_2_24_7_58 "MMIIDLE:MMIIDLE_DisplaySchedule PNULL == full_str_ptr"
#define MMIIDLE_FUNC_265_112_2_18_2_24_7_59 "MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_full_event_str.wstr_ptr"
#define MMIIDLE_FUNC_352_112_2_18_2_24_7_60 "GetIdleTextStyle ERROR! PNULL == style_ptr"
#define MMIIDLE_FUNC_567_112_2_18_2_24_7_61 "NeedShowIconAccordingToDualSysSetting ERROR! MMI_DUAL_SYS_MAX <= dual_sys,dual_sys = %d"
#define MMIIDLE_FUNC_592_112_2_18_2_24_8_62 "GetIdleTextStyle ERROR! PNULL == style_ptr"
#define MMIIDLE_FUNC_785_112_2_18_2_24_8_63 "MMIAPIIDLE_MAIN_SetIdleModeText ERROR! dual_sys =%d, PNULL == stk_idle_str"
#define MMIIDLE_FUNC_788_112_2_18_2_24_8_64 "MMIAPIIDLE_MAIN_SetIdleModeText dual_sys=%d, len=%d"
#define MMIIDLE_HWICON_355_112_2_18_2_24_9_65 "Handle_HW_Icon_Exec_Func : s_cur_hw_icon_func = %x"
#define MMIIDLE_HWICON_372_112_2_18_2_24_9_66 "Handle_HW_Icon_Exec_Func : s_cur_hw_icon_func = %x"
#define MMIIDLE_HWICON_445_112_2_18_2_24_10_67 "HandleHWIconPBProcessMsg enter"
#define MMIIDLE_HWICON_453_112_2_18_2_24_10_68 "HandleHWIconPBProcessMsg forcus"
#define MMIIDLE_HWICON_477_112_2_18_2_24_10_69 "HandleHWIconPromptWinMsg msg_id = %x"
#define MMIIDLE_STATUSBAR_1174_112_2_18_2_24_13_70 "SetStatusBarInfo MMI_WIN_ICON_BATTERY"
#define MMIIDLE_STATUSBAR_1179_112_2_18_2_24_13_71 "SetStatusBarInfo MMI_WIN_ICON_BATTERY_AUX"
#define MMIIDLE_STATUSBAR_1335_112_2_18_2_24_14_72 "MAIN_SetIdleLineState ERROR! dual_sys < MMI_DUAL_SYS_MAX,dual_sys = %d"
#define MMIIDLE_STATUSBAR_1581_112_2_18_2_24_14_73 "StatusbarGetSysDate ERROR! PNULL == date_wstr"
#define MMIIDLE_STATUSBAR_1647_112_2_18_2_24_14_74 "MAIN_SetStbDispTime Set Time OK!"
#define MMIIDLE_STATUSBAR_1715_112_2_18_2_24_14_75 "MAIN_StatusbarGetSysTime ERROR! PNULL == time_wstr"
#define MMIIDLE_STATUSBAR_1810_112_2_18_2_24_14_76 "SetRoamingInfo is_display=%d, roaming_index=%d"
#define MMIIDLE_STATUSBAR_1913_112_2_18_2_24_15_77 "GetLineRoadIconId is_display=%d, line2_index=%d"
#define MMIIDLE_STATUSBAR_1977_112_2_18_2_24_15_78 "GetLineCallForwardIconId is_display=%d, forward_index=%d"
#define MMIIDLE_STATUSBAR_2045_112_2_18_2_24_15_79 "MAIN_SetIdleRoamingState ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d"
#define MMIIDLE_STATUSBAR_2048_112_2_18_2_24_15_80 "MAIN_SetIdleRoamingState: e_dual_sys=%d, is_roaming = %d"
#define MMIIDLE_STATUSBAR_2371_112_2_18_2_24_16_81 "SetGprsInfo error dual_sys: %d "
#define MMIIDLE_STATUSBAR_2387_112_2_18_2_24_16_82 "SetGprsInfo dual_sys:%d type:%d event:%d,network_status_rat:%d"
#define MMIIDLE_STATUSBAR_2390_112_2_18_2_24_16_83 "SetGprsInfo data error! data_service_type = %d,pdp_event = %d"
#define MMIIDLE_STATUSBAR_2481_112_2_18_2_24_16_84 "MAIN_SetIdleGprsState ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d"
#define MMIIDLE_STATUSBAR_2484_112_2_18_2_24_16_85 "MAIN_SetIdleGprsState: e_dual_sys=%d, gprs_state = %d"
#define MMIIDLE_STATUSBAR_2672_112_2_18_2_24_16_86 "GetActiveIconId data_service_type: %d, index: %d"
#define MMIIDLE_STATUSBAR_2832_112_2_18_2_24_16_87 "MAIN_SetIdleChargeState batlevel=%d, bat_icon_type=%d, cur_charge_bat=%d"
#define MMIIDLE_STATUSBAR_2836_112_2_18_2_24_16_88 "MAIN_SetIdleChargeState is_charge = %d,s_idlewin_info.is_charge = %d"
#define MMIIDLE_STATUSBAR_2850_112_2_18_2_24_17_89 "MAIN_SetIdleChargeState %d, %d, %d"
#define MMIIDLE_STATUSBAR_2892_112_2_18_2_24_17_90 "MAIN_SetIdleChargeState %d, %d, %d, %d"
#define MMIIDLE_STATUSBAR_2915_112_2_18_2_24_17_91 "MAIN_SetIdleDbatLevel  dbat_info->p_bat[DBAT_MAIN_BAT]=%d"
#define MMIIDLE_STATUSBAR_2926_112_2_18_2_24_17_92 "MAIN_SetIdleDbatLevel  bat_level=%d,s_idlewin_info.batlevel=%d"
#define MMIIDLE_STATUSBAR_2945_112_2_18_2_24_17_93 "MAIN_SetIdleDbatLevel  dbat_info->p_bat[DBAT_AUX_BAT]=%d"
#define MMIIDLE_STATUSBAR_2957_112_2_18_2_24_17_94 "MAIN_SetIdleDbatLevel  bat_level=%d,MMIIDLE_GetIdleWinInfo()->batlevel_aux=%d"
#define MMIIDLE_STATUSBAR_2975_112_2_18_2_24_17_95 "MAIN_SetIdleDbatLevel  is_update=%d"
#define MMIIDLE_STATUSBAR_3007_112_2_18_2_24_17_96 "MMIAPIIDLE_SetStbBatteryInfo is_main_battery=%d,cur_use_bat=%d"
#define MMIIDLE_STATUSBAR_3025_112_2_18_2_24_17_97 "MMIAPIIDLE_SetStbBatteryInfo bat_level=%d"
#define MMIIDLE_STATUSBAR_3189_112_2_18_2_24_17_98 "MAIN_SetIdleRxLevel ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d"
#define MMIIDLE_STATUSBAR_3196_112_2_18_2_24_17_99 "MAIN_SetIdleRxLevel %d,%d,%d, %d"
#define MMIIDLE_STATUSBAR_3389_112_2_18_2_24_18_100 "AddMissCallStbScrollItem call info alloc failed"
#define MMIIDLE_STATUSBAR_3486_112_2_18_2_24_18_101 "MMIMAIN_AppendStbScrollItem() item_type = %d"
#define MMIIDLE_STATUSBAR_3597_112_2_18_2_24_18_102 "MAIN_SetIdleBTStateWhenPowerOn %d"
#define MMIIDLE_STATUSBAR_3612_112_2_18_2_24_18_103 "MAIN_SetIdleBlueToothState %d"
#define MMIIDLE_STATUSBAR_3759_112_2_18_2_24_18_104 "MAIN_SetIdleBatLevel bat_level = %d,idle level =%d"
#define MMIIDLE_STATUSBAR_3864_112_2_18_2_24_19_105 "MAIN_SetIdleTdState"
#define MMIIDLE_STATUSBAR_3877_112_2_18_2_24_19_106 "MAIN_SetIdleUnknownState"
#define MMIIDLE_STATUSBAR_4024_112_2_18_2_24_19_107 "MAIN_SetIdleShakeState vibrator and silent are true!"
#define MMIIDLE_STATUSBAR_4043_112_2_18_2_24_19_108 "MAIN_SetIdleSilentState vibrator and silent are true!"
#define MMIIDLE_STATUSBAR_4140_112_2_18_2_24_19_109 "MMIAPIIDLE_UpdateQQState "
#define MMIIDLE_STATUSBAR_4264_112_2_18_2_24_19_110 "MMIAPIIDLE_UpdateAspState "
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_IDLE_TRC)
TRACE_MSG(MAINAPP_490_112_2_18_2_23_33_0,"MAIN_HandleSysMsg: begin MISC_MSG_RTC_MIN! back light on!")
TRACE_MSG(MAINAPP_508_112_2_18_2_23_33_1,"MAIN_HandleSysMsg: MISC_MSG_RTC_MIN! back light off!")
TRACE_MSG(MAINAPP_528_112_2_18_2_23_34_2,"MAIN_HandleSysMsg: begin MISC_MSG_RTC_HOUR! back light on!")
TRACE_MSG(MAINAPP_585_112_2_18_2_23_34_3,"[idle] IdleWin_HandleMsg msg_id 0x%x")
TRACE_MSG(MAINAPP_1073_112_2_18_2_23_35_4,"IdleWin_HandleMsg case MSG_SET_VALIDATE_PRIVACY_PWD_RETURN PNULL == result_info_ptr")
TRACE_MSG(MAINAPP_1385_112_2_18_2_23_35_5,"IDLEWIN_HandleWinMsg: handle headset MSG!")
TRACE_MSG(MAINAPP_1396_112_2_18_2_23_35_6,"IdleWin_HandleMsg:received timer msg!")
TRACE_MSG(MAINAPP_1403_112_2_18_2_23_35_7,"IdleWin_HandleMsg: handle the anim wallpater timer!")
TRACE_MSG(MAINAPP_1553_112_2_18_2_23_36_8,"IdleWin_HandleMsg MSG_KEYLONG_HASH!")
TRACE_MSG(MAINAPP_1710_112_2_18_2_23_36_9,"MMAPIIDLE_init:call_ring_type is %d!")
TRACE_MSG(MAINAPP_1732_112_2_18_2_23_36_10,"MMIAPIIDLE_init: call_ring_type is error!")
TRACE_MSG(MAINAPP_1948_112_2_18_2_23_36_11,"HandleIdleMsg:idle_style %d is error!")
TRACE_MSG(MMI_SUBWINTAB_607_112_2_18_2_23_38_12,"GetSubWinTextStyle ERROR! PNULL == style_ptr")
TRACE_MSG(MMI_SUBWINTAB_706_112_2_18_2_23_38_13,"DisplaySubLcdTime 12: Xpos=%d Ypos=%d")
TRACE_MSG(MMI_SUBWINTAB_743_112_2_18_2_23_38_14,"DisplaySubLcdTime 24: Xpos=%d Ypos=%d")
TRACE_MSG(MMI_SUBWINTAB_783_112_2_18_2_23_38_15,"DisplaySubLcdDateOrStr 0 == s_sub_lcd_info.content_num")
TRACE_MSG(MMI_SUBWINTAB_910_112_2_18_2_23_39_16,"DisplaySubLcdDate: pos=%d")
TRACE_MSG(MMI_SUBWINTAB_1054_112_2_18_2_23_39_17,"MMISUB_UpdateDisplaySubLcd error! num=%d")
TRACE_MSG(MMI_SUBWINTAB_1207_112_2_18_2_23_39_18,"MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_ALARM, PNULL == str_ptr")
TRACE_MSG(MMI_SUBWINTAB_1225_112_2_18_2_23_39_19,"MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_CALL, PNULL == str_ptr")
TRACE_MSG(MMI_SUBWINTAB_1237_112_2_18_2_23_39_20,"MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_MISSED_CALL, PNULL == str_ptr")
TRACE_MSG(MMIIDLE_CSTYLE_988_112_2_18_2_23_42_21,"MMIIDLE OutputIdleWinCountedTime PNULL == disp_str.wstr_ptr")
TRACE_MSG(MMIIDLE_DIAL_2247_112_2_18_2_23_48_22,"mmiidle_dial.c HandleDialWinMsg edit_ctrl_id is not active ctrl, can't handle the message!!!")
TRACE_MSG(MMIIDLE_DIAL_3233_112_2_18_2_23_50_23,"HandleDialPenLongOkMsg:button_ctrl_id %d don't handle!")
TRACE_MSG(MMIIDLE_DIAL_3496_112_2_18_2_23_51_24,"mmiidle_dial.c HandleDialWinMsg edit_ctrl_id is not active ctrl, can't handle the message!!!")
TRACE_MSG(MMIIDLE_DIAL_5070_112_2_18_2_23_54_25,"UpdateSearchResult err para")
TRACE_MSG(MMIIDLE_DIAL_5223_112_2_18_2_23_54_26,"AppendMatchingInfo err append idx:%d, total idx: %d")
TRACE_MSG(MMIIDLE_DIAL_5255_112_2_18_2_23_54_27,"AppendMatchingInfo err num is invalid")
TRACE_MSG(MMIIDLE_DIAL_5403_112_2_18_2_23_55_28,"AppendMatchingInfo err num is invalid")
TRACE_MSG(MMIIDLE_DIAL_6280_112_2_18_2_23_56_29,"mainapp: CheckSIMADNInput adn_entry_ptr No Memory")
TRACE_MSG(MMIIDLE_DIAL_6293_112_2_18_2_23_56_30,"mainapp: CheckSIMADNInput err_code=%x")
TRACE_MSG(MMIIDLE_DISPLAY_966_112_2_18_2_23_59_31,"GetDisplayRect ERROR! tp_type = %d")
TRACE_MSG(MMIIDLE_DISPLAY_986_112_2_18_2_23_59_32,"SetDisplayRect ERROR! tp_type = %d")
TRACE_MSG(MMIIDLE_DISPLAY_1117_112_2_18_2_23_59_33,"DisplayDate ERROR! data_wstr_len = %d")
TRACE_MSG(MMIIDLE_DISPLAY_1397_112_2_18_2_24_0_34,"GetLineType ERROR! line_index = %d")
TRACE_MSG(MMIIDLE_DISPLAY_1776_112_2_18_2_24_1_35,"DisplayNetname ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_1929_112_2_18_2_24_1_36,"DisplayLineAndSelImg ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_1935_112_2_18_2_24_1_37,"DisplayLineAndSelImg ERROR! PNULL == lcd_dev_ptr")
TRACE_MSG(MMIIDLE_DISPLAY_2005_112_2_18_2_24_1_38,"GetCurItemPos:prompt_pos %d is error!")
TRACE_MSG(MMIIDLE_DISPLAY_2063_112_2_18_2_24_1_39,"DisplaySchedule ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_2135_112_2_18_2_24_1_40,"DisplayCountDown ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_2280_112_2_18_2_24_2_41,"DisplayMp3Name ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_2393_112_2_18_2_24_2_42,"DisplayFmName:fm_status %d is error!")
TRACE_MSG(MMIIDLE_DISPLAY_2411_112_2_18_2_24_2_43,"DisplayFmName ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_2513_112_2_18_2_24_2_44,"DisplayWords ERROR! PNULL == words_ptr")
TRACE_MSG(MMIIDLE_DISPLAY_2538_112_2_18_2_24_2_45,"DisplayWords ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_2628_112_2_18_2_24_2_46,"DisplayDcd ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_2678_112_2_18_2_24_3_47,"Display Wlan ERROR! line_index =%d")
TRACE_MSG(MMIIDLE_DISPLAY_3093_112_2_18_2_24_3_48,"OpenFocusItemWin:focus_line_type %d is error!")
TRACE_MSG(MMIIDLE_DISPLAY_3148_112_2_18_2_24_3_49,"OpenFocusItemWin:focus_line_type %d is error!")
TRACE_MSG(MMIIDLE_DISPLAY_3489_112_2_18_2_24_4_50,"OpenPromptWin:focus_line_type %d is error!")
TRACE_MSG(MMIIDLE_DISPLAY_3588_112_2_18_2_24_4_51,"SmartIdleHandleTpDown:tp_type %d is error!")
TRACE_MSG(MMIIDLE_DISPLAY_3717_112_2_18_2_24_5_52,"SmartIdleHandleTpUp:tp_type %d is error!")
TRACE_MSG(MMIIDLE_DISPLAY_3931_112_2_18_2_24_5_53,"UpdateShortcutOrLine:don't update line_type = %d")
TRACE_MSG(MMIIDLE_FUNC_138_112_2_18_2_24_7_54,"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_ptr")
TRACE_MSG(MMIIDLE_FUNC_164_112_2_18_2_24_7_55,"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_str.wstr_ptr")
TRACE_MSG(MMIIDLE_FUNC_176_112_2_18_2_24_7_56,"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_str.wstr_ptr")
TRACE_MSG(MMIIDLE_FUNC_219_112_2_18_2_24_7_57,"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == event_str_ptr")
TRACE_MSG(MMIIDLE_FUNC_244_112_2_18_2_24_7_58,"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == full_str_ptr")
TRACE_MSG(MMIIDLE_FUNC_265_112_2_18_2_24_7_59,"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_full_event_str.wstr_ptr")
TRACE_MSG(MMIIDLE_FUNC_352_112_2_18_2_24_7_60,"GetIdleTextStyle ERROR! PNULL == style_ptr")
TRACE_MSG(MMIIDLE_FUNC_567_112_2_18_2_24_7_61,"NeedShowIconAccordingToDualSysSetting ERROR! MMI_DUAL_SYS_MAX <= dual_sys,dual_sys = %d")
TRACE_MSG(MMIIDLE_FUNC_592_112_2_18_2_24_8_62,"GetIdleTextStyle ERROR! PNULL == style_ptr")
TRACE_MSG(MMIIDLE_FUNC_785_112_2_18_2_24_8_63,"MMIAPIIDLE_MAIN_SetIdleModeText ERROR! dual_sys =%d, PNULL == stk_idle_str")
TRACE_MSG(MMIIDLE_FUNC_788_112_2_18_2_24_8_64,"MMIAPIIDLE_MAIN_SetIdleModeText dual_sys=%d, len=%d")
TRACE_MSG(MMIIDLE_HWICON_355_112_2_18_2_24_9_65,"Handle_HW_Icon_Exec_Func : s_cur_hw_icon_func = %x")
TRACE_MSG(MMIIDLE_HWICON_372_112_2_18_2_24_9_66,"Handle_HW_Icon_Exec_Func : s_cur_hw_icon_func = %x")
TRACE_MSG(MMIIDLE_HWICON_445_112_2_18_2_24_10_67,"HandleHWIconPBProcessMsg enter")
TRACE_MSG(MMIIDLE_HWICON_453_112_2_18_2_24_10_68,"HandleHWIconPBProcessMsg forcus")
TRACE_MSG(MMIIDLE_HWICON_477_112_2_18_2_24_10_69,"HandleHWIconPromptWinMsg msg_id = %x")
TRACE_MSG(MMIIDLE_STATUSBAR_1174_112_2_18_2_24_13_70,"SetStatusBarInfo MMI_WIN_ICON_BATTERY")
TRACE_MSG(MMIIDLE_STATUSBAR_1179_112_2_18_2_24_13_71,"SetStatusBarInfo MMI_WIN_ICON_BATTERY_AUX")
TRACE_MSG(MMIIDLE_STATUSBAR_1335_112_2_18_2_24_14_72,"MAIN_SetIdleLineState ERROR! dual_sys < MMI_DUAL_SYS_MAX,dual_sys = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_1581_112_2_18_2_24_14_73,"StatusbarGetSysDate ERROR! PNULL == date_wstr")
TRACE_MSG(MMIIDLE_STATUSBAR_1647_112_2_18_2_24_14_74,"MAIN_SetStbDispTime Set Time OK!")
TRACE_MSG(MMIIDLE_STATUSBAR_1715_112_2_18_2_24_14_75,"MAIN_StatusbarGetSysTime ERROR! PNULL == time_wstr")
TRACE_MSG(MMIIDLE_STATUSBAR_1810_112_2_18_2_24_14_76,"SetRoamingInfo is_display=%d, roaming_index=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_1913_112_2_18_2_24_15_77,"GetLineRoadIconId is_display=%d, line2_index=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_1977_112_2_18_2_24_15_78,"GetLineCallForwardIconId is_display=%d, forward_index=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_2045_112_2_18_2_24_15_79,"MAIN_SetIdleRoamingState ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2048_112_2_18_2_24_15_80,"MAIN_SetIdleRoamingState: e_dual_sys=%d, is_roaming = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2371_112_2_18_2_24_16_81,"SetGprsInfo error dual_sys: %d ")
TRACE_MSG(MMIIDLE_STATUSBAR_2387_112_2_18_2_24_16_82,"SetGprsInfo dual_sys:%d type:%d event:%d,network_status_rat:%d")
TRACE_MSG(MMIIDLE_STATUSBAR_2390_112_2_18_2_24_16_83,"SetGprsInfo data error! data_service_type = %d,pdp_event = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2481_112_2_18_2_24_16_84,"MAIN_SetIdleGprsState ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2484_112_2_18_2_24_16_85,"MAIN_SetIdleGprsState: e_dual_sys=%d, gprs_state = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2672_112_2_18_2_24_16_86,"GetActiveIconId data_service_type: %d, index: %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2832_112_2_18_2_24_16_87,"MAIN_SetIdleChargeState batlevel=%d, bat_icon_type=%d, cur_charge_bat=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_2836_112_2_18_2_24_16_88,"MAIN_SetIdleChargeState is_charge = %d,s_idlewin_info.is_charge = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2850_112_2_18_2_24_17_89,"MAIN_SetIdleChargeState %d, %d, %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2892_112_2_18_2_24_17_90,"MAIN_SetIdleChargeState %d, %d, %d, %d")
TRACE_MSG(MMIIDLE_STATUSBAR_2915_112_2_18_2_24_17_91,"MAIN_SetIdleDbatLevel  dbat_info->p_bat[DBAT_MAIN_BAT]=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_2926_112_2_18_2_24_17_92,"MAIN_SetIdleDbatLevel  bat_level=%d,s_idlewin_info.batlevel=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_2945_112_2_18_2_24_17_93,"MAIN_SetIdleDbatLevel  dbat_info->p_bat[DBAT_AUX_BAT]=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_2957_112_2_18_2_24_17_94,"MAIN_SetIdleDbatLevel  bat_level=%d,MMIIDLE_GetIdleWinInfo()->batlevel_aux=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_2975_112_2_18_2_24_17_95,"MAIN_SetIdleDbatLevel  is_update=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_3007_112_2_18_2_24_17_96,"MMIAPIIDLE_SetStbBatteryInfo is_main_battery=%d,cur_use_bat=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_3025_112_2_18_2_24_17_97,"MMIAPIIDLE_SetStbBatteryInfo bat_level=%d")
TRACE_MSG(MMIIDLE_STATUSBAR_3189_112_2_18_2_24_17_98,"MAIN_SetIdleRxLevel ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_3196_112_2_18_2_24_17_99,"MAIN_SetIdleRxLevel %d,%d,%d, %d")
TRACE_MSG(MMIIDLE_STATUSBAR_3389_112_2_18_2_24_18_100,"AddMissCallStbScrollItem call info alloc failed")
TRACE_MSG(MMIIDLE_STATUSBAR_3486_112_2_18_2_24_18_101,"MMIMAIN_AppendStbScrollItem() item_type = %d")
TRACE_MSG(MMIIDLE_STATUSBAR_3597_112_2_18_2_24_18_102,"MAIN_SetIdleBTStateWhenPowerOn %d")
TRACE_MSG(MMIIDLE_STATUSBAR_3612_112_2_18_2_24_18_103,"MAIN_SetIdleBlueToothState %d")
TRACE_MSG(MMIIDLE_STATUSBAR_3759_112_2_18_2_24_18_104,"MAIN_SetIdleBatLevel bat_level = %d,idle level =%d")
TRACE_MSG(MMIIDLE_STATUSBAR_3864_112_2_18_2_24_19_105,"MAIN_SetIdleTdState")
TRACE_MSG(MMIIDLE_STATUSBAR_3877_112_2_18_2_24_19_106,"MAIN_SetIdleUnknownState")
TRACE_MSG(MMIIDLE_STATUSBAR_4024_112_2_18_2_24_19_107,"MAIN_SetIdleShakeState vibrator and silent are true!")
TRACE_MSG(MMIIDLE_STATUSBAR_4043_112_2_18_2_24_19_108,"MAIN_SetIdleSilentState vibrator and silent are true!")
TRACE_MSG(MMIIDLE_STATUSBAR_4140_112_2_18_2_24_19_109,"MMIAPIIDLE_UpdateQQState ")
TRACE_MSG(MMIIDLE_STATUSBAR_4264_112_2_18_2_24_19_110,"MMIAPIIDLE_UpdateAspState ")
END_TRACE_MAP(MMI_APP_IDLE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_IDLE_TRC_H_

