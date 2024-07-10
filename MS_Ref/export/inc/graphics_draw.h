/******************************************************************************
 ** File Name:      graphics_draw.h											  *
 ** Author:         Shan.He                                                   *
 ** DATE:           2010-8-25                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    functions to draw some graphics                            *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2010-8-25         Shan.He         Create                                  *
 *****************************************************************************/
#ifndef __GRAPHICS_DRAW_H__
#define __GRAPHICS_DRAW_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "img_ref_types.h"
#include "img_transform.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macros		                                      *
 **---------------------------------------------------------------------------*/
#define GRAPH_ERR_SIZE_UNSUPPORT 				IMGPROC_ERR_SIZE_UNSUPPORT	
#define GRAPH_ERR_FORMAT_UNSUPPORT				IMGPROC_ERR_FORMAT_UNSUPPORT
#define GRAPH_ERR_POINTER_INVALID				IMGPROC_ERR_POINTER_INVALID				
#define GRAPH_ERR_BUFFER_INVALID				IMGPROC_ERR_BUFFER_INVALID			
#define GRAPH_ERR_SCALE_OUT_OF_RANGE			IMGPROC_ERR_SCALE_OUT_OF_RANGE			
#define GRAPH_ERR_MEMORY_NOT_ENOUGH				IMGPROC_ERR_MEMORY_NOT_ENOUGH		
#define GRAPH_ERR_HARDWARE						IMGPROC_ERR_HARDWARE			
#define GRAPH_ERR_UNKNOWN						IMGPROC_ERR_UNKNOWN			
#define GRAPH_ERR_SCALE_UNSUPPORT				IMGPROC_ERR_SCALE_UNSUPPORT
#define GRAPH_ERR_ALLOC_MEMORY_FAILED			IMGPROC_ERR_ALLOC_MEMORY_FAILED
	
/**---------------------------------------------------------------------------*
 **                         Data Types                                        *
 **---------------------------------------------------------------------------*/
typedef uint32 	GRAPH_COLOR_T;
typedef int32  	GRAPH_FIXED_T;					//16.16 fixed point

typedef enum
{
	RECT_STYLE_NORMAL = 0,
	RECT_STYLE_SHADOW
}RECT_STYLE_E;

typedef enum
{
	RECT_ALPHA_BACKGROUND = 0,				//use the background alpha after draw a round rectangle on the background
	RECT_ALPHA_FOREGROUND,					//use the foreground alpha after draw a round rectangle on the background
	RECT_ALPHA_MIXED                        //use the mixture of foreground alpha and background alpha
}RECT_ALPHA_OPTION_E;

typedef enum
{
	SCALE_MODE_NO_DISTORT = 0,					//automatically adjust the horizontal and vertical scale ratio to the same one
												//and the output image will not be distort. The output image size maybe
												//smaller than the expected size
	SCALE_MODE_DISTORT							//do not adjust the horizontal and vertical scale ratio and the output image maybe distort
												//the scale ratio of both horizontal and vertical direction must range from 
												// 1/4 to 4 if a SCALE_MODE_DISTORT mode scale is expected
}SCALE_MODE_E;

typedef enum
{
	ROTATE_ARBITRARY_AUTO_CROP = 0,				//automatically crop the rotated image acoording to the target image size
	ROTATE_ARBITRARY_NO_CROP					//always output the full rotated image, which demands the enougth target buffer.
}ROTATE_ARBITRARY_MODE_E;

typedef enum
{
	CLOCK_HOUR_HANDLE = 0,
	CLOCK_MINUTE_HANDLE,
	CLOCK_SECOND_HANDLE
}CLOCK_HANDLE_TYPE_E;

typedef struct 
{
    const void				*data_ptr;      
    uint32					data_size;
    IMGREF_DATA_FORMAT_E    data_type;      
    uint16					width;             
    uint16					height;
    int16					center_x;       //horizontal coordinate of rotation center 
    int16					center_y;       //vertical coordinate of rotation center 
}CLOCK_IMAGE_T;

typedef struct 
{
    uint8      hour;
    uint8      minute;
    uint8      second;
	uint8	   padding;
}CLOCK_TIME_T;

typedef struct 
{
    CLOCK_IMAGE_T   clock_dial;			//dial image
    CLOCK_IMAGE_T   hour_handle;		//hour handle image
    CLOCK_IMAGE_T   min_handle;			//minitue handle image
    CLOCK_IMAGE_T   sec_handle;			//second handle image
    CLOCK_TIME_T    time;				//current time
    void            *target_buf_ptr;	//target buffer
    uint32          target_buf_size;	//target buffer size
}CLOCK_DRAW_IN_T;

typedef struct 
{
    void					*output_ptr;
    uint32					output_size;
    IMGREF_DATA_FORMAT_E    type;
    uint16					width;
    uint16					height;    
}CLOCK_DRAW_OUT_T;

typedef struct 
{
    CLOCK_IMAGE_T   		clock_dial;			//dial image
    CLOCK_IMAGE_T   		handle;				//handle image
	CLOCK_HANDLE_TYPE_E		handle_type;		//type of handle
    CLOCK_TIME_T    		time;				//current time	
	IMGREF_POINT_T			point;				//point to test on the dial coordinate system
}CLOCK_POINT_TEST_IN_T;


typedef struct
{
	BOOLEAN					result;				//TRUE: point on the handle. FALSE: otherwise
}CLOCK_POINT_TEST_OUT_T;


typedef struct
{
	IMGREF_SIZE_T			output_size;		//output image size, maybe smaller than the target size	
	IMGREF_DATA_FORMAT_E	output_format;		//output format
	IMGREF_CHN_T			output_chn;			//output buffer.Y data in chn0 and UV data in chn1. RGB data in chn0
}SCALE_IMAGE_OUT_T;

typedef struct
{
	IMGREF_SIZE_T			src_size;			//soure image
	IMGREF_RECT_T			src_trim_rect;		//trim rect of source image
	IMGREF_DATA_FORMAT_E	src_format;			//soure image format. ARGB888/ARGB565/RGB565/RGB888/YUV422/YUV420/YUV00
	IMGREF_CHN_T			src_chn;			//Y data in chn0 and UV data in chn1. RGB data in chn0
	SCALE_MODE_E			scale_mode;			//scale mode
		
	IMGREF_SIZE_T			target_size;		//target size
	IMGREF_DATA_FORMAT_E	target_format;		//target format, ARGB888/ARGB565/RGB565/YUV422
	IMGREF_BUF_T			target_buf;			//target buffer, must be at lest (target_image_bytes + 300) if the target format is RGB
												//or (target_image_bytes + 600) if the target format is YUV422

    BOOLEAN 				is_isp_open;
	void (*async_callback)(uint32 ret_value, SCALE_IMAGE_OUT_T *out_param_ptr, uint32 async_param);	
	uint32					async_param;				

    BOOLEAN                           scale_by_software;      // TURE: scale by software
}SCALE_IMAGE_IN_T;


typedef struct
{
	IMGREF_RANGE_T			scale_down_range;		//range of scale down, [1/range.max, 1/range.min]
	IMGREF_RANGE_T			scale_up_range;			//range of scale up, [range.min, range.max]
	BOOLEAN					is_idle;				//is the hardware is idle?	
}SCALE_IMAGE_CAP_T;

typedef struct
{
	IMGREF_SIZE_T			src_size;				//soure size. YUV422/YUV420/YUV400: width and hegiht must be 4-pixel-aligned.
													// RGB565: width and height must be 2-pixel-aligned.
	IMGREF_DATA_FORMAT_E	src_format;				//YUV422/YUV420/YUV400/RGB565/RGB888/ARGB888
	IMGREF_CHN_T			src_chn;				//soure image buffer, Y data in chn0, UV data in chn1. RGB565/RGB888/ARGB888 data in chn0
													//all the channel address must be 256-bytes-aligned
	IMGREF_ROTATION_MODE_E	rotate_mode;			//rotation mode. 
	
	IMGREF_BUF_T			target_buf;				//output buffer, must be at lest (image_bytes + 300) bytes
}ROTATE_IMAGE_IN_T;

typedef struct
{	
	IMGREF_SIZE_T			output_size;			//output image size
	IMGREF_DATA_FORMAT_E	output_format;			//output image format
	IMGREF_CHN_T			output_chn;				//output buffer. Y data in chn0 and UV data in chn1. RGB data in chn0
}ROTATE_IMAGE_OUT_T;

typedef struct
{
	IMGREF_SIZE_T			bk_size;		//can be ignored if the bk_buf.ptr is NULL
	IMGREF_BUF_T			bk_buf;			//set to NULL if use the target_buf
	IMGREF_DATA_FORMAT_E	bk_format;		//ARGB888 or RGB565. can be ignored if the bk_buf.ptr is NULL

	IMGREF_RECT_T			rect;			// the size of the rect to draw must be not smaller than 10X12
	RECT_STYLE_E			rect_style;		// draw style
	GRAPH_COLOR_T			color;			//fill color or frame of rect color
	uint32					line_width;		//reserved. ignore

	IMGREF_RECT_T			visible_rect;	//in which the rect will be drawn or filled.
	RECT_ALPHA_OPTION_E		alpha_option;		
	IMGREF_BUF_T			target_buf;		//can be ignored if the bk_buf.ptr is not NULL
}DRAW_RRECT_IN_T;

typedef struct
{
	IMGREF_SIZE_T			output_size;	//output buffer size		
	IMGREF_BUF_T			output_buf;		//output buffer
	IMGREF_DATA_FORMAT_E	output_format;	//output format
	IMGREF_RECT_T			output_rect;	//output rect
}DRAW_RRECT_OUT_T;

typedef struct
{
	IMGREF_SIZE_T			src_size;				//source image size
	IMGREF_DATA_FORMAT_E	src_format;				//source image format, both RGB565 and ARGB888 are allowed
	IMGREF_CHN_T			src_chn;				//source image data buffer
	IMGREF_POINT_T			src_center;				//rotation center point of the source image

	IMGREF_SIZE_T			target_size;			//target image size
	IMGREF_DATA_FORMAT_E	target_format;			//target image format, be the same as that of the source image
	IMGREF_CHN_T			target_chn;				//target image data buffer
	IMGREF_POINT_T			target_center;			//rotation center point of the target image	
	
	int32					angle;					//rotation angle

	ROTATE_ARBITRARY_MODE_E	mode;					//rotated mode: auto crop or no crop of rotated image

	BOOLEAN					speed_up;				//set TRUE to use quicker alogrithm. The source image will be overwritten.
}ROTATE_ARBITRARY_IN_T;

typedef struct
{	
	IMGREF_POINT_T			output_offset;			//offset of the output image when blending it to the background image
	IMGREF_SIZE_T			output_size;			//output image size
	IMGREF_DATA_FORMAT_E	output_format;			//output image format
	IMGREF_CHN_T			output_chn;				//output image buffer
}ROTATE_ARBITRARY_OUT_T;

typedef struct
{
	IMGREF_SIZE_T			src_size;				//source image size
	IMGREF_DATA_FORMAT_E	src_format;				//source image format, both RGB565 and ARGB888 are allowed
	IMGREF_CHN_T			src_chn;				//source image data buffer
	IMGREF_POINT_T			src_center;				//rotation center point of the source image

	IMGREF_SIZE_T			background_size;			//background image size
	IMGREF_DATA_FORMAT_E	background_format;		//background image format, be the same as that of the source image
	IMGREF_CHN_T			background_chn;			//background image data buffer
	IMGREF_POINT_T			background_center;		//rotation center point 	
	
	int32					angle;					//rotation angle

	ROTATE_ARBITRARY_MODE_E	mode;					//rotated mode: auto crop or no crop of rotated image
}ROTATE_ARBITRARY_BLEND_IN_T;

typedef struct
{	
	IMGREF_POINT_T			output_offset;			//offset of the output image when blending it to the background image
	IMGREF_SIZE_T			output_size;			//output image size
	IMGREF_DATA_FORMAT_E	output_format;			//output image format
	IMGREF_CHN_T			output_chn;				//output image buffer
}ROTATE_ARBITRARY_BLEND_OUT_T;

////////////////////////
//smooth and scale
typedef enum
{
    //IMGREF_SMOOTH_MEAN = 0x01,
    IMGREF_SMOOTH_MEAN_SPDUP=0x02,
    IMGREF_SMOOTH_MAX
}IMGREF_SMOOTH_TYPE_E;

typedef struct 
{
    IMGREF_CHN_T src_chn;
    IMGREF_SIZE_T src_size;
    IMGREF_RECT_T trim_rect;
    
    void* proc_buf_ptr;
    uint32 buf_size;
    
    IMGREF_CHN_T target_chn;
    IMGREF_SIZE_T target_size;
    
    IMGREF_DATA_FORMAT_E src_data_format;
    IMGREF_DATA_FORMAT_E target_data_format;
    IMGREF_SMOOTH_TYPE_E smooth_type;
	 
} SMOOTH_INPUT_PARAM_T, *SMOOTH_INPUT_PARAM_T_PTR;

//////////////////////////////////////////////
//fill polygon(only support pure color)
typedef struct
{
	IMGREF_SIZE_T	    img_size;		
	IMGREF_CHN_T      img_chn;
	
	IMGREF_POINT_T*   points_ptr;
	uint32            pt_num;	
	uint32            u32_color;
	IMGREF_RECT_T     clip_rect;
	
	IMGREF_DATA_FORMAT_E	data_format;	
}FILL_POLYGON_IN_T;

typedef struct
{
    uint32 param;

}FILL_POLYGON_OUT_T;

//////////////////////////////////////////////
//fill polygon(only support pure and gradual color)

#define GRAPH_GRAD_DIREC_V (0x01<<0)
#define GRAPH_GRAD_DIREC_H (0x01<<1)
#define GRAPH_GRAD_SPD_UNIF (0x01<<2)
#define GRAPH_GRAD_SPD_ACL (0x01<<3)


typedef enum
{
    GRAPH_FILL_MODE_PURE = 0x00,
    GRAPH_FILL_MODE_GRAD,
    GRAPH_FILL_MODE_MAX
    
}GRAPH_FILL_MODE_E;

typedef struct
{
    uint32 st_clr;
    uint32 ed_clr;
    uint32 grad_mode;
    
}FILL_GRAD_PARAM_T;

typedef struct
{
    uint32 fill_clr;        
    
}FILL_PURE_PARAM_T;

typedef struct
{
    IMGREF_SIZE_T	    img_size;		
    IMGREF_CHN_T        img_chn;

    IMGREF_POINT_T*     points_ptr;
    uint32              pt_num;	

    union
    {
        FILL_PURE_PARAM_T pure_para;
        FILL_GRAD_PARAM_T grad_para;
        
    }fill_param; 

    IMGREF_RECT_T         clip_rect;
    IMGREF_DATA_FORMAT_E  data_format;
    GRAPH_FILL_MODE_E     fill_mode;
    
}FILL_POLYGON_EX_IN_T;

typedef struct
{
     uint32 param;

}FILL_POLYGON_EX_OUT_T;


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               * 
 **---------------------------------------------------------------------------*/
 /******************************************************************************/
//  Description: Draw clock
//  Author: Shan.He 
//  Input:   
//          in_ptr:	pointer of input parameters
//  Output: 
//			out_ptr:	pointer output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 GRAPH_DrawRoundRect(const DRAW_RRECT_IN_T *in_ptr, DRAW_RRECT_OUT_T *out_ptr);

/******************************************************************************/
//  Description: Draw round rectangle
//  Author: Shan.He 
//  Input:   
//          in_ptr:	pointer of input parameters
//  Output: 
//			out_ptr:	pointer output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 GRAPH_FillRoundRect(const DRAW_RRECT_IN_T *in_ptr, DRAW_RRECT_OUT_T *out_ptr);

 /******************************************************************************/
//  Description: fill the polygon(only support Pure)
//  Author: 
//  Input:   
//          
//  Output: 
//			
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note: the coordination: left is closing and right is opening;
//       and the top and the bottom have the same rules  
/******************************************************************************/
PUBLIC BOOLEAN GRAPH_FillPolygon(FILL_POLYGON_IN_T* fill_in_ptr, FILL_POLYGON_OUT_T* fill_out_ptr);

 /******************************************************************************/
//  Description: fill the polygon(support Pure color and Gradual color)
//  Author: 
//  Input:   
//          
//  Output: 
//			
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note: the coordination: left is closing and right is opening;
//       and the top and the bottom have the same rules  
/******************************************************************************/
PUBLIC uint32 GRAPH_FillPolygonEx(FILL_POLYGON_EX_IN_T* fill_in_ptr, FILL_POLYGON_EX_OUT_T* fill_out_ptr);

/******************************************************************************/
//  Description: Draw clock
//  Author: Shan.He 
//  Input:   
//          draw_in_ptr:	pointer of input parameters
//  Output: 
//			draw_out_ptr:	pointer output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   set the data_ptr to null in hour_handle to ignore hour handle    
//			set the data_ptr to null in min_handle to ignore minute handle   
//			set the data_ptr to null in sec_handle to ignore second handle   
/******************************************************************************/
#if defined(ANALOG_CLK_SUPPORT)
PUBLIC uint32 GRAPH_DrawClock(const CLOCK_DRAW_IN_T *in_ptr, CLOCK_DRAW_OUT_T *out_ptr);
#endif

/******************************************************************************/
//  Description: scale up/down image
//  Author: Shan.He 
//  Input:   
//          scale_in_ptr:	pointer of input parameters
//  Output: 
//			scale_out_ptr:	pointer of output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 GRAPH_ScaleImage(const SCALE_IMAGE_IN_T *scale_in_ptr, SCALE_IMAGE_OUT_T *scale_out_ptr);


/******************************************************************************/
//  Description: scale and smooth image
//  Author: 
//  Input:   
//          input_ptr:	    pointer of input parameters
//  Output: 
//			None:	
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC BOOLEAN GRAPH_ScaleAndDiffusionImage(SMOOTH_INPUT_PARAM_T* input_ptr );


/******************************************************************************/
//  Description: get the capabilities of the scaling
//  Author: Shan.He 
//  Input:   
//  Output: 
//			scale_cap_ptr:	pointer of capabilities
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 GRAPH_GetScaleCapability(SCALE_IMAGE_CAP_T *scale_cap_ptr);

/******************************************************************************/
//  Description: rotate image
//  Author: Shan.He 
//  Input:   
//          rotate_in_ptr:	pointer of input parameters
//  Output: 
//			rotate_out_ptr:	pointer of output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 GRAPH_RotateImage(const ROTATE_IMAGE_IN_T *rotate_in_ptr, ROTATE_IMAGE_OUT_T *rotate_out_ptr);

/******************************************************************************/
//  Description: rotate arbitrary angle and produced anti-aliasing image
//  Author: Shan.He 
//  Input:   
//          rotate_in_ptr:	pointer of input parameters
//  Output: 
//			rotate_out_ptr:	pointer of output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 GRAPH_RotateArbitrary(const ROTATE_ARBITRARY_IN_T *rotate_in_ptr, ROTATE_ARBITRARY_OUT_T *rotate_out_ptr);

PUBLIC uint32 GRAPH_RotateArbitrary_Blend(const ROTATE_ARBITRARY_BLEND_IN_T *rotate_in_ptr, ROTATE_ARBITRARY_BLEND_OUT_T *rotate_out_ptr);
/******************************************************************************/
//  Description: get the size of image after rotation
//  Author: Shan.He 
//  Input:   
//			src_size_ptr:		pointer of the source image size
//			angle:				rotate angle
//  Output: 
//          rotated_size_ptr:	pointer of the rotated image size
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 GRAPH_GetRotatedSize(IMGREF_SIZE_T *src_size_ptr, int32 angle,
										IMGREF_SIZE_T *rotated_size_ptr);


/******************************************************************************/
//  Description: Test whether the point is on the handle of the clock
//  Author: Shan.He 
//  Input:   
//          in_ptr:			pointer of input parameters
//  Output: 
//			out_ptr:		pointer output parameters
//  Return: SCI_SUCCESS:    successful
//          others          failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 GRAPH_ClockPointTest(const CLOCK_POINT_TEST_IN_T *in_ptr, CLOCK_POINT_TEST_OUT_T *out_ptr);

/******************************************************************************/
//  Description: Get the sine value of the angle
//  Author: Shan.He 
//  Input:   
//          angle:			angle in degree
//  Output: 
//  Return:  Fixed sine value (16.16)
//  Note:  convert fixed value to float: (fixed_value / 65536.0) 
/******************************************************************************/
PUBLIC GRAPH_FIXED_T GRAPH_MathSin(int32 angle);

/******************************************************************************/
//  Description: Get the cosine value of the angle
//  Author:  Shan.He 
//  Input:   
//          angle:			angle in degree
//  Output: 
//  Return:  Fixed cosine value (16.16)
//  Note:    convert fixed value to float: (fixed_value / 65536.0)
/******************************************************************************/
PUBLIC GRAPH_FIXED_T GRAPH_MathCos(int32 angle);

/******************************************************************************
/
//  Description: get time's angle by value
//  Author: zhikun.lv
//  Input:  value of time
//  Output: out_ptr:		pointer output parameters
//  Return: N/A
//  Note:   
/******************************************************************************/
PUBLIC void GRAPH_GetAngleByTimeValue(uint8 hour, uint8 min, uint8 sec, 
uint16 *hour_angle, uint16 *min_angle, uint16 *sec_angle);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
        
#endif
