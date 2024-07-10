#ifndef _DRAW_DOT_H_
#define _DRAW_DOT_H_

#include "sci_types.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

void DrawDotU32(int16 x, int16 y, uint8 pen_width, uint32 pen_color,
                uint32 *mem_ptr, uint16 mem_width, uint16 mem_height);

void DrawDot(int16 x, int16 y, uint8 pen_width, uint16 pen_color,
             uint16 *mem_ptr, uint16 mem_width, uint16 mem_height);

#ifdef __cplusplus
    }
#endif

#endif