/******************************************************************************
 ** File Name:      mmi_kernel_trc.h                                         *
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
//trace_id:257
#ifndef _MMI_KERNEL_TRC_H_
#define _MMI_KERNEL_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMK_KBD_174_112_2_18_3_22_38_0 "HandleMSGKbd result = %d"
#define MMK_KBD_188_112_2_18_3_22_38_1 "HandleMSGKbd recode = %d"
#define MMK_KBD_339_112_2_18_3_22_39_2 "MMK_DispatchMSGKbd: keystatus = %d, keycode = %d."
#define MMK_KBD_361_112_2_18_3_22_39_3 "@KPD_DOWN kbd_dblclk_state = %d.\n"
#define MMK_KBD_419_112_2_18_3_22_39_4 "@KPD_UP kbd_dblclk_state = %d.\n"
#define MMK_KBD_433_112_2_18_3_22_39_5 "MMK_DispatchMSGKbd signalCode = %d"
#define MMK_KBD_569_112_2_18_3_22_39_6 "CheckHandleKey: has tp down,not handle key down msg!"
#define MMK_KBD_1010_112_2_18_3_22_40_7 "CheckKeyMsgNumber:key full, waiting_keys_num = %d,keycode_num=%d, %0x, %0x, %0x!"
#define MMK_MSG_413_112_2_18_3_22_41_8 "MMK_MSG.C mmk send msg handle is 0!!!"
#define MMK_MSG_462_112_2_18_3_22_41_9 "MMK_MSG.C: AddMsgToQue the message queue is full. len = %d, readpos = %d, writepos = %d"
#define MMK_MSG_579_112_2_18_3_22_42_10 "MMK_DispatchExtSig, send = %u"
#define MMK_MSG_897_112_2_18_3_22_42_11 "MMK_DispatchToHandle, ctrl_handle_result = %d"
#define MMK_MSG_911_112_2_18_3_22_42_12 "MMK_DispatchToHandle handle type is MMI_HANDLE_NONE!!!"
#define MMK_TIMER_192_112_2_18_3_22_43_13 "MMK_CreateTimer: timer_id = %d, s_timer_status = %x"
#define MMK_TIMER_212_112_2_18_3_22_43_14 "MMK_StartTimerCallback: timer_id = %d, time_out = %d, is_period = %d!"
#define MMK_TIMER_260_112_2_18_3_22_44_15 "MMK_StartTimer: owner_handle = 0x%x, timer_id = %d, time_out = %d, is_period = %d!"
#define MMK_TIMER_417_112_2_18_3_22_44_16 "StartAppTimer, timer_id = %d, timerIndex = %d, s_timer_status = %x"
#define MMK_TIMER_556_112_2_18_3_22_44_17 "CheckTimerSignal:the timer msg is too much! 0x%x,%d,%d"
#define MMK_TIMER_603_112_2_18_3_22_44_18 "MMK_TIMER.C:The timer(timerID = %d, ref_num = %d) index is error"
#define MMK_TIMER_696_112_2_18_3_22_44_19 "MMK_TIMER.C:The timer(timerID = %d, ref_num = %d) was stopped"
#define MMK_TIMER_726_112_2_18_3_22_45_20 "MMK_PauseTimer:SCI_DeactiveTimer fail!"
#define MMK_TIMER_731_112_2_18_3_22_45_21 "MMK_PauseTimer:index= %d,timer_ptr = 0x%x!"
#define MMK_TIMER_774_112_2_18_3_22_45_22 "MMK_ResumeTimer:remaining_time %d is error!"
#define MMK_TIMER_875_112_2_18_3_22_45_23 "AssignTimerIndex, status fail = %d"
#define MMK_TIMER_968_112_2_18_3_22_45_24 "MMK_DeleteTimerByHandle: timer_id=%d, owner=%d!"
#define MMK_TP_330_112_2_18_3_22_46_25 "MMK_DispatchMSGTp: x = %d, y = %d, gesture_type = %d, sig_code = %d"
#define MMK_TP_368_112_2_18_3_22_46_26 "MMK:MMK_DispatchMSGKTp tp down timer state %d, %d"
#define MMK_TP_690_112_2_18_3_22_47_27 "@DBLCLK, tick = %u."
#define MMK_TP_695_112_2_18_3_22_47_28 "@DBLCLK, s_tp_dblclk_state = %u, focus_win = 0x%x, s_tp_down_win = 0x%x, offset_x = %u, offset_y = %u n"
#define MMK_TP_701_112_2_18_3_22_47_29 "@DBLCLK, MSG_TP_PRESS_DOWN"
#define MMK_TP_711_112_2_18_3_22_47_30 "@DBLCLK, MSG_TP_PRESS_DOUBLE"
#define MMK_TP_718_112_2_18_3_22_47_31 "@DBLCLK, MSG_TP_PRESS_DOWN"
#define MMK_TP_807_112_2_18_3_22_47_32 "DefaultProcessTPMsg, down ctrl is not focus!"
#define MMK_TP_822_112_2_18_3_22_47_33 "DefaultProcessTPMsg, down ctrl is not exist!"
#define MMK_TP_881_112_2_18_3_22_47_34 "@DBLCLK, MSG_TP_PRESS_DBLCLK_UP"
#define MMK_TP_885_112_2_18_3_22_47_35 "@DBLCLK, MSG_TP_PRESS_UP"
#define MMK_TP_1308_112_2_18_3_22_48_36 "MMK_CheckTPSignale enter: state = %d, s_tp_prev_code = %d, s_tp_num = %d, s_tp_move_num = %d"
#define MMK_TP_1410_112_2_18_3_22_48_37 "CheckHandleTp: has key down,not handle tp down msg!"
#define MMK_TP_1494_112_2_18_3_22_48_38 "CheckTPPair failed"
#define UI_LAYER_737_112_2_18_3_22_51_39 "FindValidHandle() PNULL == layer_handle_ptr"
#define UI_LAYER_797_112_2_18_3_22_51_40 "UILAYER_SetLayerAlpha() PNULL == layer"
#define UI_LAYER_851_112_2_18_3_22_51_41 "UILAYER_WeakLayerAlpha() PNULL == layer || PNULL == layer->buf_ptr"
#define UI_LAYER_891_112_2_18_3_22_51_42 "UILAYER_SetLayerColorKey() PNULL == layer"
#define UI_LAYER_1033_112_2_18_3_22_51_43 "ui_layer.c CreateLayer() layer num is max!"
#define UI_LAYER_1066_112_2_18_3_22_51_44 "CreateHandle() PNULL == blend_ptr"
#define UI_LAYER_1071_112_2_18_3_22_51_45 "CreateHandle() PNULL == layer"
#define UI_LAYER_1335_112_2_18_3_22_52_46 "UILAYER_AppendBltLayer() level error!"
#define UI_LAYER_1340_112_2_18_3_22_52_47 "UILAYER_AppendBltLayer() max blt!"
#define UI_LAYER_1415_112_2_18_3_22_52_48 "StoreLayerUpdateRect() PNULL == layer"
#define UI_LAYER_1478_112_2_18_3_22_52_49 "UILAYER_InvalidateRect() PNULL == rect_ptr"
#define UI_LAYER_1521_112_2_18_3_22_52_50 "UILAYER_InvalidateRect() lcd_id = %d is error!"
#define UI_LAYER_1540_112_2_18_3_22_52_51 "GetLayerRect() PNULL == layer_ptr"
#define UI_LAYER_1581_112_2_18_3_22_52_52 "GetLayerBlendRect() PNULL == layer_blend_ptr"
#define UI_LAYER_1609_112_2_18_3_22_52_53 "ConvertLayer2LCDRect() (PNULL == layer_ptr) || (PNULL == lcd_rect_ptr) || (PNULL == layer_rect_ptr)"
#define UI_LAYER_1642_112_2_18_3_22_52_54 "ConvertLCD2LayerRect() (PNULL == layer_ptr) || (PNULL == layer_rect_ptr) || (PNULL == lcd_rect_ptr)"
#define UI_LAYER_1686_112_2_18_3_22_53_55 "UILAYER_BltLowerLayer() PNULL == rect_ptr"
#define UI_LAYER_1759_112_2_18_3_22_53_56 "UILAYER_SetMainLayerType() data_type = %d"
#define UI_LAYER_1792_112_2_18_3_22_53_57 "UILAYER_InitSystem() PNULL == mainlcd_buf_ptr"
#define UI_LAYER_1944_112_2_18_3_22_53_58 "UILAYER_GetMemCheckAddr() PNULL == layer"
#define UI_LAYER_1981_112_2_18_3_22_53_59 "UILAYER_GetLayerBufferPtr() PNULL == layer"
#define UI_LAYER_2070_112_2_18_3_22_53_60 "UILAYER_GetLayerPosition() UILAYER_GetLayerInfo() PNULL == dev_info_ptr"
#define UI_LAYER_2076_112_2_18_3_22_53_61 "UILAYER_GetLayerInfo() (PNULL == offset_x_ptr) || (PNULL == offset_y_ptr)"
#define UI_LAYER_2091_112_2_18_3_22_53_62 "UILAYER_GetLayerInfo() PNULL == layer"
#define UI_LAYER_2164_112_2_18_3_22_53_63 "UILAYER_SetLayerPosition() failed"
#define UI_LAYER_2275_112_2_18_3_22_54_64 "UILAYER_Clear() PNULL == layer || PNULL == layer->buf_ptr"
#define UI_LAYER_2319_112_2_18_3_22_54_65 "UILAYER_FillColor() PNULL == layer || PNULL == layer->buf_ptr"
#define UI_LAYER_2335_112_2_18_3_22_54_66 "AllocLayerBuf() 0 == buf_size"
#define UI_LAYER_2343_112_2_18_3_22_54_67 "AllocLayerBuf() PNULL == buf_ptr"
#define UI_LAYER_2375_112_2_18_3_22_54_68 "UILAYER_HandleWinGetFocus mmi_handle = %x"
#define UI_LAYER_2433_112_2_18_3_22_54_69 "UILAYER_HandleWinLoseFocus mmi_handle = %x"
#define UI_LAYER_2504_112_2_18_3_22_54_70 "UILAYER_HandleWinClose mmi_handle = %x"
#define UI_LAYER_2601_112_2_18_3_22_54_71 "ActiveLayer() PNULL == layer"
#define UI_LAYER_2619_112_2_18_3_22_54_72 "ActiveLayer again!"
#define UI_LAYER_2634_112_2_18_3_22_54_73 "DeactiveLayer() PNULL == layer"
#define UI_LAYER_2687_112_2_18_3_22_55_74 "GetLayerPtr() dev_info_ptr->block_id = %d > (GUI_BLOCK_MAX+UILAYER_TOTAL_LAYER_COUNT)"
#define UI_LAYER_2776_112_2_18_3_22_55_75 "UILAYER_GetHandleType() GUI_MAIN_LCD_ID != dev_info_ptr->lcd_id"
#define UI_LAYER_2781_112_2_18_3_22_55_76 "UILAYER_GetHandleType() PNULL == dev_info_ptr"
#define UI_LAYER_2850_112_2_18_3_22_55_77 "UILAYER_BltLowerLayerWhenTransparent() (PNULL == dev_info_ptr) || (PNULL == rect_ptr)"
#define UI_LAYER_2919_112_2_18_3_22_55_78 "IsRectTransparent() PNULL == rect_ptr"
#define UI_LAYER_3018_112_2_18_3_22_55_79 "dst_buf: 0x%x, (%d,%d,%d,%d)"
#define UI_LAYER_3032_112_2_18_3_22_55_80 "InvalidateRectUsingLayer() array_size==0!"
#define UI_LAYER_3310_112_2_18_3_22_56_81 "ui_layer.c UILAYER_Rotation() layer_ptr == PNULL!"
#define UI_LAYER_3345_112_2_18_3_22_56_82 "MMI_MemMoniterEnd fail!"
#define UI_LAYER_3419_112_2_18_3_22_56_83 "UILAYER_Clear() PNULL == layer || PNULL == layer->buf_ptr"
#define UI_LAYER_3896_112_2_18_3_22_57_84 "ui_layer.c BltBuffer() buf_size = %d, tick = %d"
#define UI_LAYER_3906_112_2_18_3_22_57_85 "BlendLayer() src_buf = 0x%x, buf_ptr = 0x%x!"
#define UI_LAYER_3972_112_2_18_3_22_57_86 "UILAYER_GetBltInfoForPrintScreen param error!"
#define UI_LAYER_4232_112_2_18_3_22_58_87 "UILAYER_GetBlendInfo() param error!"
#define UI_LAYER_4594_112_2_18_3_22_59_88 "UILAYER_SetLayerMemFormat() do nothing"
#define UI_LAYER_4621_112_2_18_3_22_59_89 "UILAYER_GetLayerMemFormat() do nothing"
#define UI_LAYER_4693_112_2_18_3_22_59_90 "UILAYER_SwitchDoubleMem() do nothing"
#define UI_LAYER_4808_112_2_18_3_22_59_91 "UILAYER_DoubleMemWriteBack() do nothing"
#define UI_LAYER_4889_112_2_18_3_22_59_92 "UILAYER_GetBltNum() param error!"
#define UI_LAYER_5057_112_2_18_3_23_0_93 "UILAYER_CopyLayerToBuffer() dev_info_ptr(%d,%d) not active!"
#define UI_LAYER_5097_112_2_18_3_23_0_94 "UILAYER_CopyLayerToBuffer() src_buf = 0x%x, dst_buf = 0x%x!"
#define UI_LAYER_5136_112_2_18_3_23_0_95 "UILAYER_CopyBufferToLayer() dev_info_ptr(%d,%d) not active!"
#define UI_LAYER_5192_112_2_18_3_23_0_96 "UILAYER_CopyBufferToLayer() src_buf = 0x%x, dst_buf = 0x%x!"
#define UI_LAYER_5339_112_2_18_3_23_0_97 "UILAYER_SetLayerBufferPtr() PNULL == layer"
#define UI_LAYER_5455_112_2_18_3_23_0_98 "UILAYER_MultiBlendLayer() layer_num=%d"
#define UI_LAYER_5609_112_2_18_3_23_1_99 "UILAYER_SetPreMultiplyAlpha() failed"
#define UI_LAYER_5688_112_2_18_3_23_1_100 "UILAYER_SetBlendCallback() failed"
#define UI_LAYER_SOFTBLT_266_112_2_18_3_23_1_101 "dst_rect & layer_rect not intersect! dst_rect = (%d, %d, %d, %d)"
#define UI_LAYER_SOFTBLT_267_112_2_18_3_23_1_102 "dst_rect & layer_rect not intersect! layer_rect = (%d, %d, %d, %d)"
#define UI_LAYER_SOFTBLT_275_112_2_18_3_23_2_103 "dst_rect & rect_ptr not intersect! dst_rect = (%d, %d, %d, %d)"
#define UI_LAYER_SOFTBLT_276_112_2_18_3_23_2_104 "dst_rect & rect_ptr not intersect! rect_ptr = (%d, %d, %d, %d)"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_KERNEL_TRC)
TRACE_MSG(MMK_KBD_174_112_2_18_3_22_38_0,"HandleMSGKbd result = %d")
TRACE_MSG(MMK_KBD_188_112_2_18_3_22_38_1,"HandleMSGKbd recode = %d")
TRACE_MSG(MMK_KBD_339_112_2_18_3_22_39_2,"MMK_DispatchMSGKbd: keystatus = %d, keycode = %d.")
TRACE_MSG(MMK_KBD_361_112_2_18_3_22_39_3,"@KPD_DOWN kbd_dblclk_state = %d.\n")
TRACE_MSG(MMK_KBD_419_112_2_18_3_22_39_4,"@KPD_UP kbd_dblclk_state = %d.\n")
TRACE_MSG(MMK_KBD_433_112_2_18_3_22_39_5,"MMK_DispatchMSGKbd signalCode = %d")
TRACE_MSG(MMK_KBD_569_112_2_18_3_22_39_6,"CheckHandleKey: has tp down,not handle key down msg!")
TRACE_MSG(MMK_KBD_1010_112_2_18_3_22_40_7,"CheckKeyMsgNumber:key full, waiting_keys_num = %d,keycode_num=%d, %0x, %0x, %0x!")
TRACE_MSG(MMK_MSG_413_112_2_18_3_22_41_8,"MMK_MSG.C mmk send msg handle is 0!!!")
TRACE_MSG(MMK_MSG_462_112_2_18_3_22_41_9,"MMK_MSG.C: AddMsgToQue the message queue is full. len = %d, readpos = %d, writepos = %d")
TRACE_MSG(MMK_MSG_579_112_2_18_3_22_42_10,"MMK_DispatchExtSig, send = %u")
TRACE_MSG(MMK_MSG_897_112_2_18_3_22_42_11,"MMK_DispatchToHandle, ctrl_handle_result = %d")
TRACE_MSG(MMK_MSG_911_112_2_18_3_22_42_12,"MMK_DispatchToHandle handle type is MMI_HANDLE_NONE!!!")
TRACE_MSG(MMK_TIMER_192_112_2_18_3_22_43_13,"MMK_CreateTimer: timer_id = %d, s_timer_status = %x")
TRACE_MSG(MMK_TIMER_212_112_2_18_3_22_43_14,"MMK_StartTimerCallback: timer_id = %d, time_out = %d, is_period = %d!")
TRACE_MSG(MMK_TIMER_260_112_2_18_3_22_44_15,"MMK_StartTimer: owner_handle = 0x%x, timer_id = %d, time_out = %d, is_period = %d!")
TRACE_MSG(MMK_TIMER_417_112_2_18_3_22_44_16,"StartAppTimer, timer_id = %d, timerIndex = %d, s_timer_status = %x")
TRACE_MSG(MMK_TIMER_556_112_2_18_3_22_44_17,"CheckTimerSignal:the timer msg is too much! 0x%x,%d,%d")
TRACE_MSG(MMK_TIMER_603_112_2_18_3_22_44_18,"MMK_TIMER.C:The timer(timerID = %d, ref_num = %d) index is error")
TRACE_MSG(MMK_TIMER_696_112_2_18_3_22_44_19,"MMK_TIMER.C:The timer(timerID = %d, ref_num = %d) was stopped")
TRACE_MSG(MMK_TIMER_726_112_2_18_3_22_45_20,"MMK_PauseTimer:SCI_DeactiveTimer fail!")
TRACE_MSG(MMK_TIMER_731_112_2_18_3_22_45_21,"MMK_PauseTimer:index= %d,timer_ptr = 0x%x!")
TRACE_MSG(MMK_TIMER_774_112_2_18_3_22_45_22,"MMK_ResumeTimer:remaining_time %d is error!")
TRACE_MSG(MMK_TIMER_875_112_2_18_3_22_45_23,"AssignTimerIndex, status fail = %d")
TRACE_MSG(MMK_TIMER_968_112_2_18_3_22_45_24,"MMK_DeleteTimerByHandle: timer_id=%d, owner=%d!")
TRACE_MSG(MMK_TP_330_112_2_18_3_22_46_25,"MMK_DispatchMSGTp: x = %d, y = %d, gesture_type = %d, sig_code = %d")
TRACE_MSG(MMK_TP_368_112_2_18_3_22_46_26,"MMK:MMK_DispatchMSGKTp tp down timer state %d, %d")
TRACE_MSG(MMK_TP_690_112_2_18_3_22_47_27,"@DBLCLK, tick = %u.")
TRACE_MSG(MMK_TP_695_112_2_18_3_22_47_28,"@DBLCLK, s_tp_dblclk_state = %u, focus_win = 0x%x, s_tp_down_win = 0x%x, offset_x = %u, offset_y = %u n")
TRACE_MSG(MMK_TP_701_112_2_18_3_22_47_29,"@DBLCLK, MSG_TP_PRESS_DOWN")
TRACE_MSG(MMK_TP_711_112_2_18_3_22_47_30,"@DBLCLK, MSG_TP_PRESS_DOUBLE")
TRACE_MSG(MMK_TP_718_112_2_18_3_22_47_31,"@DBLCLK, MSG_TP_PRESS_DOWN")
TRACE_MSG(MMK_TP_807_112_2_18_3_22_47_32,"DefaultProcessTPMsg, down ctrl is not focus!")
TRACE_MSG(MMK_TP_822_112_2_18_3_22_47_33,"DefaultProcessTPMsg, down ctrl is not exist!")
TRACE_MSG(MMK_TP_881_112_2_18_3_22_47_34,"@DBLCLK, MSG_TP_PRESS_DBLCLK_UP")
TRACE_MSG(MMK_TP_885_112_2_18_3_22_47_35,"@DBLCLK, MSG_TP_PRESS_UP")
TRACE_MSG(MMK_TP_1308_112_2_18_3_22_48_36,"MMK_CheckTPSignale enter: state = %d, s_tp_prev_code = %d, s_tp_num = %d, s_tp_move_num = %d")
TRACE_MSG(MMK_TP_1410_112_2_18_3_22_48_37,"CheckHandleTp: has key down,not handle tp down msg!")
TRACE_MSG(MMK_TP_1494_112_2_18_3_22_48_38,"CheckTPPair failed")
TRACE_MSG(UI_LAYER_737_112_2_18_3_22_51_39,"FindValidHandle() PNULL == layer_handle_ptr")
TRACE_MSG(UI_LAYER_797_112_2_18_3_22_51_40,"UILAYER_SetLayerAlpha() PNULL == layer")
TRACE_MSG(UI_LAYER_851_112_2_18_3_22_51_41,"UILAYER_WeakLayerAlpha() PNULL == layer || PNULL == layer->buf_ptr")
TRACE_MSG(UI_LAYER_891_112_2_18_3_22_51_42,"UILAYER_SetLayerColorKey() PNULL == layer")
TRACE_MSG(UI_LAYER_1033_112_2_18_3_22_51_43,"ui_layer.c CreateLayer() layer num is max!")
TRACE_MSG(UI_LAYER_1066_112_2_18_3_22_51_44,"CreateHandle() PNULL == blend_ptr")
TRACE_MSG(UI_LAYER_1071_112_2_18_3_22_51_45,"CreateHandle() PNULL == layer")
TRACE_MSG(UI_LAYER_1335_112_2_18_3_22_52_46,"UILAYER_AppendBltLayer() level error!")
TRACE_MSG(UI_LAYER_1340_112_2_18_3_22_52_47,"UILAYER_AppendBltLayer() max blt!")
TRACE_MSG(UI_LAYER_1415_112_2_18_3_22_52_48,"StoreLayerUpdateRect() PNULL == layer")
TRACE_MSG(UI_LAYER_1478_112_2_18_3_22_52_49,"UILAYER_InvalidateRect() PNULL == rect_ptr")
TRACE_MSG(UI_LAYER_1521_112_2_18_3_22_52_50,"UILAYER_InvalidateRect() lcd_id = %d is error!")
TRACE_MSG(UI_LAYER_1540_112_2_18_3_22_52_51,"GetLayerRect() PNULL == layer_ptr")
TRACE_MSG(UI_LAYER_1581_112_2_18_3_22_52_52,"GetLayerBlendRect() PNULL == layer_blend_ptr")
TRACE_MSG(UI_LAYER_1609_112_2_18_3_22_52_53,"ConvertLayer2LCDRect() (PNULL == layer_ptr) || (PNULL == lcd_rect_ptr) || (PNULL == layer_rect_ptr)")
TRACE_MSG(UI_LAYER_1642_112_2_18_3_22_52_54,"ConvertLCD2LayerRect() (PNULL == layer_ptr) || (PNULL == layer_rect_ptr) || (PNULL == lcd_rect_ptr)")
TRACE_MSG(UI_LAYER_1686_112_2_18_3_22_53_55,"UILAYER_BltLowerLayer() PNULL == rect_ptr")
TRACE_MSG(UI_LAYER_1759_112_2_18_3_22_53_56,"UILAYER_SetMainLayerType() data_type = %d")
TRACE_MSG(UI_LAYER_1792_112_2_18_3_22_53_57,"UILAYER_InitSystem() PNULL == mainlcd_buf_ptr")
TRACE_MSG(UI_LAYER_1944_112_2_18_3_22_53_58,"UILAYER_GetMemCheckAddr() PNULL == layer")
TRACE_MSG(UI_LAYER_1981_112_2_18_3_22_53_59,"UILAYER_GetLayerBufferPtr() PNULL == layer")
TRACE_MSG(UI_LAYER_2070_112_2_18_3_22_53_60,"UILAYER_GetLayerPosition() UILAYER_GetLayerInfo() PNULL == dev_info_ptr")
TRACE_MSG(UI_LAYER_2076_112_2_18_3_22_53_61,"UILAYER_GetLayerInfo() (PNULL == offset_x_ptr) || (PNULL == offset_y_ptr)")
TRACE_MSG(UI_LAYER_2091_112_2_18_3_22_53_62,"UILAYER_GetLayerInfo() PNULL == layer")
TRACE_MSG(UI_LAYER_2164_112_2_18_3_22_53_63,"UILAYER_SetLayerPosition() failed")
TRACE_MSG(UI_LAYER_2275_112_2_18_3_22_54_64,"UILAYER_Clear() PNULL == layer || PNULL == layer->buf_ptr")
TRACE_MSG(UI_LAYER_2319_112_2_18_3_22_54_65,"UILAYER_FillColor() PNULL == layer || PNULL == layer->buf_ptr")
TRACE_MSG(UI_LAYER_2335_112_2_18_3_22_54_66,"AllocLayerBuf() 0 == buf_size")
TRACE_MSG(UI_LAYER_2343_112_2_18_3_22_54_67,"AllocLayerBuf() PNULL == buf_ptr")
TRACE_MSG(UI_LAYER_2375_112_2_18_3_22_54_68,"UILAYER_HandleWinGetFocus mmi_handle = %x")
TRACE_MSG(UI_LAYER_2433_112_2_18_3_22_54_69,"UILAYER_HandleWinLoseFocus mmi_handle = %x")
TRACE_MSG(UI_LAYER_2504_112_2_18_3_22_54_70,"UILAYER_HandleWinClose mmi_handle = %x")
TRACE_MSG(UI_LAYER_2601_112_2_18_3_22_54_71,"ActiveLayer() PNULL == layer")
TRACE_MSG(UI_LAYER_2619_112_2_18_3_22_54_72,"ActiveLayer again!")
TRACE_MSG(UI_LAYER_2634_112_2_18_3_22_54_73,"DeactiveLayer() PNULL == layer")
TRACE_MSG(UI_LAYER_2687_112_2_18_3_22_55_74,"GetLayerPtr() dev_info_ptr->block_id = %d > (GUI_BLOCK_MAX+UILAYER_TOTAL_LAYER_COUNT)")
TRACE_MSG(UI_LAYER_2776_112_2_18_3_22_55_75,"UILAYER_GetHandleType() GUI_MAIN_LCD_ID != dev_info_ptr->lcd_id")
TRACE_MSG(UI_LAYER_2781_112_2_18_3_22_55_76,"UILAYER_GetHandleType() PNULL == dev_info_ptr")
TRACE_MSG(UI_LAYER_2850_112_2_18_3_22_55_77,"UILAYER_BltLowerLayerWhenTransparent() (PNULL == dev_info_ptr) || (PNULL == rect_ptr)")
TRACE_MSG(UI_LAYER_2919_112_2_18_3_22_55_78,"IsRectTransparent() PNULL == rect_ptr")
TRACE_MSG(UI_LAYER_3018_112_2_18_3_22_55_79,"dst_buf: 0x%x, (%d,%d,%d,%d)")
TRACE_MSG(UI_LAYER_3032_112_2_18_3_22_55_80,"InvalidateRectUsingLayer() array_size==0!")
TRACE_MSG(UI_LAYER_3310_112_2_18_3_22_56_81,"ui_layer.c UILAYER_Rotation() layer_ptr == PNULL!")
TRACE_MSG(UI_LAYER_3345_112_2_18_3_22_56_82,"MMI_MemMoniterEnd fail!")
TRACE_MSG(UI_LAYER_3419_112_2_18_3_22_56_83,"UILAYER_Clear() PNULL == layer || PNULL == layer->buf_ptr")
TRACE_MSG(UI_LAYER_3896_112_2_18_3_22_57_84,"ui_layer.c BltBuffer() buf_size = %d, tick = %d")
TRACE_MSG(UI_LAYER_3906_112_2_18_3_22_57_85,"BlendLayer() src_buf = 0x%x, buf_ptr = 0x%x!")
TRACE_MSG(UI_LAYER_3972_112_2_18_3_22_57_86,"UILAYER_GetBltInfoForPrintScreen param error!")
TRACE_MSG(UI_LAYER_4232_112_2_18_3_22_58_87,"UILAYER_GetBlendInfo() param error!")
TRACE_MSG(UI_LAYER_4594_112_2_18_3_22_59_88,"UILAYER_SetLayerMemFormat() do nothing")
TRACE_MSG(UI_LAYER_4621_112_2_18_3_22_59_89,"UILAYER_GetLayerMemFormat() do nothing")
TRACE_MSG(UI_LAYER_4693_112_2_18_3_22_59_90,"UILAYER_SwitchDoubleMem() do nothing")
TRACE_MSG(UI_LAYER_4808_112_2_18_3_22_59_91,"UILAYER_DoubleMemWriteBack() do nothing")
TRACE_MSG(UI_LAYER_4889_112_2_18_3_22_59_92,"UILAYER_GetBltNum() param error!")
TRACE_MSG(UI_LAYER_5057_112_2_18_3_23_0_93,"UILAYER_CopyLayerToBuffer() dev_info_ptr(%d,%d) not active!")
TRACE_MSG(UI_LAYER_5097_112_2_18_3_23_0_94,"UILAYER_CopyLayerToBuffer() src_buf = 0x%x, dst_buf = 0x%x!")
TRACE_MSG(UI_LAYER_5136_112_2_18_3_23_0_95,"UILAYER_CopyBufferToLayer() dev_info_ptr(%d,%d) not active!")
TRACE_MSG(UI_LAYER_5192_112_2_18_3_23_0_96,"UILAYER_CopyBufferToLayer() src_buf = 0x%x, dst_buf = 0x%x!")
TRACE_MSG(UI_LAYER_5339_112_2_18_3_23_0_97,"UILAYER_SetLayerBufferPtr() PNULL == layer")
TRACE_MSG(UI_LAYER_5455_112_2_18_3_23_0_98,"UILAYER_MultiBlendLayer() layer_num=%d")
TRACE_MSG(UI_LAYER_5609_112_2_18_3_23_1_99,"UILAYER_SetPreMultiplyAlpha() failed")
TRACE_MSG(UI_LAYER_5688_112_2_18_3_23_1_100,"UILAYER_SetBlendCallback() failed")
TRACE_MSG(UI_LAYER_SOFTBLT_266_112_2_18_3_23_1_101,"dst_rect & layer_rect not intersect! dst_rect = (%d, %d, %d, %d)")
TRACE_MSG(UI_LAYER_SOFTBLT_267_112_2_18_3_23_1_102,"dst_rect & layer_rect not intersect! layer_rect = (%d, %d, %d, %d)")
TRACE_MSG(UI_LAYER_SOFTBLT_275_112_2_18_3_23_2_103,"dst_rect & rect_ptr not intersect! dst_rect = (%d, %d, %d, %d)")
TRACE_MSG(UI_LAYER_SOFTBLT_276_112_2_18_3_23_2_104,"dst_rect & rect_ptr not intersect! rect_ptr = (%d, %d, %d, %d)")
END_TRACE_MAP(MMI_KERNEL_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_KERNEL_TRC_H_

