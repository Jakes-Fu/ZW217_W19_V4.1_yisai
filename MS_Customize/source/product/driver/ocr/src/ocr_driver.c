/********************************************************************************
 ** File Name:		OCR_Driver.c													*
 ** Author:			Jing.Li                                                 	*
 ** DATE:			09/14/2006 													*
 ** Copyright:		2006 Spreatrum, Incoporated. All Rights Reserved. 			*
 ** Description: 																*
 ********************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **--------------------------------------------------------------------------- * 
 **DATE				NAME            DESCRIPTION                                 * 
 **09/14/2006		Jing.Li		    Create. 								  *
 ******************************************************************************/
#ifndef _OCR_DRIVER_C_
	#define _OCR_DRIVER_C_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "ms_customize_trc.h"
#include "os_api.h"
#include "tw_ocr32.h"
#include "ocr_driver.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
#include "sc6600m_freq.h"
#include "chng_freq.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
#define OCR_CAPTURE_SENSOR_MODE		(SENSOR_MODE_1280X1024_NIGHT)
#define OCR_PREVIEW_SENSOR_MODE		(SENSOR_MODE_640X480_NIGHT)

// #define OCR_LCD_WIDTH		(176)
// #define OCR_LCD_HEIGHT		(220)

#define OCR_DISP_WIDTH		(128)
#define OCR_DISP_HEIGHT		(160)

//#define OCR_SENSOR_WIDTH	(1280)
//#define OCR_SENSOR_HEIGHT	(1024)

#define OCR_TARGET_WIDTH	(1280)
#define OCR_TARGET_HEIGHT	(1024)

#define OCR_TARGET_SIZE     (OCR_TARGET_WIDTH * OCR_TARGET_HEIGHT * 2)
//#define OCR_TRIM_OFFSET_X	(224)
//#define OCR_TRIM_OFFSET_Y	(0)

//#define OCR_TRIM_WIDTH		(OCR_TARGET_WIDTH - 2*OCR_TRIM_OFFSET_X)
//#define OCR_TRIM_HEIGHT		(OCR_TARGET_HEIGHT - 2*OCR_TRIM_OFFSET_Y)

#define OCR_PREVIEW_TRIM_OFFSET_X	(128)
#define OCR_PREVIEW_TRIM_OFFSET_Y	(0)

#define OCR_PREVIEW_TRIM_WIDTH		(384)
#define OCR_PREVIEW_TRIM_HEIGHT		(480)

#define OCR_REVIEW_TRIM_OFFSET_X	(224)
#define OCR_REVIEW_TRIM_OFFSET_Y	(0)

#define OCR_REVIEW_TRIM_WIDTH		(OCR_TARGET_WIDTH - 2*OCR_REVIEW_TRIM_OFFSET_X)
#define OCR_REVIEW_TRIM_HEIGHT		(OCR_TARGET_HEIGHT - 2*OCR_REVIEW_TRIM_OFFSET_Y)


// ocr state
#define OCR_IDLE			(0x0)
#define OCR_OPENED			(0x1)
#define OCR_PREVIEWING		(0x2)
#define OCR_CAPTURING		(0x4)
#define OCR_REVIEWING		(0x8)

#define OCR_MAX_LAYOUT_REGION		(OCRLIB_MAX_LAYOUT_REGION)

#define OCR_PREVIEW_BRIGHTNESS		(21)
#define OCR_PREVIEW_CONTRAST		(27)

#ifdef _DEBUG
#define OCR_DRIVER_TRACE(_x_)	SCI_TRACE_LOW _x_
#else
#define OCR_DRIVER_TRACE(_x_)	((void)0)
#endif

// scale index num: it is dependent on chip
#define OCR_SCALE_INDEX_NUM	(16)

#define OCR_BUF4LIB_SIZE	(2*1024*1024)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct{
	uint32					state;
	uint8*					p_buf;
	OCR_PREVIEW_PARAM_T		st_preview_param;
	OCR_CAPTURE_PARAM_T		st_capture_param;
	OCR_REVIEW_PARAM_T		st_review_param;
	OCR_OSD_MODE_T			st_osd_mode;
}OCR_CONTEXT;

/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                  *
 **---------------------------------------------------------------------------*/

// scale index: it is dependent on chip
LOCAL const uint8 ocr_scale_index[OCR_SCALE_INDEX_NUM] =  
{
	4, 5, 6, 7,		\
	8, 10, 12, 14,	\
	16, 20, 24, 28,	\
	32, 40, 48, 56
};
 
// OCR global control struct
LOCAL OCR_CONTEXT s_st_ocr_context = {0};

// default preview parameter
LOCAL OCR_PREVIEW_PARAM_T s_st_default_preview_param = 
{
	OCR_PREVIEW_SENSOR_MODE,
	{OCR_PREVIEW_TRIM_OFFSET_X, OCR_PREVIEW_TRIM_OFFSET_Y, OCR_PREVIEW_TRIM_WIDTH, OCR_PREVIEW_TRIM_HEIGHT},	// trim rect
	MAIN_LCD_ID,		// LCD ID
	{0, 0, OCR_DISP_WIDTH, OCR_DISP_HEIGHT},	//display rect
	10,					// frame rate
	1,					// prefilter bypass
	0x40,				// smooth id
	0,					// AF flag
	0					// is_scale
};

// default capture parameter
LOCAL OCR_CAPTURE_PARAM_T s_st_default_capture_param =
{
	OCR_CAPTURE_SENSOR_MODE,
	{0, 0, OCR_TARGET_WIDTH, OCR_TARGET_HEIGHT},	// trim rect
	1,					// prefilter enable
	0x40,				// smooth value
	6,					// quality after encoding
	OCR_TARGET_WIDTH,	// target width
	OCR_TARGET_HEIGHT,	// target height
};

// default review parameter
LOCAL OCR_REVIEW_PARAM_T s_st_default_review_param =
{
	{
		{OCR_REVIEW_TRIM_OFFSET_X, OCR_REVIEW_TRIM_OFFSET_Y, OCR_REVIEW_TRIM_WIDTH, OCR_REVIEW_TRIM_HEIGHT},	// trim rect
		MAIN_LCD_ID,		// lcd id
		{0, 0, OCR_DISP_WIDTH, OCR_DISP_HEIGHT},	//display rect
		0,					// postfilter bypass
		0x1062,				// smooth
		{0, 0, 0, 0},		// norm_rect
		0,					// is_scale
		0,					// reserved_1
		0,					// target width
		0					// target height
	},
	NULL,				// review buffer ptr
	0					// review buffer size
};

LOCAL OCR_LAYOUT_PARAMETERS_T s_ocr_layout_param;
LOCAL OCR_LAYOUT_PARAMETERS_T s_ocr_display_param;
LOCAL OCR_RECOG_PARAMETERS_T s_ocr_recog_param;

LOCAL REGION s_ocr_region_layout[OCR_MAX_LAYOUT_REGION];
LOCAL REGION s_ocr_region_display[OCR_MAX_LAYOUT_REGION];

LOCAL REGION s_ocr_region_recog[OCR_MAX_RECOG_REGION_NUM];

LOCAL OCR_CARD_INFO s_card_info[OCR_MAX_RECOG_REGION_NUM];

#pragma arm section zidata = "OCRBuffer"
LOCAL uint32 s_ocr_lib_buffer[OCR_BUF4LIB_SIZE/sizeof(uint32)];
#pragma arm section zidata

LOCAL uint8 s_ocr_lib_is_open = SCI_FALSE;

LOCAL uint32 s_dc_ocr_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;

/*--------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                */
/*--------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/******************************************************************************/
//	Description: 	calculate scale factor according to chip
//	Author:			Jing.Li
//	Input:			none
//	Output:     	scale_x: scale factor in x dimension
//					scale_y: scale factor in y dimension
//	Return:     	none
//	Note:       	None
/******************************************************************************/
LOCAL void	OCR_CalcScaleFactor(uint8 *scale_x, uint8 *scale_y);

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
PUBLIC OCR_ERROR_E OCR_Init(uint8* buf_ptr, uint32 buf_size)
{
	uint8 *ocr_buf_ptr = SCI_NULL;
	uint32 ocr_buf_size = 0;

	// Note: input parameter check is omitted because they can be NULL & 0;

	//OCR_DRIVER_TRACE:"OCR_Init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_216_112_2_18_0_34_44_1975,(uint8*)"");

	//Check if ocr is opened
	if(OCR_IDLE != s_st_ocr_context.state)
	{
		//OCR_DRIVER_TRACE:"OCR_Init: ocr has been init before or hasn't been closed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_221_112_2_18_0_34_44_1976,(uint8*)"");
		return OCR_OPENED_ERROR;
	}
	
	//set open flag
	s_st_ocr_context.state |= OCR_OPENED;

	//check if input buffer is valid
	if((SCI_NULL == buf_ptr)||(0 == buf_size))
	{
		//input is invalid, use the buffer product has defined for interpolation
		ocr_buf_ptr = (uint8*)OCR_YUV_BUF;
		ocr_buf_size = OCR_YUV_BUF_SIZE;
	}
	else
	{
		//input is valid, use it
		ocr_buf_ptr = buf_ptr;
		ocr_buf_size = buf_size;
	}
	
	SCI_ASSERT(SCI_NULL != ocr_buf_ptr);/*assert verified*/
	SCI_ASSERT(0 != ocr_buf_size);/*assert verified*/
	//open DCAM on logical
	if(DC_Open(ocr_buf_ptr, ocr_buf_size))
	{
		return OCR_ERROR;
	}

	s_st_ocr_context.p_buf = ocr_buf_ptr;
	return OCR_NONE_ERROR;
	
}

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
PUBLIC OCR_ERROR_E OCR_Preview(OCR_PREVIEW_PARAM_T *pre_param)
{
	// Note: input parameter check is omitted because they can be NULL;

	//OCR_DRIVER_TRACE:"OCR_Preview"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_268_112_2_18_0_34_45_1977,(uint8*)"");

	// check if ocr is init
	if(OCR_OPENED != s_st_ocr_context.state)
	{
		//OCR_DRIVER_TRACE:"OCR_Preview: ocr has NOT been opened, 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_273_112_2_18_0_34_45_1978,(uint8*)"d", s_st_ocr_context.state);
		return OCR_OPENED_ERROR;
	}

	// set preview parameter
	if(OCR_NONE_ERROR != OCR_SetPreviewParam(pre_param))
	{
		// if error, return parameter error
		//OCR_DRIVER_TRACE:"OCR_Preview: OCR setting parameter error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_281_112_2_18_0_34_45_1979,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	// start preview
	if(OCR_NONE_ERROR != OCR_StartPreview())
	{
		// if error, return error
		//OCR_DRIVER_TRACE:"OCR_Preview: OCR start preview error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_289_112_2_18_0_34_45_1980,(uint8*)"");
		return OCR_ERROR;
	}

	return OCR_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_SetPreviewParam(OCR_PREVIEW_PARAM_T *pre_param)
{
	OCR_PREVIEW_PARAM_T *preview_param;
		
	// Note: input parameter check is omitted because they can be NULL;

	//OCR_DRIVER_TRACE:"OCR_SetPreviewParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_310_112_2_18_0_34_45_1981,(uint8*)"");

	preview_param = &(s_st_ocr_context.st_preview_param);
	
	// check if ocr is init
	if(OCR_OPENED != s_st_ocr_context.state)
	{
		//OCR_DRIVER_TRACE:"OCR_SetPreviewParam: ocr has NOT been opened, 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_317_112_2_18_0_34_45_1982,(uint8*)"d", s_st_ocr_context.state);
		return OCR_OPENED_ERROR;
	}

	// check if input is valid
	if(SCI_NULL == pre_param)
	{
		// if input is invalid, use the default parameter
		SCI_MEMCPY(preview_param, &s_st_default_preview_param, sizeof(OCR_PREVIEW_PARAM_T));
	}
	else
	{
		// if input is valid, use it
		SCI_MEMCPY(preview_param, pre_param, sizeof(OCR_PREVIEW_PARAM_T));
	}

	SCI_ASSERT(SCI_NULL != preview_param);/*assert verified*/
	// set preview parameter
	if(OCR_NONE_ERROR != DC_SetPreviewParam(preview_param))
	{
		// if error, return parameter error
		//OCR_DRIVER_TRACE:"OCR_SetPreviewParam: OCR setting parameter error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_338_112_2_18_0_34_45_1983,(uint8*)"");
		return OCR_PARAM_ERROR;
	}
	
	DC_SetImageEffect(OCR_PREVIEW_BRIGHTNESS);
	DC_SetImageEffect(OCR_PREVIEW_CONTRAST);

	return OCR_NONE_ERROR;

}

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
PUBLIC OCR_ERROR_E OCR_StartPreview(void)
{
	//OCR_DRIVER_TRACE:"OCR_StartPreview"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_359_112_2_18_0_34_45_1984,(uint8*)"");

	// check if ocr is init
	if(OCR_OPENED != s_st_ocr_context.state)
	{
		//OCR_DRIVER_TRACE:"OCR_StartPreview: ocr has NOT been opened, 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_364_112_2_18_0_34_45_1985,(uint8*)"d", s_st_ocr_context.state);
		return OCR_OPENED_ERROR;
	}

	// check if ocr is also in preview state
	if(s_st_ocr_context.state & OCR_PREVIEWING)
	{
		//OCR_DRIVER_TRACE:"OCR_StartPreview: ocr is previewing"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_371_112_2_18_0_34_45_1986,(uint8*)"");
		return OCR_ERROR;
	}

	// start preview
	if(DC_StartPreview())
	{
		// if error, return error
		//OCR_DRIVER_TRACE:"OCR_StartPreview: ocr start preview error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_379_112_2_18_0_34_45_1987,(uint8*)"");
		return OCR_ERROR;
	}

	// set preview flag
	s_st_ocr_context.state |= OCR_PREVIEWING;

	return OCR_NONE_ERROR;

}

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
PUBLIC OCR_ERROR_E OCR_StopPreview(void)
{
	//OCR_DRIVER_TRACE:"OCR_StopPreview"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_400_112_2_18_0_34_45_1988,(uint8*)"");

	// check if ocr is init
	if(!(s_st_ocr_context.state&OCR_OPENED))
	{
		//OCR_DRIVER_TRACE:"OCR_StopPreview: ocr has NOT been opened"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_405_112_2_18_0_34_45_1989,(uint8*)"");
		return OCR_OPENED_ERROR;
	}

	// check if ocr is also in preview state
	if(!(s_st_ocr_context.state & OCR_PREVIEWING))
	{
		//OCR_DRIVER_TRACE:"OCR_StopPreview: ocr is not previewing"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_412_112_2_18_0_34_45_1990,(uint8*)"");
		return OCR_ERROR;
	}

	// start preview
	if(DC_StopPreview())
	{
		// if error, return error
		//OCR_DRIVER_TRACE:"OCR_StopPreview: ocr stop preview error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_420_112_2_18_0_34_45_1991,(uint8*)"");
		return OCR_ERROR;
	}

	// set preview flag
	s_st_ocr_context.state &= ~OCR_PREVIEWING;

	return OCR_NONE_ERROR;

}

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
PUBLIC OCR_ERROR_E OCR_DoFocus(void)
{
	//OCR_DRIVER_TRACE:"OCR_DoFocus"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_441_112_2_18_0_34_45_1992,(uint8*)"");

	// check if ocr is init
	if(!(s_st_ocr_context.state&OCR_OPENED))
	{
		//OCR_DRIVER_TRACE:"OCR_DoFocus: ocr has NOT been opened"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_446_112_2_18_0_34_45_1993,(uint8*)"");
		return OCR_OPENED_ERROR;
	}

	// check if ocr is also in preview state
	if(!(s_st_ocr_context.state & OCR_PREVIEWING))
	{
		//OCR_DRIVER_TRACE:"OCR_DoFocus: ocr is not previewing"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_453_112_2_18_0_34_45_1994,(uint8*)"");
		return OCR_ERROR;
	}

	DC_DoSingleModeAF();
	
	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_Capture(OCR_CAPTURE_PARAM_T *cap_param)
{

	// Note: input parameter check is omitted because they can be NULL;

	//OCR_DRIVER_TRACE:"OCR_Capture"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_476_112_2_18_0_34_45_1995,(uint8*)"");

	// check if ocr is init
	if(OCR_OPENED != s_st_ocr_context.state)
	{
		//OCR_DRIVER_TRACE:"OCR_Capture: ocr has NOT been opened : 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_481_112_2_18_0_34_45_1996,(uint8*)"d", s_st_ocr_context.state);
		return OCR_OPENED_ERROR;
	}

	// set capture parameter
	if(OCR_NONE_ERROR != OCR_SetCaptureParam(cap_param))
	{
		// set capture parameter error, return error
		//OCR_DRIVER_TRACE:"OCR_Capture: OCR set capture parameter error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_489_112_2_18_0_34_45_1997,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	// do capture
	if(OCR_NONE_ERROR != OCR_DoCapture())
	{
		// caputre error, return error
		//OCR_DRIVER_TRACE:"OCR_Capture: OCR caputre error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_497_112_2_18_0_34_45_1998,(uint8*)"");
		return OCR_ERROR;
	}

	return OCR_NONE_ERROR;

}

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
PUBLIC OCR_ERROR_E OCR_SetCaptureParam(OCR_CAPTURE_PARAM_T *cap_param)
{
	OCR_CAPTURE_PARAM_T *capture_param;

	// Note: input parameter check is omitted because they can be NULL;

	//OCR_DRIVER_TRACE:"OCR_SetCaptureParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_519_112_2_18_0_34_45_1999,(uint8*)"");

	capture_param = &(s_st_ocr_context.st_capture_param);

	// check if ocr is init
	if(OCR_OPENED != s_st_ocr_context.state)
	{
		//OCR_DRIVER_TRACE:"OCR_SetCaptureParams: ocr has NOT been opened : 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_526_112_2_18_0_34_45_2000,(uint8*)"d", s_st_ocr_context.state);
		return OCR_OPENED_ERROR;
	}

	// check if input parameter is valid
	if(SCI_NULL == cap_param)
	{
		// input is invalid, use default capture parameter
		SCI_MEMCPY(capture_param, &s_st_default_capture_param, sizeof(OCR_CAPTURE_PARAM_T));
	}
	else
	{
		// input is valid, use it
		SCI_MEMCPY(capture_param, cap_param, sizeof(OCR_CAPTURE_PARAM_T));
	}

	SCI_ASSERT(SCI_NULL != capture_param);/*assert verified*/
	// set capture parameter
	if(DC_SetCaptureParam(capture_param))
	{
		// set capture parameter error
		//OCR_DRIVER_TRACE:"OCR_SetCaptureParams: set parameter error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_547_112_2_18_0_34_45_2001,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_DoCapture(void)
{
	//OCR_DRIVER_TRACE:"OCR_DoCapture"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_564_112_2_18_0_34_45_2002,(uint8*)"");

	// check if ocr is init
	if(OCR_OPENED != s_st_ocr_context.state)
	{
		//OCR_DRIVER_TRACE:"OCR_DoCapture: ocr has NOT been opened : 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_569_112_2_18_0_34_45_2003,(uint8*)"d", s_st_ocr_context.state);
		return OCR_OPENED_ERROR;
	}

	// capture YUV data
	if(DC_DoSnapshotYUV())
	{
		// capture error
		//OCR_DRIVER_TRACE:"OCR_DoCapture: capture error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_577_112_2_18_0_34_45_2004,(uint8*)"");
		return OCR_OPENED_ERROR;
	}

	return OCR_NONE_ERROR;

}

/******************************************************************************/
//	Description: 	OCR return picture data and size
//	Author:			Jing.Li
//	Input:			none
//	Output:     	buf_size: data buffer size
//	Return:     	data buffer
//	Note:       	None
/******************************************************************************/
PUBLIC uint8* OCR_GetYUVbuf(uint32 *buf_size)
{
	*buf_size = s_st_ocr_context.st_capture_param.w_target_width *\
				s_st_ocr_context.st_capture_param.w_target_height * 2;

	//OCR_DRIVER_TRACE:"OCR_GetYUVbuf: %x, %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_596_112_2_18_0_34_45_2005,(uint8*)"dd", s_st_ocr_context.p_buf, *buf_size);
	return s_st_ocr_context.p_buf;
}

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
PUBLIC OCR_ERROR_E OCR_Review(OCR_REVIEW_PARAM_T *review_param)
{
	SCI_PASSERT(0, ("OCR_Review has not been realized"));  /*assert verified*/
	// later
	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_SetReviewParam(OCR_REVIEW_PARAM_T *review_param)
{
	OCR_REVIEW_PARAM_T *re_param;

	// Note: input parameter check is omitted because they can be NULL;

	//OCR_DRIVER_TRACE:"OCR_SetReviewParam: %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_630_112_2_18_0_34_45_2006,(uint8*)"d", review_param);
	
	re_param = &(s_st_ocr_context.st_review_param);
	
	// check if ocr is init
	if(OCR_OPENED != s_st_ocr_context.state)
	{
		//OCR_DRIVER_TRACE:"OCR_SetReviewParam: ocr has NOT been opened"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_637_112_2_18_0_34_45_2007,(uint8*)"");
		return OCR_OPENED_ERROR;
	}

	// check if input parameter is valid
	if(SCI_NULL == review_param)
	{
		// input is invalid, use default review parameter
		SCI_MEMCPY(re_param, &s_st_default_review_param, sizeof(OCR_REVIEW_PARAM_T));
	}
	else
	{
		// input is valid, use it
		SCI_MEMCPY(re_param, review_param, sizeof(OCR_REVIEW_PARAM_T));
	}

	SCI_ASSERT(SCI_NULL != re_param);/*assert verified*/

	// if review buffer is null(application just set parameter, give no buffer),
	// set buffer to default
	if(SCI_NULL == re_param->buf)
	{
		re_param->buf = (uint8*)OCR_YUV_BUF;
		re_param->size = (s_st_ocr_context.st_capture_param.w_target_width * \
						s_st_ocr_context.st_capture_param.w_target_width * 2);
	}

	// set review parameter
	if(DC_SetReviewParam(&(re_param->st_review_param)))
	{
		// set review parameter error
		//OCR_DRIVER_TRACE:"OCR_SetReviewParam: set parameter error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_668_112_2_18_0_34_45_2008,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	return OCR_NONE_ERROR;
}

/******************************************************************************/
//	Description: 	OCR return review parameters
//	Author:			Xiaoguang.Feng
//	Input:			none
//	Output:     	none
//	Return:     	the review parameter
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_REVIEW_PARAM_T OCR_GetReviewParam(void)
{
	return s_st_ocr_context.st_review_param;
}

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
							PLAYBACK_SET_FUNC pf_callback)
{
	//OCR_DRIVER_TRACE:"OCR_DoReview"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_699_112_2_18_0_34_46_2009,(uint8*)"");
	
	if(SCI_SUCCESS != DC_PlaybackYUVFromBuf(p_buf, len, OCR_TARGET_WIDTH, OCR_TARGET_HEIGHT, pf_callback))
		return OCR_ERROR;
	
	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_Free(void)
{
	if(s_ocr_lib_is_open)
	{
		s_ocr_lib_is_open = SCI_FALSE;
		OCRlib_Free();
	}

	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_Close(void)
{
	//OCR_DRIVER_TRACE:"OCR_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_736_112_2_18_0_34_46_2010,(uint8*)"");
	
	// check if ocr is init
	if(!(OCR_OPENED & s_st_ocr_context.state))
	{
		//OCR_DRIVER_TRACE:"OCR_Close: ocr has NOT been opened"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_741_112_2_18_0_34_46_2011,(uint8*)"");
		return OCR_OPENED_ERROR;
	}
	
	DC_Close();
	//OCRlib_Free();

	s_st_ocr_context.state = OCR_IDLE;

	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_SetOsdZoneMode(int i_zone, const OCR_OSD_MODE_T *pst_mode)
{
	SCI_ASSERT((i_zone<6)&&(i_zone>0));/*assert verified*/
	SCI_ASSERT(pst_mode);/*assert verified*/

	if(DC_SetOsdZoneMode(i_zone,  pst_mode))
	{
		// set osd zone mode error
		//OCR_DRIVER_TRACE:"OCR_SetOsdZoneMode: set ZONE mode error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_771_112_2_18_0_34_46_2012,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_SetOsdImage(int i_zone, const uint8 *p_data)
{
	SCI_ASSERT((i_zone<6)&&(i_zone>0));/*assert verified*/
	SCI_ASSERT(p_data);/*assert verified*/

	if(DC_SetOsdImage(i_zone,  p_data))
	{
		// set osd image error
		//OCR_DRIVER_TRACE:"OCR_SetOsdImage: set osd image error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_801_112_2_18_0_34_46_2013,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_SetOsdColorPalette(const uint32 *p_table)
{
	SCI_ASSERT(p_table);/*assert verified*/

	if(DC_SetOsdColorPalette(p_table))
	{
		// set osd color palette error
		//OCR_DRIVER_TRACE:"OCR_SetOsdColorPalette: set osd color palette error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_827_112_2_18_0_34_46_2014,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_ModifyOsdImage(int i_zone, int i_pixel, uint8 b_value)
{
	SCI_ASSERT((i_zone<6)&&(i_zone>0));/*assert verified*/

	if(DC_ModifyOsdImage(i_zone, i_pixel, b_value))
	{
		// modify osd image error
		//OCR_DRIVER_TRACE:"OCR_ModifyOsdImage: modify osd image error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_858_112_2_18_0_34_46_2015,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	return OCR_NONE_ERROR;
}

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
PUBLIC OCR_ERROR_E OCR_SetOsdEnableMode(int flag)
{
	if(DC_SetOsdEnableMode(flag))
	{
		// modify osd image error
		//OCR_DRIVER_TRACE:"OCR_SetOsdEnableMode: modify osd image error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_879_112_2_18_0_34_46_2016,(uint8*)"");
		return OCR_PARAM_ERROR;
	}

	return OCR_NONE_ERROR;
}

/******************************************************************************/
//	Description: 	OCR layout picture
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_Layout(void)
{
	// set layout parameter to lib
	s_ocr_layout_param.img_size.width = OCR_TARGET_WIDTH; // tmp fixed
	s_ocr_layout_param.img_size.height = OCR_TARGET_HEIGHT;// tmp fixed
	s_ocr_layout_param.regions_ptr = s_ocr_region_layout;
	
	//SCI_TRACE_LOW:"s_st_ocr_context.p_buf = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_900_112_2_18_0_34_46_2017,(uint8*)"d", s_st_ocr_context.p_buf);
	SCI_ASSERT(SCI_NULL != s_st_ocr_context.p_buf);/*assert verified*/
	
	SCI_MEMSET(s_ocr_region_layout, 0, OCR_MAX_LAYOUT_REGION*sizeof(REGION));
	OCRlib_SetLayoutParams(&s_ocr_layout_param, s_st_ocr_context.p_buf);
	s_ocr_lib_is_open = SCI_TRUE;
	
	if(OCRlib_LayoutTaskOpen(PRI_OCR_TASK))
	{
		return OCR_ERROR;
	}
	
	return OCR_NONE_ERROR;
}

/******************************************************************************/
//	Description: 	OCR stop layout
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_LayoutStop(void)
{
	if(SCI_SUCCESS != OCRlib_LayoutTaskClose())
		return OCR_ERROR;

	return OCR_NONE_ERROR;
}

/******************************************************************************/
//	Description: 	OCR return layout regions to application
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	struct pointer which layout region info is stored
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_LAYOUT_PARAMETERS_T* OCR_GetLayoutRegion(void)
{
	uint8 i = 0;
	uint8 scale_x = 0;
	uint8 scale_y = 0;
	
	SCI_MEMSET(&s_ocr_display_param, 0, sizeof(OCR_LAYOUT_PARAMETERS_T));
	
	s_ocr_display_param.num_of_regions = s_ocr_layout_param.num_of_regions;
	s_ocr_display_param.regions_ptr = s_ocr_region_display;
	
	OCR_CalcScaleFactor(&scale_x, &scale_y);
	
	for(i = 0; i < s_ocr_layout_param.num_of_regions; i++)
	{
		//OCR_DRIVER_TRACE:"OCR_GetLayoutRegion_lay %d: %d, %d, %d, %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_956_112_2_18_0_34_46_2018,(uint8*)"ddddd",	i,											s_ocr_layout_param.regions_ptr[i].left,		s_ocr_layout_param.regions_ptr[i].top,		s_ocr_layout_param.regions_ptr[i].right,	s_ocr_layout_param.regions_ptr[i].bottom	);
	
/*		s_ocr_display_param.regions_ptr[i].left		= (s_ocr_layout_param.regions_ptr[i].left - OCR_TRIM_OFFSET_X)\
														* 4 / scale_x; // OCR_DISP_WIDTH / OCR_TRIM_WIDTH;
		s_ocr_display_param.regions_ptr[i].top		= (s_ocr_layout_param.regions_ptr[i].top - OCR_TRIM_OFFSET_Y)\
														* 4 / scale_y; // OCR_DISP_HEIGHT / OCR_TRIM_HEIGHT;
		s_ocr_display_param.regions_ptr[i].right 	= (s_ocr_layout_param.regions_ptr[i].right - OCR_TRIM_OFFSET_X)\
														* 4 / scale_x; // OCR_DISP_WIDTH / OCR_TRIM_WIDTH;
		s_ocr_display_param.regions_ptr[i].bottom	= (s_ocr_layout_param.regions_ptr[i].bottom - OCR_TRIM_OFFSET_Y)\
														* 4 / scale_y; // OCR_DISP_HEIGHT / OCR_TRIM_HEIGHT;
*/
		s_ocr_display_param.regions_ptr[i].left		= (s_ocr_layout_param.regions_ptr[i].left)\
														* 4 / scale_x; // OCR_DISP_WIDTH / OCR_TRIM_WIDTH;
		s_ocr_display_param.regions_ptr[i].top		= (s_ocr_layout_param.regions_ptr[i].top)\
														* 4 / scale_y; // OCR_DISP_HEIGHT / OCR_TRIM_HEIGHT;
		s_ocr_display_param.regions_ptr[i].right 	= (s_ocr_layout_param.regions_ptr[i].right)\
														* 4 / scale_x; // OCR_DISP_WIDTH / OCR_TRIM_WIDTH;
		s_ocr_display_param.regions_ptr[i].bottom	= (s_ocr_layout_param.regions_ptr[i].bottom)\
														* 4 / scale_y; // OCR_DISP_HEIGHT / OCR_TRIM_HEIGHT;
		
		//OCR_DRIVER_TRACE:"OCR_GetLayoutRegion_disp %d: %d, %d, %d, %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_980_112_2_18_0_34_46_2019,(uint8*)"ddddd",i,s_ocr_display_param.regions_ptr[i].left,	s_ocr_display_param.regions_ptr[i].top,		s_ocr_display_param.regions_ptr[i].right,	s_ocr_display_param.regions_ptr[i].bottom	);
	}
	
	return &s_ocr_display_param;
}

/******************************************************************************/
//	Description: 	OCR set regions need to be recognized
//	Author:			Jing.Li
//	Input:			region_selected: index array which the region is selected
//					region_num: selected number
//	Output:     	none
//	Return:     	none
//	Note:       	None
/******************************************************************************/
PUBLIC void OCR_SetRecogParams(uint8* region_selected, uint8 region_num)
{
	uint8 i = 0;
	
	SCI_ASSERT(region_num <= OCR_MAX_RECOG_REGION_NUM);/*assert verified*/
	
	SCI_MEMSET(s_ocr_region_recog, 0, OCR_MAX_RECOG_REGION_NUM * sizeof(REGION));
	SCI_MEMSET(s_card_info, 0, OCR_MAX_RECOG_REGION_NUM * sizeof(OCR_CARD_INFO));
	
	for(i = 0; i < region_num; i++)
	{
		SCI_MEMCPY(&s_ocr_region_recog[i], &(s_ocr_layout_param.regions_ptr[region_selected[i]]), sizeof(REGION));
	}
	
	s_ocr_recog_param.num_of_regions = region_num;
	s_ocr_recog_param.regions_ptr = s_ocr_region_recog;
	s_ocr_recog_param.ocr_results_ptr = s_card_info;

	OCRlib_SetRecogParams(&s_ocr_recog_param);
	
	return; 
}

/******************************************************************************/
//	Description: 	OCR do auto recognition
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_AutoRecog(void)
{

    if (CHNG_FREQ_REQ_HANDLER_NULL == s_dc_ocr_chng_freq_req_handler)
    {
        s_dc_ocr_chng_freq_req_handler =  CHNG_FREQ_GetReqHandler("dc_ocr");
        SCI_ASSERT(CHNG_FREQ_REQ_HANDLER_NULL != s_dc_ocr_chng_freq_req_handler);/*assert verified*/
    }	 

    CHNG_FREQ_SetArmClk(s_dc_ocr_chng_freq_req_handler,FREQ_INDEX_DC_HIGH);
    
	if(OCRlib_AutoRecogTaskOpen(PRI_OCR_TASK))
	{
		return OCR_ERROR;
	}

	return OCR_NONE_ERROR;
}

/******************************************************************************/
//	Description: 	OCR stop auto recognition
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	0           successful
//					ohter: error (refer to PUBLIC OCR_ERROR_E)
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_ERROR_E OCR_AutoRecogStop(void)
{
	if(SCI_SUCCESS != OCRlib_AutoRecogTaskClose())
		return OCR_ERROR;

    if(s_dc_ocr_chng_freq_req_handler != CHNG_FREQ_REQ_HANDLER_NULL)
    {
        CHNG_FREQ_RestoreARMClk(s_dc_ocr_chng_freq_req_handler);
    }
    
	return OCR_NONE_ERROR;
}

/******************************************************************************/
//	Description: 	OCR return recognition result
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	the result array pointer
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_CARD_INFO* OCR_GetRecogResult(void)
{
	return s_card_info;
}

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
	)
{
	//OCR_DRIVER_TRACE:"OCR_LCD_InvalidateImage: %d, %d, %d, %d, color_id:%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,OCR_DRIVER_1090_112_2_18_0_34_46_2020,(uint8*)"ddddd",left, top, right, bottom, (*buf_ptr));

	if(SCI_SUCCESS != LCD_InvalidateRect_Image(MAIN_LCD_ID, left, top, right, bottom, buf_ptr))
		return OCR_ERROR;

	return OCR_NONE_ERROR;
}

/******************************************************************************/
//	Description: 	calculate scale factor according to chip
//	Author:			Jing.Li
//	Input:			none
//	Output:     	scale_x: scale factor in x dimension
//					scale_y: scale factor in y dimension
//	Return:     	none
//	Note:       	None
/******************************************************************************/
LOCAL void	OCR_CalcScaleFactor(uint8 *scale_x, uint8 *scale_y)
{
	uint8 i = 0;
	uint32 tmp_scale_x = 0;
	uint32 tmp_scale_y = 0;

	// calc temp scale index x
	tmp_scale_x = OCR_REVIEW_TRIM_WIDTH * 10/ OCR_DISP_WIDTH;
	// OCR_DRIVER_TRACE(("OCR_CalcScaleFactor: tmp_scale_x = %d", tmp_scale_x));
	// get chip scale factor x
	for(i = 0; i<OCR_SCALE_INDEX_NUM; i++){
		if((tmp_scale_x*4) <= (10*ocr_scale_index[i]))
			break;
	}
	i = i < 1 ? i : i-1; 
	*scale_x = ocr_scale_index[i];
	//*scale_x = 14;
	
	// calc temp scale index y
	tmp_scale_y = OCR_REVIEW_TRIM_HEIGHT * 10 / OCR_DISP_HEIGHT;
	// OCR_DRIVER_TRACE(("OCR_CalcScaleFactor: tmp_scale_y = %d", tmp_scale_y));
	// get chip scale factor y
	for(i = 0; i<OCR_SCALE_INDEX_NUM; i++){
		if((tmp_scale_y*4) <= (10*ocr_scale_index[i]))
			break;
	}
	i = i < 1 ? i : i-1; 
	*scale_y = ocr_scale_index[i];
	//*scale_y = 12;
	
	return;
}

/******************************************************************************/
//	Description: 	Get buffer used for OCR lib
//	Author:			Jing.Li
//	Input:			none
//	Output:     	buffer_size: buffer size
//	Return:     	buffer pointer
//	Note:       	None
/******************************************************************************/
PUBLIC uint8* OCR_GetOCRBuffer(uint32* buffer_size) 
{
	SCI_ASSERT(buffer_size);/*assert verified*/
	
	*buffer_size = OCR_BUF4LIB_SIZE;
	
	return (uint8*)s_ocr_lib_buffer;
}

/******************************************************************************/
//	Description: 	Get display size of OCR
//	Author:			Jing.Li
//	Input:			none
//	Output:     	disp_width:	display width
//					disp_height:	display height
//	Return:     	buffer pointer
//	Note:       	None
/******************************************************************************/
PUBLIC void OCR_GetDispSize(uint32* disp_width, uint32* disp_height) 
{
	SCI_ASSERT(disp_width && disp_height);/*assert verified*/

	*disp_width = OCR_DISP_WIDTH;
	*disp_height = OCR_DISP_HEIGHT;
}

/******************************************************************************/
//	Description: 	get scale factor
//	Author:			Jing.Li
//	Input:			none
//	Output:     	scale_x: scale factor in x dimension
//					scale_y: scale factor in y dimension
//	Return:     	none
//	Note:       	None
/******************************************************************************/
PUBLIC void	OCR_GetScaleFactor(uint8 *scale_x, uint8 *scale_y)
{
	SCI_ASSERT(scale_x && scale_y);/*assert verified*/
	
	OCR_CalcScaleFactor(scale_x, scale_y);
}

/******************************************************************************/
//	Description: 	Get target OCR YUV data size
//	Author:			Jing.Li
//	Input:			none
//	Output:     	none
//	Return:     	none
//	Note:       	None
/******************************************************************************/
PUBLIC uint32 OCR_GetOCRYUVDataSize(void)
{
    return OCR_TARGET_SIZE;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif

