/******************************************************************************
 ** File Name:      image_proc.h                                              *
 ** Author:         jianping.wang                                             *
 ** DATE:           12/04/2007                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains gif decode app interface 		  *
 **         		description												  *
 ** 							                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/04/2007     Jianping.Wang    Create.                                   *
 ******************************************************************************/
#ifndef _IMAGE_PROC_H_
#define _IMAGE_PROC_H_

#include "stdlib.h"
#include "sci_types.h"

typedef enum
{
	IMGPROC_RGB565 = 0,
	IMGPROC_RGB888,  //reserved for future use	
	IMGPROC_MAX
}IMGPROC_DATA_FORMAT_E;

typedef struct
{
    uint16 top;
    uint16 left;
    uint16 right;
    uint16 bottom;  
}IMG_PROC_RECT_T;

typedef enum
{
	IMGPROC_GIF_APP_SUCCESS = 0,
	IMGPROC_GIF_APP_FILETYPE_ERR,
	IMGPROC_GIF_APP_IMG_SIZE_NOSUPPORT_ERR,
	IMGPROC_GIF_APP_BUFFER_SIZE_ERR,
	IMGPROC_GIF_APP_FILE_ERR,
	IMGPROC_GIF_APP_UNKNOWN_ERR,
	IMGPROC_GIF_APP_CREATE_HANDLE_FAIL
}IMGPROC_GIF_APP_RETURN_E;

typedef struct _imgproc_gif_decode_app_output_tag
{
	uint16 frame_num;
	
}IMGPROC_GIF_DECODE_APP_OUTPUT_T;


typedef struct _imgproc_gif_info_tag
{
	uint16 width;
	uint16 height;

	uint32 memory_size_needed;  //Memoyr needed for  decoder
	
}IMGPROC_GIF_INFO_T;

typedef struct _imgproc_gif_decode_getframe_input_tag
{
	uint8 *target_buf_ptr;
	uint32 targer_buf_size;
	uint32 *gif_handler;
	
}IMGPROC_GIF_DECODE_GETFRAME_INPUT_T;

typedef struct _imgproc_gif_decode_frame_info_tag
{
	uint32 frame_delay; //ms
	BOOLEAN is_decode_finish;  //True means decoder finish;
	uint8   padding0;//reserved
	uint8   padding1;//reserved
	uint8   padding2;//reserved
}IMGPROC_GIF_DECODE_FRAME_INFO_T;



typedef struct _imgproc_gif_src_info_tag
{
	uint8 *src_data_ptr;
	uint32 src_data_size;

	IMGPROC_DATA_FORMAT_E data_format;

	
}IMGPROC_GIF_SRC_INFO_t;
/******************************************************************************/
//  Description: Get gif Information from GIF file buffer
//  Author:  
//  Input:      
// 		    input_ptr:    input buffer; 	  
//  Output:  info_ptr: gif image information including width, height.
//  Return:  Success: IMGPROC_GIF_APP_SUCCESS
//              Failure:        other
//  Note:       
/******************************************************************************/
PUBLIC IMGPROC_GIF_APP_RETURN_E  GIF_GetInfo_From_File(
                                             				   IMGPROC_GIF_SRC_INFO_t *input_ptr,
                                            				   IMGPROC_GIF_INFO_T *info_ptr
                                            				 );
/******************************************************************************/
//  Description:Create GIF Decoder Handler
//  Author:  
//  Input:      
// 		    input_ptr:    input buffer; 
//  Output:  gif_handler: gif handler
//  Return:  Success: IMGPROC_GIF_APP_SUCCESS
//              Failure:        other
//  Note:       
/******************************************************************************/
PUBLIC IMGPROC_GIF_APP_RETURN_E GIF_CreateHandler(
                                                    	 IMGPROC_GIF_SRC_INFO_t *input_ptr,
                                                    	 uint32 *gif_handler
                                                       );
/******************************************************************************/
//  Description:Free GIF Decoder Handler
//  Author:  
//  Input:      
// 		    gif_handler: gif handler
//               
// 		  
//  Output:  None
//  Return:  Success:    IMGPROC_GIF_APP_SUCCESS
 //              Failure:       other
//  Note:       
/******************************************************************************/
PUBLIC IMGPROC_GIF_APP_RETURN_E GIF_FreeHandler(			
                                                       uint32 gif_handler
                                                    );
/******************************************************************************/
//  Description:Get one gif frame by Handler
//  Author:  
//  Input:      
// 		    input_ptr: GIF Getframe input buffer 
//  Output:  frame_info_ptr: frame information
//  Return:  Success:    IMGPROC_GIF_APP_SUCCESS
//              Failure:       other
//  Note:       
/******************************************************************************/
PUBLIC IMGPROC_GIF_APP_RETURN_E GIF_GetFrame(	
                                                	IMGPROC_GIF_DECODE_GETFRAME_INPUT_T *input_ptr,
                                                	IMGPROC_GIF_DECODE_FRAME_INFO_T *frame_info_ptr
                                                );


#endif




