/******************************************************************************
 ** File Name:    isp_service_sc6530.c                                       *
 ** Author:       Baggio.He                                                   *
 ** DATE:         10/21/2011                                                  *
 ** Copyright:    2011 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  SC6530 ISP service,                                         *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 10/21/2011    Baggio.he       Initial version                             *
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
#include "lcdc_types.h"
#include "sc_reg.h"
#include "ref_lcd.h"
#include "rotation_app.h"
#include "ref_outport.h"
#else
#include "isp_service_pattern.h"
#endif

#include "isp_service.h"
#include "priority_system.h"
#include "dcamera_cfg.h"
#include "sfs.h"

//////////////////////////////////////////////////////////////////////////////////
#define  SC_UIX8910    0
#define  TEST 1
#define  CAMERA_ROTATION_DISPLAY  1
//static uint32 s_prevCount 100000

static int g_loop_cmd = 0;

static int g_ispmode = 0;
static volatile int g_stopPreview = 0;
BOOLEAN gIsCameraStartXfer = FALSE;
ISP_FRAME_T * g_pFrame;//hal_camera_getCurFrame();
static int g_DcamMode = 0;
static int g_MclkDiv = 42;
static ISP_YUV_DUMP_T isp_yuv_dump_config = {0};

SCI_MUTEX_PTR g_CamDataMutex = SCI_NULL;

LOCAL ISP_FRAME_T  s_enc_frame[2] = {0};

#define ISP_DATA_CLEAR(a)    do{memset((void *)(a),0,sizeof(*(a)));}while(0);
#define  CAM_MODE_CHECK()    //SCI_PASSERT((g_DcamMode < DCAMERA_MODE_PREVIEW || g_DcamMode >= DCAMERA_MODE_MAXIMUM),
                             //           ("Dcamera mode error %s, %d", __MODULE__,__LINE__))



#define  CAM_PREVIEW_STAGE_START     0
#define  CAM_PREVIEW_STAGE_PROCESS   1
#define  CAM_CAPTURE_STAGE_START     2
#define  CAM_CAPTURE_STAGE_PROCESS   3
#define  CAM_MPEG_STAGE_START        4
#define  CAM_MPEG_STAGE_PROCESS      5

void  ISP_Uix_StopPreview();
int   ISP_Uix_AllocFrameAddr();
void  ISP_Uix_LinkEncFrames(void);
int32 ISP_Uix_setEncAddr(uint32 addr);
void _ISP_Uix_ServiceMpegHandler(ISP_FRAME_T *p_frame);

/////////////////////////////////////////////////////////////////////////////////
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
#define ISP_SERVICE_DENOISE             0x15
#define ISP_SERVICE_VP_EX_LOW_MEM       0x16

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
#define ISP_DENOISE_EVENT               BIT_11

#define ISP_SERVICE_THREAD              ISP
/* AGORA_PATCH start */
#define ISP_STACK_SIZE                  ((1024 * 8) + 512)
/* AGORA_PATCH end */
#define ISP_QUEUE_NUM                   50
#define ISP_QUEUE_FEW_NUM               5
#define ISP_QUEUE_SAFE_NUM              40
#define ISP_VP_REVIEW_TIME              SCI_WAIT_FOREVER // 200 // SCI_WAIT_FOREVER//when in debuging,  //200 when in product   //ms
#define ISP_WAIT_TIME                   4000 //ms
#define ISP_SERVICE_TIMEOUT             300 //if frame rate less is than 10/3,restart VT
#define ISP_SERVICE_ENC_TIME            30
#define ISP_SERVICE_ENC_TIMEOUT         3

#define ISP_TRY_TIMES                   3
#define ISP_WORK_MEM_SIZE_DEFAULT       (1 << 20) //(150 << 10) //for 320*240*2
#define ISP_MEM_STRIDER_FOR_ONE_FRAME   0
#define ISP_SCALE_SLICE_HEIGHT          128 // It's forcely suggested the slice height should be configed as 64 in case of address alignement
#define ISP_SCALE_LINE_BUF_SIZE(w)      (w * 2 * 4 + ISP_MEM_STRIDER_FOR_ONE_FRAME)
#define ISP_SLICE_OUT_ESTIMATE(n)       ((((n) + 16 -1) & ~0xF)+4)
#define ISP_ALIGNED_BYTES               4
#define ISP_ADDRESS_ALIGNED_(a,n)       (((a) + (n) -1) / (n) * (n))

#define ISP_REVIEW_FRAME_NUM            2 // must be fewer than ISP_PATH_FRAME_COUNT_MAX
#define ISP_VT_ROUND_FRAME_NUM          2 // CID37112
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
#define ISP_SERVICE_LOG                 SCI_TRACE_LOW
#define ISP_SERVICE_ALLOC(n)            SCI_ALLOCA(n)
#define ISP_SERVICE_FREE(p)             do{ if(p) {SCI_FREE(p); p=SCI_NULL;}}while(0)

#define ISP_RTN_IF_ERR(n)               if(n)  goto exit

#define ISP_SET_ADDR(a,start,length)    a.yaddr = ISP_ADDRESS_ALIGNED_(start, ISP_ALIGNED_BYTES); \
                                        a.uaddr = ISP_ADDRESS_ALIGNED_(a.yaddr + length, ISP_ALIGNED_BYTES); \
                                        a.vaddr = 0;

#define ISP_SERVICE_SC_COEFF_SIZE       (8*1024)
#define ISP_SERVICE_DENOISE_MAX_WIDTH   960
#define ISP_SERVICE_VGA_WIDTH           640
#if defined(MAINLCD_DEV_SIZE_240X284)// wuxx add for CAMERA BF20A6,GC6133, 240X284 Ô¤ÀÀÈ«ÆÁ, and isp_service.a
// NULL
#else
#define ISP_SERVICE_DUMP_FRAME_TO_FILE  FALSE
#define ISP_SERVICE_DUMP_FRAME_TO_DDR   FALSE
// if MAX_DUMP_FRAME_LIMIT equals zero, the yuv data will be dumped in real time
// otherwise, will dump MAX_DUMP_FRAME_LIMIT num
#define MAX_DUMP_FRAME_LIMIT  3
#endif


enum
{
    ISP_STATE_PREVIEW = 0x10,
    ISP_STATE_CAPTURE = 0x20,
    ISP_STATE_CAPTURE_DONE,
    ISP_STATE_CAPTURE_SCALE,

    ISP_STATE_SCALE = 0x30,
    ISP_STATE_SCALE_SLICE,

    ISP_STATE_REVIEW = 0x40,

    ISP_STATE_MPEG = 0x50,
    ISP_STATE_MPEG_CAPTURE,
    ISP_STATE_MPEG_REVIEW,
    ISP_STATE_MPEG_SCALE,

    ISP_STATE_VP = 0x60,
    ISP_STATE_VP_CAPTURE,
    ISP_STATE_VP_REVIEW_LOCAL,
    ISP_STATE_VP_SCALE_LOCAL,
    ISP_STATE_VP_REVIEW_REMOTE,
    ISP_STATE_VP_SCALE_REMOTE,
    ISP_STATE_VP_LOW_MEM_CB_WAIT,
    ISP_STATE_VP_LOW_MEM_IDLE,
    ISP_STATE_VP_LOW_MEM_BUFF_FREE,

    ISP_STATE_DENOISE = 0x70,

    ISP_STATE_STOP = 0xE0,
    ISP_STATE_CLOSED = 0xFF
};

typedef enum
{
    ISP_SERVICE_OPEN_SIG = 1,
    ISP_SERVICE_CLOSE_SIG,
    ISP_SERVICE_PATH_SIG,
    ISP_SERVICE_SET_PARAM_SIG,
    ISP_SERVICE_START_SIG,
    ISP_SERVICE_STOP_SIG,
    ISP_SERVICE_DISP_SIG,
    ISP_SERVICE_PREVIEW_START_SIG,
    ISP_SERVICE_PREVIEW_PROCESS_SIG,
    ISP_SERVICE_CAP_START_SIG,
    ISP_SERVICE_CAP_PROCESS_SIG
}ISP_SERVICE_SIG_E;

typedef struct _isp_display_block_t
{
    uint8                               lcd_id;
    uint8                               block_id;//block id
    uint8                               img_fmt;
    uint8                               is_enable;
    ISP_RECT_T                          img_rect;
    ISP_RECT_T                          lcd_inv_rect;
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
    uint32                              module_addr;
    uint8                               service;//service id;
    uint8                               state;//machine state
    /*for cap*/
    uint8                               cap_input_image_format;
    uint8                               cap_input_image_patten;
    uint8                               cap_sensor_if_mode;
    uint8                               cap_sensor_if_endian;
    uint8                               vsync_polarity;
    uint8                               hsync_polarity;
    uint16                              thre_start;
    uint16                              thre_end;
    uint8                               fix_mode_eb;
    uint8                               b_is_atv;
    uint8                               pclk_polarity;
    uint8                               is_first_frame;
    uint8                               preview_skip_frame_num;
    uint8                               preview_deci_frame_num;
    uint8                               capture_skip_frame_num;
    uint8                               res0;
    ISP_SIZE_T                          cap_input_size;
    ISP_RECT_T                          cap_input_range;
    ISP_CAP_DEC_T                       cap_img_dec;
    ISP_SIZE_T                          cap_output_size;
    uint8                               ccir_pclk_src;
    uint8                               ccir_d0_src;
    uint8                               ccir_vsync_src;
    uint8                               first_cap_format;

    /*for review , scale , vt review */
    uint8                               path_input_image_format;
    uint8                               path_output_image_format;
    uint8                               is_slice;
    uint8                               is_slice_alone;
    ISP_DENOISE_CFG_T                   denoise_cfg;
    ISP_SIZE_T                          input_size;
    ISP_RECT_T                          input_range;
    ISP_ADDRESS_T                       input_addr;
    ISP_ADDRESS_T                       swap_buff;
    ISP_ADDRESS_T                       line_buff;
    ISP_ADDRESS_T                       output_addr;
    uint32                              slice_height;
    uint32                              slice_out_height;
    uint32                              total_scale_out;
    uint32                              total_slice_count;

    /*local display(preview,review parameter , include block,round buffer,*/
    uint8                               local_review_enable;
    uint8                               local_display_enable;
    uint8                               local_need_encode_frm;
    uint8                               local_review_format;
    uint8                               vt_review_from; // 0 local, 1, remote
    uint8                               disp_frm_round_num;
    uint8                               restart_counter;
    uint8                               res1;
    ISP_DISP_BLOCK_T                    display_block;
    ISP_RECT_T                          display_range;// preview, review,mpeg preview
    ISP_ADDRESS_T                       display_addr[ISP_PATH_FRAME_COUNT_MAX];
    ISP_FRAME_T                         display_frame;
    ISP_FRAME_T                         *display_frame_locked_ptr;
    ISP_ROTATION_E                      display_rotation;
    ISP_ADDRESS_T                       rotation_buf_addr[ISP_PATH_FRAME_COUNT_MAX];
    ISP_ADDRESS_T                       local_addr;
    ISP_FRAME_T                         local_frame;
    uint32                              display_frame_count;
    ISP_VT_MEM_BLOCK_STATE              vt_mem_block_state[ISP_VT_ROUND_FRAME_NUM];

    /*remote display(vt review) parameter , include block,round buffer,*/
    ISP_SIZE_T                          remote_input_size;
    ISP_RECT_T                          remote_input_range;
    ISP_ADDRESS_T                       remote_input_addr;
    uint8                               remote_deblock_enable;
    uint8                               remote_review_enable;
    uint8                               remote_display_enable;
    uint8                               remote_input_fmt;
    ISP_DISP_BLOCK_T                    remote_display_block;
    ISP_RECT_T                          remote_display_range;
    ISP_ADDRESS_T                       remote_display_addr[ISP_PATH_FRAME_COUNT_MAX];
    ISP_ROTATION_E                      remote_display_rotation;
    ISP_ADDRESS_T                       remote_rotation_display_addr[ISP_PATH_FRAME_COUNT_MAX];
    ISP_SIZE_T                          encoder_size;   //for jpeg capture ,mpeg capture, vt capture, scale out
    ISP_ADDRESS_T                       encoder_addr[ISP_PATH_FRAME_COUNT_MAX];
    ISP_ADDRESS_T                       encoder_rot_addr[ISP_PATH_FRAME_COUNT_MAX];
    ISP_ROTATION_E                      enc_rotation;
    uint8                               enc_stoped;
    uint8                               vt_stop_arrived;
    uint8                               is_enc_first_frame;
    uint8                               jpeg_capture_is_composed;
    ISP_FRAME_T                         *p_frame_enc;
    ISP_FRAME_T                         *p_frame_enc_cur;
    uint32                              vt_local_display_count;
    uint32                              vt_review_remote_count;
    uint32                              vt_mem_end_cur;
    uint8                               enc_frm_round_num;
    uint8                               ccir_d1_src;
    uint8                               res2[2];

    /*control variable*/
    ISP_USER_FUNC_PTR                   display_user_cb;
    SCI_MUTEX_PTR                       display_user_cb_ctrl;
    ISP_USER_FUNC_PTR                   encode_user_cb;
    SCI_MUTEX_PTR                       encode_user_cb_ctrl;
    ISP_USER_FUNC_PTR                   post_cap_user_cb;
    SCI_MUTEX_PTR                       post_cap_user_cb_ctrl;
    SCI_MUTEX_PTR                       mem_ctrl;
    SCI_TIMER_PTR                       timer_ptr;
    uint32                              timer_mode;
    get_data                            scale_user_cb;
    int32                               (*start_scale_callback)(int32 param);   //scale start user callback
    int32                               start_scale_param;                      //scale start user callback parameter
    uint32                              work_mem_len;
    void                                *p_work_mem_cur;
    void                                *p_work_mem;
    uint32                              work_mem_res_len;
    uint32                              *p_scale_coeff_mem;
    uint8                               mem_alloced_by_myself;
    uint8                               curr_queue_available;
    uint8                               b_wait_for_vt_event;
    uint8                               sys_performance_level;
    uint8                               watchdog_feeded;
    uint8                               err_code;
    uint8                               b_drv_init_ed;
    uint8                               disp_blk_id;
    uint32                              work_mem_y_len;
    void                                *p_work_mem_y;

    /////////////////////////////////////////////////////
    uint32                              uix_base_addr;
    uint32                              uix_mem_size;
    uint32                              uix_display_size;

    ISP_FRAME_T                         *uix_frame_enc_cur;
    ISP_FRAME_T                         *uix_frame_enc_head;

}ISP_SERVICE_T;



//thread context
LOCAL uint32                 s_isp_service_thread_id = SCI_INVALID_BLOCK_ID;
LOCAL uint32                 _ISP_ServiceStack[ISP_STACK_SIZE >> 2];
LOCAL uint32                 _ISP_ServiceQueue[ISP_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];
LOCAL ISP_SERVICE_T          s_isp_service = {0};
LOCAL SCI_EVENT_GROUP_PTR    s_isp_event = SCI_NULL;
LOCAL SCI_SEMAPHORE_PTR      s_isp_service_sema = SCI_NULL;
LOCAL uint32                 s_b_wait_for_opened = 0;
LOCAL uint32                 s_is_open_timeout = SCI_FALSE;
LOCAL BLOCK_ID               isp_owner_id = SCI_INVALID_BLOCK_ID;
LOCAL uint32                  *dump_frame_data = NULL;

LOCAL void    _ISP_ServiceCreate(void);
LOCAL void    _ISP_ServiceFlushQueue(void);
LOCAL void    _ISP_ServiceMainMessageLoop(uint32 argc, void *argv);
LOCAL void    _ISP_ServiceOpenSignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServiceCloseSignalHandler(uint32 service,uint32 param,void * ptr);
LOCAL void    _ISP_ServicePathSignalHandler(uint32 service,uint32 param,void * ptr);
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
LOCAL void    _ISP_ServiceGetPostCapCtrl(void);
LOCAL void    _ISP_ServiceRelPostCapCtrl(void);
LOCAL int32   _ISP_ServiceCallUserDispCB(void *p_frame);
LOCAL int32   _ISP_ServiceCallUserEncCB(void *p_frame);
LOCAL int32   _ISP_ServiceCallUserPostCapCB(uint32 param);
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
LOCAL void    _ISP_ServiceOnSensorHardwareErr(void *p);
LOCAL void    _ISP_ServiceOnTxDone(void *p);
LOCAL uint32  _ISP_ServiceGetDefaultMemSize(void);
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
LOCAL void    _ISP_ServiceStartVTExtEncodeLowMem(void);
LOCAL void    _ISP_ServiceSyncImageData(ISP_ADDRESS_T*        p_addr,
                                        uint32                img_width,
                                        uint32                img_height,
                                        uint8                 img_format,
                                        DMABUFFER_DIRECTION_E data_dir);
LOCAL void    _ISP_ServiceRotFmt(uint32 fmt, ROTATION_PARAM_T * p_rot);
LOCAL void    _ISP_ServiceSetCapParam(ISP_CAP_PARAM_T *p_cap_param);
LOCAL void    _ISP_ServiceGetCapParam(ISP_CAP_PARAM_T *p_cap_param);
LOCAL uint32  _ISP_ServiceGetPerformanceLevel(void);
LOCAL void    _ISP_ServiceCaptureOnce(uint32 output_fmt, uint32 output_flag);
LOCAL void    _ISP_ServiceInternScale(ISP_FRAME_T *p_in_frame,
                                      ISP_RECT_T  *p_in_rect,
                                      ISP_FRAME_T *p_out_frame);
LOCAL void    _ISP_ServiceMpegHandler(ISP_FRAME_T *p_frame);
LOCAL void    _ISP_ServiceVTHandler(ISP_FRAME_T *p_frame);
LOCAL void    _ISP_ServiceVTExtHandler(ISP_FRAME_T *p_frame);
LOCAL void    _ISP_ServiceVTExtHandlerLowMem(ISP_FRAME_T *p_frame);
LOCAL void    _ISP_ServiceJPEGHandler(ISP_FRAME_T *p_frame);
LOCAL void    _ISP_ServiceScaleHandler(ISP_FRAME_T *p_frame);
LOCAL  void   _ISP_ServiceGetTrimRect(ISP_SIZE_T *p_src_size,
                                      ISP_SIZE_T *p_dst_size,
                                      ISP_RECT_T *p_rect);
LOCAL void    _ISP_ServiceWaitForDone(void);
LOCAL void    _ISP_ServiceExitWait(void);
LOCAL BOOLEAN _ISP_ServiceIsLittleEndian(void);
LOCAL int32   _ISP_ServiceConfigCap(void);
LOCAL void    _ISP_ServiceStartDenoise(void);
LOCAL void    _ISP_ServiceSetDenoiseParam(ISP_DENOISE_PARAM_T * param_ptr);
LOCAL uint32  _ISP_ServiceCheckDenoiseParam(ISP_DENOISE_PARAM_T * param_ptr);
LOCAL BOOLEAN _ISP_ServiceWaitForVideoCodecDone(void);
LOCAL void    _ISP_ServiceEncodeFrameInitLowMem(void);
LOCAL uint32  _ISP_ServiceGetVtMemBlockId(uint32 * block_id_ptr, uint32 state);
LOCAL void    _ISP_ServicePostDisplayHandle(void);
LOCAL void    _ISP_ServiceDisplayOneFrameLowMem(uint32 block_id,ISP_FRAME_T *p_frame);
void _ISP_UixIrqHandler(HAL_CAMERA_IRQ_CAUSE_T cause);

LOCAL void _ISP_Uix_ServiceGetDataCtrl(void);
LOCAL void _ISP_Uix_ServiceRelDataCtrl(void);

LOCAL void Cam_preview_process();
LOCAL void Cam_preview_start();
LOCAL void Cam_snapshot_start();
LOCAL void Cam_snapshot_process();

int ISP_UixOpenCameraContoller();
PUBLIC int ISP_ServiceDumpToFile(void *addr, uint32 len, char *misc, char *ext);
PUBLIC int ISP_ServiceDumpToDDR(void *ptr);

#ifndef _ISP_PATTERN_

extern SCI_THREAD_PTR SCI_GetThreadBlockFromId(BLOCK_ID thread_id);

/*****************************************************************************/
//  Description: Layer1 set information for isp
//    Global resource dependence:
//  Author: Tim.zhu
/*****************************************************************************/
LOCAL BOOLEAN _ISP_GetDSPInfor(void)
{
    BOOLEAN bRtn = SCI_TRUE;
    REF_ioctl(REF_IOCTL_CHK_DC_CAPTICK, (void *)&bRtn);

    return bRtn;
}


/*****************************************************************************/
//  Description: Is ISP idle
//  Global resource dependence:
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:              TRUE: idle.
//                      FALSE: busy
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN ISP_ServiceIsIdle(void)
{
    return TRUE;
}

LOCAL uint32 _ISP_IsSupport2MSnapshot(void)
{

#ifdef DC_2M_SUPPORT
    return SCI_TRUE;
#else
    return SCI_FALSE;
#endif

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_483_112_2_18_1_43_56_261,(uint8*)"dddd", s->service,s->state,SCI_GetTickCount(),wait_option);

    if(s_isp_service_sema)
    {
        status = SCI_GetSemaphore(s_isp_service_sema, wait_option);
        if(status)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceOpen, time out %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_490_112_2_18_1_43_56_262,(uint8*)"d",SCI_GetTickCount());
            return SCI_ERROR;
        }
        s_b_wait_for_opened = 1;
        isp_owner_id = SCI_IdentifyThread();
    }

    ISP_SERVICE_TRACE("ISP:Open,cur_timr %d thread id %d",SCI_GetTickCount(),isp_owner_id);

    if(s_is_open_timeout)
    {
        SCI_TRACE_LOW("ISP:Open timeout 1");
        status = SCI_GetEvent(s_isp_event, ISP_OPEN_EVENT, SCI_AND_CLEAR, &actual_event, ISP_WAIT_TIME);
        s_is_open_timeout = SCI_FALSE;
    }
    else
    {
        SCI_TRACE_LOW("ISP:Open timeout 2");
        _ISP_ServiceSendRequest(0,ISP_SERVICE_OPEN_SIG,0,0);
        //SCI_TRACE_LOW("david ISP_ServiceOpen getevent start");
        status = SCI_GetEvent(s_isp_event, ISP_OPEN_EVENT, SCI_AND_CLEAR, &actual_event, ISP_WAIT_TIME);
        //SCI_TRACE_LOW("david ISP_ServiceOpen getevent end!");
    }

    if(status == SCI_SUCCESS)
    {
        SCI_TRACE_LOW("ISP:Open timeout errcode:%d", s->err_code);
        if(s->err_code)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: error code %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_508_112_2_18_1_43_56_264,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }
    else
    {
        SCI_TRACE_LOW("ISP:Open timeout 4");
        s_is_open_timeout = SCI_TRUE;

        if(s_isp_service_sema && (1 == s_b_wait_for_opened))
        {
            SCI_TRACE_LOW("ISP:Open timeout 5");
            s_b_wait_for_opened = 0;
            SCI_PutSemaphore(s_isp_service_sema);
        }
    }
    SCI_TRACE_LOW("ISP:Open status:%d", status);
    return status;
}


uint32 ISP_ServiceClose(void)
{
    ISP_SERVICE_T    *s = &s_isp_service;
    uint32           actual_event = 0;
    uint32           status = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceClose, service = %d, status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_525_112_2_18_1_43_56_265,(uint8*)"dd", s->service,s->state);

    if(ISP_STATE_CLOSED == s->state && 1 != s_b_wait_for_opened)
    {
        return SCI_SUCCESS;
    }

    if(ISP_SERVICE_PREVIEW != s->service)
    {
        _ISP_ServiceWaitForDone(); //wait for the current action done
    }
    _ISP_ServiceFlushQueue();
    if(ISP_SERVICE_PREVIEW != s->service)
    {
        _ISP_ServiceExitWait();
    }

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

    if(dump_frame_data != PNULL)
    {
        free(dump_frame_data);
        dump_frame_data = PNULL;
    }

    return status;

}


void ISP_ServiceSetClk(uint32 clk)
{

}

void ISP_ServiceReviewBypass(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             frame_t = {0};

    frame_t.yaddr = s->display_addr[0].yaddr;
    frame_t.uaddr = s->display_addr[0].uaddr;

    s->is_first_frame = 1;
    _ISP_ServiceDisplayOneFrame(s->display_block.block_id, &frame_t);

    return ;
}

uint32 ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_PREVIEW_PARAM_T     *p_local_para = SCI_NULL;
    uint32                  status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetPreviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_587_112_2_18_1_43_56_266,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x ,y,w,h {%d,%d,%d,%d} lcd rect {%d,%d,%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_597_112_2_18_1_43_56_267,(uint8*)"dddddddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h,param_ptr->lcd_rect.x,param_ptr->lcd_rect.y,param_ptr->lcd_rect.w,param_ptr->lcd_rect.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:lcd_id %d,block_id %d, disp_range.x,y,w,h {%d,%d,%d,%d} rot mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_606_112_2_18_1_43_56_268,(uint8*)"ddddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h,param_ptr->disp_rotation);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:skip_frames %d, deci_frames %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_610_112_2_18_1_43_56_269,(uint8*)"dd",param_ptr->cap_param.cap_frame_skipped,param_ptr->cap_param.cap_frame_decimated);

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
               sizeof(ISP_PREVIEW_PARAM_T)); /*lint !e718 !e746 !e628 -e718 -e746 -e628*/
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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_640_112_2_18_1_43_56_270,(uint8*)"d",s->err_code);
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_681_112_2_18_1_43_56_271,(uint8*)"");
    if(s->err_code)
    {
        ISP_SERVICE_TRACE("ISP:StartPreview,err 0x%x",s->err_code);
        return s->err_code;
    }
    _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_PREVIEW,0);

    s->is_first_frame = 1;
    s->enc_stoped = 0;
    s->err_code = ISP_ERR_CODE_NO_ERR;
    s->restart_counter = 0;
    // SCI_EVENT_GROUP_T *isp_test=(SCI_EVENT_GROUP_T *)s_isp_event;
     //TX_EVENT_FLAGS_GROUP  *isp_event_tset= (TX_EVENT_FLAGS_GROUP *)isp_test;
    status = SCI_GetEvent(s_isp_event,
                          ISP_DISPLAY_EVENT,
                          SCI_AND_CLEAR,
                          &actual_event,
                          ISP_WAIT_TIME);

    if(status)
    {
        s->err_code = ISP_ERR_CODE_ST_ERR;
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
    }

    SCI_TRACE_LOW("ISP:StartPreview status:%d",  status);

    //LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);

    return status;
}


void ISP_ServiceStopPreview(void)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    uint32                 status = SCI_SUCCESS;
    uint32                 actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStopPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_712_112_2_18_1_43_56_272,(uint8*)"");

    if(s->service != ISP_SERVICE_PREVIEW)
    {
        return ;
    }
    s->enc_stoped = 1;

    //ISP_DriverStop(s->module_addr);

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
    // ISP_DriverExitWaitForActionDone(s->module_addr);


    LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);

    return ;
}

void ISP_ServiceStartReview(void)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    uint32                 actual_event = 0;
    uint32                 status = 0;
    ISP_USER_FUNC_PTR      user_cb = SCI_NULL;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartReview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_752_112_2_18_1_43_56_274,(uint8*)"");

    if(s->err_code)
    {
        ISP_SERVICE_TRACE("ISP:StartReview,err 0x%x",s->err_code);
        return;
    }
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

    p_local_para = (ISP_ADDRESS_T*)ISP_SERVICE_ALLOC(sizeof(ISP_ADDRESS_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceReviewNext address 0x%x, {0x%x,0x%x,0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_798_112_2_18_1_43_57_275,(uint8*)"dddd",p_local_para,addr_ptr->yaddr,addr_ptr->uaddr,addr_ptr->vaddr);

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
    uint32                 status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetReviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_836_112_2_18_1_43_57_276,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input format = %d, input.x,y,w,h {%d %d %d %d} lcd rect {%d,%d,%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_847_112_2_18_1_43_57_277,(uint8*)"ddddddddd",param_ptr->input_format, param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h,param_ptr->lcd_rect.x,param_ptr->lcd_rect.y,param_ptr->lcd_rect.w,param_ptr->lcd_rect.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} rot mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_856_112_2_18_1_43_57_278,(uint8*)"ddddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h,param_ptr->disp_rotation);

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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_888_112_2_18_1_43_57_279,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }

    return status;
}
void    ISP_ServiceReviewFromDC(uint32 b_from_dc)
{

}


uint32 ISP_ServiceSetJpegCaptureParam(ISP_JPEG_PARAM_T * param_ptr)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    ISP_JPEG_PARAM_T       *p_local_para = SCI_NULL;
    uint32                 status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_909_112_2_18_1_43_57_280,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_915_112_2_18_1_43_57_281,(uint8*)"dddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_921_112_2_18_1_43_57_282,(uint8*)"dddd",param_ptr->yuv_type,param_ptr->output_size.w,param_ptr->output_size.h,param_ptr->cap_param.cap_frame_skipped);



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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_955_112_2_18_1_43_57_283,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }

    return status;

}


void ISP_ServiceStartCaptureJpeg(void)
{
    ISP_SERVICE_T          *s = &s_isp_service;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartCaptureJpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_969_112_2_18_1_43_57_284,(uint8*)"");

    s->enc_stoped = 0;
    s->is_first_frame = 1;
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_986_112_2_18_1_43_57_285,(uint8*)"");



    //ISP_SERVICE_TRACE:"ISP_SERVICE:input format = %d,yuv_type = %d,output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_994_112_2_18_1_43_57_286,(uint8*)"dddd",param_ptr->input_format, param_ptr->yuv_type,param_ptr->output_size.w,param_ptr->output_size.h);

    s->input_size.w = param_ptr->input_range.w;
    s->input_size.h = param_ptr->input_range.h;

    SCI_MEMCPY((void*)&s->input_range,
               (void*)&param_ptr->input_range,
               sizeof(ISP_RECT_T));

    SCI_MEMCPY((void*)&s->encoder_size,
               (void*)&param_ptr->output_size,
               sizeof(ISP_SIZE_T));

    s->is_slice = 0;
    s->cap_input_image_format = (uint8)param_ptr->input_format;
    s->service = ISP_SERVICE_JPEG;

    return SCI_SUCCESS;
}


uint32 ISP_ServiceSetMpegCaptureParam(ISP_MPEG_PARAM_T * param_ptr)
{
    ISP_SERVICE_T          *s = &s_isp_service;
    ISP_MPEG_PARAM_T        *p_local_para = SCI_NULL;
    uint32                  status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetMpegCaptureParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1023_112_2_18_1_43_57_287,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE: input.x,y,w,h {%d %d %d %d},skip_frames %d, deci_frames %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1031_112_2_18_1_43_57_288,(uint8*)"dddddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h,param_ptr->cap_param.cap_frame_skipped,param_ptr->cap_param.cap_frame_decimated);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}, lcd rect {%d,%d,%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1040_112_2_18_1_43_57_289,(uint8*)"ddddddd",param_ptr->yuv_type,param_ptr->encode_size.w,param_ptr->encode_size.h,param_ptr->lcd_rect.x,param_ptr->lcd_rect.y,param_ptr->lcd_rect.w,param_ptr->lcd_rect.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x,y,w,h {%d %d %d %d} disp rot %d enc rot %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1050_112_2_18_1_43_57_290,(uint8*)"dddddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h,param_ptr->disp_rotation,param_ptr->encode_rotation);

    if(_ISP_ServiceCheckMpegParam(param_ptr))
    {
        return SCI_ERROR;
    }

    if(ISP_DISPLAY_NONE != param_ptr->disp_lcd_id)
    {
        LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id,NO_PERMIT_GUI_INVALIDATE);
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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1081_112_2_18_1_43_57_291,(uint8*)"d",s->err_code);
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1100_112_2_18_1_43_57_292,(uint8*)"");
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

    param_ptr->data_format = (ISP_IMAGE_FORMAT)s->path_output_image_format;
    param_ptr->encode_rotation = s->enc_rotation;
    param_ptr->disp_rotation = s->display_rotation;

    _ISP_ServiceGetCapParam(&param_ptr->cap_param);

    return SCI_SUCCESS;
}


uint32 ISP_ServiceStartCaptureMpeg(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = 0;
    uint32                  actual_event = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartCaptureMpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1157_112_2_18_1_43_57_293,(uint8*)"");
    if(s->err_code)
    {
        ISP_SERVICE_TRACE("ISP:StartCaptureMpeg,err 0x%x",s->err_code);
        return s->err_code;
    }

    if(g_DcamMode != DCAMERA_MODE_RECORD)
    {
        SCI_PASSERT(SCI_FALSE, ("ISP_ServiceStartCaptureMpeg mode Error!"));
    }

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_MPEG,
                            0);

    s->is_first_frame = 1;
    s->is_enc_first_frame = 1;
    s->enc_stoped = 0;
    s->err_code = ISP_ERR_CODE_NO_ERR;
    s->restart_counter = 0;

    status = SCI_GetEvent(s_isp_event,
                            ISP_MPEG_START_EVENT,
                             SCI_AND_CLEAR,
                             &actual_event,
                             ISP_WAIT_TIME);

    if(status)
    {
        s->err_code = ISP_ERR_CODE_ST_ERR;
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
    }

//    if(s->local_review_enable)
//    {
//        LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
//    }

    return status;
}

BOOLEAN ISP_ServiceIsMpegOn(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceIsMpegOn, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1203_112_2_18_1_43_57_294,(uint8*)"d", s->service);

    return (s->service == ISP_SERVICE_MPEG);

}

void ISP_ServiceStartCaptureCompoundJpeg(void)
{
    _ISP_ServiceSetCompoundJpegFlag(SCI_TRUE);
}

BOOLEAN _ISP_ServiceWaitForVideoCodecDone(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_frame = s->p_frame_enc;
    uint32                  i = 0, time_out = 0;
    BOOLEAN                 ret = SCI_TRUE;

    for(i = 0; i < s->enc_frm_round_num; i++)
    {
        time_out = 0;
        while(time_out++ < ISP_SERVICE_ENC_TIMEOUT)
        {
            if(ISP_DriverFrameIsLocked(p_frame))
            {
                SCI_Sleep(ISP_SERVICE_ENC_TIME);
                ISP_SERVICE_TRACE("ISP:WaitVideoCodec Frame locked");
            }
            else
            {
                p_frame = p_frame->next;
                break;
            }
        }

        if(time_out >= ISP_SERVICE_ENC_TIMEOUT)
        {
            ISP_SERVICE_TRACE("ISP:WaitVideoCodec timeout");
            ret = SCI_FALSE;
            break;
        }
    }

    ISP_SERVICE_TRACE("ISP:WaitVideoCodec, ret %d, round %d",ret, s->enc_frm_round_num);
    return ret;
}

void ISP_ServiceStopCapture(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = SCI_SUCCESS;
    uint32                  actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStopCapture"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1221_112_2_18_1_43_57_295,(uint8*)"");

    if(s->service != ISP_SERVICE_MPEG)
    {
        return ;
    }
    s->enc_stoped = 1;
    _ISP_ServiceWaitForDone(); //wait for the current MPEG capture or MPEG scaling done
    _ISP_ServiceFlushQueue();
    _ISP_ServiceExitWait();

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_STOP_SIG,
                            0,
                            0);

    status = SCI_GetEvent(s_isp_event,
                          ISP_STOP_EVENT,
                          SCI_AND_CLEAR,
                          &actual_event,
                          ISP_WAIT_TIME);
    _ISP_ServiceWaitForVideoCodecDone();
    _ISP_ServiceCloseBlock();
    _ISP_ServiceEncodeFrameDeInit();
    return ;
}

void ISP_ServiceStopReview(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = SCI_SUCCESS;
    uint32                  actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStopReview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1254_112_2_18_1_43_57_296,(uint8*)"");

    if(s->service != ISP_SERVICE_REVIEW)
    {
        return ;
    }

    _ISP_ServiceWaitForDone(); //wait for the current review done
    _ISP_ServiceExitWait();

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
    uint32                  status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1289_112_2_18_1_43_57_297,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1296_112_2_18_1_43_57_298,(uint8*)"ddddd",param_ptr->input_format, param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1301_112_2_18_1_43_57_299,(uint8*)"ddd", param_ptr->input_addr.yaddr, param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1306_112_2_18_1_43_57_300,(uint8*)"ddd",param_ptr->output_format,param_ptr->output_size.w,param_ptr->output_size.h);


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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1339_112_2_18_1_43_57_301,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }

    return status;
}


void ISP_ServiceStartScale(void)
{
    uint32                  status = 0, actual_event = 0;
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_ADDRESS_T           NULL_ADDR = {0};

    if(s->err_code)
    {
        ISP_SERVICE_TRACE("ISP:StartScale,error code0x%x",s->err_code);
        return;
    }

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_SCALE,
                            0);

    status = SCI_GetEvent(s_isp_event,
                          ISP_SCALE_EVENT,
                          SCI_AND_CLEAR,
                          &actual_event,
                          ISP_WAIT_TIME);

    if(status&&(s->scale_user_cb))
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        (*s->scale_user_cb)(NULL_ADDR, 0, 0);
    }

    return ;

}

uint32 ISP_ServiceSetDenoiseParam(ISP_DENOISE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_SCLAE_PARAM_T       *p_local_para = SCI_NULL;
    uint32                  status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1382_112_2_18_1_43_58_302,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input_format = %d,input.w,h {%d %d }"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1387_112_2_18_1_43_58_303,(uint8*)"ddd",param_ptr->input_format, param_ptr->input_size.w,param_ptr->input_size.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1392_112_2_18_1_43_58_304,(uint8*)"ddd", param_ptr->input_addr.yaddr, param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1397_112_2_18_1_43_58_305,(uint8*)"ddd",param_ptr->output_format,param_ptr->output_size.w,param_ptr->output_size.h);

    if(_ISP_ServiceCheckDenoiseParam(param_ptr))
    {
        return SCI_ERROR;
    }

    p_local_para = (ISP_SCLAE_PARAM_T*)ISP_SERVICE_ALLOC(sizeof(ISP_DENOISE_PARAM_T));
    if(SCI_NULL == p_local_para)
    {
        return SCI_ERROR;
    }
    SCI_MEMCPY((void*)p_local_para,
               (void*)param_ptr,
               sizeof(ISP_DENOISE_PARAM_T));


    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_SET_PARAM_SIG,
                            ISP_SERVICE_DENOISE,
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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1429_112_2_18_1_43_58_306,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }

    return status;
}

LOCAL void _ISP_ServiceStartDenoise(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    uint32                  param = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartDenoise, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1443_112_2_18_1_43_58_307,(uint8*)"d", SCI_GetTickCount());

    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_REVIEW);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path*/
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_SIZE,
                                   (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_RECT,
                                   (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_ADDR,
                                   (void*)&s->input_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    param = (uint32)s->path_input_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_SIZE,
                                   (void*)& s->encoder_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_ADDR,
                                   (void*)&s->output_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    param = (uint32)s->path_output_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_DENOISE_CFG,
                                   (void*)&s->denoise_cfg);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Register ISR callback*/

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                      ISP_IRQ_NOTICE_TX_DONE,
                                      _ISP_ServiceOnTxDone);
    ISP_RTN_IF_ERR(rtn_drv);

    _ISP_ServiceSyncImageData(&s->input_addr,
                              s->input_size.w,
                              s->input_size.h,
                              s->path_input_image_format,
                              DMABUFFER_TO_DEVICE);
    s->state = ISP_STATE_DENOISE;
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1519_112_2_18_1_43_58_308,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }

    return;

}


void ISP_ServiceStartDenoise(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  actual_event = 0;
    uint32                  status = 0;
    ISP_USER_FUNC_PTR       user_cb = SCI_NULL;

    //denoise state check
    if(ISP_STATE_CLOSED == s->state)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartDenoise,wrong state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1541_112_2_18_1_43_58_309,(uint8*)"");
        return ;
    }

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartDenoise"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1545_112_2_18_1_43_58_310,(uint8*)"");

    //send request message
    _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_DENOISE,0);

    _ISP_ServiceGetEncCtrl();
    user_cb = s->encode_user_cb;
    _ISP_ServiceRelEncCtrl();

    if(SCI_NULL == user_cb)
    {
        status = SCI_GetEvent(s_isp_event,
                              ISP_DENOISE_EVENT,
                              SCI_AND_CLEAR,
                              &actual_event,
                              ISP_WAIT_TIME);

        SCI_ASSERT(!status); /*assert verified*/
    }

    return;
}


uint32 ISP_ServiceGetFrameAddress(void)
{
    return 0;
}


void ISP_ServiceFreeFrameAddress(ISP_FRAME_T *frame_ptr)
{
    //ISP_SERVICE_LOG:"ISP_SERVICE: ISP_ServiceFreeFrameAddress, frame 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1577_112_2_18_1_43_58_311,(uint8*)"d",frame_ptr);

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
    uint32                  status,actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartVT"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1603_112_2_18_1_43_58_312,(uint8*)"");
    if(s->err_code)
    {
        ISP_SERVICE_TRACE("ISP:StartVT,error code0x%x",s->err_code);
        return;
    }

    if(ISP_STATE_PREVIEW == s->state)
    {
        ISP_ServiceStopPreview();
    }

    s->service = ISP_SERVICE_VP;
    s->err_code = ISP_ERR_CODE_NO_ERR;
    s->restart_counter = 0;

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_VP,
                            0);

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

    if(status)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = ISP_ERR_CODE_ST_ERR;
    }

    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceStartVT, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1635_112_2_18_1_43_58_313,(uint8*)"d", s->service);
    return ;


}
void ISP_ServiceStartVTExtScale(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status,actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartVTExtScale"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1645_112_2_18_1_43_58_314,(uint8*)"");

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_VP_EX_SCALE,
                            0);

    status = SCI_GetEvent(s_isp_event,
                          ISP_SCALE_EVENT,
                          SCI_AND_CLEAR,
                          &actual_event,
                          ISP_WAIT_TIME);
    if(status)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = ISP_ERR_CODE_ST_ERR;
    }

    return ;
}
void ISP_ServiceStartVTExt(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status,actual_event;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartVTExt"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1665_112_2_18_1_43_58_315,(uint8*)"");

    if(s->err_code)
    {
        ISP_SERVICE_TRACE("ISP:StartVTExt,error code0x%x",s->err_code);
        return;
    }


    if(ISP_STATE_PREVIEW == s->state)
    {
        ISP_ServiceStopPreview();
    }

    s->service = ISP_SERVICE_VP_EX;
    s->err_code = ISP_ERR_CODE_NO_ERR;
    s->restart_counter = 0;

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_VP_EX,
                            0);

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

    if(status)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = ISP_ERR_CODE_ST_ERR;
    }


//    LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id, PERMIT_GUI_INVALIDATE);
    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceStartVTExt, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1698_112_2_18_1_43_58_316,(uint8*)"d", s->service);
    return ;


}

void ISP_ServiceStartVTExtLowMem(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status,actual_event;

    //ISP_SERVICE_TRACE("ISP_SERVICE:ISP_ServiceStartVTExtLowMem" );

    if(ISP_STATE_PREVIEW == s->state)
    {
        ISP_ServiceStopPreview();
    }

    s->service = ISP_SERVICE_VP_EX_LOW_MEM;
    s->err_code = ISP_ERR_CODE_NO_ERR;
    s->restart_counter = 0;

    _ISP_ServiceSendRequest(0,
                            ISP_SERVICE_START_SIG,
                            ISP_SERVICE_VP_EX_LOW_MEM,
                            0);

    s->is_first_frame = 1;
    s->b_wait_for_vt_event = 1;
    s->vt_stop_arrived = 0;

    status = SCI_GetEvent(s_isp_event,
                          ISP_VT_EVENT,
                          SCI_AND_CLEAR,
                          &actual_event,
                          ISP_WAIT_TIME);

    if(status)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = ISP_ERR_CODE_ST_ERR;
    }

    LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id, NO_PERMIT_GUI_INVALIDATE);

    ISP_SERVICE_LOG("ISP:StartVTExtLowMem, service = %d",  s->service);

    return ;

}

void ISP_ServiceContinueVTExtLowMem(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_frame = SCI_NULL;

    ISP_SERVICE_TRACE("ISP:VTExtLowMem, buffer free!" );

    if(ISP_SERVICE_VP_EX_LOW_MEM == s->service)
    {
        p_frame = (ISP_FRAME_T*)ISP_SERVICE_ALLOC(sizeof(ISP_FRAME_T));
        p_frame->flags = ISP_STATE_VP_LOW_MEM_BUFF_FREE;
        _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,ISP_SERVICE_PATH_SIG,0,(void*)p_frame);
    }
    else
    {
        ISP_SERVICE_LOG("ISP:VTExtLowMem, err");
        s->err_code = ISP_ERR_CODE_ST_ERR;
    }

    return;
}


uint32 ISP_ServiceSetVTDecodeAddr(ISP_ADDRESS_T *addr_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status,actual_event;
    ISP_ADDRESS_T           *p_addr;

    if(s->service != ISP_SERVICE_VP || SCI_NULL == addr_ptr)
        return SCI_ERROR;

    if(ISP_YUV_INVALIDE(addr_ptr->yaddr,addr_ptr->uaddr,addr_ptr->vaddr)||
       (addr_ptr->yaddr % ISP_ALIGNED_BYTES) != 0 ||
       (addr_ptr->uaddr % ISP_ALIGNED_BYTES) != 0 )
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1719_112_2_18_1_43_58_317,(uint8*)"ddd",addr_ptr->yaddr,addr_ptr->uaddr,addr_ptr->vaddr);
        return SCI_ERROR;
    }


    if(1 == s->is_first_frame)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:wait for path1 done"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1726_112_2_18_1_43_58_318,(uint8*)"");
        return SCI_SUCCESS;
    }

    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceSetVTDecodeAddr"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1730_112_2_18_1_43_58_319,(uint8*)"");

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
    uint32                  status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetVTExtScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1757_112_2_18_1_43_58_320,(uint8*)"");
    //ISP_SERVICE_LOG:"ISP_SERVICE:input_format = %d,input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1763_112_2_18_1_43_58_321,(uint8*)"ddddd",param_ptr->input_format, param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);
    //ISP_SERVICE_LOG:"ISP_SERVICE:input_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1767_112_2_18_1_43_58_322,(uint8*)"ddd", param_ptr->input_addr.yaddr, param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);
    //ISP_SERVICE_LOG:"ISP_SERVICE:output_y_addr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1771_112_2_18_1_43_58_323,(uint8*)"ddd", param_ptr->output_addr.yaddr, param_ptr->output_addr.uaddr,param_ptr->output_addr.vaddr);
    //ISP_SERVICE_LOG:"ISP_SERVICE:output_format = %d, output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1775_112_2_18_1_43_58_324,(uint8*)"ddd",param_ptr->output_format,param_ptr->output_size.w,param_ptr->output_size.h);

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
    uint32                  status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetVTDecodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1811_112_2_18_1_43_58_325,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1817_112_2_18_1_43_58_326,(uint8*)"dddd",param_ptr->remote_input_range.x,param_ptr->remote_input_range.y,param_ptr->remote_input_range.w,param_ptr->remote_input_range.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:remote_disp_lcd_id=%d, remote_disp_block_id=%d,remote_disp_range.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1825_112_2_18_1_43_58_327,(uint8*)"dddddd",param_ptr->remote_disp_lcd_id,param_ptr->remote_disp_block_id,param_ptr->remote_disp_range.x, param_ptr->remote_disp_range.y, param_ptr->remote_disp_range.w, param_ptr->remote_disp_range.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,uaddr = %d, vaddr = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1831_112_2_18_1_43_58_328,(uint8*)"dddd",param_ptr->deblock_enable,param_ptr->input_addr.yaddr,param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);


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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1864_112_2_18_1_43_58_329,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }
    return status;
}
uint32 ISP_ServiceSetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_VT_ENCODE_PARAM_T   *p_local_para = SCI_NULL;
    uint32                  status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceSetVTEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1878_112_2_18_1_43_58_330,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1884_112_2_18_1_43_58_331,(uint8*)"dddd",param_ptr->local_input_range.x,param_ptr->local_input_range.y,param_ptr->local_input_range.w,param_ptr->local_input_range.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d},skip_frames %d, deci_frames %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1891_112_2_18_1_43_58_332,(uint8*)"ddddd",param_ptr->yuv_type,param_ptr->encode_size.w,param_ptr->encode_size.h,param_ptr->cap_param.cap_frame_skipped,param_ptr->cap_param.cap_frame_decimated);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1899_112_2_18_1_43_58_333,(uint8*)"dddddd",param_ptr->local_disp_lcd_id,param_ptr->local_disp_block_id,param_ptr->local_disp_range.x, param_ptr->local_disp_range.y, param_ptr->local_disp_range.w,param_ptr->local_disp_range.h);


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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1930_112_2_18_1_43_59_334,(uint8*)"d",s->err_code);
            status = s->err_code;
        }
    }

    return status;
}
uint32 ISP_ServiceSetVTExtEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_VT_ENCODE_PARAM_T   *p_local_para = SCI_NULL;
    uint32                  status,actual_event;

    SCI_ASSERT(SCI_NULL!=param_ptr); /*assert verified*/

    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceSetVTExtEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1945_112_2_18_1_43_59_335,(uint8*)"");

    //ISP_SERVICE_TRACE:"ISP_SERVICE:input.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1951_112_2_18_1_43_59_336,(uint8*)"dddd",param_ptr->local_input_range.x,param_ptr->local_input_range.y,param_ptr->local_input_range.w,param_ptr->local_input_range.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:yuv_type = %d,output.w,h {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1956_112_2_18_1_43_59_337,(uint8*)"ddd",param_ptr->yuv_type,param_ptr->encode_size.w,param_ptr->encode_size.h);

    //ISP_SERVICE_TRACE:"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1964_112_2_18_1_43_59_338,(uint8*)"dddddd",param_ptr->local_disp_lcd_id,param_ptr->local_disp_block_id,param_ptr->local_disp_range.x, param_ptr->local_disp_range.y, param_ptr->local_disp_range.w,param_ptr->local_disp_range.h);

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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_1994_112_2_18_1_43_59_339,(uint8*)"d",s->err_code);
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2013_112_2_18_1_43_59_340,(uint8*)"");
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

    _ISP_ServiceGetCapParam(&param_ptr->cap_param);

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2078_112_2_18_1_43_59_341,(uint8*)"");

    if(s->service != ISP_SERVICE_VP)
    {
        return ;
    }
    s->vt_stop_arrived = 1;
    s->local_review_enable = 0;
    s->remote_review_enable = 0;
    s->local_display_enable = 0;
    s->remote_display_enable = 0;

    _ISP_ServiceCloseTimer();

    _ISP_ServiceWaitForDone(); //wait for the ISP action done
    _ISP_ServiceFlushQueue();
    _ISP_ServiceExitWait();

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


    LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceStopVT, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2114_112_2_18_1_43_59_342,(uint8*)"d", s->service);
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2127_112_2_18_1_43_59_343,(uint8*)"");

    if(s->service != ISP_SERVICE_VP_EX)
    {
        return ;
    }
    s->vt_stop_arrived = 1;
    s->local_review_enable = 0;
    s->local_display_enable = 0;

    _ISP_ServiceCloseTimer();

    _ISP_ServiceWaitForDone(); //wait for the ISP action done
    _ISP_ServiceFlushQueue();
    _ISP_ServiceExitWait();

    _ISP_ServiceSendRequest(0,ISP_SERVICE_STOP_SIG,0,0);

    status = SCI_GetEvent(s_isp_event,
                          ISP_STOP_EVENT,
                          SCI_AND_CLEAR,
                          &actual_event,
                          ISP_WAIT_TIME);
    s->local_review_enable = local_review_en;
    s->local_display_enable = local_display_en;

    _ISP_ServiceEncodeFrameDeInit();
    s->display_block.is_enable = 0;


    LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceStopVTExt, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2157_112_2_18_1_43_59_344,(uint8*)"d", s->service);
    return ;

}

PUBLIC int ISP_ServiceDumpToFile(void *addr, uint32 len, char *misc, char *ext)
{
    const uint16 RAW_DIR[] = {'D', ':', '\\', 0};
    const uint16 RAW_FILE_PATH[] = {'P', 'h', 'o', 't', 'o', 's', '\\', 0};
    uint16 FileName[128];
    char tmpchr[64];
    uint32 bytes_written;
    SFS_HANDLE handle;
    static uint32 time = 0;
    unsigned int i = 0, j = 0, k = 0;
    SFS_ERROR_E result;
    SFS_DEVICE_FORMAT_E format;

    SCI_ASSERT(addr && (len > 0) && ext);
    result = SFS_GetDeviceStatus(RAW_DIR);
    if (result != SFS_ERROR_NONE) { // TF first
        result = SFS_RegisterDevice(RAW_DIR, &format);
        if (result != SFS_ERROR_NONE) {
            SCI_TRACE_LOW("ISP:dumpfile err1");
            return -1;
        }
        result = SFS_GetDeviceStatus(RAW_DIR);
        if (result != SFS_ERROR_NONE) {
            SCI_TRACE_LOW("ISP:dumpfile err2");
            return -1;
        }
    }
    memcpy(FileName, RAW_DIR, sizeof(RAW_DIR));
    j = (sizeof(RAW_DIR) / sizeof(uint16) - 1);
    memcpy(FileName + j, RAW_FILE_PATH, sizeof(RAW_FILE_PATH));
    j += (sizeof(RAW_FILE_PATH) / sizeof(uint16) - 1);

    if(!ISP_ServiceGetDumpFrameNum())
    {
        time = SCI_GetTickCount();
    }
    else
    {
        time = (time++ % ISP_ServiceGetDumpFrameNum()) ? time : 1;
    }
    i = snprintf(tmpchr, sizeof(tmpchr), "dump_%ld", time);
    for (k = 0; k < i; k++)
        FileName[j++] = tmpchr[k];
    if (misc != NULL) {
        for (i = 0; i < (uint32)strlen(misc); i++)
            FileName[j++] = misc[i];
    }
    if (ext[0] != '.') {
        FileName[j++] = (uint16)'.';
    }
    for (i = 0; i < (uint32)strlen(ext); i++)
        FileName[j++] = ext[i];
    FileName[j] = 0;

    handle =
        SFS_CreateFile(FileName, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);
    if(handle == 0)
    {
        handle =
            SFS_CreateFile(FileName, SFS_MODE_CREATE_NEW | SFS_MODE_WRITE, 0, 0);
    }

    if (!handle) {
        SCI_TRACE_LOW("ISP:Create file fail");
        return -1;
    }
    // write
    SFS_WriteFile(handle, (void *)addr, len, &bytes_written, NULL);
    SFS_FlushFile(handle, NULL);
    SFS_CloseFile(handle);
    SCI_TRACE_LOW("write done 0x%x 0x%x", len, bytes_written);

    return 0;
}

PUBLIC int ISP_ServiceDumpToDDR(void *ptr)
{
    ISP_FRAME_T* dump_isp_frame_info = (ISP_FRAME_T*)ptr;

    if (dump_frame_data == NULL)
    {
        //SCI_TRACE_LOW("dump_frame_data is null, malloc for it");
        dump_frame_data = (uint32*)malloc(sizeof(uint8)*dump_isp_frame_info->height*dump_isp_frame_info->width*2);
        if(dump_frame_data == NULL)
        {
            SCI_TRACE_LOW("ISP:dumpddr malloc error");
            return -1;
        }
    }

    //memset(dump_frame_data, 0, sizeof(uint8)*dump_isp_frame_info->height*dump_isp_frame_info->width*2);
    memcpy(dump_frame_data, dump_isp_frame_info->yaddr, sizeof(uint8)*dump_isp_frame_info->height*dump_isp_frame_info->width*2);

    SCI_TRACE_LOW("ISP:dumpddr  addr = 0X%X, frame yaddr = 0x%x, h = %d, w = %d",
            dump_frame_data, dump_isp_frame_info->yaddr, dump_isp_frame_info->height, dump_isp_frame_info->width);

    return 0;
}




LOCAL void _ISP_ServiceCreate(void)
{
    uint32                 status = SCI_SUCCESS;
    /* AGORA_PATCH start */
    uint32                 thread_pori =  67;//PRI_ISP_SERVICE_TASK;
    /* AGORA_PATCH end */
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

    if(SCI_NULL == g_CamDataMutex)
    {
        g_CamDataMutex = (SCI_MUTEX_PTR)SCI_CreateMutex("cam data ctrl", SCI_INHERIT);
        SCI_ASSERT(g_CamDataMutex != SCI_NULL); /*assert verified*/
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

LOCAL void _ISP_ServicePostDisplayHandle(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             frame = {0};
    uint32                  block_id = 0;
    uint32                  rtn = SCI_SUCCESS;

    frame.flags = ISP_STATE_VP_LOW_MEM_BUFF_FREE;

    if(ISP_SERVICE_VP_EX_LOW_MEM == s->service)
    {

        rtn = ISP_ServiceGetVtMemBlockId(&block_id, ISP_VT_MEMBLK_INVALIDING);
        if(rtn)
        {
            ISP_SERVICE_TRACE("ISP:PostDisp err %d",rtn);
            s->err_code = ISP_ERR_CODE_ST_ERR;
            return;
        }

       ISP_SERVICE_TRACE("ISP:PostDisp, the block %d from %d to %d!",
                                           block_id,
                                            ISP_VT_MEMBLK_INVALIDING,
                                            ISP_VT_MEMBLK_IDLE);
        ISP_SERVICE_TRACE("ISP:PostDisp, block addr 0x%x 0x%x!",
                                            s->display_addr[block_id].yaddr,
                                            s->display_addr[block_id].uaddr);

        ISP_ServiceSetVtMemBlockState(block_id, ISP_VT_MEMBLK_IDLE);

        //send buffer free message to isp_service
        _ISP_ServiceVTExtHandlerLowMem(&frame);
    }

    return;
}

LOCAL void _ISP_ServiceMainMessageLoop(uint32 argc, void *argv)
{
    ISP_SIGNAL_T            *sig_ptr = SCI_NULL;

    while (TRUE)/*lint !e716*/
    {
        sig_ptr = (ISP_SIGNAL_T *)SCI_GetSignal(ISP_SERVICE_THREAD);
        SCI_ASSERT (sig_ptr);/*assert verified*/

        //ISP_SERVICE_TRACE:"ISP_Service: _ISP_ServiceMainMessageLoop,command=0x%x param=0x%x SignalCode 0x%x sig_ptr->ptr 0x%x time %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2255_112_2_18_1_43_59_345,(uint8*)"ddddd",sig_ptr->command , sig_ptr->param,sig_ptr->sig.SignalCode,sig_ptr->ptr,SCI_GetTickCount());
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

            case ISP_SERVICE_PATH_SIG:

                _ISP_ServicePathSignalHandler(sig_ptr->sig.SignalCode,
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

            case ISP_SERVICE_DISP_SIG:

                if(ISP_SERVICE_VP_EX_LOW_MEM == s_isp_service.service)
                {
                    _ISP_ServiceDisplayOneFrameLowMem(sig_ptr->param, sig_ptr->ptr);
                }
                else
                {
                    _ISP_ServiceDisplayOneFrame(sig_ptr->param, sig_ptr->ptr);
                }
            break;

            case ISP_SERVICE_PREVIEW_START_SIG:
                _ISP_Uix_ServiceGetDataCtrl();
                Cam_preview_start();
                _ISP_Uix_ServiceRelDataCtrl();
            break;

            case ISP_SERVICE_PREVIEW_PROCESS_SIG:
                _ISP_Uix_ServiceGetDataCtrl();
                Cam_preview_process();
                _ISP_Uix_ServiceRelDataCtrl();
            break;

            case ISP_SERVICE_CAP_START_SIG:
                _ISP_Uix_ServiceGetDataCtrl();
                Cam_snapshot_start();
                _ISP_Uix_ServiceRelDataCtrl();
            break;

            case ISP_SERVICE_CAP_PROCESS_SIG:
                _ISP_Uix_ServiceGetDataCtrl();
                Cam_snapshot_process();
                _ISP_Uix_ServiceRelDataCtrl();
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
    DPSLP_ISP_EnableAhbSleep(DISABLE_AHB_SLEEP);

    hal_CameraIrqSetHandler(_ISP_UixIrqHandler);

    ISP_UixOpenCameraContoller();

    _ISP_ServiceInit();

    SCI_TRACE_LOW("ISP:OpenSignalHandler");
    SCI_SetEvent(s_isp_event, ISP_OPEN_EVENT, SCI_OR);

    return ;
}

LOCAL void _ISP_ServiceCloseSignalHandler(uint32 service,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->b_drv_init_ed)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->b_drv_init_ed = 0;
    }

    ISP_DriverModuleDisable(AHB_REG_BASE);

   _ISP_ServiceDeInit();// event, mutex, semaphore will be released here

    DPSLP_ISP_EnableAhbSleep(ENABLE_AHB_SLEEP);

    _ISP_Uix_ServiceGetDataCtrl();

    g_stopPreview = 1;
    if (gIsCameraStartXfer == TRUE)
    {
        SCI_TRACE_LOW("ISP:stop xfer!");
        hal_CameraStopXfer(FALSE);
        gIsCameraStartXfer = FALSE;
    }
    hal_CameraClose();
    g_DcamMode = DCAMERA_MODE_PREVIEW;

    _ISP_Uix_ServiceRelDataCtrl();

    SCI_SetEvent(s_isp_event, ISP_CLOSE_EVENT, SCI_OR);

    return ;
}

LOCAL void _ISP_ServicePathSignalHandler(uint32 service,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_frame = (ISP_FRAME_T*)ptr;
    ISP_USER_FUNC_PTR       user_cb = SCI_NULL;
    uint32                  status = 0;
    int                     cb_rtn = 0;

    switch(s->service)
    {
        case ISP_SERVICE_PREVIEW:

            //ISP_SERVICE_TRACE:"ISP_SERVICE: display one frame ,p_frame = 0x%x ,y,u,v {0x%x,0x%x,0x%x}"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2384_112_2_18_1_43_59_347,(uint8*)"dddd",p_frame,            p_frame->yaddr,p_frame->uaddr,p_frame->vaddr);

            cb_rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);

            if(ISP_CB_PROC_DONE == cb_rtn)
            {
                if(s->display_user_cb != NULL && g_DcamMode == DCAMERA_MODE_VIDEOCALL)
                {
                    //
                }
                else
                {
                    _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
                }
            }
            else
            {
                s->disp_blk_id = s->display_block.block_id;
                // async callback, user will call ISP_ServiceAsyncDisplay to update LCD
            }

            if(s->is_first_frame)
            {
                ISP_DriverFrameUnLock(p_frame);
                status = SCI_SetEvent(s_isp_event, ISP_DISPLAY_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
                s->is_first_frame = 0;
            }
            else
            {
                //ISP_DriverFrameUnLock(p_frame->prev);
                ISP_DriverFrameUnLock(p_frame);
            }

            break;

        case ISP_SERVICE_MPEG:
            //_ISP_ServiceMpegHandler(p_frame);
            _ISP_Uix_ServiceMpegHandler(p_frame);

            break;

        case ISP_SERVICE_VP:

            _ISP_ServiceVTHandler(p_frame);

        break;

        case ISP_SERVICE_VP_EX:

            _ISP_ServiceVTExtHandler(p_frame);

        break;

        case ISP_SERVICE_VP_EX_LOW_MEM:

            _ISP_ServiceVTExtHandlerLowMem(p_frame);

        break;

        case ISP_SERVICE_JPEG:

            _ISP_ServiceJPEGHandler(p_frame);

        break;

        case ISP_SERVICE_REVIEW:

            _ISP_ServiceGetDispCtrl();
            user_cb = s->display_user_cb;
            if(SCI_NULL != user_cb)
            {
                cb_rtn = (*user_cb)(p_frame);
            }
            _ISP_ServiceRelDispCtrl();

            if(ISP_CB_PROC_DONE == cb_rtn)
            {
                _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
            }
            else
            {
                s->disp_blk_id = s->display_block.block_id;
            }

            if(s->is_first_frame)
            {
                s->is_first_frame = 0;
            }

            if(SCI_NULL == user_cb)
            {
                status = SCI_SetEvent(s_isp_event, ISP_DISPLAY_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
            }
        break;

        case ISP_SERVICE_DENOISE:

            _ISP_ServiceGetDispCtrl();
            user_cb = s->display_user_cb;
            _ISP_ServiceRelDispCtrl();

            if(SCI_NULL != user_cb)
            {
                (*user_cb)(p_frame);
            }
            else
            {
                status = SCI_SetEvent(s_isp_event, ISP_DENOISE_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
            }

        break;

        case ISP_SERVICE_SCALE:
            _ISP_ServiceScaleHandler(p_frame);

        break;

        default:

        break;
    }
}

LOCAL void _ISP_ServiceSetParamSignalHandler(uint32 service,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  status = 0;

    s->err_code = 0;

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

        case ISP_SERVICE_DENOISE:
        {
            ISP_DENOISE_PARAM_T *param_ptr = (ISP_DENOISE_PARAM_T *)ptr;
            _ISP_ServiceSetDenoiseParam(param_ptr);

            break;
         }

        default:

        break;
    }

    status = SCI_SetEvent(s_isp_event, ISP_SET_PARAM_EVENT, SCI_OR);
    SCI_ASSERT(!status); /*assert verified*/

#if defined(MAINLCD_DEV_SIZE_240X284)// wuxx add for CAMERA BF20A6,GC6133, 240X284 Ô¤ÀÀÈ«ÆÁ, and isp_service.a
    return ;
#endif
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


            LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id,
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

                LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id,
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


            LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id,
                             LCD_BRUSH_MODE_ASYNC);

            _ISP_ServiceStartVTEncode();

        break;

        case ISP_SERVICE_VP_EX:

            if(s->local_display_enable)
            {
                if(s->post_cap_user_cb)
                {

                    LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id,
                                 LCD_BRUSH_MODE_SYNC);
                }
                else
                {

                    LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id,
                                 LCD_BRUSH_MODE_ASYNC);
                }
            }
            _ISP_ServiceStartVTExtEncode();

        break;

        case ISP_SERVICE_VP_EX_LOW_MEM:

            if(s->local_display_enable)
            {
                    LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id,
                                 LCD_BRUSH_MODE_ASYNC);
            }
            _ISP_ServiceStartVTExtEncodeLowMem();

        break;

        case ISP_SERVICE_VP_EX_SCALE:

            _ISP_ServiceStartVTExtScale();

        break;

        case ISP_SERVICE_DENOISE:

            _ISP_ServiceStartDenoise();

        break;

        default:

        break;

    }


    if(s->err_code)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->b_drv_init_ed = 0;
    }
    else
    {
        s->b_drv_init_ed = 1;
    }

    return ;
}

LOCAL void _ISP_ServiceStopSignalHandler(uint32 service,uint32 param,void * ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_DriverStop(s->module_addr);
    // ISP_DriverSoftReset(AHB_REG_BASE);
    if(s->b_drv_init_ed)
    {
        //ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->b_drv_init_ed = 0;
    }

    //SCI_TRACE_LOW("david capture ISP_Uix_StopPreview!");
    ISP_Uix_StopPreview();

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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2771_112_2_18_1_44_0_348,(uint8*)"");
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
        case ISP_SERVICE_PATH_SIG:

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2832_112_2_18_1_44_0_349,(uint8*)"ddd",enqueued, store, count);
    return store;

}

LOCAL void _ISP_ServiceGetImageFromLCDC(ISP_FRAME_T *p_isp_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    LCD_CAP_PARAM_T        lcd_cap_t = {0};
    uint32                         frame_size = 0;
    LCD_BRUSH_MODE_E      brush_mode = LCD_BRUSH_MODE_ASYNC;
    LCDC_RECT_T               lcd_rect = {0};
    uint32                         buffer_nb = 0;

    frame_size = s->encoder_size.w * s->encoder_size.h * 2 + ISP_MEM_STRIDER_FOR_ONE_FRAME;
    lcd_cap_t.format = LCD_RGB565;
    if(s->disp_frm_round_num < 2) // CID37100
    {
        lcd_cap_t.dst_base_addr = SCI_ALLOCA(frame_size+ISP_ALIGNED_BYTES);
        if(0 ==lcd_cap_t.dst_base_addr)
        {
            ISP_SERVICE_TRACE("ISP:GetImage not enough memory!");
            return;
        }
        else
        {
            lcd_cap_t.dst_base_addr = ISP_ADDRESS_ALIGNED_(lcd_cap_t.dst_base_addr, ISP_ALIGNED_BYTES);
        }

    }
    else
    {
        lcd_cap_t.dst_base_addr = (uint32)LCD_GetBlockBuffer((LCD_ID_E)s->display_block.lcd_id, s->display_block.block_id);
        for(buffer_nb = 0; buffer_nb < s->disp_frm_round_num; buffer_nb++)
        {
            if(s->display_addr[buffer_nb].yaddr == lcd_cap_t.dst_base_addr)
            {
                break;
            }
        }

        ISP_SERVICE_TRACE("ISP:GetImage block is %d, round_nb %d",  buffer_nb, s->disp_frm_round_num);

        if(0 == buffer_nb)
        {
            lcd_cap_t.dst_base_addr = s->display_addr[s->disp_frm_round_num - 1].yaddr;
        }
        else
        {
            lcd_cap_t.dst_base_addr = s->display_addr[0].yaddr;
        }

    }

 /*
    lcd_cap_t.dst_base_addr = (uint32)ISP_ServiceGetMem(&mem_size) - frame_size;
    lcd_cap_t.dst_base_addr = ISP_ADDRESS_ALIGNED_(lcd_cap_t.dst_base_addr,
                                                   ISP_ALIGNED_BYTES);
                                                   */
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

    if(_ISP_ServiceIsLittleEndian())
    {
        lcd_cap_t.endian_switch = 2;
    }
    else
    {
        lcd_cap_t.endian_switch = 0;
    }
    lcd_cap_t.cap_en = SCI_TRUE;


    brush_mode = LCD_GetBrushMode((LCD_ID_E)s->display_block.lcd_id);
    LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id, LCD_BRUSH_MODE_SYNC);


    LCD_GetLcdCtrlSemaphore((LCD_ID_E)s->display_block.lcd_id);


    LCD_ConfigCapture((LCD_ID_E)s->display_block.lcd_id,&lcd_cap_t);

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


    LCD_ConfigCapture((LCD_ID_E)s->display_block.lcd_id,&lcd_cap_t);
    LCD_PutLcdCtrlSemaphore((LCD_ID_E)s->display_block.lcd_id);


    LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id, brush_mode);

    p_isp_frame->yaddr = lcd_cap_t.dst_base_addr;
    p_isp_frame->uaddr = lcd_cap_t.dst_base_addr + (s->encoder_size.w * s->encoder_size.h);//
    p_isp_frame->uaddr = ISP_ADDRESS_ALIGNED_(p_isp_frame->uaddr,
                                              ISP_ALIGNED_BYTES);
    p_isp_frame->width = s->encoder_size.w;
    p_isp_frame->height = s->encoder_size.h;

    return ;
}

LOCAL void _ISP_ServiceClearBlockBuf(ISP_DISP_BLOCK_T *p_disp_blk)
{
    uint32                  *p_blk_buf = SCI_NULL;
    uint32                  blk_buf_len = 0;


    p_blk_buf = LCD_GetBlockBuffer((LCD_ID_E)p_disp_blk->lcd_id,p_disp_blk->block_id);
    blk_buf_len = (uint32)(p_disp_blk->img_rect.w * p_disp_blk->img_rect.h);

    if(p_disp_blk->img_fmt == ISP_DATA_YUV422||p_disp_blk->img_fmt == ISP_DATA_YUV420)
    {
        //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2929_112_2_18_1_44_0_350,(uint8*)"dd",p_blk_buf,blk_buf_len);

        SCI_MEMSET((void*)p_blk_buf,
                    0,
                    blk_buf_len);

        p_blk_buf = LCD_GetBlockUVBuffer((LCD_ID_E)p_disp_blk->lcd_id,p_disp_blk->block_id);

        if(p_disp_blk->img_fmt == ISP_DATA_YUV420)      //uv buffer length /2 for 420 buffer
        {
            blk_buf_len>>=1;
        }

        //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2943_112_2_18_1_44_0_351,(uint8*)"dd",p_blk_buf,blk_buf_len);

        SCI_MEMSET((void*)p_blk_buf,
                    0x80,
                    blk_buf_len);
    }
    else
    {
        //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceClearBlockBuf, blk buffer 0x%x buf len 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2953_112_2_18_1_44_1_352,(uint8*)"dd",p_blk_buf,blk_buf_len);

        SCI_MEMSET((void*)p_blk_buf,
                    0,
                    blk_buf_len << 1);

    }

}

LOCAL void _ISP_ServiceCloseBlock(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceCloseBlock disp_blk enable %d remote disp blk enable %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_2969_112_2_18_1_44_1_353,(uint8*)"dd",s->display_block.is_enable,s->remote_display_block.is_enable);

    if(s->display_block.is_enable == 1 &&
       s->display_block.lcd_id < LCD_SUM &&
       s->display_block.block_id < BLOCK_SUM)
    {
        LCD_GetLcdCtrlSemaphore(0);

        if(s->service == ISP_SERVICE_REVIEW)
        {
            _ISP_ServiceClearBlockBuf(&s->display_block);
        }

        LCD_DisableBlock((LCD_ID_E)s->display_block.lcd_id,s->display_block.block_id);
        LCD_PutLcdCtrlSemaphore(0);
        s->display_block.is_enable = 0;
    }

    if(s->remote_display_block.is_enable == 1 &&
       s->remote_display_block.lcd_id < LCD_SUM &&
       s->remote_display_block.block_id < BLOCK_SUM)
    {
        LCD_GetLcdCtrlSemaphore(0);

        LCD_DisableBlock((LCD_ID_E)s->remote_display_block.lcd_id,s->remote_display_block.block_id);
        LCD_PutLcdCtrlSemaphore(0);
        s->remote_display_block.is_enable = 0;

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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3008_112_2_18_1_44_1_354,(uint8*)"");
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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3031_112_2_18_1_44_1_355,(uint8*)"dd",per_lev,s->watchdog_feeded);

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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3052_112_2_18_1_44_1_356,(uint8*)"");
        break;
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3079_112_2_18_1_44_1_357,(uint8*)"ddd", timer_mode, expire_value, ret_value);

    return ret_value;
}

LOCAL uint32 _ISP_ServiceGetPerformanceLevel(void)
{
    uint32                  curr_available = 0;
    uint32                  performance = ISP_SERVICE_PERFORMANCE_HIGH;

    curr_available = _ISP_ServiceGetQueueAvailable();
    if(curr_available < ISP_QUEUE_NUM - 20)
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

LOCAL void    _ISP_ServiceWaitForDone(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_LOG:"_ISP_ServiceWaitForDone, before %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3110_112_2_18_1_44_1_358,(uint8*)"d", SCI_GetTickCount());

    ISP_DriverWaitForCurActionDone(s->module_addr);

    //ISP_SERVICE_LOG:"_ISP_ServiceWaitForDone, after %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3114_112_2_18_1_44_1_359,(uint8*)"d", SCI_GetTickCount());

    return;
}

LOCAL void    _ISP_ServiceExitWait(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_LOG:"_ISP_ServiceExitWait, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3123_112_2_18_1_44_1_360,(uint8*)"d", SCI_GetTickCount());

    ISP_DriverExitWaitForActionDone(s->module_addr);

    return;
}

#endif

LOCAL BOOLEAN _ISP_ServiceIsLittleEndian(void)
{
    uint32                    cell = 0x5A;

    return (*(uint8*)&cell == 0x5A);
}

LOCAL void _ISP_ServiceDeleteTimer(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(PNULL != s->timer_ptr)
    {
        SCI_DeleteTimer(s->timer_ptr);
        s->timer_ptr = PNULL;

        //ISP_SERVICE_TRACE:"_ISP_ServiceDeleteTimer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3148_112_2_18_1_44_1_361,(uint8*)"");
    }
}

LOCAL BOOLEAN _ISP_ServiceCreateTimer(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    BOOLEAN                 ret_val = SCI_FALSE;

    s->timer_ptr = SCI_CreatePeriodTimer("ISP_VT_TIMER",
                                         SCI_NULL,
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3168_112_2_18_1_44_1_362,(uint8*)"d", ret_val);
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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3185_112_2_18_1_44_1_363,(uint8*)"");
        }
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


LOCAL void _ISP_ServiceConvertCoor( LCD_ID_E lcd_id, ISP_RECT_T *src_ptr, ISP_RECT_T *dst_ptr,ISP_ROTATION_E rotate_ang)
{
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(lcd_id, &lcd_info);


    //ISP_SERVICE_TRACE:"_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3236_112_2_18_1_44_1_364,(uint8*)"ddddd", src_ptr->x, src_ptr->y, src_ptr->w, src_ptr->h,rotate_ang);

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3285_112_2_18_1_44_1_365,(uint8*)"dddd", dst_ptr->x, dst_ptr->y, dst_ptr->w, dst_ptr->h);

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

    SCI_MEMCPY((void*)&p_disp_block->lcd_inv_rect,(void*)p_inv_range,sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&p_disp_block->img_rect,(void*)p_range,sizeof(ISP_RECT_T));

    if(rot_angle == ISP_ROTATION_90 ||
       rot_angle == ISP_ROTATION_270) // to get the right size of target image
    {
       p_range->w = p_disp_block->img_rect.h;
       p_range->h = p_disp_block->img_rect.w;
    }
    //ISP_SERVICE_TRACE:"ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3316_112_2_18_1_44_1_366,(uint8*)"ddddd",p_disp_block->block_id,p_disp_block->img_rect.x,p_disp_block->img_rect.y,p_disp_block->img_rect.w,p_disp_block->img_rect.h);
    if(ISP_DISPLAY_NONE == p_disp_block->block_id)
    {
        p_disp_block->img_fmt =  ISP_IMAGE_FORMAT_RGB565;
        return ;
    }

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

    if(LCD_LAYER_IMAGE == p_disp_block->block_id)
    {
        if(ISP_ERR_CODE_NO_MEM == s->err_code)
        {
            p_disp_block->img_fmt = ISP_DATA_YUV420;
            blk_cfg.resolution = LCD_RESOLUTION_YUV420; //YUV422
        }
        else
        {
            p_disp_block->img_fmt = ISP_DATA_YUV422;
            blk_cfg.resolution = LCD_RESOLUTION_YUV422; //YUV422
        }
    }
    else
    {
        p_disp_block->img_fmt = ISP_DATA_RGB565;
        blk_cfg.resolution = LCD_RESOLUTION_RGB565; //RGB565
    }

    if(_ISP_ServiceIsLittleEndian())
    {
        if(LCD_RESOLUTION_RGB565 == blk_cfg.resolution)
        {
            blk_cfg.endian_switch = 2;
        }
        else
        {
            blk_cfg.endian_switch = 1;
        }
    }
    else
    {
        blk_cfg.endian_switch = 0;
    }


    //ISP_SERVICE_TRACE:"ISP_SERVICE: block id %d blk_cfg.start.x,y end_x,y {%d,%d,%d,%d}, resolution %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3371_112_2_18_1_44_1_367,(uint8*)"dddddd",p_disp_block->block_id,blk_cfg.start_x,blk_cfg.start_y,blk_cfg.end_x,blk_cfg.end_y,blk_cfg.resolution);

    blk_cfg.set_endian_flag = 1;

    LCD_GetLcdCtrlSemaphore((LCD_ID_E)p_disp_block->lcd_id);
    LCD_ConfigBlock((LCD_ID_E)p_disp_block->lcd_id,p_disp_block->block_id, blk_cfg);
    LCD_DisableBlock((LCD_ID_E)p_disp_block->lcd_id,p_disp_block->block_id);
    LCD_PutLcdCtrlSemaphore((LCD_ID_E)p_disp_block->lcd_id);
    p_disp_block->is_enable = 0;
    return ;
}

#if 0
void lcd_video_test(void)
{
    LCD_INFO_T  lcd_info;

    LCD_Backlight_ON();

    LCD_Init();
    LCD_Clear(MAIN_LCD_ID, 0xf000);

    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);

   if (1)
    {
        BLOCKCFG_T osd_cfg;

        SCI_MEMSET ( (void *) lcd_info.lcdbuff_ptr, 0x00, lcd_info.lcd_width * lcd_info.lcd_height * 2); //lint !e746

        _DisplayText(lcd_info.lcdbuff_ptr, lcd_info.lcd_width, lcd_info.lcd_height, 10, 100, "Video Test Layer", 16);

        osd_cfg.resolution       = LCD_RESOLUTION_RGB565;
        osd_cfg.type             = 0;
        osd_cfg.priority         = 0;
        osd_cfg.start_x          = 0;
        osd_cfg.start_y          = 0;
        osd_cfg.end_x            = lcd_info.lcd_width -1;
        osd_cfg.end_y            = lcd_info.lcd_height - 1;
        osd_cfg.width            =  lcd_info.lcd_width;
        osd_cfg.alpha            = 0x40;
        osd_cfg.alpha_sel        = 1;
        osd_cfg.colorkey_en      = TRUE;
        osd_cfg.colorkey         = 0x0000;

        LCD_ConfigBlock (MAIN_LCD_ID, 1, osd_cfg);

        LCD_SetBlockBuffer (MAIN_LCD_ID, 1, lcd_info.lcdbuff_ptr);

        LCD_EnableBlock(MAIN_LCD_ID, 1);
    }

    if (1)
    {
        uint32 *p_y, *p_uv;
        BLOCKCFG_T vid_cfg = {0};

        p_y = (uint32 *)video_frame_buff;
        p_uv = (uint32 *) (video_frame_buff + (176 * 144));

        LCD_SetBlockBuffer(MAIN_LCD_ID, 0, p_y);
        LCD_SetUVBuffer(MAIN_LCD_ID, 0, p_uv);

        vid_cfg.start_x = 20;
        vid_cfg.start_y = 20;
        vid_cfg.end_x = 220;
        vid_cfg.end_y = 300;
        vid_cfg.width = 176;
        vid_cfg.colorkey_en = 0;
        vid_cfg.alpha_sel = 1;
        vid_cfg.alpha = 0xFF;
        vid_cfg.priority = 0;
        vid_cfg.type = 1;
        vid_cfg.resolution = LCD_RESOLUTION_YUV420;

        LCD_SetImageLayerSourceSize(MAIN_LCD_ID, 176, 144);
        LCD_ConfigBlock(MAIN_LCD_ID, 0, vid_cfg);
        LCD_EnableBlock(MAIN_LCD_ID, 0);
    }

    LCD_Invalidate(MAIN_LCD_ID);
}
#endif

// preview, review, mpeg preview,vt preview,vt review
#if 1
LOCAL void _ISP_ServiceDisplayOneFrame(uint32 block_id,ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_ADDRESS_T           src_addr_t = {0};
    //ISP_ADDRESS_T           *p_addr =SCI_NULL;
    uint32                  disp_addr_y = 0;
    uint32                  disp_addr_u = 0;
    uint32                  rot_angle = ISP_ROTATION_0;
    uint32                  img_width = 0;
    uint32                  img_height = 0;
    ISP_DISP_BLOCK_T        *p_block = SCI_NULL;
    BLOCKCFG_T              blk_cfg = {0};
    uint32                  b_re_cfg = 0;
    int                     dump_mode = 0;

    if(ISP_DISPLAY_NONE == block_id)
    {
        SCI_TRACE_LOW("ISP: none");
        return ;
    }

    if(PNULL == p_frame)
    {
       ISP_SERVICE_LOG("ISP:Display frame point is NULL");
        return;
    }

    //SCI_TRACE_LOW("david preview displayoneframe start yaddr:0x%x tick:%u mode:%d",
                                //p_frame->yaddr, SCI_GetTickCount(), g_Capturemode);

    //ISP_SERVICE_LOG:"_ISP_ServiceDisplayOneFrame: frame addr 0x%x 0x%x"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3405_112_2_18_1_44_1_368,(uint8*)"dd",p_frame->yaddr, p_frame->uaddr);
    disp_addr_y = p_frame->yaddr;
    disp_addr_u = p_frame->uaddr;

    disp_addr_y += DCamera_get_GoudaOffset(g_DcamMode);

    dump_mode = ISP_ServiceGetDumpMode();
    switch(dump_mode)
    {
        case ISP_SERVICE_DUMP_DDR:
            ISP_ServiceDumpToDDR((void *)p_frame);
            break;

        case ISP_SERVICE_DUMP_FILE:
            ISP_ServiceDumpToFile(p_frame->yaddr, p_frame->width *p_frame->height * 2, "_data", ".yuv");
            break;

        default:
            break;
    }

#if 0
#ifndef CAMERA_ROTATION_DISPLAY
    if(!g_Capturemode)
    {
        // set gouda trim  (640-360)/2 == 140
        disp_addr_y += 140*2;
    }
    else
    {
        // set gouda trim  (320-180)/2 == 70
        disp_addr_y += 70*2;
    }
#else
    if(g_Capturemode)
    {
        //(hight-1)* stride
        disp_addr_y += (480-1)*640*2;
    }
    else
    {
        //(hight-1)* stride
        disp_addr_y += (240-1)*320*2;
    }

#endif
#endif

    if(!p_frame->yaddr)
    {
        ISP_SERVICE_LOG("ISP:Display y addr is NULL");
        return;
    }

    if(s->is_first_frame && ISP_SERVICE_VP_EX == s->service)
    {
        LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id, PERMIT_GUI_INVALIDATE);
        s->is_first_frame = 0;
    }

    LCD_GetLcdCtrlSemaphore(MAIN_LCD_ID);

    LCD_SetBlockBuffer(MAIN_LCD_ID, 0, (uint32 *)disp_addr_y);

    if(s->service == ISP_SERVICE_VP || s->service == ISP_SERVICE_VP_EX || s->is_first_frame)
    {
        b_re_cfg = 1;
    }

    //ISP_SERVICE_LOG:"ISP_SERVICE: block enable %d,p_block->lcd_id %d, p_block->block_id %d, re config %d "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3486_112_2_18_1_44_2_370,(uint8*)"dddd",LCD_GetBlockIsEnable((LCD_ID_E)p_block->lcd_id,p_block->block_id),p_block->lcd_id,p_block->block_id,b_re_cfg);

    if(b_re_cfg)
    {

#if 0
#ifndef CAMERA_ROTATION_DISPLAY
        if(!g_Capturemode)
        {
            blk_cfg.start_x = 0;
            blk_cfg.start_y = 0;
            blk_cfg.end_x = 239;
            blk_cfg.end_y = 319;
            blk_cfg.width = 640*2; //gouda stride.
            blk_cfg.colorkey_en = 0;
            blk_cfg.alpha_sel = 1;
            blk_cfg.alpha = 0xFF;
            blk_cfg.priority = 0;
            blk_cfg.type = 1;
            blk_cfg.resolution = LCD_RESOLUTION_YUV422;
            blk_cfg.rotation= 0;// 0: 0; 1: 90; clockwise
            //LCD_SetImageLayerSourceSize(MAIN_LCD_ID, 640, 480);
            LCD_SetImageLayerSourceSize(MAIN_LCD_ID, 360, 480);
            SCI_TRACE_LOW("david displayoneframe config 640*480.");
        }
        else
        {
            blk_cfg.start_x = 0;
            blk_cfg.start_y = 0;
            blk_cfg.end_x = 239;
            blk_cfg.end_y = 319;
            blk_cfg.width = 320*2; //gouda stride.
            blk_cfg.colorkey_en = 0;
            blk_cfg.alpha_sel = 1;
            blk_cfg.alpha = 0xFF;
            blk_cfg.priority = 0;
            blk_cfg.type = 1;
            blk_cfg.resolution = LCD_RESOLUTION_YUV422;
            blk_cfg.rotation= 0;// 0: 0; 1: 90; clockwise
            LCD_SetImageLayerSourceSize(MAIN_LCD_ID, 180, 240);
            SCI_TRACE_LOW("david displayoneframe config 320*240.");
        }
#else
        // capture preview
        if(g_Capturemode)
        {
            blk_cfg.start_x = 0;
            blk_cfg.start_y = 0;
            blk_cfg.end_x = 239;
            blk_cfg.end_y = 319;
            blk_cfg.width = 640*2;  //stride
            blk_cfg.colorkey_en = 0;
            blk_cfg.alpha_sel = 1;
            blk_cfg.alpha = 0xFF;
            blk_cfg.priority = 0;
            blk_cfg.type = 1;
            blk_cfg.resolution = LCD_RESOLUTION_YUV422;
            blk_cfg.rotation= 1;// 0: 0; 1: 90; clockwise
            LCD_SetImageLayerSourceSize(MAIN_LCD_ID, 480, 640);
            SCI_TRACE_LOW("david displayoneframe config 480*640.");
        }
        // record preview
        else
        {
            blk_cfg.start_x = 0;
            blk_cfg.start_y = 0;
            blk_cfg.end_x = 239;
            blk_cfg.end_y = 319;
            blk_cfg.width = 320*2;  //stride
            blk_cfg.colorkey_en = 0;
            blk_cfg.alpha_sel = 1;
            blk_cfg.alpha = 0xFF;
            blk_cfg.priority = 0;
            blk_cfg.type = 1;
            blk_cfg.resolution = LCD_RESOLUTION_YUV422;
            blk_cfg.rotation= 1;// 0: 0; 1: 90; clockwise
            LCD_SetImageLayerSourceSize(MAIN_LCD_ID, 240, 320);
            SCI_TRACE_LOW("david displayoneframe config 240*320.");
        }
#endif
#endif
        Dcamera_Gouda_config(g_DcamMode, &blk_cfg);

        LCD_ConfigBlock(MAIN_LCD_ID, 0, blk_cfg);
        LCD_EnableBlock(MAIN_LCD_ID, 0);
    }

    //ISP_SERVICE_TRACE:"_ISP_ServiceDisplayOneFrame: s->is_first_frame = %d, time %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3540_112_2_18_1_44_2_371,(uint8*)"dd",s->is_first_frame,SCI_GetTickCount());

#ifdef DC_WATCH_UI_SUPPORT
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_128X128
    LCD_InvalidateRect (MAIN_LCD_ID, 0, 0, 127, 127);  //For 8910FF 240x320 screen test
#elif defined MAINLCD_SIZE_240X320 || defined MAINLCD_SIZE_240X240
    #if defined(MAINLCD_DEV_SIZE_240X284)// wuxx add for CAMERA BF20A6,GC6133, 240X284 Ô¤ÀÀÈ«ÆÁ, and isp_service.a
    LCD_InvalidateRect (MAIN_LCD_ID, 0, 0, 239, 283);  //For 8910FF 240x320 screen test
	#else
	LCD_InvalidateRect (MAIN_LCD_ID, 0, 0, 239, 239);  //For 8910FF 240x320 screen test
	#endif
#endif
#else
    LCD_Invalidate(MAIN_LCD_ID);
#endif

    s->display_frame_count ++;

    ISP_SERVICE_TRACE("ISP:DisplayOneFrame end time %d dvmode:%d", SCI_GetTickCount(), g_DcamMode);
}

#else

LOCAL void _ISP_ServiceDisplayOneFrame(uint32 block_id,ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_ADDRESS_T           src_addr_t = {0};
    ISP_ADDRESS_T           *p_addr =SCI_NULL;
    uint32                  disp_addr_y = 0;
    uint32                  disp_addr_u = 0;
    uint32                  rot_angle = ISP_ROTATION_0;
    uint32                  img_width = 0;
    uint32                  img_height = 0;
    ISP_DISP_BLOCK_T        *p_block = SCI_NULL;
    BLOCKCFG_T              blk_cfg = {0};
    uint32                  b_re_cfg = 0;

    if(ISP_DISPLAY_NONE == block_id)
    {
        SCI_TRACE_LOW("_ISP_ServiceDisplayOneFrame none!");
        return ;
    }

    if(PNULL == p_frame)
    {
        ISP_SERVICE_LOG("_ISP_ServiceDisplayOneFrame, input frame pointer is NULL!abnormal!");
        return;
    }

    //SCI_TRACE_LOW("david preview displayoneframe start yaddr:0x%x tick:%u mode:%d",
                    //p_frame->yaddr, SCI_GetTickCount(), g_Capturemode);
    //ISP_SERVICE_LOG:"_ISP_ServiceDisplayOneFrame: frame addr 0x%x 0x%x"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3405_112_2_18_1_44_1_368,(uint8*)"dd",p_frame->yaddr, p_frame->uaddr);


    disp_addr_y = p_frame->yaddr;
    disp_addr_u = p_frame->uaddr;

    // Config YUV addr for Gouda Rotation
    //(hight-1)* stride  clockwize 90 degree

    //disp_addr_y += (200-1)*200*2;
    disp_addr_y += ()

    if(!p_frame->yaddr)
    {
        ISP_SERVICE_LOG("_ISP_ServiceDisplayOneFrame, y address of input frame pointer is NULL!abnormal!");
        return;
    }

    if(s->is_first_frame && ISP_SERVICE_VP_EX == s->service)
    {
        LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id, PERMIT_GUI_INVALIDATE);
        s->is_first_frame = 0;
    }

    LCD_GetLcdCtrlSemaphore(MAIN_LCD_ID);

    LCD_SetBlockBuffer(MAIN_LCD_ID, 0, (uint32 *)disp_addr_y);

    if(s->service == ISP_SERVICE_VP || s->service == ISP_SERVICE_VP_EX || s->is_first_frame)
    {
        b_re_cfg = 1;
    }

    //ISP_SERVICE_LOG:"ISP_SERVICE: block enable %d,p_block->lcd_id %d, p_block->block_id %d, re config %d "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3486_112_2_18_1_44_2_370,(uint8*)"dddd",LCD_GetBlockIsEnable((LCD_ID_E)p_block->lcd_id,p_block->block_id),p_block->lcd_id,p_block->block_id,b_re_cfg);

    if(b_re_cfg)
    {
        blk_cfg.start_x     = 0;
        blk_cfg.start_y     = 0;
        blk_cfg.end_x       = 199;    //239;
        blk_cfg.end_y       = 199;    //319;
        blk_cfg.width       = 200*2;  //320*2;  //stride
        blk_cfg.colorkey_en = 0;
        blk_cfg.alpha_sel   = 1;
        blk_cfg.alpha       = 0xFF;
        blk_cfg.priority    = 0;
        blk_cfg.type        = 1;
        blk_cfg.resolution  = LCD_RESOLUTION_YUV422;
        blk_cfg.rotation    = 1;      // 0: 0; 1: 90; clockwise
        //LCD_SetImageLayerSourceSize(MAIN_LCD_ID, 240, 320);
        LCD_SetImageLayerSourceSize(MAIN_LCD_ID, 200, 200);
        SCI_TRACE_LOW("david displayoneframe config 200*200.");

        LCD_ConfigBlock(MAIN_LCD_ID, 0, blk_cfg);
        LCD_EnableBlock(MAIN_LCD_ID, 0);
    }

    //ISP_SERVICE_TRACE:"_ISP_ServiceDisplayOneFrame: s->is_first_frame = %d, time %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3540_112_2_18_1_44_2_371,(uint8*)"dd",s->is_first_frame,SCI_GetTickCount());
    LCD_Invalidate(MAIN_LCD_ID);
    s->display_frame_count ++;
    ISP_SERVICE_TRACE("david preview _ISP_ServiceDisplayOneFrame end time %d dvmode:%d", SCI_GetTickCount(), g_DcamMode);
}

#endif



LOCAL void _ISP_ServiceDisplayOneFrameLowMem(uint32 block_id,ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_ADDRESS_T           src_addr_t = {0};
    ISP_ADDRESS_T           *p_addr =SCI_NULL;
    uint32                  disp_addr_y =0;
    uint32                  disp_addr_u =0;
    uint32                  rot_angle = ISP_ROTATION_0;
    uint32                  img_width = 0;
    uint32                  img_height = 0;
    ISP_DISP_BLOCK_T        *p_block = SCI_NULL;
    BLOCKCFG_T              blk_cfg = {0};
 //   uint32                  b_re_cfg = 0;

    if(ISP_DISPLAY_NONE == block_id)
        return;

    if(PNULL == p_frame)
    {
        ISP_SERVICE_LOG("ISP:DisplayLowMem frame point is NULL");
        return;
    }
    disp_addr_y = p_frame->yaddr;
    disp_addr_u = p_frame->uaddr;

    ISP_SERVICE_LOG("_ISP_ServiceDisplayOneFrameLowMem: frame addr 0x%x 0x%x", p_frame->yaddr, p_frame->uaddr);
    p_block = &s->display_block;
    p_addr = &s->rotation_buf_addr[0];
    rot_angle = s->display_rotation;

    if(0 == p_addr->yaddr)                  //so here should be YUV, if RGB565 should be special handle
    {
        ISP_SERVICE_LOG("ISP:DisplayLowMem, y addr is NULL");
        s->err_code = ISP_ERR_CODE_NO_MEM;
        return;
    }

    if(!p_frame->yaddr)
    {
        ISP_SERVICE_LOG("ISP:DisplayLowMem, y addr pointer is NULL");
        return;
    }
    else if(ISP_DATA_YUV422 == p_block->img_fmt || ISP_DATA_YUV420 == p_block->img_fmt)
    {
        if(!p_frame->uaddr)
        {
            ISP_SERVICE_LOG("ISP:DisplayLowMem, u addrpointer is NULL");
            return;
        }
    }

    LCD_GetLcdCtrlSemaphore((LCD_ID_E)p_block->lcd_id);

    if(ISP_ROTATION_0 != rot_angle) //need rotation
    {
        rot_angle = _ISP_ServiceRotAngle2DrvAngle(rot_angle);

        src_addr_t.yaddr = p_frame->yaddr;
        src_addr_t.uaddr = p_frame->uaddr;
        src_addr_t.vaddr = p_frame->vaddr;

        if(ROTATION_180 == rot_angle ||
           ROTATION_MIRROR == rot_angle)
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
    else
    {
        //use the rotation buffer 0 as the display buffer
        disp_addr_y = s->rotation_buf_addr[0].yaddr;
        disp_addr_u = s->rotation_buf_addr[0].uaddr;

        if(0 == disp_addr_y)
        {
            ISP_SERVICE_LOG("ISP:DisplayowMem memory err");
            s->err_code = ISP_ERR_CODE_NO_MEM;
        }
        else
        {
            if(ISP_DATA_YUV422 == p_block->img_fmt)
            {
                if(0 == disp_addr_u)
                {
                    ISP_SERVICE_LOG("ISP:DisplayLowMem uv buffer err1");
                    s->err_code = ISP_ERR_CODE_NO_MEM;
                }
                else
                {
                    SCI_MEMCPY((void *)disp_addr_y, (void *)p_frame->yaddr, p_block->img_rect.w * p_block->img_rect.h);
                    SCI_MEMCPY((void *)disp_addr_u, (void *)p_frame->uaddr, p_block->img_rect.w * p_block->img_rect.h);
                }
            }
            else if(ISP_DATA_YUV420 == p_block->img_fmt)
            {
                if(0 == disp_addr_u)
                {
                    ISP_SERVICE_LOG("ISP:DisplayLowMem uv buffer err2");
                    s->err_code = ISP_ERR_CODE_NO_MEM;
                }
                else
                {
                    SCI_MEMCPY((void *)disp_addr_y, (void *)p_frame->yaddr, p_block->img_rect.w * p_block->img_rect.h);
                    SCI_MEMCPY((void *)disp_addr_u, (void *)p_frame->uaddr, (p_block->img_rect.w * p_block->img_rect.h)>>1);
                }
            }
        }
    }

    _ISP_ServicePostDisplayHandle();

    LCD_SetBlockBuffer((LCD_ID_E)p_block->lcd_id, p_block->block_id, (uint32 *)disp_addr_y);

    if(ISP_DATA_YUV422 == p_block->img_fmt ||
       ISP_DATA_YUV420 == p_block->img_fmt)
    {
         LCD_SetUVBuffer((LCD_ID_E)p_block->lcd_id, p_block->block_id, (uint32 *)disp_addr_u);
    }

    if(p_block->is_enable == 0)
    {
        LCD_EnableBlock((LCD_ID_E)p_block->lcd_id,p_block->block_id);
        p_block->is_enable = 1;
    }
    else
    {
        ISP_SERVICE_LOG("ISP:DisplayLowMem: Block %d enabled", block_id);
    }

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

    if(LCD_LAYER_IMAGE == p_block->block_id)
    {
        if(ISP_DATA_YUV422 == p_block->img_fmt)
        {
            blk_cfg.resolution = LCD_RESOLUTION_YUV422; //YUV422
        }
        else
        {
            blk_cfg.resolution = LCD_RESOLUTION_YUV420; //YUV420
        }
    }
    else
    {
        blk_cfg.resolution = LCD_RESOLUTION_RGB565; //RGB565
    }

    if(_ISP_ServiceIsLittleEndian())
    {
        if(LCD_RESOLUTION_RGB565 == blk_cfg.resolution)
        {
            blk_cfg.endian_switch = 2;
        }
        else
        {
            blk_cfg.endian_switch = 1;
        }
    }
    else
    {
        blk_cfg.endian_switch = 0;
    }

    blk_cfg.set_endian_flag = 1;

    LCD_ConfigBlock((LCD_ID_E)p_block->lcd_id,p_block->block_id, blk_cfg);
    LCD_EnableBlock((LCD_ID_E)p_block->lcd_id,p_block->block_id);

    ISP_SERVICE_TRACE("ISP:DisplayLowMem is_first_frame = %d, time %d", s->is_first_frame,SCI_GetTickCount());

    LCD_InvalidateRect((LCD_ID_E)p_block->lcd_id,
                       p_block->lcd_inv_rect.x,
                       p_block->lcd_inv_rect.y,
                       p_block->lcd_inv_rect.x + p_block->lcd_inv_rect.w - 1,
                       p_block->lcd_inv_rect.y + p_block->lcd_inv_rect.h - 1);

    if(s->is_first_frame)
    {
        LCD_SetInvalidateLimit((LCD_ID_E)s->display_block.lcd_id,PERMIT_GUI_INVALIDATE);
        s->is_first_frame = 0;
    }

    ISP_SERVICE_TRACE("ISP:DisplayLowMem end time %d", SCI_GetTickCount());
}

LOCAL void _ISP_ServiceRotFmt(uint32 fmt, ROTATION_PARAM_T * p_rot)
{
    switch(fmt)
    {
        case ISP_IMAGE_FORMAT_RGB565:
            p_rot->data_format = ROTATION_RGB565;
        break;

        case ISP_IMAGE_FORMAT_YUV422:
            p_rot->data_format = ROTATION_YUV422;
        break;

        case ISP_IMAGE_FORMAT_YUV420:
            p_rot->data_format = ROTATION_YUV420;
        break;

        case ISP_IMAGE_FORMAT_RGB888:
            p_rot->data_format = ROTATION_RGB888;
        break;

        case ISP_IMAGE_FORMAT_RGB666:
            p_rot->data_format = ROTATION_RGB666;
        break;

        case ISP_IMAGE_FORMAT_YUV400:
        case ISP_IMAGE_FORMAT_YUV420_3FRAME:
            p_rot->data_format = ROTATION_YUV400;
        break;

        default:
            p_rot->data_format = ROTATION_MAX;
        break;
    }
}

LOCAL void _ISP_ServiceRotAddr(ISP_ADDRESS_T*    p_src,
                               ISP_ADDRESS_T*    p_dst,
                               ROTATION_PARAM_T* p_rot)
{
    p_rot->src_addr.y_addr  = p_src->yaddr;
    p_rot->src_addr.uv_addr = p_src->uaddr;
    p_rot->src_addr.v_addr  = p_src->vaddr;

    p_rot->dst_addr.y_addr  = p_dst->yaddr;
    p_rot->dst_addr.uv_addr = p_dst->uaddr;
    p_rot->dst_addr.v_addr  = p_dst->vaddr;
}

LOCAL void _ISP_ServiceRotYUV4203Frame(uint32            width,
                                       uint32            height,
                                       ROTATION_PARAM_T* pRotPara)
{
    pRotPara->img_size.w        = width >> 1 ;
    pRotPara->img_size.h        = height >> 1;

    pRotPara->src_addr.y_addr   = pRotPara->src_addr.uv_addr;
    pRotPara->dst_addr.y_addr   = pRotPara->dst_addr.uv_addr;

    if(!Rotation_AppStart(pRotPara))
    {
        //SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceRotateImage-U, SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3612_112_2_18_1_44_2_372,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceRotateImage-U, FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3616_112_2_18_1_44_2_373,(uint8*)"");
        return;
    }

    pRotPara->src_addr.y_addr   = pRotPara->src_addr.v_addr;
    pRotPara->dst_addr.y_addr   = pRotPara->dst_addr.v_addr;

    if(!Rotation_AppStart(pRotPara))
    {
        //SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceRotateImage-V, SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3625_112_2_18_1_44_2_374,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceRotateImage-V, FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3629_112_2_18_1_44_2_375,(uint8*)"");
    }

    return;
}

LOCAL void _ISP_ServiceRotateImage(ISP_ADDRESS_T* p_src,
                                   ISP_ADDRESS_T* p_dst,
                                   uint32         width,
                                   uint32         height,
                                   uint32         rot_angle,
                                   uint32         img_format)
{
    ROTATION_PARAM_T rotPara = {0};

    rotPara.rotation_mode = rot_angle;

    _ISP_ServiceRotFmt(img_format, &rotPara);

    _ISP_ServiceRotAddr(p_src, p_dst, &rotPara);

    rotPara.img_size.w = width;
    rotPara.img_size.h = height;

    //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceRotateImage, rot_mode=%d,format=%d,img_size.w,h %d %d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3657_112_2_18_1_44_2_376,(uint8*)"dddd",rotPara.rotation_mode,rotPara.data_format,rotPara.img_size.w,rotPara.img_size.h);
    //ISP_SERVICE_LOG:"src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3664_112_2_18_1_44_2_377,(uint8*)"dddddd",rotPara.src_addr.y_addr,rotPara.src_addr.uv_addr,rotPara.src_addr.v_addr,rotPara.dst_addr.y_addr,rotPara.dst_addr.uv_addr,rotPara.dst_addr.v_addr);

    if(!Rotation_AppStart(&rotPara))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceRotateImage, SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3668_112_2_18_1_44_2_378,(uint8*)"");
    }
    else
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceRotateImage, FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3672_112_2_18_1_44_2_379,(uint8*)"");
    }

    if(ISP_IMAGE_FORMAT_YUV420_3FRAME == img_format)
    {
        _ISP_ServiceRotYUV4203Frame(width, height, &rotPara);
    }

    return;
}
LOCAL void _ISP_ServiceInit(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  mem_size = 0;

    SCI_TRACE_LOW("ISP:ServiceInit");
   // SCI_MEMSET((void*)s, 0, sizeof(ISP_SERVICE_T));

    s->module_addr = ISP_AHB_SLAVE_ADDR;
    s->service = ISP_SERVICE_IDLE;
    s->state = ISP_STATE_STOP;
    s->display_block.block_id = ISP_DISPLAY_NONE;
    s->remote_display_block.block_id = ISP_DISPLAY_NONE;
    s->err_code = 0;
    s->display_user_cb_ctrl = (SCI_MUTEX_PTR)SCI_CreateMutex("disp cb ctrl",SCI_INHERIT);
    SCI_ASSERT(s->display_user_cb_ctrl != SCI_NULL); /*assert verified*/
    s->encode_user_cb_ctrl = (SCI_MUTEX_PTR)SCI_CreateMutex("enc cb ctrl",SCI_INHERIT);
    SCI_ASSERT(s->encode_user_cb_ctrl != SCI_NULL); /*assert verified*/
    s->post_cap_user_cb_ctrl = (SCI_MUTEX_PTR)SCI_CreateMutex("post cap cb ctrl",SCI_INHERIT);
    SCI_ASSERT(s->post_cap_user_cb_ctrl != SCI_NULL); /*assert verified*/
    s->mem_ctrl = (SCI_MUTEX_PTR)SCI_CreateMutex("isp work memory ctrl",SCI_INHERIT);
    SCI_ASSERT(s->mem_ctrl != SCI_NULL); /*assert verified*/
    s->encode_user_cb = SCI_NULL;
    s->display_user_cb = SCI_NULL;
    s->scale_user_cb = SCI_NULL;
    s->p_work_mem = SCI_NULL;
    s->work_mem_len = 0;
    s->mem_alloced_by_myself = SCI_FALSE;
    s->p_frame_enc = SCI_NULL;
    s->b_drv_init_ed = 0;
    s->restart_counter = 0;
    s->enc_stoped = 0;
    s->first_cap_format = ISP_DATA_YUV422;
    s->local_review_format = ISP_DATA_YUV422;
    SCI_SetEvent(s_isp_event, 0, SCI_AND);

    _ISP_ServiceCreateTimer();

    #if 0
    if(SC6530 == CHIP_GetChipType())
    {
        *(volatile uint32*)AHB_SYS_CTL4 |= BIT_8;
    }
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if((SC6531EFM == CHIP_GetChipType()) ||(SC6531EFM_AB == CHIP_GetChipType())) {
        ISP_DriverSetClk(CGM_DCAM_CFG, ISP_CLK_HIG);
    }
#endif
    else {
        ISP_DriverSetClk(APB_CLK_CFG0, ISP_CLK_HIG);
    }
    #endif


    if(SCI_NULL == s->p_frame_enc)
    {
        mem_size = (uint32)(ISP_PATH_FRAME_COUNT_MAX * sizeof(ISP_FRAME_T));
        s->p_frame_enc = (ISP_FRAME_T*)ISP_SERVICE_ALLOC(mem_size);
        SCI_MEMSET((void*)s->p_frame_enc, 0, mem_size);
    }
    s->p_scale_coeff_mem = (uint32 *)SCI_ALLOCA(ISP_SERVICE_SC_COEFF_SIZE);
    SCI_ASSERT(SCI_NULL != s->p_scale_coeff_mem); /*assert verified*/


    return ;
}

LOCAL void _ISP_ServiceDeInit(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    s->is_slice_alone = 0;
    s->display_user_cb = NULL;
    if(s->mem_ctrl)
    {
        SCI_GetMutex(s->mem_ctrl, SCI_WAIT_FOREVER);
    }

    if(s->mem_alloced_by_myself)
    {
        SCI_Free(s->p_work_mem);
        s->mem_alloced_by_myself = SCI_FALSE;
    }
    s->p_work_mem = SCI_NULL;
    s->work_mem_len = 0;
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

    if(s->post_cap_user_cb_ctrl != SCI_NULL)
    {
        SCI_DeleteMutex(s->post_cap_user_cb_ctrl);
        s->post_cap_user_cb_ctrl = SCI_NULL;
    }
    LCD_SetBrushMode((LCD_ID_E)s->display_block.lcd_id, LCD_BRUSH_MODE_SYNC);

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

    if(SCI_NULL != s->p_frame_enc)
    {
        SCI_Free(s->p_frame_enc);
        s->p_frame_enc = SCI_NULL;
    }

    if(s->p_scale_coeff_mem)
    {
        SCI_Free(s->p_scale_coeff_mem);
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

LOCAL void _ISP_ServiceGetPostCapCtrl(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->post_cap_user_cb_ctrl)
        SCI_GetMutex(s->post_cap_user_cb_ctrl, SCI_WAIT_FOREVER);
}

LOCAL void _ISP_ServiceRelPostCapCtrl(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(s->post_cap_user_cb_ctrl)
        SCI_PutMutex(s->post_cap_user_cb_ctrl);
}

LOCAL int32 _ISP_ServiceCallUserDispCB(void *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn = 0;

    _ISP_ServiceGetDispCtrl();

    //SCI_TRACE_LOW("david display_user_cb = 0x%x", s->display_user_cb);
    if(s->display_user_cb)
    {
        rtn = (*s->display_user_cb)(p_frame);
    }
    _ISP_ServiceRelDispCtrl();

    return rtn;
}

LOCAL int32 _ISP_ServiceCallUserEncCB(void *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn = 0;

    _ISP_ServiceGetEncCtrl();
    if(s->encode_user_cb)
    {
        rtn = (*s->encode_user_cb)(p_frame);
    }
    _ISP_ServiceRelEncCtrl();

    return rtn;
}

LOCAL int32 _ISP_ServiceCallUserPostCapCB(uint32 param)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn = 0;
    uint32                  val = param;

    _ISP_ServiceGetPostCapCtrl();
    if(s->post_cap_user_cb)
    {
        rtn = (*s->post_cap_user_cb)((void *)&val);
    }
    _ISP_ServiceRelPostCapCtrl();

    return rtn;
}

LOCAL void _ISP_ServiceSetCapParam(ISP_CAP_PARAM_T *p_cap_param)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    s->cap_input_image_format = (uint8)p_cap_param->cap_img_format;
    s->cap_input_image_patten = (uint8)p_cap_param->cap_img_pattern;
    s->vsync_polarity         = (uint8)p_cap_param->cap_vsync_pol;
    s->hsync_polarity         = (uint8)p_cap_param->cap_hsync_pol;
    s->pclk_polarity          = (uint8)p_cap_param->cap_pclk_pol;
    s->preview_deci_frame_num = (uint8)p_cap_param->cap_frame_decimated;
    s->cap_sensor_if_mode     = (uint8)p_cap_param->cap_sensor_if_mode;
    s->cap_sensor_if_endian   = (uint8)p_cap_param->cap_sensor_if_endian;
    s->b_is_atv               = (uint8)p_cap_param->b_is_atv;
    s->ccir_pclk_src          = (uint8)p_cap_param->ccir_pclk_src;
    s->ccir_d0_src            = (uint8)p_cap_param->ccir_d0_src;
    s->ccir_vsync_src         = (uint8)p_cap_param->ccir_vsync_src;
    s->ccir_d1_src         = (uint8)p_cap_param->ccir_d1_src;
    s->thre_start             = (uint16)p_cap_param->cap_atv_thre_start;
    s->thre_end               = (uint16)p_cap_param->cap_atv_thre_end;
    s->fix_mode_eb            = (uint8)p_cap_param->cap_atv_thre_eb;

    if(ISP_SERVICE_JPEG == s->service)
    {
        s->capture_skip_frame_num = (uint8)p_cap_param->cap_frame_skipped;
    }
    else
    {
        s->preview_skip_frame_num = (uint8)p_cap_param->cap_frame_skipped;
    }

}

LOCAL void _ISP_ServiceGetCapParam(ISP_CAP_PARAM_T *p_cap_param)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    p_cap_param->cap_img_format        = (uint32)s->cap_input_image_format;
    p_cap_param->cap_img_pattern       = (uint32)s->cap_input_image_patten;
    p_cap_param->cap_vsync_pol         = (uint32)s->vsync_polarity;
    p_cap_param->cap_hsync_pol         = (uint32)s->hsync_polarity;
    p_cap_param->cap_pclk_pol          = (uint32)s->pclk_polarity;
    p_cap_param->cap_frame_decimated   = (uint32)s->preview_deci_frame_num;
    p_cap_param->cap_sensor_if_mode    = (uint32)s->cap_sensor_if_mode;
    p_cap_param->cap_sensor_if_endian  = (uint32)s->cap_sensor_if_endian;
    p_cap_param->b_is_atv              = (uint8)s->b_is_atv;
    p_cap_param->ccir_pclk_src         = (uint8)s->ccir_pclk_src;
    p_cap_param->ccir_d0_src           = (uint8)s->ccir_d0_src;
    p_cap_param->ccir_vsync_src        = (uint8)s->ccir_vsync_src;
    p_cap_param->ccir_d1_src           = (uint8)s->ccir_d1_src;
    p_cap_param->cap_atv_thre_start    = (uint16)s->thre_start;
    p_cap_param->cap_atv_thre_end      = (uint16)s->thre_end ;
    p_cap_param->cap_atv_thre_eb       = (uint8)s->fix_mode_eb;

    if(ISP_SERVICE_JPEG == s->service)
    {
        p_cap_param->cap_frame_skipped = (uint32)s->capture_skip_frame_num;
    }
    else
    {
        p_cap_param->cap_frame_skipped = (uint32)s->preview_skip_frame_num;
    }

}

LOCAL void _ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr = SCI_NULL;
    ISP_SIZE_T              dst_img_size = {0};

    s->service = ISP_SERVICE_PREVIEW;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetPreviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3903_112_2_18_1_44_2_380,(uint8*)"");

    _ISP_ServiceSetCapParam(&param_ptr->cap_param);
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
                                                          dst_img_size.w,
                                                          dst_img_size.h);

    s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;

    s->input_size.w = s->cap_output_size.w;
    s->input_size.h = s->cap_output_size.h;

    s->input_range.x = 0;
    s->input_range.y = 0;
    s->input_range.w = s->cap_output_size.w;
    s->input_range.h = s->cap_output_size.h;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetPreviewParam,input for scaling {%d,%d,%d,%d}, output from CAP {%d,%d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3954_112_2_18_1_44_2_381,(uint8*)"dddddd",s->input_range.x,s->input_range.y,s->input_range.w,s->input_range.h,s->cap_output_size.w,s->cap_output_size.h);

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_3986_112_2_18_1_44_2_382,(uint8*)"");

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

    _ISP_ServiceSetCapParam(&param_ptr->cap_param);

    s->cap_output_size.w = s->cap_input_range.w;
    s->cap_output_size.h = s->cap_input_range.h;

    s->path_output_image_format = param_ptr->yuv_type;
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
    ISP_DISP_BLOCK_T        *disp_block_ptr = &s->display_block;
    ISP_SIZE_T              cap_out_max = {0};
    ISP_SIZE_T              dst_img_size = {0};
    uint32                  max_times = 0, times = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetMpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4051_112_2_18_1_44_3_383,(uint8*)"");

    s->service = ISP_SERVICE_MPEG;
    _ISP_ServiceSetCapParam(&param_ptr->cap_param);

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

    cap_out_max.w = ISP_MAX(dst_img_size.w, s->encoder_size.w);
    if(cap_out_max.w > ISP_SCALE_FRAME_MODE_WIDTH_TH)
    {
        cap_out_max.w = ISP_SCALE_FRAME_MODE_WIDTH_TH;
        cap_out_max.h = (uint32)((s->cap_output_size.h * cap_out_max.w)/s->cap_output_size.w);
        cap_out_max.h = ISP_PIXEL_LEFT_ALIGNED_(cap_out_max.h, ISP_ALIGNED_PIXELS);
    }
    else
    {
        cap_out_max.w = cap_out_max.w / ISP_PATH_SC_COEFF_MAX;
        cap_out_max.w = ISP_PIXEL_LEFT_ALIGNED_(cap_out_max.w, ISP_ALIGNED_PIXELS);
        cap_out_max.h = ISP_MAX(dst_img_size.h, s->encoder_size.h);
        cap_out_max.h = cap_out_max.h / ISP_PATH_SC_COEFF_MAX;
        cap_out_max.h = ISP_PIXEL_LEFT_ALIGNED_(cap_out_max.h, ISP_ALIGNED_PIXELS);
    }

    s->cap_output_size.w = s->cap_input_range.w;
    s->cap_output_size.h = s->cap_input_range.h;

    max_times = s->cap_output_size.w / cap_out_max.w;
    times = s->cap_output_size.h / cap_out_max.h;
    max_times = ISP_MIN(max_times, times);
    max_times = max_times < 1 ? 1 : max_times;

    s->first_cap_format = ISP_DATA_YUV422;
    s->local_review_format = ISP_DATA_YUV422;

    ISP_SERVICE_TRACE("ISP:SetMpegParam cap_out_max %d %d times %d",cap_out_max.w,cap_out_max.h,max_times);

    for(times = 1; times < max_times; times ++)
    {
        s->cap_output_size.w = s->cap_input_range.w;
        s->cap_output_size.h = s->cap_input_range.h;

        cap_out_max.w = s->cap_output_size.w / times;
        cap_out_max.w = ISP_PIXEL_LEFT_ALIGNED_(cap_out_max.w, ISP_ALIGNED_PIXELS);
        if(cap_out_max.w > ISP_SCALE_FRAME_MODE_WIDTH_TH)
        {
            cap_out_max.w = ISP_SCALE_FRAME_MODE_WIDTH_TH;
            cap_out_max.h = (uint32)((s->cap_output_size.h * cap_out_max.w)/s->cap_output_size.w);
            cap_out_max.h = ISP_PIXEL_LEFT_ALIGNED_(cap_out_max.h, ISP_ALIGNED_PIXELS);
        }
        else
        {
            cap_out_max.h = s->cap_output_size.h / times;
            cap_out_max.h = ISP_PIXEL_LEFT_ALIGNED_(cap_out_max.h, ISP_ALIGNED_PIXELS);
        }

        s->cap_img_dec.x_factor = _ISP_ServiceGetXYDeciFactor(&s->cap_output_size.w,
                                                            &s->cap_output_size.h,
                                                            cap_out_max.w,
                                                            cap_out_max.h);

        ISP_SERVICE_TRACE("ISP:SetMpegParam deci %d s->cap_output_size %d %d",
                          s->cap_img_dec.x_factor,
                          s->cap_output_size.w,
                          s->cap_output_size.h);

        s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;

        s->cap_output_size.w = cap_out_max.w;
        s->cap_output_size.h = cap_out_max.h;
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

        s->input_range.x = (s->cap_output_size.w - s->input_range.w) >> 1;
        s->input_range.x = ISP_PIXEL_LEFT_ALIGNED_(s->input_range.x, ISP_ALIGNED_PIXELS);
        s->input_range.y = (s->cap_output_size.h - s->input_range.h)  >> 1; //input_range is for display trim window
        s->input_range.y = ISP_PIXEL_LEFT_ALIGNED_(s->input_range.y, ISP_ALIGNED_PIXELS);

        if(s->cap_output_size.w * s->encoder_size.h > s->cap_output_size.h * s->encoder_size.w)
        {
            s->remote_input_range.w = s->cap_output_size.h * s->encoder_size.w / s->encoder_size.h;
            s->remote_input_range.w = ISP_PIXEL_LEFT_ALIGNED_(s->remote_input_range.w,ISP_ALIGNED_PIXELS);
            s->remote_input_range.h = s->cap_output_size.h;
        }
        else
        {
            s->remote_input_range.h = s->cap_output_size.w * s->encoder_size.h / s->encoder_size.w;
            //cov 27062
            //s->remote_input_range.w = ISP_PIXEL_LEFT_ALIGNED_(s->remote_input_range.h,ISP_ALIGNED_PIXELS);
            s->remote_input_range.w = s->cap_output_size.w;
        }

        s->remote_input_range.x = (s->cap_output_size.w - s->remote_input_range.w) >> 1;
        s->remote_input_range.x = ISP_PIXEL_LEFT_ALIGNED_(s->remote_input_range.x, ISP_ALIGNED_PIXELS);
        s->remote_input_range.y = (s->cap_output_size.h - s->remote_input_range.h) >> 1;//remote_input_range is for encoder trim window
        s->remote_input_range.y = ISP_PIXEL_LEFT_ALIGNED_(s->remote_input_range.y, ISP_ALIGNED_PIXELS);

        s->path_output_image_format = (uint8)param_ptr->data_format;
        s->display_rotation = param_ptr->disp_rotation;
        s->enc_rotation = param_ptr->encode_rotation;

        SCI_MEMCPY((void*)&s->display_range,
                   (void*)&param_ptr->disp_range,
                   sizeof(ISP_RECT_T));


        if(ISP_DISPLAY_NONE != param_ptr->disp_lcd_id)
        {
            disp_block_ptr->lcd_id = param_ptr->disp_lcd_id;
            disp_block_ptr->block_id = param_ptr->disp_block_id;

            _ISP_ServiceConfigBlock(disp_block_ptr,
                                    &s->display_range,
                                    &param_ptr->lcd_rect,
                                    s->display_rotation);
            s->local_review_enable = 1;
        }
        else
        {
            disp_block_ptr->img_rect.w = param_ptr->disp_range.w;
            disp_block_ptr->img_rect.h = param_ptr->disp_range.h;
            s->local_review_enable = 0;
        }

        s->display_frame_count = 0;


        if(SCI_TRUE == _ISP_ServiceAllocFrame())
        {
            if((0 != s->disp_frm_round_num && 0 != s->enc_frm_round_num && ISP_DISPLAY_NONE != param_ptr->disp_lcd_id)||
               (0 != s->enc_frm_round_num && ISP_DISPLAY_NONE == param_ptr->disp_lcd_id))
            {
                s->err_code = ISP_ERR_CODE_NO_ERR;
                //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetMpegParam, few memory, YUV format %d, enc buf %d, disp buf %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4254_112_2_18_1_44_3_384,(uint8*)"ddd",s->first_cap_format,s->enc_frm_round_num,s->disp_frm_round_num);
                s->local_review_format = disp_block_ptr->img_fmt;
                break;
            }
            else
            {
                s->err_code = ISP_ERR_CODE_NO_MEM;
                s->first_cap_format = ISP_DATA_YUV420;
            }
        }
        else
        {
            s->err_code = ISP_ERR_CODE_NO_MEM;
            s->first_cap_format = ISP_DATA_YUV420;
        }

    }

    if(ISP_ERR_CODE_NO_MEM == s->err_code &&
       s->disp_frm_round_num &&
       s->enc_frm_round_num)
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
        s->local_review_format = disp_block_ptr->img_fmt;
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
            p_disp_range->x = ((p_disp_range->x >> 2) << 2);
            p_disp_range->w = tmp_size;

        }
        if(p_disp_range->h > (p_input_range->w * ISP_PATH_SC_COEFF_MAX))
        {
            tmp_size = (uint32)(p_input_range->w * ISP_PATH_SC_COEFF_MAX);
            p_disp_range->y = p_disp_range->y + ((p_disp_range->h - tmp_size) >> 1);
            p_disp_range->y = ((p_disp_range->y >> 1) << 1);
            p_disp_range->h = tmp_size;
        }
    }
    else
    {
        if(p_disp_range->w > (p_input_range->w * ISP_PATH_SC_COEFF_MAX))
        {
            tmp_size = (uint32)(p_input_range->w * ISP_PATH_SC_COEFF_MAX);
            p_disp_range->x = p_disp_range->x + ((p_disp_range->w - tmp_size) >> 1);
            p_disp_range->x = ((p_disp_range->x >> 2) << 2);
            p_disp_range->w = tmp_size;
        }
        if(p_disp_range->h > (p_input_range->h * ISP_PATH_SC_COEFF_MAX))
        {
            tmp_size = (uint32)(p_input_range->h * ISP_PATH_SC_COEFF_MAX);
            p_disp_range->y = p_disp_range->y + ((p_disp_range->h - tmp_size) >> 1);
            p_disp_range->y = ((p_disp_range->y >> 1) << 1);
            p_disp_range->h = tmp_size;
        }

    }

}
LOCAL void _ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr;
    ISP_RECT_T              disp_range = {0};

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetReviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4309_112_2_18_1_44_3_385,(uint8*)"");

    s->service = ISP_SERVICE_REVIEW;

    s->path_input_image_format = param_ptr->input_format;

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

    s->display_frame_count = 0;

    SCI_MEMCPY((void*)&disp_range, (void*)&s->display_range, sizeof(ISP_RECT_T));
    _ISP_ServiceConfigBlock(disp_block_ptr,
                            &s->display_range,
                            &param_ptr->lcd_rect,
                            s->display_rotation);

    if(ISP_DATA_YUV422 == disp_block_ptr->img_fmt) //if not from dc,re-config block,or the same configuration as preview
    {
        s->path_output_image_format = ISP_IMAGE_FORMAT_YUV422;
    }
    else
    {
        s->path_output_image_format = ISP_IMAGE_FORMAT_RGB565;
    }

    //ISP_SERVICE_TRACE:"disp_block_ptr->width = %d,disp_block_ptr->height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4369_112_2_18_1_44_3_386,(uint8*)"dd",disp_block_ptr->img_rect.w,disp_block_ptr->img_rect.h);

    if(SCI_TRUE != _ISP_ServiceAllocFrame())
    {
        s->err_code = ISP_ERR_CODE_NO_MEM;

        if(ISP_IMAGE_FORMAT_YUV422 == s->path_output_image_format &&
           ISP_DISPLAY_NONE != param_ptr->disp_block_id)
        {
            s->path_output_image_format = ISP_IMAGE_FORMAT_YUV420;
            if(SCI_TRUE == _ISP_ServiceAllocFrame())
            {
                SCI_MEMCPY((void*)&s->display_range,(void*)&disp_range, sizeof(ISP_RECT_T));

                _ISP_ServiceConfigBlock(disp_block_ptr,
                                        &s->display_range,
                                        &param_ptr->lcd_rect,
                                        s->display_rotation);
                s->err_code = ISP_ERR_CODE_NO_ERR;

            }
            else
            {
                s->err_code = ISP_ERR_CODE_NO_MEM;
            }

        }
    }
    else
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }

    return ;
}

LOCAL void _ISP_ServiceSetScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_SIZE_T              input_size = {0};
    ISP_SIZE_T              output_size = {0};

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4411_112_2_18_1_44_3_387,(uint8*)"");

    s->service = ISP_SERVICE_SCALE;

    s->path_input_image_format = (uint8)param_ptr->input_format;
    s->path_output_image_format = (uint8)param_ptr->output_format;

    if(param_ptr->is_first == SCI_TRUE)
    {
        s->state = ISP_STATE_CLOSED;
        if(param_ptr->scale_mode == ISP_SERVICE_SCALE_SLICE)
        {
            s->is_slice_alone = SCI_TRUE;
            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_SetScaleParam, slice heght %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4424_112_2_18_1_44_3_388,(uint8*)"d",param_ptr->slice_line);

        }
    }

    if(s->path_input_image_format == ISP_IMAGE_FORMAT_RGB565_EX)
    {
        s->path_input_image_format = ISP_IMAGE_FORMAT_RGB565;
    }
    else if(s->path_input_image_format == ISP_IMAGE_FORMAT_RGB888_EX)
    {
        s->path_input_image_format = ISP_IMAGE_FORMAT_RGB888;
    }

    if(s->path_output_image_format == ISP_IMAGE_FORMAT_RGB888_EX)
    {
        s->path_output_image_format = ISP_IMAGE_FORMAT_RGB888;
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

        s->start_scale_callback = param_ptr->start_scale_callback;
        s->start_scale_param = param_ptr->start_scale_param;

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
        s->swap_buff.uaddr = ISP_ADDRESS_ALIGNED_(s->swap_buff.uaddr, ISP_ALIGNED_BYTES);
    }
    else
    {
        s->swap_buff.yaddr = 0;
        s->swap_buff.uaddr = 0;
    }

    if(SCI_TRUE != _ISP_ServiceAllocFrame())
    {
        s->err_code = ISP_ERR_CODE_NO_MEM;
    }

    return ;
}


LOCAL void _ISP_ServiceSetDenoiseParam(ISP_DENOISE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetDenoiseParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4548_112_2_18_1_44_4_389,(uint8*)"");

    s->service = ISP_SERVICE_DENOISE;

    s->path_input_image_format = (uint8)param_ptr->input_format;
    s->path_output_image_format = (uint8)param_ptr->output_format;
    s->denoise_cfg.threshold_y = (uint8)param_ptr->y_thres_val;
    s->denoise_cfg.threshold_uv = (uint8)param_ptr->uv_thres_val;
    s->denoise_cfg.denoise_eb = SCI_TRUE;

    SCI_MEMCPY((void*)&s->input_size,
               (void*)&param_ptr->input_size,
               sizeof(ISP_SIZE_T));

    SCI_MEMCPY((void*)&s->input_range,
               (void*)&param_ptr->input_rect,
               sizeof(ISP_RECT_T));

    SCI_MEMCPY((void*)&s->input_addr,
               (void*)&(param_ptr->input_addr),
               sizeof(ISP_ADDRESS_T));

    SCI_MEMCPY((void*)&s->encoder_size,
               (void*)&param_ptr->output_size,
               sizeof(ISP_SIZE_T));

    SCI_MEMCPY((void*)&s->output_addr,
                   (void*)&(param_ptr->output_addr),
                    sizeof(ISP_ADDRESS_T));

    if(SCI_TRUE != _ISP_ServiceAllocFrame())
    {
        s->err_code = ISP_ERR_CODE_NO_MEM;
    }
    else
    {
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }

    return ;
}


LOCAL void _ISP_ServiceSetVTScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceSetVTScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4595_112_2_18_1_44_4_390,(uint8*)"");

    s->path_input_image_format = (uint8)param_ptr->input_format;
    s->path_output_image_format = (uint8)param_ptr->output_format;

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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4621_112_2_18_1_44_4_391,(uint8*)"");
    }

    s->scale_user_cb = param_ptr->get_scale_data;
    return ;

}

LOCAL void _ISP_ServiceEncodeFrameInit(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_enc_frame = SCI_NULL;
    uint32                  i = 0,mem_size = 0;

    if(SCI_NULL == s->p_frame_enc)
    {
        mem_size = (uint32)(ISP_PATH_FRAME_COUNT_MAX * sizeof(ISP_FRAME_T));
        p_enc_frame = (ISP_FRAME_T*)ISP_SERVICE_ALLOC(mem_size);
        SCI_MEMSET((void*)p_enc_frame, 0, mem_size);
        s->p_frame_enc = p_enc_frame;
    }
    else
    {
        p_enc_frame = s->p_frame_enc;
    }

    s->p_frame_enc_cur = s->p_frame_enc;

    for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i++)
    {
        (p_enc_frame+i)->yaddr = s->encoder_addr[i].yaddr;
        (p_enc_frame+i)->uaddr = s->encoder_addr[i].uaddr;
        (p_enc_frame+i)->width = s->encoder_size.w;
        (p_enc_frame+i)->height= s->encoder_size.h;
        (p_enc_frame+i)->next  = p_enc_frame + (i + 1) % ISP_PATH_FRAME_COUNT_MAX;
        (p_enc_frame+i)->prev  = p_enc_frame + (i - 1 + ISP_PATH_FRAME_COUNT_MAX) % ISP_PATH_FRAME_COUNT_MAX;
        //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceEncodeFrameInit, frame 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4648_112_2_18_1_44_4_392,(uint8*)"d",(p_enc_frame+i));

    }

    if(s->enc_frm_round_num < ISP_PATH_FRAME_COUNT_MAX)
    {
        (p_enc_frame + (s->enc_frm_round_num - 1))->next = p_enc_frame;
        p_enc_frame->prev = p_enc_frame + (s->enc_frm_round_num - 1);
    }

}

PUBLIC uint32 ISP_ServiceGetVtMemBlockState(uint32 block_id)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    return (uint32)s->vt_mem_block_state[block_id];
}

PUBLIC void ISP_ServiceSetVtMemBlockState(uint32 block_id, uint32 state)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    s->vt_mem_block_state[block_id] = (ISP_VT_MEM_BLOCK_STATE)state;
    return;
}

PUBLIC  ISP_ADDRESS_T * ISP_ServiceGetVtMemBlock(uint32 block_id)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    return &s->display_addr[block_id];

}

LOCAL uint32 _ISP_ServiceGetVtMemBlockId(uint32 * block_id_ptr, uint32 state)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  rtn = SCI_ERROR;
    uint32                  i = 0;

    for(i = 0; i < ISP_VT_ROUND_FRAME_NUM; i++)
    {
        if((ISP_VT_MEM_BLOCK_STATE)state == s->vt_mem_block_state[i])
        {
            *block_id_ptr = i;
            rtn = SCI_SUCCESS;
            break;
        }
    }

    return rtn;
}

PUBLIC uint32 ISP_ServiceGetVtMemBlockId(uint32 * block_id_ptr, uint32 state)
{
//  ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  rtn = SCI_ERROR;

    rtn = _ISP_ServiceGetVtMemBlockId(block_id_ptr, state);

    return rtn;
}

LOCAL void _ISP_ServiceEncodeFrameInitLowMem(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
//    ISP_FRAME_T             *p_enc_frame = SCI_NULL;
    uint32                        i = 0;

    //initial the block state of several block
    for(i = 0; i < ISP_VT_ROUND_FRAME_NUM; i++)
    {
        s->vt_mem_block_state[i] = ISP_VT_MEMBLK_IDLE;
    }

}

LOCAL void _ISP_ServiceEncodeFrameDeInit(void)
{
    return ;
}

LOCAL void _ISP_ServiceSetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_DISP_BLOCK_T        *disp_block_ptr = SCI_NULL;
    ISP_SIZE_T              cap_out_max = {0};
    ISP_SIZE_T              dst_img_size = {0};

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_SetVTEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4682_112_2_18_1_44_4_393,(uint8*)"");

    s->service = ISP_SERVICE_VP;

    _ISP_ServiceSetCapParam(&param_ptr->cap_param);

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

    if(param_ptr->disp_rotation == ISP_ROTATION_90 ||
       param_ptr->disp_rotation == ISP_ROTATION_270)
    {
        dst_img_size.w = param_ptr->local_disp_range.h;
        dst_img_size.h = param_ptr->local_disp_range.w;
    }
    else
    {
        dst_img_size.w = param_ptr->local_disp_range.w;
        dst_img_size.h = param_ptr->local_disp_range.h;
    }

    cap_out_max.w = ISP_MAX(dst_img_size.w, s->encoder_size.w);
    cap_out_max.h = ISP_MAX(dst_img_size.h, s->encoder_size.h);

    s->cap_output_size.w = s->cap_input_range.w;
    s->cap_output_size.h = s->cap_input_range.h;

    if((uint32)(s->cap_output_size.w * cap_out_max.h) > (uint32)(s->cap_output_size.h * cap_out_max.w))
    {
        cap_out_max.w = (uint32)((s->cap_output_size.w * cap_out_max.h)/s->cap_output_size.h);
        cap_out_max.w = ISP_PIXEL_ALIGNED_(cap_out_max.w, ISP_ALIGNED_PIXELS);
    }
    else
    {
        cap_out_max.h = (uint32)((s->cap_output_size.h * cap_out_max.w)/s->cap_output_size.w);
        cap_out_max.h = ISP_PIXEL_ALIGNED_(cap_out_max.h, ISP_ALIGNED_PIXELS);

    }
    s->cap_img_dec.x_factor = _ISP_ServiceGetXYDeciFactor(&s->cap_output_size.w,
                                                          &s->cap_output_size.h,
                                                          cap_out_max.w,
                                                          cap_out_max.h);


    s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;
    s->cap_output_size.w = cap_out_max.w;
    s->cap_output_size.h = cap_out_max.h;
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

    s->input_range.x = (s->cap_output_size.w - s->input_range.w) >> 1;
    s->input_range.y = (s->cap_output_size.h - s->input_range.h) >> 1;

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4792_112_2_18_1_44_4_394,(uint8*)"dd",s->local_display_enable,s->local_review_enable);

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4813_112_2_18_1_44_4_395,(uint8*)"");

    if(param_ptr->mem_mode)
    {
        s->service = ISP_SERVICE_VP_EX_LOW_MEM;
    }
    else
    {
        s->service = ISP_SERVICE_VP_EX;
    }

    _ISP_ServiceSetCapParam(&param_ptr->cap_param);

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

    if(0 == s->encoder_size.w ||
       0 == s->encoder_size.h)
    {
        if(param_ptr->disp_rotation == ISP_ROTATION_90 ||
           param_ptr->disp_rotation == ISP_ROTATION_270)
        {
            s->encoder_size.w = param_ptr->local_disp_range.h;
            s->encoder_size.h = param_ptr->local_disp_range.w;
        }
        else
        {
            s->encoder_size.w = param_ptr->local_disp_range.w;
            s->encoder_size.h = param_ptr->local_disp_range.h;
        }

        s->local_need_encode_frm = 0;
    }
    else
    {
        s->local_need_encode_frm = 1;
    }
    if(param_ptr->local_review_enable || param_ptr->local_display_enable)
    {
        SCI_MEMCPY((void*)&s->display_range,
                   (void*)&param_ptr->local_disp_range,
                   sizeof(ISP_RECT_T));

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
        s->cap_output_size.w = s->cap_input_range.w;
        s->cap_output_size.h = s->cap_input_range.h;

        s->cap_img_dec.x_factor = _ISP_ServiceGetXYDeciFactor(&s->cap_output_size.w,
                                                              &s->cap_output_size.h,
                                                              s->encoder_size.w,
                                                              s->encoder_size.h);

        s->cap_img_dec.y_factor = s->cap_img_dec.x_factor;
        s->cap_output_size.w = s->encoder_size.w;
        s->cap_output_size.h = s->encoder_size.h;
        s->input_size.w = s->cap_output_size.w;
        s->input_size.h = s->cap_output_size.h;

        if(s->cap_output_size.w * s->display_range.h >
           s->cap_output_size.h * s->display_range.w)
        {
            s->input_range.w = s->cap_output_size.h * s->display_range.w / s->display_range.h;
            s->input_range.w = ISP_PIXEL_ALIGNED_(s->input_range.w,ISP_ALIGNED_PIXELS);
            s->input_range.h = s->cap_output_size.h;
        }
        else
        {
            s->input_range.h = s->cap_output_size.w * s->display_range.h / s->display_range.w;
            s->input_range.h = ISP_PIXEL_ALIGNED_(s->input_range.h,ISP_ALIGNED_PIXELS);
            s->input_range.w = s->cap_output_size.w;
        }

        s->input_range.x = (s->cap_output_size.w - s->input_range.w) >> 1;
        s->input_range.y = (s->cap_output_size.h - s->input_range.h) >> 1;
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4930_112_2_18_1_44_4_396,(uint8*)"ddd",s->local_display_enable,s->local_review_enable,s->preview_skip_frame_num);

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4950_112_2_18_1_44_4_397,(uint8*)"");

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
    s->remote_input_fmt        = param_ptr->remote_in_format;

    if(s->remote_display_enable)
    {
        _ISP_ServiceConfigBlock(disp_block_ptr,
                                &s->remote_display_range,
                                &param_ptr->lcd_rect,
                                s->remote_display_rotation);
    }

    //ISP_SERVICE_TRACE:"_ISP_ServiceSetVTDecodeParam,display_enable = %d,review_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_4986_112_2_18_1_44_4_398,(uint8*)"dd",s->remote_display_enable,s->remote_review_enable);

    path1_frame_size = s->remote_display_range.w * s->remote_display_range.h;
    path1_frame_size += ISP_MEM_STRIDER_FOR_ONE_FRAME;

    for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i ++)
    {
        s->remote_display_addr[i].yaddr = s->vt_mem_end_cur + i * path1_frame_size * 2;
        s->remote_display_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->remote_display_addr[i].yaddr,
                                                                ISP_ALIGNED_BYTES);
        s->remote_display_addr[i].uaddr = s->remote_display_addr[i].yaddr + path1_frame_size;
        s->remote_display_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->remote_display_addr[i].uaddr,
                                                                ISP_ALIGNED_BYTES);
        s->remote_display_addr[i].vaddr = 0;

    }

    for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i ++)
    {
        s->remote_rotation_display_addr[i].yaddr = s->vt_mem_end_cur + ISP_PATH_FRAME_COUNT_MAX * path1_frame_size * 2;
        s->remote_rotation_display_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->remote_rotation_display_addr[i].yaddr,
                                                                         ISP_ALIGNED_BYTES);

        s->remote_rotation_display_addr[i].uaddr = s->remote_rotation_display_addr[i].yaddr + path1_frame_size;
        s->remote_rotation_display_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->remote_rotation_display_addr[i].uaddr,
                                                                         ISP_ALIGNED_BYTES);
        s->remote_rotation_display_addr[i].vaddr = 0;
    }

    s->vt_mem_end_cur += path1_frame_size *( ISP_PATH_FRAME_COUNT_MAX + 1) * 2;// disp buff + rotation buff

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5033_112_2_18_1_44_4_399,(uint8*)"");

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

    s->path_input_image_format = (uint8)param_ptr->input_format;
    s->path_output_image_format = (uint8)param_ptr->output_format;

    s->scale_user_cb = param_ptr->get_scale_data;
    return ;
}

LOCAL uint32 _ISP_ServiceCheckPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    LCD_INFO_T lcd_info={0x00};

     if(SCI_NULL == param_ptr)
    {
        return SCI_ERROR;
    }
    LCD_GetInfo(param_ptr->disp_lcd_id, &lcd_info);

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckPreviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5066_112_2_18_1_44_5_400,(uint8*)"");

    if(((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->disp_range.w % ISP_ALIGNED_PIXELS)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels aligned"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5077_112_2_18_1_44_5_401,(uint8*)"");
        return SCI_ERROR;
    }

    if( param_ptr->disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5083_112_2_18_1_44_5_402,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->disp_block_id >=BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5089_112_2_18_1_44_5_403,(uint8*)"");
        return SCI_ERROR;
    }


    if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5097_112_2_18_1_44_5_404,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5103_112_2_18_1_44_5_405,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->disp_range.w > lcd_info.lcd_width||
       param_ptr->disp_range.h > lcd_info.lcd_height)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Display size is larger than max lcd size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5110_112_2_18_1_44_5_406,(uint8*)"");
        return SCI_ERROR;
    }

    return SCI_SUCCESS;

}

LOCAL uint32 _ISP_ServiceCheckJpegParam(ISP_JPEG_PARAM_T * param_ptr)
{

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckJpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5121_112_2_18_1_44_5_407,(uint8*)"");

    if(SCI_NULL == param_ptr)
    {
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->output_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5132_112_2_18_1_44_5_408,(uint8*)"");
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size, {%d %d} {%d %d %d %d}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5145_112_2_18_1_44_5_409,(uint8*)"dddddd",param_ptr->input_size.w,param_ptr->input_size.h,param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h);
        return SCI_ERROR;
    }

    return SCI_SUCCESS;
}


LOCAL uint32 _ISP_ServiceCheckMpegParam(ISP_MPEG_PARAM_T * param_ptr)
{
    LCD_INFO_T lcd_info={0x00};


    if(SCI_NULL == param_ptr)
    {
        return SCI_ERROR;
    }
    if(param_ptr->disp_lcd_id == MAIN_LCD_ID || param_ptr->disp_lcd_id ==  SUB_LCD_ID)  //for PC Camera mode
    {
        LCD_GetInfo(param_ptr->disp_lcd_id, &lcd_info);
    }

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckMpegParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5163_112_2_18_1_44_5_410,(uint8*)"");

    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->disp_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->encode_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5175_112_2_18_1_44_5_411,(uint8*)"");
        return SCI_ERROR;
    }
    if(param_ptr->data_format != ISP_IMAGE_FORMAT_YUV422 &&
         param_ptr->data_format != ISP_IMAGE_FORMAT_YUV420)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Unsupported data format"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5181_112_2_18_1_44_5_412,(uint8*)"");
        return SCI_ERROR;
    }

    if(ISP_DISPLAY_NONE != param_ptr->disp_lcd_id)
    {
        if(param_ptr->disp_lcd_id >= LCD_SUM)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5189_112_2_18_1_44_5_413,(uint8*)"");
            return SCI_ERROR;
        }

        if(param_ptr->disp_block_id >=BLOCK_SUM)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5195_112_2_18_1_44_5_414,(uint8*)"");
            return SCI_ERROR;
        }

        if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
           (param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5202_112_2_18_1_44_5_415,(uint8*)"");
            return SCI_ERROR;

        }

        if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
        {
            //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5209_112_2_18_1_44_5_416,(uint8*)"");
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
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5225_112_2_18_1_44_5_417,(uint8*)"");
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5240_112_2_18_1_44_5_418,(uint8*)"");

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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5253_112_2_18_1_44_5_419,(uint8*)"d",param_ptr->input_format);
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->disp_range.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5261_112_2_18_1_44_5_420,(uint8*)"");
        return SCI_ERROR;
    }

    p_addr = &param_ptr->input_addr;
    if((p_addr->yaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->uaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->vaddr % ISP_ALIGNED_BYTES))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5273_112_2_18_1_44_5_421,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);
        return SCI_ERROR;
    }

    if(ISP_YUV_INVALIDE(p_addr->yaddr,p_addr->uaddr,p_addr->vaddr))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5282_112_2_18_1_44_5_422,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);

        return SCI_ERROR;
    }

    if(param_ptr->disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5289_112_2_18_1_44_5_423,(uint8*)"");
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5296_112_2_18_1_44_5_424,(uint8*)"");
        return SCI_ERROR;
    }
    if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5301_112_2_18_1_44_5_425,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->disp_range.w > ISP_SCALE_FRAME_MODE_WIDTH_TH)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Width of display rect is out of threshold"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5307_112_2_18_1_44_5_426,(uint8*)"");
        return SCI_ERROR;
    }

    return SCI_SUCCESS;

}

LOCAL uint32 _ISP_ServiceCheckScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
    ISP_ADDRESS_T           *p_addr = SCI_NULL;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5319_112_2_18_1_44_5_427,(uint8*)"");

    if(SCI_NULL == param_ptr)
    {
        return SCI_ERROR;
    }

    if(param_ptr->scale_mode == ISP_SERVICE_SCALE_SLICE &&
       param_ptr->slice_line == 0)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: slice mode sline height = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5329_112_2_18_1_44_5_428,(uint8*)"");
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5342_112_2_18_1_44_5_429,(uint8*)"d",param_ptr->input_format);
        return SCI_ERROR;
    }

    if(param_ptr->output_format != ISP_IMAGE_FORMAT_YUV420 &&
       param_ptr->output_format != ISP_IMAGE_FORMAT_RGB565 &&
       param_ptr->output_format != ISP_IMAGE_FORMAT_YUV422 &&
       param_ptr->output_format != ISP_IMAGE_FORMAT_RGB565_EX &&
       param_ptr->output_format != ISP_IMAGE_FORMAT_RGB888 &&
       param_ptr->output_format != ISP_IMAGE_FORMAT_RGB888_EX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: output_format = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5353_112_2_18_1_44_5_430,(uint8*)"d",param_ptr->output_format);
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->input_rect.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->output_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixel align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5361_112_2_18_1_44_5_431,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->input_rect.x % ISP_ALIGNED_PIXELS)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input start x must be 4 pixel align when scale"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5367_112_2_18_1_44_5_432,(uint8*)"");
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5374_112_2_18_1_44_5_433,(uint8*)"");
        return SCI_ERROR;
    }

    p_addr = &param_ptr->input_addr;

    if((p_addr->yaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->uaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->vaddr % ISP_ALIGNED_BYTES))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5387_112_2_18_1_44_5_434,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);
        return SCI_ERROR;
    }

    if(ISP_YUV_INVALIDE(p_addr->yaddr,p_addr->uaddr,p_addr->vaddr))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5396_112_2_18_1_44_5_435,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);

        return SCI_ERROR;
    }


    if(param_ptr->input_format == ISP_IMAGE_FORMAT_YUV420_3FRAME &&
       param_ptr->output_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5405_112_2_18_1_44_5_436,(uint8*)"");
        return SCI_ERROR;
    }


    return SCI_SUCCESS;

}

LOCAL uint32 _ISP_ServiceCheckDenoiseParam(ISP_DENOISE_PARAM_T * param_ptr)
{
    ISP_ADDRESS_T           *p_addr = SCI_NULL;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckDenoiseParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5418_112_2_18_1_44_5_437,(uint8*)"");

    if(SCI_NULL == param_ptr)
    {
        return SCI_ERROR;
    }

    if(param_ptr->input_format != ISP_IMAGE_FORMAT_YUV422)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input_format = %d, not support denoise format"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5427_112_2_18_1_44_5_438,(uint8*)"d",param_ptr->input_format);
        return SCI_ERROR;
    }

    if(param_ptr->output_format != ISP_IMAGE_FORMAT_YUV422)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: output_format = %d, not support denoise format"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5433_112_2_18_1_44_5_439,(uint8*)"d",param_ptr->output_format);
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w % ISP_ALIGNED_PIXELS)||
        (param_ptr->output_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixel align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5440_112_2_18_1_44_5_440,(uint8*)"");
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w !=  param_ptr->output_size.w) ||
       (param_ptr->input_size.h != param_ptr->output_size.h))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: output size is not corresponding with input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5447_112_2_18_1_44_5_441,(uint8*)"");
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w !=  param_ptr->input_rect.w) ||
       (param_ptr->input_size.h != param_ptr->input_rect.h)||
       (0 != param_ptr->input_rect.x)||(0 != param_ptr->input_rect.y)
       )
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: output rect now not supporting cropping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5456_112_2_18_1_44_5_442,(uint8*)"");
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w >  ISP_SERVICE_DENOISE_MAX_WIDTH))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: too large size for denoise function"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5462_112_2_18_1_44_5_443,(uint8*)"");
        return SCI_ERROR;
    }

    p_addr = &param_ptr->input_addr;

    if((p_addr->yaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->uaddr % ISP_ALIGNED_BYTES) &&
       (p_addr->vaddr % ISP_ALIGNED_BYTES))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5475_112_2_18_1_44_5_444,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);
        return SCI_ERROR;
    }

    if(ISP_YUV_INVALIDE(p_addr->yaddr,p_addr->uaddr,p_addr->vaddr))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input addr , y,u,v {0x%x, 0x%x, 0x%x}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5484_112_2_18_1_44_5_445,(uint8*)"ddd",p_addr->yaddr, p_addr->uaddr,p_addr->vaddr);

        return SCI_ERROR;
    }


    if(param_ptr->input_format == ISP_IMAGE_FORMAT_YUV420_3FRAME &&
       param_ptr->output_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Slice scaling for YUV420 3 frames is not supported"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5493_112_2_18_1_44_5_446,(uint8*)"");
        return SCI_ERROR;
    }

    return SCI_SUCCESS;

}

LOCAL uint32 _ISP_ServiceCheckVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    LCD_INFO_T lcd_info={0x00};

     if(SCI_NULL == param_ptr)
    {
        return SCI_ERROR;
    }
    LCD_GetInfo(param_ptr->local_disp_lcd_id, &lcd_info);

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckVTEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5507_112_2_18_1_44_5_447,(uint8*)"");

    if((param_ptr->local_input_size.w % ISP_ALIGNED_PIXELS) ||
       (param_ptr->local_input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->local_disp_range.w % ISP_ALIGNED_PIXELS) ||
       (param_ptr->encode_size.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixels aligned"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5519_112_2_18_1_44_5_448,(uint8*)"");
        return SCI_ERROR;
    }

    if(0 == param_ptr->local_display_enable)
    {
        return SCI_SUCCESS;
    }

    if(param_ptr->local_disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5530_112_2_18_1_44_5_449,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->local_disp_block_id >= BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5536_112_2_18_1_44_5_450,(uint8*)"");
        return SCI_ERROR;
    }


    if((param_ptr->local_input_size.w < (param_ptr->local_input_range.x + param_ptr->local_input_range.w)) ||
       (param_ptr->local_input_size.h < (param_ptr->local_input_range.y + param_ptr->local_input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5544_112_2_18_1_44_5_451,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5550_112_2_18_1_44_5_452,(uint8*)"");
        return SCI_ERROR;
    }
    if(param_ptr->local_disp_range.w > lcd_info.lcd_width ||
       param_ptr->local_disp_range.h > lcd_info.lcd_height)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Display size is larger than max lcd size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5556_112_2_18_1_44_5_453,(uint8*)"");
        return SCI_ERROR;
    }

    return SCI_SUCCESS;

}
LOCAL uint32 _ISP_ServiceCheckVTExtEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
    LCD_INFO_T lcd_info={0x00};

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceCheckVTExtEncodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5567_112_2_18_1_44_5_454,(uint8*)"");

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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5579_112_2_18_1_44_6_455,(uint8*)"");
        return SCI_ERROR;
    }

    if(0 == param_ptr->local_review_enable)
    {
        return SCI_SUCCESS;
    }

    if(param_ptr->local_disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5590_112_2_18_1_44_6_456,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->local_disp_block_id >= BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5596_112_2_18_1_44_6_457,(uint8*)"");
        return SCI_ERROR;
    }


    if((param_ptr->local_input_size.w < (param_ptr->local_input_range.x + param_ptr->local_input_range.w)) ||
       (param_ptr->local_input_size.h < (param_ptr->local_input_range.y + param_ptr->local_input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5604_112_2_18_1_44_6_458,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5610_112_2_18_1_44_6_459,(uint8*)"");
        return SCI_ERROR;
    }

    LCD_GetInfo(param_ptr->local_disp_lcd_id, &lcd_info);
    if(param_ptr->local_disp_range.w > lcd_info.lcd_width||
       param_ptr->local_disp_range.h > lcd_info.lcd_height)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Display size is larger than max lcd size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5618_112_2_18_1_44_6_460,(uint8*)"");
        return SCI_ERROR;
    }

    return SCI_SUCCESS;

}

LOCAL uint32 _ISP_ServiceCheckVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr)
{

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_CheckVTDecodeParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5629_112_2_18_1_44_6_461,(uint8*)"");

    if(SCI_NULL == param_ptr)
    {
        return SCI_ERROR;
    }

    if((param_ptr->remote_input_size.w % ISP_ALIGNED_PIXELS) ||
       (param_ptr->remote_input_range.w % ISP_ALIGNED_PIXELS)||
       (param_ptr->remote_disp_range.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: All width must be 4 pixel align"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5640_112_2_18_1_44_6_462,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->remote_input_range.x % ISP_ALIGNED_PIXELS)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: input start x must be 4 pixel align when review"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5646_112_2_18_1_44_6_463,(uint8*)"");
        return SCI_ERROR;
    }

    if((param_ptr->remote_input_size.w < (param_ptr->remote_input_range.x + param_ptr->remote_input_range.w)) ||
       (param_ptr->remote_input_size.h < (param_ptr->remote_input_range.y + param_ptr->remote_input_range.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Input range is out of input size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5653_112_2_18_1_44_6_464,(uint8*)"");
        return SCI_ERROR;
    }

    if(0 == param_ptr->remote_display_enable)
    {
        return SCI_SUCCESS;
    }

    if(param_ptr->remote_disp_lcd_id >= LCD_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5664_112_2_18_1_44_6_465,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->remote_disp_block_id >=BLOCK_SUM)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Invalid lcd id or block id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5670_112_2_18_1_44_6_466,(uint8*)"");
        return SCI_ERROR;
    }

    if(param_ptr->remote_disp_rotation >= ISP_ROTATION_MAX)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: Rotation angle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5676_112_2_18_1_44_6_467,(uint8*)"");
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

}
LOCAL void _ISP_ServiceStartVTReview(ISP_ADDRESS_T *p_review)
{

}

LOCAL void _ISP_ServiceStartVTPlayBack(void *ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             src_frm = {0}, dst_frm = {0};
    ISP_ADDRESS_T           *p_addr = (ISP_ADDRESS_T*)ptr;
    uint32                  frm_cnt = 0;

    src_frm.yaddr = p_addr->yaddr;
    src_frm.uaddr = p_addr->uaddr;
    src_frm.vaddr = p_addr->vaddr;
    src_frm.width = s->remote_input_size.w;
    src_frm.height = s->remote_input_size.h;
    src_frm.type = s->remote_input_fmt;
    dst_frm.type = s->remote_display_block.img_fmt;
    dst_frm.width = s->remote_display_range.w;
    dst_frm.height = s->remote_display_range.h;
    dst_frm.flags = ISP_STATE_VP_REVIEW_REMOTE;
    frm_cnt = s->vt_review_remote_count % ISP_PATH_FRAME_COUNT_MAX;
    dst_frm.yaddr = s->remote_display_addr[frm_cnt].yaddr;
    dst_frm.uaddr = s->remote_display_addr[frm_cnt].uaddr;
    _ISP_ServiceInternScale(&src_frm, &s->remote_input_range, &dst_frm);
    s->vt_review_remote_count ++;
}

LOCAL void _ISP_ServiceStartVTExtScale(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             in_frame_t = {0};
    ISP_FRAME_T             out_frame_t = {0};
    ISP_RECT_T              isp_rect = {0};

    //ISP_SERVICE_LOG:"ISP_SERVICE: To do image scaling"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5730_112_2_18_1_44_6_468,(uint8*)"");

    if(ISP_STATE_STOP == s->state || ISP_SERVICE_VP_EX != s->service)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: state error! state %d, service %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5734_112_2_18_1_44_6_469,(uint8*)"dd", s->state, s->service);
        return ;
    }

    isp_rect.x = s->remote_input_range.x;
    isp_rect.y = s->remote_input_range.y;
    isp_rect.w = s->remote_input_range.w;
    isp_rect.h = s->remote_input_range.h;
    in_frame_t.type = (uint8)s->path_input_image_format;
    in_frame_t.yaddr = s->remote_input_addr.yaddr;
    in_frame_t.uaddr = s->remote_input_addr.uaddr;
    in_frame_t.vaddr = s->remote_input_addr.vaddr;
    in_frame_t.width = s->remote_input_size.w;
    in_frame_t.height = s->remote_input_size.h;
    out_frame_t.flags = ISP_STATE_VP_SCALE_REMOTE;

    if(s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565_EX)
    {
        out_frame_t.type = ISP_IMAGE_FORMAT_RGB565;
    }
    else
    {
        out_frame_t.type = s->path_output_image_format;
    }
    //ISP_SERVICE_LOG:"ISP_SERVICE:transform the image frame, the flag of src and dst: 0x%x to 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5760_112_2_18_1_44_6_470,(uint8*)"dd",in_frame_t.flags,out_frame_t.flags);

    out_frame_t.yaddr = s->output_addr.yaddr;
    out_frame_t.uaddr = s->output_addr.uaddr;
    out_frame_t.vaddr = s->output_addr.vaddr;
    out_frame_t.width = s->remote_display_range.w;
    out_frame_t.height = s->remote_display_range.h;
    _ISP_ServiceInternScale(&in_frame_t, &isp_rect, &out_frame_t);
    s->vt_review_remote_count ++;

}

LOCAL void _ISP_ServiceStartPreview(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  i = 0;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              disp_size = {0};
    uint32                  param = 0;
    uint32                  addr = 0;
    uint32                  yuv_size = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5780_112_2_18_1_44_6_471,(uint8*)"");

    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_PREVIEW);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set CAP*/
    rtn_drv = _ISP_ServiceConfigCap();
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path*/
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_SIZE,
                                   (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_RECT,
                                   (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    disp_size.w = s->display_range.w;
    disp_size.h = s->display_range.h;

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_SIZE,
                                   (void*)&disp_size);
    ISP_RTN_IF_ERR(rtn_drv);


    if(ISP_DATA_YUV422 != s->display_block.img_fmt)
    {
        param = ISP_DATA_RGB565;
    }
    else
    {
        param = ISP_DATA_YUV422;
    }

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    for(i = 0; i < s->disp_frm_round_num; i++)
    {
        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                       ISP_PATH_OUTPUT_ADDR,
                                       (void*)&s->display_addr[i]);
        ISP_RTN_IF_ERR(rtn_drv);
    }

    /*Register ISR callback*/
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_TX_DONE,
                                       _ISP_ServiceOnTxDone);
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
                                       ISP_IRQ_NOTICE_CAP_LINE_ERR,
                                       _ISP_ServiceOnSensorLineErr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_CAP_FRAME_ERR,
                                       _ISP_ServiceOnSensorFrameErr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_ISP_ERR_HANDLE,
                                       _ISP_ServiceOnSensorHardwareErr);
    ISP_RTN_IF_ERR(rtn_drv);


    s->state = ISP_STATE_PREVIEW;
    rtn_drv = ISP_DriverStart(s->module_addr);

    _ISP_Uix_ServiceGetDataCtrl();
    hal_Camera_LinkFrames();

    addr = s->uix_base_addr;

    CAM_MODE_CHECK();
    yuv_size = p_Dcam_cfg[g_DcamMode]->width * p_Dcam_cfg[g_DcamMode]->height * 2;

    if(addr != 0 && (addr - s->uix_base_addr) < s->uix_mem_size)
    {
        rtn_drv = hal_camera_setOutputAddr(addr);
        ISP_RTN_IF_ERR(rtn_drv);
        addr += yuv_size;
        addr += YUV_PAD_SIZE;
    }

    if(addr != 0 && (addr - s->uix_base_addr) < s->uix_mem_size)
    {
        rtn_drv = hal_camera_setOutputAddr(addr);
        ISP_RTN_IF_ERR(rtn_drv);
    }

    SCI_TRACE_LOW("ISP:disp size:%u(k) dcam mode = %d YUV size = %d(k)",
                         s->uix_display_size/1024, g_DcamMode, yuv_size/1024);

    g_ispmode = CAM_PREVIEW_STAGE_START;
    g_stopPreview = 0;
    _ISP_Uix_ServiceRelDataCtrl();
    rtn_drv = _ISP_UixCaptureOneFrame();

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5867_112_2_18_1_44_6_472,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }
    else
    {
        s->service = ISP_SERVICE_PREVIEW;
    }

    return ;
}

LOCAL void _ISP_ServiceStartJpeg(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              output_size = {0};
    ISP_RECT_T              path_rect = {0};
    uint32                  param = 0;
    uint32                  addr = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartJpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5887_112_2_18_1_44_6_473,(uint8*)"");

    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_CAPTURE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set CAP*/
    rtn_drv = _ISP_ServiceConfigCap();
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path1*/
    output_size.w = s->cap_output_size.w;
    output_size.h = s->cap_output_size.h;

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_SIZE,
                                   (void*)&output_size);
    ISP_RTN_IF_ERR(rtn_drv);

    path_rect.w = output_size.w;
    path_rect.h = output_size.h;
    path_rect.x = 0;
    path_rect.y = 0;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_RECT,
                                   (void*)&path_rect);
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

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_SIZE,
                                   (void*)&output_size);
    ISP_RTN_IF_ERR(rtn_drv);


    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_ADDR,
                                   (void*)&s->encoder_addr[0]);
    ISP_RTN_IF_ERR(rtn_drv);


    param = (uint32)s->path_output_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Register ISR callback*/
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_TX_DONE,
                                       _ISP_ServiceOnTxDone);
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
                                       ISP_IRQ_NOTICE_CAP_LINE_ERR,
                                       _ISP_ServiceOnSensorLineErr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_CAP_FRAME_ERR,
                                       _ISP_ServiceOnSensorFrameErr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_ISP_ERR_HANDLE,
                                       _ISP_ServiceOnSensorHardwareErr);
    ISP_RTN_IF_ERR(rtn_drv);
    if(0 == s->is_slice)
    {
        s->state = ISP_STATE_CAPTURE_DONE;
    }
    else
    {
        s->state = ISP_STATE_CAPTURE_SCALE;
    }

    rtn_drv = ISP_DriverStart(s->module_addr);

///----------------------- W217 ------------------------------------

    SCI_TRACE_LOW("ISP:ISP_UixCaptureOneFrame");

    _ISP_Uix_ServiceGetDataCtrl();

    g_ispmode = CAM_CAPTURE_STAGE_START;

    addr = s->uix_base_addr;

    //Only ONE (640*480*2) buffer for snapshot.
    if(addr != 0 && (addr - s->uix_base_addr) < s->uix_mem_size)
    {
        hal_Camera_SetSnapshotAddr(addr);
        //ISP_RTN_IF_ERR(rtn_drv);
        addr += p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->width * p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->height * 2;
        addr += YUV_PAD_SIZE;
    }

    #if 0
    if(addr != 0 && (addr - s->uix_base_addr) < s->uix_mem_size)
    {
        rtn_drv = hal_camera_setOutputAddr(addr);
        ISP_RTN_IF_ERR(rtn_drv);
    }
    #endif

    _ISP_Uix_ServiceRelDataCtrl();
    rtn_drv = _ISP_UixCaptureOneFrame();

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_5985_112_2_18_1_44_6_474,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }

    return ;
}

LOCAL void _ISP_ServiceJPEGHandler(ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_ADDRESS_T           isp_addr = {0};
    ISP_FRAME_T             isp_frame_t = {0};
    uint32                       mem_size = 0;
    uint32                       mem_start = 0;

    SCI_TRACE_LOW("ISP:JPEGHandler");

    isp_addr.yaddr = p_frame->yaddr;
    isp_addr.uaddr = p_frame->uaddr;
    isp_addr.vaddr = p_frame->vaddr;

    // david add.  display pic frame
    _ISP_ServiceDisplayOneFrame(0, p_frame);

    // Jpeg enc
    _ISP_ServiceCallUserEncCB((void*)&isp_addr);

    s->enc_stoped = 1;

}

LOCAL void _ISP_ServiceScaleHandler(ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_ADDRESS_T           isp_addr = {0};
    ISP_SIZE_T              isp_size = {0};
    uint32                  status = 0;

    if(s->state == ISP_STATE_CAPTURE_SCALE)
    {
        if(s->is_slice)
        {
            if(s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565)
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
                                      s->path_output_image_format,
                                      DMABUFFER_FROM_DEVICE);


            isp_size.w = p_frame->width;
            isp_size.h = p_frame->height - s->total_scale_out;
            (*s->scale_user_cb)(isp_addr, p_frame->width, isp_size.h);
        }

        if(s->is_slice && s->is_slice_alone == SCI_FALSE)
        {
            if(s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565)
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
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6177_112_2_18_1_44_7_475,(uint8*)"dd",s->total_scale_out,s->encoder_size.h);
                s->is_slice_alone = SCI_FALSE;
                s->state = ISP_STATE_STOP;
            }
            status = SCI_SetEvent(s_isp_event, ISP_SCALE_EVENT, SCI_OR);
            SCI_ASSERT(!status); /*assert verified*/
        }
    }

}

LOCAL int32 _ISP_ServiceConfigCap(void)
{
    return ISP_DRV_RTN_SUCCESS;

    //cov 37221
    #if 0
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_CAP_SYNC_POL_T      cap_sync = {0};
    ISP_ATV_CTRL_T          atv_ctrl = {0};
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  param = 0;

    cap_sync.vsync_pol = s->vsync_polarity;
    cap_sync.hsync_pol = s->hsync_polarity;
    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_SYNC_POL,
                                  (void*)&cap_sync);
    ISP_RTN_IF_ERR(rtn_drv);

    if(ISP_SERVICE_JPEG == s->service)
    {
        param = (uint32)s->capture_skip_frame_num;
    }
    else
    {
        param = (uint32)s->preview_deci_frame_num;
        rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                      ISP_CAP_FRM_DECI,
                                      (void*)&param);
        ISP_RTN_IF_ERR(rtn_drv);

        param = (uint32)s->preview_skip_frame_num;
    }
    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_SKIP_CNT,
                                  (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    if(ISP_IMAGE_FORMAT_JPEG == s->cap_input_image_format)
    {
        param = ISP_CAP_INPUT_FORMAT_JPEG;
    }
    else
    {
        param = ISP_CAP_INPUT_FORMAT_YUV;
    }
    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_INPUT_FORMAT,
                                  (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    param = (uint32)s->cap_sensor_if_mode;
    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_IF_MODE,
                                  (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    param = (uint32)s->cap_sensor_if_endian;
    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_IF_ENDIAN,
                                  (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    param = s->cap_input_image_patten;
    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_YUV_TYPE,
                                  (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_INPUT_SIZE,
                                  (void*)&s->cap_input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_INPUT_RECT,
                                  (void*)&s->cap_input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_FRM_COUNT_CLR,
                                  NULL);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                  ISP_CAP_IMAGE_XY_DECI,
                                  (void*)&s->cap_img_dec);

#ifndef CHIP_VER_6531EFM
    rtn_drv = ISP_DriverPclkSwitch(APB_PIN_CTL1,(ISP_CCIR_PCLK_SOURCE_E)s->ccir_pclk_src);
    rtn_drv = ISP_DriverD0Switch(APB_PIN_CTL1,(ISP_CCIR_D0_SOURCE_E)s->ccir_d0_src);
    rtn_drv = ISP_DriverVsyncSwitch(APB_PIN_CTL1,(ISP_CCIR_VSYNC_SOURCE_E)s->ccir_vsync_src);
#else
    rtn_drv = ISP_DriverPclkSwitchSc6531EFM(PIN_CMPCLK_REG, s->cap_sensor_if_mode);
    rtn_drv = ISP_DriverD0SwitchSc6531EFM(PIN_CCIRD0_REG, s->cap_sensor_if_mode);
    rtn_drv = ISP_DriverVsyncSwitchSc6531EFM(PIN_CCIRVS_REG, s->cap_sensor_if_mode);
    rtn_drv = ISP_DriverD1SwitchSc6531EFM(PIN_CCIRD1_REG, s->cap_sensor_if_mode, s->ccir_d1_src);
#endif

    if(s->b_is_atv)
    {
        atv_ctrl.is_eb = s->fix_mode_eb;
        atv_ctrl.start = s->thre_start;
        atv_ctrl.end   = s->thre_end;
        rtn_drv = ISP_DriverCapConfig(s->module_addr,
                                      ISP_CAP_ATV_MODE,
                                      (void*)&atv_ctrl);
    }


exit:

    return rtn_drv;
   #endif

}

LOCAL void _ISP_ServiceInternScale(ISP_FRAME_T *p_in_frame, ISP_RECT_T *p_in_rect, ISP_FRAME_T *p_out_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              size = {0};
    ISP_ADDRESS_T           addr = {0};
    uint32                  param = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceInternScale, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6304_112_2_18_1_44_7_476,(uint8*)"d", SCI_GetTickCount());

    //ISP_SERVICE_LOG:"_ISP_ServiceInternScale, src size {%d %d}, src trim {%d %d %d %d}, dst {%d %d}, src fmt %d, dst fmt %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6316_112_2_18_1_44_7_477,(uint8*)"dddddddddd",p_in_frame->width,p_in_frame->height,p_in_rect->x,p_in_rect->y,p_in_rect->w,p_in_rect->h,p_out_frame->width,p_out_frame->height,p_in_frame->type,p_out_frame->type);

    //ISP_SERVICE_LOG:"_ISP_ServiceInternScale, src addr {0x%x 0x%x}, dst addr {0x%x 0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6322_112_2_18_1_44_7_478,(uint8*)"dddd",p_in_frame->yaddr,p_in_frame->uaddr,p_out_frame->yaddr,p_out_frame->uaddr);

    if(s->enc_stoped)
    {
        ISP_SERVICE_TRACE("ISP:InternScale error");
        return;
    }
    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_MPEG_SCALE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path*/
    size.w = p_in_frame->width;
    size.h = p_in_frame->height;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_SIZE,
                                   (void*)&size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_RECT,
                                   (void*)p_in_rect);
    ISP_RTN_IF_ERR(rtn_drv);

    addr.yaddr = p_in_frame->yaddr;
    addr.uaddr = p_in_frame->uaddr;
    addr.vaddr = p_in_frame->vaddr;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_ADDR,
                                   (void*)&addr);
    ISP_RTN_IF_ERR(rtn_drv);

    _ISP_ServiceSyncImageData(&addr,
                              size.w,
                              size.h,
                              s->path_input_image_format,
                              DMABUFFER_TO_DEVICE);

    param = (uint32)p_in_frame->type;
    if(param == ISP_IMAGE_FORMAT_RGB565_EX)
    {
        param = ISP_IMAGE_FORMAT_RGB565;
    }
    else if(param == ISP_IMAGE_FORMAT_RGB888_EX)
    {
        param = ISP_IMAGE_FORMAT_RGB888;
    }
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    size.w = p_out_frame->width;
    size.h = p_out_frame->height;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_SIZE,
                                   (void*)&size);
    ISP_RTN_IF_ERR(rtn_drv);

    addr.yaddr = p_out_frame->yaddr;
    addr.uaddr = p_out_frame->uaddr;
    addr.vaddr = p_out_frame->vaddr;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_ADDR,
                                   (void*)&addr);
    ISP_RTN_IF_ERR(rtn_drv);

    if(p_out_frame->type == ISP_IMAGE_FORMAT_RGB565)
    {
        param = SCI_TRUE;
        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                       ISP_PATH_DITHER_EN,
                                       (void*)&param);
        ISP_RTN_IF_ERR(rtn_drv);
    }


    param = p_out_frame->type;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    param = p_out_frame->flags;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                  ISP_PATH_OUTPUT_FRAME_FLAG,
                                  (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Register ISR callback*/

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_TX_DONE,
                                       _ISP_ServiceOnTxDone);
    ISP_RTN_IF_ERR(rtn_drv);

    s->state = p_out_frame->flags;
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6427_112_2_18_1_44_7_479,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }

    return;
}

LOCAL void _ISP_ServiceCaptureOnce(uint32 output_fmt, uint32 output_flag)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              isp_size = {0};
    ISP_RECT_T              isp_rect = {0};
    uint32                  param = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceCaptureOnce, time %d, output ftm %d, size {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6449_112_2_18_1_44_7_480,(uint8*)"dddd", SCI_GetTickCount(),output_fmt,s->cap_output_size.w,s->cap_output_size.h);

    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_MPEG_CAP);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set CAP*/
    rtn_drv = _ISP_ServiceConfigCap();
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path*/
    isp_size.w = (s->cap_input_range.w / (uint32)(1 << s->cap_img_dec.x_factor));
    isp_size.h = (s->cap_input_range.h / (uint32)(1 << s->cap_img_dec.y_factor));
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_SIZE,
                                   (void*)&isp_size);
    ISP_RTN_IF_ERR(rtn_drv);

    if(isp_size.w * s->cap_output_size.h > s->cap_output_size.w * isp_size.h)
    {
        isp_rect.h = isp_size.h;
        isp_rect.y = 0;
        isp_rect.w = s->cap_output_size.w * isp_size.h / s->cap_output_size.h;
        isp_rect.w = ISP_PIXEL_LEFT_ALIGNED_(isp_rect.w, ISP_ALIGNED_PIXELS);
        isp_rect.x = (isp_size.w - isp_rect.w) >> 1;
        isp_rect.x &= ~1;
    }
    else
    {
        isp_rect.w = isp_size.w;
        isp_rect.x = 0;
        isp_rect.h = isp_size.w * s->cap_output_size.h / s->cap_output_size.w;
        isp_rect.h = ISP_PIXEL_LEFT_ALIGNED_(isp_rect.h, ISP_ALIGNED_PIXELS);
        isp_rect.y = (isp_size.h - isp_rect.h) >> 1;
        isp_rect.y &= ~1;
    }

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_RECT,
                                   (void*)&isp_rect);
    ISP_RTN_IF_ERR(rtn_drv);

    param = output_fmt;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_SIZE,
                                   (void*)&s->cap_output_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_ADDR,
                                   (void*)&s->local_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    param = output_flag;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_FRAME_FLAG,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Register ISR callback*/
    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_TX_DONE,
                                       _ISP_ServiceOnTxDone);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_CAP_FIFO_OF,
                                       _ISP_ServiceOnCAPFifoOverflow);
    ISP_RTN_IF_ERR(rtn_drv);


    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_CAP_LINE_ERR,
                                       _ISP_ServiceOnSensorLineErr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_CAP_FRAME_ERR,
                                       _ISP_ServiceOnSensorFrameErr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_ISP_ERR_HANDLE,
                                       _ISP_ServiceOnSensorHardwareErr);
    ISP_RTN_IF_ERR(rtn_drv);

    s->state = output_flag;
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6548_112_2_18_1_44_7_481,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }

    return ;
}


LOCAL void _ISP_ServiceStartMpeg(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartMpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6563_112_2_18_1_44_7_482,(uint8*)"");

    _ISP_ServiceEncodeFrameInit();

    hal_Camera_LinkFrames();

    ISP_Uix_AllocFrameAddr();

    ISP_Uix_AllocEncFrame();

    g_stopPreview = 0;
    g_ispmode = CAM_PREVIEW_STAGE_START;

    _ISP_UixCaptureOneFrame();

    s->enc_stoped = 0;
    s->state = ISP_STATE_MPEG;
    s->service = ISP_SERVICE_MPEG;
    return ;
}


LOCAL void _ISP_ServiceMpegHandler(ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             dst_frame = {0};
    uint32                  status = 0;
    ISP_ADDRESS_T           src_addr = {0};
    ISP_ADDRESS_T           dst_addr = {0};
    uint32                  rot_angle = ROTATION_DIR_MAX;
    uint32                  frm_cnt = 0;
    int                     cb_rtn = ISP_CB_PROC_DONE;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceMpegHandler, p_frame->flags 0x%x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6586_112_2_18_1_44_7_483,(uint8*)"d",p_frame->flags);

    if(s->enc_stoped)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: MPEG stopped"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6590_112_2_18_1_44_7_484,(uint8*)"");
        return;
    }

    switch(p_frame->flags)
    {
        case ISP_STATE_MPEG_CAPTURE:

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one capture frame, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6599_112_2_18_1_44_7_485,(uint8*)"d", SCI_GetTickCount());

            if(s->b_is_atv&&(s->local_review_enable||s->local_display_enable))
            {
                _ISP_ServiceCallUserPostCapCB(0);
            }

            SCI_MEMCPY((void*)&s->local_frame,(void*)p_frame, sizeof(ISP_FRAME_T));

            if(s->enc_rotation == ISP_ROTATION_0 &&
               ISP_DriverFrameIsLocked(s->p_frame_enc_cur))
            {
                // No need to rotate image, but the current frame has not been released by DV.
                // then just do next review, and do not encode this frame.
                //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceMpegHandler, encode frame not released"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6608_112_2_18_1_44_7_486,(uint8*)"");

                if(s->local_review_enable)
                {
                    dst_frame.type = s->display_block.img_fmt;
                    dst_frame.width = s->display_range.w;
                    dst_frame.height = s->display_range.h;
                    dst_frame.flags = ISP_STATE_MPEG_REVIEW;
                    frm_cnt = s->display_frame_count % ISP_PATH_FRAME_COUNT_MAX;
                    dst_frame.yaddr = s->display_addr[frm_cnt].yaddr;
                    dst_frame.uaddr = s->display_addr[frm_cnt].uaddr;

                    _ISP_ServiceInternScale(&s->local_frame, &s->input_range, &dst_frame);
                }
            }
            else
            {
                dst_frame.type   = ISP_IMAGE_FORMAT_YUV422; //s->path_output_image_format;
                dst_frame.width  = 640;   //s->encoder_size.w;
                dst_frame.height = 480;  //s->encoder_size.h;
                dst_frame.flags  = ISP_STATE_MPEG_SCALE;
                if(s->enc_rotation != ISP_ROTATION_0)
                {
                    dst_frame.yaddr = s->encoder_rot_addr[0].yaddr;
                    dst_frame.uaddr = s->encoder_rot_addr[0].uaddr;
                }
                else
                {
                    dst_frame.yaddr = s->p_frame_enc_cur->yaddr;
                    dst_frame.uaddr = s->p_frame_enc_cur->uaddr;
                }
                _ISP_ServiceInternScale(p_frame, &s->remote_input_range, &dst_frame);
            }

            if(s->is_enc_first_frame)
            {
                status = SCI_SetEvent(s_isp_event, ISP_MPEG_START_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
                s->is_enc_first_frame = 0;
            }

            break;

        case ISP_STATE_MPEG_SCALE:

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one encode frame, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6647_112_2_18_1_44_8_487,(uint8*)"d", SCI_GetTickCount());

            if(!ISP_DriverFrameIsLocked(s->p_frame_enc_cur)) // is the current frame has not been released, drop this encode frame
            {
                if(s->enc_rotation != ISP_ROTATION_0)// need rotation
                {
                    rot_angle = _ISP_ServiceRotAngle2DrvAngle(s->enc_rotation);

                    dst_addr.yaddr = s->p_frame_enc_cur->yaddr;
                    dst_addr.uaddr = s->p_frame_enc_cur->uaddr;
                    src_addr.yaddr = p_frame->yaddr;
                    src_addr.uaddr = p_frame->uaddr;
                    _ISP_ServiceRotateImage(&src_addr,
                                            &dst_addr,
                                            s->encoder_size.w,
                                            s->encoder_size.h,
                                            rot_angle,
                                            s->path_output_image_format);

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
                s->p_frame_enc_cur->type = s->path_output_image_format;
                ISP_DriverFrameLock(s->p_frame_enc_cur);// unlocked by user
                _ISP_ServiceCallUserEncCB((void*)s->p_frame_enc_cur);
                s->p_frame_enc_cur = s->p_frame_enc_cur->next;
            }
            else
            {
                //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceMpegHandler, encode frame 0x%x not released!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6687_112_2_18_1_44_8_488,(uint8*)"d",s->p_frame_enc_cur);
            }

            if(s->local_review_enable)
            {
                dst_frame.type = s->display_block.img_fmt;
                dst_frame.width = s->display_range.w;
                dst_frame.height = s->display_range.h;
                dst_frame.flags = ISP_STATE_MPEG_REVIEW;
                frm_cnt = s->display_frame_count % ISP_PATH_FRAME_COUNT_MAX;
                dst_frame.yaddr = s->display_addr[frm_cnt].yaddr;
                dst_frame.uaddr = s->display_addr[frm_cnt].uaddr;
                _ISP_ServiceInternScale(&s->local_frame, &s->input_range, &dst_frame);
            }
            else
            {
                s->preview_skip_frame_num = 1;
                _ISP_ServiceCaptureOnce(s->first_cap_format, ISP_STATE_MPEG_CAPTURE);
            }

            break;

        case ISP_STATE_MPEG_REVIEW:

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one review frame, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6719_112_2_18_1_44_8_489,(uint8*)"d", SCI_GetTickCount());

            cb_rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);
            if(ISP_CB_PROC_DONE == cb_rtn)
            {
                _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
                if(s->b_is_atv&&(s->local_review_enable||s->local_display_enable))
                {
                    _ISP_ServiceCallUserPostCapCB(1);
                }
            }
            else
            {
                s->disp_blk_id = s->display_block.block_id;
            }

            if(s->is_first_frame)
            {
                s->is_first_frame = 0;
            }

            s->preview_skip_frame_num = 0;
            _ISP_ServiceCaptureOnce(s->first_cap_format, ISP_STATE_MPEG_CAPTURE);

            break;

        default:

            break;

    }

    if(s->err_code)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = 0;
    }

    return;
}


LOCAL void _ISP_ServiceStartReview(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              disp_size = {0};
    uint32                  param = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartReview, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6760_112_2_18_1_44_8_490,(uint8*)"d", SCI_GetTickCount());

    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_REVIEW);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Set Path*/
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_INPUT_SIZE,
                                    (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_INPUT_RECT,
                                    (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_INPUT_ADDR,
                                    (void*)&s->input_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    param = (uint32)s->path_input_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_INPUT_FORMAT,
                                    (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    disp_size.w = s->display_range.w;
    disp_size.h = s->display_range.h;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_OUTPUT_SIZE,
                                    (void*)&disp_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_OUTPUT_ADDR,
                                    (void*)&s->display_addr[s->display_frame_count%ISP_REVIEW_FRAME_NUM]);
    ISP_RTN_IF_ERR(rtn_drv);

    if(s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565)
    {
        param = SCI_TRUE;
        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                        ISP_PATH_DITHER_EN,
                                        (void*)&param);
        ISP_RTN_IF_ERR(rtn_drv);
    }

    param = (uint32)s->path_output_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_OUTPUT_FORMAT,
                                    (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Register ISR callback*/

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_TX_DONE,
                                       _ISP_ServiceOnTxDone);
    ISP_RTN_IF_ERR(rtn_drv);

    _ISP_ServiceSyncImageData(&s->input_addr,
                              s->input_size.w,
                              s->input_size.h,
                              s->path_input_image_format,
                              DMABUFFER_TO_DEVICE);

    s->state = ISP_STATE_REVIEW;
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6844_112_2_18_1_44_8_491,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }

    return;

}

LOCAL void _ISP_ServiceContinueReviewing(void* addr_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    ISP_SIZE_T              disp_size = {0};
    uint32                  param = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceContinueReviewing, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6863_112_2_18_1_44_8_492,(uint8*)"d", SCI_GetTickCount());

    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_REVIEW);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path*/
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_INPUT_SIZE,
                                    (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_INPUT_RECT,
                                    (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_INPUT_ADDR,
                                    addr_ptr);
    ISP_RTN_IF_ERR(rtn_drv);

    param = (uint32)s->path_input_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_INPUT_FORMAT,
                                    (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    disp_size.w = s->display_range.w;
    disp_size.h = s->display_range.h;

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_OUTPUT_SIZE,
                                    (void*)&disp_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_OUTPUT_ADDR,
                                    (void*)&s->display_addr[s->display_frame_count%ISP_REVIEW_FRAME_NUM]);
    ISP_RTN_IF_ERR(rtn_drv);

    if(s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565)
    {
        param = SCI_TRUE;
        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                        ISP_PATH_DITHER_EN,
                                        (void*)&param);
        ISP_RTN_IF_ERR(rtn_drv);
    }

    param = (uint32)s->path_output_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_OUTPUT_FORMAT,
                                    (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Register ISR callback*/

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_TX_DONE,
                                       _ISP_ServiceOnTxDone);
    ISP_RTN_IF_ERR(rtn_drv);

    _ISP_ServiceSyncImageData(&s->input_addr,
                              s->input_size.w,
                              s->input_size.h,
                              s->path_input_image_format,
                              DMABUFFER_TO_DEVICE);

    s->state = ISP_STATE_REVIEW;
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6944_112_2_18_1_44_8_493,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }

    return;
}


LOCAL void _ISP_ServiceStartScale(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    uint32                  param = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartScale,input addr {0x%x 0x%x 0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6964_112_2_18_1_44_8_494,(uint8*)"ddd",s->input_addr.yaddr,s->input_addr.uaddr,s->input_addr.vaddr);

    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSoftReset(AHB_REG_BASE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_SCALE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path*/
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_SIZE,
                                   (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_RECT,
                                   (void*)&s->input_range);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_ADDR,
                                   (void*)&s->input_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    param = (uint32)s->path_input_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_SIZE,
                                   (void*)&s->encoder_size);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_ADDR,
                                   (void*)&s->encoder_addr[0]);
    ISP_RTN_IF_ERR(rtn_drv);

    if(s->is_slice)
    {
        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                       ISP_PATH_SLICE_SCALE_EN,
                                       (void*)SCI_NULL);
        ISP_RTN_IF_ERR(rtn_drv);

        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                       ISP_PATH_SLICE_SCALE_HEIGHT,
                                       (void*)&s->slice_height);
        ISP_RTN_IF_ERR(rtn_drv);

        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                       ISP_PATH_SWAP_BUFF,
                                       (void*)&s->swap_buff);
        ISP_RTN_IF_ERR(rtn_drv);

        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                       ISP_PATH_LINE_BUFF,
                                       (void*)&s->line_buff);
        ISP_RTN_IF_ERR(rtn_drv);


    }

    if(s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565_EX ||
       s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565)
    {
        param = SCI_TRUE;
        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                       ISP_PATH_DITHER_EN,
                                       (void*)&param);
        ISP_RTN_IF_ERR(rtn_drv);

        param = ISP_IMAGE_FORMAT_RGB565;
    }
    else
    {
        param = (uint32)s->path_output_image_format;
    }

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Register ISR callback*/

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_TX_DONE,
                                       _ISP_ServiceOnTxDone);
    ISP_RTN_IF_ERR(rtn_drv);

    _ISP_ServiceSyncImageData(&s->input_addr,
                              s->input_size.w,
                              (uint32)(s->input_range.y + s->input_range.h),
                              s->path_input_image_format,
                              DMABUFFER_TO_DEVICE);

    if(s->is_slice == SCI_TRUE || s->is_slice_alone == SCI_TRUE)
    {
        s->state = ISP_STATE_SCALE_SLICE;
    }
    else
    {
        s->state = ISP_STATE_SCALE;
    }
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:
    if (PNULL != s->start_scale_callback)
    {
        s->start_scale_callback(s->start_scale_param);
    }

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7080_112_2_18_1_44_8_495,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }
}

LOCAL void _ISP_ServiceContinueScaling(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    int32                   rtn_drv = SCI_SUCCESS;
    uint32                  dst_buf_id = 0;
    ISP_RECT_T              rect = {0};
    ISP_ADDRESS_T           next_addr = {0};
    uint32                  param = 0;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceContinueScaling, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7104_112_2_18_1_44_8_496,(uint8*)"d", SCI_GetTickCount());

    rtn_drv = ISP_DriverInit(AHB_REG_BASE, s->module_addr);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMode(s->module_addr, ISP_MODE_SCALE);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverSetMem(s->p_scale_coeff_mem);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set Path*/
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_SIZE,
                                   (void*)&s->input_size);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set next input trim rect ---------start----------*/
    rect.x = s->input_range.x;
    rect.y = 0;
    rect.w = s->input_range.w;
    rect.h = s->input_range.h;

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_RECT,
                                   (void*)&rect);
    ISP_RTN_IF_ERR(rtn_drv);
    /*Set next input trim rect ---------end------------*/


    /*Set next input addr ---------start----------*/
    s->total_slice_count ++;
    next_addr.yaddr = s->input_addr.yaddr;
    next_addr.uaddr = s->input_addr.uaddr;

    if(s->is_slice_alone == SCI_FALSE)
    {
        if(ISP_IMAGE_FORMAT_YUV422 == s->path_input_image_format ||
           ISP_IMAGE_FORMAT_YUV420 == s->path_input_image_format)
        {
            next_addr.yaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w;
            if(ISP_IMAGE_FORMAT_YUV420 == s->path_input_image_format)
            {
                next_addr.uaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w / 2;
            }
            else
            {
                next_addr.uaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w;
            }
        }
        else if(ISP_IMAGE_FORMAT_RGB888 == s->path_input_image_format)
        {
            next_addr.yaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w * 4;
        }
        else// RGB565
        {
            next_addr.yaddr += (s->input_range.y + s->total_slice_count * s->slice_height) * s->input_size.w * 2;
        }
    }


    //ISP_SERVICE_LOG:"ISP_SERVICE:_ISP_ServiceContinueScaling,input addr {0x%x, 0x%x}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7167_112_2_18_1_44_9_497,(uint8*)"dd",next_addr.yaddr,next_addr.uaddr);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_ADDR,
                                   (void*)&next_addr);
    ISP_RTN_IF_ERR(rtn_drv);
    /*Set next input addr ---------end------------*/

    param = (uint32)s->path_input_image_format;
    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_INPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_SIZE,
                                   (void*)&s->encoder_size);
    ISP_RTN_IF_ERR(rtn_drv);


    /*Set next output addr ---------start----------*/
    if(s->state == ISP_STATE_CAPTURE_SCALE)
    {
        dst_buf_id = 0;
    }
    else
    {
        dst_buf_id = s->total_slice_count % ISP_PATH_FRAME_COUNT_MAX;
    }

    //ISP_SERVICE_LOG:"ISP_SERVICE:_ISP_ServiceContinueScaling,encoder_addr {0x%x, 0x%x} ,total_slice_count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7200_112_2_18_1_44_9_498,(uint8*)"ddd",s->encoder_addr[dst_buf_id].yaddr,s->encoder_addr[dst_buf_id].uaddr,s->total_slice_count);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                    ISP_PATH_OUTPUT_ADDR,
                                    (void*)&s->encoder_addr[dst_buf_id]);
    ISP_RTN_IF_ERR(rtn_drv);
    /*Set next output addr ---------end------------*/

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_SLICE_SCALE_EN,
                                   (void*)SCI_NULL);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Set next slice height ---------start----------*/
    if(SCI_FALSE == s->is_slice_alone)
    {
        if(s->total_slice_count == s->input_range.h / s->slice_height)
        {
            s->slice_height = s->input_range.h - s->total_slice_count * s->slice_height;
            //ISP_SERVICE_LOG:"ISP_SERVICE:_ISP_ServiceContinueScaling,slice_height %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7219_112_2_18_1_44_9_499,(uint8*)"d",s->slice_height);
        }
    }

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_SLICE_SCALE_HEIGHT,
                                   (void*)&s->slice_height);
    ISP_RTN_IF_ERR(rtn_drv);
    /*Set next slice height ---------end------------*/

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_SWAP_BUFF,
                                   (void*)&s->swap_buff);
    ISP_RTN_IF_ERR(rtn_drv);

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_LINE_BUFF,
                                   (void*)&s->line_buff);
    ISP_RTN_IF_ERR(rtn_drv);

    if(s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565_EX ||
       s->path_output_image_format == ISP_IMAGE_FORMAT_RGB565)
    {
        param = SCI_TRUE;
        rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                       ISP_PATH_DITHER_EN,
                                       (void*)&param);
        ISP_RTN_IF_ERR(rtn_drv);

        param = ISP_IMAGE_FORMAT_RGB565;
    }
    else
    {
        param = (uint32)s->path_output_image_format;
    }

    rtn_drv = ISP_DriverPathConfig(s->module_addr,
                                   ISP_PATH_OUTPUT_FORMAT,
                                   (void*)&param);
    ISP_RTN_IF_ERR(rtn_drv);

    /*Register ISR callback*/

    rtn_drv = ISP_DriverNoticeRegister(s->module_addr,
                                       ISP_IRQ_NOTICE_TX_DONE,
                                       _ISP_ServiceOnTxDone);
    ISP_RTN_IF_ERR(rtn_drv);

    _ISP_ServiceSyncImageData(&next_addr,
                              s->input_size.w,
                              s->input_range.h,
                              s->path_input_image_format,
                              DMABUFFER_TO_DEVICE);

    s->state = ISP_STATE_SCALE_SLICE;
    rtn_drv = ISP_DriverStart(s->module_addr);

exit:

    if(rtn_drv)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE:driver error code 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7279_112_2_18_1_44_9_500,(uint8*)"d",rtn_drv);
        s->state = ISP_STATE_STOP;
        s->err_code = rtn_drv;
    }

    return ;

}

LOCAL void _ISP_ServiceStartVTEncode(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartVTEncode, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7294_112_2_18_1_44_9_501,(uint8*)"d", SCI_GetTickCount());

    _ISP_ServiceEncodeFrameInit();

    _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);

    s->enc_stoped = 0;
    s->state = ISP_STATE_VP;
    s->service = ISP_SERVICE_VP;
    return ;
}

LOCAL void _ISP_ServiceVTHandler(ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             dst_frame = {0};
    uint32                  status = 0;
    ISP_ADDRESS_T           src_addr = {0};
    ISP_ADDRESS_T           dst_addr = {0};
    ISP_SIZE_T              isp_size = {0};
    ISP_RECT_T              trim_rect = {0};
    uint32                  rot_angle = ROTATION_DIR_MAX;
    uint32                  frm_cnt = 0;
    int32                   cb_rtn = ISP_CB_PROC_DONE;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceVTHandler, p_frame->type 0x%x , time %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7320_112_2_18_1_44_9_502,(uint8*)"dd",p_frame->type, SCI_GetTickCount());

    if(s->vt_stop_arrived)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: VP stopped"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7324_112_2_18_1_44_9_503,(uint8*)"");
        return;
    }

    switch(p_frame->flags)
    {
        case ISP_STATE_VP_CAPTURE:

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceVTHandler, Got one captured frame, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7333_112_2_18_1_44_9_504,(uint8*)"d", SCI_GetTickCount());

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

            SCI_MEMCPY((void*)&s->local_frame, (void*)p_frame, sizeof(ISP_FRAME_T));

            if(s->enc_rotation == ISP_ROTATION_0 &&
               ISP_DriverFrameIsLocked(s->p_frame_enc_cur))
            {
                if(s->local_review_enable)
                {
                    dst_frame.type = s->display_block.img_fmt;
                    dst_frame.width = s->display_range.w;
                    dst_frame.height = s->display_range.h;
                    dst_frame.flags = ISP_STATE_VP_REVIEW_LOCAL;
                    frm_cnt = s->vt_local_display_count % ISP_PATH_FRAME_COUNT_MAX;
                    dst_frame.yaddr = s->display_addr[frm_cnt].yaddr;
                    dst_frame.uaddr = s->display_addr[frm_cnt].uaddr;
                    isp_size.w = dst_frame.width;
                    isp_size.h = dst_frame.height;
                    _ISP_ServiceGetTrimRect(&s->cap_output_size,&isp_size,&trim_rect);
                    _ISP_ServiceInternScale(&s->local_frame, &trim_rect, &dst_frame);
                    s->vt_local_display_count ++;  //set the next review frame
                }
                else
                {
                    s->preview_skip_frame_num = 1;
                    _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);

                }
            }
            else
            {
                dst_frame.type = ISP_DATA_YUV420;
                dst_frame.width = s->encoder_size.w;
                dst_frame.height = s->encoder_size.h;
                dst_frame.flags = ISP_STATE_VP_SCALE_LOCAL;
                if(s->enc_rotation != ISP_ROTATION_0)
                {
                    dst_frame.yaddr = s->encoder_rot_addr[0].yaddr;
                    dst_frame.uaddr = s->encoder_rot_addr[0].uaddr;
                }
                else
                {
                    dst_frame.yaddr = s->p_frame_enc_cur->yaddr;
                    dst_frame.uaddr = s->p_frame_enc_cur->uaddr;
                }
                _ISP_ServiceInternScale(p_frame, &s->input_range, &dst_frame);
            }

            break;

        case ISP_STATE_VP_SCALE_LOCAL:

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceVTHandler, Got one uplink frame, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7397_112_2_18_1_44_9_505,(uint8*)"d", SCI_GetTickCount());

            if(!ISP_DriverFrameIsLocked(s->p_frame_enc_cur)) // is the current frame has not been released, drop this encode frame
            {
                if(s->enc_rotation != ISP_ROTATION_0)// need rotation
                {
                    rot_angle = _ISP_ServiceRotAngle2DrvAngle(s->enc_rotation);

                    dst_addr.yaddr = s->p_frame_enc_cur->yaddr;
                    dst_addr.uaddr = s->p_frame_enc_cur->uaddr;
                    src_addr.yaddr = p_frame->yaddr;
                    src_addr.uaddr = p_frame->uaddr;
                    _ISP_ServiceRotateImage(&src_addr,
                                            &dst_addr,
                                            s->encoder_size.w,
                                            s->encoder_size.h,
                                            rot_angle,
                                            s->path_output_image_format);

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

                ISP_DriverFrameLock(s->p_frame_enc_cur);// unlocked by user
                _ISP_ServiceCallUserEncCB((void*)s->p_frame_enc_cur);
                s->p_frame_enc_cur = s->p_frame_enc_cur->next;
            }
            else
            {
                //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceVTHandler, encode frame 0x%x not released!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7437_112_2_18_1_44_9_506,(uint8*)"d",s->p_frame_enc_cur);
            }

            if(s->is_enc_first_frame)
            {
                status = SCI_SetEvent(s_isp_event, ISP_MPEG_START_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
                s->is_enc_first_frame = 0;
            }

            if(s->local_review_enable)
            {
                dst_frame.type = s->display_block.img_fmt;
                dst_frame.width = s->display_range.w;
                dst_frame.height = s->display_range.h;
                dst_frame.flags = ISP_STATE_VP_REVIEW_LOCAL;
                frm_cnt = s->vt_local_display_count % ISP_PATH_FRAME_COUNT_MAX;
                dst_frame.yaddr = s->display_addr[frm_cnt].yaddr;
                dst_frame.uaddr = s->display_addr[frm_cnt].uaddr;
                isp_size.w = dst_frame.width;
                isp_size.h = dst_frame.height;
                _ISP_ServiceGetTrimRect(&s->cap_output_size,&isp_size,&trim_rect);
                _ISP_ServiceInternScale(&s->local_frame, &trim_rect, &dst_frame);
                s->vt_local_display_count ++;  //set the next review frame
            }
            else
            {
                s->preview_skip_frame_num = 1;
                _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);

            }

            break;

        case ISP_STATE_VP_REVIEW_LOCAL:

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceVTHandler, Got one local review frame, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7474_112_2_18_1_44_9_507,(uint8*)"d", SCI_GetTickCount());

            cb_rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);
            if(s->local_display_enable)
            {
                if(ISP_CB_PROC_DONE == cb_rtn)
                {
                    _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
                }
                else
                {
                    s->disp_blk_id = s->display_block.block_id;
                }
            }

            if(s->is_first_frame)
            {
                status = SCI_SetEvent(s_isp_event, ISP_DISPLAY_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
                s->is_first_frame = 0;
            }

            s->preview_skip_frame_num = 1;
            _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);

            break;

        case ISP_STATE_VP_REVIEW_REMOTE:

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceVTHandler, Got one remote review frame, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7504_112_2_18_1_44_9_508,(uint8*)"d", SCI_GetTickCount());
            //ISP_SERVICE_LOG:"ISP_SERVICE: remote display %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7505_112_2_18_1_44_9_509,(uint8*)"d",s->remote_display_enable);
            cb_rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);
            if(s->remote_display_enable)
            {
                //cov 10460
                if(ISP_CB_PROC_DONE == cb_rtn)
                {
                    _ISP_ServiceDisplayOneFrame(s->remote_display_block.block_id,p_frame);
                }
            }
            status = SCI_SetEvent(s_isp_event,ISP_VT_REMOTE_EVENT,SCI_OR);
            SCI_ASSERT(!status); /*assert verified*/

            break;

        default:

            break;

    }

    if(s->err_code)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = 0;
    }

    return;
}


LOCAL void _ISP_ServiceStartVTExtEncode(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_TRACE:"ISP_SERVICE:_ISP_ServiceStartVTExtEncode, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7530_112_2_18_1_44_9_510,(uint8*)"d", SCI_GetTickCount());

    _ISP_ServiceEncodeFrameInit();

    _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);

    s->enc_stoped = 0;
    s->state = ISP_STATE_VP;
    s->service = ISP_SERVICE_VP_EX;
    return ;
}

LOCAL void _ISP_ServiceStartVTExtEncodeLowMem(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  block_id = 0;
    uint32                  rtn = SCI_SUCCESS;

    //ISP_SERVICE_TRACE("ISP:StartVTExtEncodeLowMem, %d", SCI_GetTickCount());

    _ISP_ServiceEncodeFrameInitLowMem();

    rtn = _ISP_ServiceGetVtMemBlockId(&block_id, (uint32)ISP_VT_MEMBLK_IDLE);
    if(!rtn)
    {
        s->local_addr = s->display_addr[block_id];
        ISP_SERVICE_TRACE("ISP:VTExtEncLowMem, set block %d to state %d, y_addr 0x%x u_addr 0x%x",
                                                                            block_id,
                                                                            ISP_VT_MEMBLK_CAPTURING,
                                                                            s->display_addr[block_id].yaddr,
                                                                            s->display_addr[block_id].uaddr
                                                                            );
        ISP_ServiceSetVtMemBlockState(block_id, ISP_VT_MEMBLK_CAPTURING);
         _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);
        s->enc_stoped = 0;
        s->state = ISP_STATE_VP;
        s->service = ISP_SERVICE_VP_EX_LOW_MEM;
        s->err_code = ISP_ERR_CODE_NO_ERR;
    }
    else
    {
        ISP_SERVICE_TRACE("ISP:VTExtEncLowMem error!");
        s->err_code = ISP_ERR_CODE_NO_MEM;
    }

    return ;
}

LOCAL void _ISP_ServiceVTExtHandlerLowMem(ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                        status = 0;
//    ISP_FRAME_T             dst_frame = {0};
//    uint32                       frm_cnt = 0;
    int32                         rtn = SCI_SUCCESS;
    uint32                       block_id_src = 0, block_id_dst = 0, block_id_cap = 0;

    //ISP_SERVICE_TRACE("ISP_SERVICE: _ISP_ServiceVTExtHandlerLowMem, p_frame->type 0x%x , time %d", p_frame->type, SCI_GetTickCount());
    ISP_SERVICE_TRACE("ISP: VTExt, state is 0x%x", s->state);

    switch(p_frame->flags)
    {
        case ISP_STATE_VP_CAPTURE:

            if(s->is_first_frame)
            {
                if(s->b_wait_for_vt_event)
                {
                    status = SCI_SetEvent(s_isp_event, ISP_VT_EVENT, SCI_OR);
                    SCI_ASSERT(!status); /*assert verified*/
                    s->b_wait_for_vt_event = 0;
                }
//              s->is_first_frame = 0;                          //the flag will be clear in first display one frame function
            }

            rtn = _ISP_ServiceGetVtMemBlockId(&block_id_src, ISP_VT_MEMBLK_CAPTURING);
            if(rtn)
            {
                ISP_SERVICE_TRACE("ISP:VTExt error1");
                s->err_code = ISP_ERR_CODE_ST_ERR;
                break;
            }

            if(ISP_STATE_VP_CAPTURE == s->state)
            {
                s->state = ISP_STATE_VP_LOW_MEM_IDLE;
            }

            ISP_ServiceSetVtMemBlockState(block_id_src, ISP_VT_MEMBLK_CAPTURE_DONE);
            ISP_SERVICE_TRACE("ISP:VTExt, block id %d state change from %d to %d",
                                            block_id_src,
                                            ISP_VT_MEMBLK_CAPTURING,
                                            ISP_VT_MEMBLK_CAPTURE_DONE);

            rtn = _ISP_ServiceGetVtMemBlockId(&block_id_dst, ISP_VT_MEMBLK_IDLE);       //get IDLE buffer for callback processing
            if(rtn)
            {
                ISP_SERVICE_TRACE("ISP:VTExt error2");
                s->state = ISP_STATE_VP_LOW_MEM_CB_WAIT;
                break;
            }
            else
            {
                ISP_ServiceSetVtMemBlockState(block_id_src, ISP_VT_MEMBLK_PROCING_SRC);
                ISP_ServiceSetVtMemBlockState(block_id_dst, ISP_VT_MEMBLK_PROCING_DST);
                ISP_SERVICE_TRACE("ISP:VTExt, buffer src block id %d state%d y_addr 0x%x u_addr 0x%x",
                                            block_id_src,
                                             ISP_VT_MEMBLK_PROCING_SRC,
                                             s->display_addr[block_id_src].yaddr,
                                             s->display_addr[block_id_src].uaddr
                                             );
               ISP_SERVICE_TRACE("ISP:VTExt, buffer dst block id %d state%d y_addr 0x%x u_addr 0x%x",
                                             block_id_dst,
                                             ISP_VT_MEMBLK_PROCING_DST,
                                             s->display_addr[block_id_dst].yaddr,
                                             s->display_addr[block_id_dst].uaddr
                                             );
            }

            p_frame->yaddr = s->display_addr[block_id_src].yaddr;
            p_frame->uaddr = s->display_addr[block_id_src].uaddr;
            rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);
            if(ISP_CB_PROC_QUIT == rtn)                         //if cb get data fail then restore the buffer state
            {
                ISP_SERVICE_TRACE("ISP:VTExt,get data fail");
                ISP_ServiceSetVtMemBlockState(block_id_dst, ISP_VT_MEMBLK_IDLE);
                ISP_ServiceSetVtMemBlockState(block_id_src, ISP_VT_MEMBLK_CAPTURE_DONE);
            }
            else
            {
                ISP_SERVICE_TRACE("ISP:VTExt change state1");
                s->state = ISP_STATE_VP_LOW_MEM_IDLE;                  //success then change the isp vp state
            }

            if(s->local_display_enable)
            {
                if(ISP_CB_PROC_DONE == rtn)
                {
                    _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
                }
                else
                {
                    s->disp_blk_id = s->display_block.block_id;
                }
            }

            rtn = _ISP_ServiceGetVtMemBlockId(&block_id_cap, ISP_VT_MEMBLK_IDLE);               //get memory for cotinue capture
            if(rtn)
            {
                ISP_SERVICE_TRACE("get data fail, no block");
                s->state = ISP_STATE_VP_LOW_MEM_IDLE;
                break;
            }
            else
            {
                ISP_ServiceSetVtMemBlockState(block_id_cap, ISP_VT_MEMBLK_CAPTURING);
            }

            s->local_addr.yaddr = s->display_addr[block_id_cap].yaddr;
            s->local_addr.uaddr = s->display_addr[block_id_cap].uaddr;
            s->preview_skip_frame_num = 1;
            s->state = ISP_STATE_VP_CAPTURE;
            ISP_SERVICE_TRACE("ISP:VTExt, buffer block id %d state %d y_addr 0x%x u_addr 0x%x",
                                        block_id_cap,
                                        ISP_VT_MEMBLK_CAPTURING,
                                        s->local_addr.yaddr,
                                        s->local_addr.uaddr);
            _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);

        break;

        case ISP_STATE_VP_LOW_MEM_BUFF_FREE:

            if(ISP_STATE_VP_LOW_MEM_CB_WAIT == s->state)                        //if have data to process, need to process at once
            {
                rtn = _ISP_ServiceGetVtMemBlockId(&block_id_src, ISP_VT_MEMBLK_CAPTURE_DONE);       //get src buffer for callback processing
                if(rtn)
                {
                    ISP_SERVICE_TRACE("ISP:VTExt wait cb state!");
                    s->state = ISP_STATE_VP_LOW_MEM_IDLE;
                    break;
                }

                rtn = _ISP_ServiceGetVtMemBlockId(&block_id_dst, ISP_VT_MEMBLK_IDLE);       //get IDLE buffer for callback processing
                if(rtn)
                {
                    ISP_SERVICE_TRACE("ISP:VTExt, abnormal in buffer free!");
                    break;
                }
                else
                {
                    ISP_ServiceSetVtMemBlockState(block_id_src, ISP_VT_MEMBLK_PROCING_SRC);
                    ISP_ServiceSetVtMemBlockState(block_id_dst, ISP_VT_MEMBLK_PROCING_DST);
                    ISP_SERVICE_TRACE("ISP:VTExt, src block id %d state%d y_addr 0x%x u_addr 0x%x",
                                                 block_id_src,
                                                 ISP_VT_MEMBLK_PROCING_SRC,
                                                 s->display_addr[block_id_src].yaddr,
                                                 s->display_addr[block_id_src].uaddr
                                                 );
                    ISP_SERVICE_TRACE("ISP:VTExt, dst block id %d state%d y_addr 0x%x u_addr 0x%x",
                                                 block_id_dst,
                                                 ISP_VT_MEMBLK_PROCING_DST,
                                                 s->display_addr[block_id_dst].yaddr,
                                                 s->display_addr[block_id_dst].uaddr
                                                 );
                }

                p_frame->yaddr = s->display_addr[block_id_src].yaddr;
                p_frame->uaddr = s->display_addr[block_id_src].uaddr;
                rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);

                if(ISP_CB_PROC_QUIT == rtn)                         //if cb get data fail then restore the buffer state
                {
                    ISP_SERVICE_TRACE("ISP:VTExt,get data fail2");
                    ISP_ServiceSetVtMemBlockState(block_id_dst, ISP_VT_MEMBLK_IDLE);
                    ISP_ServiceSetVtMemBlockState(block_id_src, ISP_VT_MEMBLK_CAPTURE_DONE);
                }
                else
                {
                    ISP_SERVICE_TRACE("ISP:VTExt, change state2");
                    s->state = ISP_STATE_VP_LOW_MEM_IDLE;                  //success then change the isp vp state
                }

                if(s->local_display_enable)
                {
                    if(ISP_CB_PROC_DONE == rtn)
                    {
                        _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
                    }
                    else
                    {
                        s->disp_blk_id = s->display_block.block_id;
                    }
                }

            }

            if(ISP_STATE_VP_LOW_MEM_IDLE == s->state)
            {
                rtn = _ISP_ServiceGetVtMemBlockId(&block_id_cap, ISP_VT_MEMBLK_IDLE);       //get idle src buffer for continue capture
                if(rtn)
                {
                    ISP_SERVICE_TRACE("ISP:VTExt, no block2");
                    s->state = ISP_STATE_VP_LOW_MEM_IDLE;
                    break;
                }
                else
                {
                    ISP_ServiceSetVtMemBlockState(block_id_cap, ISP_VT_MEMBLK_CAPTURING);
                }

                s->local_addr.yaddr = s->display_addr[block_id_cap].yaddr;
                s->local_addr.uaddr = s->display_addr[block_id_cap].uaddr;
                s->preview_skip_frame_num = 1;
                s->state = ISP_STATE_VP_CAPTURE;
                ISP_SERVICE_TRACE("ISP:VTExt,buffer block id %d state %d y_addr 0x%x u_addr 0x%x",
                                            block_id_cap,
                                            ISP_VT_MEMBLK_CAPTURING,
                                            s->local_addr.yaddr,
                                            s->local_addr.uaddr);
                _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);
            }

        break;

        default:
            break;
    }

    if(s->err_code)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = 0;
    }

    ISP_SERVICE_TRACE("ISP:VTExt,over!");

    return;
}

LOCAL void _ISP_ServiceVTExtHandler(ISP_FRAME_T *p_frame)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             dst_frame = {0};
    uint32                  status = 0;
    ISP_ADDRESS_T           dst_addr = {0};
    uint32                  frm_cnt = 0;
    int32                   cb_rtn = ISP_CB_PROC_DONE;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceVTExtHandler, p_frame->type 0x%x , time %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7552_112_2_18_1_44_9_511,(uint8*)"dd",p_frame->type, SCI_GetTickCount());

    if(s->vt_stop_arrived)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: VT extension mode stopped"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7556_112_2_18_1_44_9_512,(uint8*)"");
        return;
    }

    switch(p_frame->flags)
    {
        case ISP_STATE_VP_CAPTURE:

            if(s->is_first_frame)
            {
                if(s->b_wait_for_vt_event)
                {
                    status = SCI_SetEvent(s_isp_event, ISP_VT_EVENT, SCI_OR);
                    SCI_ASSERT(!status); /*assert verified*/
                    s->b_wait_for_vt_event = 0;
                }
//                s->is_first_frame = 0;             //the flag will be clear in first display one frame function
            }

            if(s->b_is_atv&&(s->local_review_enable||s->local_display_enable))
            {
                _ISP_ServiceCallUserPostCapCB(0);
            }

            if(s->local_need_encode_frm)
            {
                if(!ISP_DriverFrameIsLocked(s->p_frame_enc_cur)) // is the current frame has not been released, drop this encode frame
                {
                    //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceVTExtHandler,Frame unlocked"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7579_112_2_18_1_44_9_513,(uint8*)"");

                    SCI_MEMCPY((void*)s->p_frame_enc_cur->yaddr,
                               (void*)p_frame->yaddr,
                               (uint32)(p_frame->width*p_frame->height));

                    SCI_MEMCPY((void*)s->p_frame_enc_cur->uaddr,
                               (void*)p_frame->uaddr,
                               (uint32)(p_frame->width*p_frame->height));

                    ISP_DriverFrameLock(s->p_frame_enc_cur);// unlocked by user
                    _ISP_ServiceCallUserEncCB((void*)s->p_frame_enc_cur);
                    s->p_frame_enc_cur = s->p_frame_enc_cur->next;
                }
                else
                {
                    //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceVTExtHandler, Frame has not been unlocked"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7595_112_2_18_1_44_9_514,(uint8*)"");
                }

                if(s->local_review_enable)
                {
                    dst_frame.type = s->display_block.img_fmt;
                    dst_frame.width = s->display_range.w;
                    dst_frame.height = s->display_range.h;
                    dst_frame.flags = ISP_STATE_VP_REVIEW_LOCAL;
                    frm_cnt = s->vt_local_display_count % ISP_PATH_FRAME_COUNT_MAX;
                    dst_frame.yaddr = s->display_addr[frm_cnt].yaddr;
                    dst_frame.uaddr = s->display_addr[frm_cnt].uaddr;
                    _ISP_ServiceInternScale(p_frame, &s->input_range, &dst_frame);
                    s->vt_local_display_count ++;  //set the next review frame
                }
                else
                {
                    s->preview_skip_frame_num = 1;
                    _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);
                }
            }
            else
            {
                cb_rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);
                if(s->local_display_enable)
                {
                    if(ISP_CB_PROC_DONE == cb_rtn)
                    {
                        _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
                        if(s->b_is_atv)
                        {
                            _ISP_ServiceCallUserPostCapCB(1);
                        }
                    }
                    else
                    {
                        s->disp_blk_id = s->display_block.block_id;
                    }

                }

                s->vt_local_display_count ++;
                frm_cnt = s->vt_local_display_count % ISP_PATH_FRAME_COUNT_MAX;
                s->local_addr.yaddr = s->display_addr[frm_cnt].yaddr;
                s->local_addr.uaddr = s->display_addr[frm_cnt].uaddr;
                s->preview_skip_frame_num = 1;
                _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);
            }


            break;

        case ISP_STATE_VP_REVIEW_LOCAL:

            cb_rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);
            if(s->local_display_enable)
            {
                if(ISP_CB_PROC_DONE == cb_rtn)
                {
                    _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
                    if(s->b_is_atv)
                    {
                        _ISP_ServiceCallUserPostCapCB(1);
                    }
                }
                else
                {
                    s->disp_blk_id = s->display_block.block_id;
                }

            }

            s->preview_skip_frame_num = 1;
            _ISP_ServiceCaptureOnce(ISP_DATA_YUV422, ISP_STATE_VP_CAPTURE);

            break;

        case ISP_STATE_VP_SCALE_REMOTE:

                dst_addr.yaddr = p_frame->yaddr;
                dst_addr.uaddr = p_frame->uaddr;
                dst_addr.vaddr = p_frame->vaddr;
                (*s->scale_user_cb)(dst_addr, p_frame->width, p_frame->height);
                status = SCI_SetEvent(s_isp_event, ISP_SCALE_EVENT, SCI_OR);
                SCI_ASSERT(!status);  /*assert verified*/

            break;

        default:

            break;

    }

    if(s->err_code)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = 0;
    }
    return;
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

    for(i = 0; i < ISP_CAP_DEC_XY_MAX + 1; i++)
    {
        width = *src_width / (uint32)(1 << i);
        height = *src_height / (uint32)(1 << i);

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
    uint32                         dummy = 0;

    REF_ioctl(REF_IOCTL_SET_DC_CAPSOF, (void *)&dummy);

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

    if(ISP_SERVICE_IDLE != s->service && 0 == s->enc_stoped)
    {
        s->enc_stoped = 1;
        if(s->restart_counter < ISP_TRY_TIMES)
        {
            _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,s->service,0);
            s->restart_counter ++;
        }
    }

    //ISP_SERVICE_TRACE:"ISP_SERVICE:Cap fifo overflow, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7787_112_2_18_1_44_10_515,(uint8*)"d",s->service);
}

LOCAL void _ISP_ServiceOnSensorLineErr(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    ISP_DriverSoftReset(AHB_REG_BASE);
    if(ISP_SERVICE_IDLE != s->service && 0 == s->enc_stoped)
    {
        s->enc_stoped = 1;
        if(s->restart_counter < ISP_TRY_TIMES)
        {
            _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,s->service,0);
            s->restart_counter ++;
        }
    }

    //ISP_SERVICE_TRACE:"ISP_SERVICE:Sensor line error, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7804_112_2_18_1_44_10_516,(uint8*)"d",s->service);
}

LOCAL void _ISP_ServiceOnSensorFrameErr(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    ISP_DriverSoftReset(AHB_REG_BASE);
    if(ISP_SERVICE_IDLE != s->service && 0 == s->enc_stoped)
    {
        s->enc_stoped = 1;
        if(s->restart_counter < ISP_TRY_TIMES)
        {
            _ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,s->service,0);
            s->restart_counter ++;
        }
    }

    //ISP_SERVICE_TRACE:"ISP_SERVICE:Sensor Frame error, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7821_112_2_18_1_44_10_517,(uint8*)"d",s->service);
}

LOCAL void _ISP_ServiceOnSensorHardwareErr(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //camera hardware error, so stop the isp service
    ISP_DriverStop(s->module_addr);

    // ISP_DriverSoftReset(AHB_REG_BASE);
    if(s->b_drv_init_ed)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->b_drv_init_ed = 0;
    }

    ISP_SERVICE_TRACE("ISP:Hw Error, service = %d", s->service);

    s->state = ISP_STATE_STOP;
    s->err_code = ISP_ERR_CODE_HW_ERR;

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7834_112_2_18_1_44_10_518,(uint8*)"d",s->service);

}

LOCAL void _ISP_ServiceOnTxDone(void *p)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_frame = SCI_NULL;

    //ISP_SERVICE_TRACE("ISP_SERVICE:_ISP_ServiceOnTxDone, service = %d, state = %d", s->service, s->state);

    if(ISP_SERVICE_IDLE != s->service && ISP_STATE_STOP != s->state)
    {
        if(ISP_SERVICE_PREVIEW == s->service)
        {
            //if(SCI_TRUE == _ISP_GetDSPInfor())
            {
                _ISP_ServiceSendRequest(0,ISP_SERVICE_PATH_SIG,0,p);
            }
        }
        else
        {
            #if 0
            p_frame = (ISP_FRAME_T*)ISP_SERVICE_ALLOC(sizeof(ISP_FRAME_T));
            SCI_MEMCPY((void*)p_frame,
                       (void*)p,
                       sizeof(ISP_FRAME_T));
            ISP_SERVICE_TRACE("ISP_SERVICE:_ISP_ServiceOnTxDone, frame type  = %d, size w%d h%d, y address 0x%x",
                                        p_frame->type, p_frame->width, p_frame->height, p_frame->yaddr);
            _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,ISP_SERVICE_PATH_SIG,0,(void*)p_frame);
            #else

            p_frame = (ISP_FRAME_T*)p;
            ISP_SERVICE_TRACE("ISP:TxDone, frame type  = %d, size w%d h%d, y address 0x%x",
                                                p_frame->type, p_frame->width, p_frame->height, p_frame->yaddr);
            _ISP_ServiceSendRequest(0,ISP_SERVICE_PATH_SIG,0,(void*)p);

            #endif

        }
    }
}

LOCAL uint32  _ISP_ServiceGetDefaultMemSize(void)
{
    uint32                  mem_size = 0;
    uint32                  frm_num = 1;
    ISP_SERVICE_T           *s = &s_isp_service;
    LCD_INFO_T              lcd_info = {0};

    LCD_GetInfo(LCD_ID_0, &lcd_info);

    switch(s->service)
    {
        case ISP_SERVICE_SCALE:
            if(s->is_slice)
            {
                if(0 == s->line_buff.yaddr)
                {
                    mem_size += ISP_SCALE_LINE_BUF_SIZE(s->encoder_size.w)+ISP_ALIGNED_BYTES;
                }

                if(0 == s->swap_buff.yaddr)
                {
                    mem_size += (s->encoder_size.w * (s->input_range.y + s->slice_height) + ISP_MEM_STRIDER_FOR_ONE_FRAME + ISP_ALIGNED_BYTES)<<1;
                }
            }

            if(SCI_FALSE == s->is_slice_alone)
            {
                if(s->is_slice)
                {
                    mem_size += (s->encoder_size.w * s->slice_out_height + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;
                }
                else
                {
                    if(0 == s->output_addr.yaddr)
                    {
                        mem_size += (s->encoder_size.w * s->encoder_size.h + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;
                    }
                }
            }
        break;

        default:

            mem_size = (uint32)(lcd_info.lcd_width * lcd_info.lcd_height);
            if(ISP_DISPLAY_NONE == s->display_block.block_id)
            {
                mem_size = (uint32)((mem_size+ISP_MEM_STRIDER_FOR_ONE_FRAME)<<1);
            }
            else
            {
                mem_size = (uint32)((mem_size+ISP_MEM_STRIDER_FOR_ONE_FRAME)*3)>>1;
                frm_num += 1;
            }

            mem_size = (uint32)(mem_size * frm_num);
        break;
    }


    //ISP_SERVICE_TRACE:"_ISP_ServiceGetDefaultMemSize, lcd {%d %d} mem_size 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7882_112_2_18_1_44_10_519,(uint8*)"ddd",lcd_info.lcd_width,lcd_info.lcd_height,mem_size);

    return mem_size;
}

LOCAL BOOLEAN _ISP_ServiceAllocFrame(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  i = 0;
    uint32                  frame_size = 0,frame_size_disp = 0;
    uint32                  path1_frame_size = 0;
    uint32                  path2_frame_size = 0;
    uint32                  base_addr = 0;
    uint32                  end_addr = 0;
    uint32                  mem_length = 0;
    uint32                  frame_count = 0;
    uint32                  disp_num = 0;
    uint32                  enc_num = 0;
    uint32                  rot_num = 0;
    BOOLEAN                 rtn = SCI_TRUE;
    void                    *p_mem_to_be_freed = SCI_NULL;

    base_addr = (uint32)ISP_ServiceGetMem(&mem_length);
    if(0 == base_addr)
    {
        //ISP_SERVICE_LOG:"_ISP_ServiceAllocFrame, Mem not given by caller, should alloc-ed here"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7907_112_2_18_1_44_10_520,(uint8*)"");

        if(s->mem_ctrl)
        {
            SCI_GetMutex(s->mem_ctrl, SCI_WAIT_FOREVER);
        }

        SCI_TRACE_LOW("hch %s camera memAvalid=%d", __FUNCTION__,SCI_GetByteHeapAvailableSpace(DYNAMIC_HEAP_APP_MEMORY) );

        mem_length = _ISP_ServiceGetDefaultMemSize();//ISP_WORK_MEM_SIZE_DEFAULT;

        p_mem_to_be_freed = (void*)ISP_SERVICE_ALLOC(mem_length);
        if(SCI_NULL == p_mem_to_be_freed)
        {
            //ISP_SERVICE_TRACE:"_ISP_ServiceAllocFrame, NO memory reserved for ISP, 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7924_112_2_18_1_44_10_522,(uint8*)"d",mem_length);
            return SCI_FALSE;
        }
        s->p_work_mem = p_mem_to_be_freed;
        s->work_mem_len = mem_length;
        end_addr = (uint32)p_mem_to_be_freed + mem_length;
        s->mem_alloced_by_myself = SCI_TRUE;
        base_addr = (uint32)s->p_work_mem;
        mem_length = s->work_mem_len;
        s->p_work_mem_cur = s->p_work_mem;
        s->work_mem_res_len = s->work_mem_len;

        if(s->mem_ctrl)
        {
            SCI_PutMutex(s->mem_ctrl);
        }
    }
    else
    {
        //ISP_SERVICE_LOG:"_ISP_ServiceAllocFrame, Mem given by caller, start 0x%x ,length 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_7946_112_2_18_1_44_10_523,(uint8*)"dd",base_addr,mem_length);
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
            disp_num++;
            if(s->display_rotation != ISP_ROTATION_0) //need one rotation buffer at least
            {
                end_addr -= (path1_frame_size << 1);
                mem_length -= (path1_frame_size << 1);
                ISP_SET_ADDR(s->rotation_buf_addr[rot_num],end_addr,frame_size);
                rot_num++;
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
                    rot_num++;
                    frame_count -= 1;
                }
            }

            for(i = 0; i < frame_count; i++)
            {
                end_addr -= (path1_frame_size << 1);
                ISP_SET_ADDR(s->display_addr[disp_num],end_addr,frame_size);
                disp_num++;
                if(disp_num >= ISP_PATH_FRAME_COUNT_MAX)
                    break;
            }

            s->disp_frm_round_num = disp_num;
            if(s->disp_frm_round_num < (ISP_PATH_FRAME_COUNT_MAX -1))
            {
                for(i = disp_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                {
                    s->display_addr[i].yaddr = s->display_addr[i % disp_num].yaddr;
                    s->display_addr[i].uaddr = s->display_addr[i % disp_num].uaddr;
                }
                s->disp_frm_round_num = ISP_PATH_FRAME_COUNT_MAX;
            }

            if(s->display_rotation != ISP_ROTATION_0)
            {
                SCI_ASSERT(rot_num);/*assert verified*/
                for(i = rot_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                {
                    s->rotation_buf_addr[i].yaddr = s->rotation_buf_addr[i % rot_num].yaddr;
                    s->rotation_buf_addr[i].uaddr = s->rotation_buf_addr[i % rot_num].uaddr;
                }
            }


        break;

        case ISP_SERVICE_JPEG:
            if(DCAMERA_IsSupport2MSnapshot() &&
               (s->cap_input_range.w > ISP_SERVICE_VGA_WIDTH) &&
               (s->encoder_size.w > ISP_SERVICE_VGA_WIDTH))
            {
                frame_size = (s->encoder_size.w * s->encoder_size.h);

                if((s->work_mem_y_len < frame_size) || (mem_length < (frame_size>>1)))
                {
                    rtn = SCI_FALSE;
                    break;
                }

                s->encoder_addr[0].yaddr = ISP_ADDRESS_ALIGNED_((uint32)s->p_work_mem_y, ISP_ALIGNED_BYTES);
                s->encoder_addr[0].uaddr = ISP_ADDRESS_ALIGNED_(base_addr, ISP_ALIGNED_BYTES);
            }
            else
            {
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

                    if(ISP_IMAGE_FORMAT_YUV422 == s->path_output_image_format)
                    {
                        mem_length = path1_frame_size << 1;
                    }
                    else
                    {
                        mem_length = (uint32)((path1_frame_size * 3) >> 1);
                    }
                }

                if(s->work_mem_len < mem_length)
                {
                    rtn = SCI_FALSE;
                    break;
                }

                ISP_SET_ADDR(s->encoder_addr[0], base_addr, frame_size);
            }

        break;

        case ISP_SERVICE_MPEG:
            s->disp_frm_round_num = 0;
            frame_size = s->cap_output_size.w * s->cap_output_size.h;
            if(ISP_DATA_YUV420 == s->first_cap_format)
            {
                path1_frame_size = (uint32)((frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) * 3) >> 1;
            }
            else
            {
                path1_frame_size = (frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;
            }
            ISP_SET_ADDR(s->local_addr, base_addr, frame_size); //alloc local frame buffer
            base_addr += path1_frame_size;
            mem_length -= path1_frame_size;

            frame_size = s->encoder_size.w * s->encoder_size.h;
            if(s->path_output_image_format == ISP_IMAGE_FORMAT_YUV420)
            {
                path2_frame_size = ((frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) * 3) >> 1;
            }
            else
            {
                path2_frame_size = (frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;
            }

            if(s->enc_rotation != ISP_ROTATION_0)
            {
                ISP_SET_ADDR(s->encoder_rot_addr[0], base_addr, frame_size); //alloc encode rotation buffer
                base_addr += path2_frame_size;
                mem_length -= path2_frame_size;
            }
            for(i = 1; i < ISP_PATH_FRAME_COUNT_MAX; i++)
            {
                s->encoder_rot_addr[i].yaddr = s->encoder_rot_addr[0].yaddr;
                s->encoder_rot_addr[i].uaddr = s->encoder_rot_addr[0].uaddr;
            }

            frame_size_disp = s->display_range.w * s->display_range.h;
            if(ISP_DATA_YUV420 == s->local_review_format)
            {
                path1_frame_size = (uint32)((frame_size_disp + ISP_MEM_STRIDER_FOR_ONE_FRAME)*3) >> 1;
            }
            else
            {
                path1_frame_size = (frame_size_disp + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;
            }
            if(s->local_review_enable)
            {
                ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size_disp);
                disp_num++;
                base_addr += path1_frame_size;
                mem_length -= path1_frame_size;

                if(s->display_rotation != ISP_ROTATION_0) //need one display rotation buffer at least
                {
                    ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size_disp);
                    rot_num++;
                    base_addr += path1_frame_size;
                    mem_length -= path1_frame_size;
                }
                s->disp_frm_round_num = 1;
            }

            //alloc one encode buffer
            ISP_SET_ADDR(s->encoder_addr[enc_num], base_addr, frame_size);
            enc_num++;
            base_addr += path2_frame_size;
            mem_length -= path2_frame_size;

            if(end_addr < base_addr) // basic buffer needed, otherwise,return error
            {
                rtn = SCI_FALSE;
                break;
            }

            s->enc_frm_round_num = enc_num;

            if(s->local_review_enable)
            {
                frame_count = mem_length / path1_frame_size;

                if(frame_count) /*there still has additional memory, so use up the left memory,first display buffer*/
                {
                    if(s->display_rotation != ISP_ROTATION_0)
                    {
                        ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size_disp);
                        rot_num++;
                    }
                    else
                    {
                        ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size_disp);
                        disp_num++;
                        SCI_TRACE_LOW("disp_num %d",disp_num);
                    }
                    base_addr += path1_frame_size;
                    mem_length -= path1_frame_size;
                    s->disp_frm_round_num ++;
                }

                if(s->display_rotation != ISP_ROTATION_0)
                {
                    SCI_ASSERT(rot_num);/*assert verified*/
                    for(i = rot_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                    {
                        s->rotation_buf_addr[i].yaddr = s->rotation_buf_addr[i % rot_num].yaddr;
                        s->rotation_buf_addr[i].uaddr = s->rotation_buf_addr[i % rot_num].uaddr;
                    }
                }
                SCI_ASSERT(disp_num);/*assert verified*/
                for(i = disp_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                {
                    s->display_addr[i].yaddr = s->display_addr[i % disp_num].yaddr;
                    s->display_addr[i].uaddr = s->display_addr[i % disp_num].uaddr;
                }
            }

            frame_count = mem_length / path2_frame_size;
            if(frame_count > 0)/*then, encode buffer*/
            {
                frame_count = frame_count > (ISP_PATH_FRAME_COUNT_MAX - enc_num) ? (ISP_PATH_FRAME_COUNT_MAX - enc_num) : frame_count;
                for(i = enc_num; i < frame_count + enc_num; i++)
                {
                    ISP_SET_ADDR(s->encoder_addr[i], base_addr, frame_size);
                    base_addr += path2_frame_size;
                    mem_length -= path2_frame_size;
                }
                s->enc_frm_round_num += frame_count;
            }

        break;

        case ISP_SERVICE_VP:

            frame_size = s->cap_output_size.w * s->cap_output_size.h;
            path1_frame_size = (frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;
            ISP_SET_ADDR(s->local_addr, base_addr, frame_size);
            base_addr += path1_frame_size;
            mem_length -= path1_frame_size;

            path1_frame_size = s->display_range.w * s->display_range.h;
            path1_frame_size += ISP_MEM_STRIDER_FOR_ONE_FRAME;

            for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i ++)
            {
                s->display_addr[i].yaddr = base_addr + i * path1_frame_size * 2;
                s->display_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->display_addr[i].yaddr,
                                                                 ISP_ALIGNED_BYTES);

                s->display_addr[i].uaddr = s->display_addr[i].yaddr + path1_frame_size;
                s->display_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->display_addr[i].uaddr,
                                                                 ISP_ALIGNED_BYTES);
                s->display_addr[i].vaddr = 0;

            }
            for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i ++)
            {
                s->rotation_buf_addr[i].yaddr = base_addr + ISP_PATH_FRAME_COUNT_MAX * path1_frame_size * 2;
                s->rotation_buf_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->rotation_buf_addr[i].yaddr,
                                                                      ISP_ALIGNED_BYTES);

                s->rotation_buf_addr[i].uaddr = s->rotation_buf_addr[i].yaddr + path1_frame_size;
                s->rotation_buf_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->rotation_buf_addr[i].uaddr,
                                                                      ISP_ALIGNED_BYTES);
                s->rotation_buf_addr[i].vaddr = 0;
            }

            base_addr += (ISP_PATH_FRAME_COUNT_MAX + 1) * path1_frame_size * 2;

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

            for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i ++)
            {
                s->encoder_addr[i].yaddr = base_addr + i * path2_frame_size * 3 / 2;
                s->encoder_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->encoder_addr[i].yaddr,
                                                                 ISP_ALIGNED_BYTES);

                s->encoder_addr[i].uaddr = s->encoder_addr[i].yaddr +  path2_frame_size;
                s->encoder_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->encoder_addr[i].uaddr,
                                                                 ISP_ALIGNED_BYTES);

                s->encoder_addr[i].vaddr = 0;
            }
            base_addr += ISP_PATH_FRAME_COUNT_MAX * path2_frame_size * 3 / 2;
            s->enc_frm_round_num = ISP_PATH_FRAME_COUNT_MAX;

            s->p_work_mem_cur = (void*)base_addr;
            if(base_addr > (uint32)s->p_work_mem_cur + s->work_mem_res_len)
            {
                rtn = SCI_FALSE;
            }
            else
            {
                s->work_mem_res_len = s->work_mem_res_len - (base_addr - (uint32)s->p_work_mem_cur);
                s->vt_mem_end_cur = base_addr + s->work_mem_res_len;
            }


        break;

        case ISP_SERVICE_VP_EX:

            frame_size = s->encoder_size.w * s->encoder_size.h;
            path1_frame_size = (frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;

            if(s->local_need_encode_frm)
            {
                ISP_SET_ADDR(s->local_addr, base_addr, frame_size);
                base_addr += path1_frame_size;
                mem_length -= path1_frame_size;
                s->enc_frm_round_num = 1;
                for(i = 0; i < s->enc_frm_round_num; i ++)
                {
                    s->encoder_addr[i].yaddr = base_addr + i * path1_frame_size;
                    s->encoder_addr[i].yaddr = ISP_ADDRESS_ALIGNED_(s->encoder_addr[i].yaddr,
                                                                     ISP_ALIGNED_BYTES);

                    s->encoder_addr[i].uaddr = s->encoder_addr[i].yaddr +  frame_size;
                    s->encoder_addr[i].uaddr = ISP_ADDRESS_ALIGNED_(s->encoder_addr[i].uaddr,
                                                                     ISP_ALIGNED_BYTES);

                    s->encoder_addr[i].vaddr = 0;
                }
                base_addr += s->enc_frm_round_num * path1_frame_size;
                mem_length -= s->enc_frm_round_num * path1_frame_size;
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
                for(i = disp_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                {
                    ISP_SET_ADDR(s->display_addr[i],s->display_addr[i % disp_num].yaddr, frame_size);
                }

                if(s->display_rotation != ISP_ROTATION_0)
                {
                    SCI_ASSERT(rot_num);/*assert verified*/
                    for(i = rot_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                    {
                        ISP_SET_ADDR(s->rotation_buf_addr[i],s->rotation_buf_addr[i % rot_num].yaddr, frame_size);
                    }
                }
            }

            if(0 == s->local_need_encode_frm)
            {
                s->enc_frm_round_num = 1;
                for(i = 0; i < s->enc_frm_round_num; i ++)
                {
                    s->encoder_addr[i].yaddr = s->display_addr[i].yaddr;
                    s->encoder_addr[i].uaddr = s->display_addr[i].uaddr;
                    s->encoder_addr[i].vaddr = 0;
                }
                s->local_addr.yaddr = s->encoder_addr[0].yaddr;
                s->local_addr.uaddr = s->encoder_addr[0].uaddr;
            }

            s->p_work_mem_cur = (void*)base_addr;
            s->work_mem_res_len = mem_length;
            s->vt_mem_end_cur = base_addr + s->work_mem_res_len;

            if(s->vt_mem_end_cur > end_addr)
            {
                rtn = SCI_FALSE;
            }

        break;

        case ISP_SERVICE_VP_EX_LOW_MEM:
            frame_size = s->encoder_size.w * s->encoder_size.h;
            path1_frame_size = (frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME) << 1;

            if(s->local_review_enable)
            {
                frame_size = s->display_range.w * s->display_range.h;
                path1_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

                //alloc the round memory of capture and display
                for(i = 0; i < ISP_VT_ROUND_FRAME_NUM; i++)
                {
                    ISP_SET_ADDR(s->display_addr[i], base_addr,frame_size);
                    base_addr += (path1_frame_size << 1);
                    mem_length -= (path1_frame_size << 1);
                    if(end_addr < base_addr)
                    {
                        rtn = SCI_FALSE;
                        break;
                    }
                }

                //need one rotation buffer at least;
                //if have rotation action, then use the rotation buffer, if no rotation function, the copy the buffer to rotation buffer,
                //at last use the rotation buffer to invalidate lcd
                ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                rot_num += 1;
                base_addr += (path1_frame_size << 1);
                mem_length -= (path1_frame_size << 1);

                if(end_addr < base_addr)
                {
                    rtn = SCI_FALSE;
                    break;
                }

                if(s->display_rotation != ISP_ROTATION_0)
                {
                    SCI_ASSERT(rot_num);/*assert verified*/
                    for(i = rot_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                    {
                        ISP_SET_ADDR(s->rotation_buf_addr[i],s->rotation_buf_addr[i % rot_num].yaddr, frame_size);
                    }
                }
            }

            s->p_work_mem_cur = (void*)base_addr;
            s->work_mem_res_len = mem_length;
            s->vt_mem_end_cur = base_addr + s->work_mem_res_len;

            if(s->vt_mem_end_cur > end_addr)
            {
                rtn = SCI_FALSE;
            }

        break;

        case ISP_SERVICE_REVIEW:

            frame_size = s->display_range.w * s->display_range.h;
            path2_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;
            if(ISP_IMAGE_FORMAT_YUV422 == s->path_output_image_format ||
               ISP_IMAGE_FORMAT_RGB565 == s->path_output_image_format)
            {
                path2_frame_size = (path2_frame_size << 1);
            }
            else
            {
                path2_frame_size = (path2_frame_size * 3) >> 1;
            }

            ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size);
            disp_num += 1;
            base_addr += path2_frame_size;
            mem_length -= path2_frame_size;

            if(s->display_rotation != ISP_ROTATION_0) //need one rotation buffer at least
            {
                ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                rot_num += 1;
                base_addr += path2_frame_size;
                mem_length -= path2_frame_size;
            }

            if(end_addr < base_addr)
            {
                rtn = SCI_FALSE;
                break;
            }

            frame_count = mem_length / path2_frame_size;
            if(frame_count > 0)
            {
                if(s->display_rotation != ISP_ROTATION_0)
                {
                    ISP_SET_ADDR(s->rotation_buf_addr[rot_num],base_addr,frame_size);
                    rot_num += 1;
                    base_addr += path2_frame_size;
                    mem_length -= path2_frame_size;
                    frame_count -= 1;
                }


            }
            if(frame_count > 0)
            {
                ISP_SET_ADDR(s->display_addr[disp_num],base_addr,frame_size);
                disp_num += 1;
                base_addr += path2_frame_size;
                mem_length -= path2_frame_size;
            }
            SCI_ASSERT(disp_num);/*assert verified*/
            for(i = disp_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
            {
                ISP_SET_ADDR(s->display_addr[i],s->display_addr[i % disp_num].yaddr, frame_size);
            }

            if(s->display_rotation != ISP_ROTATION_0)
            {
                SCI_ASSERT(rot_num);/*assert verified*/
                for(i = rot_num; i < ISP_PATH_FRAME_COUNT_MAX; i++)
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
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_8461_112_2_18_1_44_11_524,(uint8*)"ddd",s->line_buff.yaddr,s->swap_buff.yaddr,s->swap_buff.uaddr);

                }
            }

            if(s->is_slice_alone) //come from external module
            {
                if(ISP_ADDR_INVALID(s->output_addr.yaddr) ||
                   (s->output_addr.yaddr % ISP_ALIGNED_BYTES != 0))
                {
                    frame_size = s->encoder_size.w * s->slice_out_height;
                    path2_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

                    for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                    {
                        ISP_SET_ADDR(s->encoder_addr[i], base_addr, frame_size);
                    }
                    base_addr += (path2_frame_size << 1);
                }
                else
                {

                    for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                    {
                        s->encoder_addr[i].yaddr = s->output_addr.yaddr;
                        s->encoder_addr[i].uaddr = s->output_addr.uaddr;
                    }
                }
            }
            else
            {
                path1_frame_size = s->input_size.w * s->input_size.h;

                if(s->is_slice) // YUV data to be slice scaled is from internal module,
                {
                    frame_size = s->encoder_size.w * s->slice_out_height;
                    path2_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;
                    for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i++)
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
                        for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                        {
                            s->encoder_addr[i].yaddr = s->output_addr.yaddr;
                            s->encoder_addr[i].uaddr = s->output_addr.uaddr;
                        }
                    }
                    else
                    {
                        frame_size = s->encoder_size.w * s->encoder_size.h;
                        path2_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

                        if(s->input_addr.yaddr > base_addr &&
                          (s->input_addr.uaddr + path1_frame_size) < end_addr) // It's come fromr DV display last frame
                        {
                            if((base_addr + (path2_frame_size << 1)) <= s->input_addr.yaddr)
                            {
                                // do nothing, just set the current base addr as the scale out destination
                            }
                            else if((s->input_addr.uaddr + path1_frame_size + (path2_frame_size << 1)) <= end_addr)
                            {
                                base_addr = s->input_addr.uaddr + path1_frame_size;
                            }
                            else
                            {
                                rtn = SCI_FALSE;
                                break;
                            }
                        }

                        for(i = 0; i < ISP_PATH_FRAME_COUNT_MAX; i++)
                        {
                            ISP_SET_ADDR(s->encoder_addr[i], base_addr, frame_size);
                        }
                        base_addr += (path2_frame_size << 1);

                    }
                }
            }
            if(end_addr < base_addr)
            {
                rtn = SCI_FALSE;
                break;
            }
            s->p_work_mem_cur = (void*)base_addr;
            s->work_mem_res_len = s->work_mem_len - (base_addr - (uint32)s->p_work_mem);

        break;

        case ISP_SERVICE_DENOISE:

            if(s->output_addr.yaddr)
            {
                //ISP_SERVICE_TRACE:"_ISP_ServiceAllocFrame: the denoise target buffer already alloc by caller, y address %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_8540_112_2_18_1_44_11_525,(uint8*)"d", s->output_addr.yaddr);
            }
            else
            {
                frame_size = s->encoder_size.w * s->encoder_size.h;
                path2_frame_size = frame_size + ISP_MEM_STRIDER_FOR_ONE_FRAME;

                path2_frame_size = (path2_frame_size << 1);

                ISP_SET_ADDR(s->output_addr,base_addr,frame_size);
                disp_num += 1;
                base_addr += path2_frame_size;
                mem_length -= path2_frame_size;

                if(end_addr < base_addr)
                {
                    rtn = SCI_FALSE;
                    break;
                }
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
    uint32                  end_addr = (uint32)s->p_work_mem + s->work_mem_len;
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

LOCAL void    _ISP_ServiceSyncImageData(ISP_ADDRESS_T*        p_addr,
                                        uint32                img_width,
                                        uint32                img_height,
                                        uint8                 img_format,
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

LOCAL  void   _ISP_ServiceGetTrimRect(ISP_SIZE_T *p_src_size,
                                      ISP_SIZE_T *p_dst_size,
                                      ISP_RECT_T *p_rect)
{
    if(!p_src_size || !p_dst_size || !p_rect)
        return;

    if(p_src_size->w * p_dst_size->h > p_src_size->h * p_dst_size->w &&
       0 != p_dst_size->h)
    {
        p_rect->w = (uint32)(p_src_size->h * p_dst_size->w / p_dst_size->h);
        p_rect->w = ISP_PIXEL_LEFT_ALIGNED_(p_rect->w,ISP_ALIGNED_PIXELS);
        p_rect->x = (p_src_size->w - p_rect->w) >> 1;
        p_rect->x = p_rect->x & (~1);
        p_rect->y = 0;
        p_rect->h = p_src_size->h;
    }
    else
    {
        p_rect->h = (uint32)(p_src_size->w * p_dst_size->h / p_dst_size->w);
        p_rect->h = ISP_PIXEL_LEFT_ALIGNED_(p_rect->h,ISP_ALIGNED_PIXELS);
        p_rect->y = (p_src_size->h - p_rect->h) >> 1;
        p_rect->y = p_rect->y & (~1);
        p_rect->x = 0;
        p_rect->w = p_src_size->w;

    }
}

PUBLIC uint32 ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ID_E eid, int (*func)(void *))
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(SCI_NULL == func)
    {

    }

    SCI_TRACE_LOW("ISP:Register eid=%d func=0x%x", eid, func);
    switch(eid)
    {
        case ISP_SERVICE_CALLBACK_ENCODE:

            _ISP_ServiceGetEncCtrl();
            s->encode_user_cb = func;
            _ISP_ServiceRelEncCtrl();

        break;

        case ISP_SERVICE_CALLBACK_DISPLAY:

            _ISP_ServiceGetDispCtrl();
            s->display_user_cb = func;
            _ISP_ServiceRelDispCtrl();

        break;

        case ISP_SERVICE_CALLBACK_POSTCAP:

            _ISP_ServiceGetPostCapCtrl();
            s->post_cap_user_cb = func;
            _ISP_ServiceRelPostCapCtrl();

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

    SCI_TRACE_LOW("ISP:UnRegister eid=%d", eid);
    switch(eid)
    {
        case ISP_SERVICE_CALLBACK_ENCODE:

            _ISP_ServiceGetEncCtrl();
            s->encode_user_cb = SCI_NULL;
            _ISP_ServiceRelEncCtrl();

        break;

        case ISP_SERVICE_CALLBACK_DISPLAY:

            _ISP_ServiceGetDispCtrl();
            s->display_user_cb = SCI_NULL;
            _ISP_ServiceRelDispCtrl();

        break;

        case ISP_SERVICE_CALLBACK_POSTCAP:

            _ISP_ServiceGetPostCapCtrl();
            s->post_cap_user_cb = SCI_NULL;
            _ISP_ServiceRelPostCapCtrl();

        break;

        default:

            status = SCI_ERROR;

        break;
    }

    return status;
}

PUBLIC BOOLEAN ISP_ServiceIsVTOn(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    //ISP_SERVICE_LOG:"ISP_SERVICE:ISP_ServiceIsVTOn, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_8763_112_2_18_1_44_12_526,(uint8*)"d", s->service);

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
    //uint32                  output_align_value=1;
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

    return (scaling_shift + subsample_shift) <  2 ? 2 : (scaling_shift + subsample_shift);
}


/****************************************************************************************/
// Description: Get isp input width align value
// Global resource dependence:
// Author: Tim.zhu
// input parameter:
//                      input width
//                      output width
//                      data format
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
//                      output hieht
//                      data format
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
PUBLIC uint32 ISP_ServiceGetOutputWidthAlign(ISP_DATA_FORMAT_E data_format, BOOLEAN is_slice)
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
        {
            output_align_value = is_slice?8:4;
            break;
        }
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_8975_112_2_18_1_44_12_527,(uint8*)"");

    slice_height_aligned = ISP_ServiceGetScaleSliceHeightAlign(param_ptr->input_rect.w,
                                                               param_ptr->output_size.w,
                                                               param_ptr->input_rect.h,
                                                               param_ptr->output_size.h);

    if((param_ptr->input_rect.w << 10) > param_ptr->output_size.w * (ISP_PATH_SCALE_LEVEL << 10))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported scaling down size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_8984_112_2_18_1_44_12_528,(uint8*)"");
        return SCI_FALSE;
    }

    if((param_ptr->input_rect.h << 10) > param_ptr->output_size.h * (ISP_PATH_SCALE_LEVEL << 10))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported scaling down size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_8990_112_2_18_1_44_12_529,(uint8*)"");
        return SCI_FALSE;
    }

    if((param_ptr->input_rect.w*ISP_PATH_SC_COEFF_MAX < param_ptr->output_size.w)||
       (param_ptr->input_rect.h*ISP_PATH_SC_COEFF_MAX < param_ptr->output_size.h))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported scaling up size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_8997_112_2_18_1_44_12_530,(uint8*)"");
        return SCI_FALSE;
    }

    if((ISP_SERVICE_SCALE_SLICE == param_ptr->scale_mode) && (!param_ptr->is_last)
        && (0 != (param_ptr->slice_line & ((1 << slice_height_aligned) - 1))))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: slice height is not aligned, %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9004_112_2_18_1_44_12_531,(uint8*)"d", param_ptr->slice_line);
        return SCI_FALSE;
    }

    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
       (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: trim rect error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9011_112_2_18_1_44_12_532,(uint8*)"");
        return SCI_FALSE;
    }

    if((param_ptr->input_rect.x % ISP_ALIGNED_PIXELS)||(param_ptr->input_rect.w % ISP_ALIGNED_PIXELS))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: trim rect not aligned by 4 pixels"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9017_112_2_18_1_44_12_533,(uint8*)"");
        return SCI_FALSE;
    }

    if(param_ptr->output_size.w % ISP_ALIGNED_PIXELS)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: the width of output_size not aligned by 4 pixels"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9023_112_2_18_1_44_12_534,(uint8*)"");
        return SCI_FALSE;
    }

    if((ISP_SCALE_FRAME_MODE_WIDTH_TH < param_ptr->output_size.w)&&(ISP_SERVICE_SCALE_SLICE!=param_ptr->scale_mode))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: unsupported scaling mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9029_112_2_18_1_44_12_535,(uint8*)"");
        return SCI_FALSE;
    }

    if((ISP_SERVICE_SCALE_SLICE==param_ptr->scale_mode)&&((PNULL==param_ptr->get_scale_data)))
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: get_scale_data is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9035_112_2_18_1_44_12_536,(uint8*)"");
        return SCI_FALSE;
    }


    if(NULL==param_ptr->input_addr.yaddr)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: the address of surce data is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9042_112_2_18_1_44_12_537,(uint8*)"");
        return SCI_FALSE;
    }

    return SCI_TRUE;
}


PUBLIC void ISP_ServiceMemConfig(void* p_isp_mem, uint32 mem_length)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(SCI_NULL == p_isp_mem ||
       0 == mem_length)
    {
        //ISP_SERVICE_TRACE:"ISP_ServiceMemConfig, mem_ptr 0x%x, mem len 0x%x ,isp_status %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9060_112_2_18_1_44_12_538,(uint8*)"ddd",(uint32)p_isp_mem,mem_length,s->state);
        return ;
    }

    //ISP_SERVICE_LOG:"ISP_ServiceMemConfig, mem_ptr 0x%x, mem len 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9066_112_2_18_1_44_12_539,(uint8*)"dd",(uint32)p_isp_mem,mem_length);

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

PUBLIC void ISP_ServiceMemConfig_YBuffer(void* p_isp_mem, uint32 mem_length)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    if(SCI_NULL == p_isp_mem || 0 == mem_length)
    {
        ISP_SERVICE_TRACE("ISP:YBuffer, mem_ptr 0x%x, mem len 0x%x",(uint32)p_isp_mem,mem_length);
        return ;
    }

    ISP_SERVICE_TRACE("ISP:MemConfig_YBuffer, mem_ptr 0x%x, mem len 0x%x",(uint32)p_isp_mem,mem_length);

    s->p_work_mem_y = p_isp_mem;
    s->work_mem_y_len = mem_length;

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

LOCAL uint32 _ISP_ServiceFormatconvertor(ISP_IMAGE_FORMAT format)
{
    uint32 type = ISP_IMAGE_FORMAT_YUV422;
    switch(format)
    {
    case ISP_IMAGE_FORMAT_YUV422:
        type = ISP_DATA_YUV422;
    break;

    case ISP_IMAGE_FORMAT_RGB565:
        type = ISP_DATA_RGB565;
    break;

    default:
        type = ISP_DATA_RGB565;
    break;
    }
    return type;
}
void ISP_ServiceDisplayOneFrame(ISP_DISPLAY_PARAM_T *param_ptr)
{

   ISP_FRAME_T frame = {0};

    frame.width = param_ptr->input_size.w;
    frame.height = param_ptr->input_size.h;
    frame.yaddr = param_ptr->input_addr.yaddr;
    frame.uaddr = param_ptr->input_addr.uaddr;
    frame.vaddr = param_ptr->input_addr.vaddr;
    frame.type = _ISP_ServiceFormatconvertor(param_ptr->input_format);

    ISP_ServiceAsyncDisplay(&frame);
}

void ISP_ServiceAsyncDisplay(ISP_FRAME_T *frame_ptr)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             *p_frame = SCI_NULL;

    //ISP_SERVICE_LOG:"ISP_ServiceAsyncDisplay, 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9151_112_2_18_1_44_12_540,(uint8*)"d",frame_ptr);

    if(ISP_STATE_STOP == s->state || ISP_SERVICE_IDLE == s->service)
    {
        //ISP_SERVICE_TRACE:"ISP_ServiceAsyncDisplay, Wrong status or service %d %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9157_112_2_18_1_44_12_541,(uint8*)"dd",s->state,s->service);
        return;
    }

    p_frame = (ISP_FRAME_T*)ISP_SERVICE_ALLOC(sizeof(ISP_FRAME_T));
    if(SCI_NULL == frame_ptr || SCI_NULL == p_frame)
    {
        //ISP_SERVICE_TRACE:"ISP_ServiceAsyncDisplay, frame_ptr 0x%x p_frame 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_9166_112_2_18_1_44_12_542,(uint8*)"dd",    frame_ptr,p_frame);
        return;
    }

    SCI_MEMCPY((void*)p_frame,
               (void*)frame_ptr,
               sizeof(ISP_FRAME_T));
    _ISP_ServiceSendRequest(ISP_SERVICE_PARA_FREE,
                            ISP_SERVICE_DISP_SIG,
                            s->disp_blk_id,
                            (void*)p_frame);
    return;
}

uint8 ISP_ServiceGetLastErr(void)
{
    ISP_SERVICE_T           *s = &s_isp_service;

    return s->err_code;
}



// W217
//-------------------------------------------------------------------------------------

void ISP_Uix_LinkEncFrames(void)
{
    uint32                    i = 0;
    ISP_SERVICE_T             *s   = &s_isp_service;
    ISP_FRAME_T               *p_enc_frame = &s_enc_frame[0];

    for(i = 0; i < 2; i++)
    {
        ISP_DATA_CLEAR(p_enc_frame + i);
        (p_enc_frame+i)->next = p_enc_frame + (i + 1) % 2;
        (p_enc_frame+i)->prev = p_enc_frame + (i -1 + 2) % 2;
    }

    s->uix_frame_enc_cur  = p_enc_frame;
    s->uix_frame_enc_head = p_enc_frame;

    SCI_TRACE_LOW("ISP:LinkEncFrames head=0x%x", s->uix_frame_enc_head);

}

int32 ISP_Uix_setEncAddr(uint32 addr)
{
    ISP_SERVICE_T             *s   = &s_isp_service;

    //SCI_TRACE_LOW("david ISP_Uix_setEncAddr");

    s->uix_frame_enc_cur->yaddr = addr;
    s->uix_frame_enc_cur->uaddr = 0;
    s->uix_frame_enc_cur->vaddr = 0;
    s->uix_frame_enc_cur = s->uix_frame_enc_cur->next;

    SCI_TRACE_LOW("ISP:setEncAddr yaddr:0x%x", addr);

    return 0;
}



//320*240 YUV422
int ISP_Uix_AllocEncFrame()
{
    ISP_SERVICE_T           *s   = &s_isp_service;
    uint32                  size = 0;
    uint32                  addr = 0;
    uint32                  end  = 0;


    size =  p_Dcam_cfg[DCAMERA_MODE_RECORD]->width *
            p_Dcam_cfg[DCAMERA_MODE_RECORD]->height * 2;

    ISP_Uix_LinkEncFrames();

    addr = s->uix_base_addr + s->uix_display_size;
    //if(s->uix_display_size+YUV422_320_240_SIZE+YUV_PAD_SIZE < s->uix_mem_size)
    {
        ISP_Uix_setEncAddr(addr);
    }

    addr += (size+YUV_PAD_SIZE);

    //if(s->uix_display_size+YUV422_320_240_SIZE+YUV_PAD_SIZE < s->uix_mem_size)
    {
        ISP_Uix_setEncAddr(addr);
    }

    end = addr + size + YUV_PAD_SIZE;

    SCI_TRACE_LOW("ISP:AllocEncFrame. ok Endaddr=0x%x, totalusesize=%d(k)",end, (end-s->uix_base_addr)/1024);

    return 0;
}

int ISP_Uix_AllocFrameAddr()
{
    int32                   rtn_drv = 0;
    uint32                   addr    = 0;
    ISP_SERVICE_T           *s = &s_isp_service;
    uint32                  size = 0;

    addr = s->uix_base_addr;

    #if 0
    if(!g_Capturemode)
        size = YUV422_320_240_SIZE+YUV_PAD_SIZE;
    else
        size = YUV422_640_480_SIZE+YUV_PAD_SIZE;
    #endif

    size = p_Dcam_cfg[DCAMERA_MODE_RECORD]->width *
                            p_Dcam_cfg[DCAMERA_MODE_RECORD]->height * 2 + YUV_PAD_SIZE;

    if(addr != 0 && (addr - s->uix_base_addr) < s->uix_mem_size)
    {
        rtn_drv = hal_camera_setOutputAddr(addr);
        //ISP_RTN_IF_ERR(rtn_drv);
        addr += size;
    }

    if(addr != 0 && (addr - s->uix_base_addr) < s->uix_mem_size)
    {
        rtn_drv = hal_camera_setOutputAddr(addr);
        //ISP_RTN_IF_ERR(rtn_drv);
    }

    s->uix_display_size = size*2;

    SCI_TRACE_LOW("ISP:AllocFrameAddr. dvmod=%d, dispsize=%d baseaddr=0x%x",g_DcamMode, s->uix_display_size, s->uix_base_addr);

    return 0;
}

LOCAL void _ISP_Uix_ServiceGetDataCtrl(void)
{
    #if 0
    if( SCI_NULL != g_CamDataMutex )
    {
        SCI_GetMutex(g_CamDataMutex, SCI_WAIT_FOREVER);
    }
    #endif
    return;
}

LOCAL void _ISP_Uix_ServiceRelDataCtrl(void)
{
    #if 0
    if( SCI_NULL != g_CamDataMutex )
    {
        SCI_PutMutex( g_CamDataMutex );
    }
    #endif
    return;
}

static uint32 s_lastime = 0;
static uint32 s_curtime = 0;
static uint32 s_lastProcessTime = 0;

LOCAL void Cam_preview_start()
{
    uint32 time = 0;

    hal_camera_disable_spimaster();
    hal_camera_clear_fifo();// TODO: checking: clear buffer here ???
    hal_camera_disable_scl();
    hal_camera_enable_spimaster();

    hal_camera_mask_irq(CAM_IRQ_MASK);
    time = SCI_GetTickCount();
    g_pFrame = hal_camera_getCurFrame();

    //SCI_TRACE_LOW("david PREVIEW 0 hal_CameraStartXfer g_pFrame:0x%x dv:%d tick:%u, deltatime : %u",
    //                                      g_pFrame, g_Capturemode, time, time-s_lastProcessTime);

    if(g_pFrame->yaddr == NULL || g_pFrame == NULL)
    {
        SCI_TRACE_LOW("ISP:PRE yaddr NULL!!!");
        return;
    }
    if(p_Dcam_cfg[g_DcamMode]->hal_cfg.scaleEnable)
        hal_camera_enable_scl();//hechang20201103 8w sensor

    //SCI_TRACE_LOW("david PREVIEW 0 startnext tick:%u  status:0x%x scl:0x%x frame:0x%x, dvmode:%d",
    //                 OS_GetTickCount(), hal_camera_get_status(), hal_camera_get_scl(),
    //                                   g_pFrame, g_Capturemode);

    hal_CameraStartXfer(p_Dcam_cfg[g_DcamMode]->width * p_Dcam_cfg[g_DcamMode]->height * 2,
                            (uint8 *)g_pFrame->yaddr);


    gIsCameraStartXfer = TRUE;

//    SCI_TRACE_LOW("david PREVIEW 0 over g_pFrame:0x%x dv:%d tick:%u",
//                           g_pFrame, g_Capturemode, SCI_GetTickCount());

    g_ispmode = CAM_PREVIEW_STAGE_PROCESS;
    hal_camera_unmask_irq(CAM_IRQ_FEND);
}


LOCAL void Cam_preview_process()
{
    HAL_CAMERA_IRQ_CAUSE_T mask;
    int res=0;
    uint32 time = 0;
    memset(&mask, 0, sizeof(HAL_CAMERA_IRQ_CAUSE_T));

    time = OS_GetTickCount();
    SCI_TRACE_LOW("ISP: PRE 1 enter tick:%u deltatime : %u", time, time-s_curtime);

    //ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½
    hal_camera_mask_irq(CAM_IRQ_MASK);
    hal_camera_disable_spimaster();
    hal_camera_clear_fifo();
    hal_camera_disable_scl();
    hal_camera_enable_spimaster();

    if (gIsCameraStartXfer == TRUE)
    {
        gIsCameraStartXfer = FALSE;
        res = hal_CameraStopXfer(FALSE);
        //if (res != XFER_FINISHED_WITH_OVERFLOW)
        //SCI_TRACE_LOW("hch , hal_CameraStopXfer result : %d  frame yadrr  : 0x%x  finished time : %d",res,g_pFrame->yaddr,SCI_GetTickCount());
        if(0 == res)
        {
            //SCI_TRACE_LOW("david PREVIEW 1 stop tick:%u", OS_GetTickCount());

            CAM_MODE_CHECK();
            g_pFrame->width  =  p_Dcam_cfg[g_DcamMode]->width;
            g_pFrame->height =  p_Dcam_cfg[g_DcamMode]->height;
            g_pFrame->type   =  ISP_IMAGE_FORMAT_YUV422;

            //ï¿½ï¿½lcdï¿½ì²½ï¿½ï¿½Ê¾
            _ISP_ServiceOnTxDone(g_pFrame);
            hal_camera_NextFrame();
        }
        else
        {
            SCI_TRACE_LOW("ISP:PRE1 stage2 error!");
            hal_camera_clear_fifo();
            ISP_DriverFrameUnLock(g_pFrame);
            //hal_camera_NextFrame();
        }
    }

    hal_camera_disable_scl();

    //next
    if(!g_stopPreview)
    {
        g_pFrame = hal_camera_getCurFrame();
        //SCI_TRACE_LOW("david stage 1 getcurframe g_pFrame:0x%x", g_pFrame);
        if((g_pFrame && g_pFrame->yaddr == NULL) || g_pFrame == NULL)
        {
            SCI_TRACE_LOW("ISP:PRE1 g yaddr NULL");
            hal_camera_clear_fifo();
            mask.fend = 1;
            //mask.dma  = 1;
            hal_CameraIrqSetMask(mask);
            return;
        }
        if(p_Dcam_cfg[g_DcamMode]->hal_cfg.scaleEnable)
            hal_camera_enable_scl();//hechang20201103 8w sensor
    //    SCI_TRACE_LOW("david PREVIEW 1 startnext tick:%u  status:0x%x scl:0x%x frame:0x%x, dvmode:%d",
    //                                   OS_GetTickCount(), hal_camera_get_status(), hal_camera_get_scl(),
    //                                   g_pFrame, g_Capturemode);

        CAM_MODE_CHECK();
        hal_CameraStartXfer(p_Dcam_cfg[g_DcamMode]->width * p_Dcam_cfg[g_DcamMode]->height * 2,
                                                  (uint8 *)g_pFrame->yaddr);
        gIsCameraStartXfer = TRUE;
        mask.fend = 1;
        //mask.dma  = 1;
        hal_CameraIrqSetMask(mask);
    }
}


LOCAL void Cam_snapshot_start()
{
    HAL_CAMERA_IRQ_CAUSE_T mask;
    memset(&mask, 0, sizeof(HAL_CAMERA_IRQ_CAUSE_T));

    hal_camera_mask_irq(CAM_IRQ_MASK);
    hal_camera_clear_fifo();// TODO: checking: clear buffer here ???

    hal_CameraIrqSetMask(mask);

    if (gIsCameraStartXfer == TRUE)
    {
        hal_CameraStopXfer(FALSE);
        gIsCameraStartXfer = FALSE;
    }

    g_pFrame = hal_Camera_getSnapshotFrame();
    SCI_TRACE_LOW("ISP:cap0  g_pFrame:0x%x, tick:%u capmode:%d",
                                g_pFrame, SCI_GetTickCount(), g_DcamMode);

    if(NULL == g_pFrame->yaddr || NULL == g_pFrame)
    {
        SCI_TRACE_LOW("ISP: capt0 yaddr NULL");
        return;
    }

    CAM_MODE_CHECK();
    hal_CameraStartXfer(p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->width * p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->height * 2,
                            (uint8 *)g_pFrame->yaddr);

    mask.fend = 1;
    gIsCameraStartXfer = TRUE;
    hal_CameraIrqSetMask(mask);
    g_ispmode = CAM_CAPTURE_STAGE_PROCESS;
}

LOCAL void Cam_snapshot_process()
{
    int res = 0;
    HAL_CAMERA_IRQ_CAUSE_T mask;
    memset(&mask, 0, sizeof(HAL_CAMERA_IRQ_CAUSE_T));

    if (gIsCameraStartXfer == TRUE)
    {
        SCI_TRACE_LOW("ISP:cap1 g_pFrame tick:%u capmode:%d",
                        SCI_GetTickCount(), g_DcamMode);
        gIsCameraStartXfer = FALSE;
        res = hal_CameraStopXfer(FALSE);
        if (res == XFER_SUCCESS)
        {
            hal_CameraIrqSetMask(mask);
            g_ispmode = CAM_PREVIEW_STAGE_START;

            //send YUV data
            g_pFrame->width  = p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->width;
            g_pFrame->height = p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->height;
            g_pFrame->type   = ISP_IMAGE_FORMAT_YUV422;

            _ISP_ServiceOnTxDone(g_pFrame);
            //hal_camera_NextFrame();
        }
        else
        {
            SCI_TRACE_LOW("ISP:cap1 error");
            mask.fend = 1;
            hal_CameraIrqSetMask(mask);
            g_ispmode = CAM_CAPTURE_STAGE_START;
        }
    }
    SCI_TRACE_LOW("ISP:cap1 over tick:%u",SCI_GetTickCount());
}

void Cam_mpeg_start()
{
    HAL_CAMERA_IRQ_CAUSE_T mask;
    memset(&mask, 0, sizeof(HAL_CAMERA_IRQ_CAUSE_T));

    hal_camera_mask_irq(CAM_IRQ_MASK);
    hal_camera_clear_fifo();// TODO: checking: clear buffer here ???


    hal_CameraIrqSetMask(mask);
    if (gIsCameraStartXfer == TRUE)
    {
        hal_CameraStopXfer(FALSE);
        gIsCameraStartXfer = FALSE;
    }

    g_pFrame = hal_camera_getCurFrame();
    SCI_TRACE_LOW("ISP:mpeg1 g_pFrame:0x%x", g_pFrame);
    if(g_pFrame->yaddr == NULL)
    {
        SCI_TRACE_LOW("SP:mpeg1 yaddr NULL");
        return;
    }

    #if 0
    hal_CameraStartXfer(YUV422_320_240_SIZE, (uint8 *)g_pFrame->yaddr);
    #else
    hal_CameraStartXfer(YUV422_200_200_SIZE, (uint8 *)g_pFrame->yaddr);
    #endif

    mask.fend = 1;
    gIsCameraStartXfer = TRUE;
    hal_CameraIrqSetMask(mask);
    g_ispmode = CAM_MPEG_STAGE_PROCESS;
}

void Cam_mpeg_process()
{
    HAL_CAMERA_IRQ_CAUSE_T mask;
    memset(&mask, 0, sizeof(HAL_CAMERA_IRQ_CAUSE_T));

    if (gIsCameraStartXfer == TRUE)
    {
        gIsCameraStartXfer = FALSE;
        if (hal_CameraStopXfer(FALSE) == XFER_SUCCESS)
        {
            hal_CameraIrqSetMask(mask);
            g_ispmode = CAM_MPEG_STAGE_START;

            //send YUV data
            g_pFrame->width  = 320;
            g_pFrame->height = 240;
            g_pFrame->type   = ISP_IMAGE_FORMAT_YUV422;
            _ISP_ServiceOnTxDone(g_pFrame);
            hal_camera_NextFrame();
        }
        else
        {
            SCI_TRACE_LOW("SP:mpeg stopxfer error");
            mask.fend = 1;
            hal_CameraIrqSetMask(mask);
            g_ispmode = CAM_MPEG_STAGE_START;
        }
    }
}


void ISP_Uix_StopPreview()
{
    SCI_TRACE_LOW("ISP:set stop preview!");

    //_ISP_Uix_ServiceGetDataCtrl();
    SCI_DisableIRQ();
    SCI_DisableFIQ();
    g_stopPreview = 1;
    hal_camera_mask_irq(CAM_IRQ_MASK);
    hal_camera_clear_irq_status(0xf);
    if (gIsCameraStartXfer == TRUE)
    {
        SCI_TRACE_LOW("ISP:stop xfer!");
        hal_CameraStopXfer(FALSE);
        gIsCameraStartXfer = FALSE;
    }
    hal_camera_clear_fifo();
    hal_camera_enable(FALSE);
    SCI_RestoreFIQ();
    SCI_RestoreIRQ();
    //_ISP_Uix_ServiceRelDataCtrl();
}


void ISP_Uix_SetBaseaddr(uint32 addr, uint32 size)
{
    ISP_SERVICE_T           *s = &s_isp_service;
    s->uix_base_addr = addr;
    s->uix_mem_size  = size;
    SCI_TRACE_LOW("ISP:baseaddr 0x%x, size %u", addr, size);
}



#if 0
void _ISP_UixIrqHandler(HAL_CAMERA_IRQ_CAUSE_T cause)
{
    HAL_CAMERA_IRQ_CAUSE_T mask;
    memset(&mask, 0, sizeof(HAL_CAMERA_IRQ_CAUSE_T));

    s_curtime = SCI_GetTickCount();
    SCI_TRACE_LOW("david _ISP_UixIrqHandler dv:%d   time : %u, DeltaTime : %u" ,
                          g_Capturemode, SCI_GetTickCount(), s_curtime-s_lastime);
    s_lastime = s_curtime;

    switch (g_ispmode)
    {
        case CAM_PREVIEW_STAGE_START:
        if (cause.fend)
        {
            _ISP_ServiceSendRequest(0,ISP_SERVICE_PREVIEW_START_SIG,0,0);
        }
        break;

        case CAM_PREVIEW_STAGE_PROCESS:
        if(cause.dma)
        {
            //SCI_TRACE_LOW("david uixhandler dma tick:%u,cause.fend  %d ", OS_GetTickCount(),cause.fend);
        }

        if(cause.fend)
        {
            _ISP_ServiceSendRequest(0,ISP_SERVICE_PREVIEW_PROCESS_SIG,0,0);
        }
        break;

    case CAM_CAPTURE_STAGE_START:

        if (cause.fend)
        {
            _ISP_ServiceSendRequest(0,ISP_SERVICE_CAP_START_SIG,0,0);
        }
        break;

    case CAM_CAPTURE_STAGE_PROCESS:

        if (cause.fend)
        {
            _ISP_ServiceSendRequest(0,ISP_SERVICE_CAP_PROCESS_SIG,0,0);
        }
        break;

#if 0
    case CAM_MPEG_STAGE_START:

        if (cause.fend)
        {
            Cam_mpeg_start();
        }
        break;

    case CAM_MPEG_STAGE_PROCESS:

        if (cause.fend)
        {
            Cam_mpeg_process();
        }
        break;
#endif

    default:
       // gSensorInfo.camirqhandler(255, 255);
        break;
    }
    return;
}

#else

void _ISP_UixIrqHandler(HAL_CAMERA_IRQ_CAUSE_T cause)
{
    HAL_CAMERA_IRQ_CAUSE_T mask;
    memset(&mask, 0, sizeof(HAL_CAMERA_IRQ_CAUSE_T));

    s_curtime = SCI_GetTickCount();
    SCI_TRACE_LOW("ISP:UixIrqHandler dv:%d   time : %u, DeltaTime : %u" ,
                          g_DcamMode, SCI_GetTickCount(), s_curtime-s_lastime);
    s_lastime = s_curtime;

    switch (g_ispmode)
    {
        case CAM_PREVIEW_STAGE_START:

        if(g_stopPreview)
        {
            SCI_TRACE_LOW("ISP:UixIrqHandler stop!");
            hal_camera_mask_irq(CAM_IRQ_MASK);
            hal_camera_clear_fifo();
            return;
        }

        if (cause.fend)
        {
            Cam_preview_start();
        }
        break;

        case CAM_PREVIEW_STAGE_PROCESS:

        if(g_stopPreview)
        {
            SCI_TRACE_LOW("ISP:UixIrqHandler stop!");
            hal_camera_mask_irq(CAM_IRQ_MASK);
            hal_camera_clear_fifo();
            return;
        }

        if(cause.fend)
        {
            Cam_preview_process();
        }
        break;

    case CAM_CAPTURE_STAGE_START:

        if (cause.fend)
        {
            Cam_snapshot_start();
            //SP_ServiceSendRequest(0,ISP_SERVICE_CAP_START_SIG,0,0);
        }
        break;

    case CAM_CAPTURE_STAGE_PROCESS:

        if (cause.fend)
        {
            Cam_snapshot_process();
            //SP_ServiceSendRequest(0,ISP_SERVICE_CAP_PROCESS_SIG,0,0);
        }
        break;

#if 0
    case CAM_MPEG_STAGE_START:

        if (cause.fend)
        {
            Cam_mpeg_start();
        }
        break;

    case CAM_MPEG_STAGE_PROCESS:

        if (cause.fend)
        {
            Cam_mpeg_process();
        }
        break;
#endif

    default:
       // gSensorInfo.camirqhandler(255, 255);
        break;
    }
    return;
}
#endif


#if 0
static int g_preview_count = 0;
//int _ISP_UixCaptureOneFrame(CAM_FORMAT_T ImageFormat, UINT32 SizeIndex, CAM_IMAGE_T Buffer)
int _ISP_UixCaptureOneFrame()
{
    HAL_CAMERA_IRQ_CAUSE_T mask;
    memset(&mask , 0, sizeof(HAL_CAMERA_IRQ_CAUSE_T));

    SCI_TRACE_LOW("david _ISP_UixCaptureOneFrame!");

    if (g_preview_count == 0)
    {
        //hal_CameraIrqSetMask(mask);
        hwp_camera->IRQ_MASK &= ~CAM_IRQ_MASK;

        if (gIsCameraStartXfer == TRUE)
        {
            SCI_TRACE_LOW("david _ISP_UixCaptureOneFrame stop xfer!");
            hal_CameraStopXfer(FALSE);
            gIsCameraStartXfer = FALSE;
        }

       // g_ispmode = CAM_MODE_PREVIEW_STAGE_1;
        //gSensorInfo.vidbuffer = Buffer;
        //gSensorInfo.format    = ImageFormat;
        //gSensorInfo.npixels   = SizeIndex;
        //s_prevCount = 1;

        hwp_camera->CMD_SET   = CAMERA_FIFO_RESET;
           hwp_camera->IRQ_CLEAR = CAMERA_OVFL;
        hwp_camera->IRQ_MASK |= CAM_IRQ_FEND;
        hwp_camera->IRQ_MASK |= CAM_IRQ_FSTA;

        /*
        mask.fend = 1;
        //mask.overflow = 1;
        mask.fstart = 1;
        hal_CameraIrqSetMask(mask);
        */
        hal_CameraControllerEnable(TRUE);
    }
    else
    {
        g_preview_count++;
    }
    SCI_TRACE_LOW("david _ISP_UixCaptureOneFrame over !");

    return 0;
}
#else
static int cam_preview_count = 0;

int _ISP_UixCaptureOneFrame()
{
    SCI_TRACE_LOW("ISP:CaptureOneFrame");

    hal_camera_mask_irq(CAM_IRQ_MASK);
    if (gIsCameraStartXfer == TRUE)
    {
        SCI_TRACE_LOW("ISP: stop xfer!");
        hal_CameraStopXfer(FALSE);
        gIsCameraStartXfer = FALSE;
    }

    hal_camera_clear_fifo();
    hal_camera_clear_irq_status(0xf);
    hal_camera_unmask_irq(CAM_IRQ_FEND);
    hal_camera_enable(TRUE);

    return 0;
}

#endif

void ISP_UixSetDcamMode(int mode)
{
    SCI_TRACE_LOW("ISP:SetDcamMode mode=%d", mode);
    if(mode >= DCAMERA_MODE_PREVIEW && mode <= DCAMERA_MODE_VIDEOCALL)
    {
        g_DcamMode = mode;
    }
}

int ISP_UixGetDcamMode()
{
    SCI_TRACE_LOW("ISP:GetDcamMode mode=%d", g_DcamMode);
    return g_DcamMode;
}


int ISP_UixOpenCameraContoller()
{
    HAL_CAMERA_CFG_T * pCamConfig;
    HAL_CAMERA_IRQ_CAUSE_T mask = {0, 0, 0, 0};

    CAM_MODE_CHECK();
    pCamConfig = (HAL_CAMERA_CFG_T *)&(p_Dcam_cfg[g_DcamMode]->hal_cfg);

    SCI_TRACE_LOW("ISP:CamContoller mode=%d. 1 [%d.%d.%d.%d]",
                    g_DcamMode,
                    pCamConfig->rstActiveH,
                    pCamConfig->pdnActiveH,
                    pCamConfig->dropFrame,
                    pCamConfig->camClkDiv );

    SCI_TRACE_LOW("ISP:CamContoller 2 [%d. %d. %d. %d]",
                    pCamConfig->endianess,
                    pCamConfig->camId,
                    pCamConfig->vsync_inv,
                    pCamConfig->spi_pixels_per_line);

    SCI_TRACE_LOW("ISP:CamContoller 3 [%d. %d. %d. %d. %d]",
                    pCamConfig->endianess,
                    pCamConfig->camId,
                    pCamConfig->vsync_inv,
                    pCamConfig->spi_pixels_per_line,
                    pCamConfig->spi_pixels_per_column );

    SCI_TRACE_LOW("ISP:CamContoller 4 [%d. %d. %d. %d.]",
                    pCamConfig->spi_yuv_out,
                    pCamConfig->spi_mode,
                    pCamConfig->spi_little_endian_en,
                    pCamConfig->spi_ctrl_clk_div   );

    SCI_TRACE_LOW("ISP:CamContoller 5 [%d. %d. %d. %d]",
                    pCamConfig->colRatio,
                    pCamConfig->rowRatio,
                    pCamConfig->scaleEnable,
                    pCamConfig->cropEnable );


    SCI_TRACE_LOW("ISP:CameraContoller 6 [%d. %d. %d. %d. %d]",
                    pCamConfig->dstWinColStart,
                    pCamConfig->dstWinColEnd,
                    pCamConfig->dstWinRowStart,
                    pCamConfig->dstWinRowEnd,
                    pCamConfig->ddr_en);

    hal_CameraOpen(pCamConfig);
    hal_CameraIrqSetMask(mask);


    return 0;
}


/* ï¿½ï¿½ÆµÍ¨ï¿½ï¿½Ä£Ê½  SIZE:200X200 */
int ISP_UixOpenCamCtrlWatchMode(int open)
{
    HAL_CAMERA_CFG_T CamConfig;
    HAL_CAMERA_IRQ_CAUSE_T mask = {0, 0, 0, 0};

    //SCI_TRACE_LOW("david ISP_UixOpenCamCtrlWatchMode %d", open);

    memset(&CamConfig, 0, sizeof(HAL_CAMERA_CFG_T));
    CamConfig.rstActiveH = FALSE;
    CamConfig.pdnActiveH = TRUE;
    CamConfig.dropFrame  = FALSE;
    CamConfig.camClkDiv  = g_MclkDiv; //24M
    CamConfig.endianess  = NO_SWAP;
    CamConfig.camId      =  0;
    CamConfig.vsync_inv  = FALSE;
    CamConfig.spi_pixels_per_line   = 640;  //320; //IspOutWidth;
    CamConfig.spi_pixels_per_column = 480;  //IspOutHeight;
    CamConfig.spi_yuv_out           = SPI_OUT_Y0_U0_Y1_V0;
    CamConfig.spi_mode              = SPI_MODE_MASTER2_2;  //2 wire;
    CamConfig.spi_little_endian_en  = FALSE;
    CamConfig.spi_ctrl_clk_div      = 1;

//  Camera Controller VideoChat mode
//  Sensor input 640*480 -> Output 200*200
    CamConfig.colRatio   = COL_RATIO_1_2;
    CamConfig.rowRatio   = ROW_RATIO_1_2;
    CamConfig.scaleEnable = TRUE;

    CamConfig.cropEnable     = TRUE;
    CamConfig.dstWinColStart = 120; //0;
    CamConfig.dstWinColEnd   = 520; //640;
    CamConfig.dstWinRowStart = 40;  //0;
    CamConfig.dstWinRowEnd   = 440; //480;

    hal_CameraOpen(&CamConfig);
    hal_CameraIrqSetMask(mask);

    SCI_TRACE_LOW("ISP:WatchMode over!");

    return 0;
}


int ISP_UixOpenCamCtrlDVMode(int open)
{
    HAL_CAMERA_CFG_T CamConfig;
    HAL_CAMERA_IRQ_CAUSE_T mask = {0, 0, 0, 0};

    //SCI_TRACE_LOW("david ISP_UixOpenCamCtrlDVMode %d", open);

    memset(&CamConfig, 0, sizeof(HAL_CAMERA_CFG_T));
    CamConfig.rstActiveH = FALSE;
    CamConfig.pdnActiveH = TRUE;
    CamConfig.dropFrame  = FALSE;
    CamConfig.camClkDiv  = g_MclkDiv; //12M
    CamConfig.endianess  = NO_SWAP;
    CamConfig.camId      =  0;
    CamConfig.vsync_inv  = FALSE;
    CamConfig.spi_pixels_per_line   = 640;  //320; //IspOutWidth;
    CamConfig.spi_pixels_per_column = 480;  //IspOutHeight;
    CamConfig.spi_yuv_out           = SPI_OUT_Y0_U0_Y1_V0;
    CamConfig.spi_mode              = SPI_MODE_MASTER2_2;  //2 wire;
    CamConfig.spi_little_endian_en  = FALSE;
    CamConfig.spi_ctrl_clk_div      = 1;

//  Camera Controller DV mode
//  Sensor input 640*480 -> Output 320*240
    CamConfig.colRatio   = COL_RATIO_1_2;
    CamConfig.rowRatio   = ROW_RATIO_1_2;
    CamConfig.scaleEnable = TRUE;

    CamConfig.cropEnable     = TRUE;
    CamConfig.dstWinColStart = 0;
    CamConfig.dstWinColEnd   = 640;
    CamConfig.dstWinRowStart = 0;
    CamConfig.dstWinRowEnd   = 480;

    hal_CameraOpen(&CamConfig);
    hal_CameraIrqSetMask(mask);

    SCI_TRACE_LOW("ISP:CtrlDVMode over!");

    return 0;
}

int ISP_UixOpenCameraController(int open)
{
    HAL_CAMERA_CFG_T CamConfig;
    HAL_CAMERA_IRQ_CAUSE_T mask = {0, 0, 0, 0};

    //SCI_TRACE_LOW("david ISP_UixOpenCameraController %d", open);

    memset(&CamConfig, 0, sizeof(HAL_CAMERA_CFG_T));
    CamConfig.rstActiveH = FALSE;
    CamConfig.pdnActiveH = TRUE;
    CamConfig.dropFrame  = FALSE;
    CamConfig.camClkDiv  = g_MclkDiv; //
    CamConfig.endianess  = NO_SWAP;
    CamConfig.camId      =  0;
    CamConfig.cropEnable = FALSE;
    CamConfig.colRatio   = COL_RATIO_1_1;
    CamConfig.rowRatio   = ROW_RATIO_1_1;
    CamConfig.vsync_inv  = FALSE;
    CamConfig.spi_pixels_per_line   = 640;//320; //IspOutWidth;
    CamConfig.spi_pixels_per_column = 480; //IspOutHeight;
    CamConfig.spi_yuv_out           = SPI_OUT_Y0_U0_Y1_V0;
    CamConfig.spi_mode              = SPI_MODE_MASTER2_2;  //2 wire;
    CamConfig.spi_little_endian_en  = FALSE;
    CamConfig.spi_ctrl_clk_div      = 1;

    hal_CameraOpen(&CamConfig);

    hal_CameraIrqSetMask(mask);
    SCI_TRACE_LOW("ISP:CameraController over!");

    return 0;

}

//W217
LOCAL void _ISP_Uix_ServiceMpegHandler(ISP_FRAME_T *p_frame)
{

    ISP_SERVICE_T           *s = &s_isp_service;
    ISP_FRAME_T             dst_frame = {0};
    uint32                  status = 0;
    ISP_ADDRESS_T           src_addr = {0};
    ISP_ADDRESS_T           dst_addr = {0};
    uint32                  frm_cnt = 0;
    int                     cb_rtn = ISP_CB_PROC_DONE;

    //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceMpegHandler, p_frame->flags 0x%x "
 //   SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6586_112_2_18_1_44_7_483,(uint8*)"d",p_frame->flags);

    if(s->enc_stoped)
    {
        //ISP_SERVICE_TRACE:"ISP_SERVICE: MPEG stopped"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6590_112_2_18_1_44_7_484,(uint8*)"");
        return;
    }

    //switch(p_frame->flags)
    {
    //    case ISP_STATE_MPEG_CAPTURE:

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one capture frame, %d"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6599_112_2_18_1_44_7_485,(uint8*)"d", SCI_GetTickCount());

            if(s->b_is_atv&&(s->local_review_enable||s->local_display_enable))
            {
                _ISP_ServiceCallUserPostCapCB(0);
            }

            SCI_MEMCPY((void*)&s->local_frame,(void*)p_frame, sizeof(ISP_FRAME_T));

            if(s->enc_rotation == ISP_ROTATION_0 &&
               ISP_DriverFrameIsLocked(s->p_frame_enc_cur))
            {
                // No need to rotate image, but the current frame has not been released by DV.
                // then just do next review, and do not encode this frame.
                //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceMpegHandler, encode frame not released"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6608_112_2_18_1_44_7_486,(uint8*)"");

                if(s->local_review_enable)
                {
                    #if 0
                    dst_frame.type = s->display_block.img_fmt;
                    dst_frame.width = s->display_range.w;
                    dst_frame.height = s->display_range.h;
                    dst_frame.flags = ISP_STATE_MPEG_REVIEW;
                    frm_cnt = s->display_frame_count % ISP_PATH_FRAME_COUNT_MAX;
                    dst_frame.yaddr = s->display_addr[frm_cnt].yaddr;
                    dst_frame.uaddr = s->display_addr[frm_cnt].uaddr;

                    _ISP_ServiceInternScale(&s->local_frame, &s->input_range, &dst_frame);
                    #endif
                }
            }
            else
            {
                #if 0
                dst_frame.type   = ISP_IMAGE_FORMAT_YUV422; //s->path_output_image_format;
                dst_frame.width  = 320;   //s->encoder_size.w;
                dst_frame.height = 240;   //s->encoder_size.h;
                //dst_frame.flags  = ISP_STATE_MPEG_SCALE;
                dst_frame.yaddr  = s->uix_frame_enc_cur->yaddr;
                dst_frame.uaddr  = s->uix_frame_enc_cur->uaddr;
                #endif
                //gouda scale
                //TODO  ...
            }

            if(s->is_enc_first_frame)
            {
                status = SCI_SetEvent(s_isp_event, ISP_MPEG_START_EVENT, SCI_OR);
                SCI_ASSERT(!status); /*assert verified*/
                s->is_enc_first_frame = 0;
            }

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one encode frame, %d"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6647_112_2_18_1_44_8_487,(uint8*)"d", SCI_GetTickCount());

            if(!ISP_DriverFrameIsLocked(s->uix_frame_enc_cur)) // is the current frame has not been released, drop this encode frame
            {
                memcpy(s->uix_frame_enc_cur->yaddr, p_frame->yaddr,
                                p_Dcam_cfg[g_DcamMode]->height * p_Dcam_cfg[g_DcamMode]->width * 2);

                s->uix_frame_enc_cur->width  = p_Dcam_cfg[g_DcamMode]->width;
                s->uix_frame_enc_cur->height = p_Dcam_cfg[g_DcamMode]->height;

                s->uix_frame_enc_cur->type = ISP_IMAGE_FORMAT_YUV422;//s->path_output_image_format;
                ISP_DriverFrameLock(s->uix_frame_enc_cur);// unlocked by user
                _ISP_ServiceCallUserEncCB((void*)s->uix_frame_enc_cur);
                s->uix_frame_enc_cur = s->uix_frame_enc_cur->next;
            }
            else
            {
                SCI_TRACE_LOW("ISP:MpegHandler, encode frame 0x%x", s->uix_frame_enc_cur);
                //ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceMpegHandler, encode frame 0x%x not released!"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6687_112_2_18_1_44_8_488,(uint8*)"d",s->p_frame_enc_cur);
            }

            //ISP_SERVICE_LOG:"ISP_SERVICE: _ISP_ServiceMpegHandler, Got one review frame, %d"
           // SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC6530_6719_112_2_18_1_44_8_489,(uint8*)"d", SCI_GetTickCount());

            cb_rtn = _ISP_ServiceCallUserDispCB((void*)p_frame);
            if(ISP_CB_PROC_DONE == cb_rtn)
            {
                _ISP_ServiceDisplayOneFrame(s->display_block.block_id,p_frame);
                ISP_DriverFrameUnLock(p_frame);
                if(s->b_is_atv&&(s->local_review_enable||s->local_display_enable))
                {
                    _ISP_ServiceCallUserPostCapCB(1);
                }
            }
            else
            {
                s->disp_blk_id = s->display_block.block_id;
            }

            if(s->is_first_frame)
            {
                s->is_first_frame = 0;
            }

            s->preview_skip_frame_num = 0;
            //_ISP_UixCaptureOneFrame();
            //_ISP_ServiceCaptureOnce(s->first_cap_format, ISP_STATE_MPEG_CAPTURE);

    /*
          break;

        default:

            break;
    */
    }

    if(s->err_code)
    {
        ISP_DriverDeInit(AHB_REG_BASE, s->module_addr);
        s->err_code = 0;
    }

    return;
}


/*
void YUV422_UYVY_TO_I420_Y_U_V(    unsigned char* dst,
                                unsigned char* src,
                                unsigned int width,
                                unsigned int height);

void I420_Y_U_V_Rotate90(unsigned char* dst,
                         unsigned char* src,
                         unsigned int width,
                         unsigned int height);
*/


void ISP_YUV422_UYVY_Rotate_90(
    unsigned char* src,
    unsigned char* dst,
    unsigned int width,
    unsigned int height)
{
    int pic_size = width*height;
    int yuv_size = pic_size<<1;
    int yuv_width = width <<1;
    int i = 0;
    int j = 0;
    int k = 0;
    int pos = 0;

    SCI_ASSERT(src!=NULL);
    SCI_ASSERT(dst!=NULL);

    SCI_TRACE_LOW("ISP:Rot_90 w:%d, h:%d",
        width, height);

    for(i= 0; i < yuv_width; i++)
    {
        pos = yuv_size;
        for(j = height-1; j>=0; j--)
        {
            pos -= yuv_width;
            dst[k++] = src[pos+i];
        }
    }
}


void ISP_YUV422_UYVY_Rotate_270(
    unsigned char* src,
    unsigned char* dst,
    unsigned int width,
    unsigned int height)

{
    int pic_size = width*height;
    int yuv_width = width <<1;
    int i = 0;
    int j = 0;
    int k = 0;
    int pos = 0;

    SCI_ASSERT(src!=NULL);
    SCI_ASSERT(dst!=NULL);

    SCI_TRACE_LOW("ISP:Rot_270 w:%d, h:%d",width, height);

    for(i= 0; i < yuv_width; i++)
    {
        pos = yuv_width - 1;
        for(j = 0; j < height; j++)
        {
            dst[k++] = src[pos-i];
            pos += yuv_width;
        }
    }
}

/*
* ISP_ServiceSetDumpMode:
* mode: ISP_SERVICE_DUMP_FILE, the yuv data will be dumped to the UDisk,
        ISP_SERVICE_DUMP_DDR, the yuv data will be saved to memory and can be parsed by trace32
*/
void ISP_ServiceSetDumpMode(int mode)
{
    if(mode >= ISP_SERVICE_DUMP_INVALID && mode <ISP_SERVICE_DUMP_MODE_MAX)
    {
        isp_yuv_dump_config.dump_mode = mode;
    }
    else
    {
        SCI_TRACE_LOW("ISP_ServiceSetDumpMode: failed to set mode");
        isp_yuv_dump_config.dump_mode = ISP_SERVICE_DUMP_INVALID;
    }
}

/*
* ISP_ServiceSetDumpFrameNum: 
* set max num of frames that can be dumped
* if max_dump_frame_to_file = 0, indicates that the data can be dumped in real time
*/
void ISP_ServiceSetDumpFrameNum(int size)
{
    isp_yuv_dump_config.max_dump_frame_to_file = size;
}

int ISP_ServiceGetDumpMode()
{
    return isp_yuv_dump_config.dump_mode;
}

int ISP_ServiceGetDumpFrameNum()
{
    return isp_yuv_dump_config.max_dump_frame_to_file;
}


//--------------------------------------------------------------------------------------


