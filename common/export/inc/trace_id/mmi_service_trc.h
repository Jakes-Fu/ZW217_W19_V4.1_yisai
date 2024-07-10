/******************************************************************************
 ** File Name:      mmi_service_trc.h                                         *
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
//trace_id:259
#ifndef _MMI_SERVICE_TRC_H_
#define _MMI_SERVICE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMISRVAUD_API_34_112_2_18_3_23_39_0 "[MMISRV]: Auido MMISRVAUD_Play() entry, audio_handle=0x%x, offset=%d"
#define MMISRVAUD_API_42_112_2_18_3_23_39_1 "[MMISRV]: Auido MMISRVAUD_Play() exit, res=%d"
#define MMISRVAUD_API_57_112_2_18_3_23_39_2 "[MMISRV]: Auido MMISRVAUD_PlayAsy() entry, audio_handle=0x%x, offset=%d"
#define MMISRVAUD_API_65_112_2_18_3_23_39_3 "[MMISRV]: Auido MMISRVAUD_PlayAsy() exit, res=%d"
#define MMISRVAUD_API_79_112_2_18_3_23_39_4 "[MMISRV]: Auido MMISRVAUD_Stop() entry, audio_handle=0x%x"
#define MMISRVAUD_API_83_112_2_18_3_23_39_5 "[MMISRV]: Auido MMISRVAUD_Stop() exit, res=%d"
#define MMISRVAUD_API_97_112_2_18_3_23_39_6 "[MMISRV]: Auido MMISRVAUD_Pause() entry, audio_handle=0x%x"
#define MMISRVAUD_API_101_112_2_18_3_23_39_7 "[MMISRV]: Auido MMISRVAUD_Pause() exit, res=%d"
#define MMISRVAUD_API_115_112_2_18_3_23_40_8 "[MMISRV]: Auido MMISRVAUD_Resume() entry, audio_handle=0x%x"
#define MMISRVAUD_API_119_112_2_18_3_23_40_9 "[MMISRV]: Auido MMISRVAUD_Resume() exit, res=%d"
#define MMISRVAUD_API_133_112_2_18_3_23_40_10 "[MMISRV]: Auido MMISRVAUD_PauseSlightly() entry, audio_handle=0x%x"
#define MMISRVAUD_API_137_112_2_18_3_23_40_11 "[MMISRV]: Auido MMISRVAUD_PauseSlightly() exit, res=%d"
#define MMISRVAUD_API_151_112_2_18_3_23_40_12 "[MMISRV]: Auido MMISRVAUD_ResumeSlightly() entry, audio_handle=0x%x"
#define MMISRVAUD_API_155_112_2_18_3_23_40_13 "[MMISRV]: Auido MMISRVAUD_ResumeSlightly() exit, res=%d"
#define MMISRVAUD_API_169_112_2_18_3_23_40_14 "[MMISRV]: Auido MMISRVAUD_Seek() entry, audio_handle=0x%x, offset=%d"
#define MMISRVAUD_API_176_112_2_18_3_23_40_15 "[MMISRV]: Auido MMISRVAUD_Seek() exit, res=%d"
#define MMISRVAUD_API_191_112_2_18_3_23_40_16 "[MMISRV]: Auido MMISRVAUD_SeekByTime() entry, audio_handle=0x%x,time=%d"
#define MMISRVAUD_API_205_112_2_18_3_23_40_17 "[MMISRV]: Auido MMISRVAUD_SeekByTime() exit, res=%d"
#define MMISRVAUD_API_219_112_2_18_3_23_40_18 "[MMISRV]: Auido MMISRVAUD_SetAudioInfo() entry, audio_handle=0x%x, audio_info=0x%x"
#define MMISRVAUD_API_228_112_2_18_3_23_40_19 "[MMISRV]: Auido MMISRVAUD_SetAudioInfo() exit, res=%d"
#define MMISRVAUD_API_243_112_2_18_3_23_40_20 "[MMISRV]: Auido MMISRVAUD_GetContentInfo() entry, audio_handle=0x%x, content_info_ptr=0x%x"
#define MMISRVAUD_API_252_112_2_18_3_23_40_21 "[MMISRV]: Auido MMISRVAUD_GetContentInfo() exit, res=%d"
#define MMISRVAUD_API_266_112_2_18_3_23_40_22 "[MMISRV]: Auido MMISRVAUD_GetPlayingInfo() entry, audio_handle=0x%x, playing_info_ptr=0x%x"
#define MMISRVAUD_API_275_112_2_18_3_23_40_23 "[MMISRV]: Auido MMISRVAUD_GetPlayingInfo() exit, res=%d"
#define MMISRVAUD_API_289_112_2_18_3_23_40_24 "[MMISRV]: Auido MMISRVAUD_GetRecordInfo() entry, audio_handle=0x%x, record_info_ptr=0x%x"
#define MMISRVAUD_API_298_112_2_18_3_23_40_25 "[MMISRV]: Auido MMISRVAUD_GetRecordInfo() exit, res=%d"
#define MMISRVAUD_API_311_112_2_18_3_23_40_26 "[MMISRV]: Auido MMISRVAUD_GetRecordSize() entry, audio_handle=0x%x, record_info_ptr=0x%x"
#define MMISRVAUD_API_320_112_2_18_3_23_40_27 "[MMISRV]: Auido MMISRVAUD_GetRecordSize() exit, res=%d"
#define MMISRVAUD_API_333_112_2_18_3_23_40_28 "[MMISRV]: Auido MMISRVAUD_SetPlaySpeed() entry, audio_handle=0x%x, speed=%d"
#define MMISRVAUD_API_343_112_2_18_3_23_40_29 "[MMISRV]: Auido MMISRVAUD_SetPlaySpeed() exit, res=%d"
#define MMISRVAUD_API_358_112_2_18_3_23_40_30 "[MMISRV]: Auido MMISRVAUD_SetEQMode() entry, audio_handle=0x%x, eq_mode=%d"
#define MMISRVAUD_API_368_112_2_18_3_23_40_31 "[MMISRV]: Auido MMISRVAUD_SetEQMode() exit, res=%d"
#define MMISRVAUD_API_384_112_2_18_3_23_40_32 "[MMISRV]: Auido MMISRVAUD_GetVolume() entry, audio_handle=0x%x"
#define MMISRVAUD_API_395_112_2_18_3_23_40_33 "[MMISRV]: Auido MMISRVAUD_GetVolume() exit, vol=%d"
#define MMISRVAUD_API_409_112_2_18_3_23_40_34 "[MMISRV]: Auido MMISRVAUD_SetVolume() entry, audio_handle=0x%x, volume=%d"
#define MMISRVAUD_API_419_112_2_18_3_23_40_35 "[MMISRV]: Auido MMISRVAUD_SetVolume() exit, res=%d"
#define MMISRVAUD_API_434_112_2_18_3_23_40_36 "[MMISRV]: Auido MMISRVAUD_GetRoute() entry, audio_handle=0x%x"
#define MMISRVAUD_API_446_112_2_18_3_23_40_37 "[MMISRV]: Auido MMISRVAUD_GetRoute() exit, route=%d"
#define MMISRVAUD_API_460_112_2_18_3_23_40_38 "[MMISRV]: Auido MMISRVAUD_SetRoute() entry, audio_handle=0x%x, audio_route=%d"
#define MMISRVAUD_API_470_112_2_18_3_23_40_39 "[MMISRV]: Auido MMISRVAUD_SetRoute() exit, res=%d"
#define MMISRVAUD_API_484_112_2_18_3_23_40_40 "[MMISRV]: Auido MMISRVAUD_SetAllSupportRoute() entry, audio_handle=0x%x, all_support_route=%d"
#define MMISRVAUD_API_494_112_2_18_3_23_40_41 "[MMISRV]: Auido MMISRVAUD_SetAllSupportRoute() exit, res=%d"
#define MMISRVAUD_API_509_112_2_18_3_23_40_42 "[MMISRV]: Auido MMISRVAUD_SetPlayTimes() entry, audio_handle=0x%x, times=%d"
#define MMISRVAUD_API_519_112_2_18_3_23_41_43 "[MMISRV]: Auido MMISRVAUD_SetPlayTimes() exit, res=%d"
#define MMISRVAUD_API_569_112_2_18_3_23_41_44 "[MMISRV]: Auido MMISRVAUD_IsAudioEnd() entry, handle=%d, result=0x%x"
#define MMISRVAUD_API_579_112_2_18_3_23_41_45 "[MMISRV]: Auido MMISRVAUD_IsAudioEnd() exit, res=%d,result=%d"
#define MMISRVAUD_API_597_112_2_18_3_23_41_46 "[MMISRV]: Auido MMISRVAUD_IsSetRouteManully() entry, handle=0x%x"
#define MMISRVAUD_API_610_112_2_18_3_23_41_47 "[MMISRV]: Auido MMISRVAUD_IsSetRouteManully() exit, is_set=%d"
#define MMISRVAUD_API_625_112_2_18_3_23_41_48 "[MMISRV]: Auido MMISRVAUD_GetSpectrum() entry, handle=0x%x, info=0x%x"
#define MMISRVAUD_API_634_112_2_18_3_23_41_49 "[MMISRV]: Auido MMISRVAUD_GetSpectrum() exit, res=%d"
#define MMISRVAUD_API_648_112_2_18_3_23_41_50 "[MMISRV]: Auido MMISRVAUD_OpenSpectrum() entry, handle=0x%x"
#define MMISRVAUD_API_656_112_2_18_3_23_41_51 "[MMISRV]: Auido MMISRVAUD_OpenSpectrum() exit, res=%d"
#define MMISRVAUD_API_670_112_2_18_3_23_41_52 "[MMISRV]: Auido MMISRVAUD_CloseSpectrum() entry, handle=0x%x"
#define MMISRVAUD_API_678_112_2_18_3_23_41_53 "[MMISRV]: Auido MMISRVAUD_CloseSpectrum() exit, res=%d"
#define MMISRVAUD_API_693_112_2_18_3_23_41_54 "[MMISRV]: Auido MMISRVAUD_SetVoiceLB() entry, handle=0x%x, is_enable=%d"
#define MMISRVAUD_API_703_112_2_18_3_23_41_55 "[MMISRV]: Auido MMISRVAUD_SetVoiceLB() exit, res=%d"
#define MMISRVAUD_API_718_112_2_18_3_23_41_56 "[MMISRV]: Auido MMISRVAUD_GetRecordDB() entry, handle=0x%x"
#define MMISRVAUD_API_730_112_2_18_3_23_41_57 "[MMISRV]: Auido MMISRVAUD_GetRecordDB() exit, db=%d"
#define MMISRVAUD_API_1087_112_2_18_3_23_42_58 "[MMISRV]: Auido MMISRVAUD_GetHandleByName() entry, client_name=0x%x"
#define MMISRVAUD_API_1097_112_2_18_3_23_42_59 "[MMISRV]: Auido MMISRVAUD_GetHandleByName() exit, res=%d, handle=0x%x"
#define MMISRVAUD_API_1113_112_2_18_3_23_42_60 "[MMISRV]: Auido GetSrvPri() entry, handle=0x%x"
#define MMISRVAUD_API_1125_112_2_18_3_23_42_61 "[MMISRV]: Auido GetSrvPri() exit, res=%d, pri=%d"
#define MMISRVAUD_API_1213_112_2_18_3_23_42_62 "[MMISRV]: MMISRVAUD_PlayToneEx() entry, tone_type=%d, tone_id=%d, duration=%d,freq=%d"
#define MMISRVAUD_API_1222_112_2_18_3_23_42_63 "[MMISRV]: MMISRVAUD_PlayToneEx() exit, handle=0x%x, res=%d"
#define MMISRVAUD_API_1262_112_2_18_3_23_42_64 "[MMISRV]: MMISRVAUD_PlayToneWithRoute() entry, tone_type=%d, tone_id=%d, duration=%d,freq=%d,route=%d"
#define MMISRVAUD_API_1275_112_2_18_3_23_42_65 "[MMISRV]: MMISRVAUD_PlayToneWithRoute() exit, handle=0x%x, res=%d"
#define MMISRVAUD_API_1289_112_2_18_3_23_42_66 "[MMISRV]: Auido MMISRVAUD_StopTone() entry"
#define MMISRVAUD_API_1297_112_2_18_3_23_42_67 "[MMISRV]: MMISRVAUD_StopTone() exit, handle=0x%x, res=%d"
#define MMISRVAUD_API_1312_112_2_18_3_23_42_68 "[MMISRV]: MMISRVAUD_StartVibrate() entry, pri=%d, duation=%d, interval=%d,times=%d,callback=0x%x"
#define MMISRVAUD_API_1342_112_2_18_3_23_42_69 "[MMISRV]: MMISRVAUD_StartVibrate() exit, handle=0x%x,res=%d"
#define MMISRVAUD_API_1357_112_2_18_3_23_42_70 "[MMISRV]: MMISRVAUD_StopVibrate() entry, handle=0x%x"
#define MMISRVAUD_API_1375_112_2_18_3_23_42_71 "[MMISRV]: MMISRVAUD_ExtDevChg() entry"
#define MMISRVAUD_API_1390_112_2_18_3_23_42_72 "[MMISRV]: MMISRVAUD_HandleBtEvent() entry, event=%d"
#define MMISRVAUD_API_1406_112_2_18_3_23_42_73 "[MMISRV]: MMISRVAUD_EnableVoiceLB() entry, handle=0x%x, is_enable=%d,volume=%d"
#define MMISRVAUD_API_1440_112_2_18_3_23_42_74 "[MMISRV]: MMISRVAUD_EnableVoiceLB() exit, handle=0x%x, res=%d"
#define MMISRVAUD_API_1455_112_2_18_3_23_42_75 "[MMISRV]: Auido MMISRVAUD_IsAudPlaying() entry, handle=0x%x"
#define MMISRVAUD_API_1467_112_2_18_3_23_42_76 "[MMISRV]: Auido MMISRVAUD_IsAudPlaying() exit, is_playing=%d"
#define MMISRVAUD_API_1481_112_2_18_3_23_42_77 "[MMISRV]: Auido MMISRVAUD_SetAtvServiceId() entry, handle=0x%x, service_id=%d"
#define MMISRVAUD_API_1491_112_2_18_3_23_42_78 "[MMISRV]: Auido MMISRVAUD_SetAtvServiceId() exit, res=%d"
#define MMISRVAUD_API_1505_112_2_18_3_23_42_79 "[MMISRV]: Auido MMISRVAUD_SetAtvRegionId() entry, handle=0x%x, region_id=%d"
#define MMISRVAUD_API_1514_112_2_18_3_23_42_80 "[MMISRV]: Auido MMISRVAUD_SetAtvRegionId() exit, res=%d"
#define MMISRVAUD_API_1528_112_2_18_3_23_43_81 "[MMISRV]: Auido MMISRVAUD_StartRecordAtv() entry, handle=0x%x"
#define MMISRVAUD_API_1537_112_2_18_3_23_43_82 "[MMISRV]: Auido MMISRVAUD_StartRecordAtv() exit, res=%d"
#define MMISRVAUD_API_1551_112_2_18_3_23_43_83 "[MMISRV]: Auido MMISRVAUD_StopRecordAtv() entry, handle=0x%x"
#define MMISRVAUD_API_1560_112_2_18_3_23_43_84 "[MMISRV]: Auido MMISRVAUD_StopRecordAtv() exit, res=%d"
#define MMISRVAUD_API_1576_112_2_18_3_23_43_85 "[MMISRV]: Auido MMISRVAUD_ReqVirtualHandle() entry, client_name=0x%x, pri=%d"
#define MMISRVAUD_API_1591_112_2_18_3_23_43_86 "[MMISRV]: Auido MMISRVAUD_ReqVirtualHandle() exit, handle=0x%x"
#define MMISRVAUD_API_1604_112_2_18_3_23_43_87 "[MMISRV]: Auido MMISRVAUD_FreeVirtualHandle() entry, client_name=0x%x"
#define MMISRVAUD_API_1614_112_2_18_3_23_43_88 "[MMISRV]: Auido MMISRVAUD_FreeVirtualHandle() exit, res=%d"
#define MMISRVAUD_API_1629_112_2_18_3_23_43_89 "[MMISRV]: Auido MMISRVAUD_GetHandleByVolumeType() entry, volume_type=%d"
#define MMISRVAUD_API_1636_112_2_18_3_23_43_90 "[MMISRV]: Auido MMISRVAUD_GetHandleByVolumeType() exit, handle=0x%x"
#define MMISRVAUD_API_1658_112_2_18_3_23_43_91 "[MMISRV]: Auido MMISRVAUD_NotifyVolumeSelfApp() entry, is_increase=%d"
#define MMISRVAUD_328_112_2_18_3_23_44_92 "[MMISRV]: Audio InitSrvAud() entry"
#define MMISRVAUD_340_112_2_18_3_23_44_93 "[MMISRV]: Audio InitSrvAud() failed!!!"
#define MMISRVAUD_427_112_2_18_3_23_44_94 "[MMISRV]: Audio GetHandleByEntityAddr() entry,entity_addr=0x%x, handle=0x%x"
#define MMISRVAUD_528_112_2_18_3_23_44_95 "[MMISRV]: Audio SrvNotifyClient() entry, entity_ptr=0x%x, entity_ptr->service.notify=0x%x,event=%d, data=0x%x"
#define MMISRVAUD_541_112_2_18_3_23_44_96 "[MMISRV]: Audio SrvNotifyClient() exit,handle=0x%x"
#define MMISRVAUD_581_112_2_18_3_23_44_97 "[MMISRV]: Audio GetEntityByDrvHandle() exit, drv handle=0x%x, i=%d, entity_ptr=0x%x"
#define MMISRVAUD_617_112_2_18_3_23_44_98 "[MMISRV]: Audio GetEntityByTimer() exit, timer=%d, i=%d, entity_ptr=0x%x"
#define MMISRVAUD_645_112_2_18_3_23_44_99 "[MMISRV]: Audio IsSrvAudCoexist() entry, active_type=0x%x, req_type=0x%x"
#define MMISRVAUD_706_112_2_18_3_23_44_100 "[MMISRV]: Audio IsVwpCoexist() entry, active link num=%d"
#define MMISRVAUD_742_112_2_18_3_23_44_101 "[MMISRV]: Audio IsVwpCoexist() exit, res=%d"
#define MMISRVAUD_810_112_2_18_3_23_45_102 "[MMISRV]: Audio IsNewSrvCoexist() entry, type=%d"
#define MMISRVAUD_816_112_2_18_3_23_45_103 "[MMISRV]: Audio IsNewSrvCoexist(), head node's type is same with type!"
#define MMISRVAUD_837_112_2_18_3_23_45_104 "[MMISRV]: Audio IsNewSrvCoexist(), type is same"
#define MMISRVAUD_846_112_2_18_3_23_45_105 "[MMISRV]: Audio IsNewSrvCoexist(), Congradulations, It's can be co-exist!!!"
#define MMISRVAUD_853_112_2_18_3_23_45_106 "[MMISRV]: Audio IsNewSrvCoexist(), Sorry, can't be co-exist!!!"
#define MMISRVAUD_863_112_2_18_3_23_45_107 "[MMISRV]: Audio IsSrvAudEnable() entry, is_enable=%d, s_aud_service_id=%d"
#define MMISRVAUD_874_112_2_18_3_23_45_108 "[MMISRV]: Audio IsSrvNeedSwitch() entry, active service num=%d"
#define MMISRVAUD_902_112_2_18_3_23_45_109 "[MMISRV]: Audio IsSrvTypeStanding() entry, type=%d"
#define MMISRVAUD_922_112_2_18_3_23_45_110 "[MMISRV]: Audio IsTypeSame() entry, type1=%d, type2=%d"
#define MMISRVAUD_954_112_2_18_3_23_45_111 "[MMISRV]: Audio GetCurrentWorkMode() entry, active link num=%d"
#define MMISRVAUD_979_112_2_18_3_23_45_112 "[MMISRV]: Audio GetCurrentWorkMode() exit, mode=%d"
#define MMISRVAUD_996_112_2_18_3_23_45_113 "[MMISRV]: Audio GetCreateMode() entry, req_data=0x%x, type_data=0x%x, is_mixing_enable=%d"
#define MMISRVAUD_1011_112_2_18_3_23_45_114 "[MMISRV]: Audio GetCreateMode(), audio_req->type=%d"
#define MMISRVAUD_1017_112_2_18_3_23_45_115 "[MMISRV]: Audio GetCreateMode(), service num is exeeded the limit!!!"
#define MMISRVAUD_1106_112_2_18_3_23_45_116 "[MMISRV]: Audio GetCreateMode() exit, pri=%d, type=%d, mode=%d"
#define MMISRVAUD_1119_112_2_18_3_23_45_117 "[MMISRV]: Audio IsCanCreateNewService() entry, req_data=0x%x, ext_data=0x%x"
#define MMISRVAUD_1134_112_2_18_3_23_45_118 "[MMISRV]: Audio IsCanCreateNewService(), service exeed tha max value!!!"
#define MMISRVAUD_1155_112_2_18_3_23_45_119 "[MMISRV]: Audio IsCanCreateNewService(), pri is lower than current!!!"
#define MMISRVAUD_1182_112_2_18_3_23_45_120 "[MMISRV]: Audio IsCanCreateNewService(), entiry_ptr is NULL!!!"
#define MMISRVAUD_1199_112_2_18_3_23_45_121 "[MMISRV]: Audio IsCanResume() entry, entity_ptr=0x%x"
#define MMISRVAUD_1231_112_2_18_3_23_45_122 "[MMISRV]: Audio IsCanResume(), pri is lower than current!!!"
#define MMISRVAUD_1251_112_2_18_3_23_45_123 "[MMISRV]: Audio IsCanResume(), error!!!! entity_active is NULL!"
#define MMISRVAUD_1256_112_2_18_3_23_45_124 "[MMISRV]: Audio IsCanResume() exit, result=%d"
#define MMISRVAUD_1372_112_2_18_3_23_46_125 "[MMISRV]: Audio IsNeedSetRoute(), entity_ptr=0x%x,type=%d,res=%d"
#define MMISRVAUD_1376_112_2_18_3_23_46_126 "[MMISRV]: Audio IsNeedSetRoute(), entity_ptr=NULL, Error!!!"
#define MMISRVAUD_1427_112_2_18_3_23_46_127 "[MMISRV]: Audio IsTypeSupportBt() exit, type=%d, fmt=%d, res=%d"
#define MMISRVAUD_1444_112_2_18_3_23_46_128 "[MMISRV]: Audio GetShouldBeRoute() exit, entity_ptr is NULL!!!"
#define MMISRVAUD_1449_112_2_18_3_23_46_129 "[MMISRV]: Audio GetShouldBeRoute() entry, entity_ptr=0x%x, all_support_route=%d, manual_route=%d, default_route=%d"
#define MMISRVAUD_1538_112_2_18_3_23_46_130 "[MMISRV]: Audio GetShouldBeRoute() exit, the route should be %d"
#define MMISRVAUD_1576_112_2_18_3_23_46_131 "[MMISRV]: Audio SwichRoute() entry, data=0x%x, s_bt_info.state=%d, is async=%d"
#define MMISRVAUD_1614_112_2_18_3_23_46_132 "[MMISRV]: Audio SwichRoute() exit,old route=%d, new route=%d"
#define MMISRVAUD_1663_112_2_18_3_23_46_133 "[MMISRV]: Audio SwichRoute() entry, data=0x%x"
#define MMISRVAUD_1705_112_2_18_3_23_47_134 "[MMISRV]: Audio PauseAudio() entry, data=0x%x"
#define MMISRVAUD_1711_112_2_18_3_23_47_135 "[MMISRV]: Audio PauseAudio(), drv_handle=0x%x, audio_state=%d"
#define MMISRVAUD_1729_112_2_18_3_23_47_136 "[MMISRV]: Audio PauseAudioAndNotify() entry, data=0x%x"
#define MMISRVAUD_1735_112_2_18_3_23_47_137 "[MMISRV]: Audio PauseAudioAndNotify(), drv_handle=0x%x, audio_state=%d, is_auto_free=%d"
#define MMISRVAUD_1749_112_2_18_3_23_47_138 "[MMISRV]: Audio PauseAudioAndNotify(), vibrate cb=0x%x"
#define MMISRVAUD_1788_112_2_18_3_23_47_139 "[MMISRV]: Audio ResumeAudio() entry, data=0x%x"
#define MMISRVAUD_1794_112_2_18_3_23_47_140 "[MMISRV]: Audio ResumeAudio(), drv_handle=0x%x, audio_state=%d"
#define MMISRVAUD_1825_112_2_18_3_23_47_141 "[MMISRV]: Audio SrvActiveSwitch() entry, entity=0x%x"
#define MMISRVAUD_1854_112_2_18_3_23_47_142 "[MMISRV]: Audio SetAudioInfo() entry, entity_ptr=0x%x"
#define MMISRVAUD_1862_112_2_18_3_23_47_143 "[MMISRV]: Audio SetAudioInfo(), old type=%d, new type=%d, service state=%d, audio state=%d"
#define MMISRVAUD_1943_112_2_18_3_23_47_144 "[MMISRV]: Audio SetAudioInfo() exit, res=%d"
#define MMISRVAUD_1983_112_2_18_3_23_47_145 "[MMISRV]: Audio SrvResumeOnly() entry, srv_ptr=0x%x"
#define MMISRVAUD_2026_112_2_18_3_23_47_146 "[MMISRV]: Audio SrvResumeOnly() exit, resume res=%d"
#define MMISRVAUD_2045_112_2_18_3_23_47_147 "[MMISRV]: Audio SrvCreate() entry, req_data=0x%x, ext_data=0x%x"
#define MMISRVAUD_2078_112_2_18_3_23_48_148 "[MMISRV]: Audio SrvCreate(), Can't create new service!!!"
#define MMISRVAUD_2084_112_2_18_3_23_48_149 "[MMISRV]: Audio SrvCreate(), Can't create new service with replace info!!!"
#define MMISRVAUD_2097_112_2_18_3_23_48_150 "[MMISRV]: Audio SrvCreate(), driver create handle failed!!!"
#define MMISRVAUD_2106_112_2_18_3_23_48_151 "[MMISRV]: Audio SrvCreate(), memery malloc failed!!!"
#define MMISRVAUD_2112_112_2_18_3_23_48_152 "[MMISRV]: Audio SrvCreate(), start to create a new service"
#define MMISRVAUD_2212_112_2_18_3_23_48_153 "[MMISRV]: Audio SrvCreate(), add to handle box failed!!!"
#define MMISRVAUD_2228_112_2_18_3_23_48_154 "[MMISRV]: Audio SrvResume() entry, srv_ptr=0x%x"
#define MMISRVAUD_2252_112_2_18_3_23_48_155 "[MMISRV]: Audio SrvResume() entry, srv_ptr=0x%x"
#define MMISRVAUD_2308_112_2_18_3_23_48_156 "[MMISRV]: Audio SrvResume() exit, resume res=%d"
#define MMISRVAUD_2323_112_2_18_3_23_48_157 "[MMISRV]: Audio SrvSuspend() entry, srv_ptr=0x%x"
#define MMISRVAUD_2374_112_2_18_3_23_48_158 "[MMISRV]: Audio SrvSuspend() entry, entity_ptr->service.srv_state=%d, res=%d"
#define MMISRVAUD_2392_112_2_18_3_23_48_159 "[MMISRV]: Audio SrvFree() entry, srv_ptr=0x%x"
#define MMISRVAUD_2429_112_2_18_3_23_48_160 "[MMISRV]: Audio SrvFree(), srv free Ok! is_auto_resume_off=%d"
#define MMISRVAUD_2434_112_2_18_3_23_48_161 "[MMISRV]: Audio SrvFree(), suspend_srv=%d, active service num=%d"
#define MMISRVAUD_2455_112_2_18_3_23_48_162 "[MMISRV]: Audio SrvFree(), srv free from stack Ok!"
#define MMISRVAUD_2459_112_2_18_3_23_48_163 "[MMISRV]: Audio SrvFree() exit, srv free Fail!"
#define MMISRVAUD_2493_112_2_18_3_23_48_164 "[MMISRV]: Audio SrvSetPri() entry, srv_ptr=0x%x,pri=%d"
#define MMISRVAUD_2515_112_2_18_3_23_48_165 "[MMISRV]: Audio SrvSetNotify() entry, srv_ptr=0x%x, notify=%d"
#define MMISRVAUD_2583_112_2_18_3_23_49_166 "[MMISRV]: Audio SrvIoCtrl() entry, srv_ptr=0x%x, data=0x%x, ret_value=0x%x"
#define MMISRVAUD_2593_112_2_18_3_23_49_167 "[MMISRV]: Audio SrvIoCtrl(), event_data->event=%d, drv_handle=0x%x, service_state=%d, aud_state=%d"
#define MMISRVAUD_2842_112_2_18_3_23_49_168 "[MMISRV]: Audio SrvIoCtrl() exit, res=%d"
#define MMISRVAUD_2860_112_2_18_3_23_49_169 "[MMISRV]: Audio SrvCmdFunc() entry, cmd=%d"
#define MMISRVAUD_2893_112_2_18_3_23_49_170 "[MMISRV]: Audio SrvCmdFunc() ,ext cmd=%d"
#define MMISRVAUD_3007_112_2_18_3_23_49_171 "[MMISRV]: Audio SrvCmdFunc() exit, res=%d"
#define MMISRVAUD_3033_112_2_18_3_23_49_172 "[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, type=%d, i=%d, entity_ptr=0x%x"
#define MMISRVAUD_3065_112_2_18_3_23_49_173 "[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, type%d, i=%d, entity_ptr=0x%x"
#define MMISRVAUD_3110_112_2_18_3_23_50_174 "[MMISRV]: Audio MMISRVAUD_HandleAudioReport() entry, drv_handle=0x%x, rpt_data=0x%x"
#define MMISRVAUD_3115_112_2_18_3_23_50_175 "[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), entity_ptr=0x%x,report=%d"
#define MMISRVAUD_3124_112_2_18_3_23_50_176 "[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), left play times=%d"
#define MMISRVAUD_3187_112_2_18_3_23_50_177 "[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), entity_ptr->service.notify=0x%x, type=%d"
#define MMISRVAUD_3192_112_2_18_3_23_50_178 "[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), vibrate cb=0x%x"
#define MMISRVAUD_3228_112_2_18_3_23_50_179 "[MMISRV]: Audio HandleBTCloseDelayTimer() entry, timer=%d,s_bt_info.timer=%d,s_bt_info.state=%d"
#define MMISRVAUD_3253_112_2_18_3_23_50_180 "[MMISRV]: Audio HandleBTCloseDelayTimer(), Nothing to do in these state"
#define MMISRVAUD_3268_112_2_18_3_23_50_181 "[MMISRV]: Audio IsBtNeedReOpen() entry, entity_ptr=0x%x, sample_rate=%d, cur_entity_ptr=0x%x, cur sample_rate=%d, s_bt_info.state=%d"
#define MMISRVAUD_3274_112_2_18_3_23_50_182 "[MMISRV]: Audio IsBtNeedReOpen(), new type=%d, old type=%d"
#define MMISRVAUD_3306_112_2_18_3_23_50_183 "[MMISRV]: Audio IsBtNeedReOpen() exit, res=%d"
#define MMISRVAUD_3322_112_2_18_3_23_50_184 "[MMISRV]: Audio BtPrePlay() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d"
#define MMISRVAUD_3344_112_2_18_3_23_50_185 "[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_NONE"
#define MMISRVAUD_3361_112_2_18_3_23_50_186 "[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_OPEN_WAIT or BT_STATE_CLOSE_WAIT "
#define MMISRVAUD_3370_112_2_18_3_23_50_187 "[MMISRV]: Audio BtPrePlay(), error!!! BT should not be BT_STATE_PLAY"
#define MMISRVAUD_3386_112_2_18_3_23_50_188 "[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_DELAY_TIMER_WAIT"
#define MMISRVAUD_3409_112_2_18_3_23_50_189 "[MMISRV]: Audio BtPrePlay(), error!!! No this state!"
#define MMISRVAUD_3414_112_2_18_3_23_50_190 "[MMISRV]: Audio BtPrePlay() exit, result=%d"
#define MMISRVAUD_3429_112_2_18_3_23_50_191 "[MMISRV]: Audio BtPreResume() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d"
#define MMISRVAUD_3471_112_2_18_3_23_50_192 "[MMISRV]: Audio BtPreResume(), error!!! BT state should not be BT_STATE_PLAY"
#define MMISRVAUD_3508_112_2_18_3_23_50_193 "[MMISRV]: Audio BtPreResume() exit, result=%d"
#define MMISRVAUD_3522_112_2_18_3_23_50_194 "[MMISRV]: Audio BtPrePause() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d"
#define MMISRVAUD_3568_112_2_18_3_23_50_195 "[MMISRV]: Audio BtPrePause() exit, result=%d"
#define MMISRVAUD_3583_112_2_18_3_23_51_196 "[MMISRV]: Audio BtPrePause() entry, entity_ptr=0x%x, data=0x%x, s_bt_info.state=%d"
#define MMISRVAUD_3629_112_2_18_3_23_51_197 "[MMISRV]: Audio BtPrePause() exit, result=%d"
#define MMISRVAUD_3644_112_2_18_3_23_51_198 "[MMISRV]: Audio BtActiveHandle() entry"
#define MMISRVAUD_3680_112_2_18_3_23_51_199 "[MMISRV]: Audio BtDeActiveHandle() entry"
#define MMISRVAUD_3703_112_2_18_3_23_51_200 "[MMISRV]: Audio BtOpenedHandle() entry, s_bt_info.state=%d, cur entity_addr=0x%x,event=0x%d,sample_rate=%d"
#define MMISRVAUD_3705_112_2_18_3_23_51_201 "[MMISRV]: Audio BtOpenedHandle(), wait entity_addr=0x%x,event=0x%d,sample_rate=%d"
#define MMISRVAUD_3777_112_2_18_3_23_51_202 "[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d"
#define MMISRVAUD_3815_112_2_18_3_23_51_203 "[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d"
#define MMISRVAUD_3821_112_2_18_3_23_51_204 "[MMISRV]: Audio BtOpenedHandle(), cur event error!! event=%d"
#define MMISRVAUD_3910_112_2_18_3_23_51_205 "[MMISRV]: Audio BtOpenedHandle() exit, result=%d"
#define MMISRVAUD_3929_112_2_18_3_23_51_206 "[MMISRV]: Audio BTClosedHandle() entry, s_bt_info.state=%d, cur entity_addr=0x%x,event=0x%d,sample_rate=%d"
#define MMISRVAUD_3931_112_2_18_3_23_51_207 "[MMISRV]: Audio BTClosedHandle(), wait entity_addr=0x%x,event=0x%d,sample_rate=%d"
#define MMISRVAUD_3993_112_2_18_3_23_51_208 "[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d"
#define MMISRVAUD_4016_112_2_18_3_23_51_209 "[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d"
#define MMISRVAUD_4022_112_2_18_3_23_51_210 "[MMISRV]: Audio BtOpenedHandle(), cur event error!! event=%d"
#define MMISRVAUD_4049_112_2_18_3_23_51_211 "[MMISRV]: Audio BTClosedHandle() fatal error!! Entity should not be NULL"
#define MMISRVAUD_4072_112_2_18_3_23_52_212 "[MMISRV]: Audio BTClosedHandle() error bt state, may BT module error!!!"
#define MMISRVAUD_4097_112_2_18_3_23_52_213 "[MMISRV]: Audio BtNotifyAppEvent() error!!! handle=0"
#define MMISRVAUD_4107_112_2_18_3_23_52_214 "[MMISRV]: Audio BtNotifyAppEvent() entry, route=%d"
#define MMISRVAUD_4126_112_2_18_3_23_52_215 "[MMISRV]: Audio BtEventHandle() entry, event=%d"
#define MMISRVAUD_4179_112_2_18_3_23_52_216 "[MMISRV]: Audio AudPlay() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d"
#define MMISRVAUD_4238_112_2_18_3_23_52_217 "[MMISRV]: Audio AudPlay() exit, ret_val=%d"
#define MMISRVAUD_4257_112_2_18_3_23_52_218 "[MMISRV]: Audio AudResume() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d"
#define MMISRVAUD_4292_112_2_18_3_23_52_219 "[MMISRV]: Audio AudResume() exit, result=%d"
#define MMISRVAUD_4311_112_2_18_3_23_52_220 "[MMISRV]: Audio AudPause() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d,is_change_state=%d"
#define MMISRVAUD_4328_112_2_18_3_23_52_221 "[MMISRV]: Audio, Don't not surpport audio mixing now!!"
#define MMISRVAUD_4348_112_2_18_3_23_52_222 "[MMISRV]: Audio AudPause() exit, result=%d"
#define MMISRVAUD_4367_112_2_18_3_23_52_223 "[MMISRV]: Audio AudStop() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d"
#define MMISRVAUD_4386_112_2_18_3_23_52_224 "[MMISRV]: Audio, Don't not surpport audio mixing now!!"
#define MMISRVAUD_4413_112_2_18_3_23_52_225 "[MMISRV]: Audio AudStop() exit, result=%d"
#define MMISRVAUD_4440_112_2_18_3_23_52_226 "[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, handle%d, i=%d, entity_ptr=0x%x"
#define MMISRVAUD_DRV_58_112_2_18_3_23_53_227 "[MMISRV]aud drv: AudioDrvCallback() entry, hAudio=0x%x, notify_info=%d,affix_info=%d, END_IND=%d"
#define MMISRVAUD_DRV_79_112_2_18_3_23_53_228 "[MMISRV]aud drv: VideoDrvCallback entry, i_type = %d"
#define MMISRVAUD_DRV_98_112_2_18_3_23_53_229 "[MMISRV]aud drv: VideoDrvCallback entry, i_type = %d"
#define MMISRVAUD_DRV_118_112_2_18_3_23_53_230 "[MMISRV]aud drv: MvDrvCallback entry, service_info_ptr = 0x%x"
#define MMISRVAUD_DRV_167_112_2_18_3_23_53_231 "[MMISRV]aud drv: ConvertRoute(), route to device mode, route=%d, mode=%d"
#define MMISRVAUD_DRV_203_112_2_18_3_23_53_232 "[MMISRV]aud drv: ConvertEQType() eq to ref eq, eq_mode=%d, ref_eq_mode=%d"
#define MMISRVAUD_DRV_236_112_2_18_3_23_53_233 "[MMISRV]aud drv: ConvertDevMode() exit, mode=%d, route=%d"
#define MMISRVAUD_DRV_322_112_2_18_3_23_53_234 "[MMISRV]aud drv: GetRingCodec() exit, ring_type=%d, codec=0x%x"
#define MMISRVAUD_DRV_354_112_2_18_3_23_53_235 "[MMISRV]aud drv: GetToneCodec() exit, tone_type=%d, hAudioCodec=0x%x"
#define MMISRVAUD_DRV_405_112_2_18_3_23_53_236 "[MMISRV]aud drv: GetAudioDevice() exit, is_a2dp=%d, ring_fmt=%d"
#define MMISRVAUD_DRV_479_112_2_18_3_23_53_237 "[MMISRV]aud drv: GetAudioFmt() entry, type_ptr=0x%x"
#define MMISRVAUD_DRV_518_112_2_18_3_23_54_238 "[MMISRV]aud drv: GetAudioFmt() exit, fmt=%d"
#define MMISRVAUD_DRV_547_112_2_18_3_23_54_239 "[MMISRV]aud drv: CreateAudioBufferHandle() entry, is_a2dp=%d, ring_fmt=%d, pucData=0x%x, uiDataLenght=%d,buff=0x%x, size=%d"
#define MMISRVAUD_DRV_576_112_2_18_3_23_54_240 "[MMISRV]aud drv: CreateAudioBufferHandle() exit,"
#define MMISRVAUD_DRV_601_112_2_18_3_23_54_241 "[MMISRV]aud drv: CreateAudioFileHandle() entry"
#define MMISRVAUD_DRV_630_112_2_18_3_23_54_242 "[MMISRV]aud drv: CreateAudioFileHandle() exit,"
#define MMISRVAUD_DRV_657_112_2_18_3_23_54_243 "[MMISRV]aud drv: CreateAudioStreamingHandle() entry"
#define MMISRVAUD_DRV_688_112_2_18_3_23_54_244 "[MMISRV]aud drv: CreateAudioStreamingHandle() exit,"
#define MMISRVAUD_DRV_709_112_2_18_3_23_54_245 "[MMISRV]aud drv: CreateIQHandle() entry, name_ptr=0x%x,record_param=0x%x"
#define MMISRVAUD_DRV_744_112_2_18_3_23_54_246 "[MMISRV]aud drv: CreateIQHandle() exit, handle=0x%x,result=%d"
#define MMISRVAUD_DRV_810_112_2_18_3_23_54_247 "[MMISRV]aud drv: CreateRecordFileHandle() entry, src=%d, fmt=%d,frame_len=%d, file_handle=0x%x"
#define MMISRVAUD_DRV_888_112_2_18_3_23_54_248 "[MMISRV]aud drv: CreateAspFileHandle() entry, is_a2dp=%d, ring_fmt=%d, name_ptr=0x%x, buff=0x%x, size=%d"
#define MMISRVAUD_DRV_916_112_2_18_3_23_54_249 "[MMISRV]aud drv: CreateAspFileHandle() exit, handle=0x%x, result=%d"
#define MMISRVAUD_DRV_938_112_2_18_3_23_54_250 "[MMISRV]aud drv: CreateAspBufferHandle() entry, is_a2dp=%d, ring_fmt=%d, pucData=0x%x, uiDataLenght=%d,buff=0x%x, size=%d"
#define MMISRVAUD_DRV_967_112_2_18_3_23_54_251 "[MMISRV]aud drv: CreateAspBufferHandle() exit, handle=0x%x, result=%d"
#define MMISRVAUD_DRV_1005_112_2_18_3_23_55_252 "[MMISRV]aud drv: DRVAUD_CreateHandle() entry, audio_data->type=%d"
#define MMISRVAUD_DRV_1134_112_2_18_3_23_55_253 "[MMISRV]aud drv: Error!!! ATV param is exeeded the store memory!!!!!"
#define MMISRVAUD_DRV_1147_112_2_18_3_23_55_254 "[MMISRV]aud drv: DRVAUD_CreateHandle() exit, handle=0x%x"
#define MMISRVAUD_DRV_1192_112_2_18_3_23_55_255 "[MMISRV]aud drv: DRVAUD_CloseHandle() exit, handle=0x%x, result=%d"
#define MMISRVAUD_DRV_1221_112_2_18_3_23_55_256 "[MMISRV]aud drv: VideoPlay() exit, handle=0x%x, result=%d, ret=%d"
#define MMISRVAUD_DRV_1243_112_2_18_3_23_55_257 "[MMISRV]aud drv: VideoPause() exit, handle=0x%x, result=%d, ret=%d"
#define MMISRVAUD_DRV_1265_112_2_18_3_23_55_258 "[MMISRV]aud drv: VideoResume() exit, handle=0x%x, result=%d, ret=%d"
#define MMISRVAUD_DRV_1287_112_2_18_3_23_55_259 "[MMISRV]aud drv: VideoStop() exit, handle=0x%x, result=%d, ret=%d"
#define MMISRVAUD_DRV_1299_112_2_18_3_23_55_260 "[MMISRV]: ParamCtrlCallBack"
#define MMISRVAUD_DRV_1320_112_2_18_3_23_55_261 "[MMISRV]aud drv: AtvPlay(),set region, ret=%d"
#define MMISRVAUD_DRV_1324_112_2_18_3_23_55_262 "[MMISRV]aud drv: AtvPlay(),set region, ret=%d"
#define MMISRVAUD_DRV_1327_112_2_18_3_23_55_263 "[MMISRV]aud drv: AtvPlay(), preview, ret=%d"
#define MMISRVAUD_DRV_1334_112_2_18_3_23_55_264 "[MMISRV]aud drv: AtvPlay() exit, error!!!param is null!"
#define MMISRVAUD_DRV_1356_112_2_18_3_23_55_265 "[MMISRV]aud drv: AtvStop() exit, ret=%d"
#define MMISRVAUD_DRV_1378_112_2_18_3_23_56_266 "[MMISRV]aud drv: AtvPause() exit, ret=%d"
#define MMISRVAUD_DRV_1405_112_2_18_3_23_56_267 "[MMISRV]aud drv: AtvResume() exit, ret=%d"
#define MMISRVAUD_DRV_1409_112_2_18_3_23_56_268 "[MMISRV]aud drv: AtvResume() exit, error!!! param is null!"
#define MMISRVAUD_DRV_1430_112_2_18_3_23_56_269 "[MMISRV]aud drv: DRVAUD_StartReocrdATV() entry, param=0x%x"
#define MMISRVAUD_DRV_1435_112_2_18_3_23_56_270 "[MMISRV]aud drv: DRVAUD_StartReocrdATV(), stop preview result=%d"
#define MMISRVAUD_DRV_1439_112_2_18_3_23_56_271 "[MMISRV]aud drv: DRVAUD_StartReocrdATV(), start record result=%d"
#define MMISRVAUD_DRV_1459_112_2_18_3_23_56_272 "[MMISRV]aud drv: DRVAUD_StopReocrdATV() entry, type_ptr=0x%x"
#define MMISRVAUD_DRV_1462_112_2_18_3_23_56_273 "[MMISRV]aud drv: DRVAUD_StopReocrdATV(), stop record result=%d"
#define MMISRVAUD_DRV_1474_112_2_18_3_23_56_274 "[MMISRV]aud drv: DRVAUD_StopReocrdATV(),set region, ret=%d"
#define MMISRVAUD_DRV_1478_112_2_18_3_23_56_275 "[MMISRV]aud drv: DRVAUD_StopReocrdATV(),set region, ret=%d"
#define MMISRVAUD_DRV_1481_112_2_18_3_23_56_276 "[MMISRV]aud drv: DRVAUD_StopReocrdATV(), preview, ret=%d"
#define MMISRVAUD_DRV_1487_112_2_18_3_23_56_277 "[MMISRV]aud drv: DRVAUD_StopReocrdATV(), start record result=%d"
#define MMISRVAUD_DRV_1523_112_2_18_3_23_56_278 "[MMISRV]aud drv: MvPlay() entry, mv=0x%x"
#define MMISRVAUD_DRV_1544_112_2_18_3_23_56_279 "[MMISRV]aud drv: MvStop() exit, OK"
#define MMISRVAUD_DRV_1550_112_2_18_3_23_56_280 "[MMISRV]aud drv: MvStop() exit, error"
#define MMISRVAUD_DRV_1563_112_2_18_3_23_56_281 "[MMISRV]aud drv: MvStop() exit, OK"
#define MMISRVAUD_DRV_1569_112_2_18_3_23_56_282 "[MMISRV]aud drv: MvStop() exit, error"
#define MMISRVAUD_DRV_1580_112_2_18_3_23_56_283 "[MMISRV]aud drv: MvPlay() entry, mv=0x%x"
#define MMISRVAUD_DRV_1614_112_2_18_3_23_56_284 "[MMISRV]aud drv: BtVoicePlay() exit, no memory, error!!!"
#define MMISRVAUD_DRV_1638_112_2_18_3_23_56_285 "[MMISRV]aud drv: BtVoicePlay() exit, play handle=0x%x, rec handle=0x%x, play_ret=%d, rec_ret=%d"
#define MMISRVAUD_DRV_1682_112_2_18_3_23_56_286 "[MMISRV]aud drv: BtVoiceStop() exit, play handle=0x%x, rec handle=0x%x, stop play_ret=%d, stop rec_ret=%d, close play_ret=%d, close rec_ret=%d"
#define MMISRVAUD_DRV_1695_112_2_18_3_23_56_287 "[MMISRV]aud drv: DRVAUD_Play() entry, handle=0x%x, type_ptr=%d,offset=%d, route=%d"
#define MMISRVAUD_DRV_1732_112_2_18_3_23_56_288 "[MMISRV]aud drv: DRVAUD_Play(), AUDIO_Play fail, res=%d"
#define MMISRVAUD_DRV_1738_112_2_18_3_23_56_289 "[MMISRV]aud drv: DRVAUD_Play(), BtVoicePlay, res=%d"
#define MMISRVAUD_DRV_1837_112_2_18_3_23_56_290 "[MMISRV]aud drv: DRVAUD_Play() exit, type=%d, result=%d"
#define MMISRVAUD_DRV_1858_112_2_18_3_23_56_291 "[MMISRV]aud drv: DRVAUD_PlayAsy() exit, handle=0x%x, offset=%d, result=%d"
#define MMISRVAUD_DRV_1877_112_2_18_3_23_57_292 "[MMISRV]aud drv: DRVAUD_PlayAsy() entry, handle=0x%x, type_ptr=0x%x, offset=%d"
#define MMISRVAUD_DRV_1907_112_2_18_3_23_57_293 "[MMISRV]aud drv: DRVAUD_PlayAsy() exit, type=%d, res=%d"
#define MMISRVAUD_DRV_1913_112_2_18_3_23_57_294 "[MMISRV]aud drv: DRVAUD_PlayAsy() exit, type=%d, result=%d"
#define MMISRVAUD_DRV_1967_112_2_18_3_23_57_295 "[MMISRV]aud drv: DRVAUD_Pause() exit, handle=0x%x, type_ptr=0x%x, result=%d"
#define MMISRVAUD_DRV_2021_112_2_18_3_23_57_296 "[MMISRV]aud drv: DRVAUD_Stop(), HFG_AudioStop, res=%d"
#define MMISRVAUD_DRV_2029_112_2_18_3_23_57_297 "[MMISRV]aud drv: DRVAUD_Stop() exit, handle=0x%x,result=%d"
#define MMISRVAUD_DRV_2082_112_2_18_3_23_57_298 "[MMISRV]aud drv: DRVAUD_Resume() exit, handle=0x%x, type_ptr=0x%s, result=%d"
#define MMISRVAUD_DRV_2102_112_2_18_3_23_57_299 "[MMISRV]aud drv: DRVAUD_PauseSlight() entry, handle=0x%x, type_ptr=0x%"
#define MMISRVAUD_DRV_2114_112_2_18_3_23_57_300 "[MMISRV]aud drv: DRVAUD_PauseSlight(), error type!!!!! type=%d"
#define MMISRVAUD_DRV_2118_112_2_18_3_23_57_301 "[MMISRV]aud drv: DRVAUD_PauseSlight() exit, result=%d"
#define MMISRVAUD_DRV_2136_112_2_18_3_23_57_302 "[MMISRV]aud drv: DRVAUD_ResumeSlight() entry, handle=0x%x, type_ptr=0x%"
#define MMISRVAUD_DRV_2148_112_2_18_3_23_57_303 "[MMISRV]aud drv: DRVAUD_ResumeSlight(), error type!!!!! type=%d"
#define MMISRVAUD_DRV_2152_112_2_18_3_23_57_304 "[MMISRV]aud drv: DRVAUD_ResumeSlight() exit, result=%d"
#define MMISRVAUD_DRV_2175_112_2_18_3_23_57_305 "[MMISRV]aud drv: DRVAUD_Seek() exit, handle=0x%x, offset=%d, result=%d"
#define MMISRVAUD_DRV_2193_112_2_18_3_23_57_306 "[MMISRV]aud drv: DRVAUD_GetContentInfo() entry, handle=0x%x, content_info=%d"
#define MMISRVAUD_DRV_2221_112_2_18_3_23_57_307 "[MMISRV]aud drv: DRVAUD_GetContentInfo() exit, ok!!!"
#define MMISRVAUD_DRV_2227_112_2_18_3_23_57_308 "[MMISRV]aud drv: DRVAUD_GetContentInfo() exit, error!!!"
#define MMISRVAUD_DRV_2240_112_2_18_3_23_57_309 "[MMISRV]aud drv: DRVAUD_GetPlayingInfo() entry, handle=0x%x, play_info=0x%x"
#define MMISRVAUD_DRV_2253_112_2_18_3_23_57_310 "[MMISRV]aud drv: DRVAUD_GetPlayingInfo() exit, ok!!! current time=%d, total time=%d"
#define MMISRVAUD_DRV_2258_112_2_18_3_23_57_311 "[MMISRV]aud drv: DRVAUD_GetPlayingInfo() exit, error!!!"
#define MMISRVAUD_DRV_2275_112_2_18_3_23_57_312 "[MMISRV]aud drv: DRVAUD_GetRecordInfo() entry, handle=0x%x, record_info"
#define MMISRVAUD_DRV_2291_112_2_18_3_23_57_313 "[MMISRV]aud drv: DRVAUD_GetRecordInfo() exit, total_time=%d"
#define MMISRVAUD_DRV_2296_112_2_18_3_23_57_314 "[MMISRV]aud drv: DRVAUD_GetRecordInfo() exit, error!!!"
#define MMISRVAUD_DRV_2313_112_2_18_3_23_57_315 "[MMISRV]aud drv: DRVAUD_GetRecordSize() entry, handle=0x%x, record_info"
#define MMISRVAUD_DRV_2329_112_2_18_3_23_57_316 "[MMISRV]aud drv: DRVAUD_GetRecordSize() exit, total_time=%d"
#define MMISRVAUD_DRV_2334_112_2_18_3_23_57_317 "[MMISRV]aud drv: DRVAUD_GetRecordSize() exit, error!!!"
#define MMISRVAUD_DRV_2351_112_2_18_3_23_57_318 "[MMISRV]aud drv: DRVAUD_SetPlaySpeed() exit, handle=0x%x, speed=%d, result=%d"
#define MMISRVAUD_DRV_2390_112_2_18_3_23_58_319 "[MMISRV]aud drv: DRVAUD_SetEQMode() exit, handle=0x%x, eq_mode=%d, result=%d"
#define MMISRVAUD_DRV_2404_112_2_18_3_23_58_320 "[MMISRV]aud drv: DRVAUD_SetVolume() entry, handle=0x%x, volume=%d"
#define MMISRVAUD_DRV_2408_112_2_18_3_23_58_321 "[MMISRV]aud drv: DRVAUD_SetVolume() exit, don't know the mute state!"
#define MMISRVAUD_DRV_2435_112_2_18_3_23_58_322 "[MMISRV]aud drv: DRVAUD_SetVolume() exit, is_cur_mute=%d, result=%d"
#define MMISRVAUD_DRV_2457_112_2_18_3_23_58_323 "[MMISRV]aud drv: DRVAUD_GetVolume() exit, handle=0x%x, volume=0x%x, vol=%d, result=%d"
#define MMISRVAUD_DRV_2484_112_2_18_3_23_58_324 "[MMISRV]aud drv: DRVAUD_SetRoute() exit, error! handle=0x%x, route=0x%x, type_ptr=%d"
#define MMISRVAUD_DRV_2511_112_2_18_3_23_58_325 "[MMISRV]aud drv: DRVAUD_SetRoute() exit, handle=0x%x, route=0x%x, set_dev_mode=%d, cur_dev_mode=%d"
#define MMISRVAUD_DRV_2535_112_2_18_3_23_58_326 "[MMISRV]aud drv: DRVAUD_SetRoute() exit, error! handle=0x%x, route=%d, type_ptr=0x%x"
#define MMISRVAUD_DRV_2571_112_2_18_3_23_58_327 "[MMISRV]aud drv: DRVAUD_SetRoute(), MMISRVAUD_TYPE_ATV type, switch result=%d, hDevice=0x%x"
#define MMISRVAUD_DRV_2596_112_2_18_3_23_58_328 "[MMISRV]aud drv: DRVAUD_SetRoute(), normal audio, switch result=%d, hDevice=0x%x"
#define MMISRVAUD_DRV_2636_112_2_18_3_23_58_329 "[MMISRV]aud drv: DRVAUD_SetRoute() exit, handle=0x%x, route=%d, set_dev_mode=%d, cur_dev_mode=%d"
#define MMISRVAUD_DRV_2653_112_2_18_3_23_58_330 "[MMISRV]aud drv: DRVAUD_GetRoute() exit, handle=0x%x, route=%d, cur_dev_mode=%d, result=%d"
#define MMISRVAUD_DRV_2679_112_2_18_3_23_58_331 "[MMISRV]aud drv: DRVAUD_GetAudCfg() exit, handle=0x%x, cfg=0x%x"
#define MMISRVAUD_DRV_2711_112_2_18_3_23_58_332 "[MMISRV]aud drv: DRVAUD_GetAudCfg() exit, handle=0x%x, info=0x%x, result=%d"
#define MMISRVAUD_DRV_2735_112_2_18_3_23_58_333 "[MMISRV]aud drv: DRVAUD_OpenSpectrum() exit, handle=0x%x,result=%d"
#define MMISRVAUD_DRV_2759_112_2_18_3_23_58_334 "[MMISRV]aud drv: DRVAUD_CloseSpectrum() exit, handle=0x%x,result=%d"
#define MMISRVAUD_DRV_2783_112_2_18_3_23_58_335 "[MMISRV]aud drv: DRVAUD_SetVoiceLB() exit, handle=0x%x,is_enable=%d, result=%d"
#define MMISRVAUD_DRV_2816_112_2_18_3_23_58_336 "[MMISRV]aud drv: DRVAUD_SetMute() exit, handle=0x%x,is_mute=%d, is_cur_mute=%d,result=%d"
#define MMISRVAUD_DRV_2838_112_2_18_3_23_58_337 "[MMISRV]aud drv: DRVAUD_GetMute() exit, handle=0x%x,is_mute=0x%x, is_cur_mute=%d,result=%d"
#define MMISRVAUD_DRV_2861_112_2_18_3_23_58_338 "[MMISRV]aud drv: DRVAUD_SetUplinkMute() exit, handle=0x%x,is_mute=%d, result=%d"
#define MMISRVAUD_DRV_2884_112_2_18_3_23_59_339 "[MMISRV]aud drv:DRVAUD_SetPlayRate entry, handle=0x%x,rate =%d"
#define MMISRVAUD_DRV_2897_112_2_18_3_23_59_340 "[MMISRV]aud drv: DRVAUD_SetPlayRate() exit, handle=0x%x,rate=%d,play_rate=%d,result=%d"
#define MMISRVAUD_DRV_2921_112_2_18_3_23_59_341 "[MMISRV]aud drv:DRVAUD_GetRecordDb entry, handle=0x%x, db=-0x%x"
#define MMISRVAUD_DRV_2939_112_2_18_3_23_59_342 "[MMISRV]aud drv: DRVAUD_GetRecordDb exit, res=%d,db =%d!"
#define MMISRVAUD_DRV_2959_112_2_18_3_23_59_343 "[MMISRV]: Audio MMISRVAUD_HandleCallBackMsg() entry, callback_info_ptr=0x%x"
#define MMISRVAUD_DRV_3025_112_2_18_3_23_59_344 "[MMISRV]: Audio MMISRVAUD_HandleCallBackMsg() exit, handle=0x%x, result=%d, notify_info=%d,affix_info=%d"
#define MMISRVAUD_UTIL_59_112_2_18_3_23_59_345 "[MMISRV]: Audio GetLinkHeadByType() entry, link_type=%d"
#define MMISRVAUD_UTIL_96_112_2_18_3_23_59_346 "[MMISRV]: Audio MMISRVAUD_LinkCreate() entry"
#define MMISRVAUD_UTIL_123_112_2_18_3_23_59_347 "[MMISRV]: Audio MMISRVAUD_LinkCreate() exit!!!"
#define MMISRVAUD_UTIL_139_112_2_18_3_23_59_348 "[MMISRV]: Audio MMISRVAUD_LinkPush() entry, link_type=%d,data=%d,link_head=0x%x"
#define MMISRVAUD_UTIL_171_112_2_18_3_23_59_349 "[MMISRV]: Audio MMISRVAUD_LinkPop() entry, link_type=%d,link_head=0x%x"
#define MMISRVAUD_UTIL_185_112_2_18_3_23_59_350 "[MMISRV]: Audio MMISRVAUD_LinkPop() exit, data=0x%x"
#define MMISRVAUD_UTIL_201_112_2_18_3_23_59_351 "[MMISRV]: Audio MMISRVAUD_LinkAppend() entry, link_type=%d,data=%d,link_head=0x%x"
#define MMISRVAUD_UTIL_233_112_2_18_3_23_59_352 "[MMISRV]: Audio MMISRVAUD_LinkDeAppend() entry, link_type=%d,link_head=0x%x"
#define MMISRVAUD_UTIL_247_112_2_18_3_23_59_353 "[MMISRV]: Audio MMISRVAUD_LinkDeAppend() exit, data=0x%x"
#define MMISRVAUD_UTIL_264_112_2_18_3_23_59_354 "[MMISRV]: Audio MMISRVAUD_LinkGetHeadNode() entry, link_type=%d,link_head=0x%x"
#define MMISRVAUD_UTIL_279_112_2_18_3_24_0_355 "[MMISRV]: Audio MMISRVAUD_LinkGetHeadNode() exit, data=0x%x"
#define MMISRVAUD_UTIL_296_112_2_18_3_24_0_356 "[MMISRV]: Audio MMISRVAUD_LinkGetTailNode() entry, link_type=%d,link_head=0x%x"
#define MMISRVAUD_UTIL_309_112_2_18_3_24_0_357 "[MMISRV]: Audio MMISRVAUD_LinkGetTailNode() exit, data=0x%x"
#define MMISRVAUD_UTIL_342_112_2_18_3_24_0_358 "[MMISRV]: Audio MMISRVAUD_LinkGetNum() entry, link_type=%d,link_head=0x%x"
#define MMISRVAUD_UTIL_357_112_2_18_3_24_0_359 "[MMISRV]: Audio MMISRVAUD_LinkGetNum() entry, num=%d"
#define MMISRVAUD_UTIL_373_112_2_18_3_24_0_360 "[MMISRV]: Audio MMISRVAUD_LinkGetAndDelete() entry, link_type=%d,data=0x%x, link_head=0x%x"
#define MMISRVAUD_UTIL_385_112_2_18_3_24_0_361 "[MMISRV]: Audio MMISRVAUD_LinkGetAndDelete() exit, found! link_node=0x%x"
#define MMISRVAUD_UTIL_407_112_2_18_3_24_0_362 "[MMISRV]: Audio MMISRVAUD_LinkGetNode() entry, link_type=%d,data=0x%x, link_head=0x%x"
#define MMISRVAUD_UTIL_435_112_2_18_3_24_0_363 "[MMISRV]: Audio MMISRVAUD_LinkSuspend() entry, active link_head=0x%x"
#define MMISRVAUD_UTIL_443_112_2_18_3_24_0_364 "[MMISRV]: Audio MMISRVAUD_LinkSuspend(), new active link head=0x%x"
#define MMISRVAUD_UTIL_459_112_2_18_3_24_0_365 "[MMISRV]: Audio MMISRVAUD_LinkResume() entry, active link_head=0x%x,num=%d,data=0x%x"
#define MMISRVAUD_UTIL_513_112_2_18_3_24_0_366 "[MMISRV]: Audio MMISRVAUD_LinkTravel() entry, link_type=%d, link_head=0x%x, func=0x%x"
#define MMISRVAUD_UTIL_541_112_2_18_3_24_0_367 "[MMISRV]: Audio MMISRVAUD_LinkTravel() entry, link_type=%d, link_head=0x%x, func=0x%x"
#define MMISRVAUD_UTIL_577_112_2_18_3_24_0_368 "[MMISRV]: Audio, MMISRVAUD_StackPush entry, data=0x%x, stack_num=%d"
#define MMISRVAUD_UTIL_581_112_2_18_3_24_0_369 "[MMISRV]: Audio, MMISRVAUD_StackPush, active link num=%d"
#define MMISRVAUD_UTIL_592_112_2_18_3_24_0_370 "[MMISRV]: Audio, stack is full!!!"
#define MMISRVAUD_UTIL_605_112_2_18_3_24_0_371 "[MMISRV]: Audio, MMISRVAUD_StackPop entry, s_aud_stack.num=%d"
#define MMISRVAUD_UTIL_618_112_2_18_3_24_0_372 "[MMISRV]: Audio, MMISRVAUD_StackPop, success!!"
#define MMISRVAUD_UTIL_626_112_2_18_3_24_0_373 "[MMISRV]: Audio, MMISRVAUD_StackPop, this link is empty!!!"
#define MMISRVAUD_UTIL_629_112_2_18_3_24_0_374 "[MMISRV]: Audio, MMISRVAUD_StackPop exit, data=0x%x, s_aud_stack.num=%d"
#define MMISRVAUD_UTIL_644_112_2_18_3_24_0_375 "[MMISRV]: Audio, MMISRVAUD_StackGetLink entry, data=0x%x, s_aud_stack.num=%d"
#define MMISRVAUD_UTIL_669_112_2_18_3_24_0_376 "[MMISRV]: Audio, MMISRVAUD_StackGetLink,Found! i=%d"
#define MMISRVAUD_UTIL_683_112_2_18_3_24_0_377 "[MMISRV]: Audio, MMISRVAUD_StackGetLink, Not Found!"
#define MMISRVAUD_UTIL_700_112_2_18_3_24_0_378 "[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete entry, data=0x%x, s_aud_stack.num=%d"
#define MMISRVAUD_UTIL_725_112_2_18_3_24_0_379 "[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete, current link is empty!!!"
#define MMISRVAUD_UTIL_733_112_2_18_3_24_0_380 "[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete, current is not top! i=%d"
#define MMISRVAUD_UTIL_751_112_2_18_3_24_0_381 "[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete exit, i=%, res=%d"
#define MMISRVAUD_UTIL_780_112_2_18_3_24_1_382 "[MMISRV]: Audio, MMISRVAUD_IsHeadSetPlugIn =%d"
#define MMISRVAUD_UTIL_797_112_2_18_3_24_1_383 "[MMISRV]: Audio, MMISRVAUD_IsBTHeadSetActive =%d"
#define MMISRVAUD_UTIL_819_112_2_18_3_24_1_384 "[MMISRV]: Audio, MMISRVAUD_CreateTimer entry, time_out=%d"
#define MMISRVAUD_UTIL_832_112_2_18_3_24_1_385 "[MMISRV]: Audio, MMISRVAUD_StopTimer entry, timer_id=%d"
#define MMISRVAUD_UTIL_849_112_2_18_3_24_1_386 "[MMISRV]: Audio, MMISRVAUD_BtOpen entry, type_ptr=0x%x,sample_rate=%d"
#define MMISRVAUD_UTIL_874_112_2_18_3_24_1_387 "[MMISRV]: Audio, MMISRVAUD_BtOpen exit, result=%d,type=%d"
#define MMISRVAUD_UTIL_891_112_2_18_3_24_1_388 "[MMISRV]: Audio, MMISRVAUD_BtClose entry"
#define MMISRVAUD_UTIL_911_112_2_18_3_24_1_389 "[MMISRV]: Audio, MMISRVAUD_BtClose exit, result=%d"
#define MMISRVMGR_113_112_2_18_3_24_3_390 "[MMISRV]: MGR MMISRVMGR_Request() entry, srv_name=0x%x, req_data=0x%x,ext_data=0x%x"
#define MMISRVMGR_135_112_2_18_3_24_3_391 "[MMISRV]: MGR MMISRVMGR_Request() exit, handle=0x%x,cmd_func=0x%x"
#define MMISRVMGR_149_112_2_18_3_24_3_392 "[MMISRV]: MGR MMISRVMGR_Free() entry, handle=0x%x"
#define MMISRVMGR_161_112_2_18_3_24_3_393 "[MMISRV]: MGR MMISRVMGR_Free() exit, res=%d"
#define MMISRVMGR_175_112_2_18_3_24_3_394 "[MMISRV]: MGR MMISRVMGR_Suspend() entry, handle=0x%x"
#define MMISRVMGR_187_112_2_18_3_24_3_395 "[MMISRV]: MGR MMISRVMGR_Suspend() exit, res=%d"
#define MMISRVMGR_201_112_2_18_3_24_3_396 "[MMISRV]: MGR MMISRVMGR_Resume() entry, handle=0x%x"
#define MMISRVMGR_213_112_2_18_3_24_3_397 "[MMISRV]: MGR MMISRVMGR_Resume() exit, res=%d"
#define MMISRVMGR_227_112_2_18_3_24_3_398 "[MMISRV]: MGR MMISRVMGR_SetPri() entry,handle =0x%x pri=%d"
#define MMISRVMGR_239_112_2_18_3_24_3_399 "[MMISRV]: MGR MMISRVMGR_SetPri() exit, res=%d"
#define MMISRVMGR_253_112_2_18_3_24_3_400 "[MMISRV]: MGR MMISRVMGR_SetNotify() entry,handle =0x%x notify=0x%x"
#define MMISRVMGR_265_112_2_18_3_24_3_401 "[MMISRV]: MGR MMISRVMGR_SetNotify() exit, res=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_SERVICE_TRC)
TRACE_MSG(MMISRVAUD_API_34_112_2_18_3_23_39_0,"[MMISRV]: Auido MMISRVAUD_Play() entry, audio_handle=0x%x, offset=%d")
TRACE_MSG(MMISRVAUD_API_42_112_2_18_3_23_39_1,"[MMISRV]: Auido MMISRVAUD_Play() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_57_112_2_18_3_23_39_2,"[MMISRV]: Auido MMISRVAUD_PlayAsy() entry, audio_handle=0x%x, offset=%d")
TRACE_MSG(MMISRVAUD_API_65_112_2_18_3_23_39_3,"[MMISRV]: Auido MMISRVAUD_PlayAsy() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_79_112_2_18_3_23_39_4,"[MMISRV]: Auido MMISRVAUD_Stop() entry, audio_handle=0x%x")
TRACE_MSG(MMISRVAUD_API_83_112_2_18_3_23_39_5,"[MMISRV]: Auido MMISRVAUD_Stop() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_97_112_2_18_3_23_39_6,"[MMISRV]: Auido MMISRVAUD_Pause() entry, audio_handle=0x%x")
TRACE_MSG(MMISRVAUD_API_101_112_2_18_3_23_39_7,"[MMISRV]: Auido MMISRVAUD_Pause() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_115_112_2_18_3_23_40_8,"[MMISRV]: Auido MMISRVAUD_Resume() entry, audio_handle=0x%x")
TRACE_MSG(MMISRVAUD_API_119_112_2_18_3_23_40_9,"[MMISRV]: Auido MMISRVAUD_Resume() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_133_112_2_18_3_23_40_10,"[MMISRV]: Auido MMISRVAUD_PauseSlightly() entry, audio_handle=0x%x")
TRACE_MSG(MMISRVAUD_API_137_112_2_18_3_23_40_11,"[MMISRV]: Auido MMISRVAUD_PauseSlightly() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_151_112_2_18_3_23_40_12,"[MMISRV]: Auido MMISRVAUD_ResumeSlightly() entry, audio_handle=0x%x")
TRACE_MSG(MMISRVAUD_API_155_112_2_18_3_23_40_13,"[MMISRV]: Auido MMISRVAUD_ResumeSlightly() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_169_112_2_18_3_23_40_14,"[MMISRV]: Auido MMISRVAUD_Seek() entry, audio_handle=0x%x, offset=%d")
TRACE_MSG(MMISRVAUD_API_176_112_2_18_3_23_40_15,"[MMISRV]: Auido MMISRVAUD_Seek() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_191_112_2_18_3_23_40_16,"[MMISRV]: Auido MMISRVAUD_SeekByTime() entry, audio_handle=0x%x,time=%d")
TRACE_MSG(MMISRVAUD_API_205_112_2_18_3_23_40_17,"[MMISRV]: Auido MMISRVAUD_SeekByTime() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_219_112_2_18_3_23_40_18,"[MMISRV]: Auido MMISRVAUD_SetAudioInfo() entry, audio_handle=0x%x, audio_info=0x%x")
TRACE_MSG(MMISRVAUD_API_228_112_2_18_3_23_40_19,"[MMISRV]: Auido MMISRVAUD_SetAudioInfo() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_243_112_2_18_3_23_40_20,"[MMISRV]: Auido MMISRVAUD_GetContentInfo() entry, audio_handle=0x%x, content_info_ptr=0x%x")
TRACE_MSG(MMISRVAUD_API_252_112_2_18_3_23_40_21,"[MMISRV]: Auido MMISRVAUD_GetContentInfo() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_266_112_2_18_3_23_40_22,"[MMISRV]: Auido MMISRVAUD_GetPlayingInfo() entry, audio_handle=0x%x, playing_info_ptr=0x%x")
TRACE_MSG(MMISRVAUD_API_275_112_2_18_3_23_40_23,"[MMISRV]: Auido MMISRVAUD_GetPlayingInfo() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_289_112_2_18_3_23_40_24,"[MMISRV]: Auido MMISRVAUD_GetRecordInfo() entry, audio_handle=0x%x, record_info_ptr=0x%x")
TRACE_MSG(MMISRVAUD_API_298_112_2_18_3_23_40_25,"[MMISRV]: Auido MMISRVAUD_GetRecordInfo() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_311_112_2_18_3_23_40_26,"[MMISRV]: Auido MMISRVAUD_GetRecordSize() entry, audio_handle=0x%x, record_info_ptr=0x%x")
TRACE_MSG(MMISRVAUD_API_320_112_2_18_3_23_40_27,"[MMISRV]: Auido MMISRVAUD_GetRecordSize() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_333_112_2_18_3_23_40_28,"[MMISRV]: Auido MMISRVAUD_SetPlaySpeed() entry, audio_handle=0x%x, speed=%d")
TRACE_MSG(MMISRVAUD_API_343_112_2_18_3_23_40_29,"[MMISRV]: Auido MMISRVAUD_SetPlaySpeed() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_358_112_2_18_3_23_40_30,"[MMISRV]: Auido MMISRVAUD_SetEQMode() entry, audio_handle=0x%x, eq_mode=%d")
TRACE_MSG(MMISRVAUD_API_368_112_2_18_3_23_40_31,"[MMISRV]: Auido MMISRVAUD_SetEQMode() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_384_112_2_18_3_23_40_32,"[MMISRV]: Auido MMISRVAUD_GetVolume() entry, audio_handle=0x%x")
TRACE_MSG(MMISRVAUD_API_395_112_2_18_3_23_40_33,"[MMISRV]: Auido MMISRVAUD_GetVolume() exit, vol=%d")
TRACE_MSG(MMISRVAUD_API_409_112_2_18_3_23_40_34,"[MMISRV]: Auido MMISRVAUD_SetVolume() entry, audio_handle=0x%x, volume=%d")
TRACE_MSG(MMISRVAUD_API_419_112_2_18_3_23_40_35,"[MMISRV]: Auido MMISRVAUD_SetVolume() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_434_112_2_18_3_23_40_36,"[MMISRV]: Auido MMISRVAUD_GetRoute() entry, audio_handle=0x%x")
TRACE_MSG(MMISRVAUD_API_446_112_2_18_3_23_40_37,"[MMISRV]: Auido MMISRVAUD_GetRoute() exit, route=%d")
TRACE_MSG(MMISRVAUD_API_460_112_2_18_3_23_40_38,"[MMISRV]: Auido MMISRVAUD_SetRoute() entry, audio_handle=0x%x, audio_route=%d")
TRACE_MSG(MMISRVAUD_API_470_112_2_18_3_23_40_39,"[MMISRV]: Auido MMISRVAUD_SetRoute() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_484_112_2_18_3_23_40_40,"[MMISRV]: Auido MMISRVAUD_SetAllSupportRoute() entry, audio_handle=0x%x, all_support_route=%d")
TRACE_MSG(MMISRVAUD_API_494_112_2_18_3_23_40_41,"[MMISRV]: Auido MMISRVAUD_SetAllSupportRoute() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_509_112_2_18_3_23_40_42,"[MMISRV]: Auido MMISRVAUD_SetPlayTimes() entry, audio_handle=0x%x, times=%d")
TRACE_MSG(MMISRVAUD_API_519_112_2_18_3_23_41_43,"[MMISRV]: Auido MMISRVAUD_SetPlayTimes() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_569_112_2_18_3_23_41_44,"[MMISRV]: Auido MMISRVAUD_IsAudioEnd() entry, handle=%d, result=0x%x")
TRACE_MSG(MMISRVAUD_API_579_112_2_18_3_23_41_45,"[MMISRV]: Auido MMISRVAUD_IsAudioEnd() exit, res=%d,result=%d")
TRACE_MSG(MMISRVAUD_API_597_112_2_18_3_23_41_46,"[MMISRV]: Auido MMISRVAUD_IsSetRouteManully() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_610_112_2_18_3_23_41_47,"[MMISRV]: Auido MMISRVAUD_IsSetRouteManully() exit, is_set=%d")
TRACE_MSG(MMISRVAUD_API_625_112_2_18_3_23_41_48,"[MMISRV]: Auido MMISRVAUD_GetSpectrum() entry, handle=0x%x, info=0x%x")
TRACE_MSG(MMISRVAUD_API_634_112_2_18_3_23_41_49,"[MMISRV]: Auido MMISRVAUD_GetSpectrum() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_648_112_2_18_3_23_41_50,"[MMISRV]: Auido MMISRVAUD_OpenSpectrum() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_656_112_2_18_3_23_41_51,"[MMISRV]: Auido MMISRVAUD_OpenSpectrum() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_670_112_2_18_3_23_41_52,"[MMISRV]: Auido MMISRVAUD_CloseSpectrum() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_678_112_2_18_3_23_41_53,"[MMISRV]: Auido MMISRVAUD_CloseSpectrum() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_693_112_2_18_3_23_41_54,"[MMISRV]: Auido MMISRVAUD_SetVoiceLB() entry, handle=0x%x, is_enable=%d")
TRACE_MSG(MMISRVAUD_API_703_112_2_18_3_23_41_55,"[MMISRV]: Auido MMISRVAUD_SetVoiceLB() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_718_112_2_18_3_23_41_56,"[MMISRV]: Auido MMISRVAUD_GetRecordDB() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_730_112_2_18_3_23_41_57,"[MMISRV]: Auido MMISRVAUD_GetRecordDB() exit, db=%d")
TRACE_MSG(MMISRVAUD_API_1087_112_2_18_3_23_42_58,"[MMISRV]: Auido MMISRVAUD_GetHandleByName() entry, client_name=0x%x")
TRACE_MSG(MMISRVAUD_API_1097_112_2_18_3_23_42_59,"[MMISRV]: Auido MMISRVAUD_GetHandleByName() exit, res=%d, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_1113_112_2_18_3_23_42_60,"[MMISRV]: Auido GetSrvPri() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_1125_112_2_18_3_23_42_61,"[MMISRV]: Auido GetSrvPri() exit, res=%d, pri=%d")
TRACE_MSG(MMISRVAUD_API_1213_112_2_18_3_23_42_62,"[MMISRV]: MMISRVAUD_PlayToneEx() entry, tone_type=%d, tone_id=%d, duration=%d,freq=%d")
TRACE_MSG(MMISRVAUD_API_1222_112_2_18_3_23_42_63,"[MMISRV]: MMISRVAUD_PlayToneEx() exit, handle=0x%x, res=%d")
TRACE_MSG(MMISRVAUD_API_1262_112_2_18_3_23_42_64,"[MMISRV]: MMISRVAUD_PlayToneWithRoute() entry, tone_type=%d, tone_id=%d, duration=%d,freq=%d,route=%d")
TRACE_MSG(MMISRVAUD_API_1275_112_2_18_3_23_42_65,"[MMISRV]: MMISRVAUD_PlayToneWithRoute() exit, handle=0x%x, res=%d")
TRACE_MSG(MMISRVAUD_API_1289_112_2_18_3_23_42_66,"[MMISRV]: Auido MMISRVAUD_StopTone() entry")
TRACE_MSG(MMISRVAUD_API_1297_112_2_18_3_23_42_67,"[MMISRV]: MMISRVAUD_StopTone() exit, handle=0x%x, res=%d")
TRACE_MSG(MMISRVAUD_API_1312_112_2_18_3_23_42_68,"[MMISRV]: MMISRVAUD_StartVibrate() entry, pri=%d, duation=%d, interval=%d,times=%d,callback=0x%x")
TRACE_MSG(MMISRVAUD_API_1342_112_2_18_3_23_42_69,"[MMISRV]: MMISRVAUD_StartVibrate() exit, handle=0x%x,res=%d")
TRACE_MSG(MMISRVAUD_API_1357_112_2_18_3_23_42_70,"[MMISRV]: MMISRVAUD_StopVibrate() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_1375_112_2_18_3_23_42_71,"[MMISRV]: MMISRVAUD_ExtDevChg() entry")
TRACE_MSG(MMISRVAUD_API_1390_112_2_18_3_23_42_72,"[MMISRV]: MMISRVAUD_HandleBtEvent() entry, event=%d")
TRACE_MSG(MMISRVAUD_API_1406_112_2_18_3_23_42_73,"[MMISRV]: MMISRVAUD_EnableVoiceLB() entry, handle=0x%x, is_enable=%d,volume=%d")
TRACE_MSG(MMISRVAUD_API_1440_112_2_18_3_23_42_74,"[MMISRV]: MMISRVAUD_EnableVoiceLB() exit, handle=0x%x, res=%d")
TRACE_MSG(MMISRVAUD_API_1455_112_2_18_3_23_42_75,"[MMISRV]: Auido MMISRVAUD_IsAudPlaying() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_1467_112_2_18_3_23_42_76,"[MMISRV]: Auido MMISRVAUD_IsAudPlaying() exit, is_playing=%d")
TRACE_MSG(MMISRVAUD_API_1481_112_2_18_3_23_42_77,"[MMISRV]: Auido MMISRVAUD_SetAtvServiceId() entry, handle=0x%x, service_id=%d")
TRACE_MSG(MMISRVAUD_API_1491_112_2_18_3_23_42_78,"[MMISRV]: Auido MMISRVAUD_SetAtvServiceId() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_1505_112_2_18_3_23_42_79,"[MMISRV]: Auido MMISRVAUD_SetAtvRegionId() entry, handle=0x%x, region_id=%d")
TRACE_MSG(MMISRVAUD_API_1514_112_2_18_3_23_42_80,"[MMISRV]: Auido MMISRVAUD_SetAtvRegionId() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_1528_112_2_18_3_23_43_81,"[MMISRV]: Auido MMISRVAUD_StartRecordAtv() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_1537_112_2_18_3_23_43_82,"[MMISRV]: Auido MMISRVAUD_StartRecordAtv() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_1551_112_2_18_3_23_43_83,"[MMISRV]: Auido MMISRVAUD_StopRecordAtv() entry, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_1560_112_2_18_3_23_43_84,"[MMISRV]: Auido MMISRVAUD_StopRecordAtv() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_1576_112_2_18_3_23_43_85,"[MMISRV]: Auido MMISRVAUD_ReqVirtualHandle() entry, client_name=0x%x, pri=%d")
TRACE_MSG(MMISRVAUD_API_1591_112_2_18_3_23_43_86,"[MMISRV]: Auido MMISRVAUD_ReqVirtualHandle() exit, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_1604_112_2_18_3_23_43_87,"[MMISRV]: Auido MMISRVAUD_FreeVirtualHandle() entry, client_name=0x%x")
TRACE_MSG(MMISRVAUD_API_1614_112_2_18_3_23_43_88,"[MMISRV]: Auido MMISRVAUD_FreeVirtualHandle() exit, res=%d")
TRACE_MSG(MMISRVAUD_API_1629_112_2_18_3_23_43_89,"[MMISRV]: Auido MMISRVAUD_GetHandleByVolumeType() entry, volume_type=%d")
TRACE_MSG(MMISRVAUD_API_1636_112_2_18_3_23_43_90,"[MMISRV]: Auido MMISRVAUD_GetHandleByVolumeType() exit, handle=0x%x")
TRACE_MSG(MMISRVAUD_API_1658_112_2_18_3_23_43_91,"[MMISRV]: Auido MMISRVAUD_NotifyVolumeSelfApp() entry, is_increase=%d")
TRACE_MSG(MMISRVAUD_328_112_2_18_3_23_44_92,"[MMISRV]: Audio InitSrvAud() entry")
TRACE_MSG(MMISRVAUD_340_112_2_18_3_23_44_93,"[MMISRV]: Audio InitSrvAud() failed!!!")
TRACE_MSG(MMISRVAUD_427_112_2_18_3_23_44_94,"[MMISRV]: Audio GetHandleByEntityAddr() entry,entity_addr=0x%x, handle=0x%x")
TRACE_MSG(MMISRVAUD_528_112_2_18_3_23_44_95,"[MMISRV]: Audio SrvNotifyClient() entry, entity_ptr=0x%x, entity_ptr->service.notify=0x%x,event=%d, data=0x%x")
TRACE_MSG(MMISRVAUD_541_112_2_18_3_23_44_96,"[MMISRV]: Audio SrvNotifyClient() exit,handle=0x%x")
TRACE_MSG(MMISRVAUD_581_112_2_18_3_23_44_97,"[MMISRV]: Audio GetEntityByDrvHandle() exit, drv handle=0x%x, i=%d, entity_ptr=0x%x")
TRACE_MSG(MMISRVAUD_617_112_2_18_3_23_44_98,"[MMISRV]: Audio GetEntityByTimer() exit, timer=%d, i=%d, entity_ptr=0x%x")
TRACE_MSG(MMISRVAUD_645_112_2_18_3_23_44_99,"[MMISRV]: Audio IsSrvAudCoexist() entry, active_type=0x%x, req_type=0x%x")
TRACE_MSG(MMISRVAUD_706_112_2_18_3_23_44_100,"[MMISRV]: Audio IsVwpCoexist() entry, active link num=%d")
TRACE_MSG(MMISRVAUD_742_112_2_18_3_23_44_101,"[MMISRV]: Audio IsVwpCoexist() exit, res=%d")
TRACE_MSG(MMISRVAUD_810_112_2_18_3_23_45_102,"[MMISRV]: Audio IsNewSrvCoexist() entry, type=%d")
TRACE_MSG(MMISRVAUD_816_112_2_18_3_23_45_103,"[MMISRV]: Audio IsNewSrvCoexist(), head node's type is same with type!")
TRACE_MSG(MMISRVAUD_837_112_2_18_3_23_45_104,"[MMISRV]: Audio IsNewSrvCoexist(), type is same")
TRACE_MSG(MMISRVAUD_846_112_2_18_3_23_45_105,"[MMISRV]: Audio IsNewSrvCoexist(), Congradulations, It's can be co-exist!!!")
TRACE_MSG(MMISRVAUD_853_112_2_18_3_23_45_106,"[MMISRV]: Audio IsNewSrvCoexist(), Sorry, can't be co-exist!!!")
TRACE_MSG(MMISRVAUD_863_112_2_18_3_23_45_107,"[MMISRV]: Audio IsSrvAudEnable() entry, is_enable=%d, s_aud_service_id=%d")
TRACE_MSG(MMISRVAUD_874_112_2_18_3_23_45_108,"[MMISRV]: Audio IsSrvNeedSwitch() entry, active service num=%d")
TRACE_MSG(MMISRVAUD_902_112_2_18_3_23_45_109,"[MMISRV]: Audio IsSrvTypeStanding() entry, type=%d")
TRACE_MSG(MMISRVAUD_922_112_2_18_3_23_45_110,"[MMISRV]: Audio IsTypeSame() entry, type1=%d, type2=%d")
TRACE_MSG(MMISRVAUD_954_112_2_18_3_23_45_111,"[MMISRV]: Audio GetCurrentWorkMode() entry, active link num=%d")
TRACE_MSG(MMISRVAUD_979_112_2_18_3_23_45_112,"[MMISRV]: Audio GetCurrentWorkMode() exit, mode=%d")
TRACE_MSG(MMISRVAUD_996_112_2_18_3_23_45_113,"[MMISRV]: Audio GetCreateMode() entry, req_data=0x%x, type_data=0x%x, is_mixing_enable=%d")
TRACE_MSG(MMISRVAUD_1011_112_2_18_3_23_45_114,"[MMISRV]: Audio GetCreateMode(), audio_req->type=%d")
TRACE_MSG(MMISRVAUD_1017_112_2_18_3_23_45_115,"[MMISRV]: Audio GetCreateMode(), service num is exeeded the limit!!!")
TRACE_MSG(MMISRVAUD_1106_112_2_18_3_23_45_116,"[MMISRV]: Audio GetCreateMode() exit, pri=%d, type=%d, mode=%d")
TRACE_MSG(MMISRVAUD_1119_112_2_18_3_23_45_117,"[MMISRV]: Audio IsCanCreateNewService() entry, req_data=0x%x, ext_data=0x%x")
TRACE_MSG(MMISRVAUD_1134_112_2_18_3_23_45_118,"[MMISRV]: Audio IsCanCreateNewService(), service exeed tha max value!!!")
TRACE_MSG(MMISRVAUD_1155_112_2_18_3_23_45_119,"[MMISRV]: Audio IsCanCreateNewService(), pri is lower than current!!!")
TRACE_MSG(MMISRVAUD_1182_112_2_18_3_23_45_120,"[MMISRV]: Audio IsCanCreateNewService(), entiry_ptr is NULL!!!")
TRACE_MSG(MMISRVAUD_1199_112_2_18_3_23_45_121,"[MMISRV]: Audio IsCanResume() entry, entity_ptr=0x%x")
TRACE_MSG(MMISRVAUD_1231_112_2_18_3_23_45_122,"[MMISRV]: Audio IsCanResume(), pri is lower than current!!!")
TRACE_MSG(MMISRVAUD_1251_112_2_18_3_23_45_123,"[MMISRV]: Audio IsCanResume(), error!!!! entity_active is NULL!")
TRACE_MSG(MMISRVAUD_1256_112_2_18_3_23_45_124,"[MMISRV]: Audio IsCanResume() exit, result=%d")
TRACE_MSG(MMISRVAUD_1372_112_2_18_3_23_46_125,"[MMISRV]: Audio IsNeedSetRoute(), entity_ptr=0x%x,type=%d,res=%d")
TRACE_MSG(MMISRVAUD_1376_112_2_18_3_23_46_126,"[MMISRV]: Audio IsNeedSetRoute(), entity_ptr=NULL, Error!!!")
TRACE_MSG(MMISRVAUD_1427_112_2_18_3_23_46_127,"[MMISRV]: Audio IsTypeSupportBt() exit, type=%d, fmt=%d, res=%d")
TRACE_MSG(MMISRVAUD_1444_112_2_18_3_23_46_128,"[MMISRV]: Audio GetShouldBeRoute() exit, entity_ptr is NULL!!!")
TRACE_MSG(MMISRVAUD_1449_112_2_18_3_23_46_129,"[MMISRV]: Audio GetShouldBeRoute() entry, entity_ptr=0x%x, all_support_route=%d, manual_route=%d, default_route=%d")
TRACE_MSG(MMISRVAUD_1538_112_2_18_3_23_46_130,"[MMISRV]: Audio GetShouldBeRoute() exit, the route should be %d")
TRACE_MSG(MMISRVAUD_1576_112_2_18_3_23_46_131,"[MMISRV]: Audio SwichRoute() entry, data=0x%x, s_bt_info.state=%d, is async=%d")
TRACE_MSG(MMISRVAUD_1614_112_2_18_3_23_46_132,"[MMISRV]: Audio SwichRoute() exit,old route=%d, new route=%d")
TRACE_MSG(MMISRVAUD_1663_112_2_18_3_23_46_133,"[MMISRV]: Audio SwichRoute() entry, data=0x%x")
TRACE_MSG(MMISRVAUD_1705_112_2_18_3_23_47_134,"[MMISRV]: Audio PauseAudio() entry, data=0x%x")
TRACE_MSG(MMISRVAUD_1711_112_2_18_3_23_47_135,"[MMISRV]: Audio PauseAudio(), drv_handle=0x%x, audio_state=%d")
TRACE_MSG(MMISRVAUD_1729_112_2_18_3_23_47_136,"[MMISRV]: Audio PauseAudioAndNotify() entry, data=0x%x")
TRACE_MSG(MMISRVAUD_1735_112_2_18_3_23_47_137,"[MMISRV]: Audio PauseAudioAndNotify(), drv_handle=0x%x, audio_state=%d, is_auto_free=%d")
TRACE_MSG(MMISRVAUD_1749_112_2_18_3_23_47_138,"[MMISRV]: Audio PauseAudioAndNotify(), vibrate cb=0x%x")
TRACE_MSG(MMISRVAUD_1788_112_2_18_3_23_47_139,"[MMISRV]: Audio ResumeAudio() entry, data=0x%x")
TRACE_MSG(MMISRVAUD_1794_112_2_18_3_23_47_140,"[MMISRV]: Audio ResumeAudio(), drv_handle=0x%x, audio_state=%d")
TRACE_MSG(MMISRVAUD_1825_112_2_18_3_23_47_141,"[MMISRV]: Audio SrvActiveSwitch() entry, entity=0x%x")
TRACE_MSG(MMISRVAUD_1854_112_2_18_3_23_47_142,"[MMISRV]: Audio SetAudioInfo() entry, entity_ptr=0x%x")
TRACE_MSG(MMISRVAUD_1862_112_2_18_3_23_47_143,"[MMISRV]: Audio SetAudioInfo(), old type=%d, new type=%d, service state=%d, audio state=%d")
TRACE_MSG(MMISRVAUD_1943_112_2_18_3_23_47_144,"[MMISRV]: Audio SetAudioInfo() exit, res=%d")
TRACE_MSG(MMISRVAUD_1983_112_2_18_3_23_47_145,"[MMISRV]: Audio SrvResumeOnly() entry, srv_ptr=0x%x")
TRACE_MSG(MMISRVAUD_2026_112_2_18_3_23_47_146,"[MMISRV]: Audio SrvResumeOnly() exit, resume res=%d")
TRACE_MSG(MMISRVAUD_2045_112_2_18_3_23_47_147,"[MMISRV]: Audio SrvCreate() entry, req_data=0x%x, ext_data=0x%x")
TRACE_MSG(MMISRVAUD_2078_112_2_18_3_23_48_148,"[MMISRV]: Audio SrvCreate(), Can't create new service!!!")
TRACE_MSG(MMISRVAUD_2084_112_2_18_3_23_48_149,"[MMISRV]: Audio SrvCreate(), Can't create new service with replace info!!!")
TRACE_MSG(MMISRVAUD_2097_112_2_18_3_23_48_150,"[MMISRV]: Audio SrvCreate(), driver create handle failed!!!")
TRACE_MSG(MMISRVAUD_2106_112_2_18_3_23_48_151,"[MMISRV]: Audio SrvCreate(), memery malloc failed!!!")
TRACE_MSG(MMISRVAUD_2112_112_2_18_3_23_48_152,"[MMISRV]: Audio SrvCreate(), start to create a new service")
TRACE_MSG(MMISRVAUD_2212_112_2_18_3_23_48_153,"[MMISRV]: Audio SrvCreate(), add to handle box failed!!!")
TRACE_MSG(MMISRVAUD_2228_112_2_18_3_23_48_154,"[MMISRV]: Audio SrvResume() entry, srv_ptr=0x%x")
TRACE_MSG(MMISRVAUD_2252_112_2_18_3_23_48_155,"[MMISRV]: Audio SrvResume() entry, srv_ptr=0x%x")
TRACE_MSG(MMISRVAUD_2308_112_2_18_3_23_48_156,"[MMISRV]: Audio SrvResume() exit, resume res=%d")
TRACE_MSG(MMISRVAUD_2323_112_2_18_3_23_48_157,"[MMISRV]: Audio SrvSuspend() entry, srv_ptr=0x%x")
TRACE_MSG(MMISRVAUD_2374_112_2_18_3_23_48_158,"[MMISRV]: Audio SrvSuspend() entry, entity_ptr->service.srv_state=%d, res=%d")
TRACE_MSG(MMISRVAUD_2392_112_2_18_3_23_48_159,"[MMISRV]: Audio SrvFree() entry, srv_ptr=0x%x")
TRACE_MSG(MMISRVAUD_2429_112_2_18_3_23_48_160,"[MMISRV]: Audio SrvFree(), srv free Ok! is_auto_resume_off=%d")
TRACE_MSG(MMISRVAUD_2434_112_2_18_3_23_48_161,"[MMISRV]: Audio SrvFree(), suspend_srv=%d, active service num=%d")
TRACE_MSG(MMISRVAUD_2455_112_2_18_3_23_48_162,"[MMISRV]: Audio SrvFree(), srv free from stack Ok!")
TRACE_MSG(MMISRVAUD_2459_112_2_18_3_23_48_163,"[MMISRV]: Audio SrvFree() exit, srv free Fail!")
TRACE_MSG(MMISRVAUD_2493_112_2_18_3_23_48_164,"[MMISRV]: Audio SrvSetPri() entry, srv_ptr=0x%x,pri=%d")
TRACE_MSG(MMISRVAUD_2515_112_2_18_3_23_48_165,"[MMISRV]: Audio SrvSetNotify() entry, srv_ptr=0x%x, notify=%d")
TRACE_MSG(MMISRVAUD_2583_112_2_18_3_23_49_166,"[MMISRV]: Audio SrvIoCtrl() entry, srv_ptr=0x%x, data=0x%x, ret_value=0x%x")
TRACE_MSG(MMISRVAUD_2593_112_2_18_3_23_49_167,"[MMISRV]: Audio SrvIoCtrl(), event_data->event=%d, drv_handle=0x%x, service_state=%d, aud_state=%d")
TRACE_MSG(MMISRVAUD_2842_112_2_18_3_23_49_168,"[MMISRV]: Audio SrvIoCtrl() exit, res=%d")
TRACE_MSG(MMISRVAUD_2860_112_2_18_3_23_49_169,"[MMISRV]: Audio SrvCmdFunc() entry, cmd=%d")
TRACE_MSG(MMISRVAUD_2893_112_2_18_3_23_49_170,"[MMISRV]: Audio SrvCmdFunc() ,ext cmd=%d")
TRACE_MSG(MMISRVAUD_3007_112_2_18_3_23_49_171,"[MMISRV]: Audio SrvCmdFunc() exit, res=%d")
TRACE_MSG(MMISRVAUD_3033_112_2_18_3_23_49_172,"[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, type=%d, i=%d, entity_ptr=0x%x")
TRACE_MSG(MMISRVAUD_3065_112_2_18_3_23_49_173,"[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, type%d, i=%d, entity_ptr=0x%x")
TRACE_MSG(MMISRVAUD_3110_112_2_18_3_23_50_174,"[MMISRV]: Audio MMISRVAUD_HandleAudioReport() entry, drv_handle=0x%x, rpt_data=0x%x")
TRACE_MSG(MMISRVAUD_3115_112_2_18_3_23_50_175,"[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), entity_ptr=0x%x,report=%d")
TRACE_MSG(MMISRVAUD_3124_112_2_18_3_23_50_176,"[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), left play times=%d")
TRACE_MSG(MMISRVAUD_3187_112_2_18_3_23_50_177,"[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), entity_ptr->service.notify=0x%x, type=%d")
TRACE_MSG(MMISRVAUD_3192_112_2_18_3_23_50_178,"[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), vibrate cb=0x%x")
TRACE_MSG(MMISRVAUD_3228_112_2_18_3_23_50_179,"[MMISRV]: Audio HandleBTCloseDelayTimer() entry, timer=%d,s_bt_info.timer=%d,s_bt_info.state=%d")
TRACE_MSG(MMISRVAUD_3253_112_2_18_3_23_50_180,"[MMISRV]: Audio HandleBTCloseDelayTimer(), Nothing to do in these state")
TRACE_MSG(MMISRVAUD_3268_112_2_18_3_23_50_181,"[MMISRV]: Audio IsBtNeedReOpen() entry, entity_ptr=0x%x, sample_rate=%d, cur_entity_ptr=0x%x, cur sample_rate=%d, s_bt_info.state=%d")
TRACE_MSG(MMISRVAUD_3274_112_2_18_3_23_50_182,"[MMISRV]: Audio IsBtNeedReOpen(), new type=%d, old type=%d")
TRACE_MSG(MMISRVAUD_3306_112_2_18_3_23_50_183,"[MMISRV]: Audio IsBtNeedReOpen() exit, res=%d")
TRACE_MSG(MMISRVAUD_3322_112_2_18_3_23_50_184,"[MMISRV]: Audio BtPrePlay() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d")
TRACE_MSG(MMISRVAUD_3344_112_2_18_3_23_50_185,"[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_NONE")
TRACE_MSG(MMISRVAUD_3361_112_2_18_3_23_50_186,"[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_OPEN_WAIT or BT_STATE_CLOSE_WAIT ")
TRACE_MSG(MMISRVAUD_3370_112_2_18_3_23_50_187,"[MMISRV]: Audio BtPrePlay(), error!!! BT should not be BT_STATE_PLAY")
TRACE_MSG(MMISRVAUD_3386_112_2_18_3_23_50_188,"[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_DELAY_TIMER_WAIT")
TRACE_MSG(MMISRVAUD_3409_112_2_18_3_23_50_189,"[MMISRV]: Audio BtPrePlay(), error!!! No this state!")
TRACE_MSG(MMISRVAUD_3414_112_2_18_3_23_50_190,"[MMISRV]: Audio BtPrePlay() exit, result=%d")
TRACE_MSG(MMISRVAUD_3429_112_2_18_3_23_50_191,"[MMISRV]: Audio BtPreResume() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d")
TRACE_MSG(MMISRVAUD_3471_112_2_18_3_23_50_192,"[MMISRV]: Audio BtPreResume(), error!!! BT state should not be BT_STATE_PLAY")
TRACE_MSG(MMISRVAUD_3508_112_2_18_3_23_50_193,"[MMISRV]: Audio BtPreResume() exit, result=%d")
TRACE_MSG(MMISRVAUD_3522_112_2_18_3_23_50_194,"[MMISRV]: Audio BtPrePause() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d")
TRACE_MSG(MMISRVAUD_3568_112_2_18_3_23_50_195,"[MMISRV]: Audio BtPrePause() exit, result=%d")
TRACE_MSG(MMISRVAUD_3583_112_2_18_3_23_51_196,"[MMISRV]: Audio BtPrePause() entry, entity_ptr=0x%x, data=0x%x, s_bt_info.state=%d")
TRACE_MSG(MMISRVAUD_3629_112_2_18_3_23_51_197,"[MMISRV]: Audio BtPrePause() exit, result=%d")
TRACE_MSG(MMISRVAUD_3644_112_2_18_3_23_51_198,"[MMISRV]: Audio BtActiveHandle() entry")
TRACE_MSG(MMISRVAUD_3680_112_2_18_3_23_51_199,"[MMISRV]: Audio BtDeActiveHandle() entry")
TRACE_MSG(MMISRVAUD_3703_112_2_18_3_23_51_200,"[MMISRV]: Audio BtOpenedHandle() entry, s_bt_info.state=%d, cur entity_addr=0x%x,event=0x%d,sample_rate=%d")
TRACE_MSG(MMISRVAUD_3705_112_2_18_3_23_51_201,"[MMISRV]: Audio BtOpenedHandle(), wait entity_addr=0x%x,event=0x%d,sample_rate=%d")
TRACE_MSG(MMISRVAUD_3777_112_2_18_3_23_51_202,"[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d")
TRACE_MSG(MMISRVAUD_3815_112_2_18_3_23_51_203,"[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d")
TRACE_MSG(MMISRVAUD_3821_112_2_18_3_23_51_204,"[MMISRV]: Audio BtOpenedHandle(), cur event error!! event=%d")
TRACE_MSG(MMISRVAUD_3910_112_2_18_3_23_51_205,"[MMISRV]: Audio BtOpenedHandle() exit, result=%d")
TRACE_MSG(MMISRVAUD_3929_112_2_18_3_23_51_206,"[MMISRV]: Audio BTClosedHandle() entry, s_bt_info.state=%d, cur entity_addr=0x%x,event=0x%d,sample_rate=%d")
TRACE_MSG(MMISRVAUD_3931_112_2_18_3_23_51_207,"[MMISRV]: Audio BTClosedHandle(), wait entity_addr=0x%x,event=0x%d,sample_rate=%d")
TRACE_MSG(MMISRVAUD_3993_112_2_18_3_23_51_208,"[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d")
TRACE_MSG(MMISRVAUD_4016_112_2_18_3_23_51_209,"[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d")
TRACE_MSG(MMISRVAUD_4022_112_2_18_3_23_51_210,"[MMISRV]: Audio BtOpenedHandle(), cur event error!! event=%d")
TRACE_MSG(MMISRVAUD_4049_112_2_18_3_23_51_211,"[MMISRV]: Audio BTClosedHandle() fatal error!! Entity should not be NULL")
TRACE_MSG(MMISRVAUD_4072_112_2_18_3_23_52_212,"[MMISRV]: Audio BTClosedHandle() error bt state, may BT module error!!!")
TRACE_MSG(MMISRVAUD_4097_112_2_18_3_23_52_213,"[MMISRV]: Audio BtNotifyAppEvent() error!!! handle=0")
TRACE_MSG(MMISRVAUD_4107_112_2_18_3_23_52_214,"[MMISRV]: Audio BtNotifyAppEvent() entry, route=%d")
TRACE_MSG(MMISRVAUD_4126_112_2_18_3_23_52_215,"[MMISRV]: Audio BtEventHandle() entry, event=%d")
TRACE_MSG(MMISRVAUD_4179_112_2_18_3_23_52_216,"[MMISRV]: Audio AudPlay() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d")
TRACE_MSG(MMISRVAUD_4238_112_2_18_3_23_52_217,"[MMISRV]: Audio AudPlay() exit, ret_val=%d")
TRACE_MSG(MMISRVAUD_4257_112_2_18_3_23_52_218,"[MMISRV]: Audio AudResume() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d")
TRACE_MSG(MMISRVAUD_4292_112_2_18_3_23_52_219,"[MMISRV]: Audio AudResume() exit, result=%d")
TRACE_MSG(MMISRVAUD_4311_112_2_18_3_23_52_220,"[MMISRV]: Audio AudPause() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d,is_change_state=%d")
TRACE_MSG(MMISRVAUD_4328_112_2_18_3_23_52_221,"[MMISRV]: Audio, Don't not surpport audio mixing now!!")
TRACE_MSG(MMISRVAUD_4348_112_2_18_3_23_52_222,"[MMISRV]: Audio AudPause() exit, result=%d")
TRACE_MSG(MMISRVAUD_4367_112_2_18_3_23_52_223,"[MMISRV]: Audio AudStop() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d")
TRACE_MSG(MMISRVAUD_4386_112_2_18_3_23_52_224,"[MMISRV]: Audio, Don't not surpport audio mixing now!!")
TRACE_MSG(MMISRVAUD_4413_112_2_18_3_23_52_225,"[MMISRV]: Audio AudStop() exit, result=%d")
TRACE_MSG(MMISRVAUD_4440_112_2_18_3_23_52_226,"[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, handle%d, i=%d, entity_ptr=0x%x")
TRACE_MSG(MMISRVAUD_DRV_58_112_2_18_3_23_53_227,"[MMISRV]aud drv: AudioDrvCallback() entry, hAudio=0x%x, notify_info=%d,affix_info=%d, END_IND=%d")
TRACE_MSG(MMISRVAUD_DRV_79_112_2_18_3_23_53_228,"[MMISRV]aud drv: VideoDrvCallback entry, i_type = %d")
TRACE_MSG(MMISRVAUD_DRV_98_112_2_18_3_23_53_229,"[MMISRV]aud drv: VideoDrvCallback entry, i_type = %d")
TRACE_MSG(MMISRVAUD_DRV_118_112_2_18_3_23_53_230,"[MMISRV]aud drv: MvDrvCallback entry, service_info_ptr = 0x%x")
TRACE_MSG(MMISRVAUD_DRV_167_112_2_18_3_23_53_231,"[MMISRV]aud drv: ConvertRoute(), route to device mode, route=%d, mode=%d")
TRACE_MSG(MMISRVAUD_DRV_203_112_2_18_3_23_53_232,"[MMISRV]aud drv: ConvertEQType() eq to ref eq, eq_mode=%d, ref_eq_mode=%d")
TRACE_MSG(MMISRVAUD_DRV_236_112_2_18_3_23_53_233,"[MMISRV]aud drv: ConvertDevMode() exit, mode=%d, route=%d")
TRACE_MSG(MMISRVAUD_DRV_322_112_2_18_3_23_53_234,"[MMISRV]aud drv: GetRingCodec() exit, ring_type=%d, codec=0x%x")
TRACE_MSG(MMISRVAUD_DRV_354_112_2_18_3_23_53_235,"[MMISRV]aud drv: GetToneCodec() exit, tone_type=%d, hAudioCodec=0x%x")
TRACE_MSG(MMISRVAUD_DRV_405_112_2_18_3_23_53_236,"[MMISRV]aud drv: GetAudioDevice() exit, is_a2dp=%d, ring_fmt=%d")
TRACE_MSG(MMISRVAUD_DRV_479_112_2_18_3_23_53_237,"[MMISRV]aud drv: GetAudioFmt() entry, type_ptr=0x%x")
TRACE_MSG(MMISRVAUD_DRV_518_112_2_18_3_23_54_238,"[MMISRV]aud drv: GetAudioFmt() exit, fmt=%d")
TRACE_MSG(MMISRVAUD_DRV_547_112_2_18_3_23_54_239,"[MMISRV]aud drv: CreateAudioBufferHandle() entry, is_a2dp=%d, ring_fmt=%d, pucData=0x%x, uiDataLenght=%d,buff=0x%x, size=%d")
TRACE_MSG(MMISRVAUD_DRV_576_112_2_18_3_23_54_240,"[MMISRV]aud drv: CreateAudioBufferHandle() exit,")
TRACE_MSG(MMISRVAUD_DRV_601_112_2_18_3_23_54_241,"[MMISRV]aud drv: CreateAudioFileHandle() entry")
TRACE_MSG(MMISRVAUD_DRV_630_112_2_18_3_23_54_242,"[MMISRV]aud drv: CreateAudioFileHandle() exit,")
TRACE_MSG(MMISRVAUD_DRV_657_112_2_18_3_23_54_243,"[MMISRV]aud drv: CreateAudioStreamingHandle() entry")
TRACE_MSG(MMISRVAUD_DRV_688_112_2_18_3_23_54_244,"[MMISRV]aud drv: CreateAudioStreamingHandle() exit,")
TRACE_MSG(MMISRVAUD_DRV_709_112_2_18_3_23_54_245,"[MMISRV]aud drv: CreateIQHandle() entry, name_ptr=0x%x,record_param=0x%x")
TRACE_MSG(MMISRVAUD_DRV_744_112_2_18_3_23_54_246,"[MMISRV]aud drv: CreateIQHandle() exit, handle=0x%x,result=%d")
TRACE_MSG(MMISRVAUD_DRV_810_112_2_18_3_23_54_247,"[MMISRV]aud drv: CreateRecordFileHandle() entry, src=%d, fmt=%d,frame_len=%d, file_handle=0x%x")
TRACE_MSG(MMISRVAUD_DRV_888_112_2_18_3_23_54_248,"[MMISRV]aud drv: CreateAspFileHandle() entry, is_a2dp=%d, ring_fmt=%d, name_ptr=0x%x, buff=0x%x, size=%d")
TRACE_MSG(MMISRVAUD_DRV_916_112_2_18_3_23_54_249,"[MMISRV]aud drv: CreateAspFileHandle() exit, handle=0x%x, result=%d")
TRACE_MSG(MMISRVAUD_DRV_938_112_2_18_3_23_54_250,"[MMISRV]aud drv: CreateAspBufferHandle() entry, is_a2dp=%d, ring_fmt=%d, pucData=0x%x, uiDataLenght=%d,buff=0x%x, size=%d")
TRACE_MSG(MMISRVAUD_DRV_967_112_2_18_3_23_54_251,"[MMISRV]aud drv: CreateAspBufferHandle() exit, handle=0x%x, result=%d")
TRACE_MSG(MMISRVAUD_DRV_1005_112_2_18_3_23_55_252,"[MMISRV]aud drv: DRVAUD_CreateHandle() entry, audio_data->type=%d")
TRACE_MSG(MMISRVAUD_DRV_1134_112_2_18_3_23_55_253,"[MMISRV]aud drv: Error!!! ATV param is exeeded the store memory!!!!!")
TRACE_MSG(MMISRVAUD_DRV_1147_112_2_18_3_23_55_254,"[MMISRV]aud drv: DRVAUD_CreateHandle() exit, handle=0x%x")
TRACE_MSG(MMISRVAUD_DRV_1192_112_2_18_3_23_55_255,"[MMISRV]aud drv: DRVAUD_CloseHandle() exit, handle=0x%x, result=%d")
TRACE_MSG(MMISRVAUD_DRV_1221_112_2_18_3_23_55_256,"[MMISRV]aud drv: VideoPlay() exit, handle=0x%x, result=%d, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1243_112_2_18_3_23_55_257,"[MMISRV]aud drv: VideoPause() exit, handle=0x%x, result=%d, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1265_112_2_18_3_23_55_258,"[MMISRV]aud drv: VideoResume() exit, handle=0x%x, result=%d, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1287_112_2_18_3_23_55_259,"[MMISRV]aud drv: VideoStop() exit, handle=0x%x, result=%d, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1299_112_2_18_3_23_55_260,"[MMISRV]: ParamCtrlCallBack")
TRACE_MSG(MMISRVAUD_DRV_1320_112_2_18_3_23_55_261,"[MMISRV]aud drv: AtvPlay(),set region, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1324_112_2_18_3_23_55_262,"[MMISRV]aud drv: AtvPlay(),set region, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1327_112_2_18_3_23_55_263,"[MMISRV]aud drv: AtvPlay(), preview, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1334_112_2_18_3_23_55_264,"[MMISRV]aud drv: AtvPlay() exit, error!!!param is null!")
TRACE_MSG(MMISRVAUD_DRV_1356_112_2_18_3_23_55_265,"[MMISRV]aud drv: AtvStop() exit, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1378_112_2_18_3_23_56_266,"[MMISRV]aud drv: AtvPause() exit, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1405_112_2_18_3_23_56_267,"[MMISRV]aud drv: AtvResume() exit, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1409_112_2_18_3_23_56_268,"[MMISRV]aud drv: AtvResume() exit, error!!! param is null!")
TRACE_MSG(MMISRVAUD_DRV_1430_112_2_18_3_23_56_269,"[MMISRV]aud drv: DRVAUD_StartReocrdATV() entry, param=0x%x")
TRACE_MSG(MMISRVAUD_DRV_1435_112_2_18_3_23_56_270,"[MMISRV]aud drv: DRVAUD_StartReocrdATV(), stop preview result=%d")
TRACE_MSG(MMISRVAUD_DRV_1439_112_2_18_3_23_56_271,"[MMISRV]aud drv: DRVAUD_StartReocrdATV(), start record result=%d")
TRACE_MSG(MMISRVAUD_DRV_1459_112_2_18_3_23_56_272,"[MMISRV]aud drv: DRVAUD_StopReocrdATV() entry, type_ptr=0x%x")
TRACE_MSG(MMISRVAUD_DRV_1462_112_2_18_3_23_56_273,"[MMISRV]aud drv: DRVAUD_StopReocrdATV(), stop record result=%d")
TRACE_MSG(MMISRVAUD_DRV_1474_112_2_18_3_23_56_274,"[MMISRV]aud drv: DRVAUD_StopReocrdATV(),set region, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1478_112_2_18_3_23_56_275,"[MMISRV]aud drv: DRVAUD_StopReocrdATV(),set region, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1481_112_2_18_3_23_56_276,"[MMISRV]aud drv: DRVAUD_StopReocrdATV(), preview, ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1487_112_2_18_3_23_56_277,"[MMISRV]aud drv: DRVAUD_StopReocrdATV(), start record result=%d")
TRACE_MSG(MMISRVAUD_DRV_1523_112_2_18_3_23_56_278,"[MMISRV]aud drv: MvPlay() entry, mv=0x%x")
TRACE_MSG(MMISRVAUD_DRV_1544_112_2_18_3_23_56_279,"[MMISRV]aud drv: MvStop() exit, OK")
TRACE_MSG(MMISRVAUD_DRV_1550_112_2_18_3_23_56_280,"[MMISRV]aud drv: MvStop() exit, error")
TRACE_MSG(MMISRVAUD_DRV_1563_112_2_18_3_23_56_281,"[MMISRV]aud drv: MvStop() exit, OK")
TRACE_MSG(MMISRVAUD_DRV_1569_112_2_18_3_23_56_282,"[MMISRV]aud drv: MvStop() exit, error")
TRACE_MSG(MMISRVAUD_DRV_1580_112_2_18_3_23_56_283,"[MMISRV]aud drv: MvPlay() entry, mv=0x%x")
TRACE_MSG(MMISRVAUD_DRV_1614_112_2_18_3_23_56_284,"[MMISRV]aud drv: BtVoicePlay() exit, no memory, error!!!")
TRACE_MSG(MMISRVAUD_DRV_1638_112_2_18_3_23_56_285,"[MMISRV]aud drv: BtVoicePlay() exit, play handle=0x%x, rec handle=0x%x, play_ret=%d, rec_ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1682_112_2_18_3_23_56_286,"[MMISRV]aud drv: BtVoiceStop() exit, play handle=0x%x, rec handle=0x%x, stop play_ret=%d, stop rec_ret=%d, close play_ret=%d, close rec_ret=%d")
TRACE_MSG(MMISRVAUD_DRV_1695_112_2_18_3_23_56_287,"[MMISRV]aud drv: DRVAUD_Play() entry, handle=0x%x, type_ptr=%d,offset=%d, route=%d")
TRACE_MSG(MMISRVAUD_DRV_1732_112_2_18_3_23_56_288,"[MMISRV]aud drv: DRVAUD_Play(), AUDIO_Play fail, res=%d")
TRACE_MSG(MMISRVAUD_DRV_1738_112_2_18_3_23_56_289,"[MMISRV]aud drv: DRVAUD_Play(), BtVoicePlay, res=%d")
TRACE_MSG(MMISRVAUD_DRV_1837_112_2_18_3_23_56_290,"[MMISRV]aud drv: DRVAUD_Play() exit, type=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_1858_112_2_18_3_23_56_291,"[MMISRV]aud drv: DRVAUD_PlayAsy() exit, handle=0x%x, offset=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_1877_112_2_18_3_23_57_292,"[MMISRV]aud drv: DRVAUD_PlayAsy() entry, handle=0x%x, type_ptr=0x%x, offset=%d")
TRACE_MSG(MMISRVAUD_DRV_1907_112_2_18_3_23_57_293,"[MMISRV]aud drv: DRVAUD_PlayAsy() exit, type=%d, res=%d")
TRACE_MSG(MMISRVAUD_DRV_1913_112_2_18_3_23_57_294,"[MMISRV]aud drv: DRVAUD_PlayAsy() exit, type=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_1967_112_2_18_3_23_57_295,"[MMISRV]aud drv: DRVAUD_Pause() exit, handle=0x%x, type_ptr=0x%x, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2021_112_2_18_3_23_57_296,"[MMISRV]aud drv: DRVAUD_Stop(), HFG_AudioStop, res=%d")
TRACE_MSG(MMISRVAUD_DRV_2029_112_2_18_3_23_57_297,"[MMISRV]aud drv: DRVAUD_Stop() exit, handle=0x%x,result=%d")
TRACE_MSG(MMISRVAUD_DRV_2082_112_2_18_3_23_57_298,"[MMISRV]aud drv: DRVAUD_Resume() exit, handle=0x%x, type_ptr=0x%s, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2102_112_2_18_3_23_57_299,"[MMISRV]aud drv: DRVAUD_PauseSlight() entry, handle=0x%x, type_ptr=0x%")
TRACE_MSG(MMISRVAUD_DRV_2114_112_2_18_3_23_57_300,"[MMISRV]aud drv: DRVAUD_PauseSlight(), error type!!!!! type=%d")
TRACE_MSG(MMISRVAUD_DRV_2118_112_2_18_3_23_57_301,"[MMISRV]aud drv: DRVAUD_PauseSlight() exit, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2136_112_2_18_3_23_57_302,"[MMISRV]aud drv: DRVAUD_ResumeSlight() entry, handle=0x%x, type_ptr=0x%")
TRACE_MSG(MMISRVAUD_DRV_2148_112_2_18_3_23_57_303,"[MMISRV]aud drv: DRVAUD_ResumeSlight(), error type!!!!! type=%d")
TRACE_MSG(MMISRVAUD_DRV_2152_112_2_18_3_23_57_304,"[MMISRV]aud drv: DRVAUD_ResumeSlight() exit, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2175_112_2_18_3_23_57_305,"[MMISRV]aud drv: DRVAUD_Seek() exit, handle=0x%x, offset=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2193_112_2_18_3_23_57_306,"[MMISRV]aud drv: DRVAUD_GetContentInfo() entry, handle=0x%x, content_info=%d")
TRACE_MSG(MMISRVAUD_DRV_2221_112_2_18_3_23_57_307,"[MMISRV]aud drv: DRVAUD_GetContentInfo() exit, ok!!!")
TRACE_MSG(MMISRVAUD_DRV_2227_112_2_18_3_23_57_308,"[MMISRV]aud drv: DRVAUD_GetContentInfo() exit, error!!!")
TRACE_MSG(MMISRVAUD_DRV_2240_112_2_18_3_23_57_309,"[MMISRV]aud drv: DRVAUD_GetPlayingInfo() entry, handle=0x%x, play_info=0x%x")
TRACE_MSG(MMISRVAUD_DRV_2253_112_2_18_3_23_57_310,"[MMISRV]aud drv: DRVAUD_GetPlayingInfo() exit, ok!!! current time=%d, total time=%d")
TRACE_MSG(MMISRVAUD_DRV_2258_112_2_18_3_23_57_311,"[MMISRV]aud drv: DRVAUD_GetPlayingInfo() exit, error!!!")
TRACE_MSG(MMISRVAUD_DRV_2275_112_2_18_3_23_57_312,"[MMISRV]aud drv: DRVAUD_GetRecordInfo() entry, handle=0x%x, record_info")
TRACE_MSG(MMISRVAUD_DRV_2291_112_2_18_3_23_57_313,"[MMISRV]aud drv: DRVAUD_GetRecordInfo() exit, total_time=%d")
TRACE_MSG(MMISRVAUD_DRV_2296_112_2_18_3_23_57_314,"[MMISRV]aud drv: DRVAUD_GetRecordInfo() exit, error!!!")
TRACE_MSG(MMISRVAUD_DRV_2313_112_2_18_3_23_57_315,"[MMISRV]aud drv: DRVAUD_GetRecordSize() entry, handle=0x%x, record_info")
TRACE_MSG(MMISRVAUD_DRV_2329_112_2_18_3_23_57_316,"[MMISRV]aud drv: DRVAUD_GetRecordSize() exit, total_time=%d")
TRACE_MSG(MMISRVAUD_DRV_2334_112_2_18_3_23_57_317,"[MMISRV]aud drv: DRVAUD_GetRecordSize() exit, error!!!")
TRACE_MSG(MMISRVAUD_DRV_2351_112_2_18_3_23_57_318,"[MMISRV]aud drv: DRVAUD_SetPlaySpeed() exit, handle=0x%x, speed=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2390_112_2_18_3_23_58_319,"[MMISRV]aud drv: DRVAUD_SetEQMode() exit, handle=0x%x, eq_mode=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2404_112_2_18_3_23_58_320,"[MMISRV]aud drv: DRVAUD_SetVolume() entry, handle=0x%x, volume=%d")
TRACE_MSG(MMISRVAUD_DRV_2408_112_2_18_3_23_58_321,"[MMISRV]aud drv: DRVAUD_SetVolume() exit, don't know the mute state!")
TRACE_MSG(MMISRVAUD_DRV_2435_112_2_18_3_23_58_322,"[MMISRV]aud drv: DRVAUD_SetVolume() exit, is_cur_mute=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2457_112_2_18_3_23_58_323,"[MMISRV]aud drv: DRVAUD_GetVolume() exit, handle=0x%x, volume=0x%x, vol=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2484_112_2_18_3_23_58_324,"[MMISRV]aud drv: DRVAUD_SetRoute() exit, error! handle=0x%x, route=0x%x, type_ptr=%d")
TRACE_MSG(MMISRVAUD_DRV_2511_112_2_18_3_23_58_325,"[MMISRV]aud drv: DRVAUD_SetRoute() exit, handle=0x%x, route=0x%x, set_dev_mode=%d, cur_dev_mode=%d")
TRACE_MSG(MMISRVAUD_DRV_2535_112_2_18_3_23_58_326,"[MMISRV]aud drv: DRVAUD_SetRoute() exit, error! handle=0x%x, route=%d, type_ptr=0x%x")
TRACE_MSG(MMISRVAUD_DRV_2571_112_2_18_3_23_58_327,"[MMISRV]aud drv: DRVAUD_SetRoute(), MMISRVAUD_TYPE_ATV type, switch result=%d, hDevice=0x%x")
TRACE_MSG(MMISRVAUD_DRV_2596_112_2_18_3_23_58_328,"[MMISRV]aud drv: DRVAUD_SetRoute(), normal audio, switch result=%d, hDevice=0x%x")
TRACE_MSG(MMISRVAUD_DRV_2636_112_2_18_3_23_58_329,"[MMISRV]aud drv: DRVAUD_SetRoute() exit, handle=0x%x, route=%d, set_dev_mode=%d, cur_dev_mode=%d")
TRACE_MSG(MMISRVAUD_DRV_2653_112_2_18_3_23_58_330,"[MMISRV]aud drv: DRVAUD_GetRoute() exit, handle=0x%x, route=%d, cur_dev_mode=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2679_112_2_18_3_23_58_331,"[MMISRV]aud drv: DRVAUD_GetAudCfg() exit, handle=0x%x, cfg=0x%x")
TRACE_MSG(MMISRVAUD_DRV_2711_112_2_18_3_23_58_332,"[MMISRV]aud drv: DRVAUD_GetAudCfg() exit, handle=0x%x, info=0x%x, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2735_112_2_18_3_23_58_333,"[MMISRV]aud drv: DRVAUD_OpenSpectrum() exit, handle=0x%x,result=%d")
TRACE_MSG(MMISRVAUD_DRV_2759_112_2_18_3_23_58_334,"[MMISRV]aud drv: DRVAUD_CloseSpectrum() exit, handle=0x%x,result=%d")
TRACE_MSG(MMISRVAUD_DRV_2783_112_2_18_3_23_58_335,"[MMISRV]aud drv: DRVAUD_SetVoiceLB() exit, handle=0x%x,is_enable=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2816_112_2_18_3_23_58_336,"[MMISRV]aud drv: DRVAUD_SetMute() exit, handle=0x%x,is_mute=%d, is_cur_mute=%d,result=%d")
TRACE_MSG(MMISRVAUD_DRV_2838_112_2_18_3_23_58_337,"[MMISRV]aud drv: DRVAUD_GetMute() exit, handle=0x%x,is_mute=0x%x, is_cur_mute=%d,result=%d")
TRACE_MSG(MMISRVAUD_DRV_2861_112_2_18_3_23_58_338,"[MMISRV]aud drv: DRVAUD_SetUplinkMute() exit, handle=0x%x,is_mute=%d, result=%d")
TRACE_MSG(MMISRVAUD_DRV_2884_112_2_18_3_23_59_339,"[MMISRV]aud drv:DRVAUD_SetPlayRate entry, handle=0x%x,rate =%d")
TRACE_MSG(MMISRVAUD_DRV_2897_112_2_18_3_23_59_340,"[MMISRV]aud drv: DRVAUD_SetPlayRate() exit, handle=0x%x,rate=%d,play_rate=%d,result=%d")
TRACE_MSG(MMISRVAUD_DRV_2921_112_2_18_3_23_59_341,"[MMISRV]aud drv:DRVAUD_GetRecordDb entry, handle=0x%x, db=-0x%x")
TRACE_MSG(MMISRVAUD_DRV_2939_112_2_18_3_23_59_342,"[MMISRV]aud drv: DRVAUD_GetRecordDb exit, res=%d,db =%d!")
TRACE_MSG(MMISRVAUD_DRV_2959_112_2_18_3_23_59_343,"[MMISRV]: Audio MMISRVAUD_HandleCallBackMsg() entry, callback_info_ptr=0x%x")
TRACE_MSG(MMISRVAUD_DRV_3025_112_2_18_3_23_59_344,"[MMISRV]: Audio MMISRVAUD_HandleCallBackMsg() exit, handle=0x%x, result=%d, notify_info=%d,affix_info=%d")
TRACE_MSG(MMISRVAUD_UTIL_59_112_2_18_3_23_59_345,"[MMISRV]: Audio GetLinkHeadByType() entry, link_type=%d")
TRACE_MSG(MMISRVAUD_UTIL_96_112_2_18_3_23_59_346,"[MMISRV]: Audio MMISRVAUD_LinkCreate() entry")
TRACE_MSG(MMISRVAUD_UTIL_123_112_2_18_3_23_59_347,"[MMISRV]: Audio MMISRVAUD_LinkCreate() exit!!!")
TRACE_MSG(MMISRVAUD_UTIL_139_112_2_18_3_23_59_348,"[MMISRV]: Audio MMISRVAUD_LinkPush() entry, link_type=%d,data=%d,link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_171_112_2_18_3_23_59_349,"[MMISRV]: Audio MMISRVAUD_LinkPop() entry, link_type=%d,link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_185_112_2_18_3_23_59_350,"[MMISRV]: Audio MMISRVAUD_LinkPop() exit, data=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_201_112_2_18_3_23_59_351,"[MMISRV]: Audio MMISRVAUD_LinkAppend() entry, link_type=%d,data=%d,link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_233_112_2_18_3_23_59_352,"[MMISRV]: Audio MMISRVAUD_LinkDeAppend() entry, link_type=%d,link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_247_112_2_18_3_23_59_353,"[MMISRV]: Audio MMISRVAUD_LinkDeAppend() exit, data=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_264_112_2_18_3_23_59_354,"[MMISRV]: Audio MMISRVAUD_LinkGetHeadNode() entry, link_type=%d,link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_279_112_2_18_3_24_0_355,"[MMISRV]: Audio MMISRVAUD_LinkGetHeadNode() exit, data=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_296_112_2_18_3_24_0_356,"[MMISRV]: Audio MMISRVAUD_LinkGetTailNode() entry, link_type=%d,link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_309_112_2_18_3_24_0_357,"[MMISRV]: Audio MMISRVAUD_LinkGetTailNode() exit, data=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_342_112_2_18_3_24_0_358,"[MMISRV]: Audio MMISRVAUD_LinkGetNum() entry, link_type=%d,link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_357_112_2_18_3_24_0_359,"[MMISRV]: Audio MMISRVAUD_LinkGetNum() entry, num=%d")
TRACE_MSG(MMISRVAUD_UTIL_373_112_2_18_3_24_0_360,"[MMISRV]: Audio MMISRVAUD_LinkGetAndDelete() entry, link_type=%d,data=0x%x, link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_385_112_2_18_3_24_0_361,"[MMISRV]: Audio MMISRVAUD_LinkGetAndDelete() exit, found! link_node=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_407_112_2_18_3_24_0_362,"[MMISRV]: Audio MMISRVAUD_LinkGetNode() entry, link_type=%d,data=0x%x, link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_435_112_2_18_3_24_0_363,"[MMISRV]: Audio MMISRVAUD_LinkSuspend() entry, active link_head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_443_112_2_18_3_24_0_364,"[MMISRV]: Audio MMISRVAUD_LinkSuspend(), new active link head=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_459_112_2_18_3_24_0_365,"[MMISRV]: Audio MMISRVAUD_LinkResume() entry, active link_head=0x%x,num=%d,data=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_513_112_2_18_3_24_0_366,"[MMISRV]: Audio MMISRVAUD_LinkTravel() entry, link_type=%d, link_head=0x%x, func=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_541_112_2_18_3_24_0_367,"[MMISRV]: Audio MMISRVAUD_LinkTravel() entry, link_type=%d, link_head=0x%x, func=0x%x")
TRACE_MSG(MMISRVAUD_UTIL_577_112_2_18_3_24_0_368,"[MMISRV]: Audio, MMISRVAUD_StackPush entry, data=0x%x, stack_num=%d")
TRACE_MSG(MMISRVAUD_UTIL_581_112_2_18_3_24_0_369,"[MMISRV]: Audio, MMISRVAUD_StackPush, active link num=%d")
TRACE_MSG(MMISRVAUD_UTIL_592_112_2_18_3_24_0_370,"[MMISRV]: Audio, stack is full!!!")
TRACE_MSG(MMISRVAUD_UTIL_605_112_2_18_3_24_0_371,"[MMISRV]: Audio, MMISRVAUD_StackPop entry, s_aud_stack.num=%d")
TRACE_MSG(MMISRVAUD_UTIL_618_112_2_18_3_24_0_372,"[MMISRV]: Audio, MMISRVAUD_StackPop, success!!")
TRACE_MSG(MMISRVAUD_UTIL_626_112_2_18_3_24_0_373,"[MMISRV]: Audio, MMISRVAUD_StackPop, this link is empty!!!")
TRACE_MSG(MMISRVAUD_UTIL_629_112_2_18_3_24_0_374,"[MMISRV]: Audio, MMISRVAUD_StackPop exit, data=0x%x, s_aud_stack.num=%d")
TRACE_MSG(MMISRVAUD_UTIL_644_112_2_18_3_24_0_375,"[MMISRV]: Audio, MMISRVAUD_StackGetLink entry, data=0x%x, s_aud_stack.num=%d")
TRACE_MSG(MMISRVAUD_UTIL_669_112_2_18_3_24_0_376,"[MMISRV]: Audio, MMISRVAUD_StackGetLink,Found! i=%d")
TRACE_MSG(MMISRVAUD_UTIL_683_112_2_18_3_24_0_377,"[MMISRV]: Audio, MMISRVAUD_StackGetLink, Not Found!")
TRACE_MSG(MMISRVAUD_UTIL_700_112_2_18_3_24_0_378,"[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete entry, data=0x%x, s_aud_stack.num=%d")
TRACE_MSG(MMISRVAUD_UTIL_725_112_2_18_3_24_0_379,"[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete, current link is empty!!!")
TRACE_MSG(MMISRVAUD_UTIL_733_112_2_18_3_24_0_380,"[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete, current is not top! i=%d")
TRACE_MSG(MMISRVAUD_UTIL_751_112_2_18_3_24_0_381,"[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete exit, i=%, res=%d")
TRACE_MSG(MMISRVAUD_UTIL_780_112_2_18_3_24_1_382,"[MMISRV]: Audio, MMISRVAUD_IsHeadSetPlugIn =%d")
TRACE_MSG(MMISRVAUD_UTIL_797_112_2_18_3_24_1_383,"[MMISRV]: Audio, MMISRVAUD_IsBTHeadSetActive =%d")
TRACE_MSG(MMISRVAUD_UTIL_819_112_2_18_3_24_1_384,"[MMISRV]: Audio, MMISRVAUD_CreateTimer entry, time_out=%d")
TRACE_MSG(MMISRVAUD_UTIL_832_112_2_18_3_24_1_385,"[MMISRV]: Audio, MMISRVAUD_StopTimer entry, timer_id=%d")
TRACE_MSG(MMISRVAUD_UTIL_849_112_2_18_3_24_1_386,"[MMISRV]: Audio, MMISRVAUD_BtOpen entry, type_ptr=0x%x,sample_rate=%d")
TRACE_MSG(MMISRVAUD_UTIL_874_112_2_18_3_24_1_387,"[MMISRV]: Audio, MMISRVAUD_BtOpen exit, result=%d,type=%d")
TRACE_MSG(MMISRVAUD_UTIL_891_112_2_18_3_24_1_388,"[MMISRV]: Audio, MMISRVAUD_BtClose entry")
TRACE_MSG(MMISRVAUD_UTIL_911_112_2_18_3_24_1_389,"[MMISRV]: Audio, MMISRVAUD_BtClose exit, result=%d")
TRACE_MSG(MMISRVMGR_113_112_2_18_3_24_3_390,"[MMISRV]: MGR MMISRVMGR_Request() entry, srv_name=0x%x, req_data=0x%x,ext_data=0x%x")
TRACE_MSG(MMISRVMGR_135_112_2_18_3_24_3_391,"[MMISRV]: MGR MMISRVMGR_Request() exit, handle=0x%x,cmd_func=0x%x")
TRACE_MSG(MMISRVMGR_149_112_2_18_3_24_3_392,"[MMISRV]: MGR MMISRVMGR_Free() entry, handle=0x%x")
TRACE_MSG(MMISRVMGR_161_112_2_18_3_24_3_393,"[MMISRV]: MGR MMISRVMGR_Free() exit, res=%d")
TRACE_MSG(MMISRVMGR_175_112_2_18_3_24_3_394,"[MMISRV]: MGR MMISRVMGR_Suspend() entry, handle=0x%x")
TRACE_MSG(MMISRVMGR_187_112_2_18_3_24_3_395,"[MMISRV]: MGR MMISRVMGR_Suspend() exit, res=%d")
TRACE_MSG(MMISRVMGR_201_112_2_18_3_24_3_396,"[MMISRV]: MGR MMISRVMGR_Resume() entry, handle=0x%x")
TRACE_MSG(MMISRVMGR_213_112_2_18_3_24_3_397,"[MMISRV]: MGR MMISRVMGR_Resume() exit, res=%d")
TRACE_MSG(MMISRVMGR_227_112_2_18_3_24_3_398,"[MMISRV]: MGR MMISRVMGR_SetPri() entry,handle =0x%x pri=%d")
TRACE_MSG(MMISRVMGR_239_112_2_18_3_24_3_399,"[MMISRV]: MGR MMISRVMGR_SetPri() exit, res=%d")
TRACE_MSG(MMISRVMGR_253_112_2_18_3_24_3_400,"[MMISRV]: MGR MMISRVMGR_SetNotify() entry,handle =0x%x notify=0x%x")
TRACE_MSG(MMISRVMGR_265_112_2_18_3_24_3_401,"[MMISRV]: MGR MMISRVMGR_SetNotify() exit, res=%d")
END_TRACE_MAP(MMI_SERVICE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_SERVICE_TRC_H_

