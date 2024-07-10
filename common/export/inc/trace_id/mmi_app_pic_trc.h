/******************************************************************************
 ** File Name:      mmi_app_pic_trc.h                                         *
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
//trace_id:209
#ifndef _MMI_APP_PIC_TRC_H_
#define _MMI_APP_PIC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPIC_EDIT_147_112_2_18_2_45_41_0 "[MMIPIC] MMIPIC_GetCommonFileDataBuf file_info_ptr is PNULL !!!"
#define MMIPIC_EDIT_154_112_2_18_2_45_41_1 "[MMIPIC] MMIPIC_GetCommonFileDataBuf full_path_wstr_ptr is PNULL !!!"
#define MMIPIC_EDIT_161_112_2_18_2_45_41_2 "[MMIPIC] MMIPIC_GetCommonFileDataBuf full_path_wstr_len is 0 !!!"
#define MMIPIC_EDIT_175_112_2_18_2_45_41_3 "[MMIPIC] s_pic_editor_info.file_data_buffer is PNULL"
#define MMIPIC_EDIT_190_112_2_18_2_45_41_4 "MMIPIC_GetCommonFileDataBuf: read picture is %d"
#define MMIPIC_EDIT_230_112_2_18_2_45_41_5 "[MMIPIC] PicGetPicFileSize full_path_ptr is PNULL !!!"
#define MMIPIC_EDIT_275_112_2_18_2_45_41_6 "[MMIPIC] MMIPIC_DecodeJpgImage file_info_ptr is PNULL !!!"
#define MMIPIC_EDIT_282_112_2_18_2_45_41_7 "[MMIPIC] MMIPIC_DecodeJpgImage full_path_wstr_ptr is PNULL !!!"
#define MMIPIC_EDIT_289_112_2_18_2_45_41_8 "[MMIPIC] MMIPIC_DecodeJpgImage full_path_wstr_len is 0 !!!"
#define MMIPIC_EDIT_317_112_2_18_2_45_41_9 "[MMIPIC] img_output.decode_data_ptr is PNULL"
#define MMIPIC_EDIT_336_112_2_18_2_45_41_10 "[MMIPIC] s_pic_editor_info.display_bitmap_ptr is PNULL"
#define MMIPIC_EDIT_346_112_2_18_2_45_41_11 "[MMIPIC] s_pic_editor_info.display_bitmap_ptr->bit_ptr is PNULL"
#define MMIPIC_EDIT_385_112_2_18_2_45_42_12 "[MMIPIC] MMIPIC_DecodeJpgImage result = %d"
#define MMIPIC_EDIT_397_112_2_18_2_45_42_13 "[MMIPIC] MMIPIC_PicEditorInit file_info_ptr is PNULL !!!"
#define MMIPIC_EDIT_403_112_2_18_2_45_42_14 "[MMIPIC] MMIPIC_PicEditorInit full_path_wstr_ptr is PNULL !!!"
#define MMIPIC_EDIT_409_112_2_18_2_45_42_15 "[MMIPIC] MMIPIC_PicEditorInit full_path_wstr_len is 0 !!!"
#define MMIPIC_EDIT_415_112_2_18_2_45_42_16 "MMIPIC_PicEditorInit"
#define MMIPIC_EDIT_436_112_2_18_2_45_42_17 "[MMIPIC s_pic_editor_info.undo_buf_ptr is PNULL"
#define MMIPIC_EDIT_455_112_2_18_2_45_42_18 "[MMIPIC] s_pic_editor_info.process_buf_ptr is PNULL"
#define MMIPIC_EDIT_462_112_2_18_2_45_42_19 "[MMIPIC] s_pic_editor_info.output_buf_ptr is PNULL"
#define MMIPIC_EDIT_645_112_2_18_2_45_42_20 "[MMIPIC] MMIPIC_DisplayImage display_bitmap_ptr is PNULL !!!"
#define MMIPIC_EDIT_651_112_2_18_2_45_42_21 "[MMIPIC] MMIPIC_DisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!"
#define MMIPIC_EDIT_966_112_2_18_2_45_43_22 "[MMIPIC] MMIPIC_DisplayOutputImage display_bitmap_ptr is PNULL !!!"
#define MMIPIC_EDIT_972_112_2_18_2_45_43_23 "[MMIPIC] MMIPIC_DisplayOutputImage output_buf_ptr is PNULL !!!"
#define MMIPIC_EDIT_1388_112_2_18_2_45_44_24 "MMIPIC_SaveToUndoBuf: save buf and width heigh and x,y "
#define MMIPIC_EDIT_1412_112_2_18_2_45_44_25 "[MMIPIC] MMIPIC_OnlySaveOrgImgToUndoBuf: save buf and width heigh and x,y "
#define MMIPIC_EDIT_1473_112_2_18_2_45_44_26 "[MMIPIC] MMIPIC_Save filename len=%d"
#define MMIPIC_EDIT_1474_112_2_18_2_45_44_27 "[MMIPIC] MMIPIC_Save dir_name_len = %d"
#define MMIPIC_EDIT_1607_112_2_18_2_45_44_28 "[MMIPIC] MMIPIC_GetFileInfo param_to_save_as_win is PNULL !!!"
#define MMIPIC_EDIT_1647_112_2_18_2_45_44_29 "[MMIPIC] MMIPIC_SaveAs full_path_ptr is PNULL !!!"
#define MMIPIC_EDIT_1654_112_2_18_2_45_44_30 "[MMIPIC] MMIPIC_SaveAs full_path_len is 0 !!!"
#define MMIPIC_EDIT_1664_112_2_18_2_45_44_31 "MMIPIC_SaveAs is_changed=%d"
#define MMIPIC_EDIT_1699_112_2_18_2_45_44_32 "MMIAPIFMM_CopyOneFile ret=%d"
#define MMIPIC_EDIT_1859_112_2_18_2_45_45_33 "[MMIPIC] s_icon_image[i].icon_bitmap_ptr is PNULL"
#define MMIPIC_EDIT_1871_112_2_18_2_45_45_34 "[MMIPIC] s_icon_image[i].icon_bitmap_ptr->bit_ptr is PNULL"
#define MMIPIC_EDIT_1999_112_2_18_2_45_45_35 "free rotate preview image buf"
#define MMIPIC_EDIT_2232_112_2_18_2_45_45_36 "[MMIPIC] s_pic_editor_info.osd_image.osd_data_buffer is PNULL"
#define MMIPIC_EDIT_2331_112_2_18_2_45_46_37 "[MMIPIC] s_pic_editor_info.osd_image.osd_ptr is PNULL"
#define MMIPIC_EDIT_2418_112_2_18_2_45_46_38 "[MMIPIC] GetPicTextStyle text_style_ptr is PNULL !!!"
#define MMIPIC_EDIT_2476_112_2_18_2_45_46_39 "[MMIPIC] MMIPIC_DrawText text_ptr is PNULL !!!"
#define MMIPIC_EDIT_2633_112_2_18_2_45_46_40 "[MMIPIC] MMIPIC_DrawText text_ptr is PNULL !!!"
#define MMIPIC_EDIT_2804_112_2_18_2_45_46_41 "[MMIPIC] MMIPIC_MoveText text_ptr is PNULL !!!"
#define MMIPIC_EDIT_2810_112_2_18_2_45_46_42 "[MMIPIC] MMIPIC_MoveText left or right is error !!!"
#define MMIPIC_EDIT_2816_112_2_18_2_45_46_43 "[MMIPIC] MMIPIC_MoveText top or bottom is error !!!"
#define MMIPIC_EDIT_4004_112_2_18_2_45_49_44 "[MMIPIC] ConvertBmp565to555OsdImage input_565_buf is PNULL !!!"
#define MMIPIC_EDIT_4010_112_2_18_2_45_49_45 "[MMIPIC] ConvertBmp565to555OsdImage output_555_buf is PNULL !!!"
#define MMIPIC_EDIT_4016_112_2_18_2_45_49_46 "[MMIPIC] ConvertBmp565to555OsdImage image_height is 0 !!!"
#define MMIPIC_EDIT_4022_112_2_18_2_45_49_47 "[MMIPIC] ConvertBmp565to555OsdImage image_width is 0 !!!"
#define MMIPIC_EDIT_4103_112_2_18_2_45_49_48 "[MMIPIC] MMIPIC_GetFrameImage FRAME is bigger!!!"
#define MMIPIC_EDIT_4285_112_2_18_2_45_49_49 "MMIPIC_GetEffectImage:effort error mmi_effect =%d"
#define MMIPIC_EDIT_4428_112_2_18_2_45_50_50 "[MMIPIC] AdjustOffset down is error !!!"
#define MMIPIC_EDIT_4496_112_2_18_2_45_50_51 "[MMIPIC]: MMIPIC_AdjustRGB, input_param.src_ptr = 0x%x, input_param.out_buf_ptr = 0x%x"
#define MMIPIC_EDIT_4922_112_2_18_2_45_51_52 "[MMIPIC]: MMIPIC_SetRGBGradeValue, input_param.src_ptr = 0x%x, input_param.out_buf_ptr = 0x%x"
#define MMIPIC_EDIT_5115_112_2_18_2_45_51_53 "[MMIPIC] MMIPIC_SetAdjustmentValue grade = %d,adjustment = %d"
#define MMIPIC_EDIT_WINTAB_953_112_2_18_2_45_56_54 "[MMIPIC] full_path_ptr is PNULL"
#define MMIPIC_EDIT_WINTAB_961_112_2_18_2_45_56_55 "[MMIPIC] s_selected_folder_ptr is PNULL"
#define MMIPIC_EDIT_WINTAB_969_112_2_18_2_45_56_56 "[MMIPIC] g_random_name_ptr is PNULL"
#define MMIPIC_EDIT_WINTAB_978_112_2_18_2_45_56_57 "[MMIPIC] file_info_ptr->full_path_wstr_ptr is PNULL"
#define MMIPIC_EDIT_WINTAB_1214_112_2_18_2_45_57_58 "[MMIPIC] GetProgressBarStateForTP rect_index = %d, grade_level = %d, new_adjust = %"
#define MMIPIC_EDIT_WINTAB_1274_112_2_18_2_45_57_59 "HandleEditorMainWinMsg: MSG_OPEN_WINDOW"
#define MMIPIC_EDIT_WINTAB_1307_112_2_18_2_45_57_60 "HandleEditorMainWinMsg: enter pic editor  "
#define MMIPIC_EDIT_WINTAB_1322_112_2_18_2_45_57_61 "HandleEditorMainWinMsg: MSG_FULL_PAINT s_is_decoded = %d"
#define MMIPIC_EDIT_WINTAB_1333_112_2_18_2_45_57_62 "HandleEditorMainWinMsg: get file date success then begin decode jpg image "
#define MMIPIC_EDIT_WINTAB_1361_112_2_18_2_45_57_63 "HandleEditorMainWinMsg:  decode jpg image success "
#define MMIPIC_EDIT_WINTAB_1375_112_2_18_2_45_57_64 "HandleEditorMainWinMsg: MSG_GET_FOCUS"
#define MMIPIC_EDIT_WINTAB_1400_112_2_18_2_45_57_65 "HandleEditorMainWinMsg: MSG_LOSE_FOCUS"
#define MMIPIC_EDIT_WINTAB_1527_112_2_18_2_45_57_66 "HandleEditorMainWinMsg: MSG_APP_RED"
#define MMIPIC_EDIT_WINTAB_1561_112_2_18_2_45_57_67 "HandleEditorMainWinMsg: MSG_CLOSE_WINDOW"
#define MMIPIC_EDIT_WINTAB_1563_112_2_18_2_45_57_68 "Editor CloseWin s_operation_times = %d"
#define MMIPIC_EDIT_WINTAB_1905_112_2_18_2_45_58_69 "[MMIPIC] MMIPIC_GetNameAndSuffix name_suffix_ptr is PNULL !!!"
#define MMIPIC_EDIT_WINTAB_1911_112_2_18_2_45_58_70 "[MMIPIC] MMIPIC_GetNameAndSuffix name_suffix_len is 0 !!!"
#define MMIPIC_EDIT_WINTAB_1920_112_2_18_2_45_58_71 "[MMIPIC] MMIPIC_GetNameAndSuffix name_ptr or suffix_ptr is PNULL !!!"
#define MMIPIC_EDIT_WINTAB_2911_112_2_18_2_46_0_72 "[MMIPIC] EditWinGetFocusOutsideRect rect_ptr is PNULL !!!"
#define MMIPIC_EDIT_WINTAB_3807_112_2_18_2_46_2_73 "[MMIPIC] SetFileNameWinTitle file name = PNULL"
#define MMIPIC_EDIT_WINTAB_3812_112_2_18_2_46_2_74 "[MMIPIC] SetFileNameWinTitle name len = 0"
#define MMIPIC_EDIT_WINTAB_5190_112_2_18_2_46_5_75 "[MMIPIC] s_adjustment = %d"
#define MMIPIC_EDIT_WINTAB_5976_112_2_18_2_46_7_76 "[MMIPIC]:GetFilePathLen pWString is PNULL"
#define MMIPIC_EDIT_WINTAB_6003_112_2_18_2_46_7_77 "[MMIPIC]: OpenSelectPathCallBack is_success = %d!"
#define MMIPIC_EDIT_WINTAB_6007_112_2_18_2_46_7_78 "[MMIPIC]: OpenSelectPathCallBack file_num = %d"
#define MMIPIC_EDIT_WINTAB_6109_112_2_18_2_46_7_79 "HandleCopyFileWaitWinMsg  copy_result=%d"
#define MMIPIC_EDIT_WINTAB_6509_112_2_18_2_46_8_80 "SetPicEditTitle: title_height = %d"
#define MMIPIC_EDIT_WINTAB_6797_112_2_18_2_46_8_81 "[mmipic_edit_wintab]HandleSaveMsg: case MSG_OPEN_WINDOW"
#define MMIPIC_EDIT_WINTAB_6849_112_2_18_2_46_8_82 "[mmipic_edit_wintab]SaveImgFile: pic_result = "
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PIC_TRC)
TRACE_MSG(MMIPIC_EDIT_147_112_2_18_2_45_41_0,"[MMIPIC] MMIPIC_GetCommonFileDataBuf file_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_154_112_2_18_2_45_41_1,"[MMIPIC] MMIPIC_GetCommonFileDataBuf full_path_wstr_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_161_112_2_18_2_45_41_2,"[MMIPIC] MMIPIC_GetCommonFileDataBuf full_path_wstr_len is 0 !!!")
TRACE_MSG(MMIPIC_EDIT_175_112_2_18_2_45_41_3,"[MMIPIC] s_pic_editor_info.file_data_buffer is PNULL")
TRACE_MSG(MMIPIC_EDIT_190_112_2_18_2_45_41_4,"MMIPIC_GetCommonFileDataBuf: read picture is %d")
TRACE_MSG(MMIPIC_EDIT_230_112_2_18_2_45_41_5,"[MMIPIC] PicGetPicFileSize full_path_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_275_112_2_18_2_45_41_6,"[MMIPIC] MMIPIC_DecodeJpgImage file_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_282_112_2_18_2_45_41_7,"[MMIPIC] MMIPIC_DecodeJpgImage full_path_wstr_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_289_112_2_18_2_45_41_8,"[MMIPIC] MMIPIC_DecodeJpgImage full_path_wstr_len is 0 !!!")
TRACE_MSG(MMIPIC_EDIT_317_112_2_18_2_45_41_9,"[MMIPIC] img_output.decode_data_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_336_112_2_18_2_45_41_10,"[MMIPIC] s_pic_editor_info.display_bitmap_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_346_112_2_18_2_45_41_11,"[MMIPIC] s_pic_editor_info.display_bitmap_ptr->bit_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_385_112_2_18_2_45_42_12,"[MMIPIC] MMIPIC_DecodeJpgImage result = %d")
TRACE_MSG(MMIPIC_EDIT_397_112_2_18_2_45_42_13,"[MMIPIC] MMIPIC_PicEditorInit file_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_403_112_2_18_2_45_42_14,"[MMIPIC] MMIPIC_PicEditorInit full_path_wstr_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_409_112_2_18_2_45_42_15,"[MMIPIC] MMIPIC_PicEditorInit full_path_wstr_len is 0 !!!")
TRACE_MSG(MMIPIC_EDIT_415_112_2_18_2_45_42_16,"MMIPIC_PicEditorInit")
TRACE_MSG(MMIPIC_EDIT_436_112_2_18_2_45_42_17,"[MMIPIC s_pic_editor_info.undo_buf_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_455_112_2_18_2_45_42_18,"[MMIPIC] s_pic_editor_info.process_buf_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_462_112_2_18_2_45_42_19,"[MMIPIC] s_pic_editor_info.output_buf_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_645_112_2_18_2_45_42_20,"[MMIPIC] MMIPIC_DisplayImage display_bitmap_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_651_112_2_18_2_45_42_21,"[MMIPIC] MMIPIC_DisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_966_112_2_18_2_45_43_22,"[MMIPIC] MMIPIC_DisplayOutputImage display_bitmap_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_972_112_2_18_2_45_43_23,"[MMIPIC] MMIPIC_DisplayOutputImage output_buf_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_1388_112_2_18_2_45_44_24,"MMIPIC_SaveToUndoBuf: save buf and width heigh and x,y ")
TRACE_MSG(MMIPIC_EDIT_1412_112_2_18_2_45_44_25,"[MMIPIC] MMIPIC_OnlySaveOrgImgToUndoBuf: save buf and width heigh and x,y ")
TRACE_MSG(MMIPIC_EDIT_1473_112_2_18_2_45_44_26,"[MMIPIC] MMIPIC_Save filename len=%d")
TRACE_MSG(MMIPIC_EDIT_1474_112_2_18_2_45_44_27,"[MMIPIC] MMIPIC_Save dir_name_len = %d")
TRACE_MSG(MMIPIC_EDIT_1607_112_2_18_2_45_44_28,"[MMIPIC] MMIPIC_GetFileInfo param_to_save_as_win is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_1647_112_2_18_2_45_44_29,"[MMIPIC] MMIPIC_SaveAs full_path_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_1654_112_2_18_2_45_44_30,"[MMIPIC] MMIPIC_SaveAs full_path_len is 0 !!!")
TRACE_MSG(MMIPIC_EDIT_1664_112_2_18_2_45_44_31,"MMIPIC_SaveAs is_changed=%d")
TRACE_MSG(MMIPIC_EDIT_1699_112_2_18_2_45_44_32,"MMIAPIFMM_CopyOneFile ret=%d")
TRACE_MSG(MMIPIC_EDIT_1859_112_2_18_2_45_45_33,"[MMIPIC] s_icon_image[i].icon_bitmap_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_1871_112_2_18_2_45_45_34,"[MMIPIC] s_icon_image[i].icon_bitmap_ptr->bit_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_1999_112_2_18_2_45_45_35,"free rotate preview image buf")
TRACE_MSG(MMIPIC_EDIT_2232_112_2_18_2_45_45_36,"[MMIPIC] s_pic_editor_info.osd_image.osd_data_buffer is PNULL")
TRACE_MSG(MMIPIC_EDIT_2331_112_2_18_2_45_46_37,"[MMIPIC] s_pic_editor_info.osd_image.osd_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_2418_112_2_18_2_45_46_38,"[MMIPIC] GetPicTextStyle text_style_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_2476_112_2_18_2_45_46_39,"[MMIPIC] MMIPIC_DrawText text_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_2633_112_2_18_2_45_46_40,"[MMIPIC] MMIPIC_DrawText text_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_2804_112_2_18_2_45_46_41,"[MMIPIC] MMIPIC_MoveText text_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_2810_112_2_18_2_45_46_42,"[MMIPIC] MMIPIC_MoveText left or right is error !!!")
TRACE_MSG(MMIPIC_EDIT_2816_112_2_18_2_45_46_43,"[MMIPIC] MMIPIC_MoveText top or bottom is error !!!")
TRACE_MSG(MMIPIC_EDIT_4004_112_2_18_2_45_49_44,"[MMIPIC] ConvertBmp565to555OsdImage input_565_buf is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_4010_112_2_18_2_45_49_45,"[MMIPIC] ConvertBmp565to555OsdImage output_555_buf is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_4016_112_2_18_2_45_49_46,"[MMIPIC] ConvertBmp565to555OsdImage image_height is 0 !!!")
TRACE_MSG(MMIPIC_EDIT_4022_112_2_18_2_45_49_47,"[MMIPIC] ConvertBmp565to555OsdImage image_width is 0 !!!")
TRACE_MSG(MMIPIC_EDIT_4103_112_2_18_2_45_49_48,"[MMIPIC] MMIPIC_GetFrameImage FRAME is bigger!!!")
TRACE_MSG(MMIPIC_EDIT_4285_112_2_18_2_45_49_49,"MMIPIC_GetEffectImage:effort error mmi_effect =%d")
TRACE_MSG(MMIPIC_EDIT_4428_112_2_18_2_45_50_50,"[MMIPIC] AdjustOffset down is error !!!")
TRACE_MSG(MMIPIC_EDIT_4496_112_2_18_2_45_50_51,"[MMIPIC]: MMIPIC_AdjustRGB, input_param.src_ptr = 0x%x, input_param.out_buf_ptr = 0x%x")
TRACE_MSG(MMIPIC_EDIT_4922_112_2_18_2_45_51_52,"[MMIPIC]: MMIPIC_SetRGBGradeValue, input_param.src_ptr = 0x%x, input_param.out_buf_ptr = 0x%x")
TRACE_MSG(MMIPIC_EDIT_5115_112_2_18_2_45_51_53,"[MMIPIC] MMIPIC_SetAdjustmentValue grade = %d,adjustment = %d")
TRACE_MSG(MMIPIC_EDIT_WINTAB_953_112_2_18_2_45_56_54,"[MMIPIC] full_path_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_WINTAB_961_112_2_18_2_45_56_55,"[MMIPIC] s_selected_folder_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_WINTAB_969_112_2_18_2_45_56_56,"[MMIPIC] g_random_name_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_WINTAB_978_112_2_18_2_45_56_57,"[MMIPIC] file_info_ptr->full_path_wstr_ptr is PNULL")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1214_112_2_18_2_45_57_58,"[MMIPIC] GetProgressBarStateForTP rect_index = %d, grade_level = %d, new_adjust = %")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1274_112_2_18_2_45_57_59,"HandleEditorMainWinMsg: MSG_OPEN_WINDOW")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1307_112_2_18_2_45_57_60,"HandleEditorMainWinMsg: enter pic editor  ")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1322_112_2_18_2_45_57_61,"HandleEditorMainWinMsg: MSG_FULL_PAINT s_is_decoded = %d")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1333_112_2_18_2_45_57_62,"HandleEditorMainWinMsg: get file date success then begin decode jpg image ")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1361_112_2_18_2_45_57_63,"HandleEditorMainWinMsg:  decode jpg image success ")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1375_112_2_18_2_45_57_64,"HandleEditorMainWinMsg: MSG_GET_FOCUS")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1400_112_2_18_2_45_57_65,"HandleEditorMainWinMsg: MSG_LOSE_FOCUS")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1527_112_2_18_2_45_57_66,"HandleEditorMainWinMsg: MSG_APP_RED")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1561_112_2_18_2_45_57_67,"HandleEditorMainWinMsg: MSG_CLOSE_WINDOW")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1563_112_2_18_2_45_57_68,"Editor CloseWin s_operation_times = %d")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1905_112_2_18_2_45_58_69,"[MMIPIC] MMIPIC_GetNameAndSuffix name_suffix_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1911_112_2_18_2_45_58_70,"[MMIPIC] MMIPIC_GetNameAndSuffix name_suffix_len is 0 !!!")
TRACE_MSG(MMIPIC_EDIT_WINTAB_1920_112_2_18_2_45_58_71,"[MMIPIC] MMIPIC_GetNameAndSuffix name_ptr or suffix_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_WINTAB_2911_112_2_18_2_46_0_72,"[MMIPIC] EditWinGetFocusOutsideRect rect_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_EDIT_WINTAB_3807_112_2_18_2_46_2_73,"[MMIPIC] SetFileNameWinTitle file name = PNULL")
TRACE_MSG(MMIPIC_EDIT_WINTAB_3812_112_2_18_2_46_2_74,"[MMIPIC] SetFileNameWinTitle name len = 0")
TRACE_MSG(MMIPIC_EDIT_WINTAB_5190_112_2_18_2_46_5_75,"[MMIPIC] s_adjustment = %d")
TRACE_MSG(MMIPIC_EDIT_WINTAB_5976_112_2_18_2_46_7_76,"[MMIPIC]:GetFilePathLen pWString is PNULL")
TRACE_MSG(MMIPIC_EDIT_WINTAB_6003_112_2_18_2_46_7_77,"[MMIPIC]: OpenSelectPathCallBack is_success = %d!")
TRACE_MSG(MMIPIC_EDIT_WINTAB_6007_112_2_18_2_46_7_78,"[MMIPIC]: OpenSelectPathCallBack file_num = %d")
TRACE_MSG(MMIPIC_EDIT_WINTAB_6109_112_2_18_2_46_7_79,"HandleCopyFileWaitWinMsg  copy_result=%d")
TRACE_MSG(MMIPIC_EDIT_WINTAB_6509_112_2_18_2_46_8_80,"SetPicEditTitle: title_height = %d")
TRACE_MSG(MMIPIC_EDIT_WINTAB_6797_112_2_18_2_46_8_81,"[mmipic_edit_wintab]HandleSaveMsg: case MSG_OPEN_WINDOW")
TRACE_MSG(MMIPIC_EDIT_WINTAB_6849_112_2_18_2_46_8_82,"[mmipic_edit_wintab]SaveImgFile: pic_result = ")
END_TRACE_MAP(MMI_APP_PIC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PIC_TRC_H_

