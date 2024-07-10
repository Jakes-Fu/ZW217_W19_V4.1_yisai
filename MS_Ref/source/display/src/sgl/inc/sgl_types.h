/******************************************************************************
 ** File Name:      sgl_types.h		    							  		  *
 ** Author:          			                                              *
 ** DATE:           2011-12-23                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    SGL types                					              *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2011-12-23        Shan.He		  Create                                  *
 *****************************************************************************/
#ifndef __SGL_TYPES_H__
#define __SGL_TYPES_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
        
/**---------------------------------------------------------------------------*
 **                         Marcos	                                          *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Data Types                                        *
 **---------------------------------------------------------------------------*/
typedef int32 	SGL_HANDLE_T;
typedef uint32 	SGL_COLOR_ARGB_T;
typedef uint32	SGL_COLOR_RGB888_T;
typedef uint16 	SGL_COLOR_RGB565_T;
typedef uint8 	SGL_COLOR_GRAY_T;

typedef enum
{
	SGL_FORMAT_ARGB = 0,
	//SGL_FORMAT_RGB888,
	SGL_FORMAT_RGB565,
	//SGL_FORMAT_GRAY
}SGL_DATA_FORMAT_E;

typedef enum
{
	SGL_SHAPE_LINE = 0,
	SGL_SHAPE_TEXTURE_PLANE,
	SGL_SHAPE_POLYGON
}SGL_SHAPE_TYPE_E;

typedef enum
{
	SGL_TRANS_MOVE = 0,
	SGL_TRANS_ROTATE
}SGL_TRANSFORM_TYPE_E;

typedef enum
{
	SGL_AXIS_X = 0,
	SGL_AXIS_Y,
	SGL_AXIS_Z
}SGL_AXIS_E;

typedef enum
{
	SGL_LINE_SOLID = 0,
	SGL_LINE_DASH,
	SGL_LINE_DOT,
	SGL_LINE_DOT_DASH
}SGL_LINE_STYLE_E;

typedef enum
{
	SGL_LINE_CAP_SQUARE = 0,
	SGL_LINE_CAP_ROUND
}SGL_LINE_END_CAP_E;

typedef enum
{
	SGL_PGN_SOLID_OUTLINE = 0,			// draw the polygon outline with solid line
	SGL_PGN_FILL,						//fill with sole color
	SGL_PGN_GRADIENT_FILL_L2R,			//fill width gradient color which change from left to right
	SGL_PGN_GRADIENT_FILL_T2B			//fill width gradient color which change from top to bottom
}SGL_POLYGON_STYLE_E;

typedef struct
{
	int32 	left; 
	int32	top;
	int32	right;
	int32	bottom;
}SGL_RECT2_T;

typedef struct
{
	int32	x;
	int32	y;
}SGL_VERTEX2_T;

typedef struct
{
	int32 	x;
	int32	y;
	int32	z;
}SGL_VERTEX3_T;

typedef struct
{
	uint32	w;
	uint32	h;
}SGL_SIZE_T;

typedef struct
{
	SGL_VERTEX3_T		pos;	
}SGL_CAMERA_T;

typedef struct
{
	SGL_CAMERA_T		camera;	
}SGL_SCENE_T;

typedef struct
{
	uint32				width;			//canvas width in pixel
	uint32				height;			//canvas height in pixel
	void				*mem_ptr;		//pointer of the canvas memory. if it is NULL the buffer will be allocated while the canvas be created 
	uint32				mem_width;		//memory width in pixel, which maybe equal or larger than width
	SGL_DATA_FORMAT_E	data_format;
	SGL_RECT2_T			draw_rect;		//the context in the rectangle will be drawn while the others will be clipped
}SGL_CANVAS_T;

typedef struct
{
	SGL_VERTEX3_T		distance;
}SGL_MOVE_T;

typedef struct
{
	SGL_AXIS_E			axis;
	SGL_VERTEX3_T		pivot;
	int32				angle;	
}SGL_ROTATE_T;

typedef struct
{
	SGL_TRANSFORM_TYPE_E	type;
	
	union
	{
		SGL_MOVE_T		move;
		SGL_ROTATE_T	rotate;
	}op_param;
}SGL_TRANSFORM_T;

typedef struct
{
	void 				*shape_ptr;				//pointer of the speicified shape structure such as SGL_SHAPE_LINE_T and so on
	SGL_HANDLE_T		canvas_handle;			//handle of the canvas to draw
	int32				trans_count;			//set to 0 to ignore transform
	SGL_TRANSFORM_T		*trans_array_ptr;		//set to NULL to ignore transform
}SGL_DRAW_SHAPE_IN_T;

typedef struct 
{
	SGL_RECT2_T			update_rect;			//rectangle have been updated in the canvas
}SGL_DRAW_SHAPE_OUT_T;

typedef struct
{
	int32				width;					//texture width
	int32				height;					//texture height
	SGL_DATA_FORMAT_E	data_format;			//data format
	void				*data_ptr;				//data address
}SGL_TEXTURE_T;
///////////////////////////////////////////////////////////////////////
//structure of shape
typedef struct
{	
	SGL_VERTEX3_T		begin;
	SGL_VERTEX3_T		end;
	SGL_COLOR_RGB888_T	color;
	SGL_LINE_STYLE_E	style;
	SGL_LINE_END_CAP_E	end_cap;	
	int32				width;
}SGL_SHAPE_LINE_T;


typedef struct
{
	uint32				vertex_num;			//number of the vertex
	SGL_VERTEX3_T		*vertex_ptr;		//vertex array pointer, the array size is the vertex_num
	SGL_POLYGON_STYLE_E style;				//style of the ploygen

	int32				outline_width;		//valid if the style is SGL_PGN_SOLID_OUTLINE;

	union
	{
		SGL_COLOR_RGB888_T	outline_color;	//valid if the style is SGL_PGN_SOLID_OUTLINE	
		SGL_COLOR_RGB888_T fill_color;		//valid if the style is SGL_PGN_FILL
		SGL_COLOR_RGB888_T gradient_color[2]; //valid if the style is SGL_PGN_GRADIENT_FILL_xxx
	}color;	
}SGL_SHAPE_POLYGON_T;

typedef struct
{
	SGL_VERTEX3_T			left_top;		//left-top position of the plane
	SGL_SIZE_T				size;			//size of the plane
	SGL_TEXTURE_T			texture;
	
}SGL_SHAPE_TEXTURE_PLANE_T;
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
        
#endif
