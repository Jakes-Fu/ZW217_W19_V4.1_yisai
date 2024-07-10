/*******************************************************************************
 ** FileName:      dcamera_app.c                                               *
 ** Author:        Zhemin.Lin&Benny.Zhou                                       *
 ** Date:          2005/8/04                                                   *
 ** CopyRight:     2005, Spreatrum, Incoporated, All right reserved            *
 ** Description:   define DCAM application                                     *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2005/8/04      Zhemin.Lin&Benny.Zhou      Created                          *
 ** 2006/4/13      Liangwen.Zhen              Modify                           *
 ** 2009/7         Baggio.he                  8801H                            *
 ** 2010/3         Baggio.he                  Support 8800G                    *
 ** 2011/2         Baggio.he                  Add SAMSUNG interface            *
 ** 2011/3         Baggio.he                  Add Memory management            *
 ** 2011/10        Baggio.he                  Support 6530                     *
 *******************************************************************************/


//#include <stdio.h>
//#include <stdlib.h>
#include "ms_ref_dc_trc.h"
//#include <string.h>
#include "sci_types.h"
#include "os_api.h"

#include "dcamera_common.h"
#include "dc_common.h"
#include "dcamera_mem.h"
//#include "arm_reg.h"
#include "dal_jpeg.h"
#include "sc_reg.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
#include "dc_cfg.h"
#include "display.h"
#include "ref_lcd.h"
#include "exif_writer.h"
#include "rotation_app.h"
#include "dc_cm.h"
#include "threadx_os.h"
#include "jpeg_interface.h"
#include "priority_system.h"
#include "img_transform.h"          //add for the date mark blend need
#include "graphics_draw.h"
#include "dcamera_misc.h"
#ifndef ATV_SUPPORT_NONE
#include "dcamera_atv.h"
#endif
#include "card_sdio.h"
#include "sdio_card_pal.h"
#include "video_display.h"          //add for external interface compatible
#include "dcamera_cfg.h"

 /*lint -save -e551 */
/*lint -save -e655 -e64*/

//#define     DC_EX_DEBUG

#define     DCAMERA_DEBUG

#ifdef      DCAMERA_DEBUG
    #define DCAMERA_TRACE                      SCI_TRACE_LOW
#endif


#ifdef  DC_EX_DEBUG
#include "sfs.h"
LOCAL DCAMERA_CALL_BACK    ex_snapshot_callback = SCI_NULL;
LOCAL uint32               b_image_cb_for_snapshot_ex = 0;
LOCAL uint32               preview_count = 0;


#endif

/**---------------------------------------------------------------------------*
 **                         MACRO  definition                                 *
 **---------------------------------------------------------------------------*/
#define DC_SNAPSHOT_EX_EVENT                                        BIT_10

 // Preview zoom
#define DCAMERA_PREVIEW_ZOOM_LEVEL_MAX              0x08
#define DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX          0x08

#define ZOOM_STEP(x)                                ((x * 2)/4 / DCAMERA_PREVIEW_ZOOM_LEVEL_MAX)

#define DCAMERA_JPEG_ENCODE_QUALITY_LEVEL_MIN       0
#define DCAMERA_JPEG_ENCODE_QUALITY_LEVEL_MAX       8

#define DCAMERA_CTRL_LEVEL_MAX                      9
#define DCAMERA_MALLOC_BUF_SIZE_MAX                 (380*1024)  // 380K
#define DCAMERA_UNICODE_FILE_NAME_LEN_MAX           (50*2)      // 50
// Digital camera return handle
#define DC_RET_ERR(value)           if(PNULL == func_ptr)                        \
                                    {                                            \
                                        return value;                            \
                                    }                                            \
                                    else                                         \
                                    {   _DCAMERA_GetMutex(s_callback_mutex_ptr); \
                                        (func_ptr)(value, 0, 0);                 \
                                        _DCAMERA_PutMutex(s_callback_mutex_ptr); \
                                        return value;                            \
                                    }                                            \

//digital camera thread information
#define DCAMERA_TASK_NAME                           "T_DCAMERA"
#define DCAMERA_TASK_QNAME                          "DCAMERA Queue"
#define DCAMERA_TASK_STACK_SIZE                     (6*1024)
#define DCAMERA_TASK_QNUM                           30
#define DCAMERA_TASK_PRI                            PRI_DCAMERA_TASK//  (SCI_PRIORITY_LOWEST + 1)
#define DCAMERA_TASK_PREEMPTIVE                     SCI_PREEMPT
#define DCAMERA_TASK_START_FLAG                     SCI_DONT_START//SCI_AUTO_START
#define DCAMERA_QUEUE_FEW_NUM                       5

// timer
#define DCAMERA_TIMER_EXPIRE_DEFAULT_VALUE  200     // ms

// Interval time of frame in burst snapshot
#define DCAMERA_BST_INTERVAL_TIME_MIN       10      // ms
#define DCAMERA_BST_INTERVAL_TIME_MAX       500     // ms
#define DCAMERA_TIMEOUT_INTERVAL            4000
#define DCAMERA_AWB_CAL_DISABLE             0
#define DCAMERA_AWB_CAL_ENABLE              1

#define DCAMERA_BRIGTHNESS_DEFAULT          3
#define DCAMERA_CONTRAST_DEFAULT            3
#define DCAMERA_SATURATION_DEFAULT          3
#define DCAMERA_EC_DEFAULT                  3


#define DCAMERA_PARAM_INVALID_VALUE         0xFF

#define DCAMERA_ISP_CAPTURE_FRAME           0x01


// about preview handle
#define DCAMERA_PREVIEW_MODE_DEFAULT        0x00
#define DCAMERA_PREVIEW_MODE_IGNORE         0xFFFF

// about DCAMERA operation flag
#define DCAMERA_OP_ISP_CAPTURE_FRAME_FLAG   0x01
#define DCAMERA_OP_ISP_SCALE_FRAME_FLAG     0x02


// Camera error try count
#define DCAMERA_CAMERA_TRY_COUNT            3

#define DCAMERA_UPCC_JPEG_SIZE              (32*1024)

#define DCAMERA_PIXEL_ALIGNED               4
#define DCAMERA_WIDTH(w)                    ((w)& ~(DCAMERA_PIXEL_ALIGNED - 1))
#define DCAMERA_HEIGHT(h)                   ((h)& ~(DCAMERA_PIXEL_ALIGNED - 1))
#define DCAMERA_MAX(a,b)                    ((a) > (b) ? (a) : (b))
#define DCAMERA_SENSOR_MAX_SKIP_FRAME       10
#define DCAMERA_INTERPOLATION_COEF          4

#define DCAMERA_ENC_QUALITY_MID             6
#define DCAMERA_ENC_IMAGE_DOWN_STEP         32

#define DCAMERA_JPEG_ALIGN_LOW              8
#define DCAMERA_JPEG_ALIGN_MID              16
#define DCAMERA_JPEG_ALIGN_HIGH             32
#define DCAMERA_SENSOR_IDENTIFIED_SIGN      0xEF0A3B09
#define DCAMERA_ISP_PATH_YUV420_TH          1024

#define DCAMERA_JPEG_BUFFER_SIZE            200*1024

/**---------------------------------------------------------------------------*
 **                       Data  Structure definition                          *
 **---------------------------------------------------------------------------*/

typedef struct _dc_message_sig
{
    uint32  param_val;
    void *  param_ptr1;
    void *  param_ptr2;
}DCAMERA_MESSAGE_T;

typedef struct _dc_sig
{
    xSignalHeaderRec     sig;       // system signal
    DCAMERA_MESSAGE_T    dc_param;  // digital camera parameter
}DCAMERA_SIGNAL_T;

// enum
typedef enum
{
    DCAMERA_TIMER_MODE_GET_AF_STATUS = 0x00,
    DCAMERA_TIMER_MODE_MAX
}DCAMERA_TIMER_MODE_E;

typedef enum
{
    SNAPSHOT_NORMAL,
    SNAPSHOT_YUV422,
    SNAPSHOT_YUV420,
    SNAPSHOT_JPG,
    SNAPSHOT_UNK
}DCAMERA_SNAPSHOT_TYPE;

/**---------------------------------------------------------------------------*
 **                         Local variable definition                         *
 **---------------------------------------------------------------------------*/
LOCAL DCAMERA_INFO_T        s_dcamera_info              = {0};          // digital camera information struction
LOCAL DCAMERA_INFO_T *      s_dc_info_ptr           = PNULL;        // poiter of digital camera information struction
LOCAL SENSOR_EXP_INFO_T*    s_sensor_ptr            = PNULL;        // poiter of sensor information struction
LOCAL uint32                zoom_level_last_time    = DCAMERA_PREVIEW_ZOOM_LEVEL_MAX;
LOCAL uint32                dv_zoom_level_last_time = DCAMERA_PREVIEW_ZOOM_LEVEL_MAX;
LOCAL BOOLEAN               s_dc_is_open            = SCI_FALSE;    // if dc is open
LOCAL BOOLEAN               s_dc_is_preview         = SCI_FALSE;    // if dc is previewing
LOCAL BOOLEAN               s_dc_is_preview_ex      = SCI_FALSE;    // if dc is previewing
LOCAL BOOLEAN               s_dc_is_record          = SCI_FALSE;    // for pc camera preview
LOCAL BOOLEAN               s_dc_is_video_phone     = SCI_FALSE;    // for video phone preview and recorder
LOCAL uint8                 b_review_next_to_snapshot = 0;
LOCAL uint8                 b_burst_snapshot        = 0;
LOCAL uint8                 *s_upcc_dcam_ptr        = PNULL;
LOCAL uint8                 s_is_first_slice        = 0;
LOCAL BOOLEAN               s_dcam_is_stop_preview  = SCI_TRUE;
LOCAL DCAMERA_CALL_BACK     s_dc_record_callback    = PNULL;
LOCAL uint32                s_dc_operation_flag     = 0;
LOCAL SCI_SEMAPHORE_PTR     s_capture_done_sema_ptr = PNULL;
LOCAL uint32                s_sensor_id             = 0;
LOCAL uint32                s_operation_mode        = 0;
LOCAL DCAMERA_RECORD_PARAM_T record_param           = {0};
LOCAL SCI_MUTEX_PTR         s_upcc_mutx_ptr         = SCI_NULL;
LOCAL SCI_MUTEX_PTR         s_callback_mutex_ptr    = SCI_NULL;
LOCAL BLOCK_ID              s_dc_task_id            = SCI_INVALID_BLOCK_ID;
LOCAL ISP_JPEG_PARAM_T      isp_cap_param           = {0};
LOCAL volatile uint32       s_slice_out_once        = 0;
LOCAL volatile uint32       s_slice_read_out        = 0;
LOCAL volatile uint32       s_src_line_offset       = 0;
LOCAL volatile uint32       s_dst_line_offset       = 0;            //dest slice output total height
LOCAL DCAMERA_SNAPSHOT_PARAM_T                  s_snapshot_param            = {0};
LOCAL DCAMERA_SNAPSHOT_RETURN_PARAM_T           s_snapshot_rtn_param        = {0};
LOCAL DCAMERA_BURST_SNAPSHOT_PARAM_T            s_snapshot_burst_param      = {0};
LOCAL DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T     s_snapshot_burst_rtn_param  = {0};
LOCAL DCAMERA_SNAPSHOT_EX_PARAM_T               s_snapshot_ex_param         = {0};
LOCAL DCAMERA_SNAPSHOT_EX_RETURN_PARAM_T        s_snapshot_ex_rtn_param     = {0};
LOCAL SCI_EVENT_GROUP_PTR                                   s_dc_event = SCI_NULL;
LOCAL uint8                                                            s_isp_opned = 0;
LOCAL ISP_REVIEW_PARAM_T                                    s_review_param = {0};
LOCAL VIDEO_DISP_PARAM_T                                    s_video_disp_param = {0};

LOCAL uint8*               g_pic_buffer  = NULL;



/**---------------------------------------------------------------------------*
 **                         Extern  Function definition                       *
 **---------------------------------------------------------------------------*/
extern JINF_RET_E IMGJPEG_EncodeEx(JINF_ENC_IN_PARAM_T *in_param_ptr,
                                   JINF_ENC_OUT_PARAM_T *out_param_ptr);
extern JINF_RET_E IMGJPEG_DecodeEx(JINF_DEC_IN_PARAM_T * in_param_ptr,
                                JINF_DEC_OUT_PARAM_T * out_param_ptr);
extern JPEG_ENC_RET_E JPEG_EncodeJpeg(JPEG_ENC_IN_PARAM_T *in_param,
                                        JPEG_ENC_OUT_PARAM_T *out_param);
extern JPEG_DEC_RET_E JPEG_DecodeJpeg(JPEG_DEC_IN_PARAM_T  *in_param,
                                        JPEG_DEC_OUT_PARAM_T *out_param);
extern void TM_SendTestPointRequest(uint32 param1, uint32 param2);
extern void DPSLP_DCAM_EnableAhbSleep (uint32 mode);
extern PUBLIC BOOLEAN CARD_SDIO_SwichBaseClk(CARD_SDIO_SLOT_NO slotNo, uint32 sdio_base_clk);


/**---------------------------------------------------------------------------*
 **                         Local  Function definition                        *
 **---------------------------------------------------------------------------*/
int DCAMERA_OpenCameraController(int open);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_SetPreviewZoom(uint32 zoom_level);
LOCAL BOOLEAN                   _DCAMERA_IsPreview(void);
LOCAL BOOLEAN                   _DCAMERA_IsRecord(void);
LOCAL BOOLEAN                   _DCAMERA_IsVideoPhone(void);
LOCAL void                      _DCAMERA_SetVideoPhoneFlag(BOOLEAN is_vp);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_InitSensor(void);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_SetPreviewMode(DCAMERA_PARAM_ENVIRONMENT_E );
LOCAL BOOLEAN                   _DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_CMD_E, uint32);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_WaitForCaptureDone(void);
LOCAL void                      _DCAMERA_CaptureDoneNotice(void);
LOCAL uint8                     DC_GetSensorSize(void);
LOCAL void                      _DCamera_AdjustRect(ISP_RECT_T* p_src_rect,
                                                    ISP_RECT_T* p_inva_rect,
                                                    ISP_RECT_T* p_img_rect,
                                                    uint32      display_mode);
LOCAL void                      _DCAMERA_ConvertCoorToISPRect(LCD_ID_E,
                                                    ISP_RECT_T *,
                                                    ISP_RECT_T *,
                                                    DCAMERA_ROTATION_MODE_E );
LOCAL  DCAMERA_RETURN_VALUE_E _DCAMERA_JpgAddExif(ISP_FRAME_T* p_yuv,
                                                  DCAMERA_SIZE_T *p_src_size,
                                                  DCAMERA_RECT_T *p_trim_rect,
                                                  uint32*      p_jpg_buf,
                                                  uint32*      p_jpg_len,
                                                  uint32       jpg_quality);
PUBLIC void                      DCAMERA_BeforeSetting(void);
PUBLIC DCAMERA_RETURN_VALUE_E    DCAMERA_AfterSetting(void);
LOCAL void                      _DCAMERA_PreviewSensorSet(void);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_DoImageScaleInVT(DCAMERA_IMAGE_PARAM_T *p_src_img,
                                                          uint32                frame_index,
                                                          DCAMERA_IMAGE_PARAM_T *p_dst_img);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_DoSnapshot(DCAMERA_SNAPSHOT_PARAM_T *  param_ptr,
                                                    DCAMERA_SNAPSHOT_RETURN_PARAM_T* return_param_ptr);
PUBLIC uint32                    DCAMERA_SendSignal(DCAMERA_TASK_CMD_E  cmd_type,
                                                    uint32  param_val,
                                                    void*   param_ptr1,
                                                    void*   param_ptr2);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_RotateRGB565(DCAMERA_ROTATION_PARAM_T *param_ptr);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_RotateYUV422(DCAMERA_ROTATION_PARAM_T *param_ptr);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_RotateYUV420(DCAMERA_ROTATION_PARAM_T *param_ptr);
LOCAL int                       _DCAMERA_ImageRecivedCallback(void* address );
LOCAL void                      _DCAMERA_GetDateType(ISP_IMAGE_FORMAT img_format,
                                                     uint32           *p_data_type,
                                                     uint32           *p_data_pattern);
LOCAL  uint32                   _DCAMERA_GetISPDateType(DCAMERA_DATA_TYPE_E data_type,
                                                        DCAMERA_DATA_PATTERN_T data_pattern);
LOCAL void                      _DCAMERA_SetPreviewExFlag(BOOLEAN is_preview);
LOCAL BOOLEAN                   _DCAMERA_IsPreviewEx(void);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_SetPreviewEXZoom(uint32 zoom_level);
LOCAL void _DCAMERA_YUV422DownSample2YUV420(DCAMERA_IMAGE_PARAM_T *p_target_image,
                                                                                                  uint32                                  frame_id);

LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_GetSpecificImage(ISP_FRAME_T*                 p_captured,
                                                          DCAMERA_IMAGE_FRAME_PARAM_T* p_tartget_desc,
                                                          DCAMERA_IMAGE_PARAM_T*       p_target_image,
                                                          uint32                       frame_id,
                                                          uint32                       mem_start,
                                                          uint32                       *p_mem_used,
                                                          uint16                       jpg_enc_quality,
                                                          BOOLEAN                      b_exif);

LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_DoSnapshotEx(DCAMERA_SNAPSHOT_EX_PARAM_T *    param_ptr,
                                                      DCAMERA_SNAPSHOT_EX_RETURN_PARAM_T*   return_param_ptr);
LOCAL void                      _DCAMERA_BackupPreviewbuf2ReviewBuf(uint32 lcd_id, uint32 block_id);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_CapMemAssign(DCAMERA_SNAPSHOT_PARAM_T *param_ptr);
LOCAL void                      _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_MODE_E cap_rot_mode,
                                                            DCAMERA_SIZE_T *p_dst_size,
                                                            DCAMERA_RECT_T *p_trim_rect);
LOCAL uint32                    _DCAMERA_JpgEncSliceScalingGetSrcHeight(void);
LOCAL uint32                    _DCAMERA_JpgEncGetSlice(JINF_YUV_CHN_T *yuv_chn,
                                                        uint32 line_offset,
                                                        uint32 width,
                                                        uint32 read_lines);
LOCAL uint32                     _DCAMERA_JpgEncGetSliceF420T422(JINF_YUV_CHN_T *yuv_chn,
                                                         uint32 line_offset,
                                                         uint32 width,
                                                         uint32 read_lines);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_GetYUVRawData(ISP_FRAME_T  *p_src_img,
                                                       uint32       yuv_pattern,
                                                       uint8*       dst_buf);
LOCAL void                      _DCAMERA_SnapshotExFreeMem(void);
LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_GetFrameFromISP(DCAMERA_SNAPSHOT_PARAM_T *param_ptr,
                                                         BOOLEAN bIsPersonalSnapshot,
                                                         BOOLEAN bIsIspBypass,
                                                         DCAMERA_QUICK_VIEW_CALL_BACK quickview_cb);
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_StartPreviewEX(DCAMERA_PREVIEW_EX_PARAM_T const* param_ex_ptr);

LOCAL uint32 _DCAMERA_GetDateRectParam(DCAMERA_RECT_T trim_rect,
                                    DCAMERA_RECT_T *src_date_rect_ptr,
                                    DCAMERA_RECT_T *dst_date_rect_ptr);
LOCAL uint32 _DCAMERA_GetDateRectData(DCAMERA_RECT_T *src_rect_ptr,
                                    DCAMERA_RECT_T *dst_rect_ptr,
                                    uint32 *src_date_addr,
                                    uint32 *dst_date_addr);
LOCAL uint32  _DCAMERA_dateBlend(DCAMERA_RECT_T *target_rect_ptr,
                                                               DCAMERA_RECT_T  *src_date_rect_ptr,
                                                               ISP_ADDRESS_T  target_addr,
                                                               uint32 * src_date_addr);
LOCAL uint32    _DCAMERA_handleDateMark(DCAMERA_RECT_T target_rect,
                                                                            ISP_ADDRESS_T  target_addr,
                                                                            DCAMERA_RECT_T src_date_rect,
                                                                            uint32    * src_date_addr,
                                                                            BOOLEAN isCbInIsp
                                                                            );
LOCAL uint32 _DCAMERA_handleScaleAndDate(DCAMERA_SIZE_T target_size, ISP_ADDRESS_T  target_addr, BOOLEAN isCbInIsp);
LOCAL int32 _DCAMERA_reviewDateCallback(void *frame_ptr);

//LOCAL uint32 _DCAMERA_IsScalingBufferUsedForJpegEncoding(void);
LOCAL int _DCAMERA_PreviewDisplayCallback(void* address);
LOCAL int _DCAMERA_PreviewDisplayCallbackLowMem(void* address);
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_Close(void);
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_OpenISP(void);
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_CloseISP(void);

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetSensorRot(uint32 arg);
LOCAL  DCAMERA_ROTATION_MODE_E _DCAMERA_ROT_Convertor(DCAMERA_ROTATION_MODE_E src_rot,
                                                      DCAMERA_ROTATION_MODE_E *img_rot_ptr);

LOCAL void _DCAMERA_BackupReviewbuf(uint32 lcd_id, uint32 block_id, uint32 disp_buf, uint32 disp_buf_len);
LOCAL int _DCAMERA_ISP_REG_ExtCALLBACK(void *flag);
LOCAL int _DCAMERA_GetFrameFromIspExt(DCAMERA_EXT_GET_DATA_T *param_ptr);
LOCAL void _DCAMERA_getJpgMcuAlign(ISP_IMAGE_FORMAT jpg_src_format, uint32 * p_width_align, uint32 * p_height_align);
LOCAL void _Video_Display_SetFrame(const VIDEO_DISP_FRAME_T *p_frame, DISPLAY_DATA_FRAME_T * p_display_frameDate);
LOCAL uint32 _DC_GetImgSensorType(void);
LOCAL uint32 _DC_GetUpccSize(void);
LOCAL uint32 _DC_GetUpccRotationMode(void);
LOCAL uint32 _DC_GetDcPreviewRotationMode(void);
LOCAL uint32 _DC_GetDcCapRotationMode(void);

LOCAL int32 _DCAMERA_EncodeCallback(void *frame_ptr);


LOCAL void _DCAMERA_SelectFmarkPin (uint32 fmark_pin)
{
    if(SC6530 == CHIP_GetChipType())
    {
        *(volatile uint32*)0x8B0001E4 &= ~(BIT_28 | BIT_27);
        switch(fmark_pin)
        {
            case 0://LCD_FMARK_PIN_GPIO_3:
	            *(volatile uint32*)0x8B0001E4 &= ~(BIT_28 | BIT_27);
	            break;
            case 1://LCD_FMARK_PIN_LCMD8:
	            *(volatile uint32*)0x8B0001E4 |= BIT_27;
	            break;
            case 2://LCD_FMARK_PIN_SPI0_0_CS1:
	            *(volatile uint32*)0x8B0001E4 |= BIT_28;
	            break;
            case 3://LCD_FMARK_PIN_FMARK:
	            *(volatile uint32*)0x8B0001E4 |= (BIT_28 | BIT_27);
	            break;
            default:
            	break;
        }
    }
    else if(SC6531 == CHIP_GetChipType())
    {
        *(volatile uint32*)0x8B0001E4 &= ~(BIT_28 | BIT_27);
        switch(fmark_pin)
        {
            case 0://LCD_FMARK_PIN_GPIO_3:
	            *(volatile uint32*)0x8B0001E4 &= ~(BIT_28 | BIT_27);
	            break;
            case 1://LCD_FMARK_PIN_FMARK:
	            *(volatile uint32*)0x8B0001E4 |= BIT_27;
	            break;
            default:
	            // LCD_FMARK_PIN_LCMD8
	            *(volatile uint32*)0x8B0001E4 |= (BIT_28 | BIT_27);
	            break;
        }
    }
}

PUBLIC uint32 DCAMERA_IsSupport2MSnapshot(void)
{

#ifdef DC_2M_SUPPORT
    return SCI_TRUE;
#else
    return SCI_FALSE;
#endif

}

PUBLIC DCAMERA_INFO_T* GetDCAMInfo(void)
{
    return (DCAMERA_INFO_T*)&s_dcamera_info;

}

LOCAL uint32 _DCAMERA_GetQueueAvailable(uint32 threadId)
{
    extern SCI_THREAD_PTR SCI_GetThreadBlockFromId(BLOCK_ID thread_id);

    uint32          count       = 0;
    uint32          enqueued    = 0;
    uint32          store       = 0;
    char            *pQueueName = SCI_NULL;
    TX_QUEUE        *next_queue = SCI_NULL;
    TX_THREAD       *pSuspendedThread   = SCI_NULL;
    SCI_THREAD_T    *pThreadBlock       = SCI_NULL;

    pThreadBlock = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(threadId);

    tx_queue_info_get((TX_QUEUE *)&(pThreadBlock->queue),
        &pQueueName,
        (uint32 *)&enqueued,
        (uint32 *)&store,
        (TX_THREAD **)&pSuspendedThread,
        (uint32 *)&count,
        (TX_QUEUE **)&next_queue);

    //DCAMERA_TRACE:"DC: GetQueueAvailable queue info:enqueued:%d, store: %d, count: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_385_112_2_18_1_24_0_311,(uint8*)"ddd",enqueued, store, count);

    return store;
}

PUBLIC void DCAMERA_SetCapParam(ISP_CAP_PARAM_T *p_isp_cap)
{
    if(s_sensor_ptr && p_isp_cap)
    {
        p_isp_cap->cap_img_format       = s_sensor_ptr->image_format;
        p_isp_cap->cap_img_pattern      = s_sensor_ptr->image_pattern;
        p_isp_cap->cap_vsync_pol        = s_sensor_ptr->vsync_polarity;
        p_isp_cap->cap_hsync_pol        = s_sensor_ptr->hsync_polarity;
        p_isp_cap->cap_pclk_pol         = s_sensor_ptr->pclk_polarity;
        p_isp_cap->cap_frame_skipped    = s_sensor_ptr->preview_skip_num;
        p_isp_cap->cap_frame_decimated  = s_sensor_ptr->preview_deci_num;
        p_isp_cap->cap_sensor_if_mode   = s_sensor_ptr->output_mode;
        p_isp_cap->cap_sensor_if_endian = s_sensor_ptr->output_endian;

        if(SENSOR_MAIN == Sensor_GetCurId())
        {
            p_isp_cap->b_is_atv              = SCI_FALSE;
            p_isp_cap->ccir_pclk_src         = Sensor_GetCcirPclkSrc(SENSOR_MAIN);
            p_isp_cap->ccir_d0_src           = Sensor_GetCcirD0Src(SENSOR_MAIN);
            p_isp_cap->ccir_vsync_src        = Sensor_GetCcirVsyncSrc(SENSOR_MAIN);
            p_isp_cap->ccir_d1_src          = Sensor_GetCcirD1Src(SENSOR_MAIN);
        }
        else if(SENSOR_SUB == Sensor_GetCurId())
        {
            p_isp_cap->b_is_atv              = SCI_FALSE;
            p_isp_cap->ccir_pclk_src         = Sensor_GetCcirPclkSrc(SENSOR_SUB);
            p_isp_cap->ccir_d0_src           = Sensor_GetCcirD0Src(SENSOR_SUB);
            p_isp_cap->ccir_vsync_src        = Sensor_GetCcirVsyncSrc(SENSOR_SUB);
            p_isp_cap->ccir_d1_src          = Sensor_GetCcirD1Src(SENSOR_SUB);
        }
        if(s_operation_mode == DCAMERA_OP_ATV)
        {
            p_isp_cap->b_is_atv              = SCI_TRUE;
            p_isp_cap->ccir_pclk_src         = Sensor_GetCcirPclkSrc(SENSOR_ATV);
            p_isp_cap->ccir_d0_src           = Sensor_GetCcirD0Src(SENSOR_ATV);
            p_isp_cap->ccir_vsync_src        = Sensor_GetCcirVsyncSrc(SENSOR_ATV);
            p_isp_cap->ccir_d1_src          = Sensor_GetCcirD1Src(SENSOR_ATV);
            p_isp_cap->cap_atv_thre_eb       = s_sensor_ptr->threshold_eb;
            p_isp_cap->cap_atv_thre_start    = s_sensor_ptr->threshold_start;
            p_isp_cap->cap_atv_thre_end      = s_sensor_ptr->threshold_end;
        }
    }
}

PUBLIC uint32 DCAMERA_GetMpegRot(uint32 angle_needed)
{
    uint32                  angle_act = DCAMERA_ROTATION_0;

    switch(s_dcamera_info.sensor_rot)
    {
        case DCAMERA_ROTATION_180:
        {
            switch(angle_needed)
            {
                case DCAMERA_ROTATION_0:
                    angle_act = DCAMERA_ROTATION_180;
                    break;

                case DCAMERA_ROTATION_90:
                    angle_act = DCAMERA_ROTATION_270;
                    break;

                case DCAMERA_ROTATION_180:
                    angle_act = DCAMERA_ROTATION_0;
                    break;

                case DCAMERA_ROTATION_270:
                    angle_act = DCAMERA_ROTATION_90;
                    break;
                default:
                    break;
            }
            break;
        }

        case DCAMERA_ROTATION_MIRROR:
        case DCAMERA_ROTATION_FLIP:
            break;

        case DCAMERA_ROTATION_0:
        {
            if(angle_needed < DCAMERA_ROTATION_FLIP)
            {
                angle_act = angle_needed;
            }
            break;
        }
        default:
            break;
    }

    return angle_act;
}

PUBLIC void DCAMERA_GetMpegRect(DCAMERA_SIZE_T *p_src_size,DCAMERA_RECT_T *p_trim_rect)
{
    uint16                  width_trim = 0;
    uint16                  height_trim = 0;
    uint16                  zoom_step_w = 0;
    uint16                  zoom_step_h = 0;
    DCAMERA_INFO_T              *dc_info_ptr   = s_dc_info_ptr;

    if(SCI_NULL == p_src_size || SCI_NULL == p_trim_rect)
        return ;

    if(dv_zoom_level_last_time >= DCAMERA_PREVIEW_ZOOM_LEVEL_MAX)
    {
        dv_zoom_level_last_time = 0;
    }
    p_src_size->w = (uint16)dc_info_ptr->isp_preview_param.input_size.w;
    p_src_size->h = (uint16)dc_info_ptr->isp_preview_param.input_size.h;

    if(DCAMERA_OP_ATV == s_operation_mode)
    {
        p_trim_rect->x = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].trim_start_x;
        p_trim_rect->y = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].trim_start_y;
        p_trim_rect->w = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].trim_width;
        p_trim_rect->h = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].trim_height;

        if(0 == p_trim_rect->w || 0 == p_trim_rect->h)
        {
            p_trim_rect->x = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_start_x;
            p_trim_rect->y = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_start_y;
            p_trim_rect->w = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_width;
            p_trim_rect->h = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_height;
        }
    }
    else
    {
        zoom_step_w = ZOOM_STEP(p_src_size->w);
        zoom_step_h = ZOOM_STEP(p_src_size->h);

        width_trim  = zoom_step_w * dv_zoom_level_last_time;
        height_trim = zoom_step_h * dv_zoom_level_last_time;

        p_trim_rect->x += width_trim >> 1;
        p_trim_rect->y += height_trim >> 1;

        // Make sure that x offset and y offset of input image multiple of 2
        p_trim_rect->x = (p_trim_rect->x + 1) & (~1);
        p_trim_rect->y = (p_trim_rect->y + 1) & (~1);

        p_trim_rect->w = p_src_size->w - (p_trim_rect->x << 1);
        p_trim_rect->h = p_src_size->h - (p_trim_rect->y << 1);

        p_trim_rect->w = DCAMERA_WIDTH(p_trim_rect->w);
        p_trim_rect->h = DCAMERA_HEIGHT(p_trim_rect->h);
    }
    //DCAMERA_TRACE:"DC: DCAMERA_GetMpegRect, size {%d %d} rect {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_505_112_2_18_1_24_0_312,(uint8*)"dddddd",p_src_size->w,p_src_size->h,p_trim_rect->x,p_trim_rect->y,p_trim_rect->w,p_trim_rect->h);
}

/******************************************************************************/
// Description: _DCAMERA_SnapShotSendSignal
// Author:
// Input:      DCAMERA_SNAPSHOT_PARAM_T const *para
// Output:     none
// Return:     error status

// Note:
/******************************************************************************/
LOCAL uint32 _DCAMERA_SnapShotSendSignal(DCAMERA_SNAPSHOT_PARAM_T const *param_ptr)
{
    SCI_MEMSET((uint16*)&s_snapshot_param, 0, sizeof(DCAMERA_SNAPSHOT_PARAM_T)); /*lint !e718 !e746 -e718 -e746*/
    SCI_MEMSET((uint16*)&s_snapshot_rtn_param, 0, sizeof(DCAMERA_SNAPSHOT_RETURN_PARAM_T));

    SCI_MEMCPY(&s_snapshot_param, param_ptr, sizeof(DCAMERA_SNAPSHOT_PARAM_T));
    SCI_MEMCPY(&s_snapshot_rtn_param, param_ptr, sizeof(DCAMERA_SNAPSHOT_RETURN_PARAM_T)); /*lint !e516 -e516*/

    return DCAMERA_SendSignal(DCAMERA_TASK_CMD_SNAPSHOT, 0,
        (void *)&s_snapshot_param, (void *)&s_snapshot_rtn_param);

}
/******************************************************************************/
// Description: _DCAMERA_SnapShotBurstSendSignal
// Author:
// Input:      DCAMERA_SNAPSHOT_PARAM_T const   *pPara
// Output:     none
// Return:     error status

// Note:
/******************************************************************************/
LOCAL uint32 _DCAMERA_SnapShotBurstSendSignal(DCAMERA_BURST_SNAPSHOT_PARAM_T const *param_ptr)
{
    SCI_MEMSET((uint16*)&s_snapshot_burst_param, 0,
        sizeof(DCAMERA_BURST_SNAPSHOT_PARAM_T));
    SCI_MEMSET((uint16*)&s_snapshot_burst_rtn_param, 0,
        sizeof(DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T));

    SCI_MEMCPY(&s_snapshot_burst_param, param_ptr,
        sizeof(DCAMERA_BURST_SNAPSHOT_PARAM_T));
    SCI_MEMCPY(&s_snapshot_burst_rtn_param, param_ptr,
        sizeof(DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T));

    return DCAMERA_SendSignal(DCAMERA_TASK_CMD_BURST_SNAPSHOT, 0,
        (void *)&s_snapshot_burst_param, (void *)&s_snapshot_burst_rtn_param);

}
/*****************************************************************************/
//  Description: Get Mutex
//  Global resource dependence:
//  Author:
/*****************************************************************************/
LOCAL void _DCAMERA_GetMutex(SCI_MUTEX_PTR mutex)
{
    //DCAMERA_TRACE:"_DCAMERA_GetMutex"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_555_112_2_18_1_24_0_313,(uint8*)"");
    if( NULL == mutex )
    {
        //DCAMERA_TRACE:"_DCAMERA_GetMutex: Mutex is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_558_112_2_18_1_24_0_314,(uint8*)"");
        return;
    }

    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);

}//
/*****************************************************************************/
//  Description: Put mutex
//  Global resource dependence:
//  Author:
/*****************************************************************************/
LOCAL void _DCAMERA_PutMutex(SCI_MUTEX_PTR mutex)
{
    //DCAMERA_TRACE:"_DCAMERA_PutMutex"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_570_112_2_18_1_24_0_315,(uint8*)"");
    if( NULL == mutex )
    {
        //DCAMERA_TRACE:"_DCAMERA_PutMutex: Mutex is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_573_112_2_18_1_24_0_316,(uint8*)"");
        return;
    }
    SCI_PutMutex( mutex );
}

LOCAL void _DCAMERA_ConvertCoorToISPRect(LCD_ID_E               lcd_id,
                                         ISP_RECT_T             *src_ptr,
                                         ISP_RECT_T             *dst_ptr,
                                         DCAMERA_ROTATION_MODE_E rotate_ang)
{
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(lcd_id, &lcd_info);

    //DCAMERA_TRACE:"_DCAMERA_DcamRect2ISPRect,src x y w h {%d %d %d %d},rotate_ang %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_593_112_2_18_1_24_0_317,(uint8*)"ddddd", src_ptr->x, src_ptr->y, src_ptr->w, src_ptr->h,rotate_ang);

    switch(rotate_ang)
    {
        case DCAMERA_ROTATION_0:

            dst_ptr->x = (uint32)src_ptr->x;
            dst_ptr->y = (uint32)src_ptr->y;
            dst_ptr->w = (uint32)src_ptr->w;
            dst_ptr->h = (uint32)src_ptr->h;

        break;

        case DCAMERA_ROTATION_90:

            dst_ptr->w = (uint32)src_ptr->h;
            dst_ptr->h = (uint32)src_ptr->w;
            dst_ptr->y = (uint32)src_ptr->x;
            dst_ptr->x = (uint32)(lcd_info.lcd_width - src_ptr->h - src_ptr->y);


        break;

        case DCAMERA_ROTATION_180:

            dst_ptr->x = (uint32)(lcd_info.lcd_width - src_ptr->w - src_ptr->x);
            dst_ptr->y = (uint32)(lcd_info.lcd_height - src_ptr->h - src_ptr->y);
            dst_ptr->w = (uint32)src_ptr->w;
            dst_ptr->h = (uint32)src_ptr->h;

        break;

        case DCAMERA_ROTATION_270:

            dst_ptr->w = (uint32)src_ptr->h;
            dst_ptr->h = (uint32)src_ptr->w;
            dst_ptr->x = (uint32)src_ptr->y;
            dst_ptr->y = (uint32)(lcd_info.lcd_height - src_ptr->w - src_ptr->x);

        break;

        case DCAMERA_ROTATION_MIRROR:

            dst_ptr->x = (uint32)(lcd_info.lcd_width - src_ptr->w - src_ptr->x);
            dst_ptr->y = (uint32)src_ptr->y;
            dst_ptr->w = (uint32)src_ptr->w;
            dst_ptr->h = (uint32)src_ptr->h;

        break;

        case DCAMERA_ROTATION_FLIP:

            dst_ptr->x = (uint32)src_ptr->x;
            dst_ptr->y = (uint32)(lcd_info.lcd_height - src_ptr->h - src_ptr->y);
            dst_ptr->w = (uint32)src_ptr->w;
            dst_ptr->h = (uint32)src_ptr->h;

        break;

        default:

            //DCAMERA_TRACE:"_DCAMERA_DcamRect2ISPRect,wrong angle"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_654_112_2_18_1_24_0_318,(uint8*)"");

        break;

    }


    //DCAMERA_TRACE:"_DCAMERA_DcamRect2ISPRect,dst x y w h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_665_112_2_18_1_24_0_319,(uint8*)"dddd", dst_ptr->x, dst_ptr->y, dst_ptr->w, dst_ptr->h);

    return;

}

LOCAL void _DCamera_AdjustRect(ISP_RECT_T* p_src_rect,
                               ISP_RECT_T* p_inva_rect, //the rect to be invalidte
                               ISP_RECT_T* p_img_rect,  //the rect to put image
                               uint32      display_mode)
{
    uint32  width_src   = p_src_rect->w;
    uint32  height_src  = p_src_rect->h;
    uint32  width_disp  = p_img_rect->w;
    uint32  height_disp  = p_img_rect->h;

    switch(display_mode)
    {
        case DCAMERA_DISP_FULLVIEW:

            //DCAMERA_TRACE:"_DCamera_AdjustRect: fullview"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_683_112_2_18_1_24_0_320,(uint8*)"");

            if(width_src > 0)
            {
                if((uint32)(width_src * height_disp) > (uint32)(width_disp * height_src))
                {
                    p_img_rect->h = (uint32)((height_src * width_disp) / width_src);
                    p_img_rect->h = DCAMERA_HEIGHT(p_img_rect->h);
                    p_img_rect->y = p_img_rect->y + ((height_disp - p_img_rect->h) >> 1);
                    p_img_rect->y = DCAMERA_HEIGHT(p_img_rect->y);
                    p_img_rect->x = DCAMERA_WIDTH(p_img_rect->x);
                    p_img_rect->w = DCAMERA_WIDTH(p_img_rect->w);
                }
                else
                {
                    p_img_rect->w = (uint32)((width_src * height_disp) /height_src);
                    p_img_rect->w = DCAMERA_WIDTH(p_img_rect->w);
                    p_img_rect->x = p_img_rect->x + ((width_disp - p_img_rect->w) >> 1);
                    p_img_rect->x = DCAMERA_WIDTH(p_img_rect->x);
                    p_img_rect->y = DCAMERA_HEIGHT(p_img_rect->y);
                    p_img_rect->h = DCAMERA_HEIGHT(p_img_rect->h);
                }

            }

            break;

        case DCAMERA_DISP_FULLSCREEN:

            //DCAMERA_TRACE:"_DCamera_AdjustRect: fullscreen "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_713_112_2_18_1_24_0_321,(uint8*)"");

            if((uint32)(width_src * p_inva_rect->h) > (uint32)(height_src * p_inva_rect->w))
            {
                // ò???ê???óòμ????è?a?ù×?￡???DDμè±èày??ê?
                if(p_inva_rect->h > 0)
                {
                    p_src_rect->w = (uint32)((p_inva_rect->w * height_src) / p_inva_rect->h);
                }

                p_src_rect->w = DCAMERA_WIDTH(p_src_rect->w);
                p_src_rect->x = (width_src - p_src_rect->w) >> 1;
                p_src_rect->x = p_src_rect->x & ~1;
                p_src_rect->y = p_src_rect->y & ~1;
                p_src_rect->h = DCAMERA_HEIGHT(p_src_rect->h);


            }
            else if((uint32)(width_src * p_inva_rect->h) < (uint32)(p_src_rect->h * p_inva_rect->w))
            {
                // ò???ê???óòμ??í?è?a?ù×?￡???DDμè±èày??ê?
                if(p_inva_rect->w > 0)
                {
                    p_src_rect->h = (uint32)((p_inva_rect->h * width_src) / p_inva_rect->w);
                }

                p_src_rect->h = DCAMERA_HEIGHT(p_src_rect->h);
                p_src_rect->y = (height_src - p_src_rect->h) >> 1;
                p_src_rect->y = p_src_rect->y & ~1;
                p_src_rect->x = p_src_rect->x & ~1;
                p_src_rect->w = DCAMERA_WIDTH(p_src_rect->w);
            }
            // full screen, image rect equals display range
            *p_img_rect = *p_inva_rect;

            break;

        default:

            //DCAMERA_TRACE:"_DCamera_AdjustRect: any trimming and scaling"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_753_112_2_18_1_24_0_322,(uint8*)"");

            break;

    }

    //DCAMERA_TRACE:"_DCamera_AdjustRect,trim rect x,y,w,h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_763_112_2_18_1_24_0_323,(uint8*)"dddd",p_src_rect->x,p_src_rect->y,p_src_rect->w,p_src_rect->h);

}

PUBLIC void  DCAMERA_BeforeSetting(void)
{
    if(_DCAMERA_IsVideoPhone())
    {
        ISP_ServiceStopVT();
    }
    else if(_DCAMERA_IsPreview())
    {
        ISP_ServiceStopPreview();
    }
    else if(_DCAMERA_IsPreviewEx())
    {
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        ISP_ServiceStopVTExt();
    }
    else if(_DCAMERA_IsRecord())
    {
        ISP_ServiceStopCapture();
    }

}
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_AfterSetting(void)
{
    if(_DCAMERA_IsVideoPhone())
    {
        ISP_ServiceStartVT();
    }
    else if(_DCAMERA_IsPreview())
    {
        ISP_ServiceStartPreview();
    }
    else if(_DCAMERA_IsPreviewEx())
    {
        ISP_ServiceStartVTExt();
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
    }
    else if(_DCAMERA_IsRecord())
    {
        ISP_ServiceStartCaptureMpeg();
    }

    LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);

    if(ISP_ServiceGetLastErr())
    {
        return DCAMERA_OP_ISP_ERR;
    }
    else
    {
        return DCAMERA_OP_SUCCESS;
    }
}

PUBLIC void  DCAMERA_ATV_BeforeSetting(void)
{
    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    ISP_ServiceStopVTExt();
}

PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ATV_AfterSetting(BOOLEAN is_isp_start)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;

    if(is_isp_start)
    {
        ISP_ServiceStartVTExt();

        if(ISP_ServiceGetLastErr())
        {
            ret = DCAMERA_OP_ISP_ERR;
        }
        else
        {
            ret = DCAMERA_OP_SUCCESS;
        }
    }
    else
    {
        ret = DCAMERA_OP_SUCCESS;
    }

    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    return ret;
}

/******************************************************************************/
// Description: To get sensor image format
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     SENSOR_IMAGE_FORMAT define in sensor_drv.h

// Note:
/******************************************************************************/
LOCAL SENSOR_IMAGE_FORMAT _DCAMERA_GetSensorImageFormat(void)
{
    if(DCAMERA_IsOpen())
    {
        return (s_sensor_ptr == PNULL ? SENSOR_IMAGE_FORMAT_MAX : s_sensor_ptr->image_format);
    }

    return SENSOR_IMAGE_FORMAT_MAX;
}

/******************************************************************************/
// Description: Set sensor type
// Author:     Eddy.wei
// Input:      none
// Output:     none
// Return:     0            successful
//                 others      failed
// Note:            before using DCAM device, be sure you have successful open it
/******************************************************************************/
LOCAL uint32 _DCAMERA_SetSensorType(void)
{
    //SCI_TRACE_LOW:"DCAMERA_SetSensorType: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_878_112_2_18_1_24_1_324,(uint8*)"d", s_operation_mode);

    if(DCAMERA_OP_ATV == s_operation_mode || DCAMERA_OP_ATV_FACTORY == s_operation_mode)
    {
        Sensor_SetSensorType(SENSOR_TYPE_ATV);
    }
    else
    {
        Sensor_SetSensorType(SENSOR_TYPE_IMG_SENSOR);
    }

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: init sensor
// Author:     Liangwen.zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_InitSensor(void)
{
    //SENSOR_REGISTER_INFO_T_PTR p_reg_info = Sensor_GetRegisterInfo();
    SENSOR_REGISTER_INFO_T_PTR p_reg_info = SCI_NULL;

    //  Sesnor Step 1: Select sensor type as image sensor
    _DCAMERA_SetSensorType();
    //  Sesnor Step 2: Power on, reset, open ccir clock and auto identify if the sensor is ok

    if(!Sensor_Init())
    {
        return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
    }
    //  Sesnor Step 3: Select the sensor identified by s_sensor_id
    if((DCAMERA_OP_ATV != s_operation_mode) && (DCAMERA_OP_ATV_FACTORY != s_operation_mode))
    {
        p_reg_info = Sensor_GetRegisterInfo();
        if(p_reg_info == SCI_NULL)
        {
            return DCAMERA_OP_NO_SENSOR_ERR;
        }
        else
        {
            if(s_sensor_id > SENSOR_SUB)
            {
                s_sensor_id = SENSOR_SUB;
            }

            if(p_reg_info->is_register[s_sensor_id] == SCI_FALSE)
            {
                //DCAMERA_TRACE:"_DCAMERA_InitSensor, sensor %d is not exist"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_928_112_2_18_1_24_1_325,(uint8*)"d",s_sensor_id);
                s_sensor_id = s_sensor_id == SENSOR_MAIN ? SENSOR_SUB : SENSOR_MAIN;
            }
            if(Sensor_SetCurId(s_sensor_id))
            {
                //DCAMERA_TRACE:"_DCAMERA_InitSensor, No sensor exist"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_933_112_2_18_1_24_1_326,(uint8*)"");
                return DCAMERA_OP_NO_SENSOR_ERR;
            }
        }
    }

    // Sesnor Step 4: Send initial parameter to sensor
    if(SENSOR_OP_SUCCESS != Sensor_Open())
    {
        return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
    }

    // Sesnor Step 5: Get sensor information from Sensor abstract layer
    s_sensor_ptr = Sensor_GetInfo();
    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: close sensor
// Author:     Liangwen.zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_CloseSensor(void)
{
    if(PNULL != s_sensor_ptr)
    {
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        Sensor_Close();
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        s_sensor_ptr = PNULL;
    }
    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: ISP call back function
// Author:
// Input:      none
// Output:     none
// Return:

// Note:
/******************************************************************************/
LOCAL int _DCAMERA_ISP_REG_CALLBACK(void *flag)
{
    ISP_ADDRESS_T *frame_addr_ptr;
    //DCAMERA_TRACE:"DC: ISP output one frame in capture at %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_976_112_2_18_1_24_1_327,(uint8*)"d",SCI_GetTickCount());

    frame_addr_ptr = (ISP_ADDRESS_T *)flag;

    //DCAMERA_TRACE:"DC: yaddr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_981_112_2_18_1_24_1_328,(uint8*)"ddd",frame_addr_ptr->yaddr, frame_addr_ptr->uaddr, frame_addr_ptr->vaddr);

    if(SCI_INVALID_BLOCK_ID == s_dc_task_id || PNULL == s_dc_info_ptr)
        return  SCI_SUCCESS;

    s_dc_info_ptr->isp_frame.yaddr = frame_addr_ptr->yaddr;
    s_dc_info_ptr->isp_frame.uaddr = frame_addr_ptr->uaddr;
    s_dc_info_ptr->isp_frame.vaddr = frame_addr_ptr->vaddr;

    s_dc_operation_flag |= DCAMERA_OP_ISP_CAPTURE_FRAME_FLAG;

    _DCAMERA_CaptureDoneNotice();
    //DCAMERA_TRACE:"DC: Exit ISP CAPTURE FRAME callback at %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_993_112_2_18_1_24_1_329,(uint8*)"d",SCI_GetTickCount());

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: ISP scale call back function
// Author:
// Input:      none
// Output:     none
// Return:

// Note:
/******************************************************************************/
LOCAL int _DCAMERA_ISP_SCALE_CALLBACK(void *flag)
{
    s_dc_operation_flag |= DCAMERA_OP_ISP_SCALE_FRAME_FLAG;
    //DCAMERA_TRACE:"_DCAMERA_ISP_SCALE_CALLBACK: FLAG 0x%08x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1008_112_2_18_1_24_1_330,(uint8*)"d", (uint32)flag);

    return 0;
}

LOCAL void _DCAMERA_Get_UPCC_CTRL(void)
{
    if(SCI_NULL == s_upcc_mutx_ptr)
    {
        s_upcc_mutx_ptr =  SCI_CreateMutex("DC_UPCC_CALLBACK_MUTEX", SCI_INHERIT);
    }

    if( SCI_NULL == s_upcc_mutx_ptr )
        return;

    SCI_GetMutex(s_upcc_mutx_ptr, SCI_WAIT_FOREVER);
    return;
}


LOCAL void _DCAMERA_Release_UPCC_CTRL(void)
{
    if( SCI_NULL == s_upcc_mutx_ptr )
        return;
     SCI_PutMutex( s_upcc_mutx_ptr );
    return;
}



/******************************************************************************/
// Description: record callback
// Author:     Feifei.Gu
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/


LOCAL int _DCAMERA_RecordCallback ( void* address )
{
    ISP_FRAME_T             *frame_addr_ptr = (ISP_FRAME_T *)address;
    JPEG_ENC_RET_E          jpeg_ret =  JPEG_ENC_OK;
    uint32                  size;
    JPEG_ENC_IN_PARAM_T     in_param = {0};
    JPEG_ENC_OUT_PARAM_T    out_param = {0};

    if(PNULL == s_upcc_dcam_ptr)
    {
        ISP_ServiceFreeFrameAddress((void*)frame_addr_ptr);
        return SCI_TRUE;

    }

    _DCAMERA_Get_UPCC_CTRL(); //protect critical code

    in_param.image_rect.x = 0;
    in_param.image_rect.y = 0;

    in_param.image_rect.w = record_param.target_size.w;
    in_param.image_rect.h = record_param.target_size.h;

    in_param.quality_level = record_param.enc_quality_level;
    in_param.jpeg_buf_ptr   = s_upcc_dcam_ptr;
    in_param.jpeg_buf_size  =DCAMERA_UPCC_JPEG_SIZE;

    in_param.yuv_addr.y_chn_addr = (uint32)frame_addr_ptr->yaddr;
    in_param.yuv_addr.u_chn_addr = (uint32)frame_addr_ptr->uaddr;
    in_param.yuv_addr.v_chn_addr = (uint32)frame_addr_ptr->vaddr;

    jpeg_ret = JPEG_EncodeJpeg(&in_param, &out_param);
    if(JPEG_ENC_OK != jpeg_ret )
    {
        //SCI_TRACE_LOW:"UPCC: JPEG ENCODE ERR!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1081_112_2_18_1_24_1_331,(uint8*)"");
    }

    ISP_ServiceFreeFrameAddress((void*)frame_addr_ptr);//for 8801H, the parameter is the address of the structure ISP_FRAME_T

    size = out_param.out_size;
    if(0==size || JPEG_ENC_OK != jpeg_ret)
    {
        //SCI_TRACE_LOW:"UPCC_Camera.c->UPCC_GetAndSendFrame(): one frame lost."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1089_112_2_18_1_24_1_332,(uint8*)"");
        _DCAMERA_Release_UPCC_CTRL();
        return DCAMERA_OP_ENCODE_ERR;
    }

    if(PNULL != s_dc_record_callback)
    {
        (s_dc_record_callback)(SCI_SUCCESS, (void*)s_upcc_dcam_ptr, (uint32)size);
    }
    else
    {
        //DCAMERA_TRACE:"DCAMERA_app.c->DCAMERA_RecordCallback()  NO call back !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1100_112_2_18_1_24_1_333,(uint8*)"");
    }

    _DCAMERA_Release_UPCC_CTRL();

    return SCI_TRUE;
}

LOCAL int _DCAMERA_ImageRecivedCallback(void* address )
{
    if(SCI_NULL == s_dc_task_id ||
       SCI_INVALID_BLOCK_ID == s_dc_task_id ||
       DCAMERA_QUEUE_FEW_NUM >= _DCAMERA_GetQueueAvailable(s_dc_task_id) ||
       SCI_FALSE == _DCAMERA_IsPreviewEx())
    {
        ISP_ServiceFreeFrameAddress((ISP_FRAME_T*)address);
    }
    else
    {
        SCI_MEMCPY((void*)s_dcamera_info.p_prev_frm_cur, (void*)address, sizeof(ISP_FRAME_T));
        s_dcamera_info.p_prev_frm_cur->fid = (uint32)address;

        DCAMERA_SendSignal(DCAMERA_TASK_CMD_SCALE, 0, (void*)s_dcamera_info.p_prev_frm_cur, SCI_NULL);
        s_dcamera_info.p_prev_frm_cur ++;
        if(s_dcamera_info.p_prev_frm_cur > &s_dcamera_info.isp_preview_frame[DC_PREVIEW_FRAME_BUF_MAX - 1])
        {
            s_dcamera_info.p_prev_frm_cur = &s_dcamera_info.isp_preview_frame[0];
        }
    }

    return SCI_TRUE;
}

LOCAL  DCAMERA_RETURN_VALUE_E  _DCAMERA_WaitForCaptureDone(void)
{
    uint32                  status = DCAMERA_OP_SUCCESS;

    if(s_capture_done_sema_ptr)
    {
        status = SCI_GetSemaphore(s_capture_done_sema_ptr, DCAMERA_TIMEOUT_INTERVAL);
        if(status)  status = DCAMERA_OP_ISP_ERR;
    }

    return status;
}

LOCAL  void  _DCAMERA_CaptureDoneNotice(void)
{
    if(s_capture_done_sema_ptr)
    {
        SCI_PutSemaphore(s_capture_done_sema_ptr);
    }
    return ;
}

LOCAL int _DCAMERA_DoSnapShotExDecJpgFrame(void)
{
    JINF_DEC_IN_PARAM_T     jpeg_in={0};
    JINF_DEC_OUT_PARAM_T    jpeg_out={0};
    DCAMERA_RECT_T          dc_rect = {0};
    DCAMERA_SIZE_T          dc_dst_size = {0};
    JINF_RET_E              jpg_rtn = 0;

    if(0 == s_dc_info_ptr->jpg_dec_target_size.w ||
       0 == s_dc_info_ptr->jpg_dec_target_size.h)
    {
        return DCAMERA_OP_SUCCESS;
    }

    if(SCI_NULL == s_dc_info_ptr ||
       SCI_NULL == s_sensor_ptr)
    {
        return DCAMERA_OP_PARAM_ERR;
    }

    //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapShotExDecJpgFrame, snapshot_ex_yuv  0x%x  snapshot_ex_addi_mem 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1177_112_2_18_1_24_1_334,(uint8*)"dd",s_dc_info_ptr->snapshot_ex_yuv,s_dc_info_ptr->snapshot_ex_addi_mem);

    if(SCI_NULL == s_dc_info_ptr->snapshot_ex_yuv)
    {
        if(SCI_NULL == s_dc_info_ptr->snapshot_ex_addi_mem)
        {
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapShotExDecJpgFrame, s_dc_info_ptr->snapshot_ex_yuv is NULL! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1183_112_2_18_1_24_1_335,(uint8*)"");
            return jpg_rtn;
        }
        else
        {
            jpeg_in.target_buf_ptr = (uint8 *)DC_ADDR(s_dc_info_ptr->snapshot_ex_addi_mem);
            jpeg_in.target_buf_size= s_dc_info_ptr->snapshot_ex_addi_mem_len-
                (((uint32)s_dc_info_ptr->snapshot_ex_addi_mem) & (DCAMERA_PADDING_MEM_SIZE - 1)) ;
        }

    }
    else
    {
        jpeg_in.target_buf_ptr = (uint8 *)DC_ADDR(s_dc_info_ptr->snapshot_ex_yuv);
        jpeg_in.target_buf_size = s_dc_info_ptr->snapshot_ex_yuv_len -
            (((uint32)s_dc_info_ptr->snapshot_ex_yuv) & (DCAMERA_PADDING_MEM_SIZE - 1)) ;
    }
    jpeg_in.jpeg_buf_ptr = (uint8 *)s_dc_info_ptr->jpeg_buf;
    jpeg_in.jpeg_buf_size = s_dc_info_ptr->buf_len;
    dc_dst_size.w = s_dc_info_ptr->jpg_dec_target_size.w;//s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
    dc_dst_size.h = s_dc_info_ptr->jpg_dec_target_size.h;//s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;
    jpeg_in.decode_buf_ptr = (uint8*)DCAMERA_CaptureGetYUVBuf(&jpeg_in.decode_buf_size);
    jpeg_in.target_data_type = JINF_DATA_TYPE_YUV;
    jpeg_in.read_file_func = PNULL;
    jpeg_in.async_callback = PNULL;

    jpeg_in.target_width = dc_dst_size.w;
    jpeg_in.target_height = dc_dst_size.h;
    if(s_dc_info_ptr->preview_ctl_info.zoom_level)
    {
        dc_rect.w = dc_dst_size.w;
        dc_rect.h = dc_dst_size.h;
        _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);

        jpeg_in.crop_rect.x = dc_rect.x;
        jpeg_in.crop_rect.y = dc_rect.y;
        jpeg_in.crop_rect.w = dc_rect.w;
        jpeg_in.crop_rect.h = dc_rect.h;
    }

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    if(_DCAMERA_CloseISP())
    {
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        return DCAMERA_OP_ERROR;
    }

    jpg_rtn = IMGJPEG_Decode(&jpeg_in, &jpeg_out);

    s_dc_info_ptr->isp_frame.yaddr = (uint32)jpeg_out.out_data_info.yuv.yuv_chn.y_chn_ptr;
    s_dc_info_ptr->isp_frame.uaddr = (uint32)jpeg_out.out_data_info.yuv.yuv_chn.u_chn_ptr;
    s_dc_info_ptr->isp_frame.vaddr = (uint32)jpeg_out.out_data_info.yuv.yuv_chn.v_chn_ptr;
    s_dc_info_ptr->isp_frame.width = jpeg_out.output_width;
    s_dc_info_ptr->isp_frame.height= jpeg_out.output_height;

    if(_DCAMERA_OpenISP())
    {
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        return DCAMERA_OP_ERROR;
    }

    _DCAMERA_PutMutex(s_callback_mutex_ptr);
    return jpg_rtn;
}

LOCAL int _DCAMERA_DoSnapShotDecJpgFrame(DCAMERA_SNAPSHOT_PARAM_T *param_ptr)
{
    int                     rVal            = 0;
    JPEG_DEC_IN_PARAM_T     JpgDecInPara    = {0};
    JPEG_DEC_OUT_PARAM_T    JpgDecOutPara   = {0};
    void*                   mem_start   = 0;
    uint32                  mem_size    = 0;

    //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapShotDecJpgFrame Entry Time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1257_112_2_18_1_24_1_336,(uint8*)"d",                                      SCI_GetTickCount());

    JpgDecInPara.jpeg_buf   = (uint32 *)s_dc_info_ptr->jpeg_buf;
    JpgDecInPara.jpeg_size  = s_dc_info_ptr->buf_len;

    JpgDecInPara.dct_mode   = JPEG_DCT_NORMAL;
    JpgDecInPara.dec_data_type  = JPEG_DEC_DATA_TYPE_YUV422;

    if(DCAMERA_ROTATION_0 == param_ptr->rotation_mode)
    {
        s_dc_info_ptr->isp_frame.width = VGA_WIDTH;
        s_dc_info_ptr->isp_frame.height = VGA_HEIGHT;
        mem_start = DCAMERA_CaptureGetYUVBuf(&mem_size);
        JpgDecInPara.p_jpg_tmp = DCAMERA_CaptureGetJPGTmpBuf(&JpgDecInPara.jpg_tmp_len);
    }
    else
    {
        s_dc_info_ptr->isp_frame.width = s_dc_info_ptr->jpeg_rect.w;
        s_dc_info_ptr->isp_frame.height = s_dc_info_ptr->jpeg_rect.h;
        mem_start = DCAMERA_CaptureGetYUVRotBuf(&mem_size);
        JpgDecInPara.p_jpg_tmp = DCAMERA_CaptureGetYUVBuf(&JpgDecInPara.jpg_tmp_len);
    }
    if(SCI_NULL == mem_start || 0 == mem_size)
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    JpgDecInPara.yuv_buf    = (uint32 *)mem_start;

    JpgDecInPara.target_width = s_dc_info_ptr->isp_frame.width;
    JpgDecInPara.target_height= s_dc_info_ptr->isp_frame.height;
    JpgDecInPara.yuv_size   = (s_dc_info_ptr->isp_frame.width * s_dc_info_ptr->isp_frame.height * 2 + 0x200);

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    /* Close ISP, So JPEG Decoder can use ISP to do the scaling */
    if(_DCAMERA_CloseISP())
    {
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        return DCAMERA_OP_ERROR;
    }

    if(JPEG_DEC_OK != JPEG_DecodeJpeg(&JpgDecInPara, &JpgDecOutPara))
    {
        _DCAMERA_OpenISP();
        _DCAMERA_PutMutex(s_callback_mutex_ptr);

        return DCAMERA_OP_DECODE_ERR;
    }

    if(_DCAMERA_OpenISP())
    {
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        return DCAMERA_OP_ERROR;
    }

    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    s_dc_info_ptr->isp_frame.yaddr = JpgDecOutPara.data_addr.yuv_addr.y_chn_addr;
    s_dc_info_ptr->isp_frame.uaddr = JpgDecOutPara.data_addr.yuv_addr.u_chn_addr;
    s_dc_info_ptr->isp_frame.vaddr = JpgDecOutPara.data_addr.yuv_addr.v_chn_addr;
    s_dc_info_ptr->isp_frame.width = JpgDecOutPara.output_width;
    s_dc_info_ptr->isp_frame.height= JpgDecOutPara.output_height;
    //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapShotDecJpgFrame End Time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1317_112_2_18_1_24_2_337,(uint8*)"d",                                      SCI_GetTickCount());

    return rVal;
}


LOCAL int _DCAMERA_DoSnapShotReview(DCAMERA_SNAPSHOT_PARAM_T const *param_ptr,
    LCD_INFO_T *lcd_info_ptr, BOOLEAN select_personal)
{
    int                         rVal = 0;
    ISP_REVIEW_PARAM_T          review_param = {0};
    uint32                      mem_start = 0;
    uint32                      mem_size = 0;
    ISP_RECT_T                  logic_disp_rect = {0};
    ISP_RECT_T                  logic_lcdc_rect = {0};
    ISP_RECT_T                  img_src_rect = {0};
    DCAMERA_CONVERT_COOR_T      src_coor = {0};
    DCAMERA_CONVERT_COOR_T      dst_coor = {0};
    BOOLEAN                     is_datemark_en = SCI_FALSE;
/*
    uint16                      width,width_trim    = 0;
    uint16                      height,height_trim = 0;
    uint16                      zoom_w_step = 0;
    uint16                      zoom_h_step = 0;
*/
    //DCAMERA_TRACE:"DC _DCAMERA_DoSnapShotReview, start time %d  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1341_112_2_18_1_24_2_338,(uint8*)"d", SCI_GetTickCount());

    if(param_ptr->quickview_callback)
    {
        review_param.disp_range = s_dc_info_ptr->isp_preview_param.disp_range;
        review_param.lcd_rect   = s_dc_info_ptr->isp_preview_param.lcd_rect;
        review_param.disp_lcd_id = s_dc_info_ptr->isp_preview_param.disp_lcd_id;
        review_param.disp_block_id = s_dc_info_ptr->isp_preview_param.disp_block_id;
        review_param.input_size.w = s_dc_info_ptr->isp_frame.width;
        review_param.input_size.h = s_dc_info_ptr->isp_frame.height;

        if((ISP_ROTATION_270 == s_dc_info_ptr->isp_preview_param.disp_rotation ||
            ISP_ROTATION_90 == s_dc_info_ptr->isp_preview_param.disp_rotation) &&
           select_personal)
        {
            src_coor.rot_degree = 0;    // 0: normal; 1: 90; 2: 180; 3:270;
        }
        else
        {
            src_coor.rot_degree =(DCAMERA_ROTATION_MODE_E)s_dc_info_ptr->isp_preview_param.disp_rotation;
        }
        src_coor.rect.x = (uint16)review_param.disp_range.x;
        src_coor.rect.y = (uint16)review_param.disp_range.y;
        src_coor.rect.w = (uint16)review_param.disp_range.w;
        src_coor.rect.h = (uint16)review_param.disp_range.h;
        DCAMERA_Convert_Coor(review_param.disp_lcd_id, &src_coor, &dst_coor);
        logic_disp_rect.x = (uint32)dst_coor.rect.x;
        logic_disp_rect.y = (uint32)dst_coor.rect.y;
        logic_disp_rect.w = (uint32)dst_coor.rect.w;
        logic_disp_rect.h = (uint32)dst_coor.rect.h;

        src_coor.rect.x = (uint16)review_param.lcd_rect.x;
        src_coor.rect.y = (uint16)review_param.lcd_rect.y;
        src_coor.rect.w = (uint16)review_param.lcd_rect.w;
        src_coor.rect.h = (uint16)review_param.lcd_rect.h;
        DCAMERA_Convert_Coor(review_param.disp_lcd_id, &src_coor, &dst_coor);
        logic_lcdc_rect.x = (uint32)dst_coor.rect.x;
        logic_lcdc_rect.y = (uint32)dst_coor.rect.y;
        logic_lcdc_rect.w = (uint32)dst_coor.rect.w;
        logic_lcdc_rect.h = (uint32)dst_coor.rect.h;

        img_src_rect.w = review_param.input_size.w;
        img_src_rect.h = review_param.input_size.h;
         _DCamera_AdjustRect(&img_src_rect,
                            &logic_lcdc_rect,
                            &logic_disp_rect,
                            s_dc_info_ptr->display_mode);
#if 0
        width  = review_param.input_size.w;
        height = review_param.input_size.h;

        //DCAMERA_TRACE:"DC _DCAMERA_DoSnapShotReview , disp_rect {%d %d}, src {%d %d}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1393_112_2_18_1_24_2_339,(uint8*)"dddd", s_dc_info_ptr->disp_rect.w,s_dc_info_ptr->disp_rect.h,width,height);

        width_trim = width;
        height_trim = height;
        if(s_dc_info_ptr->display_mode == DCAMERA_DISP_FULLSCREEN)
        {
            if((width * s_dc_info_ptr->disp_rect.h) > (height * s_dc_info_ptr->disp_rect.w))
            {
                width_trim = s_dc_info_ptr->disp_rect.w * height / s_dc_info_ptr->disp_rect.h;
            }
            else if((width * s_dc_info_ptr->disp_rect.h) < (height * s_dc_info_ptr->disp_rect.w))
            {
                height_trim = s_dc_info_ptr->disp_rect.h * width / s_dc_info_ptr->disp_rect.w;
             }

        }

        zoom_w_step = ZOOM_STEP(width_trim);
        zoom_w_step &= ~1;
        zoom_h_step = ZOOM_STEP(height_trim);
        zoom_h_step &= ~1;

        width_trim  = (uint16)(zoom_w_step * s_dc_info_ptr->preview_ctl_info.zoom_level);
        height_trim = (uint16)(zoom_h_step * s_dc_info_ptr->preview_ctl_info.zoom_level);

        //DCAMERA_TRACE:"DC _DCAMERA_DoSnapShotReview , zoom %d , trim {%d %d}"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1419_112_2_18_1_24_2_340,(uint8*)"ddd", s_dc_info_ptr->preview_ctl_info.zoom_level,width_trim,height_trim);

        img_src_rect.x += (uint32)(width_trim >> 1);
        img_src_rect.y += (uint32)(height_trim >> 1);

        // Make sure that x offset and y offset of input image multiple of 4
        img_src_rect.x = img_src_rect.x & ~1;
        img_src_rect.y = img_src_rect.y & ~1;

        img_src_rect.w = review_param.input_size.w  - (img_src_rect.x << 1);
        img_src_rect.h = review_param.input_size.h - (img_src_rect.y << 1);

        // Make sure that height and width of input image multiple of 4
        img_src_rect.w = DCAMERA_WIDTH(img_src_rect.w);
        img_src_rect.h = DCAMERA_HEIGHT(img_src_rect.h);
#endif

        // find the difference between the JPG and YUV
        if(SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
        {
            review_param.input_format = ISP_IMAGE_FORMAT_YUV420;
        }
        else
        {
            review_param.input_format = ISP_IMAGE_FORMAT_YUV422;
        }

        review_param.input_addr.yaddr = s_dc_info_ptr->isp_frame.yaddr;
        review_param.input_addr.uaddr = s_dc_info_ptr->isp_frame.uaddr;

        review_param.disp_rotation = (ISP_ROTATION_E)src_coor.rot_degree;
        review_param.input_range.x = (uint32)img_src_rect.x;
        review_param.input_range.y = (uint32)img_src_rect.y;
        review_param.input_range.w = (uint32)img_src_rect.w;
        review_param.input_range.h = (uint32)img_src_rect.h;



        review_param.disp_range = s_dc_info_ptr->isp_preview_param.disp_range;
        review_param.lcd_rect = s_dc_info_ptr->isp_preview_param.lcd_rect;
        mem_start = (uint32)DCAMERA_CaptureGetQuickViewBuf(&mem_size);

        _DCAMERA_GetMutex(s_callback_mutex_ptr);

        ISP_ServiceMemConfig((void*)mem_start, mem_size);
        ISP_ServiceSetReviewParam(&review_param);

        //if date mark enabled, handle the date and register the display_callback
        is_datemark_en = b_burst_snapshot?(s_snapshot_burst_param.capture_date.eb):(s_snapshot_param.capture_date.eb);
        if(is_datemark_en)
        {
            ISP_ServiceRegister(ISP_SERVICE_CALLBACK_DISPLAY, (int(*)())_DCAMERA_reviewDateCallback);
        }
        else
        {
            ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_DISPLAY);
        }

        ISP_ServiceStartReview();

        if(is_datemark_en)
        {
            _DCAMERA_WaitForCaptureDone();
        }

        _DCAMERA_PutMutex(s_callback_mutex_ptr);
    }

    //DCAMERA_TRACE:"DC _DCAMERA_DoSnapShotReview end, end time %d  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1483_112_2_18_1_24_2_341,(uint8*)"d", SCI_GetTickCount());

    return rVal;
}

LOCAL int _DCAMERA_DoSnapShotRotation(DCAMERA_SNAPSHOT_PARAM_T const *param_ptr,
    uint32 image_size, BOOLEAN select_personal)
{
    int                      ret = 0;
    uint32                   mem_size = 0;
    DCAMERA_ROTATION_PARAM_T rot_param = {0};
    DCAMERA_MEM_PARAM_T      mem_param_t = {0};

/*    DCAMERA_TRACE("DC _DCAMERA_DoSnapShotRotation, start time %d  ", SCI_GetTickCount());

    if(ISP_ROTATION_0 != s_dc_info_ptr->isp_preview_param.disp_rotation)
    {
        if(((DCAMERA_ROTATION_0 != param_ptr->rotation_mode) && (!select_personal))||
           ((DCAMERA_ROTATION_0 ==  param_ptr->rotation_mode)&& (select_personal)))
*/
        if(DCAMERA_ROTATION_0 != param_ptr->rotation_mode)
        {

            //DCAMERA_TRACE:"DC _DCAMERA_DoSnapShotRotation, need rotation"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1499_112_2_18_1_24_2_342,(uint8*)"");

            rot_param.src_chn_addr.y_chn_addr = s_dc_info_ptr->isp_frame.yaddr;
            rot_param.src_chn_addr.u_chn_addr = s_dc_info_ptr->isp_frame.uaddr;
            rot_param.image_width    = (uint32)s_dc_info_ptr->isp_frame.width;
            rot_param.image_height   = (uint32)s_dc_info_ptr->isp_frame.height;
            image_size = rot_param.image_width * rot_param.image_height;
            if(select_personal)
            {
                mem_param_t.review_width = s_dc_info_ptr->target_rect.w;
                mem_param_t.review_height = s_dc_info_ptr->target_rect.h;
                mem_param_t.review_rot = s_dc_info_ptr->preview_ctl_info.rotation_mode;
                mem_param_t.b_personal_cap = 1;
                rot_param.dest_chn_addr.y_chn_addr = (uint32)DCAMERA_PreviewGetMem(&mem_param_t,&mem_size);
            }
            else
            {
                rot_param.dest_chn_addr.y_chn_addr = (uint32)DCAMERA_CaptureGetYUVBuf(&mem_size);
            }
            rot_param.dest_chn_addr.y_chn_addr = DC_ADDR(rot_param.dest_chn_addr.y_chn_addr);
            rot_param.dest_chn_addr.u_chn_addr = rot_param.dest_chn_addr.y_chn_addr + image_size;
            rot_param.dest_chn_addr.u_chn_addr = DC_ADDR(rot_param.dest_chn_addr.u_chn_addr);

            if(DCAMERA_ROTATION_0 !=  param_ptr->rotation_mode)
            {
                rot_param.rotation_mode = param_ptr->rotation_mode;
            }
            else
            {
                if(ISP_ROTATION_270 == s_dc_info_ptr->isp_preview_param.disp_rotation)
                {
                    rot_param.rotation_mode = DCAMERA_ROTATION_90;
                }
                else
                {
                    rot_param.rotation_mode = DCAMERA_ROTATION_270;
                }
            }

            if(SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
            {
                _DCAMERA_RotateYUV420(&rot_param);
            }
            else
            {
                _DCAMERA_RotateYUV422(&rot_param);
            }

            s_dc_info_ptr->isp_frame.yaddr = rot_param.dest_chn_addr.y_chn_addr;
            s_dc_info_ptr->isp_frame.uaddr = rot_param.dest_chn_addr.u_chn_addr;

            if(DCAMERA_ROTATION_90 ==  rot_param.rotation_mode ||
               DCAMERA_ROTATION_270 ==  rot_param.rotation_mode)
            {
                s_dc_info_ptr->isp_frame.width = rot_param.image_height;
                s_dc_info_ptr->isp_frame.height = rot_param.image_width;
            }

        }
/*    }

    //DCAMERA_TRACE:"DC _DCAMERA_DoSnapShotRotation end, end time %d  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1559_112_2_18_1_24_2_343,(uint8*)"d", SCI_GetTickCount());
*/
    return ret;
}

/*lint -e{529}*/
LOCAL int _DCAMERA_DoSnapShotJpgEnc(JPEG_ENC_IN_PARAM_T            *pJpgEncInPara,
                                    JPEG_ENC_OUT_PARAM_T           *pJpgEncOutPara,
                                    DCAMERA_SNAPSHOT_PARAM_T const *param_ptr,
                                    uint8                           *malloc_buffer_ptr,
                                    uint32                          malloc_buffer_len)
{
    DC_PRODUCT_CFG_T_PTR p_dc_cfg = DC_GeProductCfgPtr();
    JINF_ENC_IN_PARAM_T  jpeg_in  = {0};
    JINF_ENC_OUT_PARAM_T jpeg_out = {0};
    uint32               compress_level = param_ptr->enc_quality_level;
    DCAMERA_SIZE_T       dc_dst_size = {0};
    DCAMERA_SIZE_T       dc_size = {0};
    DCAMERA_RECT_T       dc_rect = {0};
    ISP_SCLAE_PARAM_T    isp_scale_param = {0};
    uint32               mem_size = 0;
    uint32               is_frame_enc = 0;
    DCAMERA_CAPTURE_DATE_T  date_mark = {0};

    dc_size.w = s_dc_info_ptr->isp_frame.width;
    dc_size.h = s_dc_info_ptr->isp_frame.height;
    dc_rect.w = dc_size.w;
    dc_rect.h = dc_size.h;
    dc_dst_size.w = s_dc_info_ptr->jpeg_rect.w;
    dc_dst_size.h = s_dc_info_ptr->jpeg_rect.h;
    // _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);//for is has been rotated before now


    if(SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
    {
        pJpgEncInPara->enc_data_type = JPEG_ENC_DATA_TYPE_YUV420;
    }
    else
    {
        pJpgEncInPara->enc_data_type = JPEG_ENC_DATA_TYPE_YUV422;
    }
    pJpgEncInPara->enc_slice_height = DC_SLICE_HEIGHT;


#if 0
    if(s_dc_info_ptr->preview_ctl_info.target_size < DCAMERA_PHOTO_SIZE_240X320)
    {
        uint32 i = 0, src_y = 0, src_uv = 0, dst_y = 0, dst_uv = 0, y_end_addr = 0;

		SCI_TRACE_LOW(" david capture target size < 240 320");
        // small size ,not use slice encode;
        isp_scale_param.input_size.w = (uint32)dc_size.w;
        isp_scale_param.input_size.h = (uint32)dc_size.h;
        isp_scale_param.input_rect.x = (uint32)dc_rect.x;
        isp_scale_param.input_rect.y = (uint32)dc_rect.y;
        isp_scale_param.input_rect.w = (uint32)dc_rect.w;
        isp_scale_param.input_rect.h = (uint32)dc_rect.h;
        isp_scale_param.output_size.w= s_dc_info_ptr->jpeg_rect.w;
        isp_scale_param.output_size.h= s_dc_info_ptr->jpeg_rect.h;
        isp_scale_param.scale_mode   = ISP_SERVICE_SCALE_NORMAL;
        if(SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
        {
            isp_scale_param.input_format = ISP_IMAGE_FORMAT_YUV420;
        }
        else
        {
            isp_scale_param.input_format = ISP_IMAGE_FORMAT_YUV422;
        }
        isp_scale_param.input_addr.yaddr = s_dc_info_ptr->isp_frame.yaddr;
        isp_scale_param.input_addr.uaddr = s_dc_info_ptr->isp_frame.uaddr;
        isp_scale_param.output_addr.yaddr = (uint32)DCAMERA_CaptureGetLineBuf(&mem_size);
        mem_size = s_dc_info_ptr->jpeg_rect.w * s_dc_info_ptr->jpeg_rect.h;
        isp_scale_param.output_addr.uaddr = isp_scale_param.output_addr.yaddr + mem_size;
        isp_scale_param.output_addr.uaddr = DC_ADDR(isp_scale_param.output_addr.uaddr);
        isp_scale_param.output_format = ISP_IMAGE_FORMAT_YUV422;
        src_y  = isp_scale_param.output_addr.yaddr  + s_dc_info_ptr->jpeg_rect.w * (s_dc_info_ptr->jpeg_rect.h - 1);
        src_uv = isp_scale_param.output_addr.uaddr  + s_dc_info_ptr->jpeg_rect.w * (s_dc_info_ptr->jpeg_rect.h - 1);
        y_end_addr = s_dc_info_ptr->isp_frame.yaddr + s_dc_info_ptr->jpeg_rect.w * s_dc_info_ptr->jpeg_rect.h;
        dst_y  = y_end_addr - s_dc_info_ptr->jpeg_rect.w;
        dst_uv = y_end_addr + s_dc_info_ptr->jpeg_rect.w * (s_dc_info_ptr->jpeg_rect.h - 1);
        ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, SCI_NULL);
        ISP_ServiceSetScaleParam(&isp_scale_param);
        ISP_ServiceStartScale();

        for(i=0; i<s_dc_info_ptr->jpeg_rect.h; i++)
        {
            SCI_MEMCPY((void*)dst_uv, (void*)src_uv, s_dc_info_ptr->jpeg_rect.w);
            src_uv -= s_dc_info_ptr->jpeg_rect.w;
            dst_uv -= s_dc_info_ptr->jpeg_rect.w;

            SCI_MEMCPY((void*)dst_y, (void*)src_y, s_dc_info_ptr->jpeg_rect.w);
            src_y  -= s_dc_info_ptr->jpeg_rect.w;
            dst_y  -= s_dc_info_ptr->jpeg_rect.w;
        }

        s_dc_info_ptr->isp_frame.width  = s_dc_info_ptr->jpeg_rect.w;
        s_dc_info_ptr->isp_frame.height = s_dc_info_ptr->jpeg_rect.h;
        s_dc_info_ptr->isp_frame.yaddr  = dst_y  + s_dc_info_ptr->jpeg_rect.w;
        s_dc_info_ptr->isp_frame.uaddr  = dst_uv + s_dc_info_ptr->jpeg_rect.w;

        is_frame_enc = 1; // !!! here,just for entering frame encode,

        pJpgEncInPara->enc_data_type = JPEG_ENC_DATA_TYPE_YUV422;
    }
#endif


	s_dc_info_ptr->isp_frame.width         = p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->width;
	s_dc_info_ptr->isp_frame.height        = p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->height;
	s_dc_info_ptr->jpeg_rect.w             = p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->width;
	s_dc_info_ptr->jpeg_rect.h             = p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->height;
	s_dc_info_ptr->snapshot_ctl_info.param = SNAPSHOT_YUV422;
	pJpgEncInPara->reserved                = p_Dcam_cfg[DCAMERA_MODE_SNAPSHOT]->enc_rot;

    if(((s_dc_info_ptr->isp_frame.width == s_dc_info_ptr->jpeg_rect.w &&
          s_dc_info_ptr->isp_frame.height == s_dc_info_ptr->jpeg_rect.h) &&
         (s_dc_info_ptr->snapshot_ctl_info.param == SNAPSHOT_YUV422))||
       is_frame_enc == 1)
//       (s_dc_info_ptr->preview_ctl_info.zoom_level == 0 || is_frame_enc == 1))
    {
    	SCI_TRACE_LOW(" DC:jpeg_buf:0x%x malloc_buf:0x%x", pJpgEncInPara->jpeg_buf_ptr, malloc_buffer_ptr);

        // set jpeg encode information
        pJpgEncInPara->image_rect.x = 0;
        pJpgEncInPara->image_rect.y = 0;
        pJpgEncInPara->image_rect.w = s_dc_info_ptr->isp_frame.width;
        pJpgEncInPara->image_rect.h = s_dc_info_ptr->isp_frame.height;
        pJpgEncInPara->quality_level= param_ptr->enc_quality_level;

        pJpgEncInPara->yuv_addr.y_chn_addr = (uint32)(s_dc_info_ptr->isp_frame.yaddr);
        pJpgEncInPara->yuv_addr.u_chn_addr = (uint32)(s_dc_info_ptr->isp_frame.uaddr);
        pJpgEncInPara->yuv_addr.v_chn_addr = (uint32)(s_dc_info_ptr->isp_frame.vaddr);

        if(NULL != malloc_buffer_ptr)
        {
            pJpgEncInPara->jpeg_buf_ptr     = malloc_buffer_ptr;
            pJpgEncInPara->jpeg_buf_size    = malloc_buffer_len;
        }
        else
        {
            pJpgEncInPara->jpeg_buf_ptr     = param_ptr->data_buf;
            pJpgEncInPara->jpeg_buf_size    = param_ptr->buf_len;
        }

        SCI_TRACE_LOW(" DC:jpeg_buf:0x%x malloc_buf:0x%x",pJpgEncInPara->jpeg_buf_ptr, malloc_buffer_ptr);

#if defined(_DC_EXIF_)
        if(p_dc_cfg->exif_eb == SCI_TRUE)
        {
            pJpgEncInPara->jpeg_buf_ptr += DC_EXIF_LENGTH;
            pJpgEncInPara->jpeg_buf_size -= DC_EXIF_LENGTH;

            if((p_dc_cfg->thumbnail_eb == SCI_TRUE) &&
               (pJpgEncInPara->image_rect.w > VGA_WIDTH))
            {
                pJpgEncInPara->jpeg_buf_ptr += DC_THUMBNAIL_LENGTH;
                pJpgEncInPara->jpeg_buf_size -= DC_THUMBNAIL_LENGTH;
            }
        }
#endif

        //DCAMERA_TRACE:"DC: Call JPEG_EncodeJpeg, start time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1686_112_2_18_1_24_2_344,(uint8*)"d", SCI_GetTickCount());
        //DCAMERA_TRACE:"y_addr = 0x%x,u_addr = 0x%x,v_addr = 0x%x,jpeg_buf_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1691_112_2_18_1_24_2_345,(uint8*)"dddd",s_dc_info_ptr->isp_frame.yaddr,s_dc_info_ptr->isp_frame.uaddr,s_dc_info_ptr->isp_frame.vaddr,pJpgEncInPara->jpeg_buf_ptr);

        //if the date mark function is enabled, handle and blend the date mark to the source buffer
        date_mark = b_burst_snapshot?(s_snapshot_burst_param.capture_date):(s_snapshot_param.capture_date);

        if(date_mark.eb)
        {
            DCAMERA_RECT_T target_rect = {0};
            ISP_ADDRESS_T  target_addr = {0};
            DCAMERA_RECT_T src_date_rect = {0};
            uint32    * src_date_addr = PNULL;

            //the target image rect need to add date mark
            target_rect.x = pJpgEncInPara->image_rect.x;
            target_rect.y = pJpgEncInPara->image_rect.y;
            target_rect.w = pJpgEncInPara->image_rect.w;
            target_rect.h = pJpgEncInPara->image_rect.h;

            //set the target buffer
            target_addr.yaddr = pJpgEncInPara->yuv_addr.y_chn_addr;
            target_addr.uaddr = pJpgEncInPara->yuv_addr.u_chn_addr;
            target_addr.vaddr = pJpgEncInPara->yuv_addr.v_chn_addr;

            src_date_addr = (uint32 *)date_mark.osd_addr.y_addr;

            //calculate date rect
            src_date_rect = date_mark.overlay_rect;

            _DCAMERA_handleDateMark(target_rect, target_addr, src_date_rect, src_date_addr, FALSE);
        }

        //SCI_TRACE_LOW("david capture before JPEG_EncodeJpeg");
        if(JPEG_ENC_OK != JPEG_EncodeJpeg(pJpgEncInPara, pJpgEncOutPara))
        {
        	SCI_TRACE_LOW("DC:JPEG_EncodeJpeg fail");
               return DCAMERA_OP_ENCODE_ERR;
        }

#if defined(_DC_EXIF_)
        if(p_dc_cfg->exif_eb == SCI_TRUE)
        {
            _DCAMERA_JpgAddExif(&s_dc_info_ptr->isp_frame,
                                &dc_size,
                                &dc_rect,
                                (uint32*)&pJpgEncInPara->jpeg_buf_ptr,
                                &pJpgEncOutPara->out_size,
                                param_ptr->enc_quality_level);
        }
#endif
    }
    else
    {
        //encoding jpeg in frame mode
        jpeg_in.yuv_info.yuv_chn.y_chn_ptr = (uint8 *)s_dc_info_ptr->isp_frame.yaddr;
        jpeg_in.yuv_info.yuv_chn.u_chn_ptr = (uint8 *)s_dc_info_ptr->isp_frame.uaddr;
        jpeg_in.yuv_info.sample_format = JINF_FORMAT_YUV422;
        jpeg_in.yuv_info.data_pattern  = JINF_YUV_PATTERN_CHN_Y_UV;

#if defined(CHIP_ENDIAN_LITTLE)
        jpeg_in.yuv_info.data_endian    = JINF_DATA_BIG_ENDIAN;
#endif
        jpeg_in.src_width  = s_dc_info_ptr->jpeg_rect.w;
        jpeg_in.src_height = s_dc_info_ptr->jpeg_rect.h;

        if(compress_level> DCAMERA_ENC_QUALITY_MID)//high quality
            compress_level = JINF_QUALITY_MIDDLE_HIGH;
        else if(compress_level < DCAMERA_ENC_QUALITY_MID)//low quality
            compress_level = JINF_QUALITY_LOW;
        else
            compress_level = JINF_QUALITY_MIDDLE;

        if(NULL != malloc_buffer_ptr)
        {
            jpeg_in.target_buf_ptr = malloc_buffer_ptr;
            jpeg_in.target_buf_size = malloc_buffer_len;
        }
        else
        {
            jpeg_in.target_buf_ptr = param_ptr->data_buf;
            jpeg_in.target_buf_size = param_ptr->buf_len;
        }

#if defined(_DC_EXIF_)
        if(p_dc_cfg->exif_eb == SCI_TRUE)
        {
            jpeg_in.target_buf_ptr += DC_EXIF_LENGTH;
            jpeg_in.target_buf_size -= DC_EXIF_LENGTH;

            if((p_dc_cfg->thumbnail_eb == SCI_TRUE) &&
               (jpeg_in.src_width > VGA_WIDTH))
            {
                jpeg_in.target_buf_ptr += DC_THUMBNAIL_LENGTH;
                jpeg_in.target_buf_size -= DC_THUMBNAIL_LENGTH;
            }
        }
#endif

        jpeg_in.write_file_func = PNULL;
        jpeg_in.async_callback  = PNULL;
        jpeg_in.quality         = compress_level;/*lint !e64*/
        jpeg_in.encode_buf_ptr  = DCAMERA_CaptureGetJPGTmpBuf(&jpeg_in.encode_buf_size);
        if(s_dc_info_ptr->isp_frame.width == s_dc_info_ptr->jpeg_rect.w &&
           s_dc_info_ptr->isp_frame.height == s_dc_info_ptr->jpeg_rect.h)
        {
            jpeg_in.read_yuv_func   = _DCAMERA_JpgEncGetSliceF420T422;      //420 format handle, don't need scale
            jpeg_in.enc_slice_height = DC_SLICE_HEIGHT;
        }
        else
        {
            jpeg_in.read_yuv_func   = _DCAMERA_JpgEncGetSlice;
            //jpeg_in.enc_slice_height = dc_rect.h; //_DCAMERA_JpgEncSliceScalingGetSrcHeight();
            //jpeg_in.enc_slice_height = (DC_SLICE_HEIGHT * s_dc_info_ptr->jpeg_rect.h)/jpeg_in.enc_slice_height;
            //if(jpeg_in.enc_slice_height < DC_SLICE_HEIGHT)
            //{
            //    jpeg_in.enc_slice_height = DC_SLICE_HEIGHT;
            //}
            jpeg_in.enc_slice_height = DC_SLICE_HEIGHT;
        }

        s_slice_out_once = 0;
        s_slice_read_out = 0;
        s_is_first_slice = 1;
        s_src_line_offset = 0;
        if(JINF_SUCCESS != IMGJPEG_EncodeEx(&jpeg_in, &jpeg_out))
        {
            //DCAMERA_TRACE:"DC: Call IMGJPEG_EncodeEx,Error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1806_112_2_18_1_24_2_346,(uint8*)"", SCI_GetTickCount());
            return DCAMERA_OP_ENCODE_ERR;
        }

#if defined(_DC_EXIF_)
        if(p_dc_cfg->exif_eb == SCI_TRUE)
        {
            dc_size.w = s_dc_info_ptr->isp_frame.width;
            dc_size.h = s_dc_info_ptr->isp_frame.height;
            dc_rect.w = dc_size.w;
            dc_rect.h = dc_size.h;
            dc_dst_size.w = s_dc_info_ptr->jpeg_rect.w;
            dc_dst_size.h = s_dc_info_ptr->jpeg_rect.h;
            //_DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);//for is has been rotated before now

            _DCAMERA_JpgAddExif(&s_dc_info_ptr->isp_frame,
                                &dc_size,
                                &dc_rect,
                                (uint32*)&jpeg_out.jpeg_buf_ptr,
                                &jpeg_out.jpeg_buf_size,
                                param_ptr->enc_quality_level);
        }
#endif

        pJpgEncInPara->jpeg_buf_ptr = jpeg_out.jpeg_buf_ptr;
        pJpgEncOutPara->out_size    = jpeg_out.jpeg_buf_size;

    }

    //DCAMERA_TRACE:"DC: Call JPEG_EncodeJpeg, end time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1835_112_2_18_1_24_3_347,(uint8*)"d", SCI_GetTickCount());

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: Set is stop preview
// Author:     Juan.zhang
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
LOCAL void _DCAMERA_SetIsStopPreview(BOOLEAN is_stop)
{
    s_dcam_is_stop_preview = is_stop;
}
/******************************************************************************/
// Description: Get is stop preview
// Author:     Juan.zhang
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
LOCAL BOOLEAN _DCAMERA_GetIfStopPreview(void)
{
    return s_dcam_is_stop_preview;
}

/******************************************************************************/
// Description: To handle target size of image output from sensior
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL void _DCAMERA_HandleSensorOutputSize(DCAMERA_RECT_T target_rect)
{
    uint16  width        = 0;
    uint16  height       = 0;
    uint16  search_width = 0;
    uint16  trim_width   = 0;
    uint16  trim_height  = 0;
    uint16  i = 0;
    BOOLEAN is_need_interpolation       = SCI_FALSE;
    DCAMERA_INFO_T    * dc_info_ptr     = s_dc_info_ptr;
    SENSOR_EXP_INFO_T * sensor_info_ptr = s_sensor_ptr;


    if(PNULL == s_dc_info_ptr || PNULL == s_sensor_ptr)
        return ;

    //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize -> Target width = %d, target height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1888_112_2_18_1_24_3_348,(uint8*)"dd", target_rect.w, target_rect.h);

    // If target image size more large than max size of image from sensor, need interpolation.
    if(sensor_info_ptr->source_width_max < target_rect.w)
    {
        is_need_interpolation = SCI_TRUE;
        //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize -> Need Interpolation !!!!!! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1894_112_2_18_1_24_3_349,(uint8*)"");
    }

    if(is_need_interpolation)
    {
        search_width = sensor_info_ptr->source_width_max;
    }
    else
    {
        search_width = target_rect.w;
    }
    //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize -> search_width = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1905_112_2_18_1_24_3_350,(uint8*)"d", search_width);

    // select preview mode and snapshot mode
    for(i = SENSOR_MODE_PREVIEW_ONE; i< SENSOR_MODE_MAX ; i++)
    {
        if(SENSOR_MODE_MAX != sensor_info_ptr->sensor_mode_info[i].mode)
        {
            width = sensor_info_ptr->sensor_mode_info[i].width;
            //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize -> width = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1913_112_2_18_1_24_3_351,(uint8*)"d", width);

            if( search_width <= width)
            {
                dc_info_ptr->snapshot_m = sensor_info_ptr->sensor_mode_info[i].mode;
                if(dc_info_ptr->snapshot_m < SENSOR_MODE_PREVIEW_TWO)
                {
                    dc_info_ptr->preview_m = SENSOR_MODE_PREVIEW_ONE;
                }
                else
                {
                    dc_info_ptr->preview_m = SENSOR_MODE_PREVIEW_TWO;
                }

                height = sensor_info_ptr->sensor_mode_info[i].height;

                break;
            }

        }
        if(i == (SENSOR_MODE_MAX - 1))
        {
            // can't find the right mode
            //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize,can't find the right mode"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1936_112_2_18_1_24_3_352,(uint8*)"");
        }
    }

    //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize -> preview_m = %d, snapshot_m = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1943_112_2_18_1_24_3_353,(uint8*)"dd", dc_info_ptr->preview_m, dc_info_ptr->snapshot_m     );

    // Get width and height of image in snapshot mode
    width  = sensor_info_ptr->sensor_mode_info[dc_info_ptr->snapshot_m].width;
    height = sensor_info_ptr->sensor_mode_info[dc_info_ptr->snapshot_m].height;
    //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize -> snap_width = %d, snap_height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1951_112_2_18_1_24_3_354,(uint8*)"dd", width,height      );

    // calculate zoom step in snapshot mode
    dc_info_ptr->zoom_w_step_snap = ZOOM_STEP(width);
    dc_info_ptr->zoom_h_step_snap = ZOOM_STEP(height);

    // Get width and height of image in preview mode
    width  = sensor_info_ptr->sensor_mode_info[dc_info_ptr->preview_m].width;
    height = sensor_info_ptr->sensor_mode_info[dc_info_ptr->preview_m].height;
    //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize -> preview_width = %d, preview_height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1963_112_2_18_1_24_3_355,(uint8*)"dd", width,height      );

    // calculate zoom step in preview mode
    dc_info_ptr->zoom_w_step = ZOOM_STEP(width);
    dc_info_ptr->zoom_h_step = ZOOM_STEP(height);


    switch(dc_info_ptr->preview_ctl_info.display_mode)
    {
        case DCAMERA_DISP_FULLSCREEN:
            if((width * dc_info_ptr->disp_rect.h) > (height * dc_info_ptr->disp_rect.w))
            {
                trim_width = dc_info_ptr->disp_rect.w * height / dc_info_ptr->disp_rect.h;
                dc_info_ptr->zoom_w_step = ZOOM_STEP(trim_width);
                dc_info_ptr->zoom_w_step &= ~1;

            }
            else if((width * dc_info_ptr->disp_rect.h) < (height * dc_info_ptr->disp_rect.w))
            {
                trim_height= dc_info_ptr->disp_rect.h * width / dc_info_ptr->disp_rect.w;
                dc_info_ptr->zoom_h_step = ZOOM_STEP(trim_height);
                dc_info_ptr->zoom_h_step &= ~1;
            }
            break;

        default:
            break;
    }


    //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize ->(preview) zoom width step = %d, zoom height step = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1996_112_2_18_1_24_3_356,(uint8*)"dd",dc_info_ptr->zoom_w_step,dc_info_ptr->zoom_h_step);
    //DCAMERA_TRACE:"DC: _DCAMERA_HandleSensorTargetSize ->(snapshot) zoom width step = %d, zoom height step = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2000_112_2_18_1_24_3_357,(uint8*)"dd",dc_info_ptr->zoom_w_step_snap,dc_info_ptr->zoom_h_step_snap);

}

/******************************************************************************/
// Description: Set preview brightness
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewBrightness(uint32 level)
{

    DCAMERA_BeforeSetting();
    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {

    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl( SENSOR_IOCTL_BRIGHTNESS, level);
    }

    return DCAMERA_AfterSetting();
}

/******************************************************************************/
// Description: Set preview contrast
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewContrast(uint32 level)
{
    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {

    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl( SENSOR_IOCTL_CONTRAST, level);
    }

    return DCAMERA_AfterSetting();
}

/******************************************************************************/
// Description: Set preview sharpness
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewSharpness(uint32 level)
{
    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {

    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl( SENSOR_IOCTL_SHARPNESS, level);
    }

    return DCAMERA_AfterSetting();
}
/******************************************************************************/
// Description: Set meter mode
// Author:     Baggio.he
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewMeterMOde(uint32 param)
{
    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {

    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        uint32                  meter_set[SENSOR_IOCTRL_PARAM_LENGTH] = {0};
        DCAMERA_PARAM_METER_T   *p_meter_param = (DCAMERA_PARAM_METER_T*)param;
        SENSOR_METER_MODE_E     sensor_meter_mode = SENSOR_METER_MODE_MAX;

        switch(p_meter_param->meter_mode)
        {
            case DCAMERA_METER_MODE_FULL:
                sensor_meter_mode = SENSOR_METER_MODE_FULL;
                break;
            case DCAMERA_METER_MODE_CENTER:
                sensor_meter_mode = SENSOR_METER_MODE_CENTER;
                break;
            case DCAMERA_METER_MODE_SPOT:
                sensor_meter_mode = SENSOR_METER_MODE_SPOT;
                break;
            case DCAMERA_METER_MODE_MATRIX:
                sensor_meter_mode = SENSOR_METER_MODE_MATRIX;
                break;
            default:
                break;
        }
        if(SENSOR_METER_MODE_MAX == sensor_meter_mode)
        {
            //DCAMERA_TRACE:"_DCAMERA_SetPreviewMeterMOde: meter mode of sensor error,mode = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2112_112_2_18_1_24_3_358,(uint8*)"d",p_meter_param->meter_mode);
            return DCAMERA_OP_PARAM_ERR;
        }
        meter_set[0] = (uint32)sensor_meter_mode;
        if(SENSOR_METER_MODE_SPOT == sensor_meter_mode)
        {
            meter_set[1] = (uint32)p_meter_param->meter_rect.x;
            meter_set[2] = (uint32)p_meter_param->meter_rect.y;
            meter_set[3] = (uint32)p_meter_param->meter_rect.w;
            meter_set[4] = (uint32)p_meter_param->meter_rect.h;
        }
        Sensor_Ioctl(SENSOR_IOCTL_METER_MODE, (uint32)&meter_set[0]);
    }

    return DCAMERA_AfterSetting();
}


/******************************************************************************/
// Description: Set preview saturation
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewSaturation(uint32 level)
{
    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {


    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl( SENSOR_IOCTL_SATURATION, level);
    }

    return DCAMERA_AfterSetting();
}

/******************************************************************************/
// Description: Set preview effect
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewEffect(DCAMERA_PARAM_EFFECT_E type)
{
    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {
    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl( SENSOR_IOCTL_IMAGE_EFFECT, type );
    }

    return DCAMERA_AfterSetting();
}

/******************************************************************************/
// Description: Set preview while balance mode
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewWBMode(DCAMERA_PARAM_WB_MODE_E mode)
{
    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {
    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl(SENSOR_IOCTL_SET_WB_MODE, mode);
    }
    return DCAMERA_AfterSetting();
}

/******************************************************************************/
// Description: Set ISO level
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetISO(uint16 level)
{ //Tim.zhu@20080425 for cr114537 add camera function

    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {
        //
    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl(SENSOR_IOCTL_ISO, level);
    }

    return DCAMERA_AfterSetting();
}

/******************************************************************************/
// Description: Set exposure compensation
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetExposureCompensation(uint16 level)
{//Tim.zhu@20080425 for cr114537 add camera function

    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {
        //
    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl(SENSOR_IOCTL_EXPOSURE_COMPENSATION, level);
    }

    return DCAMERA_AfterSetting();
}

/******************************************************************************/
// Description: Set preview video mode
// Author:
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewVideoMode(uint32 vid_mode)
{
    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl( SENSOR_IOCTL_VIDEO_MODE, vid_mode);
    }

    return DCAMERA_AfterSetting();
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewFlickMode(uint32 flick_mode)
{
    DCAMERA_BeforeSetting();

    if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl( SENSOR_IOCTL_ANTI_BANDING_FLICKER, flick_mode);
    }

    return DCAMERA_AfterSetting();
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetSensorRot(uint32 arg)
{
    //SCI_TRACE_LOW:"_DCAMERA_SetSensorRot, Angle %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2281_112_2_18_1_24_3_359,(uint8*)"d",arg);

    if(DCAMERA_ROTATION_FLIP == arg)
    {
        Sensor_Ioctl(SENSOR_IOCTL_VMIRROR_ENABLE,SCI_TRUE);
    }
    else if(DCAMERA_ROTATION_MIRROR == arg)
    {
        Sensor_Ioctl(SENSOR_IOCTL_HMIRROR_ENABLE,SCI_TRUE);
    }
    else if(DCAMERA_ROTATION_MIRROR_FLIP == arg)
    {
        Sensor_Ioctl(SENSOR_IOCTL_HMIRROR_ENABLE,SCI_TRUE);
        Sensor_Ioctl(SENSOR_IOCTL_VMIRROR_ENABLE,SCI_TRUE);
    }
    else
    {
        Sensor_Ioctl(SENSOR_IOCTL_HMIRROR_ENABLE,SCI_FALSE);
        Sensor_Ioctl(SENSOR_IOCTL_VMIRROR_ENABLE,SCI_FALSE);
    }

    return SCI_SUCCESS;

}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_CtrlAtvCallback(void * ptr)
{
    uint32    tmp_val = 0;
    uint32    cmd_param = 0;

    tmp_val = *(uint32 *)ptr;

    if(DCAMERA_OP_ATV == s_operation_mode &&
        (_DCAMERA_IsPreviewEx()||_DCAMERA_IsRecord()))
    {
        if(0 == tmp_val)
        {
            cmd_param = (ATV_CMD_MASK_ENABLE<<0x10)&0xFFFF0000;
        }
        else if(1 == tmp_val)
        {
            cmd_param = (ATV_CMD_MASK_DISABLE<<0x10)&0xFFFF0000;
        }

        cmd_param |= 0;
        Sensor_Ioctl(SENSOR_IOCTL_ATV, cmd_param);
    }

    return SCI_SUCCESS;
}

LOCAL  DCAMERA_ROTATION_MODE_E _DCAMERA_ROT_Convertor(DCAMERA_ROTATION_MODE_E src_rot, DCAMERA_ROTATION_MODE_E *img_rot_ptr )
{
    DCAMERA_ROTATION_MODE_E ret  = DCAMERA_ROTATION_0;

    //SCI_TRACE_LOW:"_DCAMERA_ROT_Convertor: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2314_112_2_18_1_24_3_360,(uint8*)"d", src_rot);

    *img_rot_ptr =  DCAMERA_ROTATION_0;

     switch(src_rot)
     {

        case DCAMERA_ROTATION_180:
        case DCAMERA_ROTATION_MIRROR:
        case DCAMERA_ROTATION_FLIP:
        case DCAMERA_ROTATION_0:
             *img_rot_ptr = src_rot;
        break;

        case DCAMERA_ROTATION_270:
             *img_rot_ptr =   DCAMERA_ROTATION_180;
             ret  = DCAMERA_ROTATION_90;
        break;

        case DCAMERA_ROTATION_90:
             ret  = DCAMERA_ROTATION_90;
        break;

        default:
        break;

     }

     //SCI_TRACE_LOW:"_DCAMERA_ROT_Convertor: img: %d, ret: %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2350_112_2_18_1_24_4_361,(uint8*)"dd", *img_rot_ptr, ret);

     return ret;
}

/******************************************************************************/
// Description: Set snapshot target size
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetTargetSize(uint32 arg)
{
    DCAMERA_INFO_T * dc_info_ptr = s_dc_info_ptr;
    DCAMERA_RECT_T target_rect = {0};
    DCAMERA_SIZE_T* st_sensor_size_ptr = DC_GetSensorSizeStPtr(arg);
    SENSOR_MODE_E  cur_mode    = dc_info_ptr->preview_m;

    if(arg >= DCAMERA_PHOTO_SIZE_MAX)
        return DCAMERA_OP_PARAM_ERR;

    target_rect.x = 0;
    target_rect.y = 0;
    target_rect.w = st_sensor_size_ptr->w;
    target_rect.h = st_sensor_size_ptr->h;

    // Check input parameter

    _DCAMERA_HandleSensorOutputSize(target_rect);

    dc_info_ptr->jpeg_rect.x = target_rect.x;
    dc_info_ptr->jpeg_rect.y = target_rect.y;
    dc_info_ptr->jpeg_rect.w = target_rect.w;
    dc_info_ptr->jpeg_rect.h = target_rect.h;

    if(_DCAMERA_IsPreview())
    {
        // è?1?previewμ?·?±??ê·￠éú±??ˉ￡??òDèòa??D?3?ê??ˉsensoroí??è?preview
        if(cur_mode != dc_info_ptr->preview_m)
        {
            // First, stop preview
            ISP_ServiceStopPreview();

            // Set resolution of sensor in preview mode again!
            Sensor_SetMode(dc_info_ptr->preview_m);

            // start preview by level 0 zoom
            _DCAMERA_SetPreviewZoom(0);
        }
    }
    else if(_DCAMERA_IsPreviewEx())
    {
        // 如果preview的分辨率发生变化，则需要重新初始化sensor和进入preview
        if(cur_mode != dc_info_ptr->preview_m)
        {
            // First, stop preview
            _DCAMERA_GetMutex(s_callback_mutex_ptr);
            ISP_ServiceStopVTExt();
            _DCAMERA_PutMutex(s_callback_mutex_ptr);
            // Set resolution of sensor in preview mode again!
            Sensor_SetMode(dc_info_ptr->preview_m);

            // start preview by level 0 zoom
            _DCAMERA_SetPreviewEXZoom(0);
        }

    }
    return DCAMERA_OP_SUCCESS;

}

/******************************************************************************/
// Description: Set snapshot target size
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_CheckTargetSize(uint32 arg)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_ERROR;

    DCAMERA_SIZE_T cur_sensor_size_st = {0};
    DCAMERA_SIZE_T req_size_st = {0};

    cur_sensor_size_st.w = Sensor_GetInfo()->source_width_max;
    cur_sensor_size_st.h = Sensor_GetInfo()->source_height_max;
    req_size_st.w = DC_GetSensorSizeStPtr(arg)->w;
    req_size_st.h = DC_GetSensorSizeStPtr(arg)->h;

    //DCAMERA_TRACE:"DCAM: _DCAMERA_CheckTargetSize, photo size: width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2441_112_2_18_1_24_4_362,(uint8*)"dd", cur_sensor_size_st.w, cur_sensor_size_st.h);

    if( arg >= DCAMERA_PHOTO_SIZE_MAX)
    {
        //DCAMERA_TRACE:"DCAM: invalid input arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2445_112_2_18_1_24_4_363,(uint8*)"");
        eRet = DCAMERA_OP_PARAM_ERR;
    }
    else
    {
        eRet = DCAMERA_OP_SUCCESS;

    /*
      if(cur_sensor_size_st.w< req_size_st.w && cur_sensor_size_st.h < req_size_st.h)
         {
             eRet = DCAMERA_OP_ERROR;
         }
         else
         {
               eRet = DCAMERA_OP_SUCCESS;
          }
          */
     }
     return eRet;
}

/******************************************************************************/
// Description: Set preview mode
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     NONE
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewMode(DCAMERA_PARAM_ENVIRONMENT_E mode)
{
    DCAMERA_BeforeSetting();
    if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
    {
    }
    else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
    {
        Sensor_Ioctl(SENSOR_IOCTL_PREVIEWMODE, mode);

    }

    return DCAMERA_AfterSetting();
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewSubZoom(ISP_PREVIEW_PARAM_T *p_dst_preview, uint32 dst_zoom,uint32 orig_zoom)
{
    ISP_PREVIEW_PARAM_T     last_zoom = {0};
    ISP_PREVIEW_PARAM_T     sub_zoom = {0};
    uint32                  sub_zoom_width_gap = 0;
    uint32                  sub_zoom_height_gap = 0;
    uint32                  delta = 0;
    uint32                  sub_zoom_step_x = 0;
    uint32                  sub_zoom_step_y = 0;
    uint32                  sub_zoom_step = 0;
    uint32                  i = 0;
    uint32                  isp_status  = 0;

    if(SCI_NULL == p_dst_preview ||
       DCAMERA_PREVIEW_ZOOM_LEVEL_MAX == dst_zoom ||
       DCAMERA_PREVIEW_ZOOM_LEVEL_MAX == orig_zoom)
    {
       return DCAMERA_OP_PARAM_ERR;
    }

    SCI_MEMCPY((void*)&sub_zoom,
               (void*)p_dst_preview,
               sizeof(ISP_PREVIEW_PARAM_T));

    if(SCI_SUCCESS == ISP_ServiceGetPreviewParam(&last_zoom))
    {
        //DCAMERA_TRACE:"_DCAMERA_SetPreviewSubZoom last preview x = %d y = %d, w = %d h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2518_112_2_18_1_24_4_364,(uint8*)"dddd",last_zoom.input_range.x,last_zoom.input_range.y,last_zoom.input_range.w,last_zoom.input_range.h);

        sub_zoom.input_range.x = last_zoom.input_range.x;
        sub_zoom.input_range.y = last_zoom.input_range.y;
        sub_zoom.input_range.w = last_zoom.input_range.w;
        sub_zoom.input_range.h = last_zoom.input_range.h;
        DCAMERA_SetCapParam(&sub_zoom.cap_param);
        if(sub_zoom.cap_param.cap_frame_skipped > 1)
            sub_zoom.cap_param.cap_frame_skipped = 1;
        if(dst_zoom > orig_zoom)
        {
            sub_zoom_width_gap = p_dst_preview->input_range.x - last_zoom.input_range.x;
            sub_zoom_height_gap = p_dst_preview->input_range.y - last_zoom.input_range.y;
            sub_zoom_step_x = sub_zoom_width_gap / DCAMERA_PIXEL_ALIGNED;
            if(sub_zoom_step_x > DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX)
                sub_zoom_step_x = DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX;

            sub_zoom_step_y = sub_zoom_height_gap / DCAMERA_PIXEL_ALIGNED;
            if(sub_zoom_step_y > DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX)
                sub_zoom_step_y = DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX;

            sub_zoom_step = sub_zoom_step_x < sub_zoom_step_y ? sub_zoom_step_x : sub_zoom_step_y;

            //SCI_TRACE_LOW:"_DCAMERA_SetPreviewZoom : sub zoom step level %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2541_112_2_18_1_24_4_365,(uint8*)"d", sub_zoom_step);

            for(i = 0 ; i < sub_zoom_step; i++)
            {
                if(i == sub_zoom_step - 1)
                {
                    sub_zoom.input_range = p_dst_preview->input_range;
                }
                else
                {
                    delta = (sub_zoom_width_gap * (i + 1) + sub_zoom_step - 1) / sub_zoom_step;
                    sub_zoom.input_range.x = last_zoom.input_range.x + delta;
                    sub_zoom.input_range.x = DCAMERA_WIDTH(sub_zoom.input_range.x);

                    delta = (sub_zoom_height_gap * (i + 1) + sub_zoom_step - 1) / sub_zoom_step;
                    sub_zoom.input_range.y = last_zoom.input_range.y + delta;
                    sub_zoom.input_range.y = DCAMERA_HEIGHT(sub_zoom.input_range.y);

                    sub_zoom.input_range.w = sub_zoom.input_size.w - (sub_zoom.input_range.x << 1);
                    sub_zoom.input_range.w = DCAMERA_WIDTH(sub_zoom.input_range.w);
                    sub_zoom.input_range.h = sub_zoom.input_size.h - (sub_zoom.input_range.y << 1);
                    sub_zoom.input_range.h = DCAMERA_HEIGHT(sub_zoom.input_range.h);

                }

                LCD_SetInvalidateLimit(last_zoom.disp_lcd_id, NO_PERMIT_GUI_INVALIDATE);
                if(ISP_ServiceSetPreviewParam(&sub_zoom) == SCI_SUCCESS)
                {
                    isp_status = ISP_ServiceStartPreview();
                    if(SCI_SUCCESS != isp_status)
                    {
                        return DCAMERA_OP_ISP_ERR;
                    }
                    else
                    {
                        if(i != sub_zoom_step - 1)
                        {
                            ISP_ServiceStopPreview();
                        }
                    }
                }
                else
                {
                    return DCAMERA_OP_ISP_ERR;
                }

            }

        }
        else
        {
            sub_zoom_width_gap = last_zoom.input_range.x - p_dst_preview->input_range.x;
            sub_zoom_height_gap = last_zoom.input_range.y - p_dst_preview->input_range.y;
            sub_zoom_step_x = sub_zoom_width_gap / DCAMERA_PIXEL_ALIGNED;
            if(sub_zoom_step_x > DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX)
                sub_zoom_step_x = DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX;

            sub_zoom_step_y = sub_zoom_height_gap / DCAMERA_PIXEL_ALIGNED;
            if(sub_zoom_step_y > DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX)
                sub_zoom_step_y = DCAMERA_PREVIEW_SUB_ZOOM_LEVEL_MAX;

            sub_zoom_step = sub_zoom_step_x < sub_zoom_step_y ? sub_zoom_step_x : sub_zoom_step_y;

            //SCI_TRACE_LOW:"_DCAMERA_SetPreviewZoom : sub zoom step level %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2604_112_2_18_1_24_4_366,(uint8*)"d", sub_zoom_step_x);

            for(i = 0 ; i < sub_zoom_step; i++)
            {
                if(i == sub_zoom_step - 1)
                {
                    sub_zoom.input_range = p_dst_preview->input_range;
                }
                else
                {
                    delta = (sub_zoom_width_gap * (i + 1) + sub_zoom_step - 1) / sub_zoom_step;
                    sub_zoom.input_range.x = last_zoom.input_range.x - delta;
                    sub_zoom.input_range.x = DCAMERA_WIDTH(sub_zoom.input_range.x);

                    delta = (sub_zoom_height_gap * (i + 1) + sub_zoom_step - 1) / sub_zoom_step;
                    sub_zoom.input_range.y = last_zoom.input_range.y - delta;
                    sub_zoom.input_range.y = DCAMERA_HEIGHT(sub_zoom.input_range.y);

                    sub_zoom.input_range.w = sub_zoom.input_size.w - (sub_zoom.input_range.x << 1);
                    sub_zoom.input_range.w = DCAMERA_WIDTH(sub_zoom.input_range.w);
                    sub_zoom.input_range.h = sub_zoom.input_size.h - (sub_zoom.input_range.y << 1);
                    sub_zoom.input_range.h = DCAMERA_HEIGHT(sub_zoom.input_range.h);

                }

                LCD_SetInvalidateLimit(last_zoom.disp_lcd_id, NO_PERMIT_GUI_INVALIDATE);
                if(ISP_ServiceSetPreviewParam(&sub_zoom) == SCI_SUCCESS)
                {
                    isp_status = ISP_ServiceStartPreview();
                    if(SCI_SUCCESS != isp_status)
                    {
                        return DCAMERA_OP_ISP_ERR;
                    }
                    else
                    {
                        if(i != sub_zoom_step - 1)
                        {
                            ISP_ServiceStopPreview();
                        }
                    }
                }
                else
                {
                    return DCAMERA_OP_ISP_ERR;
                }

            }
        }

    }

    return DCAMERA_OP_SUCCESS;

}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetVTZoom(uint32 zoom_level)
{
    ISP_RECT_T              trim_rect = {0};
    uint16                  width_trim = 0;
    uint16                  height_trim = 0;
    uint16                  zoom_step_w = 0;
    uint16                  zoom_step_h = 0;
    DCAMERA_INFO_T*         dc_info_ptr = s_dc_info_ptr;
    ISP_VT_ENCODE_PARAM_T   *p_vt_enc = &dc_info_ptr->vt_enc_param_t;

    if(!_DCAMERA_IsVideoPhone())
    {
        return DCAMERA_OP_ERROR;
    }

    if(zoom_level_last_time == zoom_level)
    {
        //DCAMERA_TRACE:"_DCAMERA_SetVTZoom, zoom level setting ok"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2676_112_2_18_1_24_4_367,(uint8*)"");
        return DCAMERA_OP_SUCCESS;

    }
    ISP_ServiceStopVT();

    if(SCI_ERROR == ISP_ServiceGetVTEncodeParam(p_vt_enc))
        return DCAMERA_OP_ERROR;

    zoom_step_w = ZOOM_STEP(p_vt_enc->local_input_size.w);
    zoom_step_h = ZOOM_STEP(p_vt_enc->local_input_size.h);

    width_trim  = zoom_step_w * zoom_level;
    height_trim = zoom_step_h * zoom_level;

    trim_rect.x += width_trim >> 1;
    trim_rect.y += height_trim >> 1;

    // Make sure that x offset and y offset of input image multiple of 2
    trim_rect.x = (trim_rect.x + 1) & (~1);
    trim_rect.y = (trim_rect.y + 1) & (~1);

    trim_rect.w = p_vt_enc->local_input_size.w  - (trim_rect.x << 1);
    trim_rect.h = p_vt_enc->local_input_size.h - (trim_rect.y << 1);

    trim_rect.w = DCAMERA_WIDTH(trim_rect.w);
    trim_rect.h = DCAMERA_HEIGHT(trim_rect.h);



    //DCAMERA_TRACE:"_DCAMERA_SetVTZoom: level = %d, x = %d y = %d, w = %d h = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2711_112_2_18_1_24_4_368,(uint8*)"ddddd",zoom_level,trim_rect.x,trim_rect.y,trim_rect.w,trim_rect.h);

    SCI_MEMCPY((void*)&p_vt_enc->local_input_range, (void*)&trim_rect,sizeof(ISP_RECT_T));

    ISP_ServiceSetVTEncodeParam(p_vt_enc);

    ISP_ServiceStartVT();

    if(ISP_ServiceGetLastErr())
    {
        return DCAMERA_OP_ISP_ERR;
    }

    zoom_level_last_time = zoom_level;

    return DCAMERA_OP_SUCCESS;
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetMpegZoom(uint32 zoom_level)
{
    ISP_RECT_T              trim_rect = {0};
    uint16                  width_trim = 0;
    uint16                  height_trim = 0;
    uint16                  zoom_step_w = 0;
    uint16                  zoom_step_h = 0;
    ISP_MPEG_PARAM_T        mpeg_param_t = {0};

    if(!ISP_ServiceIsMpegOn())
    {
        return DCAMERA_OP_ERROR;
    }

    if(dv_zoom_level_last_time == zoom_level)
    {
        //DCAMERA_TRACE:"_DCAMERA_SetMpegZoom, zoom level setting ok"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2745_112_2_18_1_24_4_369,(uint8*)"");
        return DCAMERA_OP_SUCCESS;

    }
    ISP_ServiceStopCapture();

    if(SCI_ERROR == ISP_ServiceGetMpegCaptureParam(&mpeg_param_t))
    {
        //DCAMERA_TRACE:"_DCAMERA_SetMpegZoom, Failed to get MPEG parameter"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2753_112_2_18_1_24_4_370,(uint8*)"");
        return DCAMERA_OP_ERROR;
    }

    zoom_step_w = ZOOM_STEP(mpeg_param_t.input_size.w);
    zoom_step_h = ZOOM_STEP(mpeg_param_t.input_size.h);

    width_trim  = zoom_step_w * zoom_level;
    height_trim = zoom_step_h * zoom_level;

    trim_rect.x += width_trim >> 1;
    trim_rect.y += height_trim >> 1;

    // Make sure that x offset and y offset of input image multiple of 2
    trim_rect.x = (trim_rect.x + 1) & (~1);
    trim_rect.y = (trim_rect.y + 1) & (~1);

    trim_rect.w = mpeg_param_t.input_size.w  - (trim_rect.x << 1);
    trim_rect.h = mpeg_param_t.input_size.h - (trim_rect.y << 1);

    trim_rect.w = DCAMERA_WIDTH(trim_rect.w);
    trim_rect.h = DCAMERA_HEIGHT(trim_rect.h);

    //DCAMERA_TRACE:"_DCAMERA_SetMpegZoom: level = %d, x = %d y = %d, w = %d h = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2781_112_2_18_1_24_4_371,(uint8*)"ddddd",zoom_level,trim_rect.x,trim_rect.y,trim_rect.w,trim_rect.h);

    SCI_MEMCPY((void*)&mpeg_param_t.input_range, (void*)&trim_rect,sizeof(ISP_RECT_T));

    ISP_ServiceSetMpegCaptureParam(&mpeg_param_t);

    if(ISP_ServiceStartCaptureMpeg())
    {
        return DCAMERA_OP_ISP_ERR;
    }
    dv_zoom_level_last_time = zoom_level;

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: To set digital camera zoom size
// Author:     Liangwen.Zhen
// Input:      zoom_level
// Output:     none
// Return:     DCAMERA_RETURN_VALUE_E
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewZoom(uint32 zoom_level)
{
    uint16  width_src   = 0;
    uint16  height_src  = 0;
    uint16  width_trim  = 0;
    uint16  height_trim = 0;
    uint32  isp_status  = 0;
    DCAMERA_RETURN_VALUE_E  ret_val         = DCAMERA_OP_SUCCESS;
    ISP_RECT_T              trim_rect       = {0};
    DCAMERA_INFO_T      *   dc_info_ptr     = s_dc_info_ptr;
    ISP_PREVIEW_PARAM_T *   isp_preview_param_ptr = &s_dc_info_ptr->isp_preview_param;
    ISP_RECT_T              logic_disp_rect = {0};
    ISP_RECT_T              logic_lcdc_rect = {0};
    DCAMERA_MEM_PARAM_T     mem_param_t = {0};
    uint32                  mem_start = 0, mem_size = 0;

    if(zoom_level_last_time == zoom_level)
    {
        //DCAMERA_TRACE:"_DCAMERA_SetPreviewZoom, dc_info_ptr->preview_ctl_info.zoom_level = %d, zoom_level = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2823_112_2_18_1_24_4_372,(uint8*)"dd",dc_info_ptr->preview_ctl_info.zoom_level,zoom_level);
        return ret_val;
    }
    else
    {
        dc_info_ptr->preview_ctl_info.zoom_level = zoom_level;
    }

    if(_DCAMERA_IsPreview() || _DCAMERA_IsRecord() )
    {
        ISP_ServiceStopPreview();
    }

    // Get size of preview
    if(SENSOR_MODE_MAX != s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].mode)
    {
        width_src  = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].width;
        height_src = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].height;
    }
    else
    {
        //DCAMERA_TRACE:"_DCAMERA_SetPreviewZoom, wrong preview mode %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2844_112_2_18_1_24_4_373,(uint8*)"d",dc_info_ptr->preview_m);
        return DCAMERA_OP_ERROR;
    }


    DCAMERA_SetCapParam(&isp_preview_param_ptr->cap_param);

    isp_preview_param_ptr->disp_rotation = (ISP_ROTATION_E)dc_info_ptr->preview_ctl_info.rotation_mode;

    // ISP: Set ISP input size
    isp_preview_param_ptr->input_size.w = width_src;
    isp_preview_param_ptr->input_size.h = height_src;

    // ISP: Set ISP display range
    isp_preview_param_ptr->disp_range.x = dc_info_ptr->target_rect.x;
    isp_preview_param_ptr->disp_range.y = dc_info_ptr->target_rect.y;

#if 0
    if(dc_info_ptr->preview_ctl_info.rotation_mode == ISP_ROTATION_90 ||
       dc_info_ptr->preview_ctl_info.rotation_mode == ISP_ROTATION_270)
    {
        isp_preview_param_ptr->disp_range.w = dc_info_ptr->target_rect.h;
        isp_preview_param_ptr->disp_range.h = dc_info_ptr->target_rect.w;
    }
    else
    {
        isp_preview_param_ptr->disp_range.w = dc_info_ptr->target_rect.w;
        isp_preview_param_ptr->disp_range.h = dc_info_ptr->target_rect.h;
    }
#else

    isp_preview_param_ptr->disp_range.w = dc_info_ptr->target_rect.w;
    isp_preview_param_ptr->disp_range.h = dc_info_ptr->target_rect.h;

#endif
    isp_preview_param_ptr->lcd_rect.x = dc_info_ptr->disp_rect.x;
    isp_preview_param_ptr->lcd_rect.y = dc_info_ptr->disp_rect.y;
    isp_preview_param_ptr->lcd_rect.w = dc_info_ptr->disp_rect.w;
    isp_preview_param_ptr->lcd_rect.h = dc_info_ptr->disp_rect.h;

    // size of source image
    trim_rect.x = 0;
    trim_rect.y = 0;
    trim_rect.w = (uint32)width_src;
    trim_rect.h = (uint32)height_src;

    //DCAMERA_TRACE:"_DCAMERA_SetPreviewZoom, logical coordinate ,lcd_rect {%d %d %d %d},disp_range {%d %d %d %d}, rot %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2899_112_2_18_1_24_5_374,(uint8*)"ddddddddd",isp_preview_param_ptr->lcd_rect.x,isp_preview_param_ptr->lcd_rect.y,isp_preview_param_ptr->lcd_rect.w,isp_preview_param_ptr->lcd_rect.h,isp_preview_param_ptr->disp_range.x,isp_preview_param_ptr->disp_range.y,isp_preview_param_ptr->disp_range.w,isp_preview_param_ptr->disp_range.h,isp_preview_param_ptr->disp_rotation);

    _DCamera_AdjustRect(&trim_rect,
                        &isp_preview_param_ptr->lcd_rect,
                        &isp_preview_param_ptr->disp_range,
                        dc_info_ptr->preview_ctl_info.display_mode);

    logic_lcdc_rect = isp_preview_param_ptr->lcd_rect;
    logic_disp_rect = isp_preview_param_ptr->disp_range;
    _DCAMERA_ConvertCoorToISPRect(isp_preview_param_ptr->disp_lcd_id,
                                  &logic_lcdc_rect,
                                  &isp_preview_param_ptr->lcd_rect,
                                  (DCAMERA_ROTATION_MODE_E)isp_preview_param_ptr->disp_rotation);

    _DCAMERA_ConvertCoorToISPRect(isp_preview_param_ptr->disp_lcd_id,
                                  &logic_disp_rect,
                                  &isp_preview_param_ptr->disp_range,
                                  (DCAMERA_ROTATION_MODE_E)isp_preview_param_ptr->disp_rotation);

    //SCI_TRACE_LOW:"_DCAMERA_SetPreviewZoom : disp_range = %d %d %d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2922_112_2_18_1_24_5_375,(uint8*)"dddd", isp_preview_param_ptr->disp_range.x,isp_preview_param_ptr->disp_range.y,isp_preview_param_ptr->disp_range.w,isp_preview_param_ptr->disp_range.h);

    width_trim  = dc_info_ptr->zoom_w_step * zoom_level;
    height_trim = dc_info_ptr->zoom_h_step * zoom_level;

    trim_rect.x += width_trim >> 1;
    trim_rect.y += height_trim >> 1;

    // Make sure that x offset and y offset of input image multiple of 4
    trim_rect.x = trim_rect.x & ~1;
    trim_rect.y = trim_rect.y & ~1;

    trim_rect.w = width_src  - (trim_rect.x << 1);
    trim_rect.h = height_src - (trim_rect.y << 1);

    // Make sure that height and width of input image multiple of 4
    trim_rect.w = DCAMERA_WIDTH(trim_rect.w);
    trim_rect.h = DCAMERA_HEIGHT(trim_rect.h);

    // ISP: Set ISP input range
    isp_preview_param_ptr->input_range.x = trim_rect.x;
    isp_preview_param_ptr->input_range.y = trim_rect.y;
    isp_preview_param_ptr->input_range.w = trim_rect.w;
    isp_preview_param_ptr->input_range.h = trim_rect.h;

        // Calculate max auto focus window
    dc_info_ptr->af_info.window_max_width  = trim_rect.w;
    dc_info_ptr->af_info.window_max_height = trim_rect.h;

    //DCAMERA_TRACE:"DC ZOOM: level = %d, x = %d y = %d, w = %d h = %d, rotation = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_2958_112_2_18_1_24_5_376,(uint8*)"dddddd",zoom_level,isp_preview_param_ptr->input_range.x,isp_preview_param_ptr->input_range.y,isp_preview_param_ptr->input_range.w,isp_preview_param_ptr->input_range.h,isp_preview_param_ptr->disp_rotation);

    if(DCAMERA_OP_NORMAL == s_operation_mode||DCAMERA_OP_MINIDC == s_operation_mode)
    {
        mem_param_t.review_width = dc_info_ptr->target_rect.w;
        mem_param_t.review_height = dc_info_ptr->target_rect.h;
        mem_param_t.review_rot = dc_info_ptr->preview_ctl_info.rotation_mode;
        mem_start = (uint32)DCAMERA_PreviewGetMem(&mem_param_t, &mem_size);
        ISP_ServiceMemConfig((void*)mem_start, mem_size);
    }

    if(zoom_level_last_time == DCAMERA_PREVIEW_ZOOM_LEVEL_MAX)
    {
        if(ISP_ServiceSetPreviewParam(isp_preview_param_ptr) == SCI_SUCCESS)
        {
            isp_status = ISP_ServiceStartPreview();
            if(SCI_SUCCESS != isp_status)
                return DCAMERA_OP_ISP_ERR;
        }
        else
        {
            return DCAMERA_OP_ISP_ERR;
        }

    }
    else
    {
        ret_val = _DCAMERA_SetPreviewSubZoom(isp_preview_param_ptr, zoom_level, zoom_level_last_time);

    }

    zoom_level_last_time = zoom_level;

    return ret_val;
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetPreviewEXZoom(uint32 zoom_level)
{
    uint16                  width_trim            = 0;
    uint16                  height_trim           = 0;
    uint32                  isp_status            = 0;
    DCAMERA_RETURN_VALUE_E  ret_val               = DCAMERA_OP_SUCCESS;
    DCAMERA_INFO_T*         dc_info_ptr           = s_dc_info_ptr;
    ISP_VT_ENCODE_PARAM_T   *p_vt_enc = &dc_info_ptr->vt_enc_param_t;

    if(zoom_level_last_time == zoom_level)
    {
        //DCAMERA_TRACE:"_DCAMERA_SetPreviewEXZoom, dc_info_ptr->preview_ctl_info.zoom_level = %d, zoom_level = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3007_112_2_18_1_24_5_377,(uint8*)"dd",dc_info_ptr->preview_ctl_info.zoom_level,zoom_level);
        return ret_val;
    }
    else
    {
        dc_info_ptr->preview_ctl_info.zoom_level = zoom_level;
    }

    if(_DCAMERA_IsPreviewEx())
    {
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        ISP_ServiceStopVTExt();
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        _DCAMERA_SetPreviewExFlag(SCI_FALSE);

    }

    isp_status = ISP_ServiceGetVTEncodeParam(p_vt_enc);
    if(isp_status)
    {
        return DCAMERA_OP_ISP_ERR;
    }

    p_vt_enc->local_input_size.w = dc_info_ptr->isp_preview_param.input_size.w;
    p_vt_enc->local_input_size.h = dc_info_ptr->isp_preview_param.input_size.h;

    p_vt_enc->local_input_range.x = dc_info_ptr->isp_preview_param.input_range.x;
    p_vt_enc->local_input_range.y = dc_info_ptr->isp_preview_param.input_range.y;
    p_vt_enc->local_input_range.w = dc_info_ptr->isp_preview_param.input_range.w;
    p_vt_enc->local_input_range.h = dc_info_ptr->isp_preview_param.input_range.h;

    width_trim  = dc_info_ptr->zoom_w_step * zoom_level;
    height_trim = dc_info_ptr->zoom_h_step * zoom_level;

    p_vt_enc->local_input_range.x += width_trim >> 1;
    p_vt_enc->local_input_range.x &= ~1;
    p_vt_enc->local_input_range.y += height_trim >> 1;
    p_vt_enc->local_input_range.y &= ~1;

    p_vt_enc->local_input_range.w = p_vt_enc->local_input_size.w - (p_vt_enc->local_input_range.x << 1);
    p_vt_enc->local_input_range.w = DCAMERA_WIDTH(p_vt_enc->local_input_range.w);
    p_vt_enc->local_input_range.h = p_vt_enc->local_input_size.h - (p_vt_enc->local_input_range.y << 1);
    p_vt_enc->local_input_range.h = DCAMERA_HEIGHT(p_vt_enc->local_input_range.h);

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    if(SCI_SUCCESS == ISP_ServiceSetVTExtEncodeParam(p_vt_enc))
    {
        ISP_ServiceStartVTExt();
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        if(ISP_ServiceGetLastErr())
        {
            return DCAMERA_OP_ISP_ERR;
        }
        zoom_level_last_time = zoom_level;
        _DCAMERA_SetPreviewExFlag(SCI_TRUE);
        return DCAMERA_OP_SUCCESS;
    }
    else
    {
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        return DCAMERA_OP_ISP_ERR;
    }

}

/******************************************************************************/
// Description:  set atv dect region
// Author:     Eddy.wei
// Input:      none
// Output:     none
// Return:     DCAMERA_RETURN_VALUE_E
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetATVDectRegion(void)
{
    DCAMERA_RETURN_VALUE_E rtn = DCAMERA_OP_SUCCESS;
    uint32 param = 0x00;

    param = (ATV_CMD_DECT_REGION<<0x10)&0xffff0000;
    rtn = Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

    return rtn;
}

/******************************************************************************/
// Description: To set atv channel
// Author:     Eddy.wei
// Input:      chn_id & callback
// Output:     none
// Return:     DCAMERA_RETURN_VALUE_E
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetATVChn(uint32 chn_id)
{
    DCAMERA_RETURN_VALUE_E rtn=DCAMERA_OP_SUCCESS;
    uint32 param=0;

    param = (ATV_CMD_SET_CHN<<0x10)&0xffff0000;
    param |=  (chn_id&0xffff);

    rtn = Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

    return rtn;
}

/******************************************************************************/
// Description: To set atv channel
// Author:     Eddy.wei
// Input:      chn_id & callback
// Output:     none
// Return:     DCAMERA_RETURN_VALUE_E
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_SetATVChnHandler(uint32 chn_id)
{
    DCAMERA_RETURN_VALUE_E rtn=DCAMERA_OP_SUCCESS;
    uint32 op_mode=(chn_id&0xffff0000)>>0x10;

    //DCAMERA_TRACE:"DCAM: _DCAMERA_SetATVChn op_mode 0x%x, id 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3122_112_2_18_1_24_5_378,(uint8*)"dd", op_mode, (chn_id&0xffff));

    if(DCAMERA_ATV_CHN_NOLY_DECT == op_mode)
    {
        rtn=_DCAMERA_SetATVDectRegion();
    }
    else
    {
        rtn=_DCAMERA_SetATVChn(chn_id);
    }

    return rtn;
}

/******************************************************************************/
// Description: To set atv region
// Author:     Eddy.wei
// Input:      region & callback
// Output:     none
// Return:     DCAMERA_RETURN_VALUE_E
//
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E  _DCAMERA_SetATVRegion(uint32 region)
{
    DCAMERA_RETURN_VALUE_E rtn=DCAMERA_OP_SUCCESS;
    uint32 param = 0;

    param=(ATV_CMD_SET_REGION<<0x10)&0xffff0000;
    param|=(region&0xffff);
    //DCAMERA_TRACE:"DCAM: _DCAMERA_SetATVRegion 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3150_112_2_18_1_24_5_379,(uint8*)"d", param);
    rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

    return rtn;
}

/******************************************************************************/
// Description: To set atv region
// Author:     Eddy.wei
// Input:      region & callback
// Output:     none
// Return:     DCAMERA_RETURN_VALUE_E
//
// Note:
/******************************************************************************/
LOCAL  DCAMERA_RETURN_VALUE_E _DCAMERA_SetATVVolume(uint32 volume_level)
{
    DCAMERA_RETURN_VALUE_E rtn=DCAMERA_OP_SUCCESS;
    uint32 param=0x00;

    param=(ATV_CMD_SET_VOLUME<<0x10)&0xffff0000;
    param|=(volume_level&0xffff);
    //DCAMERA_TRACE:"DCAM: _DCAMERA_SetATVVolume 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3170_112_2_18_1_24_5_380,(uint8*)"d", param);
    rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

    return rtn;
}

/******************************************************************************/
// Description: enable or disable the AWB
// Author:     Patric.fang
// Input:      BOOL enable_AWB_calibration
// Output:     none
// Return:     none
// Note:
/******************************************************************************/
LOCAL void _DCAMERA_EnableAWBCal(uint32 enable_AWB_calibration)
{
    if(enable_AWB_calibration && (SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat()))
    {
    }
}


/******************************************************************************/
// Description: To check if Digital camera is previewing
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     BOOLEAN
//
// Note:
/******************************************************************************/
LOCAL void _DCAMERA_SetPreviewFlag(BOOLEAN is_preview)
{
    //DCAMERA_TRACE:"DC: _DCAMERA_SetPreviewFlag -> is_preview = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3199_112_2_18_1_24_5_381,(uint8*)"d", is_preview);
    s_dc_is_preview = is_preview;
}

/******************************************************************************/
// Description: To get auto focus status
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     BOOLEAN
//
// Note:
/******************************************************************************/
LOCAL uint32 _DCAMERA_GetAutoFocusStatus(void)
{
    return SCI_SUCCESS;
}
/******************************************************************************/
// Description: To check if Digital camera is previewing
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     BOOLEAN
//
// Note:
/******************************************************************************/
LOCAL BOOLEAN _DCAMERA_IsPreview(void)
{
    return s_dc_is_preview;
}
LOCAL void _DCAMERA_SetPreviewExFlag(BOOLEAN is_preview)
{
    //DCAMERA_TRACE:"DC: _DCAMERA_SetPreviewExFlag -> is_preview = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3227_112_2_18_1_24_5_382,(uint8*)"d", is_preview);
    s_dc_is_preview_ex = is_preview;
}

LOCAL BOOLEAN _DCAMERA_IsPreviewEx(void)
{
    return s_dc_is_preview_ex;
}

/******************************************************************************/
// Description:To set dc record flay for pc camera
// Author:     Feifei.gu
// Input:      none
// Output:     none
// Return:     none
//
// Note:
/******************************************************************************/
LOCAL void _DCAMERA_SetRecordFlag(BOOLEAN is_record)
{
    s_dc_is_record = is_record;
}

/******************************************************************************/
// Description: To check if Digital camera is recording for pc camera
// Author:     Feifei.Gu
// Input:      none
// Output:     none
// Return:     BOOLEAN
//
// Note:
/******************************************************************************/
LOCAL BOOLEAN _DCAMERA_IsRecord(void)
{
    return s_dc_is_record || ISP_ServiceIsMpegOn();
}

LOCAL void _DCAMERA_SetVideoPhoneFlag(BOOLEAN is_vp)
{
    s_dc_is_video_phone = is_vp;
}

/******************************************************************************/
// Description: To check if Digital camera is in video phone
//
// Note:
/******************************************************************************/
LOCAL BOOLEAN _DCAMERA_IsVideoPhone(void)
{
    return s_dc_is_video_phone && ISP_ServiceIsVTOn();
}
/******************************************************************************/
// Description: To check if preview parameter is valid
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     BOOLEAN
//
// Note:
/******************************************************************************/
LOCAL BOOLEAN _DCAMERA_CheckPreviewParam
    (
        DCAMERA_PARAM_CTRL_CMD_E        cmd_type,
        uint32                      arg
    )
{
    BOOLEAN check_status = SCI_TRUE;

    if(DCAMERA_PARAM_INVALID_VALUE == arg)
    {
        return check_status;
    }

    switch(cmd_type)
    {
        case DCAMERA_PARAM_CTRL_BRIGHTNESS:
            if(arg >= DCAMERA_CTRL_LEVEL_MAX)
            {
                check_status = SCI_FALSE;
            }
            break;

        case DCAMERA_PARAM_CTRL_CONTRAST:
            if(arg >= DCAMERA_CTRL_LEVEL_MAX)
            {
                check_status = SCI_FALSE;
            }
            break;

        case DCAMERA_PARAM_CTRL_SHARPNESS:
            if(arg >= DCAMERA_CTRL_LEVEL_MAX)
            {
                check_status = SCI_FALSE;
            }
            break;

        case DCAMERA_PARAM_CTRL_SATURATION:
            if(arg >= DCAMERA_CTRL_LEVEL_MAX)
            {
                check_status = SCI_FALSE;
            }
            break;

        case DCAMERA_PARAM_CTRL_ZOOM:
            if(arg > DCAMERA_PREVIEW_ZOOM_LEVEL_MAX)
            {
                check_status = SCI_FALSE;
            }
            break;

        case DCAMERA_PARAM_CTRL_EFFECT:
            if(arg >= DCAMERA_EFFECT_MAX)
            {
                check_status = SCI_FALSE;
            }

            break;

        case DCAMERA_PARAM_CTRL_PREVIEW_MODE:
            if(arg >= DCAMERA_ENVIRONMENT_MAX)
            {
                check_status = SCI_FALSE;
            }
            break;
        case DCAMERA_PARAM_CTRL_WB_MODE:
            if(arg >= DCAMERA_WB_MODE_MAX)
            {
                check_status = SCI_FALSE;
            }
            break;

        case DCAMERA_PARAM_CTRL_AWB_CAL_ENABLE:
            if(arg > DCAMERA_AWB_CAL_ENABLE)
            {
                check_status = SCI_FALSE;
            }
            break;

        case DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION:

        break;

        case DCAMERA_PARAM_CTRL_VIDEO_MODE:
             if(arg >= DCAMERA_MODE_MAX)
             {
                 check_status = SCI_FALSE;
             }

        break;

        case DCAMERA_PARAM_CTRL_METER_MODE:
        {
             DCAMERA_PARAM_METER_T *p_meter = (DCAMERA_PARAM_METER_T*)arg;

             if(p_meter->meter_mode >= DCAMERA_METER_MODE_MAX)
             {
                 check_status = SCI_FALSE;
             }
        }

        break;
        case DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE:
        {
            SENSOR_REGISTER_INFO_T_PTR p_reg_info = Sensor_GetRegisterInfo();
            uint8                      sensor_id = arg;

            if(sensor_id >= SENSOR_ID_MAX)
            {
                check_status = SCI_FALSE;
            }
            else
            {
                if(p_reg_info->is_register[sensor_id] != SCI_TRUE)
                {
                    check_status = SCI_FALSE;
                }
            }

        }
        break;
        case DCAMERA_PARAM_CTRL_TESTPATTERN_MODE:
            if(arg >= DCAMERA_ENVIRONMENT_MAX || arg != DCAMERA_ENVIRONMENT_TEST_PATTERN)
            {
                check_status = SCI_FALSE;
            }
            break;


        default:
            //DCAMERA_TRACE:"DCAMERA_CheckPreviewParam:: Don't need to check the %d cmd and %d arg"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3401_112_2_18_1_24_6_383,(uint8*)"dd", cmd_type, arg);
            break;
    }

    if(!check_status)
    {
        //DCAMERA_TRACE:"DCAMERA_CheckPreviewParam:: Error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3407_112_2_18_1_24_6_384,(uint8*)"");
    }

    return check_status;
}


/******************************************************************************/
// Description: control preview effect
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_PreviewControl
    (
        DCAMERA_PARAM_CTRL_CMD_E    cmd,
        uint32                      arg
    )
{
    DCAMERA_INFO_T          * dc_info_ptr = s_dc_info_ptr;
    DCAMERA_RETURN_VALUE_E  ret_val       = DCAMERA_OP_SUCCESS;

    //DCAMERA_TRACE:"DC: _DCAMERA_PreviewControl -> cmd = %d, arg = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3430_112_2_18_1_24_6_385,(uint8*)"dd", cmd, arg);

    if(!_DCAMERA_CheckPreviewParam(cmd, arg))
    {
        return DCAMERA_OP_PARAM_ERR;
    }

    // if not in preview, store the parameter in to dc information structure
    if((!_DCAMERA_IsPreview()) &&
       (!_DCAMERA_IsPreviewEx()) &&
       (!_DCAMERA_IsRecord()) &&
       (!_DCAMERA_IsVideoPhone()))
    {
        switch(cmd)
        {
            case DCAMERA_PARAM_CTRL_BRIGHTNESS:
                 dc_info_ptr->preview_ctl_info.brightness_level = arg;
                 break;

            case DCAMERA_PARAM_CTRL_CONTRAST:
                 dc_info_ptr->preview_ctl_info.contrast_level = arg;
                 break;

            case DCAMERA_PARAM_CTRL_SHARPNESS:
                 dc_info_ptr->preview_ctl_info.sharpness_level = arg;
                 break;

            case DCAMERA_PARAM_CTRL_SATURATION:
                 dc_info_ptr->preview_ctl_info.saturation_level = arg;
                 break;

            case DCAMERA_PARAM_CTRL_ZOOM:
                 dc_info_ptr->preview_ctl_info.zoom_level = arg;
                 break;

            case DCAMERA_PARAM_CTRL_EFFECT:
                 dc_info_ptr->preview_ctl_info.effect_type = arg;
                 break;

            case DCAMERA_PARAM_CTRL_PREVIEW_MODE:
                 dc_info_ptr->preview_ctl_info.enviroment_mode = arg;
                 break;

            case DCAMERA_PARAM_CTRL_SET_TARGET_SIZE:
                 dc_info_ptr->preview_ctl_info.target_size = arg;
                 ret_val = _DCAMERA_SetTargetSize(arg);
                 break;

            case DCAMERA_PARAM_CTRL_CHECK_TARGET_SIZE:
                 ret_val = _DCAMERA_CheckTargetSize(arg);
                 break;

            case DCAMERA_PARAM_CTRL_WB_MODE:
                 dc_info_ptr->preview_ctl_info.wb_mode = arg;
                 break;

            case DCAMERA_PARAM_CTRL_ISO:
                 dc_info_ptr->preview_ctl_info.iso_level = arg;
                 break;

            case DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION:
                 dc_info_ptr->preview_ctl_info.exposure_comp_level = arg;
                 break;

            case DCAMERA_PARAM_CTRL_VIDEO_MODE:
                 dc_info_ptr->preview_ctl_info.video_mode = arg;
                 break;

            case DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION:
                 dc_info_ptr->preview_ctl_info.flicker_mode = arg;
                 break;

            case DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE:
                 ret_val = DC_SetCurImgSensorId(arg);
                 break;

            case DCAMERA_PARAM_CTRL_METER_MODE:
                 SCI_MEMCPY((void*)&(dc_info_ptr->preview_ctl_info.meter_param),
                           (void*)arg,
                           sizeof(DCAMERA_PARAM_METER_T));
                 break;

            case DCAMERA_PARAM_CTRL_ATV_CHN:
                 ret_val = _DCAMERA_SetATVChnHandler(arg);
                 break;

            case DCAMERA_PARAM_CTRL_ATV_REGION:
                 ret_val = _DCAMERA_SetATVRegion(arg);
                 break;

            case DCAMERA_PARAM_CTRL_ATV_VOLUME:
                 ret_val = _DCAMERA_SetATVVolume(arg);
                 break;

            case DCAMERA_PARAM_CTRL_FLASH:
                 dc_info_ptr->preview_ctl_info.flash_mode = arg;
                 break;

            case DCAMERA_PARAM_CTRL_FLASH_LIGHT:
                 dc_info_ptr->preview_ctl_info.is_flashlight_enable= arg;
                 break;
            case DCAMERA_PARAM_CTRL_TESTPATTERN_MODE:
                 dc_info_ptr->preview_ctl_info.enviroment_mode = arg;
                 break;
            default:
                 //DCAMERA_TRACE:"DC: _DCAMERA_PreviewControl: error cmd %d"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3519_112_2_18_1_24_6_386,(uint8*)"d", cmd);
                 break;

        }
        //DCAMERA_TRACE:"DC: _DCAMERA_PreviewControl, not in preview"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3523_112_2_18_1_24_6_387,(uint8*)"");

        return ret_val;
    }

    if(_DCAMERA_IsPreview() ||
       _DCAMERA_IsPreviewEx() ||
       _DCAMERA_IsRecord() ||
       _DCAMERA_IsVideoPhone())
    {
        switch(cmd)
        {
            case DCAMERA_PARAM_CTRL_BRIGHTNESS:             // brightness
                 if(dc_info_ptr->preview_ctl_info.brightness_level != arg)
                 {
                     ret_val = _DCAMERA_SetPreviewBrightness(arg);
                     dc_info_ptr->preview_ctl_info.brightness_level = arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_CONTRAST:               // contrast
                 if(dc_info_ptr->preview_ctl_info.contrast_level != arg)
                 {
                     ret_val = _DCAMERA_SetPreviewContrast(arg);
                     dc_info_ptr->preview_ctl_info.contrast_level = arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_SHARPNESS:              // sharpness
                 if(dc_info_ptr->preview_ctl_info.sharpness_level != arg)
                 {
                     ret_val = _DCAMERA_SetPreviewSharpness(arg);
                     dc_info_ptr->preview_ctl_info.sharpness_level = arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_SATURATION:             // saturation
                 if(dc_info_ptr->preview_ctl_info.saturation_level != arg)
                 {
                     ret_val = _DCAMERA_SetPreviewSaturation(arg);
                     dc_info_ptr->preview_ctl_info.saturation_level = arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_ZOOM:
                 if(_DCAMERA_IsVideoPhone())
                 {
                     ret_val = _DCAMERA_SetVTZoom(arg);
                 }
                 else if(_DCAMERA_IsRecord())
                 {
                     ret_val = _DCAMERA_SetMpegZoom(arg);
                 }
                 else
                 {
                     if(_DCAMERA_IsPreview())
                     {
                         ret_val = _DCAMERA_SetPreviewZoom(arg);
                     }
                     else
                     {
                         ret_val = _DCAMERA_SetPreviewEXZoom(arg);
                     }
                 }
                 break;

            case DCAMERA_PARAM_CTRL_EFFECT:                 // effect
                 if(dc_info_ptr->preview_ctl_info.effect_type != arg)
                 {
                     ret_val = _DCAMERA_SetPreviewEffect(arg);
                     dc_info_ptr->preview_ctl_info.effect_type = arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_PREVIEW_MODE:           // preview mode
                 if(dc_info_ptr->preview_ctl_info.enviroment_mode != arg)
                 {
                     ret_val = _DCAMERA_SetPreviewMode(arg);
                     dc_info_ptr->preview_ctl_info.enviroment_mode = arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_SET_TARGET_SIZE:        // target size
                 dc_info_ptr->preview_ctl_info.target_size = arg;
                 ret_val = _DCAMERA_SetTargetSize(arg);
                 break;

            case DCAMERA_PARAM_CTRL_CHECK_TARGET_SIZE:
                 ret_val = _DCAMERA_CheckTargetSize(arg);
                 break;

            case DCAMERA_PARAM_CTRL_WB_MODE:                // while balance
                 if(dc_info_ptr->preview_ctl_info.wb_mode != arg)
                 {
                     ret_val = _DCAMERA_SetPreviewWBMode(arg);
                     dc_info_ptr->preview_ctl_info.wb_mode = arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_AWB_CAL_ENABLE:         // while balance enable
                 _DCAMERA_EnableAWBCal(arg);
                 break;

            case DCAMERA_PARAM_CTRL_ISO:
                 if(dc_info_ptr->preview_ctl_info.iso_level != (uint16)arg)
                 {
                     ret_val = _DCAMERA_SetISO(arg);
                     dc_info_ptr->preview_ctl_info.iso_level = (uint16)arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION:
                 if(dc_info_ptr->preview_ctl_info.exposure_comp_level != arg)
                 {
                     ret_val = _DCAMERA_SetExposureCompensation(arg);
                     dc_info_ptr->preview_ctl_info.exposure_comp_level = (uint16)arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_VIDEO_MODE:
                 ret_val = _DCAMERA_SetPreviewVideoMode(arg);
                 break;

            case DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION:
                 if(dc_info_ptr->preview_ctl_info.flicker_mode != arg)
                 {
                     ret_val = _DCAMERA_SetPreviewFlickMode(arg);
                     dc_info_ptr->preview_ctl_info.flicker_mode = arg;
                 }
                 break;

            case DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE:
                 ret_val = DC_SetCurImgSensorId(arg);
                 break;

            case DCAMERA_PARAM_CTRL_METER_MODE:
                 if(0 != memcmp((void*)&(dc_info_ptr->preview_ctl_info.meter_param), (void*)arg,sizeof(DCAMERA_PARAM_METER_T))) /*lint !e628 -e628*/
                 {
                     ret_val = _DCAMERA_SetPreviewMeterMOde(arg);
                     SCI_MEMCPY((void*)&(dc_info_ptr->preview_ctl_info.meter_param),
                                (void*)arg,
                                sizeof(DCAMERA_PARAM_METER_T));
                 }
                 break;

            case DCAMERA_PARAM_CTRL_ATV_CHN:
                 ret_val = _DCAMERA_SetATVChnHandler(arg);
                 break;

            case DCAMERA_PARAM_CTRL_ATV_REGION:
                 ret_val = _DCAMERA_SetATVRegion(arg);
                 break;

            case DCAMERA_PARAM_CTRL_ATV_VOLUME:
                 ret_val = _DCAMERA_SetATVVolume(arg);
                 break;

            case DCAMERA_PARAM_CTRL_FLASH:
                 if(DCAMERA_FLASH_ON == arg)
                 {
                     Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_TRUE);
                 }
                 else if(DCAMERA_FLASH_OFF == arg ||
                         DCAMERA_FLASH_AUTO == arg)
                 {
                     Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_FALSE);
                 }
                 dc_info_ptr->preview_ctl_info.flash_mode = arg;
                 break;

            case DCAMERA_PARAM_CTRL_FLASH_LIGHT:
                 dc_info_ptr->preview_ctl_info.is_flashlight_enable= arg;
                 break;
            case DCAMERA_PARAM_CTRL_TESTPATTERN_MODE:
                 dc_info_ptr->preview_ctl_info.enviroment_mode = arg;
                 break;
            default:
                 //DCAMERA_TRACE:"DC: _DCAMERA_PreviewControl: error cmd %d"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3677_112_2_18_1_24_6_388,(uint8*)"d", cmd);
                 break;
        }
    }


    return ret_val;
}

#define DC_RGB565_ENDIAN_SW(n)   (((uint16)(n) >> 8)|((uint16)(n) << 8))
LOCAL void _DCAEMRA_Rgb565EndianSwitch(uint32 rgb565_addr,uint32 pixel_num)
{
    uint32                  i;
    uint16                  *p_rgb565 = (uint16*)rgb565_addr;
    uint16                  rgb565 = 0;

    for(i = 0; i < pixel_num; i++)
    {
        rgb565 = *p_rgb565;
        *p_rgb565++ = DC_RGB565_ENDIAN_SW(rgb565);

    }
}

LOCAL  DCAMERA_RETURN_VALUE_E _DCAMERA_ImageYUV422ToYUV420(ISP_FRAME_T* p_yuv_frame)
{
    uint32                      i = 0;
    uint32                      src_chn = 0, dst_chn = 0;
    uint32                      size_each_line = 0;

    size_each_line =  (uint32)p_yuv_frame->width;
    dst_chn = p_yuv_frame->uaddr + size_each_line;
    src_chn = dst_chn + size_each_line;

    //DCAMERA_TRACE:"_DCAMERA_ImageYUV422ToYUV420 ,size_each_line %d, height %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3713_112_2_18_1_24_6_389,(uint8*)"dd",size_each_line,p_yuv_frame->height);

    for(i = 1; i < p_yuv_frame->height; i++)
    {
        SCI_MEMCPY((void*)dst_chn, (void*)src_chn, size_each_line);
        src_chn += (size_each_line << 1);
        dst_chn += size_each_line;
    }

    return DCAMERA_OP_SUCCESS;
}
LOCAL  DCAMERA_RETURN_VALUE_E _DCAMERA_ImageScale(ISP_FRAME_T* p_src_frame,
                                                  ISP_FRAME_T* p_dst_frame)
{
    ISP_SCLAE_PARAM_T               scale_para = {0};
    uint32                          src_addr = 0, dst_addr = 0;
    uint32                          i;
    scale_para.output_size.w    = p_dst_frame->width;
    scale_para.output_size.h    = p_dst_frame->height;
    scale_para.input_addr.yaddr = p_src_frame->yaddr;
    scale_para.input_addr.uaddr = p_src_frame->uaddr;
    scale_para.input_addr.vaddr = p_src_frame->vaddr;
    scale_para.input_size.w     = p_src_frame->width;
    scale_para.input_size.h     = p_src_frame->height;
#if 0
    scale_para.input_rect.x     = 0;
    scale_para.input_rect.y     = 0;
    scale_para.input_rect.w     = p_src_frame->width;
    scale_para.input_rect.h     = p_src_frame->height;
#else

    if((uint32)(scale_para.input_size.w * scale_para.output_size.h) >
       (uint32)(scale_para.input_size.h * scale_para.output_size.w))
    {
        scale_para.input_rect.w = (uint32)((scale_para.input_size.h * scale_para.output_size.w)/scale_para.output_size.h);
        scale_para.input_rect.w = DCAMERA_WIDTH(scale_para.input_rect.w);
        scale_para.input_rect.h = scale_para.input_size.h;
        scale_para.input_rect.x = (scale_para.input_size.w - scale_para.input_rect.w)>>1;
        scale_para.input_rect.x = DCAMERA_WIDTH(scale_para.input_rect.x);
        scale_para.input_rect.y = 0;

    }
    else
    {
        scale_para.input_rect.w = scale_para.input_size.w;
        scale_para.input_rect.h = (uint32)((scale_para.input_size.w*scale_para.output_size.h)/scale_para.output_size.w);
        scale_para.input_rect.h = DCAMERA_HEIGHT(scale_para.input_rect.h);
        scale_para.input_rect.y = (scale_para.input_size.h - scale_para.input_rect.h)>>1;
        scale_para.input_rect.y = DCAMERA_HEIGHT(scale_para.input_rect.y);
        scale_para.input_rect.x = 0;

    }
#endif
    scale_para.input_format     = (ISP_IMAGE_FORMAT)p_src_frame->type;

    scale_para.output_addr.yaddr= p_dst_frame->yaddr;
    scale_para.output_addr.uaddr= p_dst_frame->uaddr;
    scale_para.output_addr.vaddr= p_dst_frame->vaddr;

    scale_para.output_format    =  (ISP_IMAGE_FORMAT)p_dst_frame->type;

    if(scale_para.output_size.w > VGA_WIDTH &&
       p_dst_frame->type != ISP_IMAGE_FORMAT_YUV422)
    {
        scale_para.output_format = ISP_IMAGE_FORMAT_YUV422;
    }

    if(scale_para.output_size.w == scale_para.input_rect.w &&
       scale_para.output_size.h == scale_para.input_rect.h &&
       scale_para.output_format == scale_para.input_format)
    {
        src_addr = scale_para.input_addr.yaddr + scale_para.input_rect.x;
        src_addr += (uint32)(scale_para.input_rect.y * scale_para.input_size.w);
        dst_addr = scale_para.output_addr.yaddr;
        for(i = 0; i < scale_para.input_rect.h; i++)
        {
            SCI_MEMCPY((void*)dst_addr,(void*)src_addr,scale_para.input_rect.w);
            src_addr += scale_para.input_size.w;
            dst_addr += scale_para.input_rect.w;
        }

        src_addr = scale_para.input_addr.uaddr + scale_para.input_rect.x;
        src_addr += (uint32)(scale_para.input_rect.y * scale_para.input_size.w);
        dst_addr = scale_para.output_addr.uaddr;
        for(i = 0; i < scale_para.input_rect.h; i++)
        {
            SCI_MEMCPY((void*)dst_addr,(void*)src_addr,scale_para.input_rect.w);
            src_addr += scale_para.input_size.w;
            dst_addr += scale_para.input_rect.w;
        }

    }
    else
    {
        ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, SCI_NULL);
        ISP_ServiceSetScaleParam(&scale_para);
        ISP_ServiceStartScale();
    }

    if(scale_para.output_size.w > VGA_WIDTH &&
       p_dst_frame->type != ISP_IMAGE_FORMAT_YUV422)
    {
        if(p_dst_frame->type == ISP_IMAGE_FORMAT_YUV420)
        {
            _DCAMERA_ImageYUV422ToYUV420(p_dst_frame);
        }
        else
        {
            //DCAMERA_TRACE:"_DCAMERA_ImageScale ,unsupported output format %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3821_112_2_18_1_24_6_390,(uint8*)"d",p_dst_frame->type);
            return -1;
        }

    }
    if(p_dst_frame->type == ISP_IMAGE_FORMAT_RGB565_EX)
    {
        _DCAEMRA_Rgb565EndianSwitch(scale_para.output_addr.yaddr, (uint32)(scale_para.output_size.w * scale_para.output_size.h));
    }
    p_dst_frame->yaddr = scale_para.output_addr.yaddr;
    p_dst_frame->uaddr = scale_para.output_addr.uaddr;
    p_dst_frame->vaddr = scale_para.output_addr.vaddr;
    return DCAMERA_OP_SUCCESS;
}

LOCAL  DCAMERA_RETURN_VALUE_E _DCAMERA_JpgAddExif(ISP_FRAME_T* p_yuv,
                                                  DCAMERA_SIZE_T *p_src_size,
                                                  DCAMERA_RECT_T *p_trim_rect,
                                                  uint32*      p_jpg_buf,
                                                  uint32*      p_jpg_len,
                                                  uint32       jpg_quality)
{
    DCAMERA_RETURN_VALUE_E          ret_value = DCAMERA_OP_SUCCESS;
    DC_PRODUCT_CFG_T_PTR            p_dc_cfg = DC_GeProductCfgPtr();
    ISP_SCLAE_PARAM_T               scale_para = {0};
    JPEG_ENC_IN_PARAM_T             jpeg_enc_in_param = {0};
    JPEG_ENC_OUT_PARAM_T            jpeg_enc_out_param= {0};
    JINF_EXIF_INFO_T                *p_exif_info = DC_GetExifParameter();
    JINF_WEXIF_IN_PARAM_T           exif_in_param = {0};
    JINF_WEXIF_OUT_PARAM_T          exif_out_param={0};
    uint32                          frame_length = 0;
    DCAMERA_CAPTURE_DATE_T  date_mark = {0};

    if(SCI_NULL == p_yuv ||
       SCI_NULL == p_jpg_buf ||
       SCI_NULL == p_jpg_len)
    {
        return DCAMERA_OP_PARAM_ERR;
    }

    //DCAMERA_TRACE:"_DCAMERA_JpgAddExif"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3861_112_2_18_1_24_7_391,(uint8*)"");

    if((p_dc_cfg->thumbnail_eb == SCI_TRUE) &&
       ((uint32)p_trim_rect->w > DC_WIDTH_TO_ADD_THUM))
    {
        scale_para.output_size.w    = DC_THUMBNAIL_WIDTH;
        scale_para.output_size.h    = DC_THUMBNAIL_HEIGHT;

        scale_para.input_addr.yaddr = p_yuv->yaddr;
        scale_para.input_addr.uaddr = p_yuv->uaddr;
        scale_para.input_addr.vaddr = p_yuv->vaddr;
        scale_para.input_size.w     = p_src_size->w;
        scale_para.input_size.h     = p_src_size->h;
        scale_para.output_addr.yaddr= (uint32)DCAMERA_CaptureGetThumBufY(&frame_length);
        scale_para.output_addr.uaddr= (uint32)DCAMERA_CaptureGetThumBufUV(&frame_length);

        scale_para.input_rect.x     = (uint32)p_trim_rect->x;
        scale_para.input_rect.y     = (uint32)p_trim_rect->y;
        scale_para.input_rect.w     = (uint32)p_trim_rect->w;
        scale_para.input_rect.h     = (uint32)p_trim_rect->h;
        if(SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
        {
            scale_para.input_format     = ISP_IMAGE_FORMAT_YUV420;
        }
        else
        {
            scale_para.input_format     = ISP_IMAGE_FORMAT_YUV422;
        }

        scale_para.output_format    = scale_para.input_format;

        ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, SCI_NULL);
        ISP_ServiceSetScaleParam(&scale_para);
        ISP_ServiceStartScale();

        //if the date mark function is enabled, handle and blend the date mark to the source buffer
        date_mark = b_burst_snapshot?(s_snapshot_burst_param.capture_date):(s_snapshot_param.capture_date);
        if((date_mark.eb)&&
           ((s_dc_info_ptr->isp_frame.width != s_dc_info_ptr->jpeg_rect.w)||
            (s_dc_info_ptr->isp_frame.height != s_dc_info_ptr->jpeg_rect.h)))  //frame mode has already added the date mark
        {
            DCAMERA_SIZE_T target_size = {0};
            ISP_ADDRESS_T  target_addr = {0};

            //the target image rect need to add date mark
            target_size.w = scale_para.output_size.w;
            target_size.h = scale_para.output_size.h;

            //set the target buffer
            target_addr.yaddr = scale_para.output_addr.yaddr;
            target_addr.uaddr = scale_para.output_addr.uaddr;
            target_addr.vaddr = scale_para.output_addr.vaddr;

            _DCAMERA_handleScaleAndDate(target_size, target_addr, FALSE);
        }

        jpeg_enc_in_param.image_rect.x = 0;
        jpeg_enc_in_param.image_rect.y = 0;
        jpeg_enc_in_param.image_rect.w = scale_para.output_size.w;
        jpeg_enc_in_param.image_rect.h = scale_para.output_size.h;
        jpeg_enc_in_param.quality_level= jpg_quality;

        jpeg_enc_in_param.yuv_addr.y_chn_addr = (uint32)scale_para.output_addr.yaddr;
        jpeg_enc_in_param.yuv_addr.u_chn_addr = (uint32)scale_para.output_addr.uaddr;
        jpeg_enc_in_param.yuv_addr.v_chn_addr = (uint32)scale_para.output_addr.vaddr;
        jpeg_enc_in_param.jpeg_buf_ptr  = (uint8*)((*p_jpg_buf) - DC_THUMBNAIL_LENGTH);
        jpeg_enc_in_param.jpeg_buf_size = DC_THUMBNAIL_LENGTH;

        if(ISP_IMAGE_FORMAT_YUV420 == scale_para.output_format)
        {
            jpeg_enc_in_param.enc_data_type = JPEG_ENC_DATA_TYPE_YUV420;
        }
        else
        {
            jpeg_enc_in_param.enc_data_type = JPEG_ENC_DATA_TYPE_YUV422;
        }

        if(JPEG_ENC_OK != JPEG_EncodeJpeg(&jpeg_enc_in_param, &jpeg_enc_out_param))
        {
            //DCAMERA_TRACE:"_DCAMERA_JpgAddExif, Failed to encode thumbnail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3940_112_2_18_1_24_7_392,(uint8*)"");
            return DCAMERA_OP_ENCODE_ERR;
        }
        exif_in_param.thumbnail_buf_ptr = jpeg_enc_in_param.jpeg_buf_ptr;
        exif_in_param.thumbnail_buf_size= jpeg_enc_out_param.out_size;
    }
    else
    {
        exif_in_param.thumbnail_buf_ptr = (uint8*)((*p_jpg_buf) - DC_THUMBNAIL_LENGTH);
        exif_in_param.thumbnail_buf_size= 0;
    }


    if(p_exif_info->spec_ptr)
    {
        p_exif_info->spec_ptr->basic.PixelXDimension = scale_para.input_size.w;
        p_exif_info->spec_ptr->basic.PixelYDimension = scale_para.input_size.h;
    }

    exif_in_param.src_jpeg_buf_ptr = (uint8*)*p_jpg_buf;
    exif_in_param.src_jpeg_size    = *p_jpg_len;

    exif_in_param.temp_buf_ptr     = (uint8*)DCAMERA_CaptureGetJPGTmpBuf(&exif_in_param.temp_buf_size);
    exif_in_param.wrtie_file_func  = PNULL;
    exif_in_param.target_buf_ptr   = (uint8*)exif_in_param.thumbnail_buf_ptr;
    exif_in_param.target_buf_size  = exif_in_param.thumbnail_buf_size;
    exif_in_param.exif_info_ptr    = p_exif_info;
    ret_value = IMGJPEG_WriteExif(&exif_in_param, &exif_out_param);/*lint !e64*/
    if(SCI_SUCCESS != ret_value)
    {
        //DCAMERA_TRACE:"_DCAMERA_JpgAddExif, Failed to write exif information , %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3970_112_2_18_1_24_7_393,(uint8*)"d", ret_value);
        ret_value = DCAMERA_OP_ENCODE_ERR;
    }

    *p_jpg_buf = (uint32)exif_out_param.output_buf_ptr;
    *p_jpg_len = exif_out_param.output_size;

    //DCAMERA_TRACE:"_DCAMERA_JpgAddExif, jpg buffer 0x%x, lenght %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_3977_112_2_18_1_24_7_394,(uint8*)"dd", *p_jpg_buf,*p_jpg_len);
    return ret_value;
}

/******************************************************************************/
// Description: snapshot multi images
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_DoBurstSnapshot
    (
        DCAMERA_BURST_SNAPSHOT_PARAM_T *    param_ptr,
        DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T * return_param_ptr
    )
{
    DCAMERA_SNAPSHOT_PARAM_T        snap_param        = {0};
    DCAMERA_SNAPSHOT_RETURN_PARAM_T snap_return_param = {0};
    DCAMERA_SAVE_MODE_E             save_mode         = 0;
    DCAMERA_RETURN_VALUE_E          ret_val           = DCAMERA_OP_SUCCESS;
    uint32                          interval_of_frame = 0;
    uint8                           image_count       = 0;
    uint32                          index             = 0;
    uint32                          prv_skip_num      = 0;
    uint8                           i;
    DCAMERA_MEM_PARAM_T             mem_param_t = {0};
    uint32                          mem_start = 0, mem_size = 0;

    if(!DCAMERA_IsOpen())
    {
        return DCAMERA_OP_NOT_OPEN;
    }

    if(_DCAMERA_IsPreview())
    {
        ISP_ServiceStopPreview();
        _DCAMERA_CloseISP();
    }

    SCI_ASSERT(NULL != param_ptr && NULL != return_param_ptr);/*assert verified*/

    image_count = param_ptr->image_count;
    //DCAMERA_TRACE:"DC: DCAMERA_DoBurstSnapshot -> start time = %d,image_count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4020_112_2_18_1_24_7_395,(uint8*)"dd", SCI_GetTickCount(),image_count);
    if(image_count > DCAMERA_BURST_SNAPSHOT_IMAGE_MAX)
    {
        return DCAMERA_OP_PARAM_ERR;
    }

    _DCAMERA_SetPreviewFlag(SCI_FALSE);

    snap_param.data_type            = param_ptr->data_type;
    snap_param.data_buf             = param_ptr->data_buf;
    snap_param.buf_len              = param_ptr->buf_len;
    snap_param.flash_enable         = param_ptr->flash_enable;
    snap_param.snapshot_callback    = param_ptr->burst_snapshot_callback;
    snap_param.enc_quality_level    = param_ptr->enc_quality_level;
    snap_param.personal_snapshot.select_personal    = 0;
    snap_param.quickview_callback   = param_ptr->quickview_callback;
    snap_param.rotation_mode        = param_ptr->rotation_mode;
    snap_param.capture_date     = param_ptr->capture_date;
    snap_param.capture_start_callback = param_ptr->burst_capture_start_callback;

    // To check if save in buffer or ffs
    if(PNULL != snap_param.data_buf && NULL != snap_param.buf_len)
    {
        // save data into buffer
        save_mode |= DCAMERA_SAVE_MODE_BUF;
        //DCAMERA_TRACE:"DC: DoBurstSnapshot -> save in buffer "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4044_112_2_18_1_24_7_396,(uint8*)"");
    }

    b_burst_snapshot = 1;

	//hch,change param_ptr->interval_of_frame to 1000ms
	param_ptr->interval_of_frame=1000;

    interval_of_frame = param_ptr->interval_of_frame;
    if(interval_of_frame <= DCAMERA_BST_INTERVAL_TIME_MIN)
    {
        interval_of_frame = 0;
    }
    else
    {
        interval_of_frame = interval_of_frame - DCAMERA_BST_INTERVAL_TIME_MIN;
    }

    // do something before burst snapshot in sensor layer
    Sensor_Ioctl(SENSOR_IOCTL_BEFORE_SNAPSHOT, (uint32)s_dc_info_ptr->snapshot_m);

    for(i = 0; i < image_count; i++)
    {
        // Create a new file name
        if(NULL != (save_mode&DCAMERA_SAVE_MODE_FFS))
        {
            snap_param.file_name[index+1] = 0x31 + i; // '1'-'9'
        }

        ret_val = _DCAMERA_DoSnapshot(&snap_param, &snap_return_param);

        if(DCAMERA_OP_SUCCESS != ret_val)
        {
            //DCAMERA_TRACE:"DC: DCAMERA_DoBurstSnapshot - > Snapshot single err %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4073_112_2_18_1_24_7_397,(uint8*)"d", ret_val);
            break;
        }
		if(i != image_count - 1)
			SCI_Sleep(interval_of_frame);



       #if 0
        // Continue to preview in sometime if snapshot has not stop
        //if((i < (image_count -1)) && (s_dc_info_ptr->preview_m == s_dc_info_ptr->snapshot_m))
        if((i < (image_count -1)) && interval_of_frame)
        {
            _DCAMERA_OpenISP();

		   /*
			mem_param_t.review_width = s_dc_info_ptr->target_rect.w;
            mem_param_t.review_height = s_dc_info_ptr->target_rect.h;
            mem_param_t.review_rot = s_dc_info_ptr->preview_ctl_info.rotation_mode;
            mem_start = (uint32)DCAMERA_PreviewGetMem(&mem_param_t, &mem_size);
			*/
            _DCAMERA_GetMutex(s_callback_mutex_ptr);
            /*ISP_ServiceMemConfig((void*)mem_start, mem_size); */

            prv_skip_num = s_dc_info_ptr->isp_preview_param.cap_param.cap_frame_skipped;
            if(s_dc_info_ptr->isp_preview_param.cap_param.cap_frame_skipped  > 1)
                s_dc_info_ptr->isp_preview_param.cap_param.cap_frame_skipped = 1;

            ISP_ServiceSetPreviewParam(&s_dc_info_ptr->isp_preview_param);
            s_dc_info_ptr->isp_preview_param.cap_param.cap_frame_skipped = prv_skip_num;
			ISP_ServiceStartPreview();

            _DCAMERA_PutMutex(s_callback_mutex_ptr);
            //DCAMERA_TRACE:"DC: DCAMERA_DoBurstSnapshot - > interval time of frame %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4096_112_2_18_1_24_7_398,(uint8*)"d", interval_of_frame);
            SCI_Sleep(interval_of_frame);
            ISP_ServiceStopPreview();

            //hch,mask _DCAMERA_CloseISP
           // _DCAMERA_CloseISP();
        }
		#endif
    }

    // do something before burst snapshot in sensor layer
    Sensor_Ioctl(SENSOR_IOCTL_AFTER_SNAPSHOT, (uint32)s_dc_info_ptr->preview_m);

    if((PNULL != param_ptr->burst_snapshot_callback) && (i < image_count))
    {
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        param_ptr->burst_snapshot_callback(ret_val,0,0);
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
    }

    //DCAMERA_TRACE:"DC: DCAMERA_DoBurstSnapshot -> end time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4113_112_2_18_1_24_7_399,(uint8*)"d", SCI_GetTickCount());

    return ret_val;
}


/******************************************************************************/
// Description: digital camera task routine
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:
//
// Note:
/******************************************************************************/
LOCAL void _DCAMERA_TaskRoutine
     (
         uint32 argc,
         void*  argv
     )
{
    DCAMERA_SIGNAL_T * signal;

    while (s_dc_task_id != SCI_INVALID_BLOCK_ID)
    {
        signal = (DCAMERA_SIGNAL_T *)SCI_GetSignal(s_dc_task_id);
        SCI_ASSERT(PNULL != signal);/*assert verified*/

        switch (signal->sig.SignalCode)
        {
            case DCAMERA_TASK_CMD_SNAPSHOT:
                _DCAMERA_DoSnapshot(
                (DCAMERA_SNAPSHOT_PARAM_T *) signal->dc_param.param_ptr1,
                (DCAMERA_SNAPSHOT_RETURN_PARAM_T *) signal->dc_param.param_ptr2
                );
                break;

            case DCAMERA_TASK_CMD_BURST_SNAPSHOT:
                _DCAMERA_DoBurstSnapshot(
                (DCAMERA_BURST_SNAPSHOT_PARAM_T *) signal->dc_param.param_ptr1,
                (DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T *) signal->dc_param.param_ptr2
                );
                break;

            case DCAMERA_TASK_CMD_SCALE:
                {
                    ISP_FRAME_T*                p_isp_frame = (ISP_FRAME_T*)signal->dc_param.param_ptr1;
                    uint32                      i = 0;
                    DCAMERA_IMAGE_PARAM_T       src_image = {0};
                    DCAMERA_IMAGE_PARAM_T       dst_image = {0};
                    uint32                      data_type = DCAMERA_DATA_TYPE_MAX;
                    uint32                      data_pattern = DCAMERA_YUV_MAX;
                    uint32                      img_buf = (uint32)s_dc_info_ptr->p_preview_buf;
                    uint32                      img_buf_len = (uint32)s_dc_info_ptr->preview_buf_len;
                    uint32                      img_buf_end = img_buf + img_buf_len;
                    uint32                      frame_length = 0;
                    uint32                      scale_dst_buf = 0;
                    DCAMERA_RETURN_VALUE_E      rtn = DCAMERA_OP_SUCCESS;
                    DCAMERA_ROTATION_PARAM_T    rot_param = {0};

                    if(!_DCAMERA_IsPreviewEx())
                    {
                        ISP_ServiceFreeFrameAddress((ISP_FRAME_T*)p_isp_frame->fid);
                        break;
                    }

                    _DCAMERA_GetMutex(s_callback_mutex_ptr);

                    if(1 == s_dc_info_ptr->target_frame_count &&
                       ISP_IMAGE_FORMAT_RAW == s_dc_info_ptr->target_frame_format[0])
                    {
                        rtn = _DCAMERA_GetYUVRawData(p_isp_frame, s_sensor_ptr->image_pattern, (uint8*)img_buf);
                        if(rtn)
                        {
                             _DCAMERA_PutMutex(s_callback_mutex_ptr);

                           break;
                        }

                        dst_image.dcamera_addr[0].chn1_addr = img_buf;
                        dst_image.frame_count = 1;
                        dst_image.frame_info_t[0].data_type = DCAMERA_DATA_TYPE_RAW;
                        dst_image.frame_info_t[0].data_pattern.yuv_pattern = (DCAMERA_YUV_PATTERN_E)s_sensor_ptr->image_pattern;
                        dst_image.frame_info_t[0].target_size.w = p_isp_frame->width;
                        dst_image.frame_info_t[0].target_size.h = p_isp_frame->height;
                    }
                    else
                    {
                        for(i = 0; i < s_dc_info_ptr->target_frame_count; i++)
                        {

                            src_image.frame_info_t[i].target_size.w = p_isp_frame->width;
                            src_image.frame_info_t[i].target_size.h = p_isp_frame->height;
                            src_image.dcamera_addr[i].chn1_addr = p_isp_frame->yaddr;
                            src_image.dcamera_addr[i].chn2_addr = p_isp_frame->uaddr;
                            src_image.dcamera_addr[i].chn3_addr = p_isp_frame->vaddr;
                            _DCAMERA_GetDateType( (ISP_IMAGE_FORMAT)p_isp_frame->type,
                                                 &data_type,
                                                 &data_pattern);
                            src_image.frame_info_t[i].data_type = (DCAMERA_DATA_TYPE_E)data_type;
                            src_image.frame_info_t[i].data_pattern.yuv_pattern = (DCAMERA_YUV_PATTERN_E)data_pattern;
                            _DCAMERA_GetDateType(s_dc_info_ptr->target_frame_format[i],
                                                 &data_type,
                                                 &data_pattern);
                            src_image.frame_count = 1;
                            dst_image.frame_info_t[i].data_type = (DCAMERA_DATA_TYPE_E)data_type;
                            dst_image.frame_info_t[i].data_pattern.yuv_pattern = (DCAMERA_YUV_PATTERN_E)data_pattern;
                            dst_image.frame_info_t[i].target_size.w = s_dc_info_ptr->target_frame_info_t[i].target_size.w;
                            dst_image.frame_info_t[i].target_size.h = s_dc_info_ptr->target_frame_info_t[i].target_size.h;

                            frame_length = s_dc_info_ptr->target_frame_info_t[i].target_size.w * s_dc_info_ptr->target_frame_info_t[i].target_size.h;
                            if(s_dc_info_ptr->target_frame_info_t[i].rotation_mode > DCAMERA_ROTATION_0 &&
                               s_dc_info_ptr->target_frame_info_t[i].rotation_mode < DCAMERA_ROTATION_MAX)
                            {

                                scale_dst_buf = img_buf_end - (frame_length << 1) - DCAMERA_PADDING_MEM_SIZE;

                                if(s_dc_info_ptr->target_frame_info_t[i].rotation_mode == DCAMERA_ROTATION_90 ||
                                   s_dc_info_ptr->target_frame_info_t[i].rotation_mode == DCAMERA_ROTATION_270)
                                {
                                    dst_image.frame_info_t[i].target_size.w = s_dc_info_ptr->target_frame_info_t[i].target_size.h;
                                    dst_image.frame_info_t[i].target_size.h = s_dc_info_ptr->target_frame_info_t[i].target_size.w;
                                }
                                rot_param.image_width = dst_image.frame_info_t[i].target_size.w;
                                rot_param.image_height = dst_image.frame_info_t[i].target_size.h;
                                rot_param.dest_chn_addr.y_chn_addr = img_buf;
                                rot_param.dest_chn_addr.y_chn_addr = DC_ADDR(rot_param.dest_chn_addr.y_chn_addr);
                                rot_param.dest_chn_addr.u_chn_addr = rot_param.dest_chn_addr.y_chn_addr + frame_length;
                                rot_param.dest_chn_addr.u_chn_addr = DC_ADDR(rot_param.dest_chn_addr.u_chn_addr);
                                rot_param.rotation_mode = s_dc_info_ptr->target_frame_info_t[i].rotation_mode;
                            }
                            else
                            {
                                scale_dst_buf = img_buf;
                            }
                            dst_image.dcamera_addr[i].chn1_addr = scale_dst_buf;
                            dst_image.dcamera_addr[i].chn1_addr = DC_ADDR(dst_image.dcamera_addr[i].chn1_addr);
                            dst_image.dcamera_addr[i].chn2_addr = dst_image.dcamera_addr[i].chn1_addr + frame_length;
                            dst_image.dcamera_addr[i].chn2_addr = DC_ADDR(dst_image.dcamera_addr[i].chn2_addr);
                            rtn = _DCAMERA_DoImageScaleInVT(&src_image, i, &dst_image);
                            if(rtn)
                            {
                                break;
                            }

                            if(s_dc_info_ptr->target_frame_info_t[i].rotation_mode > DCAMERA_ROTATION_0 &&
                               s_dc_info_ptr->target_frame_info_t[i].rotation_mode < DCAMERA_ROTATION_MAX)
                            {
                                rot_param.src_chn_addr.y_chn_addr = dst_image.dcamera_addr[i].chn1_addr;
                                rot_param.src_chn_addr.u_chn_addr = dst_image.dcamera_addr[i].chn2_addr;
                                if(s_dc_info_ptr->target_frame_format[i] == ISP_IMAGE_FORMAT_RGB565 ||
                                   s_dc_info_ptr->target_frame_format[i] == ISP_IMAGE_FORMAT_RGB565_EX)
                                {
                                    _DCAMERA_RotateRGB565(&rot_param);
                                }
                                else if(s_dc_info_ptr->target_frame_format[i] == ISP_IMAGE_FORMAT_YUV422)
                                {
                                    _DCAMERA_RotateYUV422(&rot_param);
                                }
                                else if(s_dc_info_ptr->target_frame_format[i] == ISP_IMAGE_FORMAT_YUV420)
                                {
                                    _DCAMERA_RotateYUV420(&rot_param);
                                }
                                else
                                {
                                    break;
                                }
                                dst_image.dcamera_addr[i].chn1_addr = rot_param.dest_chn_addr.y_chn_addr;
                                dst_image.dcamera_addr[i].chn2_addr = rot_param.dest_chn_addr.u_chn_addr;

                                if(s_dc_info_ptr->target_frame_info_t[i].rotation_mode == DCAMERA_ROTATION_90 ||
                                   s_dc_info_ptr->target_frame_info_t[i].rotation_mode == DCAMERA_ROTATION_270) //set the actually target size
                                {
                                    dst_image.frame_info_t[i].target_size.w = s_dc_info_ptr->target_frame_info_t[i].target_size.w;
                                    dst_image.frame_info_t[i].target_size.h = s_dc_info_ptr->target_frame_info_t[i].target_size.h;
                                }

                            }
                            img_buf = dst_image.dcamera_addr[i].chn2_addr + frame_length;
                        }

                        dst_image.frame_count = s_dc_info_ptr->target_frame_count;
                    }

                    if(s_dc_info_ptr->image_notice_cb)
                    {
                        (*s_dc_info_ptr->image_notice_cb)(rtn, &dst_image, s_dc_info_ptr->p_user_data);
                    }

                    ISP_ServiceFreeFrameAddress((ISP_FRAME_T*)p_isp_frame->fid);
                    _DCAMERA_PutMutex(s_callback_mutex_ptr);
                }

                break;

            case DCAMERA_TASK_CMD_SNAPSHOT_EX:


                _DCAMERA_DoSnapshotEx(
                (DCAMERA_SNAPSHOT_EX_PARAM_T   *) signal->dc_param.param_ptr1,
                (DCAMERA_SNAPSHOT_EX_RETURN_PARAM_T *)  signal->dc_param.param_ptr2
                );

                break;

            case DCAMERA_TASK_CMD_UPCC:

                //CAMERA_RecordCallback_ext(signal->dc_param.param_ptr1);
            break;

            case DCAMERA_TASK_CMD_ATV:
#ifndef ATV_SUPPORT_NONE
                DCAMERA_AtvHandle(signal->dc_param.param_val);
#endif
            break;

            default:
                //DCAMERA_TRACE:"DC: _DCAMERA_TaskRoutine() don't handle %d command"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4326_112_2_18_1_24_7_400,(uint8*)"d",signal->sig.SignalCode);
                break;
        }

        SCI_FREE(signal);

    }
}

/******************************************************************************/
// Description: digital camera send signal
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:
//
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_SendSignal
    (
        DCAMERA_TASK_CMD_E  cmd_type,
        uint32              param_val,
        void*               param_ptr1,
        void*               param_ptr2
        )
{

    DCAMERA_SIGNAL_T * dc_sig_ptr = SCI_NULL;

    SCI_ASSERT(SCI_NULL != s_dc_task_id);/*assert verified*/

    dc_sig_ptr = (DCAMERA_SIGNAL_T *)SCI_ALLOCA(sizeof(DCAMERA_SIGNAL_T));
    SCI_ASSERT(SCI_NULL != dc_sig_ptr);/*assert verified*/
    SCI_MEMSET(dc_sig_ptr, 0, sizeof(DCAMERA_SIGNAL_T));

    //init the signal
    dc_sig_ptr->sig.SignalCode   = cmd_type;
    dc_sig_ptr->sig.SignalSize   = sizeof(DCAMERA_SIGNAL_T);
    dc_sig_ptr->sig.Sender       = SCI_IdentifyThread();
    dc_sig_ptr->dc_param.param_val  = param_val;
    dc_sig_ptr->dc_param.param_ptr1 = param_ptr1;
    dc_sig_ptr->dc_param.param_ptr2 = param_ptr2;

    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader) dc_sig_ptr, s_dc_task_id))
    {
            return SCI_SUCCESS;
    }
    else
    {
            return SCI_ERROR;
    }

}
/******************************************************************************/
// Description: Creat digital camera thread
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:
//
// Note:
/******************************************************************************/
LOCAL BOOLEAN _DCAMERA_CreateThread(void)
{
    if(SCI_INVALID_BLOCK_ID == s_dc_task_id)
    {

        s_dc_task_id = SCI_CreateAppThread(DCAMERA_TASK_NAME,
                                    DCAMERA_TASK_QNAME,
                                    _DCAMERA_TaskRoutine,
                                    0,
                                    NULL,
                                    DCAMERA_TASK_STACK_SIZE,
                                    DCAMERA_TASK_QNUM,
                                    DCAMERA_TASK_PRI,
                                    DCAMERA_TASK_PREEMPTIVE,
                                    DCAMERA_TASK_START_FLAG);

        SCI_ASSERT (SCI_INVALID_BLOCK_ID != s_dc_task_id);/*assert verified*/

        DCAMERA_CreateTimer();
    }
    return SCI_TRUE;
}

/******************************************************************************/
// Description: Creat digital camera thread
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:
//
// Note:
/******************************************************************************/
#if 0
LOCAL BOOLEAN _DCAMERA_DeleteThread(void)
{

    if(SCI_INVALID_BLOCK_ID != s_dc_task_id)
    {
        SCI_DeleteThread(s_dc_task_id);
        s_dc_task_id = SCI_INVALID_BLOCK_ID;
    }

    return SCI_TRUE;
}
#endif

LOCAL uint32 _DCameraAngle2DrvAngle(DCAMERA_ROTATION_MODE_E dc_angle)
{
    uint32 angle = 0;

    switch(dc_angle)
    {
        case DCAMERA_ROTATION_90:
            angle = ROTATION_90;
            break;
        case DCAMERA_ROTATION_270:
            angle = ROTATION_270;
            break;
        case DCAMERA_ROTATION_180:
            angle = ROTATION_180;
            break;
        case DCAMERA_ROTATION_MIRROR:
            angle = ROTATION_MIRROR;
            break;
        default:
            break;
    }

    return angle;
}
/******************************************************************************/
// Description: YUV420 image data rotation function
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_RotateYUV420(DCAMERA_ROTATION_PARAM_T *param_ptr)
{
    DCAMERA_RETURN_VALUE_E  rVal    = DCAMERA_OP_SUCCESS;
    ROTATION_PARAM_T        rotPara = {0};

    SCI_ASSERT(PNULL != param_ptr);/*assert verified*/
    SCI_ASSERT((0 == param_ptr->image_width%2) && (0 == param_ptr->image_height%2));/*assert verified*/

    rotPara.data_format     = ROTATION_YUV420;
    rotPara.rotation_mode   = _DCameraAngle2DrvAngle(param_ptr->rotation_mode);

    dcRotCvtAddr(&param_ptr->src_chn_addr, &param_ptr->dest_chn_addr, &rotPara);

    dcRotCvtWH(param_ptr->image_width, param_ptr->image_height,
        rotPara.rotation_mode, &rotPara);

    //DCAMERA_TRACE:"DC: _DCAMERA_RotateYUV420, rot_mode=%d,format=%d,img_size.w,h %d %d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4479_112_2_18_1_24_8_401,(uint8*)"dddd",rotPara.rotation_mode,rotPara.data_format,rotPara.img_size.w,rotPara.img_size.h);
    //DCAMERA_TRACE:"DC: _DCAMERA_RotateYUV420,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4486_112_2_18_1_24_8_402,(uint8*)"dddddd",rotPara.src_addr.y_addr,rotPara.src_addr.uv_addr,rotPara.src_addr.v_addr,rotPara.dst_addr.y_addr,rotPara.dst_addr.uv_addr,rotPara.dst_addr.v_addr);

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    rVal = Rotation_AppStart(&rotPara);
    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    if(!rVal)
    {
        //DCAMERA_TRACE:"DC: _DCAMERA_RotateYUV420-Y, SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4494_112_2_18_1_24_8_403,(uint8*)"");
    }
    else
    {
        rVal = DCAMERA_OP_ERROR;
        //DCAMERA_TRACE:"DC: _DCAMERA_RotateYUV420-Y, FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4499_112_2_18_1_24_8_404,(uint8*)"");
        return rVal;
    }

    return rVal;
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_RotateYUV422(DCAMERA_ROTATION_PARAM_T *param_ptr)
{
    DCAMERA_RETURN_VALUE_E  rVal    = DCAMERA_OP_SUCCESS;
    ROTATION_PARAM_T        rotPara = {0};

    SCI_ASSERT(PNULL != param_ptr);/*assert verified*/

    rotPara.data_format     = ROTATION_YUV422;
    rotPara.rotation_mode   = _DCameraAngle2DrvAngle(param_ptr->rotation_mode);

    dcRotCvtAddr(&param_ptr->src_chn_addr, &param_ptr->dest_chn_addr, &rotPara);

    dcRotCvtWH(param_ptr->image_width, param_ptr->image_height,
        rotPara.rotation_mode, &rotPara);

    //DCAMERA_TRACE:"DC: _DCAMERA_RotateYUV422, rot_mode=%d,format=%d,img_size.w,h %d %d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4525_112_2_18_1_24_8_405,(uint8*)"dddd",rotPara.rotation_mode,rotPara.data_format,rotPara.img_size.w,rotPara.img_size.h);
    //DCAMERA_TRACE:"DC: _DCAMERA_RotateYUV422,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4532_112_2_18_1_24_8_406,(uint8*)"dddddd",rotPara.src_addr.y_addr,rotPara.src_addr.uv_addr,rotPara.src_addr.v_addr,rotPara.dst_addr.y_addr,rotPara.dst_addr.uv_addr,rotPara.dst_addr.v_addr);

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    rVal = Rotation_AppStart(&rotPara);
    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    if(!rVal)
    {
        //DCAMERA_TRACE:"DC: _DCAMERA_RotateYUV422, SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4540_112_2_18_1_24_8_407,(uint8*)"");
    }
    else
    {
        rVal = DCAMERA_OP_ERROR;
        //DCAMERA_TRACE:"DC: _DCAMERA_RotateYUV422, FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4545_112_2_18_1_24_8_408,(uint8*)"");
    }

    return rVal;

}

/******************************************************************************/
// Description: RGB565 image data rotation function
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_RotateRGB565(DCAMERA_ROTATION_PARAM_T *param_ptr)
{
    DCAMERA_RETURN_VALUE_E  rVal    = DCAMERA_OP_SUCCESS;
    ROTATION_PARAM_T        rotPara = {0};

    SCI_ASSERT(PNULL != param_ptr);/*assert verified*/

    rotPara.data_format     = ROTATION_RGB565;
    rotPara.rotation_mode   = _DCameraAngle2DrvAngle(param_ptr->rotation_mode);

    dcRotCvtAddr(&param_ptr->src_chn_addr, &param_ptr->dest_chn_addr, &rotPara);

    dcRotCvtWH(param_ptr->image_width, param_ptr->image_height,
        rotPara.rotation_mode, &rotPara);


    //DCAMERA_TRACE:"DC: _DCAMERA_RotateRGB565, rot_mode=%d,format=%d,img_size.w,h %d %d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4579_112_2_18_1_24_8_409,(uint8*)"dddd",rotPara.rotation_mode,rotPara.data_format,rotPara.img_size.w,rotPara.img_size.h);
    //DCAMERA_TRACE:"DC: _DCAMERA_RotateRGB565,src y,u,v 0x%x,0x%x,0x%x, dst y,u,v 0x%x,0x%x,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4586_112_2_18_1_24_8_410,(uint8*)"dddddd",rotPara.src_addr.y_addr,rotPara.src_addr.uv_addr,rotPara.src_addr.v_addr,rotPara.dst_addr.y_addr,rotPara.dst_addr.uv_addr,rotPara.dst_addr.v_addr);

    rVal = Rotation_AppStart(&rotPara);
    if(!rVal)
    {
        //DCAMERA_TRACE:"DC: _DCAMERA_RotateRGB565, SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4591_112_2_18_1_24_8_411,(uint8*)"");
    }
    else
    {
        rVal = DCAMERA_OP_ERROR;
        //DCAMERA_TRACE:"DC: _DCAMERA_RotateRGB565, FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4596_112_2_18_1_24_8_412,(uint8*)"");
    }

    return rVal;

}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_GetCapability(DCAMERA_OP_MODE_E op_mode)
{
    uint32                      ret_value = DCAMERA_OP_SUCCESS;
    DCAMERA_SNAPSHOT_CTL_INFO_T *p_snapsoh_ctrl = &(s_dcamera_info.snapshot_ctl_info);
    uint32                      sensor_size = DCAMERA_PHOTO_SIZE_MAX;
    uint32                      dc_size = DCAMERA_PHOTO_SIZE_MAX, i = 0;
    DCAMERA_SIZE_T              *p_dc_size = SCI_NULL;
    uint16                      sensor_size_w = 0, sensor_size_h = 0;
    DCAMERA_SIZE_T              *p_sensor_size = SCI_NULL;
    DCAMERA_RECT_T              dc_rect = {0};
    DCAMERA_MEM_PARAM_T         mem_param_t = {0};
    uint32                      cpt_cnt     = 0;
    LCD_INFO_T                  lcd_info_t = {0};
    DC_PRODUCT_CFG_T_PTR        p_dc_cfg = DC_GeProductCfgPtr();

    if(DCAMERA_OP_NORMAL == op_mode||DCAMERA_OP_MINIDC == op_mode)
    {
        sensor_size = (uint32)DC_GetSensorSize();
        if(sensor_size > DCAMERA_PHOTO_SIZE_MAX - 1)
        {
            sensor_size = DCAMERA_PHOTO_SIZE_MAX - 1;
        }

        p_sensor_size = DC_GetSensorSizeStPtr(sensor_size);
        sensor_size_w = (uint16)(p_sensor_size->w * DCAMERA_INTERPOLATION_COEF);
        sensor_size_h = (uint16)(p_sensor_size->h * DCAMERA_INTERPOLATION_COEF);

        for(dc_size = DCAMERA_PHOTO_SIZE_1600X1200; dc_size > DCAMERA_PHOTO_SIZE_240X320; dc_size--)
        {
            p_dc_size = DC_GetSensorSizeStPtr(dc_size);
            if(((sensor_size_w >= p_dc_size->w) && (sensor_size_h >= p_dc_size->h)) ||
               ((sensor_size_h >= p_dc_size->w) && (sensor_size_w >= p_dc_size->h)))
            {
                dc_rect.w = p_sensor_size->w;
                dc_rect.h = p_sensor_size->h;
                _DCAMERA_HandleSensorOutputSize(dc_rect);
                dc_rect.w = s_sensor_ptr->sensor_mode_info[s_dcamera_info.snapshot_m].width;
                dc_rect.h = s_sensor_ptr->sensor_mode_info[s_dcamera_info.snapshot_m].height;
                _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, p_dc_size, &dc_rect);
                mem_param_t.sensor_out_width = dc_rect.w;
                mem_param_t.sensor_out_height = dc_rect.h;

                if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dcamera_info.snapshot_m].image_format)
                {
                    mem_param_t.sensor_out_width >>= 1;
                    mem_param_t.sensor_out_height >>= 1;
                    mem_param_t.is_jpg_sensor = 1;
                }
                else
                {
                    mem_param_t.is_jpg_sensor = 0;
                }

                mem_param_t.image_width = p_dc_size->w;
                mem_param_t.image_height = p_dc_size->h;
                mem_param_t.capture_rot = (uint8)DCAMERA_ROTATION_0;
                LCD_GetInfo(0, &lcd_info_t);
                mem_param_t.review_width = lcd_info_t.lcd_width;
                mem_param_t.review_height = lcd_info_t.lcd_height;
                mem_param_t.review_rot = (uint8)DCAMERA_ROTATION_0;

#if defined(_DC_EXIF_)
                if(p_dc_cfg->exif_eb)
                {
                    mem_param_t.b_add_exif = 1;
                }
                else
                {
                    mem_param_t.b_add_exif = 0;
                }
#endif

                if(p_dc_cfg->thumbnail_eb)
                {
                    mem_param_t.b_add_thumbnail = 1;
                }
                else
                {
                    mem_param_t.b_add_thumbnail = 0;
                }

                mem_param_t.slice_height = DC_SLICE_HEIGHT;
                mem_param_t.image_format = ISP_IMAGE_FORMAT_YUV422;
                i = 0;
                do
                {
                    cpt_cnt = DCAMERA_CaptureMemReq(&mem_param_t);
                    if(cpt_cnt)
                    {
                        break;
                    }

                    if(s_dcamera_info.snapshot_m > SENSOR_MODE_PREVIEW_ONE)
                    {
                        //DCAMERA_TRACE:"DC: few memory, reduce the output mode of sensor"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4704_112_2_18_1_24_8_413,(uint8*)"");

                        s_dcamera_info.snapshot_m = s_dcamera_info.snapshot_m - 1;

                        dc_rect.w = s_sensor_ptr->sensor_mode_info[s_dcamera_info.snapshot_m].width;
                        dc_rect.h = s_sensor_ptr->sensor_mode_info[s_dcamera_info.snapshot_m].height;
                        _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, p_dc_size, &dc_rect);
                        mem_param_t.sensor_out_width = dc_rect.w;
                        mem_param_t.sensor_out_height = dc_rect.h;

                        if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dcamera_info.snapshot_m].image_format)
                        {
                            mem_param_t.sensor_out_width >>= 1;
                            mem_param_t.sensor_out_height >>= 1;
                            mem_param_t.is_jpg_sensor = 1;
                        }
                        else
                        {
                            mem_param_t.is_jpg_sensor = 0;
                        }
                    }
                    else
                    {
                        //DCAMERA_TRACE:"DC: few memory, scaling down the image size of one special output mode"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4727_112_2_18_1_24_8_414,(uint8*)"");

                        if(s_sensor_ptr->sensor_mode_info[s_dcamera_info.snapshot_m].width > ISP_SCALE_FRAME_MODE_WIDTH_TH)
                        {
                            mem_param_t.sensor_out_width = ISP_SCALE_FRAME_MODE_WIDTH_TH - i * DCAMERA_ENC_IMAGE_DOWN_STEP;
                            mem_param_t.sensor_out_height = (uint32)(mem_param_t.sensor_out_width * mem_param_t.image_height)/(uint32)mem_param_t.image_width;
                            i ++;
                        }
                        else
                        {
                            mem_param_t.sensor_out_width  -= DCAMERA_ENC_IMAGE_DOWN_STEP;
                            mem_param_t.sensor_out_height -= DCAMERA_ENC_IMAGE_DOWN_STEP * mem_param_t.image_height / mem_param_t.image_width;
                        }
                        mem_param_t.sensor_out_height = DCAMERA_HEIGHT(mem_param_t.sensor_out_height);
                    }

                }while(mem_param_t.sensor_out_width*DCAMERA_INTERPOLATION_COEF >= mem_param_t.image_width &&
                       mem_param_t.sensor_out_height*DCAMERA_INTERPOLATION_COEF >= mem_param_t.image_height);

                if(cpt_cnt)
                {
                    break;
                }
            }
        }

        if(0 == cpt_cnt)
        {
            ret_value = DCAMERA_OP_NO_ENOUGH_MEMORY;
        }
        else
        {
            p_snapsoh_ctrl->photo_size_max = dc_size;
        }

    }


    //DCAMERA_TRACE:"DC: _DCAMERA_GetCapability, photo size %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4764_112_2_18_1_24_8_415,(uint8*)"d",p_snapsoh_ctrl->photo_size_max);

    return ret_value;

}

LOCAL void _DCAMERA_ContextInit(void)
{
    s_dc_info_ptr = &s_dcamera_info;
    SCI_MEMSET(s_dc_info_ptr, 0, sizeof(DCAMERA_INFO_T));
    s_dc_info_ptr->preview_ctl_info.brightness_level = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.contrast_level = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.sharpness_level = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.saturation_level = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.exposure_comp_level = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.video_mode = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.meter_param.meter_mode = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.wb_mode = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.flicker_mode = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->preview_ctl_info.iso_level = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->isp_preview_param.disp_lcd_id = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->isp_preview_param.disp_block_id = DCAMERA_PARAM_INVALID_VALUE;
    s_dc_info_ptr->sensor_rot = DCAMERA_ROTATION_0;

    if(PNULL == s_capture_done_sema_ptr)
    {
        s_capture_done_sema_ptr = SCI_CreateSemaphore("Semaphore for Capture done", 0);
    }

    if(PNULL == s_callback_mutex_ptr)
    {
        s_callback_mutex_ptr = SCI_CreateMutex("Mutex for DC callback protection", SCI_INHERIT);
    }
    if (SCI_NULL == s_dc_event)
    {
        s_dc_event = SCI_CreateEvent("DC_EVENT");
    }
    s_dc_is_open = SCI_TRUE;
}

LOCAL void _DCAMERA_ContextDeInit(void)
{
    ISP_PREVIEW_PARAM_T *p_para = (ISP_PREVIEW_PARAM_T*)&s_dc_info_ptr->isp_preview_param;

    if(p_para->disp_lcd_id < LCD_SUPPORT_MAX &&
       p_para->disp_block_id < BLOCK_SUM)
    {
        LCD_DisableBlock(p_para->disp_lcd_id, p_para->disp_block_id);
    }

    _DCAMERA_SnapshotExFreeMem();

    s_dc_info_ptr = PNULL;
    s_dc_is_open = SCI_FALSE;
    if(PNULL != s_capture_done_sema_ptr)
    {
        SCI_DeleteSemaphore(s_capture_done_sema_ptr);
        s_capture_done_sema_ptr = PNULL;
    }
    if(PNULL != s_callback_mutex_ptr)
    {
        SCI_DeleteMutex(s_callback_mutex_ptr);
        s_callback_mutex_ptr = PNULL;
    }
    dv_zoom_level_last_time = DCAMERA_PREVIEW_ZOOM_LEVEL_MAX;
}

LOCAL uint8 DC_GetSensorSize(void)
{
    SENSOR_EXP_INFO_T* sensor_info_ptr = Sensor_GetInfo();
    uint16 sensor_width = sensor_info_ptr->source_width_max;
    uint16 sensor_height = sensor_info_ptr->source_height_max;
    uint32 sensor_type = 0;
    DCAMERA_SIZE_T *p_sensor_size = SCI_NULL;

    /* find the sensor type according to the width */
    for(sensor_type = 0; sensor_type < DCAMERA_PHOTO_SIZE_MAX; sensor_type++)
    {
        p_sensor_size = DC_GetSensorSizeStPtr(sensor_type);
        if((sensor_width == p_sensor_size->w) &&
           (sensor_height == p_sensor_size->h))
        {
            break;
        }
    }

    return sensor_type;
}

LOCAL void _DCAMERA_PreviewSensorSet(void)
{
    DCAMERA_PREVIEW_CTL_INFO_T  *preview_ctl_info_ptr   = PNULL;

    if(SCI_NULL == s_dc_info_ptr)
        return ;

    // Sesnor Step 5: Set resolution of sensor in preview mode
    Sensor_SetMode(s_dc_info_ptr->preview_m);

    preview_ctl_info_ptr = &s_dc_info_ptr->preview_ctl_info;

    if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_TESTPATTERN_MODE, preview_ctl_info_ptr->enviroment_mode))
    {
        //SCI_TRACE_LOW("_DCAMERA_PreviewSensorSet SENSOR_IOCTL_TEST_PATTERN_MODE");
        Sensor_Ioctl(SENSOR_IOCTL_TEST_PATTERN_MODE, preview_ctl_info_ptr->enviroment_mode);
    }

    if(DCAMERA_PARAM_INVALID_VALUE != preview_ctl_info_ptr->meter_param.meter_mode)
    {
        uint32  meter_set[SENSOR_IOCTRL_PARAM_LENGTH] = {0};
        SENSOR_METER_MODE_E sensor_meter_mode = SENSOR_METER_MODE_MAX;

        switch(preview_ctl_info_ptr->meter_param.meter_mode)
        {
            case DCAMERA_METER_MODE_FULL:
                sensor_meter_mode = SENSOR_METER_MODE_FULL;
                break;
            case DCAMERA_METER_MODE_CENTER:
                sensor_meter_mode = SENSOR_METER_MODE_CENTER;
                break;
            case DCAMERA_METER_MODE_SPOT:
                sensor_meter_mode = SENSOR_METER_MODE_SPOT;
                break;
            case DCAMERA_METER_MODE_MATRIX:
                sensor_meter_mode = SENSOR_METER_MODE_MATRIX;
                break;
            default:
                break;
        }

        if(SENSOR_METER_MODE_MAX == sensor_meter_mode)
        {
            //DCAMERA_TRACE:"_DCAMERA_PreviewSensorSet:meter mode of sensor error,mode = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_4908_112_2_18_1_24_9_416,(uint8*)"d",preview_ctl_info_ptr->meter_param.meter_mode);
            return;
        }

        meter_set[0] = (uint32)sensor_meter_mode;
        if(SENSOR_METER_MODE_SPOT == sensor_meter_mode)
        {
            meter_set[1] = (uint32)preview_ctl_info_ptr->meter_param.meter_rect.x;
            meter_set[2] = (uint32)preview_ctl_info_ptr->meter_param.meter_rect.y;
            meter_set[3] = (uint32)preview_ctl_info_ptr->meter_param.meter_rect.w;
            meter_set[4] = (uint32)preview_ctl_info_ptr->meter_param.meter_rect.h;
        }
        Sensor_Ioctl( SENSOR_IOCTL_METER_MODE, (uint32)&meter_set[0]);

    }
    // Sensor Step 6: Change sensor to the type of enviroment mode ,such as normal, sunny or night
    if(DCAMERA_PARAM_INVALID_VALUE != preview_ctl_info_ptr->exposure_comp_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION, preview_ctl_info_ptr->exposure_comp_level))
        {
            Sensor_Ioctl( SENSOR_IOCTL_EXPOSURE_COMPENSATION, preview_ctl_info_ptr->exposure_comp_level );
        }
    }

    if(DCAMERA_WB_MODE_MAX > preview_ctl_info_ptr->wb_mode)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_EFFECT, preview_ctl_info_ptr->wb_mode))
        {
            Sensor_Ioctl(SENSOR_IOCTL_SET_WB_MODE, preview_ctl_info_ptr->wb_mode);

        }
    }
    // Sensor Step 7: set brightness,contrast, sharpness, saturation and effect type
    if(DCAMERA_PARAM_INVALID_VALUE != preview_ctl_info_ptr->brightness_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_BRIGHTNESS, preview_ctl_info_ptr->brightness_level))
        {
            Sensor_Ioctl( SENSOR_IOCTL_BRIGHTNESS, preview_ctl_info_ptr->brightness_level);
        }
    }

    if(DCAMERA_PARAM_INVALID_VALUE != preview_ctl_info_ptr->contrast_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_CONTRAST, preview_ctl_info_ptr->contrast_level))
        {
            Sensor_Ioctl( SENSOR_IOCTL_CONTRAST, preview_ctl_info_ptr->contrast_level);
        }
    }

    if(DCAMERA_PARAM_INVALID_VALUE != preview_ctl_info_ptr->sharpness_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_SHARPNESS, preview_ctl_info_ptr->sharpness_level))
        {
            Sensor_Ioctl( SENSOR_IOCTL_SHARPNESS, preview_ctl_info_ptr->sharpness_level);
        }
    }

    if(DCAMERA_PARAM_INVALID_VALUE != preview_ctl_info_ptr->saturation_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_SATURATION, preview_ctl_info_ptr->saturation_level))
        {
            Sensor_Ioctl( SENSOR_IOCTL_SATURATION, preview_ctl_info_ptr->saturation_level);
        }
    }

    if(DCAMERA_EFFECT_MAX > preview_ctl_info_ptr->effect_type)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_EFFECT, preview_ctl_info_ptr->effect_type))
        {
            Sensor_Ioctl( SENSOR_IOCTL_IMAGE_EFFECT, preview_ctl_info_ptr->effect_type );
        }
    }

    if(DCAMERA_FLICKER_MAX > preview_ctl_info_ptr->flicker_mode)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION, preview_ctl_info_ptr->flicker_mode))
        {
            Sensor_Ioctl( SENSOR_IOCTL_ANTI_BANDING_FLICKER, preview_ctl_info_ptr->flicker_mode);
        }
    }

    if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_PREVIEW_MODE, preview_ctl_info_ptr->enviroment_mode))
    {
        if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
        {
        }
        else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
        {
            Sensor_Ioctl(SENSOR_IOCTL_PREVIEWMODE, preview_ctl_info_ptr->enviroment_mode);
        }

    }

    if(DCAMERA_MODE_MAX > preview_ctl_info_ptr->video_mode)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_VIDEO_MODE, preview_ctl_info_ptr->video_mode))
        {
            Sensor_Ioctl( SENSOR_IOCTL_VIDEO_MODE, preview_ctl_info_ptr->video_mode );
        }
    }

    if(DCAMERA_ROTATION_0 < s_dc_info_ptr->sensor_rot &&
       DCAMERA_ROTATION_MAX > s_dc_info_ptr->sensor_rot)
    {
        _DCAMERA_SetSensorRot(s_dc_info_ptr->sensor_rot);
    }

    //DCAMERA_TRACE:"_DCAMERA_PreviewSensorSet :: brightness %d, contrast %d, sharpness %d, sturation %d, effect %d, envir %d, wb_mode %d,zoom lel %d sensor_rot %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5012_112_2_18_1_24_9_417,(uint8*)"ddddddddd",preview_ctl_info_ptr->brightness_level,preview_ctl_info_ptr->contrast_level,preview_ctl_info_ptr->sharpness_level,preview_ctl_info_ptr->saturation_level,preview_ctl_info_ptr->effect_type,preview_ctl_info_ptr->enviroment_mode,preview_ctl_info_ptr->wb_mode,preview_ctl_info_ptr->zoom_level,s_dc_info_ptr->sensor_rot);

    return ;

}
/**---------------------------------------------------------------------------*
 **                         Global  Function definition                         *
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description: To check if digical camera is open
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     SCI_TRUE    has opened
//             SCI_FALSE   has not opened
// Note:
/******************************************************************************/
PUBLIC BOOLEAN DCAMERA_IsOpen(void)
{
    //DCAMERA_TRACE("DC: DCAMERA_IsOpen -> s_dc_is_open = %d", s_dc_is_open);
    return ((SCI_TRUE == s_dc_is_open)  );//&& (PNULL != s_dc_info_ptr));
}

/******************************************************************************/
// Description: create and initialize the dc context
// Author:     jing.li
// Input:      none
// Output:     s_pst_context
// Return:     0           successful
//             others      failed
// Note:       this function should be called before all other functions
/******************************************************************************/
PUBLIC uint32 DC_CreateContext(void)
{
    uint32 nRet = SCI_SUCCESS;

    return nRet;
}


int dcamera_opencamera()
{
    /*20190228-xin.xu1 -code review*/
    int ret = 0;

//SCI_TRACE_LOW("davidtest open camera");
        sensor_powerup();
        ret = _ISP_UixCaptureOneFrame();

        SCI_TRACE_LOW("DC:test open camera over");
        return ret;
}


PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Init(BOOLEAN if_force_calib, DCAMERA_CAM_PARAM_T *p_Param)
{
    DCAMERA_RETURN_VALUE_E rtn = DCAMERA_OP_SUCCESS;
    uint32                              i = 0;
    uint8                               sensor_index[SENSOR_ID_MAX] = {0xFF, 0xFF, 0xFF};

    DCAMERA_TRACE("DC: Init if_force_calib %d! nv sign 0x%x", if_force_calib, p_Param->cam_val_nv.sensor_nv_sign);

    if(DCAMERA_SENSOR_IDENTIFIED_SIGN != p_Param->cam_val_nv.sensor_nv_sign     \
        || if_force_calib                                                                                        \
        ||(!Sensor_CheckIfHaveCamera()))
    {
        rtn = Sensor_SetSensorType(SENSOR_TYPE_IMG_SENSOR);
        rtn = (DCAMERA_RETURN_VALUE_E)Sensor_Init();
        Sensor_RestoreStateFromInit();
        if(rtn)
        {
            DCAMERA_TRACE("DC:Init sensor_init error 1");
            rtn = DCAMERA_OP_NO_SENSOR_ERR;
        }
        else
        {
            for(i=0; i<SENSOR_ID_MAX; i++)
            {
                Sensor_GetIdentifiedState((SENSOR_ID_E)i, &p_Param->cam_val_nv.sensor_index[i]);
            }
        }
        p_Param->is_need_save = SCI_TRUE;
        p_Param->cam_val_nv.sensor_nv_sign = DCAMERA_SENSOR_IDENTIFIED_SIGN;
    }
    else
    {
        Sensor_SetIdentifiedState(SCI_TRUE, p_Param->cam_val_nv.sensor_index);
        rtn = (DCAMERA_RETURN_VALUE_E)Sensor_Init();
        Sensor_RestoreStateFromInit();
        if(rtn)
        {
            DCAMERA_TRACE("DC:Init sensor_init error 2");
            rtn = DCAMERA_OP_NO_SENSOR_ERR;
        }
        else
        {
            for(i=0; i<SENSOR_ID_MAX; i++)
            {
                Sensor_GetIdentifiedState((SENSOR_ID_E)i, &sensor_index[i]);
            }
        }

        p_Param->is_need_save = SCI_FALSE;

        //if camera changed then refresh the NV sensor ID
        if(sensor_index[SENSOR_MAIN] != p_Param->cam_val_nv.sensor_index[SENSOR_MAIN] ||  \
            sensor_index[SENSOR_SUB] != p_Param->cam_val_nv.sensor_index[SENSOR_SUB])
        {
            p_Param->cam_val_nv.sensor_index[SENSOR_MAIN] = sensor_index[SENSOR_MAIN];
            p_Param->cam_val_nv.sensor_index[SENSOR_SUB] = sensor_index[SENSOR_SUB];
            p_Param->is_need_save = SCI_TRUE;
        }
    }
        //SCI_TRACE_LOW("david isp_service init!");
        ISP_ServiceInit();
        Dcamera_init_config();
        SCI_TRACE_LOW("DC: init over");

        return rtn;
}





/******************************************************************************/
// Description: Open digital Camera function
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       before start preview, call this function to open digital camera
/******************************************************************************/

PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Open(DCAMERA_OP_MODE_E op_mode)
{
    DCAMERA_RETURN_VALUE_E ret_value = DCAMERA_OP_SUCCESS;
    uint32                 mem_start = 0, mem_size = 0;
    LCD_INFO_T             lcd_info = {0};
    uint32                 isp_ret = 0;

    //DCAMERA_TRACE:"DCAMERA_Open, mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5061_112_2_18_1_24_9_418,(uint8*)"d",op_mode);

    DPSLP_DCAM_EnableAhbSleep(DISABLE_AHB_SLEEP);

        SCI_TRACE_LOW("DC: %s 25 camera memAvalid=%d,totalmemAvalid=%d", __FUNCTION__,SCI_GetByteHeapAvailableSpace(DYNAMIC_HEAP_APP_MEMORY),SCI_GetHeapTotalAvailableSpace(DYNAMIC_HEAP_APP_MEMORY) );

        ISP_ServiceInit();

    /*
     * used for debug
     * ISP_ServiceSetDumpMode:
     * mode: DCAMERA_DUMP_FILE, the yuv data will be dumped to the UDisk,
             DCAMERA_DUMP_DDR, the yuv data will be saved to memory and can be parsed by trace32
     * ISP_ServiceSetDumpFrameNum:
     * set the num of frames by ISP_ServiceSetDumpFrameNum, if the parameter is zero,
     * it indicates that there is no limit to the number of dump frames
    */
    //ISP_ServiceSetDumpMode(DCAMERA_DUMP_FILE);
    //ISP_ServiceSetDumpFrameNum(0);

    if(DCAMERA_OP_NORMAL == op_mode)
    {
        ISP_UixSetDcamMode(DCAMERA_MODE_PREVIEW);
    }
    else if(DCAMERA_OP_DV == op_mode)
    {
        ISP_UixSetDcamMode(DCAMERA_MODE_RECORD);
    }
    else if(DCAMERA_OP_VIDEOCALL == op_mode)
    {
        ISP_UixSetDcamMode(DCAMERA_MODE_VIDEOCALL);
    }

    //视频通话模式下不操作LCD
	if(DCAMERA_OP_VIDEOCALL != op_mode)
	{
	    LCD_GetInfo(LCD_ID_0, &lcd_info);
	    if(lcd_info.is_use_fmark)
	    {
	        //SCI_TRACE_LOW:"-----open fmark------"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5068_112_2_18_1_24_9_419,(uint8*)"");
	        LCD_SetFmarkMode(LCD_ID_0,LCD_FMARK_DEVICE,(LCD_FMARK_POL_E)lcd_info.fmark_pol);
	        _DCAMERA_SelectFmarkPin(1);
	    }
	}

    if(!DCAMERA_IsOpen())
    {
        s_operation_mode = op_mode;

        _DCAMERA_ContextInit();
        ret_value = _DCAMERA_InitSensor();
		if(DCAMERA_OP_SUCCESS != ret_value)
        {
            _DCAMERA_Close();
            return ret_value;
        }

        mem_start = DCamera_MemConfig(op_mode, &mem_size);
        if(0 == mem_start || 0 == mem_size)
        {
            //DCAMERA_TRACE:"DC: DCAMERA_Open, memory error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5088_112_2_18_1_24_9_420,(uint8*)"");
            _DCAMERA_Close();
            return DCAMERA_OP_NO_ENOUGH_MEMORY;
        }

        ISP_Uix_SetBaseaddr(mem_start, mem_size);
        ret_value = _DCAMERA_GetCapability(op_mode);//must be called after Sensor has been initialized
        if(DCAMERA_OP_SUCCESS != ret_value)
        {
            _DCAMERA_Close();
            return ret_value;
        }

        _DCAMERA_CreateThread();

        if(DCAMERA_OP_VIDEOCALL != op_mode)
        {
            LCD_SetOpMode(MAIN_LCD_ID, OSD_OPERATION);
            //LCD_SetOpMode(SUB_LCD_ID, OSD_OPERATION);

            //SCI_TRACE_LOW("[DC]  DCAMERA_Open :::: LCD_SetOpMode  OSD_OPERATION");
            if((DCAMERA_OP_ATV_FACTORY == op_mode) || (DCAMERA_OP_UPCC == op_mode))
            {
                SCI_TRACE_LOW("DC:Open PERMIT_GUI_INVALIDATE");
                LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
            }
            else
            {
                SCI_TRACE_LOW("DC:Open NO_PERMIT_GUI_INVALIDATE");
                LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);
            }
        }

        if(op_mode == DCAMERA_OP_DV ||
           op_mode == DCAMERA_OP_VT ||
           op_mode == DCAMERA_OP_ATV||
           op_mode == DCAMERA_OP_MINIDV||
           op_mode == DCAMERA_OP_UPCC)
        {
        	ISP_UixSetDcamMode(DCAMERA_MODE_RECORD);
            isp_ret = _DCAMERA_OpenISP();
            if(SCI_SUCCESS == isp_ret)
            {
                ISP_ServiceMemConfig((void*)mem_start, mem_size);
            }
            else
            {
                _DCAMERA_Close();
                return isp_ret;
            }
        }

        SCI_ResumeThread(s_dc_task_id);
    }

    return DCAMERA_OP_SUCCESS;
}

PUBLIC uint32 _DCAMERA_TaskClose(void)
{
    DCAMERA_SIGNAL_T * dc_sig_ptr = SCI_NULL;

    if(SCI_INVALID_BLOCK_ID == s_dc_task_id)
        return  SCI_SUCCESS;

    DCAMERA_DeleteTimer();

    _DCAMERA_GetMutex(s_callback_mutex_ptr);

    ImgSensor_GetMutex();

    SCI_SuspendThread(s_dc_task_id);

    // Empty the signal queue
    while(1)/*lint !e716*/
    {
        dc_sig_ptr = (void*)SCI_PeekSignal(s_dc_task_id);
        if (dc_sig_ptr != PNULL)
        {
            SCI_FREE(dc_sig_ptr);
        }
        else
        {
            break;
        }
    }

    SCI_TerminateThread(s_dc_task_id);
    SCI_DeleteThread(s_dc_task_id);

    ImgSensor_PutMutex();
    ImgSensor_DeleteMutex();

    s_dc_task_id = SCI_INVALID_BLOCK_ID;
	_DCAMERA_PutMutex(s_callback_mutex_ptr);
    return SCI_SUCCESS;
}

LOCAL void _DCAMERA_Reset_FlashLight(void)
{
    DCAMERA_PREVIEW_CTL_INFO_T *preview_ctl_info_ptr = &s_dc_info_ptr->preview_ctl_info;
    if(s_dc_info_ptr)
    {
        if(preview_ctl_info_ptr->is_flashlight_enable)
        {
            Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_TRUE);
        }
        else
        {
            Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_FALSE);
        }
    }
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_Close(void)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    LCD_INFO_T lcd_info = {0};

    //DCAMERA_TRACE:"DCAMERA_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5174_112_2_18_1_24_9_421,(uint8*)"");

    LCD_GetInfo(LCD_ID_0,&lcd_info);
    if(lcd_info.is_use_fmark)
    {
        //SCI_TRACE_LOW:"-----close fmark------"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5179_112_2_18_1_24_9_422,(uint8*)"");
        LCD_SetFmarkMode(LCD_ID_0,LCD_FMARK_NONE,(LCD_FMARK_POL_E)lcd_info.fmark_pol);
    }

    if(DCAMERA_IsOpen())
    {
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_ENCODE);

        if(DCAMERA_OP_ATV == s_operation_mode)
        {
            ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_POSTCAP);
        }
        _DCAMERA_PutMutex(s_callback_mutex_ptr);

        _DCAMERA_TaskClose();

        _DCAMERA_CloseISP();

        _DCAMERA_Reset_FlashLight();

        _DCAMERA_CloseSensor();

        _DCAMERA_ContextDeInit();
  #if defined (ATV_WORKAROUND)

        if(s_operation_mode == DCAMERA_OP_ATV)
        {
           CARD_SDIO_SwichBaseClk((CARD_SDIO_SLOT_NO)CARD_SDIO_SLOT_0, SDIO_BASE_CLK_48M);
           SDIO_Card_Pal_SetClk(SDIO_Card_PAL_Get_Handle((SDIO_CARD_PAL_SLOT_E)CARD_SDIO_SLOT_0), SDIO_CARD_PAL_50MHz);

        }
  #endif
        DCamera_MemFree_Ybuffer();


  	    DCamera_MemFree();

        if(g_pic_buffer != NULL)
        {
            SCI_Free((void*)g_pic_buffer);
            g_pic_buffer = NULL;
            SCI_TRACE_LOW("DC:close free pic buffer.");
        }

        LCD_SetOpMode(MAIN_LCD_ID, UI_OPERATION);
    }

    ISP_UixSetDcamMode(DCAMERA_MODE_PREVIEW);
    DCAMERA_UnRigisterVideoChatCallback();

    LCD_SetCopyOsdMode(MAIN_LCD_ID,SCI_TRUE);
    LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
    LCD_SetOpMode(MAIN_LCD_ID, UI_OPERATION);
    //LCD_SetOpMode(SUB_LCD_ID, UI_OPERATION);
    DPSLP_DCAM_EnableAhbSleep(ENABLE_AHB_SLEEP);

    return eRet;
}

/******************************************************************************/
// Description: Close digital Camera function
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Close(void)
{
    return _DCAMERA_Close();
}

/******************************************************************************/
// Description: Get some DC information
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_GetInfo
    (
        DCAMERA_INFO_CMD_E          param_cmd,
        uint32*                     info_ptr
    )
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    SENSOR_EXP_INFO_T      *p_sensor_info = s_sensor_ptr;

    if(SCI_NULL == p_sensor_info)
    {
        return DCAMERA_OP_NO_SENSOR_ERR;
    }

    switch(param_cmd)
    {
        case DCAMERA_INFO_BRIGHTNESS:
        {
            (*info_ptr) = (uint32)(p_sensor_info->step_count & 0x0f);
        }
        break;

        case DCAMERA_INFO_CONTRAST:
        {
            (*info_ptr) = (uint32)((p_sensor_info->step_count & 0xf0) >> 0x04);
        }
        break;

        case DCAMERA_INFO_SHARPNESS:
        {
            (*info_ptr) = (uint32)((p_sensor_info->step_count & 0xf00) >> 0x08);
        }
        break;

        case DCAMERA_INFO_SATURATION:
        {
            (*info_ptr) = (uint32)((p_sensor_info->step_count & 0xf000) >> 0x0c);
        }
        break;

        case DCAMERA_INFO_EXPOSURE_COMPENSATION:
        {
            (*info_ptr) = (uint32)((p_sensor_info->step_count & 0xf0000) >> 0x10);
        }
        break;

        case DCAMERA_INFO_ZOOM:
        {
            DCAMERA_RECT_T      dc_rect = {0};
            uint32              zoom_lvl_x = 0;
            uint32              zoom_lvl_y = 0;

           // _DCAMERA_HandleSensorOutputSize(s_dc_info_ptr->jpeg_rect);

            dc_rect.w = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
            dc_rect.h = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;

            if(0 == dc_rect.w || 0 == dc_rect.h)
            {
                dc_rect.w = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].width;
                dc_rect.h = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].height;
            }

            if(0 == dc_rect.w || 0 == dc_rect.h)
            {
                (*info_ptr) = 0;
            }
            else
            {
                zoom_lvl_x = (uint32)(s_dc_info_ptr->jpeg_rect.w / DCAMERA_INTERPOLATION_COEF);
                zoom_lvl_x = (uint32)((zoom_lvl_x * DCAMERA_PREVIEW_ZOOM_LEVEL_MAX * 2) / dc_rect.w);
                zoom_lvl_x = (uint32)(DCAMERA_PREVIEW_ZOOM_LEVEL_MAX * 2) - zoom_lvl_x;

                zoom_lvl_y = (uint32)(s_dc_info_ptr->jpeg_rect.h / DCAMERA_INTERPOLATION_COEF);
                zoom_lvl_y = (uint32)((zoom_lvl_y * DCAMERA_PREVIEW_ZOOM_LEVEL_MAX * 2) / dc_rect.h);
                zoom_lvl_y = (uint32)(DCAMERA_PREVIEW_ZOOM_LEVEL_MAX * 2) - zoom_lvl_y;

                zoom_lvl_x = zoom_lvl_x > zoom_lvl_y ? zoom_lvl_y : zoom_lvl_x;

                (*info_ptr) = zoom_lvl_x > DCAMERA_PREVIEW_ZOOM_LEVEL_MAX ? DCAMERA_PREVIEW_ZOOM_LEVEL_MAX : zoom_lvl_x;

                if(DCAMERA_IsSupport2MSnapshot() && (s_dc_info_ptr->jpeg_rect.w > VGA_WIDTH))
                {
                    (*info_ptr) = 0;
                }

                if((DCAMERA_IMG_SENSOR_TYPE_3P4 == _DC_GetImgSensorType()) &&
                   ((s_dc_info_ptr->jpeg_rect.w > VGA_WIDTH)||(DCAMERA_OP_DV == s_operation_mode)||
                   (DCAMERA_OP_MINIDV == s_operation_mode)))
                {
                    (*info_ptr) = 0;
                }
            }

        }
        break;

        case DCAMERA_INFO_EFFECT:
        {
            *info_ptr = (0x1 << DCAMERA_EFFECT_NORMAL)
                |(0x1 << DCAMERA_EFFECT_BLACKWHITE)
                |(0x1 << DCAMERA_EFFECT_RED)
                |(0x1 << DCAMERA_EFFECT_GREEN)
                |(0x1 << DCAMERA_EFFECT_BLUE)
                |(0x1 << DCAMERA_EFFECT_YELLOW) //Tim.zhu@20080602 for tim.zhu_cr116992
                |(0x1 << DCAMERA_EFFECT_NEGATIVE)
                |(0x1 << DCAMERA_EFFECT_CANVAS)
                ;
        }
        break;

        case DCAMERA_INFO_PRVIEW_MODE:
        {
            *info_ptr = (0x1 << DCAMERA_ENVIRONMENT_NORMAL)
                |(0x1 << DCAMERA_ENVIRONMENT_NIGHT)
                |(0x0 << DCAMERA_ENVIRONMENT_SUNNY)
                ;
        }
        break;

        case DCAMERA_INFO_WB_MODE:
        {
            *info_ptr = (0x1 << DCAMERA_WB_MODE_AUTO)
                |(0x1 << DCAMERA_WB_MODE_INCANDESCENCE)
                |(0x1 << DCAMERA_WB_MODE_U30)
                |(0x1 << DCAMERA_WB_MODE_CWF)
                |(0x1 << DCAMERA_WB_MODE_FLUORESCENT)
                |(0x1 << DCAMERA_WB_MODE_SUN)
                |(0x1 << DCAMERA_WB_MODE_CLOUD);
        }
        break;

        case DCAMERA_INFO_SENSOR_SIZE:
        {
            *info_ptr = (1 << (s_dcamera_info.snapshot_ctl_info.photo_size_max + 1)) - 1;

            //DCAMERA_TRACE:"DC: DCAMERA_GetInfo, sensor size 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5337_112_2_18_1_24_9_424,(uint8*)"d",*info_ptr);
        }
        break;

        case DCAMERA_INFO_BURST_PERMIT_NUM:
        {
            DCAMERA_RECT_T              jpg_rect = {0};
            DCAMERA_SIZE_T              *p_size = SCI_NULL;
            uint32                      cpt_cnt     = 0, i = 0;
            LCD_INFO_T                  lcd_info_t = {0};
            DCAMERA_MEM_PARAM_T         mem_param_t = {0};
            DCAMERA_SIZE_T              dc_dst_size = {0};
            DCAMERA_RECT_T              dc_rect = {0};
            DC_PRODUCT_CFG_T_PTR        p_dc_cfg = DC_GeProductCfgPtr();
            DCAMERA_BURST_INPUT_T       dc_bust_info = {0};

            if(SCI_NULL == info_ptr ||
               *info_ptr >= DCAMERA_PHOTO_SIZE_MAX)
            {
                eRet = DCAMERA_OP_PARAM_ERR;
            }
            else
            {
                dc_bust_info = *(DCAMERA_BURST_INPUT_T *)info_ptr;        //get the dc operation info

                //if rotation not set then set the rotation angel to 0
#if 0
                if((dc_bust_info.cap_rot<DCAMERA_ROTATION_90)||
                    (dc_bust_info.cap_rot>DCAMERA_ROTATION_FLIP))
                {
                    dc_bust_info.cap_rot = DCAMERA_ROTATION_0;
                }

                if((dc_bust_info.review_rot<DCAMERA_ROTATION_90)||
                    (dc_bust_info.review_rot>DCAMERA_ROTATION_FLIP))
                {
                    dc_bust_info.review_rot = DCAMERA_ROTATION_0;
                }
#else
                dc_bust_info.cap_rot = DCAMERA_ROTATION_0;
                dc_bust_info.review_rot = DCAMERA_ROTATION_0;
#endif
                p_size = DC_GetSensorSizeStPtr(dc_bust_info.size_type);
                jpg_rect.w = p_size->w;
                jpg_rect.h = p_size->h;
                _DCAMERA_HandleSensorOutputSize(jpg_rect);

                mem_param_t.sensor_out_width = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
                mem_param_t.sensor_out_height = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;

                dc_rect.w = mem_param_t.sensor_out_width;
                dc_rect.h = mem_param_t.sensor_out_height;
                dc_dst_size.w = p_size->w;
                dc_dst_size.h = p_size->h;
                _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);

                mem_param_t.sensor_out_width = dc_rect.w;
                mem_param_t.sensor_out_height = dc_rect.h;
                mem_param_t.image_width = p_size->w;
                mem_param_t.image_height = p_size->h;
                if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format)
                {
                    mem_param_t.sensor_out_width >>= 1;
                    mem_param_t.sensor_out_height >>= 1;
                    mem_param_t.is_jpg_sensor = 1;
                }
                else
                {
                    mem_param_t.is_jpg_sensor = 0;
                }

                mem_param_t.capture_rot = (uint8)dc_bust_info.cap_rot;
                LCD_GetInfo(0, &lcd_info_t);
                mem_param_t.review_width = lcd_info_t.lcd_width;
                mem_param_t.review_height = lcd_info_t.lcd_height;
                mem_param_t.review_rot = (uint8)dc_bust_info.review_rot;

#if defined(_DC_EXIF_)
                if(p_dc_cfg->exif_eb)
                {
                    mem_param_t.b_add_exif = 1;
                }
                else
                {
                    mem_param_t.b_add_exif = 0;
                }
#endif

                if(p_dc_cfg->thumbnail_eb)
                {
                    mem_param_t.b_add_thumbnail = 1;
                }
                else
                {
                    mem_param_t.b_add_thumbnail = 0;
                }

                if(mem_param_t.image_width > ISP_SCALE_FRAME_MODE_WIDTH_TH)
                {
                    mem_param_t.slice_height = DC_SLICE_HEIGHT;
                }
                else
                {
                    mem_param_t.slice_height = mem_param_t.sensor_out_height;
                }
                mem_param_t.image_format = ISP_IMAGE_FORMAT_YUV422;

                do
                {
                    cpt_cnt = DCAMERA_CaptureMemReq(&mem_param_t);
                    if(cpt_cnt < 1)
                    {
                        eRet = DCAMERA_OP_NO_ENOUGH_MEMORY;
                    }
                    else
                    {
                        if(cpt_cnt >= 9)
                        {
                            *info_ptr = DC_CONTINUE_SHOOT_9;
                        }
                        else if(cpt_cnt >= 6)
                        {
                            *info_ptr = DC_CONTINUE_SHOOT_6;
                        }
                        else if(cpt_cnt >= 3)
                        {
                            *info_ptr = DC_CONTINUE_SHOOT_3;
                        }
                        else
                        {
                            *info_ptr = DC_CONTINUE_SHOOT_DISABLE;
                        }

                        eRet = DCAMERA_OP_SUCCESS;
                        s_dc_info_ptr->snapshot_ctl_info.sensor_out_width = mem_param_t.sensor_out_width;
                        s_dc_info_ptr->snapshot_ctl_info.sensor_out_height = mem_param_t.sensor_out_height;
                        break;
                    }

                    if(s_dc_info_ptr->snapshot_m > SENSOR_MODE_PREVIEW_ONE)
                    {
                        s_dc_info_ptr->snapshot_m --;

                        dc_rect.w = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
                        dc_rect.h = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;
                        _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);
                        mem_param_t.sensor_out_width = dc_rect.w;
                        mem_param_t.sensor_out_height = dc_rect.h;

                        if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format)
                        {
                            mem_param_t.sensor_out_width >>= 1;
                            mem_param_t.sensor_out_height >>= 1;
                            mem_param_t.is_jpg_sensor = 1;
                        }
                        else
                        {
                            mem_param_t.is_jpg_sensor = 0;
                        }
                    }
                    else
                    {
                        if(s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width > ISP_SCALE_FRAME_MODE_WIDTH_TH)
                        {
                            mem_param_t.sensor_out_width = ISP_SCALE_FRAME_MODE_WIDTH_TH - i * DCAMERA_ENC_IMAGE_DOWN_STEP;
                            mem_param_t.sensor_out_height = (uint32)(mem_param_t.sensor_out_width * dc_dst_size.h)/(uint32)dc_dst_size.w;
                            i ++;
                        }
                        else
                        {
                            mem_param_t.sensor_out_width  -= DCAMERA_ENC_IMAGE_DOWN_STEP;
                            mem_param_t.sensor_out_height -= DCAMERA_ENC_IMAGE_DOWN_STEP * dc_dst_size.h / dc_dst_size.w;
                        }
                        mem_param_t.sensor_out_height = DCAMERA_HEIGHT(mem_param_t.sensor_out_height);

                    }
                }while(mem_param_t.sensor_out_width*DCAMERA_INTERPOLATION_COEF >= mem_param_t.image_width &&
                       mem_param_t.sensor_out_height*DCAMERA_INTERPOLATION_COEF >= mem_param_t.image_height);

            }

        }

        break;


        case DCAMERA_INFO_IMG_SENSOR_NUM:
        {
            SENSOR_REGISTER_INFO_T_PTR p_reg_info = Sensor_GetRegisterInfo();

            if(p_reg_info == SCI_NULL)
            {
                eRet = DCAMERA_OP_NO_SENSOR_ERR;
            }
            else
            {
                *info_ptr = p_reg_info->img_sensor_num;
            }
        }
        break;

        case DCAMERA_INFO_CUR_IMG_SENSOR:
        {
            *info_ptr = s_sensor_id;
        }
        break;

        case DCAMERA_INFO_PREVIEW_ROT:

            if(_DCAMERA_IsPreview())
            {
                *info_ptr = s_dcamera_info.preview_ctl_info.rotation_mode;
            }
            else
            {
                eRet = DCAMERA_OP_ERROR;
            }
        break;

        case DCAMERA_INFO_ATV_CH_ALL_NUM:
        {
            uint32 param=(ATV_CMD_GET_ALL_CHN_NUM<<0x10)&0xffff0000;
            *info_ptr=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
        }
        break;

        case DCAMERA_INFO_ATV_RSSI:
        {
            uint32 param=(ATV_CMD_GET_RSSI<<0x10)&0xffff0000;
            *info_ptr=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
        }
        break;

        case DCAMERA_INFO_JPEG_SIZE:
        {
            DCAMERA_PARAM_PHOTO_SIZE_E  photo_size = 0;
            DCAMERA_SIZE_T              *p_size = SCI_NULL;

            if((!info_ptr) || (*(uint32*)info_ptr >= DCAMERA_PHOTO_SIZE_MAX))
            {
                eRet = DCAMERA_OP_PARAM_ERR;
            }
            else
            {
                photo_size = *(uint32*)info_ptr;
                p_size = DC_GetSensorSizeStPtr(photo_size);
                *info_ptr = (uint32)(p_size->w * p_size->h / DC_JPEG_COMPRESS_LEVEL);
            }
        }
        break;

        case DCAMERA_INFO_DISP_RECT:
        {
            DCAMERA_RECT_T rtn_rect = {0};
            DCAMERA_RECT_T_PTR rect_ptr = PNULL;
            rect_ptr = (DCAMERA_RECT_T *)info_ptr;
            rtn_rect.x = (uint16)(s_dc_info_ptr->isp_preview_param.disp_range.x);
            rtn_rect.y = (uint16)(s_dc_info_ptr->isp_preview_param.disp_range.y);
            rtn_rect.w = (uint16)(s_dc_info_ptr->isp_preview_param.disp_range.w);
            rtn_rect.h = (uint16)(s_dc_info_ptr->isp_preview_param.disp_range.h);
            *rect_ptr = rtn_rect;
        }
        break;

        case DCAMERA_INFO_SENSOR_TYPE:
        {
            *info_ptr = _DC_GetImgSensorType();
        }
        break;

        case DCAMERA_INFO_UPCC_SIZE:
        {
            (*info_ptr)=_DC_GetUpccSize();
        }
        break;

        case DCAMERA_INFO_UPCC_ROTATION:
        {
            (*info_ptr)=_DC_GetUpccRotationMode();
        }
        break;

        case DCAMERA_INFO_DC_PREVIEW_ROTATION:
        {
            (*info_ptr)=_DC_GetDcPreviewRotationMode();
        }
        break;

        case DCAMERA_INFO_DC_CAP_ROTATION:
        {
            (*info_ptr)=_DC_GetDcCapRotationMode();
        }
        break;

        default:
        {}
        break;
    }

    return eRet;
}

#ifdef DC_EX_DEBUG
void _DCAMERA_FFSSave(uint16* p_file_name, void* p_data_buf, uint32 buf_len)

{
    SFS_ERROR_E errCode;
    SFS_HANDLE  sfsHandle;
    uint32      bytes_written;

    //DCAMERA_TRACE:"DC: _DCAMERA_FFSSave, p_data_buf 0x%x buf_len 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5614_112_2_18_1_24_10_425,(uint8*)"dd",p_data_buf,buf_len);

    sfsHandle = SFS_CreateFile(p_file_name, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, 0, 0);
    errCode = SFS_WriteFile(sfsHandle, p_data_buf, buf_len, &bytes_written, 0);
    SFS_CloseFile(sfsHandle);
}

uint8       *jpg_img = {0};
uint32      jpg_length = 0;

uint32 image_notice_callback(uint32 ret_value, DCAMERA_IMAGE_PARAM_T *p_image_frame, void* p_param)
{
    uint32          i = 0;

    if(SCI_NULL == jpg_img)
    {
        jpg_img = SCI_ALLOCA(256*1024);
    }

    preview_count ++;
    if(b_image_cb_for_snapshot_ex || preview_count >= 20)
    {
        for(i = 0; i < p_image_frame->frame_count; i++)
        {
            //DCAMERA_TRACE:"DC: image_notice_callback, image count = %d, data_type %d, data_pattern %d ,width %d height %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5643_112_2_18_1_24_10_426,(uint8*)"ddddd", p_image_frame->frame_count,p_image_frame->frame_info_t[i].data_type,p_image_frame->frame_info_t[i].data_pattern.yuv_pattern,p_image_frame->frame_info_t[i].target_size.w,p_image_frame->frame_info_t[i].target_size.h);

            //DCAMERA_TRACE:"DC: image_notice_callback, p_user 0x%x, address 0x%x,0x%x,0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5649_112_2_18_1_24_10_427,(uint8*)"dddd", p_param,p_image_frame->dcamera_addr[i].chn1_addr,p_image_frame->dcamera_addr[i].chn2_addr,p_image_frame->dcamera_addr[i].chn3_addr);

            if(ex_snapshot_callback && p_image_frame->frame_info_t[i].data_type == DCAMERA_DATA_TYPE_YUV422)
            {
                _DCAMERA_FFSSave(L"D:\\photos\\yuv422_y.raw",
                                 p_image_frame->dcamera_addr[i].chn1_addr,
                                 p_image_frame->frame_info_t[i].target_size.w*p_image_frame->frame_info_t[i].target_size.h);
                _DCAMERA_FFSSave(L"D:\\photos\\yuv422_uv.raw",
                                 p_image_frame->dcamera_addr[i].chn2_addr,
                                 p_image_frame->frame_info_t[i].target_size.w*p_image_frame->frame_info_t[i].target_size.h);

            }
            if(ex_snapshot_callback && p_image_frame->frame_info_t[i].data_type == DCAMERA_DATA_TYPE_YUV420)
            {
                _DCAMERA_FFSSave(L"D:\\photos\\yuv420_y.raw",
                                 p_image_frame->dcamera_addr[i].chn1_addr,
                                 p_image_frame->frame_info_t[i].target_size.w*p_image_frame->frame_info_t[i].target_size.h);
                _DCAMERA_FFSSave(L"D:\\photos\\yuv420_uv.raw",
                                 p_image_frame->dcamera_addr[i].chn2_addr,
                                 p_image_frame->frame_info_t[i].target_size.w*p_image_frame->frame_info_t[i].target_size.h/2);

            }
            else if(ex_snapshot_callback && p_image_frame->frame_info_t[i].data_type == DCAMERA_DATA_TYPE_RGB565)
            {
                _DCAMERA_FFSSave(L"D:\\photos\\rgb565.raw",
                                 p_image_frame->dcamera_addr[i].chn1_addr,
                                 p_image_frame->frame_info_t[i].target_size.w*p_image_frame->frame_info_t[i].target_size.h*2);

            }
            else if(ex_snapshot_callback && p_image_frame->frame_info_t[i].data_type == DCAMERA_DATA_TYPE_JPEG)
            {
                _DCAMERA_FFSSave(L"D:\\photos\\test.jpg",
                                 p_image_frame->dcamera_addr[i].chn1_addr,
                                 p_image_frame->dcamera_addr[i].chn2_addr);

            }


            if(ex_snapshot_callback && p_image_frame->frame_info_t[i].data_type == DCAMERA_DATA_TYPE_JPEG)
            {
                SCI_MEMCPY((void*)(jpg_img+jpg_length),
                           (void*)p_image_frame->dcamera_addr[i].chn1_addr,
                           p_image_frame->dcamera_addr[i].chn2_addr);
                (*ex_snapshot_callback)(0,(void*)(jpg_img+jpg_length),p_image_frame->dcamera_addr[i].chn2_addr);
                jpg_length += p_image_frame->dcamera_addr[i].chn2_addr;
            }
        }
        b_image_cb_for_snapshot_ex = 0;
        preview_count = 0;
    }

    return 0;
}


uint32 preview_ex_display_callback(uint32 ret_value,
                                   DCAMERA_DISPALY_PARAM_T *p_disp_src,
                                   DCAMERA_DISPALY_PARAM_T *p_disp_dst,
                                   void* p_param)
{
    uint32          frm_size = 0;

    //DCAMERA_TRACE:"DC: preview_ex_display_callback, rtn %d ,p_param %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5711_112_2_18_1_24_10_428,(uint8*)"dd",ret_value,p_param);

    //DCAMERA_TRACE:"Src 0x%x 0x%x {%d %d}, Dst 0x%x 0x%x {%d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5721_112_2_18_1_24_10_429,(uint8*)"dddddddd",p_disp_src->yaddr,p_disp_src->uaddr,p_disp_src->width,p_disp_src->height,p_disp_dst->yaddr,p_disp_dst->uaddr,p_disp_dst->width,p_disp_dst->height);

    frm_size = (uint32)(p_disp_src->width * p_disp_dst->height);
    SCI_MEMCPY((void*)p_disp_dst->yaddr, (void*)p_disp_src->yaddr,frm_size);
    SCI_MEMCPY((void*)p_disp_dst->uaddr, (void*)p_disp_src->uaddr,frm_size);

    DCAMERA_DisplayOneFrame(p_disp_dst);
}

#endif


typedef  int (*DCAMERA_VIDEOCHAT_FUNC_PTR)(void*);
DCAMERA_VIDEOCHAT_FUNC_PTR  g_VideoChatFunc = NULL;


int DCAMERA_RigisterVideoChatCallback( int (*func)(void *))
{
    if(func)
    {
        g_VideoChatFunc = func;
    }

    return 0;
}

int DCAMERA_UnRigisterVideoChatCallback()
{
    g_VideoChatFunc = NULL;
    return 0;
}


/******************************************************************************/
// Description: Start to preview
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartPreview(DCAMERA_PREVIEW_PARAM_T const* param_ptr)
{
    DCAMERA_RETURN_VALUE_E      ret_value               = DCAMERA_OP_SUCCESS;
#if defined(DC_EX_DEBUG)

    DCAMERA_PREVIEW_EX_PARAM_T  preview_ex = {0};

    preview_ex.lcd_id = param_ptr->lcd_id;
    preview_ex.lcd_block_id = param_ptr->lcd_block_id;
    preview_ex.disp_mode = param_ptr->disp_mode;
    preview_ex.rotation_mode = DCAMERA_ROTATION_0;
    #if 0
    preview_ex.disp_rect = param_ptr->disp_rect;
    preview_ex.target_rect = param_ptr->target_rect;
    #else
    preview_ex.disp_rect.x = 0;
    preview_ex.disp_rect.y = 0;
    preview_ex.disp_rect.w = 240;
    preview_ex.disp_rect.h = 400;
    preview_ex.target_rect = preview_ex.disp_rect;
    #endif
    preview_ex.app_data_ptr = SCI_NULL;
    preview_ex.image_frame_count = 0;
    preview_ex.image_frame_info[0].target_size.w = 176;
    preview_ex.image_frame_info[0].target_size.h = 144;
    preview_ex.image_frame_info[0].rotation_mode = DCAMERA_ROTATION_0;
    preview_ex.image_frame_info[0].data_type = DCAMERA_DATA_TYPE_YUV420;
    preview_ex.image_frame_info[0].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;

    preview_ex.image_frame_info[1].target_size.w = 240;
    preview_ex.image_frame_info[1].target_size.h = 176;
    preview_ex.image_frame_info[1].rotation_mode = DCAMERA_ROTATION_180;
    preview_ex.image_frame_info[1].data_type = DCAMERA_DATA_TYPE_RGB565;

    preview_ex.image_frame_info[2].target_size.w = 352;//640; //352;
    preview_ex.image_frame_info[2].target_size.h = 288;//480; //288;
    preview_ex.image_frame_info[2].rotation_mode = DCAMERA_ROTATION_90;
    preview_ex.image_frame_info[2].data_type = DCAMERA_DATA_TYPE_YUV422;//DCAMERA_DATA_TYPE_RAW; //DCAMERA_DATA_TYPE_YUV422;
    preview_ex.image_frame_info[2].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;
    preview_ex.invalidatemode = DCAMERA_INVALIDATEMODE_EXTERN;
    preview_ex.image_notice_cb = image_notice_callback;
    preview_ex.disp_cb = preview_ex_display_callback;
    ret_value = DCAMERA_StartPreviewEX(&preview_ex);

#else
    if(DCAMERA_OP_ATV == s_operation_mode)
    {
        DCAMERA_INFO_T              *dc_info_ptr            = s_dc_info_ptr;
        DCAMERA_PREVIEW_EX_PARAM_T  preview_ex = {0};

        preview_ex.lcd_id = param_ptr->lcd_id;
        preview_ex.lcd_block_id = param_ptr->lcd_block_id;
        preview_ex.disp_rect = param_ptr->disp_rect;
        preview_ex.target_rect = param_ptr->target_rect;
        preview_ex.rotation_mode = param_ptr->rotation_mode;
        preview_ex.disp_mode = param_ptr->disp_mode;

        preview_ex.app_data_ptr = SCI_NULL;
        preview_ex.image_frame_count = 0;
        preview_ex.image_notice_cb = SCI_NULL;

        ret_value = DCAMERA_StartPreviewEX(&preview_ex);

        dc_info_ptr->atv_callback = param_ptr->dc_atv_callback;
#ifndef ATV_SUPPORT_NONE
        DCAMERA_TimerActive(ATV_GET_RSSI_TIMER, DCAMERA_AtvRssiHandler);
#endif
    }
    else
    {
        DCAMERA_INFO_T              *dc_info_ptr            = s_dc_info_ptr;
        ISP_PREVIEW_PARAM_T         *isp_preview_param_ptr  = PNULL;
        DCAMERA_PREVIEW_CTL_INFO_T  *preview_ctl_info_ptr   = PNULL;
        DCAMERA_SENSOR_INIT_T       sensor_init             = {0};
        uint32                      count                   = 0;
        DCAMERA_CONVERT_COOR_T      src_coor                = {0};
        DCAMERA_CONVERT_COOR_T      dst_coor                = {0};
        DCAMERA_ROTATION_MODE_E     sensor_rot              = DCAMERA_ROTATION_0;
        DCAMERA_ROTATION_MODE_E     disp_rot                = DCAMERA_ROTATION_0;

        //DCAMERA_TRACE:"DC: DCAMERA_StartPreview -> start time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5817_112_2_18_1_24_10_430,(uint8*)"d", SCI_GetTickCount());

        if(!DCAMERA_IsOpen())
        {
            return DCAMERA_OP_NOT_OPEN;
        }
        if(PNULL == s_sensor_ptr)
        {
            return DCAMERA_OP_NOT_OPEN;
        }

        #if 0
		if(ISP_UixGetCaptureMode())
		{
			_DCAMERA_CloseISP();
			ISP_UixEnableCaptureMode(0);
		}

        if(DCAMERA_OP_NORMAL == s_operation_mode||
           DCAMERA_OP_SPECIAL == s_operation_mode ||
           DCAMERA_OP_MINIDC == s_operation_mode)
        {
            if(_DCAMERA_OpenISP())
            {
                return DCAMERA_OP_ISP_ERR;
            }
        }
        #endif

/*  拍照->预览: 中间不会经过DCAMERA_close,DCAMERA_OPEN. 而是直接调用startpreview
 *              此时camera controller还是以拍照模式参数打开的，预览需要切换过来
 *  预览 视频通话 录像: 这三个功能之间的切换，经过了DCAMERA_close,DCAMERA_OPEN
 *                      在open函数中已经配置好模式，无需再切换
 */
        if(DCAMERA_MODE_SNAPSHOT == ISP_UixGetDcamMode())
        {
            _DCAMERA_CloseISP();
            ISP_UixSetDcamMode(DCAMERA_MODE_PREVIEW);
        }

        if(DCAMERA_OP_NORMAL == s_operation_mode||
           DCAMERA_OP_SPECIAL == s_operation_mode ||
           DCAMERA_OP_MINIDC == s_operation_mode ||
           DCAMERA_OP_VIDEOCALL == s_operation_mode)
        {
            if(_DCAMERA_OpenISP())
            {
                return DCAMERA_OP_ISP_ERR;
            }
        }

        SCI_ASSERT(NULL != param_ptr);/*assert verified*/

        //视频通话不操作lcd
        if(DCAMERA_OP_VIDEOCALL != s_operation_mode)
        {
            LCD_SetInvalidateLimit(param_ptr->lcd_id,NO_PERMIT_GUI_INVALIDATE);
            SCI_TRACE_LOW("DC]:StartPreview NO_PERMIT_GUI_INVALIDATE");

            LCD_DisableBlock(param_ptr->lcd_id, param_ptr->lcd_block_id);
            SCI_TRACE_LOW("DC:StartPreview lcd_id %d block_id %d ", param_ptr->lcd_id, param_ptr->lcd_block_id);
        }

        if(	DCAMERA_OP_VIDEOCALL == s_operation_mode && g_VideoChatFunc)
        {
            ISP_ServiceRegister( ISP_SERVICE_CALLBACK_DISPLAY, (int(*)())g_VideoChatFunc);
        }

        preview_ctl_info_ptr = &dc_info_ptr->preview_ctl_info;

        //DCAMERA_TRACE:"DC: display region , x %d,y %d,w %d,h %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5849_112_2_18_1_24_10_431,(uint8*)"dddd",param_ptr->disp_rect.x,param_ptr->disp_rect.y,param_ptr->disp_rect.w,param_ptr->disp_rect.h);

        SCI_ASSERT(param_ptr->disp_mode < DCAMERA_DISP_MODE_MAX);/*assert verified*/
        dc_info_ptr->preview_ctl_info.display_mode = param_ptr->disp_mode;
        dc_info_ptr->display_mode = param_ptr->disp_mode;

        // set preview rotation mode
        disp_rot = _DCAMERA_ROT_Convertor(param_ptr->rotation_mode, &sensor_rot);
        dc_info_ptr->preview_ctl_info.rotation_mode = disp_rot;
        dc_info_ptr->sensor_rot = (uint32)sensor_rot;

#if 0
        SCI_MEMCPY((void*)&dc_info_ptr->disp_rect,
                   (void*)&param_ptr->disp_rect,
                   sizeof(DCAMERA_RECT_T));

        SCI_MEMCPY((void*)&dc_info_ptr->target_rect,
                   (void*)&param_ptr->target_rect,
                   sizeof(DCAMERA_RECT_T));
#else

        src_coor.rect = param_ptr->disp_rect;
        src_coor.rot_degree = param_ptr->rotation_mode;
        DCAMERA_Convert_Coor(param_ptr->lcd_id, &src_coor, &dst_coor);
        dc_info_ptr->disp_rect = dst_coor.rect;

        src_coor.rect = param_ptr->target_rect;
        src_coor.rot_degree = param_ptr->rotation_mode;
        DCAMERA_Convert_Coor(param_ptr->lcd_id, &src_coor, &dst_coor);
        dc_info_ptr->target_rect = dst_coor.rect;

#endif

    /*
        // Set zoom level to 0 as normal
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_ZOOM, preview_ctl_info_ptr->zoom_level))
        {
            dc_info_ptr->preview_ctl_info.zoom_level = preview_ctl_info_ptr->zoom_level;
        }
    */
        // Set distortion mode in dc information structure


        // init snesor
        sensor_init.preview_mode     = DCAMERA_PREVIEW_MODE_IGNORE;
        sensor_init.enviroment_mode  = preview_ctl_info_ptr->enviroment_mode;
        //sensor_init.image_format     = param_ptr->image_format;

        // handle target size and get preivew mode
        _DCAMERA_HandleSensorOutputSize(dc_info_ptr->target_rect);

        _DCAMERA_PreviewSensorSet();
        // Sensor Step 8: isp driver
        isp_preview_param_ptr = &dc_info_ptr->isp_preview_param;

        // Set LCD and Block ID
        isp_preview_param_ptr->disp_lcd_id    = param_ptr->lcd_id;
        isp_preview_param_ptr->disp_block_id  = param_ptr->lcd_block_id;

        while(count < DCAMERA_CAMERA_TRY_COUNT)
        {
            count++;

            if((s_sensor_ptr->source_width_max > UXGA_WIDTH) && (SENSOR_IMAGE_FORMAT_RAW == s_sensor_ptr->image_format))
            {
                ISP_ServiceSetClk(96);
            }

            ret_value = _DCAMERA_SetPreviewZoom(preview_ctl_info_ptr->zoom_level);  // in normal mode
            if(DCAMERA_OP_ISP_ERR != ret_value)
            {
                SCI_TRACE_LOW("DC:PreviewZoom ok");
                _DCAMERA_SetPreviewFlag(SCI_TRUE);
                if(DCAMERA_FLASH_ON == preview_ctl_info_ptr->flash_mode)
                {
                    Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_TRUE);
                }
                else if(DCAMERA_FLASH_OFF == preview_ctl_info_ptr->flash_mode ||
                        DCAMERA_FLASH_AUTO == preview_ctl_info_ptr->flash_mode)
                {
                    Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_FALSE);
                }
                break;
            }
            else
            {
                zoom_level_last_time = DCAMERA_PREVIEW_ZOOM_LEVEL_MAX;

                ISP_ServiceStopPreview();
                SCI_Sleep(100);
            }
        }

        if(DCAMERA_OP_SUCCESS != ret_value)
        {
                SCI_TRACE_LOW("DC:PreviewZoom error");
            //CAMERA_CloseSensor();

        }
        _DCAMERA_SetIsStopPreview(SCI_FALSE);
        dc_info_ptr->b_preview_from = 0;

        //DCAMERA_TRACE:"DC: DCAMERA_StartPreview -> end time = %d, return value %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5943_112_2_18_1_24_11_432,(uint8*)"dd", SCI_GetTickCount(), ret_value);
    }
    return ret_value;
#endif
}



LOCAL  BOOLEAN                _DCAMERA_PreviewCheckParam(DCAMERA_PREVIEW_EX_PARAM_T const* param_ptr)
{
    LCD_INFO_T          lcd_info_t = {0};

    LCD_GetInfo(param_ptr->lcd_id, &lcd_info_t);

    //DCAMERA_TRACE:"DC: _DCAMERA_PreviewCheckParam disp_mode %d, rot %d, disp_rect {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5961_112_2_18_1_24_11_433,(uint8*)"dddddd", param_ptr->disp_mode, param_ptr->rotation_mode,param_ptr->disp_rect.x,param_ptr->disp_rect.y,param_ptr->disp_rect.w,param_ptr->disp_rect.h);
    if(param_ptr->disp_mode >= DCAMERA_DISP_MODE_MAX)
    {
        return SCI_FALSE;
    }

    if(param_ptr->rotation_mode >= DCAMERA_ROTATION_MAX)
    {
        return SCI_FALSE;
    }

    if(param_ptr->disp_rect.w % DCAMERA_PIXEL_ALIGNED ||
       param_ptr->disp_rect.h % DCAMERA_PIXEL_ALIGNED ||
       param_ptr->disp_rect.x % 2 ||
       param_ptr->disp_rect.y % 2)
    {
        return SCI_FALSE;
    }

    return SCI_TRUE;
}

LOCAL  BOOLEAN                _DCAMERA_PreviewEXCheckParam(DCAMERA_PREVIEW_EX_PARAM_T const* param_ptr)
{
    DCAMERA_IMAGE_FRAME_PARAM_T     *p_img_frame = SCI_NULL;
    uint32                          i = 0;

    if(SCI_NULL == param_ptr->image_notice_cb ||
       0 == param_ptr->image_frame_count)
    {
        return SCI_FALSE;
    }

    if(param_ptr->image_frame_count >= DCAMERA_TARGET_FRAME_MAX)
    {
        //DCAMERA_TRACE:"DC: _DCAMERA_PreviewEXCheckParam image frame count %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_5997_112_2_18_1_24_11_434,(uint8*)"d", param_ptr->image_frame_count);
        return SCI_FALSE;

    }
    for(i = 0; i < param_ptr->image_frame_count; i++)
    {
        p_img_frame = (DCAMERA_IMAGE_FRAME_PARAM_T*)&param_ptr->image_frame_info[i];
        if(p_img_frame->target_size.w < DCAMERA_PIXEL_ALIGNED ||
           p_img_frame->target_size.h < DCAMERA_PIXEL_ALIGNED ||
           (p_img_frame->target_size.w % DCAMERA_PIXEL_ALIGNED) ||
           (p_img_frame->target_size.h % DCAMERA_PIXEL_ALIGNED))
        {
            //DCAMERA_TRACE:"DC: _DCAMERA_PreviewEXCheckParam image size errror w,h %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6011_112_2_18_1_24_11_435,(uint8*)"dd", p_img_frame->target_size.w,p_img_frame->target_size.h);
            return SCI_FALSE;

        }

        if(p_img_frame->data_type != DCAMERA_DATA_TYPE_YUV422 &&
           p_img_frame->data_type != DCAMERA_DATA_TYPE_YUV420 &&
           p_img_frame->data_type != DCAMERA_DATA_TYPE_RGB565)
        {
            //DCAMERA_TRACE:"DC: _DCAMERA_PreviewEXCheckParam image data type errror %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6021_112_2_18_1_24_11_436,(uint8*)"d", p_img_frame->data_type);
            return SCI_FALSE;
        }
        if(p_img_frame->data_type == DCAMERA_DATA_TYPE_YUV420 &&
           p_img_frame->target_size.w >= ISP_SCALE_FRAME_MODE_WIDTH_TH)
        {
            //DCAMERA_TRACE:"DC: _DCAMERA_PreviewEXCheckParam slice scaling for YUV420 is unsupported"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6027_112_2_18_1_24_11_437,(uint8*)"");
            return SCI_FALSE;

        }
        //DCAMERA_TRACE:"DC: _DCAMERA_PreviewEXCheckParam image %d,width %d height %d,data type %d Rot %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6036_112_2_18_1_24_11_438,(uint8*)"ddddd",i,p_img_frame->target_size.w,p_img_frame->target_size.h,p_img_frame->data_type,p_img_frame->rotation_mode);

    }

    return SCI_TRUE;
}

LOCAL  uint32 _DCAMERA_GetISPDateType(DCAMERA_DATA_TYPE_E data_type, DCAMERA_DATA_PATTERN_T data_pattern)
{
    uint32    isp_format = ISP_IMAGE_FORMAT_MAX;

    if(DCAMERA_DATA_TYPE_YUV422 == data_type &&
       DCAMERA_YUV_TWO_PLANE == data_pattern.yuv_pattern)
    {
        isp_format = (uint32)ISP_IMAGE_FORMAT_YUV422;
    }
    else if(DCAMERA_DATA_TYPE_YUV420 == data_type)
    {
        if(DCAMERA_YUV_TWO_PLANE == data_pattern.yuv_pattern)
        {
            isp_format = (uint32)ISP_IMAGE_FORMAT_YUV420;
        }
        else
        {
            isp_format = (uint32)ISP_IMAGE_FORMAT_YUV420_3FRAME;
        }
    }
    else if(DCAMERA_DATA_TYPE_RGB565 == data_type)
    {
        isp_format = (uint32)ISP_IMAGE_FORMAT_RGB565_EX;
    }
    else
    {
        isp_format = (uint32)ISP_IMAGE_FORMAT_RAW;
    }

    return isp_format;
}

LOCAL   void _DCAMERA_GetDateType(ISP_IMAGE_FORMAT img_format, uint32 *p_data_type, uint32 *p_data_pattern)
{
    switch(img_format)
    {
        case ISP_IMAGE_FORMAT_YUV422:

            if(p_data_type)
            {
                *p_data_type = DCAMERA_DATA_TYPE_YUV422;
            }
            if(p_data_pattern)
            {
                *p_data_pattern = DCAMERA_YUV_TWO_PLANE;
            }
        break;

        case ISP_IMAGE_FORMAT_YUV420:

            if(p_data_type)
            {
                *p_data_type = DCAMERA_DATA_TYPE_YUV420;
            }
            if(p_data_pattern)
            {
                *p_data_pattern = DCAMERA_YUV_TWO_PLANE;
            }
        break;

        case ISP_IMAGE_FORMAT_YUV420_3FRAME:

            if(p_data_type)
            {
                *p_data_type = DCAMERA_DATA_TYPE_YUV420;
            }
            if(p_data_pattern)
            {
                *p_data_pattern = DCAMERA_YUV_THREE_PLANE;
            }
        break;

        case ISP_IMAGE_FORMAT_RGB565_EX:

            if(p_data_type)
            {
                *p_data_type = DCAMERA_DATA_TYPE_RGB565;
            }
        break;

         case ISP_IMAGE_FORMAT_RAW:

            if(p_data_type)
            {
                *p_data_type = DCAMERA_DATA_TYPE_RAW;
            }
        break;

        default:

        break;
    }
}

PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartPreviewEX(DCAMERA_PREVIEW_EX_PARAM_T const* param_ex_ptr)
{
    DCAMERA_RETURN_VALUE_E    rtn = DCAMERA_OP_SUCCESS;
    uint32                                      status,actual_event;
    DCAMERA_INFO_T                   *dc_info_ptr   = s_dc_info_ptr;


    if(!dc_info_ptr)
    {
        return DCAMERA_OP_NOT_OPEN;
    }

    dc_info_ptr->prev_ex_wait = 0;
    if(dc_info_ptr->is_snapshot_ex)
    {
        dc_info_ptr->prev_ex_wait = 1;
        status = SCI_GetEvent(s_dc_event,
                              DC_SNAPSHOT_EX_EVENT,
                              SCI_AND_CLEAR,
                              &actual_event,
                              DCAMERA_TIMEOUT_INTERVAL);
        dc_info_ptr->prev_ex_wait = 0;
    }

    rtn = _DCAMERA_StartPreviewEX(param_ex_ptr);

    return rtn;

}


LOCAL int _DCAMERA_PreviewDisplayCallback(void* address)
{
    DCAMERA_INFO_T              *dc_info_ptr   = s_dc_info_ptr;
    ISP_FRAME_T                 *p_frame = (ISP_FRAME_T*)address;
    uint32                      frm_size = (uint32)(p_frame->width * p_frame->height);
    DCAMERA_DISPALY_PARAM_T     scr_disp = {0};
    DCAMERA_DISPALY_PARAM_T     dst_disp = {0};
    uint32                      rtn = 0;

    //DCAMERA_TRACE:"_DCAMERA_PreviewDisplayCallback, frame 0x%x, yaddr 0x%x, uaddr 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6180_112_2_18_1_24_11_439,(uint8*)"ddd",address,p_frame->yaddr,p_frame->uaddr);

    if(ISP_ServiceIsFrameLocked((ISP_FRAME_T*)dc_info_ptr->p_dis_src_frm_cur))
    {
        //DCAMERA_TRACE:"_DCAMERA_PreviewDisplayCallback, Source Frame locked! 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6185_112_2_18_1_24_11_440,(uint8*)"d",dc_info_ptr->p_dis_src_frm_cur);
    }
    else
    {
        if(ISP_ServiceIsFrameLocked((ISP_FRAME_T*)dc_info_ptr->p_dis_dst_frm_cur))
        {
            //DCAMERA_TRACE:"_DCAMERA_PreviewDisplayCallback, Destination Frame locked! 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6192_112_2_18_1_24_11_441,(uint8*)"d",dc_info_ptr->p_dis_dst_frm_cur);
        }
        else
        {
            SCI_MEMCPY((void*)dc_info_ptr->p_dis_src_frm_cur->yaddr,
                        (void*)p_frame->yaddr,
                        frm_size);
            SCI_MEMCPY((void*)dc_info_ptr->p_dis_src_frm_cur->uaddr,
                        (void*)p_frame->uaddr,
                        frm_size);

            if(dc_info_ptr->display_cb)
            {
                ISP_ServiceFrameLock(dc_info_ptr->p_dis_src_frm_cur);
                scr_disp.yaddr = dc_info_ptr->p_dis_src_frm_cur->yaddr;
                scr_disp.uaddr = dc_info_ptr->p_dis_src_frm_cur->uaddr;
                scr_disp.width = dc_info_ptr->p_dis_src_frm_cur->width;
                scr_disp.height = dc_info_ptr->p_dis_src_frm_cur->height;
                dc_info_ptr->p_dis_src_frm_cur = dc_info_ptr->p_dis_src_frm_cur->next;

                ISP_ServiceFrameLock(dc_info_ptr->p_dis_dst_frm_cur);
                dst_disp.yaddr = dc_info_ptr->p_dis_dst_frm_cur->yaddr;
                dst_disp.uaddr = dc_info_ptr->p_dis_dst_frm_cur->uaddr;
                dst_disp.width = dc_info_ptr->p_dis_dst_frm_cur->width;
                dst_disp.height = dc_info_ptr->p_dis_dst_frm_cur->height;
                dc_info_ptr->p_dis_dst_frm_cur = dc_info_ptr->p_dis_dst_frm_cur->next;
                scr_disp.type = DCAMERA_DATA_TYPE_YUV422;
                dst_disp.type = DCAMERA_DATA_TYPE_YUV422;
                rtn = (*dc_info_ptr->display_cb)(0, &scr_disp, &dst_disp, dc_info_ptr->p_user_data);
                if(rtn)
                {
                    //DCAMERA_TRACE:"_DCAMERA: drop the current frame"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6223_112_2_18_1_24_11_442,(uint8*)"");
                    dc_info_ptr->p_dis_src_frm_cur = dc_info_ptr->p_dis_src_frm_cur->prev;
                    dc_info_ptr->p_dis_dst_frm_cur = dc_info_ptr->p_dis_dst_frm_cur->prev;
                    ISP_ServiceFrameUnLock(dc_info_ptr->p_dis_src_frm_cur);
                    ISP_ServiceFrameUnLock(dc_info_ptr->p_dis_dst_frm_cur);
                }
            }

        }
    }

    return (int)ISP_CB_PROC_WAIT;
}

LOCAL int _DCAMERA_PreviewDisplayCallbackLowMem(void* address)
{
    DCAMERA_INFO_T              *dc_info_ptr   = s_dc_info_ptr;
    ISP_FRAME_T                 *p_frame = (ISP_FRAME_T*)address;
    DCAMERA_DISPALY_PARAM_T     scr_disp = {0};
    DCAMERA_DISPALY_PARAM_T     dst_disp = {0};
    uint32                                    rtn = 0;
    uint32                                    block_id_src = 0, block_id_dst = 0;
    ISP_ADDRESS_T                     block_addr_src = {0}, block_addr_dst = {0};

    DCAMERA_TRACE("DC:LowMem, frame 0x%x, yaddr 0x%x, uaddr 0x%x", address,p_frame->yaddr,p_frame->uaddr);

    if(dc_info_ptr->display_cb)
    {
        //transfer the memory block (processing) pointer state to display source buffer state
        rtn = ISP_ServiceGetVtMemBlockId(&block_id_src, ISP_VT_MEMBLK_PROCING_SRC);
        if(!rtn)
        {
            block_addr_src = *(ISP_ServiceGetVtMemBlock(block_id_src));
            DCAMERA_TRACE("DC: block %d y_addr 0x%x u_addr 0x%x",
                                                block_id_src,
                                                block_addr_src.yaddr,
                                                block_addr_src.uaddr
                                                );
            dc_info_ptr->p_dis_src_frm_cur->yaddr = block_addr_src.yaddr;
            dc_info_ptr->p_dis_src_frm_cur->uaddr = block_addr_src.uaddr;
        }
        else
        {
            DCAMERA_TRACE("DC:LowMem, error1");
            return (int)ISP_CB_PROC_QUIT;
        }

        rtn = ISP_ServiceGetVtMemBlockId(&block_id_dst, ISP_VT_MEMBLK_PROCING_DST);
        if(!rtn)
        {
            block_addr_dst = *(ISP_ServiceGetVtMemBlock(block_id_dst));
            DCAMERA_TRACE("DC:LowMem,  block %d y_addr 0x%x u_addr 0x%x",
                                    block_id_dst,
                                    block_addr_dst.yaddr,
                                    block_addr_dst.uaddr
                                    );
            dc_info_ptr->p_dis_dst_frm_cur->yaddr = block_addr_dst.yaddr;
            dc_info_ptr->p_dis_dst_frm_cur->uaddr = block_addr_dst.uaddr;
        }
        else
        {
            DCAMERA_TRACE("DC:LowMem, error2");
            return (int)ISP_CB_PROC_QUIT;
        }

        ISP_ServiceFrameLock(dc_info_ptr->p_dis_src_frm_cur);
        scr_disp.yaddr = dc_info_ptr->p_dis_src_frm_cur->yaddr;
        scr_disp.uaddr = dc_info_ptr->p_dis_src_frm_cur->uaddr;
        scr_disp.width = dc_info_ptr->p_dis_src_frm_cur->width;
        scr_disp.height = dc_info_ptr->p_dis_src_frm_cur->height;

        ISP_ServiceFrameLock(dc_info_ptr->p_dis_dst_frm_cur);
        dst_disp.yaddr = dc_info_ptr->p_dis_dst_frm_cur->yaddr;
        dst_disp.uaddr = dc_info_ptr->p_dis_dst_frm_cur->uaddr;
        dst_disp.width = dc_info_ptr->p_dis_dst_frm_cur->width;
        dst_disp.height = dc_info_ptr->p_dis_dst_frm_cur->height;

        scr_disp.type = DCAMERA_DATA_TYPE_YUV422;
        dst_disp.type = DCAMERA_DATA_TYPE_YUV422;
        rtn = (*dc_info_ptr->display_cb)(0, &scr_disp, &dst_disp, dc_info_ptr->p_user_data);
        if(rtn)                                                                     //display not success handle
        {
            //DCAMERA_TRACE:"_DCAMERA: drop the current frame"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6223_112_2_18_1_24_11_442,(uint8*)"");

            //transfer the memory block start(src & dst) state to IDLE
            DCAMERA_TRACE("DC:LowMem, block %d state change from %d to %d", block_id_src, ISP_VT_MEMBLK_PROCING_SRC, ISP_VT_MEMBLK_IDLE);
            DCAMERA_TRACE("DC:LowMem, block %d state change from %d to %d", block_id_dst, ISP_VT_MEMBLK_PROCING_DST, ISP_VT_MEMBLK_IDLE);
            ISP_ServiceSetVtMemBlockState(block_id_src, ISP_VT_MEMBLK_IDLE);
            ISP_ServiceSetVtMemBlockState(block_id_dst, ISP_VT_MEMBLK_IDLE);

            //send buffer free message to isp_service
            ISP_ServiceContinueVTExtLowMem();
        }
    }

    return (int)ISP_CB_PROC_WAIT;
}


LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_StartPreviewEX(DCAMERA_PREVIEW_EX_PARAM_T const* param_ex_ptr)
{
    DCAMERA_INFO_T              *dc_info_ptr   = s_dc_info_ptr;
    ISP_VT_ENCODE_PARAM_T       *p_vt_enc = &dc_info_ptr->vt_enc_param_t;
    DCAMERA_CONVERT_COOR_T      src_coor       = {0};
    DCAMERA_CONVERT_COOR_T      dst_coor       = {0};
    uint32                      i,scale_in_vt_eb = 0;
    ISP_RECT_T                  isp_img_rect = {0};
    ISP_RECT_T                  logic_disp_rect = {0};
    ISP_RECT_T                  logic_lcdc_rect = {0};
    uint32                      mem_start = 0, mem_size = 0;
    uint32                      raw_prev = 0, frm_size = 0;
    uint32                      frm_no = 0;
    DCAMERA_PREVIEW_CTL_INFO_T  *preview_ctl_info_ptr = &s_dc_info_ptr->preview_ctl_info;

    //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX -> start time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6252_112_2_18_1_24_11_443,(uint8*)"d", SCI_GetTickCount());

    if(SCI_NULL == param_ex_ptr)
    {
        return DCAMERA_OP_PARAM_ERR;
    }
    if(!DCAMERA_IsOpen())
    {
        return DCAMERA_OP_NOT_OPEN;
    }

    if(DCAMERA_OP_NORMAL == s_operation_mode||
       DCAMERA_OP_SPECIAL == s_operation_mode||
       DCAMERA_OP_MINIDC == s_operation_mode)
    {
        if(_DCAMERA_OpenISP())
        {
            return DCAMERA_OP_ISP_ERR;
        }
    }

    if(PNULL == s_sensor_ptr)
    {
        return DCAMERA_OP_NOT_OPEN;
    }

    if(DCAMERA_INVALIDATEMODE_NONE != param_ex_ptr->invalidatemode)
    {
        p_vt_enc->local_review_enable = SCI_TRUE;
        if(SCI_FALSE == _DCAMERA_PreviewCheckParam(param_ex_ptr))
        {
            //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX, preview parameter error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6282_112_2_18_1_24_11_444,(uint8*)"");
            return DCAMERA_OP_PARAM_ERR;
        }
    }
    else
    {
        p_vt_enc->local_review_enable = SCI_FALSE;
    }


    scale_in_vt_eb = _DCAMERA_PreviewEXCheckParam(param_ex_ptr);

    if(p_vt_enc->local_review_enable)
    {
        DCAMERA_ROTATION_MODE_E     sensor_rot   = DCAMERA_ROTATION_0;
        DCAMERA_ROTATION_MODE_E     disp_rot       = DCAMERA_ROTATION_0;

        p_vt_enc->local_display_enable = 1;
        p_vt_enc->local_review_enable = 1;
        p_vt_enc->local_disp_lcd_id = param_ex_ptr->lcd_id;
        p_vt_enc->local_disp_block_id = param_ex_ptr->lcd_block_id;
        dc_info_ptr->isp_preview_param.disp_lcd_id = param_ex_ptr->lcd_id;
        dc_info_ptr->isp_preview_param.disp_block_id = param_ex_ptr->lcd_block_id;


        LCD_SetInvalidateLimit(param_ex_ptr->lcd_id, NO_PERMIT_GUI_INVALIDATE);
        SCI_TRACE_LOW("DC:StartPreviewEX NO_PERMIT_GUI_INVALIDATE");

        LCD_DisableBlock(param_ex_ptr->lcd_id, param_ex_ptr->lcd_block_id);
        SCI_TRACE_LOW("DC:StartPreviewEX lcd_id %d block_id %d ", param_ex_ptr->lcd_id, param_ex_ptr->lcd_block_id);

        dc_info_ptr->preview_ctl_info.display_mode = param_ex_ptr->disp_mode;
        dc_info_ptr->display_mode = param_ex_ptr->disp_mode;

        // set preview rotation mode
        if(DCAMERA_OP_ATV == s_operation_mode)
        {
            dc_info_ptr->preview_ctl_info.rotation_mode = param_ex_ptr->rotation_mode;
            dc_info_ptr->sensor_rot = (uint32)DCAMERA_ROTATION_0;
            dc_info_ptr->isp_preview_param.disp_rotation = (ISP_ROTATION_E)param_ex_ptr->rotation_mode;
        }
        else
        {
            disp_rot = _DCAMERA_ROT_Convertor(param_ex_ptr->rotation_mode, &sensor_rot);
            dc_info_ptr->preview_ctl_info.rotation_mode = disp_rot;
            dc_info_ptr->sensor_rot = (uint32)sensor_rot;
            dc_info_ptr->isp_preview_param.disp_rotation = (ISP_ROTATION_E)disp_rot;
        }

        src_coor.rect = param_ex_ptr->disp_rect;
        src_coor.rot_degree = dc_info_ptr->preview_ctl_info.rotation_mode;
        DCAMERA_Convert_Coor(param_ex_ptr->lcd_id, &src_coor, &dst_coor);
        dc_info_ptr->disp_rect = dst_coor.rect;

        src_coor.rect = param_ex_ptr->target_rect;
        src_coor.rot_degree = dc_info_ptr->preview_ctl_info.rotation_mode;
        DCAMERA_Convert_Coor(param_ex_ptr->lcd_id, &src_coor, &dst_coor);
        dc_info_ptr->target_rect = dst_coor.rect;
        _DCAMERA_HandleSensorOutputSize(dc_info_ptr->target_rect);
        p_vt_enc->local_input_size.w = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].width;
        p_vt_enc->local_input_size.h = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].height;
        p_vt_enc->local_input_range.x = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].trim_start_x;
        p_vt_enc->local_input_range.y = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].trim_start_y;
        p_vt_enc->local_input_range.w = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].trim_width;
        p_vt_enc->local_input_range.h = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].trim_height;
        p_vt_enc->local_disp_range.x = (uint32)dc_info_ptr->target_rect.x;
        p_vt_enc->local_disp_range.y = (uint32)dc_info_ptr->target_rect.y;
        p_vt_enc->local_disp_range.w = (uint32)dc_info_ptr->target_rect.w;
        p_vt_enc->local_disp_range.h = (uint32)dc_info_ptr->target_rect.h;
        p_vt_enc->lcd_rect.x = (uint32)dc_info_ptr->disp_rect.x;
        p_vt_enc->lcd_rect.y = (uint32)dc_info_ptr->disp_rect.y;
        p_vt_enc->lcd_rect.w = (uint32)dc_info_ptr->disp_rect.w;
        p_vt_enc->lcd_rect.h = (uint32)dc_info_ptr->disp_rect.h;
        _DCamera_AdjustRect(&p_vt_enc->local_input_range,
                            &p_vt_enc->lcd_rect,
                            &p_vt_enc->local_disp_range,
                            param_ex_ptr->disp_mode);

        logic_lcdc_rect = p_vt_enc->lcd_rect;
        logic_disp_rect = p_vt_enc->local_disp_range;

        _DCAMERA_ConvertCoorToISPRect(param_ex_ptr->lcd_id,
                                      &logic_lcdc_rect,
                                      &p_vt_enc->lcd_rect,
                                      (DCAMERA_ROTATION_MODE_E)dc_info_ptr->preview_ctl_info.rotation_mode);

        _DCAMERA_ConvertCoorToISPRect(param_ex_ptr->lcd_id,
                                      &logic_disp_rect,
                                      &p_vt_enc->local_disp_range,
                                      (DCAMERA_ROTATION_MODE_E)dc_info_ptr->preview_ctl_info.rotation_mode);

        p_vt_enc->disp_rotation = (ISP_ROTATION_E)dc_info_ptr->preview_ctl_info.rotation_mode;
        isp_img_rect = p_vt_enc->local_disp_range;

        if(param_ex_ptr->rotation_mode == DCAMERA_ROTATION_90 ||
           param_ex_ptr->rotation_mode == DCAMERA_ROTATION_270)
        {
            p_vt_enc->encode_size.w = (uint32)p_vt_enc->local_disp_range.h;
            p_vt_enc->encode_size.h = (uint32)p_vt_enc->local_disp_range.w;
        }
        else
        {
            p_vt_enc->encode_size.w = (uint32)p_vt_enc->local_disp_range.w;
            p_vt_enc->encode_size.h = (uint32)p_vt_enc->local_disp_range.h;
        }

        //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX, preview parameter, preview size %d %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6370_112_2_18_1_24_11_445,(uint8*)"dd",p_vt_enc->encode_size.w,p_vt_enc->encode_size.h);

        if(scale_in_vt_eb)
        {
            s_dc_info_ptr->target_frame_count = param_ex_ptr->image_frame_count;
            s_dc_info_ptr->preview_buf_len = 0;
            for(i = 0; i < param_ex_ptr->image_frame_count; i++)
            {
                dc_info_ptr->target_frame_info_t[i].target_size.w = param_ex_ptr->image_frame_info[i].target_size.w;
                dc_info_ptr->target_frame_info_t[i].target_size.h = param_ex_ptr->image_frame_info[i].target_size.h;

                dc_info_ptr->target_frame_format[i] = _DCAMERA_GetISPDateType(param_ex_ptr->image_frame_info[i].data_type,
                                                                              param_ex_ptr->image_frame_info[i].data_pattern);
                dc_info_ptr->target_frame_info_t[i].rotation_mode = param_ex_ptr->image_frame_info[i].rotation_mode;
                if(dc_info_ptr->target_frame_info_t[i].rotation_mode == DCAMERA_ROTATION_90 ||
                   dc_info_ptr->target_frame_info_t[i].rotation_mode == DCAMERA_ROTATION_270)
                {
                    p_vt_enc->encode_size.w =
                        DCAMERA_MAX(p_vt_enc->encode_size.w, dc_info_ptr->target_frame_info_t[i].target_size.h);
                    p_vt_enc->encode_size.h =
                        DCAMERA_MAX(p_vt_enc->encode_size.h, dc_info_ptr->target_frame_info_t[i].target_size.w);

                }
                else
                {
                    p_vt_enc->encode_size.w =
                        DCAMERA_MAX(p_vt_enc->encode_size.w, dc_info_ptr->target_frame_info_t[i].target_size.w);
                    p_vt_enc->encode_size.h =
                        DCAMERA_MAX(p_vt_enc->encode_size.h, dc_info_ptr->target_frame_info_t[i].target_size.h);
                }
                //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX, preview parameter, encode_size %d %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6401_112_2_18_1_24_11_446,(uint8*)"dd",p_vt_enc->encode_size.w,p_vt_enc->encode_size.h);
                s_dc_info_ptr->preview_buf_len += (dc_info_ptr->target_frame_info_t[i].target_size.w* dc_info_ptr->target_frame_info_t[i].target_size.h * 2) + DCAMERA_PADDING_MEM_SIZE;
            }
            // add rotation buffer
            s_dc_info_ptr->preview_buf_len += (p_vt_enc->encode_size.w * p_vt_enc->encode_size.h * 2) + DCAMERA_PADDING_MEM_SIZE;

        }
        else
        {
            if(DCAMERA_OP_ATV == s_operation_mode &&
               (logic_disp_rect.w > p_vt_enc->local_input_range.w ||
                logic_disp_rect.h > p_vt_enc->local_input_range.h))
            {
                p_vt_enc->encode_size.w = p_vt_enc->local_input_range.w;
                p_vt_enc->encode_size.h = p_vt_enc->local_input_range.h;
            }
            else
            {
                p_vt_enc->encode_size.w = 0;
                p_vt_enc->encode_size.h = 0;
            }
        }
    }
    else // no display in isp_service, just sample image from sensor
    {
        dc_info_ptr->isp_preview_param.disp_lcd_id = DCAMERA_PARAM_INVALID_VALUE;
        dc_info_ptr->isp_preview_param.disp_block_id = DCAMERA_PARAM_INVALID_VALUE;

        if(scale_in_vt_eb)
        {
            s_dc_info_ptr->target_frame_count = param_ex_ptr->image_frame_count;
            p_vt_enc->encode_size.w = 0;
            p_vt_enc->encode_size.h = 0;
            s_dc_info_ptr->preview_buf_len = 0;
            for(i = 0; i < param_ex_ptr->image_frame_count; i++)
            {
                dc_info_ptr->target_frame_info_t[i].target_size.w = param_ex_ptr->image_frame_info[i].target_size.w;
                dc_info_ptr->target_frame_info_t[i].target_size.h = param_ex_ptr->image_frame_info[i].target_size.h;
                dc_info_ptr->target_frame_format[i] = _DCAMERA_GetISPDateType(param_ex_ptr->image_frame_info[i].data_type,
                                                                              param_ex_ptr->image_frame_info[i].data_pattern);
                dc_info_ptr->target_frame_info_t[i].rotation_mode = param_ex_ptr->image_frame_info[i].rotation_mode;
                if(dc_info_ptr->target_frame_info_t[i].rotation_mode == DCAMERA_ROTATION_90 ||
                   dc_info_ptr->target_frame_info_t[i].rotation_mode == DCAMERA_ROTATION_270)
                {
                    p_vt_enc->encode_size.w =
                        DCAMERA_MAX(p_vt_enc->encode_size.w, dc_info_ptr->target_frame_info_t[i].target_size.h);
                    p_vt_enc->encode_size.h =
                        DCAMERA_MAX(p_vt_enc->encode_size.h, dc_info_ptr->target_frame_info_t[i].target_size.w);

                }
                else
                {
                    p_vt_enc->encode_size.w =
                        DCAMERA_MAX(p_vt_enc->encode_size.w, dc_info_ptr->target_frame_info_t[i].target_size.w);
                    p_vt_enc->encode_size.h =
                        DCAMERA_MAX(p_vt_enc->encode_size.h, dc_info_ptr->target_frame_info_t[i].target_size.h);
                }
                s_dc_info_ptr->preview_buf_len += (dc_info_ptr->target_frame_info_t[i].target_size.w* dc_info_ptr->target_frame_info_t[i].target_size.h * 2) + DCAMERA_PADDING_MEM_SIZE;
            }

            // add rotation buffer
            s_dc_info_ptr->preview_buf_len += (p_vt_enc->encode_size.w * p_vt_enc->encode_size.h * 2) + DCAMERA_PADDING_MEM_SIZE;
            dc_info_ptr->target_rect.x = 0;
            dc_info_ptr->target_rect.y = 0;
            dc_info_ptr->target_rect.w = (uint32)p_vt_enc->encode_size.w;
            dc_info_ptr->target_rect.h = (uint32)p_vt_enc->encode_size.h;

            dc_info_ptr->disp_rect = dc_info_ptr->target_rect;
            _DCAMERA_HandleSensorOutputSize(dc_info_ptr->target_rect);
            p_vt_enc->local_input_size.w = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].width;
            p_vt_enc->local_input_size.h = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].height;
            p_vt_enc->local_input_range.x = 0;
            p_vt_enc->local_input_range.y = 0;
            p_vt_enc->local_input_range.w = p_vt_enc->local_input_size.w;
            p_vt_enc->local_input_range.h = p_vt_enc->local_input_size.h;
            p_vt_enc->local_disp_range.x = (uint32)dc_info_ptr->disp_rect.x;
            p_vt_enc->local_disp_range.y = (uint32)dc_info_ptr->disp_rect.y;
            p_vt_enc->local_disp_range.w = (uint32)dc_info_ptr->disp_rect.w;
            p_vt_enc->local_disp_range.h = (uint32)dc_info_ptr->disp_rect.h;
            isp_img_rect = p_vt_enc->local_disp_range;
            _DCamera_AdjustRect(&p_vt_enc->local_input_range,
                                &p_vt_enc->local_disp_range,
                                &isp_img_rect,
                                DCAMERA_DISP_FULLSCREEN);
            //no display in isp_service, here dc_info_ptr->disp_rect equals dc_info_ptr->target_rect,
            //which means dc_info_ptr->disp_rect will not be adjusted and p_vt_enc->local_input_range will be adjusted
            //if the ratio of width and height of local_input_range is not so same as that of disp_rect;


            p_vt_enc->local_review_enable = 0;
            p_vt_enc->local_display_enable = 0;
            dc_info_ptr->preview_ctl_info.display_mode = DCAMERA_DISP_FULLSCREEN;
            dc_info_ptr->display_mode = DCAMERA_DISP_FULLSCREEN;
        }
        else
        {
            //no scaling , means raw data

            dc_info_ptr->target_rect.x = 0;
            dc_info_ptr->target_rect.y = 0;
            for(i = 0; i < param_ex_ptr->image_frame_count; i++)
            {
                if(param_ex_ptr->image_frame_info[i].data_type == DCAMERA_DATA_TYPE_RAW)
                {
                    dc_info_ptr->target_rect.w = param_ex_ptr->image_frame_info[i].target_size.w;
                    dc_info_ptr->target_rect.h = param_ex_ptr->image_frame_info[i].target_size.h;

                    dc_info_ptr->target_frame_format[0] = ISP_IMAGE_FORMAT_RAW;

                    raw_prev = 1;
                    break;
                }
            }

            if(0 == raw_prev)
            {
                //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX, preview parameter error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6507_112_2_18_1_24_12_447,(uint8*)"");
                return DCAMERA_OP_PARAM_ERR;
            }

            _DCAMERA_HandleSensorOutputSize(dc_info_ptr->target_rect);
            dc_info_ptr->target_rect.w = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].width;
            dc_info_ptr->target_rect.h = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].height;

            p_vt_enc->encode_size.w = dc_info_ptr->target_rect.w;
            p_vt_enc->encode_size.h = dc_info_ptr->target_rect.h;
            s_dc_info_ptr->target_frame_count = 1;
            dc_info_ptr->target_frame_info_t[0].target_size.w = p_vt_enc->encode_size.w;
            dc_info_ptr->target_frame_info_t[0].target_size.h = p_vt_enc->encode_size.h;

            p_vt_enc->local_input_size.w = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].width;
            p_vt_enc->local_input_size.h = s_sensor_ptr->sensor_mode_info[dc_info_ptr->preview_m].height;
            p_vt_enc->local_input_range.x = 0;
            p_vt_enc->local_input_range.y = 0;
            p_vt_enc->local_input_range.w = p_vt_enc->local_input_size.w;
            p_vt_enc->local_input_range.h = p_vt_enc->local_input_size.h;
            p_vt_enc->local_disp_range.x = (uint32)0;
            p_vt_enc->local_disp_range.y = (uint32)0;
            p_vt_enc->local_disp_range.w = (uint32)p_vt_enc->local_input_size.w;
            p_vt_enc->local_disp_range.h = (uint32)p_vt_enc->local_input_size.h;

            p_vt_enc->local_review_enable = 0;
            p_vt_enc->local_display_enable = 0;
            dc_info_ptr->preview_ctl_info.display_mode = DCAMERA_DISP_FULLSCREEN;
            dc_info_ptr->display_mode = DCAMERA_DISP_FULLSCREEN;
            s_dc_info_ptr->preview_buf_len = (p_vt_enc->encode_size.w * p_vt_enc->encode_size.h * 2) + DCAMERA_PADDING_MEM_SIZE;

        }
    }

    if(raw_prev)
    {
        Sensor_SetMode(s_dc_info_ptr->preview_m); //raw data, no effect setting
    }
    else
    {
        _DCAMERA_PreviewSensorSet();
    }
    DCAMERA_SetCapParam(&p_vt_enc->cap_param);

    dc_info_ptr->isp_preview_param.input_size.w = p_vt_enc->local_input_size.w;
    dc_info_ptr->isp_preview_param.input_size.h = p_vt_enc->local_input_size.h;
    dc_info_ptr->isp_preview_param.input_range.x = p_vt_enc->local_input_range.x;
    dc_info_ptr->isp_preview_param.input_range.y = p_vt_enc->local_input_range.y;
    dc_info_ptr->isp_preview_param.input_range.w = p_vt_enc->local_input_range.w;
    dc_info_ptr->isp_preview_param.input_range.h = p_vt_enc->local_input_range.h;
    dc_info_ptr->isp_preview_param.disp_range.x = isp_img_rect.x;
    dc_info_ptr->isp_preview_param.disp_range.y = isp_img_rect.y;
    dc_info_ptr->isp_preview_param.disp_range.w = isp_img_rect.w;
    dc_info_ptr->isp_preview_param.disp_range.h = isp_img_rect.h;
    dc_info_ptr->isp_preview_param.lcd_rect.x = p_vt_enc->lcd_rect.x;
    dc_info_ptr->isp_preview_param.lcd_rect.y = p_vt_enc->lcd_rect.y;
    dc_info_ptr->isp_preview_param.lcd_rect.w = p_vt_enc->lcd_rect.w;
    dc_info_ptr->isp_preview_param.lcd_rect.h = p_vt_enc->lcd_rect.h;
    dc_info_ptr->zoom_w_step = ZOOM_STEP(p_vt_enc->local_input_range.w);
    dc_info_ptr->zoom_h_step = ZOOM_STEP(p_vt_enc->local_input_range.h);
    dc_info_ptr->image_notice_cb = param_ex_ptr->image_notice_cb;
    dc_info_ptr->p_user_data = param_ex_ptr->app_data_ptr;
    dc_info_ptr->display_cb = param_ex_ptr->disp_cb;

    if(s_dc_info_ptr->preview_buf_len != 0)
    {
        s_dc_info_ptr->p_preview_buf = SCI_ALLOCA(s_dc_info_ptr->preview_buf_len);
        if(SCI_NULL == s_dc_info_ptr->p_preview_buf)
        {
            //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX, no memory!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6576_112_2_18_1_24_12_448,(uint8*)"");
            return DCAMERA_OP_NO_ENOUGH_MEMORY;
        }
        else
        {
            //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX, p_preview_buf 0x%x, preview_buf_len %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6583_112_2_18_1_24_12_449,(uint8*)"dd",s_dc_info_ptr->p_preview_buf,s_dc_info_ptr->preview_buf_len);
        }
    }
    s_dcamera_info.p_prev_frm_cur = &s_dcamera_info.isp_preview_frame[0];
    ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, _DCAMERA_ImageRecivedCallback);

    if(DCAMERA_OP_ATV == s_operation_mode)
    {
        ISP_ServiceRegister(ISP_SERVICE_CALLBACK_POSTCAP, _DCAMERA_CtrlAtvCallback);
    }

    if(DCAMERA_OP_SPECIAL == s_operation_mode)
    {
        mem_start = (uint32)DCAMERA_PreviewExGetBuf(&mem_size);

        DCAMERA_TRACE("DC:mem_start 0x%x, mem_size %d", mem_start, mem_size);

        if(dc_info_ptr->display_cb)
        {
            uint32 frame_buff_thres_norm = DC_DISPLAY_FRAME_BUF_MAX*2;
            uint32 frame_buff_thres_low = DC_DISPLAY_ROUND_BUF_MIN+1;
            // need initilize the display source and destination buffer
            frm_size = p_vt_enc->local_disp_range.w * p_vt_enc->local_disp_range.h;

            if(p_vt_enc->local_review_enable)
            {
                frame_buff_thres_norm += 1;     //add review buffer
            }

            if(ISP_ROTATION_90 == p_vt_enc->disp_rotation||ISP_ROTATION_270 == p_vt_enc->disp_rotation)
            {
                frame_buff_thres_norm += 1;    //add rotation at least
            }

            if(mem_size < ((frm_size << 1) * frame_buff_thres_norm))
            {
                DCAMERA_TRACE("DC: No enough memory,Total 0x%x", mem_size);

                if(mem_size < ((frm_size << 1) * frame_buff_thres_low))
                {
                    //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX, No memory even for low memory case! Total 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6599_112_2_18_1_24_12_450,(uint8*)"d",mem_size);
                    return DCAMERA_OP_NO_ENOUGH_MEMORY;
                }
                else
                {
                    p_vt_enc->mem_mode = ISP_VT_MEM_MODE_LOW;
                    DCAMERA_TRACE("DC:StartPreviewEX, try low memory");
                }
            }
            else
            {
                p_vt_enc->mem_mode = ISP_VT_MEM_MODE_NORM;
            }
//            p_vt_enc->mem_mode = ISP_VT_MEM_MODE_LOW;           //now go low memory case at all

            if(p_vt_enc->mem_mode == ISP_VT_MEM_MODE_NORM)
            {
                for(i = 0; i < DC_DISPLAY_FRAME_BUF_MAX; i++)
                {
                    SCI_MEMSET((void*)&dc_info_ptr->display_src_frame[i], 0, sizeof(ISP_FRAME_T));
                    if(ISP_ROTATION_90 == p_vt_enc->disp_rotation||ISP_ROTATION_270 == p_vt_enc->disp_rotation)
                    {
                        //display need to be rotation, so the size before rotation should be adjust for corresponding
                        dc_info_ptr->display_src_frame[i].width = p_vt_enc->local_disp_range.h;
                        dc_info_ptr->display_src_frame[i].height = p_vt_enc->local_disp_range.w;
                    }
                    else
                    {
                        dc_info_ptr->display_src_frame[i].width = p_vt_enc->local_disp_range.w;
                        dc_info_ptr->display_src_frame[i].height = p_vt_enc->local_disp_range.h;
                    }
                    dc_info_ptr->display_src_frame[i].yaddr = mem_start;
                    dc_info_ptr->display_src_frame[i].uaddr = dc_info_ptr->display_src_frame[i].yaddr + frm_size;
                    mem_start += (frm_size << 1);
                    mem_size -= (frm_size << 1);
                    frm_no = (uint32)((i + 1) % DC_DISPLAY_FRAME_BUF_MAX); //next frame
                    dc_info_ptr->display_src_frame[i].next = &dc_info_ptr->display_src_frame[frm_no];
                    frm_no = (uint32)((i - 1 + DC_DISPLAY_FRAME_BUF_MAX) % DC_DISPLAY_FRAME_BUF_MAX); //previous frame
                    dc_info_ptr->display_src_frame[i].prev = &dc_info_ptr->display_src_frame[frm_no];
                    //DCAMERA_TRACE:"DCAMERA_StartPreviewEX, disp src frame %d,0x%x, {0x%x 0x%x} ,pointer 0x%x 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6622_112_2_18_1_24_12_451,(uint8*)"dddddd",i,&dc_info_ptr->display_src_frame[i],dc_info_ptr->display_src_frame[i].yaddr,dc_info_ptr->display_src_frame[i].uaddr,dc_info_ptr->display_src_frame[i].prev,dc_info_ptr->display_src_frame[i].next);
                }
                dc_info_ptr->p_dis_src_frm_cur = &dc_info_ptr->display_src_frame[0];
                dc_info_ptr->p_dis_src_frm_free = dc_info_ptr->p_dis_src_frm_cur;

                for(i = 0; i < DC_DISPLAY_FRAME_BUF_MAX; i++)
                {
                    SCI_MEMSET((void*)&dc_info_ptr->display_dst_frame[i], 0, sizeof(ISP_FRAME_T));
                    if(ISP_ROTATION_90 == p_vt_enc->disp_rotation||ISP_ROTATION_270 == p_vt_enc->disp_rotation)
                    {
                        //display need to be rotation, so the size before rotation should be adjust for corresponding
                        dc_info_ptr->display_dst_frame[i].width = p_vt_enc->local_disp_range.h;
                        dc_info_ptr->display_dst_frame[i].height = p_vt_enc->local_disp_range.w;
                    }
                    else
                    {
                        dc_info_ptr->display_dst_frame[i].width = p_vt_enc->local_disp_range.w;
                        dc_info_ptr->display_dst_frame[i].height = p_vt_enc->local_disp_range.h;
                    }
                    dc_info_ptr->display_dst_frame[i].yaddr = mem_start;
                    dc_info_ptr->display_dst_frame[i].uaddr = dc_info_ptr->display_dst_frame[i].yaddr + frm_size;
                    mem_start += (frm_size << 1);
                    mem_size -= (frm_size << 1);
                    frm_no = (uint32)((i + 1) % DC_DISPLAY_FRAME_BUF_MAX); //next frame
                    dc_info_ptr->display_dst_frame[i].next = &dc_info_ptr->display_dst_frame[frm_no];
                    frm_no = (uint32)((i - 1 + DC_DISPLAY_FRAME_BUF_MAX) % DC_DISPLAY_FRAME_BUF_MAX); //previous frame
                    dc_info_ptr->display_dst_frame[i].prev = &dc_info_ptr->display_dst_frame[frm_no];
                    //DCAMERA_TRACE:"DCAMERA_StartPreviewEX, disp dst frame %d,0x%x, {0x%x 0x%x} ,pointer 0x%x 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6646_112_2_18_1_24_12_452,(uint8*)"dddddd",i,&dc_info_ptr->display_dst_frame[i],dc_info_ptr->display_dst_frame[i].yaddr,dc_info_ptr->display_dst_frame[i].uaddr,dc_info_ptr->display_dst_frame[i].prev,dc_info_ptr->display_dst_frame[i].next);

                }
                dc_info_ptr->p_dis_dst_frm_cur = &dc_info_ptr->display_dst_frame[0];
                dc_info_ptr->p_dis_dst_frm_free = dc_info_ptr->p_dis_dst_frm_cur;
                ISP_ServiceRegister(ISP_SERVICE_CALLBACK_DISPLAY, _DCAMERA_PreviewDisplayCallback);
            }
            else
            {
                SCI_MEMSET((void*)&dc_info_ptr->display_src_frame[0], 0, sizeof(ISP_FRAME_T));
                dc_info_ptr->p_dis_src_frm_cur = &dc_info_ptr->display_src_frame[0];
                SCI_MEMSET((void*)&dc_info_ptr->display_dst_frame[0], 0, sizeof(ISP_FRAME_T));
                dc_info_ptr->p_dis_dst_frm_cur = &dc_info_ptr->display_dst_frame[0];

                if(ISP_ROTATION_90 == p_vt_enc->disp_rotation||ISP_ROTATION_270 == p_vt_enc->disp_rotation)
                {
                    dc_info_ptr->p_dis_src_frm_cur->width = p_vt_enc->local_disp_range.h;
                    dc_info_ptr->p_dis_src_frm_cur->height = p_vt_enc->local_disp_range.w;
                    dc_info_ptr->p_dis_dst_frm_cur->width = p_vt_enc->local_disp_range.h;
                    dc_info_ptr->p_dis_dst_frm_cur->height = p_vt_enc->local_disp_range.w;
                }
                else
                {
                    dc_info_ptr->p_dis_src_frm_cur->width = p_vt_enc->local_disp_range.w;
                    dc_info_ptr->p_dis_src_frm_cur->height = p_vt_enc->local_disp_range.h;
                    dc_info_ptr->p_dis_dst_frm_cur->width = p_vt_enc->local_disp_range.w;
                    dc_info_ptr->p_dis_dst_frm_cur->height = p_vt_enc->local_disp_range.h;
                }

                ISP_ServiceRegister(ISP_SERVICE_CALLBACK_DISPLAY, _DCAMERA_PreviewDisplayCallbackLowMem);
            }

            dc_info_ptr->b_is_first_frame = 1;
        }

        ISP_ServiceMemConfig((void*)mem_start, mem_size);
    }

    if(SCI_SUCCESS == ISP_ServiceSetVTExtEncodeParam(p_vt_enc))
    {
        if(SCI_FALSE == dc_info_ptr->atv_is_autoscan)
        {
            _DCAMERA_GetMutex(s_callback_mutex_ptr);
            if(ISP_VT_MEM_MODE_LOW != p_vt_enc->mem_mode)
            {
                ISP_ServiceStartVTExt();
            }
            else
            {
                ISP_ServiceStartVTExtLowMem();
            }
            _DCAMERA_PutMutex(s_callback_mutex_ptr);
            if(ISP_ServiceGetLastErr())
            {
                return DCAMERA_OP_ISP_ERR;
            }
        }

        _DCAMERA_SetPreviewExFlag(SCI_TRUE);
        _DCAMERA_SetIsStopPreview(SCI_FALSE);
        dc_info_ptr->b_preview_from = 1;
        if(DCAMERA_OP_SPECIAL == s_operation_mode)
        {
            if(DCAMERA_FLASH_ON == preview_ctl_info_ptr->flash_mode)
            {
                Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_TRUE);
            }
            else if(DCAMERA_FLASH_OFF == preview_ctl_info_ptr->flash_mode ||
                    DCAMERA_FLASH_AUTO == preview_ctl_info_ptr->flash_mode)

            {
                Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_FALSE);
            }
        }
        return DCAMERA_OP_SUCCESS;
    }
    else
    {
        //DCAMERA_TRACE:"DC: DCAMERA_StartPreviewEX, ISP Error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6677_112_2_18_1_24_12_453,(uint8*)"");
        return DCAMERA_OP_ISP_ERR;
    }
}

LOCAL int _DCAMERA_GetScaledImageData( ISP_ADDRESS_T addr, uint32 width, uint32 height)
{

    //DCAMERA_TRACE:"DC: _DCAMERA_GetScaledImageData width height %d %d, addr 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6686_112_2_18_1_24_12_454,(uint8*)"dddd", width, height, addr.yaddr, addr.uaddr);

    if(s_dc_info_ptr)
    {
        s_dc_info_ptr->cur_addr.chn1_addr = addr.yaddr;
        s_dc_info_ptr->cur_addr.chn2_addr = addr.uaddr;
        s_dc_info_ptr->cur_addr.chn3_addr = addr.vaddr;
    }

    return 0;
}

LOCAL DCAMERA_RETURN_VALUE_E  _DCAMERA_GetYUVRawData(ISP_FRAME_T  *p_src_img,
                                                     uint32       yuv_pattern,
                                                     uint8*       dst_buf)
{
    DCAMERA_RETURN_VALUE_E  rtn = DCAMERA_OP_SUCCESS;
    uint32                  i = 0, j = 0;
    uint8*                  p_src_y = NULL;
    uint8*                  p_src_uv =  NULL;

    if(!p_src_img || !dst_buf)
        return DCAMERA_OP_PARAM_ERR;

    p_src_y =  (uint8*)p_src_img->yaddr;
    p_src_uv =  (uint8*)p_src_img->uaddr;

    switch(yuv_pattern)
    {
        case SENSOR_IMAGE_PATTERN_YUV422_YUYV:

            for(i = 0; i < p_src_img->height; i++)
            {
                for(j = 0; j < (p_src_img->width >> 1); j++)
                {
                    *dst_buf++ = *p_src_y++;
                    *dst_buf++ = *p_src_uv++;
                    *dst_buf++ = *p_src_y++;
                    *dst_buf++ = *p_src_uv++;
                }
            }
            break;

        case SENSOR_IMAGE_PATTERN_YUV422_YVYU:

            for(i = 0; i < p_src_img->height; i++)
            {
                for(j = 0; j < (p_src_img->width >> 1); j++)
                {
                    *dst_buf++ = *p_src_y++;
                    *dst_buf++ = *(p_src_uv+1);
                    *dst_buf++ = *p_src_y++;
                    *dst_buf++ = *p_src_uv;
                    p_src_uv += 2;
                }
            }
            break;

        case SENSOR_IMAGE_PATTERN_YUV422_UYVY:

            for(i = 0; i < p_src_img->height; i++)
            {
                for(j = 0; j < (p_src_img->width >> 1); j++)
                {
                    *dst_buf++ = *p_src_uv++;
                    *dst_buf++ = *p_src_y++;
                    *dst_buf++ = *p_src_uv++;
                    *dst_buf++ = *p_src_y++;
                }
            }
            break;

        case SENSOR_IMAGE_PATTERN_YUV422_VYUY:

            for(i = 0; i < p_src_img->height; i++)
            {
                for(j = 0; j < (p_src_img->width >> 1); j++)
                {
                    *dst_buf++ = *(p_src_uv+1);
                    *dst_buf++ = *p_src_y++;
                    *dst_buf++ = *p_src_uv;
                    *dst_buf++ = *p_src_y++;
                    p_src_uv += 2;
                }
            }
            break;

        default:
            rtn = DCAMERA_OP_PARAM_ERR;
            break;
    }

    return rtn;
}

LOCAL DCAMERA_RETURN_VALUE_E  _DCAMERA_DoImageScaleInVT(DCAMERA_IMAGE_PARAM_T *p_src_img,
                                                        uint32                frame_index,
                                                        DCAMERA_IMAGE_PARAM_T *p_dst_img)
{
    DCAMERA_RETURN_VALUE_E  rtn = DCAMERA_OP_SUCCESS;
    uint32                  isp_status = 0;
    ISP_SCLAE_PARAM_T       vt_scale_param = {0};

    if(!_DCAMERA_IsPreviewEx())
        return DCAMERA_OP_ISP_ERR;

    vt_scale_param.input_format = _DCAMERA_GetISPDateType(p_src_img->frame_info_t[frame_index].data_type,
                                                          p_src_img->frame_info_t[frame_index].data_pattern);

    vt_scale_param.output_size.w = p_dst_img->frame_info_t[frame_index].target_size.w;
    vt_scale_param.output_size.h = p_dst_img->frame_info_t[frame_index].target_size.h;
    vt_scale_param.input_size.w = p_src_img->frame_info_t[frame_index].target_size.w;
    vt_scale_param.input_size.h = p_src_img->frame_info_t[frame_index].target_size.h;
#if 0
    vt_scale_param.input_rect.x = 0;
    vt_scale_param.input_rect.y = 0;
    vt_scale_param.input_rect.w = vt_scale_param.input_size.w ;
    vt_scale_param.input_rect.h = vt_scale_param.input_size.h;
#else
    if((uint32)(vt_scale_param.input_size.w * vt_scale_param.output_size.h) >
       (uint32)(vt_scale_param.input_size.h * vt_scale_param.output_size.w))
    {
        vt_scale_param.input_rect.w = (uint32)((vt_scale_param.input_size.h * vt_scale_param.output_size.w)/vt_scale_param.output_size.h);
        vt_scale_param.input_rect.w = DCAMERA_WIDTH(vt_scale_param.input_rect.w);
        vt_scale_param.input_rect.h = vt_scale_param.input_size.h;
        vt_scale_param.input_rect.x = (vt_scale_param.input_size.w - vt_scale_param.input_rect.w)>>1;
        vt_scale_param.input_rect.x = DCAMERA_WIDTH(vt_scale_param.input_rect.x);
        vt_scale_param.input_rect.y = 0;

    }
    else
    {
        vt_scale_param.input_rect.w = vt_scale_param.input_size.w;
        vt_scale_param.input_rect.h = (uint32)((vt_scale_param.input_size.w*vt_scale_param.output_size.h)/vt_scale_param.output_size.w);
        vt_scale_param.input_rect.h = DCAMERA_HEIGHT(vt_scale_param.input_rect.h);
        vt_scale_param.input_rect.y = (vt_scale_param.input_size.h - vt_scale_param.input_rect.h)>>1;
        vt_scale_param.input_rect.y = DCAMERA_HEIGHT(vt_scale_param.input_rect.y);
        vt_scale_param.input_rect.x = 0;

    }
#endif
    vt_scale_param.output_format = _DCAMERA_GetISPDateType(p_dst_img->frame_info_t[frame_index].data_type,
                                                           p_dst_img->frame_info_t[frame_index].data_pattern);
    vt_scale_param.input_addr.yaddr = p_src_img->dcamera_addr[frame_index].chn1_addr;
    vt_scale_param.input_addr.uaddr = p_src_img->dcamera_addr[frame_index].chn2_addr;
    vt_scale_param.input_addr.vaddr = p_src_img->dcamera_addr[frame_index].chn3_addr;
    vt_scale_param.output_addr.yaddr = p_dst_img->dcamera_addr[frame_index].chn1_addr;
    vt_scale_param.output_addr.uaddr = p_dst_img->dcamera_addr[frame_index].chn2_addr;
    vt_scale_param.output_addr.vaddr = p_dst_img->dcamera_addr[frame_index].chn3_addr;
    vt_scale_param.get_scale_data = _DCAMERA_GetScaledImageData;

    isp_status = ISP_ServiceSetVTExtScaleParam(&vt_scale_param);
    if(0 == isp_status)
    {
        ISP_ServiceStartVTExtScale();
        p_dst_img->dcamera_addr[frame_index].chn1_addr = s_dc_info_ptr->cur_addr.chn1_addr;
        p_dst_img->dcamera_addr[frame_index].chn2_addr = s_dc_info_ptr->cur_addr.chn2_addr;
        p_dst_img->dcamera_addr[frame_index].chn3_addr = s_dc_info_ptr->cur_addr.chn3_addr;
    }
    else
    {
        //DCAMERA_TRACE:"_DCAMERA_DoImageScaleInVT, Scaling parameter ERROR!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6844_112_2_18_1_24_12_455,(uint8*)"");
        rtn =  DCAMERA_OP_ISP_ERR;
    }

    return rtn;
}

/******************************************************************************/
// Description: Stop preview
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopPreview(void)
{
    //DCAMERA_TRACE:"DC: DCAMERA_StopPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6860_112_2_18_1_24_12_456,(uint8*)"");

    if(!DCAMERA_IsOpen())
    {
        return DCAMERA_OP_NOT_OPEN;
    }

    if(!_DCAMERA_GetIfStopPreview())
    {

        if(_DCAMERA_IsPreview())
        {
            ISP_ServiceStopPreview();
            _DCAMERA_SetPreviewFlag(SCI_FALSE);
        }
        else
        {
            _DCAMERA_SetPreviewExFlag(SCI_FALSE);
            _DCAMERA_GetMutex(s_callback_mutex_ptr);
            ISP_ServiceStopVTExt();

            if(s_dc_info_ptr->p_preview_buf)
            {
                SCI_Free(s_dc_info_ptr->p_preview_buf);
                s_dc_info_ptr->p_preview_buf = SCI_NULL;
            }
            _DCAMERA_PutMutex(s_callback_mutex_ptr);

            if(DCAMERA_OP_VIDEOCALL != s_operation_mode)
            {
                LCD_SetInvalidateLimit(s_dc_info_ptr->isp_preview_param.disp_lcd_id,PERMIT_GUI_INVALIDATE);
            }
        }
        dv_zoom_level_last_time = zoom_level_last_time;
        zoom_level_last_time = DCAMERA_PREVIEW_ZOOM_LEVEL_MAX;
        _DCAMERA_SetIsStopPreview(SCI_TRUE);
    }
    else
    {
        //DCAMERA_TRACE:"DCAMERA_StopPreview,stop error,preview is already been stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6895_112_2_18_1_24_12_457,(uint8*)"");
    }

#if 0
    if(DCAMERA_OP_NORMAL == s_operation_mode||
       DCAMERA_OP_SPECIAL == s_operation_mode ||
       DCAMERA_OP_MINIDC == s_operation_mode)
    {
        _DCAMERA_CloseISP();
    }
#endif

    return DCAMERA_OP_SUCCESS;
}


/******************************************************************************/
// Description: snapshot single image
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoSnapshot
    (
        DCAMERA_SNAPSHOT_PARAM_T const  *   param_ptr,
        DCAMERA_SNAPSHOT_RETURN_PARAM_T *   return_param_ptr
    )
{
    DCAMERA_RETURN_VALUE_E ret_val = DCAMERA_OP_SUCCESS;
#if defined(DC_EX_DEBUG)
    DCAMERA_SNAPSHOT_EX_PARAM_T         snapshot_ex = {0};

    b_image_cb_for_snapshot_ex = 1;
    jpg_length = 0;
    ex_snapshot_callback = param_ptr->snapshot_callback;
    snapshot_ex.target_info[0].target_size.w = 640;
    snapshot_ex.target_info[0].target_size.h = 480;
    snapshot_ex.target_info[0].rotation_mode = DCAMERA_ROTATION_0;
    snapshot_ex.target_info[0].data_type = DCAMERA_DATA_TYPE_YUV422;//DCAMERA_DATA_TYPE_YUV422;
    snapshot_ex.target_info[0].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;
    snapshot_ex.app_data_ptr = (void*)0x12345678;

    snapshot_ex.target_info[1].target_size.w = 480;
    snapshot_ex.target_info[1].target_size.h = 320;
    snapshot_ex.target_info[1].rotation_mode = DCAMERA_ROTATION_270;
    snapshot_ex.target_info[1].data_type = DCAMERA_DATA_TYPE_YUV420;//DCAMERA_DATA_TYPE_YUV422;
    snapshot_ex.target_info[1].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;

    snapshot_ex.target_info[2].target_size.w = 400;
    snapshot_ex.target_info[2].target_size.h = 240;
    snapshot_ex.target_info[2].rotation_mode = DCAMERA_ROTATION_180;
    snapshot_ex.target_info[2].data_type = DCAMERA_DATA_TYPE_YUV422;//DCAMERA_DATA_TYPE_YUV422;
    snapshot_ex.target_info[2].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;

    snapshot_ex.target_info[3].target_size.w = 1600;
    snapshot_ex.target_info[3].target_size.h = 1200;
    snapshot_ex.target_info[3].rotation_mode = DCAMERA_ROTATION_MIRROR;
    snapshot_ex.target_info[3].data_type = DCAMERA_DATA_TYPE_JPEG;//DCAMERA_DATA_TYPE_RGB565;//DCAMERA_DATA_TYPE_YUV422;
    snapshot_ex.target_info[3].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;

    snapshot_ex.enc_quality_level = 6;
    snapshot_ex.flash_enable = 0;
    snapshot_ex.interval_of_frame = 0;
    snapshot_ex.target_frame_count = 2;
    snapshot_ex.quickview_callback = param_ptr->quickview_callback;
    snapshot_ex.snapshot_num = 1;
    snapshot_ex.b_exif_needed = 0;
    snapshot_ex.image_notice_cb = image_notice_callback;
    ret_val = DCAMERA_DoSnapshotEX(&snapshot_ex, SCI_NULL);
#else

    b_burst_snapshot = 0;

    if(DCAMERA_IsSupport2MSnapshot())
    {
        _DCAMERA_HandleSensorOutputSize(s_dc_info_ptr->jpeg_rect);
    }

    if(PNULL != param_ptr->snapshot_callback)
    {

        ret_val = _DCAMERA_DoSnapshot((DCAMERA_SNAPSHOT_PARAM_T*)param_ptr, return_param_ptr);
    }
    else
    {
        if(SCI_INVALID_BLOCK_ID != s_dc_task_id)
        //if(SCI_SUCESS == SCI_IsThreadExist(s_dc_task_id))
            _DCAMERA_SnapShotSendSignal(param_ptr);
    }

#endif
    return ret_val;
}
LOCAL void _DCAMERA_YUV422DownSample2YUV420(DCAMERA_IMAGE_PARAM_T *p_target_image,
                                                                                                 uint32                                  frame_id)
{
    uint32                      i = 0;
    uint32                      src_chn = 0, dst_chn = 0;
    uint32                      size_each_line = 0;

    size_each_line =  (uint32)p_target_image->frame_info_t[frame_id].target_size.w;
    dst_chn = p_target_image->dcamera_addr[frame_id].chn2_addr + size_each_line;
    src_chn = dst_chn + size_each_line;

    //DCAMERA_TRACE:"_DCAMERA_YUV422DownSample2YUV420 ,size_each_line %d, height %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_6992_112_2_18_1_24_13_458,(uint8*)"dd",size_each_line,p_target_image->frame_info_t[frame_id].target_size.h);

    for(i = 1; i < (uint32)(p_target_image->frame_info_t[frame_id].target_size.h >> 1); i++)
    {
        SCI_MEMCPY((void*)dst_chn, (void*)src_chn, size_each_line);
        src_chn += (size_each_line << 1);
        dst_chn += size_each_line;
    }

    return;
}
LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_GetSpecificImage(ISP_FRAME_T*                 p_captured,
                                                       DCAMERA_IMAGE_FRAME_PARAM_T* p_tartget_desc,
                                                       DCAMERA_IMAGE_PARAM_T*       p_target_image,
                                                       uint32                       frame_id,
                                                       uint32                       mem_start,
                                                       uint32                       *p_mem_used,
                                                       uint16                       jpg_enc_quality,
                                                       BOOLEAN                      b_exif)
{
    DCAMERA_RETURN_VALUE_E          ret_val = DCAMERA_OP_SUCCESS;
    ISP_FRAME_T                     src_frame_t = {0};
    ISP_FRAME_T                     dst_frame_t = {0};
    uint32                          frame_length = 0;
    JPEG_ENC_IN_PARAM_T             jpeg_enc_in_param = {0};
    JPEG_ENC_OUT_PARAM_T            jpeg_enc_out_param= {0};
    uint32                          jpeg_enc_rtn = SCI_SUCCESS;
    uint32                          b_need_transform = 0;
    uint32                          b_need_rotation = 0;
    DCAMERA_SIZE_T                  dest_size = {0};
    uint32                          rot_buf = 0;
    DCAMERA_ROTATION_PARAM_T        rot_param = {0};
#if defined(_DC_EXIF_)
    DCAMERA_SIZE_T                  dc_size = {0};
    DCAMERA_RECT_T                  dc_rect = {0};
#endif

    SCI_MEMCPY((void*)&p_target_image->frame_info_t[frame_id],
               (void*)p_tartget_desc,
               sizeof(DCAMERA_IMAGE_FRAME_PARAM_T));

    if(p_tartget_desc->rotation_mode > DCAMERA_ROTATION_0 &&
       p_tartget_desc->rotation_mode < DCAMERA_ROTATION_MAX)
    {
        b_need_rotation = 1;
        if(p_tartget_desc->rotation_mode == DCAMERA_ROTATION_90 ||
           p_tartget_desc->rotation_mode == DCAMERA_ROTATION_270)
        {
            dest_size.w = p_tartget_desc->target_size.h;
            dest_size.h = p_tartget_desc->target_size.w;
        }
        else
        {
            dest_size.w = p_tartget_desc->target_size.w;
            dest_size.h = p_tartget_desc->target_size.h;
        }
        frame_length = dest_size.w * dest_size.h + DCAMERA_PADDING_MEM_SIZE;
        rot_buf = (uint32)SCI_ALLOCA(frame_length*2);
        if(SCI_NULL == rot_buf)
        {
            return DCAMERA_OP_NO_ENOUGH_MEMORY;
        }
        rot_param.dest_chn_addr.y_chn_addr = DC_ADDR(rot_buf);
        rot_param.dest_chn_addr.u_chn_addr = rot_param.dest_chn_addr.y_chn_addr + frame_length;
        rot_param.dest_chn_addr.u_chn_addr = DC_ADDR(rot_param.dest_chn_addr.u_chn_addr);
        rot_param.src_chn_addr.y_chn_addr = rot_param.dest_chn_addr.y_chn_addr;
        rot_param.src_chn_addr.u_chn_addr = rot_param.dest_chn_addr.u_chn_addr;
        rot_param.rotation_mode = p_tartget_desc->rotation_mode;
        rot_param.image_width = dest_size.w;
        rot_param.image_height= dest_size.h;
    }
    else
    {
        dest_size.w = p_tartget_desc->target_size.w;
        dest_size.h = p_tartget_desc->target_size.h;
    }


    if(DCAMERA_DATA_TYPE_YUV422 == p_tartget_desc->data_type &&
       dest_size.w == p_captured->width &&
       dest_size.h == p_captured->height)
    {
        b_need_transform = 0;
    }
    else if(DCAMERA_DATA_TYPE_RAW == p_tartget_desc->data_type)
    {
        b_need_transform = 0;
    }
    else
    {
        b_need_transform = 1;
    }


    if(DCAMERA_DATA_TYPE_JPEG == p_tartget_desc->data_type)
    {
        if(s_dc_info_ptr->snapshot_ctl_info.param == SNAPSHOT_JPG &&
           s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width == p_tartget_desc->target_size.w &&
           s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height == p_tartget_desc->target_size.h &&
           s_dc_info_ptr->preview_ctl_info.zoom_level == 0 &&
           b_need_rotation == 0)
        {
            JINF_EXIF_INFO_T                     *p_exif_info = DC_GetExifParameter();
            JINF_WEXIF_IN_PARAM_T           exif_in_param = {0};
            JINF_WEXIF_OUT_PARAM_T         exif_out_param={0};

            //no need to zoom or encode
           //DCAMERA_TRACE:"_DCAMERA_GetSpecificImage, JPEG buf 0x%x, length 0x%x mem_start  0x%x "
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7102_112_2_18_1_24_13_459,(uint8*)"ddd",s_dc_info_ptr->jpeg_buf,s_dc_info_ptr->buf_len,mem_start);
            SCI_MEMCPY((void*)(mem_start + DC_EXIF_LENGTH),(void*)s_dc_info_ptr->jpeg_buf,s_dc_info_ptr->buf_len);
            *p_mem_used = s_dc_info_ptr->buf_len + DC_EXIF_LENGTH;

            if(p_exif_info->spec_ptr)
            {
                p_exif_info->spec_ptr->basic.PixelXDimension = p_tartget_desc->target_size.w;
                p_exif_info->spec_ptr->basic.PixelYDimension = p_tartget_desc->target_size.h;
            }

            DC_SetExifImagePixel(p_tartget_desc->target_size.w,p_tartget_desc->target_size.h);
            exif_in_param.src_jpeg_buf_ptr = (uint8*)(mem_start + DC_EXIF_LENGTH);
            exif_in_param.src_jpeg_size    = s_dc_info_ptr->buf_len;
            exif_in_param.temp_buf_ptr     = (uint8*)DCAMERA_CaptureGetJPGTmpBuf(&exif_in_param.temp_buf_size);
            exif_in_param.wrtie_file_func  = SCI_NULL;
            exif_in_param.target_buf_ptr   = (uint8*)mem_start;
            exif_in_param.target_buf_size  = DC_EXIF_LENGTH;
            exif_in_param.exif_info_ptr    = p_exif_info;
            ret_val = IMGJPEG_WriteExif(&exif_in_param, &exif_out_param);/*lint !e64*/
            if(SCI_SUCCESS != ret_val)
            {
                //DCAMERA_TRACE:"DC, Add exif error %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7123_112_2_18_1_24_13_460,(uint8*)"d", ret_val);
                ret_val = DCAMERA_OP_ENCODE_ERR;
            }
            p_target_image->dcamera_addr[frame_id].chn1_addr = (uint32)exif_out_param.output_buf_ptr;
            p_target_image->dcamera_addr[frame_id].chn2_addr = exif_out_param.output_size;

        }
        else
        {
            SCI_MEMCPY((void*)&dst_frame_t, (void*)p_captured,sizeof(ISP_FRAME_T));
            if(dest_size.w != p_captured->width ||
               dest_size.h != p_captured->height)
            {
                SCI_MEMCPY((void*)&src_frame_t, (void*)p_captured,sizeof(ISP_FRAME_T));
                frame_length = (uint32)(dest_size.w * dest_size.h);
                dst_frame_t.yaddr = DC_ADDR(mem_start);
                dst_frame_t.uaddr = dst_frame_t.yaddr + frame_length;
                dst_frame_t.uaddr = DC_ADDR(dst_frame_t.uaddr);
                dst_frame_t.width = dest_size.w;
                dst_frame_t.height = dest_size.h;
                dst_frame_t.type = ISP_IMAGE_FORMAT_YUV422;
                _DCAMERA_ImageScale(&src_frame_t, &dst_frame_t);
            }
            // set jpeg encode information
            jpeg_enc_in_param.quality_level = jpg_enc_quality;
            jpeg_enc_in_param.image_rect.x = 0;
            jpeg_enc_in_param.image_rect.y = 0;
            if(b_need_rotation)
            {
                rot_param.src_chn_addr.y_chn_addr = dst_frame_t.yaddr;
                rot_param.src_chn_addr.u_chn_addr = dst_frame_t.uaddr;
                _DCAMERA_RotateYUV422(&rot_param);
                jpeg_enc_in_param.yuv_addr.y_chn_addr = (uint32)(rot_param.dest_chn_addr.y_chn_addr);
                jpeg_enc_in_param.yuv_addr.u_chn_addr = (uint32)(rot_param.dest_chn_addr.u_chn_addr);

            }
            else
            {
                jpeg_enc_in_param.yuv_addr.y_chn_addr = (uint32)(dst_frame_t.yaddr);
                jpeg_enc_in_param.yuv_addr.u_chn_addr = (uint32)(dst_frame_t.uaddr);
            }

            jpeg_enc_in_param.image_rect.w = p_tartget_desc->target_size.w;
            jpeg_enc_in_param.image_rect.h = p_tartget_desc->target_size.h;
            jpeg_enc_in_param.jpeg_buf_ptr  = (uint8*)DCAMERA_CaptureGetJPGBuf(&jpeg_enc_in_param.jpeg_buf_size);

#if defined(_DC_EXIF_)
            if(b_exif == SCI_TRUE)
            {
                jpeg_enc_in_param.jpeg_buf_ptr =  (uint8*)((uint32)(jpeg_enc_in_param.jpeg_buf_ptr) + DC_EXIF_LENGTH);
                jpeg_enc_in_param.jpeg_buf_size -= DC_EXIF_LENGTH;
                if(jpeg_enc_in_param.image_rect.w > VGA_WIDTH)
                {
                    jpeg_enc_in_param.jpeg_buf_ptr =  (uint8*)((uint32)jpeg_enc_in_param.jpeg_buf_ptr + DC_THUMBNAIL_LENGTH);
                    jpeg_enc_in_param.jpeg_buf_size -= DC_THUMBNAIL_LENGTH;
                }

            }
 #endif
            jpeg_enc_in_param.enc_data_type = JPEG_ENC_DATA_TYPE_YUV422;

            //DCAMERA_TRACE:"DC: Call JPEG_EncodeJpeg, start time = %d,jpeg_buf_ptr = 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7185_112_2_18_1_24_13_461,(uint8*)"dd", SCI_GetTickCount(),jpeg_enc_in_param.jpeg_buf_ptr);

            jpeg_enc_rtn = JPEG_EncodeJpeg(&jpeg_enc_in_param, &jpeg_enc_out_param);
            if(JPEG_ENC_OK != jpeg_enc_rtn)
            {
                if(SCI_NULL != rot_buf)
                {
                    SCI_Free((void*)rot_buf);
                    rot_buf = SCI_NULL;
                }
                return DCAMERA_OP_ENCODE_ERR;
            }

#if defined(_DC_EXIF_)
            if(b_exif == SCI_TRUE)
            {
                dc_size.w = p_tartget_desc->target_size.w;
                dc_size.h = p_tartget_desc->target_size.h;
                dc_rect.w = dc_size.w;
                dc_rect.h = dc_size.h;
                dst_frame_t.yaddr = jpeg_enc_in_param.yuv_addr.y_chn_addr;
                dst_frame_t.uaddr = jpeg_enc_in_param.yuv_addr.u_chn_addr;
                _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dest_size, &dc_rect);//for is has been rotated before now
                ret_val = _DCAMERA_JpgAddExif(&dst_frame_t,
                                              &dc_size,
                                              &dc_rect,
                                              (uint32*)&jpeg_enc_in_param.jpeg_buf_ptr,
                                              &jpeg_enc_out_param.out_size,
                                              jpg_enc_quality);
                if(DCAMERA_OP_SUCCESS != ret_val)
                {
                    if(SCI_NULL != rot_buf)
                    {
                        SCI_Free((void*)rot_buf);
                        rot_buf = SCI_NULL;
                    }

                    return DCAMERA_OP_ENCODE_ERR;
                }
            }
#endif

            *p_mem_used = jpeg_enc_out_param.out_size;
            SCI_MEMCPY((void*)mem_start,(void*)jpeg_enc_in_param.jpeg_buf_ptr,*p_mem_used);
            p_target_image->dcamera_addr[frame_id].chn1_addr = mem_start;
            p_target_image->dcamera_addr[frame_id].chn2_addr = *p_mem_used ;
        }

        //DCAMERA_TRACE:"DC: Call JPEG_EncodeJpeg, end time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7233_112_2_18_1_24_13_462,(uint8*)"d", SCI_GetTickCount());
    }
    else if(b_need_transform)
    {
        SCI_MEMCPY((void*)&src_frame_t, (void*)p_captured, sizeof(ISP_FRAME_T));
        frame_length = (uint32)(dest_size.w * dest_size.h);
        if(b_need_rotation)
        {
            dst_frame_t.yaddr = rot_param.src_chn_addr.y_chn_addr;
            dst_frame_t.uaddr = rot_param.src_chn_addr.u_chn_addr;
        }
        else
        {
            dst_frame_t.yaddr = DC_ADDR(mem_start);
            dst_frame_t.uaddr = dst_frame_t.yaddr + frame_length;
            dst_frame_t.uaddr = DC_ADDR(dst_frame_t.uaddr);
        }
        dst_frame_t.width = dest_size.w;
        dst_frame_t.height = dest_size.h;
        dst_frame_t.type = _DCAMERA_GetISPDateType(p_tartget_desc->data_type,p_tartget_desc->data_pattern);
        _DCAMERA_ImageScale(&src_frame_t, &dst_frame_t);
        if(b_need_rotation)
        {
            rot_param.src_chn_addr.y_chn_addr = dst_frame_t.yaddr;
            rot_param.src_chn_addr.u_chn_addr = dst_frame_t.uaddr;
            rot_param.dest_chn_addr.y_chn_addr = DC_ADDR(mem_start);
            rot_param.dest_chn_addr.u_chn_addr = rot_param.dest_chn_addr.y_chn_addr + frame_length;
            rot_param.dest_chn_addr.u_chn_addr = DC_ADDR(rot_param.dest_chn_addr.u_chn_addr);
            if(p_tartget_desc->data_type == DCAMERA_DATA_TYPE_RGB565)
            {
                _DCAMERA_RotateRGB565(&rot_param);
            }
            else if(p_tartget_desc->data_type == DCAMERA_DATA_TYPE_YUV422)
            {
                _DCAMERA_RotateYUV422(&rot_param);
            }
            else if(p_tartget_desc->data_type == DCAMERA_DATA_TYPE_YUV420)
            {
                _DCAMERA_RotateYUV420(&rot_param);
            }
            else
            {
                ret_val = DCAMERA_OP_PARAM_ERR;
            }
            dst_frame_t.yaddr = rot_param.dest_chn_addr.y_chn_addr;
            dst_frame_t.uaddr = rot_param.dest_chn_addr.u_chn_addr;
        }
        p_target_image->dcamera_addr[frame_id].chn1_addr = dst_frame_t.yaddr;
        p_target_image->dcamera_addr[frame_id].chn2_addr = dst_frame_t.uaddr;
        *p_mem_used = (dst_frame_t.uaddr + frame_length) - mem_start;

    }
    else if(DCAMERA_DATA_TYPE_YUV422 == p_tartget_desc->data_type &&
            b_need_rotation)
    {
        frame_length = (uint32)(dest_size.w * dest_size.h);
        rot_param.src_chn_addr.y_chn_addr = p_captured->yaddr;
        rot_param.src_chn_addr.u_chn_addr = p_captured->uaddr;
        rot_param.dest_chn_addr.y_chn_addr = DC_ADDR((uint32)mem_start);
        rot_param.dest_chn_addr.u_chn_addr = rot_param.dest_chn_addr.y_chn_addr + frame_length;
        rot_param.dest_chn_addr.u_chn_addr = DC_ADDR(rot_param.dest_chn_addr.u_chn_addr);
        _DCAMERA_RotateYUV422(&rot_param);
        p_target_image->dcamera_addr[frame_id].chn1_addr = rot_param.dest_chn_addr.y_chn_addr;
        p_target_image->dcamera_addr[frame_id].chn2_addr = rot_param.dest_chn_addr.u_chn_addr;
        *p_mem_used = (rot_param.dest_chn_addr.u_chn_addr + frame_length) - mem_start;

    }
    else if(DCAMERA_DATA_TYPE_RAW == p_tartget_desc->data_type)
    {
        *p_mem_used = (uint32)(dest_size.w * dest_size.h * 2);
        SCI_MEMCPY((void*)mem_start,(void*)p_captured->yaddr,*p_mem_used);

        p_target_image->dcamera_addr[frame_id].chn1_addr = mem_start;
    }
    else if(DCAMERA_DATA_TYPE_YUV422 == p_tartget_desc->data_type)
    {
        frame_length = (uint32)(dest_size.w * dest_size.h);
        p_target_image->dcamera_addr[frame_id].chn1_addr = (uint32)(((uint32)mem_start + 0xFF) & ~0xFF);
        p_target_image->dcamera_addr[frame_id].chn2_addr = p_target_image->dcamera_addr[frame_id].chn1_addr + frame_length;
        p_target_image->dcamera_addr[frame_id].chn2_addr = (uint32)(((uint32)p_target_image->dcamera_addr[frame_id].chn2_addr + 0xFF) & ~0xFF);
        SCI_MEMCPY((void*)p_target_image->dcamera_addr[frame_id].chn1_addr,
                   (void*)p_captured->yaddr,
                   frame_length);
        SCI_MEMCPY((void*)p_target_image->dcamera_addr[frame_id].chn2_addr,
                   (void*)p_captured->uaddr,
                   frame_length);
       *p_mem_used = (p_target_image->dcamera_addr[frame_id].chn2_addr + frame_length) - mem_start;
       //DCAMERA_TRACE:"_DCAMERA_GetSpecificImage, YUV no transformed ,y 0x%x, u 0x%x "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7322_112_2_18_1_24_13_463,(uint8*)"dd",p_target_image->dcamera_addr[frame_id].chn1_addr,p_target_image->dcamera_addr[frame_id].chn2_addr);

    }
    else
    {
        ret_val = DCAMERA_OP_PARAM_ERR;
    }

    if(SCI_NULL != rot_buf)
    {
        SCI_Free((void*)rot_buf);
        rot_buf = SCI_NULL;
    }
    return ret_val;
}

LOCAL void                   _DCAMERA_SnapshotExFreeMem(void)
{
    if(SCI_NULL == s_dc_info_ptr)
        return ;

    if(s_dc_info_ptr->snapshot_ex_yuv)
    {
        //DCAMERA_TRACE:"_DCAMERA_SnapshotExFreeMem , free s_dc_info_ptr->snapshot_ex_yuv 0x%x, length %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7347_112_2_18_1_24_13_464,(uint8*)"dd",s_dc_info_ptr->snapshot_ex_yuv,s_dc_info_ptr->snapshot_ex_yuv_len);
        SCI_Free((void*)s_dc_info_ptr->snapshot_ex_yuv);
        s_dc_info_ptr->snapshot_ex_yuv = (void*)SCI_NULL;
        s_dc_info_ptr->snapshot_ex_yuv_len = 0;
    }
    if(s_dc_info_ptr->snapshot_ex_addi_mem != (void*)SCI_NULL)
    {
        //DCAMERA_TRACE:"_DCAMERA_SnapshotExFreeMem , free s_dc_info_ptr->snapshot_ex_addi_mem 0x%x, length %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7356_112_2_18_1_24_13_465,(uint8*)"dd",s_dc_info_ptr->snapshot_ex_addi_mem,s_dc_info_ptr->snapshot_ex_addi_mem_len);
        SCI_Free((void*)s_dc_info_ptr->snapshot_ex_addi_mem);
        s_dc_info_ptr->snapshot_ex_addi_mem = (void*)SCI_NULL;
        s_dc_info_ptr->snapshot_ex_addi_mem_len = 0;
    }
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_DoSnapshotExYUVZoom()
{
    DCAMERA_SIZE_T                  dc_dst_size  = {0};
    DCAMERA_RECT_T                  dc_rect = {0};
    ISP_SCLAE_PARAM_T               scale_para = {0};
    uint32                          mem_start = 0;
    uint32                          mem_size = 0;
    DCAMERA_RETURN_VALUE_E          rtn = DCAMERA_OP_SUCCESS;

    mem_start = (uint32)DCAMERA_CaptureExGetScaleBuf(&mem_size);
    ISP_ServiceMemConfig((void*)mem_start, mem_size);

    dc_dst_size.w = s_dc_info_ptr->jpeg_rect.w;
    dc_dst_size.h = s_dc_info_ptr->jpeg_rect.h;
    dc_rect.w = (uint16)s_dc_info_ptr->isp_frame.width;
    dc_rect.h = (uint16)s_dc_info_ptr->isp_frame.height;
    _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);
    scale_para.output_size.w    = (uint32)dc_dst_size.w;
    scale_para.output_size.h    = (uint32)dc_dst_size.h;
    scale_para.input_addr.yaddr = s_dc_info_ptr->isp_frame.yaddr;
    scale_para.input_addr.uaddr = s_dc_info_ptr->isp_frame.uaddr;
    scale_para.input_size.w     = s_dc_info_ptr->isp_frame.width;
    scale_para.input_size.h     = s_dc_info_ptr->isp_frame.height;
    scale_para.input_rect.x     = (uint32)dc_rect.x;
    scale_para.input_rect.y     = (uint32)dc_rect.y;
    scale_para.input_rect.w     = (uint32)dc_rect.w;
    scale_para.input_rect.h     = (uint32)dc_rect.h;
    scale_para.input_format     = ISP_IMAGE_FORMAT_YUV422;
    scale_para.output_addr.yaddr= DC_ADDR((uint32)s_dc_info_ptr->snapshot_ex_yuv);
    scale_para.output_addr.uaddr= scale_para.output_addr.yaddr + (uint32)(dc_dst_size.w * dc_dst_size.h);
    scale_para.output_addr.uaddr= DC_ADDR(scale_para.output_addr.uaddr);
    scale_para.output_format    = ISP_IMAGE_FORMAT_YUV422;

    ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, SCI_NULL);
    ISP_ServiceSetScaleParam(&scale_para);
    ISP_ServiceStartScale();
    s_dc_info_ptr->isp_frame.yaddr = scale_para.output_addr.yaddr;
    s_dc_info_ptr->isp_frame.uaddr = scale_para.output_addr.uaddr;
    s_dc_info_ptr->isp_frame.width = scale_para.output_size.w;
    s_dc_info_ptr->isp_frame.height= scale_para.output_size.h;

    return rtn;
}

LOCAL int _DCAMERA_DoSnapShotExReview(void)
{
    int                         rVal = 0;
    ISP_REVIEW_PARAM_T          review_param = {0};
    uint32                      mem_start = 0;
    uint32                      mem_size = 0;
    ISP_RECT_T                  logic_disp_rect = {0};
    ISP_RECT_T                  logic_lcdc_rect = {0};
    ISP_RECT_T                  img_src_rect = {0};
    DCAMERA_CONVERT_COOR_T      src_coor = {0};
    DCAMERA_CONVERT_COOR_T      dst_coor = {0};

    //DCAMERA_TRACE:"DC _DCAMERA_DoSnapShotExReview, start time %d  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7419_112_2_18_1_24_13_466,(uint8*)"d", SCI_GetTickCount());

    review_param.disp_range = s_dc_info_ptr->isp_preview_param.disp_range;
    review_param.lcd_rect   = s_dc_info_ptr->isp_preview_param.lcd_rect;
    review_param.disp_lcd_id = s_dc_info_ptr->isp_preview_param.disp_lcd_id;
    review_param.disp_block_id = s_dc_info_ptr->isp_preview_param.disp_block_id;
    review_param.input_size.w = s_dc_info_ptr->isp_frame.width;
    review_param.input_size.h = s_dc_info_ptr->isp_frame.height;


    src_coor.rot_degree =(DCAMERA_ROTATION_MODE_E)s_dc_info_ptr->isp_preview_param.disp_rotation;

    src_coor.rect.x = (uint16)review_param.disp_range.x;
    src_coor.rect.y = (uint16)review_param.disp_range.y;
    src_coor.rect.w = (uint16)review_param.disp_range.w;
    src_coor.rect.h = (uint16)review_param.disp_range.h;
    DCAMERA_Convert_Coor(review_param.disp_lcd_id, &src_coor, &dst_coor);
    logic_disp_rect.x = (uint32)dst_coor.rect.x;
    logic_disp_rect.y = (uint32)dst_coor.rect.y;
    logic_disp_rect.w = (uint32)dst_coor.rect.w;
    logic_disp_rect.h = (uint32)dst_coor.rect.h;

    src_coor.rect.x = (uint16)review_param.lcd_rect.x;
    src_coor.rect.y = (uint16)review_param.lcd_rect.y;
    src_coor.rect.w = (uint16)review_param.lcd_rect.w;
    src_coor.rect.h = (uint16)review_param.lcd_rect.h;
    DCAMERA_Convert_Coor(review_param.disp_lcd_id, &src_coor, &dst_coor);
    logic_lcdc_rect.x = (uint32)dst_coor.rect.x;
    logic_lcdc_rect.y = (uint32)dst_coor.rect.y;
    logic_lcdc_rect.w = (uint32)dst_coor.rect.w;
    logic_lcdc_rect.h = (uint32)dst_coor.rect.h;

    img_src_rect.w = review_param.input_size.w;
    img_src_rect.h = review_param.input_size.h;
     _DCamera_AdjustRect(&img_src_rect,
                        &logic_lcdc_rect,
                        &logic_disp_rect,
                        s_dc_info_ptr->display_mode);

    // find the difference between the JPG and YUV
    if(SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
    {
        review_param.input_format = ISP_IMAGE_FORMAT_YUV420;
    }
    else
    {
        review_param.input_format = ISP_IMAGE_FORMAT_YUV422;
    }

    review_param.input_addr.yaddr = s_dc_info_ptr->isp_frame.yaddr;
    review_param.input_addr.uaddr = s_dc_info_ptr->isp_frame.uaddr;

    review_param.disp_rotation = (ISP_ROTATION_E)src_coor.rot_degree;
    review_param.input_range.x = (uint32)img_src_rect.x;
    review_param.input_range.y = (uint32)img_src_rect.y;
    review_param.input_range.w = (uint32)img_src_rect.w;
    review_param.input_range.h = (uint32)img_src_rect.h;

    review_param.disp_range = s_dc_info_ptr->isp_preview_param.disp_range;
    review_param.lcd_rect = s_dc_info_ptr->isp_preview_param.lcd_rect;
    mem_start = (uint32)DCAMERA_CaptureGetQuickViewBuf(&mem_size);
    ISP_ServiceMemConfig((void*)mem_start, mem_size);
    ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_DISPLAY);
    ISP_ServiceSetReviewParam(&review_param);

    ISP_ServiceStartReview();

    //DCAMERA_TRACE:"DC _DCAMERA_DoSnapShotExReview end, end time %d  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7486_112_2_18_1_24_14_467,(uint8*)"d", SCI_GetTickCount());

    return rVal;
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_DoSnapshotEx(DCAMERA_SNAPSHOT_EX_PARAM_T *    param_ptr,
                                                   DCAMERA_SNAPSHOT_EX_RETURN_PARAM_T*   return_param_ptr)
{
    DCAMERA_RETURN_VALUE_E          ret_val           = DCAMERA_OP_SUCCESS;
    uint32                          preview_mode      = 0;
    uint32                          interval_of_frame = 0;
    DCAMERA_RECT_T                  dc_rect = {0};
    DCAMERA_IMAGE_PARAM_T           dst_image = {0};
    uint8                           i = 0, j = 0;
    uint32                          addi_mem_used = 0;
    uint32                          addi_mem_total_used = 0;
    DCAMERA_SNAPSHOT_PARAM_T        param_mem_req = {0};
    uint32                          mem_start = 0;
    uint32                          mem_size = 0;
    uint32                          b_reuse_dec_buf = 0;
    uint32                          reuse_image_id = 0;
    DCAMERA_PREVIEW_CTL_INFO_T      *preview_ctl_info_ptr = &s_dc_info_ptr->preview_ctl_info;


    SCI_ASSERT(NULL != param_ptr);/*assert verified*/
    if(!DCAMERA_IsOpen())
    {
        return DCAMERA_OP_NOT_OPEN;
    }
    s_dc_info_ptr->is_snapshot_ex = 1;

    //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx -> start time = %d,image_count = %d,exif_need %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7522_112_2_18_1_24_14_468,(uint8*)"ddd", SCI_GetTickCount(),param_ptr->target_frame_count,param_ptr->b_exif_needed);

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    param_ptr->capture_start_callback(SCI_SUCCESS, PNULL, 0);
    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    if (_DCAMERA_IsPreview())
    {
        preview_mode = 0;
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        ISP_ServiceStopPreview();
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        _DCAMERA_SetPreviewFlag(SCI_FALSE);
    }
    else if(_DCAMERA_IsPreviewEx())
    {
        preview_mode = 1; //extension preview
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        ISP_ServiceStopVTExt();
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        _DCAMERA_SetPreviewExFlag(SCI_FALSE);
    }
    else // not in two preview mode, should open ISP_Service firstly
    {
        if(DCAMERA_OP_NORMAL == s_operation_mode||
           DCAMERA_OP_SPECIAL == s_operation_mode||
           DCAMERA_OP_MINIDC == s_operation_mode)
        {
            if(_DCAMERA_OpenISP())
            {
                return DCAMERA_OP_ISP_ERR;
            }
        }
    }

    for(i = 0; i < param_ptr->snapshot_num; i++)
    {
        dc_rect.w = 0;
        dc_rect.h = 0;
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        _DCAMERA_SnapshotExFreeMem();
        _DCAMERA_PutMutex(s_callback_mutex_ptr);

        for(j = 0; j < param_ptr->target_frame_count; j++)
        {
            if(param_ptr->target_info[j].rotation_mode == DCAMERA_ROTATION_90 ||
               param_ptr->target_info[j].rotation_mode == DCAMERA_ROTATION_270)
            {
                dc_rect.w = DCAMERA_MAX(dc_rect.w,param_ptr->target_info[j].target_size.h);
                dc_rect.h = DCAMERA_MAX(dc_rect.h,param_ptr->target_info[j].target_size.w);
            }
            else
            {
                dc_rect.w = DCAMERA_MAX(dc_rect.w,param_ptr->target_info[j].target_size.w);
                dc_rect.h = DCAMERA_MAX(dc_rect.h,param_ptr->target_info[j].target_size.h);
            }
        }
        s_dc_info_ptr->jpg_dec_target_size.w = dc_rect.w;
        s_dc_info_ptr->jpg_dec_target_size.h = dc_rect.h;
        s_dc_info_ptr->jpeg_rect.x = dc_rect.x;
        s_dc_info_ptr->jpeg_rect.y = dc_rect.y;
        s_dc_info_ptr->jpeg_rect.w = dc_rect.w;
        s_dc_info_ptr->jpeg_rect.h = dc_rect.h;
        _DCAMERA_HandleSensorOutputSize(dc_rect);
        s_dc_info_ptr->jpg_dec_target_size.w = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
        s_dc_info_ptr->jpg_dec_target_size.h = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;
        if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format)
        {
            s_dc_info_ptr->snapshot_ctl_info.param = SNAPSHOT_JPG;
        }
        else if(SENSOR_IMAGE_FORMAT_YUV422 == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format)
        {
            s_dc_info_ptr->snapshot_ctl_info.param = SNAPSHOT_NORMAL;
        }
        else
        {
            ret_val = DCAMERA_OP_PARAM_ERR;
            break;
        }

        for(j = 0; j < param_ptr->target_frame_count; j++)
        {
            mem_size = param_ptr->target_info[j].target_size.w* param_ptr->target_info[j].target_size.h * 2 + DCAMERA_PADDING_MEM_SIZE;
            if(s_dc_info_ptr->snapshot_ctl_info.param == SNAPSHOT_JPG &&
                param_ptr->target_info[j].data_type == DCAMERA_DATA_TYPE_JPEG &&
                param_ptr->target_info[j].rotation_mode == DCAMERA_ROTATION_0)
            {
                mem_size = (uint32)(mem_size / 4);
            }
            else if(param_ptr->target_info[j].rotation_mode == DCAMERA_ROTATION_0)
            {
                if(s_dc_info_ptr->jpg_dec_target_size.w == param_ptr->target_info[j].target_size.w &&
                   s_dc_info_ptr->jpg_dec_target_size.h == param_ptr->target_info[j].target_size.h &&
                   (param_ptr->target_info[j].data_type == DCAMERA_DATA_TYPE_YUV420 || param_ptr->target_info[j].data_type == DCAMERA_DATA_TYPE_YUV422))
                {
                    b_reuse_dec_buf = 1;
                    reuse_image_id = j;
                }
            }
            else if(param_ptr->target_info[j].data_type == DCAMERA_DATA_TYPE_YUV420 &&
                       param_ptr->target_info[j].target_size.w <= VGA_WIDTH)
            {
                mem_size =  (mem_size *3) >> 2;
            }

            s_dc_info_ptr->snapshot_ex_addi_mem_len += mem_size;
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx  image %d,width %d height %d,data type %d, Rot %d, mem_size 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7634_112_2_18_1_24_14_469,(uint8*)"dddddd",j,param_ptr->target_info[j].target_size.w,param_ptr->target_info[j].target_size.h,param_ptr->target_info[j].data_type,param_ptr->target_info[j].rotation_mode,s_dc_info_ptr->snapshot_ex_addi_mem_len);
        }

        if(1 == b_reuse_dec_buf &&
           param_ptr->target_frame_count)
        {
            s_dc_info_ptr->snapshot_ex_addi_mem_len = 0;
        }

        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        if(0 != s_dc_info_ptr->snapshot_ex_addi_mem_len)
        {
            s_dc_info_ptr->snapshot_ex_addi_mem = (uint32)SCI_ALLOCA(s_dc_info_ptr->snapshot_ex_addi_mem_len);
            if(SCI_NULL == s_dc_info_ptr->snapshot_ex_addi_mem)
            {
                ret_val = DCAMERA_OP_NO_ENOUGH_MEMORY;
                _DCAMERA_PutMutex(s_callback_mutex_ptr);
                break;
            }
        }
        _DCAMERA_PutMutex(s_callback_mutex_ptr);

        addi_mem_total_used = 0;
        addi_mem_used = 0;

        //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx, target image buffer 0x%x size %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7661_112_2_18_1_24_14_470,(uint8*)"dd",s_dc_info_ptr->snapshot_ex_addi_mem, s_dc_info_ptr->snapshot_ex_addi_mem_len);

        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        s_dc_info_ptr->snapshot_ex_yuv = SCI_NULL;
        s_dc_info_ptr->snapshot_ex_yuv_len = 0;
        if(s_dc_info_ptr->snapshot_ctl_info.param == SNAPSHOT_JPG)
        {
            for(j = 0; j < param_ptr->target_frame_count; j++)
            {
                if( param_ptr->target_info[i].data_type == DCAMERA_DATA_TYPE_JPEG )
                    break;
            }

            if(j < param_ptr->target_frame_count) //there's one image is in JPEG
            {
                if( s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width == param_ptr->target_info[j].target_size.w &&
                    s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height == param_ptr->target_info[j].target_size.h &&
                    param_ptr->target_info[j].rotation_mode == DCAMERA_ROTATION_0 &&
                    s_dc_info_ptr->preview_ctl_info.zoom_level == 0)
                {
                    if(param_ptr->target_frame_count != 1 ||
                        param_ptr->quickview_callback)
                    {
                        s_dc_info_ptr->jpg_dec_target_size.w = VGA_WIDTH;
                        s_dc_info_ptr->jpg_dec_target_size.h = VGA_HEIGHT;
                    }
                    else
                    {
                        //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx, no need to alloc yuv source buffer!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7689_112_2_18_1_24_14_471,(uint8*)"");
                        s_dc_info_ptr->jpg_dec_target_size.w = 0;
                        s_dc_info_ptr->jpg_dec_target_size.h = 0;
                    }
                }

            }

            s_dc_info_ptr->snapshot_ex_yuv_len =  s_dc_info_ptr->jpg_dec_target_size.w * s_dc_info_ptr->jpg_dec_target_size.h;
        }
        else
        {
            // YUV output from sensor, need no decode buffer
            s_dc_info_ptr->snapshot_ex_yuv_len = 0;
        }

        if(b_reuse_dec_buf)
        {
            s_dc_info_ptr->snapshot_ex_yuv_len = 0;
        }

        if(s_dc_info_ptr->snapshot_ex_yuv_len)
        {
            s_dc_info_ptr->snapshot_ex_yuv_len = (uint32)((s_dc_info_ptr->snapshot_ex_yuv_len + DCAMERA_PADDING_MEM_SIZE) * 2);
            s_dc_info_ptr->snapshot_ex_yuv = (void*)SCI_ALLOCA(s_dc_info_ptr->snapshot_ex_yuv_len);
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx, YUV buffer for the middle image 0x%x, length 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7716_112_2_18_1_24_14_472,(uint8*)"dd",s_dc_info_ptr->snapshot_ex_yuv , s_dc_info_ptr->snapshot_ex_yuv_len);
        }

        _DCAMERA_PutMutex(s_callback_mutex_ptr);

        ret_val = _DCAMERA_CapMemAssign(&param_mem_req);
        if(ret_val)
        {
            break;
        }

        //backup the preview buffer
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        _DCAMERA_BackupPreviewbuf2ReviewBuf(s_dc_info_ptr->isp_preview_param.disp_lcd_id,
                                            s_dc_info_ptr->isp_preview_param.disp_block_id);
        _DCAMERA_PutMutex(s_callback_mutex_ptr);

        if(DCAMERA_FLASH_AUTO == preview_ctl_info_ptr->flash_mode)
        {
            Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_TRUE);
        }

        Sensor_Ioctl(SENSOR_IOCTL_BEFORE_SNAPSHOT, (uint32)s_dc_info_ptr->snapshot_m);

        param_mem_req.data_buf = DCAMERA_CaptureGetJPGBuf(&param_mem_req.buf_len);
        if (_DCAMERA_GetFrameFromISP(&param_mem_req, 0, DCAMERA_ROTATION_0, param_ptr->quickview_callback))
        {
            if(DCAMERA_FLASH_AUTO == preview_ctl_info_ptr->flash_mode)
            {
                Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_FALSE);
            }
            _DCAMERA_CloseSensor();
            //DCAMERA_TRACE:"DC: DCAMERA_GetAFrameFromISP ERROR"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7735_112_2_18_1_24_14_473,(uint8*)"");
            ret_val = DCAMERA_OP_ISP_ERR;
            break;
        }

        Sensor_Ioctl(SENSOR_IOCTL_AFTER_SNAPSHOT, (uint32)s_dc_info_ptr->preview_m);

        if(DCAMERA_FLASH_AUTO == preview_ctl_info_ptr->flash_mode)
        {
            Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_FALSE);
        }

        if(param_ptr->quickview_callback)
        {
            _DCAMERA_DoSnapShotExReview();
        }

#if defined(_DC_EXIF_)
        if(param_ptr->b_exif_needed)
        {
            DC_SetExifImageDataTime();//set digital and original  time for image
        }
#endif

        if(SENSOR_IMAGE_FORMAT_YUV422 == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format &&
           s_dc_info_ptr->preview_ctl_info.zoom_level != 0)
        {
            _DCAMERA_DoSnapshotExYUVZoom();
        }

        mem_start = (uint32)DCAMERA_CaptureExGetScaleBuf(&mem_size);
        ISP_ServiceMemConfig((void*)mem_start, mem_size);

       // s_dc_info_ptr->isp_frame.width = s_dc_info_ptr->jpeg_rect.w;
       // s_dc_info_ptr->isp_frame.height= s_dc_info_ptr->jpeg_rect.h;
        s_dc_info_ptr->isp_frame.type = ISP_IMAGE_FORMAT_YUV422;

        for(j = 0; j < param_ptr->target_frame_count; j++)
        {
            _DCAMERA_GetMutex(s_callback_mutex_ptr);
            if(b_reuse_dec_buf && (j == reuse_image_id))
            {
                addi_mem_used = s_dc_info_ptr->isp_frame.uaddr + (uint32)(s_dc_info_ptr->isp_frame.width * s_dc_info_ptr->isp_frame.height) - (uint32)s_dc_info_ptr->snapshot_ex_addi_mem ;
            }
            else
            {
                ret_val = _DCAMERA_GetSpecificImage(&s_dc_info_ptr->isp_frame,
                                                    (DCAMERA_IMAGE_FRAME_PARAM_T*)&param_ptr->target_info[j],
                                                    &dst_image,
                                                    j,
                                                    ((uint32)s_dc_info_ptr->snapshot_ex_addi_mem + addi_mem_total_used),
                                                    &addi_mem_used,
                                                    param_ptr->enc_quality_level,
                                                    param_ptr->b_exif_needed);
            }
            addi_mem_total_used += addi_mem_used;
            _DCAMERA_PutMutex(s_callback_mutex_ptr);

        }

        if(b_reuse_dec_buf)
        {
            dst_image.frame_info_t[reuse_image_id].target_size.w = (uint16)s_dc_info_ptr->isp_frame.width;
            dst_image.frame_info_t[reuse_image_id].target_size.h = (uint16)s_dc_info_ptr->isp_frame.height;
            dst_image.frame_info_t[reuse_image_id].rotation_mode = DCAMERA_ROTATION_0;
            dst_image.frame_info_t[reuse_image_id].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;
            dst_image.dcamera_addr[reuse_image_id].chn1_addr = s_dc_info_ptr->isp_frame.yaddr;
            dst_image.dcamera_addr[reuse_image_id].chn2_addr = s_dc_info_ptr->isp_frame.uaddr;
            if(param_ptr->target_info[reuse_image_id].data_type == DCAMERA_DATA_TYPE_YUV420)
            {
                dst_image.frame_info_t[reuse_image_id].data_type = DCAMERA_DATA_TYPE_YUV420;
                _DCAMERA_YUV422DownSample2YUV420(&dst_image, reuse_image_id);
            }
            else
            {
                dst_image.frame_info_t[reuse_image_id].data_type = DCAMERA_DATA_TYPE_YUV422;
            }
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx reuse_image_id %d width  height %d %d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7810_112_2_18_1_24_14_474,(uint8*)"ddd",reuse_image_id,s_dc_info_ptr->isp_frame.width,s_dc_info_ptr->isp_frame.height);
        }
        if(ret_val)
        {
            break;
        }
        dst_image.frame_count = param_ptr->target_frame_count;
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        if(param_ptr->image_notice_cb)
        {
            (*param_ptr->image_notice_cb)(0, &dst_image, param_ptr->app_data_ptr);
        }
        _DCAMERA_PutMutex(s_callback_mutex_ptr);

        if(i < (param_ptr->snapshot_num - 1))
        {
            _DCAMERA_GetMutex(s_callback_mutex_ptr);
            if(0 == s_dc_info_ptr->b_preview_from)
            {
                ISP_ServiceSetPreviewParam(&s_dc_info_ptr->isp_preview_param);
                ISP_ServiceStartPreview();
            }
            else
            {
                ISP_ServiceSetVTExtEncodeParam(&s_dc_info_ptr->vt_enc_param_t);
                ISP_ServiceStartVTExt();
            }
            _DCAMERA_PutMutex(s_callback_mutex_ptr);

            if(ISP_ServiceGetLastErr())
            {
                return DCAMERA_OP_ISP_ERR;
            }

            interval_of_frame = param_ptr->interval_of_frame;
            if(param_ptr->interval_of_frame < DCAMERA_BST_INTERVAL_TIME_MIN)
            {
                interval_of_frame = DCAMERA_BST_INTERVAL_TIME_MIN;
            }
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx - > interval time of frame %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7849_112_2_18_1_24_14_475,(uint8*)"d", interval_of_frame);
            SCI_Sleep(interval_of_frame);
        }

    }

    if(param_ptr->snapshot_num > 1 || param_ptr->target_frame_count > 1)
    {
        b_review_next_to_snapshot = 0;
    }

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    _DCAMERA_SnapshotExFreeMem();

    if(ret_val && param_ptr->image_notice_cb)
    {
        (*param_ptr->image_notice_cb)(ret_val, SCI_NULL, param_ptr->app_data_ptr);
    }
    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    if(DCAMERA_OP_NORMAL == s_operation_mode||
       DCAMERA_OP_SPECIAL == s_operation_mode||
       DCAMERA_OP_MINIDC == s_operation_mode)
    {
        _DCAMERA_CloseISP();
    }

    if(s_dc_info_ptr->prev_ex_wait)
    {
        SCI_SetEvent(s_dc_event, DC_SNAPSHOT_EX_EVENT, SCI_OR);
    }
    s_dc_info_ptr->is_snapshot_ex = 0;


    //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx -> end time = %d, rtn_code %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7883_112_2_18_1_24_14_476,(uint8*)"dd", SCI_GetTickCount(),ret_val);

    return ret_val;

}

LOCAL BOOLEAN                 _DCAMERA_SnapshotExCheckParam(DCAMERA_SNAPSHOT_EX_PARAM_T *p_snapshot_ex)
{
    uint32                i = 0;
    DCAMERA_SIZE_T        *p_dc_size = DC_GetSensorSizeStPtr(DCAMERA_PHOTO_SIZE_3264X2448);

    if(p_snapshot_ex->target_frame_count < 1 ||
       p_snapshot_ex->snapshot_num < 1 ||
       p_snapshot_ex->image_notice_cb == SCI_NULL)
    {
        //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx,target frame count or snapshot number error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7898_112_2_18_1_24_14_477,(uint8*)"");
        return SCI_FALSE;
    }

    for(i = 0; i < p_snapshot_ex->target_frame_count; i++)
    {
        if(p_snapshot_ex->target_info[i].data_type >= DCAMERA_DATA_TYPE_MAX)
        {
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx,data type of %d-th target_info error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7906_112_2_18_1_24_14_478,(uint8*)"d",i);

            return SCI_FALSE;
        }

        if(p_snapshot_ex->target_info[i].target_size.w >= p_dc_size->w ||
           p_snapshot_ex->target_info[i].target_size.h >= p_dc_size->h)
        {
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx,target size exceed the largest size output from sensor"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7914_112_2_18_1_24_14_479,(uint8*)"");
            return SCI_FALSE;
        }

        if((p_snapshot_ex->target_info[i].target_size.w % DCAMERA_PIXEL_ALIGNED != 0) ||
           (p_snapshot_ex->target_info[i].target_size.h % DCAMERA_PIXEL_ALIGNED != 0))
        {
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx,target size must be aligned by 4 pixels"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7921_112_2_18_1_24_14_480,(uint8*)"");
            return SCI_FALSE;
        }

        if(p_snapshot_ex->target_info[i].data_type == DCAMERA_DATA_TYPE_RAW &&
           p_snapshot_ex->target_info[i].rotation_mode != DCAMERA_ROTATION_0)
        {
            //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshotEx,Rotation mode not supported for RAW data"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7928_112_2_18_1_24_14_481,(uint8*)"");
            return SCI_FALSE;

        }
    }

    return SCI_TRUE;
}
//#define SNAPSHOT_EX_DEBUG

#ifdef SNAPSHOT_EX_DEBUG
#define NUM_MAX   8

uint32 captured_num = 0;
DCAMERA_SIZE_T photo_size[NUM_MAX] =
{
        {320,240},
        {480,320},
        {1280,960},
        {1600,1200},
        {2048,1536},
        {1280,768},
        {800,600},
        {800,600}
};
#endif

PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoSnapshotEX(DCAMERA_SNAPSHOT_EX_PARAM_T const*   param_ptr,
                                                   DCAMERA_SNAPSHOT_EX_RETURN_PARAM_T*  return_param_ptr)
{
    DCAMERA_RETURN_VALUE_E ret_val = DCAMERA_OP_SUCCESS;

    if(param_ptr == SCI_NULL)
    {
        return DCAMERA_OP_PARAM_ERR;
    }

    if(SCI_FALSE == _DCAMERA_SnapshotExCheckParam((DCAMERA_SNAPSHOT_EX_PARAM_T*)param_ptr))
    {
        //DCAMERA_TRACE:"DC: DCAMERA_DoSnapshotEX,YUV420 slice scaling is not supported"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_7967_112_2_18_1_24_15_482,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }


    SCI_MEMCPY((void*)&s_snapshot_ex_param, (void*)param_ptr, sizeof(DCAMERA_SNAPSHOT_EX_PARAM_T));
    SCI_MEMSET((void*)&s_snapshot_ex_rtn_param, 0, sizeof(DCAMERA_SNAPSHOT_EX_RETURN_PARAM_T));
#ifdef SNAPSHOT_EX_DEBUG
    s_snapshot_ex_param.target_info[0].target_size.w = photo_size[captured_num%NUM_MAX].w;
    s_snapshot_ex_param.target_info[0].target_size.h = photo_size[captured_num%NUM_MAX].h;
    if(4 == (uint32)(captured_num%NUM_MAX) ||
        6 == (uint32)(captured_num%NUM_MAX))
    {
         s_snapshot_ex_param.target_info[0].rotation_mode = DCAMERA_ROTATION_90;
    }
    captured_num ++;
#endif
    ret_val = DCAMERA_SendSignal(DCAMERA_TASK_CMD_SNAPSHOT_EX,
                                  0,
                                  (void *)&s_snapshot_ex_param,
                                  (void *)&s_snapshot_ex_rtn_param);
    return ret_val;
}

/******************************************************************************/
// Description: snapshot multi images
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoBurstSnapshot
    (
        DCAMERA_BURST_SNAPSHOT_PARAM_T const  * param_ptr,
        DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T * return_param_ptr
    )
{
    DCAMERA_RETURN_VALUE_E ret_val = DCAMERA_OP_SUCCESS;

#if defined(DC_EX_DEBUG)
    DCAMERA_SNAPSHOT_EX_PARAM_T         snapshot_ex = {0};

    jpg_length = 0;
    ex_snapshot_callback = param_ptr->burst_snapshot_callback;
    snapshot_ex.target_info[0].target_size.w = 352;
    snapshot_ex.target_info[0].target_size.h = 288;
    snapshot_ex.target_info[0].data_type = DCAMERA_DATA_TYPE_RGB565;//DCAMERA_DATA_TYPE_YUV422;
    snapshot_ex.target_info[0].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;
    snapshot_ex.app_data_ptr = (void*)0x12345678;

    snapshot_ex.target_info[1].target_size.w = 640;
    snapshot_ex.target_info[1].target_size.h = 480;
    snapshot_ex.target_info[1].data_type = DCAMERA_DATA_TYPE_JPEG;//DCAMERA_DATA_TYPE_YUV422;
    snapshot_ex.target_info[1].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;

    snapshot_ex.enc_quality_level = 6;
    snapshot_ex.flash_enable = 0;
    snapshot_ex.interval_of_frame = param_ptr->interval_of_frame;
    snapshot_ex.target_frame_count = 2;
    snapshot_ex.quickview_callback = SCI_NULL;
    snapshot_ex.snapshot_num = param_ptr->image_count;
    snapshot_ex.b_exif_needed = 1;
    snapshot_ex.image_notice_cb = image_notice_callback;
    ret_val = DCAMERA_DoSnapshotEX(&snapshot_ex, SCI_NULL);

#else

    if(DCAMERA_IsSupport2MSnapshot())
    {
        _DCAMERA_HandleSensorOutputSize(s_dc_info_ptr->jpeg_rect);
    }

    if(PNULL == param_ptr->burst_snapshot_callback)
    {
        ret_val = _DCAMERA_DoBurstSnapshot((DCAMERA_BURST_SNAPSHOT_PARAM_T*)param_ptr, return_param_ptr);
    }
    else
    {
        if(SCI_INVALID_BLOCK_ID != s_dc_task_id)
            //if(SCI_SUCESS == SCI_IsThreadExist(s_dc_task_id))
                _DCAMERA_SnapShotBurstSendSignal(param_ptr);
    }

#endif
    return ret_val;

}



/******************************************************************************/
// Description: Digital camera parameter control
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ParamCtrl
    (
        DCAMERA_PARAM_CTRL_CMD_E    param_cmd,
        uint32                      arg,
        DCAMERA_PARAM_CTRL_CALL_BACK        callback
    )
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    if(!DCAMERA_IsOpen() )
    {
        //DCAMERA_TRACE:"DCAMERA_ParamCtrl: dc is not open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8069_112_2_18_1_24_15_483,(uint8*)"");
        return DCAMERA_OP_NOT_OPEN;
    }
    if(PNULL == s_sensor_ptr)
        return DCAMERA_OP_NOT_OPEN;

    eRet = _DCAMERA_PreviewControl(param_cmd, arg);
    if(callback)
        callback(0,0);
    //_DCAMERA_SnapshotControl(param_cmd, arg);

    return eRet;

}

/******************************************************************************/
// Description: Set auto focus information
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_SetAutoFocus(DCAMERA_AF_INFO_T const * af_info_ptr)
{

    return DCAMERA_OP_SUCCESS;
}

LOCAL void _DCAMERA_getJpgMcuAlign(ISP_IMAGE_FORMAT jpg_src_format, uint32 * p_width_align, uint32 * p_height_align)
{
    switch(jpg_src_format)
    {
        case ISP_IMAGE_FORMAT_YUV422:
            *p_width_align = DCAMERA_JPEG_ALIGN_MID;
            *p_height_align = DCAMERA_JPEG_ALIGN_LOW;
            break;

        case ISP_IMAGE_FORMAT_YUV420:
            *p_width_align = DCAMERA_JPEG_ALIGN_MID;
            *p_height_align = DCAMERA_JPEG_ALIGN_MID;
            break;

        default :
            DCAMERA_TRACE("_DCAMERA_getJpgMcuAlign, unsupport jpeg source format is %d", jpg_src_format);
            break;
    }

    return;
}

/******************************************************************************/
// Description: Review JPEG
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ReviewJpeg(DCAMERA_REVIEW_PARAM_T const * param_ptr)
{

    uint8 *                 jpeg_buf    = PNULL;
    uint32                  jpeg_len    = 0;
    uint16                  width_src   = 0;
    uint16                  height_src  = 0;
    ISP_REVIEW_PARAM_T      review_param= {0};
    ISP_RECT_T              trim_rect   = {0};
    DCAMERA_CALL_BACK       func_ptr    = param_ptr->review_callback;
    JPEG_DEC_IN_PARAM_T     jpeg_dec_in_param   = {0};
    JPEG_DEC_OUT_PARAM_T    jpeg_dec_out_param = {0};
    DCAMERA_CONVERT_COOR_T  src_coor = {0};
    DCAMERA_CONVERT_COOR_T  dst_coor = {0};
    ISP_RECT_T              logic_disp_rect = {0};
    ISP_RECT_T              logic_lcdc_rect = {0};
    uint32                  mem_start = 0;
    uint32                  mem_size = 0;
    uint8                   img_fmt = 0, disp_fmt = 0;
    uint32                  width_align = 0, height_align = 0;
    uint32                  rtn_val = SCI_SUCCESS;
    BOOLEAN                 b_rtn = 0;

    //DCAMERA_TRACE:"DC DCAMERA_ReviewJpeg:start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8125_112_2_18_1_24_15_484,(uint8*)"");

    if(b_review_next_to_snapshot)
    {
        ISP_ServiceReviewBypass();
        //b_review_next_to_snapshot = 0;
        DC_RET_ERR(DCAMERA_OP_SUCCESS)
    }

    if(!DCAMERA_IsOpen())
    {
        DC_RET_ERR(DCAMERA_OP_NOT_OPEN)

    }
    //DCAMERA_TRACE:"DC : DCAMERA_ReviewJpeg -> start time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8139_112_2_18_1_24_15_485,(uint8*)"d", SCI_GetTickCount());

    SCI_ASSERT(PNULL != param_ptr);/*assert verified*/


    // Set lcd parameter
    if(PNULL != param_ptr->jpeg_buffer && NULL != param_ptr->jpeg_len)
    {
        jpeg_buf = param_ptr->jpeg_buffer;
        jpeg_len = param_ptr->jpeg_len;
    }
    else
    {
        DC_RET_ERR(DCAMERA_OP_PARAM_ERR)
    }

    if(s_dc_info_ptr->snapshot_ctl_info.sensor_out_width > s_dc_info_ptr->jpeg_rect.w)
    {
        jpeg_dec_in_param.target_width = s_dc_info_ptr->jpeg_rect.w;
        jpeg_dec_in_param.target_height = s_dc_info_ptr->jpeg_rect.h;
    }
    else
    {
        jpeg_dec_in_param.target_width = s_dc_info_ptr->snapshot_ctl_info.sensor_out_width;
        jpeg_dec_in_param.target_height = s_dc_info_ptr->snapshot_ctl_info.sensor_out_height;
    }

    if(0 == jpeg_dec_in_param.target_width || 0 == jpeg_dec_in_param.target_height)
    {
        DC_RET_ERR(DCAMERA_OP_PARAM_ERR)
    }

    img_fmt = ISP_IMAGE_FORMAT_YUV422;

    //calculate the display rect size

    src_coor.rect = param_ptr->target_rect;
    src_coor.rot_degree = param_ptr->rotation_mode;
    DCAMERA_Convert_Coor(param_ptr->lcd_id, &src_coor, &dst_coor);
    logic_disp_rect.x = (uint32)dst_coor.rect.x;
    logic_disp_rect.y = (uint32)dst_coor.rect.y;
    logic_disp_rect.w = (uint32)dst_coor.rect.w;
    logic_disp_rect.h = (uint32)dst_coor.rect.h;

    //adjust the decode target size to display target size
    if(jpeg_dec_in_param.target_width*(uint32)dst_coor.rect.h > jpeg_dec_in_param.target_height*(uint32)dst_coor.rect.w)
    {
        //target_width is more wider, use the height to judge, and get the lower size for review
        if(jpeg_dec_in_param.target_height > (uint32)dst_coor.rect.h)
        {
            jpeg_dec_in_param.target_width = jpeg_dec_in_param.target_width * (uint32)dst_coor.rect.h / jpeg_dec_in_param.target_height;
            jpeg_dec_in_param.target_width = DCAMERA_WIDTH(jpeg_dec_in_param.target_width);
            jpeg_dec_in_param.target_height = (uint32)dst_coor.rect.h;
        }
    }
    else
    {
        //target_width is more higher, use the width to judge, and get the narrower size for review
        if(jpeg_dec_in_param.target_width > (uint32)dst_coor.rect.w)
        {
            jpeg_dec_in_param.target_height = jpeg_dec_in_param.target_height * (uint32)dst_coor.rect.w / jpeg_dec_in_param.target_width;
            jpeg_dec_in_param.target_height = DCAMERA_HEIGHT(jpeg_dec_in_param.target_height);
            jpeg_dec_in_param.target_width = (uint32)dst_coor.rect.w;
        }
    }

    if(0 == jpeg_dec_in_param.target_width || 0 == jpeg_dec_in_param.target_height)
    {
        DC_RET_ERR(DCAMERA_OP_PARAM_ERR)
    }

    //jpeg source align handle
    _DCAMERA_getJpgMcuAlign((ISP_IMAGE_FORMAT)img_fmt, &width_align, &height_align);
    jpeg_dec_in_param.target_width =  (jpeg_dec_in_param.target_width + width_align - 1)&(~(width_align - 1));
    jpeg_dec_in_param.target_height =  (jpeg_dec_in_param.target_height + height_align - 1)&(~(height_align - 1));

    for(disp_fmt = ISP_IMAGE_FORMAT_YUV422; disp_fmt <= ISP_IMAGE_FORMAT_YUV420; disp_fmt++)
    {
        b_rtn = DCAMERA_Capture2JpegRev(jpeg_dec_in_param.target_width,
                                                              jpeg_dec_in_param.target_height,
                                                              img_fmt,
                                                              disp_fmt,
                                                              logic_disp_rect.w,
                                                              logic_disp_rect.h,
                                                              (uint8)param_ptr->rotation_mode);
        if(b_rtn)
            break;
    }

    if(SCI_FALSE == b_rtn)
    {
        DC_RET_ERR(DCAMERA_OP_NO_ENOUGH_MEMORY)
    }

    if(SNAPSHOT_JPG == s_dc_info_ptr->snapshot_ctl_info.param &&
       jpeg_dec_in_param.target_width > VGA_WIDTH &&
       jpeg_dec_in_param.target_height > VGA_HEIGHT)
    {
        jpeg_dec_in_param.target_width = VGA_WIDTH;
        jpeg_dec_in_param.target_height = VGA_HEIGHT;
    }
    jpeg_dec_in_param.jpeg_buf      = (uint32 *)jpeg_buf;   //  Pointer to the input JFIF
    jpeg_dec_in_param.jpeg_size     = jpeg_len;             //  JFIF length in bytes
    jpeg_dec_in_param.yuv_buf = DCAMERA_CaptureGetYUVBuf(&jpeg_dec_in_param.yuv_size);
    jpeg_dec_in_param.p_jpg_tmp = DCAMERA_CaptureGetJPGTmpBuf(&jpeg_dec_in_param.jpg_tmp_len);

    if(_DCAMERA_CloseISP())
    {
        DC_RET_ERR(DCAMERA_OP_REVIEW_ERR)
    }

    if(JPEG_DEC_OK != JPEG_DecodeJpeg(&jpeg_dec_in_param, &jpeg_dec_out_param ))
    {
        _DCAMERA_OpenISP();
        DC_RET_ERR(DCAMERA_OP_DECODE_ERR)
    }

    if(_DCAMERA_OpenISP())
    {
        DC_RET_ERR(DCAMERA_OP_REVIEW_ERR)
    }

    // size of source image
    width_src  = jpeg_dec_out_param.output_width;
    height_src = jpeg_dec_out_param.output_height;

    if(!(width_src&&height_src))
    {
        DC_RET_ERR(DCAMERA_OP_REVIEW_ERR)
    }

    trim_rect.x = 0;
    trim_rect.y = 0;
    trim_rect.w = width_src;
    trim_rect.h = height_src;

    //second step use isp to scale and trim
    s_dc_info_ptr->isp_frame.yaddr = jpeg_dec_out_param.data_addr.yuv_addr.y_chn_addr;
    s_dc_info_ptr->isp_frame.uaddr = jpeg_dec_out_param.data_addr.yuv_addr.u_chn_addr;
    s_dc_info_ptr->isp_frame.vaddr = jpeg_dec_out_param.data_addr.yuv_addr.v_chn_addr;


    //DCAMERA_TRACE:"DC : ISP_ServiceSetFrameMemory yaddr = 0x%x, uaddr = 0x%x,vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8236_112_2_18_1_24_15_486,(uint8*)"ddd", s_dc_info_ptr->isp_frame.yaddr,s_dc_info_ptr->isp_frame.uaddr,s_dc_info_ptr->isp_frame.vaddr);

    if(jpeg_dec_out_param.output_data_type == JPEG_DEC_DATA_TYPE_YUV420)
    {
        review_param.input_format = ISP_IMAGE_FORMAT_YUV420;
    }
    else if(jpeg_dec_out_param.output_data_type == JPEG_DEC_DATA_TYPE_YUV422)
    {
        review_param.input_format = ISP_IMAGE_FORMAT_YUV422;
    }
    else
    {
        DC_RET_ERR(DCAMERA_OP_REVIEW_ERR)
    }
    review_param.input_size.w = trim_rect.w;
    review_param.input_size.h = trim_rect.h;

    review_param.disp_lcd_id   = param_ptr->lcd_id;
    review_param.disp_block_id = param_ptr->lcd_block_id;

    // set display parameter according to input variable
    src_coor.rect = param_ptr->disp_rect;
    src_coor.rot_degree = param_ptr->rotation_mode;
    DCAMERA_Convert_Coor(param_ptr->lcd_id, &src_coor, &dst_coor);
    logic_lcdc_rect.x = (uint32)dst_coor.rect.x;
    logic_lcdc_rect.y = (uint32)dst_coor.rect.y;
    logic_lcdc_rect.w = (uint32)dst_coor.rect.w;
    logic_lcdc_rect.h = (uint32)dst_coor.rect.h;


    SCI_ASSERT(param_ptr->disp_mode < DCAMERA_DISP_MODE_MAX);/*assert verified*/

    _DCamera_AdjustRect(&trim_rect,
                        &logic_lcdc_rect,
                        &logic_disp_rect,
                        param_ptr->disp_mode);

    //DCAMERA_TRACE:"DC: review JPEG -> trimming x = %d, y = %d, w = %d, h = %d, rotation %d, distortion %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8279_112_2_18_1_24_16_487,(uint8*)"dddddd",trim_rect.x,trim_rect.y,trim_rect.w,trim_rect.h,param_ptr->rotation_mode,param_ptr->disp_mode);

    // convert to phisical coordinate

    _DCAMERA_ConvertCoorToISPRect(param_ptr->lcd_id,
                                  &logic_lcdc_rect,
                                  &review_param.lcd_rect,
                                  param_ptr->rotation_mode);

    _DCAMERA_ConvertCoorToISPRect(param_ptr->lcd_id,
                                  &logic_disp_rect,
                                  &review_param.disp_range,
                                  param_ptr->rotation_mode);


    review_param.input_range.x = trim_rect.x;
    review_param.input_range.y = trim_rect.y;
    review_param.input_range.w = trim_rect.w;
    review_param.input_range.h = trim_rect.h;

    review_param.input_addr.yaddr = s_dc_info_ptr->isp_frame.yaddr;
    review_param.input_addr.uaddr = s_dc_info_ptr->isp_frame.uaddr;
    review_param.input_addr.vaddr = s_dc_info_ptr->isp_frame.vaddr;
    review_param.disp_rotation    = (ISP_ROTATION_E)param_ptr->rotation_mode;
//    review_param.disp_rotation    = (ISP_ROTATION_E)s_dc_info_ptr->preview_ctl_info.rotation_mode;


    mem_start = (uint32)DCAMERA_CaptureGetQuickViewBuf(&mem_size);
    ISP_ServiceMemConfig((void*)mem_start, mem_size);

    rtn_val = ISP_ServiceSetReviewParam(&review_param);
    if(SCI_ERROR == rtn_val)
    {
        DC_RET_ERR(DCAMERA_OP_PARAM_ERR)
    }
    else if(ISP_ERR_CODE_NO_MEM == rtn_val)
    {
        DC_RET_ERR(DCAMERA_OP_NO_ENOUGH_MEMORY)
    }

    ISP_ServiceStartReview();

    if(PNULL != param_ptr->review_callback)
    {
        (param_ptr->review_callback)(DCAMERA_OP_SUCCESS, 0, 0);
    }

    //DCAMERA_TRACE:"DC : DCAMERA_ReviewJpeg -> end time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8326_112_2_18_1_24_16_488,(uint8*)"d", SCI_GetTickCount());
    return DCAMERA_OP_SUCCESS;
}



/******************************************************************************/
// Description: record start
// Author:     Feifei.Gu
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartRecord ( DCAMERA_RECORD_PARAM_T * param_ptr )
{

    ISP_MPEG_PARAM_T            param        = {0};
    DCAMERA_INFO_T              *dc_info_ptr = s_dc_info_ptr;
    DCAMERA_RETURN_VALUE_E      ret_val = DCAMERA_OP_SUCCESS;
    uint32                      isp_status   = 0;
    uint32                      count        = 0;

    //DCAMERA_TRACE:"DCAMERA_StartRecord: Start time %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8348_112_2_18_1_24_16_489,(uint8*)"d", SCI_GetTickCount());

    TM_SendTestPointRequest(0xfffd,SCI_GetTickCount());

    SCI_ASSERT(NULL != param_ptr);/*assert verified*/

    if(!DCAMERA_IsOpen() )
    {
        //DCAMERA_TRACE:"DCAMERA_ParamCtrl: dc is not open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8356_112_2_18_1_24_16_490,(uint8*)"");
        return DCAMERA_OP_NOT_OPEN;
    }
    if(PNULL == s_sensor_ptr)
    {
        return DCAMERA_OP_NOT_OPEN;
    }

    if(_DCAMERA_IsRecord())
    {

        return DCAMERA_OP_SUCCESS;
    }

    LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);
    SCI_TRACE_LOW("DC:StartRecord NO_PERMIT_GUI_INVALIDATE");

    TM_SendTestPointRequest(0xffff,0);
    Sensor_SetMode(SENSOR_MODE_PREVIEW_ONE);
    DCAMERA_SetCapParam(&param.cap_param);

    param.disp_block_id = 0;        //no use but should write correct


    param.disp_lcd_id   = ISP_DISPLAY_NONE;

    param.disp_range.x  = 0;
    param.disp_range.y  = 0;
    param.disp_range.w  = param_ptr->target_size.w;
    param.disp_range.h  = param_ptr->target_size.h;


    param.encode_size.w = param_ptr->target_size.w;
    param.encode_size.h = param_ptr->target_size.h;

    param.input_range.x = 0;
    param.input_range.y = 0;
    param.input_range.w = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].width;//640;
    param.input_range.h = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].height;//480;

    param.input_size.w  = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].width;//640;
    param.input_size.h  = s_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].height;//480;
    param.is_display    = 0;

    param.encode_rotation = (ISP_ROTATION_E)param_ptr->rotation_mode;

    dc_info_ptr->preview_ctl_info.video_mode = DCAMERA_PARAM_INVALID_VALUE;

    if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_PREVIEW_MODE, dc_info_ptr->preview_ctl_info.enviroment_mode))
    {
        //_DCAMERA_SetPreviewMode(preview_ctl_info_ptr->enviroment_mode);
        if(SENSOR_IMAGE_FORMAT_RAW == _DCAMERA_GetSensorImageFormat())
        {
        }
        else if(SENSOR_IMAGE_FORMAT_YUV422 == _DCAMERA_GetSensorImageFormat())
        {
            Sensor_Ioctl(SENSOR_IOCTL_PREVIEWMODE, dc_info_ptr->preview_ctl_info.enviroment_mode);

        }

    }


    if(DCAMERA_WB_MODE_MAX > dc_info_ptr->preview_ctl_info.wb_mode)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_EFFECT, dc_info_ptr->preview_ctl_info.wb_mode))
        {
            //_DCAMERA_SetPreviewWBMode(preview_ctl_info_ptr->wb_mode);
            Sensor_Ioctl(SENSOR_IOCTL_SET_WB_MODE, dc_info_ptr->preview_ctl_info.wb_mode);

        }
    }
    // Sensor Step 7: set brightness,contrast, sharpness, saturation and effect type
    if(DCAMERA_PARAM_INVALID_VALUE != dc_info_ptr->preview_ctl_info.brightness_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_BRIGHTNESS, dc_info_ptr->preview_ctl_info.brightness_level))
        {
            //_DCAMERA_SetPreviewBrightness(preview_ctl_info_ptr->brightness_level);
            Sensor_Ioctl( SENSOR_IOCTL_BRIGHTNESS, dc_info_ptr->preview_ctl_info.brightness_level);

        }
    }

    if(DCAMERA_PARAM_INVALID_VALUE != dc_info_ptr->preview_ctl_info.contrast_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_CONTRAST, dc_info_ptr->preview_ctl_info.contrast_level))
        {
            //_DCAMERA_SetPreviewContrast(preview_ctl_info_ptr->contrast_level);
            Sensor_Ioctl( SENSOR_IOCTL_CONTRAST, dc_info_ptr->preview_ctl_info.contrast_level);

        }
    }

    if(DCAMERA_PARAM_INVALID_VALUE != dc_info_ptr->preview_ctl_info.sharpness_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_SHARPNESS, dc_info_ptr->preview_ctl_info.sharpness_level))
        {
            //_DCAMERA_SetPreviewSharpness(preview_ctl_info_ptr->sharpness_level);
            Sensor_Ioctl( SENSOR_IOCTL_SHARPNESS, dc_info_ptr->preview_ctl_info.sharpness_level);
        }
    }

    if(DCAMERA_PARAM_INVALID_VALUE != dc_info_ptr->preview_ctl_info.saturation_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_SATURATION, dc_info_ptr->preview_ctl_info.saturation_level))
        {
            //_DCAMERA_SetPreviewSaturation(preview_ctl_info_ptr->saturation_level);
            Sensor_Ioctl( SENSOR_IOCTL_SATURATION, dc_info_ptr->preview_ctl_info.saturation_level);

        }
    }

    if(DCAMERA_EFFECT_MAX > dc_info_ptr->preview_ctl_info.effect_type)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_EFFECT, dc_info_ptr->preview_ctl_info.effect_type))
        {
            //_DCAMERA_SetPreviewEffect(preview_ctl_info_ptr->effect_type);
            Sensor_Ioctl( SENSOR_IOCTL_IMAGE_EFFECT, dc_info_ptr->preview_ctl_info.effect_type );
        }
    }

    if(DCAMERA_FLICKER_MAX > dc_info_ptr->preview_ctl_info.flicker_mode)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION, dc_info_ptr->preview_ctl_info.flicker_mode))
        {
            Sensor_Ioctl( SENSOR_IOCTL_ANTI_BANDING_FLICKER, dc_info_ptr->preview_ctl_info.flicker_mode);
        }
    }

    if(DCAMERA_MODE_MAX > dc_info_ptr->preview_ctl_info.video_mode)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_VIDEO_MODE, dc_info_ptr->preview_ctl_info.video_mode))
        {
            Sensor_Ioctl( SENSOR_IOCTL_VIDEO_MODE, dc_info_ptr->preview_ctl_info.video_mode );
        }
    }

    if(DCAMERA_PARAM_INVALID_VALUE != dc_info_ptr->preview_ctl_info.exposure_comp_level)
    {
        if(_DCAMERA_CheckPreviewParam(DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION, dc_info_ptr->preview_ctl_info.exposure_comp_level))
        {
            //_DCAMERA_SetPreviewEffect(preview_ctl_info_ptr->effect_type);
            Sensor_Ioctl( SENSOR_IOCTL_EXPOSURE_COMPENSATION, dc_info_ptr->preview_ctl_info.exposure_comp_level );
        }
    }

    TM_SendTestPointRequest(0xffff,1);

    while(count < DCAMERA_CAMERA_TRY_COUNT)
    {
        count++;

        if((s_sensor_ptr->source_width_max > 1600) && (SENSOR_IMAGE_FORMAT_RAW == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format))
        {
            ISP_ServiceSetClk(96);
        }

        s_dc_record_callback = param_ptr->record_callback;

        //SCI_MEMCPY(&(dc_info_ptr->record_param),  param_ptr, sizeof(DCAMERA_RECORD_PARAM_T));

        record_param.target_size.h= param_ptr->target_size.h;
        record_param.target_size.w = param_ptr->target_size.w;
        record_param.enc_quality_level =  param_ptr->enc_quality_level;


        ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE,_DCAMERA_RecordCallback );

        ISP_ServiceSetMpegCaptureParam(&param);

        isp_status = ISP_ServiceStartCaptureMpeg();
        if(SCI_SUCCESS == isp_status)
        {
            LCD_SetInvalidateLimit(MAIN_LCD_ID,PERMIT_GUI_INVALIDATE);
            ret_val = DCAMERA_OP_SUCCESS;
            _DCAMERA_SetRecordFlag(SCI_TRUE);
            break;
        }
        else
        {
            ret_val = DCAMERA_OP_ISP_ERR;

            s_dc_record_callback = PNULL;
            ISP_ServiceStopCapture();
            _DCAMERA_CloseSensor();
            // RE init sensor
            if(count < DCAMERA_CAMERA_TRY_COUNT)
            {
                ret_val = _DCAMERA_InitSensor();
                if(ret_val != DCAMERA_OP_SUCCESS)
                {
                    LCD_SetInvalidateLimit(MAIN_LCD_ID,PERMIT_GUI_INVALIDATE);

                    return ret_val;
                }
            }

            TM_SendTestPointRequest(0xfffe,ret_val);


        }
    }

    if(DCAMERA_OP_SUCCESS == ret_val)
    {
        s_upcc_dcam_ptr = (uint8*)SCI_ALLOCA(DCAMERA_UPCC_JPEG_SIZE);
        if(PNULL == s_upcc_dcam_ptr)
        {
            ret_val = DCAMERA_OP_MEMORY_ERR;
        }
    }

    //DCAMERA_TRACE:"DCAMERA_StartRecord: End time %d, return value %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8564_112_2_18_1_24_16_491,(uint8*)"dd", SCI_GetTickCount(), ret_val);
    TM_SendTestPointRequest(0xfffd,SCI_GetTickCount());
    TM_SendTestPointRequest(0xfffc,ret_val);
    return ret_val;
}

/******************************************************************************/
// Description: record stop
// Author:     Feifei.Gu
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopRecord ( void )
{
    //DCAMERA_TRACE:"DC: DCAMERA_StopRecord"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8579_112_2_18_1_24_16_492,(uint8*)"");

    if(!DCAMERA_IsOpen())
    {
        return DCAMERA_OP_NOT_OPEN;
    }

    if(_DCAMERA_IsRecord())
    {

        _DCAMERA_SetRecordFlag(SCI_FALSE);

        ISP_ServiceStopCapture();

        _DCAMERA_CloseSensor();
    }

    _DCAMERA_Get_UPCC_CTRL();  //proect critical code
    s_dc_record_callback = PNULL;
    if(PNULL != s_upcc_dcam_ptr)
    {
        SCI_FREE(s_upcc_dcam_ptr);
        s_upcc_dcam_ptr = PNULL;
    }

    _DCAMERA_Release_UPCC_CTRL();

    return DCAMERA_OP_SUCCESS;
}

PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_AdjustDisplayRect(uint32      lcd_id,
                                                        ISP_RECT_T* p_src_rect,
                                                        ISP_RECT_T* p_inva_rect, //the rect to be invalidte
                                                        ISP_RECT_T* p_img_rect,  //the rect to put image
                                                        uint32      display_mode,
                                                        uint32      rot_angle)
{
    DCAMERA_CONVERT_COOR_T  src_coor = {0};
    DCAMERA_CONVERT_COOR_T  dst_coor = {0};
    ISP_RECT_T              logic_disp_rect = {0};
    ISP_RECT_T              logic_lcdc_rect = {0};

    src_coor.rect.x = p_img_rect->x;
    src_coor.rect.y = p_img_rect->y;
    src_coor.rect.w = p_img_rect->w;
    src_coor.rect.h = p_img_rect->h;
    src_coor.rot_degree = rot_angle;
    DCAMERA_Convert_Coor(lcd_id, &src_coor, &dst_coor);
    logic_disp_rect.x = (uint32)dst_coor.rect.x;
    logic_disp_rect.y = (uint32)dst_coor.rect.y;
    logic_disp_rect.w = (uint32)dst_coor.rect.w;
    logic_disp_rect.h = (uint32)dst_coor.rect.h;

    src_coor.rect.x = p_inva_rect->x;
    src_coor.rect.y = p_inva_rect->y;
    src_coor.rect.w = p_inva_rect->w;
    src_coor.rect.h = p_inva_rect->h;
    src_coor.rot_degree = rot_angle;
    DCAMERA_Convert_Coor(lcd_id, &src_coor, &dst_coor);
    logic_lcdc_rect.x = (uint32)dst_coor.rect.x;
    logic_lcdc_rect.y = (uint32)dst_coor.rect.y;
    logic_lcdc_rect.w = (uint32)dst_coor.rect.w;
    logic_lcdc_rect.h = (uint32)dst_coor.rect.h;


    _DCamera_AdjustRect(p_src_rect,
                        &logic_lcdc_rect,
                        &logic_disp_rect,
                        display_mode);


    _DCAMERA_ConvertCoorToISPRect(lcd_id,
                                  &logic_lcdc_rect,
                                  p_inva_rect,
                                  rot_angle);

    _DCAMERA_ConvertCoorToISPRect(lcd_id,
                                  &logic_disp_rect,
                                  p_img_rect,
                                  rot_angle);

    return DCAMERA_OP_SUCCESS;

}


PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartVideoPhone(DCAMERA_PREVIEW_PARAM_T const *preview_param_ptr,
                                                                 DCAMERA_RECORD_PARAM_T * capture_param_ptr)
{
//  ISP_SetLowMemory(TRUE);
        LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
    _DCAMERA_SetVideoPhoneFlag(SCI_TRUE);
    return DCAMERA_OP_SUCCESS;
}
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopVideoPhone( void )
{
//  ISP_SetLowMemory(FALSE);
    _DCAMERA_SetVideoPhoneFlag(SCI_FALSE);
    return DCAMERA_OP_SUCCESS;
}

PUBLIC uint32 DC_GetCurImgSensorId(void)
{
    return s_sensor_id;
}


PUBLIC uint32 DC_SetCurImgSensorId(uint32 sensor_id)
{
    uint32                  rtn = SENSOR_OP_SUCCESS;
    DCAMERA_RETURN_VALUE_E  ret_value = DCAMERA_OP_SUCCESS;
    uint32                  mem_start = 0, mem_size = 0;

    //DCAMERA_TRACE:"DC_SetCurImgSensorId, sensor_id %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8692_112_2_18_1_24_17_493,(uint8*)"d",sensor_id);

    if(sensor_id == s_sensor_id)
        return DCAMERA_OP_SUCCESS;

    if(sensor_id >= SENSOR_ID_MAX)
    {
        sensor_id = SENSOR_MAIN;
    }

    if(DCAMERA_IsOpen())
    {
        if(_DCAMERA_IsPreview() ||
           _DCAMERA_IsPreviewEx() ||
           _DCAMERA_IsRecord()  ||
           _DCAMERA_IsVideoPhone())
        {
            return DCAMERA_OP_ERROR;
        }

        _DCAMERA_ContextDeInit();

        LCD_SetDisplayBuffer(0,1,SCI_NULL);

        DCamera_MemFree();

        _DCAMERA_ContextInit();

        rtn = Sensor_SetCurId((SENSOR_ID_E)sensor_id);

        if(SENSOR_OP_SUCCESS != rtn)
            return DCAMERA_OP_NO_SENSOR_ERR;

        s_sensor_ptr = Sensor_GetInfo();

        mem_start = DCamera_MemConfig(s_operation_mode, &mem_size);
        if(0 == mem_start || 0 == mem_size)
        {
            //DCAMERA_TRACE:"DC: DCAMERA_Open, memory error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8730_112_2_18_1_24_17_494,(uint8*)"");
            return DCAMERA_OP_NO_ENOUGH_MEMORY;
        }

        if(s_operation_mode == DCAMERA_OP_DV ||
           s_operation_mode == DCAMERA_OP_VT||
           s_operation_mode == DCAMERA_OP_MINIDV||
           s_operation_mode == DCAMERA_OP_UPCC)
        {
            ISP_ServiceMemConfig((void*)mem_start, mem_size);
        }

        ret_value = _DCAMERA_GetCapability(s_operation_mode);//must be called after Sensor has been initialized
        if(DCAMERA_OP_SUCCESS != ret_value)
        {
            return ret_value;
        }
    }
    else
    {
        rtn = Sensor_SetCurId((SENSOR_ID_E)sensor_id);
        if(SENSOR_OP_SUCCESS != rtn)
            return DCAMERA_OP_NO_SENSOR_ERR;
    }

    s_sensor_id = sensor_id;
    return DCAMERA_OP_SUCCESS;

}

int DC_SetOsdEnableMode(int flag)
{
    return SCI_SUCCESS;
}

int DC_SetOsdImage(int i_zone, const uint8 *p_data)
{
    return SCI_SUCCESS;
}

/*
int DC_SetOsdZoneMode(int i_zone, const DC_OSD_MODE *pst_mode)
{
    return SCI_SUCCESS;
}
*/
int DC_SetOsdColorPalette(const uint32 *p_table)
{

    return SCI_SUCCESS;
}

int DC_ClearOSDMenu(uint16 chroma_color)
{
    return SCI_SUCCESS;
}

int DC_SetFrameImage(uint8* pFrame, uint32 nSize, uint16 chroma_color)
{
    return SCI_SUCCESS;
}


PUBLIC uint32 *LCDC_GetBlockBuffer(LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32 *buf_ptr = PNULL;

    buf_ptr =   LCD_GetBlockBuffer(lcd_id,blk_num);

    return buf_ptr;
}

PUBLIC ERR_LCD_E LCDC_ConfigBlock(
                                  LCD_ID_E lcd_id,
                                  uint32 blk_num,
                                  BLOCKCFG_T config
                                )
{
    ERR_LCD_E ret;

    ret = LCD_ConfigBlock(lcd_id,blk_num, config);

    return ret;
}

PUBLIC ERR_LCD_E LCDC_EnableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
    LCD_EnableBlock(lcd_id,blk_num);
    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_DisableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
    LCD_DisableBlock(lcd_id, blk_num);
    return ERR_LCD_NONE;
}


PUBLIC void* DCAMERA_GetMemptr(void)
{
    return SCI_NULL;
}

LOCAL void _DCAMERA_BackupPreviewbuf2ReviewBuf(uint32 lcd_id, uint32 block_id)
{
    uint32          disp_buf_size = 0;
    BLOCKCFG_T      blk_info = {0};
    uint32          aligned_addr = 0;
    uint32          base_addr = 0;
    uint32          block_buf = 0;
    uint32          quick_view_size = 0;
	uint32          dc_mem_start = DCAMERA_PreviewExGetBuf(&disp_buf_size);
    uint8           rev_fmt = LCD_RESOLUTION_YUV422;

    if(lcd_id >= LCD_SUM|| block_id >= BLOCK_SUM)
    {
        DCAMERA_TRACE("DC:ReviewBuf error!");
        return;
    }

    if(s_dc_info_ptr->snapshot_ctl_info.param == SNAPSHOT_YUV420)
    {
        rev_fmt = LCD_RESOLUTION_YUV420;
    }

    LCD_GetLcdCtrlSemaphore(0);

    base_addr = (uint32)DCAMERA_CaptureGetQuickViewBuf(&quick_view_size);

    if(0 == base_addr ||0 == quick_view_size)
    {
        DCAMERA_TRACE("DC:ReviewBuf view buffer error");
        LCD_PutLcdCtrlSemaphore(0);
        return;
    }

    block_buf = (uint32)LCD_GetBlockBuffer(lcd_id, block_id);
    if (block_buf < dc_mem_start || block_buf > base_addr)
    {
        DCAMERA_TRACE("DC:ReviewBuf: block_buf error");
        //DCAMERA_TRACE("block_buf 0x%x, dc_mem_start 0x%x, base_addr 0x%x",block_buf,dc_mem_start,base_addr);
        LCD_PutLcdCtrlSemaphore(0);
        return;
     }

    LCD_GetConfigBlock(lcd_id,block_id,&blk_info);
    //DCAMERA_TRACE:"DC: _DCAMERA_BackupPreviewbuf2ReviewBuf quick_view_size 0x%x, width %d blk_info.end_y %d blk_info.start_y %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8854_112_2_18_1_24_17_495,(uint8*)"dddd",quick_view_size,blk_info.width,blk_info.end_y,blk_info.start_y);
    disp_buf_size = (uint32)((uint32)blk_info.width * (uint32)((uint32)blk_info.end_y - (uint32)blk_info.start_y + 1));
    if(LCD_RESOLUTION_YUV420 == rev_fmt)
    {
        quick_view_size = (quick_view_size * 2) / 3;
    }
    else
    {
        quick_view_size = quick_view_size >> 1;
    }

    //DCAMERA_TRACE:"DC: _DCAMERA_BackupPreviewbuf2ReviewBuf dst 0x%x, length %d, src y 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8868_112_2_18_1_24_17_496,(uint8*)"ddd",base_addr,disp_buf_size,block_buf);
    if(block_buf < base_addr ||
       blk_info.resolution != rev_fmt)
    {
        if(quick_view_size >= disp_buf_size)
        {
            if(blk_info.resolution != rev_fmt) // Preview use YUV422 always, so review must be YUV420 in this case
            {
                ISP_FRAME_T     isp_frm = {0};

                isp_frm.width = blk_info.width;
                isp_frm.height = (uint32)((uint32)blk_info.end_y - (uint32)blk_info.start_y + 1);
                isp_frm.yaddr = block_buf;
                isp_frm.uaddr = (uint32)LCD_GetBlockUVBuffer(lcd_id, block_id);

                if(0 == isp_frm.uaddr)
                {
                    DCAMERA_TRACE("DC:ReviewBuf: uv buffer error");
                    LCD_PutLcdCtrlSemaphore(0);
                    return;
                }

                _DCAMERA_ImageYUV422ToYUV420(&isp_frm);

                aligned_addr = base_addr + disp_buf_size;
                aligned_addr = DC_ADDR(aligned_addr);
                blk_info.resolution = LCD_RESOLUTION_YUV420; //YUV422
                blk_info.endian_switch = 1;
                blk_info.set_endian_flag = 1;
                LCD_ConfigBlock(lcd_id,block_id, blk_info);
                memmove((void*)aligned_addr, (void*)isp_frm.uaddr, (disp_buf_size >> 1));
                MMU_DmaCacheSync(aligned_addr, (disp_buf_size >> 1), DMABUFFER_TO_DEVICE);
                LCD_SetUVBuffer(lcd_id,block_id,(uint32*)aligned_addr);

            }
            else
            {
                uint32          uv_buf = (uint32)LCD_GetBlockUVBuffer(lcd_id, block_id);

                aligned_addr = base_addr + disp_buf_size;
                aligned_addr = DC_ADDR(aligned_addr);
                memmove((void*)aligned_addr, (void*)uv_buf, disp_buf_size);
                MMU_DmaCacheSync(aligned_addr, disp_buf_size, DMABUFFER_TO_DEVICE);
                LCD_SetUVBuffer(lcd_id,block_id,(uint32*)aligned_addr);
            }


            aligned_addr = DC_ADDR(base_addr);
            if(block_id == LCD_LAYER_IMAGE)
            {
                memmove((void*)aligned_addr, (void*)block_buf, disp_buf_size);
                MMU_DmaCacheSync(aligned_addr, disp_buf_size, DMABUFFER_TO_DEVICE);
                LCD_SetBlockBuffer(lcd_id,block_id,(uint32*)aligned_addr);
            }
            else
            {
                memmove((void*)aligned_addr, (void*)block_buf, disp_buf_size << 1);
                MMU_DmaCacheSync(aligned_addr, disp_buf_size << 1, DMABUFFER_TO_DEVICE);
                LCD_SetBlockBuffer(lcd_id,block_id,(uint32*)aligned_addr);
            }
        }
    }

    LCD_PutLcdCtrlSemaphore(0);
    return ;
}

LOCAL DCAMERA_RETURN_VALUE_E    _DCAMERA_CapMemAssign(DCAMERA_SNAPSHOT_PARAM_T *param_ptr)
{
    uint32                  cpt_cnt     = 0, i = 0;
    DCAMERA_SIZE_T          dc_dst_size = {0};
    DCAMERA_RECT_T          dc_rect = {0};
    DCAMERA_MEM_PARAM_T     mem_param_t = {0};
    DC_PRODUCT_CFG_T_PTR    p_dc_cfg = DC_GeProductCfgPtr();
    uint16                  img_act_width = 0;
    uint16                  img_act_height = 0;
//    uint32                  use_src_size = 0;

    if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format &&
       DCAMERA_ROTATION_0 == param_ptr->rotation_mode)
    {
        mem_param_t.sensor_out_width = VGA_WIDTH;
        mem_param_t.sensor_out_height = VGA_HEIGHT;
    }
    else
    {
        mem_param_t.sensor_out_width = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
        mem_param_t.sensor_out_height = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;
    }

    dc_rect.w = mem_param_t.sensor_out_width;
    dc_rect.h = mem_param_t.sensor_out_height;
    dc_dst_size.w = s_dc_info_ptr->jpeg_rect.w;
    dc_dst_size.h = s_dc_info_ptr->jpeg_rect.h;

    _DCAMERA_GetCaptureTrimRect(param_ptr->rotation_mode, &dc_dst_size, &dc_rect);

    mem_param_t.sensor_out_width = dc_rect.w;
    mem_param_t.sensor_out_height = dc_rect.h;

    //DCAMERA_TRACE:"_DCAMERA_CapMemAssign, captured size %d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_8960_112_2_18_1_24_18_497,(uint8*)"dd", mem_param_t.sensor_out_width,mem_param_t.sensor_out_height);

    mem_param_t.image_width = s_dc_info_ptr->jpeg_rect.w;
    mem_param_t.image_height = s_dc_info_ptr->jpeg_rect.h;
    if(DCAMERA_ROTATION_90 == param_ptr->rotation_mode ||
       DCAMERA_ROTATION_270 == param_ptr->rotation_mode)
    {
        img_act_width = mem_param_t.image_height;
        img_act_height = mem_param_t.image_width;
    }
    else
    {
        img_act_width = mem_param_t.image_width;
        img_act_height = mem_param_t.image_height;
    }
    mem_param_t.capture_rot = param_ptr->rotation_mode;
    mem_param_t.review_width = s_dc_info_ptr->isp_preview_param.disp_range.w;
    mem_param_t.review_height = s_dc_info_ptr->isp_preview_param.disp_range.h;
    mem_param_t.review_rot = s_dc_info_ptr->isp_preview_param.disp_rotation;
    if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format &&
       DCAMERA_ROTATION_0 == param_ptr->rotation_mode)
    {
        mem_param_t.is_jpg_sensor = 1;
    }
    else
    {
        mem_param_t.is_jpg_sensor = 0;
    }

#if defined(_DC_EXIF_)
    if(p_dc_cfg->exif_eb)
    {
        mem_param_t.b_add_exif = 1;
    }
    else
    {
        mem_param_t.b_add_exif = 0;
    }
#endif

    if(p_dc_cfg->thumbnail_eb)
    {
        mem_param_t.b_add_thumbnail = 1;
    }
    else
    {
        mem_param_t.b_add_thumbnail = 0;
    }
    mem_param_t.slice_height = DC_SLICE_HEIGHT;

    do
    {
        cpt_cnt = DCAMERA_CaptureMemReq(&mem_param_t);

        if(cpt_cnt)
        {
            if(ISP_IMAGE_FORMAT_YUV422 == mem_param_t.image_format)
            {
                s_dc_info_ptr->snapshot_ctl_info.param = SNAPSHOT_YUV422;
            }
            else
            {
                s_dc_info_ptr->snapshot_ctl_info.param = SNAPSHOT_YUV420;
            }
            s_dc_info_ptr->snapshot_ctl_info.sensor_out_width = mem_param_t.sensor_out_width;
            s_dc_info_ptr->snapshot_ctl_info.sensor_out_height = mem_param_t.sensor_out_height;

            if(mem_param_t.sensor_out_width*DCAMERA_INTERPOLATION_COEF >= img_act_width &&
               mem_param_t.sensor_out_height*DCAMERA_INTERPOLATION_COEF >= img_act_height)
            {
                return DCAMERA_OP_SUCCESS;
            }
            else
            {
                return DCAMERA_OP_NO_ENOUGH_MEMORY;
            }
        }

        if(s_dc_info_ptr->snapshot_m > SENSOR_MODE_PREVIEW_ONE)
        {
            //DCAMERA_TRACE:"DC: few memory, reduce the output mode of sensor"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9040_112_2_18_1_24_18_498,(uint8*)"");

            s_dc_info_ptr->snapshot_m = s_dc_info_ptr->snapshot_m - 1;

            dc_rect.w = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
            dc_rect.h = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;
            _DCAMERA_GetCaptureTrimRect(param_ptr->rotation_mode, &dc_dst_size, &dc_rect);

            mem_param_t.sensor_out_width = dc_rect.w;
            mem_param_t.sensor_out_height = dc_rect.h;

            if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format)
            {
                mem_param_t.sensor_out_width >>= 1;
                mem_param_t.sensor_out_height >>= 1;
                mem_param_t.is_jpg_sensor = 1;
            }
            else
            {
                mem_param_t.is_jpg_sensor = 0;
            }
        }
        else
        {
            if(s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width > ISP_SCALE_FRAME_MODE_WIDTH_TH)
            {
                mem_param_t.sensor_out_width = (uint16)(ISP_SCALE_FRAME_MODE_WIDTH_TH - i * DCAMERA_ENC_IMAGE_DOWN_STEP);
                mem_param_t.sensor_out_height = (uint16)((uint32)(mem_param_t.sensor_out_width * img_act_height)/(uint32)img_act_width);
                i ++;
            }
            else
            {
                mem_param_t.sensor_out_width  -= DCAMERA_ENC_IMAGE_DOWN_STEP;
                mem_param_t.sensor_out_height -= (uint16)(DCAMERA_ENC_IMAGE_DOWN_STEP * img_act_height / img_act_width);
            }
            mem_param_t.sensor_out_height = DCAMERA_HEIGHT(mem_param_t.sensor_out_height);

        }

    }while(mem_param_t.sensor_out_width*DCAMERA_INTERPOLATION_COEF >= img_act_width &&
           mem_param_t.sensor_out_height*DCAMERA_INTERPOLATION_COEF >= img_act_height);

    return DCAMERA_OP_NO_ENOUGH_MEMORY;
}

LOCAL void _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_MODE_E cap_rot_mode,
                                       DCAMERA_SIZE_T *p_dst_size,
                                       DCAMERA_RECT_T *p_trim_rect)
{
    uint32  zoom_w_step_snap = 0;
    uint32  zoom_h_step_snap = 0;
    uint16  target_w = 0;
    uint16  target_h = 0;
    uint16  width_src = 0;
    uint16  height_src = 0;
    uint16  width_trim  = 0;
    uint16  height_trim = 0;
 /*
    if(DCAMERA_ROTATION_90 ==  cap_rot_mode ||
       DCAMERA_ROTATION_270 ==  cap_rot_mode)
    {
        width_src = p_trim_rect->h;
        height_src = p_trim_rect->w;
    }
    else
    {
        width_src = p_trim_rect->w;
        height_src = p_trim_rect->h;
    }
*/
    width_src = p_trim_rect->w;
    height_src = p_trim_rect->h;

    if(DCAMERA_ROTATION_90 ==  cap_rot_mode ||
       DCAMERA_ROTATION_270 ==  cap_rot_mode)
    {
        target_w = p_dst_size->h;
        target_h = p_dst_size->w;
    }
    else
    {
        target_w = p_dst_size->w;
        target_h = p_dst_size->h;
    }

    if(target_w * height_src < target_h * width_src)
    {
        if((target_h > 0 ) && (p_trim_rect->w > 0))
        {
            p_trim_rect->w = target_w * height_src / target_h;
            p_trim_rect->x = (width_src - p_trim_rect->w) >> 1;
            p_trim_rect->h = height_src;
            p_trim_rect->y = 0;
        }
    }
    else
    {
        if((target_w > 0 ) && (height_src > 0))
        {
            p_trim_rect->h = target_h * width_src / target_w;
            p_trim_rect->y = (height_src - p_trim_rect->h) >> 1;
            p_trim_rect->w = width_src;
            p_trim_rect->x = 0;
        }
    }
    zoom_w_step_snap = ZOOM_STEP(p_trim_rect->w);
    zoom_w_step_snap = zoom_w_step_snap & ~1;
    zoom_h_step_snap = ZOOM_STEP(p_trim_rect->h);
    zoom_h_step_snap = zoom_h_step_snap & ~1;

    //SCI_TRACE_LOW:"_DCAMERA_GetCaptureTrimRect, src {%d %d}, dst {%d %d},trim {%d %d %d %d},zoom_step {%d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9151_112_2_18_1_24_18_499,(uint8*)"dddddddddd",width_src,height_src,target_w,target_h,p_trim_rect->x,p_trim_rect->y,p_trim_rect->w,p_trim_rect->h,zoom_w_step_snap,zoom_h_step_snap);
    width_trim  = zoom_w_step_snap * s_dc_info_ptr->preview_ctl_info.zoom_level;
    height_trim = zoom_h_step_snap * s_dc_info_ptr->preview_ctl_info.zoom_level;

    p_trim_rect->x += width_trim >> 1;
    p_trim_rect->y += height_trim >> 1;

    p_trim_rect->x = p_trim_rect->x & ~1;
    p_trim_rect->y = p_trim_rect->y & ~1;
    p_trim_rect->w = width_src - (p_trim_rect->x << 1);
    p_trim_rect->h = height_src - (p_trim_rect->y << 1);

    p_trim_rect->w = DCAMERA_WIDTH(p_trim_rect->w);
    p_trim_rect->h = DCAMERA_HEIGHT(p_trim_rect->h);
    //SCI_TRACE_LOW:"_DCAMERA_GetCaptureTrimRect,p_trim_rect {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9166_112_2_18_1_24_18_500,(uint8*)"dddd",p_trim_rect->x,p_trim_rect->y,p_trim_rect->w,p_trim_rect->h);

}

LOCAL  ISP_ADDRESS_T    slice_addr = {0};

LOCAL int _DCAMERA_GetScaledData( ISP_ADDRESS_T addr_t, uint32 width, uint32 height)
{
    //DCAMERA_TRACE:"_DCAMERA_GetScaledData, width %d, height %d, y addr 0x%x, uv addr 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9178_112_2_18_1_24_18_501,(uint8*)"dddd",width,height,addr_t.yaddr,addr_t.uaddr);
    slice_addr.yaddr = addr_t.yaddr;
    slice_addr.uaddr = addr_t.uaddr;
    s_slice_out_once = height;
    _DCAMERA_CaptureDoneNotice();
    return 0;
}

LOCAL uint32 _DCAMERA_JpgEncSliceScalingGetSrcHeight(void)
{
    DCAMERA_RECT_T      dc_rect = {0};
    DCAMERA_SIZE_T      dc_dst_size = {0};
    DCAMERA_SIZE_T      dc_size = {0};

    dc_size.w = s_dc_info_ptr->isp_frame.width;
    dc_size.h = s_dc_info_ptr->isp_frame.height;
    dc_rect.w = dc_size.w;
    dc_rect.h = dc_size.h;

    dc_dst_size.w = s_dc_info_ptr->jpeg_rect.w;
    dc_dst_size.h = s_dc_info_ptr->jpeg_rect.h;
    _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);
    return dc_rect.h;
}

/******************************************************************************
// Description: calculate the rect information for the date mark
// Author:     xz
// Input:       DCAMERA_RECT_T trim_rect         //target trim rect of the image
//                DCAMERA_RECT_T *src_date_rect_ptr      //source date mark rect
// Output:     DCAMERA_RECT_T *dst_date_rect_ptr     //dest date mark rect
// Return:     0             successful
//                others      failed
// Note:
******************************************************************************/
LOCAL uint32 _DCAMERA_GetDateRectParam(DCAMERA_RECT_T trim_rect,
                                    DCAMERA_RECT_T *src_date_rect_ptr,
                                    DCAMERA_RECT_T *dst_date_rect_ptr)
{
    uint32  tempSize = 0;
    uint32  tempStart = 0;
    uint32  tempEnd = 0;
    uint32  tempVal = 0;
    //judge if the date rect is out of the trim rect range
#if 1
    //DCAMERA_TRACE:"_DCAMERA_GetDateRectParam: trim rect x:%d, y:%d, w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9222_112_2_18_1_24_18_502,(uint8*)"dddd", trim_rect.x, trim_rect.y, trim_rect.w, trim_rect.h);
    //DCAMERA_TRACE:"_DCAMERA_GetDateRectParam: src_rect x:%d, y:%d, w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9223_112_2_18_1_24_18_503,(uint8*)"dddd", src_date_rect_ptr->x, src_date_rect_ptr->y, src_date_rect_ptr->w, src_date_rect_ptr->h);
#endif

    if((src_date_rect_ptr->x < (trim_rect.x+trim_rect.w-1))&&   \
        (src_date_rect_ptr->y < (trim_rect.y+trim_rect.h-1)))
    {
        //caculate the size
        //x direction cal
        if(src_date_rect_ptr->x >= trim_rect.x)
        {
            tempStart = src_date_rect_ptr->x;
        }
        else
        {
            tempStart = trim_rect.x;
        }

        dst_date_rect_ptr->x = tempStart;

        tempEnd = trim_rect.x + trim_rect.w - 1;
        tempVal = src_date_rect_ptr->x + src_date_rect_ptr->w - 1;
        if(tempEnd > tempVal)
        {
            tempEnd = tempVal;
        }

        tempSize = tempEnd - tempStart+1;

        if(tempSize < src_date_rect_ptr->w)
        {
            dst_date_rect_ptr->w = tempSize;
        }
        else
        {
            dst_date_rect_ptr->w = src_date_rect_ptr->w;
        }

        //y direction cal
        if(src_date_rect_ptr->y >= trim_rect.y)
        {
            tempStart = src_date_rect_ptr->y;
        }
        else
        {
            tempStart = trim_rect.y;
        }

        dst_date_rect_ptr->y = tempStart;

        tempEnd = trim_rect.y + trim_rect.h - 1;
        tempVal = src_date_rect_ptr->y + src_date_rect_ptr->h - 1;
        if(tempEnd > tempVal)
        {
            tempEnd = tempVal;
        }

        tempSize = tempEnd - tempStart+1;
        if(tempSize < src_date_rect_ptr->h)
        {
            dst_date_rect_ptr->h = tempSize;
        }
        else
        {
            dst_date_rect_ptr->h = src_date_rect_ptr->h;
        }
    }
    else
    {
        dst_date_rect_ptr->x = 0;
        dst_date_rect_ptr->y = 0;
        dst_date_rect_ptr->w = 0;
        dst_date_rect_ptr->h = 0;
        //DCAMERA_TRACE:"_DCAMERA_GetDateRectParam: snapshot the data rect out of trim range"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9295_112_2_18_1_24_18_504,(uint8*)"");
        return SCI_ERROR;
    }

#if 1
    //DCAMERA_TRACE:"_DCAMERA_GetDateRectParam: dst_rect x:%d, y:%d, w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9304_112_2_18_1_24_18_505,(uint8*)"dddd", dst_date_rect_ptr->x, dst_date_rect_ptr->y, dst_date_rect_ptr->w, dst_date_rect_ptr->h);
#endif

    return SCI_SUCCESS;
}

/******************************************************************************
// Description: to get a dest date mark buffer of target rect
// Author:     xz
// Input:       DCAMERA_RECT_T *src_date_rect_ptr      //source date mark rect
                   DCAMERA_RECT_T *dst_date_rect_ptr     //dest date mark rect
                   uint32 *src_date_addr_ptr                     //source date mark buffer address
// Output:    uint32 *dst_date_addr_ptr                     //dest date mark buffer address
// Return:     0             successful
//                others      failed
// Note:
******************************************************************************/
LOCAL uint32 _DCAMERA_GetDateRectData(DCAMERA_RECT_T *src_rect_ptr,
                                    DCAMERA_RECT_T *dst_rect_ptr,
                                    uint32 *src_date_addr,
                                    uint32 *dst_date_addr)
{
    uint32 j = 0;

    uint32 *src_ptr = src_date_addr;
    uint32 *dst_ptr = dst_date_addr;

    uint32 dst_rect_width = dst_rect_ptr->w;
    uint32 dst_rect_h = dst_rect_ptr->h;
    uint32 src_rect_w = src_rect_ptr->w;

    //check the parameter
    if((dst_rect_ptr->y < src_rect_ptr->y)||(dst_rect_ptr->x - src_rect_ptr->x)||
        ((dst_rect_ptr->y + dst_rect_ptr->h) > (src_rect_ptr->y + src_rect_ptr->h))||
        ((dst_rect_ptr->x + dst_rect_ptr->w) > (src_rect_ptr->x + src_rect_ptr->w)))
    {
        //DCAMERA_TRACE:"_DCAMERA_getDateRectData, date mark rect info is not valid!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9339_112_2_18_1_24_19_506,(uint8*)"");
        return SCI_FALSE;
    }

    if((PNULL == src_date_addr)||(PNULL == dst_date_addr))
    {
        //DCAMERA_TRACE:"_DCAMERA_getDateRectData, error source or dest date mark address !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9345_112_2_18_1_24_19_507,(uint8*)"");
        return SCI_FALSE;
    }

    src_ptr += (dst_rect_ptr->y - src_rect_ptr->y)*src_rect_ptr->w+(dst_rect_ptr->x - src_rect_ptr->x);
    //get the date rect date in the trim from source
    for(j = 0; j < dst_rect_h; j++)        //column
    {
        SCI_MEMCPY(dst_ptr,src_ptr,(dst_rect_width<<2));   //line
        dst_ptr = dst_ptr + dst_rect_width;
        src_ptr = src_ptr + src_rect_w;
    }

    return SCI_SUCCESS;
}

/******************************************************************************
// Description: to get a dest date mark buffer of target rect
// Author:     xz
// Input:       DCAMERA_RECT_T *target_rect_ptr         //target rect for the date mark be added on
                   DCAMERA_RECT_T  *src_date_rect_ptr     //date mark rect
                   ISP_ADDRESS_T  target_addr                   //target buffer of image
// Output:    uint32 *dst_date_addr_ptr                     //dest date mark buffer address
// Return:     0             successful
//                others      failed
// Note:
******************************************************************************/
LOCAL uint32  _DCAMERA_dateBlend(DCAMERA_RECT_T *target_rect_ptr,
                                                               DCAMERA_RECT_T  *src_date_rect_ptr,
                                                               ISP_ADDRESS_T  target_addr,
                                                               uint32 * src_date_addr)
{
    IMGREF_YUV_SRC param_src  ={0};
    IMGREF_OSD_T capture_date = {0};
    BOOLEAN     is_YUV422 = SCI_TRUE;
    uint32 rtn = SCI_SUCCESS;

    //target buffer
    param_src.src.y_addr = target_addr.yaddr;
    param_src.src.uv_addr = target_addr.uaddr;

    //target rect
    param_src.src_buf_rect.x = 0;
    param_src.src_buf_rect.y = 0;
    param_src.src_buf_rect.w = target_rect_ptr->w;
    param_src.src_buf_rect.h = target_rect_ptr->h;

    //date mark buffer and rect
    param_src.src_size.w = param_src.src_buf_rect.w;
    param_src.src_size.h = param_src.src_buf_rect.h;

    capture_date.color_key = 0;
    capture_date.dataformat = IMGREF_FORMAT_ARGB888;

    capture_date.overlay_rect.x = src_date_rect_ptr->x - target_rect_ptr->x;
    capture_date.overlay_rect.y = src_date_rect_ptr->y - target_rect_ptr->y;
    capture_date.overlay_rect.w = src_date_rect_ptr->w;
    capture_date.overlay_rect.h = src_date_rect_ptr->h;

    capture_date.osd_addr.y_addr= (uint32)src_date_addr;

    //DCAMERA_TRACE:"_DCAMERA_dateBlend: date addr 0x%x, x %d y %dwidth %d, height %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9405_112_2_18_1_24_19_508,(uint8*)"ddddd", capture_date.osd_addr.y_addr ,capture_date.overlay_rect.x, capture_date.overlay_rect.y, capture_date.overlay_rect.w, capture_date.overlay_rect.h);

    if(SNAPSHOT_YUV422 == s_dc_info_ptr->snapshot_ctl_info.param)
    {
        is_YUV422 = SCI_TRUE;
    }
    else if (SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
    {
        is_YUV422 = SCI_FALSE;
    }
    else
    {
        //DCAMERA_TRACE:"_DCAMERA_dateBlend: date addr 0x%x, x %d y %dwidth %d, height %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9417_112_2_18_1_24_19_509,(uint8*)"ddddd", capture_date.osd_addr.y_addr ,capture_date.overlay_rect.x, capture_date.overlay_rect.y, capture_date.overlay_rect.w, capture_date.overlay_rect.h);
        return DCAMERA_OP_PARAM_ERR;
    }
    //blender the date mark to buffer
    rtn = DCAMERA_ArgbOverlayer(&param_src, is_YUV422, &capture_date);

    return rtn;
}

/******************************************************************************
// Description: handle the date mark calculate and blender operation
// Author:     xz
// Input:       DCAMERA_RECT_T target_rect         //target rect for the date mark be added on
                   ISP_ADDRESS_T  target_addr                   //target buffer of image
// Output:    ISP_ADDRESS_T   target_addr                  //target buffer of image
// Return:     0             successful
//                others      failed
// Note:
******************************************************************************/
LOCAL uint32    _DCAMERA_handleDateMark(DCAMERA_RECT_T target_rect,
                                        ISP_ADDRESS_T  target_addr,
                                        DCAMERA_RECT_T src_date_rect,
                                        uint32    * src_date_addr,
                                        BOOLEAN isCbInIsp
                                        )
{
    DCAMERA_RECT_T dst_date_rect = {0};
    uint32    * dst_date_addr = PNULL;
    uint32      rtn =   SCI_SUCCESS;


    //calculate date rect
    rtn = _DCAMERA_GetDateRectParam(target_rect, &src_date_rect, &dst_date_rect);

    if(SCI_SUCCESS != rtn)
    {
        //DCAMERA_TRACE:"_DCAMERA_handleDateMark: the dst date mark rect is not valid!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9452_112_2_18_1_24_19_510,(uint8*)"");
    }
    else
    {
        dst_date_addr = SCI_ALLOCA(dst_date_rect.w*dst_date_rect.h*4);
        if(PNULL == dst_date_addr)
        {
            //DCAMERA_TRACE:"_DCAMERA_handleDateMark: date mark memory alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9459_112_2_18_1_24_19_511,(uint8*)"");
        }
        else
        {
            //copy the image date to the date mark buffer
            rtn = _DCAMERA_GetDateRectData(&src_date_rect, &dst_date_rect, src_date_addr, dst_date_addr);

            if(SCI_SUCCESS != rtn)
            {
                //DCAMERA_TRACE:"_DCAMERA_handleDateMark: date mark data duplicate error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9468_112_2_18_1_24_19_512,(uint8*)"");
            }
            else
            {
                //blender the date to the image
                rtn = _DCAMERA_dateBlend(&target_rect, &dst_date_rect, target_addr, dst_date_addr);
            }
            SCI_FREE(dst_date_addr);
        }
    }

    return rtn;
}

/******************************************************************************
// Description: handle the scaling and date blender operation
// Author:     xz
// Input:       DCAMERA_SIZE_T target_size         //target size for the date mark be added on
                   ISP_ADDRESS_T  target_addr         //target buffer of image
// Output:
// Global:      s_dc_info_ptr
                   s_snapshot_param
// Return:     0             successful
//                others      failed
// Note:
******************************************************************************/
LOCAL uint32 _DCAMERA_handleScaleAndDate(DCAMERA_SIZE_T target_size, ISP_ADDRESS_T  target_addr, BOOLEAN isCbInIsp)
{
    DCAMERA_SIZE_T tmp_jpeg_size = {0};
    SCALE_IMAGE_IN_T date_mark_in = {0};
    SCALE_IMAGE_OUT_T date_mark_out = {0};
    uint32  scale_coeff = 0;
    uint32 temp_val1 = 0, temp_val2=0;
    DCAMERA_RECT_T src_date_rect = {0};
    DCAMERA_RECT_T target_rect = {0};
    uint32    * src_date_addr = PNULL;
    int32   tmp_diff_val = 0;
    uint32 rtn =SCI_SUCCESS;
    DCAMERA_CAPTURE_DATE_T  date_mark = {0};

    //DCAMERA_TRACE:"_DCAMERA_handleScaleAndDate: start calculate the date mark scale coeff!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9507_112_2_18_1_24_19_513,(uint8*)"");

    date_mark = b_burst_snapshot?(s_snapshot_burst_param.capture_date):(s_snapshot_param.capture_date);

    //scale the date mark to fit the review input rect
    tmp_jpeg_size.w = s_dc_info_ptr->jpeg_rect.w;
    tmp_jpeg_size.h = s_dc_info_ptr->jpeg_rect.h;

    //w/h of target campare with w/h of jpeg rect
    temp_val1 = (uint32)(target_size.w)*(uint32)(tmp_jpeg_size.h);
    temp_val2 = (uint32)(target_size.h)*(uint32)(tmp_jpeg_size.w);

    target_rect.w = target_size.w;
    target_rect.h = target_size.h;

    if(temp_val1 >= temp_val2)
    {
        target_rect.x = 0;

        if(target_size.w == tmp_jpeg_size.w)
        {
            scale_coeff = 1<<8;
        }
        else
        {
            scale_coeff = ((uint32)(target_size.w)<<8)/tmp_jpeg_size.w;
        }

        tmp_diff_val = ((((int32)(tmp_jpeg_size.h)*(int32)scale_coeff)>>8)- (int32)target_size.h)/2;
        if(tmp_diff_val<0)
        {
            tmp_diff_val = 0;
        }
        target_rect.y = (uint16)tmp_diff_val;

        target_rect.y = target_rect.y*2/2;  //align to 2
    }
    else
    {
        target_rect.y = 0;

        if(target_size.h == tmp_jpeg_size.h)
        {
            scale_coeff = 1<<8;
        }
        else
        {
            scale_coeff = ((uint32)(target_size.h)<<8)/tmp_jpeg_size.h;
        }

        tmp_diff_val = ((((int32)(tmp_jpeg_size.h)*(int32)scale_coeff)>>8)- (int32)target_size.w)/2;
        if(tmp_diff_val<0)
        {
            tmp_diff_val = 0;
        }

        target_rect.x = (uint16)tmp_diff_val;
        target_rect.x = target_rect.x*4/4;  //align to 4
    }

    //DCAMERA_TRACE:"_DCAMERA_handleScaleAndDate: the scale coeff(256x) is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9567_112_2_18_1_24_19_514,(uint8*)"d", scale_coeff);

    //calculate the scale date parameter
    if(0x100 == scale_coeff)
    {
        src_date_rect = date_mark.overlay_rect;
        src_date_addr = (uint32 *)date_mark.osd_addr.y_addr;
        rtn = _DCAMERA_handleDateMark(target_rect, target_addr, src_date_rect, src_date_addr, isCbInIsp);
    }
    else
    {
        date_mark_in.src_size.w = date_mark.overlay_rect.w;
        date_mark_in.src_size.h = date_mark.overlay_rect.h;
        date_mark_in.src_trim_rect.x = 0;
        date_mark_in.src_trim_rect.y = 0;
        date_mark_in.src_trim_rect.w = date_mark.overlay_rect.w;
        date_mark_in.src_trim_rect.h = date_mark.overlay_rect.h;
        date_mark_in.src_format = IMGREF_FORMAT_ARGB888;
        date_mark_in.src_chn.chn0.ptr = (void *)date_mark.osd_addr.y_addr;
//      date_mark_in.src_chn.chn0.size = (uint32)(date_mark_in.src_size.w) * (uint32)(date_mark_in.src_size.h);
        DCAMERA_GetChnSize(&date_mark_in.src_chn,
                            date_mark_in.src_size.w,
                            date_mark_in.src_size.h,
                            date_mark_in.src_format,
                            0);
        date_mark_in.scale_mode = SCALE_MODE_NO_DISTORT;

        temp_val1 = scale_coeff * date_mark.overlay_rect.w;
        temp_val2 = scale_coeff * date_mark.overlay_rect.h;

        date_mark_in.target_size.w = (uint16)((temp_val1+128)>>8);    //software scale support odd width value size
        date_mark_in.target_size.h = (uint16)((temp_val2+128)>>8);
        date_mark_in.target_format = IMGREF_FORMAT_ARGB888;

        //get the out buffer  size of the date mark
//        temp_val1= (date_mark_in.target_size.w * date_mark_in.target_size.h)<<2;
        DCAMERA_GetChnSize(&date_mark_out.output_chn,
                            date_mark_in.target_size.w,
                            date_mark_in.target_size.h,
                            date_mark_in.target_format,
                            0);

        date_mark_in.target_buf.ptr = (void *)SCI_ALLOC(date_mark_out.output_chn.chn0.size);

       DCAMERA_TRACE(
           "DC:ScaleAndDate src w-%d,h-%d target w-%d,h-%d, buff addr-0x%x, size-%d",
            date_mark_in.src_trim_rect.w,
            date_mark_in.src_trim_rect.h,
            date_mark_in.target_size.w,
            date_mark_in.target_size.h,
            date_mark_in.target_buf.ptr,
            date_mark_out.output_chn.chn0.size
           );

        if(PNULL != date_mark_in.target_buf.ptr)
        {
            date_mark_in.target_buf.size = date_mark_out.output_chn.chn0.size;
            date_mark_out.output_chn.chn0 = date_mark_in.target_buf;
            date_mark_out.output_format = IMGREF_FORMAT_ARGB888;
            date_mark_in.scale_by_software = TRUE;          //use software to scale the date mark

           DCAMERA_TRACE(
                "DC:ScaleAndDate target w -%d h - %d ,input param 0x%x, output result 0x%x",
                target_size.w,
                target_size.h,
                &date_mark_in,
                &date_mark_out
                );

            if(GRAPH_ScaleImage(&date_mark_in, &date_mark_out))
            {
                SCI_FREE(date_mark_in.target_buf.ptr);
                //DCAMERA_TRACE:"_DCAMERA_handleScaleAndDate: date mark scaling failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9636_112_2_18_1_24_19_515,(uint8*)"");
                return DCAMERA_OP_ERROR;
            }

            //DCAMERA_TRACE:"_DCAMERA_handleScaleAndDate: date mark scaling successed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9640_112_2_18_1_24_19_516,(uint8*)"");

            src_date_addr = (uint32 *)date_mark_out.output_chn.chn0.ptr;
            //calculate date rect
            src_date_rect.w = date_mark_out.output_size.w;
            src_date_rect.h = date_mark_out.output_size.h;
            temp_val1 = scale_coeff * date_mark.overlay_rect.x;
            temp_val2 = scale_coeff * date_mark.overlay_rect.y;
            src_date_rect.x = (uint16)(temp_val1>>8);
            src_date_rect.y = (uint16)(temp_val2>>8);

            //DCAMERA_TRACE:"_DCAMERA_handleScaleAndDate: start blender the date mark!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9651_112_2_18_1_24_19_517,(uint8*)"");

            rtn = _DCAMERA_handleDateMark(target_rect, target_addr, src_date_rect, src_date_addr, isCbInIsp);

            SCI_FREE(date_mark_in.target_buf.ptr);
        }
        else
        {
            rtn = SCI_ERROR;
        }
    }

    return rtn;
}

/******************************************************************************
// Description: handle the review date blender operation
// Author:     xz
// Input:       DCAMERA_RECT_T target_rect         //target rect for the date mark be added on
                   ISP_ADDRESS_T  target_addr                   //target buffer of image
// Output:
// Global:      s_dc_info_ptr
                   s_snapshot_param
// Return:     0             successful
//                others      failed
// Note:
******************************************************************************/
LOCAL int32 _DCAMERA_reviewDateCallback(void *frame_ptr)
{
    ISP_FRAME_T             *frame_addr_ptr = (ISP_FRAME_T *)frame_ptr;

    DCAMERA_SIZE_T target_size = {0};
    ISP_ADDRESS_T  target_addr = {0};

    SCI_TRACE_LOW("DC: reviewDate");

    //the target image rect need to add date mark
    target_size.w = frame_addr_ptr->width;
    target_size.h = frame_addr_ptr->height;

    //set the target buffer
    target_addr.yaddr = frame_addr_ptr->yaddr;
    target_addr.uaddr = frame_addr_ptr->uaddr;

    _DCAMERA_handleScaleAndDate(target_size, target_addr, SCI_TRUE);

    _DCAMERA_CaptureDoneNotice();

    return ISP_CB_PROC_DONE;
}


#if 0
void YUV422_UYVY_TO_I420_Y_U_V(unsigned char * dst, unsigned char * src,
                               unsigned int width, unsigned int height)
{
	uint32 i = 0;
	uint32 j = 0;
	uint8 *src_y_ptr = src;
	uint8 *dst_y_ptr = NULL;
	uint8 *dst_u_ptr = NULL;
	uint8 *dst_v_ptr = NULL;

	dst_y_ptr = dst;
	dst_u_ptr = dst+ width * height + width * height / 4;
	dst_v_ptr = dst + width * height;

	for (i = 0; i<height; i++)
	{
		if (i % 2 == 0)
		{
			for (j = 0; j < width; j += 2)
			{
				*dst_u_ptr++ = *src_y_ptr++;
				*dst_y_ptr++ = *src_y_ptr++;
				*dst_v_ptr++ = *src_y_ptr++;
				*dst_y_ptr++ = *src_y_ptr++;
			}
		}
		else
		{
			for (j = 0; j < width; j += 2)
			{
				*src_y_ptr++;
				*dst_y_ptr++ = *src_y_ptr++;
				*src_y_ptr++;
				*dst_y_ptr++ = *src_y_ptr++;
			}
		}
	}
}


void I420_Y_U_V_Rotation_90(unsigned char * dst, unsigned char * src,
                            unsigned int width, unsigned int height)
{
	int halfWidth = width / 2;
	int yFinalLineStartIndex = (height - 1) * width;
	int rotatedYIndex = 0;

	//rotate y
	for (int q = 0; q < width; q++)
    {
		for (int p = height - 1; p >= 0; p--)
        {
			dst[rotatedYIndex++] = src[width * p + q];
		}
	}

    //rotate uv
	int uFinalLineStartIndex = width * height * 5 / 4 - halfWidth;
	int vFinalLineStartIndex = width * height * 3 / 2 - halfWidth;
	int rotatedUIndex = width * height;
	int rotatedVIndex = width * height * 5 / 4;

	for (int w = 0; w < width; w += 2)
    {
		int uStartIndex = uFinalLineStartIndex + w / 2;
		int vStartIndex = vFinalLineStartIndex + w / 2;
		int offset = 0;
		for (int h = 0; h < height; h += 2)
        {
			dst[rotatedUIndex++] = src[uStartIndex - offset];
			dst[rotatedVIndex++] = src[vStartIndex - offset];
			offset += halfWidth;
		}
	}
}
#endif
#if 0
static unsigned char*g_JpegTmpBuf;/*lint !e843*/
static unsigned char*g_JpegBuf;


LOCAL int32 _DCAMERA_EncodeCallback(void *frame_ptr)
{
    ISP_FRAME_T             *frame_addr_ptr = (ISP_FRAME_T *)frame_ptr;
    JPEG_ENC_RET_E          jpeg_ret =  JPEG_ENC_OK;
    uint32                  size;
    JPEG_ENC_IN_PARAM_T     in_param = {0};
    JPEG_ENC_OUT_PARAM_T    out_param = {0};

    /*
    if(!g_JpegTmpBuf)
    {
        g_JpegTmpBuf = SCI_ALLOCA(200*200*3/2);
        if(!g_JpegTmpBuf)
        {
            SCI_TRACE_LOW(" david alloc g_JpegTmpBuf error!");
            return -1
        }
    }*/

    if(!g_JpegBuf)/*lint !e774*/
    {
        g_JpegBuf = SCI_ALLOCA(30*1024);
        if(!g_JpegBuf)
        {
            SCI_TRACE_LOW(" david alloc g_JpegTmpBuf error!");
            return -1;
        }
    }

    _DCAMERA_Get_UPCC_CTRL();

    //UYVY -> Y_U_V 420 Rotate 90
    //YUV422_UYVY_TO_I420_Y_U_V(g_JpegTmpBuf, (uint8*)frame_addr_ptr->yaddr, 200, 200)
    //I420_Y_U_V_Rotation_90((uint8*)frame_addr_ptr->yaddr, g_JpegTmpBuf,  200, 200)

    in_param.image_rect.x = 0;
    in_param.image_rect.y = 0;

    in_param.image_rect.w = 200;
    in_param.image_rect.h = 200;

    in_param.quality_level = 2;

    in_param.jpeg_buf_ptr   = g_JpegBuf;
    in_param.jpeg_buf_size  = 30*1024;

    in_param.yuv_addr.y_chn_addr = (uint32)frame_addr_ptr->yaddr;
    in_param.yuv_addr.u_chn_addr = (uint32)((uint8*)frame_addr_ptr->uaddr + 200*200);
    in_param.yuv_addr.v_chn_addr = (uint32)((uint8*)frame_addr_ptr->vaddr + 200*200+ 200*200/4);

    jpeg_ret = JPEG_EncodeJpeg(&in_param, &out_param);
    if(JPEG_ENC_OK != jpeg_ret )
    {
        //SCI_TRACE_LOW:"UPCC: JPEG ENCODE ERR!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1081_112_2_18_1_24_1_331,(uint8*)"");
    }

    //ISP_ServiceFreeFrameAddress((void*)frame_addr_ptr);

    size = out_param.out_size;
    if(0==size || JPEG_ENC_OK != jpeg_ret)
    {
        //SCI_TRACE_LOW:"UPCC_Camera.c->UPCC_GetAndSendFrame(): one frame lost."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_1089_112_2_18_1_24_1_332,(uint8*)"");
        _DCAMERA_Release_UPCC_CTRL();
        return DCAMERA_OP_ENCODE_ERR;
    }
    _DCAMERA_Release_UPCC_CTRL();

    return ISP_CB_PROC_DONE;
}
#endif


LOCAL uint32 _DCAMERA_JpgEncGetSliceF420T422(JINF_YUV_CHN_T *yuv_chn,
                                     uint32 line_offset,
                                     uint32 width,
                                     uint32 read_lines)
{
    uint32                  src_buf_offset = 0;
    uint32                  slice_line = read_lines;
    uint32                  src_line_offset = 0;
    ISP_ADDRESS_T     input_addr = {0};
    ISP_ADDRESS_T     output_addr = {0};
    uint32                  mem_size = 0;
    uint32                  rtn_slice = 0;
    uint32                  i = 0;
    DCAMERA_CAPTURE_DATE_T  date_mark = {0};

    DCAMERA_TRACE("DC:JpgEncF420T422 photo_w =%d, get_slice_w=%d, line_offset %d, read_lines %d, yaddr 0x%x, uv 0x%x",
                            s_dc_info_ptr->jpeg_rect.w,width,line_offset,read_lines,yuv_chn->y_chn_ptr,yuv_chn->u_chn_ptr
                             );

    if(s_dc_info_ptr->jpeg_rect.h <= line_offset)
    {
        DCAMERA_TRACE("DC:JpgEncF420T422: photo_h=%d,line_offset=%d", s_dc_info_ptr->jpeg_rect.w,line_offset);
        return 0;
    }

    if(read_lines > DC_SLICE_HEIGHT)
    {
       DCAMERA_TRACE("DC:JpgEncF420T422: dc slice height too large!");
        return 0;
    }

    src_line_offset = s_src_line_offset;                        //offset information mark

    if(src_line_offset + slice_line > s_dc_info_ptr->isp_frame.height)
    {
        slice_line = s_dc_info_ptr->isp_frame.height - src_line_offset;
    }

    input_addr.yaddr = s_dc_info_ptr->isp_frame.yaddr;
    input_addr.uaddr = s_dc_info_ptr->isp_frame.uaddr;

    output_addr.yaddr = (uint32)DCAMERA_CaptureGetSliceBufY(&mem_size);
    if(mem_size < s_dc_info_ptr->isp_frame.width * read_lines)
    {
        DCAMERA_TRACE("DC:JpgEncF420T422 error1");
        return 0;
    }
    output_addr.uaddr = (uint32)DCAMERA_CaptureGetSliceBufUV(&mem_size);
    if(mem_size < s_dc_info_ptr->isp_frame.width * read_lines)
    {
        DCAMERA_TRACE("DC:JpgEncF420T422 error2");
        return 0;
    }
    src_buf_offset = (uint32)(src_line_offset * s_dc_info_ptr->isp_frame.width);
    input_addr.yaddr += src_buf_offset;
    input_addr.uaddr += (src_buf_offset >> 1);                  //YUV420
    slice_addr.yaddr = output_addr.yaddr;
    slice_addr.uaddr = output_addr.uaddr;

    s_src_line_offset += slice_line;

    SCI_MEMCPY((void *)output_addr.yaddr, (void *)input_addr.yaddr, (s_dc_info_ptr->isp_frame.width * slice_line));   //copy y to slice

    for(i = 0; i < slice_line; i++)                                                   //copy uv to slice
    {
        SCI_MEMCPY((void *)output_addr.uaddr, (void *)input_addr.uaddr, (uint32)s_dc_info_ptr->isp_frame.width);
        output_addr.uaddr += s_dc_info_ptr->isp_frame.width;
        if(i&1)
        {
            input_addr.uaddr += s_dc_info_ptr->isp_frame.width;
        }
    }

    s_slice_out_once = slice_line;

    //blender the date mark for the output slice
    date_mark = b_burst_snapshot?(s_snapshot_burst_param.capture_date):(s_snapshot_param.capture_date);

    if(date_mark.eb)
    {
        DCAMERA_RECT_T slice_rect = {0};
        DCAMERA_RECT_T src_date_rect = {0};
        uint32    * src_date_addr = PNULL;

        //calculate slice date rect and date buff
        slice_rect.x = 0;
        slice_rect.y = s_dst_line_offset;
        slice_rect.w = s_dc_info_ptr->jpeg_rect.w;
        slice_rect.h = s_slice_out_once;

        src_date_addr = (uint32 *)date_mark.osd_addr.y_addr;

        //calculate date rect
        src_date_rect = date_mark.overlay_rect;

        if((date_mark.overlay_rect.y < (s_dst_line_offset+s_slice_out_once))&&
            ((uint32)(date_mark.overlay_rect.y+date_mark.overlay_rect.h -1) > s_dst_line_offset))
        {
            _DCAMERA_handleDateMark(slice_rect, slice_addr, src_date_rect, src_date_addr, FALSE);
        }
    }

    s_dst_line_offset += s_slice_out_once;

    yuv_chn->y_chn_ptr = (uint8 *)slice_addr.yaddr;             /*lint !e64*/
    yuv_chn->u_chn_ptr = (uint8 *)slice_addr.uaddr;             /*lint !e64*/

    MMU_DmaCacheSync((uint32)yuv_chn->y_chn_ptr,
                     (uint32)(s_dc_info_ptr->isp_frame.width * slice_line),
                     DMABUFFER_TO_DEVICE);
    MMU_DmaCacheSync((uint32)yuv_chn->u_chn_ptr,
                     (uint32)(s_dc_info_ptr->isp_frame.width * slice_line),
                     DMABUFFER_TO_DEVICE);

    rtn_slice = s_slice_out_once;

    return rtn_slice;

}

LOCAL uint32 _DCAMERA_JpgEncGetSlice(JINF_YUV_CHN_T *yuv_chn,
                                     uint32 line_offset,
                                     uint32 width,
                                     uint32 read_lines)
{
    uint32              src_buf_offset = 0;
    uint32              slice_line = 0;
    uint32              src_line_offset = 0;
//    DCAMERA_SIZE_T      dc_size = {0};
//    DCAMERA_RECT_T      dc_rect = {0};
//    DCAMERA_SIZE_T      dc_dst_size = {0};
    ISP_SCLAE_PARAM_T   isp_scale_param = {0};
    uint32              mem_size = 0;
    uint32              rtn_slice = 0;
    DCAMERA_CAPTURE_DATE_T  date_mark = {0};

    //DCAMERA_TRACE:"_DCAMERA_JpgEncGetSlice,photo_width =%d, get_slice_width=%d, line_offset %d, read_lines %d, yaddr 0x%x, uv 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9722_112_2_18_1_24_19_518,(uint8*)"dddddd",s_dc_info_ptr->jpeg_rect.w,width,line_offset,read_lines,yuv_chn->y_chn_ptr,yuv_chn->u_chn_ptr);

    if(s_dc_info_ptr->jpeg_rect.h <= line_offset &&
       s_slice_read_out == s_slice_out_once)
    {
        //DCAMERA_TRACE:"_DCAMERA_JpgEncGetSlice,photo_height=%d,line_offsert=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9729_112_2_18_1_24_19_519,(uint8*)"dd",s_dc_info_ptr->jpeg_rect.w,line_offset);
        return 0;
    }


    if(s_slice_read_out >= s_slice_out_once) // all slice height output by last slice scaling has been read out
    {
#if 0
        dc_size.w = s_dc_info_ptr->isp_frame.width;
        dc_size.h = s_dc_info_ptr->isp_frame.height;
        dc_rect.w = dc_size.w;
        dc_rect.h = dc_size.h;

        dc_dst_size.w = s_dc_info_ptr->jpeg_rect.w;
        dc_dst_size.h = s_dc_info_ptr->jpeg_rect.h;
        _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);

        src_line_offset = s_src_line_offset;
        src_line_offset += dc_rect.y;

        slice_line = (uint32)(read_lines * dc_rect.h / s_dc_info_ptr->jpeg_rect.h);
        slice_line = (uint32)(slice_line / DC_SLICE_HEIGHT * DC_SLICE_HEIGHT);
        if(slice_line < DC_SLICE_HEIGHT)
        {
            slice_line = DC_SLICE_HEIGHT;
        }
        isp_scale_param.input_size.w = (uint32)dc_size.w;
        isp_scale_param.input_size.h = (uint32)dc_size.h;
        isp_scale_param.input_rect.x = (uint32)dc_rect.x;
        isp_scale_param.input_rect.w = (uint32)dc_rect.w;
        isp_scale_param.input_rect.h = (uint32)dc_rect.h;
#else
        src_line_offset = s_src_line_offset;
        slice_line = (uint32)(read_lines * s_dc_info_ptr->isp_frame.height / s_dc_info_ptr->jpeg_rect.h);
        slice_line = (uint32)(slice_line / DC_SLICE_HEIGHT * DC_SLICE_HEIGHT);
        if(slice_line < DC_SLICE_HEIGHT)
        {
            slice_line = DC_SLICE_HEIGHT;
        }

        if(src_line_offset + slice_line > s_dc_info_ptr->isp_frame.height)
        {
            slice_line = s_dc_info_ptr->isp_frame.height - src_line_offset;
        }
        isp_scale_param.input_size.w = (uint32)s_dc_info_ptr->isp_frame.width;
        isp_scale_param.input_size.h = (uint32)s_dc_info_ptr->isp_frame.height;
        isp_scale_param.input_rect.x = (uint32)0;
        isp_scale_param.input_rect.w = (uint32)s_dc_info_ptr->isp_frame.width;
        isp_scale_param.input_rect.h = (uint32)s_dc_info_ptr->isp_frame.height;
#endif
        isp_scale_param.output_size.w= s_dc_info_ptr->jpeg_rect.w;
        isp_scale_param.output_size.h= s_dc_info_ptr->jpeg_rect.h;


        isp_scale_param.slice_line   = slice_line;
        isp_scale_param.scale_mode   = ISP_SERVICE_SCALE_SLICE;
        if(SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
        {
            isp_scale_param.input_format = ISP_IMAGE_FORMAT_YUV420;
        }
        else
        {
            isp_scale_param.input_format = ISP_IMAGE_FORMAT_YUV422;
        }
        isp_scale_param.input_addr.yaddr = s_dc_info_ptr->isp_frame.yaddr;
        isp_scale_param.input_addr.uaddr = s_dc_info_ptr->isp_frame.uaddr;
        isp_scale_param.output_format= ISP_IMAGE_FORMAT_YUV422;
        isp_scale_param.output_addr.yaddr = (uint32)DCAMERA_CaptureGetSliceBufY(&mem_size);
        isp_scale_param.output_addr.uaddr = (uint32)DCAMERA_CaptureGetSliceBufUV(&mem_size);
        isp_scale_param.line_buf_addr = (uint32)DCAMERA_CaptureGetLineBuf(&mem_size);
        isp_scale_param.swap_buf_addr = (uint32)DCAMERA_CaptureGetSwapBufY(&mem_size);

        if(s_is_first_slice)
        {
            isp_scale_param.is_first     = 1;
            s_is_first_slice = 0;
            s_dst_line_offset = 0;                              //initial the dest slice output height
        }
        else
        {
            isp_scale_param.is_first     = 0;
        }

        src_buf_offset = (uint32)(src_line_offset * s_dc_info_ptr->isp_frame.width);

        if(src_buf_offset % DCAMERA_PADDING_MEM_SIZE)
        {
            isp_scale_param.input_addr.yaddr = (uint32)DCAMERA_CaptureGetSrcSliceBufY(&mem_size);
            isp_scale_param.input_addr.uaddr = (uint32)DCAMERA_CaptureGetSrcSliceBufUV(&mem_size);
            if(SCI_NULL == isp_scale_param.input_addr.yaddr ||
               SCI_NULL == isp_scale_param.input_addr.uaddr)
            {
                //DCAMERA_TRACE:"_DCAMERA_JpgEncGetSlice,No source slice buffer, y 0x%x u 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_9823_112_2_18_1_24_20_520,(uint8*)"dd",isp_scale_param.input_addr.yaddr,isp_scale_param.input_addr.uaddr);

                return 0;
            }
            SCI_MEMCPY((void*)isp_scale_param.input_addr.yaddr,
                       (void*)(s_dc_info_ptr->isp_frame.yaddr + src_buf_offset),
                       (uint32)(slice_line*s_dc_info_ptr->isp_frame.width));

            if(SNAPSHOT_YUV422 == s_dc_info_ptr->snapshot_ctl_info.param)
            {
                SCI_MEMCPY((void*)isp_scale_param.input_addr.uaddr,
                           (void*)(s_dc_info_ptr->isp_frame.uaddr + src_buf_offset),
                           (uint32)(slice_line*s_dc_info_ptr->isp_frame.width));
            }
            else
            {
                SCI_MEMCPY((void*)isp_scale_param.input_addr.uaddr,
                           (void*)(s_dc_info_ptr->isp_frame.uaddr + (src_buf_offset>>1)),
                           (uint32)((slice_line*s_dc_info_ptr->isp_frame.width)>>1));

            }
        }
        else
        {
            isp_scale_param.input_addr.yaddr += src_buf_offset;
            if(SNAPSHOT_YUV422 == s_dc_info_ptr->snapshot_ctl_info.param)
            {
                isp_scale_param.input_addr.uaddr += src_buf_offset;
            }
            else
            {
                isp_scale_param.input_addr.uaddr += (src_buf_offset >> 1);
            }
        }

        isp_scale_param.input_rect.y = 0;

        s_slice_out_once = 0;
        isp_scale_param.get_scale_data = _DCAMERA_GetScaledData;
        ISP_ServiceSetScaleParam(&isp_scale_param);
        ISP_ServiceStartScale();
        _DCAMERA_WaitForCaptureDone();

        s_src_line_offset += slice_line;
        if(s_slice_out_once > read_lines)
        {
            rtn_slice = read_lines;
            s_slice_read_out = read_lines;
        }
        else
        {
            rtn_slice = s_slice_out_once;
            s_slice_read_out = s_slice_out_once;
        }

        //blender the date mark for the output slice
        date_mark = b_burst_snapshot?(s_snapshot_burst_param.capture_date):(s_snapshot_param.capture_date);

        if(date_mark.eb)
        {
            DCAMERA_RECT_T slice_rect = {0};
            DCAMERA_RECT_T src_date_rect = {0};
            uint32    * src_date_addr = PNULL;

            //calculate slice date rect and date buff
            slice_rect.x = 0;
            slice_rect.y = s_dst_line_offset;
            slice_rect.w = s_dc_info_ptr->jpeg_rect.w;
            slice_rect.h = s_slice_out_once;

            src_date_addr = (uint32 *)date_mark.osd_addr.y_addr;

            //calculate date rect
            src_date_rect = date_mark.overlay_rect;

            if((date_mark.overlay_rect.y < (s_dst_line_offset+s_slice_out_once))&&
                ((uint32)(date_mark.overlay_rect.y+date_mark.overlay_rect.h -1) > s_dst_line_offset))
            {
                _DCAMERA_handleDateMark(slice_rect, slice_addr, src_date_rect, src_date_addr, FALSE);
            }
        }

        s_dst_line_offset += s_slice_out_once;

        //if(_DCAMERA_IsScalingBufferUsedForJpegEncoding())
        //{
            yuv_chn->y_chn_ptr = (uint8 *)slice_addr.yaddr;
            yuv_chn->u_chn_ptr = (uint8 *)slice_addr.uaddr;
        //}
        //else
        //{
        //    SCI_MEMCPY((void*)yuv_chn->y_chn_ptr, (void*)slice_addr.yaddr, (uint32)(width*rtn_slice));
        //    SCI_MEMCPY((void*)yuv_chn->u_chn_ptr, (void*)slice_addr.uaddr, (uint32)(width*rtn_slice));
        //}
    }
    else //there still has (s_slice_out_once - s_slice_read_out) lines to be read out yet
    {
        rtn_slice = s_slice_out_once - s_slice_read_out;

        //if(_DCAMERA_IsScalingBufferUsedForJpegEncoding())
        //{
            yuv_chn->y_chn_ptr = (uint8 *)(slice_addr.yaddr + s_slice_read_out*width);
            yuv_chn->u_chn_ptr = (uint8 *)(slice_addr.uaddr + s_slice_read_out*width);
        //}
        //else
        //{
        //    SCI_MEMCPY((void*)yuv_chn->y_chn_ptr,
        //            (void*)(slice_addr.yaddr + (uint32)(s_slice_read_out*width)),
        //            (uint32)(width*rtn_slice));
        //    SCI_MEMCPY((void*)yuv_chn->u_chn_ptr,
        //            (void*)(slice_addr.uaddr + (uint32)(s_slice_read_out*width)),
        //            (uint32)(width*rtn_slice));
        //}

        if(rtn_slice > read_lines)
        {
            s_slice_read_out += read_lines;
            rtn_slice = read_lines;
        }
        else
        {
            s_slice_read_out = s_slice_out_once;
        }
/*
        SCI_MEMCPY((void*)yuv_chn->y_chn_ptr,
                    (void*)(slice_addr.yaddr + (uint32)(s_slice_read_out*width)),
                    (uint32)(width*rtn_slice));
        SCI_MEMCPY((void*)yuv_chn->u_chn_ptr,
                    (void*)(slice_addr.uaddr + (uint32)(s_slice_read_out*width)),
                    (uint32)(width*rtn_slice));
*/

    }
    return rtn_slice;
}

LOCAL int _DCAMERA_DoSnapShotJpgZoom(DCAMERA_SNAPSHOT_PARAM_T const *param_ptr)
{
    JPEG_ZOOM_IN_PARAM_T In = {0};
    uint32 compress_level = param_ptr->enc_quality_level;
    uint8 *pJpg         = SCI_NULL;
    uint32 size         = 0;
    DCAMERA_INFO_T      *pDcInfo = s_dc_info_ptr;

    if(!(pDcInfo->buf_len && pDcInfo->jpeg_buf))
        return DCAMERA_OP_PARAM_ERR;

    if( compress_level > DCAMERA_ENC_QUALITY_MID)
        compress_level = JINF_QUALITY_MIDDLE_HIGH;
    else if(compress_level < DCAMERA_ENC_QUALITY_MID)
        compress_level =    JINF_QUALITY_LOW;
    else
        compress_level = JINF_QUALITY_MIDDLE;

    In.quality          = compress_level;
    In.jpeg_buf_ptr     = pDcInfo->jpeg_buf;
    In.jpeg_buf_size    = pDcInfo->buf_len;

    In.decode_buf_ptr   = (uint8*)DCAMERA_CaptureGetYUVBuf(&In.decode_buf_size);
    if(SCI_NULL == In.decode_buf_ptr
        || (VGA_WIDTH*VGA_HEIGHT*2) > In.decode_buf_size)
        return DCAMERA_OP_NO_ENOUGH_MEMORY;

    In.target_buf_ptr   = (uint8*)DCAMERA_CaptureGetJPGZoomBuf(&In.target_buf_size);
    if(SCI_NULL == In.target_buf_ptr || 0 == In.target_buf_size)
        return DCAMERA_OP_NO_ENOUGH_MEMORY;

    In.crop_rect.x = (uint16)(pDcInfo->zoom_w_step_snap *
        pDcInfo->preview_ctl_info.zoom_level)/2;
    In.crop_rect.y = (uint16)(pDcInfo->zoom_h_step_snap *
        pDcInfo->preview_ctl_info.zoom_level)/2;

    // Make sure that x offset and y offset of input image multiple of 4
    In.crop_rect.x = DCAMERA_WIDTH(In.crop_rect.x);
    In.crop_rect.y = DCAMERA_HEIGHT(In.crop_rect.y);

    In.crop_rect.w = (s_sensor_ptr->sensor_mode_info[pDcInfo->snapshot_m].width -
        (uint16)(pDcInfo->zoom_w_step_snap * pDcInfo->preview_ctl_info.zoom_level));
    In.crop_rect.h = (s_sensor_ptr->sensor_mode_info[pDcInfo->snapshot_m].height-
        (uint16)(pDcInfo->zoom_h_step_snap * pDcInfo->preview_ctl_info.zoom_level));

    // Make sure that height and width of input image multiple of 4
    In.crop_rect.w = DCAMERA_WIDTH(In.crop_rect.w);
    In.crop_rect.h = DCAMERA_HEIGHT(In.crop_rect.h);

    In.target_width = s_dc_info_ptr->jpeg_rect.w;
    In.target_height= s_dc_info_ptr->jpeg_rect.h;

    if(_DCAMERA_CloseISP())
        return DCAMERA_OP_ERROR;

    //DCAMERA_TRACE:"DC: _DBGDC_DoSnapShotJpgZoom flag"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10013_112_2_18_1_24_20_521,(uint8*)"");

    if(!(JPEG_ZoomJpeg(&In, &pJpg, &size)))
    {
        SCI_MEMCPY(pDcInfo->jpeg_buf, pJpg, size);
        pDcInfo->buf_len = size;
        if(_DCAMERA_OpenISP())
            return DCAMERA_OP_ERROR;

        return DCAMERA_OP_SUCCESS;
    }
    else
    {
        _DCAMERA_OpenISP();
        return DCAMERA_OP_ERROR;
    }
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_YUV422ToYUV420(void)
{
    uint32 i = 0 ,len = 0 ;
    uint32 src_width  = s_dc_info_ptr->isp_frame.width;
    uint32 src_height = s_dc_info_ptr->isp_frame.height;
    uint32 src_chn    = s_dc_info_ptr->isp_frame.uaddr;
    uint32 dst_chn    = s_dc_info_ptr->isp_frame.uaddr;

    if(PNULL == src_chn || PNULL == dst_chn)
    {
        return DCAMERA_OP_MEMORY_ERR;
    }

    for(i=0; i<src_height; i=i+2)
    {
        SCI_MEMCPY((void *)dst_chn, (void *)src_chn, src_width);
        src_chn = src_chn + (src_width<<1);
        dst_chn = dst_chn + src_width;
    }

    src_chn = s_dc_info_ptr->isp_frame.uaddr;
    dst_chn = (uint32)DCAMERA_CaptureGetUVBuf(&len);

    if(PNULL == dst_chn || 0 == len)
    {
        return DCAMERA_OP_MEMORY_ERR;
    }

    SCI_MEMCPY((void *)dst_chn, (void *)src_chn, ((src_width*src_height)>>1));

    s_dc_info_ptr->isp_frame.uaddr =  dst_chn;

    return DCAMERA_OP_SUCCESS;
}

LOCAL int _DCAMERA_GetFrameFromIspPsl(DCAMERA_SNAPSHOT_PARAM_T *param_ptr,
                                      BOOLEAN is_isp_bypass,
                                      DCAMERA_QUICK_VIEW_CALL_BACK quickview_cb)
{
    ISP_COMPOUND_JPEG_CAPTURE_PARAM_T  isp_cmp_cap_param = {0};
    DCAMERA_MEM_PARAM_T                mem_param_t = {0};
    DCAMERA_INFO_T                     *pDcInfo  = s_dc_info_ptr;
    uint16                             target_w  = 0;
    uint16                             target_h  = 0;
    void*                              mem_start = 0;
    uint32                             mem_size  = 0;
    DCAMERA_RETURN_VALUE_E             ret_val   = DCAMERA_OP_SUCCESS;

    SCI_TRACE_LOW("DC:FromIspPsl ");

    target_w = pDcInfo->jpeg_rect.w;
    target_h = pDcInfo->jpeg_rect.h;

    if(((ISP_ROTATION_270 == pDcInfo->isp_preview_param.disp_rotation)
     || (ISP_ROTATION_90 == pDcInfo->isp_preview_param.disp_rotation))
     && (!((DCAMERA_ROTATION_90 == param_ptr->rotation_mode)
     || (DCAMERA_ROTATION_270 == param_ptr->rotation_mode))))
    {
        target_w = pDcInfo->jpeg_rect.h;
        target_h = pDcInfo->jpeg_rect.w;
        mem_param_t.review_rot = pDcInfo->preview_ctl_info.rotation_mode;
    }
    else
    {
        mem_param_t.review_rot = param_ptr->rotation_mode;
    }

    //personal snapshot
    isp_cmp_cap_param.input_range.x = pDcInfo->jpeg_rect.x;
    isp_cmp_cap_param.input_range.y = pDcInfo->jpeg_rect.y;
    isp_cmp_cap_param.input_range.w = target_w;
    isp_cmp_cap_param.input_range.h = target_h;

    isp_cmp_cap_param.input_format  = ISP_IMAGE_FORMAT_YUV422;//ISP_IMAGE_FORMAT_RGB565;
    isp_cmp_cap_param.output_size.w = target_w;
    isp_cmp_cap_param.output_size.h = target_h;

    mem_param_t.review_width    = target_w;
    mem_param_t.review_height   = target_h;
    mem_param_t.b_personal_cap  = 1;
    mem_start = DCAMERA_PreviewGetMem(&mem_param_t,&mem_size);
    if(SCI_NULL == mem_start || 0 == mem_size)
        return DCAMERA_OP_NO_ENOUGH_MEMORY;

    s_dc_info_ptr->snapshot_ctl_info.param = SNAPSHOT_YUV422;       //if personal cap then use the 422 capture

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    ISP_ServiceMemConfig((void*)mem_start, mem_size);
    ISP_ServiceSetPreviewParam(&s_dc_info_ptr->isp_preview_param);
    ret_val = ISP_ServiceStartPreview();
    ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, _DCAMERA_ISP_REG_CALLBACK);
    ISP_ServiceSetCompoundJpegCaptureParam(&isp_cmp_cap_param);
    ISP_ServiceStartCaptureCompoundJpeg();
    LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);
    SCI_TRACE_LOW("DC:FromIspPsl NO_PERMIT_GUI_INVALIDATE");

    if(_DCAMERA_WaitForCaptureDone())
    {
        LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        return DCAMERA_OP_ISP_ERR;
    }

    if(quickview_cb)
    {
        (*quickview_cb)();
    }

    LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);

    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    if(DCAMERA_OP_SUCCESS != ret_val)
    {
        return DCAMERA_OP_ISP_ERR;
    }

    pDcInfo->isp_frame.width    = isp_cmp_cap_param.output_size.w;
    pDcInfo->isp_frame.height   = isp_cmp_cap_param.output_size.h;


    return DCAMERA_OP_SUCCESS;
}

LOCAL int _DCAMERA_GetFrameFromIspNml(DCAMERA_SNAPSHOT_PARAM_T *param_ptr,
                                      BOOLEAN is_isp_bypass,
                                      DCAMERA_QUICK_VIEW_CALL_BACK quickview_cb)
{
    ISP_JPEG_PARAM_T    *p_cap_para  = &isp_cap_param;
    void*               mem_start    = 0;
    uint32              mem_size     = 0;
    DCAMERA_SIZE_T      dc_dst_size = {0};
    DCAMERA_RECT_T      dc_rect = {0};

	SCI_TRACE_LOW("DC:FromIspNml ");

    if(SENSOR_MODE_MAX <= s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].mode)
        return DCAMERA_OP_ERROR;

    dc_rect.w = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
    dc_rect.h = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;
    p_cap_para->input_size.w   = dc_rect.w;
    p_cap_para->input_size.h   = dc_rect.h;
    dc_dst_size.w = s_dc_info_ptr->snapshot_ctl_info.sensor_out_width;
    dc_dst_size.h = s_dc_info_ptr->snapshot_ctl_info.sensor_out_height;

    _DCAMERA_GetCaptureTrimRect(DCAMERA_ROTATION_0, &dc_dst_size, &dc_rect);
    //the angle should be zero here for that dc_dst_size has been calculated before

    p_cap_para->input_range.x  = dc_rect.x;
    p_cap_para->input_range.y  = dc_rect.y;
    p_cap_para->input_range.w  = dc_rect.w;
    p_cap_para->input_range.h  = dc_rect.h;
    p_cap_para->output_size.w  = s_dc_info_ptr->snapshot_ctl_info.sensor_out_width;
    p_cap_para->output_size.h  = s_dc_info_ptr->snapshot_ctl_info.sensor_out_height;

    if(p_cap_para->output_size.w != p_cap_para->input_range.w ||
       p_cap_para->output_size.h != p_cap_para->input_range.h)
    {
        if(p_cap_para->output_size.w > ISP_SCALE_FRAME_MODE_WIDTH_TH)
        {
            p_cap_para->input_range.w = p_cap_para->output_size.w;
            p_cap_para->input_range.h = p_cap_para->output_size.h;
        }
    }

    if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format)
    {
        mem_start = param_ptr->data_buf;
        mem_size  = param_ptr->buf_len;
    }
    else
    {
        if(param_ptr->rotation_mode == DCAMERA_ROTATION_0)
        {
          //  mem_start = DCAMERA_CaptureGetYUVBuf(&mem_size);
        }
        else
        {
          //  mem_start = DCAMERA_CaptureGetYUVRotBuf(&mem_size);
        }

        if(SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
        {
          //  p_cap_para->yuv_type = ISP_IMAGE_FORMAT_YUV420;
        }
        else
        {
           // p_cap_para->yuv_type = ISP_IMAGE_FORMAT_YUV422;
        }

		p_cap_para->yuv_type = ISP_IMAGE_FORMAT_YUV422;

        if(DCAMERA_IsSupport2MSnapshot() &&
           (s_dc_info_ptr->snapshot_ctl_info.sensor_out_width > VGA_WIDTH) &&
           (s_dc_info_ptr->jpeg_rect.w > VGA_WIDTH))
        {
            void*   mem_start_y  = 0;
            uint32  mem_size_y   = 0;

            if(s_dc_info_ptr->snapshot_ctl_info.sensor_out_height > DCAMERA_ISP_PATH_YUV420_TH)
            {
                p_cap_para->yuv_type = ISP_IMAGE_FORMAT_YUV422;

                mem_start = DCAMERA_CaptureGetStartBuf(&mem_size);
            }
            else
            {
                mem_start = DCAMERA_CaptureGetUVBuf(&mem_size);
            }

            if(SCI_NULL == mem_start || 0 == mem_size)
                return DCAMERA_OP_NO_ENOUGH_MEMORY;

            mem_start_y = DCAMERA_CaptureGetYBuf(&mem_size_y);

            if(SCI_NULL == mem_start_y || 0 == mem_size_y)
                return DCAMERA_OP_NO_ENOUGH_MEMORY;

            ISP_ServiceMemConfig_YBuffer((void*)mem_start_y, mem_size_y);

            _DCAMERA_GetMutex(s_callback_mutex_ptr);
            LCD_SetCopyOsdMode(MAIN_LCD_ID,SCI_FALSE);
            _DCAMERA_PutMutex(s_callback_mutex_ptr);
        }
    }

    //if(SCI_NULL == mem_start || 0 == mem_size)
       // return DCAMERA_OP_NO_ENOUGH_MEMORY;

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    //ISP_ServiceMemConfig((void*)mem_start, mem_size);
    ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, _DCAMERA_ISP_REG_CALLBACK);

    if(ISP_ServiceSetJpegCaptureParam(p_cap_para))
    {
        LCD_SetCopyOsdMode(MAIN_LCD_ID,SCI_TRUE);
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
	SCI_TRACE_LOW("DC:JpegCaptureParam error ");
        return DCAMERA_OP_ISP_ERR;
    }

    ISP_ServiceStartCaptureJpeg();

    if(_DCAMERA_WaitForCaptureDone())
    {
        LCD_SetCopyOsdMode(MAIN_LCD_ID,SCI_TRUE);
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        return DCAMERA_OP_ISP_ERR;
    }
    _DCAMERA_PutMutex(s_callback_mutex_ptr);

	#if 0
    s_dc_info_ptr->isp_frame.width    = p_cap_para->output_size.w;
    s_dc_info_ptr->isp_frame.height   = p_cap_para->output_size.h;
	#else
    s_dc_info_ptr->isp_frame.width    = 640;
    s_dc_info_ptr->isp_frame.height   = p_cap_para->output_size.h;
	#endif

    if(DCAMERA_IsSupport2MSnapshot() &&
       (s_dc_info_ptr->snapshot_ctl_info.sensor_out_width > VGA_WIDTH) &&
       (s_dc_info_ptr->jpeg_rect.w > VGA_WIDTH))
    {
        if((s_dc_info_ptr->snapshot_ctl_info.sensor_out_height > DCAMERA_ISP_PATH_YUV420_TH)&&
           (ISP_IMAGE_FORMAT_YUV422 == p_cap_para->yuv_type))
        {
            _DCAMERA_YUV422ToYUV420();
        }
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        LCD_SetCopyOsdMode(MAIN_LCD_ID,SCI_TRUE);
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
    }

    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);
    SCI_TRACE_LOW("DC:IspNml NO_PERMIT_GUI_INVALIDATE");

    if(quickview_cb)
    {
        (*quickview_cb)();
    }
    LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
    SCI_TRACE_LOW("DC:FromIspNml PERMIT_GUI_INVALIDATE");

    _DCAMERA_PutMutex(s_callback_mutex_ptr);
        //SCI_TRACE_LOW("david capture _DCAMERA_GetFrameFromIspNml 111");

    if(SENSOR_IMAGE_FORMAT_JPEG == s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format)
    {
        s_dc_info_ptr->jpeg_buf = (uint8 *)s_dc_info_ptr->isp_frame.yaddr;
        s_dc_info_ptr->buf_len  = s_dc_info_ptr->isp_frame.uaddr;

        if(s_dc_info_ptr->is_snapshot_ex)
        {
            if(_DCAMERA_DoSnapShotExDecJpgFrame()) //decode and scaling
                return DCAMERA_OP_DECODE_ERR;
        }
        else
        {
            if(_DCAMERA_DoSnapShotDecJpgFrame(param_ptr))
                return DCAMERA_OP_DECODE_ERR;
        }
    }

    return SCI_SUCCESS;
}


LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_GetFrameFromISP(DCAMERA_SNAPSHOT_PARAM_T *param_ptr,
                                                      BOOLEAN bIsPersonalSnapshot,
                                                      BOOLEAN bIsIspBypass,
                                                      DCAMERA_QUICK_VIEW_CALL_BACK quickview_cb)
{
    ISP_JPEG_PARAM_T    *pIspCapPara    = &isp_cap_param;
    SENSOR_EXP_INFO_T   *pSsrExpInfo    = s_sensor_ptr;

    if(!pSsrExpInfo)
        return SCI_ERROR;

    s_dc_operation_flag &= ~DCAMERA_OP_ISP_CAPTURE_FRAME_FLAG;

    DCAMERA_SetCapParam(&pIspCapPara->cap_param);

    pIspCapPara->cap_param.cap_frame_skipped = pSsrExpInfo->capture_skip_num;

    if(bIsPersonalSnapshot)
        return _DCAMERA_GetFrameFromIspPsl(param_ptr, bIsIspBypass, quickview_cb);
    else
        return _DCAMERA_GetFrameFromIspNml(param_ptr, bIsIspBypass, quickview_cb);
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_DoSnapshot(DCAMERA_SNAPSHOT_PARAM_T*          param_ptr,
                                                 DCAMERA_SNAPSHOT_RETURN_PARAM_T*   return_param_ptr)
{
    DCAMERA_RETURN_VALUE_E          ret_value = DCAMERA_OP_SUCCESS;
    DCAMERA_CALL_BACK               func_ptr = PNULL;
    BOOLEAN                         select_personal = 0;
    BOOLEAN                         is_bypass_isp = SCI_FALSE;
    uint8*                          malloc_buffer_ptr = PNULL;
    uint32                          malloc_buffer_len = 0;
    JPEG_ENC_IN_PARAM_T             jpeg_enc_in_param = {0};
    JPEG_ENC_OUT_PARAM_T            jpeg_enc_out_param = {0};
    uint32                          call_back_return = 0;
    DCAMERA_YUV_CHN_ADDR_T          temp_yuv_addr = {0};
    uint32                          image_size = 0;
    LCD_INFO_T                      lcd_info = {0};
    DCAMERA_PREVIEW_CTL_INFO_T      *preview_ctl_info_ptr = &s_dc_info_ptr->preview_ctl_info;

	DCAMERA_INFO_T      *   dc_info_ptr     = s_dc_info_ptr;
	DCAMERA_MEM_PARAM_T     mem_param_t = {0};
	uint32                  mem_start = 0, mem_size = 0;

    //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapshot -> start time = %d,rot %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10261_112_2_18_1_24_21_522,(uint8*)"dd",                                      SCI_GetTickCount(),param_ptr->rotation_mode);
    SCI_ASSERT(NULL != param_ptr && NULL != return_param_ptr); /*assert verified*/

	#if 0
	SCI_TRACE_LOW("david capture  type:%d, quality:%d rotate:%d, img[%d]x[%d], photo[%d]X[%d]",
					param_ptr->data_type, param_ptr->enc_quality_level,
					param_ptr->rotation_mode,
					param_ptr->personal_snapshot.image_width,
					param_ptr->personal_snapshot.image_height,
					param_ptr->personal_snapshot.photo_width,
					param_ptr->personal_snapshot.photo_height);

	SCI_TRACE_LOW("david capture data:0x%x data_len:%d",
					param_ptr->data_buf,
					param_ptr->buf_len);
	#endif

	SCI_TRACE_LOW("david %s camera memAvalid=%d", __FUNCTION__,SCI_GetByteHeapAvailableSpace(DYNAMIC_HEAP_APP_MEMORY) );

	select_personal = param_ptr->personal_snapshot.select_personal;
    if(PNULL != param_ptr->snapshot_callback)
    {
        func_ptr = param_ptr->snapshot_callback;
    }

    if(!DCAMERA_IsOpen())
    {
        DC_RET_ERR(DCAMERA_OP_NOT_OPEN)
    }

    if(PNULL == s_sensor_ptr)
    {
        DC_RET_ERR(DCAMERA_OP_NOT_OPEN)
    }

    _DCAMERA_GetMutex(s_callback_mutex_ptr);

    if (NULL != param_ptr->capture_start_callback)
    {
        param_ptr->capture_start_callback(SCI_SUCCESS, PNULL, 0);
    }

    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    if(_DCAMERA_IsPreview())
    {
        SCI_TRACE_LOW("DC:capture StopPreview");
        ISP_ServiceStopPreview();
    }
    else
    {
    	#if 0
        if(DCAMERA_OP_NORMAL == s_operation_mode||
           DCAMERA_OP_SPECIAL == s_operation_mode||
           DCAMERA_OP_MINIDC == s_operation_mode)
        {
        	SCI_TRACE_LOW("david capture open isp");
            if(_DCAMERA_OpenISP())
            {
                DC_RET_ERR(DCAMERA_OP_ISP_ERR)
            }
        }
		#endif
    }

    _DCAMERA_SetPreviewFlag(SCI_FALSE);

//  Reopen Camera Controller
        //SCI_TRACE_LOW("david capture reopen Camera controller!");
        _DCAMERA_CloseISP();
        ISP_UixSetDcamMode(DCAMERA_MODE_SNAPSHOT);
    ret_value = _DCAMERA_OpenISP();
    if(SCI_SUCCESS != ret_value)
    {
        _DCAMERA_Close();
        return ret_value;
    }

    mem_param_t.review_width = dc_info_ptr->target_rect.w;
    mem_param_t.review_height = dc_info_ptr->target_rect.h;
    mem_param_t.review_rot = dc_info_ptr->preview_ctl_info.rotation_mode;
    mem_start = (uint32)DCAMERA_PreviewGetMem(&mem_param_t, &mem_size);
    ISP_ServiceMemConfig((void*)mem_start, mem_size);

	//SCI_TRACE_LOW("david capture reopen Camera controller over!");

//---------------------------------------------------------------------

    if(PNULL != param_ptr->data_buf && 0 != param_ptr->buf_len)
    {
    	SCI_TRACE_LOW("DC:alloc buffer mmi");
        malloc_buffer_ptr = param_ptr->data_buf;
        malloc_buffer_len = param_ptr->buf_len;
    }
    else
    {
         SCI_TRACE_LOW("DC: capture alloc buffer");
		if(g_pic_buffer == NULL)
		{
			malloc_buffer_ptr = (uint8*)SCI_ALLOCA(DCAMERA_JPEG_BUFFER_SIZE);
			if(malloc_buffer_ptr != NULL)
			{
                                SCI_TRACE_LOW("DC:capture alloc buffer OK size:%u(k)", DCAMERA_JPEG_BUFFER_SIZE);
			}
			else
			{
                                SCI_TRACE_LOW("DC:alloc buffer fail");
                                _DCAMERA_CloseSensor();
                                DC_RET_ERR(DCAMERA_OP_NO_ENOUGH_MEMORY)
			}
			malloc_buffer_len = DCAMERA_JPEG_BUFFER_SIZE;
			g_pic_buffer = malloc_buffer_ptr;
		}
		else
		{
			malloc_buffer_ptr = g_pic_buffer;
			malloc_buffer_len = DCAMERA_JPEG_BUFFER_SIZE;
		}

    	#if 0
        ret_value = _DCAMERA_CapMemAssign((DCAMERA_SNAPSHOT_PARAM_T*)param_ptr);

        if(ret_value)
        {
            DC_RET_ERR(ret_value)
        }
        malloc_buffer_ptr = (uint8*)DCAMERA_CaptureGetJPGBuf(&malloc_buffer_len);

        if(SCI_NULL == malloc_buffer_ptr || 0 == malloc_buffer_len)
        {
            DC_RET_ERR(DCAMERA_OP_NO_ENOUGH_MEMORY)
        }
		#endif

    }

    if(DCAMERA_IsSupport2MSnapshot() &&
       (s_dc_info_ptr->snapshot_ctl_info.sensor_out_width > VGA_WIDTH) &&
       (s_dc_info_ptr->jpeg_rect.w > VGA_WIDTH))
    {
        uint16 sensor_output_w = s_dc_info_ptr->snapshot_ctl_info.sensor_out_width;
        uint16 sensor_output_h = s_dc_info_ptr->snapshot_ctl_info.sensor_out_height;

        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);
        SCI_TRACE_LOW("DC:Snapshott NO_PERMIT_GUI_INVALIDATE");
        LCD_SetOpMode(MAIN_LCD_ID, OSD_OPERATION_SPECIAL);

        if(SCI_NULL == DCamera_MemConfig_CaptureYBuf(sensor_output_w,sensor_output_h))
        {
            LCD_SetOpMode(MAIN_LCD_ID, OSD_OPERATION);
            LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
            _DCAMERA_PutMutex(s_callback_mutex_ptr);
            DC_RET_ERR(DCAMERA_OP_NO_ENOUGH_MEMORY)

        }
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
    }

    param_ptr->data_buf = malloc_buffer_ptr;
    param_ptr->buf_len = malloc_buffer_len;

   // SCI_TRACE_LOW("david capture alloc buffer buf:0x%x, len= %d", param_ptr->data_buf,param_ptr->buf_len);

	#if 0
    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    _DCAMERA_BackupPreviewbuf2ReviewBuf(s_dc_info_ptr->isp_preview_param.disp_lcd_id,
                                        s_dc_info_ptr->isp_preview_param.disp_block_id);
    _DCAMERA_PutMutex(s_callback_mutex_ptr);
	#endif

    //DCAMERA_TRACE:"DC: DCAMERA_GetAFrameFromIsp, start time %d  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10327_112_2_18_1_24_21_523,(uint8*)"d", SCI_GetTickCount());

    // Open flash
    if(DCAMERA_FLASH_AUTO == preview_ctl_info_ptr->flash_mode)
    {
        Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_TRUE);
    }

    // do something before snapshot in sensor layer
    if(!b_burst_snapshot)
    {
        Sensor_Ioctl(SENSOR_IOCTL_BEFORE_SNAPSHOT, (uint32)s_dc_info_ptr->snapshot_m);
    }

    if(_DCAMERA_GetFrameFromISP(param_ptr, select_personal, is_bypass_isp,param_ptr->quickview_callback))
    {
        if(DCAMERA_FLASH_AUTO == preview_ctl_info_ptr->flash_mode)
        {
            Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_FALSE);
        }
        _DCAMERA_CloseSensor();
        //DCAMERA_TRACE:"DC: DCAMERA_GetAFrameFromIsp ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10341_112_2_18_1_24_21_524,(uint8*)"");
        DC_RET_ERR(DCAMERA_OP_ISP_ERR)
    }

    // do something after snapshot in sensor layer
    if(!b_burst_snapshot)
    {
        Sensor_Ioctl(SENSOR_IOCTL_AFTER_SNAPSHOT, (uint32)s_dc_info_ptr->preview_m);
    }

    // Close flash
    if(DCAMERA_FLASH_AUTO == preview_ctl_info_ptr->flash_mode)
    {
        Sensor_Ioctl(SENSOR_IOCTL_FLASH, SCI_FALSE);
    }

    //DCAMERA_TRACE:"DC DCAMERA_GetAFrameFromIsp, end time %d  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10353_112_2_18_1_24_21_525,(uint8*)"d" ,SCI_GetTickCount());

	#if 0
    if(_DCAMERA_DoSnapShotReview(param_ptr, &lcd_info, select_personal))
    {
        DC_RET_ERR(DCAMERA_OP_REVIEW_ERR)
    }
    if(_DCAMERA_DoSnapShotRotation(param_ptr, image_size, select_personal))
    {
        DC_RET_ERR(DCAMERA_OP_ROTATION_ERR)
    }
	#endif

    if(DCAMERA_DATA_TYPE_JPEG == param_ptr->data_type)
    {
    	s_dc_info_ptr->snapshot_ctl_info.param  = SNAPSHOT_YUV422;
        if(SNAPSHOT_YUV422 == s_dc_info_ptr->snapshot_ctl_info.param ||
           SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param ||
           ((SNAPSHOT_JPG == s_dc_info_ptr->snapshot_ctl_info.param) && (param_ptr->rotation_mode != DCAMERA_ROTATION_0)))
        {
            _DCAMERA_GetMutex(s_callback_mutex_ptr);

			//SCI_TRACE_LOW("david capture enter jpeg enc ");
			jpeg_enc_out_param.out_size = 1024;
            if(_DCAMERA_DoSnapShotJpgEnc(&jpeg_enc_in_param,
                                         &jpeg_enc_out_param,
                                         param_ptr,
                                         malloc_buffer_ptr,
                                         malloc_buffer_len))
            {
                _DCAMERA_PutMutex(s_callback_mutex_ptr);
                DC_RET_ERR(DCAMERA_OP_ENCODE_ERR)
            }

            _DCAMERA_PutMutex(s_callback_mutex_ptr);

            return_param_ptr->return_data_addr = jpeg_enc_in_param.jpeg_buf_ptr;
            return_param_ptr->return_data_len  = jpeg_enc_out_param.out_size;
        }
        else if(SNAPSHOT_JPG == s_dc_info_ptr->snapshot_ctl_info.param)
        {
            if(s_dc_info_ptr->preview_ctl_info.zoom_level ||
               s_dc_info_ptr->jpeg_rect.w != s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width ||
               s_dc_info_ptr->jpeg_rect.h != s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height)
            {
                _DCAMERA_GetMutex(s_callback_mutex_ptr);
                if(_DCAMERA_DoSnapShotJpgZoom(param_ptr))
                {
                    _DCAMERA_PutMutex(s_callback_mutex_ptr);
                    DC_RET_ERR(DCAMERA_OP_ENCODE_ERR)
                }
                _DCAMERA_PutMutex(s_callback_mutex_ptr);
            }

            return_param_ptr->return_data_addr = s_dc_info_ptr->jpeg_buf;
            return_param_ptr->return_data_len  = s_dc_info_ptr->buf_len;
        }

        //DCAMERA_TRACE:"DC: _DCAMERA_DoSnapShot -> flag"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10408_112_2_18_1_24_21_526,(uint8*)"");
    }
    else if((DCAMERA_DATA_TYPE_YUV422 == param_ptr->data_type))
    {
        if(SNAPSHOT_YUV422 == s_dc_info_ptr->snapshot_ctl_info.param ||
           SNAPSHOT_YUV420 == s_dc_info_ptr->snapshot_ctl_info.param)
        {
            image_size = s_dc_info_ptr->jpeg_rect.w * s_dc_info_ptr->jpeg_rect.h;

            temp_yuv_addr.y_chn_addr = s_dc_info_ptr->isp_frame.yaddr ;
            temp_yuv_addr.u_chn_addr = temp_yuv_addr.y_chn_addr + image_size;

            return_param_ptr->return_data_addr = (uint8*)s_dc_info_ptr->isp_frame.yaddr ;


            if(temp_yuv_addr.u_chn_addr != s_dc_info_ptr->isp_frame.uaddr)
            {
                if(SNAPSHOT_YUV422 == s_dc_info_ptr->snapshot_ctl_info.param)
                {
                    SCI_MEMCPY((void*)temp_yuv_addr.u_chn_addr,
                               (void*)s_dc_info_ptr->isp_frame.uaddr, image_size);
                    return_param_ptr->return_data_len  = (image_size<<1);
                }
                else
                {
                    SCI_MEMCPY((void*)temp_yuv_addr.u_chn_addr,
                               (void*)s_dc_info_ptr->isp_frame.uaddr, (image_size >> 1));
                    return_param_ptr->return_data_len  = (uint32)(image_size*3)>>1;
                }
            }

        }
    }
    else if(DCAMERA_DATA_TYPE_RAW == param_ptr->data_type)
    {
        return_param_ptr->return_data_addr = (uint8*)s_dc_info_ptr->isp_frame.yaddr;
        return_param_ptr->return_data_len  = (s_dc_info_ptr->jpeg_rect.w * s_dc_info_ptr->jpeg_rect.h);
    }
    else
    {
        DC_RET_ERR(DCAMERA_OP_PARAM_ERR)
    }

    if(DCAMERA_OP_NORMAL == s_operation_mode||
       DCAMERA_OP_SPECIAL == s_operation_mode||
       DCAMERA_OP_MINIDC == s_operation_mode)
    {
    	if(b_burst_snapshot == 0)
    	{
        	_DCAMERA_CloseISP();
    	}
    }

    if(b_burst_snapshot == 0)//single snap shot
        b_review_next_to_snapshot = 1;
    else
        b_review_next_to_snapshot = 0;

    ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_DISPLAY);

    if(DCAMERA_IsSupport2MSnapshot() &&
       (s_dc_info_ptr->snapshot_ctl_info.sensor_out_width > VGA_WIDTH) &&
       (s_dc_info_ptr->jpeg_rect.w > VGA_WIDTH))
    {
        DCamera_MemFree_Ybuffer();

        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        LCD_SetOpMode(MAIN_LCD_ID, OSD_OPERATION);
        _DCAMERA_PutMutex(s_callback_mutex_ptr);
    }

    if(param_ptr->snapshot_callback)
    {
          SCI_TRACE_LOW("DC:snapshot_callback time:%u", SCI_GetTickCount());
        _DCAMERA_GetMutex(s_callback_mutex_ptr);
        call_back_return = param_ptr->snapshot_callback(DCAMERA_OP_SUCCESS,
                                      return_param_ptr->return_data_addr,
                                      return_param_ptr->return_data_len);

        _DCAMERA_PutMutex(s_callback_mutex_ptr);
        if(SCI_SUCCESS != call_back_return)
        {
            ret_value = DCAMERA_OP_SNAPSHOT_ERR;
        }
        else
        {
            ret_value = DCAMERA_OP_SUCCESS;
        }
    }

    zoom_level_last_time = DCAMERA_PREVIEW_ZOOM_LEVEL_MAX;
    //DCAMERA_TRACE:"DC: DCAMERA_DoSnapshot -> end time = %d, callback return %d, snapshot return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10485_112_2_18_1_24_21_527,(uint8*)"ddd", SCI_GetTickCount(), call_back_return, ret_value);

    return ret_value;
}

#if 0
/********************************************************************************
Purpose: AlignBuffer,ReadYUVBuffer,SliceYUVBuffer,TargetBuffer,SliceStreamBuffer
         are be used for jpeg encoding. When the picture source is large(such as
         1280X960),the 5 blocks memory is too big. However Scaling buffer can be
         used sometimes instead of AlignBuffer & ReadYUVBuffer.

Author:  Eddy.Wei

Return:  SCI_TRUE: Scaling buffer is used instead of AlignBuffer & ReadYUVBuffer.
         SCI_FALSE:AlignBuffer & ReadYUVBuffer are all be used for JPEG encoding.
*********************************************************************************/
LOCAL uint32 _DCAMERA_IsScalingBufferUsedForJpegEncoding(void)
{
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC8800G)) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)

    if(s_dc_info_ptr->jpeg_rect.w >= 240)
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }

#else

    return SCI_FALSE;

#endif
}
#endif
PUBLIC uint32 DCAMERA_ATVAutoScanChn(DCAMERA_PREVIEW_PARAM_T* param_ptr)
{
    DCAMERA_INFO_T *pDCamInfo=GetDCAMInfo();

    //SCI_TRACE_LOW:"DCAMERA_ATVAutoScanChn: In Time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10524_112_2_18_1_24_21_528,(uint8*)"d", SCI_GetTickCount());

    pDCamInfo->atv_search_callback = param_ptr->dc_atv_callback;

    pDCamInfo->atv_is_autoscan = SCI_TRUE;

    DCAMERA_StartPreview(param_ptr);

    pDCamInfo->atv_is_autoscan = SCI_FALSE;

    DCAMERA_SendSignal(DCAMERA_TASK_CMD_ATV, DCAMERA_ATV_CMD_SCAN_CHN_START, 0, 0);

    //SCI_TRACE_LOW:"DCAMERA_ATVAutoScanChn: Out Time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10536_112_2_18_1_24_21_529,(uint8*)"d", SCI_GetTickCount());

    return SCI_SUCCESS;
}

PUBLIC uint32 DCAMERA_ATVStopScanChn(uint32 (*fun)())
{
    DCAMERA_RETURN_VALUE_E rtn=DCAMERA_OP_SUCCESS;
    DCAMERA_INFO_T *pDCamInfo=GetDCAMInfo();

    //SCI_TRACE_LOW:"DCAM: DCAMERA_ATVStopScanChn: Time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10546_112_2_18_1_24_21_530,(uint8*)"d", SCI_GetTickCount());

    pDCamInfo->atv_stop_callback=fun;

    DCAMERA_SendSignal(DCAMERA_TASK_CMD_ATV, DCAMERA_ATV_CMD_SCAN_CHN_STOP, 0, 0);

    if(PNULL==pDCamInfo->atv_stop_callback)
    {
        if(SCI_SUCCESS!=DCAMERA_GetEvent(DCAMERA_ATV_STOP_EVENT, DCAMERA_WAIT_TIME))
        {
            rtn=DCAMERA_OP_IOCTL_ERR;
        }
    }

    return rtn;
}



PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DisplayOneFrame(DCAMERA_DISPALY_PARAM_T *param_ptr)
{
    ISP_DISPLAY_PARAM_T frame = {0};
    DCAMERA_INFO_T      *dc_info_ptr   = s_dc_info_ptr;
    uint32                      block_id = 0;
    uint32                      rtn = 0;

    //DCAMERA_TRACE:"DCAMERA_DisplayOneFrame"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10570_112_2_18_1_24_21_531,(uint8*)"");

    if(ISP_VT_MEM_MODE_LOW == dc_info_ptr->vt_enc_param_t.mem_mode)
    {
        //search the procing dst buffer and set to IDLE state
        rtn = ISP_ServiceGetVtMemBlockId(&block_id, ISP_VT_MEMBLK_PROCING_SRC);
        if(rtn)
        {
            DCAMERA_TRACE("DC: DisplayOneFrame error1");
            return DCAMERA_OP_MEMORY_ERR;
        }

        DCAMERA_TRACE("DC:DisplayOneFrame, changing block %d state from %d to %d", block_id, ISP_VT_MEMBLK_PROCING_SRC, ISP_VT_MEMBLK_IDLE);
        ISP_ServiceSetVtMemBlockState(block_id, ISP_VT_MEMBLK_IDLE);

        //search the procing dst buffer and set to invalid state
        rtn = ISP_ServiceGetVtMemBlockId(&block_id, ISP_VT_MEMBLK_PROCING_DST);
        if(rtn)
        {
            DCAMERA_TRACE("DC: DisplayOneFrame error2");
            return DCAMERA_OP_MEMORY_ERR;
        }

        DCAMERA_TRACE("DC:DisplayOneFrame, changing block %d state from %d to %d", block_id, ISP_VT_MEMBLK_PROCING_DST, ISP_VT_MEMBLK_INVALIDING);
        ISP_ServiceSetVtMemBlockState(block_id, ISP_VT_MEMBLK_INVALIDING);

        //send buffer free message to isp_service
        ISP_ServiceContinueVTExtLowMem();
    }
    else
    {
        ISP_ServiceFrameUnLock(dc_info_ptr->p_dis_src_frm_free);
        dc_info_ptr->p_dis_src_frm_free = dc_info_ptr->p_dis_src_frm_free->next;
    }

    frame.input_size.w = param_ptr->width;
    frame.input_size.h = param_ptr->height;
    frame.input_addr.yaddr = param_ptr->yaddr;
    frame.input_addr.uaddr = param_ptr->uaddr;
    frame.input_addr.vaddr = param_ptr->vaddr;

    if(DCAMERA_DATA_TYPE_YUV422 == param_ptr->type)
    {
        frame.input_format = ISP_IMAGE_FORMAT_YUV422;
    }
    else
    {
        frame.input_format = ISP_IMAGE_FORMAT_RGB565;
    }
    ISP_ServiceDisplayOneFrame(&frame);

    if(dc_info_ptr->b_is_first_frame)
    {
       //DCAMERA_TRACE:"DCAMERA_DisplayOneFrame,first frame,no need to free frame"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10592_112_2_18_1_24_21_532,(uint8*)"");
       dc_info_ptr->b_is_first_frame = 0;
    }
    else
    {
        if(ISP_VT_MEM_MODE_LOW != dc_info_ptr->vt_enc_param_t.mem_mode)
        {
            //DCAMERA_TRACE:"DCAMERA_DisplayOneFrame, free last frame, 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10597_112_2_18_1_24_21_533,(uint8*)"d",dc_info_ptr->p_dis_dst_frm_free);
            ISP_ServiceFrameUnLock(dc_info_ptr->p_dis_dst_frm_free);
            dc_info_ptr->p_dis_dst_frm_free = dc_info_ptr->p_dis_dst_frm_free->next;
        }

    }

    return DCAMERA_OP_SUCCESS;
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_OpenISP(void)
{
    DCAMERA_RETURN_VALUE_E  rtn = 0;

    _DCAMERA_GetMutex(s_callback_mutex_ptr);

    if(0 == s_isp_opned)
    {
        if(ISP_ServiceOpen(DCAMERA_TIMEOUT_INTERVAL))
        {
            //DCAMERA_TRACE:"_DCAMERA_OpenISP, Failed to Open ISP_Service"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10614_112_2_18_1_24_21_534,(uint8*)"");
            rtn = DCAMERA_OP_ISP_ERR;
        }
        else
        {
            s_isp_opned = 1;
        }
    }

    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    return rtn;
}

LOCAL DCAMERA_RETURN_VALUE_E _DCAMERA_CloseISP(void)
{
    DCAMERA_RETURN_VALUE_E  rtn = 0;

        SCI_TRACE_LOW("DC:CloseISP");

    _DCAMERA_GetMutex(s_callback_mutex_ptr);

    if(s_isp_opned)
    {
        if(ISP_ServiceClose())
        {
            //DCAMERA_TRACE:"_DCAMERA_CloseISP, Failed to Close ISP_Service"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10634_112_2_18_1_24_21_535,(uint8*)"");
            rtn = DCAMERA_OP_ISP_ERR;
        }
        s_isp_opned = 0;
    }

    _DCAMERA_PutMutex(s_callback_mutex_ptr);

    return rtn;
}

LOCAL int _DCAMERA_ISP_REG_ExtCALLBACK(void *flag)
{
    ISP_ADDRESS_T *frame_addr_ptr;
    //DCAMERA_TRACE:"DC: ISP output one frame in capture at %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10646_112_2_18_1_24_21_536,(uint8*)"d",SCI_GetTickCount());

    frame_addr_ptr = (ISP_ADDRESS_T *)flag;

    //DCAMERA_TRACE:"DC: yaddr = 0x%x, uaddr = 0x%x, vaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10651_112_2_18_1_24_21_537,(uint8*)"ddd",frame_addr_ptr->yaddr, frame_addr_ptr->uaddr, frame_addr_ptr->vaddr);

    if(PNULL == s_dc_info_ptr)
        return  SCI_SUCCESS;

    s_dc_info_ptr->isp_frame.yaddr = frame_addr_ptr->yaddr;
    s_dc_info_ptr->isp_frame.uaddr = frame_addr_ptr->uaddr;
    s_dc_info_ptr->isp_frame.vaddr = frame_addr_ptr->vaddr;

    s_dc_operation_flag |= DCAMERA_OP_ISP_CAPTURE_FRAME_FLAG;

    _DCAMERA_CaptureDoneNotice();
    //DCAMERA_TRACE:"DC: Exit ISP CAPTURE FRAME callback at %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10663_112_2_18_1_24_21_538,(uint8*)"d",SCI_GetTickCount());

    return SCI_SUCCESS;
}

LOCAL int _DCAMERA_GetFrameFromIspExt(DCAMERA_EXT_GET_DATA_T *param_ptr )
{
    ISP_JPEG_PARAM_T            *p_cap_para       = &isp_cap_param;
    void*                       mem_start         = 0;
    void*                       mem_end           = 0;
    uint32                      mem_size          = 0;
    uint32                      image_mem_lenth   = 0;
    DCAMERA_SIZE_T              dc_dst_size       = {0};
    DCAMERA_RECT_T              dc_rect           = {0};


    if(SENSOR_MODE_MAX <= s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].mode)
    {
        DCAMERA_TRACE("DC:FromIspExt, error sensor mode!");
        return DCAMERA_OP_ERROR;
    }
    //set get data parameters
    dc_rect.w = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].width;
    dc_rect.h = s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].height;
    p_cap_para->input_size.w   = dc_rect.w;
    p_cap_para->input_size.h   = dc_rect.h;

    dc_dst_size.w = param_ptr->img_size.w;
    dc_dst_size.h = param_ptr->img_size.h;

    //capture rect handle
    _DCAMERA_GetCaptureTrimRect(param_ptr->display_rotation, &dc_dst_size, &dc_rect);

    p_cap_para->input_range.x  = dc_rect.x;
    p_cap_para->input_range.y  = dc_rect.y;
    p_cap_para->input_range.w  = dc_rect.w;
    p_cap_para->input_range.h  = dc_rect.h;

    p_cap_para->output_size.w  = dc_dst_size.w;
    p_cap_para->output_size.h  = dc_dst_size.h;
    p_cap_para->skip_frame_num = 0x03;

    image_mem_lenth = ((p_cap_para->output_size.w * p_cap_para->output_size.h + 255)>>8)<<8;

    mem_start =(void *)(param_ptr->buf_addr);
    mem_end = (void *)(param_ptr->buf_addr+param_ptr->buf_len);
    mem_start = (void *)DC_ADDR(mem_start);
    mem_size  =(uint32)mem_end - (uint32)mem_start;

    if(SENSOR_IMAGE_FORMAT_JPEG != s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format)
    {
        p_cap_para->yuv_type = ISP_IMAGE_FORMAT_YUV422;
        p_cap_para->sensor_data_format = (ISP_IMAGE_FORMAT)s_sensor_ptr->sensor_mode_info[s_dc_info_ptr->snapshot_m].image_format;
    }
    else
    {
        //DCAMERA_TRACE:"DC: _DCAMERA_GetFrameFromIspExt: not support sensor format!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10717_112_2_18_1_24_21_539,(uint8*)"");
            return DCAMERA_OP_SNAPSHOT_ERR;
    }

    if((DCAMERA_DATA_TYPE_JPEG < param_ptr->data_format)&&(DCAMERA_DATA_TYPE_RGB565>param_ptr->data_format))
    {
        p_cap_para->cap_param.cap_img_format = param_ptr->data_format - 1;
    }
    else if(DCAMERA_DATA_TYPE_JPEG == param_ptr->data_format)
    {
        p_cap_para->cap_param.cap_img_format = 0;       //capture YUV data at first
    }
    else
    {
        //DCAMERA_TRACE:"DC: _DCAMERA_GetFrameFromIspExt: not support dcam output format!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10731_112_2_18_1_24_22_540,(uint8*)"");
            return DCAMERA_OP_SNAPSHOT_ERR;
    }


    if(SCI_NULL == mem_start || 0 == mem_size)
    {
        DCAMERA_TRACE("DC:FromIspExt, not enough memory!");
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }
    //data capture handle
    //ISP_ServiceMemConfig((void*)mem_start, mem_size);
    ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, _DCAMERA_ISP_REG_ExtCALLBACK);

    DCAMERA_SetCapParam(&p_cap_para->cap_param);

    if(ISP_ServiceSetJpegCaptureParam(p_cap_para))
    {
        DCAMERA_TRACE("DC:FromIspExt,  error2");
        return DCAMERA_OP_ISP_ERR;
    }
    ISP_ServiceStartCaptureJpeg();

    if(_DCAMERA_WaitForCaptureDone())
    {
        DCAMERA_TRACE("DC:FromIspExt, capture timeout");
        return DCAMERA_OP_ISP_ERR;
    }

    //data handle after capture
    s_dc_info_ptr->isp_frame.width    = 640; //p_cap_para->output_size.w;
    s_dc_info_ptr->isp_frame.height   = 480; //p_cap_para->output_size.h;

    //is need to display then review the image
    if(param_ptr->is_display)
    {
        ISP_REVIEW_PARAM_T review_param = {0};

        review_param.input_size.w = s_dc_info_ptr->isp_frame.width;
        review_param.input_size.h = s_dc_info_ptr->isp_frame.height;
        review_param.input_range.x = 0;
        review_param.input_range.y = 0;
        review_param.input_range.w = review_param.input_size.w;
        review_param.input_range.h = review_param.input_size.h;
        review_param.input_format = ISP_IMAGE_FORMAT_YUV422;
        review_param.input_addr.yaddr = s_dc_info_ptr->isp_frame.yaddr;
        review_param.input_addr.uaddr = s_dc_info_ptr->isp_frame.uaddr;
        review_param.disp_lcd_id = param_ptr->lcd_id;
        review_param.disp_block_id = 0;          //? TBD

        review_param.lcd_rect.x = param_ptr->display_rect.x;
        review_param.lcd_rect.y = param_ptr->display_rect.y;
        review_param.lcd_rect.w = param_ptr->display_rect.w;
        review_param.lcd_rect.h = param_ptr->display_rect.h;
        review_param.disp_range.x = param_ptr->target_rect.x;
        review_param.disp_range.y = param_ptr->target_rect.y;
        review_param.disp_range.w = param_ptr->target_rect.w;
        review_param.disp_range.h = param_ptr->target_rect.h;
        review_param.disp_rotation = (ISP_ROTATION_E)(param_ptr->display_rotation);

        LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);
        SCI_TRACE_LOW("DC:FromIspExt NO_PERMIT_GUI_INVALIDATE");
        //review memory handle
        mem_start = (void *)((uint32)mem_start + (image_mem_lenth + 512)*2);

        //set the review buffer, now use the given buffer
        ISP_ServiceMemConfig((void*)mem_start, mem_size);

        if(ISP_ServiceSetReviewParam(&review_param))
        {
            DCAMERA_TRACE("DC:FromIspExt, error2");
            return DCAMERA_OP_PARAM_ERR;
        }

        ISP_ServiceStartReview();

        LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
    }

    //if jpeg then encode the data
    if(DCAMERA_DATA_TYPE_JPEG == param_ptr->data_format)
    {
        JINF_ENC_IN_PARAM_T JpgEnInPara = {0};
        JINF_ENC_OUT_PARAM_T JpgEnOutPara = {0};

        JpgEnInPara.target_buf_ptr = (uint8 *)(((param_ptr->buf_addr + param_ptr->buf_len - image_mem_lenth)<<2)>>2);   //ALLOC the jpeg jpg buffer
        JpgEnInPara.target_buf_size = param_ptr->buf_len - ((uint32)JpgEnInPara.target_buf_ptr - param_ptr->buf_addr);
        SCI_MEMSET(JpgEnInPara.target_buf_ptr, 0, JpgEnInPara.target_buf_size);

        //encoding jpeg in frame mode
        JpgEnInPara.yuv_info.yuv_chn.y_chn_ptr=(uint8 *)s_dc_info_ptr->isp_frame.yaddr;
        JpgEnInPara.yuv_info.yuv_chn.u_chn_ptr=(uint8 *)s_dc_info_ptr->isp_frame.uaddr;
        JpgEnInPara.yuv_info.sample_format=JINF_FORMAT_YUV422;
        JpgEnInPara.yuv_info.data_pattern=JINF_YUV_PATTERN_CHN_Y_UV;
        JpgEnInPara.yuv_info.data_endian=JINF_DATA_BIG_ENDIAN;

        JpgEnInPara.src_width=s_dc_info_ptr->isp_frame.width;
        JpgEnInPara.src_height=s_dc_info_ptr->isp_frame.height;;
        JpgEnInPara.quality=JINF_QUALITY_MIDDLE_LOW;/*lint !e64*///JINF_QUALITY_MIDDLE_LOW;

        JpgEnInPara.encode_buf_size = DC_JPEG_TMP_BUF_LEN;
        JpgEnInPara.encode_buf_ptr = (uint8 *)((uint32)JpgEnInPara.target_buf_ptr - DC_JPEG_TMP_BUF_LEN);
        if(((uint32)JpgEnInPara.encode_buf_ptr - (uint32)JpgEnInPara.yuv_info.yuv_chn.y_chn_ptr) < (image_mem_lenth*2))
        {
            //DCAMERA_TRACE:"DC: DCAMERA_ExtDisplay, not enough memory error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10830_112_2_18_1_24_22_542,(uint8*)"");
            return DCAMERA_OP_NO_ENOUGH_MEMORY;

        }

        JpgEnInPara.read_yuv_func=PNULL;
        JpgEnInPara.write_file_func=PNULL;
        JpgEnInPara.async_callback=PNULL;

        if(IMGJPEG_Encode(&JpgEnInPara, &JpgEnOutPara))      //encode jpeg
        {
            DCAMERA_TRACE("DC:FromIspExt, jpeg ecode err");
            return DCAMERA_OP_DECODE_ERR;
        }
        s_dc_info_ptr->jpeg_buf = JpgEnOutPara.jpeg_buf_ptr;
        s_dc_info_ptr->buf_len = JpgEnOutPara.jpeg_buf_size;

    }

    return DCAMERA_OP_SUCCESS;
}


/******************************************************************************/
// Description: Third part open sensor
// Input:      DCAMERA_IMG_SENSOR_MODE : img sensor mode (main or sub)
//               DCAMERA_PARAM_VIDEO_MODE_E work_mode      (normal, video or UPCC)
// Output:     none
// Return:     SCI_TRUE    open success
//                other refer the error code
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_ExtOpenSensor(DCAMERA_IMG_SENSOR_MODE img_sensor_mode,
                                    DCAMERA_PARAM_VIDEO_MODE_E work_mode)
{
//    SENSOR_REGISTER_INFO_T_PTR p_reg_info = SCI_NULL;
    uint32 img_sensor_num=0x00;
    uint32 cur_img_sensor_id=0x00;

    //DCAMERA_TRACE:"DC: DCAMERA_ExtOpenSensor start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10865_112_2_18_1_24_22_543,(uint8*)"");

    _DCAMERA_ContextInit();

    //active ahb setting
    SCI_DCAM_EnableDeepSleep(DISABLE_AHB_SLEEP);

    if(img_sensor_mode > DCAMERA_IMG_SENSOR_SUB)
    {
        _DCAMERA_ContextDeInit();
        return DCAMERA_OP_PARAM_ERR;
    }
    else
    {
        s_sensor_id = img_sensor_mode;
        s_operation_mode = DCAMERA_OP_NORMAL;
    }

    //initial the camera
    if(_DCAMERA_InitSensor())
    {
        _DCAMERA_ContextDeInit();
        return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
    }

    //get the camera infomation
    DCAMERA_GetInfo(DCAMERA_INFO_IMG_SENSOR_NUM, &img_sensor_num);

    if(0x01<img_sensor_num)
    {
        DCAMERA_GetInfo(DCAMERA_INFO_CUR_IMG_SENSOR, &cur_img_sensor_id);

        if(img_sensor_mode!=cur_img_sensor_id)
        {
            Sensor_Ioctl( DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE, img_sensor_mode);
        }
    }
    //sensor preview mode set
    s_dc_info_ptr->snapshot_m = SENSOR_MODE_PREVIEW_ONE;
    Sensor_SetMode(s_dc_info_ptr->snapshot_m);

    //DCAMERA_TRACE:"DC: DCAMERA_ExtOpenSensor accomplish!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10905_112_2_18_1_24_22_544,(uint8*)"");

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: Third part close sensor
// Input:      none
// Output:     none
// Return:     SCI_TRUE    close success
//                other refer the error code
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_ExtCloseSensor(void)
{
    uint32 rtn = DCAMERA_OP_SUCCESS;
    //DCAMERA_TRACE:"DC: DCAMERA_ExtCloseSensor start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10919_112_2_18_1_24_22_545,(uint8*)"");
    _DCAMERA_GetMutex(s_callback_mutex_ptr);
    rtn = Sensor_Close();
    _DCAMERA_PutMutex(s_callback_mutex_ptr);
    SCI_DCAM_EnableDeepSleep(ENABLE_AHB_SLEEP);
    _DCAMERA_ContextDeInit();

    //DCAMERA_TRACE:"DC: DCAMERA_ExtCloseSensor accomplish!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10926_112_2_18_1_24_22_546,(uint8*)"");
    return rtn;
}



/******************************************************************************/
// Description: Third part get img data
// Input:       DCAMERA_EXT_GET_DATA_T_PTR : the pointer of get data param
// Output:      DCAMERA_YUV_CHN_ADDR_T : image addr
//              DCAMERA_SIZE_T : img size
// Return:      SCI_SUCCESS    get data success
//                other refer the error code
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_ExtGetData(DCAMERA_EXT_GET_DATA_T_PTR get_param,
                                 DCAMERA_YUV_CHN_ADDR_T* rtn_img_addr,
                                 DCAMERA_SIZE_T* rtn_size)
 {
    uint32 rtn = DCAMERA_OP_SUCCESS;
//    DCAMERA_QUICK_VIEW_CALL_BACK quickview_cb = PNULL;

    //DCAMERA_TRACE:"DC: DCAMERA_ExtGetData start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10946_112_2_18_1_24_22_547,(uint8*)"");

    //check the parameter
    if(PNULL == get_param)
    {
        //DCAMERA_TRACE:"DC: DCAMERA_ExtGetData: error input parameter!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10951_112_2_18_1_24_22_548,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    //set the get data parametes
    s_dc_info_ptr->snapshot_ctl_info.param = SNAPSHOT_YUV422;
    s_dc_info_ptr->snapshot_m = SENSOR_MODE_PREVIEW_ONE;

    //open isp service
    if(_DCAMERA_OpenISP())
    {
        //DCAMERA_TRACE:"DC: DCAMERA_ExtGetData isp_service open  error quit!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10961_112_2_18_1_24_22_549,(uint8*)"");
        return DCAMERA_OP_ERROR;
    }

	_DCAMERA_CloseISP();
	//ISP_UixEnableCaptureMode(1);
    ISP_UixSetDcamMode(DCAMERA_MODE_SNAPSHOT);
    rtn = _DCAMERA_OpenISP();
    if(SCI_SUCCESS != rtn)
    {
        _DCAMERA_Close();
        return rtn;
    }
        //SCI_TRACE_LOW("david capture reopen Camera controller over!");

    //start capture data
    rtn = _DCAMERA_GetFrameFromIspExt(get_param);
    if(DCAMERA_OP_SUCCESS != rtn)
    {
        //DCAMERA_TRACE:"DC: DCAMERA_ExtGetData failed from sensor!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10968_112_2_18_1_24_22_550,(uint8*)"");
    }

    //close isp service
    if(_DCAMERA_CloseISP())
    {
        //DCAMERA_TRACE:"DC: DCAMERA_ExtGetData isp_service close  error quit!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10976_112_2_18_1_24_22_551,(uint8*)"");
        return DCAMERA_OP_ERROR;
    }

    if(DCAMERA_OP_SUCCESS == rtn)
    {
        //data output
        if(DCAMERA_DATA_TYPE_JPEG==get_param->data_format)
        {
            rtn_img_addr->y_chn_addr = (uint32)s_dc_info_ptr->jpeg_buf;
            rtn_img_addr->u_chn_addr = NULL;
            rtn_size->w = s_dc_info_ptr->buf_len;
            rtn_size->h = 0x00;
        }
        else
        {
            rtn_img_addr->y_chn_addr = s_dc_info_ptr->isp_frame.yaddr;
            rtn_img_addr->u_chn_addr = s_dc_info_ptr->isp_frame.uaddr;
            rtn_size->w = s_dc_info_ptr->isp_frame.width;
            rtn_size->h = s_dc_info_ptr->isp_frame.height;
        }

        //DCAMERA_TRACE:"DC: DCAMERA_ExtGetData accomplished!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_10996_112_2_18_1_24_22_552,(uint8*)"");
    }

    return rtn;
}


/******************************************************************************/
// Description: Third part backup lcd display buffer
// Input:       uint32 lcd_id,              LCD ID
//              uint32 block_id,            block id
//              uint32 disp_buf,            buffer used for backup data
//              uint32 disp_buf_len         backup buffer length
//
// Output:     none
// Return:     DCAMERA_OP_SUCCESS    set success
//
// Note:
/******************************************************************************/
LOCAL void _DCAMERA_BackupReviewbuf(uint32 lcd_id, uint32 block_id, uint32 disp_buf, uint32 disp_buf_len)
{
    uint32          disp_buf_size = 0;
    BLOCKCFG_T      blk_info = {0};
    uint32          aligned_addr = 0;
    uint32          block_buf = 0;
    uint32          end_addr = disp_buf + disp_buf_len;
    uint8           rev_fmt = LCD_RESOLUTION_YUV422;

    LCD_GetLcdCtrlSemaphore(0);

    block_buf = (uint32)LCD_GetBlockBuffer(lcd_id, block_id);
    LCD_GetConfigBlock(lcd_id,block_id,&blk_info);
    rev_fmt = blk_info.resolution;
    //DCAMERA_TRACE:"DC: _DCAMERA_BackupReviewbuf idth %d blk_info.end_y %d blk_info.start_y %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11029_112_2_18_1_24_22_553,(uint8*)"ddd",blk_info.width,blk_info.end_y,blk_info.start_y);

    disp_buf_size = (uint32)((uint32)blk_info.width * (uint32)((uint32)blk_info.end_y - (uint32)blk_info.start_y + 1));

    //DCAMERA_TRACE:"DC: _DCAMERA_BackupReviewbuf dst 0x%x, length %d, src y 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11036_112_2_18_1_24_22_554,(uint8*)"ddd",disp_buf,disp_buf_size,block_buf);

    aligned_addr = DC_ADDR(disp_buf);

    if(rev_fmt == LCD_RESOLUTION_YUV422)
    {
        if(aligned_addr + disp_buf_size <= (end_addr - disp_buf_size))
        {
            SCI_MEMCPY((void*)aligned_addr, (void*)block_buf, disp_buf_size);
            LCD_SetBlockBuffer(lcd_id,block_id,(uint32*)aligned_addr);
            block_buf = (uint32)LCD_GetBlockUVBuffer(lcd_id, block_id);
            aligned_addr += disp_buf_size;
            aligned_addr = DC_ADDR(aligned_addr);
            if(aligned_addr + disp_buf_size <= end_addr)
            {
                SCI_MEMCPY((void*)aligned_addr, (void*)block_buf, disp_buf_size);
                LCD_SetUVBuffer(lcd_id,block_id,(uint32*)aligned_addr);
            }
        }
    }
    else
    {
        disp_buf_size = disp_buf_size << 1;
        if(aligned_addr + disp_buf_size <= end_addr)
        {
            SCI_MEMCPY((void*)aligned_addr, (void*)block_buf, disp_buf_size);
            LCD_SetBlockBuffer(lcd_id,block_id,(uint32*)aligned_addr);
        }
    }

    LCD_PutLcdCtrlSemaphore(0);

    return ;
}

/******************************************************************************/
// Description: Third part display data
// Input:       DCAMERA_EXT_DISPLAY_T_PTR : the pointer of display param
// Output:     none
// Return:     SCI_SUCCESS    display success
//             SCI_ERROR      display fail
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_ExtDisplay(DCAMERA_EXT_DISPLAY_T_PTR display_param )
{
    uint32                      rtn = DCAMERA_OP_SUCCESS;
    void*                       mem_start    = PNULL;
    uint32                      mem_size     = 0;
    ISP_REVIEW_PARAM_T          review_param  = {0};

    //DCAMERA_TRACE:"DC: DCAMERA_ExtDisplay start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11084_112_2_18_1_24_22_555,(uint8*)"");

    //set parameter
    review_param.disp_lcd_id = display_param->lcd_id;
    review_param.disp_block_id = 0;          //? TBD
    review_param.lcd_rect.x = DCAMERA_WIDTH(display_param->display_rect.x);
    review_param.lcd_rect.y = DCAMERA_HEIGHT(display_param->display_rect.y);
    review_param.lcd_rect.w = DCAMERA_WIDTH(display_param->display_rect.w);
    review_param.lcd_rect.h = DCAMERA_HEIGHT(display_param->display_rect.h);
    review_param.disp_range.x = DCAMERA_WIDTH(display_param->target_rect.x);
    review_param.disp_range.y = DCAMERA_HEIGHT(display_param->target_rect.y);
    review_param.disp_range.w = DCAMERA_WIDTH(display_param->target_rect.w);
    review_param.disp_range.h = DCAMERA_HEIGHT(display_param->target_rect.h);
    review_param.disp_rotation = (ISP_ROTATION_E)(display_param->display_rotation);

    //if data is JPEG format then decode that to YUV
    if(DCAMERA_DATA_TYPE_JPEG == display_param->src_format)
    {
        JINF_DEC_IN_PARAM_T         jpg_dec_in_para  = {0};
        JINF_DEC_OUT_PARAM_T        jpg_dec_out_para = {0};
        JINF_INFO_T                 jpeg_info = {0};
        JINF_GET_INFO_IN_T          jpeg_get_info_in = {0};

        //DCAMERA_TRACE:"DC: DCAMERA_ExtDisplay: jpeg input, need to decoide at first!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11107_112_2_18_1_24_22_556,(uint8*)"");

        jpeg_get_info_in.jpeg_buf_ptr = (uint8 *)display_param->src_jpg_addr;
        jpeg_get_info_in.jpeg_buf_size = display_param->src_jpg_buf_len;

        IMGJPEG_GetInfo(&jpeg_get_info_in,&jpeg_info);

        SCI_MEMSET(&jpg_dec_in_para, 0x0, sizeof(JINF_DEC_IN_PARAM_T));
        SCI_MEMSET(&jpg_dec_out_para, 0x0, sizeof(JINF_DEC_OUT_PARAM_T));

        //this part should be modify after crop and scale of jpeg decoder are completed
        if(jpeg_info.image_width%review_param.disp_range.w
            ||jpeg_info.image_height%review_param.disp_range.h)
        {
            jpg_dec_in_para.target_width  = DCAMERA_MAX((jpeg_info.image_width/4),QVGA_WIDTH);
            jpg_dec_in_para.target_height = DCAMERA_MAX((jpeg_info.image_height/4),QVGA_HEIGHT);
        }
        else
        {
            jpg_dec_in_para.target_width  = review_param.disp_range.w;
            jpg_dec_in_para.target_height = review_param.disp_range.h;
        }

        jpg_dec_in_para.jpeg_buf_ptr    = (uint8 *)display_param->src_jpg_addr;
        jpg_dec_in_para.jpeg_buf_size   = display_param->src_jpg_buf_len;
        jpg_dec_in_para.crop_rect.x     = 0;
        jpg_dec_in_para.crop_rect.y     = 0;
        jpg_dec_in_para.crop_rect.w     = jpeg_info.image_width;
        jpg_dec_in_para.crop_rect.h     = jpeg_info.image_height;
        jpg_dec_in_para.decode_buf_ptr  = (uint8 *)display_param->jpg_decoder_addr;
        jpg_dec_in_para.decode_buf_size = display_param->src_decoder_buf_len;
        jpg_dec_in_para.target_buf_ptr  = (uint8 *)display_param->display_addr;
        jpg_dec_in_para.target_buf_size = display_param->display_buf_len;
        jpg_dec_in_para.target_data_type=JINF_DATA_TYPE_YUV;
        jpg_dec_in_para.read_file_func=PNULL;
        jpg_dec_in_para.async_callback=PNULL;

        //decode the inout jpeg image
        if(IMGJPEG_DecodeEx(&jpg_dec_in_para, &jpg_dec_out_para))
        {
            //DCAMERA_TRACE:"DC: DCAMERA_ExtDisplay decode jpeg error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11147_112_2_18_1_24_22_557,(uint8*)"");
            return DCAMERA_OP_DECODE_ERR;
        }

        review_param.input_addr.yaddr = (uint32)jpg_dec_out_para.out_data_info.yuv.yuv_chn.y_chn_ptr;
        review_param.input_addr.uaddr = (uint32)jpg_dec_out_para.out_data_info.yuv.yuv_chn.u_chn_ptr;
        review_param.input_addr.vaddr = (uint32)jpg_dec_out_para.out_data_info.yuv.yuv_chn.v_chn_ptr;
        review_param.input_size.w     = jpg_dec_out_para.output_width;
        review_param.input_size.h     = jpg_dec_out_para.output_height;
        review_param.input_format     = ISP_IMAGE_FORMAT_YUV422;
    }
    else
    {
        review_param.input_format = (ISP_IMAGE_FORMAT)display_param->src_format;
        review_param.input_addr.yaddr = display_param->src_yuv_addr.y_chn_addr;
        review_param.input_addr.uaddr = display_param->src_yuv_addr.u_chn_addr;
        review_param.input_addr.vaddr = display_param->src_yuv_addr.v_chn_addr;
        review_param.input_size.w = display_param->src_img_size.w;
        review_param.input_size.h = display_param->src_img_size.h;
    }

    //set the review buffer, now use the dc display buffer
    mem_start = (void *)DCamera_MemConfig(DCAMERA_OP_MINIDC, &mem_size);

    if(PNULL == mem_start || 0 == mem_size)
    {
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }

    //open isp
    if(_DCAMERA_OpenISP())
    {
        //DCAMERA_TRACE:"DC: DCAMERA_ExtDisplay isp_service open error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11171_112_2_18_1_24_22_558,(uint8*)"");
        DCamera_MemFree();
        return DCAMERA_OP_ERROR;
    }

    ISP_ServiceMemConfig((void*)mem_start, mem_size);
    //display handle
    review_param.input_range.x = 0;
    review_param.input_range.y = 0;
    review_param.input_range.w = review_param.input_size.w;
    review_param.input_range.h = review_param.input_size.h;

    rtn = ISP_ServiceSetReviewParam(&review_param);

    if(DCAMERA_OP_SUCCESS == rtn)
    {
        ISP_ServiceStartReview();
    }
    else
    {
            DCAMERA_TRACE("DC:ExtDisplay, set para or memory err");
    }

    //close isp
    if(_DCAMERA_CloseISP())
    {
        //DCAMERA_TRACE:"DC: DCAMERA_ExtDisplay isp_service close error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11193_112_2_18_1_24_22_559,(uint8*)"");
    }

    if(DCAMERA_OP_SUCCESS == rtn)
    {
        _DCAMERA_BackupReviewbuf(review_param.disp_lcd_id,
                                 review_param.disp_block_id,
                                 display_param->display_addr,
                                 display_param->display_buf_len);
    }

    DCamera_MemFree();

    //DCAMERA_TRACE:"DC: DCAMERA_ExtDisplay accomplished!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11203_112_2_18_1_24_22_560,(uint8*)"");

    return rtn;
}

/******************************************************************************/
// Description: Third part display data
// Input:       DC_DPLAYER_DISPLAY_PARAM_T *pst_param: video diplay set parameter pointer
// Output:     none
// Return:     DCAMERA_OP_SUCCESS    set success
//
// Note:
/******************************************************************************/
PUBLIC uint32 DC_VideoSetDisplayParam(const DC_DPLAYER_DISPLAY_PARAM_T *pst_param)
{
    uint32 rtn = DCAMERA_OP_SUCCESS;
    ISP_REVIEW_PARAM_T *p_review_param = &s_review_param;

    //DCAMERA_TRACE:"DC: DC_VideoSetDisplayParam started!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11218_112_2_18_1_24_22_561,(uint8*)"");

    p_review_param->input_size.w = pst_param->src_width;
    p_review_param->input_size.h = pst_param->src_height;
    p_review_param->disp_lcd_id = pst_param->i_lcd;
    p_review_param->disp_block_id = 0x0;
    p_review_param->disp_rotation = (ISP_ROTATION_E)pst_param->rotation;

    //set rects paramters
    p_review_param->disp_range.x = pst_param->target_rect.x;
    p_review_param->disp_range.y = pst_param->target_rect.y;
    p_review_param->disp_range.w = pst_param->target_rect.w;
    p_review_param->disp_range.h = pst_param->target_rect.h;

    p_review_param->lcd_rect.x = pst_param->st_disp_rect.x;
    p_review_param->lcd_rect.y = pst_param->st_disp_rect.y;
    p_review_param->lcd_rect.w = pst_param->st_disp_rect.w;
    p_review_param->lcd_rect.h = pst_param->st_disp_rect.h;

    if(0 == pst_param->st_trim_rect.h || 0 == pst_param->st_trim_rect.w)
    {
        p_review_param->input_range.x = 0;
        p_review_param->input_range.y = 0;
        p_review_param->input_range.w = p_review_param->input_size.w;
        p_review_param->input_range.h = p_review_param->input_size.h;
    }
    else
    {
        p_review_param->input_range.x = pst_param->st_trim_rect.x;
        p_review_param->input_range.y = pst_param->st_trim_rect.y;
        p_review_param->input_range.w = pst_param->st_trim_rect.w;
        p_review_param->input_range.h = pst_param->st_trim_rect.h;
    }
    //set and store the video parameter
    DCAMERA_AdjustDisplayRect(pst_param->i_lcd,
                              &p_review_param->input_range,
                              &p_review_param->lcd_rect,
                              &p_review_param->disp_range,
                              pst_param->disp_mode,
                              pst_param->rotation);

    //memory set
    ISP_ServiceMemConfig((void*)pst_param->video_buf_addr, pst_param->video_buf_len);

    ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ENCODE, (int(*)())pst_param->review_callback);

    //DCAMERA_TRACE:"DC: DC_VideoSetDisplayParam accomplished!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11255_112_2_18_1_24_23_562,(uint8*)"");

    return rtn;
}

/******************************************************************************/
// Description: Third part display data
// Input:       DISPLAY_DATA_FRAME_T *p_frame : display data address
// Output:     none
// Return:     DCAMERA_OP_SUCCESS    display success
//                SCI_ERROR      display fail
// Note:
/******************************************************************************/
PUBLIC uint32 DC_DisplayAFrame(DISPLAY_DATA_FRAME_T *p_frame)
{
    uint32 rtn = DCAMERA_OP_SUCCESS;
    ISP_REVIEW_PARAM_T *p_review_param = &s_review_param;

    //DCAMERA_TRACE:"DC: DC_DisplayAFrame started!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11271_112_2_18_1_24_23_563,(uint8*)"");

    //preview handle
    p_review_param->input_addr.yaddr = (uint32)p_frame->y_addr;
    p_review_param->input_addr.uaddr = (uint32)p_frame->uv_addr;
    p_review_param->input_addr.vaddr = (uint32)p_frame->uv1_addr;

    //display format set
    if(JINF_FORMAT_YUV422==p_frame->yuv_type)
    {
        p_review_param->input_format = ISP_IMAGE_FORMAT_YUV422;
    }
    else if(JINF_FORMAT_YUV420==p_frame->yuv_type)
    {
        if(YUV_PLANE_2 == p_frame->yuv_plane)
        {
            p_review_param->input_format = ISP_IMAGE_FORMAT_YUV420;
        }
        else
        {
            p_review_param->input_format = ISP_IMAGE_FORMAT_YUV420_3FRAME;
        }
    }
    else if(JINF_FORMAT_YUV400 == p_frame->yuv_type)
    {
        p_review_param->input_format = ISP_IMAGE_FORMAT_YUV400;
    }
    else
    {
        p_review_param->input_format = p_frame->yuv_type;
    }

    if(_DCAMERA_OpenISP())
    {
        rtn =  DCAMERA_OP_ERROR;
    }

    //set review parameter
    if(!ISP_ServiceSetReviewParam(&s_review_param))
    {
        ISP_ServiceStartReview();
    }
    else
    {
        DCAMERA_TRACE("DC: DC_DisplayAFrame error");
    }

    //close the isp_service
    if(_DCAMERA_CloseISP())
    {
        rtn =  DCAMERA_OP_ERROR;
    }

    //DCAMERA_TRACE:"DC: DC_DisplayAFrame accomplished!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_APP_11320_112_2_18_1_24_23_564,(uint8*)"");
    return rtn;
}

/*****************************************************************************/
//  Description : open the display module.
//  input       : none
//  output      : SCI_SUCCESS of SCI_ERROR.
//  Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_Display_Open(void)
{
    //DCAMERA_TRACE("Video_Display_Open");
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description : close the display module.
//  input       : none
//  output      : SCI_SUCCESS of SCI_ERROR.
//  Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_Display_Close(void)
{
    DCAMERA_TRACE("Video_Display_Close");
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description : set the display paramter,angle and rect in virtual window,
//                the paramter will be active when next frame is displaying.
//  input       :
//  output      : SCI_SUCCESS or SCI_ERROR.
//  Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_Display_Setparam(VIDEO_DISP_PARAM_T *p_disp_param)
{
    //initialize
    VIDEO_DISP_PARAM_T *p_video_disp_param = &s_video_disp_param;

    //DCAMERA_TRACE("Video_Display_Setparam");

    //review parameter record
    SCI_MEMSET((void*)p_video_disp_param, 0, sizeof(VIDEO_DISP_PARAM_T));
    *p_video_disp_param = *p_disp_param;

    return SCI_SUCCESS;
}

LOCAL void _Video_Display_SetFrame(const VIDEO_DISP_FRAME_T *p_frame, DISPLAY_DATA_FRAME_T * p_display_frameDate)
{

    if (DISP_DATATYP_YUV420_2PANNEL == p_frame->data_type)
    {
        p_display_frameDate->yuv_type = JINF_FORMAT_YUV420;
        p_display_frameDate->y_addr = p_frame->data.yuv2pannel.p_y;
        p_display_frameDate->uv_addr = p_frame->data.yuv2pannel.p_uv;
        p_display_frameDate->y_data_len = p_frame->dec_out_w * p_frame->dec_out_h;
        p_display_frameDate->uv_data_len =  p_display_frameDate->y_data_len >> 1;
        p_display_frameDate->yuv_plane = YUV_PLANE_2;
    }
    else if (DISP_DATATYP_YUV422_2PANNEL == p_frame->data_type)
    {
        p_display_frameDate->yuv_type = JINF_FORMAT_YUV422;
        p_display_frameDate->y_addr = p_frame->data.yuv2pannel.p_y;
        p_display_frameDate->uv_addr = p_frame->data.yuv2pannel.p_uv;
        p_display_frameDate->y_data_len = p_frame->dec_out_w * p_frame->dec_out_h;
        p_display_frameDate->uv_data_len =  p_display_frameDate->y_data_len;
        p_display_frameDate->yuv_plane = YUV_PLANE_2;
    }
    else if (DISP_DATATYP_YUV420_3PANNEL == p_frame->data_type)
    {
        p_display_frameDate->yuv_type = JINF_FORMAT_YUV420;
        p_display_frameDate->y_addr = p_frame->data.yuv3pannel.p_y;
        p_display_frameDate->uv_addr = p_frame->data.yuv3pannel.p_u;
        p_display_frameDate->uv1_addr = p_frame->data.yuv3pannel.p_v;
        p_display_frameDate->y_data_len = p_frame->dec_out_w * p_frame->dec_out_h;
        p_display_frameDate->uv_data_len =  p_display_frameDate->y_data_len >> 2;
        p_display_frameDate->uv1_data_len =  p_display_frameDate->y_data_len >> 2;
        p_display_frameDate->yuv_plane = YUV_PLANE_3;
    }
    else if (DISP_DATATYP_YUV422_3PANNEL == p_frame->data_type)
    {
        p_display_frameDate->yuv_type = JINF_FORMAT_YUV422;
        p_display_frameDate->y_addr = p_frame->data.yuv3pannel.p_y;
        p_display_frameDate->uv_addr = p_frame->data.yuv3pannel.p_u;
        p_display_frameDate->uv1_addr = p_frame->data.yuv3pannel.p_v;
        p_display_frameDate->y_data_len = p_frame->dec_out_w * p_frame->dec_out_h;
        p_display_frameDate->uv_data_len =  p_display_frameDate->y_data_len >> 1;
        p_display_frameDate->uv1_data_len =  p_display_frameDate->y_data_len >> 1;
        p_display_frameDate->yuv_plane = YUV_PLANE_3;
    }
    else
    {
        DCAMERA_TRACE("DC:not support yuv%d", p_frame->data_type);
        p_display_frameDate->yuv_type = JINF_FORMAT_YUV420;
    }

    return;
}

/*****************************************************************************/
//  Description : display one frame.
//  input       : p_frame,the info of the dec_out frame.
//  output      : SCI_SUCCESS or SCI_ERROR.
//  Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_Display_Oneframe(VIDEO_DISP_FRAME_T *p_frame)
{
    int                                              result                       = SCI_SUCCESS;
    DC_DPLAYER_DISPLAY_PARAM_T      st_param                 = {0};
    VIDEO_DISP_PARAM_T                  *p_video_disp_param = &s_video_disp_param;
    DISPLAY_DATA_FRAME_T                tDisplayFrameDate = {0};

    if(( PNULL == p_frame ) ||
        ( (0 == p_frame->org_w) ||  (0 == p_frame->org_h ) ) ||
        ( PNULL == p_frame->data.yuv3pannel.p_y))
    {
        DCAMERA_TRACE("DC:Video_Disp,p_frame == PNULL");
        return SCI_ERROR;
    }


    if( (0 == p_frame->org_w) ||  (0 == p_frame->org_h ) )
    {
        DCAMERA_TRACE("DC:Video_Disp,w(%d) or h(%d) is zero!",  p_frame->org_w,  p_frame->org_h);
        return SCI_ERROR;
    }

    //set video display parameter
    st_param.i_lcd  = p_video_disp_param->LcdId;
    st_param.st_disp_rect.x = (uint16)p_video_disp_param->disp_rect.x;
    st_param.st_disp_rect.y = (uint16)p_video_disp_param->disp_rect.y;
    st_param.st_disp_rect.w = (uint16)p_video_disp_param->disp_rect.dx;
    st_param.st_disp_rect.h = (uint16)p_video_disp_param->disp_rect.dy;
    st_param.target_rect.x = (uint16)p_video_disp_param->target_rect.x;
    st_param.target_rect.y = (uint16)p_video_disp_param->target_rect.y;
    st_param.target_rect.w = (uint16)p_video_disp_param->target_rect.dx;
    st_param.target_rect.h = (uint16)p_video_disp_param->target_rect.dy;
    st_param.rotation = (DCAMERA_ROTATION_MODE_E)p_video_disp_param->RotateAngle;
    st_param.disp_mode = (DCAMERA_DISP_MODE_E)p_video_disp_param->disp_mode;

    st_param.review_callback = PNULL;

    st_param.src_width = (uint16)p_frame->dec_out_w;
    st_param.src_height = (uint16)p_frame->dec_out_h;

    st_param.target_rect.w = (uint16)p_frame->org_w;
    st_param.target_rect.h = (uint16)p_frame->org_h;

    if(st_param.target_rect.w > st_param.src_width ||st_param.target_rect.h > st_param.src_height)
    {
        DCAMERA_TRACE("DC:Video_Disp error1");
        return SCI_ERROR;
    }
    else
    {
        st_param.target_rect.x = ((st_param.src_width  - st_param.target_rect.w)>>1)&0xFFFC;
        st_param.target_rect.y = ((st_param.src_height - st_param.target_rect.h)>>1)&0xFFFE;
    }

    st_param.video_buf_addr = PNULL;        //didn't set the buffer, means that isp will use the internal memory itself
    st_param.video_buf_len = 0;

    result = DC_VideoSetDisplayParam(&st_param);

    _Video_Display_SetFrame(p_frame, &tDisplayFrameDate);

    if (SCI_SUCCESS != DC_DisplayAFrame(&tDisplayFrameDate))
    {
        DCAMERA_TRACE("VDC:Video_Disp error2");
        result = SCI_ERROR;
    }

    return result;
}


PUBLIC DCAMERA_PARAM_PHOTO_SIZE_E DCAMERA_GetActualSnapshotMaxResolution(void)
{
    DCAMERA_PARAM_PHOTO_SIZE_E dc_size = DCAMERA_PHOTO_SIZE_MAX;

#ifdef DC_2M_SUPPORT
    dc_size = DCAMERA_PHOTO_SIZE_1600X1200;
#endif

    return dc_size;
}

/******************************************************************************
// Description: get img sensor type
// Author:      Eddy.wei
// Input:       none
// Output:      none
// Return:      sensor type
******************************************************************************/
LOCAL uint32 _DC_GetImgSensorType(void)
{
    SENSOR_EXP_INFO_T* sensor_info_ptr = Sensor_GetInfo();
    uint32 img_sensor_type=DCAMERA_IMG_SENSOR_TYPE_MAX;

    if(sensor_info_ptr->source_height_max < sensor_info_ptr->source_width_max)
    {
        img_sensor_type=DCAMERA_IMG_SENSOR_TYPE_4P3;
    }
    else if(sensor_info_ptr->source_height_max > sensor_info_ptr->source_width_max)
    {
        img_sensor_type=DCAMERA_IMG_SENSOR_TYPE_3P4;
    }
    else
    {
        img_sensor_type=DCAMERA_IMG_SENSOR_TYPE_EQ;
    }

    return img_sensor_type;

}

/******************************************************************************
// Description: get the size of upcc
// Author:      Eddy.Wei
// Input:       none
// Output:      none
// Return:      size of upcc
******************************************************************************/
LOCAL uint32 _DC_GetUpccSize(void)
{
    uint32 upcc_size = DC_UPCC_SIZE_MAX;
    uint32 img_sensor_type = _DC_GetImgSensorType();

    if(DCAMERA_IMG_SENSOR_TYPE_4P3 == img_sensor_type)
    {
        upcc_size = DC_UPCC_SIZE_320X240;
    }
    else
    {
        #if defined(MAINLCD_DEV_SIZE_160X128)||defined(MAINLCD_DEV_SIZE_220X176)||defined(MAINLCD_DEV_SIZE_320X240)
            upcc_size = DC_UPCC_SIZE_320X240;
        #else
            #if defined(MAINLCD_LOGIC_ANGLE_90) ||defined(MAINLCD_LOGIC_ANGLE_270)
            upcc_size = DC_UPCC_SIZE_320X240;
            #else
            upcc_size = DC_UPCC_SIZE_240X320;
            #endif
        #endif
    }

    return upcc_size;

}

/******************************************************************************
// Description: get the rotation mode of upcc
// Author:      Eddy.Wei
// Input:       none
// Output:      none
// Return:      rotation mode of upcc
******************************************************************************/
LOCAL uint32 _DC_GetUpccRotationMode(void)
{
    uint32 upcc_size = DC_UPCC_SIZE_MAX;
    uint32 rotation_mode = DCAMERA_ROTATION_0;
    uint32 img_sensor_type = _DC_GetImgSensorType();

    if(DCAMERA_IMG_SENSOR_TYPE_4P3 == img_sensor_type)
    {
        rotation_mode = DCAMERA_ROTATION_0;
    }
    else
    {
        upcc_size = _DC_GetUpccSize();

        if(DC_UPCC_SIZE_240X320 == upcc_size)
        {
            rotation_mode = DCAMERA_ROTATION_0;
        }
        else
        {
            #if defined(MAINLCD_LOGIC_ANGLE_270)
                rotation_mode = DCAMERA_ROTATION_270;
            #else
                rotation_mode = DCAMERA_ROTATION_90;
            #endif
        }
    }

    return rotation_mode;
}

/******************************************************************************
// Description: get the rotation mode of preview
// Author:      Eddy.Wei
// Input:       none
// Output:      none
// Return:      rotation mode of preview
******************************************************************************/
LOCAL uint32 _DC_GetDcPreviewRotationMode(void)
{
    uint32 rotation_mode = DCAMERA_ROTATION_0;
    uint32 img_sensor_type = _DC_GetImgSensorType();

    if(DCAMERA_IMG_SENSOR_TYPE_4P3 == img_sensor_type)
    {
        #if defined(MAINLCD_DEV_SIZE_160X128)||defined(MAINLCD_DEV_SIZE_220X176)||defined(MAINLCD_DEV_SIZE_320X240)
            rotation_mode = DCAMERA_ROTATION_0;
        #else
            #if defined(MAINLCD_LOGIC_ANGLE_90)
                rotation_mode = DCAMERA_ROTATION_90;
            #elif defined(MAINLCD_LOGIC_ANGLE_270)
                rotation_mode = DCAMERA_ROTATION_270;
            #endif
        #endif
    }
    else
    {
        #if defined(MAINLCD_DEV_SIZE_160X128)||defined(MAINLCD_DEV_SIZE_220X176)||defined(MAINLCD_DEV_SIZE_320X240)
            rotation_mode = DCAMERA_ROTATION_90;//or DCAMERA_ROTATION_270
        #endif
    }

    return rotation_mode;
}

/******************************************************************************
// Description: get the rotation mode of cap
// Author:      Eddy.Wei
// Input:       none
// Output:      none
// Return:      rotation mode of cap
******************************************************************************/
LOCAL uint32 _DC_GetDcCapRotationMode(void)
{
    uint32 rotation_mode = DCAMERA_ROTATION_0;
    uint32 img_sensor_type = _DC_GetImgSensorType();

    if(DCAMERA_IMG_SENSOR_TYPE_3P4 == img_sensor_type)
    {
        #if defined(MAINLCD_DEV_SIZE_160X128)||defined(MAINLCD_DEV_SIZE_220X176)||defined(MAINLCD_DEV_SIZE_320X240)
            rotation_mode = DCAMERA_ROTATION_90;//or DCAMERA_ROTATION_270
        #else
            #if defined(MAINLCD_LOGIC_ANGLE_90)
                rotation_mode = DCAMERA_ROTATION_90;
            #elif defined(MAINLCD_LOGIC_ANGLE_270)
                rotation_mode = DCAMERA_ROTATION_270;
            #endif
        #endif
    }

    return rotation_mode;
}


int DCAMERA_OpenCameraController(int open)
{
    /*20190228-xin.xu1 -code review*/
    return ISP_UixOpenCameraController(open);
}

/*lint -restore*/

