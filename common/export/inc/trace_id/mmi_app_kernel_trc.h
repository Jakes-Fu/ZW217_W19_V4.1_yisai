/******************************************************************************
 ** File Name:      mmi_app_kernel_trc.h                                         *
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
//trace_id:145
#ifndef _MMI_APP_KERNEL_TRC_H_
#define _MMI_APP_KERNEL_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define BLOCK_MEM_456_112_2_18_1_59_22_0 "BL_CheckID:invalid id = %d"
#define BLOCK_MEM_514_112_2_18_1_59_22_1 "Block_Malloc: %d"
#define BLOCK_MEM_586_112_2_18_1_59_22_2 "Block_Malloc: alloc mem fail!"
#define BLOCK_MEM_602_112_2_18_1_59_22_3 "BL_Malloc: %d"
#define BLOCK_MEM_669_112_2_18_1_59_22_4 "BL_Malloc: alloc mem fail!"
#define BLOCK_MEM_697_112_2_18_1_59_22_5 "BL_Free: %d"
#define BLOCK_MEM_701_112_2_18_1_59_22_6 "BL_Free:unused id = %d"
#define BLOCK_MEM_831_112_2_18_1_59_23_7 "MMI_BL_Malloc: %d,%s, line=%ld"
#define BLOCK_MEM_849_112_2_18_1_59_23_8 "MMI_BL_Malloc: %d,size=%d,%s, line=%ld"
#define BLOCK_MEM_866_112_2_18_1_59_23_9 "MMI_BL_Malloc: %d,%s, line=%ld"
#define BLOCK_MEM_882_112_2_18_1_59_23_10 "MMI_BL_Free: %d,%s, line=%ld"
#define MMI_ATC_1904_112_2_18_1_59_28_11 "mmi_atc.c: ATC_ExtCmd(), received AT command , len = %d"
#define MMI_ATC_2264_112_2_18_1_59_29_12 "[ATC:][ATEST:][Atest_HandlePhoneBook:] %c"
#define MMI_ATC_2352_112_2_18_1_59_29_13 "[ATC][Atest_HandlePbSim]: SIM FULL!"
#define MMI_ATC_2539_112_2_18_1_59_29_14 "[MMIATC]:Atest_HandleURL:%s"
#define MMI_ATC_2550_112_2_18_1_59_29_15 "[MMIATC]:Atest_HandleURL2:%s, %d"
#define MMI_ATC_2559_112_2_18_1_59_29_16 "[ATC:][ATEST:][ST:]Atest_HandleURL:url_input:%x, %x, %d"
#define MMI_ATC_2607_112_2_18_1_59_29_17 "[ATC:][ATEST:][FT:]Atest_HandleConnectGatewayDNS:%x, %x"
#define MMI_ATC_2669_112_2_18_1_59_29_18 "[ATC:][ATEST:][FT:]Atest_HandleConnectOther:conne_input:%x, %x"
#define MMI_ATC_2733_112_2_18_1_59_29_19 "[ATC:][ATEST:][Atest_HandleConnection:] %c"
#define MMI_ATC_2777_112_2_18_1_59_30_20 "[ATC:][FT:][Atest_HandleFtAtMMI:]connection input type error!"
#define MMI_ATC_2817_112_2_18_1_59_30_21 "[ATC:][ATEST:][FT:]Atest_HandleBmTitle:conne_input:%x, %x, %d"
#define MMI_ATC_2876_112_2_18_1_59_30_22 "[ATC:][ATEST:][FT:]Atest_HandleBmURL:conne_input:%x, %x, %d"
#define MMI_ATC_2954_112_2_18_1_59_30_23 "[ATC:][ATEST:][Atest_HandleBookmark:] %c"
#define MMI_ATC_2993_112_2_18_1_59_30_24 "[ATC:][ATEST:][ST:]Atest_HandleSMS:sms_input:%x, %x"
#define MMI_ATC_3155_112_2_18_1_59_30_25 "[ATC:][ATEST:][Atest_HandleFolder:] %c"
#define MMI_ATC_3209_112_2_18_1_59_30_26 "[ATC:][FT:][Atest_HandleStAtMMI:]connection input type error!"
#define MMI_ATC_3249_112_2_18_1_59_31_27 "[ATC:][MMI:][Atest_HandleAtMMI]input test type error!"
#define MMI_ATC_3380_112_2_18_1_59_31_28 "[AT][ATEST]:ATest_DispatchExtSig--signalType=0x%x,handleresult=%d"
#define MMI_ATC_4298_112_2_18_1_59_33_29 "[ATC:][ATEST:]Atest_GetControlTree"
#define MMI_ATC_4391_112_2_18_1_59_33_30 "[AT][ATEST]:Atest_HandleControlInput--signalType=0x%x"
#define MMI_ATC_6912_112_2_18_1_59_39_31 "[ATC:][ATEST:]Atest_ClickControl--%d"
#define MMI_ATC_6918_112_2_18_1_59_39_32 "[ATC:][ATEST:]find Control position succeed--%d--%d"
#define MMI_ATC_6934_112_2_18_1_59_39_33 "[ATC:][ATEST:] ConvertPosition failed"
#define MMI_ATC_6940_112_2_18_1_59_39_34 "[ATC:][ATEST:]find Control failed"
#define MMI_ATC_7033_112_2_18_1_59_39_35 "[ATC:][ATEST:]Search Window--%d"
#define MMI_ATC_7125_112_2_18_1_59_39_36 "[ATC:][ATEST:]Search Control,ctrl_guid=%d"
#define MMI_ATC_7416_112_2_18_1_59_40_37 "[ATC:][ATEST:]Atest_KbdConvert,keycode--%d"
#define MMI_ATC_7479_112_2_18_1_59_40_38 "[ATC:][ATEST:]Atest_SendKbdMessage,signal_keycode=%d"
#define MMI_ATC_7505_112_2_18_1_59_40_39 "[ATC:][ATEST:]Atest_SendTouchMessage,tp_type--%d"
#define MMI_ATC_7559_112_2_18_1_59_40_40 "[ATC:][ATEST:]Atest_GetCurLang-%d-%s"
#define MMI_ATC_7677_112_2_18_1_59_40_41 "[AT][ATEST]:Atest_TouchPanelDown,-%d,%d"
#define MMI_ATC_7697_112_2_18_1_59_40_42 "[AT][ATEST]:Atest_KBDown,-%d"
#define MMI_ATC_7719_112_2_18_1_59_40_43 "[AT][ATEST]:Atest_TouchPanelMove,-%d,%d"
#define MMI_ATC_7741_112_2_18_1_59_40_44 "[AT][ATEST]:Atest_TouchPanelUp,-%d,%d"
#define MMI_ATC_7769_112_2_18_1_59_40_45 "[ATC:][ATEST:] Atest_ConvertPositionByLcd,%d,%d*%d"
#define MMI_ATC_7794_112_2_18_1_59_40_46 "[ATC:][ATEST:]ConvertPosition-%d-%d"
#define MMI_ATC_7810_112_2_18_1_59_40_47 "[ATC:][ATEST:] invalid point"
#define MMI_ATC_7853_112_2_18_1_59_40_48 "[ATC:][ATEST:]Atest_Compare_content:AT string Num=%d"
#define MMI_ATC_7879_112_2_18_1_59_40_49 "[ATC:][ATEST:]Atest_Compare_content:Atest_wcslen:ctrl=%d--at=%d"
#define MMI_ATC_7885_112_2_18_1_59_40_50 "[ATC:][ATEST:]Atest_Compare_content:remove the last word if is space of ctrl_str"
#define MMI_ATC_7897_112_2_18_1_59_40_51 "[ATC:][ATEST:]Atest_Compare_content:remove the last word if is space of at_str"
#define MMI_ATC_7905_112_2_18_1_59_40_52 "[ATC:][ATEST:]Atest_Compare_content:Atest_wcslen:ctrl=%d--at=%d"
#define MMI_ATC_7909_112_2_18_1_59_40_53 "[ATC:][ATEST:]Atest_Compare_content:compare text TRUE"
#define MMI_ATC_7933_112_2_18_1_59_41_54 "[ATC:][ATEST:]Atest_Compare_content:compare id TRUE"
#define MMI_ATC_7941_112_2_18_1_59_41_55 "[ATC:][ATEST:]Atest_Compare_content:FALSE"
#define MMI_ATC_7982_112_2_18_1_59_41_56 "[ATC:][ATEST:]Atest_GetValidTPPoint item_rect:%d %d %d %d--ctrl_rect:%d %d %d %d"
#define MMI_ATC_8011_112_2_18_1_59_41_57 "[ATC:][ATEST:]Atest_GetValidTPPoint failed"
#define MMI_ATC_8029_112_2_18_1_59_41_58 "[ATC:][ATEST:]Atest_GetValidTPPoint:%d %d"
#define MMI_ATC_8421_112_2_18_1_59_42_59 "[ATC:][ATEST:]Atest_SearchQbMenuControlItem,cur_page=%d,item_num=%d,totalpage='%d'"
#define MMI_ATC_8431_112_2_18_1_59_42_60 "[ATC:][ATEST:]Atest_SearchQbMenuControlItem:page=%d/%d"
#define MMI_ATC_8435_112_2_18_1_59_42_61 "[ATC:][ATEST:]Atest_SearchQbMenuControlItem:move_page_num=%d stop"
#define MMI_ATC_8466_112_2_18_1_59_42_62 "[ATC:][ATEST:]Atest_SearchQbMenuControlItem:GUIMENU_MAINMENU_DATA_TYPE_DYNAMIC!"
#define MMI_ATC_8472_112_2_18_1_59_42_63 "[ATC:][ATEST:]Atest_SearchQbMenuControlItem:GUIMENU_MAINMENU_DATA_TYPE_FILEFOLD!"
#define MMI_ATC_8479_112_2_18_1_59_42_64 "[ATC:][ATEST:]Atest_SearchQbMenuControlItem,MMITHEME_GetResText,icon_id=%d,text_id=%d,text=%x"
#define MMI_ATC_8810_112_2_18_1_59_43_65 "[ATC:][ATEST:]Atest_SearchSlideMenuControlItem:page:%d/%d"
#define MMI_ATC_8957_112_2_18_1_59_43_66 "[ATC:][ATEST:]Atest_SearchPopMenuControlItem,down:cur_item_index=%d,index_offset=%d"
#define MMI_ATC_8969_112_2_18_1_59_43_67 "[ATC:][ATEST:]Atest_SearchPopMenuControlItem,up:cur_item_index=%d,index_offset=%d"
#define MMI_ATC_9055_112_2_18_1_59_43_68 "[ATC:][ATEST:]Atest_SearchIconMenuControlItem:page_col=%d,page_row=%d,page_item=%d"
#define MMI_ATC_9109_112_2_18_1_59_44_69 "[ATC:][ATEST:]Atest_SearchIconMenuControlItem:cur_item_index=%d,index_row=%d"
#define MMI_ATC_9134_112_2_18_1_59_44_70 "[ATC:][ATEST:]Atest_SearchIconMenuControlItem:cur_item_index=%d,index_row=%d"
#define MMI_ATC_9210_112_2_18_1_59_44_71 "[ATC:][ATEST:]Atest_SearchOptionMenuControlItem:Find index=%d"
#define MMI_ATC_9252_112_2_18_1_59_44_72 "[Atest_SearchListByIndex]-searchitemindex=%d"
#define MMI_ATC_9334_112_2_18_1_59_44_73 "[Atest_SearchListByImgIndex]-searchitemindex=%d"
#define MMI_ATC_9455_112_2_18_1_59_44_74 "[Atest_SearchListByTextImg]--%x--%x--%x--%x--%x,"
#define MMI_ATC_9460_112_2_18_1_59_45_75 "[Atest_SearchListByTextImg]-searchitemindex=%d"
#define MMI_ATC_9486_112_2_18_1_59_45_76 "[ATC:][ATEST:]Atest_SearchListByTextImg:Save buffer data! len=%d\n"
#define MMI_ATC_9510_112_2_18_1_59_45_77 "[ATC:][ATEST:]Atest_SearchListByTextImg:item_index-%d,content_index=%d"
#define MMI_ATC_9641_112_2_18_1_59_45_78 "[Atest_SearchListByTextAndImgID]--%x--%x--%x--%x--%x,"
#define MMI_ATC_9647_112_2_18_1_59_45_79 "[Atest_SearchListByTextAndImgID]-searchitemindex=%d"
#define MMI_ATC_9676_112_2_18_1_59_45_80 "[ATC:][ATEST:]Atest_SearchListByTextAndImgID:Save buffer data! len=%d\n"
#define MMI_ATC_9724_112_2_18_1_59_45_81 "[ATC:][ATEST:]Atest_SearchListByTextAndImgID:item_index-%d,content_index=%d,content_type=%d"
#define MMI_ATC_9827_112_2_18_1_59_45_82 "[Atest_SearchListControlItem]totalnum=%d,itemheight=%d"
#define MMI_ATC_10517_112_2_18_1_59_47_83 "[ATC][ATEST]:Iconlist:index_ptr=%d,row=%d,col=%d,item=%d,top=%d,bottom=%d,cur=%d"
#define MMI_ATC_11114_112_2_18_1_59_49_84 "mmi_atc.c: ParseSyncAtCmd(), mmi atc is not run"
#define MMI_ATC_11131_112_2_18_1_59_49_85 "mmi_atc.c: ParseSyncAtCmd(), module_type = %c, operation_type = %c"
#define MMI_ATC_11299_112_2_18_1_59_49_86 "ParseSyncUTAtCmd, cmd[0] 0x%x"
#define MMI_ATC_11306_112_2_18_1_59_49_87 "ParseSyncUTAtCmd, get module_type error"
#define MMI_ATC_11313_112_2_18_1_59_49_88 "ParseSyncUTAtCmd, module_type strlen %d, cmd_len %d"
#define MMI_ATC_11321_112_2_18_1_59_49_89 "ParseSyncUTAtCmd, Get operation_type error"
#define MMI_ATC_11328_112_2_18_1_59_49_90 "ParseSyncUTAtCmd, operation_type strlen %d, cmd_len %d"
#define MMI_ATC_11333_112_2_18_1_59_49_91 "mmi_atc.c: ParseSyncAtCmd(), module_type = %c, operation_type = %c"
#define MMI_ATC_11395_112_2_18_1_59_49_92 "[baokun] MMI_AUTO_TEST_ACO_OFF"
#define MMI_ATC_11402_112_2_18_1_59_49_93 "[baokun] MMI_AUTO_TEST_ACO_ON"
#define MMI_ATC_11409_112_2_18_1_59_49_94 "[baokun] MMI_AUTO_TEST_HEADSET_MODE_OFF"
#define MMI_ATC_11416_112_2_18_1_59_49_95 "[baokun] MMI_AUTO_TEST_HEADSET_MODE_ON"
#define MMI_ATC_11433_112_2_18_1_59_49_96 "[baokun] MMI_AUTO_TEST_OFF is_on %d"
#define MMI_ATC_11439_112_2_18_1_59_49_97 "[baokun] MMI_AUTO_TEST_ON is_on %d"
#define MMI_ATC_11472_112_2_18_1_59_49_98 " AutoTest Lcd Brightness  MMI_AUTO_TEST_OFF is_on %d"
#define MMI_ATC_11570_112_2_18_1_59_50_99 "mmi_atc.c: HandlePbGetInfo(), pos = %d"
#define MMI_ATC_11577_112_2_18_1_59_50_100 "mmi_atc.c: HandlePbGetInfo(), max_record_num = %d, used_record_num = %d"
#define MMI_ATC_11632_112_2_18_1_59_50_101 "mmi_atc.c: HandlePbReadItem(), pos = %d, entry_id = %d"
#define MMI_ATC_11639_112_2_18_1_59_50_102 "mmi_atc.c: HandlePbReadItem(), entry_id = %d"
#define MMI_ATC_11699_112_2_18_1_59_50_103 "mmi_atc.c: HandlePbAdd(), pos = %d, entry_id = %d"
#define MMI_ATC_11729_112_2_18_1_59_50_104 "mmi_atc.c: MMISYNC_PbDeleteCallback, is_success = %d, s_ispbop_wait = %d"
#define MMI_ATC_11767_112_2_18_1_59_50_105 "mmi_atc.c: HandlePbDelete(), pos = %d, entry_id = %d"
#define MMI_ATC_11815_112_2_18_1_59_50_106 "mmi_atc.c: HandlePbModify(), pos = %d, entry_id = %d"
#define MMI_ATC_11845_112_2_18_1_59_50_107 "mmi_atc.c: MMISYNC_PbUpdateCallback, is_success = %d, s_ispbop_wait = %d"
#define MMI_ATC_11882_112_2_18_1_59_50_108 "mmi_atc.c:HandleSmsDelete(), length error, len = %d"
#define MMI_ATC_11893_112_2_18_1_59_50_109 "mmi_atc.c:HandleSmsDelete(), record_id_str = %s"
#define MMI_ATC_11906_112_2_18_1_59_50_110 "mmi_atc.c:HandleSmsDelete(), SMS status error,  %s"
#define MMI_ATC_11924_112_2_18_1_59_50_111 "mmi_atc.c:HandleSmsDelete(), SMS storage error,  %d"
#define MMI_ATC_11931_112_2_18_1_59_50_112 "mmi_atc.c: HandleSmsDelete(), record_id = %d, is_mt_sms = %d, pos = %d"
#define MMI_ATC_12442_112_2_18_1_59_51_113 "[MMIATC] GetPbStorage pos 0x%x"
#define MMI_ATC_12560_112_2_18_1_59_52_114 "[MMIATC] DecompressCmdStr out_len %d"
#define MMI_ATC_12574_112_2_18_1_59_52_115 "[MMIATC] DecompressCmdStr out_len %d"
#define MMI_ATC_12625_112_2_18_1_59_52_116 "[MMIATC] HandleSmsSendMsg wstr_msg_ptr %x  %x"
#define MMI_ATC_12686_112_2_18_1_59_52_117 "[MMIATC] HandleSmsSendMsg wstr_msg_ptr %x  %x"
#define MMI_ATC_12749_112_2_18_1_59_52_118 "[MMIATC] MMIAPIMMS_InsertMessage wstr_msg_ptr %x  %x"
#define MMI_ATC_12812_112_2_18_1_59_52_119 "[MMIATC] MMIAPIWWW_InsertURL wstr_msg_ptr %x  %x"
#define MMI_ATC_12873_112_2_18_1_59_52_120 "[MMIATC] MIAPICONNECTION_InsertStr wstr_msg_ptr %x  %x"
#define MMI_ATC_12907_112_2_18_1_59_52_121 "[MMIATC] GetATCParameter cmd == PNULL !"
#define MMI_ATC_12912_112_2_18_1_59_52_122 "[MMIATC] GetATCParameter cmd %x  %x  %x  %x  %x  %x  %x  %x  %x"
#define MMI_ATC_12977_112_2_18_1_59_52_123 "[MMIATC] GetATCNextString parameter error !"
#define MMI_ATC_12995_112_2_18_1_59_52_124 "[MMIATC] GetATCNextString string %s result %d"
#define MMI_AUTOTEST_306_112_2_18_1_59_53_125 "---autotest---%s----%d----"
#define MMI_AUTOTEST_353_112_2_18_1_59_54_126 "---autotest---%s----%d----"
#define MMI_AUTOTEST_387_112_2_18_1_59_54_127 "---autotest---%s----%d----"
#define MMI_AUTOTEST_416_112_2_18_1_59_54_128 "---autotest---%s----%d----"
#define MMI_AUTOTEST_642_112_2_18_1_59_54_129 "[AUTOTEST] MMI_AutoTest_Trace invalid parameter !"
#define MMI_AUTOTEST_649_112_2_18_1_59_54_130 "[AUTOTEST] MMI_AutoTest_Trace malloc fail parameter !"
#define MMI_AUTOTEST_707_112_2_18_1_59_54_131 "[AUTOTEST] MMI_AutoTest_Trace invalid parameter !"
#define MMI_AUTOTEST_714_112_2_18_1_59_54_132 "[AUTOTEST] MMI_AutoTest_Trace malloc fail parameter !"
#define MMI_DEFAULT_1184_112_2_18_1_59_57_133 "[MMK_DefaultMSGKbd]Hook key is pressed."
#define MMI_DEFAULT_1190_112_2_18_1_59_57_134 "[MMK_DefaultMSGKbd]Now mode is handfree."
#define MMI_DEFAULT_1196_112_2_18_1_59_57_135 "[MMK_DefaultMSGKbd]Mode is not handfree, so stop all ring."
#define MMI_DEFAULT_1207_112_2_18_1_59_57_136 "[MMK_DefaultMSGKbd]Hook key is released."
#define MMI_DEFAULT_1257_112_2_18_1_59_57_137 "[MMK_DefaultMSGKbd]HandFree key is pressed"
#define MMI_DEFAULT_1436_112_2_18_1_59_58_138 "[MMK_DefaultMSGKbd](KEY_PRESSED)MMIAUDIO_SetVolume,AUDIO_DEVICE_MODE_EARPHONE, %d"
#define MMI_DEFAULT_1493_112_2_18_1_59_58_139 "[MMK_DefaultMSGKbd](KEY_RELEASED)MMIAUDIO_SetVolume, AUDIO_DEVICE_MODE_HANDHOLD ,%d"
#define MMI_DEFAULT_2535_112_2_18_2_0_0_140 "MMK_HandleHeadSetKeyProcess: warning! There is no call window exists when in call state!"
#define MMI_DEFAULT_2681_112_2_18_2_0_0_141 "VirWin_HandleMsg: received headset timer"
#define MMI_DEFAULT_2796_112_2_18_2_0_1_142 "MMK_DefaultMSGKbd: can not power off because be forbidden by engineer mode"
#define MMI_DEFAULT_2842_112_2_18_2_0_1_143 "MMK_HandlePublicKey: MSG_KEYDOWN_FLIP!"
#define MMI_DEFAULT_2848_112_2_18_2_0_1_144 "MMK_HandlePublicKey: MSG_KEYUP_FLIP!"
#define MMI_DEFAULT_2854_112_2_18_2_0_1_145 "MMK_HandlePublicKey: MSG_KEYDOWN_HEADSET_BUTTON!"
#define MMI_DEFAULT_2859_112_2_18_2_0_1_146 "MMK_HandlePublicKey, msg_id = MSG_KEYDOWN_HEADSET_DETECT"
#define MMI_DEFAULT_2877_112_2_18_2_0_1_147 "MMK_HandlePublicKey, msg_id = MSG_KEYUP_HEADSET_DETECT"
#define MMI_DEFAULT_2895_112_2_18_2_0_1_148 "MMK_HandlePublicKey:MSG_KEYDOWN_DOWNSIDE stop ring"
#define MMI_DEFAULT_2900_112_2_18_2_0_1_149 "MMK_HandlePublicKey: can not power off because be forbidden by engineer mode"
#define MMI_DEFAULT_2918_112_2_18_2_0_1_150 "---WD:MMK_HandlePublicKey(),hook key is pressed"
#define MMI_DEFAULT_2928_112_2_18_2_0_1_151 "TRUE == CC_IsInCall ,CC_RELEASE_ALL_NORMAL"
#define MMI_DEFAULT_2933_112_2_18_2_0_1_152 "---WD:hook key is pressed,return to idle windows"
#define MMI_DEFAULT_2940_112_2_18_2_0_1_153 "---WD:MMK_HandlePublicKey(),HANDSFREE key is pressed"
#define MMI_DEFAULT_2944_112_2_18_2_0_1_154 "---WD:handfree key is pressed,Release all call"
#define MMI_DEFAULT_2950_112_2_18_2_0_1_155 "---WD:MMK_HandlePublicKey(),open dial window"
#define MMI_DEFAULT_3034_112_2_18_2_0_1_156 "MMK_HandlePublicKey: can't handle msg! the msg_id = %d"
#define MMI_DEFAULT_3120_112_2_18_2_0_1_157 "MMIDEFAULT_TurnOnLCDBackLight: s_backlight_ctrol = %d, s_backlight_status = %d"
#define MMI_DEFAULT_3345_112_2_18_2_0_2_158 "MMIDEFAULT_AllowBackLight: s_backlight_ctrol=%d, allow_light = %d!"
#define MMI_DEFAULT_3349_112_2_18_2_0_2_159 "MMIDEFAULT_AllowBackLight vt calling "
#define MMI_DEFAULT_3379_112_2_18_2_0_2_160 "MMIDEFAULT_AllowHalfOnBackLight:s_backlight_ctrol=%d, b_half_on = %d"
#define MMI_DEFAULT_3412_112_2_18_2_0_2_161 "MMIDEFAULT_AllowHalfOnBackLight:s_backlight_ctrol=%d, b_half_on = %d"
#define MMI_DEFAULT_3510_112_2_18_2_0_2_162 "MMIDEFAULT_StartLcdBackLightTimer backlight_type is %d,s_backlight_ctrol is %d,s_backlight_status is %d"
#define MMI_DEFAULT_3613_112_2_18_2_0_2_163 "[default]: MMIDEFAULT_SetKeybadBackLight(%d) "
#define MMI_DEFAULT_3671_112_2_18_2_0_2_164 "MMIDEFAULT_SetLcdSleep: Focus win!"
#define MMI_DEFAULT_3813_112_2_18_2_0_3_165 "MMIDEFAULT_HandleSlideKey: msg_id = %d"
#define MMI_DEFAULT_3853_112_2_18_2_0_3_166 "MMIDEFAULT_HandleSlideKey invalid msg = %d"
#define MMI_DEFAULT_3868_112_2_18_2_0_3_167 "MMIDEFAULT_HandleFlipKey: is_flipup = %d"
#define MMI_DEFAULT_3918_112_2_18_2_0_3_168 "mmi_default: MMIDEFAULT_HandleBackLightOver()"
#define MMI_DEFAULT_4084_112_2_18_2_0_3_169 "MMIDEFAULT_TurnOffBackLight, focus_win_id = %d"
#define MMI_DEFAULT_4150_112_2_18_2_0_3_170 "MMK: backlight is off, abandon tp down msg!"
#define MMI_DEFAULT_4176_112_2_18_2_0_3_171 "MMK: backlight is off, abandon tp up msg; but turn on th backlight!"
#define MMI_DEFAULT_4200_112_2_18_2_0_3_172 "MMK: backlight is off, abandon tp move msg!"
#define MMI_DEFAULT_4206_112_2_18_2_0_3_173 "MMK_DefaultMSGKtp msg_id = %d"
#define MMI_DEFAULT_4377_112_2_18_2_0_4_174 "MMK_HandleMmiSig: receive 0x%x"
#define MMI_DEFAULT_4382_112_2_18_2_0_4_175 "MMK_HandleMmiSig: receive APP_COPY_END_IND"
#define MMI_DEFAULT_4451_112_2_18_2_0_4_176 "ATEST_TRACE====Enter ATEST MODULE!"
#define MMI_DEFAULT_4458_112_2_18_2_0_4_177 "MMK_HandleMmiSig: receive unknown signal! sig_code = %d"
#define MMI_DEFAULT_4894_112_2_18_2_0_5_178 "MMK_HandleWinMoveMsg msg = 0x%x"
#define MMI_DEFAULT_4915_112_2_18_2_0_5_179 "MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: idlewin disable!"
#define MMI_DEFAULT_4919_112_2_18_2_0_5_180 "MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: childwin disable!"
#define MMI_DEFAULT_4941_112_2_18_2_0_5_181 "MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: roll close!"
#define MMI_DEFAULT_4948_112_2_18_2_0_5_182 "MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: IsDisable3DWindow!"
#define MMI_DEFAULT_4954_112_2_18_2_0_5_183 "MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: not Permit3D!"
#define MMI_DEFAULT_5001_112_2_18_2_0_5_184 "MMK_HandleWinMoveMsg MSG_CLOSE_WINDOW: roll close!"
#define MMI_DEFAULT_5007_112_2_18_2_0_5_185 "MMK_HandleWinMoveMsg MSG_CLOSE_WINDOW: IsDisable3DWindow!"
#define MMI_DEFAULT_5013_112_2_18_2_0_5_186 "MMK_HandleWinMoveMsg MSG_CLOSE_WINDOW: not focus win!"
#define MMI_DEFAULT_5018_112_2_18_2_0_5_187 "MMK_HandleWinMoveMsg MSG_CLOSE_WINDOW: not Permit3D!"
#define MMI_DEFAULT_5075_112_2_18_2_0_5_188 "MMK_HandleWinMoveMsg MSG_FULL_PAINT: delay to parent win!"
#define MMI_DEFAULT_5084_112_2_18_2_0_5_189 "MMK_HandleWinMoveMsg MSG_FULL_PAINT: no need to roll!"
#define MMI_DEFAULT_5093_112_2_18_2_0_5_190 "MMK_HandleWinMoveMsg MSG_FULL_PAINT: no need to roll!"
#define MMI_DEFAULT_5102_112_2_18_2_0_5_191 "MMK_HandleWinMoveMsg MSG_FULL_PAINT: no need to roll!"
#define MMI_DEFAULT_5133_112_2_18_2_0_5_192 "MMK_HandleWinMoveMsg MSG_FULL_PAINT: no need to roll!"
#define MMI_DEFAULT_5141_112_2_18_2_0_5_193 "MMK_HandleWinMoveMsg MSG_FULL_PAINT: Is Child Win Switch!"
#define MMI_DEFAULT_5151_112_2_18_2_0_5_194 "MMK_HandleWinMoveMsg MSG_FULL_PAINT: not Permit3D!"
#define MMI_DEFAULT_5259_112_2_18_2_0_6_195 "IsPermitDesktop3D is_3dblockmem_usedbyother = %d"
#define MMI_DEFAULT_5294_112_2_18_2_0_6_196 "MMI_IsPermit3D is_3dmmi_enable = %d, is_play_mp3bg = %d, is_play_kurobg = %d"
#define MMI_DEFAULT_5397_112_2_18_2_0_6_197 "SaveOldMoveBuf AllocTrans3DBuf fail"
#define MMI_DEFAULT_5832_112_2_18_2_0_7_198 "MMIDEFAULT_ConvertSlideKey input param error!"
#define MMI_DEFAULT_5914_112_2_18_2_0_7_199 "MMIDEFAULT_SetToneStates is_tone_on=%d"
#define MMI_DEFAULT_5929_112_2_18_2_0_7_200 "MMIDEFAULT_SetHandFreeStatesAndLed: is_handfree=%d"
#define MMI_DEFAULT_5966_112_2_18_2_0_7_201 "MMIDEFAULT_SetHookStates: is_hook=%d"
#define MMI_DEFAULT_5979_112_2_18_2_0_7_202 "MMIDEFAULT_SetHandFreeStates: is_handfree=%d, %s, %d"
#define MMI_DEFAULT_5988_112_2_18_2_0_7_203 "MMIDEFAULT_GetHandFreeStates: _is_handfree=%d"
#define MMI_DEFAULT_5998_112_2_18_2_0_7_204 "MMIDEFAULT_GetHookStates: is_hook=%d"
#define MMI_DEFAULT_6012_112_2_18_2_0_7_205 "MMIDEFAULT_GetBackLightState: %d"
#define MMI_DEFAULT_6243_112_2_18_2_0_8_206 "DrawTurnOnBackLightAnim UILAYER_GetBltInfoForPrintScreen ERROR!"
#define MMI_DEFAULT_6259_112_2_18_2_0_8_207 "DrawTurnOnBackLightAnim CreateLayer FAILED result = %d"
#define MMI_DEFAULT_6286_112_2_18_2_0_8_208 "DrawTurnOnBackLightAnim 0 == handle"
#define MMI_DEFAULT_6293_112_2_18_2_0_8_209 "DrawTurnOnBackLightAnim PNULL == render_in.src_chn.chn0.ptr"
#define MMI_DEFAULT_6308_112_2_18_2_0_8_210 "DrawTurnOnBackLightAnim PNULL == render_in.dst_chn.chn0.ptr"
#define MMI_EVENT_API_193_112_2_18_2_0_8_211 "updatetime 1.\n"
#define MMI_EVENT_API_198_112_2_18_2_0_8_212 "updatetime 2.\n"
#define MMI_EVENT_API_203_112_2_18_2_0_8_213 "updatetime 3.\n"
#define MMI_EVENT_API_208_112_2_18_2_0_8_214 "udiskmount 1.\n"
#define MMI_EVENT_API_213_112_2_18_2_0_8_215 "udiskmount 2.\n"
#define MMI_MODU_MAIN_101_112_2_18_2_0_10_216 "mmi_modu_main.c enter MMI_InitilizeWinIdNameArr()"
#define MMI_MODU_MAIN_136_112_2_18_2_0_10_217 "module_id = %d, id_offset = %d"
#define MMI_MODU_MAIN_220_112_2_18_2_0_10_218 "[RES]: image handle is not belong window, type = 0x%x, label = 0x%x, win_id = 0x%x"
#define MMI_MODU_MAIN_249_112_2_18_2_0_10_219 "[RES]: anim handle is not belong window, type = 0x%x, label = 0x%x, win_id = 0x%x"
#define MMI_MODU_MAIN_595_112_2_18_2_0_11_220 "mmi_modu_main.c enter MMI_CheckAutoTestWinId()"
#define MMI_MODU_MAIN_601_112_2_18_2_0_11_221 "MMI_CheckAutoTestWinId module%d win_id_name is NULL!"
#define MMI_MODU_MAIN_648_112_2_18_2_0_11_222 "MMI_MODU_MAIN:TV FLASH MID[0x%x] DID[0x%x] EID[0x%x]"
#define MMI_MODU_MAIN_671_112_2_18_2_0_11_223 "MMI_MODU_MAIN:TV FLASH MID[0x%x] DID[0x%x] EID[0x%x]"
#define MMI_MODU_MAIN_774_112_2_18_2_0_11_224 "[MMINV]:MMI_MODULE_SMS total size = %d"
#define MMI_MODU_MAIN_780_112_2_18_2_0_11_225 "[MMINV]:MMI_MODULE_PB total size = %d"
#define MMI_MODU_MAIN_787_112_2_18_2_0_11_226 "[MMINV]:MMI_MODULE_KURO total size = %d"
#define MMI_MODU_MAIN_793_112_2_18_2_0_11_227 "[MMINV]:MMI_MODULE_FM total size = %d"
#define MMI_MODU_MAIN_798_112_2_18_2_0_11_228 "[MMINV]:%d "
#define MMI_MODU_MAIN_803_112_2_18_2_0_11_229 "[MMINV]:NV_ID=%d size=%d"
#define MMI_MODU_MAIN_806_112_2_18_2_0_11_230 "[MMINV]:%d "
#define MMI_MODU_MAIN_811_112_2_18_2_0_11_231 "[MMINV]: total module nv size = %d, all nv item num = %d"
#define MMI_OSBRIDGE_50_112_2_18_2_0_11_232 "MMI : alloc signal error, signal szie = %d"
#define MMI_RESOURCE_1711_112_2_18_2_0_16_233 "[RES]: s_handle_map[%d].handle_id = 0x%x = %d"
#define MMI_RESOURCE_1724_112_2_18_2_0_16_234 "[RES]: com_id = %d, res_id = 0x%x, size = %d, flag1 = 0x%x, flag2 = 0x%x, type = %d"
#define MMI_RESOURCE_2022_112_2_18_2_0_17_235 "CAFResMemDestroy: warring! the mmk manager memory has be destroy"
#define MMI_RESOURCE_2180_112_2_18_2_0_17_236 "ResMalloc fail,will malloc 1, ret_ptr = %d"
#define MMI_RESOURCE_2190_112_2_18_2_0_17_237 "ResMalloc fail,will malloc 2, ret_ptr = %d"
#define MMI_RESOURCE_2202_112_2_18_2_0_17_238 "ResMalloc fail,will malloc from heap, cur size = %d"
#define MMI_RESOURCE_2307_112_2_18_2_0_17_239 "[RES]: FreeDynResNodeByHandle: handle_id = %d"
#define MMI_RESOURCE_3674_112_2_18_2_0_20_240 "MMI_GetLabelAnim com_id = %d, anim_id = %d"
#define MMI_RESOURCE_3906_112_2_18_2_0_21_241 "MMI_GetLabelImage com_id = %d, image_id = %d"
#define MMI_RESOURCE_4226_112_2_18_2_0_21_242 "[MMIRES] Text cache full. offset = %d, wstr_len=%d"
#define MMI_RESOURCE_4623_112_2_18_2_0_22_243 "[RES]: MMIRES_LoadComResource comd_id = %d start"
#define MMI_RESOURCE_4658_112_2_18_2_0_22_244 "[RES]: MMIRES_LoadComResource end"
#define MMI_RESOURCE_4728_112_2_18_2_0_22_245 "[RES]: FreeDynResNodeByComID: com_id = %d, GetDLinkHeadPtr = 0x%x"
#define MMI_RESOURCE_4806_112_2_18_2_0_23_246 "[RES]: MMIRES_EmptyComRes: com_id = %d, store_type = 0x%x"
#define MMI_RESOURCE_5410_112_2_18_2_0_24_247 "MMIRES_LoadBookmarkFromRes is_res_data_exist=%d, bookmark_num=%d"
#define MMI_RGLEXPR_729_112_2_18_2_0_26_248 "---RunFSM break--"
#define MMI_RGLEXPR_843_112_2_18_2_0_26_249 "---ConvertCH---%x---"
#define MMI_RGLEXPR_846_112_2_18_2_0_26_250 "---RunFSM UCS2 break--"
#define MMI_RGLEXPR_854_112_2_18_2_0_26_251 "---IsEndState success--"
#define MMI_RGLEXPR_860_112_2_18_2_0_26_252 "---IsEndState fail--"
#define MMI_RGLEXPR_885_112_2_18_2_0_26_253 "---FindPatternUCS2 %d--"
#define MMI_RGLEXPR_916_112_2_18_2_0_26_254 "---FindMorePatternUCS2 %d--"
#define MMI_RGLEXPR_920_112_2_18_2_0_26_255 "---FindMorePatternUCS2 break--"
#define MMIMAIN_430_112_2_18_2_0_27_256 "\"\"  MMI_SetFreq   \"#"
#define MMIMAIN_447_112_2_18_2_0_27_257 "\"\"  MMI_RestoreFreq   \"#"
#define MMIMAIN_501_112_2_18_2_0_28_258 "Jesse Yu change call key to camera key"
#define MMIMAIN_506_112_2_18_2_0_28_259 "Jesse Yu change up key to SIDE_UP key"
#define MMIMAIN_511_112_2_18_2_0_28_260 "Jesse Yu change down key to SIDE_down key"
#define MMIMAIN_524_112_2_18_2_0_28_261 "Jesse Yu flip up"
#define MMIMAIN_530_112_2_18_2_0_28_262 "Jesse Yu flip down"
#define MMIMAIN_535_112_2_18_2_0_28_263 "Jesse Yu star key down"
#define MMIMAIN_565_112_2_18_2_0_28_264 "KeypadCallBackFunc: the key message is too much!"
#define MMIMAIN_666_112_2_18_2_0_28_265 "TPCallBackFunc: the TP message is too much!"
#define MMIMAIN_752_112_2_18_2_0_28_266 "TPCallBackFunc: the TP message is too much!"
#define MMIMAIN_791_112_2_18_2_0_28_267 "TPCallBackFunc: the TP message is invalid!"
#define MMIMAIN_821_112_2_18_2_0_28_268 "GsensorCallBackFunc: gsensor_code = %d"
#define MMIMAIN_825_112_2_18_2_0_28_269 "GsensorCallBackFunc: the gsensor message is too much!"
#define MMIMAIN_996_112_2_18_2_0_29_270 "Receive DBAT_UPDATE_STATE_IND"
#define MMIMAIN_1000_112_2_18_2_0_29_271 "Receive DBAT_BAT_PLUGIN_IND"
#define MMIMAIN_1004_112_2_18_2_0_29_272 "Receive DBAT_BAT_PLUGOUT_IND"
#define MMIMAIN_1008_112_2_18_2_0_29_273 "Receive DBAT_SWITCH_BAT_IND"
#define MMIMAIN_1012_112_2_18_2_0_29_274 "Receive DBAT_CHARGE_START_IND"
#define MMIMAIN_1016_112_2_18_2_0_29_275 "Receive DBAT_WARNING_IND"
#define MMIMAIN_1020_112_2_18_2_0_29_276 "Receive DBAT_SHUTDOWN_IND"
#define MMIMAIN_1024_112_2_18_2_0_29_277 "Receive DBAT_CHARGE_FINISH"
#define MMIMAIN_1028_112_2_18_2_0_29_278 "Receive DBAT_CHARGE_DISCONNECT"
#define MMIMAIN_1032_112_2_18_2_0_29_279 "Receive DBAT_CHARGE_FAULT"
#define MMIMAIN_1038_112_2_18_2_0_29_280 "DualBatteryCallBackFunc:event_id = %d"
#define MMIMAIN_1062_112_2_18_2_0_29_281 "Receive CHR_CAP_IND"
#define MMIMAIN_1073_112_2_18_2_0_29_282 "Receive CHR_CHARGE_START_IND"
#define MMIMAIN_1084_112_2_18_2_0_29_283 "Receive CHR_CHARGE_END_IND"
#define MMIMAIN_1095_112_2_18_2_0_29_284 "Receive CHR_WARNING_IND"
#define MMIMAIN_1106_112_2_18_2_0_29_285 "Receive CHR_SHUTDOWN_IND"
#define MMIMAIN_1117_112_2_18_2_0_29_286 "Receive CHR_CHARGE_FINISH"
#define MMIMAIN_1128_112_2_18_2_0_29_287 "Receive CHR_CHARGE_DISCONNECT"
#define MMIMAIN_1139_112_2_18_2_0_29_288 "Receive CHR_CHARGE_FAULT"
#define MMIMAIN_1150_112_2_18_2_0_29_289 "ChargeCallBackFunc:event_id = %d"
#define MMIMAIN_1746_112_2_18_2_0_30_290 "APP_Task: begin init, time = %d"
#define MMIMAIN_1796_112_2_18_2_0_30_291 "APP_Task: restart_condition is %d,time is %dms"
#define MMIMAIN_1900_112_2_18_2_0_31_292 "APP_Task: end init, time = %d"
#define MMIMAIN_2060_112_2_18_2_0_31_293 "Receive UPM_PLUGIN"
#define MMIMAIN_2065_112_2_18_2_0_31_294 "Receive UPM_PLUGOUT"
#define MMIMAIN_2070_112_2_18_2_0_31_295 "Receive UPM_READY"
#define MMIMAIN_2107_112_2_18_2_0_31_296 "Receive UPM_ERROR"
#define MMIMAIN_2113_112_2_18_2_0_31_297 "UPMCallBackFunc:event_id = %d"
#define MMIMAIN_2181_112_2_18_2_0_31_298 "[MMIMAIN] s_is_flip %d"
#define MMIMAIN_2240_112_2_18_2_0_31_299 "SpecialKeyHandle: flip is open already!"
#define MMIMAIN_2254_112_2_18_2_0_31_300 "SpecialKeyHandle: flip is close already!"
#define MMIMAIN_2278_112_2_18_2_0_31_301 "SpecialKeyHandle: earphone is inserted already!"
#define MMIMAIN_2293_112_2_18_2_0_31_302 "SpecialKeyHandle: earphone isn't inserted!"
#define WINDOW_PARSE_2142_112_2_18_2_0_36_303 "SetWinPrio: ERROR, the priority = %d"
#define WINDOW_PARSE_2178_112_2_18_2_0_36_304 "SetWinId: the window is open! the win_id = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_KERNEL_TRC)
TRACE_MSG(BLOCK_MEM_456_112_2_18_1_59_22_0,"BL_CheckID:invalid id = %d")
TRACE_MSG(BLOCK_MEM_514_112_2_18_1_59_22_1,"Block_Malloc: %d")
TRACE_MSG(BLOCK_MEM_586_112_2_18_1_59_22_2,"Block_Malloc: alloc mem fail!")
TRACE_MSG(BLOCK_MEM_602_112_2_18_1_59_22_3,"BL_Malloc: %d")
TRACE_MSG(BLOCK_MEM_669_112_2_18_1_59_22_4,"BL_Malloc: alloc mem fail!")
TRACE_MSG(BLOCK_MEM_697_112_2_18_1_59_22_5,"BL_Free: %d")
TRACE_MSG(BLOCK_MEM_701_112_2_18_1_59_22_6,"BL_Free:unused id = %d")
TRACE_MSG(BLOCK_MEM_831_112_2_18_1_59_23_7,"MMI_BL_Malloc: %d,%s, line=%ld")
TRACE_MSG(BLOCK_MEM_849_112_2_18_1_59_23_8,"MMI_BL_Malloc: %d,size=%d,%s, line=%ld")
TRACE_MSG(BLOCK_MEM_866_112_2_18_1_59_23_9,"MMI_BL_Malloc: %d,%s, line=%ld")
TRACE_MSG(BLOCK_MEM_882_112_2_18_1_59_23_10,"MMI_BL_Free: %d,%s, line=%ld")
TRACE_MSG(MMI_ATC_1904_112_2_18_1_59_28_11,"mmi_atc.c: ATC_ExtCmd(), received AT command , len = %d")
TRACE_MSG(MMI_ATC_2264_112_2_18_1_59_29_12,"[ATC:][ATEST:][Atest_HandlePhoneBook:] %c")
TRACE_MSG(MMI_ATC_2352_112_2_18_1_59_29_13,"[ATC][Atest_HandlePbSim]: SIM FULL!")
TRACE_MSG(MMI_ATC_2539_112_2_18_1_59_29_14,"[MMIATC]:Atest_HandleURL:%s")
TRACE_MSG(MMI_ATC_2550_112_2_18_1_59_29_15,"[MMIATC]:Atest_HandleURL2:%s, %d")
TRACE_MSG(MMI_ATC_2559_112_2_18_1_59_29_16,"[ATC:][ATEST:][ST:]Atest_HandleURL:url_input:%x, %x, %d")
TRACE_MSG(MMI_ATC_2607_112_2_18_1_59_29_17,"[ATC:][ATEST:][FT:]Atest_HandleConnectGatewayDNS:%x, %x")
TRACE_MSG(MMI_ATC_2669_112_2_18_1_59_29_18,"[ATC:][ATEST:][FT:]Atest_HandleConnectOther:conne_input:%x, %x")
TRACE_MSG(MMI_ATC_2733_112_2_18_1_59_29_19,"[ATC:][ATEST:][Atest_HandleConnection:] %c")
TRACE_MSG(MMI_ATC_2777_112_2_18_1_59_30_20,"[ATC:][FT:][Atest_HandleFtAtMMI:]connection input type error!")
TRACE_MSG(MMI_ATC_2817_112_2_18_1_59_30_21,"[ATC:][ATEST:][FT:]Atest_HandleBmTitle:conne_input:%x, %x, %d")
TRACE_MSG(MMI_ATC_2876_112_2_18_1_59_30_22,"[ATC:][ATEST:][FT:]Atest_HandleBmURL:conne_input:%x, %x, %d")
TRACE_MSG(MMI_ATC_2954_112_2_18_1_59_30_23,"[ATC:][ATEST:][Atest_HandleBookmark:] %c")
TRACE_MSG(MMI_ATC_2993_112_2_18_1_59_30_24,"[ATC:][ATEST:][ST:]Atest_HandleSMS:sms_input:%x, %x")
TRACE_MSG(MMI_ATC_3155_112_2_18_1_59_30_25,"[ATC:][ATEST:][Atest_HandleFolder:] %c")
TRACE_MSG(MMI_ATC_3209_112_2_18_1_59_30_26,"[ATC:][FT:][Atest_HandleStAtMMI:]connection input type error!")
TRACE_MSG(MMI_ATC_3249_112_2_18_1_59_31_27,"[ATC:][MMI:][Atest_HandleAtMMI]input test type error!")
TRACE_MSG(MMI_ATC_3380_112_2_18_1_59_31_28,"[AT][ATEST]:ATest_DispatchExtSig--signalType=0x%x,handleresult=%d")
TRACE_MSG(MMI_ATC_4298_112_2_18_1_59_33_29,"[ATC:][ATEST:]Atest_GetControlTree")
TRACE_MSG(MMI_ATC_4391_112_2_18_1_59_33_30,"[AT][ATEST]:Atest_HandleControlInput--signalType=0x%x")
TRACE_MSG(MMI_ATC_6912_112_2_18_1_59_39_31,"[ATC:][ATEST:]Atest_ClickControl--%d")
TRACE_MSG(MMI_ATC_6918_112_2_18_1_59_39_32,"[ATC:][ATEST:]find Control position succeed--%d--%d")
TRACE_MSG(MMI_ATC_6934_112_2_18_1_59_39_33,"[ATC:][ATEST:] ConvertPosition failed")
TRACE_MSG(MMI_ATC_6940_112_2_18_1_59_39_34,"[ATC:][ATEST:]find Control failed")
TRACE_MSG(MMI_ATC_7033_112_2_18_1_59_39_35,"[ATC:][ATEST:]Search Window--%d")
TRACE_MSG(MMI_ATC_7125_112_2_18_1_59_39_36,"[ATC:][ATEST:]Search Control,ctrl_guid=%d")
TRACE_MSG(MMI_ATC_7416_112_2_18_1_59_40_37,"[ATC:][ATEST:]Atest_KbdConvert,keycode--%d")
TRACE_MSG(MMI_ATC_7479_112_2_18_1_59_40_38,"[ATC:][ATEST:]Atest_SendKbdMessage,signal_keycode=%d")
TRACE_MSG(MMI_ATC_7505_112_2_18_1_59_40_39,"[ATC:][ATEST:]Atest_SendTouchMessage,tp_type--%d")
TRACE_MSG(MMI_ATC_7559_112_2_18_1_59_40_40,"[ATC:][ATEST:]Atest_GetCurLang-%d-%s")
TRACE_MSG(MMI_ATC_7677_112_2_18_1_59_40_41,"[AT][ATEST]:Atest_TouchPanelDown,-%d,%d")
TRACE_MSG(MMI_ATC_7697_112_2_18_1_59_40_42,"[AT][ATEST]:Atest_KBDown,-%d")
TRACE_MSG(MMI_ATC_7719_112_2_18_1_59_40_43,"[AT][ATEST]:Atest_TouchPanelMove,-%d,%d")
TRACE_MSG(MMI_ATC_7741_112_2_18_1_59_40_44,"[AT][ATEST]:Atest_TouchPanelUp,-%d,%d")
TRACE_MSG(MMI_ATC_7769_112_2_18_1_59_40_45,"[ATC:][ATEST:] Atest_ConvertPositionByLcd,%d,%d*%d")
TRACE_MSG(MMI_ATC_7794_112_2_18_1_59_40_46,"[ATC:][ATEST:]ConvertPosition-%d-%d")
TRACE_MSG(MMI_ATC_7810_112_2_18_1_59_40_47,"[ATC:][ATEST:] invalid point")
TRACE_MSG(MMI_ATC_7853_112_2_18_1_59_40_48,"[ATC:][ATEST:]Atest_Compare_content:AT string Num=%d")
TRACE_MSG(MMI_ATC_7879_112_2_18_1_59_40_49,"[ATC:][ATEST:]Atest_Compare_content:Atest_wcslen:ctrl=%d--at=%d")
TRACE_MSG(MMI_ATC_7885_112_2_18_1_59_40_50,"[ATC:][ATEST:]Atest_Compare_content:remove the last word if is space of ctrl_str")
TRACE_MSG(MMI_ATC_7897_112_2_18_1_59_40_51,"[ATC:][ATEST:]Atest_Compare_content:remove the last word if is space of at_str")
TRACE_MSG(MMI_ATC_7905_112_2_18_1_59_40_52,"[ATC:][ATEST:]Atest_Compare_content:Atest_wcslen:ctrl=%d--at=%d")
TRACE_MSG(MMI_ATC_7909_112_2_18_1_59_40_53,"[ATC:][ATEST:]Atest_Compare_content:compare text TRUE")
TRACE_MSG(MMI_ATC_7933_112_2_18_1_59_41_54,"[ATC:][ATEST:]Atest_Compare_content:compare id TRUE")
TRACE_MSG(MMI_ATC_7941_112_2_18_1_59_41_55,"[ATC:][ATEST:]Atest_Compare_content:FALSE")
TRACE_MSG(MMI_ATC_7982_112_2_18_1_59_41_56,"[ATC:][ATEST:]Atest_GetValidTPPoint item_rect:%d %d %d %d--ctrl_rect:%d %d %d %d")
TRACE_MSG(MMI_ATC_8011_112_2_18_1_59_41_57,"[ATC:][ATEST:]Atest_GetValidTPPoint failed")
TRACE_MSG(MMI_ATC_8029_112_2_18_1_59_41_58,"[ATC:][ATEST:]Atest_GetValidTPPoint:%d %d")
TRACE_MSG(MMI_ATC_8421_112_2_18_1_59_42_59,"[ATC:][ATEST:]Atest_SearchQbMenuControlItem,cur_page=%d,item_num=%d,totalpage='%d'")
TRACE_MSG(MMI_ATC_8431_112_2_18_1_59_42_60,"[ATC:][ATEST:]Atest_SearchQbMenuControlItem:page=%d/%d")
TRACE_MSG(MMI_ATC_8435_112_2_18_1_59_42_61,"[ATC:][ATEST:]Atest_SearchQbMenuControlItem:move_page_num=%d stop")
TRACE_MSG(MMI_ATC_8466_112_2_18_1_59_42_62,"[ATC:][ATEST:]Atest_SearchQbMenuControlItem:GUIMENU_MAINMENU_DATA_TYPE_DYNAMIC!")
TRACE_MSG(MMI_ATC_8472_112_2_18_1_59_42_63,"[ATC:][ATEST:]Atest_SearchQbMenuControlItem:GUIMENU_MAINMENU_DATA_TYPE_FILEFOLD!")
TRACE_MSG(MMI_ATC_8479_112_2_18_1_59_42_64,"[ATC:][ATEST:]Atest_SearchQbMenuControlItem,MMITHEME_GetResText,icon_id=%d,text_id=%d,text=%x")
TRACE_MSG(MMI_ATC_8810_112_2_18_1_59_43_65,"[ATC:][ATEST:]Atest_SearchSlideMenuControlItem:page:%d/%d")
TRACE_MSG(MMI_ATC_8957_112_2_18_1_59_43_66,"[ATC:][ATEST:]Atest_SearchPopMenuControlItem,down:cur_item_index=%d,index_offset=%d")
TRACE_MSG(MMI_ATC_8969_112_2_18_1_59_43_67,"[ATC:][ATEST:]Atest_SearchPopMenuControlItem,up:cur_item_index=%d,index_offset=%d")
TRACE_MSG(MMI_ATC_9055_112_2_18_1_59_43_68,"[ATC:][ATEST:]Atest_SearchIconMenuControlItem:page_col=%d,page_row=%d,page_item=%d")
TRACE_MSG(MMI_ATC_9109_112_2_18_1_59_44_69,"[ATC:][ATEST:]Atest_SearchIconMenuControlItem:cur_item_index=%d,index_row=%d")
TRACE_MSG(MMI_ATC_9134_112_2_18_1_59_44_70,"[ATC:][ATEST:]Atest_SearchIconMenuControlItem:cur_item_index=%d,index_row=%d")
TRACE_MSG(MMI_ATC_9210_112_2_18_1_59_44_71,"[ATC:][ATEST:]Atest_SearchOptionMenuControlItem:Find index=%d")
TRACE_MSG(MMI_ATC_9252_112_2_18_1_59_44_72,"[Atest_SearchListByIndex]-searchitemindex=%d")
TRACE_MSG(MMI_ATC_9334_112_2_18_1_59_44_73,"[Atest_SearchListByImgIndex]-searchitemindex=%d")
TRACE_MSG(MMI_ATC_9455_112_2_18_1_59_44_74,"[Atest_SearchListByTextImg]--%x--%x--%x--%x--%x,")
TRACE_MSG(MMI_ATC_9460_112_2_18_1_59_45_75,"[Atest_SearchListByTextImg]-searchitemindex=%d")
TRACE_MSG(MMI_ATC_9486_112_2_18_1_59_45_76,"[ATC:][ATEST:]Atest_SearchListByTextImg:Save buffer data! len=%d\n")
TRACE_MSG(MMI_ATC_9510_112_2_18_1_59_45_77,"[ATC:][ATEST:]Atest_SearchListByTextImg:item_index-%d,content_index=%d")
TRACE_MSG(MMI_ATC_9641_112_2_18_1_59_45_78,"[Atest_SearchListByTextAndImgID]--%x--%x--%x--%x--%x,")
TRACE_MSG(MMI_ATC_9647_112_2_18_1_59_45_79,"[Atest_SearchListByTextAndImgID]-searchitemindex=%d")
TRACE_MSG(MMI_ATC_9676_112_2_18_1_59_45_80,"[ATC:][ATEST:]Atest_SearchListByTextAndImgID:Save buffer data! len=%d\n")
TRACE_MSG(MMI_ATC_9724_112_2_18_1_59_45_81,"[ATC:][ATEST:]Atest_SearchListByTextAndImgID:item_index-%d,content_index=%d,content_type=%d")
TRACE_MSG(MMI_ATC_9827_112_2_18_1_59_45_82,"[Atest_SearchListControlItem]totalnum=%d,itemheight=%d")
TRACE_MSG(MMI_ATC_10517_112_2_18_1_59_47_83,"[ATC][ATEST]:Iconlist:index_ptr=%d,row=%d,col=%d,item=%d,top=%d,bottom=%d,cur=%d")
TRACE_MSG(MMI_ATC_11114_112_2_18_1_59_49_84,"mmi_atc.c: ParseSyncAtCmd(), mmi atc is not run")
TRACE_MSG(MMI_ATC_11131_112_2_18_1_59_49_85,"mmi_atc.c: ParseSyncAtCmd(), module_type = %c, operation_type = %c")
TRACE_MSG(MMI_ATC_11299_112_2_18_1_59_49_86,"ParseSyncUTAtCmd, cmd[0] 0x%x")
TRACE_MSG(MMI_ATC_11306_112_2_18_1_59_49_87,"ParseSyncUTAtCmd, get module_type error")
TRACE_MSG(MMI_ATC_11313_112_2_18_1_59_49_88,"ParseSyncUTAtCmd, module_type strlen %d, cmd_len %d")
TRACE_MSG(MMI_ATC_11321_112_2_18_1_59_49_89,"ParseSyncUTAtCmd, Get operation_type error")
TRACE_MSG(MMI_ATC_11328_112_2_18_1_59_49_90,"ParseSyncUTAtCmd, operation_type strlen %d, cmd_len %d")
TRACE_MSG(MMI_ATC_11333_112_2_18_1_59_49_91,"mmi_atc.c: ParseSyncAtCmd(), module_type = %c, operation_type = %c")
TRACE_MSG(MMI_ATC_11395_112_2_18_1_59_49_92,"[baokun] MMI_AUTO_TEST_ACO_OFF")
TRACE_MSG(MMI_ATC_11402_112_2_18_1_59_49_93,"[baokun] MMI_AUTO_TEST_ACO_ON")
TRACE_MSG(MMI_ATC_11409_112_2_18_1_59_49_94,"[baokun] MMI_AUTO_TEST_HEADSET_MODE_OFF")
TRACE_MSG(MMI_ATC_11416_112_2_18_1_59_49_95,"[baokun] MMI_AUTO_TEST_HEADSET_MODE_ON")
TRACE_MSG(MMI_ATC_11433_112_2_18_1_59_49_96,"[baokun] MMI_AUTO_TEST_OFF is_on %d")
TRACE_MSG(MMI_ATC_11439_112_2_18_1_59_49_97,"[baokun] MMI_AUTO_TEST_ON is_on %d")
TRACE_MSG(MMI_ATC_11472_112_2_18_1_59_49_98," AutoTest Lcd Brightness  MMI_AUTO_TEST_OFF is_on %d")
TRACE_MSG(MMI_ATC_11570_112_2_18_1_59_50_99,"mmi_atc.c: HandlePbGetInfo(), pos = %d")
TRACE_MSG(MMI_ATC_11577_112_2_18_1_59_50_100,"mmi_atc.c: HandlePbGetInfo(), max_record_num = %d, used_record_num = %d")
TRACE_MSG(MMI_ATC_11632_112_2_18_1_59_50_101,"mmi_atc.c: HandlePbReadItem(), pos = %d, entry_id = %d")
TRACE_MSG(MMI_ATC_11639_112_2_18_1_59_50_102,"mmi_atc.c: HandlePbReadItem(), entry_id = %d")
TRACE_MSG(MMI_ATC_11699_112_2_18_1_59_50_103,"mmi_atc.c: HandlePbAdd(), pos = %d, entry_id = %d")
TRACE_MSG(MMI_ATC_11729_112_2_18_1_59_50_104,"mmi_atc.c: MMISYNC_PbDeleteCallback, is_success = %d, s_ispbop_wait = %d")
TRACE_MSG(MMI_ATC_11767_112_2_18_1_59_50_105,"mmi_atc.c: HandlePbDelete(), pos = %d, entry_id = %d")
TRACE_MSG(MMI_ATC_11815_112_2_18_1_59_50_106,"mmi_atc.c: HandlePbModify(), pos = %d, entry_id = %d")
TRACE_MSG(MMI_ATC_11845_112_2_18_1_59_50_107,"mmi_atc.c: MMISYNC_PbUpdateCallback, is_success = %d, s_ispbop_wait = %d")
TRACE_MSG(MMI_ATC_11882_112_2_18_1_59_50_108,"mmi_atc.c:HandleSmsDelete(), length error, len = %d")
TRACE_MSG(MMI_ATC_11893_112_2_18_1_59_50_109,"mmi_atc.c:HandleSmsDelete(), record_id_str = %s")
TRACE_MSG(MMI_ATC_11906_112_2_18_1_59_50_110,"mmi_atc.c:HandleSmsDelete(), SMS status error,  %s")
TRACE_MSG(MMI_ATC_11924_112_2_18_1_59_50_111,"mmi_atc.c:HandleSmsDelete(), SMS storage error,  %d")
TRACE_MSG(MMI_ATC_11931_112_2_18_1_59_50_112,"mmi_atc.c: HandleSmsDelete(), record_id = %d, is_mt_sms = %d, pos = %d")
TRACE_MSG(MMI_ATC_12442_112_2_18_1_59_51_113,"[MMIATC] GetPbStorage pos 0x%x")
TRACE_MSG(MMI_ATC_12560_112_2_18_1_59_52_114,"[MMIATC] DecompressCmdStr out_len %d")
TRACE_MSG(MMI_ATC_12574_112_2_18_1_59_52_115,"[MMIATC] DecompressCmdStr out_len %d")
TRACE_MSG(MMI_ATC_12625_112_2_18_1_59_52_116,"[MMIATC] HandleSmsSendMsg wstr_msg_ptr %x  %x")
TRACE_MSG(MMI_ATC_12686_112_2_18_1_59_52_117,"[MMIATC] HandleSmsSendMsg wstr_msg_ptr %x  %x")
TRACE_MSG(MMI_ATC_12749_112_2_18_1_59_52_118,"[MMIATC] MMIAPIMMS_InsertMessage wstr_msg_ptr %x  %x")
TRACE_MSG(MMI_ATC_12812_112_2_18_1_59_52_119,"[MMIATC] MMIAPIWWW_InsertURL wstr_msg_ptr %x  %x")
TRACE_MSG(MMI_ATC_12873_112_2_18_1_59_52_120,"[MMIATC] MIAPICONNECTION_InsertStr wstr_msg_ptr %x  %x")
TRACE_MSG(MMI_ATC_12907_112_2_18_1_59_52_121,"[MMIATC] GetATCParameter cmd == PNULL !")
TRACE_MSG(MMI_ATC_12912_112_2_18_1_59_52_122,"[MMIATC] GetATCParameter cmd %x  %x  %x  %x  %x  %x  %x  %x  %x")
TRACE_MSG(MMI_ATC_12977_112_2_18_1_59_52_123,"[MMIATC] GetATCNextString parameter error !")
TRACE_MSG(MMI_ATC_12995_112_2_18_1_59_52_124,"[MMIATC] GetATCNextString string %s result %d")
TRACE_MSG(MMI_AUTOTEST_306_112_2_18_1_59_53_125,"---autotest---%s----%d----")
TRACE_MSG(MMI_AUTOTEST_353_112_2_18_1_59_54_126,"---autotest---%s----%d----")
TRACE_MSG(MMI_AUTOTEST_387_112_2_18_1_59_54_127,"---autotest---%s----%d----")
TRACE_MSG(MMI_AUTOTEST_416_112_2_18_1_59_54_128,"---autotest---%s----%d----")
TRACE_MSG(MMI_AUTOTEST_642_112_2_18_1_59_54_129,"[AUTOTEST] MMI_AutoTest_Trace invalid parameter !")
TRACE_MSG(MMI_AUTOTEST_649_112_2_18_1_59_54_130,"[AUTOTEST] MMI_AutoTest_Trace malloc fail parameter !")
TRACE_MSG(MMI_AUTOTEST_707_112_2_18_1_59_54_131,"[AUTOTEST] MMI_AutoTest_Trace invalid parameter !")
TRACE_MSG(MMI_AUTOTEST_714_112_2_18_1_59_54_132,"[AUTOTEST] MMI_AutoTest_Trace malloc fail parameter !")
TRACE_MSG(MMI_DEFAULT_1184_112_2_18_1_59_57_133,"[MMK_DefaultMSGKbd]Hook key is pressed.")
TRACE_MSG(MMI_DEFAULT_1190_112_2_18_1_59_57_134,"[MMK_DefaultMSGKbd]Now mode is handfree.")
TRACE_MSG(MMI_DEFAULT_1196_112_2_18_1_59_57_135,"[MMK_DefaultMSGKbd]Mode is not handfree, so stop all ring.")
TRACE_MSG(MMI_DEFAULT_1207_112_2_18_1_59_57_136,"[MMK_DefaultMSGKbd]Hook key is released.")
TRACE_MSG(MMI_DEFAULT_1257_112_2_18_1_59_57_137,"[MMK_DefaultMSGKbd]HandFree key is pressed")
TRACE_MSG(MMI_DEFAULT_1436_112_2_18_1_59_58_138,"[MMK_DefaultMSGKbd](KEY_PRESSED)MMIAUDIO_SetVolume,AUDIO_DEVICE_MODE_EARPHONE, %d")
TRACE_MSG(MMI_DEFAULT_1493_112_2_18_1_59_58_139,"[MMK_DefaultMSGKbd](KEY_RELEASED)MMIAUDIO_SetVolume, AUDIO_DEVICE_MODE_HANDHOLD ,%d")
TRACE_MSG(MMI_DEFAULT_2535_112_2_18_2_0_0_140,"MMK_HandleHeadSetKeyProcess: warning! There is no call window exists when in call state!")
TRACE_MSG(MMI_DEFAULT_2681_112_2_18_2_0_0_141,"VirWin_HandleMsg: received headset timer")
TRACE_MSG(MMI_DEFAULT_2796_112_2_18_2_0_1_142,"MMK_DefaultMSGKbd: can not power off because be forbidden by engineer mode")
TRACE_MSG(MMI_DEFAULT_2842_112_2_18_2_0_1_143,"MMK_HandlePublicKey: MSG_KEYDOWN_FLIP!")
TRACE_MSG(MMI_DEFAULT_2848_112_2_18_2_0_1_144,"MMK_HandlePublicKey: MSG_KEYUP_FLIP!")
TRACE_MSG(MMI_DEFAULT_2854_112_2_18_2_0_1_145,"MMK_HandlePublicKey: MSG_KEYDOWN_HEADSET_BUTTON!")
TRACE_MSG(MMI_DEFAULT_2859_112_2_18_2_0_1_146,"MMK_HandlePublicKey, msg_id = MSG_KEYDOWN_HEADSET_DETECT")
TRACE_MSG(MMI_DEFAULT_2877_112_2_18_2_0_1_147,"MMK_HandlePublicKey, msg_id = MSG_KEYUP_HEADSET_DETECT")
TRACE_MSG(MMI_DEFAULT_2895_112_2_18_2_0_1_148,"MMK_HandlePublicKey:MSG_KEYDOWN_DOWNSIDE stop ring")
TRACE_MSG(MMI_DEFAULT_2900_112_2_18_2_0_1_149,"MMK_HandlePublicKey: can not power off because be forbidden by engineer mode")
TRACE_MSG(MMI_DEFAULT_2918_112_2_18_2_0_1_150,"---WD:MMK_HandlePublicKey(),hook key is pressed")
TRACE_MSG(MMI_DEFAULT_2928_112_2_18_2_0_1_151,"TRUE == CC_IsInCall ,CC_RELEASE_ALL_NORMAL")
TRACE_MSG(MMI_DEFAULT_2933_112_2_18_2_0_1_152,"---WD:hook key is pressed,return to idle windows")
TRACE_MSG(MMI_DEFAULT_2940_112_2_18_2_0_1_153,"---WD:MMK_HandlePublicKey(),HANDSFREE key is pressed")
TRACE_MSG(MMI_DEFAULT_2944_112_2_18_2_0_1_154,"---WD:handfree key is pressed,Release all call")
TRACE_MSG(MMI_DEFAULT_2950_112_2_18_2_0_1_155,"---WD:MMK_HandlePublicKey(),open dial window")
TRACE_MSG(MMI_DEFAULT_3034_112_2_18_2_0_1_156,"MMK_HandlePublicKey: can't handle msg! the msg_id = %d")
TRACE_MSG(MMI_DEFAULT_3120_112_2_18_2_0_1_157,"MMIDEFAULT_TurnOnLCDBackLight: s_backlight_ctrol = %d, s_backlight_status = %d")
TRACE_MSG(MMI_DEFAULT_3345_112_2_18_2_0_2_158,"MMIDEFAULT_AllowBackLight: s_backlight_ctrol=%d, allow_light = %d!")
TRACE_MSG(MMI_DEFAULT_3349_112_2_18_2_0_2_159,"MMIDEFAULT_AllowBackLight vt calling ")
TRACE_MSG(MMI_DEFAULT_3379_112_2_18_2_0_2_160,"MMIDEFAULT_AllowHalfOnBackLight:s_backlight_ctrol=%d, b_half_on = %d")
TRACE_MSG(MMI_DEFAULT_3412_112_2_18_2_0_2_161,"MMIDEFAULT_AllowHalfOnBackLight:s_backlight_ctrol=%d, b_half_on = %d")
TRACE_MSG(MMI_DEFAULT_3510_112_2_18_2_0_2_162,"MMIDEFAULT_StartLcdBackLightTimer backlight_type is %d,s_backlight_ctrol is %d,s_backlight_status is %d")
TRACE_MSG(MMI_DEFAULT_3613_112_2_18_2_0_2_163,"[default]: MMIDEFAULT_SetKeybadBackLight(%d) ")
TRACE_MSG(MMI_DEFAULT_3671_112_2_18_2_0_2_164,"MMIDEFAULT_SetLcdSleep: Focus win!")
TRACE_MSG(MMI_DEFAULT_3813_112_2_18_2_0_3_165,"MMIDEFAULT_HandleSlideKey: msg_id = %d")
TRACE_MSG(MMI_DEFAULT_3853_112_2_18_2_0_3_166,"MMIDEFAULT_HandleSlideKey invalid msg = %d")
TRACE_MSG(MMI_DEFAULT_3868_112_2_18_2_0_3_167,"MMIDEFAULT_HandleFlipKey: is_flipup = %d")
TRACE_MSG(MMI_DEFAULT_3918_112_2_18_2_0_3_168,"mmi_default: MMIDEFAULT_HandleBackLightOver()")
TRACE_MSG(MMI_DEFAULT_4084_112_2_18_2_0_3_169,"MMIDEFAULT_TurnOffBackLight, focus_win_id = %d")
TRACE_MSG(MMI_DEFAULT_4150_112_2_18_2_0_3_170,"MMK: backlight is off, abandon tp down msg!")
TRACE_MSG(MMI_DEFAULT_4176_112_2_18_2_0_3_171,"MMK: backlight is off, abandon tp up msg; but turn on th backlight!")
TRACE_MSG(MMI_DEFAULT_4200_112_2_18_2_0_3_172,"MMK: backlight is off, abandon tp move msg!")
TRACE_MSG(MMI_DEFAULT_4206_112_2_18_2_0_3_173,"MMK_DefaultMSGKtp msg_id = %d")
TRACE_MSG(MMI_DEFAULT_4377_112_2_18_2_0_4_174,"MMK_HandleMmiSig: receive 0x%x")
TRACE_MSG(MMI_DEFAULT_4382_112_2_18_2_0_4_175,"MMK_HandleMmiSig: receive APP_COPY_END_IND")
TRACE_MSG(MMI_DEFAULT_4451_112_2_18_2_0_4_176,"ATEST_TRACE====Enter ATEST MODULE!")
TRACE_MSG(MMI_DEFAULT_4458_112_2_18_2_0_4_177,"MMK_HandleMmiSig: receive unknown signal! sig_code = %d")
TRACE_MSG(MMI_DEFAULT_4894_112_2_18_2_0_5_178,"MMK_HandleWinMoveMsg msg = 0x%x")
TRACE_MSG(MMI_DEFAULT_4915_112_2_18_2_0_5_179,"MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: idlewin disable!")
TRACE_MSG(MMI_DEFAULT_4919_112_2_18_2_0_5_180,"MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: childwin disable!")
TRACE_MSG(MMI_DEFAULT_4941_112_2_18_2_0_5_181,"MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: roll close!")
TRACE_MSG(MMI_DEFAULT_4948_112_2_18_2_0_5_182,"MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: IsDisable3DWindow!")
TRACE_MSG(MMI_DEFAULT_4954_112_2_18_2_0_5_183,"MMK_HandleWinMoveMsg MSG_LOSE_FOCUS: not Permit3D!")
TRACE_MSG(MMI_DEFAULT_5001_112_2_18_2_0_5_184,"MMK_HandleWinMoveMsg MSG_CLOSE_WINDOW: roll close!")
TRACE_MSG(MMI_DEFAULT_5007_112_2_18_2_0_5_185,"MMK_HandleWinMoveMsg MSG_CLOSE_WINDOW: IsDisable3DWindow!")
TRACE_MSG(MMI_DEFAULT_5013_112_2_18_2_0_5_186,"MMK_HandleWinMoveMsg MSG_CLOSE_WINDOW: not focus win!")
TRACE_MSG(MMI_DEFAULT_5018_112_2_18_2_0_5_187,"MMK_HandleWinMoveMsg MSG_CLOSE_WINDOW: not Permit3D!")
TRACE_MSG(MMI_DEFAULT_5075_112_2_18_2_0_5_188,"MMK_HandleWinMoveMsg MSG_FULL_PAINT: delay to parent win!")
TRACE_MSG(MMI_DEFAULT_5084_112_2_18_2_0_5_189,"MMK_HandleWinMoveMsg MSG_FULL_PAINT: no need to roll!")
TRACE_MSG(MMI_DEFAULT_5093_112_2_18_2_0_5_190,"MMK_HandleWinMoveMsg MSG_FULL_PAINT: no need to roll!")
TRACE_MSG(MMI_DEFAULT_5102_112_2_18_2_0_5_191,"MMK_HandleWinMoveMsg MSG_FULL_PAINT: no need to roll!")
TRACE_MSG(MMI_DEFAULT_5133_112_2_18_2_0_5_192,"MMK_HandleWinMoveMsg MSG_FULL_PAINT: no need to roll!")
TRACE_MSG(MMI_DEFAULT_5141_112_2_18_2_0_5_193,"MMK_HandleWinMoveMsg MSG_FULL_PAINT: Is Child Win Switch!")
TRACE_MSG(MMI_DEFAULT_5151_112_2_18_2_0_5_194,"MMK_HandleWinMoveMsg MSG_FULL_PAINT: not Permit3D!")
TRACE_MSG(MMI_DEFAULT_5259_112_2_18_2_0_6_195,"IsPermitDesktop3D is_3dblockmem_usedbyother = %d")
TRACE_MSG(MMI_DEFAULT_5294_112_2_18_2_0_6_196,"MMI_IsPermit3D is_3dmmi_enable = %d, is_play_mp3bg = %d, is_play_kurobg = %d")
TRACE_MSG(MMI_DEFAULT_5397_112_2_18_2_0_6_197,"SaveOldMoveBuf AllocTrans3DBuf fail")
TRACE_MSG(MMI_DEFAULT_5832_112_2_18_2_0_7_198,"MMIDEFAULT_ConvertSlideKey input param error!")
TRACE_MSG(MMI_DEFAULT_5914_112_2_18_2_0_7_199,"MMIDEFAULT_SetToneStates is_tone_on=%d")
TRACE_MSG(MMI_DEFAULT_5929_112_2_18_2_0_7_200,"MMIDEFAULT_SetHandFreeStatesAndLed: is_handfree=%d")
TRACE_MSG(MMI_DEFAULT_5966_112_2_18_2_0_7_201,"MMIDEFAULT_SetHookStates: is_hook=%d")
TRACE_MSG(MMI_DEFAULT_5979_112_2_18_2_0_7_202,"MMIDEFAULT_SetHandFreeStates: is_handfree=%d, %s, %d")
TRACE_MSG(MMI_DEFAULT_5988_112_2_18_2_0_7_203,"MMIDEFAULT_GetHandFreeStates: _is_handfree=%d")
TRACE_MSG(MMI_DEFAULT_5998_112_2_18_2_0_7_204,"MMIDEFAULT_GetHookStates: is_hook=%d")
TRACE_MSG(MMI_DEFAULT_6012_112_2_18_2_0_7_205,"MMIDEFAULT_GetBackLightState: %d")
TRACE_MSG(MMI_DEFAULT_6243_112_2_18_2_0_8_206,"DrawTurnOnBackLightAnim UILAYER_GetBltInfoForPrintScreen ERROR!")
TRACE_MSG(MMI_DEFAULT_6259_112_2_18_2_0_8_207,"DrawTurnOnBackLightAnim CreateLayer FAILED result = %d")
TRACE_MSG(MMI_DEFAULT_6286_112_2_18_2_0_8_208,"DrawTurnOnBackLightAnim 0 == handle")
TRACE_MSG(MMI_DEFAULT_6293_112_2_18_2_0_8_209,"DrawTurnOnBackLightAnim PNULL == render_in.src_chn.chn0.ptr")
TRACE_MSG(MMI_DEFAULT_6308_112_2_18_2_0_8_210,"DrawTurnOnBackLightAnim PNULL == render_in.dst_chn.chn0.ptr")
TRACE_MSG(MMI_EVENT_API_193_112_2_18_2_0_8_211,"updatetime 1.\n")
TRACE_MSG(MMI_EVENT_API_198_112_2_18_2_0_8_212,"updatetime 2.\n")
TRACE_MSG(MMI_EVENT_API_203_112_2_18_2_0_8_213,"updatetime 3.\n")
TRACE_MSG(MMI_EVENT_API_208_112_2_18_2_0_8_214,"udiskmount 1.\n")
TRACE_MSG(MMI_EVENT_API_213_112_2_18_2_0_8_215,"udiskmount 2.\n")
TRACE_MSG(MMI_MODU_MAIN_101_112_2_18_2_0_10_216,"mmi_modu_main.c enter MMI_InitilizeWinIdNameArr()")
TRACE_MSG(MMI_MODU_MAIN_136_112_2_18_2_0_10_217,"module_id = %d, id_offset = %d")
TRACE_MSG(MMI_MODU_MAIN_220_112_2_18_2_0_10_218,"[RES]: image handle is not belong window, type = 0x%x, label = 0x%x, win_id = 0x%x")
TRACE_MSG(MMI_MODU_MAIN_249_112_2_18_2_0_10_219,"[RES]: anim handle is not belong window, type = 0x%x, label = 0x%x, win_id = 0x%x")
TRACE_MSG(MMI_MODU_MAIN_595_112_2_18_2_0_11_220,"mmi_modu_main.c enter MMI_CheckAutoTestWinId()")
TRACE_MSG(MMI_MODU_MAIN_601_112_2_18_2_0_11_221,"MMI_CheckAutoTestWinId module%d win_id_name is NULL!")
TRACE_MSG(MMI_MODU_MAIN_648_112_2_18_2_0_11_222,"MMI_MODU_MAIN:TV FLASH MID[0x%x] DID[0x%x] EID[0x%x]")
TRACE_MSG(MMI_MODU_MAIN_671_112_2_18_2_0_11_223,"MMI_MODU_MAIN:TV FLASH MID[0x%x] DID[0x%x] EID[0x%x]")
TRACE_MSG(MMI_MODU_MAIN_774_112_2_18_2_0_11_224,"[MMINV]:MMI_MODULE_SMS total size = %d")
TRACE_MSG(MMI_MODU_MAIN_780_112_2_18_2_0_11_225,"[MMINV]:MMI_MODULE_PB total size = %d")
TRACE_MSG(MMI_MODU_MAIN_787_112_2_18_2_0_11_226,"[MMINV]:MMI_MODULE_KURO total size = %d")
TRACE_MSG(MMI_MODU_MAIN_793_112_2_18_2_0_11_227,"[MMINV]:MMI_MODULE_FM total size = %d")
TRACE_MSG(MMI_MODU_MAIN_798_112_2_18_2_0_11_228,"[MMINV]:%d ")
TRACE_MSG(MMI_MODU_MAIN_803_112_2_18_2_0_11_229,"[MMINV]:NV_ID=%d size=%d")
TRACE_MSG(MMI_MODU_MAIN_806_112_2_18_2_0_11_230,"[MMINV]:%d ")
TRACE_MSG(MMI_MODU_MAIN_811_112_2_18_2_0_11_231,"[MMINV]: total module nv size = %d, all nv item num = %d")
TRACE_MSG(MMI_OSBRIDGE_50_112_2_18_2_0_11_232,"MMI : alloc signal error, signal szie = %d")
TRACE_MSG(MMI_RESOURCE_1711_112_2_18_2_0_16_233,"[RES]: s_handle_map[%d].handle_id = 0x%x = %d")
TRACE_MSG(MMI_RESOURCE_1724_112_2_18_2_0_16_234,"[RES]: com_id = %d, res_id = 0x%x, size = %d, flag1 = 0x%x, flag2 = 0x%x, type = %d")
TRACE_MSG(MMI_RESOURCE_2022_112_2_18_2_0_17_235,"CAFResMemDestroy: warring! the mmk manager memory has be destroy")
TRACE_MSG(MMI_RESOURCE_2180_112_2_18_2_0_17_236,"ResMalloc fail,will malloc 1, ret_ptr = %d")
TRACE_MSG(MMI_RESOURCE_2190_112_2_18_2_0_17_237,"ResMalloc fail,will malloc 2, ret_ptr = %d")
TRACE_MSG(MMI_RESOURCE_2202_112_2_18_2_0_17_238,"ResMalloc fail,will malloc from heap, cur size = %d")
TRACE_MSG(MMI_RESOURCE_2307_112_2_18_2_0_17_239,"[RES]: FreeDynResNodeByHandle: handle_id = %d")
TRACE_MSG(MMI_RESOURCE_3674_112_2_18_2_0_20_240,"MMI_GetLabelAnim com_id = %d, anim_id = %d")
TRACE_MSG(MMI_RESOURCE_3906_112_2_18_2_0_21_241,"MMI_GetLabelImage com_id = %d, image_id = %d")
TRACE_MSG(MMI_RESOURCE_4226_112_2_18_2_0_21_242,"[MMIRES] Text cache full. offset = %d, wstr_len=%d")
TRACE_MSG(MMI_RESOURCE_4623_112_2_18_2_0_22_243,"[RES]: MMIRES_LoadComResource comd_id = %d start")
TRACE_MSG(MMI_RESOURCE_4658_112_2_18_2_0_22_244,"[RES]: MMIRES_LoadComResource end")
TRACE_MSG(MMI_RESOURCE_4728_112_2_18_2_0_22_245,"[RES]: FreeDynResNodeByComID: com_id = %d, GetDLinkHeadPtr = 0x%x")
TRACE_MSG(MMI_RESOURCE_4806_112_2_18_2_0_23_246,"[RES]: MMIRES_EmptyComRes: com_id = %d, store_type = 0x%x")
TRACE_MSG(MMI_RESOURCE_5410_112_2_18_2_0_24_247,"MMIRES_LoadBookmarkFromRes is_res_data_exist=%d, bookmark_num=%d")
TRACE_MSG(MMI_RGLEXPR_729_112_2_18_2_0_26_248,"---RunFSM break--")
TRACE_MSG(MMI_RGLEXPR_843_112_2_18_2_0_26_249,"---ConvertCH---%x---")
TRACE_MSG(MMI_RGLEXPR_846_112_2_18_2_0_26_250,"---RunFSM UCS2 break--")
TRACE_MSG(MMI_RGLEXPR_854_112_2_18_2_0_26_251,"---IsEndState success--")
TRACE_MSG(MMI_RGLEXPR_860_112_2_18_2_0_26_252,"---IsEndState fail--")
TRACE_MSG(MMI_RGLEXPR_885_112_2_18_2_0_26_253,"---FindPatternUCS2 %d--")
TRACE_MSG(MMI_RGLEXPR_916_112_2_18_2_0_26_254,"---FindMorePatternUCS2 %d--")
TRACE_MSG(MMI_RGLEXPR_920_112_2_18_2_0_26_255,"---FindMorePatternUCS2 break--")
TRACE_MSG(MMIMAIN_430_112_2_18_2_0_27_256,"\"\"  MMI_SetFreq   \"#")
TRACE_MSG(MMIMAIN_447_112_2_18_2_0_27_257,"\"\"  MMI_RestoreFreq   \"#")
TRACE_MSG(MMIMAIN_501_112_2_18_2_0_28_258,"Jesse Yu change call key to camera key")
TRACE_MSG(MMIMAIN_506_112_2_18_2_0_28_259,"Jesse Yu change up key to SIDE_UP key")
TRACE_MSG(MMIMAIN_511_112_2_18_2_0_28_260,"Jesse Yu change down key to SIDE_down key")
TRACE_MSG(MMIMAIN_524_112_2_18_2_0_28_261,"Jesse Yu flip up")
TRACE_MSG(MMIMAIN_530_112_2_18_2_0_28_262,"Jesse Yu flip down")
TRACE_MSG(MMIMAIN_535_112_2_18_2_0_28_263,"Jesse Yu star key down")
TRACE_MSG(MMIMAIN_565_112_2_18_2_0_28_264,"KeypadCallBackFunc: the key message is too much!")
TRACE_MSG(MMIMAIN_666_112_2_18_2_0_28_265,"TPCallBackFunc: the TP message is too much!")
TRACE_MSG(MMIMAIN_752_112_2_18_2_0_28_266,"TPCallBackFunc: the TP message is too much!")
TRACE_MSG(MMIMAIN_791_112_2_18_2_0_28_267,"TPCallBackFunc: the TP message is invalid!")
TRACE_MSG(MMIMAIN_821_112_2_18_2_0_28_268,"GsensorCallBackFunc: gsensor_code = %d")
TRACE_MSG(MMIMAIN_825_112_2_18_2_0_28_269,"GsensorCallBackFunc: the gsensor message is too much!")
TRACE_MSG(MMIMAIN_996_112_2_18_2_0_29_270,"Receive DBAT_UPDATE_STATE_IND")
TRACE_MSG(MMIMAIN_1000_112_2_18_2_0_29_271,"Receive DBAT_BAT_PLUGIN_IND")
TRACE_MSG(MMIMAIN_1004_112_2_18_2_0_29_272,"Receive DBAT_BAT_PLUGOUT_IND")
TRACE_MSG(MMIMAIN_1008_112_2_18_2_0_29_273,"Receive DBAT_SWITCH_BAT_IND")
TRACE_MSG(MMIMAIN_1012_112_2_18_2_0_29_274,"Receive DBAT_CHARGE_START_IND")
TRACE_MSG(MMIMAIN_1016_112_2_18_2_0_29_275,"Receive DBAT_WARNING_IND")
TRACE_MSG(MMIMAIN_1020_112_2_18_2_0_29_276,"Receive DBAT_SHUTDOWN_IND")
TRACE_MSG(MMIMAIN_1024_112_2_18_2_0_29_277,"Receive DBAT_CHARGE_FINISH")
TRACE_MSG(MMIMAIN_1028_112_2_18_2_0_29_278,"Receive DBAT_CHARGE_DISCONNECT")
TRACE_MSG(MMIMAIN_1032_112_2_18_2_0_29_279,"Receive DBAT_CHARGE_FAULT")
TRACE_MSG(MMIMAIN_1038_112_2_18_2_0_29_280,"DualBatteryCallBackFunc:event_id = %d")
TRACE_MSG(MMIMAIN_1062_112_2_18_2_0_29_281,"Receive CHR_CAP_IND")
TRACE_MSG(MMIMAIN_1073_112_2_18_2_0_29_282,"Receive CHR_CHARGE_START_IND")
TRACE_MSG(MMIMAIN_1084_112_2_18_2_0_29_283,"Receive CHR_CHARGE_END_IND")
TRACE_MSG(MMIMAIN_1095_112_2_18_2_0_29_284,"Receive CHR_WARNING_IND")
TRACE_MSG(MMIMAIN_1106_112_2_18_2_0_29_285,"Receive CHR_SHUTDOWN_IND")
TRACE_MSG(MMIMAIN_1117_112_2_18_2_0_29_286,"Receive CHR_CHARGE_FINISH")
TRACE_MSG(MMIMAIN_1128_112_2_18_2_0_29_287,"Receive CHR_CHARGE_DISCONNECT")
TRACE_MSG(MMIMAIN_1139_112_2_18_2_0_29_288,"Receive CHR_CHARGE_FAULT")
TRACE_MSG(MMIMAIN_1150_112_2_18_2_0_29_289,"ChargeCallBackFunc:event_id = %d")
TRACE_MSG(MMIMAIN_1746_112_2_18_2_0_30_290,"APP_Task: begin init, time = %d")
TRACE_MSG(MMIMAIN_1796_112_2_18_2_0_30_291,"APP_Task: restart_condition is %d,time is %dms")
TRACE_MSG(MMIMAIN_1900_112_2_18_2_0_31_292,"APP_Task: end init, time = %d")
TRACE_MSG(MMIMAIN_2060_112_2_18_2_0_31_293,"Receive UPM_PLUGIN")
TRACE_MSG(MMIMAIN_2065_112_2_18_2_0_31_294,"Receive UPM_PLUGOUT")
TRACE_MSG(MMIMAIN_2070_112_2_18_2_0_31_295,"Receive UPM_READY")
TRACE_MSG(MMIMAIN_2107_112_2_18_2_0_31_296,"Receive UPM_ERROR")
TRACE_MSG(MMIMAIN_2113_112_2_18_2_0_31_297,"UPMCallBackFunc:event_id = %d")
TRACE_MSG(MMIMAIN_2181_112_2_18_2_0_31_298,"[MMIMAIN] s_is_flip %d")
TRACE_MSG(MMIMAIN_2240_112_2_18_2_0_31_299,"SpecialKeyHandle: flip is open already!")
TRACE_MSG(MMIMAIN_2254_112_2_18_2_0_31_300,"SpecialKeyHandle: flip is close already!")
TRACE_MSG(MMIMAIN_2278_112_2_18_2_0_31_301,"SpecialKeyHandle: earphone is inserted already!")
TRACE_MSG(MMIMAIN_2293_112_2_18_2_0_31_302,"SpecialKeyHandle: earphone isn't inserted!")
TRACE_MSG(WINDOW_PARSE_2142_112_2_18_2_0_36_303,"SetWinPrio: ERROR, the priority = %d")
TRACE_MSG(WINDOW_PARSE_2178_112_2_18_2_0_36_304,"SetWinId: the window is open! the win_id = %d")
END_TRACE_MAP(MMI_APP_KERNEL_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_KERNEL_TRC_H_

