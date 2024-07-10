/******************************************************************************
 ** File Name:      img_gzip.h                                                *
 ** Author:         jianping.wang                                             *
 ** DATE:           4/01/2008                                                 *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains gzip data type and interfaces   *
 **         		description												  *
 ** 							                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 4/01/2008     Jianping.Wang    Create.                                    *
 ******************************************************************************/
 #ifndef _IMG_GZIP_H
 #define _IMG_GZIP_H
 
 #include "os_api.h"
 
 #define IMGGZIP_FILENAME_LEN   20
 typedef unsigned int  ulg;
 
typedef enum
{
	IMGGZIP_ENCODE_SUCCESS,
	IMGGZIP_ENCODE_DST_BUF_SIZE_ERR,
	IMGGZIP_ENCODE_LEVEL_ERR,
	IMGGZIP_ENCODE_SRC_DATA_IS_NULL,
	IMGGZIP_ENCODE_UNKNOWN_ERR
}IMGGZIP_ENCODE_RESULT_E;

typedef enum
{
	IMGGZIP_DECODE_SUCCESS,
	IMGGZIP_DECODE_DST_BUF_SIZE_ERR,
	IMGGZIP_DECODE_METHOD_ERR,
	IMGGZIP_DECODE_HEAD_FLAG_ERR,
	IMGGZIP_DECODE_SRC_DATA_IS_NULL,
	IMGGZIP_DECODE_UNKNOWN_ERR
}IMGGZIP_DECODE_RESULT_E;

typedef enum
{
	IMGGZIP_ENCODE_LEVEL_1 = 1,
	IMGGZIP_ENCODE_LEVEL_2,
	IMGGZIP_ENCODE_LEVEL_3,
	IMGGZIP_ENCODE_LEVEL_4,
	IMGGZIP_ENCODE_LEVEL_5,
	IMGGZIP_ENCODE_LEVEL_6,
	IMGGZIP_ENCODE_LEVEL_7,
	IMGGZIP_ENCODE_LEVEL_8,
	IMGGZIP_ENCODE_LEVEL_9,
	IMGGZIP_ENCODE_LEVEL_MAX	
}IMGGZIP_ENCODE_LEVEL_E;

typedef struct _imggzip_enode_input_param_tag
{	
	uint8  *src_buf_ptr;
	uint8  *dst_buf_ptr;
	uint32  src_size;
    uint32  dst_buf_size;
	uint32  time_stamp;
	IMGGZIP_ENCODE_LEVEL_E level;
	//char    filename[IMGGZIP_FILENAME_LEN];
}IMGGZIP_ENCODE_INPUT_PARAM_T;

typedef struct _imggzip_encode_output_tag
{
	uint32 size;
}IMGGZIP_ENCODE_OUTPUT_T;

typedef struct _imggzip_decode_intput_param_tag
{
	uint8 *src_buf_ptr;
	uint8 *dst_buf_ptr;
	uint32 src_size;
	uint32 dst_buf_size;	
    int (*read_fun)(char *buf,uint32 size);//return -1 when read fail!
    void (*write_fun)(void *buf,uint32 cnt);
}IMGGZIP_DECODE_INPUT_PARAM_T;

typedef struct _imggzip_decode_output_tag
{
	uint32 time_stamp;
//	char *filename_ptr;
	uint32 size;
}IMGGZIP_DECODE_OUTPUT_T;

typedef struct _imggzip_decode_src_param_tag
{
	uint8 *src_buf_ptr;
	uint32 src_size;

}IMGGZIP_DECODE_SRC_PARAM_T;

typedef struct _imggzip_decode_Info_param_tag
{
	uint32 src_img_size;

}IMGGZIP_DECODE_INFO_PARAM_T;


int gzip_initialize(ulg unused_address,ulg length);
int gzip_release(void);

/*****************************************************************************/
//  Description : gzip encode interface
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			input_ptr - input information pointer			 
//		output:
//			output_ptr - output information pointer
//		return:
//			encode result
/*****************************************************************************/
IMGGZIP_ENCODE_RESULT_E	IMGGZIP_Encode(
										IMGGZIP_ENCODE_INPUT_PARAM_T *input_ptr, 
										IMGGZIP_ENCODE_OUTPUT_T 	 *output_ptr
									   );
/*****************************************************************************/
//  Description : gzip decode interface
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			input_ptr - input information pointer			 
//		output:
//			output_ptr - output information pointer
//		return:
//			decode result
/*****************************************************************************/										
IMGGZIP_DECODE_RESULT_E  IMGGZIP_Decode(
										IMGGZIP_DECODE_INPUT_PARAM_T *input_ptr,
										IMGGZIP_DECODE_OUTPUT_T *output_ptr
									   );
									   
/*****************************************************************************/
//  Description : gzip get info
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			input_ptr - input information pointer			 
//		output:
//			output_ptr - output information pointer
//		return:
//			decode result
/*****************************************************************************/	
								  
IMGGZIP_DECODE_RESULT_E  IMGGZIP_Get_Info(
										IMGGZIP_DECODE_SRC_PARAM_T *input_ptr,
										IMGGZIP_DECODE_INFO_PARAM_T *output_ptr
									   );



 
#endif
