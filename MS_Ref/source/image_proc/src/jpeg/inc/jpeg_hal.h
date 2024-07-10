#ifndef _JPEG_HAL_H_
#define _JPEG_HAL_H_

#include "jpeg.h"

// the size of Res JPEG head is 8 bytes
#define JPEG_RES_HEAD_SIZE	(12)

/******************************************************************************
// Purpose:	judge if software scaling down is needed			
// Author:	shan.he
// Input:		src_size_ptr--points to the source image size
//				target_size_ptr--points to the target image size
// Output: 	None	
// Return:  	TRUE-need software scaling down
//				FALSE--does not need software scaling down
// Note:    
******************************************************************************/
PUBLIC BOOLEAN JPEG_IsSoftwareScalingDown(JPEG_SIZE_T *src_size_ptr, JPEG_SIZE_T *target_size_ptr);

/******************************************************************************
// Purpose:	judge if hardware scaling down is needed			
// Author:	shan.he
// Input:		src_size_ptr--points to the source image size
//				target_size_ptr--points to the target image size
//				target_data_type--target data type, RGB or YUV
//				src_yuv_type--source yuv format, which is valid while the  target_data_type is YUV
//				target_yuv_type--target yuv format, which is valid while the  target_data_type is YUV
// Output: 	None	
// Return:  	TRUE-need hardware scaling down
//				FALSE--does not need hardware scaling down
// Note:    
******************************************************************************/
PUBLIC BOOLEAN JPEG_IsHardwareScalingDown(JPEG_SIZE_T *src_size_ptr, 
													JINF_RECT_T *src_trim_rect_ptr,
													JPEG_SIZE_T *target_size_ptr,
													JINF_DATA_TYPE_E  target_data_type,
													JINF_YUV_FORMAT_E src_yuv_type,
													JINF_YUV_FORMAT_E target_yuv_type);

/******************************************************************************
// Purpose:	get output data yuv type according to jpeg file original type			
// Author:	frank.yang
// Input:		original_type-	jpeg file original type
// Output: 	None	
// Return:  	
// Note:    the logic
******************************************************************************/
PUBLIC void JPEG_GetMCUSize(JINF_YUV_FORMAT_E yuv_sample_format, JPEG_SIZE_T *mcu_size_ptr);

/******************************************************************************
// Purpose:	judge whether the jpeg file is supported by HW platform			
// Author:	frank.yang
// Input:		platform-platform type
//			file_type-jpeg type,baseline ,progressive or else
//			data_type-yuv type,YUV444/422/420/411/400 or else
// Output: 	None	
// Return:  	TRUE-supported by the HW platform,otherwise not supported
// Note:    
******************************************************************************/
BOOLEAN JPEG_IsHalDecSupported(JINF_JPEG_TYPE_E file_type,  
										JINF_YUV_FORMAT_E data_type);

/******************************************************************************
// Purpose:	judge whether supported by HW platform			
// Author:	shan.he
// Input:		platform-platform type
// Output: 	None	
// Return:  	TRUE-supported by the HW platform,otherwise not supported
// Note:    
******************************************************************************/
BOOLEAN JPEG_IsHalEncSupported(JINF_YUV_FORMAT_E sample_format,
										JINF_YUV_PATTERN_E yuv_pattern);

/******************************************************************************
// Purpose:	get information of res jpeg	
// Author:	shan.he
 // Input:    jpeg_buf_ptr:	jpeg stream buffer
 //			jpeg_buf_size: 	jpeg buffer size	
// Output:  
//			jpeg_res_info_ptr: jpeg info pointer
// Return:  	
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_ParseRes(uint8 *jpeg_buf_ptr, uint32 jpeg_buf_size, JPEG_RES_INFO_T *jpeg_res_info_ptr);

/******************************************************************************
// Purpose:	Is spreadtrum JPEG
// Author:	shan.he
 // Input:    jpeg_buf_ptr:	jpeg stream buffer
 //			jpeg_buf_size: 	jpeg buffer size
// Output:  None	
// Return:  TRUE if it is spreadtrum private JPEG
// Note:    
******************************************************************************/
PUBLIC BOOLEAN JPEG_IsSpreadturmJPG(uint8 *jpeg_buf_ptr, uint32 jpeg_buf_size);

/******************************************************************************
// Purpose:	convert the SJPG to STD JPEG	
// Author:	shan.he
 // Input:    None		
 //			sjpg_buf_ptr: 	buffer to receive SJPG stream
 //			sjpg_buf_size: 	SJPG buffer size
 //			std_jpg_buf_ptr:	standard JPEG buffer
 //			std_jpg_buf_size: 	standard JPEG buffer size
 //			std_jpg_info_ptr:	standard JPEG information
// Output:  None	
// Return:     size of output SJPG stream	
// Note:    
******************************************************************************/
PUBLIC uint32 JPEG_ConvertStdJPGToSJPG(uint8 *sjpg_buf_ptr, uint32 sjpg_buf_size, 												
												uint8 *std_jpg_buf_ptr, uint32 std_jpg_buf_size,
												JPEG_RES_INFO_T *std_jpg_info_ptr);

#if !defined(PLATFORM_SC6600L)
/******************************************************************************
// Purpose:	get MCU align shift value
// Author:	shan.he
 // Input:    
 //			sample_format: JPEG sample format
// Output:  
//			aligned_shift_ptr: align shift value in horzental and vertical direction
// Return:  JPEG_SUCCESS if success
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_GetMCUAlignShift(JINF_YUV_FORMAT_E sample_format, 
													JPEG_SIZE_T *aligned_shift_ptr);

/******************************************************************************
// Purpose:	get MCU aligned JPEG size	
// Author:	shan.he
 // Input:    
 //			sample_format: JPEG sample format
 //			src_size_ptr:  source image size
// Output:  
//			aligned_size_ptr: aligned image size
// Return:  JPEG_SUCCESS if success
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_GetMCUAlignedSize(JINF_YUV_FORMAT_E sample_format, JPEG_SIZE_T *src_size_ptr, JPEG_SIZE_T *aligned_size_ptr);

#endif
				
#endif		   //_JPEG_HAL_H_
