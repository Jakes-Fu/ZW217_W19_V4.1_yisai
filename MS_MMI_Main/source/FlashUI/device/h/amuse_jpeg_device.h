#ifndef _AMUSE_JPEG_DEVICE_H_
#define _AMUSE_JPEG_DEVICE_H_

int amuse_jpeg_init();
int amuse_jpeg_destroy();
int amuse_jpeg_get_dimension(unsigned char *stream, unsigned int jpeg_rgbdata_size,int *image_width, int *image_height);
int amuse_decode_jpeg_data(int image_w, int image_h, unsigned char* in_buf, int in_size, unsigned char* out_buf, int out_size);

#endif
