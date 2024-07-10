/******************************************************************************
 ** File Name:      mmi_app_pic_viewer_trc.h                                         *
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
//trace_id:211
#ifndef _MMI_APP_PIC_VIEWER_TRC_H_
#define _MMI_APP_PIC_VIEWER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPICVIEW_200_112_2_18_2_46_18_0 "MMIPICVIEW MMIPICVIEW_SetSlideTime slide_time=%d, overflow !!!"
#define MMIPICVIEW_237_112_2_18_2_46_18_1 "MMIPICVIEW MMIPICVIEW_SetViewType view_type=%d, overflow!!!"
#define MMIPICVIEW_273_112_2_18_2_46_18_2 "MMIPICVIEW GetDefaultPath path_ptr is PNULL !!!"
#define MMIPICVIEW_323_112_2_18_2_46_18_3 "MMIPICVIEW MMIAPIPICVIEW_GetDefaultPath path_ptr is PNULL !!!"
#define MMIPICVIEW_348_112_2_18_2_46_18_4 "MMIPICVIEW MMIAPIPICVIEW_SetDefaultPath path_ptr is PNULL !!!"
#define MMIPICVIEW_446_112_2_18_2_46_18_5 "MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
#define MMIPICVIEW_523_112_2_18_2_46_18_6 "MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
#define MMIPICVIEW_LIST_428_112_2_18_2_46_21_7 "MMIPICVIEW ClearPicList view_type=%d"
#define MMIPICVIEW_LIST_853_112_2_18_2_46_22_8 "MMIPICVIEW ClearPicFolderList view_type=%d"
#define MMIPICVIEW_LIST_1034_112_2_18_2_46_23_9 "MMIPICVIEW UpdatePicFolderList view_type=%d"
#define MMIPICVIEW_LIST_1209_112_2_18_2_46_23_10 "MMIPICVIEW HandlePicListWinMsg item_data_ptr is PNULL !!!"
#define MMIPICVIEW_LIST_1221_112_2_18_2_46_23_11 "MMIPICVIEW HandlePicListWinMsg item_content_ptr is PNULL !!!"
#define MMIPICVIEW_LIST_1250_112_2_18_2_46_23_12 "MMIPICVIEW HandlePicListWinMsg MSG_GET_FOCUS"
#define MMIPICVIEW_LIST_1488_112_2_18_2_46_24_13 "MMIPICVIEW CreatePicListCtrl view_type=%d"
#define MMIPICVIEW_LIST_1695_112_2_18_2_46_24_14 "MMIPICVIEW GetAllPicSuffix filter_ptr is PNULL !!!"
#define MMIPICVIEW_LIST_1765_112_2_18_2_46_24_15 "MMIPICVIEW ClearPicList view_type=%d"
#define MMIPICVIEW_LIST_1809_112_2_18_2_46_24_16 "MMIPICVIEW UpdatePicList cur_index=%d"
#define MMIPICVIEW_LIST_1990_112_2_18_2_46_25_17 "MMIPICVIEW ModifyPicListView view_type=%d"
#define MMIPICVIEW_LIST_2190_112_2_18_2_46_25_18 "MMIPICVIEW MMIPICVIEW_GetCurIndex view_type=%d"
#define MMIPICVIEW_LIST_2221_112_2_18_2_46_25_19 "MMIPICVIEW MMIPICVIEW_SetCurIndex view_type=%d"
#define MMIPICVIEW_LIST_2251_112_2_18_2_46_25_20 "MMIPICVIEW MMIPICVIEW_SetMaxSelected view_type=%d"
#define MMIPICVIEW_LIST_2282_112_2_18_2_46_25_21 "MMIPICVIEW MMIPICVIEW_GetListMarkable view_type=%d"
#define MMIPICVIEW_LIST_2314_112_2_18_2_46_25_22 "MMIPICVIEW MMIPICVIEW_SetListMarkable markable=%d"
#define MMIPICVIEW_LIST_2347_112_2_18_2_46_25_23 "MMIPICVIEW IsSelectItem view_type=%d"
#define MMIPICVIEW_LIST_2384_112_2_18_2_46_25_24 "MMIPICVIEW GetSelectItemIndex view_type=%d"
#define MMIPICVIEW_LIST_2417_112_2_18_2_46_25_25 "MMIPICVIEW SetSelectedItem view_type=%d"
#define MMIPICVIEW_LIST_2449_112_2_18_2_46_25_26 "MMIPICVIEW MMIPICVIEW_SetAllSelected view_type=%d"
#define MMIPICVIEW_LIST_2482_112_2_18_2_46_25_27 "MMIPICVIEW RemoveItem view_type=%d"
#define MMIPICVIEW_LIST_2514_112_2_18_2_46_26_28 "MMIPICVIEW MMIPICVIEW_ModifyItemIcon view_type=%d"
#define MMIPICVIEW_LIST_2594_112_2_18_2_46_26_29 "MMIPICVIEW MMIPICVIEW_ModifyItemText view_type=%d"
#define MMIPICVIEW_PREVIEW_746_112_2_18_2_46_30_30 "MMIPICVIEW HandlePicPreviewWinMsg MSG_TP_GESTURE param=%d"
#define MMIPICVIEW_PREVIEW_1324_112_2_18_2_46_31_31 "SetAnimParam:item_index%d, %d"
#define MMIPICVIEW_PREVIEW_1378_112_2_18_2_46_31_32 "SetAnimParam:GUIANIM_SetParam,filename:%s,name_len =%d,result = %d,item_index = %d"
#define MMIPICVIEW_PREVIEW_1571_112_2_18_2_46_31_33 "StartDisplayTimer:don't creat display timer"
#define MMIPICVIEW_PREVIEW_1607_112_2_18_2_46_31_34 "MMIPICVIEW PreviousPic total_num is 0 !!!"
#define MMIPICVIEW_PREVIEW_1684_112_2_18_2_46_32_35 "MMIPICVIEW NextPic total_num is 0 !!!"
#define MMIPICVIEW_PREVIEW_1795_112_2_18_2_46_32_36 "MMIPICVIEW HandleZoomInPic item_index=%d"
#define MMIPICVIEW_PREVIEW_1819_112_2_18_2_46_32_37 "MMIPICVIEW HandleZoomOutPic item_index=%d"
#define MMIPICVIEW_PREVIEW_1947_112_2_18_2_46_32_38 "MMIPICVIEW HandlePicGlideWinMsg msg_id = 0x%x "
#define MMIPICVIEW_PREVIEW_1951_112_2_18_2_46_32_39 "MMIPICVIEW HandlePicGlideWinMsg MSG_OPEN_WINDOW "
#define MMIPICVIEW_PREVIEW_1972_112_2_18_2_46_32_40 "MMIPICVIEW HandlePicGlideWinMsg MSG_GET_FOCUS "
#define MMIPICVIEW_PREVIEW_1978_112_2_18_2_46_32_41 "MMIPICVIEW HandlePicGlideWinMsg MSG_LOSE_FOCUS "
#define MMIPICVIEW_PREVIEW_2025_112_2_18_2_46_32_42 "MMIPICVIEW HandlePicGlideWinMsg MSG_APP_OK "
#define MMIPICVIEW_PREVIEW_2037_112_2_18_2_46_32_43 "MMIPICVIEW HandlePicGlideWinMsg MSG_APP_WEB "
#define MMIPICVIEW_PREVIEW_2050_112_2_18_2_46_32_44 "MMIPICVIEW HandlePicGlideWinMsg SCI_TRACE_LOW "
#define MMIPICVIEW_PREVIEW_2062_112_2_18_2_46_33_45 "MMIPICVIEW HandlePicGlideWinMsg MSG_APP_RIGHT "
#define MMIPICVIEW_PREVIEW_2074_112_2_18_2_46_33_46 "MMIPICVIEW HandlePicGlideWinMsg MSG_CTL_CANCEL "
#define MMIPICVIEW_PREVIEW_2077_112_2_18_2_46_33_47 "MMIPICVIEW HandlePicGlideWinMsg GUIICONLIST_PrepareSpecial "
#define MMIPICVIEW_PREVIEW_2084_112_2_18_2_46_33_48 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_DOWN "
#define MMIPICVIEW_PREVIEW_2089_112_2_18_2_46_33_49 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_MOVE "
#define MMIPICVIEW_PREVIEW_2094_112_2_18_2_46_33_50 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_UP "
#define MMIPICVIEW_PREVIEW_2103_112_2_18_2_46_33_51 "MMIPICVIEW HandlePicGlideWinMsg MMITHEME_UpdateRect "
#define MMIPICVIEW_PREVIEW_2110_112_2_18_2_46_33_52 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_DOUBLE "
#define MMIPICVIEW_PREVIEW_2119_112_2_18_2_46_33_53 "MMIPICVIEW HandlePicGlideWinMsg MSG_NOTIFY_UPDATE "
#define MMIPICVIEW_PREVIEW_2128_112_2_18_2_46_33_54 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE param=%d"
#define MMIPICVIEW_PREVIEW_2131_112_2_18_2_46_33_55 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE tp_points.num = %d"
#define MMIPICVIEW_PREVIEW_2132_112_2_18_2_46_33_56 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE tp_points_old.num = %d"
#define MMIPICVIEW_PREVIEW_2157_112_2_18_2_46_33_57 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist_temp1 = %.2f"
#define MMIPICVIEW_PREVIEW_2158_112_2_18_2_46_33_58 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist_temp2 = %.2f"
#define MMIPICVIEW_PREVIEW_2159_112_2_18_2_46_33_59 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist1 = %d"
#define MMIPICVIEW_PREVIEW_2160_112_2_18_2_46_33_60 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist2 = %d"
#define MMIPICVIEW_PREVIEW_2230_112_2_18_2_46_33_61 "MMIPICVIEW HandlePicGlideWinMsg MSG_CLOSE_WINDOW "
#define MMIPICVIEW_PREVIEW_2236_112_2_18_2_46_33_62 "MMIPICVIEW HandlePicGlideWinMsg MSG_NOTIFY_ANIM_DISPLAY_IND "
#define MMIPICVIEW_PREVIEW_2260_112_2_18_2_46_33_63 "MMIPICVIEW HandlePicGlideWinMsg end "
#define MMIPICVIEW_PREVIEW_2534_112_2_18_2_46_33_64 "MMIPICVIEW ExchangeAssisoMidAnimCtrl"
#define MMIPICVIEW_PREVIEW_2678_112_2_18_2_46_34_65 "SetLeftAnimParam:total_num:%d,item_index =%d"
#define MMIPICVIEW_PREVIEW_2740_112_2_18_2_46_34_66 "SetMidAnimParam:item_index =%d"
#define MMIPICVIEW_PREVIEW_2762_112_2_18_2_46_34_67 "SetRightAnimParam:total_num:%d,item_index =%d"
#define MMIPICVIEW_PREVIEW_4065_112_2_18_2_46_37_68 "MMIPICVIEW HandlePicPreviewWinMsg MSG_TP_GESTURE param=%d"
#define MMIPICVIEW_PREVIEW_4552_112_2_18_2_46_38_69 "StartDisplayTimer:don't creat display timer"
#define MMIPICVIEW_PREVIEW_4588_112_2_18_2_46_38_70 "MMIPICVIEW PreviousPic total_num is 0 !!!"
#define MMIPICVIEW_PREVIEW_4665_112_2_18_2_46_38_71 "MMIPICVIEW NextPic total_num is 0 !!!"
#define MMIPICVIEW_PREVIEW_4776_112_2_18_2_46_38_72 "MMIPICVIEW HandleZoomPic item_index=%d"
#define MMIPICVIEW_PREVIEW_4918_112_2_18_2_46_38_73 "MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE param=%d"
#define MMIPICVIEW_WINTAB_1073_112_2_18_2_46_43_74 "OpenSetDefaultPathCallBack array size == 0"
#define MMIPICVIEW_WINTAB_1078_112_2_18_2_46_43_75 "OpenSetDefaultPathCallBack failed"
#define MMIPICVIEW_WINTAB_1138_112_2_18_2_46_43_76 "MMIPICVIEW HandlePicFolderlistOptWinMsg:menu_id %d is error!!!"
#define MMIPICVIEW_WINTAB_1335_112_2_18_2_46_43_77 "MMIPICVIEW HandlePiclistOptWinMsg:menu_id %d is error!!!"
#define MMIPICVIEW_WINTAB_1449_112_2_18_2_46_44_78 "MMIPICVIEW SetGrayListOptMenu view_type=%d"
#define MMIPICVIEW_WINTAB_1719_112_2_18_2_46_44_79 "StartSlidePlayTimer:don't creat slide play timer"
#define MMIPICVIEW_WINTAB_1812_112_2_18_2_46_44_80 "MMIPICVIEW NextSlidePic total_num is 0 !!!"
#define MMIPICVIEW_WINTAB_1987_112_2_18_2_46_45_81 "MMIPICVIEW HandlePreviewOptWinMsg msg_id = 0x%x "
#define MMIPICVIEW_WINTAB_2125_112_2_18_2_46_45_82 "MMIPICVIEW HandlePreviewOptWinMsg:menu_id %d is error!!!"
#define MMIPICVIEW_WINTAB_2536_112_2_18_2_46_46_83 "MMIPICVIEW SetSlideEffectIndex cur_index is overflow !!!"
#define MMIPICVIEW_WINTAB_2589_112_2_18_2_46_46_84 "MMIPICVIEW SetSlideTimeIndex cur_index is overflow !!!"
#define MMIPICVIEW_WINTAB_2993_112_2_18_2_46_47_85 "StopDelete"
#define MMIPICVIEW_WINTAB_3585_112_2_18_2_46_48_86 "[MMIPIC]:SetDetail dev=%d,ERROR!!!!!!!!!"
#define MMIPICVIEW_WINTAB_3635_112_2_18_2_46_48_87 "MMIPICVIEW AddFixStrItem index_ptr is PNULL !!!"
#define MMIPICVIEW_WINTAB_3641_112_2_18_2_46_48_88 "MMIPICVIEW AddFixStrItem richtext_item_ptr is PNULL !!!"
#define MMIPICVIEW_WINTAB_3671_112_2_18_2_46_48_89 "MMIPICVIEW AddStrItem str_ptr is PNULL !!!"
#define MMIPICVIEW_WINTAB_3677_112_2_18_2_46_48_90 "MMIPICVIEW AddStrItem index_ptr is PNULL !!!"
#define MMIPICVIEW_WINTAB_3683_112_2_18_2_46_48_91 "MMIPICVIEW AddStrItem richtext_item_ptr is PNULL !!!"
#define MMIPICVIEW_WINTAB_3882_112_2_18_2_46_49_92 "MMIPICVIEW HandlePiclistLongokOptWinMsg:menu_id %d is error!!!"
#define MMIPICVIEW_ZOOM_259_112_2_18_2_46_51_93 "MMIPICVIEW MMIPICVIEW_Zoom full_path_wstr_ptr is PNULL !!!"
#define MMIPICVIEW_ZOOM_347_112_2_18_2_46_51_94 "MMIPICVIEW HandlePicZoomWinMsg MSG_TP_GESTURE type=%d"
#define MMIPICVIEW_ZOOM_412_112_2_18_2_46_51_95 "MMIPICVIEW HandlePicZoomWinMsg MSG_PICVIEWER_MIN_ZOOM_OUT"
#define MMIPICVIEW_ZOOM_854_112_2_18_2_46_52_96 "StartZoomOsdTimer:don't creat display timer"
#define MMIPICVIEW_ZOOM_988_112_2_18_2_46_53_97 "MMIPICVIEW HandleZoomKeyMove:msg_id %d is error!!!"
#define MMIPICVIEW_ZOOM_1101_112_2_18_2_46_53_98 "MMIPICVIEW ProcessPicState pic_state=%d"
#define MMIPICVIEW_ZOOM_1115_112_2_18_2_46_53_99 "MMIPICVIEW ProcessPicState is_decoded=%d"
#define MMIPICVIEW_ZOOM_1136_112_2_18_2_46_53_100 "MMIPICVIEW ProcessPicState zoom_out_counter=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PIC_VIEWER_TRC)
TRACE_MSG(MMIPICVIEW_200_112_2_18_2_46_18_0,"MMIPICVIEW MMIPICVIEW_SetSlideTime slide_time=%d, overflow !!!")
TRACE_MSG(MMIPICVIEW_237_112_2_18_2_46_18_1,"MMIPICVIEW MMIPICVIEW_SetViewType view_type=%d, overflow!!!")
TRACE_MSG(MMIPICVIEW_273_112_2_18_2_46_18_2,"MMIPICVIEW GetDefaultPath path_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_323_112_2_18_2_46_18_3,"MMIPICVIEW MMIAPIPICVIEW_GetDefaultPath path_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_348_112_2_18_2_46_18_4,"MMIPICVIEW MMIAPIPICVIEW_SetDefaultPath path_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_446_112_2_18_2_46_18_5,"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d")
TRACE_MSG(MMIPICVIEW_523_112_2_18_2_46_18_6,"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d")
TRACE_MSG(MMIPICVIEW_LIST_428_112_2_18_2_46_21_7,"MMIPICVIEW ClearPicList view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_853_112_2_18_2_46_22_8,"MMIPICVIEW ClearPicFolderList view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_1034_112_2_18_2_46_23_9,"MMIPICVIEW UpdatePicFolderList view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_1209_112_2_18_2_46_23_10,"MMIPICVIEW HandlePicListWinMsg item_data_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_LIST_1221_112_2_18_2_46_23_11,"MMIPICVIEW HandlePicListWinMsg item_content_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_LIST_1250_112_2_18_2_46_23_12,"MMIPICVIEW HandlePicListWinMsg MSG_GET_FOCUS")
TRACE_MSG(MMIPICVIEW_LIST_1488_112_2_18_2_46_24_13,"MMIPICVIEW CreatePicListCtrl view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_1695_112_2_18_2_46_24_14,"MMIPICVIEW GetAllPicSuffix filter_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_LIST_1765_112_2_18_2_46_24_15,"MMIPICVIEW ClearPicList view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_1809_112_2_18_2_46_24_16,"MMIPICVIEW UpdatePicList cur_index=%d")
TRACE_MSG(MMIPICVIEW_LIST_1990_112_2_18_2_46_25_17,"MMIPICVIEW ModifyPicListView view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2190_112_2_18_2_46_25_18,"MMIPICVIEW MMIPICVIEW_GetCurIndex view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2221_112_2_18_2_46_25_19,"MMIPICVIEW MMIPICVIEW_SetCurIndex view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2251_112_2_18_2_46_25_20,"MMIPICVIEW MMIPICVIEW_SetMaxSelected view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2282_112_2_18_2_46_25_21,"MMIPICVIEW MMIPICVIEW_GetListMarkable view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2314_112_2_18_2_46_25_22,"MMIPICVIEW MMIPICVIEW_SetListMarkable markable=%d")
TRACE_MSG(MMIPICVIEW_LIST_2347_112_2_18_2_46_25_23,"MMIPICVIEW IsSelectItem view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2384_112_2_18_2_46_25_24,"MMIPICVIEW GetSelectItemIndex view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2417_112_2_18_2_46_25_25,"MMIPICVIEW SetSelectedItem view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2449_112_2_18_2_46_25_26,"MMIPICVIEW MMIPICVIEW_SetAllSelected view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2482_112_2_18_2_46_25_27,"MMIPICVIEW RemoveItem view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2514_112_2_18_2_46_26_28,"MMIPICVIEW MMIPICVIEW_ModifyItemIcon view_type=%d")
TRACE_MSG(MMIPICVIEW_LIST_2594_112_2_18_2_46_26_29,"MMIPICVIEW MMIPICVIEW_ModifyItemText view_type=%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_746_112_2_18_2_46_30_30,"MMIPICVIEW HandlePicPreviewWinMsg MSG_TP_GESTURE param=%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_1324_112_2_18_2_46_31_31,"SetAnimParam:item_index%d, %d")
TRACE_MSG(MMIPICVIEW_PREVIEW_1378_112_2_18_2_46_31_32,"SetAnimParam:GUIANIM_SetParam,filename:%s,name_len =%d,result = %d,item_index = %d")
TRACE_MSG(MMIPICVIEW_PREVIEW_1571_112_2_18_2_46_31_33,"StartDisplayTimer:don't creat display timer")
TRACE_MSG(MMIPICVIEW_PREVIEW_1607_112_2_18_2_46_31_34,"MMIPICVIEW PreviousPic total_num is 0 !!!")
TRACE_MSG(MMIPICVIEW_PREVIEW_1684_112_2_18_2_46_32_35,"MMIPICVIEW NextPic total_num is 0 !!!")
TRACE_MSG(MMIPICVIEW_PREVIEW_1795_112_2_18_2_46_32_36,"MMIPICVIEW HandleZoomInPic item_index=%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_1819_112_2_18_2_46_32_37,"MMIPICVIEW HandleZoomOutPic item_index=%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_1947_112_2_18_2_46_32_38,"MMIPICVIEW HandlePicGlideWinMsg msg_id = 0x%x ")
TRACE_MSG(MMIPICVIEW_PREVIEW_1951_112_2_18_2_46_32_39,"MMIPICVIEW HandlePicGlideWinMsg MSG_OPEN_WINDOW ")
TRACE_MSG(MMIPICVIEW_PREVIEW_1972_112_2_18_2_46_32_40,"MMIPICVIEW HandlePicGlideWinMsg MSG_GET_FOCUS ")
TRACE_MSG(MMIPICVIEW_PREVIEW_1978_112_2_18_2_46_32_41,"MMIPICVIEW HandlePicGlideWinMsg MSG_LOSE_FOCUS ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2025_112_2_18_2_46_32_42,"MMIPICVIEW HandlePicGlideWinMsg MSG_APP_OK ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2037_112_2_18_2_46_32_43,"MMIPICVIEW HandlePicGlideWinMsg MSG_APP_WEB ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2050_112_2_18_2_46_32_44,"MMIPICVIEW HandlePicGlideWinMsg SCI_TRACE_LOW ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2062_112_2_18_2_46_33_45,"MMIPICVIEW HandlePicGlideWinMsg MSG_APP_RIGHT ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2074_112_2_18_2_46_33_46,"MMIPICVIEW HandlePicGlideWinMsg MSG_CTL_CANCEL ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2077_112_2_18_2_46_33_47,"MMIPICVIEW HandlePicGlideWinMsg GUIICONLIST_PrepareSpecial ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2084_112_2_18_2_46_33_48,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_DOWN ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2089_112_2_18_2_46_33_49,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_MOVE ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2094_112_2_18_2_46_33_50,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_UP ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2103_112_2_18_2_46_33_51,"MMIPICVIEW HandlePicGlideWinMsg MMITHEME_UpdateRect ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2110_112_2_18_2_46_33_52,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_PRESS_DOUBLE ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2119_112_2_18_2_46_33_53,"MMIPICVIEW HandlePicGlideWinMsg MSG_NOTIFY_UPDATE ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2128_112_2_18_2_46_33_54,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE param=%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_2131_112_2_18_2_46_33_55,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE tp_points.num = %d")
TRACE_MSG(MMIPICVIEW_PREVIEW_2132_112_2_18_2_46_33_56,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE tp_points_old.num = %d")
TRACE_MSG(MMIPICVIEW_PREVIEW_2157_112_2_18_2_46_33_57,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist_temp1 = %.2f")
TRACE_MSG(MMIPICVIEW_PREVIEW_2158_112_2_18_2_46_33_58,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist_temp2 = %.2f")
TRACE_MSG(MMIPICVIEW_PREVIEW_2159_112_2_18_2_46_33_59,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist1 = %d")
TRACE_MSG(MMIPICVIEW_PREVIEW_2160_112_2_18_2_46_33_60,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE dist2 = %d")
TRACE_MSG(MMIPICVIEW_PREVIEW_2230_112_2_18_2_46_33_61,"MMIPICVIEW HandlePicGlideWinMsg MSG_CLOSE_WINDOW ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2236_112_2_18_2_46_33_62,"MMIPICVIEW HandlePicGlideWinMsg MSG_NOTIFY_ANIM_DISPLAY_IND ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2260_112_2_18_2_46_33_63,"MMIPICVIEW HandlePicGlideWinMsg end ")
TRACE_MSG(MMIPICVIEW_PREVIEW_2534_112_2_18_2_46_33_64,"MMIPICVIEW ExchangeAssisoMidAnimCtrl")
TRACE_MSG(MMIPICVIEW_PREVIEW_2678_112_2_18_2_46_34_65,"SetLeftAnimParam:total_num:%d,item_index =%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_2740_112_2_18_2_46_34_66,"SetMidAnimParam:item_index =%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_2762_112_2_18_2_46_34_67,"SetRightAnimParam:total_num:%d,item_index =%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_4065_112_2_18_2_46_37_68,"MMIPICVIEW HandlePicPreviewWinMsg MSG_TP_GESTURE param=%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_4552_112_2_18_2_46_38_69,"StartDisplayTimer:don't creat display timer")
TRACE_MSG(MMIPICVIEW_PREVIEW_4588_112_2_18_2_46_38_70,"MMIPICVIEW PreviousPic total_num is 0 !!!")
TRACE_MSG(MMIPICVIEW_PREVIEW_4665_112_2_18_2_46_38_71,"MMIPICVIEW NextPic total_num is 0 !!!")
TRACE_MSG(MMIPICVIEW_PREVIEW_4776_112_2_18_2_46_38_72,"MMIPICVIEW HandleZoomPic item_index=%d")
TRACE_MSG(MMIPICVIEW_PREVIEW_4918_112_2_18_2_46_38_73,"MMIPICVIEW HandlePicGlideWinMsg MSG_TP_GESTURE param=%d")
TRACE_MSG(MMIPICVIEW_WINTAB_1073_112_2_18_2_46_43_74,"OpenSetDefaultPathCallBack array size == 0")
TRACE_MSG(MMIPICVIEW_WINTAB_1078_112_2_18_2_46_43_75,"OpenSetDefaultPathCallBack failed")
TRACE_MSG(MMIPICVIEW_WINTAB_1138_112_2_18_2_46_43_76,"MMIPICVIEW HandlePicFolderlistOptWinMsg:menu_id %d is error!!!")
TRACE_MSG(MMIPICVIEW_WINTAB_1335_112_2_18_2_46_43_77,"MMIPICVIEW HandlePiclistOptWinMsg:menu_id %d is error!!!")
TRACE_MSG(MMIPICVIEW_WINTAB_1449_112_2_18_2_46_44_78,"MMIPICVIEW SetGrayListOptMenu view_type=%d")
TRACE_MSG(MMIPICVIEW_WINTAB_1719_112_2_18_2_46_44_79,"StartSlidePlayTimer:don't creat slide play timer")
TRACE_MSG(MMIPICVIEW_WINTAB_1812_112_2_18_2_46_44_80,"MMIPICVIEW NextSlidePic total_num is 0 !!!")
TRACE_MSG(MMIPICVIEW_WINTAB_1987_112_2_18_2_46_45_81,"MMIPICVIEW HandlePreviewOptWinMsg msg_id = 0x%x ")
TRACE_MSG(MMIPICVIEW_WINTAB_2125_112_2_18_2_46_45_82,"MMIPICVIEW HandlePreviewOptWinMsg:menu_id %d is error!!!")
TRACE_MSG(MMIPICVIEW_WINTAB_2536_112_2_18_2_46_46_83,"MMIPICVIEW SetSlideEffectIndex cur_index is overflow !!!")
TRACE_MSG(MMIPICVIEW_WINTAB_2589_112_2_18_2_46_46_84,"MMIPICVIEW SetSlideTimeIndex cur_index is overflow !!!")
TRACE_MSG(MMIPICVIEW_WINTAB_2993_112_2_18_2_46_47_85,"StopDelete")
TRACE_MSG(MMIPICVIEW_WINTAB_3585_112_2_18_2_46_48_86,"[MMIPIC]:SetDetail dev=%d,ERROR!!!!!!!!!")
TRACE_MSG(MMIPICVIEW_WINTAB_3635_112_2_18_2_46_48_87,"MMIPICVIEW AddFixStrItem index_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_WINTAB_3641_112_2_18_2_46_48_88,"MMIPICVIEW AddFixStrItem richtext_item_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_WINTAB_3671_112_2_18_2_46_48_89,"MMIPICVIEW AddStrItem str_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_WINTAB_3677_112_2_18_2_46_48_90,"MMIPICVIEW AddStrItem index_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_WINTAB_3683_112_2_18_2_46_48_91,"MMIPICVIEW AddStrItem richtext_item_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_WINTAB_3882_112_2_18_2_46_49_92,"MMIPICVIEW HandlePiclistLongokOptWinMsg:menu_id %d is error!!!")
TRACE_MSG(MMIPICVIEW_ZOOM_259_112_2_18_2_46_51_93,"MMIPICVIEW MMIPICVIEW_Zoom full_path_wstr_ptr is PNULL !!!")
TRACE_MSG(MMIPICVIEW_ZOOM_347_112_2_18_2_46_51_94,"MMIPICVIEW HandlePicZoomWinMsg MSG_TP_GESTURE type=%d")
TRACE_MSG(MMIPICVIEW_ZOOM_412_112_2_18_2_46_51_95,"MMIPICVIEW HandlePicZoomWinMsg MSG_PICVIEWER_MIN_ZOOM_OUT")
TRACE_MSG(MMIPICVIEW_ZOOM_854_112_2_18_2_46_52_96,"StartZoomOsdTimer:don't creat display timer")
TRACE_MSG(MMIPICVIEW_ZOOM_988_112_2_18_2_46_53_97,"MMIPICVIEW HandleZoomKeyMove:msg_id %d is error!!!")
TRACE_MSG(MMIPICVIEW_ZOOM_1101_112_2_18_2_46_53_98,"MMIPICVIEW ProcessPicState pic_state=%d")
TRACE_MSG(MMIPICVIEW_ZOOM_1115_112_2_18_2_46_53_99,"MMIPICVIEW ProcessPicState is_decoded=%d")
TRACE_MSG(MMIPICVIEW_ZOOM_1136_112_2_18_2_46_53_100,"MMIPICVIEW ProcessPicState zoom_out_counter=%d")
END_TRACE_MAP(MMI_APP_PIC_VIEWER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PIC_VIEWER_TRC_H_

