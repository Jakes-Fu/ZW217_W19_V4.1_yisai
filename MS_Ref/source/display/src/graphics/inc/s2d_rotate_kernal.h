/******************************************************************************
 ** File Name:      s2d_rotate_kernal.h                                       *
 ** Author:                                                                   *
 ** DATE:           2011-11-18                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    image rotate kernal functions                             *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *                                                            *
 *****************************************************************************/
#ifndef _S2D_ROTATE_KERNAL_H_
#define _S2D_ROTATE_KERNAL_H_

 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

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
#include "sci_types.h"
#include "img_ref_types.h"
#include "s2d.h"
#include "s2d_rotate.h"

typedef struct
{
	S2D_IMG_T		src_img;
	GDI_POINT_T		src_rot_center;	
	
	S2D_IMG_T		dst_img;
	GDI_POINT_T		dst_rot_center;
	int32			rot_angle;
	
	void			*scanline_buf;
	int32			scanline_num;

	S2D_IMG_T		rotated_img;
	GDI_POINT_T		rotated_img_offset;
	GDI_POINT_T		rotated_img_center;

	int32 			mode;
	int32			speed_up;

	int32			sin_value;
	int32			cos_value;
}ROT_CXT_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               * 
 **---------------------------------------------------------------------------*/
 /******************************************************************************/
//  Description: draw the rotated RGB565 image
//  Author: 	
//  Input:  
//			 rot_cxt_ptr:  pinter of the rotation contex
//  Output:  None
//  Return:	
//			TRUE: 	successful
//			FALSE:	failed
//  Note:  
/******************************************************************************/
PUBLIC BOOLEAN S2D_DrawRGB565Polygen(ROT_CXT_T *rot_cxt_ptr);

 /******************************************************************************/
//  Description: draw the rotated RGB565 image
//  Author: 	
//  Input:  
//			 rot_cxt_ptr:  pinter of the rotation contex
//  Output:  None
//  Return:	
//			TRUE: 	successful
//			FALSE:	failed
//  Note:  
/******************************************************************************/
PUBLIC BOOLEAN S2D_DrawRGB565PolygenEx(ROT_CXT_T *rot_cxt_ptr);

/******************************************************************************/
//  Description: draw the rotated RGB565 image
//  Author: 	
//  Input:  
//			 rot_cxt_ptr:  pinter of the rotation contex
//  Output:  None
//  Return:	
//			TRUE: 	successful
//			FALSE:	failed
//  Note:  
/******************************************************************************/
PUBLIC BOOLEAN S2D_DrawRGB565PolygenToARGB888(ROT_CXT_T *rot_cxt_ptr);

/******************************************************************************/
//  Description: draw the rotated ARGB888 image
//  Author: 	
//  Input:  
//			 rot_cxt_ptr:  pinter of the rotation contex
//  Output:  None
//  Return:	
//			TRUE: 	successful
//			FALSE:	failed
//  Note:  
/******************************************************************************/
PUBLIC BOOLEAN S2D_DrawARGB888Polygen(ROT_CXT_T *rot_cxt_ptr);

/******************************************************************************/
//  Description: draw the rotated ARGB888 image
//  Author: 	
//  Input:  
//			 rot_cxt_ptr:  pinter of the rotation contex
//  Output:  None
//  Return:	
//			TRUE: 	successful
//			FALSE:	failed
//  Note:  
/******************************************************************************/
PUBLIC BOOLEAN S2D_DrawARGB888PolygenEx(ROT_CXT_T *rot_cxt_ptr);

/******************************************************************************/
//  Description: draw the rotated ARGB888 image
//  Author: 	
//  Input:  
//			 rot_cxt_ptr:  pinter of the rotation contex
//  Output:  None
//  Return:	
//			TRUE: 	successful
//			FALSE:	failed
//  Note:  
/******************************************************************************/
PUBLIC BOOLEAN S2D_DrawARGB888Polygen_Blender(ROT_CXT_T *rot_cxt_ptr);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
