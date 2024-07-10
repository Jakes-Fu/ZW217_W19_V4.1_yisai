/***************************************************************************************
** File Name:      rotation_phy.c                                                    *
** DATE:           12/06/2008                                                          *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.                   *
** Description:                                                                        *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                                 *
** 12/06/2008              Create.                                                     *
****************************************************************************************/


/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "rotation_driver.h"
//#include "v0/rotation_reg_v0.h"

//#include "dma_drv_internal.h"

/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                                              *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**------------------------------------------------------------------------------------*
 **                         Macro define                                               *
 **------------------------------------------------------------------------------------*/
#define ROTATION_DRV_ZERO   0
#define ROTATION_DRV_ONE     1
#define ROTATION_DRV_TWO    2

/****************************************************************************************/
// Description: register interrupt function
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_RegisterIntFun (ROTATION_IRQ_FUNC func)
{
    DMA_HAL_ChnIntConfig (DMA_ROT, DMA_CHN_INT_TYPE_LISTDONE, (DMA_HAL_ISR_HANDLER) func);

    return ;
}

/****************************************************************************************/
// Description: register interrupt function
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_UnRegisterIntFun (void)
{
    DMA_HAL_ChnIntConfig (DMA_ROT, NULL,NULL);

    return ;
}

/****************************************************************************************/
// Description: rotation configer
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_Cfg (void)
{
    AHB_CTL0_U *ahb_ctl0_ptr= (AHB_CTL0_U *) AHB_CTL0;
    AHB_SOFT_RST_U *ahb_soft_rst_ptr= (AHB_SOFT_RST_U *) AHB_SOFT_RST;
    DMA_CHNCFG_T dma_cfg = {0};

    // rot eb
    ahb_ctl0_ptr->mBits.rot_eb=ROTATION_DRV_ONE;

    // rot soft reset

    ahb_soft_rst_ptr->mBits.rot_rst         = ROTATION_DRV_ONE;
    ahb_soft_rst_ptr->mBits.rot_rst         = ROTATION_DRV_ONE;
    ahb_soft_rst_ptr->mBits.rot_rst         = ROTATION_DRV_ONE;
    ahb_soft_rst_ptr->mBits.rot_rst         = ROTATION_DRV_ZERO;

    dma_cfg.type = DMA_CHN_TYPE_HARD;
    dma_cfg.workmode = DMA_CHN_WORKMODE_LINKLIST;
    dma_cfg.ll_next = REG_ROTATION_DMA_CHN_CFG0;
    DMA_HAL_ChnConfig (DMA_ROT, &dma_cfg);

    DMA_HAL_ChnEnable_Ex (DMA_ROT, TRUE, NULL, 0);
}

/****************************************************************************************/
// Description: rotation unconfiger
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_Uncfg (void)
{
    DMA_HAL_ChnEnable (DMA_ROT, FALSE);

    return ;
}

/****************************************************************************************/
// Description: rotation soft ware reset
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_SoftwareReset (void)
{
    AHB_SOFT_RST_U *reg_ptr= (AHB_SOFT_RST_U *) AHB_SOFT_RST;

    // rot soft reset
    reg_ptr->mBits.rot_rst      = ROTATION_DRV_ONE;
    reg_ptr->mBits.rot_rst      = ROTATION_DRV_ONE;
    reg_ptr->mBits.rot_rst      = ROTATION_DRV_ONE;
    reg_ptr->mBits.rot_rst      = ROTATION_DRV_ZERO;
}

/****************************************************************************************/
// Description: set src addr
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_SetSrcAddr (uint32 src_addr)
{
    ROTATION_SRC_ADDR_U *reg_ptr= (ROTATION_SRC_ADDR_U *) REG_ROTATION_SRC_ADDR;

    reg_ptr->mBits.src_addr=src_addr;

    return ;
}

/****************************************************************************************/
// Description: set dst addr
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_SetDstAddr (uint32 dst_addr)
{
    ROTATION_DST_ADDR_U *reg_ptr= (ROTATION_DST_ADDR_U *) REG_ROTATION_DST_ADDR;

    reg_ptr->mBits.dst_addr=dst_addr;

    return ;
}

/****************************************************************************************/
// Description: set image size
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_SetImgSize (ROTATION_SIZE_T *size)
{
    ROTATION_IMG_SIZE_U *reg_ptr= (ROTATION_IMG_SIZE_U *) REG_ROTATION_IMG_SIZE;

    reg_ptr->mBits.width=size->w;
    reg_ptr->mBits.height=size->h;

    return ;
}

/****************************************************************************************/
// Description: set image size
// Global resource dependence:
// Author: xz
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_SetOriParam (ROTATION_ORI_PARAM_T * ori_param_ptr)
{
    return ;
}

/****************************************************************************************/
// Description: set pixel mode
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_SetPixelMode (ROTATION_PIXEL_FORMAT_E pixel_format)
{
    ROTATION_IMG_SIZE_U *reg_ptr= (ROTATION_IMG_SIZE_U *) REG_ROTATION_IMG_SIZE;

    reg_ptr->mBits.pixel_format=pixel_format;

    return ;
}

/****************************************************************************************/
// Description: set rotation mode
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_SetMode (ROTATION_MODE_E rotation_mode)
{
    ROTATION_CTRL_U *reg_ptr= (ROTATION_CTRL_U *) REG_ROTATION_CTRL;

    reg_ptr->mBits.rotation_mode=rotation_mode;

    return ;
}

/****************************************************************************************/
// Description: set uv mode
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_SetUVMode (ROTATION_UV_MODE_E uv_mode)
{
    ROTATION_CTRL_U *reg_ptr= (ROTATION_CTRL_U *) REG_ROTATION_CTRL;

    reg_ptr->mBits.rot_mode=uv_mode;

    return ;
}

/****************************************************************************************/
// Description: rotation start
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_Start (void)
{
    ROTATION_CTRL_U *reg_ptr= (ROTATION_CTRL_U *) REG_ROTATION_CTRL;

    reg_ptr->mBits.rotation_eb=ROTATION_DRV_ONE;

    return ;
}

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

//end of rotation_drv_6800h.c
