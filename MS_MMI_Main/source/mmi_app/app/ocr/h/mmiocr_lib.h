/******************************************************************************
 ** File Name:      thocr_lib.h                                              **
 ** Author:         Xiaoguang.Feng                                           **
 ** DATE:           08/23/2006                                               **
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        **
 ** Description:    load TH-OCR kernal					 					 **	
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                      **
 ** -------------------------------------------------------------------------**
 ** DATE           NAME             DESCRIPTION                              **
 ** 08/23/2006     Xiaoguang.Feng   Create.                                  **
 ******************************************************************************/
#ifdef MMI_OCR_ENABLE
#ifndef THOCR_LIB_H
#define THOCR_LIB_H

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "tw_ocr32.h"
#include "os_api.h"

/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define OCRLIB_MAX_LAYOUT_REGION 20

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
// OCR functions return
typedef TH_CARD_INFO OCR_CARD_INFO;
// OCR error type
typedef enum{
	OCR_NONE_ERROR,			//successful
	OCR_PARAM_ERROR, 		//parameter error
	OCR_NOT_OPEN,			//OCR not open
	OCR_OPENED_ERROR,		//OCR has been opened
	OCR_ERROR				//other error
}OCR_ERROR_E;

// image size
typedef struct img_size_tag
{
	uint32 width;
	uint32 height;
} IMG_SIZE_T;

// auto layout parameters
typedef struct ocr_layout_struct_tag
{
	IMG_SIZE_T img_size;
	uint32 num_of_regions;
	REGION* regions_ptr; //refer to tw_ocr32.h
} OCR_LAYOUT_PARAMETERS_T;

// auto recognize parameters
typedef struct ocr_recog_struct_tag
{
	uint32 num_of_regions;
	REGION* regions_ptr; //refer to tw_ocr32.h
	TH_CARD_INFO* ocr_results_ptr; //refer to tw_ocr32.h
} OCR_RECOG_PARAMETERS_T;

// ocr signal structure
typedef struct
{
    xSignalHeaderRec    sig;
    uint32        		param;
}
OCR_SIGNAL_T;

/******************************************************************************/
//	Description: 	OCR do auto recognition
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_AutoRecog(void);

/******************************************************************************/
//	Description: 	OCR stop auto recognition
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_AutoRecogStop(void);

/******************************************************************************/
//	Description: 	OCR return recognition result
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	the result array pointer
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_CARD_INFO* OCR_GetRecogResult(void);
#endif

#endif