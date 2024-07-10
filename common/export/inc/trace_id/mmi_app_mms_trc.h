/******************************************************************************
 ** File Name:      mmi_app_mms_trc.h                                         *
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
//trace_id:196
#ifndef _MMI_APP_MMS_TRC_H_
#define _MMI_APP_MMS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIMMS_CONTROL_MAIN_136_112_2_18_2_37_31_0 "[MMIMMS]:MMIMMS_GetTotalSizeAndAllSize all_size=%d"
#define MMIMMS_CONTROL_MAIN_292_112_2_18_2_37_31_1 "MMIMMS_ControlInit CHNG_FREQ_REQ_HANDLER_NULL == s_mmimms_chng_freq_req_handler"
#define MMIMMS_CONTROL_MAIN_347_112_2_18_2_37_31_2 "\"\"  MMIMMS_SetFreq   \"#"
#define MMIMMS_CONTROL_MAIN_364_112_2_18_2_37_31_3 "\"\"  MMIMMS_SetFreq   \"#"
#define MMIMMS_EDIT_507_112_2_18_2_37_33_4 "[MMIMMS]: mms_no_memory"
#define MMIMMS_EDIT_547_112_2_18_2_37_33_5 "[MMIMMS]:MMIMMS_SetMMSTo string_to NULL"
#define MMIMMS_EDIT_588_112_2_18_2_37_33_6 "[MMIMMS]:MMIMMS_SetMMSCC string_to NULL"
#define MMIMMS_EDIT_628_112_2_18_2_37_33_7 "[MMIMMS]:MMIMMS_SetMMSBCC string_to NULL"
#define MMIMMS_EDIT_851_112_2_18_2_37_33_8 "open the file error"
#define MMIMMS_EDIT_1611_112_2_18_2_37_35_9 "[MMIMMS]:EditMMSReadReport edit_to mms_no_memory"
#define MMIMMS_EDIT_1641_112_2_18_2_37_35_10 "[MMIMMS]:EditMMSReadReport warning malloc report_string.wstr_len=%d failed!"
#define MMIMMS_EDIT_2250_112_2_18_2_37_36_11 "open file error"
#define MMIMMS_EDIT_2262_112_2_18_2_37_36_12 "MMIMMS_AddFile add picture"
#define MMIMMS_EDIT_2270_112_2_18_2_37_36_13 "MMIMMS_AddFile add music"
#define MMIMMS_EDIT_2291_112_2_18_2_37_36_14 "MMIMMS_AddFile add movie"
#define MMIMMS_EDIT_2320_112_2_18_2_37_36_15 "MMS_AddAttachment add normal file attachment"
#define MMIMMS_EDIT_2461_112_2_18_2_37_37_16 "open file error"
#define MMIMMS_EDIT_2478_112_2_18_2_37_37_17 "MMS_AddAttachment add attachment"
#define MMIMMS_EDIT_2486_112_2_18_2_37_37_18 "MMIMMS_ReplaceAttachment replace attachment"
#define MMIMMS_EDIT_2626_112_2_18_2_37_37_19 "[MMIMMS]:AddFileNameToNameList fail"
#define MMIMMS_EDIT_2726_112_2_18_2_37_37_20 "[MMIMMS]:ReducePicForMMS PNULL == bmp_ptr || PNULL == file_name_ptr"
#define MMIMMS_EDIT_2786_112_2_18_2_37_37_21 "MMIMMS_GetCurrentSlideVideoTime video_type=%d"
#define MMIMMS_EDIT_2819_112_2_18_2_37_37_22 "GetCurrentSlideMusicTime  music_time= %d"
#define MMIMMS_EDITWIN_1970_112_2_18_2_37_42_23 "malloc all_number_wstr failed"
#define MMIMMS_EDITWIN_2028_112_2_18_2_37_42_24 "HandleMMSEditToListWinMsg,tele_len = %d"
#define MMIMMS_EDITWIN_2102_112_2_18_2_37_42_25 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!"
#define MMIMMS_EDITWIN_2107_112_2_18_2_37_42_26 "warning_ tele_len 0"
#define MMIMMS_EDITWIN_2123_112_2_18_2_37_42_27 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
#define MMIMMS_EDITWIN_2201_112_2_18_2_37_42_28 "malloc all_number_wstr failed"
#define MMIMMS_EDITWIN_2243_112_2_18_2_37_42_29 "HandleMMSEditToListWinMsg,tele_len = %d"
#define MMIMMS_EDITWIN_2317_112_2_18_2_37_43_30 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!"
#define MMIMMS_EDITWIN_2322_112_2_18_2_37_43_31 "warning_ tele_len 0"
#define MMIMMS_EDITWIN_2337_112_2_18_2_37_43_32 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
#define MMIMMS_EDITWIN_2459_112_2_18_2_37_43_33 "FNH total_size = %ld"
#define MMIMMS_EDITWIN_2978_112_2_18_2_37_44_34 "alloc space not enough"
#define MMIMMS_EDITWIN_2985_112_2_18_2_37_44_35 "mms curr slide not first slide, not show attachment"
#define MMIMMS_EDITWIN_2993_112_2_18_2_37_44_36 "mms additional_file_num == 0, not show attachment"
#define MMIMMS_EDITWIN_3074_112_2_18_2_37_44_37 "MMIMMS: MMSAppendDDLMatchingNumList num&name invalid"
#define MMIMMS_EDITWIN_3107_112_2_18_2_37_44_38 "MMIMMS: MMSAppendDDLMatchingNumList 0 == num_str.wstr_len"
#define MMIMMS_EDITWIN_3117_112_2_18_2_37_44_39 "MMIMMS: MMSAppendDDLMatchingNumList MMIAPIPB_GetSearchEntry reutrn false"
#define MMIMMS_EDITWIN_3454_112_2_18_2_37_45_40 "MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x"
#define MMIMMS_EDITWIN_4619_112_2_18_2_37_48_41 "mms add contact type max: return"
#define MMIMMS_EDITWIN_4677_112_2_18_2_37_48_42 "malloc all_number_wstr failed"
#define MMIMMS_EDITWIN_4726_112_2_18_2_37_48_43 "malloc email_addr failed"
#define MMIMMS_EDITWIN_4752_112_2_18_2_37_48_44 "HandleMMSEditToListWinMsg,tele_len = %d"
#define MMIMMS_EDITWIN_4764_112_2_18_2_37_48_45 "warning_ tele_len 0"
#define MMIMMS_EDITWIN_4877_112_2_18_2_37_48_46 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
#define MMIMMS_EDITWIN_4983_112_2_18_2_37_48_47 "MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x"
#define MMIMMS_EDITWIN_6210_112_2_18_2_37_51_48 "mms add contact type max: return"
#define MMIMMS_EDITWIN_6268_112_2_18_2_37_51_49 "malloc all_number_wstr failed"
#define MMIMMS_EDITWIN_6317_112_2_18_2_37_51_50 "malloc email_addr failed"
#define MMIMMS_EDITWIN_6343_112_2_18_2_37_51_51 "HandleMMSEditToListWinMsg,tele_len = %d"
#define MMIMMS_EDITWIN_6355_112_2_18_2_37_51_52 "warning_ tele_len 0"
#define MMIMMS_EDITWIN_6469_112_2_18_2_37_51_53 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
#define MMIMMS_EDITWIN_6586_112_2_18_2_37_51_54 "MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x"
#define MMIMMS_EDITWIN_6662_112_2_18_2_37_52_55 "HandleMMSEditMainMenuWin MSG_GET_FOCUS"
#define MMIMMS_EDITWIN_7374_112_2_18_2_37_54_56 "malloc all_number_wstr failed"
#define MMIMMS_EDITWIN_7391_112_2_18_2_37_54_57 "malloc email_addr failed"
#define MMIMMS_EDITWIN_7415_112_2_18_2_37_54_58 "HandleMMSEditToListWinMsg,tele_len = %d"
#define MMIMMS_EDITWIN_7487_112_2_18_2_37_54_59 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!"
#define MMIMMS_EDITWIN_7492_112_2_18_2_37_54_60 "warning_ tele_len 0"
#define MMIMMS_EDITWIN_7534_112_2_18_2_37_54_61 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
#define MMIMMS_EDITWIN_7698_112_2_18_2_37_54_62 "MMIMMS: HandleMMSEditAddMenuWinMsg msg_id = 0x%x"
#define MMIMMS_EDITWIN_7868_112_2_18_2_37_55_63 "MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
#define MMIMMS_EDITWIN_7985_112_2_18_2_37_55_64 "MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
#define MMIMMS_EDITWIN_8100_112_2_18_2_37_55_65 "MMIMMS: HandleMMSEditMainMenuOptWinMsg msg_id = 0x%x"
#define MMIMMS_EDITWIN_8124_112_2_18_2_37_55_66 "MMISMS: HandleEditMenuWinMsg call MMIAPIPB_OpenNumberList"
#define MMIMMS_EDITWIN_9003_112_2_18_2_37_57_67 "MMIMMS: HandleMMSEditToListWinMsg msg_id = 0x%x"
#define MMIMMS_EDITWIN_9089_112_2_18_2_37_57_68 "malloc email_addr failed"
#define MMIMMS_EDITWIN_9225_112_2_18_2_37_58_69 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
#define MMIMMS_EDITWIN_9402_112_2_18_2_37_58_70 "malloc email_addr failed"
#define MMIMMS_EDITWIN_9538_112_2_18_2_37_58_71 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
#define MMIMMS_EDITWIN_9654_112_2_18_2_37_59_72 "malloc all_number_wstr failed"
#define MMIMMS_EDITWIN_9666_112_2_18_2_37_59_73 "to num is full MMIMMS_MAX_RECEIVER_NUM"
#define MMIMMS_EDITWIN_9676_112_2_18_2_37_59_74 "malloc num_str_ptr failed"
#define MMIMMS_EDITWIN_9709_112_2_18_2_37_59_75 "malloc email_addr failed"
#define MMIMMS_EDITWIN_9735_112_2_18_2_37_59_76 "HandleMMSEditToListWinMsg,tele_len = %d"
#define MMIMMS_EDITWIN_9747_112_2_18_2_37_59_77 "warning_ tele_len 0"
#define MMIMMS_EDITWIN_9938_112_2_18_2_37_59_78 "MMIMMS: HandleMMSEditSubjectWinMsg msg_id = 0x%x"
#define MMIMMS_EDITWIN_10040_112_2_18_2_37_59_79 "MMIMMS: HandleMMSEditTextWinMsg msg_id = 0x%x"
#define MMIMMS_EDITWIN_10233_112_2_18_2_38_0_80 "MMIMMS: HandleMMSEditSetSlideTimeWin msg_id = 0x%x"
#define MMIMMS_EDITWIN_10354_112_2_18_2_38_0_81 "HandleSIMSelectSendWinMsg dual_sys = %d"
#define MMIMMS_EDITWIN_10369_112_2_18_2_38_0_82 "HandleSIMSelectSendWinMsg record id = %d"
#define MMIMMS_EDITWIN_10376_112_2_18_2_38_0_83 "HandleSIMSelectSendWinMsg : error record id"
#define MMIMMS_EDITWIN_10384_112_2_18_2_38_0_84 "HandleSIMSelectSendWinMsg MMIMMS_GetMMSSendStatus error!"
#define MMIMMS_EDITWIN_10411_112_2_18_2_38_0_85 "HandleSIMSelectSendWinMsg : exit error"
#define MMIMMS_EDITWIN_10422_112_2_18_2_38_0_86 "HandleSIMSelectSendWinMsg : list"
#define MMIMMS_EDITWIN_10491_112_2_18_2_38_0_87 "HandleSIMSelectSendWinMsg dual_sys = %d"
#define MMIMMS_EDITWIN_10501_112_2_18_2_38_0_88 "HandleSIMSelectSendWinMsg record id = %d"
#define MMIMMS_EDITWIN_10508_112_2_18_2_38_0_89 "HandleSIMSelectSendWinMsg : error record id"
#define MMIMMS_EDITWIN_10516_112_2_18_2_38_0_90 "HandleSIMSelectSendWinMsg MMIMMS_GetMMSSendStatus error!"
#define MMIMMS_EDITWIN_10542_112_2_18_2_38_0_91 "HandleSIMSelectSendWinMsg : exit error"
#define MMIMMS_EDITWIN_10553_112_2_18_2_38_0_92 "HandleSIMSelectSendWinMsg : list"
#define MMIMMS_EDITWIN_10577_112_2_18_2_38_0_93 "MMIMMS: HandleAttachmenTypeWinMsg msg_id = 0x%x"
#define MMIMMS_EDITWIN_10920_112_2_18_2_38_1_94 "MMSEditAddToItem %d"
#define MMIMMS_EDITWIN_10998_112_2_18_2_38_1_95 "MMSEditAddToItem %d"
#define MMIMMS_EDITWIN_11158_112_2_18_2_38_2_96 "MMSEditAddPicItem drm file "
#define MMIMMS_EDITWIN_11171_112_2_18_2_38_2_97 "MMSEditAddPicItem"
#define MMIMMS_EDITWIN_11251_112_2_18_2_38_2_98 "MMSEditAddPicItem drm file "
#define MMIMMS_EDITWIN_11263_112_2_18_2_38_2_99 "MMSEditAddPicItem"
#define MMIMMS_EDITWIN_11312_112_2_18_2_38_2_100 "MMSEditAddPicItem jackal12"
#define MMIMMS_EDITWIN_11322_112_2_18_2_38_2_101 "MMSEditAddPicItem jackal13"
#define MMIMMS_EDITWIN_11402_112_2_18_2_38_2_102 "MMSEditAddMusicItem: no audio"
#define MMIMMS_EDITWIN_12434_112_2_18_2_38_4_103 "can not insert again"
#define MMIMMS_EDITWIN_13038_112_2_18_2_38_5_104 "malloc email_addr failed"
#define MMIMMS_EDITWIN_13155_112_2_18_2_38_6_105 "HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
#define MMIMMS_EDITWIN_13355_112_2_18_2_38_6_106 "MMIMMS: MMIMMS_SendNewMMS send_result = %d, record_id = %d"
#define MMIMMS_EDITWIN_13377_112_2_18_2_38_6_107 "EditAndSendNewMMS : exit error"
#define MMIMMS_EDITWIN_13900_112_2_18_2_38_7_108 "FNH total_size = %ld"
#define MMIMMS_EDITWIN_14765_112_2_18_2_38_9_109 "MMS edit doc ptr is PNULL"
#define MMIMMS_EDITWIN_15660_112_2_18_2_38_11_110 "mms alloc failed "
#define MMIMMS_EDITWIN_15664_112_2_18_2_38_11_111 "MMSEditAddToItem %d"
#define MMIMMS_EDITWIN_15739_112_2_18_2_38_11_112 "mms alloc failed "
#define MMIMMS_EDITWIN_15743_112_2_18_2_38_11_113 "MMSEditAddToItem %d"
#define MMIMMS_EDITWIN_15924_112_2_18_2_38_11_114 "MMS alloc error"
#define MMIMMS_EDITWIN_15975_112_2_18_2_38_11_115 "MMS alloc error"
#define MMIMMS_EDITWIN_16096_112_2_18_2_38_11_116 "MMIAPIMMS_EditNewMMS : edit document alloc failed"
#define MMIMMS_EDITWIN_16195_112_2_18_2_38_12_117 "MMS malloc failed "
#define MMIMMS_EDITWIN_16282_112_2_18_2_38_12_118 "MMS malloc failed "
#define MMIMMS_EDITWIN_16385_112_2_18_2_38_12_119 "MMS malloc failed "
#define MMIMMS_EDITWIN_17019_112_2_18_2_38_13_120 "MMS sms get number error"
#define MMIMMS_EDITWIN_17095_112_2_18_2_38_13_121 "MMISMS: GetNumberFromPb msg_id = 0x%x, count = %d"
#define MMIMMS_EDITWIN_17302_112_2_18_2_38_14_122 "MMS file type error"
#define MMIMMS_EDITWIN_17306_112_2_18_2_38_14_123 "MMS file type error %d"
#define MMIMMS_EDITWIN_17345_112_2_18_2_38_14_124 "MMS image is null"
#define MMIMMS_EDITWIN_17427_112_2_18_2_38_14_125 "mms dc error %d"
#define MMIMMS_EDITWIN_17451_112_2_18_2_38_14_126 "mms dc error %d"
#define MMIMMS_EDITWIN_17473_112_2_18_2_38_14_127 "[MMIMMS]MMSOpenRecord mms record error!"
#define MMIMMS_EXPORT_94_112_2_18_2_38_15_128 "[MMIMMS]: HandleSIMPlugNotifyMMSFunc dual_sys=%d, notify_event=%d"
#define MMIMMS_EXPORT_119_112_2_18_2_38_15_129 "[MMIMMS]:MMIAPIMMS_AppInit"
#define MMIMMS_EXPORT_155_112_2_18_2_38_15_130 "[MMIMMS]:MMIAPIMMS_ListInit"
#define MMIMMS_EXPORT_390_112_2_18_2_38_16_131 "MMIAPIMMS_EditNewMMS : edit document alloc failed"
#define MMIMMS_EXPORT_415_112_2_18_2_38_16_132 "MMIAPIMMS_AnswerMMS : answer"
#define MMIMMS_EXPORT_470_112_2_18_2_38_16_133 "MMIAPIMMS_AnswerMMS : edit document alloc failed"
#define MMIMMS_EXPORT_544_112_2_18_2_38_16_134 "MMIAPIMMS_BrwSendUrlByMMS : edit document alloc failed"
#define MMIMMS_EXPORT_599_112_2_18_2_38_16_135 "MMIAPIMMS_DraftEditMMS : edit document alloc failed"
#define MMIMMS_EXPORT_733_112_2_18_2_38_16_136 "MMIAPIMMS_FarwardMMS : edit document alloc failed"
#define MMIMMS_EXPORT_814_112_2_18_2_38_16_137 "MMIAPIMMS_ReadMMS : edit document alloc failed"
#define MMIMMS_EXPORT_871_112_2_18_2_38_17_138 "MMIAPIMMS_OutboxSendMMS : edit document read failed"
#define MMIMMS_EXPORT_1003_112_2_18_2_38_17_139 "MMIMMS: HandleMMSPicInsertQueryWinMsg msg_id = 0x%x"
#define MMIMMS_EXPORT_1429_112_2_18_2_38_18_140 "[MMIMMS]:MMIAPIMMS_RestartAutodownMMS!"
#define MMIMMS_EXPORT_1591_112_2_18_2_38_18_141 "MMS edit init failed"
#define MMIMMS_EXPORT_1807_112_2_18_2_38_18_142 "FNH total_size = %ld"
#define MMIMMS_FILE_233_112_2_18_2_38_20_143 "[MMIMMS]:AddListItem error record_id==0"
#define MMIMMS_FILE_243_112_2_18_2_38_20_144 "[MMIMMS]:AddListItem PNULL == list_item_ptr"
#define MMIMMS_FILE_305_112_2_18_2_38_20_145 "[MMIMMS]:AddListItem PNULL == list_item_ptr"
#define MMIMMS_FILE_332_112_2_18_2_38_20_146 "[MMIMMS]:AddListItem PNULL == list_item_ptr"
#define MMIMMS_FILE_614_112_2_18_2_38_20_147 "[MMIMMS]:MMIMMS_AddAutoList PNULL == list_ptr"
#define MMIMMS_FILE_650_112_2_18_2_38_20_148 "[MMIMMS]:MMIMMS_AddManualList record_id=%d"
#define MMIMMS_FILE_659_112_2_18_2_38_20_149 "[MMIMMS]:MMIMMS_AddManualList PNULL == list_ptr"
#define MMIMMS_FILE_858_112_2_18_2_38_21_150 "[MMIMMS]:MMIMMS_ReadMMSListFile read_size=%d"
#define MMIMMS_FILE_869_112_2_18_2_38_21_151 "[MMIMMS]:MMIMMS_ReadMMSListFile mms version error. clean all..."
#define MMIMMS_FILE_901_112_2_18_2_38_21_152 "[MMIMMS]:MMIMMS_ReadMMSListFile PNULL == buffer_ptr"
#define MMIMMS_FILE_921_112_2_18_2_38_21_153 "[MMIMMS]:MMIMMS_ReadMMSListFile mms list error. clean all..."
#define MMIMMS_FILE_1037_112_2_18_2_38_21_154 "[MMIMMS]: MMIMMS_ReadPush malloc mem failed"
#define MMIMMS_FILE_1056_112_2_18_2_38_21_155 "[MMIMMS]: MMIMMS_ReadPush file is not exist!"
#define MMIMMS_FILE_1062_112_2_18_2_38_21_156 "[MMIMMS]: MMIMMS_ReadPush fetal error record_id==0!"
#define MMIMMS_FILE_1112_112_2_18_2_38_21_157 "[MMIMMS]:MMIMMS_ReadMMS record_id=%d,file_size=%d"
#define MMIMMS_FILE_1169_112_2_18_2_38_21_158 "[MMIMMS]:MMIMMS_ReadMMS buffer_ptr alloc fail!"
#define MMIMMS_FILE_1174_112_2_18_2_38_21_159 "[MMIMMS]:MMIMMS_ReadMMS file is wrong or not exist!"
#define MMIMMS_FILE_1326_112_2_18_2_38_22_160 "[MMIMMS]:IntegrateMMS record_id=%d"
#define MMIMMS_FILE_1335_112_2_18_2_38_22_161 "[MMIMMS]:IntegrateMMS MMS_SetMmsFile failed"
#define MMIMMS_FILE_1413_112_2_18_2_38_22_162 "[MMIMMS]:IntegrateMMS MMS_EncodeMms failed"
#define MMIMMS_FILE_1726_112_2_18_2_38_23_163 "[MMIMMS]:%x %x %x %x %x %x %x %x"
#define MMIMMS_FILE_1739_112_2_18_2_38_23_164 "[MMIMMS]:MMIMMS_SaveMMSListFile PNULL == buffer_ptr"
#define MMIMMS_FILE_1757_112_2_18_2_38_23_165 "[MMIMMS]:%x %x %x %x %x %x %x %x"
#define MMIMMS_FILE_1761_112_2_18_2_38_23_166 "[MMIMMS]:MMIMMS_SaveMMSListFile FAIL, fetal error!"
#define MMIMMS_FILE_1814_112_2_18_2_38_23_167 "[MMIMMS]:MMIMMS_SavePush mms sender is in black lists!"
#define MMIMMS_FILE_1820_112_2_18_2_38_23_168 "[MMIMMS]:MMIMMS_SavePush num_len is d%!"
#define MMIMMS_FILE_1825_112_2_18_2_38_23_169 "[MMIMMS]:MMIMMS_SavePush msg_ptr->phone_number is PNULL!"
#define MMIMMS_FILE_1844_112_2_18_2_38_23_170 "[MMIMMS]:MMIMMS_SavePush not enough space for list!"
#define MMIMMS_FILE_1851_112_2_18_2_38_23_171 "[MMIMMS]:MMIMMS_SavePush read push error alloc failed!"
#define MMIMMS_FILE_1872_112_2_18_2_38_23_172 "[MMIMMS]: MMIMMS_SavePush record_id=%d"
#define MMIMMS_FILE_1876_112_2_18_2_38_23_173 "[MMIMMS]: %x %x %x %x %x %x %x %x "
#define MMIMMS_FILE_1914_112_2_18_2_38_23_174 "[MMIMMS]: MMIMMS_SavePush error"
#define MMIMMS_FILE_1960_112_2_18_2_38_23_175 "[MMIMMS]:MMIMMS_SaveMMS save mms in"
#define MMIMMS_FILE_1969_112_2_18_2_38_23_176 "[MMIMMS]:MMIMMS_SaveMMS no space is_record_exist=%d,is_maxnum=%d,is_enough_space=%d"
#define MMIMMS_FILE_2022_112_2_18_2_38_23_177 "MMSEditAddToItem %d"
#define MMIMMS_FILE_2090_112_2_18_2_38_23_178 "[MMIMMS]:MMIMMS_SaveMMS record_id=%d"
#define MMIMMS_FILE_2103_112_2_18_2_38_23_179 "[MMIMMS]:MMIMMS_SaveMMS write mms file error"
#define MMIMMS_FILE_2110_112_2_18_2_38_23_180 "[MMIMMS]:MMIMMS_SaveMMS save mms list error"
#define MMIMMS_FILE_2118_112_2_18_2_38_23_181 "[MMIMMS]:MMIMMS_SaveMMS no space"
#define MMIMMS_FILE_2128_112_2_18_2_38_23_182 "[MMIMMS]:MMIMMS_SaveMMS encode error"
#define MMIMMS_FILE_2163_112_2_18_2_38_23_183 "[MMIMMS]:MMIMMS_SaveMMSContent is_text = %d"
#define MMIMMS_FILE_2174_112_2_18_2_38_23_184 "[MMIMMS]:MMIMMS_SaveMMSContent no space"
#define MMIMMS_FILE_2225_112_2_18_2_38_24_185 "[MMIMMS]:MMIMMS_SaveMMSContent buffer_len = %d"
#define MMIMMS_FILE_2446_112_2_18_2_38_24_186 "[MMIMMS]:MMIMMS_InitBlock"
#define MMIMMS_FILE_2459_112_2_18_2_38_24_187 "[MMIMMS]:MMIMMS_InitBlock call MMIAPIMEM_Config fail!"
#define MMIMMS_FILE_2466_112_2_18_2_38_24_188 "[MMIMMS]:MMIMMS_InitBlock INVALID_MEM_HANDLE != s_mms_mem_handle"
#define MMIMMS_FILE_2489_112_2_18_2_38_24_189 "[MMIMMS]:malloc error:size == 0"
#define MMIMMS_FILE_2492_112_2_18_2_38_24_190 "[MMIMMS]: malloc size= %d"
#define MMIMMS_FILE_2518_112_2_18_2_38_24_191 "[BRW]error:malloc failed,size = %d"
#define MMIMMS_FILE_2538_112_2_18_2_38_24_192 "MMIMMS_DestroyBlock, handle is null!"
#define MMIMMS_FILE_2569_112_2_18_2_38_24_193 "[MMIMMS]:MMIMMS_FREE ERR:memory leak!"
#define MMIMMS_MAIN_NEW_219_112_2_18_2_38_25_194 "MMIMMS_CheckIfEntryWinAllOpen,%d opened"
#define MMIMMS_MAIN_NEW_426_112_2_18_2_38_26_195 "MMIMMS_RecvAndDownloadMMS error type nor delivery or mms noti"
#define MMIMMS_MAIN_NEW_487_112_2_18_2_38_26_196 "MMIMMS_NewMMSOrNotiMsg p:%d, s:%d"
#define MMIMMS_MAIN_NEW_571_112_2_18_2_38_26_197 "MMIMMS_DetailAndDownloadMMS: call recv func fail!"
#define MMIMMS_MAIN_NEW_593_112_2_18_2_38_26_198 "[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d"
#define MMIMMS_MAIN_NEW_656_112_2_18_2_38_26_199 "MMS: PlayMMSRing normal call mode"
#define MMIMMS_MAIN_NEW_661_112_2_18_2_38_26_200 "MMS: MMIMMS_PlayMMSSound ring_type=%d"
#define MMIMMS_MAIN_NEW_778_112_2_18_2_38_26_201 "[MMIMMS]:MMIMMS_HandleControlMsg PDP Active Success!"
#define MMIMMS_MAIN_NEW_783_112_2_18_2_38_26_202 "[MMIMMS]:MMIMMS_HandleControlMsg reply the recv mms!"
#define MMIMMS_MAIN_NEW_822_112_2_18_2_38_26_203 "[MMIMMS]MSG_MMS_DATA_RECEIVED:download size cur_size=%d, total_size=%d"
#define MMIMMS_MAIN_NEW_843_112_2_18_2_38_26_204 "[MMIMMS]:MMIMMS_HandleControlMsg recv error = %d"
#define MMIMMS_MAIN_NEW_899_112_2_18_2_38_27_205 "[MMIMMS]MSG_MMS_DATA_SENT:temp_progress_ptr is NULL"
#define MMIMMS_MAIN_NEW_908_112_2_18_2_38_27_206 "[MMIMMS]MSG_MMS_DATA_SENT:send size cur_size=%d, total_size=%d"
#define MMIMMS_MAIN_NEW_929_112_2_18_2_38_27_207 "[MMIMMS]MSG_MMS_SEND_END error id = %d, is_saved = %d"
#define MMIMMS_MAIN_NEW_998_112_2_18_2_38_27_208 "[MMIMMS]:MMIMMS_HandleControlMsg PDP deactive!"
#define MMIMMS_MAIN_NEW_1044_112_2_18_2_38_27_209 "[MMIMMS]:MMIMMS_SetMMSMemState PNULL == item_data index"
#define MMIMMS_MAIN_NEW_1062_112_2_18_2_38_27_210 "[MMIMMS]:MMIMMS_SetMMSMemState PNULL == item_data index"
#define MMIMMS_MAIN_NEW_1102_112_2_18_2_38_27_211 "[MMIMMS]:AddMmsMemStateItems PNULL == item_data index"
#define MMIMMS_MAIN_NEW_1134_112_2_18_2_38_27_212 "[MMIMMS]:AddMmsMemStateItems default"
#define MMIMMS_MAIN_NEW_1167_112_2_18_2_38_27_213 "AddMmsMemStateItems used_size = %ld, total_size = %ld"
#define MMIMMS_MAIN_NEW_1226_112_2_18_2_38_27_214 "[MMIMMS]:AddMmsMemStateItems PNULL == item_data index"
#define MMIMMS_MAIN_NEW_1258_112_2_18_2_38_27_215 "[MMIMMS]:AddMmsMemStateItems default"
#define MMIMMS_MAIN_NEW_1286_112_2_18_2_38_27_216 "AddMmsMemStateItems used_size = %ld, total_size = %ld"
#define MMIMMS_MAIN_NEW_1386_112_2_18_2_38_28_217 "MMIMMS_DeliveryRpt_Arrive IN "
#define MMIMMS_MAIN_NEW_1397_112_2_18_2_38_28_218 "[MMIMMS]:MMIMMS_DeliveryRptArrive push_status is%d"
#define MMIMMS_NET_181_112_2_18_2_38_30_219 "[MMIMMS]:MMSNetCallback status=%d, status_value=%d"
#define MMIMMS_NET_210_112_2_18_2_38_30_220 "MMSNetCallback MMS_RECEIVING fetal error!"
#define MMIMMS_NET_219_112_2_18_2_38_30_221 "MMSNetCallback MMS_SENDING fetal error!"
#define MMIMMS_NET_232_112_2_18_2_38_30_222 "MMSNetCallback fetal error MMS_STATUS_SEND_RETURN MMIMMS_SENDRECV_SENDING != s_activemms_info.type"
#define MMIMMS_NET_302_112_2_18_2_38_30_223 "MMSNetCallback, mms Send!"
#define MMIMMS_NET_303_112_2_18_2_38_30_224 "MMSNetCallback, mms recv failed s_send_retry_pdp_time = %d"
#define MMIMMS_NET_351_112_2_18_2_38_30_225 "MMSNetCallback fetal error MMS_STATUS_SEND_RETURN MMIMMS_SENDRECV_SENDING != s_activemms_info.type"
#define MMIMMS_NET_375_112_2_18_2_38_30_226 "[MMIMMS]:MMSNetCallback MMIMMS_UpdateRecvItem fail"
#define MMIMMS_NET_455_112_2_18_2_38_30_227 "MMSNetCallback, mms recv!"
#define MMIMMS_NET_473_112_2_18_2_38_30_228 "MMSNetCallback, mms recv failed or succ"
#define MMIMMS_NET_502_112_2_18_2_38_31_229 "MMSNetCallback, mms recv failed s_recv_retry_time = %d"
#define MMIMMS_NET_520_112_2_18_2_38_31_230 "MMSNetCallback, mms recv failed retry download mms"
#define MMIMMS_NET_543_112_2_18_2_38_31_231 "MMSNetCallback,  current = %d"
#define MMIMMS_NET_547_112_2_18_2_38_31_232 "MMSNetCallback, total = %d, current = %d"
#define MMIMMS_NET_563_112_2_18_2_38_31_233 "MMSNetCallback, prompt = %d, total = %d"
#define MMIMMS_NET_582_112_2_18_2_38_31_234 "MMSNetCallback, prompt = %d, total = %d"
#define MMIMMS_NET_635_112_2_18_2_38_31_235 "[MMIMMS]:SendOldMMS PNULL == activemms_info_ptr"
#define MMIMMS_NET_645_112_2_18_2_38_31_236 "[MMIMMS]:SendOldMMS call pdp active function return true"
#define MMIMMS_NET_649_112_2_18_2_38_31_237 "[MMIMMS]:SendOldMMS PDP active fail"
#define MMIMMS_NET_716_112_2_18_2_38_31_238 "[MMIMMS]:ReceiveMMS call pdp active function return true"
#define MMIMMS_NET_720_112_2_18_2_38_31_239 "[MMIMMS]:ReceiveMMS PDP active fail"
#define MMIMMS_NET_727_112_2_18_2_38_31_240 "[MMIMMS]:ReceiveMMS url is wrong!"
#define MMIMMS_NET_745_112_2_18_2_38_31_241 "[MMIMMS]:MMIMMS_ActiveRecvSendModule status=%d"
#define MMIMMS_NET_755_112_2_18_2_38_31_242 "[MMIMMS]:MMIMMS_ActiveRecvSendModule pdp deactive different sim card"
#define MMIMMS_NET_760_112_2_18_2_38_31_243 "[MMIMMS]:MMIMMS_ActiveRecvSendModule manual_list MMIMMS_PDPDeactive"
#define MMIMMS_NET_785_112_2_18_2_38_31_244 "[MMIMMS]:MMIMMS_ActiveRecvSendModule pdp deactive different sim card"
#define MMIMMS_NET_790_112_2_18_2_38_31_245 "[MMIMMS]:MMIMMS_ActiveRecvSendModule auto_list MMIMMS_PDPDeactive"
#define MMIMMS_NET_809_112_2_18_2_38_31_246 "[MMIMMS]:MMIMMS_ActiveRecvSendModule ACTIVING"
#define MMIMMS_NET_838_112_2_18_2_38_31_247 "[MMIMMS]:MMIMMS_SendNewMMS MMIMMS_ERROR_NO_CONTENT"
#define MMIMMS_NET_852_112_2_18_2_38_32_248 "[MMIMMS]: MMIMMS_SendNewMMS add manual list fail"
#define MMIMMS_NET_877_112_2_18_2_38_32_249 "[MMIMMS]: MMIMMS_SendOldMMS add manual list fail"
#define MMIMMS_NET_902_112_2_18_2_38_32_250 "[MMIMMS]:MMIMMS_SendReadReport reportdoc_ptr=%d"
#define MMIMMS_NET_918_112_2_18_2_38_32_251 "[MMIMMS]: MMIMMS_SendReadReport add manual list fail"
#define MMIMMS_NET_946_112_2_18_2_38_32_252 "[MMIMMS]: MMIMMS_ResponceMMS respValue=%d"
#define MMIMMS_NET_990_112_2_18_2_38_32_253 "[MMIMMS]: MMIMMS_ResponceMMS add manual list fail"
#define MMIMMS_NET_1027_112_2_18_2_38_32_254 "[MMIMMS]: MMIMMS_ResponceMMS add manual list fail"
#define MMIMMS_NET_1087_112_2_18_2_38_32_255 "mmitest remove fname"
#define MMIMMS_NET_1168_112_2_18_2_38_32_256 "[MMIMMS]:MMIMMS_PDPActive dual_sys=%d"
#define MMIMMS_NET_1198_112_2_18_2_38_32_257 "[MMIMMS]:HandlePDPMsg msg_id=%d"
#define MMIMMS_NET_1215_112_2_18_2_38_32_258 "HandlePDPMsg: MMIPDP_DEACTIVE_IND"
#define MMIMMS_NET_1234_112_2_18_2_38_33_259 "MMIMMS_HandleNetTimer:recv timer"
#define MMIMMS_NET_1242_112_2_18_2_38_33_260 "MMIMMS_HandleNetTimer:recv timer pdp permit not!"
#define MMIMMS_NET_1257_112_2_18_2_38_33_261 "MMIMMS_HandleNetTimer:retry recv timer! detach pdp"
#define MMIMMS_NET_1286_112_2_18_2_38_33_262 "[MMIMMS]:MMIMMS_IsPdpPermitUsing net_status = %d"
#define MMIMMS_NET_1335_112_2_18_2_38_33_263 "[MMIMMS]:HandlePDPActiveCnf"
#define MMIMMS_NET_1340_112_2_18_2_38_33_264 "[MMIMMS]:HandlePDPActiveCnf msg_ptr->result=%d"
#define MMIMMS_NET_1414_112_2_18_2_38_33_265 "[MMIMMS]:HandlePDPActiveCnf alloc mms_center_ptr failed"
#define MMIMMS_NET_1443_112_2_18_2_38_33_266 "[MMIMMS]:HandlePDPActiveCnf alloc mms_center_ptr failed"
#define MMIMMS_NET_1498_112_2_18_2_38_33_267 "[MMIMMS]:HandlePDPActiveCnf s_activemms_info.type=%d"
#define MMIMMS_NET_1506_112_2_18_2_38_33_268 "[MMIMMS]:HandlePDPActiveCnf alloc fname failed"
#define MMIMMS_NET_1539_112_2_18_2_38_33_269 "MMIMMS]:MMIMMS_SendNewMMS mms send failed"
#define MMIMMS_NET_1546_112_2_18_2_38_33_270 "[MMIMMS]:MMIMMS_SendNewMMS MMS_SetConfig(&cfgset) failed"
#define MMIMMS_NET_1559_112_2_18_2_38_33_271 "[MMIMMS]:HandlePDPActiveCnf alloc fname failed"
#define MMIMMS_NET_1603_112_2_18_2_38_33_272 "[MMIMMS]:MMIMMS_ReceiveMMS immediate_recv=%d"
#define MMIMMS_NET_1620_112_2_18_2_38_33_273 "MMIMMS]:MMIMMS_ReceiveMMS mms recv failed"
#define MMIMMS_NET_1626_112_2_18_2_38_33_274 "[MMIMMS]:MMIMMS_ReceiveMMS MMS_SetConfig(&cfgset) failed"
#define MMIMMS_NET_1632_112_2_18_2_38_33_275 "[MMIMMS]:HandlePDPActiveCnf recv no space"
#define MMIMMS_NET_1638_112_2_18_2_38_33_276 "[MMIMMS]:MMIMMS_ReceiveMMS alloc pushfile failed"
#define MMIMMS_NET_1653_112_2_18_2_38_33_277 "[MMIMMS]:HandlePDPActiveCnf s_activemms_info.type= %d  WRONG"
#define MMIMMS_PREVIEWWIN_833_112_2_18_2_38_36_278 "HandleMMSPreviewWinMsg msg_id=%x,cur_edoc=%d"
#define MMIMMS_PREVIEWWIN_855_112_2_18_2_38_36_279 "HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!"
#define MMIMMS_PREVIEWWIN_891_112_2_18_2_38_36_280 "HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!"
#define MMIMMS_PREVIEWWIN_1131_112_2_18_2_38_36_281 "HandleMMSPreviewWinMsg playing video! MSG_FULL_PAINT"
#define MMIMMS_PREVIEWWIN_1181_112_2_18_2_38_36_282 "HandleMMSPreviewWinMsg play video"
#define MMIMMS_PREVIEWWIN_1219_112_2_18_2_38_37_283 "HandleMMSPreviewWinMsg playing video! MSG_MMS_PLAY_SLIDE"
#define MMIMMS_PREVIEWWIN_1244_112_2_18_2_38_37_284 "HandleMMSPreviewWinMsg vedio_time=%d,duaration=%d, music_time= %d"
#define MMIMMS_PREVIEWWIN_1323_112_2_18_2_38_37_285 "HandleMMSPreviewWinMsg playing video! MSG_GET_FOCUS"
#define MMIMMS_PREVIEWWIN_1384_112_2_18_2_38_37_286 "HandleMMSPreviewWinMsg playing video!"
#define MMIMMS_PREVIEWWIN_1541_112_2_18_2_38_37_287 "timer:playing state!"
#define MMIMMS_PREVIEWWIN_1549_112_2_18_2_38_37_288 "HandleMMSPreviewWinMsg playing video! MSG_CTL_RICHTEXT_SWITCHFOCUS"
#define MMIMMS_PREVIEWWIN_1605_112_2_18_2_38_37_289 "HandleMMSPreviewWinMsg 1 mms_list_info_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_1611_112_2_18_2_38_37_290 "HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_1621_112_2_18_2_38_37_291 "HandleMMSPreviewWinMsg number_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_1663_112_2_18_2_38_37_292 "HandleMMSPreviewWinMsg 2 mms_list_info_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_1729_112_2_18_2_38_38_293 "HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_1746_112_2_18_2_38_38_294 "HandleMMSPreviewWinMsg focus_url_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_1758_112_2_18_2_38_38_295 "HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_1847_112_2_18_2_38_38_296 "s_edit_mms_preview_state=%d"
#define MMIMMS_PREVIEWWIN_1852_112_2_18_2_38_38_297 "focus item type error!"
#define MMIMMS_PREVIEWWIN_1910_112_2_18_2_38_38_298 "unkown mms type"
#define MMIMMS_PREVIEWWIN_1915_112_2_18_2_38_38_299 "HandleMMSPreviewWinMsg : mms_list_info_ptr is null"
#define MMIMMS_PREVIEWWIN_1945_112_2_18_2_38_38_300 "HandleMMSPreviewWinMsg playing video!"
#define MMIMMS_PREVIEWWIN_1999_112_2_18_2_38_38_301 "HandleMMSPreviewWinMsg playing video!"
#define MMIMMS_PREVIEWWIN_2081_112_2_18_2_38_38_302 "HandleMMSPreviewWinMsg msg_id=%x,cur_edoc=%d"
#define MMIMMS_PREVIEWWIN_2103_112_2_18_2_38_38_303 "HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!"
#define MMIMMS_PREVIEWWIN_2140_112_2_18_2_38_38_304 "HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!"
#define MMIMMS_PREVIEWWIN_2358_112_2_18_2_38_39_305 "HandleMMSPreviewWinMsg playing video! MSG_FULL_PAINT"
#define MMIMMS_PREVIEWWIN_2405_112_2_18_2_38_39_306 "HandleMMSPreviewWinMsg play video"
#define MMIMMS_PREVIEWWIN_2437_112_2_18_2_38_39_307 "HandleMMSPreviewWinMsg playing video! MSG_MMS_PLAY_SLIDE"
#define MMIMMS_PREVIEWWIN_2462_112_2_18_2_38_39_308 "HandleMMSPreviewWinMsg vedio_time=%d,duaration=%d, music_time= %d"
#define MMIMMS_PREVIEWWIN_2529_112_2_18_2_38_39_309 "HandleMMSPreviewWinMsg playing video! MSG_GET_FOCUS"
#define MMIMMS_PREVIEWWIN_2590_112_2_18_2_38_39_310 "HandleMMSPreviewWinMsg playing video!"
#define MMIMMS_PREVIEWWIN_2705_112_2_18_2_38_39_311 "timer:playing state!"
#define MMIMMS_PREVIEWWIN_2713_112_2_18_2_38_39_312 "HandleMMSPreviewWinMsg playing video! MSG_CTL_RICHTEXT_SWITCHFOCUS"
#define MMIMMS_PREVIEWWIN_2769_112_2_18_2_38_40_313 "HandleMMSPreviewWinMsg 1 mms_list_info_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_2775_112_2_18_2_38_40_314 "HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_2785_112_2_18_2_38_40_315 "HandleMMSPreviewWinMsg number_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_2827_112_2_18_2_38_40_316 "HandleMMSPreviewWinMsg 2 mms_list_info_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_2893_112_2_18_2_38_40_317 "HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_2910_112_2_18_2_38_40_318 "HandleMMSPreviewWinMsg focus_url_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_2922_112_2_18_2_38_40_319 "HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
#define MMIMMS_PREVIEWWIN_3005_112_2_18_2_38_40_320 "s_edit_mms_preview_state=%d"
#define MMIMMS_PREVIEWWIN_3010_112_2_18_2_38_40_321 "focus item type error!"
#define MMIMMS_PREVIEWWIN_3068_112_2_18_2_38_40_322 "unkown mms type"
#define MMIMMS_PREVIEWWIN_3073_112_2_18_2_38_40_323 "HandleMMSPreviewWinMsg : mms_list_info_ptr is null"
#define MMIMMS_PREVIEWWIN_3103_112_2_18_2_38_40_324 "HandleMMSPreviewWinMsg playing video!"
#define MMIMMS_PREVIEWWIN_3154_112_2_18_2_38_40_325 "HandleMMSPreviewWinMsg playing video!"
#define MMIMMS_PREVIEWWIN_3229_112_2_18_2_38_41_326 "MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_3395_112_2_18_2_38_41_327 "MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_3433_112_2_18_2_38_41_328 "HandleMMSPreviewUrlPopWinMsg alloc failed"
#define MMIMMS_PREVIEWWIN_3470_112_2_18_2_38_41_329 "HandleMMSPreviewUrlPopWinMsg BOOKMARK alloc failed"
#define MMIMMS_PREVIEWWIN_3565_112_2_18_2_38_41_330 "MMIMMS: HandleMMSPreviewEmailPopWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_3689_112_2_18_2_38_41_331 "MMIMMS: HandleInboxMMSOptMenuWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_4051_112_2_18_2_38_42_332 "MMIMMS TTS play text ok"
#define MMIMMS_PREVIEWWIN_4073_112_2_18_2_38_42_333 "warning_ menu_id=%d"
#define MMIMMS_PREVIEWWIN_4121_112_2_18_2_38_42_334 "warning_ menu_id=%d"
#define MMIMMS_PREVIEWWIN_4182_112_2_18_2_38_42_335 "MMIMMS: HandleInboxMMSDetailOptMenuWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_4240_112_2_18_2_38_43_336 "MMIMMS: HandleInboxMMSDetailOptMenuWinMsg active mms error!"
#define MMIMMS_PREVIEWWIN_4988_112_2_18_2_38_44_337 "MMIMMS: HandleSendboxMMSOptMenuWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_5282_112_2_18_2_38_45_338 "MMIMMS: HandleOutboxMMSOptMenuWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_5467_112_2_18_2_38_45_339 "menu_id=%d"
#define MMIMMS_PREVIEWWIN_5578_112_2_18_2_38_45_340 "MMIMMS: HandleSaveMultiFileWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_5902_112_2_18_2_38_46_341 "MMIMMS: HandleMultifileViewWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_6199_112_2_18_2_38_46_342 "not permit play ring!"
#define MMIMMS_PREVIEWWIN_6331_112_2_18_2_38_47_343 "MMIMMS: item.text_data.buf.len = %d"
#define MMIMMS_PREVIEWWIN_6332_112_2_18_2_38_47_344 "MMIMMS: ViewMultifile data %x %x %x %x"
#define MMIMMS_PREVIEWWIN_6403_112_2_18_2_38_47_345 "warning_ AppendListItem"
#define MMIMMS_PREVIEWWIN_6639_112_2_18_2_38_47_346 "warning catstring dst_ptr=%d,src1_ptr=%d"
#define MMIMMS_PREVIEWWIN_6644_112_2_18_2_38_47_347 "warning catstring len1=%d,len2=%d"
#define MMIMMS_PREVIEWWIN_6656_112_2_18_2_38_47_348 "MMIMMS_CatString: malloc for dst_ptr->str_ptr fail!"
#define MMIMMS_PREVIEWWIN_7004_112_2_18_2_38_48_349 "MMIMMS: HandleSaveFileOptMenuWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_7119_112_2_18_2_38_48_350 "menu_id=%d"
#define MMIMMS_PREVIEWWIN_7592_112_2_18_2_38_49_351 "MMIMMS: HandleInboxMMSDetailOptMenuWinMsg active mms error!"
#define MMIMMS_PREVIEWWIN_7738_112_2_18_2_38_50_352 "MMIMMS: HandleInboxMMSNotiWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_7919_112_2_18_2_38_50_353 "MMIMMS: HandleInboxMMSNotiWinMsg active mms error!"
#define MMIMMS_PREVIEWWIN_7965_112_2_18_2_38_50_354 "MMIMMS: HandleInboxMMSNotiWinMsg msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_8094_112_2_18_2_38_50_355 "MMIMMS: HandleInboxMMSNotiWinMsg active mms error!"
#define MMIMMS_PREVIEWWIN_8140_112_2_18_2_38_50_356 "MMIMMS: HandleSendReadRptWaitingWin msg_id = 0x%x"
#define MMIMMS_PREVIEWWIN_8624_112_2_18_2_38_52_357 "MmsPreviewAddPicAndMovieItem drm file "
#define MMIMMS_PREVIEWWIN_8712_112_2_18_2_38_52_358 "MmsPreviewAddMusicItem fmt=%d"
#define MMIMMS_PREVIEWWIN_8730_112_2_18_2_38_52_359 "not permit play ring!"
#define MMIMMS_PREVIEWWIN_8738_112_2_18_2_38_52_360 "MmsPreviewAddMusicItem audio type error!"
#define MMIMMS_PREVIEWWIN_8743_112_2_18_2_38_52_361 "MmsPreviewAddMusicItem data error!"
#define MMIMMS_PREVIEWWIN_8783_112_2_18_2_38_52_362 "MmsPreviewPlayVideoItem video_type=%d, play_param.video_buf_info.video_buf_ptr = %x"
#define MMIMMS_PREVIEWWIN_8799_112_2_18_2_38_52_363 "MmsPreviewPlayVideoItem video type error"
#define MMIMMS_PREVIEWWIN_8902_112_2_18_2_38_52_364 "MmsPreviewPlayVideoItem data error"
#define MMIMMS_PREVIEWWIN_8973_112_2_18_2_38_52_365 "MmsRingPlayCallBack result=%d"
#define MMIMMS_PREVIEWWIN_9003_112_2_18_2_38_52_366 "[VP] MmsVideoPlayCallBack msg_ptr->msg_id = %x"
#define MMIMMS_PREVIEWWIN_9047_112_2_18_2_38_52_367 "MmsVideoPlayCallBack "
#define MMIMMS_PUSH_297_112_2_18_2_38_54_368 "[MMIMMS]:HandleMMSPushCallback status = %d"
#define MMIMMS_PUSH_304_112_2_18_2_38_54_369 "[MMIMMS]:HandleMMSPushCallback THE MSG->de_status is%d"
#define MMIMMS_PUSH_322_112_2_18_2_38_54_370 "[MMIMMS]:HandleMMSPushCallback MMS_PUSH_TYPE_READORIG_IND"
#define MMIMMS_PUSH_471_112_2_18_2_38_54_371 "[MMIMMS]:HandleMMSPushCallback error s_push_dual_sys >= MN_DUAL_SYS_MAX"
#define MMIMMS_PUSH_480_112_2_18_2_38_54_372 "[MMIMMS]:HandleMMSPushCallback read push error alloc failed!"
#define MMIMMS_PUSH_501_112_2_18_2_38_54_373 "[MMIMMS]:HandleMMSPushCallback read push error!"
#define MMIMMS_PUSH_555_112_2_18_2_38_54_374 "[MMIMMS]:HandleMMSPushCallback push is in black lists"
#define MMIMMS_PUSH_561_112_2_18_2_38_54_375 "[MMIMMS]:HandleMMSPushCallback push save fail"
#define MMIMMS_PUSH_590_112_2_18_2_38_54_376 "[MMIMMS]:HandleMMSPushCallback retrieve manual mode"
#define MMIMMS_PUSH_661_112_2_18_2_38_54_377 "MMIMMS_PushRecvSms dual_sys = %d"
#define MMIMMS_PUSH_664_112_2_18_2_38_54_378 "MMICMS_PUSH_recv_data:not ready!"
#define MMIMMS_PUSH_670_112_2_18_2_38_54_379 "s_mms_data MALLOC FAILED"
#define MMIMMS_PUSH_681_112_2_18_2_38_54_380 "mmibrowser.c:MMICMS_PUSH_recv_data() iei_arr[%d].iei=%d"
#define MMIMMS_PUSH_685_112_2_18_2_38_55_381 "it's a CONCAT push!"
#define MMIMMS_PUSH_718_112_2_18_2_38_55_382 "message_id=%d,total_num=%d,index_id=%d"
#define MMIMMS_PUSH_722_112_2_18_2_38_55_383 "MMIMMS_DecodePush:alloc push_item_ptr failed"
#define MMIMMS_PUSH_752_112_2_18_2_38_55_384 "\"# sunlu MMIMMS MMIMMS_DecodePush sms_ap_id = %d"
#define MMIMMS_PUSH_760_112_2_18_2_38_55_385 "mmibrowser.c:MMICMS_PUSH_recv_data() it is a MMS push"
#define MMIMMS_PUSH_794_112_2_18_2_38_55_386 "\"# sunlu MMIMMS_DecodePush MMIMAIL_IsPushMailForContent return TRUE!!!!!"
#define MMIMMS_PUSH_848_112_2_18_2_38_55_387 "the same PUSH,index_id=%d,time=%d"
#define MMIMMS_PUSH_853_112_2_18_2_38_55_388 "message_num=%d,total_num=%d"
#define MMIMMS_PUSH_893_112_2_18_2_38_55_389 "detail_index=%d"
#define MMIMMS_PUSH_898_112_2_18_2_38_55_390 "write longpush file detail_index=%d,index_id=%d"
#define MMIMMS_PUSH_908_112_2_18_2_38_55_391 "concact push,ok!"
#define MMIMMS_PUSH_996_112_2_18_2_38_55_392 "HandleMMSPreviewWinMsg focus_num_ptr == s_long_push!"
#define MMIMMS_SETTING_59_112_2_18_2_38_55_393 "[MMIMMS]:MMIMMS_ResetMMSFactorySetting"
#define MMIMMS_SETTING_107_112_2_18_2_38_56_394 "[MMIMMS]:MMIMMS_InitSetting"
#define MMIMMS_SETTING_133_112_2_18_2_38_56_395 "[MMIMMS]:MMIMMS_Setting_GetDeliveryReport deliver_report=%d"
#define MMIMMS_SETTING_146_112_2_18_2_38_56_396 "[MMIMMS]:MMIMMS_Setting_SetDeliveryReport is_need_delivery_rpt_send=%d"
#define MMIMMS_SETTING_171_112_2_18_2_38_56_397 "[MMIMMS]:MMIMMS_Setting_GetReadReport read_rpt=%d"
#define MMIMMS_SETTING_184_112_2_18_2_38_56_398 "[MMIMMS]:MMIMMS_Setting_SetReadReport is_need_read_rpt_send=%d"
#define MMIMMS_SETTING_209_112_2_18_2_38_56_399 "[MMIMMS]:MMIMMS_Setting_GetAnonymous is_anonymous=%d"
#define MMIMMS_SETTING_222_112_2_18_2_38_56_400 "[MMIMMS]:MMIMMS_Setting_SetAnonymousReport is_anonymous=%d"
#define MMIMMS_SETTING_247_112_2_18_2_38_56_401 "[MMIMMS]:MMIMMS_Setting_GetPlaysound is_play_sound=%d"
#define MMIMMS_SETTING_260_112_2_18_2_38_56_402 "[MMIMMS]:MMIMMS_Setting_SetPlaysound is_play_sound=%d"
#define MMIMMS_SETTING_280_112_2_18_2_38_56_403 "MMIMMS_Setting_GetAdvertising"
#define MMIMMS_SETTING_299_112_2_18_2_38_56_404 "MMIMMS_Setting_SetAdvertising"
#define MMIMMS_SETTING_325_112_2_18_2_38_56_405 "[MMIMMS]:MMIMMS_Setting_GetExtractContent is_extract_content=%d"
#define MMIMMS_SETTING_338_112_2_18_2_38_56_406 "[MMIMMS]:MMIMMS_Setting_SetExtractContent is_extract_content=%d"
#define MMIMMS_SETTING_363_112_2_18_2_38_56_407 "[MMIMMS]:MMIMMS_Setting_GetPriority priority=%d"
#define MMIMMS_SETTING_376_112_2_18_2_38_56_408 "[MMIMMS]:MMIMMS_Setting_SetPriority priority=%d"
#define MMIMMS_SETTING_402_112_2_18_2_38_56_409 "[MMIMMS]:MMIMMS_Setting_GetRetrieveMode retrieve_mode=%d"
#define MMIMMS_SETTING_417_112_2_18_2_38_56_410 "[MMIMMS]:MMIMMS_Setting_SetRetrieveMode retrieve_mode=%d"
#define MMIMMS_SETTING_443_112_2_18_2_38_56_411 "[MMIMMS]:MMIMMS_Setting_GetValidPeriod valid_period=%d"
#define MMIMMS_SETTING_456_112_2_18_2_38_56_412 "[MMIMMS]:MMIMMS_Setting_SetValidPeriod valid_period=%d"
#define MMIMMS_SETTING_481_112_2_18_2_38_56_413 "[MMIMMS]:MMIMMS_Setting_GetRecvDeliveryReport is_need_delivery_rpt_recv=%d"
#define MMIMMS_SETTING_494_112_2_18_2_38_56_414 "[MMIMMS]:MMIMMS_Setting_SetRecvDeliveryReport is_need_delivery_rpt_recv=%d"
#define MMIMMS_SETTING_514_112_2_18_2_38_56_415 "MMIMMS_Setting_GetPreferDisk"
#define MMIMMS_SETTING_521_112_2_18_2_38_56_416 "[MMIMMS]:MMIMMS_Setting_GetPreferDisk device_type=%d"
#define MMIMMS_SETTING_534_112_2_18_2_38_56_417 "[MMIMMS]:MMIMMS_Setting_SetPreferDisk device_type=%d"
#define MMIMMS_SETTING_564_112_2_18_2_38_57_418 "[MMIMMS]:MMIMMS_Setting_GetNetSettingIndex,dual_sys=%d,index=%d"
#define MMIMMS_SETTING_580_112_2_18_2_38_57_419 "[MMIMMS]:MMIMMS_Setting_SetNetSettingIndex dual_sys=%d index=%d"
#define MMIMMS_SETTING_604_112_2_18_2_38_57_420 "MMIMMS_Setting_ConnectionCallback,index=%d,dual_sys=%d"
#define MMIMMS_SETTING_664_112_2_18_2_38_57_421 "[MMIMMS]:MMIMMS_Setting_GetRetrieveModeLocal RetrieveModeLocal=%d"
#define MMIMMS_SETTING_669_112_2_18_2_38_57_422 "[MMIMMS]:MMIMMS_Setting_GetRetrieveModeRoam RetrieveModeRoam=%d"
#define MMIMMS_SETTING_692_112_2_18_2_38_57_423 "[MMIMMS]:MMIMMS_Setting_SetRetrieveModeLocal RetrieveModeLocal=%d"
#define MMIMMS_SETTING_697_112_2_18_2_38_57_424 "[MMIMMS]:MMIMMS_Setting_SetRetrieveModeRoam RetrieveModeRoam=%d"
#define MMIMMS_SETTINGWIN_1297_112_2_18_2_39_0_425 "list_index=%d"
#define MMIMMS_WAITINGWIN_136_112_2_18_2_39_3_426 "HandleMMSDownloadingWin: MSG_MMS_SEND_RETRYING send size g_mms_cur_size=%d, g_mms_total_size=%d"
#define MMIMMS_WAITINGWIN_181_112_2_18_2_39_3_427 "HandleMMSDownloadingWin:download size g_mms_cur_size=%d, g_mms_total_size=%d"
#define MMIMMS_WAITINGWIN_278_112_2_18_2_39_3_428 "HandleMMSSendingWin: MSG_FULL_PAINT send size g_mms_cur_size=%d, g_mms_total_size=%d"
#define MMIMMS_WAITINGWIN_287_112_2_18_2_39_3_429 "HandleMMSSendingWin: MSG_MMS_SEND_RETRYING send size g_mms_cur_size=%d, g_mms_total_size=%d"
#define MMIMMS_WAITINGWIN_378_112_2_18_2_39_3_430 "HandleMMSSendingWin:send size g_mms_cur_size=%d, g_mms_total_size=%d"
#define MMIMMS_WAITINGWIN_394_112_2_18_2_39_3_431 "HandleMMSSendingWin error id = %d, is_saved = %d"
#define MMIMMS_WAITINGWIN_494_112_2_18_2_39_4_432 "MMSMMI_CreateDownloadWaitingWin dual_sys = %d"
#define MMIMMS_WAITINGWIN_557_112_2_18_2_39_4_433 "MMSMMI_CreateSendingWaitingWin dual_sys = %d"
#define MMIMMS_WAITINGWIN_611_112_2_18_2_39_4_434 "mmimms_main.c, MMIMMS_DisplaySendRecvProgress(), pos = %d, totalsize = %d."
#define MMIMMS_WAITINGWIN_698_112_2_18_2_39_4_435 "FNH now_size = %ld, total_size = %ld"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MMS_TRC)
TRACE_MSG(MMIMMS_CONTROL_MAIN_136_112_2_18_2_37_31_0,"[MMIMMS]:MMIMMS_GetTotalSizeAndAllSize all_size=%d")
TRACE_MSG(MMIMMS_CONTROL_MAIN_292_112_2_18_2_37_31_1,"MMIMMS_ControlInit CHNG_FREQ_REQ_HANDLER_NULL == s_mmimms_chng_freq_req_handler")
TRACE_MSG(MMIMMS_CONTROL_MAIN_347_112_2_18_2_37_31_2,"\"\"  MMIMMS_SetFreq   \"#")
TRACE_MSG(MMIMMS_CONTROL_MAIN_364_112_2_18_2_37_31_3,"\"\"  MMIMMS_SetFreq   \"#")
TRACE_MSG(MMIMMS_EDIT_507_112_2_18_2_37_33_4,"[MMIMMS]: mms_no_memory")
TRACE_MSG(MMIMMS_EDIT_547_112_2_18_2_37_33_5,"[MMIMMS]:MMIMMS_SetMMSTo string_to NULL")
TRACE_MSG(MMIMMS_EDIT_588_112_2_18_2_37_33_6,"[MMIMMS]:MMIMMS_SetMMSCC string_to NULL")
TRACE_MSG(MMIMMS_EDIT_628_112_2_18_2_37_33_7,"[MMIMMS]:MMIMMS_SetMMSBCC string_to NULL")
TRACE_MSG(MMIMMS_EDIT_851_112_2_18_2_37_33_8,"open the file error")
TRACE_MSG(MMIMMS_EDIT_1611_112_2_18_2_37_35_9,"[MMIMMS]:EditMMSReadReport edit_to mms_no_memory")
TRACE_MSG(MMIMMS_EDIT_1641_112_2_18_2_37_35_10,"[MMIMMS]:EditMMSReadReport warning malloc report_string.wstr_len=%d failed!")
TRACE_MSG(MMIMMS_EDIT_2250_112_2_18_2_37_36_11,"open file error")
TRACE_MSG(MMIMMS_EDIT_2262_112_2_18_2_37_36_12,"MMIMMS_AddFile add picture")
TRACE_MSG(MMIMMS_EDIT_2270_112_2_18_2_37_36_13,"MMIMMS_AddFile add music")
TRACE_MSG(MMIMMS_EDIT_2291_112_2_18_2_37_36_14,"MMIMMS_AddFile add movie")
TRACE_MSG(MMIMMS_EDIT_2320_112_2_18_2_37_36_15,"MMS_AddAttachment add normal file attachment")
TRACE_MSG(MMIMMS_EDIT_2461_112_2_18_2_37_37_16,"open file error")
TRACE_MSG(MMIMMS_EDIT_2478_112_2_18_2_37_37_17,"MMS_AddAttachment add attachment")
TRACE_MSG(MMIMMS_EDIT_2486_112_2_18_2_37_37_18,"MMIMMS_ReplaceAttachment replace attachment")
TRACE_MSG(MMIMMS_EDIT_2626_112_2_18_2_37_37_19,"[MMIMMS]:AddFileNameToNameList fail")
TRACE_MSG(MMIMMS_EDIT_2726_112_2_18_2_37_37_20,"[MMIMMS]:ReducePicForMMS PNULL == bmp_ptr || PNULL == file_name_ptr")
TRACE_MSG(MMIMMS_EDIT_2786_112_2_18_2_37_37_21,"MMIMMS_GetCurrentSlideVideoTime video_type=%d")
TRACE_MSG(MMIMMS_EDIT_2819_112_2_18_2_37_37_22,"GetCurrentSlideMusicTime  music_time= %d")
TRACE_MSG(MMIMMS_EDITWIN_1970_112_2_18_2_37_42_23,"malloc all_number_wstr failed")
TRACE_MSG(MMIMMS_EDITWIN_2028_112_2_18_2_37_42_24,"HandleMMSEditToListWinMsg,tele_len = %d")
TRACE_MSG(MMIMMS_EDITWIN_2102_112_2_18_2_37_42_25,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!")
TRACE_MSG(MMIMMS_EDITWIN_2107_112_2_18_2_37_42_26,"warning_ tele_len 0")
TRACE_MSG(MMIMMS_EDITWIN_2123_112_2_18_2_37_42_27,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo")
TRACE_MSG(MMIMMS_EDITWIN_2201_112_2_18_2_37_42_28,"malloc all_number_wstr failed")
TRACE_MSG(MMIMMS_EDITWIN_2243_112_2_18_2_37_42_29,"HandleMMSEditToListWinMsg,tele_len = %d")
TRACE_MSG(MMIMMS_EDITWIN_2317_112_2_18_2_37_43_30,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!")
TRACE_MSG(MMIMMS_EDITWIN_2322_112_2_18_2_37_43_31,"warning_ tele_len 0")
TRACE_MSG(MMIMMS_EDITWIN_2337_112_2_18_2_37_43_32,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo")
TRACE_MSG(MMIMMS_EDITWIN_2459_112_2_18_2_37_43_33,"FNH total_size = %ld")
TRACE_MSG(MMIMMS_EDITWIN_2978_112_2_18_2_37_44_34,"alloc space not enough")
TRACE_MSG(MMIMMS_EDITWIN_2985_112_2_18_2_37_44_35,"mms curr slide not first slide, not show attachment")
TRACE_MSG(MMIMMS_EDITWIN_2993_112_2_18_2_37_44_36,"mms additional_file_num == 0, not show attachment")
TRACE_MSG(MMIMMS_EDITWIN_3074_112_2_18_2_37_44_37,"MMIMMS: MMSAppendDDLMatchingNumList num&name invalid")
TRACE_MSG(MMIMMS_EDITWIN_3107_112_2_18_2_37_44_38,"MMIMMS: MMSAppendDDLMatchingNumList 0 == num_str.wstr_len")
TRACE_MSG(MMIMMS_EDITWIN_3117_112_2_18_2_37_44_39,"MMIMMS: MMSAppendDDLMatchingNumList MMIAPIPB_GetSearchEntry reutrn false")
TRACE_MSG(MMIMMS_EDITWIN_3454_112_2_18_2_37_45_40,"MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_4619_112_2_18_2_37_48_41,"mms add contact type max: return")
TRACE_MSG(MMIMMS_EDITWIN_4677_112_2_18_2_37_48_42,"malloc all_number_wstr failed")
TRACE_MSG(MMIMMS_EDITWIN_4726_112_2_18_2_37_48_43,"malloc email_addr failed")
TRACE_MSG(MMIMMS_EDITWIN_4752_112_2_18_2_37_48_44,"HandleMMSEditToListWinMsg,tele_len = %d")
TRACE_MSG(MMIMMS_EDITWIN_4764_112_2_18_2_37_48_45,"warning_ tele_len 0")
TRACE_MSG(MMIMMS_EDITWIN_4877_112_2_18_2_37_48_46,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo")
TRACE_MSG(MMIMMS_EDITWIN_4983_112_2_18_2_37_48_47,"MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_6210_112_2_18_2_37_51_48,"mms add contact type max: return")
TRACE_MSG(MMIMMS_EDITWIN_6268_112_2_18_2_37_51_49,"malloc all_number_wstr failed")
TRACE_MSG(MMIMMS_EDITWIN_6317_112_2_18_2_37_51_50,"malloc email_addr failed")
TRACE_MSG(MMIMMS_EDITWIN_6343_112_2_18_2_37_51_51,"HandleMMSEditToListWinMsg,tele_len = %d")
TRACE_MSG(MMIMMS_EDITWIN_6355_112_2_18_2_37_51_52,"warning_ tele_len 0")
TRACE_MSG(MMIMMS_EDITWIN_6469_112_2_18_2_37_51_53,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo")
TRACE_MSG(MMIMMS_EDITWIN_6586_112_2_18_2_37_51_54,"MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_6662_112_2_18_2_37_52_55,"HandleMMSEditMainMenuWin MSG_GET_FOCUS")
TRACE_MSG(MMIMMS_EDITWIN_7374_112_2_18_2_37_54_56,"malloc all_number_wstr failed")
TRACE_MSG(MMIMMS_EDITWIN_7391_112_2_18_2_37_54_57,"malloc email_addr failed")
TRACE_MSG(MMIMMS_EDITWIN_7415_112_2_18_2_37_54_58,"HandleMMSEditToListWinMsg,tele_len = %d")
TRACE_MSG(MMIMMS_EDITWIN_7487_112_2_18_2_37_54_59,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!")
TRACE_MSG(MMIMMS_EDITWIN_7492_112_2_18_2_37_54_60,"warning_ tele_len 0")
TRACE_MSG(MMIMMS_EDITWIN_7534_112_2_18_2_37_54_61,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo")
TRACE_MSG(MMIMMS_EDITWIN_7698_112_2_18_2_37_54_62,"MMIMMS: HandleMMSEditAddMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_7868_112_2_18_2_37_55_63,"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_7985_112_2_18_2_37_55_64,"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_8100_112_2_18_2_37_55_65,"MMIMMS: HandleMMSEditMainMenuOptWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_8124_112_2_18_2_37_55_66,"MMISMS: HandleEditMenuWinMsg call MMIAPIPB_OpenNumberList")
TRACE_MSG(MMIMMS_EDITWIN_9003_112_2_18_2_37_57_67,"MMIMMS: HandleMMSEditToListWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_9089_112_2_18_2_37_57_68,"malloc email_addr failed")
TRACE_MSG(MMIMMS_EDITWIN_9225_112_2_18_2_37_58_69,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo")
TRACE_MSG(MMIMMS_EDITWIN_9402_112_2_18_2_37_58_70,"malloc email_addr failed")
TRACE_MSG(MMIMMS_EDITWIN_9538_112_2_18_2_37_58_71,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo")
TRACE_MSG(MMIMMS_EDITWIN_9654_112_2_18_2_37_59_72,"malloc all_number_wstr failed")
TRACE_MSG(MMIMMS_EDITWIN_9666_112_2_18_2_37_59_73,"to num is full MMIMMS_MAX_RECEIVER_NUM")
TRACE_MSG(MMIMMS_EDITWIN_9676_112_2_18_2_37_59_74,"malloc num_str_ptr failed")
TRACE_MSG(MMIMMS_EDITWIN_9709_112_2_18_2_37_59_75,"malloc email_addr failed")
TRACE_MSG(MMIMMS_EDITWIN_9735_112_2_18_2_37_59_76,"HandleMMSEditToListWinMsg,tele_len = %d")
TRACE_MSG(MMIMMS_EDITWIN_9747_112_2_18_2_37_59_77,"warning_ tele_len 0")
TRACE_MSG(MMIMMS_EDITWIN_9938_112_2_18_2_37_59_78,"MMIMMS: HandleMMSEditSubjectWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_10040_112_2_18_2_37_59_79,"MMIMMS: HandleMMSEditTextWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_10233_112_2_18_2_38_0_80,"MMIMMS: HandleMMSEditSetSlideTimeWin msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_10354_112_2_18_2_38_0_81,"HandleSIMSelectSendWinMsg dual_sys = %d")
TRACE_MSG(MMIMMS_EDITWIN_10369_112_2_18_2_38_0_82,"HandleSIMSelectSendWinMsg record id = %d")
TRACE_MSG(MMIMMS_EDITWIN_10376_112_2_18_2_38_0_83,"HandleSIMSelectSendWinMsg : error record id")
TRACE_MSG(MMIMMS_EDITWIN_10384_112_2_18_2_38_0_84,"HandleSIMSelectSendWinMsg MMIMMS_GetMMSSendStatus error!")
TRACE_MSG(MMIMMS_EDITWIN_10411_112_2_18_2_38_0_85,"HandleSIMSelectSendWinMsg : exit error")
TRACE_MSG(MMIMMS_EDITWIN_10422_112_2_18_2_38_0_86,"HandleSIMSelectSendWinMsg : list")
TRACE_MSG(MMIMMS_EDITWIN_10491_112_2_18_2_38_0_87,"HandleSIMSelectSendWinMsg dual_sys = %d")
TRACE_MSG(MMIMMS_EDITWIN_10501_112_2_18_2_38_0_88,"HandleSIMSelectSendWinMsg record id = %d")
TRACE_MSG(MMIMMS_EDITWIN_10508_112_2_18_2_38_0_89,"HandleSIMSelectSendWinMsg : error record id")
TRACE_MSG(MMIMMS_EDITWIN_10516_112_2_18_2_38_0_90,"HandleSIMSelectSendWinMsg MMIMMS_GetMMSSendStatus error!")
TRACE_MSG(MMIMMS_EDITWIN_10542_112_2_18_2_38_0_91,"HandleSIMSelectSendWinMsg : exit error")
TRACE_MSG(MMIMMS_EDITWIN_10553_112_2_18_2_38_0_92,"HandleSIMSelectSendWinMsg : list")
TRACE_MSG(MMIMMS_EDITWIN_10577_112_2_18_2_38_0_93,"MMIMMS: HandleAttachmenTypeWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EDITWIN_10920_112_2_18_2_38_1_94,"MMSEditAddToItem %d")
TRACE_MSG(MMIMMS_EDITWIN_10998_112_2_18_2_38_1_95,"MMSEditAddToItem %d")
TRACE_MSG(MMIMMS_EDITWIN_11158_112_2_18_2_38_2_96,"MMSEditAddPicItem drm file ")
TRACE_MSG(MMIMMS_EDITWIN_11171_112_2_18_2_38_2_97,"MMSEditAddPicItem")
TRACE_MSG(MMIMMS_EDITWIN_11251_112_2_18_2_38_2_98,"MMSEditAddPicItem drm file ")
TRACE_MSG(MMIMMS_EDITWIN_11263_112_2_18_2_38_2_99,"MMSEditAddPicItem")
TRACE_MSG(MMIMMS_EDITWIN_11312_112_2_18_2_38_2_100,"MMSEditAddPicItem jackal12")
TRACE_MSG(MMIMMS_EDITWIN_11322_112_2_18_2_38_2_101,"MMSEditAddPicItem jackal13")
TRACE_MSG(MMIMMS_EDITWIN_11402_112_2_18_2_38_2_102,"MMSEditAddMusicItem: no audio")
TRACE_MSG(MMIMMS_EDITWIN_12434_112_2_18_2_38_4_103,"can not insert again")
TRACE_MSG(MMIMMS_EDITWIN_13038_112_2_18_2_38_5_104,"malloc email_addr failed")
TRACE_MSG(MMIMMS_EDITWIN_13155_112_2_18_2_38_6_105,"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo")
TRACE_MSG(MMIMMS_EDITWIN_13355_112_2_18_2_38_6_106,"MMIMMS: MMIMMS_SendNewMMS send_result = %d, record_id = %d")
TRACE_MSG(MMIMMS_EDITWIN_13377_112_2_18_2_38_6_107,"EditAndSendNewMMS : exit error")
TRACE_MSG(MMIMMS_EDITWIN_13900_112_2_18_2_38_7_108,"FNH total_size = %ld")
TRACE_MSG(MMIMMS_EDITWIN_14765_112_2_18_2_38_9_109,"MMS edit doc ptr is PNULL")
TRACE_MSG(MMIMMS_EDITWIN_15660_112_2_18_2_38_11_110,"mms alloc failed ")
TRACE_MSG(MMIMMS_EDITWIN_15664_112_2_18_2_38_11_111,"MMSEditAddToItem %d")
TRACE_MSG(MMIMMS_EDITWIN_15739_112_2_18_2_38_11_112,"mms alloc failed ")
TRACE_MSG(MMIMMS_EDITWIN_15743_112_2_18_2_38_11_113,"MMSEditAddToItem %d")
TRACE_MSG(MMIMMS_EDITWIN_15924_112_2_18_2_38_11_114,"MMS alloc error")
TRACE_MSG(MMIMMS_EDITWIN_15975_112_2_18_2_38_11_115,"MMS alloc error")
TRACE_MSG(MMIMMS_EDITWIN_16096_112_2_18_2_38_11_116,"MMIAPIMMS_EditNewMMS : edit document alloc failed")
TRACE_MSG(MMIMMS_EDITWIN_16195_112_2_18_2_38_12_117,"MMS malloc failed ")
TRACE_MSG(MMIMMS_EDITWIN_16282_112_2_18_2_38_12_118,"MMS malloc failed ")
TRACE_MSG(MMIMMS_EDITWIN_16385_112_2_18_2_38_12_119,"MMS malloc failed ")
TRACE_MSG(MMIMMS_EDITWIN_17019_112_2_18_2_38_13_120,"MMS sms get number error")
TRACE_MSG(MMIMMS_EDITWIN_17095_112_2_18_2_38_13_121,"MMISMS: GetNumberFromPb msg_id = 0x%x, count = %d")
TRACE_MSG(MMIMMS_EDITWIN_17302_112_2_18_2_38_14_122,"MMS file type error")
TRACE_MSG(MMIMMS_EDITWIN_17306_112_2_18_2_38_14_123,"MMS file type error %d")
TRACE_MSG(MMIMMS_EDITWIN_17345_112_2_18_2_38_14_124,"MMS image is null")
TRACE_MSG(MMIMMS_EDITWIN_17427_112_2_18_2_38_14_125,"mms dc error %d")
TRACE_MSG(MMIMMS_EDITWIN_17451_112_2_18_2_38_14_126,"mms dc error %d")
TRACE_MSG(MMIMMS_EDITWIN_17473_112_2_18_2_38_14_127,"[MMIMMS]MMSOpenRecord mms record error!")
TRACE_MSG(MMIMMS_EXPORT_94_112_2_18_2_38_15_128,"[MMIMMS]: HandleSIMPlugNotifyMMSFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMIMMS_EXPORT_119_112_2_18_2_38_15_129,"[MMIMMS]:MMIAPIMMS_AppInit")
TRACE_MSG(MMIMMS_EXPORT_155_112_2_18_2_38_15_130,"[MMIMMS]:MMIAPIMMS_ListInit")
TRACE_MSG(MMIMMS_EXPORT_390_112_2_18_2_38_16_131,"MMIAPIMMS_EditNewMMS : edit document alloc failed")
TRACE_MSG(MMIMMS_EXPORT_415_112_2_18_2_38_16_132,"MMIAPIMMS_AnswerMMS : answer")
TRACE_MSG(MMIMMS_EXPORT_470_112_2_18_2_38_16_133,"MMIAPIMMS_AnswerMMS : edit document alloc failed")
TRACE_MSG(MMIMMS_EXPORT_544_112_2_18_2_38_16_134,"MMIAPIMMS_BrwSendUrlByMMS : edit document alloc failed")
TRACE_MSG(MMIMMS_EXPORT_599_112_2_18_2_38_16_135,"MMIAPIMMS_DraftEditMMS : edit document alloc failed")
TRACE_MSG(MMIMMS_EXPORT_733_112_2_18_2_38_16_136,"MMIAPIMMS_FarwardMMS : edit document alloc failed")
TRACE_MSG(MMIMMS_EXPORT_814_112_2_18_2_38_16_137,"MMIAPIMMS_ReadMMS : edit document alloc failed")
TRACE_MSG(MMIMMS_EXPORT_871_112_2_18_2_38_17_138,"MMIAPIMMS_OutboxSendMMS : edit document read failed")
TRACE_MSG(MMIMMS_EXPORT_1003_112_2_18_2_38_17_139,"MMIMMS: HandleMMSPicInsertQueryWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_EXPORT_1429_112_2_18_2_38_18_140,"[MMIMMS]:MMIAPIMMS_RestartAutodownMMS!")
TRACE_MSG(MMIMMS_EXPORT_1591_112_2_18_2_38_18_141,"MMS edit init failed")
TRACE_MSG(MMIMMS_EXPORT_1807_112_2_18_2_38_18_142,"FNH total_size = %ld")
TRACE_MSG(MMIMMS_FILE_233_112_2_18_2_38_20_143,"[MMIMMS]:AddListItem error record_id==0")
TRACE_MSG(MMIMMS_FILE_243_112_2_18_2_38_20_144,"[MMIMMS]:AddListItem PNULL == list_item_ptr")
TRACE_MSG(MMIMMS_FILE_305_112_2_18_2_38_20_145,"[MMIMMS]:AddListItem PNULL == list_item_ptr")
TRACE_MSG(MMIMMS_FILE_332_112_2_18_2_38_20_146,"[MMIMMS]:AddListItem PNULL == list_item_ptr")
TRACE_MSG(MMIMMS_FILE_614_112_2_18_2_38_20_147,"[MMIMMS]:MMIMMS_AddAutoList PNULL == list_ptr")
TRACE_MSG(MMIMMS_FILE_650_112_2_18_2_38_20_148,"[MMIMMS]:MMIMMS_AddManualList record_id=%d")
TRACE_MSG(MMIMMS_FILE_659_112_2_18_2_38_20_149,"[MMIMMS]:MMIMMS_AddManualList PNULL == list_ptr")
TRACE_MSG(MMIMMS_FILE_858_112_2_18_2_38_21_150,"[MMIMMS]:MMIMMS_ReadMMSListFile read_size=%d")
TRACE_MSG(MMIMMS_FILE_869_112_2_18_2_38_21_151,"[MMIMMS]:MMIMMS_ReadMMSListFile mms version error. clean all...")
TRACE_MSG(MMIMMS_FILE_901_112_2_18_2_38_21_152,"[MMIMMS]:MMIMMS_ReadMMSListFile PNULL == buffer_ptr")
TRACE_MSG(MMIMMS_FILE_921_112_2_18_2_38_21_153,"[MMIMMS]:MMIMMS_ReadMMSListFile mms list error. clean all...")
TRACE_MSG(MMIMMS_FILE_1037_112_2_18_2_38_21_154,"[MMIMMS]: MMIMMS_ReadPush malloc mem failed")
TRACE_MSG(MMIMMS_FILE_1056_112_2_18_2_38_21_155,"[MMIMMS]: MMIMMS_ReadPush file is not exist!")
TRACE_MSG(MMIMMS_FILE_1062_112_2_18_2_38_21_156,"[MMIMMS]: MMIMMS_ReadPush fetal error record_id==0!")
TRACE_MSG(MMIMMS_FILE_1112_112_2_18_2_38_21_157,"[MMIMMS]:MMIMMS_ReadMMS record_id=%d,file_size=%d")
TRACE_MSG(MMIMMS_FILE_1169_112_2_18_2_38_21_158,"[MMIMMS]:MMIMMS_ReadMMS buffer_ptr alloc fail!")
TRACE_MSG(MMIMMS_FILE_1174_112_2_18_2_38_21_159,"[MMIMMS]:MMIMMS_ReadMMS file is wrong or not exist!")
TRACE_MSG(MMIMMS_FILE_1326_112_2_18_2_38_22_160,"[MMIMMS]:IntegrateMMS record_id=%d")
TRACE_MSG(MMIMMS_FILE_1335_112_2_18_2_38_22_161,"[MMIMMS]:IntegrateMMS MMS_SetMmsFile failed")
TRACE_MSG(MMIMMS_FILE_1413_112_2_18_2_38_22_162,"[MMIMMS]:IntegrateMMS MMS_EncodeMms failed")
TRACE_MSG(MMIMMS_FILE_1726_112_2_18_2_38_23_163,"[MMIMMS]:%x %x %x %x %x %x %x %x")
TRACE_MSG(MMIMMS_FILE_1739_112_2_18_2_38_23_164,"[MMIMMS]:MMIMMS_SaveMMSListFile PNULL == buffer_ptr")
TRACE_MSG(MMIMMS_FILE_1757_112_2_18_2_38_23_165,"[MMIMMS]:%x %x %x %x %x %x %x %x")
TRACE_MSG(MMIMMS_FILE_1761_112_2_18_2_38_23_166,"[MMIMMS]:MMIMMS_SaveMMSListFile FAIL, fetal error!")
TRACE_MSG(MMIMMS_FILE_1814_112_2_18_2_38_23_167,"[MMIMMS]:MMIMMS_SavePush mms sender is in black lists!")
TRACE_MSG(MMIMMS_FILE_1820_112_2_18_2_38_23_168,"[MMIMMS]:MMIMMS_SavePush num_len is d%!")
TRACE_MSG(MMIMMS_FILE_1825_112_2_18_2_38_23_169,"[MMIMMS]:MMIMMS_SavePush msg_ptr->phone_number is PNULL!")
TRACE_MSG(MMIMMS_FILE_1844_112_2_18_2_38_23_170,"[MMIMMS]:MMIMMS_SavePush not enough space for list!")
TRACE_MSG(MMIMMS_FILE_1851_112_2_18_2_38_23_171,"[MMIMMS]:MMIMMS_SavePush read push error alloc failed!")
TRACE_MSG(MMIMMS_FILE_1872_112_2_18_2_38_23_172,"[MMIMMS]: MMIMMS_SavePush record_id=%d")
TRACE_MSG(MMIMMS_FILE_1876_112_2_18_2_38_23_173,"[MMIMMS]: %x %x %x %x %x %x %x %x ")
TRACE_MSG(MMIMMS_FILE_1914_112_2_18_2_38_23_174,"[MMIMMS]: MMIMMS_SavePush error")
TRACE_MSG(MMIMMS_FILE_1960_112_2_18_2_38_23_175,"[MMIMMS]:MMIMMS_SaveMMS save mms in")
TRACE_MSG(MMIMMS_FILE_1969_112_2_18_2_38_23_176,"[MMIMMS]:MMIMMS_SaveMMS no space is_record_exist=%d,is_maxnum=%d,is_enough_space=%d")
TRACE_MSG(MMIMMS_FILE_2022_112_2_18_2_38_23_177,"MMSEditAddToItem %d")
TRACE_MSG(MMIMMS_FILE_2090_112_2_18_2_38_23_178,"[MMIMMS]:MMIMMS_SaveMMS record_id=%d")
TRACE_MSG(MMIMMS_FILE_2103_112_2_18_2_38_23_179,"[MMIMMS]:MMIMMS_SaveMMS write mms file error")
TRACE_MSG(MMIMMS_FILE_2110_112_2_18_2_38_23_180,"[MMIMMS]:MMIMMS_SaveMMS save mms list error")
TRACE_MSG(MMIMMS_FILE_2118_112_2_18_2_38_23_181,"[MMIMMS]:MMIMMS_SaveMMS no space")
TRACE_MSG(MMIMMS_FILE_2128_112_2_18_2_38_23_182,"[MMIMMS]:MMIMMS_SaveMMS encode error")
TRACE_MSG(MMIMMS_FILE_2163_112_2_18_2_38_23_183,"[MMIMMS]:MMIMMS_SaveMMSContent is_text = %d")
TRACE_MSG(MMIMMS_FILE_2174_112_2_18_2_38_23_184,"[MMIMMS]:MMIMMS_SaveMMSContent no space")
TRACE_MSG(MMIMMS_FILE_2225_112_2_18_2_38_24_185,"[MMIMMS]:MMIMMS_SaveMMSContent buffer_len = %d")
TRACE_MSG(MMIMMS_FILE_2446_112_2_18_2_38_24_186,"[MMIMMS]:MMIMMS_InitBlock")
TRACE_MSG(MMIMMS_FILE_2459_112_2_18_2_38_24_187,"[MMIMMS]:MMIMMS_InitBlock call MMIAPIMEM_Config fail!")
TRACE_MSG(MMIMMS_FILE_2466_112_2_18_2_38_24_188,"[MMIMMS]:MMIMMS_InitBlock INVALID_MEM_HANDLE != s_mms_mem_handle")
TRACE_MSG(MMIMMS_FILE_2489_112_2_18_2_38_24_189,"[MMIMMS]:malloc error:size == 0")
TRACE_MSG(MMIMMS_FILE_2492_112_2_18_2_38_24_190,"[MMIMMS]: malloc size= %d")
TRACE_MSG(MMIMMS_FILE_2518_112_2_18_2_38_24_191,"[BRW]error:malloc failed,size = %d")
TRACE_MSG(MMIMMS_FILE_2538_112_2_18_2_38_24_192,"MMIMMS_DestroyBlock, handle is null!")
TRACE_MSG(MMIMMS_FILE_2569_112_2_18_2_38_24_193,"[MMIMMS]:MMIMMS_FREE ERR:memory leak!")
TRACE_MSG(MMIMMS_MAIN_NEW_219_112_2_18_2_38_25_194,"MMIMMS_CheckIfEntryWinAllOpen,%d opened")
TRACE_MSG(MMIMMS_MAIN_NEW_426_112_2_18_2_38_26_195,"MMIMMS_RecvAndDownloadMMS error type nor delivery or mms noti")
TRACE_MSG(MMIMMS_MAIN_NEW_487_112_2_18_2_38_26_196,"MMIMMS_NewMMSOrNotiMsg p:%d, s:%d")
TRACE_MSG(MMIMMS_MAIN_NEW_571_112_2_18_2_38_26_197,"MMIMMS_DetailAndDownloadMMS: call recv func fail!")
TRACE_MSG(MMIMMS_MAIN_NEW_593_112_2_18_2_38_26_198,"[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d")
TRACE_MSG(MMIMMS_MAIN_NEW_656_112_2_18_2_38_26_199,"MMS: PlayMMSRing normal call mode")
TRACE_MSG(MMIMMS_MAIN_NEW_661_112_2_18_2_38_26_200,"MMS: MMIMMS_PlayMMSSound ring_type=%d")
TRACE_MSG(MMIMMS_MAIN_NEW_778_112_2_18_2_38_26_201,"[MMIMMS]:MMIMMS_HandleControlMsg PDP Active Success!")
TRACE_MSG(MMIMMS_MAIN_NEW_783_112_2_18_2_38_26_202,"[MMIMMS]:MMIMMS_HandleControlMsg reply the recv mms!")
TRACE_MSG(MMIMMS_MAIN_NEW_822_112_2_18_2_38_26_203,"[MMIMMS]MSG_MMS_DATA_RECEIVED:download size cur_size=%d, total_size=%d")
TRACE_MSG(MMIMMS_MAIN_NEW_843_112_2_18_2_38_26_204,"[MMIMMS]:MMIMMS_HandleControlMsg recv error = %d")
TRACE_MSG(MMIMMS_MAIN_NEW_899_112_2_18_2_38_27_205,"[MMIMMS]MSG_MMS_DATA_SENT:temp_progress_ptr is NULL")
TRACE_MSG(MMIMMS_MAIN_NEW_908_112_2_18_2_38_27_206,"[MMIMMS]MSG_MMS_DATA_SENT:send size cur_size=%d, total_size=%d")
TRACE_MSG(MMIMMS_MAIN_NEW_929_112_2_18_2_38_27_207,"[MMIMMS]MSG_MMS_SEND_END error id = %d, is_saved = %d")
TRACE_MSG(MMIMMS_MAIN_NEW_998_112_2_18_2_38_27_208,"[MMIMMS]:MMIMMS_HandleControlMsg PDP deactive!")
TRACE_MSG(MMIMMS_MAIN_NEW_1044_112_2_18_2_38_27_209,"[MMIMMS]:MMIMMS_SetMMSMemState PNULL == item_data index")
TRACE_MSG(MMIMMS_MAIN_NEW_1062_112_2_18_2_38_27_210,"[MMIMMS]:MMIMMS_SetMMSMemState PNULL == item_data index")
TRACE_MSG(MMIMMS_MAIN_NEW_1102_112_2_18_2_38_27_211,"[MMIMMS]:AddMmsMemStateItems PNULL == item_data index")
TRACE_MSG(MMIMMS_MAIN_NEW_1134_112_2_18_2_38_27_212,"[MMIMMS]:AddMmsMemStateItems default")
TRACE_MSG(MMIMMS_MAIN_NEW_1167_112_2_18_2_38_27_213,"AddMmsMemStateItems used_size = %ld, total_size = %ld")
TRACE_MSG(MMIMMS_MAIN_NEW_1226_112_2_18_2_38_27_214,"[MMIMMS]:AddMmsMemStateItems PNULL == item_data index")
TRACE_MSG(MMIMMS_MAIN_NEW_1258_112_2_18_2_38_27_215,"[MMIMMS]:AddMmsMemStateItems default")
TRACE_MSG(MMIMMS_MAIN_NEW_1286_112_2_18_2_38_27_216,"AddMmsMemStateItems used_size = %ld, total_size = %ld")
TRACE_MSG(MMIMMS_MAIN_NEW_1386_112_2_18_2_38_28_217,"MMIMMS_DeliveryRpt_Arrive IN ")
TRACE_MSG(MMIMMS_MAIN_NEW_1397_112_2_18_2_38_28_218,"[MMIMMS]:MMIMMS_DeliveryRptArrive push_status is%d")
TRACE_MSG(MMIMMS_NET_181_112_2_18_2_38_30_219,"[MMIMMS]:MMSNetCallback status=%d, status_value=%d")
TRACE_MSG(MMIMMS_NET_210_112_2_18_2_38_30_220,"MMSNetCallback MMS_RECEIVING fetal error!")
TRACE_MSG(MMIMMS_NET_219_112_2_18_2_38_30_221,"MMSNetCallback MMS_SENDING fetal error!")
TRACE_MSG(MMIMMS_NET_232_112_2_18_2_38_30_222,"MMSNetCallback fetal error MMS_STATUS_SEND_RETURN MMIMMS_SENDRECV_SENDING != s_activemms_info.type")
TRACE_MSG(MMIMMS_NET_302_112_2_18_2_38_30_223,"MMSNetCallback, mms Send!")
TRACE_MSG(MMIMMS_NET_303_112_2_18_2_38_30_224,"MMSNetCallback, mms recv failed s_send_retry_pdp_time = %d")
TRACE_MSG(MMIMMS_NET_351_112_2_18_2_38_30_225,"MMSNetCallback fetal error MMS_STATUS_SEND_RETURN MMIMMS_SENDRECV_SENDING != s_activemms_info.type")
TRACE_MSG(MMIMMS_NET_375_112_2_18_2_38_30_226,"[MMIMMS]:MMSNetCallback MMIMMS_UpdateRecvItem fail")
TRACE_MSG(MMIMMS_NET_455_112_2_18_2_38_30_227,"MMSNetCallback, mms recv!")
TRACE_MSG(MMIMMS_NET_473_112_2_18_2_38_30_228,"MMSNetCallback, mms recv failed or succ")
TRACE_MSG(MMIMMS_NET_502_112_2_18_2_38_31_229,"MMSNetCallback, mms recv failed s_recv_retry_time = %d")
TRACE_MSG(MMIMMS_NET_520_112_2_18_2_38_31_230,"MMSNetCallback, mms recv failed retry download mms")
TRACE_MSG(MMIMMS_NET_543_112_2_18_2_38_31_231,"MMSNetCallback,  current = %d")
TRACE_MSG(MMIMMS_NET_547_112_2_18_2_38_31_232,"MMSNetCallback, total = %d, current = %d")
TRACE_MSG(MMIMMS_NET_563_112_2_18_2_38_31_233,"MMSNetCallback, prompt = %d, total = %d")
TRACE_MSG(MMIMMS_NET_582_112_2_18_2_38_31_234,"MMSNetCallback, prompt = %d, total = %d")
TRACE_MSG(MMIMMS_NET_635_112_2_18_2_38_31_235,"[MMIMMS]:SendOldMMS PNULL == activemms_info_ptr")
TRACE_MSG(MMIMMS_NET_645_112_2_18_2_38_31_236,"[MMIMMS]:SendOldMMS call pdp active function return true")
TRACE_MSG(MMIMMS_NET_649_112_2_18_2_38_31_237,"[MMIMMS]:SendOldMMS PDP active fail")
TRACE_MSG(MMIMMS_NET_716_112_2_18_2_38_31_238,"[MMIMMS]:ReceiveMMS call pdp active function return true")
TRACE_MSG(MMIMMS_NET_720_112_2_18_2_38_31_239,"[MMIMMS]:ReceiveMMS PDP active fail")
TRACE_MSG(MMIMMS_NET_727_112_2_18_2_38_31_240,"[MMIMMS]:ReceiveMMS url is wrong!")
TRACE_MSG(MMIMMS_NET_745_112_2_18_2_38_31_241,"[MMIMMS]:MMIMMS_ActiveRecvSendModule status=%d")
TRACE_MSG(MMIMMS_NET_755_112_2_18_2_38_31_242,"[MMIMMS]:MMIMMS_ActiveRecvSendModule pdp deactive different sim card")
TRACE_MSG(MMIMMS_NET_760_112_2_18_2_38_31_243,"[MMIMMS]:MMIMMS_ActiveRecvSendModule manual_list MMIMMS_PDPDeactive")
TRACE_MSG(MMIMMS_NET_785_112_2_18_2_38_31_244,"[MMIMMS]:MMIMMS_ActiveRecvSendModule pdp deactive different sim card")
TRACE_MSG(MMIMMS_NET_790_112_2_18_2_38_31_245,"[MMIMMS]:MMIMMS_ActiveRecvSendModule auto_list MMIMMS_PDPDeactive")
TRACE_MSG(MMIMMS_NET_809_112_2_18_2_38_31_246,"[MMIMMS]:MMIMMS_ActiveRecvSendModule ACTIVING")
TRACE_MSG(MMIMMS_NET_838_112_2_18_2_38_31_247,"[MMIMMS]:MMIMMS_SendNewMMS MMIMMS_ERROR_NO_CONTENT")
TRACE_MSG(MMIMMS_NET_852_112_2_18_2_38_32_248,"[MMIMMS]: MMIMMS_SendNewMMS add manual list fail")
TRACE_MSG(MMIMMS_NET_877_112_2_18_2_38_32_249,"[MMIMMS]: MMIMMS_SendOldMMS add manual list fail")
TRACE_MSG(MMIMMS_NET_902_112_2_18_2_38_32_250,"[MMIMMS]:MMIMMS_SendReadReport reportdoc_ptr=%d")
TRACE_MSG(MMIMMS_NET_918_112_2_18_2_38_32_251,"[MMIMMS]: MMIMMS_SendReadReport add manual list fail")
TRACE_MSG(MMIMMS_NET_946_112_2_18_2_38_32_252,"[MMIMMS]: MMIMMS_ResponceMMS respValue=%d")
TRACE_MSG(MMIMMS_NET_990_112_2_18_2_38_32_253,"[MMIMMS]: MMIMMS_ResponceMMS add manual list fail")
TRACE_MSG(MMIMMS_NET_1027_112_2_18_2_38_32_254,"[MMIMMS]: MMIMMS_ResponceMMS add manual list fail")
TRACE_MSG(MMIMMS_NET_1087_112_2_18_2_38_32_255,"mmitest remove fname")
TRACE_MSG(MMIMMS_NET_1168_112_2_18_2_38_32_256,"[MMIMMS]:MMIMMS_PDPActive dual_sys=%d")
TRACE_MSG(MMIMMS_NET_1198_112_2_18_2_38_32_257,"[MMIMMS]:HandlePDPMsg msg_id=%d")
TRACE_MSG(MMIMMS_NET_1215_112_2_18_2_38_32_258,"HandlePDPMsg: MMIPDP_DEACTIVE_IND")
TRACE_MSG(MMIMMS_NET_1234_112_2_18_2_38_33_259,"MMIMMS_HandleNetTimer:recv timer")
TRACE_MSG(MMIMMS_NET_1242_112_2_18_2_38_33_260,"MMIMMS_HandleNetTimer:recv timer pdp permit not!")
TRACE_MSG(MMIMMS_NET_1257_112_2_18_2_38_33_261,"MMIMMS_HandleNetTimer:retry recv timer! detach pdp")
TRACE_MSG(MMIMMS_NET_1286_112_2_18_2_38_33_262,"[MMIMMS]:MMIMMS_IsPdpPermitUsing net_status = %d")
TRACE_MSG(MMIMMS_NET_1335_112_2_18_2_38_33_263,"[MMIMMS]:HandlePDPActiveCnf")
TRACE_MSG(MMIMMS_NET_1340_112_2_18_2_38_33_264,"[MMIMMS]:HandlePDPActiveCnf msg_ptr->result=%d")
TRACE_MSG(MMIMMS_NET_1414_112_2_18_2_38_33_265,"[MMIMMS]:HandlePDPActiveCnf alloc mms_center_ptr failed")
TRACE_MSG(MMIMMS_NET_1443_112_2_18_2_38_33_266,"[MMIMMS]:HandlePDPActiveCnf alloc mms_center_ptr failed")
TRACE_MSG(MMIMMS_NET_1498_112_2_18_2_38_33_267,"[MMIMMS]:HandlePDPActiveCnf s_activemms_info.type=%d")
TRACE_MSG(MMIMMS_NET_1506_112_2_18_2_38_33_268,"[MMIMMS]:HandlePDPActiveCnf alloc fname failed")
TRACE_MSG(MMIMMS_NET_1539_112_2_18_2_38_33_269,"MMIMMS]:MMIMMS_SendNewMMS mms send failed")
TRACE_MSG(MMIMMS_NET_1546_112_2_18_2_38_33_270,"[MMIMMS]:MMIMMS_SendNewMMS MMS_SetConfig(&cfgset) failed")
TRACE_MSG(MMIMMS_NET_1559_112_2_18_2_38_33_271,"[MMIMMS]:HandlePDPActiveCnf alloc fname failed")
TRACE_MSG(MMIMMS_NET_1603_112_2_18_2_38_33_272,"[MMIMMS]:MMIMMS_ReceiveMMS immediate_recv=%d")
TRACE_MSG(MMIMMS_NET_1620_112_2_18_2_38_33_273,"MMIMMS]:MMIMMS_ReceiveMMS mms recv failed")
TRACE_MSG(MMIMMS_NET_1626_112_2_18_2_38_33_274,"[MMIMMS]:MMIMMS_ReceiveMMS MMS_SetConfig(&cfgset) failed")
TRACE_MSG(MMIMMS_NET_1632_112_2_18_2_38_33_275,"[MMIMMS]:HandlePDPActiveCnf recv no space")
TRACE_MSG(MMIMMS_NET_1638_112_2_18_2_38_33_276,"[MMIMMS]:MMIMMS_ReceiveMMS alloc pushfile failed")
TRACE_MSG(MMIMMS_NET_1653_112_2_18_2_38_33_277,"[MMIMMS]:HandlePDPActiveCnf s_activemms_info.type= %d  WRONG")
TRACE_MSG(MMIMMS_PREVIEWWIN_833_112_2_18_2_38_36_278,"HandleMMSPreviewWinMsg msg_id=%x,cur_edoc=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_855_112_2_18_2_38_36_279,"HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_891_112_2_18_2_38_36_280,"HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1131_112_2_18_2_38_36_281,"HandleMMSPreviewWinMsg playing video! MSG_FULL_PAINT")
TRACE_MSG(MMIMMS_PREVIEWWIN_1181_112_2_18_2_38_36_282,"HandleMMSPreviewWinMsg play video")
TRACE_MSG(MMIMMS_PREVIEWWIN_1219_112_2_18_2_38_37_283,"HandleMMSPreviewWinMsg playing video! MSG_MMS_PLAY_SLIDE")
TRACE_MSG(MMIMMS_PREVIEWWIN_1244_112_2_18_2_38_37_284,"HandleMMSPreviewWinMsg vedio_time=%d,duaration=%d, music_time= %d")
TRACE_MSG(MMIMMS_PREVIEWWIN_1323_112_2_18_2_38_37_285,"HandleMMSPreviewWinMsg playing video! MSG_GET_FOCUS")
TRACE_MSG(MMIMMS_PREVIEWWIN_1384_112_2_18_2_38_37_286,"HandleMMSPreviewWinMsg playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1541_112_2_18_2_38_37_287,"timer:playing state!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1549_112_2_18_2_38_37_288,"HandleMMSPreviewWinMsg playing video! MSG_CTL_RICHTEXT_SWITCHFOCUS")
TRACE_MSG(MMIMMS_PREVIEWWIN_1605_112_2_18_2_38_37_289,"HandleMMSPreviewWinMsg 1 mms_list_info_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1611_112_2_18_2_38_37_290,"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1621_112_2_18_2_38_37_291,"HandleMMSPreviewWinMsg number_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1663_112_2_18_2_38_37_292,"HandleMMSPreviewWinMsg 2 mms_list_info_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1729_112_2_18_2_38_38_293,"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1746_112_2_18_2_38_38_294,"HandleMMSPreviewWinMsg focus_url_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1758_112_2_18_2_38_38_295,"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1847_112_2_18_2_38_38_296,"s_edit_mms_preview_state=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_1852_112_2_18_2_38_38_297,"focus item type error!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1910_112_2_18_2_38_38_298,"unkown mms type")
TRACE_MSG(MMIMMS_PREVIEWWIN_1915_112_2_18_2_38_38_299,"HandleMMSPreviewWinMsg : mms_list_info_ptr is null")
TRACE_MSG(MMIMMS_PREVIEWWIN_1945_112_2_18_2_38_38_300,"HandleMMSPreviewWinMsg playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_1999_112_2_18_2_38_38_301,"HandleMMSPreviewWinMsg playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2081_112_2_18_2_38_38_302,"HandleMMSPreviewWinMsg msg_id=%x,cur_edoc=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_2103_112_2_18_2_38_38_303,"HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2140_112_2_18_2_38_38_304,"HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2358_112_2_18_2_38_39_305,"HandleMMSPreviewWinMsg playing video! MSG_FULL_PAINT")
TRACE_MSG(MMIMMS_PREVIEWWIN_2405_112_2_18_2_38_39_306,"HandleMMSPreviewWinMsg play video")
TRACE_MSG(MMIMMS_PREVIEWWIN_2437_112_2_18_2_38_39_307,"HandleMMSPreviewWinMsg playing video! MSG_MMS_PLAY_SLIDE")
TRACE_MSG(MMIMMS_PREVIEWWIN_2462_112_2_18_2_38_39_308,"HandleMMSPreviewWinMsg vedio_time=%d,duaration=%d, music_time= %d")
TRACE_MSG(MMIMMS_PREVIEWWIN_2529_112_2_18_2_38_39_309,"HandleMMSPreviewWinMsg playing video! MSG_GET_FOCUS")
TRACE_MSG(MMIMMS_PREVIEWWIN_2590_112_2_18_2_38_39_310,"HandleMMSPreviewWinMsg playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2705_112_2_18_2_38_39_311,"timer:playing state!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2713_112_2_18_2_38_39_312,"HandleMMSPreviewWinMsg playing video! MSG_CTL_RICHTEXT_SWITCHFOCUS")
TRACE_MSG(MMIMMS_PREVIEWWIN_2769_112_2_18_2_38_40_313,"HandleMMSPreviewWinMsg 1 mms_list_info_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2775_112_2_18_2_38_40_314,"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2785_112_2_18_2_38_40_315,"HandleMMSPreviewWinMsg number_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2827_112_2_18_2_38_40_316,"HandleMMSPreviewWinMsg 2 mms_list_info_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2893_112_2_18_2_38_40_317,"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2910_112_2_18_2_38_40_318,"HandleMMSPreviewWinMsg focus_url_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_2922_112_2_18_2_38_40_319,"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!")
TRACE_MSG(MMIMMS_PREVIEWWIN_3005_112_2_18_2_38_40_320,"s_edit_mms_preview_state=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_3010_112_2_18_2_38_40_321,"focus item type error!")
TRACE_MSG(MMIMMS_PREVIEWWIN_3068_112_2_18_2_38_40_322,"unkown mms type")
TRACE_MSG(MMIMMS_PREVIEWWIN_3073_112_2_18_2_38_40_323,"HandleMMSPreviewWinMsg : mms_list_info_ptr is null")
TRACE_MSG(MMIMMS_PREVIEWWIN_3103_112_2_18_2_38_40_324,"HandleMMSPreviewWinMsg playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_3154_112_2_18_2_38_40_325,"HandleMMSPreviewWinMsg playing video!")
TRACE_MSG(MMIMMS_PREVIEWWIN_3229_112_2_18_2_38_41_326,"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_3395_112_2_18_2_38_41_327,"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_3433_112_2_18_2_38_41_328,"HandleMMSPreviewUrlPopWinMsg alloc failed")
TRACE_MSG(MMIMMS_PREVIEWWIN_3470_112_2_18_2_38_41_329,"HandleMMSPreviewUrlPopWinMsg BOOKMARK alloc failed")
TRACE_MSG(MMIMMS_PREVIEWWIN_3565_112_2_18_2_38_41_330,"MMIMMS: HandleMMSPreviewEmailPopWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_3689_112_2_18_2_38_41_331,"MMIMMS: HandleInboxMMSOptMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_4051_112_2_18_2_38_42_332,"MMIMMS TTS play text ok")
TRACE_MSG(MMIMMS_PREVIEWWIN_4073_112_2_18_2_38_42_333,"warning_ menu_id=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_4121_112_2_18_2_38_42_334,"warning_ menu_id=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_4182_112_2_18_2_38_42_335,"MMIMMS: HandleInboxMMSDetailOptMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_4240_112_2_18_2_38_43_336,"MMIMMS: HandleInboxMMSDetailOptMenuWinMsg active mms error!")
TRACE_MSG(MMIMMS_PREVIEWWIN_4988_112_2_18_2_38_44_337,"MMIMMS: HandleSendboxMMSOptMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_5282_112_2_18_2_38_45_338,"MMIMMS: HandleOutboxMMSOptMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_5467_112_2_18_2_38_45_339,"menu_id=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_5578_112_2_18_2_38_45_340,"MMIMMS: HandleSaveMultiFileWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_5902_112_2_18_2_38_46_341,"MMIMMS: HandleMultifileViewWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_6199_112_2_18_2_38_46_342,"not permit play ring!")
TRACE_MSG(MMIMMS_PREVIEWWIN_6331_112_2_18_2_38_47_343,"MMIMMS: item.text_data.buf.len = %d")
TRACE_MSG(MMIMMS_PREVIEWWIN_6332_112_2_18_2_38_47_344,"MMIMMS: ViewMultifile data %x %x %x %x")
TRACE_MSG(MMIMMS_PREVIEWWIN_6403_112_2_18_2_38_47_345,"warning_ AppendListItem")
TRACE_MSG(MMIMMS_PREVIEWWIN_6639_112_2_18_2_38_47_346,"warning catstring dst_ptr=%d,src1_ptr=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_6644_112_2_18_2_38_47_347,"warning catstring len1=%d,len2=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_6656_112_2_18_2_38_47_348,"MMIMMS_CatString: malloc for dst_ptr->str_ptr fail!")
TRACE_MSG(MMIMMS_PREVIEWWIN_7004_112_2_18_2_38_48_349,"MMIMMS: HandleSaveFileOptMenuWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_7119_112_2_18_2_38_48_350,"menu_id=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_7592_112_2_18_2_38_49_351,"MMIMMS: HandleInboxMMSDetailOptMenuWinMsg active mms error!")
TRACE_MSG(MMIMMS_PREVIEWWIN_7738_112_2_18_2_38_50_352,"MMIMMS: HandleInboxMMSNotiWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_7919_112_2_18_2_38_50_353,"MMIMMS: HandleInboxMMSNotiWinMsg active mms error!")
TRACE_MSG(MMIMMS_PREVIEWWIN_7965_112_2_18_2_38_50_354,"MMIMMS: HandleInboxMMSNotiWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_8094_112_2_18_2_38_50_355,"MMIMMS: HandleInboxMMSNotiWinMsg active mms error!")
TRACE_MSG(MMIMMS_PREVIEWWIN_8140_112_2_18_2_38_50_356,"MMIMMS: HandleSendReadRptWaitingWin msg_id = 0x%x")
TRACE_MSG(MMIMMS_PREVIEWWIN_8624_112_2_18_2_38_52_357,"MmsPreviewAddPicAndMovieItem drm file ")
TRACE_MSG(MMIMMS_PREVIEWWIN_8712_112_2_18_2_38_52_358,"MmsPreviewAddMusicItem fmt=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_8730_112_2_18_2_38_52_359,"not permit play ring!")
TRACE_MSG(MMIMMS_PREVIEWWIN_8738_112_2_18_2_38_52_360,"MmsPreviewAddMusicItem audio type error!")
TRACE_MSG(MMIMMS_PREVIEWWIN_8743_112_2_18_2_38_52_361,"MmsPreviewAddMusicItem data error!")
TRACE_MSG(MMIMMS_PREVIEWWIN_8783_112_2_18_2_38_52_362,"MmsPreviewPlayVideoItem video_type=%d, play_param.video_buf_info.video_buf_ptr = %x")
TRACE_MSG(MMIMMS_PREVIEWWIN_8799_112_2_18_2_38_52_363,"MmsPreviewPlayVideoItem video type error")
TRACE_MSG(MMIMMS_PREVIEWWIN_8902_112_2_18_2_38_52_364,"MmsPreviewPlayVideoItem data error")
TRACE_MSG(MMIMMS_PREVIEWWIN_8973_112_2_18_2_38_52_365,"MmsRingPlayCallBack result=%d")
TRACE_MSG(MMIMMS_PREVIEWWIN_9003_112_2_18_2_38_52_366,"[VP] MmsVideoPlayCallBack msg_ptr->msg_id = %x")
TRACE_MSG(MMIMMS_PREVIEWWIN_9047_112_2_18_2_38_52_367,"MmsVideoPlayCallBack ")
TRACE_MSG(MMIMMS_PUSH_297_112_2_18_2_38_54_368,"[MMIMMS]:HandleMMSPushCallback status = %d")
TRACE_MSG(MMIMMS_PUSH_304_112_2_18_2_38_54_369,"[MMIMMS]:HandleMMSPushCallback THE MSG->de_status is%d")
TRACE_MSG(MMIMMS_PUSH_322_112_2_18_2_38_54_370,"[MMIMMS]:HandleMMSPushCallback MMS_PUSH_TYPE_READORIG_IND")
TRACE_MSG(MMIMMS_PUSH_471_112_2_18_2_38_54_371,"[MMIMMS]:HandleMMSPushCallback error s_push_dual_sys >= MN_DUAL_SYS_MAX")
TRACE_MSG(MMIMMS_PUSH_480_112_2_18_2_38_54_372,"[MMIMMS]:HandleMMSPushCallback read push error alloc failed!")
TRACE_MSG(MMIMMS_PUSH_501_112_2_18_2_38_54_373,"[MMIMMS]:HandleMMSPushCallback read push error!")
TRACE_MSG(MMIMMS_PUSH_555_112_2_18_2_38_54_374,"[MMIMMS]:HandleMMSPushCallback push is in black lists")
TRACE_MSG(MMIMMS_PUSH_561_112_2_18_2_38_54_375,"[MMIMMS]:HandleMMSPushCallback push save fail")
TRACE_MSG(MMIMMS_PUSH_590_112_2_18_2_38_54_376,"[MMIMMS]:HandleMMSPushCallback retrieve manual mode")
TRACE_MSG(MMIMMS_PUSH_661_112_2_18_2_38_54_377,"MMIMMS_PushRecvSms dual_sys = %d")
TRACE_MSG(MMIMMS_PUSH_664_112_2_18_2_38_54_378,"MMICMS_PUSH_recv_data:not ready!")
TRACE_MSG(MMIMMS_PUSH_670_112_2_18_2_38_54_379,"s_mms_data MALLOC FAILED")
TRACE_MSG(MMIMMS_PUSH_681_112_2_18_2_38_54_380,"mmibrowser.c:MMICMS_PUSH_recv_data() iei_arr[%d].iei=%d")
TRACE_MSG(MMIMMS_PUSH_685_112_2_18_2_38_55_381,"it's a CONCAT push!")
TRACE_MSG(MMIMMS_PUSH_718_112_2_18_2_38_55_382,"message_id=%d,total_num=%d,index_id=%d")
TRACE_MSG(MMIMMS_PUSH_722_112_2_18_2_38_55_383,"MMIMMS_DecodePush:alloc push_item_ptr failed")
TRACE_MSG(MMIMMS_PUSH_752_112_2_18_2_38_55_384,"\"# sunlu MMIMMS MMIMMS_DecodePush sms_ap_id = %d")
TRACE_MSG(MMIMMS_PUSH_760_112_2_18_2_38_55_385,"mmibrowser.c:MMICMS_PUSH_recv_data() it is a MMS push")
TRACE_MSG(MMIMMS_PUSH_794_112_2_18_2_38_55_386,"\"# sunlu MMIMMS_DecodePush MMIMAIL_IsPushMailForContent return TRUE!!!!!")
TRACE_MSG(MMIMMS_PUSH_848_112_2_18_2_38_55_387,"the same PUSH,index_id=%d,time=%d")
TRACE_MSG(MMIMMS_PUSH_853_112_2_18_2_38_55_388,"message_num=%d,total_num=%d")
TRACE_MSG(MMIMMS_PUSH_893_112_2_18_2_38_55_389,"detail_index=%d")
TRACE_MSG(MMIMMS_PUSH_898_112_2_18_2_38_55_390,"write longpush file detail_index=%d,index_id=%d")
TRACE_MSG(MMIMMS_PUSH_908_112_2_18_2_38_55_391,"concact push,ok!")
TRACE_MSG(MMIMMS_PUSH_996_112_2_18_2_38_55_392,"HandleMMSPreviewWinMsg focus_num_ptr == s_long_push!")
TRACE_MSG(MMIMMS_SETTING_59_112_2_18_2_38_55_393,"[MMIMMS]:MMIMMS_ResetMMSFactorySetting")
TRACE_MSG(MMIMMS_SETTING_107_112_2_18_2_38_56_394,"[MMIMMS]:MMIMMS_InitSetting")
TRACE_MSG(MMIMMS_SETTING_133_112_2_18_2_38_56_395,"[MMIMMS]:MMIMMS_Setting_GetDeliveryReport deliver_report=%d")
TRACE_MSG(MMIMMS_SETTING_146_112_2_18_2_38_56_396,"[MMIMMS]:MMIMMS_Setting_SetDeliveryReport is_need_delivery_rpt_send=%d")
TRACE_MSG(MMIMMS_SETTING_171_112_2_18_2_38_56_397,"[MMIMMS]:MMIMMS_Setting_GetReadReport read_rpt=%d")
TRACE_MSG(MMIMMS_SETTING_184_112_2_18_2_38_56_398,"[MMIMMS]:MMIMMS_Setting_SetReadReport is_need_read_rpt_send=%d")
TRACE_MSG(MMIMMS_SETTING_209_112_2_18_2_38_56_399,"[MMIMMS]:MMIMMS_Setting_GetAnonymous is_anonymous=%d")
TRACE_MSG(MMIMMS_SETTING_222_112_2_18_2_38_56_400,"[MMIMMS]:MMIMMS_Setting_SetAnonymousReport is_anonymous=%d")
TRACE_MSG(MMIMMS_SETTING_247_112_2_18_2_38_56_401,"[MMIMMS]:MMIMMS_Setting_GetPlaysound is_play_sound=%d")
TRACE_MSG(MMIMMS_SETTING_260_112_2_18_2_38_56_402,"[MMIMMS]:MMIMMS_Setting_SetPlaysound is_play_sound=%d")
TRACE_MSG(MMIMMS_SETTING_280_112_2_18_2_38_56_403,"MMIMMS_Setting_GetAdvertising")
TRACE_MSG(MMIMMS_SETTING_299_112_2_18_2_38_56_404,"MMIMMS_Setting_SetAdvertising")
TRACE_MSG(MMIMMS_SETTING_325_112_2_18_2_38_56_405,"[MMIMMS]:MMIMMS_Setting_GetExtractContent is_extract_content=%d")
TRACE_MSG(MMIMMS_SETTING_338_112_2_18_2_38_56_406,"[MMIMMS]:MMIMMS_Setting_SetExtractContent is_extract_content=%d")
TRACE_MSG(MMIMMS_SETTING_363_112_2_18_2_38_56_407,"[MMIMMS]:MMIMMS_Setting_GetPriority priority=%d")
TRACE_MSG(MMIMMS_SETTING_376_112_2_18_2_38_56_408,"[MMIMMS]:MMIMMS_Setting_SetPriority priority=%d")
TRACE_MSG(MMIMMS_SETTING_402_112_2_18_2_38_56_409,"[MMIMMS]:MMIMMS_Setting_GetRetrieveMode retrieve_mode=%d")
TRACE_MSG(MMIMMS_SETTING_417_112_2_18_2_38_56_410,"[MMIMMS]:MMIMMS_Setting_SetRetrieveMode retrieve_mode=%d")
TRACE_MSG(MMIMMS_SETTING_443_112_2_18_2_38_56_411,"[MMIMMS]:MMIMMS_Setting_GetValidPeriod valid_period=%d")
TRACE_MSG(MMIMMS_SETTING_456_112_2_18_2_38_56_412,"[MMIMMS]:MMIMMS_Setting_SetValidPeriod valid_period=%d")
TRACE_MSG(MMIMMS_SETTING_481_112_2_18_2_38_56_413,"[MMIMMS]:MMIMMS_Setting_GetRecvDeliveryReport is_need_delivery_rpt_recv=%d")
TRACE_MSG(MMIMMS_SETTING_494_112_2_18_2_38_56_414,"[MMIMMS]:MMIMMS_Setting_SetRecvDeliveryReport is_need_delivery_rpt_recv=%d")
TRACE_MSG(MMIMMS_SETTING_514_112_2_18_2_38_56_415,"MMIMMS_Setting_GetPreferDisk")
TRACE_MSG(MMIMMS_SETTING_521_112_2_18_2_38_56_416,"[MMIMMS]:MMIMMS_Setting_GetPreferDisk device_type=%d")
TRACE_MSG(MMIMMS_SETTING_534_112_2_18_2_38_56_417,"[MMIMMS]:MMIMMS_Setting_SetPreferDisk device_type=%d")
TRACE_MSG(MMIMMS_SETTING_564_112_2_18_2_38_57_418,"[MMIMMS]:MMIMMS_Setting_GetNetSettingIndex,dual_sys=%d,index=%d")
TRACE_MSG(MMIMMS_SETTING_580_112_2_18_2_38_57_419,"[MMIMMS]:MMIMMS_Setting_SetNetSettingIndex dual_sys=%d index=%d")
TRACE_MSG(MMIMMS_SETTING_604_112_2_18_2_38_57_420,"MMIMMS_Setting_ConnectionCallback,index=%d,dual_sys=%d")
TRACE_MSG(MMIMMS_SETTING_664_112_2_18_2_38_57_421,"[MMIMMS]:MMIMMS_Setting_GetRetrieveModeLocal RetrieveModeLocal=%d")
TRACE_MSG(MMIMMS_SETTING_669_112_2_18_2_38_57_422,"[MMIMMS]:MMIMMS_Setting_GetRetrieveModeRoam RetrieveModeRoam=%d")
TRACE_MSG(MMIMMS_SETTING_692_112_2_18_2_38_57_423,"[MMIMMS]:MMIMMS_Setting_SetRetrieveModeLocal RetrieveModeLocal=%d")
TRACE_MSG(MMIMMS_SETTING_697_112_2_18_2_38_57_424,"[MMIMMS]:MMIMMS_Setting_SetRetrieveModeRoam RetrieveModeRoam=%d")
TRACE_MSG(MMIMMS_SETTINGWIN_1297_112_2_18_2_39_0_425,"list_index=%d")
TRACE_MSG(MMIMMS_WAITINGWIN_136_112_2_18_2_39_3_426,"HandleMMSDownloadingWin: MSG_MMS_SEND_RETRYING send size g_mms_cur_size=%d, g_mms_total_size=%d")
TRACE_MSG(MMIMMS_WAITINGWIN_181_112_2_18_2_39_3_427,"HandleMMSDownloadingWin:download size g_mms_cur_size=%d, g_mms_total_size=%d")
TRACE_MSG(MMIMMS_WAITINGWIN_278_112_2_18_2_39_3_428,"HandleMMSSendingWin: MSG_FULL_PAINT send size g_mms_cur_size=%d, g_mms_total_size=%d")
TRACE_MSG(MMIMMS_WAITINGWIN_287_112_2_18_2_39_3_429,"HandleMMSSendingWin: MSG_MMS_SEND_RETRYING send size g_mms_cur_size=%d, g_mms_total_size=%d")
TRACE_MSG(MMIMMS_WAITINGWIN_378_112_2_18_2_39_3_430,"HandleMMSSendingWin:send size g_mms_cur_size=%d, g_mms_total_size=%d")
TRACE_MSG(MMIMMS_WAITINGWIN_394_112_2_18_2_39_3_431,"HandleMMSSendingWin error id = %d, is_saved = %d")
TRACE_MSG(MMIMMS_WAITINGWIN_494_112_2_18_2_39_4_432,"MMSMMI_CreateDownloadWaitingWin dual_sys = %d")
TRACE_MSG(MMIMMS_WAITINGWIN_557_112_2_18_2_39_4_433,"MMSMMI_CreateSendingWaitingWin dual_sys = %d")
TRACE_MSG(MMIMMS_WAITINGWIN_611_112_2_18_2_39_4_434,"mmimms_main.c, MMIMMS_DisplaySendRecvProgress(), pos = %d, totalsize = %d.")
TRACE_MSG(MMIMMS_WAITINGWIN_698_112_2_18_2_39_4_435,"FNH now_size = %ld, total_size = %ld")
END_TRACE_MAP(MMI_APP_MMS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MMS_TRC_H_

