/******************************************************************************
 ** File Name:      mmi_app_pic_crop_trc.h                                         *
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
//trace_id:210
#ifndef _MMI_APP_PIC_CROP_TRC_H_
#define _MMI_APP_PIC_CROP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPIC_CROP_130_112_2_18_2_46_11_0 "[MMIPICCROP] MMIPICCROP_Init crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_135_112_2_18_2_46_11_1 "[MMIPICCROP] MMIPICCROP_Init crop_src is PNULL !!!"
#define MMIPIC_CROP_138_112_2_18_2_46_11_2 "[MMIPICCROP]  MMIPICCROP_Init"
#define MMIPIC_CROP_172_112_2_18_2_46_11_3 "[MMIPICCROP] MMIPICCROP_Init crop_info_ptr->file_info.full_path_wstr_ptr is PNULL"
#define MMIPIC_CROP_208_112_2_18_2_46_11_4 "[MMIPICCROP] MMIPICCROP_Init crop_src is error !!!"
#define MMIPIC_CROP_215_112_2_18_2_46_11_5 "[MMIPICCROP] MMIPICCROP_Init crop_info_ptr->random_name_ptr is PNULL"
#define MMIPIC_CROP_294_112_2_18_2_46_11_6 "GetImgInfoByPath:get info result is %d!"
#define MMIPIC_CROP_363_112_2_18_2_46_11_7 "[MMIPICCROP] MMIPICCROP_DecodeJpgImage crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_374_112_2_18_2_46_11_8 "[MMIPICCROP] MMIPICCROP_DecodeJpgImage GetImgInfoByPath is FALSE"
#define MMIPIC_CROP_397_112_2_18_2_46_11_9 "[MMIPICCROP] MMIPICCROP_DecodeJpgImage img_output.decode_data_ptr is PNULL"
#define MMIPIC_CROP_419_112_2_18_2_46_11_10 "[MMIPICCROP] MMIPICCROP_DecodeJpgImage s_pic_editor_info.display_bitmap_ptr is PNULL"
#define MMIPIC_CROP_432_112_2_18_2_46_11_11 "[MMIPICCROP] MMIPICCROP_DecodeJpgImage s_pic_editor_info.display_bitmap_ptr->bit_ptr is PNULL"
#define MMIPIC_CROP_464_112_2_18_2_46_11_12 "[MMIPICCROP] MMIPICCROP_DecodeJpgImage err_type = %d"
#define MMIPIC_CROP_482_112_2_18_2_46_11_13 "[MMIPICCROP] CropGetOriginalImageRect crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_513_112_2_18_2_46_12_14 "[MMIPICCROP] CropGetFileIDString id >= id_max"
#define MMIPIC_CROP_550_112_2_18_2_46_12_15 "[MMIPICCROP] CropCreateSaveFileName crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_657_112_2_18_2_46_12_16 "[MMIPICCROP] MMIPICROP_Save crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_779_112_2_18_2_46_12_17 "[MMIPICCROP] MMIPICCROP_GetCropImageData crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_832_112_2_18_2_46_12_18 "[MMIPICCROP] MMIPICCROP_GetCropImageData img_output.decode_data_ptr is PNULL"
#define MMIPIC_CROP_853_112_2_18_2_46_12_19 "[MMIPICCROP] MMIPICCROP_GetCropImageData s_pic_editor_info.display_bitmap_ptr is PNULL"
#define MMIPIC_CROP_879_112_2_18_2_46_12_20 "[MMIPICCROP] MMIPICCROP_GetCropImageData err_type = %d"
#define MMIPIC_CROP_902_112_2_18_2_46_12_21 "[MMIPICCROP] MMIPICCROP_ShowMsg crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_964_112_2_18_2_46_12_22 "[MMIPICCROP] CropIsValidRect crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1011_112_2_18_2_46_13_23 "[MMIPICCROP] MMIPICCROP_IsPermitCrop crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1029_112_2_18_2_46_13_24 "[MMIPICCROP] MMIPICCROP_IsPermitCrop result = %d"
#define MMIPIC_CROP_1052_112_2_18_2_46_13_25 "[MMIPICCROP] CropHandleTpMsg crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1369_112_2_18_2_46_13_26 "[MMIPICCROP] CropDisplayBgAndTitle crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1392_112_2_18_2_46_13_27 "[MMIPICCROP] CropDisplayBgAndTitle crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1434_112_2_18_2_46_13_28 "[MMIPICCROP] CropFrameDisplay crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1515_112_2_18_2_46_14_29 "[MMIPICCROP] MMIPICCROP_DisplayImage"
#define MMIPIC_CROP_1518_112_2_18_2_46_14_30 "[MMIPICCROP] MMIPICCROP_DisplayImage crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1524_112_2_18_2_46_14_31 "[MMIPICCROP] MMIPICCROP_DisplayImage display_bitmap_ptr is PNULL !!!"
#define MMIPIC_CROP_1530_112_2_18_2_46_14_32 "[MMIPICCROP] MMIPICCROP_DisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!"
#define MMIPIC_CROP_1568_112_2_18_2_46_14_33 "[MMIPICCROP] MMIPICCROP_ErrorCallback crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1581_112_2_18_2_46_14_34 "[MMIPICCROP] MMIPICCROP_ErrorCallback err_type = %d"
#define MMIPIC_CROP_1596_112_2_18_2_46_14_35 "[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_1603_112_2_18_2_46_14_36 "[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.process_buf_ptr is PNULL"
#define MMIPIC_CROP_1610_112_2_18_2_46_14_37 "[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.output_buf_ptr is PNULL"
#define MMIPIC_CROP_1617_112_2_18_2_46_14_38 "[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.jpg_buf_ptr is PNULL"
#define MMIPIC_CROP_1635_112_2_18_2_46_14_39 "[MMIPICCROP] MMIPICCROP_Exit crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_WINTAB_191_112_2_18_2_46_15_40 "[MMIPICCROP] CropFrameRectInit crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_WINTAB_304_112_2_18_2_46_15_41 "[MMIPICCROP] HandleCropSaveWaitingWin crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_WINTAB_308_112_2_18_2_46_15_42 "[MMIPICCROP]: HandleCropSaveWaitingWin, msg=%x"
#define MMIPIC_CROP_WINTAB_320_112_2_18_2_46_15_43 "[MMIPICCROP] HandleCropSaveWaitingWin MMIPICCROP_GetCropImageData error!!!"
#define MMIPIC_CROP_WINTAB_327_112_2_18_2_46_15_44 "[MMIPICCROP]: HandleCropSaveWaitingWin, err_type=%x"
#define MMIPIC_CROP_WINTAB_386_112_2_18_2_46_15_45 "[MMIPICCROP] CropOkButtonProc crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_WINTAB_436_112_2_18_2_46_15_46 "[MMIPICCROP] CropSetCtrlParam crop_info_ptr is PNULL !!!"
#define MMIPIC_CROP_WINTAB_496_112_2_18_2_46_15_47 "HandlePicCropMainWinMsg: MSG_OPEN_WINDOW"
#define MMIPIC_CROP_WINTAB_514_112_2_18_2_46_15_48 "HandlePicCropMainWinMsg: enter pic editor  "
#define MMIPIC_CROP_WINTAB_517_112_2_18_2_46_15_49 "HandlePicCropMainWinMsg: MSG_FULL_PAINT s_is_decoded = %d"
#define MMIPIC_CROP_WINTAB_525_112_2_18_2_46_15_50 "HandlePicCropMainWinMsg: get file date success then begin decode jpg image "
#define MMIPIC_CROP_WINTAB_533_112_2_18_2_46_16_51 "HandlePicCropMainWinMsg:  decode jpg image success "
#define MMIPIC_CROP_WINTAB_549_112_2_18_2_46_16_52 "HandlePicCropMainWinMsg: MSG_GET_FOCUS"
#define MMIPIC_CROP_WINTAB_554_112_2_18_2_46_16_53 "HandlePicCropMainWinMsg: MSG_LOSE_FOCUS"
#define MMIPIC_CROP_WINTAB_592_112_2_18_2_46_16_54 "HandlePicCropMainWinMsg: MSG_APP_CANCEL"
#define MMIPIC_CROP_WINTAB_597_112_2_18_2_46_16_55 "HandlePicCropMainWinMsg: MSG_APP_RED"
#define MMIPIC_CROP_WINTAB_613_112_2_18_2_46_16_56 "HandlePicCropMainWinMsg: MSG_APP_DIRECTION"
#define MMIPIC_CROP_WINTAB_654_112_2_18_2_46_16_57 "HandlePicCropMainWinMsg: MSG_CLOSE_WINDOW"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PIC_CROP_TRC)
TRACE_MSG(MMIPIC_CROP_130_112_2_18_2_46_11_0,"[MMIPICCROP] MMIPICCROP_Init crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_135_112_2_18_2_46_11_1,"[MMIPICCROP] MMIPICCROP_Init crop_src is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_138_112_2_18_2_46_11_2,"[MMIPICCROP]  MMIPICCROP_Init")
TRACE_MSG(MMIPIC_CROP_172_112_2_18_2_46_11_3,"[MMIPICCROP] MMIPICCROP_Init crop_info_ptr->file_info.full_path_wstr_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_208_112_2_18_2_46_11_4,"[MMIPICCROP] MMIPICCROP_Init crop_src is error !!!")
TRACE_MSG(MMIPIC_CROP_215_112_2_18_2_46_11_5,"[MMIPICCROP] MMIPICCROP_Init crop_info_ptr->random_name_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_294_112_2_18_2_46_11_6,"GetImgInfoByPath:get info result is %d!")
TRACE_MSG(MMIPIC_CROP_363_112_2_18_2_46_11_7,"[MMIPICCROP] MMIPICCROP_DecodeJpgImage crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_374_112_2_18_2_46_11_8,"[MMIPICCROP] MMIPICCROP_DecodeJpgImage GetImgInfoByPath is FALSE")
TRACE_MSG(MMIPIC_CROP_397_112_2_18_2_46_11_9,"[MMIPICCROP] MMIPICCROP_DecodeJpgImage img_output.decode_data_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_419_112_2_18_2_46_11_10,"[MMIPICCROP] MMIPICCROP_DecodeJpgImage s_pic_editor_info.display_bitmap_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_432_112_2_18_2_46_11_11,"[MMIPICCROP] MMIPICCROP_DecodeJpgImage s_pic_editor_info.display_bitmap_ptr->bit_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_464_112_2_18_2_46_11_12,"[MMIPICCROP] MMIPICCROP_DecodeJpgImage err_type = %d")
TRACE_MSG(MMIPIC_CROP_482_112_2_18_2_46_11_13,"[MMIPICCROP] CropGetOriginalImageRect crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_513_112_2_18_2_46_12_14,"[MMIPICCROP] CropGetFileIDString id >= id_max")
TRACE_MSG(MMIPIC_CROP_550_112_2_18_2_46_12_15,"[MMIPICCROP] CropCreateSaveFileName crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_657_112_2_18_2_46_12_16,"[MMIPICCROP] MMIPICROP_Save crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_779_112_2_18_2_46_12_17,"[MMIPICCROP] MMIPICCROP_GetCropImageData crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_832_112_2_18_2_46_12_18,"[MMIPICCROP] MMIPICCROP_GetCropImageData img_output.decode_data_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_853_112_2_18_2_46_12_19,"[MMIPICCROP] MMIPICCROP_GetCropImageData s_pic_editor_info.display_bitmap_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_879_112_2_18_2_46_12_20,"[MMIPICCROP] MMIPICCROP_GetCropImageData err_type = %d")
TRACE_MSG(MMIPIC_CROP_902_112_2_18_2_46_12_21,"[MMIPICCROP] MMIPICCROP_ShowMsg crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_964_112_2_18_2_46_12_22,"[MMIPICCROP] CropIsValidRect crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1011_112_2_18_2_46_13_23,"[MMIPICCROP] MMIPICCROP_IsPermitCrop crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1029_112_2_18_2_46_13_24,"[MMIPICCROP] MMIPICCROP_IsPermitCrop result = %d")
TRACE_MSG(MMIPIC_CROP_1052_112_2_18_2_46_13_25,"[MMIPICCROP] CropHandleTpMsg crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1369_112_2_18_2_46_13_26,"[MMIPICCROP] CropDisplayBgAndTitle crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1392_112_2_18_2_46_13_27,"[MMIPICCROP] CropDisplayBgAndTitle crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1434_112_2_18_2_46_13_28,"[MMIPICCROP] CropFrameDisplay crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1515_112_2_18_2_46_14_29,"[MMIPICCROP] MMIPICCROP_DisplayImage")
TRACE_MSG(MMIPIC_CROP_1518_112_2_18_2_46_14_30,"[MMIPICCROP] MMIPICCROP_DisplayImage crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1524_112_2_18_2_46_14_31,"[MMIPICCROP] MMIPICCROP_DisplayImage display_bitmap_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1530_112_2_18_2_46_14_32,"[MMIPICCROP] MMIPICCROP_DisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1568_112_2_18_2_46_14_33,"[MMIPICCROP] MMIPICCROP_ErrorCallback crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1581_112_2_18_2_46_14_34,"[MMIPICCROP] MMIPICCROP_ErrorCallback err_type = %d")
TRACE_MSG(MMIPIC_CROP_1596_112_2_18_2_46_14_35,"[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_1603_112_2_18_2_46_14_36,"[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.process_buf_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_1610_112_2_18_2_46_14_37,"[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.output_buf_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_1617_112_2_18_2_46_14_38,"[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.jpg_buf_ptr is PNULL")
TRACE_MSG(MMIPIC_CROP_1635_112_2_18_2_46_14_39,"[MMIPICCROP] MMIPICCROP_Exit crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_WINTAB_191_112_2_18_2_46_15_40,"[MMIPICCROP] CropFrameRectInit crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_WINTAB_304_112_2_18_2_46_15_41,"[MMIPICCROP] HandleCropSaveWaitingWin crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_WINTAB_308_112_2_18_2_46_15_42,"[MMIPICCROP]: HandleCropSaveWaitingWin, msg=%x")
TRACE_MSG(MMIPIC_CROP_WINTAB_320_112_2_18_2_46_15_43,"[MMIPICCROP] HandleCropSaveWaitingWin MMIPICCROP_GetCropImageData error!!!")
TRACE_MSG(MMIPIC_CROP_WINTAB_327_112_2_18_2_46_15_44,"[MMIPICCROP]: HandleCropSaveWaitingWin, err_type=%x")
TRACE_MSG(MMIPIC_CROP_WINTAB_386_112_2_18_2_46_15_45,"[MMIPICCROP] CropOkButtonProc crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_WINTAB_436_112_2_18_2_46_15_46,"[MMIPICCROP] CropSetCtrlParam crop_info_ptr is PNULL !!!")
TRACE_MSG(MMIPIC_CROP_WINTAB_496_112_2_18_2_46_15_47,"HandlePicCropMainWinMsg: MSG_OPEN_WINDOW")
TRACE_MSG(MMIPIC_CROP_WINTAB_514_112_2_18_2_46_15_48,"HandlePicCropMainWinMsg: enter pic editor  ")
TRACE_MSG(MMIPIC_CROP_WINTAB_517_112_2_18_2_46_15_49,"HandlePicCropMainWinMsg: MSG_FULL_PAINT s_is_decoded = %d")
TRACE_MSG(MMIPIC_CROP_WINTAB_525_112_2_18_2_46_15_50,"HandlePicCropMainWinMsg: get file date success then begin decode jpg image ")
TRACE_MSG(MMIPIC_CROP_WINTAB_533_112_2_18_2_46_16_51,"HandlePicCropMainWinMsg:  decode jpg image success ")
TRACE_MSG(MMIPIC_CROP_WINTAB_549_112_2_18_2_46_16_52,"HandlePicCropMainWinMsg: MSG_GET_FOCUS")
TRACE_MSG(MMIPIC_CROP_WINTAB_554_112_2_18_2_46_16_53,"HandlePicCropMainWinMsg: MSG_LOSE_FOCUS")
TRACE_MSG(MMIPIC_CROP_WINTAB_592_112_2_18_2_46_16_54,"HandlePicCropMainWinMsg: MSG_APP_CANCEL")
TRACE_MSG(MMIPIC_CROP_WINTAB_597_112_2_18_2_46_16_55,"HandlePicCropMainWinMsg: MSG_APP_RED")
TRACE_MSG(MMIPIC_CROP_WINTAB_613_112_2_18_2_46_16_56,"HandlePicCropMainWinMsg: MSG_APP_DIRECTION")
TRACE_MSG(MMIPIC_CROP_WINTAB_654_112_2_18_2_46_16_57,"HandlePicCropMainWinMsg: MSG_CLOSE_WINDOW")
END_TRACE_MAP(MMI_APP_PIC_CROP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PIC_CROP_TRC_H_

