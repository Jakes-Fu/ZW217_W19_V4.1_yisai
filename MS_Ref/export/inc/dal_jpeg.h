/******************************************************************************
 ** File Name:       dal_jpeg.h                                               *
 ** Author:          		                                                  *
 ** DATE:            04/10/2006                                               *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME             DESCRIPTION                            *
 ******************************************************************************/


#ifndef _DAL_JPEG_H
#define _DAL_JPEG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/*
 * Data Type struct 
 */
typedef enum
{
    JPEG_ENC_FRAME_READY = 1,
    JPEG_ENC_RESTART_INTERVAL = 2,
    JPEG_ENC_OK = 0,
    JPEG_ENC_ERROR = -1,
    JPEG_ENC_NULL_ARGUMENT = -2,
    JPEG_ENC_INVALID_ARGUMENT = -3,
    JPEG_ENC_MEMORY_ERROR = -4,
    JPEG_ENC_INVALID_STATUS = -5,
    JPEG_ENC_OUTPUT_BUFFER_OVERFLOW = -6,
    JPEG_ENC_EWL_ERROR = -7,
    JPEG_ENC_EWL_MEMORY_ERROR = -8,
    JPEG_ENC_HW_ERROR = -9,
    JPEG_ENC_HW_TIMEOUT = -10,
    JPEG_ENC_INSTANCE_ERROR = -11
} JPEG_ENC_RET_E;

typedef enum
{
	JPEG_DEC_VPD_FRAME_READY_STRM_ERROR = 4,
	JPEG_DEC_FRAME_READY_STRM_ERROR = 3,
	JPEG_DEC_VPD_FRAME_READY = 2,
    JPEG_DEC_FRAME_READY = 1,
    JPEG_DEC_OK = 0,
    JPEG_DEC_ERROR = -1,
    JPEG_DEC_NULL_ARGUMENT = -2,
    JPEG_DEC_INVALID_ARGUMENT = -3,
    JPEG_DEC_MEMORY_ERROR = -4,
    JPEG_DEC_INVALID_STATUS = -5,
    JPEG_DEC_DWL_ERROR = -6,
    JPEG_DEC_DWL_MEMORY_ERROR = -7,
    JPEG_DEC_HW_ERROR = -8,
    JPEG_DEC_HW_TIMEOUT = -9,
    JPEG_DEC_INSTANCE_ERROR = -10,
    JPEG_DEC_UNSUPPORTED = -11

}JPEG_DEC_RET_E;

typedef enum
{
	JPEG_ENC_DATA_TYPE_YUV422 = 0,
	JPEG_ENC_DATA_TYPE_YUV420,
	JPEG_ENC_DATA_TYPE_YUV444,
	JPEG_ENC_DATA_TYPE_YUV422_BLOCK,
	JPEG_ENC_DATA_TYPE_YUV420_BLOCK,
	JPEG_ENC_DATA_TYPE_YUV422_MCU,
	JPEG_ENC_DATA_TYPE_YUV420_MCU,
	JPEG_ENC_DATA_TYPE_MAX
}JPEG_ENC_DATA_TYPE_E;
typedef enum
{
	JPEG_DEC_DATA_TYPE_YUV420 = 0,
	JPEG_DEC_DATA_TYPE_YUV422,
	JPEG_DEC_DATA_TYPE_YUV444,
	JPEG_DEC_DATA_TYPE_YUV411,
	JPEG_DEC_DATA_TYPE_RGB565,
	JPEG_DEC_DATA_TYPE_MAX
}JPEG_DEC_DATA_TYPE_E;
typedef enum
{
	JPEG_DATA_BIG_ENDIAN = 0,
	JPEG_DATA_LITTLE_ENDIAN,
	JPEG_DATA_ENDIAN_MAX
}JPEG_DATA_ENDIAN_E;

typedef enum
{
	JPEG_DCT_NORMAL = 0,
	JPEG_DCT_FAST,
	JPEG_DCT_MAX
}JPEG_DCT_MODE_E;
typedef struct
{
	uint32 y_chn_addr;	// Y data channel address 
	uint32 u_chn_addr;	// U data channel address
	uint32 v_chn_addr;	// V data channel address
}JPEG_YUV_CHN_ADDR_T;

typedef struct
{
	uint16 x;			// x offset of rectangle
	uint16 y;			// y offset of rectangle
	uint16 w;			// width of rectangle
	uint16 h;			// height of rectangle
}JPEG_RECT_T;

typedef struct jpeg_enc_in_param_tag
{
   JPEG_YUV_CHN_ADDR_T	yuv_addr;						// yuv data address
   JPEG_RECT_T			image_rect;						// image rectangle to encoded
   uint32 				quality_level;					//encode quantization level  0-8
   uint8* 				jpeg_buf_ptr;					//address of Output Buffer
   uint32 				jpeg_buf_size;					//size of Output Buffer

   JPEG_ENC_DATA_TYPE_E enc_data_type;					//type of yuv data to be encoded
   JPEG_DATA_ENDIAN_E	endian_type;					//endian format of yuv data
   JPEG_DCT_MODE_E		dct_mode;						//normal or fast dct
   uint32               enc_slice_height;

   void*               p_jpg_tmp;
   uint32              jpg_tmp_len;
   uint32              reserved;
   
}JPEG_ENC_IN_PARAM_T;

typedef struct jpeg_enc_out_param_tag
{
   uint32 out_size;							//size of Output JPEG data
}JPEG_ENC_OUT_PARAM_T;


typedef struct jpeg_dec_in_param_tag
{
   	uint32* 				jpeg_buf;						// address of jpeg buffer
	uint32 					jpeg_size;						// size of jpeg data
	JPEG_DEC_DATA_TYPE_E	dec_data_type;					// data type of output data after jpeg decode 
	JPEG_DCT_MODE_E			dct_mode;						//normal or fast idct

	uint32* 				output_buf;						// when data type is JPEG_DEC_DATA_TYPE_RGB565,need this buffer
	uint32* 				yuv_buf;	//if caller indicate the output yuv addr 
	uint32 				yuv_size;	//the size of yuv_buf if we get from caller 
	uint32              target_width;
	uint32              target_height;    
    void*               p_jpg_tmp;
    uint32              jpg_tmp_len;
}JPEG_DEC_IN_PARAM_T;

typedef struct jpeg_dec_outparam_tag
{
   	uint32 				output_width;		// width of output image
	uint32 				output_height;		// height of output image
	union 
	{
		JPEG_YUV_CHN_ADDR_T	yuv_addr;			// yuv data address
		uint32 				rgb565_addr;		// rgb565 data address
	}data_addr;
   	JPEG_DEC_DATA_TYPE_E    output_data_type;

}JPEG_DEC_OUT_PARAM_T;

typedef struct jpeg_dec_disp_param_tag
{
   uint32 DisplayWidth;						//Width of display image
   uint32 DisplayHeight;					//Height of display image
   uint32 OutputFormat;						//0:YUV420;1:RGB565
   
}JPEG_DEC_DISP_PARAM_T;




typedef struct
{
    uint32 outpWidth;        				//image Width
    uint32 outpHeight;       				//image Heigth
    uint32 Xdensity;						//density in X
    uint32 Ydensity;						//density in Y
}JPEG_DEC_IMAGEINFO_T;

typedef struct jpeg_zoom_in_param_tag
{
    uint8   *jpeg_buf_ptr;		    //jpeg stream buffer		
	uint32  jpeg_buf_size;			//jpeg stream size

    JPEG_RECT_T crop_rect;
        
   	uint16  target_width;           //target width
	uint16  target_height;	        //target height
    uint32  quality;

    uint8   *target_buf_ptr;        //target buffer, if it is NULL, get target buffer from decode_buf_ptr
    uint32  target_buf_size;        //target buffer size.

    uint8   *decode_buf_ptr;   
    uint32  decode_buf_size;   
}JPEG_ZOOM_IN_PARAM_T;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                               
 **---------------------------------------------------------------------------*/
 

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:  	Encode YUV420 image data to a JPEG file .
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC  JPEG_ENC_RET_E IMG_JPEG_EncodeJpeg(JPEG_ENC_IN_PARAM_T *in_param,JPEG_ENC_OUT_PARAM_T *out_param);

/*****************************************************************************/
//  Description:  Get the JPEG image information from a JPEG file in buffer.
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC JPEG_DEC_RET_E IMG_JPEG_GetImageInfo(JPEG_DEC_IN_PARAM_T *in_param,JPEG_DEC_IMAGEINFO_T *image_info);

/*****************************************************************************/
//  Description:  Decode JPEG file to YUV420 or RGB565 image data.
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC JPEG_DEC_RET_E IMG_JPEG_DecodeJpeg(JPEG_DEC_IN_PARAM_T  *in_param, JPEG_DEC_OUT_PARAM_T *out_param );

/*****************************************************************************/
//  Description:  Zoom JPEG file to appointed target.
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int JPEG_ZoomJpeg(JPEG_ZOOM_IN_PARAM_T *pIn, uint8 **pJpg, uint32 *size);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // _DAL_JPEG_H
