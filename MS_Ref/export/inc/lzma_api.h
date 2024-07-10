#ifndef __LZMA_API_H__
#define __LZMA_API_H__
#include "os_api.h"

typedef uint32  LZMA_STATUS;

#define LZMA_SUCCESS     0
#define LZMA_ERROR       1
#define LZMA_ERROR_DATA  2
#define LZMA_ERROR_MEM   3
#define LZMA_ERROR_READ  4
#define LZMA_ERROR_WRITE 5

typedef struct LZMA_INPUT_Tag
{
  unsigned char* zip_buf_addr;
  unsigned int   zip_buf_len;
  unsigned char* unzip_buf_addr;
  unsigned int   unzip_buf_len;
}LZMA_INPUT_T;

typedef struct LZMA_OUTPUT_Tag
{
  unsigned int  unzip_size;
}LZMA_OUTPUT_T;

LZMA_STATUS LZMA_UnZip(LZMA_OUTPUT_T* out_param, LZMA_INPUT_T* in_param);

#endif