/*==============================================================================
File Name: mmiim_touch_draw.c
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/



#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif


/*
备注1 我们为了简化绘制过程，统一采用32位色描点
备注2 因为系统不支持用32位色写字符，所以，只能最后统一设置alpha
*/

/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "mmiim_touch_draw.h"


#if defined (MMI_IM_PDA_SUPPORT)



//以下是画按钮的函数，按说，应该另成系统
/*
因为是特殊的应用，总是能保证所有的显示元素都在屏幕范围内，所以去掉相应的判断以提
高效率。即，所有的点、线、面的绘制，都认为是在当前buffer内的。
即，对任意点(x, y),  x in [0, lcd_w), y in [0, lcd_h)
画的任意圆角框，我们总认为大小是足够大，即 w,h in [13, ...)
我们可以确定我们总是在固定的层上开发，所以，可以确定层的数据类型
在预知的情况下，减少判断和多种情况的编码，即IM_DATA_TYPE_ARGB888模式下
在我们设定按钮的时候，我们需要
1 方框
2 填充的方框
3 圆角的方框
4 填充的圆角的方框
5 带边框效果的方框
6 带边框效果的圆角的方框
7 边框效果一般有: flat, raised, sunken, solid, ridge, groove
8 边框宽度可设置
----
1 边框是往rect的外部扩张的
2 边框的有两个属性，宽度和类型
3 矩形可以看作最简单的边框
*/

#define ROUND_RECT_WIDTH_LIMIT (16)
#define ROUND_RECT_HEIGHT_LIMIT (12)

#if defined MMIIM_LAYER_565
#define MASK_RED_565 (0x0000f800)
#define MASK_GREEN_565 (0x000007E0)
#define MASK_BLUE_565 (0x0000001f)
#endif

#define MASK_ALPHA (0xff000000)
#define MASK_RED (0x00ff0000)
#define MASK_GREEN (0x0000ff00)
#define MASK_BLUE (0x000000ff)


#if defined MMIIM_LAYER_565
#define DRAW_POINT(buf, buf_w, x, y, c)\
    do {\
        *(((uint16 *)buf) + (y)*((int32)buf_w) + (x)) = (c);\
    } while (0);
#else
#define DRAW_POINT(buf, buf_w, x, y, c)\
    do {\
        *((buf) + (y)*((int32)buf_w) + (x)) = (c);\
    } while (0);
#endif

/*==============================================================================
Note: 用硬代码描点来完成
0-----yx
1---yxx
2--yx
3-yx
4-x
5yx
6x
需要补上5个y点，否则在宽边情况下，会漏，用 is_full 来控制。
通常，最外面一圈，是无需full的，特别的，有且仅有一圈的时候，即为最外圈
---------
修改一下
0-yx
1yx
2x
==============================================================================*/
#if 0
#define ROUND_RECT_SIZE (6)
uint8 const s_round_rect_delta[ROUND_RECT_SIZE] = {6, 4, 3, 2, 1, 1};
uint8 const s_round_rect_delta_full[ROUND_RECT_SIZE] = {5, 3, 2, 1, 1, 0};
uint8 const s_round_rect_point[][2] =
{
    {5, 1}, {4, 1}, {3, 2}, {2, 3}, {1, 4}, {1, 5}
};
uint8 const s_round_rect_point_full[][2] =
{
    {5, 0}, {3, 1}, {2, 2}, {1, 3}, {0, 5}
};
#else
#define ROUND_RECT_SIZE (2)
uint8 const s_round_rect_delta[ROUND_RECT_SIZE] = {2, 1};
uint8 const s_round_rect_delta_full[ROUND_RECT_SIZE] = {1, 0};
uint8 const s_round_rect_point[][2] =
{
    {1, 1}
};
uint8 const s_round_rect_point_full[][2] =
{
    {1, 0}, {0, 1}
};

#endif


#ifdef MMIIM_LAYER_565
LOCAL __inline uint16 GetGradientColor565(
    uint16 c_begin,
    uint16 c_end,
    int16 len,
    int16 a,
    int16 pos
)
{
    //uint16 ba = (c_begin & MASK_ALPHA) >> 24;
    int16 br = (c_begin & MASK_RED_565) >> 11;
    int16 bg = (c_begin & MASK_GREEN_565) >> 5;
    int16 bb = (c_begin & MASK_BLUE_565);
    //uint16 da = (uint16)((c_end & MASK_ALPHA) >> 24) - ba;
    int16 dr = (int16)((c_end & MASK_RED_565) >> 11) - br;
    int16 dg = (int16)((c_end & MASK_GREEN_565) >> 5) - bg;
    int16 db = (int16)((c_end & MASK_BLUE_565)) - bb;
    int16 extra = a * pos * (len - pos) / len;

    SCI_ASSERT(0 <= pos && pos < len); /*assert verified*/

    //if (da != 0) ba += da * pos / len - extra;

    if (dr != 0) br += dr * pos / len - extra;

    if (dg != 0) bg += dg * pos / len - extra;

    if (db != 0) bb += db * pos / len - extra;

    //SCI_ASSERT(0 <= ba && ba <= 0xff); /*assert verified*/
    SCI_ASSERT(0 <= br && br <= 0xff); /*assert verified*/
    SCI_ASSERT(0 <= bg && bg <= 0xff); /*assert verified*/
    SCI_ASSERT(0 <= bb && bb <= 0xff); /*assert verified*/

    return (uint16)((br << 11) | (bg << 5) | bb);
}
#endif

/*==============================================================================
Description: GetGradientColor
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
S = a*T*T + v0*T => v0 = (S - a*T*T)/T
s = a*t*t + v0*t => s = S*t/T - a*t(T-t)
S - 总位移 - 对应颜色之间的变化数值
T - 总时间 - 对应渐进的距离
a - 加速度 - 渐进变化的效果控制，即变化快慢的控制
s - t时刻的位移 - 任意点的颜色变化数值
**将加速度的影响缩小len倍，否则，从0-255的距离上，加速度即使是1也太可怕了
==============================================================================*/
LOCAL __inline uint32 GetGradientColor(
    uint32 c_begin,
    uint32 c_end,
    int16 len,
    int16 a,
    int16 pos
)
{
    int32 ba = (c_begin & MASK_ALPHA) >> 24;
    int32 br = (c_begin & MASK_RED) >> 16;
    int32 bg = (c_begin & MASK_GREEN) >> 8;
    int32 bb = (c_begin & MASK_BLUE);
    int32 da = (int32)((c_end & MASK_ALPHA) >> 24) - ba;
    int32 dr = (int32)((c_end & MASK_RED) >> 16) - br;
    int32 dg = (int32)((c_end & MASK_GREEN) >> 8) - bg;
    int32 db = (int32)((c_end & MASK_BLUE)) - bb;
    int32 extra = a * pos * (len - pos) / len;

    SCI_ASSERT(0 <= pos && pos < len); /*assert verified*/

    if (da != 0) ba += da * pos / len - extra;

    if (dr != 0) br += dr * pos / len - extra;

    if (dg != 0) bg += dg * pos / len - extra;

    if (db != 0) bb += db * pos / len - extra;

    SCI_ASSERT(0 <= ba && ba <= 0xff); /*assert verified*/
    SCI_ASSERT(0 <= br && br <= 0xff); /*assert verified*/
    SCI_ASSERT(0 <= bg && bg <= 0xff); /*assert verified*/
    SCI_ASSERT(0 <= bb && bb <= 0xff); /*assert verified*/

    return (uint32)((ba << 24) | (br << 16) | (bg << 8) | bb);
}




LOCAL __inline void DrawRectRaw(
    uint32 *buf,
    uint32 mem_width,
    int16 left,
    int16 right,
    int16 top,
    int16 bottom,
    uint32 color_left,
    uint32 color_right,
    uint32 color_top,
    uint32 color_bottom
)
{
    int16 x = 0, y = 0;

#if defined MMIIM_LAYER_565
    color_left = RGB888TO565 (color_left);
    color_right = RGB888TO565 (color_right);
    color_top = RGB888TO565 (color_top);
    color_bottom = RGB888TO565 (color_bottom);
#endif
    //SCI_ASSERT(PNULL != buf); /*assert verified*/
    if(PNULL == buf)
    {
        return;
    }
    
    for (x = left + 1; x <= right - 1; x++)
    {
        DRAW_POINT(buf, mem_width, x, top, color_top);
        DRAW_POINT(buf, mem_width, x, bottom, color_bottom);
    }

    for (y = top + 1; y <= bottom - 1; y++)
    {
        DRAW_POINT(buf, mem_width, left, y, color_left);
        DRAW_POINT(buf, mem_width, right, y, color_right);
    }

#ifdef MMIIM_LAYER_565
    DRAW_POINT(buf, mem_width, left, top, GetGradientColor565 (color_left, color_top, 3, 0, 1));
    DRAW_POINT(buf, mem_width, left, bottom, GetGradientColor565(color_left, color_bottom, 3, 0, 1));
    DRAW_POINT(buf, mem_width, right, top, GetGradientColor565(color_right, color_top, 3, 0, 1));
    DRAW_POINT(buf, mem_width, right, bottom, GetGradientColor565(color_right, color_bottom, 3, 0, 1));
#else
    DRAW_POINT(buf, mem_width, left, top, GetGradientColor(color_left, color_top, 3, 0, 1));
    DRAW_POINT(buf, mem_width, left, bottom, GetGradientColor(color_left, color_bottom, 3, 0, 1));
    DRAW_POINT(buf, mem_width, right, top, GetGradientColor(color_right, color_top, 3, 0, 1));
    DRAW_POINT(buf, mem_width, right, bottom, GetGradientColor(color_right, color_bottom, 3, 0, 1));
#endif

    return;
}


LOCAL __inline void DrawBorderUnevenRaw(
    uint32 *buf,
    uint32 mem_width,
    int16 left,
    int16 right,
    int16 top,
    int16 bottom,
    uint32 color_left,
    uint32 color_right,
    uint32 color_top,
    uint32 color_bottom,
    uint8 border_width
)
{
    uint8 out_step = 0;

    for (out_step = 1; out_step <= border_width; out_step++)
    {
        DrawRectRaw(buf, mem_width,
                    left - out_step, right + out_step, top - out_step, bottom + out_step,
                    color_left, color_right, color_top, color_bottom);
    }

    return;
}


LOCAL __inline void DrawRoundRectRaw(
    uint32 *buf,
    uint32 mem_width,
    int16 left,
    int16 right,
    int16 top,
    int16 bottom,
    uint32 color_left,
    uint32 color_right,
    uint32 color_top,
    uint32 color_bottom,
    BOOLEAN is_full
)
{
    int16 x = 0, y = 0;
    uint32 i = 0;
    uint32 left_top[5];
    uint32 left_bottom[5];
    uint32 right_top[5];
    uint32 right_bottom[5];

    //SCI_ASSERT(PNULL != buf); /*assert verified*/
    //SCI_ASSERT(right - left + 1 >= ROUND_RECT_WIDTH_LIMIT); /*assert verified*/
    //SCI_ASSERT(bottom - top + 1 >= ROUND_RECT_HEIGHT_LIMIT); /*assert verified*/
    if(PNULL == buf || right - left + 1 < ROUND_RECT_WIDTH_LIMIT || bottom - top + 1 < ROUND_RECT_HEIGHT_LIMIT)
    {
        return;
    }

#if defined MMIIM_LAYER_565
    color_left = RGB888TO565 (color_left);
    color_right = RGB888TO565 (color_right);
    color_top = RGB888TO565 (color_top);
    color_bottom = RGB888TO565 (color_bottom);
#endif
    
    for (x = left + ROUND_RECT_SIZE; x <= right - ROUND_RECT_SIZE; x++)
    {
        DRAW_POINT(buf, mem_width, x, top, color_top);
        DRAW_POINT(buf, mem_width, x, bottom, color_bottom);
    }

    for (y = top + ROUND_RECT_SIZE; y <= bottom - ROUND_RECT_SIZE; y++)
    {
        DRAW_POINT(buf, mem_width, left, y, color_left);
        DRAW_POINT(buf, mem_width, right, y, color_right);
    }

#ifdef MMIIM_LAYER_565
    //生成转角颜色
    for (i = 0; i < 5; i++)
    {
        left_top[i] = GetGradientColor565 (color_left, color_top, 5, 0, i);
        left_bottom[i] = GetGradientColor565 (color_left, color_bottom, 5, 0, i);
        right_top[i] = GetGradientColor565 (color_right, color_top, 5, 0, i);
        right_bottom[i] = GetGradientColor565 (color_right, color_bottom, 5, 0, i);
    }
#else
    //生成转角颜色
    for (i = 0; i < 5; i++)
    {
        left_top[i] = GetGradientColor(color_left, color_top, 5, 0, i);
        left_bottom[i] = GetGradientColor(color_left, color_bottom, 5, 0, i);
        right_top[i] = GetGradientColor(color_right, color_top, 5, 0, i);
        right_bottom[i] = GetGradientColor(color_right, color_bottom, 5, 0, i);
    }
#endif

    for (i = 0; i < ARR_SIZE(s_round_rect_point); i++)
    {
        uint8 m = s_round_rect_point[i][0];
        uint8 n = s_round_rect_point[i][1];
        int32 dc = m - n;

        if (dc > 2) dc = 2;

        if (dc < -2) dc = -2;

        dc += 2;

        //left - top
        DRAW_POINT(buf, mem_width, left + m, top + n, left_top[dc]);
        //right - top
        DRAW_POINT(buf, mem_width, right - m, top + n, right_top[dc]);
        //left - bottom
        DRAW_POINT(buf, mem_width, left + m, bottom - n, left_bottom[dc]);
        //right - bottom
        DRAW_POINT(buf, mem_width, right - m, bottom - n, right_bottom[dc]);
    }

    if (is_full)
    {
        for (i = 0; i < ARR_SIZE(s_round_rect_point_full); i++)
        {
            uint8 m = s_round_rect_point_full[i][0];
            uint8 n = s_round_rect_point_full[i][1];
            int32 dc = m - n;

            if (dc > 2) dc = 2;

            if (dc < -2) dc = -2;

            dc += 2;

            //left - top
            DRAW_POINT(buf, mem_width, left + m, top + n, left_top[dc]);
            //right - top
            DRAW_POINT(buf, mem_width, right - m, top + n, right_top[dc]);
            //left - bottom
            DRAW_POINT(buf, mem_width, left + m, bottom - n, left_bottom[dc]);
            //right - bottom
            DRAW_POINT(buf, mem_width, right - m, bottom - n, right_bottom[dc]);
        }
    }

    return;
}


LOCAL __inline void DrawRoundBorderUnevenRaw(
    uint32 *buf,
    uint32 mem_width,
    int16 left,
    int16 right,
    int16 top,
    int16 bottom,
    uint32 color_left,
    uint32 color_right,
    uint32 color_top,
    uint32 color_bottom,
    uint8 border_width,
    BOOLEAN is_full
)
{
    uint8 out_step = 0;

    for (out_step = 1; out_step < border_width; out_step++)
    {
        DrawRoundRectRaw(buf, mem_width,
                         left - out_step, right + out_step, top - out_step, bottom + out_step,
                         color_left, color_right, color_top, color_bottom, TRUE);
    }

    DrawRoundRectRaw(buf, mem_width,
                     left - out_step, right + out_step, top - out_step, bottom + out_step,
                     color_left, color_right, color_top, color_bottom, is_full);

    return;
}






/*==============================================================================
Description: 填充一个圆
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 用Bresenham算法
2 用于粗直线的两端在手写的时候承接圆润
3 为了简化我们总是在顶端画一个圆，而不去在意重复画的那部分浪费得效率
!!!该函数是不安全的,目前其安全性由调用函数保证
==============================================================================*/
PUBLIC BOOLEAN _FillCircle(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    int16 x, int16 y, int16 r,
    uint32 color
)
{
    uint32 *buf;
    int16 dx = 0, dy = r, e = 3 - 2 * r;
    int16 lx = 0;

    //SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    
    if (r <= 0 ||PNULL == dc_data_ptr) return TRUE;

#if defined MMIIM_LAYER_565
    color = RGB888TO565 (color);
#endif

    buf = dc_data_ptr->lcd_buffer_ptr;

    //先画一条过圆心的横线
    for (lx = x - r; lx <= x + r; lx++)
    {
        DRAW_POINT(buf, dc_data_ptr->mem_width, lx, y, color);
    }

    //再画上下两点
    DRAW_POINT(buf, dc_data_ptr->mem_width, x, y - r, color);
    DRAW_POINT(buf, dc_data_ptr->mem_width, x, y + r, color);

    while (dx < dy)
    {
        if (e < 0)
        {
            e += 4 * dx + 6;
            dx++;
        }
        else
        {
            e += 4 * (dx - dy) + 10;
            dx++;
            dy--;

            for (lx = x - dx; lx <= x + dx; lx++)
            {
                DRAW_POINT(buf, dc_data_ptr->mem_width, lx, y - dy, color);
                DRAW_POINT(buf, dc_data_ptr->mem_width, lx, y + dy, color);
            }
        }


        for (lx = x - dy; lx <= x + dy; lx++)
        {
            DRAW_POINT(buf, dc_data_ptr->mem_width, lx, y - dx, color);
            DRAW_POINT(buf, dc_data_ptr->mem_width, lx, y + dx, color);
        }
    }

    return TRUE;
}


/*==============================================================================
Description: 画一条直线
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 用Bresenham算法
只需要在一端画个转接的圆就可以了
==============================================================================*/
PUBLIC BOOLEAN _DrawLine(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    int16 x0, int16 y0, int16 x1, int16 y1,
    int16 width,
    uint32 color
)
{
    uint32 *buf;
    int16 dx, dy, step = 1, e;
    int16 w = 0;
    int16 dw = (width >> 1);
#ifdef MMIIM_LAYER_565
    uint16 color_565 = RGB888TO565 (color);
#endif
    //SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    if (PNULL == dc_data_ptr)
    {
        return FALSE;
    }

    if (x0 < width || y0 < width || x1 < width || y1 < width
        || x0 + width >= dc_data_ptr->width/*lint -save -e574 -e737*/
        || x1 + width >= dc_data_ptr->width
        || y0 + width >= dc_data_ptr->height
        || y1 + width >= dc_data_ptr->height)
    {
        return FALSE;
    }
    
    buf = dc_data_ptr->lcd_buffer_ptr;

    if (abs(x1 - x0) > abs(y1 - y0))
    {
        if (x0 > x1)
        {
            x0 = x0 ^ x1;
            x1 = x0 ^ x1;
            x0 = x0 ^ x1;
            y0 = y0 ^ y1;
            y1 = y0 ^ y1;
            y0 = y0 ^ y1;
        }

        dx = x1 - x0;
        dy = y1 - y0;

        if (dy < 0)
        {
            step = -1;
            dy = -dy;
        }

        dx <<= 1;
        dy <<= 1;
        e = -dx;

        for (; x0 < x1; x0++)
        {
            for (w = 0; w < width; w++)
            {
                #ifdef MMIIM_LAYER_565
                    DRAW_POINT(buf, dc_data_ptr->mem_width, x0, y0 - w + dw, color_565);
                #else
                    DRAW_POINT(buf, dc_data_ptr->mem_width, x0, y0 - w + dw, color);
                #endif
            }

            e += dy;

            if (e > 0)
            {
                y0 += step;
                e -= dx;
            }
        }
    }
    else
    {
        if (y0 > y1)
        {
            x0 = x0 ^ x1;
            x1 = x0 ^ x1;
            x0 = x0 ^ x1;
            y0 = y0 ^ y1;
            y1 = y0 ^ y1;
            y0 = y0 ^ y1;
        }

        dx = x1 - x0;
        dy = y1 - y0;

        if (dx < 0)
        {
            step = -1;
            dx = -dx;
        }

        dx <<= 1;
        dy <<= 1;
        e = -dy;

        for (; y0 < y1; y0++)
        {
            for (w = 0; w < width; w++)
            {
                #ifdef MMIIM_LAYER_565
                    DRAW_POINT(buf, dc_data_ptr->mem_width, x0 - w + dw, y0, color_565);
                #else
                    DRAW_POINT(buf, dc_data_ptr->mem_width, x0 - w + dw, y0, color);
                #endif
            }

            e += dx;

            if (e > 0)
            {
                x0 += step;
                e -= dy;
            }
        }
    }

    //_FillCircle(dc_data_ptr, x0, y0, dw, color);
    _FillCircle(dc_data_ptr, x1, y1, dw, color);

    return TRUE;
}


/*==============================================================================
Description: 绘制矩形
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN _DrawRect(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint32 color
)
{
    DC_CHECK(dc_data_ptr, rect_ptr);

    DrawRectRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                rect_ptr->left, rect_ptr->right, rect_ptr->top, rect_ptr->bottom,
                color, color, color, color);

    return TRUE;
}


/*==============================================================================
Description: 绘制填充的矩形
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN _FillRect(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint32 color    //该颜色如果为888，则不做转换，否则在函数内部转换为565
)
{
    int16 x = 0, y = 0;
    uint32 *buffer = PNULL;

    DC_CHECK(dc_data_ptr, rect_ptr);
    
#if defined MMIIM_LAYER_565
    color = RGB888TO565 (color);
#endif

    buffer = dc_data_ptr->lcd_buffer_ptr;

    for (y = rect_ptr->top; y <= rect_ptr->bottom; y++)
        for (x = rect_ptr->left; x <= rect_ptr->right; x++)
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, y, color);

    return TRUE;
}


/*==============================================================================
Description: 绘制填充的矩形 - 颜色渐进
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 我们按钮只需要做上下渐进效果的
==============================================================================*/
PUBLIC BOOLEAN _FillGradientRect(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint32 color_top,
    uint32 color_bottom,
    int16 acceleration
)
{
    int16 x = 0, y = 0;
    uint32 *buffer = PNULL;
    int16 len = 0;

    DC_CHECK(dc_data_ptr, rect_ptr);

    buffer = dc_data_ptr->lcd_buffer_ptr;
    len = rect_ptr->bottom - rect_ptr->top + 1;

    for (y = rect_ptr->top; y <= rect_ptr->bottom; y++)
    {
        uint32 color = GetGradientColor (color_top, color_bottom, len, acceleration, y - rect_ptr->top);

#ifdef MMIIM_LAYER_565
        color = RGB888TO565 (color);
#endif
        for (x = rect_ptr->left; x <= rect_ptr->right; x++)
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, y, color);
    }

    return TRUE;
}


/*==============================================================================
Description: 绘制边框 - 不平的，凹的或者凸的
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: RASIED or SUNKEN
==============================================================================*/
PUBLIC BOOLEAN _DrawBorderUneven(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint8 width,
    BORDER_RELIEF_T relief,
    uint32 color,
    uint32 color_bright,
    uint32 color_dark
)
{
    uint32 up_color = 0;
    uint32 down_color = 0;

    DC_CHECK(dc_data_ptr, rect_ptr);

    if (RELIEF_RAISED == relief)
    {
        up_color = color_bright;
        down_color = color_dark;
    }
    else if (RELIEF_SUNKEN == relief)
    {
        up_color = color_dark;
        down_color = color_bright;
    }
    else if (RELIEF_SOLID == relief)
    {
        up_color = color;
        down_color = color;
    }
    else
    {
        return FALSE;
    }

    DrawBorderUnevenRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                        rect_ptr->left, rect_ptr->right, rect_ptr->top, rect_ptr->bottom,
                        up_color, down_color, up_color, down_color, width);

    return TRUE;
}


/*==============================================================================
Description: 绘制边框 - 有形状的，山脊状或凹槽状
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 RIDGE or GROOVE
2 至少宽度要是3 - 里面、外面和中间
==============================================================================*/
PUBLIC BOOLEAN _DrawBorderSharp(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint8 width,
    BORDER_RELIEF_T relief,
    uint32 color,
    uint32 color_bright,
    uint32 color_dark
)
{
    uint8 in_width = 0;
    uint8 out_width = 0;
    GUI_RECT_T mid_rect = {0};
    uint32 up_color = 0;
    uint32 down_color = 0;

    DC_CHECK(dc_data_ptr, rect_ptr);

    if (width < 3)
    {
        return FALSE;
    }

    if (RELIEF_GROOVE == relief)
    {
        up_color = color_bright;
        down_color = color_dark;
    }
    else if (RELIEF_RIDGE == relief)
    {
        up_color = color_dark;
        down_color = color_bright;
    }
    else
    {
        return FALSE;
    }

    in_width = width / 2;
    out_width = width - in_width - 1;
    mid_rect = *rect_ptr;
    mid_rect.left -= in_width + 1;
    mid_rect.right += in_width + 1;
    mid_rect.top -= in_width + 1;
    mid_rect.bottom += in_width + 1;

    //inner
    DrawBorderUnevenRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                        rect_ptr->left, rect_ptr->right, rect_ptr->top, rect_ptr->bottom,
                        up_color, down_color, up_color, down_color, in_width);

    //middle
    DrawRectRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                mid_rect.left, mid_rect.right, mid_rect.top, mid_rect.bottom,
                color, color, color, color);

    //outer
    DrawBorderUnevenRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                        mid_rect.left, mid_rect.right, mid_rect.top, mid_rect.bottom,
                        down_color, up_color, down_color, up_color, out_width);


    return TRUE;
}


/*==============================================================================
Description: 绘制矩形(ROUND)
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN _DrawRoundRect(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint32 color
)
{
    DC_CHECK(dc_data_ptr, rect_ptr);

    DrawRoundRectRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                     rect_ptr->left, rect_ptr->right, rect_ptr->top, rect_ptr->bottom,
                     color, color, color, color, FALSE);

    return TRUE;
}


/*==============================================================================
Description: 绘制填充的矩形(ROUND)
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN _FillRoundRect(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint32 color,
    BOOLEAN is_full
)
{
    int16 x = 0, y = 0;
    uint32 *buffer = PNULL;
    uint8 const *delta_ptr = PNULL;

    DC_CHECK(dc_data_ptr, rect_ptr);
    
#if defined MMIIM_LAYER_565
    color = RGB888TO565 (color);
#endif

    buffer = dc_data_ptr->lcd_buffer_ptr;

    delta_ptr = is_full ? s_round_rect_delta_full : s_round_rect_delta;

    for (y = 0; y < ROUND_RECT_SIZE; y++)
    {
        for (x = rect_ptr->left + delta_ptr[y]; x <= rect_ptr->right - delta_ptr[y]; x++)
        {
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_ptr->top + y, color);
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_ptr->bottom - y, color);
        }
    }

    for (y = rect_ptr->top + ROUND_RECT_SIZE; y <= rect_ptr->bottom - ROUND_RECT_SIZE; y++)
        for (x = rect_ptr->left; x <= rect_ptr->right; x++)
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, y, color);

    return TRUE;
}


/*==============================================================================
Description: 绘制填充的矩形(ROUND) - 颜色渐进
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN _FillGradientRoundRect(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint32 color_top,
    uint32 color_bottom,
    int16 acceleration,
    BOOLEAN is_full
)
{
    int16 x = 0, y = 0;
    uint32 *buffer = PNULL;
    int16 len = 0;
    uint8 const *delta_ptr = PNULL;

    DC_CHECK(dc_data_ptr, rect_ptr);
    
    buffer = dc_data_ptr->lcd_buffer_ptr;
    len = rect_ptr->bottom - rect_ptr->top + 1;

    delta_ptr = is_full ? s_round_rect_delta_full : s_round_rect_delta;

    for (y = 0; y < ROUND_RECT_SIZE; y++)
    {
        uint32 color = GetGradientColor(color_top, color_bottom, len, acceleration, y);
#if defined MMIIM_LAYER_565
        color = RGB888TO565 (color);        //分两步骤，防止先转换为565，再计算渐变色失真
#endif        
        for (x = rect_ptr->left + delta_ptr[y]; x <= rect_ptr->right - delta_ptr[y]; x++)
        {
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_ptr->top + y, color);
        }

        color = GetGradientColor (color_top, color_bottom, len, acceleration, len - 1 - y);
#if defined MMIIM_LAYER_565
        color = RGB888TO565 (color);        //分两步骤，防止失真
#endif        
        for (x = rect_ptr->left + delta_ptr[y]; x <= rect_ptr->right - delta_ptr[y]; x++)
        {
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_ptr->bottom - y, color);
        }
    }

    for (y = rect_ptr->top + ROUND_RECT_SIZE; y <= rect_ptr->bottom - ROUND_RECT_SIZE; y++)
    {
        uint32 color = GetGradientColor (color_top, color_bottom, len, acceleration, y - rect_ptr->top);

#if defined MMIIM_LAYER_565
        color = RGB888TO565 (color);        //分两步骤，防止失真
#endif        

        for (x = rect_ptr->left; x <= rect_ptr->right; x++)
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, y, color);
    }

    return TRUE;
}


/*==============================================================================
Description: 绘制填充的矩形(ROUND) - 颜色渐进, 分上下两层(平分)
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN _FillGradientBlockRoundRect(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint32 color_0_top,
    uint32 color_0_bottom,
    int16 acceleration_0,
    uint32 color_1_top,
    uint32 color_1_bottom,
    int16 acceleration_1,
    uint32 color_2_top,
    uint32 color_2_bottom,
    int16 acceleration_2,
    BOOLEAN is_full
)
{
    uint8 const block_up = 4;
    uint8 const block_middle = 5;
    uint8 const block_down = 1;
    uint8 const block_all = block_up + block_middle + block_down;
    GUI_RECT_T rect_0 = {0};
    GUI_RECT_T rect_1 = {0};
    GUI_RECT_T rect_2 = {0};
    int16 height = 0;

    DC_CHECK(dc_data_ptr, rect_ptr);

    height = rect_ptr->bottom - rect_ptr->top + 1;

    rect_0 = *rect_ptr;
    rect_0.bottom = rect_0.top + height * block_up / block_all;

    rect_1 = *rect_ptr;
    rect_1.top = rect_0.bottom + 1 - ROUND_RECT_SIZE;
    rect_1.bottom = rect_0.top + height * (block_middle + block_up) / block_all;

    rect_2 = *rect_ptr;
    rect_2.top = rect_1.bottom + 1 - ROUND_RECT_SIZE;

    _FillGradientRoundRect(dc_data_ptr, &rect_0,
                           color_0_top, color_0_bottom, acceleration_0, is_full);

    _FillGradientRoundRect(dc_data_ptr, &rect_1,
                           color_1_top, color_1_bottom, acceleration_1, is_full);

    _FillGradientRoundRect(dc_data_ptr, &rect_2,
                           color_2_top, color_2_bottom, acceleration_2, is_full);

    return TRUE;
}


/*==============================================================================
Description: 绘制填充的矩形(ROUND) - 上8%颜色渐进,下部分纯色
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN _FillGradientPureBlockRoundRect (
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint32 color_0_top,
    uint32 color_0_bottom,
    int16 acceleration_0,
    BOOLEAN is_full
)
{
    GUI_RECT_T rect_1 = {0};
    int16 x = 0;
    uint32 *buffer = PNULL;
    uint32 color = 0;
#if defined MMIIM_LAYER_565
    uint32 color_0_top_565 = RGB888TO565 (color_0_top);
    uint32 color_0_bottom_565 = RGB888TO565 (color_0_bottom);
#endif

    DC_CHECK(dc_data_ptr, rect_ptr);

    buffer = dc_data_ptr->lcd_buffer_ptr;
    rect_1 = *rect_ptr;

    //第一行
    for (x = rect_1.left + 2; x <= rect_1.right - 2; x++)
    {
        #if defined MMIIM_LAYER_565 
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_1.top, color_0_top_565);
        #else
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_1.top, color_0_top);
        #endif
    }

    //第二行
    color = GetGradientColor (color_0_top, color_0_bottom, 2, acceleration_0, 1);
#if defined MMIIM_LAYER_565    
    color = RGB888TO565 (color);
#endif
    for (x = rect_1.left + 1; x <= rect_1.right - 1; x++)
    {
        DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_1.top + 1, color);
    }

    //中间部分
    rect_1.top += 2;
    rect_1.bottom -= 2;
    _FillRect (dc_data_ptr, &rect_1, color_0_bottom);

    //底部分
    //倒数第一行
    rect_1 = *rect_ptr;
    for (x = rect_1.left + 2; x <= rect_1.right - 2; x++)
    {
        #if defined MMIIM_LAYER_565 
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_1.bottom, color_0_bottom_565);
        #else
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_1.bottom, color_0_bottom);
        #endif
    }

    //倒数第二行
    for (x = rect_1.left + 1; x <= rect_1.right - 1; x++)
    {
        #if defined MMIIM_LAYER_565 
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_1.bottom - 1, color_0_bottom_565);
        #else
            DRAW_POINT(buffer, dc_data_ptr->mem_width, x, rect_1.bottom - 1, color_0_bottom);
        #endif
    }
    
    return TRUE;
}


/*==============================================================================
Description: 绘制边框 - 不平的，凹的或者凸的(ROUND)
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: RASIED or SUNKEN
==============================================================================*/
PUBLIC BOOLEAN _DrawRoundBorderUneven(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint8 width,
    BORDER_RELIEF_T relief,
    uint32 color,
    uint32 color_bright,
    uint32 color_dark
)
{
    uint32 up_color = 0;
    uint32 down_color = 0;

    DC_CHECK(dc_data_ptr, rect_ptr);

    if (RELIEF_RAISED == relief)
    {
        up_color = color_bright;
        down_color = color_dark;
    }
    else if (RELIEF_SUNKEN == relief)
    {
        up_color = color_dark;
        down_color = color_bright;
    }
    else if (RELIEF_SOLID == relief)
    {
        up_color = color;
        down_color = color;
    }
    else
    {
        return FALSE;
    }

    DrawRoundBorderUnevenRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                             rect_ptr->left, rect_ptr->right, rect_ptr->top, rect_ptr->bottom,
                             up_color, down_color, up_color, down_color, width, FALSE);

    return TRUE;
}


/*==============================================================================
Description: 绘制边框 - 有形状的，山脊状或凹槽状(ROUND)
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 RIDGE or GROOVE
2 至少宽度要是3 - 里面、外面和中间
==============================================================================*/
PUBLIC BOOLEAN _DrawRoundBorderSharp(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint8 width,
    BORDER_RELIEF_T relief,
    uint32 color,
    uint32 color_bright,
    uint32 color_dark
)
{
    uint8 in_width = 0;
    uint8 out_width = 0;
    GUI_RECT_T mid_rect = {0};
    uint32 up_color = 0;
    uint32 down_color = 0;

    DC_CHECK(dc_data_ptr, rect_ptr);

    if (width < 3)
    {
        return FALSE;
    }

    if (RELIEF_GROOVE == relief)
    {
        up_color = color_bright;
        down_color = color_dark;
    }
    else if (RELIEF_RIDGE == relief)
    {
        up_color = color_dark;
        down_color = color_bright;
    }
    else
    {
        return FALSE;
    }

    in_width = width / 2;
    out_width = width - in_width - 1;
    mid_rect = *rect_ptr;
    mid_rect.left -= in_width + 1;
    mid_rect.right += in_width + 1;
    mid_rect.top -= in_width + 1;
    mid_rect.bottom += in_width + 1;

    //inner
    DrawRoundBorderUnevenRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                             rect_ptr->left, rect_ptr->right, rect_ptr->top, rect_ptr->bottom,
                             up_color, down_color, up_color, down_color, in_width, TRUE);

    //middle
    DrawRoundRectRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                     mid_rect.left, mid_rect.right, mid_rect.top, mid_rect.bottom,
                     color, color, color, color, TRUE);

    //outer
    DrawRoundBorderUnevenRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                             mid_rect.left, mid_rect.right, mid_rect.top, mid_rect.bottom,
                             down_color, up_color, down_color, up_color, out_width, FALSE);


    return TRUE;
}


/*==============================================================================
Description: 绘制边框 - 内外层明暗变化的边框(ROUND)
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 内嵌明亮或者阴暗
2 至少宽度要是2 - 里面、外面
==============================================================================*/
PUBLIC BOOLEAN _DrawRoundBorderInset(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUI_RECT_T const *rect_ptr,
    uint8 width,
    BORDER_RELIEF_T relief,
    uint32 color_bright,
    uint32 color_dark
)
{
    uint8 in_width = 0;
    uint8 out_width = 0;
    GUI_RECT_T mid_rect = {0};
    uint32 in_color = 0;
    uint32 out_color = 0;

    DC_CHECK(dc_data_ptr, rect_ptr);

    if (width < 2)
    {
        return FALSE;
    }

    if (RELIEF_INSET_BRIGHT == relief)
    {
        in_color = color_bright;
        out_color = color_dark;
    }
    else if (RELIEF_INSET_DARK == relief)
    {
        in_color = color_dark;
        out_color = color_bright;
    }
    else
    {
        return FALSE;
    }

    in_width = width / 2;
    out_width = width - in_width;
    mid_rect = *rect_ptr;
    mid_rect.left -= in_width;
    mid_rect.right += in_width;
    mid_rect.top -= in_width;
    mid_rect.bottom += in_width;

    //inner
    DrawRoundBorderUnevenRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                             rect_ptr->left, rect_ptr->right, rect_ptr->top, rect_ptr->bottom,
                             in_color, in_color, in_color, in_color, in_width, TRUE);

    //outer
    DrawRoundBorderUnevenRaw(dc_data_ptr->lcd_buffer_ptr, dc_data_ptr->mem_width,
                             mid_rect.left, mid_rect.right, mid_rect.top, mid_rect.bottom,
                             out_color, out_color, out_color, out_color, out_width, FALSE);


    return TRUE;
}


#endif //defined (MMI_IM_PDA_SUPPORT) 

