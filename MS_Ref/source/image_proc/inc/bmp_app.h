#ifndef _BMP_DECODE_APP_H_
#define _BMP_DECODE_APP_H_

#include "sci_types.h"

#ifdef WIN_TEST

#include "scaling_down_cus.h"


#else

#include "os_api.h"
#include "scaling_down_cus.h"

#endif


#if 0
	
typedef struct bmp_decode_app_t_
{
	uint8 *decode_buf_ptr;  //
	uint8 *target_buf_ptr;  
	uint8 *file_buf_ptr;
	
	
	
	uint8 *src_ptr;      //test use, not useful for future

	int32 decode_buffer_size;  //decode buffer size, must be larger than 96*1024
	int32 target_buf_ptr_size; 
	int32 file_buf_ptr_size;   //file buffer , muest be larger than 32 * 1024
	
	
	
	int32 *src_width_ptr;
	int32 *src_height_ptr;
	int32 *scr_color_depth_ptr;  //2;4;8;24;
	int32 src_img_size;
			
	int32 target_width;  	
	int32 target_height;
	P_DATA_TYPE_CUS target_type;//RGB565 ; //RGB888

	int32 *ret_target_width_ptr;
	int32 *ret_target_height_ptr;


	
	
	void (*app_read_data)(uint8 *data_ptr, uint32 data_count,  uint32 *ret_data_count_ptr);

	
    
}BMP_DECODE_APP_T;







typedef enum
{
	BMP_APP_SUCCESS = 0,
	BMP_APP_ERROR_TYPE,
	BMP_APP_ERROR_FILE_HEADER,
	BMP_APP_ERROR_INFO_HEADER,
	BMP_APP_ERROR_NO_ENOUGH_DATA,
	BMP_APP_ERROR_SIZE,
	BMP_APP_ERROR_NO_SUPPORT,

	BMP_APP_ERROR_SCALING,

	BMP_APP_NO_MEMORY,
	BMP_APP_ERROR_UN_KNOWN,

	BMP_APP_ERROR_MAX = 128
}BMP_APP_ERROR;


/*****************************************************************************/
// 	Description :create bmp app memory
//	Global resource dependence : 
//  Author: Hansen
//	Note:
/*****************************************************************************/

PUBLIC BMP_APP_ERROR bmp_create_app(uint32 target_width );

/*****************************************************************************/
// 	Description :create wbmp app memory
//	Global resource dependence : 
//  Author: Hansen
//	Note:
/*****************************************************************************/

PUBLIC BMP_APP_ERROR wbmp_create_app(uint32 target_width);


/*****************************************************************************/
// 	Description : BMP Decoder
//	Global resource dependence : 
//  Author: Hansen
//	Note:
/*****************************************************************************/

PUBLIC BMP_APP_ERROR  bmp_decode_app(BMP_DECODE_APP_T *bmp_decode_app_ptr);
/*****************************************************************************/
// 	Description : WBMP Decoder
//	Global resource dependence : 
//  Author: Hansen
//	Note:
/*****************************************************************************/

#endif
#endif

