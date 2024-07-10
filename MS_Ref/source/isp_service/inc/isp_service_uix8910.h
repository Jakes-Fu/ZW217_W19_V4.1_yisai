/******************************************************************************
 ** File Name:    isp_service_sc6530.h                                               *
 ** Author:       Baggio.He                                                   *
 ** DATE:         3/31/2010                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  SC6530 ISP service,                                        *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/31/2010     Baggio.he       Initial version                             *
 *****************************************************************************/
 
#ifndef _ISP_SERVICE_H_
#define _ISP_SERVICE_H_

#include "isp_drv.h"

#define ISP_QXGA_H                      1536
#define ISP_UXGA_W                      1600
#define ISP_UXGA_H                      1200
#define ISP_SXGA_H                      1024
#define ISP_SXGAM_H                     960
#define ISP_XGA_H                       768
#define ISP_SVGA_H                      600
#define ISP_QSVGA_H                     512
#define ISP_VGA_W                       640
#define ISP_VGA_H                       480
#define ISP_QCIF_W                      176
#define ISP_QCIF_H                      144

#define ISP_DISPLAY_NONE                0xFF
#define ISP_SCALE_PARAM_T               ISP_SCLAE_PARAM_T

typedef int (*get_data)( ISP_ADDRESS_T addr, uint32 width, uint32 height);

typedef enum
{
    ISP_CB_PROC_DONE = 0,
    ISP_CB_PROC_WAIT,
    ISP_CB_PROC_QUIT
}ISP_CB_RTN;

typedef enum
{
    ISP_ERR_CODE_NO_ERR = 0,    
    ISP_ERR_CODE_NO_MEM = ISP_DRV_RTN_MAX + 1,
    ISP_ERR_CODE_ST_ERR,
    ISP_ERR_CODE_HW_ERR,
    ISP_ERR_CODE_MAX
}ISP_ERR_CODE;

typedef enum
{
    ISP_ROTATION_0 = 0,
    ISP_ROTATION_90,
    ISP_ROTATION_180,
    ISP_ROTATION_270,
    ISP_ROTATION_MIRROR,
    ISP_ROTATION_MAX
}ISP_ROTATION_E;

typedef enum
{
    ISP_SERVICE_CALLBACK_ENCODE = 1,
    ISP_SERVICE_CALLBACK_DISPLAY = 2,
    ISP_SERVICE_CALLBACK_POSTCAP = 3,

}ISP_SERVICE_CALLBACK_ID_E;

typedef enum
{
    ISP_SERVICE_SCALE_NORMAL = 0,           
    ISP_SERVICE_SCALE_SLICE,
    ISP_SERVICE_SCALE_MODE_MAX
}ISP_SERVICE_SCALE_MODE_E;

typedef enum
{
    ISP_IMAGE_FORMAT_YUV422 = 0,
    ISP_IMAGE_FORMAT_YUV420,
    ISP_IMAGE_FORMAT_YUV400,    
    ISP_IMAGE_FORMAT_YUV420_3FRAME,        
    ISP_IMAGE_FORMAT_RGB565,
    ISP_IMAGE_FORMAT_RGB888,
    ISP_IMAGE_FORMAT_CCIR656,
    ISP_IMAGE_FORMAT_JPEG,
    ISP_IMAGE_FORMAT_RAW,
    ISP_IMAGE_FORMAT_RGB666,
    ISP_IMAGE_FORMAT_RGB565_EX,
    ISP_IMAGE_FORMAT_RGB888_EX,
    ISP_IMAGE_FORMAT_MAX
}ISP_IMAGE_FORMAT;

typedef enum
{
     ISP_VT_MEM_MODE_NORM = 0,
     ISP_VT_MEM_MODE_LOW,
     ISP_VT_MEM_MODE_MAX
}ISP_VT_MEM_MODE;

typedef enum
{
    ISP_VT_MEMBLK_IDLE = 0,

    ISP_VT_MEMBLK_CAPTURING,
    
    ISP_VT_MEMBLK_CAPTURE_DONE,

    ISP_VT_MEMBLK_PROCING_SRC,

    ISP_VT_MEMBLK_PROCING_DST,

    ISP_VT_MEMBLK_INVALIDING,

    ISP_VT_MEMBLK_STATE_MAX,
}ISP_VT_MEM_BLOCK_STATE;

typedef struct
{
    uint32                              cap_img_format;
    uint32                              cap_img_pattern;    
    uint32                              cap_vsync_pol;
    uint32                              cap_hsync_pol;    
    uint32                              cap_pclk_pol;
    uint32                              cap_frame_skipped;    
    uint32                              cap_frame_decimated;    
    uint32                              cap_sensor_if_mode; 
    uint32                              cap_sensor_if_endian; 
    uint16                              cap_atv_thre_start;
    uint16                              cap_atv_thre_end; 
    uint8                               cap_atv_thre_eb;
    uint8                               b_is_atv;
    uint8                               ccir_pclk_src;
    uint8                               ccir_d0_src;
    uint8                               ccir_vsync_src;
    uint8                               ccir_d1_src;
    uint8                               res[2];
}ISP_CAP_PARAM_T;

typedef struct
{
    ISP_SIZE_T                          input_size;
    ISP_RECT_T                          input_range;

    uint32                              disp_lcd_id;
    uint32                              disp_block_id;
    ISP_RECT_T                          disp_range;
    ISP_ROTATION_E                      disp_rotation;// 0: 0; 1: 90; 2: 180; 3: 270
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
    ISP_CAP_PARAM_T                     cap_param;
}ISP_PREVIEW_PARAM_T;

typedef struct
{
    ISP_SIZE_T                          input_size;
    ISP_RECT_T                          input_range;
    ISP_SIZE_T                          output_size;
    uint32                              yuv_type;           
    //0:   not shif128, output data is signed; 1 shift128, out put data is unsigned.
    //1    for hantro use
    //0xff for raw rgb debug
    uint32                              skip_frame_num;
    ISP_IMAGE_FORMAT                    sensor_data_format;  
    ISP_CAP_PARAM_T                     cap_param;
    
}ISP_JPEG_PARAM_T;

typedef struct
{
    ISP_RECT_T                          input_range;
    ISP_IMAGE_FORMAT                    input_format;
    ISP_SIZE_T                          output_size;
    uint32                              yuv_type; 
                                       
}ISP_COMPOUND_JPEG_CAPTURE_PARAM_T;

typedef struct
{
    ISP_SIZE_T                          input_size;
    ISP_RECT_T                          input_range;
    ISP_IMAGE_FORMAT                    input_format;
    ISP_ADDRESS_T                       input_addr; //input address
    uint32                              disp_lcd_id;
    uint32                              disp_block_id;
    ISP_RECT_T                          disp_range;
    ISP_ROTATION_E                      disp_rotation; // 0: 0; 1: 90; 2: 180; 3: 270
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
}ISP_REVIEW_PARAM_T;

typedef struct
{
    ISP_SIZE_T                          input_size;
    ISP_RECT_T                          input_rect;
    ISP_IMAGE_FORMAT                    input_format; //422/420/400    
    ISP_ADDRESS_T                       input_addr;
    uint32                              input_buf_size;
    ISP_IMAGE_FORMAT                    output_format; //422/420/400/RGB        
    ISP_SIZE_T                          output_size;
    ISP_ADDRESS_T                       output_addr;
    uint32                              output_buf_size;  
    uint32                              swap_buf_addr;
    uint32                              line_buf_addr;    
    uint32                              slice_line;
    ISP_SERVICE_SCALE_MODE_E            scale_mode;    
    BOOLEAN                             is_first;    
    BOOLEAN                             is_last;
    get_data                            get_scale_data; 

    int32                               (*start_scale_callback)(int32 param);   //scale start user callback
    int32                               start_scale_param;                      //scale start user callback parameter
    
}ISP_SCLAE_PARAM_T;

typedef struct
{
    ISP_SIZE_T                          input_size;
    ISP_RECT_T                          input_rect;
    ISP_IMAGE_FORMAT                    input_format; //422 only    
    ISP_ADDRESS_T                       input_addr;
    ISP_IMAGE_FORMAT                    output_format; //422 only        
    ISP_SIZE_T                          output_size;
    ISP_ADDRESS_T                       output_addr;
    uint8                              y_thres_val;
    uint8                              uv_thres_val;    
}ISP_DENOISE_PARAM_T;

typedef struct
{
    ISP_SIZE_T                          input_size;
    ISP_RECT_T                          input_range;
    ISP_SIZE_T                          encode_size;
    ISP_ROTATION_E                      encode_rotation;    
    uint32                              yuv_type;
    uint32                              disp_lcd_id;
    uint32                              disp_block_id;
    uint32                              is_display;
    ISP_RECT_T                          disp_range;
    ISP_ROTATION_E                      disp_rotation;
    ISP_IMAGE_FORMAT                    data_format;
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
    ISP_CAP_PARAM_T                     cap_param;
    
}ISP_MPEG_PARAM_T;

typedef struct
{
    //local 
    ISP_SIZE_T                          local_input_size;
    ISP_RECT_T                          local_input_range;
    ISP_SIZE_T                          encode_size;
    ISP_ROTATION_E                      encode_rotation;
    uint32                              yuv_type;
    uint32                              local_disp_lcd_id;
    uint32                              local_disp_block_id;
    ISP_RECT_T                          local_disp_range;
    ISP_ROTATION_E                      disp_rotation;
    uint32                              local_review_enable;    
    uint32                              local_display_enable;
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
    ISP_CAP_PARAM_T                     cap_param;
    ISP_VT_MEM_MODE                     mem_mode;         
    
}ISP_VT_ENCODE_PARAM_T;

typedef struct
{
    //remote    
    ISP_SIZE_T                          remote_input_size;
    ISP_RECT_T                          remote_input_range;
    ISP_IMAGE_FORMAT                    remote_in_format;
    uint32                              remote_disp_lcd_id;
    uint32                              remote_disp_block_id;
    ISP_RECT_T                          remote_disp_range;
    ISP_ROTATION_E                      remote_disp_rotation;
    ISP_ADDRESS_T                       input_addr; //input address
    uint32                              deblock_enable;
    uint32                              remote_review_enable;       
    uint32                              remote_display_enable;  
    ISP_IMAGE_FORMAT                    remote_out_format;
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
}ISP_VT_DECODE_PARAM_T;

typedef struct
{
    ISP_SIZE_T                          input_size;
    ISP_RECT_T                          input_rect;
    ISP_IMAGE_FORMAT                    input_format; //422/420/400    
    ISP_ADDRESS_T                       input_addr;

}ISP_DISPLAY_PARAM_T;


void ISP_ServiceInit(void);

uint32 ISP_ServiceOpen(uint32 wait_option);


uint32 ISP_ServiceClose(void);
void ISP_ServiceSetClk(uint32 clk);


uint32 ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr);

uint32 ISP_ServiceGetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr);

uint32 ISP_ServiceStartPreview(void);


void ISP_ServiceStopPreview(void);

void ISP_ServiceStartReview(void);

uint32 ISP_ServiceReviewNext(ISP_ADDRESS_T *addr_ptr);

void ISP_ServiceReviewBypass(void);

uint32 ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T *param_ptr);
void    ISP_ServiceReviewFromDC(uint32 b_from_dc);


uint32 ISP_ServiceSetJpegCaptureParam(ISP_JPEG_PARAM_T * param_ptr);


void ISP_ServiceStartCaptureJpeg(void);


uint32 ISP_ServiceSetCompoundJpegCaptureParam(ISP_COMPOUND_JPEG_CAPTURE_PARAM_T * param_ptr);


void ISP_ServiceStartCaptureCompoundJpeg(void);


uint32 ISP_ServiceSetMpegCaptureParam(ISP_MPEG_PARAM_T * param_ptr);

uint32 ISP_ServiceGetMpegCaptureParam(ISP_MPEG_PARAM_T * param_ptr);

uint32 ISP_ServiceStartCaptureMpeg(void);

BOOLEAN ISP_ServiceIsMpegOn(void);

void ISP_ServiceStopCapture(void);

void ISP_ServiceStopReview(void);


uint32 ISP_ServiceSetScaleParam(ISP_SCLAE_PARAM_T * param_ptr);

void ISP_ServiceStartScale(void);


uint32 ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ID_E eid, int (*func)(void *));


uint32 ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_ID_E eid);


uint32 ISP_ServiceGetFrameAddress(void);


void ISP_ServiceFreeFrameAddress(ISP_FRAME_T *frame_ptr);

uint8 ISP_ServiceIsFrameLocked(ISP_FRAME_T *frame_ptr);
int32 ISP_ServiceFrameLock(ISP_FRAME_T *f);
int32 ISP_ServiceFrameUnLock(ISP_FRAME_T *f);

void ISP_ServiceStartVT(void);
uint32 ISP_ServiceSetVTDecodeAddr(ISP_ADDRESS_T *addr_ptr);
uint32 ISP_ServiceSetVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr);
uint32 ISP_ServiceSetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);
uint32 ISP_ServiceGetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);

void ISP_ServiceStopVT(void);

void ISP_ServiceStartVTExt(void);
void ISP_ServiceStartVTExtLowMem(void);
void ISP_ServiceContinueVTExtLowMem(void);

uint32 ISP_ServiceSetVTExtEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);
uint32 ISP_ServiceSetVTExtScaleParam(ISP_SCLAE_PARAM_T * param_ptr);
void ISP_ServiceStartVTExtScale(void);

void ISP_ServiceStopVTExt(void);
BOOLEAN ISP_ServiceIsVTOn(void);

uint32 ISP_ServiceGetOutputHeightAlign(ISP_DATA_FORMAT_E data_format);
uint32 ISP_ServiceGetOutputWidthAlign(ISP_DATA_FORMAT_E data_format, BOOLEAN is_slice);
uint32 ISP_ServiceGetInputHeightAlign(uint32 input_height, uint32 output_height);
uint32 ISP_ServiceGetInputWidthAlign(uint32 input_width, 
                                     uint32 input_height, 
                                     uint32 output_width, 
                                     uint32 output_height);
uint32 ISP_ServiceGetScaleSliceHeightAlign(uint32 input_width, 
                                           uint32 output_width,
                                           uint32 input_height, 
                                           uint32 output_height);
BOOLEAN ISP_ServiceIsScaleSupported(ISP_SCALE_PARAM_T * param_ptr);

uint32 ISP_ServiceSetDenoiseParam(ISP_DENOISE_PARAM_T * param_ptr);
void ISP_ServiceStartDenoise(void);

void ISP_ServiceAsyncDisplay(ISP_FRAME_T *frame_ptr);
void ISP_ServiceMemConfig(void* p_isp_mem, uint32 mem_length);
void* ISP_ServiceGetMem(uint32 *p_mem_length);
void ISP_ServiceMemConfig_YBuffer(void* p_isp_mem, uint32 mem_length);

PUBLIC  uint32 ISP_ServiceGetVtMemBlockState(uint32 block_id);
PUBLIC  void ISP_ServiceSetVtMemBlockState(uint32 block_id, uint32 state);
PUBLIC  ISP_ADDRESS_T * ISP_ServiceGetVtMemBlock(uint32 block_id);
PUBLIC  uint32 ISP_ServiceGetVtMemBlockId(uint32 * block_id_ptr, uint32 state);

/*****************************************************************************/
//  Description: Is ISP idle
//  Global resource dependence:
// Author: 
// input parameter:     
//                      None
// output parameter:
//                      None
// return:              TRUE: idle. 
//                      FALSE: busy
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISP_ServiceIsIdle(void);

uint8 ISP_ServiceGetLastErr(void);

void ISP_ServiceDisplayOneFrame(ISP_DISPLAY_PARAM_T *param_ptr);

int ISP_UixOpenCameraController(int open);

int _ISP_UixCaptureOneFrame();

void ISP_Uix_SetBaseaddr(uint32 addr, uint32 size);


void ISP_UixSetDcamMode(int mode);

int ISP_UixGetDcamMode();

#endif//_ISP_SERVICE_H_
