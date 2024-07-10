/******************************************************************************
 ** File Name:      mmi_app_uzone_trc.h                                         *
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
//trace_id:233
#ifndef _MMI_APP_UZONE_TRC_H_
#define _MMI_APP_UZONE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIUZONE_209_112_2_18_3_4_20_0 "[MMIUZONE] UzoneMp3Cb"
#define MMIUZONE_223_112_2_18_3_4_20_1 "[MMIUZONE] UzoneFMCb"
#define MMIUZONE_237_112_2_18_3_4_20_2 "[MMIUZONE] UzoneImageViewerCb"
#define MMIUZONE_249_112_2_18_3_4_20_3 "[MMIUZONE] UzoneVPCb"
#define MMIUZONE_263_112_2_18_3_4_20_4 "[MMIUZONE] UzoneATVCb"
#define MMIUZONE_277_112_2_18_3_4_20_5 "[MMIUZONE] UzoneCameraCb"
#define MMIUZONE_291_112_2_18_3_4_20_6 "[MMIUZONE] UzoneRecordCb"
#define MMIUZONE_303_112_2_18_3_4_20_7 "[MMIUZONE] UzoneKMCb"
#define MMIUZONE_317_112_2_18_3_4_20_8 "[MMIUZONE] UzoneEbookCb"
#define MMIUZONE_485_112_2_18_3_4_20_9 "[MMIUZONE]MMIUZONE_ReleaseLayers"
#define MMIUZONE_510_112_2_18_3_4_20_10 "[MMIUZONE]MMIUZONE_CreateLayers"
#define MMIUZONE_514_112_2_18_3_4_20_11 "[MMIUZONE]MMIUZONE_CreateLayers - Invalid param"
#define MMIUZONE_541_112_2_18_3_4_20_12 "[MMIUZONE]MMIUZONE_CreateLayers - create bg layer fail"
#define MMIUZONE_551_112_2_18_3_4_20_13 "[MMIUZONE]MMIUZONE_CreateLayers - create fg layer fail"
#define MMIUZONE_576_112_2_18_3_4_20_14 "[MMIUZONE]MMIUZONE_CreateLayers - create src layer fail"
#define MMIUZONE_596_112_2_18_3_4_20_15 "[MMIUZONE] MMIUZONE_Init enter!"
#define MMIUZONE_600_112_2_18_3_4_20_16 "[MMIUZONE] MMIUZONE_Init return FALSE for invalid buffer"
#define MMIUZONE_620_112_2_18_3_4_20_17 "[MMIUZONE] MMIUZONE_Init - create layer fail"
#define MMIUZONE_636_112_2_18_3_4_20_18 "[MMIUZONE] MMIUZONE_Destroy enter!"
#define MMIUZONE_656_112_2_18_3_4_21_19 "[MMIUZONE] MMIUZONE_Create enter!"
#define MMIUZONE_668_112_2_18_3_4_21_20 "[MMIUZONE] create uzone fail! Can't malloc memory (size: %d)!"
#define MMIUZONE_677_112_2_18_3_4_21_21 "[MMIUZONE] create uzone fail! Can't malloc memory (size: %d)!"
#define MMIUZONE_697_112_2_18_3_4_21_22 "[MMIUZONE] MMIUZONE_ItemAction (%d)"
#define MMIUZONE_702_112_2_18_3_4_21_23 "[MMIUZONE] MMIUZONE_ItemAction - Invalid param"
#define MMIUZONE_DISPLAY_108_112_2_18_3_4_21_24 "[MMIUZONE] FillHSrcLayer - running info is PNULL!"
#define MMIUZONE_DISPLAY_116_112_2_18_3_4_21_25 "[MMIUZONE] FillBgLayer - bg layer is not active!"
#define MMIUZONE_DISPLAY_142_112_2_18_3_4_21_26 "[MMIUZONE] FillBgLayer - draw bg fail!"
#define MMIUZONE_DISPLAY_153_112_2_18_3_4_21_27 "[MMIUZONE] FillBgLayer - draw light fail!"
#define MMIUZONE_DISPLAY_180_112_2_18_3_4_21_28 "[MMIUZONE] FillItem - Invalid param!"
#define MMIUZONE_DISPLAY_193_112_2_18_3_4_21_29 "[MMIUZONE] FillItem(%d) - draw icon bg fail!"
#define MMIUZONE_DISPLAY_206_112_2_18_3_4_21_30 "[MMIUZONE] FillItem(%d) - draw icon fail!"
#define MMIUZONE_DISPLAY_222_112_2_18_3_4_21_31 "[MMIUZONE] FillItem(%d) - draw string bg fail!"
#define MMIUZONE_DISPLAY_258_112_2_18_3_4_21_32 "[MMIUZONE] FillHSrcLayer - running info is PNULL!"
#define MMIUZONE_DISPLAY_277_112_2_18_3_4_21_33 "[MMIUZONE] FillHSrcLayer: create temp layer fail!"
#define MMIUZONE_DISPLAY_299_112_2_18_3_4_21_34 "[MMIUZONE] FillHSrcLayer: file item %d fail!"
#define MMIUZONE_DISPLAY_336_112_2_18_3_4_22_35 "[MMIUZONE] FillVSrcLayer - running info is PNULL!"
#define MMIUZONE_DISPLAY_363_112_2_18_3_4_22_36 "[MMIUZONE] FillVSrcLayer: file item %d fail!"
#define MMIUZONE_DISPLAY_383_112_2_18_3_4_22_37 "[MMIUZONE] FillSrcLayer - running info is PNULL!"
#define MMIUZONE_DISPLAY_389_112_2_18_3_4_22_38 "[MMIUZONE] FillSrcLayer - src layer is not active!"
#define MMIUZONE_DISPLAY_396_112_2_18_3_4_22_39 "[MMIUZONE] FillSrcLayer - fill h src layer fail!"
#define MMIUZONE_DISPLAY_401_112_2_18_3_4_22_40 "[MMIUZONE] FillSrcLayer - fill v src layer fail!"
#define MMIUZONE_DISPLAY_417_112_2_18_3_4_22_41 "[MMIUZONE] MMIUZONE_FillLayers - fill bg layer fail!"
#define MMIUZONE_DISPLAY_423_112_2_18_3_4_22_42 "[MMIUZONE] MMIUZONE_FillLayers - fill src layer fail!"
#define MMIUZONE_DISPLAY_442_112_2_18_3_4_22_43 "[MMIUZONE] MMIUZONE_DrawWindow - running info is PNULL!"
#define MMIUZONE_DISPLAY_468_112_2_18_3_4_22_44 "[MMIUZONE] MMIUZONE_ExistDraw - running info is PNULL!"
#define MMIUZONE_EXPORT_104_112_2_18_3_4_22_45 "[MMIUZONE] MMIAPIUZONE_Init - current itme = %d"
#define MMIUZONE_EXPORT_118_112_2_18_3_4_22_46 "[MMIUZONE] MMIAPIUZONE_ResetFactorySettings - current itme = %d"
#define MMIUZONE_SE_305_112_2_18_3_4_24_47 "SEAPI_SetItemCallback failed!"
#define MMIUZONE_SE_491_112_2_18_3_4_24_48 "[MMIUZONE] SE_DrawOneFrame direction = %d,start_location = %d,cur_frame_num = %d"
#define MMIUZONE_SE_1129_112_2_18_3_4_25_49 "[MMIUZONE] SE_HandleTPUpMsg multi_se_info->fling_recover_start_frame = %d"
#define MMIUZONE_SE_1185_112_2_18_3_4_25_50 "[MMIUZONE] SE_HandleTPUpMsg fling_velocity = %d"
#define MMIUZONE_SE_1223_112_2_18_3_4_26_51 "[MMIUZONE] SE_HandleTPUpMsg index = %d"
#define MMIUZONE_SE_1359_112_2_18_3_4_26_52 "[MMIUZONE] SE_HandleWinTimerMsg param = %d"
#define MMIUZONE_SE_1414_112_2_18_3_4_26_53 "[MMIUZONE] SE_HandleWinTimerMsg MSG_ID = %d"
#define MMIUZONE_SE_1481_112_2_18_3_4_26_54 "[MMIUZONE] SEAPI_HandleMsg msg_id = 0x%04x"
#define MMIUZONE_WINTAB_209_112_2_18_3_4_27_55 "[MMIUZONE] MMIUZONE_Entry"
#define MMIUZONE_WINTAB_214_112_2_18_3_4_27_56 "[MMIUZONE] MMIUZONE_Entry return fail for error environment!"
#define MMIUZONE_WINTAB_273_112_2_18_3_4_27_57 "[MMIUZONE] StartUzoneApplet ret = %d,applet_ptr=0x%lX"
#define MMIUZONE_WINTAB_293_112_2_18_3_4_27_58 "[MMIUZONE] CloseUzoneApplet applet_ptr=0x%lX"
#define MMIUZONE_WINTAB_304_112_2_18_3_4_27_59 "[MMIUZONE] SaveNV (%d)"
#define MMIUZONE_WINTAB_318_112_2_18_3_4_27_60 "[MMIUZONE] InitUzoneSE - Invalid param"
#define MMIUZONE_WINTAB_322_112_2_18_3_4_27_61 "[MMIUZONE] InitUzoneSe- current item = %d"
#define MMIUZONE_WINTAB_337_112_2_18_3_4_27_62 "[MMIUZONE] HandleOpenWindow"
#define MMIUZONE_WINTAB_341_112_2_18_3_4_27_63 "[MMIUZONE] HandleOpenWindow - MMIUZONE_Init fail"
#define MMIUZONE_WINTAB_347_112_2_18_3_4_27_64 "[MMIUZONE] HandleOpenWindow - MMIUZONE_FillLayers fail"
#define MMIUZONE_WINTAB_366_112_2_18_3_4_27_65 "[MMIUZONE] HandleGetFocus - Invalid param"
#define MMIUZONE_WINTAB_373_112_2_18_3_4_27_66 "[MMIUZONE] HandleGetFocusWindow- current item = %d, landsape (%d)"
#define MMIUZONE_WINTAB_377_112_2_18_3_4_27_67 "[MMIUZONE] HandleGetFocus - MMIUZONE_CreateLayers fail"
#define MMIUZONE_WINTAB_383_112_2_18_3_4_27_68 "[MMIUZONE] HandleGetFocus - MMIUZONE_FillLayers fail"
#define MMIUZONE_WINTAB_399_112_2_18_3_4_27_69 "[MMIUZONE] HandleLoseFocusWindow- current item = %d"
#define MMIUZONE_WINTAB_414_112_2_18_3_4_27_70 "[MMIUZONE] HandleLCDSwitch - Invalid param"
#define MMIUZONE_WINTAB_422_112_2_18_3_4_27_71 "[MMIUZONE] HandleLCDSwitch - landscape is (%d), current item = %d"
#define MMIUZONE_WINTAB_431_112_2_18_3_4_27_72 "[MMIUZONE] HandleLCDSwitch - MMIUZONE_CreateLayers fail"
#define MMIUZONE_WINTAB_437_112_2_18_3_4_27_73 "[MMIUZONE] HandleLCDSwitch - MMIUZONE_FillLayers fail"
#define MMIUZONE_WINTAB_461_112_2_18_3_4_27_74 "[MMIUZONE] HandleFullPaint - Invalid param"
#define MMIUZONE_WINTAB_467_112_2_18_3_4_27_75 "[MMIUZONE] HandleFullPaint -is not focus window"
#define MMIUZONE_WINTAB_471_112_2_18_3_4_27_76 "[MMIUZONE] HandleFullPaint - MMIUZONE_CreateLayers fail"
#define MMIUZONE_WINTAB_477_112_2_18_3_4_27_77 "[MMIUZONE] HandleFullPaint - MMIUZONE_FillLayers fail"
#define MMIUZONE_WINTAB_493_112_2_18_3_4_28_78 "[MMIUZONE] HandleFullPaint -is focus window"
#define MMIUZONE_WINTAB_510_112_2_18_3_4_28_79 "[MMIUZONE] HandleFullPaintFinish - Invalid param"
#define MMIUZONE_WINTAB_516_112_2_18_3_4_28_80 "[MMIUZONE] HandleFullPaintFinish -is not focus window"
#define MMIUZONE_WINTAB_536_112_2_18_3_4_28_81 "[MMIUZONE] UzoneApplet_HandleEvent msg_id = 0x%04x"
#define MMIUZONE_WINTAB_546_112_2_18_3_4_28_82 "[MMIUZONE] MMIUZONE_Create return fail!"
#define MMIUZONE_WINTAB_584_112_2_18_3_4_28_83 "[MMIUZONE] HandleUzoneMainWinMsg msg_id = 0x%04x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_UZONE_TRC)
TRACE_MSG(MMIUZONE_209_112_2_18_3_4_20_0,"[MMIUZONE] UzoneMp3Cb")
TRACE_MSG(MMIUZONE_223_112_2_18_3_4_20_1,"[MMIUZONE] UzoneFMCb")
TRACE_MSG(MMIUZONE_237_112_2_18_3_4_20_2,"[MMIUZONE] UzoneImageViewerCb")
TRACE_MSG(MMIUZONE_249_112_2_18_3_4_20_3,"[MMIUZONE] UzoneVPCb")
TRACE_MSG(MMIUZONE_263_112_2_18_3_4_20_4,"[MMIUZONE] UzoneATVCb")
TRACE_MSG(MMIUZONE_277_112_2_18_3_4_20_5,"[MMIUZONE] UzoneCameraCb")
TRACE_MSG(MMIUZONE_291_112_2_18_3_4_20_6,"[MMIUZONE] UzoneRecordCb")
TRACE_MSG(MMIUZONE_303_112_2_18_3_4_20_7,"[MMIUZONE] UzoneKMCb")
TRACE_MSG(MMIUZONE_317_112_2_18_3_4_20_8,"[MMIUZONE] UzoneEbookCb")
TRACE_MSG(MMIUZONE_485_112_2_18_3_4_20_9,"[MMIUZONE]MMIUZONE_ReleaseLayers")
TRACE_MSG(MMIUZONE_510_112_2_18_3_4_20_10,"[MMIUZONE]MMIUZONE_CreateLayers")
TRACE_MSG(MMIUZONE_514_112_2_18_3_4_20_11,"[MMIUZONE]MMIUZONE_CreateLayers - Invalid param")
TRACE_MSG(MMIUZONE_541_112_2_18_3_4_20_12,"[MMIUZONE]MMIUZONE_CreateLayers - create bg layer fail")
TRACE_MSG(MMIUZONE_551_112_2_18_3_4_20_13,"[MMIUZONE]MMIUZONE_CreateLayers - create fg layer fail")
TRACE_MSG(MMIUZONE_576_112_2_18_3_4_20_14,"[MMIUZONE]MMIUZONE_CreateLayers - create src layer fail")
TRACE_MSG(MMIUZONE_596_112_2_18_3_4_20_15,"[MMIUZONE] MMIUZONE_Init enter!")
TRACE_MSG(MMIUZONE_600_112_2_18_3_4_20_16,"[MMIUZONE] MMIUZONE_Init return FALSE for invalid buffer")
TRACE_MSG(MMIUZONE_620_112_2_18_3_4_20_17,"[MMIUZONE] MMIUZONE_Init - create layer fail")
TRACE_MSG(MMIUZONE_636_112_2_18_3_4_20_18,"[MMIUZONE] MMIUZONE_Destroy enter!")
TRACE_MSG(MMIUZONE_656_112_2_18_3_4_21_19,"[MMIUZONE] MMIUZONE_Create enter!")
TRACE_MSG(MMIUZONE_668_112_2_18_3_4_21_20,"[MMIUZONE] create uzone fail! Can't malloc memory (size: %d)!")
TRACE_MSG(MMIUZONE_677_112_2_18_3_4_21_21,"[MMIUZONE] create uzone fail! Can't malloc memory (size: %d)!")
TRACE_MSG(MMIUZONE_697_112_2_18_3_4_21_22,"[MMIUZONE] MMIUZONE_ItemAction (%d)")
TRACE_MSG(MMIUZONE_702_112_2_18_3_4_21_23,"[MMIUZONE] MMIUZONE_ItemAction - Invalid param")
TRACE_MSG(MMIUZONE_DISPLAY_108_112_2_18_3_4_21_24,"[MMIUZONE] FillHSrcLayer - running info is PNULL!")
TRACE_MSG(MMIUZONE_DISPLAY_116_112_2_18_3_4_21_25,"[MMIUZONE] FillBgLayer - bg layer is not active!")
TRACE_MSG(MMIUZONE_DISPLAY_142_112_2_18_3_4_21_26,"[MMIUZONE] FillBgLayer - draw bg fail!")
TRACE_MSG(MMIUZONE_DISPLAY_153_112_2_18_3_4_21_27,"[MMIUZONE] FillBgLayer - draw light fail!")
TRACE_MSG(MMIUZONE_DISPLAY_180_112_2_18_3_4_21_28,"[MMIUZONE] FillItem - Invalid param!")
TRACE_MSG(MMIUZONE_DISPLAY_193_112_2_18_3_4_21_29,"[MMIUZONE] FillItem(%d) - draw icon bg fail!")
TRACE_MSG(MMIUZONE_DISPLAY_206_112_2_18_3_4_21_30,"[MMIUZONE] FillItem(%d) - draw icon fail!")
TRACE_MSG(MMIUZONE_DISPLAY_222_112_2_18_3_4_21_31,"[MMIUZONE] FillItem(%d) - draw string bg fail!")
TRACE_MSG(MMIUZONE_DISPLAY_258_112_2_18_3_4_21_32,"[MMIUZONE] FillHSrcLayer - running info is PNULL!")
TRACE_MSG(MMIUZONE_DISPLAY_277_112_2_18_3_4_21_33,"[MMIUZONE] FillHSrcLayer: create temp layer fail!")
TRACE_MSG(MMIUZONE_DISPLAY_299_112_2_18_3_4_21_34,"[MMIUZONE] FillHSrcLayer: file item %d fail!")
TRACE_MSG(MMIUZONE_DISPLAY_336_112_2_18_3_4_22_35,"[MMIUZONE] FillVSrcLayer - running info is PNULL!")
TRACE_MSG(MMIUZONE_DISPLAY_363_112_2_18_3_4_22_36,"[MMIUZONE] FillVSrcLayer: file item %d fail!")
TRACE_MSG(MMIUZONE_DISPLAY_383_112_2_18_3_4_22_37,"[MMIUZONE] FillSrcLayer - running info is PNULL!")
TRACE_MSG(MMIUZONE_DISPLAY_389_112_2_18_3_4_22_38,"[MMIUZONE] FillSrcLayer - src layer is not active!")
TRACE_MSG(MMIUZONE_DISPLAY_396_112_2_18_3_4_22_39,"[MMIUZONE] FillSrcLayer - fill h src layer fail!")
TRACE_MSG(MMIUZONE_DISPLAY_401_112_2_18_3_4_22_40,"[MMIUZONE] FillSrcLayer - fill v src layer fail!")
TRACE_MSG(MMIUZONE_DISPLAY_417_112_2_18_3_4_22_41,"[MMIUZONE] MMIUZONE_FillLayers - fill bg layer fail!")
TRACE_MSG(MMIUZONE_DISPLAY_423_112_2_18_3_4_22_42,"[MMIUZONE] MMIUZONE_FillLayers - fill src layer fail!")
TRACE_MSG(MMIUZONE_DISPLAY_442_112_2_18_3_4_22_43,"[MMIUZONE] MMIUZONE_DrawWindow - running info is PNULL!")
TRACE_MSG(MMIUZONE_DISPLAY_468_112_2_18_3_4_22_44,"[MMIUZONE] MMIUZONE_ExistDraw - running info is PNULL!")
TRACE_MSG(MMIUZONE_EXPORT_104_112_2_18_3_4_22_45,"[MMIUZONE] MMIAPIUZONE_Init - current itme = %d")
TRACE_MSG(MMIUZONE_EXPORT_118_112_2_18_3_4_22_46,"[MMIUZONE] MMIAPIUZONE_ResetFactorySettings - current itme = %d")
TRACE_MSG(MMIUZONE_SE_305_112_2_18_3_4_24_47,"SEAPI_SetItemCallback failed!")
TRACE_MSG(MMIUZONE_SE_491_112_2_18_3_4_24_48,"[MMIUZONE] SE_DrawOneFrame direction = %d,start_location = %d,cur_frame_num = %d")
TRACE_MSG(MMIUZONE_SE_1129_112_2_18_3_4_25_49,"[MMIUZONE] SE_HandleTPUpMsg multi_se_info->fling_recover_start_frame = %d")
TRACE_MSG(MMIUZONE_SE_1185_112_2_18_3_4_25_50,"[MMIUZONE] SE_HandleTPUpMsg fling_velocity = %d")
TRACE_MSG(MMIUZONE_SE_1223_112_2_18_3_4_26_51,"[MMIUZONE] SE_HandleTPUpMsg index = %d")
TRACE_MSG(MMIUZONE_SE_1359_112_2_18_3_4_26_52,"[MMIUZONE] SE_HandleWinTimerMsg param = %d")
TRACE_MSG(MMIUZONE_SE_1414_112_2_18_3_4_26_53,"[MMIUZONE] SE_HandleWinTimerMsg MSG_ID = %d")
TRACE_MSG(MMIUZONE_SE_1481_112_2_18_3_4_26_54,"[MMIUZONE] SEAPI_HandleMsg msg_id = 0x%04x")
TRACE_MSG(MMIUZONE_WINTAB_209_112_2_18_3_4_27_55,"[MMIUZONE] MMIUZONE_Entry")
TRACE_MSG(MMIUZONE_WINTAB_214_112_2_18_3_4_27_56,"[MMIUZONE] MMIUZONE_Entry return fail for error environment!")
TRACE_MSG(MMIUZONE_WINTAB_273_112_2_18_3_4_27_57,"[MMIUZONE] StartUzoneApplet ret = %d,applet_ptr=0x%lX")
TRACE_MSG(MMIUZONE_WINTAB_293_112_2_18_3_4_27_58,"[MMIUZONE] CloseUzoneApplet applet_ptr=0x%lX")
TRACE_MSG(MMIUZONE_WINTAB_304_112_2_18_3_4_27_59,"[MMIUZONE] SaveNV (%d)")
TRACE_MSG(MMIUZONE_WINTAB_318_112_2_18_3_4_27_60,"[MMIUZONE] InitUzoneSE - Invalid param")
TRACE_MSG(MMIUZONE_WINTAB_322_112_2_18_3_4_27_61,"[MMIUZONE] InitUzoneSe- current item = %d")
TRACE_MSG(MMIUZONE_WINTAB_337_112_2_18_3_4_27_62,"[MMIUZONE] HandleOpenWindow")
TRACE_MSG(MMIUZONE_WINTAB_341_112_2_18_3_4_27_63,"[MMIUZONE] HandleOpenWindow - MMIUZONE_Init fail")
TRACE_MSG(MMIUZONE_WINTAB_347_112_2_18_3_4_27_64,"[MMIUZONE] HandleOpenWindow - MMIUZONE_FillLayers fail")
TRACE_MSG(MMIUZONE_WINTAB_366_112_2_18_3_4_27_65,"[MMIUZONE] HandleGetFocus - Invalid param")
TRACE_MSG(MMIUZONE_WINTAB_373_112_2_18_3_4_27_66,"[MMIUZONE] HandleGetFocusWindow- current item = %d, landsape (%d)")
TRACE_MSG(MMIUZONE_WINTAB_377_112_2_18_3_4_27_67,"[MMIUZONE] HandleGetFocus - MMIUZONE_CreateLayers fail")
TRACE_MSG(MMIUZONE_WINTAB_383_112_2_18_3_4_27_68,"[MMIUZONE] HandleGetFocus - MMIUZONE_FillLayers fail")
TRACE_MSG(MMIUZONE_WINTAB_399_112_2_18_3_4_27_69,"[MMIUZONE] HandleLoseFocusWindow- current item = %d")
TRACE_MSG(MMIUZONE_WINTAB_414_112_2_18_3_4_27_70,"[MMIUZONE] HandleLCDSwitch - Invalid param")
TRACE_MSG(MMIUZONE_WINTAB_422_112_2_18_3_4_27_71,"[MMIUZONE] HandleLCDSwitch - landscape is (%d), current item = %d")
TRACE_MSG(MMIUZONE_WINTAB_431_112_2_18_3_4_27_72,"[MMIUZONE] HandleLCDSwitch - MMIUZONE_CreateLayers fail")
TRACE_MSG(MMIUZONE_WINTAB_437_112_2_18_3_4_27_73,"[MMIUZONE] HandleLCDSwitch - MMIUZONE_FillLayers fail")
TRACE_MSG(MMIUZONE_WINTAB_461_112_2_18_3_4_27_74,"[MMIUZONE] HandleFullPaint - Invalid param")
TRACE_MSG(MMIUZONE_WINTAB_467_112_2_18_3_4_27_75,"[MMIUZONE] HandleFullPaint -is not focus window")
TRACE_MSG(MMIUZONE_WINTAB_471_112_2_18_3_4_27_76,"[MMIUZONE] HandleFullPaint - MMIUZONE_CreateLayers fail")
TRACE_MSG(MMIUZONE_WINTAB_477_112_2_18_3_4_27_77,"[MMIUZONE] HandleFullPaint - MMIUZONE_FillLayers fail")
TRACE_MSG(MMIUZONE_WINTAB_493_112_2_18_3_4_28_78,"[MMIUZONE] HandleFullPaint -is focus window")
TRACE_MSG(MMIUZONE_WINTAB_510_112_2_18_3_4_28_79,"[MMIUZONE] HandleFullPaintFinish - Invalid param")
TRACE_MSG(MMIUZONE_WINTAB_516_112_2_18_3_4_28_80,"[MMIUZONE] HandleFullPaintFinish -is not focus window")
TRACE_MSG(MMIUZONE_WINTAB_536_112_2_18_3_4_28_81,"[MMIUZONE] UzoneApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIUZONE_WINTAB_546_112_2_18_3_4_28_82,"[MMIUZONE] MMIUZONE_Create return fail!")
TRACE_MSG(MMIUZONE_WINTAB_584_112_2_18_3_4_28_83,"[MMIUZONE] HandleUzoneMainWinMsg msg_id = 0x%04x")
END_TRACE_MAP(MMI_APP_UZONE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_UZONE_TRC_H_

