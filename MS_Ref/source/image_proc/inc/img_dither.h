/******************************************************************************
 ** File Name:      img_dither.h                                              *
 ** Author:                                                                   *
 ** DATE:             2011-9-10                                               *
 ** Copyright:       	2010 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     	common types of image reference module                *
 ** Note:                                                                     *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                        *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                            * 
 *****************************************************************************/

#ifndef _IMG_DITHER_H_
#define _IMG_DITHER_H_

#include "sci_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SPRD_R16_BITS     5
#define SPRD_G16_BITS     6
#define SPRD_B16_BITS     5

#define SPRD_R16_SHIFT    (SPRD_B16_BITS + SPRD_G16_BITS)
#define SPRD_G16_SHIFT    (SPRD_B16_BITS)
#define SPRD_B16_SHIFT    0

#define SPRD_R16_MASK     ((1 << SPRD_R16_BITS) - 1)
#define SPRD_G16_MASK     ((1 << SPRD_G16_BITS) - 1)
#define SPRD_B16_MASK     ((1 << SPRD_B16_BITS) - 1)

#define GetPackedR16(color)   (((uint32)(color) >> SPRD_R16_SHIFT) & SPRD_R16_MASK)
#define GetPackedG16(color)   (((uint32)(color) >> SPRD_G16_SHIFT) & SPRD_G16_MASK)
#define GetPackedB16(color)   (((uint32)(color) >> SPRD_B16_SHIFT) & SPRD_B16_MASK)
 
#define PackRGB16(r, g, b) (uint16)((r << SPRD_R16_SHIFT)   \
                    | (g << SPRD_G16_SHIFT) | (b << SPRD_B16_SHIFT))


#define SPRD_R32_BITS     8
#define SPRD_G32_BITS     8
#define SPRD_B32_BITS     8

#define SPRD_R32_SHIFT    0
#define SPRD_G32_SHIFT    8
#define SPRD_B32_SHIFT    16

#define SPRD_R32_MASK     ((1 << SPRD_R32_BITS) - 1)
#define SPRD_G32_MASK     ((1 << SPRD_G32_BITS) - 1)
#define SPRD_B32_MASK     ((1 << SPRD_B32_BITS) - 1)

#define DITHER_R32_FOR_565(r, d)  (r + d - (r >> 5))
#define DITHER_G32_FOR_565(g, d)  (g + (d >> 1) - (g >> 6))
#define DITHER_B32_FOR_565(b, d)  (b + d - (b >> 5))

#define SPRD_R32ToR16(r)   ((uint32)(r) >> (SPRD_R32_BITS - SPRD_R16_BITS))
#define SPRD_G32ToG16(g)   ((uint32)(g) >> (SPRD_G32_BITS - SPRD_G16_BITS))
#define SPRD_B32ToB16(b)   ((uint32)(b) >> (SPRD_B32_BITS - SPRD_B16_BITS))

#ifdef __cplusplus
}
#endif

#endif
