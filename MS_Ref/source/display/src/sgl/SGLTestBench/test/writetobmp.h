#ifndef __WRITE_TO_BMP_H_
#define __WRITE_TO_BMP_H_

#include "sci_types.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

//data_type = 0:  BGR
//data_type = 1:  ARGB
//data_type = 2:  RGB565
//data_type = 3:  GRAY
int WriteBmp(const char *bmp_name, void *data_ptr,long width, long height, unsigned int data_type);

int ReadFromFile(const char *file_name, void *data_ptr,unsigned int data_size);

#ifdef __cplusplus
    }
#endif

#endif