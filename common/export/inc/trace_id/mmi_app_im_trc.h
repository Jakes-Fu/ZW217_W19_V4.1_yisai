/******************************************************************************
 ** File Name:      mmi_app_im_trc.h                                         *
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
//trace_id:185
#ifndef _MMI_APP_IM_TRC_H_
#define _MMI_APP_IM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIIM_419_112_2_18_2_24_46_0 "MMIAPIIM_InitNv read IMNV_SOGOU_UPDATE_SETTING error!"
#define MMIIM_637_112_2_18_2_24_47_1 "MMIAPIIM_HWGetAreaType Err"
#define MMIIM_658_112_2_18_2_24_47_2 "MMIAPIIM_HWSetAreaType Err"
#define MMIIM_1335_112_2_18_2_24_48_3 "InitMlData: im_ml_data_ptr is null."
#define MMIIM_1360_112_2_18_2_24_48_4 "== SetMultiLangMethod == can not find method %d in multi-lang data"
#define MMIIM_1393_112_2_18_2_24_48_5 "== InitMlData == can get lang index %d"
#define MMIIM_1571_112_2_18_2_24_49_6 "MMIAPIIM_InitLangKeyTable"
#define MMIIM_1582_112_2_18_2_24_49_7 "MMIAPIIM_InitLangKeyTable"
#define MMIIM_1594_112_2_18_2_24_49_8 "MMIAPIIM_InitLangKeyTable"
#define MMIIM_1605_112_2_18_2_24_49_9 "MMIAPIIM_InitLangKeyTable"
#define MMIIM_1620_112_2_18_2_24_49_10 "MMIAPIIM_InitLangKeyTable"
#define MMIIM_1631_112_2_18_2_24_49_11 "MMIAPIIM_InitLangKeyTable"
#define MMIIM_1682_112_2_18_2_24_49_12 "MMIAPIIM_GetKeyIndexByKeytable ERR PARAM"
#define MMIIM_1695_112_2_18_2_24_49_13 "MMIAPIIM_GetKeyIndexByKeytable"
#define MMIIM_1720_112_2_18_2_24_49_14 "MMIAPIIM_GetKeyIndexByKeytable"
#define MMIIM_1836_112_2_18_2_24_49_15 "MMIIM_GetWcharKeyValueByTable"
#define MMIIM_1880_112_2_18_2_24_49_16 "MMIIM_GetWcharKeyValueByTable"
#define MMIIM_1911_112_2_18_2_24_49_17 "MMIIM_GetWcharKeyValueByTable"
#define MMIIM_BASE_371_112_2_18_2_24_50_18 "MMIIM_DrawSpace"
#define MMIIM_BASE_1479_112_2_18_2_24_52_19 "MMIIM_QueryAsso: context_ptr is null"
#define MMIIM_BASE_1484_112_2_18_2_24_52_20 "MMIIM_QueryAsso: asso_ptr is null"
#define MMIIM_BASE_1489_112_2_18_2_24_52_21 "MMIIM_QueryAsso: asso_len_ptr is null"
#define MMIIM_CSTAR_1125_112_2_18_2_24_58_22 "[MMIIM] Initialize cstar: win_id=%X"
#define MMIIM_CSTAR_1165_112_2_18_2_24_58_23 "[MMIIM] Terminate: data_ptr is null."
#define MMIIM_CSTAR_1169_112_2_18_2_24_58_24 "[MMIIM] Terminate cstar: cs_handle=%08X"
#define MMIIM_CSTAR_1209_112_2_18_2_24_59_25 "[MMIIM] HandleMsg: data_ptr is null."
#define MMIIM_CSTAR_1383_112_2_18_2_24_59_26 "[MMIIM] HandleTpKeySelect: data_ptr is null."
#define MMIIM_CSTAR_1392_112_2_18_2_24_59_27 "[MMIIM] HandleTpKeySelect PNULL == content!"
#define MMIIM_CSTAR_1448_112_2_18_2_24_59_28 "[MMIIM] HandleSysMsg: data_ptr is null."
#define MMIIM_CSTAR_1471_112_2_18_2_24_59_29 "[MMIIM] HandleSysMsg: IsImReloadDirty, method=%04X"
#define MMIIM_CSTAR_1875_112_2_18_2_25_0_30 "[MMIIM] SetMethod: handle is null."
#define MMIIM_CSTAR_1886_112_2_18_2_25_0_31 "[MMIIM] SetMethod: cs_handle=%8X, cs_method=%04X, method=%04X"
#define MMIIM_CSTAR_2122_112_2_18_2_25_1_32 "[MMIIM] ConvertKey PNULL"
#define MMIIM_CSTAR_2464_112_2_18_2_25_1_33 "[MMIIM] ConvertKey: cs_vk=%d, cs_vk_type=%X, event=%d, msg=%X"
#define MMIIM_IM_SWITCHING_2221_112_2_18_2_25_7_34 "[MMIIM] HandleSwitchingIMWinMsg: win_data_ptr is null."
#define MMIIM_IM_SWITCHING_2401_112_2_18_2_25_7_35 "[MMIIM] MMIIM_SetLangCreateWin: im_kernel_ptr is null."
#define MMIIM_IM_SWITCHING_2439_112_2_18_2_25_8_36 "[MMIIM] OnIMClicked: cur_selection >= win_data_ptr->item_counts."
#define MMIIM_IM_SWITCHING_2514_112_2_18_2_25_8_37 "[MMIIM] OnIMReleased: win_data_ptr is null."
#define MMIIM_IM_SWITCHING_2623_112_2_18_2_25_8_38 "[MMIIM] MMIIM_AppendIMListItem: win_data_ptr is null."
#define MMIIM_IM_SWITCHING_2665_112_2_18_2_25_8_39 "[MMIIM] SendChangeMethodMsg: method is GUIIM_M_NONE."
#define MMIIM_IM_SWITCHING_2672_112_2_18_2_25_8_40 "[MMIIM] SendChangeMethodMsg: method_cfg_ptr is null."
#define MMIIM_IM_SWITCHING_2720_112_2_18_2_25_8_41 "[MMIIM] SendChangeMethodMsg: method=%04X, new_method=%04X."
#define MMIIM_IM_SWITCHING_2752_112_2_18_2_25_8_42 "[MMIIM] InitIMDisplayValues: param is not valid."
#define MMIIM_IM_SWITCHING_4278_112_2_18_2_25_11_43 "[MMIIM] HandleSetInputLangWindow: win_data_ptr is null."
#define MMIIM_IM_SWITCHING_4290_112_2_18_2_25_11_44 "[MMIIM] HandleSetInputLangWindow: MSG_OPEN_WINDOW param is null."
#define MMIIM_IM_SWITCHING_4394_112_2_18_2_25_11_45 "[MMIIM] AppendLangItems: win_data_ptr is null."
#define MMIIM_IM_SWITCHING_4448_112_2_18_2_25_12_46 "[MMIIM] OnLangClicked: param is null."
#define MMIIM_SOGOU_1876_112_2_18_2_25_16_47 "[MMIIM] HandleMsg: data_ptr is null."
#define MMIIM_SOGOU_2611_112_2_18_2_25_18_48 "MMI: ConvertKey PNULL"
#define MMIIM_SOGOU_2645_112_2_18_2_25_18_49 "mmiim_sogou.c ConvertKey: doesn't process MSG_KEYLONG_OK, return 1"
#define MMIIM_SOGOU_3781_112_2_18_2_25_20_50 "MMIAPIIM_DrawSkinInRect Err param"
#define MMIIM_SOGOU_3789_112_2_18_2_25_20_51 "MMIAPIIM_DrawSkinInRect Err disprect"
#define MMIIM_SOGOU_3808_112_2_18_2_25_20_52 "MMIAPIIM_DrawSkinInRect"
#define MMIIM_SOGOU_3853_112_2_18_2_25_20_53 "God, MMIAPIIM_DrawSkinInRect memory over"
#define MMIIM_SP_HW_457_112_2_18_2_25_30_54 "mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!"
#define MMIIM_SP_HW_466_112_2_18_2_25_30_55 "mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!"
#define MMIIM_SP_HW_477_112_2_18_2_25_30_56 "mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!"
#define MMIIM_SP_HW_483_112_2_18_2_25_30_57 "mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!"
#define MMIIM_SP_HW_676_112_2_18_2_25_30_58 "[MMIIM] Initialize hw: win_id=%X"
#define MMIIM_SP_HW_906_112_2_18_2_25_31_59 "[MMIIM] Terminate hw: data_ptr is null."
#define MMIIM_SP_HW_910_112_2_18_2_25_31_60 "[MMIIM] Terminate hw: win_id=%X"
#define MMIIM_SP_HW_957_112_2_18_2_25_31_61 "[MMIIM] HandleMsg: data_ptr is null."
#define MMIIM_SP_HW_2444_112_2_18_2_25_34_62 "[MMIIM] RecognizeTrace log:"
#define MMIIM_SP_HW_2445_112_2_18_2_25_34_63 "("
#define MMIIM_SP_HW_2448_112_2_18_2_25_34_64 "%d,%d,"
#define MMIIM_SP_HW_2455_112_2_18_2_25_34_65 ")"
#define MMIIM_SP_HW_3099_112_2_18_2_25_35_66 "[MMIIM] CommitChar: data_ptr is null"
#define MMIIM_SP_HW_3125_112_2_18_2_25_36_67 "[MMIIM] CommitChar: data_ptr is null"
#define MMIIM_SP_HW_3151_112_2_18_2_25_36_68 "[MMIIM] CommitUnderlineChar: data_ptr is null"
#define MMIIM_SP_HW_3207_112_2_18_2_25_36_69 "[MMIIM] LimitSet: data_ptr is null"
#define MMIIM_SP_HW_3333_112_2_18_2_25_36_70 "[MMIIM] PopSymbPanel: data_ptr is null"
#define MMIIM_SP_HW_3364_112_2_18_2_25_36_71 "[MMIIM] InitAttributeInfo: data_ptr is null"
#define MMIIM_SP_TP_BASIC_1138_112_2_18_2_26_41_72 "MMIIM_SP_TP_KbDraw, kb display false."
#define MMIIM_TOUCH_APP_CSTAR_1326_112_2_18_2_26_46_73 "HandleSysMsg MSG_TP_PRESS_DOWN point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_CSTAR_1330_112_2_18_2_26_46_74 "HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_DOWN point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_CSTAR_1358_112_2_18_2_26_46_75 "HandleSysMsg MSG_TP_PRESS_MOVE point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_CSTAR_1362_112_2_18_2_26_46_76 "HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_MOVE point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_CSTAR_1429_112_2_18_2_26_46_77 "HandleSysMsg MSG_TP_PRESS_UP point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_CSTAR_1433_112_2_18_2_26_46_78 "HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_UP point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_SOGOU_1231_112_2_18_2_26_59_79 "HandleSysMsg MSG_TP_PRESS_DOWN point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_SOGOU_1235_112_2_18_2_26_59_80 "HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_DOWN point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_SOGOU_1263_112_2_18_2_26_59_81 "HandleSysMsg MSG_TP_PRESS_MOVE point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_SOGOU_1267_112_2_18_2_26_59_82 "HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_MOVE point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_SOGOU_1334_112_2_18_2_26_59_83 "HandleSysMsg MSG_TP_PRESS_UP point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_SOGOU_1338_112_2_18_2_26_59_84 "HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_UP point.x = %d, point.y = %d"
#define MMIIM_TOUCH_APP_SOGOU_4257_112_2_18_2_27_5_85 "IsInputting"
#define MMIIM_TOUCH_COMMON_1302_112_2_18_2_27_8_86 "[mmi_touch.c]HandleSettingWinMsg(), data lost..."
#define MMIIM_TOUCH_COMMON_2267_112_2_18_2_27_10_87 "DrawLangSwitchKey"
#define MMIIM_TOUCH_COMMON_4439_112_2_18_2_27_14_88 "VkbUp cao touchpanel"
#define MMIIM_TOUCH_COMMON_4729_112_2_18_2_27_15_89 "VkeySetDown"
#define MMIIM_TOUCH_COMMON_4918_112_2_18_2_27_15_90 "PanelSlideCueClose"
#define MMIIM_TOUCH_COMMON_4932_112_2_18_2_27_15_91 "PanelSlideCueClose"
#define MMIIM_TOUCH_COMMON_5231_112_2_18_2_27_16_92 "RefreshTipLetterSelectIndex"
#define MMIIM_TOUCH_COMMON_5571_112_2_18_2_27_16_93 "MMIIM_OpenMethodSettingMenu  data_ptr->init_param.method_param_ptr"
#define MMIIM_TOUCH_COMMON_5627_112_2_18_2_27_16_94 "MMIIM_ConfigSettingMenu"
#define MMIIM_TOUCH_COMMON_5657_112_2_18_2_27_17_95 "MMIIM_CreateSettingMenuLayer"
#define MMIIM_TOUCH_COMMON_5973_112_2_18_2_27_17_96 "MMIIM_MethodSettingMenuUp"
#define MMIIM_TOUCH_COMMON_6063_112_2_18_2_27_17_97 "MMIIM_VkeyUpSet"
#define MMIIM_TOUCH_IME_COMMON_2108_112_2_18_2_27_25_98 "MultiTapCommitCB"
#define MMIIM_TOUCH_IME_COMMON_2114_112_2_18_2_27_25_99 "MultiTapCommitCB"
#define MMIIM_TOUCH_IME_COMMON_2120_112_2_18_2_27_25_100 "MultiTapCommitCB"
#define MMIIM_TOUCH_IME_COMMON_2274_112_2_18_2_27_25_101 "ClearMultitapTimer"
#define MMIIM_TOUCH_IME_COMMON_2859_112_2_18_2_27_26_102 "ThumbSmartRedefineKey0 method == %x"
#define MMIIM_TOUCH_IME_COMMON_2926_112_2_18_2_27_26_103 "ThumbSmartRedefineKey1 method == %d"
#define MMIIM_TOUCH_IME_COMMON_2966_112_2_18_2_27_27_104 "ThumbSmartRedefineCaps method == %d"
#define MMIIM_TOUCH_IME_COMMON_3032_112_2_18_2_27_27_105 "ThumbMultitapRedefineKey0 method == %x"
#define MMIIM_TOUCH_IME_COMMON_3107_112_2_18_2_27_27_106 "ThumbMultitapRedefineCaps method == %d"
#define MMIIM_TOUCH_IME_COMMON_3529_112_2_18_2_27_28_107 "QwertSmartRedefineCaps method == %d"
#define MMIIM_TOUCH_IME_COMMON_3573_112_2_18_2_27_28_108 "QwertMultitapRedefineCaps method == %d"
#define MMIIM_TOUCH_IME_COMMON_3637_112_2_18_2_27_28_109 "MultiTapCharKeyInit"
#define MMIIM_TOUCH_IME_CSTAR_9124_112_2_18_2_27_47_110 "SmartInitEngine"
#define MMIIM_TOUCH_IME_CSTAR_9133_112_2_18_2_27_47_111 "SmartInitEngine"
#define MMIIM_TOUCH_IME_CSTAR_9166_112_2_18_2_27_47_112 "ThumbSmartInit"
#define MMIIM_TOUCH_IME_CSTAR_10339_112_2_18_2_27_50_113 "Cstar_SetCapsLock"
#define MMIIM_TOUCH_IME_CSTAR_10782_112_2_18_2_27_51_114 "ThumbAmharicCharLong"
#define MMIIM_TOUCH_IME_CSTAR_11459_112_2_18_2_27_52_115 "kmx_amharic_digit_match"
#define MMIIM_TOUCH_IME_CSTAR_11466_112_2_18_2_27_52_116 "kmx_amharic_digit_match"
#define MMIIM_TOUCH_IME_CSTAR_11476_112_2_18_2_27_52_117 "kmx_amharic_digit_match"
#define MMIIM_TOUCH_IME_CSTAR_11508_112_2_18_2_27_52_118 "kmx_amharic_unicode_char_match"
#define MMIIM_TOUCH_IME_HW_1012_112_2_18_2_27_55_119 "HwFullScreenGetPanelRect"
#define MMIIM_TOUCH_IME_HW_1167_112_2_18_2_27_55_120 "HwFullScreenPanelKeyMove"
#define MMIIM_TOUCH_IME_HW_1211_112_2_18_2_27_55_121 "HwFullScreenPanelKeyMove EDIT_STATE_WRITE"
#define MMIIM_TOUCH_IME_HW_1231_112_2_18_2_27_55_122 "HwFullScreenPanelKeyMove LCD_PrepareDrawParam"
#define MMIIM_TOUCH_IME_HW_1342_112_2_18_2_27_55_123 "HwFullScreenPanelKeyUp"
#define MMIIM_TOUCH_IME_HW_1381_112_2_18_2_27_55_124 "HwFullScreenPanelKeyLong"
#define MMIIM_TOUCH_IME_HW_1556_112_2_18_2_27_56_125 "HwFullScreenWinMove EDIT_STATE_WRITE"
#define MMIIM_TOUCH_IME_HW_1576_112_2_18_2_27_56_126 "HwFullScreenWinMove LCD_PrepareDrawParam"
#define MMIIM_TOUCH_IME_HW_1635_112_2_18_2_27_56_127 "HwFullScreenWinUp"
#define MMIIM_TOUCH_IME_HW_1651_112_2_18_2_27_56_128 "VkbUp touchpanel"
#define MMIIM_TOUCH_IME_HW_1664_112_2_18_2_27_56_129 "HwFullScreenWinUp"
#define MMIIM_TOUCH_IME_HW_1708_112_2_18_2_27_56_130 "HWFullScreenHandleTpMsg"
#define MMIIM_TOUCH_IME_HW_2345_112_2_18_2_27_57_131 "HwFullScreenCharUp key_ptr"
#define MMIIM_TOUCH_IME_HW_2387_112_2_18_2_27_58_132 "HwFullScreenCtrlUp key_ptr"
#define MMIIM_TOUCH_IME_HW_2446_112_2_18_2_27_58_133 "HwFullScreenCtrlUp VKEY_PAGEUP"
#define MMIIM_TOUCH_IME_HW_2462_112_2_18_2_27_58_134 "HwFullScreenCtrlUp VKEY_PAGEDOWN"
#define MMIIM_TOUCH_IME_HW_2493_112_2_18_2_27_58_135 "HwFullScreenSysUp key_ptr"
#define MMIIM_TOUCH_IME_HW_2533_112_2_18_2_27_58_136 "HwFullScreenCandUp key_ptr"
#define MMIIM_TOUCH_IME_HW_2541_112_2_18_2_27_58_137 "HwFullScreenCandUp select_idx"
#define MMIIM_TOUCH_IME_HW_2598_112_2_18_2_27_58_138 "HwFullScreenRecognizeCB"
#define MMIIM_TOUCH_IME_HW_2793_112_2_18_2_27_58_139 "HWFullScreenPreProc_HandleTpMsg"
#define MMIIM_TOUCH_IME_HW_2915_112_2_18_2_27_59_140 "HwInitEngineAttr"
#define MMIIM_TOUCH_IME_HW_2922_112_2_18_2_27_59_141 "HwHanWangRecognize"
#define MMIIM_TOUCH_IME_HW_2929_112_2_18_2_27_59_142 "HwHanWangRecognize"
#define MMIIM_TOUCH_IME_HW_2941_112_2_18_2_27_59_143 "HwInitEngineAttr"
#define MMIIM_TOUCH_IME_HW_3248_112_2_18_2_27_59_144 "HwHanWangRecognize"
#define MMIIM_TOUCH_IME_HW_3255_112_2_18_2_27_59_145 "HwHanWangRecognize"
#define MMIIM_TOUCH_IME_HW_3262_112_2_18_2_27_59_146 "HwHanWangRecognize"
#define MMIIM_TOUCH_IME_HW_3779_112_2_18_2_28_0_147 "Sogou_HwRecognize"
#define MMIIM_TOUCH_IME_HW_3782_112_2_18_2_28_0_148 "mmiim_touch_sogou.c Sogou_HwRecognize lang = %d"
#define MMIIM_TOUCH_IME_HW_4078_112_2_18_2_28_1_149 "Sogou_CandToImeResult"
#define MMIIM_TOUCH_IME_SOGOU_61_112_2_18_2_28_2_150 "Sogou_TestCandNextPage"
#define MMIIM_TOUCH_IME_SOGOU_386_112_2_18_2_28_2_151 "Sogou_DrawVkbSkin"
#define MMIIM_TOUCH_IME_SOGOU_407_112_2_18_2_28_2_152 "Sogou_DrawVkbSkin"
#define MMIIM_TOUCH_IME_SOGOU_426_112_2_18_2_28_2_153 "Sogou_DrawVkbSkin"
#define MMIIM_TOUCH_IME_SOGOU_488_112_2_18_2_28_2_154 "ThumbLatinDispEdit"
#define MMIIM_TOUCH_IME_SOGOU_589_112_2_18_2_28_3_155 "MMIIM_TOUCH_Sogou_GetImeResult"
#define MMIIM_TOUCH_IME_SOGOU_646_112_2_18_2_28_3_156 "Sogou_SetLastDisp"
#define MMIIM_TOUCH_IME_SOGOU_684_112_2_18_2_28_3_157 "Sogou_GetLastDisp"
#define MMIIM_TOUCH_IME_SOGOU_724_112_2_18_2_28_3_158 "Sogou_SetCapsLock"
#define MMIIM_TOUCH_IME_SOGOU_730_112_2_18_2_28_3_159 "Sogou_SetCapsLock"
#define MMIIM_TOUCH_IME_SOGOU_778_112_2_18_2_28_3_160 "Sogou_AssociateTo2DemensionArray"
#define MMIIM_TOUCH_IME_SOGOU_1233_112_2_18_2_28_4_161 "Sogou_ImeInit"
#define MMIIM_TOUCH_IME_SOGOU_1240_112_2_18_2_28_4_162 "Sogou_ImeInit"
#define MMIIM_TOUCH_IME_SOGOU_1269_112_2_18_2_28_4_163 "ThumbPinyinInit Sogou_ImeInit"
#define MMIIM_TOUCH_IME_SOGOU_1712_112_2_18_2_28_5_164 "ThumbPinyinCharUp"
#define MMIIM_TOUCH_IME_SOGOU_1792_112_2_18_2_28_5_165 "ThumbPinyinCtrlLong"
#define MMIIM_TOUCH_IME_SOGOU_1844_112_2_18_2_28_5_166 "ThumbPinyinCtrlUp"
#define MMIIM_TOUCH_IME_SOGOU_2093_112_2_18_2_28_6_167 "ThumbPinyinSetLetterCand"
#define MMIIM_TOUCH_IME_SOGOU_2177_112_2_18_2_28_6_168 "ThumbPinyinCandUp"
#define MMIIM_TOUCH_IME_SOGOU_2280_112_2_18_2_28_6_169 "ThumbPinyinSetCand"
#define MMIIM_TOUCH_IME_SOGOU_2382_112_2_18_2_28_6_170 "ThumbPinyinDispEdit"
#define MMIIM_TOUCH_IME_SOGOU_2480_112_2_18_2_28_7_171 "ThumbPinyinSetCust"
#define MMIIM_TOUCH_IME_SOGOU_2553_112_2_18_2_28_7_172 "QwertPinyinInit Sogou_ImeInit"
#define MMIIM_TOUCH_IME_SOGOU_2862_112_2_18_2_28_7_173 "ThumbStrokeCharUp"
#define MMIIM_TOUCH_IME_SOGOU_2916_112_2_18_2_28_8_174 "ThumbStrokeCtrlLong"
#define MMIIM_TOUCH_IME_SOGOU_2968_112_2_18_2_28_8_175 "ThumbStrokeCtrlUp"
#define MMIIM_TOUCH_IME_SOGOU_3129_112_2_18_2_28_8_176 "ThumbStrokeCandUp"
#define MMIIM_TOUCH_IME_SOGOU_3221_112_2_18_2_28_8_177 "ThumbStrokeSetCand"
#define MMIIM_TOUCH_IME_SOGOU_3317_112_2_18_2_28_8_178 "ThumbStrokeDispEdit"
#define MMIIM_TOUCH_IME_SOGOU_3465_112_2_18_2_28_9_179 "QwertPinyinInit Sogou_ImeInit"
#define MMIIM_TOUCH_IME_SOGOU_3817_112_2_18_2_28_9_180 "QwertPinyinCtrlLong"
#define MMIIM_TOUCH_IME_SOGOU_3869_112_2_18_2_28_9_181 "QwertPinyinCtrlUp"
#define MMIIM_TOUCH_IME_SOGOU_4017_112_2_18_2_28_10_182 "QwertPinyinCandUp"
#define MMIIM_TOUCH_IME_SOGOU_4129_112_2_18_2_28_10_183 "QwertPinyinSetCand"
#define MMIIM_TOUCH_IME_SOGOU_4566_112_2_18_2_28_11_184 "ThumbSmartInit Sogou_ImeInit"
#define MMIIM_TOUCH_IME_SOGOU_4666_112_2_18_2_28_11_185 "ThumbSmartCharLong"
#define MMIIM_TOUCH_IME_SOGOU_4768_112_2_18_2_28_11_186 "ThumbSmartCtrlLong"
#define MMIIM_TOUCH_IME_SOGOU_4835_112_2_18_2_28_11_187 "ThumbSmartCtrlUp"
#define MMIIM_TOUCH_IME_SOGOU_5036_112_2_18_2_28_12_188 "ThumbSmartCandUp"
#define MMIIM_TOUCH_IME_SOGOU_5149_112_2_18_2_28_12_189 "SmartSetCand"
#define MMIIM_TOUCH_IME_SOGOU_5294_112_2_18_2_28_12_190 "ThumbSmartDispEdit"
#define MMIIM_TOUCH_IME_SOGOU_5403_112_2_18_2_28_12_191 "QwertPinyinInit Sogou_ImeInit"
#define MMIIM_TOUCH_IME_SOGOU_5830_112_2_18_2_28_13_192 "QwertSmartCtrlLong"
#define MMIIM_TOUCH_IME_SOGOU_5883_112_2_18_2_28_13_193 "QwertSmartCtrlUp"
#define MMIIM_TOUCH_IME_SOGOU_6031_112_2_18_2_28_14_194 "QwertSmartCandUp"
#define MMIIM_TOUCH_SET_133_112_2_18_2_28_14_195 "mmiim_touch_set.c: MMIIM_CreateSettingIMLayer setting_im_layer is already created"
#define MMIIM_TOUCH_SET_212_112_2_18_2_28_14_196 "mmiim_touch_set.c: MMIIM_CreateSettingIMLayer data_ptr->settingim_menu_index malloc failed"
#define MMIIM_TOUCH_SET_650_112_2_18_2_28_15_197 "mmiim_touch_set.c: MMIIM_HandleSettingIMMove---point not in heightligth rect---data_ptr->settingim_top_item %d,point.y %d highlight top %d bottom %d"
#define MMIIM_TOUCH_SET_812_112_2_18_2_28_16_198 "mmiim_touch_set.c: MMIIM_HandleSettingIMUp point is not in the expected rect"
#define MMIIM_TOUCH_SET_1000_112_2_18_2_28_16_199 "mmiim_touch_set.c: DrawSettingIm settingim_halfposition %d,highlight_index %d,data_ptr->settingim_top_item %d,data_ptr->settingim_last_highlight %d"
#define MMIIM_TP_CSTAR_616_112_2_18_2_28_18_200 "[MMIIM] Initialize tpcstar: win_id=%X"
#define MMIIM_TP_CSTAR_713_112_2_18_2_28_18_201 "[MMIIM] Terminate tpcstar: data_ptr is null."
#define MMIIM_TP_CSTAR_717_112_2_18_2_28_18_202 "[MMIIM] Terminate tpcstar: cs_handle=%08X"
#define MMIIM_TP_CSTAR_739_112_2_18_2_28_18_203 "[MMIIM] Terminate tpcstar: stop timer id=%d"
#define MMIIM_TP_CSTAR_747_112_2_18_2_28_18_204 "[MMIIM] Terminate tpcstar: release layer."
#define MMIIM_TP_CSTAR_777_112_2_18_2_28_18_205 "[MMIIM] HandleMsg: data_ptr is null."
#define MMIIM_TP_CSTAR_782_112_2_18_2_28_18_206 "[MMIIM] HandleMsg:event:%d,stateId:%d,stateId_bak:%d,stateId_bakii:%d"
#define MMIIM_TP_CSTAR_904_112_2_18_2_28_18_207 "[MMIIM] HandleSysMsg: param is null."
#define MMIIM_TP_CSTAR_968_112_2_18_2_28_18_208 "[MMIIM] HandleSysMsg: IsImReloadDirty, method=%04X, stateId=%d"
#define MMIIM_TP_CSTAR_1068_112_2_18_2_28_19_209 "[MMIIM] TransMsg: param is null."
#define MMIIM_TP_CSTAR_1799_112_2_18_2_28_20_210 "[MMIIM] TpUpNoInputProcess: param is null."
#define MMIIM_TP_CSTAR_1872_112_2_18_2_28_20_211 "[MMIIM] TpUpInputingProcess: param is null."
#define MMIIM_TP_CSTAR_1992_112_2_18_2_28_20_212 "[MMIIM] TpUpAssociateProcess: param is null."
#define MMIIM_TP_CSTAR_2084_112_2_18_2_28_21_213 "[MMIIM] TpUpUserWordProcess: param is null."
#define MMIIM_TP_CSTAR_2209_112_2_18_2_28_21_214 "[MMIIM] TpLongProcess: param is null."
#define MMIIM_TP_CSTAR_2305_112_2_18_2_28_21_215 "[MMIIM] TpCandPressUp, p.x=%d, p.y=%d"
#define MMIIM_TP_CSTAR_2311_112_2_18_2_28_21_216 "[MMIIM] TpCandPressUp: param is null."
#define MMIIM_TP_CSTAR_2600_112_2_18_2_28_22_217 "[MMIIM] HandleChoicePageSimulator: param is null."
#define MMIIM_TP_CSTAR_2821_112_2_18_2_28_22_218 "[MMIIM] ConvertMethodtoState cs_im =%d"
#define MMIIM_TP_CSTAR_3004_112_2_18_2_28_22_219 "[MMIIM] HandleCstarEvent"
#define MMIIM_TP_CSTAR_3008_112_2_18_2_28_22_220 "[MMIIM] HandleCstarEvent: handle_ptr is null."
#define MMIIM_TP_CSTAR_3136_112_2_18_2_28_23_221 "[MMIIM] HandleReturn"
#define MMIIM_TP_CSTAR_3141_112_2_18_2_28_23_222 "[MMIIM] HandleReturn: param is null."
#define MMIIM_TP_CSTAR_3169_112_2_18_2_28_23_223 "[MMIIM] HandleSwicth"
#define MMIIM_TP_CSTAR_3174_112_2_18_2_28_23_224 "[MMIIM] HandleSwicth: param is null."
#define MMIIM_TP_CSTAR_3224_112_2_18_2_28_23_225 "[MMIIM] HandleChar"
#define MMIIM_TP_CSTAR_3230_112_2_18_2_28_23_226 "[MMIIM] HandleChar: param is null."
#define MMIIM_TP_CSTAR_3319_112_2_18_2_28_23_227 "[MMIIM] Handle26Key"
#define MMIIM_TP_CSTAR_3325_112_2_18_2_28_23_228 "[MMIIM] HandleChar: param is null."
#define MMIIM_TP_CSTAR_3381_112_2_18_2_28_23_229 "[MMIIM] Handle9Key"
#define MMIIM_TP_CSTAR_3387_112_2_18_2_28_23_230 "[MMIIM] Handle9Key: param is null."
#define MMIIM_TP_CSTAR_3485_112_2_18_2_28_23_231 "[MMIIM] HandleMethodSelect"
#define MMIIM_TP_CSTAR_3490_112_2_18_2_28_23_232 "[MMIIM] HandleMethodSelect: param is null."
#define MMIIM_TP_CSTAR_3516_112_2_18_2_28_23_233 "[MMIIM] HandleCaps"
#define MMIIM_TP_CSTAR_3522_112_2_18_2_28_23_234 "[MMIIM] HandleCaps: param is null."
#define MMIIM_TP_CSTAR_3540_112_2_18_2_28_24_235 "[MMIIM] HandleCaps: method=%04X, new_method=%04X."
#define MMIIM_TP_CSTAR_3565_112_2_18_2_28_24_236 "[MMIIM] Handle26KeyTo9Key"
#define MMIIM_TP_CSTAR_3569_112_2_18_2_28_24_237 "[MMIIM] Handle26KeyTo9Key: param is null."
#define MMIIM_TP_CSTAR_3600_112_2_18_2_28_24_238 "[MMIIM] Handle26KeyTo9Key: method=%04X, new_method=%04X."
#define MMIIM_TP_CSTAR_3625_112_2_18_2_28_24_239 "[MMIIM] Handle9KeyTo26Key"
#define MMIIM_TP_CSTAR_3629_112_2_18_2_28_24_240 "[MMIIM] Handle9KeyTo26Key: param is null."
#define MMIIM_TP_CSTAR_3654_112_2_18_2_28_24_241 "[MMIIM] Handle9KeyTo26Key: method=%04X, new_method=%04X."
#define MMIIM_TP_CSTAR_3675_112_2_18_2_28_24_242 "[MMIIM] HandleLock"
#define MMIIM_TP_CSTAR_3680_112_2_18_2_28_24_243 "[MMIIM] HandleLock: param is null."
#define MMIIM_TP_CSTAR_3728_112_2_18_2_28_24_244 "[MMIIM] HandleSwicthSymbol: param is null."
#define MMIIM_TP_SOGOU_2038_112_2_18_2_28_52_245 "TpCandPressUp iLoop=%d,font=%d,label_width=%d"
#define MMIIM_TP_SOGOU_2395_112_2_18_2_28_53_246 "mmiim_tp_sogou.c: HandleSogouEvent: HandleImeOnKeyEvent return ERROR_INPUT_KEYS!"
#define MMIIM_TP_SOGOU_2476_112_2_18_2_28_53_247 "HandleReturn"
#define MMIIM_TP_SOGOU_2507_112_2_18_2_28_53_248 "HandleSwicth"
#define MMIIM_TP_SOGOU_2558_112_2_18_2_28_53_249 "HandleChar"
#define MMIIM_TP_SOGOU_2656_112_2_18_2_28_53_250 "Handle26Key"
#define MMIIM_TP_SOGOU_2745_112_2_18_2_28_53_251 "Handle9Key"
#define MMIIM_TP_SOGOU_2827_112_2_18_2_28_54_252 "HandleMethodSelect"
#define MMIIM_TP_SOGOU_2858_112_2_18_2_28_54_253 "HandleCaps"
#define MMIIM_TP_SOGOU_2905_112_2_18_2_28_54_254 "Handle26KeyTo9Key"
#define MMIIM_TP_SOGOU_2963_112_2_18_2_28_54_255 "Handle9KeyTo26Key"
#define MMIIM_TP_SOGOU_3011_112_2_18_2_28_54_256 "HandleLock"
#define MMIIM_TP_SP_307_112_2_18_2_29_10_257 "Initialize"
#define MMIIM_TP_SP_365_112_2_18_2_29_11_258 "Terminate"
#define MMIIM_TP_SP_414_112_2_18_2_29_11_259 "HandleMsg:event:%d,stateId:%d,stateId_bak:%d,stateId_bakii:%d"
#define MMIIM_TP_SP_1531_112_2_18_2_29_13_260 "TpCandPressUp"
#define MMIIM_TP_SP_1957_112_2_18_2_29_14_261 "HandleSPEvent"
#define MMIIM_TP_SP_2097_112_2_18_2_29_14_262 "HandleReturn"
#define MMIIM_TP_SP_2124_112_2_18_2_29_14_263 "HandleSwicth"
#define MMIIM_TP_SP_2178_112_2_18_2_29_14_264 "HandleChar"
#define MMIIM_TP_SP_2257_112_2_18_2_29_14_265 "Handle26Key"
#define MMIIM_TP_SP_2321_112_2_18_2_29_14_266 "Handle9Key"
#define MMIIM_TP_SP_2390_112_2_18_2_29_15_267 "HandleMethodSelect"
#define MMIIM_TP_SP_2421_112_2_18_2_29_15_268 "HandleCaps"
#define MMIIM_TP_SP_2469_112_2_18_2_29_15_269 "Handle26KeyTo9Key"
#define MMIIM_TP_SP_2523_112_2_18_2_29_15_270 "Handle9KeyTo26Key"
#define MMIIM_TP_SP_2566_112_2_18_2_29_15_271 "HandleLock"
#define MMIIM_TP_SP_2949_112_2_18_2_29_16_272 "buffer[%d]= 0x%x"
#define MMIIM_TP_UI_1333_112_2_18_2_29_19_273 "[MMIIM] DrawAllBoard:%d"
#define MMIIM_TP_UI_2224_112_2_18_2_29_21_274 "[MMIIM] MMIIMUI_DrawKeyBoard"
#define MMIIM_TP_UI_2298_112_2_18_2_29_21_275 "[MMIIM] MMIIMUI_DrawReplaceKey"
#define MMIIM_TP_UI_2442_112_2_18_2_29_21_276 "[MMIIM] MMIIMUI_DrawEditBar"
#define MMIIM_TP_UI_2705_112_2_18_2_29_22_277 "[MMIIM] TpCandPressDown"
#define MMIIM_TP_UI_2935_112_2_18_2_29_22_278 "[MMIIM] MMIIMUI_DrawCandBar"
#define MMIIM_TP_UI_3579_112_2_18_2_29_23_279 "[MMIIM] MMIIMUI_ChangeRect: data_ptr is null."
#define MMIIM_TP_UI_3737_112_2_18_2_29_24_280 "[MMIIM] MMIIMUI_IsCommonKeyBoardState: handle_ptr is null."
#define MMIIM_TP_UI_3773_112_2_18_2_29_24_281 "[MMIIM] IsDisplayTips: handle_ptr is null."
#define MMIIM_TP_UI_3812_112_2_18_2_29_24_282 "[MMIIM] GetReplaceKeyImgID"
#define MMIIM_TP_UI_4083_112_2_18_2_29_24_283 "[MMIIM] GetTipsRect: handle_ptr is null."
#define MMIIM_TP_UI_4584_112_2_18_2_29_25_284 "[MMIIM] GetVKboardState"
#define MMIIM_TP_UI_4791_112_2_18_2_29_26_285 "[MMIIM] MMIIMUI_TransMsg: p.x=%d, p.y=%d, x=%d, y=%d"
#define MMIIM_TP_UI_4946_112_2_18_2_29_26_286 "[MMIIM] MMIIMUI_SearchFromStrtab: param is null."
#define MMIIM_TP_UI_4975_112_2_18_2_29_26_287 "[MMIIM] MMIIMUI_ClearKbUpOnInValidRect: data_ptr is null."
#define MMIIM_WINTAB_739_112_2_18_2_29_28_288 "SetImPyFuzzy:selected_item[%d] %d is error!"
#define MMIIM_WINTAB_823_112_2_18_2_29_28_289 "AppendImHwSpeedItem:hw_speed %d is error!"
#define MMIIM_WINTAB_863_112_2_18_2_29_28_290 "SetImHwSpeed:cur_item %d is error!"
#define MMIIM_WINTAB_946_112_2_18_2_29_28_291 "AppendImHwThicknessItem:hw_thickness %d is error!"
#define MMIIM_WINTAB_986_112_2_18_2_29_28_292 "SetImHwThickness:cur_item %d is error!"
#define MMIIM_WINTAB_1264_112_2_18_2_29_29_293 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
#define MMIIM_WINTAB_1471_112_2_18_2_29_29_294 "HandleHwColorPenOkMsg: button_idx_ptr is null."
#define MMIIM_WINTAB_1574_112_2_18_2_29_30_295 "AppendImHwSpeedItem: hw_speed %d is error!"
#define MMIIM_WINTAB_1625_112_2_18_2_29_30_296 "AppendImHwThicknessItem: hw_thickness %d is error!"
#define MMIIM_WINTAB_1687_112_2_18_2_29_30_297 "AppendImHwThicknessItem: hw_thickness %d is error!"
#define MMIIM_WINTAB_1726_112_2_18_2_29_30_298 "SetImKeyboardSize:cur_item %d is error!"
#define WDP_SG_INTERFACE_NET_70_112_2_18_2_29_30_299 "wdp_SG_Interface_net.c SG_soc_setopt: error!"
#define WDP_SG_INTERFACE_NET_107_112_2_18_2_29_31_300 "wdp_sg_interface_net.c SG_soc_create socket id = %d"
#define WDP_SG_INTERFACE_NET_202_112_2_18_2_29_31_301 "SG_soc_send sci_sock_send err  conn_err=%d"
#define WDP_SG_INTERFACE_NET_232_112_2_18_2_29_31_302 "SG_soc_recv recvSocketInput->s=%d,receive_len=%d"
#define WDP_SG_INTERFACE_NET_267_112_2_18_2_29_31_303 "Sogou Core trace: %s"
#define WDP_SG_INTERFACE_NET_593_112_2_18_2_29_32_304 "SG_Assert : assertion failed!"
#define WDP_UPDATE_194_112_2_18_2_29_32_305 "sg_read_skinbinfile sfs_err=%d"
#define WDP_UPDATE_212_112_2_18_2_29_32_306 "wdp_update.c UpdateInit: begin."
#define WDP_UPDATE_236_112_2_18_2_29_32_307 "wdp_update.c SG_UpdateSetNetWork s_is_resolving_address = %d"
#define WDP_UPDATE_325_112_2_18_2_29_33_308 "wdp_update.c MMIAPIIM_SGUpdateCallBackFunc: s_update_status = %d ,s_is_set_network = %d"
#define WDP_UPDATE_512_112_2_18_2_29_33_309 "SG_UPDATE_TIMEROUT sg_getDataOutput.errorCode=%d,sg_getDataOutput.httpState=%d"
#define WDP_UPDATE_515_112_2_18_2_29_33_310 "SG_UPDATE_TIMEROUT SG_HTTP_OK"
#define WDP_UPDATE_519_112_2_18_2_29_33_311 "SG_UPDATE_TIMEROUT SG_HTTP_STATE_IDLE"
#define WDP_UPDATE_555_112_2_18_2_29_33_312 "SG_UPDATE_TIMEROUT error"
#define WDP_UPDATE_679_112_2_18_2_29_33_313 "SG_Get_IMEI MNNV_GetIMEISVNEx Error!"
#define WDP_UPDATE_695_112_2_18_2_29_33_314 "HandleSGPsMsg msg_ptr->msg_id=0x%04x, msg_ptr->result=0x%04x"
#define WDP_UPDATE_702_112_2_18_2_29_33_315 "HandleSGPsMsg ProcessGprsActiveCnf succ"
#define WDP_UPDATE_732_112_2_18_2_29_33_316 "HandleSGPsMsg g_sg_gprs_retry_time = %d"
#define WDP_UPDATE_767_112_2_18_2_29_33_317 "wdp_update.c MMIAPIIM_SGGetSkinServerAdd: resolve successful"
#define WDP_UPDATE_783_112_2_18_2_29_34_318 "wdp_update.c MMIAPIIM_SGGetSkinServerAdd: timeout"
#define WDP_UPDATE_795_112_2_18_2_29_34_319 "wdp_update.c MMIAPIIM_SGGetSkinServerAdd: retry times = %d"
#define WDP_UPDATE_WINTAB_786_112_2_18_2_29_36_320 "HandleInputSettingsMenuWindow 0x%04x"
#define WDP_UPDATE_WINTAB_1248_112_2_18_2_29_37_321 "HandleSkinSelectMenuWindow win_id=0x%04x,msg_id=0x%04x"
#define WDP_UPDATE_WINTAB_2432_112_2_18_2_29_39_322 "HandleSkinBrowseWindow win_id=0x%04x,msg_id=0x%04x"
#define WDP_UPDATE_WINTAB_2437_112_2_18_2_29_39_323 "HandleSkinBrowseWindow ERR s_skin_ptr"
#define WDP_UPDATE_WINTAB_3198_112_2_18_2_29_41_324 "HandleAutomaticUpdateSettingsListWindow msg_id = 0x%04x"
#define WDP_UPDATE_WINTAB_3936_112_2_18_2_29_42_325 "SOGOU_AutoUpdateWordRemind g_sogou_update_settings_data.is_need_connect_remind=%d"
#define WDP_UPDATE_WINTAB_3968_112_2_18_2_29_42_326 "SOGOU_AutoUpdateWordRemind g_sogou_update_settings_data.is_need_update_notif=%d"
#define WDP_UPDATE_WINTAB_4128_112_2_18_2_29_43_327 "wdp_update_wintab.c OpenUpdateWindow: over wifi!"
#define WDP_UPDATE_WINTAB_4133_112_2_18_2_29_43_328 "wdp_update_wintab.c OpenUpdateWindow: over sim, status = SG_IM_UPD_S_CREATING_SOCKET"
#define WDP_UPDATE_WINTAB_4138_112_2_18_2_29_43_329 "wdp_update_wintab.c OpenUpdateWindow: over sim, status = SG_IM_UPD_S_START"
#define WDP_UPDATE_WINTAB_4358_112_2_18_2_29_43_330 "HandleUpdateWatchingWindow MMIPUB_HandleWaitWinMsg begin win_id=0x%04x,msg_id=0x%04x"
#define WDP_UPDATE_WINTAB_4378_112_2_18_2_29_43_331 "HandleUpdateExitWin win_id=0x%04x,msg_id=0x%04x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_IM_TRC)
TRACE_MSG(MMIIM_419_112_2_18_2_24_46_0,"MMIAPIIM_InitNv read IMNV_SOGOU_UPDATE_SETTING error!")
TRACE_MSG(MMIIM_637_112_2_18_2_24_47_1,"MMIAPIIM_HWGetAreaType Err")
TRACE_MSG(MMIIM_658_112_2_18_2_24_47_2,"MMIAPIIM_HWSetAreaType Err")
TRACE_MSG(MMIIM_1335_112_2_18_2_24_48_3,"InitMlData: im_ml_data_ptr is null.")
TRACE_MSG(MMIIM_1360_112_2_18_2_24_48_4,"== SetMultiLangMethod == can not find method %d in multi-lang data")
TRACE_MSG(MMIIM_1393_112_2_18_2_24_48_5,"== InitMlData == can get lang index %d")
TRACE_MSG(MMIIM_1571_112_2_18_2_24_49_6,"MMIAPIIM_InitLangKeyTable")
TRACE_MSG(MMIIM_1582_112_2_18_2_24_49_7,"MMIAPIIM_InitLangKeyTable")
TRACE_MSG(MMIIM_1594_112_2_18_2_24_49_8,"MMIAPIIM_InitLangKeyTable")
TRACE_MSG(MMIIM_1605_112_2_18_2_24_49_9,"MMIAPIIM_InitLangKeyTable")
TRACE_MSG(MMIIM_1620_112_2_18_2_24_49_10,"MMIAPIIM_InitLangKeyTable")
TRACE_MSG(MMIIM_1631_112_2_18_2_24_49_11,"MMIAPIIM_InitLangKeyTable")
TRACE_MSG(MMIIM_1682_112_2_18_2_24_49_12,"MMIAPIIM_GetKeyIndexByKeytable ERR PARAM")
TRACE_MSG(MMIIM_1695_112_2_18_2_24_49_13,"MMIAPIIM_GetKeyIndexByKeytable")
TRACE_MSG(MMIIM_1720_112_2_18_2_24_49_14,"MMIAPIIM_GetKeyIndexByKeytable")
TRACE_MSG(MMIIM_1836_112_2_18_2_24_49_15,"MMIIM_GetWcharKeyValueByTable")
TRACE_MSG(MMIIM_1880_112_2_18_2_24_49_16,"MMIIM_GetWcharKeyValueByTable")
TRACE_MSG(MMIIM_1911_112_2_18_2_24_49_17,"MMIIM_GetWcharKeyValueByTable")
TRACE_MSG(MMIIM_BASE_371_112_2_18_2_24_50_18,"MMIIM_DrawSpace")
TRACE_MSG(MMIIM_BASE_1479_112_2_18_2_24_52_19,"MMIIM_QueryAsso: context_ptr is null")
TRACE_MSG(MMIIM_BASE_1484_112_2_18_2_24_52_20,"MMIIM_QueryAsso: asso_ptr is null")
TRACE_MSG(MMIIM_BASE_1489_112_2_18_2_24_52_21,"MMIIM_QueryAsso: asso_len_ptr is null")
TRACE_MSG(MMIIM_CSTAR_1125_112_2_18_2_24_58_22,"[MMIIM] Initialize cstar: win_id=%X")
TRACE_MSG(MMIIM_CSTAR_1165_112_2_18_2_24_58_23,"[MMIIM] Terminate: data_ptr is null.")
TRACE_MSG(MMIIM_CSTAR_1169_112_2_18_2_24_58_24,"[MMIIM] Terminate cstar: cs_handle=%08X")
TRACE_MSG(MMIIM_CSTAR_1209_112_2_18_2_24_59_25,"[MMIIM] HandleMsg: data_ptr is null.")
TRACE_MSG(MMIIM_CSTAR_1383_112_2_18_2_24_59_26,"[MMIIM] HandleTpKeySelect: data_ptr is null.")
TRACE_MSG(MMIIM_CSTAR_1392_112_2_18_2_24_59_27,"[MMIIM] HandleTpKeySelect PNULL == content!")
TRACE_MSG(MMIIM_CSTAR_1448_112_2_18_2_24_59_28,"[MMIIM] HandleSysMsg: data_ptr is null.")
TRACE_MSG(MMIIM_CSTAR_1471_112_2_18_2_24_59_29,"[MMIIM] HandleSysMsg: IsImReloadDirty, method=%04X")
TRACE_MSG(MMIIM_CSTAR_1875_112_2_18_2_25_0_30,"[MMIIM] SetMethod: handle is null.")
TRACE_MSG(MMIIM_CSTAR_1886_112_2_18_2_25_0_31,"[MMIIM] SetMethod: cs_handle=%8X, cs_method=%04X, method=%04X")
TRACE_MSG(MMIIM_CSTAR_2122_112_2_18_2_25_1_32,"[MMIIM] ConvertKey PNULL")
TRACE_MSG(MMIIM_CSTAR_2464_112_2_18_2_25_1_33,"[MMIIM] ConvertKey: cs_vk=%d, cs_vk_type=%X, event=%d, msg=%X")
TRACE_MSG(MMIIM_IM_SWITCHING_2221_112_2_18_2_25_7_34,"[MMIIM] HandleSwitchingIMWinMsg: win_data_ptr is null.")
TRACE_MSG(MMIIM_IM_SWITCHING_2401_112_2_18_2_25_7_35,"[MMIIM] MMIIM_SetLangCreateWin: im_kernel_ptr is null.")
TRACE_MSG(MMIIM_IM_SWITCHING_2439_112_2_18_2_25_8_36,"[MMIIM] OnIMClicked: cur_selection >= win_data_ptr->item_counts.")
TRACE_MSG(MMIIM_IM_SWITCHING_2514_112_2_18_2_25_8_37,"[MMIIM] OnIMReleased: win_data_ptr is null.")
TRACE_MSG(MMIIM_IM_SWITCHING_2623_112_2_18_2_25_8_38,"[MMIIM] MMIIM_AppendIMListItem: win_data_ptr is null.")
TRACE_MSG(MMIIM_IM_SWITCHING_2665_112_2_18_2_25_8_39,"[MMIIM] SendChangeMethodMsg: method is GUIIM_M_NONE.")
TRACE_MSG(MMIIM_IM_SWITCHING_2672_112_2_18_2_25_8_40,"[MMIIM] SendChangeMethodMsg: method_cfg_ptr is null.")
TRACE_MSG(MMIIM_IM_SWITCHING_2720_112_2_18_2_25_8_41,"[MMIIM] SendChangeMethodMsg: method=%04X, new_method=%04X.")
TRACE_MSG(MMIIM_IM_SWITCHING_2752_112_2_18_2_25_8_42,"[MMIIM] InitIMDisplayValues: param is not valid.")
TRACE_MSG(MMIIM_IM_SWITCHING_4278_112_2_18_2_25_11_43,"[MMIIM] HandleSetInputLangWindow: win_data_ptr is null.")
TRACE_MSG(MMIIM_IM_SWITCHING_4290_112_2_18_2_25_11_44,"[MMIIM] HandleSetInputLangWindow: MSG_OPEN_WINDOW param is null.")
TRACE_MSG(MMIIM_IM_SWITCHING_4394_112_2_18_2_25_11_45,"[MMIIM] AppendLangItems: win_data_ptr is null.")
TRACE_MSG(MMIIM_IM_SWITCHING_4448_112_2_18_2_25_12_46,"[MMIIM] OnLangClicked: param is null.")
TRACE_MSG(MMIIM_SOGOU_1876_112_2_18_2_25_16_47,"[MMIIM] HandleMsg: data_ptr is null.")
TRACE_MSG(MMIIM_SOGOU_2611_112_2_18_2_25_18_48,"MMI: ConvertKey PNULL")
TRACE_MSG(MMIIM_SOGOU_2645_112_2_18_2_25_18_49,"mmiim_sogou.c ConvertKey: doesn't process MSG_KEYLONG_OK, return 1")
TRACE_MSG(MMIIM_SOGOU_3781_112_2_18_2_25_20_50,"MMIAPIIM_DrawSkinInRect Err param")
TRACE_MSG(MMIIM_SOGOU_3789_112_2_18_2_25_20_51,"MMIAPIIM_DrawSkinInRect Err disprect")
TRACE_MSG(MMIIM_SOGOU_3808_112_2_18_2_25_20_52,"MMIAPIIM_DrawSkinInRect")
TRACE_MSG(MMIIM_SOGOU_3853_112_2_18_2_25_20_53,"God, MMIAPIIM_DrawSkinInRect memory over")
TRACE_MSG(MMIIM_SP_HW_457_112_2_18_2_25_30_54,"mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!")
TRACE_MSG(MMIIM_SP_HW_466_112_2_18_2_25_30_55,"mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!")
TRACE_MSG(MMIIM_SP_HW_477_112_2_18_2_25_30_56,"mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!")
TRACE_MSG(MMIIM_SP_HW_483_112_2_18_2_25_30_57,"mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!")
TRACE_MSG(MMIIM_SP_HW_676_112_2_18_2_25_30_58,"[MMIIM] Initialize hw: win_id=%X")
TRACE_MSG(MMIIM_SP_HW_906_112_2_18_2_25_31_59,"[MMIIM] Terminate hw: data_ptr is null.")
TRACE_MSG(MMIIM_SP_HW_910_112_2_18_2_25_31_60,"[MMIIM] Terminate hw: win_id=%X")
TRACE_MSG(MMIIM_SP_HW_957_112_2_18_2_25_31_61,"[MMIIM] HandleMsg: data_ptr is null.")
TRACE_MSG(MMIIM_SP_HW_2444_112_2_18_2_25_34_62,"[MMIIM] RecognizeTrace log:")
TRACE_MSG(MMIIM_SP_HW_2445_112_2_18_2_25_34_63,"(")
TRACE_MSG(MMIIM_SP_HW_2448_112_2_18_2_25_34_64,"%d,%d,")
TRACE_MSG(MMIIM_SP_HW_2455_112_2_18_2_25_34_65,")")
TRACE_MSG(MMIIM_SP_HW_3099_112_2_18_2_25_35_66,"[MMIIM] CommitChar: data_ptr is null")
TRACE_MSG(MMIIM_SP_HW_3125_112_2_18_2_25_36_67,"[MMIIM] CommitChar: data_ptr is null")
TRACE_MSG(MMIIM_SP_HW_3151_112_2_18_2_25_36_68,"[MMIIM] CommitUnderlineChar: data_ptr is null")
TRACE_MSG(MMIIM_SP_HW_3207_112_2_18_2_25_36_69,"[MMIIM] LimitSet: data_ptr is null")
TRACE_MSG(MMIIM_SP_HW_3333_112_2_18_2_25_36_70,"[MMIIM] PopSymbPanel: data_ptr is null")
TRACE_MSG(MMIIM_SP_HW_3364_112_2_18_2_25_36_71,"[MMIIM] InitAttributeInfo: data_ptr is null")
TRACE_MSG(MMIIM_SP_TP_BASIC_1138_112_2_18_2_26_41_72,"MMIIM_SP_TP_KbDraw, kb display false.")
TRACE_MSG(MMIIM_TOUCH_APP_CSTAR_1326_112_2_18_2_26_46_73,"HandleSysMsg MSG_TP_PRESS_DOWN point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_CSTAR_1330_112_2_18_2_26_46_74,"HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_DOWN point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_CSTAR_1358_112_2_18_2_26_46_75,"HandleSysMsg MSG_TP_PRESS_MOVE point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_CSTAR_1362_112_2_18_2_26_46_76,"HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_MOVE point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_CSTAR_1429_112_2_18_2_26_46_77,"HandleSysMsg MSG_TP_PRESS_UP point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_CSTAR_1433_112_2_18_2_26_46_78,"HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_UP point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_SOGOU_1231_112_2_18_2_26_59_79,"HandleSysMsg MSG_TP_PRESS_DOWN point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_SOGOU_1235_112_2_18_2_26_59_80,"HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_DOWN point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_SOGOU_1263_112_2_18_2_26_59_81,"HandleSysMsg MSG_TP_PRESS_MOVE point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_SOGOU_1267_112_2_18_2_26_59_82,"HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_MOVE point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_SOGOU_1334_112_2_18_2_26_59_83,"HandleSysMsg MSG_TP_PRESS_UP point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_SOGOU_1338_112_2_18_2_26_59_84,"HWFullScreenPreProc_HandleTpMsg MSG_TP_PRESS_UP point.x = %d, point.y = %d")
TRACE_MSG(MMIIM_TOUCH_APP_SOGOU_4257_112_2_18_2_27_5_85,"IsInputting")
TRACE_MSG(MMIIM_TOUCH_COMMON_1302_112_2_18_2_27_8_86,"[mmi_touch.c]HandleSettingWinMsg(), data lost...")
TRACE_MSG(MMIIM_TOUCH_COMMON_2267_112_2_18_2_27_10_87,"DrawLangSwitchKey")
TRACE_MSG(MMIIM_TOUCH_COMMON_4439_112_2_18_2_27_14_88,"VkbUp cao touchpanel")
TRACE_MSG(MMIIM_TOUCH_COMMON_4729_112_2_18_2_27_15_89,"VkeySetDown")
TRACE_MSG(MMIIM_TOUCH_COMMON_4918_112_2_18_2_27_15_90,"PanelSlideCueClose")
TRACE_MSG(MMIIM_TOUCH_COMMON_4932_112_2_18_2_27_15_91,"PanelSlideCueClose")
TRACE_MSG(MMIIM_TOUCH_COMMON_5231_112_2_18_2_27_16_92,"RefreshTipLetterSelectIndex")
TRACE_MSG(MMIIM_TOUCH_COMMON_5571_112_2_18_2_27_16_93,"MMIIM_OpenMethodSettingMenu  data_ptr->init_param.method_param_ptr")
TRACE_MSG(MMIIM_TOUCH_COMMON_5627_112_2_18_2_27_16_94,"MMIIM_ConfigSettingMenu")
TRACE_MSG(MMIIM_TOUCH_COMMON_5657_112_2_18_2_27_17_95,"MMIIM_CreateSettingMenuLayer")
TRACE_MSG(MMIIM_TOUCH_COMMON_5973_112_2_18_2_27_17_96,"MMIIM_MethodSettingMenuUp")
TRACE_MSG(MMIIM_TOUCH_COMMON_6063_112_2_18_2_27_17_97,"MMIIM_VkeyUpSet")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_2108_112_2_18_2_27_25_98,"MultiTapCommitCB")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_2114_112_2_18_2_27_25_99,"MultiTapCommitCB")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_2120_112_2_18_2_27_25_100,"MultiTapCommitCB")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_2274_112_2_18_2_27_25_101,"ClearMultitapTimer")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_2859_112_2_18_2_27_26_102,"ThumbSmartRedefineKey0 method == %x")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_2926_112_2_18_2_27_26_103,"ThumbSmartRedefineKey1 method == %d")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_2966_112_2_18_2_27_27_104,"ThumbSmartRedefineCaps method == %d")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_3032_112_2_18_2_27_27_105,"ThumbMultitapRedefineKey0 method == %x")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_3107_112_2_18_2_27_27_106,"ThumbMultitapRedefineCaps method == %d")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_3529_112_2_18_2_27_28_107,"QwertSmartRedefineCaps method == %d")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_3573_112_2_18_2_27_28_108,"QwertMultitapRedefineCaps method == %d")
TRACE_MSG(MMIIM_TOUCH_IME_COMMON_3637_112_2_18_2_27_28_109,"MultiTapCharKeyInit")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_9124_112_2_18_2_27_47_110,"SmartInitEngine")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_9133_112_2_18_2_27_47_111,"SmartInitEngine")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_9166_112_2_18_2_27_47_112,"ThumbSmartInit")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_10339_112_2_18_2_27_50_113,"Cstar_SetCapsLock")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_10782_112_2_18_2_27_51_114,"ThumbAmharicCharLong")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_11459_112_2_18_2_27_52_115,"kmx_amharic_digit_match")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_11466_112_2_18_2_27_52_116,"kmx_amharic_digit_match")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_11476_112_2_18_2_27_52_117,"kmx_amharic_digit_match")
TRACE_MSG(MMIIM_TOUCH_IME_CSTAR_11508_112_2_18_2_27_52_118,"kmx_amharic_unicode_char_match")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1012_112_2_18_2_27_55_119,"HwFullScreenGetPanelRect")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1167_112_2_18_2_27_55_120,"HwFullScreenPanelKeyMove")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1211_112_2_18_2_27_55_121,"HwFullScreenPanelKeyMove EDIT_STATE_WRITE")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1231_112_2_18_2_27_55_122,"HwFullScreenPanelKeyMove LCD_PrepareDrawParam")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1342_112_2_18_2_27_55_123,"HwFullScreenPanelKeyUp")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1381_112_2_18_2_27_55_124,"HwFullScreenPanelKeyLong")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1556_112_2_18_2_27_56_125,"HwFullScreenWinMove EDIT_STATE_WRITE")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1576_112_2_18_2_27_56_126,"HwFullScreenWinMove LCD_PrepareDrawParam")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1635_112_2_18_2_27_56_127,"HwFullScreenWinUp")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1651_112_2_18_2_27_56_128,"VkbUp touchpanel")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1664_112_2_18_2_27_56_129,"HwFullScreenWinUp")
TRACE_MSG(MMIIM_TOUCH_IME_HW_1708_112_2_18_2_27_56_130,"HWFullScreenHandleTpMsg")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2345_112_2_18_2_27_57_131,"HwFullScreenCharUp key_ptr")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2387_112_2_18_2_27_58_132,"HwFullScreenCtrlUp key_ptr")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2446_112_2_18_2_27_58_133,"HwFullScreenCtrlUp VKEY_PAGEUP")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2462_112_2_18_2_27_58_134,"HwFullScreenCtrlUp VKEY_PAGEDOWN")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2493_112_2_18_2_27_58_135,"HwFullScreenSysUp key_ptr")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2533_112_2_18_2_27_58_136,"HwFullScreenCandUp key_ptr")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2541_112_2_18_2_27_58_137,"HwFullScreenCandUp select_idx")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2598_112_2_18_2_27_58_138,"HwFullScreenRecognizeCB")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2793_112_2_18_2_27_58_139,"HWFullScreenPreProc_HandleTpMsg")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2915_112_2_18_2_27_59_140,"HwInitEngineAttr")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2922_112_2_18_2_27_59_141,"HwHanWangRecognize")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2929_112_2_18_2_27_59_142,"HwHanWangRecognize")
TRACE_MSG(MMIIM_TOUCH_IME_HW_2941_112_2_18_2_27_59_143,"HwInitEngineAttr")
TRACE_MSG(MMIIM_TOUCH_IME_HW_3248_112_2_18_2_27_59_144,"HwHanWangRecognize")
TRACE_MSG(MMIIM_TOUCH_IME_HW_3255_112_2_18_2_27_59_145,"HwHanWangRecognize")
TRACE_MSG(MMIIM_TOUCH_IME_HW_3262_112_2_18_2_27_59_146,"HwHanWangRecognize")
TRACE_MSG(MMIIM_TOUCH_IME_HW_3779_112_2_18_2_28_0_147,"Sogou_HwRecognize")
TRACE_MSG(MMIIM_TOUCH_IME_HW_3782_112_2_18_2_28_0_148,"mmiim_touch_sogou.c Sogou_HwRecognize lang = %d")
TRACE_MSG(MMIIM_TOUCH_IME_HW_4078_112_2_18_2_28_1_149,"Sogou_CandToImeResult")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_61_112_2_18_2_28_2_150,"Sogou_TestCandNextPage")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_386_112_2_18_2_28_2_151,"Sogou_DrawVkbSkin")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_407_112_2_18_2_28_2_152,"Sogou_DrawVkbSkin")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_426_112_2_18_2_28_2_153,"Sogou_DrawVkbSkin")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_488_112_2_18_2_28_2_154,"ThumbLatinDispEdit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_589_112_2_18_2_28_3_155,"MMIIM_TOUCH_Sogou_GetImeResult")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_646_112_2_18_2_28_3_156,"Sogou_SetLastDisp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_684_112_2_18_2_28_3_157,"Sogou_GetLastDisp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_724_112_2_18_2_28_3_158,"Sogou_SetCapsLock")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_730_112_2_18_2_28_3_159,"Sogou_SetCapsLock")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_778_112_2_18_2_28_3_160,"Sogou_AssociateTo2DemensionArray")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_1233_112_2_18_2_28_4_161,"Sogou_ImeInit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_1240_112_2_18_2_28_4_162,"Sogou_ImeInit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_1269_112_2_18_2_28_4_163,"ThumbPinyinInit Sogou_ImeInit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_1712_112_2_18_2_28_5_164,"ThumbPinyinCharUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_1792_112_2_18_2_28_5_165,"ThumbPinyinCtrlLong")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_1844_112_2_18_2_28_5_166,"ThumbPinyinCtrlUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2093_112_2_18_2_28_6_167,"ThumbPinyinSetLetterCand")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2177_112_2_18_2_28_6_168,"ThumbPinyinCandUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2280_112_2_18_2_28_6_169,"ThumbPinyinSetCand")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2382_112_2_18_2_28_6_170,"ThumbPinyinDispEdit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2480_112_2_18_2_28_7_171,"ThumbPinyinSetCust")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2553_112_2_18_2_28_7_172,"QwertPinyinInit Sogou_ImeInit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2862_112_2_18_2_28_7_173,"ThumbStrokeCharUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2916_112_2_18_2_28_8_174,"ThumbStrokeCtrlLong")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_2968_112_2_18_2_28_8_175,"ThumbStrokeCtrlUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_3129_112_2_18_2_28_8_176,"ThumbStrokeCandUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_3221_112_2_18_2_28_8_177,"ThumbStrokeSetCand")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_3317_112_2_18_2_28_8_178,"ThumbStrokeDispEdit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_3465_112_2_18_2_28_9_179,"QwertPinyinInit Sogou_ImeInit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_3817_112_2_18_2_28_9_180,"QwertPinyinCtrlLong")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_3869_112_2_18_2_28_9_181,"QwertPinyinCtrlUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_4017_112_2_18_2_28_10_182,"QwertPinyinCandUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_4129_112_2_18_2_28_10_183,"QwertPinyinSetCand")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_4566_112_2_18_2_28_11_184,"ThumbSmartInit Sogou_ImeInit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_4666_112_2_18_2_28_11_185,"ThumbSmartCharLong")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_4768_112_2_18_2_28_11_186,"ThumbSmartCtrlLong")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_4835_112_2_18_2_28_11_187,"ThumbSmartCtrlUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_5036_112_2_18_2_28_12_188,"ThumbSmartCandUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_5149_112_2_18_2_28_12_189,"SmartSetCand")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_5294_112_2_18_2_28_12_190,"ThumbSmartDispEdit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_5403_112_2_18_2_28_12_191,"QwertPinyinInit Sogou_ImeInit")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_5830_112_2_18_2_28_13_192,"QwertSmartCtrlLong")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_5883_112_2_18_2_28_13_193,"QwertSmartCtrlUp")
TRACE_MSG(MMIIM_TOUCH_IME_SOGOU_6031_112_2_18_2_28_14_194,"QwertSmartCandUp")
TRACE_MSG(MMIIM_TOUCH_SET_133_112_2_18_2_28_14_195,"mmiim_touch_set.c: MMIIM_CreateSettingIMLayer setting_im_layer is already created")
TRACE_MSG(MMIIM_TOUCH_SET_212_112_2_18_2_28_14_196,"mmiim_touch_set.c: MMIIM_CreateSettingIMLayer data_ptr->settingim_menu_index malloc failed")
TRACE_MSG(MMIIM_TOUCH_SET_650_112_2_18_2_28_15_197,"mmiim_touch_set.c: MMIIM_HandleSettingIMMove---point not in heightligth rect---data_ptr->settingim_top_item %d,point.y %d highlight top %d bottom %d")
TRACE_MSG(MMIIM_TOUCH_SET_812_112_2_18_2_28_16_198,"mmiim_touch_set.c: MMIIM_HandleSettingIMUp point is not in the expected rect")
TRACE_MSG(MMIIM_TOUCH_SET_1000_112_2_18_2_28_16_199,"mmiim_touch_set.c: DrawSettingIm settingim_halfposition %d,highlight_index %d,data_ptr->settingim_top_item %d,data_ptr->settingim_last_highlight %d")
TRACE_MSG(MMIIM_TP_CSTAR_616_112_2_18_2_28_18_200,"[MMIIM] Initialize tpcstar: win_id=%X")
TRACE_MSG(MMIIM_TP_CSTAR_713_112_2_18_2_28_18_201,"[MMIIM] Terminate tpcstar: data_ptr is null.")
TRACE_MSG(MMIIM_TP_CSTAR_717_112_2_18_2_28_18_202,"[MMIIM] Terminate tpcstar: cs_handle=%08X")
TRACE_MSG(MMIIM_TP_CSTAR_739_112_2_18_2_28_18_203,"[MMIIM] Terminate tpcstar: stop timer id=%d")
TRACE_MSG(MMIIM_TP_CSTAR_747_112_2_18_2_28_18_204,"[MMIIM] Terminate tpcstar: release layer.")
TRACE_MSG(MMIIM_TP_CSTAR_777_112_2_18_2_28_18_205,"[MMIIM] HandleMsg: data_ptr is null.")
TRACE_MSG(MMIIM_TP_CSTAR_782_112_2_18_2_28_18_206,"[MMIIM] HandleMsg:event:%d,stateId:%d,stateId_bak:%d,stateId_bakii:%d")
TRACE_MSG(MMIIM_TP_CSTAR_904_112_2_18_2_28_18_207,"[MMIIM] HandleSysMsg: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_968_112_2_18_2_28_18_208,"[MMIIM] HandleSysMsg: IsImReloadDirty, method=%04X, stateId=%d")
TRACE_MSG(MMIIM_TP_CSTAR_1068_112_2_18_2_28_19_209,"[MMIIM] TransMsg: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_1799_112_2_18_2_28_20_210,"[MMIIM] TpUpNoInputProcess: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_1872_112_2_18_2_28_20_211,"[MMIIM] TpUpInputingProcess: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_1992_112_2_18_2_28_20_212,"[MMIIM] TpUpAssociateProcess: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_2084_112_2_18_2_28_21_213,"[MMIIM] TpUpUserWordProcess: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_2209_112_2_18_2_28_21_214,"[MMIIM] TpLongProcess: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_2305_112_2_18_2_28_21_215,"[MMIIM] TpCandPressUp, p.x=%d, p.y=%d")
TRACE_MSG(MMIIM_TP_CSTAR_2311_112_2_18_2_28_21_216,"[MMIIM] TpCandPressUp: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_2600_112_2_18_2_28_22_217,"[MMIIM] HandleChoicePageSimulator: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_2821_112_2_18_2_28_22_218,"[MMIIM] ConvertMethodtoState cs_im =%d")
TRACE_MSG(MMIIM_TP_CSTAR_3004_112_2_18_2_28_22_219,"[MMIIM] HandleCstarEvent")
TRACE_MSG(MMIIM_TP_CSTAR_3008_112_2_18_2_28_22_220,"[MMIIM] HandleCstarEvent: handle_ptr is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3136_112_2_18_2_28_23_221,"[MMIIM] HandleReturn")
TRACE_MSG(MMIIM_TP_CSTAR_3141_112_2_18_2_28_23_222,"[MMIIM] HandleReturn: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3169_112_2_18_2_28_23_223,"[MMIIM] HandleSwicth")
TRACE_MSG(MMIIM_TP_CSTAR_3174_112_2_18_2_28_23_224,"[MMIIM] HandleSwicth: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3224_112_2_18_2_28_23_225,"[MMIIM] HandleChar")
TRACE_MSG(MMIIM_TP_CSTAR_3230_112_2_18_2_28_23_226,"[MMIIM] HandleChar: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3319_112_2_18_2_28_23_227,"[MMIIM] Handle26Key")
TRACE_MSG(MMIIM_TP_CSTAR_3325_112_2_18_2_28_23_228,"[MMIIM] HandleChar: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3381_112_2_18_2_28_23_229,"[MMIIM] Handle9Key")
TRACE_MSG(MMIIM_TP_CSTAR_3387_112_2_18_2_28_23_230,"[MMIIM] Handle9Key: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3485_112_2_18_2_28_23_231,"[MMIIM] HandleMethodSelect")
TRACE_MSG(MMIIM_TP_CSTAR_3490_112_2_18_2_28_23_232,"[MMIIM] HandleMethodSelect: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3516_112_2_18_2_28_23_233,"[MMIIM] HandleCaps")
TRACE_MSG(MMIIM_TP_CSTAR_3522_112_2_18_2_28_23_234,"[MMIIM] HandleCaps: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3540_112_2_18_2_28_24_235,"[MMIIM] HandleCaps: method=%04X, new_method=%04X.")
TRACE_MSG(MMIIM_TP_CSTAR_3565_112_2_18_2_28_24_236,"[MMIIM] Handle26KeyTo9Key")
TRACE_MSG(MMIIM_TP_CSTAR_3569_112_2_18_2_28_24_237,"[MMIIM] Handle26KeyTo9Key: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3600_112_2_18_2_28_24_238,"[MMIIM] Handle26KeyTo9Key: method=%04X, new_method=%04X.")
TRACE_MSG(MMIIM_TP_CSTAR_3625_112_2_18_2_28_24_239,"[MMIIM] Handle9KeyTo26Key")
TRACE_MSG(MMIIM_TP_CSTAR_3629_112_2_18_2_28_24_240,"[MMIIM] Handle9KeyTo26Key: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3654_112_2_18_2_28_24_241,"[MMIIM] Handle9KeyTo26Key: method=%04X, new_method=%04X.")
TRACE_MSG(MMIIM_TP_CSTAR_3675_112_2_18_2_28_24_242,"[MMIIM] HandleLock")
TRACE_MSG(MMIIM_TP_CSTAR_3680_112_2_18_2_28_24_243,"[MMIIM] HandleLock: param is null.")
TRACE_MSG(MMIIM_TP_CSTAR_3728_112_2_18_2_28_24_244,"[MMIIM] HandleSwicthSymbol: param is null.")
TRACE_MSG(MMIIM_TP_SOGOU_2038_112_2_18_2_28_52_245,"TpCandPressUp iLoop=%d,font=%d,label_width=%d")
TRACE_MSG(MMIIM_TP_SOGOU_2395_112_2_18_2_28_53_246,"mmiim_tp_sogou.c: HandleSogouEvent: HandleImeOnKeyEvent return ERROR_INPUT_KEYS!")
TRACE_MSG(MMIIM_TP_SOGOU_2476_112_2_18_2_28_53_247,"HandleReturn")
TRACE_MSG(MMIIM_TP_SOGOU_2507_112_2_18_2_28_53_248,"HandleSwicth")
TRACE_MSG(MMIIM_TP_SOGOU_2558_112_2_18_2_28_53_249,"HandleChar")
TRACE_MSG(MMIIM_TP_SOGOU_2656_112_2_18_2_28_53_250,"Handle26Key")
TRACE_MSG(MMIIM_TP_SOGOU_2745_112_2_18_2_28_53_251,"Handle9Key")
TRACE_MSG(MMIIM_TP_SOGOU_2827_112_2_18_2_28_54_252,"HandleMethodSelect")
TRACE_MSG(MMIIM_TP_SOGOU_2858_112_2_18_2_28_54_253,"HandleCaps")
TRACE_MSG(MMIIM_TP_SOGOU_2905_112_2_18_2_28_54_254,"Handle26KeyTo9Key")
TRACE_MSG(MMIIM_TP_SOGOU_2963_112_2_18_2_28_54_255,"Handle9KeyTo26Key")
TRACE_MSG(MMIIM_TP_SOGOU_3011_112_2_18_2_28_54_256,"HandleLock")
TRACE_MSG(MMIIM_TP_SP_307_112_2_18_2_29_10_257,"Initialize")
TRACE_MSG(MMIIM_TP_SP_365_112_2_18_2_29_11_258,"Terminate")
TRACE_MSG(MMIIM_TP_SP_414_112_2_18_2_29_11_259,"HandleMsg:event:%d,stateId:%d,stateId_bak:%d,stateId_bakii:%d")
TRACE_MSG(MMIIM_TP_SP_1531_112_2_18_2_29_13_260,"TpCandPressUp")
TRACE_MSG(MMIIM_TP_SP_1957_112_2_18_2_29_14_261,"HandleSPEvent")
TRACE_MSG(MMIIM_TP_SP_2097_112_2_18_2_29_14_262,"HandleReturn")
TRACE_MSG(MMIIM_TP_SP_2124_112_2_18_2_29_14_263,"HandleSwicth")
TRACE_MSG(MMIIM_TP_SP_2178_112_2_18_2_29_14_264,"HandleChar")
TRACE_MSG(MMIIM_TP_SP_2257_112_2_18_2_29_14_265,"Handle26Key")
TRACE_MSG(MMIIM_TP_SP_2321_112_2_18_2_29_14_266,"Handle9Key")
TRACE_MSG(MMIIM_TP_SP_2390_112_2_18_2_29_15_267,"HandleMethodSelect")
TRACE_MSG(MMIIM_TP_SP_2421_112_2_18_2_29_15_268,"HandleCaps")
TRACE_MSG(MMIIM_TP_SP_2469_112_2_18_2_29_15_269,"Handle26KeyTo9Key")
TRACE_MSG(MMIIM_TP_SP_2523_112_2_18_2_29_15_270,"Handle9KeyTo26Key")
TRACE_MSG(MMIIM_TP_SP_2566_112_2_18_2_29_15_271,"HandleLock")
TRACE_MSG(MMIIM_TP_SP_2949_112_2_18_2_29_16_272,"buffer[%d]= 0x%x")
TRACE_MSG(MMIIM_TP_UI_1333_112_2_18_2_29_19_273,"[MMIIM] DrawAllBoard:%d")
TRACE_MSG(MMIIM_TP_UI_2224_112_2_18_2_29_21_274,"[MMIIM] MMIIMUI_DrawKeyBoard")
TRACE_MSG(MMIIM_TP_UI_2298_112_2_18_2_29_21_275,"[MMIIM] MMIIMUI_DrawReplaceKey")
TRACE_MSG(MMIIM_TP_UI_2442_112_2_18_2_29_21_276,"[MMIIM] MMIIMUI_DrawEditBar")
TRACE_MSG(MMIIM_TP_UI_2705_112_2_18_2_29_22_277,"[MMIIM] TpCandPressDown")
TRACE_MSG(MMIIM_TP_UI_2935_112_2_18_2_29_22_278,"[MMIIM] MMIIMUI_DrawCandBar")
TRACE_MSG(MMIIM_TP_UI_3579_112_2_18_2_29_23_279,"[MMIIM] MMIIMUI_ChangeRect: data_ptr is null.")
TRACE_MSG(MMIIM_TP_UI_3737_112_2_18_2_29_24_280,"[MMIIM] MMIIMUI_IsCommonKeyBoardState: handle_ptr is null.")
TRACE_MSG(MMIIM_TP_UI_3773_112_2_18_2_29_24_281,"[MMIIM] IsDisplayTips: handle_ptr is null.")
TRACE_MSG(MMIIM_TP_UI_3812_112_2_18_2_29_24_282,"[MMIIM] GetReplaceKeyImgID")
TRACE_MSG(MMIIM_TP_UI_4083_112_2_18_2_29_24_283,"[MMIIM] GetTipsRect: handle_ptr is null.")
TRACE_MSG(MMIIM_TP_UI_4584_112_2_18_2_29_25_284,"[MMIIM] GetVKboardState")
TRACE_MSG(MMIIM_TP_UI_4791_112_2_18_2_29_26_285,"[MMIIM] MMIIMUI_TransMsg: p.x=%d, p.y=%d, x=%d, y=%d")
TRACE_MSG(MMIIM_TP_UI_4946_112_2_18_2_29_26_286,"[MMIIM] MMIIMUI_SearchFromStrtab: param is null.")
TRACE_MSG(MMIIM_TP_UI_4975_112_2_18_2_29_26_287,"[MMIIM] MMIIMUI_ClearKbUpOnInValidRect: data_ptr is null.")
TRACE_MSG(MMIIM_WINTAB_739_112_2_18_2_29_28_288,"SetImPyFuzzy:selected_item[%d] %d is error!")
TRACE_MSG(MMIIM_WINTAB_823_112_2_18_2_29_28_289,"AppendImHwSpeedItem:hw_speed %d is error!")
TRACE_MSG(MMIIM_WINTAB_863_112_2_18_2_29_28_290,"SetImHwSpeed:cur_item %d is error!")
TRACE_MSG(MMIIM_WINTAB_946_112_2_18_2_29_28_291,"AppendImHwThicknessItem:hw_thickness %d is error!")
TRACE_MSG(MMIIM_WINTAB_986_112_2_18_2_29_28_292,"SetImHwThickness:cur_item %d is error!")
TRACE_MSG(MMIIM_WINTAB_1264_112_2_18_2_29_29_293,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID")
TRACE_MSG(MMIIM_WINTAB_1471_112_2_18_2_29_29_294,"HandleHwColorPenOkMsg: button_idx_ptr is null.")
TRACE_MSG(MMIIM_WINTAB_1574_112_2_18_2_29_30_295,"AppendImHwSpeedItem: hw_speed %d is error!")
TRACE_MSG(MMIIM_WINTAB_1625_112_2_18_2_29_30_296,"AppendImHwThicknessItem: hw_thickness %d is error!")
TRACE_MSG(MMIIM_WINTAB_1687_112_2_18_2_29_30_297,"AppendImHwThicknessItem: hw_thickness %d is error!")
TRACE_MSG(MMIIM_WINTAB_1726_112_2_18_2_29_30_298,"SetImKeyboardSize:cur_item %d is error!")
TRACE_MSG(WDP_SG_INTERFACE_NET_70_112_2_18_2_29_30_299,"wdp_SG_Interface_net.c SG_soc_setopt: error!")
TRACE_MSG(WDP_SG_INTERFACE_NET_107_112_2_18_2_29_31_300,"wdp_sg_interface_net.c SG_soc_create socket id = %d")
TRACE_MSG(WDP_SG_INTERFACE_NET_202_112_2_18_2_29_31_301,"SG_soc_send sci_sock_send err  conn_err=%d")
TRACE_MSG(WDP_SG_INTERFACE_NET_232_112_2_18_2_29_31_302,"SG_soc_recv recvSocketInput->s=%d,receive_len=%d")
TRACE_MSG(WDP_SG_INTERFACE_NET_267_112_2_18_2_29_31_303,"Sogou Core trace: %s")
TRACE_MSG(WDP_SG_INTERFACE_NET_593_112_2_18_2_29_32_304,"SG_Assert : assertion failed!")
TRACE_MSG(WDP_UPDATE_194_112_2_18_2_29_32_305,"sg_read_skinbinfile sfs_err=%d")
TRACE_MSG(WDP_UPDATE_212_112_2_18_2_29_32_306,"wdp_update.c UpdateInit: begin.")
TRACE_MSG(WDP_UPDATE_236_112_2_18_2_29_32_307,"wdp_update.c SG_UpdateSetNetWork s_is_resolving_address = %d")
TRACE_MSG(WDP_UPDATE_325_112_2_18_2_29_33_308,"wdp_update.c MMIAPIIM_SGUpdateCallBackFunc: s_update_status = %d ,s_is_set_network = %d")
TRACE_MSG(WDP_UPDATE_512_112_2_18_2_29_33_309,"SG_UPDATE_TIMEROUT sg_getDataOutput.errorCode=%d,sg_getDataOutput.httpState=%d")
TRACE_MSG(WDP_UPDATE_515_112_2_18_2_29_33_310,"SG_UPDATE_TIMEROUT SG_HTTP_OK")
TRACE_MSG(WDP_UPDATE_519_112_2_18_2_29_33_311,"SG_UPDATE_TIMEROUT SG_HTTP_STATE_IDLE")
TRACE_MSG(WDP_UPDATE_555_112_2_18_2_29_33_312,"SG_UPDATE_TIMEROUT error")
TRACE_MSG(WDP_UPDATE_679_112_2_18_2_29_33_313,"SG_Get_IMEI MNNV_GetIMEISVNEx Error!")
TRACE_MSG(WDP_UPDATE_695_112_2_18_2_29_33_314,"HandleSGPsMsg msg_ptr->msg_id=0x%04x, msg_ptr->result=0x%04x")
TRACE_MSG(WDP_UPDATE_702_112_2_18_2_29_33_315,"HandleSGPsMsg ProcessGprsActiveCnf succ")
TRACE_MSG(WDP_UPDATE_732_112_2_18_2_29_33_316,"HandleSGPsMsg g_sg_gprs_retry_time = %d")
TRACE_MSG(WDP_UPDATE_767_112_2_18_2_29_33_317,"wdp_update.c MMIAPIIM_SGGetSkinServerAdd: resolve successful")
TRACE_MSG(WDP_UPDATE_783_112_2_18_2_29_34_318,"wdp_update.c MMIAPIIM_SGGetSkinServerAdd: timeout")
TRACE_MSG(WDP_UPDATE_795_112_2_18_2_29_34_319,"wdp_update.c MMIAPIIM_SGGetSkinServerAdd: retry times = %d")
TRACE_MSG(WDP_UPDATE_WINTAB_786_112_2_18_2_29_36_320,"HandleInputSettingsMenuWindow 0x%04x")
TRACE_MSG(WDP_UPDATE_WINTAB_1248_112_2_18_2_29_37_321,"HandleSkinSelectMenuWindow win_id=0x%04x,msg_id=0x%04x")
TRACE_MSG(WDP_UPDATE_WINTAB_2432_112_2_18_2_29_39_322,"HandleSkinBrowseWindow win_id=0x%04x,msg_id=0x%04x")
TRACE_MSG(WDP_UPDATE_WINTAB_2437_112_2_18_2_29_39_323,"HandleSkinBrowseWindow ERR s_skin_ptr")
TRACE_MSG(WDP_UPDATE_WINTAB_3198_112_2_18_2_29_41_324,"HandleAutomaticUpdateSettingsListWindow msg_id = 0x%04x")
TRACE_MSG(WDP_UPDATE_WINTAB_3936_112_2_18_2_29_42_325,"SOGOU_AutoUpdateWordRemind g_sogou_update_settings_data.is_need_connect_remind=%d")
TRACE_MSG(WDP_UPDATE_WINTAB_3968_112_2_18_2_29_42_326,"SOGOU_AutoUpdateWordRemind g_sogou_update_settings_data.is_need_update_notif=%d")
TRACE_MSG(WDP_UPDATE_WINTAB_4128_112_2_18_2_29_43_327,"wdp_update_wintab.c OpenUpdateWindow: over wifi!")
TRACE_MSG(WDP_UPDATE_WINTAB_4133_112_2_18_2_29_43_328,"wdp_update_wintab.c OpenUpdateWindow: over sim, status = SG_IM_UPD_S_CREATING_SOCKET")
TRACE_MSG(WDP_UPDATE_WINTAB_4138_112_2_18_2_29_43_329,"wdp_update_wintab.c OpenUpdateWindow: over sim, status = SG_IM_UPD_S_START")
TRACE_MSG(WDP_UPDATE_WINTAB_4358_112_2_18_2_29_43_330,"HandleUpdateWatchingWindow MMIPUB_HandleWaitWinMsg begin win_id=0x%04x,msg_id=0x%04x")
TRACE_MSG(WDP_UPDATE_WINTAB_4378_112_2_18_2_29_43_331,"HandleUpdateExitWin win_id=0x%04x,msg_id=0x%04x")
END_TRACE_MAP(MMI_APP_IM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_IM_TRC_H_

