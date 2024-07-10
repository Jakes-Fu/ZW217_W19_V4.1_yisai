#ifndef _OD_TYPE_DEF_H_
#define _OD_TYPE_DEF_H_
//类型定义
typedef unsigned char OD_U8;
typedef unsigned short int OD_U16;
typedef unsigned int OD_U32;
typedef signed char OD_S8;
typedef signed short int OD_S16;
typedef signed int OD_S32;
typedef float  OD_FLOAT;
typedef double OD_DOUBLE;
typedef unsigned char OD_BOOL;
typedef void (*OD_FuncPtr) (void);

#define OD_TRUE  (1)
#define OD_FALSE (0)
#define OD_NULL  (0)
#ifndef NULL
#define NULL      (0)
#endif
#ifndef FALSE
#define FALSE           (0)
#endif
#ifndef TRUE
#define TRUE            (1)
#endif

#ifndef null
#define null      (0)
#endif
#ifndef false
#define false           (0)
#endif
#ifndef true
#define true            (1)
#endif

typedef struct
{
    OD_S16   left;
    OD_S16   top;
    OD_S16   right;
    OD_S16   bottom;
}od_game_rect_t;

typedef struct
{
    OD_U8 r; // 0 ~ 255
    OD_U8 g; // 0 ~ 255
    OD_U8 b; // 0 ~ 255
}od_game_color;

typedef enum
{
	OD_GAME_SIM_NONE,
	OD_GAME_SIM_1,
	OD_GAME_SIM_2,
	OD_GAME_SIM_3,
	OD_GAME_SIM_4,
	OD_GAME_SIM_FLASE	
}OD_GAME_SIM_TYPE;    
#endif
