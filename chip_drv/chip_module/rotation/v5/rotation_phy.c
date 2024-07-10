/***************************************************************************************
** File Name:      rotation_drv_6530.c                                                    *
** DATE:           08/11/2011                                                          *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.                   *
** Description:                                                                        *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                                 *
** 08/11/2011              Create.             
****************************************************************************************/


/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "rotation_driver.h"
#include "../../dma/dma_phy.h"

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
//#define DMA_ROTATION ROTATION_DMA_CHANNEL //0~24
LOCAL uint32 rotation_dma_channel =   0;


/****************************************************************************************/
// Description: register interrupt function
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_RegisterIntFun (ROTATION_IRQ_FUNC func)
{
    DMA_HAL_ChnIntConfig (rotation_dma_channel, DMA_CHN_INT_TYPE_LISTDONE, (DMA_HAL_ISR_HANDLER) func);

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
    DMA_HAL_ChnIntConfig (rotation_dma_channel, NULL,NULL);

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
    DMA_CHNCFG_T dma_cfg = {0};
    
    // rotation enable
    CHIP_REG_SET (AHB_EB0_SET, ROTA_EB_SET);

    Rotation_SoftwareReset();

   //Alloc a idle dma channel with your uid     
    rotation_dma_channel = DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_FULL);
 
    //config DMA channel
    dma_cfg.type = DMA_CHN_TYPE_HARD;
    dma_cfg.workmode = DMA_CHN_WORKMODE_LINKLIST;
    dma_cfg.ll_next = REG_ROTATION_DMA_CHN_CFG0;

    DMA_PHY_ChnConfig(rotation_dma_channel, &dma_cfg);
    ROTATION_DMA_CONNECT(ROTATION_DMA_REQ_ADDR, rotation_dma_channel); 

    DMA_HAL_ChnEnable_Ex (rotation_dma_channel, TRUE, NULL, 0);
}

/****************************************************************************************/
// Description: rotation unconfiger
// Global resource dependence:
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void Rotation_Uncfg (void)
{
    DMA_HAL_ChnEnable_Ex (rotation_dma_channel, FALSE, NULL, 0);

    ROTATION_DMA_CONNECT(ROTATION_DMA_REQ_ADDR, 0);
    DMA_HAL_FreeChn(rotation_dma_channel);
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
    uint32 i=0;
    // rot soft reset
    CHIP_REG_SET (AHB_RST0_SET, ROTA_SOFT_RST_SET);
    for(i=0; i<10; i++){;}
    CHIP_REG_SET (AHB_RST0_CLR, ROTA_SOFT_RST_CLR);
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
    ROTATION_ORI_COORD_U *reg_ptr= (ROTATION_ORI_COORD_U *) ROT_ORICOORD;
    uint32 * ori_width_ptr = (uint32 *)ROT_ORIWIDTH;
    reg_ptr->mBits.ori_x = ori_param_ptr->offset_x & 0xFFF;
    reg_ptr->mBits.ori_y = ori_param_ptr->offset_y & 0xFFF;
    *ori_width_ptr = ori_param_ptr->ori_width;
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

