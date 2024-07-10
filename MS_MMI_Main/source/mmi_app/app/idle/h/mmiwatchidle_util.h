#ifndef MMIWATCHIDLE_UTIL_H
#define MMIWATCHIDLE_UTIL_H

#include "mmk_type.h"
#include "window_parse.h"
#include "mmi_wallpaper_export.h"
#include "mmitheme_pos.h"
#include "mmi_position.h"
#include "graphics_draw.h"
#include "Dal_time.h"
#include "mmk_timer.h"
//#include "mmimenu_export.h"

#ifdef ANALOG_CLK_SUPPORT  //yangyu add
//If define btwatch macro, then complie code under.

#ifndef ARGB_GET_A
#define ARGB_GET_A(_argb) (((_argb) >> 24) & 0xff)
#endif
#ifndef ARGB_GET_R
#define ARGB_GET_R(_argb) (((_argb) >> 16) & 0xff)
#endif
#ifndef ARGB_GET_G
#define ARGB_GET_G(_argb) (((_argb) >> 8) & 0xff)
#endif
#ifndef ARGB_GET_B
#define ARGB_GET_B(_argb) ((_argb) & 0xff)
#endif

#ifndef RGB888_TO_RGB565
#define RGB888_TO_RGB565(r, g, b)  (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | ((b >> 3) & 0x1f))
#endif

#ifndef BLEND_MASK
#define BLEND_MASK 0x7E0F81F
#endif

typedef struct 
{
    void   *data_ptr;
    uint32  data_type;  //0: RGB565, 1: ARGB888
    uint16  width;
    uint16  height;
    int16   start_x;
    int16   start_y;
}MMI_WATCHIDLE_CLOCK_ALPHA_IMG_T; /*lint !e770*/

BOOLEAN ProcessAlphaBlending(MMI_WATCHIDLE_CLOCK_ALPHA_IMG_T *bk_img_ptr, 
MMI_WATCHIDLE_CLOCK_ALPHA_IMG_T *fore_img_ptr);

uint16 BlendRGB565(uint16 bk_color, uint16 fore_color, uint8 alpha);

#endif
#endif