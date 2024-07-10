/******************************************************************************
 ** File Name:      mmi_app_stk_trc.h                                         *
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
//trace_id:224
#ifndef _MMI_APP_STK_TRC_H_
#define _MMI_APP_STK_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMISTK_APP_145_112_2_18_2_55_50_0 "[MMI_STK]:MMIAPISTK_CnfIdleScrAva"
#define MMISTK_APP_165_112_2_18_2_55_51_1 "[MMI_STK]:MMIAPISTK_CnfSimLangSel"
#define MMISTK_APP_293_112_2_18_2_55_51_2 "[MMI_STK] enter MMISTK_AppHandlePsMsg: msg_id = 0x%04X, APP_MN_STK_SIGNAL_START =0x%04X"
#define MMISTK_APP_425_112_2_18_2_55_51_3 "[MMI_STK] leave MMISTK_AppHandlePsMsg: result = %d"
#define MMISTK_WIN_1034_112_2_18_2_55_54_4 "[MMI_STK] enter SIMATStr2Uint8Str:PNULL == in_simat_str ||PNULL == out_str_ptr||0 == in_buf_len"
#define MMISTK_WIN_1038_112_2_18_2_55_54_5 "[MMI_STK] enter SIMATStr2Uint8Str, simat_str->dcs = 0x%02x, len=%d"
#define MMISTK_WIN_1089_112_2_18_2_55_54_6 "[MMI_STK] leave SIMATStr2Uint8Str"
#define MMISTK_WIN_1106_112_2_18_2_55_54_7 "SIMATStr2MMIStr:PNULL == simat_str ||PNULL == mmi_str||0 == mmi_buf_len"
#define MMISTK_WIN_1110_112_2_18_2_55_54_8 "[MMI_STK] enter SIMATStr2MMIStr, simat_str->dcs = 0x%02x, len=%d"
#define MMISTK_WIN_1138_112_2_18_2_55_54_9 "[MMI_STK] enter SIMATStr2MMIStr, 8 bit string length is %d"
#define MMISTK_WIN_1174_112_2_18_2_55_54_10 "[MMI_STK] leave SIMATStr2MMIStr"
#define MMISTK_WIN_1188_112_2_18_2_55_54_11 "MMIStr2SIMATStr:PNULL == simat_str||PNULL == mmi_str "
#define MMISTK_WIN_1335_112_2_18_2_55_54_12 "AlphaId2MMIStr:PNULL == alpha_id_str||PNULL == mmi_str"
#define MMISTK_WIN_1339_112_2_18_2_55_54_13 "[MMI_STK] enter AlphaId2MMIStr, alpha_id_str->id_ptr[0] = 0x%02x"
#define MMISTK_WIN_1361_112_2_18_2_55_54_14 "SimItem2MMIStr:PNULL == item_str||PNULL == mmi_str"
#define MMISTK_WIN_1365_112_2_18_2_55_54_15 "[MMI_STK] enter SimItem2MMIStr, item_str->item_content_entry_buf[0] = 0x%02x"
#define MMISTK_WIN_1414_112_2_18_2_55_54_16 "MMISTK_OnSetupMenuCmd:PNULL == arg"
#define MMISTK_WIN_1421_112_2_18_2_55_54_17 "[MMI_STK] enter MMISTK_OnSetupMenuCmd, item_data_object_num is %d, dual_sys=%d"
#define MMISTK_WIN_1425_112_2_18_2_55_54_18 "MMISTK_OnSetupMenuCmd:arg_in_ptr->dual_sys >= MMI_DUAL_SYS_MAX"
#define MMISTK_WIN_1450_112_2_18_2_55_54_19 "[MMI_STK] MMISTK_OnSetupMenuCmd,dual_sys=%d,menu_item_amount = %d"
#define MMISTK_WIN_1490_112_2_18_2_55_55_20 "MMISTK_OnSelectItemCmd:PNULL == arg"
#define MMISTK_WIN_1497_112_2_18_2_55_55_21 "[MMI_STK] enter MMISTK_OnSelectItemCmd"
#define MMISTK_WIN_1552_112_2_18_2_55_55_22 "[MMI_STK] leave MMISTK_OnSelectItemCmd because ExistActiveOrSetupCall"
#define MMISTK_WIN_1571_112_2_18_2_55_55_23 "[MMI_STK] Leave MMISTK_OnSelectItemCmd No Memory"
#define MMISTK_WIN_1587_112_2_18_2_55_55_24 "[MMI_STK] leave MMISTK_OnSelectItemCmd because CanDisplayStk() return FALSE"
#define MMISTK_WIN_1601_112_2_18_2_55_55_25 "[MMI_STK] param_in->is_alpha_id = %d, s_mmistk_menutitle_need_refresh = %d"
#define MMISTK_WIN_1609_112_2_18_2_55_55_26 "[MMI_STK] param_in->alpha_id.id_ptr = 0x%02x, param_in->alpha_id.len = %d"
#define MMISTK_WIN_1620_112_2_18_2_55_55_27 "[MMI_STK] menu_name.wstr_len=%d"
#define MMISTK_WIN_1646_112_2_18_2_55_55_28 "[MMI_STK] menu_name.wstr_len=%d"
#define MMISTK_WIN_1759_112_2_18_2_55_55_29 "[MMI_STK] load %d current level menu items,object_num=%d"
#define MMISTK_WIN_1770_112_2_18_2_55_55_30 "[MMI_STK] Leave MMISTK_OnSelectItemCmd No Memory"
#define MMISTK_WIN_1852_112_2_18_2_55_55_31 "[MMI_STK] param_in->is_item_id = %d, param_in->item_id = %d"
#define MMISTK_WIN_1866_112_2_18_2_55_55_32 "[MMI_STK] GUILIST_SetTopItemIndex"
#define MMISTK_WIN_1905_112_2_18_2_55_55_33 "[MMI_STK] leave MMISTK_OnSelectItemCmd"
#define MMISTK_WIN_1952_112_2_18_2_55_55_34 "[MMI_STK] after timer duration %d, Timer(%d) arrived"
#define MMISTK_WIN_1981_112_2_18_2_55_55_35 "SelectItemOptionWinHandleMsg:PNULL == s_mmistk_cur_menu_info_ptr[dual_sys]"
#define MMISTK_WIN_2401_112_2_18_2_55_56_36 "MMISTK_OnDisplayTextCmd:PNULL == arg"
#define MMISTK_WIN_2408_112_2_18_2_55_56_37 "[MMI_STK] enter MMISTK_OnDisplayTextCmd"
#define MMISTK_WIN_2442_112_2_18_2_55_56_38 "[MMI_STK] get if the text is high priority %d,if is display text cnf = %d, if is diaplaying text %d, is_ir= %d"
#define MMISTK_WIN_2460_112_2_18_2_55_56_39 "[MMI_STK] leave MMISTK_OnDisplayTextCmd because low priority"
#define MMISTK_WIN_2473_112_2_18_2_55_56_40 "[MMI_STK] MMISTK_OnDisplayTextCmd, if is icon exist = %d, the icon id is = %d, the text length is %d"
#define MMISTK_WIN_2485_112_2_18_2_55_56_41 "[MMI_STK] leave MMISTK_OnDisplayTextCmd because the data not understood by ME"
#define MMISTK_WIN_2498_112_2_18_2_55_57_42 "[MMI_STK] DisplayText  timer_duration = %d"
#define MMISTK_WIN_2508_112_2_18_2_55_57_43 "[MMI_STK] param_in->text_str.len = %d"
#define MMISTK_WIN_2510_112_2_18_2_55_57_44 "[MMI_STK] s_mmistk_displaydata.length = %d"
#define MMISTK_WIN_2548_112_2_18_2_55_57_45 "[MMI_STK] leave MMISTK_OnDisplayTextCmd"
#define MMISTK_WIN_2786_112_2_18_2_55_57_46 "MMISTK_OnGetInkeyCmd:PNULL == arg"
#define MMISTK_WIN_2792_112_2_18_2_55_57_47 "[MMI_STK] enter MMISTK_OnGetInkeyCmd"
#define MMISTK_WIN_2827_112_2_18_2_55_57_48 "[MMI_STK] leave MMISTK_OnGetInkeyCmd because CanDisplayStk() return FALSE"
#define MMISTK_WIN_2842_112_2_18_2_55_57_49 "[MMI_STK] s_mmistk_inkey_type = %d, help_info=%d, is_icon_id=%d"
#define MMISTK_WIN_2849_112_2_18_2_55_57_50 "[MMI_STK] s_mmistk_displaydata.wstr_len = %d"
#define MMISTK_WIN_2872_112_2_18_2_55_57_51 "[MMI_STK] leave MMISTK_OnGetInkeyCmd, start timer(%d), timer duration = %d"
#define MMISTK_WIN_3111_112_2_18_2_55_58_52 "MMISTK_OnGetInputCmd:PNULL == arg"
#define MMISTK_WIN_3118_112_2_18_2_55_58_53 "[MMI_STK] enter MMISTK_OnGetInputCmd"
#define MMISTK_WIN_3153_112_2_18_2_55_58_54 "[MMI_STK] leave MMISTK_OnGetInputCmd because CanDisplayStk() return FALSE"
#define MMISTK_WIN_3170_112_2_18_2_55_58_55 "[MMI_STK] s_mmistk_input_type = %d, s_mmistk_input_is_echo = %d, s_mmistk_input_is_pack_req = %d, s_mmistk_is_input_help_info=%d,s_mmistk_input_is_icon_id=%d"
#define MMISTK_WIN_3171_112_2_18_2_55_58_56 "[MMI_STK] s_mmistk_input_length.min_len = %d, s_mmistk_input_length.max_len = %d"
#define MMISTK_WIN_3177_112_2_18_2_55_58_57 "[MMI_STK] param_in->text_str.len = %d"
#define MMISTK_WIN_3186_112_2_18_2_55_58_58 "[MMI_STK] s_mmistk_displaydata.length = %d, is_default_text = %d,default_text.len=%d"
#define MMISTK_WIN_3206_112_2_18_2_55_58_59 "[MMI_STK] leave MMISTK_OnGetInputCmd, start timer(%d), timer duration = %d"
#define MMISTK_WIN_3221_112_2_18_2_55_58_60 "MMISTK: MMISTK_OnMoreTimeCmd"
#define MMISTK_WIN_3229_112_2_18_2_55_58_61 "MMISTK: MMISTK_OnMoreTimeCmd"
#define MMISTK_WIN_3317_112_2_18_2_55_58_62 "[MMI_STK] s_mmistk_displaydata.length = %d"
#define MMISTK_WIN_3360_112_2_18_2_55_58_63 "MMISTK_OnSendSmsCmd:PNULL == arg"
#define MMISTK_WIN_3373_112_2_18_2_55_58_64 "[MMI_STK] param_in->is_alpha_id = %d, param_in->alpha_id.len = %d, param_in->is_packet_IND = %d, param_in->is_address = %d"
#define MMISTK_WIN_3378_112_2_18_2_55_58_65 "[MMI_STK] param_in->address.number_type = 0x%02x, param_in->address.number_plan = 0x%02x, param_in->address.number_len = %d"
#define MMISTK_WIN_3398_112_2_18_2_55_58_66 "[MMI_STK] MMISTK_OnSendSmsCmd s_mmistk_sendsms_cnf=%d"
#define MMISTK_WIN_3468_112_2_18_2_55_58_67 "[MMI_STK] MNSMS_SendSTKSmsPDU return MN_RETURN_SUCCESS"
#define MMISTK_WIN_3478_112_2_18_2_55_58_68 "[MMI_STK] MNSMS_SendSTKSmsPDU return MN_RETURN_FAILURE, the code is %d"
#define MMISTK_WIN_3480_112_2_18_2_55_58_69 "[MMI_STK] leave MMISTK_OnSendSmsCmd"
#define MMISTK_WIN_3610_112_2_18_2_55_59_70 "MMISTK_OnSetupCallCmd:PNULL == arg"
#define MMISTK_WIN_3635_112_2_18_2_55_59_71 "[MMI_STK] enter MMISTK_OnSetupCallCmd"
#define MMISTK_WIN_3644_112_2_18_2_55_59_72 "[MMI_STK] leave MMISTK_OnSetupCallCmd because no call address"
#define MMISTK_WIN_3649_112_2_18_2_55_59_73 "[MMI_STK] get make the call type is %d"
#define MMISTK_WIN_3664_112_2_18_2_55_59_74 "[MMI_STK] leave MMISTK_OnSetupCallCmd because there is in call already"
#define MMISTK_WIN_3695_112_2_18_2_55_59_75 "[MMI_STK] param_in->is_cs_alpha_id = %d, param_in->cs_alpha_id.len = %d"
#define MMISTK_WIN_3696_112_2_18_2_55_59_76 "[MMI_STK] param_in->is_uc_alpha_id = %d, param_in->uc_alpha_id.len = %d"
#define MMISTK_WIN_3723_112_2_18_2_55_59_77 "[MMI_STK] s_is_setup_call_duration=%d,duration=%d"
#define MMISTK_WIN_3739_112_2_18_2_55_59_78 "[MMI_STK] MMIAPICOM_BcdToStr tel_ptr=%s, tel_len=%d"
#define MMISTK_WIN_3752_112_2_18_2_55_59_79 "[MMI_STK] param_in->is_cps = %d,param_in->cps.len = %d"
#define MMISTK_WIN_3799_112_2_18_2_55_59_80 "[MMI_STK] leave MMISTK_OnSetupCallCmd"
#define MMISTK_WIN_3873_112_2_18_2_55_59_81 "[MMI_STK] s_mmistk_displaydata.wstr_len = %d"
#define MMISTK_WIN_3909_112_2_18_2_55_59_82 "GetPlayToneId: the audio tone is %d"
#define MMISTK_WIN_3980_112_2_18_2_55_59_83 "MMISTK_OnPlayToneCmd:PNULL == arg"
#define MMISTK_WIN_4009_112_2_18_2_56_0_84 "[MMI_STK] param_in->is_alpha_id = %d, param_in->alpha_id.len = %d"
#define MMISTK_WIN_4016_112_2_18_2_56_0_85 "[MMI_STK] param_in->alpha_id.len = %d"
#define MMISTK_WIN_4030_112_2_18_2_56_0_86 "[MMI_STK] enter MMISTK_OnPlayToneCmd, param_in->is_tone_exist = %d, the tone type is %d"
#define MMISTK_WIN_4031_112_2_18_2_56_0_87 "[MMI_STK] param_in->is_duration_exist = %d, param_in->duration.duration = %d"
#define MMISTK_WIN_4053_112_2_18_2_56_0_88 "[MMI_STK] leave MMISTK_OnPlayToneCmd, start timer(%d), timer duration = %d"
#define MMISTK_WIN_4088_112_2_18_2_56_0_89 "MMISTK_OnProvideLICmd:PNULL == arg"
#define MMISTK_WIN_4096_112_2_18_2_56_0_90 "[MMI_STK] enter MMISTK_OnProvideLICmd, info_type = %d"
#define MMISTK_WIN_4234_112_2_18_2_56_0_91 "[MMI_STK] leave MMISTK_OnProvideLICmd"
#define MMISTK_WIN_4258_112_2_18_2_56_0_92 "MMISTK_OnSendSSCmd:PNULL == arg"
#define MMISTK_WIN_4266_112_2_18_2_56_0_93 "[MMI_STK] enter MMISTK_OnSendSSCmd"
#define MMISTK_WIN_4292_112_2_18_2_56_0_94 "MMISTK_OnSendSSCmd: currently busy on SS transaction, return"
#define MMISTK_WIN_4302_112_2_18_2_56_0_95 "MMISTK_OnSendSSCmd: currently busy on USSD transaction, return"
#define MMISTK_WIN_4346_112_2_18_2_56_0_96 "[MMI_STK] bcd_str length = %d"
#define MMISTK_WIN_4383_112_2_18_2_56_0_97 "[MMI_STK] phone_oper = %d, send ss result is %d"
#define MMISTK_WIN_4400_112_2_18_2_56_0_98 "[MMI_STK] send ss failure, the result is= %d"
#define MMISTK_WIN_4403_112_2_18_2_56_0_99 "[MMI_STK] leave MMISTK_OnSendSSCmd"
#define MMISTK_WIN_4422_112_2_18_2_56_0_100 "MMISTK_OnSendUSSDCmd:PNULL == arg"
#define MMISTK_WIN_4430_112_2_18_2_56_0_101 "[MMI_STK] enter MMISTK_OnSendUSSDCmd"
#define MMISTK_WIN_4454_112_2_18_2_56_0_102 "MMISTK_OnSendUSSDCmd: currently busy on SS transaction, return"
#define MMISTK_WIN_4464_112_2_18_2_56_0_103 "MMISTK_OnSendUSSDCmd: currently busy on USSD transaction, return"
#define MMISTK_WIN_4504_112_2_18_2_56_1_104 "[MMI_STK] param_in->ussd.data_code_scheme %d"
#define MMISTK_WIN_4505_112_2_18_2_56_1_105 "[MMI_STK] param_in->ussd.str_len %d"
#define MMISTK_WIN_4524_112_2_18_2_56_1_106 "[MMI_STK] send originate ussd success"
#define MMISTK_WIN_4528_112_2_18_2_56_1_107 "[MMI_STK] send ussd failure"
#define MMISTK_WIN_4533_112_2_18_2_56_1_108 "[MMI_STK] leave MMISTK_OnSendUSSDCmd"
#define MMISTK_WIN_4579_112_2_18_2_56_1_109 "OnRefreshCnfSimFileChange: dual_sys=%d,is_file_change=%d,"
#define MMISTK_WIN_4617_112_2_18_2_56_1_110 "OnRefreshCnfSimFileChange:sim_file = %d"
#define MMISTK_WIN_4625_112_2_18_2_56_1_111 "OnRefreshCnfSimFileChange: No file change"
#define MMISTK_WIN_4649_112_2_18_2_56_1_112 "MMISTK_OnRefreshCnf: dual_sys=%d, is_ok=%d"
#define MMISTK_WIN_4715_112_2_18_2_56_1_113 "MMISTK_OnRefreshCmd:PNULL == arg"
#define MMISTK_WIN_4722_112_2_18_2_56_1_114 "[MMI_STK] enter MMISTK_OnRefreshCmd, refresh_type = %d,dual_sys=%d"
#define MMISTK_WIN_4791_112_2_18_2_56_1_115 "refresh type = %d"
#define MMISTK_WIN_4795_112_2_18_2_56_1_116 "[MMI_STK] leave MMISTK_OnRefreshCmd"
#define MMISTK_WIN_4819_112_2_18_2_56_1_117 "MMISTK_OnSetupEventlistCmd:PNULL == arg"
#define MMISTK_WIN_4824_112_2_18_2_56_1_118 "[MMI_STK] enter MMISTK_OnSetupEventlistCmd"
#define MMISTK_WIN_4834_112_2_18_2_56_1_119 "[MMI_STK] leave MMISTK_OnSetupEventlistCmd"
#define MMISTK_WIN_4855_112_2_18_2_56_1_120 "MMISTK_OnSetupIdletextCmd:PNULL == arg"
#define MMISTK_WIN_4860_112_2_18_2_56_1_121 "[MMI_STK] enter MMISTK_OnSetupIdletextCmd"
#define MMISTK_WIN_4929_112_2_18_2_56_1_122 "[MMI_STK] leave MMISTK_OnSetupIdletextCmd"
#define MMISTK_WIN_4948_112_2_18_2_56_1_123 "MMISTK_OnSendDtmfCmd:PNULL == arg"
#define MMISTK_WIN_4975_112_2_18_2_56_1_124 "[MMI_STK] enter MMISTK_OnSendDtmfCmd"
#define MMISTK_WIN_4977_112_2_18_2_56_1_125 "[MMI_STK]OnSendDtmfCmd: param_in->is_alpha_id = %d,param_in->alpha_id.len = %d,is_icon_id=%d"
#define MMISTK_WIN_4999_112_2_18_2_56_2_126 "[MMI_STK]OnSendDtmfCmd: param_in->dtmf.len is %d"
#define MMISTK_WIN_5008_112_2_18_2_56_2_127 "[MMI_STK]OnSendDtmfCmd: s_mmistk_dtmf_len is %d"
#define MMISTK_WIN_5024_112_2_18_2_56_2_128 "[MMI_STK]OnSendDtmfCmd: s_is_mmi_dtmf_pause=%d"
#define MMISTK_WIN_5064_112_2_18_2_56_2_129 "[MMI_STK] leave MMISTK_OnSendDtmfCmd"
#define MMISTK_WIN_5087_112_2_18_2_56_2_130 "MMISTK_OnLangNotifyCmd:PNULL == arg"
#define MMISTK_WIN_5094_112_2_18_2_56_2_131 "[MMI_STK] enter MMISTK_OnLangNotifyCmd specific_lang_noti=%d"
#define MMISTK_WIN_5117_112_2_18_2_56_2_132 "[MMI_STK] MMISTK_OnLangNotifyCmd lang=%s, language_type=%d"
#define MMISTK_WIN_5120_112_2_18_2_56_2_133 "[MMI_STK] leave MMISTK_OnLangNotifyCmd"
#define MMISTK_WIN_5145_112_2_18_2_56_2_134 "MMISTK_OnCcToMmiCmd arg is NULL"
#define MMISTK_WIN_5156_112_2_18_2_56_2_135 "[MMI_STK] MMISTK_OnCcToMmiCmd dual_sys=%d, is_simc_data=%d, is_ccp1=%d, is_ccp2=%d, is_cps=%d, is_alpha_id=%d, len=%d, result=%d, data_type=%d, num_len=%d, data_type=%d"
#define MMISTK_WIN_5210_112_2_18_2_56_2_136 "[MMI_STK] Leave MMISTK_OnCcToMmiCmd is_cc_clt_success=%d"
#define MMISTK_WIN_5230_112_2_18_2_56_2_137 "MMISTK_OnSSControlRspCmd:PNULL == arg"
#define MMISTK_WIN_5237_112_2_18_2_56_2_138 "[MMI_STK] leave MMISTK_OnSSControlRspCmd"
#define MMISTK_WIN_5257_112_2_18_2_56_2_139 "MMISTK_OnUSSDControlRspCmd:PNULL == arg"
#define MMISTK_WIN_5264_112_2_18_2_56_2_140 "[MMI_STK] leave MMISTK_OnUSSDControlRspCmd"
#define MMISTK_WIN_5292_112_2_18_2_56_2_141 "MMISTK_OnSmsCtlToMmiCmd arg is NULL"
#define MMISTK_WIN_5301_112_2_18_2_56_2_142 "[MMI_STK] MMISTK_OnSmsCtlToMmiCmd dual_sys=%d, is_alpha_id=%d, len=%d, is_dest_me=%d, is_dest_sc=%d, result=%d"
#define MMISTK_WIN_5381_112_2_18_2_56_2_143 "[MMI_STK] Leave MMISTK_OnSmsCtlToMmiCmd is_sms_clt_success=%d"
#define MMISTK_WIN_5400_112_2_18_2_56_2_144 "MMISTK_OnSMSP2PRspCmd:PNULL == arg"
#define MMISTK_WIN_5407_112_2_18_2_56_2_145 "[MMI_STK] leave MMISTK_OnSMSP2PRspCmd"
#define MMISTK_WIN_5426_112_2_18_2_56_2_146 "MMISTK_OnSMSCBRspCmd:PNULL == arg"
#define MMISTK_WIN_5432_112_2_18_2_56_2_147 "[MMI_STK] leave MMISTK_OnSMSCBRspCmd"
#define MMISTK_WIN_5470_112_2_18_2_56_2_148 "[MMI_STK] enter CheckUnCloseWin, s_mmistk_close_window = %d, curr_win = %d"
#define MMISTK_WIN_5531_112_2_18_2_56_3_149 "[MMI_STK] leave CheckUnCloseWin"
#define MMISTK_WIN_5543_112_2_18_2_56_3_150 "[MMI_STK] enter CloseAllStkWin"
#define MMISTK_WIN_5555_112_2_18_2_56_3_151 "[MMI_STK] leave CloseAllStkWin"
#define MMISTK_WIN_5583_112_2_18_2_56_3_152 "mmistk_win.c: is in proactive stk session"
#define MMISTK_WIN_5588_112_2_18_2_56_3_153 "mmistk_win.c: is not in proactive stk session"
#define MMISTK_WIN_5604_112_2_18_2_56_3_154 "[MMI_STK] enter MMISTK_OnSessionTerminateCmd"
#define MMISTK_WIN_5610_112_2_18_2_56_3_155 "[MMI_STK] leave MMISTK_OnSessionTerminateCmd dual_sys=%d"
#define MMISTK_WIN_5651_112_2_18_2_56_3_156 "[MMI_STK] leave MMISTK_OnSessionTerminateCmd"
#define MMISTK_WIN_5668_112_2_18_2_56_3_157 "GetMenuActionIdName:PNULL == name_ptr"
#define MMISTK_WIN_5708_112_2_18_2_56_3_158 "[MMI_STK] SetMenuActionIdData Param Error!"
#define MMISTK_WIN_5763_112_2_18_2_56_3_159 "[MMI_STK] enter MMISTK_MainMenuWinHandleMsg, msg_id = 0x%04X, s_mmistk_indmenusel_cnf = %d"
#define MMISTK_WIN_5841_112_2_18_2_56_3_160 "MMISTK_MainMenuWinHandleMsg need_item_cont_ptr is NULL"
#define MMISTK_WIN_5954_112_2_18_2_56_3_161 "[MMI_STK] leave MMISTK_MainMenuWinHandleMsg(0x%02X), s_mmistk_indmenusel_cnf = %d, result(%d)"
#define MMISTK_WIN_5978_112_2_18_2_56_3_162 "[MMI_STK] enter MenuWinHandleMsg, msg_id = 0x%04X, s_mmistk_selectitem_cnf = %d"
#define MMISTK_WIN_6057_112_2_18_2_56_4_163 "MenuWinHandleMsg need_item_cont_ptr is NULL"
#define MMISTK_WIN_6097_112_2_18_2_56_4_164 "[MMI_STK] after timer duration %d, Timer(%d) arrived"
#define MMISTK_WIN_6161_112_2_18_2_56_4_165 "MenuWinHandleMsg ::MSG_CTL_OK s_mmistk_cur_menu_info_ptr"
#define MMISTK_WIN_6298_112_2_18_2_56_4_166 "[MMI_STK] leave MenuWinHandleMsg(0x%02X), s_mmistk_selectitem_cnf = %d, result(%d)"
#define MMISTK_WIN_6319_112_2_18_2_56_4_167 "[MMI_STK] enter DisplayTextWinHandleMsg, msg_id = 0x%04X, s_mmistk_displaytext_cnf = %d, s_mmistk_close_window = %d"
#define MMISTK_WIN_6435_112_2_18_2_56_4_168 "[MMI_STK] after timer duration %d, Timer(%d) arrived"
#define MMISTK_WIN_6536_112_2_18_2_56_5_169 "[MMI_STK] leave DisplayTextWinHandleMsg(0x%02X), s_mmistk_displaytext_cnf = %d, result = %d"
#define MMISTK_WIN_6560_112_2_18_2_56_5_170 "[MMI_STK] enter GetInkeyWinHandleMsg, msg_id = 0x%04X, s_mmistk_getinkey_cnf = %d, s_mmistk_close_window = %d"
#define MMISTK_WIN_6809_112_2_18_2_56_5_171 "[MMI_STK] after timer duration %d, Timer(%d) arrived"
#define MMISTK_WIN_6872_112_2_18_2_56_5_172 "[MMI_STK] leave GetInkeyWinHandleMsg(0x%02X), s_mmistk_getinkey_cnf = %d, result(%d)"
#define MMISTK_WIN_6898_112_2_18_2_56_5_173 "[MMI_STK] enter GetInputWinHandleMsg, msg_id = 0x%04X, s_mmistk_getinput_cnf = %d, s_mmistk_close_window = %d"
#define MMISTK_WIN_6946_112_2_18_2_56_5_174 "[MMI_STK] GetInputWinHandleMsg, SIMAT_TR_TERMINATED_BY_USER, the result is = %d"
#define MMISTK_WIN_6961_112_2_18_2_56_5_175 "[MMI_STK] GetInputWinHandleMsg, SIMAT_TR_BACK_MOVE, the result is = %d"
#define MMISTK_WIN_7029_112_2_18_2_56_6_176 "[MMI_STK] next_act = %d, str_info.wstr_len = %d"
#define MMISTK_WIN_7047_112_2_18_2_56_6_177 "[MMI_STK] next_act = %d, str_info.wstr_len = %d"
#define MMISTK_WIN_7066_112_2_18_2_56_6_178 "[MMI_STK] next_act = %d, str_info.wstr_len = %d"
#define MMISTK_WIN_7093_112_2_18_2_56_6_179 "[MMI_STK] next_act = %d, txt_info.str_len = %d"
#define MMISTK_WIN_7124_112_2_18_2_56_6_180 "[MMI_STK] after MMIStr2SIMATStr, s_mmistk_input_is_pack_req = %d"
#define MMISTK_WIN_7167_112_2_18_2_56_6_181 "[MMI_STK] after timer duration %d, Timer(%d) arrived"
#define MMISTK_WIN_7232_112_2_18_2_56_6_182 "[MMI_STK] leave GetInputWinHandleMsg(0x%02X), s_mmistk_getinput_cnf = %d, result(%d)"
#define MMISTK_WIN_7254_112_2_18_2_56_6_183 "[MMI_STK] enter SendSmsWinHandleMsg, msg_id = 0x%04X, s_mmistk_sendsms_cnf = %d, s_mmistk_close_window = %d"
#define MMISTK_WIN_7302_112_2_18_2_56_6_184 "[MMI_STK] leave SendSmsWinHandleMsg(0x%02X), s_mmistk_sendsms_cnf = %d, result(%d)"
#define MMISTK_WIN_7323_112_2_18_2_56_6_185 "[MMI_STK] enter PlayToneWinHandleMsg, msg_id = 0x%04X, s_mmistk_playtone_cnf = %d, s_mmistk_close_window = %d"
#define MMISTK_WIN_7417_112_2_18_2_56_6_186 "[MMI_STK] leave PlayToneWinHandleMsg(0x%02X), s_mmistk_playtone_cnf = %d, result(%d)"
#define MMISTK_WIN_7450_112_2_18_2_56_6_187 "MMISTK_SendReqToSs:PNULL == ss_oper"
#define MMISTK_WIN_7547_112_2_18_2_56_7_188 "[MMI_STK] s_mmistk_displaydata.length = %d"
#define MMISTK_WIN_7581_112_2_18_2_56_7_189 "[MMI_STK] StartSetupCallOperation dual_sys=%d, s_mmistk_setup_call_type=%d"
#define MMISTK_WIN_7600_112_2_18_2_56_7_190 "[MMI_STK] SetupCallWinHandleMsg because there is in call already"
#define MMISTK_WIN_7681_112_2_18_2_56_7_191 "[MMI_STK] enter SetupCallWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d"
#define MMISTK_WIN_7742_112_2_18_2_56_7_192 "[MMI_STK] leave SetupCallWinHandleMsg"
#define MMISTK_WIN_7777_112_2_18_2_56_7_193 "[MMI_STK] MMISTK_SetupCall, s_is_mmistk_setup_call=%d"
#define MMISTK_WIN_7783_112_2_18_2_56_7_194 "[MMI_STK] MMISTK_SetupCall, is_oper_cnf=%d,is_call_active=%d,s_mmistk_call_oper_type=%d"
#define MMISTK_WIN_7988_112_2_18_2_56_7_195 "[MMI_STK] enter SendSSWinHandleMsg, msg_id = 0x%04X"
#define MMISTK_WIN_8026_112_2_18_2_56_8_196 "[MMI_STK] leave SendSSWinHandleMsg(0x%02X), result(%d)"
#define MMISTK_WIN_8041_112_2_18_2_56_8_197 "[MMI_STK] MMISTK_OnSendSSCnf, s_mmistk_sendss_waiting_respond= %d"
#define MMISTK_WIN_8044_112_2_18_2_56_8_198 "MMISTK_OnSendSSCnf:PNULL == arg"
#define MMISTK_WIN_8056_112_2_18_2_56_8_199 "send ss success: MN_SS_OPERATION_SUCC result_octs_len=%d"
#define MMISTK_WIN_8076_112_2_18_2_56_8_200 "send ss failure: MN_SS_OPERATION_REJECT"
#define MMISTK_WIN_8081_112_2_18_2_56_8_201 "send ss failure: MN_SS_OPERATION_ERROR,The error code is %d"
#define MMISTK_WIN_8087_112_2_18_2_56_8_202 "MMISTK_OnSendSSCnf:PNULL == ((MnSsRegisterCnfS*)(arg))->result_type"
#define MMISTK_WIN_8108_112_2_18_2_56_8_203 "[MMI_STK] MMISTK_OnSendUSSDCnf, s_mmistk_sendussd_waiting_respond= %d"
#define MMISTK_WIN_8136_112_2_18_2_56_8_204 "[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_SUCC"
#define MMISTK_WIN_8143_112_2_18_2_56_8_205 "[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_ERROR, ussd info is %d"
#define MMISTK_WIN_8149_112_2_18_2_56_8_206 "[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_REJECT"
#define MMISTK_WIN_8153_112_2_18_2_56_8_207 "result type = %d"
#define MMISTK_WIN_8175_112_2_18_2_56_8_208 "[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_SUCC"
#define MMISTK_WIN_8181_112_2_18_2_56_8_209 "[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_ERROR, ussd info is %d"
#define MMISTK_WIN_8186_112_2_18_2_56_8_210 "[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_REJECT"
#define MMISTK_WIN_8190_112_2_18_2_56_8_211 "result type = %d"
#define MMISTK_WIN_8216_112_2_18_2_56_8_212 "result MMISTK_OnSendRegisterPaswdCnf ::PNULL == arg"
#define MMISTK_WIN_8220_112_2_18_2_56_8_213 "[MMI_STK] MMISTK_OnSendRegisterPaswdCnf, s_mmistk_sendss_waiting_respond= %d, dual_sys=%d"
#define MMISTK_WIN_8230_112_2_18_2_56_8_214 "[MMI_STK] send ss success"
#define MMISTK_WIN_8236_112_2_18_2_56_8_215 "[MMI_STK] send ss failure"
#define MMISTK_WIN_8279_112_2_18_2_56_8_216 "MMISTK_OnSetupMenuCmd: toplevel_menu_info.menu_item_amount %d"
#define MMISTK_WIN_8322_112_2_18_2_56_8_217 "MMISTK_OnSetupMenuCmd: the alpha_id.len is %d"
#define MMISTK_WIN_8330_112_2_18_2_56_8_218 "[MMI_STK] menu_title.wstr_len=%d"
#define MMISTK_WIN_8376_112_2_18_2_56_8_219 "[MMI_STK] leave OnSetupMenuCmd, load %d top level menus"
#define MMISTK_WIN_8457_112_2_18_2_56_8_220 "[MMI_STK] enter MMISTK_SendSMSPDUCnf, s_mmistk_sendsms_cnf = %d, send_result = %d, s_mmistk_close_window = %d"
#define MMISTK_WIN_8471_112_2_18_2_56_8_221 "[MMI_STK] MMISTK_SendSMSPDUCnf, send success"
#define MMISTK_WIN_8485_112_2_18_2_56_8_222 "[MMI_STK] MMISTK_SendSMSPDUCnf, send fail, fail code is %d, last_fail_cause=%d"
#define MMISTK_WIN_8489_112_2_18_2_56_8_223 "[MMI_STK] MMISTK_SendSMSPDUCnf, send fail, fail code is %d"
#define MMISTK_WIN_8499_112_2_18_2_56_9_224 "[MMI_STK] leave MMISTK_SendSMSPDUCnf, s_mmistk_sendsms_cnf = %d"
#define MMISTK_WIN_8513_112_2_18_2_56_9_225 "[MMI_STK] leave MMISTK_EnterToplevelMenu Ptr NULL"
#define MMISTK_WIN_8517_112_2_18_2_56_9_226 "[MMI_STK] enter MMISTK_EnterToplevelMenu dual_sys=%d menu_item_amount = %d"
#define MMISTK_WIN_8521_112_2_18_2_56_9_227 "[MMI_STK] leave MMISTK_EnterToplevelMenu because top level menu items <=0 or STk Main Menu Window already open"
#define MMISTK_WIN_8528_112_2_18_2_56_9_228 "[MMI_STK] enter MMISTK_EnterToplevelMenu"
#define MMISTK_WIN_8539_112_2_18_2_56_9_229 "[MMI_STK] leave MMIAPISTK_EnterToplevelMenu"
#define MMISTK_WIN_8872_112_2_18_2_56_10_230 "[MMISTK]: HandleSIMPlugNotifySTKFunc dual_sys=%d, notify_event=%d"
#define MMISTK_WIN_9146_112_2_18_2_56_10_231 "[MMI_STK] StartLaunchBrowserOperation dual_sys=%d, url_len=%d,launch_browser_type"
#define MMISTK_WIN_9163_112_2_18_2_56_10_232 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED with url browser is running!"
#define MMISTK_WIN_9173_112_2_18_2_56_11_233 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url success!"
#define MMISTK_WIN_9179_112_2_18_2_56_11_234 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url error!"
#define MMISTK_WIN_9191_112_2_18_2_56_11_235 "[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST with url browser is not running!"
#define MMISTK_WIN_9201_112_2_18_2_56_11_236 "[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser with url success!"
#define MMISTK_WIN_9207_112_2_18_2_56_11_237 "[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser with url error!"
#define MMISTK_WIN_9215_112_2_18_2_56_11_238 "[MMI_STK] launch_browser_type is default with url!"
#define MMISTK_WIN_9229_112_2_18_2_56_11_239 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED default browser is running!"
#define MMISTK_WIN_9239_112_2_18_2_56_11_240 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser default success!"
#define MMISTK_WIN_9245_112_2_18_2_56_11_241 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser default error!"
#define MMISTK_WIN_9257_112_2_18_2_56_11_242 "[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST default browser is not running!"
#define MMISTK_WIN_9267_112_2_18_2_56_11_243 "[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser default success!"
#define MMISTK_WIN_9273_112_2_18_2_56_11_244 "[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser default error!"
#define MMISTK_WIN_9281_112_2_18_2_56_11_245 "[MMI_STK] launch_browser_type is default with default url!"
#define MMISTK_WIN_9311_112_2_18_2_56_11_246 "[MMI_STK] enter LaunchBrowserWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d"
#define MMISTK_WIN_9337_112_2_18_2_56_11_247 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser user deny"
#define MMISTK_WIN_9368_112_2_18_2_56_11_248 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser user deny"
#define MMISTK_WIN_9394_112_2_18_2_56_11_249 "[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url error!"
#define MMISTK_WIN_9403_112_2_18_2_56_11_250 "[MMI_STK] leave LaunchBrowserWinHandleMsg result %d"
#define MMISTK_WIN_9423_112_2_18_2_56_11_251 "MMISTK_OnLaunchBrowserCmd arg is NULL"
#define MMISTK_WIN_9440_112_2_18_2_56_11_252 "[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because alloc MnSimatLaunchBrowserIndS fail"
#define MMISTK_WIN_9458_112_2_18_2_56_11_253 "[MMI_STK] enter MMISTK_OnLaunchBrowserCmd"
#define MMISTK_WIN_9467_112_2_18_2_56_12_254 "[MMI_STK] dual_sys:%d, launch_browser_type:%d, is_browser_id:%d, is_icon_id:%d, is_alpha_id:%d, is_gate_way:%d, is_bear:%d"
#define MMISTK_WIN_9476_112_2_18_2_56_12_255 "[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because ExistActiveOrSetupCall"
#define MMISTK_WIN_9488_112_2_18_2_56_12_256 "[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because MMS sending"
#define MMISTK_WIN_9537_112_2_18_2_56_12_257 "[MMI_STK] leave MMISTK_OnLaunchBrowserCmd"
#define MMISTK_WIN_9581_112_2_18_2_56_12_258 "[MMI_STK]MMISTK_CreateSocket enter netid = %d"
#define MMISTK_WIN_9597_112_2_18_2_56_12_259 "[MMI_STK]MMISTK_CreateSocket, e = %d"
#define MMISTK_WIN_9608_112_2_18_2_56_12_260 "[MMI_STK]sci_sock_setsockopt, result=%d"
#define MMISTK_WIN_9614_112_2_18_2_56_12_261 "[MMI_STK]MMISTK_CreateSocket, return %ld"
#define MMISTK_WIN_9630_112_2_18_2_56_12_262 "[MMI_STK]MMISTK_CloseSocket: %d"
#define MMISTK_WIN_9665_112_2_18_2_56_12_263 "[MMI_STK]connection is established, sock=%ld"
#define MMISTK_WIN_9670_112_2_18_2_56_12_264 "[MMI_STK]MMISTK_ConnectSocket, err = %d"
#define MMISTK_WIN_9683_112_2_18_2_56_12_265 "[MMI_STK]SocketConnect, sock=%d, return=%d"
#define MMISTK_WIN_9707_112_2_18_2_56_12_266 "[MMI_STK]MMISTK_SocketSendData error= %d"
#define MMISTK_WIN_9711_112_2_18_2_56_12_267 "[MMI_STK]MMISTK_SocketSendData, s=%d, return %d"
#define MMISTK_WIN_9727_112_2_18_2_56_12_268 "[MMI_STK] MMISTK_AssignChannelID enter!"
#define MMISTK_WIN_9735_112_2_18_2_56_12_269 "[MMI_STK] MMISTK_AssignChannelID channel_id = %d!"
#define MMISTK_WIN_9759_112_2_18_2_56_12_270 "[MMI_STK] MMISTK_GetAvailableChannelID = %d"
#define MMISTK_WIN_9764_112_2_18_2_56_12_271 "[MMI_STK] MMISTK_GetAvailableChannelID = 0"
#define MMISTK_WIN_9779_112_2_18_2_56_13_272 "[MMI_STK] MMISTK_ReleaseChannelID enter!"
#define MMISTK_WIN_9788_112_2_18_2_56_13_273 "[MMI_STK] MMISTK_ReleaseChannelID channel_id = %d!"
#define MMISTK_WIN_9805_112_2_18_2_56_13_274 "[MMI_STK] MMISTK_ReleaseAllChannelID enter!"
#define MMISTK_WIN_9864_112_2_18_2_56_13_275 "[MMI_STK] SIMAT_CnfOpenChannel success!"
#define MMISTK_WIN_9876_112_2_18_2_56_13_276 "[MMI_STK] MMISTK_ConnectSocket error!"
#define MMISTK_WIN_9895_112_2_18_2_56_13_277 "[MMI_STK]MMISTK_HandlePdpActiveCnf result =%d"
#define MMISTK_WIN_9909_112_2_18_2_56_13_278 "[MMI_STK] MMISTK_CreateSocket error!"
#define MMISTK_WIN_9951_112_2_18_2_56_13_279 " MMISTK_OnLaunchBrowserCmd ::PNULL == msg_ptr"
#define MMISTK_WIN_9954_112_2_18_2_56_13_280 "[MMI_STK]:HandleSTKPDPMsg enter msg_id = %d!"
#define MMISTK_WIN_10003_112_2_18_2_56_13_281 "[MMI_STK]:HandleSTKPDPMsg exit!"
#define MMISTK_WIN_10020_112_2_18_2_56_13_282 "[MMI_STK]:MMISTK_ActivatePDPcontext enter!"
#define MMISTK_WIN_10036_112_2_18_2_56_13_283 "[MMI_STK]:MMIAPIPDP_Active = %d!"
#define MMISTK_WIN_10055_112_2_18_2_56_13_284 "[MMI_STK] MMISTK_DeactivePDPContext fail!"
#define MMISTK_WIN_10072_112_2_18_2_56_13_285 "[MMISTK]MMISTK_CnfOpenChannel is_success=%d, result=%d"
#define MMISTK_WIN_10103_112_2_18_2_56_13_286 "[MMI_STK] MMISTK_AssignChannelID error!"
#define MMISTK_WIN_10141_112_2_18_2_56_13_287 "[MMI_STK] SIMAT_CnfOpenChannel success!"
#define MMISTK_WIN_10166_112_2_18_2_56_14_288 "[MMI_STK] enter OpenChannelWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d"
#define MMISTK_WIN_10258_112_2_18_2_56_14_289 "[MMI_STK] leave OpenChannelWinHandleMsg result %d"
#define MMISTK_WIN_10438_112_2_18_2_56_14_290 "[MMI_STK] enter MMISTK_OnOpenChannelCmd"
#define MMISTK_WIN_10442_112_2_18_2_56_14_291 "[MMI_STK] MMISTK_OnOpenChannelCmd arg == PNULL!!!"
#define MMISTK_WIN_10504_112_2_18_2_56_14_292 "[MMI_STK] leave MMISTK_OnSetupCallCmd because there is in call already"
#define MMISTK_WIN_10519_112_2_18_2_56_14_293 "[MMI_STK] is_alpha_id:%d, len:%d is_other_address:%d, is_text_str_login:%d, is_text_str_pwd:%d, is_trans_level:%d, is_data_dest_addr:%d, is_net_access_name:%d,is_icon_id:%d"
#define MMISTK_WIN_10525_112_2_18_2_56_14_294 "[MMI_STK] bear_desc.bear_type = %d, len = %d, bear_desc_qos = %s"
#define MMISTK_WIN_10536_112_2_18_2_56_14_295 "[MMI_STK] param_in->buf_size = %d"
#define MMISTK_WIN_10544_112_2_18_2_56_14_296 "[MMI_STK] param_in->bear_desc.bear_type = %d"
#define MMISTK_WIN_10549_112_2_18_2_56_14_297 "[MMI_STK] leave MMISTK_OnOpenChannelCmd because beyond bear type!"
#define MMISTK_WIN_10553_112_2_18_2_56_14_298 "[MMI_STK] param_in->data_dest_address.addr_type = %d"
#define MMISTK_WIN_10557_112_2_18_2_56_14_299 "[MMI_STK] leave MMISTK_OnOpenChannelCmd because beyond dest addr type!"
#define MMISTK_WIN_10576_112_2_18_2_56_14_300 "[MMI_STK] ata_dest_address.addr_len = %d, type = %d, dest_address = %s"
#define MMISTK_WIN_10582_112_2_18_2_56_14_301 "[MMI_STK] port = %d, trans_level_type = %d"
#define MMISTK_WIN_10630_112_2_18_2_56_15_302 "[MMI_STK] dual_sys:%d, apn_ptr:%s, login_ptr:%s, psw_ptr:%s"
#define MMISTK_WIN_10635_112_2_18_2_56_15_303 "[MMI_STK] leave MMISTK_OnOpenChannelCmd because sim not ready!"
#define MMISTK_WIN_10665_112_2_18_2_56_15_304 "[MMI_STK] leave MMITSK_OnOpenChannelCmd because pdp active fail!"
#define MMISTK_WIN_10681_112_2_18_2_56_15_305 "[MMI_STK] MMISTK_AssignChannelID MMISTK_PDP_STATUS_ACTIVE_OK error!"
#define MMISTK_WIN_10689_112_2_18_2_56_15_306 "[MMI_STK] MMISTK_PDP_STATUS_ACTIVE_OK bear_desc.bear_type = %d, len = %d, bear_desc_qos = %s"
#define MMISTK_WIN_10700_112_2_18_2_56_15_307 "[MMI_STK] MMISTK_PDP_STATUS_ACTIVE_OK param_in->buf_size = %d"
#define MMISTK_WIN_10738_112_2_18_2_56_15_308 "[MMI_STK] SIMAT_CnfOpenChannel success!"
#define MMISTK_WIN_10743_112_2_18_2_56_15_309 "[MMI_STK] leave MMISTK_OnOpenChannelCmd because MMISTK_PDP_STATUS_ACTIVE_OK apn,psw,login not match!"
#define MMISTK_WIN_10749_112_2_18_2_56_15_310 "[MMI_STK] leave MMISTK_OnOpenChannelCmd because MMISTK_GetPdpActiveStatus activing!"
#define MMISTK_WIN_10770_112_2_18_2_56_15_311 "[MMI_STK] enter MMISTK_OnCloseChannelCmd"
#define MMISTK_WIN_10774_112_2_18_2_56_15_312 "[MMI_STK] MMISTK_OnCloseChannelCmd arg == PNULL!!!"
#define MMISTK_WIN_10829_112_2_18_2_56_15_313 "[MMI_STK] enter MMISTK_OnGetChannelStatusCmd"
#define MMISTK_WIN_10833_112_2_18_2_56_15_314 "[MMI_STK] MMISTK_OnGetChannelStatusCmd arg == PNULL!!!"
#define MMISTK_WIN_10854_112_2_18_2_56_15_315 "[MMI_STK] GetChannelStatus success ChannelID = %d"
#define MMISTK_WIN_10860_112_2_18_2_56_15_316 "[MMI_STK] GetChannelStatus failure"
#define MMISTK_WIN_10871_112_2_18_2_56_15_317 "[MMI_STK] leave MMISTK_OnGetChannelStatusCmd"
#define MMISTK_WIN_10899_112_2_18_2_56_15_318 "[MMI_STK] MMISTK_GetSocketDataLen = %ld, channel id= %d"
#define MMISTK_WIN_10924_112_2_18_2_56_15_319 "[MMI_STK] MMISTK_GetSocketData timer"
#define MMISTK_WIN_10939_112_2_18_2_56_15_320 "[MMI_STK] HandleSTKSocketGetDataTimer times = %d"
#define MMISTK_WIN_10969_112_2_18_2_56_16_321 "[MMI_STK] enter MMISTK_OnSendDataCmd"
#define MMISTK_WIN_10973_112_2_18_2_56_16_322 "[MMI_STK] MMISTK_OnSendDataCmd arg == PNULL!!!"
#define MMISTK_WIN_11061_112_2_18_2_56_16_323 "[MMI_STK] enter MMISTK_OnReceiveDataCmd"
#define MMISTK_WIN_11065_112_2_18_2_56_16_324 "[MMI_STK] MMISTK_OnReceiveDataCmd arg == PNULL!!!"
#define MMISTK_WIN_11142_112_2_18_2_56_16_325 "[MMI_STK] MMISTK_IsStkRefresh s_is_stk_refresh=%d"
#define MMISTK_WIN_11192_112_2_18_2_56_16_326 " MMISTK_GetStkLang ::PNULL == lang_ptr"
#define MMISTK_WIN_11225_112_2_18_2_56_16_327 "[MMI_STK] MMISTK_OnRelUSSD, s_mmistk_sendussd_waiting_respond=%d, dual_sys=%d"
#define MMISTK_WIN_11268_112_2_18_2_56_16_328 "MMISTK_HandleSTKTimer timer_id=%d,playtone_timer_id=%d"
#define MMISTK_WIN_11272_112_2_18_2_56_16_329 " MMISTK_GetStkLang ::0 == timer_id"
#define MMISTK_WIN_11306_112_2_18_2_56_16_330 "[MMI_STK] after play duration, Timer(%d) arrived"
#define MMISTK_WIN_11426_112_2_18_2_56_16_331 " HandleSTKSendDtmfStr ::dual_sys >= MMI_DUAL_SYS_MAX"
#define MMISTK_WIN_11440_112_2_18_2_56_16_332 "[MMI_STK]HandleSTKSendDtmfStr, the dtmf length is %d, the dtmf value is 0x%d"
#define MMISTK_WIN_11459_112_2_18_2_56_16_333 "[MMI_STK]HandleSTKSendDtmfStr, is not in speech now, send dtmf fail"
#define MMISTK_WIN_11544_112_2_18_2_56_17_334 "MMISTK_BrowserTermination dual_sys=%d"
#define MMISTK_WIN_11681_112_2_18_2_56_17_335 "CreateIconAnimCtrl fail! icon is NULL!"
#define MMISTK_WIN_11733_112_2_18_2_56_17_336 "DecodeSimImgToBmp img_width =%d, img_height = %d, cs = %d, len = %d"
#define MMISTK_WIN_11755_112_2_18_2_56_17_337 "DecodeSimImgToBmp MN_SIM_IMG_CODE_TYPE_E is ERROR!!!"
#define MMISTK_WIN_11776_112_2_18_2_56_17_338 "DecodeBasicImg fail! src data = NULL!"
#define MMISTK_WIN_11782_112_2_18_2_56_17_339 "DecodeBasicImg fail! icon data error!"
#define MMISTK_WIN_11789_112_2_18_2_56_17_340 "DecodeBasicImg alloc fail,no mem!"
#define MMISTK_WIN_11846_112_2_18_2_56_17_341 "DecodeColorImg fail! src data = NULL!"
#define MMISTK_WIN_11853_112_2_18_2_56_17_342 "DecodeColorImg fail! icon data error!"
#define MMISTK_WIN_11860_112_2_18_2_56_17_343 "DecodeColorImg alloc fail,no mem!"
#define MMISTK_WIN_11889_112_2_18_2_56_18_344 "DecodeColorImg data error!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_STK_TRC)
TRACE_MSG(MMISTK_APP_145_112_2_18_2_55_50_0,"[MMI_STK]:MMIAPISTK_CnfIdleScrAva")
TRACE_MSG(MMISTK_APP_165_112_2_18_2_55_51_1,"[MMI_STK]:MMIAPISTK_CnfSimLangSel")
TRACE_MSG(MMISTK_APP_293_112_2_18_2_55_51_2,"[MMI_STK] enter MMISTK_AppHandlePsMsg: msg_id = 0x%04X, APP_MN_STK_SIGNAL_START =0x%04X")
TRACE_MSG(MMISTK_APP_425_112_2_18_2_55_51_3,"[MMI_STK] leave MMISTK_AppHandlePsMsg: result = %d")
TRACE_MSG(MMISTK_WIN_1034_112_2_18_2_55_54_4,"[MMI_STK] enter SIMATStr2Uint8Str:PNULL == in_simat_str ||PNULL == out_str_ptr||0 == in_buf_len")
TRACE_MSG(MMISTK_WIN_1038_112_2_18_2_55_54_5,"[MMI_STK] enter SIMATStr2Uint8Str, simat_str->dcs = 0x%02x, len=%d")
TRACE_MSG(MMISTK_WIN_1089_112_2_18_2_55_54_6,"[MMI_STK] leave SIMATStr2Uint8Str")
TRACE_MSG(MMISTK_WIN_1106_112_2_18_2_55_54_7,"SIMATStr2MMIStr:PNULL == simat_str ||PNULL == mmi_str||0 == mmi_buf_len")
TRACE_MSG(MMISTK_WIN_1110_112_2_18_2_55_54_8,"[MMI_STK] enter SIMATStr2MMIStr, simat_str->dcs = 0x%02x, len=%d")
TRACE_MSG(MMISTK_WIN_1138_112_2_18_2_55_54_9,"[MMI_STK] enter SIMATStr2MMIStr, 8 bit string length is %d")
TRACE_MSG(MMISTK_WIN_1174_112_2_18_2_55_54_10,"[MMI_STK] leave SIMATStr2MMIStr")
TRACE_MSG(MMISTK_WIN_1188_112_2_18_2_55_54_11,"MMIStr2SIMATStr:PNULL == simat_str||PNULL == mmi_str ")
TRACE_MSG(MMISTK_WIN_1335_112_2_18_2_55_54_12,"AlphaId2MMIStr:PNULL == alpha_id_str||PNULL == mmi_str")
TRACE_MSG(MMISTK_WIN_1339_112_2_18_2_55_54_13,"[MMI_STK] enter AlphaId2MMIStr, alpha_id_str->id_ptr[0] = 0x%02x")
TRACE_MSG(MMISTK_WIN_1361_112_2_18_2_55_54_14,"SimItem2MMIStr:PNULL == item_str||PNULL == mmi_str")
TRACE_MSG(MMISTK_WIN_1365_112_2_18_2_55_54_15,"[MMI_STK] enter SimItem2MMIStr, item_str->item_content_entry_buf[0] = 0x%02x")
TRACE_MSG(MMISTK_WIN_1414_112_2_18_2_55_54_16,"MMISTK_OnSetupMenuCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_1421_112_2_18_2_55_54_17,"[MMI_STK] enter MMISTK_OnSetupMenuCmd, item_data_object_num is %d, dual_sys=%d")
TRACE_MSG(MMISTK_WIN_1425_112_2_18_2_55_54_18,"MMISTK_OnSetupMenuCmd:arg_in_ptr->dual_sys >= MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISTK_WIN_1450_112_2_18_2_55_54_19,"[MMI_STK] MMISTK_OnSetupMenuCmd,dual_sys=%d,menu_item_amount = %d")
TRACE_MSG(MMISTK_WIN_1490_112_2_18_2_55_55_20,"MMISTK_OnSelectItemCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_1497_112_2_18_2_55_55_21,"[MMI_STK] enter MMISTK_OnSelectItemCmd")
TRACE_MSG(MMISTK_WIN_1552_112_2_18_2_55_55_22,"[MMI_STK] leave MMISTK_OnSelectItemCmd because ExistActiveOrSetupCall")
TRACE_MSG(MMISTK_WIN_1571_112_2_18_2_55_55_23,"[MMI_STK] Leave MMISTK_OnSelectItemCmd No Memory")
TRACE_MSG(MMISTK_WIN_1587_112_2_18_2_55_55_24,"[MMI_STK] leave MMISTK_OnSelectItemCmd because CanDisplayStk() return FALSE")
TRACE_MSG(MMISTK_WIN_1601_112_2_18_2_55_55_25,"[MMI_STK] param_in->is_alpha_id = %d, s_mmistk_menutitle_need_refresh = %d")
TRACE_MSG(MMISTK_WIN_1609_112_2_18_2_55_55_26,"[MMI_STK] param_in->alpha_id.id_ptr = 0x%02x, param_in->alpha_id.len = %d")
TRACE_MSG(MMISTK_WIN_1620_112_2_18_2_55_55_27,"[MMI_STK] menu_name.wstr_len=%d")
TRACE_MSG(MMISTK_WIN_1646_112_2_18_2_55_55_28,"[MMI_STK] menu_name.wstr_len=%d")
TRACE_MSG(MMISTK_WIN_1759_112_2_18_2_55_55_29,"[MMI_STK] load %d current level menu items,object_num=%d")
TRACE_MSG(MMISTK_WIN_1770_112_2_18_2_55_55_30,"[MMI_STK] Leave MMISTK_OnSelectItemCmd No Memory")
TRACE_MSG(MMISTK_WIN_1852_112_2_18_2_55_55_31,"[MMI_STK] param_in->is_item_id = %d, param_in->item_id = %d")
TRACE_MSG(MMISTK_WIN_1866_112_2_18_2_55_55_32,"[MMI_STK] GUILIST_SetTopItemIndex")
TRACE_MSG(MMISTK_WIN_1905_112_2_18_2_55_55_33,"[MMI_STK] leave MMISTK_OnSelectItemCmd")
TRACE_MSG(MMISTK_WIN_1952_112_2_18_2_55_55_34,"[MMI_STK] after timer duration %d, Timer(%d) arrived")
TRACE_MSG(MMISTK_WIN_1981_112_2_18_2_55_55_35,"SelectItemOptionWinHandleMsg:PNULL == s_mmistk_cur_menu_info_ptr[dual_sys]")
TRACE_MSG(MMISTK_WIN_2401_112_2_18_2_55_56_36,"MMISTK_OnDisplayTextCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_2408_112_2_18_2_55_56_37,"[MMI_STK] enter MMISTK_OnDisplayTextCmd")
TRACE_MSG(MMISTK_WIN_2442_112_2_18_2_55_56_38,"[MMI_STK] get if the text is high priority %d,if is display text cnf = %d, if is diaplaying text %d, is_ir= %d")
TRACE_MSG(MMISTK_WIN_2460_112_2_18_2_55_56_39,"[MMI_STK] leave MMISTK_OnDisplayTextCmd because low priority")
TRACE_MSG(MMISTK_WIN_2473_112_2_18_2_55_56_40,"[MMI_STK] MMISTK_OnDisplayTextCmd, if is icon exist = %d, the icon id is = %d, the text length is %d")
TRACE_MSG(MMISTK_WIN_2485_112_2_18_2_55_56_41,"[MMI_STK] leave MMISTK_OnDisplayTextCmd because the data not understood by ME")
TRACE_MSG(MMISTK_WIN_2498_112_2_18_2_55_57_42,"[MMI_STK] DisplayText  timer_duration = %d")
TRACE_MSG(MMISTK_WIN_2508_112_2_18_2_55_57_43,"[MMI_STK] param_in->text_str.len = %d")
TRACE_MSG(MMISTK_WIN_2510_112_2_18_2_55_57_44,"[MMI_STK] s_mmistk_displaydata.length = %d")
TRACE_MSG(MMISTK_WIN_2548_112_2_18_2_55_57_45,"[MMI_STK] leave MMISTK_OnDisplayTextCmd")
TRACE_MSG(MMISTK_WIN_2786_112_2_18_2_55_57_46,"MMISTK_OnGetInkeyCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_2792_112_2_18_2_55_57_47,"[MMI_STK] enter MMISTK_OnGetInkeyCmd")
TRACE_MSG(MMISTK_WIN_2827_112_2_18_2_55_57_48,"[MMI_STK] leave MMISTK_OnGetInkeyCmd because CanDisplayStk() return FALSE")
TRACE_MSG(MMISTK_WIN_2842_112_2_18_2_55_57_49,"[MMI_STK] s_mmistk_inkey_type = %d, help_info=%d, is_icon_id=%d")
TRACE_MSG(MMISTK_WIN_2849_112_2_18_2_55_57_50,"[MMI_STK] s_mmistk_displaydata.wstr_len = %d")
TRACE_MSG(MMISTK_WIN_2872_112_2_18_2_55_57_51,"[MMI_STK] leave MMISTK_OnGetInkeyCmd, start timer(%d), timer duration = %d")
TRACE_MSG(MMISTK_WIN_3111_112_2_18_2_55_58_52,"MMISTK_OnGetInputCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_3118_112_2_18_2_55_58_53,"[MMI_STK] enter MMISTK_OnGetInputCmd")
TRACE_MSG(MMISTK_WIN_3153_112_2_18_2_55_58_54,"[MMI_STK] leave MMISTK_OnGetInputCmd because CanDisplayStk() return FALSE")
TRACE_MSG(MMISTK_WIN_3170_112_2_18_2_55_58_55,"[MMI_STK] s_mmistk_input_type = %d, s_mmistk_input_is_echo = %d, s_mmistk_input_is_pack_req = %d, s_mmistk_is_input_help_info=%d,s_mmistk_input_is_icon_id=%d")
TRACE_MSG(MMISTK_WIN_3171_112_2_18_2_55_58_56,"[MMI_STK] s_mmistk_input_length.min_len = %d, s_mmistk_input_length.max_len = %d")
TRACE_MSG(MMISTK_WIN_3177_112_2_18_2_55_58_57,"[MMI_STK] param_in->text_str.len = %d")
TRACE_MSG(MMISTK_WIN_3186_112_2_18_2_55_58_58,"[MMI_STK] s_mmistk_displaydata.length = %d, is_default_text = %d,default_text.len=%d")
TRACE_MSG(MMISTK_WIN_3206_112_2_18_2_55_58_59,"[MMI_STK] leave MMISTK_OnGetInputCmd, start timer(%d), timer duration = %d")
TRACE_MSG(MMISTK_WIN_3221_112_2_18_2_55_58_60,"MMISTK: MMISTK_OnMoreTimeCmd")
TRACE_MSG(MMISTK_WIN_3229_112_2_18_2_55_58_61,"MMISTK: MMISTK_OnMoreTimeCmd")
TRACE_MSG(MMISTK_WIN_3317_112_2_18_2_55_58_62,"[MMI_STK] s_mmistk_displaydata.length = %d")
TRACE_MSG(MMISTK_WIN_3360_112_2_18_2_55_58_63,"MMISTK_OnSendSmsCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_3373_112_2_18_2_55_58_64,"[MMI_STK] param_in->is_alpha_id = %d, param_in->alpha_id.len = %d, param_in->is_packet_IND = %d, param_in->is_address = %d")
TRACE_MSG(MMISTK_WIN_3378_112_2_18_2_55_58_65,"[MMI_STK] param_in->address.number_type = 0x%02x, param_in->address.number_plan = 0x%02x, param_in->address.number_len = %d")
TRACE_MSG(MMISTK_WIN_3398_112_2_18_2_55_58_66,"[MMI_STK] MMISTK_OnSendSmsCmd s_mmistk_sendsms_cnf=%d")
TRACE_MSG(MMISTK_WIN_3468_112_2_18_2_55_58_67,"[MMI_STK] MNSMS_SendSTKSmsPDU return MN_RETURN_SUCCESS")
TRACE_MSG(MMISTK_WIN_3478_112_2_18_2_55_58_68,"[MMI_STK] MNSMS_SendSTKSmsPDU return MN_RETURN_FAILURE, the code is %d")
TRACE_MSG(MMISTK_WIN_3480_112_2_18_2_55_58_69,"[MMI_STK] leave MMISTK_OnSendSmsCmd")
TRACE_MSG(MMISTK_WIN_3610_112_2_18_2_55_59_70,"MMISTK_OnSetupCallCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_3635_112_2_18_2_55_59_71,"[MMI_STK] enter MMISTK_OnSetupCallCmd")
TRACE_MSG(MMISTK_WIN_3644_112_2_18_2_55_59_72,"[MMI_STK] leave MMISTK_OnSetupCallCmd because no call address")
TRACE_MSG(MMISTK_WIN_3649_112_2_18_2_55_59_73,"[MMI_STK] get make the call type is %d")
TRACE_MSG(MMISTK_WIN_3664_112_2_18_2_55_59_74,"[MMI_STK] leave MMISTK_OnSetupCallCmd because there is in call already")
TRACE_MSG(MMISTK_WIN_3695_112_2_18_2_55_59_75,"[MMI_STK] param_in->is_cs_alpha_id = %d, param_in->cs_alpha_id.len = %d")
TRACE_MSG(MMISTK_WIN_3696_112_2_18_2_55_59_76,"[MMI_STK] param_in->is_uc_alpha_id = %d, param_in->uc_alpha_id.len = %d")
TRACE_MSG(MMISTK_WIN_3723_112_2_18_2_55_59_77,"[MMI_STK] s_is_setup_call_duration=%d,duration=%d")
TRACE_MSG(MMISTK_WIN_3739_112_2_18_2_55_59_78,"[MMI_STK] MMIAPICOM_BcdToStr tel_ptr=%s, tel_len=%d")
TRACE_MSG(MMISTK_WIN_3752_112_2_18_2_55_59_79,"[MMI_STK] param_in->is_cps = %d,param_in->cps.len = %d")
TRACE_MSG(MMISTK_WIN_3799_112_2_18_2_55_59_80,"[MMI_STK] leave MMISTK_OnSetupCallCmd")
TRACE_MSG(MMISTK_WIN_3873_112_2_18_2_55_59_81,"[MMI_STK] s_mmistk_displaydata.wstr_len = %d")
TRACE_MSG(MMISTK_WIN_3909_112_2_18_2_55_59_82,"GetPlayToneId: the audio tone is %d")
TRACE_MSG(MMISTK_WIN_3980_112_2_18_2_55_59_83,"MMISTK_OnPlayToneCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_4009_112_2_18_2_56_0_84,"[MMI_STK] param_in->is_alpha_id = %d, param_in->alpha_id.len = %d")
TRACE_MSG(MMISTK_WIN_4016_112_2_18_2_56_0_85,"[MMI_STK] param_in->alpha_id.len = %d")
TRACE_MSG(MMISTK_WIN_4030_112_2_18_2_56_0_86,"[MMI_STK] enter MMISTK_OnPlayToneCmd, param_in->is_tone_exist = %d, the tone type is %d")
TRACE_MSG(MMISTK_WIN_4031_112_2_18_2_56_0_87,"[MMI_STK] param_in->is_duration_exist = %d, param_in->duration.duration = %d")
TRACE_MSG(MMISTK_WIN_4053_112_2_18_2_56_0_88,"[MMI_STK] leave MMISTK_OnPlayToneCmd, start timer(%d), timer duration = %d")
TRACE_MSG(MMISTK_WIN_4088_112_2_18_2_56_0_89,"MMISTK_OnProvideLICmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_4096_112_2_18_2_56_0_90,"[MMI_STK] enter MMISTK_OnProvideLICmd, info_type = %d")
TRACE_MSG(MMISTK_WIN_4234_112_2_18_2_56_0_91,"[MMI_STK] leave MMISTK_OnProvideLICmd")
TRACE_MSG(MMISTK_WIN_4258_112_2_18_2_56_0_92,"MMISTK_OnSendSSCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_4266_112_2_18_2_56_0_93,"[MMI_STK] enter MMISTK_OnSendSSCmd")
TRACE_MSG(MMISTK_WIN_4292_112_2_18_2_56_0_94,"MMISTK_OnSendSSCmd: currently busy on SS transaction, return")
TRACE_MSG(MMISTK_WIN_4302_112_2_18_2_56_0_95,"MMISTK_OnSendSSCmd: currently busy on USSD transaction, return")
TRACE_MSG(MMISTK_WIN_4346_112_2_18_2_56_0_96,"[MMI_STK] bcd_str length = %d")
TRACE_MSG(MMISTK_WIN_4383_112_2_18_2_56_0_97,"[MMI_STK] phone_oper = %d, send ss result is %d")
TRACE_MSG(MMISTK_WIN_4400_112_2_18_2_56_0_98,"[MMI_STK] send ss failure, the result is= %d")
TRACE_MSG(MMISTK_WIN_4403_112_2_18_2_56_0_99,"[MMI_STK] leave MMISTK_OnSendSSCmd")
TRACE_MSG(MMISTK_WIN_4422_112_2_18_2_56_0_100,"MMISTK_OnSendUSSDCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_4430_112_2_18_2_56_0_101,"[MMI_STK] enter MMISTK_OnSendUSSDCmd")
TRACE_MSG(MMISTK_WIN_4454_112_2_18_2_56_0_102,"MMISTK_OnSendUSSDCmd: currently busy on SS transaction, return")
TRACE_MSG(MMISTK_WIN_4464_112_2_18_2_56_0_103,"MMISTK_OnSendUSSDCmd: currently busy on USSD transaction, return")
TRACE_MSG(MMISTK_WIN_4504_112_2_18_2_56_1_104,"[MMI_STK] param_in->ussd.data_code_scheme %d")
TRACE_MSG(MMISTK_WIN_4505_112_2_18_2_56_1_105,"[MMI_STK] param_in->ussd.str_len %d")
TRACE_MSG(MMISTK_WIN_4524_112_2_18_2_56_1_106,"[MMI_STK] send originate ussd success")
TRACE_MSG(MMISTK_WIN_4528_112_2_18_2_56_1_107,"[MMI_STK] send ussd failure")
TRACE_MSG(MMISTK_WIN_4533_112_2_18_2_56_1_108,"[MMI_STK] leave MMISTK_OnSendUSSDCmd")
TRACE_MSG(MMISTK_WIN_4579_112_2_18_2_56_1_109,"OnRefreshCnfSimFileChange: dual_sys=%d,is_file_change=%d,")
TRACE_MSG(MMISTK_WIN_4617_112_2_18_2_56_1_110,"OnRefreshCnfSimFileChange:sim_file = %d")
TRACE_MSG(MMISTK_WIN_4625_112_2_18_2_56_1_111,"OnRefreshCnfSimFileChange: No file change")
TRACE_MSG(MMISTK_WIN_4649_112_2_18_2_56_1_112,"MMISTK_OnRefreshCnf: dual_sys=%d, is_ok=%d")
TRACE_MSG(MMISTK_WIN_4715_112_2_18_2_56_1_113,"MMISTK_OnRefreshCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_4722_112_2_18_2_56_1_114,"[MMI_STK] enter MMISTK_OnRefreshCmd, refresh_type = %d,dual_sys=%d")
TRACE_MSG(MMISTK_WIN_4791_112_2_18_2_56_1_115,"refresh type = %d")
TRACE_MSG(MMISTK_WIN_4795_112_2_18_2_56_1_116,"[MMI_STK] leave MMISTK_OnRefreshCmd")
TRACE_MSG(MMISTK_WIN_4819_112_2_18_2_56_1_117,"MMISTK_OnSetupEventlistCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_4824_112_2_18_2_56_1_118,"[MMI_STK] enter MMISTK_OnSetupEventlistCmd")
TRACE_MSG(MMISTK_WIN_4834_112_2_18_2_56_1_119,"[MMI_STK] leave MMISTK_OnSetupEventlistCmd")
TRACE_MSG(MMISTK_WIN_4855_112_2_18_2_56_1_120,"MMISTK_OnSetupIdletextCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_4860_112_2_18_2_56_1_121,"[MMI_STK] enter MMISTK_OnSetupIdletextCmd")
TRACE_MSG(MMISTK_WIN_4929_112_2_18_2_56_1_122,"[MMI_STK] leave MMISTK_OnSetupIdletextCmd")
TRACE_MSG(MMISTK_WIN_4948_112_2_18_2_56_1_123,"MMISTK_OnSendDtmfCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_4975_112_2_18_2_56_1_124,"[MMI_STK] enter MMISTK_OnSendDtmfCmd")
TRACE_MSG(MMISTK_WIN_4977_112_2_18_2_56_1_125,"[MMI_STK]OnSendDtmfCmd: param_in->is_alpha_id = %d,param_in->alpha_id.len = %d,is_icon_id=%d")
TRACE_MSG(MMISTK_WIN_4999_112_2_18_2_56_2_126,"[MMI_STK]OnSendDtmfCmd: param_in->dtmf.len is %d")
TRACE_MSG(MMISTK_WIN_5008_112_2_18_2_56_2_127,"[MMI_STK]OnSendDtmfCmd: s_mmistk_dtmf_len is %d")
TRACE_MSG(MMISTK_WIN_5024_112_2_18_2_56_2_128,"[MMI_STK]OnSendDtmfCmd: s_is_mmi_dtmf_pause=%d")
TRACE_MSG(MMISTK_WIN_5064_112_2_18_2_56_2_129,"[MMI_STK] leave MMISTK_OnSendDtmfCmd")
TRACE_MSG(MMISTK_WIN_5087_112_2_18_2_56_2_130,"MMISTK_OnLangNotifyCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_5094_112_2_18_2_56_2_131,"[MMI_STK] enter MMISTK_OnLangNotifyCmd specific_lang_noti=%d")
TRACE_MSG(MMISTK_WIN_5117_112_2_18_2_56_2_132,"[MMI_STK] MMISTK_OnLangNotifyCmd lang=%s, language_type=%d")
TRACE_MSG(MMISTK_WIN_5120_112_2_18_2_56_2_133,"[MMI_STK] leave MMISTK_OnLangNotifyCmd")
TRACE_MSG(MMISTK_WIN_5145_112_2_18_2_56_2_134,"MMISTK_OnCcToMmiCmd arg is NULL")
TRACE_MSG(MMISTK_WIN_5156_112_2_18_2_56_2_135,"[MMI_STK] MMISTK_OnCcToMmiCmd dual_sys=%d, is_simc_data=%d, is_ccp1=%d, is_ccp2=%d, is_cps=%d, is_alpha_id=%d, len=%d, result=%d, data_type=%d, num_len=%d, data_type=%d")
TRACE_MSG(MMISTK_WIN_5210_112_2_18_2_56_2_136,"[MMI_STK] Leave MMISTK_OnCcToMmiCmd is_cc_clt_success=%d")
TRACE_MSG(MMISTK_WIN_5230_112_2_18_2_56_2_137,"MMISTK_OnSSControlRspCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_5237_112_2_18_2_56_2_138,"[MMI_STK] leave MMISTK_OnSSControlRspCmd")
TRACE_MSG(MMISTK_WIN_5257_112_2_18_2_56_2_139,"MMISTK_OnUSSDControlRspCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_5264_112_2_18_2_56_2_140,"[MMI_STK] leave MMISTK_OnUSSDControlRspCmd")
TRACE_MSG(MMISTK_WIN_5292_112_2_18_2_56_2_141,"MMISTK_OnSmsCtlToMmiCmd arg is NULL")
TRACE_MSG(MMISTK_WIN_5301_112_2_18_2_56_2_142,"[MMI_STK] MMISTK_OnSmsCtlToMmiCmd dual_sys=%d, is_alpha_id=%d, len=%d, is_dest_me=%d, is_dest_sc=%d, result=%d")
TRACE_MSG(MMISTK_WIN_5381_112_2_18_2_56_2_143,"[MMI_STK] Leave MMISTK_OnSmsCtlToMmiCmd is_sms_clt_success=%d")
TRACE_MSG(MMISTK_WIN_5400_112_2_18_2_56_2_144,"MMISTK_OnSMSP2PRspCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_5407_112_2_18_2_56_2_145,"[MMI_STK] leave MMISTK_OnSMSP2PRspCmd")
TRACE_MSG(MMISTK_WIN_5426_112_2_18_2_56_2_146,"MMISTK_OnSMSCBRspCmd:PNULL == arg")
TRACE_MSG(MMISTK_WIN_5432_112_2_18_2_56_2_147,"[MMI_STK] leave MMISTK_OnSMSCBRspCmd")
TRACE_MSG(MMISTK_WIN_5470_112_2_18_2_56_2_148,"[MMI_STK] enter CheckUnCloseWin, s_mmistk_close_window = %d, curr_win = %d")
TRACE_MSG(MMISTK_WIN_5531_112_2_18_2_56_3_149,"[MMI_STK] leave CheckUnCloseWin")
TRACE_MSG(MMISTK_WIN_5543_112_2_18_2_56_3_150,"[MMI_STK] enter CloseAllStkWin")
TRACE_MSG(MMISTK_WIN_5555_112_2_18_2_56_3_151,"[MMI_STK] leave CloseAllStkWin")
TRACE_MSG(MMISTK_WIN_5583_112_2_18_2_56_3_152,"mmistk_win.c: is in proactive stk session")
TRACE_MSG(MMISTK_WIN_5588_112_2_18_2_56_3_153,"mmistk_win.c: is not in proactive stk session")
TRACE_MSG(MMISTK_WIN_5604_112_2_18_2_56_3_154,"[MMI_STK] enter MMISTK_OnSessionTerminateCmd")
TRACE_MSG(MMISTK_WIN_5610_112_2_18_2_56_3_155,"[MMI_STK] leave MMISTK_OnSessionTerminateCmd dual_sys=%d")
TRACE_MSG(MMISTK_WIN_5651_112_2_18_2_56_3_156,"[MMI_STK] leave MMISTK_OnSessionTerminateCmd")
TRACE_MSG(MMISTK_WIN_5668_112_2_18_2_56_3_157,"GetMenuActionIdName:PNULL == name_ptr")
TRACE_MSG(MMISTK_WIN_5708_112_2_18_2_56_3_158,"[MMI_STK] SetMenuActionIdData Param Error!")
TRACE_MSG(MMISTK_WIN_5763_112_2_18_2_56_3_159,"[MMI_STK] enter MMISTK_MainMenuWinHandleMsg, msg_id = 0x%04X, s_mmistk_indmenusel_cnf = %d")
TRACE_MSG(MMISTK_WIN_5841_112_2_18_2_56_3_160,"MMISTK_MainMenuWinHandleMsg need_item_cont_ptr is NULL")
TRACE_MSG(MMISTK_WIN_5954_112_2_18_2_56_3_161,"[MMI_STK] leave MMISTK_MainMenuWinHandleMsg(0x%02X), s_mmistk_indmenusel_cnf = %d, result(%d)")
TRACE_MSG(MMISTK_WIN_5978_112_2_18_2_56_3_162,"[MMI_STK] enter MenuWinHandleMsg, msg_id = 0x%04X, s_mmistk_selectitem_cnf = %d")
TRACE_MSG(MMISTK_WIN_6057_112_2_18_2_56_4_163,"MenuWinHandleMsg need_item_cont_ptr is NULL")
TRACE_MSG(MMISTK_WIN_6097_112_2_18_2_56_4_164,"[MMI_STK] after timer duration %d, Timer(%d) arrived")
TRACE_MSG(MMISTK_WIN_6161_112_2_18_2_56_4_165,"MenuWinHandleMsg ::MSG_CTL_OK s_mmistk_cur_menu_info_ptr")
TRACE_MSG(MMISTK_WIN_6298_112_2_18_2_56_4_166,"[MMI_STK] leave MenuWinHandleMsg(0x%02X), s_mmistk_selectitem_cnf = %d, result(%d)")
TRACE_MSG(MMISTK_WIN_6319_112_2_18_2_56_4_167,"[MMI_STK] enter DisplayTextWinHandleMsg, msg_id = 0x%04X, s_mmistk_displaytext_cnf = %d, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_6435_112_2_18_2_56_4_168,"[MMI_STK] after timer duration %d, Timer(%d) arrived")
TRACE_MSG(MMISTK_WIN_6536_112_2_18_2_56_5_169,"[MMI_STK] leave DisplayTextWinHandleMsg(0x%02X), s_mmistk_displaytext_cnf = %d, result = %d")
TRACE_MSG(MMISTK_WIN_6560_112_2_18_2_56_5_170,"[MMI_STK] enter GetInkeyWinHandleMsg, msg_id = 0x%04X, s_mmistk_getinkey_cnf = %d, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_6809_112_2_18_2_56_5_171,"[MMI_STK] after timer duration %d, Timer(%d) arrived")
TRACE_MSG(MMISTK_WIN_6872_112_2_18_2_56_5_172,"[MMI_STK] leave GetInkeyWinHandleMsg(0x%02X), s_mmistk_getinkey_cnf = %d, result(%d)")
TRACE_MSG(MMISTK_WIN_6898_112_2_18_2_56_5_173,"[MMI_STK] enter GetInputWinHandleMsg, msg_id = 0x%04X, s_mmistk_getinput_cnf = %d, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_6946_112_2_18_2_56_5_174,"[MMI_STK] GetInputWinHandleMsg, SIMAT_TR_TERMINATED_BY_USER, the result is = %d")
TRACE_MSG(MMISTK_WIN_6961_112_2_18_2_56_5_175,"[MMI_STK] GetInputWinHandleMsg, SIMAT_TR_BACK_MOVE, the result is = %d")
TRACE_MSG(MMISTK_WIN_7029_112_2_18_2_56_6_176,"[MMI_STK] next_act = %d, str_info.wstr_len = %d")
TRACE_MSG(MMISTK_WIN_7047_112_2_18_2_56_6_177,"[MMI_STK] next_act = %d, str_info.wstr_len = %d")
TRACE_MSG(MMISTK_WIN_7066_112_2_18_2_56_6_178,"[MMI_STK] next_act = %d, str_info.wstr_len = %d")
TRACE_MSG(MMISTK_WIN_7093_112_2_18_2_56_6_179,"[MMI_STK] next_act = %d, txt_info.str_len = %d")
TRACE_MSG(MMISTK_WIN_7124_112_2_18_2_56_6_180,"[MMI_STK] after MMIStr2SIMATStr, s_mmistk_input_is_pack_req = %d")
TRACE_MSG(MMISTK_WIN_7167_112_2_18_2_56_6_181,"[MMI_STK] after timer duration %d, Timer(%d) arrived")
TRACE_MSG(MMISTK_WIN_7232_112_2_18_2_56_6_182,"[MMI_STK] leave GetInputWinHandleMsg(0x%02X), s_mmistk_getinput_cnf = %d, result(%d)")
TRACE_MSG(MMISTK_WIN_7254_112_2_18_2_56_6_183,"[MMI_STK] enter SendSmsWinHandleMsg, msg_id = 0x%04X, s_mmistk_sendsms_cnf = %d, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_7302_112_2_18_2_56_6_184,"[MMI_STK] leave SendSmsWinHandleMsg(0x%02X), s_mmistk_sendsms_cnf = %d, result(%d)")
TRACE_MSG(MMISTK_WIN_7323_112_2_18_2_56_6_185,"[MMI_STK] enter PlayToneWinHandleMsg, msg_id = 0x%04X, s_mmistk_playtone_cnf = %d, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_7417_112_2_18_2_56_6_186,"[MMI_STK] leave PlayToneWinHandleMsg(0x%02X), s_mmistk_playtone_cnf = %d, result(%d)")
TRACE_MSG(MMISTK_WIN_7450_112_2_18_2_56_6_187,"MMISTK_SendReqToSs:PNULL == ss_oper")
TRACE_MSG(MMISTK_WIN_7547_112_2_18_2_56_7_188,"[MMI_STK] s_mmistk_displaydata.length = %d")
TRACE_MSG(MMISTK_WIN_7581_112_2_18_2_56_7_189,"[MMI_STK] StartSetupCallOperation dual_sys=%d, s_mmistk_setup_call_type=%d")
TRACE_MSG(MMISTK_WIN_7600_112_2_18_2_56_7_190,"[MMI_STK] SetupCallWinHandleMsg because there is in call already")
TRACE_MSG(MMISTK_WIN_7681_112_2_18_2_56_7_191,"[MMI_STK] enter SetupCallWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_7742_112_2_18_2_56_7_192,"[MMI_STK] leave SetupCallWinHandleMsg")
TRACE_MSG(MMISTK_WIN_7777_112_2_18_2_56_7_193,"[MMI_STK] MMISTK_SetupCall, s_is_mmistk_setup_call=%d")
TRACE_MSG(MMISTK_WIN_7783_112_2_18_2_56_7_194,"[MMI_STK] MMISTK_SetupCall, is_oper_cnf=%d,is_call_active=%d,s_mmistk_call_oper_type=%d")
TRACE_MSG(MMISTK_WIN_7988_112_2_18_2_56_7_195,"[MMI_STK] enter SendSSWinHandleMsg, msg_id = 0x%04X")
TRACE_MSG(MMISTK_WIN_8026_112_2_18_2_56_8_196,"[MMI_STK] leave SendSSWinHandleMsg(0x%02X), result(%d)")
TRACE_MSG(MMISTK_WIN_8041_112_2_18_2_56_8_197,"[MMI_STK] MMISTK_OnSendSSCnf, s_mmistk_sendss_waiting_respond= %d")
TRACE_MSG(MMISTK_WIN_8044_112_2_18_2_56_8_198,"MMISTK_OnSendSSCnf:PNULL == arg")
TRACE_MSG(MMISTK_WIN_8056_112_2_18_2_56_8_199,"send ss success: MN_SS_OPERATION_SUCC result_octs_len=%d")
TRACE_MSG(MMISTK_WIN_8076_112_2_18_2_56_8_200,"send ss failure: MN_SS_OPERATION_REJECT")
TRACE_MSG(MMISTK_WIN_8081_112_2_18_2_56_8_201,"send ss failure: MN_SS_OPERATION_ERROR,The error code is %d")
TRACE_MSG(MMISTK_WIN_8087_112_2_18_2_56_8_202,"MMISTK_OnSendSSCnf:PNULL == ((MnSsRegisterCnfS*)(arg))->result_type")
TRACE_MSG(MMISTK_WIN_8108_112_2_18_2_56_8_203,"[MMI_STK] MMISTK_OnSendUSSDCnf, s_mmistk_sendussd_waiting_respond= %d")
TRACE_MSG(MMISTK_WIN_8136_112_2_18_2_56_8_204,"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_SUCC")
TRACE_MSG(MMISTK_WIN_8143_112_2_18_2_56_8_205,"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_ERROR, ussd info is %d")
TRACE_MSG(MMISTK_WIN_8149_112_2_18_2_56_8_206,"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_REJECT")
TRACE_MSG(MMISTK_WIN_8153_112_2_18_2_56_8_207,"result type = %d")
TRACE_MSG(MMISTK_WIN_8175_112_2_18_2_56_8_208,"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_SUCC")
TRACE_MSG(MMISTK_WIN_8181_112_2_18_2_56_8_209,"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_ERROR, ussd info is %d")
TRACE_MSG(MMISTK_WIN_8186_112_2_18_2_56_8_210,"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_REJECT")
TRACE_MSG(MMISTK_WIN_8190_112_2_18_2_56_8_211,"result type = %d")
TRACE_MSG(MMISTK_WIN_8216_112_2_18_2_56_8_212,"result MMISTK_OnSendRegisterPaswdCnf ::PNULL == arg")
TRACE_MSG(MMISTK_WIN_8220_112_2_18_2_56_8_213,"[MMI_STK] MMISTK_OnSendRegisterPaswdCnf, s_mmistk_sendss_waiting_respond= %d, dual_sys=%d")
TRACE_MSG(MMISTK_WIN_8230_112_2_18_2_56_8_214,"[MMI_STK] send ss success")
TRACE_MSG(MMISTK_WIN_8236_112_2_18_2_56_8_215,"[MMI_STK] send ss failure")
TRACE_MSG(MMISTK_WIN_8279_112_2_18_2_56_8_216,"MMISTK_OnSetupMenuCmd: toplevel_menu_info.menu_item_amount %d")
TRACE_MSG(MMISTK_WIN_8322_112_2_18_2_56_8_217,"MMISTK_OnSetupMenuCmd: the alpha_id.len is %d")
TRACE_MSG(MMISTK_WIN_8330_112_2_18_2_56_8_218,"[MMI_STK] menu_title.wstr_len=%d")
TRACE_MSG(MMISTK_WIN_8376_112_2_18_2_56_8_219,"[MMI_STK] leave OnSetupMenuCmd, load %d top level menus")
TRACE_MSG(MMISTK_WIN_8457_112_2_18_2_56_8_220,"[MMI_STK] enter MMISTK_SendSMSPDUCnf, s_mmistk_sendsms_cnf = %d, send_result = %d, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_8471_112_2_18_2_56_8_221,"[MMI_STK] MMISTK_SendSMSPDUCnf, send success")
TRACE_MSG(MMISTK_WIN_8485_112_2_18_2_56_8_222,"[MMI_STK] MMISTK_SendSMSPDUCnf, send fail, fail code is %d, last_fail_cause=%d")
TRACE_MSG(MMISTK_WIN_8489_112_2_18_2_56_8_223,"[MMI_STK] MMISTK_SendSMSPDUCnf, send fail, fail code is %d")
TRACE_MSG(MMISTK_WIN_8499_112_2_18_2_56_9_224,"[MMI_STK] leave MMISTK_SendSMSPDUCnf, s_mmistk_sendsms_cnf = %d")
TRACE_MSG(MMISTK_WIN_8513_112_2_18_2_56_9_225,"[MMI_STK] leave MMISTK_EnterToplevelMenu Ptr NULL")
TRACE_MSG(MMISTK_WIN_8517_112_2_18_2_56_9_226,"[MMI_STK] enter MMISTK_EnterToplevelMenu dual_sys=%d menu_item_amount = %d")
TRACE_MSG(MMISTK_WIN_8521_112_2_18_2_56_9_227,"[MMI_STK] leave MMISTK_EnterToplevelMenu because top level menu items <=0 or STk Main Menu Window already open")
TRACE_MSG(MMISTK_WIN_8528_112_2_18_2_56_9_228,"[MMI_STK] enter MMISTK_EnterToplevelMenu")
TRACE_MSG(MMISTK_WIN_8539_112_2_18_2_56_9_229,"[MMI_STK] leave MMIAPISTK_EnterToplevelMenu")
TRACE_MSG(MMISTK_WIN_8872_112_2_18_2_56_10_230,"[MMISTK]: HandleSIMPlugNotifySTKFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMISTK_WIN_9146_112_2_18_2_56_10_231,"[MMI_STK] StartLaunchBrowserOperation dual_sys=%d, url_len=%d,launch_browser_type")
TRACE_MSG(MMISTK_WIN_9163_112_2_18_2_56_10_232,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED with url browser is running!")
TRACE_MSG(MMISTK_WIN_9173_112_2_18_2_56_11_233,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url success!")
TRACE_MSG(MMISTK_WIN_9179_112_2_18_2_56_11_234,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url error!")
TRACE_MSG(MMISTK_WIN_9191_112_2_18_2_56_11_235,"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST with url browser is not running!")
TRACE_MSG(MMISTK_WIN_9201_112_2_18_2_56_11_236,"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser with url success!")
TRACE_MSG(MMISTK_WIN_9207_112_2_18_2_56_11_237,"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser with url error!")
TRACE_MSG(MMISTK_WIN_9215_112_2_18_2_56_11_238,"[MMI_STK] launch_browser_type is default with url!")
TRACE_MSG(MMISTK_WIN_9229_112_2_18_2_56_11_239,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED default browser is running!")
TRACE_MSG(MMISTK_WIN_9239_112_2_18_2_56_11_240,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser default success!")
TRACE_MSG(MMISTK_WIN_9245_112_2_18_2_56_11_241,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser default error!")
TRACE_MSG(MMISTK_WIN_9257_112_2_18_2_56_11_242,"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST default browser is not running!")
TRACE_MSG(MMISTK_WIN_9267_112_2_18_2_56_11_243,"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser default success!")
TRACE_MSG(MMISTK_WIN_9273_112_2_18_2_56_11_244,"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser default error!")
TRACE_MSG(MMISTK_WIN_9281_112_2_18_2_56_11_245,"[MMI_STK] launch_browser_type is default with default url!")
TRACE_MSG(MMISTK_WIN_9311_112_2_18_2_56_11_246,"[MMI_STK] enter LaunchBrowserWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_9337_112_2_18_2_56_11_247,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser user deny")
TRACE_MSG(MMISTK_WIN_9368_112_2_18_2_56_11_248,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser user deny")
TRACE_MSG(MMISTK_WIN_9394_112_2_18_2_56_11_249,"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url error!")
TRACE_MSG(MMISTK_WIN_9403_112_2_18_2_56_11_250,"[MMI_STK] leave LaunchBrowserWinHandleMsg result %d")
TRACE_MSG(MMISTK_WIN_9423_112_2_18_2_56_11_251,"MMISTK_OnLaunchBrowserCmd arg is NULL")
TRACE_MSG(MMISTK_WIN_9440_112_2_18_2_56_11_252,"[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because alloc MnSimatLaunchBrowserIndS fail")
TRACE_MSG(MMISTK_WIN_9458_112_2_18_2_56_11_253,"[MMI_STK] enter MMISTK_OnLaunchBrowserCmd")
TRACE_MSG(MMISTK_WIN_9467_112_2_18_2_56_12_254,"[MMI_STK] dual_sys:%d, launch_browser_type:%d, is_browser_id:%d, is_icon_id:%d, is_alpha_id:%d, is_gate_way:%d, is_bear:%d")
TRACE_MSG(MMISTK_WIN_9476_112_2_18_2_56_12_255,"[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because ExistActiveOrSetupCall")
TRACE_MSG(MMISTK_WIN_9488_112_2_18_2_56_12_256,"[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because MMS sending")
TRACE_MSG(MMISTK_WIN_9537_112_2_18_2_56_12_257,"[MMI_STK] leave MMISTK_OnLaunchBrowserCmd")
TRACE_MSG(MMISTK_WIN_9581_112_2_18_2_56_12_258,"[MMI_STK]MMISTK_CreateSocket enter netid = %d")
TRACE_MSG(MMISTK_WIN_9597_112_2_18_2_56_12_259,"[MMI_STK]MMISTK_CreateSocket, e = %d")
TRACE_MSG(MMISTK_WIN_9608_112_2_18_2_56_12_260,"[MMI_STK]sci_sock_setsockopt, result=%d")
TRACE_MSG(MMISTK_WIN_9614_112_2_18_2_56_12_261,"[MMI_STK]MMISTK_CreateSocket, return %ld")
TRACE_MSG(MMISTK_WIN_9630_112_2_18_2_56_12_262,"[MMI_STK]MMISTK_CloseSocket: %d")
TRACE_MSG(MMISTK_WIN_9665_112_2_18_2_56_12_263,"[MMI_STK]connection is established, sock=%ld")
TRACE_MSG(MMISTK_WIN_9670_112_2_18_2_56_12_264,"[MMI_STK]MMISTK_ConnectSocket, err = %d")
TRACE_MSG(MMISTK_WIN_9683_112_2_18_2_56_12_265,"[MMI_STK]SocketConnect, sock=%d, return=%d")
TRACE_MSG(MMISTK_WIN_9707_112_2_18_2_56_12_266,"[MMI_STK]MMISTK_SocketSendData error= %d")
TRACE_MSG(MMISTK_WIN_9711_112_2_18_2_56_12_267,"[MMI_STK]MMISTK_SocketSendData, s=%d, return %d")
TRACE_MSG(MMISTK_WIN_9727_112_2_18_2_56_12_268,"[MMI_STK] MMISTK_AssignChannelID enter!")
TRACE_MSG(MMISTK_WIN_9735_112_2_18_2_56_12_269,"[MMI_STK] MMISTK_AssignChannelID channel_id = %d!")
TRACE_MSG(MMISTK_WIN_9759_112_2_18_2_56_12_270,"[MMI_STK] MMISTK_GetAvailableChannelID = %d")
TRACE_MSG(MMISTK_WIN_9764_112_2_18_2_56_12_271,"[MMI_STK] MMISTK_GetAvailableChannelID = 0")
TRACE_MSG(MMISTK_WIN_9779_112_2_18_2_56_13_272,"[MMI_STK] MMISTK_ReleaseChannelID enter!")
TRACE_MSG(MMISTK_WIN_9788_112_2_18_2_56_13_273,"[MMI_STK] MMISTK_ReleaseChannelID channel_id = %d!")
TRACE_MSG(MMISTK_WIN_9805_112_2_18_2_56_13_274,"[MMI_STK] MMISTK_ReleaseAllChannelID enter!")
TRACE_MSG(MMISTK_WIN_9864_112_2_18_2_56_13_275,"[MMI_STK] SIMAT_CnfOpenChannel success!")
TRACE_MSG(MMISTK_WIN_9876_112_2_18_2_56_13_276,"[MMI_STK] MMISTK_ConnectSocket error!")
TRACE_MSG(MMISTK_WIN_9895_112_2_18_2_56_13_277,"[MMI_STK]MMISTK_HandlePdpActiveCnf result =%d")
TRACE_MSG(MMISTK_WIN_9909_112_2_18_2_56_13_278,"[MMI_STK] MMISTK_CreateSocket error!")
TRACE_MSG(MMISTK_WIN_9951_112_2_18_2_56_13_279," MMISTK_OnLaunchBrowserCmd ::PNULL == msg_ptr")
TRACE_MSG(MMISTK_WIN_9954_112_2_18_2_56_13_280,"[MMI_STK]:HandleSTKPDPMsg enter msg_id = %d!")
TRACE_MSG(MMISTK_WIN_10003_112_2_18_2_56_13_281,"[MMI_STK]:HandleSTKPDPMsg exit!")
TRACE_MSG(MMISTK_WIN_10020_112_2_18_2_56_13_282,"[MMI_STK]:MMISTK_ActivatePDPcontext enter!")
TRACE_MSG(MMISTK_WIN_10036_112_2_18_2_56_13_283,"[MMI_STK]:MMIAPIPDP_Active = %d!")
TRACE_MSG(MMISTK_WIN_10055_112_2_18_2_56_13_284,"[MMI_STK] MMISTK_DeactivePDPContext fail!")
TRACE_MSG(MMISTK_WIN_10072_112_2_18_2_56_13_285,"[MMISTK]MMISTK_CnfOpenChannel is_success=%d, result=%d")
TRACE_MSG(MMISTK_WIN_10103_112_2_18_2_56_13_286,"[MMI_STK] MMISTK_AssignChannelID error!")
TRACE_MSG(MMISTK_WIN_10141_112_2_18_2_56_13_287,"[MMI_STK] SIMAT_CnfOpenChannel success!")
TRACE_MSG(MMISTK_WIN_10166_112_2_18_2_56_14_288,"[MMI_STK] enter OpenChannelWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d")
TRACE_MSG(MMISTK_WIN_10258_112_2_18_2_56_14_289,"[MMI_STK] leave OpenChannelWinHandleMsg result %d")
TRACE_MSG(MMISTK_WIN_10438_112_2_18_2_56_14_290,"[MMI_STK] enter MMISTK_OnOpenChannelCmd")
TRACE_MSG(MMISTK_WIN_10442_112_2_18_2_56_14_291,"[MMI_STK] MMISTK_OnOpenChannelCmd arg == PNULL!!!")
TRACE_MSG(MMISTK_WIN_10504_112_2_18_2_56_14_292,"[MMI_STK] leave MMISTK_OnSetupCallCmd because there is in call already")
TRACE_MSG(MMISTK_WIN_10519_112_2_18_2_56_14_293,"[MMI_STK] is_alpha_id:%d, len:%d is_other_address:%d, is_text_str_login:%d, is_text_str_pwd:%d, is_trans_level:%d, is_data_dest_addr:%d, is_net_access_name:%d,is_icon_id:%d")
TRACE_MSG(MMISTK_WIN_10525_112_2_18_2_56_14_294,"[MMI_STK] bear_desc.bear_type = %d, len = %d, bear_desc_qos = %s")
TRACE_MSG(MMISTK_WIN_10536_112_2_18_2_56_14_295,"[MMI_STK] param_in->buf_size = %d")
TRACE_MSG(MMISTK_WIN_10544_112_2_18_2_56_14_296,"[MMI_STK] param_in->bear_desc.bear_type = %d")
TRACE_MSG(MMISTK_WIN_10549_112_2_18_2_56_14_297,"[MMI_STK] leave MMISTK_OnOpenChannelCmd because beyond bear type!")
TRACE_MSG(MMISTK_WIN_10553_112_2_18_2_56_14_298,"[MMI_STK] param_in->data_dest_address.addr_type = %d")
TRACE_MSG(MMISTK_WIN_10557_112_2_18_2_56_14_299,"[MMI_STK] leave MMISTK_OnOpenChannelCmd because beyond dest addr type!")
TRACE_MSG(MMISTK_WIN_10576_112_2_18_2_56_14_300,"[MMI_STK] ata_dest_address.addr_len = %d, type = %d, dest_address = %s")
TRACE_MSG(MMISTK_WIN_10582_112_2_18_2_56_14_301,"[MMI_STK] port = %d, trans_level_type = %d")
TRACE_MSG(MMISTK_WIN_10630_112_2_18_2_56_15_302,"[MMI_STK] dual_sys:%d, apn_ptr:%s, login_ptr:%s, psw_ptr:%s")
TRACE_MSG(MMISTK_WIN_10635_112_2_18_2_56_15_303,"[MMI_STK] leave MMISTK_OnOpenChannelCmd because sim not ready!")
TRACE_MSG(MMISTK_WIN_10665_112_2_18_2_56_15_304,"[MMI_STK] leave MMITSK_OnOpenChannelCmd because pdp active fail!")
TRACE_MSG(MMISTK_WIN_10681_112_2_18_2_56_15_305,"[MMI_STK] MMISTK_AssignChannelID MMISTK_PDP_STATUS_ACTIVE_OK error!")
TRACE_MSG(MMISTK_WIN_10689_112_2_18_2_56_15_306,"[MMI_STK] MMISTK_PDP_STATUS_ACTIVE_OK bear_desc.bear_type = %d, len = %d, bear_desc_qos = %s")
TRACE_MSG(MMISTK_WIN_10700_112_2_18_2_56_15_307,"[MMI_STK] MMISTK_PDP_STATUS_ACTIVE_OK param_in->buf_size = %d")
TRACE_MSG(MMISTK_WIN_10738_112_2_18_2_56_15_308,"[MMI_STK] SIMAT_CnfOpenChannel success!")
TRACE_MSG(MMISTK_WIN_10743_112_2_18_2_56_15_309,"[MMI_STK] leave MMISTK_OnOpenChannelCmd because MMISTK_PDP_STATUS_ACTIVE_OK apn,psw,login not match!")
TRACE_MSG(MMISTK_WIN_10749_112_2_18_2_56_15_310,"[MMI_STK] leave MMISTK_OnOpenChannelCmd because MMISTK_GetPdpActiveStatus activing!")
TRACE_MSG(MMISTK_WIN_10770_112_2_18_2_56_15_311,"[MMI_STK] enter MMISTK_OnCloseChannelCmd")
TRACE_MSG(MMISTK_WIN_10774_112_2_18_2_56_15_312,"[MMI_STK] MMISTK_OnCloseChannelCmd arg == PNULL!!!")
TRACE_MSG(MMISTK_WIN_10829_112_2_18_2_56_15_313,"[MMI_STK] enter MMISTK_OnGetChannelStatusCmd")
TRACE_MSG(MMISTK_WIN_10833_112_2_18_2_56_15_314,"[MMI_STK] MMISTK_OnGetChannelStatusCmd arg == PNULL!!!")
TRACE_MSG(MMISTK_WIN_10854_112_2_18_2_56_15_315,"[MMI_STK] GetChannelStatus success ChannelID = %d")
TRACE_MSG(MMISTK_WIN_10860_112_2_18_2_56_15_316,"[MMI_STK] GetChannelStatus failure")
TRACE_MSG(MMISTK_WIN_10871_112_2_18_2_56_15_317,"[MMI_STK] leave MMISTK_OnGetChannelStatusCmd")
TRACE_MSG(MMISTK_WIN_10899_112_2_18_2_56_15_318,"[MMI_STK] MMISTK_GetSocketDataLen = %ld, channel id= %d")
TRACE_MSG(MMISTK_WIN_10924_112_2_18_2_56_15_319,"[MMI_STK] MMISTK_GetSocketData timer")
TRACE_MSG(MMISTK_WIN_10939_112_2_18_2_56_15_320,"[MMI_STK] HandleSTKSocketGetDataTimer times = %d")
TRACE_MSG(MMISTK_WIN_10969_112_2_18_2_56_16_321,"[MMI_STK] enter MMISTK_OnSendDataCmd")
TRACE_MSG(MMISTK_WIN_10973_112_2_18_2_56_16_322,"[MMI_STK] MMISTK_OnSendDataCmd arg == PNULL!!!")
TRACE_MSG(MMISTK_WIN_11061_112_2_18_2_56_16_323,"[MMI_STK] enter MMISTK_OnReceiveDataCmd")
TRACE_MSG(MMISTK_WIN_11065_112_2_18_2_56_16_324,"[MMI_STK] MMISTK_OnReceiveDataCmd arg == PNULL!!!")
TRACE_MSG(MMISTK_WIN_11142_112_2_18_2_56_16_325,"[MMI_STK] MMISTK_IsStkRefresh s_is_stk_refresh=%d")
TRACE_MSG(MMISTK_WIN_11192_112_2_18_2_56_16_326," MMISTK_GetStkLang ::PNULL == lang_ptr")
TRACE_MSG(MMISTK_WIN_11225_112_2_18_2_56_16_327,"[MMI_STK] MMISTK_OnRelUSSD, s_mmistk_sendussd_waiting_respond=%d, dual_sys=%d")
TRACE_MSG(MMISTK_WIN_11268_112_2_18_2_56_16_328,"MMISTK_HandleSTKTimer timer_id=%d,playtone_timer_id=%d")
TRACE_MSG(MMISTK_WIN_11272_112_2_18_2_56_16_329," MMISTK_GetStkLang ::0 == timer_id")
TRACE_MSG(MMISTK_WIN_11306_112_2_18_2_56_16_330,"[MMI_STK] after play duration, Timer(%d) arrived")
TRACE_MSG(MMISTK_WIN_11426_112_2_18_2_56_16_331," HandleSTKSendDtmfStr ::dual_sys >= MMI_DUAL_SYS_MAX")
TRACE_MSG(MMISTK_WIN_11440_112_2_18_2_56_16_332,"[MMI_STK]HandleSTKSendDtmfStr, the dtmf length is %d, the dtmf value is 0x%d")
TRACE_MSG(MMISTK_WIN_11459_112_2_18_2_56_16_333,"[MMI_STK]HandleSTKSendDtmfStr, is not in speech now, send dtmf fail")
TRACE_MSG(MMISTK_WIN_11544_112_2_18_2_56_17_334,"MMISTK_BrowserTermination dual_sys=%d")
TRACE_MSG(MMISTK_WIN_11681_112_2_18_2_56_17_335,"CreateIconAnimCtrl fail! icon is NULL!")
TRACE_MSG(MMISTK_WIN_11733_112_2_18_2_56_17_336,"DecodeSimImgToBmp img_width =%d, img_height = %d, cs = %d, len = %d")
TRACE_MSG(MMISTK_WIN_11755_112_2_18_2_56_17_337,"DecodeSimImgToBmp MN_SIM_IMG_CODE_TYPE_E is ERROR!!!")
TRACE_MSG(MMISTK_WIN_11776_112_2_18_2_56_17_338,"DecodeBasicImg fail! src data = NULL!")
TRACE_MSG(MMISTK_WIN_11782_112_2_18_2_56_17_339,"DecodeBasicImg fail! icon data error!")
TRACE_MSG(MMISTK_WIN_11789_112_2_18_2_56_17_340,"DecodeBasicImg alloc fail,no mem!")
TRACE_MSG(MMISTK_WIN_11846_112_2_18_2_56_17_341,"DecodeColorImg fail! src data = NULL!")
TRACE_MSG(MMISTK_WIN_11853_112_2_18_2_56_17_342,"DecodeColorImg fail! icon data error!")
TRACE_MSG(MMISTK_WIN_11860_112_2_18_2_56_17_343,"DecodeColorImg alloc fail,no mem!")
TRACE_MSG(MMISTK_WIN_11889_112_2_18_2_56_18_344,"DecodeColorImg data error!")
END_TRACE_MAP(MMI_APP_STK_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_STK_TRC_H_

