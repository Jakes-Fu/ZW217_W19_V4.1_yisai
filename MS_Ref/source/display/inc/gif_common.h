/******************************************************************************
 ** File Name:      gif_common.h                                              *
 ** Author:         Jianping.Wang                                             *
 ** DATE:           2009/01/14                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define BMP operation interface                            *
 ** Note:           None                                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2009/01/14     Jianping.Wang     Create.                                  *
 ******************************************************************************/
#ifndef _GIF_COMMON_H
#define _GIF_COMMON_H


#ifdef __cplusplus
extern   "C"
    {
#endif

#include "img_ref_types.h" 

typedef struct _img_gif_create_input_tag
{
	uint8 *src_data_ptr;
	uint32 src_data_size;
	uint16 *background__ptr;
	uint16 background_width;
	uint16 background_height;
	uint16 image_w;
	uint16 image_h;

	uint16 background_x;
	uint16 background_y;	
	IMGREF_DATA_FORMAT_E format;
	
}IMG_GIF_CREATE_HANDLE_INPUT_T;

typedef struct _img_gif_decode_getframe_input_tag
{
	uint8 *target_buf_ptr;
	uint32 targer_buf_size;
	uint16 start_x;
	uint16 start_y;
	uint32 *gif_handler;

    IMG_RECT_T      disprect;
    IMG_RECT_T      imgrect;
    IMGREF_DATA_FORMAT_E format;	
}IMG_GIF_DECODE_GETFRAME_INPUT_T;
typedef struct _img_gif_info_tag
{
	uint16 width;
	uint16 height;
	uint16 frame_num;
	uint16 pading0;	
}IMG_GIF_INFO_T;

typedef enum
{
	IMG_GIF_DECODER_SUCCESS = 0   ,      
	IMG_GIF_DECODER_MALLOC_MEM_FAIL,
	IMG_GIF_DECODER_FREE_MEM_FAIL,
	IMG_GIF_DECODER_GIF_VERSION_ERR,
	IMG_GIF_DECODER_GIF_FILE_ERR,
	IMG_GIF_DECODER_INPUT_PARAMETER_ERR,
	IMG_GIF_DECODER_NO_SUPPORT_SIZE,
	IMG_GIF_DECODER_FINISH_ANIMATION,	
	IMG_GIF_DECODER_MAX	
}IMG_GIF_DECODER_RETURN_E;

/*****************************************************************************/
//  Description :
//     Get Res Gif  Header (Used for Sppreadtrum) 
//  Author:
//      Jianping.Wang
/*****************************************************************************/
IMG_GIF_DECODER_RETURN_E IMGGIFDECODER_Res_GetInfo(
                											   uint8 *src_data_ptr,
                											   uint32 src_data_size,
                											   IMG_GIF_INFO_T *info_ptr
                											   );
/*****************************************************************************/
//  Description :
//     get gif image 
//  Author:
//      Jianping.Wang
//  Input:
//		input_ptr - input parameter pointer
/*****************************************************************************/
IMG_GIF_DECODER_RETURN_E IMGGIFDECODER_GetFrame(	
					                                        IMG_GIF_DECODE_GETFRAME_INPUT_T  *input_ptr
							                               );
/*****************************************************************************/
//  Description :
//     get gif image 
//  Author:
//      Jianping.Wang
/*****************************************************************************/
PUBLIC IMG_GIF_DECODER_RETURN_E IMGGIFDECODER_GetCurFrame(	
												                        IMG_GIF_DECODE_GETFRAME_INPUT_T  *input_ptr
											                           );
/*****************************************************************************/
//  Description :
//     free gif decoder handle 
//  Author:
//      Jianping.Wang
//  Input:
//		gif_handler - input gif decoder handle
/*****************************************************************************/
 IMG_GIF_DECODER_RETURN_E IMGGIFDECODER_FreeHandler(			
                                    							uint32 gif_handler
                                    							);		
/*****************************************************************************/
//  Description :
//     create gif decoder handle 
//  Author:
//      Jianping.Wang
//  Input:
//		input_ptr - input parameter pointer
//		gif_handler - output gif decoder handle pointer
/*****************************************************************************/
IMG_GIF_DECODER_RETURN_E IMGGIFDECODER_CreateHandler (
                                            						IMG_GIF_CREATE_HANDLE_INPUT_T *input_ptr,
                                            						uint32 *gif_handler
                                            					  );
/*****************************************************************************/
//  Description :
//     get gif information 
//  Author:
//      Jianping.Wang
//  Input:
//		src_data_ptr - gif stream address
//		src_data_size -size of gif stream
//		info_ptr - output information pointer
/*****************************************************************************/
IMG_GIF_DECODER_RETURN_E IMGGIFDECODER_GetInfo(
                                    						uint8 *src_data_ptr,
                                    						uint32 src_data_size,
                                    						IMG_GIF_INFO_T *info_ptr
                                    					 );	
	
			                 
#ifdef __cplusplus
    }
#endif
			                 
		
#endif  //GIF_H

