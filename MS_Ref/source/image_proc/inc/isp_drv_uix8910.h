/******************************************************************************
 ** File Name:     isp_drv_sc6530.h                                           *
 ** Author:        Baggio.he                                                  *
 ** DATE:          10/17/2011                                                 *
 ** Copyright:     2011 Spreatrum, Incoporated. All Rights Reserved.          *
 ** Description:                                                              *
 ******************************************************************************/
 
/******************************************************************************
 **                   Edit    History                                         *
 ****************************************************************************** 
 ** DATE           NAME            DESCRIPTION                       Version  *
 ** 10/17/2011     Baggio.he       Initial version for ISP driver.    1.0     * 
 ******************************************************************************/
 
#ifndef _ISP_DRV_UIX8910_H_
#define _ISP_DRV_UIX8910_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
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
#define ISP_AHB_SLAVE_ADDR                             DCAM_REG_BASE
#define ISP_PATH_FRAME_COUNT_MAX                       2
#define ISP_SCALE_FRAME_MODE_WIDTH_TH                  480
#define ISP_PATH_SC_COEFF_MAX                          4
#define ISP_CAP_DEC_FRAME_MAX                          3
#define ISP_CAP_DEC_XY_MAX                             3
#define ISP_PATH_SCALE_LEVEL                           64
#define ISP_PATH_SCALE_LEVEL_MAX                       256
#define ISP_PATH_SCALE_LEVEL_MIN                       16

typedef void (*ISP_ISR_FUNC_PTR)(void*);

/**---------------------------------------------------------------------------*
**                               Data Prototype                              ** 
**----------------------------------------------------------------------------*/
//enum
typedef enum
{
    ISP_DRV_RTN_SUCCESS = 0,
    ISP_DRV_RTN_PARA_ERR = 0x10,
    ISP_DRV_RTN_IO_ID_UNSUPPORTED,
    ISP_DRV_RTN_ISR_NOTICE_ID_ERR,
    ISP_DRV_RTN_MASTER_SEL_ERR,    
    ISP_DRV_RTN_MODE_ERR,
    ISP_DRV_RTN_IVSP_TIMWOUT,   
    ISP_DRV_RTN_IVSP_ERR,    
    
    ISP_DRV_RTN_CAP_FRAME_SEL_ERR = 0x20,
    ISP_DRV_RTN_CAP_IF_MODE_ERR, 
    ISP_DRV_RTN_CAP_IF_ENDIAN_ERR,     
    ISP_DRV_RTN_CAP_INPUT_IF_ERR,      
    ISP_DRV_RTN_CAP_INPUT_FORMAT_ERR,
    ISP_DRV_RTN_CAP_INPUT_YUV_ERR,
    ISP_DRV_RTN_CAP_SYNC_POL_ERR,
    ISP_DRV_RTN_CAP_FIFO_DATA_RATE_ERR,
    ISP_DRV_RTN_CAP_SKIP_FRAME_TOO_MANY,
    ISP_DRV_RTN_CAP_FRAME_DECI_FACTOR_ERR,
    ISP_DRV_RTN_CAP_XY_DECI_FACTOR_ERR,
    ISP_DRV_RTN_CAP_FRAME_SIZE_ERR,
    
    ISP_DRV_RTN_PATH_SRC_SIZE_ERR = 0x30,
    ISP_DRV_RTN_PATH_TRIM_SIZE_ERR,
    ISP_DRV_RTN_PATH_DES_SIZE_ERR,
    ISP_DRV_RTN_PATH_INPUT_FORMAT_ERR,
    ISP_DRV_RTN_PATH_OUTPUT_FORMAT_ERR,
    ISP_DRV_RTN_PATH_SC_COEFF_ERR,
    ISP_DRV_RTN_PATH_SUB_SAMPLE_ERR,
    ISP_DRV_RTN_PATH_ADDR_INVALIDE,
    ISP_DRV_RTN_PATH_FRAME_TOO_MANY,    
    ISP_DRV_RTN_PATH_FRAME_LOCKED,
    ISP_DRV_RTN_MAX
}ISP_DRV_RTN_E;


typedef enum
{
    ISP_DATA_YUV422 = 0,
    ISP_DATA_YUV420,
    ISP_DATA_YUV400,
    ISP_DATA_YUV420_3FRAME,
    ISP_DATA_RGB565,
    ISP_DATA_RGB888,
    ISP_DATA_CCIR656,
    ISP_DATA_JPEG,
    ISP_DATA_MAX
}ISP_DATA_FORMAT_E;

typedef enum 
{
    ISP_MODE_IDLE = 0,
    ISP_MODE_CAPTURE,
    ISP_MODE_PREVIEW,
    ISP_MODE_MPEG_CAP,
    ISP_MODE_MPEG_SCALE,    
    ISP_MODE_REVIEW,
    ISP_MODE_SCALE,
    ISP_MODE_DENOISE,
    ISP_MODE_MAX
} ISP_MODE_E;

typedef enum
{
    ISP_CAP_IF_CCIR601_8BIT = 0,            
    ISP_CAP_IF_CCIR601_4BIT,             
    ISP_CAP_IF_CCIR601_2BIT,            
    ISP_CAP_IF_CCIR601_1BIT,  
    ISP_CAP_IF_CCIR656_8BIT,        
    ISP_CAP_IF_CCIR656_4BIT,            
    ISP_CAP_IF_CCIR656_2BIT,            
    ISP_CAP_IF_CCIR656_1BIT,    
    ISP_CAP_IF_SPI_8BIT,
    ISP_CAP_IF_SPI_4BIT,
    ISP_CAP_IF_SPI_2BIT,
    ISP_CAP_IF_SPI_1BIT,  
    ISP_CAP_IF_PACKAGE_8Bit, 
    ISP_CAP_IF_PACKAGE_4Bit, 
    ISP_CAP_IF_PACKAGE_2Bit,     
    ISP_CAP_IF_PACKAGE_1Bit,   
    ISP_CAP_IF_PACKAGE_DDR_8Bit, 
    ISP_CAP_IF_PACKAGE_DDR_4Bit, 
    ISP_CAP_IF_PACKAGE_DDR_2Bit,     
    ISP_CAP_IF_PACKAGE_DDR_1Bit,
    ISP_CAP_IF_OV_SPI_8BIT,
    ISP_CAP_IF_OV_SPI_4BIT,
    ISP_CAP_IF_OV_SPI_2BIT,
    ISP_CAP_IF_OV_SPI_1BIT,
    ISP_CAP_IF_RAW,
    ISP_CAP_IF_MAX
}ISP_CAP_IF_E;

typedef enum
{
    ISP_CAP_IF_BE = 0,            
    ISP_CAP_IF_LE, 
    ISP_CAP_IF_ENDIAN_MAX
}ISP_CAP_IF_ENDIAN_E;

typedef enum
{
    ISP_CAP_INPUT_FORMAT_YUV = 0,
    ISP_CAP_INPUT_FORMAT_JPEG,
    ISP_CAP_INPUT_FORMAT_MAX
}ISP_CAP_INPUT_FORMAT_E;

typedef enum 
{
    ISP_PATTERN_YUYV  = 0,
    ISP_PATTERN_YVYU,
    ISP_PATTERN_UYVY,
    ISP_PATTERN_VYUY,
    ISP_PATTERN_MAX
}ISP_CAP_PATTERN_E;

typedef enum 
{
    ISP_CAPTURE_MODE_SINGLE=0,
    ISP_CAPTURE_MODE_MULTIPLE,
    ISP_CAPTURE__MODE_MAX
}ISP_CAPTURE_MODE_E;


typedef enum
{
    ISP_IRQ_NOTICE_SENSOR_SOF = 0,
    ISP_IRQ_NOTICE_SENSOR_EOF,      
    ISP_IRQ_NOTICE_CAP_SOF,
    ISP_IRQ_NOTICE_CAP_EOF,    
    ISP_IRQ_NOTICE_TX_DONE, 
    ISP_IRQ_NOTICE_CAP_LINE_ERR,    
    ISP_IRQ_NOTICE_CAP_FIFO_OF,    
    ISP_IRQ_NOTICE_CAP_FRAME_ERR,       
    ISP_IRQ_NOTICE_JPEG_BUF_OF, 
    ISP_IRQ_NOTICE_ISP_ERR_HANDLE,
    ISP_IRQ_NOTICE_NUMBER,    
}ISP_IRQ_NOTICE_ID_E;

typedef enum
{
    ISP_CAP_IF_MODE = 0,
    ISP_CAP_IF_ENDIAN,    
    ISP_CAP_SYNC_POL,
    ISP_CAP_INPUT_FORMAT,    
    ISP_CAP_YUV_TYPE,
    ISP_CAP_SKIP_CNT,
    ISP_CAP_FRM_DECI,
    ISP_CAP_FRM_COUNT_CLR,
    ISP_CAP_FRM_COUNT_GET,
    ISP_CAP_INPUT_RECT,
    ISP_CAP_INPUT_SIZE,
    ISP_CAP_IMAGE_XY_DECI,
    ISP_CAP_FRAME_GET_LENGTH,
    ISP_CAP_JPEG_MEM_IN_32K,
    ISP_CAP_ATV_MODE,

    
    ISP_PATH_INPUT_FORMAT = 0x10,
    ISP_PATH_INPUT_SIZE,
    ISP_PATH_INPUT_RECT,
    ISP_PATH_INPUT_ADDR,
    ISP_PATH_OUTPUT_SIZE,
    ISP_PATH_OUTPUT_FORMAT,
    ISP_PATH_OUTPUT_ADDR,
    ISP_PATH_OUTPUT_FRAME_FLAG,
    ISP_PATH_SWAP_BUFF,
    ISP_PATH_LINE_BUFF,
    ISP_PATH_SUB_SAMPLE_EN,
    ISP_PATH_SUB_SAMPLE_MOD,
    ISP_PATH_SLICE_SCALE_EN,
    ISP_PATH_SLICE_SCALE_HEIGHT,
    ISP_PATH_DITHER_EN,
    ISP_PATH_IS_IN_SCALE_RANGE,
    ISP_PATH_IS_SCALE_EN,   
    ISP_PATH_SLICE_OUT_HEIGHT,
    ISP_PATH_DENOISE_CFG,
    ISP_CFG_ID_E_MAX
}ISP_CFG_ID_E;

typedef enum 
{
    ISP_SUB_2  = 0,
    ISP_SUB_4,
    ISP_SUB_8,
    ISP_SUB_16,
    ISP_SUB_MAX
}ISP_SUB_SAMPLE_MODE_E;

typedef enum 
{
    ISP_SCALE_NOEMAL = 0,
    ISP_SCALE_SLICE,
    ISP_SCALE__MODE_MAX
}ISP_SCALE_MODE_E;

typedef enum
{
    ISP_AHB_FRAME_SRC,
    ISP_AHB_FRAME_DST,
    ISP_AHB_FRAME_SWAP,
    ISP_AHB_FRAME_LINE,
    ISP_AHB_FRAME_MAX
}ISP_AHB_FRAME_E;

enum
{
    IRAM_FOR_ISP = 0,
    IRAM_FOR_ARM
};

typedef enum
{
    ISP_CLK_LOW = 0,
    ISP_CLK_MID,        
    ISP_CLK_HIG,   
    ISP_CLK_MAX   
}ISP_CLK_SEL_E;

typedef enum
{
    ISP_CCIR_PCLK_SOURCE_CCIR=0x00,
    ISP_CCIR_PCLK_SOURCE_MTCK,
    ISP_CCIR_PCLK_SOURCE_ATV,
    ISP_CCIR_PCLK_SOURCE_MAX,
}ISP_CCIR_PCLK_SOURCE_E;

typedef enum
{
    ISP_CCIR_D0_SOURCE_CCIRD0=0x00,
    ISP_CCIR_D0_SOURCE_MTDI,
    ISP_CCIR_D0_SOURCE_MAX,
}ISP_CCIR_D0_SOURCE_E;

typedef enum
{
    ISP_CCIR_D1_SOURCE_CCIRD1=0x00,
    ISP_CCIR_D1_SOURCE_SDI1_0,
    ISP_CCIR_D1_SOURCE_SDI1_1,
    ISP_CCIR_D1_SOURCE_MAX,
}ISP_CCIR_D1_SOURCE_E;

typedef enum
{
    ISP_CCIR_VSYNC_SOURCE_CCIRVS=0x00,
    ISP_CCIR_VSYNC_SOURCE_MTMS,
    ISP_CCIR_VSYNC_SOURCE_MAX,
}ISP_CCIR_VSYNC_SOURCE_E;

typedef struct _isp_atv_ctrl_tag
{
    uint16               start;
    uint16               end;
    uint8                is_eb;
    uint8                res[3];
} ISP_ATV_CTRL_T, *ISP_ATV_CTRL_T_PTR;

typedef struct _isp_denoise_cfg_tag
{
    uint8                threshold_y;
    uint8                threshold_uv;
    uint8                denoise_eb;    
    uint8                res;
}ISP_DENOISE_CFG_T;

typedef struct _isp_cap_sync_pol_tag
{
    uint16               vsync_pol;
    uint16               hsync_pol;
}ISP_CAP_SYNC_POL_T;

typedef struct _isp_cap_dec_tag
{
    uint8                x_factor;
    uint8                y_factor;
    uint8                reserved;
    uint8                res;
}ISP_CAP_DEC_T;

typedef struct _isp_size_tag
{
    uint32               w;
    uint32               h;
}ISP_SIZE_T;

typedef struct _isp_rect_tag 
{
    uint32               x;
    uint32               y; 
    uint32               w;
    uint32               h;
    
}ISP_RECT_T;

typedef struct _isp_data_addr_tag
{
    uint32               yaddr;
    uint32               uaddr;
    uint32               vaddr;
}ISP_ADDRESS_T;

typedef struct _isp_frame_t
{
    uint32               yaddr;
    uint32               uaddr;
    uint32               vaddr;    
    uint32               lock;
    uint32               fid;    
    uint16               width;
    uint16               height;
    uint8                type;
    uint8                flags;
    uint8                res0;
    uint8                res1;
    struct _isp_frame_t  *prev;
    struct _isp_frame_t  *next;
}ISP_FRAME_T;


PUBLIC int32 ISP_DriverSetMem(uint32 *p);
PUBLIC int32 ISP_DriverInit(uint32 ahb_ctrl_addr,uint32 base_addr);
PUBLIC int32 ISP_DriverDeInit(uint32 ahb_ctrl_addr,uint32 base_addr);
PUBLIC int32 ISP_DriverSoftReset(uint32 ahb_ctrl_addr);
PUBLIC int32 ISP_DriverPclkSwitch(uint32 glb_ctrl_addr, ISP_CCIR_PCLK_SOURCE_E ccir_pclk_src);
PUBLIC int32 ISP_DriverD0Switch(uint32 glb_ctrl_addr, ISP_CCIR_D0_SOURCE_E ccir_d0_src);
PUBLIC int32 ISP_DriverVsyncSwitch(uint32 glb_ctrl_addr, ISP_CCIR_VSYNC_SOURCE_E ccir_vsync_src);
PUBLIC int32 ISP_DriverPclkSwitchSc6531EFM(uint32 glb_ctrl_addr, uint8 cap_if_mode);
PUBLIC int32 ISP_DriverD0SwitchSc6531EFM(uint32 glb_ctrl_addr, uint8 cap_if_mode);
PUBLIC int32 ISP_DriverD1SwitchSc6531EFM(uint32 glb_ctrl_addr, uint8 cap_if_mode, ISP_CCIR_D1_SOURCE_E ccir_d1_src);
PUBLIC int32 ISP_DriverVsyncSwitchSc6531EFM(uint32 glb_ctrl_addr, uint8 cap_if_mode);
PUBLIC int32 ISP_DriverSetClk(uint32 pll_src_addr,ISP_CLK_SEL_E clk_sel);
PUBLIC int32 ISP_DriverWaitForCurActionDone(uint32 base_addr);    
PUBLIC int32 ISP_DriverExitWaitForActionDone(uint32 base_addr);  

PUBLIC int32 ISP_DriverStart(uint32 base_addr);
PUBLIC int32 ISP_DriverStop(uint32 base_addr);
PUBLIC int32 ISP_DriverSetMode(uint32 base_addr,ISP_MODE_E isp_mode);
PUBLIC int32 ISP_DriverNoticeRegister(uint32 base_addr,ISP_IRQ_NOTICE_ID_E notice_id,ISP_ISR_FUNC_PTR user_func);
PUBLIC int32 ISP_DriverFrameLock(ISP_FRAME_T *f);





// uix8910
//-------------------------------------------------------------------------------

#define    YUV422_640_480_SIZE  640*480*2
#define    YUV422_320_240_SIZE  320*240*2
#define    YUV422_200_200_SIZE  200*200*2 
#define    YUV_PAD_SIZE         16


typedef struct
{
    /// An overflow occured.
    uint8 overflow:1;

    /// Frame start interrupt cause.
    uint8 fstart:1;

    /// Frame end interrupt cause.
    uint8 fend:1;

    /// DMA done interrupt cause.
    uint8 dma:1;
} HAL_CAMERA_IRQ_CAUSE_T;


// =============================================================================
// HAL_CAMERA_ENDIANESS_T
// -----------------------------------------------------------------------------
/// Camera endiannes. Defines the swapping operation done on any 32 bits of
/// received data from the camera data flow, to get pixels in the proper order.
// =============================================================================
typedef enum
{
    /// No reordering.
    NO_SWAP=0,

    /// Bytes order is inverted.
    BYTE_SWAP=1,

    /// 16 bits data are swapped.
    HALF_WORD_SWAP=2,

    /// Bits are swapped.
    BIT_SWAP=3
} HAL_CAMERA_ENDIANESS_T;


// =============================================================================
// HAL_CAMERA_XFER_STATUS_T
// -----------------------------------------------------------------------------
/// Describes the status returned by the #hal_CameraStopXfer() function.
// =============================================================================
typedef enum
{
    /// The transfer finished properly, the whole image
    /// has been received.
    XFER_SUCCESS,

    /// The transfer has not finished.
    XFER_NOT_FINISHED,

    /// The transfer finished, but an overflow occured.
    /// There is probably a misconfiguration between
    /// the number of pixels expected and the camera
    /// resolution configuration.
    XFER_FINISHED_WITH_OVERFLOW,

    /// There was no IFC configured for a transfer.
    /// #hal_CameraStopXfer() was called before
    /// #hal_CameraStartXfer() ?
    XFER_ERR_WRONG_PARAM
} HAL_CAMERA_XFER_STATUS_T;

// =============================================================================
// HAL_CAMERA_ROW_RATIO_T
// -----------------------------------------------------------------------------
/// Describes the row pixel decimation ratio.
// =============================================================================
typedef enum
{
    /// Original size, decimation disabled, 1/1.
    ROW_RATIO_1_1,

    /// Row pixel decimation 1/2.
    ROW_RATIO_1_2,

    /// Row pixel decimation 1/3.
    ROW_RATIO_1_3,

    /// Row pixel decimation 1/4.
    ROW_RATIO_1_4,
} HAL_CAMERA_ROW_RATIO_T;


// =============================================================================
// HAL_CAMERA_COL_RATIO_T
// -----------------------------------------------------------------------------
/// Describes the column pixel decimation ratio.
// =============================================================================
typedef enum
{
    /// Original size, decimation disabled, 1/1.
    COL_RATIO_1_1,

    /// Column pixel decimation 1/2.
    COL_RATIO_1_2,

    /// Column pixel decimation 1/3.
    COL_RATIO_1_3,

    /// Column pixel decimation 1/4.
    COL_RATIO_1_4,
} HAL_CAMERA_COL_RATIO_T;

typedef enum
{
    SPI_MODE_NO = 0,    // parallel sensor in use
    SPI_MODE_SLAVE ,    // SPI sensor as SPI slave
    SPI_MODE_MASTER1,   // SPI sensor as SPI master, 1 sdo output with SSN
    SPI_MODE_MASTER2_1, // SPI sensor as SPI master, 1 sdo output without SSN
    SPI_MODE_MASTER2_2, // SPI sensor as SPI master, 2 sdos output
    SPI_MODE_MASTER2_4, // SPI sensor as SPI master, 4 sdos output
    SPI_MODE_UNDEF,
} HAL_CAMERA_SPI_MODE_E;

typedef enum
{
    SPI_OUT_Y0_U0_Y1_V0 = 0,
    SPI_OUT_Y0_V0_Y1_U0,
    SPI_OUT_U0_Y0_V0_Y1,
    SPI_OUT_U0_Y1_V0_Y0,
    SPI_OUT_V0_Y1_U0_Y0,
    SPI_OUT_V0_Y0_U0_Y1,
    SPI_OUT_Y1_V0_Y0_U0,
    SPI_OUT_Y1_U0_Y0_V0,
} HAL_CAMERA_SPI_YUV_OUT_E;

// =============================================================================
// HAL_CAMERA_CFG_T
// -----------------------------------------------------------------------------
/// Camera configuration. This structure is used by #hal_CameraOpen() to
/// configure how the chip and the camera sensors are connected, and which
/// camera sensor to use in case there would be two.
// =============================================================================
#if 0
typedef struct
{
    /// Reset line polarity. Active high if \c TRUE.
    BOOLEAN rstActiveH;
    /// Power DOWN line polarity. Active high if \c TRUE.
    BOOLEAN pdnActiveH;
    /// If \c TRUE, all even frame are skipped.
    BOOLEAN dropFrame;
    // Enable window crop
    BOOLEAN cropEnable;
    /// Clock divider to apply to produce the camera clock
    /// from the 156 MHz clock.
    uint8 camClkDiv;
    uint8 reOrder;
    BOOLEAN  vsync_inv;
    /// Describes the swapping operation to apply to data
    /// received from the camera sensor.
    HAL_CAMERA_ENDIANESS_T endianess;
    /// Id (0 or 1) of the camera to use.
    uint32 camId;
    /// Row pixel decimation ratio
    HAL_CAMERA_ROW_RATIO_T rowRatio;
    /// Column pixel decimation ratio
    HAL_CAMERA_COL_RATIO_T colRatio;
    // start pixel of cropped window
    uint16 dstWinColStart;
    // end pixel of cropped window
    uint16 dstWinColEnd;
    // start line of cropped window
    uint16 dstWinRowStart;
    // end line of cropped window
    uint16 dstWinRowEnd;

    uint16 spi_pixels_per_line;
    uint16 spi_pixels_per_column;
    BOOLEAN   spi_href_inv;
    BOOLEAN   spi_little_endian_en;
    // ssn high enable, only for spi master mode 1
    BOOLEAN   spi_ssn_high_en;
    uint16 spi_ctrl_clk_div;
    HAL_CAMERA_SPI_MODE_E       spi_mode;
    HAL_CAMERA_SPI_YUV_OUT_E  spi_yuv_out;
} HAL_CAMERA_CFG_T;
#endif
#ifndef WIN32
//typedef unsigned char			BOOL;
#define BOOL unsigned char
#endif
typedef unsigned char			UINT8;
typedef unsigned short			UINT16;
typedef unsigned long			UINT32;

#define  VOID       void
typedef struct
{
    /// Reset line polarity. Active high if \c TRUE.
    BOOL rstActiveH;
    /// Power DOWN line polarity. Active high if \c TRUE.
    BOOL pdnActiveH;
    /// If \c TRUE, all even frame are skipped.
    BOOL dropFrame;
    // Enable window crop
    BOOL cropEnable;
    /// Clock divider to apply to produce the camera clock
    /// from the 156 MHz clock.
    UINT8 camClkDiv;
    UINT8 reOrder;
    BOOL  vsync_inv;
    /// Describes the swapping operation to apply to data
    /// received from the camera sensor.
    HAL_CAMERA_ENDIANESS_T endianess;
    /// Id (0 or 1) of the camera to use.
    UINT32 camId;
    /// Row pixel decimation ratio
    HAL_CAMERA_ROW_RATIO_T rowRatio;
    /// Column pixel decimation ratio
    HAL_CAMERA_COL_RATIO_T colRatio;
    // start pixel of cropped window
    // In RDA8910, if scaleEnable, means originAreaColStart
    UINT16 dstWinColStart;
    // end pixel of cropped window
    // In RDA8910, if scaleEnable, means originAreaColEnd
    UINT16 dstWinColEnd;
    // start line of cropped window
    // In RDA8910, if scaleEnable, means originAreaRowStart
    UINT16 dstWinRowStart;
    // end line of cropped window
    // In RDA8910, if scaleEnable, means originAreaRowEnd
    UINT16 dstWinRowEnd;
    UINT16 spi_pixels_per_line;
    UINT16 spi_pixels_per_column;
    BOOL  spi_href_inv;
    BOOL  spi_little_endian_en;
    // Enable scaling image, not as decimation
    BOOL scaleEnable;
    // ssn high enable, only for spi master mode 1
    BOOL  spi_ssn_high_en;
    UINT16 spi_ctrl_clk_div;
    HAL_CAMERA_SPI_MODE_E       spi_mode;
    HAL_CAMERA_SPI_YUV_OUT_E  spi_yuv_out;
    //spi ddr enable
    BOOL  ddr_en;        
    
    /// The remapped GPIO controlling PDN (HAL_GPIO_NONE if not remapped), for the rear sensor.
    //HAL_APO_ID_T    camPdnRemap;
    /// The remapped GPIO controlling RST (HAL_GPIO_NONE if not remapped), for the rear sensor.
    //HAL_APO_ID_T    camRstRemap;
    //HAL_APO_ID_T    cam1PdnRemap;
    /// The remapped GPIO controlling RST (HAL_GPIO_NONE if not remapped), for the rear sensor.
    //HAL_APO_ID_T    cam1RstRemap;

    BOOL        csi_mode;
    UINT16      num_d_term_en;
    UINT16      num_hs_settle;
    UINT16      num_c_term_en;
    UINT16      num_c_hs_settle;
    UINT8          csi_cs;  // 0 use csi1 . 1 use csi2
    UINT16      frame_line_number;
    UINT8          csi_lane_2v8;
    UINT8          sensor_format;

} HAL_CAMERA_CFG_T;



ISP_FRAME_T * hal_camera_getCurFrame();
// =============================================================================
// HAL_CAMERA_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// Type for the user function called when a camera interrupt occurs.
// =============================================================================
typedef void (*HAL_CAMERA_IRQ_HANDLER_T)(HAL_CAMERA_IRQ_CAUSE_T cause);

//typedef void (*HAL_CAMERA_IRQ_HANDLER_T)(unsigned int cause);
// =============================================================================
// hal_CameraOpen
// -----------------------------------------------------------------------------
/// Enable the Camera Control module and set the default configuration
/// for the selected external camera module.  It is both held in reset
/// AND kept in power down.
///
/// In case of dual-sensor platform, the #camId field of the #camConfig
/// parameter is to be used to select which sensor is to be enabled.
///
/// @param camConfig Camera configuration. (cf #HAL_CAMERA_CFG_T).
// =============================================================================
PUBLIC void hal_CameraOpen(HAL_CAMERA_CFG_T* camConfig);
PUBLIC void hal_CameraPowerDownBoth(BOOLEAN PowerDown);

// =============================================================================
// hal_CameraClose
// -----------------------------------------------------------------------------
/// Power off the camera sensor by setting the PowerDown bit.
/// Resets the camera sensor by enabling the Camera Reset bit.
/// This function can only be called after the camera transfer has been stopped
/// by a call to #hal_CameraStopXfer().
// =============================================================================
PUBLIC void hal_CameraClose(void);


// =============================================================================
// hal_CameraReset
// -----------------------------------------------------------------------------
/// Controls the Reset line going to the sensor
/// Uses the rstActiveH field of the configuration structure to know the polarity
/// Should not be called before the open
/// @param reset \c TRUE, the sensor is in reset
///              \c FALSE, the sensor is out of reset
// =============================================================================
PUBLIC void hal_CameraReset(BOOLEAN reset);


// =============================================================================
// hal_CameraPowerOn
// -----------------------------------------------------------------------------
/// Controls the PDN line going to the sensor
/// Uses the pdnActiveH field of the configuration structure to know the polarity
/// Should not be called before the open
/// @param powerDown \c TRUE, the sensor is in Power Down
///              \c FALSE, the sensor is out of Power Down
// =============================================================================
PUBLIC void hal_CameraPowerDown(BOOLEAN powerDown);


// =============================================================================
// hal_CameraControllerEnable
// -----------------------------------------------------------------------------
/// Turns the Camera controller module On and Off
/// Should not be called before the open
/// @param enable \cTRUE, the Controller is active
///               FALSE, the Controller is turned off
// =============================================================================
PUBLIC void hal_CameraControllerEnable(BOOLEAN enable);


// ============================================================================
// hal_CameraIrqSetHandler
// ----------------------------------------------------------------------------
/// This function configures which user function will be called by the
/// camera driver when an interruption is generated.
///
/// @param handler Pointer to a user function called when an interruption
/// is generated by the camera driver.
// ============================================================================
PUBLIC void hal_CameraIrqSetHandler(HAL_CAMERA_IRQ_HANDLER_T handler);


// =============================================================================
// hal_CameraIrqConfig()
// -----------------------------------------------------------------------------
/// Configure the desired interrupts
///
/// @param mask Mask to enable specific interrupts.  Valid interrupts are
/// liste in HAL_CAMERA_IRQ_CAUSE_T.
// =============================================================================
PUBLIC void hal_CameraIrqSetMask(HAL_CAMERA_IRQ_CAUSE_T mask);


// =============================================================================
// hal_CameraStartXfer
// -----------------------------------------------------------------------------
/// This function begins the IFC transfer of the camera data.  The camera
/// itself is reset and the camera module internal fifo is cleared.  The IFC
/// transfer is then started.
/// @param bufSize This is the size of the buffer in _bytes_
/// @param buffer Pointer to the video buffer where the IFC will store the data.
/// @return IFC channel number.
// =============================================================================
PUBLIC uint8 hal_CameraStartXfer(uint32 bufSize, uint8* buffer);



// =============================================================================
// hal_CameraStopXfer()
// -----------------------------------------------------------------------------
/// Must be called at the end of the Frame Capture
/// If an underflow occured and the IFC tranfer is not complete,
/// this function will handle the channel release
///
/// @param stop If \c TRUE, stops the camera controller.
/// @return 0 when the IC transfer was complete
///         1 when the IFC transfer was not complete and the channel had to be released
// =============================================================================
PUBLIC HAL_CAMERA_XFER_STATUS_T hal_CameraStopXfer(BOOLEAN stop);


// =============================================================================
// hal_CameraSetupClockDivider
// -----------------------------------------------------------------------------
/// setup the divider of the camera module to generate the required frequency
/// @param divider : the generated clock will be 156MHz/divider
/// divider must be between 2 and 18
// =============================================================================
PUBLIC void hal_CameraSetupClockDivider(uint8 divider);


#define CAMERA_OVFL                (1<<0)
#define CAMERA_VSYNC_R             (1<<1)
#define CAMERA_VSYNC_F             (1<<2)
#define CAMERA_DMA_DONE            (1<<3)
#define CAMERA_FIFO_EMPTY          (1<<4)
#define CAMERA_SPI_OVFL            (1<<5)

enum HAL_CAMEAR_IRQ_MASK
{
	CAM_IRQ_NULL = 0x0,
	CAM_IRQ_FSTA = CAMERA_VSYNC_R,
	CAM_IRQ_FEND = CAMERA_VSYNC_F,
	CAM_IRQ_DMA  = CAMERA_DMA_DONE,
	CAM_IRQ_OVF  = CAMERA_OVFL,
};

#define CAM_IRQ_MASK (CAM_IRQ_FSTA | CAM_IRQ_FEND | CAM_IRQ_DMA | CAM_IRQ_OVF)

void hal_camera_unmask_irq(unsigned int mask);


void hal_camera_mask_irq(unsigned int mask);


void hal_camera_clear_fifo(void);


void hal_camera_clear_irq_status(unsigned mask);

void hal_camera_enable(BOOLEAN enable);

ISP_FRAME_T* hal_Camera_getSnapshotFrame();

void hal_Camera_SetSnapshotAddr(uint32 addr);

uint32 hal_camera_GetCurFrameAddr();
uint32 hal_camera_get_status();

void hal_camera_enable_scl();

uint32 hal_camera_get_scl();

void hal_camera_disable_spimaster();

void hal_camera_enable_spimaster();


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////

typedef enum
{
	CAM_FORMAT_RGB565,
	CAM_FORMAT_YUV,
	CAM_FORMAT_RAW8,
	CAM_FORMAT_RAW10,
} CAM_FORMAT_T;

typedef enum
{
	CAM_NPIX_UXGA = (1600*1200),	// 1600x1200
	CAM_NPIX_SXGA2 = (1280*1024),   // 1280x1024
	CAM_NPIX_SXGA = (800*600),	// 800x600
	CAM_NPIX_VGA = (640*480),	// 640x480 / 480x640
	CAM_NPIX_ATV_1 = (360*288),
	CAM_NPIX_ATV_2 = (360*240),
	CAM_NPIX_WQVGA = (432*240),	// 432x240 / 240x432
	CAM_NPIX_QVGA_For_DCT = (324*240),
	CAM_NPIX_SMALL_QVGA_For_DCT = (312*240),
	CAM_NPIX_QVGA = (320*240),	// 320x240 / 240x320
	CAM_NPIX_QCIF = (220*176),	// 220x176 / 176x220
	CAM_NPIX_QQVGA = (160*120),	// 160x120 / 120x160
	CAM_NPIX_QQVGA_2 = (160*128)	// 128x160 / 160x128
} CAM_SIZE_T;


void camera_test_csi_in();

//////////////////////////////////////////////////////////////////////////
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif //for _ISP_DRV_SC6530_H_

