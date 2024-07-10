#ifndef _DRAW_LINE_H_
#define _DRAW_LINE_H_

#include "sci_types.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef struct
{
    int16   left;
    int16   top;
    int16   right;
    int16   bottom;
}GUI_RECT_T;

void DrawThickLineAAU32(int16           x1,             //x coordinate of first point
                        int16           y1,             //y coordinage of first point
                        int16           x2,             //x coordinate of second point
                        int16           y2,             //y coordinate of sconcd point
                         uint32           color,           //color to draw.
                         uint16           width,           //unit: pixel
                         uint32           *mem_ptr,
                         uint16           mem_width,
                         uint16           mem_height,
                         GUI_RECT_T       *update_rect_ptr);

void DrawThickLineAA(int16           x1,             //x coordinate of first point
                     int16           y1,             //y coordinage of first point
                     int16           x2,             //x coordinate of second point
                     int16           y2,             //y coordinate of sconcd point
                     uint32           color,           //color to draw.
                     uint16           width,           //unit: pixel
                     uint16           *mem_ptr,
                     uint16           mem_width,
                     uint16           mem_height,
                     GUI_RECT_T       *update_rect_ptr);

void DrawDashLine(int16           x1,             //x coordinate of first point
                  int16           y1,             //y coordinage of first point
                  int16           x2,             //x coordinate of second point
                  int16           y2,             //y coordinate of sconcd point
                  uint32           color,           //color to draw.
                  uint16           width,           //unit: pixel
                  uint16           *mem_ptr,
                  uint16           mem_width,
                  uint16           mem_height,
                  uint32           mem_type,         //0: rgb565, 1: argb888
                  GUI_RECT_T       *update_rect_ptr);


void DrawDotLine(int16           x1,             //x coordinate of first point
                  int16           y1,             //y coordinage of first point
                  int16           x2,             //x coordinate of second point
                  int16           y2,             //y coordinate of sconcd point
                  uint32           color,           //color to draw.
                  uint16           width,           //unit: pixel
                  uint16           *mem_ptr,
                  uint16           mem_width,
                  uint16           mem_height,
                  uint32           mem_type,         //0: rgb565, 1: argb888
                  GUI_RECT_T       *update_rect_ptr);

void DrawDashDotLine(int16           x1,             //x coordinate of first point
                      int16           y1,             //y coordinage of first point
                      int16           x2,             //x coordinate of second point
                      int16           y2,             //y coordinate of sconcd point
                      uint32           color,           //color to draw.
                      uint16           width,           //unit: pixel
                      uint16           *mem_ptr,
                      uint16           mem_width,
                      uint16           mem_height,
                      uint32           mem_type,         //0: rgb565, 1: argb888
                      GUI_RECT_T       *update_rect_ptr                      
                      );
#ifdef __cplusplus
    }
#endif

#endif