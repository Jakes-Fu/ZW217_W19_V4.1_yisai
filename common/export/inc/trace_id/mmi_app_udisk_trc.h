/******************************************************************************
 ** File Name:      mmi_app_udisk_trc.h                                         *
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
//trace_id:231
#ifndef _MMI_APP_UDISK_TRC_H_
#define _MMI_APP_UDISK_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_SD_128_112_2_18_3_3_59_0 "mmisd:MMIAPISD_Init: register Udisk enter"
#define MMI_SD_134_112_2_18_3_3_59_1 "mmisd:MMIAPISD_Init: register Udisk success"
#define MMI_SD_138_112_2_18_3_3_59_2 "mmisd:MMIAPISD_Init: register Udisk fail"
#define MMI_SD_157_112_2_18_3_3_59_3 "mmisd:MMIAPISD_Init: register sd callback func failed"
#define MMI_SD_196_112_2_18_3_4_0_4 "MMIAPISD_Init scmstatus = %d"
#define MMI_SD_521_112_2_18_3_4_0_5 "MMIAPISD_StopSDCard, enter"
#define MMI_SD_546_112_2_18_3_4_0_6 "MMIAPISD_StopOperaSDPlug"
#define MMI_SD_569_112_2_18_3_4_0_7 "[MMIUDISK]InitFolder invalid param !"
#define MMI_SD_639_112_2_18_3_4_0_8 "mmi_sd.c enter MMIAPISD_InitForFAT()"
#define MMI_SD_804_112_2_18_3_4_1_9 "[MMIUDISK] HandleSDMsg slot_name %d"
#define MMI_SD_808_112_2_18_3_4_1_10 "HandleSDMsg"
#define MMI_SD_815_112_2_18_3_4_1_11 "SCM plug in"
#define MMI_SD_821_112_2_18_3_4_1_12 "SCM plug out"
#define MMI_SD_827_112_2_18_3_4_1_13 "SCM SLOT Error"
#define MMI_SD_PLUG_211_112_2_18_3_4_2_14 "MMIAPISD_HandleSDPlug scmstatus = %d"
#define MMI_SD_PLUG_216_112_2_18_3_4_2_15 "mmi_sd_plug.c: MMIAPISD_HandleSDPlug, plug_msg_id=%d,scmstatus = %d"
#define MMI_SD_PLUG_228_112_2_18_3_4_2_16 "mmi_sd_plug.c: MMIAPISD_HandleSDPlug MSG_KEYDOWN_SDCARD_DETECT, ignore"
#define MMI_SD_PLUG_246_112_2_18_3_4_2_17 "mmi_sd_plug.c: MMIAPISD_HandleSDPlug MSG_KEYUP_SDCARD_DETECT, ignore"
#define MMI_SD_PLUG_327_112_2_18_3_4_2_18 "MMIAPISD_CheckAllDelayedDevice, enter"
#define MMI_SD_PLUG_333_112_2_18_3_4_2_19 "MMIAPISD_CheckAllDelayedDevice,s_delayed_devices_msg[%d] 0x%x"
#define MMI_SD_PLUG_356_112_2_18_3_4_2_20 "mmi_sd_plug.c: HandleSDPlug, plug_msg_id=%d"
#define MMI_SD_PLUG_431_112_2_18_3_4_2_21 "mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_1, scm_type = %d"
#define MMI_SD_PLUG_465_112_2_18_3_4_2_22 "mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_2, scm_type = %d"
#define MMI_SD_PLUG_498_112_2_18_3_4_2_23 "mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_3, scm_type = %d"
#define MMI_SD_PLUG_557_112_2_18_3_4_2_24 "mmi_sd_plug.c: StopCurOperation enter"
#define MMI_SD_PLUG_752_112_2_18_3_4_3_25 "mmi_sd_plug.c: StopCurOperation exit"
#define MMI_SD_PLUG_766_112_2_18_3_4_3_26 "mmi_sd_plug.c: RegisterOneFileSystem enter"
#define MMI_SD_PLUG_781_112_2_18_3_4_3_27 "RegisterOneFileSystem: MMISD_InitSDCard is_success = %d"
#define MMI_SD_PLUG_796_112_2_18_3_4_3_28 "RegisterOneFileSystem: MMIAPISD_StopSDCard is_success = %d"
#define MMI_SD_PLUG_799_112_2_18_3_4_3_29 "mmi_sd_plug.c: RegisterFileSystem exit"
#define MMI_SD_PLUG_815_112_2_18_3_4_3_30 "MMISD_SDPlugInit, is_sd_in = %d"
#define MMIUDISK_240_112_2_18_3_4_4_31 "mmisd:MMIAPISD_SysDeviceinit: register sys device success"
#define MMIUDISK_244_112_2_18_3_4_4_32 "mmisd:MMIAPISD_SysDeviceinit: register sys device fail"
#define MMIUDISK_279_112_2_18_3_4_4_33 "mmiudisk:MMIAPIUdisk_HandlePlugIn:restart_condition=%d"
#define MMIUDISK_511_112_2_18_3_4_5_34 "MMIAPIUdisk_HandlePlugOut s_is_power_on = %d"
#define MMIUDISK_555_112_2_18_3_4_5_35 "mmiudisk:MMIUDISK_InitFFS_MMI: format udisk success"
#define MMIUDISK_559_112_2_18_3_4_5_36 "mmiudisk:MMIUDISK_InitFFS_MMI: format udisk fail"
#define MMIUDISK_620_112_2_18_3_4_5_37 "mmiudisk:mmiudisk.c:MMIUDISK_SetUcomMode type=%d"
#define MMIUDISK_657_112_2_18_3_4_5_38 "MMIUDISK_SetUcomMode ucom_type = %d"
#define MMIUDISK_863_112_2_18_3_4_5_39 "mmiudisk:MMIAPIUDISK_UdiskIsRun s_udisk_is_run=%d"
#define MMIUDISK_996_112_2_18_3_4_6_40 "mmiudisk:MMIAPIUdisk_HandleFormatCnf:result is %d"
#define MMIUDISK_1068_112_2_18_3_4_6_41 "[mmiudisk MMIUDISK_SetPCCameraSensorID ret=%d, sensor_id=%d"
#define MMIUDISK_1093_112_2_18_3_4_6_42 "mmiudisk MMIUDISK_GetSensorNumber ret=%d, sensor_number=%d"
#define MMIUDISK_1114_112_2_18_3_4_6_43 "mmiudisk MMIUDISK_GetCurSensorID ret=%d, sensor_id=%d"
#define MMIUDISK_1130_112_2_18_3_4_6_44 "mmiudisk:UPCC_OpenPCCamera"
#define MMIUDISK_1194_112_2_18_3_4_6_45 "MMIAPISD_GetDeviceStatus sd  exist %d"
#define MMIUDISK_1518_112_2_18_3_4_7_46 "MMI device total_low =%d,used_low =%d, free_low =%d total_space_high=%d"
#define MMIUDISK_1571_112_2_18_3_4_7_47 "[MMIUDISK]MMIUDISK_GetFileSizeString str_ptr 0x%x wstr_len %d"
#define MMIUDISK_1577_112_2_18_3_4_7_48 "MMIUDISK_GetFileSizeString low_size =%d,hight_size =%d,tb_size =%d"
#define MMIUDISK_1615_112_2_18_3_4_7_49 "mmiudisk:StopUdisk: register SD success"
#define MMIUDISK_1626_112_2_18_3_4_7_50 "mmiudisk:StopUdisk: register SD fail"
#define MMIUDISK_SIMU_102_112_2_18_3_4_9_51 "HandlePsAndRefMsg:receive UPM_PLUGIN_IND"
#define MMIUDISK_SIMU_108_112_2_18_3_4_9_52 "HandlePsAndRefMsg:receive UPM_PLUGOUT_IND"
#define MMIUDISK_SIMU_137_112_2_18_3_4_9_53 "HandlePsAndRefMsg:receive UPM_PLUGIN_IND"
#define MMIUDISK_SIMU_143_112_2_18_3_4_9_54 "HandlePsAndRefMsg:receive UPM_PLUGOUT_IND"
#define MMIUDISK_WINTAB_296_112_2_18_3_4_9_55 "[mmiudisk]: MMIUDISK_StartUPCC"
#define MMIUDISK_WINTAB_393_112_2_18_3_4_10_56 "UART selected SIO_UserPortBusy"
#define MMIUDISK_WINTAB_1017_112_2_18_3_4_11_57 "[mmiudisk]: HandleUPCCUsingWindow MSG_OPEN_WINDOW"
#define MMIUDISK_WINTAB_1026_112_2_18_3_4_11_58 "[mmiudisk]: HandleUPCCUsingWindow MSG_FULL_PAINT"
#define MMIUDISK_WINTAB_1046_112_2_18_3_4_11_59 "[mmiudisk]: HandleUPCCUsingWindow MMIUDISK_OpenPCCamera sucess"
#define MMIUDISK_WINTAB_1079_112_2_18_3_4_11_60 "[mmiudisk]: HandleUPCCUsingWindow MSG_LOSE_FOCUS"
#define MMIUDISK_WINTAB_1269_112_2_18_3_4_11_61 "[mmiudisk]: MMIUDISK_StartUPCC"
#define MMIUDISK_WINTAB_1386_112_2_18_3_4_12_62 "mmiudisk HandleUPCCSensorWinMsg openpccamera ret=%d"
#define MMIUDISK_WINTAB_1458_112_2_18_3_4_12_63 "mmiudisk HandleUPCCSensorWinMsg MSG_CLOSE_WINDOW upcc_err=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_UDISK_TRC)
TRACE_MSG(MMI_SD_128_112_2_18_3_3_59_0,"mmisd:MMIAPISD_Init: register Udisk enter")
TRACE_MSG(MMI_SD_134_112_2_18_3_3_59_1,"mmisd:MMIAPISD_Init: register Udisk success")
TRACE_MSG(MMI_SD_138_112_2_18_3_3_59_2,"mmisd:MMIAPISD_Init: register Udisk fail")
TRACE_MSG(MMI_SD_157_112_2_18_3_3_59_3,"mmisd:MMIAPISD_Init: register sd callback func failed")
TRACE_MSG(MMI_SD_196_112_2_18_3_4_0_4,"MMIAPISD_Init scmstatus = %d")
TRACE_MSG(MMI_SD_521_112_2_18_3_4_0_5,"MMIAPISD_StopSDCard, enter")
TRACE_MSG(MMI_SD_546_112_2_18_3_4_0_6,"MMIAPISD_StopOperaSDPlug")
TRACE_MSG(MMI_SD_569_112_2_18_3_4_0_7,"[MMIUDISK]InitFolder invalid param !")
TRACE_MSG(MMI_SD_639_112_2_18_3_4_0_8,"mmi_sd.c enter MMIAPISD_InitForFAT()")
TRACE_MSG(MMI_SD_804_112_2_18_3_4_1_9,"[MMIUDISK] HandleSDMsg slot_name %d")
TRACE_MSG(MMI_SD_808_112_2_18_3_4_1_10,"HandleSDMsg")
TRACE_MSG(MMI_SD_815_112_2_18_3_4_1_11,"SCM plug in")
TRACE_MSG(MMI_SD_821_112_2_18_3_4_1_12,"SCM plug out")
TRACE_MSG(MMI_SD_827_112_2_18_3_4_1_13,"SCM SLOT Error")
TRACE_MSG(MMI_SD_PLUG_211_112_2_18_3_4_2_14,"MMIAPISD_HandleSDPlug scmstatus = %d")
TRACE_MSG(MMI_SD_PLUG_216_112_2_18_3_4_2_15,"mmi_sd_plug.c: MMIAPISD_HandleSDPlug, plug_msg_id=%d,scmstatus = %d")
TRACE_MSG(MMI_SD_PLUG_228_112_2_18_3_4_2_16,"mmi_sd_plug.c: MMIAPISD_HandleSDPlug MSG_KEYDOWN_SDCARD_DETECT, ignore")
TRACE_MSG(MMI_SD_PLUG_246_112_2_18_3_4_2_17,"mmi_sd_plug.c: MMIAPISD_HandleSDPlug MSG_KEYUP_SDCARD_DETECT, ignore")
TRACE_MSG(MMI_SD_PLUG_327_112_2_18_3_4_2_18,"MMIAPISD_CheckAllDelayedDevice, enter")
TRACE_MSG(MMI_SD_PLUG_333_112_2_18_3_4_2_19,"MMIAPISD_CheckAllDelayedDevice,s_delayed_devices_msg[%d] 0x%x")
TRACE_MSG(MMI_SD_PLUG_356_112_2_18_3_4_2_20,"mmi_sd_plug.c: HandleSDPlug, plug_msg_id=%d")
TRACE_MSG(MMI_SD_PLUG_431_112_2_18_3_4_2_21,"mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_1, scm_type = %d")
TRACE_MSG(MMI_SD_PLUG_465_112_2_18_3_4_2_22,"mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_2, scm_type = %d")
TRACE_MSG(MMI_SD_PLUG_498_112_2_18_3_4_2_23,"mmi_sd_plug.c: HandleSDPlugWaiting: received MSG_HANDLE_SDPLUG_STEP_3, scm_type = %d")
TRACE_MSG(MMI_SD_PLUG_557_112_2_18_3_4_2_24,"mmi_sd_plug.c: StopCurOperation enter")
TRACE_MSG(MMI_SD_PLUG_752_112_2_18_3_4_3_25,"mmi_sd_plug.c: StopCurOperation exit")
TRACE_MSG(MMI_SD_PLUG_766_112_2_18_3_4_3_26,"mmi_sd_plug.c: RegisterOneFileSystem enter")
TRACE_MSG(MMI_SD_PLUG_781_112_2_18_3_4_3_27,"RegisterOneFileSystem: MMISD_InitSDCard is_success = %d")
TRACE_MSG(MMI_SD_PLUG_796_112_2_18_3_4_3_28,"RegisterOneFileSystem: MMIAPISD_StopSDCard is_success = %d")
TRACE_MSG(MMI_SD_PLUG_799_112_2_18_3_4_3_29,"mmi_sd_plug.c: RegisterFileSystem exit")
TRACE_MSG(MMI_SD_PLUG_815_112_2_18_3_4_3_30,"MMISD_SDPlugInit, is_sd_in = %d")
TRACE_MSG(MMIUDISK_240_112_2_18_3_4_4_31,"mmisd:MMIAPISD_SysDeviceinit: register sys device success")
TRACE_MSG(MMIUDISK_244_112_2_18_3_4_4_32,"mmisd:MMIAPISD_SysDeviceinit: register sys device fail")
TRACE_MSG(MMIUDISK_279_112_2_18_3_4_4_33,"mmiudisk:MMIAPIUdisk_HandlePlugIn:restart_condition=%d")
TRACE_MSG(MMIUDISK_511_112_2_18_3_4_5_34,"MMIAPIUdisk_HandlePlugOut s_is_power_on = %d")
TRACE_MSG(MMIUDISK_555_112_2_18_3_4_5_35,"mmiudisk:MMIUDISK_InitFFS_MMI: format udisk success")
TRACE_MSG(MMIUDISK_559_112_2_18_3_4_5_36,"mmiudisk:MMIUDISK_InitFFS_MMI: format udisk fail")
TRACE_MSG(MMIUDISK_620_112_2_18_3_4_5_37,"mmiudisk:mmiudisk.c:MMIUDISK_SetUcomMode type=%d")
TRACE_MSG(MMIUDISK_657_112_2_18_3_4_5_38,"MMIUDISK_SetUcomMode ucom_type = %d")
TRACE_MSG(MMIUDISK_863_112_2_18_3_4_5_39,"mmiudisk:MMIAPIUDISK_UdiskIsRun s_udisk_is_run=%d")
TRACE_MSG(MMIUDISK_996_112_2_18_3_4_6_40,"mmiudisk:MMIAPIUdisk_HandleFormatCnf:result is %d")
TRACE_MSG(MMIUDISK_1068_112_2_18_3_4_6_41,"[mmiudisk MMIUDISK_SetPCCameraSensorID ret=%d, sensor_id=%d")
TRACE_MSG(MMIUDISK_1093_112_2_18_3_4_6_42,"mmiudisk MMIUDISK_GetSensorNumber ret=%d, sensor_number=%d")
TRACE_MSG(MMIUDISK_1114_112_2_18_3_4_6_43,"mmiudisk MMIUDISK_GetCurSensorID ret=%d, sensor_id=%d")
TRACE_MSG(MMIUDISK_1130_112_2_18_3_4_6_44,"mmiudisk:UPCC_OpenPCCamera")
TRACE_MSG(MMIUDISK_1194_112_2_18_3_4_6_45,"MMIAPISD_GetDeviceStatus sd  exist %d")
TRACE_MSG(MMIUDISK_1518_112_2_18_3_4_7_46,"MMI device total_low =%d,used_low =%d, free_low =%d total_space_high=%d")
TRACE_MSG(MMIUDISK_1571_112_2_18_3_4_7_47,"[MMIUDISK]MMIUDISK_GetFileSizeString str_ptr 0x%x wstr_len %d")
TRACE_MSG(MMIUDISK_1577_112_2_18_3_4_7_48,"MMIUDISK_GetFileSizeString low_size =%d,hight_size =%d,tb_size =%d")
TRACE_MSG(MMIUDISK_1615_112_2_18_3_4_7_49,"mmiudisk:StopUdisk: register SD success")
TRACE_MSG(MMIUDISK_1626_112_2_18_3_4_7_50,"mmiudisk:StopUdisk: register SD fail")
TRACE_MSG(MMIUDISK_SIMU_102_112_2_18_3_4_9_51,"HandlePsAndRefMsg:receive UPM_PLUGIN_IND")
TRACE_MSG(MMIUDISK_SIMU_108_112_2_18_3_4_9_52,"HandlePsAndRefMsg:receive UPM_PLUGOUT_IND")
TRACE_MSG(MMIUDISK_SIMU_137_112_2_18_3_4_9_53,"HandlePsAndRefMsg:receive UPM_PLUGIN_IND")
TRACE_MSG(MMIUDISK_SIMU_143_112_2_18_3_4_9_54,"HandlePsAndRefMsg:receive UPM_PLUGOUT_IND")
TRACE_MSG(MMIUDISK_WINTAB_296_112_2_18_3_4_9_55,"[mmiudisk]: MMIUDISK_StartUPCC")
TRACE_MSG(MMIUDISK_WINTAB_393_112_2_18_3_4_10_56,"UART selected SIO_UserPortBusy")
TRACE_MSG(MMIUDISK_WINTAB_1017_112_2_18_3_4_11_57,"[mmiudisk]: HandleUPCCUsingWindow MSG_OPEN_WINDOW")
TRACE_MSG(MMIUDISK_WINTAB_1026_112_2_18_3_4_11_58,"[mmiudisk]: HandleUPCCUsingWindow MSG_FULL_PAINT")
TRACE_MSG(MMIUDISK_WINTAB_1046_112_2_18_3_4_11_59,"[mmiudisk]: HandleUPCCUsingWindow MMIUDISK_OpenPCCamera sucess")
TRACE_MSG(MMIUDISK_WINTAB_1079_112_2_18_3_4_11_60,"[mmiudisk]: HandleUPCCUsingWindow MSG_LOSE_FOCUS")
TRACE_MSG(MMIUDISK_WINTAB_1269_112_2_18_3_4_11_61,"[mmiudisk]: MMIUDISK_StartUPCC")
TRACE_MSG(MMIUDISK_WINTAB_1386_112_2_18_3_4_12_62,"mmiudisk HandleUPCCSensorWinMsg openpccamera ret=%d")
TRACE_MSG(MMIUDISK_WINTAB_1458_112_2_18_3_4_12_63,"mmiudisk HandleUPCCSensorWinMsg MSG_CLOSE_WINDOW upcc_err=%d")
END_TRACE_MAP(MMI_APP_UDISK_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_UDISK_TRC_H_

