/******************************************************************************
 ** File Name:		jpeg_head_info.h                                            *
 ** Author:			Shan.He		                                          		*
 ** DATE:			10/24/2009                                                *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:	parse jpeg head									        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** -------------------------------------------------------------------------*
 ** DATE              NAME             DESCRIPTION                          *
 ** 2009-10-24		  Shan.He		   Created								*
 ******************************************************************************/

#ifndef _JPEG_HEAD_INFO_H_
#define _JPEG_HEAD_INFO_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "jpeg_interface.h"
#include "jpeg_stream.h"
#include "jpeg_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Structures		                                  *
 **---------------------------------------------------------------------------*/
#define HUFF_BITS_COUNT_LEN     16
#define HUFF_VALUE_LEN          256
#define MAX_COMPONENT_NUM       3

typedef struct 
{
    uint8       marker;
    uint32      offset;
    uint32      size;
}JPEG_APP_SECTION;

typedef struct 
{
    uint8       huff_bits_count_tbl[16];            
    uint8       huff_value_tbl[256];     
}JPEG_HUFFMAN_TABLE_T;

typedef struct  
{   
    JPEG_HUFFMAN_TABLE_T    ac_table[4];
    JPEG_HUFFMAN_TABLE_T    dc_table[4];
    uint32                  ac_table_num;           //number of AC table 
    uint32                  dc_table_num;           //number of DC table
}JPEG_DHT_INFO_T;

typedef struct
{
    uint8                   quat_tbl[64];
}JPEG_QUAT_TABLE_T;

typedef struct  
{
    JPEG_QUAT_TABLE_T       tables[4];
    uint32                  table_num;
}JPEG_DQT_INFO_T;

typedef struct 
{
    uint32                  component_id;
    uint32                  dc_huff_table;
    uint32                  ac_huff_table;
}JPEG_SOS_COMPONENT_INFO_T;

typedef struct 
{
    uint32                      component_count;
    JPEG_SOS_COMPONENT_INFO_T   component_info[MAX_COMPONENT_NUM];
    uint32                      spectral_selection_start;
    uint32                      spectral_selection_end;
    uint32                      successive_approximate;
}JPEG_SOS_INFO_T;

typedef struct 
{
    uint32                  restart_interval;
}JPEG_DRI_INFO_T;

typedef struct
{
    uint16	            width;
	uint16	            height;
	JINF_JPEG_TYPE_E	file_type;
	JINF_YUV_FORMAT_E 	sample_format;	
	BOOLEAN				is_rotated_yuv422;
}JPEG_BASIC_INFO_T;

typedef struct 
{
	JINF_READ_FILE_FUNC	read_file_func;
	uint32				read_file_size;
	uint8 				*stream_buf;
	uint32				stream_buf_size;
}JPEG_PARSE_HEAD_IN_PARAM_T;

typedef struct
{
    JPEG_BASIC_INFO_T       *basic_info_ptr;
    JINF_THUMBNAIL_INFO_T   *thumbnail_info_ptr;
    JINF_EXIF_INFO_T        *exif_info_ptr;
    JPEG_DHT_INFO_T         *dht_info_ptr;
    JPEG_DQT_INFO_T         *dqt_info_ptr;
    JPEG_DRI_INFO_T         *dri_info_ptr;
    JPEG_SOS_INFO_T         *sos_info_ptr;
    uint32                  head_size;
}JPEG_HEAD_INFO_T;

/*****************************************************************************
**	Name : 			Jpeg_ParseHead
**	Description:	Parse jpeg marker
**	Author:			Shan.he
**  Parameters:     
**                  in_param_ptr:       pointer of parse head input parmaters
**                  head_info_ptr:      pointer of the jpeg head info structure,
**                                      output parameter
**	Note:           context_ptr and basic_info_ptr must be valid while the  
**                  thumbnail_info_ptr and exif_info_ptr could be invalid 
**                  indicating the relative information be ignored.
**                  if the jpeg stream is in the buffer, set the context_ptr->stream_buf 
**		            and context_ptr->stream_buf_size as the source JPEG buffer 
**                  address and size; 
**		            if the jpeg stream is in the file, set the context_ptr->stream_buf 
**		            and context_ptr->stream_buf_size as the address and size of the 
**		            read buffer used to receive JPEG stream read from file, 
**                  16K bytes is the recommended size of the read buffer. 
**                  set the context_ptr->read_file_func as the function pointer 
**                  of read file.
*****************************************************************************/
PUBLIC JPEG_RET_E Jpeg_ParseHead(JPEG_PARSE_HEAD_IN_PARAM_T *in_param_ptr,
								 JPEG_HEAD_INFO_T *head_info_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
