

#ifndef DCAMERA_JPG_CODEC_H_
#define DCAMERA_JPG_CODEC_H_

#include "dal_jpeg.h"


#define POS_Y1	0
#define POS_U	1
#define POS_Y2	2
#define POS_V	3

//#define JPEG_BASE_BUFFER_LEN (10*1024)

typedef enum
{
	JPG_ENC_MODULE,
	JPG_DEC_MODULE
};



/******************************************************************************
// Description:	Software YUYV scaling,special for VGA --> QVGA
// Author:		frank.yang
// Input:      
			pYUYV			--pointer to source yuv422 data		
			nWidth			--width of source yuv422 data	
			nHeight			--height of source yuv422 data	
			pOutYUYV		--pointer to target yuv422 data			
			tWidth			--width of target yuv422 data	
			tHeight			--height of target yuv422 data	

// Return:  
// Note:   pYUYV=pOutYUYV is allowed    	
******************************************************************************/
PUBLIC int DCamera_Scale_VGA_to_QVGA(uint8* pYUYV, uint16 nWidth,uint16 nHeight,
									uint8* pOutYUYV,uint16 tWidth,uint16 tHeight);



/******************************************************************************
// Description:	Software jpeg encoder
// Author:		frank.yang
// Input:      
			in_param		--in struct pointer to encoding parameters		
			out_param		--out struct pointer to encoding parameters	
			format			--format,YUV_422/YUV_420/YUV_422_MCU/YUV_420_MCU

// Return:  result of encoding,refer JPEG_ENC_RET_E
// Note:       	
******************************************************************************/
PUBLIC int DCamera_EncodeJpeg(JPEG_ENC_IN_PARAM_T *in_param,
					JPEG_ENC_OUT_PARAM_T *out_param);

/******************************************************************************
// Description:	Software jpeg decoder
// Author:		frank.yang
// Input:      
			in_param		--in struct pointer to decoding parameters		
			out_param		--out struct pointer to decoding parameters	
// Return:  result of encoding,refer JPEG_ENC_RET_E
// Note:       	
******************************************************************************/
PUBLIC int DCamera_DecodeJpeg(JPEG_DEC_IN_PARAM_T* in_param,
					JPEG_DEC_OUT_PARAM_T* out_param);


/******************************************************************************
// Description:	jpeg info
// Author:		frank.yang
// Input:      
			in_param		--in struct pointer to decoding parameters		
			out_param		--out struct pointer to jpeg parameters	
// Return:  result of encoding,refer JPEG_DEC_RET_E
// Note:       	
******************************************************************************/
PUBLIC int DCamera_GetImageInfo(JPEG_DEC_IN_PARAM_T* in_param,
								JPEG_DEC_IMAGEINFO_T* image_param);

#endif