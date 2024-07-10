#ifndef _DC_COMMON_H_
#define _DC_COMMON_H_

#include "dal_dcamera.h"
#include "sci_types.h"
#include "img_ref_types.h"

typedef struct _dc_convert_coordinate_tag_
{
    DCAMERA_RECT_T rect;
    DCAMERA_ROTATION_MODE_E rot_degree;
}DCAMERA_CONVERT_COOR_T;

/******************************************************************************
// Description: dc one pixel yuv to rgb888
// Author:   Tim.zhu
// Input:     y: the value of y
//              uv: the value of u
//              v: the value of v
// Output:   rgb0/rgb0: the value of rgb888
// Return:   SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DCAMERA_PixelYuv2Rgb888(uint8 y0, uint8 y1, uint8 uv, uint8 v, uint32* rgb0, uint32* rgb1);

/******************************************************************************
// Description: dc two pixel rgb888 to yuv
// Author:      Tim.zhu
// Output:      rgb0/rgb1: the value of rgb888
// Input:        y: the value of y
//                 uv: the value of u
//                 v: the value of v
// Return:      SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DCAMERA_PixelRgb8882Yuv(uint32 rgb0, uint32 rgb1, uint8* y0, uint8* y1,uint8* uv, uint8* v);

/******************************************************************************
// Description: dc yuv data overlayer for capture data
// Author:       Tim.zhu
// Input:         DCAMERA_YUV_SRC_T_PTR: the point of src yuv information
// Output:       none
// Return:        SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DCAMERA_ArgbOverlayer(IMGREF_YUV_SRC *param_ptr, BOOLEAN src_yuv_mode, IMGREF_OSD_T *capture_date_ptr);

/******************************************************************************
// Description: convert the coordinate 
// Author:       Baggio.He
// Input:         LCD_ID_E:lcd_id
                     DCAMERA_CONVERT_COOR_T *src_ptr
// Output:       DCAMERA_CONVERT_COOR_T *dst_ptr
// Return:        SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC void DCAMERA_Convert_Coor( LCD_ID_E lcd_id, DCAMERA_CONVERT_COOR_T *src_ptr, DCAMERA_CONVERT_COOR_T *dst_ptr);

/******************************************************************************/
//  Description: calculate the channel size of the specified data format
//  Author: 	Shan.He 
//  Input:   
//			width:		image width
//			height:		image height
//			format:		image data format
//			extra_bytes:  extra bytes added to each channel
//  Output: 
//          chn_ptr:	pointer of the channel information. the pointer of each channel is invalid
//  Return:	TRUE    		successful
//  		FALSE          	failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 DCAMERA_GetChnSize(IMGREF_CHN_T *chn_ptr, uint16 width, uint16 height, 
									IMGREF_DATA_FORMAT_E format, uint32 extra_bytes);

#endif


