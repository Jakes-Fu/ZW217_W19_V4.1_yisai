/******************************************************************************
 ** File Name:       jpeg.h                                               	  *
 ** Author:          Frank.Yang		                                          *
 ** DATE:            03/17/2008                                               *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME             DESCRIPTION                            *
 ******************************************************************************/

 
#ifndef _JPEG_H_
#define _JPEG_H_

#include "string.h"

#include "sci_api.h"
#include "jpeg_exif.h"
#include "jpeg_scaling_api.h"
#include "jpeg_interface.h"
#include "jpeg_head_info.h"

#include "jpeg_types.h"

#if (defined(JPG_DEC_SUPPORT_JPG_HW) || defined(JPG_ENC_SUPPORT_JPG_HW))
#include "jpeg_common.h"
#include "jpeg_fw_def.h"
#endif

/************************************************************************/
/* MACRO Define                                                         */
/************************************************************************/
//for saving code sapce, the following macros defined
/*
*HW will skip forward at most 255 bytes for 64 words aligned,so,we have to add 256 bytes
*48 bytes is the length of FIFO,12 words
*/
#define	STREAM_BUFFER_SIZE_EXTEND		256

#define FLOAT_PRECISION			1024
#define JPEG_RATIO_CALC(a,b)	((a) * FLOAT_PRECISION / (b))

#define	JPEG_EVENT_WAIT_TIME			(20 * 1000)	//ms
#define JPEG_EVENT_WAIT_QUIT_TIME		(20 * 1000)  //ms
#define JPEG_WAIT_VSP_TIME				(10 * 1000)	 //ms

#define LOCK_FLAG_NONE							0XFF
#define LOCK_FLAG_FREE_PROTECTION				0

/*
*pingpong buffers indicator,used in encoding and decoding mode.
*/					  
#define	JPEG_PINGPONG_BUF_0				0
#define	JPEG_PINGPONG_BUF_1				1

/*add by shan.he*/
/*max stream pingpong buffer size, make it word align*/
#define MAX_STREAM_BUF_SIZE (1 * 1024 * 1024 - 4)	
/*max MCU number that YUV pingpong buffer can contain*/
#define MAX_MCU_IN_YUV_BUF		  (65535)
#define JPEG_ALIGN_FOWARD(_align_data, _align_uint) (((_align_data) + (_align_uint) - 1) / (_align_uint) * (_align_uint))
#define JPEG_ALIGN_BACKWARD(_align_data, _align_uint) ((_align_data) / (_align_uint) * (_align_uint))
#define JPEG_ALIGN(_align_data, _align_shift) 	(((_align_data) + (1 << (_align_shift)) - 1) >> (_align_shift) << (_align_shift))

/*stream pingpong buffer size*/
#define STREAM_BUF_SIZE 	 (16 * 1024)

#define HW_SCALING_MAX_TIME 	2

//resource jpeg ID
#define JPEG_RES_ID_0 			  'S'
#define JPEG_RES_ID_1 			  'J'
#define JPEG_RES_ID_2 			  'P'
#define JPEG_RES_ID_3 			  'G'



#if (defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G) || defined(PLATFORM_SC6530)) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define JPG_HARDWARE_SCALING
#endif

#if (defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G) || defined(PLATFORM_SC6530)) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define CACHE_ENABLE 1
#else
#define CACHE_ENABLE 0
#endif

//cache line is 32-byte-align
#define CACHE_LINE_ALIGN		  32

#define JPEG_WORK_MODE_NORMAL 				0
#define JPEG_WORK_MODE_SWITCH_DEC_ENC 		1
#define JPEG_WORK_MODE_SWITCH_ALONE			2

typedef enum
{
	JPEG_STATE_IDLE = 0,
	JPEG_STATE_DECODING,
	JPEG_STATE_ENCODING,	
	JPEG_STATE_CREATING_MINIATURE,
	JPEG_STATE_GETTING_INFO,	
	JPEG_STATE_DECODING_RES,
	JPEG_STATE_MAX
}JPEG_STATE_E;

typedef enum
{
	JPEG_SIG_DEC_START = 0,
	JPEG_SIG_ENC_START,
	JPEG_SIG_DEC_END,
	JPEG_SIG_ENC_END,
	JPEG_SIG_CREATE_MINIATURE,
	JPEG_SIG_READ_PING_PONG_BUF,
	JPEG_SIG_WRITE_PING_PONG_BUF,
	JPEG_SIG_READ_PING_PONG_BUF_FIRST,	
	JPEG_SIG_WRITE_PING_PONG_BUF_FIRST,	
	JPEG_SIG_OVERTIME,	
	JPEG_SIG_ERROR,	
	JPEG_SIG_QUIT,
	JPEG_SIG_ERROR_RESTART,
	JPEG_SIG_MAX
}JPEG_SIG_E;

typedef enum
{
	JPEG_FDCT_MODE_NORMAL = 0,
	JPEG_FDCT_MODE_FAST,
	JPEG_FDCT_MODE_MAX
}JPEG_FDCT_MODE_E;

typedef struct JPEG_SIZE_tag
{
	uint16	w;
	uint16 	h;
}JPEG_SIZE_T;

typedef enum
{
	JPEG_PLATFORM_6800 = 0,
	JPEG_PLATFORM_ROCKY,
	JPEG_PLATFORM_HIMALAYA,
	JPEG_PLATFORM_NILE,
	JPEG_PLATFORM_8800E,
	JPEG_PLATFORM_6600L,
	JPEG_PLATFORM_UNKNOWN,
	JPEG_PLATFORM_MAX
}JPEG_PLATFORM_E;

typedef enum
{
	JPEG_RES_SJPG = 0,				//spreadtrum private jpeg
	JPEG_RES_STD_JPG				//standard jpeg
}JPEG_RES_JPG_TYPE_E;

typedef struct tag_jpeg_res_info
{
	uint16					width;
	uint16					height;
	JINF_YUV_FORMAT_E		sample_format;	
	JPEG_QUALITY_E 			quality;
	uint32					head_size;
	JPEG_RES_JPG_TYPE_E     type;
}JPEG_RES_INFO_T;

typedef struct
{
	uint32	src_width;
	uint32	src_height;
	uint32	offset_lines;	
	uint32	slice_height;	
	uint32	subsample_shift;	//subsample factor = 1 << shift
	JINF_YUV_FORMAT_E	format;
	JINF_YUV_CHN_T		src_yuv_chn;
	JINF_YUV_CHN_T		dst_yuv_chn;
}JPEG_SUBSAMPLE_INFO;

#if defined(JPG_HARDWARE_SCALING)	
typedef struct
{
	JPEG_SIZE_T				src_size;				//source image size
	JINF_RECT_T				trim_rect;				//trim rect in source image whic will input to the scaling down module
	JPEG_SIZE_T				out_size;				//output size of scaling down module
	JINF_YUV_FORMAT_E		input_yuv_format;		//imput data format
	JINF_DATA_TYPE_E		output_data_type;		//output data type, YUV422 or RGB565
	JINF_YUV_CHN_T			output_chn;				//output buffer channel
	uint8 					*output_buf;			//output buffer
	uint32					output_buf_size;		//output buffer size
	uint32					input_lines;			//lines input to scale module	
	uint32					slice_height;			//slice height
	uint32					scaled_lines;			
	BOOLEAN					is_first_slice;
	BOOLEAN					is_last_slice;
	uint8					*swap_buf;				//target_width * src_slice_height * 2, 256 bytes aligned
	uint32					swap_buf_size;		
	uint8					*line_buf;				//target_width * 8, 256 bytes aligned
	uint32					line_buf_size;
	JPEG_RET_E				scale_result;
}JPEG_SCALE_PARAM_T;
#endif
					
typedef struct JPEG_ENC_CONTEXT_tag
{
	JINF_ASYNC_CALLBACK		async_callback;

	JINF_YUV_CHN_T			yuv_ping_chn;
	JINF_YUV_CHN_T			yuv_pong_chn;
	uint32					yuv_buf_size;
	
	uint32					slice_height;
	uint32					slice_y_size;
	uint32					slice_uv_size;

	JINF_YUV_INFO_T			src_yuv_info;
	
	uint32					src_width;
	uint32					src_height;
	JINF_READ_YUV_FUNC		yuv_read_func;
	uint32					read_lines;	
	
	uint8 					*stream_ping_buf;
	uint8					*stream_pong_buf;
	uint32					stream_buf_size;

	uint8					*target_buf;
	uint32					target_buf_size;	

	JINF_WRITE_FILE_FUNC 	write_stream_func;
	uint32					write_size;

	void					*temp_buf;
	uint32					temp_buf_size;
	uint32					temp_buf_used_size;

	JINF_QUALITY_E			enc_quality;

	uint32					output_bytes;
	
	uint32					stream_size;

	uint32					encoded_lines;

	JPEG_SIZE_T				src_mcu_aligned_size;
	JINF_YUV_CHN_T			yuv_align_chn;
	BOOLEAN					need_copy_to_target;
	BOOLEAN					need_align;

	#if (defined(JPG_DEC_SUPPORT_JPG_HW) || defined(JPG_ENC_SUPPORT_JPG_HW))
	JPEG_ENC_INPUT_PARA_T	fw_enc_info;
	#endif	

	JINF_YUV_CHN_T			backup_yuv;
	JINF_YUV_FORMAT_E		backup_format;
	JINF_YUV_PATTERN_E		backup_pattern;
	JPEG_SIZE_T				backup_size;
	uint32					backup_lines;

	uint32					lines_in_ping;
	uint32					lines_in_pong;
	//BOOLEAN					is_dec_enc_switch;		//decode and encode switch mode
	BOOLEAN					is_frame_mode;

	JINF_YUV_FORMAT_E		enc_yuv_format;
	JINF_YUV_PATTERN_E		enc_yuv_pattern;

	JINF_READ_YUV_FUNC		yuv_read_func_origin;	

	uint32					work_mode;				// 0: normal, 1: dec_enc_switch, 2: enc/dec alone switch

	JINF_YUV_CHN_T			yuv_read_chn;					//yuv read buffer, used to read yuv data by calling yuv_read_func
	JPEG_SIZE_T				yuv_read_size;					//yuv size in read buffer
	JINF_YUV_CHN_T			yuv_read_cur_chn;				//current pointer
	JPEG_SIZE_T				yuv_read_remaind_size;			//yuv size in read buffer
	uint32					yuv_read_line_offset;
}JPEG_ENC_CONTEXT_T;

typedef struct JPEG_DEC_CONTEXT_tag
{
	/*temp buffer*/
	void					*temp_buf;
	uint32					temp_buf_size;
	uint32					temp_buf_used_size;
	
	/*input jpeg stream pingpong buffer*/
	void					*stream_ping_buf;
	void					*stream_pong_buf;
	uint32					stream_buf_size;

	/*output yuv data pingpong buffer*/
	JINF_YUV_CHN_T			yuv_ping_chn;
	JINF_YUV_CHN_T			yuv_pong_chn;
	uint32					yuv_buf_size;

	void					*jpeg_buf;
	uint32					jpeg_buf_size;

	/*stream read function pointer*/
	JINF_READ_FILE_FUNC		read_file_func;
	/*bytes have read*/
	uint32					read_size;	

	/*target data info*/
	JINF_YUV_CHN_T			target_yuv_info;
	uint32					target_y_size;
	uint32					target_uv_size;
	
	JINF_DATA_TYPE_E		target_data_type;
	JINF_YUV_FORMAT_E		target_sample_format;
	JINF_DATA_ENDIAN_E		target_yuv_endian;

	uint8					*target_rgb_buf;
	uint32					target_rgb_buf_size;
	JINF_RGB_FORMAT_E		target_rgb_format;

	/*yuv data write function pointer*/
	JINF_WRITE_YUV_FUNC		write_yuv_func;	
	JINF_WRITE_FILE_FUNC	write_file_func;

	/*lines have written*/
	uint32					write_lines;
	uint32					write_bytes;

	void					*target_buf;
	uint32					target_buf_size;	
    uint32                  target_buf_width;    

	JINF_ASYNC_CALLBACK		async_callback;
	
	/*jpeg basic information*/
	JINF_JPEG_TYPE_E    	file_type;
	JINF_YUV_FORMAT_E 		jpeg_sample_format;
	
	uint32					slice_height;
	uint32					decoded_slice_height;		//maybe subsuampled
	uint32					subsampled_slice_height;	//slice height after software subsample
	//uint16					*rgb_buf;

	JINF_YUV_FORMAT_E		decoded_sample_format;
	JINF_YUV_FORMAT_E		subsampled_sample_format;	//sample format after software subsample

	BOOLEAN					is_scaling_down;
	SCALE_INPUT_INFO_T		scale_input;
	SCALE_OUTPUT_INFO_T		scale_output;
	uint32					scale_input_lines;
	uint32					scale_output_lines;

	JINF_YUV_CHN_T			scale_out_yuv;				//yuv buffer for output of scaling down
	uint32					cur_buf_flag;				// 0, none, 1: ping, 2: pong, 3: both

	uint8					*rgb_temp_buf;
	uint32					rgb_temp_buf_size;

	uint32					dec_need_stream_size;
	uint32					dec_stream_size;	

	JPEG_QUALITY_E			jpeg_quality;

	JPEG_SIZE_T				src_size;			//orignal jpeg size
	JPEG_SIZE_T				src_mcu_aligned_size;//orignal mcu aligned size
	JPEG_SIZE_T				decoded_size;		//size after decoding and subsumpling, maybe contains dummy lines and columns
	JPEG_SIZE_T				scaled_size;		//size after scaling down, maybe contains dummy lines and columns
	JPEG_SIZE_T				output_size;		//actual output size without dummp lines and columns
	JPEG_SIZE_T				target_size;		//output size being expected 
	JPEG_SIZE_T				subsampled_size;	//size after subsampling
	JPEG_SIZE_T				subsampled_actual_size; // size after subsumpling without dummy lines and columns

	JPEG_SCALING_FACTOR_E	fw_subsample_factor;	//subsample factor
	uint8					*fw_decode_buf;		//decode temp buffer used by firmware
	uint32					fw_decode_buf_size;	//decode temp buffer size
	uint32					decoded_slice_num;	//number of decoded slice

	BOOLEAN					is_frame_mode;

	uint32					output_lines;				//output lines of decoder or scaler

	JINF_RECT_T				target_crop_size;			//target size of whole frame
	JINF_RECT_T				src_rect;					//source crop rect
	JINF_RECT_T				scaled_src_rect;			//source crop rect after subsample and scaled
	JPEG_SIZE_T				crop_target_size;				
	JINF_RECT_T				output_rect;				//rect actually output]

	JINF_YUV_CHN_T			subsample_yuv;				//yuv slice after softeware subsample
	uint32					soft_subsample_factor;		//software subsample factor
	uint32					soft_subsample_shift;		//software subsample shift, soft_subsample_factor = 1 << soft_subsample_factor
	uint32					decoded_lines;				//output lines output from decoder

	uint32					hard_subsample_factor;		//hardware subsample foctor
	uint32					hard_subsample_shift;		//hardware subsample shift, hard_subsample_factor = 1 << hard_subsample_shift
	BOOLEAN					is_rotated_yuv422;			//is rotate yuv422 format
	JINF_YUV_CHN_T			rotate_yuv_chn;				//buffer for rotate yuv422 format	

#if defined(JPG_HARDWARE_SCALING)
	JPEG_SCALE_PARAM_T		scale_param;
#endif	
	//BOOLEAN					is_dec_enc_switch;

#if (defined(JPG_DEC_SUPPORT_JPG_HW) || defined(JPG_ENC_SUPPORT_JPG_HW))
	JPEG_DEC_INPUT_PARA_T	fw_dec_info;	
#endif

	uint32					stream_switch_cnt;   

	JPEG_SIZE_T				scale_output_size;
	uint32					dec_stream_offset;

	JINF_YUV_PATTERN_E		target_yuv_pattern;
	uint32					scaling_mode;			//0: software scaling, 1: hardware scaling
	JINF_YUV_PATTERN_E		decoded_yuv_pattern;

	JINF_DATA_TYPE_E		scaled_data_type;
	JINF_YUV_FORMAT_E		scaled_yuv_format;
	uint32 					scaled_output_lines;
    BOOLEAN                 is_trimming;

	uint8					*convert_buf;
	uint32					convert_buf_size;	

	uint32					work_mode;				// 0: normal, 1: dec_enc_switch, 2: enc/dec alone switch

    JINF_RECT_T             img_rect;
    JINF_RECT_T             target_rect;
	uint8					*rgb_slice_buf;
	uint32					rgb_slice_buf_size; 

	JINF_RECT_T				target_yuv_rect;
	BOOLEAN					is_software_y2r;		//whether use software yuv to rgb convert
	JINF_DEC_MODE_E         img_dec_mode;
}JPEG_DEC_CONTEXT_T;

typedef struct JPEG_SCALING_CONTEXT_T_tag
{
	JINF_DATA_TYPE_E		in_data_type;
	JPEG_SIZE_T				in_size;
	JINF_RECT_T				in_rect;
	JINF_YUV_FORMAT_E		in_yuv_format;
	JINF_YUV_CHN_T			in_yuv_info;
	uint32					in_buf_size;
	
	JPEG_SIZE_T				out_size;
	JINF_DATA_TYPE_E		out_data_type;
	JINF_YUV_FORMAT_E		out_yuv_format;
	JINF_YUV_CHN_T			out_yuv_info;
	uint32					out_buf_size;

	uint8 					*temp_buf_ptr;
	uint32					temp_buf_used_size;
	uint32					temp_buf_size;
}JPEG_SCALING_CONTEXT_T;

typedef struct JPEG_SCALING_PARAM_T_tag
{
	JPEG_SIZE_T				src_size;
	JINF_RECT_T				src_crop_rect;
	JPEG_SIZE_T				target_size;
	JPEG_SIZE_T				subsampled_size;
	uint32					subsample_factor;
	uint32					scaling_factor_numerator[HW_SCALING_MAX_TIME];
	uint32					scaling_factor_denominator[HW_SCALING_MAX_TIME];
}JPEG_SCALING_PARAM_T;



typedef struct JPEG_CONTEXT_Tag
{

#if defined(PLATFORM_SC6600L)
	union
	{
		JPEG_DEC_CONTEXT_T		decode;
		JPEG_ENC_CONTEXT_T		encode;
		JPEG_SCALING_CONTEXT_T	scaling;
	}context;
#else
	JPEG_DEC_CONTEXT_T		decode;
	JPEG_ENC_CONTEXT_T		encode;
#endif	

	JPEG_STATE_E		state;					/*idle,encode,decode or get information*/
	JPEG_STATE_E		mini_state;				/*valid only when state is JPEG_STATE_CREATING_MINIATURE*/
	JPEG_PLATFORM_E		platform;
	BOOLEAN				is_software;			/*whether use software decoding?*/
}JPEG_CONTEXT_T;	


typedef void (*JPEG_CallBack)(JPEG_CONTEXT_T *pContext, uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y);

PUBLIC void JPEG_RegIntHandler(void);

/******************************************************************************
// Purpose:	convert JPEG_RET_E type to JINF_RET_E type	 		
// Author:	frank.yang
// Input:      
			inner_ret	--JPEG_RET_E type	
// Output:  	None	
// Return: 	JINF_RET_E type	
// Note:    
******************************************************************************/
PUBLIC JINF_RET_E Jpeg_RetConvertor(JPEG_RET_E inner_ret);

/******************************************************************************
// Purpose:	get jpeg context
// Author:	Eddy.Wei
// Input:      	None
// Output:  	None	
// Return:  	the pointer of the jpeg context 
// Note:    
******************************************************************************/
PUBLIC JPEG_CONTEXT_T* JPEG_GetContext(void);


/******************************************************************************
// Purpose:	get current state
// Author:	shan.he
// Input:      	None
// Output:  	None	
// Return:  	current state
// Note:    
******************************************************************************/
PUBLIC JPEG_STATE_E JPEG_GetCurrentState(void);

/******************************************************************************
// Purpose:	get context of decode
// Author:	shan.he
// Input:      	None
// Output:  	None	
// Return:  	the pointer of the decode context
// Note:    
******************************************************************************/
PUBLIC JPEG_DEC_CONTEXT_T* JPEG_GetDecodeContext(void);

/******************************************************************************
// Purpose:	get context of encode
// Author:	shan.he
// Input:      	None
// Output:  	None	
// Return:  	the pointer of the encode context
// Note:    
******************************************************************************/
PUBLIC JPEG_ENC_CONTEXT_T* JPEG_GetEncodeContext(void);


#endif


