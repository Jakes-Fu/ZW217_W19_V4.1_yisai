#include "draw_line.h"
#include "draw_dot.h"

#define SWAP(a, b, t) {(t)=(a);(a)=(b);(b)=(t);}
#define ABS(x)        ((x)>0?(x):-(x))
	
#define FIXED_PRECISION 16
#define FIXED_MASS      0xffff0000
#define FIXED_FRAC      0x0000ffff
#define FIXED_ONE       0x00010000
#define FIXED_HALF      0x00008000

typedef int fixed;

#define R_COLOR(color)          ((color) & 0xf800)
#define G_COLOR(color)          ((color) & 0x07e0)
#define B_COLOR(color)          ((color) & 0x001f)

#define ARGB888_R_COLOR(color)          (((color) >> 16) & 0xff)
#define ARGB888_G_COLOR(color)          (((color) >> 8) & 0xff)
#define ARGB888_B_COLOR(color)          ((color) & 0xff)

#define ARGB_GET_R(_color) ((uint8)((_color) >> 16))
#define ARGB_GET_G(_color) ((uint8)((_color) >> 8))
#define ARGB_GET_B(_color) ((uint8)(_color))

#define ARGB_SET_R(_color, _r) ((_color) | ((_r) << 16))
#define ARGB_SET_G(_color, _g) ((_color) | ((_g) << 8))
#define ARGB_SET_B(_color, _b) ((_color) | (_b))

#define ALPHA_BLENDING(_x1, _x2, _a) (((((_x1) - (_x2)) * (_a)) >> 8) + (_x2))

/*
*This macro write color value to lcd buffer,
*and if the pixel is not in lcd size, do nothing 
*/
#define WR_COLOR_IN_LCD(_lcd_ptr, _buf_width, _x, _y, _width, _height, _color)  \
{                                                       \
    if ( ((_x)<0) || ((_x)>=(_width)) || ((_y)<0) || ((_y)>=(_height)) ) return;    \
    *( (_lcd_ptr) + (_y)*(_buf_width) + (_x) ) = (_color);  \
}

/*
*This macro read color value from lcd buffer,
*and if the pixel is not in lcd size, do nothing 
*/
#define RD_COLOR_IN_LCD(_lcd_ptr, _buf_width, _x, _y, _width, _height, _color)  \
{                                                       \
    if ( ((_x)<0) || ((_x)>=(_width)) || ((_y)<0) || ((_y)>=(_height)) ) return;    \
    (_color) = *( (_lcd_ptr) + (_y)*(_buf_width) + (_x) );  \
}


__inline fixed fixed_cvt_i2x(int x)         {return x<<FIXED_PRECISION;}
__inline int   fixed_cvt_x2i(fixed x)       {return x>>FIXED_PRECISION;}
__inline fixed fixed_cvt_f2x(float x)       {return (int)(x*(1<<FIXED_PRECISION));}
__inline float fixed_cvt_x2f(fixed x)       {return (float)x/(1<<FIXED_PRECISION);}

__inline fixed fixed_frac(fixed x)          {return x&FIXED_FRAC;}
__inline fixed fixed_mass(fixed x)          {return x&FIXED_MASS;}

__inline fixed fixed_ceil(fixed x)          {return (x+FIXED_FRAC)&FIXED_MASS;}
__inline fixed fixed_floor(fixed x)         {return  x&FIXED_MASS;}

__inline fixed fixed_idiv(int a, int b)     {return (a<<FIXED_PRECISION)/b;}
__inline fixed fixed_mul(fixed a, fixed b)  {return (a*b)>>FIXED_PRECISION;}
__inline fixed fixed_scale(fixed a, int b)  {return a*b;}

/*
*This macro write color value to lcd buffer, in xor mode
*and if the pixel is not in lcd size, do nothing 
*/
#define WR_COLOR_IN_LCD_XOR(_lcd_ptr,_buf_width,_x, _y, _width, _height, _color)    \
{                                                       \
    if ( ((_x)<0) || ((_x)>=(_width)) || ((_y)<0) || ((_y)>=(_height)) ) return;    \
    *( (_lcd_ptr) + (_y)*(_buf_width) + (_x) ) ^= (_color);     \
}


__inline int isqrt(int x)
{
	int root = 0;
	int seed = 1<<30;
	while(seed > x)
	{
		seed>>=2;
	}
	while(seed != 0)
	{
		if(x >= seed+root)
		{
			x    -= seed+root;
			root += seed*2;
		}
		root >>= 1;
		seed >>= 2;
	}
	return root;
}

LOCAL fixed coverage(fixed frac, fixed slope)
{
	if(slope>=0)
	{
		if(frac<slope)
		{
			return frac/2*frac/slope;
		}
		if(frac<=FIXED_ONE)
		{
			return frac-slope/2;
		}
	}
	else
	{
		frac = FIXED_ONE - frac;
		if(frac<-slope)
		{
			return -frac/2*frac/slope;
		}
		if(frac<=FIXED_ONE)
		{
			return frac+slope/2;
		}
	}
    return 0;
}


__inline void blend_color(
                        uint16 *buffer_ptr,
                        uint32 buf_width,
                        uint32 lcd_width,
                        uint32 lcd_height,
                        int16 x,
                        int16 y,
                        uint16 color,
                        fixed alpha
                        )
{
    uint32 lcd_color;
    uint16 alpha_value = alpha >> 8;
    if(0 == alpha_value) return;
    RD_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, lcd_color);
    color = 
        R_COLOR((R_COLOR(color) * alpha_value + R_COLOR(lcd_color) * (256 - alpha_value)) >> 8) |
        G_COLOR((G_COLOR(color) * alpha_value + G_COLOR(lcd_color) * (256 - alpha_value)) >> 8) |
        B_COLOR((B_COLOR(color) * alpha_value + B_COLOR(lcd_color) * (256 - alpha_value)) >> 8);
    WR_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, color);
}

__inline void blend_color_U32(
                                    uint32 *buffer_ptr,
                                    uint32 buf_width,
                                    uint32 lcd_width,
                                    uint32 lcd_height,
                                    int16 x,
                                    int16 y,
                                    uint32 color,
                                    fixed alpha                                  
                                  )
{
#if 1
    uint32 lcd_color;
    int32 alpha_value = alpha >> 8;
    uint32 result = 0;
    uint32  c, lcd_c;

    if(0 == alpha_value)	return;

    RD_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, lcd_color);

    result = 0xff000000;
    
    c = ARGB_GET_R(color);
    lcd_c = ARGB_GET_R(lcd_color);
    result = result | ((ALPHA_BLENDING(c, lcd_c, alpha_value) & 0xff) << 16);

    c = ARGB_GET_G(color);
    lcd_c = ARGB_GET_G(lcd_color);
    result = result | ((ALPHA_BLENDING(c, lcd_c, alpha_value) & 0xff) << 8);

    c = ARGB_GET_B(color);
    lcd_c = ARGB_GET_B(lcd_color);
    result = result | (ALPHA_BLENDING(c, lcd_c, alpha_value) & 0xff);

    WR_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, result);
#else
    uint8 alpha_value = alpha >> 8;
    uint32 result = 0;

    if(0 == alpha_value)	return;

	result = (color & 0xffffff) | (alpha_value << 24);
    WR_COLOR_IN_LCD(buffer_ptr, (int32)buf_width, x, y, (int32)lcd_width, (int32)lcd_height, result);	
#endif
}

__inline void fill_color(
                        uint16 *buffer_ptr,
                        uint32 buf_width,
                        uint32 lcd_width,
                        uint32 lcd_height,
                        int16 x,
                        int16 y,
                        uint32 color
                        )
{
    WR_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, color);
}

__inline void fill_color_U32(
                        uint32 *buffer_ptr,
                        uint32 buf_width,
                        uint32 lcd_width,
                        uint32 lcd_height,
                        int16 x,
                        int16 y,
                        uint32 color
                        )
{
    WR_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, color);
}

#define CAP_PATTERN_MAX 5

#if 1
LOCAL const fixed pattern_1[] ={
        0x0000142B,  0x000074E4,  0x0000142B,
        0x000074E4,  0x0000ffff,  0x000074E4,
        0x0000142B,  0x000074E4,  0x0000142B
    };

LOCAL const fixed pattern_2[] ={
        0x0000142B,  0x000074E4,  0x0000142B,
        0x000074E4,  0x0000ffff,  0x000074E4,
        0x0000142B,  0x000074E4,  0x0000142B
    };
LOCAL const fixed pattern_3[] ={
        0x00008B9F,  0x0000F8C3,  0x00008B9F,
        0x0000F8C3,  0x0000ffff,  0x0000F8C3,
        0x00008B9F,  0x0000F8C3,  0x00008B9F
    };
LOCAL const fixed pattern_4[] ={
        0x00000000,  0x000036BD,  0x00007A9D,  0x000036BD,  0x00000000,
        0x000036BD,  0x0000FC26,  0x0000ffff,  0x0000FC26,  0x000036BD,
        0x00007A9D,  0x0000ffff,  0x0000ffff,  0x0000ffff,  0x00007A9D,
        0x000036BD,  0x0000FC26,  0x0000ffff,  0x0000FC26,  0x000036BD,
        0x00000000,  0x000036BD,  0x00007A9D,  0x000036BD,  0x00000000
    };
LOCAL const fixed pattern_5[] ={
        0x00002309,  0x0000C4F2,  0x0000FBB5,  0x0000C4F2,  0x00002309,
        0x0000C4F2,  0x0000ffff,  0x0000ffff,  0x0000ffff,  0x0000C4F2,
        0x0000FBB5,  0x0000ffff,  0x0000ffff,  0x0000ffff,  0x0000FBB5,
        0x0000C4F2,  0x0000ffff,  0x0000ffff,  0x0000ffff,  0x0000C4F2,
        0x00002309,  0x0000C4F2,  0x0000FBB5,  0x0000C4F2,  0x00002309
    };
#else
LOCAL const fixed pattern_1[] ={
        0x0000142B,  0x000074E4,  0x0000142B,
        0x000074E4,  0x00010000,  0x000074E4,
        0x0000142B,  0x000074E4,  0x0000142B
    };

LOCAL const fixed pattern_2[] ={
        0x0000142B,  0x000074E4,  0x0000142B,
        0x000074E4,  0x00010000,  0x000074E4,
        0x0000142B,  0x000074E4,  0x0000142B
    };
LOCAL const fixed pattern_3[] ={
        0x00008B9F,  0x0000F8C3,  0x00008B9F,
        0x0000F8C3,  0x00010000,  0x0000F8C3,
        0x00008B9F,  0x0000F8C3,  0x00008B9F
    };
LOCAL const fixed pattern_4[] ={
        0x00000000,  0x000036BD,  0x00007A9D,  0x000036BD,  0x00000000,
        0x000036BD,  0x0000FC26,  0x00010000,  0x0000FC26,  0x000036BD,
        0x00007A9D,  0x00010000,  0x00010000,  0x00010000,  0x00007A9D,
        0x000036BD,  0x0000FC26,  0x00010000,  0x0000FC26,  0x000036BD,
        0x00000000,  0x000036BD,  0x00007A9D,  0x000036BD,  0x00000000
    };
LOCAL const fixed pattern_5[] ={
        0x00002309,  0x0000C4F2,  0x0000FBB5,  0x0000C4F2,  0x00002309,
        0x0000C4F2,  0x00010000,  0x00010000,  0x00010000,  0x0000C4F2,
        0x0000FBB5,  0x00010000,  0x00010000,  0x00010000,  0x0000FBB5,
        0x0000C4F2,  0x00010000,  0x00010000,  0x00010000,  0x0000C4F2,
        0x00002309,  0x0000C4F2,  0x0000FBB5,  0x0000C4F2,  0x00002309
    };
#endif

LOCAL const fixed* patterns[] ={
		pattern_1,
        pattern_2,
        pattern_3,
        pattern_4,
        pattern_5
    };

/*****************************************************************************/
//   Description : Draw circle end cap for aa thick lines
//   Global resource dependence : none
//   Author: Geng.Ren
//   Note:   
/*****************************************************************************/
LOCAL void DrawEndCapAA( 
                       int16 x,             //x coordinate of one end point
                       int16 y,             //y coordinage of one end point
                       uint32            color,     //color to draw.
                       uint16            width,
                       void             *mem_ptr,
                       uint16           mem_width,
                       uint16           mem_height,                
                       GUI_RECT_T       *update_rect_ptr
                       )

{
    fixed*      pattern_ptr = PNULL;//patterns[width-1];
    int16 xx, yy, x1, y1, x2, y2, stride;

	if(width > CAP_PATTERN_MAX )
	{
		return;
	}

	pattern_ptr = (fixed*)patterns[width-1]; 

    if ( PNULL == mem_ptr) return;

    width >>= 1;
    stride = (width<<1) + 1;

    x1 = x - width;
    y1 = y - width;
    x2 = x + width;
    y2 = y + width;
    
    for(xx=x1; xx<=x2; xx++)
    {
        for(yy=y1; yy<=y2; yy++)
        {
			blend_color(mem_ptr, mem_width, mem_width, mem_height, xx, yy, color, pattern_ptr[(xx-x1)+(yy-y1)*stride]);
        }
    } 
       
    if (NULL != update_rect_ptr)
    {
        update_rect_ptr->left   = x1;
        update_rect_ptr->top    = y1;
        update_rect_ptr->right  = x2;
        update_rect_ptr->bottom = y2;
    }
}

/*****************************************************************************/
//   Description : Draw circle end cap for aa thick lines
//   Global resource dependence : none
//   Author: Geng.Ren
//   Note:   
/*****************************************************************************/
LOCAL void DrawEndCapAAU32( 
                       int16 x,             //x coordinate of one end point
                       int16 y,             //y coordinage of one end point
                       uint32            color,     //color to draw.
                       uint16            width,
                       void             *mem_ptr,
                       uint16           mem_width,
                       uint16           mem_height,                
                       GUI_RECT_T       *update_rect_ptr
                       )

{
    fixed*      pattern_ptr = PNULL;//patterns[width-1];
    int16 xx, yy, x1, y1, x2, y2, stride;

	if(width > CAP_PATTERN_MAX )
	{
		return;
	}

	pattern_ptr = (fixed*)patterns[width-1]; 

    if ( PNULL == mem_ptr) return;

    width >>= 1;
    stride = (width<<1) + 1;

    x1 = x - width;
    y1 = y - width;
    x2 = x + width;
    y2 = y + width;

    for(xx=x1; xx<=x2; xx++)
    {
        for(yy=y1; yy<=y2; yy++)
        {
			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, xx, yy, color, pattern_ptr[(xx-x1)+(yy-y1)*stride]);
        }
    }     
   
    if (NULL != update_rect_ptr)
    {
        update_rect_ptr->left   = x1;
        update_rect_ptr->top    = y1;
        update_rect_ptr->right  = x2;
        update_rect_ptr->bottom = y2;
    }
}

                         
void DrawThickLineAA(int16           x1,             //x coordinate of first point
                     int16           y1,             //y coordinage of first point
                     int16           x2,             //x coordinate of second point
                     int16           y2,             //y coordinate of sconcd point
                     uint32           color,           //color to draw.
                     uint16           width,           //unit: pixel
                     uint16           *mem_ptr,
                     uint16           mem_width,
                     uint16           mem_height,
                     GUI_RECT_T       *update_rect_ptr)
{
    GUI_RECT_T  rect = {0};

    if((x1==x2)&&(y1==y2))
        return;

    if ( PNULL == mem_ptr) return;
           
    DrawEndCapAA(x1, y1, color, width, mem_ptr, mem_width, mem_height, &rect);
    DrawEndCapAA(x2, y2, color, width, mem_ptr, mem_width, mem_height, &rect);

    {
        int dx = x2 - x1;
        int dy = y2 - y1;

        int len = isqrt(dx*dx+dy*dy);

		fixed slope, span, unit, step;
        fixed start_x, start_y, end_y, end_x, cur_x, cur_y;
        fixed start_x_align, start_y_align, end_x_align, end_y_align;

    	if(ABS(dx)>ABS(dy))
    	{
            if(x1>x2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dy,dx);
            span  = fixed_idiv(width*len,dx)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_x   = fixed_cvt_i2x(x2);
        	start_x = fixed_cvt_i2x(x1);
       		start_y = fixed_cvt_i2x(y1) - span/2 + FIXED_HALF;
            end_y   = fixed_cvt_i2x(y2) + span/2 + FIXED_HALF;

            start_x_align = fixed_floor(start_x);
            end_x_align   = fixed_floor(end_x);
            cur_y         = start_y + fixed_mul(start_x_align-start_x,slope);

        	for(cur_x=start_x_align; cur_x<end_x_align; cur_x+=FIXED_ONE, cur_y+=slope)
        	{
        		blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_y),slope));
        		start_y_align = fixed_floor(cur_y) + unit;
        		if(fixed_mass(cur_y) != fixed_mass(cur_y+slope))
        		{
        			blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+slope), color, FIXED_ONE - coverage(fixed_frac(cur_y+slope),slope));
        			start_y_align += unit;
        		}
        		blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span+slope), color, coverage(fixed_frac(cur_y+span+slope),slope));
        		end_y_align = fixed_floor(cur_y+span+slope);
        		if(fixed_mass(cur_y+span) != fixed_mass(cur_y+span+slope))
        		{
        			blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_y+span),slope));
        			end_y_align -= unit;
        		}
				
				if(slope<0)
				{
				   	for(step=start_y_align; step>end_y_align; step+=unit)
        			{
        				fill_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color);
        			}
				}
				else
				{
				   	for(step=start_y_align; step<end_y_align; step+=unit)
        			{
        				fill_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color);
        			}
				}

        	}
    	}
    	else
    	{
            if(y1>y2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dx,dy);
            span  = fixed_idiv(width*len,dy)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_y   = fixed_cvt_i2x(y2);
        	start_y = fixed_cvt_i2x(y1);
       		start_x = fixed_cvt_i2x(x1) - span/2 + FIXED_HALF;
            end_x   = fixed_cvt_i2x(x2) + span/2 + FIXED_HALF;

            start_y_align = fixed_floor(start_y);
            end_y_align   = fixed_floor(end_y);
            cur_x         = start_x + fixed_mul(start_y_align-start_y,slope);

        	for(cur_y=start_y_align; cur_y<end_y_align; cur_y+=FIXED_ONE, cur_x+=slope)
        	{
        		blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_x),slope));
        		start_x_align = fixed_floor(cur_x) + unit;
        		if(fixed_mass(cur_x) != fixed_mass(cur_x+slope))
        		{
        			blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+slope), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(fixed_frac(cur_x+slope),slope));
        			start_x_align += unit;
        		}
        		blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+span+slope), fixed_cvt_x2i(cur_y), color, coverage(fixed_frac(cur_x+span+slope),slope));
        		end_x_align = fixed_floor(cur_x+span+slope);
        		if(fixed_mass(cur_x+span) != fixed_mass(cur_x+span+slope))
        		{
        			blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+span), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_x+span),slope));
        			end_x_align -= unit;
        		}
				if(slope < 0)
				{
        			for(step=start_x_align; step>end_x_align; step+=unit)
        			{
        				fill_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color);
        			}
				}
				else
				{
        			for(step=start_x_align; step<end_x_align; step+=unit)
        			{
        				fill_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color);
        			}
				}
        	}
    	}

        if (NULL != update_rect_ptr)
        {
            update_rect_ptr->left   = (int16)fixed_cvt_x2i(MIN(start_x,end_x))-width;
            update_rect_ptr->top    = (int16)fixed_cvt_x2i(MIN(start_y,end_y))-width;
            update_rect_ptr->right  = (int16)fixed_cvt_x2i(MAX(start_x,end_x))+width;
            update_rect_ptr->bottom = (int16)fixed_cvt_x2i(MAX(start_y,end_y))+width;
        }
    }    
}

void DrawThickLineAAU32(int16           x1,             //x coordinate of first point
                        int16           y1,             //y coordinage of first point
                        int16           x2,             //x coordinate of second point
                        int16           y2,             //y coordinate of sconcd point
                         uint32           color,           //color to draw.
                         uint16           width,           //unit: pixel
                         uint32           *mem_ptr,
                         uint16           mem_width,
                         uint16           mem_height,
                         GUI_RECT_T       *update_rect_ptr)
{
    GUI_RECT_T  rect = {0};

    if((x1==x2)&&(y1==y2))
        return;

    if ( PNULL == mem_ptr) return;
           
    DrawEndCapAAU32(x1, y1, color, width, mem_ptr, mem_width, mem_height, &rect);
    DrawEndCapAAU32(x2, y2, color, width, mem_ptr, mem_width, mem_height, &rect);

    {
        int dx = x2 - x1;
        int dy = y2 - y1;

        int len = isqrt(dx*dx+dy*dy);

		fixed slope, span, unit, step;
        fixed start_x, start_y, end_y, end_x, cur_x, cur_y;
        fixed start_x_align, start_y_align, end_x_align, end_y_align;

    	if(ABS(dx)>ABS(dy))
    	{
            if(x1>x2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dy,dx);
            span  = fixed_idiv(width*len,dx)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_x   = fixed_cvt_i2x(x2);
        	start_x = fixed_cvt_i2x(x1);
       		start_y = fixed_cvt_i2x(y1) - span/2 + FIXED_HALF;
            end_y   = fixed_cvt_i2x(y2) + span/2 + FIXED_HALF;

            start_x_align = fixed_floor(start_x);
            end_x_align   = fixed_floor(end_x);
            cur_y         = start_y + fixed_mul(start_x_align-start_x,slope);

        	for(cur_x=start_x_align; cur_x<end_x_align; cur_x+=FIXED_ONE, cur_y+=slope)
        	{
        		blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_y),slope));
        		start_y_align = fixed_floor(cur_y) + unit;
        		if(fixed_mass(cur_y) != fixed_mass(cur_y+slope))
        		{
        			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+slope), color, FIXED_ONE - coverage(fixed_frac(cur_y+slope),slope));
        			start_y_align += unit;
        		}
        		blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span+slope), color, coverage(fixed_frac(cur_y+span+slope),slope));
        		end_y_align = fixed_floor(cur_y+span+slope);
        		if(fixed_mass(cur_y+span) != fixed_mass(cur_y+span+slope))
        		{
        			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_y+span),slope));
        			end_y_align -= unit;
        		}
				
				if(slope<0)
				{
				   	for(step=start_y_align; step>end_y_align; step+=unit)
        			{
        				fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color);
        			}
				}
				else
				{
				   	for(step=start_y_align; step<end_y_align; step+=unit)
        			{
        				fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color);
        			}
				}

        	}
    	}
    	else
    	{
            if(y1>y2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dx,dy);
            span  = fixed_idiv(width*len,dy)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_y   = fixed_cvt_i2x(y2);
        	start_y = fixed_cvt_i2x(y1);
       		start_x = fixed_cvt_i2x(x1) - span/2 + FIXED_HALF;
            end_x   = fixed_cvt_i2x(x2) + span/2 + FIXED_HALF;

            start_y_align = fixed_floor(start_y);
            end_y_align   = fixed_floor(end_y);
            cur_x         = start_x + fixed_mul(start_y_align-start_y,slope);

        	for(cur_y=start_y_align; cur_y<end_y_align; cur_y+=FIXED_ONE, cur_x+=slope)
        	{
        		blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_x),slope));
        		start_x_align = fixed_floor(cur_x) + unit;
        		if(fixed_mass(cur_x) != fixed_mass(cur_x+slope))
        		{
        			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+slope), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(fixed_frac(cur_x+slope),slope));
        			start_x_align += unit;
        		}
        		blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+span+slope), fixed_cvt_x2i(cur_y), color, coverage(fixed_frac(cur_x+span+slope),slope));
        		end_x_align = fixed_floor(cur_x+span+slope);
        		if(fixed_mass(cur_x+span) != fixed_mass(cur_x+span+slope))
        		{
        			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+span), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_x+span),slope));
        			end_x_align -= unit;
        		}
				if(slope < 0)
				{
        			for(step=start_x_align; step>end_x_align; step+=unit)
        			{
        				fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color);
        			}
				}
				else
				{
        			for(step=start_x_align; step<end_x_align; step+=unit)
        			{
        				fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color);
        			}
				}
        	}
    	}

        if (NULL != update_rect_ptr)
        {
            update_rect_ptr->left   = (int16)fixed_cvt_x2i(MIN(start_x,end_x))-width;
            update_rect_ptr->top    = (int16)fixed_cvt_x2i(MIN(start_y,end_y))-width;
            update_rect_ptr->right  = (int16)fixed_cvt_x2i(MAX(start_x,end_x))+width;
            update_rect_ptr->bottom = (int16)fixed_cvt_x2i(MAX(start_y,end_y))+width;
        }
    }    
}


void GetLineRect(int16 x1, int16 y1, int16 x2, int16 y2, uint16 width,
                   uint16 mem_width, uint16 mem_height, GUI_RECT_T *update_rect_ptr)
{
    if (NULL != update_rect_ptr)
    {
        if (x1 > x2)
        {
            int16 temp;             
            SWAP(x1, x2, temp);
        }

        if (y1 > y2)
        {
            int16 temp;             
            SWAP(y1, y1, temp);
        }

        x1 -= width;
        x2 += width;
        y1 -= width;
        y2 += width;
        
        update_rect_ptr->left = x1;
        update_rect_ptr->right = x2;
        update_rect_ptr->top = y1;
        update_rect_ptr->bottom = y2;        
    }
}

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
                  GUI_RECT_T       *update_rect_ptr)
{
    uint16  dash_len    = (8 + width * 2);
    uint16  slice_len   = dash_len + 4 + width * 2;
    fixed   f_dash_dx   = 0;
    fixed   f_dash_dy   = 0;
    fixed   f_slice_dx  = 0;
    fixed   f_slice_dy  = 0;
    int16  total_dx = 0;
    int16  total_dy = 0;
    uint16  len = 0;

    if (NULL == mem_ptr)
    {
        return;
    }

    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    len = isqrt(total_dx * total_dx + total_dy * total_dy);

    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (total_dy <= total_dx)
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (x2 < x1)
        {
            int16 temp = x1;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dx, len);
        f_sin = fixed_idiv(total_dy, len);

        f_dash_dx = fixed_scale(dash_len, f_cos);        
        f_dash_dy = fixed_scale(dash_len, f_sin);   
        f_slice_dx = fixed_scale(slice_len, f_cos);
        f_slice_dy = fixed_scale(slice_len, f_sin);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (x1 < x2) 
        {
            f_xn = f_x1 + f_dash_dx;
            f_yn = f_y1 + f_dash_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn);

            if (xn > x2)
            {
                xn = x2;
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32(x1, y1, xn, yn, color, width, mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA(x1, y1, xn, yn, color, width, mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        }
    }
    else
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (y2 < y1)
        {
            int16 temp;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dy, len);
        f_sin = fixed_idiv(total_dx, len);

        f_dash_dy = fixed_scale(dash_len, f_cos);
        f_dash_dx = fixed_scale(dash_len, f_sin);   
        f_slice_dy = fixed_scale(slice_len, f_cos);
        f_slice_dx = fixed_scale(slice_len, f_sin);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (y1 < y2) 
        {
            f_xn = f_x1 + f_dash_dx;
            f_yn = f_y1 + f_dash_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn);

            if (yn > y2)
            {
                xn = x2;
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32(x1, y1, xn, yn, color, width, mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA(x1, y1, xn, yn, color, width, mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        }
    }
}

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
                  GUI_RECT_T       *update_rect_ptr)
{
    uint16  dot_width   = width;
    uint16  slice_len   = dot_width * 2;
    fixed   f_slice_dx  = 0;
    fixed   f_slice_dy  = 0;
    int16  total_dx = 0;
    int16  total_dy = 0;
    uint16  len = 0;
    
    if (NULL == mem_ptr)
    {
        return;
    }

    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    len = isqrt(total_dx * total_dx + total_dy * total_dy);

    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (total_dy <= total_dx)
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (x2 < x1)
        {
            int16 temp = x1;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dx, len);
        f_sin = fixed_idiv(total_dy, len);

        f_slice_dx = fixed_scale(slice_len, f_cos);
        f_slice_dy = fixed_scale(slice_len, f_sin);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (x1 < x2) 
        {
            if (1 == mem_type)
            {
                //draw dot
                DrawDotU32(x1, y1, width, color, mem_ptr, mem_width, mem_height, NULL);  
            }
            else
            {
                //draw dot
                DrawDot(x1, y1, width, color, mem_ptr, mem_width, mem_height, NULL);                
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        }
    }
    else
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (y2 < y1)
        {
            int16 temp;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }
        
        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dy, len);
        f_sin = fixed_idiv(total_dx, len);
        
        f_slice_dx = fixed_scale(slice_len, f_sin);
        f_slice_dy = fixed_scale(slice_len, f_cos);        

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (y1 < y2) 
        {
            if (1 == mem_type)
            {
                //draw dot
                DrawDotU32(x1, y1, width, color, mem_ptr, mem_width, mem_height, NULL);  
            }
            else
            {
                //draw dot
                DrawDot(x1, y1, width, color, mem_ptr, mem_width, mem_height, NULL);     
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        };
    }    
}

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
                      )
{
    uint16  dot_width   = width + 1;
    uint16  dash_len    = (4 + width * 2);
    uint16  blank_len   = 4;
    uint16  dot_len     = dash_len + blank_len + width + dot_width / 2;        
    uint16  slice_len   = dot_len + dot_width / 2 + 1 + blank_len + width;
    fixed   f_dot_dx    = 0;
    fixed   f_dot_dy    = 0;
    fixed   f_dash_dx   = 0;
    fixed   f_dash_dy   = 0;
    fixed   f_slice_dx  = 0;
    fixed   f_slice_dy  = 0;
    int16  total_dx = 0;
    int16  total_dy = 0;
    uint16  len = 0;

    if (NULL == mem_ptr)
    {
        return;
    }

    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    len = isqrt(total_dx * total_dx + total_dy * total_dy);

    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (total_dy <= total_dx)
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (x2 < x1)
        {
            int16 temp = x1;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dx, len);
        f_sin = fixed_idiv(total_dy, len);

        f_dash_dx = fixed_scale(dash_len, f_cos);        
        f_dash_dy = fixed_scale(dash_len, f_sin);   
        f_dot_dx = fixed_scale(dot_len, f_cos);        
        f_dot_dy = fixed_scale(dot_len, f_sin);
        f_slice_dx = fixed_scale(slice_len, f_cos);
        f_slice_dy = fixed_scale(slice_len, f_sin);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (x1 < x2) 
        {
            //////////////////////////////////////////////////////
            f_xn = f_x1 + f_dash_dx;
            f_yn = f_y1 + f_dash_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn);

            if (xn > x2)
            {
                xn = x2;
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32(x1, y1, xn, yn, color, width, mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA(x1, y1, xn, yn, color, width, mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            f_xn = f_x1 + f_dot_dx;
            f_yn = f_y1 + f_dot_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn); 

            if (xn > x2)
            {
                break;
            }
            
            if (1 == mem_type)
            {
                DrawDotU32(xn, yn, dot_width, color, mem_ptr, mem_width, mem_height, NULL);
            }
            else
            {
                DrawDot(xn, yn, dot_width, color, mem_ptr, mem_width, mem_height, NULL);
            }

            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1); 
        }
    }
    else
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (y2 < y1)
        {
            int16 temp;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dy, len);
        f_sin = fixed_idiv(total_dx, len);

        f_dash_dx = fixed_scale(dash_len, f_sin);
        f_dash_dy = fixed_scale(dash_len, f_cos);  
        f_dot_dx = fixed_scale(dot_len, f_sin);        
        f_dot_dy = fixed_scale(dot_len, f_cos);        
        f_slice_dx = fixed_scale(slice_len, f_sin);
        f_slice_dy = fixed_scale(slice_len, f_cos);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (y1 < y2) 
        {
            f_xn = f_x1 + f_dash_dx;
            f_yn = f_y1 + f_dash_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn);

            if (yn > y2)
            {
                xn = x2;
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32(x1, y1, xn, yn, color, width, mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA(x1, y1, xn, yn, color, width, mem_ptr, mem_width,
                                mem_height, NULL);                
            }

            f_xn = f_x1 + f_dot_dx;
            f_yn = f_y1 + f_dot_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn); 

            if (yn > y2)
            {
                break;
            }
            
            if (1 == mem_type)
            {
                DrawDotU32(xn, yn, dot_width, color, mem_ptr, mem_width, mem_height, NULL);
            }
            else
            {
                DrawDot(xn, yn, dot_width, color, mem_ptr, mem_width, mem_height, NULL);
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        };
    }
}