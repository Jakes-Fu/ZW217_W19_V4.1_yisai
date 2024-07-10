/******************************************************************************
 ** File Name:      dc_misc.h                                                        *
 ** Author:         Jing.Li                                                             *
 ** DATE:           2008/12/02                                                          *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.                   *
 ** Description:                                                                        *
 *******************************************************************************

 ******************************************************************************
 **                        Edit History                                                *
 ** ----------------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                        *
 ** 2008/12/02     Jing.Li          Create.                                            *
 ******************************************************************************/

#ifndef DC_MISC_H
#define DC_MISC_H
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "dal_dcamera.h"
#include "img_ref_types.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define         	                          *
 **---------------------------------------------------------------------------*/
#define CONVERT_ROTAION_MODE(x) DC_Convert_Rotation_Mode_to_Isp(x)

#define CONVERT_SENSOR_FORMAT(x) DC_Convert_Sensor_Format_to_Isp(x)

#define CHECK_IS_INTERPOLATION_SUPPORT

#define DC_LEFT_TRUNK(x, y)     ((x)/(y)*(y))
#define DC_RIGHT_TRUNK(x, y)    (((x)+(y)-1)/(y)*(y))

#define DC_LEFT_TRUNK16(x)      ((x)&0xfffffff0)
#define DC_RIGHT_TRUNK16(x)     (((x)+15)&0xfffffff0)

#define DC_LEFT_TRUNK8(x)       ((x)&0xfffffff8)
#define DC_RIGHT_TRUNK8(x)      (((x)+7)&0xfffffff8)

#define DC_LEFT_TRUNK2(x)       ((x)&0xfffffffe)
#define DC_RIGHT_TRUNK2(x)      (((x)+1)&0xfffffffe)

#define DC_MAX(a, b)					((a>b)? a : b)
#define DC_MIN(a, b)						((a<b)? a : b)

#define QVGA_WIDTH      (320)
#define QVGA_HEIGHT     (240)
#define WQVGA_WIDTH     (400)
#define WQVGA_HEIGHT    (240)
#define WQVGA_SIZE      (WQVGA_WIDTH*WQVGA_HEIGHT*2)
#define CIF_WIDTH	    (352)
#define CIF_HEIGHT	    (288)
#define VGA_WIDTH	    (640)
#define VGA_HEIGHT	    (480)
#define VGA_SIZE	    (VGA_WIDTH*VGA_HEIGHT*2)
#define QVGA_SIZE	    (VGA_SIZE/4)
#define SXGA_WIDTH      (1280)
#define SXGA_HEIGHT     (960)
#define SXGA_SIZE       (SXGA_WIDTH*SXGA_HEIGHT*2)
#define UXGA_WIDTH      (1600)
#define UXGA_HEIGHT     (1200)
#define UXGA_SIZE       (UXGA_WIDTH*UXGA_HEIGHT*2)
#define QXGA_WIDTH      (2048)
#define QXGA_HEIGHT     (1536)
#define QXGA_SIZE       (QXGA_WIDTH*QXGA_HEIGHT*2)

#define PREVIEW_SUPPORT                     (0X0001)
#define CAPTURE_SUPPORT                     (0X0010)
#define BURST_SHOT_SUPPORT                  (0X0100)
#define DV_SUPPORT                          (0X1000)



#define HW_INTERPOLATION_SLICE_LINE 0x10

#define DC_BUF_ALGIN                0xff
#define DC_BUF_ALGIN_BITS           0x08
#define DCAM_STATUS_DISPLAY_DONE	0x40

//event
#define DC_PARAM_CTRL_EVENT     BIT_0
#define DC_ATV_STOP_EVENT       BIT_1

#define DC_WAIT_FOREVER         0xfffffff
#define DC_WAIT_TIME            5000 //5s
#define DC_PARAM_CTRL_TIMEOUT   10000 //10s

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef enum
{
	DC_INTERPOLATION_TYPE_HW=0x01,
	DC_INTERPOLATION_TYPE_SW,
	DC_INTERPOLATION_TYPE_MAX,
}DC_INTERPOLATION_TYPE_E;

typedef enum
{
    DC_OP_READ=0x01,		
    DC_OP_WRITE,
    DC_OP_READ_WRITE,
    DC_OP_MAX
}DC_OP_MODE_E;

typedef struct _img_scale_param_t
{
    ISP_DATA_ADDR_T  input_yuv;
    ISP_SIZE_T          input_size;
    ISP_RECT_T     input_rect;
    ISP_DATA_ADDR_T  output_yuv;
    ISP_SIZE_T          output_size;
}IMG_SCALE_PARAM_T;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: Get Mutex
// Global resource dependence:
// Author: Tim.zhu
// input:
//          onne
// output:
//          none
// return:
//          none
/*****************************************************************************/
PUBLIC void DC_GetMutex(void);

/*****************************************************************************/
// Description: Put mutex
// Global resource dependence:
// Author: Tim.zhu
// input:
//          onne
// output:
//          none
// return:
//          none
/*****************************************************************************/
PUBLIC void DC_PutMutex(void);

/*****************************************************************************/
// Description: delete mutex
// Global resource dependence:
// Author: Tim.zhu
// input:
//          onne
// output:
//          none
// return:
//          none
/*****************************************************************************/
PUBLIC void DC_DelMutex(void);

/****************************************************************************************/
// Description: dc set event
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 set_flags : event put flags
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void DC_SetEvent(uint32 set_flags);

/****************************************************************************************/
// Description: Isp get event
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 requested_flags : event request flags
//                      uint32 wait_option : event wait option
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/*****************************************************************************************/
PUBLIC uint32 DC_GetEvent(uint32 requested_flags, uint32 wait_option);

/******************************************************************************
// Description:	dc update cache
// Author: Tim.zhu
// Input:
// Return:
// Note:			
******************************************************************************/ 
PUBLIC uint32 DC_UpdateCache(uint32 buffer_addr, uint32 size, DC_OP_MODE_E op_mode);

/******************************************************************************
// Description:	dc align
// Author:		Tim.zhu
// Input:			source length
// Return:		align length
// Note:			
******************************************************************************/ 
PUBLIC uint32 DC_Algin(uint32 pLen , uint16 algin_blk, uint16 algin_bits);

/******************************************************************************
// Description:     map dc rotaion mode to isp rotation mode
// Author:          jing.li
// Input:           src_mode: dc rotation mode
// Output:		    none
// Return:          isp rotation mode
// Note:            
******************************************************************************/
PUBLIC uint32 DC_Convert_Rotation_Mode_to_Isp(uint32 src_mode);

/******************************************************************************
// Description:     adjust trim & display size according to distortion mode
// Author:          jing.li
// Input:           st_trim_rect_ptr: trim rect for sensor
//                  st_disp_rect_ptr: display rect on LCD
//                  disp_mode:
// Output:		    st_trim_rect_ptr(it may be changed according to the distortion mode)
//                  st_disp_rect_ptr(it may be changed according to the distortion mode)
// Return:          none
// Note:            
******************************************************************************/
PUBLIC void DC_AdjRect_OnDistortion(DCAMERA_RECT_T* src_rect_ptr,
                                            DCAMERA_RECT_T* disp_rect_ptr, 
                                            DCAMERA_RECT_T* target_rect_ptr, 
                                            DCAMERA_DISP_MODE_E disp_mode, 
                                            DCAMERA_ROTATION_MODE_E rot_mode);

/******************************************************************************
// Description:     match the lcd point to image point
// Author:          Tim.zhu
// Input:           lcd_point_ptr: the point of lcd touch
// Output:		    target_point_ptr: the point of preview size 
// Return:          none
// Note:            
******************************************************************************/
PUBLIC uint32 DC_LcdPointMatchToImg(DCAMERA_RECT_T_PTR lcd_point_ptr, DCAMERA_RECT_T_PTR target_point_ptr);

/******************************************************************************
// Description:     get image sensor signal info
// Author:          Tim.zhu
// Input:           none
// Output:          none
// Return:          void* : the pointer of sensor signal
// Note:            none
******************************************************************************/
PUBLIC void DC_GetImgSensorSignalInfo(SENSOR_MODE_E sensor_mode,ISP_SENSOR_SIGNAL_INFO_T_PTR isp_sensor_singnal_ptr);

/******************************************************************************
// Description:     convert format
// Author:          jing.li
// Input:           sensor format: dcamera value
// Output:		    none
// Return:          isp format value
// Note:            
******************************************************************************/
PUBLIC uint32 DC_Convert_Sensor_Format_to_Isp(uint32 sensor_format);

/******************************************************************************
// Description:     set lcdc buffer buffer
// Author:          Tim.zhu
// Input:           none
// Output:          none
// Return:          uint32 : low 16bit rgb565
// Note:            none
******************************************************************************/
PUBLIC uint32 DC_GetBackGroundColor(void);

/******************************************************************************
// Description:     checker the display rect
// Author:          Tim.zhu
// Input:           none
// Output:          none
// Return:          uint32 : SCI_TRUE: support SCI_FALSE: no support
// Note:            none
******************************************************************************/
PUBLIC uint32 DC_CheckDispRect(DCAMERA_RECT_T* disp_rect, DCAMERA_RECT_T* target_rect, DCAMERA_ROTATION_MODE_E rotation_mode);

/******************************************************************************
// Description:     free the app buffer
// Author:          jing.li
// Input:           app - dc application
// Output:          none
// Return:          none
// Note:            none
******************************************************************************/
PUBLIC void DC_Free(uint32 app);

/******************************************************************************
// Description:     assign the DC buffer according to app input
// Author:          jing.li
// Input:           none
// Output:          none
// Return:          0       successful
//                  others  failed
// Note:            none
******************************************************************************/
PUBLIC uint32 DC_Malloc(uint32 app);

/******************************************************************************
// Description: get img sensor max output yuv width
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the width of yuv
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorYuvWidth(void);

/******************************************************************************
// Description: get img sensor max output yuv height
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the height of yuv
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorYuvHeight(void);

/******************************************************************************
// Description: get img sensor max output jpg width
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the width of jpg
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorJpgWidth(void);

/******************************************************************************
// Description: get img sensor max output jpg height
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the height of jpg
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorJpgHeight(void);
/******************************************************************************
// Description: get img sensor match size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the enmu of image size
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureMatchSize(DCAMERA_SIZE_T* photo_ptr);

/******************************************************************************
// Description: get img sensor max size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the enmu of sensor max size
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetMaxSensorSize(void);

/******************************************************************************
// Description: the first and end line process
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  SCI_SUCCESS:success
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_ImgTerminalLineCopy(DCAMERA_DATA_ADDR_T_PTR data_addr, DCAMERA_SIZE_T_PTR size, DCAM_DATA_TYPE_E data_format);

/******************************************************************************
// Description: set dc snapshot img sensor mode
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  None
// Note:    none
******************************************************************************/
PUBLIC void DC_SetCaptureImgSensorMode(void);

/******************************************************************************
// Description: get previewing zoom level
// Author:      jing.li
// Input:       none
// Output:      none
// Return:      zoom level,0~4
******************************************************************************/ 
PUBLIC uint8 DC_GetZoomMaxLevel(uint32* infor_ptr);

/******************************************************************************
// Description: dc one pixel yuv to rgb888
// Author:      Tim.zhu
// Input:       y: the value of y
//              uv: the value of u
//              v: the value of v
// Output:      rgb0/rgb0: the value of rgb888
// Return:      SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DC_PixelYuv2Rgb888(uint8 y0, uint8 y1, uint8 uv, uint8 v, uint32* rgb0, uint32* rgb1);

/******************************************************************************
// Description: dc two pixel rgb888 to yuv
// Author:      Tim.zhu
// Output:      rgb0/rgb1: the value of rgb888
// Input:       y: the value of y
//              uv: the value of u
//              v: the value of v
// Return:      SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DC_PixelRgb8882Yuv(uint32 rgb0, uint32 rgb1, uint8* y0, uint8* y1,uint8* uv, uint8* v);

/******************************************************************************
// Description: dc one pixel yuv to rgb565
// Author:      Tim.zhu
// Input:       y: the value of y
//              uv: the value of u
//              v: the value of v
// Output:      rgb565: the value of rgb565
// Return:      SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DC_PixelYuv2Rgb565(uint8 y, uint8 uv, uint8 v, uint16* rgb565);

/******************************************************************************
// Description: dc one pixel rgb to yuv
// Author:      Tim.zhu
// Input:       rgb565: the value of rgb565
// Output:      y: the value of y
//              uv: the value of u
//              v: the value of v
// Return:      SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DC_PixelRgb5652Yuv(uint16 rgb565, uint8* y, uint8* uv, uint8* v);

/******************************************************************************
// Description: dc yuv data overlayer for capture data
// Author:      Tim.zhu
// Input:       DCAMERA_YUV_SRC_T_PTR: the point of src yuv information
// Output:      none
// Return:      SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DC_ArgbOverlayer(IMGREF_YUV_SRC *param_ptr, IMGREF_OSD_T *capture_date_ptr);

/******************************************************************************/
// Description: rotate YUV422 image, data in y and uv channels.
// Author:     
// Input:      
//		    dst_yuv_ptr: dest yuv channel info, y data in y channel, uv data in u channel.
//		    src_yuv_ptr: source yuv channel info, y data in y channel, uv data in u channel.
//		    width: image width, must be exactly divided by 2.
//          height: image height
//          angle: 1: 90 degrees, 3: 270 degrees
// Output: none
// Return: none
// Note:   
/******************************************************************************/
PUBLIC void DC_RotateYUV422_Y_UV(JINF_YUV_CHN_T *dst_yuv_ptr, JINF_YUV_CHN_T *src_yuv_ptr, 
                                   uint32 width, uint32 height, uint32 angle);

/******************************************************************************/
// Description: rotate RGB565 image, data in rgb channels.
// Author:     
// Input:      
//		    dst_yuv_ptr: dest yuv channel info, y data in y channel, uv data in u channel.
//		    src_yuv_ptr: source yuv channel info, y data in y channel, uv data in u channel.
//		    width: image width, must be exactly divided by 2.
//          height: image height
//          angle: 1: 90 degrees
// Output: none
// Return: none
// Note:   
/******************************************************************************/
PUBLIC void DC_RotateRgb565(uint16 *dst_ptr, uint16 *src_ptr, 
                                   uint32 width, uint32 height, uint32 angle);

/******************************************************************************/
// Description: rotate yuv422 image
// Author:     
// Input:      
//          rotation_mode: 0: 90 degrees
// Output: none
// Return: DCAMERA_OP_SUCCESS: success
//         other: fail
// Note:   
/******************************************************************************/
PUBLIC uint32 DC_Rotation(DCAMERA_ROTATION_MODE_E rotation_mode);

/******************************************************************************/
//  Description: scale down yuv422 data which in y and uv channel.
//  Author:      shan.he
//  Input:     
// 		    YUV_IMG_SCALE_INFO_T: scale info parameters
//  Output: 
//  Return: none
//  Note:   
/******************************************************************************/
PUBLIC void DC_ScaleDown_Y_UV_422(IMG_SCALE_PARAM_T *scale_info_ptr);

/******************************************************************************/
//  Description: scale down yuv422 data which in y and uv channel.
//  Author:      shan.he
//  Input:     
// 		    YUV_IMG_SCALE_INFO_T: scale info parameters
//  Output: 
//  Return: none
//  Note:   
/******************************************************************************/
PUBLIC void DC_Trim_Y_UV_422(DCAMERA_YUV_CHN_ADDR_T* src_addr_ptr,
                                DCAMERA_YUV_CHN_ADDR_T* dst_addr_ptr,
                                DCAMERA_SIZE_T src_size,
                                DCAMERA_RECT_T src_rect,
                                DCAMERA_SIZE_T dst_size);

PUBLIC uint32 DC_RGB2YUV(
						uint32 width,
						uint32 height,
						uint32 rgb_buf_addr,
						uint32 yuv_buf_y_addr,
						uint32 yuv_buf_uv_addr
					  );


PUBLIC void DC_ImageOverlayRGB565(uint16 *pDstImage, uint16 img_width, uint16 img_height, uint16 *pOverlayImage, uint16 overlay_width, uint16 overlayer_height, uint16 keyColor);

#if (defined(PLATFORM_SC6600L))
/****************************************************************************************/
// Description: get jpeg slice encoder iram, copy the code to iram
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void DC_Get_JpegSliceEncoderIram(void);

/****************************************************************************************/
// Description: release jpeg slice encoder iram
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void DC_Release_JpegSliceEncoderIram(void);
#endif

/******************************************************************************/
// Description: scale up yuv 422 image, the scaling factor is 5/2. only support YUV422
//              format, and data in y and uv channel.
// Author:     
// Input:      
//		    dst_yuv_ptr: dest yuv channel info, y data in y channel, uv data in u channel.
//		    src_yuv_ptr: source yuv channel info, y data in y channel, uv data in u channel.
//		    width: image width, must be exactly divided by 50.
//          height: image height, must be exactly divided by 25.
//          temp_buf: temp buffer, ((width * 5 / 2) * 5) bytes
//          temp_buf_size: temp buffer size
// Output: none
// Return: none
// Note:   
/******************************************************************************/
PUBLIC void DC_SW_ScaleUpVGATo2M_Y_UV(JINF_YUV_CHN_T *dst_yuv_ptr, JINF_YUV_CHN_T *src_yuv_ptr, 
                                  uint32 width, uint32 height, uint8 *temp_buf, uint32 temp_buf_size);

/******************************************************************************/
// Description: scale up yuv 422 image, the scaling factor is 2. only support YUV422
//              format, and data in y and uv channel.
// Author:     
// Input:      
//		    dst_yuv_ptr: dest yuv channel info, y data in y channel, uv data in u channel.
//		    src_yuv_ptr: source yuv channel info, y data in y channel, uv data in u channel.
//		    width: image width, must be exactly divided by 2.
//          height: image height
// Output: none
// Return: none
// Note:   
/******************************************************************************/
PUBLIC void DC_SW_ScaleUp2X_Y_UV(JINF_YUV_CHN_T *dst_yuv_ptr, JINF_YUV_CHN_T *src_yuv_ptr, uint32 width, uint32 height);

/******************************************************************************/
// Description: scale up yuv 422 image, the scaling factor is 5/2. only support YUV422
//              format, and data in y and uv channel.
// Author:     
// Input:      
//		    dst_yuv_ptr: dest yuv channel info, y data in y channel, uv data in u channel.
//		    src_yuv_ptr: source yuv channel info, y data in y channel, uv data in u channel.
//		    width: image width, must be exactly divided by 50.
//          height: image height, must be exactly divided by 25.
//          temp_buf: temp buffer, ((width * 5 / 2) * 5) bytes
//          temp_buf_size: temp buffer size
// Output: none
// Return: none
// Note:   
/******************************************************************************/
PUBLIC uint32 DC_HW_YuvSliceScale(JINF_YUV_CHN_T *dst_yuv_ptr, JINF_YUV_CHN_T *src_yuv_ptr, uint32 out_height);

/******************************************************************************/
// Description: afternoon, call back notice the data address and width/height
// Author:     
// Input:      
//		    dst_yuv_ptr: dest yuv channel info, y data in y channel, uv data in u channel.
//		    width: out image width
//          height: out image height
// Output: none
// Return: DCAMERA_OP_SUCCESS: success
//                      other: fail
// Note:   
/******************************************************************************/ 
PUBLIC uint32 DC_Get_HwSliceScaleLine(ISP_DATA_ADDR_T dst_yuv_ptr, uint32 width, uint32 height);

/******************************************************************************
// Description: get support max GCD digital
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      max GCD
******************************************************************************/ 
PUBLIC uint16 DC_GetGCD(uint16 m, uint16 n);

/******************************************************************************
// Description: get interpolation type
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      0x01: HW
//              0x02: SW
******************************************************************************/ 
PUBLIC uint32 DC_GetInterpolationType(void);

/******************************************************************************
// Description: get interpolation type
// Author:      Tim.zhu
// Input:       src_height : soruce height
//              dst_height : dst height
//              interpolation_type : interpolation type
// Output:      none
// Return:      uint16: slice scale line
******************************************************************************/ 
PUBLIC uint16 DC_GetInterpolationSliceLine(uint16 src_height, uint16 dst_height, uint32 interpolation_type);

/******************************************************************************
// Description: get capture jpg encoder buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
// Output:  None 
// Return:  uint32:the length of capture jpg buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureJpgBufSize(uint16 sensor_width, uint16 sensor_height);

/******************************************************************************
// Description: get capture yuv buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
// Output:  None 
// Return:  uint32:the length of capture yuv buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureYuvBufSize(uint16 sensor_width, uint16 sensor_height, uint16 photo_width, uint16 photo_height);

/******************************************************************************
// Description: get capture jpg encoder fw buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
// Output:  None 
// Return:  uint32:the length of capture encoder fw buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureJpgFwBufSize(uint16 sensor_width, uint16 photo_width);

/******************************************************************************
// Description: get capture swap buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            photo_width: photo width
// Output:  None 
// Return:  uint32:the length of capture swap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureSwapBufSize(uint16 sensor_width, uint16 photo_width);

/******************************************************************************
// Description: get capture minature jpg temp buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
            photo_width: photo wodth
            photo_height: photo height
// Output:  None 
// Return:  uint32:the length of capture minature jpg temp buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureMinAtureJpgTempBufSize(uint16 sensor_width, uint16 sensor_height,uint16 photo_width, uint16 photo_height);

/******************************************************************************
// Description: get support max sensor size
// Author:      jing.li
// Input:       none
// Output:      none
// Return:      zoom level,0~4
******************************************************************************/ 
PUBLIC uint32 DC_GetSensorSize(void);

/******************************************************************************
// Description: get img sensor cur format
// Author:      Tim.zhu
// Input:       sensor mode
// Output:      none
// Return:      img sensor cur format
******************************************************************************/ 
PUBLIC SENSOR_IMAGE_FORMAT DC_GetSensorCurFormat(SENSOR_MODE_E sensor_mode);

/******************************************************************************
// Description: get continue shoot max num
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      conuintue shoot num  9~0
******************************************************************************/ 
PUBLIC uint8 DC_GetContinueShootMaxNum(uint32* size);

/******************************************************************************
// Description: get img sensor type shoot max num
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      sensor type
******************************************************************************/ 
PUBLIC uint32 DC_GetImgSensorType(uint32* param);

/******************************************************************************
// Description: check the supported feature of defined mem size
// Author:      jing.li
// Input:       mem size
// Output:      none
// Return:      none
******************************************************************************/ 
PUBLIC uint32 DC_ExGetFeatureSupportInfo(uint32 ex_mem_size);

/****************************************************************************************/
// Description: isp timer active
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      timer_expire: timer timeout expire
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void DC_TimerActive(uint32 timer_expire, uint32(*fun)());

/****************************************************************************************/
// Description: isp timer dactive
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void DC_TimerDactive(void);

/******************************************************************************
// Description: get memory size of dc feature
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      sensor type
******************************************************************************/ 
PUBLIC uint32 DC_GetMemSize(DC_STATE_E dc_state);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /*#ifndef DC_MISC_H*/
