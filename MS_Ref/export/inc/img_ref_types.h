/******************************************************************************
 ** File Name:      img_ref_types.h                                           *
 ** Author:           Shan.He                                                 *
 ** DATE:             2010-8-25                                               *
 ** Copyright:       	2010 Spreadtrum, Incoporated. All Rights Reserved.      *
 ** Description:     	common types of image reference module                *
 ** Note:                                                				      *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                        *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                            * 
 ** 2010-8-25      Shan.He             Create.                               *
 *****************************************************************************/

#ifndef _IMG_REF_TYPES_H_
#define _IMG_REF_TYPES_H_

#include "sci_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	IMGREF_FORMAT_RGB565 = 0,
	IMGREF_FORMAT_RGB888,
	IMGREF_FORMAT_ARGB888,
	IMGREF_FORMAT_ARGB565,
	IMGREF_FORMAT_ARGB555,
	IMGREF_FORMAT_COMPRESSED_ARGB	,

	IMGREF_FORMAT_YUV422,
	IMGREF_FORMAT_YUV420,
	IMGREF_FORMAT_YUV400
}IMGREF_DATA_FORMAT_E;

typedef enum
{	
	IMGREF_ROTATION_90 = 0,
	IMGREF_ROTATION_180,
	IMGREF_ROTATION_270,
	IMGREF_ROTATION_MIRROR	
}IMGREF_ROTATION_MODE_E;

typedef struct
{
	uint16 		w;
	uint16		h;	
}IMGREF_SIZE_T;

typedef struct
{
	int16 		x;
	int16		y;	
}IMGREF_POINT_T;

typedef struct
{
	int16		x;
	int16		y;
	uint16		w;
	uint16		h;	
}IMGREF_RECT_T;

typedef struct
{
	void		*ptr;
	uint32		size;
}IMGREF_BUF_T;

typedef struct
{
	IMGREF_BUF_T	chn0;
	IMGREF_BUF_T	chn1;
	IMGREF_BUF_T	chn2;
}IMGREF_CHN_T;

typedef struct
{
	int32			min;
	int32			max;
}IMGREF_RANGE_T;

typedef struct
{
	int16 		x;
	int16		y;	
	uint16 		x_dec;
	uint16		y_dec;	

}FIX16_POINT_T;

typedef struct
{
	int16 		x;
	int16		y;	
	int16		z;
	int16	    w;
}IMGREF_POINT_3D_T;

typedef struct
 {
    uint32 y_addr;
    uint32 uv_addr;
    uint32 v_addr;
 }IMGREF_DATA_ADDR_T;

typedef struct
{

    IMGREF_DATA_ADDR_T osd_addr;// y_addr argb888
    IMGREF_RECT_T overlay_rect;
    uint32 color_key; // reserve
    uint32 osd_offset; // reserve
    IMGREF_DATA_FORMAT_E dataformat;
    
}IMGREF_OSD_T;

typedef struct
{
    IMGREF_SIZE_T src_size;
    IMGREF_DATA_ADDR_T src;
    IMGREF_RECT_T src_buf_rect;
    
}IMGREF_YUV_SRC;

								
#ifdef __cplusplus
}
#endif

#endif
