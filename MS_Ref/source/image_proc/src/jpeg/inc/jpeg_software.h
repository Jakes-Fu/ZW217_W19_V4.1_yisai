/******************************************************************************
 ** File Name:      jpeg_software.h                                                                              *
 ** Author:           Shan.He                                                                                            *
 ** DATE:             2008-12-15                                                                                          *
 ** Copyright:       2008 Spreatrum, Incoporated. All Rights Reserved.                                  *
 ** Description:     decode jpeg use jpeglib.                                                                 *
 ** Note:                                                     *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                                                                       *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                                                                 * 
 ** 2008-12-15       Shan.He             Create.                                                                     *
 *****************************************************************************/
#ifndef __JPEG_SOFTWARE_H__
#define __JPEG_SOFTWARE_H__
///////////////////////////////////////////////////////////////////////////////

#include "jpeg_interface.h"

///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
//  Description: Decode jpeg to RGB data or YUV data.
//  Author: Shan.He 
//  Input:    in_param_ptr: pointer of input parameter structure            
//  Output:  out_param_ptr: pointer of output parameter structure
//  Return: none
//  Note:   
/******************************************************************************/
PUBLIC JINF_RET_E SWJPEG_Decode(JINF_DEC_IN_PARAM_T  *in_param_ptr, 
					                  JINF_DEC_OUT_PARAM_T *out_param_ptr);


/******************************************************************************/
//  Description: Create a miniature jpeg with desired size for the input jpeg.
//  Author: Shan.He 
//  Input:    in_param_ptr: pointer of input parameter structure            
//  Output:  out_param_ptr: pointer of output parameter structure
//  Return: none
//  Note:   
/******************************************************************************/
PUBLIC JINF_RET_E SWJPEG_CreateMiniature(JINF_MINI_IN_PARAM_T *in_param_ptr,
					                            JINF_MINI_OUT_PARAM_T *out_param_ptr);

/******************************************************************************/
//  Description: Encode YUV444 to jpeg.
//  Author: Shan.He 
//  Input:    in_param_ptr: pointer of input parameter structure            
//  Output:  out_param_ptr: pointer of output parameter structure
//  Return: none
//  Note:   
/******************************************************************************/
PUBLIC JINF_RET_E SWJPEG_Encode(JINF_ENC_IN_PARAM_T *in_param_ptr,
					                      JINF_ENC_OUT_PARAM_T *out_param_ptr);

/******************************************************************************/
//  Description: is software encode supported
//  Author: Shan.He 
//  Input:    sample_format: yuv sample format            
//  Output:  yuv_pattern: yuv data pattern
//  Return: none
//  Note:   
/******************************************************************************/
PUBLIC BOOLEAN SWJPEG_IsEncodeSupported(JINF_YUV_FORMAT_E sample_format,
													JINF_YUV_PATTERN_E yuv_pattern);

/****************************************************************************/
/* Purpose:	get decode buffer size											*/
/* Author:	shan.he															*/
/* Input:      																*/
/*			in_param_ptr			--pointer of input parameters			*/
/*			out_param_ptr			--pointer of output parameters			*/	
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E SWJPEG_GetDecodeBufSize(JINF_GET_DEC_BUF_SIZE_IN_T *in_param_ptr,
										   JINF_GET_DEC_BUF_SIZE_OUT_T *out_param_ptr);

#ifdef __cplusplus
}
#endif

#endif
