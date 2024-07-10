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

#define WINSINC

//parameter define
#define  FLOAT_PRECSION 16
#define  SCALE_PRECSION 32
#define  MAX_MEM_SIZE   96*1024
#define NUM_READ_LINES 4

#ifdef WINSINC
#define  NUM_TAP 3
#else
#define NUM_TAP 2
#endif


#define  RGB_MODE 3
#define  GRAY_MODE 1
//parameter define

//config define
//#define DYNAMIC_GEN_FILTER
#define SUPPORT_WINDOWED_SINC
#define SUPPORT_BILINEAR
#define SUPPORT_BOX_AVERAGE
//config define

//funtion define
//#define MIN(O1,O2) ((O1) > (O2)? (O2): (O1))

#define SCI_MALLOC(_TYPE,_SIZE) ((_TYPE*)malloc(_SIZE * sizeof(_TYPE)))
//funtion define

//Global variable declare
#ifdef WIN32
FILE *infile, *outfile;
#endif
#ifdef DYNAMIC_GEN_FILTER
static int16 *Filter_Table[SCALE_PRECSION][SCALE_PRECSION];
#endif
//Global variable declare

typedef struct CX_SCALE_Struct* cx_scale_ptr;
typedef struct CX_INPUT_INFO_Struct* cx_input_info_ptr;
typedef struct CX_OUTPUT_INFO_Struct* cx_output_info_ptr;
typedef struct CX_Buffer_Struct* cx_buf_ptr;
//type define 
typedef enum SCALE_TYPE
{
	BILINEAR,
	FIXED_WIN_SINC,
	BOX_AVERAGE
} P_SCALE_TYPE;

typedef enum DATA_TYPE
{
	GRAY  = 1,
	RGB565,
	RGB888,
	RGB666,
	YuYv,
	VyUy,
	YUV422,
} P_DATA_TYPE;
//type define

typedef enum ERROR_TYPE
{
	SCALING_OK,
	SCALING_FAIL,
	INIT_OK,
	INIT_FAIL,
	UNKNOWN_DATA_TYPE,
	TRANSFORM_OK,
	TRANSFORM_FAILED,
	MEMORY_ALLOC_FAILED
} P_ERROR_TYPE;

//Struct Define
struct CX_SCALE_Struct
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
	int16 *coeff;
	int16 *filter;
	int16 *g_ptr_filter;
	uint8 input_Bytes_in_Pixel;
	uint8 output_Bytes_in_Pixel;
	uint8 output_times;
	uint8 input_comp_num;
	uint8 output_comp_num;
	uint8 scale_factor;
	uint8 *rgb_buf;
	uint8 *line_buf[3];
	cx_buf_ptr src_buf;

	uint16 read_lines;

	P_SCALE_TYPE scale_mode;//method used to scaling;
	P_DATA_TYPE in_type, out_type;
	P_ERROR_TYPE (*scaling_fun) (cx_scale_ptr s_info, uint8 *input, uint8 *output);
	void (*input_transformer) (uint8 *input, uint8 *output, int32 length);
	void (*output_transformer) (uint32 input, uint8 *output, int32 length);
	void (*Interpolation_line) (cx_scale_ptr S_Info, uint8 *input, uint8 *output);
	void (*Interpolation_column)(cx_scale_ptr S_Info, uint8 **input, uint8 *output);
	void (*read_data)(uint8* return_ptr, uint16 num_of_line);
};
/*Input para*/
struct CX_INPUT_INFO_Struct
{
	int32 target_width; //target width
	int32 target_height;//target height
	int32 input_width;//original width;
	int32 input_height;//original height;
	P_DATA_TYPE in_type;//data type;eg:rgb565,rgb888...
	//	BOOLEAN IsLargerThan_1Mega;//large file flag...
	
	uint8*  inSrc;//source point...
	uint8*  Block_Buf;
	FILE *  pFile_name;
	uint8*  g_line_buf_ptr;
	void (*read_data)(uint8* return_ptr, uint16 num_of_line);//callback function()
};

/*Output para*/
struct CX_OUTPUT_INFO_Struct
{
	int32 output_width;
	int32 output_height;
	P_DATA_TYPE out_type;
	int32 data_size;
	uint8 *OutBuf;
	
};

/*Source managerment*/
struct CX_Buffer_Struct
{
	uint8 *line_ptr;//point of the buffer;
	uint32 num_line;//numbers of line
	uint32 left_line;//lefts of line
};

//
P_ERROR_TYPE SPRD_Image_Scaling(cx_input_info_ptr Info, cx_output_info_ptr outinfo);


//Funciton define


void IMG_Generate_Filter(uint8 M, uint8 N);

int16 *IMG_GetFilter(cx_scale_ptr info);

//void CX_SCALE_Create(cx_scale_ptr S_Info);
void IMG_CX_SCALE_Destroy(cx_scale_ptr S_Info);

P_ERROR_TYPE IMG_Init_Scale_Info(cx_scale_ptr S_Info, cx_input_info_ptr Info, cx_output_info_ptr outinfo);
P_ERROR_TYPE IMG_Init_Data_Type(cx_scale_ptr S_Info);
P_ERROR_TYPE IMG_Init_COEEF_Table(cx_scale_ptr S_Info);
P_ERROR_TYPE IMG_Init_Scale_Fun(cx_scale_ptr S_Info);


P_ERROR_TYPE IMG_CX_SCALING(cx_scale_ptr s_info, uint8 *output);

//
P_ERROR_TYPE IMG_Init_WinSinc_Interpolation(cx_scale_ptr S_Info);
void S_WinSinc_Interpolation_YUV_line(cx_scale_ptr S_Info, uint8 *input, uint8 *output);
void S_WinSinc_Interpolation_YUV_column(cx_scale_ptr S_Info, uint8 **input, uint8 *output);
void S_WinSinc_Interpolation_VYUY_line(cx_scale_ptr S_Info, uint8 *input, uint8 *output);

//
void IMG_S_Caculate_M_N(uint32 M1, uint32 N1, uint8* M, uint8* N, uint8 level);
void *IMG_Get_Data_Src(cx_scale_ptr S_Info, int16 step);
//extern
extern const int16 PE_FIXED_3TAP_SINC_COEFF_1_X[16][NUM_TAP];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_2_X[16][NUM_TAP * 2];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_3_X[16][NUM_TAP * 3];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_4_X[16][NUM_TAP * 4];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_5_X[16][NUM_TAP * 5];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_6_X[16][NUM_TAP * 6];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_7_X[16][NUM_TAP * 7];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_8_X[16][NUM_TAP * 8];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_9_X[16][NUM_TAP * 9];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_10_X[16][NUM_TAP * 10];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_11_X[16][NUM_TAP * 11];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_12_X[16][NUM_TAP * 12];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_13_X[16][NUM_TAP * 13];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_14_X[16][NUM_TAP * 14];
extern const int16 PE_FIXED_3TAP_SINC_COEFF_15_X[16][NUM_TAP * 15];

extern const int32  PE_NN[16];
extern const uint8  PE_DD[79][2];
extern const uint32  PE_SS[79];
#define FIX_POINT_INT(X,Y)     ((((X)*PE_NN[(Y)])+4095) >> 16) // only used in this condition. 
//Funciton define


#endif