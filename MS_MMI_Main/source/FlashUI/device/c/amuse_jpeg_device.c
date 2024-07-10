#include "amuse.h"
#include "jpeg_interface.h"
#ifndef WIN32
//#include <assert.h>
#include "os_api.h"
//#include "flashtask_main.h"
#endif
#include "amuse_file_device.h"
//refrence to Dc_app.c

//static JpegDecInst 			gAmuse_Jpeg_decorder= NULL;
//static int 					gAmuse_jpeg_ret = JPEGDEC_OK;
//static JpegDecIn 			gAmuse_jpeg_in = {0};	
//static JpegDecImageInfo 	gAmuse_image_info = {0};
//static JpegDecOut 			gAmuse_jpeg_out = {0};
	
#define AMUSE_JPEG_DECODE_TEMP_BUFFER_SIZE (200*1024)

int amuse_jpeg_get_dimension(unsigned char *stream, unsigned int jpeg_rgbdata_size,int *image_width, int *image_height)
{
	JINF_GET_INFO_IN_T 	jpeg_info_in = {0};
	JINF_INFO_T			jpeg_info = {0};
	JINF_RET_E			jpeg_ret = JINF_SUCCESS;

	//SCI_ASSERT(stream != NULL);
	//SCI_ASSERT(jpeg_rgbdata_size > 0);
	if(NULL == stream || jpeg_rgbdata_size == 0)
	{
	   *image_width = 0;
	   *image_height = 0;
	   return -1;
	}
	
	jpeg_info_in.jpeg_buf_ptr  = (uint8 *)stream; 	
	jpeg_info_in.jpeg_buf_size = jpeg_rgbdata_size;
	jpeg_ret =  IMGJPEG_GetInfo(&jpeg_info_in, &jpeg_info);

	if(jpeg_ret == JINF_SUCCESS)
	{
		*image_width = jpeg_info.image_width;
		*image_height = jpeg_info.image_height;
		//SCI_ASSERT(*image_width > 0);
		//SCI_ASSERT(*image_height > 0);
		if(*image_width < 0)
		{
		   *image_width = 0;
		}
		if(*image_height < 0)
		{
		   *image_height = 0;
		}
		amuse_printf("FlashTask : Success to dimension the decoder!\n");
		return 0;
	}
	else
	{
		*image_width = 0;
		*image_height = 0;
		amuse_printf("FlashTask : Failed to dimension the decoder. Error code: %d", jpeg_ret);
		//SCI_ASSERT(0);   
		/*lint -unreachable */	 
		return -1;
	}
}

int amuse_decode_jpeg_data(int image_w, int image_h, unsigned char* in_buf, int in_size, unsigned char* out_buf, int out_size)
{
	JINF_DEC_IN_PARAM_T 		jpeg_dec_in = {0};
	JINF_DEC_OUT_PARAM_T		jpeg_dec_out = {0};
	JINF_RET_E					jpeg_ret = JINF_SUCCESS;
	
	//SCI_ASSERT(in_buf != NULL && out_buf != NULL);
    if(NULL == in_buf||NULL == out_buf)
	{
	  return -1;
	}
#ifdef WIN32
	IMGJPEG_Init();
#endif

	jpeg_dec_in.target_width = image_w;
	jpeg_dec_in.target_height = image_h;
	jpeg_dec_in.jpeg_buf_ptr = in_buf;
	jpeg_dec_in.jpeg_buf_size = in_size;
	jpeg_dec_in.target_buf_ptr = out_buf;
	jpeg_dec_in.target_buf_size = out_size + 256;
	jpeg_dec_in.target_data_type = JINF_DATA_TYPE_RGB;
	jpeg_dec_in.target_rgb_format = JINF_FORMAT_RGB565;
	jpeg_dec_in.read_file_func		= PNULL;
	jpeg_dec_in.async_callback		= PNULL;
	jpeg_dec_in.target_data_endian	= JINF_DATA_BIG_ENDIAN;
	jpeg_dec_in.decode_buf_ptr		= (uint8*)SCI_ALLOC_APP(AMUSE_JPEG_DECODE_TEMP_BUFFER_SIZE);
	jpeg_dec_in.decode_buf_size	= (uint32)AMUSE_JPEG_DECODE_TEMP_BUFFER_SIZE;
	SCI_ASSERT(jpeg_dec_in.decode_buf_ptr != NULL);/*assert verified*/
		
	//output info
	/*jpeg_dec_out.output_width = image_w;
    jpeg_dec_out.output_width = image_h;
	jpeg_dec_out.out_data_info.rgb_buf_ptr = out_buf;
	jpeg_dec_out.out_data_info.rgb_format = JINF_FORMAT_RGB565;
	jpeg_dec_out.out_data_info.data_endian = JINF_DATA_BIG_ENDIAN;*/
	
	jpeg_ret = IMGJPEG_Decode(&jpeg_dec_in, &jpeg_dec_out);

	SCI_FREE(jpeg_dec_in.decode_buf_ptr);
	if(JINF_SUCCESS == jpeg_ret)
	{
		amuse_printf("Success to decode a jpeg!\n");
	}
	else
	{
		amuse_printf("Failed to decode a jpeg. Error code: %d\n", jpeg_ret);
		//SCI_ASSERT(0);   
		/*lint -unreachable */
		return -1;
	}
	return 0;
}