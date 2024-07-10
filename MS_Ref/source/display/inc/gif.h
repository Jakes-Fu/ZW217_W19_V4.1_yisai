/******************************************************************************
 ** File Name:      gif.h                                                     *
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
#ifndef GIF_H
#define GIF_H

#ifdef __cplusplus
extern   "C"
    {
#endif


#include "sci_types.h"
#include "dal_img.h"

#define GIF_COLOR_TABLE_SIZE		256
#define GIF_PIXLE_BYTE_NUM			2
#define GIF_PIXLE_BYTE_NUM_ARGB888 4
#define GIF_DEC_TEMP_BUF_LEN		16384
#define GIF_DEC_STACK_BUF_LEN		(4096+1)

#define GIF_RES_MAX_CODES    		4095
#define GIF_RES_FILE_HEAD_NUM       13

#define GIF_CLEAR     256
#define GIF_END       257
#define GIF_NEWCODE   258

#define GIF_RES_WIDTH_MAX	 1024
#define GIF_RES_HEIGHT_MAX	 1024

#define RGB88TORGB565_TEST(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | (((b) >> 3) & 0x1f))

#define GIFENDIANCONVERT(a) ((((a)<<8)&0xff00)|(((a)>>8)&0xff))
#define GIF_RES_HEADER_SIZE (sizeof(GIF_RES_HEADER_T))

typedef struct 
{
	int8  header_mark[4];
	uint16 frame_count;
	uint16 img_width;
	uint16 img_height;
	uint16 reserved;
	
}GIF_RES_HEADER_T;




typedef enum
{
		IMG_GIF_NO_ACTION = 0,
		IMG_GIF_KEEP_IMAGE    ,
		IMG_GIF_RESTORE_BACKGROND,
		IMG_GIF_RESTORE_IMAGE,
		IMG_GIF_DISPLAY_METHOD_ERR
}IMG_GIF_DISPOSAL_METHOD_E;

typedef enum
{
	DEC_IDLE = 0,
	DEC_INIT_DONE,
	DEC_GOING,
	DEC_FINISH_A_FRAME,
	DEC_FILE_END,
	DEC_MALLOC_FAIL,
	DEC_FILE_ERR,
	DEC_NO_SUPPORT_SIZE,
	DEC_SUCCESS_DECODE,

	DEC_MAX
}IMG_GIF_DECODER_STATUS_E;

typedef struct _gif_res_dec_rgb_color_tag
{ 
	uint8 r;
	uint8 g;
	uint8 b;
}IMG_GIF_COLOR_TABLE_T;

typedef struct _img_gif_decoder_handler_tag
{
		uint8  *src_data_ptr;
		uint32 src_data_size;
		uint32 ibf;
		uint8 *mem_ptr;
		uint16 *save_preframe_buf_ptr;
		uint32 save_preframe_buf_size;
		uint16 *global_color_table_ptr;
		uint32 global_color_table_index_num;	
		uint8  background_index;
		uint8  transparent_index;
		uint8  transparent_flag;
        uint8  padding;
		uint32  frame_num;
		uint16 *save_background_buf_ptr;
		uint32 save_background_buf_size;
		uint16 background_buf_width;
		uint16 background_buf_height;
		uint16 delay_time;
		uint8  set_dispmeth_flag;
		uint8  dispmeth_pre;
		uint16 image_w;
		uint16 image_h;
		IMG_GIF_DISPOSAL_METHOD_E disp_method;	
		IMG_GIF_DISPOSAL_METHOD_E pre_disp_method;	//previour frame display method
		
		IMG_GIF_DECODER_STATUS_E  status;

   	       IMG_RECT_T imgrect;

		uint16 *save_cur_frame_buf_ptr; 
		uint32 save_cur_frame_buf_size;  //in bytes
		uint16 save_cur_width;
		uint16 save_cur_height;
		IMGREF_DATA_FORMAT_E format;

		uint16 total_num;
		uint16 dec_init;
		
}IMG_GIF_DECODER_HANDLER_T;

typedef struct _gif_res_image_tag /* Image Descriptor */
{      
	uint16 l;
	uint16 t;
	uint16 w;
	uint16 h;
	uint8  pf;
	uint8  is_local_table;
	uint16 is_interlaced;
	uint16 *table_ptr;
	
}GIF_RES_IMAGE_T;

typedef enum
{
	GIF_RES_INIT_SUCCESS = 0, 
	GIF_RES_INIT_FAIL,
	GIF_RES_PARAMETERM_ERROR, 
	GIF_RES_VERSION_ERROR, 
	GIF_RES_NO_SUPPORT_SIZE	
}GIF_RES_INIT_RESULT_E;

typedef struct _gif_res_dec_dscgif_tag   /* Logic Screen Descriptor  */
{		
	int8 header[6];			/* Firma and version */
	uint16 scrwidth;
	uint16 scrheight;
	int8 pflds;
	int8 bcindx;
	int8 pxasrat;
}GIF_RES_DEC_DSCGIF_T;

typedef struct _gif_res_dec_output_pos_tag   
{
	uint16 *target_buf_ptr;
	uint32 target_buf_size;
	uint16 start_x;
	uint16 start_y;
	uint16 end_x;
	uint16 end_y;

	uint16 width;
	uint16 height;

	
	
}GIF_RES_DEC_OUTPUT_POS_T;

typedef struct _gif_res_image_comp_input_tag
{
	IMG_GIF_DECODER_HANDLER_T *gif_handle_ptr;
	GIF_RES_DEC_OUTPUT_POS_T  *output_pos_ptr;
	uint8 *index_buf_ptr;
	GIF_RES_IMAGE_T *image_ptr;
	
}GIF_RES_IMAGE_COMP_INPUT_T;

			                 
#ifdef __cplusplus
    }
#endif
			                 
		
#endif  //GIF_H

