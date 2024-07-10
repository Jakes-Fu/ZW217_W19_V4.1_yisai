/********************************************************************************
 ** File Name:		ocr_driver.h												*
 ** Author:			Jing.Li                                                 	*
 ** DATE:			09/12/2006 													*
 ** Copyright:		2006 Spreatrum, Incoporated. All Rights Reserved. 			*
 ** Description: 																*
 ********************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **--------------------------------------------------------------------------- * 
 **DATE				NAME            DESCRIPTION                                 * 
 **09/12/2006		Jing.Li		    Create. 								  *
 ******************************************************************************/
#ifndef _OCR_DRIVER_H_
    #define _OCR_DRIVER_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "sci_types.h"
//#include "digitalcamera.h"
#include "tw_ocr32.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define OCR_MAX_RECOG_REGION_NUM	5

extern uint32 GetYUVExtBuffer(void);
extern uint32 GetYUVExtBufferSize (void);

#define OCR_YUV_BUF         (GetYUVExtBuffer())
#define OCR_YUV_BUF_SIZE    (GetYUVExtBufferSize())

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
//preview parameter
typedef DC_PREVIEW_PARAM_T OCR_PREVIEW_PARAM_T;

//capture parameter
typedef DC_CAPTURE_PARAM_T OCR_CAPTURE_PARAM_T;

//review parameter
typedef struct{
	DC_REVIEW_PARAM_T st_review_param;
	uint8 *buf;
	uint32 size;
}OCR_REVIEW_PARAM_T;

//OSD mode struct
typedef DC_OSD_MODE OCR_OSD_MODE_T;

typedef TH_CARD_INFO OCR_CARD_INFO;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    OCR Init function
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			buf_ptr: buffer point(used for store picture)
//					buf_size: buffer size
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           If either buf_ptr or buf_size is zero, OCR will use static buffer 
//					defined by itself.
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_Init(uint8* buf_ptr, uint32 buf_size); 

/*****************************************************************************/
//  Description:    OCR Set parameter&Start preview
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			pre_param: parameter struct pointer
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           If input is NULL, OCR will use default parameter.
//					It will do setting parameter and start preview.
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_Preview(OCR_PREVIEW_PARAM_T *pre_param); 

/*****************************************************************************/
//  Description:    OCR Set preview parameter
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			pre_param: parameter struct pointer
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           If input is NULL, OCR will use default parameter.
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_SetPreviewParam(OCR_PREVIEW_PARAM_T *pre_param);

/*****************************************************************************/
//  Description:    OCR Start preview
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			None
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           None
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_StartPreview(void);

/*****************************************************************************/
//  Description:    OCR Stop preview
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			None
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           None
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_StopPreview(void);

/*****************************************************************************/
//  Description:    OCR do auto focus function
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			None
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           None
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_DoFocus(void);

/*****************************************************************************/
//  Description:    OCR Set parameter&Do Capture
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			cap_param: parameter struct pointer
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           If input is NULL, OCR will use default parameter.
//					It will do setting parameter and capturing.
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_Capture(OCR_CAPTURE_PARAM_T *cap_param);

/*****************************************************************************/
//  Description:    OCR set capture parameter
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			cap_param: parameter struct pointer
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           If input is NULL, OCR will use default parameter.
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_SetCaptureParam(OCR_CAPTURE_PARAM_T *cap_param);

/*****************************************************************************/
//  Description:    OCR Do Capture
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			None
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           None
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_DoCapture(void);

/*****************************************************************************/
//  Description:    OCR Set parameter&Do review
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			review_param: parameter struct pointer
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           If any input is NULL, OCR will use default parameter.
//					It will do setting parameter and reviewing.
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_Review(OCR_REVIEW_PARAM_T *review_param);

/*****************************************************************************/
//  Description:    OCR Set parameter
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			review_param: parameter struct pointer
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           If any input is NULL, OCR will use default parameter.
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_SetReviewParam(OCR_REVIEW_PARAM_T *review_param);

/******************************************************************************/
//	Description: 	OCR return review parameters
//	Author:			Xiaoguang.Feng
//	Input:			none
//	Output:     	none
//	Return:     	the review parameter
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_REVIEW_PARAM_T OCR_GetReviewParam(void);

/*****************************************************************************/
//  Description:    OCR Do review
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			None
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           None
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_DoReview(
							uint8 *p_buf, 
							uint32 len,
							PLAYBACK_SET_FUNC pf_callback);

/*****************************************************************************/
//  Description:    OCR Close function
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			None
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           None
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_Close(void);

// OSD may be chip related
/*****************************************************************************/
//  Description:    OCR Set OSD Zone mode
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			i_zone      which zone to be config: 0~5 (PRODUCT_DRIVER_SC6600M)
//             		pst_mode    zone mode
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           None
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_SetOsdZoneMode(int i_zone, const OCR_OSD_MODE_T *pst_mode);

/******************************************************************************/
//	Description:	OCR config osd image data
//	Author:			Jing.Li
//	Input:   		i_zone      the osd image in which zone
//  	           	p_image		point to image data, image data size is fixed on 256 bytes
//								data format is shown as following:
//								bit[7] bit[6]----- osd operation mode: 
//												00 transparent, 
//												01 half-transparent
//												10 XOR
//												11 overwrite
//								bit[5]..bit[0]-----color index
//	Output:			none
//	Return:			0	successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:			None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_SetOsdImage(int i_zone, const uint8 *p_data);

/******************************************************************************/
//	Description: 	OCR config osd color palette
//	Author:			Jing.Li
//	Input: 			p_table     color palette, the table must have 64 entries
//								data format is shown as following:
//								bit[ 7]~bit[ 0]   V value(-128~127)
//                         		bit[15]~bit[ 8]   U value(-128~127)
//                         		bit[23]~bit[16]   Y value(-128~127)
//                         		bit[31]~bit[24]   reserved
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_SetOsdColorPalette(const uint32 *p_table);

/******************************************************************************/
//	Description: config osd image data when previewing/capturing/reviewing
//	Author:			Jing.Li
//	Input:      	i_zone      the osd image in which zone
//             		i_pixel     pixel id from 0~255. 0 top-left , 255 right-bottom 
//                   		    scaning from left to right and top to bottom.
//             		b_value     data value of this pixel
//                         		data format is shown as following:
//                         		bit[7] bit[6]----- osd operation mode: 
//                                            00 transparent, 
//                                            01 half-transparent
//                                            10 XOR
//                                            11 overwrite
//                         		bit[5]..bit[0]-----color index
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:      		None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_ModifyOsdImage(int i_zone, int i_pixel, uint8 b_value);

/******************************************************************************/
//	Description: 	OCR set osd enable mode
//	Author:			Jing.Li
//	Input:			i_flag      enable mode of osd zones
//                         		bit[i]  0, the i-th zone is disable
//                         		bit[i]  1, the i-th zone is enable
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_SetOsdEnableMode(int flag);

/******************************************************************************/
//	Description: 	OCR return picture data and size
//	Author:			Jing.Li
//	Input:			none
//	Output:     	buf_size: data buffer size
//	Return:     	data buffer
//	Note:       	None
/******************************************************************************/
PUBLIC uint8* OCR_GetYUVbuf(uint32 *buf_size);

/******************************************************************************/
//	Description: 	OCR layout picture
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_Layout(void);

/******************************************************************************/
//	Description: 	OCR return layout regions to application
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	struct pointer which layout region info is stored
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_LAYOUT_PARAMETERS_T* OCR_GetLayoutRegion(void);

/******************************************************************************/
//	Description: 	OCR set regions need to be recognized
//	Author:			Jing.Li
//	Input:			region_selected: index array which the region is selected
//					region_num: selected number
//	Output:     	none
//	Return:     	none
//	Note:       	None
/******************************************************************************/
PUBLIC void OCR_SetRecogParams(uint8* region_selected, uint8 region_num);

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
//	Description: 	OCR return recognition result
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	the result array pointer
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_CARD_INFO* OCR_GetRecogResult(void);

/******************************************************************************/
//	Description: 	OCR stop layout
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_LayoutStop(void);

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
//	Description: 	OCR draw funtion
//	Author:			Jing.Li
//	Input:			left, top, right, bottom: coordinate of image
//					buf_ptr: image buffer
//	Output:     	none
//	Return:     	0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_LCD_InvalidateImage(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom,	//bottom of the rectangle
	uint16 *buf_ptr //buffer pointer of the image to display
	);

/******************************************************************************/
//	Description: 	Get buffer used for OCR lib
//	Author:			Jing.Li
//	Input:			none
//	Output:     	buffer_size: buffer size
//	Return:     	buffer pointer
//	Note:       	None
/******************************************************************************/
PUBLIC uint8* OCR_GetOCRBuffer(uint32* buffer_size) ;

/*****************************************************************************/
//  Description:    OCR free resource function
//	Global resource dependence: 
//  Author:         Jing.Li
//	Input: 			None
//	Output:			None
//	Return			0: successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:           close ocr lib
/*****************************************************************************/
PUBLIC OCR_ERROR_E OCR_Free(void);

/******************************************************************************/
//	Description: 	Get display size of OCR
//	Author:			Jing.Li
//	Input:			none
//	Output:     	disp_width:	display width
//					disp_height:	display height
//	Return:     	buffer pointer
//	Note:       	None
/******************************************************************************/
PUBLIC void OCR_GetDispSize(uint32* disp_width, uint32* disp_height);

/******************************************************************************/
//	Description: 	get scale factor
//	Author:			Jing.Li
//	Input:			none
//	Output:     	scale_x: scale factor in x dimension
//					scale_y: scale factor in y dimension
//	Return:     	none
//	Note:       	None
/******************************************************************************/
PUBLIC void	OCR_GetScaleFactor(uint8 *scale_x, uint8 *scale_y);

/******************************************************************************/
//	Description: 	Get target OCR YUV data size
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	none
//	Note:       	None
/******************************************************************************/
PUBLIC uint32 OCR_GetOCRYUVDataSize(void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 

