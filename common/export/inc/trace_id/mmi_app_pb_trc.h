/******************************************************************************
 ** File Name:      mmi_app_pb_trc.h                                         *
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
//trace_id:206
#ifndef _MMI_APP_PB_TRC_H_
#define _MMI_APP_PB_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPB_ADAPT_141_112_2_18_2_42_58_0 "[MMIPB]InitPBFolder Udisk and SDCard not exist !!"
#define MMIPB_ADAPT_397_112_2_18_2_42_59_1 "[MMIPB]MMIPB_SavePhotoIDInfoToFile file_ret %d"
#define MMIPB_ADAPT_459_112_2_18_2_42_59_2 "[MMIPB]MMIPB_ReadPhotoFileName file_ret %d"
#define MMIPB_ADAPT_509_112_2_18_2_42_59_3 "[MMIPB]MMIPB_SaveRingInfoToFile file_ret %d"
#define MMIPB_ADAPT_576_112_2_18_2_42_59_4 "[MMIPB]MMIPB_ReadRingFileName file_ret %d"
#define MMIPB_ADAPT_648_112_2_18_2_42_59_5 "[MMIPB]MMIPB_SaveRingInfoToFile file_ret %d"
#define MMIPB_ADAPT_713_112_2_18_2_42_59_6 "[MMIPB]MMIPB_ReadRingFileName file_ret %d"
#define MMIPB_ADAPT_761_112_2_18_2_43_0_7 "[MMIPB]MMIPB_SaveASPInfoToFile file_ret %d"
#define MMIPB_ADAPT_826_112_2_18_2_43_0_8 "[MMIPB]MMIPB_ReadRingFileName file_ret %d"
#define MMIPB_ADAPT_1275_112_2_18_2_43_1_9 "MMIPB_ContactToUSIMAddtionalContact contact_ptr 0x%x"
#define MMIPB_ADAPT_1327_112_2_18_2_43_1_10 "MMIPB_ContactToUSIMAddtionalContact contact_ptr 0x%x"
#define MMIPB_ADAPT_1761_112_2_18_2_43_2_11 "[MMIPB] MMIPB_WstrToInternalString invalid param !"
#define MMIPB_ADAPT_1899_112_2_18_2_43_2_12 "PHONEBOOK_EncodeAlphabetToSimStandard return flase !"
#define MMIPB_ADAPT_1939_112_2_18_2_43_2_13 "[MMIPB] MMIPB_GSMAlphabetToWstr invalid param !"
#define MMIPB_ADAPT_1954_112_2_18_2_43_2_14 "MMIPB_GSMAlphabetToWstr decode alphabet with sim standard is error"
#define MMIPB_ADAPT_2166_112_2_18_2_43_2_15 "[MMIPB] MMIPB_BCDNumberToUTF8 invalid param !"
#define MMIPB_ADAPT_2220_112_2_18_2_43_2_16 "[MMIPB] MMIPB_BCDNumberToString invalid param !"
#define MMIPB_ADAPT_2254_112_2_18_2_43_3_17 "MMIPB_GetNameLetters name_ptr %d "
#define MMIPB_ADAPT_2665_112_2_18_2_43_3_18 "[MMIPB]MMIPB_ConvertNameToString name_ptr 0x%x str_ptr 0x%x!"
#define MMIPB_APP_258_112_2_18_2_43_5_19 "MMIPB_StartApplet"
#define MMIPB_APP_353_112_2_18_2_43_5_20 "mmi Applet_HandleEvent msg_id = 0x%04x"
#define MMIPB_APP_453_112_2_18_2_43_5_21 "[mmipb] MMIPB_CreateSelNode PNULL == handle_node_ptr "
#define MMIPB_APP_459_112_2_18_2_43_5_22 "MMIPB_CreateSelNode PNULL== s_sel_node_ptr"
#define MMIPB_APP_471_112_2_18_2_43_5_23 "MMIPB_CreateSelNode PNULL== handle_node_ptr ->select_listr"
#define MMIPB_APP_529_112_2_18_2_43_5_24 "[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_node_ptr "
#define MMIPB_APP_535_112_2_18_2_43_5_25 "[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_data_ptr"
#define MMIPB_APP_603_112_2_18_2_43_5_26 "[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_node_ptr "
#define MMIPB_APP_613_112_2_18_2_43_5_27 "[MMIPB] MMIPB_DeleteSelHeadNode handle_data_ptr->sel_count %d"
#define MMIPB_APP_663_112_2_18_2_43_5_28 "[MMIPB] MMIPB_HasSelNode PNULL == handle_node_ptr "
#define MMIPB_APP_690_112_2_18_2_43_5_29 "[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_node_ptr "
#define MMIPB_APP_699_112_2_18_2_43_5_30 "[MMIPB]MMIPB_DeleteSelNode list is PNULL handle %d"
#define MMIPB_APP_740_112_2_18_2_43_5_31 "[MMIPB] MMIPB_DeleteAllSelNode PNULL == handle_node_ptr "
#define MMIPB_APP_902_112_2_18_2_43_6_32 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
#define MMIPB_APP_1005_112_2_18_2_43_6_33 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
#define MMIPB_APP_1082_112_2_18_2_43_6_34 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
#define MMIPB_APP_1158_112_2_18_2_43_6_35 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
#define MMIPB_APP_1230_112_2_18_2_43_6_36 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
#define MMIPB_APP_1374_112_2_18_2_43_7_37 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
#define MMIPB_APP_1462_112_2_18_2_43_7_38 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
#define MMIPB_APP_1552_112_2_18_2_43_7_39 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
#define MMIPB_APP_1558_112_2_18_2_43_7_40 "[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == applet_ptr"
#define MMIPB_APP_1718_112_2_18_2_43_7_41 "[MMIPB] mmipb_app_callback handle %d error %d"
#define MMIPB_APP_1762_112_2_18_2_43_8_42 "[MMIPB][PB]MMIPB_CheckSpace needed_record %d max_record %d"
#define MMIPB_APP_1787_112_2_18_2_43_8_43 "[mmipb] MMIPB_SetOpcType PNULL == handle_node_ptr "
#define MMIPB_APP_1810_112_2_18_2_43_8_44 "[mmipb] MMIPB_AddContact not ready !"
#define MMIPB_APP_1861_112_2_18_2_43_8_45 "[MMIPB] MMIPB_UpdatePhonebookEntry (PNULL == contact_ptr) !"
#define MMIPB_APP_1874_112_2_18_2_43_8_46 "[MMIPB][PB] MMIPB_UpdatePhonebookEntry result %d"
#define MMIPB_APP_1893_112_2_18_2_43_8_47 "[mmipb] MMIAPIPB_DeleteContact not ready !"
#define MMIPB_APP_1896_112_2_18_2_43_8_48 "[MMIPB] MMIAPIPB_DeleteContact, storage=%d, entry_id=%d,"
#define MMIPB_APP_1914_112_2_18_2_43_8_49 "[MMIPB] MMIPB_DeleteAllMostUsedContact result %"
#define MMIPB_APP_1928_112_2_18_2_43_8_50 "[mmipb] MMIPB_AddContact not ready !"
#define MMIPB_APP_1971_112_2_18_2_43_8_51 "[MMIPB] MMIPB_UpdateContactAsyn (PNULL == contact_ptr) !"
#define MMIPB_APP_1977_112_2_18_2_43_8_52 "[MMIPB] MMIPB_UpdateContactAsyn (PNULL == update_contact_ptr) !"
#define MMIPB_APP_1990_112_2_18_2_43_8_53 "[MMIPB][PB] MMIPB_UpdateContactAsyn result %d"
#define MMIPB_APP_2004_112_2_18_2_43_8_54 "[mmipb] MMIPB_AddContact not ready !"
#define MMIPB_APP_2034_112_2_18_2_43_8_55 "[mmipb]MMIPB_AddContactAsyn, ret %d"
#define MMIPB_APP_2114_112_2_18_2_43_8_56 "[mmipb] MMIPB_AddContact not ready !"
#define MMIPB_APP_2119_112_2_18_2_43_8_57 "[mmipb] MMIPB_ModifyContactAsyn PNULL == contact_ptr!"
#define MMIPB_APP_2147_112_2_18_2_43_9_58 "[mmipb]MMIPB_ModifyContactAsyn, ret %d"
#define MMIPB_APP_2193_112_2_18_2_43_9_59 "[MMIPB] MMIPB_ExportVcard (PNULL == mmipb_contact_ptr) !"
#define MMIPB_APP_2200_112_2_18_2_43_9_60 "[mmipb]  MMIPB_ExportVcard  error ret %d"
#define MMIPB_APP_2261_112_2_18_2_43_9_61 "[mmipb] MMIPB_AddContact not ready !"
#define MMIPB_APP_2267_112_2_18_2_43_9_62 "[MMIPB] MMIPB_UpdateContactAsyn (PNULL == mmipb_contact_ptr) !"
#define MMIPB_APP_2276_112_2_18_2_43_9_63 "[mmipb] MMIAPIPB_ExportVcard error ret %d"
#define MMIPB_APP_2295_112_2_18_2_43_9_64 "[mmipb]MMIPB_ExportVcardAsyn, ret %d"
#define MMIPB_APP_2319_112_2_18_2_43_9_65 "[mmipb] MMIPB_AddContact not ready !"
#define MMIPB_APP_2344_112_2_18_2_43_9_66 "[mmipb] MMIAPIPB_ExportContactAsyn not ready !"
#define MMIPB_APP_2349_112_2_18_2_43_9_67 "[mmipb] MMIAPIPB_ExportContactAsyn PNULL == handle_node_ptr !"
#define MMIPB_APP_2368_112_2_18_2_43_9_68 "[mmipb]MMIPB_ExportContactAsyn there is not contact to delete"
#define MMIPB_APP_2392_112_2_18_2_43_9_69 "[mmipb] MMIPB_AddContact not ready !"
#define MMIPB_APP_2399_112_2_18_2_43_9_70 "[MMIPB]MMIPB_ImportVcard MMIPB_Read RET %d"
#define MMIPB_APP_2451_112_2_18_2_43_9_71 "[mmipb] MMIPB_AddContact not ready !"
#define MMIPB_APP_2488_112_2_18_2_43_9_72 "[mmipb] MMIAPIPB_ExportContactAsyn not ready !"
#define MMIPB_APP_2493_112_2_18_2_43_9_73 "[mmipb] MMIAPIPB_ExportContactAsyn PNULL == handle_node_ptr !"
#define MMIPB_APP_2518_112_2_18_2_43_9_74 "[mmipb]MMIPB_ExportContactAsyn there is not contact to delete"
#define MMIPB_APP_2543_112_2_18_2_43_9_75 "[mmipb] MMIPB_DeleteContact not ready !"
#define MMIPB_APP_2550_112_2_18_2_43_9_76 "[MMIPB] MMIPB_DeleteContactNode, entry_id=%d, storage_id %d"
#define MMIPB_APP_2555_112_2_18_2_43_9_77 "[mmipb]MMIPB_DeleteContactAsyn, result %d"
#define MMIPB_APP_2587_112_2_18_2_43_9_78 "[mmipb] MMIPB_DeleteContactAsynForPB not ready !"
#define MMIPB_APP_2592_112_2_18_2_43_9_79 "[mmipb] MMIPB_DeleteContactAsynForPB PNULL == handle_node_ptr !"
#define MMIPB_APP_2608_112_2_18_2_43_9_80 "[mmipb]MMIPB_DeleteContactAsynForPB there is not contact to delete"
#define MMIPB_APP_2631_112_2_18_2_43_10_81 "[mmipb] MMIPB_GetContactInfo not ready !"
#define MMIPB_APP_2681_112_2_18_2_43_10_82 "[mmipb] MMIPB_GetContactInfo not ready !"
#define MMIPB_APP_2898_112_2_18_2_43_10_83 "[mmipb] MMIPB_GetContactInfo not ready !"
#define MMIPB_APP_2904_112_2_18_2_43_10_84 "[mmipb]MMIPB_CopyContactAsyn PNULL == handle_node_ptr "
#define MMIPB_APP_2945_112_2_18_2_43_10_85 "[mmipb]MMIPB_CopyContactAsyn, ret %d"
#define MMIPB_APP_2988_112_2_18_2_43_10_86 "[mmipb] MMIPB_GetContactInfo not ready !"
#define MMIPB_APP_2993_112_2_18_2_43_10_87 "[mmipb]MMIPB_CopyMultiContactAsyn PNULL == handle_node_ptr "
#define MMIPB_APP_3125_112_2_18_2_43_11_88 "[mmipb] MMIPB_WriteGroupName name->wstr_len %d"
#define MMIPB_APP_3162_112_2_18_2_43_11_89 "[mmipb] MMIPB_GetSelectCount not ready !"
#define MMIPB_APP_3169_112_2_18_2_43_11_90 "[mmipb] MMIPB_GetSelectCount PNULL == handle_node_ptr !"
#define MMIPB_APP_3176_112_2_18_2_43_11_91 "[mmipb] MMIPB_GetSelectCount PNULL == applet_ptr !"
#define MMIPB_APP_3193_112_2_18_2_43_11_92 "[mmipb] MMIPB_GetSelectCount PNULL == handle_node_ptr ->select_file_list !"
#define MMIPB_APP_3313_112_2_18_2_43_11_93 "[MMIPB]MMIPB_HandleCallReadyCnf call_ready_param_ptr 0x%x "
#define MMIPB_APP_3321_112_2_18_2_43_11_94 "[MMIPB]MMIPB_HandleCallReadyCnf dual_sys %d"
#define MMIPB_APP_3456_112_2_18_2_43_11_95 "[MMIPB]HandlePBPsMsg 0x%x"
#define MMIPB_APP_3461_112_2_18_2_43_11_96 "[MMIPB]HandlePBPsMsg MMIPB_HandleCallReadyCnf"
#define MMIPB_APP_3475_112_2_18_2_43_11_97 "[MMIPB]HandlePBPsMsg APP_MN_SIM_NOT_READY_IND"
#define MMIPB_APP_3564_112_2_18_2_43_11_98 "[MMIPB]: HandleSIMPlugNotifyPBFunc dual_sys=%d, notify_event=%d"
#define MMIPB_APP_3659_112_2_18_2_43_12_99 "[MMIPB]MMIPB_InitSimStatus  dual_sys %d"
#define MMIPB_APP_3663_112_2_18_2_43_12_100 "[MMIPB]MMIPB_InitSimStatus dual_sys %d"
#define MMIPB_APP_3681_112_2_18_2_43_12_101 "[MMIPB]MMIPB_HandlePbSimNotReadyInd (PNULL == sim_not_ready_param_ptr) !"
#define MMIPB_APP_3688_112_2_18_2_43_12_102 "[MMIPB]MMIPB_HandlePbSimNotReadyInd sim_not_ready_param_ptr->dual_sys %d!"
#define MMIPB_APP_3695_112_2_18_2_43_12_103 "[MMIPB]MMIPB_HandlePbSimNotReadyInd dual_sys %d sim_not_ready_param_ptr->reason %d"
#define MMIPB_APP_3727_112_2_18_2_43_12_104 "[MMIPB]MMIPB_HandlePbSimNotReadyInd is_need_response %d !"
#define MMIPB_APP_3940_112_2_18_2_43_12_105 "[mmipb] MMIPB_InitEntryList read sim  contact error entry_id %d, sim_type %d"
#define MMIPB_APP_3964_112_2_18_2_43_12_106 "[mmipb] MMIPB_InitEntryList read nv contact error entry_id %d"
#define MMIPB_APP_3982_112_2_18_2_43_12_107 "[mmipb] MMIPB_InitEntryList read mostused contact error entry_id %d"
#define MMIPB_APP_3995_112_2_18_2_43_12_108 "[MMIPB][SNS] MMIPB_InitEntryList read sns contact num = %d"
#define MMIPB_APP_4038_112_2_18_2_43_12_109 "[MMIPB]MMIPB_ReadUSIMExtCnf enter usim_ext_ptr %d"
#define MMIPB_APP_4042_112_2_18_2_43_12_110 "[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->adn_entry_id %d dual_sys"
#define MMIPB_APP_4045_112_2_18_2_43_12_111 "[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.anr_ret[%d] %d"
#define MMIPB_APP_4052_112_2_18_2_43_12_112 "[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.email_ret %d email len %d"
#define MMIPB_APP_4057_112_2_18_2_43_12_113 "[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.sne_ret %d sne len %d"
#define MMIPB_APP_4105_112_2_18_2_43_12_114 "[MMIPB] HandleEntryUpdateResult (update_result == PNULL)"
#define MMIPB_APP_4113_112_2_18_2_43_12_115 "[MMIPB]HandleEntryUpdateResult update_result %d !"
#define MMIPB_APP_4119_112_2_18_2_43_12_116 "[MMIPB]HandleEntryUpdateResult update_result->update_result.adn_flag %d !cur_num_index %d entry id %d"
#define MMIPB_APP_4199_112_2_18_2_43_13_117 "[MMIPB] delete_result PNULL == delete_result !"
#define MMIPB_APP_4202_112_2_18_2_43_13_118 "[MMIPB]HandleEntryDeleteResult, delete_result adn_result%d dual_sys %d"
#define MMIPB_APP_4232_112_2_18_2_43_13_119 "[MMIPB] MMIPB_SetPBStatus status %d"
#define MMIPB_APP_4246_112_2_18_2_43_13_120 "[MMIPB] MMIPB_GetPBStatus status %d"
#define MMIPB_APP_4263_112_2_18_2_43_13_121 "[MMIPB]MMIPB_SetNumberUsed pb not ready !"
#define MMIPB_APP_4320_112_2_18_2_43_13_122 "[mmipb]MMIPB_ReadSearchedName PNULL == contact_ptr"
#define MMIPB_APP_4327_112_2_18_2_43_13_123 "[mmipb]MMIPB_ReadSearchedName PNULL == handle_node_ptr "
#define MMIPB_APP_4335_112_2_18_2_43_13_124 "[mmipb]MMIPB_ReadSearchedName PNULL == mmipb_handle_data"
#define MMIPB_APP_4347_112_2_18_2_43_13_125 "[mmipb]MMIPB_ReadSearchedName index %d"
#define MMIPB_APP_4409_112_2_18_2_43_13_126 "[mmipb] MMIPB_ReadSearchedName PNULL == nameber_ptr"
#define MMIPB_APP_4441_112_2_18_2_43_13_127 "[mmipb] MMIPB_ReadSearchedName PNULL == nameber_ptr"
#define MMIPB_APP_4448_112_2_18_2_43_13_128 "[mmipb] MMIPB_GetSelectNumber PNULL == contact_ptr"
#define MMIPB_APP_4482_112_2_18_2_43_13_129 "[mmipb] MMIPB_GetSelectVcard PNULL == data_ptr"
#define MMIPB_APP_4490_112_2_18_2_43_13_130 "mmipb, MMIPB_GetSelectVcardPNULL == contact_ptr "
#define MMIPB_APP_4513_112_2_18_2_43_13_131 "[MMIPB]MMIPB_AbortPB is_abort %d"
#define MMIPB_APP_4525_112_2_18_2_43_13_132 "[MMIPB]MMIPB_AbortPB is_abort %d"
#define MMIPB_APP_4639_112_2_18_2_43_14_133 "[MMIPB] MMIPB_GetCustomPhotoPathByNumber invalid param !"
#define MMIPB_APP_4644_112_2_18_2_43_14_134 "[MMIPB]MMIPB_GetCustomPhotoPathByNumber not find !!"
#define MMIPB_APP_4711_112_2_18_2_43_14_135 "[MMIPB] MMIPB_GetCustomASPFileByNumber invalid param !"
#define MMIPB_APP_4715_112_2_18_2_43_14_136 "[MMIPB]MMIPB_GetCustomASPFileByNumber entered"
#define MMIPB_APP_4719_112_2_18_2_43_14_137 "[MMIPB]MMIPB_GetCustomASPFileByNumber not find !!"
#define MMIPB_APP_4753_112_2_18_2_43_14_138 "[MMIPB] MMIPB_GetCustomASPFileByNumber has deleted asp pb"
#define MMIPB_APP_4852_112_2_18_2_43_14_139 "[MMIPB] MMIPB_GetCustomRingFileByNumber invalid param !"
#define MMIPB_APP_4856_112_2_18_2_43_14_140 "[MMIPB]MMIPB_GetCustomRingFileByNumber entered"
#define MMIPB_APP_4860_112_2_18_2_43_14_141 "[MMIPB]MMIPB_GetCustomRingFileByNumber not find !!"
#define MMIPB_APP_4895_112_2_18_2_43_14_142 "[MMIPB] MMIPB_GetCustomRingFileByNumber has deleted  pb"
#define MMIPB_APP_5021_112_2_18_2_43_14_143 "[mmipb] MMIPB_AddContactToGroupAsynForPB not ready !"
#define MMIPB_APP_5026_112_2_18_2_43_14_144 "[mmipb] MMIPB_AddContactToGroupAsynForPB PNULL == handle_node_ptr !"
#define MMIPB_APP_5061_112_2_18_2_43_14_145 "[mmipb]MMIPB_AddContactToGroupAsynForPB there is not contact"
#define MMIPB_APP_5129_112_2_18_2_43_14_146 "[mmipb] MMIPB_SendGroupManage PNULL == handle_node_ptr !"
#define MMIPB_APP_5200_112_2_18_2_43_15_147 "[mmipb] MMIPB_AddContactToFavouriteAsynForPB not ready !"
#define MMIPB_APP_5205_112_2_18_2_43_15_148 "[mmipb] MMIPB_AddContactToFavouriteAsynForPB PNULL == handle_node_ptr !"
#define MMIPB_APP_5221_112_2_18_2_43_15_149 "[mmipb]MMIPB_AddContactToFavouriteAsynForPB there is not contact to delete"
#define MMIPB_APP_5288_112_2_18_2_43_15_150 "[mmipb] MMIPB_AddContactToFavouriteAsynForPB not ready !"
#define MMIPB_APP_5293_112_2_18_2_43_15_151 "[mmipb] MMIPB_AddContactToFavouriteAsynForPB PNULL == handle_node_ptr !"
#define MMIPB_APP_5309_112_2_18_2_43_15_152 "[mmipb]MMIPB_AddContactToFavouriteAsynForPB there is not contact to delete"
#define MMIPB_APP_5389_112_2_18_2_43_15_153 "[MMIPB]MakeNumberCallByBCDNumber number_str_ptr == PNULL !"
#define MMIPB_APP_5530_112_2_18_2_43_15_154 "[MMIPB]MMIPB_CallBCDNumber !"
#define MMIPB_APP_5723_112_2_18_2_43_16_155 "[MMIPB] ikeybackupcallback invalid param !"
#define MMIPB_APP_5731_112_2_18_2_43_16_156 "[MMIPB] ikeybackupcallback type error !"
#define MMIPB_APP_5807_112_2_18_2_43_16_157 "[MMIPB] ikeyrestore_callback invalid param !"
#define MMIPB_APP_5921_112_2_18_2_43_16_158 "[mmipb] MMIAPIPB_GetMSISDNumber, PNULL == contact_ptr"
#define MMIPB_APP_5943_112_2_18_2_43_16_159 "[mmipb] MMIPB_GetMSISDNumber j %d, max_num %d"
#define MMIPB_APP_5973_112_2_18_2_43_16_160 "[mmipb] MMIAPIPB_GetMSISDNumber, PNULL == contact_ptr"
#define MMIPB_APP_5993_112_2_18_2_43_16_161 "[mmipb] MMIPB_GetMSISDNumberBySim j %d, max_num %d"
#define MMIPB_ATC_113_112_2_18_2_43_17_162 "[MMIPB] PNULL == pb_head_info_ptr !"
#define MMIPB_ATC_157_112_2_18_2_43_17_163 "[MMIATC] max_record_num %d max_alpha_len %d max_num_len %d max_email_len %d !"
#define MMIPB_ATC_215_112_2_18_2_43_17_164 "[MMIPB]MMIPB_SyncRead  PNULL == pb_item_info_ptr !"
#define MMIPB_ATC_249_112_2_18_2_43_17_165 "MMIPB_SyncRead, read from nv"
#define MMIPB_ATC_335_112_2_18_2_43_17_166 "[MMIPB]MMIPB_SyncAdd pb_item_info_ptr %d entry_id !"
#define MMIPB_ATC_362_112_2_18_2_43_17_167 "MMIPB_SyncAdd, add to nv"
#define MMIPB_ATC_413_112_2_18_2_43_17_168 "[MMIPB] CopySyncEntryToStoreEntry (PNULL == pb_entry_ptr) || (PNULL == pb_item_info_ptr) !"
#define MMIPB_ATC_496_112_2_18_2_43_17_169 "[MMIPB]MMIPB_SyncDelete entry_id %d !"
#define MMIPB_ATC_506_112_2_18_2_43_17_170 "MMIPB_SyncDelete from nv"
#define MMIPB_ATC_576_112_2_18_2_43_18_171 "MMIPB_SyncModify in nv"
#define MMIPB_ATC_618_112_2_18_2_43_18_172 "[MMIPB] MMIPBGROUP_SyncHeadInfo (NULL == group_head_info_ptr) !"
#define MMIPB_ATC_631_112_2_18_2_43_18_173 "[MMIPB] MMIPBGROUP_SyncHeadInfo max_group_num %d max_name_len %d !"
#define MMIPB_DATALIST_185_112_2_18_2_43_18_174 "[MMIPB]SetEntryIdFlag entry_id 0x%x pb_sort_ptr %d"
#define MMIPB_DATALIST_382_112_2_18_2_43_19_175 "[MMIPB]MMIPB_GetContactNameNumberByIndex invalid param "
#define MMIPB_DATALIST_568_112_2_18_2_43_19_176 "CompareLettersInSortTable valid parameter !!"
#define MMIPB_DATALIST_615_112_2_18_2_43_19_177 "CompareLettersInSortTable valid parameter !!"
#define MMIPB_DATALIST_981_112_2_18_2_43_20_178 "[mmipb] FindDailNumberContact PNULL == contact_ptr"
#define MMIPB_DATALIST_1255_112_2_18_2_43_21_179 "ContactListCmp valid parameter !!"
#define MMIPB_DATALIST_1296_112_2_18_2_43_21_180 "[MMIPB] MMIPB_InsertContactListNode max_count %d, count %d"
#define MMIPB_DATALIST_1302_112_2_18_2_43_21_181 "[MMIPB]MMIPB_InsertQListNode letters_ptr == PNULL !"
#define MMIPB_DATALIST_1764_112_2_18_2_43_22_182 "MostUsedContactListCmp valid parameter !!"
#define MMIPB_DATALIST_1770_112_2_18_2_43_22_183 "MostUsedContactListCmp ret %d "
#define MMIPB_DATALIST_1789_112_2_18_2_43_22_184 "[MMIPB] MMIPB_InsertMostUsedContactListNode invalid param !"
#define MMIPB_DATALIST_2487_112_2_18_2_43_23_185 "[MMIPB]MMIPB_FindFirstNotInitEntryID parameter invalid !"
#define MMIPB_DATALIST_2509_112_2_18_2_43_23_186 "[MMIPB]MMIPB_FindFirstNotInitEntryID result %d"
#define MMIPB_DATALIST_2532_112_2_18_2_43_23_187 "[MMIPB] MMIPB_ReadUSIMAddtionalFromContactList invalid param !"
#define MMIPB_DATALIST_2589_112_2_18_2_43_24_188 "[MMIPB] MMIPB_SaveAddtionToList invalid param !"
#define MMIPB_EXPORT_1136_112_2_18_2_43_26_189 "MMIAPIPB_Wstrstrpos pnull"
#define MMIPB_INTERFACE_335_112_2_18_2_43_28_190 "[MMIPB] MMIPB_WriteTrigger"
#define MMIPB_INTERFACE_354_112_2_18_2_43_28_191 "[MMIPB] MMIPB_DeleteTrigger"
#define MMIPB_INTERFACE_421_112_2_18_2_43_28_192 "[baokun] MMIPB_Write invalid param !"
#define MMIPB_INTERFACE_477_112_2_18_2_43_28_193 "[baokun] MMIPB_Delete invalid param !"
#define MMIPB_INTERFACE_510_112_2_18_2_43_28_194 "[MMIPB] MMIPB_Delete exit result %d"
#define MMIPB_INTERFACE_520_112_2_18_2_43_28_195 "[MMIPB] MMIPB_SetWriteError error %d"
#define MMIPB_INTERFACE_536_112_2_18_2_43_28_196 "[MMIPB] MMIPB_GetWriteError error %d"
#define MMIPB_INTERFACE_546_112_2_18_2_43_28_197 "[MMIPB] MMIPB_SetDeleteError error %d"
#define MMIPB_INTERFACE_563_112_2_18_2_43_28_198 "[MMIPB] MMIPB_GetDeleteError error %d"
#define MMIPB_INTERFACE_579_112_2_18_2_43_28_199 "[MMIPB] MMIPB_ReadUSIMAddtionalAsyn dual_sys %d contact_id %d"
#define MMIPB_INTERFACE_604_112_2_18_2_43_28_200 "[MMIPB]ReadUSimContact invalid param !"
#define MMIPB_INTERFACE_652_112_2_18_2_43_28_201 "[MMIPB] SetUSIMUpdateFlag invalid param !"
#define MMIPB_INTERFACE_790_112_2_18_2_43_29_202 "[MMIPB]WriteUSimContact invalid param !"
#define MMIPB_INTERFACE_846_112_2_18_2_43_29_203 "[MMIPB] WriteUSIMContact contact_id %d storage_id %d result %d"
#define MMIPB_INTERFACE_894_112_2_18_2_43_29_204 "[MMIPB] DeleteUSIMContact contact_id %d storage_id %d result %d"
#define MMIPB_INTERFACE_1137_112_2_18_2_43_29_205 "[MMIPB] WriteContact contact_id %d storage_id %d result %d"
#define MMIPB_INTERFACE_1188_112_2_18_2_43_30_206 "[MMIPB] DeleteContact contact_id %d storage_id %d result %d"
#define MMIPB_INTERFACE_1210_112_2_18_2_43_30_207 "[MMIPB]ReadVcardContact invalid param !"
#define MMIPB_INTERFACE_1230_112_2_18_2_43_30_208 "[MMIPB] ReadVcardContact  result %d"
#define MMIPB_INTERFACE_1252_112_2_18_2_43_30_209 "[MMIPB] WriteVcardContact invalid param !"
#define MMIPB_INTERFACE_1272_112_2_18_2_43_30_210 "[MMIPB]WriteVcardContact result %d"
#define MMIPB_INTERFACE_1291_112_2_18_2_43_30_211 "[MMIPB]DeleteVcardContact result %d"
#define MMIPB_INTERFACE_1326_112_2_18_2_43_30_212 "[MMIPB][SNS] ReadSnsContact index %d, ret %d"
#define MMIPB_INTERFACE_1552_112_2_18_2_43_30_213 "[MMIPB]MMIPB_CreateHandle applet_handle %d"
#define MMIPB_INTERFACE_1581_112_2_18_2_43_30_214 "[MMIPB]MMIPB_CloseHandle handle %d"
#define MMIPB_INTERFACE_1665_112_2_18_2_43_31_215 "[MMIPB]MMIPB_ReadAsyn queue full !"
#define MMIPB_INTERFACE_1749_112_2_18_2_43_31_216 "[MMIPB]MMIPB_WriteAsyn queue full !"
#define MMIPB_INTERFACE_1769_112_2_18_2_43_31_217 "[MMIPB]MMIPB_WriteAsyn result %d"
#define MMIPB_INTERFACE_1824_112_2_18_2_43_31_218 "[MMIPB]MMIPB_DeleteAsyn queue full !"
#define MMIPB_INTERFACE_1844_112_2_18_2_43_31_219 "[MMIPB]MMIPB_DeleteAsyn result %d"
#define MMIPB_INTERFACE_1872_112_2_18_2_43_31_220 "[MMIPB]MMIPB_IsCurrentOperation contact_id %d storage_id %d result %d"
#define MMIPB_INTERFACE_1900_112_2_18_2_43_31_221 "[MMIPB]MMIPB_CmdConfirm handle 0x%x  result %d ret %d"
#define MMIPB_INTERFACE_1917_112_2_18_2_43_31_222 "[MMIPB]MMIPB_GetAllNormalContactCount count %d"
#define MMIPB_INTERFACE_1929_112_2_18_2_43_31_223 "[MMIPB]MMIPB_GetAllNormalContactCount count %d"
#define MMIPB_INTERFACE_1947_112_2_18_2_43_31_224 "[MMIPB]MMIPB_GetAllNormalContactPhoneCount count %d"
#define MMIPB_INTERFACE_1965_112_2_18_2_43_31_225 "[MMIPB]MMIPB_GetAllNormalContactMailCount count %d"
#define MMIPB_INTERFACE_2007_112_2_18_2_43_31_226 "[MMIPB]MMIPB_GetContactCount storage_id 0x%x count %d"
#define MMIPB_INTERFACE_2075_112_2_18_2_43_32_227 "[MMIPB]MMIPB_GetGroupContactCount group_id 0x%x count %d"
#define MMIPB_INTERFACE_2181_112_2_18_2_43_32_228 "[MMIPB]MMIPB_FindFirstContact group 0x%x handle_ptr 0x%x result %d"
#define MMIPB_INTERFACE_2229_112_2_18_2_43_32_229 "[MMIPB]MMIPB_FindFirstMostUsedContact  handle_ptr 0x%x result %d"
#define MMIPB_INTERFACE_2314_112_2_18_2_43_32_230 "[MMIPB]MMIPB_FindNextMostContact  handle 0x%x index %d result %d"
#define MMIPB_MEM_467_112_2_18_2_43_34_231 "[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr!"
#define MMIPB_MEM_502_112_2_18_2_43_34_232 "[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr!"
#define MMIPB_MEM_506_112_2_18_2_43_34_233 "[MMIMEM]:MMIMEM_Malloc mem_ptr =0x%x"
#define MMIPB_MEM_538_112_2_18_2_43_34_234 "MMIPB_Malloc: size = %d,file = %s, line=%ld"
#define MMIPB_MEM_555_112_2_18_2_43_34_235 "MMIMEM_MFree: mem_ptr = 0x%x,file = %s, line=%ld"
#define MMIPB_MENU_2153_112_2_18_2_43_39_236 "HandleStorageSelectWinMsg node_id is %d"
#define MMIPB_MENU_2196_112_2_18_2_43_39_237 "[MMIPB]OpenAddWin"
#define MMIPB_MENU_2982_112_2_18_2_43_40_238 "MMI_PB_SET_PHOTO_FILE msg file_name_ptr is %d"
#define MMIPB_MENU_3062_112_2_18_2_43_40_239 "[baokun] add MSG_CTL_PENOK param %d"
#define MMIPB_MENU_3287_112_2_18_2_43_41_240 "[MMIPB] MSG_MULTIM_SELECTED_RETURN param == 0 !"
#define MMIPB_MENU_3352_112_2_18_2_43_41_241 "[MMIPB] MSG_SET_SELECT_FIX_CALL_RING_RETURN param == 0 !"
#define MMIPB_MENU_3357_112_2_18_2_43_41_242 "[MMIPB]HandleEntryAddWinMsg MSG_SET_SELECT_FIX_CALL_RING_RETURN ring_id=%d"
#define MMIPB_MENU_3762_112_2_18_2_43_42_243 "[MMIPB]SetContactAddOrEditItem paramer illegal !"
#define MMIPB_MENU_3768_112_2_18_2_43_42_244 "[MMIPB]SetContactAddOrEditItem item_id = %d,item_index = %d, is_update =%d"
#define MMIPB_MENU_3919_112_2_18_2_43_42_245 "[MMIPB]SetContactAddOrEditItem index %d"
#define MMIPB_MENU_3959_112_2_18_2_43_42_246 "[MMIPB]UpdateContactAddEditInfo paramer illegal !"
#define MMIPB_MENU_3970_112_2_18_2_43_42_247 "[MMIPB]UpdateContactAddEditInfo cur item_id = %d"
#define MMIPB_MENU_4122_112_2_18_2_43_43_248 "[MMIPB]UpdateContactAddEditInfo default error"
#define MMIPB_MENU_4157_112_2_18_2_43_43_249 "[MMIPB]InitContactEditItems PNULL == add_data_ptr"
#define MMIPB_MENU_4295_112_2_18_2_43_43_250 "[MMIPB]InitContactEditItems total item = %d"
#define MMIPB_MENU_4412_112_2_18_2_43_43_251 "[MMIPB]OpenOperateWinInNvAdd %d == max_ucs2_len"
#define MMIPB_MENU_4442_112_2_18_2_43_43_252 "[MMIPB]OpenOperateWinInNvAdd PNULL == str_ptr !"
#define MMIPB_MENU_4548_112_2_18_2_43_43_253 "[MMIPB]OpenOperateWinInNvAdd ERROR index %d"
#define MMIPB_MENU_5672_112_2_18_2_43_46_254 "[MMIPB] IniContactEditorCtrl PNULL == entry_ptr"
#define MMIPB_MENU_5690_112_2_18_2_43_46_255 "InitContactEditorCtrl name storage_id is %d"
#define MMIPB_MENU_5698_112_2_18_2_43_46_256 "InitContactEditorCtrl name max_ucs2_len is %d"
#define MMIPB_MENU_6215_112_2_18_2_43_47_257 "[MMIPB] IniContactEditorCtrl PNULL == entry_ptr"
#define MMIPB_MENU_6233_112_2_18_2_43_47_258 "InitContactEditorCtrl name storage_id is %d"
#define MMIPB_MENU_6241_112_2_18_2_43_47_259 "InitContactEditorCtrl name max_ucs2_len is %d"
#define MMIPB_MENU_6737_112_2_18_2_43_48_260 "[MMIPB] UpdateContactEditNumber delete_index >= MMIPB_MAX_NV_PHONE_NUM"
#define MMIPB_MENU_7145_112_2_18_2_43_49_261 "[baokun] AddContact ret %d"
#define MMIPB_MENU_7177_112_2_18_2_43_49_262 "DeleteContact applet handle is %d"
#define MMIPB_MENU_7179_112_2_18_2_43_49_263 "DeleteContact applet_ptr is %d"
#define MMIPB_MENU_9818_112_2_18_2_43_55_264 "MMIBT_OpenFileCallBack, file_num = %d"
#define MMIPB_MENU_10196_112_2_18_2_43_56_265 "[mmipb] MMIPB_EnterPbTabOptMenu PNULL == data_ptr"
#define MMIPB_MENU_10202_112_2_18_2_43_56_266 "[mmipb] MMIPB_EnterPbTabOptMenu PNULL == menu_infor_ptr"
#define MMIPB_MENU_10205_112_2_18_2_43_56_267 "MMIPB_EnterPbTabOptMenu menu_infor_ptr is %d"
#define MMIPB_MENU_10211_112_2_18_2_43_56_268 "[mmipb] MMIPB_EnterPbTabOptMenu PNULL == contact_ptr"
#define MMIPB_MENU_10226_112_2_18_2_43_56_269 "MMIPB_EnterPbTabOptMenu the applet handle is %d"
#define MMIPB_MENU_11365_112_2_18_2_43_59_270 "[mmipb] MMIPB_UpdateListContentItemForContact param null"
#define MMIPB_MENU_11385_112_2_18_2_43_59_271 "[MMIPB]MMIPB_UpdateListContentItemForContact is_photo_id_exist %d"
#define MMIPB_SEARCH_316_112_2_18_2_44_0_272 "[MMIPB]alloc s_pb_number_list.item_ptr failed !!"
#define MMIPB_SEARCH_330_112_2_18_2_44_0_273 "[MMIPB]alloc s_pb_mostused_number_list.item_ptr failed !!"
#define MMIPB_SEARCH_368_112_2_18_2_44_0_274 "[MMIPB] AddPhoneNumberToSearchList PNULL == entry_ptr !"
#define MMIPB_SEARCH_412_112_2_18_2_44_0_275 "[MMIPB]MMIPB_GetNameByNumber bcd_number %d name_ptr %d"
#define MMIPB_SEARCH_418_112_2_18_2_44_1_276 "[MMIPB]MMIPB_GetNameByNumber MMIAPIPB_IsPbReady not ready !!"
#define MMIPB_SEARCH_497_112_2_18_2_44_1_277 "[MMIPB]MMIPB_GetNameByNumber bcd_number %d type_ptr %d"
#define MMIPB_SEARCH_504_112_2_18_2_44_1_278 "[MMIPB]MMIPB_GetNameByNumber MMIAPIPB_IsPbReady not ready !!"
#define MMIPB_SEARCH_709_112_2_18_2_44_1_279 "[MMIPB] InsertToList PNULL == item_ptr !"
#define MMIPB_SEARCH_726_112_2_18_2_44_1_280 "[MMIPB] InsertToList search_list_ptr->count %d"
#define MMIPB_SEARCH_731_112_2_18_2_44_1_281 "[MMIPB] InsertToList search_list_ptr->item_ptr == PNULL !"
#define MMIPB_SEARCH_781_112_2_18_2_44_1_282 "[MMIPB] IntelligentSearch item_ptr 0x%x next_pos_ptr 0x%x !"
#define MMIPB_SEARCH_784_112_2_18_2_44_1_283 "[baokun] item_ptr len %d high 0x%x, low 0x%x"
#define MMIPB_SEARCH_885_112_2_18_2_44_1_284 "[MMIPB]MMIPB_SearchPhoneNumber alloc failed!"
#define MMIPB_SEARCH_1094_112_2_18_2_44_2_285 "[MMIPB] MMIPB_DelNumberFromList search_list_ptr->item_ptr == PNULL !"
#define MMIPB_SEARCH_1307_112_2_18_2_44_2_286 "MMIPB_MultiSearch group %d count %d"
#define MMIPB_SEARCH_1336_112_2_18_2_44_2_287 "[MMIPB]MMIPB_SearchList enter %d "
#define MMIPB_SEARCH_1339_112_2_18_2_44_2_288 "[mmipb] MMIPB_SearchList not ready !"
#define MMIPB_SEARCH_1497_112_2_18_2_44_3_289 "MMIPB_SearchList field_type 0x%x search_type 0x%x count %d"
#define MMIPB_SEARCH_1573_112_2_18_2_44_3_290 "MMIPB_GetSearchEntry index %d result %d"
#define MMIPB_SEARCH_1614_112_2_18_2_44_3_291 "MMIPB_GetSearchEntryID index %d result %d"
#define MMIPB_STORAGE_195_112_2_18_2_44_8_292 "[MMIPB][MMIPB]MMIPB_GetMaxAlphaLen storage %d dual_sys %d feild %d max_len %d"
#define MMIPB_STORAGE_248_112_2_18_2_44_8_293 "MMIPB_ReadPhoneContact invalid param"
#define MMIPB_STORAGE_283_112_2_18_2_44_8_294 "MMIPB_ReadMostUsedContact invalid param"
#define MMIPB_STORAGE_319_112_2_18_2_44_8_295 "MMIPB_ReadSIMContact invalid param"
#define MMIPB_STORAGE_369_112_2_18_2_44_8_296 "MMIPB_ReadVcardContact invalid param"
#define MMIPB_STORAGE_505_112_2_18_2_44_8_297 "[MMIPB]MMIPB_WritePhoneContact entry_id %d !!"
#define MMIPB_STORAGE_538_112_2_18_2_44_8_298 "[MMIPB]MMIPB_WritePhoneContact entry_id %d !!"
#define MMIPB_STORAGE_641_112_2_18_2_44_9_299 "MMIPB_ReadVcardContact invalid param"
#define MMIPB_STORAGE_787_112_2_18_2_44_9_300 "[MMIPB] MMIPB_WriteVcardContact invalid param !!"
#define MMIPB_STORAGE_1023_112_2_18_2_44_9_301 "mmipb_mnapi: MMIPB_GetPhonebookInfo storage = %d"
#define MMIPB_TASK_174_112_2_18_2_44_11_302 "[MMIPB]PBTask_Entry: SigCode unknown"
#define MMIPB_TASK_191_112_2_18_2_44_11_303 "MMIPB_CreateTask, pb task hasn't been closed!"
#define MMIPB_TASK_207_112_2_18_2_44_11_304 "MMIPB_CreateTask, g_mmipb_pb_task_id = %d"
#define MMIPB_VIEW_1765_112_2_18_2_44_17_305 "MMIPB_CreateMostUsedContactTabWin PNULL == applet_ptr"
#define MMIPB_VIEW_1772_112_2_18_2_44_17_306 "[MMIPB]MMIPB_CreateMostUsedContactTabWin group_infor_ptr == PNULL !"
#define MMIPB_VIEW_1806_112_2_18_2_44_17_307 "MMIPB_OpenGroupWin PNULL == applet_ptr"
#define MMIPB_VIEW_1825_112_2_18_2_44_17_308 "MMIPB_CreateAllContactTabWin PNULL == applet_ptr"
#define MMIPB_VIEW_1834_112_2_18_2_44_17_309 "[MMIPB]MMIPB_CreateAllContactTabWin group_infor_ptr == PNULL !"
#define MMIPB_VIEW_1940_112_2_18_2_44_17_310 "MMIPB_GetListAllowableMaxSelectedNum PNULL == applet_ptr"
#define MMIPB_VIEW_2119_112_2_18_2_44_18_311 "[MMIPB]AppendDyncListItem dync_type %d index %d item_content_index %d"
#define MMIPB_VIEW_2227_112_2_18_2_44_18_312 "AppendDyncName PNULL == applet_ptr"
#define MMIPB_VIEW_2262_112_2_18_2_44_18_313 "AppendDyncNumber PNULL == applet_ptr"
#define MMIPB_VIEW_2372_112_2_18_2_44_18_314 " [mmipb] AppendDetailDyncPhoto PNULL == custom_photo_ptr"
#define MMIPB_VIEW_2378_112_2_18_2_44_18_315 "[mmipb] AppendDetailDyncPhoto PNULL == item_ptr"
#define MMIPB_VIEW_2410_112_2_18_2_44_18_316 "[MMIPB]AppendDetailDyncPhoto is_photo_id_exist %d"
#define MMIPB_VIEW_2482_112_2_18_2_44_19_317 "AppendDyncListItem PNULL == applet_ptr"
#define MMIPB_VIEW_2489_112_2_18_2_44_19_318 "[mmipb] AppendDyncListItem PNULL == item_ptr"
#define MMIPB_VIEW_2500_112_2_18_2_44_19_319 "[mmipb] AppendDyncListItem PNULL == list_item_ptr"
#define MMIPB_VIEW_2718_112_2_18_2_44_19_320 "MMIPB_WriteListMode PNULL == applet_ptr"
#define MMIPB_VIEW_2738_112_2_18_2_44_19_321 "MMIPB_ReadListMode PNULL == applet_ptr"
#define MMIPB_VIEW_2757_112_2_18_2_44_19_322 "[MMIPB] GetDisplayStringFromPhonebookName dist_ptr 0x%x name_ptr 0x%x"
#define MMIPB_VIEW_2887_112_2_18_2_44_19_323 "[mmipb] SwitchToAppTab PNULL == applet_ptr"
#define MMIPB_VIEW_2938_112_2_18_2_44_19_324 "GetCurAppType PNULL == applet_ptr"
#define MMIPB_VIEW_3065_112_2_18_2_44_20_325 "[mmipb MMIPB_GetContactInfoFromListIndex] PNULL == applet_ptr || PNULL == contact_ptr"
#define MMIPB_VIEW_3074_112_2_18_2_44_20_326 "[mmipb MMIPB_GetContactInfoByListIndex] PNULL  == list_item_ptr %d"
#define MMIPB_VIEW_3334_112_2_18_2_44_20_327 "PbCreateToolbar PNULL == applet_ptr"
#define MMIPB_VIEW_3799_112_2_18_2_44_21_328 "[MMIPB] HandleMainWinMsgPNULL == applet_ptr "
#define MMIPB_VIEW_4221_112_2_18_2_44_22_329 "[MMIPB]HandleEntryAddWinMsg MSG_SET_SELECT_FIX_CALL_RING_RETURN ring_id=%d"
#define MMIPB_VIEW_4511_112_2_18_2_44_23_330 "[MMIPB]CAN'T FIND ANY CONTACT TO INSERT"
#define MMIPB_VIEW_5231_112_2_18_2_44_24_331 "mmipb HandleMemDetailWinMsg PNULL == applet_ptr"
#define MMIPB_VIEW_6456_112_2_18_2_44_26_332 "[MMIPB] HandleIconCheckContent menu_infor_ptr %d, list_item_ptr %d "
#define MMIPB_VIEW_6460_112_2_18_2_44_26_333 "MMIPB HandleIconCheckContent item_id %d item_content_ptr->item_content_index %d"
#define MMIPB_VIEW_6781_112_2_18_2_44_27_334 "[mmipb] PbCreateDetailChildWin PNULL == applet_ptr"
#define MMIPB_VIEW_6866_112_2_18_2_44_27_335 "HandleEntryDetailPdaWinMsg list_item_ptr %d"
#define MMIPB_VIEW_6871_112_2_18_2_44_27_336 "HandleEntryDetailPdaWinMsg applet_ptr %d"
#define MMIPB_VIEW_6904_112_2_18_2_44_27_337 "[mmipb] DealWithPenDetailNumItem PNULL == menu_data_ptr"
#define MMIPB_VIEW_6911_112_2_18_2_44_27_338 "[mmipb] DealWithPenDetailNumItem PNULL == menu_data_ptr->contact_ptr"
#define MMIPB_VIEW_7019_112_2_18_2_44_27_339 "HandleEntryDetailPdaWinMsg list_item_ptr %d"
#define MMIPB_VIEW_7046_112_2_18_2_44_28_340 "HandleEntryDetailPdaWinMsg list_item_ptr %d"
#define MMIPB_VIEW_7374_112_2_18_2_44_28_341 "SetEntryDetails contact_ptr %d"
#define MMIPB_VIEW_7990_112_2_18_2_44_29_342 "[mmipb] MMIPB_SearchAndAppendItem PNULL == applet_ptr"
#define MMIPB_VIEW_7996_112_2_18_2_44_29_343 "[MMIPB]CAN'T FIND ANY CONTACT TO INSERT"
#define MMIPB_VIEW_8073_112_2_18_2_44_30_344 "[MMIPB]CAN'T FIND ANY CONTACT TO INSERT"
#define MMIPB_VIEW_8120_112_2_18_2_44_30_345 "SetListItem PNULL == applet_ptr"
#define MMIPB_VIEW_8236_112_2_18_2_44_30_346 "[mmipb] PbCreateChildWin PNULL == applet_ptr"
#define MMIPB_VIEW_8352_112_2_18_2_44_30_347 "[MMIPB]HandleWaitAndEnterPBListWinMsg MSG_OPEN_WINDOW create pb list"
#define MMIPB_VIEW_8361_112_2_18_2_44_30_348 "[MMIPB]HandleWaitAndEnterPBListWinMsg timeout"
#define MMIPB_VIEW_8376_112_2_18_2_44_30_349 "[MMIPB]HandleWaitAndEnterPBListWinMsg create pb list is_key_locked %d"
#define MMIPB_VIEW_8394_112_2_18_2_44_30_350 "[MMIPB]HandleWaitAndEnterPBListWinMsg user exit"
#define MMIPB_VIEW_8502_112_2_18_2_44_31_351 "[mmipb] HandleToolBarAction PNULL == contact_ptr"
#define MMIPB_VIEW_8956_112_2_18_2_44_32_352 "[MMIPB]HandlePDAEntryListWinMsg win id 0x%x msg_id 0x%x"
#define MMIPB_VIEW_8973_112_2_18_2_44_32_353 "[MMIPB] MSG_NOTIFY_TPMOVE_LEFT_SIDE"
#define MMIPB_VIEW_8978_112_2_18_2_44_32_354 "[MMIPB] MSG_NOTIFY_TPMOVE_RIGHT_SIDE"
#define MMIPB_VIEW_9063_112_2_18_2_44_32_355 "[MMIPB]HandleEntryListWinMsg MSG_CTL_TAB_SWITCH win id 0x%x "
#define MMIPB_VIEW_9143_112_2_18_2_44_33_356 "the applet handle is %d"
#define MMIPB_VIEW_9346_112_2_18_2_44_33_357 "[MMIPB] HandlePDAEntryListWinMsg PNULL == param"
#define MMIPB_VIEW_9563_112_2_18_2_44_34_358 "[MMIPB]MMIPB_InitNameEditBox PNULL == name_ptr"
#define MMIPB_VIEW_9626_112_2_18_2_44_34_359 "[MMIPB]MMIPB_GetEntryNameFromEditbox PNULL == name_ptr"
#define MMIPB_VIEW_9947_112_2_18_2_44_34_360 "[MMIPB]HandleEntryListWinMsg win id 0x%x msg_id 0x%x"
#define MMIPB_VIEW_10030_112_2_18_2_44_35_361 "[MMIPB]HandleEntryListWinMsg MSG_CTL_TAB_SWITCH win id 0x%x "
#define MMIPB_VIEW_10565_112_2_18_2_44_36_362 "[mmipb MMIPB_GetContactInfoFromListIndex] PNULL == applet_ptr || PNULL == contact_ptr"
#define MMIPB_VIEW_10580_112_2_18_2_44_36_363 "[mmipb MMIPB_GetContactInfoFromListIndex] PNULL  == list_item_ptr %d"
#define MMIPB_VIEW_10627_112_2_18_2_44_36_364 "[mmipb] DealWithTPButton PNULL ==  applet_ptr"
#define MMIPB_VIEW_10829_112_2_18_2_44_36_365 "[MMIPB]HandleWaitWinMsg red key pressed"
#define MMIPB_VIEW_10918_112_2_18_2_44_36_366 "[mmipb] MMIPB_OpenProcessWin PNULL == applet_ptr"
#define MMIPB_VIEW_11193_112_2_18_2_44_37_367 "[MMIPB] MMIPB_OpenMainWin not allowed to open !"
#define MMIPB_VIEW_11415_112_2_18_2_44_37_368 "[MMIPB] CompareGroupName name_1_ptr 0x%x"
#define MMIPB_VIEW_11494_112_2_18_2_44_37_369 "[MMIPB]PbCreateChildWin group_infor_ptr == PNULL !"
#define MMIPB_VIEW_11599_112_2_18_2_44_38_370 "[mmipb] MMIPB_GetListHandleByWin PNULL == applet_ptr "
#define MMIPB_VIEW_11629_112_2_18_2_44_38_371 "[mmipb] MMIPB_GetListHandleByWin PNULL == applet_ptr "
#define MMIPB_VIEW_11745_112_2_18_2_44_38_372 "[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr"
#define MMIPB_VIEW_11753_112_2_18_2_44_38_373 "[MMIPB]MMIAPIPB_OpenVcardFileContactWin  RET %d"
#define MMIPB_VIEW_11799_112_2_18_2_44_38_374 "[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr"
#define MMIPB_VIEW_11886_112_2_18_2_44_38_375 "[MMIPB]MMIPB_IsCurContactValid PNULL == contact_ptr"
#define MMIPB_VIEW_11900_112_2_18_2_44_38_376 "[baokun] MMIPB_IsCurMSISDNItemValid ret %d "
#define MMIPB_VIEW_11920_112_2_18_2_44_38_377 "[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr"
#define MMIPB_VIEW_11950_112_2_18_2_44_38_378 "[MMIPB] MMIPB_ReadVCardData %d"
#define MMIPB_VIEW_11974_112_2_18_2_44_38_379 "MMIAPIPB_GetPBMSISDN pb_msisdn_ptr %d"
#define MMIPB_VIEW_12015_112_2_18_2_44_39_380 "[MMIPB]MMIPB_SetEntryNameToListItem PNULL == name_ptr"
#define MMIPB_VIEW_12053_112_2_18_2_44_39_381 "[MMIPB]LoadSIMDailNumberList: dual_sys=%d contact_type %d"
#define MMIPB_VIEW_12058_112_2_18_2_44_39_382 "[mmipb] LoadFDNList PNULL == contact_ptr"
#define MMIPB_VIEW_12239_112_2_18_2_44_39_383 "[MMIPB] HandleSIMDNListWinMsg PNULL == contact_ptr"
#define MMIPB_VIEW_12386_112_2_18_2_44_39_384 "[MMIPB] HandleSIMDNListWinMsg PNULL == contact_ptr"
#define MMIPB_VIEW_12655_112_2_18_2_44_40_385 "[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail"
#define MMIPB_VIEW_12681_112_2_18_2_44_40_386 "[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail"
#define MMIPB_VIEW_12735_112_2_18_2_44_40_387 "[MMIPB] OpenSelectNumTypeWin PNULL == list_item_ptr"
#define MMIPB_VIEW_12852_112_2_18_2_44_40_388 "[MMIPB]SetEntryListItemForEx PNULL == need_item_data_ptr"
#define MMIPB_VIEW_12866_112_2_18_2_44_40_389 "[mmipb] AppendDyncListItem PNULL == list_item_ptr"
#define MMIPB_VIEW_12902_112_2_18_2_44_40_390 "[mmipb] SetEntryListItemForEx PNULL == custom_photo_ptr"
#define MMIPB_VIEW_12911_112_2_18_2_44_40_391 "[mmipb] SetEntryListItemForEx PNULL == entry_name_ptr"
#define MMIPB_VIEW_12920_112_2_18_2_44_40_392 "[mmipb] SetEntryListItemForEx PNULL == temp1_wstr_ptr"
#define MMIPB_VIEW_12931_112_2_18_2_44_40_393 "[mmipb] SetEntryListItemForEx PNULL == temp2_wstr_ptr"
#define MMIPB_VIEW_13112_112_2_18_2_44_41_394 "[MMIPB] HandleEntryExPenOk PNULL == applet_ptr"
#define MMIPB_VIEW_13170_112_2_18_2_44_41_395 "[mmipb MMIPB_GetContactInfoFromListIndex] PNULL  == list_item_ptr %d"
#define MMIPB_VIEW_13184_112_2_18_2_44_41_396 "[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail"
#define MMIPB_VIEW_13410_112_2_18_2_44_42_397 "[mmipb] HandleResultForCallLog PNULL ==  applet_ptr"
#define MMIPB_VIEW_13514_112_2_18_2_44_42_398 "[MMIPB] MMIPB_OpenEntryListForEx type %d"
#define MMIPB_VIEW_14529_112_2_18_2_44_44_399 "[mmipb], MMIPB_OpenOperationSelectWin alloc data fail"
#define MMIPB_VIEW_14571_112_2_18_2_44_44_400 "PNULL == applet_ptr"
#define MMIPB_VIEW_14586_112_2_18_2_44_44_401 "[MMIPB] ALLOC add_data.contact_ptr ERROR "
#define MMIPB_VIEW_14641_112_2_18_2_44_44_402 "[MMIPB] MMIPB_AddToBlackList PNULL == applet_ptr"
#define MMIPB_VIEW_14776_112_2_18_2_44_44_403 "[MMIPB] MMIPB_OpenDetailWin, PNULL == contact_ptr"
#define MMIPB_VIEW_15112_112_2_18_2_44_45_404 "[mmipb] MMIPB_LocalSearch not ready !"
#define MMIPB_VIEW_15118_112_2_18_2_44_45_405 "[mmipb], MMIPB_LocalSearch PNULL == match_data_ptr || PNULL == match_data_ptr->match_item_ptr"
#define MMIPB_VIEW_15125_112_2_18_2_44_45_406 "[MMIPB]CAN'T FIND ANY CONTACT TO INSERT"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PB_TRC)
TRACE_MSG(MMIPB_ADAPT_141_112_2_18_2_42_58_0,"[MMIPB]InitPBFolder Udisk and SDCard not exist !!")
TRACE_MSG(MMIPB_ADAPT_397_112_2_18_2_42_59_1,"[MMIPB]MMIPB_SavePhotoIDInfoToFile file_ret %d")
TRACE_MSG(MMIPB_ADAPT_459_112_2_18_2_42_59_2,"[MMIPB]MMIPB_ReadPhotoFileName file_ret %d")
TRACE_MSG(MMIPB_ADAPT_509_112_2_18_2_42_59_3,"[MMIPB]MMIPB_SaveRingInfoToFile file_ret %d")
TRACE_MSG(MMIPB_ADAPT_576_112_2_18_2_42_59_4,"[MMIPB]MMIPB_ReadRingFileName file_ret %d")
TRACE_MSG(MMIPB_ADAPT_648_112_2_18_2_42_59_5,"[MMIPB]MMIPB_SaveRingInfoToFile file_ret %d")
TRACE_MSG(MMIPB_ADAPT_713_112_2_18_2_42_59_6,"[MMIPB]MMIPB_ReadRingFileName file_ret %d")
TRACE_MSG(MMIPB_ADAPT_761_112_2_18_2_43_0_7,"[MMIPB]MMIPB_SaveASPInfoToFile file_ret %d")
TRACE_MSG(MMIPB_ADAPT_826_112_2_18_2_43_0_8,"[MMIPB]MMIPB_ReadRingFileName file_ret %d")
TRACE_MSG(MMIPB_ADAPT_1275_112_2_18_2_43_1_9,"MMIPB_ContactToUSIMAddtionalContact contact_ptr 0x%x")
TRACE_MSG(MMIPB_ADAPT_1327_112_2_18_2_43_1_10,"MMIPB_ContactToUSIMAddtionalContact contact_ptr 0x%x")
TRACE_MSG(MMIPB_ADAPT_1761_112_2_18_2_43_2_11,"[MMIPB] MMIPB_WstrToInternalString invalid param !")
TRACE_MSG(MMIPB_ADAPT_1899_112_2_18_2_43_2_12,"PHONEBOOK_EncodeAlphabetToSimStandard return flase !")
TRACE_MSG(MMIPB_ADAPT_1939_112_2_18_2_43_2_13,"[MMIPB] MMIPB_GSMAlphabetToWstr invalid param !")
TRACE_MSG(MMIPB_ADAPT_1954_112_2_18_2_43_2_14,"MMIPB_GSMAlphabetToWstr decode alphabet with sim standard is error")
TRACE_MSG(MMIPB_ADAPT_2166_112_2_18_2_43_2_15,"[MMIPB] MMIPB_BCDNumberToUTF8 invalid param !")
TRACE_MSG(MMIPB_ADAPT_2220_112_2_18_2_43_2_16,"[MMIPB] MMIPB_BCDNumberToString invalid param !")
TRACE_MSG(MMIPB_ADAPT_2254_112_2_18_2_43_3_17,"MMIPB_GetNameLetters name_ptr %d ")
TRACE_MSG(MMIPB_ADAPT_2665_112_2_18_2_43_3_18,"[MMIPB]MMIPB_ConvertNameToString name_ptr 0x%x str_ptr 0x%x!")
TRACE_MSG(MMIPB_APP_258_112_2_18_2_43_5_19,"MMIPB_StartApplet")
TRACE_MSG(MMIPB_APP_353_112_2_18_2_43_5_20,"mmi Applet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIPB_APP_453_112_2_18_2_43_5_21,"[mmipb] MMIPB_CreateSelNode PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_459_112_2_18_2_43_5_22,"MMIPB_CreateSelNode PNULL== s_sel_node_ptr")
TRACE_MSG(MMIPB_APP_471_112_2_18_2_43_5_23,"MMIPB_CreateSelNode PNULL== handle_node_ptr ->select_listr")
TRACE_MSG(MMIPB_APP_529_112_2_18_2_43_5_24,"[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_535_112_2_18_2_43_5_25,"[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_data_ptr")
TRACE_MSG(MMIPB_APP_603_112_2_18_2_43_5_26,"[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_613_112_2_18_2_43_5_27,"[MMIPB] MMIPB_DeleteSelHeadNode handle_data_ptr->sel_count %d")
TRACE_MSG(MMIPB_APP_663_112_2_18_2_43_5_28,"[MMIPB] MMIPB_HasSelNode PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_690_112_2_18_2_43_5_29,"[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_699_112_2_18_2_43_5_30,"[MMIPB]MMIPB_DeleteSelNode list is PNULL handle %d")
TRACE_MSG(MMIPB_APP_740_112_2_18_2_43_5_31,"[MMIPB] MMIPB_DeleteAllSelNode PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_902_112_2_18_2_43_6_32,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1005_112_2_18_2_43_6_33,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1082_112_2_18_2_43_6_34,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1158_112_2_18_2_43_6_35,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1230_112_2_18_2_43_6_36,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1374_112_2_18_2_43_7_37,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1462_112_2_18_2_43_7_38,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1552_112_2_18_2_43_7_39,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1558_112_2_18_2_43_7_40,"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == applet_ptr")
TRACE_MSG(MMIPB_APP_1718_112_2_18_2_43_7_41,"[MMIPB] mmipb_app_callback handle %d error %d")
TRACE_MSG(MMIPB_APP_1762_112_2_18_2_43_8_42,"[MMIPB][PB]MMIPB_CheckSpace needed_record %d max_record %d")
TRACE_MSG(MMIPB_APP_1787_112_2_18_2_43_8_43,"[mmipb] MMIPB_SetOpcType PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_1810_112_2_18_2_43_8_44,"[mmipb] MMIPB_AddContact not ready !")
TRACE_MSG(MMIPB_APP_1861_112_2_18_2_43_8_45,"[MMIPB] MMIPB_UpdatePhonebookEntry (PNULL == contact_ptr) !")
TRACE_MSG(MMIPB_APP_1874_112_2_18_2_43_8_46,"[MMIPB][PB] MMIPB_UpdatePhonebookEntry result %d")
TRACE_MSG(MMIPB_APP_1893_112_2_18_2_43_8_47,"[mmipb] MMIAPIPB_DeleteContact not ready !")
TRACE_MSG(MMIPB_APP_1896_112_2_18_2_43_8_48,"[MMIPB] MMIAPIPB_DeleteContact, storage=%d, entry_id=%d,")
TRACE_MSG(MMIPB_APP_1914_112_2_18_2_43_8_49,"[MMIPB] MMIPB_DeleteAllMostUsedContact result %")
TRACE_MSG(MMIPB_APP_1928_112_2_18_2_43_8_50,"[mmipb] MMIPB_AddContact not ready !")
TRACE_MSG(MMIPB_APP_1971_112_2_18_2_43_8_51,"[MMIPB] MMIPB_UpdateContactAsyn (PNULL == contact_ptr) !")
TRACE_MSG(MMIPB_APP_1977_112_2_18_2_43_8_52,"[MMIPB] MMIPB_UpdateContactAsyn (PNULL == update_contact_ptr) !")
TRACE_MSG(MMIPB_APP_1990_112_2_18_2_43_8_53,"[MMIPB][PB] MMIPB_UpdateContactAsyn result %d")
TRACE_MSG(MMIPB_APP_2004_112_2_18_2_43_8_54,"[mmipb] MMIPB_AddContact not ready !")
TRACE_MSG(MMIPB_APP_2034_112_2_18_2_43_8_55,"[mmipb]MMIPB_AddContactAsyn, ret %d")
TRACE_MSG(MMIPB_APP_2114_112_2_18_2_43_8_56,"[mmipb] MMIPB_AddContact not ready !")
TRACE_MSG(MMIPB_APP_2119_112_2_18_2_43_8_57,"[mmipb] MMIPB_ModifyContactAsyn PNULL == contact_ptr!")
TRACE_MSG(MMIPB_APP_2147_112_2_18_2_43_9_58,"[mmipb]MMIPB_ModifyContactAsyn, ret %d")
TRACE_MSG(MMIPB_APP_2193_112_2_18_2_43_9_59,"[MMIPB] MMIPB_ExportVcard (PNULL == mmipb_contact_ptr) !")
TRACE_MSG(MMIPB_APP_2200_112_2_18_2_43_9_60,"[mmipb]  MMIPB_ExportVcard  error ret %d")
TRACE_MSG(MMIPB_APP_2261_112_2_18_2_43_9_61,"[mmipb] MMIPB_AddContact not ready !")
TRACE_MSG(MMIPB_APP_2267_112_2_18_2_43_9_62,"[MMIPB] MMIPB_UpdateContactAsyn (PNULL == mmipb_contact_ptr) !")
TRACE_MSG(MMIPB_APP_2276_112_2_18_2_43_9_63,"[mmipb] MMIAPIPB_ExportVcard error ret %d")
TRACE_MSG(MMIPB_APP_2295_112_2_18_2_43_9_64,"[mmipb]MMIPB_ExportVcardAsyn, ret %d")
TRACE_MSG(MMIPB_APP_2319_112_2_18_2_43_9_65,"[mmipb] MMIPB_AddContact not ready !")
TRACE_MSG(MMIPB_APP_2344_112_2_18_2_43_9_66,"[mmipb] MMIAPIPB_ExportContactAsyn not ready !")
TRACE_MSG(MMIPB_APP_2349_112_2_18_2_43_9_67,"[mmipb] MMIAPIPB_ExportContactAsyn PNULL == handle_node_ptr !")
TRACE_MSG(MMIPB_APP_2368_112_2_18_2_43_9_68,"[mmipb]MMIPB_ExportContactAsyn there is not contact to delete")
TRACE_MSG(MMIPB_APP_2392_112_2_18_2_43_9_69,"[mmipb] MMIPB_AddContact not ready !")
TRACE_MSG(MMIPB_APP_2399_112_2_18_2_43_9_70,"[MMIPB]MMIPB_ImportVcard MMIPB_Read RET %d")
TRACE_MSG(MMIPB_APP_2451_112_2_18_2_43_9_71,"[mmipb] MMIPB_AddContact not ready !")
TRACE_MSG(MMIPB_APP_2488_112_2_18_2_43_9_72,"[mmipb] MMIAPIPB_ExportContactAsyn not ready !")
TRACE_MSG(MMIPB_APP_2493_112_2_18_2_43_9_73,"[mmipb] MMIAPIPB_ExportContactAsyn PNULL == handle_node_ptr !")
TRACE_MSG(MMIPB_APP_2518_112_2_18_2_43_9_74,"[mmipb]MMIPB_ExportContactAsyn there is not contact to delete")
TRACE_MSG(MMIPB_APP_2543_112_2_18_2_43_9_75,"[mmipb] MMIPB_DeleteContact not ready !")
TRACE_MSG(MMIPB_APP_2550_112_2_18_2_43_9_76,"[MMIPB] MMIPB_DeleteContactNode, entry_id=%d, storage_id %d")
TRACE_MSG(MMIPB_APP_2555_112_2_18_2_43_9_77,"[mmipb]MMIPB_DeleteContactAsyn, result %d")
TRACE_MSG(MMIPB_APP_2587_112_2_18_2_43_9_78,"[mmipb] MMIPB_DeleteContactAsynForPB not ready !")
TRACE_MSG(MMIPB_APP_2592_112_2_18_2_43_9_79,"[mmipb] MMIPB_DeleteContactAsynForPB PNULL == handle_node_ptr !")
TRACE_MSG(MMIPB_APP_2608_112_2_18_2_43_9_80,"[mmipb]MMIPB_DeleteContactAsynForPB there is not contact to delete")
TRACE_MSG(MMIPB_APP_2631_112_2_18_2_43_10_81,"[mmipb] MMIPB_GetContactInfo not ready !")
TRACE_MSG(MMIPB_APP_2681_112_2_18_2_43_10_82,"[mmipb] MMIPB_GetContactInfo not ready !")
TRACE_MSG(MMIPB_APP_2898_112_2_18_2_43_10_83,"[mmipb] MMIPB_GetContactInfo not ready !")
TRACE_MSG(MMIPB_APP_2904_112_2_18_2_43_10_84,"[mmipb]MMIPB_CopyContactAsyn PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_2945_112_2_18_2_43_10_85,"[mmipb]MMIPB_CopyContactAsyn, ret %d")
TRACE_MSG(MMIPB_APP_2988_112_2_18_2_43_10_86,"[mmipb] MMIPB_GetContactInfo not ready !")
TRACE_MSG(MMIPB_APP_2993_112_2_18_2_43_10_87,"[mmipb]MMIPB_CopyMultiContactAsyn PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_3125_112_2_18_2_43_11_88,"[mmipb] MMIPB_WriteGroupName name->wstr_len %d")
TRACE_MSG(MMIPB_APP_3162_112_2_18_2_43_11_89,"[mmipb] MMIPB_GetSelectCount not ready !")
TRACE_MSG(MMIPB_APP_3169_112_2_18_2_43_11_90,"[mmipb] MMIPB_GetSelectCount PNULL == handle_node_ptr !")
TRACE_MSG(MMIPB_APP_3176_112_2_18_2_43_11_91,"[mmipb] MMIPB_GetSelectCount PNULL == applet_ptr !")
TRACE_MSG(MMIPB_APP_3193_112_2_18_2_43_11_92,"[mmipb] MMIPB_GetSelectCount PNULL == handle_node_ptr ->select_file_list !")
TRACE_MSG(MMIPB_APP_3313_112_2_18_2_43_11_93,"[MMIPB]MMIPB_HandleCallReadyCnf call_ready_param_ptr 0x%x ")
TRACE_MSG(MMIPB_APP_3321_112_2_18_2_43_11_94,"[MMIPB]MMIPB_HandleCallReadyCnf dual_sys %d")
TRACE_MSG(MMIPB_APP_3456_112_2_18_2_43_11_95,"[MMIPB]HandlePBPsMsg 0x%x")
TRACE_MSG(MMIPB_APP_3461_112_2_18_2_43_11_96,"[MMIPB]HandlePBPsMsg MMIPB_HandleCallReadyCnf")
TRACE_MSG(MMIPB_APP_3475_112_2_18_2_43_11_97,"[MMIPB]HandlePBPsMsg APP_MN_SIM_NOT_READY_IND")
TRACE_MSG(MMIPB_APP_3564_112_2_18_2_43_11_98,"[MMIPB]: HandleSIMPlugNotifyPBFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMIPB_APP_3659_112_2_18_2_43_12_99,"[MMIPB]MMIPB_InitSimStatus  dual_sys %d")
TRACE_MSG(MMIPB_APP_3663_112_2_18_2_43_12_100,"[MMIPB]MMIPB_InitSimStatus dual_sys %d")
TRACE_MSG(MMIPB_APP_3681_112_2_18_2_43_12_101,"[MMIPB]MMIPB_HandlePbSimNotReadyInd (PNULL == sim_not_ready_param_ptr) !")
TRACE_MSG(MMIPB_APP_3688_112_2_18_2_43_12_102,"[MMIPB]MMIPB_HandlePbSimNotReadyInd sim_not_ready_param_ptr->dual_sys %d!")
TRACE_MSG(MMIPB_APP_3695_112_2_18_2_43_12_103,"[MMIPB]MMIPB_HandlePbSimNotReadyInd dual_sys %d sim_not_ready_param_ptr->reason %d")
TRACE_MSG(MMIPB_APP_3727_112_2_18_2_43_12_104,"[MMIPB]MMIPB_HandlePbSimNotReadyInd is_need_response %d !")
TRACE_MSG(MMIPB_APP_3940_112_2_18_2_43_12_105,"[mmipb] MMIPB_InitEntryList read sim  contact error entry_id %d, sim_type %d")
TRACE_MSG(MMIPB_APP_3964_112_2_18_2_43_12_106,"[mmipb] MMIPB_InitEntryList read nv contact error entry_id %d")
TRACE_MSG(MMIPB_APP_3982_112_2_18_2_43_12_107,"[mmipb] MMIPB_InitEntryList read mostused contact error entry_id %d")
TRACE_MSG(MMIPB_APP_3995_112_2_18_2_43_12_108,"[MMIPB][SNS] MMIPB_InitEntryList read sns contact num = %d")
TRACE_MSG(MMIPB_APP_4038_112_2_18_2_43_12_109,"[MMIPB]MMIPB_ReadUSIMExtCnf enter usim_ext_ptr %d")
TRACE_MSG(MMIPB_APP_4042_112_2_18_2_43_12_110,"[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->adn_entry_id %d dual_sys")
TRACE_MSG(MMIPB_APP_4045_112_2_18_2_43_12_111,"[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.anr_ret[%d] %d")
TRACE_MSG(MMIPB_APP_4052_112_2_18_2_43_12_112,"[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.email_ret %d email len %d")
TRACE_MSG(MMIPB_APP_4057_112_2_18_2_43_12_113,"[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.sne_ret %d sne len %d")
TRACE_MSG(MMIPB_APP_4105_112_2_18_2_43_12_114,"[MMIPB] HandleEntryUpdateResult (update_result == PNULL)")
TRACE_MSG(MMIPB_APP_4113_112_2_18_2_43_12_115,"[MMIPB]HandleEntryUpdateResult update_result %d !")
TRACE_MSG(MMIPB_APP_4119_112_2_18_2_43_12_116,"[MMIPB]HandleEntryUpdateResult update_result->update_result.adn_flag %d !cur_num_index %d entry id %d")
TRACE_MSG(MMIPB_APP_4199_112_2_18_2_43_13_117,"[MMIPB] delete_result PNULL == delete_result !")
TRACE_MSG(MMIPB_APP_4202_112_2_18_2_43_13_118,"[MMIPB]HandleEntryDeleteResult, delete_result adn_result%d dual_sys %d")
TRACE_MSG(MMIPB_APP_4232_112_2_18_2_43_13_119,"[MMIPB] MMIPB_SetPBStatus status %d")
TRACE_MSG(MMIPB_APP_4246_112_2_18_2_43_13_120,"[MMIPB] MMIPB_GetPBStatus status %d")
TRACE_MSG(MMIPB_APP_4263_112_2_18_2_43_13_121,"[MMIPB]MMIPB_SetNumberUsed pb not ready !")
TRACE_MSG(MMIPB_APP_4320_112_2_18_2_43_13_122,"[mmipb]MMIPB_ReadSearchedName PNULL == contact_ptr")
TRACE_MSG(MMIPB_APP_4327_112_2_18_2_43_13_123,"[mmipb]MMIPB_ReadSearchedName PNULL == handle_node_ptr ")
TRACE_MSG(MMIPB_APP_4335_112_2_18_2_43_13_124,"[mmipb]MMIPB_ReadSearchedName PNULL == mmipb_handle_data")
TRACE_MSG(MMIPB_APP_4347_112_2_18_2_43_13_125,"[mmipb]MMIPB_ReadSearchedName index %d")
TRACE_MSG(MMIPB_APP_4409_112_2_18_2_43_13_126,"[mmipb] MMIPB_ReadSearchedName PNULL == nameber_ptr")
TRACE_MSG(MMIPB_APP_4441_112_2_18_2_43_13_127,"[mmipb] MMIPB_ReadSearchedName PNULL == nameber_ptr")
TRACE_MSG(MMIPB_APP_4448_112_2_18_2_43_13_128,"[mmipb] MMIPB_GetSelectNumber PNULL == contact_ptr")
TRACE_MSG(MMIPB_APP_4482_112_2_18_2_43_13_129,"[mmipb] MMIPB_GetSelectVcard PNULL == data_ptr")
TRACE_MSG(MMIPB_APP_4490_112_2_18_2_43_13_130,"mmipb, MMIPB_GetSelectVcardPNULL == contact_ptr ")
TRACE_MSG(MMIPB_APP_4513_112_2_18_2_43_13_131,"[MMIPB]MMIPB_AbortPB is_abort %d")
TRACE_MSG(MMIPB_APP_4525_112_2_18_2_43_13_132,"[MMIPB]MMIPB_AbortPB is_abort %d")
TRACE_MSG(MMIPB_APP_4639_112_2_18_2_43_14_133,"[MMIPB] MMIPB_GetCustomPhotoPathByNumber invalid param !")
TRACE_MSG(MMIPB_APP_4644_112_2_18_2_43_14_134,"[MMIPB]MMIPB_GetCustomPhotoPathByNumber not find !!")
TRACE_MSG(MMIPB_APP_4711_112_2_18_2_43_14_135,"[MMIPB] MMIPB_GetCustomASPFileByNumber invalid param !")
TRACE_MSG(MMIPB_APP_4715_112_2_18_2_43_14_136,"[MMIPB]MMIPB_GetCustomASPFileByNumber entered")
TRACE_MSG(MMIPB_APP_4719_112_2_18_2_43_14_137,"[MMIPB]MMIPB_GetCustomASPFileByNumber not find !!")
TRACE_MSG(MMIPB_APP_4753_112_2_18_2_43_14_138,"[MMIPB] MMIPB_GetCustomASPFileByNumber has deleted asp pb")
TRACE_MSG(MMIPB_APP_4852_112_2_18_2_43_14_139,"[MMIPB] MMIPB_GetCustomRingFileByNumber invalid param !")
TRACE_MSG(MMIPB_APP_4856_112_2_18_2_43_14_140,"[MMIPB]MMIPB_GetCustomRingFileByNumber entered")
TRACE_MSG(MMIPB_APP_4860_112_2_18_2_43_14_141,"[MMIPB]MMIPB_GetCustomRingFileByNumber not find !!")
TRACE_MSG(MMIPB_APP_4895_112_2_18_2_43_14_142,"[MMIPB] MMIPB_GetCustomRingFileByNumber has deleted  pb")
TRACE_MSG(MMIPB_APP_5021_112_2_18_2_43_14_143,"[mmipb] MMIPB_AddContactToGroupAsynForPB not ready !")
TRACE_MSG(MMIPB_APP_5026_112_2_18_2_43_14_144,"[mmipb] MMIPB_AddContactToGroupAsynForPB PNULL == handle_node_ptr !")
TRACE_MSG(MMIPB_APP_5061_112_2_18_2_43_14_145,"[mmipb]MMIPB_AddContactToGroupAsynForPB there is not contact")
TRACE_MSG(MMIPB_APP_5129_112_2_18_2_43_14_146,"[mmipb] MMIPB_SendGroupManage PNULL == handle_node_ptr !")
TRACE_MSG(MMIPB_APP_5200_112_2_18_2_43_15_147,"[mmipb] MMIPB_AddContactToFavouriteAsynForPB not ready !")
TRACE_MSG(MMIPB_APP_5205_112_2_18_2_43_15_148,"[mmipb] MMIPB_AddContactToFavouriteAsynForPB PNULL == handle_node_ptr !")
TRACE_MSG(MMIPB_APP_5221_112_2_18_2_43_15_149,"[mmipb]MMIPB_AddContactToFavouriteAsynForPB there is not contact to delete")
TRACE_MSG(MMIPB_APP_5288_112_2_18_2_43_15_150,"[mmipb] MMIPB_AddContactToFavouriteAsynForPB not ready !")
TRACE_MSG(MMIPB_APP_5293_112_2_18_2_43_15_151,"[mmipb] MMIPB_AddContactToFavouriteAsynForPB PNULL == handle_node_ptr !")
TRACE_MSG(MMIPB_APP_5309_112_2_18_2_43_15_152,"[mmipb]MMIPB_AddContactToFavouriteAsynForPB there is not contact to delete")
TRACE_MSG(MMIPB_APP_5389_112_2_18_2_43_15_153,"[MMIPB]MakeNumberCallByBCDNumber number_str_ptr == PNULL !")
TRACE_MSG(MMIPB_APP_5530_112_2_18_2_43_15_154,"[MMIPB]MMIPB_CallBCDNumber !")
TRACE_MSG(MMIPB_APP_5723_112_2_18_2_43_16_155,"[MMIPB] ikeybackupcallback invalid param !")
TRACE_MSG(MMIPB_APP_5731_112_2_18_2_43_16_156,"[MMIPB] ikeybackupcallback type error !")
TRACE_MSG(MMIPB_APP_5807_112_2_18_2_43_16_157,"[MMIPB] ikeyrestore_callback invalid param !")
TRACE_MSG(MMIPB_APP_5921_112_2_18_2_43_16_158,"[mmipb] MMIAPIPB_GetMSISDNumber, PNULL == contact_ptr")
TRACE_MSG(MMIPB_APP_5943_112_2_18_2_43_16_159,"[mmipb] MMIPB_GetMSISDNumber j %d, max_num %d")
TRACE_MSG(MMIPB_APP_5973_112_2_18_2_43_16_160,"[mmipb] MMIAPIPB_GetMSISDNumber, PNULL == contact_ptr")
TRACE_MSG(MMIPB_APP_5993_112_2_18_2_43_16_161,"[mmipb] MMIPB_GetMSISDNumberBySim j %d, max_num %d")
TRACE_MSG(MMIPB_ATC_113_112_2_18_2_43_17_162,"[MMIPB] PNULL == pb_head_info_ptr !")
TRACE_MSG(MMIPB_ATC_157_112_2_18_2_43_17_163,"[MMIATC] max_record_num %d max_alpha_len %d max_num_len %d max_email_len %d !")
TRACE_MSG(MMIPB_ATC_215_112_2_18_2_43_17_164,"[MMIPB]MMIPB_SyncRead  PNULL == pb_item_info_ptr !")
TRACE_MSG(MMIPB_ATC_249_112_2_18_2_43_17_165,"MMIPB_SyncRead, read from nv")
TRACE_MSG(MMIPB_ATC_335_112_2_18_2_43_17_166,"[MMIPB]MMIPB_SyncAdd pb_item_info_ptr %d entry_id !")
TRACE_MSG(MMIPB_ATC_362_112_2_18_2_43_17_167,"MMIPB_SyncAdd, add to nv")
TRACE_MSG(MMIPB_ATC_413_112_2_18_2_43_17_168,"[MMIPB] CopySyncEntryToStoreEntry (PNULL == pb_entry_ptr) || (PNULL == pb_item_info_ptr) !")
TRACE_MSG(MMIPB_ATC_496_112_2_18_2_43_17_169,"[MMIPB]MMIPB_SyncDelete entry_id %d !")
TRACE_MSG(MMIPB_ATC_506_112_2_18_2_43_17_170,"MMIPB_SyncDelete from nv")
TRACE_MSG(MMIPB_ATC_576_112_2_18_2_43_18_171,"MMIPB_SyncModify in nv")
TRACE_MSG(MMIPB_ATC_618_112_2_18_2_43_18_172,"[MMIPB] MMIPBGROUP_SyncHeadInfo (NULL == group_head_info_ptr) !")
TRACE_MSG(MMIPB_ATC_631_112_2_18_2_43_18_173,"[MMIPB] MMIPBGROUP_SyncHeadInfo max_group_num %d max_name_len %d !")
TRACE_MSG(MMIPB_DATALIST_185_112_2_18_2_43_18_174,"[MMIPB]SetEntryIdFlag entry_id 0x%x pb_sort_ptr %d")
TRACE_MSG(MMIPB_DATALIST_382_112_2_18_2_43_19_175,"[MMIPB]MMIPB_GetContactNameNumberByIndex invalid param ")
TRACE_MSG(MMIPB_DATALIST_568_112_2_18_2_43_19_176,"CompareLettersInSortTable valid parameter !!")
TRACE_MSG(MMIPB_DATALIST_615_112_2_18_2_43_19_177,"CompareLettersInSortTable valid parameter !!")
TRACE_MSG(MMIPB_DATALIST_981_112_2_18_2_43_20_178,"[mmipb] FindDailNumberContact PNULL == contact_ptr")
TRACE_MSG(MMIPB_DATALIST_1255_112_2_18_2_43_21_179,"ContactListCmp valid parameter !!")
TRACE_MSG(MMIPB_DATALIST_1296_112_2_18_2_43_21_180,"[MMIPB] MMIPB_InsertContactListNode max_count %d, count %d")
TRACE_MSG(MMIPB_DATALIST_1302_112_2_18_2_43_21_181,"[MMIPB]MMIPB_InsertQListNode letters_ptr == PNULL !")
TRACE_MSG(MMIPB_DATALIST_1764_112_2_18_2_43_22_182,"MostUsedContactListCmp valid parameter !!")
TRACE_MSG(MMIPB_DATALIST_1770_112_2_18_2_43_22_183,"MostUsedContactListCmp ret %d ")
TRACE_MSG(MMIPB_DATALIST_1789_112_2_18_2_43_22_184,"[MMIPB] MMIPB_InsertMostUsedContactListNode invalid param !")
TRACE_MSG(MMIPB_DATALIST_2487_112_2_18_2_43_23_185,"[MMIPB]MMIPB_FindFirstNotInitEntryID parameter invalid !")
TRACE_MSG(MMIPB_DATALIST_2509_112_2_18_2_43_23_186,"[MMIPB]MMIPB_FindFirstNotInitEntryID result %d")
TRACE_MSG(MMIPB_DATALIST_2532_112_2_18_2_43_23_187,"[MMIPB] MMIPB_ReadUSIMAddtionalFromContactList invalid param !")
TRACE_MSG(MMIPB_DATALIST_2589_112_2_18_2_43_24_188,"[MMIPB] MMIPB_SaveAddtionToList invalid param !")
TRACE_MSG(MMIPB_EXPORT_1136_112_2_18_2_43_26_189,"MMIAPIPB_Wstrstrpos pnull")
TRACE_MSG(MMIPB_INTERFACE_335_112_2_18_2_43_28_190,"[MMIPB] MMIPB_WriteTrigger")
TRACE_MSG(MMIPB_INTERFACE_354_112_2_18_2_43_28_191,"[MMIPB] MMIPB_DeleteTrigger")
TRACE_MSG(MMIPB_INTERFACE_421_112_2_18_2_43_28_192,"[baokun] MMIPB_Write invalid param !")
TRACE_MSG(MMIPB_INTERFACE_477_112_2_18_2_43_28_193,"[baokun] MMIPB_Delete invalid param !")
TRACE_MSG(MMIPB_INTERFACE_510_112_2_18_2_43_28_194,"[MMIPB] MMIPB_Delete exit result %d")
TRACE_MSG(MMIPB_INTERFACE_520_112_2_18_2_43_28_195,"[MMIPB] MMIPB_SetWriteError error %d")
TRACE_MSG(MMIPB_INTERFACE_536_112_2_18_2_43_28_196,"[MMIPB] MMIPB_GetWriteError error %d")
TRACE_MSG(MMIPB_INTERFACE_546_112_2_18_2_43_28_197,"[MMIPB] MMIPB_SetDeleteError error %d")
TRACE_MSG(MMIPB_INTERFACE_563_112_2_18_2_43_28_198,"[MMIPB] MMIPB_GetDeleteError error %d")
TRACE_MSG(MMIPB_INTERFACE_579_112_2_18_2_43_28_199,"[MMIPB] MMIPB_ReadUSIMAddtionalAsyn dual_sys %d contact_id %d")
TRACE_MSG(MMIPB_INTERFACE_604_112_2_18_2_43_28_200,"[MMIPB]ReadUSimContact invalid param !")
TRACE_MSG(MMIPB_INTERFACE_652_112_2_18_2_43_28_201,"[MMIPB] SetUSIMUpdateFlag invalid param !")
TRACE_MSG(MMIPB_INTERFACE_790_112_2_18_2_43_29_202,"[MMIPB]WriteUSimContact invalid param !")
TRACE_MSG(MMIPB_INTERFACE_846_112_2_18_2_43_29_203,"[MMIPB] WriteUSIMContact contact_id %d storage_id %d result %d")
TRACE_MSG(MMIPB_INTERFACE_894_112_2_18_2_43_29_204,"[MMIPB] DeleteUSIMContact contact_id %d storage_id %d result %d")
TRACE_MSG(MMIPB_INTERFACE_1137_112_2_18_2_43_29_205,"[MMIPB] WriteContact contact_id %d storage_id %d result %d")
TRACE_MSG(MMIPB_INTERFACE_1188_112_2_18_2_43_30_206,"[MMIPB] DeleteContact contact_id %d storage_id %d result %d")
TRACE_MSG(MMIPB_INTERFACE_1210_112_2_18_2_43_30_207,"[MMIPB]ReadVcardContact invalid param !")
TRACE_MSG(MMIPB_INTERFACE_1230_112_2_18_2_43_30_208,"[MMIPB] ReadVcardContact  result %d")
TRACE_MSG(MMIPB_INTERFACE_1252_112_2_18_2_43_30_209,"[MMIPB] WriteVcardContact invalid param !")
TRACE_MSG(MMIPB_INTERFACE_1272_112_2_18_2_43_30_210,"[MMIPB]WriteVcardContact result %d")
TRACE_MSG(MMIPB_INTERFACE_1291_112_2_18_2_43_30_211,"[MMIPB]DeleteVcardContact result %d")
TRACE_MSG(MMIPB_INTERFACE_1326_112_2_18_2_43_30_212,"[MMIPB][SNS] ReadSnsContact index %d, ret %d")
TRACE_MSG(MMIPB_INTERFACE_1552_112_2_18_2_43_30_213,"[MMIPB]MMIPB_CreateHandle applet_handle %d")
TRACE_MSG(MMIPB_INTERFACE_1581_112_2_18_2_43_30_214,"[MMIPB]MMIPB_CloseHandle handle %d")
TRACE_MSG(MMIPB_INTERFACE_1665_112_2_18_2_43_31_215,"[MMIPB]MMIPB_ReadAsyn queue full !")
TRACE_MSG(MMIPB_INTERFACE_1749_112_2_18_2_43_31_216,"[MMIPB]MMIPB_WriteAsyn queue full !")
TRACE_MSG(MMIPB_INTERFACE_1769_112_2_18_2_43_31_217,"[MMIPB]MMIPB_WriteAsyn result %d")
TRACE_MSG(MMIPB_INTERFACE_1824_112_2_18_2_43_31_218,"[MMIPB]MMIPB_DeleteAsyn queue full !")
TRACE_MSG(MMIPB_INTERFACE_1844_112_2_18_2_43_31_219,"[MMIPB]MMIPB_DeleteAsyn result %d")
TRACE_MSG(MMIPB_INTERFACE_1872_112_2_18_2_43_31_220,"[MMIPB]MMIPB_IsCurrentOperation contact_id %d storage_id %d result %d")
TRACE_MSG(MMIPB_INTERFACE_1900_112_2_18_2_43_31_221,"[MMIPB]MMIPB_CmdConfirm handle 0x%x  result %d ret %d")
TRACE_MSG(MMIPB_INTERFACE_1917_112_2_18_2_43_31_222,"[MMIPB]MMIPB_GetAllNormalContactCount count %d")
TRACE_MSG(MMIPB_INTERFACE_1929_112_2_18_2_43_31_223,"[MMIPB]MMIPB_GetAllNormalContactCount count %d")
TRACE_MSG(MMIPB_INTERFACE_1947_112_2_18_2_43_31_224,"[MMIPB]MMIPB_GetAllNormalContactPhoneCount count %d")
TRACE_MSG(MMIPB_INTERFACE_1965_112_2_18_2_43_31_225,"[MMIPB]MMIPB_GetAllNormalContactMailCount count %d")
TRACE_MSG(MMIPB_INTERFACE_2007_112_2_18_2_43_31_226,"[MMIPB]MMIPB_GetContactCount storage_id 0x%x count %d")
TRACE_MSG(MMIPB_INTERFACE_2075_112_2_18_2_43_32_227,"[MMIPB]MMIPB_GetGroupContactCount group_id 0x%x count %d")
TRACE_MSG(MMIPB_INTERFACE_2181_112_2_18_2_43_32_228,"[MMIPB]MMIPB_FindFirstContact group 0x%x handle_ptr 0x%x result %d")
TRACE_MSG(MMIPB_INTERFACE_2229_112_2_18_2_43_32_229,"[MMIPB]MMIPB_FindFirstMostUsedContact  handle_ptr 0x%x result %d")
TRACE_MSG(MMIPB_INTERFACE_2314_112_2_18_2_43_32_230,"[MMIPB]MMIPB_FindNextMostContact  handle 0x%x index %d result %d")
TRACE_MSG(MMIPB_MEM_467_112_2_18_2_43_34_231,"[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr!")
TRACE_MSG(MMIPB_MEM_502_112_2_18_2_43_34_232,"[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr!")
TRACE_MSG(MMIPB_MEM_506_112_2_18_2_43_34_233,"[MMIMEM]:MMIMEM_Malloc mem_ptr =0x%x")
TRACE_MSG(MMIPB_MEM_538_112_2_18_2_43_34_234,"MMIPB_Malloc: size = %d,file = %s, line=%ld")
TRACE_MSG(MMIPB_MEM_555_112_2_18_2_43_34_235,"MMIMEM_MFree: mem_ptr = 0x%x,file = %s, line=%ld")
TRACE_MSG(MMIPB_MENU_2153_112_2_18_2_43_39_236,"HandleStorageSelectWinMsg node_id is %d")
TRACE_MSG(MMIPB_MENU_2196_112_2_18_2_43_39_237,"[MMIPB]OpenAddWin")
TRACE_MSG(MMIPB_MENU_2982_112_2_18_2_43_40_238,"MMI_PB_SET_PHOTO_FILE msg file_name_ptr is %d")
TRACE_MSG(MMIPB_MENU_3062_112_2_18_2_43_40_239,"[baokun] add MSG_CTL_PENOK param %d")
TRACE_MSG(MMIPB_MENU_3287_112_2_18_2_43_41_240,"[MMIPB] MSG_MULTIM_SELECTED_RETURN param == 0 !")
TRACE_MSG(MMIPB_MENU_3352_112_2_18_2_43_41_241,"[MMIPB] MSG_SET_SELECT_FIX_CALL_RING_RETURN param == 0 !")
TRACE_MSG(MMIPB_MENU_3357_112_2_18_2_43_41_242,"[MMIPB]HandleEntryAddWinMsg MSG_SET_SELECT_FIX_CALL_RING_RETURN ring_id=%d")
TRACE_MSG(MMIPB_MENU_3762_112_2_18_2_43_42_243,"[MMIPB]SetContactAddOrEditItem paramer illegal !")
TRACE_MSG(MMIPB_MENU_3768_112_2_18_2_43_42_244,"[MMIPB]SetContactAddOrEditItem item_id = %d,item_index = %d, is_update =%d")
TRACE_MSG(MMIPB_MENU_3919_112_2_18_2_43_42_245,"[MMIPB]SetContactAddOrEditItem index %d")
TRACE_MSG(MMIPB_MENU_3959_112_2_18_2_43_42_246,"[MMIPB]UpdateContactAddEditInfo paramer illegal !")
TRACE_MSG(MMIPB_MENU_3970_112_2_18_2_43_42_247,"[MMIPB]UpdateContactAddEditInfo cur item_id = %d")
TRACE_MSG(MMIPB_MENU_4122_112_2_18_2_43_43_248,"[MMIPB]UpdateContactAddEditInfo default error")
TRACE_MSG(MMIPB_MENU_4157_112_2_18_2_43_43_249,"[MMIPB]InitContactEditItems PNULL == add_data_ptr")
TRACE_MSG(MMIPB_MENU_4295_112_2_18_2_43_43_250,"[MMIPB]InitContactEditItems total item = %d")
TRACE_MSG(MMIPB_MENU_4412_112_2_18_2_43_43_251,"[MMIPB]OpenOperateWinInNvAdd %d == max_ucs2_len")
TRACE_MSG(MMIPB_MENU_4442_112_2_18_2_43_43_252,"[MMIPB]OpenOperateWinInNvAdd PNULL == str_ptr !")
TRACE_MSG(MMIPB_MENU_4548_112_2_18_2_43_43_253,"[MMIPB]OpenOperateWinInNvAdd ERROR index %d")
TRACE_MSG(MMIPB_MENU_5672_112_2_18_2_43_46_254,"[MMIPB] IniContactEditorCtrl PNULL == entry_ptr")
TRACE_MSG(MMIPB_MENU_5690_112_2_18_2_43_46_255,"InitContactEditorCtrl name storage_id is %d")
TRACE_MSG(MMIPB_MENU_5698_112_2_18_2_43_46_256,"InitContactEditorCtrl name max_ucs2_len is %d")
TRACE_MSG(MMIPB_MENU_6215_112_2_18_2_43_47_257,"[MMIPB] IniContactEditorCtrl PNULL == entry_ptr")
TRACE_MSG(MMIPB_MENU_6233_112_2_18_2_43_47_258,"InitContactEditorCtrl name storage_id is %d")
TRACE_MSG(MMIPB_MENU_6241_112_2_18_2_43_47_259,"InitContactEditorCtrl name max_ucs2_len is %d")
TRACE_MSG(MMIPB_MENU_6737_112_2_18_2_43_48_260,"[MMIPB] UpdateContactEditNumber delete_index >= MMIPB_MAX_NV_PHONE_NUM")
TRACE_MSG(MMIPB_MENU_7145_112_2_18_2_43_49_261,"[baokun] AddContact ret %d")
TRACE_MSG(MMIPB_MENU_7177_112_2_18_2_43_49_262,"DeleteContact applet handle is %d")
TRACE_MSG(MMIPB_MENU_7179_112_2_18_2_43_49_263,"DeleteContact applet_ptr is %d")
TRACE_MSG(MMIPB_MENU_9818_112_2_18_2_43_55_264,"MMIBT_OpenFileCallBack, file_num = %d")
TRACE_MSG(MMIPB_MENU_10196_112_2_18_2_43_56_265,"[mmipb] MMIPB_EnterPbTabOptMenu PNULL == data_ptr")
TRACE_MSG(MMIPB_MENU_10202_112_2_18_2_43_56_266,"[mmipb] MMIPB_EnterPbTabOptMenu PNULL == menu_infor_ptr")
TRACE_MSG(MMIPB_MENU_10205_112_2_18_2_43_56_267,"MMIPB_EnterPbTabOptMenu menu_infor_ptr is %d")
TRACE_MSG(MMIPB_MENU_10211_112_2_18_2_43_56_268,"[mmipb] MMIPB_EnterPbTabOptMenu PNULL == contact_ptr")
TRACE_MSG(MMIPB_MENU_10226_112_2_18_2_43_56_269,"MMIPB_EnterPbTabOptMenu the applet handle is %d")
TRACE_MSG(MMIPB_MENU_11365_112_2_18_2_43_59_270,"[mmipb] MMIPB_UpdateListContentItemForContact param null")
TRACE_MSG(MMIPB_MENU_11385_112_2_18_2_43_59_271,"[MMIPB]MMIPB_UpdateListContentItemForContact is_photo_id_exist %d")
TRACE_MSG(MMIPB_SEARCH_316_112_2_18_2_44_0_272,"[MMIPB]alloc s_pb_number_list.item_ptr failed !!")
TRACE_MSG(MMIPB_SEARCH_330_112_2_18_2_44_0_273,"[MMIPB]alloc s_pb_mostused_number_list.item_ptr failed !!")
TRACE_MSG(MMIPB_SEARCH_368_112_2_18_2_44_0_274,"[MMIPB] AddPhoneNumberToSearchList PNULL == entry_ptr !")
TRACE_MSG(MMIPB_SEARCH_412_112_2_18_2_44_0_275,"[MMIPB]MMIPB_GetNameByNumber bcd_number %d name_ptr %d")
TRACE_MSG(MMIPB_SEARCH_418_112_2_18_2_44_1_276,"[MMIPB]MMIPB_GetNameByNumber MMIAPIPB_IsPbReady not ready !!")
TRACE_MSG(MMIPB_SEARCH_497_112_2_18_2_44_1_277,"[MMIPB]MMIPB_GetNameByNumber bcd_number %d type_ptr %d")
TRACE_MSG(MMIPB_SEARCH_504_112_2_18_2_44_1_278,"[MMIPB]MMIPB_GetNameByNumber MMIAPIPB_IsPbReady not ready !!")
TRACE_MSG(MMIPB_SEARCH_709_112_2_18_2_44_1_279,"[MMIPB] InsertToList PNULL == item_ptr !")
TRACE_MSG(MMIPB_SEARCH_726_112_2_18_2_44_1_280,"[MMIPB] InsertToList search_list_ptr->count %d")
TRACE_MSG(MMIPB_SEARCH_731_112_2_18_2_44_1_281,"[MMIPB] InsertToList search_list_ptr->item_ptr == PNULL !")
TRACE_MSG(MMIPB_SEARCH_781_112_2_18_2_44_1_282,"[MMIPB] IntelligentSearch item_ptr 0x%x next_pos_ptr 0x%x !")
TRACE_MSG(MMIPB_SEARCH_784_112_2_18_2_44_1_283,"[baokun] item_ptr len %d high 0x%x, low 0x%x")
TRACE_MSG(MMIPB_SEARCH_885_112_2_18_2_44_1_284,"[MMIPB]MMIPB_SearchPhoneNumber alloc failed!")
TRACE_MSG(MMIPB_SEARCH_1094_112_2_18_2_44_2_285,"[MMIPB] MMIPB_DelNumberFromList search_list_ptr->item_ptr == PNULL !")
TRACE_MSG(MMIPB_SEARCH_1307_112_2_18_2_44_2_286,"MMIPB_MultiSearch group %d count %d")
TRACE_MSG(MMIPB_SEARCH_1336_112_2_18_2_44_2_287,"[MMIPB]MMIPB_SearchList enter %d ")
TRACE_MSG(MMIPB_SEARCH_1339_112_2_18_2_44_2_288,"[mmipb] MMIPB_SearchList not ready !")
TRACE_MSG(MMIPB_SEARCH_1497_112_2_18_2_44_3_289,"MMIPB_SearchList field_type 0x%x search_type 0x%x count %d")
TRACE_MSG(MMIPB_SEARCH_1573_112_2_18_2_44_3_290,"MMIPB_GetSearchEntry index %d result %d")
TRACE_MSG(MMIPB_SEARCH_1614_112_2_18_2_44_3_291,"MMIPB_GetSearchEntryID index %d result %d")
TRACE_MSG(MMIPB_STORAGE_195_112_2_18_2_44_8_292,"[MMIPB][MMIPB]MMIPB_GetMaxAlphaLen storage %d dual_sys %d feild %d max_len %d")
TRACE_MSG(MMIPB_STORAGE_248_112_2_18_2_44_8_293,"MMIPB_ReadPhoneContact invalid param")
TRACE_MSG(MMIPB_STORAGE_283_112_2_18_2_44_8_294,"MMIPB_ReadMostUsedContact invalid param")
TRACE_MSG(MMIPB_STORAGE_319_112_2_18_2_44_8_295,"MMIPB_ReadSIMContact invalid param")
TRACE_MSG(MMIPB_STORAGE_369_112_2_18_2_44_8_296,"MMIPB_ReadVcardContact invalid param")
TRACE_MSG(MMIPB_STORAGE_505_112_2_18_2_44_8_297,"[MMIPB]MMIPB_WritePhoneContact entry_id %d !!")
TRACE_MSG(MMIPB_STORAGE_538_112_2_18_2_44_8_298,"[MMIPB]MMIPB_WritePhoneContact entry_id %d !!")
TRACE_MSG(MMIPB_STORAGE_641_112_2_18_2_44_9_299,"MMIPB_ReadVcardContact invalid param")
TRACE_MSG(MMIPB_STORAGE_787_112_2_18_2_44_9_300,"[MMIPB] MMIPB_WriteVcardContact invalid param !!")
TRACE_MSG(MMIPB_STORAGE_1023_112_2_18_2_44_9_301,"mmipb_mnapi: MMIPB_GetPhonebookInfo storage = %d")
TRACE_MSG(MMIPB_TASK_174_112_2_18_2_44_11_302,"[MMIPB]PBTask_Entry: SigCode unknown")
TRACE_MSG(MMIPB_TASK_191_112_2_18_2_44_11_303,"MMIPB_CreateTask, pb task hasn't been closed!")
TRACE_MSG(MMIPB_TASK_207_112_2_18_2_44_11_304,"MMIPB_CreateTask, g_mmipb_pb_task_id = %d")
TRACE_MSG(MMIPB_VIEW_1765_112_2_18_2_44_17_305,"MMIPB_CreateMostUsedContactTabWin PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_1772_112_2_18_2_44_17_306,"[MMIPB]MMIPB_CreateMostUsedContactTabWin group_infor_ptr == PNULL !")
TRACE_MSG(MMIPB_VIEW_1806_112_2_18_2_44_17_307,"MMIPB_OpenGroupWin PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_1825_112_2_18_2_44_17_308,"MMIPB_CreateAllContactTabWin PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_1834_112_2_18_2_44_17_309,"[MMIPB]MMIPB_CreateAllContactTabWin group_infor_ptr == PNULL !")
TRACE_MSG(MMIPB_VIEW_1940_112_2_18_2_44_17_310,"MMIPB_GetListAllowableMaxSelectedNum PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_2119_112_2_18_2_44_18_311,"[MMIPB]AppendDyncListItem dync_type %d index %d item_content_index %d")
TRACE_MSG(MMIPB_VIEW_2227_112_2_18_2_44_18_312,"AppendDyncName PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_2262_112_2_18_2_44_18_313,"AppendDyncNumber PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_2372_112_2_18_2_44_18_314," [mmipb] AppendDetailDyncPhoto PNULL == custom_photo_ptr")
TRACE_MSG(MMIPB_VIEW_2378_112_2_18_2_44_18_315,"[mmipb] AppendDetailDyncPhoto PNULL == item_ptr")
TRACE_MSG(MMIPB_VIEW_2410_112_2_18_2_44_18_316,"[MMIPB]AppendDetailDyncPhoto is_photo_id_exist %d")
TRACE_MSG(MMIPB_VIEW_2482_112_2_18_2_44_19_317,"AppendDyncListItem PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_2489_112_2_18_2_44_19_318,"[mmipb] AppendDyncListItem PNULL == item_ptr")
TRACE_MSG(MMIPB_VIEW_2500_112_2_18_2_44_19_319,"[mmipb] AppendDyncListItem PNULL == list_item_ptr")
TRACE_MSG(MMIPB_VIEW_2718_112_2_18_2_44_19_320,"MMIPB_WriteListMode PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_2738_112_2_18_2_44_19_321,"MMIPB_ReadListMode PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_2757_112_2_18_2_44_19_322,"[MMIPB] GetDisplayStringFromPhonebookName dist_ptr 0x%x name_ptr 0x%x")
TRACE_MSG(MMIPB_VIEW_2887_112_2_18_2_44_19_323,"[mmipb] SwitchToAppTab PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_2938_112_2_18_2_44_19_324,"GetCurAppType PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_3065_112_2_18_2_44_20_325,"[mmipb MMIPB_GetContactInfoFromListIndex] PNULL == applet_ptr || PNULL == contact_ptr")
TRACE_MSG(MMIPB_VIEW_3074_112_2_18_2_44_20_326,"[mmipb MMIPB_GetContactInfoByListIndex] PNULL  == list_item_ptr %d")
TRACE_MSG(MMIPB_VIEW_3334_112_2_18_2_44_20_327,"PbCreateToolbar PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_3799_112_2_18_2_44_21_328,"[MMIPB] HandleMainWinMsgPNULL == applet_ptr ")
TRACE_MSG(MMIPB_VIEW_4221_112_2_18_2_44_22_329,"[MMIPB]HandleEntryAddWinMsg MSG_SET_SELECT_FIX_CALL_RING_RETURN ring_id=%d")
TRACE_MSG(MMIPB_VIEW_4511_112_2_18_2_44_23_330,"[MMIPB]CAN'T FIND ANY CONTACT TO INSERT")
TRACE_MSG(MMIPB_VIEW_5231_112_2_18_2_44_24_331,"mmipb HandleMemDetailWinMsg PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_6456_112_2_18_2_44_26_332,"[MMIPB] HandleIconCheckContent menu_infor_ptr %d, list_item_ptr %d ")
TRACE_MSG(MMIPB_VIEW_6460_112_2_18_2_44_26_333,"MMIPB HandleIconCheckContent item_id %d item_content_ptr->item_content_index %d")
TRACE_MSG(MMIPB_VIEW_6781_112_2_18_2_44_27_334,"[mmipb] PbCreateDetailChildWin PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_6866_112_2_18_2_44_27_335,"HandleEntryDetailPdaWinMsg list_item_ptr %d")
TRACE_MSG(MMIPB_VIEW_6871_112_2_18_2_44_27_336,"HandleEntryDetailPdaWinMsg applet_ptr %d")
TRACE_MSG(MMIPB_VIEW_6904_112_2_18_2_44_27_337,"[mmipb] DealWithPenDetailNumItem PNULL == menu_data_ptr")
TRACE_MSG(MMIPB_VIEW_6911_112_2_18_2_44_27_338,"[mmipb] DealWithPenDetailNumItem PNULL == menu_data_ptr->contact_ptr")
TRACE_MSG(MMIPB_VIEW_7019_112_2_18_2_44_27_339,"HandleEntryDetailPdaWinMsg list_item_ptr %d")
TRACE_MSG(MMIPB_VIEW_7046_112_2_18_2_44_28_340,"HandleEntryDetailPdaWinMsg list_item_ptr %d")
TRACE_MSG(MMIPB_VIEW_7374_112_2_18_2_44_28_341,"SetEntryDetails contact_ptr %d")
TRACE_MSG(MMIPB_VIEW_7990_112_2_18_2_44_29_342,"[mmipb] MMIPB_SearchAndAppendItem PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_7996_112_2_18_2_44_29_343,"[MMIPB]CAN'T FIND ANY CONTACT TO INSERT")
TRACE_MSG(MMIPB_VIEW_8073_112_2_18_2_44_30_344,"[MMIPB]CAN'T FIND ANY CONTACT TO INSERT")
TRACE_MSG(MMIPB_VIEW_8120_112_2_18_2_44_30_345,"SetListItem PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_8236_112_2_18_2_44_30_346,"[mmipb] PbCreateChildWin PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_8352_112_2_18_2_44_30_347,"[MMIPB]HandleWaitAndEnterPBListWinMsg MSG_OPEN_WINDOW create pb list")
TRACE_MSG(MMIPB_VIEW_8361_112_2_18_2_44_30_348,"[MMIPB]HandleWaitAndEnterPBListWinMsg timeout")
TRACE_MSG(MMIPB_VIEW_8376_112_2_18_2_44_30_349,"[MMIPB]HandleWaitAndEnterPBListWinMsg create pb list is_key_locked %d")
TRACE_MSG(MMIPB_VIEW_8394_112_2_18_2_44_30_350,"[MMIPB]HandleWaitAndEnterPBListWinMsg user exit")
TRACE_MSG(MMIPB_VIEW_8502_112_2_18_2_44_31_351,"[mmipb] HandleToolBarAction PNULL == contact_ptr")
TRACE_MSG(MMIPB_VIEW_8956_112_2_18_2_44_32_352,"[MMIPB]HandlePDAEntryListWinMsg win id 0x%x msg_id 0x%x")
TRACE_MSG(MMIPB_VIEW_8973_112_2_18_2_44_32_353,"[MMIPB] MSG_NOTIFY_TPMOVE_LEFT_SIDE")
TRACE_MSG(MMIPB_VIEW_8978_112_2_18_2_44_32_354,"[MMIPB] MSG_NOTIFY_TPMOVE_RIGHT_SIDE")
TRACE_MSG(MMIPB_VIEW_9063_112_2_18_2_44_32_355,"[MMIPB]HandleEntryListWinMsg MSG_CTL_TAB_SWITCH win id 0x%x ")
TRACE_MSG(MMIPB_VIEW_9143_112_2_18_2_44_33_356,"the applet handle is %d")
TRACE_MSG(MMIPB_VIEW_9346_112_2_18_2_44_33_357,"[MMIPB] HandlePDAEntryListWinMsg PNULL == param")
TRACE_MSG(MMIPB_VIEW_9563_112_2_18_2_44_34_358,"[MMIPB]MMIPB_InitNameEditBox PNULL == name_ptr")
TRACE_MSG(MMIPB_VIEW_9626_112_2_18_2_44_34_359,"[MMIPB]MMIPB_GetEntryNameFromEditbox PNULL == name_ptr")
TRACE_MSG(MMIPB_VIEW_9947_112_2_18_2_44_34_360,"[MMIPB]HandleEntryListWinMsg win id 0x%x msg_id 0x%x")
TRACE_MSG(MMIPB_VIEW_10030_112_2_18_2_44_35_361,"[MMIPB]HandleEntryListWinMsg MSG_CTL_TAB_SWITCH win id 0x%x ")
TRACE_MSG(MMIPB_VIEW_10565_112_2_18_2_44_36_362,"[mmipb MMIPB_GetContactInfoFromListIndex] PNULL == applet_ptr || PNULL == contact_ptr")
TRACE_MSG(MMIPB_VIEW_10580_112_2_18_2_44_36_363,"[mmipb MMIPB_GetContactInfoFromListIndex] PNULL  == list_item_ptr %d")
TRACE_MSG(MMIPB_VIEW_10627_112_2_18_2_44_36_364,"[mmipb] DealWithTPButton PNULL ==  applet_ptr")
TRACE_MSG(MMIPB_VIEW_10829_112_2_18_2_44_36_365,"[MMIPB]HandleWaitWinMsg red key pressed")
TRACE_MSG(MMIPB_VIEW_10918_112_2_18_2_44_36_366,"[mmipb] MMIPB_OpenProcessWin PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_11193_112_2_18_2_44_37_367,"[MMIPB] MMIPB_OpenMainWin not allowed to open !")
TRACE_MSG(MMIPB_VIEW_11415_112_2_18_2_44_37_368,"[MMIPB] CompareGroupName name_1_ptr 0x%x")
TRACE_MSG(MMIPB_VIEW_11494_112_2_18_2_44_37_369,"[MMIPB]PbCreateChildWin group_infor_ptr == PNULL !")
TRACE_MSG(MMIPB_VIEW_11599_112_2_18_2_44_38_370,"[mmipb] MMIPB_GetListHandleByWin PNULL == applet_ptr ")
TRACE_MSG(MMIPB_VIEW_11629_112_2_18_2_44_38_371,"[mmipb] MMIPB_GetListHandleByWin PNULL == applet_ptr ")
TRACE_MSG(MMIPB_VIEW_11745_112_2_18_2_44_38_372,"[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr")
TRACE_MSG(MMIPB_VIEW_11753_112_2_18_2_44_38_373,"[MMIPB]MMIAPIPB_OpenVcardFileContactWin  RET %d")
TRACE_MSG(MMIPB_VIEW_11799_112_2_18_2_44_38_374,"[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr")
TRACE_MSG(MMIPB_VIEW_11886_112_2_18_2_44_38_375,"[MMIPB]MMIPB_IsCurContactValid PNULL == contact_ptr")
TRACE_MSG(MMIPB_VIEW_11900_112_2_18_2_44_38_376,"[baokun] MMIPB_IsCurMSISDNItemValid ret %d ")
TRACE_MSG(MMIPB_VIEW_11920_112_2_18_2_44_38_377,"[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr")
TRACE_MSG(MMIPB_VIEW_11950_112_2_18_2_44_38_378,"[MMIPB] MMIPB_ReadVCardData %d")
TRACE_MSG(MMIPB_VIEW_11974_112_2_18_2_44_38_379,"MMIAPIPB_GetPBMSISDN pb_msisdn_ptr %d")
TRACE_MSG(MMIPB_VIEW_12015_112_2_18_2_44_39_380,"[MMIPB]MMIPB_SetEntryNameToListItem PNULL == name_ptr")
TRACE_MSG(MMIPB_VIEW_12053_112_2_18_2_44_39_381,"[MMIPB]LoadSIMDailNumberList: dual_sys=%d contact_type %d")
TRACE_MSG(MMIPB_VIEW_12058_112_2_18_2_44_39_382,"[mmipb] LoadFDNList PNULL == contact_ptr")
TRACE_MSG(MMIPB_VIEW_12239_112_2_18_2_44_39_383,"[MMIPB] HandleSIMDNListWinMsg PNULL == contact_ptr")
TRACE_MSG(MMIPB_VIEW_12386_112_2_18_2_44_39_384,"[MMIPB] HandleSIMDNListWinMsg PNULL == contact_ptr")
TRACE_MSG(MMIPB_VIEW_12655_112_2_18_2_44_40_385,"[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail")
TRACE_MSG(MMIPB_VIEW_12681_112_2_18_2_44_40_386,"[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail")
TRACE_MSG(MMIPB_VIEW_12735_112_2_18_2_44_40_387,"[MMIPB] OpenSelectNumTypeWin PNULL == list_item_ptr")
TRACE_MSG(MMIPB_VIEW_12852_112_2_18_2_44_40_388,"[MMIPB]SetEntryListItemForEx PNULL == need_item_data_ptr")
TRACE_MSG(MMIPB_VIEW_12866_112_2_18_2_44_40_389,"[mmipb] AppendDyncListItem PNULL == list_item_ptr")
TRACE_MSG(MMIPB_VIEW_12902_112_2_18_2_44_40_390,"[mmipb] SetEntryListItemForEx PNULL == custom_photo_ptr")
TRACE_MSG(MMIPB_VIEW_12911_112_2_18_2_44_40_391,"[mmipb] SetEntryListItemForEx PNULL == entry_name_ptr")
TRACE_MSG(MMIPB_VIEW_12920_112_2_18_2_44_40_392,"[mmipb] SetEntryListItemForEx PNULL == temp1_wstr_ptr")
TRACE_MSG(MMIPB_VIEW_12931_112_2_18_2_44_40_393,"[mmipb] SetEntryListItemForEx PNULL == temp2_wstr_ptr")
TRACE_MSG(MMIPB_VIEW_13112_112_2_18_2_44_41_394,"[MMIPB] HandleEntryExPenOk PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_13170_112_2_18_2_44_41_395,"[mmipb MMIPB_GetContactInfoFromListIndex] PNULL  == list_item_ptr %d")
TRACE_MSG(MMIPB_VIEW_13184_112_2_18_2_44_41_396,"[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail")
TRACE_MSG(MMIPB_VIEW_13410_112_2_18_2_44_42_397,"[mmipb] HandleResultForCallLog PNULL ==  applet_ptr")
TRACE_MSG(MMIPB_VIEW_13514_112_2_18_2_44_42_398,"[MMIPB] MMIPB_OpenEntryListForEx type %d")
TRACE_MSG(MMIPB_VIEW_14529_112_2_18_2_44_44_399,"[mmipb], MMIPB_OpenOperationSelectWin alloc data fail")
TRACE_MSG(MMIPB_VIEW_14571_112_2_18_2_44_44_400,"PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_14586_112_2_18_2_44_44_401,"[MMIPB] ALLOC add_data.contact_ptr ERROR ")
TRACE_MSG(MMIPB_VIEW_14641_112_2_18_2_44_44_402,"[MMIPB] MMIPB_AddToBlackList PNULL == applet_ptr")
TRACE_MSG(MMIPB_VIEW_14776_112_2_18_2_44_44_403,"[MMIPB] MMIPB_OpenDetailWin, PNULL == contact_ptr")
TRACE_MSG(MMIPB_VIEW_15112_112_2_18_2_44_45_404,"[mmipb] MMIPB_LocalSearch not ready !")
TRACE_MSG(MMIPB_VIEW_15118_112_2_18_2_44_45_405,"[mmipb], MMIPB_LocalSearch PNULL == match_data_ptr || PNULL == match_data_ptr->match_item_ptr")
TRACE_MSG(MMIPB_VIEW_15125_112_2_18_2_44_45_406,"[MMIPB]CAN'T FIND ANY CONTACT TO INSERT")
END_TRACE_MAP(MMI_APP_PB_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PB_TRC_H_

