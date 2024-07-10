// modify by lebao music
#pragma once
#ifndef __BMP_FILE_H__
#define __BMP_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define BMP_MAGIC 19778

#define BMP_GET_PADDING(a) ((a) % 4)

typedef enum bmp_error_t
{
	BMP_FILE_NOT_OPENED = -4,
	BMP_HEADER_NOT_INITIALIZED,
	BMP_INVALID_FILE,
	BMP_ERROR,
	BMP_OK = 0
} bmp_error;

typedef struct _bmp_header
{
	unsigned int   bfSize;
	unsigned int   bfReserved;
	unsigned int   bfOffBits;
	
	unsigned int   biSize;
	int            biWidth;
	int            biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int   biCompression;
	unsigned int   biSizeImage;
	int            biXPelsPerMeter;
	int            biYPelsPerMeter;
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;
} bmp_header;

typedef struct _bmp_pixel
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} bmp_pixel;

// This is faster than a function call
#define BMP_PIXEL(r,g,b) ((bmp_pixel){(b),(g),(r)})

typedef struct _bmp_img
{
	bmp_header   img_header;
	bmp_pixel  **img_pixels;
} bmp_img;

// BMP_IMG
void            bmp_img_alloc(bmp_img* img);
void            bmp_img_init_df(bmp_img* img, const int width, const int height);
void            bmp_img_free(bmp_img* img);
void			bmp_pixel_init(bmp_pixel *pxl, const unsigned char red, const unsigned char green, const unsigned char blue);

bmp_error  bmp_img_write(const bmp_img* img, const char *filename);
bmp_error  bmp_img_read(bmp_img* img, const char *filename);
bmp_error  bmp_img_write_buffer(const bmp_img* img, char ** buffer, int* size);

#ifdef __cplusplus
}
#endif
#endif /* __BMP_FILE_H__ */