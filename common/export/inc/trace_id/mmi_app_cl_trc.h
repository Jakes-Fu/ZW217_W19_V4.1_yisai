/******************************************************************************
 ** File Name:      mmi_app_cl_trc.h                                         *
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
//trace_id:160
#ifndef _MMI_APP_CL_TRC_H_
#define _MMI_APP_CL_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMICL_424_112_2_18_2_11_16_0 "MMIAPICL_Init call info alloc failed"
#define MMICL_535_112_2_18_2_11_16_1 "MMICL_RecordCallInfo: the type of call is error!"
#define MMICL_576_112_2_18_2_11_16_2 "MMICL_DeleteAppointedCallInfo call info alloc failed"
#define MMICL_620_112_2_18_2_11_16_3 "MMICL_DeleteAppointedCallInfo error record_num = %d"
#define MMICL_686_112_2_18_2_11_16_4 "MMICL_DeleteAppointedCallInfo call info alloc failed"
#define MMICL_738_112_2_18_2_11_17_5 "MMICL_DeleteAppointedCallInfo error record_num = %d"
#define MMICL_793_112_2_18_2_11_17_6 "AddNewCallInfo call info alloc failed"
#define MMICL_959_112_2_18_2_11_17_7 "call_bcdinfo alloc fail!!"
#define MMICL_1249_112_2_18_2_11_18_8 "call_bcdinfo alloc fail!!"
#define MMICL_1382_112_2_18_2_11_18_9 "RecordCallTime:the time of missed call is zero!"
#define MMICL_1395_112_2_18_2_11_18_10 "RecordCallTime:the time of refused call is zero!"
#define MMICL_1541_112_2_18_2_11_18_11 "MMICL_ReadAllNV call_info_ptr ==  PNULL"
#define MMICL_1609_112_2_18_2_11_18_12 "MMICL_ReadLastMissedCall() call info alloc failed"
#define MMICL_1639_112_2_18_2_11_18_13 "MMICL_ReadLastMissedCall() not find missed call! record_num = %d"
#define MMICL_1832_112_2_18_2_11_19_14 "MMIAPICL_SearchList"
#define MMICL_1854_112_2_18_2_11_19_15 "arraycall_info alloc memory failed!!"
#define MMICL_1917_112_2_18_2_11_19_16 "MMIAPICL_SearchList result:%d"
#define MMICL_1929_112_2_18_2_11_19_17 "MMIAPICL_SearchExit"
#define MMICL_1951_112_2_18_2_11_19_18 "MMIAPICL_GetSearchEntryName index:%d"
#define MMICL_1955_112_2_18_2_11_19_19 "MMIAPICL_GetSearchEntryName index:parameter error !"
#define MMICL_2040_112_2_18_2_11_19_20 "MMIAPICL_GetSearchEntryNumber parameter error !"
#define MMICL_2066_112_2_18_2_11_20_21 "MMIAPICL_GetSearchEntryNumber parameter error !"
#define MMICL_2149_112_2_18_2_11_20_22 "GetAllRecordsNum call info alloc failed"
#define MMICL_2177_112_2_18_2_11_20_23 "GetAllRecordsNum call info alloc failed"
#define MMICL_2320_112_2_18_2_11_20_24 "mmicl calllog_restore_callback: error param"
#define MMICL_COSTS_WINTAB_255_112_2_18_2_11_21_25 "SavePuctInfo call_info_ptr is NULL"
#define MMICL_COSTS_WINTAB_317_112_2_18_2_11_21_26 "PuctDecodeToChargeStr currency OR out_str is NULL"
#define MMICL_COSTS_WINTAB_336_112_2_18_2_11_22_27 "PuctDecodeToChargeStr error len = %d, exponent = %d"
#define MMICL_COSTS_WINTAB_347_112_2_18_2_11_22_28 "PuctDecodeToChargeStr error"
#define MMICL_COSTS_WINTAB_356_112_2_18_2_11_22_29 "PuctDecodeToChargeStr error"
#define MMICL_COSTS_WINTAB_392_112_2_18_2_11_22_30 "PuctDecodeToChargeStr error"
#define MMICL_COSTS_WINTAB_432_112_2_18_2_11_22_31 "PuctDecodeToChargeStr str_info OR eppu OR ex is NULL"
#define MMICL_COSTS_WINTAB_535_112_2_18_2_11_22_32 "GetUpdateAcmMax error len = %d"
#define MMICL_COSTS_WINTAB_1683_112_2_18_2_11_24_33 "MMICL_SetSimSelect sim_select = %d"
#define MMICL_LDN_222_112_2_18_2_11_25_34 "MMICL: InitCallInfo:storage=%d, dual_sys=%d"
#define MMICL_LDN_272_112_2_18_2_11_25_35 "MMICL: InitCallInfo ldn_entry_ptr No Memory"
#define MMICL_LDN_281_112_2_18_2_11_25_36 "MMICL: InitCallInfo call_info_ptr No Memory"
#define MMICL_LDN_407_112_2_18_2_11_25_37 "MMICL: InitCallInfo:is_ldn_valid=%d, max_record_num=%d, used_record_num=%d"
#define MMICL_LDN_456_112_2_18_2_11_26_38 "MMICL: MMICL_ReadCallInfo err_code=%d"
#define MMICL_LDN_512_112_2_18_2_11_26_39 "MMICL: MMICL_UpdateCallInfo ldn file not exist!"
#define MMICL_LDN_590_112_2_18_2_11_26_40 "MMICL:  MMICL_UpdateCallInfo return flase "
#define MMICL_LDN_680_112_2_18_2_11_26_41 "MMICL: MMICL_UpdateCallInfo dial num not present"
#define MMICL_LDN_683_112_2_18_2_11_26_42 "MMICL: MMICL_UpdateCallInfo err_code=%d"
#define MMICL_LDN_735_112_2_18_2_11_26_43 "MMICL: MMICL_DeleteCallInfo ldn file not exist!"
#define MMICL_LDN_752_112_2_18_2_11_26_44 "MMICL: MMICL_DeleteCallInfo No Memory"
#define MMICL_LDN_800_112_2_18_2_11_26_45 "MMICL: MMICL_DeleteCallInfo err_code=%d"
#define MMICL_LDN_853_112_2_18_2_11_26_46 "MMICL: MMICL_DeleteAllCallnfo ldn file not exist!"
#define MMICL_LDN_882_112_2_18_2_11_26_47 "MMICL: MMICL_DeleteAllCallnfo err_code=%d"
#define MMICL_LDN_911_112_2_18_2_11_26_48 "MMICL_IsCallInfoReady is_callinfo_ready=%d"
#define MMICL_LDN_983_112_2_18_2_11_27_49 "MMICL: MMICL_DeleteAllCallnfo dual_sys=%d"
#define MMICL_LDN_995_112_2_18_2_11_27_50 "MMICL: MMICL_DeleteAllCallnfo err_code=%d"
#define MMICL_LDN_1060_112_2_18_2_11_27_51 "MMICL: GetLdnIsDeleteAll s_is_ldn_delete_all =%d"
#define MMICL_WINTAB_1835_112_2_18_2_11_31_52 "GetDisplayPhoneNum call info alloc failed"
#define MMICL_WINTAB_2076_112_2_18_2_11_31_53 "GetAllRecordsNum call info alloc failed"
#define MMICL_WINTAB_2626_112_2_18_2_11_32_54 "SetLogListItem call info alloc failed"
#define MMICL_WINTAB_2667_112_2_18_2_11_32_55 "SetLogSelectListItem call info alloc failed"
#define MMICL_WINTAB_2739_112_2_18_2_11_33_56 "SetLogListItem call info alloc failed"
#define MMICL_WINTAB_3123_112_2_18_2_11_33_57 "AppendLogListItem:the call_type %d is error!"
#define MMICL_WINTAB_3162_112_2_18_2_11_33_58 "MMIAPICL_GetCallTypeImageID:the call_type %d is error!"
#define MMICL_WINTAB_3814_112_2_18_2_11_35_59 "MMIAPICL_MakeCallByRecordIndex call info alloc failed"
#define MMICL_WINTAB_3917_112_2_18_2_11_35_60 "MMIAPICL_MakeCallByRecordIndex: the call number is not exited, arraycall_info->record_num is = %d,is num present = %d"
#define MMICL_WINTAB_4640_112_2_18_2_11_36_61 "HandleCallLogChildWindow call info alloc failed"
#define MMICL_WINTAB_4743_112_2_18_2_11_36_62 "kick the key VT"
#define MMICL_WINTAB_4927_112_2_18_2_11_37_63 "HandleCallLogChildWindow MSG_CTL_PENOK arraycall_info is NULL"
#define MMICL_WINTAB_5030_112_2_18_2_11_37_64 "HandleDialedChildWindow call info alloc failed"
#define MMICL_WINTAB_5118_112_2_18_2_11_37_65 "kick the key VT"
#define MMICL_WINTAB_5269_112_2_18_2_11_38_66 "HandleDialedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
#define MMICL_WINTAB_5370_112_2_18_2_11_38_67 "HandleReceivedChildWindow call info alloc failed"
#define MMICL_WINTAB_5605_112_2_18_2_11_38_68 "HandleReceivedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
#define MMICL_WINTAB_5708_112_2_18_2_11_38_69 "HandleMissedChildWindow call info alloc failed"
#define MMICL_WINTAB_5945_112_2_18_2_11_39_70 "HandleMissedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
#define MMICL_WINTAB_6049_112_2_18_2_11_39_71 "HandleRefusedChildWindow call info alloc failed"
#define MMICL_WINTAB_6288_112_2_18_2_11_40_72 "HandleRefusedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
#define MMICL_WINTAB_6502_112_2_18_2_11_40_73 "HandleAllChildWindow call info alloc failed"
#define MMICL_WINTAB_6980_112_2_18_2_11_41_74 "HandleSelectAllChildWindow call info alloc failed"
#define MMICL_WINTAB_7798_112_2_18_2_11_43_75 "SetLogListDetail call info alloc failed"
#define MMICL_WINTAB_8032_112_2_18_2_11_43_76 "HandleLogListDetailWindow call info alloc failed"
#define MMICL_WINTAB_8509_112_2_18_2_11_44_77 "GetDisplayPhoneName call_name OR call_name->wstr_ptr is NULL"
#define MMICL_WINTAB_8544_112_2_18_2_11_44_78 "GetDisplayPhoneNum call info alloc failed"
#define MMICL_WINTAB_8591_112_2_18_2_11_44_79 "GetDisplayPhoneNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)"
#define MMICL_WINTAB_8597_112_2_18_2_11_44_80 "GetDisplayPhoneNum PNULL == tele_num ||PNULL == dual_sys_ptr"
#define MMICL_WINTAB_8620_112_2_18_2_11_44_81 "GetDisplayPhoneNum call info alloc failed"
#define MMICL_WINTAB_8757_112_2_18_2_11_45_82 "GetPartyPhoneNum call info alloc failed"
#define MMICL_WINTAB_8767_112_2_18_2_11_45_83 "GetPartyPhoneNum PNULL == bcd_number"
#define MMICL_WINTAB_8877_112_2_18_2_11_45_84 "DeleteAllCallLog call info alloc failed"
#define MMICL_WINTAB_8899_112_2_18_2_11_45_85 "DeleteAllCallLog call info alloc failed"
#define MMICL_WINTAB_8950_112_2_18_2_11_45_86 "DeleteAllCallLog: the call_type is %d!"
#define MMICL_WINTAB_8969_112_2_18_2_11_45_87 "DeleteAllCallLog call info alloc failed"
#define MMICL_WINTAB_8985_112_2_18_2_11_45_88 "DeleteAllCallLog call info alloc failed"
#define MMICL_WINTAB_9012_112_2_18_2_11_45_89 "DeleteAllCallLog: the call_type is %d!"
#define MMICL_WINTAB_9180_112_2_18_2_11_46_90 "SetCallTimeLabel:nv_item %d is error!"
#define MMICL_WINTAB_9210_112_2_18_2_11_46_91 "ConvertTimeToDisplay  time is NULL"
#define MMICL_WINTAB_9300_112_2_18_2_11_46_92 "MMICL_OpenMainMenuWindowExt:mainmenu_style %d is error!"
#define MMICL_WINTAB_9317_112_2_18_2_11_46_93 "MMIAPICL_GetSelectedItemNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)"
#define MMICL_WINTAB_9323_112_2_18_2_11_46_94 "MMIAPICL_GetSelectedItemNum PNULL == tele_num"
#define MMICL_WINTAB_9385_112_2_18_2_11_46_95 "SetHeadAndContentInRText PNULL == index"
#define MMICL_WINTAB_9434_112_2_18_2_11_46_96 "SetHeadAndContentInRText PNULL == index"
#define MMICL_WINTAB_9557_112_2_18_2_11_47_97 "HandleGprsFlowrateWinMsg dual_sys = %d"
#define MMICL_WINTAB_10080_112_2_18_2_11_48_98 "Istyle_HandleAllChildWindow call info alloc failed"
#define MMICL_WINTAB_10215_112_2_18_2_11_49_99 "Istyle_HandlePBDetailCalllogChildWindow call info alloc failed"
#define MMICL_WINTAB_10476_112_2_18_2_11_50_100 "SetLogListItem call info alloc failed"
#define MMICL_WINTAB_10526_112_2_18_2_11_50_101 "SetLogListItem call info alloc failed"
#define MMICL_WINTAB_10678_112_2_18_2_11_50_102 "SetLogListItem call info alloc failed"
#define MMICL_WINTAB_10951_112_2_18_2_11_51_103 "HandleDialedChildWindow call info alloc failed"
#define MMICL_WINTAB_11100_112_2_18_2_11_51_104 "SetCallTimesListOptMenuTitle call info alloc failed"
#define MMICL_WINTAB_11365_112_2_18_2_11_51_105 "SetCallTimesListOptMenuTitle call info alloc failed"
#define MMICL_WINTAB_11482_112_2_18_2_11_52_106 "SetLogListDetail call info alloc failed"
#define MMICL_WINTAB_11705_112_2_18_2_11_52_107 "HandleLogListDetailWindow call info alloc failed"
#define MMICL_WINTAB_11835_112_2_18_2_11_52_108 "mmicl GetRecordNumByType null ptr"
#define MMICL_WINTAB_11878_112_2_18_2_11_52_109 "Enter MMIAPICL_GetNameByClInfo"
#define MMICL_WINTAB_11882_112_2_18_2_11_52_110 "MMIAPICL_GetNameByClInfo index:parameter error !"
#define MMICL_WINTAB_12079_112_2_18_2_11_53_111 "HandlePBDetailCallLogListWindow call info alloc failed"
#define MMICL_WINTAB_12898_112_2_18_2_11_55_112 "GetDisplayPhoneNum call info alloc failed"
#define MMICL_WINTAB_13268_112_2_18_2_11_55_113 "GetPartyPhoneNum call info alloc failed"
#define MMICL_WINTAB_13324_112_2_18_2_11_55_114 "GetPartyPhoneNum call info alloc failed"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_CL_TRC)
TRACE_MSG(MMICL_424_112_2_18_2_11_16_0,"MMIAPICL_Init call info alloc failed")
TRACE_MSG(MMICL_535_112_2_18_2_11_16_1,"MMICL_RecordCallInfo: the type of call is error!")
TRACE_MSG(MMICL_576_112_2_18_2_11_16_2,"MMICL_DeleteAppointedCallInfo call info alloc failed")
TRACE_MSG(MMICL_620_112_2_18_2_11_16_3,"MMICL_DeleteAppointedCallInfo error record_num = %d")
TRACE_MSG(MMICL_686_112_2_18_2_11_16_4,"MMICL_DeleteAppointedCallInfo call info alloc failed")
TRACE_MSG(MMICL_738_112_2_18_2_11_17_5,"MMICL_DeleteAppointedCallInfo error record_num = %d")
TRACE_MSG(MMICL_793_112_2_18_2_11_17_6,"AddNewCallInfo call info alloc failed")
TRACE_MSG(MMICL_959_112_2_18_2_11_17_7,"call_bcdinfo alloc fail!!")
TRACE_MSG(MMICL_1249_112_2_18_2_11_18_8,"call_bcdinfo alloc fail!!")
TRACE_MSG(MMICL_1382_112_2_18_2_11_18_9,"RecordCallTime:the time of missed call is zero!")
TRACE_MSG(MMICL_1395_112_2_18_2_11_18_10,"RecordCallTime:the time of refused call is zero!")
TRACE_MSG(MMICL_1541_112_2_18_2_11_18_11,"MMICL_ReadAllNV call_info_ptr ==  PNULL")
TRACE_MSG(MMICL_1609_112_2_18_2_11_18_12,"MMICL_ReadLastMissedCall() call info alloc failed")
TRACE_MSG(MMICL_1639_112_2_18_2_11_18_13,"MMICL_ReadLastMissedCall() not find missed call! record_num = %d")
TRACE_MSG(MMICL_1832_112_2_18_2_11_19_14,"MMIAPICL_SearchList")
TRACE_MSG(MMICL_1854_112_2_18_2_11_19_15,"arraycall_info alloc memory failed!!")
TRACE_MSG(MMICL_1917_112_2_18_2_11_19_16,"MMIAPICL_SearchList result:%d")
TRACE_MSG(MMICL_1929_112_2_18_2_11_19_17,"MMIAPICL_SearchExit")
TRACE_MSG(MMICL_1951_112_2_18_2_11_19_18,"MMIAPICL_GetSearchEntryName index:%d")
TRACE_MSG(MMICL_1955_112_2_18_2_11_19_19,"MMIAPICL_GetSearchEntryName index:parameter error !")
TRACE_MSG(MMICL_2040_112_2_18_2_11_19_20,"MMIAPICL_GetSearchEntryNumber parameter error !")
TRACE_MSG(MMICL_2066_112_2_18_2_11_20_21,"MMIAPICL_GetSearchEntryNumber parameter error !")
TRACE_MSG(MMICL_2149_112_2_18_2_11_20_22,"GetAllRecordsNum call info alloc failed")
TRACE_MSG(MMICL_2177_112_2_18_2_11_20_23,"GetAllRecordsNum call info alloc failed")
TRACE_MSG(MMICL_2320_112_2_18_2_11_20_24,"mmicl calllog_restore_callback: error param")
TRACE_MSG(MMICL_COSTS_WINTAB_255_112_2_18_2_11_21_25,"SavePuctInfo call_info_ptr is NULL")
TRACE_MSG(MMICL_COSTS_WINTAB_317_112_2_18_2_11_21_26,"PuctDecodeToChargeStr currency OR out_str is NULL")
TRACE_MSG(MMICL_COSTS_WINTAB_336_112_2_18_2_11_22_27,"PuctDecodeToChargeStr error len = %d, exponent = %d")
TRACE_MSG(MMICL_COSTS_WINTAB_347_112_2_18_2_11_22_28,"PuctDecodeToChargeStr error")
TRACE_MSG(MMICL_COSTS_WINTAB_356_112_2_18_2_11_22_29,"PuctDecodeToChargeStr error")
TRACE_MSG(MMICL_COSTS_WINTAB_392_112_2_18_2_11_22_30,"PuctDecodeToChargeStr error")
TRACE_MSG(MMICL_COSTS_WINTAB_432_112_2_18_2_11_22_31,"PuctDecodeToChargeStr str_info OR eppu OR ex is NULL")
TRACE_MSG(MMICL_COSTS_WINTAB_535_112_2_18_2_11_22_32,"GetUpdateAcmMax error len = %d")
TRACE_MSG(MMICL_COSTS_WINTAB_1683_112_2_18_2_11_24_33,"MMICL_SetSimSelect sim_select = %d")
TRACE_MSG(MMICL_LDN_222_112_2_18_2_11_25_34,"MMICL: InitCallInfo:storage=%d, dual_sys=%d")
TRACE_MSG(MMICL_LDN_272_112_2_18_2_11_25_35,"MMICL: InitCallInfo ldn_entry_ptr No Memory")
TRACE_MSG(MMICL_LDN_281_112_2_18_2_11_25_36,"MMICL: InitCallInfo call_info_ptr No Memory")
TRACE_MSG(MMICL_LDN_407_112_2_18_2_11_25_37,"MMICL: InitCallInfo:is_ldn_valid=%d, max_record_num=%d, used_record_num=%d")
TRACE_MSG(MMICL_LDN_456_112_2_18_2_11_26_38,"MMICL: MMICL_ReadCallInfo err_code=%d")
TRACE_MSG(MMICL_LDN_512_112_2_18_2_11_26_39,"MMICL: MMICL_UpdateCallInfo ldn file not exist!")
TRACE_MSG(MMICL_LDN_590_112_2_18_2_11_26_40,"MMICL:  MMICL_UpdateCallInfo return flase ")
TRACE_MSG(MMICL_LDN_680_112_2_18_2_11_26_41,"MMICL: MMICL_UpdateCallInfo dial num not present")
TRACE_MSG(MMICL_LDN_683_112_2_18_2_11_26_42,"MMICL: MMICL_UpdateCallInfo err_code=%d")
TRACE_MSG(MMICL_LDN_735_112_2_18_2_11_26_43,"MMICL: MMICL_DeleteCallInfo ldn file not exist!")
TRACE_MSG(MMICL_LDN_752_112_2_18_2_11_26_44,"MMICL: MMICL_DeleteCallInfo No Memory")
TRACE_MSG(MMICL_LDN_800_112_2_18_2_11_26_45,"MMICL: MMICL_DeleteCallInfo err_code=%d")
TRACE_MSG(MMICL_LDN_853_112_2_18_2_11_26_46,"MMICL: MMICL_DeleteAllCallnfo ldn file not exist!")
TRACE_MSG(MMICL_LDN_882_112_2_18_2_11_26_47,"MMICL: MMICL_DeleteAllCallnfo err_code=%d")
TRACE_MSG(MMICL_LDN_911_112_2_18_2_11_26_48,"MMICL_IsCallInfoReady is_callinfo_ready=%d")
TRACE_MSG(MMICL_LDN_983_112_2_18_2_11_27_49,"MMICL: MMICL_DeleteAllCallnfo dual_sys=%d")
TRACE_MSG(MMICL_LDN_995_112_2_18_2_11_27_50,"MMICL: MMICL_DeleteAllCallnfo err_code=%d")
TRACE_MSG(MMICL_LDN_1060_112_2_18_2_11_27_51,"MMICL: GetLdnIsDeleteAll s_is_ldn_delete_all =%d")
TRACE_MSG(MMICL_WINTAB_1835_112_2_18_2_11_31_52,"GetDisplayPhoneNum call info alloc failed")
TRACE_MSG(MMICL_WINTAB_2076_112_2_18_2_11_31_53,"GetAllRecordsNum call info alloc failed")
TRACE_MSG(MMICL_WINTAB_2626_112_2_18_2_11_32_54,"SetLogListItem call info alloc failed")
TRACE_MSG(MMICL_WINTAB_2667_112_2_18_2_11_32_55,"SetLogSelectListItem call info alloc failed")
TRACE_MSG(MMICL_WINTAB_2739_112_2_18_2_11_33_56,"SetLogListItem call info alloc failed")
TRACE_MSG(MMICL_WINTAB_3123_112_2_18_2_11_33_57,"AppendLogListItem:the call_type %d is error!")
TRACE_MSG(MMICL_WINTAB_3162_112_2_18_2_11_33_58,"MMIAPICL_GetCallTypeImageID:the call_type %d is error!")
TRACE_MSG(MMICL_WINTAB_3814_112_2_18_2_11_35_59,"MMIAPICL_MakeCallByRecordIndex call info alloc failed")
TRACE_MSG(MMICL_WINTAB_3917_112_2_18_2_11_35_60,"MMIAPICL_MakeCallByRecordIndex: the call number is not exited, arraycall_info->record_num is = %d,is num present = %d")
TRACE_MSG(MMICL_WINTAB_4640_112_2_18_2_11_36_61,"HandleCallLogChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_4743_112_2_18_2_11_36_62,"kick the key VT")
TRACE_MSG(MMICL_WINTAB_4927_112_2_18_2_11_37_63,"HandleCallLogChildWindow MSG_CTL_PENOK arraycall_info is NULL")
TRACE_MSG(MMICL_WINTAB_5030_112_2_18_2_11_37_64,"HandleDialedChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_5118_112_2_18_2_11_37_65,"kick the key VT")
TRACE_MSG(MMICL_WINTAB_5269_112_2_18_2_11_38_66,"HandleDialedChildWindow MSG_CTL_PENOK arraycall_info is NULL")
TRACE_MSG(MMICL_WINTAB_5370_112_2_18_2_11_38_67,"HandleReceivedChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_5605_112_2_18_2_11_38_68,"HandleReceivedChildWindow MSG_CTL_PENOK arraycall_info is NULL")
TRACE_MSG(MMICL_WINTAB_5708_112_2_18_2_11_38_69,"HandleMissedChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_5945_112_2_18_2_11_39_70,"HandleMissedChildWindow MSG_CTL_PENOK arraycall_info is NULL")
TRACE_MSG(MMICL_WINTAB_6049_112_2_18_2_11_39_71,"HandleRefusedChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_6288_112_2_18_2_11_40_72,"HandleRefusedChildWindow MSG_CTL_PENOK arraycall_info is NULL")
TRACE_MSG(MMICL_WINTAB_6502_112_2_18_2_11_40_73,"HandleAllChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_6980_112_2_18_2_11_41_74,"HandleSelectAllChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_7798_112_2_18_2_11_43_75,"SetLogListDetail call info alloc failed")
TRACE_MSG(MMICL_WINTAB_8032_112_2_18_2_11_43_76,"HandleLogListDetailWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_8509_112_2_18_2_11_44_77,"GetDisplayPhoneName call_name OR call_name->wstr_ptr is NULL")
TRACE_MSG(MMICL_WINTAB_8544_112_2_18_2_11_44_78,"GetDisplayPhoneNum call info alloc failed")
TRACE_MSG(MMICL_WINTAB_8591_112_2_18_2_11_44_79,"GetDisplayPhoneNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)")
TRACE_MSG(MMICL_WINTAB_8597_112_2_18_2_11_44_80,"GetDisplayPhoneNum PNULL == tele_num ||PNULL == dual_sys_ptr")
TRACE_MSG(MMICL_WINTAB_8620_112_2_18_2_11_44_81,"GetDisplayPhoneNum call info alloc failed")
TRACE_MSG(MMICL_WINTAB_8757_112_2_18_2_11_45_82,"GetPartyPhoneNum call info alloc failed")
TRACE_MSG(MMICL_WINTAB_8767_112_2_18_2_11_45_83,"GetPartyPhoneNum PNULL == bcd_number")
TRACE_MSG(MMICL_WINTAB_8877_112_2_18_2_11_45_84,"DeleteAllCallLog call info alloc failed")
TRACE_MSG(MMICL_WINTAB_8899_112_2_18_2_11_45_85,"DeleteAllCallLog call info alloc failed")
TRACE_MSG(MMICL_WINTAB_8950_112_2_18_2_11_45_86,"DeleteAllCallLog: the call_type is %d!")
TRACE_MSG(MMICL_WINTAB_8969_112_2_18_2_11_45_87,"DeleteAllCallLog call info alloc failed")
TRACE_MSG(MMICL_WINTAB_8985_112_2_18_2_11_45_88,"DeleteAllCallLog call info alloc failed")
TRACE_MSG(MMICL_WINTAB_9012_112_2_18_2_11_45_89,"DeleteAllCallLog: the call_type is %d!")
TRACE_MSG(MMICL_WINTAB_9180_112_2_18_2_11_46_90,"SetCallTimeLabel:nv_item %d is error!")
TRACE_MSG(MMICL_WINTAB_9210_112_2_18_2_11_46_91,"ConvertTimeToDisplay  time is NULL")
TRACE_MSG(MMICL_WINTAB_9300_112_2_18_2_11_46_92,"MMICL_OpenMainMenuWindowExt:mainmenu_style %d is error!")
TRACE_MSG(MMICL_WINTAB_9317_112_2_18_2_11_46_93,"MMIAPICL_GetSelectedItemNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)")
TRACE_MSG(MMICL_WINTAB_9323_112_2_18_2_11_46_94,"MMIAPICL_GetSelectedItemNum PNULL == tele_num")
TRACE_MSG(MMICL_WINTAB_9385_112_2_18_2_11_46_95,"SetHeadAndContentInRText PNULL == index")
TRACE_MSG(MMICL_WINTAB_9434_112_2_18_2_11_46_96,"SetHeadAndContentInRText PNULL == index")
TRACE_MSG(MMICL_WINTAB_9557_112_2_18_2_11_47_97,"HandleGprsFlowrateWinMsg dual_sys = %d")
TRACE_MSG(MMICL_WINTAB_10080_112_2_18_2_11_48_98,"Istyle_HandleAllChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_10215_112_2_18_2_11_49_99,"Istyle_HandlePBDetailCalllogChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_10476_112_2_18_2_11_50_100,"SetLogListItem call info alloc failed")
TRACE_MSG(MMICL_WINTAB_10526_112_2_18_2_11_50_101,"SetLogListItem call info alloc failed")
TRACE_MSG(MMICL_WINTAB_10678_112_2_18_2_11_50_102,"SetLogListItem call info alloc failed")
TRACE_MSG(MMICL_WINTAB_10951_112_2_18_2_11_51_103,"HandleDialedChildWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_11100_112_2_18_2_11_51_104,"SetCallTimesListOptMenuTitle call info alloc failed")
TRACE_MSG(MMICL_WINTAB_11365_112_2_18_2_11_51_105,"SetCallTimesListOptMenuTitle call info alloc failed")
TRACE_MSG(MMICL_WINTAB_11482_112_2_18_2_11_52_106,"SetLogListDetail call info alloc failed")
TRACE_MSG(MMICL_WINTAB_11705_112_2_18_2_11_52_107,"HandleLogListDetailWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_11835_112_2_18_2_11_52_108,"mmicl GetRecordNumByType null ptr")
TRACE_MSG(MMICL_WINTAB_11878_112_2_18_2_11_52_109,"Enter MMIAPICL_GetNameByClInfo")
TRACE_MSG(MMICL_WINTAB_11882_112_2_18_2_11_52_110,"MMIAPICL_GetNameByClInfo index:parameter error !")
TRACE_MSG(MMICL_WINTAB_12079_112_2_18_2_11_53_111,"HandlePBDetailCallLogListWindow call info alloc failed")
TRACE_MSG(MMICL_WINTAB_12898_112_2_18_2_11_55_112,"GetDisplayPhoneNum call info alloc failed")
TRACE_MSG(MMICL_WINTAB_13268_112_2_18_2_11_55_113,"GetPartyPhoneNum call info alloc failed")
TRACE_MSG(MMICL_WINTAB_13324_112_2_18_2_11_55_114,"GetPartyPhoneNum call info alloc failed")
END_TRACE_MAP(MMI_APP_CL_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_CL_TRC_H_

