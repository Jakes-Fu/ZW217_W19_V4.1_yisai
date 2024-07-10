/******************************************************************************
 ** File Name:      mmi_app_accessory_trc.h                                         *
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
//trace_id:147
#ifndef _MMI_APP_ACCESSORY_TRC_H_
#define _MMI_APP_ACCESSORY_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_SOLARLUNAR_551_112_2_18_2_0_45_0 "MMICALENDAR_GetMonthday error month=%d"
#define MMI_SOLARLUNAR_593_112_2_18_2_0_45_1 "MMICALENDAR_GetFirstday:solar_month %d is error!"
#define MMI_SOLARLUNAR_624_112_2_18_2_0_45_2 "CalcDaysBetweenTwoDate: start_year,end_year = %d.%d"
#define MMI_SOLARLUNAR_648_112_2_18_2_0_45_3 "CalcDaysBetweenTwoDate error end_month=%d"
#define MMI_SOLARLUNAR_657_112_2_18_2_0_45_4 "CalcDaysBetweenTwoDate error start_month=%d"
#define MMI_SOLARLUNAR_764_112_2_18_2_0_46_5 "MMICALENDAR_IsDateValid: month is %d"
#define MMI_SOLARLUNAR_834_112_2_18_2_0_46_6 "MMICALENDAR_IsQueryDateValid: month is %d"
#define MMI_SOLARLUNAR_908_112_2_18_2_0_46_7 "MMICALENDAR_MoveDaysForward: PNULL == to_year_ptr or to_month_ptr or to_day_ptr!"
#define MMI_SOLARLUNAR_914_112_2_18_2_0_46_8 "MMICALENDAR_MoveDaysForward: cur_year = %d"
#define MMI_SOLARLUNAR_1144_112_2_18_2_0_46_9 "MMI_SOLARLUNAR.C MMICALENDAR_SolarToLunar *year=%x, *moth=%x, *day=%x"
#define MMI_SOLARLUNAR_1245_112_2_18_2_0_47_10 "MMI_SOLARLUNAR.C MMICALENDAR_IsLunarFeast *feast_index_ptr=%x"
#define MMI_SOLARLUNAR_1251_112_2_18_2_0_47_11 "MMICALENDAR_IsLunarFeast error offset=%d"
#define MMIALARM_1449_112_2_18_2_0_52_12 "MMIALM_GeEvent: the event_id is %d"
#define MMIALARM_1509_112_2_18_2_0_52_13 "ModifyEvent: the event_id is %d"
#define MMIALARM_1560_112_2_18_2_0_53_14 "MMIALM_AddOneEvent NULL == event_ptr!"
#define MMIALARM_1563_112_2_18_2_0_53_15 "MMIALM_AddOneEvent enter!"
#define MMIALARM_1742_112_2_18_2_0_53_16 "MMIALM_DelEvents start_event > end_event!"
#define MMIALARM_1801_112_2_18_2_0_53_17 "MMIALM_UpdateEvents enter!"
#define MMIALARM_1826_112_2_18_2_0_53_18 "MMIALM_UpdateEvents enter! i=%d"
#define MMIALARM_2186_112_2_18_2_0_54_19 "MMIALM_GetArrivedEvent PNULL == event_ptr || PNULL == event_id"
#define MMIALARM_2225_112_2_18_2_0_54_20 "mmialarm.c MMIALM_GetArrivedEvent when poweroff"
#define MMIALARM_2512_112_2_18_2_0_55_21 "SetNextAlarmTimeByCurrent: event_ptr->fre is %d"
#define MMIALARM_2795_112_2_18_2_0_55_22 "SetNextAlarmTimeByCurrent(), success, year = %d, month = %d, day = %d, hour = %d, minute = %d"
#define MMIALARM_2799_112_2_18_2_0_55_23 "SetNextAlarmTimeByCurrent(), fail"
#define MMIALARM_3002_112_2_18_2_0_56_24 "IsMatchEvent:NULL == event_ptr"
#define MMIALARM_3181_112_2_18_2_0_56_25 "IncreaseDateByOne error! mon=%d"
#define MMIALARM_3264_112_2_18_2_0_56_26 "[StopClockTimer]: StopTimer: the timer has stopped!"
#define MMIALARM_3609_112_2_18_2_0_57_27 " HandleEventAliveWinMsg, msg_id = %x"
#define MMIALARM_4024_112_2_18_2_0_58_28 "mmialarm.c: ring type = %d"
#define MMIALARM_4141_112_2_18_2_0_58_29 "mmialarm.c MMIAPIFM_PlayInputFreq"
#define MMIALARM_4246_112_2_18_2_0_58_30 "mmialarm.c MMIAPIFM_PlayInputFreq"
#define MMIALARM_4287_112_2_18_2_0_58_31 "mmialarm.c HandleAlarmExpiredWinMsg() msg_id = %x"
#define MMIALARM_4751_112_2_18_2_0_59_32 "MMIAPIALM_StartAlarm, find event result = %d"
#define MMIALARM_4773_112_2_18_2_0_59_33 "MMIAPIALM_StartAlarm, delay alarm win pop"
#define MMIALARM_4790_112_2_18_2_0_59_34 "MMIAPIALM_StartAlarm, open alarm win at once"
#define MMIALARM_4797_112_2_18_2_0_59_35 "MMIAPIALM_StartAlarm, the alarm is not exist!"
#define MMIALARM_4823_112_2_18_2_1_0_36 "mmialarm.c: OpenAlarmExpiredWin(), type = %d"
#define MMIALARM_4924_112_2_18_2_1_0_37 "MMIAPIALM_CheckAndStartDelayedAlarm(), is_delayed = %d, is_on = %d"
#define MMIALARM_5540_112_2_18_2_1_1_38 "mmialarm.c: HandleAutoPowerOffExpired() MSG_CTL_OK "
#define MMIALARM_6419_112_2_18_2_1_3_39 "OpenAlarmAliveWin error: Invalid s_arrived_event.type  is %d:"
#define MMIALARM_6491_112_2_18_2_1_3_40 "SOGOU Update event coming"
#define MMIALARM_6565_112_2_18_2_1_3_41 "HandlePastEventWinMsg, msg_id = %x"
#define MMIALARM_6792_112_2_18_2_1_4_42 "OpenPastSchEventWin error: Invalid past_event->type is %d:"
#define MMIALARM_6923_112_2_18_2_1_4_43 "mmialarm.c MMIAPIALM_GetFirstAlarmRing ReadRingInfoFileName fail!"
#define MMIALARM_8646_112_2_18_2_1_8_44 "%s  %d  Event %d is set to on!"
#define MMIALARM_9249_112_2_18_2_1_9_45 "MMIAPIALARM_GetEventTimeString:PNULL == str_ptr || ALM_EVENT_TIME_STR_LEN > str_len"
#define MMIALARM_9419_112_2_18_2_1_9_46 "MMIAPIALARM_SetAlarmRing:alarm_id > EVENT_ALARM_MAX || PNULL == full_path_ptr || 0 == full_path_len!"
#define MMIALARM_9527_112_2_18_2_1_10_47 "InitAlarmFolder result %d"
#define MMIALARM_9580_112_2_18_2_1_10_48 "mmialarm.c GetRingInfoFileName MMIAPIFMM_CombineFullPath fail!"
#define MMIALARM_9591_112_2_18_2_1_10_49 "GetRingInfoFileName event_id %d, ret %d"
#define MMIALARM_9630_112_2_18_2_1_10_50 "MMIALM_SaveRingIDInfoToFile file_ret %d"
#define MMIALARM_9676_112_2_18_2_1_10_51 "ReadRingInfoFileName file_ret %d"
#define MMIALARM_9704_112_2_18_2_1_10_52 "DeleteRingInfoFile file_ret %d"
#define MMIALARM_9834_112_2_18_2_1_10_53 "[MMIALM] MMIALM_Interrupt s_alarm_is_interrupted %d"
#define MMIALARM_9843_112_2_18_2_1_10_54 "[MMIALM] MMIALM_GetAlmInterruptFlag s_alarm_is_interrupted %d"
#define MMIALARM_9961_112_2_18_2_1_11_55 "mmialarm.c: HandlePastAutoPowerOn() MSG_CTL_OK "
#define MMIALARM_10395_112_2_18_2_1_11_56 "[MMIFM] ConvertFloatStrToInt ret %d"
#define MMIALARM_CLOCK_568_112_2_18_2_1_15_57 "aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
#define MMIALARM_CLOCK_720_112_2_18_2_1_15_58 "aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
#define MMIALARM_CLOCK_1150_112_2_18_2_1_16_59 "DisplayAlmClock rect = %d,%d,%d,%d"
#define MMIALARM_CLOCK_1155_112_2_18_2_1_16_60 "MMIWIDGET_ACLOCK DisplayAClock lcd_id = %d"
#define MMIALARM_JUMP_CLOCK_331_112_2_18_2_1_18_61 "aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
#define MMIALARM_JUMP_CLOCK_483_112_2_18_2_1_18_62 "aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
#define MMIALARM_SERVICE_462_112_2_18_2_1_21_63 "StartAlmService:year=%d,mon=%d,mday=%d,hour=%d,min=%d"
#define MMIALARM_SERVICE_472_112_2_18_2_1_21_64 "StartAlmService:while"
#define MMIALARM_SERVICE_476_112_2_18_2_1_21_65 "StartAlmService:alarm callback"
#define MMIALARM_SERVICE_566_112_2_18_2_1_21_66 "SetAlmService:cur_time:year=%d,mon=%d,mday=%d,hour=%d,min=%d"
#define MMIALARM_SERVICE_585_112_2_18_2_1_21_67 "SetAlmService:latest_event_time:year=%d,mon=%d,mday=%d,hour=%d,min=%d"
#define MMIALARM_SERVICE_593_112_2_18_2_1_21_68 "[MMIALARM SERVICE]SetAlmService enter!"
#define MMIALARM_SERVICE_597_112_2_18_2_1_21_69 "SetAlmService: ERR_TM_NONE!"
#define MMICALC_MAIN_784_112_2_18_2_1_23_70 "SetCalcTextStyle:PNULL == style_ptr"
#define MMICALC_MAIN_854_112_2_18_2_1_24_71 "ProcessKeyPressed:PNULL == buf || max_len <= 0 !"
#define MMICALC_MAIN_2991_112_2_18_2_1_28_72 "DigitLineEditor:PNULL == buf!"
#define MMICALC_MAIN_3688_112_2_18_2_1_30_73 "wrong operator type"
#define MMICALENDAR_MAIN_5027_112_2_18_2_1_41_74 "[MMIALM]HandleWaitWinMsg red key pressed"
#define MMICALENDAR_MAIN_5761_112_2_18_2_1_43_75 "UpdateLayer:error!"
#define MMICOUNTEDTIME_MAIN_771_112_2_18_2_1_48_76 "ST_DisplayCurTime: the rect is null"
#define MMICOUNTEDTIME_MAIN_785_112_2_18_2_1_48_77 "ST_DisplayCurTime: the rect is invalid"
#define MMICOUNTEDTIME_MAIN_825_112_2_18_2_1_48_78 "MMIAPICT_HandleTimer timer_id = %d"
#define MMICOUNTEDTIME_MAIN_883_112_2_18_2_1_48_79 "MMIAPICT_HandleTimer s_ct_vibrator_time_id in"
#define MMICOUNTEDTIME_MAIN_983_112_2_18_2_1_49_80 "Mmicountedtime_main.c, delay ct arrive win pop"
#define MMICOUNTEDTIME_MAIN_998_112_2_18_2_1_49_81 "Mmicountedtime_main.c, open ct arrive win at once"
#define MMICOUNTEDTIME_MAIN_1081_112_2_18_2_1_49_82 "MMIAPICT_HandleCountedTimeArriveWin: msg_id = %d"
#define MMIPHONESEARCH_WINTAB_1972_112_2_18_2_1_55_83 "HandlePHONESEARCHDownloadCB enter"
#define MMISCHEDULE_6723_112_2_18_2_2_10_84 "MMIAPISCH_IsEventToday s_user_list.total = %d"
#define MMISCHEDULE_6779_112_2_18_2_2_10_85 "AddOneDetailItem fail "
#define MMISCHEDULE_7743_112_2_18_2_2_12_86 "MMIAPISCH_ReadVCalendarData"
#define MMISCHEDULE_7746_112_2_18_2_2_12_87 "MMIAPISCH_ReadVCalendarData PNULL == data_ptr"
#define MMISCHEDULE_7750_112_2_18_2_2_12_88 "MMIAPISCH_ReadVCalendarData: start parser"
#define MMISCHEDULE_7797_112_2_18_2_2_12_89 "mmivcalendar: MMIAPISCH_ReadVcalendarFile"
#define MMISCHEDULE_7800_112_2_18_2_2_12_90 "mmivcalendar MMIAPISCH_ReadVcalendarFile PNULL == name_ptr"
#define MMISCHEDULE_7810_112_2_18_2_2_12_91 "MMIAPISCH_ReadVcalendarFile: start parser"
#define MMISCHEDULE_8344_112_2_18_2_2_13_92 "VcalendarFolderSelectionCallBack, file_num = %d"
#define MMISCHEDULE_8514_112_2_18_2_2_13_93 "SaveOneVCalendar is_success %d"
#define MMISCHEDULE_8839_112_2_18_2_2_14_94 "HandleLoadVCalendarWaitWinMsg red key pressed"
#define MMISCHEDULE_8904_112_2_18_2_2_15_95 "MMIAPISCH_OpenVCalendarFile"
#define MMISCHEDULE_8908_112_2_18_2_2_15_96 "MMIAPISCH_OpenVCalendarFile NULL == name_ptr"
#define MMISCHEDULE_8979_112_2_18_2_2_15_97 "MSG_VCALENDAR_SAVE_SCH_CNF is_save_success %d param %d "
#define MMISCHEDULE_ATC_160_112_2_18_2_2_17_98 "MMIMEMO_SyncAdd: memo_item_info_ptr->memo_mode is %d"
#define MMISCHEDULE_ATC_321_112_2_18_2_2_18_99 "MMIMEMO_SyncModify: memo_item_info_ptr->memo_mode is %d"
#define MMISCHEDULE_ATC_447_112_2_18_2_2_18_100 "MMIMEMO_SyncRead: event_ptr->fre_mode is %d"
#define MMISTOPWATCH_MAIN_692_112_2_18_2_2_19_101 "ST_DisplayCurTime: the rect is invalid"
#define MMISTOPWATCH_MAIN_770_112_2_18_2_2_20_102 "MMIAPIST_APIST_HandleTimer timer_id = %d"
#define MMIUNITCONVERSION_1477_112_2_18_2_2_24_103 "StrToInt32:num=[%d],expand=[%d]"
#define MMIUNITCONVERSION_1567_112_2_18_2_2_24_104 "UC:pre_validChar=[%d]"
#define MMIUNITCONVERSION_1568_112_2_18_2_2_24_105 "UC:validChar=[%d]"
#define MMIUNITCONVERSION_1615_112_2_18_2_2_24_106 "UC:lastchar=[%d]"
#define MMIUNITCONVERSION_1958_112_2_18_2_2_25_107 "UC:result_interger.num=[%d]"
#define MMIUNITCONVERSION_1959_112_2_18_2_2_25_108 "UC:result_decimal.num=[%d]"
#define MMIUNITCONVERSION_1960_112_2_18_2_2_25_109 "UC:result_decimal.expand=[%d]"
#define MMIWCLK_MAIN_721_112_2_18_2_2_28_110 "error: DrawAClock target_buf_ptr alloc failed,  return directly"
#define MMIWCLK_MAIN_765_112_2_18_2_2_28_111 "error: foreground image rect is not in background image rect, return directly"
#define MMIWCLK_MAIN_1683_112_2_18_2_2_30_112 "HandleWclkAddCityWinMsg, msg_id = 0x%02x"
#define MMIWCLK_MAIN_2642_112_2_18_2_2_32_113 "MMIAUT::MMIAPIWCLK_SetTimeZone mcc= %d,time_zone = %d, current_year =%d, dst_value=%d"
#define MMIWCLK_MAIN_2656_112_2_18_2_2_32_114 "MMIAUT::MMIAPIWCLK_SetTimeZone city_index1=%d"
#define MMIWCLK_MAIN_2672_112_2_18_2_2_32_115 "MMIAUT::MMIAPIWCLK_SetTimeZone city_index2=%d"
#define MMIWCLK_MAIN_2692_112_2_18_2_2_32_116 "MMIAUT::MMIAPIWCLK_SetTimeZone city_index3=%d"
#define MMIWCLK_MAIN_2726_112_2_18_2_2_32_117 "MMIAUT::MMIAPIWCLK_SetTimeZone daylight_start_datetime=%d:%d:%d,%d-%d-%d"
#define MMIWCLK_MAIN_2729_112_2_18_2_2_32_118 "MMIAUT::MMIAPIWCLK_SetTimeZone daylight_end_datetime=%d:%d:%d,%d-%d-%d"
#define MMIWCLK_MAIN_2773_112_2_18_2_2_32_119 "MMIAUT::MMIAPIWCLK_SetTimeZone sys_second = %d,dst_start = %d,dst_end= %d"
#define MMISCHEDULE_6335_112_2_18_2_2_32_120 "SaveEditSch"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_ACCESSORY_TRC)
TRACE_MSG(MMI_SOLARLUNAR_551_112_2_18_2_0_45_0,"MMICALENDAR_GetMonthday error month=%d")
TRACE_MSG(MMI_SOLARLUNAR_593_112_2_18_2_0_45_1,"MMICALENDAR_GetFirstday:solar_month %d is error!")
TRACE_MSG(MMI_SOLARLUNAR_624_112_2_18_2_0_45_2,"CalcDaysBetweenTwoDate: start_year,end_year = %d.%d")
TRACE_MSG(MMI_SOLARLUNAR_648_112_2_18_2_0_45_3,"CalcDaysBetweenTwoDate error end_month=%d")
TRACE_MSG(MMI_SOLARLUNAR_657_112_2_18_2_0_45_4,"CalcDaysBetweenTwoDate error start_month=%d")
TRACE_MSG(MMI_SOLARLUNAR_764_112_2_18_2_0_46_5,"MMICALENDAR_IsDateValid: month is %d")
TRACE_MSG(MMI_SOLARLUNAR_834_112_2_18_2_0_46_6,"MMICALENDAR_IsQueryDateValid: month is %d")
TRACE_MSG(MMI_SOLARLUNAR_908_112_2_18_2_0_46_7,"MMICALENDAR_MoveDaysForward: PNULL == to_year_ptr or to_month_ptr or to_day_ptr!")
TRACE_MSG(MMI_SOLARLUNAR_914_112_2_18_2_0_46_8,"MMICALENDAR_MoveDaysForward: cur_year = %d")
TRACE_MSG(MMI_SOLARLUNAR_1144_112_2_18_2_0_46_9,"MMI_SOLARLUNAR.C MMICALENDAR_SolarToLunar *year=%x, *moth=%x, *day=%x")
TRACE_MSG(MMI_SOLARLUNAR_1245_112_2_18_2_0_47_10,"MMI_SOLARLUNAR.C MMICALENDAR_IsLunarFeast *feast_index_ptr=%x")
TRACE_MSG(MMI_SOLARLUNAR_1251_112_2_18_2_0_47_11,"MMICALENDAR_IsLunarFeast error offset=%d")
TRACE_MSG(MMIALARM_1449_112_2_18_2_0_52_12,"MMIALM_GeEvent: the event_id is %d")
TRACE_MSG(MMIALARM_1509_112_2_18_2_0_52_13,"ModifyEvent: the event_id is %d")
TRACE_MSG(MMIALARM_1560_112_2_18_2_0_53_14,"MMIALM_AddOneEvent NULL == event_ptr!")
TRACE_MSG(MMIALARM_1563_112_2_18_2_0_53_15,"MMIALM_AddOneEvent enter!")
TRACE_MSG(MMIALARM_1742_112_2_18_2_0_53_16,"MMIALM_DelEvents start_event > end_event!")
TRACE_MSG(MMIALARM_1801_112_2_18_2_0_53_17,"MMIALM_UpdateEvents enter!")
TRACE_MSG(MMIALARM_1826_112_2_18_2_0_53_18,"MMIALM_UpdateEvents enter! i=%d")
TRACE_MSG(MMIALARM_2186_112_2_18_2_0_54_19,"MMIALM_GetArrivedEvent PNULL == event_ptr || PNULL == event_id")
TRACE_MSG(MMIALARM_2225_112_2_18_2_0_54_20,"mmialarm.c MMIALM_GetArrivedEvent when poweroff")
TRACE_MSG(MMIALARM_2512_112_2_18_2_0_55_21,"SetNextAlarmTimeByCurrent: event_ptr->fre is %d")
TRACE_MSG(MMIALARM_2795_112_2_18_2_0_55_22,"SetNextAlarmTimeByCurrent(), success, year = %d, month = %d, day = %d, hour = %d, minute = %d")
TRACE_MSG(MMIALARM_2799_112_2_18_2_0_55_23,"SetNextAlarmTimeByCurrent(), fail")
TRACE_MSG(MMIALARM_3002_112_2_18_2_0_56_24,"IsMatchEvent:NULL == event_ptr")
TRACE_MSG(MMIALARM_3181_112_2_18_2_0_56_25,"IncreaseDateByOne error! mon=%d")
TRACE_MSG(MMIALARM_3264_112_2_18_2_0_56_26,"[StopClockTimer]: StopTimer: the timer has stopped!")
TRACE_MSG(MMIALARM_3609_112_2_18_2_0_57_27," HandleEventAliveWinMsg, msg_id = %x")
TRACE_MSG(MMIALARM_4024_112_2_18_2_0_58_28,"mmialarm.c: ring type = %d")
TRACE_MSG(MMIALARM_4141_112_2_18_2_0_58_29,"mmialarm.c MMIAPIFM_PlayInputFreq")
TRACE_MSG(MMIALARM_4246_112_2_18_2_0_58_30,"mmialarm.c MMIAPIFM_PlayInputFreq")
TRACE_MSG(MMIALARM_4287_112_2_18_2_0_58_31,"mmialarm.c HandleAlarmExpiredWinMsg() msg_id = %x")
TRACE_MSG(MMIALARM_4751_112_2_18_2_0_59_32,"MMIAPIALM_StartAlarm, find event result = %d")
TRACE_MSG(MMIALARM_4773_112_2_18_2_0_59_33,"MMIAPIALM_StartAlarm, delay alarm win pop")
TRACE_MSG(MMIALARM_4790_112_2_18_2_0_59_34,"MMIAPIALM_StartAlarm, open alarm win at once")
TRACE_MSG(MMIALARM_4797_112_2_18_2_0_59_35,"MMIAPIALM_StartAlarm, the alarm is not exist!")
TRACE_MSG(MMIALARM_4823_112_2_18_2_1_0_36,"mmialarm.c: OpenAlarmExpiredWin(), type = %d")
TRACE_MSG(MMIALARM_4924_112_2_18_2_1_0_37,"MMIAPIALM_CheckAndStartDelayedAlarm(), is_delayed = %d, is_on = %d")
TRACE_MSG(MMIALARM_5540_112_2_18_2_1_1_38,"mmialarm.c: HandleAutoPowerOffExpired() MSG_CTL_OK ")
TRACE_MSG(MMIALARM_6419_112_2_18_2_1_3_39,"OpenAlarmAliveWin error: Invalid s_arrived_event.type  is %d:")
TRACE_MSG(MMIALARM_6491_112_2_18_2_1_3_40,"SOGOU Update event coming")
TRACE_MSG(MMIALARM_6565_112_2_18_2_1_3_41,"HandlePastEventWinMsg, msg_id = %x")
TRACE_MSG(MMIALARM_6792_112_2_18_2_1_4_42,"OpenPastSchEventWin error: Invalid past_event->type is %d:")
TRACE_MSG(MMIALARM_6923_112_2_18_2_1_4_43,"mmialarm.c MMIAPIALM_GetFirstAlarmRing ReadRingInfoFileName fail!")
TRACE_MSG(MMIALARM_8646_112_2_18_2_1_8_44,"%s  %d  Event %d is set to on!")
TRACE_MSG(MMIALARM_9249_112_2_18_2_1_9_45,"MMIAPIALARM_GetEventTimeString:PNULL == str_ptr || ALM_EVENT_TIME_STR_LEN > str_len")
TRACE_MSG(MMIALARM_9419_112_2_18_2_1_9_46,"MMIAPIALARM_SetAlarmRing:alarm_id > EVENT_ALARM_MAX || PNULL == full_path_ptr || 0 == full_path_len!")
TRACE_MSG(MMIALARM_9527_112_2_18_2_1_10_47,"InitAlarmFolder result %d")
TRACE_MSG(MMIALARM_9580_112_2_18_2_1_10_48,"mmialarm.c GetRingInfoFileName MMIAPIFMM_CombineFullPath fail!")
TRACE_MSG(MMIALARM_9591_112_2_18_2_1_10_49,"GetRingInfoFileName event_id %d, ret %d")
TRACE_MSG(MMIALARM_9630_112_2_18_2_1_10_50,"MMIALM_SaveRingIDInfoToFile file_ret %d")
TRACE_MSG(MMIALARM_9676_112_2_18_2_1_10_51,"ReadRingInfoFileName file_ret %d")
TRACE_MSG(MMIALARM_9704_112_2_18_2_1_10_52,"DeleteRingInfoFile file_ret %d")
TRACE_MSG(MMIALARM_9834_112_2_18_2_1_10_53,"[MMIALM] MMIALM_Interrupt s_alarm_is_interrupted %d")
TRACE_MSG(MMIALARM_9843_112_2_18_2_1_10_54,"[MMIALM] MMIALM_GetAlmInterruptFlag s_alarm_is_interrupted %d")
TRACE_MSG(MMIALARM_9961_112_2_18_2_1_11_55,"mmialarm.c: HandlePastAutoPowerOn() MSG_CTL_OK ")
TRACE_MSG(MMIALARM_10395_112_2_18_2_1_11_56,"[MMIFM] ConvertFloatStrToInt ret %d")
TRACE_MSG(MMIALARM_CLOCK_568_112_2_18_2_1_15_57,"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y")
TRACE_MSG(MMIALARM_CLOCK_720_112_2_18_2_1_15_58,"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y")
TRACE_MSG(MMIALARM_CLOCK_1150_112_2_18_2_1_16_59,"DisplayAlmClock rect = %d,%d,%d,%d")
TRACE_MSG(MMIALARM_CLOCK_1155_112_2_18_2_1_16_60,"MMIWIDGET_ACLOCK DisplayAClock lcd_id = %d")
TRACE_MSG(MMIALARM_JUMP_CLOCK_331_112_2_18_2_1_18_61,"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y")
TRACE_MSG(MMIALARM_JUMP_CLOCK_483_112_2_18_2_1_18_62,"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y")
TRACE_MSG(MMIALARM_SERVICE_462_112_2_18_2_1_21_63,"StartAlmService:year=%d,mon=%d,mday=%d,hour=%d,min=%d")
TRACE_MSG(MMIALARM_SERVICE_472_112_2_18_2_1_21_64,"StartAlmService:while")
TRACE_MSG(MMIALARM_SERVICE_476_112_2_18_2_1_21_65,"StartAlmService:alarm callback")
TRACE_MSG(MMIALARM_SERVICE_566_112_2_18_2_1_21_66,"SetAlmService:cur_time:year=%d,mon=%d,mday=%d,hour=%d,min=%d")
TRACE_MSG(MMIALARM_SERVICE_585_112_2_18_2_1_21_67,"SetAlmService:latest_event_time:year=%d,mon=%d,mday=%d,hour=%d,min=%d")
TRACE_MSG(MMIALARM_SERVICE_593_112_2_18_2_1_21_68,"[MMIALARM SERVICE]SetAlmService enter!")
TRACE_MSG(MMIALARM_SERVICE_597_112_2_18_2_1_21_69,"SetAlmService: ERR_TM_NONE!")
TRACE_MSG(MMICALC_MAIN_784_112_2_18_2_1_23_70,"SetCalcTextStyle:PNULL == style_ptr")
TRACE_MSG(MMICALC_MAIN_854_112_2_18_2_1_24_71,"ProcessKeyPressed:PNULL == buf || max_len <= 0 !")
TRACE_MSG(MMICALC_MAIN_2991_112_2_18_2_1_28_72,"DigitLineEditor:PNULL == buf!")
TRACE_MSG(MMICALC_MAIN_3688_112_2_18_2_1_30_73,"wrong operator type")
TRACE_MSG(MMICALENDAR_MAIN_5027_112_2_18_2_1_41_74,"[MMIALM]HandleWaitWinMsg red key pressed")
TRACE_MSG(MMICALENDAR_MAIN_5761_112_2_18_2_1_43_75,"UpdateLayer:error!")
TRACE_MSG(MMICOUNTEDTIME_MAIN_771_112_2_18_2_1_48_76,"ST_DisplayCurTime: the rect is null")
TRACE_MSG(MMICOUNTEDTIME_MAIN_785_112_2_18_2_1_48_77,"ST_DisplayCurTime: the rect is invalid")
TRACE_MSG(MMICOUNTEDTIME_MAIN_825_112_2_18_2_1_48_78,"MMIAPICT_HandleTimer timer_id = %d")
TRACE_MSG(MMICOUNTEDTIME_MAIN_883_112_2_18_2_1_48_79,"MMIAPICT_HandleTimer s_ct_vibrator_time_id in")
TRACE_MSG(MMICOUNTEDTIME_MAIN_983_112_2_18_2_1_49_80,"Mmicountedtime_main.c, delay ct arrive win pop")
TRACE_MSG(MMICOUNTEDTIME_MAIN_998_112_2_18_2_1_49_81,"Mmicountedtime_main.c, open ct arrive win at once")
TRACE_MSG(MMICOUNTEDTIME_MAIN_1081_112_2_18_2_1_49_82,"MMIAPICT_HandleCountedTimeArriveWin: msg_id = %d")
TRACE_MSG(MMIPHONESEARCH_WINTAB_1972_112_2_18_2_1_55_83,"HandlePHONESEARCHDownloadCB enter")
TRACE_MSG(MMISCHEDULE_6723_112_2_18_2_2_10_84,"MMIAPISCH_IsEventToday s_user_list.total = %d")
TRACE_MSG(MMISCHEDULE_6779_112_2_18_2_2_10_85,"AddOneDetailItem fail ")
TRACE_MSG(MMISCHEDULE_7743_112_2_18_2_2_12_86,"MMIAPISCH_ReadVCalendarData")
TRACE_MSG(MMISCHEDULE_7746_112_2_18_2_2_12_87,"MMIAPISCH_ReadVCalendarData PNULL == data_ptr")
TRACE_MSG(MMISCHEDULE_7750_112_2_18_2_2_12_88,"MMIAPISCH_ReadVCalendarData: start parser")
TRACE_MSG(MMISCHEDULE_7797_112_2_18_2_2_12_89,"mmivcalendar: MMIAPISCH_ReadVcalendarFile")
TRACE_MSG(MMISCHEDULE_7800_112_2_18_2_2_12_90,"mmivcalendar MMIAPISCH_ReadVcalendarFile PNULL == name_ptr")
TRACE_MSG(MMISCHEDULE_7810_112_2_18_2_2_12_91,"MMIAPISCH_ReadVcalendarFile: start parser")
TRACE_MSG(MMISCHEDULE_8344_112_2_18_2_2_13_92,"VcalendarFolderSelectionCallBack, file_num = %d")
TRACE_MSG(MMISCHEDULE_8514_112_2_18_2_2_13_93,"SaveOneVCalendar is_success %d")
TRACE_MSG(MMISCHEDULE_8839_112_2_18_2_2_14_94,"HandleLoadVCalendarWaitWinMsg red key pressed")
TRACE_MSG(MMISCHEDULE_8904_112_2_18_2_2_15_95,"MMIAPISCH_OpenVCalendarFile")
TRACE_MSG(MMISCHEDULE_8908_112_2_18_2_2_15_96,"MMIAPISCH_OpenVCalendarFile NULL == name_ptr")
TRACE_MSG(MMISCHEDULE_8979_112_2_18_2_2_15_97,"MSG_VCALENDAR_SAVE_SCH_CNF is_save_success %d param %d ")
TRACE_MSG(MMISCHEDULE_ATC_160_112_2_18_2_2_17_98,"MMIMEMO_SyncAdd: memo_item_info_ptr->memo_mode is %d")
TRACE_MSG(MMISCHEDULE_ATC_321_112_2_18_2_2_18_99,"MMIMEMO_SyncModify: memo_item_info_ptr->memo_mode is %d")
TRACE_MSG(MMISCHEDULE_ATC_447_112_2_18_2_2_18_100,"MMIMEMO_SyncRead: event_ptr->fre_mode is %d")
TRACE_MSG(MMISTOPWATCH_MAIN_692_112_2_18_2_2_19_101,"ST_DisplayCurTime: the rect is invalid")
TRACE_MSG(MMISTOPWATCH_MAIN_770_112_2_18_2_2_20_102,"MMIAPIST_APIST_HandleTimer timer_id = %d")
TRACE_MSG(MMIUNITCONVERSION_1477_112_2_18_2_2_24_103,"StrToInt32:num=[%d],expand=[%d]")
TRACE_MSG(MMIUNITCONVERSION_1567_112_2_18_2_2_24_104,"UC:pre_validChar=[%d]")
TRACE_MSG(MMIUNITCONVERSION_1568_112_2_18_2_2_24_105,"UC:validChar=[%d]")
TRACE_MSG(MMIUNITCONVERSION_1615_112_2_18_2_2_24_106,"UC:lastchar=[%d]")
TRACE_MSG(MMIUNITCONVERSION_1958_112_2_18_2_2_25_107,"UC:result_interger.num=[%d]")
TRACE_MSG(MMIUNITCONVERSION_1959_112_2_18_2_2_25_108,"UC:result_decimal.num=[%d]")
TRACE_MSG(MMIUNITCONVERSION_1960_112_2_18_2_2_25_109,"UC:result_decimal.expand=[%d]")
TRACE_MSG(MMIWCLK_MAIN_721_112_2_18_2_2_28_110,"error: DrawAClock target_buf_ptr alloc failed,  return directly")
TRACE_MSG(MMIWCLK_MAIN_765_112_2_18_2_2_28_111,"error: foreground image rect is not in background image rect, return directly")
TRACE_MSG(MMIWCLK_MAIN_1683_112_2_18_2_2_30_112,"HandleWclkAddCityWinMsg, msg_id = 0x%02x")
TRACE_MSG(MMIWCLK_MAIN_2642_112_2_18_2_2_32_113,"MMIAUT::MMIAPIWCLK_SetTimeZone mcc= %d,time_zone = %d, current_year =%d, dst_value=%d")
TRACE_MSG(MMIWCLK_MAIN_2656_112_2_18_2_2_32_114,"MMIAUT::MMIAPIWCLK_SetTimeZone city_index1=%d")
TRACE_MSG(MMIWCLK_MAIN_2672_112_2_18_2_2_32_115,"MMIAUT::MMIAPIWCLK_SetTimeZone city_index2=%d")
TRACE_MSG(MMIWCLK_MAIN_2692_112_2_18_2_2_32_116,"MMIAUT::MMIAPIWCLK_SetTimeZone city_index3=%d")
TRACE_MSG(MMIWCLK_MAIN_2726_112_2_18_2_2_32_117,"MMIAUT::MMIAPIWCLK_SetTimeZone daylight_start_datetime=%d:%d:%d,%d-%d-%d")
TRACE_MSG(MMIWCLK_MAIN_2729_112_2_18_2_2_32_118,"MMIAUT::MMIAPIWCLK_SetTimeZone daylight_end_datetime=%d:%d:%d,%d-%d-%d")
TRACE_MSG(MMIWCLK_MAIN_2773_112_2_18_2_2_32_119,"MMIAUT::MMIAPIWCLK_SetTimeZone sys_second = %d,dst_start = %d,dst_end= %d")
TRACE_MSG(MMISCHEDULE_6335_112_2_18_2_2_32_120,"SaveEditSch")
END_TRACE_MAP(MMI_APP_ACCESSORY_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_ACCESSORY_TRC_H_

