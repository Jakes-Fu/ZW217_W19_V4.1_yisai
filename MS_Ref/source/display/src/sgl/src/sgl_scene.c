/******************************************************************************
 ** File Name:      sgl_scene.c	                                              *
 ** Author:         Shan.He                                                   *
 ** DATE:           2012-01-21                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    implemetation of scene		                              *
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
#include "sgl_scene.h"

/**---------------------------------------------------------------------------*
 **                         Macros                                            *
 **---------------------------------------------------------------------------*/
LOCAL const char SCENE_MARK[] = "sgl_scene";
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
 LOCAL BOOLEAN _SceneCheckCxt(_SCENE_CXT_T *cxt_ptr)
{
	if (NULL != cxt_ptr && cxt_ptr->mark == SCENE_MARK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
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
PUBLIC SGL_HANDLE_T	 SGL_CreateScene(SGL_SCENE_T *scene_ptr)
{
	_SCENE_CXT_T	*cxt_ptr = NULL;
	
	if (NULL == scene_ptr)
	{
		return NULL;
	}

	cxt_ptr = (_SCENE_CXT_T *)SGL_ALLOC(sizeof(_SCENE_CXT_T));
	if (cxt_ptr)
	{
		cxt_ptr->mark = SCENE_MARK;
		cxt_ptr->camera = scene_ptr->camera;
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
PUBLIC int32 SGL_DestroyScene(SGL_HANDLE_T scene_handle)
{
	_SCENE_CXT_T	*cxt_ptr = (_SCENE_CXT_T *)scene_handle;

	if (_SceneCheckCxt(cxt_ptr))
	{
		SGL_FREE(cxt_ptr);
		return SGL_SUCCESS;
	}
	else
	{
		return SGL_ERROR;
	}
}
