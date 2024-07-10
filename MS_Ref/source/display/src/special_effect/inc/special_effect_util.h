/******************************************************************************
 ** File Name:    special_effect_util.h                                             *
 ** Author:       Allen.Mao                                                   *
 ** DATE:         06/12/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *

 ******************************************************************************/

#ifndef _SPECIAL_EFFECT_UTIL_H_
#define _SPECIAL_EFFECT_UTIL_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "special_effect.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/

// fixed number : 16.16
#define fx32    int32
#define EFFECT_FX32_TO_INT(fx)  ((fx) >> 16)
#define EFFECT_INT_TO_FX32(i)   ((i) << 16)

//
#define SE_MIN(a,b)   ((a) < (b) ? (a):(b))
#define SE_ABS(a)     ((a) > 0 ? (a): -(a))

// RGB565
#define SE_RGB16_0(r)   ((uint16)( (((uint16)(r) >> 3) << 11) )) 
#define SE_RGB16_1(r,g) ((uint16)( (((uint16)(r) >> 3) << 11)     \
                                       | (((uint16)(g) >>2) << 5) )) 
#define SE_RGB16(r,g,b) ((uint16)( (((uint16)(r) >> 3) << 11)      \
                                       | (((uint16)(g) >>2) << 5)  \
                                       | (((uint16)(b) >>3) >>1) )) 
// ARGB888
#define SE_A32_bits   24
#define SE_R32_bits   16
#define SE_G32_bits   8
#define SE_B32_bits   0

#define SE_ARGB32(a,r,g,b)              ( \
                                            ((uint32)(a) << 24) | \
                                            ((uint32)(r) << 16) | \
                                            ((uint32)(g) << 8)  | \
                                            ((uint32)(b)     ))   

// handle tag
#define SE_HANDLE_TAG  0x20836920    //  " SE "


#define SE_ARRAY_COUNT(array)   (sizeof(array) / sizeof(array[0]))
#define SE_FIX32_TO_INT(f)      ((f + 32767) >> 16)
/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

// func ops interface
typedef struct SE_effect_func_tag
{
    SE_HANDLE   (*create) (void *);    
    SE_RESULT_E (*render) (SE_HANDLE , void *,void *);
    SE_RESULT_E (*destory)(SE_HANDLE );        
}SE_EFFECT_FUNC_T;

typedef struct SE_rect_tag {
    int16 x, y;
    uint16 w, h;
}SE_EFFECT_RECT_T;

// ARGB surface
typedef struct SE_surface_tag
{
    IMGREF_DATA_FORMAT_E  format;      
    void *base; 
    uint16 width;
    uint16 height;
    uint16 pitch;    
}SE_EFFECT_SURFACE_T;

#define SE_AMASK 0xFF000000
#define SE_RMASK 0xFF0000
#define SE_GMASK 0xFF00
#define SE_BMASK 0xFF

#define SE_ASHIFT 24
#define SE_RSHIFT 16
#define SE_GSHIFT 8
#define SE_BSHIFT 0

#define SE_RMASK16 0xF800
#define SE_GMASK16 0x7E0
#define SE_BMASK16 0x1F


#define SE_RSHIFT16 11
#define SE_GSHIFT16 5
#define SE_BSHIFT16 0

/*!
\brief Unwrap RGBA16 values from a pixel using mask, shift and loss for surface.
*/
#define SE_RGB_FROM_PIXEL16(pixel, r, g, b)                \
    {                                    \
    r = ((pixel & SE_RMASK16) >> (SE_RSHIFT16 - 3));         \
    g = ((pixel & SE_GMASK16) >> (SE_GSHIFT16 - 2));         \
    b = ((pixel & SE_BMASK16) << 3);         \
    }

/*!
\brief Unwrap RGB values from a pixel using mask, shift and loss for surface.
*/
#define SE_RGB_FROM_PIXEL(pixel, r, g, b)                \
    {                                    \
    r = ((pixel & SE_RMASK) >> SE_RSHIFT);         \
    g = ((pixel & SE_GMASK) >> SE_GSHIFT);         \
    b = ((pixel & SE_BMASK) >> SE_BSHIFT);         \
    }

/*!
\brief Unwrap RGBA values from a pixel using mask, shift and loss for surface.
*/


#define SE_RGBA_FROM_PIXEL(pixel, r, g, b, a)                \
    {                                    \
    r = ((pixel & SE_RMASK) >> SE_RSHIFT);         \
    g = ((pixel & SE_GMASK) >> SE_GSHIFT);         \
    b = ((pixel & SE_BMASK) >> SE_BSHIFT);         \
    a = ((pixel & SE_AMASK) >> SE_ASHIFT);         \
    }


/*!
\brief Wrap a pixel16 from RGB values using mask, shift and loss for surface.
*/
#define SE_PIXEL16_FROM_RGB(pixel, r, g, b)                \
    {                                    \
    pixel =  (uint16)(((r  >> 3) << SE_RSHIFT16)|                \
        ((g >> 2) << SE_GSHIFT16)|                \
        ((b >> 3) << SE_BSHIFT16));                \
    }


/*!
\brief Wrap a pixel from RGBA values using mask, shift and loss for surface.
*/
#define SE_PIXEL_FROM_RGBA(pixel, r, g, b, a)                \
    {                                    \
    pixel = (r << SE_RSHIFT)|                \
    (g << SE_GSHIFT)|                \
    (b << SE_BSHIFT)|                \
    (a << SE_ASHIFT);                \
    }

/*!
\brief Assemble pixel into buffer pointer from separate RGB values.
*/
#define SE_ASSEMBLE_RGB16(buf, r, g, b)            \
    {                                    \
    uint16 pixel;                    \
    \
    SE_PIXEL16_FROM_RGB(pixel, r, g, b);    \
    *((uint16 *)(buf)) = pixel;            \
    }

/*!
\brief Assemble pixel into buffer pointer from separate RGB values.
*/
#define SE_ASSEMBLE_RGB(buf, r, g, b)            \
    {                                    \
    uint32 pixel;                    \
    \
    SE_PIXEL_FROM_RGBA(pixel, r, g, b, 0xFF);    \
    *((uint32 *)(buf)) = pixel;            \
    }

/*!
\brief Assemble pixel into buffer pointer from separate RGBA values.
*/
#define SE_ASSEMBLE_RGBA(buf, r, g, b, a)            \
    {                                    \
    Uint32 pixel;                    \
    \
    SE_PIXEL_FROM_RGBA(pixel, r, g, b, a);    \
    *((uint32 *)(buf)) = pixel;            \
    }

/*!
\brief Blend the RGB values of two pixels based on a source alpha value.
*/
#define SE_ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB)    \
    do {                        \
    dR = (((sR-dR)*(A+1)) >> 8)+dR;        \
    dG = (((sG-dG)*(A+1)) >> 8)+dG;        \
    dB = (((sB-dB)*(A+1)) >> 8)+dB;        \
    } while(0)

/*!
\brief 4-times unrolled DUFFs loop.

This is a very useful loop for optimizing blitters.
*/
#define GFX_DUFFS_LOOP4(pixel_copy_increment, width)            \
    { int n = (width+3)/4;                            \
    switch (width & 3) {                        \
    case 0: do {    pixel_copy_increment;                \
    case 3:        pixel_copy_increment;                \
    case 2:        pixel_copy_increment;                \
    case 1:        pixel_copy_increment;                \
    } while ( --n > 0 );                    \
    }                                \
    }


/**----------------------------------------------------------------------------*
**                         Functions                                          **
**----------------------------------------------------------------------------*/

/******************************************************************************
// Description: set random seed
// Input:        seed  : int32
// Output:      None
// Return:        None
// Note:
********************************************************************************/ 
// 
PUBLIC void  SE_srand(uint32 seed) ;

/******************************************************************************
// Description: generate random number , not reetant
// Input:        
// Output:      None
// Return:        random number
// Note:
********************************************************************************/ 
PUBLIC int32 SE_rand(void) ;



/******************************************************************************
// Description: fix 32 sin 
// Input:        index : uint32 
// Output:      None
// Return:        fx32, sin value
// Note:        index >= 0
********************************************************************************/ 

PUBLIC int32 SE_fsin(uint32 index);

#define SE_fsin_safe(index)  do {                             \
                                if (index < 0) {              \
                                    index = index % 512 + 512;\
                                }                             \
                                SE_fsin(index);               \
                             }while(0)

/******************************************************************************
// Description: boxColor  (fill rectangle with ARGB color)
// Input:        
// Output:      
// Return:      
// Note:        will not check parameters, for efficiency
********************************************************************************/ 
PUBLIC int32 SE_FillRect(SE_EFFECT_SURFACE_T *surface,
                        int32 x1, int32 y1, int32 x2, int32 y2, 
                        uint32 color);


/******************************************************************************
// Description: clear surface  ( with ARGB color)
// Input:        
// Output:      
// Return:      
// Note:        will not check parameters, for efficiency
********************************************************************************/ 
PUBLIC int32 SE_ClearSurface(SE_EFFECT_SURFACE_T *surface,
                        uint32 color);


PUBLIC int32 SE_BitBlit(SE_EFFECT_SURFACE_T * src, SE_EFFECT_RECT_T * srcrect, 
                SE_EFFECT_SURFACE_T * dst, SE_EFFECT_RECT_T * dstrect);

PUBLIC int32 SE_Alphablend(SE_EFFECT_SURFACE_T * src, SE_EFFECT_RECT_T * srcrect, 
                      SE_EFFECT_SURFACE_T * dst, SE_EFFECT_RECT_T * dstrect,
                      uint8 alpha);

#ifdef __cplusplus
    }
#endif

#endif



