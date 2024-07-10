/******************************************************************************
 ** File Name:      sprd_codec_ap_phy.h                                    *
 ** Author:         zhenfang.wang                                       *
 ** DATE:           8/5/2015                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec ap control  for sc2713                                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 6/3/2016     ye.zhang             Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifndef __SPRD_CODEC_AP_PHY_SC2713_H___
#define __SPRD_CODEC_AP_PHY_SC2713_H___

#define ADIE_ARM_MODULE_EN (0x82001800)
#define MODULE_AUD_EN (BIT_4)
#define ANA_BASE_ADDR ADIE_ARM_MODULE_EN

#define    REG_ANA_ARM_MODULE_EN          ( ANA_BASE_ADDR + 0x0000 )
#define    REG_ANA_ARM_CLK_EN             ( ANA_BASE_ADDR + 0x0004 )
#define    REG_ANA_RTC_CLK_EN             ( ANA_BASE_ADDR + 0x0008 )
#define    REG_ANA_ARM_RST                ( ANA_BASE_ADDR + 0x000C )
#define    REG_ANA_LDO_DCDC_PD_RTCSET     ( ANA_BASE_ADDR + 0x0010 )
#define    REG_ANA_LDO_DCDC_PD_RTCCLR     ( ANA_BASE_ADDR + 0x0014 )
#define    REG_ANA_RTC_CTRL               ( ANA_BASE_ADDR + 0x0018 )
#define    REG_ANA_LDO_PD_CTRL            ( ANA_BASE_ADDR + 0x001C )
#define    REG_ANA_LDO_V_CTRL0            ( ANA_BASE_ADDR + 0x0020 )
#define    REG_ANA_LDO_V_CTRL1            ( ANA_BASE_ADDR + 0x0024 )
#define    REG_ANA_LDO_V_CTRL2            ( ANA_BASE_ADDR + 0x0028 )
#define    REG_ANA_LDO_CAL_CTRL0          ( ANA_BASE_ADDR + 0x002C )
#define    REG_ANA_LDO_CAL_CTRL1          ( ANA_BASE_ADDR + 0x0030 )
#define    REG_ANA_LDO_CAL_CTRL2          ( ANA_BASE_ADDR + 0x0034 )
#define    REG_ANA_LDO_CAL_CTRL3          ( ANA_BASE_ADDR + 0x0038 )
#define    REG_ANA_LDO_CAL_CTRL4          ( ANA_BASE_ADDR + 0x003C )
#define    REG_ANA_LDO_CAL_CTRL5          ( ANA_BASE_ADDR + 0x0040 )
#define    REG_ANA_LDO_CAL_CTRL6          ( ANA_BASE_ADDR + 0x0044 )
#define    REG_ANA_AUXAD_CTL              ( ANA_BASE_ADDR + 0x0048 )
#define    REG_ANA_DCDC_CTRL0             ( ANA_BASE_ADDR + 0x004C )
#define    REG_ANA_DCDC_CTRL1             ( ANA_BASE_ADDR + 0x0050 )
#define    REG_ANA_DCDC_CTRL2             ( ANA_BASE_ADDR + 0x0054 )
#define    REG_ANA_DCDC_CTRL3             ( ANA_BASE_ADDR + 0x0058 )
#define    REG_ANA_DCDC_CTRL4             ( ANA_BASE_ADDR + 0x005C )
#define    REG_ANA_DCDC_CTRL5             ( ANA_BASE_ADDR + 0x0060 )
#define    REG_ANA_DCDC_CTRL6             ( ANA_BASE_ADDR + 0x0064 )
#define    REG_ANA_DDR2_CTRL              ( ANA_BASE_ADDR + 0x0068 )
#define    REG_ANA_SLP_WAIT_DCDCARM       ( ANA_BASE_ADDR + 0x006C )
#define    REG_ANA_LDO1828_XTL_CTL        ( ANA_BASE_ADDR + 0x0070 )
#define    REG_ANA_LDO_SLP_CTRL0          ( ANA_BASE_ADDR + 0x0074 )
#define    REG_ANA_LDO_SLP_CTRL1          ( ANA_BASE_ADDR + 0x0078 )
#define    REG_ANA_LDO_SLP_CTRL2          ( ANA_BASE_ADDR + 0x007C )
#define    REG_ANA_LDO_SLP_CTRL3          ( ANA_BASE_ADDR + 0x0080 )
#define    REG_ANA_AUD_SLP_CTRL4          ( ANA_BASE_ADDR + 0x0084 )
#define    REG_ANA_DCDC_SLP_CTRL          ( ANA_BASE_ADDR + 0x0088 )
#define    REG_ANA_XTL_WAIT_CTRL          ( ANA_BASE_ADDR + 0x008C )

#define REGS_CODEC_AP_BASE (0x82001000 + 0x600)
#define SPRD_CODEC_AP_REG_BASE	(REGS_CODEC_AP_BASE)
#define SCI_ADDR(base, off) ((base) + (off))

#define  AUD_AP_DIG_CFG0                SCI_ADDR(REGS_CODEC_AP_BASE,0x0000)
#define  AUD_AP_DIG_CFG1                SCI_ADDR(REGS_CODEC_AP_BASE,0x0004) 
#define  AUD_AP_DIG_CFG2                SCI_ADDR(REGS_CODEC_AP_BASE,0x0008) 
#define  AUD_AP_DIG_CFG3                SCI_ADDR(REGS_CODEC_AP_BASE,0x000C) 
#define  AUD_AP_DIG_CFG4                SCI_ADDR(REGS_CODEC_AP_BASE,0x0010)
#define  AUD_AP_DIG_STS0                SCI_ADDR(REGS_CODEC_AP_BASE,0x0014) 
#define  AUD_AP_DIG_STS1                SCI_ADDR(REGS_CODEC_AP_BASE,0x0018) 

#define  AUD_AP_ANA_CFG0                SCI_ADDR(REGS_CODEC_AP_BASE,0x0040)
#define  AUD_AP_ANA_CFG1                SCI_ADDR(REGS_CODEC_AP_BASE,0x0044) 
#define  AUD_AP_ANA_CFG2                SCI_ADDR(REGS_CODEC_AP_BASE,0x0048)
#define  AUD_AP_ANA_CFG3                SCI_ADDR(REGS_CODEC_AP_BASE,0x004C)

#define  AUD_AP_ANA_CFG4                SCI_ADDR(REGS_CODEC_AP_BASE,0x0060) 
#define  AUD_AP_ANA_CFG5                SCI_ADDR(REGS_CODEC_AP_BASE,0x0064) 
#define  AUD_AP_ANA_CFG6                SCI_ADDR(REGS_CODEC_AP_BASE,0x0068)
#define  AUD_AP_ANA_CFG7                SCI_ADDR(REGS_CODEC_AP_BASE,0x006C) 
#define  AUD_AP_ANA_CFG8                SCI_ADDR(REGS_CODEC_AP_BASE,0x0070) 
#define  AUD_AP_ANA_CFG9                SCI_ADDR(REGS_CODEC_AP_BASE,0x0074)
#define  AUD_AP_ANA_CFG10                SCI_ADDR(REGS_CODEC_AP_BASE,0x0078) 
#define  AUD_AP_ANA_CFG11                SCI_ADDR(REGS_CODEC_AP_BASE,0x007C) 
#define  AUD_AP_ANA_CFG12                SCI_ADDR(REGS_CODEC_AP_BASE,0x0080)
#define  AUD_AP_ANA_CFG13                SCI_ADDR(REGS_CODEC_AP_BASE,0x0084) 
#define  AUD_AP_ANA_CFG14                SCI_ADDR(REGS_CODEC_AP_BASE,0x0088) 
#define  AUD_AP_ANA_CFG15                SCI_ADDR(REGS_CODEC_AP_BASE,0x008C)
#define  AUD_AP_ANA_CFG16                SCI_ADDR(REGS_CODEC_AP_BASE,0x0090) 
#define  AUD_AP_ANA_CFG17               SCI_ADDR(REGS_CODEC_AP_BASE,0x0094) 
#define  AUD_AP_ANA_CFG18                SCI_ADDR(REGS_CODEC_AP_BASE,0x0098) 
#define  AUD_AP_ANA_CFG19               SCI_ADDR(REGS_CODEC_AP_BASE,0x009C) 
#define  AUD_AP_ANA_CFG20                SCI_ADDR(REGS_CODEC_AP_BASE,0x00A0)
#define  AUD_AP_ANA_CFG21                SCI_ADDR(REGS_CODEC_AP_BASE,0x00A4)

#define  AUD_AP_ANA_STS0                SCI_ADDR(REGS_CODEC_AP_BASE,0x00C0)
#define  AUD_AP_ANA_CFG22               SCI_ADDR(REGS_CODEC_AP_BASE,0x00C4) 
#define  AUD_AP_ANA_CFG23               SCI_ADDR(REGS_CODEC_AP_BASE,0x00C8)
#define  AUD_AP_ANA_STS1                SCI_ADDR(REGS_CODEC_AP_BASE,0x00CC)
#define  AUD_AP_ANA_STS2                SCI_ADDR(REGS_CODEC_AP_BASE,0x00D0)

#define  AUD_AP_ANA_CHIP_RESERVE0                SCI_ADDR(REGS_CODEC_AP_BASE,0x00D4)
#define  AUD_AP_ANA_CHIP_RESERVE1                SCI_ADDR(REGS_CODEC_AP_BASE,0x00D8)

#define MICBIAS_V_21            (0)
#define MICBIAS_V_19            (1)
#define MICBIAS_V_23            (2)
#define MICBIAS_V_25            (3)

#define AUD_BG_I_150            (0)
#define AUD_BG_I_200            (1)
#define AUD_BG_I_250            (2)
#define AUD_BG_I_300            (3)

#define AUD_VCM_V_165           (0)
#define AUD_VCM_V_17            (1)
#define AUD_VCM_V_16            (2)
#define AUD_VCM_V_145           (3)



#define PA_LDO_V_29             (0)
#define PA_LDO_V_30             (1)
#define PA_LDO_V_31             (2)
#define PA_LDO_V_32             (3)
#define PA_LDO_V_33             (4)
#define PA_LDO_V_34             (5)
#define PA_LDO_V_35             (6)
#define PA_LDO_V_36             (7)

#define AUD_OTP_T_104_74        (0)
#define AUD_OTP_T_128_98        (1)
#define AUD_OTP_T_152_122       (2)
#define AUD_OTP_T_175_144       (3)
#define AUD_OTP_T_6_N24         (4)
#define AUD_OTP_T_30_0          (5)
#define AUD_OTP_T_55_25         (6)
#define AUD_OTP_T_79_49         (7)
///..

#define AUD_OVP_T_16            (0)
#define AUD_OVP_T_8             (1)
#define AUD_OVP_T_4             (2)
#define AUD_OVP_T_2             (3)
#define AUD_OVP_T_1             (4)
#define AUD_OVP_T_05            (5)
#define AUD_OVP_T_025           (6)
#define AUD_OVP_T_012           (7)

#define AUD_OVP_V_542_518       (0)
#define AUD_OVP_V_527_503       (1)
#define AUD_OVP_V_512_488       (2)
#define AUD_OVP_V_496_473       (3)
#define AUD_OVP_V_481_459       (4)
#define AUD_OVP_V_465_444       (5)
#define AUD_OVP_V_450_429       (6)
#define AUD_OVP_V_435_415       (7)


#define ADC_PGA_G_N6            (0)
#define ADC_PGA_G_N3            (1)
#define ADC_PGA_G_0             (2)
#define ADC_PGA_G_P3            (3)
#define ADC_PGA_G_P6            (4)
#define ADC_PGA_G_P9            (5)
#define ADC_PGA_G_P12           (6)
#define ADC_PGA_G_P15           (7)
#define ADC_PGA_G_P18           (8)
#define ADC_PGA_G_P21           (9)
#define ADC_PGA_G_P24           (10)
#define ADC_PGA_G_P27           (11)
#define ADC_PGA_G_P30           (12)
#define ADC_PGA_G_P33           (13)
#define ADC_PGA_G_P36           (14)
#define ADC_PGA_G_P39           (15)

#define PA_DTRI_F_300           (0)
#define PA_DTRI_F_590           (1)
#define PA_DTRI_F_870           (2)
#define PA_DTRI_F_1000          (3)

#define PA_SS_T_1               (0)
#define PA_SS_T_1_2             (1)
#define PA_SS_T_1_4             (2)
#define PA_SS_T_1_8             (3)
#define PA_SS_T_1_16            (4)
#define PA_SS_T_1_32            (5)
#define PA_SS_T_1_64            (6)
#define PA_SS_T_1_128           (7)

#define PA_SS_F_3_2             (0)
#define PA_SS_F_9               (1)
#define PA_SS_F_22              (2)
#define PA_SS_F_47              (3)

#define DAC_HP_PGA_G_MUTE       (0)
#define DAC_HP_PGA_G_N33        (1)
#define DAC_HP_PGA_G_N30        (2)
#define DAC_HP_PGA_G_N27        (3)
#define DAC_HP_PGA_G_N24        (4)
#define DAC_HP_PGA_G_N21        (5)
#define DAC_HP_PGA_G_N18        (6)
#define DAC_HP_PGA_G_N15        (7)
#define DAC_HP_PGA_G_N12        (8)
#define DAC_HP_PGA_G_N9         (9)
#define DAC_HP_PGA_G_N6         (10)
#define DAC_HP_PGA_G_N3         (11)
#define DAC_HP_PGA_G_0          (12)
#define DAC_HP_PGA_G_P3         (13)
#define DAC_HP_PGA_G_P6         (14)
#define DAC_HP_PGA_G_P9         (15)

#define DAC_EAR_PGA_G_MUTE      (0)
#define DAC_EAR_PGA_G_N33       (1)
#define DAC_EAR_PGA_G_N30       (2)
#define DAC_EAR_PGA_G_N27       (3)
#define DAC_EAR_PGA_G_N24       (4)
#define DAC_EAR_PGA_G_N21       (5)
#define DAC_EAR_PGA_G_N18       (6)
#define DAC_EAR_PGA_G_N15       (7)
#define DAC_EAR_PGA_G_N12       (8)
#define DAC_EAR_PGA_G_N9        (9)
#define DAC_EAR_PGA_G_N6        (10)
#define DAC_EAR_PGA_G_N3        (11)
#define DAC_EAR_PGA_G_0         (12)
#define DAC_EAR_PGA_G_P3        (13)
#define DAC_EAR_PGA_G_P6        (14)
#define DAC_EAR_PGA_G_P9        (15)

#define DAC_PA_PGA_G_MUTE       (0)
#define DAC_PA_PGA_G_N21        (1)
#define DAC_PA_PGA_G_N18        (2)
#define DAC_PA_PGA_G_N15        (3)
#define DAC_PA_PGA_G_N12        (4)
#define DAC_PA_PGA_G_N9         (5)
#define DAC_PA_PGA_G_N6         (6)
#define DAC_PA_PGA_G_N3         (7)
#define DAC_PA_PGA_G_0          (8)
#define DAC_PA_PGA_G_P3         (9)
#define DAC_PA_PGA_G_P6         (10)
#define DAC_PA_PGA_G_P9         (11)
#define DAC_PA_PGA_G_P12        (12)
#define DAC_PA_PGA_G_P15        (13)
#define DAC_PA_PGA_G_P18        (14)
#define DAC_PA_PGA_G_P21        (15)

#define HP_POP_STEP_012         (0)
#define HP_POP_STEP_025         (1)
#define HP_POP_STEP_05          (2)
#define HP_POP_STEP_1           (3)
#define HP_POP_STEP_2           (4)
#define HP_POP_STEP_4           (5)
#define HP_POP_STEP_8           (6)
#define HP_POP_STEP_16          (7)

#define HP_POP_CTL_DIS          (0)
#define HP_POP_CTL_UP           (1)
#define HP_POP_CTL_DOWN         (2)
#define HP_POP_CTL_HOLD         (3)

#define HP_POP_CHG_V_05         (0)
#define HP_POP_CHG_V_1          (1)
#define HP_POP_CHG_V_2          (2)
#define HP_POP_CHG_V_4          (3)

#define HP_POP_FLG_CHGING       (0)
#define HP_POP_FLG_1_2          (1)
#define HP_POP_FLG_3_4          (2)
#define HP_POP_FLG_NEAR_CMP     (3)


//DIG_CFG0 (ENB)
#define AUDIFA_DACL_EN            (BIT_0)
#define AUDIFA_ADCL_EN           (BIT_1)
#define AUDIFA_DACR_EN            (BIT_2)
#define AUDIFA_ADCR_EN           (BIT_3)

#define AUDIO_LOOP_MAP_SEL      (BIT_4)
#define AUDIO_ANA_LOOP_EN       (BIT_5)

//DIG_CFG1(OCPTMR_CTL,OTPTMR_CTL)
//audif_ocptmr_ctl
#define OCP_PRECIS_SHIFT        (0)
#define OCP_PRECIS_MASK         (0x7<<OCP_PRECIS_SHIFT)

#define OCP_PD_THD_SHIFT        (3)
#define OCP_PD_THD_MASK         (0x7<<OCP_PD_THD_SHIFT)

//audif_otptmr_ctl
#define OTP_PRECIS_SHIFT        (8)
#define OTP_PRECIS_MASK         (0x7<<OTP_PRECIS_SHIFT)

#define OTP_PD_THD_SHIFT        (11)
#define OTP_PD_THD_MASK         (0x7<<OTP_PD_THD_SHIFT)

//DIG_CFG2(SHUTDOWN_CTL,OVPTMR_CTL)
//audif_shutdown_ctl
#define HP_SHUTDOWN_EN          (BIT_0)
#define EAR_SHUTDOWN_EN         (BIT_1)
#define LOL_SHUTDOWN_EN          (BIT_2)
#define LOR_SHUTDOWN_EN         (BIT_3)
#define PA_SHUTDOWN_EN0         (BIT_4)
#define PA_SHUTDOWN_EN1         (BIT_5)
#define PA_SHUTDOWN_EN2         (BIT_6)
#define AUDIO_CLK_SEL           (BIT_7)
//audif_ovptmr_ctl
#define OVP_PRECIS_SHIFT        (8)
#define OVP_PRECIS_MASK         (0x7<<OVP_PRECIS_SHIFT)

#define OVP_PD_THD_SHIFT        (11)
#define OVP_PD_THD_MASK         (0x7<<OVP_PD_THD_SHIFT)

#define AUD_PRT_EN      (BIT_14)

/* AUDIF_INT_EN */
/* AUDIF_INT_CLR */
/* AUDIF_INT_RAW */
/* AUDIF_INT_MASK */
#define AUDIO_POP_IRQ           (BIT_7)
#define OVP_IRQ                 (BIT_6)
#define OTP_IRQ                 (BIT_5)
#define PA_OCP_IRQ              (BIT_4)
#define LOR_OCP_IRQ              (BIT_3)
#define LOL_OCP_IRQ              (BIT_2)
#define EAR_OCP_IRQ             (BIT_1)
#define HP_OCP_IRQ              (BIT_0)


//DIG_CFG3 (AUDINT CLR , AUD SHUTDOWN CLR )
//audif_shutdown_clr
#define PA_SHUTDOWN_CLR  (BIT_8)
#define HP_SHUTDOWN_CLR  (BIT_9)
#define EAR_SHUTDOWN_CLR  (BIT_10)
#define LOL_SHUTDOWN_CLR  (BIT_11)
#define LOR_SHUTDOWN_CLR  (BIT_12)
//DIG_STS0 (INT_RAW, Shutdown status)  RO
#define BT_PA_SHUTDOWN  (BIT_8)
#define BT_HP_SHUTDOWN  (BIT_9)
#define BT_EAR_SHUTDOWN  (BIT_10)
#define BT_LOL_SHUTDOWN  (BIT_11)
#define BT_LOR_SHUTDOWN  (BIT_12)

//ANA_CFG0 (PMUR1, PMUR2)
//pmur2
#define PA_SW_EN                (BIT_15)
#define PA_LDO_EN               (BIT_14)
#define PA_EN                   (BIT_13)
//#define PAR_SW_EN             (BIT_12)
//#define PAR_LDO_EN            (BIT_11)
//#define PAR_EN                (BIT_10)
#define OVP_PD                  (BIT_9)
#define OVP_LDO_EN              (BIT_8)

//pmur1
#define VB_EN                   (BIT_7)
#define VBO_EN                  (BIT_6)
#define HEADMICBIAS_EN          (BIT_5)
#define MICBIAS_V     		    (3) 
#define MICBIAS_V_MASK          (0x3 << MICBIAS_V) 
#define MICBIAS_HV_EN           (BIT_2) //Audio micbias/auxmicbias/headmicbias output high voltage enable signal
#define HEADMIC_SLEEP_EN        (BIT_1)
#define PMUR1                   (BIT_0)


//ANA_CFG1 (PMUR3, PMUR4)
//pmur4
#define MICBIAS_EN              (BIT_15)
#define AUXMICBIAS_EN           (BIT_14)
#define VCM_V                   (11)
#define VCM_V_MASK              (0x7 << VCM_V)
#define BG_I                    (9)
#define BG_I_MASK               (0x3 << BG_I)

#define BG_EN                   (BIT_7)
#define BG_RST                  (BIT_6)

//pmur3
#define BG_IBIAS_EN             (BIT_5)
#define VCM_EN                  (BIT_4)
#define VCM_BUF_EN              (BIT_3)
#define ICM_PLUS_EN             (BIT_2) 
#define SEL_VCMI                (BIT_1)  //Select VCMI to output enable signal
#define VCMI_FAST_EN            (BIT_0)  //VCMI fast charging enable signal


//ANA_CFG2 (PMUR5, PMUR6)
#define PA_SWOCP_PD             (BIT_15)
#define PA_LDOOCP_PD            (BIT_14)
#define PA_LDO_V                (11)
#define PA_LDO_V_MASK           (0x7 << PA_LDO_V)
#define VCM_CAL                 (0)
#define VCM_CAL_MASK            (0x1F << VCM_CAL)


//ANA_CFG3 (PMUR7, PMUR8)
//pmur8
#define OVP_V         (13)
#define OVP_V_MASK          (0x7<<AUD_OVP_V_SHIFT)
#define OVP_DEG_EN              (BIT_12)
#define OVP_T         (9)
#define OVP_T_MASK          (0x7<<AUD_OVP_T_SHIFT)

//pmur7
#define OTP_PD              (BIT_7)
#define OTP_T         (3)
#define OTP_T_MASK          (0x7<<AUD_OTP_T_SHIFT)


//ANA_CFG4(CCR)
//ccr
#define ADC_CLK_EN              (BIT_7)
#define ADC_CLK_RST             (BIT_6)
#define ADC_CLK_F               (4)
#define ADC_CLK_F_MASK          (0x3<<ADC_CLK_F)

#define DAC_CLK_EN              (BIT_3)
#define DAC_CLK_F               (1)
#define DAC_CLK_F_MASK          (0x3<<DAC_CLK_F)

#define DRV_CLK_EN              (BIT_0)



//ANA_CFG5(AACR1,AACR2)
//aacr2
#define ADCPGAL_BYP_LSB             (14)
#define ADCPGAL_BYP_MASK        (0x3 << ADCPGAL_BYP_LSB)
#define ADCPGAL_EN_LSB              (12)
#define ADCPGAL_EN_MASK         (0x3 << ADCPGAL_EN_LSB)
#define ADCPGAR_BYP_LSB             (10)
#define ADCPGAR_BYP_MASK        (0x3 << ADCPGAR_BYP_LSB)
#define ADCPGAR_EN_LSB              (8)
#define ADCPGAR_EN_MASK         (0x3 << ADCPGAR_EN_LSB)
//aacr1
#define ADC_IBUF_PD             (BIT_7)
#define ADC_VREF1P5             (BIT_6)
#define ADCL_PD                 (BIT_5)
#define ADCL_RST                (BIT_4)
#define ADCR_PD                 (BIT_3)
#define ADCR_RST                (BIT_2)

//ANA_CFG6(AAICR1,AAICR2)
#define MIC_ADCR                (BIT_15)
#define AUXMIC_ADCR             (BIT_14)
#define HEADMIC_ADCR            (BIT_13)
#define AIL_ADCR                (BIT_12)
#define AIR_ADCR                (BIT_11)
#define MIC_ADCL                (BIT_7)
#define AUXMIC_ADCL             (BIT_6)
#define HEADMIC_ADCL            (BIT_5)
#define AIL_ADCL                (BIT_4)
#define AIR_ADCL                (BIT_3)

//ANA_CFG7(ACGR1)  toadd ??
#define AUD_PLI_SHIFT (0)
#define AUD_PLI_MASK (0x3<<AUD_PLI_SHIFT)
#define AUD_PHEAD_SHIFT (2)
#define AUD_PHEAD_MASK (0x3<<AUD_PHEAD_SHIFT)
#define AUD_PAUX_SHIFT (4)
#define AUD_PAUX_MASK (0x3<<AUD_PAUX_SHIFT)
#define AUD_PMIC_SHIFT (6)
#define AUD_PMIC_MASK (0x3<<AUD_PMIC_SHIFT)

//ANA_CFG8 (ACGR2,ACGR3)  
#define ADPGAR_G                (8)
#define ADPGAR_G_MASK           (0x3F<<ADPGAR_G)
#define ADPGAL_G                (0)
#define ADPGAL_G_MASK           (0x3F<<ADPGAL_G)

//ANA_CFG9(DACR,DACGR)
//dacr
#define DACR_G                  (8)
#define DACR_G_MASK             (0x7 << DACR_G)
#define DACL_G                  (12)
#define DACL_G_MASK             (0x7 << DACL_G)
#define DACL_EN                 (BIT_7)
#define DACR_EN                 (BIT_6)
#define DACBUF_I_S              (BIT_5)
#define DACBUF_I_S_LSB              (5)

//ANA_CFG10(DAOCR2) 
#define ADCL_AOL                (BIT_7)
#define ADCR_AOL                (BIT_6)
#define DACL_AOL                (BIT_5)
#define DACR_AOL                (BIT_4)
#define ADCL_AOR                (BIT_3)
#define ADCR_AOR                (BIT_2)
#define DACL_AOR                (BIT_1)
#define DACR_AOR                (BIT_0)

//ANA_CFG11(DAOCR1,DAOCR3 )
//daocr1
#define DACL_EAR                (BIT_15)
#define ADCL_P_HPL              (BIT_7)
#define ADCR_P_HPL              (BIT_6)
#define DACL_P_HPL              (BIT_5)
#define DACR_P_HPL              (BIT_4)
#define ADCL_N_HPR              (BIT_3)
#define ADCR_P_HPR              (BIT_2)
#define DACL_N_HPR              (BIT_1)
#define DACR_P_HPR              (BIT_0)

//ANA_CFG12(DCR1,DCR2)
//dcr2
#define PA_D_EN                 (BIT_15)
#define PA_DTRI_F               (13)
#define PA_DTRI_F_MASK          (0x3<<PA_DTRI_F)
#define PA_DEMI_EN              (BIT_12)
#define PA_SS_EN                (BIT_11)
#define PA_SS_RST               (BIT_10)
#define DRV_STOP_EN             (BIT_9)

//dcr1
#define HPL_EN              (BIT_7)
#define HPR_EN              (BIT_6)
#define EAR_EN              (BIT_5)
#define AOL_EN              (BIT_4)
#define AOR_EN             (BIT_3)
#define DIFF_EN             (BIT_2)
#define HP_VCMI_EN     (BIT_1)


//ANA_CFG13(DCR3,DCR4)
//dcr4
#define PA_SS_F                        (14)
#define PA_SS_F_MASK            (0x3<<PA_SS_F_SHIFT)
#define PA_SS_32K_EN            (BIT_13)
#define PA_SS_T                        (10)
#define PA_SS_T_MASK            (0x7<<PA_SS_T_SHIFT)

//dcr3
#define DRV_OCP_AOR_PD       (BIT_7)
#define DRV_OCP_AOL_PD       (BIT_6)
#define DRV_OCP_EAR_PD       (BIT_5) 
#define DRV_OCP_HP_PD        (BIT_4) 

#define DRV_OCP_PA_I_500    ~(BIT_3)
#define DRV_OCP_PA_I_700    (BIT_3)
#define DRV_OCP_LO_I_4      ~(BIT_2)
#define DRV_OCP_LO_I_6      (BIT_2)
#define DRV_OCP_EAR_I_300   ~(BIT_1)
#define DRV_OCP_EAR_I_450   (BIT_1)
#define DRV_OCP_HP_I_130    ~(BIT_0) 
#define DRV_OCP_HP_I_190    (BIT_0)


//ANA_CFG14(DCR5,DCR6) toadd?
#define AUD_NG_PA_EN    (BIT_5)
#define AUD_NG_DA_EN    (BIT_6)
#define AUD_NG_EN           (BIT_7)

//ANA_CFG15(DCR7,DCR8)
#define AUD_CHP_OSC_SHIFT (0)
#define AUD_CHP_OSC_MASK  (0x3<<AUD_CHP_OSC_SHIFT)

#define AUD_CHP_R_MUTE  (BIT_2)
#define AUD_CHP_L_MUTE  (BIT_3)

#define AUD_CHP_HPR_EN  (BIT_4)
#define AUD_CHP_HPL_EN  (BIT_5)

#define AUD_CHP_EN (BIT_6)
#define AUD_CHP_REF_EN (BIT_7)

#define AUD_CHP_MODE_NEG (BIT_14)
#define AUD_CHP_LPW   (BIT_15)

//ANA_CFG16(DCGR1,DCGR2)
//dcgr2
#define EAR_G                   (12)
#define EAR_G_MASK              (0xF << EAR_G)
//dcgr1
#define HPL_G                   (4)
#define HPL_G_MASK              (0xF << HPL_G)
#define HPR_G                   (0)
#define HPR_G_MASK              (0xF << HPR_G)

//ANA_CFG17(DCGR3)
//dcgr3
#define AOL_G                   (4)
#define AOL_G_MASK              (0xF << AOL_G)

#define AOR_G                   (0)
#define AOR_G_MASK              (0xF << AOR_G)

//ANA_CFG18(PNRCR1,PNRCR2)
//pnrcr2
#define HP_POP_SPEED_SHIFT      (8)
#define HP_POP_SPEED_MASK       (0xFF<<HP_POP_SPEED_SHIFT)
//pnrcr1
#define HP_POP_CTL        (6)
#define HP_POP_CTL_MASK         (0x3<<HP_POP_CTL)
#if 0
#define HP_POP_CTL_DIS          (0)
#define HP_POP_CTL_UP           (1)
#define HP_POP_CTL_DOWN         (2)
#define HP_POP_CTL_HOLD         (3)
#endif
#define HP_POP_STEP       (3)
#define HP_POP_STEP_MASK        (0x7<<HP_POP_STEP)

#if 0
#define HP_POP_STEP_012         (0)
#define HP_POP_STEP_025         (1)
#define HP_POP_STEP_05          (2)
#define HP_POP_STEP_1           (3)
#define HP_POP_STEP_2           (4)
#define HP_POP_STEP_4           (5)
#define HP_POP_STEP_8           (6)
#define HP_POP_STEP_16          (7)
#endif

#define HP_POP_CHAG_PD          (BIT_2)
#define HP_POP_SOFTCHAG_EN      (BIT_1)

//ANA_CFG19(PNRCR3)
//pnrcr3
#define HP_POP_CHG_V      (5)
#define HP_POP_CHG_V_MASK       (0x3<<HP_POP_CHG_V)

#if 0
//ANA_CFG20(HIBDR1,HIBDR2)
#define AUD_HEAD_SBUT_SHIFT   (1)
#define AUD_HEAD_SBUT_MASK   (0xF<<AUD_HEAD_SBUT_SHIFT)

#define AUD_HEAD_SDET_SHIFT  (5)
#define AUD_HEAD_SDET_MASK  (0x3<<AUD_HEAD_SDET_SHIFT)

#define AUD_HEAD_DETECT_PD   (BIT_7)

//toadd??
#define AUD_HEAD_INS_VREF_SHIFT  (8)
#define AUD_HEAD_INS_VREF_MASK  (0x3<<AUD_HEAD_INS_VREF_SHIFT)

#define AUD_HEAD_INS_HMBIAS_EN  (BIT_11)

#define AUD_HEAD_INS_PD   (BIT_12)

#define AUD_HEADMIC_ADC_SEL (BIT_13)

#define AUD_HEAD2ADC_EN (BIT_14)

#define AUD_HEAD_BUF_EN (BIT_15)

//ANA_CFG21(HIBDR3)  toadd ??
#define AUD_HEAD_APP_P_SHIFT  (3)
#define AUD_HEAD_APP_P_MASK   (0x3<<AUD_HEAD_APP_P_SHIFT)

#define AUD_HEAD_APP_G_SHIFT (5)
#define AUD_HEAD_APP_G_MASK  (0x3<<AUD_HEAD_APP_G_SHIFT)

#define AUD_HEAD_APP_EN  (BIT_7)
#endif

//ANA_STS0(IFR1,IFR2 ) RO
//ifr1
#define OTP_FLAG        (BIT_12)
//#define HEAD_BUTTON         (BIT_7)
//#define HEAD_INSERT         (BIT_6)
//#define HEAD_INSERT_2         (BIT_5)
#define HP_POP_FLG             (3)
#define HP_POP_FLG_MASK         (0x3<<HP_POP_FLG)
#define OVP_FLAG            (BIT_2)


//ANA_CFG22
#define AUD_DAC_NG_L     (BIT_0)

//ANA_CFG23
#define AUD_DAC_NG_R     (BIT_0)

//ANA_STS1(IFR3,IFR4 ) RO
//ifr3
#define OCP_AOR   (6)  
#define OCP_AOR_MASK    (0x3<<OCP_AOR)
#define OCP_AOL    (4)  
#define OCP_AOL_MASK     (0x3<<OCP_AOL)
#define OCP_EAR   (2)
#define OCP_EAR_MASK    (0x3<<OCP_EAR)
#define OCP_HP    (0)
#define OCP_HP_MASK     (0x3<<OCP_HP)

typedef struct
{
    uint32  audif_enb;                   //0000  ENB
    uint32  audif_ocp_otp_tmr_ctl;       //0004  OCPTMR_CTL,OTPTMR_CTL
    uint32  audif_ovptmr_shutdown_ctl;   //0008  SHUTDOWN_CTL,OVPTMR_CTL
    uint32  audif_int_clr;               //000C  AUDINT CLR , AUD SHUTDOWN CLR
    uint32  audif_int_en;                //0010  AUD_INT_EN
    uint32  audif_int_raw;               //0014  INT_RAW, Shutdown status
    uint32  audif_int_mask;              //0018  INT_MSK
    
    uint32  reserved0;          //001C
    uint32  reserved1;          //0020
    uint32  reserved2;          //0024
    uint32  reserved3;          //0028
    uint32  reserved4;          //002C
    uint32  reserved5;          //0030
    uint32  reserved6;          //0034
    uint32  reserved7;          //0038
    uint32  reserved8;          //003C
    
    uint32  pmur2_pmur1;        //0x40   ANA_CFG0; 
    uint32  pmur4_pmur3;        //0044   ANA_CFG1; 
    uint32  pmur6_pmur5;        //0048   ANA_CFG2; 
    uint32  pmur8_pmur7;        //004C   ANA_CFG3; 
    
    uint32  reserved9;          //0050
    uint32  reserved10;         //0054
    uint32  reserved11;         //0058
    uint32  reserved12;         //005C
    
    uint32  ccr;                //0060   ANA_CFG4;
    uint32  aacr2_aacr1;        //0064   ANA_CFG5;
    uint32  aaicr2_aaicr1;      //0068   ANA_CFG6;
    uint32  acgr1;              //006C   ANA_CFG7;
    uint32  acgr3_acgr2;        //0070   ANA_CFG8;
    uint32  dacgr_dacr;         //0074   ANA_CFG9;
    uint32  daocr2;             //0078   ANA_CFG10
    uint32  daocr3_daocr1;      //007C   ANA_CFG11
                                                               
    uint32  dcr2_dcr1;          //0080   ANA_CFG12
    uint32  dcr4_dcr3;          //0084   ANA_CFG13
    uint32  dcr6_dcr5;          //0088   ANA_CFG14
    uint32  dcr8_dcr7;          //008C   ANA_CFG15
    uint32  dcgr2_dcgr1;        //0090   ANA_CFG16
    uint32  dcgr3;              //0094   ANA_CFG17
    uint32  pnrcr2_pnrcr1;      //0098   ANA_CFG18
    uint32  pnrcr3;             //009C   ANA_CFG19
    uint32  hibdr2_hibdr1;      //00A0   ANA_CFG20
    uint32  hibdr3;             //00A4   ANA_CFG21
    
    uint32  reserved13;         //00A8
    uint32  reserved14;         //00AC
    uint32  reserved15;         //00B0
    uint32  reserved16;         //00B4
    uint32  reserved17;         //00B8
    uint32  reserved18;         //00BC
                                
    uint32  ifr2_ifr1;          //00C0   ANA_STS0
    uint32  ana_cfg22;          //00C4   Reserve
    uint32  ana_cfg23;          //00C8   Reserve 
    uint32  ifr4_ifr3;          //00CC   ANA_STS1
    uint32  reserved21;         //00D0   ANA_STS2
    uint32  reserved22;         //00D4   CHIP_RESERVE0
    uint32  reserved23;         //00D8   CHIP_RESERVE1
} SPRD_CODEC_AP_REG_CTL_T;

#define 	AUD_AP_PATH_CLK_6_P_5_MHZ		(0)
#define 	AUD_AP_PATH_CLK_3_P_25_MHZ	(1)
#define 	AUD_AP_PATH_CLK_13_MHZ		(2)
PUBLIC void __sprd_codec_ap_adc_en(uint32 en, uint32 chan);
#endif//__SPRD_CODEC_AP_PHY_H___
