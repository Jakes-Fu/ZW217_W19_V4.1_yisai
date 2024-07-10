/******************************************************************************
 ** File Name:      sgl_shape.c	                                              *
 ** Author:         Shan.He                                                   *
 ** DATE:           2012-01-21                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    implemetation of shape		                              *
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
#include "sgl_shape.h"

/**---------------------------------------------------------------------------*
 **                         Macros                                            *
 **---------------------------------------------------------------------------*/
LOCAL const char 	SHAPE_MARK[] 			= "sgl_shape";
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
LOCAL BOOLEAN _ShapeCheckCxt(_SHAPE_CXT_T *cxt_ptr)
{
	if (NULL != cxt_ptr && cxt_ptr->mark == SHAPE_MARK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
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
LOCAL BOOLEAN _ShapeCheckParam(SGL_DRAW_SHAPE_IN_T *draw_shape_ptr)
{
	if (NULL == draw_shape_ptr)
	{
		return FALSE;
	}

	if (NULL == draw_shape_ptr->canvas_handle)
	{
		return FALSE;
	}

	if (NULL == draw_shape_ptr->shape_ptr)
	{
		return FALSE;		
	}

	return TRUE;
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
LOCAL SHAPE_CALLBACK _GetShapeCallback(SGL_SHAPE_TYPE_E shape_type, _SHAPE_CALLBACK_TYPE cb_type)
{
	SHAPE_CALLBACK	callback_func = NULL;
	_SHAPE_CB_TBL_T	*callback_table_ptr = {NULL};

	switch (shape_type)
	{
	case SGL_SHAPE_LINE:
		callback_table_ptr = &g_shape_line_func;
		break;

	case SGL_SHAPE_POLYGON:
		callback_table_ptr = &g_shape_polygon_func;
		break;

	case SGL_SHAPE_TEXTURE_PLANE:
		callback_table_ptr = &g_shape_texture_plane_func;
		break;

	default:
		break;
	}

	switch (cb_type)
	{
	case SHAPE_CB_CREATE:
		callback_func = callback_table_ptr->create_func;
		break;

	case SHAPE_CB_DRAW:
		callback_func = callback_table_ptr->draw_func;
		break;

	case SHAPE_CB_DESTROY:
		callback_func = callback_table_ptr->destroy_func;
		break;		

	default:
		break;
	}	

	return callback_func;
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
PUBLIC SGL_HANDLE_T  SGL_CreateShape(SGL_SHAPE_TYPE_E shape_type, SGL_HANDLE_T scene_handle)
{
	_SHAPE_CXT_T	*cxt_ptr = NULL;
	
	cxt_ptr = (_SHAPE_CXT_T *)SGL_ALLOC(sizeof(_SHAPE_CXT_T));
	if (cxt_ptr)
	{
		SHAPE_CALLBACK 	create_func = NULL;
		
		cxt_ptr->mark = SHAPE_MARK;

		cxt_ptr->shape_type = shape_type;
		cxt_ptr->scene_handle = scene_handle;

		create_func = _GetShapeCallback(shape_type, SHAPE_CB_CREATE);

		if (NULL != create_func)
		{
			int32 ret = SGL_SUCCESS;
			
			ret = create_func(cxt_ptr);
			if (SGL_SUCCESS != ret)
			{
				SGL_FREE(cxt_ptr);	
				cxt_ptr = NULL;
			}
		}
	}
	
	return (SGL_HANDLE_T)cxt_ptr;
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
PUBLIC int32 SGL_DrawShape(SGL_HANDLE_T shape_handle, SGL_DRAW_SHAPE_IN_T *draw_shape_in_ptr,
								SGL_DRAW_SHAPE_OUT_T *draw_shape_out_ptr)
{
	_SHAPE_CXT_T	*cxt_ptr = (_SHAPE_CXT_T *)shape_handle;
	SHAPE_CALLBACK 	draw_func = NULL;
	int32 			ret = SGL_SUCCESS;
	
	if (!_ShapeCheckCxt(cxt_ptr))
	{
		return SGL_ERROR;
	}
	
	if (!_ShapeCheckParam(draw_shape_in_ptr))
	{
		return SGL_ERROR;
	}

	cxt_ptr->canvas_handle = draw_shape_in_ptr->canvas_handle;
	cxt_ptr->shape_ptr = draw_shape_in_ptr->shape_ptr;
	cxt_ptr->trans_array_ptr = draw_shape_in_ptr->trans_array_ptr;
	cxt_ptr->trans_count = draw_shape_in_ptr->trans_count;

	draw_func = _GetShapeCallback(cxt_ptr->shape_type, SHAPE_CB_DRAW);
	if (NULL != draw_func)
	{
		ret = draw_func(cxt_ptr);
	}
	
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
PUBLIC int32 SGL_DestroyShape(SGL_HANDLE_T shape_handle)
{
	_SHAPE_CXT_T	*cxt_ptr = (_SHAPE_CXT_T *)shape_handle;	
	
	if (_ShapeCheckCxt(cxt_ptr))
	{
		int32 			ret = SGL_SUCCESS;
		SHAPE_CALLBACK 	destroy_func = NULL;
		
		destroy_func = _GetShapeCallback(cxt_ptr->shape_type, SHAPE_CB_DESTROY);
		if (NULL != destroy_func)
		{
			ret = destroy_func(cxt_ptr);
		}
	
		SGL_FREE(cxt_ptr);
		return SGL_SUCCESS;
	}
	else
	{
		return SGL_ERROR;
	}
}

