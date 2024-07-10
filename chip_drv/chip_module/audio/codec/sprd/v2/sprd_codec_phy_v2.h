////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.rdamicro.com/svn/developing1/Sources/edrv/trunk/aud/ti/src/audp_ti.h $ //
//    $Author: jingyuli $                                                        //
//    $Date: 2016-06-07 16:37:31 +0800 (周二, 07 六月 2016) $                     //
//    $Revision: 32438 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file audp_wm8960.h                                                        //
/// That file is the private header for the ti implementation of the AUD  //
/// interface.                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _SPRD_CODEC_PHY_V2_
#define _SPRD_CODEC_PHY_V2_

#define SC2720_ADI_ADDR               0x50300000

//Reg define from 0 to end
#define AUD_ANALOGREG_OFFSET   0x700
#define AUD_ANALOGREG_DIRECTWRITE_OFFSET 0x8000  //direct write map address


    #define    SC2720_ANA_PMU0                  (0x00000000+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_PMU1                  (0x00000004+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_PMU2                  (0x00000008+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_PMU3                  (0x0000000C+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_PMU4                  (0x00000010+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_PMU5                  (0x00000014+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_CLK0                  (0x00000018+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_CDC0                  (0x0000001C+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_CDC1                  (0x00000020+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_CDC2                  (0x00000024+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_CDC3                  (0x00000028+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_CDC4                  (0x0000002C+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_HDT0                  (0x00000030+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_HDT1                  (0x00000034+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_HDT2                  (0x00000038+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_DCL0                  (0x0000003C+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_DCL1                  (0x00000040+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_DCL2                  (0x00000044+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_DCL4                  (0x0000004C+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_DCL5                  (0x00000050+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_DCL6                  (0x00000054+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_DCL7                  (0x00000058+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_STS0                  (0x0000005C+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_STS2                  (0x00000064+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_STS3                  (0x00000068+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_STS4                  (0x0000006C+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_STS5                  (0x00000070+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_STS6                  (0x00000074+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_STS7                  (0x00000078+AUD_ANALOGREG_OFFSET)
    #define    SC2720_ANA_CLK1                  (0x0000007C+AUD_ANALOGREG_OFFSET)

    #define    SC2720_GLOBAL_MODULE_EN0                    (0x00000008+0xC00)
    #define    SC2720_GLOBAL_POWER_PD_SW                (0x0000001C+0xC00)
    #define    SC2720_GLOBAL_XTL_WAIT_CTRL0             (0x000001E8+0xC00)
    #define    SC2720_AUD_CFGA_CLR                                  (0x00000000+0x800)
    #define    SC2720_AUD_CFGA_LP_MODULE_CTRL         (0x00000030+0x800)
    #define    SC2720_AUD_CLK_EN                                   (0x00000038+0x800)
    #define    SC2720_AUD_SOFT_RST                               (0x0000003C+0x800)
    #define    SC2720_GLOBAL_ARM_CLK_EN0                   (0x0000000C+0xC00)
    #define    SC2720_GLOBAL_AUDIO_CTRL0                   (0x00000208+0xC00)
    #define    SC2720_ADI_SCLK                                        (0x00000004+0x600)
    #define    SC2720_AUD_DASYNC                                   (0x00000030+0x600)
    #define    SC2720_AUD_DAD0                                        (0x00000034+0x600)
    #define    SC2720_AUD_ANA_ET2                                   (0x00000034+0x800)

//SC2720_GLOBAL_MODULE_EN0
#define SC2720_WRITE_REQ           (1)
#define SC2720_READ_REQ            (1)

#define SC2720_RG_GLOBAL_MODULE_CAL_EN       (1)
#define SC2720_RG_GLOBAL_MODULE_RTC_EN  (1)
#define SC2720_RG_GLOBAL_MODULE_WDG_EN (1)
#define SC2720_RG_GLOBAL_MODULE_EIC_EN       (1)
#define SC2720_RG_GLOBAL_MODULE_AUD_EN      (1)
#define SC2720_RG_GLOBAL_MODULE_ADC_EN   (1)
#define SC2720_RG_GLOBAL_MODULE_EFS_EN (1)
#define SC2720_RG_GLOBAL_MODULE_FGU_EN (1)
#define SC2720_RG_GLOBAL_MODULE_PINREG_EN (1)
#define SC2720_RG_GLOBAL_MODULE_BLTC_EN      (1)
#define SC2720_RG_GLOBAL_MODULE_TMR_EN (1)
#define SC2720_RG_GLOBAL_MODULE_TYPC_EN (1)




typedef unsigned long               UINT32;
typedef unsigned short              UINT16;
typedef signed long int             INT32;
typedef signed short int            INT16;
typedef unsigned char               UINT8;
typedef signed char                 INT8;
typedef signed short int            SINT16;
typedef signed long int             SINT32;
typedef unsigned int BOOL;
typedef int INT;

#define   PUBLIC
#define   PRIVATE                     static
#define   VOID                           void
#define	TRUE		1
#define	FALSE		0

typedef enum
{
    AUD_HEADSET_TYPE_PLUGOUT,
    AUD_HEADSET_TYPE_INSERT4P,
    AUD_HEADSET_TYPE_INSERT3P,
    AUD_HEADSET_TYPE_QTY = 0xFF000000
} AUD_HEADSET_TYPE_T;

typedef union
{
    struct {
     BOOL    CAL_EN       :1;
     BOOL    RTC_EN       :1;
     BOOL    WDG_EN      :1;
     BOOL    EIC_EN         :1;
     BOOL    AUD_EN       :1;
     BOOL    ADC_EN        :1;
     BOOL    EFS_EN        :1;
     BOOL    FGU_EN        :1;
     BOOL    PINREG_EN   :1;
     BOOL    BLTC_EN        :1;
     BOOL    RESERVD        :2;
     BOOL    TMR_EN          :1;
     BOOL    RESERVD_2   :1;
     BOOL    TYPC_EN       :1;
     BOOL    RESERVD_3   :1;
     UINT32 :16;
     }reg_t;
    UINT32 reg;
}SC2720_GLOBAL_MODULE_EN_T;

//SC2720_GLOBAL_POWER_PD_SW
#define SC2720_RG_GLOBAL_LDO_DCXO_PD       (1)
#define SC2720_RG_GLOBAL_LDO_EMM_PD  (1)
#define SC2720_RG_GLOBAL_LDO_CP_PD (1)
#define SC2720_RG_GLOBAL_DCDC_GEN_PD       (1)
#define SC2720_RG_GLOBAL_RESERVED2      (1)
#define SC2720_RG_GLOBAL_DCDC_CORE_PD   (1)
#define SC2720_RG_GLOBAL_OSC3M_EN (1)
#define SC2720_RG_GLOBAL_LDO_MEM_PD (1)
#define SC2720_RG_GLOBAL_LDO_AVDD18_PD (1)
#define SC2720_RG_GLOBAL_LDO_VDD28_PD      (1)
#define SC2720_RG_GLOBAL_BG_PD (1)

/*
typedef union
{
    struct {
     BOOL    BG_PD_AP       :1;
     BOOL    LDO_VDD28_PD_AP       :1;
     BOOL    LDO_AVDD18_PD      :1;
     BOOL    LDO_MEM_PD         :1;
     BOOL    OSC3M_EN       :1;
     BOOL    DCDC_CORE_PD       :1;
     BOOL    RESERVED2        :1;
     BOOL    DCDC_GEN_PD        :1;
     BOOL    LDO_CP_PD        :1;
     BOOL    LDO_EMM_PD   :1;
     BOOL    LDO_DCXO_PD_AP        :1;
     UINT32 :21;
     }reg_t;
    UINT32 reg;
}SC2720_GLOBAL_POWER_PD_SW_T;

*/

//SC2720_GLOBAL_XTL_WAIT_CTRL0
#define SC2720_RG_GLOBAL_EXT_XTL3_FOR26M_EN       (1)
#define SC2720_RG_GLOBAL_EXT_XTL2_FOR26M_EN  (1)
#define SC2720_RG_GLOBAL_EXT_XTL1_FOR26M_EN (1)
#define SC2720_RG_GLOBAL_EXT_XTL0_FOR26M_EN       (1)
#define SC2720_RG_GLOBAL_SLP_XTLBUF_PD_EN      (1)
#define SC2720_RG_GLOBAL_XTL_EN   (1)
#define SC2720_RG_GLOBAL_XTL_WAIT(n) (((n)&0xFF))

typedef union
{
    struct {
     UINT32    XTL_WAIT:8;  
     BOOL   XTL_EN_AP       :1;
     BOOL    SLP_XTLBUF_PD_EN_AP      :1;
     BOOL    EXT_XTL0_FOR26M_EN         :1;
     BOOL    EXT_XTL1_FOR26M_EN       :1;
     BOOL    EXT_XTL2_FOR26M_EN        :1;
     BOOL    EXT_XTL3_FOR26M_EN       :1;
     UINT32 :18;
     }reg_t;
    UINT32 reg;
}SC2720_GLOBAL_XTL_WAIT_CTRL0_T;

//SC2720_AUD_CFGA_LP_MODULE_CTRL
#define SC2720_RG_ADC_EN_R       (1)
#define SC2720_RG_DAC_EN_R  (1)
#define SC2720_RG_ADC_EN_L (1)
#define SC2720_RG_DAC_EN_L       (1)
#define SC2720_RG_AUDIO_LOOP_MAP_SEL      (1)
#define SC2720_RG_AUDIO_LOOP_EN   (1)

typedef union
{
    struct {    
     BOOL   AUDIO_LOOP_EN       :1;
     BOOL    AUDIO_LOOP_MAP_SEL      :1;
     BOOL    DAC_EN_L         :1;
     BOOL    ADC_EN_L       :1;
     BOOL    DAC_EN_R        :1;
     BOOL    ADC_EN_R       :1;
     UINT32 :26;
     }reg_t;
    UINT32 reg;
}SC2720_AUD_CFGA_LP_MODULE_CTRL_T;

//SC2720_AUD_CLK_EN
#define SC2720_RG_CLK_AUD_LOOP_INV_EN       (1)
#define SC2720_RG_CLK_AUD_6P5M_EN       (1)
#define SC2720_RG_CLK_AUD_LOOP_EN       (1)
#define SC2720_RG_CLK_AUD_HID_EN       (1)
#define SC2720_RG_CLK_AUD_1K_EN       (1)
#define SC2720_RG_CLK_AUD_32K_EN       (1)

typedef union
{
    struct {    
     BOOL   CLK_AUD_32K_EN       :1;
     BOOL    CLK_AUD_1K_EN       :1;
     BOOL    CLK_AUD_HID_EN         :1;
     BOOL    CLK_AUD_LOOP_EN       :1;
     BOOL    CLK_AUD_6P5M_EN        :1;
     BOOL    CLK_AUD_LOOP_INV_EN_AP       :1;
     UINT32 :26;
     }reg_t;
    UINT32 reg;
}SC2720_AUD_CLK_EN_T;

//SC2720_GLOBAL_ARM_CLK_EN0
#define SC2720_RG_CLK_AUXAD_EN       (1)
#define SC2720_RG_CLK_AUXADC_EN       (1)
#define SC2720_RG_CLK_CAL_SRC_SEL(n) (((n)&3))
#define SC2720_RG_CLK_CAL_EN       (1)
#define SC2720_RG_CLK_AUD_IF_6P5M_EN       (1)
#define SC2720_RG_CLK_AUD_IF_EN       (1)

typedef union
{
    struct {    
     BOOL   CLK_AUD_IF_EN       :1;
     BOOL    CLK_AUD_IF_6P5M_EN       :1;
     BOOL    CLK_CAL_EN         :1;
     UINT32    CLK_CAL_SRC_SEL       :2;
     BOOL    CLK_AUXADC_EN        :1;
     BOOL    CLK_AUXAD_EN       :1;
     UINT32 :25;
     }reg_t;
    UINT32 reg;
}SC2720_GLOBAL_ARM_CLK_EN0_T;

//SC2720_GLOBAL_AUDIO_CTRL0
#define SC2720_RG_CLK_AUD_IF_TX_INV_EN       (1)
#define SC2720_RG_CLK_AUD_IF_RX_INV_EN       (1)
#define SC2720_RG_CLK_AUD_IF_6P5M_TX_INV_EN       (1)
#define SC2720_RG_CLK_AUD_IF_6P5M_RX_INV_EN       (1)

typedef union
{
    struct {    
     BOOL   CLK_AUD_IF_6P5M_RX_INV_EN_AP       :1;
     BOOL    CLK_AUD_IF_6P5M_TX_INV_EN_AP       :1;
     BOOL    CLK_AUD_IF_RX_INV_EN_AP         :1;
     BOOL    CLK_AUD_IF_TX_INV_EN_AP       :1;
     UINT32 :28;
     }reg_t;
    UINT32 reg;
}SC2720_GLOBAL_AUDIO_CTRL0_T;

//SC2720_ADI_SCLK
#define SC2720_RG_ADI_SCLK_DRV(n) (((n)&3))
#define SC2720_RG_ADI_SCLK_FUNC_WPU       (1)
#define SC2720_RG_ADI_SCLK_FUNC_WPD       (1)
#define SC2720_RG_ADI_SCLK_SEL(n) (((n)&3))
#define SC2720_RG_ADI_SCLK_WPU       (1)
#define SC2720_RG_ADI_SCLK_WPD       (1)
#define SC2720_RG_ADI_SCLK_IE       (1)
#define SC2720_RG_ADI_SCLK_OE       (1)

typedef union
{
    struct {    
     BOOL   ADI_SCLK_OE       :1;
     BOOL    ADI_SCLK_IE       :1;
     BOOL    ADI_SCLK_WPD         :1;
     BOOL    ADI_SCLK_WPU         :1;
     UINT32    ADI_SCLK_SEL       :2;
     BOOL    ADI_SCLK_FUNC_WPD        :1;
     BOOL    ADI_SCLK_FUNC_WPU       :1;
     UINT32    ADI_SCLK_DRV       :2;
     UINT32 :22;
     }reg_t;
    UINT32 reg;
}SC2720_ADI_SCLK_T;

//SC2720_AUD_DASYNC
#define SC2720_RG_AUD_DASYNC_DRV(n) (((n)&3))
#define SC2720_RG_AUD_DASYNC_FUNC_WPU       (1)
#define SC2720_RG_AUD_DASYNC_FUNC_WPD       (1)
#define SC2720_RG_AUD_DASYNC_SEL(n) (((n)&3))
#define SC2720_RG_AUD_DASYNC_WPU       (1)
#define SC2720_RG_AUD_DASYNC_WPD       (1)
#define SC2720_RG_AUD_DASYNC_IE       (1)
#define SC2720_RG_AUD_DASYNC_OE       (1)

typedef union
{
    struct {    
     BOOL   AUD_DASYNC_OE       :1;
     BOOL    AUD_DASYNC_IE       :1;
     BOOL    AUD_DASYNC_WPD         :1;
     BOOL    AUD_DASYNC_WPU         :1;
     UINT32    AUD_DASYNC_SEL       :2;
     BOOL    AUD_DASYNC_FUNC_WPD        :1;
     BOOL    AUD_DASYNC_FUNC_WPU       :1;
     UINT32    AUD_DASYNC_DRV       :2;
     UINT32 :22;
     }reg_t;
    UINT32 reg;
}SC2720_AUD_DASYNC_T;

//SC2720_AUD_DAD0
#define SC2720_RG_AUD_DAD0_DRV(n) (((n)&3))
#define SC2720_RG_AUD_DAD0_FUNC_WPU       (1)
#define SC2720_RG_AUD_DAD0_FUNC_WPD       (1)
#define SC2720_RG_AUD_DAD0_SEL(n) (((n)&3))
#define SC2720_RG_AUD_DAD0_WPU       (1)
#define SC2720_RG_AUD_DAD0_WPD       (1)
#define SC2720_RG_AUD_DAD0_IE       (1)
#define SC2720_RG_AUD_DAD0_OE       (1)

typedef union
{
    struct {    
     BOOL   AUD_DAD0_OE       :1;
     BOOL    AUD_DAD0_IE       :1;
     BOOL    AUD_DAD0_WPD         :1;
     BOOL    AUD_DAD0_WPU         :1;
     UINT32    AUD_DAD0_SEL       :2;
     BOOL    AUD_DAD0_FUNC_WPD        :1;
     BOOL    AUD_DAD0_FUNC_WPU       :1;
     UINT32    AUD_DAD0_DRV       :2;
     UINT32 :22;
     }reg_t;
    UINT32 reg;
}SC2720_AUD_DAD0_T;

//SC2720_AUD_ANA_ET2
#define SC2720_DEM_BYPASS       (1)
#define SC2720_RG_AUD_DALR_MIX_SEL(n) (((n)&3))
#define SC2720_RG_AUD_DAS_MIX_SEL(n) (((n)&3))

typedef union
{
    struct {    
     UINT32    AUD_DAS_MIX_SEL       :2;
     UINT32    AUD_DALR_MIX_SEL       :2;
     BOOL    DEM_BYPASS       :1;
     UINT32 :27;
     }reg_t;
    UINT32 reg;
}SC2720_AUD_ANA_ET2_T;


//SC2720_ANA_PMU0
#define SC2720_WRITE_REQ           (1)
#define SC2720_READ_REQ            (1)
#define SC2720_RG_AUD_VB_EN        (1)
#define SC2720_RG_AUD_VB_NLEAK_PD  (1)
#define SC2720_RG_AUD_VB_HDMC_SP_PD (1)
#define SC2720_RG_AUD_RG_EN        (1)
#define SC2720_RG_AUD_BIAS_EN      (1)
#define SC2720_RG_AUD_MICBIAS_EN   (1)
#define SC2720_RG_AUD_HMIC_BIAS_EN (1)
#define SC2720_RG_AUD_HMIC_SLEEP_EN (1)
#define SC2720_RG_AUD_MIC_SLEEP_EN (1)
#define SC2720_RG_AUD_VBG_SEL      (1)
#define SC2720_RG_AUD_VBG_TEMP_BIASTUNE (1)
#define SC2720_RG_AUD_VBG_TEMP_TUNE(n) (((n)&3))
#define SC2720_RG_AUD_MICBIAS_PLGB (1)
#define SC2720_RG_AUD_HMICBIAS_VREF_SEL (1)
#define SC2720_RG_AUD_HMIC_COMP_MODE1_EN (1)

typedef union
{
    struct {
     BOOL    HMIC_COMP_MODE1_EN  :1;
     BOOL    HMICBIAS_VREF_SEL  :1;
     BOOL    MICBIAS_PLGB   :1;
     UINT32    VBG_TEMP_TUNE   :2;
     BOOL    VBG_TEMP_BIASTUNE   :1;
     BOOL    VBG_SEL        :1;
     BOOL    MIC_SLEEP_EN   :1;
     BOOL    HMIC_SLEEP_EN   :1;
     BOOL    HMIC_BIAS_EN   :1;
     BOOL    MICBIAS_EN     :1;
     BOOL    BIAS_EN        :1;
     BOOL    BG_EN          :1;
     BOOL    VB_HDMC_SP_PD   :1;
     BOOL    VB_NLEAK_PD    :1;
     BOOL    VB_EN          :1;
     UINT32 :16;
     }reg_t;
    UINT32 reg;
}SC2720_ANA_PMU0_T;

//SC2720_ANA_PMU1
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_VB_CAL(n)    (((n)&31))
#define SC2720_RG_AUD_VB_V(n)      (((n)&31))
#define SC2720_RG_AUD_HMIC_BIAS_V(n) (((n)&7))
#define SC2720_RG_AUD_MICBIAS_V(n) (((n)&7))

typedef union
{
    struct {
     UINT32    VB_CAL:5;   
     UINT32    VB_V:5;   
     UINT32    HMIC_BIAS_V:3;   
     UINT32    MICBIAS_V :3;   
     UINT32 :16;
     }reg_t;
    UINT32 reg;
}SC2720_ANA_PMU1_T;

//SC2720_ANA_PMU2
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_HP_IB(n)     (((n)&3))
#define SC2720_RG_AUD_HP_IBCUR3(n) (((n)&7))
#define SC2720_RG_AUD_PA_AB_I(n)   (((n)&3))
#define SC2720_RG_AUD_ADPGA_IBIAS_SEL(n) (((n)&15))
#define SC2720_RG_AUD_DA_IG(n)     (((n)&3))
#define SC2720_RG_AUD_DRV_PM_SEL(n) (((n)&3))
#define SC2720_RESERVED            (1)
typedef union
{
    struct {
     BOOL    RESERVED :1;   
     UINT32    DRV_PM_SEL :2;   
     UINT32    DA_IG :2;   
     UINT32    ADPGA_IBIAS_SEL :4;   
     UINT32    PA_AB_I :2;   
     UINT32    HP_IBCUR3 :3;   
     UINT32    HP_IB :2;   
     UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_PMU2_T;

//SC2720_ANA_PMU3
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_PA_OTP_PD    (1)
#define SC2720_RG_AUD_OTP_T(n)     (((n)&7))
#define SC2720_RG_AUD_PA_OVP_PD    (1)
#define SC2720_RG_AUD_PA_OVP_THD   (1)
#define SC2720_RG_AUD_PA_OVP_V(n)  (((n)&7))
#define SC2720_RG_AUD_PA_OCP_PD    (1)
#define SC2720_RG_AUD_PA_OCP_S     (1)
#define SC2720_RG_AUD_DRV_OCP_PD   (1)
#define SC2720_RG_AUD_DRV_OCP_MODE(n) (((n)&3))
#define SC2720_RG_AUD_PA_VCM_V(n)  (((n)&3))
typedef union
{
    struct {
     UINT32    PA_VCM_V :2;   
     UINT32    DRV_OCP_MODE :2;   
     BOOL    DRV_OCP_PD :1;  
     BOOL    PA_OCP_S :1;  
     BOOL    PA_OCP_PD :1;  
     UINT32    PA_OVP_V :3;   
     BOOL    PA_OVP_THD :1;  
     BOOL    PA_OVP_PD :1;  
     UINT32    OTP_T :3;   
     BOOL    PA_OTP_PD :1;  
     UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_PMU3_T;

//SC2720_ANA_PMU4
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_PA_KSEL(n)   (((n)&3))
#define SC2720_RG_AUD_PA_DEGSEL(n) (((n)&3))
#define SC2720_RG_AUD_PA_EMI_L(n)  (((n)&7))
#define SC2720_RG_AUD_PA_SS_EN     (1)
#define SC2720_RG_AUD_PA_SS_RST    (1)
#define SC2720_RG_AUD_PA_SS_F(n)   (((n)&3))
#define SC2720_RG_AUD_PA_SS_32K_EN (1)
#define SC2720_RG_AUD_PA_SS_T(n)   (((n)&7))
typedef union
{
    struct {
     UINT32    PA_SS_T :3;   
     BOOL    PA_SS_32K_EN :1;  
     UINT32    PA_SS_F :2;
     BOOL    PA_SS_RST :1;  
     BOOL    PA_SS_EN :1;  
     UINT32    PA_EMI_L :3;   
     UINT32    PA_DEGSEL :2;
     UINT32    PA_KSEL :2;
     UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_PMU4_T;

//SC2720_ANA_PMU5
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_PA_D_EN      (1)
#define SC2720_RG_AUD_PA_DFLCK_EN  (1)
#define SC2720_RG_AUD_PA_DFLCK_RSL (1)
#define SC2720_RG_AUD_PA_DTRI_FC(n) (((n)&7))
#define SC2720_RG_AUD_PA_DTRI_FF(n) (((n)&0x3F))
#define SC2720_RG_AUD_PA_STOP_EN   (1)
#define SC2720_RG_AUD_PA_SH_DET_EN (1)
#define SC2720_RG_AUD_PA_SL_DET_EN (1)
typedef union
{
    struct {
     BOOL    PA_SL_DET_EN :1;
     BOOL    PA_SH_DET_EN :1;
     BOOL    PA_STOP_EN :1;
     UINT32    PA_DTRI_FF :6;   
     UINT32   PA_DTRI_FC :3;   
     BOOL    PA_DFLCK_RSL :1;
     BOOL    PA_DFLCK_EN :1;
     BOOL    PA_D_EN :1;
     UINT32 :17;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_PMU5_T;

//SC2720_ANA_CLK0
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_DIG_CLK_6P5M_EN (1)
#define SC2720_RG_AUD_DIG_CLK_LOOP_EN (1)
#define SC2720_RG_AUD_ANA_CLK_EN   (1)
#define SC2720_RG_AUD_AD_CLK_EN    (1)
#define SC2720_RG_AUD_AD_CLK_RST   (1)
#define SC2720_RG_AUD_DA_CLK_EN    (1)
#define SC2720_RG_DRV_CLK_EN       (1)
#define SC2720_RG_AUD_DCDCGEN_CLK_EN (1)
#define SC2720_RG_AUD_DCDCMEM_CLK_EN (1)
#define SC2720_RG_AUD_DCDCCORE_CLK_EN (1)
#define SC2720_RG_AUD_DCDCCHG_CLK_EN (1)
#define SC2720_RG_AUD_AD_CLK_F(n)  (((n)&3))
#define SC2720_RG_AUD_DA_CLK_F(n)  (((n)&3))
typedef union
{
    struct {
     UINT32    DA_CLK_F :2;
     UINT32    AD_CLK_F :2;
     BOOL    DCDCCHG_CLK_EN :1;
     BOOL    DCDCCORE_CLK_EN :1;
     BOOL    DCDCMEM_CLK_EN :1;
     BOOL    DCDCGEN_CLK_EN :1;
     BOOL    DRV_CLK_EN :1;
     BOOL    DA_CLK_EN :1;
     BOOL    AD_CLK_RST :1;
     BOOL    AD_CLK_EN :1;
     BOOL    ANA_CLK_EN :1;
     BOOL    DIG_CLK_LOOP_EN  :1;
     BOOL    DIG_CLK_6P5M_EN :1;
     UINT32 :17;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_CLK0_T;


//SC2720_ANA_CDC0
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_ADPGA_IBIAS_EN (1)
#define SC2720_RG_AUD_ADPGA_IBUF_EN (1)
#define SC2720_RG_AUD_ADPGAL_EN    (1)
#define SC2720_RG_AUD_ADPGAR_EN    (1)
#define SC2720_RG_AUD_ADPGAL_BYP(n) (((n)&3))
#define SC2720_RG_AUD_ADPGAR_BYP(n) (((n)&3))
#define SC2720_RG_AUD_ADL_EN       (1)
#define SC2720_RG_AUD_ADL_RST      (1)
#define SC2720_RG_AUD_ADR_EN       (1)
#define SC2720_RG_AUD_ADR_RST      (1)
#define SC2720_RG_AUD_VREF_SFCUR   (1)
#define SC2720_RG_AUD_SHMIC_DPOP   (1)
#define SC2720_RG_AUD_SHMIC_DPOPVCM_EN (1)
#define SC2720_RG_AUD_ADC_BULKSW   (1)
typedef union
{
    struct {
     BOOL    ADC_BULKSW :1;
     BOOL    SHMIC_DPOPVCM_EN :1;
     BOOL    SHMIC_DPOP :1;
     BOOL    VREF_SFCUR :1;
     BOOL    ADR_RST :1;
     BOOL    ADR_EN :1;
     BOOL    ADL_RST :1;
     BOOL    ADL_EN :1;
     UINT32    ADPGAR_BYP :2;
     UINT32    ADPGAL_BYP :2;
     BOOL    ADPGAR_EN :1;
     BOOL    ADPGAL_EN :1;
     BOOL    ADPGA_IBUF_EN :1;
     BOOL    ADPGA_IBIAS_EN :1;
     UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_CDC0_T;

//SC2720_ANA_CDC1
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_ADPGA_RESERVED(n) (((n)&3))
#define SC2720_RG_AUD_ADVCMI_INT_SEL(n) (((n)&3))
#define SC2720_RG_AUD_ADPGAL_G(n)  (((n)&7))
#define SC2720_RG_AUD_ADPGAR_G(n)  (((n)&7))
#define SC2720_RG_AUD_DALR_OS_D(n) (((n)&7))
#define SC2720_RG_AUD_DAS_OS_D(n)  (((n)&7))
typedef union
{
    struct {
     UINT32    DAS_OS_D :3;   
     UINT32    DALR_OS_D :3;   
     UINT32    ADPGAR_G :3;   
     UINT32    ADPGAL_G :3;   
     UINT32    ADVCMI_INT_SEL :2;
     UINT32    ADPGA_RESERVED :2;
     UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_CDC1_T;

//SC2720_ANA_CDC2
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_DAS_EN       (1)
#define SC2720_RG_AUD_DAL_EN       (1)
#define SC2720_RG_AUD_DAR_EN       (1)
#define SC2720_HPL_FLOOPEN         (1)
#define SC2720_HPL_FLOOP_END       (1)
#define SC2720_HPR_FLOOPEN         (1)
#define SC2720_HPR_FLOOP_END       (1)
#define SC2720_RCV_FLOOPEN         (1)
#define SC2720_RCV_FLOOP_END       (1)
#define SC2720_RG_AUD_HPL_EN       (1)
#define SC2720_RG_AUD_HPR_EN       (1)
#define SC2720_RG_AUD_HPBUF_EN     (1)
#define SC2720_RG_AUD_RCV_EN       (1)
#define SC2720_RG_AUD_PA_EN        (1)
typedef union
{
    struct {
     BOOL    PA_EN :1;
     BOOL    RCV_EN :1;
     BOOL    HPBUF_EN :1;
     BOOL    HPR_EN :1;
     BOOL    HPL_EN :1;
     BOOL    RCV_FLOOP_END :1;
     BOOL    RCV_FLOOPEN :1;
     BOOL    HPR_FLOOP_END :1;
     BOOL    HPR_FLOOPEN :1;
     BOOL    HPL_FLOOP_END :1;
     BOOL    HPL_FLOOPEN :1;
     BOOL    DAR_EN :1;
     BOOL    DAL_EN :1;
     BOOL    DAS_EN :1;
     UINT32 :18;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_CDC2_T;

//SC2720_ANA_CDC3
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_DALR_OS_EN   (1)
#define SC2720_RG_AUD_DAS_OS_EN    (1)
#define SC2720_RG_AUD_PA_NG_EN     (1)
#define SC2720_RG_AUD_SDALHPL      (1)
#define SC2720_RG_AUD_SDARHPR      (1)
#define SC2720_RG_AUD_SDAL_RCV     (1)
#define SC2720_RG_AUD_SDA_PA       (1)
#define SC2720_RG_AUD_SHMICPA_DEBUG (1)
#define SC2720_RG_AUD_SMICDRV_DEBUG (1)
#define SC2720_RG_AUD_SMIC_1PGAL   (1)
#define SC2720_RG_AUD_SMIC_2PGAR   (1)
#define SC2720_RG_AUD_SHMICPGAL    (1)
#define SC2720_RG_AUD_SHMICPGAR    (1)
typedef union
{
    struct {
     BOOL    SHMICPGAR :1;
     BOOL    SHMICPGAL :1;
     BOOL    SMIC_2PGAR :1;
     BOOL    SMIC_1PGAL :1;
     BOOL    SMICDRV_DEBUG :1;
     BOOL    SHMICPA_DEBUG :1;
     BOOL    SDA_PA :1;
     BOOL    SDAL_RCV :1;
     BOOL    SDARHPR :1;
     BOOL    SDALHPL :1;
     BOOL    PA_NG_EN :1;
     BOOL    DAS_OS_EN :1;
     BOOL    DALR_OS_EN :1;
     UINT32 :19;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_CDC3_T;

//SC2720_ANA_CDC4
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_PA_G(n)      (((n)&15))
#define SC2720_RG_AUD_RCV_G(n)     (((n)&15))
#define SC2720_RG_AUD_HPL_G(n)     (((n)&15))
#define SC2720_RG_AUD_HPR_G(n)     (((n)&15))
typedef union
{
    struct {
     UINT32    HPR_G :4;
     UINT32    HPL_G :4;
     UINT32    RCV_G :4;
     UINT32    PA_G :4;
     UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_CDC4_T;

//SC2720_ANA_HDT0
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_HEDET_MUX2ADC_SEL_PD (1)
#define SC2720_RG_AUD_HEDET_BUF_EN (1)
#define SC2720_RG_AUD_HEDET_BDET_EN (1)
#define SC2720_RG_AUD_HEDET_V21_EN (1)
#define SC2720_RG_AUD_HEDET_VREF_EN (1)
#define SC2720_RG_AUD_HEDET_MICDEF_EN (1)
#define SC2720_RG_AUD_HEDET_V2AD_SCALE (1)
#define SC2720_RG_AUD_HEDET_LDET_L_FILTER_EN (1)
#define SC2720_RG_AUD_HEDET_BUF_CHOP (1)
#define SC2720_RG_AUD_HEDET_MUX2ADC_SEL(n) (((n)&7))
#define SC2720_RG_AUD_HEDET_V21_SEL(n) (((n)&15))
typedef union
{
    struct {
    UINT32    HEDET_V21_SEL :4;
    UINT32    HEDET_MUX2ADC_SEL :3;   
    BOOL    HEDET_BUF_CHOP :1;
    BOOL    HEDET_LDET_L_FILTER_EN :1;
    BOOL    HEDET_V2AD_SCALE :1;
    BOOL    HEDET_MICDET_EN :1;
    BOOL    HEDET_VREF_EN :1;
    BOOL    HEDET_V21_EN :1;
    BOOL    HEDET_BDET_EN :1;
    BOOL    HEDET_BUF_EN :1;
    BOOL    HEDET_MUX2ADC_SEL_PD :1;
       UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_HDT0_T;

//SC2720_ANA_HDT1
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_HEDET_MICDET_REF_SEL(n) (((n)&7))
#define SC2720_RG_AUD_HEDET_MICDET_HYS_SEL(n) (((n)&3))
#define SC2720_RG_AUD_HEDET_LDET_REFL_SEL(n) (((n)&7))
#define SC2720_RG_AUD_HEDET_LDET_REFH_SEL(n) (((n)&3))
#define SC2720_RG_AUD_HEDET_LDET_PU_PD(n) (((n)&3))
#define SC2720_RG_AUD_HEDET_LDET_L_HYS_SEL(n) (((n)&3))
#define SC2720_RG_AUD_HEDET_LDET_H_HYS_SEL(n) (((n)&3))
typedef union
{
    struct {
    UINT32    HEDET_LDET_H_HYS_SEL :2;
    UINT32    HEDET_LDET_L_HYS_SEL :2;
    UINT32    HEDET_LDET_PU_PD :2;
    UINT32    HEDET_LDET_REFH_SEL :2;
    UINT32    HEDET_LDET_REFL_SEL :3;  
    UINT32    HEDET_MICDET_HYS_SEL :2;
    UINT32    HEDET_MICDET_REF_SEL :3;  
       UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_HDT1_T;

//SC2720_ANA_HDT2
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_CHG_PROC_STS_BYPAS (1)
#define SC2720_RG_AUD_HEDET_JACK_TYPE(n) (((n)&3))
#define SC2720_RG_AUD_HEDET_BDET_REF_SEL(n) (((n)&15))
#define SC2720_RG_AUD_HEDET_BDET_HYS_SEL(n) (((n)&3))
#define SC2720_RG_AUD_PLGPD_EN     (1)
#define SC2720_RG_HP_DRIVER_EN     (1)
#define SC2720_RG_AUD_HPL_EN_D2HDT_EN (1)
#define SC2720_RG_AUD_HPL_EN_D2HDT_T(n) (((n)&3))
typedef union
{
    struct {
    UINT32    HPL_EN_D2HDT_T :2;
    BOOL    HPL_EN_D2HDT_EN :1;
    BOOL    HP_DRIVER_EN :1;
    BOOL    PLGPD_EN :1;
    UINT32    HEDET_BDET_HYS_SEL :2;
    UINT32    HEDET_BDET_REF_SEL :4;
    UINT32    HEDET_JACK_TYPE :2;
    BOOL    CHG_PROC_STS_BYPAS :1;
       UINT32 :18;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_HDT2_T;

//SC2720_ANA_DCL0
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_PA_RESERVED(n) (((n)&0x3F))
#define SC2720_RG_AUD_DCL_EN       (1)
#define SC2720_RG_HP_DCL_RST       (1)
#define SC2720_RG_AUD_DRV_SOFT_T(n) (((n)&7))
#define SC2720_RG_AUD_DRV_SOFT_EN  (1)
#define SC2720_RG_AUD_DPOP_AUTO_RST (1)
typedef union
{
    struct {
    BOOL    DPOP_AUTO_RST :1;
    BOOL    DRV_SOFT_EN :1;
    UINT32    DRV_SOFT_T :3;
    BOOL    HP_DCL_RST :1;
    BOOL    DCL_EN :1;
    UINT32    PA_RESERVED :6;
       UINT32 :19;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_DCL0_T;

//SC2720_ANA_DCL1
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_PACAL_EN     (1)
#define SC2720_RG_AUD_PACAL_DIV(n) (((n)&3))
#define SC2720_RG_AUD_PA_OVP_ABMOD_PD (1)
#define SC2720_RG_AUD_PA_OVP_ABMOD_T(n) (((n)&7))
#define SC2720_RG_AUD_PA_OVP_DEG_EN (1)
#define SC2720_RG_AUD_PA_OVP_DEG_T(n) (((n)&7))
typedef union
{
    struct {
    UINT32    PA_OVP_DEG_T :3;
    BOOL    PA_OVP_DEG_EN :1;
    UINT32    PA_OVP_ABMOD_T :3;
    BOOL    PA_OVP_ABMOD_PD :1;
    UINT32    PACAL_DIV :2;
    BOOL    PACAL_EN :1;
       UINT32 :21;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_DCL1_T;

//SC2720_ANA_DCL2
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_PA_OTP_DEG_EN (1)
#define SC2720_RG_AUD_PA_OTP_DEG_T(n) (((n)&7))
#define SC2720_RG_AUD_PA_OTP_MUTE_EN (1)
#define SC2720_RG_AUD_PA_OCP_DEG_EN (1)
#define SC2720_RG_AUD_PA_OCP_DEG_T(n) (((n)&7))
#define SC2720_RG_AUD_PA_OCP_MUTE_EN (1)
#define SC2720_RG_AUD_PA_OCP_MUTE_T(n) (((n)&7))
typedef union
{
    struct {
    UINT32    PA_OCP_MUTE_T :3;
    BOOL    PA_OCP_MUTE_EN :1;
    UINT32    PA_OCP_DEG_T :3;
    BOOL    PA_OCP_DEG_EN :1;
    BOOL    PA_OTP_MUTE_EN :1;
    UINT32    PA_OTP_DEG_T :3;
    BOOL    PA_OTP_DEG_EN :1;
       UINT32 :19;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_DCL2_T;

//SC2720_ANA_DCL4
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_HPL_DEPOP_CHG_CURSEL(n) (((n)&0xFF))
#define SC2720_RG_HPR_DEPOP_CHG_CURSEL(n) (((n)&0xFF))
typedef union
{
    struct {
    UINT32    HPR_DEPOP_CHG_CURSEL :8;
    UINT32    HPL_DEPOP_CHG_CURSEL :8;
       UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_DCL4_T;

//SC2720_ANA_DCL5
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_HPL_RDAC_START (1)
#define SC2720_RG_AUD_HPR_RDAC_START (1)
#define SC2720_RG_AUD_HP_DPOP_FDIN_EN (1)
#define SC2720_RG_AUD_HP_DPOP_FDOUT_EN (1)
#define SC2720_RG_AUD_HP_DPOP_GAIN_N1(n) (((n)&7))
#define SC2720_RG_AUD_HP_DPOP_GAIN_N2(n) (((n)&7))
#define SC2720_RG_AUD_HP_DPOP_GAIN_T(n) (((n)&7))
#define SC2720_RG_AUD_HPL_RDAC_STS (1)
#define SC2720_RG_AUD_HPR_RDAC_STS (1)
//#define SC2720_RESERVED          (1<<0)

typedef union
{
    struct {
       BOOL    RESERVED:1;
       BOOL    HPR_RDAC_STS:1;
    BOOL    HPL_RDAC_STS :1;
    UINT32    HP_DPOP_GAIN_T :3;
    UINT32    HP_DPOP_GAIN_N2 :3;
    UINT32    HP_DPOP_GAIN_N1 :3;
    BOOL    HP_DPOP_FDOUT_EN :1;
    BOOL    HP_DPOP_FDIN_EN :1;
    BOOL    HPR_RDAC_START :1;
    BOOL    HPL_RDAC_START :1;
       UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_DCL5_T;

//SC2720_ANA_DCL6
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_CALDC_WAIT_T(n)  (((n)&7))
#define SC2720_RG_AUD_HPL_DPOP_CLKN1(n) (((n)&3))
#define SC2720_RG_AUD_HPL_DPOP_N1(n) (((n)&3))
#define SC2720_RG_AUD_HPL_DPOP_VAL1(n) (((n)&7))
#define SC2720_RG_AUD_HPL_DPOP_CLKN2(n) (((n)&3))
#define SC2720_RG_AUD_HPL_DPOP_N2(n) (((n)&3))
//#define SC2720_RESERVED          (1<<0)
typedef union
{
    struct {
    BOOL    RESERVED:1;
     UINT32    HPL_DPOP_N2:2;
     UINT32    HPL_DPOP_CLKN2:2;
     UINT32    HPL_DPOP_VAL1:3;
     UINT32    HPL_DPOP_N1:2;
     UINT32    HPL_DPOP_CLKN1:2;
     UINT32    RG_CALDC_WAIT_T:3;
     UINT32 :17;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_DCL6_T;

//SC2720_ANA_DCL7
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_DEPOPL_PCUR_OPT(n) (((n)&3))
#define SC2720_RG_DEPOPR_PCUR_OPT(n) (((n)&3))
#define SC2720_RG_AUD_HPR_DPOP_CLKN1(n) (((n)&3))
#define SC2720_RG_AUD_HPR_DPOP_N1(n) (((n)&3))
#define SC2720_RG_AUD_HPR_DPOP_VAL1(n) (((n)&7))
#define SC2720_RG_AUD_HPR_DPOP_CLKN2(n) (((n)&3))
#define SC2720_RG_AUD_HPR_DPOP_N2(n) (((n)&3))
typedef union
{
    struct {
     UINT32    HPR_DPOP_N2:2;
     UINT32    HPR_DPOP_CLKN2:2;
     UINT32    HPR_DPOP_VAL1:3;
     UINT32    HPR_DPOP_N1:2;
     UINT32    HPR_DPOP_CLKN1:2;
    UINT32    RG_DEPOPR_PCUR_OPT:2;
    UINT32    RG_DEPOPL_PCUR_OPT:2;
        UINT32 :17;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_DCL7_T;

//SC2720_ANA_STS0
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_HP_DEPOP_WAIT_T1(n) (((n)&7))
#define SC2720_RG_HP_DEPOP_WAIT_T2(n) (((n)&7))
#define SC2720_RG_HP_DEPOP_WAIT_T3(n) (((n)&3))
#define SC2720_RG_HP_DEPOP_WAIT_T4(n) (((n)&3))
#define SC2720_RG_DC_CALI_IDACVAL(n) (((n)&7))
#define SC2720_DC_CALI_IDAC_CURSEL(n) (((n)&3))
#define SC2720_RG_DCCALI_RDACI_ADJ (1)
typedef union
{
    struct {
    BOOL    RG_DCCALI_RDACI_ADJ :1;
    UINT32    DC_CALI_IDAC_CURSEL:2;
    UINT32    RG_DC_CALI_IDACVAL:3;
    UINT32    RG_HP_DEPOP_WAIT_T4:2;
    UINT32    RG_HP_DEPOP_WAIT_T3:2;
    UINT32    RG_HP_DEPOP_WAIT_T2:3;
    UINT32    RG_HP_DEPOP_WAIT_T1:3;
        UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_STS0_T;

//SC2720_ANA_STS2
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_CALDC_START      (1)
#define SC2720_RG_CALDC_EN         (1)
#define SC2720_RG_CALDC_ENO        (1)
#define SC2720_RG_AUD_DCCAL_STS    (1)
#define SC2720_RG_DCCALI_STS_BYPASS (1)
#define SC2720_RG_AUD_HP_DPOP_DVLD (1)
#define SC2720_RG_DEPOP_CHG_START  (1)
#define SC2720_RG_DEPOP_CHG_EN     (1)
#define SC2720_RG_AUD_PLUGIN       (1)
#define SC2720_RG_DEPOP_EN         (1)
#define SC2720_RG_DEPOP_CHG_STS    (1)
#define SC2720_RG_AUD_RCV_DPOP_DVLD (1)
#define SC2720_RG_HPL_PU_ENB       (1)
#define SC2720_RG_HPR_PU_ENB       (1)
#define SC2720_RG_INSBUF_EN        (1)
//#define SC2720_RESERVED          (1<<0)
typedef union
{
    struct {
    BOOL    RESERVED :1;
    BOOL    RG_INSBUF_EN  :1;
    BOOL    RG_HPR_PU_ENB  :1;
    BOOL    RG_HPL_PU_ENB   :1;
     BOOL    RCV_DPOP_DVLD  :1;
    BOOL    RG_DEPOP_CHG_STS   :1;
    BOOL    RG_DEPOP_EN     :1;
     BOOL    PLUGIN    :1;
    BOOL    RG_DEPOP_CHG_EN     :1;
    BOOL    RG_DEPOP_CHG_START :1;
     BOOL    HP_DPOP_DVLD  :1;
    BOOL    RG_DCCALI_STS_BYPASS  :1;
     BOOL    DCCAL_STS     :1;
    BOOL    RG_CALDC_ENO       :1;
    BOOL    RG_CALDC_EN         :1;
    BOOL    RG_CALDC_START     :1;
        UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_STS2_T;

//SC2720_ANA_STS3
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_CLK3_RESERVE(n) (((n)&3))
#define SC2720_RG_DEPOP_BIAS_SEL(n) (((n)&3))
#define SC2720_RG_DEPOP_OPA_SEL(n) (((n)&3))
#define SC2720_RG_AUD_HWSW_SEL(n)  (((n)&0x3F))
typedef union
{
    struct {
     UINT32    HWSW_SEL:6;
    UINT32    RG_DEPOP_OPA_SEL:2;
    UINT32    RG_DEPOP_BIAS_SEL:2;
     UINT32    CLK3_RESERVE:2;
        UINT32 :20;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_STS3_T;

//SC2720_ANA_STS4
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_HPL_DCCAL_RDACL(n)  (((n)&0xFF))
#define SC2720_HPR_DCCAL_RDACL(n)  (((n)&15))
#define SC2720_HPL_DCCAL_IDACL_SEL(n) (((n)&3))
#define SC2720_HPR_DCCAL_IDACL_SEL(n) (((n)&3))
typedef union
{
    struct {
    UINT32    HPR_DCCAL_IDACL_SEL:2;
    UINT32    HPL_DCCAL_IDACL_SEL:2;
    UINT32    HPR_DCCAL_RDACL: 4;
    UINT32    HPL_DCCAL_RDACL:8;
        UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_STS4_T;

//SC2720_ANA_STS5
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_HPL_DCCAL_IDACL(n)  (((n)&31))
#define SC2720_HPR_DCCAL_IDACL(n)  (((n)&31))
#define SC2720_RG_HPL_COMP_INS(n)  (((n)&3))
#define SC2720_RG_HPR_COMP_INS(n)  (((n)&3))
//#define SC2720_RESERVED(n)       (((n)&3)<<0)
typedef union
{
    struct {
    UINT32    RESERVED:2;
    UINT32    RG_HPR_COMP_INS:2;
    UINT32    RG_HPL_COMP_INS:2;
    UINT32    HPR_DCCAL_IDACL:5;
    UINT32    HPL_DCCAL_IDACL:5;
   UINT32 :16;

    }reg_t;
    UINT32 reg;
}SC2720_ANA_STS5_T;

//SC2720_ANA_STS6
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_PCCAL_DO(n)  (((n)&0x1FFF))
#define SC2720_RG_AUD_PCCAL_DVLD   (1)
//#define SC2720_RESERVED(n)       (((n)&3)<<0)
typedef union
{
    struct {
     UINT32    RESERVED:2;
     BOOL    PCCAL_DVLD:1;
     UINT32    PCCAL_DO :13;
        UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_STS6_T;

//SC2720_ANA_STS7
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_HEAD_INSERT_ALL (1)
#define SC2720_RG_AUD_HEAD_INSERT3 (1)
#define SC2720_RG_AUD_HEAD_INSERT2 (1)
#define SC2720_RG_AUD_HEAD_INSERT  (1)
#define SC2720_RG_AUD_HEAD_BUTTON  (1)
#define SC2720_RG_AUD_PA_SH_FLAG   (1)
#define SC2720_RG_AUD_PA_SL_FLAG   (1)
#define SC2720_RG_AUD_PA_OVP_FLAG  (1)
#define SC2720_RG_AUD_PA_OTP_FLAG  (1)
#define SC2720_RG_AUD_DRV_OCP_FLAG(n) (((n)&15))
typedef union
{
    struct {
     UINT32    DRV_OCP_FLAG:4;
     BOOL    PA_OTP_FLAG :1;
     BOOL    PA_OVP_FLAG  :1;
     BOOL    PA_SL_FLAG   :1;
     BOOL    PA_SH_FLAG  :1;
     BOOL    HEAD_BUTTON  :1;
     BOOL    HEAD_INSERT :1;
     BOOL    HEAD_INSERT2 :1;
     BOOL    HEAD_INSERT3 :1;
     BOOL    HEAD_INSERT_ALL:1;
        UINT32 :19;

    }reg_t;
    UINT32 reg;
}SC2720_ANA_STS7_T;

//SC2720_ANA_CLK1
//#define SC2720_WRITE_REQ         (1<<31)
//#define SC2720_READ_REQ          (1<<30)
#define SC2720_RG_AUD_DCDCGEN_CLK_F(n) (((n)&3))
#define SC2720_RG_AUD_DCDCCORE_CLK_F(n) (((n)&3))
#define SC2720_RG_AUD_DCDCCHG_CLK_F(n) (((n)&3))
#define SC2720_RG_AUD_PA_CLK_F(n)  (((n)&3))
#define SC2720_RG_AUD_PN_SEL(n)    (((n)&0xFF))

typedef union
{
    struct {
     UINT32    PN_SEL :8;
     UINT32    PA_CLK_F :2;
     UINT32    DCDCCHG_CLK_F :2;
     UINT32    DCDCCORE_CLK_F :2;
     UINT32    DCDCGEN_CLK_F :2;
     UINT32 :16;
    }reg_t;
    UINT32 reg;
}SC2720_ANA_CLK1_T;

typedef struct {
	UINT32 A;
	UINT32 B;
	UINT32 E1;
	UINT32 E2;
	UINT32 cal_type;
} SC2720_HEADSET_AUXADC_CAL_T;

enum {
	JACK_TYPE_NO = 0, /* Normal-open jack */
	JACK_TYPE_NC, /* Normal-close jack */
	JACK_TYPE_MAX
};


typedef enum {
	HEADSET_4POLE_NORMAL,
	HEADSET_NO_MIC,
	HEADSET_4POLE_NOT_NORMAL,
	HEADSET_APPLE,
	HEADSET_TYPE_ERR = -1,
}SC2720_HEADSET_TYPE_T;

typedef struct{
	UINT32 adc_min;
	UINT32 adc_max;
	UINT32 code;
}SC2720_HEADSET_BUTTONS_T;

typedef struct {
	int				id;

	int				modes;
	int				currentmode;
	#if 0
	struct device			dev;

	struct iio_event_interface	*event_interface;

	struct iio_buffer		*buffer;
	struct list_head		buffer_list;
	int				scan_bytes;
	struct mutex			mlock;

	const unsigned long		*available_scan_masks;
	unsigned			masklength;
	const unsigned long		*active_scan_mask;
	bool				scan_timestamp;
	unsigned			scan_index_timestamp;
	struct iio_trigger		*trig;
	struct iio_poll_func		*pollfunc;
	struct iio_poll_func		*pollfunc_event;

	struct iio_chan_spec const	*channels;
	int				num_channels;

	struct list_head		channel_attr_list;
	struct attribute_group		chan_attr_group;
	const char			*name;
	const struct iio_info		*info;
	struct mutex			info_exist_lock;
	const struct iio_buffer_setup_ops	*setup_ops;
	struct cdev			chrdev;
#define IIO_MAX_GROUPS 6
	const struct attribute_group	*groups[IIO_MAX_GROUPS + 1];

	int				groupcounter;

	unsigned long			flags;
#if defined(CONFIG_DEBUG_FS)
	struct dentry			*debugfs_dentry;
	unsigned			cached_reg_addr;
#endif
#endif
}SC2720_II0_DEV_T ;//TODO

typedef struct {
	int			channel;
	int			channel2;
	unsigned long		address;
	int			scan_index;

}SC2720_IIO_CHAN_SPEC_T ;//TODO



typedef struct {
	SC2720_II0_DEV_T *indio_dev;
	const SC2720_IIO_CHAN_SPEC_T *channel;
	void *data;
}SC2720_IIO_CHANNEL_T;

enum {
	HDST_GPIO_DET_L = 0,
	HDST_GPIO_DET_H,
	HDST_GPIO_DET_MIC,
	HDST_GPIO_DET_ALL,
	HDST_GPIO_BUTTON,
	HDST_GPIO_MAX
};

typedef struct {
	UINT32 gpio_switch;
	UINT32 jack_type;
	UINT32 gpios[HDST_GPIO_MAX];
	UINT32 dbnc_times[HDST_GPIO_MAX]; /* debounce times */
	UINT32 irq_trigger_levels[HDST_GPIO_MAX];
	UINT32 adc_threshold_3pole_detect;
	UINT32 irq_threshold_button;
	UINT32 voltage_headmicbias;
	UINT32 sprd_adc_gnd;
	UINT32 sprd_half_adc_gnd;
	UINT32 sprd_one_half_adc_gnd;
	UINT32 sprd_stable_value;
	UINT32 coefficient;
	SC2720_HEADSET_BUTTONS_T *headset_buttons;
	UINT32 nbuttons;
	BOOL do_fm_mute;
}SC2720_HEADSET_PLATFORM_DATA_T;

typedef struct {
	INT headphone;
	INT irq_detect;
	INT irq_button;
	INT irq_detect_l;
	INT irq_detect_h;
	INT irq_detect_mic;
	INT irq_detect_all;
#if 0
	struct platform_device *pdev;
	//struct sprd_headset_platform_data pdata;
	struct delayed_work det_work;
	struct workqueue_struct *det_work_q;
	struct delayed_work det_all_work;
	struct workqueue_struct *det_all_work_q;
	struct work_struct btn_work;
	struct workqueue_struct *btn_work_q;
	struct snd_soc_codec *codec;
	struct sprd_headset_power power;
	struct semaphore sem;
	struct snd_soc_jack hdst_jack;
	struct snd_soc_jack btn_jack;
	enum snd_jack_types hdst_status;
	enum snd_jack_types btns_pressed;
	//struct iio_channel *adc_chan;
	struct mutex irq_btn_lock;
	struct mutex irq_det_lock;
	struct mutex irq_det_all_lock;
	struct mutex irq_det_mic_lock;
	struct delayed_work reg_dump_work;
	struct workqueue_struct *reg_dump_work_q;
	struct work_struct fc_work; /* for fast charge */
#ifdef ADPGAR_BYP_SELECT
	/* used for adpgar bypass selecting. */
	struct delayed_work adpgar_work;
	struct workqueue_struct *adpgar_work_q;
#endif
#endif
	INT debug_level;
	INT det_err_cnt; /* detecting error count */
	INT gpio_det_val_last; /* detecting gpio last value */
	INT gpio_btn_val_last; /* button detecting gpio last value */

	INT btn_stat_last; /* 0==released, 1==pressed */
	/* if the hardware detected a headset is
	 * plugged in, set plug_state_last = 1
	 */
	INT plug_stat_last;
	INT report;

	//struct wake_lock det_wakelock;
	//struct wake_lock det_all_wakelock;
	//struct wake_lock btn_wakelock;
}SC2720_HEADSET_T;


// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  __attribute__((section (".hal.globalvars.c")))

// =============================================================================
//  FUNCTIONS
// =============================================================================

#endif

