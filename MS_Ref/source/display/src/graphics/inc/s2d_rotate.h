/******************************************************************************
 ** File Name:      s2d_rotate.h                                              *
 ** Author:                                                                   *
 ** DATE:           2010-8-25                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    image rotate functions                                    *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *                                                            *
 *****************************************************************************/
#ifndef _S2D_ROTATE_H_
#define _S2D_ROTATE_H_

 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "s2d.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         Data Types                                        *
 **---------------------------------------------------------------------------*/
#if 0
#define COLOR32_T uint32 

typedef struct Rect_Map_t {
	COLOR32_T	*map_date_ptr;
    int16	map_width;							//图片宽度						
    int16	map_height;							//图片高度
    int16	map_mid_x;							//旋转点相对图片的位置
	int16	map_mid_y;							//旋转点相对图片的位置
    int16	window_mid_x;						//旋转点相对窗口的位置
	int16	window_mid_y;						//旋转点相对窗口的位置
}RECT_MAP_T;

typedef struct Rotate_Rect_t {
	COLOR32_T	*rotate_date_ptr;
    int16	rect_start_x;						//区域起点横坐标					
    int16	rect_start_y;						//区域起点纵坐标
    int16	rect_width;							//区域宽
	int16	rect_height;							//区域高
}ROTATE_RECT_T;
#endif

typedef struct 
{
    uint8   sec;        // secondsafter the minute - [0,59] 
    uint8   min;        // minutesafter the hour - [0,59] 
    uint8   hour;       // hours since midnight - [0,23] 
}TM_TIME_T;

typedef struct 
{
    uint16   angle_sec;        
    uint16   angle_min;        
    uint16   angle_hour;       
}TM_TIME_ANGLE_T;

typedef struct
{
	void	*data_ptr;
	uint32	data_size;
	uint32	width;
	uint32	height;
	uint32	format;					//0: ARGB888, 1: RGB565, 2: ARGB555
}S2D_IMG_T;

typedef struct
{
	S2D_IMG_T		src_img;
	GDI_POINT_T		src_rot_center;
	
	S2D_IMG_T		dst_img;	
	GDI_POINT_T		dst_rot_center;
	uint32			dst_color;
	uint32			dst_is_fill_color;	// 0: do not fill the dst with color, others: fill the dst with color

	int32			angle;
	
	void			*temp_buf_ptr;
	uint32			temp_buf_size;

	int32			mode;				//0: auto crop; 1: no crop
	BOOLEAN 		speed_up;
}S2D_ROT_IN_T;

typedef struct
{
	S2D_IMG_T		out_img;	
	GDI_POINT_T		out_img_offset;
}S2D_ROT_OUT_T;

typedef struct
{
	IMGREF_SIZE_T	src_size;
	GDI_POINT_T		src_rot_center;
	
	IMGREF_SIZE_T	dst_size;
	GDI_POINT_T		dst_rot_center;

	int32			angle;
}S2D_ROT_TEST_IN_T;

typedef struct
{
	IMGREF_POINT_T	rot_offset;
	IMGREF_SIZE_T	rot_size;
	IMGREF_POINT_T	rot_vertex[4];	
}S2D_ROT_TEST_OUT_T;

typedef struct
{
	S2D_IMG_T		src_img;
	GDI_POINT_T		src_rot_center;
	
	S2D_IMG_T		dst_img;	
	GDI_POINT_T		dst_rot_center;
	uint32			dst_color;
	uint32			dst_is_fill_color;	// 0: do not fill the dst with color, others: fill the dst with color

	int32			angle;
	
	void			*temp_buf_ptr;
	uint32			temp_buf_size;

	int32			mode;				//0: auto crop; 1: no crop
}S2D_ROT_BLEND_IN_T;

typedef struct
{
	S2D_IMG_T		out_img;	
	GDI_POINT_T		out_img_offset;
}S2D_ROT_BLEND_OUT_T;



/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               * 
 **---------------------------------------------------------------------------*/
 /******************************************************************************/
//  Description: get the rotation angle of each handle corresponding to the time
//  Author: 	
//  Input:  
//			 sTime_ptr:   pinter of the time structure
//			 sAnlge_ptr:  pointer of the angle struncture
//  Output:  None
//  Return:	 None
//  Note:  
/******************************************************************************/
PUBLIC void S2d_GetTimeAngle(TM_TIME_T	*sTime_ptr,	TM_TIME_ANGLE_T		*sAnlge_ptr);

 /******************************************************************************/
//  Description: get the rotation angle of each handle corresponding to the time
//  Author: 	
//  Input:  
//			 sTime_ptr:   pinter of the time structure
//			 sAnlge_ptr:  pointer of the angle struncture
//  Output:  None
//  Return:	 None
//  Note:  
/******************************************************************************/
PUBLIC BOOLEAN S2d_Rotate(S2D_ROT_IN_T *rot_in_ptr, S2D_ROT_OUT_T *rot_out_ptr);


 /******************************************************************************/
//  Description: rotate the image and blend it with background image
//  Author: 	
//  Input:  
//			 rot_in_ptr:   pinter of the time structure
//			 rot_out_ptr:  pointer of the angle struncture
//  Output:  None
//  Return:	 None
//  Note:  
/******************************************************************************/
PUBLIC BOOLEAN S2d_Rotate_Blend(S2D_ROT_IN_T *rot_in_ptr, S2D_ROT_OUT_T *rot_out_ptr);


/******************************************************************************/
//  Description: get the size of the temp buffer of rotation
//  Author: 	
//  Input:   None
//  Output:  None
//  Return:	 the size of the temp buffer
//  Note:  
/******************************************************************************/
PUBLIC uint32 S2d_GetRotateTempBufSize();

/******************************************************************************/
//  Description: get the size of the rotated image
//  Author: 	
//  Input:   None
//  Output:  None
//  Return:	 None
//  Note:  
/******************************************************************************/
PUBLIC void S2d_GetRotatedSize(IMGREF_SIZE_T *rotated_size_ptr, 
							  IMGREF_SIZE_T *src_size_ptr, int32 angle);

/******************************************************************************/
//  Description: Only calculate the vertices of the rectange after rotation
//  Author: 	
//  Input:   
//  Output:  
//  Return:	 
//  Note:  
/******************************************************************************/
PUBLIC uint32 S2d_RotateTest(S2D_ROT_TEST_IN_T *rot_in_ptr, S2D_ROT_TEST_OUT_T *rot_out_ptr);

/******************************************************************************/
//  Description: Check whether the point is in the ploygen
//  Author: 	
//  Input:   
//  Output:  
//  Return:	 
//  Note:  
/******************************************************************************/
PUBLIC uint32 S2d_PointInPloygen(IMGREF_POINT_T *vertex_ptr, uint32 vertex_num, 
									 IMGREF_POINT_T *point_ptr, BOOLEAN *result_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif