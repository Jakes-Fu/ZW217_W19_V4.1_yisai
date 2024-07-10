#ifndef _JPEG_RES_DECODER_H_
#define _JPEG_RES_DECODER_H_

#include "dal_img.h"


IMG_RES_ERROR_E JPEG_GetImageInfo(IMG_RES_SRC_T  *img_src_ptr, IMG_RES_INFO_T *img_info_ptr);

IMG_RES_ERROR_E JPEG_Display_Img(IMG_RES_SRC_T  *img_src_ptr, IMG_RES_DST_T *img_disp_ptr);


#endif