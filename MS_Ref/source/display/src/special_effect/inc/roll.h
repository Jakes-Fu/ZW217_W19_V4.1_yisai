#ifndef _ROLL_H_
#define _ROLL_H_

#include "sci_types.h"
#include "img_ref_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{ 
    int16            top;
    int16            left;
    uint16           row_inx;
    uint16           clmn_inx;
    uint32*           icon_img_buf;
    
}ROLL_ICON_IMAGE;

typedef struct
{
    uint16           row_num;
    uint16           clmn_num;
    uint16           icon_w;
    uint16           icon_h;
    ROLL_ICON_IMAGE  *icon_ptr;
    
}ROLL_ICONS_PARAM;

typedef struct
{

	IMGREF_SIZE_T			src_size;			//source image size
	IMGREF_RECT_T			src_rect;			//reserved

	uint32					*src_buf_ptr;		//source image buffer, if it is NULL, use strip image
	
	uint32					**src_slice_pptr;	//valid if src_buf_ptr is NULL. Divide the source image to serveral strips. 
	uint32					slice_num;			//strip numbers
	uint32					slice_height;		//size of each strip
	ROLL_ICONS_PARAM        icons_para;         //valid if src_buf_ptr and src_slice_pptr are NULL. Divide the source image to serveral icon images
	                                            //icon image is saved according to column

	uint32					*target_buf_ptr;	//target buffer pointer
	IMGREF_SIZE_T			target_size;		//target image size
	IMGREF_RECT_T			target_rect;		//rectangle to receive output image

	int32					angle[2];
	IMGREF_POINT_3D_T		src_spindle[2];		// spindle position of source image
	IMGREF_POINT_3D_T		target_spindle[2];	// spindle position of target image
	IMGREF_POINT_3D_T		eye;				//position of eye
}ROLL_INFO_T;

PUBLIC int32 ROLL_GetFrame(ROLL_INFO_T *roll_info_ptr);

#ifdef __cplusplus
}

#endif

#endif