/******************************************************************************
 ** File Name:      vbc_phy_v0.h                                              *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Rigister definition for physical layer.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/
#ifndef _VBC_PHY_V0_H_
#define _VBC_PHY_V0_H_

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
// Voiceband registers
#define VBDA0                   (ARM_VBC_BASE+0x0000)
#define VBDA1                   (ARM_VBC_BASE+0x0004)
#define VBAD0                   (ARM_VBC_BASE+0x0008)
#define VBAD1                   (ARM_VBC_BASE+0x000C)
#define VBBUFFERSIZE            (ARM_VBC_BASE+0x0010)
#define VBADBUFFDTA             (ARM_VBC_BASE+0x0014)
#define VBDABUFFDTA             (ARM_VBC_BASE+0x0018)
#define VBADCNT                 (ARM_VBC_BASE+0x001C)
#define VBDACNT                 (ARM_VBC_BASE+0x0020)
#define VBDAICTL                (ARM_VBC_BASE+0x0024)
#define VBDAIIN                 (ARM_VBC_BASE+0x0028)
#define VBDAIOUT                (ARM_VBC_BASE+0x002C)
#define VBMCCTL                 (ARM_VBC_BASE+0x0030)
#define VBINTTYPE               (ARM_VBC_BASE+0x0034)
#define HPCTL                   (ARM_VBC_BASE+0x0040)
#define HPCOEF0                 (ARM_VBC_BASE+0x0044)
#define HPCOEF1                 (ARM_VBC_BASE+0x0048)
#define HPCOEF2                 (ARM_VBC_BASE+0x004c)
#define HPCOEF3                 (ARM_VBC_BASE+0x0050)
#define HPCOEF4                 (ARM_VBC_BASE+0x0054)
#define HPCOEF5                 (ARM_VBC_BASE+0x0058)
#define HPCOEF6                 (ARM_VBC_BASE+0x005c)
#define HPCOEF7                 (ARM_VBC_BASE+0x0060)
#define HPCOEF8                 (ARM_VBC_BASE+0x0064)
#define HPCOEF9                 (ARM_VBC_BASE+0x0068)
#define HPCOEF10                (ARM_VBC_BASE+0x006c)
#define HPCOEF11                (ARM_VBC_BASE+0x0070)
#define HPCOEF12                (ARM_VBC_BASE+0x0074)
#define HPCOEF13                (ARM_VBC_BASE+0x0078)
#define HPCOEF14                (ARM_VBC_BASE+0x007c)


// VBBUFFSIZE
#define VBADBUFFSIZE_SHIFT          (0)
#define VBADBUFFSIZE_MASK           (0xFF<<VBADBUFFSIZE_SHIFT)
#define VBDABUFFSIZE_SHIFT          (8)
#define VBDABUFFSIZE_MASK           (0xFF<<VBDABUFFSIZE_SHIFT)

// VBADBUFFDTA
#define VBISADCK_INV                (BIT_9)
#define VBISDACK_INV                (BIT_10)
#define VBIIS_DLOOP                 (BIT_13)
#define VBPCM_MODE                  (BIT_14)
#define VBIIS_LRCK                  (BIT_15)

// VBDABUFFDTA
#define VBRAMSW_NUMBER              (BIT_9)
#define VBRAMSW_EN                  (BIT_10)
#define VBAD0DMA_EN                 (BIT_11)
#define VBAD1DMA_EN                 (BIT_12)
#define VBDA0DMA_EN                 (BIT_13)
#define VBDA1DMA_EN                 (BIT_14)
#define VBENABLE                    (BIT_15)


// High pass filter
#define  VBHP_LIMIT_SHIFT       (0)
#define  VBHP_LIMIT_MASK        (0xFF << VBHP_LIMIT_SHIFT)
#define  VBHP_WID_SEL_SHIFT     (8)
#define  VBHP_WID_SEL_MASK      (1<<VBHP_WID_SEL_SHIFT)
#define  VBHP_WID_SEL_16BITS    (0<<VBHP_WID_SEL_SHIFT)
#define  VBHP_WID_SEL_24BITS    (1<<VBHP_WID_SEL_SHIFT)
#define  VBHP_REG_CLR_SHIFT     (9)
#define  VBHP_REG_CLR_MASK      (1<<VBHP_REG_CLR_SHIFT)
#define  VBHP_REG_CLR_ON        (1<<VBHP_REG_CLR_SHIFT)
#define  VBHP_REG_CLR_OFF       (0<<VBHP_REG_CLR_SHIFT)
#define  VBHP_EN_SHIFT          (10)
#define  VBHP_EN_MASK           (1<<VBHP_EN_SHIFT)
#define  VBHP_EN_ENABLE         (1<<VBHP_EN_SHIFT)
#define  VBHP_EN_DISABLE        (0<<VBHP_EN_SHIFT)

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_VBC_PHY_V0_H_
