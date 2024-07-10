/******************************************************************************
 ** File Name:      mmi_app_otapush_trc.h                                         *
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
//trace_id:204
#ifndef _MMI_APP_OTAPUSH_TRC_H_
#define _MMI_APP_OTAPUSH_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIOTA_FILE_122_112_2_18_2_42_36_0 "ffs_type=%d"
#define MMIOTA_MAIN_2191_112_2_18_2_42_41_1 "MMIOTA_InitSettingMap"
#define MMIOTA_MAIN_2202_112_2_18_2_42_41_2 "MMIOTA_InitSettingMap all memery failed!"
#define MMIOTA_MAIN_2330_112_2_18_2_42_41_3 "MMIOTA_InitSettingMap all memery failed!"
#define MMIOTA_MAIN_2350_112_2_18_2_42_41_4 "MMIOTA_InitSettingMap all memery failed!"
#define MMIOTA_MAIN_3412_112_2_18_2_42_44_5 "MMIOTA_ParseWBXML DCD 2"
#define MMIOTA_MAIN_3452_112_2_18_2_42_44_6 "MMIAPIOTA_ParseWBXML all memery failed!"
#define MMIOTA_MAIN_3528_112_2_18_2_42_44_7 "dcd is wap push entered"
#define MMIOTA_WINTABLE_551_112_2_18_2_42_46_8 "[MMISMS]: HandleSIMPlugNotifyOTAFunc dual_sys=%d, notify_event=%d"
#define MMIOTA_WINTABLE_602_112_2_18_2_42_46_9 "MMIOTA_FreePushList Sms index=%d"
#define MMIOTA_WINTABLE_641_112_2_18_2_42_46_10 "MMIOTA_ReadOtaFile IN -------"
#define MMIOTA_WINTABLE_645_112_2_18_2_42_46_11 "fnh ota.cnf is exist!"
#define MMIOTA_WINTABLE_647_112_2_18_2_42_46_12 "fnh cur_hefs = %d"
#define MMIOTA_WINTABLE_655_112_2_18_2_42_46_13 "fnh read ota file  size is zero!"
#define MMIOTA_WINTABLE_663_112_2_18_2_42_46_14 "fnh malloc param_read_ptr fail!"
#define MMIOTA_WINTABLE_670_112_2_18_2_42_46_15 "fnh size is %d"
#define MMIOTA_WINTABLE_675_112_2_18_2_42_46_16 "fnh malloc param_read_ptr->buf_ptr fail!"
#define MMIOTA_WINTABLE_689_112_2_18_2_42_46_17 "fnh after FILE_Read returnvalue = %d"
#define MMIOTA_WINTABLE_708_112_2_18_2_42_46_18 "FNH THE ota FILE IS failed!"
#define MMIOTA_WINTABLE_714_112_2_18_2_42_46_19 "FNH THE ota FILE IS not exist!"
#define MMIOTA_WINTABLE_731_112_2_18_2_42_46_20 "MMIOTA_ReadOtaItem IN ----"
#define MMIOTA_WINTABLE_805_112_2_18_2_42_46_21 "[MMIMMS]:MMIAPIOTA_AddOTAPUSH push sender is in black lists!"
#define MMIOTA_WINTABLE_811_112_2_18_2_42_46_22 "[MMIMMS]:MMIAPIOTA_AddOTAPUSH sender_sr_len is d%!"
#define MMIOTA_WINTABLE_815_112_2_18_2_42_46_23 "[MMIMMS]:MMIAPIOTA_AddOTAPUSH sender_ptr is PNULL!"
#define MMIOTA_WINTABLE_932_112_2_18_2_42_47_24 "MMIOTA_otapush error!"
#define MMIOTA_WINTABLE_942_112_2_18_2_42_47_25 "MMIOTA_otapush alert"
#define MMIOTA_WINTABLE_1042_112_2_18_2_42_47_26 "MMIOTA_OTAUpdateState,id=%d"
#define MMIOTA_WINTABLE_1103_112_2_18_2_42_47_27 "MMIOTA_InsertOTAToSms,index=%d"
#define MMIOTA_WINTABLE_1193_112_2_18_2_42_47_28 "MMIOTA_StoreOTAPUSH  IN"
#define MMIOTA_WINTABLE_1216_112_2_18_2_42_47_29 "fnh param_save_ptr malloc fail"
#define MMIOTA_WINTABLE_1245_112_2_18_2_42_47_30 "fnh MMIMMS_SaveOTAPASHFile efs_error is %d"
#define MMIOTA_WINTABLE_1257_112_2_18_2_42_47_31 "VC:DeleteVideoFile FFS_AYS_Delete = %d"
#define MMIOTA_WINTABLE_1272_112_2_18_2_42_47_32 "VC: MMIVC_CallBackDeleteMMSFile run"
#define MMIOTA_WINTABLE_1275_112_2_18_2_42_47_33 "MMIOTA_StoreOTAPUSH OUT -----------"
#define MMIOTA_WINTABLE_1412_112_2_18_2_42_48_34 "SaveOTASettingValue dual_sys=%d"
#define MMIOTA_WINTABLE_1614_112_2_18_2_42_48_35 "HandleMMSConfigInfoDetailWinMsg"
#define MMIOTA_WINTABLE_1633_112_2_18_2_42_48_36 "cur_sel=%d"
#define MMIOTA_WINTABLE_2069_112_2_18_2_42_49_37 "malloc Detail_FormConfigInfo failed"
#define MMIOTA_WINTABLE_2088_112_2_18_2_42_49_38 "Detail_FormConfigInfo  label_string2.wstr_ptr alloc failed"
#define MMIOTA_WINTABLE_2097_112_2_18_2_42_49_39 "Detail_FormConfigInfo  str_dst->wstr_ptr alloc failed"
#define MMIOTA_WINTABLE_2139_112_2_18_2_42_49_40 "HandleOTAConfigSetOptionWinMsg() msg_id=%d"
#define MMIOTA_WINTABLE_2199_112_2_18_2_42_49_41 "warning i>OTA_CONFIG_INFO_NUM_MAX"
#define MMIOTA_WINTABLE_2245_112_2_18_2_42_49_42 "save OTA config setting_type=%d"
#define MMIOTA_WINTABLE_2280_112_2_18_2_42_49_43 "save bookmark!!"
#define MMIOTA_WINTABLE_2290_112_2_18_2_42_49_44 "warning i>OTA_CONFIG_INFO_NUM_MAX"
#define MMIOTA_WINTABLE_2470_112_2_18_2_42_50_45 "HandleMMSConfigInfoDetailWinMsg"
#define MMIOTA_WINTABLE_2486_112_2_18_2_42_50_46 "cur_sel=%d"
#define MMIOTA_WINTABLE_2714_112_2_18_2_42_50_47 "MMIOTA_DealWapPush() IN"
#define MMIOTA_WINTABLE_2721_112_2_18_2_42_50_48 "MMIOTA_DealWapPush() execute_action = %d"
#define MMIOTA_WINTABLE_2756_112_2_18_2_42_50_49 "MMIOTA_DealWapPush() signal_action = %d"
#define MMIOTA_WINTABLE_2759_112_2_18_2_42_50_50 "MMIOTA_DealWapPush() MMIOTA_SI_SIGNAL_ACTION_NONE"
#define MMIOTA_WINTABLE_2769_112_2_18_2_42_50_51 "MMIOTA_DealWapPush() PNULL = g_push_info_ptr"
#define MMIOTA_WINTABLE_2781_112_2_18_2_42_50_52 "MMIOTA_DealWapPush delete index =%d"
#define MMIOTA_WINTABLE_2821_112_2_18_2_42_50_53 "MMIOTA_DealWapPush() co"
#define MMIOTA_WINTABLE_2829_112_2_18_2_42_50_54 "MMIOTA_DealWapPush() error"
#define MMIOTA_WINTABLE_2834_112_2_18_2_42_50_55 "MMIOTA_DealWapPush() save"
#define MMIOTA_WINTABLE_2841_112_2_18_2_42_50_56 "MMIOTA_DealWapPush() PNULL == g_push_info_ptr"
#define MMIOTA_WINTABLE_2915_112_2_18_2_42_51_57 "g_cmsbrw_pushmsg_info.item_detail[index].time=%d"
#define MMIOTA_WINTABLE_2928_112_2_18_2_42_51_58 "MMIOTA_DealWapPush() alert"
#define MMIOTA_WINTABLE_2982_112_2_18_2_42_51_59 "MMIOTA_DealWapPush() read si"
#define MMIOTA_WINTABLE_2996_112_2_18_2_42_51_60 "MMIOTA_DealWapPush() enter broswer"
#define MMIOTA_WINTABLE_3005_112_2_18_2_42_51_61 "MMIOTA_DealWapPush() succed"
#define MMIOTA_WINTABLE_3033_112_2_18_2_42_51_62 "item item_num %d"
#define MMIOTA_WINTABLE_3040_112_2_18_2_42_51_63 "MMICMS_WriteDataToItemStruct() maxnum>=MMICMSBRW_MAX_ITEM_NUM"
#define MMIOTA_WINTABLE_3063_112_2_18_2_42_51_64 "the latest listitem %d "
#define MMIOTA_WINTABLE_3071_112_2_18_2_42_51_65 "MMICMS_WriteDataToItemStruct() maxnum>=MMICMSBRW_MAX_ITEM_NUM index=%d"
#define MMIOTA_WINTABLE_3085_112_2_18_2_42_51_66 "MMICMS_WriteDataToItemStruct() maxnum<MMICMSBRW_MAX_ITEM_NUM"
#define MMIOTA_WINTABLE_3098_112_2_18_2_42_51_67 "MMICMS_WriteDataToItemStruct() index=%d"
#define MMIOTA_WINTABLE_3169_112_2_18_2_42_51_68 "MMIAPIBRW_InsertWapPushToSms,index=%d"
#define MMIOTA_WINTABLE_3190_112_2_18_2_42_51_69 "time=%d,cur_item->time=%d"
#define MMIOTA_WINTABLE_3243_112_2_18_2_42_51_70 "MMIOTA_WriteWapPushFile() in!filelen=%d"
#define MMIOTA_WINTABLE_3244_112_2_18_2_42_51_71 "MMIOTA_WriteWapPushFile:flash_filename=%s"
#define MMIOTA_WINTABLE_3258_112_2_18_2_42_51_72 "MMIOTA_WriteWapPushFile,create cur_hefs1"
#define MMIOTA_WINTABLE_3262_112_2_18_2_42_51_73 "MMIOTA_WriteWapPushFilecreate cur_hefs2=%d"
#define MMIOTA_WINTABLE_3263_112_2_18_2_42_51_74 "return fail"
#define MMIOTA_WINTABLE_3278_112_2_18_2_42_51_75 "MMIOTA_WriteWapPushFile error=%d"
#define MMIOTA_WINTABLE_3288_112_2_18_2_42_51_76 "VC:DeleteVideoFile FFS_AYS_Delete = %d"
#define MMIOTA_WINTABLE_3332_112_2_18_2_42_51_77 "MMIMMS_HandleNewMsgWin(),OPEN"
#define MMIOTA_WINTABLE_3411_112_2_18_2_42_52_78 "MMIOTA_WapPushEnter,ID=%d"
#define MMIOTA_WINTABLE_3414_112_2_18_2_42_52_79 "MMIOTA_WapPushEnter,ID is too large"
#define MMIOTA_WINTABLE_3454_112_2_18_2_42_52_80 "MMIAPIOTA_WapPushUpdateState,ID=%d"
#define MMIOTA_WINTABLE_3474_112_2_18_2_42_52_81 "MMIAPIOTA_WapPushUpdateState,folder_type=%d"
#define MMIOTA_WINTABLE_3567_112_2_18_2_42_52_82 "MMIAPIBRW_InsertWapPushToSms PNULL == g_push_info_ptr"
#define MMIOTA_WINTABLE_3575_112_2_18_2_42_52_83 "MMIAPIBRW_InsertWapPushToSms list_num=%d"
#define MMIOTA_WINTABLE_3582_112_2_18_2_42_52_84 "MMIAPIBRW_InsertWapPushToSms time is zero!"
#define MMIOTA_WINTABLE_3610_112_2_18_2_42_52_85 "MMIAPIOTA_DelPushByID ID =%d, totalnum = %d"
#define MMIOTA_WINTABLE_3620_112_2_18_2_42_52_86 "MMIAPIOTA_DelPushByID index =%d"
#define MMIOTA_WINTABLE_3639_112_2_18_2_42_52_87 "g_cmsbrw_pushmsg_info.item_num=%d"
#define MMIOTA_WINTABLE_3679_112_2_18_2_42_52_88 "MMIBRW_WriteFile_Ex,file_type=%d"
#define MMIOTA_WINTABLE_3682_112_2_18_2_42_52_89 "wrong param"
#define MMIOTA_WINTABLE_3693_112_2_18_2_42_52_90 "cyz create file failed"
#define MMIOTA_WINTABLE_3697_112_2_18_2_42_52_91 "MMICMSWAP_FILE_Create failed"
#define MMIOTA_WINTABLE_3708_112_2_18_2_42_52_92 "cyz write file error=%d"
#define MMIOTA_WINTABLE_3746_112_2_18_2_42_52_93 "MMIOTA_FileOpCbf,efs_err=%d,buf_size=%d,result=%d"
#define MMIOTA_WINTABLE_3755_112_2_18_2_42_52_94 "cyz file_type=%d"
#define MMIOTA_WINTABLE_3756_112_2_18_2_42_52_95 "param addr=%d"
#define MMIOTA_WINTABLE_3779_112_2_18_2_42_52_96 "MMIAPIOTA_GetPushLockFlag record_id ERROR = %d"
#define MMIOTA_WINTABLE_3904_112_2_18_2_42_53_97 "HandlePushmsgWinMsg() msg_id=%d"
#define MMIOTA_WINTABLE_4040_112_2_18_2_42_53_98 "HandlePushmsgMenuWinMsg() msg_id=%d"
#define MMIOTA_WINTABLE_4051_112_2_18_2_42_53_99 "HandlePushmsgMenuWinMsg() open"
#define MMIOTA_WINTABLE_4054_112_2_18_2_42_53_100 "HandlePushmsgMenuWinMsg() true"
#define MMIOTA_WINTABLE_4059_112_2_18_2_42_53_101 "HandlePushmsgMenuWinMsg() false"
#define MMIOTA_WINTABLE_4153_112_2_18_2_42_53_102 "ID_CMSBRW_PUSHMSG_DELETE index =%d"
#define MMIOTA_WINTABLE_4167_112_2_18_2_42_53_103 "ID_CMSBRW_PUSHMSG_DELETE index =%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_OTAPUSH_TRC)
TRACE_MSG(MMIOTA_FILE_122_112_2_18_2_42_36_0,"ffs_type=%d")
TRACE_MSG(MMIOTA_MAIN_2191_112_2_18_2_42_41_1,"MMIOTA_InitSettingMap")
TRACE_MSG(MMIOTA_MAIN_2202_112_2_18_2_42_41_2,"MMIOTA_InitSettingMap all memery failed!")
TRACE_MSG(MMIOTA_MAIN_2330_112_2_18_2_42_41_3,"MMIOTA_InitSettingMap all memery failed!")
TRACE_MSG(MMIOTA_MAIN_2350_112_2_18_2_42_41_4,"MMIOTA_InitSettingMap all memery failed!")
TRACE_MSG(MMIOTA_MAIN_3412_112_2_18_2_42_44_5,"MMIOTA_ParseWBXML DCD 2")
TRACE_MSG(MMIOTA_MAIN_3452_112_2_18_2_42_44_6,"MMIAPIOTA_ParseWBXML all memery failed!")
TRACE_MSG(MMIOTA_MAIN_3528_112_2_18_2_42_44_7,"dcd is wap push entered")
TRACE_MSG(MMIOTA_WINTABLE_551_112_2_18_2_42_46_8,"[MMISMS]: HandleSIMPlugNotifyOTAFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMIOTA_WINTABLE_602_112_2_18_2_42_46_9,"MMIOTA_FreePushList Sms index=%d")
TRACE_MSG(MMIOTA_WINTABLE_641_112_2_18_2_42_46_10,"MMIOTA_ReadOtaFile IN -------")
TRACE_MSG(MMIOTA_WINTABLE_645_112_2_18_2_42_46_11,"fnh ota.cnf is exist!")
TRACE_MSG(MMIOTA_WINTABLE_647_112_2_18_2_42_46_12,"fnh cur_hefs = %d")
TRACE_MSG(MMIOTA_WINTABLE_655_112_2_18_2_42_46_13,"fnh read ota file  size is zero!")
TRACE_MSG(MMIOTA_WINTABLE_663_112_2_18_2_42_46_14,"fnh malloc param_read_ptr fail!")
TRACE_MSG(MMIOTA_WINTABLE_670_112_2_18_2_42_46_15,"fnh size is %d")
TRACE_MSG(MMIOTA_WINTABLE_675_112_2_18_2_42_46_16,"fnh malloc param_read_ptr->buf_ptr fail!")
TRACE_MSG(MMIOTA_WINTABLE_689_112_2_18_2_42_46_17,"fnh after FILE_Read returnvalue = %d")
TRACE_MSG(MMIOTA_WINTABLE_708_112_2_18_2_42_46_18,"FNH THE ota FILE IS failed!")
TRACE_MSG(MMIOTA_WINTABLE_714_112_2_18_2_42_46_19,"FNH THE ota FILE IS not exist!")
TRACE_MSG(MMIOTA_WINTABLE_731_112_2_18_2_42_46_20,"MMIOTA_ReadOtaItem IN ----")
TRACE_MSG(MMIOTA_WINTABLE_805_112_2_18_2_42_46_21,"[MMIMMS]:MMIAPIOTA_AddOTAPUSH push sender is in black lists!")
TRACE_MSG(MMIOTA_WINTABLE_811_112_2_18_2_42_46_22,"[MMIMMS]:MMIAPIOTA_AddOTAPUSH sender_sr_len is d%!")
TRACE_MSG(MMIOTA_WINTABLE_815_112_2_18_2_42_46_23,"[MMIMMS]:MMIAPIOTA_AddOTAPUSH sender_ptr is PNULL!")
TRACE_MSG(MMIOTA_WINTABLE_932_112_2_18_2_42_47_24,"MMIOTA_otapush error!")
TRACE_MSG(MMIOTA_WINTABLE_942_112_2_18_2_42_47_25,"MMIOTA_otapush alert")
TRACE_MSG(MMIOTA_WINTABLE_1042_112_2_18_2_42_47_26,"MMIOTA_OTAUpdateState,id=%d")
TRACE_MSG(MMIOTA_WINTABLE_1103_112_2_18_2_42_47_27,"MMIOTA_InsertOTAToSms,index=%d")
TRACE_MSG(MMIOTA_WINTABLE_1193_112_2_18_2_42_47_28,"MMIOTA_StoreOTAPUSH  IN")
TRACE_MSG(MMIOTA_WINTABLE_1216_112_2_18_2_42_47_29,"fnh param_save_ptr malloc fail")
TRACE_MSG(MMIOTA_WINTABLE_1245_112_2_18_2_42_47_30,"fnh MMIMMS_SaveOTAPASHFile efs_error is %d")
TRACE_MSG(MMIOTA_WINTABLE_1257_112_2_18_2_42_47_31,"VC:DeleteVideoFile FFS_AYS_Delete = %d")
TRACE_MSG(MMIOTA_WINTABLE_1272_112_2_18_2_42_47_32,"VC: MMIVC_CallBackDeleteMMSFile run")
TRACE_MSG(MMIOTA_WINTABLE_1275_112_2_18_2_42_47_33,"MMIOTA_StoreOTAPUSH OUT -----------")
TRACE_MSG(MMIOTA_WINTABLE_1412_112_2_18_2_42_48_34,"SaveOTASettingValue dual_sys=%d")
TRACE_MSG(MMIOTA_WINTABLE_1614_112_2_18_2_42_48_35,"HandleMMSConfigInfoDetailWinMsg")
TRACE_MSG(MMIOTA_WINTABLE_1633_112_2_18_2_42_48_36,"cur_sel=%d")
TRACE_MSG(MMIOTA_WINTABLE_2069_112_2_18_2_42_49_37,"malloc Detail_FormConfigInfo failed")
TRACE_MSG(MMIOTA_WINTABLE_2088_112_2_18_2_42_49_38,"Detail_FormConfigInfo  label_string2.wstr_ptr alloc failed")
TRACE_MSG(MMIOTA_WINTABLE_2097_112_2_18_2_42_49_39,"Detail_FormConfigInfo  str_dst->wstr_ptr alloc failed")
TRACE_MSG(MMIOTA_WINTABLE_2139_112_2_18_2_42_49_40,"HandleOTAConfigSetOptionWinMsg() msg_id=%d")
TRACE_MSG(MMIOTA_WINTABLE_2199_112_2_18_2_42_49_41,"warning i>OTA_CONFIG_INFO_NUM_MAX")
TRACE_MSG(MMIOTA_WINTABLE_2245_112_2_18_2_42_49_42,"save OTA config setting_type=%d")
TRACE_MSG(MMIOTA_WINTABLE_2280_112_2_18_2_42_49_43,"save bookmark!!")
TRACE_MSG(MMIOTA_WINTABLE_2290_112_2_18_2_42_49_44,"warning i>OTA_CONFIG_INFO_NUM_MAX")
TRACE_MSG(MMIOTA_WINTABLE_2470_112_2_18_2_42_50_45,"HandleMMSConfigInfoDetailWinMsg")
TRACE_MSG(MMIOTA_WINTABLE_2486_112_2_18_2_42_50_46,"cur_sel=%d")
TRACE_MSG(MMIOTA_WINTABLE_2714_112_2_18_2_42_50_47,"MMIOTA_DealWapPush() IN")
TRACE_MSG(MMIOTA_WINTABLE_2721_112_2_18_2_42_50_48,"MMIOTA_DealWapPush() execute_action = %d")
TRACE_MSG(MMIOTA_WINTABLE_2756_112_2_18_2_42_50_49,"MMIOTA_DealWapPush() signal_action = %d")
TRACE_MSG(MMIOTA_WINTABLE_2759_112_2_18_2_42_50_50,"MMIOTA_DealWapPush() MMIOTA_SI_SIGNAL_ACTION_NONE")
TRACE_MSG(MMIOTA_WINTABLE_2769_112_2_18_2_42_50_51,"MMIOTA_DealWapPush() PNULL = g_push_info_ptr")
TRACE_MSG(MMIOTA_WINTABLE_2781_112_2_18_2_42_50_52,"MMIOTA_DealWapPush delete index =%d")
TRACE_MSG(MMIOTA_WINTABLE_2821_112_2_18_2_42_50_53,"MMIOTA_DealWapPush() co")
TRACE_MSG(MMIOTA_WINTABLE_2829_112_2_18_2_42_50_54,"MMIOTA_DealWapPush() error")
TRACE_MSG(MMIOTA_WINTABLE_2834_112_2_18_2_42_50_55,"MMIOTA_DealWapPush() save")
TRACE_MSG(MMIOTA_WINTABLE_2841_112_2_18_2_42_50_56,"MMIOTA_DealWapPush() PNULL == g_push_info_ptr")
TRACE_MSG(MMIOTA_WINTABLE_2915_112_2_18_2_42_51_57,"g_cmsbrw_pushmsg_info.item_detail[index].time=%d")
TRACE_MSG(MMIOTA_WINTABLE_2928_112_2_18_2_42_51_58,"MMIOTA_DealWapPush() alert")
TRACE_MSG(MMIOTA_WINTABLE_2982_112_2_18_2_42_51_59,"MMIOTA_DealWapPush() read si")
TRACE_MSG(MMIOTA_WINTABLE_2996_112_2_18_2_42_51_60,"MMIOTA_DealWapPush() enter broswer")
TRACE_MSG(MMIOTA_WINTABLE_3005_112_2_18_2_42_51_61,"MMIOTA_DealWapPush() succed")
TRACE_MSG(MMIOTA_WINTABLE_3033_112_2_18_2_42_51_62,"item item_num %d")
TRACE_MSG(MMIOTA_WINTABLE_3040_112_2_18_2_42_51_63,"MMICMS_WriteDataToItemStruct() maxnum>=MMICMSBRW_MAX_ITEM_NUM")
TRACE_MSG(MMIOTA_WINTABLE_3063_112_2_18_2_42_51_64,"the latest listitem %d ")
TRACE_MSG(MMIOTA_WINTABLE_3071_112_2_18_2_42_51_65,"MMICMS_WriteDataToItemStruct() maxnum>=MMICMSBRW_MAX_ITEM_NUM index=%d")
TRACE_MSG(MMIOTA_WINTABLE_3085_112_2_18_2_42_51_66,"MMICMS_WriteDataToItemStruct() maxnum<MMICMSBRW_MAX_ITEM_NUM")
TRACE_MSG(MMIOTA_WINTABLE_3098_112_2_18_2_42_51_67,"MMICMS_WriteDataToItemStruct() index=%d")
TRACE_MSG(MMIOTA_WINTABLE_3169_112_2_18_2_42_51_68,"MMIAPIBRW_InsertWapPushToSms,index=%d")
TRACE_MSG(MMIOTA_WINTABLE_3190_112_2_18_2_42_51_69,"time=%d,cur_item->time=%d")
TRACE_MSG(MMIOTA_WINTABLE_3243_112_2_18_2_42_51_70,"MMIOTA_WriteWapPushFile() in!filelen=%d")
TRACE_MSG(MMIOTA_WINTABLE_3244_112_2_18_2_42_51_71,"MMIOTA_WriteWapPushFile:flash_filename=%s")
TRACE_MSG(MMIOTA_WINTABLE_3258_112_2_18_2_42_51_72,"MMIOTA_WriteWapPushFile,create cur_hefs1")
TRACE_MSG(MMIOTA_WINTABLE_3262_112_2_18_2_42_51_73,"MMIOTA_WriteWapPushFilecreate cur_hefs2=%d")
TRACE_MSG(MMIOTA_WINTABLE_3263_112_2_18_2_42_51_74,"return fail")
TRACE_MSG(MMIOTA_WINTABLE_3278_112_2_18_2_42_51_75,"MMIOTA_WriteWapPushFile error=%d")
TRACE_MSG(MMIOTA_WINTABLE_3288_112_2_18_2_42_51_76,"VC:DeleteVideoFile FFS_AYS_Delete = %d")
TRACE_MSG(MMIOTA_WINTABLE_3332_112_2_18_2_42_51_77,"MMIMMS_HandleNewMsgWin(),OPEN")
TRACE_MSG(MMIOTA_WINTABLE_3411_112_2_18_2_42_52_78,"MMIOTA_WapPushEnter,ID=%d")
TRACE_MSG(MMIOTA_WINTABLE_3414_112_2_18_2_42_52_79,"MMIOTA_WapPushEnter,ID is too large")
TRACE_MSG(MMIOTA_WINTABLE_3454_112_2_18_2_42_52_80,"MMIAPIOTA_WapPushUpdateState,ID=%d")
TRACE_MSG(MMIOTA_WINTABLE_3474_112_2_18_2_42_52_81,"MMIAPIOTA_WapPushUpdateState,folder_type=%d")
TRACE_MSG(MMIOTA_WINTABLE_3567_112_2_18_2_42_52_82,"MMIAPIBRW_InsertWapPushToSms PNULL == g_push_info_ptr")
TRACE_MSG(MMIOTA_WINTABLE_3575_112_2_18_2_42_52_83,"MMIAPIBRW_InsertWapPushToSms list_num=%d")
TRACE_MSG(MMIOTA_WINTABLE_3582_112_2_18_2_42_52_84,"MMIAPIBRW_InsertWapPushToSms time is zero!")
TRACE_MSG(MMIOTA_WINTABLE_3610_112_2_18_2_42_52_85,"MMIAPIOTA_DelPushByID ID =%d, totalnum = %d")
TRACE_MSG(MMIOTA_WINTABLE_3620_112_2_18_2_42_52_86,"MMIAPIOTA_DelPushByID index =%d")
TRACE_MSG(MMIOTA_WINTABLE_3639_112_2_18_2_42_52_87,"g_cmsbrw_pushmsg_info.item_num=%d")
TRACE_MSG(MMIOTA_WINTABLE_3679_112_2_18_2_42_52_88,"MMIBRW_WriteFile_Ex,file_type=%d")
TRACE_MSG(MMIOTA_WINTABLE_3682_112_2_18_2_42_52_89,"wrong param")
TRACE_MSG(MMIOTA_WINTABLE_3693_112_2_18_2_42_52_90,"cyz create file failed")
TRACE_MSG(MMIOTA_WINTABLE_3697_112_2_18_2_42_52_91,"MMICMSWAP_FILE_Create failed")
TRACE_MSG(MMIOTA_WINTABLE_3708_112_2_18_2_42_52_92,"cyz write file error=%d")
TRACE_MSG(MMIOTA_WINTABLE_3746_112_2_18_2_42_52_93,"MMIOTA_FileOpCbf,efs_err=%d,buf_size=%d,result=%d")
TRACE_MSG(MMIOTA_WINTABLE_3755_112_2_18_2_42_52_94,"cyz file_type=%d")
TRACE_MSG(MMIOTA_WINTABLE_3756_112_2_18_2_42_52_95,"param addr=%d")
TRACE_MSG(MMIOTA_WINTABLE_3779_112_2_18_2_42_52_96,"MMIAPIOTA_GetPushLockFlag record_id ERROR = %d")
TRACE_MSG(MMIOTA_WINTABLE_3904_112_2_18_2_42_53_97,"HandlePushmsgWinMsg() msg_id=%d")
TRACE_MSG(MMIOTA_WINTABLE_4040_112_2_18_2_42_53_98,"HandlePushmsgMenuWinMsg() msg_id=%d")
TRACE_MSG(MMIOTA_WINTABLE_4051_112_2_18_2_42_53_99,"HandlePushmsgMenuWinMsg() open")
TRACE_MSG(MMIOTA_WINTABLE_4054_112_2_18_2_42_53_100,"HandlePushmsgMenuWinMsg() true")
TRACE_MSG(MMIOTA_WINTABLE_4059_112_2_18_2_42_53_101,"HandlePushmsgMenuWinMsg() false")
TRACE_MSG(MMIOTA_WINTABLE_4153_112_2_18_2_42_53_102,"ID_CMSBRW_PUSHMSG_DELETE index =%d")
TRACE_MSG(MMIOTA_WINTABLE_4167_112_2_18_2_42_53_103,"ID_CMSBRW_PUSHMSG_DELETE index =%d")
END_TRACE_MAP(MMI_APP_OTAPUSH_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_OTAPUSH_TRC_H_

