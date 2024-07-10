#include "sci_types.h"
#include "dal_jpeg.h"
//photo editor
typedef enum
{
	PEPROCESS_IMAGE_FORMAT_RGB_565 = 0,
	PEPROCESS_IMAGE_FORMAT_RGB_888,
	PEPROCESS_IMAGE_FORMAT_RGB_666,
	PEPROCESS_IMAGE_FORMAT_GRAY,
	PEPROCESS_IMAGE_FORMAT_JPEG,
	PEPROCESS_IMAGE_FORMAT_BMP,
	PEPROCESS_IMAGE_FORMAT_MAX     
}PEPROCESS_IMAGE_FORMAT_E;

typedef enum
{
	PEPROCESS_ADJUST_BRIGHTNESS = 0, // adjust brightness
	PEPROCESS_ADJUST_CONSTRACT ,     // adjust cons tract
	PEPROCESS_COLOR_BALANCE ,        // Color Balance
	PEPROCESS_ADJUST_SATURATION ,    // adjust Saturation
	PEPROCESS_ADJUST_HUE ,           // Adjust Hue
	//PEPROCESS_GRAY ,  				 // Gray
	//PEPROCESS_BINARIZE ,             // Binarize
	PEPROCESS_EFFECT_PROCESS ,       // EffectProcess
	//PEPROCESS_NEGATIVE_RGB ,         // NegativeRGB
	//PEPROCESS_NEGATIVE_GRAY ,        // NegativeGray
 	PEPROCESS_SMOOTH ,  			 // Smooth
	//PEPROCESS_EMBOSS , 			     // Emboss
	PEPROCESS_SHARPEN , 			 // Sharpen
	PEPROCESS_IMAGE_DENOISE , 		 // ImageDenoise
	//PEPROCESS_EXPOSURE , 			 // Exposure
//	PEPROCESS_DIFFUSION , 			 // Diffusion
	PEPROCESS_MIRROR , 			     // Mirror
	PEPROCESS_FLIP , 				 // flip
	PEPROCESS_ROTATION , 			 // rotation
	PEPROCESS_PHOTOFRAME,			 // photo frame
	PEPROCESS_SCALINGDOWN,			 // scaling down
	PEPROCESS_ENCODE_JPEG ,		     // JPEG	
	PEPROCESS_ENCODE_BMP,		     // BMP	
	PEPROCESS_GROUP_COLOR_BALANCE,			 	//adjust color by group
	PEPROCESS_GROUP_BRIGHTNESS_CONTRAST, //adjust brightness and constract by group
	PEPROCESS_GROUP_SATURATION_HUE,				//adjust saturation adn hue by group
	PEPROCESS_PROCESS_MAX
}PEPROCESS_TYPE_E;

typedef struct _peprocess_brightness_param_tag
{
	int8   brightness_offset; //[-100:100]
}PEPROCESS_BRIGHTNESS_PARAM_T;

typedef struct _peprocess_contrast_param_tag
{
	uint8  gain;    //[0:128]
}PEPROCESS_CONTRAST_PARAM_T;

typedef struct _peprocess_denoise_param_tag
{
	uint8  threshold;	// (0:16)
}PEPROCESS_DENOISE_PARAM_T;

typedef struct _peprocess_saturation_param_tag
{
	uint8  gain; //[0:128]
}PEPROCESS_SATURATION_PARAM_T;

//typedef struct _peprocess_banarize_parm_tag
//{
//	uint8   threshold; //128
//}PEPROCESS_BANARIZE_PARAM_T;

typedef enum
{
	PEPROCESS_HUE_DEGREE_0 = 0,
	PEPROCESS_HUE_DEGREE_45,
	PEPROCESS_HUE_DEGREE_90,
	PEPROCESS_HUE_DEGREE_135,
	PEPROCESS_HUE_DEGREE_180,
	PEPROCESS_HUE_DEGREE_225,
	PEPROCESS_HUE_DEGREE_270,
	PEPROCESS_HUE_DEGREE_315,
	PEPROCESS_HUE_DEGREE_MAX
}PEPROCESS_HUE_DEGREE_E;

typedef struct _peprocess_hue_param_tag
{
	PEPROCESS_HUE_DEGREE_E degree;
}PEPROCESS_HUE_PARAM_T;

typedef struct _peprocess_group_color_balance_param_tag
{
	int8	cyna_to_red;			//[-100:100]
	int8	magenta_to_green;		//[-100:100]
	int8	yellow_to_blue;			//[-100:100]
}PEPROCESS_GROUP_COLOR_BALANCE_PARAM_T;

typedef struct _peprocess_group_brightness_contrast_param_tag
{
	int8	brightness;		//[-100:100]
	uint8	contrast;		//[0:128]
}PEPROCESS_GROUP_BRIGHTNESS_CONTRAST_PARAM_T;

typedef struct _peprocess_group_saturation_hue_param_tag
{
	uint8					saturation;	//[0:128]
	PEPROCESS_HUE_DEGREE_E	hue;			
}PEPROCESS_GROUP_SATURATION_HUE_PARAM_T;

typedef enum
{
	PEPROCESS_ROTATION_DEGREE_90 = 0,
	PEPROCESS_ROTATION_DEGREE_180,
	PEPROCESS_ROTATION_DEGREE_270 ,
	PEPROCESS_ROTATION_DEGREE_MAX
}PEPROCESS_ROTATION_DEGREE_E;

typedef struct _peprocess_rotation_param_tag
{
	PEPROCESS_ROTATION_DEGREE_E degree;
} PEPROCESS_ROTATION_PARAM_T;

typedef enum
{
	PEPROCESS_CYAN_TO_RED = 0,
	PEPROCESS_MAGENTA_TO_GREEN,
	PEPROCESS_YELLOW_TO_BLUE ,
	PEPROCESS_COLORB_ALANCE_MAX
}PEPROCESS_COLOR_BALANCE_E;

typedef struct _peprocess_color_balance_param_tag
{
	PEPROCESS_COLOR_BALANCE_E type;
	int8              offset;//[-100:100]
}PEPROCESS_COLOR_BALANCE_PARAM_T;

typedef enum
{
	PEPROCESS_EFFECT_SEPIA = 0,
	PEPROCESS_EFFECT_RED,
	PEPROCESS_EFFECT_GREEN,
	PEPROCESS_EFFECT_BLUE,
	PEPROCESS_EFFECT_GRAY,
	PEPROCESS_EFFECT_BINARIZE,
	PEPROCESS_EFFECT_NEGATIVE_RGB,
	PEPROCESS_EFFECT_NEGATIVE_GRAY,
	PEPROCESS_EFFECT_EMBOSS,
	PEPROCESS_EFFECT_EXPOSURE,
	PEPROCESS_EFFECT_DIFFUSION,
	PEPROCESS_EFFECT_MAX
} PEPROCESS_EFFECT_TYPE_E;

typedef struct _peprocess_effect_param_tag
{
	PEPROCESS_EFFECT_TYPE_E type;
}PEPROCESS_EFFECT_PARAM_T;


typedef struct _peprocess_sharpen_param_tag
{
	uint8  threshold;//[8:32]
	uint32 rate;     //[16:64]
}PEPROCESS_SHARPEN_PARAM_T;

typedef struct _peprocess_photoframe_parma_tag
{
	uint8 *osd_ptr;
	uint16 colorkey;
	int16 offset_x;
	int16 offset_y;
	uint16 width;
	uint16 height;
}PEPROCESS_PHOTOFRAME_PARAM_T;

typedef enum
{
	PEPROCESS_COMPRESS_LEVEL_LOW = 0,
	PEPROCESS_COMPRESS_LEVEL_MID,
	PEPROCESS_COMPRESS_LEVEL_HIGH,
	PEPROCESS_COMPRESS_LEVEL_MAX
}PEPROCESS_COMPRESS_LEVEL_E;
typedef struct _peprocess_encode_jpeg_param_tag
{	
	PEPROCESS_COMPRESS_LEVEL_E quality_level;		
}PEPROCESS_ENCODE_JPEG_PARAM_T;


typedef union _pe_process_parameter_u_tag
{
	PEPROCESS_HUE_PARAM_T            adjust_hue;	
	PEPROCESS_EFFECT_PARAM_T         effect;
	PEPROCESS_SHARPEN_PARAM_T        sharpen;
	PEPROCESS_DENOISE_PARAM_T        denoise;			
	PEPROCESS_CONTRAST_PARAM_T       contrast; 	 	
//	PEPROCESS_BANARIZE_PARAM_T       banarize;		
	PEPROCESS_ROTATION_PARAM_T       rotation;
	PEPROCESS_SATURATION_PARAM_T     saturation; 
	PEPROCESS_BRIGHTNESS_PARAM_T     brightness;
	PEPROCESS_PHOTOFRAME_PARAM_T     photoframe;
	PEPROCESS_COLOR_BALANCE_PARAM_T  color_balance;
	PEPROCESS_ENCODE_JPEG_PARAM_T    encode_to_jpeg;	
	PEPROCESS_GROUP_COLOR_BALANCE_PARAM_T 			group_color_balance;
	PEPROCESS_GROUP_BRIGHTNESS_CONTRAST_PARAM_T		group_brightness_contrast;
	PEPROCESS_GROUP_SATURATION_HUE_PARAM_T			group_saturation_hue;
} PEPROCESS_PARAMETER_U;


typedef struct _peprocess_parameter_tag
{
	uint8                  *process_buf_ptr;
    uint32                 process_buf_size;
	PEPROCESS_TYPE_E       type;	
	PEPROCESS_PARAMETER_U  process_param;	
}PEPROCESS_PARAMETER_T;


typedef struct _peprocess_input_tag
{
	uint8             *src_ptr;	
    uint8             *out_buf_ptr;
	uint32            out_buf_size;   
    uint16            input_width;
    uint16            input_height;
    uint16			  target_width;
    uint16            target_height;  
    PEPROCESS_IMAGE_FORMAT_E   in_type;
    PEPROCESS_IMAGE_FORMAT_E   out_type;  
} PEPROCESS_INPUT_T;


typedef struct _peprocess_output_tag
{		
	uint16          actual_out_width;
	uint16          actual_out_height;	
	uint32          compress_out_size;
} PEPROCESS_OUTPUT_T;

typedef enum
{
	PEPROCESS_OK = 0,
	PEPROCESS_BUFFER_SMALL_ERR,
	PEPROCESS_PROCESSTYPE_ERR,
	PEPROCESS_BRIGHTNESS_PARAM_ERR ,
	PEPROCESS_HUE_PARAM_ERR,
	PEPROCESS_EFFTCT_PARAM_ERR,
	PEPROCESS_SHARPEN_PARAM_ERR,
	PEPROCESS_DENOISE_PARAM_ERR,
	PEPROCESS_CONTRAST_PARAM_ERR,	
	PEPROCESS_ROTATION_PARAM_ERR,
	PEPROCESS_SATURATION_PARAM_ERR,
	PEPROCESS_PHOTOFRAME_PARAM_ERR,
	PEPROCESS_COLORBALANCE_PARAM_ERR,
	PEPROCESS_SCALINGDOWN_FAIL_EER,
	PEPROCESS_ENCODE_JPEG_ERR,
	PEPROCESS_SOURCE_IMAGE_SIZE_ERR,
	PEPROCESS_SOURCE_IMAGE_FORMAT_ERR,
	PEPROCESS_NO_SUPPORT
} PEPROCESS_RESULT_E;

/*****************************************************************************/
//  Description:photo editor process interface
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			input_ptr -  input information address
//			output_ptr - output information address
//			process_param_ptr - process parameter address
//		output:
//			output_ptr - out information
//		return:
//			result
/*****************************************************************************/
PEPROCESS_RESULT_E PEPROCESS_ImageProcess(
											PEPROCESS_INPUT_T  *input_ptr, 
							   				PEPROCESS_OUTPUT_T *output_ptr,
							   				PEPROCESS_PARAMETER_T *process_param_ptr
										 );


