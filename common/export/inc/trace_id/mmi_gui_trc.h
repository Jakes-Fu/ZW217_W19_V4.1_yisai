/******************************************************************************
 ** File Name:      mmi_gui_trc.h                                         *
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
//trace_id:256
#ifndef _MMI_GUI_TRC_H_
#define _MMI_GUI_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define GUIANIM_976_112_2_18_3_14_50_0 "GUIANIM_SetParam:file name len is 0!"
#define GUIANIM_1190_112_2_18_3_14_50_1 "GUIANIM_AdjustDisplayPosition:align_style %d is error!"
#define GUIANIM_1428_112_2_18_3_14_51_2 "DecodeImgBufOrFile:anim rect is empty!"
#define GUIANIM_1565_112_2_18_3_14_51_3 "GetImageInfo: image type is %d"
#define GUIANIM_1599_112_2_18_3_14_51_4 "GetImageInfo:image is not support!"
#define GUIANIM_1683_112_2_18_3_14_51_5 "IsDecodeThumbnail:thumbnail %d*%d >= display %d*%d!"
#define GUIANIM_1718_112_2_18_3_14_51_6 "IsEncodeJpg:current disk is no enough free space!"
#define GUIANIM_1723_112_2_18_3_14_51_7 "IsEncodeJpg:don't creat miniature pic,resolution is %d*%d"
#define GUIANIM_1884_112_2_18_3_14_51_8 "SetDecEncTarget:no target buffer!"
#define GUIANIM_1915_112_2_18_3_14_52_9 "SetDecEncTarget:no gif backup buffer!"
#define GUIANIM_2197_112_2_18_3_14_52_10 "DecodeAnimOrImgRes:is not resource!"
#define GUIANIM_2251_112_2_18_3_14_52_11 "GetMiniaturFileName: miniature name len is too long!"
#define GUIANIM_2414_112_2_18_3_14_53_12 "IsTwoFileCorresponding: get miniature file size_time_len %d is error!"
#define GUIANIM_2553_112_2_18_3_14_53_13 "HandleAnimGetDataCnf:get data cnf,is_display=%d,handle=0x%x,result=%d,is_encode=%d,thumb=%d,frame=%d,end=%d!"
#define GUIANIM_2768_112_2_18_3_14_53_14 "SetImageInfoFail: multi frame gif error!"
#define GUIANIM_2837_112_2_18_3_14_53_15 "CreatJpgMiniature: generate miniature file fail!"
#define GUIANIM_2912_112_2_18_3_14_53_16 "GenerateMiniatureFile: write src file size and creat time error!"
#define GUIANIM_2917_112_2_18_3_14_54_17 "GenerateMiniatureFile: get file size and creat timer error!"
#define GUIANIM_2922_112_2_18_3_14_54_18 "GenerateMiniatureFile: write miniature file error!"
#define GUIANIM_2959_112_2_18_3_14_54_19 "GetFileSizeAndCreatTime:SFS_FindFirstFile file_handle %d is error!"
#define GUIANIM_3047_112_2_18_3_14_54_20 "HandleAnimTimer:only play anim once!"
#define GUIANIM_DISPLAY_266_112_2_18_3_14_58_21 "GUIANIM_Display:anim image rect and display rect are not crossed!"
#define GUIANIM_DISPLAY_293_112_2_18_3_14_58_22 "GUIANIM_Display:image is_decoded = %d"
#define GUIANIM_DISPLAY_303_112_2_18_3_14_58_23 "GUIANIM_Display:is not focus!"
#define GUIANIM_DISPLAY_485_112_2_18_3_14_58_24 "DisplayGifImg:only play gif once!"
#define GUIANIM_DISPLAY_621_112_2_18_3_14_58_25 "DisplayGifRes:display gif fail!"
#define GUIANIM_DISPLAY_668_112_2_18_3_14_58_26 "CopyOrDisplayBg:alloc bg buffer fail!"
#define GUIANIM_DISPLAY_699_112_2_18_3_14_59_27 "GUIANIM_StartGifTimer: time_out is %d!"
#define GUIANIM_DISPLAY_1063_112_2_18_3_14_59_28 "DisplayDefaultPicIcon:default_img_id is invalid!"
#define GUIANIM_DISPLAY_1079_112_2_18_3_15_0_29 "DisplayDefaultPicIcon:bg_type %d is error!"
#define GUIANIM_DISPLAY_1136_112_2_18_3_15_0_30 "DisplayDefaultPicIcon:anim image rect and display rect are not crossed!"
#define GUIANIM_DISPLAY_1142_112_2_18_3_15_0_31 "DisplayDefaultPicIcon:anim rect is empty!"
#define GUIANIM_REF_113_112_2_18_3_15_1_32 "GUIANIM_CreatHandle:creat handle=0x%x result is %d!"
#define GUIANIM_REF_269_112_2_18_3_15_1_33 "GUIANIM_GetFrame:get frame result is %d!"
#define GUIANIM_REF_337_112_2_18_3_15_2_34 "AsynGetFrameCallbackFunc:post get data cnf msg to MMI,img_handle=0x%x!"
#define GUIANIM_REF_375_112_2_18_3_15_2_35 "GUIANIM_DestroyHandle:destroy handle=0x%x result is %d!"
#define GUIANIM_REF_408_112_2_18_3_15_2_36 "DeleteGetDataCnf:handle=0x%x,img_handle=0x%x delete cnf msg!"
#define GUIANIM_REF_454_112_2_18_3_15_2_37 "GUIANIM_GetInfo:get info result is %d!"
#define GUIANIM_REF_638_112_2_18_3_15_2_38 "GUIANIM_ZoomImage:zoom result is %d!"
#define GUIANIM_REF_682_112_2_18_3_15_2_39 "GUIANIM_ZoomRange:zoom result is %d!"
#define GUIANIM_REF_748_112_2_18_3_15_2_40 "GUIANIM_RotateImage:rotate result is %d!"
#define GUIANIM_REF_843_112_2_18_3_15_3_41 "GUIANIM_RotateArbitrary:rotate result is %d!"
#define GUIANIM_REF_893_112_2_18_3_15_3_42 "GUIANIM_GetRotateSize:rotate result is %d!"
#define GUIANIM_ROTATE_227_112_2_18_3_15_3_43 "IsRotate:rotate_mode %d is not rotate!"
#define GUIANIM_ROTATE_263_112_2_18_3_15_3_44 "IsRotate:angle %d is not rotate!"
#define GUIANIM_ROTATE_391_112_2_18_3_15_4_45 "RotateImage:rotate fail!"
#define GUIANIM_ROTATE_489_112_2_18_3_15_4_46 "RotateMode:no rotate buffer!"
#define GUIANIM_ROTATE_573_112_2_18_3_15_4_47 "RotateArbitrary:no rotate buffer!"
#define GUIANIM_ZOOM_312_112_2_18_3_15_5_48 "GUIANIM_ZoomUtmost:ZoomUtmost fail!"
#define GUIANIM_ZOOM_413_112_2_18_3_15_5_49 "ZoomImage:no zoom buffer!"
#define GUIANIM_ZOOM_652_112_2_18_3_15_6_50 "GUIANIM_Zoom:zoom fail!"
#define GUIANIM_ZOOM_945_112_2_18_3_15_6_51 "GUIANIM_ZoomMove:move_x and move_y are zero!"
#define GUIBLOCK_762_112_2_18_3_15_8_52 "GUIBLOCK_SetHalfTransparent: enable block fail!"
#define GUIBLOCK_767_112_2_18_3_15_8_53 "GUIBLOCK_SetHalfTransparent: register block fail!"
#define GUIBLOCK_794_112_2_18_3_15_8_54 "== GUIBLOCK_GetInfo == lcd id %d is error"
#define GUIBLOCK_831_112_2_18_3_15_8_55 "== GUIBLOCK_GetBuf == block_buf_ptr is PNULL"
#define GUIBUTTON_898_112_2_18_3_15_10_56 "DisplayButtonFg:bg_type %d is error !"
#define GUICOMMON_1029_112_2_18_3_15_16_57 "GUI_DisplayBorder:border type %d is error!"
#define GUICOMMON_1121_112_2_18_3_15_17_58 "GUI_DisplayBorderToBuf:border type %d is error!"
#define GUICOMMON_1239_112_2_18_3_15_17_59 "GUI_DisplayBg:shape %d is error !"
#define GUICOMMON_1245_112_2_18_3_15_17_60 "GUI_DisplayBg:bg_type %d is error !"
#define GUICOMMON_1449_112_2_18_3_15_17_61 "GUI_DisplaySheen:get buffer info fail!"
#define GUICOMMON_1872_112_2_18_3_15_18_62 "== GUICOM TIMING START == %d"
#define GUICOMMON_1887_112_2_18_3_15_18_63 "== GUICOM TIMING ELAPSE == %s: %d, %d"
#define GUIDC_671_112_2_18_3_15_24_64 "GUIDC_SetLcdId Error input param!"
#define GUIDC_696_112_2_18_3_15_24_65 "GUIDC_SetBlockId Error input param!"
#define GUIDC_719_112_2_18_3_15_24_66 "GUIDC_SetOrigin Error input param!"
#define GUIDC_740_112_2_18_3_15_24_67 "GUIDC_SetRect Error input param!"
#define GUIDC_765_112_2_18_3_15_24_68 "GUIDC_SetFontType Error input param!"
#define GUIDC_788_112_2_18_3_15_24_69 "GUIDC_GetFontType Error input param!"
#define GUIDC_814_112_2_18_3_15_24_70 "GUIDC_SetFontEffect Error input param!"
#define GUIDC_837_112_2_18_3_15_24_71 "GUIDC_GetFontEffect Error input param!"
#define GUIDC_863_112_2_18_3_15_24_72 "GUIDC_SetFontColor Error input param!"
#define GUIDC_886_112_2_18_3_15_24_73 "GUIDC_GetFontColor Error input param!"
#define GUIDC_912_112_2_18_3_15_24_74 "GUIDC_SetTextAlign Error input param!"
#define GUIDC_935_112_2_18_3_15_24_75 "GUIDC_GetTextAlign Error input param!"
#define GUIDC_996_112_2_18_3_15_25_76 "GUIDC_SetPenSize Error input param!"
#define GUIDC_1019_112_2_18_3_15_25_77 "GUIDC_GetPenSize Error input param!"
#define GUIDC_1045_112_2_18_3_15_25_78 "GUIDC_SetPenColor Error input param!"
#define GUIDC_1068_112_2_18_3_15_25_79 "GUIDC_GetPenColor Error input param!"
#define GUIDC_1094_112_2_18_3_15_25_80 "GUIDC_SetPixel Error input param!"
#define GUIDC_1137_112_2_18_3_15_25_81 "GUIDC_GetPixel Error input param!"
#define GUIDC_1185_112_2_18_3_15_25_82 "GUIDC_DrawLine Error input param!"
#define GUIDC_1211_112_2_18_3_15_25_83 "GUIDC_DrawRect Error input param!"
#define GUIDC_1238_112_2_18_3_15_25_84 "GUIDC_FillRect Error input param!"
#define GUIDC_1621_112_2_18_3_15_26_85 "GUIDC_GetFontWidth dc_ptr == PNULL!"
#define GUIDC_1634_112_2_18_3_15_26_86 "GUIDC_GetFontHeight dc_ptr == PNULL!"
#define GUIDC_1647_112_2_18_3_15_26_87 "GUIDC_GetStringWidth dc_ptr == PNULL!"
#define GUIDC_1660_112_2_18_3_15_26_88 "GUIDC_GetStringHeight dc_ptr == PNULL!"
#define GUIDROPDOWNLIST_537_112_2_18_3_15_27_89 "DropDownListInit PNULL == init_ptr!"
#define GUIDROPDOWNLIST_613_112_2_18_3_15_28_90 "DropDownListConstruct PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_662_112_2_18_3_15_28_91 "DropDownListDestruct PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_984_112_2_18_3_15_28_92 "DropDownListHandleMsg PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_1383_112_2_18_3_15_29_93 "GUI GetDropdownlistRect: pos = %d"
#define GUIDROPDOWNLIST_1390_112_2_18_3_15_29_94 "GUI GetDropdownlistRect: PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_1450_112_2_18_3_15_29_95 "GUI DrawDropDownlistButton: dropdownlist_ptr->is_has_button %d!"
#define GUIDROPDOWNLIST_1470_112_2_18_3_15_29_96 "DropDownlistOnPenUp: PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_1534_112_2_18_3_15_29_97 "DropDownlistOnPenDown: PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_1573_112_2_18_3_15_30_98 "DropDownlistOnPenMove: PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_1615_112_2_18_3_15_30_99 "LoadDataToList: PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_1681_112_2_18_3_15_30_100 "GetPenPosition: PNULL INPUT PARAM!"
#define GUIDROPDOWNLIST_1725_112_2_18_3_15_30_101 "CalListRect error input param!"
#define GUIDROPDOWNLIST_1782_112_2_18_3_15_30_102 "CalListRect show type = %d error!"
#define GUIDROPDOWNLIST_1930_112_2_18_3_15_30_103 "GUIDROPDOWNLIST CreateListBox failed!"
#define GUIDROPDOWNLIST_1998_112_2_18_3_15_30_104 "GUIDROPDOWNLIST_AppendItemArray:array_size = %d is error!"
#define GUIDROPDOWNLIST_2004_112_2_18_3_15_30_105 "GUIDROPDOWNLIST_AppendItemArray item_array_ptr is NULL!"
#define GUIDROPDOWNLIST_2178_112_2_18_3_15_31_106 "DropdownInsertItemByPtr item_ptr is NULL!"
#define GUIDROPDOWNLIST_2184_112_2_18_3_15_31_107 "DropdownInsertItemByPtr list_ptr is NULL!"
#define GUIDROPDOWNLIST_2404_112_2_18_3_15_31_108 "DropdownSetMaxItemByPtr list_ptr is NULL!"
#define GUIDROPDOWNLIST_2418_112_2_18_3_15_31_109 "DropdownSetMaxItemByPtr error ctrl type!"
#define GUIDROPDOWNLIST_3189_112_2_18_3_15_33_110 "SetTextToEditCtrl"
#define GUIDROPDOWNLIST_3294_112_2_18_3_15_33_111 "CreateInnerEditCtrl result = %d, ctrl_handle = %d"
#define GUIDROPDOWNLIST_3298_112_2_18_3_15_33_112 "CreateInnerEditCtrl IN PARAM PNULL!"
#define GUIDROPDOWNLIST_3326_112_2_18_3_15_33_113 "ChangeDDLCtrlType old type = %d, new type = %d"
#define GUIDROPDOWNLIST_3339_112_2_18_3_15_33_114 "ChangeDDLCtrlType need not create edit handle"
#define GUIEDIT_1066_112_2_18_3_15_36_115 "GetEditImInfo:edit list style %d is error!"
#define GUIEDIT_1102_112_2_18_3_15_36_116 "GetEditImInfo:edit type %d is error!"
#define GUIEDIT_1346_112_2_18_3_15_36_117 "SetEditImInfo:edit list style %d is error!"
#define GUIEDIT_1445_112_2_18_3_15_37_118 "SetEditImInfo:edit type %d is error!"
#define GUIEDIT_2929_112_2_18_3_15_40_119 "EditAddStr: edit rect is wrong,not update!"
#define GUIEDIT_3092_112_2_18_3_15_40_120 "AddString:add underline string!"
#define GUIEDIT_3778_112_2_18_3_15_41_121 "HandleParseErrorStr: %d position string is error!"
#define GUIEDIT_4020_112_2_18_3_15_42_122 "HandleTextCancelKey:window intercept the cancel msg!"
#define GUIEDIT_4449_112_2_18_3_15_43_123 "HandleEditOtherMsg:edit type %d is error!"
#define GUIEDIT_4792_112_2_18_3_15_43_124 "IsValidItem:item_style%d is error!"
#define GUIEDIT_5263_112_2_18_3_15_44_125 "CheckEditString: edit type %d is error!"
#define GUIEDIT_5560_112_2_18_3_15_45_126 "CheckPasswordStr:password style %d is error!"
#define GUIEDIT_6847_112_2_18_3_15_48_127 "GUIEDIT_SetAlign:edit type %d is error!"
#define GUIEDIT_CURSOR_575_112_2_18_3_15_58_128 "GUIEDIT_MoveCursor:move_direction %d is error!"
#define GUIEDIT_DATE_758_112_2_18_3_16_1_129 "GetNewDate: date bit %d is error!"
#define GUIEDIT_DATE_802_112_2_18_3_16_2_130 "GetCurDateBit:date style %d is error!"
#define GUIEDIT_INTERNAL_467_112_2_18_3_16_5_131 "GUIEDIT_NotifyParentMsg:notify_msg = %d is error!"
#define GUIEDIT_INTERNAL_759_112_2_18_3_16_6_132 "GetEditAlign:edit type %d is error!"
#define GUIEDIT_INTERNAL_843_112_2_18_3_16_6_133 "GetSeparatorChar:separator style %d is error!"
#define GUIEDIT_INTERNAL_1147_112_2_18_3_16_7_134 "GUIEDIT_IsFixedFormat:edit type %d is error!"
#define GUIEDIT_TIME_619_112_2_18_3_16_19_135 "GetNewTime: time bit %d is error!"
#define GUIEDIT_TIME_682_112_2_18_3_16_19_136 "GetCurTimeBit: current cursor position %d is error!"
#define GUIEDIT_TIME_872_112_2_18_3_16_19_137 "AdjustMinOrSec:display style %d is error!"
#define GUIEDIT_TOUCH_645_112_2_18_3_16_22_138 "slide_state error: %d"
#define GUIEDIT_TOUCH_702_112_2_18_3_16_22_139 "slide_state error: %d"
#define GUIEDIT_TOUCH_747_112_2_18_3_16_22_140 "slide_state error: %d"
#define GUIEDIT_TOUCH_955_112_2_18_3_16_23_141 "slide_state error: %d"
#define GUIEDIT_TOUCH_1820_112_2_18_3_16_25_142 "== GetTouchInfo == y: %d, value: %d"
#define GUI_UCS2B_CONVERTER_338_112_2_18_3_16_57_143 "GUI_UTF82UCS2B: i %d char %x"
#define GUI_UCS2B_CONVERTER_535_112_2_18_3_16_57_144 "== GUI_WstrToOtherCode == code_type %d is error!"
#define GUI_UCS2B_CONVERTER_693_112_2_18_3_16_58_145 "== GUI_OtherCodeToWstr == code_type %d is error!"
#define GUI_UNICODE_167_112_2_18_3_17_0_146 "== GUI_GetCyrillicFontIndex == Cyrillic Index exceed"
#define GUIFONT_96_112_2_18_3_17_1_147 "== GUI_GetCusorCoordinate == param rect_ptr is PNULL"
#define GUIFONT_149_112_2_18_3_17_1_148 "== GUI_GetCursorInfo == param rect_ptr is PNULL"
#define GUIFORM_3774_112_2_18_3_18_23_149 "%s== form == "
#define GUIFORM_3847_112_2_18_3_18_23_150 "%schild_ptr=0x%x, index=%d, ctrl_ptr=0x%08x, id=0x%08x, guid=%s, "
#define GUIFORM_3881_112_2_18_3_18_23_151 "%sindex=%d, id=0x%08x, guid=%s"
#define GUIFORM_3898_112_2_18_3_18_23_152 "%sindex=%d, id=0x%08x, guid=%s"
#define GUIFORM_3915_112_2_18_3_18_23_153 "%sindex=%d, id=0x%08x, guid=%s"
#define GUIFORM_3932_112_2_18_3_18_23_154 "%sindex=%d, id=0x%08x, guid=%s"
#define GUIFORM_3955_112_2_18_3_18_23_155 "==============   Dump form control basic tree   =============="
#define GUIFORM_3957_112_2_18_3_18_23_156 "==============================================================\n\n"
#define GUIFORM_3959_112_2_18_3_18_23_157 "==============   Dump form control tree   =============="
#define GUIFORM_3961_112_2_18_3_18_23_158 "==============================================================\n\n"
#define GUIFORM_CALC_675_112_2_18_3_18_24_159 "CalChildHeight: height type %d is error!"
#define GUIFORM_LAYOUT_520_112_2_18_3_18_29_160 "AdjustFormRelativeRect:layout_type %d is error!"
#define GUIFORM_LAYOUT_611_112_2_18_3_18_29_161 "ModifyPosByAlign:align %d is error!"
#define GUIFORM_LAYOUT_1100_112_2_18_3_18_30_162 "GetChildRelativeRect:layout_type %d is error!"
#define GUIFORM_LAYOUT_1416_112_2_18_3_18_31_163 "DestroyPrgCtrl:layout_type %d is error!"
#define GUIFORM_MOVE_255_112_2_18_3_18_33_164 "GUIFORM_GetDisplayTopForKey: order form don't handle msg %d"
#define GUIFORM_MSG_338_112_2_18_3_18_35_165 "TpDownCtrlHandleTpMsg:tp down control is not focus!"
#define GUIREF_SCALE_239_112_2_18_3_18_41_166 "GUIREF_TvScalingDown:s_is_clear_osd = %d,i = %d!"
#define GUIREF_SCALE_278_112_2_18_3_18_41_167 "GUIREF_TvScalingDown1:i = %d,scale_start_x = %d,scale_end_x = %d,start_y = %d,end_y = %d"
#define GUIREF_SCALE_337_112_2_18_3_18_41_168 "GUIREF_GetBlockBufferBeforeScale:lcd_id = %d,block_id = %d,s_tv_block_buffer_addr[block_id] = 0x%x"
#define GUIREF_SCALE_363_112_2_18_3_18_42_169 "GUIREF_SetBlockBufferBeforeScale:lcd_id = %d,block_id = %d,block_buffer_addr = 0x%x"
#define GUIREF_SCALE_397_112_2_18_3_18_42_170 "GetTvOutputDataPtr1:s_is_used_memory1[block_id] = %d,is_copy = %d,color_key = 0x%x"
#define GUIREF_SCALE_413_112_2_18_3_18_42_171 "GetTvOutputDataPtr2:s_is_used_memory1[block_id] = %d,is_copy = %d,color_key = 0x%x"
#define GUIREF_SCALE_498_112_2_18_3_18_42_172 "GUIREF_SetBlockCfgBeforeScale:lcd_id = %d,block_id = %d,start_x = %d,end_x = %d,start_y = %d,end_y = %d"
#define GUIGRAPH_2630_112_2_18_3_18_48_173 "== my_mem16set_win32 == param is error: pline=0x%x, byte_num=%d"
#define GUIGRAPH_2697_112_2_18_3_18_48_174 "== MainLcdFillRect == the box is error!"
#define GUIICONLIST_2770_112_2_18_3_19_8_175 "GetNextIndex:msg_id %d is error!"
#define GUIICONLIST_5711_112_2_18_3_19_14_176 "IconListCtrlAppendIcon d%"
#define GUIICONLIST_INTERNAL_265_112_2_18_3_19_16_177 "GUIICONLIST_GetItemWidth:icon list style %d is error!"
#define GUIICONLIST_INTERNAL_323_112_2_18_3_19_17_178 "GUIICONLIST_GetItemHeight:icon list style %d is error!"
#define GUIICONLIST_INTERNAL_352_112_2_18_3_19_17_179 "GUIICONLIST_GetItemHspace:icon list style %d is error!"
#define GUIICONLIST_INTERNAL_381_112_2_18_3_19_17_180 "GUIICONLIST_GetItemVspace:icon list style %d is error!"
#define GUIICONLIST_INTERNAL_410_112_2_18_3_19_17_181 "GUIICONLIST_GetIconWidth:icon list style %d is error!"
#define GUIICONLIST_INTERNAL_439_112_2_18_3_19_17_182 "GUIICONLIST_GetIconHeight:icon list style %d is error!"
#define GUIICONLIST_INTERNAL_835_112_2_18_3_19_18_183 "GUIICONLIST_GetItemHeight:icon list style %d is error!"
#define GUIICONLIST_INTERNAL_1116_112_2_18_3_19_18_184 "GUIICONLIST_GetLayoutStyle:iconlist LayoutStyle %d is error!"
#define GUIIM_1361_112_2_18_3_19_27_185 "GUIIM: Setting wrong. lang = %#x"
#define GUIIM_1370_112_2_18_3_19_27_186 "GUIIM: Setting wrong. lang = %#x. Undef method: %#x"
#define GUIIM_1418_112_2_18_3_19_27_187 "GUIIM: Setting wrong, not in set. input = %#x"
#define GUIIM_1429_112_2_18_3_19_27_188 "GUIIM: Setting wrong. input = %#x"
#define GUIIM_1442_112_2_18_3_19_27_189 "GUIIM: Can not get api. input = %#x. Undef method: %#x"
#define GUIIM_1502_112_2_18_3_19_27_190 "GUIIM: Setting wrong, not in set. type = %#x"
#define GUIIM_1513_112_2_18_3_19_27_191 "GUIIM: Setting wrong. type = %#x"
#define GUIIM_1522_112_2_18_3_19_27_192 "GUIIM: Setting wrong. type = %#x. Undef method: %#x"
#define GUIIM_1569_112_2_18_3_19_27_193 "GUIIM: Setting wrong. cap = %#x"
#define GUIIM_1578_112_2_18_3_19_27_194 "GUIIM: Setting wrong. cap = %#x. Undef method: %#x"
#define GUIIM_1623_112_2_18_3_19_27_195 "GUIIM: Setting wrong. cap = %#x"
#define GUIIMG_152_112_2_18_3_19_30_196 "GUIIMG_Decode: decode_data_size 0x%x < target_buf_size 0x%x"
#define GUIIMG_186_112_2_18_3_19_30_197 "GUIIMG_GetImgInfo:get info result is %d!"
#define GUIIMG_592_112_2_18_3_19_31_198 "GUIIMG_DisplayBmp:display rect is not on lcd!"
#define GUIIMG_618_112_2_18_3_19_31_199 "GUIIMG_DisplayBmp:image rect is not cross with show rect!"
#define GUIIMG_906_112_2_18_3_19_32_200 "GUIIMG_DisplayBmp:display rect is not on lcd!"
#define GUIIMG_932_112_2_18_3_19_32_201 "GUIIMG_DisplayBmp:image rect is not cross with show rect!"
#define GUILABEL_1208_112_2_18_3_19_35_202 "GetLabelAlign:label_align %d is error!"
#define GUILCD_128_112_2_18_3_19_38_203 "GUILCD_Init:init lcd %d is fail!"
#define GUILCD_169_112_2_18_3_19_38_204 "InitLcdLogicAngle:init lcd %d is fail!"
#define GUILCD_452_112_2_18_3_19_38_205 "InvalidateLcdRect.c UILAYER_GetBlendInfo failed!"
#define GUILCD_676_112_2_18_3_19_39_206 "GUILCD_GetLogicWidthHeight: logic angle %d is error!"
#define GUILCD_760_112_2_18_3_19_39_207 "== GetInvalidateBlockInfo == can get lcd id %d info"
#define GUILISTBOX_3392_112_2_18_3_19_48_208 "GetEffectContentStr SCI_ALLOCA error!"
#define GUILISTBOX_3402_112_2_18_3_19_48_209 "GetEffectContentStr error!"
#define GUILISTBOX_4832_112_2_18_3_19_51_210 "InitScrollBar list_ptr->prgbox_ptr = PNULL!"
#define GUILISTBOX_5230_112_2_18_3_19_52_211 "GUILIST_GetItemState PNULL == item_ptr!"
#define GUILISTBOX_7163_112_2_18_3_19_56_212 "HandleListItemSlide first!"
#define GUILISTBOX_7729_112_2_18_3_19_57_213 "GUILIST_StartSwitchItem NULL LISt!"
#define GUILISTBOX_7768_112_2_18_3_19_57_214 "GUILIST_StartSwitchItem NULL LISt!"
#define GUILISTBOX_7789_112_2_18_3_19_57_215 "GUILIST_StopSwitchItem NULL LISt!"
#define GUILISTBOX_8469_112_2_18_3_19_59_216 "GUILIST_Construct init param error!"
#define GUILISTBOX_8492_112_2_18_3_19_59_217 "GUILIST_Destruct list handle = 0x%0x"
#define GUILISTBOX_8612_112_2_18_3_19_59_218 "GUILIST_HandleMsg invalid ctrl type!"
#define GUILISTBOX_10076_112_2_18_3_20_3_219 "GUILIST_Init: init param error!"
#define GUILISTBOX_10093_112_2_18_3_20_3_220 "GUILIST_Init: list_ptr = %x"
#define GUILISTBOX_10212_112_2_18_3_20_3_221 "GUILIST_SetMaxItemByPtr error ctrl type!"
#define GUILISTBOX_10257_112_2_18_3_20_3_222 "GUILIST_SetMaxSelectedItemByPtr: list_ptr->type %d is error!"
#define GUILISTBOX_10264_112_2_18_3_20_3_223 "GUILIST_SetMaxSelectedItemByPtr: error ctrl type!"
#define GUILISTBOX_10616_112_2_18_3_20_4_224 "GUILIST_SetItemContentByPtr: type = %d!"
#define GUILISTBOX_13845_112_2_18_3_20_10_225 "ReviceListDisplayItem 1 Error!"
#define GUILISTBOX_13866_112_2_18_3_20_10_226 "ReviceListDisplayItem 2 Error!"
#define GUILISTBOX_14646_112_2_18_3_20_12_227 "HandleListTpUp %f"
#define GUILISTBOX_14889_112_2_18_3_20_12_228 "HandleListTpLong slide state = %d"
#define GUIMENU_1304_112_2_18_3_20_19_229 "MenuHandleMsg %d, %d"
#define GUIMENU_1325_112_2_18_3_20_19_230 "MenuHandleMsg %d, %d"
#define GUIMENU_1332_112_2_18_3_20_19_231 "MenuHandleMsg %d, %d"
#define GUIMENU_1374_112_2_18_3_20_20_232 "MenuHandleMsg MSG_APP_CANCEL %d, %d"
#define GUIMENU_1396_112_2_18_3_20_20_233 "MenuHandleMsg MSG_APP_UP %d, %d"
#define GUIMENU_1416_112_2_18_3_20_20_234 "MenuHandleMsg MSG_APP_1 %d, %d"
#define GUIMENU_1437_112_2_18_3_20_20_235 "MenuHandleMsg MSG_APP_HASH %d, %d"
#define GUIMENU_1548_112_2_18_3_20_20_236 "MenuHandleMsg MSG_APP_CANCEL %d, %d"
#define GUIMENU_2125_112_2_18_3_20_21_237 "GetCurItemPtr:menu_style = %d is error!"
#define GUIMENU_2297_112_2_18_3_20_22_238 "== NotifyParentMsg == msg_id %d is not handled"
#define GUIMENU_2340_112_2_18_3_20_22_239 "PushMenuStack: menu stack %d is full!"
#define GUIMENU_2374_112_2_18_3_20_22_240 "PopMenuStack: menu stack %d is null!"
#define GUIMENU_2703_112_2_18_3_20_23_241 "== HandleMenuDirectionKey == msg_id %d is not handled"
#define GUIMENU_3281_112_2_18_3_20_24_242 "== GetMenuPtr == ctrl_handle 0x%x is not menu"
#define GUIMENU_3614_112_2_18_3_20_24_243 "== GUIMENU_CreatDynamic == menu_style %d is error!"
#define GUIMENU_3622_112_2_18_3_20_24_244 "== GUIMENU_CreatDynamic == rect_ptr is PNULL"
#define GUIMENU_4028_112_2_18_3_20_25_245 "== GUIMENU_GetCurNodeId == error when GetNodeByVisibleIndex"
#define GUIMENU_4074_112_2_18_3_20_25_246 "GUIMENU_SetPopItemStatus:selected item has exist"
#define GUIMENU_4081_112_2_18_3_20_25_247 "GUIMENU_SetPopItemStatus:selected item not exist"
#define GUIMENU_6878_112_2_18_3_20_31_248 "GUIMENU_SetItemVisibleByNum error: visible: %d , ptr: %d"
#define GUIMSGBOX_1200_112_2_18_3_20_34_249 "gui warning:ShowMsgbox: msgbox_ptr = PNULL "
#define GUIMSGBOX_1273_112_2_18_3_20_34_250 "gui warning:ShowMsgBoxText: text_info_ptr = PNULL "
#define GUIMSGBOX_1350_112_2_18_3_20_34_251 "gui warning:HandleMsgBoxLoseFocus: msgbox_ptr = PNULL "
#define GUIPRGBOX_382_112_2_18_3_20_38_252 "GUIPRGBOX_Init, alloc memory, prgbox_ctrl_ptr = %x"
#define GUIPRGBOX_972_112_2_18_3_20_39_253 "DisplayVerticalScroll bg type = %d"
#define GUIPRGBOX_2162_112_2_18_3_20_41_254 "[baokun] TP MOVE tp_point.y %d cur_item_index %d"
#define GUIRES_IMG_492_112_2_18_3_20_46_255 "GUIRES_GetImgInfo:img_type %d is error!"
#define GUIRES_IMG_498_112_2_18_3_20_46_256 "GUIRES_GetImgInfo:res_result = %d"
#define GUIRES_IMG_534_112_2_18_3_20_46_257 "== GUIRES_IsArgb == error when MMI_GetLabelImage"
#define GUIRES_IMG_565_112_2_18_3_20_46_258 "== GUIRES_IsSabm == error when MMI_GetLabelImage"
#define GUIRES_IMG_604_112_2_18_3_20_46_259 "== GUIRES_GetImgWidthHeight == error when MMI_GetLabelImage"
#define GUIRES_IMG_736_112_2_18_3_20_46_260 "== GUIRES_DisplayImg == error when MMI_GetLabelImage"
#define GUIRES_IMG_803_112_2_18_3_20_46_261 "== GUIRES_DisplaySabmImg == error when MMI_GetLabelImage"
#define GUIRES_IMG_858_112_2_18_3_20_46_262 "== GUIRES_DisplayImgEx == error when MMI_GetLabelImage"
#define GUIRES_IMG_919_112_2_18_3_20_46_263 "== GUIRES_DisplayImgWitchColorExchange == error when MMI_GetLabelImage"
#define GUIRES_IMG_1043_112_2_18_3_20_47_264 "GUIRES_DisplayImg:display image res_result = %d"
#define GUIRES_IMG_1050_112_2_18_3_20_47_265 "GUIRES_DisplayImg:get image info res_result = %d"
#define GUIRES_IMG_1182_112_2_18_3_20_47_266 "GetImgRect:rect = {%d,%d,%d,%d} is error!"
#define GUIRES_IMG_1305_112_2_18_3_20_47_267 "GetDisplayCtl: display block buffer is PNULL!"
#define GUIRES_IMG_1376_112_2_18_3_20_48_268 "AdjustImgAndDisRect:rect = {%d,%d,%d,%d} is error!"
#define GUIRES_IMG_1415_112_2_18_3_20_48_269 "== GUIRES_GetAnimWidthHeight == error when MMI_GetLabelAnim"
#define GUIRES_IMG_1482_112_2_18_3_20_48_270 "== GUIRES_DisplayAnim == error when MMI_GetLabelAnim"
#define GUIRES_IMG_1534_112_2_18_3_20_48_271 "== GUIRES_DisplayAnimEx == error when MMI_GetLabelAnim"
#define GUIRES_IMG_1684_112_2_18_3_20_48_272 "DisplayAnim:display anim res_result = %d"
#define GUIRES_IMG_1689_112_2_18_3_20_48_273 "DisplayAnim:creat handle res_result = %d"
#define GUIRES_IMG_1699_112_2_18_3_20_48_274 "DisplayAnim:free handle res_result = %d"
#define GUIRES_IMG_1707_112_2_18_3_20_48_275 "DisplayAnim:get anim info res_result = %d!"
#define GUIRES_IMG_1982_112_2_18_3_20_49_276 "GUIRES_CopyImgBuf:display image res_result = %d"
#define GUIRES_IMG_1990_112_2_18_3_20_49_277 "GUIRES_CopyImgBuf:get image info res_result = %d"
#define GUIRES_IMG_2028_112_2_18_3_20_49_278 "GUIRES_CreatHandle:creat handle res_result = %d"
#define GUIRES_IMG_2056_112_2_18_3_20_49_279 "GUIRES_FreeHandle:free handle res_result = %d"
#define GUIRES_IMG_2090_112_2_18_3_20_49_280 "== GUIRES_DisplayGif == param is error"
#define GUIRES_IMG_2144_112_2_18_3_20_49_281 "== GUIRES_DisplayGifEx == param is error"
#define GUIRES_IMG_2296_112_2_18_3_20_49_282 "GUIRES_DisplayImg:get image info res_result = %d"
#define GUIRES_IMG_2348_112_2_18_3_20_49_283 "GUIRES_DisplayImg:display image res_result = %d"
#define GUIRICHTEXT_5919_112_2_18_3_21_2_284 "richtext copy failed, malloc size: %d."
#define GUISCROLLKEY_546_112_2_18_3_21_12_285 "GUISCROLLKEY: scrollkey_ptr->bg_type error!"
#define GUISCROLLKEY_747_112_2_18_3_21_13_286 "DrawButtonContent invalid type = %d!"
#define GUISCROLLKEY_1452_112_2_18_3_21_14_287 "GUISCROLLKEY_SetTextId: the win_handle = %d"
#define GUISCROLLKEY_1516_112_2_18_3_21_15_288 "GUISCROLLKEY_SetButtonTextId: the win_handle = %d"
#define GUISCROLLKEY_1555_112_2_18_3_21_15_289 "GUISCROLLKEY_SetButtonTextId: the win_handle = %d"
#define GUISCROLLKEY_1615_112_2_18_3_21_15_290 "GUISCROLLKEY_SetButtonIconId: the win_handle = %d"
#define GUISCROLLKEY_1858_112_2_18_3_21_15_291 "GUISCROLLKEY_SetButtonState(),win_handle = %d, state = %d"
#define GUISETLIST_1281_112_2_18_3_21_19_292 "InsertItem error ctrl type!"
#define GUISETLIST_1337_112_2_18_3_21_20_293 "AddItem error ctrl type!"
#define GUISETLIST_1523_112_2_18_3_21_20_294 "GetImageDispRect error input param!"
#define GUISETLIST_1591_112_2_18_3_21_20_295 "GetImageDispRect: not surport image!"
#define GUISETLIST_1627_112_2_18_3_21_20_296 "GetTextRect error input param!"
#define GUISETLIST_1676_112_2_18_3_21_20_297 "GetTextRect: not surport text!"
#define GUISETLIST_1719_112_2_18_3_21_20_298 "GetArrowRect error input param!"
#define GUISETLIST_1863_112_2_18_3_21_21_299 "DrawSetlistFrame error input param!"
#define GUISETLIST_1882_112_2_18_3_21_21_300 "AdjustSetlistRect error input param!"
#define GUISETLIST_1933_112_2_18_3_21_21_301 "DestroySetlistItemData: item_data_type %d is error!"
#define GUISETLIST_2053_112_2_18_3_21_21_302 "DrawItemImage: type %d is not image!"
#define GUISETLIST_2655_112_2_18_3_21_22_303 "HandleSetListTpUp NULL!"
#define GUISETLIST_2710_112_2_18_3_21_23_304 "HandleSetListLeftKeyDown error input param!"
#define GUISETLIST_2752_112_2_18_3_21_23_305 "HandleSetListRightKeyDown error input param!"
#define GUISETLIST_2794_112_2_18_3_21_23_306 "HandleSetListWebKeyDown error input param!"
#define GUISETLIST_3112_112_2_18_3_21_23_307 "GUISETLIST_HandleMessage error ctrl type!"
#define GUISETLIST_3385_112_2_18_3_21_24_308 "GUISETLIST_CallBack MMI_SETLIST_SET_ITEM_STYLE %d"
#define GUISETLIST_3460_112_2_18_3_21_24_309 "GUISETLIST_CallBack MMI_SETLIST_IS_OPEN_FULL_WIN %d"
#define GUISOFTKEY_528_112_2_18_3_21_26_310 "GUISOFTKEY: softkey_ptr->bg_type error!"
#define GUISOFTKEY_706_112_2_18_3_21_27_311 "DrawButtonContent invalid type = %d!"
#define GUISOFTKEY_1558_112_2_18_3_21_29_312 "GUISOFTKEY_SetTextId: the win_handle = %d"
#define GUISOFTKEY_1701_112_2_18_3_21_29_313 "GUISOFTKEY_SetButtonTextId: the win_handle = %d"
#define GUISOFTKEY_1790_112_2_18_3_21_29_314 "GUISOFTKEY_SetButtonTextId: the win_handle = %d"
#define GUISOFTKEY_1892_112_2_18_3_21_29_315 "GUISOFTKEY_SetButtonIconId: the win_handle = %d"
#define GUISOFTKEY_1953_112_2_18_3_21_29_316 "GUISOFTKEY_SetStyle() not support!"
#define GUISOFTKEY_2030_112_2_18_3_21_30_317 "GUISOFTKEY_SetBgType() not support!"
#define GUISOFTKEY_2066_112_2_18_3_21_30_318 "GUISOFTKEY_Setfont() not support!"
#define GUISOFTKEY_2165_112_2_18_3_21_30_319 "GUISOFTKEY_SetButtonState(),win_handle = %d, state = %d"
#define GUISOFTKEY_2254_112_2_18_3_21_30_320 "GUISOFTKEY_GetTextId() not support!"
#define GUISOFTKEY_2330_112_2_18_3_21_30_321 "GUISOFTKEY_IsTPEnable() not support!"
#define GUISOFTKEY_2364_112_2_18_3_21_30_322 "GUISOFTKEY_SetTPEnable() not support!"
#define GUISOFTKEY_BUTTON_124_112_2_18_3_21_31_323 "InitButtonCtrl() invalid param!"
#define GUISOFTKEY_BUTTON_151_112_2_18_3_21_31_324 "GUIBUTTONSOFTKEY_Init() invalid param!"
#define GUISOFTKEY_BUTTON_207_112_2_18_3_21_31_325 "GUIBUTTONOFTKEY_Update() invalid param!"
#define GUISOFTKEY_BUTTON_230_112_2_18_3_21_31_326 "GUIBUTTONSOFTKEY_SetVisible() invalid param!"
#define GUISOFTKEY_BUTTON_253_112_2_18_3_21_31_327 "GUIBUTTONSOFTKEY_SetButtonState() invalid param!"
#define GUISOFTKEY_BUTTON_274_112_2_18_3_21_32_328 "GUIBUTTONSOFTKEY_SetButtonState() invalid param!"
#define GUISOFTKEY_BUTTON_297_112_2_18_3_21_32_329 "GUIBUTTONSOFTKEY_SetTextId() invalid param!"
#define GUISTATUSBAR_319_112_2_18_3_21_33_330 "GUI STATUSBAR: ctrl data error!"
#define GUISTATUSBAR_653_112_2_18_3_21_34_331 "CalculateItemRect 1 item_data_ptr = PNULL, %d"
#define GUISTATUSBAR_740_112_2_18_3_21_34_332 "CalculateItemRect 2 item_data_ptr = PNULL, %d"
#define GUISTATUSBAR_935_112_2_18_3_21_34_333 "CalculateItemRect 1 item_data_ptr = PNULL, %d"
#define GUISTATUSBAR_1028_112_2_18_3_21_35_334 "CalculateItemRect 2 item_data_ptr = PNULL, %d"
#define GUISTATUSBAR_1079_112_2_18_3_21_35_335 "DrawStatusBarBg error input param!"
#define GUISTATUSBAR_1149_112_2_18_3_21_35_336 "DrawStatusBarBg status bar background type = %d!"
#define GUISTATUSBAR_1190_112_2_18_3_21_35_337 "DrawStatusBarAllItem error input param!"
#define GUISTATUSBAR_1395_112_2_18_3_21_35_338 "DrawStatusBarScrollItem error input param!"
#define GUISTATUSBAR_1675_112_2_18_3_21_36_339 "HandleTimer statusbar_ptr->is_shown = %d"
#define GUISTATUSBAR_1690_112_2_18_3_21_36_340 "HandleTimer item_index = %d"
#define GUISTATUSBAR_1900_112_2_18_3_21_36_341 "StatusBarConstruct error init data!"
#define GUISTATUSBAR_1942_112_2_18_3_21_37_342 "HandleMessage input error param!"
#define GUISTATUSBAR_DATA_117_112_2_18_3_21_38_343 "InsertItemData PNULL == item_ptr!"
#define GUISTATUSBAR_DATA_123_112_2_18_3_21_38_344 "InsertItemData PNULL == item_data_ptr!"
#define GUISTATUSBAR_SCROLLMSG_305_112_2_18_3_21_40_345 "GUISTBSCROLLMSG_AppendScrollItem() item_ptr = 0x%x"
#define GUISTRING_538_112_2_18_3_21_42_346 "GetYPosition invalid input param!"
#define GUISTRING_590_112_2_18_3_21_42_347 "[GetYPosition guistring.c] len = %d, str_ptr = %x"
#define GUISTRING_929_112_2_18_3_21_43_348 "error! [DrawTextInRect] len = %d"
#define GUITAB_839_112_2_18_3_21_50_349 "TabConstruct init param error!"
#define GUITAB_882_112_2_18_3_21_50_350 "TabDestruct input param error!"
#define GUITAB_3029_112_2_18_3_21_54_351 "DrawItemAnim type error! type = %d"
#define GUITAB_3075_112_2_18_3_21_55_352 "GUITAB: DrawChildWindow no focus window!"
#define GUITAB_3443_112_2_18_3_21_55_353 "GUITAB_Init init param null!"
#define GUITAB_3692_112_2_18_3_21_56_354 "MoveToNextValidItem index = %d, total num = %d, valid num = %d"
#define GUITAB_3724_112_2_18_3_21_56_355 "MoveToNextValidItem error type! %d"
#define GUITAB_3809_112_2_18_3_21_56_356 "HandleTabTpDown:tp point not in tab valid rect!"
#define GUITAB_4111_112_2_18_3_21_57_357 "GetMoveItemNum error! type = %d"
#define GUITAB_4480_112_2_18_3_21_57_358 "HandleTabMsg error ctrl type!"
#define GUITAB_4925_112_2_18_3_21_58_359 "guitab.c: AllocAllItemsSpace failed"
#define GUITAB_5040_112_2_18_3_21_58_360 "GUITAB_AppendItemByPtr error ctrl type!"
#define GUITAB_5070_112_2_18_3_21_58_361 "GUITAB_InsertItemByPtr error ctrl type!"
#define GUITAB_5113_112_2_18_3_21_59_362 "GUITAB_AppendSimpleItem input param error!"
#define GUITAB_5232_112_2_18_3_21_59_363 "GUITAB_DeleteAllItems cur num = %d, total num = %d, valid num = %d"
#define GUITAB_5436_112_2_18_3_21_59_364 "item state = %d, item index = %d"
#define GUITAB_5660_112_2_18_3_22_0_365 "GUITAB_SetItemImagePtr: type %d is not anim!"
#define GUITAB_6211_112_2_18_3_22_1_366 "SetScrollCardState type error! %d"
#define GUITAB_6233_112_2_18_3_22_1_367 "GUITAB_SwitchToNextItem FALSE!"
#define GUITAB_6265_112_2_18_3_22_1_368 "GUITAB_SwitchToNextItem type error! %d"
#define GUITITLE_1041_112_2_18_3_22_22_369 "GUITITLE_SetTextScrollType: the type is error!"
#define GUITITLE_1618_112_2_18_3_22_23_370 "DisplayScrollTitleByOffset: the param is error!"
#define GUITITLE_2230_112_2_18_3_22_24_371 "DisplayButtonText: text str is NULL!"
#define GUITITLE_2592_112_2_18_3_22_25_372 "DisplayTitle: text str is NULL!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_GUI_TRC)
TRACE_MSG(GUIANIM_976_112_2_18_3_14_50_0,"GUIANIM_SetParam:file name len is 0!")
TRACE_MSG(GUIANIM_1190_112_2_18_3_14_50_1,"GUIANIM_AdjustDisplayPosition:align_style %d is error!")
TRACE_MSG(GUIANIM_1428_112_2_18_3_14_51_2,"DecodeImgBufOrFile:anim rect is empty!")
TRACE_MSG(GUIANIM_1565_112_2_18_3_14_51_3,"GetImageInfo: image type is %d")
TRACE_MSG(GUIANIM_1599_112_2_18_3_14_51_4,"GetImageInfo:image is not support!")
TRACE_MSG(GUIANIM_1683_112_2_18_3_14_51_5,"IsDecodeThumbnail:thumbnail %d*%d >= display %d*%d!")
TRACE_MSG(GUIANIM_1718_112_2_18_3_14_51_6,"IsEncodeJpg:current disk is no enough free space!")
TRACE_MSG(GUIANIM_1723_112_2_18_3_14_51_7,"IsEncodeJpg:don't creat miniature pic,resolution is %d*%d")
TRACE_MSG(GUIANIM_1884_112_2_18_3_14_51_8,"SetDecEncTarget:no target buffer!")
TRACE_MSG(GUIANIM_1915_112_2_18_3_14_52_9,"SetDecEncTarget:no gif backup buffer!")
TRACE_MSG(GUIANIM_2197_112_2_18_3_14_52_10,"DecodeAnimOrImgRes:is not resource!")
TRACE_MSG(GUIANIM_2251_112_2_18_3_14_52_11,"GetMiniaturFileName: miniature name len is too long!")
TRACE_MSG(GUIANIM_2414_112_2_18_3_14_53_12,"IsTwoFileCorresponding: get miniature file size_time_len %d is error!")
TRACE_MSG(GUIANIM_2553_112_2_18_3_14_53_13,"HandleAnimGetDataCnf:get data cnf,is_display=%d,handle=0x%x,result=%d,is_encode=%d,thumb=%d,frame=%d,end=%d!")
TRACE_MSG(GUIANIM_2768_112_2_18_3_14_53_14,"SetImageInfoFail: multi frame gif error!")
TRACE_MSG(GUIANIM_2837_112_2_18_3_14_53_15,"CreatJpgMiniature: generate miniature file fail!")
TRACE_MSG(GUIANIM_2912_112_2_18_3_14_53_16,"GenerateMiniatureFile: write src file size and creat time error!")
TRACE_MSG(GUIANIM_2917_112_2_18_3_14_54_17,"GenerateMiniatureFile: get file size and creat timer error!")
TRACE_MSG(GUIANIM_2922_112_2_18_3_14_54_18,"GenerateMiniatureFile: write miniature file error!")
TRACE_MSG(GUIANIM_2959_112_2_18_3_14_54_19,"GetFileSizeAndCreatTime:SFS_FindFirstFile file_handle %d is error!")
TRACE_MSG(GUIANIM_3047_112_2_18_3_14_54_20,"HandleAnimTimer:only play anim once!")
TRACE_MSG(GUIANIM_DISPLAY_266_112_2_18_3_14_58_21,"GUIANIM_Display:anim image rect and display rect are not crossed!")
TRACE_MSG(GUIANIM_DISPLAY_293_112_2_18_3_14_58_22,"GUIANIM_Display:image is_decoded = %d")
TRACE_MSG(GUIANIM_DISPLAY_303_112_2_18_3_14_58_23,"GUIANIM_Display:is not focus!")
TRACE_MSG(GUIANIM_DISPLAY_485_112_2_18_3_14_58_24,"DisplayGifImg:only play gif once!")
TRACE_MSG(GUIANIM_DISPLAY_621_112_2_18_3_14_58_25,"DisplayGifRes:display gif fail!")
TRACE_MSG(GUIANIM_DISPLAY_668_112_2_18_3_14_58_26,"CopyOrDisplayBg:alloc bg buffer fail!")
TRACE_MSG(GUIANIM_DISPLAY_699_112_2_18_3_14_59_27,"GUIANIM_StartGifTimer: time_out is %d!")
TRACE_MSG(GUIANIM_DISPLAY_1063_112_2_18_3_14_59_28,"DisplayDefaultPicIcon:default_img_id is invalid!")
TRACE_MSG(GUIANIM_DISPLAY_1079_112_2_18_3_15_0_29,"DisplayDefaultPicIcon:bg_type %d is error!")
TRACE_MSG(GUIANIM_DISPLAY_1136_112_2_18_3_15_0_30,"DisplayDefaultPicIcon:anim image rect and display rect are not crossed!")
TRACE_MSG(GUIANIM_DISPLAY_1142_112_2_18_3_15_0_31,"DisplayDefaultPicIcon:anim rect is empty!")
TRACE_MSG(GUIANIM_REF_113_112_2_18_3_15_1_32,"GUIANIM_CreatHandle:creat handle=0x%x result is %d!")
TRACE_MSG(GUIANIM_REF_269_112_2_18_3_15_1_33,"GUIANIM_GetFrame:get frame result is %d!")
TRACE_MSG(GUIANIM_REF_337_112_2_18_3_15_2_34,"AsynGetFrameCallbackFunc:post get data cnf msg to MMI,img_handle=0x%x!")
TRACE_MSG(GUIANIM_REF_375_112_2_18_3_15_2_35,"GUIANIM_DestroyHandle:destroy handle=0x%x result is %d!")
TRACE_MSG(GUIANIM_REF_408_112_2_18_3_15_2_36,"DeleteGetDataCnf:handle=0x%x,img_handle=0x%x delete cnf msg!")
TRACE_MSG(GUIANIM_REF_454_112_2_18_3_15_2_37,"GUIANIM_GetInfo:get info result is %d!")
TRACE_MSG(GUIANIM_REF_638_112_2_18_3_15_2_38,"GUIANIM_ZoomImage:zoom result is %d!")
TRACE_MSG(GUIANIM_REF_682_112_2_18_3_15_2_39,"GUIANIM_ZoomRange:zoom result is %d!")
TRACE_MSG(GUIANIM_REF_748_112_2_18_3_15_2_40,"GUIANIM_RotateImage:rotate result is %d!")
TRACE_MSG(GUIANIM_REF_843_112_2_18_3_15_3_41,"GUIANIM_RotateArbitrary:rotate result is %d!")
TRACE_MSG(GUIANIM_REF_893_112_2_18_3_15_3_42,"GUIANIM_GetRotateSize:rotate result is %d!")
TRACE_MSG(GUIANIM_ROTATE_227_112_2_18_3_15_3_43,"IsRotate:rotate_mode %d is not rotate!")
TRACE_MSG(GUIANIM_ROTATE_263_112_2_18_3_15_3_44,"IsRotate:angle %d is not rotate!")
TRACE_MSG(GUIANIM_ROTATE_391_112_2_18_3_15_4_45,"RotateImage:rotate fail!")
TRACE_MSG(GUIANIM_ROTATE_489_112_2_18_3_15_4_46,"RotateMode:no rotate buffer!")
TRACE_MSG(GUIANIM_ROTATE_573_112_2_18_3_15_4_47,"RotateArbitrary:no rotate buffer!")
TRACE_MSG(GUIANIM_ZOOM_312_112_2_18_3_15_5_48,"GUIANIM_ZoomUtmost:ZoomUtmost fail!")
TRACE_MSG(GUIANIM_ZOOM_413_112_2_18_3_15_5_49,"ZoomImage:no zoom buffer!")
TRACE_MSG(GUIANIM_ZOOM_652_112_2_18_3_15_6_50,"GUIANIM_Zoom:zoom fail!")
TRACE_MSG(GUIANIM_ZOOM_945_112_2_18_3_15_6_51,"GUIANIM_ZoomMove:move_x and move_y are zero!")
TRACE_MSG(GUIBLOCK_762_112_2_18_3_15_8_52,"GUIBLOCK_SetHalfTransparent: enable block fail!")
TRACE_MSG(GUIBLOCK_767_112_2_18_3_15_8_53,"GUIBLOCK_SetHalfTransparent: register block fail!")
TRACE_MSG(GUIBLOCK_794_112_2_18_3_15_8_54,"== GUIBLOCK_GetInfo == lcd id %d is error")
TRACE_MSG(GUIBLOCK_831_112_2_18_3_15_8_55,"== GUIBLOCK_GetBuf == block_buf_ptr is PNULL")
TRACE_MSG(GUIBUTTON_898_112_2_18_3_15_10_56,"DisplayButtonFg:bg_type %d is error !")
TRACE_MSG(GUICOMMON_1029_112_2_18_3_15_16_57,"GUI_DisplayBorder:border type %d is error!")
TRACE_MSG(GUICOMMON_1121_112_2_18_3_15_17_58,"GUI_DisplayBorderToBuf:border type %d is error!")
TRACE_MSG(GUICOMMON_1239_112_2_18_3_15_17_59,"GUI_DisplayBg:shape %d is error !")
TRACE_MSG(GUICOMMON_1245_112_2_18_3_15_17_60,"GUI_DisplayBg:bg_type %d is error !")
TRACE_MSG(GUICOMMON_1449_112_2_18_3_15_17_61,"GUI_DisplaySheen:get buffer info fail!")
TRACE_MSG(GUICOMMON_1872_112_2_18_3_15_18_62,"== GUICOM TIMING START == %d")
TRACE_MSG(GUICOMMON_1887_112_2_18_3_15_18_63,"== GUICOM TIMING ELAPSE == %s: %d, %d")
TRACE_MSG(GUIDC_671_112_2_18_3_15_24_64,"GUIDC_SetLcdId Error input param!")
TRACE_MSG(GUIDC_696_112_2_18_3_15_24_65,"GUIDC_SetBlockId Error input param!")
TRACE_MSG(GUIDC_719_112_2_18_3_15_24_66,"GUIDC_SetOrigin Error input param!")
TRACE_MSG(GUIDC_740_112_2_18_3_15_24_67,"GUIDC_SetRect Error input param!")
TRACE_MSG(GUIDC_765_112_2_18_3_15_24_68,"GUIDC_SetFontType Error input param!")
TRACE_MSG(GUIDC_788_112_2_18_3_15_24_69,"GUIDC_GetFontType Error input param!")
TRACE_MSG(GUIDC_814_112_2_18_3_15_24_70,"GUIDC_SetFontEffect Error input param!")
TRACE_MSG(GUIDC_837_112_2_18_3_15_24_71,"GUIDC_GetFontEffect Error input param!")
TRACE_MSG(GUIDC_863_112_2_18_3_15_24_72,"GUIDC_SetFontColor Error input param!")
TRACE_MSG(GUIDC_886_112_2_18_3_15_24_73,"GUIDC_GetFontColor Error input param!")
TRACE_MSG(GUIDC_912_112_2_18_3_15_24_74,"GUIDC_SetTextAlign Error input param!")
TRACE_MSG(GUIDC_935_112_2_18_3_15_24_75,"GUIDC_GetTextAlign Error input param!")
TRACE_MSG(GUIDC_996_112_2_18_3_15_25_76,"GUIDC_SetPenSize Error input param!")
TRACE_MSG(GUIDC_1019_112_2_18_3_15_25_77,"GUIDC_GetPenSize Error input param!")
TRACE_MSG(GUIDC_1045_112_2_18_3_15_25_78,"GUIDC_SetPenColor Error input param!")
TRACE_MSG(GUIDC_1068_112_2_18_3_15_25_79,"GUIDC_GetPenColor Error input param!")
TRACE_MSG(GUIDC_1094_112_2_18_3_15_25_80,"GUIDC_SetPixel Error input param!")
TRACE_MSG(GUIDC_1137_112_2_18_3_15_25_81,"GUIDC_GetPixel Error input param!")
TRACE_MSG(GUIDC_1185_112_2_18_3_15_25_82,"GUIDC_DrawLine Error input param!")
TRACE_MSG(GUIDC_1211_112_2_18_3_15_25_83,"GUIDC_DrawRect Error input param!")
TRACE_MSG(GUIDC_1238_112_2_18_3_15_25_84,"GUIDC_FillRect Error input param!")
TRACE_MSG(GUIDC_1621_112_2_18_3_15_26_85,"GUIDC_GetFontWidth dc_ptr == PNULL!")
TRACE_MSG(GUIDC_1634_112_2_18_3_15_26_86,"GUIDC_GetFontHeight dc_ptr == PNULL!")
TRACE_MSG(GUIDC_1647_112_2_18_3_15_26_87,"GUIDC_GetStringWidth dc_ptr == PNULL!")
TRACE_MSG(GUIDC_1660_112_2_18_3_15_26_88,"GUIDC_GetStringHeight dc_ptr == PNULL!")
TRACE_MSG(GUIDROPDOWNLIST_537_112_2_18_3_15_27_89,"DropDownListInit PNULL == init_ptr!")
TRACE_MSG(GUIDROPDOWNLIST_613_112_2_18_3_15_28_90,"DropDownListConstruct PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_662_112_2_18_3_15_28_91,"DropDownListDestruct PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_984_112_2_18_3_15_28_92,"DropDownListHandleMsg PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_1383_112_2_18_3_15_29_93,"GUI GetDropdownlistRect: pos = %d")
TRACE_MSG(GUIDROPDOWNLIST_1390_112_2_18_3_15_29_94,"GUI GetDropdownlistRect: PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_1450_112_2_18_3_15_29_95,"GUI DrawDropDownlistButton: dropdownlist_ptr->is_has_button %d!")
TRACE_MSG(GUIDROPDOWNLIST_1470_112_2_18_3_15_29_96,"DropDownlistOnPenUp: PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_1534_112_2_18_3_15_29_97,"DropDownlistOnPenDown: PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_1573_112_2_18_3_15_30_98,"DropDownlistOnPenMove: PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_1615_112_2_18_3_15_30_99,"LoadDataToList: PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_1681_112_2_18_3_15_30_100,"GetPenPosition: PNULL INPUT PARAM!")
TRACE_MSG(GUIDROPDOWNLIST_1725_112_2_18_3_15_30_101,"CalListRect error input param!")
TRACE_MSG(GUIDROPDOWNLIST_1782_112_2_18_3_15_30_102,"CalListRect show type = %d error!")
TRACE_MSG(GUIDROPDOWNLIST_1930_112_2_18_3_15_30_103,"GUIDROPDOWNLIST CreateListBox failed!")
TRACE_MSG(GUIDROPDOWNLIST_1998_112_2_18_3_15_30_104,"GUIDROPDOWNLIST_AppendItemArray:array_size = %d is error!")
TRACE_MSG(GUIDROPDOWNLIST_2004_112_2_18_3_15_30_105,"GUIDROPDOWNLIST_AppendItemArray item_array_ptr is NULL!")
TRACE_MSG(GUIDROPDOWNLIST_2178_112_2_18_3_15_31_106,"DropdownInsertItemByPtr item_ptr is NULL!")
TRACE_MSG(GUIDROPDOWNLIST_2184_112_2_18_3_15_31_107,"DropdownInsertItemByPtr list_ptr is NULL!")
TRACE_MSG(GUIDROPDOWNLIST_2404_112_2_18_3_15_31_108,"DropdownSetMaxItemByPtr list_ptr is NULL!")
TRACE_MSG(GUIDROPDOWNLIST_2418_112_2_18_3_15_31_109,"DropdownSetMaxItemByPtr error ctrl type!")
TRACE_MSG(GUIDROPDOWNLIST_3189_112_2_18_3_15_33_110,"SetTextToEditCtrl")
TRACE_MSG(GUIDROPDOWNLIST_3294_112_2_18_3_15_33_111,"CreateInnerEditCtrl result = %d, ctrl_handle = %d")
TRACE_MSG(GUIDROPDOWNLIST_3298_112_2_18_3_15_33_112,"CreateInnerEditCtrl IN PARAM PNULL!")
TRACE_MSG(GUIDROPDOWNLIST_3326_112_2_18_3_15_33_113,"ChangeDDLCtrlType old type = %d, new type = %d")
TRACE_MSG(GUIDROPDOWNLIST_3339_112_2_18_3_15_33_114,"ChangeDDLCtrlType need not create edit handle")
TRACE_MSG(GUIEDIT_1066_112_2_18_3_15_36_115,"GetEditImInfo:edit list style %d is error!")
TRACE_MSG(GUIEDIT_1102_112_2_18_3_15_36_116,"GetEditImInfo:edit type %d is error!")
TRACE_MSG(GUIEDIT_1346_112_2_18_3_15_36_117,"SetEditImInfo:edit list style %d is error!")
TRACE_MSG(GUIEDIT_1445_112_2_18_3_15_37_118,"SetEditImInfo:edit type %d is error!")
TRACE_MSG(GUIEDIT_2929_112_2_18_3_15_40_119,"EditAddStr: edit rect is wrong,not update!")
TRACE_MSG(GUIEDIT_3092_112_2_18_3_15_40_120,"AddString:add underline string!")
TRACE_MSG(GUIEDIT_3778_112_2_18_3_15_41_121,"HandleParseErrorStr: %d position string is error!")
TRACE_MSG(GUIEDIT_4020_112_2_18_3_15_42_122,"HandleTextCancelKey:window intercept the cancel msg!")
TRACE_MSG(GUIEDIT_4449_112_2_18_3_15_43_123,"HandleEditOtherMsg:edit type %d is error!")
TRACE_MSG(GUIEDIT_4792_112_2_18_3_15_43_124,"IsValidItem:item_style%d is error!")
TRACE_MSG(GUIEDIT_5263_112_2_18_3_15_44_125,"CheckEditString: edit type %d is error!")
TRACE_MSG(GUIEDIT_5560_112_2_18_3_15_45_126,"CheckPasswordStr:password style %d is error!")
TRACE_MSG(GUIEDIT_6847_112_2_18_3_15_48_127,"GUIEDIT_SetAlign:edit type %d is error!")
TRACE_MSG(GUIEDIT_CURSOR_575_112_2_18_3_15_58_128,"GUIEDIT_MoveCursor:move_direction %d is error!")
TRACE_MSG(GUIEDIT_DATE_758_112_2_18_3_16_1_129,"GetNewDate: date bit %d is error!")
TRACE_MSG(GUIEDIT_DATE_802_112_2_18_3_16_2_130,"GetCurDateBit:date style %d is error!")
TRACE_MSG(GUIEDIT_INTERNAL_467_112_2_18_3_16_5_131,"GUIEDIT_NotifyParentMsg:notify_msg = %d is error!")
TRACE_MSG(GUIEDIT_INTERNAL_759_112_2_18_3_16_6_132,"GetEditAlign:edit type %d is error!")
TRACE_MSG(GUIEDIT_INTERNAL_843_112_2_18_3_16_6_133,"GetSeparatorChar:separator style %d is error!")
TRACE_MSG(GUIEDIT_INTERNAL_1147_112_2_18_3_16_7_134,"GUIEDIT_IsFixedFormat:edit type %d is error!")
TRACE_MSG(GUIEDIT_TIME_619_112_2_18_3_16_19_135,"GetNewTime: time bit %d is error!")
TRACE_MSG(GUIEDIT_TIME_682_112_2_18_3_16_19_136,"GetCurTimeBit: current cursor position %d is error!")
TRACE_MSG(GUIEDIT_TIME_872_112_2_18_3_16_19_137,"AdjustMinOrSec:display style %d is error!")
TRACE_MSG(GUIEDIT_TOUCH_645_112_2_18_3_16_22_138,"slide_state error: %d")
TRACE_MSG(GUIEDIT_TOUCH_702_112_2_18_3_16_22_139,"slide_state error: %d")
TRACE_MSG(GUIEDIT_TOUCH_747_112_2_18_3_16_22_140,"slide_state error: %d")
TRACE_MSG(GUIEDIT_TOUCH_955_112_2_18_3_16_23_141,"slide_state error: %d")
TRACE_MSG(GUIEDIT_TOUCH_1820_112_2_18_3_16_25_142,"== GetTouchInfo == y: %d, value: %d")
TRACE_MSG(GUI_UCS2B_CONVERTER_338_112_2_18_3_16_57_143,"GUI_UTF82UCS2B: i %d char %x")
TRACE_MSG(GUI_UCS2B_CONVERTER_535_112_2_18_3_16_57_144,"== GUI_WstrToOtherCode == code_type %d is error!")
TRACE_MSG(GUI_UCS2B_CONVERTER_693_112_2_18_3_16_58_145,"== GUI_OtherCodeToWstr == code_type %d is error!")
TRACE_MSG(GUI_UNICODE_167_112_2_18_3_17_0_146,"== GUI_GetCyrillicFontIndex == Cyrillic Index exceed")
TRACE_MSG(GUIFONT_96_112_2_18_3_17_1_147,"== GUI_GetCusorCoordinate == param rect_ptr is PNULL")
TRACE_MSG(GUIFONT_149_112_2_18_3_17_1_148,"== GUI_GetCursorInfo == param rect_ptr is PNULL")
TRACE_MSG(GUIFORM_3774_112_2_18_3_18_23_149,"%s== form == ")
TRACE_MSG(GUIFORM_3847_112_2_18_3_18_23_150,"%schild_ptr=0x%x, index=%d, ctrl_ptr=0x%08x, id=0x%08x, guid=%s, ")
TRACE_MSG(GUIFORM_3881_112_2_18_3_18_23_151,"%sindex=%d, id=0x%08x, guid=%s")
TRACE_MSG(GUIFORM_3898_112_2_18_3_18_23_152,"%sindex=%d, id=0x%08x, guid=%s")
TRACE_MSG(GUIFORM_3915_112_2_18_3_18_23_153,"%sindex=%d, id=0x%08x, guid=%s")
TRACE_MSG(GUIFORM_3932_112_2_18_3_18_23_154,"%sindex=%d, id=0x%08x, guid=%s")
TRACE_MSG(GUIFORM_3955_112_2_18_3_18_23_155,"==============   Dump form control basic tree   ==============")
TRACE_MSG(GUIFORM_3957_112_2_18_3_18_23_156,"==============================================================\n\n")
TRACE_MSG(GUIFORM_3959_112_2_18_3_18_23_157,"==============   Dump form control tree   ==============")
TRACE_MSG(GUIFORM_3961_112_2_18_3_18_23_158,"==============================================================\n\n")
TRACE_MSG(GUIFORM_CALC_675_112_2_18_3_18_24_159,"CalChildHeight: height type %d is error!")
TRACE_MSG(GUIFORM_LAYOUT_520_112_2_18_3_18_29_160,"AdjustFormRelativeRect:layout_type %d is error!")
TRACE_MSG(GUIFORM_LAYOUT_611_112_2_18_3_18_29_161,"ModifyPosByAlign:align %d is error!")
TRACE_MSG(GUIFORM_LAYOUT_1100_112_2_18_3_18_30_162,"GetChildRelativeRect:layout_type %d is error!")
TRACE_MSG(GUIFORM_LAYOUT_1416_112_2_18_3_18_31_163,"DestroyPrgCtrl:layout_type %d is error!")
TRACE_MSG(GUIFORM_MOVE_255_112_2_18_3_18_33_164,"GUIFORM_GetDisplayTopForKey: order form don't handle msg %d")
TRACE_MSG(GUIFORM_MSG_338_112_2_18_3_18_35_165,"TpDownCtrlHandleTpMsg:tp down control is not focus!")
TRACE_MSG(GUIREF_SCALE_239_112_2_18_3_18_41_166,"GUIREF_TvScalingDown:s_is_clear_osd = %d,i = %d!")
TRACE_MSG(GUIREF_SCALE_278_112_2_18_3_18_41_167,"GUIREF_TvScalingDown1:i = %d,scale_start_x = %d,scale_end_x = %d,start_y = %d,end_y = %d")
TRACE_MSG(GUIREF_SCALE_337_112_2_18_3_18_41_168,"GUIREF_GetBlockBufferBeforeScale:lcd_id = %d,block_id = %d,s_tv_block_buffer_addr[block_id] = 0x%x")
TRACE_MSG(GUIREF_SCALE_363_112_2_18_3_18_42_169,"GUIREF_SetBlockBufferBeforeScale:lcd_id = %d,block_id = %d,block_buffer_addr = 0x%x")
TRACE_MSG(GUIREF_SCALE_397_112_2_18_3_18_42_170,"GetTvOutputDataPtr1:s_is_used_memory1[block_id] = %d,is_copy = %d,color_key = 0x%x")
TRACE_MSG(GUIREF_SCALE_413_112_2_18_3_18_42_171,"GetTvOutputDataPtr2:s_is_used_memory1[block_id] = %d,is_copy = %d,color_key = 0x%x")
TRACE_MSG(GUIREF_SCALE_498_112_2_18_3_18_42_172,"GUIREF_SetBlockCfgBeforeScale:lcd_id = %d,block_id = %d,start_x = %d,end_x = %d,start_y = %d,end_y = %d")
TRACE_MSG(GUIGRAPH_2630_112_2_18_3_18_48_173,"== my_mem16set_win32 == param is error: pline=0x%x, byte_num=%d")
TRACE_MSG(GUIGRAPH_2697_112_2_18_3_18_48_174,"== MainLcdFillRect == the box is error!")
TRACE_MSG(GUIICONLIST_2770_112_2_18_3_19_8_175,"GetNextIndex:msg_id %d is error!")
TRACE_MSG(GUIICONLIST_5711_112_2_18_3_19_14_176,"IconListCtrlAppendIcon d%")
TRACE_MSG(GUIICONLIST_INTERNAL_265_112_2_18_3_19_16_177,"GUIICONLIST_GetItemWidth:icon list style %d is error!")
TRACE_MSG(GUIICONLIST_INTERNAL_323_112_2_18_3_19_17_178,"GUIICONLIST_GetItemHeight:icon list style %d is error!")
TRACE_MSG(GUIICONLIST_INTERNAL_352_112_2_18_3_19_17_179,"GUIICONLIST_GetItemHspace:icon list style %d is error!")
TRACE_MSG(GUIICONLIST_INTERNAL_381_112_2_18_3_19_17_180,"GUIICONLIST_GetItemVspace:icon list style %d is error!")
TRACE_MSG(GUIICONLIST_INTERNAL_410_112_2_18_3_19_17_181,"GUIICONLIST_GetIconWidth:icon list style %d is error!")
TRACE_MSG(GUIICONLIST_INTERNAL_439_112_2_18_3_19_17_182,"GUIICONLIST_GetIconHeight:icon list style %d is error!")
TRACE_MSG(GUIICONLIST_INTERNAL_835_112_2_18_3_19_18_183,"GUIICONLIST_GetItemHeight:icon list style %d is error!")
TRACE_MSG(GUIICONLIST_INTERNAL_1116_112_2_18_3_19_18_184,"GUIICONLIST_GetLayoutStyle:iconlist LayoutStyle %d is error!")
TRACE_MSG(GUIIM_1361_112_2_18_3_19_27_185,"GUIIM: Setting wrong. lang = %#x")
TRACE_MSG(GUIIM_1370_112_2_18_3_19_27_186,"GUIIM: Setting wrong. lang = %#x. Undef method: %#x")
TRACE_MSG(GUIIM_1418_112_2_18_3_19_27_187,"GUIIM: Setting wrong, not in set. input = %#x")
TRACE_MSG(GUIIM_1429_112_2_18_3_19_27_188,"GUIIM: Setting wrong. input = %#x")
TRACE_MSG(GUIIM_1442_112_2_18_3_19_27_189,"GUIIM: Can not get api. input = %#x. Undef method: %#x")
TRACE_MSG(GUIIM_1502_112_2_18_3_19_27_190,"GUIIM: Setting wrong, not in set. type = %#x")
TRACE_MSG(GUIIM_1513_112_2_18_3_19_27_191,"GUIIM: Setting wrong. type = %#x")
TRACE_MSG(GUIIM_1522_112_2_18_3_19_27_192,"GUIIM: Setting wrong. type = %#x. Undef method: %#x")
TRACE_MSG(GUIIM_1569_112_2_18_3_19_27_193,"GUIIM: Setting wrong. cap = %#x")
TRACE_MSG(GUIIM_1578_112_2_18_3_19_27_194,"GUIIM: Setting wrong. cap = %#x. Undef method: %#x")
TRACE_MSG(GUIIM_1623_112_2_18_3_19_27_195,"GUIIM: Setting wrong. cap = %#x")
TRACE_MSG(GUIIMG_152_112_2_18_3_19_30_196,"GUIIMG_Decode: decode_data_size 0x%x < target_buf_size 0x%x")
TRACE_MSG(GUIIMG_186_112_2_18_3_19_30_197,"GUIIMG_GetImgInfo:get info result is %d!")
TRACE_MSG(GUIIMG_592_112_2_18_3_19_31_198,"GUIIMG_DisplayBmp:display rect is not on lcd!")
TRACE_MSG(GUIIMG_618_112_2_18_3_19_31_199,"GUIIMG_DisplayBmp:image rect is not cross with show rect!")
TRACE_MSG(GUIIMG_906_112_2_18_3_19_32_200,"GUIIMG_DisplayBmp:display rect is not on lcd!")
TRACE_MSG(GUIIMG_932_112_2_18_3_19_32_201,"GUIIMG_DisplayBmp:image rect is not cross with show rect!")
TRACE_MSG(GUILABEL_1208_112_2_18_3_19_35_202,"GetLabelAlign:label_align %d is error!")
TRACE_MSG(GUILCD_128_112_2_18_3_19_38_203,"GUILCD_Init:init lcd %d is fail!")
TRACE_MSG(GUILCD_169_112_2_18_3_19_38_204,"InitLcdLogicAngle:init lcd %d is fail!")
TRACE_MSG(GUILCD_452_112_2_18_3_19_38_205,"InvalidateLcdRect.c UILAYER_GetBlendInfo failed!")
TRACE_MSG(GUILCD_676_112_2_18_3_19_39_206,"GUILCD_GetLogicWidthHeight: logic angle %d is error!")
TRACE_MSG(GUILCD_760_112_2_18_3_19_39_207,"== GetInvalidateBlockInfo == can get lcd id %d info")
TRACE_MSG(GUILISTBOX_3392_112_2_18_3_19_48_208,"GetEffectContentStr SCI_ALLOCA error!")
TRACE_MSG(GUILISTBOX_3402_112_2_18_3_19_48_209,"GetEffectContentStr error!")
TRACE_MSG(GUILISTBOX_4832_112_2_18_3_19_51_210,"InitScrollBar list_ptr->prgbox_ptr = PNULL!")
TRACE_MSG(GUILISTBOX_5230_112_2_18_3_19_52_211,"GUILIST_GetItemState PNULL == item_ptr!")
TRACE_MSG(GUILISTBOX_7163_112_2_18_3_19_56_212,"HandleListItemSlide first!")
TRACE_MSG(GUILISTBOX_7729_112_2_18_3_19_57_213,"GUILIST_StartSwitchItem NULL LISt!")
TRACE_MSG(GUILISTBOX_7768_112_2_18_3_19_57_214,"GUILIST_StartSwitchItem NULL LISt!")
TRACE_MSG(GUILISTBOX_7789_112_2_18_3_19_57_215,"GUILIST_StopSwitchItem NULL LISt!")
TRACE_MSG(GUILISTBOX_8469_112_2_18_3_19_59_216,"GUILIST_Construct init param error!")
TRACE_MSG(GUILISTBOX_8492_112_2_18_3_19_59_217,"GUILIST_Destruct list handle = 0x%0x")
TRACE_MSG(GUILISTBOX_8612_112_2_18_3_19_59_218,"GUILIST_HandleMsg invalid ctrl type!")
TRACE_MSG(GUILISTBOX_10076_112_2_18_3_20_3_219,"GUILIST_Init: init param error!")
TRACE_MSG(GUILISTBOX_10093_112_2_18_3_20_3_220,"GUILIST_Init: list_ptr = %x")
TRACE_MSG(GUILISTBOX_10212_112_2_18_3_20_3_221,"GUILIST_SetMaxItemByPtr error ctrl type!")
TRACE_MSG(GUILISTBOX_10257_112_2_18_3_20_3_222,"GUILIST_SetMaxSelectedItemByPtr: list_ptr->type %d is error!")
TRACE_MSG(GUILISTBOX_10264_112_2_18_3_20_3_223,"GUILIST_SetMaxSelectedItemByPtr: error ctrl type!")
TRACE_MSG(GUILISTBOX_10616_112_2_18_3_20_4_224,"GUILIST_SetItemContentByPtr: type = %d!")
TRACE_MSG(GUILISTBOX_13845_112_2_18_3_20_10_225,"ReviceListDisplayItem 1 Error!")
TRACE_MSG(GUILISTBOX_13866_112_2_18_3_20_10_226,"ReviceListDisplayItem 2 Error!")
TRACE_MSG(GUILISTBOX_14646_112_2_18_3_20_12_227,"HandleListTpUp %f")
TRACE_MSG(GUILISTBOX_14889_112_2_18_3_20_12_228,"HandleListTpLong slide state = %d")
TRACE_MSG(GUIMENU_1304_112_2_18_3_20_19_229,"MenuHandleMsg %d, %d")
TRACE_MSG(GUIMENU_1325_112_2_18_3_20_19_230,"MenuHandleMsg %d, %d")
TRACE_MSG(GUIMENU_1332_112_2_18_3_20_19_231,"MenuHandleMsg %d, %d")
TRACE_MSG(GUIMENU_1374_112_2_18_3_20_20_232,"MenuHandleMsg MSG_APP_CANCEL %d, %d")
TRACE_MSG(GUIMENU_1396_112_2_18_3_20_20_233,"MenuHandleMsg MSG_APP_UP %d, %d")
TRACE_MSG(GUIMENU_1416_112_2_18_3_20_20_234,"MenuHandleMsg MSG_APP_1 %d, %d")
TRACE_MSG(GUIMENU_1437_112_2_18_3_20_20_235,"MenuHandleMsg MSG_APP_HASH %d, %d")
TRACE_MSG(GUIMENU_1548_112_2_18_3_20_20_236,"MenuHandleMsg MSG_APP_CANCEL %d, %d")
TRACE_MSG(GUIMENU_2125_112_2_18_3_20_21_237,"GetCurItemPtr:menu_style = %d is error!")
TRACE_MSG(GUIMENU_2297_112_2_18_3_20_22_238,"== NotifyParentMsg == msg_id %d is not handled")
TRACE_MSG(GUIMENU_2340_112_2_18_3_20_22_239,"PushMenuStack: menu stack %d is full!")
TRACE_MSG(GUIMENU_2374_112_2_18_3_20_22_240,"PopMenuStack: menu stack %d is null!")
TRACE_MSG(GUIMENU_2703_112_2_18_3_20_23_241,"== HandleMenuDirectionKey == msg_id %d is not handled")
TRACE_MSG(GUIMENU_3281_112_2_18_3_20_24_242,"== GetMenuPtr == ctrl_handle 0x%x is not menu")
TRACE_MSG(GUIMENU_3614_112_2_18_3_20_24_243,"== GUIMENU_CreatDynamic == menu_style %d is error!")
TRACE_MSG(GUIMENU_3622_112_2_18_3_20_24_244,"== GUIMENU_CreatDynamic == rect_ptr is PNULL")
TRACE_MSG(GUIMENU_4028_112_2_18_3_20_25_245,"== GUIMENU_GetCurNodeId == error when GetNodeByVisibleIndex")
TRACE_MSG(GUIMENU_4074_112_2_18_3_20_25_246,"GUIMENU_SetPopItemStatus:selected item has exist")
TRACE_MSG(GUIMENU_4081_112_2_18_3_20_25_247,"GUIMENU_SetPopItemStatus:selected item not exist")
TRACE_MSG(GUIMENU_6878_112_2_18_3_20_31_248,"GUIMENU_SetItemVisibleByNum error: visible: %d , ptr: %d")
TRACE_MSG(GUIMSGBOX_1200_112_2_18_3_20_34_249,"gui warning:ShowMsgbox: msgbox_ptr = PNULL ")
TRACE_MSG(GUIMSGBOX_1273_112_2_18_3_20_34_250,"gui warning:ShowMsgBoxText: text_info_ptr = PNULL ")
TRACE_MSG(GUIMSGBOX_1350_112_2_18_3_20_34_251,"gui warning:HandleMsgBoxLoseFocus: msgbox_ptr = PNULL ")
TRACE_MSG(GUIPRGBOX_382_112_2_18_3_20_38_252,"GUIPRGBOX_Init, alloc memory, prgbox_ctrl_ptr = %x")
TRACE_MSG(GUIPRGBOX_972_112_2_18_3_20_39_253,"DisplayVerticalScroll bg type = %d")
TRACE_MSG(GUIPRGBOX_2162_112_2_18_3_20_41_254,"[baokun] TP MOVE tp_point.y %d cur_item_index %d")
TRACE_MSG(GUIRES_IMG_492_112_2_18_3_20_46_255,"GUIRES_GetImgInfo:img_type %d is error!")
TRACE_MSG(GUIRES_IMG_498_112_2_18_3_20_46_256,"GUIRES_GetImgInfo:res_result = %d")
TRACE_MSG(GUIRES_IMG_534_112_2_18_3_20_46_257,"== GUIRES_IsArgb == error when MMI_GetLabelImage")
TRACE_MSG(GUIRES_IMG_565_112_2_18_3_20_46_258,"== GUIRES_IsSabm == error when MMI_GetLabelImage")
TRACE_MSG(GUIRES_IMG_604_112_2_18_3_20_46_259,"== GUIRES_GetImgWidthHeight == error when MMI_GetLabelImage")
TRACE_MSG(GUIRES_IMG_736_112_2_18_3_20_46_260,"== GUIRES_DisplayImg == error when MMI_GetLabelImage")
TRACE_MSG(GUIRES_IMG_803_112_2_18_3_20_46_261,"== GUIRES_DisplaySabmImg == error when MMI_GetLabelImage")
TRACE_MSG(GUIRES_IMG_858_112_2_18_3_20_46_262,"== GUIRES_DisplayImgEx == error when MMI_GetLabelImage")
TRACE_MSG(GUIRES_IMG_919_112_2_18_3_20_46_263,"== GUIRES_DisplayImgWitchColorExchange == error when MMI_GetLabelImage")
TRACE_MSG(GUIRES_IMG_1043_112_2_18_3_20_47_264,"GUIRES_DisplayImg:display image res_result = %d")
TRACE_MSG(GUIRES_IMG_1050_112_2_18_3_20_47_265,"GUIRES_DisplayImg:get image info res_result = %d")
TRACE_MSG(GUIRES_IMG_1182_112_2_18_3_20_47_266,"GetImgRect:rect = {%d,%d,%d,%d} is error!")
TRACE_MSG(GUIRES_IMG_1305_112_2_18_3_20_47_267,"GetDisplayCtl: display block buffer is PNULL!")
TRACE_MSG(GUIRES_IMG_1376_112_2_18_3_20_48_268,"AdjustImgAndDisRect:rect = {%d,%d,%d,%d} is error!")
TRACE_MSG(GUIRES_IMG_1415_112_2_18_3_20_48_269,"== GUIRES_GetAnimWidthHeight == error when MMI_GetLabelAnim")
TRACE_MSG(GUIRES_IMG_1482_112_2_18_3_20_48_270,"== GUIRES_DisplayAnim == error when MMI_GetLabelAnim")
TRACE_MSG(GUIRES_IMG_1534_112_2_18_3_20_48_271,"== GUIRES_DisplayAnimEx == error when MMI_GetLabelAnim")
TRACE_MSG(GUIRES_IMG_1684_112_2_18_3_20_48_272,"DisplayAnim:display anim res_result = %d")
TRACE_MSG(GUIRES_IMG_1689_112_2_18_3_20_48_273,"DisplayAnim:creat handle res_result = %d")
TRACE_MSG(GUIRES_IMG_1699_112_2_18_3_20_48_274,"DisplayAnim:free handle res_result = %d")
TRACE_MSG(GUIRES_IMG_1707_112_2_18_3_20_48_275,"DisplayAnim:get anim info res_result = %d!")
TRACE_MSG(GUIRES_IMG_1982_112_2_18_3_20_49_276,"GUIRES_CopyImgBuf:display image res_result = %d")
TRACE_MSG(GUIRES_IMG_1990_112_2_18_3_20_49_277,"GUIRES_CopyImgBuf:get image info res_result = %d")
TRACE_MSG(GUIRES_IMG_2028_112_2_18_3_20_49_278,"GUIRES_CreatHandle:creat handle res_result = %d")
TRACE_MSG(GUIRES_IMG_2056_112_2_18_3_20_49_279,"GUIRES_FreeHandle:free handle res_result = %d")
TRACE_MSG(GUIRES_IMG_2090_112_2_18_3_20_49_280,"== GUIRES_DisplayGif == param is error")
TRACE_MSG(GUIRES_IMG_2144_112_2_18_3_20_49_281,"== GUIRES_DisplayGifEx == param is error")
TRACE_MSG(GUIRES_IMG_2296_112_2_18_3_20_49_282,"GUIRES_DisplayImg:get image info res_result = %d")
TRACE_MSG(GUIRES_IMG_2348_112_2_18_3_20_49_283,"GUIRES_DisplayImg:display image res_result = %d")
TRACE_MSG(GUIRICHTEXT_5919_112_2_18_3_21_2_284,"richtext copy failed, malloc size: %d.")
TRACE_MSG(GUISCROLLKEY_546_112_2_18_3_21_12_285,"GUISCROLLKEY: scrollkey_ptr->bg_type error!")
TRACE_MSG(GUISCROLLKEY_747_112_2_18_3_21_13_286,"DrawButtonContent invalid type = %d!")
TRACE_MSG(GUISCROLLKEY_1452_112_2_18_3_21_14_287,"GUISCROLLKEY_SetTextId: the win_handle = %d")
TRACE_MSG(GUISCROLLKEY_1516_112_2_18_3_21_15_288,"GUISCROLLKEY_SetButtonTextId: the win_handle = %d")
TRACE_MSG(GUISCROLLKEY_1555_112_2_18_3_21_15_289,"GUISCROLLKEY_SetButtonTextId: the win_handle = %d")
TRACE_MSG(GUISCROLLKEY_1615_112_2_18_3_21_15_290,"GUISCROLLKEY_SetButtonIconId: the win_handle = %d")
TRACE_MSG(GUISCROLLKEY_1858_112_2_18_3_21_15_291,"GUISCROLLKEY_SetButtonState(),win_handle = %d, state = %d")
TRACE_MSG(GUISETLIST_1281_112_2_18_3_21_19_292,"InsertItem error ctrl type!")
TRACE_MSG(GUISETLIST_1337_112_2_18_3_21_20_293,"AddItem error ctrl type!")
TRACE_MSG(GUISETLIST_1523_112_2_18_3_21_20_294,"GetImageDispRect error input param!")
TRACE_MSG(GUISETLIST_1591_112_2_18_3_21_20_295,"GetImageDispRect: not surport image!")
TRACE_MSG(GUISETLIST_1627_112_2_18_3_21_20_296,"GetTextRect error input param!")
TRACE_MSG(GUISETLIST_1676_112_2_18_3_21_20_297,"GetTextRect: not surport text!")
TRACE_MSG(GUISETLIST_1719_112_2_18_3_21_20_298,"GetArrowRect error input param!")
TRACE_MSG(GUISETLIST_1863_112_2_18_3_21_21_299,"DrawSetlistFrame error input param!")
TRACE_MSG(GUISETLIST_1882_112_2_18_3_21_21_300,"AdjustSetlistRect error input param!")
TRACE_MSG(GUISETLIST_1933_112_2_18_3_21_21_301,"DestroySetlistItemData: item_data_type %d is error!")
TRACE_MSG(GUISETLIST_2053_112_2_18_3_21_21_302,"DrawItemImage: type %d is not image!")
TRACE_MSG(GUISETLIST_2655_112_2_18_3_21_22_303,"HandleSetListTpUp NULL!")
TRACE_MSG(GUISETLIST_2710_112_2_18_3_21_23_304,"HandleSetListLeftKeyDown error input param!")
TRACE_MSG(GUISETLIST_2752_112_2_18_3_21_23_305,"HandleSetListRightKeyDown error input param!")
TRACE_MSG(GUISETLIST_2794_112_2_18_3_21_23_306,"HandleSetListWebKeyDown error input param!")
TRACE_MSG(GUISETLIST_3112_112_2_18_3_21_23_307,"GUISETLIST_HandleMessage error ctrl type!")
TRACE_MSG(GUISETLIST_3385_112_2_18_3_21_24_308,"GUISETLIST_CallBack MMI_SETLIST_SET_ITEM_STYLE %d")
TRACE_MSG(GUISETLIST_3460_112_2_18_3_21_24_309,"GUISETLIST_CallBack MMI_SETLIST_IS_OPEN_FULL_WIN %d")
TRACE_MSG(GUISOFTKEY_528_112_2_18_3_21_26_310,"GUISOFTKEY: softkey_ptr->bg_type error!")
TRACE_MSG(GUISOFTKEY_706_112_2_18_3_21_27_311,"DrawButtonContent invalid type = %d!")
TRACE_MSG(GUISOFTKEY_1558_112_2_18_3_21_29_312,"GUISOFTKEY_SetTextId: the win_handle = %d")
TRACE_MSG(GUISOFTKEY_1701_112_2_18_3_21_29_313,"GUISOFTKEY_SetButtonTextId: the win_handle = %d")
TRACE_MSG(GUISOFTKEY_1790_112_2_18_3_21_29_314,"GUISOFTKEY_SetButtonTextId: the win_handle = %d")
TRACE_MSG(GUISOFTKEY_1892_112_2_18_3_21_29_315,"GUISOFTKEY_SetButtonIconId: the win_handle = %d")
TRACE_MSG(GUISOFTKEY_1953_112_2_18_3_21_29_316,"GUISOFTKEY_SetStyle() not support!")
TRACE_MSG(GUISOFTKEY_2030_112_2_18_3_21_30_317,"GUISOFTKEY_SetBgType() not support!")
TRACE_MSG(GUISOFTKEY_2066_112_2_18_3_21_30_318,"GUISOFTKEY_Setfont() not support!")
TRACE_MSG(GUISOFTKEY_2165_112_2_18_3_21_30_319,"GUISOFTKEY_SetButtonState(),win_handle = %d, state = %d")
TRACE_MSG(GUISOFTKEY_2254_112_2_18_3_21_30_320,"GUISOFTKEY_GetTextId() not support!")
TRACE_MSG(GUISOFTKEY_2330_112_2_18_3_21_30_321,"GUISOFTKEY_IsTPEnable() not support!")
TRACE_MSG(GUISOFTKEY_2364_112_2_18_3_21_30_322,"GUISOFTKEY_SetTPEnable() not support!")
TRACE_MSG(GUISOFTKEY_BUTTON_124_112_2_18_3_21_31_323,"InitButtonCtrl() invalid param!")
TRACE_MSG(GUISOFTKEY_BUTTON_151_112_2_18_3_21_31_324,"GUIBUTTONSOFTKEY_Init() invalid param!")
TRACE_MSG(GUISOFTKEY_BUTTON_207_112_2_18_3_21_31_325,"GUIBUTTONOFTKEY_Update() invalid param!")
TRACE_MSG(GUISOFTKEY_BUTTON_230_112_2_18_3_21_31_326,"GUIBUTTONSOFTKEY_SetVisible() invalid param!")
TRACE_MSG(GUISOFTKEY_BUTTON_253_112_2_18_3_21_31_327,"GUIBUTTONSOFTKEY_SetButtonState() invalid param!")
TRACE_MSG(GUISOFTKEY_BUTTON_274_112_2_18_3_21_32_328,"GUIBUTTONSOFTKEY_SetButtonState() invalid param!")
TRACE_MSG(GUISOFTKEY_BUTTON_297_112_2_18_3_21_32_329,"GUIBUTTONSOFTKEY_SetTextId() invalid param!")
TRACE_MSG(GUISTATUSBAR_319_112_2_18_3_21_33_330,"GUI STATUSBAR: ctrl data error!")
TRACE_MSG(GUISTATUSBAR_653_112_2_18_3_21_34_331,"CalculateItemRect 1 item_data_ptr = PNULL, %d")
TRACE_MSG(GUISTATUSBAR_740_112_2_18_3_21_34_332,"CalculateItemRect 2 item_data_ptr = PNULL, %d")
TRACE_MSG(GUISTATUSBAR_935_112_2_18_3_21_34_333,"CalculateItemRect 1 item_data_ptr = PNULL, %d")
TRACE_MSG(GUISTATUSBAR_1028_112_2_18_3_21_35_334,"CalculateItemRect 2 item_data_ptr = PNULL, %d")
TRACE_MSG(GUISTATUSBAR_1079_112_2_18_3_21_35_335,"DrawStatusBarBg error input param!")
TRACE_MSG(GUISTATUSBAR_1149_112_2_18_3_21_35_336,"DrawStatusBarBg status bar background type = %d!")
TRACE_MSG(GUISTATUSBAR_1190_112_2_18_3_21_35_337,"DrawStatusBarAllItem error input param!")
TRACE_MSG(GUISTATUSBAR_1395_112_2_18_3_21_35_338,"DrawStatusBarScrollItem error input param!")
TRACE_MSG(GUISTATUSBAR_1675_112_2_18_3_21_36_339,"HandleTimer statusbar_ptr->is_shown = %d")
TRACE_MSG(GUISTATUSBAR_1690_112_2_18_3_21_36_340,"HandleTimer item_index = %d")
TRACE_MSG(GUISTATUSBAR_1900_112_2_18_3_21_36_341,"StatusBarConstruct error init data!")
TRACE_MSG(GUISTATUSBAR_1942_112_2_18_3_21_37_342,"HandleMessage input error param!")
TRACE_MSG(GUISTATUSBAR_DATA_117_112_2_18_3_21_38_343,"InsertItemData PNULL == item_ptr!")
TRACE_MSG(GUISTATUSBAR_DATA_123_112_2_18_3_21_38_344,"InsertItemData PNULL == item_data_ptr!")
TRACE_MSG(GUISTATUSBAR_SCROLLMSG_305_112_2_18_3_21_40_345,"GUISTBSCROLLMSG_AppendScrollItem() item_ptr = 0x%x")
TRACE_MSG(GUISTRING_538_112_2_18_3_21_42_346,"GetYPosition invalid input param!")
TRACE_MSG(GUISTRING_590_112_2_18_3_21_42_347,"[GetYPosition guistring.c] len = %d, str_ptr = %x")
TRACE_MSG(GUISTRING_929_112_2_18_3_21_43_348,"error! [DrawTextInRect] len = %d")
TRACE_MSG(GUITAB_839_112_2_18_3_21_50_349,"TabConstruct init param error!")
TRACE_MSG(GUITAB_882_112_2_18_3_21_50_350,"TabDestruct input param error!")
TRACE_MSG(GUITAB_3029_112_2_18_3_21_54_351,"DrawItemAnim type error! type = %d")
TRACE_MSG(GUITAB_3075_112_2_18_3_21_55_352,"GUITAB: DrawChildWindow no focus window!")
TRACE_MSG(GUITAB_3443_112_2_18_3_21_55_353,"GUITAB_Init init param null!")
TRACE_MSG(GUITAB_3692_112_2_18_3_21_56_354,"MoveToNextValidItem index = %d, total num = %d, valid num = %d")
TRACE_MSG(GUITAB_3724_112_2_18_3_21_56_355,"MoveToNextValidItem error type! %d")
TRACE_MSG(GUITAB_3809_112_2_18_3_21_56_356,"HandleTabTpDown:tp point not in tab valid rect!")
TRACE_MSG(GUITAB_4111_112_2_18_3_21_57_357,"GetMoveItemNum error! type = %d")
TRACE_MSG(GUITAB_4480_112_2_18_3_21_57_358,"HandleTabMsg error ctrl type!")
TRACE_MSG(GUITAB_4925_112_2_18_3_21_58_359,"guitab.c: AllocAllItemsSpace failed")
TRACE_MSG(GUITAB_5040_112_2_18_3_21_58_360,"GUITAB_AppendItemByPtr error ctrl type!")
TRACE_MSG(GUITAB_5070_112_2_18_3_21_58_361,"GUITAB_InsertItemByPtr error ctrl type!")
TRACE_MSG(GUITAB_5113_112_2_18_3_21_59_362,"GUITAB_AppendSimpleItem input param error!")
TRACE_MSG(GUITAB_5232_112_2_18_3_21_59_363,"GUITAB_DeleteAllItems cur num = %d, total num = %d, valid num = %d")
TRACE_MSG(GUITAB_5436_112_2_18_3_21_59_364,"item state = %d, item index = %d")
TRACE_MSG(GUITAB_5660_112_2_18_3_22_0_365,"GUITAB_SetItemImagePtr: type %d is not anim!")
TRACE_MSG(GUITAB_6211_112_2_18_3_22_1_366,"SetScrollCardState type error! %d")
TRACE_MSG(GUITAB_6233_112_2_18_3_22_1_367,"GUITAB_SwitchToNextItem FALSE!")
TRACE_MSG(GUITAB_6265_112_2_18_3_22_1_368,"GUITAB_SwitchToNextItem type error! %d")
TRACE_MSG(GUITITLE_1041_112_2_18_3_22_22_369,"GUITITLE_SetTextScrollType: the type is error!")
TRACE_MSG(GUITITLE_1618_112_2_18_3_22_23_370,"DisplayScrollTitleByOffset: the param is error!")
TRACE_MSG(GUITITLE_2230_112_2_18_3_22_24_371,"DisplayButtonText: text str is NULL!")
TRACE_MSG(GUITITLE_2592_112_2_18_3_22_25_372,"DisplayTitle: text str is NULL!")
END_TRACE_MAP(MMI_GUI_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_GUI_TRC_H_

