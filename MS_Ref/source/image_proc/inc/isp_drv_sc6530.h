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
 
#ifndef _ISP_DRV_SC6530_H_
#define _ISP_DRV_SC6530_H_
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
#define ISP_PATH_FRAME_COUNT_MAX                       4
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
PUBLIC int32 ISP_DriverFrameUnLock(ISP_FRAME_T *f);
PUBLIC uint8 ISP_DriverFrameIsLocked(ISP_FRAME_T *f);
PUBLIC int32 ISP_DriverCapEnable(uint32 base_addr);
PUBLIC int32 ISP_DriverCapDisable(uint32 base_addr);
PUBLIC int32 ISP_DriverCapConfig(uint32 base_addr,ISP_CFG_ID_E id,void* param);
PUBLIC int32 ISP_DriverCapGetInfo(uint32 base_addr, ISP_CFG_ID_E id, void* param);

PUBLIC int32 ISP_DriverPathConfig(uint32 base_addr,ISP_CFG_ID_E id,void* param);
PUBLIC int32 ISP_DriverPathGetInfo(uint32 base_addr,ISP_CFG_ID_E id,void* param);
PUBLIC int32 ISP_DriverModuleDisable(uint32 ahb_ctrl_addr);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif //for _ISP_DRV_SC6530_H_

