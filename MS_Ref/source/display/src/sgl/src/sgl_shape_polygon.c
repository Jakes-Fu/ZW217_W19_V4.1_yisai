/******************************************************************************
 ** File Name:      sgl_shape_ploygen.c	                                      *
 ** Author:         Shan.He                                                   *
 ** DATE:           2012-02-01                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    implemetation of ploygen		                          *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2012-02-01        Shan.He         Create                                  *
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
/**---------------------------------------------------------------------------*
 **                         Structures                                        *
 **---------------------------------------------------------------------------*/
#define MAX_POLYGON_VERTEX_NUM			16
#define MIN_POLYGON_VERTEX_NUM			3
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
LOCAL int32 _CheckParam(SGL_SHAPE_POLYGON_T *pgn_ptr)
{
	if (NULL == pgn_ptr)
	{
		return SGL_ERROR;
	}

	if (NULL == pgn_ptr->vertex_ptr || pgn_ptr->vertex_num < MIN_POLYGON_VERTEX_NUM
		|| pgn_ptr->vertex_num > MAX_POLYGON_VERTEX_NUM)
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
LOCAL int32 _FillPolygon(SGL_SHAPE_POLYGON_T *pgn_ptr, _CANVAS_CXT_T *canvas_ptr)
{
	FILL_POLYGON_IN_T 	pgn_fill_in = {0};
	FILL_POLYGON_OUT_T	pgn_fill_out = {0};
	int32				i = 0;
	IMGREF_POINT_T		*point_ptr = NULL;
	int32				ret = SGL_ERROR;

	point_ptr = SGL_ALLOC(pgn_ptr->vertex_num * sizeof(IMGREF_POINT_T));
	if (NULL == point_ptr)
	{
		return SGL_ERROR;
	}

	for (i=0; i<pgn_ptr->vertex_num; i++)
	{
		point_ptr[i].x = pgn_ptr->vertex_ptr[i].x;
		point_ptr[i].y = pgn_ptr->vertex_ptr[i].y;
	}				

	pgn_fill_in.points_ptr = point_ptr;
	pgn_fill_in.pt_num = pgn_ptr->vertex_num;
	pgn_fill_in.clip_rect.x = canvas_ptr->draw_rect.left;
	pgn_fill_in.clip_rect.y = canvas_ptr->draw_rect.top;
	pgn_fill_in.clip_rect.w = canvas_ptr->draw_rect.right - canvas_ptr->draw_rect.left + 1;
	pgn_fill_in.clip_rect.h = canvas_ptr->draw_rect.bottom - canvas_ptr->draw_rect.top + 1;
	pgn_fill_in.img_chn.chn0.ptr = canvas_ptr->mem_ptr;
	pgn_fill_in.img_chn.chn0.size = canvas_ptr->mem_width * canvas_ptr->height * canvas_ptr->bytes_per_pixel;
	pgn_fill_in.img_size.w = canvas_ptr->mem_width;
	pgn_fill_in.img_size.h = canvas_ptr->height;
	pgn_fill_in.u32_color = pgn_ptr->color.fill_color;
	
	switch (canvas_ptr->data_format)
	{
	case SGL_FORMAT_ARGB:
		pgn_fill_in.data_format = IMGREF_FORMAT_ARGB888;
		break;
		
	case SGL_FORMAT_RGB565:
		pgn_fill_in.data_format = IMGREF_FORMAT_RGB565;
		break;

	default:
		goto _FILL_PGN_EXIT;
	}


	if (!GRAPH_FillPolygon(&pgn_fill_in, &pgn_fill_out))
	{
		goto _FILL_PGN_EXIT;	
	}

	ret = SGL_SUCCESS;
	
_FILL_PGN_EXIT:

	SGL_FREE(point_ptr);

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
LOCAL int32 _CreatePolygon(_SHAPE_CXT_T *cxt_ptr)
{
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
LOCAL int32 _DrawPolygon(_SHAPE_CXT_T *cxt_ptr)
{
	SGL_SHAPE_POLYGON_T		*pgn_ptr = NULL;
	_CANVAS_CXT_T			*canvas_ptr = NULL;
	int32					ret = SGL_ERROR;
	
	if (NULL == cxt_ptr)
	{
		return SGL_ERROR;		
	}

	pgn_ptr = (SGL_SHAPE_POLYGON_T *)cxt_ptr->shape_ptr;
	canvas_ptr = (_CANVAS_CXT_T *)cxt_ptr->canvas_handle;

	if (NULL == canvas_ptr || NULL == canvas_ptr->mem_ptr || NULL == pgn_ptr)
	{
		return SGL_ERROR;
	}

	ret = SGL_ERROR;
	switch (pgn_ptr->style)
	{
	case SGL_PGN_FILL:
		ret = _FillPolygon(pgn_ptr, canvas_ptr);
		break;

	default:
		break;
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
LOCAL int32 _DestroyPolygon(_SHAPE_CXT_T *cxt_ptr)
{
	return SGL_SUCCESS;
}

/**---------------------------------------------------------------------------*
 **                         Public Functions                                   *
 **---------------------------------------------------------------------------*/

PUBLIC _SHAPE_CB_TBL_T g_shape_polygon_func = 
{
	NULL,
	_DrawPolygon,	
	NULL
};

