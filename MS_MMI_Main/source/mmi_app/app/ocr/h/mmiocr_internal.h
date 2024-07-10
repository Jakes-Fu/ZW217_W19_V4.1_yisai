/*****************************************************************************
** File Name:      mmiocr_internal.h                                           *
** Author:         jian.ma                                               *
** Date:           06/2007                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe ocr modual internal interface*
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2007        jian.ma        Create
******************************************************************************/
#ifdef MMI_OCR_ENABLE
#ifndef _MMIOCR_INTERNAL_H_
#define _MMIOCR_INTERNAL_H_ 



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmk_type.h"
#include "mmiocr_text.h"
#include "mmidc_export.h"
//#include "mmidc_review.h"
#include "mmiocr_lib.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define OCR_MAX_RECOG_REGION_NUM 30     //(name,personal_number,company_number,mail_address,other(ZIPCODE))

#define OCR_PHOTO_SIZE_176X224_W				224
#define OCR_PHOTO_SIZE_176X224_H				176
#define OCR_PHOTO_SIZE_320X240_W				320
#define OCR_PHOTO_SIZE_320X240_H				240
#define OCR_PHOTO_SIZE_640X480_W				640
#define OCR_PHOTO_SIZE_640X480_H				480
#define OCR_PHOTO_SIZE_1280X1024_W			1280
#define OCR_PHOTO_SIZE_1280X1024_H			1024
#define OCR_PHOTO_SIZE_1600X1200_W			1600
#define OCR_PHOTO_SIZE_1600X1200_H			1200

#define MMI_OCR_PREVIEW_START_X         0
#define MMI_OCR_PREVIEW_START_Y         69
#define MMI_OCR_PREVIEW_WIDTH           MMI_MAINSCREEN_WIDTH
#define MMI_OCR_PREVIEW_HEIGHT          180
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct 
{
	uint8* data_buffer;
	uint32 data_len;
}OCR_DATA_INFO_T;

typedef enum
{
	OSD_OCR_WIN_CAPTURE_NONE,			
	OSD_OCR_WIN_CAPTURE_SNAPSHOTING,			// 显示拍摄中的OSD窗口	
	OSD_OCR_WIN_CAPTURE_DONE,
	OSD_OCR_WIN_CAPTURE_RECOG_START,
	OSD_OCR_WIN_CAPTURE_RECOGING,
	OSD_OCR_WIN_CAPTURE_RECOG_DONE,
	OSD_OCR_WIN_CAPTURE_MAX
}OSD_OCR_WIN_CAPTURE_MODE_E;					// 这个模式都是在一个窗口中（专门显示osd的窗口，和显示dc的窗口不是同一个），不同的模式只是在同一个有透明色的窗口中显示不同的 osd 图标。

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Handle the signal which come from dc AF.
//	Global resource dependence : none
//  Author: Annie.an
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIOCR_HandleOCRMsg(
									    PWND    app_ptr, 
									    uint16  msg_id, 
									    DPARAM  param
										);

/*****************************************************************************/
// 	Description : do capture
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_Capture(MMIDC_SETTING_INFO_T const *setting_ptr);


/*****************************************************************************/
// 	Description : review the captured photos
//	Global resource dependence : 
//  Author:gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_ReviewCapturedPhotos(uint32 owner_win_id, MULTI_SHOT_E multi_shot, BOOLEAN is_first_review, BOOLEAN is_full_rect);

/*****************************************************************************/
// 	Description : get image rect
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC BOOLEAN 	MMIOCR_GetImageRect(DCAMERA_RECT_T *image_rect);


/*****************************************************************************/
 // 	Description : According the result t.
 //	Global resource dependence : none
 //  Author: Annie.an
 //	Note:
 /*****************************************************************************/
 PUBLIC void MMIOCR_DisplayResultInPB(OCR_CARD_INFO *result);
 
 
/*****************************************************************************/
// 	Description : get buf data
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC uint8* MMIOCR_GetBufData(void);

/*****************************************************************************/
// 	Description : Set current capture osd mode
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIOCR_SetCurCaptureOSDMode(OSD_OCR_WIN_CAPTURE_MODE_E cur_mode);

/*****************************************************************************/
// 	Description : Get current capture osd mode
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC OSD_OCR_WIN_CAPTURE_MODE_E MMIOCR_GetCurCaptureOSDMode(void);

//*****************************************************************************/
// 	Description : alert DC_Open, DC_SetPreviewParam, DC_StartPreview wrong
//	Global resource dependence : none
//  Author: jian.ma 
//	Note:
/*****************************************************************************/
PUBLIC void MMIOCR_AlertOCRRecogWrong(void);

/*****************************************************************************/
// 	Description : start recog timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StartRecogTimer(void);

/*****************************************************************************/
// 	Description : stop recog timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StopRecogTimer(void);

/*****************************************************************************/
// 	Description : stop recog time out timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StopRecogTimeOutTimer(void);

/*****************************************************************************/
// 	Description : Update current capture osd 
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIOCR_UpdateCurCaptureOSD(void);

/*****************************************************************************/
// 	Description : stop all timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StopAllTimer(void);

/*****************************************************************************/
// 	Description : Set card info size num
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_SetRecogCardInfoSizeNum(uint32 recog_num);

/*****************************************************************************/
// 	Description : Get card info size num
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIOCR_GetRecogCardInfoSizeNum(void);

/*****************************************************************************/
// 	Description : start recog timerout timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StartRecogTimeOutTimer(void);

/*****************************************************************************/
// 	Description : use DC Driver interface to start preview
//	Global resource dependence : 
//  Author: gang.tong
//	Note:	这个函数用于Stop了Preview以后，再次Start
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_StartPreview(DC_OWNER_E owner);

/*****************************************************************************/
// 	Description : open preview window 
//	Global resource dependence : none
//  Author: jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_OpenPreviewWin(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

#endif