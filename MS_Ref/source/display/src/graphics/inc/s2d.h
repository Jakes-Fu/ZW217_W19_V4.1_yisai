#ifndef __S2D_H__
#define __S2D_H__
#include "img_ref_types.h"
#define IN
#define OUT
#define INOUT

#define PUBLIC
#define PRIVATE
#define LOCAL static

#define MAX_POLYGON_LINE_NUM	(2048*2)
#define MAX_POLYGON_ALPHA_POINT_NUM	(2048*16)


typedef unsigned char		BOOLEAN;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;
typedef unsigned int		uint;

typedef signed char			int8;
typedef signed short		int16;
typedef signed long int		int32;


#define TRUE   1
#define FALSE  0
#define SCI_TRUE                    TRUE       // Boolean true value
#define SCI_FALSE                   FALSE       // Boolean false value
//#define PNULL	0

#define ABS_A(x) (((x) >= 0)? (x) : (-(x)))

#define GDI_SWAP_2_POINTS(A,B)	(*(A))=(*(A))^(*(B));\
								(*(B))=(*(A))^(*(B));\
								(*(A))=(*(A))^(*(B));
//这个宏定义需要保证
//win_x_end>=win_x_start
//win_y_end>=win_y_start
#define GDI_CHECK_SHOW(X,Y) ((X>=win_x_start)\
					&&(X<win_x_end)\
					&&(Y>=win_y_start)\
					&&(Y<win_y_end))
#define G2D_ALLOC	malloc
#define G2D_FREE	free

#define WINDOW_MAX_X	(32767)
#define WINDOW_MIN_X	(-32767)
#define WINDOW_MAX_Y	(32767)
#define WINDOW_MIN_Y	(-32767)

#define GET_FIX16_X(Point)	(Point.x*65536+Point.x_dec)
#define GET_FIX16_Y(Point)	(Point.y*65536+Point.y_dec)

typedef struct GDI_POINT_t {
    int16	x;
	int16	y;
}GDI_POINT_T;




#endif
