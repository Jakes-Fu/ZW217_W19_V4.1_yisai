/*****************************************************************************
** File Name:      spml_outline.h                                            *
** Author:                                                                   *
** Date:           31/10/2009                                                *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file provide functions about font handle text line   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                    NAME             DESCRIPTION                      *
** 31/10/2009              James.Zhang         Create                        *
******************************************************************************/
#ifndef SPML_OUTLINE_H
#define SPML_OUTLINE_H
/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#ifdef __FONTTOOL
#include "spml_def.h"
#else
#include "spml_api.h"
#endif

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef __cplusplus
	extern   "C"
    {
#endif

#ifndef __FONTTOOL
/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
//SPML_OUTLINE_T flag
#define SPML_CURVE_TAG( flag )  ( (flag) & 3 )

#define SPML_CURVE_TAG_ON           1
#define SPML_CURVE_TAG_CONIC        0
#define SPML_CURVE_TAG_CUBIC        2
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
    SPML_BBOX_T             clip_box;
    
    uint8*  vector_buffer_ptr;       //矢量字体处理缓冲区
    uint32  size_of_vector_buffer;   //缓冲区大小

} SPML_RASTER_PARAMS_T;

#endif // __FONTTOOL

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init spml
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:初始化显示引擎
/*****************************************************************************/
PUBLIC int32 SPML_GrayRasterRender( 
                                   const SPML_RASTER_PARAMS_T*  params
                                   );

/*****************************************************************************/
// 	Description : init spml
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:初始化显示引擎
/*****************************************************************************/
PUBLIC int32 SPML_OutlineGetBBox( 
                                 SPML_OUTLINE_T*  outline,  
                                 SPML_BBOX_T  *abbox,
                                 SPML_BBOX_T  *acbox
                                 );


/*****************************************************************************/
// 	Description : init spml
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:初始化显示引擎
/*****************************************************************************/
PUBLIC void SPML_OutlineTranslate( 
                                  const SPML_OUTLINE_T*  outline,
                                  SPML_POS_T        xOffset,
                                  SPML_POS_T        yOffset
                                  );

/*****************************************************************************/
// 	Description : init spml
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:初始化显示引擎
/*****************************************************************************/
PUBLIC void SPML_OutlineTransform( 
                                  const SPML_OUTLINE_T*  outline,
                                  const SPML_MATRIX_T*   matrix
                                  );

/*****************************************************************************/
// 	Description : init spml
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:初始化显示引擎
/*****************************************************************************/
PUBLIC void SPML_Vector_Transform( 
								  SPML_VECTOR_T*        vector,
								  const SPML_MATRIX_T*  matrix
								  );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
	}
#endif

#endif
