/******************************************************************************
 ** File Name:    sc6600l_isp_drv.h                                           *
 ** Author:       Jianping.Wang                                               *
 ** DATE:         01/21/2006                                                  *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 01/21/2006    Jianping.Wang   Create.                                     *
 ******************************************************************************/
#ifndef _SC6600L_ISP_DRV_H_
#define _SC6600L_ISP_DRV_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
#include "sc_reg.h"
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
#define ISP_DEFAULT_CLK					48
#define CAP_FIFO_FULL_SPEED				0
#define CAP_FIFO_HALF_SPEED				1
#define CAP_FIFO_RESERVED				2
#define CAP_FIFO_DOUBLE_SPEED			3
#define CAP_INPUT_FORMAT_YUV	0x0
#define CAP_INPUT_FORMAT_JPEG	0x1


#define ISP_IRQ_LINE_MASK     			0x00000FFFUL

#define SCI_TRACE_ISP  SCI_TraceLow
#define ISP_ERR_TRACE(_x_) SCI_TraceLow _x_
/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
//enum
typedef enum
{
    ISP_DATA_YUV422 = 0,
    ISP_DATA_YUV420,
    ISP_DATA_YUV400,
    ISP_DATA_YUV420_3FRAME,
    ISP_DATA_RGB565,
    ISP_DATA_RGB666,
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
    ISP_MODE_REVIEW,
    ISP_MODE_MAX
} ISP_MODE_E;

typedef enum 
{
    PATTERN_YUYV  = 0,
    PATTERN_YVYU,
    PATTERN_UYVY,
    PATTERN_VYUY
}ISP_CAP_PATTERN_E;

typedef enum
{
    ISP_ROTATION_0 = 0,
    ISP_ROTATION_90,
    ISP_ROTATION_180,
    ISP_ROTATION_270,
    ISP_ROTATION_MAX
}ISP_ROTATION_E;

typedef enum
{
    ISP_CAP_DECI_DISABLE=1,
    ISP_CAP_DECI_2,
    ISP_CAP_DECI_3,
    ISP_CAP_DECI_4,
    ISP_CAP_DECI_5,
    ISP_CAP_DECI_6,
    ISP_CAP_DECI_7,
    ISP_CAP_DECI_8,
    ISP_CAP_DECI_9,
    ISP_CAP_DECI_10,
    ISP_CAP_DECI_11,
    ISP_CAP_DECI_12,
    ISP_CAP_DECI_13,
    ISP_CAP_DECI_14,
    ISP_CAP_DECI_15,
    ISP_CAP_DECI_MAX
}ISP_CAP_DECI_ID_E;

typedef enum
{
    CAP_CCIR565_ENABLE = 0,
    CAP_TV_FRAME_SEL,
    CAP_SYNC_POL,
    CAP_YUV_TYPE,
    CAP_FIFO_DATA_RATE,
    CAP_PRE_SKIP_CNT,
    CAP_CAP_FRM_DECI,
    CAP_CAP_FRM_CLR,
    CAP_CAP_START,
    CAP_CAP_END,
    CAP_CAP_DECI,
    CAP_TRANS_EB,
    CAP_PRE_SHIFT128,
    CAP_CAP_JPEG_GET_NUM,
    CAP_CAP_JPEG_DROP_NUM,
    CAP_MEM_SIZE,
    CAP_REG_ID_MAX
}CAP_REG_ID_E;

typedef enum
{
    ISP_JPEG_MEM_SIZE_512K	= 1,   //512K
    ISP_JPEG_MEM_SIZE_640K,		   //640*1024
    ISP_JPEG_MEM_SIZE_768K,		   //768*1024
    ISP_JPEG_MEM_SIZE_1024K,	   //1024*1024
    ISP_JPEG_MEM_SIZE_1280K,	   //1280*1024
    ISP_JPEG_MEM_SIZE_1536K,	   //1536*1024
    ISP_JPEG_MEM_SIZE_2048K,	   //2048*1024
    ISP_JPEG_MEM_SIZE_MAX
}ISP_CAPTURE_JPEG_MEM_SIZE_E;

typedef enum
{
    ISP_ATV_FIX_MODE = 0,
    ISP_ATV_AUTO_MODE,
    ISP_ATV_MAX
}ISP_ATV_MODE_E;

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

//structure
typedef struct _isp_rect_tag //rectangle
{
    uint16  x;
    uint16  y;
    uint16  w;
    uint16  h;
}ISP_RECT_T, *ISP_RECT_T_PTR;

typedef struct _isp_size_tag
{
    uint16 w;
    uint16 h;
} ISP_SIZE_T, *ISP_SIZE_T_PTR;

typedef struct _isp_atv_ctrl_tag
{
    BOOLEAN           is_eb;
    ISP_ATV_MODE_E        mode;
    uint16	         start;
    uint16           end;
} ISP_ARV_CTRL_T, *ISP_ARV_CTRL_T_PTR;

typedef struct _isp_data_addr_tag
 {
    uint32 y_addr;
    uint32 uv_addr;
    uint32 v_addr;
 }ISP_DATA_ADDR_T, *ISP_DATA_ADDR_T_PTR;

typedef struct _isp_data_len_tag
 {
    uint32 y_len;
    uint32 uv_len;
    uint32 v_len;
 }ISP_DATA_LEN_T, *ISP_DATA_LEN_T_PTR;

typedef struct _ahb_master_module_tag
{
    ISP_DATA_ADDR_T input_addr;
    ISP_DATA_ADDR_T output_addr;
}AHB_MASTER_MODULE_T, *AHB_MASTER_MODULE_T_PTR;

typedef struct _isp_drv_info_tag
{
    uint8  ivsp_mode; // 0: isp mode 1: vsp mode
    uint8  isp_mode;
    uint8  reserve1;
    uint8  reserve0;
    uint32 isp_sel_counter;
    uint32 isp_mem_type;
    BOOLEAN s_is_atv_enable;
    BOOLEAN boolean_reserve2;
    BOOLEAN boolean_reserve1;
    BOOLEAN boolean_reserve0;
}ISP_DRV_INFO_T, *ISP_DRV_INFO_T_PTR;

typedef struct _isp_alpha_tag
{
    uint8 alpha;
    uint8 r;
    uint8 g;
    uint8 b;
}ISP_ALPHA_T;

typedef struct _isp_signal_tag
{
    xSignalHeaderRec   		sig;

    uint32        						service;
    uint32        						command;    
    uint32 								param0;
    uint32								param1;
    void          						*ptr;
}ISP_SIGNAL_T, *ISP_SIGNAL_T_PTR;

//interfaces
/*****************************************************************************/
//  Description: Get TV state
//  Author: Tim.zhu
//	Note:
//		input: none
//		output: none
//		return: success
/*****************************************************************************/
PUBLIC BOOLEAN ISP_TV_IsEnable(void);

/*****************************************************************************/
//  Description: Set TV enable
//  Author: Tim.zhu
//	Note:
//		input: none
//		output: none
//		return: success
/*****************************************************************************/
PUBLIC BOOLEAN Set_ATV_Enable(BOOLEAN is_eb);

/****************************************************************************************/
// Description: isp software reset
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void ISP_SoftReset(void);

/****************************************************************************************/
// Description: sel the dcam is isp omde
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_SelectISP(void);

/****************************************************************************************/
// Description: release isp
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_FreeISP(void);

/****************************************************************************************/
// Description: start isp according isp mode
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_Start(void);

/****************************************************************************************/
// Description: isp stop
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_Stop(void);

/****************************************************************************************/
// Description: cap enable
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC int ISP_CapEnable(void);

/****************************************************************************************/
// Description: cap disable
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC int ISP_CapDisable(void);

/****************************************************************************************/
// Description: set cap cfg inf
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetCAP(CAP_REG_ID_E id,uint32 param);

/****************************************************************************************/
// Description: get last jpeg len
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_GetJpegLength(void);

/*****************************************************************************************/
// Description: set the scale coeff
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetScaleCoeff(uint32 *h_scale_ptr, uint32* v_scale_ptr);

/****************************************************************************************/
// Description: set ver down top
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetVerDownTap(uint32 tap_num);

/****************************************************************************************/
// Description: set ahb master writ data addr
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_SetAhbMaster(AHB_MASTER_MODULE_T *param_ptr);

/****************************************************************************************/
// Description: set scale slice swap ahb master writ data addr
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetScaleSliceSwapMaster(AHB_MASTER_MODULE_T *param_ptr);

/****************************************************************************************/
// Description: set dcam isp mode or vsp mode
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetMode(ISP_MODE_E mode);

/****************************************************************************************/
// Description: set dcam isp mode or vsp mode
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC ISP_MODE_E ISP_GetMode(void);

/****************************************************************************************/
// Description: sel ahb_clk domain or d_cam domain
// Global resource dependence: 
// Author: Jianping.wang
// Note:
//
/*****************************************************************************************/
PUBLIC void ISP_SwichtClkDomain(uint32 domain);

/****************************************************************************************/
// Description: get the currect clk domain
// Global resource dependence: 
// Author: Jianping.wang
// Note:
//
/*****************************************************************************************/
PUBLIC uint32 ISP_GetClkDomain(void);

/****************************************************************************************/
// Description: set the currect clk domain is ahb
// Global resource dependence: 
// Author: Tim.zhu
// Note:
//
/*****************************************************************************************/
PUBLIC uint32 ISP_SetClkAhbDomain(void);

/****************************************************************************************/
// Description: set the currect clk domain is dcam
// Global resource dependence: 
// Author: Tim.zhu
// Note:
//
/*****************************************************************************************/
PUBLIC uint32 ISP_SetClkDcamDomain(void);

/****************************************************************************************/
// Description: set cap input data format
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetCAPInputFormat(uint32 format);

/****************************************************************************************/
// Description: make the shade register effect
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_ForceCopy(void);

/****************************************************************************************/
// Description: make the shade register effect
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void ISP_AutoCopy(void);

/****************************************************************************************/
// Description: set cap output size equal ot scaling module input size
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetSrcSize(uint32 x,uint32 y);

/****************************************************************************************/
// Description: set scaling mode output size
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SerDstSize(uint32 x,uint32 y);

/****************************************************************************************/
// Description: set scaling module trim range
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetScaleTrimRect(ISP_RECT_T rect);

/****************************************************************************************/
// Description: set scaling module trim range
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetScaleTrimRectY(uint32 y);

/****************************************************************************************/
// Description: set scaling input data format
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetScaleInputFormat(ISP_DATA_FORMAT_E format);

/****************************************************************************************/
// Description: bypass the scaling process
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_BypassScale(BOOLEAN is_bypass);

/****************************************************************************************/
// Description: enable scale trim
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_EnableScaleTrim(BOOLEAN is_enable);

/****************************************************************************************/
// Description: set cap input yuv data pattern, only effect for yuv data
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_SetCapPattern(ISP_CAP_PATTERN_E pattern);

/****************************************************************************************/
// Description: enable yub2rgb
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_EnableYUV2RGB(BOOLEAN is_enable);

/****************************************************************************************/
// Description: enable dither
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void ISP_EnableDither(BOOLEAN is_enable);

/*****************************************************************************/
//  Description: Set_ISP_Matrix
//  Author: Tim.zhu
//	Note:
//		input: none
//		output: none
//		return: success
/*****************************************************************************/
PUBLIC BOOLEAN Set_ISP_Matrix(void);

/*****************************************************************************/
//  Description: ISP_SetMemType
//  Author: Tim.zhu
//	Note:
//		input: mem_type
//		output: none
//		return: none
/*****************************************************************************/
PUBLIC void ISP_SetMemType(uint32 mem_type);

/****************************************************************************************/
// Description: Set scale input type yuv or rgb
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void ISP_SetScaleInputType(BOOLEAN is_rgb);

/****************************************************************************************/
// Description: set scale input rgb mode 888 or 565
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/ 
PUBLIC void ISP_SetScaleInputRgbFormat(BOOLEAN is_rgb565);

/****************************************************************************************/
// Description: get cap max deci
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_GetMaxCapDeci(void);

/****************************************************************************************/
// Description: set the yuv2rgb alpha
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void ISP_SetAlpha(ISP_ALPHA_T *param);

/****************************************************************************************/
// Description: get preview max scale up coeff
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_GetMaxPreviewScaleUpCoeff(void);

/****************************************************************************************/
// Description: get max scale up coeff
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_GetMaxScaleUpCoeff(void);

/****************************************************************************************/
// Description: get max scale down coeff
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_GetMaxScaleDownCoeff(void);

/****************************************************************************************/
// Description: 4X scale up enable
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void ISP_4MultipleScaleUpEnable(void);

/****************************************************************************************/
// Description: the cap eof interrupt is effect
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_CapEofSupport(void);

/****************************************************************************************/
// Description: the bad frame check is support
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_BadFrameCheckSupport(void);

/****************************************************************************************/
// Description: the is support the 4X scale up
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_4MultipleScaleUpSupport(void);

/****************************************************************************************/
// Description: Get the pointer Scaling coeff
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32* ISP_GetVScaleCoeffTab(uint32 v_coeff);

/****************************************************************************************/
// Description: Get the pointer of Scaling coeff
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32* ISP_GetHScaleCoeffTab(uint32 h_coeff);

/******************************************************************************/
// Description: Get MAX SCALE. width
// Author:     Tim.zhu
// Input:      none
// Output:     none
// Return:    max scale width
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxScaleWidth(void);

/******************************************************************************/
// Description: Get MAX SCALE. height
// Author:     Tim.zhu
// Input:      none
// Output:     none
// Return:    max scale height
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxScaleHeight(void);

/******************************************************************************/
// Description: Get max width of display
// Author:     
// Input:      none
// Output:     none
// Return:  the max width of display
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxDisplayWidth(void);

/******************************************************************************/
// Description: Get max hdight of display
// Author:     
// Input:      none
// Output:     none
// Return:  the max height of display    
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxDisplayHeight(void);

/******************************************************************************/
// Description: Get Capture MAX. size(width and height)
// Author:     Tim.zhu
// Input:      none
// Output:     none
// Return:    max capture size
//             
// Note:       
/******************************************************************************/
PUBLIC ISP_SIZE_T* ISP_GetMaxCaptureSize(void);

/****************************************************************************************/
// Description: check the scale up ability
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_CheckScaleUpAbility(uint16 input_width, uint16 input_height, uint16 output_width, uint16 output_height);

/****************************************************************************************/
// Description: get max scale up coeff
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_GetMaxScaleUpCoeff(void);

/****************************************************************************************/
// Description: get max scale down coeff
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_GetMaxScaleDownCoeff(void);

/****************************************************************************************/
// Description: set scale mode
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetScaleMode(ISP_SCALE_MODE_E scale_mode);

/******************************************************************************
// Description:	Get is support slice scale scale mode
// Author:		Tim.zhu
// Input:			None
// Return:		BOOLEAN : SCI_TRUE: support slice SCI_FALSE: not support slice mode
// Note:			
******************************************************************************/ 
PUBLIC BOOLEAN ISP_IsSupportSliceScale(void);

/******************************************************************************
// Description:	Get is support subsample
// Author:		Tim.zhu
// Input:			None
// Return:		BOOLEAN : SCI_TRUE: support subsample SCI_FALSE: not support Subsample
// Note:			
******************************************************************************/ 
PUBLIC BOOLEAN ISP_IsSupportSubsample(void);

/****************************************************************************************/
// Description: set the height slice scale
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetScaleSliceHeight(uint32 height);

/****************************************************************************************/
// Description: get the height of slice scale done
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_GetScaleSliceDoneHeight(void);

/****************************************************************************************/
// Description: set slice scale last slice
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetScaleLastSlice(BOOLEAN is_last);

/****************************************************************************************/
// Description: set sub sample mode
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC int ISP_SetSubSampleMode(BOOLEAN is_enable, ISP_SUB_SAMPLE_MODE_E sub_mode);

/******************************************************************************/
// Description: get max sub sample mul slice scale
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/******************************************************************************/
PUBLIC uint32 ISP_GetMaxSubSampleMul(void) ;

/******************************************************************************/
// Description: Get max width of display
// Author:     
// Input:      none
// Output:     none
// Return:  the max width of display
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxDisplayWidth(void);

/******************************************************************************/
// Description: Get max hdight of display
// Author:     
// Input:      none
// Output:     none
// Return:  the max height of display    
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxDisplayHeight(void);

/******************************************************************************/
// Description: Get Capture MAX width
// Author:     
// Input:      none
// Output:     none
// Return:    
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxCaptureWidth(void); 

/******************************************************************************/
// Description: Get Capture MAX height
// Author:     
// Input:      none
// Output:     none
// Return:    
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxCaptureHeight(void);

/******************************************************************************/
// Description: Get max width of slice scale
// Author:     
// Input:      none
// Output:     none
// Return:    
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxSliceScaleWidth(void);

/******************************************************************************/
// Description: Get max height of slice scale
// Author:     
// Input:      none
// Output:     none
// Return:    
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetMaxSliceScaleHeight(void);

/******************************************************************************/
// Description: Get max height of slice scale
// Author:     
// Input:      none
// Output:     none
// Return:    
//             
// Note:       
/******************************************************************************/
PUBLIC uint16 ISP_GetSliceScaleLineWidth(uint16 width);

/******************************************************************************/
// Description: Get preview output data format
// Author:     
// Input:      none
// Output:     none
// Return:    
//             
// Note:       
/******************************************************************************/
PUBLIC ISP_DATA_FORMAT_E ISP_GetDispDataFormat(void);

/******************************************************************************/
// Description: Get analog tv dma mode
// Author:     
// Input:      none
// Output:     none
// Return:    
//             
// Note:       
/******************************************************************************/
PUBLIC BOOLEAN ISP_GetAnalogTVDmaEb(void);

/******************************************************************************/
// Description: isp set analog tv mode
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/******************************************************************************/
PUBLIC uint32 ISP_SetATVMode(ISP_ARV_CTRL_T* atv_ptr);

/******************************************************************************/
// Description: check is support update cache
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/******************************************************************************/
PUBLIC BOOLEAN ISP_IsSupportUpdateCache(void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
