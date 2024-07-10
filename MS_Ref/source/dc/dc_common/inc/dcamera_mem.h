/*******************************************************************************
 ** FileName:      Dcamera_mem.h                                                *
 ** Author:        Baggiohe                                                     *
 ** Date:          2011/1/21                                                   *
 ** CopyRight:     2011, Spreatrum, Incoporated, All right reserved            *
 ** Description:   memory management for DC                                    *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2011/1/21      Baggiohe               Created                          *
 *******************************************************************************/

#ifndef _DCAMERA_MEM_H_
#define _DCAMERA_MEM_H_

#ifdef __cplusplus
	extern "C"
	{
#endif

#include "sci_types.h"
#define DC_JPEG_YUV_PADDING_BYTES           64                 
#define DC_THUMBNAIL_LENGTH                 (12*1024)
#define DC_EXIF_LENGTH                      (24*1024)  
#if defined(PLATFORM_SC8800G)
#define DCAMERA_PADDING_MEM_SIZE            (2*1024)
#else
#define DCAMERA_PADDING_MEM_SIZE            (4)
#endif

#define DC_MEM_ALIGNED_(a,n)                (((a) + (n) -1) / (n) * (n))
#define DC_ADDR(a)                          DC_MEM_ALIGNED_((uint32)a, DCAMERA_PADDING_MEM_SIZE)

typedef struct dcamera_mem_man_param_tag
{
    uint16    sensor_out_width;
    uint16    sensor_out_height;
//    uint16    sensor_trim_width;
//    uint16    sensor_trim_height;
    uint16    image_width;
    uint16    image_height;
    uint16    review_width;
    uint16    review_height;
    uint16    slice_height;
    uint8     is_jpg_sensor;
    uint8     burst_number;
    uint8     capture_rot;
    uint8     review_rot;
    uint8     b_add_exif;
    uint8     b_add_thumbnail;
    uint8     b_personal_cap;
    uint8     image_format;
}DCAMERA_MEM_PARAM_T;

PUBLIC uint32 DCamera_MemConfig_CaptureYBuf(uint16 sensor_out_w, uint16 sensor_out_h);   
PUBLIC void   DCamera_MemFree_Ybuffer(void);
PUBLIC uint32 DCamera_MemConfig(uint32 op_mode, uint32 *p_mem_size);
PUBLIC void   DCamera_MemFree(void);
PUBLIC void*  DCAMERA_PreviewGetMem(DCAMERA_MEM_PARAM_T* p_cap_mem_cfg, uint32 *p_buf_len);
PUBLIC uint32 DCAMERA_CaptureMemReq(DCAMERA_MEM_PARAM_T* p_cap_mem_cfg);
PUBLIC void*  DCAMERA_CaptureGetStartBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetOSDBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetYBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetUVBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetYUVBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetYUVRotBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetQuickViewBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetJPGBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetJPGZoomBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetJPGTmpBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetLineBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetSwapBufY(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetSwapBufUV(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetSliceBufY(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetSliceBufUV(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetSrcSliceBufY(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetSrcSliceBufUV(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetThumBufY(uint32* p_buf_len);
PUBLIC void*  DCAMERA_CaptureGetThumBufUV(uint32* p_buf_len);
PUBLIC uint32 DCAMERA_CaptureGetBurstNum(void);
PUBLIC void*  DCAMERA_CaptureExGetScaleBuf(uint32* p_buf_len);
PUBLIC void*  DCAMERA_PreviewExGetBuf(uint32* p_buf_len);
PUBLIC BOOLEAN   DCAMERA_Capture2JpegRev(uint16 img_width, 
                                                                        uint16 img_height, 
                                                                        uint8 img_fmt, 
                                                                        uint8 disp_fmt,
                                                                        uint16 review_width, 
                                                                        uint16 review_height, 
                                                                        uint8 rot);

#ifdef __cplusplus
    }
#endif

#endif  /*#ifndef _DCAMERA_MEM_H_*/

