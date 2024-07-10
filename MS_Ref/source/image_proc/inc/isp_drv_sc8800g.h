/******************************************************************************
 ** File Name:    isp_drv_sc8800g.h                                           *
 ** Author:        Baggio.he                                               		*
 ** DATE:          03/10/2010                                                 *
 ** Copyright:    2008 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
 
/******************************************************************************
 **                   Edit    History                                         *
 ****************************************************************************** 
 ** DATE           NAME            DESCRIPTION                       Version  *
 ** 03/10/2010     Baggio.he       Initial version for ISP driver.    1.0     * 
 ******************************************************************************/
 
#ifndef _ISP_DRV_8800G_H_
#define _ISP_DRV_8800G_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dc_cfg.h"
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
typedef void (*ISP_ISR_FUNC_PTR)(void*);

#define ISP_DEFAULT_CLK                   48
#define CAP_FIFO_FULL_SPEED               0
#define CAP_FIFO_HALF_SPEED               1
#define CAP_FIFO_RESERVED                 2
#define CAP_FIFO_DOUBLE_SPEED             3
#define CAP_INPUT_FORMAT_YUV              0
#define CAP_INPUT_FORMAT_JPEG             1
#define ISP_PATH1_FRAME_COUNT_MAX         DC_DISPLAY_ROUND_BUFFER
#define ISP_PATH2_FRAME_COUNT_MAX         ISP_PATH1_FRAME_COUNT_MAX
#define ISP_PATH_SC_COEFF_MAX             4
#define ISP_PATH_SCALE_LEVEL              64
#define ISP_CAP_DEC_FRAME_MAX             15
#define ISP_CAP_DEC_XY_MAX                15
#define ISP_SCALE_FRAME_MODE_WIDTH_TH     640

#define ISP_SCALE_COEFF_H_NUM             48
#define ISP_SCALE_COEFF_V_NUM             68

#define ISP_AHB_SLAVE_ADDR                0x20200000


#define ISP_AHB_CTRL_MOD_EN_OFFSET        0
#define ISP_AHB_CTRL_MEM_SW_OFFSET        4
#define ISP_AHB_CTRL_SOFT_RESET_OFFSET    0x10
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
    
    ISP_DRV_RTN_CAP_FRAME_SEL_ERR = 0x20,
    ISP_DRV_RTN_CAP_INPUT_FORMAT_ERR,
    ISP_DRV_RTN_CAP_INPUT_YUV_ERR,
    ISP_DRV_RTN_CAP_SYNC_POL_ERR,
    ISP_DRV_RTN_CAP_FIFO_DATA_RATE_ERR,
    ISP_DRV_RTN_CAP_SKIP_FRAME_TOO_MANY,
    ISP_DRV_RTN_CAP_FRAME_DECI_FACTOR_ERR,
    ISP_DRV_RTN_CAP_XY_DECI_FACTOR_ERR,
    ISP_DRV_RTN_CAP_FRAME_SIZE_ERR,
    ISP_DRV_RTN_CAP_JPEG_DROP_NUM_ERR,
    
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
    ISP_MODE_PREVIEW_EX,
    ISP_MODE_REVIEW,
    ISP_MODE_SCALE,
    ISP_MODE_MPEG,
    ISP_MODE_VT,
    ISP_MODE_VT_REVIEW,
    ISP_MODE_MAX
} ISP_MODE_E;

typedef enum
{
    ISP_CAP_CCIR656_FRAME_SEL_ODD = 0,
    ISP_CAP_CCIR656_FRAME_SEL_EVEN,
    ISP_CAP_CCIR656_FRAME_SEL_BOTH,
    ISP_CAP_CCIR656_FRAME_SEL_MAX
}ISP_CAP_CCIR656_FRAME_SEL_E;

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
    ISP_CAP_FIFO_DATA_RATE_FULL = 0,
    ISP_CAP_FIFO_DATA_RATE_HALF,
    ISP_CAP_FIFO_DATA_RATE_RES,
    ISP_CAP_FIFO_DATA_RATE_DOUBLE,
    ISP_CAP_FIFO_DATA_RATE_MAX
}ISP_CAP_FIFO_DATA_RATE_E;

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
    ISP_IRQ_NOTICE_PATH1_DONE, 
    ISP_IRQ_NOTICE_CAP_FIFO_OF,    
    ISP_IRQ_NOTICE_SENSOR_LINE_ERR,		
    ISP_IRQ_NOTICE_SENSOR_FRAME_ERR,		
    ISP_IRQ_NOTICE_JPEG_BUF_OF,		
    ISP_IRQ_NOTICE_PATH2_DONE,    
    ISP_IRQ_NOTICE_NUMBER,    
}ISP_IRQ_NOTICE_ID_E;

typedef enum
{
    ISP_CAP_CCIR565_ENABLE = 0,
    ISP_CAP_TV_FRAME_SEL,
    ISP_CAP_SYNC_POL,
    ISP_CAP_INPUT_FORMAT,    
    ISP_CAP_YUV_TYPE,
    ISP_CAP_FIFO_DATA_RATE,
    ISP_CAP_PRE_SKIP_CNT,
    ISP_CAP_FRM_DECI,
    ISP_CAP_FRM_COUNT_CLR,
    ISP_CAP_FRM_COUNT_GET,
    ISP_CAP_INPUT_RECT,
    ISP_CAP_IMAGE_XY_DECI,
    ISP_CAP_JPEG_GET_NUM,
    ISP_CAP_JPEG_DROP_NUM,
    ISP_CAP_JPEG_GET_LENGTH,
    ISP_CAP_JPEG_MEM_IN_16K,

    
    ISP_PATH_INPUT_FORMAT,
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
    ISP_CAP_IMG_DEC_MODE_DIRECT = 0,
    ISP_CAP_IMG_DEC_MODE_AVERAGE		
}ISP_CAP_IMA_DEC_MODE_E;

typedef enum 
{
    ISP_SCALE_NOEMAL = 0,
    ISP_SCALE_SLICE,
    ISP_SCALE__MODE_MAX
}ISP_SCALE_MODE_E;

typedef enum
{
    ISP_AHB_FRAME_SRC,
    ISP_AHB_FRAME_PATH1_DST,
    ISP_AHB_FRAME_PATH2_DST,
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
    ISP_CLK_96M = 0,
    ISP_CLK_64M,        
    ISP_CLK_48M,            
    ISP_CLK_26M
}ISP_CLK_SEL_E;


typedef struct _isp_cap_sync_pol_tag
{
    uint16               vsync_pol;
    uint16               hsync_pol;
}ISP_CAP_SYNC_POL_T;

typedef struct _isp_cap_dec_tag
{
    uint8                x_factor;
    uint8                y_factor;
    uint8                x_mode;
    uint8                reserved;
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
    uint32               type;
    uint32               lock;
    uint32               flags;
    uint32               fid;
    uint32               afval;
    uint32               width;
    uint32               height;
    uint32               yaddr;
    uint32               uaddr;
    uint32               vaddr;
    uint32               rgbaddr;
    struct _isp_frame_t  *prev;
    struct _isp_frame_t  *next;
}ISP_FRAME_T;

PUBLIC int32 ISP_DriverSetMem(uint32 *p);
PUBLIC int32 ISP_DriverModuleInit(uint32 base_addr);
PUBLIC int32 ISP_DriverModuleEnable(uint32 ahb_ctrl_addr);
PUBLIC int32 ISP_DriverModuleDisable(uint32 ahb_ctrl_addr) ;
PUBLIC int32 ISP_DriverSoftReset(uint32 ahb_ctrl_addr);
PUBLIC int32 ISP_DriverSetClk(uint32 pll_src_addr,ISP_CLK_SEL_E clk_sel);
PUBLIC void  ISP_DriverIramSwitch(uint32 base_addr,uint32 isp_or_arm);

PUBLIC int32 ISP_DriverStart(uint32 base_addr);
PUBLIC int32 ISP_DriverExtInit(uint32 base_addr); 
PUBLIC int32 ISP_DriverExtStart(uint32 base_addr);
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

PUBLIC int32 ISP_DriverPath1Enable(uint32 base_addr);
PUBLIC int32 ISP_DriverPath1Disable(uint32 base_addr);
PUBLIC int32 ISP_DriverPath1Config(uint32 base_addr,ISP_CFG_ID_E id,void* param);
PUBLIC int32 ISP_DriverPath1GetInfo(uint32 base_addr,ISP_CFG_ID_E id,void* param);

PUBLIC int32 ISP_DriverPath2Enable(uint32 base_addr);
PUBLIC int32 ISP_DriverPath2Disable(uint32 base_addr);
PUBLIC int32 ISP_DriverPath2Config(uint32 base_addr,ISP_CFG_ID_E id,void* param);
PUBLIC int32 ISP_DriverPath2GetInfo(uint32 base_addr,ISP_CFG_ID_E id,void* param);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
