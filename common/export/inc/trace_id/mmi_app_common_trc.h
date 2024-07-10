/******************************************************************************
 ** File Name:      mmi_app_common_trc.h                                         *
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
//trace_id:144
#ifndef _MMI_APP_COMMON_TRC_H_
#define _MMI_APP_COMMON_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_COMMON_597_112_2_18_1_58_10_0 "MMIAPICOM_DelDirAll path_len =%d,path_buf_len=%d"
#define MMI_COMMON_605_112_2_18_1_58_11_1 "MMIAPICOM_DelDirAll path_len =%d,"
#define MMI_COMMON_674_112_2_18_1_58_11_2 "MMIAPICOM_GenPartyNumber PNULL == tele_num_ptr || PNULL == party_num"
#define MMI_COMMON_681_112_2_18_1_58_11_3 "MMI_GenPartyNumber  length = 0\n"
#define MMI_COMMON_741_112_2_18_1_58_11_4 "MMIAPICOM_ConvertWStr2Num PNULL == wnum_str || PNULL == bcdnum_ptr"
#define MMI_COMMON_749_112_2_18_1_58_11_5 "MMI_GenPartyNumber  length = 0\n"
#define MMI_COMMON_865_112_2_18_1_58_11_6 "MMIAPICOM_GenDispNumber max_tele_len = %d, party_len = %d"
#define MMI_COMMON_920_112_2_18_1_58_11_7 "MMIAPICOM_GenDispNumber max_tele_len = %d "
#define MMI_COMMON_945_112_2_18_1_58_11_8 "MMI_GenDispNumber: called_num->num_len = 0"
#define MMI_COMMON_1002_112_2_18_1_58_11_9 "MMIAPICOM_SplitSuffix PNULL == addsuffix_name_ptr"
#define MMI_COMMON_1093_112_2_18_1_58_11_10 "MMIAPICOM_IsPhoneNumber ret %d"
#define MMI_COMMON_1184_112_2_18_1_58_12_11 "MMIAPICOM_GetLcdDevInfo PNULL==lcd_dev_info"
#define MMI_COMMON_1242_112_2_18_1_58_12_12 "MMIAPICOM_GetFullPathByFilename invalid !"
#define MMI_COMMON_1407_112_2_18_1_58_12_13 "1 full_file_name_len %d m %d"
#define MMI_COMMON_1418_112_2_18_1_58_12_14 "full_file_name_len %d m %d"
#define MMI_COMMON_1427_112_2_18_1_58_12_15 "file_name_len %d m %d"
#define MMI_COMMON_1548_112_2_18_1_58_12_16 "MMIAPICOM_GetLongFileNameAndSuffix PNULL == ucs_full_name_ptr || (PNULL == ucs_suffix_ptr && PNULL == ucs_name_ptr)"
#define MMI_COMMON_1554_112_2_18_1_58_12_17 "GetFileNameAndSuffix: the file length is %d"
#define MMI_COMMON_1611_112_2_18_1_58_13_18 "GetFileNameAndSuffix: the value m is %d, no suffix file name len is %d,the suffix len is %d"
#define MMI_COMMON_1631_112_2_18_1_58_13_19 "MMIAPICOM_GetFileNameAndSuffix PNULL == ucs_full_name_ptr || ( PNULL == ucs_suffix_ptr && PNULL == ucs_name_ptr)"
#define MMI_COMMON_1636_112_2_18_1_58_13_20 "GetFileNameAndSuffix: the file length is %d"
#define MMI_COMMON_1685_112_2_18_1_58_13_21 "GetFileNameAndSuffix: the value m is %d, no suffix file name len is %d,the suffix len is %d"
#define MMI_COMMON_1704_112_2_18_1_58_13_22 "MMIAPICOM_IsSameFileType PNULL == file_suffix_ptr || PNULL == type_suffix_ptr"
#define MMI_COMMON_1750_112_2_18_1_58_13_23 "MMIAPICOM_GetNameAndSuffix PNULL == name_suffix_ptr || 0 == name_suffix_len || (PNULL==name_ptr && PNULL == suffix_ptr)"
#define MMI_COMMON_1801_112_2_18_1_58_13_24 "MMIAPICOM_FileNameIsValid PNULL == file_name_ptr"
#define MMI_COMMON_1848_112_2_18_1_58_13_25 "MMIAPICOM_GetImgaeType PNULL == full_path_ptr"
#define MMI_COMMON_2028_112_2_18_1_58_13_26 "MMIAPICOM_GetMovieType PNULL == file_name_ptr || 0 == name_len"
#define MMI_COMMON_2140_112_2_18_1_58_14_27 "MMIAPICOM_CreateRandFile PNULL == path_ptr || 0 == path_len || PNULL == file_name_ptr|| PNULL == file_name_len_ptr"
#define MMI_COMMON_2147_112_2_18_1_58_14_28 "MMIAPICOM_GenPartyNumber1 PNULL == temp_wstr"
#define MMI_COMMON_2154_112_2_18_1_58_14_29 "MMIAPICOM_GenPartyNumber2 PNULL == temp_str"
#define MMI_COMMON_2214_112_2_18_1_58_14_30 "MMIAPICOM_GetRandFile, success!!"
#define MMI_COMMON_2219_112_2_18_1_58_14_31 "MMIAPICOM_GetRandFile, fail!!"
#define MMI_COMMON_2266_112_2_18_1_58_14_32 "MMIAPICOM_CreateRandFileAsyn PNULL == path_ptr || 0 == path_len || PNULL == file_name_ptr || PNULL == file_name_len_ptr"
#define MMI_COMMON_2274_112_2_18_1_58_14_33 "MMIAPICOM_CreateRandFileAsyn PNULL == temp_wstr"
#define MMI_COMMON_2281_112_2_18_1_58_14_34 "MMIAPICOM_CreateRandFileAsyn PNULL == temp_str"
#define MMI_COMMON_2336_112_2_18_1_58_14_35 "MMIAPICOM_GetRandFile, success!!"
#define MMI_COMMON_3036_112_2_18_1_58_15_36 "MMI_WSTRNCPY: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d"
#define MMI_COMMON_3071_112_2_18_1_58_15_37 "MMI_STRTOWSTR: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d"
#define MMI_COMMON_3111_112_2_18_1_58_16_38 "MMI_WSTRNTOSTR: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d"
#define MMI_COMMON_3169_112_2_18_1_58_16_39 "MMIAPICOM_WstrTraceOut str is %s\n"
#define MMI_COMMON_3354_112_2_18_1_58_16_40 "MMIAPICOM_CombinePath PNULL == full_path_ptr || PNULL == full_path_len_ptr"
#define MMI_COMMON_3500_112_2_18_1_58_16_41 "MMICOM: key = 0x%x result= %d"
#define MMI_COMMON_3594_112_2_18_1_58_16_42 "MMI_GetFreeSpace: free_space = %d K bytes"
#define MMI_COMMON_3890_112_2_18_1_58_17_43 "[mmi_common.c] MMIAPICOM_BinSearch search_info 0x%x  func 0x%x !"
#define MMI_COMMON_3963_112_2_18_1_58_17_44 "[mmi_common.c] MMIAPICOM_BinSearch cmp_result %d, ret_pos %d is_in_list %d mid_pos %d"
#define MMI_COMMON_4018_112_2_18_1_58_17_45 "MMIAPIPTSCR_WriteBMPFile MMIAPIFMM_CreateFile fail!"
#define MMI_COMMON_4053_112_2_18_1_58_17_46 "MMIAPIPTSCR_WriteBMPFile WriteBMPFile bmp is to large! w = %d, nLineBytes = %d"
#define MMI_COMMON_4075_112_2_18_1_58_17_47 "MMIAPIPTSCR_WriteBMPFile WriteBMPFile alloc fail!"
#define MMI_COMMON_4336_112_2_18_1_58_18_48 "MMIAPICOM_GetAudioPri entry: audio_type_str=0x%x"
#define MMI_COMMON_4352_112_2_18_1_58_18_49 "MMIAPICOM_GetAudioPri exit: pri=%d"
#define MMI_PUBMENUWIN_277_112_2_18_1_58_38_50 "MMIPUB_OpenMenuWin:win_id %d is already open!"
#define MMI_PUBRICHTEXTWIN_243_112_2_18_1_58_38_51 "MMIPUB_OpenRichTextWin:win_id %d is already open!"
#define MMI_PUBTEXTWIN_245_112_2_18_1_58_39_52 "MMIPUB_OpenTextWin:win_id %d is already open!"
#define MMI_PUBWIN_2101_112_2_18_1_58_43_53 "OpenAlertWin:win_id %d is already open!"
#define MMI_PUBWIN_2424_112_2_18_1_58_44_54 "MMIPUB_OpenAlertWinByTextId:win_id %d is already open!"
#define MMI_PUBWIN_2450_112_2_18_1_58_44_55 "MMIPUB_HandleProgressWinMsg, win_id = %d, msg_id = %x"
#define MMI_PUBWIN_2669_112_2_18_1_58_45_56 "MMIPUB_HandleAlertWinMsg, win_id = %d, msg_id = %x"
#define MMI_PUBWIN_3713_112_2_18_1_58_47_57 "MMIPUB_SetWinSoftkeyEx:win_id %d is not open!"
#define MMI_PUBWIN_3753_112_2_18_1_58_47_58 "MMIPUB_SetWinSoftkeyEx:win_id %d is not open!"
#define MMI_PUBWIN_3794_112_2_18_1_58_47_59 "MMIPUB_SetWinSoftkeyGray:win_id %d is failed!"
#define MMI_PUBWIN_4062_112_2_18_1_58_47_60 "OpenQueryWin:win_id %d is already open!"
#define MMI_PUBWIN_4117_112_2_18_1_58_47_61 "MMIPUB_HandleQueryWinMsg, win_id = %d, msg_id = %x"
#define MMI_PUBWIN_4313_112_2_18_1_58_48_62 "OpenWaitWin:win_id %d is already open!"
#define MMI_PUBWIN_4435_112_2_18_1_58_48_63 "MMIPUB_HandleWaitWinMsg, win_id = %d, msg_id = %x"
#define MMI_PUBWIN_4501_112_2_18_1_58_48_64 "MMIPUB_SetWaitWinText:win_id %d is not open!"
#define MMI_PUBWIN_5081_112_2_18_1_58_50_65 "OpenAlertTextWin:win_id %d is already open!"
#define MMI_PUBWIN_5285_112_2_18_1_58_50_66 "OpenQueryTextWin:win_id %d is already open!"
#define MMI_PUBWIN_5659_112_2_18_1_58_51_67 "GetTipsDisRect, cur dis_style error, dis_style = %d"
#define MMI_PUBWIN_6289_112_2_18_1_58_53_68 "MMIPUB_OpenEditWin param error"
#define MMI_PUBWIN_6390_112_2_18_1_58_53_69 "MMIPUB_OpenWin:win_id %d is already open!"
#define MMI_PUBWIN_6613_112_2_18_1_58_53_70 "MMIPUB_OpenTempWaitWin() MMK_CreateWindow fail!"
#define MMI_SETLIST_WIN_113_112_2_18_1_58_55_71 "SetPopWinTitle error input param!"
#define MMI_SETLIST_WIN_199_112_2_18_1_58_55_72 "HandleSetlistPopWinMsg error input param!"
#define MMI_SETLIST_WIN_308_112_2_18_1_58_55_73 "SetPopWinListBox input param is null!"
#define MMI_SLIDE_2773_112_2_18_1_59_2_74 "%s %s\n"
#define MMI_SOFTKEYTEST_153_112_2_18_1_59_3_75 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID"
#define MMI_SOFTKEYTEST_158_112_2_18_1_59_3_76 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
#define MMI_SOFTKEYTEST_163_112_2_18_1_59_3_77 "HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID"
#define MMICOM_DATA_1015_112_2_18_1_59_6_78 "0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, n"
#define MMICOM_DATA_1024_112_2_18_1_59_6_79 "0x%02x,"
#define MMICOM_DATA_1027_112_2_18_1_59_6_80 "\n"
#define MMICOM_DATA_1107_112_2_18_1_59_6_81 "invalid cValue"
#define MMICOM_EXTRACT_130_112_2_18_1_59_7_82 "HandleExtractNumberPopWinMsg msg_id = 0x%x, dual_sys=%d"
#define MMICOM_EXTRACT_342_112_2_18_1_59_7_83 "MMIUSSSD: HandleExtractUrlPopWinMsg msg_id = 0x%x, dual_sys=%d"
#define MMICOM_EXTRACT_481_112_2_18_1_59_7_84 "HandleExtractEmailPopWinMsg msg_id = 0x%x, dual_sys=%d"
#define MMICOM_PANEL_210_112_2_18_1_59_8_85 "[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin Panel is opened!"
#define MMICOM_PANEL_231_112_2_18_1_59_9_86 "[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin open other panel!"
#define MMICOM_PANEL_235_112_2_18_1_59_9_87 "[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin cur=%d, min_value = %d, max=%d, type=%d!"
#define MMICOM_PANEL_240_112_2_18_1_59_9_88 "[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin error max_value==0"
#define MMICOM_PANEL_903_112_2_18_1_59_10_89 "[MMICOMPANEL]:panel_type error. type=%d"
#define MMICOM_STRING_368_112_2_18_1_59_12_90 "MMIAPICOM_Ascii2default PNULL == ascii_ptr || PNULL == default_ptr!"
#define MMICOM_STRING_416_112_2_18_1_59_12_91 "MMIAPICOM_Default2Ascii PNULL == ascii_ptr || PNULL == default_ptr!"
#define MMICOM_STRING_468_112_2_18_1_59_12_92 "MMIAPICOM_DefaultToWstr PNULL == ascii_ptr || PNULL == default_ptr!"
#define MMICOM_STRING_553_112_2_18_1_59_12_93 "MMIAPICOM_MergeTwoString PNULL == string_1 || PNULL == string_2 || PNULL == string_3"
#define MMICOM_STRING_646_112_2_18_1_59_12_94 "MMIAPICOM_StrcatFromSTRINGToUCS2 PNULL == ucs2_str_ptr || PNULL == ucs2_str_len_ptr || PNULL == string_ptr"
#define MMICOM_STRING_669_112_2_18_1_59_12_95 "MMIAPICOM_StrcatFromStrToUCS2 PNULL == ucs2_str_ptr || PNULL == ucs2_str_len_ptr || PNULL == str_ptr"
#define MMICOM_STRING_691_112_2_18_1_59_12_96 "MMIAPICOM_GetInt32ValueFromBuf PNULL == dataPtr"
#define MMICOM_STRING_716_112_2_18_1_59_12_97 "MMIAPICOM_GetInt8ValueFromBuf PNULL == dataPtr"
#define MMICOM_STRING_740_112_2_18_1_59_12_98 "MMIAPICOM_GetInt16ValueFromBuf PNULL == dataPtr"
#define MMICOM_STRING_790_112_2_18_1_59_12_99 "MMIAPICOM_CompareTwoWstrExt PNULL == wstr1_ptr || PNULL == wstr2_ptr"
#define MMICOM_STRING_894_112_2_18_1_59_13_100 "MMIAPICOM_CopyString PNULL == out_len_ptr || PNULL == out_alpha_ptr || PNULL == in_alpha_ptr"
#define MMICOM_STRING_1035_112_2_18_1_59_13_101 "MMIAPICOM_CopyString PNULL == i1_text_ptr"
#define MMICOM_STRING_1069_112_2_18_1_59_13_102 "PNULL != str_ptr, PNULL != str_info_ptr"
#define MMICOM_STRING_1103_112_2_18_1_59_13_103 "PNULL != str, PNULL != num"
#define MMICOM_STRING_1142_112_2_18_1_59_13_104 "MMIAPICOM_CountDefaultExt PNULL == ascii_ptr"
#define MMICOM_STRING_1209_112_2_18_1_59_13_105 "PNULL != num_string.wstr_ptr"
#define MMICOM_STRING_1251_112_2_18_1_59_13_106 "MMIAPICOM_ConvertAlphabetToUnicode PNULL == i1_default_text_ptr || PNULL == o1_unicode_text_ptr"
#define MMICOM_STRING_1287_112_2_18_1_59_13_107 "MMIAPICOM_Int2Str PNULL == buf || buf_len <2"
#define MMICOM_STRING_1327_112_2_18_1_59_14_108 "MMIAPICOM_CountUcs2Char PNULL == txt_ptr"
#define MMICOM_STRING_1394_112_2_18_1_59_14_109 "MMIAPICOM_IsNumberString PNULL == cmd_char"
#define MMICOM_STRING_1424_112_2_18_1_59_14_110 "MMIAPICOM_ConvertCmdChar2Num PNULL == cmd_char"
#define MMICOM_STRING_1437_112_2_18_1_59_14_111 "MMIAPICOM_ConvertCmdChar2Num u8_char=%d,i=%d"
#define MMICOM_STRING_1494_112_2_18_1_59_14_112 "MMIAPICOM_GBToSTRING PNULL == src_ptr || PNULL == des_str"
#define MMICOM_STRING_1549_112_2_18_1_59_14_113 "MMIAPICOM_GetStrWidth PNULL == str_ptr || PNULL == str_len_ptr || font_type >= GUI_MAX_FONT_NUM"
#define MMICOM_STRING_1575_112_2_18_1_59_14_114 "MMIAPICOM_GetStrWidth str_width= %d,str_space=%d,font_width=%d"
#define MMICOM_STRING_1603_112_2_18_1_59_14_115 "MMIAPICOM_ParseSimStr PNULL == sim_str || PNULL == mmi_str->wstr_ptr || mmi_buf_len == 0 "
#define MMICOM_STRING_1608_112_2_18_1_59_14_116 "[MMICOM] sim_str_len = %d"
#define MMICOM_STRING_1768_112_2_18_1_59_14_117 "[MMICOM] mmi_str->length = %d"
#define MMICOM_STRING_1782_112_2_18_1_59_14_118 "MMIAPICOM_CopyStr2Buf PNULL == dst_buf_ptr || PNULL == src_ptr "
#define MMICOM_STRING_1815_112_2_18_1_59_14_119 "[MMICOM]:MMIAPICOM_CatTwoString fail, dst_ptr=%d,src1_ptr=%d"
#define MMICOM_STRING_1821_112_2_18_1_59_14_120 "[MMICOM]:MMIAPICOM_CatTwoString fail, len1=%d,len2=%d"
#define MMICOM_STRING_1838_112_2_18_1_59_15_121 "[MMICOM]:MMIAPICOM_CatTwoString fail, malloc for dst_ptr->str_ptr fail!"
#define MMICOM_STRING_1863_112_2_18_1_59_15_122 "warning CatStringByTextID:dst_ptr=%d"
#define MMICOM_STRING_1887_112_2_18_1_59_15_123 "warning CatStringByTextID2 dst_ptr NULL"
#define MMICOM_STRING_1938_112_2_18_1_59_15_124 "MMIAPICOM_EncodeGSMAlphabet alpha_type  %d !"
#define MMICOM_STRING_1967_112_2_18_1_59_15_125 "MMIAPICOM_DecodeGSMAlphabet invalid parameter !"
#define MMICOM_STRING_2513_112_2_18_1_59_16_126 "MMIAPICOM_GetGSMAlphabetLen encode_type %d wchar_len %d alpha_len %d"
#define MMICOM_STRING_2657_112_2_18_1_59_16_127 "MMIAPICOM_Wchar2default PNULL == wchar_ptr || PNULL == default_ptr!"
#define MMICOM_STRING_2775_112_2_18_1_59_16_128 "MMIAPICOM_Default2Wchar PNULL == wchar_ptr || PNULL == default_ptr!"
#define MMICOM_TIME_140_112_2_18_1_59_17_129 "CheckDayOfDateIsValid:date.date_month = %d is error!"
#define MMICOM_TIME_194_112_2_18_1_59_17_130 "year = %d, mon = %d, day = %d, hour = %d, min = %d, sec = %d "
#define MMICOM_TIME_340_112_2_18_1_59_18_131 "MMI_COMMON:tm_mday = %d,tm_mon = %d,tm_year = %dn"
#define MMICOM_TIME_414_112_2_18_1_59_18_132 "MMI_COMMON:error leap_year = %d\n"
#define MMICOM_TIME_438_112_2_18_1_59_18_133 "MMI_COMMON:error day = %d\n"
#define MMICOM_TIME_468_112_2_18_1_59_18_134 "MMI_COMMON:error month = %d\n"
#define MMICOM_TIME_610_112_2_18_1_59_18_135 "GetRandomTimer return timer = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_COMMON_TRC)
TRACE_MSG(MMI_COMMON_597_112_2_18_1_58_10_0,"MMIAPICOM_DelDirAll path_len =%d,path_buf_len=%d")
TRACE_MSG(MMI_COMMON_605_112_2_18_1_58_11_1,"MMIAPICOM_DelDirAll path_len =%d,")
TRACE_MSG(MMI_COMMON_674_112_2_18_1_58_11_2,"MMIAPICOM_GenPartyNumber PNULL == tele_num_ptr || PNULL == party_num")
TRACE_MSG(MMI_COMMON_681_112_2_18_1_58_11_3,"MMI_GenPartyNumber  length = 0\n")
TRACE_MSG(MMI_COMMON_741_112_2_18_1_58_11_4,"MMIAPICOM_ConvertWStr2Num PNULL == wnum_str || PNULL == bcdnum_ptr")
TRACE_MSG(MMI_COMMON_749_112_2_18_1_58_11_5,"MMI_GenPartyNumber  length = 0\n")
TRACE_MSG(MMI_COMMON_865_112_2_18_1_58_11_6,"MMIAPICOM_GenDispNumber max_tele_len = %d, party_len = %d")
TRACE_MSG(MMI_COMMON_920_112_2_18_1_58_11_7,"MMIAPICOM_GenDispNumber max_tele_len = %d ")
TRACE_MSG(MMI_COMMON_945_112_2_18_1_58_11_8,"MMI_GenDispNumber: called_num->num_len = 0")
TRACE_MSG(MMI_COMMON_1002_112_2_18_1_58_11_9,"MMIAPICOM_SplitSuffix PNULL == addsuffix_name_ptr")
TRACE_MSG(MMI_COMMON_1093_112_2_18_1_58_11_10,"MMIAPICOM_IsPhoneNumber ret %d")
TRACE_MSG(MMI_COMMON_1184_112_2_18_1_58_12_11,"MMIAPICOM_GetLcdDevInfo PNULL==lcd_dev_info")
TRACE_MSG(MMI_COMMON_1242_112_2_18_1_58_12_12,"MMIAPICOM_GetFullPathByFilename invalid !")
TRACE_MSG(MMI_COMMON_1407_112_2_18_1_58_12_13,"1 full_file_name_len %d m %d")
TRACE_MSG(MMI_COMMON_1418_112_2_18_1_58_12_14,"full_file_name_len %d m %d")
TRACE_MSG(MMI_COMMON_1427_112_2_18_1_58_12_15,"file_name_len %d m %d")
TRACE_MSG(MMI_COMMON_1548_112_2_18_1_58_12_16,"MMIAPICOM_GetLongFileNameAndSuffix PNULL == ucs_full_name_ptr || (PNULL == ucs_suffix_ptr && PNULL == ucs_name_ptr)")
TRACE_MSG(MMI_COMMON_1554_112_2_18_1_58_12_17,"GetFileNameAndSuffix: the file length is %d")
TRACE_MSG(MMI_COMMON_1611_112_2_18_1_58_13_18,"GetFileNameAndSuffix: the value m is %d, no suffix file name len is %d,the suffix len is %d")
TRACE_MSG(MMI_COMMON_1631_112_2_18_1_58_13_19,"MMIAPICOM_GetFileNameAndSuffix PNULL == ucs_full_name_ptr || ( PNULL == ucs_suffix_ptr && PNULL == ucs_name_ptr)")
TRACE_MSG(MMI_COMMON_1636_112_2_18_1_58_13_20,"GetFileNameAndSuffix: the file length is %d")
TRACE_MSG(MMI_COMMON_1685_112_2_18_1_58_13_21,"GetFileNameAndSuffix: the value m is %d, no suffix file name len is %d,the suffix len is %d")
TRACE_MSG(MMI_COMMON_1704_112_2_18_1_58_13_22,"MMIAPICOM_IsSameFileType PNULL == file_suffix_ptr || PNULL == type_suffix_ptr")
TRACE_MSG(MMI_COMMON_1750_112_2_18_1_58_13_23,"MMIAPICOM_GetNameAndSuffix PNULL == name_suffix_ptr || 0 == name_suffix_len || (PNULL==name_ptr && PNULL == suffix_ptr)")
TRACE_MSG(MMI_COMMON_1801_112_2_18_1_58_13_24,"MMIAPICOM_FileNameIsValid PNULL == file_name_ptr")
TRACE_MSG(MMI_COMMON_1848_112_2_18_1_58_13_25,"MMIAPICOM_GetImgaeType PNULL == full_path_ptr")
TRACE_MSG(MMI_COMMON_2028_112_2_18_1_58_13_26,"MMIAPICOM_GetMovieType PNULL == file_name_ptr || 0 == name_len")
TRACE_MSG(MMI_COMMON_2140_112_2_18_1_58_14_27,"MMIAPICOM_CreateRandFile PNULL == path_ptr || 0 == path_len || PNULL == file_name_ptr|| PNULL == file_name_len_ptr")
TRACE_MSG(MMI_COMMON_2147_112_2_18_1_58_14_28,"MMIAPICOM_GenPartyNumber1 PNULL == temp_wstr")
TRACE_MSG(MMI_COMMON_2154_112_2_18_1_58_14_29,"MMIAPICOM_GenPartyNumber2 PNULL == temp_str")
TRACE_MSG(MMI_COMMON_2214_112_2_18_1_58_14_30,"MMIAPICOM_GetRandFile, success!!")
TRACE_MSG(MMI_COMMON_2219_112_2_18_1_58_14_31,"MMIAPICOM_GetRandFile, fail!!")
TRACE_MSG(MMI_COMMON_2266_112_2_18_1_58_14_32,"MMIAPICOM_CreateRandFileAsyn PNULL == path_ptr || 0 == path_len || PNULL == file_name_ptr || PNULL == file_name_len_ptr")
TRACE_MSG(MMI_COMMON_2274_112_2_18_1_58_14_33,"MMIAPICOM_CreateRandFileAsyn PNULL == temp_wstr")
TRACE_MSG(MMI_COMMON_2281_112_2_18_1_58_14_34,"MMIAPICOM_CreateRandFileAsyn PNULL == temp_str")
TRACE_MSG(MMI_COMMON_2336_112_2_18_1_58_14_35,"MMIAPICOM_GetRandFile, success!!")
TRACE_MSG(MMI_COMMON_3036_112_2_18_1_58_15_36,"MMI_WSTRNCPY: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d")
TRACE_MSG(MMI_COMMON_3071_112_2_18_1_58_15_37,"MMI_STRTOWSTR: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d")
TRACE_MSG(MMI_COMMON_3111_112_2_18_1_58_16_38,"MMI_WSTRNTOSTR: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d")
TRACE_MSG(MMI_COMMON_3169_112_2_18_1_58_16_39,"MMIAPICOM_WstrTraceOut str is %s\n")
TRACE_MSG(MMI_COMMON_3354_112_2_18_1_58_16_40,"MMIAPICOM_CombinePath PNULL == full_path_ptr || PNULL == full_path_len_ptr")
TRACE_MSG(MMI_COMMON_3500_112_2_18_1_58_16_41,"MMICOM: key = 0x%x result= %d")
TRACE_MSG(MMI_COMMON_3594_112_2_18_1_58_16_42,"MMI_GetFreeSpace: free_space = %d K bytes")
TRACE_MSG(MMI_COMMON_3890_112_2_18_1_58_17_43,"[mmi_common.c] MMIAPICOM_BinSearch search_info 0x%x  func 0x%x !")
TRACE_MSG(MMI_COMMON_3963_112_2_18_1_58_17_44,"[mmi_common.c] MMIAPICOM_BinSearch cmp_result %d, ret_pos %d is_in_list %d mid_pos %d")
TRACE_MSG(MMI_COMMON_4018_112_2_18_1_58_17_45,"MMIAPIPTSCR_WriteBMPFile MMIAPIFMM_CreateFile fail!")
TRACE_MSG(MMI_COMMON_4053_112_2_18_1_58_17_46,"MMIAPIPTSCR_WriteBMPFile WriteBMPFile bmp is to large! w = %d, nLineBytes = %d")
TRACE_MSG(MMI_COMMON_4075_112_2_18_1_58_17_47,"MMIAPIPTSCR_WriteBMPFile WriteBMPFile alloc fail!")
TRACE_MSG(MMI_COMMON_4336_112_2_18_1_58_18_48,"MMIAPICOM_GetAudioPri entry: audio_type_str=0x%x")
TRACE_MSG(MMI_COMMON_4352_112_2_18_1_58_18_49,"MMIAPICOM_GetAudioPri exit: pri=%d")
TRACE_MSG(MMI_PUBMENUWIN_277_112_2_18_1_58_38_50,"MMIPUB_OpenMenuWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBRICHTEXTWIN_243_112_2_18_1_58_38_51,"MMIPUB_OpenRichTextWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBTEXTWIN_245_112_2_18_1_58_39_52,"MMIPUB_OpenTextWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBWIN_2101_112_2_18_1_58_43_53,"OpenAlertWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBWIN_2424_112_2_18_1_58_44_54,"MMIPUB_OpenAlertWinByTextId:win_id %d is already open!")
TRACE_MSG(MMI_PUBWIN_2450_112_2_18_1_58_44_55,"MMIPUB_HandleProgressWinMsg, win_id = %d, msg_id = %x")
TRACE_MSG(MMI_PUBWIN_2669_112_2_18_1_58_45_56,"MMIPUB_HandleAlertWinMsg, win_id = %d, msg_id = %x")
TRACE_MSG(MMI_PUBWIN_3713_112_2_18_1_58_47_57,"MMIPUB_SetWinSoftkeyEx:win_id %d is not open!")
TRACE_MSG(MMI_PUBWIN_3753_112_2_18_1_58_47_58,"MMIPUB_SetWinSoftkeyEx:win_id %d is not open!")
TRACE_MSG(MMI_PUBWIN_3794_112_2_18_1_58_47_59,"MMIPUB_SetWinSoftkeyGray:win_id %d is failed!")
TRACE_MSG(MMI_PUBWIN_4062_112_2_18_1_58_47_60,"OpenQueryWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBWIN_4117_112_2_18_1_58_47_61,"MMIPUB_HandleQueryWinMsg, win_id = %d, msg_id = %x")
TRACE_MSG(MMI_PUBWIN_4313_112_2_18_1_58_48_62,"OpenWaitWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBWIN_4435_112_2_18_1_58_48_63,"MMIPUB_HandleWaitWinMsg, win_id = %d, msg_id = %x")
TRACE_MSG(MMI_PUBWIN_4501_112_2_18_1_58_48_64,"MMIPUB_SetWaitWinText:win_id %d is not open!")
TRACE_MSG(MMI_PUBWIN_5081_112_2_18_1_58_50_65,"OpenAlertTextWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBWIN_5285_112_2_18_1_58_50_66,"OpenQueryTextWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBWIN_5659_112_2_18_1_58_51_67,"GetTipsDisRect, cur dis_style error, dis_style = %d")
TRACE_MSG(MMI_PUBWIN_6289_112_2_18_1_58_53_68,"MMIPUB_OpenEditWin param error")
TRACE_MSG(MMI_PUBWIN_6390_112_2_18_1_58_53_69,"MMIPUB_OpenWin:win_id %d is already open!")
TRACE_MSG(MMI_PUBWIN_6613_112_2_18_1_58_53_70,"MMIPUB_OpenTempWaitWin() MMK_CreateWindow fail!")
TRACE_MSG(MMI_SETLIST_WIN_113_112_2_18_1_58_55_71,"SetPopWinTitle error input param!")
TRACE_MSG(MMI_SETLIST_WIN_199_112_2_18_1_58_55_72,"HandleSetlistPopWinMsg error input param!")
TRACE_MSG(MMI_SETLIST_WIN_308_112_2_18_1_58_55_73,"SetPopWinListBox input param is null!")
TRACE_MSG(MMI_SLIDE_2773_112_2_18_1_59_2_74,"%s %s\n")
TRACE_MSG(MMI_SOFTKEYTEST_153_112_2_18_1_59_3_75,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID")
TRACE_MSG(MMI_SOFTKEYTEST_158_112_2_18_1_59_3_76,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID")
TRACE_MSG(MMI_SOFTKEYTEST_163_112_2_18_1_59_3_77,"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID")
TRACE_MSG(MMICOM_DATA_1015_112_2_18_1_59_6_78,"0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, n")
TRACE_MSG(MMICOM_DATA_1024_112_2_18_1_59_6_79,"0x%02x,")
TRACE_MSG(MMICOM_DATA_1027_112_2_18_1_59_6_80,"\n")
TRACE_MSG(MMICOM_DATA_1107_112_2_18_1_59_6_81,"invalid cValue")
TRACE_MSG(MMICOM_EXTRACT_130_112_2_18_1_59_7_82,"HandleExtractNumberPopWinMsg msg_id = 0x%x, dual_sys=%d")
TRACE_MSG(MMICOM_EXTRACT_342_112_2_18_1_59_7_83,"MMIUSSSD: HandleExtractUrlPopWinMsg msg_id = 0x%x, dual_sys=%d")
TRACE_MSG(MMICOM_EXTRACT_481_112_2_18_1_59_7_84,"HandleExtractEmailPopWinMsg msg_id = 0x%x, dual_sys=%d")
TRACE_MSG(MMICOM_PANEL_210_112_2_18_1_59_8_85,"[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin Panel is opened!")
TRACE_MSG(MMICOM_PANEL_231_112_2_18_1_59_9_86,"[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin open other panel!")
TRACE_MSG(MMICOM_PANEL_235_112_2_18_1_59_9_87,"[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin cur=%d, min_value = %d, max=%d, type=%d!")
TRACE_MSG(MMICOM_PANEL_240_112_2_18_1_59_9_88,"[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin error max_value==0")
TRACE_MSG(MMICOM_PANEL_903_112_2_18_1_59_10_89,"[MMICOMPANEL]:panel_type error. type=%d")
TRACE_MSG(MMICOM_STRING_368_112_2_18_1_59_12_90,"MMIAPICOM_Ascii2default PNULL == ascii_ptr || PNULL == default_ptr!")
TRACE_MSG(MMICOM_STRING_416_112_2_18_1_59_12_91,"MMIAPICOM_Default2Ascii PNULL == ascii_ptr || PNULL == default_ptr!")
TRACE_MSG(MMICOM_STRING_468_112_2_18_1_59_12_92,"MMIAPICOM_DefaultToWstr PNULL == ascii_ptr || PNULL == default_ptr!")
TRACE_MSG(MMICOM_STRING_553_112_2_18_1_59_12_93,"MMIAPICOM_MergeTwoString PNULL == string_1 || PNULL == string_2 || PNULL == string_3")
TRACE_MSG(MMICOM_STRING_646_112_2_18_1_59_12_94,"MMIAPICOM_StrcatFromSTRINGToUCS2 PNULL == ucs2_str_ptr || PNULL == ucs2_str_len_ptr || PNULL == string_ptr")
TRACE_MSG(MMICOM_STRING_669_112_2_18_1_59_12_95,"MMIAPICOM_StrcatFromStrToUCS2 PNULL == ucs2_str_ptr || PNULL == ucs2_str_len_ptr || PNULL == str_ptr")
TRACE_MSG(MMICOM_STRING_691_112_2_18_1_59_12_96,"MMIAPICOM_GetInt32ValueFromBuf PNULL == dataPtr")
TRACE_MSG(MMICOM_STRING_716_112_2_18_1_59_12_97,"MMIAPICOM_GetInt8ValueFromBuf PNULL == dataPtr")
TRACE_MSG(MMICOM_STRING_740_112_2_18_1_59_12_98,"MMIAPICOM_GetInt16ValueFromBuf PNULL == dataPtr")
TRACE_MSG(MMICOM_STRING_790_112_2_18_1_59_12_99,"MMIAPICOM_CompareTwoWstrExt PNULL == wstr1_ptr || PNULL == wstr2_ptr")
TRACE_MSG(MMICOM_STRING_894_112_2_18_1_59_13_100,"MMIAPICOM_CopyString PNULL == out_len_ptr || PNULL == out_alpha_ptr || PNULL == in_alpha_ptr")
TRACE_MSG(MMICOM_STRING_1035_112_2_18_1_59_13_101,"MMIAPICOM_CopyString PNULL == i1_text_ptr")
TRACE_MSG(MMICOM_STRING_1069_112_2_18_1_59_13_102,"PNULL != str_ptr, PNULL != str_info_ptr")
TRACE_MSG(MMICOM_STRING_1103_112_2_18_1_59_13_103,"PNULL != str, PNULL != num")
TRACE_MSG(MMICOM_STRING_1142_112_2_18_1_59_13_104,"MMIAPICOM_CountDefaultExt PNULL == ascii_ptr")
TRACE_MSG(MMICOM_STRING_1209_112_2_18_1_59_13_105,"PNULL != num_string.wstr_ptr")
TRACE_MSG(MMICOM_STRING_1251_112_2_18_1_59_13_106,"MMIAPICOM_ConvertAlphabetToUnicode PNULL == i1_default_text_ptr || PNULL == o1_unicode_text_ptr")
TRACE_MSG(MMICOM_STRING_1287_112_2_18_1_59_13_107,"MMIAPICOM_Int2Str PNULL == buf || buf_len <2")
TRACE_MSG(MMICOM_STRING_1327_112_2_18_1_59_14_108,"MMIAPICOM_CountUcs2Char PNULL == txt_ptr")
TRACE_MSG(MMICOM_STRING_1394_112_2_18_1_59_14_109,"MMIAPICOM_IsNumberString PNULL == cmd_char")
TRACE_MSG(MMICOM_STRING_1424_112_2_18_1_59_14_110,"MMIAPICOM_ConvertCmdChar2Num PNULL == cmd_char")
TRACE_MSG(MMICOM_STRING_1437_112_2_18_1_59_14_111,"MMIAPICOM_ConvertCmdChar2Num u8_char=%d,i=%d")
TRACE_MSG(MMICOM_STRING_1494_112_2_18_1_59_14_112,"MMIAPICOM_GBToSTRING PNULL == src_ptr || PNULL == des_str")
TRACE_MSG(MMICOM_STRING_1549_112_2_18_1_59_14_113,"MMIAPICOM_GetStrWidth PNULL == str_ptr || PNULL == str_len_ptr || font_type >= GUI_MAX_FONT_NUM")
TRACE_MSG(MMICOM_STRING_1575_112_2_18_1_59_14_114,"MMIAPICOM_GetStrWidth str_width= %d,str_space=%d,font_width=%d")
TRACE_MSG(MMICOM_STRING_1603_112_2_18_1_59_14_115,"MMIAPICOM_ParseSimStr PNULL == sim_str || PNULL == mmi_str->wstr_ptr || mmi_buf_len == 0 ")
TRACE_MSG(MMICOM_STRING_1608_112_2_18_1_59_14_116,"[MMICOM] sim_str_len = %d")
TRACE_MSG(MMICOM_STRING_1768_112_2_18_1_59_14_117,"[MMICOM] mmi_str->length = %d")
TRACE_MSG(MMICOM_STRING_1782_112_2_18_1_59_14_118,"MMIAPICOM_CopyStr2Buf PNULL == dst_buf_ptr || PNULL == src_ptr ")
TRACE_MSG(MMICOM_STRING_1815_112_2_18_1_59_14_119,"[MMICOM]:MMIAPICOM_CatTwoString fail, dst_ptr=%d,src1_ptr=%d")
TRACE_MSG(MMICOM_STRING_1821_112_2_18_1_59_14_120,"[MMICOM]:MMIAPICOM_CatTwoString fail, len1=%d,len2=%d")
TRACE_MSG(MMICOM_STRING_1838_112_2_18_1_59_15_121,"[MMICOM]:MMIAPICOM_CatTwoString fail, malloc for dst_ptr->str_ptr fail!")
TRACE_MSG(MMICOM_STRING_1863_112_2_18_1_59_15_122,"warning CatStringByTextID:dst_ptr=%d")
TRACE_MSG(MMICOM_STRING_1887_112_2_18_1_59_15_123,"warning CatStringByTextID2 dst_ptr NULL")
TRACE_MSG(MMICOM_STRING_1938_112_2_18_1_59_15_124,"MMIAPICOM_EncodeGSMAlphabet alpha_type  %d !")
TRACE_MSG(MMICOM_STRING_1967_112_2_18_1_59_15_125,"MMIAPICOM_DecodeGSMAlphabet invalid parameter !")
TRACE_MSG(MMICOM_STRING_2513_112_2_18_1_59_16_126,"MMIAPICOM_GetGSMAlphabetLen encode_type %d wchar_len %d alpha_len %d")
TRACE_MSG(MMICOM_STRING_2657_112_2_18_1_59_16_127,"MMIAPICOM_Wchar2default PNULL == wchar_ptr || PNULL == default_ptr!")
TRACE_MSG(MMICOM_STRING_2775_112_2_18_1_59_16_128,"MMIAPICOM_Default2Wchar PNULL == wchar_ptr || PNULL == default_ptr!")
TRACE_MSG(MMICOM_TIME_140_112_2_18_1_59_17_129,"CheckDayOfDateIsValid:date.date_month = %d is error!")
TRACE_MSG(MMICOM_TIME_194_112_2_18_1_59_17_130,"year = %d, mon = %d, day = %d, hour = %d, min = %d, sec = %d ")
TRACE_MSG(MMICOM_TIME_340_112_2_18_1_59_18_131,"MMI_COMMON:tm_mday = %d,tm_mon = %d,tm_year = %dn")
TRACE_MSG(MMICOM_TIME_414_112_2_18_1_59_18_132,"MMI_COMMON:error leap_year = %d\n")
TRACE_MSG(MMICOM_TIME_438_112_2_18_1_59_18_133,"MMI_COMMON:error day = %d\n")
TRACE_MSG(MMICOM_TIME_468_112_2_18_1_59_18_134,"MMI_COMMON:error month = %d\n")
TRACE_MSG(MMICOM_TIME_610_112_2_18_1_59_18_135,"GetRandomTimer return timer = %d")
END_TRACE_MAP(MMI_APP_COMMON_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_COMMON_TRC_H_

