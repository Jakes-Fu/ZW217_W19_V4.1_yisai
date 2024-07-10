/******************************************************************************
 ** File Name:      vbc_phy_v5.c                                              *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Physical layer of voiceband                               *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "dma_drv_internal.h"

#include "audio_drvapi.h"
#include "audio_hal.h"
#include "vbc_phy.h"
#include "vbc_phy_v5.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
#ifdef VBC_DEBUG
#define VBC_PHY_DBG       SCI_TRACE_LOW
#define VBC_PHY_DBG_ASS   SCI_ASSERT
#else
#define VBC_PHY_DBG(...)
#define VBC_PHY_DBG_ASS(x)
#endif

#define VBC_PHY_PRT       SCI_TRACE_LOW


#if defined (CHAN_L_TO_DA0)
#define ARM_VB_DALON                ARM_VB_DA0ON
#define ARM_VB_DARON                ARM_VB_DA1ON
#define ARM_VB_ADLON                ARM_VB_ADCON
#define ARM_VB_ADRON                ARM_VB_AD1ON
#define VBDALDMA_EN                 VBDA0DMA_EN
#define VBDARDMA_EN                 VBDA1DMA_EN
#define VBADLDMA_EN                 VBAD0DMA_EN
#define VBADRDMA_EN                 VBAD1DMA_EN
#define VBDAL                       VBDA0
#define VBDAR                       VBDA1
#define VBADL                       VBAD0
#define VBADR                       VBAD1
#define VBADPATH_ADL_DGMUX          VBADPATH_AD0_DGMUX
#define VBADPATH_ADR_DGMUX          VBADPATH_AD1_DGMUX
#define VBDAPATH_DAL_ADDFM_MASK     VBDAPATH_DA0_ADDFM_MASK
#define VBDAPATH_DAR_ADDFM_MASK     VBDAPATH_DA1_ADDFM_MASK
#define VBDAPATH_DAL_ADDFM_SHIFT    VBDAPATH_DA0_ADDFM_SHIFT
#define VBDAPATH_DAR_ADDFM_SHIFT    VBDAPATH_DA1_ADDFM_SHIFT
#else
#define ARM_VB_DALON                ARM_VB_DA1ON
#define ARM_VB_DARON                ARM_VB_DA0ON
#define ARM_VB_ADLON                ARM_VB_AD1ON
#define ARM_VB_ADRON                ARM_VB_ADCON
#define VBDALDMA_EN                 VBDA1DMA_EN
#define VBDARDMA_EN                 VBDA0DMA_EN
#define VBADLDMA_EN                 VBAD1DMA_EN
#define VBADRDMA_EN                 VBAD0DMA_EN
#define VBDAL                       VBDA1
#define VBDAR                       VBDA0
#define VBADL                       VBAD1
#define VBADR                       VBAD0
#define VBADPATH_ADL_DGMUX          VBADPATH_AD1_DGMUX
#define VBADPATH_ADR_DGMUX          VBADPATH_AD0_DGMUX
#define VBDAPATH_DAL_ADDFM_MASK     VBDAPATH_DA1_ADDFM_MASK
#define VBDAPATH_DAR_ADDFM_MASK     VBDAPATH_DA0_ADDFM_MASK
#define VBDAPATH_DAL_ADDFM_SHIFT    VBDAPATH_DA1_ADDFM_SHIFT
#define VBDAPATH_DAR_ADDFM_SHIFT    VBDAPATH_DA0_ADDFM_SHIFT
#endif



/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/


LOCAL void _vbc_en_op(uint32 en,uint32 reg,uint32 bit)
{
    if( en )
    {
        VB_WRITE_BITS(reg,(bit),(bit));
    }
    else
    {
        VB_WRITE_BITS(reg,~(bit),(bit));
    }
}

LOCAL void _vbc_set_mask(uint32 reg,uint32 val
    ,uint32 mask,uint32 shif
)
{
    VB_WRITE_BITS(reg,(val<<shif),mask);
}

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description :    Enable DAC clock for VBC module.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_da_enable(uint32 en,uint32 chan)
{
    if (en)
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            REG32 (VB_CLK_CTL) |= ARM_VB_DALON ;
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            REG32 (VB_CLK_CTL) |= ARM_VB_DARON ;
        }
    }
    else
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            REG32 (VB_CLK_CTL) &= ~ (ARM_VB_DALON);
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            REG32 (VB_CLK_CTL) &= ~ (ARM_VB_DARON);
        }
    }
}

/*****************************************************************************/
// Description :    Enable ADC clock for VBC module.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_ad_enable(uint32 en,uint32 chan)
{
    if (en)
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            REG32 (VB_CLK_CTL) |= ARM_VB_ADLON ;
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            REG32 (VB_CLK_CTL) |= ARM_VB_ADRON ;
        }
    }
    else
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            REG32 (VB_CLK_CTL) &= ~ (ARM_VB_ADLON);
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            REG32 (VB_CLK_CTL) &= ~ (ARM_VB_ADRON);
        }
    }
}

/*****************************************************************************/
// Description :    Enables DA channel of VB module to DMA.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_dma_da_chn_en(uint32 en,uint32 chan)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        _vbc_en_op(en,VBDABUFFDTA,VBDALDMA_EN);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        _vbc_en_op(en,VBDABUFFDTA,VBDARDMA_EN);
    }
}

/*****************************************************************************/
// Description :    Enables AD channel of VB module to DMA.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_dma_ad_chn_en(uint32 en,uint32 chan)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        _vbc_en_op(en,VBDABUFFDTA,VBADLDMA_EN);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        _vbc_en_op(en,VBDABUFFDTA,VBADRDMA_EN);
    }
}

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    Swtiches the controlling of VB module.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_arm_acc_switch(uint32 is_arm)
{
    if (is_arm)
    {
        VB_SWTICH_ARM_CTL (SCI_TRUE);
    }
    else
    {
        VB_SWTICH_ARM_CTL (SCI_FALSE);
    }
}


PUBLIC int32 __vbc_set_buffer_size(uint32 da_buf_size,uint32 ad_buf_size)
{
    int32 ret = AUDIO_HAL_ERROR;
    VBC_PHY_DBG_ASS(da_buf_size<=VB_DA_BUF_SIZE);/*assert verified*/
    VBC_PHY_DBG_ASS(ad_buf_size<=VB_AD_BUF_SIZE);/*assert verified*/
    if((da_buf_size>0) && (da_buf_size<=VB_DA_BUF_SIZE))
    {
        _vbc_set_mask(VBBUFFERSIZE,(da_buf_size-1)
            ,VBDABUFFSIZE_MASK,VBDABUFFSIZE_SHIFT);
        ret = AUDIO_HAL_SUCCESS;
    }
    if((ad_buf_size>0) && (ad_buf_size<=VB_AD_BUF_SIZE))
    {
        _vbc_set_mask(VBBUFFERSIZE,(ad_buf_size-1)
            ,VBADBUFFSIZE_MASK,VBADBUFFSIZE_SHIFT);
        ret = AUDIO_HAL_SUCCESS;
    }
    return ret;
}

/*****************************************************************************/
// Description :    reset VB module pingpang buffer.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_clear_da_buffer(void)
{
    uint32 reg_save = 0;
    
    SCI_DisableIRQ(); //Protect GR Register.
    
    reg_save = REG32 (VB_CLK_CTL);
    
    REG32 (VB_CLK_CTL) |= ARM_VB_ANAON;
    
    __vbc_da_enable(1,AUDIO_ALL_CHAN);

    // Enable MCU read ADC, write DAC0, DAC1 data buffer when VBENABLE bit low.
    _vbc_en_op(1,VBDABUFFDTA,VBRAMSW_EN);

    __vbc_set_buffer_size(VB_DA_BUF_SIZE,0);

    // Select data buffer 1 and clear it
    _vbc_en_op(1,VBDABUFFDTA,VBRAMSW_NUMBER);

    __vbc_write_z(VB_DA_BUF_SIZE);

    // Select data buffer 0 and clear it
    _vbc_en_op(0,VBDABUFFDTA,VBRAMSW_NUMBER);

    __vbc_write_z(VB_DA_BUF_SIZE);

    // Disable MCU read ADC, write DAC0, DAC1 data buffer when VBENABLE bit low
    _vbc_en_op(0,VBDABUFFDTA,VBRAMSW_EN);

    REG32 (VB_CLK_CTL) = reg_save;

    SCI_RestoreIRQ();
}

PUBLIC void __vbc_iis_high_for_da0(uint32 en)
{
    //MUST BE 0. OR SOME TEST PERFORMANCE WILL BE FAIL!!
    // IIS timeing :0 High(right channel) for DA0, Low(left channel) for DA1
    _vbc_en_op(0,VBADBUFFDTA,VBIIS_LRCK);
}

/*****************************************************************************/
// Description :    Initailizes the VB module. It's called by HAL layer when
//                  system starts up.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_init(void)
{
    volatile int i;

    // Enable ARM control
    __vbc_arm_acc_switch(SCI_TRUE);

    // Delay a while
    for (i=0; i<5; i++) {}

    __vbc_clear_da_buffer();
}

PUBLIC void __vbc_reset(void)
{
    VB_PHY_RESET_M();
}

/*****************************************************************************/
// Description :    open the VB module.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_open(void)
{
    REG32 (VB_CLK_CTL) |= ARM_VB_ANAON;
    __vbc_clear_da_buffer();
    __vbc_set_buffer_size(VB_DA_BUF_SIZE,VB_AD_BUF_SIZE);
}

/*****************************************************************************/
// Description :    Close VB module.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_close(void)
{
    __vbc_disable();
    
    __vbc_clear_da_buffer();

    __vbc_reset();
    __vbc_da_enable(0,AUDIO_ALL_CHAN);
    __vbc_ad_enable(0,AUDIO_ALL_CHAN);

    VB_PHY_HPFSwitch(SCI_FALSE);
}

/*****************************************************************************/
// Description :    Disable VB module.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_disable(void)
{
    // VB_DISABLE
    _vbc_en_op(0,VBDABUFFDTA,VBENABLE);
}

/*****************************************************************************/
// Description :    Enable VB module.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_enable(void)
{
    // VB_ENABLE
    _vbc_en_op(1,VBDABUFFDTA,VBENABLE);
}


/*****************************************************************************/
// Description :    Writes specific data to pingpang buffer of VB module.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_write(int16 *l_ptr, int16 *r_ptr,uint16 size)
{
    uint32 i;
    for (i = 0; i < size; i += 1 )
    {
        REG32(VBDAL) = * (l_ptr + i);
        REG32(VBDAR) = * (r_ptr + i);
    }
}

PUBLIC void __vbc_write_z(uint16 size)
{
    uint32 i;
    for (i = 0; i < size; i += 1 )
    {
        REG32(VBDAL) = 0;
        REG32(VBDAR) = 0;
    }
}

/*****************************************************************************/
// Description :    Returns the interrupt flag of VBDA.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC uint32 __vbc_is_da_int(void)
{
    return (REG32 (VB_INT_MASK_BASE) & VB_DA_INT_MASK);
}

/*****************************************************************************/
// Description :    Returns the interrupt flag of VBAD.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC uint32 __vbc_is_ad_int(void)
{
    return (REG32 (VB_INT_MASK_BASE) & VB_AD_INT_MASK);
}

/*****************************************************************************/
// Description :    Clears the interrupt flag of VBDA.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_clr_da_int(void)
{
    REG32 (VB_INT_CLR_BASE) = VB_DA_INT_CLR_MASK;
}

/*****************************************************************************/
// Description :    Clears the interrupt flag of VBAD.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_clr_ad_int(void)
{
    REG32 (VB_INT_CLR_BASE) = VB_AD_INT_CLR_MASK;
}

/*****************************************************************************/
// Description :    Select DAC I2S port.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_dac_sel_iis(uint32 port)
{
    _vbc_set_mask(VBIISSEL,port,VBIISSEL_DA_PORT_MASK,VBIISSEL_DA_PORT_SHIFT);
}


/*****************************************************************************/
// Description :    Select ADC I2S port.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_adc_sel_iis(uint32 port)
{
    _vbc_set_mask(VBIISSEL,port,VBIISSEL_AD_PORT_MASK,VBIISSEL_AD_PORT_SHIFT);
}


/*****************************************************************************/
// Description :    Set DAC FM Mix mode.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __vbc_dac_fm_mix(uint32 mode,uint32 chan)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        _vbc_set_mask(DAPATHCTL,mode,VBDAPATH_DAL_ADDFM_MASK,VBDAPATH_DAL_ADDFM_SHIFT);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        _vbc_set_mask(DAPATHCTL,mode,VBDAPATH_DAR_ADDFM_MASK,VBDAPATH_DAR_ADDFM_SHIFT);
    }
}

PUBLIC void __vbc_adc_dgmux(uint32 mode,uint32 chan)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        _vbc_en_op(mode,ADPATHCTL,VBADPATH_ADL_DGMUX);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        _vbc_en_op(mode,ADPATHCTL,VBADPATH_ADR_DGMUX);
    }
}

PUBLIC int32 __vbc_dma_da_chan_cfg(DMA_CHNCFG_T * pCfg,uint32 chan)
{
    int32 ret               = AUDIO_HAL_SUCCESS;

    pCfg->type              = DMA_CHN_TYPE_HARD;
    pCfg->priority          = DMA_CHN_PRIORITY_3;
    pCfg->workmode          = DMA_CHN_WORKMODE_NORMAL;
    pCfg->src_datawidth     = DMA_DATAWIDTH_HALFWORD;
    pCfg->dest_datawidth    = DMA_DATAWIDTH_HALFWORD;
    if( ( AUDIO_CHAN_L==(AUDIO_CHAN_L & (chan)) ) )
    {
        pCfg->dest_address  = (uint32) VBDAL;
    }
    else if( ( AUDIO_CHAN_R==(AUDIO_CHAN_R & (chan)) ) )
    {
        pCfg->dest_address  = (uint32) VBDAR;
    }
    else
    {
        //VBC_PHY_PRT:"[AUDDRV][VBC] dac pls set cfg chan."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VBC_PHY_V5_439_112_2_17_23_3_19_365,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    pCfg->requestmode       = DMA_CHN_REQMODE_NORMAL;
    pCfg->block_length      = (VB_DA_BUF_SIZE << 1);
    pCfg->src_step          = 2;  // src increament
    pCfg->dest_step         = 0;  // des nochange
    pCfg->src_blockmode     = DMA_BLOCKMODE_INCR4;
    pCfg->dest_blockmode    = DMA_BLOCKMODE_SINGLETRANS;

    return ret;
}

PUBLIC int32 __vbc_dma_ad_chan_cfg(DMA_CHNCFG_T * pCfg,uint32 chan)
{
    int32 ret               = AUDIO_HAL_SUCCESS;

    pCfg->type              = DMA_CHN_TYPE_HARD;
    pCfg->priority          = DMA_CHN_PRIORITY_3;
    pCfg->workmode          = DMA_CHN_WORKMODE_NORMAL;
    pCfg->src_datawidth     = DMA_DATAWIDTH_HALFWORD;
    pCfg->dest_datawidth    = DMA_DATAWIDTH_HALFWORD;
    if( ( AUDIO_CHAN_L==(AUDIO_CHAN_L & (chan)) ) )
    {
        pCfg->src_address   = (uint32) (VBADL);
    }
    else if( ( AUDIO_CHAN_R==(AUDIO_CHAN_R & (chan)) ) )
    {
        pCfg->src_address   = (uint32) (VBADR);
    }
    else
    {
        //VBC_PHY_PRT:"[AUDDRV][VBC] adc pls set cfg chan."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VBC_PHY_V5_471_112_2_17_23_3_19_366,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    pCfg->requestmode       = DMA_CHN_REQMODE_NORMAL;
    pCfg->block_length      = (VB_AD_BUF_SIZE << 1);
    pCfg->src_step          = 0;  // des increament
    pCfg->dest_step         = 2;  // src nochange
    pCfg->src_blockmode     = DMA_BLOCKMODE_SINGLETRANS;
    pCfg->dest_blockmode    = DMA_BLOCKMODE_INCR4;
    return ret;
}


PUBLIC int32 __vbc_dma_dac_chan_en(DMA_CHNCFG_T * pCfg,uint32 chan_id)
{
    int32 ret               = AUDIO_HAL_SUCCESS;
    DMA_CAHCECONFIG_T       cacheConfig[1];

    VBC_PHY_DBG_ASS((pCfg->total_length));/*assert verified*/
    VBC_PHY_DBG_ASS((pCfg->block_length));/*assert verified*/
    VBC_PHY_DBG_ASS((pCfg->block_length)<=(VB_DA_BUF_SIZE<<1));/*assert verified*/
    VBC_PHY_DBG_ASS(0==((pCfg->total_length)%(pCfg->block_length)));/*assert verified*/

    if( 0==((pCfg->total_length)%(pCfg->block_length)) )
    {
        DMA_HAL_ChnConfig(chan_id,pCfg);
        cacheConfig[0].bufferAddr   = (uint32) pCfg->src_address;
        cacheConfig[0].bufferSize   = pCfg->total_length;
        cacheConfig[0].dir          = DMABUFFER_TO_DEVICE;
        DMA_HAL_ChnEnable_Ex(chan_id, SCI_TRUE, cacheConfig, 1);
    }
    else
    {
        //VBC_PHY_PRT:"[AUDDRV][VBC] dac dma chan en fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VBC_PHY_V5_504_112_2_17_23_3_19_367,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }

    return ret;
}

PUBLIC int32 __vbc_dma_adc_chan_en(DMA_CHNCFG_T * pCfg,uint32 chan_id)
{
    int32 ret               = AUDIO_HAL_SUCCESS;
    DMA_CAHCECONFIG_T       cacheConfig[1];

    VBC_PHY_DBG_ASS((pCfg->total_length));/*assert verified*/
    VBC_PHY_DBG_ASS((pCfg->block_length));/*assert verified*/
    VBC_PHY_DBG_ASS((pCfg->block_length)<=(VB_AD_BUF_SIZE<<1));/*assert verified*/
    VBC_PHY_DBG_ASS(0==((pCfg->total_length)%(pCfg->block_length)));/*assert verified*/

    if( 0==((pCfg->total_length)%(pCfg->block_length)) )
    {
        DMA_HAL_ChnConfig(chan_id,pCfg);
        cacheConfig[0].bufferAddr   = (uint32) pCfg->dest_address;
        cacheConfig[0].bufferSize   = pCfg->total_length;
        cacheConfig[0].dir          = DMABUFFER_FROM_DEVICE;

        DMA_HAL_ChnEnable_Ex(chan_id, SCI_TRUE, cacheConfig, 1);
    }
    else
    {
        //VBC_PHY_PRT:"[AUDDRV][VBC] adc dma chan en fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VBC_PHY_V5_532_112_2_17_23_3_19_368,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }

    return ret;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


