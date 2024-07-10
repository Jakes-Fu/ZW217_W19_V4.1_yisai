/************************************************************************/
/* Author:wangyi
   Date:2007.08.11
   This file is written for scaling down the source image...
                                                                        */
/************************************************************************/

#ifndef _SCALING_DOWN_ADVANCE_H_
#define _SCALING_DOWN_ADVANCE_H_

#include "jpeg_scaling_api.h"
#include "scaling_down_cus.h"
//parameter define

#define  NUM_TAP_ADV 3

#define  RGB_4BYTE_ADV 4
#define  RGB_MODE_ADV  3
#define  GRAY_MODE_ADV 1
#define  Y_UV_MODE_ADV 2

#define CX_PRINT   SCI_TRACE_LOW 

//config define
#define BASE_COFF  16

//#define SCI_MALLOC_ADV(_TYPE,_SIZE) ((_TYPE*)SCI_ALLOC(_SIZE * sizeof(_TYPE)))

//funtion define

//Global variable declare
#if defined(SMALL_SYS) && defined(WIN32)

FILE *infile, *outfile;
#define PRINTF printf
#define SCI_ASSERT assert
#define SCI_MEMCPY memcpy

#endif

#if defined(SMALL_SYS) && defined(_ARM_)
#define PRINTF
#define SCI_ASSERT
#define SCI_MEMCPY memcpy
#endif

typedef struct slice_scale_ctrl_tag
{
	int32		num_ip_rows;	//number of rows which have been interpolated
	int16		last_pos;		//
	int16		slice_index;	//current id of slice which will be scaled

	int16		slice_number;	//total slice number
	int8		filter_num;		//id of filter which will be used for interpolation
//	BOOLEAN		extra_flag;		//flag for judgy that need interpolated lines in the preview slice scaling
	
	int8		copy_line_num;		//
	uint8		*copy_line_ptr;		//interpolated lines required by next slice scaling
	uint8		*filter_end_ptr;		//indicate the end of the filter, add by shan.he
	int32		slice_pos;
} SLICE_SCALE_CTRL_T;



//Struct Define
typedef struct scale_info_tag
{
	int32 input_width;
	int32 input_height;

	int32 input_ext_width;	//input extended width, dummy columns included
	int32 input_ext_height;	//input extended height, dummy lines included

	int32 output_width;
	int32 output_height;

	int32 act_out_w;
	int32 act_out_h;

	uint8 input_Bytes_in_Pixel;
	uint8 output_Bytes_in_Pixel;
	uint8 output_times;
	uint8 input_comp_num;

	int8 M;
	int8 N;
	uint8 output_comp_num;
	uint8 scale_factor;
	
	uint8 *coeff;
	uint8 *filter;
	uint8 *g_ptr_filter;

	uint8 *uv_buf;
	uint8 *rgb_buf;					//rgb buffer which will be used for data transformer:eg,rgb565->rgb888
	uint8 *line_buf[NUM_TAP_ADV];	//line buffer
	SLICE_SCALE_CTRL_T slice_ctrl;	//control infomation
	DATA_FORMAT_T input_data_ptr;

	P_DATA_TYPE_ADV_E in_type;
	P_DATA_TYPE_ADV_E out_type;

	P_ERROR_TYPE_ADV (*scaling_fun) (struct scale_info_tag *s_info, uint8 *input, uint8 *output);
	void (*input_transformer) (uint8 *input, uint8 *output, int32 length);
	void (*output_transformer) (uint32 input, uint8 *output, int32 length);
	P_ERROR_TYPE_ADV (*Interpolation_line) (struct scale_info_tag *S_Info, DATA_FORMAT_T *input, uint8 *output);
	P_ERROR_TYPE_ADV (*Interpolation_column)(struct scale_info_tag *S_Info, uint8 **input, DATA_FORMAT_T *output);

} SCALE_INFO_T;


#define FIXED_3TAP_SINC_COEFF_1_X_adv		FIXED_3TAP_SINC_COEFF_1_X_cus
#define FIXED_3TAP_SINC_COEFF_2_X_adv		FIXED_3TAP_SINC_COEFF_2_X_cus
#define FIXED_3TAP_SINC_COEFF_3_X_adv		FIXED_3TAP_SINC_COEFF_3_X_cus
#define FIXED_3TAP_SINC_COEFF_4_X_adv		FIXED_3TAP_SINC_COEFF_4_X_cus
#define FIXED_3TAP_SINC_COEFF_5_X_adv		FIXED_3TAP_SINC_COEFF_5_X_cus
#define FIXED_3TAP_SINC_COEFF_6_X_adv		FIXED_3TAP_SINC_COEFF_6_X_cus
#define FIXED_3TAP_SINC_COEFF_7_X_adv		FIXED_3TAP_SINC_COEFF_7_X_cus
#define FIXED_3TAP_SINC_COEFF_8_X_adv		FIXED_3TAP_SINC_COEFF_8_X_cus
#define FIXED_3TAP_SINC_COEFF_9_X_adv		FIXED_3TAP_SINC_COEFF_9_X_cus
#define FIXED_3TAP_SINC_COEFF_10_X_adv		FIXED_3TAP_SINC_COEFF_10_X_cus
#define FIXED_3TAP_SINC_COEFF_11_X_adv		FIXED_3TAP_SINC_COEFF_11_X_cus
#define FIXED_3TAP_SINC_COEFF_12_X_adv		FIXED_3TAP_SINC_COEFF_12_X_cus
#define FIXED_3TAP_SINC_COEFF_13_X_adv		FIXED_3TAP_SINC_COEFF_13_X_cus
#define FIXED_3TAP_SINC_COEFF_14_X_adv		FIXED_3TAP_SINC_COEFF_14_X_cus
#define FIXED_3TAP_SINC_COEFF_15_X_adv		FIXED_3TAP_SINC_COEFF_15_X_cus

#define FIXED_3TAP_SINC_COEFF_X_X_adv		FIXED_3TAP_SINC_COEFF_X_X_cus

//extern SCALE_INFO_T *g_ScaleInfo;
extern uint8 *gScale_current_buf_ptr;
extern uint8 *gScale_buf_ptr;
extern BOOLEAN gScale_ctrl_flag0;
//extern uint32 g_slice_scaling_height_chroma;

#define DD_adv 				DD_cus
#define SS_adv 				SS_cus
#define NN_adv				NN_cus

#define FIX_POINT_INT_adv			FIX_POINT_INT_cus

P_ERROR_TYPE_ADV S_WinSinc_Interpolation_Y_UV_line_fast_for_uvuv(SCALE_INFO_T* S_Info,
															DATA_FORMAT_T *input, 
															uint8 *output);

P_ERROR_TYPE_ADV S_WinSinc_Interpolation_Y_UV_line_fast_adv(SCALE_INFO_T* S_Info,
													   DATA_FORMAT_T *input, 
													   uint8 *output);
P_ERROR_TYPE_ADV S_WinSinc_Interpolation_YUYV_column_adv(SCALE_INFO_T* S_Info, 
													uint8 **input,
													DATA_FORMAT_T *output);
P_ERROR_TYPE_ADV S_WinSinc_Interpolation_Gray_line_adv(SCALE_INFO_T* S_Info, 
												 DATA_FORMAT_T *input, 
												 uint8 *output);
P_ERROR_TYPE_ADV S_WinSinc_Interpolation_Gray_column_adv(SCALE_INFO_T* S_Info, 
												   uint8 **input, 
												   DATA_FORMAT_T *output);

SCALE_INFO_T *Get_Scale_Info(void);

#endif
