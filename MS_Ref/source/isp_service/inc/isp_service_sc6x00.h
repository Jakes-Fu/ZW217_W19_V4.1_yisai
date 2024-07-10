
/******************************************************************************
 ** File Name:    sc6600l_isp_service.h                                       *
 ** Author:       Jianping.Wang                                               *
 ** DATE:         06/18/2006                                                  *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 06/18/2006    Jianping.Wang   Create.                                     *
 ******************************************************************************/
#ifndef _SC6600L_ISP_SERVICE_H_
#define _SC6600L_ISP_SERVICE_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
#include "ivsp_drv.h"
#include "isp_drv.h"
#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

#define ISP_PREVIEW_BUF_NUM		3
#define ISP_PREVIEW_BUF_0		0
#define ISP_PREVIEW_BUF_1		1
#define ISP_PREVIEW_BUF_2		2

#define ISP_ROTATION_BUF_NUM	2
#define ISP_ROTATION_BUF_0		0
#define ISP_ROTATION_BUF_1		1

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/

//enum
typedef enum
{
    ISP_SERVICE_IDLE,
    ISP_SERVICE_PREVIEW,
    ISP_SERVICE_CAPTURE,
    ISP_SERVICE_REVIEW,
    ISP_SERVICE_MOTION_JPEG,
    ISP_SERVICE_SCALE,
    ISP_SERVICE_SLICE_SCALE,    
    ISP_SERVICE_SET_SENSOR_PARAM, 
    ISP_SERVICE_SET_SENSOR_CALLBACK,     
    ISP_SERVICE_DC_ZOOM,
    ISP_SERVICE_DC_ZOOM_CALLBACK,   
    ISP_SERVICE_UPCC,       
    ISP_SERVICE_ATV_PREVIEW,   
    ISP_SERVICE_ATV_RECORDER,
    ISP_SERVICE_SENSOR_EOF,
    ISP_SERVICE_ID_MAX
}ISP_SERVICE_ID_E; 

typedef enum 
{
    ISP_SERVICE_OPEN_SIG 	= 1,
    ISP_SERVICE_CLOSE_SIG,	
    ISP_SERVICE_START_SIG,
    ISP_SERVICE_SET_SENSOR_PARAM_SIG,
    ISP_SERVICE_STOP_SIG,	
    ISP_SERVICE_TRANSFER_DONE_SIG,
    ISP_SERVICE_DISP_SIG,
    ISP_SERVICE_OVERFLOW_SIG,
    ISP_SERVICE_START_TIMEOUT_SIG, 
    ISP_SERVICE_STOP_TIMEOUT_SIG,     
    ISP_SERVICE_VIDEO_RECORDING_TIMEOUT_SIG,         
    ISP_SERVICE_SENSOR_EOF_SIG,         
    ISP_SERVICE_SIG_MAX
} ISP_SERVICE_SIG_E;

typedef enum
{
    ISP_SERVICE_CLOSE_STATE = 0,	
    ISP_SERVICE_IDLE_STATE,
    ISP_SERVICE_PREVIEW_STATE,
    ISP_SERVICE_CAPTURE_STATE,    
    ISP_SERVICE_REVIEW_STATE,	
    ISP_SERVICE_SCALE_STATE,
    ISP_SERVICE_MJPG_STATE,
    ISP_SERVICE_UPCC_STATE,
    ISP_SERVICE_STATE_MAX
}ISP_SERVICE_STATE_E;

typedef enum
{
    ISP_SERVICE_ZOOM_100X = 0,		
    ISP_SERVICE_ZOOM_125X,			
    ISP_SERVICE_ZOOM_150X,
    ISP_SERVICE_ZOOM_175X,
    ISP_SERVICE_ZOOM_200X,
    ISP_SERVICE_ZOOM_MAX
}ISP_SERVICE_ZOOM_LEVEL_E;

typedef enum
{
    ISP_SERVICE_ZOOM_IN = 0,			
    ISP_SERVICE_ZOOM_OUT,
    ISP_SERVICE_ZOOM_MODE_MAX
}ISP_SERVICE_ZOOM_MODE_E;

typedef enum
{
    ISP_SERVICE_SCALE_NORMAL = 0,			
    ISP_SERVICE_SCALE_SLICE,
    ISP_SERVICE_SCALE_MODE_MAX
}ISP_SERVICE_SCALE_MODE_E;

typedef enum
{
    ISP_SERVICE_ENCODE_CALLBACK=0,
    ISP_SERVICE_GET_DATA_CALLBACK,     
    ISP_SERVICE_MJPG_CALLBACK_NOTICE,
    ISP_SERVICE_DC_CALLBACK,
    ISP_SERVICE_STOP_CALLBACK,
    ISP_SERVICE_ZOOM_CALLBACK,
    ISP_SERVICE_CALLBACK_MAX
}ISP_SERVICE_CALLBACK_ID_E;

typedef enum
{
    ISP_SERVICE_DISPLAY_END=0,
    ISP_SERVICE_DISPLAY_STATE,	
    ISP_SERVICE_DISPLAY_MAX
}ISP_SERVICE_DISPLAY_ID_E;


typedef enum
{
     ISP_SERVICE_INVALIDATE_ASYNC = 0,
     ISP_SERVICE_INVALIDATE_SYNC   

}ISP_SERVICE_INVALIDATE_MODE;

typedef enum
{
	ISP_SERVICE_OWNER_NONE = 0,
	ISP_SERVICE_OWNER_DC,
	ISP_SERVICE_OWNER_JPEG,
	ISP_SERVICE_OWNER_SCALER,
	ISP_SERVICE_OWNER_UNKNOWN
}ISP_SERVICE_OWNER_E;

//structure
typedef int (*zoom_callback)(uint32 is_success);
typedef int (*set_data)( ISP_DATA_ADDR_T* addr,uint32* width,uint32* height);
typedef int (*get_data)( ISP_DATA_ADDR_T addr,uint32 width,uint32 height);
typedef int (*set_sensor_param)(uint32 cmd, uint32 arg);
typedef int (*review_success_callback)(void* );
typedef int (*mjpeg_record_notice_func)(void *);
typedef int (*dc_callback)(void*);
typedef int (*stop_callback)(void*);
typedef int (*success_callback)(uint32 is_success);

 typedef struct
{
    uint32  cmd;
    uint32  arg;
    uint32  syn_ctrl;
    set_sensor_param    set_param;
    success_callback    set_param_success;
} ISP_SENSOR_PARAM_T, *ISP_SENSOR_PARAM_T_PTR;

 typedef struct _isp_sensor_signal_info_tag
{
    ISP_DATA_FORMAT_E img_format;
    uint32 img_pattern;
    uint32 vsync_polarity;
    uint32 hsync_polarity;
    uint32 preview_skip_num;
    uint32 capture_skip_num;
    uint32 preview_deci_num;
    uint32 video_preview_deci_num;
    uint16 threshold_eb;
    uint16 threshold_mode;
    uint16 threshold_start;
    uint16 threshold_end;
} ISP_SENSOR_SIGNAL_INFO_T,*ISP_SENSOR_SIGNAL_INFO_T_PTR;

typedef struct _isp_mjpeg_info_tag
{
    ISP_SIZE_T  capture_size;    
    ISP_SIZE_T  review_size;
    ISP_SIZE_T  jpeg_target_size;

    //capture scale coeff
    ISP_SIZE_T  capture_scale_input_size;    
    ISP_RECT_T  capture_scale_trim_rect;
    uint32      capture_scale_trim_eb;        
    uint32      capture_scale_bypass;
    uint32      *capture_h_scale_coeff_ptr;
    uint32      *capture_v_scale_coeff_ptr;

    //display scale coeff, 
    ISP_SIZE_T  review_scale_input_size;    
    ISP_RECT_T  review_scale_trim_rect;
    uint32      review_scale_trim_eb;        
    uint32      review_scale_bypass;
    uint32      *review_h_scale_coeff_ptr;
    uint32      *review_v_scale_coeff_ptr;	

    //jpeg scale coeff
    ISP_SIZE_T  jpeg_scale_input_size;    
    ISP_RECT_T  jpeg_scale_trim_rect;	
    uint32      jpeg_scale_trim_eb;    
    uint32      jpeg_scale_bypass;
    uint32      *jpeg_h_scale_coeff_ptr;
    uint32      *jpeg_v_scale_coeff_ptr;   

    ISP_DATA_ADDR_T capture_yuv_addr;
    uint32 capture_yuv_size;
    ISP_DATA_ADDR_T review_rgb_addr;
    uint32 review_rgb_size;	
    ISP_DATA_ADDR_T jpeg_yuv_addr;
    uint32 jpge_yuv_size;
}ISP_MJPEG_INFO_T;


typedef struct
{
    ISP_SIZE_T      input_size;
    ISP_RECT_T      input_rect;
    uint32          init_zoom_level;    
    uint32          disp_lcd_id;
    uint32          disp_block_id;  // reserve
    uint32          pclk;
    ISP_RECT_T      disp_rect;
    ISP_RECT_T      target_rect;
    ISP_ROTATION_E  disp_rotation;// 0: 0; 1: 90; 2: 180; 3: 270
    uint32          disp_addr; //cap data pointer	
    uint32          disp_buf_size;
    uint32          rot_buf_addr; //rotation buf pointer	
    uint32          rot_buf_size;
    dc_callback         suc_callback;
} ISP_PREVIEW_PARAM_T;

typedef struct
{	
    ISP_SIZE_T          input_size;
    ISP_RECT_T          input_rect;
    ISP_SIZE_T          output_size;	
    ISP_RECT_T          zoom_rect;  // reserved 
    uint32              skip_num; //sensor output jpeg    
    uint32              pclk;
    uint32              capture_addr; //sensor output jpeg
    uint32              capture_buf_size;//sensor output jpeg
    ISP_DATA_FORMAT_E   sensor_data_format;  
    ISP_DATA_FORMAT_E   output_data_format;      
    get_data            get_cap_data;       
} ISP_JPEG_PARAM_T;

typedef struct
{
    ISP_DATA_ADDR_T addr;
    ISP_SIZE_T      size;
} ISP_CAPTRUE_DATA_T;


typedef struct
{
    ISP_SIZE_T          input_size;
    ISP_RECT_T          input_rect;
    ISP_DATA_FORMAT_E   input_format; //422/420/400
    ISP_DATA_ADDR_T     src_addr;	//input address
    ISP_DATA_LEN_T      src_len;
    ISP_DATA_FORMAT_E   disp_format;    
    uint32              disp_addr;
    uint32              disp_buf_size;
    uint32              disp_lcd_id;
    uint32              disp_block_id;
    ISP_RECT_T          disp_rect;
    ISP_RECT_T          target_rect;
    ISP_DATA_ADDR_T     rot_addr;   
    ISP_DATA_LEN_T      rot_buf_len;
    ISP_ROTATION_E      disp_rotation; // 0: 0; 1: 90; 2: 180; 3: 270 
    uint32              slice_line;
    BOOLEAN             is_slice_scale;
    BOOLEAN             is_copy_display_data;
    BOOLEAN             reserve1;
    BOOLEAN             reserve2;
    ISP_SERVICE_INVALIDATE_MODE mode;    
    dc_callback         suc_callback;
    
}ISP_REVIEW_PARAM_T;

typedef struct
{
    ISP_SIZE_T                  input_size;
    ISP_RECT_T                  input_rect;
    ISP_DATA_FORMAT_E           input_format; //422/420/400 (rgb565/888 only 6800h)    
    ISP_DATA_ADDR_T             input_addr;
    uint32                      input_buf_size;
    ISP_DATA_FORMAT_E           output_format; //422/(RGB only 6800h)        
    ISP_SIZE_T                  output_size;
    ISP_DATA_ADDR_T             output_addr;
    uint32                      output_buf_size;  
    uint32                      swap_buf_addr;
    uint32                      swap_buf_size;    
    uint32                      line_buf_addr; 
    uint32                      line_buf_size;
    uint32                      slice_line;
    ISP_SERVICE_SCALE_MODE_E    scale_mode;    
    BOOLEAN                     is_first;    
    BOOLEAN                     is_last;
    BOOLEAN                     reserve1;
    BOOLEAN                     reserve2;
    get_data                    get_scale_data;   

	int32 						(*start_scale_callback)(int32);
	int32						start_scale_param;
} ISP_SCALE_PARAM_T;


typedef struct
{
    ISP_SIZE_T      input_size;
    ISP_RECT_T      input_rect;
    ISP_SIZE_T      capture_size;  // capture yuv size

    ISP_RECT_T      jpeg_target_rect;   // mjpeg encoder src rect/src is capture_size
    ISP_SIZE_T      jpeg_target_size;   // mjpeg encoder size

    uint32          capture_addr; //capture yuv data buf pointer	
    uint32          capture_buf_size;

    uint32          scale_addr; //scale data pointer	
    uint32          scale_buf_size;  

    uint32          disp_addr; //disp data pointer	    
    uint32          disp_buf_size;    

    ISP_DATA_ADDR_T     rot_addr;   
    ISP_DATA_LEN_T      rot_buf_len;

    uint32          lcd_id;
    uint32          lcd_block_id;
    ISP_RECT_T      disp_rect;
    ISP_RECT_T      target_src_rect;    //disp src rect/src is capture_size
    ISP_RECT_T      target_rect;
    ISP_ROTATION_E  disp_rotation;

    BOOLEAN         is_pause;
    BOOLEAN         is_disp;
    BOOLEAN         is_copy_display_data;
    BOOLEAN         reserve0;
    uint32          pclk;
    ISP_SERVICE_INVALIDATE_MODE mode;
    get_data      get_cap_data;
    
} ISP_MJPEG_PARAM_T;

typedef struct
{
    uint32 		  zoom_level;
    uint32 		  zoom_mode;     
    uint32 		  zoom_step;  
    success_callback    zoom_success;
} ISP_ZOOM_PARAM_T;

typedef struct
{
    uint32 num;
    uint32 cur_num;
    success_callback callback;
} ISP_SENSOR_FRAME_COUNTER_T,*ISP_SENSOR_FRAME_COUNTER_T_PTR;

typedef struct
{
    uint16 disp_w;
    uint16 disp_h;
    uint16 target_start_x;
    uint16 target_start_y;
    uint16 target_end_x;
    uint16 target_end_y;
    ISP_DATA_ADDR_T disp_buf;
}ISP_MPEG_DISPLAY_DATA_T, *ISP_MPEG_DISPLAY_DATA_T_PTR;

typedef uint32 (* ISP_MPG_DISPLAY_CALLBACK)(uint16 ret_value, void* param_ptr, uint32 param_size);

typedef struct _isp_service_tag
{
    uint32                      service;
    ISP_SERVICE_STATE_E         state;  
    //data format
    ISP_DATA_FORMAT_E           input_image_format;
    uint32                      input_image_pattern;
    ISP_DATA_FORMAT_E           output_image_format;
    ISP_DATA_FORMAT_E           frame_data_format;

    //the polarity of sync signal
    uint32                      vsync_polarity;
    uint32                      hsync_polarity;
    uint32                      pclk_polarity;
    uint32                      isp_feature_enable;
    uint16                      threshold_eb;
    uint16                      threshold_mode;
    uint16                      threshold_start;
    uint16                      threshold_end;
    //control flag
    uint32                      cap_deci;
    uint32                      skip_counter;
    uint32                      display_state;    
    uint32                      skip_frame_num;
    uint32                      preview_skip_frame_num;
    uint32                      capture_skip_frame_num;   
    uint32                      preview_deci_frame_num; 
    uint32                      video_preview_deci_frame_num;
    uint32                      deci_frame_counter;
    uint32                      bad_frame_counter;
    uint32                      time_out_counter;
    IVSP_CLK_E                  dcam_clk;
    //buffer info
    uint32                      isp_buf_addr;
    uint32                      isp_buf_size;
    uint32                      rot_buf_addr;
    uint32                      rot_buf_size;
    uint32                      init_zoom_level;    
    ISP_DATA_ADDR_T             preview_buf[ISP_PREVIEW_BUF_NUM];
    ISP_DATA_ADDR_T             rotation_buf[ISP_ROTATION_BUF_NUM];  
    uint32                      preview_buf_id;
    uint32                      preview_loop_buf_num;
    uint32                      rotation_loop_buf_num;
    ISP_DATA_ADDR_T             preview_isp_out_buf_addr;     
    ISP_DATA_ADDR_T             preview_display_buf_addr;
    ISP_DATA_ADDR_T             display_buf_addr;
    uint32                      rotation_buf_id;
    //display      
    uint32                      lcd_id;
    ISP_DATA_FORMAT_E           disp_format;
    ISP_ROTATION_E              rotation; 
    ISP_RECT_T                  disp_rect; // for lcd display rect
    ISP_RECT_T                  target_rect;
    ISP_RECT_T                  full_screen_rect; // for full screen display   
    ISP_REVIEW_PARAM_T          src_review_param;
    ISP_DATA_ADDR_T             review_display_buf_addr;    
    ISP_SIZE_T                  input_size;       
    ISP_RECT_T                  cap_trim_rect;   // cap trim rect
    ISP_SIZE_T                  cap_output_size; // cap output size and scale input size
    ISP_RECT_T                  scale_trim_rect; //scale trim rect for scale function
    ISP_SIZE_T                  output_size;   // dcam output size
    ISP_RECT_T                  preview_input_rect;   // recoder preview rect for zomm capture calc the capture rect

    //input &output addr
    ISP_DATA_LEN_T              update_cache;
    ISP_DATA_ADDR_T             input_addr;
    ISP_DATA_ADDR_T             output_addr;   
    uint32                      input_buf_size;
    ISP_DATA_ADDR_T             swap_addr;    
    uint32                      line_buf_addr; 
    ISP_SUB_SAMPLE_MODE_E       subsample_mode;    
    BOOLEAN                     subsample_eb;
    BOOLEAN                     is_rgb_mode;
    BOOLEAN                     is_rgb565;
    BOOLEAN                     is_bypass_scale;
    ISP_CAPTURE_JPEG_MEM_SIZE_E out_buf_size;
    uint32                      jpg_buf_limit;    
    uint32                      cap_jpg_size;    
    uint32                      scale_trim_eb;     
    ISP_SCALE_MODE_E            scale_mode;    
    uint32                      slice_line;       
    uint32                      *h_scale_coeff_ptr;   
    uint32                      *v_scale_coeff_ptr;   
    ISP_SUB_SAMPLE_MODE_E       sub_sample_mode;
    uint32                      slice_complete_line; 
    //mjpeg
    ISP_MJPEG_INFO_T            mjpg_info;
    BOOLEAN                     is_stop;   
    BOOLEAN                     is_pause;
    BOOLEAN                     is_full_view;
    BOOLEAN                     yuv2rgb_eb;
    //digital zoom
    ISP_ZOOM_PARAM_T            zoom_ctl;
    ISP_RECT_T                  zoom_rect; //scale trim rect for scale function
    uint16*                     zoom_array_ptr;
    BOOLEAN                     is_zoom;
    BOOLEAN                     is_send_zoom_sig;    
    BOOLEAN                     is_success;        
    BOOLEAN                     is_eof_done;    
    BOOLEAN                     is_video_mode;      
    BOOLEAN                     is_timeout; 
    BOOLEAN                     is_dir_display;    
    BOOLEAN                     is_tx_done; 
    BOOLEAN                     is_first;
    BOOLEAN                     is_last;
    BOOLEAN                     is_sub_sample;
    BOOLEAN                     is_first_frame;
    BOOLEAN                     is_disp;
    BOOLEAN                     is_get_frame_data;
    BOOLEAN                     is_copy_display_data;  //atv/preview/video recording/video player need save display buffer
    BOOLEAN                     reserve1;

	uint32 						*scale_vcoeff_tmp_ptr;	//horizontal coefficient buffer
	uint32 						*scale_hcoeff_tmp_ptr;	//vertical coefficient buffer

	uint32 						*scale_tmp_ptr;			//temp buffer used for coefficient calculation
	uint32 						scale_tmp_size;	

    //sensor param ptr
    ISP_SENSOR_PARAM_T          isp_sensor_ctl;
    ISP_SENSOR_FRAME_COUNTER_T  isp_sensor_frame_counter;
    ISP_SERVICE_INVALIDATE_MODE invalidate_mode;

    int (*set_src_data)( ISP_DATA_ADDR_T addr,uint32 width,uint32 height);   //get data
    int (*get_data)( ISP_DATA_ADDR_T addr,uint32 width,uint32 height);   //get data
    int (*get_frame_data)(uint32 y_addr,uint32 uv_addr,uint16 width,uint16 height);
    int (*dc_callback)(void*);
    int (*stop_callback)(void*);
    int (*mjpeg_record_notice_func)(void *); //notice dv    

	int32 (*start_scale_callback)(int32);	//be called after the isp be started
	int32 start_scale_param;
}ISP_SERVICE_T, *ISP_SERVICE_T_PTR;

/****************************************************************************************/
// Description: ISP Service Stop
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceFeatureStop(void);

/****************************************************************************************/
// Description: ISP align
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_Align(uint32 pLen, uint32 align_coeff, uint32 align_len);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void  ISP_ServiceGetMutex(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void  ISP_ServicePutMutex(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_ServiceInit(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_ServiceOpen(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_ServiceClose (void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int32 ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ID_E id, int (*func)());

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartPreview(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetAtvEnable(BOOLEAN is_eb);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_ServiceGetAtvIsEnable(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStopPreview(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetCaptureParam(ISP_JPEG_PARAM_T *param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartSyncCapture(ISP_DATA_ADDR_T* capture_addr_ptr, uint32* width_ptr,uint32* height_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartSyncCaptureExt(ISP_DATA_ADDR_T* capture_addr_ptr, uint16* width_ptr,uint16* height_ptr, uint32 timer_out);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartAsyncCapture(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 SetTVReviewParam(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ATVErrHandler(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_ServiceStopCapture(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetCaptureData(ISP_CAPTRUE_DATA_T *data_info_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T *param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartReview(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetScaleParam(ISP_SCALE_PARAM_T *param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_CalcZoomTab(uint16 input_start_x, 
                                    uint16 input_start_y, 
                                    uint16 input_width, 
                                    uint16 input_height, 
                                    uint16 output_width, 
                                    uint16 output_height);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 SetATVPreviewParam(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceStartScale(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetSliceScaleParam(ISP_SCALE_PARAM_T *param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartSliceScale(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetUpccParam(ISP_MJPEG_PARAM_T *param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceStartUpcc(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetMjpegParam(ISP_MJPEG_PARAM_T *param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceStartMjpeg(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServicePauseMjpeg(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceResumeMjpeg(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceStopMjpeg(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceGetSensorFrameCounter(ISP_SENSOR_FRAME_COUNTER_T_PTR param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC ISP_SERVICE_STATE_E ISP_ServiceGetState(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetInputWidthAlign(uint32 input_width, uint32 input_height, 
												  uint32 output_width, uint32 output_height);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetInputHeightAlign(uint32 input_height, uint32 output_height);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetOutputWidthAlign(ISP_DATA_FORMAT_E data_format);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetOutputHeightAlign(ISP_DATA_FORMAT_E data_format);


/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceZoom(uint32 zoom_level, int (*func)());

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetSensorParam(ISP_SENSOR_PARAM_T* param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetSensorSignalInfo(ISP_SENSOR_SIGNAL_INFO_T_PTR param_ptr);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC ISP_SERVICE_T* ISP_ServiceGetISPInfo(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC ISP_RECT_T_PTR ISP_ServiceGetZoomTabPtr(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceGetFrameData(int (*func)(), ISP_DATA_FORMAT_E data_format);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetVideFlag(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceCleanVideFlag(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceGetReviewDisplayBuf(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetMaxZoomLevel(uint32 max_zoom_level);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetMemType(uint32 mem_type);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxZoomLevel(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxCaptureWidth(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxCaptureHeight(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetDisplayFormat(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxDisplayWidth(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxDisplayHeight(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxScaleWidth(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxScaleHeight(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxSliceScaleWidth(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxSliceScaleHeight(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetSliceScaleLineWidth(uint16 src_width);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxScaleUpCoeff(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxScaleDownCoeff(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxPreviewScaleUpCoeff(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxScaleLevel(void);

/****************************************************************************************/
// Description: 
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetScaleMatchHeight(uint16 input_height, uint16 output_height);

/****************************************************************************************/
// Description: Get align factor of slice scaling height
// Global resource dependence: 
// Author: 
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: the shift value of align factor   
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetScaleSliceHeightAlign(uint32 input_width, uint32 output_width,
													uint32 input_height, uint32 output_height);

/****************************************************************************************/
// Description: Check if the scaling is supported
// Global resource dependence: 
// Author:
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          TRUE: 		supported
//			FALSE:		unsupported
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_ServiceIsScaleSupported(ISP_SCALE_PARAM_T * param_ptr);


/*****************************************************************************/
//  Description: Lock ISP Service
//	Global resource dependence:
// Author: Tim.zhu
// input parameter:
//                      owner:		the locker
//						wait_option:  SCI_WAIT_FOREVER or max milliseconds to wait
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: lock successful
//			other:		  lock timeout
// Note:
/*****************************************************************************/
PUBLIC uint32 ISP_ServiceLock(ISP_SERVICE_OWNER_E owner,  uint32 wait_option);

/*****************************************************************************/
//  Description: Unlock ISP Service
//	Global resource dependence:
// Author: Tim.zhu
// input parameter:
//                      owner:		the locker
// output parameter:
//                      None
// return:				None
// Note: Only the owner can unlock the ISP Service
/*****************************************************************************/
PUBLIC void ISP_ServiceUnlock(ISP_SERVICE_OWNER_E owner);

/*****************************************************************************/
//  Description: Is ISP idle
//	Global resource dependence:
// Author: Tim.zhu
// input parameter:		
//						None
// output parameter:
//                      None
// return:				TRUE: idle. 
//						FALSE: busy
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISP_ServiceIsIdle(void);

/****************************************************************************************/
// Description: is support the update cache
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          TRUE: 		supported
//			FALSE:		unsupported
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_ServiceIsSupportedUpdateCache(void);


PUBLIC uint32 ISP_Service_GetDirectMaxScaleWidth(void);

PUBLIC void ISP_Regist_Mpeg_Display_Callback(ISP_MPG_DISPLAY_CALLBACK callback_func);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/


#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
