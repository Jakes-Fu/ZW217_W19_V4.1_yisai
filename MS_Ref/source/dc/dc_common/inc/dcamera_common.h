/*******************************************************************************
 ** FileName:      dc_context.h                                                *
 ** Author:        Zhemin.Lin&Benny.Zhou                                       *
 ** Date:          2005/8/04                                                   *
 ** CopyRight:     2005, Spreatrum, Incoporated, All right reserved            *
 ** Description:   define DCAM context                                         *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2005/8/04      Zhemin.Lin&Benny.Zhou      Created                          *
 ** 2006/4/13      Liangwen.Zhen              Modify
 *******************************************************************************/

#ifndef DCAMERA_CONTEXT_H
#define DCAMERA_CONTEXT_H

#ifdef __cplusplus
    extern "C"
    {
#endif

#include "os_api.h"
#include "dal_dcamera.h"
#include "deep_sleep.h"
#include "sensor_drv.h"
#include "isp_service.h"

#define DC_PREVIEW_FRAME_BUF_MAX   4
#define DC_DISPLAY_FRAME_BUF_MAX   2
#define DC_DISPLAY_ROUND_BUF_MIN    3
/**---------------------------------------------------------------------------*
 **                         Macro definition                                  *
 **---------------------------------------------------------------------------*/
#define QQVGA_WIDTH     (160)
#define QQVGA_HEIGHT    (120)
#define QVGA_WIDTH      (320)
#define QVGA_HEIGHT     (240)
#define WQVGA_WIDTH     (400)
#define WQVGA_HEIGHT    (240)
#define WQVGA_SIZE      (WQVGA_WIDTH*WQVGA_HEIGHT*2)
#define VGA_WIDTH       (640)
#define VGA_HEIGHT      (480)
#define VGA_SIZE        (VGA_WIDTH*VGA_HEIGHT*2)
#define SVGA_WIDTH      (800)
#define SVGA_HEIGHT     (600)
#define SVGA_SIZE       (SVGA_WIDTH*SVGA_HEIGHT*2)
#define XGA_WIDTH       (1024)
#define XGA_HEIGHT      (768)
#define XGA_SIZE        (XGA_WIDTH*XGA_HEIGHT*2)
#define QVGA_SIZE       (VGA_SIZE/4)
#define SXGA_WIDTH      (1280)
#define SXGA_HEIGHT     (960)
#define SXGA_SIZE       (SXGA_WIDTH*SXGA_HEIGHT*2)
#define UXGA_WIDTH      (1600)
#define UXGA_HEIGHT     (1200)
#define UXGA_SIZE       (UXGA_WIDTH*UXGA_HEIGHT*2)
#define QXGA_WIDTH      (2048)
#define QXGA_HEIGHT     (1536)
#define QXGA_SIZE       (QXGA_WIDTH*QXGA_HEIGHT*2)

/**---------------------------------------------------------------------------*
 **                         Data Structure definition                         *
 **---------------------------------------------------------------------------*/
typedef enum
{
    DCAMERA_SAVE_MODE_BUF = 1,
    DCAMERA_SAVE_MODE_FFS,
    DCAMERA_SAVE_MODE_BOTH,
    
    DCAMERA_SAVE_MODE_MAX
}DCAMERA_SAVE_MODE_E;

// enum: conversion type
typedef enum
{
    DCAMERA_CONV_TYPE_YUV420_2_YUV420 = 0x00,   
    DCAMERA_CONV_TYPE_YUV420_2_RGB565,
    DCAMERA_CONV_TYPE_RGB565_2_RGB565,
    DCAMERA_CONV_TYPE_RGB565_2_YUV420,
        
    DCAMERA_CONV_TYPE_MAX   
}DCAMERA_CONV_TYPE_E;

// Image format conversion Structure
typedef struct
{
    DCAMERA_CONV_TYPE_E conv_type;
    uint16              src_width;          // width of source image, multiple of 16
    uint16              src_height;         // height of source image , multiple of 8
    DCAMERA_RECT_T      input_rect;         // input range, that can't exceed the source image size
    uint32*             input_buffer_ptr;   // buffer that store input image data
    uint16              dest_width;         // width of destination , multiple of 16
    uint16              dest_height;        // height of destination , multiple of 8
    uint32*             output_buffer_ptr;  // point of output buffer
    uint32              output_buffer_len;  // lenght of output buffer
    
}DCAMERA_IMAGE_FORMAT_CONV_PARAM_T;

// Image Review Structure
typedef struct
{
    DCAMERA_DATA_TYPE_E         image_type;         // data type of image
    LCD_ID_E                    lcd_id;             // lcd id define in dal_lcd_sc6800.h
    uint32                      lcd_block_id;       // lcd block id: 0~5
    DCAMERA_RECT_T              disp_rect;          // display size on lcd
    DCAMERA_RECT_T              target_rect;
    uint8*                      image_buffer;       // use to save image data   
    uint8*                      image_buffer_ext1;  // use to save image data for another plane address
    uint8*                      image_buffer_ext2;  // use to save image data for another plane address    
    uint32                      buffer_len;         // lenght of buffer 
    uint16*                     file_name;          // file name in Unicode (relative direction)    
    DCAMERA_RECT_T              image_src_rect;     // rectangle of source image
    DCAMERA_RECT_T              image_disp_rect;    // rectangle of image that will show on lcd 
    DCAMERA_ROTATION_MODE_E     rotation_mode;      // 0: normal; 1: 90; 2: 180; 3:270;
    DCAMERA_DISP_MODE_E         display_mode;   // 0: normal(no distortion); 1:proportion distortion; 2: trimming distortion    
    DCAMERA_CALL_BACK           review_callback;    // call back function for review JPEG, don't care param1 and param2 
}DCAMERA_IMAGE_REVIEW_PARAM_T;


// Image rotation Structure
typedef struct
{
    DCAMERA_IMAGE_ROTATION_TYPE_E   image_type;
    DCAMERA_ROTATION_MODE_E         rotation_mode;
    uint16                          src_width;          // width of source image
    uint16                          src_height;         // height of source image
    uint32*                         input_buffer_ptr;   // buffer that store input image data
    uint32                          input_buffer_len;
    uint32*                         output_buffer_ptr;  // point of output buffer
    uint32                          output_buffer_len;  // lenght of output buffer
    
}DCAMERA_IMAGE_ROTATION_PARAM_T;

// Structure: preview control information
typedef struct
{
    uint8                   brightness_level;
    uint8                   contrast_level;
    uint8                   sharpness_level;
    uint8                   saturation_level;
    uint16                  iso_level;
    uint16                  exposure_comp_level;
    
    uint32                  zoom_level;     
    uint32                  target_size;

    DCAMERA_DISP_MODE_E     display_mode;
    DCAMERA_ROTATION_MODE_E     rotation_mode;
    DCAMERA_PARAM_EFFECT_E      effect_type;
    DCAMERA_PARAM_WB_MODE_E     wb_mode;
    DCAMERA_PARAM_ENVIRONMENT_E enviroment_mode;
    DCAMERA_PARAM_FLICKER_E flicker_mode;    //Timzhu@20080421 add for banding flicker elimination
    DCAMERA_PARAM_VIDEO_MODE_E  video_mode;
    DCAMERA_PARAM_METER_T       meter_param;
    DCAMERA_FLASH_MODE_E        flash_mode;
    BOOLEAN                     is_flashlight_enable;
    uint8                       res[3];

}DCAMERA_PREVIEW_CTL_INFO_T;

// Structure: snapshot control information
typedef struct
{
    uint8                       param; // YUV422 or YUV420 or JPEG(output from sensor directly)
    uint8                       photo_size_max; //the maximum size of Photo DCAMERA can take
    uint8                       need_exif; //whether EXIF information needed
    uint8                       need_thumbnail; //whether thumbnail needed
    uint16                      sensor_out_width;
    uint16                      sensor_out_height;    
}DCAMERA_SNAPSHOT_CTL_INFO_T;

// Structure: auto focus information
typedef struct
{
    uint16  window_max_width;
    uint16  window_max_height;
    uint32  status;

}DCAMAER_AF_INFO_T;
// Structure: all digital camera information
typedef struct
{
    // Sensor   
    SENSOR_MODE_E           preview_m;
    SENSOR_MODE_E           snapshot_m;
    
    // preview
    uint16                  zoom_w_step;
    uint16                  zoom_h_step;
    uint16                  zoom_w_step_snap;
    uint16                  zoom_h_step_snap;

    DCAMERA_RECT_T          disp_rect;
    DCAMERA_RECT_T          target_rect;    
    DCAMERA_DISP_MODE_E     display_mode;
    uint32                  b_preview_from;
    ISP_PREVIEW_PARAM_T     isp_preview_param;
    ISP_VT_ENCODE_PARAM_T   vt_enc_param_t;

    
    DCAMERA_PREVIEW_CTL_INFO_T  preview_ctl_info;
    
    // Snapshot
    DCAMERA_RECT_T              jpeg_rect;  
    uint8*                      jpeg_buf;   
    uint32                      buf_len;    
    
    ISP_FRAME_T                 isp_frame; 
    ISP_FRAME_T                 isp_preview_frame[DC_PREVIEW_FRAME_BUF_MAX]; 
    ISP_FRAME_T                 *p_prev_frm_cur;         
    DCAMERA_SNAPSHOT_CTL_INFO_T snapshot_ctl_info;
    DCAMAER_AF_INFO_T           af_info;

    DCAMERA_IMAGE_FRAME_PARAM_T     target_frame_info_t[DCAMERA_TARGET_FRAME_MAX];      
    ISP_IMAGE_FORMAT                target_frame_format[DCAMERA_TARGET_FRAME_MAX];
    uint32                          target_frame_count;
    void*                           p_preview_buf;
    uint32                          preview_buf_len;
    void*                           snapshot_ex_yuv;
    uint32                          snapshot_ex_yuv_len;
    void*                           snapshot_ex_addi_mem;
    uint32                          snapshot_ex_addi_mem_len;    
    uint32                          is_snapshot_ex;
    uint32                          prev_ex_wait;
    DCAMERA_SIZE_T                  jpg_dec_target_size;    
    DCAMERA_ADDR_T                  cur_addr;
    DCAMEMRA_IMAGE_NOTICE_CALLBACK  image_notice_cb;
    void                            *p_user_data;
    DCAMEMRA_DISPLAY_CALLBACK       display_cb;
    ISP_FRAME_T                     display_src_frame[DC_DISPLAY_FRAME_BUF_MAX]; 
    ISP_FRAME_T                     display_dst_frame[DC_DISPLAY_FRAME_BUF_MAX]; 
    ISP_FRAME_T                     *p_dis_src_frm_cur;  
    ISP_FRAME_T                     *p_dis_dst_frm_cur;  
    ISP_FRAME_T                     *p_dis_src_frm_free;  
    ISP_FRAME_T                     *p_dis_dst_frm_free;  
    
    
    DC_ATV_CALLBACK_T               atv_call_param;
    DCAMERA_ATV_CALLBACK            atv_search_callback;
    DCAMERA_ATV_CALLBACK            atv_stop_callback;
    DCAMERA_ATV_CALLBACK            atv_callback; 
    BOOLEAN                         atv_is_autoscan;

    uint8                           b_is_first_frame;
    uint8                           res[2];
    uint32                          sensor_rot; 
}DCAMERA_INFO_T;

typedef struct
{
    uint32 input_height;
    uint32 input_width;
    uint32 quality_level;
    uint32 frame_type;
    
    uint32 frame_y_addr;
    uint32 frame_u_addr;
    uint32 frame_v_addr;
    
    uint8* output_buf;
    uint32 output_buf_len;
    
    uint32 jpeg_size_return;
    
}DCAMERA_JPEG_ENCODE_INFO_T;

typedef struct
{
    uint32* jpeg_buf;
    uint32 jpeg_len;
    
    uint32 output_width;
    uint32 output_height;
    uint32 output_frame_y_addr;
    uint32 output_frame_u_addr;
    uint32 output_frame_v_addr;
}DCAMERA_JPEG_DECODE_INFO_T;

typedef struct
{
    uint32                          preview_mode;   // 0:defult; 0xffff: don't need to set
    DCAMERA_PARAM_ENVIRONMENT_E     enviroment_mode; 

}DCAMERA_SENSOR_INIT_T;

typedef struct
{
    DCAMERA_ROTATION_MODE_E         rotation_mode;
    DCAMERA_YUV_CHN_ADDR_T          src_chn_addr;
    uint32                          image_width;
    uint32                          image_height;
    DCAMERA_YUV_CHN_ADDR_T          dest_chn_addr;
}DCAMERA_ROTATION_PARAM_T;

typedef enum
{
    YUV_PLANE_2 = 0,
    YUV_PLANE_3,
    YUV_PLANE_MAX
}DC_YUV_PLANE_E;

typedef struct DISPLAY_DATA_FRAME_Tag
{
    uint8*  y_addr;
    uint8*  uv_addr;
    uint8*  uv1_addr;
    uint32  y_data_len;
    uint32  uv_data_len;
    uint32  uv1_data_len;
    uint32  yuv_type;
    DC_YUV_PLANE_E yuv_plane;
}DISPLAY_DATA_FRAME_T;

typedef struct
{
    uint32 video_buf_addr;
    uint32 video_buf_len;
    int    i_lcd;                                      // display on which lcd
    DCAMERA_RECT_T st_disp_rect;         // display rectangular on LCD
    DCAMERA_RECT_T target_rect;           // display rectangular on LCD    
    DCAMERA_RECT_T st_trim_rect;         // trim rectangular
    uint16 src_width;
    uint16 src_height;
    DCAMERA_DISP_MODE_E         disp_mode;
    DCAMERA_ROTATION_MODE_E     rotation;
    DCAMERA_CALL_BACK           review_callback;    // call back function for MJPEG playback, don't care param1 and param2  
}DC_DPLAYER_DISPLAY_PARAM_T;

typedef enum
{
    SENSOR_MODE_BEGIN,

    SENSOR_MODE_END,

    SENSOR_EFFECT_BRIGHTNESS_BEGIN,     

    SENSOR_EFFECT_BRIGHTNESS_0,

    SENSOR_EFFECT_BRIGHTNESS_1,

    SENSOR_EFFECT_BRIGHTNESS_2,

    SENSOR_EFFECT_BRIGHTNESS_3,

    SENSOR_EFFECT_BRIGHTNESS_4,

    SENSOR_EFFECT_BRIGHTNESS_5, 

    SENSOR_EFFECT_BRIGHTNESS_6,     

    //decrease brightness level
    SENSOR_EFFECT_BRIGHTNESS_END,

    SENSOR_EFFECT_CONTRAST_BEGIN,          

    SENSOR_EFFECT_CONTRAST_0,

    SENSOR_EFFECT_CONTRAST_1,

    SENSOR_EFFECT_CONTRAST_2,

    SENSOR_EFFECT_CONTRAST_3,

    SENSOR_EFFECT_CONTRAST_4,

    SENSOR_EFFECT_CONTRAST_5,

    SENSOR_EFFECT_CONTRAST_6,

    //decrease constrast level
    SENSOR_EFFECT_CONTRAST_END,

    SENSOR_EFFECT_EV_LEVEL_BEGIN,

    SENSOR_EFFECT_EV_LEVEL_0,

    SENSOR_EFFECT_EV_LEVEL_1,

    SENSOR_EFFECT_EV_LEVEL_2,

    SENSOR_EFFECT_EV_LEVEL_3,

    SENSOR_EFFECT_EV_LEVEL_4,

    SENSOR_EFFECT_EV_LEVEL_5,

    SENSOR_EFFECT_EV_LEVEL_6,
    
    SENSOR_EFFECT_EV_LEVEL_END,
    
    SENSOR_EFFECT_SEPIA_BEGIN,           

    SENSOR_EFFECT_SEPIA_NORMAL,             

    SENSOR_EFFECT_SEPIA_BLACKWHITE,       

    SENSOR_EFFECT_SEPIA_BLUE,            

    SENSOR_EFFECT_SEPIA_GREEN,                  

    SENSOR_EFFECT_SEPIA_RED,             

    SENSOR_EFFECT_SEPIA_CANVAS,                 

    SENSOR_EFFECT_SEPIA_NEGATIVE,    

    SENSOR_EFFECT_SEPIA_END,

    SENSOR_EFFECT_VFLIP_ENABLE,

    SENSOR_EFFECT_VFLIP_DISABLE,

    SENSOR_SEQUENCE_END

}DC_SENSOR_MODE_E;
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_AdjustDisplayRect(uint32      lcd_id,
                                                        ISP_RECT_T* p_src_rect, 
                                                        ISP_RECT_T* p_inva_rect, //the rect to be invalidte
                                                        ISP_RECT_T* p_img_rect,  //the rect to put image
                                                        uint32      display_mode,
                                                        uint32      rot_angle);

#ifdef __cplusplus
    }
#endif

#endif  /*#ifndef DCAMERA_CONTEXT_H*/

