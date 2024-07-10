/******************************************************************************
 ** File Name:      mmi_app_ocr_trc.h                                         *
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
//trace_id:202
#ifndef _MMI_APP_OCR_TRC_H_
#define _MMI_APP_OCR_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIOCR_108_112_2_18_2_42_5_0 "[MMIOCR]:MMIOCR_HandleOCRMsg: msg_id %d"
#define MMIOCR_184_112_2_18_2_42_6_1 "[MMIOCR]: SnapShot return: 0x%.2x"
#define MMIOCR_209_112_2_18_2_42_6_2 "[MMIOCR]: DCAdap_CaptureCallBack, data_len =%d ret = %d"
#define MMIOCR_237_112_2_18_2_42_6_3 "[MMIOCR]: MMIOCR_ReviewCapturedPhotos, review failed"
#define MMIOCR_294_112_2_18_2_42_6_4 "[MMIOCR]: review_param image_src_rect w=%d,h=%d"
#define MMIOCR_306_112_2_18_2_42_6_5 "[MMIOCR]: ReviewPhotoFromBuf return 0x%.2x"
#define MMIOCR_324_112_2_18_2_42_6_6 "[MMIOCR]: ReviewPhotoCallBack, ret_value = %d"
#define MMIOCR_380_112_2_18_2_42_6_7 "[MMIOCR]:result[%d].wClass[0] = 0x%x"
#define MMIOCR_387_112_2_18_2_42_6_8 "[MMIOCR]:enter ENG name processs flag[0]= %d"
#define MMIOCR_413_112_2_18_2_42_6_9 "[MMIOCR]:enter email process flag[1]= %d"
#define MMIOCR_431_112_2_18_2_42_6_10 "[MMIOCR]:enter mobile number process flag[2]= %d"
#define MMIOCR_452_112_2_18_2_42_6_11 "[MMIOCR]:enter company number process flag[3]= %d"
#define MMIOCR_470_112_2_18_2_42_6_12 "[MMIOCR]:enter  zipcode  process flag[4]= %d"
#define MMIOCR_EXPORT_81_112_2_18_2_42_7_13 "[MMIOCR]: MMIAPIOCR_HandleProcessTimer %d"
#define MMIOCR_LIB_216_112_2_18_2_42_8_14 "[MMIOCR]:OCRlib_ThreadAutoRecogEntry: OCRlib_AutoRecog done."
#define MMIOCR_LIB_255_112_2_18_2_42_8_15 "[MMIOCR]:Recognize OK, Classified regions= %d\n"
#define MMIOCR_LIB_335_112_2_18_2_42_8_16 "[MMIOCR]:ocr_autorecog_task_close,task_id = %d"
#define MMIOCR_LIB_397_112_2_18_2_42_8_17 "[MMIOCR]:FreeOCRBuffer(), s_ocr_lib_buffer=0x%08lx"
#define MMIOCR_LIB_420_112_2_18_2_42_8_18 "[MMIOCR]:AllocOCRBuffer(), add=0x%x"
#define MMIOCR_LIB_452_112_2_18_2_42_8_19 "[MMIOCR]:OCRlib_Initialize: OCR LIB VER %d"
#define MMIOCR_LIB_477_112_2_18_2_42_8_20 "[MMIOCR]:ocr initialize failed, error code = %d"
#define MMIOCR_LIB_491_112_2_18_2_42_9_21 "[MMIOCR]:ocr set image failed, error code = %d"
#define MMIOCR_WINTAB_180_112_2_18_2_42_9_22 "[MMIOCR]: MMIOCR_OpenPreviewWin, ret = %d"
#define MMIOCR_WINTAB_205_112_2_18_2_42_9_23 "\" AutoFocusCallback : result %d"
#define MMIOCR_WINTAB_230_112_2_18_2_42_9_24 "[MMIOCR]: MMIOCR_HandlePreviewWinMsg, msg_id = 0x%.2x"
#define MMIOCR_WINTAB_385_112_2_18_2_42_10_25 "[MMIOCR]::HandlePreviewWinMsg: MSG_CLOSE_WINDOW, stop dc error!"
#define MMIOCR_WINTAB_390_112_2_18_2_42_10_26 "[MMIOCR]: HandlePreviewWinMsg: MSG_CLOSE_WINDOW, close dc error! "
#define MMIOCR_WINTAB_429_112_2_18_2_42_10_27 "[MMIOCR]: HandleCaptureWinMsg, msg_id = 0x%.2x"
#define MMIOCR_WINTAB_812_112_2_18_2_42_11_28 "[MMIOCR]:mmiocr_wintab.c:OCR_AutoRecog fail"
#define MMIOCR_WINTAB_852_112_2_18_2_42_11_29 "[MMIOCR]: Start Preview retrun: 0x%.2x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_OCR_TRC)
TRACE_MSG(MMIOCR_108_112_2_18_2_42_5_0,"[MMIOCR]:MMIOCR_HandleOCRMsg: msg_id %d")
TRACE_MSG(MMIOCR_184_112_2_18_2_42_6_1,"[MMIOCR]: SnapShot return: 0x%.2x")
TRACE_MSG(MMIOCR_209_112_2_18_2_42_6_2,"[MMIOCR]: DCAdap_CaptureCallBack, data_len =%d ret = %d")
TRACE_MSG(MMIOCR_237_112_2_18_2_42_6_3,"[MMIOCR]: MMIOCR_ReviewCapturedPhotos, review failed")
TRACE_MSG(MMIOCR_294_112_2_18_2_42_6_4,"[MMIOCR]: review_param image_src_rect w=%d,h=%d")
TRACE_MSG(MMIOCR_306_112_2_18_2_42_6_5,"[MMIOCR]: ReviewPhotoFromBuf return 0x%.2x")
TRACE_MSG(MMIOCR_324_112_2_18_2_42_6_6,"[MMIOCR]: ReviewPhotoCallBack, ret_value = %d")
TRACE_MSG(MMIOCR_380_112_2_18_2_42_6_7,"[MMIOCR]:result[%d].wClass[0] = 0x%x")
TRACE_MSG(MMIOCR_387_112_2_18_2_42_6_8,"[MMIOCR]:enter ENG name processs flag[0]= %d")
TRACE_MSG(MMIOCR_413_112_2_18_2_42_6_9,"[MMIOCR]:enter email process flag[1]= %d")
TRACE_MSG(MMIOCR_431_112_2_18_2_42_6_10,"[MMIOCR]:enter mobile number process flag[2]= %d")
TRACE_MSG(MMIOCR_452_112_2_18_2_42_6_11,"[MMIOCR]:enter company number process flag[3]= %d")
TRACE_MSG(MMIOCR_470_112_2_18_2_42_6_12,"[MMIOCR]:enter  zipcode  process flag[4]= %d")
TRACE_MSG(MMIOCR_EXPORT_81_112_2_18_2_42_7_13,"[MMIOCR]: MMIAPIOCR_HandleProcessTimer %d")
TRACE_MSG(MMIOCR_LIB_216_112_2_18_2_42_8_14,"[MMIOCR]:OCRlib_ThreadAutoRecogEntry: OCRlib_AutoRecog done.")
TRACE_MSG(MMIOCR_LIB_255_112_2_18_2_42_8_15,"[MMIOCR]:Recognize OK, Classified regions= %d\n")
TRACE_MSG(MMIOCR_LIB_335_112_2_18_2_42_8_16,"[MMIOCR]:ocr_autorecog_task_close,task_id = %d")
TRACE_MSG(MMIOCR_LIB_397_112_2_18_2_42_8_17,"[MMIOCR]:FreeOCRBuffer(), s_ocr_lib_buffer=0x%08lx")
TRACE_MSG(MMIOCR_LIB_420_112_2_18_2_42_8_18,"[MMIOCR]:AllocOCRBuffer(), add=0x%x")
TRACE_MSG(MMIOCR_LIB_452_112_2_18_2_42_8_19,"[MMIOCR]:OCRlib_Initialize: OCR LIB VER %d")
TRACE_MSG(MMIOCR_LIB_477_112_2_18_2_42_8_20,"[MMIOCR]:ocr initialize failed, error code = %d")
TRACE_MSG(MMIOCR_LIB_491_112_2_18_2_42_9_21,"[MMIOCR]:ocr set image failed, error code = %d")
TRACE_MSG(MMIOCR_WINTAB_180_112_2_18_2_42_9_22,"[MMIOCR]: MMIOCR_OpenPreviewWin, ret = %d")
TRACE_MSG(MMIOCR_WINTAB_205_112_2_18_2_42_9_23,"\" AutoFocusCallback : result %d")
TRACE_MSG(MMIOCR_WINTAB_230_112_2_18_2_42_9_24,"[MMIOCR]: MMIOCR_HandlePreviewWinMsg, msg_id = 0x%.2x")
TRACE_MSG(MMIOCR_WINTAB_385_112_2_18_2_42_10_25,"[MMIOCR]::HandlePreviewWinMsg: MSG_CLOSE_WINDOW, stop dc error!")
TRACE_MSG(MMIOCR_WINTAB_390_112_2_18_2_42_10_26,"[MMIOCR]: HandlePreviewWinMsg: MSG_CLOSE_WINDOW, close dc error! ")
TRACE_MSG(MMIOCR_WINTAB_429_112_2_18_2_42_10_27,"[MMIOCR]: HandleCaptureWinMsg, msg_id = 0x%.2x")
TRACE_MSG(MMIOCR_WINTAB_812_112_2_18_2_42_11_28,"[MMIOCR]:mmiocr_wintab.c:OCR_AutoRecog fail")
TRACE_MSG(MMIOCR_WINTAB_852_112_2_18_2_42_11_29,"[MMIOCR]: Start Preview retrun: 0x%.2x")
END_TRACE_MAP(MMI_APP_OCR_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_OCR_TRC_H_

