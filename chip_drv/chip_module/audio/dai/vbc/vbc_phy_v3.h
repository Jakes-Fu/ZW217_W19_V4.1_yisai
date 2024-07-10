/******************************************************************************
 ** File Name:      vbc_phy_v3.h                                              *
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
#ifndef _VBC_PHY_V3_H_
#define _VBC_PHY_V3_H_

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
#define VBINTTYPE               (ARM_VBC_BASE+0x0034)

#define DAPATHCTL               (ARM_VBC_BASE+0x0040)
#define DADGCTL                 (ARM_VBC_BASE+0x0044)
#define DAHPCTL                 (ARM_VBC_BASE+0x0048)
#define DAALCCTL0               (ARM_VBC_BASE+0x004C)
#define DAALCCTL1               (ARM_VBC_BASE+0x0050)
#define DAALCCTL2               (ARM_VBC_BASE+0x0054)
#define DAALCCTL3               (ARM_VBC_BASE+0x0058)
#define DAALCCTL4               (ARM_VBC_BASE+0x005C)
#define DAALCCTL5               (ARM_VBC_BASE+0x0060)
#define DAALCCTL6               (ARM_VBC_BASE+0x0064)
#define DAALCCTL7               (ARM_VBC_BASE+0x0068)
#define DAALCCTL8               (ARM_VBC_BASE+0x006C)
#define DAALCCTL9               (ARM_VBC_BASE+0x0070)
#define DAALCCTL10              (ARM_VBC_BASE+0x0074)
#define STCTL0                  (ARM_VBC_BASE+0x0078)
#define STCTL1                  (ARM_VBC_BASE+0x007C)
#define ADPATHCTL               (ARM_VBC_BASE+0x0080)
#define ADDGCTL                 (ARM_VBC_BASE+0x0084)

#define HPCOEF0                 (ARM_VBC_BASE+0x0100)
#define HPCOEF1                 (ARM_VBC_BASE+0x0104)
#define HPCOEF2                 (ARM_VBC_BASE+0x0108)
#define HPCOEF3                 (ARM_VBC_BASE+0x010C)
#define HPCOEF4                 (ARM_VBC_BASE+0x0110)
#define HPCOEF5                 (ARM_VBC_BASE+0x0114)
#define HPCOEF6                 (ARM_VBC_BASE+0x0118)
#define HPCOEF7                 (ARM_VBC_BASE+0x011C)
#define HPCOEF8                 (ARM_VBC_BASE+0x0120)
#define HPCOEF9                 (ARM_VBC_BASE+0x0124)
#define HPCOEF10                (ARM_VBC_BASE+0x0128)
#define HPCOEF11                (ARM_VBC_BASE+0x012C)
#define HPCOEF12                (ARM_VBC_BASE+0x0130)
#define HPCOEF13                (ARM_VBC_BASE+0x0134)
#define HPCOEF14                (ARM_VBC_BASE+0x0138)
#define HPCOEF15                (ARM_VBC_BASE+0x013C)
#define HPCOEF16                (ARM_VBC_BASE+0x0140)
#define HPCOEF17                (ARM_VBC_BASE+0x0144)
#define HPCOEF18                (ARM_VBC_BASE+0x0148)
#define HPCOEF19                (ARM_VBC_BASE+0x014C)
#define HPCOEF20                (ARM_VBC_BASE+0x0150)
#define HPCOEF21                (ARM_VBC_BASE+0x0154)
#define HPCOEF22                (ARM_VBC_BASE+0x0158)
#define HPCOEF23                (ARM_VBC_BASE+0x015C)
#define HPCOEF24                (ARM_VBC_BASE+0x0160)
#define HPCOEF25                (ARM_VBC_BASE+0x0164)
#define HPCOEF26                (ARM_VBC_BASE+0x0168)
#define HPCOEF27                (ARM_VBC_BASE+0x016C)
#define HPCOEF28                (ARM_VBC_BASE+0x0170)
#define HPCOEF29                (ARM_VBC_BASE+0x0174)
#define HPCOEF30                (ARM_VBC_BASE+0x0178)
#define HPCOEF31                (ARM_VBC_BASE+0x017C)
#define HPCOEF32                (ARM_VBC_BASE+0x0180)
#define HPCOEF33                (ARM_VBC_BASE+0x0184)
#define HPCOEF34                (ARM_VBC_BASE+0x0188)
#define HPCOEF35                (ARM_VBC_BASE+0x018C)
#define HPCOEF36                (ARM_VBC_BASE+0x0190)
#define HPCOEF37                (ARM_VBC_BASE+0x0194)
#define HPCOEF38                (ARM_VBC_BASE+0x0198)
#define HPCOEF39                (ARM_VBC_BASE+0x019C)
#define HPCOEF40                (ARM_VBC_BASE+0x01A0)
#define HPCOEF41                (ARM_VBC_BASE+0x01A4)
#define HPCOEF42                (ARM_VBC_BASE+0x01A8)


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


///VBC ALG CONFIG

//DAPATHCTL
#define VBDAPATH_DA0_ADDFM_SHIFT    (0)
#define VBDAPATH_DA0_ADDFM_MASK     (0x3<<VBDAPATH_DA0_ADDFM_SHIFT)
#define VBDAPATH_DA1_ADDFM_SHIFT    (2)
#define VBDAPATH_DA1_ADDFM_MASK     (0x3<<VBDAPATH_DA1_ADDFM_SHIFT)
#define VBDAPATH_DA0_ADDST_SHIFT    (4)
#define VBDAPATH_DA0_ADDST_MASK     (0x3<<VBDAPATH_DA0_ADDST_SHIFT)
#define VBDAPATH_DA1_ADDST_SHIFT    (6)
#define VBDAPATH_DA1_ADDST_MASK     (0x3<<VBDAPATH_DA1_ADDST_SHIFT)


//DADGCTL
#define VBDADG_DG0_SHIFT            (0)
#define VBDADG_EN0_SHIFT            (7)
#define VBDADG_DG1_SHIFT            (8)
#define VBDADG_EN1_SHIFT            (15)
//DAHPCTL
#define  VBDAHP_LIMIT_SHIFT         (0)
#define  VBDAHP_WID_SEL_SHIFT       (8)
#define  VBDAHP_REG_CLR_SHIFT       (9)
#define  VBDAHP_EN_SHIFT            (10)
#define  VBDAALC_EN_SHIFT           (11)
//STCTL0
#define  VBST_HPF_COEFF0_SHIFT      (0)
#define  VBST_HPF_DG0_SHIFT         (4)
#define  VBST_HPF_EN0_SHIFT         (11)
#define  VBST_EN0_SHIFT             (12)
//STCTL1
#define  VBST_HPF_COEFF1_SHIFT      (0)
#define  VBST_HPF_DG1_SHIFT         (4)
#define  VBST_HPF_EN1_SHIFT         (11)
#define  VBST_EN1_SHIFT             (12)
//ADPATHCTL
#define  VBADPATH_AD0_INMUX_SHIFT   (0)
#define  VBADPATH_AD1_INMUX_SHIFT   (2)

#define  VBADPATH_AD0_DGMUX         (BIT_4)
#define  VBADPATH_AD1_DGMUX         (BIT_5)
//ADDGCTL
#define VBADDG_DG0_SHIFT            (0)
#define VBADDG_EN0_SHIFT            (7)
#define VBADDG_DG1_SHIFT            (8)
#define VBADDG_EN1_SHIFT            (15)

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_VBC_PHY_V3_H_
