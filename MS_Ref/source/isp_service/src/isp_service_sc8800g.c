/******************************************************************************
 ** File Name:    isp_service_sc8800g.c                                       *
 ** Author:       Baggio.He                                                   *
 ** DATE:         3/31/2010                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  SC8800G ISP service,                                        *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/31/2010     Baggio.he       Initial version                             *
 *****************************************************************************/
 
#ifndef _ISP_PATTERN_
#include "ms_ref_isp_service_trc.h"
#include "os_api.h"
#include "tasks_id.h"
#include "threadx_os.h"
#include "ref_tasks_def.h"
#include "dal_power.h" 
#include "deep_sleep.h"
#include "power_manager.h"
#include "chng_freq.h"
#include "dal_lcd.h"
#include "display.h"
#include "dc_cfg.h"
#include "lcd_cfg.h"
#else
#include "isp_service_pattern.h"
#endif

#include "lcdc_types.h"
#include "sc_reg.h"
#include "isp_service_sc8800g.h"
#include "ref_lcd.h"
#include "rotation_app.h"
#include "isp_service_cm.h"
#include "priority_system.h"

typedef  int (*ISP_USER_FUNC_PTR)(void*);
//service ID
#define ISP_SERVICE_IDLE                0x00
#define ISP_SERVICE_MPEG                0x01
#define ISP_SERVICE_JPEG                0x02
#define ISP_SERVICE_PREVIEW             0x03
#define ISP_SERVICE_SCALE               0x04
#define ISP_SERVICE_REVIEW              0x07
#define ISP_SERVICE_VP                  0x08
#define ISP_SERVICE_VP_ENCODE           0x09
#define ISP_SERVICE_VP_DECODE           0x10
#define ISP_SERVICE_VP_PLAYBACK         0x11
#define ISP_SERVICE_VP_EX               0x12
#define ISP_SERVICE_VP_EX_ENCODE        0x13
#define ISP_SERVICE_VP_EX_SCALE         0x14

//machine state ID
//machine state ID
#define ISP_STATE_STOP                  1
#define ISP_STATE_PREVIEW               2
#define ISP_STATE_CAPTURE               3
#define ISP_STATE_PAUSE                 4
#define ISP_STATE_SCALE_DONE            7
#define ISP_STATE_MPEG                  8

//isp next to when jpeg mode
#define ISP_STATE_CAPTURE_DONE          9
#define ISP_STATE_CAPTURE_SCALE         10
#define ISP_STATE_CAPTURE_CFA           11
#define ISP_STATE_VP                    12
#define ISP_STATE_IDLETEMP              13
#define ISP_STATE_REVIEW_DONE           14
#define ISP_STATE_REVIEW_SLICE          15
#define ISP_STATE_SCALE_SLICE           16
#define ISP_STATE_SCALE                 17
#define ISP_STATE_REVIEW                18
#define ISP_STATE_CLOSED                0xFF

#define ISP_OPEN_EVENT                  BIT_0
#define ISP_CLOSE_EVENT                 BIT_1
#define ISP_DISPLAY_EVENT               BIT_2
#define ISP_MPEG_START_EVENT            BIT_3
#define ISP_VT_EVENT                    BIT_4
#define ISP_REVIEW_EVENT                BIT_5
#define ISP_ROTATION_EVENT              BIT_6
#define ISP_VT_REMOTE_EVENT             BIT_7
#define ISP_SCALE_EVENT                 BIT_8
#define ISP_STOP_EVENT                  BIT_9
#define ISP_SET_PARAM_EVENT             BIT_10

#define ISP_SERVICE_THREAD              ISP
#define ISP_STACK_SIZE                  (1024 * 4)
#define ISP_QUEUE_NUM                   50
#define ISP_QUEUE_FEW_NUM               5
#define ISP_QUEUE_SAFE_NUM              40
#define ISP_VP_REVIEW_TIME              200 // SCI_WAIT_FOREVER//when in debuging,  //200 when in product   //ms
#define ISP_WAIT_TIME                   6000 // SCI_WAIT_FOREVER//6000 //ms
#define ISP_SERVICE_TIMEOUT             300 //if frame rate less is than 10/3,restart VT 

#define ISP_TRY_TIMES                   10
#define ISP_WORK_MEM_SIZE_DEFAULT       (2 << 20)
#define ISP_IN_BUF_SIZE                 (1 << 20)
#define ISP_MEM_ADDR_HIGH               0xFC000000
#define ISP_MEM_IS_IN_SAME_SEGE(x,y)    ((((uint32)(x))&ISP_MEM_ADDR_HIGH) == (((uint32)(y))&ISP_MEM_ADDR_HIGH))
#define ISP_MEM_STRIDER_FOR_ONE_FRAME   512
#define ISP_SCALE_SLICE_HEIGHT          128 // It's forcely suggested the slice height should be configed as 64 in case of address alignement
#define ISP_SCALE_LINE_BUF_SIZE(w)      (((w + 640 - 1) / 640) * 640 * 2 * 4 + ISP_MEM_STRIDER_FOR_ONE_FRAME)
#define ISP_SLICE_HEIGHT_TO_REUSE_MEM   16
#define ISP_SLICE_OUT_ESTIMATE(n)       (((n) + 8 + 16 -1) & ~0xF)
#define ISP_ALIGNED_BYTES               256
#define ISP_ADDRESS_ALIGNED_(a,n)       (((a) + (n) -1) / (n) * (n))

#define ISP_REVIEW_FRAME_NUM            2 // must be fewer than ISP_PATH1_FRAME_COUNT_MAX
#define ISP_ALIGNED_PIXELS              4
#define ISP_PIXEL_ALIGNED_(a,n)         ((((a) + (n) -1) / (n)) * (n))
#define ISP_PIXEL_LEFT_ALIGNED_(a,n)    ((a) & ~((n) - 1))
#define ISP_INVALID_ADDR                0x800
#define ISP_ADDR_INVALID(addr)          ((uint32)(addr) < ISP_INVALID_ADDR)
#define ISP_YUV_INVALIDE(y,u,v)         (ISP_ADDR_INVALID(y) && ISP_ADDR_INVALID(u) && ISP_ADDR_INVALID(v))

#define ISP_MAX(a,b)                    ((a) > (b) ? (a) : (b))
#define ISP_MIN(a,b)                    ((a) > (b) ? (b) : (a))

#define ISP_SERVICE_PARA_FREE           0x5A00
 
#define ISP_SERVICE_TRACE               SCI_TRACE_LOW
#define ISP_SERVICE_LOG                  // SCI_TRACE_LOW
#define ISP_SERVICE_ALLOC(n)            SCI_ALLOCA(n)
#define ISP_SERVICE_FREE(p)             do{ if(p) {SCI_FREE(p); p=SCI_NULL;}}while(0)

#define ISP_RTN_IF_ERR(n)               if(n)  goto exit

#define ISP_SET_ADDR(a,start,length)    a.yaddr = ISP_ADDRESS_ALIGNED_(start, ISP_ALIGNED_BYTES); \
                                        a.uaddr = ISP_ADDRESS_ALIGNED_(a.yaddr + length, ISP_ALIGNED_BYTES); \
                                        a.vaddr = 0;

										
#define ISP_SERVICE_SCALE_COEFF_BUF_SIZE        (8*1024)
enum
{
    VP_REVIEW_LOCAL_422TO420 = 0,
    VP_REVIEW_LOCAL_422TORGB,		
    VP_REVIEW_REMOTE_420TORGB,
    VP_EX_LOCAL_REVIEW,
    VP_EX_REMOTE_SCALE
};


typedef enum 
{
    ISP_SERVICE_OPEN_SIG = 1,
    ISP_SERVICE_CLOSE_SIG,
    ISP_SERVICE_PATH1_SIG,
    ISP_SERVICE_PATH2_SIG,
    ISP_SERVICE_SET_PARAM_SIG,
    ISP_SERVICE_START_SIG,
    ISP_SERVICE_STOP_SIG
}ISP_SERVICE_SIG_E;


typedef struct _isp_display_block_t
{
    uint32                              lcd_id;
    uint32                              block_id;//block id
    ISP_RECT_T                          img_rect;
    ISP_RECT_T                          lcd_inv_rect;
    uint32                              img_fmt;
    uint32                              is_enable;
}ISP_DISP_BLOCK_T;

typedef struct _isp_signal_t
{
    xSignalHeaderRec                    sig;
    uint32                              command;
    uint32                              param;
    void                                *ptr;
}ISP_SIGNAL_T;

enum
{
    ISP_SERVICE_PERFORMANCE_HIGH   = 1,
    ISP_SERVICE_PERFORMANCE_MID,
    ISP_SERVICE_PERFORMANCE_LOW,
    ISP_SERVICE_PERFORMANCE_LOWEST
};


typedef enum
{
    ISP_SERVICE_PATH1_TIMEOUT = 0x00,
    ISP_SERVICE_TIMER_MODE_MAX
}ISP_SERVICE_TIMER_MODE_E;	

typedef struct _isp_service_t
{
    uint32                              service;//service id;
    uint32                              state;//machine state
    uint32                              module_addr;

    /*for cap*/
    uint32                              cap_input_image_format;
    uint32                              cap_input_image_patten;
    uint32                              vsync_polarity;
    uint32                              hsync_polarity;
    uint32                              pclk_polarity;

    uint32                              is_first_frame;
    uint32                              preview_skip_frame_num;
    uint32                              preview_deci_frame_num;	
    uint32                              capture_skip_frame_num;
    ISP_SIZE_T                          cap_input_size;   
    ISP_RECT_T                          cap_input_range;
    ISP_CAP_DEC_T                       cap_img_dec;
    ISP_SIZE_T                          cap_output_size;	
	
    /*for review , scale , vt review */	
    uint32                              path2_input_image_format;	
    uint32                              path2_output_image_format;
    ISP_SIZE_T                          input_size;   
    ISP_RECT_T                          input_range;	
    ISP_ADDRESS_T                       input_addr;	
    ISP_ADDRESS_T                       swap_buff;
    ISP_ADDRESS_T                       line_buff;
    ISP_ADDRESS_T                       output_addr;
    uint32                              is_slice;
    uint32                              slice_height;  
    uint32                              slice_out_height;      
    uint32                              total_scale_out;
    uint32                              total_slice_count;
    uint32                              vt_review_from; // 0 local, 1, remote
    uint32                              is_review_dc;
    uint32                              is_slice_alone;
    
    /*local display(preview,review parameter , include block,round buffer,*/
    uint32                              local_review_enable;    
    uint32                              local_display_enable;
    ISP_DISP_BLOCK_T                    display_block;
    ISP_RECT_T                          display_range;// preview, review,mpeg preview	
    ISP_ADDRESS_T                       display_addr[ISP_PATH1_FRAME_COUNT_MAX];
    ISP_FRAME_T                         display_frame;
    ISP_FRAME_T                         *display_frame_locked_ptr;
    ISP_ROTATION_E                      display_rotation;
    ISP_ADDRESS_T                       rotation_buf_addr[ISP_PATH1_FRAME_COUNT_MAX];
    ISP_ADDRESS_T                       local_addr[ISP_PATH1_FRAME_COUNT_MAX];
    uint32                              display_frame_count;    
    
    /*remote display(vt review) parameter , include block,round buffer,*/
    ISP_SIZE_T                          remote_input_size;   
    ISP_RECT_T                          remote_input_range;	
    ISP_ADDRESS_T                       remote_input_addr;	
	
    uint32                              remote_deblock_enable;
    uint32                              remote_review_enable;    
    uint32                              remote_display_enable;
    ISP_DISP_BLOCK_T                    remote_display_block;
    ISP_RECT_T                          remote_display_range;	
    ISP_ADDRESS_T                       remote_display_addr[ISP_PATH2_FRAME_COUNT_MAX];
    ISP_ROTATION_E                      remote_display_rotation; 
    ISP_ADDRESS_T                       remote_rotation_display_addr[ISP_PATH2_FRAME_COUNT_MAX];

    ISP_SIZE_T                          encoder_size;	//for jpeg capture ,mpeg capture, vt capture, scale out
    ISP_ADDRESS_T                       encoder_addr[ISP_PATH2_FRAME_COUNT_MAX];
    ISP_ADDRESS_T                       encoder_rot_addr[ISP_PATH2_FRAME_COUNT_MAX];
    ISP_ROTATION_E                      enc_rotation; 
	uint32								enc_stoped;
    uint32                              is_enc_first_frame;	
    uint32                              jpeg_capture_is_composed;
    ISP_FRAME_T                         *p_frame_enc;
    ISP_FRAME_T                         *p_frame_enc_cur;    
    uint32                              vt_local_encode_count;    
    uint32                              vt_local_display_count;    	
    uint32                              vt_review_remote_count;
    uint32                              vt_stop_arrived;
    uint32                              vt_mem_end_cur;
    
    /*control variable*/
    ISP_USER_FUNC_PTR                   display_user_cb;
    SCI_MUTEX_PTR                       display_user_cb_ctrl;    
    ISP_USER_FUNC_PTR                   encode_user_cb;
    SCI_MUTEX_PTR                       encode_user_cb_ctrl;
    SCI_MUTEX_PTR                       mem_ctrl;    
    uint32                              chng_freq_req_handler;
    get_data                            scale_user_cb;
    SCI_SEMAPHORE_PTR                   review_path_sema;
    uint32                              curr_queue_available;
    uint32                              b_wait_for_vt_event;
    SCI_TIMER_PTR                       timer_ptr;	
    uint32                              timer_mode;
    uint32                              sys_performance_level;
    uint32                              watchdog_feeded;
    uint32                              mem_alloced_by_myself;
    void                                *p_work_mem;
    uint32                              work_mem_len;
    void                                *p_work_mem_cur;
    uint32                              work_mem_res_len;
    uint32                              err_code;    
    uint32                              rgb_ex_flag;
    uint32                              rgb_ex_height;    
    uint32                              *p_scale_coeff_mem;
    void                                *p_in_buf;
    uint32                              in_buf_len;
}ISP_SERVICE_T;



//thread context
LOCAL uint32                 s_isp_service_thread_id = SCI_INVALID_BLOCK_ID;
LOCAL uint32                 _ISP_ServiceStack[ISP_STACK_SIZE >> 2];
LOCAL uint32                 _ISP_ServiceQueue[ISP_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];
LOCAL ISP_SERVICE_T          s_isp_service = {0};
LOCAL SCI_EVENT_GROUP_PTR    s_isp_event = SCI_NULL;
LOCAL SCI_SEMAPHORE_PTR      s_isp_service_sema = SCI_NULL;
LOCAL uint32                 s_b_wait_for_opened = 0;

LOCAL void    _ISP_ServiceCreate(void);
LOCAL void    _ISP_ServiceFlushQueue(void);
LOCAL void    _ISP_ServiceMainMessageLoop(uint32 argc, void *argv);
LOCAL void    _ISP_ServiceOpenSignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServiceCloseSignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServicePath1SignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServicePath2SignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServiceScaleSignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServiceSetParamSignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServiceStartSignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServiceStopSignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL int32   _ISP_ServiceSendRequest(uint32 ptr_free_flag,uint32 command,uint32 param,void * ptr);
LOCAL BOOLEAN _ISP_ServiceCommandLostPermit(uint32 command);
LOCAL uint32  _ISP_ServiceGetQueueAvailable(void);
LOCAL void    _ISP_ServiceConfigBlock(ISP_DISP_BLOCK_T *p_disp_block,
                                      ISP_RECT_T       *p_range,
                                      ISP_RECT_T       *p_inv_range,
                                      uint32           rot_angle);
LOCAL void    _ISP_ServiceCloseBlock(void);
LOCAL void    _ISP_ServiceClearBlockBuf(ISP_DISP_BLOCK_T *p_disp_blk);
LOCAL void    _ISP_ServiceRecoverBlkCfg(uint32 block_id);
LOCAL void    _ISP_ServiceDisplayOneFrame(uint32 block_id,ISP_FRAME_T *p_frame);
LOCAL void    _ISP_ServiceRotateImage(ISP_ADDRESS_T *p_src, 
                                      ISP_ADDRESS_T *p_dst, 
                                      uint32 width,
                                      uint32 height,
                                      uint32 rot_angle,
                                      uint32 img_format);
LOCAL void    _ISP_ServiceInit(void);
LOCAL void    _ISP_ServiceDeInit(void);
LOCAL void    _ISP_ServiceGetDispCtrl(void);
LOCAL void    _ISP_ServiceRelDispCtrl(void);
LOCAL void    _ISP_ServiceGetEncCtrl(void);
LOCAL void    _ISP_ServiceRelEncCtrl(void);
LOCAL void    _ISP_ServiceCallUserDispCB(void *p_frame);
LOCAL void    _ISP_ServiceCallUserEncCB(void *p_frame);
LOCAL void    _ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceSetJpegParam(ISP_JPEG_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceSetMpegParam(ISP_MPEG_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceReviewWindowRearrange(ISP_RECT_T *p_input_range,
                                                ISP_RECT_T *p_disp_range,
                                                ISP_RECT_T *p_lcd_rect,
                                                uint32     rot_angle);
LOCAL void    _ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceSetScaleParam(ISP_SCLAE_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceEncodeFrameInit(void);
LOCAL void    _ISP_ServiceEncodeFrameDeInit(void);
LOCAL void    _ISP_ServiceSetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceSetVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceEnterVTReview(void);
LOCAL void    _ISP_ServiceExitVTReview(void);
LOCAL uint32  _ISP_ServiceCheckPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceCheckJpegParam(ISP_JPEG_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceCheckMpegParam(ISP_MPEG_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceCheckReviewParam(ISP_REVIEW_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceCheckScaleParam(ISP_SCLAE_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceCheckVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceCheckVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceSetVPFlag(uint32 flag);
LOCAL void    _ISP_ServiceStartVTReview(ISP_ADDRESS_T *p_review);
LOCAL void    _ISP_ServiceStartVTPlayBack(void *ptr);
LOCAL void    _ISP_ServiceStartVTLocalPlayBack(ISP_FRAME_T *ptr);
LOCAL void    _ISP_ServiceTimeout(void);
LOCAL void    _ISP_ServiceTimerExpireHandle(uint32 param);
LOCAL void    _ISP_ServiceCloseTimer(void);
LOCAL void    _ISP_ServiceDeleteTimer(void);
LOCAL BOOLEAN _ISP_ServiceCreateTimer(void);
LOCAL BOOLEAN _ISP_ServiceStartTimer(uint32 timer_mode,uint32 expire_value);
LOCAL void    _ISP_ServiceStartPreview(void);
LOCAL void    _ISP_ServiceStartJpeg(void);
LOCAL void    _ISP_ServiceStartMpeg(void);
LOCAL void    _ISP_ServiceStartReview(void);
LOCAL void    _ISP_ServiceContinueReviewing(void* addr_ptr);
LOCAL void    _ISP_ServiceStartScale(void);
LOCAL void    _ISP_ServiceContinueScaling(void);
LOCAL void    _ISP_ServiceStartVTEncode(void);
LOCAL void    _ISP_ServiceSetCompoundJpegFlag(BOOLEAN is_on);
LOCAL void    _ISP_ServiceGetImageFromLCDC(ISP_FRAME_T *p_isp_frame);
LOCAL BOOLEAN _ISP_ServiceGetCompoundJpegFlag(void);
LOCAL uint32  _ISP_ServiceGetXYDeciFactor(uint32* src_width, 
                                          uint32* src_height,
                                          uint32  dst_width,
                                          uint32  dst_height);
LOCAL void    _ISP_ServiceOnSensorSOF(void *p);
LOCAL void    _ISP_ServiceOnSensorEOF(void *p);
LOCAL void    _ISP_ServiceOnCAPSOF(void *p);
LOCAL void    _ISP_ServiceOnCAPEOF(void *p);
LOCAL void    _ISP_ServiceOnCAPFifoOverflow(void *p);
LOCAL void    _ISP_ServiceOnSensorLineErr(void *p);
LOCAL void    _ISP_ServiceOnSensorFrameErr(void *p);
LOCAL void    _ISP_ServiceOnJpegBufOverflow(void *p);
LOCAL void    _ISP_ServiceOnPath1(void *p);
LOCAL void    _ISP_ServiceOnPath2(void *p);
LOCAL BOOLEAN _ISP_ServiceAllocFrame(void);
LOCAL uint32  _ISP_ServiceRotAngle2DrvAngle(ISP_ROTATION_E isp_angle);
LOCAL void    _ISP_ServiceStartVTScale(ISP_FRAME_T *p_src_frame, 
                                       ISP_RECT_T  *p_trim_rect,
                                       ISP_FRAME_T *p_dst_frame);
LOCAL void*   _ISP_ServiceGetVTExtScaleBuf(uint32  *p_buf_len,
                                           uint32  width, 
                                           uint32  height, 
                                           uint32  img_format);
LOCAL void    _ISP_ServiceSetVTExtEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceSetVTExtScaleParam(ISP_SCLAE_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceCheckVTExtEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);
LOCAL void    _ISP_ServiceStartVTExtScale(void);
LOCAL void    _ISP_ServiceStartVTExtEncode(void);
LOCAL void    _ISP_ServiceRGB565FromLittleToHalfBig(uint32 rgb565_addr,uint32 pixel_num);
LOCAL void    _ISP_ServiceRGB888FromBigToLittle(uint32, uint32);
LOCAL void    _ISP_ServiceSyncImageData(ISP_ADDRESS_T*        p_addr, 
                                        uint32                img_width,
                                        uint32                img_height,                                        
                                        uint32                img_format, 
                                        DMABUFFER_DIRECTION_E data_dir);
LOCAL BOOLEAN _ISP_ServiceCheckMemAddr(ISP_ADDRESS_T  *p_addr,
                                       ISP_SIZE_T     *input_size,
                                       uint32         img_fmt,
                                       uint32         mem_start,
                                       uint32         *p_mem_len);
LOCAL BOOLEAN _ISP_ServiceSyncMem2Output(ISP_ADDRESS_T* p_internal,
                                          ISP_ADDRESS_T* p_external,
                                          ISP_SIZE_T*    input_size,
                                          uint32         img_fmt);
#if 0
LOCAL uint32  _ISP_ServiceGetBytePerPixel(uint32 resolution);
LOCAL uint32  _ISP_ServiceGetResolution(uint32 resolution);
#endif

#ifndef _ISP_PATTERN_

extern SCI_THREAD_PTR SCI_GetThreadBlockFromId(BLOCK_ID thread_id);

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
PUBLIC BOOLEAN ISP_ServiceIsIdle(void)
{
	return TRUE;
}

void ISP_ServiceInit(void)
{
    if(SCI_INVALID_BLOCK_ID == s_isp_service_thread_id)
    {
        _ISP_ServiceCreate();
    }
}

uint32 ISP_ServiceOpen(uint32 wait_option)
{
    ISP_SERVICE_T    *s = &s_isp_service;
    uint32           actual_event = 0;
    uint32           status = 0;
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceOpen, service = %d, state %d, cur_time %d, wait_opt %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_475_112_2_18_1_44_14_543,(uint8*)"dddd", s->service,s->state,SCI_GetTickCount(),wait_option);

    if(s_isp_service_sema)
    {
        status = SCI_GetSemaphore(s_isp_service_sema, wait_option);
    	if(status)
    	{
            //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceOpen, time out %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_482_112_2_18_1_44_14_544,(uint8*)"d",SCI_GetTickCount());
            return SCI_ERROR;
    	}        
        s_b_wait_for_opened = 1;
    }
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceOpen, Got controller,cur_timr %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_488_112_2_18_1_44_14_545,(uint8*)"d",SCI_GetTickCount());
    _ISP_ServiceSendRequest(0,ISP_SERVICE_OPEN_SIG,0,0);
    status = SCI_GetEvent(s_isp_event, 
                          ISP_OPEN_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

    return status;
}


uint32 ISP_ServiceClose(void)
{
    ISP_SERVICE_T    *s = &s_isp_service;
    uint32           actual_event = 0; 
    uint32           status = 0;
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceClose, service = %d, status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_508_112_2_18_1_44_14_546,(uint8*)"dd", s->service,s->state);

    if(ISP_STATE_CLOSED == s->state && 1 != s_b_wait_for_opened)
    {
        return SCI_SUCCESS;
    }
    
    ISP_DriverStop(s->module_addr);

    _ISP_ServiceFlushQueue();
    _ISP_ServiceSendRequest(0,ISP_SERVICE_CLOSE_SIG,0,0);
    status = SCI_GetEvent(s_isp_event, 
                          ISP_CLOSE_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

    /* When get event fail, do not assert, just continue */
    if(s_isp_service_sema && (1 == s_b_wait_for_opened))
    {
        s_b_wait_for_opened = 0;
        SCI_PutSemaphore(s_isp_service_sema);	
    }
    
    return status;
    
}

void ISP_ServiceSetClk(uint32 clk)
{

}

void ISP_ServiceReviewBypass(void)
{

    ISP_DISP_BLOCK_T    *disp_block_ptr = &s_isp_service.display_block;
    
    LCD_InvalidateRect(disp_block_ptr->lcd_id,
                       disp_block_ptr->lcd_inv_rect.x,
                       disp_block_ptr->lcd_inv_rect.y,
                       disp_block_ptr->lcd_inv_rect.x + disp_block_ptr->lcd_inv_rect.w - 1,
                       disp_block_ptr->lcd_inv_rect.y + disp_block_ptr->lcd_inv_rect.h - 1);		   

    return ;
	


}

uint32 ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_PREVIEW_PARAM_T     *p_local_para = SCI_NULL;
    uint32	                status,actual_event;
	
    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetPreviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_566_112_2_18_1_44_14_547,(uint8*)"");
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x ,y,w,h {%d,%d,%d,%d} lcd rect {%d,%d,%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_576_112_2_18_1_44_14_548,(uint8*)"dddddddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h,param_ptr->lcd_rect.x,param_ptr->lcd_rect.y,param_ptr->lcd_rect.w,param_ptr->lcd_rect.h);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:lcd_id %d,block_id %d, disp_range.x,y,w,h {%d,%d,%d,%d} rot mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_585_112_2_18_1_44_14_549,(uint8*)"ddddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h,param_ptr->disp_rotation);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:skip_frames %d, deci_frames %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_589_112_2_18_1_44_14_550,(uint8*)"dd",param_ptr->cap_param.cap_frame_skipped,param_ptr->cap_param.cap_frame_decimated);

    if(_ISP_ServiceCheckPreviewParam(param_ptr))
    {
        return SCI_ERROR;
    }	
	
    p_local_para = (ISP_PREVIEW_PARAM_T*)ISP_SERVICE_ALLOC(sizeof(ISP_PREVIEW_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_PREVIEW_PARAM_T));
    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_PREVIEW,
                            (void*)p_local_para);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);    
    
    if(status == SCI_SUCCESS)
    {
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_619_112_2_18_1_44_14_551,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }

    return status;
}

uint32 ISP_ServiceGetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    ISP_SERVICE_T         *s = &s_isp_service;

    if(!param_ptr)
        return SCI_ERROR;


    SCI_MEMCPY((void*)&(param_ptr->input_size), 
                (void*)&(s->cap_input_size),
                sizeof(ISP_SIZE_T));

    SCI_MEMCPY((void*)&(param_ptr->input_range), 
                (void*)&(s->cap_input_range),
                sizeof(ISP_RECT_T));

    SCI_MEMCPY((void*)&(param_ptr->disp_range), 
                (void*)&(s->display_range), 
                sizeof(ISP_RECT_T));

    param_ptr->disp_lcd_id = s->display_block.lcd_id;    
    param_ptr->disp_block_id = s->display_block.block_id;

    param_ptr->disp_rotation = s->display_rotation;
    return SCI_SUCCESS;    
}

uint32 ISP_ServiceStartPreview(void)
{
    ISP_SERVICE_T         *s = &s_isp_service;
    uint32                actual_event = 0;
    uint32                status = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_660_112_2_18_1_44_14_552,(uint8*)"");
    _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_PREVIEW,0);
    s->is_first_frame = 1;  
        
    status = SCI_GetEvent(s_isp_event, 
                      	  ISP_DISPLAY_EVENT, 
                       	  SCI_AND_CLEAR, 
                       	  &actual_event, 
                       	  ISP_WAIT_TIME);

    LCD_SetInvalidateLimit(s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
    
    return status;
}


void ISP_ServiceStopPreview(void)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    uint32                 status = SCI_SUCCESS;
    uint32                 actual_event;
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStopPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_682_112_2_18_1_44_15_553,(uint8*)"");

    if(s->state != ISP_STATE_PREVIEW)
    {
        return ;
    }
    ISP_DriverStop(s->module_addr);

    _ISP_ServiceFlushQueue();
	
    _ISP_ServiceSendRequest(0,ISP_SERVICE_STOP_SIG,0,0);
    status = SCI_GetEvent(s_isp_event, 
                          ISP_STOP_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

    //_ISP_ServiceCloseBlock can not be called here , for it will disable the block
    //so, just clear the enable flag.
    s->display_block.is_enable = 0;
   
    LCD_SetInvalidateLimit(s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
   
    return ;
}

void ISP_ServiceStartReview(void)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    uint32                 actual_event = 0;
    uint32                 status = 0;
    ISP_USER_FUNC_PTR      user_cb = SCI_NULL;
  
    if(ISP_STATE_CLOSED == s->state)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartReview,wrong state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_717_112_2_18_1_44_15_554,(uint8*)"");
        return ;
    }
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartReview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_721_112_2_18_1_44_15_555,(uint8*)"");

    _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_REVIEW,0);
    
    s->is_first_frame = 1;  
    
    _ISP_ServiceGetDispCtrl();
    user_cb = s->display_user_cb;     
    _ISP_ServiceRelDispCtrl();
	
    if(SCI_NULL == user_cb)
    {
        status = SCI_GetEvent(s_isp_event, 
                              ISP_DISPLAY_EVENT, 
                              SCI_AND_CLEAR, 
                              &actual_event, 
                              ISP_WAIT_TIME);
		                       	  
        SCI_ASSERT(!status); /*assert verified*/
    }
	
    return ;
}

uint32 ISP_ServiceReviewNext(ISP_ADDRESS_T *addr_ptr)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    ISP_ADDRESS_T          *p_local_para = SCI_NULL;
    uint32                 status = 0,actual_event;
    ISP_USER_FUNC_PTR      user_cb = SCI_NULL;
    
    if(ISP_ADDR_INVALID(addr_ptr) ||
       ISP_YUV_INVALIDE(addr_ptr->yaddr, addr_ptr->uaddr, addr_ptr->vaddr))
    {
        return SCI_ERROR;
    }

    if(!ISP_MEM_IS_IN_SAME_SEGE(addr_ptr->yaddr,s->p_work_mem) ||
       !ISP_MEM_IS_IN_SAME_SEGE(addr_ptr->uaddr,s->p_work_mem))   
    {
        
    }
    
    p_local_para = (ISP_ADDRESS_T*)ISP_SERVICE_ALLOC(sizeof(ISP_ADDRESS_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceReviewNext address 0x%x, {0x%x,0x%x,0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_773_112_2_18_1_44_15_556,(uint8*)"dddd",p_local_para,addr_ptr->yaddr,addr_ptr->uaddr,addr_ptr->vaddr);
    
    SCI_MEMCPY((void*)p_local_para, 
               (void*)addr_ptr, 
               sizeof(ISP_ADDRESS_T));

    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_REVIEW,
                            (void*)p_local_para);    
    
    _ISP_ServiceGetDispCtrl();
    user_cb = s->display_user_cb;     
    _ISP_ServiceRelDispCtrl();    

    if(SCI_NULL == user_cb)
    {
        status = SCI_GetEvent(s_isp_event, 
                              ISP_DISPLAY_EVENT, 
                              SCI_AND_CLEAR, 
                              &actual_event, 
                              ISP_WAIT_TIME);
		                       	  
        SCI_ASSERT(!status); /*assert verified*/
    }
	
    return SCI_SUCCESS;
    
}

uint32 ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T *param_ptr)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    ISP_REVIEW_PARAM_T     *p_local_para = SCI_NULL;
    uint32	               status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetReviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_811_112_2_18_1_44_15_557,(uint8*)"");
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input format = %d, input.x,y,w,h {%d %d %d %d} lcd rect {%d,%d,%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_822_112_2_18_1_44_15_558,(uint8*)"ddddddddd",param_ptr->input_format, param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h,param_ptr->lcd_rect.x,param_ptr->lcd_rect.y,param_ptr->lcd_rect.w,param_ptr->lcd_rect.h);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} rot mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_831_112_2_18_1_44_15_559,(uint8*)"ddddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h,param_ptr->disp_rotation);
	
    if(_ISP_ServiceCheckReviewParam(param_ptr))
    {
        return SCI_ERROR;
    }
	
    p_local_para = (ISP_REVIEW_PARAM_T*)ISP_SERVICE_ALLOC(sizeof(ISP_REVIEW_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_REVIEW_PARAM_T));

    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_REVIEW,
                            (void*)p_local_para);


    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

    if(status == SCI_SUCCESS)
    {
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_863_112_2_18_1_44_15_560,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }
      		
    return status;	
}
void	ISP_ServiceReviewFromDC(uint32 b_from_dc)
{
    ISP_SERVICE_T          *s = &s_isp_service;

    s->is_review_dc = b_from_dc;
}


uint32 ISP_ServiceSetJpegCaptureParam(ISP_JPEG_PARAM_T * param_ptr)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    ISP_JPEG_PARAM_T       *p_local_para = SCI_NULL;
    uint32	               status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_886_112_2_18_1_44_15_561,(uint8*)"");
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_892_112_2_18_1_44_15_562,(uint8*)"dddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_898_112_2_18_1_44_15_563,(uint8*)"dddd",param_ptr->yuv_type,param_ptr->output_size.w,param_ptr->output_size.h,param_ptr->cap_param.cap_frame_skipped);
	

	
    if(_ISP_ServiceCheckJpegParam(param_ptr))
    {
        return SCI_ERROR;
    }

    p_local_para = (ISP_JPEG_PARAM_T*)ISP_SERVICE_ALLOC(sizeof(ISP_JPEG_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_JPEG_PARAM_T));
	
    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_JPEG,
                            (void*)p_local_para);


    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);		
    
    if(status == SCI_SUCCESS)
    {
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_932_112_2_18_1_44_15_564,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }
        
    return status;

}


void ISP_ServiceStartCaptureJpeg(void)
{
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartCaptureJpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_944_112_2_18_1_44_15_565,(uint8*)"");
	
    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_JPEG,
                            0);
}


uint32 ISP_ServiceSetCompoundJpegCaptureParam(ISP_COMPOUND_JPEG_CAPTURE_PARAM_T * param_ptr)
{

    ISP_SERVICE_T          *s = &s_isp_service;
	
    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetCompoundJpegCaptureParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_960_112_2_18_1_44_15_566,(uint8*)"");
	
	
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input format = %d,yuv_type = %d,output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_968_112_2_18_1_44_15_567,(uint8*)"dddd",param_ptr->input_format, param_ptr->yuv_type,param_ptr->output_size.w,param_ptr->output_size.h);

    s->input_size.w = param_ptr->input_range.w;
    s->input_size.h = param_ptr->input_range.h;	
	
    SCI_MEMCPY((void*)&s->input_range, 
               (void*)&param_ptr->input_range, 
               sizeof(ISP_RECT_T));
               
    SCI_MEMCPY((void*)&s->encoder_size, 
               (void*)&param_ptr->output_size, 
               sizeof(ISP_SIZE_T));
	
    s->is_slice = 0;
    s->cap_input_image_format = param_ptr->input_format;
    s->service = ISP_SERVICE_JPEG;
		
    return SCI_SUCCESS;
}


uint32 ISP_ServiceSetMpegCaptureParam(ISP_MPEG_PARAM_T * param_ptr)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    ISP_MPEG_PARAM_T        *p_local_para = SCI_NULL;
    uint32	                status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetMpegCaptureParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_997_112_2_18_1_44_15_568,(uint8*)"");
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE: input.x,y,w,h {%d %d %d %d},skip_frames %d, deci_frames %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1005_112_2_18_1_44_15_569,(uint8*)"dddddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h,param_ptr->cap_param.cap_frame_skipped,param_ptr->cap_param.cap_frame_decimated);
   
    //ISP_SERVICE_TRACE:"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}, lcd rect {%d,%d,%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1014_112_2_18_1_44_15_570,(uint8*)"ddddddd",param_ptr->yuv_type,param_ptr->encode_size.w,param_ptr->encode_size.h,param_ptr->lcd_rect.x,param_ptr->lcd_rect.y,param_ptr->lcd_rect.w,param_ptr->lcd_rect.h);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} disp rot %d enc rot %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1024_112_2_18_1_44_15_571,(uint8*)"dddddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h,param_ptr->disp_rotation,param_ptr->encode_rotation);
	
    if(_ISP_ServiceCheckMpegParam(param_ptr))
    {
        return SCI_ERROR;
    }
	
    p_local_para = (ISP_MPEG_PARAM_T*)ISP_SERVICE_ALLOC(sizeof(ISP_MPEG_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_MPEG_PARAM_T));
	
    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_MPEG,
                            (void*)p_local_para);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    
    if(status == SCI_SUCCESS)
    {
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1055_112_2_18_1_44_15_572,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }
      		
    return status;
}

uint32 ISP_ServiceGetMpegCaptureParam(ISP_MPEG_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    
    if(!param_ptr)
    {
        return SCI_ERROR;
    }
    
    if(s->service != ISP_SERVICE_MPEG)
    {
        //ISP_SERVICE_LOG:"ISP_ServiceGetMpegCaptureParam, Not in MPEG mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1074_112_2_18_1_44_15_573,(uint8*)"");
        return SCI_ERROR;        
    }


    SCI_MEMCPY((void*)&(param_ptr->input_size), 
               (void*)&(s->cap_input_size),
               sizeof(ISP_SIZE_T));
    
    SCI_MEMCPY((void*)&(param_ptr->input_range), 
               (void*)&(s->cap_input_range),
               sizeof(ISP_RECT_T));

    if(s->enc_rotation == ISP_ROTATION_90 || 
       s->enc_rotation == ISP_ROTATION_270)
    {
        param_ptr->encode_size.h = s->encoder_size.w;
        param_ptr->encode_size.w = s->encoder_size.h;
    }
    else
    {
        param_ptr->encode_size.w = s->encoder_size.w;
        param_ptr->encode_size.h = s->encoder_size.h;
    }        

    SCI_MEMCPY((void*)&(param_ptr->disp_range), 
               (void*)&(s->display_range), 
               sizeof(ISP_RECT_T));
    
    if(s->display_rotation == ISP_ROTATION_90 ||
       s->display_rotation == ISP_ROTATION_270) // width and height switched in BlockConfig,here just recorver
    {
        param_ptr->disp_range.w = s->display_range.h;    
        param_ptr->disp_range.h = s->display_range.w;            
    }

    SCI_MEMCPY((void*)&(param_ptr->lcd_rect), 
               (void*)&(s->display_block.lcd_inv_rect), 
               sizeof(ISP_RECT_T));
    
    param_ptr->disp_lcd_id = s->display_block.lcd_id;    
    param_ptr->disp_block_id = s->display_block.block_id;
	param_ptr->data_format = s->path2_output_image_format;
    param_ptr->encode_rotation = s->enc_rotation;
    param_ptr->disp_rotation = s->display_rotation;
    param_ptr->cap_param.cap_img_format = s->cap_input_image_format;
    param_ptr->cap_param.cap_img_pattern = s->cap_input_image_patten;
    param_ptr->cap_param.cap_vsync_pol = s->vsync_polarity;
    param_ptr->cap_param.cap_hsync_pol = s->hsync_polarity;
    param_ptr->cap_param.cap_pclk_pol = s->pclk_polarity;
    param_ptr->cap_param.cap_frame_skipped = s->preview_skip_frame_num;
    param_ptr->cap_param.cap_frame_decimated = s->preview_deci_frame_num;

    return SCI_SUCCESS;}

uint32 ISP_ServiceStartCaptureMpeg(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = 0;
    uint32                  actual_event = 0;
    uint32                  event = 0;
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartCaptureMpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1136_112_2_18_1_44_15_574,(uint8*)"");

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_MPEG,
                            0);
    
    s->is_first_frame = 1;
    s->is_enc_first_frame = 1;
	s->enc_stoped = 0;

    event = ISP_DISPLAY_EVENT|ISP_MPEG_START_EVENT;

       
    status = SCI_GetEvent(s_isp_event, 
                      	  event, 
                       	  SCI_AND_CLEAR, 
                       	  &actual_event, 
                       	  ISP_WAIT_TIME);

    if(s->display_block.lcd_id != ISP_DISPLAY_NONE)
    {
        LCD_SetInvalidateLimit(s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
    }
                       	  
    return status;
}

BOOLEAN ISP_ServiceIsMpegOn(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceIsMpegOn, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1168_112_2_18_1_44_16_575,(uint8*)"d", s->service);
    
    return (s->service == ISP_SERVICE_MPEG);
    
}

void ISP_ServiceStartCaptureCompoundJpeg(void)
{
    _ISP_ServiceSetCompoundJpegFlag(SCI_TRUE);
}


void ISP_ServiceStopCapture(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = SCI_SUCCESS;
    uint32                  actual_event;
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStopCapture"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1186_112_2_18_1_44_16_576,(uint8*)"");

    if(s->state != ISP_STATE_MPEG)
    {
        return ;
    }
	s->enc_stoped = 1;
    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_STOP_SIG,
                            0,
                            0);
                            
    status = SCI_GetEvent(s_isp_event, 
                          ISP_STOP_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    
    _ISP_ServiceCloseBlock();
    if(s->enc_rotation != ISP_ROTATION_0)	
    {
        _ISP_ServiceEncodeFrameDeInit();
    }
    return ;
}

void ISP_ServiceStopReview(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = SCI_SUCCESS;
    uint32                  actual_event;
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStopReview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1218_112_2_18_1_44_16_577,(uint8*)"");

    if(s->state != ISP_STATE_REVIEW)
    {
        return ;
    }
    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_STOP_SIG,
                            0,
                            0);
                            
    status = SCI_GetEvent(s_isp_event, 
                          ISP_STOP_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    
    _ISP_ServiceCloseBlock();
    
    return ;
}


uint32 ISP_ServiceSetScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    ISP_SCLAE_PARAM_T       *p_local_para = SCI_NULL;
    uint32	                status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1249_112_2_18_1_44_16_578,(uint8*)"");
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1256_112_2_18_1_44_16_579,(uint8*)"ddddd",param_ptr->input_format, param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1261_112_2_18_1_44_16_580,(uint8*)"ddd", param_ptr->input_addr.yaddr, param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1266_112_2_18_1_44_16_581,(uint8*)"ddd",param_ptr->output_format,param_ptr->output_size.w,param_ptr->output_size.h);
	
	
    if(_ISP_ServiceCheckScaleParam(param_ptr))
    {
        return SCI_ERROR;
    }

    p_local_para = (ISP_SCLAE_PARAM_T*)ISP_SERVICE_ALLOC(sizeof(ISP_SCLAE_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_SCLAE_PARAM_T));

	
    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_SCALE,
                            (void*)p_local_para);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    
    if(status == SCI_SUCCESS)
    {
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1299_112_2_18_1_44_16_582,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }
         
    return status;
}


void ISP_ServiceStartScale(void)
{
    uint32	                status,actual_event;

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_SCALE,
                            0);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SCALE_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

    return ;

}


uint32 ISP_ServiceGetFrameAddress(void)
{
    return 0;    
}


void ISP_ServiceFreeFrameAddress(ISP_FRAME_T *frame_ptr)
{
    //ISP_SERVICE_LOG:"ISP_SERVICE: ISP_ServiceFreeFrameAddress, frame 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1336_112_2_18_1_44_16_583,(uint8*)"d",frame_ptr);

    ISP_DriverFrameUnLock(frame_ptr);
}

uint8 ISP_ServiceIsFrameLocked(ISP_FRAME_T *frame_ptr)
{
    return ISP_DriverFrameIsLocked(frame_ptr);
}

int32 ISP_ServiceFrameLock(ISP_FRAME_T *f)
{
    return ISP_DriverFrameLock(f);
}
int32 ISP_ServiceFrameUnLock(ISP_FRAME_T *f)
{
    return ISP_DriverFrameUnLock(f);    
}

void ISP_ServiceStartVT(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32	                status,actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartVT"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1360_112_2_18_1_44_16_584,(uint8*)"");
	
    if(ISP_STATE_PREVIEW == s->state)
    {
        ISP_ServiceStopPreview();
    }
    
    s->service = ISP_SERVICE_VP;

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_VP,
                            0);

    s->vt_local_encode_count = 0;
    s->vt_local_display_count = 0;
    s->vt_review_remote_count = 0;	
    s->is_first_frame = 1;
    s->b_wait_for_vt_event = 1;
    s->vt_stop_arrived = 0;
    status = SCI_GetEvent(s_isp_event, 
                          ISP_VT_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceStartVT, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1386_112_2_18_1_44_16_585,(uint8*)"d", s->service);
    return ;
	
	
}
void ISP_ServiceStartVTExtScale(void)
{

    uint32	                status,actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartVTExtScale"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1396_112_2_18_1_44_16_586,(uint8*)"");

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_VP_EX_SCALE,
                            0);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SCALE_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    
    return ;        
}
void ISP_ServiceStartVTExt(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32	                status,actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartVTExt"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1416_112_2_18_1_44_16_587,(uint8*)"");
	
    if(ISP_STATE_PREVIEW == s->state)
    {
        ISP_ServiceStopPreview();
    }
    
    s->service = ISP_SERVICE_VP_EX;

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_VP_EX,
                            0);

    s->vt_local_encode_count = 0;
    s->vt_local_display_count = 0;
    s->vt_review_remote_count = 0;	
    s->is_first_frame = 1;
    s->b_wait_for_vt_event = 1;
    s->vt_stop_arrived = 0;
    status = SCI_GetEvent(s_isp_event, 
                          ISP_VT_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

    LCD_SetInvalidateLimit(s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceStartVTExt, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1443_112_2_18_1_44_16_588,(uint8*)"d", s->service);
    return ;
	
	
}
uint32 ISP_ServiceSetVTDecodeAddr(ISP_ADDRESS_T *addr_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32	                status,actual_event;
    ISP_ADDRESS_T           *p_addr;
    	
    if(s->state != ISP_STATE_VP  || SCI_NULL == addr_ptr)
        return SCI_ERROR;		

    if(ISP_YUV_INVALIDE(addr_ptr->yaddr,addr_ptr->uaddr,addr_ptr->vaddr)||
       (addr_ptr->yaddr % ISP_ALIGNED_BYTES) != 0 || 
       (addr_ptr->uaddr % ISP_ALIGNED_BYTES) != 0 )
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1464_112_2_18_1_44_16_589,(uint8*)"ddd",addr_ptr->yaddr,addr_ptr->uaddr,addr_ptr->vaddr);
        return SCI_ERROR;		
    }
        

    if(1 == s->is_first_frame)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:wait for path1 done"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1471_112_2_18_1_44_16_590,(uint8*)"");
        return SCI_SUCCESS;
    }
	
    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceSetVTDecodeAddr"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1475_112_2_18_1_44_16_591,(uint8*)"");

    p_addr = (ISP_ADDRESS_T*)ISP_SERVICE_ALLOC(sizeof(ISP_ADDRESS_T));

    SCI_MEMCPY((void*)p_addr, 
               (void*)addr_ptr, 
               sizeof(ISP_ADDRESS_T));	

    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_VP_PLAYBACK,
                            (void*)p_addr);
    status = SCI_GetEvent(s_isp_event, 
                          ISP_VT_REMOTE_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    return SCI_SUCCESS;	

}
uint32 ISP_ServiceSetVTExtScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_SCLAE_PARAM_T       *p_local_para = SCI_NULL;
    uint32	                status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetVTExtScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1502_112_2_18_1_44_16_592,(uint8*)"");
    //ISP_SERVICE_LOG:"ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1508_112_2_18_1_44_16_593,(uint8*)"ddddd",param_ptr->input_format, param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);
    //ISP_SERVICE_LOG:"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1512_112_2_18_1_44_16_594,(uint8*)"ddd", param_ptr->input_addr.yaddr, param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);
    //ISP_SERVICE_LOG:"ISP_SERVICE:output_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1516_112_2_18_1_44_16_595,(uint8*)"ddd", param_ptr->output_addr.yaddr, param_ptr->output_addr.uaddr,param_ptr->output_addr.vaddr);
    //ISP_SERVICE_LOG:"ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1520_112_2_18_1_44_16_596,(uint8*)"ddd",param_ptr->output_format,param_ptr->output_size.w,param_ptr->output_size.h);

    if(_ISP_ServiceCheckScaleParam(param_ptr))
    {
        return SCI_ERROR;
    }

    p_local_para = (ISP_SCLAE_PARAM_T*)SCI_ALLOC(sizeof(ISP_SCLAE_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_SCLAE_PARAM_T));

    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_VP_EX_SCALE,
                            (void*)p_local_para);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    return status;    
}
uint32 ISP_ServiceSetVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_VT_DECODE_PARAM_T   *p_local_para = SCI_NULL;
    uint32	                status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetVTDecodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1556_112_2_18_1_44_16_597,(uint8*)"");
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1562_112_2_18_1_44_16_598,(uint8*)"dddd",param_ptr->remote_input_range.x,param_ptr->remote_input_range.y,param_ptr->remote_input_range.w,param_ptr->remote_input_range.h);
						
    //ISP_SERVICE_TRACE:"ISP_SERVICE:remote_disp_lcd_id=%d, remote_disp_block_id=%d,remote_disp_range.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1570_112_2_18_1_44_16_599,(uint8*)"dddddd",param_ptr->remote_disp_lcd_id,param_ptr->remote_disp_block_id,param_ptr->remote_disp_range.x, param_ptr->remote_disp_range.y, param_ptr->remote_disp_range.w, param_ptr->remote_disp_range.h);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,uaddr = %d, vaddr = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1576_112_2_18_1_44_16_600,(uint8*)"dddd",param_ptr->deblock_enable,param_ptr->input_addr.yaddr,param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);
	
	
    if(_ISP_ServiceCheckVTDecodeParam(param_ptr))
    {
        return SCI_ERROR;
    }

    p_local_para = (ISP_VT_DECODE_PARAM_T*)ISP_SERVICE_ALLOC(sizeof(ISP_VT_DECODE_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
	
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_VT_DECODE_PARAM_T));
	
    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_VP_DECODE,
                            (void*)p_local_para);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    
    if(status == SCI_SUCCESS)
    {
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1609_112_2_18_1_44_16_601,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }    
    return status;
}
uint32 ISP_ServiceSetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_VT_ENCODE_PARAM_T   *p_local_para = SCI_NULL;
    uint32	                status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetVTEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1623_112_2_18_1_44_16_602,(uint8*)"");
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1629_112_2_18_1_44_16_603,(uint8*)"dddd",param_ptr->local_input_range.x,param_ptr->local_input_range.y,param_ptr->local_input_range.w,param_ptr->local_input_range.h);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d, deci_frames %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1636_112_2_18_1_44_16_604,(uint8*)"ddddd",param_ptr->yuv_type,param_ptr->encode_size.w,param_ptr->encode_size.h,param_ptr->cap_param.cap_frame_skipped,param_ptr->cap_param.cap_frame_decimated);
					
    //ISP_SERVICE_TRACE:"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1644_112_2_18_1_44_16_605,(uint8*)"dddddd",param_ptr->local_disp_lcd_id,param_ptr->local_disp_block_id,param_ptr->local_disp_range.x, param_ptr->local_disp_range.y, param_ptr->local_disp_range.w,param_ptr->local_disp_range.h);
	
	
    if(_ISP_ServiceCheckVTEncodeParam(param_ptr))
    {
        return SCI_ERROR;
    }
    p_local_para = (ISP_VT_ENCODE_PARAM_T*)ISP_SERVICE_ALLOC(sizeof(ISP_VT_ENCODE_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_VT_ENCODE_PARAM_T));	
               
    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_VP_ENCODE,
                            (void*)p_local_para);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

    if(status == SCI_SUCCESS)
    {
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1675_112_2_18_1_44_16_606,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }
      
    return status;
}
uint32 ISP_ServiceSetVTExtEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_VT_ENCODE_PARAM_T   *p_local_para = SCI_NULL;
    uint32	                status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/
	
    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceSetVTExtEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1690_112_2_18_1_44_16_607,(uint8*)"");
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1696_112_2_18_1_44_16_608,(uint8*)"dddd",param_ptr->local_input_range.x,param_ptr->local_input_range.y,param_ptr->local_input_range.w,param_ptr->local_input_range.h);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1701_112_2_18_1_44_16_609,(uint8*)"ddd",param_ptr->yuv_type,param_ptr->encode_size.w,param_ptr->encode_size.h);
					
    //ISP_SERVICE_TRACE:"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1709_112_2_18_1_44_16_610,(uint8*)"dddddd",param_ptr->local_disp_lcd_id,param_ptr->local_disp_block_id,param_ptr->local_disp_range.x, param_ptr->local_disp_range.y, param_ptr->local_disp_range.w,param_ptr->local_disp_range.h);
	
    if(_ISP_ServiceCheckVTExtEncodeParam(param_ptr))
    {
        return SCI_ERROR;
    }
    p_local_para = (ISP_VT_ENCODE_PARAM_T*)SCI_ALLOC(sizeof(ISP_VT_ENCODE_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para, 
               (void*)param_ptr, 
               sizeof(ISP_VT_ENCODE_PARAM_T));	
               
    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_VP_EX_ENCODE,
                            (void*)p_local_para);

    status = SCI_GetEvent(s_isp_event, 
                          ISP_SET_PARAM_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
    
    if(status == SCI_SUCCESS)
    {
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1739_112_2_18_1_44_17_611,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }
      
    return status;
}
uint32 ISP_ServiceGetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    
    if(!param_ptr)
    {
        return SCI_ERROR;
    }
    
    if(s->service != ISP_SERVICE_VP &&
       s->service != ISP_SERVICE_VP_EX)
    {
        //ISP_SERVICE_LOG:"ISP_ServiceGetVTEncodeParam, Not in VT mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1758_112_2_18_1_44_17_612,(uint8*)"");
        return SCI_ERROR;        
    }


    SCI_MEMCPY((void*)&(param_ptr->local_input_size), 
               (void*)&(s->cap_input_size),
               sizeof(ISP_SIZE_T));
    
    SCI_MEMCPY((void*)&(param_ptr->local_input_range), 
               (void*)&(s->cap_input_range),
               sizeof(ISP_RECT_T));
        
    if(s->enc_rotation == ISP_ROTATION_90 || 
       s->enc_rotation == ISP_ROTATION_270)
    {
        param_ptr->encode_size.h = s->encoder_size.w;
        param_ptr->encode_size.w = s->encoder_size.h;
    }
    else
    {
        param_ptr->encode_size.w = s->encoder_size.w;
        param_ptr->encode_size.h = s->encoder_size.h;
    } 
    
    SCI_MEMCPY((void*)&(param_ptr->local_disp_range), 
               (void*)&(s->display_range), 
               sizeof(ISP_RECT_T));
    
    if(s->display_rotation == ISP_ROTATION_90 ||
       s->display_rotation == ISP_ROTATION_270) // width and height switched in BlockConfig,here just recorver
    {
        param_ptr->local_disp_range.w = s->display_range.h;    
        param_ptr->local_disp_range.h = s->display_range.w;            
    }

    SCI_MEMCPY((void*)&(param_ptr->lcd_rect), 
               (void*)&(s->display_block.lcd_inv_rect), 
               sizeof(ISP_RECT_T));
    
    param_ptr->local_display_enable = s->local_display_enable;
    param_ptr->local_review_enable = s->local_review_enable;    
    
    param_ptr->local_disp_lcd_id = s->display_block.lcd_id;    
    param_ptr->local_disp_block_id = s->display_block.block_id;
	
    param_ptr->disp_rotation = s->display_rotation;
    param_ptr->encode_rotation = s->enc_rotation;
    param_ptr->cap_param.cap_img_format = s->cap_input_image_format;
    param_ptr->cap_param.cap_img_pattern = s->cap_input_image_patten;
    param_ptr->cap_param.cap_vsync_pol = s->vsync_polarity;
    param_ptr->cap_param.cap_hsync_pol = s->hsync_polarity;
    param_ptr->cap_param.cap_pclk_pol = s->pclk_polarity;
    param_ptr->cap_param.cap_frame_skipped = s->preview_skip_frame_num;
    param_ptr->cap_param.cap_frame_decimated = s->preview_deci_frame_num;

    return SCI_SUCCESS;
    
}

void ISP_ServiceStopVT(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = SCI_SUCCESS;
    uint32                  actual_event;
    uint32                  local_review_en = s->local_review_enable;
    uint32                  local_display_en = s->local_display_enable;
    uint32                  remote_review_en = s->remote_review_enable;
    uint32                  remote_display_en = s->remote_display_enable;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStopVT"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1828_112_2_18_1_44_17_613,(uint8*)"");

    if(s->state != ISP_STATE_VP)
    {
        return ;
    }
    s->vt_stop_arrived = 1;
    s->local_review_enable = 0;
    s->remote_review_enable = 0;
    s->local_display_enable = 0;
    s->remote_display_enable = 0;

	_ISP_ServiceCloseTimer();
    // _ISP_ServiceFlushQueue();
    
    _ISP_ServiceEnterVTReview();
    ISP_DriverStop(s->module_addr);
    _ISP_ServiceExitVTReview();	    
    
    _ISP_ServiceSendRequest(0,ISP_SERVICE_STOP_SIG,0,0);
   
    status = SCI_GetEvent(s_isp_event, 
                       	  ISP_STOP_EVENT, 
                       	  SCI_AND_CLEAR, 
                       	  &actual_event, 
                       	  ISP_WAIT_TIME);
    s->local_review_enable = local_review_en;
    s->local_display_enable = local_display_en;
    s->remote_review_enable = remote_review_en;
    s->remote_display_enable = remote_display_en;

	
	_ISP_ServiceCloseBlock();
	
    _ISP_ServiceEncodeFrameDeInit();    
    
    LCD_SetInvalidateLimit(s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceStopVT, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1865_112_2_18_1_44_17_614,(uint8*)"d", s->service);
    return ;

}

void ISP_ServiceStopVTExt(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = SCI_SUCCESS;
    uint32                  actual_event;
    uint32                  local_review_en = s->local_review_enable;
    uint32                  local_display_en = s->local_display_enable;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStopVTExt"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1878_112_2_18_1_44_17_615,(uint8*)"");

    if(s->state != ISP_STATE_VP)
    {
        return ;
    }
    s->vt_stop_arrived = 1;
    s->local_review_enable = 0;
    s->local_display_enable = 0;

	_ISP_ServiceCloseTimer();
    //_ISP_ServiceFlushQueue();
    
    _ISP_ServiceEnterVTReview();
    ISP_DriverStop(s->module_addr);
    _ISP_ServiceExitVTReview();	    
    
    _ISP_ServiceSendRequest(0,ISP_SERVICE_STOP_SIG,0,0);
   
    status = SCI_GetEvent(s_isp_event, 
                       	  ISP_STOP_EVENT, 
                       	  SCI_AND_CLEAR, 
                       	  &actual_event, 
                       	  ISP_WAIT_TIME);
    s->local_review_enable = local_review_en;
    s->local_display_enable = local_display_en;
	
	// _ISP_ServiceCloseBlock();
    s->display_block.is_enable = 0;
	
    LCD_SetInvalidateLimit(s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceStopVTExt, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_1909_112_2_18_1_44_17_616,(uint8*)"d", s->service);
    return ;

}
 




LOCAL void _ISP_ServiceCreate(void)
{
    uint32                 status = SCI_SUCCESS;
    uint32                 thread_pori =  PRI_ISP_SERVICE_TASK;
    ISP_SERVICE_T          *s = &s_isp_service;

    
    status = SCI_CreateStaticThread(ISP_SERVICE_THREAD,
                                    "TASK_ISP_SERVICE", 
                                    _ISP_ServiceMainMessageLoop,
                                    0,
                                    0,
                                    _ISP_ServiceStack,
                                    ISP_STACK_SIZE,
                                    thread_pori, 
                                    SCI_PREEMPT, 
                                    "Q_ISP",
                                    _ISP_ServiceQueue,
                                    ISP_QUEUE_NUM,
                                    SCI_AUTO_START);

    if (SCI_SUCCESS != status )
    {
        SCI_ASSERT(0);   /*assert verified*/ 
    }

    s_isp_service_thread_id = ISP_SERVICE_THREAD;
	
    if (SCI_NULL == s_isp_event)
    {
        s_isp_event = SCI_CreateEvent("ISP_EVENT"); 
    }

    if(SCI_NULL == s_isp_service_sema)
    {
        s_isp_service_sema = (SCI_SEMAPHORE_PTR)SCI_CreateSemaphore("Semaphore for isp service", 1);    
    }
    s_b_wait_for_opened = 0;
    
    s->state = ISP_STATE_CLOSED;
    s->service = ISP_SERVICE_IDLE;


    return ;
	
}
LOCAL void _ISP_ServiceFlushQueue(void)
{
    ISP_SIGNAL_T     *sig_ptr;    
	
    SCI_SuspendThread(ISP_SERVICE_THREAD);
	
    while (SCI_TRUE)/*lint !e716*/
    {
        sig_ptr = (void*)SCI_PeekSignal(ISP_SERVICE_THREAD);
        if (sig_ptr != PNULL)
        {
            if((sig_ptr->sig.SignalCode & ISP_SERVICE_PARA_FREE) == ISP_SERVICE_PARA_FREE)
            {
                ISP_SERVICE_FREE(sig_ptr->ptr);
            }
            SCI_FREE (sig_ptr);
        }
        else
        {
            break;
        }
    }   
	
    SCI_ResumeThread(ISP_SERVICE_THREAD);

    return ;
	
	
}
LOCAL void _ISP_ServiceMainMessageLoop(uint32 argc, void *argv)
{
    ISP_SIGNAL_T            *sig_ptr = SCI_NULL;
	
    while (TRUE)/*lint !e716*/
    {
        sig_ptr = (ISP_SIGNAL_T *)SCI_GetSignal(ISP_SERVICE_THREAD);
        SCI_ASSERT (sig_ptr);/*assert verified*/
			
        //ISP_SERVICE_TRACE:"ISP_Service: _ISP_ServiceMainMessageLoop,command=0x%x param=0x%x SignalCode 0x%x sig_ptr->ptr 0x%x time %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2007_112_2_18_1_44_17_617,(uint8*)"ddddd",sig_ptr->command , sig_ptr->param,sig_ptr->sig.SignalCode,sig_ptr->ptr,SCI_GetTickCount());

        switch(sig_ptr->command)
        {
            case ISP_SERVICE_OPEN_SIG:
            
                _ISP_ServiceOpenSignalHandler(sig_ptr->sig.SignalCode, 
                                              sig_ptr->param, 
                                              sig_ptr->ptr);
            
            break;
            
            case ISP_SERVICE_CLOSE_SIG:
			
                _ISP_ServiceCloseSignalHandler(sig_ptr->sig.SignalCode, 
                                               sig_ptr->param, 
                                               sig_ptr->ptr);
            
            break;
			
            case ISP_SERVICE_PATH1_SIG:
 
                _ISP_ServicePath1SignalHandler(sig_ptr->sig.SignalCode, 
                                               sig_ptr->param, 
                                               sig_ptr->ptr);
            break;
            
            case ISP_SERVICE_PATH2_SIG:
            
                _ISP_ServicePath2SignalHandler(sig_ptr->sig.SignalCode, 
                                               sig_ptr->param, 
                                               sig_ptr->ptr);
                                               
            break;
            
            case ISP_SERVICE_SET_PARAM_SIG:
            
                _ISP_ServiceSetParamSignalHandler(sig_ptr->sig.SignalCode, 
                                                  sig_ptr->param, 
                                                  sig_ptr->ptr);	
                
            break;
            
            case ISP_SERVICE_START_SIG:
            
                _ISP_ServiceStartSignalHandler(sig_ptr->sig.SignalCode, 
                                               sig_ptr->param, 
                                               sig_ptr->ptr);	
                                               
            break;
            
            case ISP_SERVICE_STOP_SIG:
           
                _ISP_ServiceStopSignalHandler(sig_ptr->sig.SignalCode, 
                                              sig_ptr->param, 
                                              sig_ptr->ptr);	
                
            break;	
            
            default:
            
            break;
        }

        if((sig_ptr->sig.SignalCode & ISP_SERVICE_PARA_FREE) == ISP_SERVICE_PARA_FREE)
        {
            ISP_SERVICE_FREE(sig_ptr->ptr);
        }
        ISP_SERVICE_FREE(sig_ptr);
    }
      
}

LOCAL void _ISP_ServiceOpenSignalHandler(uint32 service,uint32 param,void * ptr)
{
	
    _ISP_ServiceInit();

    ISP_DriverIramSwitch(AHB_REG_BASE,IRAM_FOR_ISP); // swicth IRAM to ISP
	
    ISP_DriverModuleEnable(AHB_REG_BASE);
    
    DPSLP_ISP_EnableAhbSleep(DISABLE_AHB_SLEEP);   
    
    SCI_SetEvent(s_isp_event, ISP_OPEN_EVENT, SCI_OR);

    return ;
}

LOCAL void _ISP_ServiceCloseSignalHandler(uint32 service,uint32 param,void * ptr)
{
  
    ISP_DriverModuleDisable(AHB_REG_BASE);
    
    ISP_DriverIramSwitch(AHB_REG_BASE,IRAM_FOR_ARM); // swicth IRAM to ARM
   	
    DPSLP_ISP_EnableAhbSleep(ENABLE_AHB_SLEEP);

    _ISP_ServiceDeInit();// event, mutex, semaphore will be released here

    SCI_SetEvent(s_isp_event, ISP_CLOSE_EVENT, SCI_OR);
    
    return ;

}

LOCAL void _ISP_ServicePath1SignalHandler(uint32 service,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_frame = (ISP_FRAME_T*)ptr;
    uint32                  status = 0;
    ISP_ADDRESS_T           isp_addr = {0};
    ISP_FRAME_T             isp_frame_t = {0};
    uint32                  frame_size = 0; 
   
    switch(s->service)
    {
        case ISP_SERVICE_PREVIEW:
        case ISP_SERVICE_MPEG:
			
            //ISP_SERVICE_TRACE:"ISP_SERVICE: display one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2131_112_2_18_1_44_17_618,(uint8*)"dddd",p_frame,            p_frame->yaddr,p_frame->uaddr,p_frame->vaddr);
            _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
            _ISP_ServiceCallUserDispCB((void*)p_frame);
            
            
            if(s->is_first_frame)
            {
                status = SCI_SetEvent(s_isp_event, ISP_DISPLAY_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
                s->is_first_frame = 0;			   
            }
            else
            {
                ISP_DriverFrameUnLock(p_frame->prev);
            }
			
            break;
			
        case ISP_SERVICE_VP:

            //ISP_SERVICE_TRACE:"ISP_SERVICE: vp get one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2155_112_2_18_1_44_17_619,(uint8*)"dddd",p_frame,            p_frame->yaddr,p_frame->uaddr,p_frame->vaddr);

            if(s->vt_stop_arrived)
            {
                ISP_DriverFrameUnLock(p_frame);         
                //ISP_SERVICE_TRACE:"ISP_SERVICE: VT Stopped, don't do local review"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2160_112_2_18_1_44_17_620,(uint8*)"");
            }
            else
            {
                if(!ISP_DriverFrameIsLocked(s->p_frame_enc_cur)) // is the current frame has not been released, drop this encode frame
                {
                    if(SCI_SUCCESS == _ISP_ServiceEnterVTReview())
                    {
                        isp_addr.yaddr = p_frame->yaddr;
                        isp_addr.uaddr = p_frame->uaddr;
                        isp_addr.vaddr = p_frame->vaddr;
                        //ISP_SERVICE_LOG:"ISP_SERVICE:transform the local frame from yuv422 to yuv420"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2171_112_2_18_1_44_17_621,(uint8*)"");
                        _ISP_ServiceSetVPFlag(VP_REVIEW_LOCAL_422TO420);
                        _ISP_ServiceStartVTReview(&isp_addr);
                    }
                    else
                    {
                        //ISP_SERVICE_TRACE:"ISP_SERVICE: timeout, couldn't convert yuv422 to yuv420"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2177_112_2_18_1_44_17_622,(uint8*)"");
                    }
                }
                else
                {
                    // do local reviewing
                    _ISP_ServiceStartVTLocalPlayBack(p_frame);
                }
            }
            
            if(s->is_first_frame)
            {
                s->display_frame_locked_ptr = p_frame;
                _ISP_ServiceStartTimer(ISP_SERVICE_PATH1_TIMEOUT,ISP_SERVICE_TIMEOUT);

                if(s->b_wait_for_vt_event)
                {
                    status = SCI_SetEvent(s_isp_event, ISP_VT_EVENT, SCI_OR);
                    SCI_ASSERT(!status); /*assert verified*/
                    s->b_wait_for_vt_event = 0;						
                }
                s->is_first_frame = 0;			   
            }
				
        break;

        case ISP_SERVICE_VP_EX:
            
            //ISP_SERVICE_TRACE:"ISP_SERVICE: vp get one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2209_112_2_18_1_44_18_623,(uint8*)"dddd",p_frame,            p_frame->yaddr,p_frame->uaddr,p_frame->vaddr);
            if(0 == s->vt_stop_arrived)
            {
             //   ISP_DriverFrameLock(p_frame);
                s->vt_local_encode_count ++;
                p_frame->type = ISP_DATA_YUV422;
                _ISP_ServiceCallUserEncCB((void*)p_frame);
            }
            
            if(s->local_review_enable)
            {
                if(SCI_SUCCESS == _ISP_ServiceEnterVTReview())
                {
                    //ISP_SERVICE_LOG:"ISP_SERVICE:transform the local frame from yuv422 to rgb565"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2222_112_2_18_1_44_18_624,(uint8*)"");
                    isp_frame_t.width = s->display_range.w;
                    isp_frame_t.height = s->display_range.h;
                    isp_frame_t.flags = VP_EX_LOCAL_REVIEW;
                    isp_frame_t.type = ISP_DATA_RGB565;
                    isp_frame_t.yaddr = s->display_addr[s->vt_local_display_count % ISP_PATH1_FRAME_COUNT_MAX].yaddr;
                    isp_frame_t.uaddr = s->display_addr[s->vt_local_display_count % ISP_PATH1_FRAME_COUNT_MAX].uaddr;
                    p_frame->type = ISP_DATA_YUV422;
                    _ISP_ServiceStartVTScale(p_frame, SCI_NULL, &isp_frame_t);
                    s->vt_local_display_count++;
                }
                else
                {
                    //ISP_SERVICE_TRACE:"ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2235_112_2_18_1_44_18_625,(uint8*)"");
                }                
            }
            else
            {
                if(s->is_first_frame)
                {
                    if(s->b_wait_for_vt_event)
                    {
                        status = SCI_SetEvent(s_isp_event, ISP_VT_EVENT, SCI_OR);
                        SCI_ASSERT(!status); /*assert verified*/
                        s->b_wait_for_vt_event = 0;						
                    }
                    s->is_first_frame = 0;			   
                }            
            }            

        break;
        case ISP_SERVICE_JPEG:

            if(_ISP_ServiceGetCompoundJpegFlag())
            {
                ISP_DriverStop(s->module_addr);
                _ISP_ServiceGetImageFromLCDC(&isp_frame_t);    
                s->input_addr.yaddr = isp_frame_t.yaddr;
                s->input_addr.uaddr = isp_frame_t.uaddr;
                s->input_addr.vaddr = isp_frame_t.vaddr;	
                frame_size = isp_frame_t.width * isp_frame_t.height;
                frame_size += ISP_MEM_STRIDER_FOR_ONE_FRAME;
                s->encoder_addr[0].uaddr = s->input_addr.yaddr - frame_size;
                s->encoder_addr[0].uaddr = ISP_ADDRESS_ALIGNED_(s->encoder_addr[0].uaddr,
                                                            ISP_ALIGNED_BYTES);   
                s->encoder_addr[0].yaddr = s->encoder_addr[0].uaddr - frame_size;
                s->encoder_addr[0].yaddr = ISP_ADDRESS_ALIGNED_(s->encoder_addr[0].yaddr,
                                                            ISP_ALIGNED_BYTES);   

                s->path2_input_image_format = ISP_IMAGE_FORMAT_RGB565;
                s->path2_output_image_format = ISP_IMAGE_FORMAT_YUV422;		
                s->service =  ISP_SERVICE_SCALE;               				
                _ISP_ServiceStartScale();
                s->state = ISP_STATE_CAPTURE_SCALE;
                _ISP_ServiceSetCompoundJpegFlag(SCI_FALSE);
            }
            else
            {
                if(ISP_STATE_CAPTURE_DONE == s_isp_service.state)
                {
                    isp_addr.yaddr = p_frame->yaddr;
                    if(s->cap_input_image_format != ISP_IMAGE_FORMAT_JPEG)
                    {
                        isp_addr.uaddr = p_frame->uaddr;
                        isp_addr.vaddr = p_frame->vaddr;
                    }
                    else
                    {
                        ISP_DriverCapGetInfo(s->module_addr, ISP_CAP_JPEG_GET_LENGTH, (void *)&isp_addr.uaddr);
                    }

                    _ISP_ServiceCallUserEncCB((void*)&isp_addr);           
                }
                else
                {
                    s->input_addr.yaddr = p_frame->yaddr;
                    s->input_addr.uaddr = p_frame->uaddr;
                    s->input_addr.vaddr = p_frame->vaddr;		
                    s->path2_input_image_format = ISP_IMAGE_FORMAT_YUV422;
                    s->path2_output_image_format = ISP_IMAGE_FORMAT_YUV422;		
                    s->service =  ISP_SERVICE_SCALE;           
                    s->total_slice_count = 0;
                    s->total_scale_out = 0;   
                    if(s->encoder_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH) //should use slice mode
                    {
                        s->is_slice = SCI_TRUE;
                        s->slice_height = ISP_SCALE_SLICE_HEIGHT;
                        if(s->slice_height == s->input_range.h)
                        {
                            s->slice_out_height = s->encoder_size.h;
                        }
                        else
                        {
                            s->slice_out_height = s->slice_height * s->encoder_size.h / s->input_range.h;
                            s->slice_out_height = ISP_SLICE_OUT_ESTIMATE(s->slice_out_height);
                        }
                        s->total_slice_count = 0;
                    }
                    else
                    {
                        s->is_slice = SCI_FALSE;
                    } 
                    _ISP_ServiceStartScale();
                    s->state = ISP_STATE_CAPTURE_SCALE;
				
                }
            }
			
        break;
            
        default:
        
        break;
    }
}

LOCAL void _ISP_ServicePath2SignalHandler(uint32 service,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_frame = (ISP_FRAME_T*)ptr;
    uint32                  status = 0;	
    ISP_USER_FUNC_PTR       user_cb = SCI_NULL;
    ISP_ADDRESS_T           isp_src_addr = {0};    
    ISP_ADDRESS_T           isp_addr = {0};
    uint32                  rot_angle = ROTATION_DIR_MAX;
    ISP_SIZE_T              isp_size = {0};
    
    switch(s_isp_service.service)
    {
        case ISP_SERVICE_MPEG:

			if(s->enc_stoped == 0)
			{
                if(s->enc_rotation != ISP_ROTATION_0)// need rotation
                {
                    if(!ISP_DriverFrameIsLocked(s->p_frame_enc_cur)) // is the current frame has not been released, drop this encode frame
                    {
                        rot_angle = _ISP_ServiceRotAngle2DrvAngle(s->enc_rotation);
                    
                        isp_addr.yaddr = s->p_frame_enc_cur->yaddr;
                        isp_addr.uaddr = s->p_frame_enc_cur->uaddr;
                        isp_src_addr.yaddr = p_frame->yaddr;
                        isp_src_addr.uaddr = p_frame->uaddr;                        
                        _ISP_ServiceRotateImage(&isp_src_addr,
                                                &isp_addr,
                                                s->encoder_size.w,
                                                s->encoder_size.h,
                                                rot_angle,
                                                s->path2_output_image_format);
                       if(s->enc_rotation == ISP_ROTATION_90 || 
                          s->enc_rotation == ISP_ROTATION_270)
                       {
                           s->p_frame_enc_cur->width = s->encoder_size.h;
                           s->p_frame_enc_cur->height = s->encoder_size.w;
                       }
                       else
                       {
                           s->p_frame_enc_cur->width = s->encoder_size.w;
                           s->p_frame_enc_cur->height = s->encoder_size.h;
                        
                       }    
                       ISP_DriverFrameUnLock(p_frame);
                       ISP_DriverFrameLock(s->p_frame_enc_cur);// unlocked by user    
                       _ISP_ServiceCallUserEncCB((void*)s->p_frame_enc_cur);  
                       s->p_frame_enc_cur = s->p_frame_enc_cur->next;
                    }
                    else
                    {
                        ISP_DriverFrameUnLock(p_frame);
                        //ISP_SERVICE_TRACE:"ISP_SERVICE: Encode frame not released by user, drop this frame"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2391_112_2_18_1_44_18_626,(uint8*)"");
                    }
                    
                }
                else
                {
            	    _ISP_ServiceCallUserEncCB((void*)p_frame);  // unlocked by user         
                }
			}

            if(s->is_enc_first_frame)
            {	
                status = SCI_SetEvent(s_isp_event, ISP_MPEG_START_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
                s->is_enc_first_frame = 0;
            }

        break;
        case ISP_SERVICE_VP_EX:

			//ISP_SERVICE_LOG:"ISP_SERVICE: flag %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2411_112_2_18_1_44_18_627,(uint8*)"d",p_frame->flags);
            if(VP_EX_LOCAL_REVIEW == p_frame->flags)
            {
                if(s->local_display_enable)                
                {
                    _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);    
                }

                if(s->is_first_frame)
                {
                    if(s->b_wait_for_vt_event)
                    {
                        status = SCI_SetEvent(s_isp_event, ISP_VT_EVENT, SCI_OR);
                        SCI_ASSERT(!status); /*assert verified*/
                        s->b_wait_for_vt_event = 0;						
                    }
                    s->is_first_frame = 0;			   
                }            
                
            }
            else if(VP_EX_REMOTE_SCALE == p_frame->flags)
            { 
                isp_addr.yaddr = p_frame->yaddr;
                isp_addr.uaddr = p_frame->uaddr;
                isp_addr.vaddr = p_frame->vaddr;
                p_frame->type = s->path2_output_image_format;

#ifdef CHIP_ENDIAN_LITTLE
                if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565_EX)
                {
                    _ISP_ServiceRGB565FromLittleToHalfBig(p_frame->yaddr, (uint32)(p_frame->width * p_frame->height));
                }
#endif   
                
                (*s->scale_user_cb)(isp_addr, p_frame->width, p_frame->height);      

                status = SCI_SetEvent(s_isp_event, ISP_SCALE_EVENT, SCI_OR);
                SCI_ASSERT(!status);  /*assert verified*/             
                
            }
            else
            {
                //ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported frame flags"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2453_112_2_18_1_44_18_628,(uint8*)"");
            }
                
        break;
		
        case ISP_SERVICE_VP:

				//ISP_SERVICE_TRACE:"ISP_SERVICE: flag %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2460_112_2_18_1_44_18_629,(uint8*)"d",p_frame->flags);
				
                if(VP_REVIEW_LOCAL_422TO420 == p_frame->flags)
                {
                    //ISP_SERVICE_TRACE:"ISP_SERVICE:vp transform to yuv420 done,p_frame = 0x%x,y,u,v {0x%x,0x%x,0x%x}"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2468_112_2_18_1_44_18_630,(uint8*)"dddd",p_frame,            p_frame->yaddr,p_frame->uaddr,p_frame->vaddr);
                    
                    if(0 == s->vt_stop_arrived)          
                    {
                        ISP_DriverFrameLock(s->p_frame_enc_cur);
                        if(s->enc_rotation != ISP_ROTATION_0)	
                        {
                            rot_angle = _ISP_ServiceRotAngle2DrvAngle(s->enc_rotation);
                        
                            isp_addr.yaddr = s->p_frame_enc_cur->yaddr;
                            isp_addr.uaddr = s->p_frame_enc_cur->uaddr;
                            _ISP_ServiceRotateImage(&s->encoder_rot_addr[0],
                                                    &isp_addr,
                                                    s->encoder_size.w,
                                                    s->encoder_size.h,
                                                    rot_angle,
                                                    ISP_IMAGE_FORMAT_YUV420);
                           if(s->enc_rotation == ISP_ROTATION_90 || 
                              s->enc_rotation == ISP_ROTATION_270)
                           {
                               s->p_frame_enc_cur->width = s->encoder_size.h;
                               s->p_frame_enc_cur->height = s->encoder_size.w;
                           }
                           else
                           {
                               s->p_frame_enc_cur->width = s->encoder_size.w;
                               s->p_frame_enc_cur->height = s->encoder_size.h;
                            
                           }
                        }
                        _ISP_ServiceCallUserEncCB((void*)s->p_frame_enc_cur);  
                        s->p_frame_enc_cur = s->p_frame_enc_cur->next;
          				//ISP_SERVICE_LOG:"ISP_SERVICE: encode frame switched to next one 0x%x"
          				SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2500_112_2_18_1_44_18_631,(uint8*)"d",s->p_frame_enc_cur);

                    }

                    if(s->local_review_enable)
                    {
                        _ISP_ServiceStartVTLocalPlayBack(s->display_frame_locked_ptr);
                    }
                    else
                    {
                        ISP_DriverFrameUnLock(s->display_frame_locked_ptr);
                        s->display_frame_locked_ptr = s->display_frame_locked_ptr->next;
                    }
					
                }
                else if(VP_REVIEW_LOCAL_422TORGB == p_frame->flags)
                {
                    if(s->local_display_enable)
                    {                    
                        _ISP_ServiceCallUserDispCB((void*)p_frame);
                        _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);    
                    }
                    ISP_DriverFrameUnLock(s->display_frame_locked_ptr);
                    s->display_frame_locked_ptr = s->display_frame_locked_ptr->next;
                    
                }
                else if(VP_REVIEW_REMOTE_420TORGB == p_frame->flags) 
                {
      				//ISP_SERVICE_LOG:"ISP_SERVICE: remote display %d"
      				SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2528_112_2_18_1_44_18_632,(uint8*)"d",s->remote_display_enable);

                    status = SCI_SetEvent(s_isp_event,ISP_VT_REMOTE_EVENT,SCI_OR);
                    SCI_ASSERT(!status); /*assert verified*/
                    _ISP_ServiceCallUserDispCB((void*)p_frame);
                    if(s->remote_display_enable)
                    {
                        _ISP_ServiceDisplayOneFrame(s->remote_display_block.block_id,p_frame);    
                    }
                }
                else
                {
                    //ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported frame flags"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2540_112_2_18_1_44_18_633,(uint8*)"");
                }
				
        break;

        case ISP_SERVICE_REVIEW:

            _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
            
            if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565)
            {
                _ISP_ServiceRGB565FromLittleToHalfBig(p_frame->yaddr, 
                                                     (uint32)(p_frame->width*p_frame->height));
            }

            if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565)
            {
                _ISP_ServiceRGB565FromLittleToHalfBig(p_frame->yaddr, 
                                                                                    (uint32)(p_frame->width*p_frame->height));
            }

            _ISP_ServiceGetDispCtrl();
            user_cb = s->display_user_cb;
            _ISP_ServiceRelDispCtrl();

            if(s->is_first_frame)
            {
                s->is_first_frame = 0;
            }
			
            if(SCI_NULL != user_cb)
            {
                (*user_cb)(p_frame);
            }
            else
            {
                status = SCI_SetEvent(s_isp_event, ISP_DISPLAY_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
            }
            

        break;
		
        case ISP_SERVICE_SCALE:

                if(s->state == ISP_STATE_CAPTURE_SCALE)
                {
                    if(s->is_slice)
                    {
                        if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565)
                        {
                            SCI_MEMCPY((void*)(s->output_addr.yaddr + (uint32)(s->total_scale_out*p_frame->width*2)),
                                       (void*)p_frame->yaddr,
                                       (uint32)((p_frame->height - s->total_scale_out)*p_frame->width*2));
                        }
                        else
                        {
                            SCI_MEMCPY((void*)(s->output_addr.yaddr + (uint32)(s->total_scale_out*p_frame->width)),
                                       (void*)p_frame->yaddr,
                                       (uint32)((p_frame->height - s->total_scale_out)*p_frame->width));
                            SCI_MEMCPY((void*)(s->output_addr.uaddr + (uint32)(s->total_scale_out*p_frame->width)),
                                       (void*)p_frame->uaddr,
                                       (uint32)((p_frame->height - s->total_scale_out)*p_frame->width));                        
                        }
                        s->total_scale_out = p_frame->height;
                        if(s->total_scale_out  < s->encoder_size.h) // there is other slice  to be done
                        {
                            _ISP_ServiceContinueScaling();
                            s->state = ISP_STATE_CAPTURE_SCALE;
                        }
                        else // all slice scale(include normal scale) done
                        {
                            isp_addr.yaddr = s->output_addr.yaddr;
                            isp_addr.uaddr = s->output_addr.uaddr;
                            
                            _ISP_ServiceCallUserEncCB((void*)&isp_addr);           
                        }
                    }
                    else
                    {
                            isp_addr.yaddr = p_frame->yaddr;
                            isp_addr.uaddr = p_frame->uaddr;
                            _ISP_ServiceCallUserEncCB((void*)&isp_addr);                             
                    }
                    
                }
                else
                {
                    if(s->scale_user_cb)
                    {
                        isp_addr.yaddr = p_frame->yaddr;
                        isp_addr.uaddr = p_frame->uaddr;
                        isp_addr.vaddr = p_frame->vaddr;

                        _ISP_ServiceSyncImageData(&isp_addr,
                                                  p_frame->width,
                                                  (uint32)(p_frame->height - s->total_scale_out),
                                                  s->path2_output_image_format,
                                                  DMABUFFER_FROM_DEVICE);
                        
#ifdef CHIP_ENDIAN_LITTLE
                        if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565_EX)
                        {
                            _ISP_ServiceRGB565FromLittleToHalfBig(p_frame->yaddr, 
                               (uint32)(p_frame->width * (p_frame->height - s->total_scale_out)));
                         }
                        
                        if(s->rgb_ex_flag)
                        {
                            if(s->path2_input_image_format == ISP_IMAGE_FORMAT_RGB565)
                            {
                        
                                _ISP_ServiceRGB565FromLittleToHalfBig(s->local_addr[0].yaddr, 
                                    (uint32)(s->rgb_ex_height * s->input_size.w));
                            }
                            else if(s->path2_input_image_format == ISP_IMAGE_FORMAT_RGB888)
                            {
                                _ISP_ServiceRGB888FromBigToLittle(s->local_addr[0].yaddr, 
                                                                  (uint32)(s->rgb_ex_height * s->input_size.w));
                                                        
                            }
                            s->rgb_ex_flag = SCI_FALSE;
                        }

                        
#endif    
                        isp_size.w = p_frame->width;
                        isp_size.h = p_frame->height - s->total_scale_out;
                        _ISP_ServiceSyncMem2Output(&isp_addr, &s->output_addr,&isp_size,s->path2_output_image_format);
                        (*s->scale_user_cb)(isp_addr, p_frame->width, isp_size.h);
                    }

                    if(s->is_slice && s->is_slice_alone == SCI_FALSE)
                    {
                        if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565)
                        {
                            SCI_MEMCPY((void*)(s->output_addr.yaddr + (uint32)(s->total_scale_out*p_frame->width*2)),
                                       (void*)p_frame->yaddr,
                                       (uint32)((p_frame->height - s->total_scale_out)*p_frame->width*2));
                        }
                        else
                        {
                            SCI_MEMCPY((void*)(s->output_addr.yaddr + (uint32)(s->total_scale_out*p_frame->width)),
                                       (void*)p_frame->yaddr,
                                       (uint32)((p_frame->height - s->total_scale_out)*p_frame->width));
                            SCI_MEMCPY((void*)(s->output_addr.uaddr + (uint32)(s->total_scale_out*p_frame->width)),
                                       (void*)p_frame->uaddr,
                                       (uint32)((p_frame->height - s->total_scale_out)*p_frame->width));                        
                        }				
                    }
                    s->total_scale_out = p_frame->height;
                    
                    if(s->is_slice_alone != SCI_TRUE &&
                       s->total_scale_out  < s->encoder_size.h)// there is other slice  to be done
                    {
                        _ISP_ServiceContinueScaling();
                    }
                    else // all slice scale(include normal scale) done
                    {
                        if(s->total_scale_out  >= s->encoder_size.h)
                        {
                            //ISP_SERVICE_LOG:"ISP_SERVICE: s->total_scale_out %d s->encoder_size.h %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2703_112_2_18_1_44_19_634,(uint8*)"dd",s->total_scale_out,s->encoder_size.h);
                            s->is_slice_alone = SCI_FALSE;
                            s->state = ISP_STATE_IDLETEMP;
                        }
                        status = SCI_SetEvent(s_isp_event, ISP_SCALE_EVENT, SCI_OR);
                        SCI_ASSERT(!status); /*assert verified*/
                    }
                }
        break;			

        case ISP_SERVICE_PREVIEW:

            //ISP_SERVICE_TRACE:"ISP_SERVICE: display one frame ,y,u,v {0x%x,0x%x,0x%x}"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_2718_112_2_18_1_44_19_635,(uint8*)"ddd",p_frame->yaddr,p_frame->uaddr,p_frame->vaddr);
            
            _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
            _ISP_ServiceCallUserDispCB((void*)p_frame);
            if(s->is_first_frame)
            {
                status = SCI_SetEvent(s_isp_event, ISP_DISPLAY_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
                s->is_first_frame = 0;			   
            }
            else
            {
                ISP_DriverFrameUnLock(p_frame->prev);                
            }

        break;		

        default:

        break;
    }
}

LOCAL void _ISP_ServiceScaleSignalHandler(uint32 service,uint32 param,void * ptr)
{

}

LOCAL void _ISP_ServiceSetParamSignalHandler(uint32 service,uint32 param,void * ptr)
{
    uint32                  status = 0;	
    
    switch(param)
    {
        case ISP_SERVICE_PREVIEW:
        {
        
            ISP_PREVIEW_PARAM_T *param_ptr = (ISP_PREVIEW_PARAM_T *)ptr;
            _ISP_ServiceSetPreviewParam(param_ptr);				
            
            break;
        }

        case ISP_SERVICE_JPEG:
        {

            ISP_JPEG_PARAM_T *param_ptr = (ISP_JPEG_PARAM_T *)ptr;
            _ISP_ServiceSetJpegParam(param_ptr);				
            
            break;
        }

        case ISP_SERVICE_MPEG:
        {
            
            ISP_MPEG_PARAM_T *param_ptr = (ISP_MPEG_PARAM_T *)ptr;
            _ISP_ServiceSetMpegParam(param_ptr);	
            
            break;
        }

        case ISP_SERVICE_REVIEW:
        {
            
            ISP_REVIEW_PARAM_T *param_ptr = (ISP_REVIEW_PARAM_T *)ptr;
            _ISP_ServiceSetReviewParam(param_ptr);	
            
            break;
        }

        case ISP_SERVICE_SCALE:
        {
        
            ISP_SCLAE_PARAM_T *param_ptr = (ISP_SCLAE_PARAM_T *)ptr;
            _ISP_ServiceSetScaleParam(param_ptr);				
           
            break;
        }

        case ISP_SERVICE_VP_ENCODE:
        {
       
            ISP_VT_ENCODE_PARAM_T *param_ptr = (ISP_VT_ENCODE_PARAM_T *)ptr;
            _ISP_ServiceSetVTEncodeParam(param_ptr);
    
           break;
        }

        case ISP_SERVICE_VP_EX_ENCODE:
        {
       
            ISP_VT_ENCODE_PARAM_T *param_ptr = (ISP_VT_ENCODE_PARAM_T *)ptr;
            _ISP_ServiceSetVTExtEncodeParam(param_ptr);
    
           break;
        }

        case ISP_SERVICE_VP_EX_SCALE:
        {
       
            ISP_SCLAE_PARAM_T *param_ptr = (ISP_SCLAE_PARAM_T *)ptr;
            _ISP_ServiceSetVTExtScaleParam(param_ptr);
    
           break;
        }

        case ISP_SERVICE_VP_DECODE:
        {
            
            ISP_VT_DECODE_PARAM_T *param_ptr = (ISP_VT_DECODE_PARAM_T *)ptr;
            _ISP_ServiceSetVTDecodeParam(param_ptr);				
            
            break;
        }

        default:
        
        break;
    }

    status = SCI_SetEvent(s_isp_event, ISP_SET_PARAM_EVENT, SCI_OR);
    SCI_ASSERT(!status); /*assert verified*/

    return ;
}


LOCAL void _ISP_ServiceStartSignalHandler(uint32 service,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;

	
    switch(param)
    {
        case ISP_SERVICE_VP_PLAYBACK:
		
            _ISP_ServiceStartVTPlayBack(ptr);
            
        break;
        
        case ISP_SERVICE_PREVIEW:
        
            LCD_SetBrushMode(s->display_block.lcd_id, 
                             LCD_BRUSH_MODE_ASYNC);
                             
            _ISP_ServiceStartPreview();
			
        break;
        
        case ISP_SERVICE_JPEG:
		
            _ISP_ServiceStartJpeg();
			
        break;
		
        case ISP_SERVICE_MPEG:
 
            _ISP_ServiceStartMpeg();
        
        break;
		
        case ISP_SERVICE_REVIEW:

            if(s->is_first_frame)
            {
                LCD_SetBrushMode(s->display_block.lcd_id, 
                                 LCD_BRUSH_MODE_ASYNC);
            }

            if(ISP_ADDR_INVALID(ptr))
            {
                _ISP_ServiceStartReview();
            
            }
            else
            {
                _ISP_ServiceContinueReviewing(ptr);
            }
			
        break;
		
        case ISP_SERVICE_SCALE:

            if(ISP_STATE_SCALE_SLICE == s->state)
            {
                _ISP_ServiceContinueScaling();            
            }
            else
            {
                _ISP_ServiceStartScale();
            }
            
        break;
		
        case ISP_SERVICE_VP:
			
            LCD_SetBrushMode(s->display_block.lcd_id, 
                             LCD_BRUSH_MODE_ASYNC);
            
            _ISP_ServiceStartVTEncode();
            
        break;

        case ISP_SERVICE_VP_EX:

            if(s->local_display_enable)
            {
                LCD_SetBrushMode(s->display_block.lcd_id, 
                                 LCD_BRUSH_MODE_ASYNC);
            }
            _ISP_ServiceStartVTExtEncode();

        break;

        case ISP_SERVICE_VP_EX_SCALE:

            _ISP_ServiceStartVTExtScale();

        break;   
        default:

        break;
	
    }
}

LOCAL void _ISP_ServiceStopSignalHandler(uint32 service,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    ISP_DriverSoftReset(AHB_REG_BASE);	
/*
    if(s->mem_ctrl)
    {
        SCI_GetMutex(s->mem_ctrl, SCI_WAIT_FOREVER);
    }

    if(s->mem_alloced_by_myself)
    {
        SCI_Free(s->p_work_mem);
        s->p_work_mem = SCI_NULL;
        s->mem_alloced_by_myself = SCI_FALSE;
        s->work_mem_len = 0;
    }
    if(s->mem_ctrl)
    {
        SCI_PutMutex(s->mem_ctrl);
    }
*/
    SCI_SetEvent(s_isp_event, 
                 ISP_STOP_EVENT, 
                 SCI_OR);
                 
    s->state = ISP_STATE_STOP;
}

LOCAL int32 _ISP_ServiceSendRequest(uint32 ptr_free_flag,uint32 command,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_SIGNAL_T            *sig_ptr = SCI_NULL;
    xSignalHeaderRec        *sig = SCI_NULL;

    s->curr_queue_available = _ISP_ServiceGetQueueAvailable();

    if (s->curr_queue_available >= ISP_QUEUE_FEW_NUM)
    {
        sig_ptr = (ISP_SIGNAL_T *)ISP_SERVICE_ALLOC(sizeof(ISP_SIGNAL_T));
        SCI_ASSERT(SCI_NULL != sig_ptr); /*assert verified*/

        sig = &sig_ptr->sig;
        
        sig->SignalSize  = sizeof(ISP_SIGNAL_T);
        sig->Sender      = ISP_SERVICE_THREAD;
        sig->SignalCode  = ptr_free_flag | s->service;
        sig_ptr->command = command ;
        sig_ptr->param   = param ;
        sig_ptr->ptr     = ptr ;

        if (SCI_SUCCESS != SCI_SendSignal((xSignalHeader)sig_ptr, 
                                          ISP_SERVICE_THREAD))
        {
            SCI_PASSERT(SCI_FALSE,("Failed to send Signal"));/*assert verified*/
        }
    }
    else
    {
        if(!_ISP_ServiceCommandLostPermit(command))
        {
            //ISP_SERVICE_TRACE:"_ISP_ServiceSendRequest, ISP Service Queue full"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3005_112_2_18_1_44_19_636,(uint8*)"");
        }
			
    }
    
    return 0;
}

LOCAL BOOLEAN _ISP_ServiceCommandLostPermit(uint32 command)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    BOOLEAN                 b_is_permit = SCI_FALSE;
	 
    switch(command)   
    {
        case ISP_SERVICE_PATH1_SIG:
        
            if(s->service == ISP_SERVICE_PREVIEW ||
               s->service == ISP_SERVICE_MPEG ||
               s->service == ISP_SERVICE_VP)
            {
                b_is_permit = SCI_TRUE;
            }
			
        break;
        
        case ISP_SERVICE_PATH2_SIG:			
        
            if(s->service == ISP_SERVICE_PREVIEW ||
               s->service == ISP_SERVICE_MPEG ||
               s->service == ISP_SERVICE_VP)
            {
                b_is_permit = SCI_TRUE;
            }
            
        break;
        
        default:
        	
        break;

    }

    return b_is_permit;
 
}

LOCAL uint32 _ISP_ServiceGetQueueAvailable(void)
{
    uint32                  status = 0;
    uint32                  count = 0;
    uint32                  enqueued = 0;
    uint32                  store = 0;
    char                    *queue_name_ptr = SCI_NULL;
    TX_THREAD               *suspended_thread = SCI_NULL;	
    TX_QUEUE                *next_queue = SCI_NULL;
    SCI_THREAD_T            *thread_block  = SCI_NULL;
    
   
    thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(ISP_SERVICE_THREAD);
    
    status = tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
                               &queue_name_ptr,
                               (uint32 *)&enqueued,
                               (uint32 *)&store,
                               (TX_THREAD **)&suspended_thread,
                               (uint32 *)&count,
                               (TX_QUEUE **)&next_queue);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE: queue info:enqueued:%d, store: %d, count: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3077_112_2_18_1_44_19_637,(uint8*)"ddd",enqueued, store, count);
    return store;
    
} 

LOCAL uint32 _ISP_ServiceGetPerformanceLevel()
{
    uint32                  curr_available = 0;
    uint32                  performance = ISP_SERVICE_PERFORMANCE_HIGH;    
    
    curr_available = _ISP_ServiceGetQueueAvailable();
    if(curr_available < ISP_QUEUE_NUM -20)
    {
        performance = ISP_SERVICE_PERFORMANCE_LOW;
    }
    else if(curr_available < ISP_QUEUE_NUM - 10)
    {
        performance = ISP_SERVICE_PERFORMANCE_MID;
    }
    else if(curr_available > ISP_QUEUE_NUM - 5)
    {
        performance = ISP_SERVICE_PERFORMANCE_HIGH;
    }
    
    return performance;
}

LOCAL void _ISP_ServiceGetImageFromLCDC(ISP_FRAME_T *p_isp_frame) 
{
    ISP_SERVICE_T           *s = &s_isp_service;
    LCD_CAP_PARAM_T         lcd_cap_t = {0};
    uint32                  frame_size = 0;
    uint32                  mem_size = 0;
    LCD_BRUSH_MODE_E        brush_mode = LCD_BRUSH_MODE_ASYNC;
    LCDC_RECT_T             lcd_rect = {0};
        
    frame_size = s->encoder_size.w * s->encoder_size.h * 2 + ISP_MEM_STRIDER_FOR_ONE_FRAME;
    lcd_cap_t.format = LCD_RGB565;
    lcd_cap_t.dst_base_addr = (uint32)ISP_ServiceGetMem(&mem_size) - frame_size;
    lcd_cap_t.dst_base_addr = ISP_ADDRESS_ALIGNED_(lcd_cap_t.dst_base_addr,
                                                   ISP_ALIGNED_BYTES);
    lcd_cap_t.cap_rect.x = s->input_range.x;
    lcd_cap_t.cap_rect.y = s->input_range.y;	
    lcd_cap_t.cap_rect.w = s->input_range.w;
    lcd_cap_t.cap_rect.h = s->input_range.h;
    lcd_cap_t.dst_size.w = s->encoder_size.w;
    lcd_cap_t.dst_size.h = s->encoder_size.h;
    lcd_cap_t.dst_trim_rect.x = 0;
    lcd_cap_t.dst_trim_rect.y = 0;	
    lcd_cap_t.dst_trim_rect.w = s->encoder_size.w;
    lcd_cap_t.dst_trim_rect.h = s->encoder_size.h;
    lcd_cap_t.set_endian_flag = 1;
    
#ifdef CHIP_ENDIAN_LITTLE
    lcd_cap_t.endian_switch = 1;
#else	
    lcd_cap_t.endian_switch = 0;
#endif

    lcd_cap_t.cap_en = SCI_TRUE;

    brush_mode = LCD_GetBrushMode(s->display_block.lcd_id);
    LCD_SetBrushMode(s->display_block.lcd_id, LCD_BRUSH_MODE_SYNC);
    LCD_ConfigCapture(s->display_block.lcd_id,&lcd_cap_t);

    if(ISP_ROTATION_0 == s->display_rotation)
    {
        lcd_rect.x = (uint16)s->display_range.x;
        lcd_rect.y = (uint16)s->display_range.y;
        lcd_rect.w = (uint16)s->display_range.w;
        lcd_rect.h = (uint16)s->display_range.h;	    
    }
    else
    {
        lcd_rect.x = (uint16)s->display_range.y;
        lcd_rect.y = (uint16)s->display_range.x;
        lcd_rect.w = (uint16)s->display_range.h;
        lcd_rect.h = (uint16)s->display_range.w;	    
    }
    
    LCD_InvalidateRect(0,
                       lcd_rect.x, 
                       lcd_rect.y,
                       lcd_rect.x + lcd_rect.w - 1, 
                       lcd_rect.y + lcd_rect.h - 1); 
    lcd_cap_t.cap_en = SCI_FALSE;
    LCD_ConfigCapture(s->display_block.lcd_id,&lcd_cap_t);
    LCD_SetBrushMode(s->display_block.lcd_id, brush_mode);                       

    p_isp_frame->yaddr = lcd_cap_t.dst_base_addr;
    p_isp_frame->uaddr = lcd_cap_t.dst_base_addr + (s->encoder_size.w * s->encoder_size.h);//
    p_isp_frame->uaddr = ISP_ADDRESS_ALIGNED_(p_isp_frame->uaddr,
                                              ISP_ALIGNED_BYTES);    
    p_isp_frame->width = s->encoder_size.w;
    p_isp_frame->height = s->encoder_size.h;	

    return ;
}

#endif

PUBLIC uint32 ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ID_E eid, int (*func)(void *))
{
    ISP_SERVICE_T           *s = &s_isp_service;
    
    if(SCI_NULL == func)
    {

    }

    switch(eid)
    {
	    case ISP_SERVICE_CALLBACK_ENCODE   :   
        
            _ISP_ServiceGetEncCtrl();
            s->encode_user_cb = func; 
            _ISP_ServiceRelEncCtrl();			
 
        break;
	    
	    case ISP_SERVICE_CALLBACK_DISPLAY   :   

            _ISP_ServiceGetDispCtrl();
            s->display_user_cb = func; 
            _ISP_ServiceRelDispCtrl();		
            					
        break;
        
        default:
            
        break;
    }

    return SCI_SUCCESS;
}


uint32 ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_ID_E eid)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = SCI_SUCCESS;


    switch(eid)
    {
        case ISP_SERVICE_CALLBACK_ENCODE   :   
			
            _ISP_ServiceGetEncCtrl();				
            s->encode_user_cb = SCI_NULL;  
            _ISP_ServiceRelEncCtrl();			
            
        break;
	    
        case ISP_SERVICE_CALLBACK_DISPLAY   :   
            
            _ISP_ServiceGetDispCtrl();						
            s->display_user_cb = SCI_NULL;
            _ISP_ServiceRelDispCtrl();			
            
        break;
	    
        default:
         
            status = SCI_ERROR;
            
        break;
    }
    
    return status;
}

LOCAL void _ISP_ServiceConvertCoor( LCD_ID_E lcd_id, ISP_RECT_T *src_ptr, ISP_RECT_T *dst_ptr,ISP_ROTATION_E rotate_ang)
{
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(lcd_id, &lcd_info);


    //ISP_SERVICE_TRACE:"_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3260_112_2_18_1_44_20_638,(uint8*)"ddddd", src_ptr->x, src_ptr->y, src_ptr->w, src_ptr->h,rotate_ang);

    switch(rotate_ang)
    {
        case ISP_ROTATION_0:

            SCI_MEMCPY(dst_ptr, src_ptr, sizeof(ISP_RECT_T));

        break;
        
        case ISP_ROTATION_90:

            dst_ptr->w = src_ptr->h;
            dst_ptr->h = src_ptr->w;
            dst_ptr->y = src_ptr->x;
            dst_ptr->x = lcd_info.lcd_width - src_ptr->h - src_ptr->y;


        break;

        case ISP_ROTATION_180:

            dst_ptr->x = lcd_info.lcd_width - src_ptr->w - src_ptr->x;
            dst_ptr->y = lcd_info.lcd_height - src_ptr->h - src_ptr->y;
            dst_ptr->w =  src_ptr->w;
            dst_ptr->h =  src_ptr->h;

        break;

        case ISP_ROTATION_270:

            dst_ptr->w = src_ptr->h;
            dst_ptr->h = src_ptr->w;
            dst_ptr->x = src_ptr->y;
            dst_ptr->y = lcd_info.lcd_height - src_ptr->w - src_ptr->x;

        break;

        default:

        break;

    }


    //ISP_SERVICE_TRACE:"_ISP_ServiceConvertCoor,dst x y w h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3309_112_2_18_1_44_20_639,(uint8*)"dddd", dst_ptr->x, dst_ptr->y, dst_ptr->w, dst_ptr->h);

    return;

}

LOCAL void _ISP_ServiceConfigBlock(ISP_DISP_BLOCK_T *p_disp_block,
                                   ISP_RECT_T       *p_range,
                                   ISP_RECT_T       *p_inv_range,
                                   uint32           rot_angle)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    BLOCKCFG_T              blk_cfg = {0};

    LCD_SetBackground(0);

#if 1    
    SCI_MEMCPY((void*)&p_disp_block->lcd_inv_rect,(void*)p_inv_range,sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&p_disp_block->img_rect,(void*)p_range,sizeof(ISP_RECT_T));  

    if(rot_angle == ISP_ROTATION_90 ||
       rot_angle == ISP_ROTATION_270) // to get the right size of target image
    {
       p_range->w = p_disp_block->img_rect.h;
       p_range->h = p_disp_block->img_rect.w;
    }
    
#else

    _ISP_ServiceConvertCoor(p_disp_block->lcd_id, 
                            p_inv_range, 
                            &p_disp_block->lcd_inv_rect, 
                            rot_angle);

    _ISP_ServiceConvertCoor(p_disp_block->lcd_id, 
                            p_range, 
                            &p_disp_block->img_rect, 
                            rot_angle);

#endif

	//ISP_SERVICE_TRACE:"ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3355_112_2_18_1_44_20_640,(uint8*)"ddddd",p_disp_block->block_id,p_disp_block->img_rect.x,p_disp_block->img_rect.y,p_disp_block->img_rect.w,p_disp_block->img_rect.h);
	
    blk_cfg.start_x = p_disp_block->img_rect.x;
    blk_cfg.start_y = p_disp_block->img_rect.y;
    blk_cfg.end_x = p_disp_block->img_rect.x + p_disp_block->img_rect.w - 1;
    blk_cfg.end_y = p_disp_block->img_rect.y + p_disp_block->img_rect.h - 1;       
    blk_cfg.width = p_disp_block->img_rect.w;
    blk_cfg.colorkey_en = 0;
    blk_cfg.alpha_sel = 1;
    blk_cfg.alpha = 0xFF;
    blk_cfg.priority = 0;
    blk_cfg.type = 1;

    if(s->service != ISP_SERVICE_VP &&
       s->service != ISP_SERVICE_VP_EX)    
    {
        if(LCD_LAYER_IMAGE == p_disp_block->block_id)
        {
            p_disp_block->img_fmt = ISP_DATA_YUV422;
            blk_cfg.resolution = LCD_RESOLUTION_YUV422; //YUV422
        }
        else 
        {
            p_disp_block->img_fmt = ISP_DATA_RGB565;
            blk_cfg.resolution = LCD_RESOLUTION_RGB565; //RGB565
        }
    }
    else
    {
        p_disp_block->img_fmt = ISP_DATA_RGB565;
        blk_cfg.resolution = LCD_RESOLUTION_RGB565; //RGB565

    }
    
#ifdef CHIP_ENDIAN_LITTLE
    blk_cfg.endian_switch = 1;
#else
    blk_cfg.endian_switch = 0;
#endif
    
	//ISP_SERVICE_TRACE:"ISP_SERVICE: block id %d blk_cfg.start.x,y end_x,y {%d,%d,%d,%d}, resolution %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3401_112_2_18_1_44_20_641,(uint8*)"dddddd",p_disp_block->block_id,blk_cfg.start_x,blk_cfg.start_y,blk_cfg.end_x,blk_cfg.end_y,blk_cfg.resolution);
	
    blk_cfg.set_endian_flag = 1;

    LCD_GetLcdCtrlSemaphore(0);
    LCD_ConfigBlock(p_disp_block->lcd_id,p_disp_block->block_id, blk_cfg);
    LCD_DisableBlock(p_disp_block->lcd_id,p_disp_block->block_id);
    LCD_PutLcdCtrlSemaphore(0);
    p_disp_block->is_enable = 0;

}

LOCAL void _ISP_ServiceClearBlockBuf(ISP_DISP_BLOCK_T *p_disp_blk)
{
    uint32                  *p_blk_buf = SCI_NULL;
    uint32                  blk_buf_len = 0;

    p_blk_buf = LCD_GetBlockBuffer(p_disp_blk->lcd_id,p_disp_blk->block_id);
    blk_buf_len = (uint32)(p_disp_blk->img_rect.w * p_disp_blk->img_rect.h);
    
    if(p_disp_blk->img_fmt == ISP_DATA_YUV422)
    {
    	//ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3425_112_2_18_1_44_20_642,(uint8*)"dd",p_blk_buf,blk_buf_len);
    
        SCI_MEMSET((void*)p_blk_buf, 
                    0, 
                    blk_buf_len);
        p_blk_buf = LCD_GetBlockUVBuffer(p_disp_blk->lcd_id,p_disp_blk->block_id);
    	//ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3433_112_2_18_1_44_20_643,(uint8*)"dd",p_blk_buf,blk_buf_len);
        
        SCI_MEMSET((void*)p_blk_buf, 
                    0x80, 
                    blk_buf_len);
    }
    else
    {
    	//ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3443_112_2_18_1_44_20_644,(uint8*)"dd",p_blk_buf,blk_buf_len);
    
        SCI_MEMSET((void*)p_blk_buf, 
                    0, 
                    blk_buf_len << 1);
        
    }   
    LCD_InvalidateRect(p_disp_blk->lcd_id,
                       p_disp_blk->lcd_inv_rect.x, 
                       p_disp_blk->lcd_inv_rect.y,
                       p_disp_blk->lcd_inv_rect.x + p_disp_blk->lcd_inv_rect.w - 1, 
                       p_disp_blk->lcd_inv_rect.y + p_disp_blk->lcd_inv_rect.h - 1);     
}

LOCAL void _ISP_ServiceCloseBlock(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    
	//ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceCloseBlock disp_blk enable %d remote disp blk enable %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3463_112_2_18_1_44_20_645,(uint8*)"dd",s->display_block.is_enable,s->remote_display_block.is_enable);

    if(s->display_block.is_enable == 1 &&
       s->display_block.lcd_id < LCD_SUM && 
       s->display_block.block_id < BLOCK_SUM)
    {
        LCD_GetLcdCtrlSemaphore(0);
        if(s->service == ISP_SERVICE_REVIEW) 
        {
            _ISP_ServiceClearBlockBuf(&s->display_block);   
        }
        LCD_DisableBlock(s->display_block.lcd_id,s->display_block.block_id);
        if(s->service != ISP_SERVICE_REVIEW) 
        {
            LCD_PutLcdCtrlSemaphore(0);
        }
        s->display_block.is_enable = 0;
    }

    if(s->remote_display_block.is_enable == 1 &&
       s->remote_display_block.lcd_id < LCD_SUM && 
       s->remote_display_block.block_id < BLOCK_SUM)
    {
        LCD_GetLcdCtrlSemaphore(0);
        LCD_DisableBlock(s->remote_display_block.lcd_id,s->remote_display_block.block_id);
        LCD_PutLcdCtrlSemaphore(0);
        s->remote_display_block.is_enable = 0;

    }
    
}


LOCAL void _ISP_ServiceRecoverBlkCfg(uint32 block_id)
{
/*
    ISP_SERVICE_T           *s = &s_isp_service;

    if(LCD_LAYER_IMAGE == block_id)
    {
        if(s->b_img_re_cfg)
        {
            LCD_ConfigBlock(0,block_id, s->img_blk_cfg);
            s->b_img_re_cfg = SCI_FALSE;
        }
    }
    else
    {
        if(s->b_osd_re_cfg)
        {
            if(s->display_block.block_id == block_id)    	
            {
                LCD_ConfigBlock(0,block_id, s->osd_blk_cfg);
            }
            else
            {
                LCD_ConfigBlock(0,block_id, s->osd2_blk_cfg);        	
            }
            s->b_osd_re_cfg = SCI_FALSE;			
        }
    }
*/

    return ;
}

// preview, review, mpeg preview,vt preview,vt review

LOCAL void _ISP_ServiceDisplayOneFrame(uint32 block_id,ISP_FRAME_T *p_frame) 
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_ADDRESS_T           src_addr_t = {0};
    ISP_ADDRESS_T           *p_addr =SCI_NULL;
    uint32                  disp_addr_y = p_frame->yaddr;
    uint32                  disp_addr_u = p_frame->uaddr;
    uint32                  rot_angle = ISP_ROTATION_0;
    uint32                  img_width = 0;
    uint32                  img_height = 0;
    ISP_DISP_BLOCK_T        *p_block = SCI_NULL;
    BLOCKCFG_T              blk_cfg = {0};

    if(ISP_DISPLAY_NONE == block_id)
        return ;

    if(block_id == s->display_block.block_id)
    {
        p_block = &s->display_block;
        p_addr = &s->rotation_buf_addr[s->display_frame_count % ISP_PATH1_FRAME_COUNT_MAX];
        rot_angle = s->display_rotation;
    }
    else
    {
        p_block = &s->remote_display_block;
        p_addr = &s->remote_rotation_display_addr[0];  
        rot_angle = s->remote_display_rotation;     
    }
    
    if(ISP_ROTATION_0 != rot_angle) //need rotation
    {
        rot_angle = _ISP_ServiceRotAngle2DrvAngle(rot_angle);
        
        src_addr_t.yaddr = p_frame->yaddr;
        src_addr_t.uaddr = p_frame->uaddr;	
        src_addr_t.vaddr = p_frame->vaddr;		

        if(ROTATION_180 == rot_angle)
        {
            img_width = p_block->img_rect.w;
            img_height = p_block->img_rect.h;
        }
        else
        {
            img_width = p_block->img_rect.h;
            img_height = p_block->img_rect.w;
        }
        
        _ISP_ServiceRotateImage(&src_addr_t, 
                                p_addr,
                                img_width, //the width of image is the height of block
                                img_height,
                                rot_angle,
                                p_block->img_fmt);
		
        disp_addr_y = p_addr->yaddr;
        disp_addr_u = p_addr->uaddr;
    }


    LCD_GetLcdCtrlSemaphore(0);

    LCD_SetBlockBuffer(p_block->lcd_id, p_block->block_id, (uint32 *)disp_addr_y);
	
    if(ISP_DATA_YUV422 == p_block->img_fmt)
    {
        LCD_SetUVBuffer(p_block->lcd_id, p_block->block_id, (uint32 *)disp_addr_u);
    }

    if(p_block->is_enable == 0)
    {
        LCD_EnableBlock(p_block->lcd_id,p_block->block_id);
        p_block->is_enable = 1;
    }
    else
    {
        //ISP_SERVICE_LOG:"_ISP_ServiceDisplayOneFrame: Block %d has been enabled"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3606_112_2_18_1_44_20_646,(uint8*)"d",block_id);
    }

	//ISP_SERVICE_LOG:"ISP_SERVICE: block enable %d,p_block->lcd_id %d, p_block->block_id %d "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3612_112_2_18_1_44_20_647,(uint8*)"ddd",LCD_GetBlockIsEnable(p_block->lcd_id,p_block->block_id),p_block->lcd_id,p_block->block_id);

    blk_cfg.start_x = p_block->img_rect.x;
    blk_cfg.start_y = p_block->img_rect.y;
    blk_cfg.end_x = p_block->img_rect.x + p_block->img_rect.w - 1;
    blk_cfg.end_y = p_block->img_rect.y + p_block->img_rect.h - 1;       
    blk_cfg.width = p_block->img_rect.w;
    blk_cfg.colorkey_en = 0;
    blk_cfg.alpha_sel = 1;
    blk_cfg.alpha = 0xFF;
    blk_cfg.priority = 0;
    blk_cfg.type = 1;
    
    if(s->service == ISP_SERVICE_VP ||
       s->service == ISP_SERVICE_VP_EX)
    {

        blk_cfg.resolution = LCD_RESOLUTION_RGB565; //RGB565

        #ifdef CHIP_ENDIAN_LITTLE
        blk_cfg.endian_switch = 1;
        #else
        blk_cfg.endian_switch = 0;
        #endif    

	    blk_cfg.set_endian_flag = 1;
		LCD_ConfigBlock(p_block->lcd_id,p_block->block_id, blk_cfg);
        LCD_EnableBlock(p_block->lcd_id,p_block->block_id);
    }
    else
    {
        //ISP_SERVICE_TRACE:"_ISP_ServiceDisplayOneFrame: s->is_first_frame = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3643_112_2_18_1_44_20_648,(uint8*)"d",s->is_first_frame);
        
        if(s->is_first_frame)    
        {
            if(LCD_LAYER_IMAGE == p_block->block_id)
            {
                blk_cfg.resolution = LCD_RESOLUTION_YUV422; //YUV422
            }
            else 
            {
                blk_cfg.resolution = LCD_RESOLUTION_RGB565; //RGB565
            }
            #ifdef CHIP_ENDIAN_LITTLE
            blk_cfg.endian_switch = 1;
            #else
            blk_cfg.endian_switch = 0;
            #endif  
    	    blk_cfg.set_endian_flag = 1;
    		LCD_ConfigBlock(p_block->lcd_id,p_block->block_id, blk_cfg);
            LCD_EnableBlock(p_block->lcd_id,p_block->block_id);            
        }
    }
    
    LCD_InvalidateRect(p_block->lcd_id,
                       p_block->lcd_inv_rect.x, 
                       p_block->lcd_inv_rect.y,
                       p_block->lcd_inv_rect.x + p_block->lcd_inv_rect.w - 1, 
                       p_block->lcd_inv_rect.y + p_block->lcd_inv_rect.h - 1); 

    s->display_frame_count ++;



}

LOCAL void _ISP_ServiceRotateImage(ISP_ADDRESS_T *p_src, 
                                   ISP_ADDRESS_T *p_dst, 
                                   uint32 width,
                                   uint32 height,
                                   uint32 rot_angle,
                                   uint32 img_format)
{
	/*Just ISP_IMAGE_FORMAT_RGB565/ISP_IMAGE_FORMAT_YUV420_3FRAME
		ISP_IMAGE_FORMAT_YUV422	?*/
    ROTATION_PARAM_T rotPara = {0};

    rotPara.rotation_mode = rot_angle;

	ispRotCvtFmt(img_format, &rotPara);

	ispRotCvtAddr(p_src, p_dst, &rotPara);
	
    ispRotCvtWH(width, height, rot_angle, &rotPara);

	//ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceRotateImage, rot_mode=%d,format=%d,img_size.w,h %d %d,"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3701_112_2_18_1_44_20_649,(uint8*)"dddd",rotPara.rotation_mode,rotPara.data_format,rotPara.img_size.w,rotPara.img_size.h);
    //ISP_SERVICE_LOG:"src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3708_112_2_18_1_44_20_650,(uint8*)"dddddd",rotPara.src_addr.y_addr,rotPara.src_addr.uv_addr,rotPara.src_addr.v_addr,rotPara.dst_addr.y_addr,rotPara.dst_addr.uv_addr,rotPara.dst_addr.v_addr);
    
    if(!Rotation_AppStart(&rotPara))
		//ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceRotateImage, SUCCESS"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3711_112_2_18_1_44_20_651,(uint8*)"");
	else
		//ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceRotateImage, FAIL"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3713_112_2_18_1_44_20_652,(uint8*)"");

	if(ISP_IMAGE_FORMAT_YUV420_3FRAME == rot_angle)
		ispRotYUV4203Frame(width, height, &rotPara);
}
LOCAL void _ISP_ServiceInit(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
	
   // SCI_MEMSET((void*)s, 0, sizeof(ISP_SERVICE_T));
    
    s->module_addr = ISP_AHB_SLAVE_ADDR;
    s->service = ISP_SERVICE_IDLE;
    s->state = ISP_STATE_STOP;
    s->display_block.block_id = ISP_DISPLAY_NONE;
    s->remote_display_block.block_id = ISP_DISPLAY_NONE;
	
    s->review_path_sema = (SCI_SEMAPHORE_PTR)SCI_CreateSemaphore("Semaphore for vp review path in", 1);
    SCI_ASSERT(s->review_path_sema != SCI_NULL); /*assert verified*/
    s->display_user_cb_ctrl = (SCI_MUTEX_PTR)SCI_CreateMutex("disp cb ctrl",SCI_INHERIT);
    SCI_ASSERT(s->display_user_cb_ctrl != SCI_NULL); /*assert verified*/
    s->encode_user_cb_ctrl = (SCI_MUTEX_PTR)SCI_CreateMutex("enc cb ctrl",SCI_INHERIT);
    SCI_ASSERT(s->encode_user_cb_ctrl != SCI_NULL); /*assert verified*/
    s->mem_ctrl = (SCI_MUTEX_PTR)SCI_CreateMutex("isp work memory ctrl",SCI_INHERIT);
    SCI_ASSERT(s->mem_ctrl != SCI_NULL); /*assert verified*/
    s->encode_user_cb = SCI_NULL; 
    s->display_user_cb = SCI_NULL; 
    s->p_work_mem = SCI_NULL;
    s->work_mem_len = 0;
    s->mem_alloced_by_myself = SCI_FALSE;
    
    _ISP_ServiceCreateTimer();	

    s->chng_freq_req_handler = CHNG_FREQ_GetReqHandler("isp");
    CHNG_FREQ_SetArmClk(s->chng_freq_req_handler,FREQ_INDEX_ISP_HIGH);

    s->p_scale_coeff_mem = (uint32 *)SCI_ALLOCA(ISP_SERVICE_SCALE_COEFF_BUF_SIZE);
    SCI_ASSERT(SCI_NULL != s->p_scale_coeff_mem); /*assert verified*/
    
    return ;
}

LOCAL void _ISP_ServiceDeInit(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->mem_ctrl)
    {
        SCI_GetMutex(s->mem_ctrl, SCI_WAIT_FOREVER);
    }

    if(s->mem_alloced_by_myself)
    {
        SCI_Free(s->p_work_mem);
        s->p_work_mem = SCI_NULL;
        s->mem_alloced_by_myself = SCI_FALSE;
        s->work_mem_len = 0;
    }
    if(s->mem_ctrl)
    {
        SCI_PutMutex(s->mem_ctrl);
        SCI_DeleteMutex(s->mem_ctrl);    
        s->mem_ctrl = SCI_NULL;
    }
    
    if(s->display_user_cb_ctrl != SCI_NULL)	
    {
        SCI_DeleteMutex(s->display_user_cb_ctrl);    
        s->display_user_cb_ctrl = SCI_NULL;
    }

    if(s->encode_user_cb_ctrl != SCI_NULL)
    {
        SCI_DeleteMutex(s->encode_user_cb_ctrl);    
        s->encode_user_cb_ctrl = SCI_NULL;
    }
	
    if(s->chng_freq_req_handler != CHNG_FREQ_REQ_HANDLER_NULL)
    {
        //ISP_SERVICE_TRACE:"restore ARMCLK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3792_112_2_18_1_44_21_653,(uint8*)"");
        CHNG_FREQ_RestoreARMClk(s->chng_freq_req_handler);
        CHNG_FREQ_DeleteReqHandler(s->chng_freq_req_handler); 
        s->chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
    }
						
    if(s->review_path_sema)
    {
        SCI_PutSemaphore(s->review_path_sema);//if any task suspended on this object, resume it
        SCI_DeleteSemaphore(s->review_path_sema);
        s->review_path_sema = NULL;
    }

    LCD_SetBrushMode(s->display_block.lcd_id, LCD_BRUSH_MODE_SYNC);

    if(s->display_block.block_id != ISP_DISPLAY_NONE)
    {
        _ISP_ServiceRecoverBlkCfg(s->display_block.block_id);
        s->display_block.block_id = ISP_DISPLAY_NONE;
    }
	
    if(s->remote_display_block.block_id != ISP_DISPLAY_NONE)
    {
        _ISP_ServiceRecoverBlkCfg(s->remote_display_block.block_id);
        s->remote_display_block.block_id = ISP_DISPLAY_NONE;
    }

    _ISP_ServiceDeleteTimer();

    if(s->p_scale_coeff_mem)
    {
        SCI_FREE(s->p_scale_coeff_mem);
        s->p_scale_coeff_mem = SCI_NULL;
    }

    s->state = ISP_STATE_CLOSED;
    s->service = ISP_SERVICE_IDLE;

    return ;

}

LOCAL void _ISP_ServiceGetDispCtrl(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->display_user_cb_ctrl)
        SCI_GetMutex(s->display_user_cb_ctrl, SCI_WAIT_FOREVER);
}

LOCAL void _ISP_ServiceRelDispCtrl(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->display_user_cb_ctrl)
        SCI_PutMutex(s->display_user_cb_ctrl);
}

LOCAL void _ISP_ServiceGetEncCtrl(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->encode_user_cb_ctrl)
        SCI_GetMutex(s->encode_user_cb_ctrl, SCI_WAIT_FOREVER);
}

LOCAL void _ISP_ServiceRelEncCtrl(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->encode_user_cb_ctrl)
        SCI_PutMutex(s->encode_user_cb_ctrl);
}
LOCAL void _ISP_ServiceCallUserDispCB(void *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    _ISP_ServiceGetDispCtrl();
    if(s->display_user_cb)
    {
        (*s->display_user_cb)(p_frame); 	     	
    }
    _ISP_ServiceRelDispCtrl();			

}
LOCAL void _ISP_ServiceCallUserEncCB(void *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    _ISP_ServiceGetEncCtrl();
    if(s->encode_user_cb)
    {
        (*s->encode_user_cb)(p_frame); 	     	
    }
    _ISP_ServiceRelEncCtrl();			            

}

LOCAL void _ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr = SCI_NULL;
    uint32                  coeff = 1 + (uint32)(LCD_LAYER_IMAGE != param_ptr->disp_block_id);
    ISP_SIZE_T              dst_img_size = {0};	
    
    s->service = ISP_SERVICE_PREVIEW;
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetPreviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3899_112_2_18_1_44_21_654,(uint8*)"");
	
	
    s->cap_input_image_format = param_ptr->cap_param.cap_img_format;
    s->cap_input_image_patten = param_ptr->cap_param.cap_img_pattern;
    s->vsync_polarity         = param_ptr->cap_param.cap_vsync_pol;
    s->hsync_polarity         = param_ptr->cap_param.cap_hsync_pol;
    s->pclk_polarity          = param_ptr->cap_param.cap_pclk_pol;
    s->preview_skip_frame_num = param_ptr->cap_param.cap_frame_skipped;
    s->preview_deci_frame_num = param_ptr->cap_param.cap_frame_decimated;
	
    SCI_MEMCPY((void*)&(s->cap_input_size), 
               (void*)&param_ptr->input_size, 
               sizeof(ISP_SIZE_T));
	
    SCI_MEMCPY((void*)&(s->cap_input_range), 
               (void*)&param_ptr->input_range, 
               sizeof(ISP_RECT_T));
  
    SCI_MEMCPY((void*)&(s->display_range), 
               (void*)&(param_ptr->disp_range), 
               sizeof(ISP_RECT_T));

    if(param_ptr->disp_rotation == ISP_ROTATION_90 ||
       param_ptr->disp_rotation == ISP_ROTATION_270)
    {
        dst_img_size.w = param_ptr->disp_range.h;
        dst_img_size.h = param_ptr->disp_range.w;        
    }
    else
    {
        dst_img_size.w = param_ptr->disp_range.w;
        dst_img_size.h = param_ptr->disp_range.h;        
    }
    
    s->cap_output_size.w = s->cap_input_range.w;
    s->cap_output_size.h = s->cap_input_range.h;

    s->cap_img_dec.x_factor = _ISP_ServiceGetXYDeciFactor(&s->cap_output_size.w,
                                                          &s->cap_output_size.h,
                                                          dst_img_size.w * coeff,
                                                          dst_img_size.h * coeff);
                                                          

    s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;
    s->cap_img_dec.x_mode = ISP_CAP_IMG_DEC_MODE_DIRECT;

    s->input_size.w = s->cap_output_size.w;
    s->input_size.h = s->cap_output_size.h;

    s->input_range.x = 0;
    s->input_range.y = 0;
    s->input_range.w = s->cap_output_size.w;
    s->input_range.h = s->cap_output_size.h;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetPreviewParam,input for scaling {%d,%d,%d,%d}, output from CAP {%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3960_112_2_18_1_44_21_655,(uint8*)"dddddd",s->input_range.x,s->input_range.y,s->input_range.w,s->input_range.h,s->cap_output_size.w,s->cap_output_size.h);

    s->display_rotation = param_ptr->disp_rotation;
	
    disp_block_ptr = &(s->display_block);
	
    disp_block_ptr->lcd_id = param_ptr->disp_lcd_id;
    disp_block_ptr->block_id = param_ptr->disp_block_id;
    
    _ISP_ServiceConfigBlock(disp_block_ptr, 
                            &s->display_range, 
                            &param_ptr->lcd_rect, 
                            s->display_rotation);

    s->display_frame_count = 0;
    
    if(SCI_TRUE == _ISP_ServiceAllocFrame())
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }
    else
    {   
        s->err_code = ISP_ERR_CODE_NO_MEM;    
    }

    return ;

}

LOCAL void _ISP_ServiceSetJpegParam(ISP_JPEG_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetJpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_3993_112_2_18_1_44_21_656,(uint8*)"");
	
    s->service = ISP_SERVICE_JPEG;					
	
    SCI_MEMCPY((void*)&s->cap_input_size, 
               (void*)&param_ptr->input_size, 
               sizeof(ISP_SIZE_T));
	
    SCI_MEMCPY((void*)&s->cap_input_range, 
               (void*)&param_ptr->input_range, 
               sizeof(ISP_RECT_T));
	
    SCI_MEMCPY((void*)&s->encoder_size, 
               (void*)&param_ptr->output_size, 
               sizeof(ISP_SIZE_T));
	
    s->cap_input_image_format = param_ptr->cap_param.cap_img_format;
    s->cap_input_image_patten = param_ptr->cap_param.cap_img_pattern;
    s->vsync_polarity         = param_ptr->cap_param.cap_vsync_pol;
    s->hsync_polarity         = param_ptr->cap_param.cap_hsync_pol;
    s->pclk_polarity          = param_ptr->cap_param.cap_pclk_pol;
    s->capture_skip_frame_num = param_ptr->cap_param.cap_frame_skipped;

    s->cap_output_size.w = s->cap_input_range.w;
    s->cap_output_size.h = s->cap_input_range.h;

    s->input_size.w = s->cap_output_size.w;
    s->input_size.h = s->cap_output_size.h;
    s->input_range.x = 0; 
    s->input_range.y = 0;
    s->input_range.w = s->cap_output_size.w;
    s->input_range.h = s->cap_output_size.h;
    s->cap_img_dec.x_factor = _ISP_ServiceGetXYDeciFactor(&s->cap_output_size.w,
                                                          &s->cap_output_size.h,
                                                          s->encoder_size.w,
                                                          s->encoder_size.h);
    s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;
    s->cap_img_dec.x_mode = ISP_CAP_IMG_DEC_MODE_DIRECT;		

    if((s->cap_output_size.w < s->encoder_size.w || s->cap_output_size.h < s->encoder_size.h) &&
       (s->encoder_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH))
    {
        s->is_slice = SCI_TRUE;
        s->slice_height = ISP_SCALE_SLICE_HEIGHT;		
    }
    else
    {
        s->is_slice = SCI_FALSE;
    }

    if(SCI_TRUE == _ISP_ServiceAllocFrame())
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }
    else
    {   
        s->err_code = ISP_ERR_CODE_NO_MEM;    
    }

    return ;

}

LOCAL void _ISP_ServiceSetMpegParam(ISP_MPEG_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr = SCI_NULL;
    ISP_SIZE_T              cap_out_max = {0};
    ISP_SIZE_T              dst_img_size = {0};	

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetMpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4063_112_2_18_1_44_21_657,(uint8*)"");

    s->service = ISP_SERVICE_MPEG;
    s->cap_input_image_format = param_ptr->cap_param.cap_img_format;
    s->cap_input_image_patten = param_ptr->cap_param.cap_img_pattern;
    s->vsync_polarity         = param_ptr->cap_param.cap_vsync_pol;
    s->hsync_polarity         = param_ptr->cap_param.cap_hsync_pol;
    s->pclk_polarity          = param_ptr->cap_param.cap_pclk_pol;
    s->preview_skip_frame_num = param_ptr->cap_param.cap_frame_skipped;
    s->preview_deci_frame_num = param_ptr->cap_param.cap_frame_decimated;


    SCI_MEMCPY((void*)&s->cap_input_size, 
               (void*)&param_ptr->input_size, 
               sizeof(ISP_SIZE_T));
#if 1	
    SCI_MEMCPY((void*)&s->cap_input_range,
               (void*)&param_ptr->input_range, 
               sizeof(ISP_RECT_T));
#else
    s->cap_input_range.x = 0;
    s->cap_input_range.y = 0;
    s->cap_input_range.w = param_ptr->input_size.w;
    s->cap_input_range.h = param_ptr->input_size.h;
#endif

    if(param_ptr->encode_rotation == ISP_ROTATION_90 || 
       param_ptr->encode_rotation == ISP_ROTATION_270)
    {
        s->encoder_size.w = param_ptr->encode_size.h;
        s->encoder_size.h = param_ptr->encode_size.w;
    }
    else
    {
        s->encoder_size.w = param_ptr->encode_size.w;
        s->encoder_size.h = param_ptr->encode_size.h;
    }
	
    SCI_MEMCPY((void*)&s->display_range,
               (void*)&param_ptr->disp_range, 
               sizeof(ISP_RECT_T));

    
    if(param_ptr->disp_rotation == ISP_ROTATION_90 ||
       param_ptr->disp_rotation == ISP_ROTATION_270)
    {
        dst_img_size.w = param_ptr->disp_range.h;
        dst_img_size.h = param_ptr->disp_range.w;        
    }
    else
    {
        dst_img_size.w = param_ptr->disp_range.w;
        dst_img_size.h = param_ptr->disp_range.h;        
    }

    cap_out_max.w = ISP_MIN(dst_img_size.w, s->encoder_size.w * 2);
    cap_out_max.h = ISP_MIN(dst_img_size.h, s->encoder_size.h * 2);	   

    s->cap_output_size.w = s->cap_input_range.w;
    s->cap_output_size.h = s->cap_input_range.h;

    s->cap_img_dec.x_factor = _ISP_ServiceGetXYDeciFactor(&s->cap_output_size.w,
                                                          &s->cap_output_size.h,
                                                          cap_out_max.w,
                                                          cap_out_max.h);
                                                          

    s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;
    s->cap_img_dec.x_mode = ISP_CAP_IMG_DEC_MODE_DIRECT;

    s->input_size.w = s->cap_output_size.w;
    s->input_size.h = s->cap_output_size.h;
    s->remote_input_size.w = s->cap_output_size.w;
    s->remote_input_size.h = s->cap_output_size.h;	
	   
    if(s->cap_output_size.w * dst_img_size.h > s->cap_output_size.h * dst_img_size.w)
    {
        s->input_range.w = s->cap_output_size.h * dst_img_size.w / dst_img_size.h;
        s->input_range.w = ISP_PIXEL_LEFT_ALIGNED_(s->input_range.w, ISP_ALIGNED_PIXELS);;
        s->input_range.h = s->cap_output_size.h;
    }
    else
    {
        s->input_range.h = s->cap_output_size.w * dst_img_size.h / dst_img_size.w;
        s->input_range.h = ISP_PIXEL_LEFT_ALIGNED_(s->input_range.h, ISP_ALIGNED_PIXELS);;
        s->input_range.w = s->cap_output_size.w;
    }
	
    s->input_range.x = (s->cap_output_size.w - s->input_range.w) / 2;
    s->input_range.x = ISP_PIXEL_LEFT_ALIGNED_(s->input_range.x, ISP_ALIGNED_PIXELS);
    s->input_range.y = (s->cap_output_size.h - s->input_range.h) / 2; //input_range is for path1(display) trim window 
    s->input_range.y = ISP_PIXEL_LEFT_ALIGNED_(s->input_range.y, ISP_ALIGNED_PIXELS);

    if(s->cap_output_size.w * s->encoder_size.h > s->cap_output_size.h * s->encoder_size.w)
    {
        s->remote_input_range.w = s->cap_output_size.h * s->encoder_size.w / s->encoder_size.h;
        s->remote_input_range.w = ISP_PIXEL_ALIGNED_(s->remote_input_range.w,ISP_ALIGNED_PIXELS);
        s->remote_input_range.h = s->cap_output_size.h;
    }
    else
    {
        s->remote_input_range.h = s->cap_output_size.w * s->encoder_size.h / s->encoder_size.w;
        s->remote_input_range.w = ISP_PIXEL_ALIGNED_(s->remote_input_range.h,ISP_ALIGNED_PIXELS);
        s->remote_input_range.w = s->cap_output_size.w;
    }
	
    s->remote_input_range.x = (s->cap_output_size.w - s->remote_input_range.w) / 2;
    s->remote_input_range.y = (s->cap_output_size.h - s->remote_input_range.h) / 2;//remote_input_range is for path2(encoder) trim window 

    s->path2_output_image_format = param_ptr->data_format;
    s->display_rotation = param_ptr->disp_rotation;
    s->enc_rotation = param_ptr->encode_rotation;
    disp_block_ptr = &s->display_block;

    if(ISP_DISPLAY_NONE != param_ptr->disp_lcd_id)
    {
        disp_block_ptr->lcd_id = param_ptr->disp_lcd_id;
        disp_block_ptr->block_id = param_ptr->disp_block_id;
                      
        _ISP_ServiceConfigBlock(disp_block_ptr, 
                                &s->display_range, 
                                &param_ptr->lcd_rect, 
                                s->display_rotation);

    }
    else
    {
        disp_block_ptr->img_rect.w = param_ptr->disp_range.w;
        disp_block_ptr->img_rect.h = param_ptr->disp_range.h;
    }

    s->display_frame_count = 0;

    if(SCI_TRUE == _ISP_ServiceAllocFrame())
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }
    else
    {   
        s->err_code = ISP_ERR_CODE_NO_MEM;    
    }
    return ;
	
}
LOCAL void _ISP_ServiceReviewWindowRearrange(ISP_RECT_T *p_input_range,
                                             ISP_RECT_T *p_disp_range,
                                             ISP_RECT_T *p_lcd_rect,
                                             uint32     rot_angle)
{
    uint32                  tmp_size = 0;
	
    if(rot_angle == ISP_ROTATION_90 ||
       rot_angle == ISP_ROTATION_270) 
    {
        if(p_disp_range->w > (p_input_range->h * ISP_PATH_SC_COEFF_MAX))  
        {
            tmp_size = (uint32)(p_input_range->h * ISP_PATH_SC_COEFF_MAX);
			p_disp_range->x = p_disp_range->x + ((p_disp_range->w - tmp_size) >> 1);
            p_disp_range->w = tmp_size;
			
        }
        if(p_disp_range->h > (p_input_range->w * ISP_PATH_SC_COEFF_MAX))  
        {
            tmp_size = (uint32)(p_input_range->w * ISP_PATH_SC_COEFF_MAX);
			p_disp_range->y = p_disp_range->y + ((p_disp_range->h - tmp_size) >> 1);
			p_disp_range->h = tmp_size;			
        }
    }
    else
    {
        if(p_disp_range->w > (p_input_range->w * ISP_PATH_SC_COEFF_MAX))  
        {
            tmp_size = (uint32)(p_input_range->w * ISP_PATH_SC_COEFF_MAX);
			p_disp_range->x = p_disp_range->x + ((p_disp_range->w - tmp_size) >> 1);
            p_disp_range->w = tmp_size;
        }
        if(p_disp_range->h > (p_input_range->h * ISP_PATH_SC_COEFF_MAX))  
        {
            tmp_size = (uint32)(p_input_range->h * ISP_PATH_SC_COEFF_MAX);
			p_disp_range->y = p_disp_range->y + ((p_disp_range->h - tmp_size) >> 1);
			p_disp_range->h = tmp_size;
        }
        
    }

}
LOCAL void _ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr;
    uint32                  mem_size = 0;
    BOOLEAN                 rtn = SCI_FALSE;
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetReviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4256_112_2_18_1_44_22_658,(uint8*)"");
	
    s->service = ISP_SERVICE_REVIEW;
    
    s->path2_input_image_format = param_ptr->input_format;
	
    SCI_MEMCPY((void*)&s->input_size, 
               (void*)&param_ptr->input_size, 
               sizeof(ISP_SIZE_T));
	
    SCI_MEMCPY((void*)&s->input_range, 
               (void*)&param_ptr->input_range, 
               sizeof(ISP_RECT_T));
	
    SCI_MEMCPY((void*)&s->input_addr, 
               (void*)&param_ptr->input_addr, 
               sizeof(ISP_ADDRESS_T));
	
    SCI_MEMCPY((void*)&s->display_range, 
               (void*)&param_ptr->disp_range, 
               sizeof(ISP_RECT_T));

    _ISP_ServiceReviewWindowRearrange(&s->input_range,
                                      &s->display_range,
                                      &param_ptr->lcd_rect,
                                      param_ptr->disp_rotation);
  
    s->display_rotation = param_ptr->disp_rotation;

    disp_block_ptr = &s->display_block;
	
    disp_block_ptr->lcd_id   = param_ptr->disp_lcd_id;
    disp_block_ptr->block_id = param_ptr->disp_block_id;	
                      
    if(ISP_DISPLAY_NONE == param_ptr->disp_block_id)
    {
        disp_block_ptr->img_fmt =  ISP_IMAGE_FORMAT_RGB565;   
    }
    else
    {
        _ISP_ServiceConfigBlock(disp_block_ptr, 
                                &s->display_range, 
                                &param_ptr->lcd_rect, 
                                s->display_rotation);        
    }
		
    if(ISP_DATA_YUV422 == disp_block_ptr->img_fmt) //if not from dc,re-config block,or the same configuration as preview
    {	
        s->path2_output_image_format = ISP_IMAGE_FORMAT_YUV422;
    }
    else
    {
        s->path2_output_image_format = ISP_IMAGE_FORMAT_RGB565; 	
    }

    s->display_frame_count = 0;
    //ISP_SERVICE_TRACE:"disp_block_ptr->width = %d,disp_block_ptr->height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4314_112_2_18_1_44_22_659,(uint8*)"dd",disp_block_ptr->img_rect.w,disp_block_ptr->img_rect.h);
    
    if(SCI_TRUE == _ISP_ServiceAllocFrame())
    {
        mem_size = s->work_mem_res_len;

        rtn = _ISP_ServiceCheckMemAddr(&s->input_addr, 
                                       &s->input_size, 
                                       s->path2_input_image_format,
                                       (uint32)s->p_work_mem_cur,
                                       &mem_size);
        if(SCI_TRUE == rtn)
        {
            s->err_code = ISP_ERR_CODE_NO_ERR;
        }
        else
        {
            s->err_code = ISP_ERR_CODE_NO_MEM;    
        }
    }
    else
    {   
        s->err_code = ISP_ERR_CODE_NO_MEM;    
    }
   
    return ;
}

LOCAL void _ISP_ServiceSetScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_SIZE_T              input_size = {0};
    ISP_SIZE_T              output_size = {0};   
    uint32                  dst_buf_id = 0;
    uint32                  mem_start = (uint32)s->p_work_mem_cur;
    uint32                  mem_size = s->work_mem_res_len;
    BOOLEAN                 rtn = SCI_FALSE;
     
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4352_112_2_18_1_44_22_660,(uint8*)"");
	
    s->service = ISP_SERVICE_SCALE;
	
    s->path2_input_image_format = param_ptr->input_format;
    s->path2_output_image_format = param_ptr->output_format;

    if(param_ptr->is_first == SCI_TRUE)
    {
        s->state = ISP_STATE_CLOSED;
	    if(param_ptr->scale_mode == ISP_SERVICE_SCALE_SLICE)
	    {
	        s->is_slice_alone = SCI_TRUE;
            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_SetScaleParam, slice heght %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4365_112_2_18_1_44_22_661,(uint8*)"d",param_ptr->slice_line);
            
	    }
    }

    if(s->path2_input_image_format == ISP_IMAGE_FORMAT_RGB565_EX)
    {
#ifdef CHIP_ENDIAN_LITTLE
        s->rgb_ex_height  = param_ptr->input_rect.y + param_ptr->slice_line;
        _ISP_ServiceRGB565FromLittleToHalfBig(param_ptr->input_addr.yaddr, 
                                              (uint32)(s->rgb_ex_height * param_ptr->input_size.w));
        SCI_MEMCPY((void*)&s->local_addr[0],(void*)&param_ptr->input_addr,sizeof(ISP_ADDRESS_T));
        s->rgb_ex_flag    = SCI_TRUE;
#endif    
        s->path2_input_image_format = ISP_IMAGE_FORMAT_RGB565;
    }
    else if(s->path2_input_image_format == ISP_IMAGE_FORMAT_RGB888_EX)
    {
#ifdef CHIP_ENDIAN_LITTLE
        s->rgb_ex_height  = param_ptr->input_rect.y + param_ptr->slice_line;
        //ISP_SERVICE_LOG:"_ISP_ServiceSetScaleParam, rgb_ex_height %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4385_112_2_18_1_44_22_662,(uint8*)"d",s->rgb_ex_height);
        _ISP_ServiceRGB888FromBigToLittle(param_ptr->input_addr.yaddr, 
                                              (uint32)(s->rgb_ex_height * param_ptr->input_size.w));
        SCI_MEMCPY((void*)&s->local_addr[0],(void*)&param_ptr->input_addr,sizeof(ISP_ADDRESS_T));
        s->rgb_ex_flag = SCI_TRUE;
#endif    
        s->path2_input_image_format = ISP_IMAGE_FORMAT_RGB888;
    }    
    
    SCI_MEMCPY((void*)&s->input_size, 
               (void*)&param_ptr->input_size, 
               sizeof(ISP_SIZE_T));
    SCI_MEMCPY((void*)&input_size,(void*)&s->input_size,sizeof(ISP_SIZE_T));
    SCI_MEMCPY((void*)&s->input_range, 
               (void*)&(param_ptr->input_rect), 
               sizeof(ISP_RECT_T));
    input_size.h = s->input_range.y + s->input_range.h;	
    SCI_MEMCPY((void*)&s->input_addr,
               (void*)&(param_ptr->input_addr), 
               sizeof(ISP_ADDRESS_T));
	
    SCI_MEMCPY((void*)&s->output_addr,
               (void*)&(param_ptr->output_addr), 
               sizeof(ISP_ADDRESS_T));
	
    SCI_MEMCPY((void*)&s->encoder_size, 
               (void*)&param_ptr->output_size, 
               sizeof(ISP_SIZE_T));
    SCI_MEMCPY((void*)&output_size, 
               (void*)&param_ptr->output_size, 
               sizeof(ISP_SIZE_T));
    
    if(param_ptr->scale_mode == ISP_SERVICE_SCALE_SLICE)
    {
        s->slice_height = param_ptr->slice_line;
    }
    else if(s->encoder_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH &&
            param_ptr->slice_line == 0)
    {
        s->slice_height = ISP_SCALE_SLICE_HEIGHT;   
    }
    
    if(param_ptr->scale_mode == ISP_SERVICE_SCALE_NORMAL ||
       (param_ptr->scale_mode == ISP_SERVICE_SCALE_SLICE && param_ptr->is_first == SCI_TRUE))               
    {
        if(s->encoder_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH ||
           param_ptr->scale_mode == ISP_SERVICE_SCALE_SLICE) //should use slice mode
        {
            s->is_slice = SCI_TRUE;
	        
            if(s->slice_height == s->input_range.h)
            {
                input_size.h = s->input_range.y + s->input_range.h;	
                s->slice_out_height = s->encoder_size.h;
                output_size.h = s->encoder_size.h;
            }
            else
            {
                input_size.h = s->input_range.y + s->slice_height;	
                s->slice_out_height = (s->slice_height + s->input_range.y) * s->encoder_size.h / s->input_range.h;
                s->slice_out_height = ISP_SLICE_OUT_ESTIMATE(s->slice_out_height);
                output_size.h = s->slice_out_height;
                
            }
            s->total_slice_count = 0;

        }
        else
        {
            s->is_slice = SCI_FALSE;
        }

        s->total_scale_out = 0;   
        s->scale_user_cb = param_ptr->get_scale_data;
    }

    if(!ISP_ADDR_INVALID(param_ptr->line_buf_addr))
    {
        s->line_buff.yaddr = param_ptr->line_buf_addr;
    }
    else
    {
        s->line_buff.yaddr = 0;
    }

    if(!ISP_ADDR_INVALID(param_ptr->swap_buf_addr))
    {
        s->swap_buff.yaddr = param_ptr->swap_buf_addr;
        s->swap_buff.uaddr = param_ptr->swap_buf_addr + (s->encoder_size.w * s->slice_height);  
        s->swap_buff.uaddr = (s->swap_buff.uaddr + ISP_MEM_STRIDER_FOR_ONE_FRAME) & ~0xFF;
    }  
    else
    {
        s->swap_buff.yaddr = 0;
        s->swap_buff.uaddr = 0;  
    }

    if(SCI_TRUE == _ISP_ServiceAllocFrame())
    {
        mem_start = (uint32)s->p_work_mem_cur;
        mem_size = s->work_mem_res_len;
        rtn = _ISP_ServiceCheckMemAddr(&s->input_addr, 
                                             &input_size, 
                                             param_ptr->input_format,
                                             mem_start,
                                             &mem_size);
        if(SCI_FALSE == rtn)
        {
            s->err_code = ISP_ERR_CODE_NO_MEM;                        
        }
        else
        {
            mem_start += mem_size;
            mem_size = s->work_mem_res_len - mem_size;
            if(s->is_slice || s->is_slice_alone)
            {
                if(param_ptr->is_first)
                {
                    dst_buf_id = 0;    
                }
                else
                {
                    dst_buf_id = (1 + s->total_slice_count) % ISP_PATH2_FRAME_COUNT_MAX;
                }
                
                rtn = _ISP_ServiceCheckMemAddr(&s->encoder_addr[dst_buf_id], 
                                               &output_size, 
                                               param_ptr->output_format,
                                               mem_start,
                                               &mem_size);
                                               
            }
            else
            {
                rtn = _ISP_ServiceCheckMemAddr(&s->encoder_addr[0], 
                                               &output_size, 
                                               param_ptr->output_format,
                                               mem_start,
                                               &mem_size);
            }

            if(SCI_FALSE == rtn)
            {
                s->err_code = ISP_ERR_CODE_NO_MEM;                        
            }
            else
            {
                s->err_code = ISP_ERR_CODE_NO_ERR;            
            }
        }
    }
    else
    {   
        s->err_code = ISP_ERR_CODE_NO_MEM;    
    }
   
    return ;
}

LOCAL void _ISP_ServiceSetVTScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  mem_start = (uint32)s->p_work_mem_cur;
    uint32                  mem_size = s->work_mem_res_len;
    BOOLEAN                 rtn  = SCI_FALSE;
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceSetVTScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4551_112_2_18_1_44_22_663,(uint8*)"");
	
    s->path2_input_image_format = param_ptr->input_format;
    s->path2_output_image_format = param_ptr->output_format;
	
    SCI_MEMCPY((void*)&s->remote_input_size, 
               (void*)&param_ptr->input_size, 
               sizeof(ISP_SIZE_T));
	
    SCI_MEMCPY((void*)&s->remote_input_range, 
               (void*)&(param_ptr->input_rect), 
               sizeof(ISP_RECT_T));
	
    SCI_MEMCPY((void*)&s->remote_input_addr,
               (void*)&(param_ptr->input_addr), 
               sizeof(ISP_ADDRESS_T));
	
    SCI_MEMCPY((void*)&s->output_addr,
               (void*)&(param_ptr->output_addr), 
               sizeof(ISP_ADDRESS_T));

    s->remote_display_range.w = param_ptr->output_size.w;
    s->remote_display_range.h = param_ptr->output_size.h;    

    if(s->encoder_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH) //should use slice mode
    {
        //ISP_SERVICE_TRACE:"Scale in VT mode, width of target image is too large"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4577_112_2_18_1_44_22_664,(uint8*)"");
    }

    rtn = _ISP_ServiceCheckMemAddr(&s->remote_input_addr, 
                                   &s->remote_input_size, 
                                   s->path2_input_image_format,
                                   mem_start,
                                   &mem_size);
    
    s->scale_user_cb = param_ptr->get_scale_data;
    return ;
	
}

LOCAL void _ISP_ServiceEncodeFrameInit(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_enc_frame = SCI_NULL;
    uint32                  i = 0,mem_size = 0;
    
    mem_size = (uint32)(ISP_PATH2_FRAME_COUNT_MAX * sizeof(ISP_FRAME_T));
    p_enc_frame = (ISP_FRAME_T*)ISP_SERVICE_ALLOC(mem_size);
    SCI_MEMSET((void*)p_enc_frame, 0, mem_size);
    for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i++)
    {
        (p_enc_frame+i)->yaddr = s->encoder_addr[i].yaddr;
        (p_enc_frame+i)->uaddr = s->encoder_addr[i].uaddr;            
        (p_enc_frame+i)->width = s->encoder_size.w;
        (p_enc_frame+i)->height= s->encoder_size.h;
        (p_enc_frame+i)->next  = p_enc_frame + (i + 1) % ISP_PATH2_FRAME_COUNT_MAX;
        (p_enc_frame+i)->prev  = p_enc_frame + (i - 1 + ISP_PATH2_FRAME_COUNT_MAX) % ISP_PATH2_FRAME_COUNT_MAX;
    	//ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceEncodeFrameInit, frame 0x%x"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4608_112_2_18_1_44_22_665,(uint8*)"d",(p_enc_frame+i));

    }
    s->p_frame_enc = p_enc_frame;
    s->p_frame_enc_cur = p_enc_frame; 
}

LOCAL void _ISP_ServiceEncodeFrameDeInit(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(SCI_NULL != s->p_frame_enc)    
    {
        SCI_Free(s->p_frame_enc);
        s->p_frame_enc = SCI_NULL;
        s->p_frame_enc_cur = SCI_NULL;
    }
}

LOCAL void _ISP_ServiceSetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr = SCI_NULL;
   
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetVTEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4632_112_2_18_1_44_22_666,(uint8*)"");

    s->service = ISP_SERVICE_VP;				
	
    s->cap_input_image_format = param_ptr->cap_param.cap_img_format;
    s->cap_input_image_patten = param_ptr->cap_param.cap_img_pattern;
    s->vsync_polarity         = param_ptr->cap_param.cap_vsync_pol;
    s->hsync_polarity         = param_ptr->cap_param.cap_hsync_pol;
    s->pclk_polarity          = param_ptr->cap_param.cap_pclk_pol;
    s->preview_skip_frame_num = param_ptr->cap_param.cap_frame_skipped;
    s->preview_deci_frame_num = param_ptr->cap_param.cap_frame_decimated;


    SCI_MEMCPY((void*)&s->cap_input_size, 
               (void*)&param_ptr->local_input_size, 
               sizeof(ISP_SIZE_T));
	
    SCI_MEMCPY((void*)&s->cap_input_range, 
               (void*)&param_ptr->local_input_range, 
               sizeof(ISP_RECT_T));

    if(param_ptr->encode_rotation == ISP_ROTATION_90 || 
       param_ptr->encode_rotation == ISP_ROTATION_270)
    {
        s->encoder_size.w = param_ptr->encode_size.h;
        s->encoder_size.h = param_ptr->encode_size.w;
    }
    else
    {
        s->encoder_size.w = param_ptr->encode_size.w;
        s->encoder_size.h = param_ptr->encode_size.h;
    }
    
    SCI_MEMCPY((void*)&s->display_range, 
               (void*)&param_ptr->local_disp_range, 
               sizeof(ISP_RECT_T));
	
    s->cap_output_size.w = s->cap_input_range.w;
    s->cap_output_size.h = s->cap_input_range.h;

    s->cap_img_dec.x_factor = _ISP_ServiceGetXYDeciFactor(&s->cap_output_size.w,
                                                          &s->cap_output_size.h,
                                                          s->encoder_size.w,
                                                          s->encoder_size.h);
                                                          
    s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;
    s->cap_img_dec.x_mode = ISP_CAP_IMG_DEC_MODE_DIRECT;

    s->input_size.w = s->cap_output_size.w;
    s->input_size.h = s->cap_output_size.h;

    if(s->cap_output_size.w * s->encoder_size.h > 
       s->cap_output_size.h * s->encoder_size.w)
    {
        s->input_range.w = s->cap_output_size.h * s->encoder_size.w / s->encoder_size.h;
        s->input_range.w = ISP_PIXEL_ALIGNED_(s->input_range.w,ISP_ALIGNED_PIXELS);
        s->input_range.h = s->cap_output_size.h;
    }
    else
    {
        s->input_range.h = s->cap_output_size.w * s->encoder_size.h / s->encoder_size.w;
        s->input_range.h = ISP_PIXEL_ALIGNED_(s->input_range.h,ISP_ALIGNED_PIXELS);
        s->input_range.w = s->cap_output_size.w;
    }
	
    s->input_range.x = (s->cap_output_size.w - s->input_range.w) / 2;
    s->input_range.y = (s->cap_output_size.h - s->input_range.h) / 2;

    s->enc_rotation     = param_ptr->encode_rotation;	
    s->display_rotation = param_ptr->disp_rotation;
    disp_block_ptr = &s->display_block;
	
 
    disp_block_ptr->lcd_id     = param_ptr->local_disp_lcd_id;
    disp_block_ptr->block_id   = param_ptr->local_disp_block_id;

    s->local_review_enable     = param_ptr->local_review_enable;    
    s->local_display_enable    = param_ptr->local_display_enable;
    
    if(s->local_display_enable)
    {
        _ISP_ServiceConfigBlock(disp_block_ptr, 
                                &s->display_range, 
                                &param_ptr->lcd_rect, 
                                s->display_rotation);
    }
    
    //ISP_SERVICE_TRACE:"_ISP_ServiceSetVTEncodeParam,display_enable = %d,review_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4721_112_2_18_1_44_23_667,(uint8*)"dd",s->local_display_enable,s->local_review_enable);

    s->display_frame_count = 0;
    
    if(SCI_TRUE == _ISP_ServiceAllocFrame())
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }
    else
    {   
        s->err_code = ISP_ERR_CODE_NO_MEM;    
    }
   
    return ;
}

LOCAL void _ISP_ServiceSetVTExtEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr = SCI_NULL;
   
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceSetVTExtEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4742_112_2_18_1_44_23_668,(uint8*)"");

    s->service = ISP_SERVICE_VP_EX;		
    
    s->cap_input_image_format = param_ptr->cap_param.cap_img_format;
    s->cap_input_image_patten = param_ptr->cap_param.cap_img_pattern;
    s->vsync_polarity         = param_ptr->cap_param.cap_vsync_pol;
    s->hsync_polarity         = param_ptr->cap_param.cap_hsync_pol;
    s->pclk_polarity          = param_ptr->cap_param.cap_pclk_pol;
    s->preview_skip_frame_num = param_ptr->cap_param.cap_frame_skipped;
    s->preview_deci_frame_num = param_ptr->cap_param.cap_frame_decimated;

    SCI_MEMCPY((void*)&s->cap_input_size, 
               (void*)&param_ptr->local_input_size, 
               sizeof(ISP_SIZE_T));
	
    SCI_MEMCPY((void*)&s->cap_input_range, 
               (void*)&param_ptr->local_input_range, 
               sizeof(ISP_RECT_T));

    if(param_ptr->encode_rotation == ISP_ROTATION_90 || 
       param_ptr->encode_rotation == ISP_ROTATION_270)
    {
        s->encoder_size.w = param_ptr->encode_size.h;
        s->encoder_size.h = param_ptr->encode_size.w;
    }
    else
    {
        s->encoder_size.w = param_ptr->encode_size.w;
        s->encoder_size.h = param_ptr->encode_size.h;
    }

    if(param_ptr->local_review_enable || param_ptr->local_display_enable)
    {
        SCI_MEMCPY((void*)&s->display_range, 
                   (void*)&param_ptr->local_disp_range, 
                   sizeof(ISP_RECT_T));
    	
        s->cap_output_size.w = s->cap_input_range.w;
        s->cap_output_size.h = s->cap_input_range.h;

        s->cap_img_dec.x_factor = _ISP_ServiceGetXYDeciFactor(&s->cap_output_size.w,
                                                              &s->cap_output_size.h,
                                                              s->encoder_size.w,
                                                              s->encoder_size.h);
        															
        s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;
        s->cap_img_dec.x_mode = ISP_CAP_IMG_DEC_MODE_DIRECT;

        s->input_size.w = s->cap_output_size.w;
        s->input_size.h = s->cap_output_size.h;

        if(s->cap_output_size.w * s->encoder_size.h > 
           s->cap_output_size.h * s->encoder_size.w)
        {
            s->input_range.w = s->cap_output_size.h * s->encoder_size.w / s->encoder_size.h;
            s->input_range.w = ISP_PIXEL_ALIGNED_(s->input_range.w,ISP_ALIGNED_PIXELS);
            s->input_range.h = s->cap_output_size.h;
        }
        else
        {
            s->input_range.h = s->cap_output_size.w * s->encoder_size.h / s->encoder_size.w;
            s->input_range.h = ISP_PIXEL_ALIGNED_(s->input_range.h,ISP_ALIGNED_PIXELS);
            s->input_range.w = s->cap_output_size.w;
        }
    	
        s->input_range.x = (s->cap_output_size.w - s->input_range.w) / 2;
        s->input_range.y = (s->cap_output_size.h - s->input_range.h) / 2;

        s->enc_rotation     = param_ptr->encode_rotation;	
        s->display_rotation = param_ptr->disp_rotation;
        disp_block_ptr = &s->display_block;
    	
     
        disp_block_ptr->lcd_id     = param_ptr->local_disp_lcd_id;
        disp_block_ptr->block_id   = param_ptr->local_disp_block_id;

        s->local_review_enable     = 1;    
        s->local_display_enable    = 1;
        if(s->local_display_enable)
        {
            _ISP_ServiceConfigBlock(disp_block_ptr, 
                                    &s->display_range, 
                                    &param_ptr->lcd_rect, 
                                    s->display_rotation);
        }
    }
    else
    {
        s->local_review_enable     = 0;    
        s->local_display_enable    = 0;
        s->cap_output_size.w = s->cap_input_range.w;
        s->cap_output_size.h = s->cap_input_range.h;        
        s->input_size.w = s->cap_output_size.w;
        s->input_size.h = s->cap_output_size.h;        
        s->input_range.x = 0;
        s->input_range.y = 0;        
        s->input_range.w = s->input_size.w;
        s->input_range.h = s->input_size.h;        
        
    }
    
    //ISP_SERVICE_TRACE:"_ISP_ServiceSetVTExtEncodeParam,display_enable = %d,review_enable = %d, skip num %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4847_112_2_18_1_44_23_669,(uint8*)"ddd",s->local_display_enable,s->local_review_enable,s->preview_skip_frame_num);

    if(SCI_TRUE == _ISP_ServiceAllocFrame())
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }
    else
    {   
        s->err_code = ISP_ERR_CODE_NO_MEM;    
    }

    return ;
}
LOCAL void _ISP_ServiceSetVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr = SCI_NULL;
    uint32                  path1_frame_size = 0;
    uint32                  i = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetVTDecodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4867_112_2_18_1_44_23_670,(uint8*)"");

    s->service = ISP_SERVICE_VP;				
    SCI_MEMCPY((void*)&s->remote_input_size,
               (void*)&param_ptr->remote_input_size,
               sizeof(ISP_SIZE_T));
	
    SCI_MEMCPY((void*)&s->remote_input_range, 
               (void*)&param_ptr->remote_input_range, 
               sizeof(ISP_RECT_T));
	
    SCI_MEMCPY((void*)&s->remote_display_range, 
               (void*)&param_ptr->remote_disp_range, 
               sizeof(ISP_RECT_T));
	
    disp_block_ptr = &(s->remote_display_block);
	
    disp_block_ptr->lcd_id     = param_ptr->remote_disp_lcd_id;
    disp_block_ptr->block_id   = param_ptr->remote_disp_block_id;

    s->remote_deblock_enable   = param_ptr->deblock_enable;
    s->remote_display_rotation = param_ptr->remote_disp_rotation;
    s->remote_review_enable    = param_ptr->remote_review_enable;	 
    s->remote_display_enable   = param_ptr->remote_display_enable;
	
    if(s->remote_display_enable)
    {
        _ISP_ServiceConfigBlock(disp_block_ptr, 
                                &s->remote_display_range, 
                                &param_ptr->lcd_rect, 
                                s->remote_display_rotation);
    }
  	
    //ISP_SERVICE_TRACE:"_ISP_ServiceSetVTDecodeParam,display_enable = %d,review_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4902_112_2_18_1_44_23_671,(uint8*)"dd",s->remote_display_enable,s->remote_review_enable);

    path1_frame_size = s->remote_display_range.w * s->remote_display_range.h;
    path1_frame_size += ISP_MEM_STRIDER_FOR_ONE_FRAME;
	
    for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i ++)
    {
        s->remote_display_addr[i].yaddr = s->vt_mem_end_cur + i * path1_frame_size * 2;
        s->remote_display_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->remote_display_addr[i].yaddr,
                                                                ISP_ALIGNED_BYTES);
        s->remote_display_addr[i].uaddr = s->remote_display_addr[i].yaddr + path1_frame_size;
        s->remote_display_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->remote_display_addr[i].uaddr,
                                                                ISP_ALIGNED_BYTES);
        s->remote_display_addr[i].vaddr = 0;
		
    }

    for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i ++)
    {
        s->remote_rotation_display_addr[i].yaddr = s->vt_mem_end_cur + ISP_PATH2_FRAME_COUNT_MAX * path1_frame_size * 2;
        s->remote_rotation_display_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->remote_rotation_display_addr[i].yaddr,
                                                                         ISP_ALIGNED_BYTES);
		
        s->remote_rotation_display_addr[i].uaddr = s->remote_rotation_display_addr[i].yaddr + path1_frame_size;
        s->remote_rotation_display_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->remote_rotation_display_addr[i].uaddr,
                                                                         ISP_ALIGNED_BYTES);
        s->remote_rotation_display_addr[i].vaddr = 0;
    }

    s->vt_mem_end_cur += path1_frame_size *( ISP_PATH2_FRAME_COUNT_MAX + 1) * 2;// disp buff + rotation buff

    if(s->vt_mem_end_cur > (uint32)s->p_work_mem + s->work_mem_len)
    {
        s->err_code = ISP_ERR_CODE_NO_MEM;      
    }
    else
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }

    return ;
}

LOCAL void _ISP_ServiceSetVTExtScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceSetVTExtScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_4949_112_2_18_1_44_23_672,(uint8*)"");

    SCI_MEMCPY((void*)&s->remote_input_size,
               (void*)&param_ptr->input_size,
               sizeof(ISP_SIZE_T));
	
    SCI_MEMCPY((void*)&s->remote_input_range, 
               (void*)&param_ptr->input_rect, 
               sizeof(ISP_RECT_T));

    s->remote_display_range.w = param_ptr->output_size.w;
    s->remote_display_range.h = param_ptr->output_size.h;    

    SCI_MEMCPY((void*)&s->remote_input_addr,
               (void*)&(param_ptr->input_addr), 
               sizeof(ISP_ADDRESS_T));
    SCI_MEMCPY((void*)&s->output_addr,
               (void*)&(param_ptr->output_addr),
               sizeof(ISP_ADDRESS_T));
    
    s->path2_input_image_format = param_ptr->input_format;
    s->path2_output_image_format = param_ptr->output_format;

    s->scale_user_cb = param_ptr->get_scale_data;
    return ;
}

LOCAL  uint32 _ISP_ServiceEnterVTReview(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = 0;

    status = SCI_GetSemaphore(s->review_path_sema, ISP_VP_REVIEW_TIME);
    if(status)
    {
        SCI_PutSemaphore(s->review_path_sema);
    }
	
    return status;		

}

LOCAL void _ISP_ServiceExitVTReview(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->review_path_sema)
    {
        SCI_PutSemaphore(s->review_path_sema);
    }
    
    return ;
}


LOCAL uint32 _ISP_ServiceCheckPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(param_ptr->disp_lcd_id, &lcd_info);
		
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckPreviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5010_112_2_18_1_44_23_673,(uint8*)"");
	
    if(SCI_NULL == param_ptr)  
    {
        return SCI_ERROR;
    }
	
    if(((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->disp_range.w % ISP_ALIGNED_PIXELS)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels aligned"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5021_112_2_18_1_44_23_674,(uint8*)"");
        return SCI_ERROR;
    }
	
    if( param_ptr->disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5027_112_2_18_1_44_23_675,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->disp_block_id >=BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5033_112_2_18_1_44_23_676,(uint8*)"");
        return SCI_ERROR;
    }
	
	
    if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5041_112_2_18_1_44_23_677,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5047_112_2_18_1_44_23_678,(uint8*)"");
        return SCI_ERROR;
    }
    
    if(param_ptr->disp_range.w > lcd_info.lcd_width||
       param_ptr->disp_range.h > lcd_info.lcd_height)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Display size is larger than max lcd size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5054_112_2_18_1_44_23_679,(uint8*)"");
        return SCI_ERROR;
    }

    return SCI_SUCCESS;

}

LOCAL uint32 _ISP_ServiceCheckJpegParam(ISP_JPEG_PARAM_T * param_ptr)
{
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckJpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5065_112_2_18_1_44_23_680,(uint8*)"");
	
    if(SCI_NULL == param_ptr)  
    {
        return SCI_ERROR;
    }
	
    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->output_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5076_112_2_18_1_44_23_681,(uint8*)"");
        return SCI_ERROR;
    }
	
    if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5083_112_2_18_1_44_23_682,(uint8*)"");
        return SCI_ERROR;
    }

    if(CHIP_GetChipType() == SC8801G2)	
    {
        if(param_ptr->cap_param.cap_img_format == ISP_CAP_INPUT_FORMAT_JPEG)
        {
            return SCI_ERROR;            
        }
        else
        {
            if(param_ptr->input_size.w > ISP_UXGA_W || 
               param_ptr->input_size.h > ISP_UXGA_W)
            {
                return SCI_ERROR;            
            }
        }
    }

    return SCI_SUCCESS;
}


LOCAL uint32 _ISP_ServiceCheckMpegParam(ISP_MPEG_PARAM_T * param_ptr)
{
    LCD_INFO_T lcd_info={0x00};
    

		if(param_ptr->disp_lcd_id == MAIN_LCD_ID || param_ptr->disp_lcd_id ==  SUB_LCD_ID)  //for PC Camera mode
		{
    	    LCD_GetInfo(param_ptr->disp_lcd_id, &lcd_info);
    }

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckMpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5117_112_2_18_1_44_24_683,(uint8*)"");
	
    if(SCI_NULL == param_ptr)  
    {
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->disp_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->encode_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5129_112_2_18_1_44_24_684,(uint8*)"");
        return SCI_ERROR;
    }
    if(param_ptr->data_format != ISP_IMAGE_FORMAT_YUV422 &&
    	 param_ptr->data_format != ISP_IMAGE_FORMAT_YUV420)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Unsupported data format"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5135_112_2_18_1_44_24_685,(uint8*)"");
        return SCI_ERROR;
    }
    
    if(ISP_DISPLAY_NONE != param_ptr->disp_lcd_id)
    {
        if(param_ptr->disp_lcd_id >= LCD_SUM)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5143_112_2_18_1_44_24_686,(uint8*)"");
            return SCI_ERROR;
        }
	
        if(param_ptr->disp_block_id >=BLOCK_SUM)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5149_112_2_18_1_44_24_687,(uint8*)"");
            return SCI_ERROR;
        }
		
        if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
           (param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5156_112_2_18_1_44_24_688,(uint8*)"");
            return SCI_ERROR;

        }

        if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5163_112_2_18_1_44_24_689,(uint8*)"");
            return SCI_ERROR;
        }
        
        if(ISP_ROTATION_0 != param_ptr->disp_rotation)
        {
		
        }
        else
        {
        		if(param_ptr->disp_lcd_id == MAIN_LCD_ID || param_ptr->disp_lcd_id ==  SUB_LCD_ID)  //for PC Camera mode
        		{	

	            if((param_ptr->disp_range.w > lcd_info.lcd_width)||
	               (param_ptr->disp_range.h > lcd_info.lcd_height))
	            {
	                //ISP_SERVICE_TRACE:"ISP_SERVICE: Display size is larger than max lcd size"
	                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5179_112_2_18_1_44_24_690,(uint8*)"");
	                return SCI_ERROR;
	            }
	         }
        }
    }
	
    return SCI_SUCCESS;

}

LOCAL uint32 _ISP_ServiceCheckReviewParam(ISP_REVIEW_PARAM_T * param_ptr)
{
    ISP_ADDRESS_T           *p_addr = SCI_NULL;
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckReviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5194_112_2_18_1_44_24_691,(uint8*)"");
	
    if(SCI_NULL == param_ptr)  
    {
        return SCI_ERROR;
    }
	
    if(param_ptr->input_format != ISP_IMAGE_FORMAT_YUV420 &&
       param_ptr->input_format != ISP_IMAGE_FORMAT_RGB565 &&
       param_ptr->input_format != ISP_IMAGE_FORMAT_YUV422 &&
       param_ptr->input_format != ISP_IMAGE_FORMAT_RGB888 &&
       param_ptr->input_format != ISP_IMAGE_FORMAT_YUV420_3FRAME)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input_format = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5207_112_2_18_1_44_24_692,(uint8*)"d",param_ptr->input_format);
        return SCI_ERROR;
    }
	
    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->disp_range.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5215_112_2_18_1_44_24_693,(uint8*)"");
        return SCI_ERROR;
    }
#if 0	
    if(param_ptr->input_range.x % ISP_ALIGNED_PIXELS)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input start x must be 4 pixels align when review"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5221_112_2_18_1_44_24_694,(uint8*)"");
        return SCI_ERROR;
    }
#endif
    p_addr = &param_ptr->input_addr;
    
    if((p_addr->yaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->uaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->vaddr % ISP_ALIGNED_BYTES))	
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5234_112_2_18_1_44_24_695,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);
        return SCI_ERROR;    
    }

    if(ISP_YUV_INVALIDE(p_addr->yaddr,p_addr->uaddr,p_addr->vaddr))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5243_112_2_18_1_44_24_696,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);
        
        return SCI_ERROR;
    }
    
    if(param_ptr->disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5250_112_2_18_1_44_24_697,(uint8*)"");
        return SCI_ERROR;
    }
/*	
    if(param_ptr->disp_block_id >= BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5255_112_2_18_1_44_24_698,(uint8*)"");
        return SCI_ERROR;
    }
*/	
    if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
	   (param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5262_112_2_18_1_44_24_699,(uint8*)"");
        return SCI_ERROR;
    }
    if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5267_112_2_18_1_44_24_700,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->disp_range.w > ISP_SCALE_FRAME_MODE_WIDTH_TH)	
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Width of display rect is out of threshold"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5273_112_2_18_1_44_24_701,(uint8*)"");
        return SCI_ERROR;
    }
    
    return SCI_SUCCESS;
    
}

LOCAL uint32 _ISP_ServiceCheckScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_ADDRESS_T           *p_addr = SCI_NULL;
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5285_112_2_18_1_44_24_702,(uint8*)"");
	
    if(SCI_NULL == param_ptr)  
    {
        return SCI_ERROR;
    }

    if(param_ptr->scale_mode == ISP_SERVICE_SCALE_SLICE && 
       param_ptr->slice_line == 0)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: slice mode sline height = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5295_112_2_18_1_44_24_703,(uint8*)"");
        return SCI_ERROR;
    }
    
    if(param_ptr->input_format != ISP_IMAGE_FORMAT_YUV420 && 
       param_ptr->input_format != ISP_IMAGE_FORMAT_RGB565 && 
       param_ptr->input_format != ISP_IMAGE_FORMAT_YUV422 &&  
       param_ptr->input_format != ISP_IMAGE_FORMAT_RGB888 &&
       param_ptr->input_format != ISP_IMAGE_FORMAT_RGB888_EX &&
       param_ptr->input_format != ISP_IMAGE_FORMAT_YUV400 && 
       param_ptr->input_format != ISP_IMAGE_FORMAT_RGB565_EX &&       
       param_ptr->input_format != ISP_IMAGE_FORMAT_YUV420_3FRAME)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input_format = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5308_112_2_18_1_44_24_704,(uint8*)"d",param_ptr->input_format);
        return SCI_ERROR;
    }
	
    if(param_ptr->output_format != ISP_IMAGE_FORMAT_YUV420 && 
       param_ptr->output_format != ISP_IMAGE_FORMAT_RGB565 && 
       param_ptr->output_format != ISP_IMAGE_FORMAT_YUV422 &&
       param_ptr->output_format != ISP_IMAGE_FORMAT_RGB565_EX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: output_format = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5317_112_2_18_1_44_24_705,(uint8*)"d",param_ptr->output_format);
        return SCI_ERROR;
    }
	
    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_rect.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->output_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixel align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5325_112_2_18_1_44_24_706,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->input_rect.x % ISP_ALIGNED_PIXELS)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input start x must be 4 pixel align when scale"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5331_112_2_18_1_44_24_707,(uint8*)"");
        return SCI_ERROR;
    }
	
    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5338_112_2_18_1_44_24_708,(uint8*)"");
        return SCI_ERROR;
    }

    p_addr = &param_ptr->input_addr;
    
    if((p_addr->yaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->uaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->vaddr % ISP_ALIGNED_BYTES))	
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5351_112_2_18_1_44_24_709,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);
        return SCI_ERROR;    
    }

    if(ISP_YUV_INVALIDE(p_addr->yaddr,p_addr->uaddr,p_addr->vaddr))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5360_112_2_18_1_44_24_710,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);
        
        return SCI_ERROR;
    }
    

    if(param_ptr->input_format == ISP_IMAGE_FORMAT_YUV420_3FRAME &&
       param_ptr->output_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5369_112_2_18_1_44_24_711,(uint8*)"");
        return SCI_ERROR;
    }

    
    return SCI_SUCCESS; 
	
}


LOCAL uint32 _ISP_ServiceCheckVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(param_ptr->local_disp_lcd_id, &lcd_info);
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckVTEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5385_112_2_18_1_44_24_712,(uint8*)"");

    if(SCI_NULL == param_ptr)  
    {
        return SCI_ERROR;
    }
	
    if((param_ptr->local_input_size.w % ISP_ALIGNED_PIXELS) ||
       (param_ptr->local_input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->local_disp_range.w % ISP_ALIGNED_PIXELS) ||
       (param_ptr->encode_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels aligned"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5397_112_2_18_1_44_24_713,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(0 == param_ptr->local_display_enable)
    {
        return SCI_SUCCESS;
    }
  
    if(param_ptr->local_disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5408_112_2_18_1_44_24_714,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->local_disp_block_id >= BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5414_112_2_18_1_44_24_715,(uint8*)"");
        return SCI_ERROR;
    }
	
	
    if((param_ptr->local_input_size.w < (param_ptr->local_input_range.x + param_ptr->local_input_range.w)) ||
       (param_ptr->local_input_size.h < (param_ptr->local_input_range.y + param_ptr->local_input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5422_112_2_18_1_44_24_716,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5428_112_2_18_1_44_24_717,(uint8*)"");
        return SCI_ERROR;
    }
    if(param_ptr->local_disp_range.w > lcd_info.lcd_width ||
       param_ptr->local_disp_range.h > lcd_info.lcd_height)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Display size is larger than max lcd size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5434_112_2_18_1_44_24_718,(uint8*)"");
        return SCI_ERROR;
    }
    
    return SCI_SUCCESS;
    
}
LOCAL uint32 _ISP_ServiceCheckVTExtEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    LCD_INFO_T lcd_info={0x00};

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceCheckVTExtEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5445_112_2_18_1_44_24_719,(uint8*)"");

    if(SCI_NULL == param_ptr)  
    {
        return SCI_ERROR;
    }
	
    if((param_ptr->local_input_size.w % ISP_ALIGNED_PIXELS) ||
       (param_ptr->local_input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->local_disp_range.w % ISP_ALIGNED_PIXELS) ||
       (param_ptr->encode_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels aligned"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5457_112_2_18_1_44_24_720,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(0 == param_ptr->local_review_enable)
    {
        return SCI_SUCCESS;
    }
  
    if(param_ptr->local_disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5468_112_2_18_1_44_24_721,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->local_disp_block_id >= BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5474_112_2_18_1_44_24_722,(uint8*)"");
        return SCI_ERROR;
    }
	
	
    if((param_ptr->local_input_size.w < (param_ptr->local_input_range.x + param_ptr->local_input_range.w)) ||
       (param_ptr->local_input_size.h < (param_ptr->local_input_range.y + param_ptr->local_input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5482_112_2_18_1_44_24_723,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5488_112_2_18_1_44_24_724,(uint8*)"");
        return SCI_ERROR;
    }
    
    LCD_GetInfo(param_ptr->local_disp_lcd_id, &lcd_info);
    if(param_ptr->local_disp_range.w > lcd_info.lcd_width||
       param_ptr->local_disp_range.h > lcd_info.lcd_height)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Display size is larger than max lcd size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5496_112_2_18_1_44_24_725,(uint8*)"");
        return SCI_ERROR;
    }

    return SCI_SUCCESS;
    
}

LOCAL uint32 _ISP_ServiceCheckVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr)
{
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckVTDecodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5507_112_2_18_1_44_24_726,(uint8*)"");
	
    if(SCI_NULL == param_ptr)  
    {
        return SCI_ERROR;
    }
	
    if((param_ptr->remote_input_size.w % ISP_ALIGNED_PIXELS) ||
       (param_ptr->remote_input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->remote_disp_range.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixel align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5518_112_2_18_1_44_24_727,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->remote_input_range.x % ISP_ALIGNED_PIXELS)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input start x must be 4 pixel align when review"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5524_112_2_18_1_44_24_728,(uint8*)"");
        return SCI_ERROR;
    }
	
    if((param_ptr->remote_input_size.w < (param_ptr->remote_input_range.x + param_ptr->remote_input_range.w)) ||
       (param_ptr->remote_input_size.h < (param_ptr->remote_input_range.y + param_ptr->remote_input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5531_112_2_18_1_44_24_729,(uint8*)"");
        return SCI_ERROR;
    }

    if(0 == param_ptr->remote_display_enable)
    {
        return SCI_SUCCESS;   	
    }
    
    if(param_ptr->remote_disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5542_112_2_18_1_44_24_730,(uint8*)"");
        return SCI_ERROR;
    }
	
    if(param_ptr->remote_disp_block_id >=BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5548_112_2_18_1_44_24_731,(uint8*)"");
        return SCI_ERROR;
    }
    
    if(param_ptr->remote_disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5554_112_2_18_1_44_24_732,(uint8*)"");
        return SCI_ERROR;
    }

    return SCI_SUCCESS;
}

LOCAL void _ISP_ServiceSetVPFlag(uint32 flag)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    
    s->vt_review_from = flag;
}

LOCAL void _ISP_ServiceStartVTScale(ISP_FRAME_T *p_src_frame, ISP_RECT_T *p_trim_rect,ISP_FRAME_T *p_dst_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              isp_size = {0};
    ISP_RECT_T              isp_rect = {0};
    ISP_ADDRESS_T           isp_addr = {0};
    uint32                  param    = SCI_TRUE;
    
    if(p_trim_rect == SCI_NULL)
    {
        if(p_src_frame->width * p_dst_frame->height > p_src_frame->height * p_dst_frame->width)
        {
            isp_rect.w = p_src_frame->height * p_dst_frame->width / p_dst_frame->height;
            isp_rect.w = ISP_PIXEL_LEFT_ALIGNED_(isp_rect.w, ISP_ALIGNED_PIXELS);
            isp_rect.h = p_src_frame->height;
            isp_rect.y = 0;
            isp_rect.x = (p_src_frame->width - isp_rect.w) >> 1;
            isp_rect.x = ISP_PIXEL_LEFT_ALIGNED_(isp_rect.x, ISP_ALIGNED_PIXELS);
            
        }
        else
        {
            isp_rect.h = p_src_frame->width * p_dst_frame->height / p_dst_frame->width;
            isp_rect.h = ISP_PIXEL_LEFT_ALIGNED_(isp_rect.h, ISP_ALIGNED_PIXELS);
            isp_rect.w = p_src_frame->width;
            isp_rect.x = 0;
            isp_rect.y = (p_src_frame->height - isp_rect.h) >> 1;
            isp_rect.y = ISP_PIXEL_LEFT_ALIGNED_(isp_rect.y, ISP_ALIGNED_PIXELS);
        }
    }
    else
    {
        isp_rect.x = p_trim_rect->x;
        isp_rect.y = p_trim_rect->y;
        isp_rect.w = p_trim_rect->w;
        isp_rect.h = p_trim_rect->h;        
    }
    
    rtn_drv = ISP_DriverExtInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    isp_size.w = p_src_frame->width;
    isp_size.h = p_src_frame->height;    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_SIZE, 
                                    (void*)&isp_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&isp_rect);
    ISP_RTN_IF_ERR(rtn_drv);

    isp_addr.yaddr = p_src_frame->yaddr;
    isp_addr.uaddr = p_src_frame->uaddr;
    isp_addr.vaddr = p_src_frame->vaddr;    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_ADDR, 
                                    (void*)&isp_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                   ISP_PATH_INPUT_FORMAT, 
                                    (void*)&p_src_frame->type);
    ISP_RTN_IF_ERR(rtn_drv);

    isp_size.w = p_dst_frame->width;
    isp_size.h = p_dst_frame->height;    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                   ISP_PATH_OUTPUT_SIZE, 
                                    (void*)&isp_size);
    ISP_RTN_IF_ERR(rtn_drv);

    isp_addr.yaddr = p_dst_frame->yaddr;
    isp_addr.uaddr = p_dst_frame->uaddr;
    isp_addr.vaddr = p_dst_frame->vaddr;    

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_ADDR, 
                                    (void*)&isp_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                   ISP_PATH_OUTPUT_FORMAT, 
                                   (void*)&p_dst_frame->type);
    ISP_RTN_IF_ERR(rtn_drv);
    
    if(ISP_DATA_RGB565 == p_dst_frame->type)
    {
        //ISP_SERVICE_LOG:"ISP_SERVICE:_ISP_ServiceStartVTScale, dithering enabled!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5658_112_2_18_1_44_25_733,(uint8*)"");
        param = SCI_TRUE;
        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                       ISP_PATH_DITHER_EN, 
                                        (void*)&param);
        ISP_RTN_IF_ERR(rtn_drv);
    }
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                   ISP_PATH_OUTPUT_FRAME_FLAG, 
                                   (void*)&p_dst_frame->flags);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverExtStart(s->module_addr);    
    
exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5677_112_2_18_1_44_25_734,(uint8*)"d",rtn_drv);
    }
    
}
LOCAL void _ISP_ServiceStartVTReview(ISP_ADDRESS_T *p_review)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              out_size = {0};
    ISP_RECT_T              in_rect = {0};    
    uint32                  input_format = 0;
    uint32                  output_format = 0;    
    uint32                  dst_buf_id = 0;   
    ISP_ADDRESS_T           yuv420_addr = {0};
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartVTReview ,review_from = %d, y,u,v = {0x%x,0x%x,0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5696_112_2_18_1_44_25_735,(uint8*)"dddd",s->vt_review_from,p_review->yaddr,p_review->uaddr,p_review->vaddr);

    switch(s->vt_review_from)
    {
        case VP_REVIEW_LOCAL_422TO420:
        
            rtn_drv = ISP_DriverExtInit(s->module_addr);
            ISP_RTN_IF_ERR(rtn_drv);

            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_SIZE, 
                                            (void*)&s->encoder_size);
            ISP_RTN_IF_ERR(rtn_drv);

            in_rect.w = s->encoder_size.w;
            in_rect.h = s->encoder_size.h;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_RECT, 
                                            (void*)&in_rect);
            ISP_RTN_IF_ERR(rtn_drv);

            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_ADDR, 
                                            (void*)p_review);
            ISP_RTN_IF_ERR(rtn_drv);

            input_format = ISP_DATA_YUV422;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_INPUT_FORMAT, 
                                            (void*)&input_format);
            ISP_RTN_IF_ERR(rtn_drv);

            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_SIZE, 
                                            (void*)&s->encoder_size);
            ISP_RTN_IF_ERR(rtn_drv);

            if(s->enc_rotation != ISP_ROTATION_0)	
            {
                yuv420_addr.yaddr = s->encoder_rot_addr[0].yaddr;
                yuv420_addr.uaddr = s->encoder_rot_addr[0].uaddr;
            }
            else
            {
                yuv420_addr.yaddr = s->p_frame_enc_cur->yaddr;
                yuv420_addr.uaddr = s->p_frame_enc_cur->uaddr;
            }
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_OUTPUT_ADDR, 
                                            (void*)&yuv420_addr);
            ISP_RTN_IF_ERR(rtn_drv);
	
            output_format = ISP_DATA_YUV420;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_FORMAT, 
                                           (void*)&output_format);
            ISP_RTN_IF_ERR(rtn_drv);

            output_format = SCI_FALSE;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_DITHER_EN, 
                                           (void*)&output_format);
            ISP_RTN_IF_ERR(rtn_drv);
            
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_FRAME_FLAG, 
                                           (void*)&s->vt_review_from);
            ISP_RTN_IF_ERR(rtn_drv);
		

            s->vt_local_encode_count ++;
            rtn_drv = ISP_DriverExtStart(s->module_addr);

			
        break;
		
        case VP_REVIEW_LOCAL_422TORGB:
        
            rtn_drv = ISP_DriverExtInit(s->module_addr);
            ISP_RTN_IF_ERR(rtn_drv);

            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_SIZE, 
                                            (void*)&s->encoder_size);
            ISP_RTN_IF_ERR(rtn_drv);

            in_rect.w = s->encoder_size.w;
            in_rect.h = s->encoder_size.h;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_RECT, 
                                            (void*)&in_rect);
            ISP_RTN_IF_ERR(rtn_drv);

            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_ADDR, 
                                            (void*)p_review);
            ISP_RTN_IF_ERR(rtn_drv);

            input_format = ISP_DATA_YUV422;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_INPUT_FORMAT, 
                                            (void*)&input_format);
            ISP_RTN_IF_ERR(rtn_drv);

            out_size.w = s->display_range.w;
            out_size.h = s->display_range.h;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_SIZE, 
                                            (void*)&out_size);
            ISP_RTN_IF_ERR(rtn_drv);

            dst_buf_id = s->vt_local_display_count % ISP_PATH2_FRAME_COUNT_MAX ;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_OUTPUT_ADDR, 
                                            (void*)&s->display_addr[dst_buf_id]);
            ISP_RTN_IF_ERR(rtn_drv);
	
            output_format = ISP_DATA_RGB565;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_FORMAT, 
                                           (void*)&output_format);
            ISP_RTN_IF_ERR(rtn_drv);

            output_format = SCI_TRUE;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_DITHER_EN, 
                                           (void*)&output_format);
            ISP_RTN_IF_ERR(rtn_drv);
            
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_FRAME_FLAG, 
                                           (void*)&s->vt_review_from);
            ISP_RTN_IF_ERR(rtn_drv);
		
            s->vt_local_display_count ++;
            rtn_drv = ISP_DriverExtStart(s->module_addr);

			
        break;		
		
        case VP_REVIEW_REMOTE_420TORGB:
  
            rtn_drv = ISP_DriverExtInit(s->module_addr);
            ISP_RTN_IF_ERR(rtn_drv);
	
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_SIZE, 
                                            (void*)&s->remote_input_size);
            ISP_RTN_IF_ERR(rtn_drv);

            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_RECT, 
                                            (void*)&s->remote_input_range);
            ISP_RTN_IF_ERR(rtn_drv);

            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_INPUT_ADDR, 
                                            (void*)p_review);
            ISP_RTN_IF_ERR(rtn_drv);

            input_format = ISP_DATA_YUV420;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_INPUT_FORMAT, 
                                            (void*)&input_format);
            ISP_RTN_IF_ERR(rtn_drv);

            out_size.w = s->remote_display_range.w;
            out_size.h = s->remote_display_range.h;
    
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_SIZE, 
                                            (void*)&out_size);
            ISP_RTN_IF_ERR(rtn_drv);

            dst_buf_id = s->vt_review_remote_count % ISP_PATH2_FRAME_COUNT_MAX ;
			
		    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartVTReview ,display_buffer 0x%x"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5873_112_2_18_1_44_25_736,(uint8*)"d",s->remote_display_addr[dst_buf_id].yaddr);
			
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_OUTPUT_ADDR, 
                                            (void*)&s->remote_display_addr[dst_buf_id]);
            ISP_RTN_IF_ERR(rtn_drv);
	
            output_format = ISP_DATA_RGB565;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_FORMAT, 
                                           (void*)&output_format);
            ISP_RTN_IF_ERR(rtn_drv);

            output_format = SCI_TRUE;
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_DITHER_EN, 
                                           (void*)&output_format);
            ISP_RTN_IF_ERR(rtn_drv);
            
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                           ISP_PATH_OUTPUT_FRAME_FLAG, 
                                           (void*)&s->vt_review_from);
            ISP_RTN_IF_ERR(rtn_drv);
		
            s->vt_review_remote_count ++;
            rtn_drv = ISP_DriverExtStart(s->module_addr);
			
        break;
		
        default:
        
           
        break;
    }


exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5913_112_2_18_1_44_25_737,(uint8*)"d",rtn_drv);
    }
	
}

LOCAL void _ISP_ServiceStartVTPlayBack(void *ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_ADDRESS_T           addr_t = {0};
    uint32                  mem_start = (uint32)s->p_work_mem_cur;
    uint32                  mem_size = s->work_mem_res_len;
    BOOLEAN                 rtn = SCI_FALSE;
    
    SCI_MEMCPY((void*)&addr_t,ptr,sizeof(ISP_ADDRESS_T));

    //ISP_SERVICE_TRACE:"ISP_SERVICE: To display remote frame,s->remote_review_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5929_112_2_18_1_44_25_738,(uint8*)"d",s->remote_review_enable);

    if(s->remote_review_enable)
    {
        if(SCI_SUCCESS == _ISP_ServiceEnterVTReview())
        {
            //ISP_SERVICE_LOG:"ISP_SERVICE:display the remote frame"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5935_112_2_18_1_44_25_739,(uint8*)"");
            rtn = _ISP_ServiceCheckMemAddr(&addr_t, 
                                           &s->remote_input_size,
                                           ISP_IMAGE_FORMAT_YUV420,
                                           mem_start,
                                           &mem_size);
            if(SCI_FALSE == rtn)
            {
                //ISP_SERVICE_TRACE:"ISP_SERVICE: No mem for remote in buffer"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5943_112_2_18_1_44_25_740,(uint8*)"");
                return ;
            }
            _ISP_ServiceSetVPFlag(VP_REVIEW_REMOTE_420TORGB);
            _ISP_ServiceStartVTReview(&addr_t);
        }
        else
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: timeout, don't display the remote frame"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5951_112_2_18_1_44_25_741,(uint8*)"");
        }
    }

}

LOCAL void _ISP_ServiceStartVTExtScale(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             in_frame_t = {0};
    ISP_FRAME_T             out_frame_t = {0};
    ISP_RECT_T              isp_rect = {0};
    ISP_ADDRESS_T           addr_t = {0};
    ISP_SIZE_T              isp_size = {0};
    uint32                  mem_start = (uint32)s->p_work_mem_cur;
    uint32                  mem_size = s->work_mem_res_len;
    BOOLEAN                 rtn = SCI_FALSE;
    
    //ISP_SERVICE_LOG:"ISP_SERVICE: To do image scaling"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5969_112_2_18_1_44_25_742,(uint8*)"");

    if(ISP_STATE_STOP == s->state)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: state error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5973_112_2_18_1_44_25_743,(uint8*)"");
        return ;
    }
    if(SCI_SUCCESS == _ISP_ServiceEnterVTReview())
    {
        isp_rect.x = s->remote_input_range.x;
        isp_rect.y = s->remote_input_range.y;
        isp_rect.w = s->remote_input_range.w;
        isp_rect.h = s->remote_input_range.h;
        addr_t.yaddr = s->remote_input_addr.yaddr;
        addr_t.uaddr = s->remote_input_addr.uaddr;
        addr_t.vaddr = s->remote_input_addr.vaddr;
        rtn = _ISP_ServiceCheckMemAddr(&addr_t, 
                                       &s->remote_input_size,
                                       s->path2_input_image_format,
                                       mem_start,
                                       &mem_size); 
        if(rtn == SCI_FALSE)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: NO memory for input!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_5992_112_2_18_1_44_25_744,(uint8*)"");
            return ;        
        }
        else
        {
            mem_start += mem_size;
            mem_size = s->work_mem_res_len - mem_size;
        }
        
        in_frame_t.type = s->path2_input_image_format;
        in_frame_t.yaddr = addr_t.yaddr;
        in_frame_t.uaddr = addr_t.uaddr;
        in_frame_t.vaddr = addr_t.vaddr;
        
        in_frame_t.width = s->remote_input_size.w;
        in_frame_t.height = s->remote_input_size.h;
        out_frame_t.flags = VP_EX_REMOTE_SCALE;
        
        if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565_EX)
        {
            out_frame_t.type = ISP_IMAGE_FORMAT_RGB565;            
        }
        else
        {
            out_frame_t.type = s->path2_output_image_format;
        }
        //ISP_SERVICE_LOG:"ISP_SERVICE:transform the image frame from %d to %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6020_112_2_18_1_44_25_745,(uint8*)"dd",in_frame_t.flags,out_frame_t.flags);
        
        out_frame_t.width = s->remote_display_range.w;
        out_frame_t.height = s->remote_display_range.h;
        isp_size.w = out_frame_t.width;
        isp_size.h = out_frame_t.height;        
        addr_t.yaddr = s->output_addr.yaddr;
        addr_t.uaddr = s->output_addr.uaddr;
        addr_t.vaddr = s->output_addr.vaddr;
        rtn = _ISP_ServiceCheckMemAddr(&addr_t, 
                                       &isp_size,
                                       s->path2_output_image_format,
                                       mem_start,
                                       &mem_size); 
        if(rtn == SCI_FALSE)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: NO memory for output!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6036_112_2_18_1_44_25_746,(uint8*)"");
            return ;        
        }

        out_frame_t.yaddr = addr_t.yaddr;
        out_frame_t.uaddr = addr_t.uaddr;
        out_frame_t.vaddr = addr_t.vaddr;

        _ISP_ServiceStartVTScale(&in_frame_t, &isp_rect, &out_frame_t);

    }
    else
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6049_112_2_18_1_44_25_747,(uint8*)"");
    }  
                
}
LOCAL void _ISP_ServiceStartVTLocalPlayBack(ISP_FRAME_T *ptr)
{
    ISP_FRAME_T             *addr_ptr = (ISP_FRAME_T*)ptr;
    ISP_ADDRESS_T           isp_addr = {0};

    if(SCI_SUCCESS == _ISP_ServiceEnterVTReview())
    {
        isp_addr.yaddr = addr_ptr->yaddr;
        isp_addr.uaddr = addr_ptr->uaddr;
        isp_addr.vaddr = addr_ptr->vaddr;
        //ISP_SERVICE_LOG:"ISP_SERVICE:transform the local frame from yuv422 to rgb565"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6063_112_2_18_1_44_25_748,(uint8*)"");
        //ISP_SERVICE_LOG:"ISP_SERVICE:p_frame = 0x%x,y,u,v {0x%x,0x%x,0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6068_112_2_18_1_44_25_749,(uint8*)"dddd",addr_ptr,isp_addr.yaddr,isp_addr.uaddr,isp_addr.vaddr);
        _ISP_ServiceSetVPFlag(VP_REVIEW_LOCAL_422TORGB);
        _ISP_ServiceStartVTReview(&isp_addr);
    }
    else
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: timeout, couldn't convert yuv422 to rgb565"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6074_112_2_18_1_44_25_750,(uint8*)"");
    }

}

LOCAL void _ISP_ServiceTimeout(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    switch(s->service)    
    {
        case ISP_SERVICE_VP:
            
            s->is_first_frame = 1;  // stop review frame from remote ,see ISP_ServiceSetVTDecodeAddr
            //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceVTTimeout, to restart VT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6088_112_2_18_1_44_25_751,(uint8*)"");
            _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_VP,0);

        break;
        
        default:
        	
        break;
    }
}

LOCAL void _ISP_ServiceTimerExpireHandle(uint32 param)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  per_lev = ISP_SERVICE_PERFORMANCE_HIGH;	
    switch(s->timer_mode)
    {
        case ISP_SERVICE_PATH1_TIMEOUT:	

            per_lev = _ISP_ServiceGetPerformanceLevel();

            //ISP_SERVICE_TRACE:"ISP_SERVICE: performance_level = %d, watchdog_feeded = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6111_112_2_18_1_44_25_752,(uint8*)"dd",per_lev,s->watchdog_feeded);

            if(s->sys_performance_level != per_lev)
            {
                if(s->watchdog_feeded)
                {
                    s->watchdog_feeded = 0;
                    ISP_DriverCapConfig(s->module_addr, 
                                        ISP_CAP_FRM_DECI, 
                                        (void*)&s->sys_performance_level);
                    s->sys_performance_level = per_lev;
                }
                else
                {
                    _ISP_ServiceTimeout();
                }
            }
            
        break;
        
        default:
            //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceTimerExpireHandle,unsupported timeout mode"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6132_112_2_18_1_44_25_753,(uint8*)"");
        break;
    }
}


LOCAL BOOLEAN _ISP_ServiceCreateTimer(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    BOOLEAN                 ret_val = SCI_FALSE;
	
    s->timer_ptr = SCI_CreatePeriodTimer("ISP_VT_TIMER",
                                         _ISP_ServiceTimerExpireHandle,
                                         0,
                                         ISP_SERVICE_TIMEOUT,
                                         SCI_NO_ACTIVATE);
						
    if(PNULL != s->timer_ptr)
    {
        ret_val = SCI_TRUE;
    }
    else
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE : _ISP_VTCreateTimer, ret_val %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6155_112_2_18_1_44_26_754,(uint8*)"d", ret_val);
    }
    
    return ret_val;
	
}

LOCAL void _ISP_ServiceCloseTimer(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    
    if(PNULL != s->timer_ptr)
    {
        if( SCI_IsTimerActive(s->timer_ptr))
        {
            SCI_DeactiveTimer(s->timer_ptr);
            s->timer_mode = ISP_SERVICE_TIMER_MODE_MAX;
            //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceCloseTimer"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6172_112_2_18_1_44_26_755,(uint8*)"");
        }
    }
	
}

LOCAL void _ISP_ServiceDeleteTimer(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
        		
    if(PNULL != s->timer_ptr)
    {
        SCI_DeleteTimer(s->timer_ptr);
        s->timer_ptr = PNULL;
		
        //ISP_SERVICE_TRACE:"_DCAMERA_DeleteTimer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6187_112_2_18_1_44_26_756,(uint8*)"");
    }
}

LOCAL BOOLEAN _ISP_ServiceStartTimer(uint32 timer_mode,
                                     uint32 expire_value)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    BOOLEAN                 ret_value = SCI_FALSE;
	
    if(PNULL != s->timer_ptr)
    {
        if(SCI_SUCCESS == SCI_ChangeTimer(s->timer_ptr,
                                          _ISP_ServiceTimerExpireHandle,
                                          expire_value))
        {
            SCI_ActiveTimer(s->timer_ptr);
            s->timer_mode = timer_mode;
            ret_value = SCI_TRUE;
        }
    }
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceStartTimer,: mode %d, expire value %d, ret_val, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6212_112_2_18_1_44_26_757,(uint8*)"ddd", timer_mode, expire_value, ret_value);
	
    return ret_value;
}	

LOCAL void _ISP_ServiceStartPreview(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  i = 0;
    ISP_CAP_SYNC_POL_T      cap_sync = {0};
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              disp_size = {0};
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6225_112_2_18_1_44_26_758,(uint8*)"");

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverModuleInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverSetClk(GR_PLL_SCR,ISP_CLK_64M);
    ISP_RTN_IF_ERR(rtn_drv);

    if(ISP_DATA_YUV422 == s->display_block.img_fmt)
    {
        rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_PREVIEW);
    }
    else
    {
        rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_PREVIEW_EX);    
    }

    ISP_RTN_IF_ERR(rtn_drv);
	
    /*Set CAP*/
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_PRE_SKIP_CNT, 
                                  (void*)&s->preview_skip_frame_num);
    ISP_RTN_IF_ERR(rtn_drv);
		
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_DECI, 
                                  (void*)&s->preview_deci_frame_num);
    ISP_RTN_IF_ERR(rtn_drv);

    cap_sync.vsync_pol = s->vsync_polarity;
    cap_sync.hsync_pol = s->hsync_polarity;
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_SYNC_POL, 
                                  (void*)&cap_sync);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_FORMAT, 
                                  (void*)&s->cap_input_image_format);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_YUV_TYPE, 
                                  (void*)&s->cap_input_image_patten);	
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_RECT, 
                                  (void*)&s->cap_input_range);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_COUNT_CLR, 
                                  NULL);
    ISP_RTN_IF_ERR(rtn_drv);
	
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_IMAGE_XY_DECI, 
                                  (void*)&s->cap_img_dec);
    ISP_RTN_IF_ERR(rtn_drv);

    disp_size.w = s->display_range.w;
    disp_size.h = s->display_range.h;

    if(ISP_DATA_YUV422 == s->display_block.img_fmt)
    {
        /*Set Path1*/
        rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                        ISP_PATH_INPUT_SIZE, 
                                        (void*)&s->input_size);
        ISP_RTN_IF_ERR(rtn_drv);

        rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                        ISP_PATH_INPUT_RECT, 
                                        (void*)&s->input_range);
        ISP_RTN_IF_ERR(rtn_drv);

    
        rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                        ISP_PATH_OUTPUT_SIZE, 
                                        (void*)&disp_size);
        ISP_RTN_IF_ERR(rtn_drv);

		
        for(i = 0; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
        {
            rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                            ISP_PATH_OUTPUT_ADDR, 
                                            (void*)&s->display_addr[i]);
            ISP_RTN_IF_ERR(rtn_drv);
        }

        /*Register ISR callback*/
        rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                           ISP_IRQ_NOTICE_PATH1_DONE,
                                           _ISP_ServiceOnPath1);
        ISP_RTN_IF_ERR(rtn_drv);
    }
    else
    {
        /*Set Path2*/
        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_INPUT_SIZE, 
                                        (void*)&s->input_size);
        ISP_RTN_IF_ERR(rtn_drv);

        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_INPUT_RECT, 
                                        (void*)&s->input_range);
        ISP_RTN_IF_ERR(rtn_drv);

        s->path2_output_image_format = ISP_DATA_RGB565;
        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_OUTPUT_FORMAT, 
                                        (void*)&s->path2_output_image_format);
        ISP_RTN_IF_ERR(rtn_drv);

        i = SCI_TRUE;
        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_DITHER_EN, 
                                        (void*)&i);
        ISP_RTN_IF_ERR(rtn_drv);

        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_OUTPUT_SIZE, 
                                        (void*)&disp_size);
        ISP_RTN_IF_ERR(rtn_drv);

        for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i ++)
        {
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_OUTPUT_ADDR, 
                                            (void*)&s->display_addr[i]);
            ISP_RTN_IF_ERR(rtn_drv);
        }
		

        /*Register ISR callback*/
        rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                           ISP_IRQ_NOTICE_PATH2_DONE,
                                           _ISP_ServiceOnPath2);        
    }

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_CAP_FIFO_OF,
                                       _ISP_ServiceOnCAPFifoOverflow);      
    ISP_RTN_IF_ERR(rtn_drv);
    
    
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_LINE_ERR,
                                       _ISP_ServiceOnSensorLineErr);    
    ISP_RTN_IF_ERR(rtn_drv);    

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_FRAME_ERR,
                                       _ISP_ServiceOnSensorFrameErr);      
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6396_112_2_18_1_44_26_759,(uint8*)"d",rtn_drv);
    }
    else
    {
        s->state = ISP_STATE_PREVIEW;
    }

    return ;
	
}

LOCAL void _ISP_ServiceStartJpeg(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_CAP_SYNC_POL_T      cap_sync = {0};
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              output_size = {0};
    uint32                  cap_input_image_format = 0;
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartJpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6415_112_2_18_1_44_26_760,(uint8*)"");

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverModuleInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetClk(GR_PLL_SCR,ISP_CLK_64M);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr,
                                ISP_MODE_CAPTURE);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Set CAP*/
    cap_sync.vsync_pol = s->vsync_polarity;
    cap_sync.hsync_pol = s->hsync_polarity;
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_SYNC_POL, 
                                  (void*)&cap_sync);
    ISP_RTN_IF_ERR(rtn_drv);
    
    if(ISP_IMAGE_FORMAT_JPEG == s->cap_input_image_format)
    {
        rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_JPEG_DROP_NUM, 
                                  (void*)&s->capture_skip_frame_num);
        ISP_RTN_IF_ERR(rtn_drv);
    
        cap_input_image_format = 1; //JPG mode
    }
    else
    {
        rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_PRE_SKIP_CNT, 
                                  (void*)&s->capture_skip_frame_num);
        ISP_RTN_IF_ERR(rtn_drv);

        cap_input_image_format = 0; //YCbCr mode
    }    
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_FORMAT, 
                                  (void*)&cap_input_image_format);	
    ISP_RTN_IF_ERR(rtn_drv);
	
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_YUV_TYPE, 
                                  (void*)&s->cap_input_image_patten);	
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_RECT, 
                                  (void*)&s->cap_input_range);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_COUNT_CLR, 
                                  NULL);
    ISP_RTN_IF_ERR(rtn_drv);

	
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_IMAGE_XY_DECI, 
                                  (void*)&s->cap_img_dec);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Set Path1*/
    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_INPUT_SIZE, 
                                    (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    if(s->is_slice)
    {
        output_size.w = s->cap_output_size.w;
        output_size.h = s->cap_output_size.h;    	    	       
    }
    else
    {
        output_size.w = s->encoder_size.w;
        output_size.h = s->encoder_size.h;    	    	       
    }

    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_SIZE, 
                                    (void*)&output_size);
    ISP_RTN_IF_ERR(rtn_drv);

		
    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_ADDR, 
                                    (void*)&s->encoder_addr[0]);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Register ISR callback*/
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH1_DONE,
                                       _ISP_ServiceOnPath1);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_CAP_FIFO_OF,
                                       _ISP_ServiceOnCAPFifoOverflow);      
    ISP_RTN_IF_ERR(rtn_drv);
    
    
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_JPEG_BUF_OF,
                                       _ISP_ServiceOnJpegBufOverflow);    
    ISP_RTN_IF_ERR(rtn_drv);    

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_LINE_ERR,
                                       _ISP_ServiceOnSensorLineErr);    
    ISP_RTN_IF_ERR(rtn_drv);    

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_FRAME_ERR,
                                       _ISP_ServiceOnSensorFrameErr);      
    ISP_RTN_IF_ERR(rtn_drv);
 
    rtn_drv = ISP_DriverStart(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);


exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6555_112_2_18_1_44_26_761,(uint8*)"d",rtn_drv);
    }
    else
    {
        if(0 == s->is_slice)
        {
            s->state = ISP_STATE_CAPTURE_DONE;
        }
        else
        {
            s->state = ISP_STATE_CAPTURE_SCALE;
        }
    }
    return ;
}

LOCAL void _ISP_ServiceStartMpeg(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  i = 0;
    ISP_CAP_SYNC_POL_T      cap_sync = {0};
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              disp_size = {0};

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartMpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6579_112_2_18_1_44_26_762,(uint8*)"");

    if(s->enc_rotation != ISP_ROTATION_0)	
    {
        _ISP_ServiceEncodeFrameInit();
    }

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverModuleInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetClk(GR_PLL_SCR,ISP_CLK_64M);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr,
                                ISP_MODE_MPEG);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set CAP*/
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_PRE_SKIP_CNT, 
                                  (void*)&s->preview_skip_frame_num);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_DECI, 
                                  (void*)&s->preview_deci_frame_num);
    ISP_RTN_IF_ERR(rtn_drv);

    cap_sync.vsync_pol = s->vsync_polarity;
    cap_sync.hsync_pol = s->hsync_polarity;
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_SYNC_POL, 
                                  (void*)&cap_sync);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_FORMAT, 
                                  (void*)&s->cap_input_image_format);
    ISP_RTN_IF_ERR(rtn_drv);
	
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_YUV_TYPE, 
                                  (void*)&s->cap_input_image_patten);	
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_RECT, 
                                  (void*)&s->cap_input_range);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_COUNT_CLR, 
                                  NULL);
    ISP_RTN_IF_ERR(rtn_drv);

	
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_IMAGE_XY_DECI, 
                                  (void*)&s->cap_img_dec);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Set Path1*/
    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_INPUT_SIZE, 
                                    (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    disp_size.w = s->display_range.w;
    disp_size.h = s->display_range.h;
    
    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_SIZE, 
                                    (void*)&disp_size);
    ISP_RTN_IF_ERR(rtn_drv);


    for(i = 0; i < ISP_PATH1_FRAME_COUNT_MAX; i ++)
    {
        rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                        ISP_PATH_OUTPUT_ADDR, 
                                        (void*)&s->display_addr[i]);
        ISP_RTN_IF_ERR(rtn_drv);
    }

    /*Set Path2*/
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_SIZE, 
                                    (void*)&s->remote_input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&s->remote_input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_FORMAT, 
                                    (void*)&s->path2_output_image_format);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_SIZE, 
                                    (void*)&s->encoder_size);
    ISP_RTN_IF_ERR(rtn_drv);

    for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i ++)
    {
        if(s->enc_rotation != ISP_ROTATION_0)	
        {
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_OUTPUT_ADDR, 
                                            (void*)&s->encoder_rot_addr[i]);
        }
        else
        {
            rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                            ISP_PATH_OUTPUT_ADDR, 
                                            (void*)&s->encoder_addr[i]);
        }
        ISP_RTN_IF_ERR(rtn_drv);
    }
		

    /*Register ISR callback*/
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH1_DONE,
                                       _ISP_ServiceOnPath1);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH2_DONE,
                                       _ISP_ServiceOnPath2);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_CAP_FIFO_OF,
                                       _ISP_ServiceOnCAPFifoOverflow);      
    ISP_RTN_IF_ERR(rtn_drv);
    
    
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_LINE_ERR,
                                       _ISP_ServiceOnSensorLineErr);    
    ISP_RTN_IF_ERR(rtn_drv);    

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_FRAME_ERR,
                                       _ISP_ServiceOnSensorFrameErr);      
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6746_112_2_18_1_44_27_763,(uint8*)"d",rtn_drv);
    }
    else
    {
        s->state = ISP_STATE_MPEG;
    }

    return ;   
}


LOCAL void _ISP_ServiceStartReview(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              disp_size = {0};
    uint32                  param = 0;
        
    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartReview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6764_112_2_18_1_44_27_764,(uint8*)"");

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverModuleInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetClk(GR_PLL_SCR,ISP_CLK_64M);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr,
                                ISP_MODE_REVIEW);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path2*/
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_SIZE, 
                                    (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_ADDR, 
                                    (void*)&s->input_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_FORMAT, 
                                    (void*)&s->path2_input_image_format);
    ISP_RTN_IF_ERR(rtn_drv);

    disp_size.w = s->display_range.w;
    disp_size.h = s->display_range.h;
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_SIZE, 
                                    (void*)&disp_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_ADDR, 
                                    (void*)&s->display_addr[s->display_frame_count%ISP_REVIEW_FRAME_NUM]);
    ISP_RTN_IF_ERR(rtn_drv);

    if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565)
    {
        param = SCI_TRUE;
        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_DITHER_EN, 
                                        (void*)&param);
        ISP_RTN_IF_ERR(rtn_drv);        
    }
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_FORMAT, 
                                    (void*)&s->path2_output_image_format);
    ISP_RTN_IF_ERR(rtn_drv);
		

    /*Register ISR callback*/

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH2_DONE,
                                       _ISP_ServiceOnPath2);
    ISP_RTN_IF_ERR(rtn_drv);

    _ISP_ServiceSyncImageData(&s->input_addr, 
                              s->input_size.w, 
                              s->input_size.h, 
                              s->path2_input_image_format, 
                              DMABUFFER_TO_DEVICE);

    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6850_112_2_18_1_44_27_765,(uint8*)"d",rtn_drv);
    }
    else
    {
        s->state = ISP_STATE_REVIEW;                            
    }

	
}

LOCAL void _ISP_ServiceContinueReviewing(void* addr_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    uint32                  mem_size = s->work_mem_res_len;
    BOOLEAN                 rtn = SCI_FALSE;

    rtn_drv = ISP_DriverExtInit(s->module_addr);    
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_ADDR, 
                                    (void*)&s->display_addr[s->display_frame_count%ISP_REVIEW_FRAME_NUM]);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn = _ISP_ServiceCheckMemAddr((ISP_ADDRESS_T*)addr_ptr, 
                                    &s->input_size, 
                                    s->path2_input_image_format,
                                    (uint32)s->p_work_mem_cur,
                                    &mem_size);
    if(SCI_TRUE == rtn)
    {
        rtn_drv = 0;
    }
    else
    {
        rtn_drv = ISP_DRV_RTN_PARA_ERR;    
    }
    ISP_RTN_IF_ERR(rtn_drv);
       
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_ADDR, 
                                    (void*)addr_ptr);
    ISP_RTN_IF_ERR(rtn_drv);

    

    rtn_drv = ISP_DriverExtStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6903_112_2_18_1_44_27_766,(uint8*)"d",rtn_drv);
    }
    else
    {
        s->state = ISP_STATE_REVIEW;
    }

    
}


LOCAL void _ISP_ServiceStartScale(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    uint32                  output_format = s->path2_output_image_format;
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartScale,input addr {0x%x 0x%x 0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_6923_112_2_18_1_44_27_767,(uint8*)"ddd",s->input_addr.yaddr,s->input_addr.uaddr,s->input_addr.vaddr);

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverModuleInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetClk(GR_PLL_SCR,ISP_CLK_64M);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr,
                                ISP_MODE_SCALE);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path2*/
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_SIZE, 
                                    (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_ADDR, 
                                    (void*)&s->input_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_FORMAT, 
                                    (void*)&s->path2_input_image_format);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_SIZE, 
                                    (void*)&s->encoder_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_ADDR, 
                                    (void*)&s->encoder_addr[0]);
    ISP_RTN_IF_ERR(rtn_drv);

    if(s->is_slice)	
    {
        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_SLICE_SCALE_EN, 
                                        (void*)SCI_NULL);
        ISP_RTN_IF_ERR(rtn_drv);
        
        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_SLICE_SCALE_HEIGHT, 
                                        (void*)&s->slice_height);
        ISP_RTN_IF_ERR(rtn_drv);

        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_SWAP_BUFF, 
                                        (void*)&s->swap_buff);
        ISP_RTN_IF_ERR(rtn_drv);

        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_LINE_BUFF, 
                                        (void*)&s->line_buff);
        ISP_RTN_IF_ERR(rtn_drv);

		
    }

    if(s->path2_output_image_format == ISP_IMAGE_FORMAT_RGB565_EX)
    {
        output_format = SCI_TRUE;
        rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                        ISP_PATH_DITHER_EN, 
                                        (void*)&output_format);
        ISP_RTN_IF_ERR(rtn_drv);        
        
        output_format = ISP_IMAGE_FORMAT_RGB565;            
    }
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_FORMAT, 
                                    (void*)&output_format);
    ISP_RTN_IF_ERR(rtn_drv);
		

    /*Register ISR callback*/

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH2_DONE,
                                       _ISP_ServiceOnPath2);
    ISP_RTN_IF_ERR(rtn_drv);

    _ISP_ServiceSyncImageData(&s->input_addr, 
                              s->input_size.w, 
                              (uint32)(s->input_range.y + s->input_range.h),
                              s->path2_input_image_format, 
                              DMABUFFER_TO_DEVICE);

    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7033_112_2_18_1_44_27_768,(uint8*)"d",rtn_drv);
    }
    else
    {
        if(s->is_slice == SCI_TRUE || s->is_slice_alone == SCI_TRUE)
        {
            s->state = ISP_STATE_SCALE_SLICE;
        }
        else
        {
            s->state = ISP_STATE_SCALE;
        }
    }
}

LOCAL void _ISP_ServiceContinueScaling(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    uint32                  dst_buf_id = 0;
    ISP_RECT_T              rect = {0};
    ISP_ADDRESS_T           next_addr = {0};

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceContinueScaling"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7056_112_2_18_1_44_28_769,(uint8*)"");
    
    rtn_drv = ISP_DriverExtInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    s->total_slice_count ++;
    next_addr.yaddr = s->input_addr.yaddr;
    next_addr.uaddr = s->input_addr.uaddr;
    
    if(s->is_slice_alone == SCI_FALSE)
    {
        if(ISP_IMAGE_FORMAT_YUV422 == s->path2_input_image_format || 
           ISP_IMAGE_FORMAT_YUV420 == s->path2_input_image_format)
        {
            next_addr.yaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w;
            if(ISP_IMAGE_FORMAT_YUV420 == s->path2_input_image_format)
            {
                next_addr.uaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w / 2;
            }
            else
            {
                next_addr.uaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w;        	
            }
        }
        else if(ISP_IMAGE_FORMAT_RGB888 == s->path2_input_image_format)
        {
            next_addr.yaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w * 4;    
        }
        else// RGB565
        {
            next_addr.yaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w * 2;    
        }
    }

    rect.x = s->input_range.x;
    rect.y = 0;
    rect.w = s->input_range.w;	
    rect.h = s->input_range.h;

    //ISP_SERVICE_LOG:"ISP_SERVICE:_ISP_ServiceContinueScaling,input addr {0x%x, 0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7097_112_2_18_1_44_28_770,(uint8*)"dd",next_addr.yaddr,next_addr.uaddr);
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_ADDR, 
                                    (void*)&next_addr);
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&rect);
    ISP_RTN_IF_ERR(rtn_drv);

    if(s->state == ISP_STATE_CAPTURE_SCALE)
    {
        dst_buf_id = 0;
    }
    else
    {
        dst_buf_id = s->total_slice_count % ISP_PATH2_FRAME_COUNT_MAX;
    }

    //ISP_SERVICE_LOG:"ISP_SERVICE:_ISP_ServiceContinueScaling,encoder_addr {0x%x, 0x%x} ,total_slice_count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7121_112_2_18_1_44_28_771,(uint8*)"ddd",s->encoder_addr[dst_buf_id].yaddr,s->encoder_addr[dst_buf_id].uaddr,s->total_slice_count);
    
    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_ADDR, 
                                    (void*)&s->encoder_addr[dst_buf_id]);
    ISP_RTN_IF_ERR(rtn_drv);

    if(SCI_FALSE == s->is_slice_alone)
    {
        if(s->total_slice_count == s->input_range.h / s->slice_height)
        {
            s->slice_height = s->input_range.h - s->total_slice_count * s->slice_height;
            //ISP_SERVICE_LOG:"ISP_SERVICE:_ISP_ServiceContinueScaling,slice_height %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7133_112_2_18_1_44_28_772,(uint8*)"d",s->slice_height);
        }
    }

    rtn_drv = ISP_DriverPath2Config(s->module_addr, 
                                    ISP_PATH_SLICE_SCALE_HEIGHT, 
                                    (void*)&s->slice_height);
    ISP_RTN_IF_ERR(rtn_drv);
    
    _ISP_ServiceSyncImageData(&next_addr, 
                              s->input_size.w, 
                              s->input_range.h,
                              s->path2_input_image_format, 
                              DMABUFFER_TO_DEVICE);
    
    rtn_drv = ISP_DriverExtStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7154_112_2_18_1_44_28_773,(uint8*)"d",rtn_drv);
    }
    else
    {
        s->state = ISP_STATE_SCALE_SLICE;
    }

}

LOCAL void _ISP_ServiceStartVTEncode(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  i = 0;
    ISP_CAP_SYNC_POL_T      cap_sync = {0};
    int32                   rtn_drv = SCI_SUCCESS;
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartVTEncode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7170_112_2_18_1_44_28_774,(uint8*)"");

    _ISP_ServiceEncodeFrameInit();
    
    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);
	
    rtn_drv = ISP_DriverModuleInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetClk(GR_PLL_SCR,ISP_CLK_64M);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_VT);
    ISP_RTN_IF_ERR(rtn_drv);
	
    /*Set CAP*/
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_PRE_SKIP_CNT, 
                                  (void*)&s->preview_skip_frame_num);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_DECI, 
                                  (void*)&s->preview_deci_frame_num);
    ISP_RTN_IF_ERR(rtn_drv);

    cap_sync.vsync_pol = s->vsync_polarity;
    cap_sync.hsync_pol = s->hsync_polarity;
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_SYNC_POL, 
                                  (void*)&cap_sync);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_FORMAT, 
                                  (void*)&s->cap_input_image_format);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_YUV_TYPE, 
                                  (void*)&s->cap_input_image_patten);	
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_RECT, 
                                  (void*)&s->cap_input_range);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_COUNT_CLR, 
                                  NULL);
    ISP_RTN_IF_ERR(rtn_drv);
	
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_IMAGE_XY_DECI, 
                                  (void*)&s->cap_img_dec);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Set Path1*/
    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_INPUT_SIZE, 
                                    (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_SIZE, 
                                    (void*)&s->encoder_size);
    ISP_RTN_IF_ERR(rtn_drv);

		
    for(i = 0; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
    {
        rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                        ISP_PATH_OUTPUT_ADDR, 
                                        (void*)&s->local_addr[i]);
        ISP_RTN_IF_ERR(rtn_drv);
    }

    /*Register ISR callback*/
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH1_DONE,
                                       _ISP_ServiceOnPath1);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH2_DONE,
                                       _ISP_ServiceOnPath2);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_CAP_SOF,
                                       _ISP_ServiceOnCAPSOF);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_CAP_FIFO_OF,
                                       _ISP_ServiceOnCAPFifoOverflow);      
    ISP_RTN_IF_ERR(rtn_drv);
    
    
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_LINE_ERR,
                                       _ISP_ServiceOnSensorLineErr);    
    ISP_RTN_IF_ERR(rtn_drv);    

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_FRAME_ERR,
                                       _ISP_ServiceOnSensorFrameErr);      
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7295_112_2_18_1_44_28_775,(uint8*)"d",rtn_drv);
    }
    else
    {
        s->state = ISP_STATE_VP;
    }

    return ;
}

LOCAL void _ISP_ServiceStartVTExtEncode(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  i = 0;
    ISP_CAP_SYNC_POL_T      cap_sync = {0};
    int32                   rtn_drv = SCI_SUCCESS;
	
    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartVTExtEncode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7312_112_2_18_1_44_28_776,(uint8*)"");

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);
	
    rtn_drv = ISP_DriverModuleInit(s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetClk(GR_PLL_SCR,ISP_CLK_64M);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_VT);
    ISP_RTN_IF_ERR(rtn_drv);
	
    /*Set CAP*/
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_PRE_SKIP_CNT, 
                                  (void*)&s->preview_skip_frame_num);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_DECI, 
                                  (void*)&s->preview_deci_frame_num);
    ISP_RTN_IF_ERR(rtn_drv);

    cap_sync.vsync_pol = s->vsync_polarity;
    cap_sync.hsync_pol = s->hsync_polarity;
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_SYNC_POL, 
                                  (void*)&cap_sync);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_FORMAT, 
                                  (void*)&s->cap_input_image_format);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_YUV_TYPE, 
                                  (void*)&s->cap_input_image_patten);	
    ISP_RTN_IF_ERR(rtn_drv);
	
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_INPUT_RECT, 
                                  (void*)&s->cap_input_range);

    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_FRM_COUNT_CLR, 
                                  NULL);
    ISP_RTN_IF_ERR(rtn_drv);
	
    rtn_drv = ISP_DriverCapConfig(s->module_addr, 
                                  ISP_CAP_IMAGE_XY_DECI, 
                                  (void*)&s->cap_img_dec);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Set Path1*/
    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_INPUT_SIZE, 
                                    (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_INPUT_RECT, 
                                    (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                    ISP_PATH_OUTPUT_SIZE, 
                                    (void*)&s->encoder_size);
    ISP_RTN_IF_ERR(rtn_drv);

		
    for(i = 0; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
    {
        rtn_drv = ISP_DriverPath1Config(s->module_addr, 
                                        ISP_PATH_OUTPUT_ADDR, 
                                        (void*)&s->local_addr[i]);
        ISP_RTN_IF_ERR(rtn_drv);
    }

    /*Register ISR callback*/
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH1_DONE,
                                       _ISP_ServiceOnPath1);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_PATH2_DONE,
                                       _ISP_ServiceOnPath2);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_CAP_SOF,
                                       _ISP_ServiceOnCAPSOF);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_LINE_ERR,
                                       _ISP_ServiceOnSensorLineErr);    
    ISP_RTN_IF_ERR(rtn_drv);    

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr, 
                                       ISP_IRQ_NOTICE_SENSOR_FRAME_ERR,
                                       _ISP_ServiceOnSensorFrameErr);      
    ISP_RTN_IF_ERR(rtn_drv);
    
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7429_112_2_18_1_44_28_777,(uint8*)"d",rtn_drv);
    }
    else
    {
        s->state = ISP_STATE_VP;
    }

    return ;
}
LOCAL void _ISP_ServiceSetCompoundJpegFlag(BOOLEAN is_on)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    s->jpeg_capture_is_composed = is_on;
}

LOCAL BOOLEAN _ISP_ServiceGetCompoundJpegFlag(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    return s->jpeg_capture_is_composed;
}


LOCAL uint32 _ISP_ServiceGetXYDeciFactor(uint32* src_width, 
                                         uint32* src_height,
                                         uint32  dst_width,
                                         uint32  dst_height)
{
    uint32                  i = 0;
    uint32                  width = 0;
    uint32                  height = 0;
    
    for(i = 1; i < ISP_CAP_DEC_XY_MAX + 1; i++)
    {
        width = *src_width / i;
        height = *src_height / i;
   
        if(width <= (dst_width * ISP_PATH_SC_COEFF_MAX) && 
           height <= (dst_height * ISP_PATH_SC_COEFF_MAX) &&
           (width % ISP_ALIGNED_PIXELS) == 0)
        {
            break; 
        }
        
    }

    *src_width = width;
    *src_height = height;
    
    return i;
    
}

LOCAL void _ISP_ServiceOnSensorSOF(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(ISP_SERVICE_IDLE != s->service)
    {
	
    }
    
}

LOCAL void _ISP_ServiceOnSensorEOF(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(ISP_SERVICE_IDLE != s->service)
    {
	
    }
    
}
LOCAL void _ISP_ServiceOnCAPSOF(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(ISP_SERVICE_IDLE != s->service)
    {
        s->watchdog_feeded = SCI_TRUE;
    }
	
}

LOCAL void _ISP_ServiceOnCAPEOF(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(ISP_SERVICE_IDLE != s->service)
    {
	
    }
    
}

LOCAL void _ISP_ServiceOnCAPFifoOverflow(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    ISP_DriverSoftReset(AHB_REG_BASE);
    if(ISP_SERVICE_IDLE != s->service)
    {
        _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,s->service,0);
    }
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:Cap fifo overflow, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7536_112_2_18_1_44_28_778,(uint8*)"d",s->service);
}

LOCAL void _ISP_ServiceOnSensorLineErr(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    ISP_DriverSoftReset(AHB_REG_BASE);
    if(ISP_SERVICE_IDLE != s->service)
    {
        _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,s->service,0);
    }
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:Sensor line error, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7549_112_2_18_1_44_29_779,(uint8*)"d",s->service);
}

LOCAL void _ISP_ServiceOnSensorFrameErr(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    ISP_DriverSoftReset(AHB_REG_BASE);
    if(ISP_SERVICE_IDLE != s->service)
    {
        _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,s->service,0);
    }
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:Sensor Frame error, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7562_112_2_18_1_44_29_780,(uint8*)"d",s->service);
}

LOCAL void _ISP_ServiceOnJpegBufOverflow(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    ISP_DriverSoftReset(AHB_REG_BASE);
    if(ISP_SERVICE_IDLE != s->service)
    {
        _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,s->service,0);
    }
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE:Sensor Jpeg buffer error, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7575_112_2_18_1_44_29_781,(uint8*)"d",s->service);

}
LOCAL void _ISP_ServiceOnPath1(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(ISP_SERVICE_IDLE != s->service)
    {
        _ISP_ServiceSendRequest(0,ISP_SERVICE_PATH1_SIG,0,p);
    }
}

LOCAL void _ISP_ServiceOnPath2(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;
	ISP_FRAME_T				*p_frame = SCI_NULL;
	
    if(ISP_SERVICE_IDLE != s->service && SCI_NULL != p)
    {
        if(ISP_SERVICE_VP == s->service ||
           ISP_SERVICE_VP_EX == s->service)
        {
        	p_frame = (ISP_FRAME_T*)ISP_SERVICE_ALLOC(sizeof(ISP_FRAME_T));
            SCI_MEMCPY((void*)p_frame,
                       (void*)p,
                       sizeof(ISP_FRAME_T));
			
			_ISP_ServiceExitVTReview();

	        _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,ISP_SERVICE_PATH2_SIG,0,(void*)p_frame);
        }
		else
		{
	        _ISP_ServiceSendRequest(0,ISP_SERVICE_PATH2_SIG,0,p);
		}
		
    }
}
/*
LOCAL uint32 try_times = 0;
void *ISP_ALLOC(uint32 size)
{
    try_times ++;    
    if(try_times < 5)
    {
        return (void*)0x03f00000;
    }
    else
    {

        return (void*)0x04A00000;        
    }
}

void ISP_Free(void* p_mem)
{
    return ;
}
*/

LOCAL BOOLEAN _ISP_ServiceAllocFrame(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  i = 0,j = 0;
    uint32                  frame_size = 0;
    uint32                  path1_frame_size = 0;
    uint32                  path2_frame_size = 0;
    uint32                  base_addr = 0;
    uint32	                end_addr = 0;
    uint32                  mem_length = 0;
    uint32                  frame_count = 0;
    uint32                  disp_num = 0;
    uint32                  rot_num = 0;    
    BOOLEAN                 rtn = SCI_TRUE;
    void                    *p_mem_to_be_freed[ISP_TRY_TIMES] = {0};

    base_addr = (uint32)ISP_ServiceGetMem(&mem_length);
    if(0 == base_addr)
    {
        //ISP_SERVICE_LOG:"_ISP_ServiceAllocFrame, Mem not given by caller, should alloc-ed here"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7654_112_2_18_1_44_29_782,(uint8*)"");
        
        if(s->mem_ctrl)
        {
            SCI_GetMutex(s->mem_ctrl, SCI_WAIT_FOREVER);
        }
        
        mem_length = ISP_WORK_MEM_SIZE_DEFAULT;
        for(i = 0; i < ISP_TRY_TIMES; i++)        
        {
            p_mem_to_be_freed[i] = ISP_SERVICE_ALLOC(mem_length);
            if(SCI_NULL == p_mem_to_be_freed[i])
            {
                //ISP_SERVICE_TRACE:"_ISP_ServiceAllocFrame, NO memory reserved for ISP"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7667_112_2_18_1_44_29_783,(uint8*)"");
                break;
            }    

            end_addr = (uint32)p_mem_to_be_freed[i] + mem_length;      
            if(ISP_MEM_IS_IN_SAME_SEGE(p_mem_to_be_freed[i], end_addr))
            {
                s->p_work_mem = p_mem_to_be_freed[i];
                s->work_mem_len = mem_length;
                break;
            }
            //ISP_SERVICE_TRACE:"_ISP_ServiceAllocFrame, Memory not in one 64M Segement,try again"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7678_112_2_18_1_44_29_784,(uint8*)"");
        }

        for(j = 0; j < i; j++)
        {
            if(SCI_NULL != p_mem_to_be_freed[j])
            {
                SCI_Free(p_mem_to_be_freed[j]);
            }   
            //ISP_SERVICE_TRACE:"_ISP_ServiceAllocFrame, %d block mem freed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7687_112_2_18_1_44_29_785,(uint8*)"d", j);
        }
        
        if(i != ISP_TRY_TIMES && SCI_NULL != s->p_work_mem)
        {
            //ISP_SERVICE_TRACE:"_ISP_ServiceAllocFrame, Memory successfully,total try times %d ,addr 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7692_112_2_18_1_44_29_786,(uint8*)"dd",i,s->p_work_mem);
            s->mem_alloced_by_myself = SCI_TRUE;
            base_addr = (uint32)s->p_work_mem;
            mem_length = s->work_mem_len;
            s->p_work_mem_cur = s->p_work_mem;
            s->work_mem_res_len = s->work_mem_len;
        }
        if(s->mem_ctrl)
        {
            SCI_PutMutex(s->mem_ctrl);
        }
    }
    else
    {
        //ISP_SERVICE_LOG:"_ISP_ServiceAllocFrame, Mem given by caller, start 0x%x ,length 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7708_112_2_18_1_44_29_787,(uint8*)"dd",base_addr,mem_length);
        end_addr =  base_addr + mem_length;
    }

    if(0 == base_addr)
    {
        return SCI_FALSE;
    }
    
    switch(s->service)
    {
        case ISP_SERVICE_PREVIEW:
            
            frame_size = s->display_range.w * s->display_range.h;
            path1_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

            end_addr -= (path1_frame_size << 1); 
            mem_length -= (path1_frame_size << 1); 
            ISP_SET_ADDR(s->display_addr[disp_num],end_addr,frame_size);
            disp_num += 1;
            
            if(s->display_rotation != ISP_ROTATION_0) //need one rotation buffer at least
            {
                end_addr -= (path1_frame_size << 1); 
                mem_length -= (path1_frame_size << 1);    
                ISP_SET_ADDR(s->rotation_buf_addr[rot_num],end_addr,frame_size);
                rot_num += 1;
            }

            if(end_addr < base_addr)
            {
                rtn = SCI_FALSE;
                break;
            }
            
            frame_count = mem_length / (path1_frame_size << 1);
            
            if(frame_count > 0)
            {
                if(s->display_rotation != ISP_ROTATION_0) 
                {
                    end_addr -= (path1_frame_size << 1); 
                    mem_length -= (path1_frame_size << 1);    
                    ISP_SET_ADDR(s->rotation_buf_addr[rot_num],end_addr,frame_size);
                    rot_num += 1;
                    frame_count -= 1;
                }
            }
            
            if(frame_count > 0)
            {
                end_addr -= (path1_frame_size << 1); 
                mem_length -= (path1_frame_size << 1); 
                ISP_SET_ADDR(s->display_addr[disp_num],end_addr,frame_size);
                disp_num += 1;
            }

            SCI_ASSERT(disp_num);/*assert verified*/
            for(i = disp_num; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
            {
               s->display_addr[i].yaddr = s->display_addr[i % disp_num].yaddr;
               s->display_addr[i].uaddr = s->display_addr[i % disp_num].uaddr;                   
            }

            if(s->display_rotation != ISP_ROTATION_0) 
            {
                SCI_ASSERT(rot_num);/*assert verified*/
                for(i = rot_num; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
                {
                   s->rotation_buf_addr[i].yaddr = s->rotation_buf_addr[i % rot_num].yaddr;
                   s->rotation_buf_addr[i].uaddr = s->rotation_buf_addr[i % rot_num].uaddr;                   
                }
            }

            
        break;
            
        case ISP_SERVICE_JPEG:

            if(s->cap_input_image_format == ISP_IMAGE_FORMAT_JPEG)
            {
                frame_size = (s->encoder_size.w * s->encoder_size.h)/6;
                path1_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;
                mem_length = path1_frame_size;
            }
            else
            {
                frame_size = (s->encoder_size.w * s->encoder_size.h);
                path1_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;
                mem_length = path1_frame_size << 1;
            }
            
            if(s->work_mem_len < mem_length)          
            {
                rtn = SCI_FALSE;
                break;
            }
            
            ISP_SET_ADDR(s->encoder_addr[0], base_addr, frame_size);
            
        break;
            
        case ISP_SERVICE_MPEG:
            
            frame_size = s->encoder_size.w * s->encoder_size.h;

            if(s->path2_output_image_format == ISP_IMAGE_FORMAT_YUV420)
            {
                path2_frame_size = ((frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) * 3) >> 1;
            }
            else
            {
                path2_frame_size = (frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;
            }

            if(s->enc_rotation != ISP_ROTATION_0)	
            {
                ISP_SET_ADDR(s->encoder_rot_addr[0], base_addr, frame_size);
                base_addr += path2_frame_size;
                mem_length -= path2_frame_size;
            }
            for(i = 1; i < ISP_PATH2_FRAME_COUNT_MAX; i++)            
            {
                s->encoder_rot_addr[i].yaddr = s->encoder_rot_addr[0].yaddr;
                s->encoder_rot_addr[i].uaddr = s->encoder_rot_addr[0].uaddr;
            }
            
            for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i++)
            {
                ISP_SET_ADDR(s->encoder_addr[i], base_addr, frame_size);
                base_addr += path2_frame_size;
                mem_length -= path2_frame_size;
            }

            frame_size = s->display_range.w * s->display_range.h;
            path1_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

            ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size);
            disp_num += 1;
            base_addr += (path1_frame_size << 1); 
            mem_length -= (path1_frame_size << 1); 
            
            if(s->display_rotation != ISP_ROTATION_0) //need one rotation buffer at least
            {
                ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                rot_num += 1;
                base_addr += (path1_frame_size << 1); 
                mem_length -= (path1_frame_size << 1); 
            }

            if(end_addr < base_addr)
            {
                rtn = SCI_FALSE;
                break;
            }
            
            frame_count = mem_length / (path1_frame_size << 1);
            if(frame_count > 0)
            {
                if(s->display_rotation != ISP_ROTATION_0) 
                {
                    ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                    rot_num += 1;
                    base_addr += (path1_frame_size << 1); 
                    mem_length -= (path1_frame_size << 1); 
                    frame_count -= 1;
                }
            }

            if(frame_count > 0)
            {
                ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size);
                disp_num += 1;
                base_addr += (path1_frame_size << 1); 
                mem_length -= (path1_frame_size << 1);                     
            }
            SCI_ASSERT(disp_num);/*assert verified*/
            for(i = disp_num; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
            {
                s->display_addr[i].yaddr = s->display_addr[i % disp_num].yaddr;
                s->display_addr[i].uaddr = s->display_addr[i % disp_num].uaddr;                   
            }

            if(s->display_rotation != ISP_ROTATION_0) 
            {
                SCI_ASSERT(rot_num);/*assert verified*/
                for(i = rot_num; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
                {
                   s->rotation_buf_addr[i].yaddr = s->rotation_buf_addr[i % rot_num].yaddr;
                   s->rotation_buf_addr[i].uaddr = s->rotation_buf_addr[i % rot_num].uaddr;                   
                }
            }

        break;
		
        case ISP_SERVICE_REVIEW:
        
            frame_size = s->display_range.w * s->display_range.h;
            path2_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

            ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size);
            disp_num += 1;
            base_addr += (path2_frame_size << 1); 
            mem_length -= (path2_frame_size << 1);                     
            
            if(s->display_rotation != ISP_ROTATION_0) //need one rotation buffer at least
            {
                ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                rot_num += 1;
                base_addr += (path2_frame_size << 1); 
                mem_length -= (path2_frame_size << 1); 
            }  

            if(end_addr < base_addr)
            {
                rtn = SCI_FALSE;
                break;
            }
            
            frame_count = mem_length / (path2_frame_size << 1);
            if(frame_count > 0)
            {
                if(s->display_rotation != ISP_ROTATION_0) 
                {
                    ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                    rot_num += 1;
                    base_addr += (path2_frame_size << 1); 
                    mem_length -= (path2_frame_size << 1); 
                    frame_count -= 1;
                }


            }
            if(frame_count > 0)
            {
                ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size);
                disp_num += 1;
                base_addr += (path2_frame_size << 1); 
                mem_length -= (path2_frame_size << 1);                     
            }
            SCI_ASSERT(disp_num);/*assert verified*/
            for(i = disp_num; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
            {
               ISP_SET_ADDR(s->display_addr[i],s->display_addr[i % disp_num].yaddr, frame_size);        
            }

            if(s->display_rotation != ISP_ROTATION_0) 
            {
                SCI_ASSERT(rot_num);/*assert verified*/
                for(i = rot_num; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
                {
                   s->rotation_buf_addr[i].yaddr = s->rotation_buf_addr[i % rot_num].yaddr;
                   s->rotation_buf_addr[i].uaddr = s->rotation_buf_addr[i % rot_num].uaddr;                   
                }
            }            
            s->p_work_mem_cur = (void*)base_addr;
            s->work_mem_res_len = s->work_mem_len - (base_addr - (uint32)s->p_work_mem);
			
        break;
        
        case ISP_SERVICE_SCALE:

            if(s->is_slice)
            {
                if(s->line_buff.yaddr == 0 && 
                   s->swap_buff.yaddr == 0 && 
                   s->swap_buff.uaddr == 0)
                {
                    s->line_buff.yaddr = base_addr;
                    s->line_buff.yaddr = ISP_ADDRESS_ALIGNED_(s->line_buff.yaddr,ISP_ALIGNED_BYTES);	
                    s->swap_buff.yaddr = s->line_buff.yaddr + ISP_SCALE_LINE_BUF_SIZE(s->encoder_size.w);
                    s->swap_buff.yaddr = ISP_ADDRESS_ALIGNED_(s->swap_buff.yaddr,ISP_ALIGNED_BYTES);	
                    s->swap_buff.uaddr = s->swap_buff.yaddr + s->encoder_size.w * (s->input_range.y + s->slice_height);
                    s->swap_buff.uaddr += ISP_MEM_STRIDER_FOR_ONE_FRAME;
                    s->swap_buff.uaddr = ISP_ADDRESS_ALIGNED_(s->swap_buff.uaddr,ISP_ALIGNED_BYTES);	
                    base_addr = s->swap_buff.uaddr + s->encoder_size.w * (s->input_range.y + s->slice_height) + ISP_MEM_STRIDER_FOR_ONE_FRAME; 			
                    //ISP_SERVICE_TRACE:"_ISP_ServiceAllocFrame, line_buff , 0x%x swap_buff y,u, 0x%x,0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_7987_112_2_18_1_44_29_788,(uint8*)"ddd",s->line_buff.yaddr,s->swap_buff.yaddr,s->swap_buff.uaddr);
                    
                }
            }

            if(s->is_slice_alone) //come from external module
            {
                for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i++)                  	
                {
                    s->encoder_addr[i].yaddr = s->output_addr.yaddr;                 
                    s->encoder_addr[i].uaddr = s->output_addr.uaddr;                 				
                }
            }
            else
            {
                path1_frame_size = s->input_size.w * s->input_size.h;   
            
                if(s->is_slice)	// YUV data to be slice scaled is from internal module, 
                {
                    frame_size = s->encoder_size.w * s->slice_out_height;
                    path2_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;  
                    for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i++)                  	
                    {
                        ISP_SET_ADDR(s->encoder_addr[i], base_addr, frame_size);
                    }
                    base_addr += (path2_frame_size << 1);
                }
                else
                {
                    if(!ISP_ADDR_INVALID(s->output_addr.yaddr) &&
                       (s->output_addr.yaddr % ISP_ALIGNED_BYTES == 0))
                    {
                        for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i++)                  	
                        {
                            s->encoder_addr[i].yaddr = s->output_addr.yaddr;                 
                            s->encoder_addr[i].uaddr = s->output_addr.uaddr;                 				
                        }
                    }
                    else
                    {
                        if(s->input_addr.yaddr > base_addr && 
                          (s->input_addr.uaddr + path1_frame_size) < end_addr)
                        {
                            base_addr = s->input_addr.uaddr + path1_frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;
                        }//DV display last frame
                        frame_size = s->encoder_size.w * s->encoder_size.h;
                        path2_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;  
                        for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i++)                  	
                        {
                            ISP_SET_ADDR(s->encoder_addr[i], base_addr, frame_size);
                        }   
                        base_addr += (path2_frame_size << 1);
                        
                    }
                }
            }
            s->p_work_mem_cur = (void*)base_addr;
            s->work_mem_res_len = s->work_mem_len - (base_addr - (uint32)s->p_work_mem);
            
        break;
		
        case ISP_SERVICE_VP:

            path1_frame_size = s->encoder_size.w * s->encoder_size.h;
            path1_frame_size += ISP_MEM_STRIDER_FOR_ONE_FRAME;

            for(i = 0; i < ISP_PATH1_FRAME_COUNT_MAX; i ++)
            {
                s->local_addr[i].yaddr = base_addr + i * path1_frame_size * 2;
                s->local_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->local_addr[i].yaddr,
                                                               ISP_ALIGNED_BYTES);
                                                                 
                s->local_addr[i].uaddr = s->local_addr[i].yaddr + path1_frame_size;
                s->local_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->local_addr[i].uaddr,
                                                               ISP_ALIGNED_BYTES);
                s->local_addr[i].vaddr = 0;
				
            }
            base_addr += ISP_PATH1_FRAME_COUNT_MAX * path1_frame_size * 2;
            path1_frame_size = s->display_range.w * s->display_range.h;
            path1_frame_size += ISP_MEM_STRIDER_FOR_ONE_FRAME;
			
            for(i = 0; i < ISP_PATH1_FRAME_COUNT_MAX; i ++)
            {
                s->display_addr[i].yaddr = base_addr + i * path1_frame_size * 2;
                s->display_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->display_addr[i].yaddr,
                                                                 ISP_ALIGNED_BYTES);
                                                                 
                s->display_addr[i].uaddr = s->display_addr[i].yaddr + path1_frame_size;
                s->display_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->display_addr[i].uaddr,
                                                                 ISP_ALIGNED_BYTES);
                s->display_addr[i].vaddr = 0;
				
            }
            for(i = 0; i < ISP_PATH1_FRAME_COUNT_MAX; i ++)
            {
                s->rotation_buf_addr[i].yaddr = base_addr + ISP_PATH1_FRAME_COUNT_MAX * path1_frame_size * 2;
                s->rotation_buf_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->rotation_buf_addr[i].yaddr,
                                                                      ISP_ALIGNED_BYTES);
    			
                s->rotation_buf_addr[i].uaddr = s->rotation_buf_addr[i].yaddr + path1_frame_size;
                s->rotation_buf_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->rotation_buf_addr[i].uaddr,
                                                                      ISP_ALIGNED_BYTES);
                s->rotation_buf_addr[i].vaddr = 0;
            }

            base_addr += (ISP_PATH1_FRAME_COUNT_MAX + 1) * path1_frame_size * 2;
           
            path2_frame_size = s->encoder_size.w * s->encoder_size.h;
            path2_frame_size += ISP_MEM_STRIDER_FOR_ONE_FRAME;
            if(s->enc_rotation != ISP_ROTATION_0)	
            {
                s->encoder_rot_addr[0].yaddr = base_addr;          
                s->encoder_rot_addr[0].yaddr = ISP_ADDRESS_ALIGNED_(s->encoder_rot_addr[0].yaddr,
                                                                    ISP_ALIGNED_BYTES);
                s->encoder_rot_addr[0].uaddr = s->encoder_rot_addr[0].yaddr +  path2_frame_size;
                s->encoder_rot_addr[0].uaddr = ISP_ADDRESS_ALIGNED_(s->encoder_rot_addr[0].uaddr,
                                                                    ISP_ALIGNED_BYTES);
				
                s->encoder_rot_addr[0].vaddr = 0;
                
            }
            
            base_addr += path2_frame_size * 3 / 2; 
            
            for(i = 0; i < ISP_PATH2_FRAME_COUNT_MAX; i ++)
            {
                s->encoder_addr[i].yaddr = base_addr + i * path2_frame_size * 3 / 2;
                s->encoder_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->encoder_addr[i].yaddr,
                                                                 ISP_ALIGNED_BYTES);
				
                s->encoder_addr[i].uaddr = s->encoder_addr[i].yaddr +  path2_frame_size;
                s->encoder_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->encoder_addr[i].uaddr,
                                                                 ISP_ALIGNED_BYTES);
				
                s->encoder_addr[i].vaddr = 0;
            }
            base_addr += ISP_PATH2_FRAME_COUNT_MAX * path2_frame_size * 3 / 2;

            s->p_work_mem_cur = (void*)base_addr;
            s->work_mem_res_len = (uint32)(2*(ISP_QCIF_W * ISP_QCIF_H + 2 * ISP_MEM_STRIDER_FOR_ONE_FRAME));
            s->vt_mem_end_cur = base_addr + s->work_mem_res_len;

            if(s->vt_mem_end_cur > end_addr)
            {
                rtn = SCI_FALSE;
            }	
            			
        break;
        case ISP_SERVICE_VP_EX:

            frame_size = s->encoder_size.w * s->encoder_size.h;
            path1_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

            for(i = 0; i < ISP_PATH1_FRAME_COUNT_MAX; i ++)
            {
                ISP_SET_ADDR(s->local_addr[i], base_addr, frame_size);
                base_addr += (path1_frame_size << 1);
                mem_length -= (path1_frame_size << 1);
            }

            if(s->local_review_enable)            
            {
                frame_size = s->display_range.w * s->display_range.h;
                path1_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

                ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size);
                disp_num += 1;
                base_addr += (path1_frame_size << 1); 
                mem_length -= (path1_frame_size << 1); 
                
                if(s->display_rotation != ISP_ROTATION_0) //need one rotation buffer at least
                {
                    ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                    rot_num += 1;
                    base_addr += (path1_frame_size << 1); 
                    mem_length -= (path1_frame_size << 1); 
                }

                if(end_addr < base_addr)
                {
                    rtn = SCI_FALSE;
                    break;
                }
                
                frame_count = mem_length / (path1_frame_size << 1);
                if(frame_count > 0)
                {
                    if(s->display_rotation != ISP_ROTATION_0) 
                    {
                        ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                        rot_num += 1;
                        base_addr += (path1_frame_size << 1); 
                        mem_length -= (path1_frame_size << 1); 
                        
                    }

                    if(frame_count > 1)
                    {
                        ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size);
                        disp_num += 1;
                        base_addr += (path1_frame_size << 1); 
                        mem_length -= (path1_frame_size << 1);                     
                    }
                }

                SCI_ASSERT(disp_num);/*assert verified*/
                for(i = disp_num; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
                {
                   ISP_SET_ADDR(s->display_addr[i],s->display_addr[i % disp_num].yaddr, frame_size);        
                }

                if(s->display_rotation != ISP_ROTATION_0) 
                {
                    SCI_ASSERT(rot_num);/*assert verified*/
                    for(i = rot_num; i < ISP_PATH1_FRAME_COUNT_MAX; i++)
                    {
                       ISP_SET_ADDR(s->rotation_buf_addr[i],s->rotation_buf_addr[i % rot_num].yaddr, frame_size);        
                    }
                }
            }

            s->p_work_mem_cur = (void*)base_addr;
            s->work_mem_res_len = (uint32)(2*(s->encoder_size.w * s->encoder_size.h + ISP_MEM_STRIDER_FOR_ONE_FRAME));
            s->vt_mem_end_cur = base_addr + s->work_mem_res_len;
            
            if(s->vt_mem_end_cur > end_addr)
            {
                rtn = SCI_FALSE;
            }	


        break;		
        
        default:

        break;
	
    }

    return rtn;
}

LOCAL void* _ISP_ServiceGetVTExtScaleBuf(uint32  *p_buf_len,uint32 width, uint32 height, uint32 img_format)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32	                end_addr = (uint32)s->p_work_mem + s->work_mem_len;
    uint32                  scale_buf_length = 0; 

    scale_buf_length = 2 * (width * height + ISP_MEM_STRIDER_FOR_ONE_FRAME); // whatever it's YUV422(RGB565) or YUV420
        
        
    *p_buf_len = end_addr - s->vt_mem_end_cur;
    if(s->vt_mem_end_cur + scale_buf_length > end_addr)
    {   
        return SCI_NULL;
    }
    else
    {
        *p_buf_len = end_addr - s->vt_mem_end_cur;
        return (void*)s->vt_mem_end_cur;
    }
}


LOCAL uint32 _ISP_ServiceRotAngle2DrvAngle(ISP_ROTATION_E isp_angle)
{
    uint32                  angle = ROTATION_DIR_MAX;
	
    switch(isp_angle)
    {
        case ISP_ROTATION_90:
            angle = ROTATION_90;
        break;
		
        case ISP_ROTATION_270:
            angle = ROTATION_270;
        break;
		
        case ISP_ROTATION_180:
            angle = ROTATION_180;
        break;
		
        case ISP_ROTATION_MIRROR:
            angle = ROTATION_MIRROR;
        break;

        default:
        break;
    }

    return angle;
}


PUBLIC BOOLEAN ISP_ServiceIsVTOn(void)
{    
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceIsVTOn, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8286_112_2_18_1_44_30_789,(uint8*)"d", s->service);
    
    return (s->service == ISP_SERVICE_VP);
}


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
PUBLIC uint32 ISP_ServiceGetScaleSliceHeightAlign(uint32 input_width, 
                                                  uint32 output_width,
                                                  uint32 input_height, 
                                                  uint32 output_height)
{
//    uint32                  output_align_value=1;
    uint32                  subsample_shift=0x00;
	uint32                  input_len = 0;
	uint32                  output_len = 0;
	uint32                  scaling_shift = 0;

	if (output_width * input_height < output_height * input_width)
	{
		input_len = input_width;
		output_len = output_width;
	}
	else
	{
		input_len = input_height;
		output_len = output_height;
	}
	
    if((output_len*4)<input_len)
    {
        if(input_len<((output_len<<2)<<1))
        {
            subsample_shift=1;
        }
        else if(input_len<((output_len<<2)<<2))
        {
            subsample_shift=2;
        }
        else if(input_len<((output_len<<2)<<3))
        {
            subsample_shift=3;
        }
        else if(input_len<((output_len<<2)<<4))
        {
            subsample_shift=4;
        } 
    }	

	input_len >>= subsample_shift;

	/*makesure that output one line at least per slice*/
	while (input_len > output_len)
	{
		scaling_shift++;
		input_len >>= scaling_shift;
	}

	return (scaling_shift + subsample_shift);
}


/****************************************************************************************/
// Description: Get isp input width align value
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      input width
//						output width
//						data format
// output parameter:
//                      None
// return:
//          uint32: The value of align
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetInputWidthAlign(uint32 input_width, 
                                            uint32 input_height, 
                                            uint32 output_width, 
                                            uint32 output_height)
{
    uint32                  align_value=1;
    uint32                  subsample_shift=0x00;
	uint32                  input_len = 0;
	uint32                  output_len = 0;

	if (output_width * input_height < output_height * input_width)
	{
		input_len = input_width;
		output_len = output_width;
	}
	else
	{
		input_len = input_height;
		output_len = output_height;
	}
	
    if((output_len*4)<input_len)
    {
        if(input_len<((output_len<<2)<<1))
        {
            subsample_shift=1;
        }
        else if(input_len<((output_len<<2)<<2))
        {
            subsample_shift=2;
        }
        else if(input_len<((output_len<<2)<<3))
        {
            subsample_shift=3;
        }
        else if(input_len<((output_len<<2)<<4))
        {
            subsample_shift=4;
        } 
    }	

	align_value = 4 << subsample_shift;

	return align_value;
}

/****************************************************************************************/
// Description: Get isp input height align value
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      input height
//						output hieht
//						data format
// output parameter:
//                      None
// return:
//          uint32: The value of align    
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetInputHeightAlign(uint32 input_height, uint32 output_height)
{
    return 2;
}

/****************************************************************************************/
// Description: Get isp output width align value
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: The value of align 
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetOutputWidthAlign(ISP_DATA_FORMAT_E data_format)
{
    uint32                  output_align_value=0x4;
    ISP_IMAGE_FORMAT        service_data_format = (ISP_IMAGE_FORMAT)data_format;
    
    switch(service_data_format)
    {
        case ISP_IMAGE_FORMAT_RGB888:
        case ISP_IMAGE_FORMAT_RGB888_EX:            
        {
            output_align_value = 2;			
            break;
        }
        case ISP_IMAGE_FORMAT_RGB565:        
		case ISP_IMAGE_FORMAT_RGB565_EX:  
        case ISP_IMAGE_FORMAT_YUV422:
        case ISP_IMAGE_FORMAT_YUV420:
        case ISP_IMAGE_FORMAT_YUV400:
        case ISP_IMAGE_FORMAT_YUV420_3FRAME:			
        {
            output_align_value = 4;
            break;
        }
        
        default:
            break;       

    }

    return output_align_value;    
}

/****************************************************************************************/
// Description: Get isp output height align value
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: the value of align
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetOutputHeightAlign(ISP_DATA_FORMAT_E data_format)
{
    return 2;   
}

PUBLIC BOOLEAN ISP_ServiceIsScaleSupported(ISP_SCALE_PARAM_T * param_ptr)
{
	uint32                  slice_height_aligned = 0;


    //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_CheckScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8494_112_2_18_1_44_31_790,(uint8*)"");

	slice_height_aligned = ISP_ServiceGetScaleSliceHeightAlign(param_ptr->input_rect.w,
															   param_ptr->output_size.w,
														       param_ptr->input_rect.h,
														       param_ptr->output_size.h);

	if((param_ptr->input_rect.w << 10) > param_ptr->output_size.w * (ISP_PATH_SCALE_LEVEL << 10))
	{
		//ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported scaling down size"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8503_112_2_18_1_44_31_791,(uint8*)"");
		return SCI_FALSE;
	}

	if((param_ptr->input_rect.h << 10) > param_ptr->output_size.h * (ISP_PATH_SCALE_LEVEL << 10))
	{
		//ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported scaling down size"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8509_112_2_18_1_44_31_792,(uint8*)"");
		return SCI_FALSE;
	}	
    
    if((param_ptr->input_rect.w*ISP_PATH_SC_COEFF_MAX < param_ptr->output_size.w)||
       (param_ptr->input_rect.h*ISP_PATH_SC_COEFF_MAX < param_ptr->output_size.h))
    {
		//ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported scaling up size"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8516_112_2_18_1_44_31_793,(uint8*)"");
        return SCI_FALSE;
    }

	if((ISP_SERVICE_SCALE_SLICE == param_ptr->scale_mode) && (!param_ptr->is_last)		
		&& (0 != (param_ptr->slice_line & ((1 << slice_height_aligned) - 1))))
	{	
		//ISP_SERVICE_TRACE:"ISP_SERVICE: slice height is not aligned, %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8523_112_2_18_1_44_31_794,(uint8*)"d", param_ptr->slice_line);
		return SCI_FALSE;
	}
	
    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
		//ISP_SERVICE_TRACE:"ISP_SERVICE: trim rect error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8530_112_2_18_1_44_31_795,(uint8*)"");
		return SCI_FALSE;
    }

    if((param_ptr->input_rect.x % ISP_ALIGNED_PIXELS)||(param_ptr->input_rect.w % ISP_ALIGNED_PIXELS))
    {
		//ISP_SERVICE_TRACE:"ISP_SERVICE: trim rect not aligned by 4 pixels"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8536_112_2_18_1_44_31_796,(uint8*)"");
        return SCI_FALSE;
    }

    if(param_ptr->output_size.w % ISP_ALIGNED_PIXELS)
    {
		//ISP_SERVICE_TRACE:"ISP_SERVICE: the width of output_size not aligned by 4 pixels"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8542_112_2_18_1_44_31_797,(uint8*)"");
        return SCI_FALSE;
    } 

    if((ISP_SCALE_FRAME_MODE_WIDTH_TH < param_ptr->output_size.w)&&(ISP_SERVICE_SCALE_SLICE!=param_ptr->scale_mode))
    {
		//ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported scaling mode"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8548_112_2_18_1_44_31_798,(uint8*)"");
        return SCI_FALSE;
    }

    if((ISP_SERVICE_SCALE_SLICE==param_ptr->scale_mode)&&((PNULL==param_ptr->get_scale_data)))
    {
		//ISP_SERVICE_TRACE:"ISP_SERVICE: get_scale_data is NULL"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8554_112_2_18_1_44_31_799,(uint8*)"");
        return SCI_FALSE;
    } 


    if(NULL==param_ptr->input_addr.yaddr)
    {
		//ISP_SERVICE_TRACE:"ISP_SERVICE: the address of surce data is NULL"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8561_112_2_18_1_44_31_800,(uint8*)"");
        return SCI_FALSE;
    }
     
    return SCI_TRUE;
}
#define RGB565_LITTLE2HALFBIG(n)   (((uint16)(n) >> 8)|((uint16)(n) << 8))
LOCAL void _ISP_ServiceRGB565FromLittleToHalfBig(uint32 rgb565_addr,uint32 pixel_num)
{
    uint32                  i;
    uint16                  *p_rgb565 = (uint16*)rgb565_addr;
    uint16                  rgb565 = 0;

    for(i = 0; i < pixel_num; i++)
    {
	    rgb565 = *p_rgb565;
        *p_rgb565++ = RGB565_LITTLE2HALFBIG(rgb565);

    }
}

LOCAL void _ISP_ServiceRGB888FromBigToLittle(uint32 rgb888_addr, uint32 pixel_num)
{
    uint32  i;
    uint32  *p_rgb888 = (uint32*)rgb888_addr;
    uint32  rgb888 = 0;

    //ISP_SERVICE_LOG:"_ISP_ServiceRGB888FromBigToLittle, addr 0x%x, pixel_num %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8588_112_2_18_1_44_31_801,(uint8*)"dd",rgb888_addr,pixel_num);
    
    for(i = 0; i < pixel_num; i++)
    {
	    rgb888 = *p_rgb888;
        *p_rgb888++ = (((rgb888 << 24) & 0xff000000) | ((rgb888 << 8) & 0xff0000) 
                          | ((rgb888 >> 8) & 0xff00) | ((rgb888 >> 24) & 0xff));
    }
}

LOCAL void    _ISP_ServiceSyncImageData(ISP_ADDRESS_T*        p_addr, 
                                        uint32                img_width,
                                        uint32                img_height,                                        
                                        uint32                img_format, 
                                        DMABUFFER_DIRECTION_E data_dir)
{
    uint32  buffer_size = 0;

    if(ISP_IMAGE_FORMAT_RGB565 == img_format ||
       ISP_IMAGE_FORMAT_RGB565_EX == img_format)
    {
       buffer_size =  (uint32)(img_width * img_height * 2);
    }
    else if(ISP_IMAGE_FORMAT_RGB888 == img_format ||
        ISP_IMAGE_FORMAT_RGB888_EX == img_format)
    {
       buffer_size =  (uint32)(img_width * img_height * 4);
    }
    else 
    {
       buffer_size =  (uint32)(img_width * img_height);        
    }

    MMU_DmaCacheSync(p_addr->yaddr, buffer_size, data_dir);

    if(ISP_IMAGE_FORMAT_YUV422 == img_format)
    {
        MMU_DmaCacheSync(p_addr->uaddr, buffer_size, data_dir);
    }else if(ISP_IMAGE_FORMAT_YUV420 == img_format)
    {
        MMU_DmaCacheSync(p_addr->uaddr, (buffer_size >> 1), data_dir);
    }else if(ISP_IMAGE_FORMAT_YUV420_3FRAME == img_format)
    {
        MMU_DmaCacheSync(p_addr->uaddr, (buffer_size >> 2), data_dir);
        MMU_DmaCacheSync(p_addr->vaddr, (buffer_size >> 2), data_dir);        
    }
        
}

PUBLIC void ISP_ServiceMemConfig(void* p_isp_mem, uint32 mem_length)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(SCI_NULL == p_isp_mem || 
       0 == mem_length)
    {
        //ISP_SERVICE_TRACE:"ISP_ServiceMemConfig, mem_ptr 0x%x, mnem len 0x%x ,isp_status %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8647_112_2_18_1_44_31_802,(uint8*)"ddd",(uint32)p_isp_mem,mem_length,s->state);
        return ;
    }

    //ISP_SERVICE_LOG:"ISP_ServiceMemConfig, mem_ptr 0x%x, mnem len 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8653_112_2_18_1_44_31_803,(uint8*)"dd",(uint32)p_isp_mem,mem_length);
    
    if(s->mem_ctrl)
    {
        SCI_GetMutex(s->mem_ctrl, SCI_WAIT_FOREVER);
    }

    if(SCI_NULL != s->p_work_mem &&
       SCI_TRUE == s->mem_alloced_by_myself)
    {
        SCI_Free(s->p_work_mem);
    }
    s->p_work_mem = p_isp_mem;
    s->work_mem_len = mem_length;
    s->mem_alloced_by_myself = SCI_FALSE;
    
    if(s->mem_ctrl)
    {
        SCI_PutMutex(s->mem_ctrl);
    }
    return ;
}

PUBLIC void* ISP_ServiceGetMem(uint32 *p_mem_length)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    void                    *p_mem = SCI_NULL;
        
    if(s->mem_ctrl)
    {
        SCI_GetMutex(s->mem_ctrl, SCI_WAIT_FOREVER);
    }
    
    p_mem = s->p_work_mem;
    if(p_mem_length)
    {
        *p_mem_length = s->work_mem_len;
    }
    
    if(s->mem_ctrl)
    {
        SCI_PutMutex(s->mem_ctrl);
    }

    return p_mem;
}

LOCAL BOOLEAN _ISP_ServiceCheckMemAddr(ISP_ADDRESS_T  *p_addr,
                                       ISP_SIZE_T     *input_size,
                                       uint32         img_fmt,
                                       uint32         mem_start,
                                       uint32         *p_mem_len)
{
    uint32                  plane_size = 0,frame_size = 0;    
    ISP_ADDRESS_T           new_addr = {0};
    BOOLEAN                 rtn = SCI_TRUE;
    
    if(!p_addr || !input_size ||
       !mem_start || !p_mem_len)
    {
        //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, param error, p_addr 0x%x, input_size 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8715_112_2_18_1_44_31_804,(uint8*)"dd",p_addr,input_size);
        return SCI_FALSE;
    }
    
    if(ISP_IMAGE_FORMAT_MAX <= img_fmt ||
       input_size->w == 0 ||
       input_size->h == 0)
    {
        //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, param error, in_fmt %d, input_size {%d %d}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8726_112_2_18_1_44_31_805,(uint8*)"ddd",img_fmt,input_size->w,input_size->h);
        return SCI_FALSE;
    }

    //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, in, p_addr, {0x%x 0x%x 0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8731_112_2_18_1_44_31_806,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);

    
    switch(img_fmt)
    {
        case ISP_IMAGE_FORMAT_YUV422:
        case ISP_IMAGE_FORMAT_YUV420:
        case ISP_IMAGE_FORMAT_YUV420_3FRAME:
        case ISP_IMAGE_FORMAT_YUV400:
            
            if(!ISP_MEM_IS_IN_SAME_SEGE(p_addr->yaddr,mem_start) ||
               !ISP_MEM_IS_IN_SAME_SEGE(p_addr->uaddr,mem_start))
            {
                //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, Need adjust the memory address, img_fmt %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8744_112_2_18_1_44_31_807,(uint8*)"d",img_fmt);
                
                plane_size = (uint32)(input_size->w * input_size->h);
                if(ISP_IMAGE_FORMAT_YUV400 == img_fmt)
                {
                    frame_size = plane_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;
                }
                else if(ISP_IMAGE_FORMAT_YUV422 == img_fmt)
                {
                    frame_size = (plane_size + ISP_MEM_STRIDER_FOR_ONE_FRAME)<< 1;                    
                }
                else
                {
                    frame_size = (plane_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) + ((plane_size + ISP_MEM_STRIDER_FOR_ONE_FRAME)>> 1);
                }

                if(frame_size > *p_mem_len)
                {
                    //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, No memory, work_mem_len 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8762_112_2_18_1_44_31_808,(uint8*)"d",*p_mem_len);
                    *p_mem_len = 0;
                    rtn = SCI_FALSE;    
                }
                else
                {
                    new_addr.yaddr = ISP_ADDRESS_ALIGNED_(mem_start, ISP_ALIGNED_BYTES);
                    SCI_MEMCPY((void*)new_addr.yaddr, (void*)p_addr->yaddr, plane_size);
                    p_addr->yaddr = new_addr.yaddr;
                    if(ISP_IMAGE_FORMAT_YUV400 != img_fmt)
                    {
                        new_addr.uaddr = new_addr.yaddr + plane_size;
                        new_addr.uaddr = ISP_ADDRESS_ALIGNED_(new_addr.uaddr, ISP_ALIGNED_BYTES);
                        if(ISP_IMAGE_FORMAT_YUV422 == img_fmt)
                        {
                            SCI_MEMCPY((void*)new_addr.uaddr, (void*)p_addr->uaddr, plane_size);                            
                        }
                        else if(ISP_IMAGE_FORMAT_YUV420 == img_fmt)
                        {
                            SCI_MEMCPY((void*)new_addr.uaddr, (void*)p_addr->uaddr, (plane_size>>1));
                        }
                        else
                        {
                            SCI_MEMCPY((void*)new_addr.uaddr, (void*)p_addr->uaddr, (plane_size>>2));
                        }
                        p_addr->uaddr = new_addr.uaddr;
                        if(ISP_IMAGE_FORMAT_YUV420_3FRAME == img_fmt)
                        {
                            new_addr.vaddr = new_addr.uaddr + (plane_size >> 2);
                            new_addr.vaddr = ISP_ADDRESS_ALIGNED_(new_addr.vaddr, ISP_ALIGNED_BYTES);
                            SCI_MEMCPY((void*)new_addr.vaddr, (void*)p_addr->vaddr, (plane_size>>2));    
                            p_addr->vaddr = new_addr.vaddr;
                        }
                    }
                    *p_mem_len = frame_size;
                }
            }
            else
            {
                *p_mem_len = 0;
            }
            
        break;


        case ISP_IMAGE_FORMAT_RGB565:
        case ISP_IMAGE_FORMAT_RGB565_EX:    
        case ISP_IMAGE_FORMAT_RGB888:
        case ISP_IMAGE_FORMAT_RGB888_EX:            
            if(!ISP_MEM_IS_IN_SAME_SEGE(p_addr->yaddr,mem_start))
            {
                //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, Need adjust the memory address, img_fmt %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8813_112_2_18_1_44_31_809,(uint8*)"d",img_fmt);

                plane_size = (uint32)(input_size->w * input_size->h);
                if(ISP_IMAGE_FORMAT_RGB565 == img_fmt || 
                   ISP_IMAGE_FORMAT_RGB565_EX == img_fmt)
                {
                    frame_size = plane_size << 1;
                }
                else 
                {
                    frame_size = plane_size << 2;                    
                }
                
                if((frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) > *p_mem_len)
                {
                    //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, No memory, work_mem_len 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8828_112_2_18_1_44_31_810,(uint8*)"d",*p_mem_len);
                    *p_mem_len = 0;
                    rtn = SCI_FALSE;    
                }
                else
                {                
                    new_addr.yaddr = ISP_ADDRESS_ALIGNED_(mem_start, ISP_ALIGNED_BYTES);
                    SCI_MEMCPY((void*)new_addr.yaddr, (void*)p_addr->yaddr, frame_size);
                    p_addr->yaddr = new_addr.yaddr;  
                    *p_mem_len = frame_size;
                }
            }
            else
            {
                *p_mem_len = 0;
            }
            break;

        default:
            
            //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, unsupported image fotmat %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8848_112_2_18_1_44_31_811,(uint8*)"d",img_fmt);
            
        break;
        
    }

    
    //ISP_SERVICE_TRACE:"_ISP_ServiceCheckMemAddr, out, p_addr, {0x%x 0x%x 0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8856_112_2_18_1_44_31_812,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);

    return rtn;    
}

LOCAL BOOLEAN _ISP_ServiceSyncMem2Output(ISP_ADDRESS_T* p_internal,
                                          ISP_ADDRESS_T* p_external,
                                          ISP_SIZE_T*    input_size,
                                          uint32         img_fmt)
{
    uint32                  frame_size = (uint32)(input_size->w * input_size->h);
    
    if(SCI_NULL == p_internal || 
       SCI_NULL == p_external ||
       SCI_NULL == input_size || 
       ISP_YUV_INVALIDE(p_internal->yaddr,p_internal->uaddr,p_internal->vaddr) ||
       ISP_YUV_INVALIDE(p_external->yaddr,p_external->uaddr,p_external->vaddr) )
    {
        //ISP_SERVICE_TRACE:"_ISP_ServiceSyncMemToOutput ,parameter error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8874_112_2_18_1_44_31_813,(uint8*)"");
        return SCI_FALSE;
    }

    if(ISP_IMAGE_FORMAT_YUV422 != img_fmt && 
        ISP_IMAGE_FORMAT_YUV420 != img_fmt &&        
        ISP_IMAGE_FORMAT_RGB565 != img_fmt &&
        ISP_IMAGE_FORMAT_RGB565_EX != img_fmt)
    {
        //ISP_SERVICE_TRACE:"_ISP_ServiceSyncMemToOutput ,format error %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8883_112_2_18_1_44_31_814,(uint8*)"d", img_fmt);
        return SCI_FALSE;
    }
        
    
    //ISP_SERVICE_LOG:"_ISP_ServiceSyncMemToOutput, in, p_in_addr, {0x%x 0x%x}, p_out_addr {0x%x 0x%x},fmt %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8893_112_2_18_1_44_31_815,(uint8*)"ddddd",p_internal->yaddr, p_internal->uaddr,p_external->yaddr,p_external->uaddr,img_fmt);
    
    if(p_internal->yaddr == p_external->yaddr)
    {
        //ISP_SERVICE_TRACE:"_ISP_ServiceSyncMemToOutput, No need to Sync memory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800G_8897_112_2_18_1_44_31_816,(uint8*)"");
        return SCI_TRUE;
    }

    if(ISP_IMAGE_FORMAT_YUV422 == img_fmt || ISP_IMAGE_FORMAT_YUV420 == img_fmt)
    {
        SCI_MEMCPY((void*)p_external->yaddr, (void*)p_internal->yaddr, frame_size);    
        if(ISP_IMAGE_FORMAT_YUV422 == img_fmt)
        {
            SCI_MEMCPY((void*)p_external->uaddr, (void*)p_internal->uaddr, frame_size);    
        }
        else
        {
            SCI_MEMCPY((void*)p_external->uaddr, (void*)p_internal->uaddr, (frame_size >> 1));                
        }
    }
    else
    {
        SCI_MEMCPY((void*)p_external->yaddr, (void*)p_internal->yaddr, (frame_size << 1));    
    }

    SCI_MEMCPY((void*)p_internal, (void*)p_external, sizeof(ISP_ADDRESS_T));
    
    return SCI_TRUE;

}
uint8 ISP_ServiceGetLastErr(void)
{
	return SCI_FALSE;
}

