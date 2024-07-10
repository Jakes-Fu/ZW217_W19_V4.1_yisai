/******************************************************************************
 ** File Name:	jpg_decoder.h                                     	  							*
 ** Author:		Shan.He                                     						*
 ** DATE:		08/10/2009                                              			 				*
 ** Copyright: 	2009 Spreatrum, Incoporated. All Rights Reserved.         				*
 ** Description:                                                              								*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       								*
 ** -------------------------------------------------------------------------*
 ** DATE              NAME             DESCRIPTION                            						*
 ******************************************************************************/
#ifndef _JPEG_DECODER_H_
#define _JPEG_DECODER_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "img_dec_hal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

 /****************************************************************************/
/* Purpose:	Initialize jpeg resource.					                    */
/* Author:																	*/
/* Input:      																*/
/* Output:																	*/
/* Return:																	*/
/* Note:   Should be called once before any of the others functions be called.                */
/****************************************************************************/
PUBLIC uint32 JPEGDEC_Init();

 /****************************************************************************/
/* Purpose:	get jpeg information											*/
/* Author:																	*/
/* Input:      																*/
/* Output:																	*/
/* Return:																	*/
/* Note:    																*/
/****************************************************************************/
PUBLIC IMG_DEC_RET_E JPEGDEC_GetInfo(IMG_DEC_HAL_SRC_T *src_info_ptr, 
											IMG_DEC_JPEG_INFO_T	 *info_ptr);

 /****************************************************************************/
/* Purpose:	create handle													*/
/* Author:																	*/
/* Input:      																*/
/* Output:																	*/
/* Return:																	*/
/* Note:    																*/
/****************************************************************************/
PUBLIC uint32 JPGDEC_CreateHandle(IMG_DEC_HAL_SRC_T *src_param_ptr);

 /****************************************************************************/
/* Purpose:	decode one jpeg													*/
/* Author:																	*/
/* Input:      																*/
/* Output:																	*/
/* Return:																	*/
/* Note:    																*/
/****************************************************************************/
PUBLIC IMG_DEC_RET_E JPGDEC_GetFrame(IMG_DEC_FRAME_IN_PARAM_T *in_param_ptr, 
									    	IMG_DEC_FRAME_OUT_PARAM_T *out_param_ptr, 
									    	IMG_DEC_FRAME_EXTRA_T *extra_info_ptr);


 /****************************************************************************/
/* Purpose:	destroy handle													*/
/* Author:																	*/
/* Input:      																*/
/* Output:																	*/
/* Return:																	*/
/* Note:    																*/
/****************************************************************************/
PUBLIC IMG_DEC_RET_E JPGDEC_DestoryHandle(uint32 handle, IMG_DEC_EXIT_TYPE exit_type);

#ifdef __cplusplus
	}
#endif

#endif
