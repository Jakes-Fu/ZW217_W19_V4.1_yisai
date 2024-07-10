/******************************************************************************
 ** File Name:      test_bench.c	                                          *
 ** Author:         Shan.He                                                   *
 ** DATE:           2012-01-21                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    test bench of SGL			                              *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2012-01-21        Shan.He         Create                                  *
 *****************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sgl_os_api.h"
#include "sgl_api.h"
#include "writetobmp.h"
#include "sgl_color.h"
#include "read_res_img.h"

/**---------------------------------------------------------------------------*
 **                         Macros                                            *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Structures                                        *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Functions                                   *
 **---------------------------------------------------------------------------*/
BOOLEAN SaveCanvas(SGL_CANVAS_T *canvas_ptr, const char *name)
{
	uint32 data_type = 0;
	int32 result = 0;

	switch (canvas_ptr->data_format)
	{
	case SGL_FORMAT_ARGB:
		data_type = 1;
		break;

	case SGL_FORMAT_RGB565:
		data_type = 2;
		break;

#if 0
	case SGL_FORMAT_GRAY:
		data_type = 3;
		break;
#endif
	}

	result = WriteBmp(name, (void *)canvas_ptr->mem_ptr, canvas_ptr->mem_width, 
						canvas_ptr->height, data_type);

	return (0 == result) ? TRUE : FALSE;
}

BOOLEAN GetTexture(uint32 id, SGL_TEXTURE_T *texture_ptr)
{
	RES_IMG_T res_img = {0};
	
	if (!ReadResImageFromID(id, NULL, 0, &res_img))
	{
		return FALSE;
	}

	switch (res_img.format)
	{
	case 0:
		texture_ptr->data_format = SGL_FORMAT_ARGB;
		break;

	case 1:
		texture_ptr->data_format = SGL_FORMAT_RGB565;
		break;	

	default:
		return FALSE;
	}	

	texture_ptr->width = res_img.width;
	texture_ptr->height = res_img.height;
	
	texture_ptr->data_ptr = SGL_ALLOC(res_img.size);
	if (NULL == texture_ptr->data_ptr)
	{
		return FALSE;
	}
	
	if (!ReadResImageFromID(id, texture_ptr->data_ptr, res_img.size, &res_img))
	{
		SGL_FREE(texture_ptr->data_ptr);
		return FALSE;
	}

	return TRUE;
}

/**---------------------------------------------------------------------------*
 **                         Public Functions                                   *
 **---------------------------------------------------------------------------*/
/******************************************************************************/
//  Description: 
//  Author: 
//  Input:   
//          
//  Output: 
//			
//  Return: 
//  Note:   
/******************************************************************************/
PUBLIC void TEST_DrawLine(const char *save_name)
{
	SGL_SCENE_T		scene = {0};
	
	SGL_HANDLE_T	canvas_handle = NULL;
	SGL_CANVAS_T	canvas = {0};

	SGL_HANDLE_T			shape_handle = NULL;
	SGL_DRAW_SHAPE_IN_T		draw_shape_in = {0};
	SGL_DRAW_SHAPE_OUT_T	draw_shape_out = {0};
	
	SGL_SHAPE_LINE_T 	line = {0};

	////////////////////////////////////////////////////////////
	//create canvas
	canvas.data_format = SGL_FORMAT_RGB565;
	canvas.width = 240;
	canvas.height = 320;
	canvas.mem_width = 240;
	canvas.draw_rect.left = 0;
	canvas.draw_rect.top = 0;
	canvas.draw_rect.right = canvas.width - 1;
	canvas.draw_rect.bottom = canvas.height - 1;
	canvas.mem_ptr = SGL_ALLOC(canvas.mem_width * canvas.height * 2);

	if (NULL == canvas.mem_ptr)
	{
		goto TEST_EXIT;	
	}
	
	canvas_handle = SGL_CreateCanvas(&canvas);
	if (NULL == canvas_handle)
	{
		goto TEST_EXIT;	
	}

	SGL_ClearCanvas(canvas_handle, 0);
	
	////////////////////////////////////////////////////////////
	//draw shape
	shape_handle = SGL_CreateShape(SGL_SHAPE_LINE, NULL);
	if (NULL == shape_handle)
	{
		goto TEST_EXIT;	
	}

	draw_shape_in.canvas_handle = canvas_handle;
	draw_shape_in.shape_ptr = (void *)&line;

	/*line property*/
	line.begin.x = 0;
	line.begin.y = 0;
	line.begin.z = 0;

	line.end.x = 239;
	line.end.y = 319;
	line.end.z = 0;

	line.color = 0xFFFFFF;
	line.width = 4;	
	line.style = SGL_LINE_DOT_DASH;

	if (SGL_SUCCESS != SGL_DrawShape(shape_handle, &draw_shape_in, &draw_shape_out))
	{
		goto TEST_EXIT;
	}

	/*line property*/
	line.begin.x = 239;
	line.begin.y = 0;
	line.begin.z = 0;

	line.end.x = 0;
	line.end.y = 319;
	line.end.z = 0;

	if (SGL_SUCCESS != SGL_DrawShape(shape_handle, &draw_shape_in, &draw_shape_out))
	{
		goto TEST_EXIT;
	}

	////////////////////////////////////////////////////////////
	//save canvas to bitmap for debug
	if (save_name)
	{
		SaveCanvas(&canvas, save_name);
	}	
	
	////////////////////////////////////////////////////////////
	//destroy resoures
TEST_EXIT:
	SGL_DestroyCanvas(canvas_handle);
	SGL_DestroyShape(shape_handle);

	if (NULL != canvas.mem_ptr)
	{
		SGL_FREE(canvas.mem_ptr);
	}

	return;
}

/******************************************************************************/
//  Description: 
//  Author: 
//  Input:   
//          
//  Output: 
//			
//  Return: 
//  Note:   
/******************************************************************************/
PUBLIC void TEST_DrawPolygon(const char *save_name)
{
	SGL_SCENE_T		scene = {0};
	
	SGL_HANDLE_T	canvas_handle = NULL;
	SGL_CANVAS_T	canvas = {0};

	SGL_HANDLE_T			shape_handle = NULL;
	SGL_DRAW_SHAPE_IN_T		draw_shape_in = {0};
	SGL_DRAW_SHAPE_OUT_T	draw_shape_out = {0};
	
	SGL_SHAPE_POLYGON_T 	polygon = {0};
	SGL_VERTEX3_T			vertex[4] = {0};

	////////////////////////////////////////////////////////////
	//create canvas
	canvas.data_format = SGL_FORMAT_RGB565;
	canvas.width = 240;
	canvas.height = 320;
	canvas.mem_width = 240;
	canvas.draw_rect.left = 0;
	canvas.draw_rect.top = 0;
	canvas.draw_rect.right = canvas.width - 1;
	canvas.draw_rect.bottom = canvas.height - 1;
	canvas.mem_ptr = SGL_ALLOC(canvas.mem_width * canvas.height * 2);

	if (NULL == canvas.mem_ptr)
	{
		goto TEST_EXIT;	
	}
	
	canvas_handle = SGL_CreateCanvas(&canvas);
	if (NULL == canvas_handle)
	{
		goto TEST_EXIT;	
	}

	SGL_ClearCanvas(canvas_handle, 0);
	
	////////////////////////////////////////////////////////////
	//draw shape
	shape_handle = SGL_CreateShape(SGL_SHAPE_POLYGON, NULL);
	if (NULL == shape_handle)
	{
		goto TEST_EXIT;	
	}

	draw_shape_in.canvas_handle = canvas_handle;
	draw_shape_in.shape_ptr = (void *)&polygon;

	/*polygon property*/
	polygon.color.fill_color = 0xff0000;
	polygon.style = SGL_PGN_FILL;
	polygon.vertex_num = 4;
	polygon.vertex_ptr = vertex;
	vertex[0].x = 40;
	vertex[0].y = 10;
	vertex[1].x = 70;
	vertex[1].y = 30;
	vertex[2].x = 50;
	vertex[2].y = 80;
	vertex[3].x = 10;
	vertex[3].y = 45;	

	if (SGL_SUCCESS != SGL_DrawShape(shape_handle, &draw_shape_in, &draw_shape_out))
	{
		goto TEST_EXIT;
	}

	/*line property*/
	polygon.color.fill_color = 0xffff00;
	polygon.style = SGL_PGN_FILL;
	polygon.vertex_num = 4;
	polygon.vertex_ptr = vertex;
	vertex[0].x = 65;
	vertex[0].y = 100;
	vertex[1].x = 200;
	vertex[1].y = 120;
	vertex[2].x = 160;
	vertex[2].y = 200;
	vertex[3].x = 40;
	vertex[3].y = 260;

	if (SGL_SUCCESS != SGL_DrawShape(shape_handle, &draw_shape_in, &draw_shape_out))
	{
		goto TEST_EXIT;
	}

	////////////////////////////////////////////////////////////
	//save canvas to bitmap for debug
	if (save_name)
	{
		SaveCanvas(&canvas, save_name);
	}	
	
	////////////////////////////////////////////////////////////
	//destroy resoures
TEST_EXIT:
	SGL_DestroyCanvas(canvas_handle);
	SGL_DestroyShape(shape_handle);

	if (NULL != canvas.mem_ptr)
	{
		SGL_FREE(canvas.mem_ptr);
	}

	return;
}


/******************************************************************************/
//  Description: 
//  Author: 
//  Input:   
//          
//  Output: 
//			
//  Return: 
//  Note:   
/******************************************************************************/
PUBLIC void TEST_DrawTexturePlane(const char *save_name)
{
	SGL_SCENE_T		scene = {0};
	
	SGL_HANDLE_T	canvas_handle = NULL;
	SGL_CANVAS_T	canvas = {0};

	SGL_HANDLE_T			shape_handle = NULL;
	SGL_DRAW_SHAPE_IN_T		draw_shape_in = {0};
	SGL_DRAW_SHAPE_OUT_T	draw_shape_out = {0};
	
	SGL_SHAPE_TEXTURE_PLANE_T plane = {0};
	SGL_TRANSFORM_T			   transform = {0};

	SGL_TEXTURE_T			texture = {0};
	uint32					texture_id = 1;

	////////////////////////////////////////////////////////////
	//get texture
	if (!GetTexture(texture_id, &texture))
	{
		goto TEST_EXIT;	
	}

	////////////////////////////////////////////////////////////
	//create canvas
	canvas.data_format = texture.data_format;//SGL_FORMAT_ARGB;//SGL_FORMAT_RGB565;
	canvas.width = 480;
	canvas.height = 480;
	canvas.mem_width = canvas.width;
	canvas.draw_rect.left = 0;
	canvas.draw_rect.top = 0;
	canvas.draw_rect.right = canvas.width - 1;
	canvas.draw_rect.bottom = canvas.height - 1;
	canvas.mem_ptr = SGL_ALLOC(canvas.mem_width * canvas.height * 4);

	if (NULL == canvas.mem_ptr)
	{
		goto TEST_EXIT;	
	}
	
	canvas_handle = SGL_CreateCanvas(&canvas);
	if (NULL == canvas_handle)
	{
		goto TEST_EXIT;	
	}

	SGL_ClearCanvas(canvas_handle, 0);	
	////////////////////////////////////////////////////////////
	//draw shape
	shape_handle = SGL_CreateShape(SGL_SHAPE_TEXTURE_PLANE, NULL);
	if (NULL == shape_handle)
	{
		goto TEST_EXIT;	
	}

	draw_shape_in.canvas_handle = canvas_handle;
	draw_shape_in.shape_ptr = (void *)&plane;

	/*plane property*/	
	plane.texture = texture;
	plane.left_top.x = (canvas.width - texture.width) / 2;
	plane.left_top.y = (canvas.height - texture.height) / 2;
	plane.size.w = texture.width;
	plane.size.h = texture.height;

	draw_shape_in.trans_array_ptr = &transform;
	draw_shape_in.trans_count = 1;

	transform.type = SGL_TRANS_ROTATE;
	transform.op_param.rotate.axis = SGL_AXIS_Z;
	transform.op_param.rotate.pivot.x = canvas.width / 2;
	transform.op_param.rotate.pivot.y = canvas.height / 2;
	transform.op_param.rotate.angle = 45;
	
	if (SGL_SUCCESS != SGL_DrawShape(shape_handle, &draw_shape_in, &draw_shape_out))
	{
		goto TEST_EXIT;
	}

	////////////////////////////////////////////////////////////
	//save canvas to bitmap for debug
	if (save_name)
	{
		SaveCanvas(&canvas, save_name);
	}	
	
	////////////////////////////////////////////////////////////
	//destroy resoures
TEST_EXIT:
	SGL_DestroyCanvas(canvas_handle);
	SGL_DestroyShape(shape_handle);

	if (NULL != canvas.mem_ptr)
	{
		SGL_FREE(canvas.mem_ptr);
	}

	if (NULL != texture.data_ptr)
	{
		SGL_FREE(texture.data_ptr);
	}

	return;	
}
