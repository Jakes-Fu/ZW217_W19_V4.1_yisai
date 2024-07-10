#ifndef __READ_RES_IMG_H_
#define __READ_RES_IMG_H_

#include "sci_types.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef struct
{
	char				file_name[256];
	unsigned int		width;
	unsigned int		height;
	unsigned int		format;		//0: argb888, 1: rgb565, 
	unsigned int		size;
}RES_IMG_T;

BOOLEAN ReadResImageFromID(unsigned int id, void *data_ptr, unsigned int data_size,
						   RES_IMG_T *res_img_ptr);

#ifdef __cplusplus
    }
#endif

#endif