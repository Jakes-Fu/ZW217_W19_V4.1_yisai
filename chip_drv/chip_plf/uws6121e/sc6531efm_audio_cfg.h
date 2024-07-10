/******************************************************************************
 ** File Name:      sc6531efm_audio_cfg.h                                        *
 ** Author:         ken.kuang                                                 *
 ** DATE:           24/09/2011                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Configurations of audio device                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/09/2011     ken.kuang         Create.                                  *
 ******************************************************************************/
#ifndef _SC6531EFM_AUDIO_CFG_H_
#define _SC6531EFM_AUDIO_CFG_H_

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "../export/inc/adi_hal_internal.h"
#include "uws6121e_reg_analog.h"
#include "sc6531efm_reg_global.h"
#include "sc6531efm_reg_aon.h"
#include "../common/inc/chip_drv_common_io.h"

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

/**---------------------------------------------------------------------------*
 **                         GLOBAL defination                                 *
 **---------------------------------------------------------------------------*/

#if !defined (RELEASE_INFO)
#define AUDIO_HAL_DEBUG
#define CODEC_DEBUG
#define VBC_DEBUG
#define I2S_DAI_DEBUG
#define PCM_DAI_DEBUG
#endif

#define AUDIO_PLAYBACK_AUTO_TRIGGER 80   //80%

#define AUDIO_PLAYBACK_BUFFER_DIV 10   //BUFFER DIV
#define AUDIO_CAPTRUE_BUFFER_DIV  10   //BUFFER DIV

#define PGA_SMOOTH_ADJUST 1
#define PGA_SMOOTH_ADJUST_STEP 5   //UNIT:MS
#define CLOSE_DELAY_TIME    2
#define SKIP_SMOOTH_ADJ

#define MIC_BAIS_LDO_USE_FOR_EXT 0

/**---------------------------------------------------------------------------*
 **                         VBC defination                                    *
 **---------------------------------------------------------------------------*/
//MUST call by the same thread.
#define VB_WRITE_BITS(_Reg, _Value, _Mask)                  \
do{                                                         \
    uint32 local_tem_val = CHIP_REG_GET((uint32)(_Reg));    \
    local_tem_val &= (uint32)(~(_Mask));                    \
    local_tem_val |= (uint32)((_Value)&(_Mask));            \
    CHIP_REG_SET(_Reg, local_tem_val);                      \
}while(0)

#define VB_DA_BUF_SIZE              (160)   // VB buffer size in word
#define VB_AD_BUF_SIZE              (160)   // VB buffer size in word

#define VB_SUPPORT_DATA_FORMAT (AUDIO_DATA_FMT_LL_RR | AUDIO_DATA_FMT_MONO)
#define VB_DA_DATA_FORMAT (AUDIO_DATA_FMT_LL_RR)
#define VB_AD_DATA_FORMAT (AUDIO_DATA_FMT_MONO)

#define VBC_USE_DMA_TRIGGER

#if defined (VBC_USE_DMA_TRIGGER)
#define DMA_VB_AD0_OFFSET                      0
#define DMA_VB_AD1_OFFSET                      1
#define DMA_VB_DA0_OFFSET                      2
#define DMA_VB_DA1_OFFSET                      3

#define VBC_REQ_ID_ADDR(_req_offset) (REG_DMA_BASE + 0x40 + (_req_offset)*0x04)

#define VBC_DMA_NO_CHN_VAL 0xFFFFFFFF

#define VBC_USE_DMA_MODE DMA_CHN_TRSF_TYPE_STANDARD

#define VBC_DMA_ALLOC(_req_id,_chn_id)                   \
    DRV_MACRO_START                                     \
    if( VBC_DMA_NO_CHN_VAL == _chn_id ){                 \
        _chn_id = DMA_HAL_AllocChn(VBC_USE_DMA_MODE);    \
        CHIP_REG_SET(VBC_REQ_ID_ADDR(_req_id),_chn_id);  \
    }                                                   \
    DRV_MACRO_END

#define VBC_DMA_FREE(_req_id,_chn_id)                    \
    DRV_MACRO_START                                     \
    if( VBC_DMA_NO_CHN_VAL != _chn_id ){                 \
        CHIP_REG_SET(VBC_REQ_ID_ADDR(_req_id),0);        \
        DMA_HAL_FreeChn(_chn_id);                       \
        _chn_id = VBC_DMA_NO_CHN_VAL;                    \
    }                                                   \
    DRV_MACRO_END

#endif


#define VB_CLK_CTL              APB_PERI_CTL0


#define VB_INT_MASK_BASE        APB_INT_STS0
#define VB_DA_INT_MASK          VBCDA_IRQ
#define VB_AD_INT_MASK          VBCAD_IRQ

#define VB_INT_CLR_BASE         APB_INT_SET_CLR0
#define VB_DA_INT_CLR_MASK      VBCDA_IRQ_CLR
#define VB_AD_INT_CLR_MASK      VBCAD_IRQ_CLR

#define ARM_VB_ADCON            ARM_VB_ADC0ON
#define ARM_VB_AD1ON            ARM_VB_ADC1ON

#define VB_SWTICH_ARM_CTL(x)    do{vb_switch_arm_ctl(x);}while(0)
#define VB_PHY_PCLK_ENALBE_CTL(x)
#define VB_PHY_RESET_M()        do{vbc_phy_reset();}while(0)


static __inline void vbc_phy_reset (void)
{
    volatile uint32 i = 0;
    //VBC Soft Reset
    REG32 (APB_RST0_SET) = VBC_SOFT_RST_SET;

    for (i = 0; i < 10; i++);

    REG32 (APB_RST0_CLR) = VBC_SOFT_RST_CLR;
}


// Control switching between ARM and DSP
static __inline void vb_switch_arm_ctl (BOOLEAN is_arm)
{
    if (is_arm)
    {
        REG32 (VB_CLK_CTL) |= ARM_VB_ACC;
    }
    else
    {
        REG32 (VB_CLK_CTL) &= ~ARM_VB_ACC;
    }
}


/**---------------------------------------------------------------------------*
 **                         CODEC defination                                  *
 **---------------------------------------------------------------------------*/
#define SPRD_CODEC_DP_REG_BASE AUDIO_BASE
#define SPRD_CODEC_AP_REG_BASE ANA_AUDIO_BASE


/*
 * use INT for DELAY , need timeout.
*/
#define USE_INT_DELAY           1

#if (USE_INT_DELAY)
#define SPRD_CODEC_DACMUTE_INT         (TB_AUD_INT)
#endif

//MUST call by the same thread.
#ifndef FPGA_TEST
#define SPRD_CODEC_AP_WRITE_BITS(_Reg, _Value, _Mask) ANA_REG_MSK_OR((uint32)(_Reg), _Value, _Mask)
#define SPRD_CODEC_AP_REG_SET(_Reg,_Value)            ANA_REG_SET((uint32)(_Reg),_Value)
#define SPRD_CODEC_AP_REG_GET(_Reg)                   ANA_REG_GET((uint32)(_Reg))
#else
#define SPRD_CODEC_AP_WRITE_BITS(_Reg, _Value, _Mask) ANA_REG_MSK_OR_AUDIO((uint32)(_Reg), _Value, _Mask)
#define SPRD_CODEC_AP_REG_SET(_Reg,_Value)            ANA_REG_SET_AUDIO((uint32)(_Reg),_Value)
#define SPRD_CODEC_AP_REG_GET(_Reg)                   ANA_REG_GET_AUDIO((uint32)(_Reg))
#endif

#define SPRD_CODEC_DP_WRITE_BITS(_Reg, _Value, _Mask)       \
do{                                                         \
    uint32 local_tem_val = CHIP_REG_GET((uint32)(_Reg));    \
    local_tem_val &= (uint32)(~(_Mask));                    \
    local_tem_val |= (uint32)((_Value)&(_Mask));            \
    CHIP_REG_SET(_Reg, local_tem_val);                      \
}while(0)

#define SPRD_CODEC_DP_REG_SET(_Reg,_Value)            CHIP_REG_SET((uint32)(_Reg),_Value)
#define SPRD_CODEC_DP_REG_GET(_Reg)                   CHIP_REG_GET((uint32)(_Reg))

#define SPRD_CODEC_SWTICH_ARM_CTL(x)               do{sprd_codec_switch_arm_ctl(x);}while(0)
#define SPRD_CODEC_PHY_REG_EN(x)                   do{sprd_codec_reg_mclk(x);}while(0)
#define SPRD_CODEC_PHY_MCLK_M(x)                   do{sprd_codec_mclk(x);}while(0)
#define SPRD_CODEC_IS_ARM_CTL()                    sprd_codec_is_arm_ctl()
#define SPRD_CODEC_PHY_RESET_M()                   do{sprd_codec_phy_reset();}while(0)
#define SPRD_CODEC_AUDIF_EN(x,is_auto)

/* LDO SLP Control */
#define SPRD_CODEC_VCM_SLP(auto_close)          do{LDO_AutoCloseInDeepSleep(SLP_LDO_AUDIO_VCM,0);}while(0)
#define SPRD_CODEC_VCMBUF_SLP(auto_close)       do{LDO_AutoCloseInDeepSleep(SLP_LDO_AUDIO_VCMBUF,0);}while(0)
#define SPRD_CODEC_BG_SLP(auto_close)           do{LDO_AutoCloseInDeepSleep(SLP_LDO_AUDIO_BG,0);}while(0)
#define SPRD_CODEC_VB_SLP(auto_close)           do{LDO_AutoCloseInDeepSleep(SLP_LDO_VB,0);}while(0)
#define SPRD_CODEC_VBO_SLP(auto_close)          do{LDO_AutoCloseInDeepSleep(SLP_LDO_VBO,0);}while(0)
#define SPRD_CODEC_BG_I_SLP(auto_close)         do{}while(0)
#define SPRD_CODEC_MICBIAS_SLP(auto_close)      do{LDO_AutoCloseInDeepSleep(SLP_LDO_AUDIO_MICBIAS,0);}while(0)
#define SPRD_CODEC_AUXMICBIAS_SLP(auto_close)   do{}while(0)


#define SPRD_CODEC_CTRL_BY_DSP                     AUDIO_FUN_DSP_CTL_CODEC

static __inline void sprd_codec_phy_reset (void)
{
    volatile uint32 i = 0;
    //AUDIF Soft Reset
    REG32 (APB_RST0_SET) = AUD_SOFT_RST_SET;
    ANA_REG_OR(ANA_SOFT_RST0,ANA_AUD_IF_SOFT_RST);

    for (i = 0; i < 10; i++){};
    
    REG32 (APB_RST0_CLR) = AUD_SOFT_RST_CLR;
    ANA_REG_AND(ANA_SOFT_RST0,~ANA_AUD_IF_SOFT_RST);
}

static __inline uint32 sprd_codec_is_arm_ctl(void)
{
    if( REG32(APB_PERI_CTL0) & (AUD_CTRL_SEL | CLK_AUD_ARM_CTRL) )
    {
        return 1;
    }
    return 0;
}

// Control switching between ARM and DSP
static __inline void sprd_codec_switch_arm_ctl (uint32 is_arm)
{
    if (is_arm)
    {
        REG32 (APB_PERI_CTL0) |= (AUD_CTRL_SEL | CLK_AUD_ARM_CTRL);
    }
    else
    {
        REG32 (APB_PERI_CTL0) &= ~(AUD_CTRL_SEL | CLK_AUD_ARM_CTRL);
    }
}

static __inline void sprd_codec_reg_mclk(uint32 en)
{
    if( en )
    {
        REG32( APB_EB0_SET ) = AUD_EB_SET; //d-die
        //bringup_temp   ANA_REG_OR(ANA_ARM_CLK_EN0,ANA_CLK_AUD_IF_EN);
    }
    else
    {
        REG32( APB_EB0_CLR ) = AUD_EB_CLR; //d-die
        //bringup_temp   ANA_REG_AND(ANA_ARM_CLK_EN0,~ANA_CLK_AUD_IF_EN);
    }
}


static __inline void sprd_codec_mclk(uint32 en)
{
    if( en )
    {
          ANA_REG_OR(ANA_DIG_CLK_EN0_RTC,ANA_CLK_AUD_IF_EN);
     	 //bringup_temp   ANA_REG_OR(ANA_ARM_CLK_EN0,ANA_CLK_AUD_IF_6P5M_EN);
    }
    else
    {
          ANA_REG_AND(ANA_DIG_CLK_EN0_RTC,~ANA_CLK_AUD_IF_EN);
          //bringup_temp   ANA_REG_AND(ANA_ARM_CLK_EN0,~ANA_CLK_AUD_IF_6P5M_EN);
    }
}


/**---------------------------------------------------------------------------*
 **                         I2S/PCM defination                                *
 **---------------------------------------------------------------------------*/

#define IIS_PCM_RESET()             do{iis_pcm_reset();}while(0)
#define IIS_PCM_SWITCH_TO_ARM(id,x) do{iis_pcm_switch_ctrl(x);}while(0)

static __inline void iis_pcm_reset(void)
{
    volatile uint32 i = 0;

    //IIS Soft Reset
    REG32(APB_RST0_SET) = IIS_SOFT_RST_SET_MCU;

    for (i = 0; i < 10; i++);

    REG32(APB_RST0_CLR) = IIS_SOFT_RST_CLR_MCU;

}


static __inline void iis_pcm_switch_ctrl (BOOLEAN is_arm)
{
    if (is_arm)
    {
        IIS_PCM_RESET();

        //IIS Access Enable for MCU
        REG32 (APB_EB0_SET) = IIS_EB_SET;
        REG32 (APB_CLK_CFG1) &= ~CLK_IIS_DSP_CTL_SEL;
        //Select ARM Access IIS
        REG32 (APB_PERI_CTL0) &= ~IIS_MUX_SEL;
    }
    else
    {
        //IIS Access Dsiable for MCU
        REG32 (APB_EB0_CLR) = IIS_EB_CLR;
        REG32 (APB_CLK_CFG1) |= CLK_IIS_DSP_CTL_SEL;
        //Select DSP Access IIS
        REG32 (APB_PERI_CTL0) |= IIS_MUX_SEL;


        IIS_PCM_RESET();
    }
}

#define IIS_USE_DMA_TRIGGER 1

#if defined (IIS_USE_DMA_TRIGGER)
#define DMA_IIS_TX_OFFSET                      0
#define DMA_IIS_RX_OFFSET                      1

#define IIS_REQ_ID_ADDR(_req_offset) (REG_DMA_BASE + 0x40 + (_req_offset)*0x04)

#define IIS_DMA_NO_CHN_VAL 0xFFFFFFFF

#define IIS_USE_DMA_MODE DMA_CHN_TRSF_TYPE_STANDARD

#define IIS_DMA_ALLOC(_req_id,_chn_id)                   \
    DRV_MACRO_START                                      \
    if( IIS_DMA_NO_CHN_VAL == _chn_id ){                 \
        _chn_id = DMA_HAL_AllocChn(IIS_USE_DMA_MODE);    \
        CHIP_REG_SET(IIS_REQ_ID_ADDR(_req_id),_chn_id);  \
    }                                                   \
    DRV_MACRO_END

#define IIS_DMA_FREE(_req_id,_chn_id)                    \
    DRV_MACRO_START                                      \
    if( IIS_DMA_NO_CHN_VAL != _chn_id ){                 \
        CHIP_REG_SET(IIS_REQ_ID_ADDR(_req_id),0);        \
        DMA_HAL_FreeChn(_chn_id);                        \
        _chn_id = IIS_DMA_NO_CHN_VAL;                    \
    }                                                    \
    DRV_MACRO_END

#endif

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

#define DOLPHIN_REG_BASE    0x0

//MUST call by the same thread.
#define DOL_WRITE_BITS(_Reg, _Value, _Mask)
#define DOL_REG_GET(_Reg)

#define DOL_IS_ARM_CTL()
#define DOL_LINEIN_REC_EN(x)
#define DOL_LINEIN_REC_S(x)
#define DOL_PHY_RESET_M()

#define DOL_CTRL_BY_DSP                     AUDIO_FUN_DSP

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_SC6531EFM_AUDIO_CFG_H_
