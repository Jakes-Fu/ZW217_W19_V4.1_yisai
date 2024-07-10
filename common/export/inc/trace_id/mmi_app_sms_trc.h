/******************************************************************************
 ** File Name:      mmi_app_sms_trc.h                                         *
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
//trace_id:222
#ifndef _MMI_APP_SMS_TRC_H_
#define _MMI_APP_SMS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMISMS_API_376_112_2_18_2_52_27_0 "MMISMS: MMIAPISMS_OpenFreqWordListForCc len = %d, addr = %p"
#define MMISMS_API_483_112_2_18_2_52_27_1 "MMISMS: MMIAPISMS_DeleteMsgInorder : msg_type = %d,record_id = %d"
#define MMISMS_API_524_112_2_18_2_52_27_2 "MMISMS: MMIAPISMS_InsertOneMsgToOrder order_ptr==PNULL!"
#define MMISMS_API_528_112_2_18_2_52_27_3 "MMISMS: MMIAPISMS_InsertOneMsgToOrder order->mo_mt_type = %d, mms_record_id = %d, dual_sys = %d, msg_type = %d"
#define MMISMS_API_546_112_2_18_2_52_27_4 "state_info.display_content.content_len = %d"
#define MMISMS_API_613_112_2_18_2_52_27_5 "MMISMS: MMIAPISMS_InsertOneMsgToOrder order_ptr==PNULL!"
#define MMISMS_API_617_112_2_18_2_52_27_6 "MMISMS: MMIAPISMS_UpdateMsgStatusInOrder enter: record_id = %d, dual_sys = %d "
#define MMISMS_API_697_112_2_18_2_52_27_7 "MMISMS: MMIAPISMS_UpdateMsgStatusInOrder i = %d"
#define MMISMS_API_725_112_2_18_2_52_27_8 "MMISMS: MMIAPISMS_DeleteAllMMSDone enter!"
#define MMISMS_API_747_112_2_18_2_52_28_9 "MMISMS: MMIAPISMS_MMSOrderDone enter!"
#define MMISMS_API_768_112_2_18_2_52_28_10 "MMISMS:MMIAPISMS_MMSInitDone"
#define MMISMS_API_1193_112_2_18_2_52_29_11 "MMISMS: MMIAPISMS_DeleteAllMailMsg"
#define MMISMS_API_1344_112_2_18_2_52_29_12 "MMIWIDGET_NewSms_GetOriginNumByPos PNULL == num_ptr || PNULL == dual_sys_ptr"
#define MMISMS_API_1401_112_2_18_2_52_29_13 "MMIWIDGET_NewSms_GetOriginTimeAndDate PNULL == date_ptr || PNULL == time_ptr"
#define MMISMS_DELALLWIN_387_112_2_18_2_52_31_14 "MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x"
#define MMISMS_DELALLWIN_407_112_2_18_2_52_31_15 "MMISMS:HandleBoxDelChildWinMsg need_item_data_ptr = PNULL"
#define MMISMS_DELALLWIN_538_112_2_18_2_52_31_16 "MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x"
#define MMISMS_DELALLWIN_558_112_2_18_2_52_32_17 "MMISMS:HandleBoxDelChildWinMsg need_item_data_ptr = PNULL"
#define MMISMS_DELALLWIN_917_112_2_18_2_52_33_18 "SetMsgTypeListItemData msg_type = %d"
#define MMISMS_DELALLWIN_968_112_2_18_2_52_33_19 "SetMsgTypeListItemData msg_type = %d"
#define MMISMS_DELALLWIN_1047_112_2_18_2_52_33_20 "GetMsgTypeNumString msg_type = %d"
#define MMISMS_DELETE_85_112_2_18_2_52_34_21 "MMISMS: HandleDeleteCnf cause = %d"
#define MMISMS_DELETE_161_112_2_18_2_52_34_22 "MMISMS: DeleteReqToMN(%d, %d, %d)"
#define MMISMS_DELETE_170_112_2_18_2_52_34_23 "MMISMS: DeleteReqToMN record_id = %d, storage = %d"
#define MMISMS_DELETE_173_112_2_18_2_52_34_24 "MMISMS: DeleteReqToMN MNSMS_UpdateSmsStatusEx result = %d"
#define MMISMS_DELETE_177_112_2_18_2_52_34_25 "MMISMS: DeleteReqToMN oper_status = %d"
#define MMISMS_DELETE_181_112_2_18_2_52_34_26 "sms: DeleteReqToMN, g_mmisms_global.operation.cur_type = %d"
#define MMISMS_DELETE_251_112_2_18_2_52_34_27 "MMISMS: MMISMS_HandleUpdateSMSCnf sig_ptr == PNULL"
#define MMISMS_DELETE_255_112_2_18_2_52_34_28 "MMISMS: MMISMS_HandleUpdateSMSCnf record_id = %d, storage = %d,oper_status = %d"
#define MMISMS_DELETE_334_112_2_18_2_52_35_29 "MMISMS: MMISMS_HandleUpdateSMSCnf get record_id = %d, storage = %d"
#define MMISMS_DELETE_422_112_2_18_2_52_35_30 "MMISMS: MMISMS_HandleUpdateSMSCnf %d"
#define MMISMS_DELETE_533_112_2_18_2_52_35_31 "MMISMS: MMISMS_HandleUpdateSMSCnf %d"
#define MMISMS_DELETE_583_112_2_18_2_52_35_32 "MMISMS: MMISMS_HandleUpdateSMSCnf %d"
#define MMISMS_DELETE_652_112_2_18_2_52_35_33 "MMISMS: MMISMS_HandleUpdateSMSCnf sig_ptr->record_id = %d, sig_ptr->storage = %d, oper_status = %d"
#define MMISMS_DELETE_750_112_2_18_2_52_35_34 "MMISMS: MMISMS_AppDeleteMsgBox type = %d"
#define MMISMS_DELETE_811_112_2_18_2_52_36_35 "MMISMS: MMISMS_DeleteInfoInSendListById"
#define MMISMS_DELETE_884_112_2_18_2_52_36_36 "MMISMS: MMISMS_AppDeleteMsgBox: The SmsBox is empty!"
#define MMISMS_DELETE_929_112_2_18_2_52_36_37 "MMISMS: MMISMS_DeletePointedSMS pos_index = %d, order_id = %d, type = %d"
#define MMISMS_DELETE_948_112_2_18_2_52_36_38 "MMISMS: HandleMoveCnf cause = %d"
#define MMISMS_DELETE_952_112_2_18_2_52_36_39 "MMISMS: HandleMoveCnf move_all_flag = %d"
#define MMISMS_EDIT_73_112_2_18_2_52_36_40 "MMISMS: HandleEditMoSmsSuccess record_id = %d, storage = %d, status = %d, oper_status = %d"
#define MMISMS_EDIT_168_112_2_18_2_52_37_41 "MMISMS: UpdateEditStatusToOrderList(%d, %d, %d, %d)"
#define MMISMS_EDIT_272_112_2_18_2_52_37_42 "MMISMS: MMISMS_HandleEditSMSCnf sig_ptr = PNULL"
#define MMISMS_EDIT_275_112_2_18_2_52_37_43 "MMISMS: MMISMS_HandleEditSMSCnf status = %d"
#define MMISMS_EDIT_328_112_2_18_2_52_37_44 "MMISMS:MMISMS_InterceptMsgContent PNULL !=  send_info_ptr"
#define MMISMS_EDIT_333_112_2_18_2_52_37_45 "MMISMS:MMISMS_InterceptMsgContent PNULL ==  send_info_ptr"
#define MMISMS_EDIT_337_112_2_18_2_52_37_46 "MMISMS:MMISMS_InterceptMsgContent intercpet_len_ptr intercpet_content_ptr send_message = PNULL"
#define MMISMS_EDIT_461_112_2_18_2_52_37_47 "MMISMS:MMISMS_InterceptClass0MsgContent intercpet_len_ptr intercpet_content_ptr = PNULL"
#define MMISMS_EDIT_587_112_2_18_2_52_38_48 "MMISMSR8: MMISMS_SetMessageContent content_ptr"
#define MMISMS_EDIT_601_112_2_18_2_52_38_49 "MMISMS: MMISMS_SetMessageContent No memory!"
#define MMISMS_EDIT_654_112_2_18_2_52_38_50 "MMISMSR8: MMISMS_SetMessageContent language=%d"
#define MMISMS_EDIT_697_112_2_18_2_52_38_51 "MMISMS: MMISMS_SetMessageContent length=%d,alphabet=%d,is_need_packet=%d"
#define MMISMS_EDIT_699_112_2_18_2_52_38_52 "MMISMS: MMISMS_SetMessageContent length=%d,is_ucs2=%d,is_need_packet=%d"
#define MMISMS_EDIT_830_112_2_18_2_52_38_53 "MMISMS: MMISMS_SetMessageContent: SCI_MEMCPY length = %d"
#define MMISMS_EDIT_860_112_2_18_2_52_38_54 "MMISMS:MMISMS_InterceptClass0MsgContent intercpet_len_ptr intercpet_content_ptr = PNULL"
#define MMISMS_FILE_1449_112_2_18_2_52_41_55 "MMISMS:MMISMS_GetDisplayPathText path_value_str_ptr path_name_ptr path_name_len_ptr = PNULL"
#define MMISMS_FILE_1491_112_2_18_2_52_41_56 "MMISMS:MMISMS_GetCustomizedExportPath full_path_ptr full_path_len_ptr = PNULL"
#define MMISMS_FILE_1498_112_2_18_2_52_41_57 "MMISMS_GetCustomizedExportPath len=%d"
#define MMISMS_FILE_1639_112_2_18_2_52_42_58 "MMISMS:MMISMS_GetDefaultExportPath full_path_ptr full_path_len_ptr = PNULL"
#define MMISMS_FILE_1709_112_2_18_2_52_42_59 "Export_OpenLabelCallBack is_success = %d"
#define MMISMS_FILE_1715_112_2_18_2_52_42_60 "Export_OpenLabelCallBack, file_num = %d"
#define MMISMS_FILE_1753_112_2_18_2_52_42_61 "Export_OpenLabelCallBack is_success = %d"
#define MMISMS_FILE_1759_112_2_18_2_52_42_62 "Export_OpenLabelCallBack, file_num = %d"
#define MMISMS_FILE_1875_112_2_18_2_52_42_63 "MMISMS: MMISMS_AppExportMsgBox type = %d"
#define MMISMS_FILE_1936_112_2_18_2_52_43_64 "MMISMS_AppExportMsgBox: msg_type is %d"
#define MMISMS_FILE_1958_112_2_18_2_52_43_65 "MMISMS: s_export_file_handle: The SmsBox is empty!"
#define MMISMS_FILE_2069_112_2_18_2_52_43_66 "CreateExportMsgFile s_export_file_handle is 0x%x"
#define MMISMS_FILE_2153_112_2_18_2_52_43_67 "MMISMS_ExportMsgToFile: error is %d"
#define MMISMS_FILE_2157_112_2_18_2_52_43_68 "MMISMS_ExportMsgToFile: s_mmisms_export_current_num is %d"
#define MMISMS_FILE_2216_112_2_18_2_52_43_69 "MMISMS:MMISMS_GetDisplayPathText path_ptr name_ptr full_path_ptr full_path_len_ptr = PNULL"
#define MMISMS_FILE_2221_112_2_18_2_52_43_70 "MMISMS_CombineExportFile name_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d"
#define MMISMS_FILE_2282_112_2_18_2_52_43_71 "MMISMS_SetExportFileHandle: file_handle is 0x%x"
#define MMISMS_FILE_2291_112_2_18_2_52_43_72 "MMISMS_GetExportFileHandle: s_export_file_handle is 0x%x"
#define MMISMS_FILE_2501_112_2_18_2_52_44_73 "MMISMS: SetExportFileContent length = %d"
#define MMISMS_FILE_2526_112_2_18_2_52_44_74 "MMISMS:GetFileTransmitterDispString orginal_addr_ptr string_ptr = PNULL"
#define MMISMS_FILE_2536_112_2_18_2_52_44_75 "GetFileTransmitterDispString num_name.wstr_ptr alloc fail"
#define MMISMS_FILE_2547_112_2_18_2_52_44_76 "MMISMS: GetFileTransmitterDispString number_type = %d, number_plan = %d"
#define MMISMS_FILE_2645_112_2_18_2_52_44_77 "WriteExportFileBoxType box_type is %d"
#define MMISMS_FILE_2750_112_2_18_2_52_44_78 "WriteExportFileBoxType error is %d"
#define MMISMS_FILE_2793_112_2_18_2_52_44_79 "MMISMS_SetExportTotalNum: total_num is %d"
#define MMISMS_FILE_2803_112_2_18_2_52_44_80 "MMISMS_GetExportTotalNum: s_mmisms_export_total_num is %d"
#define MMISMS_FILE_2813_112_2_18_2_52_44_81 "MMISMS_SetExportCurrentNum: current_num is %d"
#define MMISMS_FILE_2823_112_2_18_2_52_45_82 "MMISMS_GetExportCurrentNum: s_mmisms_export_current_num is %d"
#define MMISMS_FILE_2833_112_2_18_2_52_45_83 "MMISMS_SetExportCurBoxType: box_type is %d"
#define MMISMS_FILE_2843_112_2_18_2_52_45_84 "MMISMS_GetExportCurBoxType: s_mmisms_cur_boxtype is %d"
#define MMISMS_FILE_2866_112_2_18_2_52_45_85 "MMISMS_SetExportSecurityBoxFlag: is_export is %d"
#define MMISMS_FILE_2876_112_2_18_2_52_45_86 "MMISMS_GetExportSecurityBoxFlag: s_mmisms_export_securitybox is %d"
#define MMISMS_FILE_2890_112_2_18_2_52_45_87 "MMISMS_FreeExportFilePtr"
#define MMISMS_NV_113_112_2_18_2_52_49_88 "MMISMS:MMISMS_NV_ReadSmsItem item_ptr = PNULL"
#define MMISMS_NV_186_112_2_18_2_52_49_89 "SMS: MMISMS_NV_SetMaxSmsNum max num = %d"
#define MMISMS_NV_219_112_2_18_2_52_49_90 "SMS: MMISMS_NV_DelAllSmsItem nv_result = %d"
#define MMISMS_NV_293_112_2_18_2_52_49_91 "MMISMS:MMISMS_NV_ReadFolderTypeManager folder_type_ptr = PNULL"
#define MMISMS_NV_313_112_2_18_2_52_49_92 "MMISMS: MMISMS_WriteFolderTypeManager folder_type_ptr = PNULL"
#define MMISMS_NV_334_112_2_18_2_52_49_93 "MMISMS: MMISMS_NV_SetSecurityBoxPwd pwd_ptr = PNULL"
#define MMISMS_NV_369_112_2_18_2_52_49_94 "MMISMS:MMISMS_ReadExportPath path_ptr = PNULL"
#define MMISMS_NV_393_112_2_18_2_52_49_95 "MMISMS: MMISMS_WriteExportPath path_ptr = PNULL"
#define MMISMS_NV_409_112_2_18_2_52_49_96 "MMISMS:MMISMS_ReadExportMethodType method_type = PNULL"
#define MMISMS_READ_551_112_2_18_2_52_51_97 "SearchStrInReadMsg dst_len = %d count = %d"
#define MMISMS_READ_657_112_2_18_2_52_51_98 "SearchStrInMMSMsg dst_len = %d count = %d"
#define MMISMS_READ_746_112_2_18_2_52_51_99 "SearchStrInPushMsg dst_len = %d count = %d"
#define MMISMS_READ_896_112_2_18_2_52_51_100 "HandleOkMsgInSmsList index is %d"
#define MMISMS_READ_1272_112_2_18_2_52_52_101 "MMISMS:SaveReadedSmsToGlobal new_user_data_ptr address_ptr = PNULL"
#define MMISMS_READ_1276_112_2_18_2_52_52_102 "MMISMS: SaveReadedSmsToGlobal alphabet_type = %d, msg_type = %d, dual_sys = %d "
#define MMISMS_READ_1303_112_2_18_2_52_52_103 "MMISMSR8:SaveReadedSmsToGlobal language is %d,is_lock_shift is %d,is_single_shift is %d"
#define MMISMS_READ_1402_112_2_18_2_52_53_104 "MMISMS:MMISMS_AssembleSmsToGlobal new_user_data_ptr address_ptr = PNULL"
#define MMISMS_READ_1409_112_2_18_2_52_53_105 "MMISMS_AssembleSmsToGlobal refnum_in_new = %d,refnum_in_save = %d"
#define MMISMS_READ_1434_112_2_18_2_52_53_106 "MMISMS_AssembleSmsToGlobal seqnum_in_save = %d,seqnum_in_new = %d"
#define MMISMS_READ_1495_112_2_18_2_52_53_107 "MMISMS: AssembleSms new_seq = %d, save_seq = %d"
#define MMISMS_READ_1585_112_2_18_2_52_53_108 "MMISMS: AssembleSms new_seq = %d, save_seq = %d"
#define MMISMS_READ_1606_112_2_18_2_52_53_109 "MMISMS:GetSeqNumFromUserHead user_data_head_ptr = PNULL"
#define MMISMS_READ_1633_112_2_18_2_52_53_110 "MMISMS:GetMaxNumFromUserHead user_data_head_ptr = PNULL"
#define MMISMS_READ_1666_112_2_18_2_52_53_111 "MMISMS:AppendString2ReadContent str_ptr = PNULL"
#define MMISMS_READ_1682_112_2_18_2_52_53_112 "MMISMS: AppendString2ReadContent read_content_length = %d, append_len = %d"
#define MMISMS_READ_1695_112_2_18_2_52_53_113 "AppendString2ReadContent len is %d"
#define MMISMS_READ_1726_112_2_18_2_52_53_114 "MMISMS:AssembleStatusReportToGlobal status_report_ptr = PNULL"
#define MMISMS_READ_1875_112_2_18_2_52_53_115 "MMISMS:MMISMS_AppReadSms order_id = PNULL"
#define MMISMS_READ_1912_112_2_18_2_52_54_116 "MMISMS: MMISMS_AppReadSms order_id = %d, max_sms_num = %d, record_id = %d, storage = %d"
#define MMISMS_READ_1915_112_2_18_2_52_54_117 "MMISMS:MMISMS_AppReadSms order_info.flag.dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMS_READ_1937_112_2_18_2_52_54_118 "MMISMS:GetSMSBoxTypeByReadSMSCnf sig_ptr = PNULL"
#define MMISMS_READ_1987_112_2_18_2_52_54_119 "MMI_SMS:GetSMSBoxTypeByReadSMSCnf box_type=%d"
#define MMISMS_READ_2022_112_2_18_2_52_54_120 "MMISMS:MMISMS_HandleUserReadSMSCnf sig_ptr = PNULL"
#define MMISMS_READ_2028_112_2_18_2_52_54_121 "MMISMS:MMISMS_HandleUserReadSMSCnf sig_ptr->item_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMS_READ_2034_112_2_18_2_52_54_122 "SMS: MMISMS_HandleUserReadSMSCnf dual_sys = %d, cause = %d, status = %d, record_id = %d, storage = %d"
#define MMISMS_READ_2049_112_2_18_2_52_54_123 "SMS: MMISMS_HandleUserReadSMSCnf g_sms_backup_data_ptr->time = %d, g_sms_backup_data_ptr->record_id = %d"
#define MMISMS_READ_2054_112_2_18_2_52_54_124 "SMS: MMISMS_HandleUserReadSMSCnf sig_ptr->sms_t_u.mo_sms_t.sc_addr_t.num_len = %d"
#define MMISMS_READ_2124_112_2_18_2_52_54_125 "sms:g_mmisms_global.read_msg.dual_sys = %d, g_mmisms_global.operation.cur_type = %d"
#define MMISMS_READ_2140_112_2_18_2_52_54_126 "MMISMS: MMISMS_HandleUserReadSMSCnf head_len = %d"
#define MMISMS_READ_2240_112_2_18_2_52_54_127 "MMISMS: OperateMtIndAsMmsPush mms head length = %d"
#define MMISMS_READ_2399_112_2_18_2_52_55_128 "security_box_type is %d"
#define MMISMS_READ_2428_112_2_18_2_52_55_129 "MMI_SMS: Warning---The oper type(%d)is changed(%d)! Discard!"
#define MMISMS_READ_2463_112_2_18_2_52_55_130 "MMI_SMS: Warning---oper_status(%d)is MMISMS_READ_SMS! Discard!"
#define MMISMS_READ_2513_112_2_18_2_52_55_131 "MMISMS_HandleUserReadSMSCnf seq num is %d,max num is %d"
#define MMISMS_READ_2545_112_2_18_2_52_55_132 "sms:g_mmisms_global.read_msg.dual_sys = %d, g_mmisms_global.operation.cur_type = %d"
#define MMISMS_READ_2579_112_2_18_2_52_55_133 "MMISMS: MMISMS_HandleUserReadSMSCnf head_len = %d"
#define MMISMS_READ_2630_112_2_18_2_52_55_134 "MMISMS: MMISMS_HandleUserReadSMSCnf: the next_order_id = 0x%x"
#define MMISMS_READ_2910_112_2_18_2_52_56_135 "MMISMS:MMISMS_GetRefNumFromUserHead user_data_head_ptr = PNULL"
#define MMISMS_READ_2936_112_2_18_2_52_56_136 "SMS:MMISMS_HandleReadSmsCnf, record_id=%d, storage=%d, status=%d, dual_sys=%d"
#define MMISMS_READ_2985_112_2_18_2_52_56_137 "MMISMS: Sms Order OK! time=%ld"
#define MMISMS_READ_3079_112_2_18_2_52_56_138 "mmisms:MMISMS_InitSmsList"
#define MMISMS_READ_3149_112_2_18_2_52_56_139 "MMISMS:ReadSmsForOrder dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMS_READ_3179_112_2_18_2_52_56_140 "MMISMS: ReadSmsForOrder status_ptr = PNULL"
#define MMISMS_READ_3194_112_2_18_2_52_56_141 "ReadSmsForOrder SIM NOT Ready"
#define MMISMS_READ_3208_112_2_18_2_52_56_142 "SMS ReadSmsForOrder storage=%d, record_id=%d, max_sms_num=%d"
#define MMISMS_READ_3220_112_2_18_2_52_56_143 "SMS: ReadSmsForOrder the storage(%d) is ready, record_id = %d"
#define MMISMS_READ_3322_112_2_18_2_52_56_144 "MMISMS:MMISMS_HandleSMSOrder sig_ptr = PNULL"
#define MMISMS_READ_3348_112_2_18_2_52_56_145 "MMISMS_HandleUserReadSMSCnf:send_type is %d"
#define MMISMS_READ_3391_112_2_18_2_52_57_146 "MMISMS: MMISMS_HandleSMSOrder sig_ptr->cause is %d"
#define MMISMS_READ_3444_112_2_18_2_52_57_147 "MMISMS:HandleMtSMSOrder mt_sms_ptr = PNULL"
#define MMISMS_READ_3506_112_2_18_2_52_57_148 "MMISMSR8:InsertNewMtToOrder language is %d,is_lock_shift is %d,is_single_shift is %d"
#define MMISMS_READ_3519_112_2_18_2_52_57_149 "MMISMSR8:InsertNewMtToOrder content_len is %d"
#define MMISMS_READ_3568_112_2_18_2_52_57_150 "HandleMtSMSOrder:err_code=%d,is_new_mms=%d,send_type=%d"
#define MMISMS_READ_3595_112_2_18_2_52_57_151 "MMISMS: HandleMtSMSOrder: head_len = %d, ref_num = 0x%X, seq_num = 0x%X"
#define MMISMS_READ_3606_112_2_18_2_52_57_152 "MMISMS: HandleMtSMSOrder: head_len = %d, ref_num = 0x%X, seq_num = 0x%X"
#define MMISMS_READ_3636_112_2_18_2_52_57_153 "MMISMS:HandleSROrder mt_sr_ptr = PNULL"
#define MMISMS_READ_3741_112_2_18_2_52_57_154 "MMISMS:HandleMoSMSOrder mo_sms_ptr = PNULL"
#define MMISMS_READ_3771_112_2_18_2_52_57_155 "MMISMS: HandleMoSMSOrder head_len = %d"
#define MMISMS_READ_3792_112_2_18_2_52_57_156 "HandleMoSMSOrder folder_type = %d,record_id = %d"
#define MMISMS_READ_3807_112_2_18_2_52_57_157 "MMISMSR8:InsertNewMtToOrder language is %d,is_lock_shift is %d,is_single_shift is %d"
#define MMISMS_READ_3820_112_2_18_2_52_57_158 "MMISMSR8:InsertNewMtToOrder content_len is %d"
#define MMISMS_READ_3903_112_2_18_2_52_58_159 "HandleMoSMSOrder sc_addr_present is %d,len is %d"
#define MMISMS_READ_3943_112_2_18_2_52_58_160 "MMISMS:MMISMS_GetNameByNumberFromPB name_ptr number_ptr = PNULL"
#define MMISMS_READ_3966_112_2_18_2_52_58_161 "MMISMS: MMISMS_GetNameByNumberFromPB is_found = %d"
#define MMISMS_READ_3998_112_2_18_2_52_58_162 "MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM"
#define MMISMS_READ_4035_112_2_18_2_52_58_163 "MMISMS: MMISMS_GetContactInfo contact_info_ptr = PNULL"
#define MMISMS_READ_4043_112_2_18_2_52_58_164 "MMISMS: MMISMS_GetContactInfo name_str.wstr_ptr = PNULL"
#define MMISMS_READ_4051_112_2_18_2_52_58_165 "MMISMS: MMISMS_GetContactInfo email_str.wstr_ptr = PNULL"
#define MMISMS_READ_4209_112_2_18_2_52_58_166 "MMISMS:AppendString2Contact conv_str_ptr org_str_ptr = PNULL"
#define MMISMS_READ_4267_112_2_18_2_52_58_167 "MMISMS:MMISMS_GetSMSUseNumber num_arr_ptr = PNULL"
#define MMISMS_READ_4390_112_2_18_2_52_59_168 "MMISMS:MMISMS_GetOriginTimeAndDate date_ptr time_ptr = PNULL"
#define MMISMS_READ_4432_112_2_18_2_52_59_169 "StartSmsSortTimer: the timer has started!"
#define MMISMS_READ_4482_112_2_18_2_52_59_170 "StopSmsSortTimer: the timer has stop!"
#define MMISMS_READ_4658_112_2_18_2_52_59_171 "MMISMS: MMISMS_AppMoveMsgBox type = %d,oper_status = %d"
#define MMISMS_READ_4831_112_2_18_2_52_59_172 "MMISMS_AppMoveMsgBox msg_type = %d"
#define MMISMS_READ_4975_112_2_18_2_53_0_173 "MMISMS: MMISMS_AppDeleteMsgBox: The SmsBox is empty!"
#define MMISMS_READ_5020_112_2_18_2_53_0_174 "MMISMS: MMISMS_AppMoveMsgBox type = %d,oper_status = %d"
#define MMISMS_READ_5129_112_2_18_2_53_0_175 "MMISMS_AppMoveMsgBox msg_type = %d"
#define MMISMS_READ_5177_112_2_18_2_53_0_176 "MMISMS: MMISMS_AppDeleteMsgBox: The SmsBox is empty!"
#define MMISMS_READ_5245_112_2_18_2_53_0_177 "MMISMS_SetChatLongSmsItemContet:seq_len is %d,ref_num is %d,max is %d,seq is %d"
#define MMISMS_RECEIVE_299_112_2_18_2_53_2_178 "MMISMS:AddJavaSMSData java_data_ptr = PNULL"
#define MMISMS_RECEIVE_303_112_2_18_2_53_2_179 "MMISMS:AddJavaSMSData java_data_ptr = is_long =%d,total_num=%d,cur_num=%d"
#define MMISMS_RECEIVE_334_112_2_18_2_53_2_180 "MMISMS:AddJavaSMSData javasms_func = PNULL"
#define MMISMS_RECEIVE_413_112_2_18_2_53_2_181 "MMISMS: OperateMtIndForJava: user_data_ptr = PNULL!"
#define MMISMS_RECEIVE_426_112_2_18_2_53_2_182 "MMISMS: OperateMtIndForJava: is_new_java_sms = %d"
#define MMISMS_RECEIVE_433_112_2_18_2_53_2_183 "MMISMS: OperateMtIndForJava: valid_date_ptr alloc fail!"
#define MMISMS_RECEIVE_443_112_2_18_2_53_2_184 "MMISMS: OperateMtIndForJava: iei_arr[%d].iei=%d"
#define MMISMS_RECEIVE_496_112_2_18_2_53_2_185 "MMISMS:OperateMtIndForJava java_data_ptr = PNULL"
#define MMISMS_RECEIVE_564_112_2_18_2_53_2_186 "MMISMS: OperateMtIndForJava: sms_t.dcs.alphabet_type = %d"
#define MMISMS_RECEIVE_571_112_2_18_2_53_2_187 "MMISMS: OperateMtIndForJava: MN_SMS_UCS2_ALPHABET user_data_ptr->user_valid_data_t.length  %d"
#define MMISMS_RECEIVE_578_112_2_18_2_53_2_188 "MMISMS: OperateMtIndForJava: user_data_ptr->user_valid_data_t.length = %d"
#define MMISMS_RECEIVE_588_112_2_18_2_53_2_189 "second:MMISMS: OperateMtIndForJava: MN_SMS_UCS2_ALPHABET user_data_ptr->user_valid_data_t.length  %d"
#define MMISMS_RECEIVE_594_112_2_18_2_53_2_190 "second:MMISMS: OperateMtIndForJava: user_data_ptr->user_valid_data_t.length = %d"
#define MMISMS_RECEIVE_597_112_2_18_2_53_2_191 "seq_indx,split_bytesseq_len =%d,%d,%d"
#define MMISMS_RECEIVE_605_112_2_18_2_53_2_192 "seq_indx,split_bytesseq_len =%d,%d,%d"
#define MMISMS_RECEIVE_641_112_2_18_2_53_2_193 "SMS: InitLongVcardsmsStruct()"
#define MMISMS_RECEIVE_687_112_2_18_2_53_3_194 "MMISMS:OperateMtIndAsSms user_data_ptr mt_sms_ptr = PNULL"
#define MMISMS_RECEIVE_724_112_2_18_2_53_3_195 "MMISMS:OperateMtIndAsSms storage = %d, record_id = %d"
#define MMISMS_RECEIVE_748_112_2_18_2_53_3_196 " MMISMS:OperateMtIndAsSms storage = %d, record_id = %d"
#define MMISMS_RECEIVE_798_112_2_18_2_53_3_197 "MMISMS: OperateMtIndAsSms head length = %d"
#define MMISMS_RECEIVE_799_112_2_18_2_53_3_198 "MMISMS: OperateMtIndAsSms ref = %d"
#define MMISMS_RECEIVE_817_112_2_18_2_53_3_199 "MMISMS: OperateMtIndAsSms need not update window"
#define MMISMS_RECEIVE_902_112_2_18_2_53_3_200 " MMISMS:OperateMtIndAsSms storage = %d, record_id = %d"
#define MMISMS_RECEIVE_987_112_2_18_2_53_3_201 "MMISMS:InsertNewMtToOrder mt_sms_ptr = PNULL"
#define MMISMS_RECEIVE_996_112_2_18_2_53_3_202 "MMISMS: InsertNewMtToOrder(%d,%d, %d, %d), pid_e=%d"
#define MMISMS_RECEIVE_1045_112_2_18_2_53_3_203 "MMISMSR8:InsertNewMtToOrder content_len is %d,alpha is %d"
#define MMISMS_RECEIVE_1055_112_2_18_2_53_3_204 "MMISMSR8:InsertNewMtToOrder language is %d,is_lock_shift is %d,is_single_shift is %d"
#define MMISMS_RECEIVE_1068_112_2_18_2_53_3_205 "MMISMSR8:InsertNewMtToOrder content_len is %d"
#define MMISMS_RECEIVE_1119_112_2_18_2_53_3_206 "MMISMS: InsertNewMtToOrder head_len = %d, ref_num = 0x%X, seq_num = 0x%X"
#define MMISMS_RECEIVE_1368_112_2_18_2_53_4_207 "MMISMS:OperateMtIndAsMmsPush user_data_ptr = PNULL"
#define MMISMS_RECEIVE_1382_112_2_18_2_53_4_208 "MMISMS: OperateMtIndAsMmsPush err_code=%d, is_new_mms=%d"
#define MMISMS_RECEIVE_1414_112_2_18_2_53_4_209 "MMISMS: OperateMtIndAsMmsPush mms head length = %d"
#define MMISMS_RECEIVE_1591_112_2_18_2_53_4_210 "MMISMS_R8:AddSMSContentItemtoRichText,language is %d,alphabet is %d"
#define MMISMS_RECEIVE_1596_112_2_18_2_53_4_211 "MMISMS_R8:AddSMSContentItemtoRichText,convert_msg_len is %d,content_len is %d"
#define MMISMS_RECEIVE_1854_112_2_18_2_53_5_212 "MMISMS_R8:AddSMSContentItemtoRichText,language is %d,alphabet is %d,&&=%d"
#define MMISMS_RECEIVE_1863_112_2_18_2_53_5_213 "MMISMS_R8:AddSMSContentItemtoRichText,convert_msg_len is %d,content_len is %d"
#define MMISMS_RECEIVE_1915_112_2_18_2_53_5_214 "MMISMS_R8:AddSMSContentItemtoRichText,convert_msg_len is %d,content_len is %d"
#define MMISMS_RECEIVE_2059_112_2_18_2_53_5_215 "MMISMS:LoadSms2Window msg_data_ptr orginal_addr_ptr = PNULL"
#define MMISMS_RECEIVE_2145_112_2_18_2_53_6_216 "MMISMS:ShowMsgContent msg_data_ptr orginal_addr_ptr = PNULL"
#define MMISMS_RECEIVE_2202_112_2_18_2_53_6_217 "MMISMS_R8:ShowMsgContent convert_msg_len is %d,content_len is %d"
#define MMISMS_RECEIVE_2291_112_2_18_2_53_6_218 "MMISMS_R8:ShowMsgContent language is %d,alphabet is %d"
#define MMISMS_RECEIVE_2332_112_2_18_2_53_6_219 "MMISMS: ShowMsgContent length = %d"
#define MMISMS_RECEIVE_2359_112_2_18_2_53_6_220 "MMISMS:MMISMS_GetTimeDispString time_ptr = PNULL"
#define MMISMS_RECEIVE_2399_112_2_18_2_53_6_221 "MMISMS:GetTransmitterDispString orginal_addr_ptr string_ptr = PNULL"
#define MMISMS_RECEIVE_2410_112_2_18_2_53_6_222 "MMISMS: GetTransmitterDispString number_type = %d, number_plan = %d"
#define MMISMS_RECEIVE_2473_112_2_18_2_53_6_223 "MMISMS:MMISMS_HandleSMSMTInd sig_ptr = PNULL"
#define MMISMS_RECEIVE_2480_112_2_18_2_53_6_224 "MMISMS: HandleSMSMTInd user_head_is_exist = %d, record_id = %d, storage = %d"
#define MMISMS_RECEIVE_2736_112_2_18_2_53_7_225 "MMISMS:MMISMS_HandleSMSReportInd sig_ptr = PNULL"
#define MMISMS_RECEIVE_2798_112_2_18_2_53_7_226 "MMISMS:MMISMS_HandleSMSReportInd dual_sys is %d,storage is %d,record_id is %d"
#define MMISMS_RECEIVE_2855_112_2_18_2_53_7_227 "SMS: MMISMS_IsMtForVcard()"
#define MMISMS_RECEIVE_2872_112_2_18_2_53_7_228 "SMS: MMISMS_IsMtForVcard: index = %d  port = %d "
#define MMISMS_RECEIVE_2923_112_2_18_2_53_7_229 "MMISMS:MMISMS_OperateMtIndForVcardOrVcal user_data_ptr = PNULL"
#define MMISMS_RECEIVE_2928_112_2_18_2_53_7_230 "SMS: OperateMtIndForVcard: send_type = %d"
#define MMISMS_RECEIVE_2944_112_2_18_2_53_7_231 "SMS: OperateMtIndForVcard: is_finished = %d"
#define MMISMS_RECEIVE_2978_112_2_18_2_53_7_232 "MMISMS:OperateMtIndAsSms sms_ptr time_stamp_ptr = PNULL"
#define MMISMS_RECEIVE_2981_112_2_18_2_53_7_233 "SMS: MMISMS_RecvVcardOrVcalSms"
#define MMISMS_RECEIVE_3008_112_2_18_2_53_7_234 "SMS: MMISMS_RecvVcardOrVcalSms  index=%d"
#define MMISMS_RECEIVE_3050_112_2_18_2_53_7_235 "SMS: MMISMS_RecvVcardOrVcalSms ref_num=%d,max_num=%d,seq_num=%d"
#define MMISMS_RECEIVE_3055_112_2_18_2_53_7_236 "SMS: MMISMS_RecvVcardOrVcalSms ref_num=%d,max_num=%d,seq_num=%d,vcard_sms[i].ref_num=%d"
#define MMISMS_RECEIVE_3066_112_2_18_2_53_7_237 "MMISMS_RecvVcardOrVcalSms the same seq sms"
#define MMISMS_RECEIVE_3071_112_2_18_2_53_7_238 "SMS: MMISMS_RecvVcardOrVcalSms receive_num=%d"
#define MMISMS_RECEIVE_3086_112_2_18_2_53_7_239 "SMS: MMISMS_RecvVcardOrVcalSms i=%d,current_time=%d"
#define MMISMS_RECEIVE_3099_112_2_18_2_53_7_240 "SMS: MMISMS_RecvVcardOrVcalSms i=%d"
#define MMISMS_RECEIVE_3105_112_2_18_2_53_7_241 "SMS: MMISMS_RecvVcardOrVcalSms empty_pos=%d"
#define MMISMS_RECEIVE_3108_112_2_18_2_53_7_242 "SMS: MMISMS_RecvVcardOrVcalSms write global var,seq_num=%d,ref_num=%d,max_num=%d"
#define MMISMS_RECEIVE_3124_112_2_18_2_53_8_243 "SMS: MMISMS_RecvVcardOrVcalSms write vcf file"
#define MMISMS_RECEIVE_3178_112_2_18_2_53_8_244 "SMS: MMISMS_RecvVcardOrVcalSms free memory"
#define MMISMS_RECEIVE_3182_112_2_18_2_53_8_245 "SMS: MMISMS_RecvVcardOrVcalSms exist vcard sms"
#define MMISMS_RECEIVE_3206_112_2_18_2_53_8_246 "SMS: MMISMS_SaveVcalendarSms send_type=%d, vcard_data.length=%d"
#define MMISMS_RECEIVE_3232_112_2_18_2_53_8_247 "MMISMS:GetVcardFileNameOverSms sc_time_ptr name_ptr = PNULL"
#define MMISMS_RECEIVE_3255_112_2_18_2_53_8_248 "MMISMS:MMISMS_SaveVcardSms name_ptr buf_ptr = PNULL"
#define MMISMS_RECEIVE_3280_112_2_18_2_53_8_249 "MMISMS:MMISMS_SaveVcardSms name_ptr buf_ptr = PNULL"
#define MMISMS_RECEIVE_3305_112_2_18_2_53_8_250 "MMISMS_ShowNewVcardOrVcalendarPrompt type=%d"
#define MMISMS_RECEIVE_3324_112_2_18_2_53_8_251 "SMS: MMISMS_ShowNewVcardOrVcalendarPrompt"
#define MMISMS_SAVE_178_112_2_18_2_53_9_252 "MMISMS:SaveMsgReq user_data_ptr  = PNULL"
#define MMISMS_SAVE_183_112_2_18_2_53_9_253 "MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL"
#define MMISMS_SAVE_249_112_2_18_2_53_9_254 "MMISMS: SaveMsgReq err_code = %d"
#define MMISMS_SAVE_261_112_2_18_2_53_9_255 "MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED"
#define MMISMS_SAVE_287_112_2_18_2_53_9_256 "MMISMS:SaveMsgReq user_data_ptr  = PNULL"
#define MMISMS_SAVE_292_112_2_18_2_53_9_257 "MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL"
#define MMISMS_SAVE_329_112_2_18_2_53_9_258 "MMISMS: SaveMsgReq err_code = %d"
#define MMISMS_SAVE_341_112_2_18_2_53_9_259 "MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED"
#define MMISMS_SAVE_371_112_2_18_2_53_9_260 "MMISMS:SaveMsgReq user_data_ptr  = PNULL"
#define MMISMS_SAVE_376_112_2_18_2_53_9_261 "MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL"
#define MMISMS_SAVE_429_112_2_18_2_53_9_262 "MMISMS: UpdateMsgReq err_code = %d"
#define MMISMS_SAVE_442_112_2_18_2_53_9_263 "MMISMS: UpdateMsgReq the storage is MN_SMS_NO_STORED"
#define MMISMS_SAVE_486_112_2_18_2_53_10_264 "MMISMS:MMISMS_HandleSaveSMSCnf sig_ptr = PNULL"
#define MMISMS_SAVE_490_112_2_18_2_53_10_265 "MMISMS: HandleSaveSMSCnf record_id = %d, storage = %d, cause = %d, dual_sys = %d, oper_status = %d"
#define MMISMS_SAVE_508_112_2_18_2_53_10_266 "MMISMS: HandleSaveSMSCnf 11 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d"
#define MMISMS_SAVE_525_112_2_18_2_53_10_267 "MMISMS: HandleSaveSMSCnf 22 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d"
#define MMISMS_SAVE_624_112_2_18_2_53_10_268 "MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SAVE_SEND_SMS send_info_ptr = PNULL"
#define MMISMS_SAVE_627_112_2_18_2_53_10_269 "HandleSaveSMSCnf:MMISMS_SAVE_SEND_SMS "
#define MMISMS_SAVE_710_112_2_18_2_53_10_270 "MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SEND_SAVE_SMS send_info_ptr = PNULL"
#define MMISMS_SAVE_713_112_2_18_2_53_10_271 "HandleSaveSMSCnf:resend_count is %d,send_result is %d"
#define MMISMS_SAVE_733_112_2_18_2_53_10_272 "MMISMS: UpdateMsgReq err_code = %d"
#define MMISMS_SAVE_770_112_2_18_2_53_10_273 "MMISMS: HandleSaveSMSCnf Deliver Cancel!"
#define MMISMS_SAVE_861_112_2_18_2_53_10_274 "MMISMS: HandleSaveSMSCnf oper_status(%d) is error"
#define MMISMS_SAVE_990_112_2_18_2_53_11_275 "MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SEND_SAVE_SMS send_info_ptr = PNULL"
#define MMISMS_SAVE_1127_112_2_18_2_53_11_276 "MMISMS: MMISMS_HandleSaveSMSCnf %d"
#define MMISMS_SAVE_1146_112_2_18_2_53_11_277 "MMISMS: MMISMS_HandleSaveSMSCnf %d"
#define MMISMS_SAVE_1163_112_2_18_2_53_11_278 "MMISMS: MMISMS_HandleSaveSMSCnf %d"
#define MMISMS_SAVE_1217_112_2_18_2_53_11_279 "MMISMS: MMISMS_HandleSaveSMSCnf %d"
#define MMISMS_SAVE_1233_112_2_18_2_53_11_280 "MMISMS: MMISMS_HandleSaveSMSCnf %d"
#define MMISMS_SAVE_1302_112_2_18_2_53_11_281 "MMISMS:MMISMS_AppSaveSms send_info_ptr = PNULL"
#define MMISMS_SAVE_1403_112_2_18_2_53_11_282 "MMISMS: SaveInfoToOrderByResult(%d, %d, %d, %d)"
#define MMISMS_SAVE_1408_112_2_18_2_53_11_283 "MMISMS:MMISMS_InterceptMsgContent PNULL !=  send_info_ptr"
#define MMISMS_SAVE_1413_112_2_18_2_53_11_284 "MMISMS:MMISMS_InterceptMsgContent PNULL ==  send_info_ptr"
#define MMISMS_SAVE_1431_112_2_18_2_53_11_285 "seq_num is %d,max_num is %d"
#define MMISMS_SAVE_1445_112_2_18_2_53_11_286 "seq_num is %d,max_num is %d"
#define MMISMS_SAVE_1753_112_2_18_2_53_12_287 "MMISMS:MMISMS_AppUpdateMoSms send_info_ptr != PNULL"
#define MMISMS_SAVE_1831_112_2_18_2_53_12_288 "MMISMS:MMISMS_SaveNextMessage send_info_ptr != PNULL"
#define MMISMS_SAVE_2052_112_2_18_2_53_13_289 "MMISMS:MMISMS_EncodeMoSmsData mo_sms_ptr user_data_ptr = PNULL"
#define MMISMS_SAVE_2067_112_2_18_2_53_13_290 "MMISMS: MMISMS_EncodeMoSmsData user_head_is_exist = %d"
#define MMISMS_SAVE_2093_112_2_18_2_53_13_291 "MMISMS:MMISMS_EncodeMoSmsData mt_sms_ptr user_data_ptr = PNULL"
#define MMISMS_SAVE_2107_112_2_18_2_53_13_292 "MMISMS: MMISMS_EncodeMtSmsData user_head_is_exist = %d"
#define MMISMS_SAVE_2139_112_2_18_2_53_13_293 "MMISMS:MMISMS_GetStoreStorage storage_ptr record_id_ptr = PNULL"
#define MMISMS_SAVE_2186_112_2_18_2_53_13_294 "MMISMS:MMISMS_GetStoreStorage status_ptr = PNULL"
#define MMISMS_SAVE_2229_112_2_18_2_53_13_295 "MMISMS: MMISMS_GetStoreStorage storage = %d, record_id = %d"
#define MMISMS_SAVE_2253_112_2_18_2_53_13_296 "MMISMS:MMISMS_GetStoreStorageForSendLongSms storage_ptr = PNULL"
#define MMISMS_SAVE_2336_112_2_18_2_53_13_297 "MMISMS:MMISMS_GetStoreStorageForSendLongSms 000"
#define MMISMS_SAVE_2340_112_2_18_2_53_13_298 "SMS: MMISMS_GetStoreStorageForSendLongSms storage = %d"
#define MMISMS_SAVE_2547_112_2_18_2_53_14_299 "MMISMS:MMISMS_SetSMSSaveInfo dest_num_ptr = PNULL"
#define MMISMS_SAVE_2572_112_2_18_2_53_14_300 "MMISMS:MMISMS_GetSMSMemory max_num_ptr used_num_ptr = PNULL"
#define MMISMS_SAVE_2579_112_2_18_2_53_14_301 "SMS: MMISMS_GetSMSMemory dual_sys=%d, is_nv = %d, max_num = %d, used_num = %d"
#define MMISMS_SAVE_2595_112_2_18_2_53_14_302 "MMISMS:MMISMS_GetLenByAlpha split_len_ptr num_ptr = PNULL"
#define MMISMS_SAVE_2654_112_2_18_2_53_14_303 "SMS: MMISMS_GetLenByAlpha alpha = %d"
#define MMISMS_SAVE_2683_112_2_18_2_53_14_304 "MMISMS:MMISMS_GetUserData sms_content_ptr tpdu_head_ptr user_data_ptr = PNULL"
#define MMISMS_SAVE_2736_112_2_18_2_53_14_305 "SMS: MMISMS_GetUserData sms_index = %d"
#define MMISMS_SAVE_2754_112_2_18_2_53_14_306 "MMISMS: MMISMS_GetUserData: valid data len  is = %d,contect len = %d, msg len =%d = %d"
#define MMISMS_SAVE_2759_112_2_18_2_53_14_307 "MMISMS: MMISMS_GetUserData length = %d"
#define MMISMS_SAVE_2801_112_2_18_2_53_14_308 "SMS: MMISMS_GetUserData tpdu_head_ptr->length = %d"
#define MMISMS_SAVE_2810_112_2_18_2_53_14_309 "MMISMS: MMISMS_GetUserData: HEAD LEN is = %d"
#define MMISMS_SAVE_2838_112_2_18_2_53_14_310 "MMISMS:MMISMS_FindFreeRecordId record_id_ptr = PNULL"
#define MMISMS_SAVE_2847_112_2_18_2_53_14_311 "MMISMS:MMISMS_FindFreeRecordId status_arr.mn_sms_status_arr_e_ptr = PNULL"
#define MMISMS_SAVE_2889_112_2_18_2_53_14_312 "MMISMS: MMISMS_FindFreeRecordId storage = %d, record_id = %d"
#define MMISMS_SAVE_2910_112_2_18_2_53_14_313 "MMISMS:MMISMS_UpdateStatusReportMsgReq storage = MN_SMS_STORAGE_ALL"
#define MMISMS_SAVE_2917_112_2_18_2_53_14_314 "MMISMS: UpdateMsgReq err_code = %d"
#define MMISMS_SAVE_2930_112_2_18_2_53_14_315 "MMISMS: UpdateMsgStatusReportReq the storage is MN_SMS_NO_STORED"
#define MMISMS_SAVE_2951_112_2_18_2_53_14_316 "MMISMS_MOVESaveSms:mo_mt_type is %d"
#define MMISMS_SAVE_3040_112_2_18_2_53_15_317 "MMISMS: MMISMS_CopySaveSms ret_val = %d"
#define MMISMS_SAVE_3084_112_2_18_2_53_15_318 "MMISMS_MOVESaveSms:mo_mt_type is %d"
#define MMISMS_SAVE_3164_112_2_18_2_53_15_319 "MMISMS: MMISMS_CopySaveSms ret_val = %d"
#define MMISMS_SAVE_3287_112_2_18_2_53_15_320 "MMISMS: SaveMsgReq err_code = %d"
#define MMISMS_SAVE_3299_112_2_18_2_53_15_321 "MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED"
#define MMISMS_SAVE_3335_112_2_18_2_53_16_322 "MMISMS_CopyNextMessage ret_val = %d"
#define MMISMS_SAVE_3370_112_2_18_2_53_16_323 "SMS: IsExistNextSMS %d"
#define MMISMS_SAVE_3442_112_2_18_2_53_16_324 "SMS: Get_Current_Max_Num =%d"
#define MMISMS_SAVE_3470_112_2_18_2_53_16_325 "SMS: Get_Current_Ref_Num =%d"
#define MMISMS_SAVE_3498_112_2_18_2_53_16_326 "SMS: Get_Current_Seq_Num =%d"
#define MMISMS_SAVE_3524_112_2_18_2_53_16_327 "MMISMS:MMISMS_GetReadUserData sms_content_ptr tpdu_head_ptr user_data_ptr = PNULL"
#define MMISMS_SAVE_3562_112_2_18_2_53_16_328 "SMS: MMISMS_GetReadUserData sms_index = %d"
#define MMISMS_SAVE_3580_112_2_18_2_53_16_329 "MMISMS: MMISMS_GetReadUserData: valid data len  is = %d,contect len = %d, msg len =%d = %d"
#define MMISMS_SAVE_3585_112_2_18_2_53_16_330 "MMISMS: MMISMS_GetReadUserData length = %d"
#define MMISMS_SAVE_3627_112_2_18_2_53_16_331 "SMS: MMISMS_GetReadUserData tpdu_head_ptr->length = %d"
#define MMISMS_SAVE_3636_112_2_18_2_53_16_332 "MMISMS: MMISMS_GetReadUserData: HEAD LEN is = %d"
#define MMISMS_SEND_204_112_2_18_2_53_17_333 "MMISMS:HandleSMSSendSuccessCnf send_message = PNULL"
#define MMISMS_SEND_332_112_2_18_2_53_17_334 "MMISMS:HandleSMSSendErrorCnf PNULL ==  send_info_ptr"
#define MMISMS_SEND_338_112_2_18_2_53_17_335 "MMISMS:HandleSMSSendErrorCnf cause ==  MN_SMS_OPERATE_SUCCESS"
#define MMISMS_SEND_354_112_2_18_2_53_17_336 "HandleSMSSendErrorCnf:cancel send sms is success"
#define MMISMS_SEND_364_112_2_18_2_53_17_337 "HandleSMSSendErrorCnf:resend_count is %d"
#define MMISMS_SEND_411_112_2_18_2_53_17_338 "MMISMS:HandleSMSSendCnfErrorNoResend PNULL ==  send_info_ptr"
#define MMISMS_SEND_415_112_2_18_2_53_17_339 "HandleSMSSendCnfErrorNoResend send type"
#define MMISMS_SEND_459_112_2_18_2_53_17_340 "MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d"
#define MMISMS_SEND_530_112_2_18_2_53_17_341 "MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d"
#define MMISMS_SEND_625_112_2_18_2_53_18_342 "MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d"
#define MMISMS_SEND_670_112_2_18_2_53_18_343 "MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d"
#define MMISMS_SEND_734_112_2_18_2_53_18_344 "MMISMS:InitUserDataHeadForJAVA data_head_ptr = PNULL"
#define MMISMS_SEND_737_112_2_18_2_53_18_345 "mmivcard:InitUserDataHeadForVcard srcPort=%d"
#define MMISMS_SEND_763_112_2_18_2_53_18_346 "MMISMS:HandleSMSSendCnf PNULL ==  send_info_ptr"
#define MMISMS_SEND_766_112_2_18_2_53_18_347 "MMISMS: HandleSMSSendCnf cause = %d"
#define MMISMS_SEND_803_112_2_18_2_53_18_348 "MMISMS:OperAfterSendCnf PNULL ==  send_info_ptr"
#define MMISMS_SEND_1015_112_2_18_2_53_18_349 "MMISMS:SendMsgReqToMN dest_number_ptr user_data_ptr = PNULL"
#define MMISMS_SEND_1018_112_2_18_2_53_18_350 "SMS: SendMsgReqToMN storage = %d, dual_sys = %d,is_resend=%d"
#define MMISMS_SEND_1087_112_2_18_2_53_18_351 "MMISMS: SendMsgReqToMN is_more_msg=%d,send_path=%d"
#define MMISMS_SEND_1098_112_2_18_2_53_18_352 "MMISMS: SendMsgReqToMN mn_err_code = %d"
#define MMISMS_SEND_1153_112_2_18_2_53_19_353 "MMISMS:MMISMS_SendMsgBySeqNum send_info_ptr = PNULL"
#define MMISMS_SEND_1161_112_2_18_2_53_19_354 "MMISMS: MMISMS_SendMsgBySeqNum seq_num = %d, dual_sys = %d"
#define MMISMS_SEND_1167_112_2_18_2_53_19_355 "MMISMS:MMISMS_SendMsgBySeqNum seq_num > GET_HEAD_MAX_NUM()"
#define MMISMS_SEND_1242_112_2_18_2_53_19_356 "MMISMS:MMISMS_SendMsgBySeqNum !MMISMS_GetDestNumber()"
#define MMISMS_SEND_1257_112_2_18_2_53_19_357 "MMISMS: CloseSendWinByResult(%d)"
#define MMISMS_SEND_1286_112_2_18_2_53_19_358 "MMISMS:MMISMS_ClearDestAddrById PNULL ==  send_info_ptr"
#define MMISMS_SEND_1347_112_2_18_2_53_19_359 "MMISMS:SendFailCount PNULL ==  send_info_ptr"
#define MMISMS_SEND_1370_112_2_18_2_53_19_360 "MMISMS:MMISMS_GetDestNumber dest_info_ptr = PNULL"
#define MMISMS_SEND_1376_112_2_18_2_53_19_361 "MMISMS:MMISMS_GetDestNumber dest_number_ptr = PNULL"
#define MMISMS_SEND_1413_112_2_18_2_53_19_362 "MMISMS:MMISMS_SplitLongMsg sms_content_ptr = PNULL"
#define MMISMS_SEND_1491_112_2_18_2_53_19_363 "MMISMS:MMISMS_CheckNextDestAddr send_info_ptr = PNULL"
#define MMISMS_SEND_1499_112_2_18_2_53_19_364 "MMISMS: MMISMS_CheckNextDestAddr: the number is = %d, the addr id = %d"
#define MMISMS_SEND_1521_112_2_18_2_53_19_365 "MMISMS:MMISMS_IsExistNextSMS send_info_ptr = PNULL"
#define MMISMS_SEND_1533_112_2_18_2_53_19_366 "MMISMS_IsExistNextSMS:max_num is %d,seq_num is %d"
#define MMISMS_SEND_1542_112_2_18_2_53_19_367 "SMS: IsExistNextSMS %d"
#define MMISMS_SEND_1570_112_2_18_2_53_19_368 "MMISMS:MMISMS_InitUserDataHead data_head_ptr = PNULL"
#define MMISMS_SEND_1575_112_2_18_2_53_19_369 "MMISMS:MMISMS_InitUserDataHead 0 >= max_num || max_num > MMISMS_SPLIT_MAX_NUM"
#define MMISMS_SEND_1580_112_2_18_2_53_19_370 "MMISMS:MMISMS_InitUserDataHead 0 >= seq_num || seq_num > max_num"
#define MMISMS_SEND_1584_112_2_18_2_53_19_371 "MMISMS: MMISMS_InitUserDataHead(%d, %d)"
#define MMISMS_SEND_1641_112_2_18_2_53_20_372 "mmisms send_info_ptr null"
#define MMISMS_SEND_1663_112_2_18_2_53_20_373 "MMISMS:MMISMS_MoveDestAddrId send_info_ptr = PNULL"
#define MMISMS_SEND_1690_112_2_18_2_53_20_374 "MMISMS:MMISMS_GetResultofSending status_id_ptr dest_count_ptr = PNULL"
#define MMISMS_SEND_1697_112_2_18_2_53_20_375 "MMISMS:MMISMS_GetResultofSending send_info_ptr = PNULL"
#define MMISMS_SEND_1702_112_2_18_2_53_20_376 "MMISMS: MMISMS_GetResultofSending dest_addr_id = %d"
#define MMISMS_SEND_1714_112_2_18_2_53_20_377 "MMISMS: MMISMS_GetResultofSending: list_num = %d"
#define MMISMS_SEND_1715_112_2_18_2_53_20_378 "MMISMS: MMISMS_GetResultofSending: dest_count = %d"
#define MMISMS_SEND_1789_112_2_18_2_53_20_379 "MMISMS:MMISMS_ShowSendResult fail_id_ptr = PNULL"
#define MMISMS_SEND_1792_112_2_18_2_53_20_380 "MMISMS: MMISMS_ShowSendResult  fail_count = %d, id_ptr = %p "
#define MMISMS_SEND_1890_112_2_18_2_53_20_381 "MMISMS:MMISMS_HandleSendCnf sig_ptr = PNULL"
#define MMISMS_SEND_1901_112_2_18_2_53_20_382 "MMISMS:MMISMS_HandleSendCnf sig_ptr->cause = %d"
#define MMISMS_SEND_1964_112_2_18_2_53_20_383 "MMISMS:MMISMS_AppSendSms send_info_ptr = PNULL"
#define MMISMS_SEND_1980_112_2_18_2_53_20_384 "MMISMS: MMISMS_AppSendSms is_need_save = %d"
#define MMISMS_SEND_1983_112_2_18_2_53_20_385 "MMISMS: MMISMS_AppSendSms total_num = %d"
#define MMISMS_SEND_1992_112_2_18_2_53_20_386 "MMISMS: MMISMS_AppSendSms send_result = %d"
#define MMISMS_SEND_2008_112_2_18_2_53_20_387 " MMISMS_HandleSendCnfWhenWriteCnf: send_status = %d"
#define MMISMS_SEND_2013_112_2_18_2_53_20_388 "MMISMS:MMISMS_HandleSendCnfWhenWriteCnf send_info_ptr = PNULL"
#define MMISMS_SEND_2221_112_2_18_2_53_21_389 "MMISMS:MMISMS_SetAddressToMessage num_ptr = PNULL"
#define MMISMS_SEND_2226_112_2_18_2_53_21_390 "MMISMS:MMISMS_SetAddressToMessage count = %d"
#define MMISMS_SEND_2268_112_2_18_2_53_21_391 "MMISMS: MMISMS_GetDestCount count = %d"
#define MMISMS_SEND_2289_112_2_18_2_53_21_392 "MMISMS:MMISMS_GetPointedDestNum num_len_ptr num_ptr = PNULL"
#define MMISMS_SEND_2333_112_2_18_2_53_21_393 "MMISMS:MMISMS_DeleteDestNum send_info_ptr = PNULL"
#define MMISMS_SEND_2392_112_2_18_2_53_21_394 "MMISMS_GetDeliverResendFlag: g_mmisms_global.edit_content.is_resend = %d"
#define MMISMS_SEND_2404_112_2_18_2_53_21_395 "MMISMS_SetDeliverResendFlag: is_resend = %d"
#define MMISMS_SEND_2423_112_2_18_2_53_21_396 "MMISMS:MMISMS_GetSMSContent string_ptr = PNULL"
#define MMISMS_SEND_2472_112_2_18_2_53_21_397 "MMISMS_R8:MMISMS_GetSMSContent language is %d,is_lock_shift is %d,is_single_shift is %d"
#define MMISMS_SEND_2485_112_2_18_2_53_21_398 "MMISMS_R8:MMISMS_GetSMSContent content_len is %d"
#define MMISMS_SEND_2578_112_2_18_2_53_21_399 " MMISMS_WintabSendSMS (PNULL == send_info_ptr"
#define MMISMS_SEND_2696_112_2_18_2_53_22_400 "MMISMS:MMISMS_WintabSendVcard p_temp = PNULL"
#define MMISMS_SEND_2708_112_2_18_2_53_22_401 "MMISMS_WintabSendVcard, err_code = %d"
#define MMISMS_SEND_2737_112_2_18_2_53_22_402 "MMISMS:MMISMS_WintabSendVcalendar p_temp = PNULL"
#define MMISMS_SEND_2749_112_2_18_2_53_22_403 "MMISMS_WintabSendVcalendar, err_code = %d"
#define MMISMS_SEND_2767_112_2_18_2_53_22_404 "MMISMS:MMISMS_AppSendSmsForVcardAndVcal sms_init_ptr sms_init_ptr->wstr_ptr = PNULL"
#define MMISMS_SEND_2771_112_2_18_2_53_22_405 "mmivcard:MMISMS_AppSendSmsForVcardAndVcal"
#define MMISMS_SEND_2777_112_2_18_2_53_22_406 "MMISMS MMISMS_AppSendSmsForVcardAndVcal PNULL == send_message"
#define MMISMS_SEND_2844_112_2_18_2_53_22_407 "MMISMS:MMISMS_HandleWriteError send_info_ptr = PNULL"
#define MMISMS_SEND_2903_112_2_18_2_53_22_408 "MMISMS:Set_Longsms_Head_Var head_ptr = PNULL"
#define MMISMS_SEND_2907_112_2_18_2_53_22_409 "SMS: Set_Longsms_Head_Var ref_num=%d,max_num=%d,seq_num=%d"
#define MMISMS_SEND_2936_112_2_18_2_53_22_410 "MMISMS:Set_SmsR8_Head_Var head_ptr = PNULL"
#define MMISMS_SEND_2975_112_2_18_2_53_22_411 "MMISMS:Get_Current_Ref_Num edit_content_ptr = PNULL"
#define MMISMS_SEND_2989_112_2_18_2_53_22_412 "SMS: Get_Current_Ref_Num =%d"
#define MMISMS_SEND_3009_112_2_18_2_53_22_413 "MMISMS:Get_Head_Max_Num edit_content_ptr = PNULL"
#define MMISMS_SEND_3028_112_2_18_2_53_22_414 "SMS: Get_Current_Max_Num =%d"
#define MMISMS_SEND_3048_112_2_18_2_53_22_415 "MMISMS:Get_Current_Seq_Num edit_content_ptr = PNULL"
#define MMISMS_SEND_3062_112_2_18_2_53_22_416 "SMS: Get_Current_Seq_Num =%d"
#define MMISMS_SEND_3083_112_2_18_2_53_22_417 "MMISMS:Set_Current_Seq_Num head_ptr = PNULL"
#define MMISMS_SEND_3086_112_2_18_2_53_22_418 "SMS: Set_Current_Seq_Num seq_num=%d"
#define MMISMS_SEND_3125_112_2_18_2_53_23_419 "MMISMS:MMISMS_GetMnCalledNum dest_ptr party_number_ptr dest_number_ptr = PNULL"
#define MMISMS_SEND_3160_112_2_18_2_53_23_420 "MMISMS:MMISMS_SetDeliverCancel send_info_ptr = PNULL"
#define MMISMS_SEND_3163_112_2_18_2_53_23_421 "MMISMS: MMISMS_SetDeliverCancel=%d"
#define MMISMS_SEND_3180_112_2_18_2_53_23_422 "MMISMS:MMISMS_GetDeliverCancel send_info_ptr = PNULL"
#define MMISMS_SEND_3184_112_2_18_2_53_23_423 "MMISMS: MMISMS_GetDeliverCancel=%d"
#define MMISMS_SEND_3200_112_2_18_2_53_23_424 "MMISMS: MMISMS_ResponseSmsCtl dual_sys=%d, is_need_modified=%d"
#define MMISMS_SEND_3220_112_2_18_2_53_23_425 "MMISMS MMISMS_ResponseSmsCtl PNULL == send_message"
#define MMISMS_SEND_3266_112_2_18_2_53_23_426 "[MMISMS] MMISMS_ResponseSmsCtl Success!"
#define MMISMS_SEND_3293_112_2_18_2_53_23_427 "[MMISMS] alloc fail"
#define MMISMS_SEND_3305_112_2_18_2_53_23_428 "[MMISMS] MMISMS_CreateSendSMS Success!"
#define MMISMS_SEND_3376_112_2_18_2_53_23_429 "[MMISMS] MMISMS_AddSendList status error  !"
#define MMISMS_SEND_3386_112_2_18_2_53_23_430 "[MMISMS] MMISMS_AddSendList s_mmisms_sendlist.send_num = %d!"
#define MMISMS_SEND_3398_112_2_18_2_53_23_431 "[MMISMS] MMISMS_AddSendList the same one!"
#define MMISMS_SEND_3531_112_2_18_2_53_23_432 "[MMISMS] MMISMS_DeleteSendList no sms delete!"
#define MMISMS_SEND_3568_112_2_18_2_53_23_433 "[MMISMS] MMISMS_DeleteSendList s_mmisms_sendlist.send_num = %d!"
#define MMISMS_SEND_3611_112_2_18_2_53_24_434 "MMISMS:MMISMS_GetSendInfoFromSendListByRecordId send_list_num >=  MMISMS_SENDLIST_MAX_NUM"
#define MMISMS_SEND_3623_112_2_18_2_53_24_435 "MMISMS:MMISMS_GetSendInfoFromSendListByRecordId send_list_num ==  %d"
#define MMISMS_SEND_3652_112_2_18_2_53_24_436 "MMISMS:MMISMS_SetBackSendStatus longsms_index >=  MMISMS_SPLIT_MAX_NUM"
#define MMISMS_SEND_3673_112_2_18_2_53_24_437 "MMISMS:MMISMS_SetBackSendStatus longsms_index >=  MMISMS_SPLIT_MAX_NUM"
#define MMISMS_SEND_3697_112_2_18_2_53_24_438 "[MMISMS] MMISMS_ActiveSendList PNULL == send_info_ptr!"
#define MMISMS_SEND_3700_112_2_18_2_53_24_439 "[MMISMS] MMISMS_ActiveSendList send_info_ptr->backsend_status = %d !"
#define MMISMS_SEND_3781_112_2_18_2_53_24_440 "[MMISMS] MMISMS_AppSendSmsToMn sms no dest address!"
#define MMISMS_SEND_3784_112_2_18_2_53_24_441 "[MMISMS] MMISMS_AppSendSmsToMn send_result = %d"
#define MMISMS_SEND_3838_112_2_18_2_53_24_442 "MMISMS MMISMS_SendSmsForOther PNULL == send_data_ptr || PNULL == sms_content_ptr"
#define MMISMS_SEND_3843_112_2_18_2_53_24_443 "MMISMS_SendSmsForOther dual_sys = %d src_port = %d dst_port = %d"
#define MMISMS_SEND_3849_112_2_18_2_53_24_444 "MMISMS MMISMS_SendSmsForOther PNULL == dest_addr_ptr || PNULL == sms_content_ptr"
#define MMISMS_SEND_3856_112_2_18_2_53_24_445 " MMISMS_SendSmsForOther sim is not ok,can't send sms!"
#define MMISMS_SEND_3864_112_2_18_2_53_24_446 "MMISMS MMISMS_SendSmsForOther PNULL == send_info_ptr"
#define MMISMS_SEND_3922_112_2_18_2_53_24_447 "MMISMS_SendSmsForOther err_code == %d"
#define MMISMS_SEND_3944_112_2_18_2_53_24_448 "MMISMS:InitUserDataHeadForOtherModules data_head_ptr = PNULL"
#define MMISMS_SEND_3948_112_2_18_2_53_24_449 "MMISMS: InitUserDataHeadForOtherModules (srcPort:%d, dstPort:%d)"
#define MMISMS_SEND_3990_112_2_18_2_53_24_450 "MMISMS:MMISMS_UpdateSmsSendindNumber send_info_ptr = PNULL"
#define MMISMS_SEND_3995_112_2_18_2_53_24_451 "MMISMS:MMISMS_UpdateSmsSendindNumber backsend_status != MMISMS_SENDSTATUS_SENDING"
#define MMISMS_SEND_4006_112_2_18_2_53_24_452 "MMISMS:MMISMS_UpdateSmsSendindNumber dest_addr_id = %d, update_record_id = %d"
#define MMISMS_SEND_4041_112_2_18_2_53_25_453 "MMISMS: MMISMS_UpdateCurSendRecordId 11 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d, sending_record_id = %d"
#define MMISMS_SEND_4194_112_2_18_2_53_25_454 "MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM"
#define MMISMS_SEND_4230_112_2_18_2_53_25_455 "MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM"
#define MMISMS_SET_279_112_2_18_2_53_26_456 "MMISMS: MMISMS_SetRefNumToNV ref_num = %d"
#define MMISMS_SET_356_112_2_18_2_53_26_457 "MMISMS:MMISMS_GetFrequentWord word_ptr = PNULL"
#define MMISMS_SET_379_112_2_18_2_53_26_458 "MMISMS:MMISMS_ModifyPointedWord item_phase_ptr = PNULL"
#define MMISMS_SET_512_112_2_18_2_53_26_459 "MMISMS:MMISMS_AppGetSCAddr sc_addr sc_addr_len = PNULL"
#define MMISMS_SET_615_112_2_18_2_53_27_460 "MMISMS:MMISMS_DecodeUserDataHead user_data_header_ptr_t user_head_ptr_t = PNULL"
#define MMISMS_SET_663_112_2_18_2_53_27_461 "MMISMS:MMISMS_FreeUserDataHeadSpace user_head_ptr_t = PNULL"
#define MMISMS_SET_750_112_2_18_2_53_27_462 "MMISMS:MMISMS_Default2NationalR8 default_ptr national_ptr = PNULL"
#define MMISMS_SET_858_112_2_18_2_53_27_463 "MMISMS:MMISMS_Default2NationalR8 default_ptr national_ptr = PNULL"
#define MMISMS_SET_1590_112_2_18_2_53_29_464 "MMIAPISET_SetTTSInfo cur_selection = %d"
#define MMISMS_SETTINGWIN_714_112_2_18_2_53_31_465 "HandleSmsTTSVolSetWindow msg_id = %d"
#define MMISMS_SETTINGWIN_1015_112_2_18_2_53_32_466 "InitSettingBrowser:is_open is %d"
#define MMISMS_SETTINGWIN_1992_112_2_18_2_53_34_467 "InitSettingBrowser:is_open is %d"
#define MMISMS_SETTINGWIN_2277_112_2_18_2_53_34_468 "MMISMS: HandleSettingSmsStatusWinMsg: menu_id = %d"
#define MMISMS_SETTINGWIN_2686_112_2_18_2_53_35_469 "SetSIMMode:i = %d,alpha_id_present = %d,len = %d,"
#define MMISMS_SETTINGWIN_3428_112_2_18_2_53_36_470 "MMISMS:ConvertSimToModeName sim_alpha_ptr 0x%x"
#define MMISMS_SETTINGWIN_3444_112_2_18_2_53_36_471 "MMISMS:ConvertSimToModeName decode alphabet with sim standard is error"
#define MMISMS_SETTINGWIN_3503_112_2_18_2_53_37_472 "HandleSettingConcatenatedSMSWinMsg msg_id is 0x%x"
#define MMISMS_SETTINGWIN_3514_112_2_18_2_53_37_473 "HandleSettingConcatenatedSMSWinMsg is_contenated_sms=%d"
#define MMISMS_SETTINGWIN_3598_112_2_18_2_53_37_474 "HandleSettingAlphabetManagementWinMsg msg_id is 0x%x"
#define MMISMS_SETTINGWIN_3611_112_2_18_2_53_37_475 "HandleSettingAlphabetManagementWinMsg alpha_management=%d"
#define MMISMS_TIMERMSG_1355_112_2_18_2_53_40_476 "HandleEditTimerMsgWinMsg: msg_id = %d"
#define MMISMS_TIMERMSG_1713_112_2_18_2_53_41_477 "HandleEditTimerMsgWinMsg:  MSG_CTL_PENOK event_fast_info_ptr->fre_mode = %d"
#define MMISMS_TIMERMSG_2085_112_2_18_2_53_41_478 "DisplaySystemDate:date_type %d is error!"
#define MMISMS_TIMERMSG_2119_112_2_18_2_53_41_479 "DisplaySystemDate:date_type %d is error!"
#define MMISMS_TIMERMSG_2601_112_2_18_2_53_43_480 "MMISMS:MMISMS_SetIsTimerMode: g_mmisms_global.is_timer_mode =%d, is_timer_mode=%d"
#define MMISMS_TIMERMSG_2617_112_2_18_2_53_43_481 "MMISMS:MMISMS_GetIsTimerMode: g_mmisms_global.is_timer_mode =%d"
#define MMISMS_TIMERMSG_2628_112_2_18_2_53_43_482 "MMISMS:MMISMS_SetIsTimerMsg: g_mmisms_global.is_timer_msg =%d, is_timer_msg=%d"
#define MMISMS_TIMERMSG_2645_112_2_18_2_53_43_483 "MMISMS:MMISMS_GetIsTimerMsg: g_mmisms_global.is_timer_msg =%d"
#define MMISMS_TIMERMSG_2797_112_2_18_2_53_43_484 "MMISMS: IsSmsOperBusy is_sms_sending=%d, is_sms_deleting=%d, is_sms_saving=%d, is_sms_reading=%d, is_sms_moving=%d, is_sms_exporting=%d, is_sms_copying=%d"
#define MMISMS_TIMERMSG_2838_112_2_18_2_53_43_485 "MMISMS: MMISMS_TIMERMSG_CheckTimerMSG s_mmisms_timer_sms_timer_id=%d, is_direct=%d"
#define MMISMS_TIMERMSG_2888_112_2_18_2_53_43_486 "MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback svc_id=%d, svc_type=%d"
#define MMISMS_TIMERMSG_2924_112_2_18_2_53_43_487 "MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback dual_sys=%d, is_fly_mode=%d, is_order_ok=%d, is_sms_sending=%d, cc=%d, pdp=%d, ussd=%d, ss=%d, cur_oper_status=%d, is_cs_available=%d"
#define MMISMS_TIMERMSG_2927_112_2_18_2_53_43_488 "MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback dual_sys=%d, is_fly_mode=%d, is_order_ok=%d, is_sms_sending=%d, cc=%d, ussd=%d, ss=%d, cur_oper_status=%d, is_cs_available=%d"
#define MMISMS_TIMERMSG_2956_112_2_18_2_53_43_489 "MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback not allow send sms now"
#define MMISMS_TIMERMSG_3010_112_2_18_2_53_43_490 "MMISMS_TIMERMSG_ContentNotifyCallback %d-%d-%d %d:%d"
#define MMISMS_TIMERMSG_3108_112_2_18_2_53_44_491 "TIMERMSG_InitContentNotifyInfo %d-%d-%d %d:%d"
#define MMISMS_TIMERMSG_3110_112_2_18_2_53_44_492 "TIMERMSG_InitContentNotifyInfo before update cursor->event_id = %d"
#define MMISMS_TIMERMSG_3127_112_2_18_2_53_44_493 "TIMERMSG_InitContentNotifyInfo after update cursor->event_id = %d"
#define MMISMS_TIMERMSG_3205_112_2_18_2_53_44_494 "TIMERMSG_UpdateAlarm %d-%d-%d %d:%d"
#define MMISMS_TIMERMSG_3207_112_2_18_2_53_44_495 "TIMERMSG_UpdateAlarm before update cursor->event_id = %d"
#define MMISMS_TIMERMSG_3224_112_2_18_2_53_44_496 "TIMERMSG_UpdateAlarm after update cursor->event_id = %d"
#define MMISMS_VOICEMAIL_115_112_2_18_2_53_44_497 "MMISMS_HandleSetVoiMessAddrCnf: record_id is error"
#define MMISMS_VOICEMAIL_236_112_2_18_2_53_44_498 "MMISMS:MMISMS_HandleGetVoiMessAddrCnf get_vm_addr_info.dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMS_VOICEMAIL_239_112_2_18_2_53_44_499 "MMISMS:MMISMS_HandleGetVoiMessAddrCnf:get APP_MN_GET_VMW_ADDR_CNF.The result is %d,record  id is %d"
#define MMISMS_VOICEMAIL_315_112_2_18_2_53_45_500 "MMISMS: MMISMS_HandleGetVoiMessAddrCnf: get voice addr from sim card"
#define MMISMS_VOICEMAIL_319_112_2_18_2_53_45_501 "MMISMS: MMISMS_HandleGetVoiMessAddrCnf: THE record_id is wrong"
#define MMISMS_VOICEMAIL_353_112_2_18_2_53_45_502 " the menu id is error"
#define MMISMS_VOICEMAIL_373_112_2_18_2_53_45_503 "HandleGetVoiMessAddrCnf: get the voice addr for the voicemail box!"
#define MMISMS_VOICEMAIL_465_112_2_18_2_53_45_504 "MMISMS: MMISMS_HandleGetVoiMessFlagCnf: if there is new voice message = %d"
#define MMISMS_VOICEMAIL_523_112_2_18_2_53_45_505 "MMISMS:MMISMS_GetVoiceMailAddr mail_len_ptr voice_mail_ptr = PNULL"
#define MMISMS_VOICEMAIL_621_112_2_18_2_53_45_506 "MMISMS:MMISMS_SetVoiceMailAddr voice_mail_ptr = PNULL"
#define MMISMSAPP_MAIN_570_112_2_18_2_53_47_507 "MMISMS MMISMS_IsMtForJava: i value is = %d  .iei=%d"
#define MMISMSAPP_MAIN_586_112_2_18_2_53_47_508 "MMISMS MMISMS_IsMtForJava: There is no java port number!"
#define MMISMSAPP_MAIN_605_112_2_18_2_53_47_509 "MMISMS MMISMS_IsMtForJava: port_test1 is = %d, tport_test2 is = %d "
#define MMISMSAPP_MAIN_618_112_2_18_2_53_47_510 "MMISMS MMISMS_IsMtForJava: MT Ind the des port num is = %d, the src port is = %d "
#define MMISMSAPP_MAIN_624_112_2_18_2_53_47_511 "MMISMS MMISMS_IsMtForJava: the port result is = %d"
#define MMISMSAPP_MAIN_638_112_2_18_2_53_47_512 "[MMISMS]: HandleSIMPlugNotifySMSFunc dual_sys=%d, notify_event=%d"
#define MMISMSAPP_MAIN_704_112_2_18_2_53_47_513 "[MMISMS]: HandleSMSNotifyEventCallback dual_sys=%d, notify_event=%d, param=%d"
#define MMISMSAPP_MAIN_868_112_2_18_2_53_47_514 "MMISMS:HandlePsMsg app_ptr = PNULL"
#define MMISMSAPP_MAIN_871_112_2_18_2_53_47_515 "MMISMS:HandlePsMsg, msg_id=%x"
#define MMISMSAPP_MAIN_877_112_2_18_2_53_47_516 "HandlePsMsg APP_MN_SMS_READY_IND"
#define MMISMSAPP_MAIN_881_112_2_18_2_53_47_517 "SMS: receive APP_MN_SMS_READY_IND"
#define MMISMSAPP_MAIN_886_112_2_18_2_53_47_518 "MMISMS: receive APP_MN_SIM_NOT_READY_IND"
#define MMISMSAPP_MAIN_892_112_2_18_2_53_47_519 "MMISMS: receive APP_MN_READ_SMS_CNF"
#define MMISMSAPP_MAIN_897_112_2_18_2_53_47_520 "MMISMS: receive APP_MN_UPDATE_SMS_STATE_CNF"
#define MMISMSAPP_MAIN_925_112_2_18_2_53_48_521 "mmi receive APP_MN_SMS_MEM_OVER_FLOW_IND"
#define MMISMSAPP_MAIN_1309_112_2_18_2_53_48_522 "MMISMS_GetSendPath preferred_connection=%d"
#define MMISMSAPP_MAIN_1373_112_2_18_2_53_48_523 "MMISMS_GetSavePos is_sim_first is %d"
#define MMISMSAPP_MAIN_1385_112_2_18_2_53_48_524 "MMISMS_GetSavePos mmi_save_pos is %d"
#define MMISMSAPP_MAIN_1397_112_2_18_2_53_48_525 "MMISMS_GetSavePos save_pos is %d"
#define MMISMSAPP_MAIN_1442_112_2_18_2_53_49_526 "MMISMS:HandleSetSMSReceiveCnf sig_ptr = PNULL"
#define MMISMSAPP_MAIN_1445_112_2_18_2_53_49_527 "MMISMS: HandleSetSMSReceiveCnf cause = %d, s_resend_limit=%d, s_last_send_time=%d"
#define MMISMSAPP_MAIN_1496_112_2_18_2_53_49_528 "MMISMS: MMISMS_IsSMSReady dual_sys=%d"
#define MMISMSAPP_MAIN_1499_112_2_18_2_53_49_529 " MMISMS: MMISMS_IsSMSReady[%d]=%d"
#define MMISMSAPP_MAIN_1539_112_2_18_2_53_49_530 "MMISMS: GetRefNumFormNV ref_num = %d"
#define MMISMSAPP_MAIN_1650_112_2_18_2_53_49_531 "MMISMS:MMISMS_GetOriginNum dual_sys_ptr num_ptr = PNULL"
#define MMISMSAPP_MAIN_1685_112_2_18_2_53_49_532 "MMISMS:MMISMS_GetOriginNumByPos dual_sys_ptr num_ptr = PNULL"
#define MMISMSAPP_MAIN_1738_112_2_18_2_53_49_533 "MMISMS_SetOperStatus oper_status is %d, g_mmisms_global.operation.oper_status = %d"
#define MMISMSAPP_MAIN_1753_112_2_18_2_53_49_534 "MMISMS_SetMoveOperStatus oper_status is %d"
#define MMISMSAPP_MAIN_1768_112_2_18_2_53_49_535 "MMISMS:MMISMS_SetCurOrderId index >= MMISMS_SPLIT_MAX_NUM"
#define MMISMSAPP_MAIN_1784_112_2_18_2_53_49_536 "MMISMS:MMISMS_SetCurOrderId index >= MMISMS_SPLIT_MAX_NUM"
#define MMISMSAPP_MAIN_1808_112_2_18_2_53_49_537 "MMISMS_GetCurOperationType cur_type = %d"
#define MMISMSAPP_MAIN_1825_112_2_18_2_53_49_538 "MMISMS:HandleMemFullInd sig_ptr = PNULL"
#define MMISMSAPP_MAIN_1832_112_2_18_2_53_49_539 "SMS: HandleMemFullInd mem_status = %d, unread_count = %d"
#define MMISMSAPP_MAIN_1857_112_2_18_2_53_49_540 "MMISMS: MMISMS_DisplayPrompByErr err_code = %d"
#define MMISMSAPP_MAIN_1904_112_2_18_2_53_50_541 "MMISMS: MMISMS_DisplayUpdatePrompt(%d)"
#define MMISMSAPP_MAIN_1987_112_2_18_2_53_50_542 "MMISMS_GetCurrentOperStatus: oper_status = %d"
#define MMISMSAPP_MAIN_1997_112_2_18_2_53_50_543 "MMISMS_GetCurrentMoveOperStatus: oper_status = %d"
#define MMISMSAPP_MAIN_2075_112_2_18_2_53_50_544 "MMISMS_ClearOperInfo"
#define MMISMSAPP_MAIN_2111_112_2_18_2_53_50_545 "MMISMS:MMISMS_SetCurOperationOrderId order_id = PNULL"
#define MMISMSAPP_MAIN_2160_112_2_18_2_53_50_546 "MMISMS:MMISMS_SetCurOperationOrderId order_id = PNULL"
#define MMISMSAPP_MAIN_2257_112_2_18_2_53_50_547 "MMISMS_IsOrderOk order_status = 0x%x"
#define MMISMSAPP_MAIN_2309_112_2_18_2_53_50_548 "MMISMS:StartAppInitSmsList dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_MAIN_2364_112_2_18_2_53_50_549 "MMISMS:MMISMS_SetCurSaveDualSys dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_MAIN_2376_112_2_18_2_53_51_550 "APP_MN_SMS_READY_IND (dual_sys=%d)"
#define MMISMSAPP_MAIN_2393_112_2_18_2_53_51_551 "MMISMS:MMISMS_SetCurVoiceMailDualSys dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_MAIN_2418_112_2_18_2_53_51_552 "MMISMS:MMISMS_SetCurSendDualSys dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_MAIN_2454_112_2_18_2_53_51_553 "MMISMS:MMISMS_SetCurSaveDualSys dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_MAIN_2499_112_2_18_2_53_51_554 "MMISMS:MMISMS_HandleSimNotReadyInd dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_MAIN_2502_112_2_18_2_53_51_555 "MMISMS: MMISMS_HandleSimNotReadyInd sim_not_ready_ind reason = %d"
#define MMISMSAPP_MAIN_2536_112_2_18_2_53_51_556 "MMISMS_RefreshSMS: dual_sys=%d"
#define MMISMSAPP_MAIN_2566_112_2_18_2_53_51_557 "MMISMS_SetIsNeedLoadSaved:is_need_load_saved=%d"
#define MMISMSAPP_MAIN_2577_112_2_18_2_53_51_558 "MMISMS_GetCurSaveDualSys:is_need_load_saved=%d"
#define MMISMSAPP_MAIN_2588_112_2_18_2_53_51_559 "MMISMS_SetIsSendFail:is_send_fail=%d"
#define MMISMSAPP_MAIN_2599_112_2_18_2_53_51_560 "MMISMS_GetIsSendFail:is_send_fail=%d"
#define MMISMSAPP_MAIN_2617_112_2_18_2_53_51_561 "MMISMS:MMISMS_SetCurOperationOrderId message_content_ptr display_content_ptr = PNULL"
#define MMISMSAPP_MAIN_2675_112_2_18_2_53_51_562 "MMISMS:MMISMS_GetMaxSimSmsNum dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_MAIN_2689_112_2_18_2_53_51_563 "MMISMS:MMISMS_SetCurSaveDualSys dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_MAIN_2796_112_2_18_2_53_51_564 "MMISMS:MMISMS_SetCurSearchOperationOrderId order_id = PNULL"
#define MMISMSAPP_MAIN_2813_112_2_18_2_53_51_565 "MMISMS_GetIsNeedReSort s_is_sms_need_re_sort=%d"
#define MMISMSAPP_MAIN_2824_112_2_18_2_53_51_566 "MMISMS_SetIsNeedReSort is_need_re_sort=%d"
#define MMISMSAPP_MAIN_2840_112_2_18_2_53_51_567 "MMISMS_GetIsSMSRefresh s_is_sms_refresh[%d]=%d"
#define MMISMSAPP_MAIN_2856_112_2_18_2_53_51_568 "MMISMS_SetIsSMSRefresh is_sms_refresh[%d]=%d"
#define MMISMSAPP_MAIN_2958_112_2_18_2_53_52_569 "MMISMS:MMISMS_SetChatCurOperationOrderId order_id = PNULL"
#define MMISMSAPP_MAIN_2998_112_2_18_2_53_52_570 "MMISMS:MMISMS_SetChatCurOperationOrderId order_id = PNULL"
#define MMISMSAPP_MAIN_3106_112_2_18_2_53_52_571 "MMISMS: MMISMS_GetAlphabetManagement alpha_management=%d"
#define MMISMSAPP_MAIN_3328_112_2_18_2_53_52_572 "MMISMS:MMISMS_IKeyBackup sms_total_num = %d, item_data_req_ptr->number = %d"
#define MMISMSAPP_MAIN_3460_112_2_18_2_53_53_573 "MMISMS: MMISMS_HandleUserReadSMSCnf head_len = %d"
#define MMISMSAPP_ORDER_522_112_2_18_2_53_54_574 "MMISMS:MMISMS_GetSmsStateBoxtype sms_state_ptr = PNULL"
#define MMISMSAPP_ORDER_555_112_2_18_2_53_54_575 "GetSmsStateBoxtype Error mo_mt_type=%d"
#define MMISMSAPP_ORDER_569_112_2_18_2_53_54_576 "MMISMS:IsSameLongSmsPart sms1_state_ptr sms2_state_ptr = PNULL"
#define MMISMSAPP_ORDER_600_112_2_18_2_53_54_577 "MMISMS:CompareSmsState sms1_state_ptr sms2_state_ptr = PNULL"
#define MMISMSAPP_ORDER_605_112_2_18_2_53_54_578 "MMISMS:CompareSmsState MMISMS_MT_TYPE != type && MMISMS_MO_TYPE != type"
#define MMISMSAPP_ORDER_655_112_2_18_2_53_54_579 "InsertNode:new_state_ptr->longsms_max_num = %d,new_state_ptr->longsms_seq_num = %d,new_state_ptr->head_ref = %d,msg_type=%d"
#define MMISMSAPP_ORDER_883_112_2_18_2_53_55_580 "MMISMS:MMISMS_GetMsgType cur_order_id = PNULL"
#define MMISMSAPP_ORDER_888_112_2_18_2_53_55_581 "MMISMS: MMISMS_GetMsgType msg_type is %d"
#define MMISMSAPP_ORDER_929_112_2_18_2_53_55_582 "MMISMS:MMISMS_GetCurMsgIsMarked order_id = PNULL"
#define MMISMSAPP_ORDER_1113_112_2_18_2_53_56_583 "MMISMS:MMISMS_SetPointedMsgMarked dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_ORDER_1120_112_2_18_2_53_56_584 "MMISMS:MMISMS_SetPointedMsgMarked order_id = PNULL"
#define MMISMSAPP_ORDER_1169_112_2_18_2_53_56_585 "MMISMS:MMISMS_GetPointedSMSMemInfo storage_ptr record_id_ptr = PNULL"
#define MMISMSAPP_ORDER_1174_112_2_18_2_53_56_586 "MMISMS:MMISMS_GetPointedSMSMemInfo 0 = s_mmisms_order.max_sms_num.total_msg_num"
#define MMISMSAPP_ORDER_1187_112_2_18_2_53_56_587 "MMISMS: MMISMS_GetPointedSMSMemInfo: order_id is PNULL"
#define MMISMSAPP_ORDER_1210_112_2_18_2_53_56_588 "MMISMS:MMISMS_GetOrderIdByStorage order_id = PNULL"
#define MMISMSAPP_ORDER_1216_112_2_18_2_53_56_589 "MMISMS: MMISMS_GetOrderIdByStorage msg_type = %d,storage = %d, record_id= %d"
#define MMISMSAPP_ORDER_1265_112_2_18_2_53_56_590 "MMISMS: MMISMS_GetOrderIdByStorage: Can't find the order id by storage and record id"
#define MMISMSAPP_ORDER_1314_112_2_18_2_53_56_591 "MMISMS_DelSMSByAt(%d, %d, %d)"
#define MMISMSAPP_ORDER_1325_112_2_18_2_53_56_592 "MMISMS:MMISMS_DelSMSByAT status_ptr = PNULL"
#define MMISMSAPP_ORDER_1363_112_2_18_2_53_56_593 "MMISMS:MMISMS_TranslateMNStatus2SFStatus MMISMS_MT_TYPE != type && MMISMS_MO_TYPE != type"
#define MMISMSAPP_ORDER_1388_112_2_18_2_53_56_594 "SMS: MMISMS_TranslateMNStatus2SFStatus ERROR mn_status = %d"
#define MMISMSAPP_ORDER_1418_112_2_18_2_53_56_595 "SMS: MMISMS_TranslateMNStatus2SFStatus ERROR mn_status = %d"
#define MMISMSAPP_ORDER_1537_112_2_18_2_53_56_596 "MMISMS: MMISMS LinkedArrayAllocNode: No array unit to alloc! "
#define MMISMSAPP_ORDER_1554_112_2_18_2_53_56_597 "MMISMS: LinkedArrayFreeNode : s_mmisms_linked_order.used_unit_head_ptr is PNULL "
#define MMISMSAPP_ORDER_1586_112_2_18_2_53_56_598 "MMISMS: LinkedArrayFreeNode : can't find the node in used link!"
#define MMISMSAPP_ORDER_1632_112_2_18_2_53_57_599 "MMISMS: MMISMS_GetTobeReadSmsCount ret_val=%d, update_sms_num=%d"
#define MMISMSAPP_ORDER_1668_112_2_18_2_53_57_600 "MMISMS: MMISMS_GetTobeReadMsgCount ret_val=%d, update_sms_num=%d"
#define MMISMSAPP_ORDER_1744_112_2_18_2_53_57_601 "MMISMS: MMISMS_GetTobeReadMessageCount ret_val=%d, update_sms_num=%d"
#define MMISMSAPP_ORDER_1889_112_2_18_2_53_57_602 "MMISMS:MMISMS_InitSmsOrder"
#define MMISMSAPP_ORDER_2028_112_2_18_2_53_57_603 "MMISMS:MMISMS_InsertSmsToOrder order_info_ptr = PNULL"
#define MMISMSAPP_ORDER_2042_112_2_18_2_53_57_604 "MMISMS_InsertSmsToOrder sim%d sms exceed(%d)!"
#define MMISMSAPP_ORDER_2049_112_2_18_2_53_57_605 "MMISMS_InsertSmsToOrder (%d)MEMORY FULL, RETURN!"
#define MMISMSAPP_ORDER_2057_112_2_18_2_53_57_606 "MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
#define MMISMSAPP_ORDER_2071_112_2_18_2_53_57_607 "MMISMS: MMISMS_InsertSmsToOrder new_state_ptr->flag.mo_mt_type is %d"
#define MMISMSAPP_ORDER_2109_112_2_18_2_53_58_608 "MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
#define MMISMSAPP_ORDER_2172_112_2_18_2_53_58_609 "MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
#define MMISMSAPP_ORDER_2299_112_2_18_2_53_58_610 "MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
#define MMISMSAPP_ORDER_2361_112_2_18_2_53_58_611 "MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
#define MMISMSAPP_ORDER_2500_112_2_18_2_53_58_612 "MMISMS:MMISMS_GetSmsInfoFromOrder order_id order_info_ptr = PNULL"
#define MMISMSAPP_ORDER_2518_112_2_18_2_53_58_613 "MMISMS:MMISMS_UpdateSmsInfoInOrder old_order_id new_order_info_ptr = PNULL"
#define MMISMSAPP_ORDER_2575_112_2_18_2_53_58_614 "MMISMS_GetBoxTotalNumberFromOrder: box_type = %d"
#define MMISMSAPP_ORDER_2781_112_2_18_2_53_59_615 "MMISMS:DeleteNode sms_state_ptr = PNULL"
#define MMISMSAPP_ORDER_2785_112_2_18_2_53_59_616 "DeleteNode: sms_state_ptr->longsms_max_num = %d, sms_state_ptr->longsms_seq_num = %d,sms_state_ptr->head_ref = %d,msg_type=%d"
#define MMISMSAPP_ORDER_2789_112_2_18_2_53_59_617 "MMISMS: DeleteNode: delete_type_head_ptr is PNULL"
#define MMISMSAPP_ORDER_2906_112_2_18_2_53_59_618 "MMISMS: DeleteNode :can't find the sms ptr in long sms link"
#define MMISMSAPP_ORDER_2913_112_2_18_2_53_59_619 "MMISMS DeleteNode: there is no sms have the same ref with the to delete sms1_state_ptr"
#define MMISMSAPP_ORDER_2966_112_2_18_2_53_59_620 "HandleLongSMSOrderMsgNum is_to_be_read=%d,is_insert=%d"
#define MMISMSAPP_ORDER_2969_112_2_18_2_53_59_621 "MMISMS:HandleLongSMSOrderMsgNum total_msg_num_ptr = PNULL"
#define MMISMSAPP_ORDER_3017_112_2_18_2_53_59_622 "HandleOrderMsgNum is_to_be_read=%d,is_insert=%d"
#define MMISMSAPP_ORDER_3020_112_2_18_2_53_59_623 "MMISMS:HandleOrderMsgNum total_msg_num_ptr = PNULL"
#define MMISMSAPP_ORDER_3105_112_2_18_2_54_0_624 "MMISMS: MMISMS_LoadSmsListFromOrder the count of list box is %d"
#define MMISMSAPP_ORDER_3223_112_2_18_2_54_0_625 "MMISMS: MMISMS_ListSmsFromOrder the undownloaded sms count is %d"
#define MMISMSAPP_ORDER_3224_112_2_18_2_54_0_626 "MMISMS: MMISMS_ListSmsFromOrder the unreaded sms count is %d"
#define MMISMSAPP_ORDER_3225_112_2_18_2_54_0_627 "MMISMS: MMISMS_ListSmsFromOrder the readed sms count is %d"
#define MMISMSAPP_ORDER_3276_112_2_18_2_54_0_628 "MMISMS: MMISMS_LoadSmsListFromOrder: the box_type is %d"
#define MMISMSAPP_ORDER_3429_112_2_18_2_54_0_629 "subject.length is %d"
#define MMISMSAPP_ORDER_3689_112_2_18_2_54_1_630 "subject.length is %d"
#define MMISMSAPP_ORDER_3882_112_2_18_2_54_1_631 "MMISMS:FindNameAndStoreInfoByNum bcd_num_ptr = PNULL"
#define MMISMSAPP_ORDER_3885_112_2_18_2_54_1_632 "MMISMS: FindNameAndStoreInfoByNum num_len = %d"
#define MMISMSAPP_ORDER_3901_112_2_18_2_54_1_633 "MMISMS: FindNameAndStoreInfoByNum num_len = %d, pb_num.number_len = %d"
#define MMISMSAPP_ORDER_4219_112_2_18_2_54_2_634 "MMISMS: list_index is %d "
#define MMISMSAPP_ORDER_4223_112_2_18_2_54_2_635 "MMISMS: s_mmisms_order.total_undownloaded_num is %d"
#define MMISMSAPP_ORDER_4224_112_2_18_2_54_2_636 "MMISMS: s_mmisms_order.to_be_read_num is %d"
#define MMISMSAPP_ORDER_4289_112_2_18_2_54_2_637 "MMISMS: MMISMS_LoadSmsListFromOrder: the box_type is %d"
#define MMISMSAPP_ORDER_4293_112_2_18_2_54_2_638 "head_order_id is 0x%x"
#define MMISMSAPP_ORDER_4330_112_2_18_2_54_2_639 "MMISMS:cur_order_id is 0x%x"
#define MMISMSAPP_ORDER_4344_112_2_18_2_54_2_640 "MMISMS: list_index is %d "
#define MMISMSAPP_ORDER_4348_112_2_18_2_54_2_641 "head_order_id is 0x%x"
#define MMISMSAPP_ORDER_4372_112_2_18_2_54_2_642 "MMISMS:cur_order_id is 0x%x"
#define MMISMSAPP_ORDER_4390_112_2_18_2_54_2_643 "MMISMS:MMISMS_InsertSmsToUnhandleOrder new_state_ptr = PNULL"
#define MMISMSAPP_ORDER_5088_112_2_18_2_54_3_644 "MMISMS:MMISMS_GetPointedPosRecordID order_id = PNULL"
#define MMISMSAPP_ORDER_5210_112_2_18_2_54_4_645 "HandleMaxSimSMSNum max_sim_num=%d exceed limit!"
#define MMISMSAPP_ORDER_5259_112_2_18_2_54_4_646 "mmisms:MMISMS_OrderInit max_sms_num=%d"
#define MMISMSAPP_ORDER_5296_112_2_18_2_54_4_647 "MMISMS:CompareStateForSort sms1_state_ptr sms2_state_ptr = PNULL"
#define MMISMSAPP_ORDER_5422_112_2_18_2_54_4_648 "MMISMS:SortOrderList head = PNULL"
#define MMISMSAPP_ORDER_5429_112_2_18_2_54_4_649 "MMISMS:SortOrderList temp_arry alloc fail"
#define MMISMSAPP_ORDER_5621_112_2_18_2_54_5_650 "MMISMS:MMISMS_SetCurMsgLocked order_id = PNULL"
#define MMISMSAPP_ORDER_5759_112_2_18_2_54_5_651 "MMISMS:MMISMS_CurMsgIsLocked order_id = PNULL"
#define MMISMSAPP_ORDER_5812_112_2_18_2_54_5_652 "MMISMS:MMISMS_CurMsgRecordId order_id = PNULL"
#define MMISMSAPP_ORDER_6104_112_2_18_2_54_5_653 "MMISMS_UpdateUnreadOrder:is_need_update_order=%d"
#define MMISMSAPP_ORDER_6328_112_2_18_2_54_6_654 "MMISMS_GetSmsBoxHeaderOrderId delete_sms_num=%d"
#define MMISMSAPP_ORDER_6571_112_2_18_2_54_6_655 "MMISMS:GetOrderLinkOriginCard dual_sys = PNULL"
#define MMISMSAPP_ORDER_6651_112_2_18_2_54_7_656 "MMISMS:MMISMS_GetSMSUsedAndMax max_num_ptr used_num_ptr sms_ori_used_num_ptr = PNULL"
#define MMISMSAPP_ORDER_6669_112_2_18_2_54_7_657 "SMS: MMISMS_GetSMSUsedAndMax dual_sys=%d, is_nv = %d, max_num = %d, used_num = %d,ori=%d"
#define MMISMSAPP_ORDER_6690_112_2_18_2_54_7_658 "MMISMS_SetPointedMsgNumMarked:index is %d,len is %d"
#define MMISMSAPP_ORDER_6704_112_2_18_2_54_7_659 "fengming:sender[%d] is %d,number[%d] is %d"
#define MMISMSAPP_ORDER_6709_112_2_18_2_54_7_660 "MMISMS_SetPointedMsgNumMarked:msg_type is %d"
#define MMISMSAPP_ORDER_6803_112_2_18_2_54_7_661 "MMISMS_SetPointedMsgNumMarked:index is %d,len is %d"
#define MMISMSAPP_ORDER_6817_112_2_18_2_54_7_662 "MMISMS_SetPointedMsgNumMarked:msg_type is %d"
#define MMISMSAPP_ORDER_6918_112_2_18_2_54_7_663 "fengming:sender[%d] is %d"
#define MMISMSAPP_ORDER_6971_112_2_18_2_54_7_664 "fengming:number[%d] is %d"
#define MMISMSAPP_ORDER_6974_112_2_18_2_54_7_665 "MMISMS:GetNumber len is %d,msg_type is %d,mo_mt_type is %d"
#define MMISMSAPP_ORDER_7100_112_2_18_2_54_7_666 "MMISMS:MMISMS_CurMsgFolderType order_id = PNULL"
#define MMISMSAPP_ORDER_7304_112_2_18_2_54_8_667 "MMISMS_GetSmsBoxHeaderOrderId move_sms_num=%d"
#define MMISMSAPP_ORDER_7354_112_2_18_2_54_8_668 "MMISMS_SetCurMsgFolderType alloc failed "
#define MMISMSAPP_ORDER_7371_112_2_18_2_54_8_669 "MMISMS_SetCurMsgFolderType new_order_info_ptr = 0x%x,record_id is %d"
#define MMISMSAPP_ORDER_7404_112_2_18_2_54_8_670 "MMISMS_SetCurMsgFolderType folder_type = %d,record_id = %d"
#define MMISMSAPP_ORDER_7468_112_2_18_2_54_8_671 "MMISMS:MMISMS_CompareSearchSmsState sms1_state_ptr sms2_state_ptr = PNULL"
#define MMISMSAPP_ORDER_8283_112_2_18_2_54_10_672 "MMISMS_GetMarkedStorageMsgNumByDualsys:box_type = %d,storage = %d,dual_sys = %d"
#define MMISMSAPP_ORDER_8480_112_2_18_2_54_10_673 "MMISMS_GetMarkedStorageMsgNumByDualsys:mark_num = %d"
#define MMISMSAPP_ORDER_8497_112_2_18_2_54_10_674 "MMISMS_GetMarkedStorageMsgNum:box_type = %d,storage = %d"
#define MMISMSAPP_ORDER_8654_112_2_18_2_54_11_675 "MMISMS_GetMarkedStorageMsgNum:mark_num = %d"
#define MMISMSAPP_ORDER_8671_112_2_18_2_54_11_676 "MMISMS_GetMarkedMsgNumByType:box_type = %d,msg_type = %d"
#define MMISMSAPP_ORDER_8788_112_2_18_2_54_11_677 "MMISMS_GetMarkedMsgNumByType:mark_num = %d"
#define MMISMSAPP_ORDER_9104_112_2_18_2_54_12_678 "MMISMS_SetCurChatMsgMarked: index = %d"
#define MMISMSAPP_ORDER_9146_112_2_18_2_54_12_679 "MMISMS_SetCurChatMsgMarked: index = %d"
#define MMISMSAPP_ORDER_9215_112_2_18_2_54_12_680 "MMISMS_SetCurChatMsgMarked: index = %d"
#define MMISMSAPP_ORDER_9262_112_2_18_2_54_12_681 "MMISMS_SetCurChatMsgMarked: index = %d"
#define MMISMSAPP_ORDER_9304_112_2_18_2_54_12_682 "MMISMS_SetCurChatMsgMarked: index = %d"
#define MMISMSAPP_ORDER_9375_112_2_18_2_54_12_683 "InsertNodeToChatOrder: new_state_ptr is PNULL!"
#define MMISMSAPP_ORDER_9386_112_2_18_2_54_12_684 "InsertNodeToChatOrder: mo_mt_type is %d,folder_type is %d,msg_type is %d"
#define MMISMSAPP_ORDER_9666_112_2_18_2_54_13_685 "InsertSameNumNodeToChatOrderByTime: long sms not find in order!!"
#define MMISMSAPP_ORDER_9748_112_2_18_2_54_13_686 "InsertSameNumNodeToChatOrderByTime: long sms not find in order!!"
#define MMISMSAPP_ORDER_9888_112_2_18_2_54_14_687 "DeleteNodeFromChatOrder: delete_state_ptr is PNULL!"
#define MMISMSAPP_ORDER_9898_112_2_18_2_54_14_688 "DeleteNodeFromChatOrder: mo_mt_type is %d,folder_type is %d,msg_type is %d"
#define MMISMSAPP_ORDER_9976_112_2_18_2_54_14_689 "[jixin]:same num"
#define MMISMSAPP_ORDER_9982_112_2_18_2_54_14_690 "[jixin]:diff num"
#define MMISMSAPP_ORDER_10012_112_2_18_2_54_14_691 "DeleteChatNode:input para is PNULL!!"
#define MMISMSAPP_ORDER_10347_112_2_18_2_54_15_692 "MMISMS: DeleteChatNode :can't find the sms ptr in long sms link"
#define MMISMSAPP_ORDER_10360_112_2_18_2_54_15_693 "MMISMS DeleteChatNode: there is no sms have the same ref with the to delete sms1_state_ptr"
#define MMISMSAPP_ORDER_10365_112_2_18_2_54_15_694 "MMISMS: DeleteChatNode total_num = %d"
#define MMISMSAPP_ORDER_10657_112_2_18_2_54_15_695 "GetChatNodeTotalUnreadNum:unread_num = %d"
#define MMISMSAPP_ORDER_10669_112_2_18_2_54_15_696 "MMISMS_GetChatNodeTotalUnreadNum:unread_num = %d"
#define MMISMSAPP_ORDER_10686_112_2_18_2_54_15_697 "MMISMS_SetCurChatMsgMarked: index = %d,is_marked = %d"
#define MMISMSAPP_ORDER_10731_112_2_18_2_54_15_698 "MMISMS_SetCurChatMsgMarked: index = %d,is_marked = %d"
#define MMISMSAPP_ORDER_10847_112_2_18_2_54_16_699 "GetChatOrderLinkDeleteAvailNum: delete_sms_num = %d"
#define MMISMSAPP_ORDER_10876_112_2_18_2_54_16_700 "MMISMS_CurChatMsgUnlockedNum: delete_num = %d"
#define MMISMSAPP_ORDER_10894_112_2_18_2_54_16_701 "head_order_id is 0x%x"
#define MMISMSAPP_ORDER_10935_112_2_18_2_54_16_702 "head_order_id is 0x%x"
#define MMISMSAPP_ORDER_11038_112_2_18_2_54_16_703 "head_order_id is 0x%x"
#define MMISMSAPP_ORDER_11194_112_2_18_2_54_17_704 "mmisms : return_order_id is PNULL"
#define MMISMSAPP_ORDER_11261_112_2_18_2_54_17_705 "MMISMS: MMISMS_GetUpdateSMSNum update_sms_num=%d"
#define MMISMSAPP_ORDER_11279_112_2_18_2_54_17_706 "head_order_id is 0x%x"
#define MMISMSAPP_ORDER_11368_112_2_18_2_54_17_707 "HandleOkMsgInSmsList index is %d"
#define MMISMSAPP_ORDER_11464_112_2_18_2_54_17_708 "MMISMS: sms_record_id=%d, mo_mt_type=%d"
#define MMISMSAPP_ORDER_11499_112_2_18_2_54_17_709 "head_order_id is 0x%x"
#define MMISMSAPP_ORDER_11518_112_2_18_2_54_17_710 "MMISMS:cur_order_id is 0x%x"
#define MMISMSAPP_ORDER_11901_112_2_18_2_54_18_711 "MMIAPISMS_SetMatchItemContent name_str.wstr_ptr alloc fail"
#define MMISMSAPP_ORDER_11940_112_2_18_2_54_18_712 "MMIAPISMS_SetMatchItemContent2 name_str.wstr_ptr alloc fail"
#define MMISMSAPP_ORDER_11989_112_2_18_2_54_18_713 "MMISMS: MMISMS_ReplaceSms pid_e=%d"
#define MMISMSAPP_ORDER_12116_112_2_18_2_54_19_714 "MMISMS: MMISMS_ReplaceSms is_replace=%d"
#define MMISMSAPP_ORDER_12138_112_2_18_2_54_19_715 "MMISMS: MMISMS_FullOrder Order Not Ready!"
#define MMISMSAPP_ORDER_12147_112_2_18_2_54_19_716 "MMISMS: MMISMS_FullOrder No Memory!"
#define MMISMSAPP_ORDER_12206_112_2_18_2_54_19_717 "MMISMS:MMISMS_FindMsgForSmsBackup index = %d"
#define MMISMSAPP_ORDER_12248_112_2_18_2_54_19_718 "MMISMS: MMISMS_IsExistSameSms 0 == backup_data_ptr->time"
#define MMISMSAPP_ORDER_12265_112_2_18_2_54_19_719 "MMISMS: MMISMS_IsExistSameSms is_exist = %d, backup_data_ptr->time = %d, backup_data_ptr->record_id = %d"
#define MMISMSAPP_ORDER_12439_112_2_18_2_54_19_720 "MMISMS: MMISMS_LoadSmsListFromOrder the count of list box is %d"
#define MMISMSAPP_WINTAB_4082_112_2_18_2_54_28_721 "MMISMS:SetSmsMemState2String item_data index = PNULL"
#define MMISMSAPP_WINTAB_4194_112_2_18_2_54_28_722 "MMISMS:SetSmsMemState2String item_data index = PNULL"
#define MMISMSAPP_WINTAB_4354_112_2_18_2_54_29_723 "MMISMS: MMISMS HandleSmsNewMsgMenuWinMsg: menu_id = %d"
#define MMISMSAPP_WINTAB_4445_112_2_18_2_54_29_724 "SetNumState2String:chat total_num = %d,unread_num = %d"
#define MMISMSAPP_WINTAB_4465_112_2_18_2_54_29_725 "SetNumState2String:chat total_num = %d,unread_num = %d"
#define MMISMSAPP_WINTAB_4475_112_2_18_2_54_29_726 "SetNumState2String:chat total_num = %d,unread_num = %d"
#define MMISMSAPP_WINTAB_4485_112_2_18_2_54_29_727 "SetNumState2String:chat total_num = %d,unread_num = %d"
#define MMISMSAPP_WINTAB_4495_112_2_18_2_54_29_728 "SetNumState2String:chat total_num = %d,unread_num = %d"
#define MMISMSAPP_WINTAB_4848_112_2_18_2_54_30_729 "HandleSmsMainMenuOptWinMsg menu_id = %d"
#define MMISMSAPP_WINTAB_5005_112_2_18_2_54_30_730 "MMISMS:OpenVMNumberWin dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_WINTAB_5014_112_2_18_2_54_30_731 "OpenVMNumberWin: MNSMS_GetVoiMessAddr()"
#define MMISMSAPP_WINTAB_5036_112_2_18_2_54_30_732 "MMISMS:OpenVMNumberWin dual_sys >=  MMI_DUAL_SYS_MAX"
#define MMISMSAPP_WINTAB_5060_112_2_18_2_54_30_733 "OpenVMNumberWin: MNSMS_GetVoiMessAddr()"
#define MMISMSAPP_WINTAB_5478_112_2_18_2_54_31_734 "MMISMS: HandleEditMenuWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_5747_112_2_18_2_54_31_735 "MMISMS: HandleEditMenuWinMsg call MMIAPIPB_OpenNumberList"
#define MMISMSAPP_WINTAB_6052_112_2_18_2_54_32_736 "MMISMS:IsSCAddrValid num_str_ptr = PNULL"
#define MMISMSAPP_WINTAB_6096_112_2_18_2_54_32_737 "MMISMS: HandleInputSignatureWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_6150_112_2_18_2_54_32_738 "MMISMS: HandleInputSCWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_6266_112_2_18_2_54_33_739 "MMISMS_HandleSendWaitWinMsg: Send cancel! dual_sys = %d"
#define MMISMSAPP_WINTAB_6278_112_2_18_2_54_33_740 "MMISMS:MMISMS_HandleSendWaitWinMsg send_info_ptr = PNULL"
#define MMISMSAPP_WINTAB_6361_112_2_18_2_54_33_741 "MMISMS:MMISMS_HandleSendWaitWinMsg send_info_ptr = PNULL"
#define MMISMSAPP_WINTAB_6364_112_2_18_2_54_33_742 "MMISMS_HandleSendWaitWinMsg:Stop g_sms_retry_timer  %d"
#define MMISMSAPP_WINTAB_6455_112_2_18_2_54_33_743 "MMISMS_HandleWaitWinMsg:insert unhandle sms"
#define MMISMSAPP_WINTAB_6858_112_2_18_2_54_34_744 "MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_6925_112_2_18_2_54_34_745 "MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
#define MMISMSAPP_WINTAB_7377_112_2_18_2_54_35_746 "MMISMS: HandleSendSuccBoxChildWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_7421_112_2_18_2_54_35_747 "MMISMS:MSG_CTL_LIST_NEED_ITEM_DATAa need_item_data_ptr = PNULL"
#define MMISMSAPP_WINTAB_7757_112_2_18_2_54_36_748 "MMISMS: HandleSendFailBoxChildWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_7802_112_2_18_2_54_36_749 "MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
#define MMISMSAPP_WINTAB_8132_112_2_18_2_54_37_750 "MMISMS: HandleDraftBoxChildWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_8174_112_2_18_2_54_37_751 "MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
#define MMISMSAPP_WINTAB_8504_112_2_18_2_54_38_752 "MMISMS: MMISMS_BoxEnableGrayed s_marked_sms_num = %d, sim_ok_num=%d"
#define MMISMSAPP_WINTAB_8569_112_2_18_2_54_38_753 "MMISMS: MMISMS_BoxEnableGrayed s_marked_sms_num = %d"
#define MMISMSAPP_WINTAB_9592_112_2_18_2_54_40_754 "HandleOkMsgInSmsList index is %d"
#define MMISMSAPP_WINTAB_9634_112_2_18_2_54_41_755 "MMISMS:HandleOkMsgInSmsList order_id = PNULL"
#define MMISMSAPP_WINTAB_9682_112_2_18_2_54_41_756 "MMISMS:HandleOkMsgInSmsList order_id = PNULL"
#define MMISMSAPP_WINTAB_9711_112_2_18_2_54_41_757 "MMISMS:HandleOkMsgInSmsList order_id = PNULL"
#define MMISMSAPP_WINTAB_10967_112_2_18_2_54_43_758 "MMISMS: HandleShowSMSWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_11332_112_2_18_2_54_44_759 "sms pen ok has handled"
#define MMISMSAPP_WINTAB_11356_112_2_18_2_54_44_760 "MMISMS:HandleShowSMSWinMsg focus_num_ptr = PNULL"
#define MMISMSAPP_WINTAB_11374_112_2_18_2_54_44_761 "MMISMS:HandleShowSMSWinMsg focus_url_ptr = PNULL"
#define MMISMSAPP_WINTAB_11386_112_2_18_2_54_44_762 "MMISMS:HandleShowSMSWinMsg focus_email_ptr = PNULL"
#define MMISMSAPP_WINTAB_11828_112_2_18_2_54_45_763 "MMISMS: HandleShowSMSWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_12146_112_2_18_2_54_46_764 "sms pen ok has handled"
#define MMISMSAPP_WINTAB_12170_112_2_18_2_54_46_765 "MMISMS:HandleShowSMSWinMsg focus_num_ptr = PNULL"
#define MMISMSAPP_WINTAB_12188_112_2_18_2_54_46_766 "MMISMS:HandleShowSMSWinMsg focus_url_ptr = PNULL"
#define MMISMSAPP_WINTAB_12200_112_2_18_2_54_46_767 "MMISMS:HandleShowSMSWinMsg focus_email_ptr = PNULL"
#define MMISMSAPP_WINTAB_12603_112_2_18_2_54_47_768 "MMIMMS: HandleSMSExtractNumberPopWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_12844_112_2_18_2_54_47_769 "MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_13006_112_2_18_2_54_48_770 "MMIMMS: HandleSMSExtractEmailWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_13196_112_2_18_2_54_48_771 "SMS: HandleSmsMtboxMenuOptWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_13701_112_2_18_2_54_49_772 "MMISMS:Mark num all , num is %d"
#define MMISMSAPP_WINTAB_13719_112_2_18_2_54_49_773 "MMISMS:Mark num all , num is %d"
#define MMISMSAPP_WINTAB_14911_112_2_18_2_54_52_774 "MMISMS:Mark num all , num is %d"
#define MMISMSAPP_WINTAB_15277_112_2_18_2_54_52_775 " the menu id is error"
#define MMISMSAPP_WINTAB_15359_112_2_18_2_54_52_776 " the menu id is error"
#define MMISMSAPP_WINTAB_15447_112_2_18_2_54_53_777 " the menu id is error"
#define MMISMSAPP_WINTAB_15566_112_2_18_2_54_53_778 " the menu id is error"
#define MMISMSAPP_WINTAB_15594_112_2_18_2_54_53_779 " the menu id is error"
#define MMISMSAPP_WINTAB_15659_112_2_18_2_54_53_780 " the menu id is error"
#define MMISMSAPP_WINTAB_15746_112_2_18_2_54_53_781 " the menu id is error"
#define MMISMSAPP_WINTAB_15829_112_2_18_2_54_53_782 " the menu id is error"
#define MMISMSAPP_WINTAB_15849_112_2_18_2_54_54_783 "MMISMS:MMISMS_OpenCallVMWin dual_sys >= MMI_DUAL_SYS_MAX || vmn_type >= MMISMS_VMN_TYPE_MAX"
#define MMISMSAPP_WINTAB_16276_112_2_18_2_54_54_784 "HandleEntryDetailPdaWinMsg list_item_ptr %d"
#define MMISMSAPP_WINTAB_16552_112_2_18_2_54_55_785 "MMISMS:HandleWordListForCcWinMsg cur_list_ptr = PNULL"
#define MMISMSAPP_WINTAB_16595_112_2_18_2_54_55_786 "MMISMS:HandleWordListForCcWinMsg cur_list_ptr = PNULL"
#define MMISMSAPP_WINTAB_16706_112_2_18_2_54_55_787 "MMISMS:HandleSetPresetSms text_ptr = PNULL"
#define MMISMSAPP_WINTAB_16749_112_2_18_2_54_55_788 "MMISMS:HandleSetPresetSms text_ptr = PNULL"
#define MMISMSAPP_WINTAB_17251_112_2_18_2_54_56_789 "MMISMS:AppendListItem string_ptr = PNULL"
#define MMISMSAPP_WINTAB_17320_112_2_18_2_54_56_790 "HandleSendNumOptWinMsg, send_type = %d, msg_id = %x"
#define MMISMSAPP_WINTAB_17616_112_2_18_2_54_57_791 "MMISMS: GetNumberFromPb msg_id = 0x%x, count = %d"
#define MMISMSAPP_WINTAB_17670_112_2_18_2_54_57_792 "MMISMS: MMISMS_GetNumberFromCalllog msg_id = 0x%x, count = %d"
#define MMISMSAPP_WINTAB_17818_112_2_18_2_54_57_793 "MMISMS_HandleSendOperation: win_id = 0x%x,is_re_send = %d"
#define MMISMSAPP_WINTAB_17837_112_2_18_2_54_58_794 "MMISMS_HandleSendOperation: timer sms"
#define MMISMSAPP_WINTAB_17940_112_2_18_2_54_58_795 "MMISMS: HandleSendPromptOptMenuWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_18062_112_2_18_2_54_58_796 "MMISMS:HandleSelectWordWinMsg cur_list_ptr = PNULL"
#define MMISMSAPP_WINTAB_18131_112_2_18_2_54_58_797 "MMISMS:MMISMS_GetListCurrentItemContent str_len_ptr wstr_ptr item_ptr = PNULL"
#define MMISMSAPP_WINTAB_18431_112_2_18_2_54_59_798 "MMISMS: HandleSendSuccMenuWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_19422_112_2_18_2_55_1_799 "MMISMS_HandleNewMsgWin %d"
#define MMISMSAPP_WINTAB_19519_112_2_18_2_55_1_800 "mmisms: order_id is PNULL"
#define MMISMSAPP_WINTAB_19673_112_2_18_2_55_1_801 "mmisms: order_id is PNULL"
#define MMISMSAPP_WINTAB_19769_112_2_18_2_55_1_802 "MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected"
#define MMISMSAPP_WINTAB_19809_112_2_18_2_55_1_803 "SMS: PlayNewSMSRing vt mode"
#define MMISMSAPP_WINTAB_19817_112_2_18_2_55_1_804 "MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected"
#define MMISMSAPP_WINTAB_19994_112_2_18_2_55_2_805 "MMISMS: HandleMsgBoxWinMsg msg_id = %d"
#define MMISMSAPP_WINTAB_20070_112_2_18_2_55_2_806 "RingCallBack ENTER RingCallBack"
#define MMISMSAPP_WINTAB_20102_112_2_18_2_55_2_807 "PlaySMSRing ring_type = %d"
#define MMISMSAPP_WINTAB_20136_112_2_18_2_55_2_808 "mary MMISET_MSG_VIBRA_AND_RING"
#define MMISMSAPP_WINTAB_20172_112_2_18_2_55_2_809 "sim sms num error max_num = %d,used_num = %d "
#define MMISMSAPP_WINTAB_20183_112_2_18_2_55_2_810 "nv sms num error max_num = %d,used_num = %d "
#define MMISMSAPP_WINTAB_20248_112_2_18_2_55_2_811 "MMISMS: MMISMS_GetSaveSmsNumFlag g_save_sms_num_flag = %d"
#define MMISMSAPP_WINTAB_20660_112_2_18_2_55_3_812 "HandleSettingSavePosWinMsg msg_id is 0x%x"
#define MMISMSAPP_WINTAB_20671_112_2_18_2_55_3_813 "HandleSettingSavePosWinMsg pos is %d"
#define MMISMSAPP_WINTAB_20994_112_2_18_2_55_4_814 "MMISMS_ShowMemFullPrompt:dual_sys=%d,mem_status=%d"
#define MMISMSAPP_WINTAB_22152_112_2_18_2_55_6_815 "SetNumState2String:chat total_num = %d,unread_num = %d"
#define MMISMSAPP_WINTAB_22365_112_2_18_2_55_7_816 "MMISMS:GetExtractURLOrST gbstr = PNULL"
#define MMISMSAPP_WINTAB_22370_112_2_18_2_55_7_817 "MMISMS:GetExtractURLOrST index >= s_extract.total_num"
#define MMISMSAPP_WINTAB_22429_112_2_18_2_55_7_818 "index = %d, gbstr_len = %d"
#define MMISMSAPP_WINTAB_22432_112_2_18_2_55_7_819 "extract st addr is %x,%x,%x,%x,%x,%x,%x,%x"
#define MMISMSAPP_WINTAB_22519_112_2_18_2_55_7_820 "MMISMS_HandleNewVcardMsgWin name_len=%d"
#define MMISMSAPP_WINTAB_22651_112_2_18_2_55_7_821 "MMISMS_HandleNewVcalendarMsgWin name_len=%d"
#define MMISMSAPP_WINTAB_22888_112_2_18_2_55_8_822 " sms num error max_num = %d,used_num = %d "
#define MMISMSAPP_WINTAB_22917_112_2_18_2_55_8_823 "MMISMS StartMenuWinTimer: the key timer has started!"
#define MMISMSAPP_WINTAB_22934_112_2_18_2_55_8_824 "MMISMS StopMenuWinTimer: the key timer has stop!"
#define MMISMSAPP_WINTAB_23040_112_2_18_2_55_8_825 "MMISMS_IsInSMSWin :win_id is %d"
#define MMISMSAPP_WINTAB_23072_112_2_18_2_55_8_826 "mmisms_getpushsmsbrowserinfo: open_flag is %d"
#define MMISMSAPP_WINTAB_23090_112_2_18_2_55_8_827 "mmisms_setpushsmsbrowserinfo: open_flag is %d,flag is %d"
#define MMISMSAPP_WINTAB_23172_112_2_18_2_55_8_828 "HandleDelQueryWinMsg: cancel del same num"
#define MMISMSAPP_WINTAB_23431_112_2_18_2_55_9_829 "SMS: HandleSMSClass0OptionPopWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_23660_112_2_18_2_55_9_830 "MMISMS: MMISMS_UpdateMMSMarkedFlag s_marked_sms_num = %d"
#define MMISMSAPP_WINTAB_23687_112_2_18_2_55_9_831 "MMISMS: HandleSecurityBoxChildWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_23731_112_2_18_2_55_9_832 "MMISMS:HandleSecurityBoxChildWinMsg need_item_data_ptr = PNULL"
#define MMISMSAPP_WINTAB_24333_112_2_18_2_55_11_833 "move cancel"
#define MMISMSAPP_WINTAB_24813_112_2_18_2_55_12_834 "MMISMS:IsCorrectSecurityboxPwd pwd_ptr = PNULL"
#define MMISMSAPP_WINTAB_24868_112_2_18_2_55_12_835 "MMISMS_GetMoveSingleFlag:s_is_move_single = %d"
#define MMISMSAPP_WINTAB_24883_112_2_18_2_55_12_836 "MMISMS_SetMoveSingleFlag:s_is_move_single = %d"
#define MMISMSAPP_WINTAB_24950_112_2_18_2_55_12_837 "MMISMS_SetExportBoxType: export_box_type is %d"
#define MMISMSAPP_WINTAB_24960_112_2_18_2_55_12_838 "MMISMS_GetExportBoxType: s_export_type is %d"
#define MMISMSAPP_WINTAB_25115_112_2_18_2_55_12_839 "MMISMS_SetMarkedMsgNum: marked_num is %d"
#define MMISMSAPP_WINTAB_25126_112_2_18_2_55_12_840 "MMISMS_GetMarkedMsgNum: marked_num is %d"
#define MMISMSAPP_WINTAB_25180_112_2_18_2_55_12_841 "[MMISMS ATC] MMIAPISMS_InsertMessage result %d"
#define MMISMSAPP_WINTAB_25217_112_2_18_2_55_12_842 "MMISMS_GetWinFont: font is %d"
#define MMISMSAPP_WINTAB_26001_112_2_18_2_55_14_843 "HandleEditSmsWinMsg alloc mem fail"
#define MMISMSAPP_WINTAB_26834_112_2_18_2_55_16_844 "SetIsSelectPB: s_is_add_contact is %d"
#define MMISMSAPP_WINTAB_26844_112_2_18_2_55_16_845 "GetIsAddContact: s_is_add_contact is %d"
#define MMISMSAPP_WINTAB_27455_112_2_18_2_55_17_846 "mmisms date_time_str PNULL"
#define MMISMSAPP_WINTAB_27500_112_2_18_2_55_17_847 "MMISMS: HandleChatBoxChildWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_27560_112_2_18_2_55_17_848 "MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
#define MMISMSAPP_WINTAB_28309_112_2_18_2_55_19_849 "MMISMS: HandleShowChatSmsWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_28546_112_2_18_2_55_19_850 "mmisms alloc failed"
#define MMISMSAPP_WINTAB_28604_112_2_18_2_55_19_851 "sms file info alloc error"
#define MMISMSAPP_WINTAB_28671_112_2_18_2_55_19_852 "mmisms alloc failed"
#define MMISMSAPP_WINTAB_28790_112_2_18_2_55_20_853 "sms file info alloc error"
#define MMISMSAPP_WINTAB_28813_112_2_18_2_55_20_854 "mmisms alloc failed"
#define MMISMSAPP_WINTAB_28872_112_2_18_2_55_20_855 "MMISMS:ShowChatSmsWin focus_num_ptr = PNULL"
#define MMISMSAPP_WINTAB_29154_112_2_18_2_55_20_856 "SetMMSChatItemParam:PNULL == order_id"
#define MMISMSAPP_WINTAB_29159_112_2_18_2_55_20_857 "SetMMSChatItemParam:MMISMS_MO_DRAFT"
#define MMISMSAPP_WINTAB_29167_112_2_18_2_55_20_858 "SetMMSChatItemParam:not find index!!"
#define MMISMSAPP_WINTAB_29183_112_2_18_2_55_20_859 "SetMMSChatItemParam: else"
#define MMISMSAPP_WINTAB_29530_112_2_18_2_55_21_860 "SetChatItemParam:not find index!!"
#define MMISMSAPP_WINTAB_30054_112_2_18_2_55_22_861 "SetChatWinIndex: index is %d,is_active is %d"
#define MMISMSAPP_WINTAB_30107_112_2_18_2_55_22_862 "GetChatWinIndex: index is %d,result is %d"
#define MMISMSAPP_WINTAB_30175_112_2_18_2_55_22_863 "SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_30210_112_2_18_2_55_22_864 "MMISMS:  HandleSmsChatMenuOptWinMsg = %d"
#define MMISMSAPP_WINTAB_30331_112_2_18_2_55_23_865 "OperateAfterDelSingleChatItem:g_mmisms_global.del_index is %d"
#define MMISMSAPP_WINTAB_30347_112_2_18_2_55_23_866 "mmisms chat not focus in chat item"
#define MMISMSAPP_WINTAB_30381_112_2_18_2_55_23_867 "SMS: HandleSmsChatItemMenuOptWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_30436_112_2_18_2_55_23_868 "MMISMS:  HandleSmsChatItemMenuOptWinMsg = %d"
#define MMISMSAPP_WINTAB_30692_112_2_18_2_55_23_869 "mmisms alloc failed"
#define MMISMSAPP_WINTAB_30815_112_2_18_2_55_24_870 "MMISMS_HandleWaitWinMsg:insert unhandle sms"
#define MMISMSAPP_WINTAB_30825_112_2_18_2_55_24_871 "MMISMS_HandleSendWaitWinMsg: Send cancel! dual_sys = %d"
#define MMISMSAPP_WINTAB_31121_112_2_18_2_55_24_872 "HandleEntryDetailPdaWinMsg list_item_ptr %d"
#define MMISMSAPP_WINTAB_31267_112_2_18_2_55_25_873 "SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_31331_112_2_18_2_55_25_874 "SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_31396_112_2_18_2_55_25_875 "SMS: HandleChatMarkMenuWindow, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_31442_112_2_18_2_55_25_876 "SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_31544_112_2_18_2_55_25_877 "SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_31617_112_2_18_2_55_25_878 "SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x"
#define MMISMSAPP_WINTAB_31826_112_2_18_2_55_26_879 "sms order type = %d"
#define MMISMSAPP_WINTAB_31958_112_2_18_2_55_26_880 "MMIAPISMS_OpenValidateSecruityBox. user_callback = PNULL"
#define MMISMSAPP_WINTAB_31983_112_2_18_2_55_26_881 "MMIAPISMS_OpenValidateSecruityBox. user_callback = PNULL"
#define MMISMSAPP_WINTAB_32273_112_2_18_2_55_27_882 "MMISMS:HandleOkMsgInSmsList order_id = PNULL"
#define MMISMSAPP_WINTAB_32302_112_2_18_2_55_27_883 "MMISMS:HandleOkMsgInSmsList order_id = PNULL"
#define MMISMSAPP_WINTAB_32450_112_2_18_2_55_27_884 "MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x"
#define MMISMSAPP_WINTAB_32497_112_2_18_2_55_27_885 "MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
#define MMISMSCB_WINTAB_1283_112_2_18_2_55_31_886 "SMSCB: ConvertCBMsgContextToMsgItem msg_len = %d, alphabet_type = %d"
#define MMISMSCB_WINTAB_1372_112_2_18_2_55_31_887 "MMISMS:DisplayCBMsgContext cb_msg_item_ptr = PNULL"
#define MMISMSCB_WINTAB_1520_112_2_18_2_55_31_888 "MMISMSCB_Init result[%d] = %d"
#define MMISMSCB_WINTAB_2140_112_2_18_2_55_33_889 "MMISMS:AddNewMsgToCBMsgList cb_msg_ptr = PNULL"
#define MMISMSCB_WINTAB_2143_112_2_18_2_55_33_890 "SMSCB: AddNewMsgToCBMsgList"
#define MMISMSCB_WINTAB_2148_112_2_18_2_55_33_891 "SMSCB: cur_msg_ptr alloc failed"
#define MMISMSCB_WINTAB_2220_112_2_18_2_55_33_892 "MMISMS:AddNewMarcoMsgToCBMsgList cb_msg_ptr = PNULL"
#define MMISMSCB_WINTAB_2223_112_2_18_2_55_33_893 "SMSCB: AddNewMarcoMsgToCBMsgList"
#define MMISMSCB_WINTAB_2228_112_2_18_2_55_33_894 "SMSCB: cur_msg_ptr alloc failed"
#define MMISMSCB_WINTAB_2297_112_2_18_2_55_33_895 "SMSCB: DeleteMsgFromCBMsgList num = d%, index = d%"
#define MMISMSCB_WINTAB_2337_112_2_18_2_55_33_896 "SMSCB: DeleteMsgFromCBMsgList num1 = d%"
#define MMISMSCB_WINTAB_2361_112_2_18_2_55_33_897 "MMISMS:DirectDisplayNewCBMsg cb_msg_ptr = PNULL"
#define MMISMSCB_WINTAB_2427_112_2_18_2_55_33_898 "MMISMS:DirectDisplayNewMarcoCBMsg cb_msg_ptr = PNULL"
#define MMISMSCB_WINTAB_2492_112_2_18_2_55_33_899 "MMISMS:MMIAPISMSCB_HandleCBMsgInd cb_msg_ptr = PNULL"
#define MMISMSCB_WINTAB_2495_112_2_18_2_55_33_900 "SMSCB: MMIAPISMSCB_HandleCBMsgInd dual_sys = %d"
#define MMISMSCB_WINTAB_2557_112_2_18_2_55_33_901 "MMISMS:MMIAPISMSCB_HandleMarcoCBMsgInd cb_msg_ptr = PNULL"
#define MMISMSCB_WINTAB_2560_112_2_18_2_55_33_902 "SMSCB: MMISMSCB_HandleMacroCBMsgInd dual_sys = %d"
#define MMISMSCB_WINTAB_2925_112_2_18_2_55_34_903 "MMISMS:AppendSmsCBLanguageListItem lang_list = PNULL"
#define MMISMSCB_WINTAB_3168_112_2_18_2_55_35_904 "MMISMS:IsAllLangSelected lang_list = PNULL"
#define MMISMSCB_WINTAB_3764_112_2_18_2_55_36_905 "MMISMS:WstrToLongNum num = PNULL"
#define MMISMSCB_WINTAB_4273_112_2_18_2_55_37_906 "SMSCB: HandleSmsCBReadWindow index = %d"
#define MMISMSCB_WINTAB_4530_112_2_18_2_55_37_907 "SMSCB: HandleSmsCBMainMenuWindow menu_id = %d"
#define MMISMSCB_WINTAB_4888_112_2_18_2_55_38_908 "SMSCB: HandleSmsCBMainMenuWindow menu_id = %d"
#define MMISMSCB_WINTAB_5214_112_2_18_2_55_39_909 "SMSCB: HandleSmsCBMainMenuWindow menu_id = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_SMS_TRC)
TRACE_MSG(MMISMS_API_376_112_2_18_2_52_27_0,"MMISMS: MMIAPISMS_OpenFreqWordListForCc len = %d, addr = %p")
TRACE_MSG(MMISMS_API_483_112_2_18_2_52_27_1,"MMISMS: MMIAPISMS_DeleteMsgInorder : msg_type = %d,record_id = %d")
TRACE_MSG(MMISMS_API_524_112_2_18_2_52_27_2,"MMISMS: MMIAPISMS_InsertOneMsgToOrder order_ptr==PNULL!")
TRACE_MSG(MMISMS_API_528_112_2_18_2_52_27_3,"MMISMS: MMIAPISMS_InsertOneMsgToOrder order->mo_mt_type = %d, mms_record_id = %d, dual_sys = %d, msg_type = %d")
TRACE_MSG(MMISMS_API_546_112_2_18_2_52_27_4,"state_info.display_content.content_len = %d")
TRACE_MSG(MMISMS_API_613_112_2_18_2_52_27_5,"MMISMS: MMIAPISMS_InsertOneMsgToOrder order_ptr==PNULL!")
TRACE_MSG(MMISMS_API_617_112_2_18_2_52_27_6,"MMISMS: MMIAPISMS_UpdateMsgStatusInOrder enter: record_id = %d, dual_sys = %d ")
TRACE_MSG(MMISMS_API_697_112_2_18_2_52_27_7,"MMISMS: MMIAPISMS_UpdateMsgStatusInOrder i = %d")
TRACE_MSG(MMISMS_API_725_112_2_18_2_52_27_8,"MMISMS: MMIAPISMS_DeleteAllMMSDone enter!")
TRACE_MSG(MMISMS_API_747_112_2_18_2_52_28_9,"MMISMS: MMIAPISMS_MMSOrderDone enter!")
TRACE_MSG(MMISMS_API_768_112_2_18_2_52_28_10,"MMISMS:MMIAPISMS_MMSInitDone")
TRACE_MSG(MMISMS_API_1193_112_2_18_2_52_29_11,"MMISMS: MMIAPISMS_DeleteAllMailMsg")
TRACE_MSG(MMISMS_API_1344_112_2_18_2_52_29_12,"MMIWIDGET_NewSms_GetOriginNumByPos PNULL == num_ptr || PNULL == dual_sys_ptr")
TRACE_MSG(MMISMS_API_1401_112_2_18_2_52_29_13,"MMIWIDGET_NewSms_GetOriginTimeAndDate PNULL == date_ptr || PNULL == time_ptr")
TRACE_MSG(MMISMS_DELALLWIN_387_112_2_18_2_52_31_14,"MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMS_DELALLWIN_407_112_2_18_2_52_31_15,"MMISMS:HandleBoxDelChildWinMsg need_item_data_ptr = PNULL")
TRACE_MSG(MMISMS_DELALLWIN_538_112_2_18_2_52_31_16,"MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMS_DELALLWIN_558_112_2_18_2_52_32_17,"MMISMS:HandleBoxDelChildWinMsg need_item_data_ptr = PNULL")
TRACE_MSG(MMISMS_DELALLWIN_917_112_2_18_2_52_33_18,"SetMsgTypeListItemData msg_type = %d")
TRACE_MSG(MMISMS_DELALLWIN_968_112_2_18_2_52_33_19,"SetMsgTypeListItemData msg_type = %d")
TRACE_MSG(MMISMS_DELALLWIN_1047_112_2_18_2_52_33_20,"GetMsgTypeNumString msg_type = %d")
TRACE_MSG(MMISMS_DELETE_85_112_2_18_2_52_34_21,"MMISMS: HandleDeleteCnf cause = %d")
TRACE_MSG(MMISMS_DELETE_161_112_2_18_2_52_34_22,"MMISMS: DeleteReqToMN(%d, %d, %d)")
TRACE_MSG(MMISMS_DELETE_170_112_2_18_2_52_34_23,"MMISMS: DeleteReqToMN record_id = %d, storage = %d")
TRACE_MSG(MMISMS_DELETE_173_112_2_18_2_52_34_24,"MMISMS: DeleteReqToMN MNSMS_UpdateSmsStatusEx result = %d")
TRACE_MSG(MMISMS_DELETE_177_112_2_18_2_52_34_25,"MMISMS: DeleteReqToMN oper_status = %d")
TRACE_MSG(MMISMS_DELETE_181_112_2_18_2_52_34_26,"sms: DeleteReqToMN, g_mmisms_global.operation.cur_type = %d")
TRACE_MSG(MMISMS_DELETE_251_112_2_18_2_52_34_27,"MMISMS: MMISMS_HandleUpdateSMSCnf sig_ptr == PNULL")
TRACE_MSG(MMISMS_DELETE_255_112_2_18_2_52_34_28,"MMISMS: MMISMS_HandleUpdateSMSCnf record_id = %d, storage = %d,oper_status = %d")
TRACE_MSG(MMISMS_DELETE_334_112_2_18_2_52_35_29,"MMISMS: MMISMS_HandleUpdateSMSCnf get record_id = %d, storage = %d")
TRACE_MSG(MMISMS_DELETE_422_112_2_18_2_52_35_30,"MMISMS: MMISMS_HandleUpdateSMSCnf %d")
TRACE_MSG(MMISMS_DELETE_533_112_2_18_2_52_35_31,"MMISMS: MMISMS_HandleUpdateSMSCnf %d")
TRACE_MSG(MMISMS_DELETE_583_112_2_18_2_52_35_32,"MMISMS: MMISMS_HandleUpdateSMSCnf %d")
TRACE_MSG(MMISMS_DELETE_652_112_2_18_2_52_35_33,"MMISMS: MMISMS_HandleUpdateSMSCnf sig_ptr->record_id = %d, sig_ptr->storage = %d, oper_status = %d")
TRACE_MSG(MMISMS_DELETE_750_112_2_18_2_52_35_34,"MMISMS: MMISMS_AppDeleteMsgBox type = %d")
TRACE_MSG(MMISMS_DELETE_811_112_2_18_2_52_36_35,"MMISMS: MMISMS_DeleteInfoInSendListById")
TRACE_MSG(MMISMS_DELETE_884_112_2_18_2_52_36_36,"MMISMS: MMISMS_AppDeleteMsgBox: The SmsBox is empty!")
TRACE_MSG(MMISMS_DELETE_929_112_2_18_2_52_36_37,"MMISMS: MMISMS_DeletePointedSMS pos_index = %d, order_id = %d, type = %d")
TRACE_MSG(MMISMS_DELETE_948_112_2_18_2_52_36_38,"MMISMS: HandleMoveCnf cause = %d")
TRACE_MSG(MMISMS_DELETE_952_112_2_18_2_52_36_39,"MMISMS: HandleMoveCnf move_all_flag = %d")
TRACE_MSG(MMISMS_EDIT_73_112_2_18_2_52_36_40,"MMISMS: HandleEditMoSmsSuccess record_id = %d, storage = %d, status = %d, oper_status = %d")
TRACE_MSG(MMISMS_EDIT_168_112_2_18_2_52_37_41,"MMISMS: UpdateEditStatusToOrderList(%d, %d, %d, %d)")
TRACE_MSG(MMISMS_EDIT_272_112_2_18_2_52_37_42,"MMISMS: MMISMS_HandleEditSMSCnf sig_ptr = PNULL")
TRACE_MSG(MMISMS_EDIT_275_112_2_18_2_52_37_43,"MMISMS: MMISMS_HandleEditSMSCnf status = %d")
TRACE_MSG(MMISMS_EDIT_328_112_2_18_2_52_37_44,"MMISMS:MMISMS_InterceptMsgContent PNULL !=  send_info_ptr")
TRACE_MSG(MMISMS_EDIT_333_112_2_18_2_52_37_45,"MMISMS:MMISMS_InterceptMsgContent PNULL ==  send_info_ptr")
TRACE_MSG(MMISMS_EDIT_337_112_2_18_2_52_37_46,"MMISMS:MMISMS_InterceptMsgContent intercpet_len_ptr intercpet_content_ptr send_message = PNULL")
TRACE_MSG(MMISMS_EDIT_461_112_2_18_2_52_37_47,"MMISMS:MMISMS_InterceptClass0MsgContent intercpet_len_ptr intercpet_content_ptr = PNULL")
TRACE_MSG(MMISMS_EDIT_587_112_2_18_2_52_38_48,"MMISMSR8: MMISMS_SetMessageContent content_ptr")
TRACE_MSG(MMISMS_EDIT_601_112_2_18_2_52_38_49,"MMISMS: MMISMS_SetMessageContent No memory!")
TRACE_MSG(MMISMS_EDIT_654_112_2_18_2_52_38_50,"MMISMSR8: MMISMS_SetMessageContent language=%d")
TRACE_MSG(MMISMS_EDIT_697_112_2_18_2_52_38_51,"MMISMS: MMISMS_SetMessageContent length=%d,alphabet=%d,is_need_packet=%d")
TRACE_MSG(MMISMS_EDIT_699_112_2_18_2_52_38_52,"MMISMS: MMISMS_SetMessageContent length=%d,is_ucs2=%d,is_need_packet=%d")
TRACE_MSG(MMISMS_EDIT_830_112_2_18_2_52_38_53,"MMISMS: MMISMS_SetMessageContent: SCI_MEMCPY length = %d")
TRACE_MSG(MMISMS_EDIT_860_112_2_18_2_52_38_54,"MMISMS:MMISMS_InterceptClass0MsgContent intercpet_len_ptr intercpet_content_ptr = PNULL")
TRACE_MSG(MMISMS_FILE_1449_112_2_18_2_52_41_55,"MMISMS:MMISMS_GetDisplayPathText path_value_str_ptr path_name_ptr path_name_len_ptr = PNULL")
TRACE_MSG(MMISMS_FILE_1491_112_2_18_2_52_41_56,"MMISMS:MMISMS_GetCustomizedExportPath full_path_ptr full_path_len_ptr = PNULL")
TRACE_MSG(MMISMS_FILE_1498_112_2_18_2_52_41_57,"MMISMS_GetCustomizedExportPath len=%d")
TRACE_MSG(MMISMS_FILE_1639_112_2_18_2_52_42_58,"MMISMS:MMISMS_GetDefaultExportPath full_path_ptr full_path_len_ptr = PNULL")
TRACE_MSG(MMISMS_FILE_1709_112_2_18_2_52_42_59,"Export_OpenLabelCallBack is_success = %d")
TRACE_MSG(MMISMS_FILE_1715_112_2_18_2_52_42_60,"Export_OpenLabelCallBack, file_num = %d")
TRACE_MSG(MMISMS_FILE_1753_112_2_18_2_52_42_61,"Export_OpenLabelCallBack is_success = %d")
TRACE_MSG(MMISMS_FILE_1759_112_2_18_2_52_42_62,"Export_OpenLabelCallBack, file_num = %d")
TRACE_MSG(MMISMS_FILE_1875_112_2_18_2_52_42_63,"MMISMS: MMISMS_AppExportMsgBox type = %d")
TRACE_MSG(MMISMS_FILE_1936_112_2_18_2_52_43_64,"MMISMS_AppExportMsgBox: msg_type is %d")
TRACE_MSG(MMISMS_FILE_1958_112_2_18_2_52_43_65,"MMISMS: s_export_file_handle: The SmsBox is empty!")
TRACE_MSG(MMISMS_FILE_2069_112_2_18_2_52_43_66,"CreateExportMsgFile s_export_file_handle is 0x%x")
TRACE_MSG(MMISMS_FILE_2153_112_2_18_2_52_43_67,"MMISMS_ExportMsgToFile: error is %d")
TRACE_MSG(MMISMS_FILE_2157_112_2_18_2_52_43_68,"MMISMS_ExportMsgToFile: s_mmisms_export_current_num is %d")
TRACE_MSG(MMISMS_FILE_2216_112_2_18_2_52_43_69,"MMISMS:MMISMS_GetDisplayPathText path_ptr name_ptr full_path_ptr full_path_len_ptr = PNULL")
TRACE_MSG(MMISMS_FILE_2221_112_2_18_2_52_43_70,"MMISMS_CombineExportFile name_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d")
TRACE_MSG(MMISMS_FILE_2282_112_2_18_2_52_43_71,"MMISMS_SetExportFileHandle: file_handle is 0x%x")
TRACE_MSG(MMISMS_FILE_2291_112_2_18_2_52_43_72,"MMISMS_GetExportFileHandle: s_export_file_handle is 0x%x")
TRACE_MSG(MMISMS_FILE_2501_112_2_18_2_52_44_73,"MMISMS: SetExportFileContent length = %d")
TRACE_MSG(MMISMS_FILE_2526_112_2_18_2_52_44_74,"MMISMS:GetFileTransmitterDispString orginal_addr_ptr string_ptr = PNULL")
TRACE_MSG(MMISMS_FILE_2536_112_2_18_2_52_44_75,"GetFileTransmitterDispString num_name.wstr_ptr alloc fail")
TRACE_MSG(MMISMS_FILE_2547_112_2_18_2_52_44_76,"MMISMS: GetFileTransmitterDispString number_type = %d, number_plan = %d")
TRACE_MSG(MMISMS_FILE_2645_112_2_18_2_52_44_77,"WriteExportFileBoxType box_type is %d")
TRACE_MSG(MMISMS_FILE_2750_112_2_18_2_52_44_78,"WriteExportFileBoxType error is %d")
TRACE_MSG(MMISMS_FILE_2793_112_2_18_2_52_44_79,"MMISMS_SetExportTotalNum: total_num is %d")
TRACE_MSG(MMISMS_FILE_2803_112_2_18_2_52_44_80,"MMISMS_GetExportTotalNum: s_mmisms_export_total_num is %d")
TRACE_MSG(MMISMS_FILE_2813_112_2_18_2_52_44_81,"MMISMS_SetExportCurrentNum: current_num is %d")
TRACE_MSG(MMISMS_FILE_2823_112_2_18_2_52_45_82,"MMISMS_GetExportCurrentNum: s_mmisms_export_current_num is %d")
TRACE_MSG(MMISMS_FILE_2833_112_2_18_2_52_45_83,"MMISMS_SetExportCurBoxType: box_type is %d")
TRACE_MSG(MMISMS_FILE_2843_112_2_18_2_52_45_84,"MMISMS_GetExportCurBoxType: s_mmisms_cur_boxtype is %d")
TRACE_MSG(MMISMS_FILE_2866_112_2_18_2_52_45_85,"MMISMS_SetExportSecurityBoxFlag: is_export is %d")
TRACE_MSG(MMISMS_FILE_2876_112_2_18_2_52_45_86,"MMISMS_GetExportSecurityBoxFlag: s_mmisms_export_securitybox is %d")
TRACE_MSG(MMISMS_FILE_2890_112_2_18_2_52_45_87,"MMISMS_FreeExportFilePtr")
TRACE_MSG(MMISMS_NV_113_112_2_18_2_52_49_88,"MMISMS:MMISMS_NV_ReadSmsItem item_ptr = PNULL")
TRACE_MSG(MMISMS_NV_186_112_2_18_2_52_49_89,"SMS: MMISMS_NV_SetMaxSmsNum max num = %d")
TRACE_MSG(MMISMS_NV_219_112_2_18_2_52_49_90,"SMS: MMISMS_NV_DelAllSmsItem nv_result = %d")
TRACE_MSG(MMISMS_NV_293_112_2_18_2_52_49_91,"MMISMS:MMISMS_NV_ReadFolderTypeManager folder_type_ptr = PNULL")
TRACE_MSG(MMISMS_NV_313_112_2_18_2_52_49_92,"MMISMS: MMISMS_WriteFolderTypeManager folder_type_ptr = PNULL")
TRACE_MSG(MMISMS_NV_334_112_2_18_2_52_49_93,"MMISMS: MMISMS_NV_SetSecurityBoxPwd pwd_ptr = PNULL")
TRACE_MSG(MMISMS_NV_369_112_2_18_2_52_49_94,"MMISMS:MMISMS_ReadExportPath path_ptr = PNULL")
TRACE_MSG(MMISMS_NV_393_112_2_18_2_52_49_95,"MMISMS: MMISMS_WriteExportPath path_ptr = PNULL")
TRACE_MSG(MMISMS_NV_409_112_2_18_2_52_49_96,"MMISMS:MMISMS_ReadExportMethodType method_type = PNULL")
TRACE_MSG(MMISMS_READ_551_112_2_18_2_52_51_97,"SearchStrInReadMsg dst_len = %d count = %d")
TRACE_MSG(MMISMS_READ_657_112_2_18_2_52_51_98,"SearchStrInMMSMsg dst_len = %d count = %d")
TRACE_MSG(MMISMS_READ_746_112_2_18_2_52_51_99,"SearchStrInPushMsg dst_len = %d count = %d")
TRACE_MSG(MMISMS_READ_896_112_2_18_2_52_51_100,"HandleOkMsgInSmsList index is %d")
TRACE_MSG(MMISMS_READ_1272_112_2_18_2_52_52_101,"MMISMS:SaveReadedSmsToGlobal new_user_data_ptr address_ptr = PNULL")
TRACE_MSG(MMISMS_READ_1276_112_2_18_2_52_52_102,"MMISMS: SaveReadedSmsToGlobal alphabet_type = %d, msg_type = %d, dual_sys = %d ")
TRACE_MSG(MMISMS_READ_1303_112_2_18_2_52_52_103,"MMISMSR8:SaveReadedSmsToGlobal language is %d,is_lock_shift is %d,is_single_shift is %d")
TRACE_MSG(MMISMS_READ_1402_112_2_18_2_52_53_104,"MMISMS:MMISMS_AssembleSmsToGlobal new_user_data_ptr address_ptr = PNULL")
TRACE_MSG(MMISMS_READ_1409_112_2_18_2_52_53_105,"MMISMS_AssembleSmsToGlobal refnum_in_new = %d,refnum_in_save = %d")
TRACE_MSG(MMISMS_READ_1434_112_2_18_2_52_53_106,"MMISMS_AssembleSmsToGlobal seqnum_in_save = %d,seqnum_in_new = %d")
TRACE_MSG(MMISMS_READ_1495_112_2_18_2_52_53_107,"MMISMS: AssembleSms new_seq = %d, save_seq = %d")
TRACE_MSG(MMISMS_READ_1585_112_2_18_2_52_53_108,"MMISMS: AssembleSms new_seq = %d, save_seq = %d")
TRACE_MSG(MMISMS_READ_1606_112_2_18_2_52_53_109,"MMISMS:GetSeqNumFromUserHead user_data_head_ptr = PNULL")
TRACE_MSG(MMISMS_READ_1633_112_2_18_2_52_53_110,"MMISMS:GetMaxNumFromUserHead user_data_head_ptr = PNULL")
TRACE_MSG(MMISMS_READ_1666_112_2_18_2_52_53_111,"MMISMS:AppendString2ReadContent str_ptr = PNULL")
TRACE_MSG(MMISMS_READ_1682_112_2_18_2_52_53_112,"MMISMS: AppendString2ReadContent read_content_length = %d, append_len = %d")
TRACE_MSG(MMISMS_READ_1695_112_2_18_2_52_53_113,"AppendString2ReadContent len is %d")
TRACE_MSG(MMISMS_READ_1726_112_2_18_2_52_53_114,"MMISMS:AssembleStatusReportToGlobal status_report_ptr = PNULL")
TRACE_MSG(MMISMS_READ_1875_112_2_18_2_52_53_115,"MMISMS:MMISMS_AppReadSms order_id = PNULL")
TRACE_MSG(MMISMS_READ_1912_112_2_18_2_52_54_116,"MMISMS: MMISMS_AppReadSms order_id = %d, max_sms_num = %d, record_id = %d, storage = %d")
TRACE_MSG(MMISMS_READ_1915_112_2_18_2_52_54_117,"MMISMS:MMISMS_AppReadSms order_info.flag.dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMS_READ_1937_112_2_18_2_52_54_118,"MMISMS:GetSMSBoxTypeByReadSMSCnf sig_ptr = PNULL")
TRACE_MSG(MMISMS_READ_1987_112_2_18_2_52_54_119,"MMI_SMS:GetSMSBoxTypeByReadSMSCnf box_type=%d")
TRACE_MSG(MMISMS_READ_2022_112_2_18_2_52_54_120,"MMISMS:MMISMS_HandleUserReadSMSCnf sig_ptr = PNULL")
TRACE_MSG(MMISMS_READ_2028_112_2_18_2_52_54_121,"MMISMS:MMISMS_HandleUserReadSMSCnf sig_ptr->item_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMS_READ_2034_112_2_18_2_52_54_122,"SMS: MMISMS_HandleUserReadSMSCnf dual_sys = %d, cause = %d, status = %d, record_id = %d, storage = %d")
TRACE_MSG(MMISMS_READ_2049_112_2_18_2_52_54_123,"SMS: MMISMS_HandleUserReadSMSCnf g_sms_backup_data_ptr->time = %d, g_sms_backup_data_ptr->record_id = %d")
TRACE_MSG(MMISMS_READ_2054_112_2_18_2_52_54_124,"SMS: MMISMS_HandleUserReadSMSCnf sig_ptr->sms_t_u.mo_sms_t.sc_addr_t.num_len = %d")
TRACE_MSG(MMISMS_READ_2124_112_2_18_2_52_54_125,"sms:g_mmisms_global.read_msg.dual_sys = %d, g_mmisms_global.operation.cur_type = %d")
TRACE_MSG(MMISMS_READ_2140_112_2_18_2_52_54_126,"MMISMS: MMISMS_HandleUserReadSMSCnf head_len = %d")
TRACE_MSG(MMISMS_READ_2240_112_2_18_2_52_54_127,"MMISMS: OperateMtIndAsMmsPush mms head length = %d")
TRACE_MSG(MMISMS_READ_2399_112_2_18_2_52_55_128,"security_box_type is %d")
TRACE_MSG(MMISMS_READ_2428_112_2_18_2_52_55_129,"MMI_SMS: Warning---The oper type(%d)is changed(%d)! Discard!")
TRACE_MSG(MMISMS_READ_2463_112_2_18_2_52_55_130,"MMI_SMS: Warning---oper_status(%d)is MMISMS_READ_SMS! Discard!")
TRACE_MSG(MMISMS_READ_2513_112_2_18_2_52_55_131,"MMISMS_HandleUserReadSMSCnf seq num is %d,max num is %d")
TRACE_MSG(MMISMS_READ_2545_112_2_18_2_52_55_132,"sms:g_mmisms_global.read_msg.dual_sys = %d, g_mmisms_global.operation.cur_type = %d")
TRACE_MSG(MMISMS_READ_2579_112_2_18_2_52_55_133,"MMISMS: MMISMS_HandleUserReadSMSCnf head_len = %d")
TRACE_MSG(MMISMS_READ_2630_112_2_18_2_52_55_134,"MMISMS: MMISMS_HandleUserReadSMSCnf: the next_order_id = 0x%x")
TRACE_MSG(MMISMS_READ_2910_112_2_18_2_52_56_135,"MMISMS:MMISMS_GetRefNumFromUserHead user_data_head_ptr = PNULL")
TRACE_MSG(MMISMS_READ_2936_112_2_18_2_52_56_136,"SMS:MMISMS_HandleReadSmsCnf, record_id=%d, storage=%d, status=%d, dual_sys=%d")
TRACE_MSG(MMISMS_READ_2985_112_2_18_2_52_56_137,"MMISMS: Sms Order OK! time=%ld")
TRACE_MSG(MMISMS_READ_3079_112_2_18_2_52_56_138,"mmisms:MMISMS_InitSmsList")
TRACE_MSG(MMISMS_READ_3149_112_2_18_2_52_56_139,"MMISMS:ReadSmsForOrder dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMS_READ_3179_112_2_18_2_52_56_140,"MMISMS: ReadSmsForOrder status_ptr = PNULL")
TRACE_MSG(MMISMS_READ_3194_112_2_18_2_52_56_141,"ReadSmsForOrder SIM NOT Ready")
TRACE_MSG(MMISMS_READ_3208_112_2_18_2_52_56_142,"SMS ReadSmsForOrder storage=%d, record_id=%d, max_sms_num=%d")
TRACE_MSG(MMISMS_READ_3220_112_2_18_2_52_56_143,"SMS: ReadSmsForOrder the storage(%d) is ready, record_id = %d")
TRACE_MSG(MMISMS_READ_3322_112_2_18_2_52_56_144,"MMISMS:MMISMS_HandleSMSOrder sig_ptr = PNULL")
TRACE_MSG(MMISMS_READ_3348_112_2_18_2_52_56_145,"MMISMS_HandleUserReadSMSCnf:send_type is %d")
TRACE_MSG(MMISMS_READ_3391_112_2_18_2_52_57_146,"MMISMS: MMISMS_HandleSMSOrder sig_ptr->cause is %d")
TRACE_MSG(MMISMS_READ_3444_112_2_18_2_52_57_147,"MMISMS:HandleMtSMSOrder mt_sms_ptr = PNULL")
TRACE_MSG(MMISMS_READ_3506_112_2_18_2_52_57_148,"MMISMSR8:InsertNewMtToOrder language is %d,is_lock_shift is %d,is_single_shift is %d")
TRACE_MSG(MMISMS_READ_3519_112_2_18_2_52_57_149,"MMISMSR8:InsertNewMtToOrder content_len is %d")
TRACE_MSG(MMISMS_READ_3568_112_2_18_2_52_57_150,"HandleMtSMSOrder:err_code=%d,is_new_mms=%d,send_type=%d")
TRACE_MSG(MMISMS_READ_3595_112_2_18_2_52_57_151,"MMISMS: HandleMtSMSOrder: head_len = %d, ref_num = 0x%X, seq_num = 0x%X")
TRACE_MSG(MMISMS_READ_3606_112_2_18_2_52_57_152,"MMISMS: HandleMtSMSOrder: head_len = %d, ref_num = 0x%X, seq_num = 0x%X")
TRACE_MSG(MMISMS_READ_3636_112_2_18_2_52_57_153,"MMISMS:HandleSROrder mt_sr_ptr = PNULL")
TRACE_MSG(MMISMS_READ_3741_112_2_18_2_52_57_154,"MMISMS:HandleMoSMSOrder mo_sms_ptr = PNULL")
TRACE_MSG(MMISMS_READ_3771_112_2_18_2_52_57_155,"MMISMS: HandleMoSMSOrder head_len = %d")
TRACE_MSG(MMISMS_READ_3792_112_2_18_2_52_57_156,"HandleMoSMSOrder folder_type = %d,record_id = %d")
TRACE_MSG(MMISMS_READ_3807_112_2_18_2_52_57_157,"MMISMSR8:InsertNewMtToOrder language is %d,is_lock_shift is %d,is_single_shift is %d")
TRACE_MSG(MMISMS_READ_3820_112_2_18_2_52_57_158,"MMISMSR8:InsertNewMtToOrder content_len is %d")
TRACE_MSG(MMISMS_READ_3903_112_2_18_2_52_58_159,"HandleMoSMSOrder sc_addr_present is %d,len is %d")
TRACE_MSG(MMISMS_READ_3943_112_2_18_2_52_58_160,"MMISMS:MMISMS_GetNameByNumberFromPB name_ptr number_ptr = PNULL")
TRACE_MSG(MMISMS_READ_3966_112_2_18_2_52_58_161,"MMISMS: MMISMS_GetNameByNumberFromPB is_found = %d")
TRACE_MSG(MMISMS_READ_3998_112_2_18_2_52_58_162,"MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM")
TRACE_MSG(MMISMS_READ_4035_112_2_18_2_52_58_163,"MMISMS: MMISMS_GetContactInfo contact_info_ptr = PNULL")
TRACE_MSG(MMISMS_READ_4043_112_2_18_2_52_58_164,"MMISMS: MMISMS_GetContactInfo name_str.wstr_ptr = PNULL")
TRACE_MSG(MMISMS_READ_4051_112_2_18_2_52_58_165,"MMISMS: MMISMS_GetContactInfo email_str.wstr_ptr = PNULL")
TRACE_MSG(MMISMS_READ_4209_112_2_18_2_52_58_166,"MMISMS:AppendString2Contact conv_str_ptr org_str_ptr = PNULL")
TRACE_MSG(MMISMS_READ_4267_112_2_18_2_52_58_167,"MMISMS:MMISMS_GetSMSUseNumber num_arr_ptr = PNULL")
TRACE_MSG(MMISMS_READ_4390_112_2_18_2_52_59_168,"MMISMS:MMISMS_GetOriginTimeAndDate date_ptr time_ptr = PNULL")
TRACE_MSG(MMISMS_READ_4432_112_2_18_2_52_59_169,"StartSmsSortTimer: the timer has started!")
TRACE_MSG(MMISMS_READ_4482_112_2_18_2_52_59_170,"StopSmsSortTimer: the timer has stop!")
TRACE_MSG(MMISMS_READ_4658_112_2_18_2_52_59_171,"MMISMS: MMISMS_AppMoveMsgBox type = %d,oper_status = %d")
TRACE_MSG(MMISMS_READ_4831_112_2_18_2_52_59_172,"MMISMS_AppMoveMsgBox msg_type = %d")
TRACE_MSG(MMISMS_READ_4975_112_2_18_2_53_0_173,"MMISMS: MMISMS_AppDeleteMsgBox: The SmsBox is empty!")
TRACE_MSG(MMISMS_READ_5020_112_2_18_2_53_0_174,"MMISMS: MMISMS_AppMoveMsgBox type = %d,oper_status = %d")
TRACE_MSG(MMISMS_READ_5129_112_2_18_2_53_0_175,"MMISMS_AppMoveMsgBox msg_type = %d")
TRACE_MSG(MMISMS_READ_5177_112_2_18_2_53_0_176,"MMISMS: MMISMS_AppDeleteMsgBox: The SmsBox is empty!")
TRACE_MSG(MMISMS_READ_5245_112_2_18_2_53_0_177,"MMISMS_SetChatLongSmsItemContet:seq_len is %d,ref_num is %d,max is %d,seq is %d")
TRACE_MSG(MMISMS_RECEIVE_299_112_2_18_2_53_2_178,"MMISMS:AddJavaSMSData java_data_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_303_112_2_18_2_53_2_179,"MMISMS:AddJavaSMSData java_data_ptr = is_long =%d,total_num=%d,cur_num=%d")
TRACE_MSG(MMISMS_RECEIVE_334_112_2_18_2_53_2_180,"MMISMS:AddJavaSMSData javasms_func = PNULL")
TRACE_MSG(MMISMS_RECEIVE_413_112_2_18_2_53_2_181,"MMISMS: OperateMtIndForJava: user_data_ptr = PNULL!")
TRACE_MSG(MMISMS_RECEIVE_426_112_2_18_2_53_2_182,"MMISMS: OperateMtIndForJava: is_new_java_sms = %d")
TRACE_MSG(MMISMS_RECEIVE_433_112_2_18_2_53_2_183,"MMISMS: OperateMtIndForJava: valid_date_ptr alloc fail!")
TRACE_MSG(MMISMS_RECEIVE_443_112_2_18_2_53_2_184,"MMISMS: OperateMtIndForJava: iei_arr[%d].iei=%d")
TRACE_MSG(MMISMS_RECEIVE_496_112_2_18_2_53_2_185,"MMISMS:OperateMtIndForJava java_data_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_564_112_2_18_2_53_2_186,"MMISMS: OperateMtIndForJava: sms_t.dcs.alphabet_type = %d")
TRACE_MSG(MMISMS_RECEIVE_571_112_2_18_2_53_2_187,"MMISMS: OperateMtIndForJava: MN_SMS_UCS2_ALPHABET user_data_ptr->user_valid_data_t.length  %d")
TRACE_MSG(MMISMS_RECEIVE_578_112_2_18_2_53_2_188,"MMISMS: OperateMtIndForJava: user_data_ptr->user_valid_data_t.length = %d")
TRACE_MSG(MMISMS_RECEIVE_588_112_2_18_2_53_2_189,"second:MMISMS: OperateMtIndForJava: MN_SMS_UCS2_ALPHABET user_data_ptr->user_valid_data_t.length  %d")
TRACE_MSG(MMISMS_RECEIVE_594_112_2_18_2_53_2_190,"second:MMISMS: OperateMtIndForJava: user_data_ptr->user_valid_data_t.length = %d")
TRACE_MSG(MMISMS_RECEIVE_597_112_2_18_2_53_2_191,"seq_indx,split_bytesseq_len =%d,%d,%d")
TRACE_MSG(MMISMS_RECEIVE_605_112_2_18_2_53_2_192,"seq_indx,split_bytesseq_len =%d,%d,%d")
TRACE_MSG(MMISMS_RECEIVE_641_112_2_18_2_53_2_193,"SMS: InitLongVcardsmsStruct()")
TRACE_MSG(MMISMS_RECEIVE_687_112_2_18_2_53_3_194,"MMISMS:OperateMtIndAsSms user_data_ptr mt_sms_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_724_112_2_18_2_53_3_195,"MMISMS:OperateMtIndAsSms storage = %d, record_id = %d")
TRACE_MSG(MMISMS_RECEIVE_748_112_2_18_2_53_3_196," MMISMS:OperateMtIndAsSms storage = %d, record_id = %d")
TRACE_MSG(MMISMS_RECEIVE_798_112_2_18_2_53_3_197,"MMISMS: OperateMtIndAsSms head length = %d")
TRACE_MSG(MMISMS_RECEIVE_799_112_2_18_2_53_3_198,"MMISMS: OperateMtIndAsSms ref = %d")
TRACE_MSG(MMISMS_RECEIVE_817_112_2_18_2_53_3_199,"MMISMS: OperateMtIndAsSms need not update window")
TRACE_MSG(MMISMS_RECEIVE_902_112_2_18_2_53_3_200," MMISMS:OperateMtIndAsSms storage = %d, record_id = %d")
TRACE_MSG(MMISMS_RECEIVE_987_112_2_18_2_53_3_201,"MMISMS:InsertNewMtToOrder mt_sms_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_996_112_2_18_2_53_3_202,"MMISMS: InsertNewMtToOrder(%d,%d, %d, %d), pid_e=%d")
TRACE_MSG(MMISMS_RECEIVE_1045_112_2_18_2_53_3_203,"MMISMSR8:InsertNewMtToOrder content_len is %d,alpha is %d")
TRACE_MSG(MMISMS_RECEIVE_1055_112_2_18_2_53_3_204,"MMISMSR8:InsertNewMtToOrder language is %d,is_lock_shift is %d,is_single_shift is %d")
TRACE_MSG(MMISMS_RECEIVE_1068_112_2_18_2_53_3_205,"MMISMSR8:InsertNewMtToOrder content_len is %d")
TRACE_MSG(MMISMS_RECEIVE_1119_112_2_18_2_53_3_206,"MMISMS: InsertNewMtToOrder head_len = %d, ref_num = 0x%X, seq_num = 0x%X")
TRACE_MSG(MMISMS_RECEIVE_1368_112_2_18_2_53_4_207,"MMISMS:OperateMtIndAsMmsPush user_data_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_1382_112_2_18_2_53_4_208,"MMISMS: OperateMtIndAsMmsPush err_code=%d, is_new_mms=%d")
TRACE_MSG(MMISMS_RECEIVE_1414_112_2_18_2_53_4_209,"MMISMS: OperateMtIndAsMmsPush mms head length = %d")
TRACE_MSG(MMISMS_RECEIVE_1591_112_2_18_2_53_4_210,"MMISMS_R8:AddSMSContentItemtoRichText,language is %d,alphabet is %d")
TRACE_MSG(MMISMS_RECEIVE_1596_112_2_18_2_53_4_211,"MMISMS_R8:AddSMSContentItemtoRichText,convert_msg_len is %d,content_len is %d")
TRACE_MSG(MMISMS_RECEIVE_1854_112_2_18_2_53_5_212,"MMISMS_R8:AddSMSContentItemtoRichText,language is %d,alphabet is %d,&&=%d")
TRACE_MSG(MMISMS_RECEIVE_1863_112_2_18_2_53_5_213,"MMISMS_R8:AddSMSContentItemtoRichText,convert_msg_len is %d,content_len is %d")
TRACE_MSG(MMISMS_RECEIVE_1915_112_2_18_2_53_5_214,"MMISMS_R8:AddSMSContentItemtoRichText,convert_msg_len is %d,content_len is %d")
TRACE_MSG(MMISMS_RECEIVE_2059_112_2_18_2_53_5_215,"MMISMS:LoadSms2Window msg_data_ptr orginal_addr_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_2145_112_2_18_2_53_6_216,"MMISMS:ShowMsgContent msg_data_ptr orginal_addr_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_2202_112_2_18_2_53_6_217,"MMISMS_R8:ShowMsgContent convert_msg_len is %d,content_len is %d")
TRACE_MSG(MMISMS_RECEIVE_2291_112_2_18_2_53_6_218,"MMISMS_R8:ShowMsgContent language is %d,alphabet is %d")
TRACE_MSG(MMISMS_RECEIVE_2332_112_2_18_2_53_6_219,"MMISMS: ShowMsgContent length = %d")
TRACE_MSG(MMISMS_RECEIVE_2359_112_2_18_2_53_6_220,"MMISMS:MMISMS_GetTimeDispString time_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_2399_112_2_18_2_53_6_221,"MMISMS:GetTransmitterDispString orginal_addr_ptr string_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_2410_112_2_18_2_53_6_222,"MMISMS: GetTransmitterDispString number_type = %d, number_plan = %d")
TRACE_MSG(MMISMS_RECEIVE_2473_112_2_18_2_53_6_223,"MMISMS:MMISMS_HandleSMSMTInd sig_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_2480_112_2_18_2_53_6_224,"MMISMS: HandleSMSMTInd user_head_is_exist = %d, record_id = %d, storage = %d")
TRACE_MSG(MMISMS_RECEIVE_2736_112_2_18_2_53_7_225,"MMISMS:MMISMS_HandleSMSReportInd sig_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_2798_112_2_18_2_53_7_226,"MMISMS:MMISMS_HandleSMSReportInd dual_sys is %d,storage is %d,record_id is %d")
TRACE_MSG(MMISMS_RECEIVE_2855_112_2_18_2_53_7_227,"SMS: MMISMS_IsMtForVcard()")
TRACE_MSG(MMISMS_RECEIVE_2872_112_2_18_2_53_7_228,"SMS: MMISMS_IsMtForVcard: index = %d  port = %d ")
TRACE_MSG(MMISMS_RECEIVE_2923_112_2_18_2_53_7_229,"MMISMS:MMISMS_OperateMtIndForVcardOrVcal user_data_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_2928_112_2_18_2_53_7_230,"SMS: OperateMtIndForVcard: send_type = %d")
TRACE_MSG(MMISMS_RECEIVE_2944_112_2_18_2_53_7_231,"SMS: OperateMtIndForVcard: is_finished = %d")
TRACE_MSG(MMISMS_RECEIVE_2978_112_2_18_2_53_7_232,"MMISMS:OperateMtIndAsSms sms_ptr time_stamp_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_2981_112_2_18_2_53_7_233,"SMS: MMISMS_RecvVcardOrVcalSms")
TRACE_MSG(MMISMS_RECEIVE_3008_112_2_18_2_53_7_234,"SMS: MMISMS_RecvVcardOrVcalSms  index=%d")
TRACE_MSG(MMISMS_RECEIVE_3050_112_2_18_2_53_7_235,"SMS: MMISMS_RecvVcardOrVcalSms ref_num=%d,max_num=%d,seq_num=%d")
TRACE_MSG(MMISMS_RECEIVE_3055_112_2_18_2_53_7_236,"SMS: MMISMS_RecvVcardOrVcalSms ref_num=%d,max_num=%d,seq_num=%d,vcard_sms[i].ref_num=%d")
TRACE_MSG(MMISMS_RECEIVE_3066_112_2_18_2_53_7_237,"MMISMS_RecvVcardOrVcalSms the same seq sms")
TRACE_MSG(MMISMS_RECEIVE_3071_112_2_18_2_53_7_238,"SMS: MMISMS_RecvVcardOrVcalSms receive_num=%d")
TRACE_MSG(MMISMS_RECEIVE_3086_112_2_18_2_53_7_239,"SMS: MMISMS_RecvVcardOrVcalSms i=%d,current_time=%d")
TRACE_MSG(MMISMS_RECEIVE_3099_112_2_18_2_53_7_240,"SMS: MMISMS_RecvVcardOrVcalSms i=%d")
TRACE_MSG(MMISMS_RECEIVE_3105_112_2_18_2_53_7_241,"SMS: MMISMS_RecvVcardOrVcalSms empty_pos=%d")
TRACE_MSG(MMISMS_RECEIVE_3108_112_2_18_2_53_7_242,"SMS: MMISMS_RecvVcardOrVcalSms write global var,seq_num=%d,ref_num=%d,max_num=%d")
TRACE_MSG(MMISMS_RECEIVE_3124_112_2_18_2_53_8_243,"SMS: MMISMS_RecvVcardOrVcalSms write vcf file")
TRACE_MSG(MMISMS_RECEIVE_3178_112_2_18_2_53_8_244,"SMS: MMISMS_RecvVcardOrVcalSms free memory")
TRACE_MSG(MMISMS_RECEIVE_3182_112_2_18_2_53_8_245,"SMS: MMISMS_RecvVcardOrVcalSms exist vcard sms")
TRACE_MSG(MMISMS_RECEIVE_3206_112_2_18_2_53_8_246,"SMS: MMISMS_SaveVcalendarSms send_type=%d, vcard_data.length=%d")
TRACE_MSG(MMISMS_RECEIVE_3232_112_2_18_2_53_8_247,"MMISMS:GetVcardFileNameOverSms sc_time_ptr name_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_3255_112_2_18_2_53_8_248,"MMISMS:MMISMS_SaveVcardSms name_ptr buf_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_3280_112_2_18_2_53_8_249,"MMISMS:MMISMS_SaveVcardSms name_ptr buf_ptr = PNULL")
TRACE_MSG(MMISMS_RECEIVE_3305_112_2_18_2_53_8_250,"MMISMS_ShowNewVcardOrVcalendarPrompt type=%d")
TRACE_MSG(MMISMS_RECEIVE_3324_112_2_18_2_53_8_251,"SMS: MMISMS_ShowNewVcardOrVcalendarPrompt")
TRACE_MSG(MMISMS_SAVE_178_112_2_18_2_53_9_252,"MMISMS:SaveMsgReq user_data_ptr  = PNULL")
TRACE_MSG(MMISMS_SAVE_183_112_2_18_2_53_9_253,"MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL")
TRACE_MSG(MMISMS_SAVE_249_112_2_18_2_53_9_254,"MMISMS: SaveMsgReq err_code = %d")
TRACE_MSG(MMISMS_SAVE_261_112_2_18_2_53_9_255,"MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED")
TRACE_MSG(MMISMS_SAVE_287_112_2_18_2_53_9_256,"MMISMS:SaveMsgReq user_data_ptr  = PNULL")
TRACE_MSG(MMISMS_SAVE_292_112_2_18_2_53_9_257,"MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL")
TRACE_MSG(MMISMS_SAVE_329_112_2_18_2_53_9_258,"MMISMS: SaveMsgReq err_code = %d")
TRACE_MSG(MMISMS_SAVE_341_112_2_18_2_53_9_259,"MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED")
TRACE_MSG(MMISMS_SAVE_371_112_2_18_2_53_9_260,"MMISMS:SaveMsgReq user_data_ptr  = PNULL")
TRACE_MSG(MMISMS_SAVE_376_112_2_18_2_53_9_261,"MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL")
TRACE_MSG(MMISMS_SAVE_429_112_2_18_2_53_9_262,"MMISMS: UpdateMsgReq err_code = %d")
TRACE_MSG(MMISMS_SAVE_442_112_2_18_2_53_9_263,"MMISMS: UpdateMsgReq the storage is MN_SMS_NO_STORED")
TRACE_MSG(MMISMS_SAVE_486_112_2_18_2_53_10_264,"MMISMS:MMISMS_HandleSaveSMSCnf sig_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_490_112_2_18_2_53_10_265,"MMISMS: HandleSaveSMSCnf record_id = %d, storage = %d, cause = %d, dual_sys = %d, oper_status = %d")
TRACE_MSG(MMISMS_SAVE_508_112_2_18_2_53_10_266,"MMISMS: HandleSaveSMSCnf 11 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d")
TRACE_MSG(MMISMS_SAVE_525_112_2_18_2_53_10_267,"MMISMS: HandleSaveSMSCnf 22 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d")
TRACE_MSG(MMISMS_SAVE_624_112_2_18_2_53_10_268,"MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SAVE_SEND_SMS send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_627_112_2_18_2_53_10_269,"HandleSaveSMSCnf:MMISMS_SAVE_SEND_SMS ")
TRACE_MSG(MMISMS_SAVE_710_112_2_18_2_53_10_270,"MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SEND_SAVE_SMS send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_713_112_2_18_2_53_10_271,"HandleSaveSMSCnf:resend_count is %d,send_result is %d")
TRACE_MSG(MMISMS_SAVE_733_112_2_18_2_53_10_272,"MMISMS: UpdateMsgReq err_code = %d")
TRACE_MSG(MMISMS_SAVE_770_112_2_18_2_53_10_273,"MMISMS: HandleSaveSMSCnf Deliver Cancel!")
TRACE_MSG(MMISMS_SAVE_861_112_2_18_2_53_10_274,"MMISMS: HandleSaveSMSCnf oper_status(%d) is error")
TRACE_MSG(MMISMS_SAVE_990_112_2_18_2_53_11_275,"MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SEND_SAVE_SMS send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_1127_112_2_18_2_53_11_276,"MMISMS: MMISMS_HandleSaveSMSCnf %d")
TRACE_MSG(MMISMS_SAVE_1146_112_2_18_2_53_11_277,"MMISMS: MMISMS_HandleSaveSMSCnf %d")
TRACE_MSG(MMISMS_SAVE_1163_112_2_18_2_53_11_278,"MMISMS: MMISMS_HandleSaveSMSCnf %d")
TRACE_MSG(MMISMS_SAVE_1217_112_2_18_2_53_11_279,"MMISMS: MMISMS_HandleSaveSMSCnf %d")
TRACE_MSG(MMISMS_SAVE_1233_112_2_18_2_53_11_280,"MMISMS: MMISMS_HandleSaveSMSCnf %d")
TRACE_MSG(MMISMS_SAVE_1302_112_2_18_2_53_11_281,"MMISMS:MMISMS_AppSaveSms send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_1403_112_2_18_2_53_11_282,"MMISMS: SaveInfoToOrderByResult(%d, %d, %d, %d)")
TRACE_MSG(MMISMS_SAVE_1408_112_2_18_2_53_11_283,"MMISMS:MMISMS_InterceptMsgContent PNULL !=  send_info_ptr")
TRACE_MSG(MMISMS_SAVE_1413_112_2_18_2_53_11_284,"MMISMS:MMISMS_InterceptMsgContent PNULL ==  send_info_ptr")
TRACE_MSG(MMISMS_SAVE_1431_112_2_18_2_53_11_285,"seq_num is %d,max_num is %d")
TRACE_MSG(MMISMS_SAVE_1445_112_2_18_2_53_11_286,"seq_num is %d,max_num is %d")
TRACE_MSG(MMISMS_SAVE_1753_112_2_18_2_53_12_287,"MMISMS:MMISMS_AppUpdateMoSms send_info_ptr != PNULL")
TRACE_MSG(MMISMS_SAVE_1831_112_2_18_2_53_12_288,"MMISMS:MMISMS_SaveNextMessage send_info_ptr != PNULL")
TRACE_MSG(MMISMS_SAVE_2052_112_2_18_2_53_13_289,"MMISMS:MMISMS_EncodeMoSmsData mo_sms_ptr user_data_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2067_112_2_18_2_53_13_290,"MMISMS: MMISMS_EncodeMoSmsData user_head_is_exist = %d")
TRACE_MSG(MMISMS_SAVE_2093_112_2_18_2_53_13_291,"MMISMS:MMISMS_EncodeMoSmsData mt_sms_ptr user_data_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2107_112_2_18_2_53_13_292,"MMISMS: MMISMS_EncodeMtSmsData user_head_is_exist = %d")
TRACE_MSG(MMISMS_SAVE_2139_112_2_18_2_53_13_293,"MMISMS:MMISMS_GetStoreStorage storage_ptr record_id_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2186_112_2_18_2_53_13_294,"MMISMS:MMISMS_GetStoreStorage status_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2229_112_2_18_2_53_13_295,"MMISMS: MMISMS_GetStoreStorage storage = %d, record_id = %d")
TRACE_MSG(MMISMS_SAVE_2253_112_2_18_2_53_13_296,"MMISMS:MMISMS_GetStoreStorageForSendLongSms storage_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2336_112_2_18_2_53_13_297,"MMISMS:MMISMS_GetStoreStorageForSendLongSms 000")
TRACE_MSG(MMISMS_SAVE_2340_112_2_18_2_53_13_298,"SMS: MMISMS_GetStoreStorageForSendLongSms storage = %d")
TRACE_MSG(MMISMS_SAVE_2547_112_2_18_2_53_14_299,"MMISMS:MMISMS_SetSMSSaveInfo dest_num_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2572_112_2_18_2_53_14_300,"MMISMS:MMISMS_GetSMSMemory max_num_ptr used_num_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2579_112_2_18_2_53_14_301,"SMS: MMISMS_GetSMSMemory dual_sys=%d, is_nv = %d, max_num = %d, used_num = %d")
TRACE_MSG(MMISMS_SAVE_2595_112_2_18_2_53_14_302,"MMISMS:MMISMS_GetLenByAlpha split_len_ptr num_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2654_112_2_18_2_53_14_303,"SMS: MMISMS_GetLenByAlpha alpha = %d")
TRACE_MSG(MMISMS_SAVE_2683_112_2_18_2_53_14_304,"MMISMS:MMISMS_GetUserData sms_content_ptr tpdu_head_ptr user_data_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2736_112_2_18_2_53_14_305,"SMS: MMISMS_GetUserData sms_index = %d")
TRACE_MSG(MMISMS_SAVE_2754_112_2_18_2_53_14_306,"MMISMS: MMISMS_GetUserData: valid data len  is = %d,contect len = %d, msg len =%d = %d")
TRACE_MSG(MMISMS_SAVE_2759_112_2_18_2_53_14_307,"MMISMS: MMISMS_GetUserData length = %d")
TRACE_MSG(MMISMS_SAVE_2801_112_2_18_2_53_14_308,"SMS: MMISMS_GetUserData tpdu_head_ptr->length = %d")
TRACE_MSG(MMISMS_SAVE_2810_112_2_18_2_53_14_309,"MMISMS: MMISMS_GetUserData: HEAD LEN is = %d")
TRACE_MSG(MMISMS_SAVE_2838_112_2_18_2_53_14_310,"MMISMS:MMISMS_FindFreeRecordId record_id_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2847_112_2_18_2_53_14_311,"MMISMS:MMISMS_FindFreeRecordId status_arr.mn_sms_status_arr_e_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_2889_112_2_18_2_53_14_312,"MMISMS: MMISMS_FindFreeRecordId storage = %d, record_id = %d")
TRACE_MSG(MMISMS_SAVE_2910_112_2_18_2_53_14_313,"MMISMS:MMISMS_UpdateStatusReportMsgReq storage = MN_SMS_STORAGE_ALL")
TRACE_MSG(MMISMS_SAVE_2917_112_2_18_2_53_14_314,"MMISMS: UpdateMsgReq err_code = %d")
TRACE_MSG(MMISMS_SAVE_2930_112_2_18_2_53_14_315,"MMISMS: UpdateMsgStatusReportReq the storage is MN_SMS_NO_STORED")
TRACE_MSG(MMISMS_SAVE_2951_112_2_18_2_53_14_316,"MMISMS_MOVESaveSms:mo_mt_type is %d")
TRACE_MSG(MMISMS_SAVE_3040_112_2_18_2_53_15_317,"MMISMS: MMISMS_CopySaveSms ret_val = %d")
TRACE_MSG(MMISMS_SAVE_3084_112_2_18_2_53_15_318,"MMISMS_MOVESaveSms:mo_mt_type is %d")
TRACE_MSG(MMISMS_SAVE_3164_112_2_18_2_53_15_319,"MMISMS: MMISMS_CopySaveSms ret_val = %d")
TRACE_MSG(MMISMS_SAVE_3287_112_2_18_2_53_15_320,"MMISMS: SaveMsgReq err_code = %d")
TRACE_MSG(MMISMS_SAVE_3299_112_2_18_2_53_15_321,"MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED")
TRACE_MSG(MMISMS_SAVE_3335_112_2_18_2_53_16_322,"MMISMS_CopyNextMessage ret_val = %d")
TRACE_MSG(MMISMS_SAVE_3370_112_2_18_2_53_16_323,"SMS: IsExistNextSMS %d")
TRACE_MSG(MMISMS_SAVE_3442_112_2_18_2_53_16_324,"SMS: Get_Current_Max_Num =%d")
TRACE_MSG(MMISMS_SAVE_3470_112_2_18_2_53_16_325,"SMS: Get_Current_Ref_Num =%d")
TRACE_MSG(MMISMS_SAVE_3498_112_2_18_2_53_16_326,"SMS: Get_Current_Seq_Num =%d")
TRACE_MSG(MMISMS_SAVE_3524_112_2_18_2_53_16_327,"MMISMS:MMISMS_GetReadUserData sms_content_ptr tpdu_head_ptr user_data_ptr = PNULL")
TRACE_MSG(MMISMS_SAVE_3562_112_2_18_2_53_16_328,"SMS: MMISMS_GetReadUserData sms_index = %d")
TRACE_MSG(MMISMS_SAVE_3580_112_2_18_2_53_16_329,"MMISMS: MMISMS_GetReadUserData: valid data len  is = %d,contect len = %d, msg len =%d = %d")
TRACE_MSG(MMISMS_SAVE_3585_112_2_18_2_53_16_330,"MMISMS: MMISMS_GetReadUserData length = %d")
TRACE_MSG(MMISMS_SAVE_3627_112_2_18_2_53_16_331,"SMS: MMISMS_GetReadUserData tpdu_head_ptr->length = %d")
TRACE_MSG(MMISMS_SAVE_3636_112_2_18_2_53_16_332,"MMISMS: MMISMS_GetReadUserData: HEAD LEN is = %d")
TRACE_MSG(MMISMS_SEND_204_112_2_18_2_53_17_333,"MMISMS:HandleSMSSendSuccessCnf send_message = PNULL")
TRACE_MSG(MMISMS_SEND_332_112_2_18_2_53_17_334,"MMISMS:HandleSMSSendErrorCnf PNULL ==  send_info_ptr")
TRACE_MSG(MMISMS_SEND_338_112_2_18_2_53_17_335,"MMISMS:HandleSMSSendErrorCnf cause ==  MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(MMISMS_SEND_354_112_2_18_2_53_17_336,"HandleSMSSendErrorCnf:cancel send sms is success")
TRACE_MSG(MMISMS_SEND_364_112_2_18_2_53_17_337,"HandleSMSSendErrorCnf:resend_count is %d")
TRACE_MSG(MMISMS_SEND_411_112_2_18_2_53_17_338,"MMISMS:HandleSMSSendCnfErrorNoResend PNULL ==  send_info_ptr")
TRACE_MSG(MMISMS_SEND_415_112_2_18_2_53_17_339,"HandleSMSSendCnfErrorNoResend send type")
TRACE_MSG(MMISMS_SEND_459_112_2_18_2_53_17_340,"MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d")
TRACE_MSG(MMISMS_SEND_530_112_2_18_2_53_17_341,"MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d")
TRACE_MSG(MMISMS_SEND_625_112_2_18_2_53_18_342,"MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d")
TRACE_MSG(MMISMS_SEND_670_112_2_18_2_53_18_343,"MMISMS: HandleSMSSendCnfErrorNoResend is_finish = %d")
TRACE_MSG(MMISMS_SEND_734_112_2_18_2_53_18_344,"MMISMS:InitUserDataHeadForJAVA data_head_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_737_112_2_18_2_53_18_345,"mmivcard:InitUserDataHeadForVcard srcPort=%d")
TRACE_MSG(MMISMS_SEND_763_112_2_18_2_53_18_346,"MMISMS:HandleSMSSendCnf PNULL ==  send_info_ptr")
TRACE_MSG(MMISMS_SEND_766_112_2_18_2_53_18_347,"MMISMS: HandleSMSSendCnf cause = %d")
TRACE_MSG(MMISMS_SEND_803_112_2_18_2_53_18_348,"MMISMS:OperAfterSendCnf PNULL ==  send_info_ptr")
TRACE_MSG(MMISMS_SEND_1015_112_2_18_2_53_18_349,"MMISMS:SendMsgReqToMN dest_number_ptr user_data_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1018_112_2_18_2_53_18_350,"SMS: SendMsgReqToMN storage = %d, dual_sys = %d,is_resend=%d")
TRACE_MSG(MMISMS_SEND_1087_112_2_18_2_53_18_351,"MMISMS: SendMsgReqToMN is_more_msg=%d,send_path=%d")
TRACE_MSG(MMISMS_SEND_1098_112_2_18_2_53_18_352,"MMISMS: SendMsgReqToMN mn_err_code = %d")
TRACE_MSG(MMISMS_SEND_1153_112_2_18_2_53_19_353,"MMISMS:MMISMS_SendMsgBySeqNum send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1161_112_2_18_2_53_19_354,"MMISMS: MMISMS_SendMsgBySeqNum seq_num = %d, dual_sys = %d")
TRACE_MSG(MMISMS_SEND_1167_112_2_18_2_53_19_355,"MMISMS:MMISMS_SendMsgBySeqNum seq_num > GET_HEAD_MAX_NUM()")
TRACE_MSG(MMISMS_SEND_1242_112_2_18_2_53_19_356,"MMISMS:MMISMS_SendMsgBySeqNum !MMISMS_GetDestNumber()")
TRACE_MSG(MMISMS_SEND_1257_112_2_18_2_53_19_357,"MMISMS: CloseSendWinByResult(%d)")
TRACE_MSG(MMISMS_SEND_1286_112_2_18_2_53_19_358,"MMISMS:MMISMS_ClearDestAddrById PNULL ==  send_info_ptr")
TRACE_MSG(MMISMS_SEND_1347_112_2_18_2_53_19_359,"MMISMS:SendFailCount PNULL ==  send_info_ptr")
TRACE_MSG(MMISMS_SEND_1370_112_2_18_2_53_19_360,"MMISMS:MMISMS_GetDestNumber dest_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1376_112_2_18_2_53_19_361,"MMISMS:MMISMS_GetDestNumber dest_number_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1413_112_2_18_2_53_19_362,"MMISMS:MMISMS_SplitLongMsg sms_content_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1491_112_2_18_2_53_19_363,"MMISMS:MMISMS_CheckNextDestAddr send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1499_112_2_18_2_53_19_364,"MMISMS: MMISMS_CheckNextDestAddr: the number is = %d, the addr id = %d")
TRACE_MSG(MMISMS_SEND_1521_112_2_18_2_53_19_365,"MMISMS:MMISMS_IsExistNextSMS send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1533_112_2_18_2_53_19_366,"MMISMS_IsExistNextSMS:max_num is %d,seq_num is %d")
TRACE_MSG(MMISMS_SEND_1542_112_2_18_2_53_19_367,"SMS: IsExistNextSMS %d")
TRACE_MSG(MMISMS_SEND_1570_112_2_18_2_53_19_368,"MMISMS:MMISMS_InitUserDataHead data_head_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1575_112_2_18_2_53_19_369,"MMISMS:MMISMS_InitUserDataHead 0 >= max_num || max_num > MMISMS_SPLIT_MAX_NUM")
TRACE_MSG(MMISMS_SEND_1580_112_2_18_2_53_19_370,"MMISMS:MMISMS_InitUserDataHead 0 >= seq_num || seq_num > max_num")
TRACE_MSG(MMISMS_SEND_1584_112_2_18_2_53_19_371,"MMISMS: MMISMS_InitUserDataHead(%d, %d)")
TRACE_MSG(MMISMS_SEND_1641_112_2_18_2_53_20_372,"mmisms send_info_ptr null")
TRACE_MSG(MMISMS_SEND_1663_112_2_18_2_53_20_373,"MMISMS:MMISMS_MoveDestAddrId send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1690_112_2_18_2_53_20_374,"MMISMS:MMISMS_GetResultofSending status_id_ptr dest_count_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1697_112_2_18_2_53_20_375,"MMISMS:MMISMS_GetResultofSending send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1702_112_2_18_2_53_20_376,"MMISMS: MMISMS_GetResultofSending dest_addr_id = %d")
TRACE_MSG(MMISMS_SEND_1714_112_2_18_2_53_20_377,"MMISMS: MMISMS_GetResultofSending: list_num = %d")
TRACE_MSG(MMISMS_SEND_1715_112_2_18_2_53_20_378,"MMISMS: MMISMS_GetResultofSending: dest_count = %d")
TRACE_MSG(MMISMS_SEND_1789_112_2_18_2_53_20_379,"MMISMS:MMISMS_ShowSendResult fail_id_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1792_112_2_18_2_53_20_380,"MMISMS: MMISMS_ShowSendResult  fail_count = %d, id_ptr = %p ")
TRACE_MSG(MMISMS_SEND_1890_112_2_18_2_53_20_381,"MMISMS:MMISMS_HandleSendCnf sig_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1901_112_2_18_2_53_20_382,"MMISMS:MMISMS_HandleSendCnf sig_ptr->cause = %d")
TRACE_MSG(MMISMS_SEND_1964_112_2_18_2_53_20_383,"MMISMS:MMISMS_AppSendSms send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_1980_112_2_18_2_53_20_384,"MMISMS: MMISMS_AppSendSms is_need_save = %d")
TRACE_MSG(MMISMS_SEND_1983_112_2_18_2_53_20_385,"MMISMS: MMISMS_AppSendSms total_num = %d")
TRACE_MSG(MMISMS_SEND_1992_112_2_18_2_53_20_386,"MMISMS: MMISMS_AppSendSms send_result = %d")
TRACE_MSG(MMISMS_SEND_2008_112_2_18_2_53_20_387," MMISMS_HandleSendCnfWhenWriteCnf: send_status = %d")
TRACE_MSG(MMISMS_SEND_2013_112_2_18_2_53_20_388,"MMISMS:MMISMS_HandleSendCnfWhenWriteCnf send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2221_112_2_18_2_53_21_389,"MMISMS:MMISMS_SetAddressToMessage num_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2226_112_2_18_2_53_21_390,"MMISMS:MMISMS_SetAddressToMessage count = %d")
TRACE_MSG(MMISMS_SEND_2268_112_2_18_2_53_21_391,"MMISMS: MMISMS_GetDestCount count = %d")
TRACE_MSG(MMISMS_SEND_2289_112_2_18_2_53_21_392,"MMISMS:MMISMS_GetPointedDestNum num_len_ptr num_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2333_112_2_18_2_53_21_393,"MMISMS:MMISMS_DeleteDestNum send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2392_112_2_18_2_53_21_394,"MMISMS_GetDeliverResendFlag: g_mmisms_global.edit_content.is_resend = %d")
TRACE_MSG(MMISMS_SEND_2404_112_2_18_2_53_21_395,"MMISMS_SetDeliverResendFlag: is_resend = %d")
TRACE_MSG(MMISMS_SEND_2423_112_2_18_2_53_21_396,"MMISMS:MMISMS_GetSMSContent string_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2472_112_2_18_2_53_21_397,"MMISMS_R8:MMISMS_GetSMSContent language is %d,is_lock_shift is %d,is_single_shift is %d")
TRACE_MSG(MMISMS_SEND_2485_112_2_18_2_53_21_398,"MMISMS_R8:MMISMS_GetSMSContent content_len is %d")
TRACE_MSG(MMISMS_SEND_2578_112_2_18_2_53_21_399," MMISMS_WintabSendSMS (PNULL == send_info_ptr")
TRACE_MSG(MMISMS_SEND_2696_112_2_18_2_53_22_400,"MMISMS:MMISMS_WintabSendVcard p_temp = PNULL")
TRACE_MSG(MMISMS_SEND_2708_112_2_18_2_53_22_401,"MMISMS_WintabSendVcard, err_code = %d")
TRACE_MSG(MMISMS_SEND_2737_112_2_18_2_53_22_402,"MMISMS:MMISMS_WintabSendVcalendar p_temp = PNULL")
TRACE_MSG(MMISMS_SEND_2749_112_2_18_2_53_22_403,"MMISMS_WintabSendVcalendar, err_code = %d")
TRACE_MSG(MMISMS_SEND_2767_112_2_18_2_53_22_404,"MMISMS:MMISMS_AppSendSmsForVcardAndVcal sms_init_ptr sms_init_ptr->wstr_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2771_112_2_18_2_53_22_405,"mmivcard:MMISMS_AppSendSmsForVcardAndVcal")
TRACE_MSG(MMISMS_SEND_2777_112_2_18_2_53_22_406,"MMISMS MMISMS_AppSendSmsForVcardAndVcal PNULL == send_message")
TRACE_MSG(MMISMS_SEND_2844_112_2_18_2_53_22_407,"MMISMS:MMISMS_HandleWriteError send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2903_112_2_18_2_53_22_408,"MMISMS:Set_Longsms_Head_Var head_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2907_112_2_18_2_53_22_409,"SMS: Set_Longsms_Head_Var ref_num=%d,max_num=%d,seq_num=%d")
TRACE_MSG(MMISMS_SEND_2936_112_2_18_2_53_22_410,"MMISMS:Set_SmsR8_Head_Var head_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2975_112_2_18_2_53_22_411,"MMISMS:Get_Current_Ref_Num edit_content_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_2989_112_2_18_2_53_22_412,"SMS: Get_Current_Ref_Num =%d")
TRACE_MSG(MMISMS_SEND_3009_112_2_18_2_53_22_413,"MMISMS:Get_Head_Max_Num edit_content_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_3028_112_2_18_2_53_22_414,"SMS: Get_Current_Max_Num =%d")
TRACE_MSG(MMISMS_SEND_3048_112_2_18_2_53_22_415,"MMISMS:Get_Current_Seq_Num edit_content_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_3062_112_2_18_2_53_22_416,"SMS: Get_Current_Seq_Num =%d")
TRACE_MSG(MMISMS_SEND_3083_112_2_18_2_53_22_417,"MMISMS:Set_Current_Seq_Num head_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_3086_112_2_18_2_53_22_418,"SMS: Set_Current_Seq_Num seq_num=%d")
TRACE_MSG(MMISMS_SEND_3125_112_2_18_2_53_23_419,"MMISMS:MMISMS_GetMnCalledNum dest_ptr party_number_ptr dest_number_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_3160_112_2_18_2_53_23_420,"MMISMS:MMISMS_SetDeliverCancel send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_3163_112_2_18_2_53_23_421,"MMISMS: MMISMS_SetDeliverCancel=%d")
TRACE_MSG(MMISMS_SEND_3180_112_2_18_2_53_23_422,"MMISMS:MMISMS_GetDeliverCancel send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_3184_112_2_18_2_53_23_423,"MMISMS: MMISMS_GetDeliverCancel=%d")
TRACE_MSG(MMISMS_SEND_3200_112_2_18_2_53_23_424,"MMISMS: MMISMS_ResponseSmsCtl dual_sys=%d, is_need_modified=%d")
TRACE_MSG(MMISMS_SEND_3220_112_2_18_2_53_23_425,"MMISMS MMISMS_ResponseSmsCtl PNULL == send_message")
TRACE_MSG(MMISMS_SEND_3266_112_2_18_2_53_23_426,"[MMISMS] MMISMS_ResponseSmsCtl Success!")
TRACE_MSG(MMISMS_SEND_3293_112_2_18_2_53_23_427,"[MMISMS] alloc fail")
TRACE_MSG(MMISMS_SEND_3305_112_2_18_2_53_23_428,"[MMISMS] MMISMS_CreateSendSMS Success!")
TRACE_MSG(MMISMS_SEND_3376_112_2_18_2_53_23_429,"[MMISMS] MMISMS_AddSendList status error  !")
TRACE_MSG(MMISMS_SEND_3386_112_2_18_2_53_23_430,"[MMISMS] MMISMS_AddSendList s_mmisms_sendlist.send_num = %d!")
TRACE_MSG(MMISMS_SEND_3398_112_2_18_2_53_23_431,"[MMISMS] MMISMS_AddSendList the same one!")
TRACE_MSG(MMISMS_SEND_3531_112_2_18_2_53_23_432,"[MMISMS] MMISMS_DeleteSendList no sms delete!")
TRACE_MSG(MMISMS_SEND_3568_112_2_18_2_53_23_433,"[MMISMS] MMISMS_DeleteSendList s_mmisms_sendlist.send_num = %d!")
TRACE_MSG(MMISMS_SEND_3611_112_2_18_2_53_24_434,"MMISMS:MMISMS_GetSendInfoFromSendListByRecordId send_list_num >=  MMISMS_SENDLIST_MAX_NUM")
TRACE_MSG(MMISMS_SEND_3623_112_2_18_2_53_24_435,"MMISMS:MMISMS_GetSendInfoFromSendListByRecordId send_list_num ==  %d")
TRACE_MSG(MMISMS_SEND_3652_112_2_18_2_53_24_436,"MMISMS:MMISMS_SetBackSendStatus longsms_index >=  MMISMS_SPLIT_MAX_NUM")
TRACE_MSG(MMISMS_SEND_3673_112_2_18_2_53_24_437,"MMISMS:MMISMS_SetBackSendStatus longsms_index >=  MMISMS_SPLIT_MAX_NUM")
TRACE_MSG(MMISMS_SEND_3697_112_2_18_2_53_24_438,"[MMISMS] MMISMS_ActiveSendList PNULL == send_info_ptr!")
TRACE_MSG(MMISMS_SEND_3700_112_2_18_2_53_24_439,"[MMISMS] MMISMS_ActiveSendList send_info_ptr->backsend_status = %d !")
TRACE_MSG(MMISMS_SEND_3781_112_2_18_2_53_24_440,"[MMISMS] MMISMS_AppSendSmsToMn sms no dest address!")
TRACE_MSG(MMISMS_SEND_3784_112_2_18_2_53_24_441,"[MMISMS] MMISMS_AppSendSmsToMn send_result = %d")
TRACE_MSG(MMISMS_SEND_3838_112_2_18_2_53_24_442,"MMISMS MMISMS_SendSmsForOther PNULL == send_data_ptr || PNULL == sms_content_ptr")
TRACE_MSG(MMISMS_SEND_3843_112_2_18_2_53_24_443,"MMISMS_SendSmsForOther dual_sys = %d src_port = %d dst_port = %d")
TRACE_MSG(MMISMS_SEND_3849_112_2_18_2_53_24_444,"MMISMS MMISMS_SendSmsForOther PNULL == dest_addr_ptr || PNULL == sms_content_ptr")
TRACE_MSG(MMISMS_SEND_3856_112_2_18_2_53_24_445," MMISMS_SendSmsForOther sim is not ok,can't send sms!")
TRACE_MSG(MMISMS_SEND_3864_112_2_18_2_53_24_446,"MMISMS MMISMS_SendSmsForOther PNULL == send_info_ptr")
TRACE_MSG(MMISMS_SEND_3922_112_2_18_2_53_24_447,"MMISMS_SendSmsForOther err_code == %d")
TRACE_MSG(MMISMS_SEND_3944_112_2_18_2_53_24_448,"MMISMS:InitUserDataHeadForOtherModules data_head_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_3948_112_2_18_2_53_24_449,"MMISMS: InitUserDataHeadForOtherModules (srcPort:%d, dstPort:%d)")
TRACE_MSG(MMISMS_SEND_3990_112_2_18_2_53_24_450,"MMISMS:MMISMS_UpdateSmsSendindNumber send_info_ptr = PNULL")
TRACE_MSG(MMISMS_SEND_3995_112_2_18_2_53_24_451,"MMISMS:MMISMS_UpdateSmsSendindNumber backsend_status != MMISMS_SENDSTATUS_SENDING")
TRACE_MSG(MMISMS_SEND_4006_112_2_18_2_53_24_452,"MMISMS:MMISMS_UpdateSmsSendindNumber dest_addr_id = %d, update_record_id = %d")
TRACE_MSG(MMISMS_SEND_4041_112_2_18_2_53_25_453,"MMISMS: MMISMS_UpdateCurSendRecordId 11 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d, sending_record_id = %d")
TRACE_MSG(MMISMS_SEND_4194_112_2_18_2_53_25_454,"MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM")
TRACE_MSG(MMISMS_SEND_4230_112_2_18_2_53_25_455,"MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM")
TRACE_MSG(MMISMS_SET_279_112_2_18_2_53_26_456,"MMISMS: MMISMS_SetRefNumToNV ref_num = %d")
TRACE_MSG(MMISMS_SET_356_112_2_18_2_53_26_457,"MMISMS:MMISMS_GetFrequentWord word_ptr = PNULL")
TRACE_MSG(MMISMS_SET_379_112_2_18_2_53_26_458,"MMISMS:MMISMS_ModifyPointedWord item_phase_ptr = PNULL")
TRACE_MSG(MMISMS_SET_512_112_2_18_2_53_26_459,"MMISMS:MMISMS_AppGetSCAddr sc_addr sc_addr_len = PNULL")
TRACE_MSG(MMISMS_SET_615_112_2_18_2_53_27_460,"MMISMS:MMISMS_DecodeUserDataHead user_data_header_ptr_t user_head_ptr_t = PNULL")
TRACE_MSG(MMISMS_SET_663_112_2_18_2_53_27_461,"MMISMS:MMISMS_FreeUserDataHeadSpace user_head_ptr_t = PNULL")
TRACE_MSG(MMISMS_SET_750_112_2_18_2_53_27_462,"MMISMS:MMISMS_Default2NationalR8 default_ptr national_ptr = PNULL")
TRACE_MSG(MMISMS_SET_858_112_2_18_2_53_27_463,"MMISMS:MMISMS_Default2NationalR8 default_ptr national_ptr = PNULL")
TRACE_MSG(MMISMS_SET_1590_112_2_18_2_53_29_464,"MMIAPISET_SetTTSInfo cur_selection = %d")
TRACE_MSG(MMISMS_SETTINGWIN_714_112_2_18_2_53_31_465,"HandleSmsTTSVolSetWindow msg_id = %d")
TRACE_MSG(MMISMS_SETTINGWIN_1015_112_2_18_2_53_32_466,"InitSettingBrowser:is_open is %d")
TRACE_MSG(MMISMS_SETTINGWIN_1992_112_2_18_2_53_34_467,"InitSettingBrowser:is_open is %d")
TRACE_MSG(MMISMS_SETTINGWIN_2277_112_2_18_2_53_34_468,"MMISMS: HandleSettingSmsStatusWinMsg: menu_id = %d")
TRACE_MSG(MMISMS_SETTINGWIN_2686_112_2_18_2_53_35_469,"SetSIMMode:i = %d,alpha_id_present = %d,len = %d,")
TRACE_MSG(MMISMS_SETTINGWIN_3428_112_2_18_2_53_36_470,"MMISMS:ConvertSimToModeName sim_alpha_ptr 0x%x")
TRACE_MSG(MMISMS_SETTINGWIN_3444_112_2_18_2_53_36_471,"MMISMS:ConvertSimToModeName decode alphabet with sim standard is error")
TRACE_MSG(MMISMS_SETTINGWIN_3503_112_2_18_2_53_37_472,"HandleSettingConcatenatedSMSWinMsg msg_id is 0x%x")
TRACE_MSG(MMISMS_SETTINGWIN_3514_112_2_18_2_53_37_473,"HandleSettingConcatenatedSMSWinMsg is_contenated_sms=%d")
TRACE_MSG(MMISMS_SETTINGWIN_3598_112_2_18_2_53_37_474,"HandleSettingAlphabetManagementWinMsg msg_id is 0x%x")
TRACE_MSG(MMISMS_SETTINGWIN_3611_112_2_18_2_53_37_475,"HandleSettingAlphabetManagementWinMsg alpha_management=%d")
TRACE_MSG(MMISMS_TIMERMSG_1355_112_2_18_2_53_40_476,"HandleEditTimerMsgWinMsg: msg_id = %d")
TRACE_MSG(MMISMS_TIMERMSG_1713_112_2_18_2_53_41_477,"HandleEditTimerMsgWinMsg:  MSG_CTL_PENOK event_fast_info_ptr->fre_mode = %d")
TRACE_MSG(MMISMS_TIMERMSG_2085_112_2_18_2_53_41_478,"DisplaySystemDate:date_type %d is error!")
TRACE_MSG(MMISMS_TIMERMSG_2119_112_2_18_2_53_41_479,"DisplaySystemDate:date_type %d is error!")
TRACE_MSG(MMISMS_TIMERMSG_2601_112_2_18_2_53_43_480,"MMISMS:MMISMS_SetIsTimerMode: g_mmisms_global.is_timer_mode =%d, is_timer_mode=%d")
TRACE_MSG(MMISMS_TIMERMSG_2617_112_2_18_2_53_43_481,"MMISMS:MMISMS_GetIsTimerMode: g_mmisms_global.is_timer_mode =%d")
TRACE_MSG(MMISMS_TIMERMSG_2628_112_2_18_2_53_43_482,"MMISMS:MMISMS_SetIsTimerMsg: g_mmisms_global.is_timer_msg =%d, is_timer_msg=%d")
TRACE_MSG(MMISMS_TIMERMSG_2645_112_2_18_2_53_43_483,"MMISMS:MMISMS_GetIsTimerMsg: g_mmisms_global.is_timer_msg =%d")
TRACE_MSG(MMISMS_TIMERMSG_2797_112_2_18_2_53_43_484,"MMISMS: IsSmsOperBusy is_sms_sending=%d, is_sms_deleting=%d, is_sms_saving=%d, is_sms_reading=%d, is_sms_moving=%d, is_sms_exporting=%d, is_sms_copying=%d")
TRACE_MSG(MMISMS_TIMERMSG_2838_112_2_18_2_53_43_485,"MMISMS: MMISMS_TIMERMSG_CheckTimerMSG s_mmisms_timer_sms_timer_id=%d, is_direct=%d")
TRACE_MSG(MMISMS_TIMERMSG_2888_112_2_18_2_53_43_486,"MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback svc_id=%d, svc_type=%d")
TRACE_MSG(MMISMS_TIMERMSG_2924_112_2_18_2_53_43_487,"MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback dual_sys=%d, is_fly_mode=%d, is_order_ok=%d, is_sms_sending=%d, cc=%d, pdp=%d, ussd=%d, ss=%d, cur_oper_status=%d, is_cs_available=%d")
TRACE_MSG(MMISMS_TIMERMSG_2927_112_2_18_2_53_43_488,"MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback dual_sys=%d, is_fly_mode=%d, is_order_ok=%d, is_sms_sending=%d, cc=%d, ussd=%d, ss=%d, cur_oper_status=%d, is_cs_available=%d")
TRACE_MSG(MMISMS_TIMERMSG_2956_112_2_18_2_53_43_489,"MMISMS: MMISMS_TIMERMSG_ContentNotifyCallback not allow send sms now")
TRACE_MSG(MMISMS_TIMERMSG_3010_112_2_18_2_53_43_490,"MMISMS_TIMERMSG_ContentNotifyCallback %d-%d-%d %d:%d")
TRACE_MSG(MMISMS_TIMERMSG_3108_112_2_18_2_53_44_491,"TIMERMSG_InitContentNotifyInfo %d-%d-%d %d:%d")
TRACE_MSG(MMISMS_TIMERMSG_3110_112_2_18_2_53_44_492,"TIMERMSG_InitContentNotifyInfo before update cursor->event_id = %d")
TRACE_MSG(MMISMS_TIMERMSG_3127_112_2_18_2_53_44_493,"TIMERMSG_InitContentNotifyInfo after update cursor->event_id = %d")
TRACE_MSG(MMISMS_TIMERMSG_3205_112_2_18_2_53_44_494,"TIMERMSG_UpdateAlarm %d-%d-%d %d:%d")
TRACE_MSG(MMISMS_TIMERMSG_3207_112_2_18_2_53_44_495,"TIMERMSG_UpdateAlarm before update cursor->event_id = %d")
TRACE_MSG(MMISMS_TIMERMSG_3224_112_2_18_2_53_44_496,"TIMERMSG_UpdateAlarm after update cursor->event_id = %d")
TRACE_MSG(MMISMS_VOICEMAIL_115_112_2_18_2_53_44_497,"MMISMS_HandleSetVoiMessAddrCnf: record_id is error")
TRACE_MSG(MMISMS_VOICEMAIL_236_112_2_18_2_53_44_498,"MMISMS:MMISMS_HandleGetVoiMessAddrCnf get_vm_addr_info.dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMS_VOICEMAIL_239_112_2_18_2_53_44_499,"MMISMS:MMISMS_HandleGetVoiMessAddrCnf:get APP_MN_GET_VMW_ADDR_CNF.The result is %d,record  id is %d")
TRACE_MSG(MMISMS_VOICEMAIL_315_112_2_18_2_53_45_500,"MMISMS: MMISMS_HandleGetVoiMessAddrCnf: get voice addr from sim card")
TRACE_MSG(MMISMS_VOICEMAIL_319_112_2_18_2_53_45_501,"MMISMS: MMISMS_HandleGetVoiMessAddrCnf: THE record_id is wrong")
TRACE_MSG(MMISMS_VOICEMAIL_353_112_2_18_2_53_45_502," the menu id is error")
TRACE_MSG(MMISMS_VOICEMAIL_373_112_2_18_2_53_45_503,"HandleGetVoiMessAddrCnf: get the voice addr for the voicemail box!")
TRACE_MSG(MMISMS_VOICEMAIL_465_112_2_18_2_53_45_504,"MMISMS: MMISMS_HandleGetVoiMessFlagCnf: if there is new voice message = %d")
TRACE_MSG(MMISMS_VOICEMAIL_523_112_2_18_2_53_45_505,"MMISMS:MMISMS_GetVoiceMailAddr mail_len_ptr voice_mail_ptr = PNULL")
TRACE_MSG(MMISMS_VOICEMAIL_621_112_2_18_2_53_45_506,"MMISMS:MMISMS_SetVoiceMailAddr voice_mail_ptr = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_570_112_2_18_2_53_47_507,"MMISMS MMISMS_IsMtForJava: i value is = %d  .iei=%d")
TRACE_MSG(MMISMSAPP_MAIN_586_112_2_18_2_53_47_508,"MMISMS MMISMS_IsMtForJava: There is no java port number!")
TRACE_MSG(MMISMSAPP_MAIN_605_112_2_18_2_53_47_509,"MMISMS MMISMS_IsMtForJava: port_test1 is = %d, tport_test2 is = %d ")
TRACE_MSG(MMISMSAPP_MAIN_618_112_2_18_2_53_47_510,"MMISMS MMISMS_IsMtForJava: MT Ind the des port num is = %d, the src port is = %d ")
TRACE_MSG(MMISMSAPP_MAIN_624_112_2_18_2_53_47_511,"MMISMS MMISMS_IsMtForJava: the port result is = %d")
TRACE_MSG(MMISMSAPP_MAIN_638_112_2_18_2_53_47_512,"[MMISMS]: HandleSIMPlugNotifySMSFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMISMSAPP_MAIN_704_112_2_18_2_53_47_513,"[MMISMS]: HandleSMSNotifyEventCallback dual_sys=%d, notify_event=%d, param=%d")
TRACE_MSG(MMISMSAPP_MAIN_868_112_2_18_2_53_47_514,"MMISMS:HandlePsMsg app_ptr = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_871_112_2_18_2_53_47_515,"MMISMS:HandlePsMsg, msg_id=%x")
TRACE_MSG(MMISMSAPP_MAIN_877_112_2_18_2_53_47_516,"HandlePsMsg APP_MN_SMS_READY_IND")
TRACE_MSG(MMISMSAPP_MAIN_881_112_2_18_2_53_47_517,"SMS: receive APP_MN_SMS_READY_IND")
TRACE_MSG(MMISMSAPP_MAIN_886_112_2_18_2_53_47_518,"MMISMS: receive APP_MN_SIM_NOT_READY_IND")
TRACE_MSG(MMISMSAPP_MAIN_892_112_2_18_2_53_47_519,"MMISMS: receive APP_MN_READ_SMS_CNF")
TRACE_MSG(MMISMSAPP_MAIN_897_112_2_18_2_53_47_520,"MMISMS: receive APP_MN_UPDATE_SMS_STATE_CNF")
TRACE_MSG(MMISMSAPP_MAIN_925_112_2_18_2_53_48_521,"mmi receive APP_MN_SMS_MEM_OVER_FLOW_IND")
TRACE_MSG(MMISMSAPP_MAIN_1309_112_2_18_2_53_48_522,"MMISMS_GetSendPath preferred_connection=%d")
TRACE_MSG(MMISMSAPP_MAIN_1373_112_2_18_2_53_48_523,"MMISMS_GetSavePos is_sim_first is %d")
TRACE_MSG(MMISMSAPP_MAIN_1385_112_2_18_2_53_48_524,"MMISMS_GetSavePos mmi_save_pos is %d")
TRACE_MSG(MMISMSAPP_MAIN_1397_112_2_18_2_53_48_525,"MMISMS_GetSavePos save_pos is %d")
TRACE_MSG(MMISMSAPP_MAIN_1442_112_2_18_2_53_49_526,"MMISMS:HandleSetSMSReceiveCnf sig_ptr = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_1445_112_2_18_2_53_49_527,"MMISMS: HandleSetSMSReceiveCnf cause = %d, s_resend_limit=%d, s_last_send_time=%d")
TRACE_MSG(MMISMSAPP_MAIN_1496_112_2_18_2_53_49_528,"MMISMS: MMISMS_IsSMSReady dual_sys=%d")
TRACE_MSG(MMISMSAPP_MAIN_1499_112_2_18_2_53_49_529," MMISMS: MMISMS_IsSMSReady[%d]=%d")
TRACE_MSG(MMISMSAPP_MAIN_1539_112_2_18_2_53_49_530,"MMISMS: GetRefNumFormNV ref_num = %d")
TRACE_MSG(MMISMSAPP_MAIN_1650_112_2_18_2_53_49_531,"MMISMS:MMISMS_GetOriginNum dual_sys_ptr num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_1685_112_2_18_2_53_49_532,"MMISMS:MMISMS_GetOriginNumByPos dual_sys_ptr num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_1738_112_2_18_2_53_49_533,"MMISMS_SetOperStatus oper_status is %d, g_mmisms_global.operation.oper_status = %d")
TRACE_MSG(MMISMSAPP_MAIN_1753_112_2_18_2_53_49_534,"MMISMS_SetMoveOperStatus oper_status is %d")
TRACE_MSG(MMISMSAPP_MAIN_1768_112_2_18_2_53_49_535,"MMISMS:MMISMS_SetCurOrderId index >= MMISMS_SPLIT_MAX_NUM")
TRACE_MSG(MMISMSAPP_MAIN_1784_112_2_18_2_53_49_536,"MMISMS:MMISMS_SetCurOrderId index >= MMISMS_SPLIT_MAX_NUM")
TRACE_MSG(MMISMSAPP_MAIN_1808_112_2_18_2_53_49_537,"MMISMS_GetCurOperationType cur_type = %d")
TRACE_MSG(MMISMSAPP_MAIN_1825_112_2_18_2_53_49_538,"MMISMS:HandleMemFullInd sig_ptr = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_1832_112_2_18_2_53_49_539,"SMS: HandleMemFullInd mem_status = %d, unread_count = %d")
TRACE_MSG(MMISMSAPP_MAIN_1857_112_2_18_2_53_49_540,"MMISMS: MMISMS_DisplayPrompByErr err_code = %d")
TRACE_MSG(MMISMSAPP_MAIN_1904_112_2_18_2_53_50_541,"MMISMS: MMISMS_DisplayUpdatePrompt(%d)")
TRACE_MSG(MMISMSAPP_MAIN_1987_112_2_18_2_53_50_542,"MMISMS_GetCurrentOperStatus: oper_status = %d")
TRACE_MSG(MMISMSAPP_MAIN_1997_112_2_18_2_53_50_543,"MMISMS_GetCurrentMoveOperStatus: oper_status = %d")
TRACE_MSG(MMISMSAPP_MAIN_2075_112_2_18_2_53_50_544,"MMISMS_ClearOperInfo")
TRACE_MSG(MMISMSAPP_MAIN_2111_112_2_18_2_53_50_545,"MMISMS:MMISMS_SetCurOperationOrderId order_id = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_2160_112_2_18_2_53_50_546,"MMISMS:MMISMS_SetCurOperationOrderId order_id = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_2257_112_2_18_2_53_50_547,"MMISMS_IsOrderOk order_status = 0x%x")
TRACE_MSG(MMISMSAPP_MAIN_2309_112_2_18_2_53_50_548,"MMISMS:StartAppInitSmsList dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_MAIN_2364_112_2_18_2_53_50_549,"MMISMS:MMISMS_SetCurSaveDualSys dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_MAIN_2376_112_2_18_2_53_51_550,"APP_MN_SMS_READY_IND (dual_sys=%d)")
TRACE_MSG(MMISMSAPP_MAIN_2393_112_2_18_2_53_51_551,"MMISMS:MMISMS_SetCurVoiceMailDualSys dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_MAIN_2418_112_2_18_2_53_51_552,"MMISMS:MMISMS_SetCurSendDualSys dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_MAIN_2454_112_2_18_2_53_51_553,"MMISMS:MMISMS_SetCurSaveDualSys dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_MAIN_2499_112_2_18_2_53_51_554,"MMISMS:MMISMS_HandleSimNotReadyInd dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_MAIN_2502_112_2_18_2_53_51_555,"MMISMS: MMISMS_HandleSimNotReadyInd sim_not_ready_ind reason = %d")
TRACE_MSG(MMISMSAPP_MAIN_2536_112_2_18_2_53_51_556,"MMISMS_RefreshSMS: dual_sys=%d")
TRACE_MSG(MMISMSAPP_MAIN_2566_112_2_18_2_53_51_557,"MMISMS_SetIsNeedLoadSaved:is_need_load_saved=%d")
TRACE_MSG(MMISMSAPP_MAIN_2577_112_2_18_2_53_51_558,"MMISMS_GetCurSaveDualSys:is_need_load_saved=%d")
TRACE_MSG(MMISMSAPP_MAIN_2588_112_2_18_2_53_51_559,"MMISMS_SetIsSendFail:is_send_fail=%d")
TRACE_MSG(MMISMSAPP_MAIN_2599_112_2_18_2_53_51_560,"MMISMS_GetIsSendFail:is_send_fail=%d")
TRACE_MSG(MMISMSAPP_MAIN_2617_112_2_18_2_53_51_561,"MMISMS:MMISMS_SetCurOperationOrderId message_content_ptr display_content_ptr = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_2675_112_2_18_2_53_51_562,"MMISMS:MMISMS_GetMaxSimSmsNum dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_MAIN_2689_112_2_18_2_53_51_563,"MMISMS:MMISMS_SetCurSaveDualSys dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_MAIN_2796_112_2_18_2_53_51_564,"MMISMS:MMISMS_SetCurSearchOperationOrderId order_id = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_2813_112_2_18_2_53_51_565,"MMISMS_GetIsNeedReSort s_is_sms_need_re_sort=%d")
TRACE_MSG(MMISMSAPP_MAIN_2824_112_2_18_2_53_51_566,"MMISMS_SetIsNeedReSort is_need_re_sort=%d")
TRACE_MSG(MMISMSAPP_MAIN_2840_112_2_18_2_53_51_567,"MMISMS_GetIsSMSRefresh s_is_sms_refresh[%d]=%d")
TRACE_MSG(MMISMSAPP_MAIN_2856_112_2_18_2_53_51_568,"MMISMS_SetIsSMSRefresh is_sms_refresh[%d]=%d")
TRACE_MSG(MMISMSAPP_MAIN_2958_112_2_18_2_53_52_569,"MMISMS:MMISMS_SetChatCurOperationOrderId order_id = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_2998_112_2_18_2_53_52_570,"MMISMS:MMISMS_SetChatCurOperationOrderId order_id = PNULL")
TRACE_MSG(MMISMSAPP_MAIN_3106_112_2_18_2_53_52_571,"MMISMS: MMISMS_GetAlphabetManagement alpha_management=%d")
TRACE_MSG(MMISMSAPP_MAIN_3328_112_2_18_2_53_52_572,"MMISMS:MMISMS_IKeyBackup sms_total_num = %d, item_data_req_ptr->number = %d")
TRACE_MSG(MMISMSAPP_MAIN_3460_112_2_18_2_53_53_573,"MMISMS: MMISMS_HandleUserReadSMSCnf head_len = %d")
TRACE_MSG(MMISMSAPP_ORDER_522_112_2_18_2_53_54_574,"MMISMS:MMISMS_GetSmsStateBoxtype sms_state_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_555_112_2_18_2_53_54_575,"GetSmsStateBoxtype Error mo_mt_type=%d")
TRACE_MSG(MMISMSAPP_ORDER_569_112_2_18_2_53_54_576,"MMISMS:IsSameLongSmsPart sms1_state_ptr sms2_state_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_600_112_2_18_2_53_54_577,"MMISMS:CompareSmsState sms1_state_ptr sms2_state_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_605_112_2_18_2_53_54_578,"MMISMS:CompareSmsState MMISMS_MT_TYPE != type && MMISMS_MO_TYPE != type")
TRACE_MSG(MMISMSAPP_ORDER_655_112_2_18_2_53_54_579,"InsertNode:new_state_ptr->longsms_max_num = %d,new_state_ptr->longsms_seq_num = %d,new_state_ptr->head_ref = %d,msg_type=%d")
TRACE_MSG(MMISMSAPP_ORDER_883_112_2_18_2_53_55_580,"MMISMS:MMISMS_GetMsgType cur_order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_888_112_2_18_2_53_55_581,"MMISMS: MMISMS_GetMsgType msg_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_929_112_2_18_2_53_55_582,"MMISMS:MMISMS_GetCurMsgIsMarked order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_1113_112_2_18_2_53_56_583,"MMISMS:MMISMS_SetPointedMsgMarked dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_ORDER_1120_112_2_18_2_53_56_584,"MMISMS:MMISMS_SetPointedMsgMarked order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_1169_112_2_18_2_53_56_585,"MMISMS:MMISMS_GetPointedSMSMemInfo storage_ptr record_id_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_1174_112_2_18_2_53_56_586,"MMISMS:MMISMS_GetPointedSMSMemInfo 0 = s_mmisms_order.max_sms_num.total_msg_num")
TRACE_MSG(MMISMSAPP_ORDER_1187_112_2_18_2_53_56_587,"MMISMS: MMISMS_GetPointedSMSMemInfo: order_id is PNULL")
TRACE_MSG(MMISMSAPP_ORDER_1210_112_2_18_2_53_56_588,"MMISMS:MMISMS_GetOrderIdByStorage order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_1216_112_2_18_2_53_56_589,"MMISMS: MMISMS_GetOrderIdByStorage msg_type = %d,storage = %d, record_id= %d")
TRACE_MSG(MMISMSAPP_ORDER_1265_112_2_18_2_53_56_590,"MMISMS: MMISMS_GetOrderIdByStorage: Can't find the order id by storage and record id")
TRACE_MSG(MMISMSAPP_ORDER_1314_112_2_18_2_53_56_591,"MMISMS_DelSMSByAt(%d, %d, %d)")
TRACE_MSG(MMISMSAPP_ORDER_1325_112_2_18_2_53_56_592,"MMISMS:MMISMS_DelSMSByAT status_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_1363_112_2_18_2_53_56_593,"MMISMS:MMISMS_TranslateMNStatus2SFStatus MMISMS_MT_TYPE != type && MMISMS_MO_TYPE != type")
TRACE_MSG(MMISMSAPP_ORDER_1388_112_2_18_2_53_56_594,"SMS: MMISMS_TranslateMNStatus2SFStatus ERROR mn_status = %d")
TRACE_MSG(MMISMSAPP_ORDER_1418_112_2_18_2_53_56_595,"SMS: MMISMS_TranslateMNStatus2SFStatus ERROR mn_status = %d")
TRACE_MSG(MMISMSAPP_ORDER_1537_112_2_18_2_53_56_596,"MMISMS: MMISMS LinkedArrayAllocNode: No array unit to alloc! ")
TRACE_MSG(MMISMSAPP_ORDER_1554_112_2_18_2_53_56_597,"MMISMS: LinkedArrayFreeNode : s_mmisms_linked_order.used_unit_head_ptr is PNULL ")
TRACE_MSG(MMISMSAPP_ORDER_1586_112_2_18_2_53_56_598,"MMISMS: LinkedArrayFreeNode : can't find the node in used link!")
TRACE_MSG(MMISMSAPP_ORDER_1632_112_2_18_2_53_57_599,"MMISMS: MMISMS_GetTobeReadSmsCount ret_val=%d, update_sms_num=%d")
TRACE_MSG(MMISMSAPP_ORDER_1668_112_2_18_2_53_57_600,"MMISMS: MMISMS_GetTobeReadMsgCount ret_val=%d, update_sms_num=%d")
TRACE_MSG(MMISMSAPP_ORDER_1744_112_2_18_2_53_57_601,"MMISMS: MMISMS_GetTobeReadMessageCount ret_val=%d, update_sms_num=%d")
TRACE_MSG(MMISMSAPP_ORDER_1889_112_2_18_2_53_57_602,"MMISMS:MMISMS_InitSmsOrder")
TRACE_MSG(MMISMSAPP_ORDER_2028_112_2_18_2_53_57_603,"MMISMS:MMISMS_InsertSmsToOrder order_info_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_2042_112_2_18_2_53_57_604,"MMISMS_InsertSmsToOrder sim%d sms exceed(%d)!")
TRACE_MSG(MMISMSAPP_ORDER_2049_112_2_18_2_53_57_605,"MMISMS_InsertSmsToOrder (%d)MEMORY FULL, RETURN!")
TRACE_MSG(MMISMSAPP_ORDER_2057_112_2_18_2_53_57_606,"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!")
TRACE_MSG(MMISMSAPP_ORDER_2071_112_2_18_2_53_57_607,"MMISMS: MMISMS_InsertSmsToOrder new_state_ptr->flag.mo_mt_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_2109_112_2_18_2_53_58_608,"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!")
TRACE_MSG(MMISMSAPP_ORDER_2172_112_2_18_2_53_58_609,"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!")
TRACE_MSG(MMISMSAPP_ORDER_2299_112_2_18_2_53_58_610,"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!")
TRACE_MSG(MMISMSAPP_ORDER_2361_112_2_18_2_53_58_611,"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!")
TRACE_MSG(MMISMSAPP_ORDER_2500_112_2_18_2_53_58_612,"MMISMS:MMISMS_GetSmsInfoFromOrder order_id order_info_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_2518_112_2_18_2_53_58_613,"MMISMS:MMISMS_UpdateSmsInfoInOrder old_order_id new_order_info_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_2575_112_2_18_2_53_58_614,"MMISMS_GetBoxTotalNumberFromOrder: box_type = %d")
TRACE_MSG(MMISMSAPP_ORDER_2781_112_2_18_2_53_59_615,"MMISMS:DeleteNode sms_state_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_2785_112_2_18_2_53_59_616,"DeleteNode: sms_state_ptr->longsms_max_num = %d, sms_state_ptr->longsms_seq_num = %d,sms_state_ptr->head_ref = %d,msg_type=%d")
TRACE_MSG(MMISMSAPP_ORDER_2789_112_2_18_2_53_59_617,"MMISMS: DeleteNode: delete_type_head_ptr is PNULL")
TRACE_MSG(MMISMSAPP_ORDER_2906_112_2_18_2_53_59_618,"MMISMS: DeleteNode :can't find the sms ptr in long sms link")
TRACE_MSG(MMISMSAPP_ORDER_2913_112_2_18_2_53_59_619,"MMISMS DeleteNode: there is no sms have the same ref with the to delete sms1_state_ptr")
TRACE_MSG(MMISMSAPP_ORDER_2966_112_2_18_2_53_59_620,"HandleLongSMSOrderMsgNum is_to_be_read=%d,is_insert=%d")
TRACE_MSG(MMISMSAPP_ORDER_2969_112_2_18_2_53_59_621,"MMISMS:HandleLongSMSOrderMsgNum total_msg_num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_3017_112_2_18_2_53_59_622,"HandleOrderMsgNum is_to_be_read=%d,is_insert=%d")
TRACE_MSG(MMISMSAPP_ORDER_3020_112_2_18_2_53_59_623,"MMISMS:HandleOrderMsgNum total_msg_num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_3105_112_2_18_2_54_0_624,"MMISMS: MMISMS_LoadSmsListFromOrder the count of list box is %d")
TRACE_MSG(MMISMSAPP_ORDER_3223_112_2_18_2_54_0_625,"MMISMS: MMISMS_ListSmsFromOrder the undownloaded sms count is %d")
TRACE_MSG(MMISMSAPP_ORDER_3224_112_2_18_2_54_0_626,"MMISMS: MMISMS_ListSmsFromOrder the unreaded sms count is %d")
TRACE_MSG(MMISMSAPP_ORDER_3225_112_2_18_2_54_0_627,"MMISMS: MMISMS_ListSmsFromOrder the readed sms count is %d")
TRACE_MSG(MMISMSAPP_ORDER_3276_112_2_18_2_54_0_628,"MMISMS: MMISMS_LoadSmsListFromOrder: the box_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_3429_112_2_18_2_54_0_629,"subject.length is %d")
TRACE_MSG(MMISMSAPP_ORDER_3689_112_2_18_2_54_1_630,"subject.length is %d")
TRACE_MSG(MMISMSAPP_ORDER_3882_112_2_18_2_54_1_631,"MMISMS:FindNameAndStoreInfoByNum bcd_num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_3885_112_2_18_2_54_1_632,"MMISMS: FindNameAndStoreInfoByNum num_len = %d")
TRACE_MSG(MMISMSAPP_ORDER_3901_112_2_18_2_54_1_633,"MMISMS: FindNameAndStoreInfoByNum num_len = %d, pb_num.number_len = %d")
TRACE_MSG(MMISMSAPP_ORDER_4219_112_2_18_2_54_2_634,"MMISMS: list_index is %d ")
TRACE_MSG(MMISMSAPP_ORDER_4223_112_2_18_2_54_2_635,"MMISMS: s_mmisms_order.total_undownloaded_num is %d")
TRACE_MSG(MMISMSAPP_ORDER_4224_112_2_18_2_54_2_636,"MMISMS: s_mmisms_order.to_be_read_num is %d")
TRACE_MSG(MMISMSAPP_ORDER_4289_112_2_18_2_54_2_637,"MMISMS: MMISMS_LoadSmsListFromOrder: the box_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_4293_112_2_18_2_54_2_638,"head_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_4330_112_2_18_2_54_2_639,"MMISMS:cur_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_4344_112_2_18_2_54_2_640,"MMISMS: list_index is %d ")
TRACE_MSG(MMISMSAPP_ORDER_4348_112_2_18_2_54_2_641,"head_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_4372_112_2_18_2_54_2_642,"MMISMS:cur_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_4390_112_2_18_2_54_2_643,"MMISMS:MMISMS_InsertSmsToUnhandleOrder new_state_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_5088_112_2_18_2_54_3_644,"MMISMS:MMISMS_GetPointedPosRecordID order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_5210_112_2_18_2_54_4_645,"HandleMaxSimSMSNum max_sim_num=%d exceed limit!")
TRACE_MSG(MMISMSAPP_ORDER_5259_112_2_18_2_54_4_646,"mmisms:MMISMS_OrderInit max_sms_num=%d")
TRACE_MSG(MMISMSAPP_ORDER_5296_112_2_18_2_54_4_647,"MMISMS:CompareStateForSort sms1_state_ptr sms2_state_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_5422_112_2_18_2_54_4_648,"MMISMS:SortOrderList head = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_5429_112_2_18_2_54_4_649,"MMISMS:SortOrderList temp_arry alloc fail")
TRACE_MSG(MMISMSAPP_ORDER_5621_112_2_18_2_54_5_650,"MMISMS:MMISMS_SetCurMsgLocked order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_5759_112_2_18_2_54_5_651,"MMISMS:MMISMS_CurMsgIsLocked order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_5812_112_2_18_2_54_5_652,"MMISMS:MMISMS_CurMsgRecordId order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_6104_112_2_18_2_54_5_653,"MMISMS_UpdateUnreadOrder:is_need_update_order=%d")
TRACE_MSG(MMISMSAPP_ORDER_6328_112_2_18_2_54_6_654,"MMISMS_GetSmsBoxHeaderOrderId delete_sms_num=%d")
TRACE_MSG(MMISMSAPP_ORDER_6571_112_2_18_2_54_6_655,"MMISMS:GetOrderLinkOriginCard dual_sys = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_6651_112_2_18_2_54_7_656,"MMISMS:MMISMS_GetSMSUsedAndMax max_num_ptr used_num_ptr sms_ori_used_num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_6669_112_2_18_2_54_7_657,"SMS: MMISMS_GetSMSUsedAndMax dual_sys=%d, is_nv = %d, max_num = %d, used_num = %d,ori=%d")
TRACE_MSG(MMISMSAPP_ORDER_6690_112_2_18_2_54_7_658,"MMISMS_SetPointedMsgNumMarked:index is %d,len is %d")
TRACE_MSG(MMISMSAPP_ORDER_6704_112_2_18_2_54_7_659,"fengming:sender[%d] is %d,number[%d] is %d")
TRACE_MSG(MMISMSAPP_ORDER_6709_112_2_18_2_54_7_660,"MMISMS_SetPointedMsgNumMarked:msg_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_6803_112_2_18_2_54_7_661,"MMISMS_SetPointedMsgNumMarked:index is %d,len is %d")
TRACE_MSG(MMISMSAPP_ORDER_6817_112_2_18_2_54_7_662,"MMISMS_SetPointedMsgNumMarked:msg_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_6918_112_2_18_2_54_7_663,"fengming:sender[%d] is %d")
TRACE_MSG(MMISMSAPP_ORDER_6971_112_2_18_2_54_7_664,"fengming:number[%d] is %d")
TRACE_MSG(MMISMSAPP_ORDER_6974_112_2_18_2_54_7_665,"MMISMS:GetNumber len is %d,msg_type is %d,mo_mt_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_7100_112_2_18_2_54_7_666,"MMISMS:MMISMS_CurMsgFolderType order_id = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_7304_112_2_18_2_54_8_667,"MMISMS_GetSmsBoxHeaderOrderId move_sms_num=%d")
TRACE_MSG(MMISMSAPP_ORDER_7354_112_2_18_2_54_8_668,"MMISMS_SetCurMsgFolderType alloc failed ")
TRACE_MSG(MMISMSAPP_ORDER_7371_112_2_18_2_54_8_669,"MMISMS_SetCurMsgFolderType new_order_info_ptr = 0x%x,record_id is %d")
TRACE_MSG(MMISMSAPP_ORDER_7404_112_2_18_2_54_8_670,"MMISMS_SetCurMsgFolderType folder_type = %d,record_id = %d")
TRACE_MSG(MMISMSAPP_ORDER_7468_112_2_18_2_54_8_671,"MMISMS:MMISMS_CompareSearchSmsState sms1_state_ptr sms2_state_ptr = PNULL")
TRACE_MSG(MMISMSAPP_ORDER_8283_112_2_18_2_54_10_672,"MMISMS_GetMarkedStorageMsgNumByDualsys:box_type = %d,storage = %d,dual_sys = %d")
TRACE_MSG(MMISMSAPP_ORDER_8480_112_2_18_2_54_10_673,"MMISMS_GetMarkedStorageMsgNumByDualsys:mark_num = %d")
TRACE_MSG(MMISMSAPP_ORDER_8497_112_2_18_2_54_10_674,"MMISMS_GetMarkedStorageMsgNum:box_type = %d,storage = %d")
TRACE_MSG(MMISMSAPP_ORDER_8654_112_2_18_2_54_11_675,"MMISMS_GetMarkedStorageMsgNum:mark_num = %d")
TRACE_MSG(MMISMSAPP_ORDER_8671_112_2_18_2_54_11_676,"MMISMS_GetMarkedMsgNumByType:box_type = %d,msg_type = %d")
TRACE_MSG(MMISMSAPP_ORDER_8788_112_2_18_2_54_11_677,"MMISMS_GetMarkedMsgNumByType:mark_num = %d")
TRACE_MSG(MMISMSAPP_ORDER_9104_112_2_18_2_54_12_678,"MMISMS_SetCurChatMsgMarked: index = %d")
TRACE_MSG(MMISMSAPP_ORDER_9146_112_2_18_2_54_12_679,"MMISMS_SetCurChatMsgMarked: index = %d")
TRACE_MSG(MMISMSAPP_ORDER_9215_112_2_18_2_54_12_680,"MMISMS_SetCurChatMsgMarked: index = %d")
TRACE_MSG(MMISMSAPP_ORDER_9262_112_2_18_2_54_12_681,"MMISMS_SetCurChatMsgMarked: index = %d")
TRACE_MSG(MMISMSAPP_ORDER_9304_112_2_18_2_54_12_682,"MMISMS_SetCurChatMsgMarked: index = %d")
TRACE_MSG(MMISMSAPP_ORDER_9375_112_2_18_2_54_12_683,"InsertNodeToChatOrder: new_state_ptr is PNULL!")
TRACE_MSG(MMISMSAPP_ORDER_9386_112_2_18_2_54_12_684,"InsertNodeToChatOrder: mo_mt_type is %d,folder_type is %d,msg_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_9666_112_2_18_2_54_13_685,"InsertSameNumNodeToChatOrderByTime: long sms not find in order!!")
TRACE_MSG(MMISMSAPP_ORDER_9748_112_2_18_2_54_13_686,"InsertSameNumNodeToChatOrderByTime: long sms not find in order!!")
TRACE_MSG(MMISMSAPP_ORDER_9888_112_2_18_2_54_14_687,"DeleteNodeFromChatOrder: delete_state_ptr is PNULL!")
TRACE_MSG(MMISMSAPP_ORDER_9898_112_2_18_2_54_14_688,"DeleteNodeFromChatOrder: mo_mt_type is %d,folder_type is %d,msg_type is %d")
TRACE_MSG(MMISMSAPP_ORDER_9976_112_2_18_2_54_14_689,"[jixin]:same num")
TRACE_MSG(MMISMSAPP_ORDER_9982_112_2_18_2_54_14_690,"[jixin]:diff num")
TRACE_MSG(MMISMSAPP_ORDER_10012_112_2_18_2_54_14_691,"DeleteChatNode:input para is PNULL!!")
TRACE_MSG(MMISMSAPP_ORDER_10347_112_2_18_2_54_15_692,"MMISMS: DeleteChatNode :can't find the sms ptr in long sms link")
TRACE_MSG(MMISMSAPP_ORDER_10360_112_2_18_2_54_15_693,"MMISMS DeleteChatNode: there is no sms have the same ref with the to delete sms1_state_ptr")
TRACE_MSG(MMISMSAPP_ORDER_10365_112_2_18_2_54_15_694,"MMISMS: DeleteChatNode total_num = %d")
TRACE_MSG(MMISMSAPP_ORDER_10657_112_2_18_2_54_15_695,"GetChatNodeTotalUnreadNum:unread_num = %d")
TRACE_MSG(MMISMSAPP_ORDER_10669_112_2_18_2_54_15_696,"MMISMS_GetChatNodeTotalUnreadNum:unread_num = %d")
TRACE_MSG(MMISMSAPP_ORDER_10686_112_2_18_2_54_15_697,"MMISMS_SetCurChatMsgMarked: index = %d,is_marked = %d")
TRACE_MSG(MMISMSAPP_ORDER_10731_112_2_18_2_54_15_698,"MMISMS_SetCurChatMsgMarked: index = %d,is_marked = %d")
TRACE_MSG(MMISMSAPP_ORDER_10847_112_2_18_2_54_16_699,"GetChatOrderLinkDeleteAvailNum: delete_sms_num = %d")
TRACE_MSG(MMISMSAPP_ORDER_10876_112_2_18_2_54_16_700,"MMISMS_CurChatMsgUnlockedNum: delete_num = %d")
TRACE_MSG(MMISMSAPP_ORDER_10894_112_2_18_2_54_16_701,"head_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_10935_112_2_18_2_54_16_702,"head_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_11038_112_2_18_2_54_16_703,"head_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_11194_112_2_18_2_54_17_704,"mmisms : return_order_id is PNULL")
TRACE_MSG(MMISMSAPP_ORDER_11261_112_2_18_2_54_17_705,"MMISMS: MMISMS_GetUpdateSMSNum update_sms_num=%d")
TRACE_MSG(MMISMSAPP_ORDER_11279_112_2_18_2_54_17_706,"head_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_11368_112_2_18_2_54_17_707,"HandleOkMsgInSmsList index is %d")
TRACE_MSG(MMISMSAPP_ORDER_11464_112_2_18_2_54_17_708,"MMISMS: sms_record_id=%d, mo_mt_type=%d")
TRACE_MSG(MMISMSAPP_ORDER_11499_112_2_18_2_54_17_709,"head_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_11518_112_2_18_2_54_17_710,"MMISMS:cur_order_id is 0x%x")
TRACE_MSG(MMISMSAPP_ORDER_11901_112_2_18_2_54_18_711,"MMIAPISMS_SetMatchItemContent name_str.wstr_ptr alloc fail")
TRACE_MSG(MMISMSAPP_ORDER_11940_112_2_18_2_54_18_712,"MMIAPISMS_SetMatchItemContent2 name_str.wstr_ptr alloc fail")
TRACE_MSG(MMISMSAPP_ORDER_11989_112_2_18_2_54_18_713,"MMISMS: MMISMS_ReplaceSms pid_e=%d")
TRACE_MSG(MMISMSAPP_ORDER_12116_112_2_18_2_54_19_714,"MMISMS: MMISMS_ReplaceSms is_replace=%d")
TRACE_MSG(MMISMSAPP_ORDER_12138_112_2_18_2_54_19_715,"MMISMS: MMISMS_FullOrder Order Not Ready!")
TRACE_MSG(MMISMSAPP_ORDER_12147_112_2_18_2_54_19_716,"MMISMS: MMISMS_FullOrder No Memory!")
TRACE_MSG(MMISMSAPP_ORDER_12206_112_2_18_2_54_19_717,"MMISMS:MMISMS_FindMsgForSmsBackup index = %d")
TRACE_MSG(MMISMSAPP_ORDER_12248_112_2_18_2_54_19_718,"MMISMS: MMISMS_IsExistSameSms 0 == backup_data_ptr->time")
TRACE_MSG(MMISMSAPP_ORDER_12265_112_2_18_2_54_19_719,"MMISMS: MMISMS_IsExistSameSms is_exist = %d, backup_data_ptr->time = %d, backup_data_ptr->record_id = %d")
TRACE_MSG(MMISMSAPP_ORDER_12439_112_2_18_2_54_19_720,"MMISMS: MMISMS_LoadSmsListFromOrder the count of list box is %d")
TRACE_MSG(MMISMSAPP_WINTAB_4082_112_2_18_2_54_28_721,"MMISMS:SetSmsMemState2String item_data index = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_4194_112_2_18_2_54_28_722,"MMISMS:SetSmsMemState2String item_data index = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_4354_112_2_18_2_54_29_723,"MMISMS: MMISMS HandleSmsNewMsgMenuWinMsg: menu_id = %d")
TRACE_MSG(MMISMSAPP_WINTAB_4445_112_2_18_2_54_29_724,"SetNumState2String:chat total_num = %d,unread_num = %d")
TRACE_MSG(MMISMSAPP_WINTAB_4465_112_2_18_2_54_29_725,"SetNumState2String:chat total_num = %d,unread_num = %d")
TRACE_MSG(MMISMSAPP_WINTAB_4475_112_2_18_2_54_29_726,"SetNumState2String:chat total_num = %d,unread_num = %d")
TRACE_MSG(MMISMSAPP_WINTAB_4485_112_2_18_2_54_29_727,"SetNumState2String:chat total_num = %d,unread_num = %d")
TRACE_MSG(MMISMSAPP_WINTAB_4495_112_2_18_2_54_29_728,"SetNumState2String:chat total_num = %d,unread_num = %d")
TRACE_MSG(MMISMSAPP_WINTAB_4848_112_2_18_2_54_30_729,"HandleSmsMainMenuOptWinMsg menu_id = %d")
TRACE_MSG(MMISMSAPP_WINTAB_5005_112_2_18_2_54_30_730,"MMISMS:OpenVMNumberWin dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_WINTAB_5014_112_2_18_2_54_30_731,"OpenVMNumberWin: MNSMS_GetVoiMessAddr()")
TRACE_MSG(MMISMSAPP_WINTAB_5036_112_2_18_2_54_30_732,"MMISMS:OpenVMNumberWin dual_sys >=  MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISMSAPP_WINTAB_5060_112_2_18_2_54_30_733,"OpenVMNumberWin: MNSMS_GetVoiMessAddr()")
TRACE_MSG(MMISMSAPP_WINTAB_5478_112_2_18_2_54_31_734,"MMISMS: HandleEditMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_5747_112_2_18_2_54_31_735,"MMISMS: HandleEditMenuWinMsg call MMIAPIPB_OpenNumberList")
TRACE_MSG(MMISMSAPP_WINTAB_6052_112_2_18_2_54_32_736,"MMISMS:IsSCAddrValid num_str_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_6096_112_2_18_2_54_32_737,"MMISMS: HandleInputSignatureWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_6150_112_2_18_2_54_32_738,"MMISMS: HandleInputSCWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_6266_112_2_18_2_54_33_739,"MMISMS_HandleSendWaitWinMsg: Send cancel! dual_sys = %d")
TRACE_MSG(MMISMSAPP_WINTAB_6278_112_2_18_2_54_33_740,"MMISMS:MMISMS_HandleSendWaitWinMsg send_info_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_6361_112_2_18_2_54_33_741,"MMISMS:MMISMS_HandleSendWaitWinMsg send_info_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_6364_112_2_18_2_54_33_742,"MMISMS_HandleSendWaitWinMsg:Stop g_sms_retry_timer  %d")
TRACE_MSG(MMISMSAPP_WINTAB_6455_112_2_18_2_54_33_743,"MMISMS_HandleWaitWinMsg:insert unhandle sms")
TRACE_MSG(MMISMSAPP_WINTAB_6858_112_2_18_2_54_34_744,"MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_6925_112_2_18_2_54_34_745,"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_7377_112_2_18_2_54_35_746,"MMISMS: HandleSendSuccBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_7421_112_2_18_2_54_35_747,"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATAa need_item_data_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_7757_112_2_18_2_54_36_748,"MMISMS: HandleSendFailBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_7802_112_2_18_2_54_36_749,"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_8132_112_2_18_2_54_37_750,"MMISMS: HandleDraftBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_8174_112_2_18_2_54_37_751,"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_8504_112_2_18_2_54_38_752,"MMISMS: MMISMS_BoxEnableGrayed s_marked_sms_num = %d, sim_ok_num=%d")
TRACE_MSG(MMISMSAPP_WINTAB_8569_112_2_18_2_54_38_753,"MMISMS: MMISMS_BoxEnableGrayed s_marked_sms_num = %d")
TRACE_MSG(MMISMSAPP_WINTAB_9592_112_2_18_2_54_40_754,"HandleOkMsgInSmsList index is %d")
TRACE_MSG(MMISMSAPP_WINTAB_9634_112_2_18_2_54_41_755,"MMISMS:HandleOkMsgInSmsList order_id = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_9682_112_2_18_2_54_41_756,"MMISMS:HandleOkMsgInSmsList order_id = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_9711_112_2_18_2_54_41_757,"MMISMS:HandleOkMsgInSmsList order_id = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_10967_112_2_18_2_54_43_758,"MMISMS: HandleShowSMSWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_11332_112_2_18_2_54_44_759,"sms pen ok has handled")
TRACE_MSG(MMISMSAPP_WINTAB_11356_112_2_18_2_54_44_760,"MMISMS:HandleShowSMSWinMsg focus_num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_11374_112_2_18_2_54_44_761,"MMISMS:HandleShowSMSWinMsg focus_url_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_11386_112_2_18_2_54_44_762,"MMISMS:HandleShowSMSWinMsg focus_email_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_11828_112_2_18_2_54_45_763,"MMISMS: HandleShowSMSWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_12146_112_2_18_2_54_46_764,"sms pen ok has handled")
TRACE_MSG(MMISMSAPP_WINTAB_12170_112_2_18_2_54_46_765,"MMISMS:HandleShowSMSWinMsg focus_num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_12188_112_2_18_2_54_46_766,"MMISMS:HandleShowSMSWinMsg focus_url_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_12200_112_2_18_2_54_46_767,"MMISMS:HandleShowSMSWinMsg focus_email_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_12603_112_2_18_2_54_47_768,"MMIMMS: HandleSMSExtractNumberPopWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_12844_112_2_18_2_54_47_769,"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_13006_112_2_18_2_54_48_770,"MMIMMS: HandleSMSExtractEmailWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_13196_112_2_18_2_54_48_771,"SMS: HandleSmsMtboxMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_13701_112_2_18_2_54_49_772,"MMISMS:Mark num all , num is %d")
TRACE_MSG(MMISMSAPP_WINTAB_13719_112_2_18_2_54_49_773,"MMISMS:Mark num all , num is %d")
TRACE_MSG(MMISMSAPP_WINTAB_14911_112_2_18_2_54_52_774,"MMISMS:Mark num all , num is %d")
TRACE_MSG(MMISMSAPP_WINTAB_15277_112_2_18_2_54_52_775," the menu id is error")
TRACE_MSG(MMISMSAPP_WINTAB_15359_112_2_18_2_54_52_776," the menu id is error")
TRACE_MSG(MMISMSAPP_WINTAB_15447_112_2_18_2_54_53_777," the menu id is error")
TRACE_MSG(MMISMSAPP_WINTAB_15566_112_2_18_2_54_53_778," the menu id is error")
TRACE_MSG(MMISMSAPP_WINTAB_15594_112_2_18_2_54_53_779," the menu id is error")
TRACE_MSG(MMISMSAPP_WINTAB_15659_112_2_18_2_54_53_780," the menu id is error")
TRACE_MSG(MMISMSAPP_WINTAB_15746_112_2_18_2_54_53_781," the menu id is error")
TRACE_MSG(MMISMSAPP_WINTAB_15829_112_2_18_2_54_53_782," the menu id is error")
TRACE_MSG(MMISMSAPP_WINTAB_15849_112_2_18_2_54_54_783,"MMISMS:MMISMS_OpenCallVMWin dual_sys >= MMI_DUAL_SYS_MAX || vmn_type >= MMISMS_VMN_TYPE_MAX")
TRACE_MSG(MMISMSAPP_WINTAB_16276_112_2_18_2_54_54_784,"HandleEntryDetailPdaWinMsg list_item_ptr %d")
TRACE_MSG(MMISMSAPP_WINTAB_16552_112_2_18_2_54_55_785,"MMISMS:HandleWordListForCcWinMsg cur_list_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_16595_112_2_18_2_54_55_786,"MMISMS:HandleWordListForCcWinMsg cur_list_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_16706_112_2_18_2_54_55_787,"MMISMS:HandleSetPresetSms text_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_16749_112_2_18_2_54_55_788,"MMISMS:HandleSetPresetSms text_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_17251_112_2_18_2_54_56_789,"MMISMS:AppendListItem string_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_17320_112_2_18_2_54_56_790,"HandleSendNumOptWinMsg, send_type = %d, msg_id = %x")
TRACE_MSG(MMISMSAPP_WINTAB_17616_112_2_18_2_54_57_791,"MMISMS: GetNumberFromPb msg_id = 0x%x, count = %d")
TRACE_MSG(MMISMSAPP_WINTAB_17670_112_2_18_2_54_57_792,"MMISMS: MMISMS_GetNumberFromCalllog msg_id = 0x%x, count = %d")
TRACE_MSG(MMISMSAPP_WINTAB_17818_112_2_18_2_54_57_793,"MMISMS_HandleSendOperation: win_id = 0x%x,is_re_send = %d")
TRACE_MSG(MMISMSAPP_WINTAB_17837_112_2_18_2_54_58_794,"MMISMS_HandleSendOperation: timer sms")
TRACE_MSG(MMISMSAPP_WINTAB_17940_112_2_18_2_54_58_795,"MMISMS: HandleSendPromptOptMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_18062_112_2_18_2_54_58_796,"MMISMS:HandleSelectWordWinMsg cur_list_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_18131_112_2_18_2_54_58_797,"MMISMS:MMISMS_GetListCurrentItemContent str_len_ptr wstr_ptr item_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_18431_112_2_18_2_54_59_798,"MMISMS: HandleSendSuccMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_19422_112_2_18_2_55_1_799,"MMISMS_HandleNewMsgWin %d")
TRACE_MSG(MMISMSAPP_WINTAB_19519_112_2_18_2_55_1_800,"mmisms: order_id is PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_19673_112_2_18_2_55_1_801,"mmisms: order_id is PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_19769_112_2_18_2_55_1_802,"MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected")
TRACE_MSG(MMISMSAPP_WINTAB_19809_112_2_18_2_55_1_803,"SMS: PlayNewSMSRing vt mode")
TRACE_MSG(MMISMSAPP_WINTAB_19817_112_2_18_2_55_1_804,"MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected")
TRACE_MSG(MMISMSAPP_WINTAB_19994_112_2_18_2_55_2_805,"MMISMS: HandleMsgBoxWinMsg msg_id = %d")
TRACE_MSG(MMISMSAPP_WINTAB_20070_112_2_18_2_55_2_806,"RingCallBack ENTER RingCallBack")
TRACE_MSG(MMISMSAPP_WINTAB_20102_112_2_18_2_55_2_807,"PlaySMSRing ring_type = %d")
TRACE_MSG(MMISMSAPP_WINTAB_20136_112_2_18_2_55_2_808,"mary MMISET_MSG_VIBRA_AND_RING")
TRACE_MSG(MMISMSAPP_WINTAB_20172_112_2_18_2_55_2_809,"sim sms num error max_num = %d,used_num = %d ")
TRACE_MSG(MMISMSAPP_WINTAB_20183_112_2_18_2_55_2_810,"nv sms num error max_num = %d,used_num = %d ")
TRACE_MSG(MMISMSAPP_WINTAB_20248_112_2_18_2_55_2_811,"MMISMS: MMISMS_GetSaveSmsNumFlag g_save_sms_num_flag = %d")
TRACE_MSG(MMISMSAPP_WINTAB_20660_112_2_18_2_55_3_812,"HandleSettingSavePosWinMsg msg_id is 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_20671_112_2_18_2_55_3_813,"HandleSettingSavePosWinMsg pos is %d")
TRACE_MSG(MMISMSAPP_WINTAB_20994_112_2_18_2_55_4_814,"MMISMS_ShowMemFullPrompt:dual_sys=%d,mem_status=%d")
TRACE_MSG(MMISMSAPP_WINTAB_22152_112_2_18_2_55_6_815,"SetNumState2String:chat total_num = %d,unread_num = %d")
TRACE_MSG(MMISMSAPP_WINTAB_22365_112_2_18_2_55_7_816,"MMISMS:GetExtractURLOrST gbstr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_22370_112_2_18_2_55_7_817,"MMISMS:GetExtractURLOrST index >= s_extract.total_num")
TRACE_MSG(MMISMSAPP_WINTAB_22429_112_2_18_2_55_7_818,"index = %d, gbstr_len = %d")
TRACE_MSG(MMISMSAPP_WINTAB_22432_112_2_18_2_55_7_819,"extract st addr is %x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(MMISMSAPP_WINTAB_22519_112_2_18_2_55_7_820,"MMISMS_HandleNewVcardMsgWin name_len=%d")
TRACE_MSG(MMISMSAPP_WINTAB_22651_112_2_18_2_55_7_821,"MMISMS_HandleNewVcalendarMsgWin name_len=%d")
TRACE_MSG(MMISMSAPP_WINTAB_22888_112_2_18_2_55_8_822," sms num error max_num = %d,used_num = %d ")
TRACE_MSG(MMISMSAPP_WINTAB_22917_112_2_18_2_55_8_823,"MMISMS StartMenuWinTimer: the key timer has started!")
TRACE_MSG(MMISMSAPP_WINTAB_22934_112_2_18_2_55_8_824,"MMISMS StopMenuWinTimer: the key timer has stop!")
TRACE_MSG(MMISMSAPP_WINTAB_23040_112_2_18_2_55_8_825,"MMISMS_IsInSMSWin :win_id is %d")
TRACE_MSG(MMISMSAPP_WINTAB_23072_112_2_18_2_55_8_826,"mmisms_getpushsmsbrowserinfo: open_flag is %d")
TRACE_MSG(MMISMSAPP_WINTAB_23090_112_2_18_2_55_8_827,"mmisms_setpushsmsbrowserinfo: open_flag is %d,flag is %d")
TRACE_MSG(MMISMSAPP_WINTAB_23172_112_2_18_2_55_8_828,"HandleDelQueryWinMsg: cancel del same num")
TRACE_MSG(MMISMSAPP_WINTAB_23431_112_2_18_2_55_9_829,"SMS: HandleSMSClass0OptionPopWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_23660_112_2_18_2_55_9_830,"MMISMS: MMISMS_UpdateMMSMarkedFlag s_marked_sms_num = %d")
TRACE_MSG(MMISMSAPP_WINTAB_23687_112_2_18_2_55_9_831,"MMISMS: HandleSecurityBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_23731_112_2_18_2_55_9_832,"MMISMS:HandleSecurityBoxChildWinMsg need_item_data_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_24333_112_2_18_2_55_11_833,"move cancel")
TRACE_MSG(MMISMSAPP_WINTAB_24813_112_2_18_2_55_12_834,"MMISMS:IsCorrectSecurityboxPwd pwd_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_24868_112_2_18_2_55_12_835,"MMISMS_GetMoveSingleFlag:s_is_move_single = %d")
TRACE_MSG(MMISMSAPP_WINTAB_24883_112_2_18_2_55_12_836,"MMISMS_SetMoveSingleFlag:s_is_move_single = %d")
TRACE_MSG(MMISMSAPP_WINTAB_24950_112_2_18_2_55_12_837,"MMISMS_SetExportBoxType: export_box_type is %d")
TRACE_MSG(MMISMSAPP_WINTAB_24960_112_2_18_2_55_12_838,"MMISMS_GetExportBoxType: s_export_type is %d")
TRACE_MSG(MMISMSAPP_WINTAB_25115_112_2_18_2_55_12_839,"MMISMS_SetMarkedMsgNum: marked_num is %d")
TRACE_MSG(MMISMSAPP_WINTAB_25126_112_2_18_2_55_12_840,"MMISMS_GetMarkedMsgNum: marked_num is %d")
TRACE_MSG(MMISMSAPP_WINTAB_25180_112_2_18_2_55_12_841,"[MMISMS ATC] MMIAPISMS_InsertMessage result %d")
TRACE_MSG(MMISMSAPP_WINTAB_25217_112_2_18_2_55_12_842,"MMISMS_GetWinFont: font is %d")
TRACE_MSG(MMISMSAPP_WINTAB_26001_112_2_18_2_55_14_843,"HandleEditSmsWinMsg alloc mem fail")
TRACE_MSG(MMISMSAPP_WINTAB_26834_112_2_18_2_55_16_844,"SetIsSelectPB: s_is_add_contact is %d")
TRACE_MSG(MMISMSAPP_WINTAB_26844_112_2_18_2_55_16_845,"GetIsAddContact: s_is_add_contact is %d")
TRACE_MSG(MMISMSAPP_WINTAB_27455_112_2_18_2_55_17_846,"mmisms date_time_str PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_27500_112_2_18_2_55_17_847,"MMISMS: HandleChatBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_27560_112_2_18_2_55_17_848,"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_28309_112_2_18_2_55_19_849,"MMISMS: HandleShowChatSmsWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_28546_112_2_18_2_55_19_850,"mmisms alloc failed")
TRACE_MSG(MMISMSAPP_WINTAB_28604_112_2_18_2_55_19_851,"sms file info alloc error")
TRACE_MSG(MMISMSAPP_WINTAB_28671_112_2_18_2_55_19_852,"mmisms alloc failed")
TRACE_MSG(MMISMSAPP_WINTAB_28790_112_2_18_2_55_20_853,"sms file info alloc error")
TRACE_MSG(MMISMSAPP_WINTAB_28813_112_2_18_2_55_20_854,"mmisms alloc failed")
TRACE_MSG(MMISMSAPP_WINTAB_28872_112_2_18_2_55_20_855,"MMISMS:ShowChatSmsWin focus_num_ptr = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_29154_112_2_18_2_55_20_856,"SetMMSChatItemParam:PNULL == order_id")
TRACE_MSG(MMISMSAPP_WINTAB_29159_112_2_18_2_55_20_857,"SetMMSChatItemParam:MMISMS_MO_DRAFT")
TRACE_MSG(MMISMSAPP_WINTAB_29167_112_2_18_2_55_20_858,"SetMMSChatItemParam:not find index!!")
TRACE_MSG(MMISMSAPP_WINTAB_29183_112_2_18_2_55_20_859,"SetMMSChatItemParam: else")
TRACE_MSG(MMISMSAPP_WINTAB_29530_112_2_18_2_55_21_860,"SetChatItemParam:not find index!!")
TRACE_MSG(MMISMSAPP_WINTAB_30054_112_2_18_2_55_22_861,"SetChatWinIndex: index is %d,is_active is %d")
TRACE_MSG(MMISMSAPP_WINTAB_30107_112_2_18_2_55_22_862,"GetChatWinIndex: index is %d,result is %d")
TRACE_MSG(MMISMSAPP_WINTAB_30175_112_2_18_2_55_22_863,"SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_30210_112_2_18_2_55_22_864,"MMISMS:  HandleSmsChatMenuOptWinMsg = %d")
TRACE_MSG(MMISMSAPP_WINTAB_30331_112_2_18_2_55_23_865,"OperateAfterDelSingleChatItem:g_mmisms_global.del_index is %d")
TRACE_MSG(MMISMSAPP_WINTAB_30347_112_2_18_2_55_23_866,"mmisms chat not focus in chat item")
TRACE_MSG(MMISMSAPP_WINTAB_30381_112_2_18_2_55_23_867,"SMS: HandleSmsChatItemMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_30436_112_2_18_2_55_23_868,"MMISMS:  HandleSmsChatItemMenuOptWinMsg = %d")
TRACE_MSG(MMISMSAPP_WINTAB_30692_112_2_18_2_55_23_869,"mmisms alloc failed")
TRACE_MSG(MMISMSAPP_WINTAB_30815_112_2_18_2_55_24_870,"MMISMS_HandleWaitWinMsg:insert unhandle sms")
TRACE_MSG(MMISMSAPP_WINTAB_30825_112_2_18_2_55_24_871,"MMISMS_HandleSendWaitWinMsg: Send cancel! dual_sys = %d")
TRACE_MSG(MMISMSAPP_WINTAB_31121_112_2_18_2_55_24_872,"HandleEntryDetailPdaWinMsg list_item_ptr %d")
TRACE_MSG(MMISMSAPP_WINTAB_31267_112_2_18_2_55_25_873,"SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_31331_112_2_18_2_55_25_874,"SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_31396_112_2_18_2_55_25_875,"SMS: HandleChatMarkMenuWindow, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_31442_112_2_18_2_55_25_876,"SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_31544_112_2_18_2_55_25_877,"SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_31617_112_2_18_2_55_25_878,"SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_31826_112_2_18_2_55_26_879,"sms order type = %d")
TRACE_MSG(MMISMSAPP_WINTAB_31958_112_2_18_2_55_26_880,"MMIAPISMS_OpenValidateSecruityBox. user_callback = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_31983_112_2_18_2_55_26_881,"MMIAPISMS_OpenValidateSecruityBox. user_callback = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_32273_112_2_18_2_55_27_882,"MMISMS:HandleOkMsgInSmsList order_id = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_32302_112_2_18_2_55_27_883,"MMISMS:HandleOkMsgInSmsList order_id = PNULL")
TRACE_MSG(MMISMSAPP_WINTAB_32450_112_2_18_2_55_27_884,"MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x")
TRACE_MSG(MMISMSAPP_WINTAB_32497_112_2_18_2_55_27_885,"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_1283_112_2_18_2_55_31_886,"SMSCB: ConvertCBMsgContextToMsgItem msg_len = %d, alphabet_type = %d")
TRACE_MSG(MMISMSCB_WINTAB_1372_112_2_18_2_55_31_887,"MMISMS:DisplayCBMsgContext cb_msg_item_ptr = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_1520_112_2_18_2_55_31_888,"MMISMSCB_Init result[%d] = %d")
TRACE_MSG(MMISMSCB_WINTAB_2140_112_2_18_2_55_33_889,"MMISMS:AddNewMsgToCBMsgList cb_msg_ptr = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_2143_112_2_18_2_55_33_890,"SMSCB: AddNewMsgToCBMsgList")
TRACE_MSG(MMISMSCB_WINTAB_2148_112_2_18_2_55_33_891,"SMSCB: cur_msg_ptr alloc failed")
TRACE_MSG(MMISMSCB_WINTAB_2220_112_2_18_2_55_33_892,"MMISMS:AddNewMarcoMsgToCBMsgList cb_msg_ptr = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_2223_112_2_18_2_55_33_893,"SMSCB: AddNewMarcoMsgToCBMsgList")
TRACE_MSG(MMISMSCB_WINTAB_2228_112_2_18_2_55_33_894,"SMSCB: cur_msg_ptr alloc failed")
TRACE_MSG(MMISMSCB_WINTAB_2297_112_2_18_2_55_33_895,"SMSCB: DeleteMsgFromCBMsgList num = d%, index = d%")
TRACE_MSG(MMISMSCB_WINTAB_2337_112_2_18_2_55_33_896,"SMSCB: DeleteMsgFromCBMsgList num1 = d%")
TRACE_MSG(MMISMSCB_WINTAB_2361_112_2_18_2_55_33_897,"MMISMS:DirectDisplayNewCBMsg cb_msg_ptr = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_2427_112_2_18_2_55_33_898,"MMISMS:DirectDisplayNewMarcoCBMsg cb_msg_ptr = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_2492_112_2_18_2_55_33_899,"MMISMS:MMIAPISMSCB_HandleCBMsgInd cb_msg_ptr = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_2495_112_2_18_2_55_33_900,"SMSCB: MMIAPISMSCB_HandleCBMsgInd dual_sys = %d")
TRACE_MSG(MMISMSCB_WINTAB_2557_112_2_18_2_55_33_901,"MMISMS:MMIAPISMSCB_HandleMarcoCBMsgInd cb_msg_ptr = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_2560_112_2_18_2_55_33_902,"SMSCB: MMISMSCB_HandleMacroCBMsgInd dual_sys = %d")
TRACE_MSG(MMISMSCB_WINTAB_2925_112_2_18_2_55_34_903,"MMISMS:AppendSmsCBLanguageListItem lang_list = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_3168_112_2_18_2_55_35_904,"MMISMS:IsAllLangSelected lang_list = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_3764_112_2_18_2_55_36_905,"MMISMS:WstrToLongNum num = PNULL")
TRACE_MSG(MMISMSCB_WINTAB_4273_112_2_18_2_55_37_906,"SMSCB: HandleSmsCBReadWindow index = %d")
TRACE_MSG(MMISMSCB_WINTAB_4530_112_2_18_2_55_37_907,"SMSCB: HandleSmsCBMainMenuWindow menu_id = %d")
TRACE_MSG(MMISMSCB_WINTAB_4888_112_2_18_2_55_38_908,"SMSCB: HandleSmsCBMainMenuWindow menu_id = %d")
TRACE_MSG(MMISMSCB_WINTAB_5214_112_2_18_2_55_39_909,"SMSCB: HandleSmsCBMainMenuWindow menu_id = %d")
END_TRACE_MAP(MMI_APP_SMS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_SMS_TRC_H_

