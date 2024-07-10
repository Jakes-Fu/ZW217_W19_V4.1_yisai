/************************************************************************/
/* Author:wangyi
   Date:2007.08.11
   This file is written for scaling down the source image...
                                                                        */
/************************************************************************/

#ifndef _SCALING_H
#define _SCALING_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sci_types.h"
//#include "math.h"
#include "os_api.h"

#define WINSINC_CUS

//parameter define
#define  FLOAT_PRECSION_CUS 16
#define  SCALE_PRECSION_CUS 32
#define  MAX_MEM_SIZE_CUS   96*1024
#ifdef WINSINC_CUS
#define  NUM_TAP_CUS 3
#else
#define NUM_TAP_CUS 2
#endif

#define  RGB_4BYTE_CUS 4
#define  RGB_MODE_CUS 3
#define  GRAY_MODE_CUS 1
//parameter define

//config define
//#define DYNAMIC_GEN_FILTER
#define SUPPORT_WINDOWED_SINC_CUS
#define SUPPORT_BILINEAR_CUS
#define SUPPORT_BOX_AVERAGE_CUS
//config define
#define BASE_COFF  16
//funtion define
//#define MIN(O1,O2) ((O1) > (O2)? (O2): (O1))

#define SCI_MALLOC_CUS(_TYPE,_SIZE) ((_TYPE*)SCI_ALLOC_APP(_SIZE * sizeof(_TYPE)))
//funtion define

//Global variable declare
#ifdef WIN32
FILE *infile, *outfile;
#endif
#ifdef DYNAMIC_GEN_FILTER
static int16 *Filter_Table_cus[SCALE_PRECSION_CUS][SCALE_PRECSION_CUS];
#endif
//Global variable declare

typedef struct CX_SCALE_Struct_cus* cx_scale_ptr_cus;
typedef struct CX_INPUT_INFO_Struct_cus* cx_input_info_ptr_cus;
typedef struct CX_OUTPUT_INFO_Struct_cus* cx_output_info_ptr_cus;
typedef struct CX_Buffer_Struct_cus* cx_buf_ptr_cus;
typedef struct CX_Memory_FunInfo_cus* cx_memory_funifo_ptr_cus;

struct CX_CTRL_Struct_cus
{
	int16 cur_pos;
	int16 last_pos;
	int16 u;
	int16 v;
};
//type define 
typedef enum SCALE_TYPE_cus
{
	BILINEAR_CUS,
	FIXED_WIN_SINC_CUS,
	BOX_AVERAGE_CUS
} P_SCALE_TYPE_CUS;

typedef enum DATA_TYPE_cus

{
	GRAY_CUS  = 1,
	RGB565_CUS,
	RGB888_CUS,
	RGBX888_CUS,
	RGB666_CUS,
	YuYv_CUS,
	YUV422_CUS,
} P_DATA_TYPE_CUS;
//type define

typedef enum ERROR_TYPE_cus
{
	SCALING_OK_CUS,
	SCALING_FAIL_CUS,
	INIT_OK_CUS,
	INIT_FAIL_CUS,
	UNKNOWN_DATA_TYPE_CUS,
	TRANSFORM_OK_CUS,
	TRANSFORM_FAILED_CUS,
	MEMORY_ALLOC_FAILED_CUS
} P_ERROR_TYPE_CUS;

//Struct Define
struct CX_SCALE_Struct_cus
{
	int32 M;
	int32 N;
	int32 input_width;
	int32 input_height;
	int32 M_input_width;
	int32 M_input_height;
	int32 output_width;
	int32 output_height;
	int32 act_out_w;
	int32 act_out_h;
	uint8 *coeff;
	uint8 *filter;
	uint8 *g_ptr_filter;
	uint8 input_Bytes_in_Pixel;
	uint8 output_Bytes_in_Pixel;
	uint8 output_times;
	uint8 input_comp_num;
	uint8 output_comp_num;
	uint8 scale_factor;
	uint8 *input_src;
	uint8 *rgb_buf;
	uint8 **line_buf;
	cx_buf_ptr_cus src_buf;
	struct CX_CTRL_Struct_cus line_ctrl;
	//
	uint16 read_lines;
	//
	P_SCALE_TYPE_CUS scale_mode;//method used to scaling;
	P_DATA_TYPE_CUS in_type, out_type;
	P_ERROR_TYPE_CUS (*scaling_fun) (cx_scale_ptr_cus s_info, uint8 *input, uint8 *output);
	void (*input_transformer) (uint8 *input, uint8 *output, int32 length);
	void (*output_transformer) (uint32 input, uint8 *output, int32 length);
	void (*Interpolation_line) (cx_scale_ptr_cus S_Info, uint8 *input, uint8 *output);
	void (*Interpolation_column)(cx_scale_ptr_cus S_Info, uint8 **input, uint8 *output);
	void (*read_data)(uint8* return_ptr, uint16 num_of_line, uint16* return_lines);
	void *(*img_alloc)(uint32 size);
	void (*img_free)(void *);
};
/*Input para*/
struct CX_INPUT_INFO_Struct_cus
{
	int32 target_width; //target width
	int32 target_height;//target height
	int32 input_width;//original width;
	int32 input_height;//original height;
	P_DATA_TYPE_CUS in_type;//data type;eg:rgb565,rgb888...
//	BOOLEAN IsLargerThan_1Mega;//large file flag...
	
	uint8*  inSrc;//source point...
	uint8*  Block_Buf;
	FILE *  pFile_name;
	uint8*  g_line_buf_ptr;
	void (*read_data)(uint8* return_ptr, uint16 num_of_line, uint16* return_lines);//callback function()
	void *(*img_alloc)(uint32 size);
	void (*img_free)(void *);
};

/*Output para*/
struct CX_OUTPUT_INFO_Struct_cus
{
	int32 output_width;
	int32 output_height;
	P_DATA_TYPE_CUS out_type;
	int32 data_size;
	uint8 *OutBuf;
	
};
/*Source managerment*/
struct CX_Buffer_Struct_cus
{
	uint8 *line_ptr;//point of the buffer;
	uint32 num_line;//numbers of line
	uint32 left_line;//lefts of line
};
struct CX_Memory_FunInfo_cus
{
	void *(*img_alloc)(uint32 size);
	void (*img_free)(void *);
};

//
P_ERROR_TYPE_CUS SPRD_IMG_Scaling_cus(cx_input_info_ptr_cus Info, cx_output_info_ptr_cus outinfo);


//Funciton define


void Generate_Filter_cus(uint8 M, uint8 N);

uint8 *GetFilter_cus(cx_scale_ptr_cus info);

void CX_SCALE_Create_cus(cx_scale_ptr_cus S_Info);
//void CX_SCALE_Destroy_cus(cx_scale_ptr_cus S_Info);
void CX_SCALE_Destroy_cus(cx_memory_funifo_ptr_cus info);

P_ERROR_TYPE_CUS Init_Scale_Info_cus(cx_scale_ptr_cus S_Info, cx_input_info_ptr_cus Info, cx_output_info_ptr_cus outinfo);
P_ERROR_TYPE_CUS Init_Data_Type_cus(cx_scale_ptr_cus S_Info);
P_ERROR_TYPE_CUS Init_COEEF_Table_cus(cx_scale_ptr_cus S_Info);
P_ERROR_TYPE_CUS Init_Scale_Fun_cus(cx_scale_ptr_cus S_Info);


//P_ERROR_TYPE_CUS CX_SCALING_cus(cx_scale_ptr_cus s_info,uint8 *output);
P_ERROR_TYPE_CUS CX_SCALING_CUS(cx_scale_ptr_cus s_info, uint8 *output);


//
void RGB565TO888_cus(uint8 *input, uint8 * output, int32 length);
void RGB888TO565_cus(uint32 input, uint8 * output, int32 length);
void SIMPLE_COPY_cus(uint32 input, uint8 * output, int32 length);
void YuYvTOYUV422_cus(uint32 input, uint8 *output, int32 length);
//
P_ERROR_TYPE_CUS Init_WinSinc_Interpolation_cus(cx_scale_ptr_cus S_Info);
void S_WinSinc_Interpolation_YUV_line_cus(cx_scale_ptr_cus S_Info, uint8 *input, uint8 *output);
void S_WinSinc_Interpolation_YUV_column_cus(cx_scale_ptr_cus S_Info, uint8 **input, uint8 *output);
void S_WinSinc_Interpolation_Gray_line_cus(cx_scale_ptr_cus S_Info, uint8 *input, uint8 *output);
void S_WinSinc_Interpolation_Gray_column_cus(cx_scale_ptr_cus S_Info, uint8 **input, uint8 *output);
void S_WinSinc_Interpolation_Multi_Comp_line_cus(cx_scale_ptr_cus S_Info, uint8 *input, uint8 *output);
void S_WinSinc_Interpolation_Multi_Comp_column_cus(cx_scale_ptr_cus S_Info, uint8 **input, uint8 *output);

// for RGBX888_CUS (ARGB8888)
void S_WinSinc_Interpolation_Multi_Comp_line_cus_ex(cx_scale_ptr_cus S_Info, uint8 *input, uint8 *output);
void S_WinSinc_Interpolation_Multi_Comp_column_cus_ex(cx_scale_ptr_cus S_Info, uint8 **input, uint8 *output);

//
void S_Caculate_M_N_cus(cx_scale_ptr_cus S_Info, uint32 M1, uint32 N1, uint16* M, uint16* N, uint8 level);

//extern
extern const uint8 FIXED_3TAP_SINC_COEFF_1_X_cus[15][NUM_TAP_CUS];
extern const uint8  FIXED_3TAP_SINC_COEFF_2_X_cus[13][NUM_TAP_CUS * 2];
extern const uint8  FIXED_3TAP_SINC_COEFF_3_X_cus[13][NUM_TAP_CUS * 3];
extern const uint8  FIXED_3TAP_SINC_COEFF_4_X_cus[11][NUM_TAP_CUS * 4];
extern const uint8  FIXED_3TAP_SINC_COEFF_5_X_cus[11][NUM_TAP_CUS * 5];
extern const uint8  FIXED_3TAP_SINC_COEFF_6_X_cus[9][NUM_TAP_CUS * 6];
extern const uint8  FIXED_3TAP_SINC_COEFF_7_X_cus[9][NUM_TAP_CUS * 7];
extern const uint8  FIXED_3TAP_SINC_COEFF_8_X_cus[7][NUM_TAP_CUS * 8];
extern const uint8  FIXED_3TAP_SINC_COEFF_9_X_cus[7][NUM_TAP_CUS * 9];
extern const uint8  FIXED_3TAP_SINC_COEFF_10_X_cus[3][NUM_TAP_CUS * 10];
extern const uint8  FIXED_3TAP_SINC_COEFF_11_X_cus[5][NUM_TAP_CUS * 11];
extern const uint8  FIXED_3TAP_SINC_COEFF_12_X_cus[3][NUM_TAP_CUS * 12];
extern const uint8  FIXED_3TAP_SINC_COEFF_13_X_cus[3][NUM_TAP_CUS * 13];
extern const uint8  FIXED_3TAP_SINC_COEFF_14_X_cus[1][NUM_TAP_CUS * 14];
extern const uint8  FIXED_3TAP_SINC_COEFF_15_X_cus[1][NUM_TAP_CUS * 15];
extern const uint8  FIXED_3TAP_SINC_COEFF_X_X_cus[1][NUM_TAP_CUS];

extern const uint32  NN_cus[16];
extern const uint8   DD_cus[79][2];
extern const uint32  SS_cus[79];
#define FIX_POINT_INT_cus(X,Y)     ((((X)*NN_cus[(Y)])+4095) >> 16) // only used in this condition. 
//Funciton define


#endif

