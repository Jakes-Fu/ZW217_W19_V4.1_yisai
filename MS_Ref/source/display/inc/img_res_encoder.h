#ifndef _AMRLOADER_SRC_H
#define _AMRLOADER_SRC_H

//#include <stdio.h>
//#include "stdlib.h"
//#include "windows.h"

typedef struct
{
	unsigned char * img_ptr;
	long img_size;

}SRC_IMG_CONVERT_T;

typedef struct
{
	unsigned char * img_ptr;
	long img_size;

}DES_IMG_CONVERT_T;

typedef enum
{
	IMG_CONVERT_SUCCESS,
	IMG_CONVERT_FAIL,
	IMG_CONVERT_MAX

}IMG_CONVERT_E;

typedef ;



IMG_CONVERT_E bmp32convertargb(SRC_IMG_CONVERT_T * img_src_ptr, DES_IMG_CONVERT_T *img_dst_ptr);
IMG_CONVERT_E gifresconvertor(SRC_IMG_CONVERT_T * img_src_ptr, DES_IMG_CONVERT_T *img_dst_ptr);


#endif




