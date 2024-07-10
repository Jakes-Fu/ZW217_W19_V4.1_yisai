/******************************************************************************
 ** File Name:      graphics_3d_rotate.h									  *
 ** Author:         Tim.Xia                                                   *
 ** DATE:           2010-11-25                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    functions to draw some graphics                            *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2010-11-25          Tim.Xia       Create                                  *
 *****************************************************************************/

#ifndef __GRAPHICS_3D_H__
#define __GRAPHICS_3D_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "img_ref_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32 COLOR32;
typedef uint16 COLOR16;
#if 0
/*****************************************************************************/
//  Description: S2D_LinearInit
//  Note:        
//****************************************************************************/
PUBLIC void S2D_LinearInit(uint16 scr_width, uint16 scr_height);

/*****************************************************************************/
//  Description: S2D_Dynamic_LinearCreate
//  Note:        
//****************************************************************************/
PUBLIC void  S2D_Dynamic_LinearCreate(int16 x,int16 y,uint16 scr_width, uint16 scr_height);

/*****************************************************************************/
//  Description: S2D_LinearUpdate
//  Note:        
//****************************************************************************/
PUBLIC void S2D_LinearUpdate(COLOR16 *target_buf_ptr, uint16 scr_width, uint16 scr_height);

/*****************************************************************************/
//  Description: S2D_LinearDestroy
//  Note:        
//****************************************************************************/
PUBLIC void S2D_LinearDestroy(void);
#endif
/*****************************************************************************/
//  Description: S2D_Blending
//  Note:        
//****************************************************************************/
PUBLIC BOOLEAN S2D_Blending(IMGREF_SIZE_T	bg_window,
								 COLOR16	*bg_buf,
								 IMGREF_SIZE_T	up_window,
								 FIX16_POINT_T	offset,
								 COLOR32	*up_buf,
								 COLOR16	*out_buf);
PUBLIC uint GDI_sqrt(uint x);

/*****************************************************************************/
//  Description: S2D_RippleUpdate
//  Note:        
//****************************************************************************/
PUBLIC void S2D_RippleUpdate(COLOR16 *target_buf_ptr,COLOR16 *bg_buf_ptr ,uint16 scr_width, uint16 scr_height);

/*****************************************************************************/
//  Description: S2D_RippleInit
//  Note:        
//****************************************************************************/
PUBLIC void S2D_RippleInit(void);

/*****************************************************************************/
//  Description: S2D_RippleSet
//  Note:        
//****************************************************************************/
PUBLIC void S2D_RippleSet(IMGREF_POINT_T mid_point);

#ifdef __cplusplus
}
#endif

#endif

