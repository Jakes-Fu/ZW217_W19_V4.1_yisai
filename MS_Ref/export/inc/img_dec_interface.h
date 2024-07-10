#ifndef _IMG_DEC_INTERFACE_H_
#define _IMG_DEC_INTERFACE_H_

#include "sci_types.h"
#include "os_api.h"
#include "jpeg_interface.h"

#define IMG_LOG SCI_TRACE_LOW

typedef enum
{  

    IMG_DEC_TYPE_BMP = 0,		
    IMG_DEC_TYPE_WBMP,			
	IMG_DEC_TYPE_JPEG,			
	IMG_DEC_TYPE_PNG,			
	IMG_DEC_TYPE_GIF,	
	IMG_DEC_TYPE_ICO,	
    IMG_DEC_TYPE_UNKNOWN     	

}IMG_DEC_TYPE_E;



typedef enum

{
	IMG_DEC_RGB565 = 0,
	IMG_DEC_RGB888,
	IMG_DEC_RGB666,
	IMG_DEC_YUV,
    IMG_DEC_ARGB888,
	IMG_DEC_JPEG//only for JPEG Create miniature
	
}IMG_DEC_DATA_FORMAT_E;


typedef enum
{  
    IMG_DEC_RET_SUCCESS = 0,
	IMG_DEC_RET_UNKNOWN_FILE_TYPE ,			
	IMG_DEC_RET_MEMORY_NOT_ENOUGH ,		
	IMG_DEC_RET_BAD_DATA,
	IMG_DEC_RET_INVALID_HANDLE,
	IMG_DEC_RET_BEYOND_MAX_RESOLUTION,
	IMG_DEC_RET_BEYOND_MAX_FILESIZE,
	IMG_DEC_RET_FILE_SIZE_NOTENOUGH,
	IMG_DEC_RET_SFS_ERROR,
	IMG_DEC_RET_BUSYING,
	IMG_DEC_RET_SCALING_ERROR,
	IMG_DEC_RET_FAILED	
}IMG_DEC_RET_E;


typedef enum
{
	IMG_DEC_FORMAT_YUV444 = 0,
	IMG_DEC_FORMAT_YUV422,
	IMG_DEC_FORMAT_YUV420,
	IMG_DEC_FORMAT_YUV411,
	IMG_DEC_FORMAT_YUV400
	
}IMG_DEC_YUV_FORMAT_E;



typedef struct img_dec_rect_tag
{

    uint16 left;
    uint16 top;
    uint16 right;
    uint16 bottom;  
	
}IMG_DEC_RECT_T;

typedef struct _img_dec_bmp_info_tag
{
	uint16 img_width;		
	uint16 img_height;

	uint32 bit_depth;
    BOOLEAN is_rle_compressed;
    uint8 padding0;//reserved
	uint8 padding1;//reserved
	uint8 padding2;//reserved
}IMG_DEC_BMP_INFO_T;

//wbmp detail information
typedef struct _img_dec_wbmp_info_tag
{
	uint16 img_width;		
	uint16 img_height;
	uint32 bit_depth;
	
}IMG_DEC_WBMP_INFO_T;

//gif detail information
typedef struct _img_DEC_gif_info_tag
{
	uint16 img_width;		
	uint16 img_height;
	BOOLEAN is_interlaced;//	
	uint8 padding0;//reserved
	uint8 padding1;//reserved
	uint8 padding2;//reserved
}IMG_DEC_GIF_INFO_T;

//png detail information
typedef struct _img_dec_png_info_tag
{
	uint16 img_width;		
	uint16 img_height;

	BOOLEAN is_interlaced;
    uint8 padding0;//reserved
	uint8 padding1;//reserved
	uint8 padding2;//reserved
	uint32 bit_depth;
	uint32 pixel_depth;
}IMG_DEC_PNG_INFO_T;

typedef struct img_dec_jpeg_thumbnail_info_tag
{
	BOOLEAN   	is_thumb_existed;
    uint8       padding0;//reserved
	uint8       padding1;//reserved
	uint8       padding2;//reserved
	uint16		thumb_width;
	uint16		thumb_height;
	uint32		thumb_offset;
   	uint32		thumb_size;      

}IMG_DEC_JPEG_THUMBNAIL_INFO_T;




typedef struct _img_dec_jpeg_info_tag
{

	BOOLEAN	is_dec_supported;   	
	BOOLEAN is_progressive; 
    uint8   padding0;//reserved
	uint8   padding1;//reserved	
	uint16  img_width;		
	uint16  img_heigth;
	IMG_DEC_YUV_FORMAT_E	sample_format;		
	IMG_DEC_JPEG_THUMBNAIL_INFO_T	thumbnail_info;

}IMG_DEC_JPEG_INFO_T;


#define MAX_ICO_COUNT 256

typedef struct _img_dec_ico_info_tag
{	
	uint32 count;	
	uint16 img_width[MAX_ICO_COUNT];
	uint16 img_height[MAX_ICO_COUNT];
	
}IMG_DEC_ICO_INFO_T;


typedef struct _img_dec_info_tag
{
	IMG_DEC_TYPE_E img_type;
	union{

	IMG_DEC_BMP_INFO_T bmp_info;
	IMG_DEC_WBMP_INFO_T wbmp_info;
     	IMG_DEC_GIF_INFO_T gif_info;
     	IMG_DEC_PNG_INFO_T png_info;
     	IMG_DEC_JPEG_INFO_T jpeg_info;
	IMG_DEC_ICO_INFO_T ico_info;
	
	}img_detail_info;
	
    uint32 align_num;
	
}IMG_DEC_INFO_T;

typedef struct _img_dec_src_tag
{
	uint8 *src_ptr;  
	uint32 src_file_size; 
	uint16 *file_name_ptr;
	
	void *(*img_alloc)(uint32 size);
	void (*img_free)(void *);

	IMG_DEC_INFO_T *info_param_ptr;


}IMG_DEC_SRC_T;

typedef enum
{
	IMG_DEC_TARGET_SIZE_RESIZABLE = 0,		//the target size maybe resized and the actual output size maybe not equal to the input target size
	IMG_DEC_TARGET_SIZE_UNRESIZABLE		   //the target size can not be resized and the actual output size must eqaul to the input target size
}IMG_DEC_MODE_E;


//get frame output parmeter

typedef struct _img_dec_get_frame_out_param_tag
{
	BOOLEAN is_decode_finished;
    uint8   is_process_alpha;
	uint8   padding0;//reserved
	uint8   padding1;//reserved
	uint32  delay_time;    

	uint32 real_width;	
	uint32 real_height; 
	uint32 img_size; 
	uint32 current_frame;

	uint32 y_addr_offset; 
	uint32 u_addr_offset;
	uint32 v_addr_offset;
	
	IMG_DEC_YUV_FORMAT_E yuv_format;

}IMG_DEC_FRAME_OUT_PARAM_T;

//get frame input parameter

typedef struct _img_dec_frame_in_param_tag
{
	uint32 handle;

	uint8 		*target_ptr;
	uint32 		target_buf_size;
	uint16		target_width; 
	uint16		target_height;	
	IMG_DEC_RECT_T img_rect;
	IMG_DEC_RECT_T target_rect;
	IMG_DEC_DATA_FORMAT_E data_format;
	uint8             frame_index;
	BOOLEAN		is_dec_thumbnail;			//only used for jpeg decoder: TRUE: decode jpeg thumbnail, FALSE: decoder the original jpeg
    BOOLEAN     is_exist_background;//used to gif decoder
	uint8       padding1;//reserved
	uint8       padding2;//reserved
	uint8       *alpha_buf_ptr;
    uint32      alpha_buf_size;       
	uint32 (*write_data) ( uint8 *data_ptr, uint32 data_count);
	void (*callback)(uint32 handle, IMG_DEC_RET_E ret, IMG_DEC_FRAME_OUT_PARAM_T *out_param_ptr, uint32 app_param_ptr);

	uint32 app_param_ptr;//buf saving app parameter.
	uint32 app_param_size;  //buf size

    JINF_QUALITY_E quality; 
	uint16	target_buf_width; 
	uint16	target_buf_height;
	uint32  custom_alpha_color;
	IMG_DEC_MODE_E img_dec_mode;

} IMG_DEC_FRAME_IN_PARAM_T;


/****************************************************************************/
/* Purpose:	get image info...				 						*/
/* Author:														*/
/* Input:      														*/
/*			src_param_ptr --points to source param					*/
/* Output:	info_param_ptr        --image information					*/
/* Return:	operation results										*/
/* Note:    														*/
/****************************************************************************/

PUBLIC IMG_DEC_RET_E IMG_DEC_GetInfo(IMG_DEC_SRC_T *src_param_ptr, IMG_DEC_INFO_T *info_param_ptr);

/****************************************************************************/
/* Purpose:	Create Decoder handle...				 	*/
/* Author:														*/
/* Input:      																*/
/*			src_param_ptr --pointer to source param						*/
/* Output:	handle_ptr        -- pointer to the handle						*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/	
PUBLIC IMG_DEC_RET_E IMG_DEC_Create (IMG_DEC_SRC_T *src_param_ptr, uint32  *handle_ptr);

/****************************************************************************/
/* Purpose:	Create Decoder handle, get detail info				 	*/
/* Author:														*/
/* Input:      																*/
/*			src_param_ptr --pointer to source param						*/
/* Output:	handle_ptr        -- pointer to the handle						*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/	
PUBLIC IMG_DEC_RET_E IMG_DEC_CreateEx (IMG_DEC_SRC_T *src_param_ptr, uint32  *handle_ptr);

/****************************************************************************/
/* Purpose:	Destroy Decoder Handle				 	*/
/* Author:														*/
/* Input:      																*/
/*			handle -- handle will be destroyed						*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/	
PUBLIC IMG_DEC_RET_E IMG_DEC_Destroy (uint32 handle);

/****************************************************************************/
/* Purpose:	Get one Frame by handle				 	*/
/* Author:														*/
/* Input:      																*/
/*			in_param_ptr -- pointer to frame input param						*/
/* Output:	out_param_ptr        -- pointer to the handle						*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/	
PUBLIC IMG_DEC_RET_E IMG_DEC_GetFrame(IMG_DEC_FRAME_IN_PARAM_T *in_param_ptr,
				      IMG_DEC_FRAME_OUT_PARAM_T *out_param_ptr

);

/****************************************************************************/
/* Purpose:	Init decoder resources  			 							*/
/* Author:																	*/
/* Input:   None															*/
/* Output:	None															*/
/* Return:	None															*/
/* Note:    Should be called before any of IMG_DEC_xxx function.		       */
/****************************************************************************/	
PUBLIC void IMG_DEC_Init(void);

#endif


