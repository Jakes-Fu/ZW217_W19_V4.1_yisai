/******************************************************************************
 ** File Name:      decoder_interface.h                                       *
 ** Author:         jianping.wang                                             *
 ** DATE:           12/04/2007                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains decoder app interface 		  *
 **         		description												  *
 ** 							                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/04/2007     Jianping.Wang    Create.                                   *
 ******************************************************************************/
#ifndef _DECODER_INTERFACE_H_
#define _DECODER_INTERFACE_H_


#include "stdlib.h"
#include "sci_types.h"
#include "scaling_down_cus.h"
#include "image_proc.h"


typedef enum
{
	IMGPROC_PNG_APP_SUCCESS = 0,
	IMGPROC_PNG_APP_FILETYPE_ERR,
	IMGPROC_PNG_APP_IMG_SIZE_NOSUPPORT_ERR,
	IMGPROC_PNG_APP_BUFFER_SIZE_ERR,
	IMGPROC_PNG_APP_FILE_ERR,
	IMGPROC_PNG_APP_UNKNOWN_ERR
}IMGPORC_PNG_APP_RETURN_E;


typedef enum
{
	IMGPROC_BMP_APP_SUCCESS = 0,
	IMGPROC_BMP_APP_FILETYPE_ERR,
	IMGPROC_BMP_APP_IMG_SIZE_NOSUPPORT_ERR,
	IMGPROC_BMP_APP_BUFFER_SIZE_ERR,
    IMGPROC_BMP_APP_FILE_ERR,
	IMGPROC_BMP_APP_UNKNOW_ERR	
}IMGPROC_BMP_APP_ERROR_E;

typedef struct bmp_decode_app_t_
{
    uint8	*scalingdown_buf_ptr;
    uint8	*target_buf_ptr;
    uint8	*file_buf_ptr;
    int32 	scalingdown_buf_ptr_size;
    int32   target_buf_ptr_size;
    int32   file_buf_ptr_size;
    int32	*src_width_ptr;
    int32	*src_height_ptr;
    int32	*scr_color_depth_ptr;
    int32	src_img_size;
    int32	target_width;
    int32	target_height;
    int32	*ret_target_width_ptr;
    int32	*ret_target_height_ptr;	
    void	(*app_read_data) ( uint8 *data_ptr,  uint32	 data_count, uint32 *ret_data_count_ptr);

    IMG_PROC_RECT_T imgrect;
    P_DATA_TYPE_CUS	target_type;
    void *(*img_alloc)(uint32 size);
    void (*img_free)(void *);
} IMGPROC_BMP_DECODE_APP_T;

typedef struct _imgproc_png_display_info_tag
{
	uint16 image_orgin_width;
	uint16 image_orgin_height;
	uint16 image_disaplay_width;
	uint16 image_disaplay_height;
	uint8 *data_buf_ptr;
}IMGPROC_PNG_DECODE_APP_OUTPUT_T;

typedef struct _imgproc_png_decode_app_input_tag
{	
	uint8 *read_buf_ptr;
	uint32 read_buf_size;
	uint8 *scalingdown_buf_ptr;		//96k bytes	
	uint32 scalingdown_buf_size;
	uint8 *target_buf_ptr;			//300k bytes
	uint32 target_buf_size;
	uint8 *save_buf_ptr;
	uint32 save_buf_size;	
	uint16 target_width;
	uint16 target_height;
	uint32 backgroundcolor;  //r g b;
	P_DATA_TYPE_CUS target_type;//RGB565 ; //RGB888	
	void (*app_read_data)(uint8 *data_ptr, uint32 data_count, uint32 *ret_line_ptr);	
	IMG_PROC_RECT_T imgrect;
	uint32 img_file_size;
	uint32 handle;

	uint16 target_buf_width; 
	uint16 target_buf_height;
}IMGPROC_PNG_DECODE_APP_INPUT_T;

typedef struct _bmp_create_app_input_tag
{
    uint32 target_width;
    void *(*img_alloc)(uint32 size);
    void (*img_free)(void *);
}BMP_APP_INPUT_T;

typedef struct _imgproc_gif_display_info_tag
{
	uint16 image_orgin_width;
	uint16 image_orgin_height;
	uint16 image_disaplay_width;
	uint16 image_disaplay_height;
	uint16 current_frame_number;
	uint16 delay_time; //ms
	uint8 *data_buf_ptr;

	IMGPROC_GIF_APP_RETURN_E ret;

	BOOLEAN is_decode_finish;
    uint8   padding0;//reserved
    uint8   padding1;//reserved
    uint8   padding2;//reserved
	
}IMGPROC_GIF_DISPLAY_INFO_T;
#if 0
typedef struct _imgproc_gif_decode_app_input_tag   
{		
	uint8 *read_file_buf_ptr;		//32k bytes
	uint32 read_file_buf_size;
	uint8 *scalingdown_buf_ptr;		//96k bytes	
	uint32 scalingdown_buf_size;
	uint8 *target_buf_ptr;			//300k bytes
	uint32 target_buf_size;
	uint8 *save_pre_frame_buf_ptr;	//300k bytes
	uint32 save_pre_frame_buf_size;
	uint16 target_width;
	uint16 target_height;
	uint32 backgroundcolor;  //r g b;
	P_DATA_TYPE_CUS target_type;//RGB565 ; //RGB888	
	void (*app_read_data)(uint8 *data_ptr, uint32 data_count, uint32 *ret_line_ptr);
	void (*app_notice_display)(IMGPROC_GIF_DISPLAY_INFO_T *display_info);
	BOOLEAN (*app_is_continue_next_frame)(void);  // TRUE continue decoder; False; quit current decoder;
	                                              //No return: will be wait until this function return;
    uint32 handle;	                                              
} IMGPROC_GIF_DECODE_APP_INPUT_T;
#endif
/*****************************************************************************/
//  Description:png decode app interface
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
IMGPORC_PNG_APP_RETURN_E PNGAPP_Decode(
										       IMGPROC_PNG_DECODE_APP_INPUT_T  *input_para_ptr,
										       IMGPROC_PNG_DECODE_APP_OUTPUT_T *output_ptr
									          );
/*****************************************************************************/
//  Description:free png decode resource when MMI cann't normaly complete png application 
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
void PNGAPP_FreeDecodeRes(uint32 handle);
/*****************************************************************************/
//  Description:free gif decode resource when MMI cann't normaly complete gif application 
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			none
//		output:
//			noen 
//		return:
//			none
/*****************************************************************************/
void GIFDECODEAPP_FreeDecodeRes(uint32 handle);



PUBLIC IMGPROC_BMP_APP_ERROR_E  BMPAPP_BmpDecode(IMGPROC_BMP_DECODE_APP_T *bmp_decode_app_ptr);
PUBLIC IMGPROC_BMP_APP_ERROR_E BMPAPP_WbmpDecode(IMGPROC_BMP_DECODE_APP_T *bmp_decode_app_ptr);


PUBLIC void BMPAPP_Bmp_Destroy_App(BMP_APP_INPUT_T *input_ptr);
PUBLIC void  BMPAPP_Wbmp_Destroy_App(BMP_APP_INPUT_T *input_ptr);

PUBLIC void CX_SCALE_Destroy_cus(cx_memory_funifo_ptr_cus info);

/******************************************************************************/
//  Description: Get gif Information from GIF handler
//  Author:  
//  Input:      
// 		    gif_handler:    gif handler;  		  
//  Output:  info_ptr: gif image information including width, height.
//  Return:  IMGPROC_GIF_APP_SUCCESS            
//  Note:       
/******************************************************************************/
PUBLIC IMGPROC_GIF_APP_RETURN_E  GIF_GetInfo_From_Handler(
                                                					uint32 gif_handler,
                                                					IMGPROC_GIF_INFO_T *info_ptr
                                                				  );

#endif