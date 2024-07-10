#include "sci_types.h"
#include "draw_dot.h"

#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

#define RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define ARGB_GET_R(_color) ((uint8)((_color) >> 16))
#define ARGB_GET_G(_color) ((uint8)((_color) >> 8))
#define ARGB_GET_B(_color) ((uint8)(_color))

#define ARGB_SET_R(_color, _r) ((_color) | ((_r) << 16))
#define ARGB_SET_G(_color, _g) ((_color) | ((_g) << 8))
#define ARGB_SET_B(_color, _b) ((_color) | (_b))

#define ALPHA_BLENDING(_x1, _x2, _a) (((((_x1) - (_x2)) * (_a)) >> 8) + (_x2))

#define DOT_MIN_WIDTH 1
#define DOT_MAX_WIDTH 7

const LOCAL uint8 s_dot_tbl[DOT_MAX_WIDTH][DOT_MAX_WIDTH * DOT_MAX_WIDTH] = 
{
    // pen_width = 1
    {
        192
    },

    // pen_width = 2
    {
        208,    208,
        208,    208
    },

     // pen_width = 3
    {
        128,    240,    128,    
        240,    255,    240,    
        128,    240,    128
    },

    // pen_width = 4
    {
        96,     224,    224,    96,
        224,    255,    255,    224,
        224,    255,    255,    224,
        96,     224,    224,    96,
    },

    // pen_width = 5
    {
        32,     192,    248,    192,     32,
        192,    255,    255,    255,    192,
        248,    255,    255,    255,    248,
        192,    255,    255,    255,    192,
        32,     192,    248,    192,     32
    },

    // pen_width = 6
    {
        16,     144,    240,    240,     144,   16,
        144,    255,    255,    255,     255,   144,
        240,    255,    255,    255,     255,   240,
        240,    255,    255,    255,     255,   240,
        144,    255,    255,    255,     255,   144,
        16,     144,    240,    240,     144,   16
    },

    // pen_width = 7
    {
        0,      144,    224,    240,     224,   144,    0,
        144,    255,    255,    255,     255,   255,    144,
        224,    255,    255,    255,     255,   255,    224,
        240,    255,    255,    255,     255,   255,    240,
        224,    255,    255,    255,     255,   255,    224,
        144,    255,    255,    255,     255,   255,    144,
        0,      144,    224,    240,     224,   144,    0
    }
};

void DrawDotU32(int16 x, int16 y, uint8 pen_width, uint32 pen_color,
                uint32 *mem_ptr, uint16 mem_width, uint16 mem_height)
{
    const uint8 *dot_tbl_ptr = NULL;
    int16 dot_x = 0;
    int16 dot_y = 0;
    int16 dot_w = 0;
    int16 dot_h = 0;
    uint16 i, j;

    if (NULL == mem_ptr)
    {
        return;
    }
    
    pen_width = (pen_width < DOT_MIN_WIDTH) ? DOT_MIN_WIDTH : pen_width;
    pen_width = (pen_width > DOT_MAX_WIDTH) ? DOT_MAX_WIDTH : pen_width;

    dot_w = pen_width;
    dot_h = pen_width;
        
    dot_tbl_ptr = s_dot_tbl[pen_width - 1];
   
    if (x < 0)
    {
        dot_x = -x;
        dot_w -= dot_x;      
        x = 0;
    }

    if (y < 0)
    {
        dot_y = -y;
        dot_h -= dot_y;
        y = 0;
    }

    if (x + dot_w > mem_width)
    {
        dot_w = mem_width - x;
    }

    if (y + dot_h > mem_height)
    {
        dot_h = mem_height - y;
    }

    dot_w = (dot_w < 0) ? 0 : dot_w;
    dot_h = (dot_h < 0) ? 0 : dot_h;

    mem_ptr += (mem_width * y + x);
    dot_tbl_ptr += (pen_width * dot_y + dot_x);

    for (i=0; i<dot_h; i++)
    {
        for (j=0; j<dot_w; j++)
        {
            uint8 alpha = *(dot_tbl_ptr + j);
            uint32 back_color = *(mem_ptr + j);
            uint32 result = 0;
            uint8 back = 0, fore = 0;
            
            //alpha
            result = 0xff000000;     //alpha = 255

            //r
            back = ARGB_GET_R(back_color);
            fore = ARGB_GET_R(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = ARGB_SET_R(result, back);

            //g
            back = ARGB_GET_G(back_color);
            fore = ARGB_GET_G(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = ARGB_SET_G(result, back);
            
            //b
            back = ARGB_GET_B(back_color);
            fore = ARGB_GET_B(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = ARGB_SET_B(result, back);
      
            *(mem_ptr + j) = result;                        
        }

        mem_ptr += mem_width;
        dot_tbl_ptr += pen_width;
    }
}

void DrawDot(int16 x, int16 y, uint8 pen_width, uint16 pen_color,
             uint16 *mem_ptr, uint16 mem_width, uint16 mem_height)
{
    const uint8 *dot_tbl_ptr = NULL;
    int16 dot_x = 0;
    int16 dot_y = 0;
    int16 dot_w = 0;
    int16 dot_h = 0;
    uint16 i, j;
    
    if (NULL == mem_ptr)
    {
        return;
    }

    pen_width = (pen_width < DOT_MIN_WIDTH) ? DOT_MIN_WIDTH : pen_width;
    pen_width = (pen_width > DOT_MAX_WIDTH) ? DOT_MAX_WIDTH : pen_width;

    dot_w = pen_width;
    dot_h = pen_width;

    x = (x - pen_width / 2);
    y = (y - pen_width / 2);
        
    dot_tbl_ptr = s_dot_tbl[pen_width - 1];
   
    if (x < 0)
    {
        dot_x = -x;
        dot_w -= dot_x;      
        x = 0;
    }

    if (y < 0)
    {
        dot_y = -y;
        dot_h -= dot_y;
        y = 0;
    }

    if (x + dot_w > mem_width)
    {
        dot_w = mem_width - x;

    }

    if (y + dot_h > mem_height)
    {
        dot_h = mem_height - y;
    }

    dot_w = (dot_w < 0) ? 0 : dot_w;
    dot_h = (dot_h < 0) ? 0 : dot_h;

    mem_ptr += (mem_width * y + x);
    dot_tbl_ptr += (pen_width * dot_y + dot_x);

    for (i=0; i<dot_h; i++)
    {
        for (j=0; j<dot_w; j++)
        {
            uint8 alpha = *(dot_tbl_ptr + j);
            uint16 back_color = *(mem_ptr + j);
            uint16 result= 0;
            uint8 back = 0, fore = 0;
            
            //r
            back = RGB565_GET_R(back_color);
            fore = RGB565_GET_R(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = RGB565_SET_R(result, back);

            //g
            back = RGB565_GET_G(back_color);
            fore = RGB565_GET_G(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = RGB565_SET_G(result, back);
            
            //b
            back = RGB565_GET_B(back_color);
            fore = RGB565_GET_B(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = RGB565_SET_B(result, back);
      
            *(mem_ptr + j) = result;                        
        }

        mem_ptr += mem_width;
        dot_tbl_ptr += pen_width;
    }
}
