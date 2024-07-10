/******************************************************************************
 ** File Name:      img_transform.h												  									*
 ** Author:         Shan.He                                                   *
 ** DATE:           2010-11-27                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    functions to process some images                          *
 ** Note:                                                      				  			*
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2010-11-27         Shan.He         Create                                 *
 *****************************************************************************/
#ifndef __IMG_TRANSFORM_H__
#define __IMG_TRANSFORM_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "img_ref_types.h"
#include "jpeg_interface.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
        
/**---------------------------------------------------------------------------*
 **                         Data Types                                        *
 **---------------------------------------------------------------------------*/
#define IMGPROC_ERR_SIZE_UNSUPPORT							1
#define IMGPROC_ERR_FORMAT_UNSUPPORT						2
#define IMGPROC_ERR_POINTER_INVALID							3
#define IMGPROC_ERR_BUFFER_INVALID							4
#define IMGPROC_ERR_SCALE_OUT_OF_RANGE						5
#define IMGPROC_ERR_MEMORY_NOT_ENOUGH						6
#define IMGPROC_ERR_HARDWARE								7
#define IMGPROC_ERR_UNKNOWN									8
#define IMGPROC_ERR_SCALE_UNSUPPORT							9
#define IMGPROC_ERR_ALLOC_MEMORY_FAILED						10

#define IMGPROC_HSCALE_SIZE_BOTTOM                                           112             //if output size(w/h) is smaller then that,  the use software scaling

typedef enum
{
	IMGPROC_SCALE_MODE_NO_DISTORT = 0,					//automatically adjust the horizontal and vertical scale ratio to the same one
														//and the output image will not be distort. The output image size maybe
														//smaller than the expected size
	IMGPROC_SCALE_MODE_DISTORT							//do not adjust the horizontal and vertical scale ratio and the output image maybe distort
														//the scale ratio of both horizontal and vertical direction must range from 
														// 1/4 to 4 if a SCALE_MODE_DISTORT mode scale is expected
}IMGPROC_SCALE_MODE_E;

typedef struct
{
	IMGREF_SIZE_T			output_size;
	IMGREF_DATA_FORMAT_E	output_format;
	IMGREF_CHN_T			output_chn;	
}IMGPROC_SCALE_OUT_T;

typedef struct
{
	IMGREF_SIZE_T			src_size;
	IMGREF_RECT_T			src_trim_rect;
	IMGREF_DATA_FORMAT_E	src_format;
	IMGREF_CHN_T			src_chn;
	IMGPROC_SCALE_MODE_E	scale_mode;
	
	IMGREF_SIZE_T			target_size;
	IMGREF_DATA_FORMAT_E	target_format;
	IMGREF_BUF_T			target_buf;
    BOOLEAN 				is_isp_open;
	void (*async_callback)(uint32 ret_value, IMGPROC_SCALE_OUT_T *out_param_ptr, uint32 async_param);	
	uint32					async_param;

	BOOLEAN					scale_by_software;		//TRUE to use software scaler
}IMGPROC_SCALE_IN_T;


typedef struct
{

	IMGREF_RANGE_T			scale_down_range;		//range of scale down, [1/range.max, 1/range.min]
	IMGREF_RANGE_T			scale_up_range;			//range of scale up, [range.min, range.max]
	BOOLEAN					is_idle;				//is the hardware is idle
}IMGPROC_SCALE_CAP_T;

 typedef struct
{
	IMGREF_CHN_T 			src_chn;				//if the source format is YUV: y data in channel 0, uv data in channel 1
													//if the source format is RGB: rgb data in channel 0
	uint32					src_width;				//must be exactly divided by 2 if the source is YUV422 or YUV420
	uint32 					src_height;				//must be exactly divided by 2 if the source is YUV420
	IMGREF_DATA_FORMAT_E	src_format;				//format RGB565, YUV422, YUV420, YUV400 are supported now
	
	IMGREF_DATA_FORMAT_E	target_format;			//format RGB565, YUV422, YUV420, YUV400 are supported now	
	IMGREF_BUF_T			target_buf;				
}IMGPROC_YUV_RGB_CVT_IN_T;

 typedef struct
{
	IMGREF_CHN_T 			output_chn;				//if the source format is YUV: y data in channel 0, uv data in channel 1
													//if the source format is RGB: rgb data in channel 0
	uint32					output_width;
	uint32 					output_height;
	IMGREF_DATA_FORMAT_E	output_format;
}IMGPROC_YUV_RGB_CVT_OUT_T;
 
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               * 
 **---------------------------------------------------------------------------*/
/******************************************************************************/
//  Description: scale up/down image
//  Author: Shan.He 
//  Input:   
//          scale_in_ptr:	pointer of input parameters
//  Output: 
//			scale_out_ptr:	pointer of output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 IMGPROC_ScaleImage(const IMGPROC_SCALE_IN_T *scale_in_ptr, IMGPROC_SCALE_OUT_T *scale_out_ptr);

/******************************************************************************/
//  Description: get the capabilities of the scaling
//  Author: Shan.He 
//  Input:   
//  Output: 
//			scale_cap_ptr:	pointer of capabilities
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 IMGPROC_GetScaleCapability(IMGPROC_SCALE_CAP_T *scale_cap_ptr);

/******************************************************************************/
//  Description: convert YUV to RGB or RGB to YUV
//  Author: 	Shan.He 
//  Input:  
//			cvt_in_ptr:	convert input parameters
//  Output:	
//			cvt_out_ptr:  convert output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 IMGPROC_YuvRgbConvert(const IMGPROC_YUV_RGB_CVT_IN_T *cvt_in_ptr, 
										  IMGPROC_YUV_RGB_CVT_OUT_T *cvt_out_ptr);

/******************************************************************************/
//  Description: convert YUV to RGB or RGB to YUV
//  Author: 	Shan.He 
//  Input:  
//			cvt_in_ptr:	convert input parameters
//  Output:	
//			cvt_out_ptr:  convert output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/

PUBLIC void IMGPROC_RgbYuvConvert(uint8  *src_ptr,uint8 *dst_ptr,uint16 width,uint16 height);


/******************************************************************************/
//  Description: convert YUV to JPG
//  Author: 	Shan.He 
//  Input:  
//			cvt_in_ptr:	convert input parameters
//  Output:	
//			cvt_out_ptr:  convert output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/

PUBLIC uint32 IMGPROC_YuvJpgConvert(uint8  *src_ptr,uint8 *dst_ptr,uint16 width,uint16 height,uint32 dst_buf_size,JINF_QUALITY_E quality);

/******************************************************************************/
//  Description: get alpha from ARGB888
//  Author: 	Jiawei.He 
//  Input:  
//			src_a888_addr:	input argb888 image data
//          size        :   image pixel numbers
//  Output:	
//			dst_apha_addr:  output alpha data
//  Return:  None
//         
//  Note:   
/******************************************************************************/
PUBLIC void IMGPROC_GetAlpha(uint32 *src_a888_addr, uint32 *dst_apha_addr, uint32 size);

/******************************************************************************/
//  Description: get alpha from ARGB888
//  Author: 	Jiawei.He 
//  Input:  
//			src_a888_addr:	input argb888 image data
//          size        :   image pixel numbers
//  Output:	
//			dst_apha_addr:  output alpha data
//  Return:  None
//         
//  Note:   
/******************************************************************************/
PUBLIC void IMGPROC_SetAlpha(uint32 *src_888_addr, uint32 *src_apha_addr, uint32 size, uint32 *dst_a888_addr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
        
#endif

