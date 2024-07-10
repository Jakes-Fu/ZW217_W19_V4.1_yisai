/******************************************************************************
 ** File Name:      sgl_canvas.c	                                          *
 ** Author:         Shan.He                                                   *
 ** DATE:           2012-01-21                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    implemetation of canvas		                              *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2012-01-21         Shan.He         Create                                 *
 *****************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sgl_os_api.h"
#include "sgl_api.h"
#include "sgl_canvas.h"
#include "sgl_color.h"

/**---------------------------------------------------------------------------*
 **                         Macros                                            *
 **---------------------------------------------------------------------------*/
LOCAL const char 	CANVAS_MARK[] 			= "sgl_canvas";
LOCAL const uint8 	MIN_CANVAS_WIDTH 		= 2;
LOCAL const uint8	MIN_CANVAS_HEIGHT 		= 2;
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
LOCAL BOOLEAN _CanvasCheckCxt(_CANVAS_CXT_T *cxt_ptr)
{
	if (NULL != cxt_ptr && cxt_ptr->mark == CANVAS_MARK)
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
LOCAL BOOLEAN _CanvasCheckParam(SGL_CANVAS_T *canvas_ptr)
{
	uint32 bytes_per_pixels = 0;
	
	if (NULL == canvas_ptr)
	{
		return FALSE;
	}

	//check size
	if (canvas_ptr->width < MIN_CANVAS_WIDTH || canvas_ptr->height < MIN_CANVAS_HEIGHT)
	{
		return FALSE;
	}

	//check format
	switch (canvas_ptr->data_format)
	{
	case SGL_FORMAT_ARGB:
	//case SGL_FORMAT_RGB888:
		bytes_per_pixels = 4;
		break;
		
	case SGL_FORMAT_RGB565:
		bytes_per_pixels = 2;
		break;
#if 0
	case SGL_FORMAT_GRAY:
		bytes_per_pixels = 1;
		break;
#endif

	default:
		return FALSE;
	}

	//check memory width
	if (canvas_ptr->mem_width < canvas_ptr->width)
	{
		return FALSE;
	}	

	//check draw rect
	if (canvas_ptr->draw_rect.left < 0 || canvas_ptr->draw_rect.left >= canvas_ptr->width)
	{
		return FALSE;
	}

	if (canvas_ptr->draw_rect.right >= canvas_ptr->width  
		|| canvas_ptr->draw_rect.left >= canvas_ptr->draw_rect.right)
	{
		return FALSE;
	}	

	if (canvas_ptr->draw_rect.top < 0 || canvas_ptr->draw_rect.top >= canvas_ptr->height)
	{
		return FALSE;
	}

	if (canvas_ptr->draw_rect.bottom >= canvas_ptr->height 
		|| canvas_ptr->draw_rect.top >= canvas_ptr->draw_rect.bottom)
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
LOCAL uint32 _GetBytesPerPixel(SGL_DATA_FORMAT_E format)
{
	uint32 bytes_per_pixels = 0;
	
	switch (format)
	{
	case SGL_FORMAT_ARGB:
	//case SGL_FORMAT_RGB888:
		bytes_per_pixels = 4;
		break;
		
	case SGL_FORMAT_RGB565:
		bytes_per_pixels = 2;
		break;
#if 0		
	case SGL_FORMAT_GRAY:
		bytes_per_pixels = 1;
		break;
#endif

	default:
		bytes_per_pixels = 0;
		break;
	}

	return bytes_per_pixels;
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
PUBLIC SGL_HANDLE_T  SGL_CreateCanvas(SGL_CANVAS_T *canvas_ptr)
{
	_CANVAS_CXT_T	*cxt_ptr = NULL;
	void 			*mem_ptr = NULL;
	uint32			bytes_per_pixels = 0;
	
	if (!_CanvasCheckParam(canvas_ptr))
	{
		return NULL;
	}

	bytes_per_pixels = _GetBytesPerPixel(canvas_ptr->data_format);	

	//allocate memory for canvas if needed
	if (NULL == canvas_ptr->mem_ptr)
	{
		mem_ptr = SGL_ALLOC(canvas_ptr->mem_width * canvas_ptr->height 
												   * bytes_per_pixels);
	}		
	else
	{
		mem_ptr = canvas_ptr->mem_ptr;
	}	

	if (NULL == mem_ptr)
	{
		return NULL;
	}

	cxt_ptr = (_CANVAS_CXT_T *)SGL_ALLOC(sizeof(_CANVAS_CXT_T));
	if (cxt_ptr)
	{			
		cxt_ptr->mark = CANVAS_MARK;

		cxt_ptr->width = canvas_ptr->width;
		cxt_ptr->height = canvas_ptr->height;
		cxt_ptr->mem_width = canvas_ptr->mem_width;		
		cxt_ptr->data_format = canvas_ptr->data_format;
		cxt_ptr->draw_rect = canvas_ptr->draw_rect;
		
		cxt_ptr->mem_ptr = mem_ptr;
		cxt_ptr->bytes_per_pixel = bytes_per_pixels;
	}
	else
	{
		if (canvas_ptr->mem_ptr != mem_ptr)
		{
			SGL_FREE(mem_ptr);
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
PUBLIC int32 SGL_ClearCanvas(SGL_HANDLE_T canvas_handle,  uint32 clear_color)
{
	_CANVAS_CXT_T	*cxt_ptr = (_CANVAS_CXT_T *)canvas_handle;

	if (!_CanvasCheckCxt(cxt_ptr))
	{
		return SGL_ERROR;
	}

	switch (cxt_ptr->data_format)
	{
	case SGL_FORMAT_ARGB:
		{
			uint8 a = SGL_ARGB_GET_A(clear_color);
			uint8 r = SGL_ARGB_GET_R(clear_color);
			uint8 g = SGL_ARGB_GET_G(clear_color);
			uint8 b = SGL_ARGB_GET_B(clear_color);
			
			if ((a == r) && (a == g) && (a == b))
			{
				SGL_MEMSET(cxt_ptr->mem_ptr, a, cxt_ptr->mem_width 
								* cxt_ptr->height * cxt_ptr->bytes_per_pixel);
			}
			else
			{
				uint32 	*ptr = cxt_ptr->mem_ptr;
				int32 	pixels = cxt_ptr->mem_width * cxt_ptr->height;

				while (pixels--)
				{
					*ptr++ = clear_color;
				}				
			}
		}
		break;

#if 0
	case SGL_FORMAT_RGB888:
		{
			uint32 	*ptr = cxt_ptr->mem_ptr;
			int32 	pixels = cxt_ptr->mem_width * cxt_ptr->height;

			clear_color |= 0xff000000;

			while (pixels--)
			{
				*ptr++ = clear_color;
			}							
		}		
		break;
#endif

	case SGL_FORMAT_RGB565:
		if ((clear_color & 0xff) == ((clear_color >> 8) & 0xff))
		{
			SGL_MEMSET(cxt_ptr->mem_ptr, (uint8)(clear_color & 0xff),
				cxt_ptr->mem_width * cxt_ptr->height * cxt_ptr->bytes_per_pixel);
		}
		else
		{
			uint16 	*ptr = cxt_ptr->mem_ptr;
			int32 	pixels = cxt_ptr->mem_width * cxt_ptr->height;

			while (pixels--)
			{
				*ptr++ = clear_color;
			}
		}			
		break;	

#if 0
	case SGL_FORMAT_GRAY:
		SGL_MEMSET(cxt_ptr->mem_ptr, (uint8)clear_color,
					cxt_ptr->mem_width * cxt_ptr->height);		
		break;
#endif

	default:
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
PUBLIC int32 SGL_DestroyCanvas(SGL_HANDLE_T canvas_handle)
{
	_CANVAS_CXT_T	*cxt_ptr = (_CANVAS_CXT_T *)canvas_handle;

	if (_CanvasCheckCxt(cxt_ptr))
	{
		SGL_FREE(cxt_ptr);
		return SGL_SUCCESS;
	}
	else
	{
		return SGL_ERROR;
	}
}
