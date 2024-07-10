#ifndef ALPHA_RES_DECODER_H_
#define ALPHA_RES_DECODER_H_

#include "sci_types.h"
#include "dal_img.h"


typedef struct
{
	uint16 interval;
	uint16 framenum;
	
}MOV_ARGB_INFO_T;

typedef struct
{
	uint8 *argb_ptr;
	uint32 argb_size;
       IMG_RECT_T imgrect;

	uint16 interval;
	uint16 frame_num;
	
}MOV_ARGB_HANDLER_T;


IMG_RES_ERROR_E ALPHA_RES_MOV_GetImageInfo(IMG_RES_SRC_T  *img_src_ptr, IMG_RES_INFO_T *img_info_ptr);
IMG_RES_ERROR_E ALPHA_RES_Create_Handler(IMG_RES_SRC_T  *img_src_ptr, uint32* Handler);
IMG_RES_ERROR_E ALPHA_RES_Free_Handler(uint32 Handler);
IMG_RES_ERROR_E ALPHA_RES_Display_MOV(uint32 Handler, IMG_RES_DST_T *img_dst_ptr, IMG_RES_INFO_T *img_info_ptr);

IMG_RES_ERROR_E ALPHA_RES_GetImageInfo(IMG_RES_SRC_T  *img_src_ptr, IMG_RES_INFO_T *img_info_ptr);
IMG_RES_ERROR_E ALPHA_RES_Display_Img(IMG_RES_SRC_T  *img_src_ptr, IMG_RES_DST_T *img_dst_ptr);

#ifdef WRE_SUPPORT
int ARGB_Decoder_AlphaEx(uint16* src_ptr, uint16* rgb_ptr,uint8* alpha_ptr);
#endif
#endif