#ifndef _SPML_DEF_H__
#define _SPML_DEF_H__

#include <assert.h>
#include <memory.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef __cplusplus
	extern   "C"
   {
#endif

#define PUBLIC
#define LOCAL	static

#ifdef __FONTTOOL

#define RASTER_ERROR_MEMORY_OVERFLOW   (-4)

//#define NULL 0
#define FALSE 0
#define TRUE  1
#define PNULL (0)

#define SCI_ASSERT(_EXP)   assert(_EXP) 

#define SPML_ALLOCA			SCI_ALLOCA
#define SPML_FREE			SCI_FREE
#define SCI_FREE(_PTR)		free(_PTR)
#define SCI_ALLOCA(_SIZE)    malloc(_SIZE) 

		
#define SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE) \
    do  \
    { \
        SCI_ASSERT(PNULL != (_DEST_PTR)); \
        if ((_SIZE)>0) { memcpy((_DEST_PTR), (_SRC_PTR), (_SIZE));} \
    }while(0);

#define SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE) \
    do\
    {\
        SCI_ASSERT(PNULL != _DEST_PTR); \
        if (_SIZE>0) { memset(_DEST_PTR, _VALUE, _SIZE);  }	\
    }while(0);

#endif //__FONTTOOL

#define SPML_Int64           __int64

typedef unsigned char		BOOLEAN;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;
typedef unsigned long int 	uint64;
typedef unsigned int		uint;

typedef signed char			int8;
typedef signed short		int16;
typedef signed long int		int32;

typedef int					SPML_Error;
typedef signed long			SPML_Pos;
typedef signed long         SPML_Fixed;
typedef signed long         SPML_Long;
typedef signed int          SPML_Int;

/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
//SPML_OUTLINE_T flag
#define SPML_CURVE_TAG( flag )  ( (flag) & 3 )

#define SPML_CURVE_TAG_ON           1
#define SPML_CURVE_TAG_CONIC        0
#define SPML_CURVE_TAG_CUBIC        2

#define SUB_GLYPH_POINT_MAX 128
#define SUB_GLYPH_CONTOURS_MAX 32
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/    
typedef int32 SPML_POS_T;

typedef struct
{
    SPML_POS_T  x;
    SPML_POS_T  y;
    
} SPML_VECTOR_T;

typedef struct
{
    SPML_POS_T  xMin, yMin;
    SPML_POS_T  xMax, yMax;
    
} SPML_BBOX_T;

typedef struct
{
	int32  xx;
    int32  xy;
    int32  yx;
    int32  yy;
}SPML_MATRIX_T;


typedef struct
{
    uint16       point_num;        //点个数
    uint16       contour_num;      //轮廓个数
    
    SPML_VECTOR_T* points;        /* the outline's points               */
    uint8*       tags;            /* the points flags                   */
    int16*       contours;        /* the contour end points             */
    
} SPML_OUTLINE_T;

typedef struct
{
	SPML_OUTLINE_T outline;
	int32         max_contours;
    int32         max_points;    
} SPML_OUTLINE_SPEC_T;

typedef struct
{
    int32           rows;
    int32           width;
    int32           pitch;
    uint8*          buffer;
    int16           num_grays;
    int8            pixel_mode;
    int8            palette_mode;
    void*           palette;
    
}SPML_BITMAP_T;

typedef struct
{
    int16   x;
    uint16  len;
    uint8   coverage;
    
} SPML_SPAN_T;

typedef struct
{
    const SPML_BITMAP_T*    target;
    const void*             source;
    SPML_BBOX_T             cbox;
    
    uint8*  vector_buffer_ptr;       //矢量字体处理缓冲区
    uint32  size_of_vector_buffer;   //缓冲区大小

} SPML_RASTER_PARAMS_T;

//typedef SPML_TEXT_METRICS_T SPML_GLYPH_METRICS_T;

typedef uint16 SPML_GLYPH_ID_T;
typedef uint16 SPML_FONT_TYPE_T;
typedef struct SPML_FONT_T*		SPML_FONT_PTR_T;
typedef struct SPML_AF_GLYPHHINTS_tag*  SPML_AF_GLYPHHINTS_PTR_T;
 	
typedef struct
{
    SPML_GLYPH_ID_T               glyph_id;
    SPML_FONT_TYPE_T              type;             /**< the glyph rendering type, see T_FONT_RENDER_TYPE */
    uint16                        style;            /**< glyph style, see T_TEXT_FORMAT */
    int16                         x;                /**< num of pixels in the left side of origin point */
    int16                         y;                /**< num of pixels in the upper side of origin point */
    uint16                        width;            /**< width of bitmap */
    uint16                        height;           /**< height of bitmap */
    uint16                        advance;          /**< num of pixels to origin point of next char */
    uint16                        size;             /**< num of bytes for glyph image data, or font size when outline */
    uint16                        shink_size;       //outline时的缩放比例
    const uint8*                  bits;             /**< the glyph bitmap data, may in compressed format */
    SPML_FONT_PTR_T               font;
    uint8*                        file_data_ptr;
    uint8                         flags;
    BOOLEAN                       is_cache;

	uint16						  code;			//add
//	SPML_AF_GLYPHHINTS_PTR_T	  hints;		//add
}SPML_GLYPH_IMAGE_T;

typedef struct  
{
    const uint8*     table_ptr;          /**< font bitmap tables */
    uint32           table_offset;
    uint32           table_size;         //table_size
    
}SPML_FONT_TABLE_T;

typedef struct SPML_FONT_T
{
    uint32            store_handle;
    uint32            tag;             /**< font tag */
    int32             flags;           /**< font flags */
    int32             em;              /**< font units per em */
    
    SPML_FONT_TABLE_T* table_list;
    uint16            table_count;     /**< count of font bitmap tables */
    uint16            table_idx;       /**< index of current font bitmap table */
    uint32            size;            /**< current font size */

    SPML_AF_GLYPHHINTS_PTR_T	  hints;		//add
}SPML_FONT_T;
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
	}
#endif

#endif //_SPML_DEF_H__