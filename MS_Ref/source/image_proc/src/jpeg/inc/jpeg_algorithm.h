/******************************************************************************
 ** File Name:	jpeg_algorithm.h                                               	  
 ** Author:		shan.he		                                          
 ** DATE:		13/01/2009                                               
 ** Copyright:	2009 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:                                                              
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 ** ------------------------------------------------------------------------- 
 ** DATE              NAME             DESCRIPTION       
 ** 13/01/2009        shan.he		   Create
 ******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "jpeg.h"

#if (defined(JPG_DEC_SUPPORT_JPG_HW) || defined(JPG_ENC_SUPPORT_JPG_HW))

typedef struct
{
	JINF_YUV_CHN_T		src_yuv;
	JINF_YUV_FORMAT_E	src_format;
	JINF_YUV_PATTERN_E	src_pattern;
	uint32				src_width;
	uint32				src_height;
	uint32				subsample_factor;
	
	JINF_YUV_CHN_T		dst_yuv;
	JINF_YUV_PATTERN_E	dst_pattern;
}SUBSAMPLE_INPUT_INFO_T;

typedef struct
{
	JINF_YUV_CHN_T		yuv_chn;
	JINF_YUV_PATTERN_E	yuv_pattern;
	JINF_YUV_FORMAT_E	yuv_format;
	uint32				output_width;
	uint32				output_height;
}SUBSAMPLE_OUTPUT_INFO_T;

#if !defined(PLATFORM_SC6600L)
typedef struct
{
	JINF_YUV_CHN_T		src_yuv;
	JINF_YUV_FORMAT_E	src_format;
	JINF_YUV_PATTERN_E	src_pattern;
	uint32				src_width;
	uint32				src_height;
	JINF_RECT_T			trim_rect;

	JINF_RGB_FORMAT_E	dst_format;
	uint16				*dst_buf;
	uint32				dst_buf_size;
}YUV2RGB_INPUT_INFO_T;

typedef struct
{
	uint16				*rgb_buf;
	uint32				output_width;
	uint32				output_height;
}YUV2RGB_OUTPUT_INFO_T;

typedef struct
{
	JINF_YUV_CHN_T		src_yuv;
	JINF_YUV_FORMAT_E	src_format;
	JINF_YUV_PATTERN_E	src_pattern;
	uint32				src_width;
	uint32				src_height;
	
	JINF_YUV_CHN_T		dst_yuv;
	JINF_YUV_PATTERN_E	dst_pattern;
}YUV_ALIGN_INPUT_INFO_T;

typedef struct
{
	JINF_YUV_CHN_T		yuv_chn;
	JINF_YUV_PATTERN_E	yuv_pattern;
	JINF_YUV_FORMAT_E	yuv_format;
	uint32				output_width;
	uint32				output_height;
}YUV_ALIGN_OUTPUT_INFO_T;

typedef struct
{
	JINF_YUV_CHN_T		src_yuv;
	JINF_YUV_FORMAT_E	src_format;
	JINF_YUV_PATTERN_E	src_pattern;
	uint32				src_width;
	uint32				src_height;
	JINF_RECT_T			trim_rect;
	
	JINF_YUV_CHN_T		dst_yuv;
	JINF_YUV_FORMAT_E	dst_format;
	JINF_YUV_PATTERN_E	dst_pattern;
}YUV_CONVERT_INFO_T;

typedef struct
{
	JINF_YUV_CHN_T		yuv_chn;
	JINF_YUV_PATTERN_E	yuv_pattern;
	JINF_YUV_FORMAT_E	yuv_format;
	uint32				output_width;
	uint32				output_height;
}YUV_CONVERT_OUTPUT_INFO_T;

LOCAL const uint8 g_horz_uv_size_shift[8] = 
{
	0,		/*for JINF_FORMAT_YUV444*/	
	1,		/*for JINF_FORMAT_YUV422*/	
	1,		/*for JINF_FORMAT_YUV420*/	
	2,		/*for JINF_FORMAT_YUV411*/	
	0,		/*for JINF_FORMAT_YUV400*/	
	0,		/*for JINF_FORMAT_YUV422R*/	
	0,		/*for JINF_FORMAT_YUV411R*/	
	0,		/*for JINF_FORMAT_YUV_MAX*/
};

LOCAL const uint8 g_vert_uv_size_shift[8] = 
{
	0,		/*for JINF_FORMAT_YUV444*/	
	0,		/*for JINF_FORMAT_YUV422*/	
	1,		/*for JINF_FORMAT_YUV420*/	
	0,		/*for JINF_FORMAT_YUV411*/	
	0,		/*for JINF_FORMAT_YUV400*/	
	1,		/*for JINF_FORMAT_YUV422R*/	
	2,		/*for JINF_FORMAT_YUV411R*/	
	0,		/*for JINF_FORMAT_YUV_MAX*/
};
#endif

/******************************************************************************
// Purpose:	convert YUV422 to RGB565 format, Y data in y channel and UV data in u channel.			
// Author:	shan.he
// Input:	yuv_chn_ptr--yuv buffer
//			input_width--input width
//			input_height--input height
//			output_buf_ptr--output buffer pointer
//			output_width--output width
//			output_height--output height
// Output: 	None	
// Return:  None
// Note:    
******************************************************************************/
PUBLIC void YUV422_Channel_Y_UV_To_RGB565(JINF_YUV_CHN_T *yuv_chn_ptr, uint32 input_width, uint32 input_height, 
													 void *output_buf_ptr, uint32 output_width, uint32 output_height);

/******************************************************************************
// Purpose:	convert YUV422 to ARGB8888 format, Y data in y channel and UV data in u channel.			
// Author:	shan.he
// Input:	yuv_chn_ptr--yuv buffer
//			input_width--input width
//			input_height--input height
//			output_buf_ptr--output buffer pointer
//			output_width--output width
//			output_height--output height
// Output: 	None	
// Return:  None
// Note:    
******************************************************************************/
PUBLIC void YUV422_Channel_Y_UV_To_ARGB8888(JINF_YUV_CHN_T *yuv_chn_ptr, uint32 input_width, uint32 input_height, 
													 void *output_buf_ptr, uint32 output_width, uint32 output_height);

/******************************************************************************
// Purpose:	convert YUV420 to RGB565 format, Y data in y channel and UV data in u channel.			
// Author:	shan.he
// Input:	yuv_chn_ptr--yuv buffer
//			input_width--input width
//			input_height--input height
//			output_buf_ptr--output buffer pointer
//			output_width--output width
//			output_height--output height
// Output: 	None	
// Return:  None
// Note:    
******************************************************************************/
PUBLIC void YUV420_Channel_Y_UV_To_RGB565(JINF_YUV_CHN_T *yuv_chn_ptr, uint32 input_width, uint32 input_height, 
													void *output_buf_ptr, uint32 output_width, uint32 output_height);													 


/******************************************************************************
// Purpose:	convert YUV420 to ARGB8888 format, Y data in y channel and UV data in u channel.			
// Author:	shan.he
// Input:	yuv_chn_ptr--yuv buffer
//			input_width--input width
//			input_height--input height
//			output_buf_ptr--output buffer pointer
//			output_width--output width
//			output_height--output height
// Output: 	None	
// Return:  None
// Note:    
******************************************************************************/
PUBLIC void YUV420_Channel_Y_UV_To_ARGB8888(JINF_YUV_CHN_T *yuv_chn_ptr, uint32 input_width, uint32 input_height, 
													void *output_buf_ptr, uint32 output_width, uint32 output_height);

/******************************************************************************
// Purpose:	convert YUV400 to RGB565 format, Y data in y channel and UV data in u channel.			
// Author:	shan.he
// Input:	yuv_chn_ptr--yuv buffer
//			input_width--input width
//			input_height--input height
//			output_buf_ptr--output buffer pointer
//			output_width--output width
//			output_height--output height
// Output: 	None	
// Return:  None
// Note:    
******************************************************************************/
PUBLIC void YUV400_Channel_Y_UV_To_RGB565(JINF_YUV_CHN_T *yuv_chn_ptr, uint32 input_width, uint32 input_height, 
													void *output_buf_ptr, uint32 output_width, uint32 output_height);

/******************************************************************************
// Purpose:	convert YUV400 to ARGB8888 format, Y data in y channel and UV data in u channel.			
// Author:	shan.he
// Input:	yuv_chn_ptr--yuv buffer
//			input_width--input width
//			input_height--input height
//			output_buf_ptr--output buffer pointer
//			output_width--output width
//			output_height--output height
// Output: 	None	
// Return:  None
// Note:    
******************************************************************************/
PUBLIC void YUV400_Channel_Y_UV_To_ARGB8888(JINF_YUV_CHN_T *yuv_chn_ptr, uint32 input_width, uint32 input_height, 
													void *output_buf_ptr, uint32 output_width, uint32 output_height);

/******************************************************************************
// Purpose:	subsample YUV422, 1/2, olny suppor YUV420 and YUV422, the output data is YUV422
// Author:	shan.he
// Input:	dst_yuv_ptr--dst yuv buffer
//			src_yuv_ptr--src yuv buffer
//			src_width--width
//			src_height--height
// Output: 	None	
// Return:  None
// Note:    
******************************************************************************/
PUBLIC void Y_UV_HalfSubsample(JINF_YUV_CHN_T *dst_yuv_ptr, JINF_YUV_CHN_T *src_yuv_ptr, 
									uint32 src_width, uint32 src_height, JINF_YUV_FORMAT_E yuv_format);

/******************************************************************************
// Purpose:	subsample YUV422, 1/4, olny suppor YUV420 and YUV422, the output data is YUV422
// Author:	shan.he
// Input:	dst_yuv_ptr--dst yuv buffer
//			src_yuv_ptr--src yuv buffer
//			src_width--width
//			src_height--height
// Output: 	None	
// Return:  None
// Note:    
******************************************************************************/
PUBLIC void Y_UV_QuartSubsample(JINF_YUV_CHN_T *dst_yuv_ptr, JINF_YUV_CHN_T *src_yuv_ptr, 
									uint32 src_width, uint32 src_height, JINF_YUV_FORMAT_E yuv_format);

#if !defined(PLATFORM_SC6600L)
PUBLIC void ConvertToYUV422(JINF_YUV_CHN_T *dst_yuv_ptr, JINF_YUV_CHN_T *src_yuv_ptr, 
								uint32 src_width, uint32 src_height, JINF_YUV_FORMAT_E yuv_format);

PUBLIC void Y_UV_To_Y_U_V(JINF_YUV_CHN_T *dst_yuv_ptr, JINF_YUV_CHN_T *src_yuv_ptr, 
								uint32 src_width, uint32 src_height, JINF_YUV_FORMAT_E yuv_format);

PUBLIC void MCU_Align_Y_U_V( JINF_YUV_CHN_T *dst_yuv_chn_ptr, JINF_YUV_CHN_T *src_yuv_chn_ptr,
							uint32 src_width, uint32 src_height, JINF_YUV_FORMAT_E sample_format);

PUBLIC void JPEG_SplitAndAlign_Y_UV422(JINF_YUV_CHN_T *dst_yuv_chn_ptr, JINF_YUV_CHN_T *src_yuv_chn_ptr, 
											 JPEG_SIZE_T *src_size_ptr);

PUBLIC void SubsampleYUV(SUBSAMPLE_INPUT_INFO_T *in_param_ptr,
						SUBSAMPLE_OUTPUT_INFO_T *out_param_ptr);

PUBLIC void YUVAlign(YUV_ALIGN_INPUT_INFO_T *in_param_ptr,
						YUV_ALIGN_OUTPUT_INFO_T *out_param_ptr);

PUBLIC void YUVConvert(YUV_CONVERT_INFO_T *in_param_ptr,
					 YUV_CONVERT_OUTPUT_INFO_T *out_param_ptr);


/******************************************************************************
// Purpose:	Convert YUV data to ARGB888 or RGB565 data			
// Author:	shan.he
// Input:		in_param_ptr--pointer of input parameters
//				out_param_ptr--pointer of output parameters
// Output: 	None	
// Return:  
// Note:    
******************************************************************************/
PUBLIC void YUV2RGB(YUV2RGB_INPUT_INFO_T *in_param_ptr, 
							YUV2RGB_OUTPUT_INFO_T *out_param_ptr);

/******************************************************************************
// Purpose:	Convert RGB888 data to ARGB888 data			
// Author:	shan.he
// Input:		data_buf_ptr--RGB888 data
// Output: 	None	
// Return:  	operation results
// Note:    
******************************************************************************/
PUBLIC void JPEG_Rgb888ToArgb888(uint32 *data_buf_ptr, uint32 pixels);

/******************************************************************************
// Purpose:	Convert RGB565 data to ARGB888 data			
// Author:	shan.he
// Input:		
//			rgb565_buf--RGB565 data buffer
//			pixels--pixel of image
// Output: 		
//			argb888_buf--ARGB888 data buffer
// Return:  	operation results
// Note:    
******************************************************************************/
PUBLIC void JPEG_Rgb565ToArgb888(uint32 *argb888_buf, uint16 *rgb565_buf, uint32 pixels);
#endif

#endif
