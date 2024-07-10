/******************************************************************************
 ** File Name:      sgl_shape_texture_plane.c	                              *
 ** Author:         Shan.He                                                   *
 ** DATE:           2012-02-02                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    implemetation of ploygen		                          *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2012-02-02        Shan.He         Create                                  *
 *****************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sgl_os_api.h"
#include "sgl_api.h"
#include "sgl_shape.h"
#include "sgl_canvas.h"
#include "sgl_color.h"
#include "graphics_draw.h"

/**---------------------------------------------------------------------------*
 **                         Macros                                            *
 **---------------------------------------------------------------------------*/
 #define MIN_TEXTURE_WIDTH		4
 #define MIN_TEXTURE_HEIGHT		4
/**---------------------------------------------------------------------------*
 **                         Structures                                        *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         LOCAL Functions                                   *
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
LOCAL int32 _CheckParam(SGL_SHAPE_TEXTURE_PLANE_T *plane_ptr, _CANVAS_CXT_T *canvas_ptr)
{
	if (NULL == plane_ptr)
	{
		return SGL_ERROR;
	}

	if (NULL == plane_ptr->texture.data_ptr)
	{
		return SGL_ERROR;
	}

	if (plane_ptr->texture.width < MIN_TEXTURE_WIDTH 
		|| plane_ptr->texture.height < MIN_TEXTURE_HEIGHT)
	{
		return SGL_ERROR;
	}

	return SGL_SUCCESS;
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
LOCAL int32 _RotateOnZAxis(SGL_SHAPE_TEXTURE_PLANE_T *plane_ptr, _CANVAS_CXT_T *canvas_ptr,
							  SGL_ROTATE_T *rotate_ptr)
{
	ROTATE_ARBITRARY_IN_T	rotate_in = {0};
	ROTATE_ARBITRARY_OUT_T	rotate_out = {0};
	uint32 ret = 0;
	uint32 texture_bytes_per_pixels = canvas_ptr->bytes_per_pixel;

	rotate_in.angle = rotate_ptr->angle;
	rotate_in.mode = ROTATE_ARBITRARY_AUTO_CROP;

	rotate_in.src_center.x = rotate_ptr->pivot.x - plane_ptr->left_top.x;
	rotate_in.src_center.y = rotate_ptr->pivot.y - plane_ptr->left_top.y;
	rotate_in.src_size.w = plane_ptr->size.w;
	rotate_in.src_size.h = plane_ptr->size.h;
	rotate_in.src_chn.chn0.ptr = (void *)plane_ptr->texture.data_ptr;
	rotate_in.src_chn.chn0.size = plane_ptr->size.w * plane_ptr->size.h * texture_bytes_per_pixels;
	
	rotate_in.target_center.x = rotate_ptr->pivot.x;
	rotate_in.target_center.y = rotate_ptr->pivot.y;
	rotate_in.target_chn.chn0.ptr = canvas_ptr->mem_ptr;
	rotate_in.target_chn.chn0.size = canvas_ptr->mem_width * canvas_ptr->height * canvas_ptr->bytes_per_pixel;
	rotate_in.target_size.w = canvas_ptr->mem_width;
	rotate_in.target_size.h = canvas_ptr->height;

	switch (canvas_ptr->data_format)
	{
	case SGL_FORMAT_RGB565:
		rotate_in.target_format = IMGREF_FORMAT_RGB565;
		break;

	case SGL_FORMAT_ARGB:
		rotate_in.target_format = IMGREF_FORMAT_ARGB888;
		break;

	default:
		return SGL_ERROR;
	}

	switch (plane_ptr->texture.data_format)
	{
	case SGL_FORMAT_RGB565:
		rotate_in.src_format = IMGREF_FORMAT_RGB565;
		break;

	case SGL_FORMAT_ARGB:
		rotate_in.src_format = IMGREF_FORMAT_ARGB888;
		break;

	default:
		return SGL_ERROR;
	}
	
	ret = GRAPH_RotateArbitrary(&rotate_in, &rotate_out);
	
	return ret;
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
LOCAL int32 _DrawTexturePlane(_SHAPE_CXT_T *cxt_ptr)
{
	SGL_SHAPE_TEXTURE_PLANE_T		*plane_ptr = NULL;
	_CANVAS_CXT_T					*canvas_ptr = NULL;
	int32							ret = SGL_ERROR;
	
	if (NULL == cxt_ptr)
	{
		return SGL_ERROR;		
	}

	plane_ptr = (SGL_SHAPE_TEXTURE_PLANE_T *)cxt_ptr->shape_ptr;
	canvas_ptr = (_CANVAS_CXT_T *)cxt_ptr->canvas_handle;

	if (0 == cxt_ptr->trans_count || NULL == cxt_ptr->trans_array_ptr)
	{
		//no transform, draw the plane on the canvas directly
	}
	else
	{
		SGL_ROTATE_T	*rotate_ptr = NULL;
		
		if (cxt_ptr->trans_count > 1 || SGL_TRANS_ROTATE != cxt_ptr->trans_array_ptr->type)
		{
			return SGL_ERROR;
		}

		rotate_ptr = &cxt_ptr->trans_array_ptr->op_param.rotate;

		switch (rotate_ptr->axis)
		{
		case SGL_AXIS_Z:
			ret = _RotateOnZAxis(plane_ptr, canvas_ptr, rotate_ptr);
			break;

		default:
			return SGL_ERROR;
		}		
	}

	return ret;
}


/**---------------------------------------------------------------------------*
 **                         Public Functions                                   *
 **---------------------------------------------------------------------------*/

PUBLIC _SHAPE_CB_TBL_T g_shape_texture_plane_func = 
{
	NULL,
	_DrawTexturePlane,	
	NULL
};

