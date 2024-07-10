/******************************************************************************
 ** File Name:      mmi_app_theme_trc.h                                         *
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
//trace_id:228
#ifndef _MMI_APP_THEME_TRC_H_
#define _MMI_APP_THEME_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_THEME_224_112_2_18_3_1_47_0 "MMITHEME_GetCalendarStyle: the param is PNULL!"
#define MMI_THEME_262_112_2_18_3_1_47_1 "MMITHEME_GetMsgBoxStyle: the param is PNULL!"
#define MMI_THEME_1038_112_2_18_3_1_49_2 "MMITHEME_GetMaxLenForPb:encode_type %d is error!"
#define MMI_THEME_1166_112_2_18_3_1_49_3 "== MMITHEME_CheckLcdBlockId == lcd id: %d, block id: %d is error"
#define MMI_THEME_1449_112_2_18_3_1_50_4 "PNULL != dev_info_ptr, PNULL != rect_ptr"
#define MMIDISPLAY_DATA_119_112_2_18_3_1_51_5 "== MMITHEME_GetIdleDispStyle == e_idle_disp_index %d is error"
#define MMIDISPLAY_DATA_132_112_2_18_3_1_51_6 "132 == width && 64 = height e_idle_disp_index = %d"
#define MMIDISPLAY_DATA_161_112_2_18_3_1_52_7 "== MMITHEME_GetIdleIconPos == e_icon: %d is error"
#define MMITHEME_BLOCK_86_112_2_18_3_1_54_8 "MMITHEME_GetBlockBuf: block %d is used!"
#define MMITHEME_BLOCK_103_112_2_18_3_1_54_9 "MMITHEME_GetBlockBuf: block_id %d no block buffer"
#define MMITHEME_EDIT_587_112_2_18_3_1_57_10 "MMITHEME_GetEditTheme:edit_type %d is error!"
#define MMITHEME_EDIT_652_112_2_18_3_1_57_11 "MMITHEME_GetEditDateRange: year range type %d is error!"
#define MMITHEME_EDIT_917_112_2_18_3_1_58_12 "HandleLandscapeEditWinMsg:edit_type %d is not set string!"
#define MMITHEME_FORM_266_112_2_18_3_1_59_13 "MMITHEME_ModifyFormTheme: form type %d is error!"
#define MMITHEME_FRAME_ENGINE_450_112_2_18_3_2_0_14 "== MMIFRAME_Start == frame : %d/%d"
#define MMITHEME_IM_23274_112_2_18_3_2_51_15 "MMITHEME_IM: English is Local, and be limited!"
#define MMITHEME_LAYER_SPECIAL_817_112_2_18_3_2_55_16 "ZoomseFunc, ticks = %d"
#define MMITHEME_LIST_3191_112_2_18_3_3_2_17 "MMITHEME_DisplayListIndex: cur_num = %d, total_num = %d! title_bg_image_id =%d"
#define MMITHEME_LIST_3195_112_2_18_3_3_2_18 "MMITHEME_DisplayListIndex: cur_num > total_num!"
#define MMITHEME_MENU_1864_112_2_18_3_3_6_19 "DisplayDyMenuIconInFile: input is null!"
#define MMITHEME_POS_580_112_2_18_3_3_9_20 "MMITHEME_GetSoftkeyRect: softkey_index %d is error!"
#define MMITHEME_SPECIAL_466_112_2_18_3_3_14_21 "GetSpecialEffectFrameNum()  default frame num = 0"
#define MMITHEME_SPECIAL_1050_112_2_18_3_3_15_22 "PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
#define MMITHEME_SPECIAL_1222_112_2_18_3_3_15_23 "PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
#define MMITHEME_SPECIAL_1339_112_2_18_3_3_16_24 "PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
#define MMITHEME_SPECIAL_1454_112_2_18_3_3_16_25 "PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
#define MMITHEME_SPECIAL_1569_112_2_18_3_3_16_26 "PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
#define MMITHEME_SPECIAL_1686_112_2_18_3_3_16_27 "PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
#define MMITHEME_SPECIAL_1897_112_2_18_3_3_17_28 "MMITHEME_InvalidateRect_3DRotate old_buf_ptr = %d, new_buf_ptr = %d"
#define MMITHEME_SPECIAL_2805_112_2_18_3_3_19_29 "[ROTATE]Prepare finished, time=%d"
#define MMITHEME_SPECIAL_2820_112_2_18_3_3_19_30 "[ROTATE]First sync scale,time=%d"
#define MMITHEME_SPECIAL_2826_112_2_18_3_3_19_31 "[ROTATE]Pre-Half, Before scal, tick=%d"
#define MMITHEME_SPECIAL_2866_112_2_18_3_3_19_32 "[ROTATE]Pre-Half, Async scale, time=%d"
#define MMITHEME_SPECIAL_2904_112_2_18_3_3_19_33 "[ROTATE]Pre-Half, rotate, time=%d"
#define MMITHEME_SPECIAL_2908_112_2_18_3_3_19_34 "[ROTATE]Pre-Half, refresh LCD, time=%d"
#define MMITHEME_SPECIAL_2948_112_2_18_3_3_19_35 "[ROTATE]Be-Half, scale, time=%d"
#define MMITHEME_SPECIAL_2979_112_2_18_3_3_19_36 "[ROTATE]Be-Half, rotate, time=%d"
#define MMITHEME_SPECIAL_2982_112_2_18_3_3_19_37 "[ROTATE]Be-Half, refresh LCD, time=%d"
#define MMITHEME_SPECIAL_2998_112_2_18_3_3_19_38 "[ROTATE]Restore lcd buf, time=%d"
#define MMITHEME_SPECIAL_3005_112_2_18_3_3_19_39 "[ROTATE]whole time=%d"
#define MMITHEME_SPECIAL_3175_112_2_18_3_3_19_40 "[ROTATE]Pre-Half, Before scal, tick=%d"
#define MMITHEME_SPECIAL_3193_112_2_18_3_3_19_41 "[ROTATE]Pre-Half, After scal2, tick=%d"
#define MMITHEME_SPECIAL_3227_112_2_18_3_3_19_42 "[ROTATE]Pre-Half, After rotate, tick=%d"
#define MMITHEME_SPECIAL_3230_112_2_18_3_3_19_43 "[ROTATE]Pre-Half, After refresh lcd, tick=%d"
#define MMITHEME_SPECIAL_3236_112_2_18_3_3_19_44 "[ROTATE]Be-Half, Before scale, tick=%d"
#define MMITHEME_SPECIAL_3254_112_2_18_3_3_19_45 "[ROTATE]Be-Half, After scale2, tick=%d"
#define MMITHEME_SPECIAL_3288_112_2_18_3_3_19_46 "[ROTATE]Be-Half, After rotate, tick=%d"
#define MMITHEME_SPECIAL_3291_112_2_18_3_3_19_47 "[ROTATE]Be-Half, After refresh lcd, tick=%d"
#define MMITHEME_SPECIAL_3462_112_2_18_3_3_20_48 "MMITHEME_PlayWinRotate Error type!"
#define MMITHEME_SPECIAL_3724_112_2_18_3_3_20_49 "MMITHEME_PopupMenuEffect: buff_old = %d, buff_new = %d"
#define MMITHEME_SPECIAL_3915_112_2_18_3_3_21_50 "MMITHEME_InvalidateRect_FadeIn old_buf_ptr = %d, new_buf_ptr = %d"
#define MMITHEME_STATUSBAR_100_112_2_18_3_3_23_51 "status bar, tp move %d"
#define MMITHEME_UPDATE_260_112_2_18_3_3_26_52 "MMITHEME_StoreUpdateRect:update rect is error!"
#define MMITHEME_UPDATE_293_112_2_18_3_3_27_53 "MMITHEME_StoreUpdateRect: the lcd_id = %d is error!"
#define MMITHEME_UPDATE_576_112_2_18_3_3_27_54 "MMITHEME_UpdateRect: the s_update_delay_count = %d"
#define MMITHEME_UPDATE_742_112_2_18_3_3_27_55 "StoreUpdateBlock:lcd_id %d, block_id %dis error!"
#define MMITHEME_UPDATE_759_112_2_18_3_3_27_56 "MMITHEME_SetUpdateDelayCount: the s_update_delay_count = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_THEME_TRC)
TRACE_MSG(MMI_THEME_224_112_2_18_3_1_47_0,"MMITHEME_GetCalendarStyle: the param is PNULL!")
TRACE_MSG(MMI_THEME_262_112_2_18_3_1_47_1,"MMITHEME_GetMsgBoxStyle: the param is PNULL!")
TRACE_MSG(MMI_THEME_1038_112_2_18_3_1_49_2,"MMITHEME_GetMaxLenForPb:encode_type %d is error!")
TRACE_MSG(MMI_THEME_1166_112_2_18_3_1_49_3,"== MMITHEME_CheckLcdBlockId == lcd id: %d, block id: %d is error")
TRACE_MSG(MMI_THEME_1449_112_2_18_3_1_50_4,"PNULL != dev_info_ptr, PNULL != rect_ptr")
TRACE_MSG(MMIDISPLAY_DATA_119_112_2_18_3_1_51_5,"== MMITHEME_GetIdleDispStyle == e_idle_disp_index %d is error")
TRACE_MSG(MMIDISPLAY_DATA_132_112_2_18_3_1_51_6,"132 == width && 64 = height e_idle_disp_index = %d")
TRACE_MSG(MMIDISPLAY_DATA_161_112_2_18_3_1_52_7,"== MMITHEME_GetIdleIconPos == e_icon: %d is error")
TRACE_MSG(MMITHEME_BLOCK_86_112_2_18_3_1_54_8,"MMITHEME_GetBlockBuf: block %d is used!")
TRACE_MSG(MMITHEME_BLOCK_103_112_2_18_3_1_54_9,"MMITHEME_GetBlockBuf: block_id %d no block buffer")
TRACE_MSG(MMITHEME_EDIT_587_112_2_18_3_1_57_10,"MMITHEME_GetEditTheme:edit_type %d is error!")
TRACE_MSG(MMITHEME_EDIT_652_112_2_18_3_1_57_11,"MMITHEME_GetEditDateRange: year range type %d is error!")
TRACE_MSG(MMITHEME_EDIT_917_112_2_18_3_1_58_12,"HandleLandscapeEditWinMsg:edit_type %d is not set string!")
TRACE_MSG(MMITHEME_FORM_266_112_2_18_3_1_59_13,"MMITHEME_ModifyFormTheme: form type %d is error!")
TRACE_MSG(MMITHEME_FRAME_ENGINE_450_112_2_18_3_2_0_14,"== MMIFRAME_Start == frame : %d/%d")
TRACE_MSG(MMITHEME_IM_23274_112_2_18_3_2_51_15,"MMITHEME_IM: English is Local, and be limited!")
TRACE_MSG(MMITHEME_LAYER_SPECIAL_817_112_2_18_3_2_55_16,"ZoomseFunc, ticks = %d")
TRACE_MSG(MMITHEME_LIST_3191_112_2_18_3_3_2_17,"MMITHEME_DisplayListIndex: cur_num = %d, total_num = %d! title_bg_image_id =%d")
TRACE_MSG(MMITHEME_LIST_3195_112_2_18_3_3_2_18,"MMITHEME_DisplayListIndex: cur_num > total_num!")
TRACE_MSG(MMITHEME_MENU_1864_112_2_18_3_3_6_19,"DisplayDyMenuIconInFile: input is null!")
TRACE_MSG(MMITHEME_POS_580_112_2_18_3_3_9_20,"MMITHEME_GetSoftkeyRect: softkey_index %d is error!")
TRACE_MSG(MMITHEME_SPECIAL_466_112_2_18_3_3_14_21,"GetSpecialEffectFrameNum()  default frame num = 0")
TRACE_MSG(MMITHEME_SPECIAL_1050_112_2_18_3_3_15_22,"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d")
TRACE_MSG(MMITHEME_SPECIAL_1222_112_2_18_3_3_15_23,"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d")
TRACE_MSG(MMITHEME_SPECIAL_1339_112_2_18_3_3_16_24,"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d")
TRACE_MSG(MMITHEME_SPECIAL_1454_112_2_18_3_3_16_25,"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d")
TRACE_MSG(MMITHEME_SPECIAL_1569_112_2_18_3_3_16_26,"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d")
TRACE_MSG(MMITHEME_SPECIAL_1686_112_2_18_3_3_16_27,"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d")
TRACE_MSG(MMITHEME_SPECIAL_1897_112_2_18_3_3_17_28,"MMITHEME_InvalidateRect_3DRotate old_buf_ptr = %d, new_buf_ptr = %d")
TRACE_MSG(MMITHEME_SPECIAL_2805_112_2_18_3_3_19_29,"[ROTATE]Prepare finished, time=%d")
TRACE_MSG(MMITHEME_SPECIAL_2820_112_2_18_3_3_19_30,"[ROTATE]First sync scale,time=%d")
TRACE_MSG(MMITHEME_SPECIAL_2826_112_2_18_3_3_19_31,"[ROTATE]Pre-Half, Before scal, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_2866_112_2_18_3_3_19_32,"[ROTATE]Pre-Half, Async scale, time=%d")
TRACE_MSG(MMITHEME_SPECIAL_2904_112_2_18_3_3_19_33,"[ROTATE]Pre-Half, rotate, time=%d")
TRACE_MSG(MMITHEME_SPECIAL_2908_112_2_18_3_3_19_34,"[ROTATE]Pre-Half, refresh LCD, time=%d")
TRACE_MSG(MMITHEME_SPECIAL_2948_112_2_18_3_3_19_35,"[ROTATE]Be-Half, scale, time=%d")
TRACE_MSG(MMITHEME_SPECIAL_2979_112_2_18_3_3_19_36,"[ROTATE]Be-Half, rotate, time=%d")
TRACE_MSG(MMITHEME_SPECIAL_2982_112_2_18_3_3_19_37,"[ROTATE]Be-Half, refresh LCD, time=%d")
TRACE_MSG(MMITHEME_SPECIAL_2998_112_2_18_3_3_19_38,"[ROTATE]Restore lcd buf, time=%d")
TRACE_MSG(MMITHEME_SPECIAL_3005_112_2_18_3_3_19_39,"[ROTATE]whole time=%d")
TRACE_MSG(MMITHEME_SPECIAL_3175_112_2_18_3_3_19_40,"[ROTATE]Pre-Half, Before scal, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_3193_112_2_18_3_3_19_41,"[ROTATE]Pre-Half, After scal2, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_3227_112_2_18_3_3_19_42,"[ROTATE]Pre-Half, After rotate, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_3230_112_2_18_3_3_19_43,"[ROTATE]Pre-Half, After refresh lcd, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_3236_112_2_18_3_3_19_44,"[ROTATE]Be-Half, Before scale, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_3254_112_2_18_3_3_19_45,"[ROTATE]Be-Half, After scale2, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_3288_112_2_18_3_3_19_46,"[ROTATE]Be-Half, After rotate, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_3291_112_2_18_3_3_19_47,"[ROTATE]Be-Half, After refresh lcd, tick=%d")
TRACE_MSG(MMITHEME_SPECIAL_3462_112_2_18_3_3_20_48,"MMITHEME_PlayWinRotate Error type!")
TRACE_MSG(MMITHEME_SPECIAL_3724_112_2_18_3_3_20_49,"MMITHEME_PopupMenuEffect: buff_old = %d, buff_new = %d")
TRACE_MSG(MMITHEME_SPECIAL_3915_112_2_18_3_3_21_50,"MMITHEME_InvalidateRect_FadeIn old_buf_ptr = %d, new_buf_ptr = %d")
TRACE_MSG(MMITHEME_STATUSBAR_100_112_2_18_3_3_23_51,"status bar, tp move %d")
TRACE_MSG(MMITHEME_UPDATE_260_112_2_18_3_3_26_52,"MMITHEME_StoreUpdateRect:update rect is error!")
TRACE_MSG(MMITHEME_UPDATE_293_112_2_18_3_3_27_53,"MMITHEME_StoreUpdateRect: the lcd_id = %d is error!")
TRACE_MSG(MMITHEME_UPDATE_576_112_2_18_3_3_27_54,"MMITHEME_UpdateRect: the s_update_delay_count = %d")
TRACE_MSG(MMITHEME_UPDATE_742_112_2_18_3_3_27_55,"StoreUpdateBlock:lcd_id %d, block_id %dis error!")
TRACE_MSG(MMITHEME_UPDATE_759_112_2_18_3_3_27_56,"MMITHEME_SetUpdateDelayCount: the s_update_delay_count = %d")
END_TRACE_MAP(MMI_APP_THEME_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_THEME_TRC_H_

