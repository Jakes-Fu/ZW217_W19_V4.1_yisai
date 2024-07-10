#include "writetobmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

#define RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define ARGB_GET_R(_color) ((uint8)((_color) >> 16))
#define ARGB_GET_G(_color) ((uint8)((_color) >> 8))
#define ARGB_GET_B(_color) ((uint8)(_color))

#define ARGB_SET_R(_color, _r) ((_color) | ((_r) << 16))
#define ARGB_SET_G(_color, _g) ((_color) | ((_g) << 8))
#define ARGB_SET_B(_color, _b) ((_color) | (_b))

void DumpLong(char **dst, long LongNumber, int NumByte)
{
   unsigned c;
   long LocalLongNumber;
   int i;
   char *ptr = *dst;

   LocalLongNumber = LongNumber;

   for(i = 0; i < NumByte ;i++) {
      c = LocalLongNumber & 0xFF;
	  LocalLongNumber = LocalLongNumber >> 8;
      *ptr++ = c;
   }

    *dst = ptr;
}

int DumpBitmap(void *data_ptr, long width, long height,FILE *fpbmp)
{
   int i;
   int flag;
   int ByteNumPerPixel;
   long FileSize;
   long LocalLongNum;
   char *temp_ptr = NULL;
   char header[100] = {0};
   char *header_ptr = header;
   char *temp_line_buf = NULL;
   int  stride = width * 3;

   flag = 0;
   ByteNumPerPixel = 3;
   FileSize = width * height * ByteNumPerPixel + 54;

   /* File Header */

   *header_ptr++ = 'B';
   *header_ptr++ = 'M';

   DumpLong(&header_ptr, FileSize, 4);
   LocalLongNum = 0;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = 54;
   DumpLong(&header_ptr, LocalLongNum, 4);

   /* Image Header */
   LocalLongNum = 40;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = (long) width;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = (long) height;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = 1;
   DumpLong(&header_ptr, LocalLongNum, 2);
   LocalLongNum = ByteNumPerPixel * 8;
   DumpLong(&header_ptr, LocalLongNum, 2);
   LocalLongNum = 0;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = width * height * ByteNumPerPixel;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = 0;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = 0;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = 0;
   DumpLong(&header_ptr, LocalLongNum, 4);
   LocalLongNum = 0;
   DumpLong(&header_ptr, LocalLongNum, 4);

   fwrite(header, 1, 54, fpbmp);

   if (width & 3)
   {
       stride = (stride + 3) / 4 * 4;
       temp_line_buf = malloc(stride);
       memset(temp_line_buf, 0, stride);

       temp_ptr = (char *)data_ptr + (height - 1) * width * 3;
       for(i=0;  i< height; i++) 
       {       
            memcpy(temp_line_buf, temp_ptr, width * 3);
            fwrite(temp_ptr, 1, stride, fpbmp);
            temp_ptr -= (width * 3);
       }
   }
   else
   {
       temp_ptr = (char *)data_ptr + (height - 1) * width * 3;
       for(i=0;  i< height; i++) 
       {       
            fwrite(temp_ptr, 1, width * 3, fpbmp);
            temp_ptr -= (width * 3);
       }
   }

   if (temp_line_buf)
       free(temp_line_buf);

   return (flag);
}

int Write2Bmp(const char *bmp_name, void *data_ptr,long width, long height)
{
    FILE *fpbmp = NULL;

    if (NULL == data_ptr || height <= 0 || height <= 0)
    {
        return 1;
    }

    fpbmp = fopen(bmp_name, "w+b");

    if (NULL == fpbmp)
    {
        return 1;
    }

    DumpBitmap(data_ptr, width, height, fpbmp);

    fclose(fpbmp);

    return 0;
}

void RGB5652BGR888(unsigned char *bgr_ptr, unsigned short *rgb565_ptr, long width, long height)
{
    long i, j;

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            unsigned short rgb565 = *rgb565_ptr++;

            *bgr_ptr++ = RGB565_GET_B(rgb565);
            *bgr_ptr++ = RGB565_GET_G(rgb565);
            *bgr_ptr++ = RGB565_GET_R(rgb565);            
        }
    }   
}

void ARGB2BGR888(unsigned char *bgr_ptr, unsigned int *argb_ptr, long width, long height)
{
    long i, j;

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            unsigned int argb = *argb_ptr++;
            
            *bgr_ptr++ = ARGB_GET_B(argb);
            *bgr_ptr++ = ARGB_GET_G(argb);
            *bgr_ptr++ = ARGB_GET_R(argb);            
        }
    }   
}

void GRAY2BGR888(unsigned char *bgr_ptr, unsigned char *gray_ptr, long width, long height)
{
    long i, j;

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            unsigned char color = *gray_ptr++;
            
            *bgr_ptr++ = color;
            *bgr_ptr++ = color;
            *bgr_ptr++ = color;            
        }
    }   
}

//data_type = 0:  BGR
//data_type = 1:  ARGB
//data_type = 2:  RGB565
//data_type = 3:  GRAY
int WriteBmp(const char *bmp_name, void *data_ptr,long width, long height, unsigned int data_type)
{
	long	bgr_mem_size = width * height * 3;
	void 	*bgr_mem_ptr = NULL;
	void	*bmp_data_ptr = NULL;

	if (data_type > 3)
	{
		return 1;
	}

	if (data_type > 0)
	{
		bgr_mem_ptr = (void *)malloc(bgr_mem_size);

		if (!bgr_mem_ptr)
		{
			return 2;
		}
	}

	switch (data_type)
	{
	case 0:		
		bmp_data_ptr = data_ptr;
		break;

	case 1:
		ARGB2BGR888((unsigned char *)bgr_mem_ptr, (unsigned int *)data_ptr, width, height);	
		bmp_data_ptr = bgr_mem_ptr;
		break;

	case 2:
		RGB5652BGR888((unsigned char *)bgr_mem_ptr, (unsigned short *)data_ptr, width, height);
		bmp_data_ptr = bgr_mem_ptr;
		break;

	case 3:
		GRAY2BGR888((unsigned char *)bgr_mem_ptr, (unsigned char *)data_ptr, width, height);
		bmp_data_ptr = bgr_mem_ptr;
		break;

	default:		
		break;
	}

	Write2Bmp(bmp_name, bmp_data_ptr, width, height);

	if (bgr_mem_ptr)
	{		
		free(bgr_mem_ptr);
		bgr_mem_ptr = NULL;
	}

	return 0;	
}