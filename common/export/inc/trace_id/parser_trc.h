/******************************************************************************
 ** File Name:      parser_trc.h                                         *
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
//trace_id:13
#ifndef _PARSER_TRC_H_
#define _PARSER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define BRW_CORE_API_66_112_2_18_0_36_32_0 "[BRW]BrwGetImgUrl image_ptr is null"
#define BRW_CORE_API_90_112_2_18_0_36_32_1 "[BRW]BrwGetImgUrl:BrwLayout failed"
#define BRW_CORE_API_127_112_2_18_0_36_32_2 "[BRW]BrwGetImgUrl:BrwLayout failed"
#define BRW_CORE_API_175_112_2_18_0_36_32_3 "[BRW]BrwGetInputImgUrl: image_ptr is null"
#define BRW_CORE_API_276_112_2_18_0_36_32_4 "[BRW]BrwUpdateImgMimeType use mime %d"
#define BRW_CORE_API_317_112_2_18_0_36_32_5 "[BRW]BrwSetImgBuf:data_len =%d, mime =%d"
#define BRW_CORE_API_359_112_2_18_0_36_32_6 "[BRW]BrwSetImgBuf:image_status =%d"
#define BRW_CORE_API_396_112_2_18_0_36_32_7 "[BRW]BrwSetImgBuf:image_status =%d"
#define BRW_CORE_API_401_112_2_18_0_36_32_8 "[BRW]BrwSetImgBuf:is_calc_ok =%d"
#define BRW_CORE_API_657_112_2_18_0_36_33_9 "BrwParseWebPage input invaild!"
#define BRW_CORE_API_663_112_2_18_0_36_33_10 "BrwParseWebPage: BrwSetParserInfo return false!"
#define BRW_CORE_API_673_112_2_18_0_36_33_11 "BrwParseWebPage: BrwSetLayoutInfo return false!"
#define BRW_CORE_API_688_112_2_18_0_36_33_12 "BrwParseWebPage: BRW_MIME_PLAINTEXT calloc fail!"
#define BRW_CORE_API_704_112_2_18_0_36_33_13 "BrwParseWebPage: BrwParse return %d!"
#define BRW_CORE_API_752_112_2_18_0_36_33_14 "BRW_ParseBufConstructor: create ctrl return false!"
#define BRW_CORE_API_759_112_2_18_0_36_33_15 "BRW_ParseBufConstructor: BrwSetParserBufInfo return false!"
#define BRW_CORE_API_765_112_2_18_0_36_33_16 "BRW_ParseBufConstructor: BrwParse return %d!"
#define BRW_CORE_API_871_112_2_18_0_36_34_17 "[BRW]BrwSetLayoutInfo error"
#define BRW_CORE_API_1202_112_2_18_0_36_34_18 "BRW: no layout when timer come\n"
#define BRW_CORE_API_1208_112_2_18_0_36_34_19 "BRW: unkown timer id\n"
#define BRW_CORE_API_1252_112_2_18_0_36_34_20 "BRW: no layout when timer come\n"
#define BRW_CORE_API_1495_112_2_18_0_36_35_21 "[BRW]BrwDestroyLayoutCtrl file:%s, line:%d"
#define BRW_CORE_API_1545_112_2_18_0_36_35_22 "[BRW]BrwProcTpScroll wml_ctrl_ptr error"
#define BRW_CORE_API_1554_112_2_18_0_36_35_23 "[BRW]BrwProcTpScroll xhtml_ctrl_ptr error"
#define BRW_CORE_API_1868_112_2_18_0_36_36_24 "[BRW]BrwSetBgsoundBuf in param error"
#define BRW_CORE_API_1875_112_2_18_0_36_36_25 "[BRW]BrwSetBgsoundBuf mime_type error"
#define BRW_CORE_API_1885_112_2_18_0_36_36_26 "[BRW]BrwSetBgsoundBuf bgsound_ptr is null"
#define BRW_CORE_API_1895_112_2_18_0_36_36_27 "[BRW]BrwSetBgsoundBuf:status =%d\tdata_len =%d\tmime_type =%d"
#define BRW_CORE_API_1903_112_2_18_0_36_36_28 "[BRW]BrwSetBgsoundBuf bgsound_ptr is null"
#define BRW_CORE_API_1913_112_2_18_0_36_36_29 "[BRW]BrwSetBgsoundBuf:status =%d\tdata_len =%d\tmime_type =%d"
#define BRW_CORE_API_1918_112_2_18_0_36_36_30 "[BRW]BrwSetBgsoundBuf bgsound_ptr is null OR content_type error:%d\n"
#define BRW_CORE_API_1962_112_2_18_0_36_36_31 "[BRW]BrwGetBgsoundBuf in param error"
#define BRW_CORE_API_1997_112_2_18_0_36_36_32 "[BRW]BrwGetBgsoundBuf error"
#define BRW_CORE_API_2036_112_2_18_0_36_36_33 "[brw]now BrwGetBgsoundUrl\n"
#define BRW_CORE_API_2040_112_2_18_0_36_36_34 "[BRW]BrwGetBgsoundUrl in param error"
#define BRW_CORE_API_2051_112_2_18_0_36_36_35 "[BRW]BrwGetBgsoundUrl bgsound_ptr is null"
#define BRW_CORE_API_2078_112_2_18_0_36_36_36 "[BRW]BrwGetBgsoundUrl bgsound_ptr is null"
#define BRW_CORE_API_2130_112_2_18_0_36_36_37 "[BRW]BrwFreeBgsoundBuf in param error"
#define BRW_CORE_API_3232_112_2_18_0_36_38_38 "[BRW]BrwLayoutPageUp input error"
#define BRW_CORE_API_3240_112_2_18_0_36_38_39 "[BRW]BrwLayoutPageUp wml_ctrl_ptr error"
#define BRW_CORE_API_3248_112_2_18_0_36_38_40 "[BRW]BrwLayoutPageUp xhtml_ctrl_ptr error"
#define BRW_CORE_API_3277_112_2_18_0_36_38_41 "[BRW]BrwLayoutPageDown input error"
#define BRW_CORE_API_3285_112_2_18_0_36_38_42 "[BRW]BrwLayoutPageDown wml_ctrl_ptr error"
#define BRW_CORE_API_3293_112_2_18_0_36_38_43 "[BRW]BrwLayoutPageDown xhtml_ctrl_ptr error"
#define BRW_CORE_API_3323_112_2_18_0_36_39_44 "[BRW]BrwLayoutLineUp input error"
#define BRW_CORE_API_3331_112_2_18_0_36_39_45 "[BRW]BrwLayoutLineUp wml_ctrl_ptr error"
#define BRW_CORE_API_3339_112_2_18_0_36_39_46 "[BRW]BrwLayoutLineUp xhtml_ctrl_ptr error"
#define BRW_CORE_API_3368_112_2_18_0_36_39_47 "[BRW]BrwLayoutLineDown input error"
#define BRW_CORE_API_3376_112_2_18_0_36_39_48 "[BRW]BrwLayoutLineDown wml_ctrl_ptr error"
#define BRW_CORE_API_3384_112_2_18_0_36_39_49 "[BRW]BrwLayoutLineDown xhtml_ctrl_ptr error"
#define BRW_CORE_API_3414_112_2_18_0_36_39_50 "[BRW]BrwLayoutMoveFocused input error"
#define BRW_CORE_API_3422_112_2_18_0_36_39_51 "[BRW]BrwLayoutMoveFocused wml_ctrl_ptr error"
#define BRW_CORE_API_3430_112_2_18_0_36_39_52 "[BRW]BrwLayoutMoveFocused xhtml_ctrl_ptr error"
#define BRW_CORE_API_3463_112_2_18_0_36_39_53 "[BRW]BrwLayoutMoveVertical input error"
#define BRW_CORE_API_3471_112_2_18_0_36_39_54 "[BRW]BrwLayoutMoveVertical wml_ctrl_ptr error"
#define BRW_CORE_API_3479_112_2_18_0_36_39_55 "[BRW]BrwLayoutMoveVertical xhtml_ctrl_ptr error"
#define BRW_CORE_API_3573_112_2_18_0_36_39_56 "[BRW]BrwGetFocusImageInfo IMG status= %d"
#define BRW_CORE_API_3590_112_2_18_0_36_39_57 "[BRW]BrwGetFocusImageInfo cur_card_ptr==%p"
#define BRW_CORE_API_3595_112_2_18_0_36_39_58 "[BRW]BrwGetFocusImageInfo wml_ctrl_ptr==%p"
#define BRW_CORE_API_3615_112_2_18_0_36_39_59 "[BRW]BrwGetFocusImageInfo IMG status= %d"
#define BRW_CORE_API_3630_112_2_18_0_36_39_60 "[BRW]BrwGetFocusImageInfo xhtml_ctrl_ptr==%p"
#define BRW_CORE_API_3686_112_2_18_0_36_39_61 "[BRW]BrwGetTpPressImageInfo xhtml_ctrl_ptr==%p"
#define BRW_CORE_API_3707_112_2_18_0_36_39_62 "[BRW]BrwGetTpPressImageInfo wml_ctrl_ptr==%p"
#define BRW_CORE_API_3780_112_2_18_0_36_39_63 "[BRW]BrwGetFocusImgBuf focused_hotlink_ptr==%p"
#define BRW_CORE_API_3785_112_2_18_0_36_39_64 "[BRW]BrwGetFocusImgBuf xhtml_ctrl_ptr==%p"
#define BRW_CORE_API_3819_112_2_18_0_36_40_65 "[BRW]BrwGetFocusImgBuf focused_hotlink_ptr==%p"
#define BRW_CORE_API_3824_112_2_18_0_36_40_66 "[BRW]BrwGetFocusImgBuf cur_card_ptr==%p"
#define BRW_CORE_API_3829_112_2_18_0_36_40_67 "[BRW]BrwGetFocusImgBuf wml_ctrl_ptr==%p"
#define BRW_CSS_131_112_2_18_0_36_40_68 "BRWCSS_Init"
#define BRW_CSS_136_112_2_18_0_36_40_69 "BRWCSS_Init lwc_initialise: %i\n"
#define BRW_CSS_146_112_2_18_0_36_40_70 "BRWCSS_Init css_initialise code: %d"
#define BRW_CSS_155_112_2_18_0_36_40_71 "BRWCSS_INIT css_stylesheet_create error!"
#define BRW_CSS_166_112_2_18_0_36_40_72 "BRWCSS_INIT css_stylesheet_append_data error!"
#define BRW_CSS_174_112_2_18_0_36_40_73 "BRWCSS_INIT css_stylesheet_append_data_done error!"
#define BRW_CSS_197_112_2_18_0_36_40_74 "BRWCSS_CreateHandle"
#define BRW_CSS_201_112_2_18_0_36_40_75 "BRWCSS_CreateHandle alloc fail"
#define BRW_CSS_248_112_2_18_0_36_40_76 "BRWCSS_LoadCss invalid input!"
#define BRW_CSS_270_112_2_18_0_36_40_77 "BRWCSS_LoadCss css_stylesheet_create error!"
#define BRW_CSS_288_112_2_18_0_36_40_78 "BRWCSS_LoadCss css_stylesheet_append_data error!"
#define BRW_CSS_366_112_2_18_0_36_40_79 "BRWCSS_LoadCss css_select_ctx_create error code is %d"
#define BRW_CSS_376_112_2_18_0_36_40_80 "BRWCSS_LoadCss css_select_ctx_append_sheet error code is %d"
#define BRW_CSS_392_112_2_18_0_36_40_81 "BRWCSS_LoadCss css_select_ctx_append_sheet error code is %d"
#define BRW_CSS_439_112_2_18_0_36_41_82 "BRWCSS_Parse css_select_ctx_create error code is %d"
#define BRW_CSS_451_112_2_18_0_36_41_83 "BRWCSS_Parse css_computed_style_create error code is %d"
#define BRW_CSS_480_112_2_18_0_36_41_84 "BRWCSS_Parse css_computed_style_compose error code is %d"
#define BRW_CSS_586_112_2_18_0_36_41_85 "BRWCSS_Deinit!"
#define BRW_CSS_595_112_2_18_0_36_41_86 "BRWCSS_Deinit error code %d"
#define BRW_CSS_640_112_2_18_0_36_41_87 "brw ExpatMalloc malloc error"
#define BRW_CSS_739_112_2_18_0_36_41_88 "XML ExpatRealloc, malloc p2 err!"
#define BRW_CSS_764_112_2_18_0_36_41_89 "brw BRWMem_ReallocEx malloc fail!"
#define BRW_CSS_845_112_2_18_0_36_41_90 "brw_css node_name  node == 0 || name == 0"
#define BRW_CSS_998_112_2_18_0_36_42_91 "named_ancestor_node node_name  node == 0 || name == 0 || ancestor == 0"
#define BRW_CSS_1039_112_2_18_0_36_42_92 "brw_css named_parent_node  node == 0 || name == 0 || parent == 0"
#define BRW_CSS_1077_112_2_18_0_36_42_93 "brw_css sibling  node == 0 || name == 0 || sibling == 0"
#define BRW_CSS_1115_112_2_18_0_36_42_94 "brw_css parent_node  node == 0 || parent == 0"
#define BRW_CSS_1148_112_2_18_0_36_42_95 "brw_css sibling_node  node == 0 || sibling == 0"
#define BRW_CSS_1382_112_2_18_0_36_43_96 "brw_css node_is_first_child  node == 0 || match == 0"
#define BRW_CSS_1406_112_2_18_0_36_43_97 "named_ancestor_node node_name  node == 0 || match == 0"
#define BRW_CSS_1437_112_2_18_0_36_43_98 "named_ancestor_node node_name  node == 0 || match == 0"
#define BRW_GLOBAL_105_112_2_18_0_36_45_99 "brw hash alloc buckets fail"
#define BRW_GLOBAL_407_112_2_18_0_36_45_100 "BRW: unkonw charset: try utf8\n"
#define BRW_GLOBAL_669_112_2_18_0_36_46_101 "[BRW]BrwUtf16CharToUtf8Buffer input error"
#define BRW_GLOBAL_1167_112_2_18_0_36_47_102 "Brw UTF8toUtf16 occurs unknown character 1"
#define BRW_GLOBAL_1180_112_2_18_0_36_47_103 "Brw UTF8toUtf16 occurs unknown character 2"
#define BRW_GLOBAL_1186_112_2_18_0_36_47_104 "Brw UTF8toUtf16 occurs unknown character 3"
#define BRW_GLOBAL_1479_112_2_18_0_36_47_105 "[BRW]brwstr_grow input error"
#define BRW_GLOBAL_1616_112_2_18_0_36_48_106 "[BRW]brwstr_search_char input error"
#define BRW_GLOBAL_1690_112_2_18_0_36_48_107 "[BRW]brwstr_insert_data input error"
#define BRW_GLOBAL_1871_112_2_18_0_36_48_108 "[BRW]woctstr_grow input error"
#define BRW_GLOBAL_1914_112_2_18_0_36_48_109 "[BRW]wbrwstr_insert_data input error"
#define BRW_GLOBAL_1959_112_2_18_0_36_48_110 "[BRW]wbrwstr_append_data input error"
#define BRW_GLOBAL_3086_112_2_18_0_36_51_111 "[BRW]RemallocStack error"
#define BRW_JS_468_112_2_18_0_36_52_112 "BRW BRWJS_Init input error"
#define BRW_JS_473_112_2_18_0_36_52_113 "BRW BRWJS_Init  context exsists"
#define BRW_JS_480_112_2_18_0_36_52_114 "BRW BRWJS_Init  init runtime error"
#define BRW_JS_487_112_2_18_0_36_52_115 "BRW BRWJS_Init  init context error"
#define BRW_JS_499_112_2_18_0_36_52_116 "BRW BRWJS_Init  init window error"
#define BRW_JS_537_112_2_18_0_36_52_117 "BrwJs_ErrorHandler input error"
#define BRW_JS_540_112_2_18_0_36_52_118 "Brw JS error  %s,File: %s:%u"
#define BRW_JS_574_112_2_18_0_36_52_119 "BRW BRWJS_Execute input error"
#define BRW_JS_610_112_2_18_0_36_52_120 "BRWJS_Execute result return false"
#define BRW_JS_632_112_2_18_0_36_53_121 "BRW BRWJS_Execute input error"
#define BRW_JS_665_112_2_18_0_36_53_122 "BRWJS_Execute result return false"
#define BRW_JS_682_112_2_18_0_36_53_123 "BRW BRWJS_Destroy input error "
#define BRW_JS_692_112_2_18_0_36_53_124 "Brw BRWJS_Destroy runtime NULL"
#define BRW_LAYOUT_70_112_2_18_0_36_53_125 "[BRW]LayoutCtrlNew error"
#define BRW_LAYOUT_222_112_2_18_0_36_53_126 "[BRW]BrwLayoutProcessTimerEvent wml_ctrl_ptr error"
#define BRW_LAYOUT_230_112_2_18_0_36_53_127 "[BRW]BrwLayoutProcessTimerEvent xhtml_ctrl_ptr error"
#define BRW_PARSER_119_112_2_18_0_36_54_128 "[BRW]BrwXmlParserCreate wml_parser_ptr error"
#define BRW_PARSER_139_112_2_18_0_36_54_129 "[BRW]BrwXmlParserCreate wml_parser_ptr error"
#define BRW_PARSER_165_112_2_18_0_36_54_130 "[BRW]BrwXmlParserCreate xhtml_parser_ptr error"
#define BRW_PARSER_220_112_2_18_0_36_54_131 "BrwXmlParse input invalid!"
#define BRW_PARSER_234_112_2_18_0_36_54_132 "BrwXmlParse: parser_ptr->is_use_wbxml!"
#define BRW_PARSER_242_112_2_18_0_36_54_133 "[BRW]BrwXmlParse parser_ptr error"
#define BRW_PARSER_247_112_2_18_0_36_54_134 "BrwXmlParse SGML_Parse fail!"
#define BRW_PARSER_277_112_2_18_0_36_54_135 "BrwParse input invalid!"
#define BRW_PARSER_283_112_2_18_0_36_54_136 "BrwParse BrwXmlParserCreate fail!"
#define BRW_PARSER_347_112_2_18_0_36_54_137 "[BRW]BrwXmlParserBufCreate xhtml_parser_ptr error"
#define BRW_PARSER_406_112_2_18_0_36_54_138 "BrwParseBuf input invalid!"
#define BRW_PARSER_412_112_2_18_0_36_54_139 "BrwParseBuf BrwXmlParserCreate fail!"
#define BRW_PARSER_462_112_2_18_0_36_55_140 "[BRW]BrwSetParserBufInfo xhtml_ctrl_ptr error"
#define BRW_PARSER_502_112_2_18_0_36_55_141 "BrwSetParserBufInfo:charset is NULL "
#define BRW_PARSER_506_112_2_18_0_36_55_142 "BrwSetParserBufInfo:charset=%s "
#define BRW_PARSER_545_112_2_18_0_36_55_143 "[BRW]BrwSetParserInfo wml_ctrl_ptr error"
#define BRW_PARSER_555_112_2_18_0_36_55_144 "[BRW]BrwSetParserInfo xhtml_ctrl_ptr error"
#define BRW_PARSER_608_112_2_18_0_36_55_145 "BrwParseWebPage: BrwSetParserInfo charset is NULL "
#define BRW_PARSER_612_112_2_18_0_36_55_146 "BrwParseWebPage: BrwSetParserInfo charset=%s "
#define BRW_SGML_PARSER_126_112_2_18_0_36_57_147 "brw SGML_Parse input invalid!"
#define BRW_SGML_PARSER_135_112_2_18_0_36_57_148 "[BRW]SGML_Parse parser member error"
#define BRW_SGML_PARSER_140_112_2_18_0_36_57_149 "brw SGML_Parse input buf_ptr is NULL or len <= 0!"
#define BRW_URL_196_112_2_18_0_36_58_150 "BRW_InsertClickURL invalid absoult_url"
#define BRW_URL_230_112_2_18_0_36_58_151 "BRW_InsertClickURL alloc memory fail"
#define BRW_URL_251_112_2_18_0_36_59_152 "BRW_IfHasClickedURL invalid input!"
#define BRW_URL_312_112_2_18_0_36_59_153 "[BRW]URL_MovetoSchemeEnd error"
#define BRW_URL_392_112_2_18_0_36_59_154 "[BRW]URL_MovetoHostEnd end error"
#define BRW_URL_701_112_2_18_0_37_0_155 "[BRW]LayoutGetVarList error"
#define BRW_URL_759_112_2_18_0_37_0_156 "[BRW]WmlPostFieldVdata2URL tv error"
#define BRW_URL_813_112_2_18_0_37_0_157 "[BRW]WmlPostFieldVdata2URL tv var_type error"
#define BRW_URL_1558_112_2_18_0_37_2_158 "[BRW]MakeRequestByTaskGo input error"
#define BRW_URL_1670_112_2_18_0_37_2_159 "[BRW]XhtmlMakeRequestByFrom layout_ctrl_ptr error"
#define BRW_URL_1694_112_2_18_0_37_2_160 "[BRW]XhtmlMakeRequestByFrom active_ptr error"
#define BRW_URL_1703_112_2_18_0_37_2_161 "[BRW]XhtmlMakeRequestByFrom input_ptr error"
#define BRW_URL_1744_112_2_18_0_37_2_162 "XhtmlMakeRequestByFrom alloc file data failed"
#define BRW_URL_1789_112_2_18_0_37_2_163 "[BRW]XhtmlMakeRequestByFrom button_ptr error"
#define BRW_URL_1809_112_2_18_0_37_2_164 "[BRW]XhtmlMakeRequestByFrom select_ptr error"
#define BRW_WML_LAYOUT_246_112_2_18_0_37_3_165 "[BRW]WmlLayoutProcessTimerEvent card_ptr error"
#define BRW_WML_LAYOUT_258_112_2_18_0_37_3_166 "BRW: card timer event coming"
#define BRW_WML_LAYOUT_347_112_2_18_0_37_4_167 "[BRW]WmlFindTimerInterval card_ptr error"
#define BRW_WML_LAYOUT_508_112_2_18_0_37_4_168 "BRW: backward had run in this card"
#define BRW_WML_LAYOUT_804_112_2_18_0_37_5_169 "[BRW]WmlLayoutLineDownAndMoveFocus input error"
#define BRW_WML_LAYOUT_894_112_2_18_0_37_5_170 "[BRW]WmlLayoutLineUpAndMoveFocus input error"
#define BRW_WML_LAYOUT_980_112_2_18_0_37_5_171 "[BRW]WmlLayoutPageUp input error"
#define BRW_WML_LAYOUT_1049_112_2_18_0_37_5_172 "[BRW]WmlLayoutPageDown input error"
#define BRW_WML_LAYOUT_1119_112_2_18_0_37_5_173 "[BRW]WmlLayoutMoveVerticalAndMoveFocus input error"
#define BRW_WML_LAYOUT_1176_112_2_18_0_37_5_174 "[BRW]WmlMoveFocusedLayout input error"
#define BRW_WML_LAYOUT_1368_112_2_18_0_37_6_175 "BRW: WmlLayoutCalc return false: no card \n"
#define BRW_WML_LAYOUT_1566_112_2_18_0_37_6_176 "[BRW]WmlLayoutRepaint input error"
#define BRW_WML_LAYOUT_1576_112_2_18_0_37_6_177 "[BRW]WmlLayoutRepaint  error"
#define BRW_WML_LAYOUT_1598_112_2_18_0_37_6_178 "[BRW]WmlLayoutRepaint win error"
#define BRW_WML_LAYOUT_1768_112_2_18_0_37_7_179 "[BRW]WmlSetSelectResult  wml_ctrl_ptr error"
#define BRW_WML_LAYOUT_1822_112_2_18_0_37_7_180 "[BRW]WmlSetSelectResult  wml_ctrl_ptr error"
#define BRW_WML_LAYOUT_1896_112_2_18_0_37_7_181 "[BRW]WmlFindHotLinkLayoutByPenPoint  input error"
#define BRW_WML_LAYOUT_1947_112_2_18_0_37_7_182 "[BRW]WmlFindHotLinkLayoutByPenPoint   error"
#define BRW_WML_LAYOUT_1960_112_2_18_0_37_7_183 "[BRW]Calc_P  input error"
#define BRW_WML_LAYOUT_2058_112_2_18_0_37_7_184 "[BRW]Calc_P   error"
#define BRW_WML_LAYOUT_2261_112_2_18_0_37_8_185 "[BRW]Draw_P  input error"
#define BRW_WML_LAYOUT_2413_112_2_18_0_37_8_186 "[BRW]Calc_Image  input type error"
#define BRW_WML_LAYOUT_2513_112_2_18_0_37_8_187 "[BRW]Calc_Image char_width  error"
#define BRW_WML_LAYOUT_2554_112_2_18_0_37_8_188 "[BRW]Draw_Image  input type error"
#define BRW_WML_LAYOUT_2628_112_2_18_0_37_8_189 "[BRW]Calc_Text input  error"
#define BRW_WML_LAYOUT_2645_112_2_18_0_37_8_190 "[BRW]Calc_Text  input type error"
#define BRW_WML_LAYOUT_2658_112_2_18_0_37_8_191 "[BRW]Calc_Text  error"
#define BRW_WML_LAYOUT_2689_112_2_18_0_37_9_192 "[BRW]Draw_Text input  error"
#define BRW_WML_LAYOUT_2764_112_2_18_0_37_9_193 "[BRW]FindOptionByIndex   error"
#define BRW_WML_LAYOUT_2782_112_2_18_0_37_9_194 "[BRW]TruncateSelectShowingText wml_ctrl_ptr  error"
#define BRW_WML_LAYOUT_2803_112_2_18_0_37_9_195 "[BRW]Calc_Select layout_type  error"
#define BRW_WML_LAYOUT_2858_112_2_18_0_37_9_196 "[BRW]Draw_Select layout_type  error"
#define BRW_WML_LAYOUT_2923_112_2_18_0_37_9_197 "[BRW]Calc_Anchor_A   error"
#define BRW_WML_LAYOUT_2961_112_2_18_0_37_9_198 "[BRW]Draw_Anchor layout_type  error"
#define BRW_WML_LAYOUT_3003_112_2_18_0_37_9_199 "[BRW]Draw_A layout_type  error"
#define BRW_WML_LAYOUT_3050_112_2_18_0_37_9_200 "[BRW]Calc_Do layout_type  error"
#define BRW_WML_LAYOUT_3089_112_2_18_0_37_9_201 "[BRW]Draw_Do input  error"
#define BRW_WML_LAYOUT_3140_112_2_18_0_37_9_202 "[BRW]Calc_Input input  error"
#define BRW_WML_LAYOUT_3193_112_2_18_0_37_10_203 "[BRW]Draw_Input input  error"
#define BRW_WML_LAYOUT_3261_112_2_18_0_37_10_204 "[BRW]Calc_Fieldset input  error"
#define BRW_WML_LAYOUT_3305_112_2_18_0_37_10_205 "[BRW]Draw_Fieldset input  error"
#define BRW_WML_LAYOUT_3835_112_2_18_0_37_11_206 "[BRW]Draw_Td_Children input  error"
#define BRW_WML_LAYOUT_3907_112_2_18_0_37_11_207 "[BRW]Draw_Table layout_type  error"
#define BRW_WML_LAYOUT_3995_112_2_18_0_37_11_208 "[BRW]wmlIsAnchorChildInScreen input  error"
#define BRW_WML_LAYOUT_4087_112_2_18_0_37_11_209 "[BRW]IsAnchorChildClickedByPen input  error"
#define BRW_WML_LAYOUT_4128_112_2_18_0_37_11_210 "[BRW]WmlSetRectMaxAndMinValue input  error"
#define BRW_WML_LAYOUT_4151_112_2_18_0_37_11_211 "[BRW]GetHotLinkLayoutChildRect input  error"
#define BRW_WML_LAYOUT_4169_112_2_18_0_37_11_212 "[BRW]GetHotLinkLayoutChildRect drawtxt_ptr  error"
#define BRW_WML_LAYOUT_4222_112_2_18_0_37_12_213 "[BRW]GetHotLinkLayoutRect   error"
#define BRW_WML_LAYOUT_4332_112_2_18_0_37_12_214 "[BRW]WmlFindHotLinkLayoutByPenPoint IMG"
#define BRW_WML_LAYOUT_4337_112_2_18_0_37_12_215 "[BRW]IsHotLinkLayoutInScreen   error"
#define BRW_WML_LAYOUT_4676_112_2_18_0_37_12_216 "[BRW]DrawHotLinkLayoutGetFocus wml_ctrl_ptr  error"
#define BRW_WML_LAYOUT_5058_112_2_18_0_37_13_217 "[BRW]task_go input  error"
#define BRW_WML_LAYOUT_5069_112_2_18_0_37_13_218 "[BRW]task_go input  error"
#define BRW_WML_LAYOUT_5081_112_2_18_0_37_13_219 "[BRW]task_prev input  error"
#define BRW_WML_LAYOUT_5099_112_2_18_0_37_13_220 "[BRW]task_refresh input  error"
#define BRW_WML_LAYOUT_5109_112_2_18_0_37_13_221 "[BRW]task_refresh wml_ctrl_ptr  error"
#define BRW_WML_LAYOUT_5113_112_2_18_0_37_13_222 "BRW: wml refresh\n"
#define BRW_WML_LAYOUT_5149_112_2_18_0_37_13_223 "[BRW]task_refresh wml_ctrl_ptr  error"
#define BRW_WML_LAYOUT_5158_112_2_18_0_37_13_224 "[BRW]task_refresh wml_ctrl_ptr  error"
#define BRW_WML_LAYOUT_5267_112_2_18_0_37_14_225 "[BRW]WmlLayoutTpMoveVerticalAndMoveFocus input  error"
#define BRW_WML_LAYOUT_5349_112_2_18_0_37_14_226 "[BRW]WmlPointIsInHotLink input  error"
#define BRW_WML_LAYOUT_5385_112_2_18_0_37_14_227 "[BRW]WmlGetFocusAnchorChildrenImageInfo input  error"
#define BRW_WML_LAYOUT_5508_112_2_18_0_37_14_228 "#brw BrwGetStringInfoInLineWithDir return value error!"
#define BRW_WML_PARSER_383_112_2_18_0_37_15_229 "[BRW]WML_StartElement_Func input  error"
#define BRW_WML_PARSER_424_112_2_18_0_37_15_230 "[BRW]WML_StartElement_Func index  error"
#define BRW_WML_PARSER_464_112_2_18_0_37_16_231 "[BRW]WML_EndElement_Func input  error"
#define BRW_WML_PARSER_484_112_2_18_0_37_16_232 "[BRW]WML_EndElement_Func input  error"
#define BRW_WML_PARSER_520_112_2_18_0_37_16_233 "[BRW]WML_CharacterData_Func input  error"
#define BRW_WML_PARSER_537_112_2_18_0_37_16_234 "[BRW]WML_StartCdataSection_Func input  error"
#define BRW_WML_PARSER_547_112_2_18_0_37_16_235 "[BRW]WML_EndCdataSection_Func input  error"
#define BRW_WML_PARSER_558_112_2_18_0_37_16_236 "[BRW]WML_DectXml input  error"
#define BRW_WML_PARSER_608_112_2_18_0_37_16_237 "[BRW]WBXML_StartElementHandler input  error"
#define BRW_WML_PARSER_617_112_2_18_0_37_16_238 "<%s"
#define BRW_WML_PARSER_624_112_2_18_0_37_16_239 " %s=\"%s\""
#define BRW_WML_PARSER_630_112_2_18_0_37_16_240 "/>\n"
#define BRW_WML_PARSER_632_112_2_18_0_37_16_241 ">\n"
#define BRW_WML_PARSER_666_112_2_18_0_37_16_242 "[BRW]WBXML_StartElementHandler parent_index  error"
#define BRW_WML_PARSER_704_112_2_18_0_37_16_243 "[BRW]WBXML_EndElementHandler input  error"
#define BRW_WML_PARSER_711_112_2_18_0_37_16_244 "</%s>\n"
#define BRW_WML_PARSER_731_112_2_18_0_37_16_245 "[BRW]WBXML_EndElementHandler parent_index  error"
#define BRW_WML_PARSER_766_112_2_18_0_37_16_246 "[BRW]WBXML_CharactersHandler input  error"
#define BRW_WML_PARSER_782_112_2_18_0_37_16_247 "[BRW]WBXML_StartDocumentHandler input  error"
#define BRW_WML_PARSER_1011_112_2_18_0_37_17_248 "[BRW]WML_ProcessCharacter parent_index  error"
#define BRW_WML_PARSER_1023_112_2_18_0_37_17_249 "[BRW]WML_ProcessCharacter len  error"
#define BRW_WML_PARSER_1046_112_2_18_0_37_17_250 "[BRW]WBXML_ProcessCharacter len  error"
#define BRW_WML_PARSER_1065_112_2_18_0_37_17_251 "[BRW]WBXML_ProcessCharacter parent_index  error"
#define BRW_WML_PARSER_1146_112_2_18_0_37_17_252 "[BRW]DoDuplicate input  error"
#define BRW_WML_PARSER_1229_112_2_18_0_37_17_253 "[BRW]DoCompare layout_type  error"
#define BRW_WML_PARSER_2253_112_2_18_0_37_19_254 "[BRW]WmlParse_Wml_Child wml_ctrl_ptr  error"
#define BRW_WML_PARSER_2300_112_2_18_0_37_19_255 "[BRW]WmlParse_Wml_Child layout_type  error"
#define BRW_WML_PARSER_2655_112_2_18_0_37_20_256 "\n[brw]WmlParse_Anchor_Child process second bgsound\n"
#define BRW_WML_PARSER_2673_112_2_18_0_37_20_257 "[BRW]WmlParse_A_Child   error"
#define BRW_WML_PARSER_2740_112_2_18_0_37_20_258 "\n[brw]WmlParse_A_Child process second bgsound\n"
#define BRW_WML_PARSER_2939_112_2_18_0_37_21_259 "[BRW]WmlParse_P_Child layout_type  error"
#define BRW_WML_PARSER_3050_112_2_18_0_37_21_260 "\n[brw]WmlParse_A_Child process second bgsound\n"
#define BRW_WML_PARSER_3227_112_2_18_0_37_21_261 "[BRW]WmlParse_Prev_Child task_type  error"
#define BRW_WML_PARSER_3250_112_2_18_0_37_21_262 "[BRW]WmlParse_Head_Child input  error"
#define BRW_WML_PARSER_3335_112_2_18_0_37_21_263 "[BRW]WmlParse_Card_Child layout_type  error"
#define BRW_WML_PARSER_3488_112_2_18_0_37_22_264 "\n[brw]WmlParse_Card_Child process second bgsound\n"
#define BRW_WML_PARSER_3697_112_2_18_0_37_22_265 "\n[brw]WmlParse_A_Child process second bgsound\n"
#define BRW_WML_PARSER_3744_112_2_18_0_37_22_266 "\n[BRW]set bgsound loop = %d\n"
#define BRW_WML_PARSER_3749_112_2_18_0_37_22_267 "\n[BRW]Invaild bgsound loop value:%s\n"
#define BRW_WML_PARSER_3768_112_2_18_0_37_22_268 "\n[BRW]set bgsound balance = %d\n"
#define BRW_WML_PARSER_3774_112_2_18_0_37_22_269 "\n[BRW]Invaild bgsound balance value:%s\n"
#define BRW_WML_PARSER_3792_112_2_18_0_37_22_270 "\n[BRW]set bgsound volume = %d\n"
#define BRW_WML_PARSER_3798_112_2_18_0_37_22_271 "\n[BRW]Invaild bgsound volume value:%s\n"
#define BRW_WML_PARSER_3816_112_2_18_0_37_22_272 "[BRW]WML_BgsoundAttrsValueIsValid in param error"
#define BRW_XHTML_CALC_DRAW_5246_112_2_18_0_37_34_273 "#brw Xhtml_Draw_Input BrwGetStringInfoInLineWithDir return value error!"
#define BRW_XHTML_CALC_DRAW_8815_112_2_18_0_37_42_274 "#brw BrwGetStringInfoInLineWithDir return value error!"
#define BRW_XHTML_LAYOUT_3166_112_2_18_0_37_48_275 "[BRW]XhtmlLayoutInputClickProcess active_ptr error"
#define BRW_XHTML_LAYOUT_3292_112_2_18_0_37_49_276 "[BRW]WmlGetFocusAnchorChildrenImageInfo input  error"
#define BRW_XHTML_LAYOUT_3752_112_2_18_0_37_50_277 "[BRW]XhtmlHandleReset active_ptr  error"
#define BRW_XHTML_LAYOUT_3761_112_2_18_0_37_50_278 "[BRW]XhtmlHandleReset input_ptr  error"
#define BRW_XHTML_LAYOUT_3807_112_2_18_0_37_50_279 "[BRW]XhtmlHandleReset select_ptr  error"
#define BRW_XHTML_LAYOUT_4585_112_2_18_0_37_51_280 "Brw Xhtml_ComputeCSS Start"
#define BRW_XHTML_LAYOUT_4597_112_2_18_0_37_51_281 "Brw Xhtml_ComputeCSS end"
#define BRW_XHTML_PARSER_108_112_2_18_0_37_52_282 "brw BRWMem_ReallocEx malloc fail!"
#define BRW_XHTML_PARSER_1021_112_2_18_0_37_54_283 "brw XHTML_EndElement_Func: get invalid index"
#define BRW_XHTML_PARSER_1387_112_2_18_0_37_55_284 "[BRW]XHTML_ProcessCharacter parent_layout  error"
#define BRW_XHTML_PARSER_2920_112_2_18_0_37_58_285 "\n[BRW]set bgsound loop = %d\n"
#define BRW_XHTML_PARSER_2925_112_2_18_0_37_58_286 "\n[BRW]Invaild bgsound loop value:%s\n"
#define BRW_XHTML_PARSER_2944_112_2_18_0_37_58_287 "\n[BRW]set bgsound balance = %d\n"
#define BRW_XHTML_PARSER_2950_112_2_18_0_37_58_288 "\nInvaild bgsound balance value:%s\n"
#define BRW_XHTML_PARSER_2968_112_2_18_0_37_58_289 "\n[BRW]set bgsound volume = %d\n"
#define BRW_XHTML_PARSER_2974_112_2_18_0_37_58_290 "\n[BRW]Invaild bgsound volume value:%s\n"
#define BRW_XHTML_PARSER_3800_112_2_18_0_37_59_291 "XHTML_ProcessTag image is null"
#define BRW_XHTML_PARSER_3977_112_2_18_0_38_0_292 "\nXHTML_ProcessTag process second bgsound\n"
#define BRW_XHTML_PARSER_4062_112_2_18_0_38_0_293 "XHTML_ProcessTag object image is null"
#define BRW_XHTML_PARSER_4100_112_2_18_0_38_0_294 "[BRW] XHTML_ProcessTag:parser_ptr->stack_layout_ptr is NULL!"
#define BRW_XHTML_PARSER_4323_112_2_18_0_38_0_295 "[BRW]Xhtml_BgsoundAttrsValueIsValid in param error"
#define BRW_XHTML_PARSER_4611_112_2_18_0_38_1_296 "ProcessParseBufCharacter: Other tag need not inform app"
#define BRW_XHTML_PARSER_4925_112_2_18_0_38_2_297 "[BRW] XHTML_ProcessTag:parser_ptr->stack_layout_ptr is NULL!"
#define BRW_XHTML_PARSER_5161_112_2_18_0_38_2_298 "brw XHTML_EndElement_Func: get invalid index"
#define JSOBJECT_ANCHOR_74_112_2_18_0_38_2_299 "JSObject_SetAnchorProperty JS_GetPrivate null or type error"
#define JSOBJECT_ANCHOR_118_112_2_18_0_38_3_300 "JSObject_SetAnchorProperty JS_GetPrivate null or type error"
#define JSOBJECT_BODY_145_112_2_18_0_38_3_301 "sipdermonkey: func, bodySetDir not implemented"
#define JSOBJECT_BODY_174_112_2_18_0_38_3_302 "sipdermonkey: func, bodyGetLang not implemented"
#define JSOBJECT_CHECKBOX_94_112_2_18_0_38_4_303 "JSObject_GetCheckboxProperty JS_GetPrivate null or type error"
#define JSOBJECT_CHECKBOX_159_112_2_18_0_38_4_304 "JSObject_SetCheckboxProperty JS_GetPrivate null or type error"
#define JSOBJECT_CHECKBOX_220_112_2_18_0_38_4_305 "JSObject_SetCheckboxProperty JS_GetPrivate null or type error"
#define JSOBJECT_DIV_76_112_2_18_0_38_4_306 "JSObject_GetDivProperty JS_GetPrivate null or type error"
#define JSOBJECT_DIV_174_112_2_18_0_38_5_307 "JSObject_SetDivProperty JS_GetPrivate null or type error"
#define JSOBJECT_DIV_181_112_2_18_0_38_5_308 "JSObject_GetDivProperty xhtml_ctrl_ptr error"
#define JSOBJECT_DOC_621_112_2_18_0_38_7_309 "sipdermonkey: func, document_creatTextNode not implemented"
#define JSOBJECT_DOC_1125_112_2_18_0_38_8_310 "sipdermonkey: func, documentLastModified not implemented"
#define JSOBJECT_FORM_110_112_2_18_0_38_9_311 "JSObject_GetFormProperty JS_GetPrivate null or type error"
#define JSOBJECT_FORM_241_112_2_18_0_38_9_312 "JSObject_SetFormProperty JS_GetPrivate null or type error"
#define JSOBJECT_FORM_248_112_2_18_0_38_9_313 "BRW JSObject_SetFormProperty handle error"
#define JSOBJECT_FORM_337_112_2_18_0_38_9_314 "JSObject_SetFormProperty JS_GetPrivate null or type error"
#define JSOBJECT_FORM_375_112_2_18_0_38_9_315 "JSObject_SetFormProperty JS_GetPrivate null or type error"
#define JSOBJECT_IMAGE_220_112_2_18_0_38_11_316 "sipdermonkey: func, imageSetBorder not implemented"
#define JSOBJECT_IMAGE_304_112_2_18_0_38_11_317 "sipdermonkey: func, imageSetHspace not implemented"
#define JSOBJECT_IMAGE_470_112_2_18_0_38_12_318 "sipdermonkey: func, imageSetHspace not implemented"
#define JSOBJECT_IMAGE_517_112_2_18_0_38_12_319 "sipdermonkey: func, image_onload implemented"
#define JSOBJECT_INPUTBUTTON_104_112_2_18_0_38_12_320 "JSObject_GetInputbuttonProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTBUTTON_163_112_2_18_0_38_12_321 "JSObject_SetInputbuttonProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTBUTTON_232_112_2_18_0_38_13_322 "JSObject_SetInputbuttonProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTBUTTON_275_112_2_18_0_38_13_323 "JSObject_SetInputbuttonProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTBUTTON_320_112_2_18_0_38_13_324 "JSObject_SetInputbuttonProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTHIDDEN_80_112_2_18_0_38_13_325 "JSObject_GetInputhiddenProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTHIDDEN_139_112_2_18_0_38_13_326 "JSObject_SetInputhiddenProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTPASSWORD_105_112_2_18_0_38_14_327 "JSObject_GetInputpasswordProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTPASSWORD_174_112_2_18_0_38_14_328 "JSObject_SetInputpasswordProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTPASSWORD_226_112_2_18_0_38_14_329 "BRW JSObject_SetInputpasswordProperty vp type error"
#define JSOBJECT_INPUTPASSWORD_234_112_2_18_0_38_14_330 "BRW JSObject_SetInputpasswordProperty vp type error"
#define JSOBJECT_INPUTPASSWORD_270_112_2_18_0_38_14_331 "JSObject_SetInputpasswordProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTPASSWORD_313_112_2_18_0_38_14_332 "JSObject_SetInputpasswordProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTRADIO_97_112_2_18_0_38_15_333 "JSObject_GetInputradioProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTRADIO_165_112_2_18_0_38_15_334 "JSObject_SetInputradioProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTRADIO_241_112_2_18_0_38_15_335 "JSObject_SetInputradioProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTRESET_92_112_2_18_0_38_15_336 "JSObject_GetInputresetProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTRESET_151_112_2_18_0_38_16_337 "JSObject_SetInputresetProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTRESET_222_112_2_18_0_38_16_338 "JSObject_SetInputresetProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTSUBMIT_93_112_2_18_0_38_16_339 "JSObject_GetInputsubmitProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTSUBMIT_155_112_2_18_0_38_16_340 "JSObject_SetInputsubmitProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTSUBMIT_229_112_2_18_0_38_16_341 "JSObject_SetInputsubmitProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTTEXT_106_112_2_18_0_38_17_342 "JSObject_GetInputtextProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTTEXT_176_112_2_18_0_38_17_343 "JSObject_SetInputtextProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTTEXT_241_112_2_18_0_38_17_344 "BRW JSObject_SetInputtextProperty vp type error"
#define JSOBJECT_INPUTTEXT_249_112_2_18_0_38_17_345 "BRW JSObject_SetInputtextProperty vp type error"
#define JSOBJECT_INPUTTEXT_284_112_2_18_0_38_17_346 "JSObject_SetInputtextProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTTEXT_327_112_2_18_0_38_17_347 "JSObject_SetInputtextProperty JS_GetPrivate null or type error"
#define JSOBJECT_INPUTTEXT_372_112_2_18_0_38_17_348 "JSObject_SetInputtextProperty JS_GetPrivate null or type error"
#define JSOBJECT_LOCATION_362_112_2_18_0_38_19_349 "BRW JSObject_Location_Assign url null error"
#define JSOBJECT_LOCATION_431_112_2_18_0_38_19_350 "BRW JSObject_Location_Assign url null error"
#define JSOBJECT_OPTION_84_112_2_18_0_38_21_351 "JSObject_GetOptionProperty JS_GetPrivate null or type error"
#define JSOBJECT_OPTION_156_112_2_18_0_38_21_352 "JSObject_SetOptionProperty JS_GetPrivate null or type error"
#define JSOBJECT_SELECT_107_112_2_18_0_38_22_353 "JSObject_GetSelectProperty JS_GetPrivate null or type error"
#define JSOBJECT_SELECT_199_112_2_18_0_38_22_354 "JSObject_SetSelectProperty JS_GetPrivate null or type error"
#define JSOBJECT_SELECT_206_112_2_18_0_38_22_355 "JSObject_GetSelectProperty xhtml_ctrl_ptr error"
#define JSOBJECT_SELECT_278_112_2_18_0_38_22_356 "JSObject_SetSelectProperty JS_GetPrivate null or type error"
#define JSOBJECT_SELECT_315_112_2_18_0_38_22_357 "JSObject_SetSelectProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLE_85_112_2_18_0_38_23_358 "JSObject_GetTableProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLE_128_112_2_18_0_38_23_359 "JSObject_SetTableProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLE_180_112_2_18_0_38_23_360 "JSObject_SetTableProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLE_192_112_2_18_0_38_23_361 "BRW JSObject_Table_InsertRow vp type error"
#define JSOBJECT_TABLE_198_112_2_18_0_38_23_362 "BRW JSObject_Table_InsertRow val error"
#define JSOBJECT_TABLECELL_78_112_2_18_0_38_23_363 "JSObject_GetTablecellProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLECELL_127_112_2_18_0_38_23_364 "JSObject_SetTablecellProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLECELL_134_112_2_18_0_38_23_365 "JSObject_GetTablecellProperty xhtml_ctrl_ptr error"
#define JSOBJECT_TABLEROW_83_112_2_18_0_38_24_366 "JSObject_GetTablerowProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLEROW_123_112_2_18_0_38_24_367 "JSObject_SetTablerowProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLEROW_130_112_2_18_0_38_24_368 "JSObject_GetTablerowProperty xhtml_ctrl_ptr error"
#define JSOBJECT_TABLEROW_175_112_2_18_0_38_24_369 "JSObject_SetTablerowProperty JS_GetPrivate null or type error"
#define JSOBJECT_TABLEROW_182_112_2_18_0_38_24_370 "BRW JSObject_Tablerow_InsertCell handle error"
#define JSOBJECT_TABLEROW_188_112_2_18_0_38_24_371 "BRW JSObject_Tablerow_InsertCell argv  type error"
#define JSOBJECT_TEXTAREA_104_112_2_18_0_38_24_372 "JSObject_GetTextareaProperty JS_GetPrivate null or type error"
#define JSOBJECT_TEXTAREA_167_112_2_18_0_38_25_373 "JSObject_SetTextareaProperty JS_GetPrivate null or type error"
#define JSOBJECT_TEXTAREA_201_112_2_18_0_38_25_374 "BRW JSObject_SetTextareaProperty vp type error"
#define JSOBJECT_TEXTAREA_210_112_2_18_0_38_25_375 "BRW JSObject_SetTextareaProperty vp type error"
#define JSOBJECT_TEXTAREA_246_112_2_18_0_38_25_376 "JSObject_SetTextareaProperty JS_GetPrivate null or type error"
#define JSOBJECT_TEXTAREA_289_112_2_18_0_38_25_377 "JSObject_SetTextareaProperty JS_GetPrivate null or type error"
#define JSAPI_3128_112_2_18_0_38_45_378 "js err, file:%s,line:%d"
#define JSDHASH_582_112_2_18_0_39_9_379 "js err, file:%s,line:%d"
#define JSDTOA_876_112_2_18_0_39_11_380 "js err, file:%,line:%d; cmp called with a->x[a->wds-1] == 0"
#define JSDTOA_878_112_2_18_0_39_11_381 "js err, file:%,line:%d; cmp called with b->x[b->wds-1] == 0"
#define JSDTOA_1015_112_2_18_0_39_12_382 "js err, file:%s, line:%d; zero y in b2d"
#define JSDTOA_2083_112_2_18_0_39_14_383 "js err, file:%s,line:%d"
#define JSDTOA_2102_112_2_18_0_39_14_384 "js err, file:%s,line:%d"
#define JSDTOA_2244_112_2_18_0_39_14_385 "js err, file:%s,line:%d"
#define JSEMIT_653_112_2_18_0_39_18_386 "js err, file:%s,line:%d"
#define JSEMIT_825_112_2_18_0_39_18_387 "js err, file:%s,line:%d"
#define JSEMIT_1896_112_2_18_0_39_20_388 "js err, file:%s,line:%d"
#define JSEMIT_1910_112_2_18_0_39_20_389 "js err, file:%s,line:%d"
#define JSEMIT_1924_112_2_18_0_39_20_390 "js err, file:%s,line:%d"
#define JSEMIT_3255_112_2_18_0_39_23_391 "js err, file:%s,line:%d"
#define JSEMIT_3345_112_2_18_0_39_24_392 "js err, file:%s,line:%d"
#define JSEMIT_4152_112_2_18_0_39_25_393 "js err, file:%s,line:%d"
#define JSEMIT_4474_112_2_18_0_39_26_394 "js err, file:%s,line:%d"
#define JSEMIT_4714_112_2_18_0_39_27_395 "js err, file:%s,line:%d"
#define JSEMIT_4838_112_2_18_0_39_27_396 "js err, file:%s,line:%d"
#define JSEMIT_4970_112_2_18_0_39_27_397 "js err, file:%s,line:%d"
#define JSINTERP_3804_112_2_18_0_39_47_398 "js err, file:%s,line:%d"
#define JSINTERP_4815_112_2_18_0_39_49_399 "js err, file:%s,line:%d"
#define JSLOCK_992_112_2_18_0_39_53_400 "js err, file:%s,line:%d"
#define JSOBJ_234_112_2_18_0_39_59_401 "js err, file:%s,line:%d"
#define JSOPCODE_1525_112_2_18_0_40_11_402 "js err, file:%s,line:%d"
#define JSOPCODE_2139_112_2_18_0_40_12_403 "js err, file:%s,line:%d"
#define JSOPCODE_3139_112_2_18_0_40_14_404 "js err, file:%s,line:%d"
#define JSPARSE_1172_112_2_18_0_40_17_405 "js err, file:%s,line:%d"
#define JSPARSE_2770_112_2_18_0_40_20_406 "js err, file:%s,line:%d"
#define JSPRF_595_112_2_18_0_40_26_407 "js err, file:%s,line:%d"
#define JSPRF_608_112_2_18_0_40_26_408 "js err, file:%s,line:%d"
#define JSPRF_716_112_2_18_0_40_26_409 "js err, file:%s,line:%d"
#define JSPRF_976_112_2_18_0_40_27_410 "js err, file:%s,line:%d"
#define JSPRF_988_112_2_18_0_40_27_411 "js err, file:%s,line:%d"
#define JSPRF_1019_112_2_18_0_40_27_412 "js err, file:%s,line:%d"
#define JSSCRIPT_1175_112_2_18_0_40_46_413 "js err, file:%s,line:%d"
#define JSSCRIPT_1189_112_2_18_0_40_46_414 "js err, file:%s,line:%d"
#define JSXML_2104_112_2_18_0_41_3_415 "js err, file:%s,line:%d"
#define JSXML_2190_112_2_18_0_41_4_416 "js err, file:%s,line:%d"
#define MMMSDECODER_503_112_2_18_0_43_2_417 "MMS_PARSE:mmsReadWellKnownMedia type=%s"
#define MMMSDECODER_720_112_2_18_0_43_2_418 "MMS_PARSE: mmsReadConTypeVal: the charset is =%d"
#define MMMSDECODER_807_112_2_18_0_43_2_419 "MMS_PARSE: headerlen = %d"
#define MMMSDECODER_808_112_2_18_0_43_2_420 "MMS_PARSE: c_datalen = %d"
#define MMMSDECODER_819_112_2_18_0_43_2_421 "MMS_PARSE: content type is %s"
#define MMMSDECODER_1365_112_2_18_0_43_3_422 "MMS_PARSE: mmsGetSubjectLen subLen=%d"
#define MMMSDECODER_1382_112_2_18_0_43_3_423 "MMS_PARSE: **mmsGetSubjectLen subLen=%d"
#define MMMSDECODER_1396_112_2_18_0_43_3_424 "MMS_PARSE***mmsGetSubjectLen subLen=%d"
#define MMMSDECODER_1411_112_2_18_0_43_3_425 "MMS_PARSE*^_^mmsGetSubjectLen subLen=%d"
#define MMMSDECODER_1432_112_2_18_0_43_4_426 "MMS_PARSE:mmsReadHeader input invalid!"
#define MMMSDECODER_1445_112_2_18_0_43_4_427 "MMS_PARSE:mmsReadHeader, current byte = %d, m_i = %d, m_long = %d"
#define MMMSDECODER_1460_112_2_18_0_43_4_428 "MMS_PARSE:mmsReadHeader, message type = %d"
#define MMMSDECODER_1677_112_2_18_0_43_4_429 "mmsReadHeader pMmshead->Mms_subj is %s,length is %d"
#define MMMSDECODER_1861_112_2_18_0_43_4_430 "mmsReadHeader unsupport header,first byte value= %d"
#define MMMSDECODER_1959_112_2_18_0_43_5_431 "MMS_PARSE: mmsDecode input invalid! "
#define MMMSDECODER_1970_112_2_18_0_43_5_432 "MMS_PARSE: mmsDecode open decode file failed! "
#define MMMSDECODER_1978_112_2_18_0_43_5_433 "MMS_PARSE: *** error!mmsDecode:malloc failed"
#define MMMSDECODER_1999_112_2_18_0_43_5_434 "MMS_PARSE: mmsDecode:mms_cfa mmsfile size is :%d"
#define MMMSDECODER_2002_112_2_18_0_43_5_435 "MMS_PARSE: mmsDecode pMmsDecoder->m_long <5000"
#define MMMSDECODER_2006_112_2_18_0_43_5_436 "MMS_PARSE: mms lib *** error!mmsDecode:DOCTYPE wml PUBLIC"
#define MMMSDECODER_2014_112_2_18_0_43_5_437 "MMS_PARSE: mmsDecode, message type =%d"
#define MMMSDOC_94_112_2_18_0_43_5_438 "MMS_PARSE: mmsGetEncodedLen after change into utf-8,the size is %d,\n"
#define MMMSDOC_240_112_2_18_0_43_6_439 "MMS_PARSE: in mmsNewSlideSize not new_slide slide addr is %d"
#define MMMSDOC_571_112_2_18_0_43_6_440 "MMS_PARSE: erro:file name is too long"
#define MMMSDOC_708_112_2_18_0_43_6_441 "MMS_PARSE: MMS_AddConetnt in"
#define MMMSDOC_712_112_2_18_0_43_6_442 "MMS_PARSE: MMS_AddConetnt error:pSlide is null or pDocument is null"
#define MMMSDOC_716_112_2_18_0_43_6_443 "MMS_PARSE: MMS_AddConetnt type is %d"
#define MMMSDOC_720_112_2_18_0_43_6_444 "MMS_PARSE: MMS_AddConetnt pFilename = NULL"
#define MMMSDOC_724_112_2_18_0_43_6_445 "MMS_PARSE: MMS_AddConetnt insert content file name is :%s"
#define MMMSDOC_726_112_2_18_0_43_6_446 "MMS_PARSE: MMS_AddConetnt datalen is %d"
#define MMMSDOC_735_112_2_18_0_43_6_447 "MMS_PARSE: MMS_AddConetnt pContent = NULL"
#define MMMSDOC_752_112_2_18_0_43_7_448 "MMS_PARSE: MMS_AddConetnt: add text case ,will call mmsGetEncodedLen ,print all pFilename\n"
#define MMMSDOC_753_112_2_18_0_43_7_449 "MMS_PARSE: MMS_AddConetnt pFileData is %s,filelength is %d"
#define MMMSDOC_756_112_2_18_0_43_7_450 "MMS_PARSE: MMS_AddConetnt cms_mms get len is %d"
#define MMMSDOC_765_112_2_18_0_43_7_451 "MMS_PARSE: MMS_AddConetnt error:file length is too long"
#define MMMSDOC_782_112_2_18_0_43_7_452 "MMS_PARSE: MMS_AddConetnt:now add a img content"
#define MMMSDOC_785_112_2_18_0_43_7_453 "MMS_PARSE: MMS_AddConetnt pContent = %x"
#define MMMSDOC_788_112_2_18_0_43_7_454 "MMS_PARSE: MMS_AddConetnt error: pContent = NULL"
#define MMMSDOC_801_112_2_18_0_43_7_455 "MMS_PARSE: MMS_AddConetnt file name is %s"
#define MMMSDOC_825_112_2_18_0_43_7_456 "MMS_PARSE: MMS_AddConetnt error:file length is too long"
#define MMMSDOC_883_112_2_18_0_43_7_457 "MMS_PARSE: MMS_AddAttachment in"
#define MMMSDOC_888_112_2_18_0_43_7_458 "MMS_PARSE: MMS_AddAttachment error:param is null!"
#define MMMSDOC_891_112_2_18_0_43_7_459 "MMS_PARSE: MMS_AddAttachment insert content file name is :%s"
#define MMMSDOC_895_112_2_18_0_43_7_460 "MMS_PARSE: MMS_AddAttachment file name is %d"
#define MMMSDOC_898_112_2_18_0_43_7_461 "MMS_PARSE: MMS_AddAttachment error:file name length error!"
#define MMMSDOC_906_112_2_18_0_43_7_462 "MMS_PARSE: MMS_AddAttachment error: no memory"
#define MMMSDOC_962_112_2_18_0_43_7_463 "GetSuffixFromFileName: input error!"
#define MMMSDOC_999_112_2_18_0_43_7_464 "MMS_PARSE:GetContentTypeBySuffix, suffix = %s"
#define MMMSDOC_1426_112_2_18_0_43_8_465 "MMS_PARSE: MMS_CreateNewSlide slide addr is %d"
#define MMMSDOC_1427_112_2_18_0_43_8_466 "MMS_PARSE: MMS_CreateNewSlide pDocument addr is %d"
#define MMMSDOC_1476_112_2_18_0_43_8_467 "MMS_PARSE: MMS_CreateNewSlide slide addr is %d"
#define MMMSDOC_1477_112_2_18_0_43_8_468 "MMS_PARSE: MMS_CreateNewSlide pDocument addr is %d"
#define MMMSENCODER_643_112_2_18_0_43_10_469 "MMS_PARSE: Encodelist in text/pain case\n"
#define MMMSENCODER_662_112_2_18_0_43_10_470 "MMS_PARSE: Encodelist len is %d"
#define MMMSENCODER_1257_112_2_18_0_43_12_471 "MMS_PARSE: pMmsHead->Mms_content_type = %s"
#define MMMSENCODER_1332_112_2_18_0_43_12_472 "MMS_PARSE: mmsEncodeBody input invalid!"
#define MMMSENCODER_1352_112_2_18_0_43_12_473 "MMS_PARSE: mmsEncodeBody list is pnull!"
#define MMMSENCODER_1380_112_2_18_0_43_12_474 "MMS_PARSE: mmsEncode input invalid!"
#define MMMSENCODER_1388_112_2_18_0_43_12_475 "MMS_PARSE: mmsEncode alloc failed!"
#define MMMSENCODER_1395_112_2_18_0_43_12_476 "MMS_PARSE: mmsEncode ,pathname is %s"
#define MMMSENCODER_1402_112_2_18_0_43_12_477 "MMS_PARSE: mmsEncode out put 0!"
#define MMMSENCODER_1421_112_2_18_0_43_12_478 "MMS_PARSE: mmsEncode set file failed!"
#define MMMSENCODER_1437_112_2_18_0_43_12_479 "MMS_PARSE: mmsEncode encode body error!"
#define MMMSHTTP_244_112_2_18_0_43_13_480 "MMS_PARSE: get http net init cnf !"
#define MMMSHTTP_249_112_2_18_0_43_13_481 "MMS_PARSE: get wsp net init cnf !"
#define MMMSHTTP_255_112_2_18_0_43_13_482 "MMS_PARSE: get net request ind !"
#define MMMSHTTP_261_112_2_18_0_43_13_483 "MMS_PARSE: get net progress ind !"
#define MMMSHTTP_267_112_2_18_0_43_13_484 "MMS_PARSE: get net data ind !"
#define MMMSHTTP_273_112_2_18_0_43_13_485 "MMS_PARSE: get net post cnf !"
#define MMMSHTTP_279_112_2_18_0_43_13_486 "MMS_PARSE: get net close cnf !"
#define MMMSHTTP_285_112_2_18_0_43_13_487 "MMS_PARSE: get net cancel cnf !"
#define MMMSHTTP_291_112_2_18_0_43_13_488 "MMS_PARSE: get net header ind !"
#define MMMSHTTP_297_112_2_18_0_43_13_489 "MMS_PARSE: get net get cnf !"
#define MMMSHTTP_303_112_2_18_0_43_13_490 "MMS_PARSE: get net error ind !"
#define MMMSHTTP_309_112_2_18_0_43_13_491 "MMS_PARSE: get net auth ind !"
#define MMMSHTTP_452_112_2_18_0_43_13_492 "MMS_PARSE: MmsLibCancelMms enter! http context = %d, request =%d!"
#define MMMSHTTP_477_112_2_18_0_43_13_493 "MMS_PARSE: MmsLibCancelMms, send mms enter close request!"
#define MMMSHTTP_483_112_2_18_0_43_13_494 "MMS_PARSE: MmsLibCancelMms, recv mms enter close request!"
#define MMMSHTTP_494_112_2_18_0_43_13_495 "MMS_PARSE: MmsLibCancelMms, wait http init cnf and cancel!"
#define MMMSHTTP_519_112_2_18_0_43_13_496 "MMS_PARSE: MmsLibCancelMms wsp context = %d, request =%d!"
#define MMMSHTTP_523_112_2_18_0_43_13_497 "MMS_PARSE: MmsLibCancelMms, wsp send mms enter close request!"
#define MMMSHTTP_529_112_2_18_0_43_13_498 "MMS_PARSE: MmsLibCancelMms,wsp recv mms enter close request!"
#define MMMSHTTP_540_112_2_18_0_43_13_499 "MMS_PARSE: MmsLibCancelMms, wait wsp init cnf and cancel!"
#define MMMSHTTP_569_112_2_18_0_43_13_500 "MMS_PARSE:MmsLibHttpInitCnf input invalid!"
#define MMMSHTTP_576_112_2_18_0_43_13_501 "MMS_PARSE:MmsLibHttpInitCnf saved module id =%d, msg module id =%d!"
#define MMMSHTTP_585_112_2_18_0_43_13_502 "MMS_PARSE:MmsLibHttpInitCnf, context_id = %d, instance = %d"
#define MMMSHTTP_590_112_2_18_0_43_13_503 "MMS_PARSE:MmsLibHttpInitCnf init fail!"
#define MMMSHTTP_599_112_2_18_0_43_13_504 "MMS_PARSE: MmsLibHttpInitCnf, send mms enter close request!"
#define MMMSHTTP_605_112_2_18_0_43_13_505 "MMS_PARSE: MmsLibHttpInitCnf, recv mms enter close request!"
#define MMMSHTTP_638_112_2_18_0_43_13_506 "MMS_PARSE:MmsLibWspInitCnf input invalid!"
#define MMMSHTTP_645_112_2_18_0_43_13_507 "MMS_PARSE:MmsLibWspInitCnf saved module id =%d, msg module id =%d!"
#define MMMSHTTP_653_112_2_18_0_43_14_508 "MMS_PARSE:MmsLibWspInitCnf, context_id = %d"
#define MMMSHTTP_658_112_2_18_0_43_14_509 "MMS_PARSE:MmsLibWspInitCnf init fail!"
#define MMMSHTTP_667_112_2_18_0_43_14_510 "MMS_PARSE: MmsLibWspInitCnf, send mms enter close request!"
#define MMMSHTTP_673_112_2_18_0_43_14_511 "MMS_PARSE: MmsLibWspInitCnf, recv mms enter close request!"
#define MMMSHTTP_703_112_2_18_0_43_14_512 "MMS_PARSE: MmsLibHttpRequestIdInd input invalid!"
#define MMMSHTTP_707_112_2_18_0_43_14_513 "MMS_PARSE: MmsLibHttpRequestIdInd enter, g_context_id =%d, sig_context_id = %d"
#define MMMSHTTP_711_112_2_18_0_43_14_514 "MMS_PARSE: MmsLibHttpRequestIdInd context not match!"
#define MMMSHTTP_717_112_2_18_0_43_14_515 "MMS_PARSE: MmsLibHttpRequestIdInd , request id = %d"
#define MMMSHTTP_746_112_2_18_0_43_14_516 "MMS_PARSE: MmsLibHttpHeaderInd request does not match!"
#define MMMSHTTP_768_112_2_18_0_43_14_517 "MMS_PARSE: MmsLibHttpRecvDataInd input invalid!"
#define MMMSHTTP_778_112_2_18_0_43_14_518 "MMS_PARSE: MmsLibHttpRecvDataInd request does not match!"
#define MMMSHTTP_782_112_2_18_0_43_14_519 "MMS_PARSE: MmsLibHttpRecvDataInd,data_ptr = %d,len =%d"
#define MMMSHTTP_798_112_2_18_0_43_14_520 "MMS_PARSE: MmsLibHttpRecvDataInd, send mms, get data len = %d"
#define MMMSHTTP_802_112_2_18_0_43_14_521 "MMS_PARSE: MmsLibHttpRecvDataInd, save new data!"
#define MMMSHTTP_807_112_2_18_0_43_14_522 "MMS_PARSE: MmsLibHttpRecvDataInd, save again data!"
#define MMMSHTTP_826_112_2_18_0_43_14_523 "MMS_PARSE: MmsLibHttpRecvDataInd, create recv file fail!"
#define MMMSHTTP_837_112_2_18_0_43_14_524 "MMS_PARSE: MmsLibHttpRecvDataInd, write file fail!"
#define MMMSHTTP_844_112_2_18_0_43_14_525 "MMS_PARSE: MmsLibHttpRecvDataInd,current is http,recv is file. should no this message!"
#define MMMSHTTP_851_112_2_18_0_43_14_526 "MMS_PARSE: MmsLibHttpRecvDataInd, recv rsp mms do nothing!"
#define MMMSHTTP_874_112_2_18_0_43_14_527 "MMS_PARSE: MmsLibHttpNetProgIndSig input invalid!"
#define MMMSHTTP_884_112_2_18_0_43_14_528 "MMS_PARSE: MmsLibHttpNetProgIndSig request does not match!, sig request =%d"
#define MMMSHTTP_907_112_2_18_0_43_14_529 "MMS_PARSE:MmsLibHttpNetProgIndSig recv total len = %d, current len = %d"
#define MMMSHTTP_915_112_2_18_0_43_14_530 "MMS_PARSE:MmsLibHttpNetProgIndSig send total len = %d, current len = %d"
#define MMMSHTTP_921_112_2_18_0_43_14_531 "MMS_PARSE:  MmsLibHttpNetProgIndSig: status error"
#define MMMSHTTP_943_112_2_18_0_43_14_532 "MMS_PARSE:MmsLibHttpPostCnf,input invalid!"
#define MMMSHTTP_946_112_2_18_0_43_14_533 "MMS_PARSE: MmsLibHttpPostCnf enter! current opt =%d"
#define MMMSHTTP_955_112_2_18_0_43_14_534 "MMS_PARSE: MmsLibHttpPostCnf request does not match!, sig request =%d"
#define MMMSHTTP_964_112_2_18_0_43_14_535 "MMS_PARSE: MmsLibHttpPostCnf, file name is invalid"
#define MMMSHTTP_973_112_2_18_0_43_14_536 "MMS_PARSE: MmsLibHttpPostCnf, create new mms fail"
#define MMMSHTTP_986_112_2_18_0_43_14_537 "MMS_PARSE:MmsLibHttpPostCnf decode mms error! "
#define MMMSHTTP_1002_112_2_18_0_43_14_538 "MMS_PARSE: MmsLibHttpPostCnf, send mms enter close request!"
#define MMMSHTTP_1014_112_2_18_0_43_14_539 "MMS_PARSE: MmsLibHttpPostCnf, status error = %d!"
#define MMMSHTTP_1021_112_2_18_0_43_14_540 "MMS_PARSE: MmsLibHttpPostCnf, address unresolved!"
#define MMMSHTTP_1031_112_2_18_0_43_14_541 "MMS_PARSE: MmsLibHttpPostCnf, recv mms enter close request!"
#define MMMSHTTP_1065_112_2_18_0_43_14_542 "MMS_PARSE: MmsLibHttpErrorInd request does not match!, sig request =%d"
#define MMMSHTTP_1069_112_2_18_0_43_14_543 "MMS_PARSE: MmsLibHttpErrorInd, get net error = %d"
#define MMMSHTTP_1075_112_2_18_0_43_14_544 "MMS_PARSE: MmsLibHttpErrorInd, send mms enter close request!"
#define MMMSHTTP_1081_112_2_18_0_43_14_545 "MMS_PARSE: MmsLibHttpErrorInd, recv mms occurs error!"
#define MMMSHTTP_1098_112_2_18_0_43_14_546 "MMS_PARSE: MmsLibHttpErrorInd, recv rsp mms occurs error!"
#define MMMSHTTP_1104_112_2_18_0_43_14_547 "MMS_PARSE: MmsLibHttpErrorInd, has called http/wsp cancel request function,do nothing!"
#define MMMSHTTP_1109_112_2_18_0_43_14_548 "MMS_PARSE: MmsLibHttpErrorInd, state error!"
#define MMMSHTTP_1130_112_2_18_0_43_14_549 "MMS_PARSE:SendRecvFail, get current opt= %d, need retry = %d"
#define MMMSHTTP_1135_112_2_18_0_43_15_550 "MMS_PARSE:SendRecvFail, send mms error!"
#define MMMSHTTP_1139_112_2_18_0_43_15_551 "MMS_PARSE:SendRecvFail,fatal error, need retry = %d,tryed = %d, max= %d!"
#define MMMSHTTP_1149_112_2_18_0_43_15_552 "MMS_PARSE:SendRecvFail, send mms error! http call fail"
#define MMMSHTTP_1158_112_2_18_0_43_15_553 "MMS_PARSE:SendRecvFail, send mms error! wsp call fail"
#define MMMSHTTP_1165_112_2_18_0_43_15_554 "MMS_PARSE:SendRecvFail, send mms error! net type error"
#define MMMSHTTP_1180_112_2_18_0_43_15_555 "MMS_PARSE: SendRecvFail, inform app retrying sending , times = %d!"
#define MMMSHTTP_1188_112_2_18_0_43_15_556 "MMS_PARSE:SendRecvFail, recv mms error!"
#define MMMSHTTP_1192_112_2_18_0_43_15_557 "MMS_PARSE:SendFail, fatal error,need_retry= %d,tryed = %d, max= %d!"
#define MMMSHTTP_1202_112_2_18_0_43_15_558 "MMS_PARSE:SendRecvFail, recv mms error! http call fail"
#define MMMSHTTP_1211_112_2_18_0_43_15_559 "MMS_PARSE:SendRecvFail, recv mms error! wsp call fail"
#define MMMSHTTP_1218_112_2_18_0_43_15_560 "MMS_PARSE:SendRecvFail, recv mms error! net type error"
#define MMMSHTTP_1242_112_2_18_0_43_15_561 "MMS_PARSE: SendRecvFail, inform app retrying receiving , times = %d!"
#define MMMSHTTP_1249_112_2_18_0_43_15_562 "MMS_PARSE: SendRecvFail, state error!"
#define MMMSHTTP_1282_112_2_18_0_43_15_563 "MMS_PARSE: MmsLibNetAuthInd request does not match!"
#define MMMSHTTP_1286_112_2_18_0_43_15_564 "MMS_PARSE: MmsLibNetAuthInd, auth type = %d"
#define MMMSHTTP_1293_112_2_18_0_43_15_565 "MMS_PARSE: MmsLibNetAuthInd, send mms enter close request!"
#define MMMSHTTP_1298_112_2_18_0_43_15_566 "MMS_PARSE: MmsLibNetAuthInd, recv mms need auth fail!"
#define MMMSHTTP_1303_112_2_18_0_43_15_567 "MMS_PARSE: MmsLibNetAuthInd, recv rsp occurs error!"
#define MMMSHTTP_1309_112_2_18_0_43_15_568 "MMS_PARSE: MmsLibNetAuthInd, state error!"
#define MMMSHTTP_1340_112_2_18_0_43_15_569 "MMS_PARSE:MmsLibHttpGetCnf,input invalid!"
#define MMMSHTTP_1350_112_2_18_0_43_15_570 "MMS_PARSE: MmsLibHttpGetCnf request does not match!, cnf request =%d"
#define MMMSHTTP_1354_112_2_18_0_43_15_571 "MMS_PARSE:MmsLibHttpGetCnf,opt =%d"
#define MMMSHTTP_1362_112_2_18_0_43_15_572 "MMS_PARSE:MmsLibHttpGetCnf,result =%d"
#define MMMSHTTP_1377_112_2_18_0_43_15_573 "MMS_PARSE:MmsLibHttpGetCnf, buffer alloc error!"
#define MMMSHTTP_1394_112_2_18_0_43_15_574 "MMS_PARSE:MmsLibHttpGetCnf, get header from sys fail!"
#define MMMSHTTP_1408_112_2_18_0_43_15_575 "MMS_PARSE:MmsLibHttpGetCnf, recv is not mms!"
#define MMMSHTTP_1460_112_2_18_0_43_15_576 "MMS_PARSE:MmsLibHttpGetCnf, create mms_doc_resp fail!"
#define MMMSHTTP_1477_112_2_18_0_43_15_577 "MMS_PARSE:MmsLibHttpGetCnf, post rsp to server!"
#define MMMSHTTP_1538_112_2_18_0_43_15_578 "MMS_PARSE:MmsLibHttpGetCnf, wsp post rsp to server!"
#define MMMSHTTP_1546_112_2_18_0_43_15_579 "MMS_PARSE: MmsLibHttpGetCnf, get cnf error enter close request!"
#define MMMSHTTP_1555_112_2_18_0_43_15_580 "MMS_PARSE: MmsLibHttpGetCnf, handle occurs error!"
#define MMMSHTTP_1612_112_2_18_0_43_15_581 "MMS_PARSE: MmsLibHttpCancelCnf request does not match!"
#define MMMSHTTP_1616_112_2_18_0_43_16_582 "MMS_PARSE:MmsLibHttpGetCnf,opt =%d"
#define MMMSHTTP_1620_112_2_18_0_43_16_583 "MMS_PARSE: MmsLibHttpCancelCnf, get recv cancel cnf!"
#define MMMSHTTP_1625_112_2_18_0_43_16_584 "MMS_PARSE: MmsLibHttpCancelCnf, get send cancel cnf!"
#define MMMSHTTP_1743_112_2_18_0_43_16_585 "MMS_PARSE: StartHttpMmsAction, enter http get!"
#define MMMSHTTP_1748_112_2_18_0_43_16_586 "MMS_PARSE:StartHttpMmsAction opt unknown!"
#define MMMSHTTP_1793_112_2_18_0_43_16_587 "MMS_PARSE: StartWspMmsAction, enter wsp post!,size = %d"
#define MMMSHTTP_1818_112_2_18_0_43_16_588 "MMS_PARSE: StartWspMmsAction, enter wsp get!,url = %s"
#define MMMSHTTP_1823_112_2_18_0_43_16_589 "MMS_PARSE:StartWspMmsAction opt unknown!"
#define MMMSMAIN_91_112_2_18_0_43_16_590 "MMS_PARSE: mmsAddNewAddr : addr list1 : %s"
#define MMMSMAIN_146_112_2_18_0_43_16_591 "MMS_PARSE: mmsParseRecepientString : addr too int64 , cut "
#define MMMSMAIN_155_112_2_18_0_43_16_592 "MMS_PARSE: mmsParseRecepientString : too many addr , ignore"
#define MMMSMAIN_346_112_2_18_0_43_17_593 "MMS_PARSE: mmsParseManual : get type mpeg4\n"
#define MMMSMAIN_351_112_2_18_0_43_17_594 "MMS_PARSE: mmsParseManual : get type 3gpp\n"
#define MMMSMAIN_356_112_2_18_0_43_17_595 "MMS_PARSE: mmsParseManual : get type video avi\n"
#define MMMSMAIN_622_112_2_18_0_43_17_596 "MMS_PARSE: MMS_DeleteMms, input invalid!"
#define MMMSMAIN_669_112_2_18_0_43_17_597 "MMS_PARSE: MMS_DeleteMms, delete smil file!"
#define MMMSMAIN_987_112_2_18_0_43_18_598 " MMS_PARSE: MMS_SetHeadField : before mmsParseRecepientString"
#define MMMSMAIN_989_112_2_18_0_43_18_599 " MMS_PARSE: MMS_SetHeadField : after mmsParseRecepientString"
#define MMMSMAIN_1018_112_2_18_0_43_18_600 "MMS_PARSE:  MMS_SetHeadField : before mmsParseRecepientString"
#define MMMSMAIN_1020_112_2_18_0_43_18_601 "MMS_PARSE:  MMS_SetHeadField : after mmsParseRecepientString"
#define MMMSMAIN_1066_112_2_18_0_43_18_602 "MMS_PARSE:  MMS_SetHeadField : before mmsCompleteAddress"
#define MMMSMAIN_1068_112_2_18_0_43_18_603 "MMS_PARSE:  MMS_SetHeadField : after mmsCompleteAddress"
#define MMMSMAIN_1130_112_2_18_0_43_18_604 " MMS_SetHeadField : the subject is null"
#define MMMSMAIN_1149_112_2_18_0_43_18_605 "MMS_PARSE:  MMS_SetHeadField : subject len is %d "
#define MMMSMAIN_1184_112_2_18_0_43_18_606 "MMS_PARSE:  MMS_SetHeadField : before mmsParseRecepientString"
#define MMMSMAIN_1187_112_2_18_0_43_18_607 "MMS_PARSE:  MMS_SetHeadField : after mmsParseRecepientString"
#define MMMSMAIN_1468_112_2_18_0_43_19_608 "MMS_PARSE:  MMS_GetHeadField : return -1 "
#define MMMSMAIN_1478_112_2_18_0_43_19_609 "MMS_PARSE:  MMS_GetHeadField : first < 1f and ea "
#define MMMSMAIN_1491_112_2_18_0_43_19_610 "MMS_PARSE:  MMS_GetHeadField : real_uni_len = %d"
#define MMMSMAIN_1527_112_2_18_0_43_19_611 "MMS_PARSE:  MMS_GetHeadField : real_uni_len = %d"
#define MMMSMAIN_1548_112_2_18_0_43_19_612 "MMS_PARSE:  MMS_GetHeadField : MMS_SUBJECT::: first == 1f and 3333 ea "
#define MMMSMAIN_1573_112_2_18_0_43_19_613 "MMS_PARSE:  MMS_GetHeadField : real_uni_len = %d"
#define MMMSMAIN_1592_112_2_18_0_43_19_614 "MMS_PARSE:  MMS_GetHeadField : MMS_SUBJECT::: else "
#define MMMSMAIN_1598_112_2_18_0_43_19_615 "MMS_PARSE:  MMS_GetHeadField : real_uni_len = %d"
#define MMMSMAIN_1626_112_2_18_0_43_19_616 "MMS_PARSE:  MMS_GetHeadField : in to len = %d"
#define MMMSMAIN_1644_112_2_18_0_43_19_617 "MMS_PARSE:  MMS_GetHeadField : in to before mmsStrcat"
#define MMMSMAIN_1719_112_2_18_0_43_19_618 "MMS_PARSE:  mmsAddMmsContent : return 0 "
#define MMMSMAIN_1722_112_2_18_0_43_19_619 "MMS_PARSE:  mmsAddMmsContent : pList1->filename len = %d"
#define MMMSMAIN_1866_112_2_18_0_43_20_620 " MMS_PARSE: MMS_DecodeMms ,decode error!"
#define MMMSMAIN_1871_112_2_18_0_43_20_621 " MMS_PARSE: MMS_DecodeMms ,decode OK!"
#define MMMSMAIN_1899_112_2_18_0_43_20_622 "MMS_PARSE: MMS_GetHeaderFromSysFile, input error!"
#define MMMSMAIN_1907_112_2_18_0_43_20_623 "MMS_PARSE: MMS_GetHeaderFromSysFile, create file error!"
#define MMMSMAIN_1914_112_2_18_0_43_20_624 "MMS_PARSE: MMS_GetHeaderFromSysFile, create file error!"
#define MMMSMAIN_1925_112_2_18_0_43_20_625 "MMS_PARSE: MMS_GetHeaderFromSysFile, read file error!"
#define MMMSMAIN_1971_112_2_18_0_43_20_626 "MMS_PARSE: MMS_DecodeMmsHead open decode file failed! "
#define MMMSMAIN_1979_112_2_18_0_43_20_627 "MMS_PARSE: MMS_DecodeMmsHead malloc failed"
#define MMMSMAIN_2000_112_2_18_0_43_20_628 "MMS_PARSE: MMS_DecodeMmsHead,mms_cfa mmsfile size is :%d"
#define MMMSMAIN_2003_112_2_18_0_43_20_629 "MMS_PARSE: MMS_DecodeMmsHead pMmsDecoder->m_long <5000"
#define MMMSMAIN_2007_112_2_18_0_43_20_630 "MMS_PARSE: MMS_DecodeMmsHead,DOCTYPE wml PUBLIC"
#define MMMSMAIN_2054_112_2_18_0_43_20_631 "MMS_PARSE:MMS_SendRecvInit, mms lib status =%d!/n"
#define MMMSMAIN_2058_112_2_18_0_43_20_632 "MMS_PARSE:MMS_SendRecvInit, enter!/n"
#define MMMSMAIN_2063_112_2_18_0_43_20_633 "MMS_PARSE:MMS_SendRecvInit, mms centry error!"
#define MMMSMAIN_2150_112_2_18_0_43_20_634 "MMS_PARSE:MMS_SendRecvInit, wsp no proxy error!"
#define MMMSMAIN_2407_112_2_18_0_43_21_635 "MMS_PARSE:MMS_SendRecvClose, enter close mms lib"
#define MMMSMAIN_2431_112_2_18_0_43_21_636 "MMS_PARSE: MMS_Send input param ptr error!"
#define MMMSMAIN_2436_112_2_18_0_43_21_637 "MMS_PARSE: MMS_Send input param vf file name error!"
#define MMMSMAIN_2442_112_2_18_0_43_21_638 "MMS_PARSE: MMS_Send input param send max time error!"
#define MMMSMAIN_2446_112_2_18_0_43_21_639 "MMS_PARSE: MMS_Send enter,send name = %s, cnf name = %s"
#define MMMSMAIN_2506_112_2_18_0_43_21_640 "MMS_PARSE:  MMS_Send:call http/wsp to send!"
#define MMMSMAIN_2529_112_2_18_0_43_21_641 "MMS_PARSE: MMS_ReceiveByFile, input invalid!"
#define MMMSMAIN_2535_112_2_18_0_43_21_642 "MMS_PARSE: MMS_Receive input param send max time error!"
#define MMMSMAIN_2539_112_2_18_0_43_21_643 "MMS_PARSE: MMS_ReceiveBy,VFFile name =%s"
#define MMMSMAIN_2604_112_2_18_0_43_21_644 "MMS_PARSE: MMS_CancelSendRecv, cancel action"
#define MMMSMAIN_2609_112_2_18_0_43_21_645 "MMS_PARSE:  MMS_CancelSendRecv, mms lib is not busy"
#define MMMSMAIN_2660_112_2_18_0_43_21_646 "MMS_PARSE:GetSuffixByContentType, Content type = %s"
#define MMMSMAIN_3108_112_2_18_0_43_22_647 "MMS_PARSE:  mmsStoreContent : length = %d"
#define MMMSMAIN_3119_112_2_18_0_43_22_648 "MMS_PARSE:  mmsStoreContent : pBuffer is null"
#define MMMSMAIN_3125_112_2_18_0_43_22_649 "MMS_PARSE:  mmsStoreContent : length:%d"
#define MMMSMAIN_3178_112_2_18_0_43_22_650 "MMS_PARSE: access mmsStoreContent type is unknown, add no suffix"
#define MMMSMAIN_3207_112_2_18_0_43_22_651 "MMS_PARSE: mmsStoreContent file has exist = %s"
#define MMMSMAIN_3223_112_2_18_0_43_22_652 "MMS_PARSE: mmsStoreContent new file name = %s"
#define MMMSMAIN_3352_112_2_18_0_43_23_653 "MMS_PARSE:  mmsStoreContent : pCon->type = %d"
#define MMMSMAIN_3413_112_2_18_0_43_23_654 "MMS_PARSE:  mmsStoreContent : pList->char_set is %d"
#define MMMSMAIN_3664_112_2_18_0_43_23_655 "MMS_PARSE:  MMS_ParsePushMsg : call start"
#define MMMSMAIN_3666_112_2_18_0_43_23_656 "MMS_PARSE:  MMS_ParsePushMsg : call end"
#define MMMSMAIN_3673_112_2_18_0_43_23_657 "MMS_PARSE:  MMS_ParsePushMsg : data_len = %d"
#define MMMSMAIN_3676_112_2_18_0_43_23_658 "MMS_PARSE:  MMS_ParsePushMsg : is_exist_user_head = %d"
#define MMMSMAIN_3689_112_2_18_0_43_23_659 "MMS_PARSE:  MMS_ParsePushMsg : ***sms message is not mms notification!!"
#define MMMSMAIN_3725_112_2_18_0_43_23_660 "MMS_PARSE:  mmsOnPushIndication : mmsVFopen begin"
#define MMMSMAIN_3727_112_2_18_0_43_23_661 "MMS_PARSE:  mmsOnPushIndication : mmsVFopen end"
#define MMMSMAIN_3739_112_2_18_0_43_23_662 "MMS_PARSE: mmsOnPushIndication : mmsOnPushIndicate"
#define MMMSMAIN_3828_112_2_18_0_43_24_663 "MMS_PARSE: mmsNameIdMapperGetbycid : pTemp->cid=%s"
#define MMMSMAIN_3945_112_2_18_0_43_24_664 "MMS_PARSE:mmsAddAccessory, input invalid!"
#define MMMSMAIN_4108_112_2_18_0_43_24_665 "MMS_PARSE: MMS_ReAlloc enter, input invalid!"
#define MMMSMAIN_4144_112_2_18_0_43_24_666 "MMS_PARSE: MMS_MemAlloc enter, no function!"
#define MMMSMAIN_4161_112_2_18_0_43_24_667 "MMS_PARSE: MMS_MemFree enter, no function!"
#define MMMSMAIN_4390_112_2_18_0_43_25_668 "GUI_UTF82UCS2B: i %d char %x"
#define MMMSNETWORK_106_112_2_18_0_43_25_669 "MMS_PARSE: mmsOnNotiIndicate : mms lib *** mms_urs is %s"
#define MMMSNETWORK_123_112_2_18_0_43_25_670 "MMS_PARSE:mmsOnNotiIndicate, url= %s, msg size = %d"
#define MMMSNETWORK_209_112_2_18_0_43_25_671 "MMS_PARSE: mmsOnPushIndicate : it is a dilivery indicate message"
#define MMMSNETWORK_214_112_2_18_0_43_25_672 "MMS_PARSE: mmsOnPushIndicate : it is a notification indicate message"
#define MMMSNETWORK_223_112_2_18_0_43_25_673 "MMS_PARSE: mmsOnPushIndicate : it is a invalid message"
#define MMMSPARSER_2138_112_2_18_0_43_30_674 "MMS_PARSE: mmsParseFile : before parse mmsParseBlock"
#define MMMSPARSER_2181_112_2_18_0_43_30_675 "MMS_PARSE: mmsParse : mmsParserInit"
#define MMMSPARSER_2184_112_2_18_0_43_30_676 "MMS_PARSE: mmsParse :  after parse mmsParseFile"
#define MMMSSMIL_1253_112_2_18_0_43_33_677 "MMS_PARSE: mmsSmilParseText : before call mmsGetEncodedLen ,len = %d,charset = %d"
#define MMMSSMIL_1260_112_2_18_0_43_33_678 "MMS_PARSE: mmsSmilParseText : after call mmsGetEncodedLen, len = %d "
#define MMMSSMIL_1444_112_2_18_0_43_33_679 "MMS_PARSE: mmsSmilParseSub : SMIL_EL_AUDIO"
#define MMMSSMIL_1482_112_2_18_0_43_33_680 "MMS_PARSE: mmsSmilParse : before smil mmsParse"
#define MMMSSMIL_1484_112_2_18_0_43_33_681 "MMS_PARSE: mmsSmilParse : after smil mmsParse"
#define MMMSSMIL_1486_112_2_18_0_43_33_682 "MMS_PARSE: mmsSmilParse : after smil mmsSmilParseSub"
#define MMMSSMIL_1932_112_2_18_0_43_34_683 "MMS_PARSE: mmsToSmil access MMS_IntegrateMmsBody"
#define MMMSSMIL_2195_112_2_18_0_43_35_684 "MMS_PARSE: MMS_IntegrateMmsBody : mpeg4 type MMSSmil.c \n"
#define MMMSSMIL_2200_112_2_18_0_43_35_685 "MMS_PARSE: MMS_IntegrateMmsBody : 3gpp type MMSSmil.c \n"
#define MMMSSMIL_2205_112_2_18_0_43_35_686 "MMS_PARSE: MMS_IntegrateMmsBody : avi type MMSSmil.c \n"
#define MMMSSMIL_2254_112_2_18_0_43_35_687 "MMS_PARSE: MMS_IntegrateMmsBody :  while branch"
#define MMMSSMIL_2291_112_2_18_0_43_35_688 "MMS_PARSE: MMS_GetEditBody, input invalid!"
#define MMMSSMIL_2308_112_2_18_0_43_35_689 "MMS_PARSE: MMS_GetEditBody ftype==1 "
#define MMMSSMIL_2311_112_2_18_0_43_35_690 "MMS_PARSE: MMS_GetEditBody : num is %d"
#define MMMSSMIL_2318_112_2_18_0_43_35_691 "MMS_PARSE: MMS_GetEditBody, mms type invalid!"
#define MMMSSMIL_2322_112_2_18_0_43_35_692 "MMS_PARSE: MMS_GetEditBody : before mmsStoreContent"
#define MMMSSMIL_2324_112_2_18_0_43_35_693 "MMS_PARSE: MMS_GetEditBody : after store mms"
#define MMMSSMIL_2428_112_2_18_0_43_35_694 "MMS_PARSE: MMS_GetEditBody : no smil and only accessory!"
#define MMMSSMIL_2436_112_2_18_0_43_35_695 "MMS_PARSE: MMS_GetEditBody : ftype==1 "
#define MMMSSMIL_2439_112_2_18_0_43_35_696 "MMS_PARSE: MMS_GetEditBody : ftype is %d"
#define MMMSSMIL_2444_112_2_18_0_43_35_697 "MMS_PARSE: MMS_GetEditBody : after smil parse"
#define MMMSSMIL_2461_112_2_18_0_43_35_698 "MMS_PARSE: MMS_GetEditBody :  reparse mms manually"
#define MMMSSMIL_2466_112_2_18_0_43_35_699 "MMS_PARSE: MMS_GetEditBody : before mmsSetEdocDrm"
#define MMMSSMIL_2470_112_2_18_0_43_35_700 "MMS_PARSE: MMS_GetEditBody : before MMS_DeleteMms"
#define MMMSSMIL_2474_112_2_18_0_43_35_701 "MMS_PARSE: MMS_GetEditBody : before mmsSetContentLayout"
#define SYSMEMFILE_86_112_2_18_0_43_36_702 "MMS_PARSE: sysVFileInitInfo, init info =%x"
#define SYSMEMFILE_91_112_2_18_0_43_36_703 "MMS_PARSE: sysVFileInitInfo, init failed!"
#define SYSMEMFILE_199_112_2_18_0_43_36_704 "MMS_PARSE: sysVFileDelNode : has no content or the node to be delete"
#define SYSMEMFILE_207_112_2_18_0_43_36_705 "MMS_PARSE: sysVFileDelNode :no file in list!"
#define SYSMEMFILE_241_112_2_18_0_43_36_706 "MMS_PARSE: sysVFileDelNode : not found "
#define SYSMEMFILE_364_112_2_18_0_43_36_707 "MMS_PARSE: sysVFopen : open file name = %s"
#define SYSMEMFILE_377_112_2_18_0_43_36_708 "MMS_PARSE: sysVFopen : FileName is too long"
#define SYSMEMFILE_402_112_2_18_0_43_36_709 "MMS_PARSE: sysVFopen : file %s already exists"
#define SYSMEMFILE_420_112_2_18_0_43_36_710 "MMS_PARSE: sysVFopen : FileName is too long"
#define SYSMEMFILE_431_112_2_18_0_43_36_711 "MMS_PARSE: sysVFopen : add new node fial!"
#define SYSMEMFILE_448_112_2_18_0_43_36_712 "MMS_PARSE: sysVFopen : File %s not found"
#define SYSMEMFILE_490_112_2_18_0_43_36_713 "MMS_PARSE: sysVFread  : WarningFile %d not found"
#define SYSMEMFILE_543_112_2_18_0_43_36_714 "MMS_PARSE: sysVFwrite : Memory error!"
#define SYSMEMFILE_553_112_2_18_0_43_36_715 "MMS_PARSE: sysVFwrite : File %d not found"
#define SYSMEMFILE_582_112_2_18_0_43_37_716 "MMS_PARSE: sysVFseek : unknown seek origin %d"
#define SYSMEMFILE_588_112_2_18_0_43_37_717 "MMS_PARSE: sysVFseek : File %d not found!"
#define SYSMEMFILE_618_112_2_18_0_43_37_718 "MMS_PARSE: Waringing sysVFtell : File %d not found!\n"
#define SYSMEMFILE_637_112_2_18_0_43_37_719 "MMS_PARSE: sysVFremove : remove file name = %s"
#define SYSMEMFILE_645_112_2_18_0_43_37_720 "MMS_PARSE: sysVFremove : File %s not found"
#define SYSMEMFILE_661_112_2_18_0_43_37_721 "MMMS_PARSE: sysVFSetBufPointer enter,name = %s, size =%d"
#define SYSMEMFILE_669_112_2_18_0_43_37_722 "MMMS_PARSE: sysVFSetBufPointer, memory alloc fail!"
#define SYSMEMFILE_681_112_2_18_0_43_37_723 "MMMS_PARSE: sysVFSetBufPointer, error tmp null! name = %s, size =%d"
#define SYSMEMFILE_697_112_2_18_0_43_37_724 "MMS_PARSE: sysVFRemoveByHandle : file handle %d not found"
#define SYSMEMFILE_716_112_2_18_0_43_37_725 "MMS_PARSE: sysVFclose : file %d not found"
#define SYSMEMFILE_735_112_2_18_0_43_37_726 "MMS_PARSE: setfiledata the data is not NULL"
#define SYSMEMFILE_739_112_2_18_0_43_37_727 "MMS_PARSE: setfiledata the data is NULL"
#define SYSMEMFILE_889_112_2_18_0_43_37_728 "MMS_PARSE: sysVFGetFileInfo : wrong parma"
#define SYSMEMFILE_905_112_2_18_0_43_37_729 "MMS_PARSE: sysVFGetFileInfo : error ,open file "
#define SYSMEMFILE_918_112_2_18_0_43_37_730 "MMS_PARSE: sysVFCreateFile : input invalid!"
#define SYSMEMFILE_925_112_2_18_0_43_37_731 "MMS_PARSE: sysVFCreateFile : file %s already exists"
#define SYSMEMFILE_935_112_2_18_0_43_37_732 "MMS_PARSE: sysVFCreateFile : FileName is too long"
#define SYSMEMFILE_963_112_2_18_0_43_37_733 "MMS_PARSE: sysVFDestroyFile : input invalid!"
#define SYSMEMFILE_976_112_2_18_0_43_37_734 "MMS_PARSE: sysVFDestroyFile : File %s not found"
#define SYSMEMFILE_985_112_2_18_0_43_37_735 "MMS_PARSE: Access sysVFGetBufPointer in"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(PARSER_TRC)
TRACE_MSG(BRW_CORE_API_66_112_2_18_0_36_32_0,"[BRW]BrwGetImgUrl image_ptr is null")
TRACE_MSG(BRW_CORE_API_90_112_2_18_0_36_32_1,"[BRW]BrwGetImgUrl:BrwLayout failed")
TRACE_MSG(BRW_CORE_API_127_112_2_18_0_36_32_2,"[BRW]BrwGetImgUrl:BrwLayout failed")
TRACE_MSG(BRW_CORE_API_175_112_2_18_0_36_32_3,"[BRW]BrwGetInputImgUrl: image_ptr is null")
TRACE_MSG(BRW_CORE_API_276_112_2_18_0_36_32_4,"[BRW]BrwUpdateImgMimeType use mime %d")
TRACE_MSG(BRW_CORE_API_317_112_2_18_0_36_32_5,"[BRW]BrwSetImgBuf:data_len =%d, mime =%d")
TRACE_MSG(BRW_CORE_API_359_112_2_18_0_36_32_6,"[BRW]BrwSetImgBuf:image_status =%d")
TRACE_MSG(BRW_CORE_API_396_112_2_18_0_36_32_7,"[BRW]BrwSetImgBuf:image_status =%d")
TRACE_MSG(BRW_CORE_API_401_112_2_18_0_36_32_8,"[BRW]BrwSetImgBuf:is_calc_ok =%d")
TRACE_MSG(BRW_CORE_API_657_112_2_18_0_36_33_9,"BrwParseWebPage input invaild!")
TRACE_MSG(BRW_CORE_API_663_112_2_18_0_36_33_10,"BrwParseWebPage: BrwSetParserInfo return false!")
TRACE_MSG(BRW_CORE_API_673_112_2_18_0_36_33_11,"BrwParseWebPage: BrwSetLayoutInfo return false!")
TRACE_MSG(BRW_CORE_API_688_112_2_18_0_36_33_12,"BrwParseWebPage: BRW_MIME_PLAINTEXT calloc fail!")
TRACE_MSG(BRW_CORE_API_704_112_2_18_0_36_33_13,"BrwParseWebPage: BrwParse return %d!")
TRACE_MSG(BRW_CORE_API_752_112_2_18_0_36_33_14,"BRW_ParseBufConstructor: create ctrl return false!")
TRACE_MSG(BRW_CORE_API_759_112_2_18_0_36_33_15,"BRW_ParseBufConstructor: BrwSetParserBufInfo return false!")
TRACE_MSG(BRW_CORE_API_765_112_2_18_0_36_33_16,"BRW_ParseBufConstructor: BrwParse return %d!")
TRACE_MSG(BRW_CORE_API_871_112_2_18_0_36_34_17,"[BRW]BrwSetLayoutInfo error")
TRACE_MSG(BRW_CORE_API_1202_112_2_18_0_36_34_18,"BRW: no layout when timer come\n")
TRACE_MSG(BRW_CORE_API_1208_112_2_18_0_36_34_19,"BRW: unkown timer id\n")
TRACE_MSG(BRW_CORE_API_1252_112_2_18_0_36_34_20,"BRW: no layout when timer come\n")
TRACE_MSG(BRW_CORE_API_1495_112_2_18_0_36_35_21,"[BRW]BrwDestroyLayoutCtrl file:%s, line:%d")
TRACE_MSG(BRW_CORE_API_1545_112_2_18_0_36_35_22,"[BRW]BrwProcTpScroll wml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_1554_112_2_18_0_36_35_23,"[BRW]BrwProcTpScroll xhtml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_1868_112_2_18_0_36_36_24,"[BRW]BrwSetBgsoundBuf in param error")
TRACE_MSG(BRW_CORE_API_1875_112_2_18_0_36_36_25,"[BRW]BrwSetBgsoundBuf mime_type error")
TRACE_MSG(BRW_CORE_API_1885_112_2_18_0_36_36_26,"[BRW]BrwSetBgsoundBuf bgsound_ptr is null")
TRACE_MSG(BRW_CORE_API_1895_112_2_18_0_36_36_27,"[BRW]BrwSetBgsoundBuf:status =%d\tdata_len =%d\tmime_type =%d")
TRACE_MSG(BRW_CORE_API_1903_112_2_18_0_36_36_28,"[BRW]BrwSetBgsoundBuf bgsound_ptr is null")
TRACE_MSG(BRW_CORE_API_1913_112_2_18_0_36_36_29,"[BRW]BrwSetBgsoundBuf:status =%d\tdata_len =%d\tmime_type =%d")
TRACE_MSG(BRW_CORE_API_1918_112_2_18_0_36_36_30,"[BRW]BrwSetBgsoundBuf bgsound_ptr is null OR content_type error:%d\n")
TRACE_MSG(BRW_CORE_API_1962_112_2_18_0_36_36_31,"[BRW]BrwGetBgsoundBuf in param error")
TRACE_MSG(BRW_CORE_API_1997_112_2_18_0_36_36_32,"[BRW]BrwGetBgsoundBuf error")
TRACE_MSG(BRW_CORE_API_2036_112_2_18_0_36_36_33,"[brw]now BrwGetBgsoundUrl\n")
TRACE_MSG(BRW_CORE_API_2040_112_2_18_0_36_36_34,"[BRW]BrwGetBgsoundUrl in param error")
TRACE_MSG(BRW_CORE_API_2051_112_2_18_0_36_36_35,"[BRW]BrwGetBgsoundUrl bgsound_ptr is null")
TRACE_MSG(BRW_CORE_API_2078_112_2_18_0_36_36_36,"[BRW]BrwGetBgsoundUrl bgsound_ptr is null")
TRACE_MSG(BRW_CORE_API_2130_112_2_18_0_36_36_37,"[BRW]BrwFreeBgsoundBuf in param error")
TRACE_MSG(BRW_CORE_API_3232_112_2_18_0_36_38_38,"[BRW]BrwLayoutPageUp input error")
TRACE_MSG(BRW_CORE_API_3240_112_2_18_0_36_38_39,"[BRW]BrwLayoutPageUp wml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3248_112_2_18_0_36_38_40,"[BRW]BrwLayoutPageUp xhtml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3277_112_2_18_0_36_38_41,"[BRW]BrwLayoutPageDown input error")
TRACE_MSG(BRW_CORE_API_3285_112_2_18_0_36_38_42,"[BRW]BrwLayoutPageDown wml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3293_112_2_18_0_36_38_43,"[BRW]BrwLayoutPageDown xhtml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3323_112_2_18_0_36_39_44,"[BRW]BrwLayoutLineUp input error")
TRACE_MSG(BRW_CORE_API_3331_112_2_18_0_36_39_45,"[BRW]BrwLayoutLineUp wml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3339_112_2_18_0_36_39_46,"[BRW]BrwLayoutLineUp xhtml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3368_112_2_18_0_36_39_47,"[BRW]BrwLayoutLineDown input error")
TRACE_MSG(BRW_CORE_API_3376_112_2_18_0_36_39_48,"[BRW]BrwLayoutLineDown wml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3384_112_2_18_0_36_39_49,"[BRW]BrwLayoutLineDown xhtml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3414_112_2_18_0_36_39_50,"[BRW]BrwLayoutMoveFocused input error")
TRACE_MSG(BRW_CORE_API_3422_112_2_18_0_36_39_51,"[BRW]BrwLayoutMoveFocused wml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3430_112_2_18_0_36_39_52,"[BRW]BrwLayoutMoveFocused xhtml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3463_112_2_18_0_36_39_53,"[BRW]BrwLayoutMoveVertical input error")
TRACE_MSG(BRW_CORE_API_3471_112_2_18_0_36_39_54,"[BRW]BrwLayoutMoveVertical wml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3479_112_2_18_0_36_39_55,"[BRW]BrwLayoutMoveVertical xhtml_ctrl_ptr error")
TRACE_MSG(BRW_CORE_API_3573_112_2_18_0_36_39_56,"[BRW]BrwGetFocusImageInfo IMG status= %d")
TRACE_MSG(BRW_CORE_API_3590_112_2_18_0_36_39_57,"[BRW]BrwGetFocusImageInfo cur_card_ptr==%p")
TRACE_MSG(BRW_CORE_API_3595_112_2_18_0_36_39_58,"[BRW]BrwGetFocusImageInfo wml_ctrl_ptr==%p")
TRACE_MSG(BRW_CORE_API_3615_112_2_18_0_36_39_59,"[BRW]BrwGetFocusImageInfo IMG status= %d")
TRACE_MSG(BRW_CORE_API_3630_112_2_18_0_36_39_60,"[BRW]BrwGetFocusImageInfo xhtml_ctrl_ptr==%p")
TRACE_MSG(BRW_CORE_API_3686_112_2_18_0_36_39_61,"[BRW]BrwGetTpPressImageInfo xhtml_ctrl_ptr==%p")
TRACE_MSG(BRW_CORE_API_3707_112_2_18_0_36_39_62,"[BRW]BrwGetTpPressImageInfo wml_ctrl_ptr==%p")
TRACE_MSG(BRW_CORE_API_3780_112_2_18_0_36_39_63,"[BRW]BrwGetFocusImgBuf focused_hotlink_ptr==%p")
TRACE_MSG(BRW_CORE_API_3785_112_2_18_0_36_39_64,"[BRW]BrwGetFocusImgBuf xhtml_ctrl_ptr==%p")
TRACE_MSG(BRW_CORE_API_3819_112_2_18_0_36_40_65,"[BRW]BrwGetFocusImgBuf focused_hotlink_ptr==%p")
TRACE_MSG(BRW_CORE_API_3824_112_2_18_0_36_40_66,"[BRW]BrwGetFocusImgBuf cur_card_ptr==%p")
TRACE_MSG(BRW_CORE_API_3829_112_2_18_0_36_40_67,"[BRW]BrwGetFocusImgBuf wml_ctrl_ptr==%p")
TRACE_MSG(BRW_CSS_131_112_2_18_0_36_40_68,"BRWCSS_Init")
TRACE_MSG(BRW_CSS_136_112_2_18_0_36_40_69,"BRWCSS_Init lwc_initialise: %i\n")
TRACE_MSG(BRW_CSS_146_112_2_18_0_36_40_70,"BRWCSS_Init css_initialise code: %d")
TRACE_MSG(BRW_CSS_155_112_2_18_0_36_40_71,"BRWCSS_INIT css_stylesheet_create error!")
TRACE_MSG(BRW_CSS_166_112_2_18_0_36_40_72,"BRWCSS_INIT css_stylesheet_append_data error!")
TRACE_MSG(BRW_CSS_174_112_2_18_0_36_40_73,"BRWCSS_INIT css_stylesheet_append_data_done error!")
TRACE_MSG(BRW_CSS_197_112_2_18_0_36_40_74,"BRWCSS_CreateHandle")
TRACE_MSG(BRW_CSS_201_112_2_18_0_36_40_75,"BRWCSS_CreateHandle alloc fail")
TRACE_MSG(BRW_CSS_248_112_2_18_0_36_40_76,"BRWCSS_LoadCss invalid input!")
TRACE_MSG(BRW_CSS_270_112_2_18_0_36_40_77,"BRWCSS_LoadCss css_stylesheet_create error!")
TRACE_MSG(BRW_CSS_288_112_2_18_0_36_40_78,"BRWCSS_LoadCss css_stylesheet_append_data error!")
TRACE_MSG(BRW_CSS_366_112_2_18_0_36_40_79,"BRWCSS_LoadCss css_select_ctx_create error code is %d")
TRACE_MSG(BRW_CSS_376_112_2_18_0_36_40_80,"BRWCSS_LoadCss css_select_ctx_append_sheet error code is %d")
TRACE_MSG(BRW_CSS_392_112_2_18_0_36_40_81,"BRWCSS_LoadCss css_select_ctx_append_sheet error code is %d")
TRACE_MSG(BRW_CSS_439_112_2_18_0_36_41_82,"BRWCSS_Parse css_select_ctx_create error code is %d")
TRACE_MSG(BRW_CSS_451_112_2_18_0_36_41_83,"BRWCSS_Parse css_computed_style_create error code is %d")
TRACE_MSG(BRW_CSS_480_112_2_18_0_36_41_84,"BRWCSS_Parse css_computed_style_compose error code is %d")
TRACE_MSG(BRW_CSS_586_112_2_18_0_36_41_85,"BRWCSS_Deinit!")
TRACE_MSG(BRW_CSS_595_112_2_18_0_36_41_86,"BRWCSS_Deinit error code %d")
TRACE_MSG(BRW_CSS_640_112_2_18_0_36_41_87,"brw ExpatMalloc malloc error")
TRACE_MSG(BRW_CSS_739_112_2_18_0_36_41_88,"XML ExpatRealloc, malloc p2 err!")
TRACE_MSG(BRW_CSS_764_112_2_18_0_36_41_89,"brw BRWMem_ReallocEx malloc fail!")
TRACE_MSG(BRW_CSS_845_112_2_18_0_36_41_90,"brw_css node_name  node == 0 || name == 0")
TRACE_MSG(BRW_CSS_998_112_2_18_0_36_42_91,"named_ancestor_node node_name  node == 0 || name == 0 || ancestor == 0")
TRACE_MSG(BRW_CSS_1039_112_2_18_0_36_42_92,"brw_css named_parent_node  node == 0 || name == 0 || parent == 0")
TRACE_MSG(BRW_CSS_1077_112_2_18_0_36_42_93,"brw_css sibling  node == 0 || name == 0 || sibling == 0")
TRACE_MSG(BRW_CSS_1115_112_2_18_0_36_42_94,"brw_css parent_node  node == 0 || parent == 0")
TRACE_MSG(BRW_CSS_1148_112_2_18_0_36_42_95,"brw_css sibling_node  node == 0 || sibling == 0")
TRACE_MSG(BRW_CSS_1382_112_2_18_0_36_43_96,"brw_css node_is_first_child  node == 0 || match == 0")
TRACE_MSG(BRW_CSS_1406_112_2_18_0_36_43_97,"named_ancestor_node node_name  node == 0 || match == 0")
TRACE_MSG(BRW_CSS_1437_112_2_18_0_36_43_98,"named_ancestor_node node_name  node == 0 || match == 0")
TRACE_MSG(BRW_GLOBAL_105_112_2_18_0_36_45_99,"brw hash alloc buckets fail")
TRACE_MSG(BRW_GLOBAL_407_112_2_18_0_36_45_100,"BRW: unkonw charset: try utf8\n")
TRACE_MSG(BRW_GLOBAL_669_112_2_18_0_36_46_101,"[BRW]BrwUtf16CharToUtf8Buffer input error")
TRACE_MSG(BRW_GLOBAL_1167_112_2_18_0_36_47_102,"Brw UTF8toUtf16 occurs unknown character 1")
TRACE_MSG(BRW_GLOBAL_1180_112_2_18_0_36_47_103,"Brw UTF8toUtf16 occurs unknown character 2")
TRACE_MSG(BRW_GLOBAL_1186_112_2_18_0_36_47_104,"Brw UTF8toUtf16 occurs unknown character 3")
TRACE_MSG(BRW_GLOBAL_1479_112_2_18_0_36_47_105,"[BRW]brwstr_grow input error")
TRACE_MSG(BRW_GLOBAL_1616_112_2_18_0_36_48_106,"[BRW]brwstr_search_char input error")
TRACE_MSG(BRW_GLOBAL_1690_112_2_18_0_36_48_107,"[BRW]brwstr_insert_data input error")
TRACE_MSG(BRW_GLOBAL_1871_112_2_18_0_36_48_108,"[BRW]woctstr_grow input error")
TRACE_MSG(BRW_GLOBAL_1914_112_2_18_0_36_48_109,"[BRW]wbrwstr_insert_data input error")
TRACE_MSG(BRW_GLOBAL_1959_112_2_18_0_36_48_110,"[BRW]wbrwstr_append_data input error")
TRACE_MSG(BRW_GLOBAL_3086_112_2_18_0_36_51_111,"[BRW]RemallocStack error")
TRACE_MSG(BRW_JS_468_112_2_18_0_36_52_112,"BRW BRWJS_Init input error")
TRACE_MSG(BRW_JS_473_112_2_18_0_36_52_113,"BRW BRWJS_Init  context exsists")
TRACE_MSG(BRW_JS_480_112_2_18_0_36_52_114,"BRW BRWJS_Init  init runtime error")
TRACE_MSG(BRW_JS_487_112_2_18_0_36_52_115,"BRW BRWJS_Init  init context error")
TRACE_MSG(BRW_JS_499_112_2_18_0_36_52_116,"BRW BRWJS_Init  init window error")
TRACE_MSG(BRW_JS_537_112_2_18_0_36_52_117,"BrwJs_ErrorHandler input error")
TRACE_MSG(BRW_JS_540_112_2_18_0_36_52_118,"Brw JS error  %s,File: %s:%u")
TRACE_MSG(BRW_JS_574_112_2_18_0_36_52_119,"BRW BRWJS_Execute input error")
TRACE_MSG(BRW_JS_610_112_2_18_0_36_52_120,"BRWJS_Execute result return false")
TRACE_MSG(BRW_JS_632_112_2_18_0_36_53_121,"BRW BRWJS_Execute input error")
TRACE_MSG(BRW_JS_665_112_2_18_0_36_53_122,"BRWJS_Execute result return false")
TRACE_MSG(BRW_JS_682_112_2_18_0_36_53_123,"BRW BRWJS_Destroy input error ")
TRACE_MSG(BRW_JS_692_112_2_18_0_36_53_124,"Brw BRWJS_Destroy runtime NULL")
TRACE_MSG(BRW_LAYOUT_70_112_2_18_0_36_53_125,"[BRW]LayoutCtrlNew error")
TRACE_MSG(BRW_LAYOUT_222_112_2_18_0_36_53_126,"[BRW]BrwLayoutProcessTimerEvent wml_ctrl_ptr error")
TRACE_MSG(BRW_LAYOUT_230_112_2_18_0_36_53_127,"[BRW]BrwLayoutProcessTimerEvent xhtml_ctrl_ptr error")
TRACE_MSG(BRW_PARSER_119_112_2_18_0_36_54_128,"[BRW]BrwXmlParserCreate wml_parser_ptr error")
TRACE_MSG(BRW_PARSER_139_112_2_18_0_36_54_129,"[BRW]BrwXmlParserCreate wml_parser_ptr error")
TRACE_MSG(BRW_PARSER_165_112_2_18_0_36_54_130,"[BRW]BrwXmlParserCreate xhtml_parser_ptr error")
TRACE_MSG(BRW_PARSER_220_112_2_18_0_36_54_131,"BrwXmlParse input invalid!")
TRACE_MSG(BRW_PARSER_234_112_2_18_0_36_54_132,"BrwXmlParse: parser_ptr->is_use_wbxml!")
TRACE_MSG(BRW_PARSER_242_112_2_18_0_36_54_133,"[BRW]BrwXmlParse parser_ptr error")
TRACE_MSG(BRW_PARSER_247_112_2_18_0_36_54_134,"BrwXmlParse SGML_Parse fail!")
TRACE_MSG(BRW_PARSER_277_112_2_18_0_36_54_135,"BrwParse input invalid!")
TRACE_MSG(BRW_PARSER_283_112_2_18_0_36_54_136,"BrwParse BrwXmlParserCreate fail!")
TRACE_MSG(BRW_PARSER_347_112_2_18_0_36_54_137,"[BRW]BrwXmlParserBufCreate xhtml_parser_ptr error")
TRACE_MSG(BRW_PARSER_406_112_2_18_0_36_54_138,"BrwParseBuf input invalid!")
TRACE_MSG(BRW_PARSER_412_112_2_18_0_36_54_139,"BrwParseBuf BrwXmlParserCreate fail!")
TRACE_MSG(BRW_PARSER_462_112_2_18_0_36_55_140,"[BRW]BrwSetParserBufInfo xhtml_ctrl_ptr error")
TRACE_MSG(BRW_PARSER_502_112_2_18_0_36_55_141,"BrwSetParserBufInfo:charset is NULL ")
TRACE_MSG(BRW_PARSER_506_112_2_18_0_36_55_142,"BrwSetParserBufInfo:charset=%s ")
TRACE_MSG(BRW_PARSER_545_112_2_18_0_36_55_143,"[BRW]BrwSetParserInfo wml_ctrl_ptr error")
TRACE_MSG(BRW_PARSER_555_112_2_18_0_36_55_144,"[BRW]BrwSetParserInfo xhtml_ctrl_ptr error")
TRACE_MSG(BRW_PARSER_608_112_2_18_0_36_55_145,"BrwParseWebPage: BrwSetParserInfo charset is NULL ")
TRACE_MSG(BRW_PARSER_612_112_2_18_0_36_55_146,"BrwParseWebPage: BrwSetParserInfo charset=%s ")
TRACE_MSG(BRW_SGML_PARSER_126_112_2_18_0_36_57_147,"brw SGML_Parse input invalid!")
TRACE_MSG(BRW_SGML_PARSER_135_112_2_18_0_36_57_148,"[BRW]SGML_Parse parser member error")
TRACE_MSG(BRW_SGML_PARSER_140_112_2_18_0_36_57_149,"brw SGML_Parse input buf_ptr is NULL or len <= 0!")
TRACE_MSG(BRW_URL_196_112_2_18_0_36_58_150,"BRW_InsertClickURL invalid absoult_url")
TRACE_MSG(BRW_URL_230_112_2_18_0_36_58_151,"BRW_InsertClickURL alloc memory fail")
TRACE_MSG(BRW_URL_251_112_2_18_0_36_59_152,"BRW_IfHasClickedURL invalid input!")
TRACE_MSG(BRW_URL_312_112_2_18_0_36_59_153,"[BRW]URL_MovetoSchemeEnd error")
TRACE_MSG(BRW_URL_392_112_2_18_0_36_59_154,"[BRW]URL_MovetoHostEnd end error")
TRACE_MSG(BRW_URL_701_112_2_18_0_37_0_155,"[BRW]LayoutGetVarList error")
TRACE_MSG(BRW_URL_759_112_2_18_0_37_0_156,"[BRW]WmlPostFieldVdata2URL tv error")
TRACE_MSG(BRW_URL_813_112_2_18_0_37_0_157,"[BRW]WmlPostFieldVdata2URL tv var_type error")
TRACE_MSG(BRW_URL_1558_112_2_18_0_37_2_158,"[BRW]MakeRequestByTaskGo input error")
TRACE_MSG(BRW_URL_1670_112_2_18_0_37_2_159,"[BRW]XhtmlMakeRequestByFrom layout_ctrl_ptr error")
TRACE_MSG(BRW_URL_1694_112_2_18_0_37_2_160,"[BRW]XhtmlMakeRequestByFrom active_ptr error")
TRACE_MSG(BRW_URL_1703_112_2_18_0_37_2_161,"[BRW]XhtmlMakeRequestByFrom input_ptr error")
TRACE_MSG(BRW_URL_1744_112_2_18_0_37_2_162,"XhtmlMakeRequestByFrom alloc file data failed")
TRACE_MSG(BRW_URL_1789_112_2_18_0_37_2_163,"[BRW]XhtmlMakeRequestByFrom button_ptr error")
TRACE_MSG(BRW_URL_1809_112_2_18_0_37_2_164,"[BRW]XhtmlMakeRequestByFrom select_ptr error")
TRACE_MSG(BRW_WML_LAYOUT_246_112_2_18_0_37_3_165,"[BRW]WmlLayoutProcessTimerEvent card_ptr error")
TRACE_MSG(BRW_WML_LAYOUT_258_112_2_18_0_37_3_166,"BRW: card timer event coming")
TRACE_MSG(BRW_WML_LAYOUT_347_112_2_18_0_37_4_167,"[BRW]WmlFindTimerInterval card_ptr error")
TRACE_MSG(BRW_WML_LAYOUT_508_112_2_18_0_37_4_168,"BRW: backward had run in this card")
TRACE_MSG(BRW_WML_LAYOUT_804_112_2_18_0_37_5_169,"[BRW]WmlLayoutLineDownAndMoveFocus input error")
TRACE_MSG(BRW_WML_LAYOUT_894_112_2_18_0_37_5_170,"[BRW]WmlLayoutLineUpAndMoveFocus input error")
TRACE_MSG(BRW_WML_LAYOUT_980_112_2_18_0_37_5_171,"[BRW]WmlLayoutPageUp input error")
TRACE_MSG(BRW_WML_LAYOUT_1049_112_2_18_0_37_5_172,"[BRW]WmlLayoutPageDown input error")
TRACE_MSG(BRW_WML_LAYOUT_1119_112_2_18_0_37_5_173,"[BRW]WmlLayoutMoveVerticalAndMoveFocus input error")
TRACE_MSG(BRW_WML_LAYOUT_1176_112_2_18_0_37_5_174,"[BRW]WmlMoveFocusedLayout input error")
TRACE_MSG(BRW_WML_LAYOUT_1368_112_2_18_0_37_6_175,"BRW: WmlLayoutCalc return false: no card \n")
TRACE_MSG(BRW_WML_LAYOUT_1566_112_2_18_0_37_6_176,"[BRW]WmlLayoutRepaint input error")
TRACE_MSG(BRW_WML_LAYOUT_1576_112_2_18_0_37_6_177,"[BRW]WmlLayoutRepaint  error")
TRACE_MSG(BRW_WML_LAYOUT_1598_112_2_18_0_37_6_178,"[BRW]WmlLayoutRepaint win error")
TRACE_MSG(BRW_WML_LAYOUT_1768_112_2_18_0_37_7_179,"[BRW]WmlSetSelectResult  wml_ctrl_ptr error")
TRACE_MSG(BRW_WML_LAYOUT_1822_112_2_18_0_37_7_180,"[BRW]WmlSetSelectResult  wml_ctrl_ptr error")
TRACE_MSG(BRW_WML_LAYOUT_1896_112_2_18_0_37_7_181,"[BRW]WmlFindHotLinkLayoutByPenPoint  input error")
TRACE_MSG(BRW_WML_LAYOUT_1947_112_2_18_0_37_7_182,"[BRW]WmlFindHotLinkLayoutByPenPoint   error")
TRACE_MSG(BRW_WML_LAYOUT_1960_112_2_18_0_37_7_183,"[BRW]Calc_P  input error")
TRACE_MSG(BRW_WML_LAYOUT_2058_112_2_18_0_37_7_184,"[BRW]Calc_P   error")
TRACE_MSG(BRW_WML_LAYOUT_2261_112_2_18_0_37_8_185,"[BRW]Draw_P  input error")
TRACE_MSG(BRW_WML_LAYOUT_2413_112_2_18_0_37_8_186,"[BRW]Calc_Image  input type error")
TRACE_MSG(BRW_WML_LAYOUT_2513_112_2_18_0_37_8_187,"[BRW]Calc_Image char_width  error")
TRACE_MSG(BRW_WML_LAYOUT_2554_112_2_18_0_37_8_188,"[BRW]Draw_Image  input type error")
TRACE_MSG(BRW_WML_LAYOUT_2628_112_2_18_0_37_8_189,"[BRW]Calc_Text input  error")
TRACE_MSG(BRW_WML_LAYOUT_2645_112_2_18_0_37_8_190,"[BRW]Calc_Text  input type error")
TRACE_MSG(BRW_WML_LAYOUT_2658_112_2_18_0_37_8_191,"[BRW]Calc_Text  error")
TRACE_MSG(BRW_WML_LAYOUT_2689_112_2_18_0_37_9_192,"[BRW]Draw_Text input  error")
TRACE_MSG(BRW_WML_LAYOUT_2764_112_2_18_0_37_9_193,"[BRW]FindOptionByIndex   error")
TRACE_MSG(BRW_WML_LAYOUT_2782_112_2_18_0_37_9_194,"[BRW]TruncateSelectShowingText wml_ctrl_ptr  error")
TRACE_MSG(BRW_WML_LAYOUT_2803_112_2_18_0_37_9_195,"[BRW]Calc_Select layout_type  error")
TRACE_MSG(BRW_WML_LAYOUT_2858_112_2_18_0_37_9_196,"[BRW]Draw_Select layout_type  error")
TRACE_MSG(BRW_WML_LAYOUT_2923_112_2_18_0_37_9_197,"[BRW]Calc_Anchor_A   error")
TRACE_MSG(BRW_WML_LAYOUT_2961_112_2_18_0_37_9_198,"[BRW]Draw_Anchor layout_type  error")
TRACE_MSG(BRW_WML_LAYOUT_3003_112_2_18_0_37_9_199,"[BRW]Draw_A layout_type  error")
TRACE_MSG(BRW_WML_LAYOUT_3050_112_2_18_0_37_9_200,"[BRW]Calc_Do layout_type  error")
TRACE_MSG(BRW_WML_LAYOUT_3089_112_2_18_0_37_9_201,"[BRW]Draw_Do input  error")
TRACE_MSG(BRW_WML_LAYOUT_3140_112_2_18_0_37_9_202,"[BRW]Calc_Input input  error")
TRACE_MSG(BRW_WML_LAYOUT_3193_112_2_18_0_37_10_203,"[BRW]Draw_Input input  error")
TRACE_MSG(BRW_WML_LAYOUT_3261_112_2_18_0_37_10_204,"[BRW]Calc_Fieldset input  error")
TRACE_MSG(BRW_WML_LAYOUT_3305_112_2_18_0_37_10_205,"[BRW]Draw_Fieldset input  error")
TRACE_MSG(BRW_WML_LAYOUT_3835_112_2_18_0_37_11_206,"[BRW]Draw_Td_Children input  error")
TRACE_MSG(BRW_WML_LAYOUT_3907_112_2_18_0_37_11_207,"[BRW]Draw_Table layout_type  error")
TRACE_MSG(BRW_WML_LAYOUT_3995_112_2_18_0_37_11_208,"[BRW]wmlIsAnchorChildInScreen input  error")
TRACE_MSG(BRW_WML_LAYOUT_4087_112_2_18_0_37_11_209,"[BRW]IsAnchorChildClickedByPen input  error")
TRACE_MSG(BRW_WML_LAYOUT_4128_112_2_18_0_37_11_210,"[BRW]WmlSetRectMaxAndMinValue input  error")
TRACE_MSG(BRW_WML_LAYOUT_4151_112_2_18_0_37_11_211,"[BRW]GetHotLinkLayoutChildRect input  error")
TRACE_MSG(BRW_WML_LAYOUT_4169_112_2_18_0_37_11_212,"[BRW]GetHotLinkLayoutChildRect drawtxt_ptr  error")
TRACE_MSG(BRW_WML_LAYOUT_4222_112_2_18_0_37_12_213,"[BRW]GetHotLinkLayoutRect   error")
TRACE_MSG(BRW_WML_LAYOUT_4332_112_2_18_0_37_12_214,"[BRW]WmlFindHotLinkLayoutByPenPoint IMG")
TRACE_MSG(BRW_WML_LAYOUT_4337_112_2_18_0_37_12_215,"[BRW]IsHotLinkLayoutInScreen   error")
TRACE_MSG(BRW_WML_LAYOUT_4676_112_2_18_0_37_12_216,"[BRW]DrawHotLinkLayoutGetFocus wml_ctrl_ptr  error")
TRACE_MSG(BRW_WML_LAYOUT_5058_112_2_18_0_37_13_217,"[BRW]task_go input  error")
TRACE_MSG(BRW_WML_LAYOUT_5069_112_2_18_0_37_13_218,"[BRW]task_go input  error")
TRACE_MSG(BRW_WML_LAYOUT_5081_112_2_18_0_37_13_219,"[BRW]task_prev input  error")
TRACE_MSG(BRW_WML_LAYOUT_5099_112_2_18_0_37_13_220,"[BRW]task_refresh input  error")
TRACE_MSG(BRW_WML_LAYOUT_5109_112_2_18_0_37_13_221,"[BRW]task_refresh wml_ctrl_ptr  error")
TRACE_MSG(BRW_WML_LAYOUT_5113_112_2_18_0_37_13_222,"BRW: wml refresh\n")
TRACE_MSG(BRW_WML_LAYOUT_5149_112_2_18_0_37_13_223,"[BRW]task_refresh wml_ctrl_ptr  error")
TRACE_MSG(BRW_WML_LAYOUT_5158_112_2_18_0_37_13_224,"[BRW]task_refresh wml_ctrl_ptr  error")
TRACE_MSG(BRW_WML_LAYOUT_5267_112_2_18_0_37_14_225,"[BRW]WmlLayoutTpMoveVerticalAndMoveFocus input  error")
TRACE_MSG(BRW_WML_LAYOUT_5349_112_2_18_0_37_14_226,"[BRW]WmlPointIsInHotLink input  error")
TRACE_MSG(BRW_WML_LAYOUT_5385_112_2_18_0_37_14_227,"[BRW]WmlGetFocusAnchorChildrenImageInfo input  error")
TRACE_MSG(BRW_WML_LAYOUT_5508_112_2_18_0_37_14_228,"#brw BrwGetStringInfoInLineWithDir return value error!")
TRACE_MSG(BRW_WML_PARSER_383_112_2_18_0_37_15_229,"[BRW]WML_StartElement_Func input  error")
TRACE_MSG(BRW_WML_PARSER_424_112_2_18_0_37_15_230,"[BRW]WML_StartElement_Func index  error")
TRACE_MSG(BRW_WML_PARSER_464_112_2_18_0_37_16_231,"[BRW]WML_EndElement_Func input  error")
TRACE_MSG(BRW_WML_PARSER_484_112_2_18_0_37_16_232,"[BRW]WML_EndElement_Func input  error")
TRACE_MSG(BRW_WML_PARSER_520_112_2_18_0_37_16_233,"[BRW]WML_CharacterData_Func input  error")
TRACE_MSG(BRW_WML_PARSER_537_112_2_18_0_37_16_234,"[BRW]WML_StartCdataSection_Func input  error")
TRACE_MSG(BRW_WML_PARSER_547_112_2_18_0_37_16_235,"[BRW]WML_EndCdataSection_Func input  error")
TRACE_MSG(BRW_WML_PARSER_558_112_2_18_0_37_16_236,"[BRW]WML_DectXml input  error")
TRACE_MSG(BRW_WML_PARSER_608_112_2_18_0_37_16_237,"[BRW]WBXML_StartElementHandler input  error")
TRACE_MSG(BRW_WML_PARSER_617_112_2_18_0_37_16_238,"<%s")
TRACE_MSG(BRW_WML_PARSER_624_112_2_18_0_37_16_239," %s=\"%s\"")
TRACE_MSG(BRW_WML_PARSER_630_112_2_18_0_37_16_240,"/>\n")
TRACE_MSG(BRW_WML_PARSER_632_112_2_18_0_37_16_241,">\n")
TRACE_MSG(BRW_WML_PARSER_666_112_2_18_0_37_16_242,"[BRW]WBXML_StartElementHandler parent_index  error")
TRACE_MSG(BRW_WML_PARSER_704_112_2_18_0_37_16_243,"[BRW]WBXML_EndElementHandler input  error")
TRACE_MSG(BRW_WML_PARSER_711_112_2_18_0_37_16_244,"</%s>\n")
TRACE_MSG(BRW_WML_PARSER_731_112_2_18_0_37_16_245,"[BRW]WBXML_EndElementHandler parent_index  error")
TRACE_MSG(BRW_WML_PARSER_766_112_2_18_0_37_16_246,"[BRW]WBXML_CharactersHandler input  error")
TRACE_MSG(BRW_WML_PARSER_782_112_2_18_0_37_16_247,"[BRW]WBXML_StartDocumentHandler input  error")
TRACE_MSG(BRW_WML_PARSER_1011_112_2_18_0_37_17_248,"[BRW]WML_ProcessCharacter parent_index  error")
TRACE_MSG(BRW_WML_PARSER_1023_112_2_18_0_37_17_249,"[BRW]WML_ProcessCharacter len  error")
TRACE_MSG(BRW_WML_PARSER_1046_112_2_18_0_37_17_250,"[BRW]WBXML_ProcessCharacter len  error")
TRACE_MSG(BRW_WML_PARSER_1065_112_2_18_0_37_17_251,"[BRW]WBXML_ProcessCharacter parent_index  error")
TRACE_MSG(BRW_WML_PARSER_1146_112_2_18_0_37_17_252,"[BRW]DoDuplicate input  error")
TRACE_MSG(BRW_WML_PARSER_1229_112_2_18_0_37_17_253,"[BRW]DoCompare layout_type  error")
TRACE_MSG(BRW_WML_PARSER_2253_112_2_18_0_37_19_254,"[BRW]WmlParse_Wml_Child wml_ctrl_ptr  error")
TRACE_MSG(BRW_WML_PARSER_2300_112_2_18_0_37_19_255,"[BRW]WmlParse_Wml_Child layout_type  error")
TRACE_MSG(BRW_WML_PARSER_2655_112_2_18_0_37_20_256,"\n[brw]WmlParse_Anchor_Child process second bgsound\n")
TRACE_MSG(BRW_WML_PARSER_2673_112_2_18_0_37_20_257,"[BRW]WmlParse_A_Child   error")
TRACE_MSG(BRW_WML_PARSER_2740_112_2_18_0_37_20_258,"\n[brw]WmlParse_A_Child process second bgsound\n")
TRACE_MSG(BRW_WML_PARSER_2939_112_2_18_0_37_21_259,"[BRW]WmlParse_P_Child layout_type  error")
TRACE_MSG(BRW_WML_PARSER_3050_112_2_18_0_37_21_260,"\n[brw]WmlParse_A_Child process second bgsound\n")
TRACE_MSG(BRW_WML_PARSER_3227_112_2_18_0_37_21_261,"[BRW]WmlParse_Prev_Child task_type  error")
TRACE_MSG(BRW_WML_PARSER_3250_112_2_18_0_37_21_262,"[BRW]WmlParse_Head_Child input  error")
TRACE_MSG(BRW_WML_PARSER_3335_112_2_18_0_37_21_263,"[BRW]WmlParse_Card_Child layout_type  error")
TRACE_MSG(BRW_WML_PARSER_3488_112_2_18_0_37_22_264,"\n[brw]WmlParse_Card_Child process second bgsound\n")
TRACE_MSG(BRW_WML_PARSER_3697_112_2_18_0_37_22_265,"\n[brw]WmlParse_A_Child process second bgsound\n")
TRACE_MSG(BRW_WML_PARSER_3744_112_2_18_0_37_22_266,"\n[BRW]set bgsound loop = %d\n")
TRACE_MSG(BRW_WML_PARSER_3749_112_2_18_0_37_22_267,"\n[BRW]Invaild bgsound loop value:%s\n")
TRACE_MSG(BRW_WML_PARSER_3768_112_2_18_0_37_22_268,"\n[BRW]set bgsound balance = %d\n")
TRACE_MSG(BRW_WML_PARSER_3774_112_2_18_0_37_22_269,"\n[BRW]Invaild bgsound balance value:%s\n")
TRACE_MSG(BRW_WML_PARSER_3792_112_2_18_0_37_22_270,"\n[BRW]set bgsound volume = %d\n")
TRACE_MSG(BRW_WML_PARSER_3798_112_2_18_0_37_22_271,"\n[BRW]Invaild bgsound volume value:%s\n")
TRACE_MSG(BRW_WML_PARSER_3816_112_2_18_0_37_22_272,"[BRW]WML_BgsoundAttrsValueIsValid in param error")
TRACE_MSG(BRW_XHTML_CALC_DRAW_5246_112_2_18_0_37_34_273,"#brw Xhtml_Draw_Input BrwGetStringInfoInLineWithDir return value error!")
TRACE_MSG(BRW_XHTML_CALC_DRAW_8815_112_2_18_0_37_42_274,"#brw BrwGetStringInfoInLineWithDir return value error!")
TRACE_MSG(BRW_XHTML_LAYOUT_3166_112_2_18_0_37_48_275,"[BRW]XhtmlLayoutInputClickProcess active_ptr error")
TRACE_MSG(BRW_XHTML_LAYOUT_3292_112_2_18_0_37_49_276,"[BRW]WmlGetFocusAnchorChildrenImageInfo input  error")
TRACE_MSG(BRW_XHTML_LAYOUT_3752_112_2_18_0_37_50_277,"[BRW]XhtmlHandleReset active_ptr  error")
TRACE_MSG(BRW_XHTML_LAYOUT_3761_112_2_18_0_37_50_278,"[BRW]XhtmlHandleReset input_ptr  error")
TRACE_MSG(BRW_XHTML_LAYOUT_3807_112_2_18_0_37_50_279,"[BRW]XhtmlHandleReset select_ptr  error")
TRACE_MSG(BRW_XHTML_LAYOUT_4585_112_2_18_0_37_51_280,"Brw Xhtml_ComputeCSS Start")
TRACE_MSG(BRW_XHTML_LAYOUT_4597_112_2_18_0_37_51_281,"Brw Xhtml_ComputeCSS end")
TRACE_MSG(BRW_XHTML_PARSER_108_112_2_18_0_37_52_282,"brw BRWMem_ReallocEx malloc fail!")
TRACE_MSG(BRW_XHTML_PARSER_1021_112_2_18_0_37_54_283,"brw XHTML_EndElement_Func: get invalid index")
TRACE_MSG(BRW_XHTML_PARSER_1387_112_2_18_0_37_55_284,"[BRW]XHTML_ProcessCharacter parent_layout  error")
TRACE_MSG(BRW_XHTML_PARSER_2920_112_2_18_0_37_58_285,"\n[BRW]set bgsound loop = %d\n")
TRACE_MSG(BRW_XHTML_PARSER_2925_112_2_18_0_37_58_286,"\n[BRW]Invaild bgsound loop value:%s\n")
TRACE_MSG(BRW_XHTML_PARSER_2944_112_2_18_0_37_58_287,"\n[BRW]set bgsound balance = %d\n")
TRACE_MSG(BRW_XHTML_PARSER_2950_112_2_18_0_37_58_288,"\nInvaild bgsound balance value:%s\n")
TRACE_MSG(BRW_XHTML_PARSER_2968_112_2_18_0_37_58_289,"\n[BRW]set bgsound volume = %d\n")
TRACE_MSG(BRW_XHTML_PARSER_2974_112_2_18_0_37_58_290,"\n[BRW]Invaild bgsound volume value:%s\n")
TRACE_MSG(BRW_XHTML_PARSER_3800_112_2_18_0_37_59_291,"XHTML_ProcessTag image is null")
TRACE_MSG(BRW_XHTML_PARSER_3977_112_2_18_0_38_0_292,"\nXHTML_ProcessTag process second bgsound\n")
TRACE_MSG(BRW_XHTML_PARSER_4062_112_2_18_0_38_0_293,"XHTML_ProcessTag object image is null")
TRACE_MSG(BRW_XHTML_PARSER_4100_112_2_18_0_38_0_294,"[BRW] XHTML_ProcessTag:parser_ptr->stack_layout_ptr is NULL!")
TRACE_MSG(BRW_XHTML_PARSER_4323_112_2_18_0_38_0_295,"[BRW]Xhtml_BgsoundAttrsValueIsValid in param error")
TRACE_MSG(BRW_XHTML_PARSER_4611_112_2_18_0_38_1_296,"ProcessParseBufCharacter: Other tag need not inform app")
TRACE_MSG(BRW_XHTML_PARSER_4925_112_2_18_0_38_2_297,"[BRW] XHTML_ProcessTag:parser_ptr->stack_layout_ptr is NULL!")
TRACE_MSG(BRW_XHTML_PARSER_5161_112_2_18_0_38_2_298,"brw XHTML_EndElement_Func: get invalid index")
TRACE_MSG(JSOBJECT_ANCHOR_74_112_2_18_0_38_2_299,"JSObject_SetAnchorProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_ANCHOR_118_112_2_18_0_38_3_300,"JSObject_SetAnchorProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_BODY_145_112_2_18_0_38_3_301,"sipdermonkey: func, bodySetDir not implemented")
TRACE_MSG(JSOBJECT_BODY_174_112_2_18_0_38_3_302,"sipdermonkey: func, bodyGetLang not implemented")
TRACE_MSG(JSOBJECT_CHECKBOX_94_112_2_18_0_38_4_303,"JSObject_GetCheckboxProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_CHECKBOX_159_112_2_18_0_38_4_304,"JSObject_SetCheckboxProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_CHECKBOX_220_112_2_18_0_38_4_305,"JSObject_SetCheckboxProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_DIV_76_112_2_18_0_38_4_306,"JSObject_GetDivProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_DIV_174_112_2_18_0_38_5_307,"JSObject_SetDivProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_DIV_181_112_2_18_0_38_5_308,"JSObject_GetDivProperty xhtml_ctrl_ptr error")
TRACE_MSG(JSOBJECT_DOC_621_112_2_18_0_38_7_309,"sipdermonkey: func, document_creatTextNode not implemented")
TRACE_MSG(JSOBJECT_DOC_1125_112_2_18_0_38_8_310,"sipdermonkey: func, documentLastModified not implemented")
TRACE_MSG(JSOBJECT_FORM_110_112_2_18_0_38_9_311,"JSObject_GetFormProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_FORM_241_112_2_18_0_38_9_312,"JSObject_SetFormProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_FORM_248_112_2_18_0_38_9_313,"BRW JSObject_SetFormProperty handle error")
TRACE_MSG(JSOBJECT_FORM_337_112_2_18_0_38_9_314,"JSObject_SetFormProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_FORM_375_112_2_18_0_38_9_315,"JSObject_SetFormProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_IMAGE_220_112_2_18_0_38_11_316,"sipdermonkey: func, imageSetBorder not implemented")
TRACE_MSG(JSOBJECT_IMAGE_304_112_2_18_0_38_11_317,"sipdermonkey: func, imageSetHspace not implemented")
TRACE_MSG(JSOBJECT_IMAGE_470_112_2_18_0_38_12_318,"sipdermonkey: func, imageSetHspace not implemented")
TRACE_MSG(JSOBJECT_IMAGE_517_112_2_18_0_38_12_319,"sipdermonkey: func, image_onload implemented")
TRACE_MSG(JSOBJECT_INPUTBUTTON_104_112_2_18_0_38_12_320,"JSObject_GetInputbuttonProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTBUTTON_163_112_2_18_0_38_12_321,"JSObject_SetInputbuttonProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTBUTTON_232_112_2_18_0_38_13_322,"JSObject_SetInputbuttonProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTBUTTON_275_112_2_18_0_38_13_323,"JSObject_SetInputbuttonProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTBUTTON_320_112_2_18_0_38_13_324,"JSObject_SetInputbuttonProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTHIDDEN_80_112_2_18_0_38_13_325,"JSObject_GetInputhiddenProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTHIDDEN_139_112_2_18_0_38_13_326,"JSObject_SetInputhiddenProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTPASSWORD_105_112_2_18_0_38_14_327,"JSObject_GetInputpasswordProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTPASSWORD_174_112_2_18_0_38_14_328,"JSObject_SetInputpasswordProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTPASSWORD_226_112_2_18_0_38_14_329,"BRW JSObject_SetInputpasswordProperty vp type error")
TRACE_MSG(JSOBJECT_INPUTPASSWORD_234_112_2_18_0_38_14_330,"BRW JSObject_SetInputpasswordProperty vp type error")
TRACE_MSG(JSOBJECT_INPUTPASSWORD_270_112_2_18_0_38_14_331,"JSObject_SetInputpasswordProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTPASSWORD_313_112_2_18_0_38_14_332,"JSObject_SetInputpasswordProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTRADIO_97_112_2_18_0_38_15_333,"JSObject_GetInputradioProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTRADIO_165_112_2_18_0_38_15_334,"JSObject_SetInputradioProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTRADIO_241_112_2_18_0_38_15_335,"JSObject_SetInputradioProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTRESET_92_112_2_18_0_38_15_336,"JSObject_GetInputresetProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTRESET_151_112_2_18_0_38_16_337,"JSObject_SetInputresetProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTRESET_222_112_2_18_0_38_16_338,"JSObject_SetInputresetProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTSUBMIT_93_112_2_18_0_38_16_339,"JSObject_GetInputsubmitProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTSUBMIT_155_112_2_18_0_38_16_340,"JSObject_SetInputsubmitProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTSUBMIT_229_112_2_18_0_38_16_341,"JSObject_SetInputsubmitProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTTEXT_106_112_2_18_0_38_17_342,"JSObject_GetInputtextProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTTEXT_176_112_2_18_0_38_17_343,"JSObject_SetInputtextProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTTEXT_241_112_2_18_0_38_17_344,"BRW JSObject_SetInputtextProperty vp type error")
TRACE_MSG(JSOBJECT_INPUTTEXT_249_112_2_18_0_38_17_345,"BRW JSObject_SetInputtextProperty vp type error")
TRACE_MSG(JSOBJECT_INPUTTEXT_284_112_2_18_0_38_17_346,"JSObject_SetInputtextProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTTEXT_327_112_2_18_0_38_17_347,"JSObject_SetInputtextProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_INPUTTEXT_372_112_2_18_0_38_17_348,"JSObject_SetInputtextProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_LOCATION_362_112_2_18_0_38_19_349,"BRW JSObject_Location_Assign url null error")
TRACE_MSG(JSOBJECT_LOCATION_431_112_2_18_0_38_19_350,"BRW JSObject_Location_Assign url null error")
TRACE_MSG(JSOBJECT_OPTION_84_112_2_18_0_38_21_351,"JSObject_GetOptionProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_OPTION_156_112_2_18_0_38_21_352,"JSObject_SetOptionProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_SELECT_107_112_2_18_0_38_22_353,"JSObject_GetSelectProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_SELECT_199_112_2_18_0_38_22_354,"JSObject_SetSelectProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_SELECT_206_112_2_18_0_38_22_355,"JSObject_GetSelectProperty xhtml_ctrl_ptr error")
TRACE_MSG(JSOBJECT_SELECT_278_112_2_18_0_38_22_356,"JSObject_SetSelectProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_SELECT_315_112_2_18_0_38_22_357,"JSObject_SetSelectProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLE_85_112_2_18_0_38_23_358,"JSObject_GetTableProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLE_128_112_2_18_0_38_23_359,"JSObject_SetTableProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLE_180_112_2_18_0_38_23_360,"JSObject_SetTableProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLE_192_112_2_18_0_38_23_361,"BRW JSObject_Table_InsertRow vp type error")
TRACE_MSG(JSOBJECT_TABLE_198_112_2_18_0_38_23_362,"BRW JSObject_Table_InsertRow val error")
TRACE_MSG(JSOBJECT_TABLECELL_78_112_2_18_0_38_23_363,"JSObject_GetTablecellProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLECELL_127_112_2_18_0_38_23_364,"JSObject_SetTablecellProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLECELL_134_112_2_18_0_38_23_365,"JSObject_GetTablecellProperty xhtml_ctrl_ptr error")
TRACE_MSG(JSOBJECT_TABLEROW_83_112_2_18_0_38_24_366,"JSObject_GetTablerowProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLEROW_123_112_2_18_0_38_24_367,"JSObject_SetTablerowProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLEROW_130_112_2_18_0_38_24_368,"JSObject_GetTablerowProperty xhtml_ctrl_ptr error")
TRACE_MSG(JSOBJECT_TABLEROW_175_112_2_18_0_38_24_369,"JSObject_SetTablerowProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TABLEROW_182_112_2_18_0_38_24_370,"BRW JSObject_Tablerow_InsertCell handle error")
TRACE_MSG(JSOBJECT_TABLEROW_188_112_2_18_0_38_24_371,"BRW JSObject_Tablerow_InsertCell argv  type error")
TRACE_MSG(JSOBJECT_TEXTAREA_104_112_2_18_0_38_24_372,"JSObject_GetTextareaProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TEXTAREA_167_112_2_18_0_38_25_373,"JSObject_SetTextareaProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TEXTAREA_201_112_2_18_0_38_25_374,"BRW JSObject_SetTextareaProperty vp type error")
TRACE_MSG(JSOBJECT_TEXTAREA_210_112_2_18_0_38_25_375,"BRW JSObject_SetTextareaProperty vp type error")
TRACE_MSG(JSOBJECT_TEXTAREA_246_112_2_18_0_38_25_376,"JSObject_SetTextareaProperty JS_GetPrivate null or type error")
TRACE_MSG(JSOBJECT_TEXTAREA_289_112_2_18_0_38_25_377,"JSObject_SetTextareaProperty JS_GetPrivate null or type error")
TRACE_MSG(JSAPI_3128_112_2_18_0_38_45_378,"js err, file:%s,line:%d")
TRACE_MSG(JSDHASH_582_112_2_18_0_39_9_379,"js err, file:%s,line:%d")
TRACE_MSG(JSDTOA_876_112_2_18_0_39_11_380,"js err, file:%,line:%d; cmp called with a->x[a->wds-1] == 0")
TRACE_MSG(JSDTOA_878_112_2_18_0_39_11_381,"js err, file:%,line:%d; cmp called with b->x[b->wds-1] == 0")
TRACE_MSG(JSDTOA_1015_112_2_18_0_39_12_382,"js err, file:%s, line:%d; zero y in b2d")
TRACE_MSG(JSDTOA_2083_112_2_18_0_39_14_383,"js err, file:%s,line:%d")
TRACE_MSG(JSDTOA_2102_112_2_18_0_39_14_384,"js err, file:%s,line:%d")
TRACE_MSG(JSDTOA_2244_112_2_18_0_39_14_385,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_653_112_2_18_0_39_18_386,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_825_112_2_18_0_39_18_387,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_1896_112_2_18_0_39_20_388,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_1910_112_2_18_0_39_20_389,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_1924_112_2_18_0_39_20_390,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_3255_112_2_18_0_39_23_391,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_3345_112_2_18_0_39_24_392,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_4152_112_2_18_0_39_25_393,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_4474_112_2_18_0_39_26_394,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_4714_112_2_18_0_39_27_395,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_4838_112_2_18_0_39_27_396,"js err, file:%s,line:%d")
TRACE_MSG(JSEMIT_4970_112_2_18_0_39_27_397,"js err, file:%s,line:%d")
TRACE_MSG(JSINTERP_3804_112_2_18_0_39_47_398,"js err, file:%s,line:%d")
TRACE_MSG(JSINTERP_4815_112_2_18_0_39_49_399,"js err, file:%s,line:%d")
TRACE_MSG(JSLOCK_992_112_2_18_0_39_53_400,"js err, file:%s,line:%d")
TRACE_MSG(JSOBJ_234_112_2_18_0_39_59_401,"js err, file:%s,line:%d")
TRACE_MSG(JSOPCODE_1525_112_2_18_0_40_11_402,"js err, file:%s,line:%d")
TRACE_MSG(JSOPCODE_2139_112_2_18_0_40_12_403,"js err, file:%s,line:%d")
TRACE_MSG(JSOPCODE_3139_112_2_18_0_40_14_404,"js err, file:%s,line:%d")
TRACE_MSG(JSPARSE_1172_112_2_18_0_40_17_405,"js err, file:%s,line:%d")
TRACE_MSG(JSPARSE_2770_112_2_18_0_40_20_406,"js err, file:%s,line:%d")
TRACE_MSG(JSPRF_595_112_2_18_0_40_26_407,"js err, file:%s,line:%d")
TRACE_MSG(JSPRF_608_112_2_18_0_40_26_408,"js err, file:%s,line:%d")
TRACE_MSG(JSPRF_716_112_2_18_0_40_26_409,"js err, file:%s,line:%d")
TRACE_MSG(JSPRF_976_112_2_18_0_40_27_410,"js err, file:%s,line:%d")
TRACE_MSG(JSPRF_988_112_2_18_0_40_27_411,"js err, file:%s,line:%d")
TRACE_MSG(JSPRF_1019_112_2_18_0_40_27_412,"js err, file:%s,line:%d")
TRACE_MSG(JSSCRIPT_1175_112_2_18_0_40_46_413,"js err, file:%s,line:%d")
TRACE_MSG(JSSCRIPT_1189_112_2_18_0_40_46_414,"js err, file:%s,line:%d")
TRACE_MSG(JSXML_2104_112_2_18_0_41_3_415,"js err, file:%s,line:%d")
TRACE_MSG(JSXML_2190_112_2_18_0_41_4_416,"js err, file:%s,line:%d")
TRACE_MSG(MMMSDECODER_503_112_2_18_0_43_2_417,"MMS_PARSE:mmsReadWellKnownMedia type=%s")
TRACE_MSG(MMMSDECODER_720_112_2_18_0_43_2_418,"MMS_PARSE: mmsReadConTypeVal: the charset is =%d")
TRACE_MSG(MMMSDECODER_807_112_2_18_0_43_2_419,"MMS_PARSE: headerlen = %d")
TRACE_MSG(MMMSDECODER_808_112_2_18_0_43_2_420,"MMS_PARSE: c_datalen = %d")
TRACE_MSG(MMMSDECODER_819_112_2_18_0_43_2_421,"MMS_PARSE: content type is %s")
TRACE_MSG(MMMSDECODER_1365_112_2_18_0_43_3_422,"MMS_PARSE: mmsGetSubjectLen subLen=%d")
TRACE_MSG(MMMSDECODER_1382_112_2_18_0_43_3_423,"MMS_PARSE: **mmsGetSubjectLen subLen=%d")
TRACE_MSG(MMMSDECODER_1396_112_2_18_0_43_3_424,"MMS_PARSE***mmsGetSubjectLen subLen=%d")
TRACE_MSG(MMMSDECODER_1411_112_2_18_0_43_3_425,"MMS_PARSE*^_^mmsGetSubjectLen subLen=%d")
TRACE_MSG(MMMSDECODER_1432_112_2_18_0_43_4_426,"MMS_PARSE:mmsReadHeader input invalid!")
TRACE_MSG(MMMSDECODER_1445_112_2_18_0_43_4_427,"MMS_PARSE:mmsReadHeader, current byte = %d, m_i = %d, m_long = %d")
TRACE_MSG(MMMSDECODER_1460_112_2_18_0_43_4_428,"MMS_PARSE:mmsReadHeader, message type = %d")
TRACE_MSG(MMMSDECODER_1677_112_2_18_0_43_4_429,"mmsReadHeader pMmshead->Mms_subj is %s,length is %d")
TRACE_MSG(MMMSDECODER_1861_112_2_18_0_43_4_430,"mmsReadHeader unsupport header,first byte value= %d")
TRACE_MSG(MMMSDECODER_1959_112_2_18_0_43_5_431,"MMS_PARSE: mmsDecode input invalid! ")
TRACE_MSG(MMMSDECODER_1970_112_2_18_0_43_5_432,"MMS_PARSE: mmsDecode open decode file failed! ")
TRACE_MSG(MMMSDECODER_1978_112_2_18_0_43_5_433,"MMS_PARSE: *** error!mmsDecode:malloc failed")
TRACE_MSG(MMMSDECODER_1999_112_2_18_0_43_5_434,"MMS_PARSE: mmsDecode:mms_cfa mmsfile size is :%d")
TRACE_MSG(MMMSDECODER_2002_112_2_18_0_43_5_435,"MMS_PARSE: mmsDecode pMmsDecoder->m_long <5000")
TRACE_MSG(MMMSDECODER_2006_112_2_18_0_43_5_436,"MMS_PARSE: mms lib *** error!mmsDecode:DOCTYPE wml PUBLIC")
TRACE_MSG(MMMSDECODER_2014_112_2_18_0_43_5_437,"MMS_PARSE: mmsDecode, message type =%d")
TRACE_MSG(MMMSDOC_94_112_2_18_0_43_5_438,"MMS_PARSE: mmsGetEncodedLen after change into utf-8,the size is %d,\n")
TRACE_MSG(MMMSDOC_240_112_2_18_0_43_6_439,"MMS_PARSE: in mmsNewSlideSize not new_slide slide addr is %d")
TRACE_MSG(MMMSDOC_571_112_2_18_0_43_6_440,"MMS_PARSE: erro:file name is too long")
TRACE_MSG(MMMSDOC_708_112_2_18_0_43_6_441,"MMS_PARSE: MMS_AddConetnt in")
TRACE_MSG(MMMSDOC_712_112_2_18_0_43_6_442,"MMS_PARSE: MMS_AddConetnt error:pSlide is null or pDocument is null")
TRACE_MSG(MMMSDOC_716_112_2_18_0_43_6_443,"MMS_PARSE: MMS_AddConetnt type is %d")
TRACE_MSG(MMMSDOC_720_112_2_18_0_43_6_444,"MMS_PARSE: MMS_AddConetnt pFilename = NULL")
TRACE_MSG(MMMSDOC_724_112_2_18_0_43_6_445,"MMS_PARSE: MMS_AddConetnt insert content file name is :%s")
TRACE_MSG(MMMSDOC_726_112_2_18_0_43_6_446,"MMS_PARSE: MMS_AddConetnt datalen is %d")
TRACE_MSG(MMMSDOC_735_112_2_18_0_43_6_447,"MMS_PARSE: MMS_AddConetnt pContent = NULL")
TRACE_MSG(MMMSDOC_752_112_2_18_0_43_7_448,"MMS_PARSE: MMS_AddConetnt: add text case ,will call mmsGetEncodedLen ,print all pFilename\n")
TRACE_MSG(MMMSDOC_753_112_2_18_0_43_7_449,"MMS_PARSE: MMS_AddConetnt pFileData is %s,filelength is %d")
TRACE_MSG(MMMSDOC_756_112_2_18_0_43_7_450,"MMS_PARSE: MMS_AddConetnt cms_mms get len is %d")
TRACE_MSG(MMMSDOC_765_112_2_18_0_43_7_451,"MMS_PARSE: MMS_AddConetnt error:file length is too long")
TRACE_MSG(MMMSDOC_782_112_2_18_0_43_7_452,"MMS_PARSE: MMS_AddConetnt:now add a img content")
TRACE_MSG(MMMSDOC_785_112_2_18_0_43_7_453,"MMS_PARSE: MMS_AddConetnt pContent = %x")
TRACE_MSG(MMMSDOC_788_112_2_18_0_43_7_454,"MMS_PARSE: MMS_AddConetnt error: pContent = NULL")
TRACE_MSG(MMMSDOC_801_112_2_18_0_43_7_455,"MMS_PARSE: MMS_AddConetnt file name is %s")
TRACE_MSG(MMMSDOC_825_112_2_18_0_43_7_456,"MMS_PARSE: MMS_AddConetnt error:file length is too long")
TRACE_MSG(MMMSDOC_883_112_2_18_0_43_7_457,"MMS_PARSE: MMS_AddAttachment in")
TRACE_MSG(MMMSDOC_888_112_2_18_0_43_7_458,"MMS_PARSE: MMS_AddAttachment error:param is null!")
TRACE_MSG(MMMSDOC_891_112_2_18_0_43_7_459,"MMS_PARSE: MMS_AddAttachment insert content file name is :%s")
TRACE_MSG(MMMSDOC_895_112_2_18_0_43_7_460,"MMS_PARSE: MMS_AddAttachment file name is %d")
TRACE_MSG(MMMSDOC_898_112_2_18_0_43_7_461,"MMS_PARSE: MMS_AddAttachment error:file name length error!")
TRACE_MSG(MMMSDOC_906_112_2_18_0_43_7_462,"MMS_PARSE: MMS_AddAttachment error: no memory")
TRACE_MSG(MMMSDOC_962_112_2_18_0_43_7_463,"GetSuffixFromFileName: input error!")
TRACE_MSG(MMMSDOC_999_112_2_18_0_43_7_464,"MMS_PARSE:GetContentTypeBySuffix, suffix = %s")
TRACE_MSG(MMMSDOC_1426_112_2_18_0_43_8_465,"MMS_PARSE: MMS_CreateNewSlide slide addr is %d")
TRACE_MSG(MMMSDOC_1427_112_2_18_0_43_8_466,"MMS_PARSE: MMS_CreateNewSlide pDocument addr is %d")
TRACE_MSG(MMMSDOC_1476_112_2_18_0_43_8_467,"MMS_PARSE: MMS_CreateNewSlide slide addr is %d")
TRACE_MSG(MMMSDOC_1477_112_2_18_0_43_8_468,"MMS_PARSE: MMS_CreateNewSlide pDocument addr is %d")
TRACE_MSG(MMMSENCODER_643_112_2_18_0_43_10_469,"MMS_PARSE: Encodelist in text/pain case\n")
TRACE_MSG(MMMSENCODER_662_112_2_18_0_43_10_470,"MMS_PARSE: Encodelist len is %d")
TRACE_MSG(MMMSENCODER_1257_112_2_18_0_43_12_471,"MMS_PARSE: pMmsHead->Mms_content_type = %s")
TRACE_MSG(MMMSENCODER_1332_112_2_18_0_43_12_472,"MMS_PARSE: mmsEncodeBody input invalid!")
TRACE_MSG(MMMSENCODER_1352_112_2_18_0_43_12_473,"MMS_PARSE: mmsEncodeBody list is pnull!")
TRACE_MSG(MMMSENCODER_1380_112_2_18_0_43_12_474,"MMS_PARSE: mmsEncode input invalid!")
TRACE_MSG(MMMSENCODER_1388_112_2_18_0_43_12_475,"MMS_PARSE: mmsEncode alloc failed!")
TRACE_MSG(MMMSENCODER_1395_112_2_18_0_43_12_476,"MMS_PARSE: mmsEncode ,pathname is %s")
TRACE_MSG(MMMSENCODER_1402_112_2_18_0_43_12_477,"MMS_PARSE: mmsEncode out put 0!")
TRACE_MSG(MMMSENCODER_1421_112_2_18_0_43_12_478,"MMS_PARSE: mmsEncode set file failed!")
TRACE_MSG(MMMSENCODER_1437_112_2_18_0_43_12_479,"MMS_PARSE: mmsEncode encode body error!")
TRACE_MSG(MMMSHTTP_244_112_2_18_0_43_13_480,"MMS_PARSE: get http net init cnf !")
TRACE_MSG(MMMSHTTP_249_112_2_18_0_43_13_481,"MMS_PARSE: get wsp net init cnf !")
TRACE_MSG(MMMSHTTP_255_112_2_18_0_43_13_482,"MMS_PARSE: get net request ind !")
TRACE_MSG(MMMSHTTP_261_112_2_18_0_43_13_483,"MMS_PARSE: get net progress ind !")
TRACE_MSG(MMMSHTTP_267_112_2_18_0_43_13_484,"MMS_PARSE: get net data ind !")
TRACE_MSG(MMMSHTTP_273_112_2_18_0_43_13_485,"MMS_PARSE: get net post cnf !")
TRACE_MSG(MMMSHTTP_279_112_2_18_0_43_13_486,"MMS_PARSE: get net close cnf !")
TRACE_MSG(MMMSHTTP_285_112_2_18_0_43_13_487,"MMS_PARSE: get net cancel cnf !")
TRACE_MSG(MMMSHTTP_291_112_2_18_0_43_13_488,"MMS_PARSE: get net header ind !")
TRACE_MSG(MMMSHTTP_297_112_2_18_0_43_13_489,"MMS_PARSE: get net get cnf !")
TRACE_MSG(MMMSHTTP_303_112_2_18_0_43_13_490,"MMS_PARSE: get net error ind !")
TRACE_MSG(MMMSHTTP_309_112_2_18_0_43_13_491,"MMS_PARSE: get net auth ind !")
TRACE_MSG(MMMSHTTP_452_112_2_18_0_43_13_492,"MMS_PARSE: MmsLibCancelMms enter! http context = %d, request =%d!")
TRACE_MSG(MMMSHTTP_477_112_2_18_0_43_13_493,"MMS_PARSE: MmsLibCancelMms, send mms enter close request!")
TRACE_MSG(MMMSHTTP_483_112_2_18_0_43_13_494,"MMS_PARSE: MmsLibCancelMms, recv mms enter close request!")
TRACE_MSG(MMMSHTTP_494_112_2_18_0_43_13_495,"MMS_PARSE: MmsLibCancelMms, wait http init cnf and cancel!")
TRACE_MSG(MMMSHTTP_519_112_2_18_0_43_13_496,"MMS_PARSE: MmsLibCancelMms wsp context = %d, request =%d!")
TRACE_MSG(MMMSHTTP_523_112_2_18_0_43_13_497,"MMS_PARSE: MmsLibCancelMms, wsp send mms enter close request!")
TRACE_MSG(MMMSHTTP_529_112_2_18_0_43_13_498,"MMS_PARSE: MmsLibCancelMms,wsp recv mms enter close request!")
TRACE_MSG(MMMSHTTP_540_112_2_18_0_43_13_499,"MMS_PARSE: MmsLibCancelMms, wait wsp init cnf and cancel!")
TRACE_MSG(MMMSHTTP_569_112_2_18_0_43_13_500,"MMS_PARSE:MmsLibHttpInitCnf input invalid!")
TRACE_MSG(MMMSHTTP_576_112_2_18_0_43_13_501,"MMS_PARSE:MmsLibHttpInitCnf saved module id =%d, msg module id =%d!")
TRACE_MSG(MMMSHTTP_585_112_2_18_0_43_13_502,"MMS_PARSE:MmsLibHttpInitCnf, context_id = %d, instance = %d")
TRACE_MSG(MMMSHTTP_590_112_2_18_0_43_13_503,"MMS_PARSE:MmsLibHttpInitCnf init fail!")
TRACE_MSG(MMMSHTTP_599_112_2_18_0_43_13_504,"MMS_PARSE: MmsLibHttpInitCnf, send mms enter close request!")
TRACE_MSG(MMMSHTTP_605_112_2_18_0_43_13_505,"MMS_PARSE: MmsLibHttpInitCnf, recv mms enter close request!")
TRACE_MSG(MMMSHTTP_638_112_2_18_0_43_13_506,"MMS_PARSE:MmsLibWspInitCnf input invalid!")
TRACE_MSG(MMMSHTTP_645_112_2_18_0_43_13_507,"MMS_PARSE:MmsLibWspInitCnf saved module id =%d, msg module id =%d!")
TRACE_MSG(MMMSHTTP_653_112_2_18_0_43_14_508,"MMS_PARSE:MmsLibWspInitCnf, context_id = %d")
TRACE_MSG(MMMSHTTP_658_112_2_18_0_43_14_509,"MMS_PARSE:MmsLibWspInitCnf init fail!")
TRACE_MSG(MMMSHTTP_667_112_2_18_0_43_14_510,"MMS_PARSE: MmsLibWspInitCnf, send mms enter close request!")
TRACE_MSG(MMMSHTTP_673_112_2_18_0_43_14_511,"MMS_PARSE: MmsLibWspInitCnf, recv mms enter close request!")
TRACE_MSG(MMMSHTTP_703_112_2_18_0_43_14_512,"MMS_PARSE: MmsLibHttpRequestIdInd input invalid!")
TRACE_MSG(MMMSHTTP_707_112_2_18_0_43_14_513,"MMS_PARSE: MmsLibHttpRequestIdInd enter, g_context_id =%d, sig_context_id = %d")
TRACE_MSG(MMMSHTTP_711_112_2_18_0_43_14_514,"MMS_PARSE: MmsLibHttpRequestIdInd context not match!")
TRACE_MSG(MMMSHTTP_717_112_2_18_0_43_14_515,"MMS_PARSE: MmsLibHttpRequestIdInd , request id = %d")
TRACE_MSG(MMMSHTTP_746_112_2_18_0_43_14_516,"MMS_PARSE: MmsLibHttpHeaderInd request does not match!")
TRACE_MSG(MMMSHTTP_768_112_2_18_0_43_14_517,"MMS_PARSE: MmsLibHttpRecvDataInd input invalid!")
TRACE_MSG(MMMSHTTP_778_112_2_18_0_43_14_518,"MMS_PARSE: MmsLibHttpRecvDataInd request does not match!")
TRACE_MSG(MMMSHTTP_782_112_2_18_0_43_14_519,"MMS_PARSE: MmsLibHttpRecvDataInd,data_ptr = %d,len =%d")
TRACE_MSG(MMMSHTTP_798_112_2_18_0_43_14_520,"MMS_PARSE: MmsLibHttpRecvDataInd, send mms, get data len = %d")
TRACE_MSG(MMMSHTTP_802_112_2_18_0_43_14_521,"MMS_PARSE: MmsLibHttpRecvDataInd, save new data!")
TRACE_MSG(MMMSHTTP_807_112_2_18_0_43_14_522,"MMS_PARSE: MmsLibHttpRecvDataInd, save again data!")
TRACE_MSG(MMMSHTTP_826_112_2_18_0_43_14_523,"MMS_PARSE: MmsLibHttpRecvDataInd, create recv file fail!")
TRACE_MSG(MMMSHTTP_837_112_2_18_0_43_14_524,"MMS_PARSE: MmsLibHttpRecvDataInd, write file fail!")
TRACE_MSG(MMMSHTTP_844_112_2_18_0_43_14_525,"MMS_PARSE: MmsLibHttpRecvDataInd,current is http,recv is file. should no this message!")
TRACE_MSG(MMMSHTTP_851_112_2_18_0_43_14_526,"MMS_PARSE: MmsLibHttpRecvDataInd, recv rsp mms do nothing!")
TRACE_MSG(MMMSHTTP_874_112_2_18_0_43_14_527,"MMS_PARSE: MmsLibHttpNetProgIndSig input invalid!")
TRACE_MSG(MMMSHTTP_884_112_2_18_0_43_14_528,"MMS_PARSE: MmsLibHttpNetProgIndSig request does not match!, sig request =%d")
TRACE_MSG(MMMSHTTP_907_112_2_18_0_43_14_529,"MMS_PARSE:MmsLibHttpNetProgIndSig recv total len = %d, current len = %d")
TRACE_MSG(MMMSHTTP_915_112_2_18_0_43_14_530,"MMS_PARSE:MmsLibHttpNetProgIndSig send total len = %d, current len = %d")
TRACE_MSG(MMMSHTTP_921_112_2_18_0_43_14_531,"MMS_PARSE:  MmsLibHttpNetProgIndSig: status error")
TRACE_MSG(MMMSHTTP_943_112_2_18_0_43_14_532,"MMS_PARSE:MmsLibHttpPostCnf,input invalid!")
TRACE_MSG(MMMSHTTP_946_112_2_18_0_43_14_533,"MMS_PARSE: MmsLibHttpPostCnf enter! current opt =%d")
TRACE_MSG(MMMSHTTP_955_112_2_18_0_43_14_534,"MMS_PARSE: MmsLibHttpPostCnf request does not match!, sig request =%d")
TRACE_MSG(MMMSHTTP_964_112_2_18_0_43_14_535,"MMS_PARSE: MmsLibHttpPostCnf, file name is invalid")
TRACE_MSG(MMMSHTTP_973_112_2_18_0_43_14_536,"MMS_PARSE: MmsLibHttpPostCnf, create new mms fail")
TRACE_MSG(MMMSHTTP_986_112_2_18_0_43_14_537,"MMS_PARSE:MmsLibHttpPostCnf decode mms error! ")
TRACE_MSG(MMMSHTTP_1002_112_2_18_0_43_14_538,"MMS_PARSE: MmsLibHttpPostCnf, send mms enter close request!")
TRACE_MSG(MMMSHTTP_1014_112_2_18_0_43_14_539,"MMS_PARSE: MmsLibHttpPostCnf, status error = %d!")
TRACE_MSG(MMMSHTTP_1021_112_2_18_0_43_14_540,"MMS_PARSE: MmsLibHttpPostCnf, address unresolved!")
TRACE_MSG(MMMSHTTP_1031_112_2_18_0_43_14_541,"MMS_PARSE: MmsLibHttpPostCnf, recv mms enter close request!")
TRACE_MSG(MMMSHTTP_1065_112_2_18_0_43_14_542,"MMS_PARSE: MmsLibHttpErrorInd request does not match!, sig request =%d")
TRACE_MSG(MMMSHTTP_1069_112_2_18_0_43_14_543,"MMS_PARSE: MmsLibHttpErrorInd, get net error = %d")
TRACE_MSG(MMMSHTTP_1075_112_2_18_0_43_14_544,"MMS_PARSE: MmsLibHttpErrorInd, send mms enter close request!")
TRACE_MSG(MMMSHTTP_1081_112_2_18_0_43_14_545,"MMS_PARSE: MmsLibHttpErrorInd, recv mms occurs error!")
TRACE_MSG(MMMSHTTP_1098_112_2_18_0_43_14_546,"MMS_PARSE: MmsLibHttpErrorInd, recv rsp mms occurs error!")
TRACE_MSG(MMMSHTTP_1104_112_2_18_0_43_14_547,"MMS_PARSE: MmsLibHttpErrorInd, has called http/wsp cancel request function,do nothing!")
TRACE_MSG(MMMSHTTP_1109_112_2_18_0_43_14_548,"MMS_PARSE: MmsLibHttpErrorInd, state error!")
TRACE_MSG(MMMSHTTP_1130_112_2_18_0_43_14_549,"MMS_PARSE:SendRecvFail, get current opt= %d, need retry = %d")
TRACE_MSG(MMMSHTTP_1135_112_2_18_0_43_15_550,"MMS_PARSE:SendRecvFail, send mms error!")
TRACE_MSG(MMMSHTTP_1139_112_2_18_0_43_15_551,"MMS_PARSE:SendRecvFail,fatal error, need retry = %d,tryed = %d, max= %d!")
TRACE_MSG(MMMSHTTP_1149_112_2_18_0_43_15_552,"MMS_PARSE:SendRecvFail, send mms error! http call fail")
TRACE_MSG(MMMSHTTP_1158_112_2_18_0_43_15_553,"MMS_PARSE:SendRecvFail, send mms error! wsp call fail")
TRACE_MSG(MMMSHTTP_1165_112_2_18_0_43_15_554,"MMS_PARSE:SendRecvFail, send mms error! net type error")
TRACE_MSG(MMMSHTTP_1180_112_2_18_0_43_15_555,"MMS_PARSE: SendRecvFail, inform app retrying sending , times = %d!")
TRACE_MSG(MMMSHTTP_1188_112_2_18_0_43_15_556,"MMS_PARSE:SendRecvFail, recv mms error!")
TRACE_MSG(MMMSHTTP_1192_112_2_18_0_43_15_557,"MMS_PARSE:SendFail, fatal error,need_retry= %d,tryed = %d, max= %d!")
TRACE_MSG(MMMSHTTP_1202_112_2_18_0_43_15_558,"MMS_PARSE:SendRecvFail, recv mms error! http call fail")
TRACE_MSG(MMMSHTTP_1211_112_2_18_0_43_15_559,"MMS_PARSE:SendRecvFail, recv mms error! wsp call fail")
TRACE_MSG(MMMSHTTP_1218_112_2_18_0_43_15_560,"MMS_PARSE:SendRecvFail, recv mms error! net type error")
TRACE_MSG(MMMSHTTP_1242_112_2_18_0_43_15_561,"MMS_PARSE: SendRecvFail, inform app retrying receiving , times = %d!")
TRACE_MSG(MMMSHTTP_1249_112_2_18_0_43_15_562,"MMS_PARSE: SendRecvFail, state error!")
TRACE_MSG(MMMSHTTP_1282_112_2_18_0_43_15_563,"MMS_PARSE: MmsLibNetAuthInd request does not match!")
TRACE_MSG(MMMSHTTP_1286_112_2_18_0_43_15_564,"MMS_PARSE: MmsLibNetAuthInd, auth type = %d")
TRACE_MSG(MMMSHTTP_1293_112_2_18_0_43_15_565,"MMS_PARSE: MmsLibNetAuthInd, send mms enter close request!")
TRACE_MSG(MMMSHTTP_1298_112_2_18_0_43_15_566,"MMS_PARSE: MmsLibNetAuthInd, recv mms need auth fail!")
TRACE_MSG(MMMSHTTP_1303_112_2_18_0_43_15_567,"MMS_PARSE: MmsLibNetAuthInd, recv rsp occurs error!")
TRACE_MSG(MMMSHTTP_1309_112_2_18_0_43_15_568,"MMS_PARSE: MmsLibNetAuthInd, state error!")
TRACE_MSG(MMMSHTTP_1340_112_2_18_0_43_15_569,"MMS_PARSE:MmsLibHttpGetCnf,input invalid!")
TRACE_MSG(MMMSHTTP_1350_112_2_18_0_43_15_570,"MMS_PARSE: MmsLibHttpGetCnf request does not match!, cnf request =%d")
TRACE_MSG(MMMSHTTP_1354_112_2_18_0_43_15_571,"MMS_PARSE:MmsLibHttpGetCnf,opt =%d")
TRACE_MSG(MMMSHTTP_1362_112_2_18_0_43_15_572,"MMS_PARSE:MmsLibHttpGetCnf,result =%d")
TRACE_MSG(MMMSHTTP_1377_112_2_18_0_43_15_573,"MMS_PARSE:MmsLibHttpGetCnf, buffer alloc error!")
TRACE_MSG(MMMSHTTP_1394_112_2_18_0_43_15_574,"MMS_PARSE:MmsLibHttpGetCnf, get header from sys fail!")
TRACE_MSG(MMMSHTTP_1408_112_2_18_0_43_15_575,"MMS_PARSE:MmsLibHttpGetCnf, recv is not mms!")
TRACE_MSG(MMMSHTTP_1460_112_2_18_0_43_15_576,"MMS_PARSE:MmsLibHttpGetCnf, create mms_doc_resp fail!")
TRACE_MSG(MMMSHTTP_1477_112_2_18_0_43_15_577,"MMS_PARSE:MmsLibHttpGetCnf, post rsp to server!")
TRACE_MSG(MMMSHTTP_1538_112_2_18_0_43_15_578,"MMS_PARSE:MmsLibHttpGetCnf, wsp post rsp to server!")
TRACE_MSG(MMMSHTTP_1546_112_2_18_0_43_15_579,"MMS_PARSE: MmsLibHttpGetCnf, get cnf error enter close request!")
TRACE_MSG(MMMSHTTP_1555_112_2_18_0_43_15_580,"MMS_PARSE: MmsLibHttpGetCnf, handle occurs error!")
TRACE_MSG(MMMSHTTP_1612_112_2_18_0_43_15_581,"MMS_PARSE: MmsLibHttpCancelCnf request does not match!")
TRACE_MSG(MMMSHTTP_1616_112_2_18_0_43_16_582,"MMS_PARSE:MmsLibHttpGetCnf,opt =%d")
TRACE_MSG(MMMSHTTP_1620_112_2_18_0_43_16_583,"MMS_PARSE: MmsLibHttpCancelCnf, get recv cancel cnf!")
TRACE_MSG(MMMSHTTP_1625_112_2_18_0_43_16_584,"MMS_PARSE: MmsLibHttpCancelCnf, get send cancel cnf!")
TRACE_MSG(MMMSHTTP_1743_112_2_18_0_43_16_585,"MMS_PARSE: StartHttpMmsAction, enter http get!")
TRACE_MSG(MMMSHTTP_1748_112_2_18_0_43_16_586,"MMS_PARSE:StartHttpMmsAction opt unknown!")
TRACE_MSG(MMMSHTTP_1793_112_2_18_0_43_16_587,"MMS_PARSE: StartWspMmsAction, enter wsp post!,size = %d")
TRACE_MSG(MMMSHTTP_1818_112_2_18_0_43_16_588,"MMS_PARSE: StartWspMmsAction, enter wsp get!,url = %s")
TRACE_MSG(MMMSHTTP_1823_112_2_18_0_43_16_589,"MMS_PARSE:StartWspMmsAction opt unknown!")
TRACE_MSG(MMMSMAIN_91_112_2_18_0_43_16_590,"MMS_PARSE: mmsAddNewAddr : addr list1 : %s")
TRACE_MSG(MMMSMAIN_146_112_2_18_0_43_16_591,"MMS_PARSE: mmsParseRecepientString : addr too int64 , cut ")
TRACE_MSG(MMMSMAIN_155_112_2_18_0_43_16_592,"MMS_PARSE: mmsParseRecepientString : too many addr , ignore")
TRACE_MSG(MMMSMAIN_346_112_2_18_0_43_17_593,"MMS_PARSE: mmsParseManual : get type mpeg4\n")
TRACE_MSG(MMMSMAIN_351_112_2_18_0_43_17_594,"MMS_PARSE: mmsParseManual : get type 3gpp\n")
TRACE_MSG(MMMSMAIN_356_112_2_18_0_43_17_595,"MMS_PARSE: mmsParseManual : get type video avi\n")
TRACE_MSG(MMMSMAIN_622_112_2_18_0_43_17_596,"MMS_PARSE: MMS_DeleteMms, input invalid!")
TRACE_MSG(MMMSMAIN_669_112_2_18_0_43_17_597,"MMS_PARSE: MMS_DeleteMms, delete smil file!")
TRACE_MSG(MMMSMAIN_987_112_2_18_0_43_18_598," MMS_PARSE: MMS_SetHeadField : before mmsParseRecepientString")
TRACE_MSG(MMMSMAIN_989_112_2_18_0_43_18_599," MMS_PARSE: MMS_SetHeadField : after mmsParseRecepientString")
TRACE_MSG(MMMSMAIN_1018_112_2_18_0_43_18_600,"MMS_PARSE:  MMS_SetHeadField : before mmsParseRecepientString")
TRACE_MSG(MMMSMAIN_1020_112_2_18_0_43_18_601,"MMS_PARSE:  MMS_SetHeadField : after mmsParseRecepientString")
TRACE_MSG(MMMSMAIN_1066_112_2_18_0_43_18_602,"MMS_PARSE:  MMS_SetHeadField : before mmsCompleteAddress")
TRACE_MSG(MMMSMAIN_1068_112_2_18_0_43_18_603,"MMS_PARSE:  MMS_SetHeadField : after mmsCompleteAddress")
TRACE_MSG(MMMSMAIN_1130_112_2_18_0_43_18_604," MMS_SetHeadField : the subject is null")
TRACE_MSG(MMMSMAIN_1149_112_2_18_0_43_18_605,"MMS_PARSE:  MMS_SetHeadField : subject len is %d ")
TRACE_MSG(MMMSMAIN_1184_112_2_18_0_43_18_606,"MMS_PARSE:  MMS_SetHeadField : before mmsParseRecepientString")
TRACE_MSG(MMMSMAIN_1187_112_2_18_0_43_18_607,"MMS_PARSE:  MMS_SetHeadField : after mmsParseRecepientString")
TRACE_MSG(MMMSMAIN_1468_112_2_18_0_43_19_608,"MMS_PARSE:  MMS_GetHeadField : return -1 ")
TRACE_MSG(MMMSMAIN_1478_112_2_18_0_43_19_609,"MMS_PARSE:  MMS_GetHeadField : first < 1f and ea ")
TRACE_MSG(MMMSMAIN_1491_112_2_18_0_43_19_610,"MMS_PARSE:  MMS_GetHeadField : real_uni_len = %d")
TRACE_MSG(MMMSMAIN_1527_112_2_18_0_43_19_611,"MMS_PARSE:  MMS_GetHeadField : real_uni_len = %d")
TRACE_MSG(MMMSMAIN_1548_112_2_18_0_43_19_612,"MMS_PARSE:  MMS_GetHeadField : MMS_SUBJECT::: first == 1f and 3333 ea ")
TRACE_MSG(MMMSMAIN_1573_112_2_18_0_43_19_613,"MMS_PARSE:  MMS_GetHeadField : real_uni_len = %d")
TRACE_MSG(MMMSMAIN_1592_112_2_18_0_43_19_614,"MMS_PARSE:  MMS_GetHeadField : MMS_SUBJECT::: else ")
TRACE_MSG(MMMSMAIN_1598_112_2_18_0_43_19_615,"MMS_PARSE:  MMS_GetHeadField : real_uni_len = %d")
TRACE_MSG(MMMSMAIN_1626_112_2_18_0_43_19_616,"MMS_PARSE:  MMS_GetHeadField : in to len = %d")
TRACE_MSG(MMMSMAIN_1644_112_2_18_0_43_19_617,"MMS_PARSE:  MMS_GetHeadField : in to before mmsStrcat")
TRACE_MSG(MMMSMAIN_1719_112_2_18_0_43_19_618,"MMS_PARSE:  mmsAddMmsContent : return 0 ")
TRACE_MSG(MMMSMAIN_1722_112_2_18_0_43_19_619,"MMS_PARSE:  mmsAddMmsContent : pList1->filename len = %d")
TRACE_MSG(MMMSMAIN_1866_112_2_18_0_43_20_620," MMS_PARSE: MMS_DecodeMms ,decode error!")
TRACE_MSG(MMMSMAIN_1871_112_2_18_0_43_20_621," MMS_PARSE: MMS_DecodeMms ,decode OK!")
TRACE_MSG(MMMSMAIN_1899_112_2_18_0_43_20_622,"MMS_PARSE: MMS_GetHeaderFromSysFile, input error!")
TRACE_MSG(MMMSMAIN_1907_112_2_18_0_43_20_623,"MMS_PARSE: MMS_GetHeaderFromSysFile, create file error!")
TRACE_MSG(MMMSMAIN_1914_112_2_18_0_43_20_624,"MMS_PARSE: MMS_GetHeaderFromSysFile, create file error!")
TRACE_MSG(MMMSMAIN_1925_112_2_18_0_43_20_625,"MMS_PARSE: MMS_GetHeaderFromSysFile, read file error!")
TRACE_MSG(MMMSMAIN_1971_112_2_18_0_43_20_626,"MMS_PARSE: MMS_DecodeMmsHead open decode file failed! ")
TRACE_MSG(MMMSMAIN_1979_112_2_18_0_43_20_627,"MMS_PARSE: MMS_DecodeMmsHead malloc failed")
TRACE_MSG(MMMSMAIN_2000_112_2_18_0_43_20_628,"MMS_PARSE: MMS_DecodeMmsHead,mms_cfa mmsfile size is :%d")
TRACE_MSG(MMMSMAIN_2003_112_2_18_0_43_20_629,"MMS_PARSE: MMS_DecodeMmsHead pMmsDecoder->m_long <5000")
TRACE_MSG(MMMSMAIN_2007_112_2_18_0_43_20_630,"MMS_PARSE: MMS_DecodeMmsHead,DOCTYPE wml PUBLIC")
TRACE_MSG(MMMSMAIN_2054_112_2_18_0_43_20_631,"MMS_PARSE:MMS_SendRecvInit, mms lib status =%d!/n")
TRACE_MSG(MMMSMAIN_2058_112_2_18_0_43_20_632,"MMS_PARSE:MMS_SendRecvInit, enter!/n")
TRACE_MSG(MMMSMAIN_2063_112_2_18_0_43_20_633,"MMS_PARSE:MMS_SendRecvInit, mms centry error!")
TRACE_MSG(MMMSMAIN_2150_112_2_18_0_43_20_634,"MMS_PARSE:MMS_SendRecvInit, wsp no proxy error!")
TRACE_MSG(MMMSMAIN_2407_112_2_18_0_43_21_635,"MMS_PARSE:MMS_SendRecvClose, enter close mms lib")
TRACE_MSG(MMMSMAIN_2431_112_2_18_0_43_21_636,"MMS_PARSE: MMS_Send input param ptr error!")
TRACE_MSG(MMMSMAIN_2436_112_2_18_0_43_21_637,"MMS_PARSE: MMS_Send input param vf file name error!")
TRACE_MSG(MMMSMAIN_2442_112_2_18_0_43_21_638,"MMS_PARSE: MMS_Send input param send max time error!")
TRACE_MSG(MMMSMAIN_2446_112_2_18_0_43_21_639,"MMS_PARSE: MMS_Send enter,send name = %s, cnf name = %s")
TRACE_MSG(MMMSMAIN_2506_112_2_18_0_43_21_640,"MMS_PARSE:  MMS_Send:call http/wsp to send!")
TRACE_MSG(MMMSMAIN_2529_112_2_18_0_43_21_641,"MMS_PARSE: MMS_ReceiveByFile, input invalid!")
TRACE_MSG(MMMSMAIN_2535_112_2_18_0_43_21_642,"MMS_PARSE: MMS_Receive input param send max time error!")
TRACE_MSG(MMMSMAIN_2539_112_2_18_0_43_21_643,"MMS_PARSE: MMS_ReceiveBy,VFFile name =%s")
TRACE_MSG(MMMSMAIN_2604_112_2_18_0_43_21_644,"MMS_PARSE: MMS_CancelSendRecv, cancel action")
TRACE_MSG(MMMSMAIN_2609_112_2_18_0_43_21_645,"MMS_PARSE:  MMS_CancelSendRecv, mms lib is not busy")
TRACE_MSG(MMMSMAIN_2660_112_2_18_0_43_21_646,"MMS_PARSE:GetSuffixByContentType, Content type = %s")
TRACE_MSG(MMMSMAIN_3108_112_2_18_0_43_22_647,"MMS_PARSE:  mmsStoreContent : length = %d")
TRACE_MSG(MMMSMAIN_3119_112_2_18_0_43_22_648,"MMS_PARSE:  mmsStoreContent : pBuffer is null")
TRACE_MSG(MMMSMAIN_3125_112_2_18_0_43_22_649,"MMS_PARSE:  mmsStoreContent : length:%d")
TRACE_MSG(MMMSMAIN_3178_112_2_18_0_43_22_650,"MMS_PARSE: access mmsStoreContent type is unknown, add no suffix")
TRACE_MSG(MMMSMAIN_3207_112_2_18_0_43_22_651,"MMS_PARSE: mmsStoreContent file has exist = %s")
TRACE_MSG(MMMSMAIN_3223_112_2_18_0_43_22_652,"MMS_PARSE: mmsStoreContent new file name = %s")
TRACE_MSG(MMMSMAIN_3352_112_2_18_0_43_23_653,"MMS_PARSE:  mmsStoreContent : pCon->type = %d")
TRACE_MSG(MMMSMAIN_3413_112_2_18_0_43_23_654,"MMS_PARSE:  mmsStoreContent : pList->char_set is %d")
TRACE_MSG(MMMSMAIN_3664_112_2_18_0_43_23_655,"MMS_PARSE:  MMS_ParsePushMsg : call start")
TRACE_MSG(MMMSMAIN_3666_112_2_18_0_43_23_656,"MMS_PARSE:  MMS_ParsePushMsg : call end")
TRACE_MSG(MMMSMAIN_3673_112_2_18_0_43_23_657,"MMS_PARSE:  MMS_ParsePushMsg : data_len = %d")
TRACE_MSG(MMMSMAIN_3676_112_2_18_0_43_23_658,"MMS_PARSE:  MMS_ParsePushMsg : is_exist_user_head = %d")
TRACE_MSG(MMMSMAIN_3689_112_2_18_0_43_23_659,"MMS_PARSE:  MMS_ParsePushMsg : ***sms message is not mms notification!!")
TRACE_MSG(MMMSMAIN_3725_112_2_18_0_43_23_660,"MMS_PARSE:  mmsOnPushIndication : mmsVFopen begin")
TRACE_MSG(MMMSMAIN_3727_112_2_18_0_43_23_661,"MMS_PARSE:  mmsOnPushIndication : mmsVFopen end")
TRACE_MSG(MMMSMAIN_3739_112_2_18_0_43_23_662,"MMS_PARSE: mmsOnPushIndication : mmsOnPushIndicate")
TRACE_MSG(MMMSMAIN_3828_112_2_18_0_43_24_663,"MMS_PARSE: mmsNameIdMapperGetbycid : pTemp->cid=%s")
TRACE_MSG(MMMSMAIN_3945_112_2_18_0_43_24_664,"MMS_PARSE:mmsAddAccessory, input invalid!")
TRACE_MSG(MMMSMAIN_4108_112_2_18_0_43_24_665,"MMS_PARSE: MMS_ReAlloc enter, input invalid!")
TRACE_MSG(MMMSMAIN_4144_112_2_18_0_43_24_666,"MMS_PARSE: MMS_MemAlloc enter, no function!")
TRACE_MSG(MMMSMAIN_4161_112_2_18_0_43_24_667,"MMS_PARSE: MMS_MemFree enter, no function!")
TRACE_MSG(MMMSMAIN_4390_112_2_18_0_43_25_668,"GUI_UTF82UCS2B: i %d char %x")
TRACE_MSG(MMMSNETWORK_106_112_2_18_0_43_25_669,"MMS_PARSE: mmsOnNotiIndicate : mms lib *** mms_urs is %s")
TRACE_MSG(MMMSNETWORK_123_112_2_18_0_43_25_670,"MMS_PARSE:mmsOnNotiIndicate, url= %s, msg size = %d")
TRACE_MSG(MMMSNETWORK_209_112_2_18_0_43_25_671,"MMS_PARSE: mmsOnPushIndicate : it is a dilivery indicate message")
TRACE_MSG(MMMSNETWORK_214_112_2_18_0_43_25_672,"MMS_PARSE: mmsOnPushIndicate : it is a notification indicate message")
TRACE_MSG(MMMSNETWORK_223_112_2_18_0_43_25_673,"MMS_PARSE: mmsOnPushIndicate : it is a invalid message")
TRACE_MSG(MMMSPARSER_2138_112_2_18_0_43_30_674,"MMS_PARSE: mmsParseFile : before parse mmsParseBlock")
TRACE_MSG(MMMSPARSER_2181_112_2_18_0_43_30_675,"MMS_PARSE: mmsParse : mmsParserInit")
TRACE_MSG(MMMSPARSER_2184_112_2_18_0_43_30_676,"MMS_PARSE: mmsParse :  after parse mmsParseFile")
TRACE_MSG(MMMSSMIL_1253_112_2_18_0_43_33_677,"MMS_PARSE: mmsSmilParseText : before call mmsGetEncodedLen ,len = %d,charset = %d")
TRACE_MSG(MMMSSMIL_1260_112_2_18_0_43_33_678,"MMS_PARSE: mmsSmilParseText : after call mmsGetEncodedLen, len = %d ")
TRACE_MSG(MMMSSMIL_1444_112_2_18_0_43_33_679,"MMS_PARSE: mmsSmilParseSub : SMIL_EL_AUDIO")
TRACE_MSG(MMMSSMIL_1482_112_2_18_0_43_33_680,"MMS_PARSE: mmsSmilParse : before smil mmsParse")
TRACE_MSG(MMMSSMIL_1484_112_2_18_0_43_33_681,"MMS_PARSE: mmsSmilParse : after smil mmsParse")
TRACE_MSG(MMMSSMIL_1486_112_2_18_0_43_33_682,"MMS_PARSE: mmsSmilParse : after smil mmsSmilParseSub")
TRACE_MSG(MMMSSMIL_1932_112_2_18_0_43_34_683,"MMS_PARSE: mmsToSmil access MMS_IntegrateMmsBody")
TRACE_MSG(MMMSSMIL_2195_112_2_18_0_43_35_684,"MMS_PARSE: MMS_IntegrateMmsBody : mpeg4 type MMSSmil.c \n")
TRACE_MSG(MMMSSMIL_2200_112_2_18_0_43_35_685,"MMS_PARSE: MMS_IntegrateMmsBody : 3gpp type MMSSmil.c \n")
TRACE_MSG(MMMSSMIL_2205_112_2_18_0_43_35_686,"MMS_PARSE: MMS_IntegrateMmsBody : avi type MMSSmil.c \n")
TRACE_MSG(MMMSSMIL_2254_112_2_18_0_43_35_687,"MMS_PARSE: MMS_IntegrateMmsBody :  while branch")
TRACE_MSG(MMMSSMIL_2291_112_2_18_0_43_35_688,"MMS_PARSE: MMS_GetEditBody, input invalid!")
TRACE_MSG(MMMSSMIL_2308_112_2_18_0_43_35_689,"MMS_PARSE: MMS_GetEditBody ftype==1 ")
TRACE_MSG(MMMSSMIL_2311_112_2_18_0_43_35_690,"MMS_PARSE: MMS_GetEditBody : num is %d")
TRACE_MSG(MMMSSMIL_2318_112_2_18_0_43_35_691,"MMS_PARSE: MMS_GetEditBody, mms type invalid!")
TRACE_MSG(MMMSSMIL_2322_112_2_18_0_43_35_692,"MMS_PARSE: MMS_GetEditBody : before mmsStoreContent")
TRACE_MSG(MMMSSMIL_2324_112_2_18_0_43_35_693,"MMS_PARSE: MMS_GetEditBody : after store mms")
TRACE_MSG(MMMSSMIL_2428_112_2_18_0_43_35_694,"MMS_PARSE: MMS_GetEditBody : no smil and only accessory!")
TRACE_MSG(MMMSSMIL_2436_112_2_18_0_43_35_695,"MMS_PARSE: MMS_GetEditBody : ftype==1 ")
TRACE_MSG(MMMSSMIL_2439_112_2_18_0_43_35_696,"MMS_PARSE: MMS_GetEditBody : ftype is %d")
TRACE_MSG(MMMSSMIL_2444_112_2_18_0_43_35_697,"MMS_PARSE: MMS_GetEditBody : after smil parse")
TRACE_MSG(MMMSSMIL_2461_112_2_18_0_43_35_698,"MMS_PARSE: MMS_GetEditBody :  reparse mms manually")
TRACE_MSG(MMMSSMIL_2466_112_2_18_0_43_35_699,"MMS_PARSE: MMS_GetEditBody : before mmsSetEdocDrm")
TRACE_MSG(MMMSSMIL_2470_112_2_18_0_43_35_700,"MMS_PARSE: MMS_GetEditBody : before MMS_DeleteMms")
TRACE_MSG(MMMSSMIL_2474_112_2_18_0_43_35_701,"MMS_PARSE: MMS_GetEditBody : before mmsSetContentLayout")
TRACE_MSG(SYSMEMFILE_86_112_2_18_0_43_36_702,"MMS_PARSE: sysVFileInitInfo, init info =%x")
TRACE_MSG(SYSMEMFILE_91_112_2_18_0_43_36_703,"MMS_PARSE: sysVFileInitInfo, init failed!")
TRACE_MSG(SYSMEMFILE_199_112_2_18_0_43_36_704,"MMS_PARSE: sysVFileDelNode : has no content or the node to be delete")
TRACE_MSG(SYSMEMFILE_207_112_2_18_0_43_36_705,"MMS_PARSE: sysVFileDelNode :no file in list!")
TRACE_MSG(SYSMEMFILE_241_112_2_18_0_43_36_706,"MMS_PARSE: sysVFileDelNode : not found ")
TRACE_MSG(SYSMEMFILE_364_112_2_18_0_43_36_707,"MMS_PARSE: sysVFopen : open file name = %s")
TRACE_MSG(SYSMEMFILE_377_112_2_18_0_43_36_708,"MMS_PARSE: sysVFopen : FileName is too long")
TRACE_MSG(SYSMEMFILE_402_112_2_18_0_43_36_709,"MMS_PARSE: sysVFopen : file %s already exists")
TRACE_MSG(SYSMEMFILE_420_112_2_18_0_43_36_710,"MMS_PARSE: sysVFopen : FileName is too long")
TRACE_MSG(SYSMEMFILE_431_112_2_18_0_43_36_711,"MMS_PARSE: sysVFopen : add new node fial!")
TRACE_MSG(SYSMEMFILE_448_112_2_18_0_43_36_712,"MMS_PARSE: sysVFopen : File %s not found")
TRACE_MSG(SYSMEMFILE_490_112_2_18_0_43_36_713,"MMS_PARSE: sysVFread  : WarningFile %d not found")
TRACE_MSG(SYSMEMFILE_543_112_2_18_0_43_36_714,"MMS_PARSE: sysVFwrite : Memory error!")
TRACE_MSG(SYSMEMFILE_553_112_2_18_0_43_36_715,"MMS_PARSE: sysVFwrite : File %d not found")
TRACE_MSG(SYSMEMFILE_582_112_2_18_0_43_37_716,"MMS_PARSE: sysVFseek : unknown seek origin %d")
TRACE_MSG(SYSMEMFILE_588_112_2_18_0_43_37_717,"MMS_PARSE: sysVFseek : File %d not found!")
TRACE_MSG(SYSMEMFILE_618_112_2_18_0_43_37_718,"MMS_PARSE: Waringing sysVFtell : File %d not found!\n")
TRACE_MSG(SYSMEMFILE_637_112_2_18_0_43_37_719,"MMS_PARSE: sysVFremove : remove file name = %s")
TRACE_MSG(SYSMEMFILE_645_112_2_18_0_43_37_720,"MMS_PARSE: sysVFremove : File %s not found")
TRACE_MSG(SYSMEMFILE_661_112_2_18_0_43_37_721,"MMMS_PARSE: sysVFSetBufPointer enter,name = %s, size =%d")
TRACE_MSG(SYSMEMFILE_669_112_2_18_0_43_37_722,"MMMS_PARSE: sysVFSetBufPointer, memory alloc fail!")
TRACE_MSG(SYSMEMFILE_681_112_2_18_0_43_37_723,"MMMS_PARSE: sysVFSetBufPointer, error tmp null! name = %s, size =%d")
TRACE_MSG(SYSMEMFILE_697_112_2_18_0_43_37_724,"MMS_PARSE: sysVFRemoveByHandle : file handle %d not found")
TRACE_MSG(SYSMEMFILE_716_112_2_18_0_43_37_725,"MMS_PARSE: sysVFclose : file %d not found")
TRACE_MSG(SYSMEMFILE_735_112_2_18_0_43_37_726,"MMS_PARSE: setfiledata the data is not NULL")
TRACE_MSG(SYSMEMFILE_739_112_2_18_0_43_37_727,"MMS_PARSE: setfiledata the data is NULL")
TRACE_MSG(SYSMEMFILE_889_112_2_18_0_43_37_728,"MMS_PARSE: sysVFGetFileInfo : wrong parma")
TRACE_MSG(SYSMEMFILE_905_112_2_18_0_43_37_729,"MMS_PARSE: sysVFGetFileInfo : error ,open file ")
TRACE_MSG(SYSMEMFILE_918_112_2_18_0_43_37_730,"MMS_PARSE: sysVFCreateFile : input invalid!")
TRACE_MSG(SYSMEMFILE_925_112_2_18_0_43_37_731,"MMS_PARSE: sysVFCreateFile : file %s already exists")
TRACE_MSG(SYSMEMFILE_935_112_2_18_0_43_37_732,"MMS_PARSE: sysVFCreateFile : FileName is too long")
TRACE_MSG(SYSMEMFILE_963_112_2_18_0_43_37_733,"MMS_PARSE: sysVFDestroyFile : input invalid!")
TRACE_MSG(SYSMEMFILE_976_112_2_18_0_43_37_734,"MMS_PARSE: sysVFDestroyFile : File %s not found")
TRACE_MSG(SYSMEMFILE_985_112_2_18_0_43_37_735,"MMS_PARSE: Access sysVFGetBufPointer in")
END_TRACE_MAP(PARSER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _PARSER_TRC_H_

