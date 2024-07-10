/******************************************************************************
 ** File Name:      mmi_app_engtd_trc.h                                         *
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
//trace_id:174
#ifndef _MMI_APP_ENGTD_TRC_H_
#define _MMI_APP_ENGTD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIENG_DYNAMIC_MAIN_889_112_2_18_2_18_31_0 "DynaEngItemListWinHandleMsg msg_id = %d"
#define MMIENG_DYNAMIC_MAIN_910_112_2_18_2_18_31_1 "DynaEngItemListWinHandleMsg subfunc_arr_index = %d"
#define MMIENG_DYNAMIC_MAIN_920_112_2_18_2_18_31_2 "DynaEngItemListWinHandleMsg getcountfunc = %d, getfunc = %d, setfunc = %d"
#define MMIENG_DYNAMIC_MAIN_924_112_2_18_2_18_31_3 "DynaEngItemListWinHandleMsg getcountfunc = %d"
#define MMIENG_DYNAMIC_MAIN_1010_112_2_18_2_18_31_4 "eng_dyn: GetEngItem %d, %s %d"
#define MMIENG_DYNAMIC_MAIN_1027_112_2_18_2_18_31_5 "eng_dyn: GetEngItem array>1: %d, %s %s %d"
#define MMIENG_DYNAMIC_MAIN_1062_112_2_18_2_18_31_6 "eng_dyn: dynaeng_name %8x"
#define MMIENG_DYNAMIC_MAIN_1172_112_2_18_2_18_31_7 "DynaEngValueListWinHandleMsg MSG_ID = %x"
#define MMIENG_DYNAMIC_MAIN_1195_112_2_18_2_18_31_8 "DynaEngValueListWinHandleMsg item.arr_count = %d"
#define MMIENG_DYNAMIC_MAIN_1215_112_2_18_2_18_31_9 "DynaEngValueListWinHandleMsg item.arr_value[%d] = %d"
#define MMIENG_DYNAMIC_MAIN_1223_112_2_18_2_18_31_10 "DynaEngValueListWinHandleMsg length = %d"
#define MMIENG_DYNAMIC_MAIN_2009_112_2_18_2_18_33_11 "MN_AL_EM:  auto_attach_power_on_flag == %d"
#define MMIENGTD_MAIN_320_112_2_18_2_18_35_12 "[mmiengtd_main.c] kUpBitRate is %d,kDownBitRate is %d"
#define MMIENGTD_WIN_1690_112_2_18_2_18_40_13 "ENG_ONE_CTRL_TOP()"
#define MMIENGTD_WIN_1719_112_2_18_2_18_40_14 "mmiengtd_win.c GetTimeslotInfoStr str_buf == null"
#define MMIENGTD_WIN_1722_112_2_18_2_18_40_15 "mmiengtd_win.c GetTimeslotInfoStr buf_len is %d"
#define MMIENGTD_WIN_1845_112_2_18_2_18_41_16 "mmiengtd_win.c GetDpchInfoStr str_buf == null"
#define MMIENGTD_WIN_1848_112_2_18_2_18_41_17 "mmieng_wintd.c GetDpchInfoStr buf_len is %d"
#define MMIENGTD_WIN_1966_112_2_18_2_18_41_18 "mmiengtd_win.c GetBlerInfoStr str_buf == null"
#define MMIENGTD_WIN_1969_112_2_18_2_18_41_19 "mmieng_wintd.c GetBlerInfoStr buf_len is %d"
#define MMIENGTD_WIN_2083_112_2_18_2_18_41_20 "mmiengtd_win.c GetOtherInfoStr str_buf == null"
#define MMIENGTD_WIN_2086_112_2_18_2_18_41_21 "mmieng_wintd.c GetOtherInfoStr buf_len is %d"
#define MMIENGTD_WIN_2220_112_2_18_2_18_41_22 "mmiengtd_win.c GetLyTwoInfoStr str_buf == null"
#define MMIENGTD_WIN_2223_112_2_18_2_18_41_23 "mmieng_wintd.c GetLyTwoInfoStr buf_len is %d"
#define MMIENGTD_WIN_2326_112_2_18_2_18_42_24 "mmiengtd_win.c GetScellInfoStr str_buf == null"
#define MMIENGTD_WIN_2329_112_2_18_2_18_42_25 "mmieng_wintd.c GetScellInfoStr buf_len is %d"
#define MMIENGTD_WIN_2474_112_2_18_2_18_42_26 "mmiengtd_win.c GetNcellInfoStr str_buf == null"
#define MMIENGTD_WIN_2477_112_2_18_2_18_42_27 "mmieng_wintd.c GetNcellInfoStr buf_len is %d"
#define MMIENGTD_WIN_2611_112_2_18_2_18_42_28 "mmiengtd_win.c GetNASMMInfoStr str_buf == null"
#define MMIENGTD_WIN_2614_112_2_18_2_18_42_29 "mmieng_wintd.c GetNASMMInfoStr buf_len is %d"
#define MMIENGTD_WIN_2701_112_2_18_2_18_43_30 "mmiengtd_win.c GetNASGMMInfoStr str_buf == null"
#define MMIENGTD_WIN_2704_112_2_18_2_18_43_31 "mmieng_wintd.c GetNASGMMInfoStr buf_len is %d"
#define MMIENGTD_WIN_4155_112_2_18_2_18_46_32 "mmieng_wintd.c SaveUart0MixModeSelect sel_num is %d"
#define MMIENGTD_WIN_4356_112_2_18_2_18_46_33 "mmiengtd_win.c GetAPNSetting APNcn_ptr == null"
#define MMIENGTD_WIN_4374_112_2_18_2_18_46_34 "mmiengtd_win.c SetAPNSetting APNcn_ptr == null"
#define MMIENGTD_WIN_4723_112_2_18_2_18_47_35 "mmieng_wintd.c EngIMSISetWinHandleMsg net_imsi.wstr_len is %d"
#define MMIENGTD_WIN_4925_112_2_18_2_18_47_36 "mmiengtd_win.c GetNetworkInfoStr str_buf == network_buf"
#define MMIENGTD_WIN_4928_112_2_18_2_18_47_37 "mmieng_wintd.c GetNetworkInfoStr buf_len is %d"
#define MMIENGTD_WIN_5157_112_2_18_2_18_48_38 "HandleApnSettingWinMsgg() msg_id = %d"
#define MMIENGTD_WIN_5555_112_2_18_2_18_49_39 "mmieng_wintd.c SavePSBitrateSelectItem sel_num is %d"
#define MMIENGTD_WIN_5864_112_2_18_2_18_49_40 "SetRadioListBoxItems item_str_array 0x%x"
#define MMIENGTD_WIN_5916_112_2_18_2_18_49_41 "[MMIENG VT]  GET RequestMode  request_mode %d"
#define MMIENGTD_WIN_5955_112_2_18_2_18_50_42 "[MMIENG VT]  SET RequestMode request_mode  %d"
#define MMIENGTD_WIN_6007_112_2_18_2_18_50_43 "[MMIENG VT]  GET video_layer.video_al_1 %d, video_layer.video_al_2 %d, video_layer.video_al_3 %d"
#define MMIENGTD_WIN_6069_112_2_18_2_18_50_44 "[MMIENG VT]  SET video_layer.video_al_1 %d, video_layer.video_al_2 %d, video_layer.video_al_3 %d"
#define MMIENGTD_WIN_6498_112_2_18_2_18_51_45 "HandleUPLMNPromptWindow uplmn_txt.wstr_len =%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_ENGTD_TRC)
TRACE_MSG(MMIENG_DYNAMIC_MAIN_889_112_2_18_2_18_31_0,"DynaEngItemListWinHandleMsg msg_id = %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_910_112_2_18_2_18_31_1,"DynaEngItemListWinHandleMsg subfunc_arr_index = %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_920_112_2_18_2_18_31_2,"DynaEngItemListWinHandleMsg getcountfunc = %d, getfunc = %d, setfunc = %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_924_112_2_18_2_18_31_3,"DynaEngItemListWinHandleMsg getcountfunc = %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_1010_112_2_18_2_18_31_4,"eng_dyn: GetEngItem %d, %s %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_1027_112_2_18_2_18_31_5,"eng_dyn: GetEngItem array>1: %d, %s %s %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_1062_112_2_18_2_18_31_6,"eng_dyn: dynaeng_name %8x")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_1172_112_2_18_2_18_31_7,"DynaEngValueListWinHandleMsg MSG_ID = %x")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_1195_112_2_18_2_18_31_8,"DynaEngValueListWinHandleMsg item.arr_count = %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_1215_112_2_18_2_18_31_9,"DynaEngValueListWinHandleMsg item.arr_value[%d] = %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_1223_112_2_18_2_18_31_10,"DynaEngValueListWinHandleMsg length = %d")
TRACE_MSG(MMIENG_DYNAMIC_MAIN_2009_112_2_18_2_18_33_11,"MN_AL_EM:  auto_attach_power_on_flag == %d")
TRACE_MSG(MMIENGTD_MAIN_320_112_2_18_2_18_35_12,"[mmiengtd_main.c] kUpBitRate is %d,kDownBitRate is %d")
TRACE_MSG(MMIENGTD_WIN_1690_112_2_18_2_18_40_13,"ENG_ONE_CTRL_TOP()")
TRACE_MSG(MMIENGTD_WIN_1719_112_2_18_2_18_40_14,"mmiengtd_win.c GetTimeslotInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_1722_112_2_18_2_18_40_15,"mmiengtd_win.c GetTimeslotInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_1845_112_2_18_2_18_41_16,"mmiengtd_win.c GetDpchInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_1848_112_2_18_2_18_41_17,"mmieng_wintd.c GetDpchInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_1966_112_2_18_2_18_41_18,"mmiengtd_win.c GetBlerInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_1969_112_2_18_2_18_41_19,"mmieng_wintd.c GetBlerInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_2083_112_2_18_2_18_41_20,"mmiengtd_win.c GetOtherInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_2086_112_2_18_2_18_41_21,"mmieng_wintd.c GetOtherInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_2220_112_2_18_2_18_41_22,"mmiengtd_win.c GetLyTwoInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_2223_112_2_18_2_18_41_23,"mmieng_wintd.c GetLyTwoInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_2326_112_2_18_2_18_42_24,"mmiengtd_win.c GetScellInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_2329_112_2_18_2_18_42_25,"mmieng_wintd.c GetScellInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_2474_112_2_18_2_18_42_26,"mmiengtd_win.c GetNcellInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_2477_112_2_18_2_18_42_27,"mmieng_wintd.c GetNcellInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_2611_112_2_18_2_18_42_28,"mmiengtd_win.c GetNASMMInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_2614_112_2_18_2_18_42_29,"mmieng_wintd.c GetNASMMInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_2701_112_2_18_2_18_43_30,"mmiengtd_win.c GetNASGMMInfoStr str_buf == null")
TRACE_MSG(MMIENGTD_WIN_2704_112_2_18_2_18_43_31,"mmieng_wintd.c GetNASGMMInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_4155_112_2_18_2_18_46_32,"mmieng_wintd.c SaveUart0MixModeSelect sel_num is %d")
TRACE_MSG(MMIENGTD_WIN_4356_112_2_18_2_18_46_33,"mmiengtd_win.c GetAPNSetting APNcn_ptr == null")
TRACE_MSG(MMIENGTD_WIN_4374_112_2_18_2_18_46_34,"mmiengtd_win.c SetAPNSetting APNcn_ptr == null")
TRACE_MSG(MMIENGTD_WIN_4723_112_2_18_2_18_47_35,"mmieng_wintd.c EngIMSISetWinHandleMsg net_imsi.wstr_len is %d")
TRACE_MSG(MMIENGTD_WIN_4925_112_2_18_2_18_47_36,"mmiengtd_win.c GetNetworkInfoStr str_buf == network_buf")
TRACE_MSG(MMIENGTD_WIN_4928_112_2_18_2_18_47_37,"mmieng_wintd.c GetNetworkInfoStr buf_len is %d")
TRACE_MSG(MMIENGTD_WIN_5157_112_2_18_2_18_48_38,"HandleApnSettingWinMsgg() msg_id = %d")
TRACE_MSG(MMIENGTD_WIN_5555_112_2_18_2_18_49_39,"mmieng_wintd.c SavePSBitrateSelectItem sel_num is %d")
TRACE_MSG(MMIENGTD_WIN_5864_112_2_18_2_18_49_40,"SetRadioListBoxItems item_str_array 0x%x")
TRACE_MSG(MMIENGTD_WIN_5916_112_2_18_2_18_49_41,"[MMIENG VT]  GET RequestMode  request_mode %d")
TRACE_MSG(MMIENGTD_WIN_5955_112_2_18_2_18_50_42,"[MMIENG VT]  SET RequestMode request_mode  %d")
TRACE_MSG(MMIENGTD_WIN_6007_112_2_18_2_18_50_43,"[MMIENG VT]  GET video_layer.video_al_1 %d, video_layer.video_al_2 %d, video_layer.video_al_3 %d")
TRACE_MSG(MMIENGTD_WIN_6069_112_2_18_2_18_50_44,"[MMIENG VT]  SET video_layer.video_al_1 %d, video_layer.video_al_2 %d, video_layer.video_al_3 %d")
TRACE_MSG(MMIENGTD_WIN_6498_112_2_18_2_18_51_45,"HandleUPLMNPromptWindow uplmn_txt.wstr_len =%d")
END_TRACE_MAP(MMI_APP_ENGTD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_ENGTD_TRC_H_

