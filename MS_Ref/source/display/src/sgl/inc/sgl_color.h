/******************************************************************************
 ** File Name:      sgl_color.h		    							  		  *
 ** Author:          			                                              *
 ** DATE:           2012-01-29                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Macors of color operations                 		          *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            		 DESCRIPTION                      *    
 ** 2012-01-29        Shan.He		         Create                           *
 *****************************************************************************/
#ifndef __SGL_COLOR_H__
#define __SGL_COLOR_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sgl_types.h"

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
 #define SGL_MAKE_ARGB(_a, _r, _g, _b) 	(((_a) << 24) | (((_r) & 0xff) << 16) | (((_g) & 0xff) << 8) | ((_b) & 0xff))
 #define SGL_MAKE_RGB888(_r, _g, _b) 	((0xff << 24) | (((_r) & 0xff) << 16) | (((_g) & 0xff) << 8) | ((_b) & 0xff))
 #define SGL_MAKE_RGB565(_r, _g, _b) 	((((_r) & 0xf8) << 8) | (((_g) & 0xfe) << 3) | (((_b) & 0xf8) >> 3))
 
#define SGL_RGB565_GET_R(_color) 	(((_color) >> 8) & 0xf8)
#define SGL_RGB565_GET_G(_color) 	(((_color) >> 3) & 0xfc)
#define SGL_RGB565_GET_B(_color) 	(((_color) << 3) & 0xf8)

#define SGL_RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define SGL_RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define SGL_RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define SGL_ARGB_GET_A(_color) 		((uint8)((_color) >> 24))
#define SGL_ARGB_GET_R(_color) 		((uint8)((_color) >> 16))
#define SGL_ARGB_GET_G(_color) 		((uint8)((_color) >> 8))
#define SGL_ARGB_GET_B(_color) 		((uint8)(_color))

#define SGL_ARGB_SET_R(_color, _r) 	((_color) | ((_r) << 16))
#define SGL_ARGB_SET_G(_color, _g) 	((_color) | ((_g) << 8))
#define SGL_ARGB_SET_B(_color, _b) 	((_color) | (_b))

#define SGL_RGB888_GET_R			SGL_ARGB_GET_R
#define SGL_RGB888_GET_G			SGL_ARGB_GET_G
#define SGL_RGB888_GET_B			SGL_ARGB_GET_B

#define SGL_RGB888_SET_R			SGL_ARGB_SET_R
#define SGL_RGB888_SET_G			SGL_ARGB_SET_G
#define SGL_RGB888_SET_B			SGL_ARGB_SET_B

#define SGL_RGB888_TO_RGB565(_c)	((((_c) >> 8) & 0xf800) | (((_c) >> 5) & 0x7e0) | (((_c) >> 3) & 0x7f))

#define SGL_ALPHA_BLENDING(_x1, _x2, _a) (((((_x1) - (_x2)) * (_a)) >> 8) + (_x2))
/**---------------------------------------------------------------------------*
 **                         Data Types                                        *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               * 
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
        
#endif
