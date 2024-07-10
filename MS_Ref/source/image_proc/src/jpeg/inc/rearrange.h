/*****************************************************************************
** File Name:      rearrange.h                                               *
** Author:         Yi.Wang                                                   *
** Date:           03/10/2009                                                *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    implementate the mcu rearrangement according to some rule *
******************************************************************************

******************************************************************************
  **                        Edit History                                     *
  ** ------------------------------------------------------------------------*
  ** DATE            NAME             DESCRIPTION                            *
  ** 03/10/2009      Yi.Wang          Create.                                *
******************************************************************************/

#ifndef REARRANGE_H
#define REARRANGE_H

#include "sci_types.h"

#if (defined(JPG_DEC_SUPPORT_JPG_HW) || defined(JPG_ENC_SUPPORT_JPG_HW))


int32 JPEGFW_Rearrange_image( uint8 *input_image_ptr, 
							  uint8 *output_image_ptr, 
							  uint8 mcu_format, 
							  uint32 width, 
							  uint32 height, 
							  uint8 scale_factor);

void JPEGFW_SampleChroma422to420(uint8 *input_uv_ptr, 
								 uint8 *output_uv_ptr, 
								 uint32 chroma_width, 
								 uint32 chroma_height
								 );

#endif

#endif
