#ifndef _SCALING_DOWN_API_H_
#define _SCALING_DOWN_API_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sci_types.h"

//type define 
typedef enum DATA_TYPE_adv
{
	DATA_TYPE_GRAY_ADV  = 1,
	DATA_TYPE_RGB565_ADV,
	DATA_TYPE_RGB888_ADV,
	DATA_TYPE_RGBX888_ADV,
	DATA_TYPE_RGB666_ADV,
	DATA_TYPE_YUYV_ADV,
	DATA_TYPE_YUV422_ADV,
	DATA_TYPE_YUV420_ADV
	
} P_DATA_TYPE_ADV_E;

typedef enum SCALING_MODE_adv
{
	MODE_SMALLER_THAN_EXPECT = 0,			//size of output image must be smaller than expect size
	MODE_BIGGER_THAN_EXPECT					//size of output image must be bigger than expect size
}P_SCALING_MODE_ADV_E;

typedef enum ERROR_TYPE_adv
{
	SCALING_OK_ADV,
	SCALING_FAIL_ADV,
	INIT_OK_ADV,
	INIT_FAIL_ADV,
	UNKNOWN_DATA_TYPE_ADV,
	TRANSFORM_OK_ADV,
	TRANSFORM_FAILED_ADV,
	MEMORY_ALLOC_FAILED_ADV,
	UNSUPPORT_SCALING_FACTOR
} P_ERROR_TYPE_ADV;

typedef struct data_format_tag
{
	uint8 *plane_a;
	uint8 *plane_b;
	uint8 *plane_c;
	uint32 num_plane;
} DATA_FORMAT_T;
/*Input para*/
typedef struct scale_input_info_tag
{
	int32 target_width; //target width
	int32 target_height;//target height
	int32 input_width;//original width;
	int32 input_height;//original height;

	int32 input_ext_width;		//extended width, dummy columns included
	int32 input_ext_height;		//extended height, dummy lines included
	
	uint8*  g_line_buf_ptr;//line buffer point
	uint8*  scale_mem_ptr;//memory used in scaling
	uint32  scale_mem_size;//memory size used in scaling
	int32 slice_number;//total number of slice
	P_DATA_TYPE_ADV_E in_type;//data type;eg:rgb565,rgb888...
	P_SCALING_MODE_ADV_E in_mode;	//scaling mode
} SCALE_INPUT_INFO_T;

/*Output para*/
typedef struct scale_output_info_tag
{
	uint32 factor_numerator;	//numerator of scaling factor
	uint32 factor_denominator;	//denominator of scaling factor
	int32 output_width;//output width
	int32 output_height;//output height
	P_DATA_TYPE_ADV_E out_type;//data type;eg:rgb565,rgb888...
} SCALE_OUTPUT_INFO_T;

typedef struct scale_size_tag
{
	int16 w;
	int16 h;
}SCALE_SIZE_T;


//Funciton define

PUBLIC P_ERROR_TYPE_ADV Scale_Initialize(SCALE_INPUT_INFO_T *inputinfo, 
										 SCALE_OUTPUT_INFO_T *outinfo);
PUBLIC P_ERROR_TYPE_ADV Scale_one_slice(DATA_FORMAT_T *inbuf, 
										DATA_FORMAT_T *output_buf, 
										uint16 *slice_width,
										uint16 *slice_height_luma,
										//int32 *slice_height_chroma,
										uint32 num_lines);
PUBLIC void Scale_Get_WidthHeight(SCALE_OUTPUT_INFO_T *outinfo);

/************************************************************************
** Purpose  : get output scaling info according to the source size and target size									
** Name     : Scale_GetScalingInfo()
** Parameter:
** Author   : shan.he
*************************************************************************/
PUBLIC P_ERROR_TYPE_ADV Scale_GetScalingInfo(SCALE_INPUT_INFO_T *inputinfo, 
								  				 SCALE_OUTPUT_INFO_T *outinfo);

PUBLIC P_ERROR_TYPE_ADV Scale_SetSliceNumber(uint32 slice_number);


#endif