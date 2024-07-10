
#ifndef _DC_APP_6600L_H_
#define _DC_APP_6600L_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "sci_types.h"
#include "sensor_drv.h"
#include "isp_service.h"
#include "dal_dcamera.h"
#include "dal_recorder.h"
#include "dc_common_6600l.h"
#include "img_cascade.h"
#include "priority_system.h"
#define _DC_DEBUG

#ifdef _DC_DEBUG
#define DCAM_PRINT   SCI_TRACE_LOW
#define DC_LOG_TRACE(_x_)   SCI_TraceLow _x_
#else
#define DCAM_PRINT(x)
#define DC_LOG_TRACE(_x_)   ((void)0)
#endif
#define DC_ERR_TRACE(_x_)   SCI_TraceLow _x_

#define FILENAME_STR    __MODULE__

#define max(a,b) (((a) > (b)) ? (a) : (b))

#define max3(a,b,c) (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c)))


#define DCAM_BST_INTERVAL_TIME_MIN		(100)
#define DCAM_BST_INTERVAL_TIME_MAX		(1500)

#define SENSOR_SIZE_NUM                  10
#define FRAME_MODE_SIZE_MAX             (640*480*2)
#define SLICE_WIDTH_1300K				1280
#define SLICE_HEIGHT_1300K				16
#define SLICE_Y_WIDTH_300K			640
#define SLICE_UV_WIDTH_300K			320
#define SLICE_HEIGHT_300K				8

#define THUMB_WIDTH 320
#define THUMB_HEIGHT 240


#define JPEG_FW_BUFFER_SIZE					(30*1024)		//FirmWare
#define JPEG_BS_BUFFER_SIZE					(100*1024)		//BitStream
#define JPEG_CX_BUFFER_SIZE					(20*1024)		//BitStream
#define JPEG_INTERPOLATION_BUFFER_SIZE		(80*1024)		//work buffer
#define JPEG_FRAMES_BUFFER_SIZE				(300*1024)

#define DC_LEFT_TRUNK16(x)          ((x)&0xfffffff0)
#define DC_RIGHT_TRUNK16(x)         (((x)+15)&0xfffffff0)

#define DC_LEFT_TRUNK8(x)           ((x)&0xfffffff8)
#define DC_RIGHT_TRUNK8(x)          (((x)+7)&0xfffffff8)

#define DC_LEFT_TRUNK4(x)           ((x)&0xfffffffc)
#define DC_RIGHT_TRUNK4(x)          (((x)+3)&0xfffffffc)

#define DC_LEFT_TRUNK2(x)           ((x)&0xfffffffe)
#define DC_RIGHT_TRUNK2(x)          (((x)+1)&0xfffffffe)

#define DCAM_TIMEOUT_CHECK			(1000) //ms

#define DCAM_BURST_INTERVAL_MIN	(50) //ms

#define DCAM_TASK_PRIORITY			PRI_DCAM_TASK  //28

#define MJPEG_QVGA_SIZE_MAX			(20*1024)

#define DCAM_STACK_SIZE     		8000
#define DCAM_QUEUE_NUM        		10

#define DC_RETURN_VAL_IF_FAIL(exp, ret) do{if(exp) return ret;}while(0)
#define DC_RETURN_VAL_IF_FALSE(exp, ret) do{if(SCI_FALSE == (exp)) return ret;}while(0)
#define DC_RETURN_VAL_IF_FAIL_WARNING(exp, ret, warning) do{if(exp) {DC_LOG_TRACE(warning); return ret;}}while(0)
#define DC_RETURN_VAL_IF_FALSE_WARNING(exp, ret, warning) do{if(!(exp)) {DC_LOG_TRACE(warning); return ret;}}while(0)
#define DC_RETURN_IF_FAIL(exp, warning) do{if(exp) {DC_LOG_TRACE(warning); return exp;}}while(0)

#define DECLARE_GLOBAL_CONTEXT    extern DCAM_CONTEXT_T g_dcam_context

#define DCAM_STATE (g_dcam_context.state)
#define SET_DCAM_STATE(state) do{DCAM_STATE = state;}while(0)
#define DC_IN_STATE(state)    (state == DCAM_STATE)

#define PREVIEW_MODE        (g_dcam_context.preview_mode)
#define SNAPSHOT_MODE       (g_dcam_context.snapshot_mode)

#define SNAPSHOT_PARAM_ST   (g_dcam_context.snapshot_param)
#define PREVIEW_PARAM_ST    (g_dcam_context.preview_param)
#define REVIEW_PARAM_ST     (g_dcam_context.review_param)
#define BURSTSNAPSHOT_PARAM_ST (g_dcam_context.burst_snapshot_param)
#define BURSTSNAPSHOT_RETURN_PARAM_ST (g_dcam_context.burst_snapshot_return_param)
#define UPCC_REC_PARAM_ST  (g_dcam_context.upcc_param)

#define PREVIEW_DISP_RECT   (PREVIEW_PARAM_ST.disp_rect)

#define DC_TIMER_PTR        (g_dcam_context.ovfw_timer_ptr)

#define ZOOM_LEVEL          (g_dcam_context.zoom_level)
#define ENVIROMENT_MODE     (g_dcam_context.e_enviroment_mode)
#define PREVIEW_BRIGHTNESS  (g_dcam_context.brightness)
#define PREVIEW_CONTRAST    (g_dcam_context.contrast)
#define PREVIEW_SHARPNESS   (g_dcam_context.sharpness)
#define PREVIEW_SATURATION  (g_dcam_context.saturation)
#define PREVIEW_EFFECT      (g_dcam_context.effect)
#define PREVIEW_WB_MODE     (g_dcam_context.wb)
#define EV_MODE             (g_dcam_context.ev_mode)
#define FLICKER_MODE        (g_dcam_context.banding_flicker_mode)
#define VIDEO_MODE          (g_dcam_context.video_mode)

#define SNAPSHOT_TARGET_WIDTH       (g_dcam_context.target_width)
#define SNAPSHOT_TARGET_HEIGHT      (g_dcam_context.target_height)

#define SNAPSHOT_TRIM_WIDTH         (g_dcam_context.trim_rect.w)
#define SNAPSHOT_TRIM_HEIGHT        (g_dcam_context.trim_rect.h)


#define CURRENT_TIME    SCI_GetTickCount()

typedef enum
{
    DC_ATV_CMD_SCAN_CHN_START=0x01,
    DC_ATV_CMD_SCAN_CHN,
    DC_ATV_CMD_SCAN_CHN_STOP,
    DC_ATV_CMD_RSSI,
    DC_ATV_CMD_DECT_REGION,
    DC_ATV_CMD_MAX,
}DC_ATV_CMD_E;

typedef enum
{
    DCAM_MSG_PREVIEW = 0,
    DCAM_MSG_SNAPSHOT,
    DCAM_MSG_DECODE,
    DCAM_MSG_DISPLAY,
    DCAM_MSG_DISPLAY_DONE,
    DCAM_MSG_BURST_SNAPSHOT,
    DCAM_MSG_MJPEG_CAPTURE,
    DCAM_MSG_OVERTIME,
    DCAM_MSG_ZOOM_DONE,
    DCAM_MSG_ATV,
    DC_PARAM_CTRL_CALLBACK_PROC,
    DCAM_MSG_MAX
}DCAM_MSG_E;

typedef enum
{
    DC_STATE_IDLE,
    DC_STATE_PREVIEWING,
    DC_STATE_CAPTURING,
    DC_STATE_REVIEWING,
    DC_STATE_PANORAMA,
    DC_STATE_FOURINONE,	
    DC_STATE_REC,
    DC_STATE_VIDEO_REVIEW,
    DC_STATE_MJPEG_REC_PAUSE,
    DC_STATE_BURST_CAPTURING,
    DC_STATE_UPCC_REC,
    DC_STATE_BYPASSING,
    DC_STATE_ATV_SCAN_CHN,
    DC_STATE_UNKNOWN,
    DC_STATE_MAX 
}DC_STATE_E;

typedef enum
{
	DCAM_DATA_TYPE_RGB565 = 0,
	DCAM_DATA_TYPE_RGB666,
	DCAM_DATA_TYPE_RGB888,
	DCAM_DATA_TYPE_YUV444,
	DCAM_DATA_TYPE_YUV422,
	DCAM_DATA_TYPE_YUV420,
	DCAM_DATA_TYPE_YUV411,
	DCAM_DATA_TYPE_YUV400,					//ºÚ°×Í¼Ïñ¸ñÊ½
	DCAM_DATA_TYPE_MAX
}DCAM_DATA_TYPE_E;  

typedef enum
{
	DCAM_RECORD_YUV,
	DCAM_RECORD_MJPEG

}DCAM_RECORD_TYPE;

typedef struct
{
    xSignalHeaderRec    sig;
    uint32        		param;
}DCAM_SIGNAL_T;

typedef struct MM_FEATURE_SUPPORT_Tag
{
    uint32  feature_support_bit;//bit 0\4\8\12 preview\capture\burst\dv. 1:enable 0:disable
    uint32  ex_mem_size;		//size of the mem
    uint8*  mem_ptr;
    uint8   max_burst_shot_num[SENSOR_SIZE_NUM];
    uint8   max_sup_capture_size;
}MM_FEATURE_SUPPORT_T;

typedef struct DCAM_CONTEXT_Tag
{
    DC_STATE_E			state;
    int					status;
    BOOLEAN				is_open;
    BOOLEAN				is_smooth_zoom;
    BOOLEAN				is_zoom_busy;
    BOOLEAN				is_interpolation_needed;
    BOOLEAN				is_sensor_init;
    BOOLEAN				is_sensor_open;
    BOOLEAN				is_burst_snapshot;
    BOOLEAN				is_first;
    BOOLEAN				exif_eb;
    BOOLEAN				thumbnail_eb;
    BOOLEAN				ext_open;
    BOOLEAN				flash_eb;
    //sensor
    SENSOR_MODE_E		preview_mode;
    SENSOR_MODE_E		snapshot_mode;
    SENSOR_IMAGE_FORMAT	sensor_format;
    SENSOR_EXP_INFO_T*	sensor_info_ptr;
    uint16					brightness;
    uint16					contrast;
    uint16					sharpness;
    uint16					saturation;
    uint16					zoom_level;
    uint16					effect;
    uint16					wb;
    uint16					exposure;
    DCAMERA_PARAM_ENVIRONMENT_E e_enviroment_mode;
    DCAMERA_PARAM_FLICKER_E banding_flicker_mode;
    DCAMERA_PARAM_EV_E      ev_mode;
    DCAMERA_PARAM_VIDEO_MODE_E video_mode;

    uint16					zoom_step_num;
    uint16					zoom_display_num;
    DCAMERA_RECT_T 		target_rect;
    DCAMERA_RECT_T 		disp_rect;
    DCAMERA_RECT_T		trim_rect;

    DCAMERA_PREVIEW_PARAM_T		preview_param;
    DCAMERA_SNAPSHOT_PARAM_T	snapshot_param;
    DCAMERA_BURST_SNAPSHOT_PARAM_T	burst_snapshot_param;
    DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T burst_snapshot_return_param;
    DCAMERA_REVIEW_PARAM_T		review_param;

    DC_MJPG_PARAM_T				mjpeg_param;
    DC_DPLAYER_DISPLAY_PARAM_T	mjpeg_review_param;
    DCAMERA_RECORD_PARAM_T		upcc_param;
    DCAMERA_YUV_SRC_T           src_yuv;
    DCAMERA_CAPTURE_DATE_T      capture_date;
    DC_ATV_CALLBACK_T           atv_call_param;

    //REVIEW_INFO_T		review_info;
    uint16					max_photo_width;
    uint16					max_photo_height;
    uint16					lcd_width;
    uint16					lcd_height;
    uint16					target_width;
    uint16					target_height;
    uint16					actual_width;
    uint16					actual_height;

    uint32					burst_count;

    uint8*					buf_ptr;/*point to the buffer allocated by BL_Malloc()*/
    uint32					buf_len;

    DCAMERA_SIZE_T rtn_size;
    DCAMERA_DATA_LEN_T rtn_buf_len;
    ISP_DATA_ADDR_T		rtn_buf;
    ISP_DATA_ADDR_T		yyuv_buf;
    uint32				yyuv_buf_len;

    ISP_DATA_ADDR_T		quick_view_buf;
    uint32				quick_view_buf_len;    

    uint32					dc_buf_addr;
    uint32					dc_buf_len;

    uint8*					thumbnail_src_addr;
    uint32					thumbnail_src_size;

    uint32					thumbnail_buf_addr;
    uint32					thumbnail_buf_len;
    uint32					thumbnail_size;

    uint32					exif_buf_addr;
    uint32					exif_buf_len;

    uint8*					jpeg_buf_ptr;
    uint32					jpeg_buf_len;

    uint32					jpeg_size;
    uint32					enc_slice_height;

    uint8*					isp_capture_ptr;
    uint32					isp_capture_len;

    uint8*					isp_display_ptr;
    uint32					isp_display_len;

    uint8*					disp_buf_ptr;
    uint32					disp_buf_len;

    uint8*					rot_buf_ptr;
    uint32					rot_buf_len;

    uint32					jpg_yuv_buf_addr;
    uint32					jpg_yuv_buf_len;

    uint32					src_jpg_addr;
    uint32					src_jpg_len;

    uint32					minature_swap_buf_addr;
    uint32					minature_swap_buf_len;

    uint32					exif_swap_buf_addr;
    uint32					exif_swap_buf_len;

    uint8*					jpeg_fw_ptr;
    uint32					jpeg_fw_len;

    uint8*					cx_buf_ptr;
    uint32					cx_buf_len;

    uint8*					interpolation_buf_ptr;
    uint32					interpolation_buf_len;

    uint8*					frames_buf_ptr;
    uint32					frames_buf_len;
    uint32					i_frame;

    uint8*					dc_raw_buf_ptr;
    uint32					dc_raw_buf_len;

    uint8*					total_buf_ptr;
    uint32					total_buf_len;

    uint8*					remain_buf_ptr;
    uint32					remain_buf_len;

    uint8*                             disp_date_ptr;
    uint32                             disp_date_size;
    

    ISP_DATA_ADDR_T         combine_buf[MAX_COMBINE_NUM];
    uint32			        max_combine_num;    
    uint16			        combine_num;
    uint16			        panorama_dir; 

    DCAMERA_DISP_BG_COLOR_T bg_color;

    uint16                  back_line;
    uint16                  complete_line;
    uint16                  slice_remain_line;  

    SCI_TIMER_PTR			ovfw_timer_ptr;
    SCI_TIMER_PTR			burst_timer_ptr;

    DCAMERA_ATV_CALLBACK atv_search_callback;
    DCAMERA_ATV_CALLBACK atv_stop_callback;
    DCAMERA_ATV_CALLBACK atv_callback; 
    DCAMERA_CALL_BACK	zoom_callback;
    DCAMERA_PARAM_CTRL_CALL_BACK param_callback;
    DC_RECORD_CALLBACK_FUNC		mjpeg_record_callback;
    DCAM_RECORD_TYPE record_type;
    MM_FEATURE_SUPPORT_T  feature_context;
    DCAMERA_OP_MODE_E  dcamera_mode;
    
}DCAM_CONTEXT_T, *DCAM_CONTEXT_T_PTR;

/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC void DC_ISPZoomDone(void);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC void DC_SendMsg(DCAM_MSG_E sig_code, uint32 param);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC uint32 DC_TaskOpen(uint32 priority);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC uint32 DC_TaskClose(void);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC uint32 DCAMERA_SetAtvEnable(BOOLEAN is_eb) ;
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC int DC_StopMpeg4Rec(void);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC int DC_StartMpeg4Rec(uint32 dw_framenum, DC_RECORD_CALLBACK_FUNC pf_callback);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DC_Open(void);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC int DC_DisplayAFrame(DISPLAY_DATA_FRAME_T* p_frame);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC uint32 DC_GetFrameData(int (*func)(), DCAMERA_DATA_TYPE_E data_format);
/******************************************************************************/
// Description: 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC uint32 DC_SetSensorParamAll(void);
 
#ifdef __cplusplus
}
#endif


#endif
