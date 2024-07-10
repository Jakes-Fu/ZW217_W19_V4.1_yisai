/******************************************************************************
 ** File Name:      mmi_kernel_sys_trc.h                                         *
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
//trace_id:258
#ifndef _MMI_KERNEL_SYS_TRC_H_
#define _MMI_KERNEL_SYS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMK_APP_706_112_2_18_3_23_5_0 "MMK_DAMP_PLAY_TYPE_E error type %d"
#define MMK_APPLET_227_112_2_18_3_23_5_1 "AppletCreateWindow failed, the same window id!"
#define MMK_APPLET_235_112_2_18_3_23_6_2 "AppletCreateWindow failed, no applet!"
#define MMK_APPLET_245_112_2_18_3_23_6_3 "AppletCreateWindow failed, parent win error!"
#define MMK_APPLET_273_112_2_18_3_23_6_4 "AppletCreateWindow failed, can't construct!"
#define MMK_APPLET_396_112_2_18_3_23_6_5 "MMK_CloseApplet failed"
#define MMK_APPLET_421_112_2_18_3_23_6_6 "MMK_StartApplet, guid = 0x%x"
#define MMK_APPLET_477_112_2_18_3_23_6_7 "MMK_StartApplet failed"
#define MMK_APPLET_625_112_2_18_3_23_6_8 "MMK_CloseWin failed!"
#define MMK_APPLET_654_112_2_18_3_23_6_9 "MMK_CloseAllWinByApplet failed!"
#define MMK_APPLET_685_112_2_18_3_23_6_10 "MMK_GetWinHandle failed!"
#define MMK_APPLET_711_112_2_18_3_23_7_11 "MMK_GetAppletPtr failed!"
#define MMK_APPLET_924_112_2_18_3_23_7_12 "MMK_RunAppletProc, applet_handle = 0x%x, guid = 0x%x, msg_id = 0x%x"
#define MMK_APPLET_930_112_2_18_3_23_7_13 "MMK_RunAppletProc failed"
#define MMK_APPLET_1041_112_2_18_3_23_7_14 "MMK_ReturnAppointedWin failed!"
#define MMK_APPLET_1074_112_2_18_3_23_7_15 "MMK_CloseMiddleWin: begin_win_id = end_win_id = %d!"
#define MMK_APPLET_1118_112_2_18_3_23_7_16 "MMK_CloseMiddleWin failed!"
#define MMK_APPLET_1160_112_2_18_3_23_7_17 "MMK_ProcSpecialAppletMsg failed"
#define MMK_APPLET_1344_112_2_18_3_23_8_18 "MMK_StartApplet, guid = 0x%x"
#define MMK_APPLET_1410_112_2_18_3_23_8_19 "MMK_CloseApplet failed"
#define MMK_MODINFO_1213_112_2_18_3_23_14_20 "XmlGetClassAttribs current type %d"
#define MMK_MODINFO_1616_112_2_18_3_23_15_21 "XmlParseCharactersSAX current tag %d"
#define MMK_MODULE_MANAGER_215_112_2_18_3_23_17_22 "[MOD MNG]: warring! the mmk manager memory has be destroy"
#define MMK_MODULE_MANAGER_316_112_2_18_3_23_17_23 "andrewzhang:code_ptr:%d\r\n"
#define MMK_MODULE_MANAGER_322_112_2_18_3_23_17_24 "andrewzhang:count:%d\r\n"
#define MMK_MODULE_MANAGER_331_112_2_18_3_23_17_25 "andrewzhang:offset2:%d\r\n"
#define MMK_MODULE_MANAGER_332_112_2_18_3_23_17_26 "andrewzhang:ro_size:%d\r\n"
#define MMK_MODULE_MANAGER_340_112_2_18_3_23_17_27 "andrewzhang:address:%d\r\n"
#define MMK_MODULE_MANAGER_448_112_2_18_3_23_18_28 "[MOD MNG]: error! module code size is 0x%x"
#define MMK_MODULE_MANAGER_456_112_2_18_3_23_18_29 "[MOD MNG]: error! module code no memory malloc"
#define MMK_SHELL_554_112_2_18_3_23_20_30 "[ISHELL_LoadResRing]: MMIRES_GetRingInfo  error! "
#define MMK_SHELL_560_112_2_18_3_23_20_31 "[ISHELL_LoadResRing]: ri.data_len = %d "
#define MMK_SHELL_571_112_2_18_3_23_20_32 "[ISHELL_LoadResRing]: IMEDIA_SetMediaData  error! "
#define MMK_SHELL_1106_112_2_18_3_23_21_33 "[MOD MNG]: error! module code size is 0x%x"
#define MMK_WINDOW_804_112_2_18_3_23_23_34 "james test, MMK_WindowTreeNodeConstruct start = %d"
#define MMK_WINDOW_869_112_2_18_3_23_24_35 "MMK_WindowTreeNodeDestruct: parent window closed in child win MSG_CLOSE_WINDOW msg"
#define MMK_WINDOW_883_112_2_18_3_23_24_36 "MMK_WindowTreeNodeDestruct: this window has been closed"
#define MMK_WINDOW_1069_112_2_18_3_23_24_37 "PostUpdateScreenMsg: don't open or close window when full paint!!!"
#define MMK_WINDOW_1177_112_2_18_3_23_24_38 "james test, MMK_UpdateScreen = %d"
#define MMK_WINDOW_1179_112_2_18_3_23_24_39 "james test, MMK_UpdateScreen end = %d"
#define MMK_WINDOW_1343_112_2_18_3_23_25_40 "WinTableCreate: parse failure"
#define MMK_WINDOW_1897_112_2_18_3_23_26_41 "MMK_OpenWin failed, window is not open !"
#define MMK_WINDOW_1917_112_2_18_3_23_26_42 "MMI: CreateWin, create window is fail!"
#define MMK_WINDOW_2198_112_2_18_3_23_27_43 "MMI: AddToZorderSystem failed!"
#define MMK_WINDOW_3154_112_2_18_3_23_29_44 "MMK_DefaultProcessWinMsg, down ctrl is not focus!"
#define MMK_WINDOW_3189_112_2_18_3_23_29_45 "MMK_DefaultProcessWinMsg failed"
#define MMK_WINDOW_3244_112_2_18_3_23_29_46 "MMK_ProcSpecialWinMsg MSG_GET_FOCUS failed"
#define MMK_WINDOW_3295_112_2_18_3_23_29_47 "MMK_ProcSpecialWinMsg failed"
#define MMK_WINDOW_3323_112_2_18_3_23_29_48 "james test, MMK_RunCtrlProc start = %d, guid = %d, msg_id = 0x%08x"
#define MMK_WINDOW_3363_112_2_18_3_23_29_49 "james ticks, MMK_RunCtrlProc end ticks = %d, guid = %d, msg_id = 0x%08x"
#define MMK_WINDOW_3369_112_2_18_3_23_29_50 "MMI: MMK_RunCtrlProc! the hwnd is not control!"
#define MMK_WINDOW_3518_112_2_18_3_23_29_51 "MMI:MMK_SetActiveCtrl, the control is active!"
#define MMK_WINDOW_3631_112_2_18_3_23_30_52 "MMK_ProcOpenWinMsg failed"
#define MMK_WINDOW_4248_112_2_18_3_23_31_53 "james test, MMK_RunWinProc start %d, win_id = 0x%08x, msg_id = 0x%08x"
#define MMK_WINDOW_4253_112_2_18_3_23_31_54 "MMK_RunWinProc, win_handle = 0x%08x, win_id = 0x%08x, win_name = %s, msg_id = 0x%08x"
#define MMK_WINDOW_4270_112_2_18_3_23_31_55 "james ticks, MMK_RunWinProc end ticks = %d, win_id = 0x%08x, msg_id = 0x%08x"
#define MMK_WINDOW_4275_112_2_18_3_23_31_56 "MMK_RunWinProc failed!"
#define MMK_WINDOW_4299_112_2_18_3_23_31_57 "james test, MMK_RunWinHookProc start %d, win_id = 0x%08x, msg_id = 0x%08x"
#define MMK_WINDOW_4301_112_2_18_3_23_31_58 "MMK_RunWinHookProc, win_handle = 0x%08x, win_id = 0x%08x, msg_id = 0x%08x"
#define MMK_WINDOW_4318_112_2_18_3_23_31_59 "james ticks, MMK_RunWinHookProc end ticks = %d, win_id = 0x%08x, msg_id = 0x%08x"
#define MMK_WINDOW_4496_112_2_18_3_23_31_60 "MMK_FreeWinAddData: the win_id = %d"
#define MMK_WINDOW_4549_112_2_18_3_23_31_61 "MMK_FreeWinAddDataEx: the win_id = %d"
#define MMK_WINDOW_4579_112_2_18_3_23_31_62 "MMK_WinGetActive: the win_handle = %d"
#define MMK_WINDOW_4617_112_2_18_3_23_32_63 "MMK_WinInactiveCtrl: the win_handle = %d"
#define MMK_WINDOW_4681_112_2_18_3_23_32_64 "MMI: MMK_GetPrevWinHandle failed!"
#define MMK_WINDOW_7725_112_2_18_3_23_38_65 "%sid=0x%08x, handle=0x%08x, ctrl_ptr=0x%08x, guid=%s, rect={%d,%d,%d,%d}"
#define MMK_WINDOW_7785_112_2_18_3_23_38_66 "==============   Dump window 0x%08x control tree   =============="
#define MMK_WINDOW_7789_112_2_18_3_23_38_67 "== style=0x%08x, active=0x%08x =="
#define MMK_WINDOW_7795_112_2_18_3_23_38_68 "==============================================================\n\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_KERNEL_SYS_TRC)
TRACE_MSG(MMK_APP_706_112_2_18_3_23_5_0,"MMK_DAMP_PLAY_TYPE_E error type %d")
TRACE_MSG(MMK_APPLET_227_112_2_18_3_23_5_1,"AppletCreateWindow failed, the same window id!")
TRACE_MSG(MMK_APPLET_235_112_2_18_3_23_6_2,"AppletCreateWindow failed, no applet!")
TRACE_MSG(MMK_APPLET_245_112_2_18_3_23_6_3,"AppletCreateWindow failed, parent win error!")
TRACE_MSG(MMK_APPLET_273_112_2_18_3_23_6_4,"AppletCreateWindow failed, can't construct!")
TRACE_MSG(MMK_APPLET_396_112_2_18_3_23_6_5,"MMK_CloseApplet failed")
TRACE_MSG(MMK_APPLET_421_112_2_18_3_23_6_6,"MMK_StartApplet, guid = 0x%x")
TRACE_MSG(MMK_APPLET_477_112_2_18_3_23_6_7,"MMK_StartApplet failed")
TRACE_MSG(MMK_APPLET_625_112_2_18_3_23_6_8,"MMK_CloseWin failed!")
TRACE_MSG(MMK_APPLET_654_112_2_18_3_23_6_9,"MMK_CloseAllWinByApplet failed!")
TRACE_MSG(MMK_APPLET_685_112_2_18_3_23_6_10,"MMK_GetWinHandle failed!")
TRACE_MSG(MMK_APPLET_711_112_2_18_3_23_7_11,"MMK_GetAppletPtr failed!")
TRACE_MSG(MMK_APPLET_924_112_2_18_3_23_7_12,"MMK_RunAppletProc, applet_handle = 0x%x, guid = 0x%x, msg_id = 0x%x")
TRACE_MSG(MMK_APPLET_930_112_2_18_3_23_7_13,"MMK_RunAppletProc failed")
TRACE_MSG(MMK_APPLET_1041_112_2_18_3_23_7_14,"MMK_ReturnAppointedWin failed!")
TRACE_MSG(MMK_APPLET_1074_112_2_18_3_23_7_15,"MMK_CloseMiddleWin: begin_win_id = end_win_id = %d!")
TRACE_MSG(MMK_APPLET_1118_112_2_18_3_23_7_16,"MMK_CloseMiddleWin failed!")
TRACE_MSG(MMK_APPLET_1160_112_2_18_3_23_7_17,"MMK_ProcSpecialAppletMsg failed")
TRACE_MSG(MMK_APPLET_1344_112_2_18_3_23_8_18,"MMK_StartApplet, guid = 0x%x")
TRACE_MSG(MMK_APPLET_1410_112_2_18_3_23_8_19,"MMK_CloseApplet failed")
TRACE_MSG(MMK_MODINFO_1213_112_2_18_3_23_14_20,"XmlGetClassAttribs current type %d")
TRACE_MSG(MMK_MODINFO_1616_112_2_18_3_23_15_21,"XmlParseCharactersSAX current tag %d")
TRACE_MSG(MMK_MODULE_MANAGER_215_112_2_18_3_23_17_22,"[MOD MNG]: warring! the mmk manager memory has be destroy")
TRACE_MSG(MMK_MODULE_MANAGER_316_112_2_18_3_23_17_23,"andrewzhang:code_ptr:%d\r\n")
TRACE_MSG(MMK_MODULE_MANAGER_322_112_2_18_3_23_17_24,"andrewzhang:count:%d\r\n")
TRACE_MSG(MMK_MODULE_MANAGER_331_112_2_18_3_23_17_25,"andrewzhang:offset2:%d\r\n")
TRACE_MSG(MMK_MODULE_MANAGER_332_112_2_18_3_23_17_26,"andrewzhang:ro_size:%d\r\n")
TRACE_MSG(MMK_MODULE_MANAGER_340_112_2_18_3_23_17_27,"andrewzhang:address:%d\r\n")
TRACE_MSG(MMK_MODULE_MANAGER_448_112_2_18_3_23_18_28,"[MOD MNG]: error! module code size is 0x%x")
TRACE_MSG(MMK_MODULE_MANAGER_456_112_2_18_3_23_18_29,"[MOD MNG]: error! module code no memory malloc")
TRACE_MSG(MMK_SHELL_554_112_2_18_3_23_20_30,"[ISHELL_LoadResRing]: MMIRES_GetRingInfo  error! ")
TRACE_MSG(MMK_SHELL_560_112_2_18_3_23_20_31,"[ISHELL_LoadResRing]: ri.data_len = %d ")
TRACE_MSG(MMK_SHELL_571_112_2_18_3_23_20_32,"[ISHELL_LoadResRing]: IMEDIA_SetMediaData  error! ")
TRACE_MSG(MMK_SHELL_1106_112_2_18_3_23_21_33,"[MOD MNG]: error! module code size is 0x%x")
TRACE_MSG(MMK_WINDOW_804_112_2_18_3_23_23_34,"james test, MMK_WindowTreeNodeConstruct start = %d")
TRACE_MSG(MMK_WINDOW_869_112_2_18_3_23_24_35,"MMK_WindowTreeNodeDestruct: parent window closed in child win MSG_CLOSE_WINDOW msg")
TRACE_MSG(MMK_WINDOW_883_112_2_18_3_23_24_36,"MMK_WindowTreeNodeDestruct: this window has been closed")
TRACE_MSG(MMK_WINDOW_1069_112_2_18_3_23_24_37,"PostUpdateScreenMsg: don't open or close window when full paint!!!")
TRACE_MSG(MMK_WINDOW_1177_112_2_18_3_23_24_38,"james test, MMK_UpdateScreen = %d")
TRACE_MSG(MMK_WINDOW_1179_112_2_18_3_23_24_39,"james test, MMK_UpdateScreen end = %d")
TRACE_MSG(MMK_WINDOW_1343_112_2_18_3_23_25_40,"WinTableCreate: parse failure")
TRACE_MSG(MMK_WINDOW_1897_112_2_18_3_23_26_41,"MMK_OpenWin failed, window is not open !")
TRACE_MSG(MMK_WINDOW_1917_112_2_18_3_23_26_42,"MMI: CreateWin, create window is fail!")
TRACE_MSG(MMK_WINDOW_2198_112_2_18_3_23_27_43,"MMI: AddToZorderSystem failed!")
TRACE_MSG(MMK_WINDOW_3154_112_2_18_3_23_29_44,"MMK_DefaultProcessWinMsg, down ctrl is not focus!")
TRACE_MSG(MMK_WINDOW_3189_112_2_18_3_23_29_45,"MMK_DefaultProcessWinMsg failed")
TRACE_MSG(MMK_WINDOW_3244_112_2_18_3_23_29_46,"MMK_ProcSpecialWinMsg MSG_GET_FOCUS failed")
TRACE_MSG(MMK_WINDOW_3295_112_2_18_3_23_29_47,"MMK_ProcSpecialWinMsg failed")
TRACE_MSG(MMK_WINDOW_3323_112_2_18_3_23_29_48,"james test, MMK_RunCtrlProc start = %d, guid = %d, msg_id = 0x%08x")
TRACE_MSG(MMK_WINDOW_3363_112_2_18_3_23_29_49,"james ticks, MMK_RunCtrlProc end ticks = %d, guid = %d, msg_id = 0x%08x")
TRACE_MSG(MMK_WINDOW_3369_112_2_18_3_23_29_50,"MMI: MMK_RunCtrlProc! the hwnd is not control!")
TRACE_MSG(MMK_WINDOW_3518_112_2_18_3_23_29_51,"MMI:MMK_SetActiveCtrl, the control is active!")
TRACE_MSG(MMK_WINDOW_3631_112_2_18_3_23_30_52,"MMK_ProcOpenWinMsg failed")
TRACE_MSG(MMK_WINDOW_4248_112_2_18_3_23_31_53,"james test, MMK_RunWinProc start %d, win_id = 0x%08x, msg_id = 0x%08x")
TRACE_MSG(MMK_WINDOW_4253_112_2_18_3_23_31_54,"MMK_RunWinProc, win_handle = 0x%08x, win_id = 0x%08x, win_name = %s, msg_id = 0x%08x")
TRACE_MSG(MMK_WINDOW_4270_112_2_18_3_23_31_55,"james ticks, MMK_RunWinProc end ticks = %d, win_id = 0x%08x, msg_id = 0x%08x")
TRACE_MSG(MMK_WINDOW_4275_112_2_18_3_23_31_56,"MMK_RunWinProc failed!")
TRACE_MSG(MMK_WINDOW_4299_112_2_18_3_23_31_57,"james test, MMK_RunWinHookProc start %d, win_id = 0x%08x, msg_id = 0x%08x")
TRACE_MSG(MMK_WINDOW_4301_112_2_18_3_23_31_58,"MMK_RunWinHookProc, win_handle = 0x%08x, win_id = 0x%08x, msg_id = 0x%08x")
TRACE_MSG(MMK_WINDOW_4318_112_2_18_3_23_31_59,"james ticks, MMK_RunWinHookProc end ticks = %d, win_id = 0x%08x, msg_id = 0x%08x")
TRACE_MSG(MMK_WINDOW_4496_112_2_18_3_23_31_60,"MMK_FreeWinAddData: the win_id = %d")
TRACE_MSG(MMK_WINDOW_4549_112_2_18_3_23_31_61,"MMK_FreeWinAddDataEx: the win_id = %d")
TRACE_MSG(MMK_WINDOW_4579_112_2_18_3_23_31_62,"MMK_WinGetActive: the win_handle = %d")
TRACE_MSG(MMK_WINDOW_4617_112_2_18_3_23_32_63,"MMK_WinInactiveCtrl: the win_handle = %d")
TRACE_MSG(MMK_WINDOW_4681_112_2_18_3_23_32_64,"MMI: MMK_GetPrevWinHandle failed!")
TRACE_MSG(MMK_WINDOW_7725_112_2_18_3_23_38_65,"%sid=0x%08x, handle=0x%08x, ctrl_ptr=0x%08x, guid=%s, rect={%d,%d,%d,%d}")
TRACE_MSG(MMK_WINDOW_7785_112_2_18_3_23_38_66,"==============   Dump window 0x%08x control tree   ==============")
TRACE_MSG(MMK_WINDOW_7789_112_2_18_3_23_38_67,"== style=0x%08x, active=0x%08x ==")
TRACE_MSG(MMK_WINDOW_7795_112_2_18_3_23_38_68,"==============================================================\n\n")
END_TRACE_MAP(MMI_KERNEL_SYS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_KERNEL_SYS_TRC_H_

