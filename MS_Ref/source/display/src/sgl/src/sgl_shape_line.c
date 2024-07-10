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
#include "draw_line.h"
#include "sgl_canvas.h"
#include "sgl_color.h"

/**---------------------------------------------------------------------------*
 **                         Macros                                            *
 **---------------------------------------------------------------------------*/
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
LOCAL int32 _DrawSolidLine(SGL_SHAPE_LINE_T	*line_ptr, _CANVAS_CXT_T *canvas_ptr)
{
	uint32				color = 0;

	switch (canvas_ptr->data_format)
	{
	case SGL_FORMAT_ARGB:	
		color = line_ptr->color | 0xFF;
		DrawThickLineAAU32(line_ptr->begin.x, line_ptr->begin.y, line_ptr->end.x, line_ptr->end.y,
							color, line_ptr->width, canvas_ptr->mem_ptr,
							canvas_ptr->mem_width, canvas_ptr->height, NULL);
		break;

	case SGL_FORMAT_RGB565:
		color = SGL_RGB888_TO_RGB565(line_ptr->color);
		DrawThickLineAA(line_ptr->begin.x, line_ptr->begin.y, line_ptr->end.x, line_ptr->end.y,
							color, line_ptr->width, canvas_ptr->mem_ptr,
							canvas_ptr->mem_width, canvas_ptr->height, NULL);		
		break;

	default:
		break;
	}

	return 0;
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
LOCAL int32 _DrawDashLine(SGL_SHAPE_LINE_T	*line_ptr, _CANVAS_CXT_T *canvas_ptr)
{
	uint32				color = 0;
	int32				mem_type = 0;

	switch (canvas_ptr->data_format)
	{
	case SGL_FORMAT_ARGB:	
		color = line_ptr->color | 0xFF;
		mem_type = 1;
		break;

	case SGL_FORMAT_RGB565:
		color = SGL_RGB888_TO_RGB565(line_ptr->color);
		mem_type = 0;	
		break;

	default:
		break;
	}

	DrawDashLine(line_ptr->begin.x, line_ptr->begin.y, line_ptr->end.x, line_ptr->end.y,
						color, line_ptr->width, canvas_ptr->mem_ptr,
						canvas_ptr->mem_width, canvas_ptr->height, mem_type, NULL);	

	return 0;
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
LOCAL int32 _DrawDotLine(SGL_SHAPE_LINE_T	*line_ptr, _CANVAS_CXT_T *canvas_ptr)
{
	uint32				color = 0;
	int32				mem_type = 0;

	switch (canvas_ptr->data_format)
	{
	case SGL_FORMAT_ARGB:	
		color = line_ptr->color | 0xFF;
		mem_type = 1;
		break;

	case SGL_FORMAT_RGB565:
		color = SGL_RGB888_TO_RGB565(line_ptr->color);
		mem_type = 0;	
		break;

	default:
		break;
	}

	DrawDotLine(line_ptr->begin.x, line_ptr->begin.y, line_ptr->end.x, line_ptr->end.y,
						color, line_ptr->width, canvas_ptr->mem_ptr,
						canvas_ptr->mem_width, canvas_ptr->height, mem_type, NULL);	

	return 0;
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
LOCAL int32 _DrawDashDotLine(SGL_SHAPE_LINE_T	*line_ptr, _CANVAS_CXT_T *canvas_ptr)
{
	uint32				color = 0;
	int32				mem_type = 0;

	switch (canvas_ptr->data_format)
	{
	case SGL_FORMAT_ARGB:	
		color = line_ptr->color | 0xFF;
		mem_type = 1;
		break;

	case SGL_FORMAT_RGB565:
		color = SGL_RGB888_TO_RGB565(line_ptr->color);
		mem_type = 0;	
		break;

	default:
		break;
	}

	DrawDashDotLine(line_ptr->begin.x, line_ptr->begin.y, line_ptr->end.x, line_ptr->end.y,
						color, line_ptr->width, canvas_ptr->mem_ptr,
						canvas_ptr->mem_width, canvas_ptr->height, mem_type, NULL);	

	return 0;
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
LOCAL int32 _CreateLine(_SHAPE_CXT_T *cxt_ptr)
{
	return 0;
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
LOCAL int32 _DrawLine(_SHAPE_CXT_T *cxt_ptr)
{
	SGL_SHAPE_LINE_T	*line_ptr = NULL;
	_CANVAS_CXT_T		*canvas_ptr = NULL;
	
	if (NULL == cxt_ptr)
	{
		return SGL_ERROR;		
	}

	line_ptr = (SGL_SHAPE_LINE_T *)cxt_ptr->shape_ptr;
	canvas_ptr = (_CANVAS_CXT_T *)cxt_ptr->canvas_handle;

	if (NULL == canvas_ptr || NULL == canvas_ptr->mem_ptr)
	{
		return SGL_ERROR;
	}

	switch (line_ptr->style)
	{
	case SGL_LINE_SOLID:
		_DrawSolidLine(line_ptr, canvas_ptr);
		break;

	case SGL_LINE_DASH:
		_DrawDashLine(line_ptr, canvas_ptr);
		break;

	case SGL_LINE_DOT:
		_DrawDotLine(line_ptr, canvas_ptr);
		break;

	case SGL_LINE_DOT_DASH:
		_DrawDashDotLine(line_ptr, canvas_ptr);
		break;

	default:
		return SGL_ERROR;
	}
	
	return 0;
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
LOCAL int32 _DestroyLine(_SHAPE_CXT_T *cxt_ptr)
{
	return 0;
}

/**---------------------------------------------------------------------------*
 **                         Public Functions                                   *
 **---------------------------------------------------------------------------*/

PUBLIC _SHAPE_CB_TBL_T g_shape_line_func = 
{
	NULL,
	_DrawLine,	
	NULL
};

