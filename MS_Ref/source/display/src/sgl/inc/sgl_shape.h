/******************************************************************************
 ** File Name:      sgl_shape.h		    							  		  *
 ** Author:          			                                              *
 ** DATE:           2012-01-29                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    SGL shape                					              *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2012-01-29        Shan.He		  Create                                  *
 *****************************************************************************/
#ifndef __SGL_SHAPE_H__
#define __SGL_SHAPE_H__

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
 typedef enum
{
	SHAPE_CB_CREATE = 0,
	SHAPE_CB_DRAW,
	SHAPE_CB_DESTROY
}_SHAPE_CALLBACK_TYPE;

typedef struct _shape_cxt_tag
{
	const char     		*mark;

	SGL_SHAPE_TYPE_E	shape_type;
	void				*shape_ptr;

	SGL_HANDLE_T		scene_handle;
	SGL_HANDLE_T		canvas_handle;		
	
	int32				trans_count;			
	SGL_TRANSFORM_T		*trans_array_ptr;	

	//output parameters
	SGL_RECT2_T			update_rect;
}_SHAPE_CXT_T;

typedef int32 (* SHAPE_CALLBACK)(_SHAPE_CXT_T *);

typedef struct
{
	SHAPE_CALLBACK  	create_func;
	SHAPE_CALLBACK		draw_func;
	SHAPE_CALLBACK		destroy_func;
}_SHAPE_CB_TBL_T;


/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
extern PUBLIC _SHAPE_CB_TBL_T g_shape_line_func;
extern PUBLIC _SHAPE_CB_TBL_T g_shape_polygon_func;
extern PUBLIC _SHAPE_CB_TBL_T g_shape_texture_plane_func;
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
        
#endif
