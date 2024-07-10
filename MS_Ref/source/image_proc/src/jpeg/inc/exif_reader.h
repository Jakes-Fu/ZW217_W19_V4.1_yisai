/******************************************************************************
 ** File Name:       exif_reader.h                                            *
 ** Author:          Shan.He                                       			  *
 ** DATE:            09/15/2010                                               *
 ** Copyright:       2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:       parse exif 	                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ---------------------------------------------------------------------------*
 ** DATE              NAME             DESCRIPTION                            *
 **09/15/2010		  Shan.He			Created	
 ******************************************************************************/
 #ifndef _EXIF_READER_H_
#define _EXIF_READER_H_

/**---------------------------------------------------------------------------*
 **                         Dependence                                        *
 **---------------------------------------------------------------------------*/
#include "jpeg_interface.h"
#include "jpeg_stream.h"
#include "jpeg_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************
**	Name : 			Jpeg_ParseExif
**	Description:	Parse exif
**	Author:			Shan.he
**  Parameters:     
**                  context_ptr:    context pointer
**                  exif_info_ptr:  pointer of exif info structure, output parameter    
**	Note:           
*****************************************************************************/
PUBLIC JPEG_RET_E Jpeg_ParseExif(JPEG_READ_STREAM_CONTEXT_T *context_ptr,
                                	JINF_EXIF_INFO_T *exif_info_ptr,
                                	JINF_THUMBNAIL_INFO_T *thumbnail_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif	
		
#endif		