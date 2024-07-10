#include "amuse_jpeg_hw_decoder.h"
#include "amuse_jpeg_device.h"
#include "amuse_file_device.h"
#define JPEG_HW_DECODER_DEBUG 0

/*
  * The interface of getting Jpeg dimension info (width & height) by hardware.
  * This interface should be implemented by every platform with hardware Jpeg Codec.
  * Brad, 2006-11-14
  */
int swf_jpeg_get_dimension_HW(unsigned char *jpeg_buf,unsigned int buf_size, int *width, int *height)
{
	int i=0;

#if JPEG_HW_DECODER_DEBUG
	amuse_printf("");
	amuse_printf("Original Jpeg data:");
	amuse_printf("Jpeg stream = %d, size = %d", (int)jpeg_buf, (int)buf_size);
	amuse_printf("Jpeg stream start with: %d, %d, %d, %d",jpeg_buf[0],jpeg_buf[1],jpeg_buf[2],jpeg_buf[3]);
	amuse_printf("Jpeg stream end  with: %d, %d, %d, %d",jpeg_buf[buf_size-4],jpeg_buf[buf_size-3],jpeg_buf[buf_size-2],jpeg_buf[buf_size-1]);
#endif

	if(buf_size>6)
	{
		while(i<6){
			if(jpeg_buf[i]==0xff && jpeg_buf[i+1]==0xd9)
			{
				jpeg_buf[i+1]=0xd8;
			}
			i+=2;
		}
	}

#if JPEG_HW_DECODER_DEBUG
	amuse_printf("Recovered Jpeg data:");
	amuse_printf("Jpeg stream = %d, size = %d", (int)jpeg_buf, (int)buf_size);
	amuse_printf("Jpeg stream start with: %d, %d, %d, %d",jpeg_buf[0],jpeg_buf[1],jpeg_buf[2],jpeg_buf[3]);
	amuse_printf("Jpeg stream end  with: %d, %d, %d, %d",jpeg_buf[buf_size-4],jpeg_buf[buf_size-3],jpeg_buf[buf_size-2],jpeg_buf[buf_size-1]);
	amuse_printf("Getting into amuse_jpeg_get_dimension !!");
#endif
	
	if(amuse_jpeg_get_dimension(jpeg_buf, buf_size, width, height)<0)
	{
		amuse_printf("amuse_jpeg_get_dimension FAILED !!");
		return -1;
	}

	if(*width==0 || *height==0){
		amuse_printf("Getting into SW get_dimension >>>>>> !!");
#ifndef WIN32 
		swf_jpeg_get_dimension_Aid(jpeg_buf, buf_size, width, height);/*lint -e628*/
#endif
	}

#if JPEG_HW_DECODER_DEBUG
	amuse_printf("Jpeg width=%d, height=%d", *width, *height);
#endif
	
	return 0;
}

/*
  * The interface of decoding Jpeg data by hardware.
  * This interface should be implemented by every platform with hardware Jpeg Codec.
  * Brad, 2006-11-14
  */
int swf_jpeg_draw_to_buffer_HW(int width, int height,
                                         unsigned char *jpeg_src, unsigned int jpeg_src_size, 
                                         unsigned char *dest_buf, int dest_buf_size)
{
	int res=0;

	
	
	res = amuse_decode_jpeg_data(width, height, jpeg_src, jpeg_src_size, 
		dest_buf, dest_buf_size);
	
	if(res<0)
	{
		amuse_printf("amuse_decode_jpeg_data FAILED, res = %d !!",res);
		return -1;
	}

#if JPEG_HW_DECODER_DEBUG
	amuse_printf("amuse_decode_jpeg_data SUCCEEDED !!");
	amuse_printf("");
#endif
	
	return 0;
}
