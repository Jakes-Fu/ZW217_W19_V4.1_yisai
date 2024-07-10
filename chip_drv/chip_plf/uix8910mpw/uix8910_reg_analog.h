/*******************************************************************************
 ** File Name:    uix8910_reg_analog.h                                        *
 ** Author:       windy                                                        *
 ** DATE:         09/19/11                                                     *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.            *
 ** Description:                                                               *
 *******************************************************************************/

/*******************************************************************************
 **                   Edit    History                                          *
 **----------------------------------------------------------------------------*
 ** DATE          NAME                 DESCRIPTION                             *
 ** 09/19/11      windy                Create.            
 ** 11/14/11      windy                Modified version 29 of NEMOG Analog Die Global Register.doc   *
 ******************************************************************************/
 
#ifndef _UIX8910_GLB_REG_H_
#define _UIX8910_GLB_REG_H_

#include "rda2720m_global.h"
#include "rda2720m_bltc.h"
#include "rda2720m_int.h"
#include "rda2720m_eic.h"

/*******************************************************************************
 **                      Compiler Flag                                         *
 *******************************************************************************/
#ifdef __cplusplus
extern   "C"
{
#endif

#define ANA_CHIP_ID_LOW               			(uint32)(&hwp_rda2720mGlobal->CHIP_ID_LOW)
#define ANA_CHIP_ID_HIGH               			(uint32)(&hwp_rda2720mGlobal->CHIP_ID_HIGH)
#define ANA_MODULE_EN0               			(uint32)(&hwp_rda2720mGlobal->MODULE_EN0)
#define ANA_ARM_CLK_EN0               			(uint32)(&hwp_rda2720mGlobal->ARM_CLK_EN0)
#define ANA_RTC_CLK_EN0               			(uint32)(&hwp_rda2720mGlobal->RTC_CLK_EN0)
#define ANA_SOFT_RST0               			(uint32)(&hwp_rda2720mGlobal->SOFT_RST0)
#define ANA_POWER_PD_SW               	    	(uint32)(&hwp_rda2720mGlobal->POWER_PD_SW)
#define ANA_POWER_PD_HW                 		(uint32)(&hwp_rda2720mGlobal->POWER_PD_HW)
#define ANA_LDO_CAMA_REG0               		(uint32)(&hwp_rda2720mGlobal->LDO_CAMA_REG0)
#define ANA_LDO_CAMA_REG1              		    (uint32)(&hwp_rda2720mGlobal->LDO_CAMA_REG1)
#define ANA_LDO_SIM0_REG0                		(uint32)(&hwp_rda2720mGlobal->LDO_SIM0_REG0)
#define ANA_LDO_SIM0_REG1                		(uint32)(&hwp_rda2720mGlobal->LDO_SIM0_REG1)
#define ANA_LDO_SIM0_REG2                		(uint32)(&hwp_rda2720mGlobal->LDO_SIM0_PD_REG)
#define ANA_LDO_SIM1_REG0                		(uint32)(&hwp_rda2720mGlobal->LDO_SIM1_REG0)
#define ANA_LDO_SIM1_REG1                		(uint32)(&hwp_rda2720mGlobal->LDO_SIM1_REG1)
#define ANA_LDO_SIM1_PD_REG            		    (uint32)(&hwp_rda2720mGlobal->LDO_SIM1_PD1_REG)
#define ANA_LDO_SD_REG0               			(uint32)(&hwp_rda2720mGlobal->LDO_SD_REG0)
#define ANA_LDO_SD_REG1               			(uint32)(&hwp_rda2720mGlobal->LDO_SD_REG1)
#define ANA_LDO_SD_REG2               			(uint32)(&hwp_rda2720mGlobal->LDO_SD_PD_REG)
#define ANA_LDO_SDIO_REG0               		(uint32)(&hwp_rda2720mGlobal->LDO_SDIO_REG0)
#define ANA_LDO_SDIO_REG1               		(uint32)(&hwp_rda2720mGlobal->LDO_SDIO_REG1)
#define ANA_LDO_SDIO_REG2               		(uint32)(&hwp_rda2720mGlobal->LDO_SDIO_PD_REG)
#define ANA_LDO_VDD28_REG0              		(uint32)(&hwp_rda2720mGlobal->LDO_VDD28_REG0)
#define ANA_LDO_VDD28_REG1              		(uint32)(&hwp_rda2720mGlobal->LDO_VDD28_REG1)
#define ANA_LDO_BTPHY_REG0                		(uint32)(&hwp_rda2720mGlobal->LDO_CON_REG0)
#define ANA_LDO_BTPHY_REG1                		(uint32)(&hwp_rda2720mGlobal->LDO_CON_REG1)
#define ANA_LDO_DCXO_REG0                		(uint32)(&hwp_rda2720mGlobal->LDO_DCXO_REG0)
#define ANA_LDO_DCXO_REG1               		(uint32)(&hwp_rda2720mGlobal->LDO_DCXO_REG1)
#define ANA_LDO_USB_REG0               	    	(uint32)(&hwp_rda2720mGlobal->LDO_USB_REG0)
#define ANA_LDO_USB_REG1               	    	(uint32)(&hwp_rda2720mGlobal->LDO_USB_REG1)
#define ANA_LDO_CAMD_REG0               		(uint32)(&hwp_rda2720mGlobal->LDO_CAMD_REG0)
#define ANA_LDO_CAMD_REG1               		(uint32)(&hwp_rda2720mGlobal->LDO_CAMD_REG1)
#define ANA_LDO_CAMIO_REG0               		(uint32)(&hwp_rda2720mGlobal->LDO_CAMIO_REG0)
#define ANA_LDO_CAMIO_REG1               		(uint32)(&hwp_rda2720mGlobal->LDO_CAMIO_REG1)
#define ANA_LDO_CAMMOT_REG0               		(uint32)(&hwp_rda2720mGlobal->LDO_CAMMOT_REG0)
#define ANA_LDO_CAMMOT_REG1               		(uint32)(&hwp_rda2720mGlobal->LDO_CAMMOT_REG1)
#define ANA_LDO_SF_REG0               			(uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)     //really no
#define ANA_LDO_SF_REG1               			(uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)     //really no
#define ANA_LDO_VDD18_REG0              		(uint32)(&hwp_rda2720mGlobal->LDO_AVDD18_REG0)
#define ANA_LDO_VDD18_REG1              		(uint32)(&hwp_rda2720mGlobal->LDO_AVDD18_REG1)
#define ANA_LDO_RF_REG0               			(uint32)(&hwp_rda2720mGlobal->LDO_RF18A_REG0)
#define ANA_LDO_RF_REG1               			(uint32)(&hwp_rda2720mGlobal->LDO_RF18A_REG1)
#define ANA_LDO_CH_CTRL               			(uint32)(&hwp_rda2720mGlobal->LDO_CH_CTRL)
#define ANA_LDO_REFTRIM_REG            	  	    (uint32)(&hwp_rda2720mGlobal->LDO_TRIM_REG)
#define ANA_SLP_CTRL               			  	(uint32)(&hwp_rda2720mGlobal->SLP_CTRL)
#define ANA_SLP_LDO_PD_CTRL0               	    (uint32)(&hwp_rda2720mGlobal->SLP_LDO_PD_CTRL0)
#define ANA_SLP_LDO_LP_CTRL0                 	(uint32)(&hwp_rda2720mGlobal->SLP_LDO_LP_CTRL0)
#define ANA_LDO_CORE_SLP_CTRL0               	(uint32)(&hwp_rda2720mGlobal->DCDC_CORE_SLP_CTRL0)
#define ANA_LDO_CORE_SLP_CTRL1               	(uint32)(&hwp_rda2720mGlobal->DCDC_CORE_SLP_CTRL1)
#define ANA_LDO_CORE_SLP_CTRL2               	(uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)   //really no
#define ANA_LDO_CORE_SLP_CTRL3               	(uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)   //really no
#define ANA_LDO_XTL_EN0               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN0)
#define ANA_LDO_XTL_EN1               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN1)
#define ANA_LDO_XTL_EN2               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN2)
#define ANA_LDO_XTL_EN3               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN3)
#define ANA_LDO_XTL_EN4               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN4)
#define ANA_LDO_XTL_EN5               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN5)
#define ANA_LDO_XTL_EN6               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN6)
#define ANA_LDO_XTL_EN7               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN7)
#define ANA_LDO_XTL_EN8               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN8)
#define ANA_LDO_XTL_EN9               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN9)
#define ANA_LDO_XTL_EN10               		  	(uint32)(&hwp_rda2720mGlobal->LDO_XTL_EN10)
#define ANA_CLK32KLESS_CTRL0               	    (uint32)(&hwp_rda2720mGlobal->CLK32KLESS_CTRL0)
#define ANA_CLK32KLESS_CTRL1               	    (uint32)(&hwp_rda2720mGlobal->CLK32KLESS_CTRL1)
#define ANA_XTL_WAIT_CTRL                		(uint32)(&hwp_rda2720mGlobal->XTL_WAIT_CTRL0)
#define ANA_IB_CTRL               				(uint32)(&hwp_rda2720mGlobal->IB_CTRL)
#define ANA_KPLED_CTRL               			(uint32)(&hwp_rda2720mGlobal->KPLED_CTRL0)
#define ANA_VIBR_CTRL               			(uint32)(&hwp_rda2720mGlobal->VIBR_CTRL1)
#define ANA_AUDIO_CTRL0               	  	  	(uint32)(&hwp_rda2720mGlobal->AUDIO_CTRL0)
#define ANA_AUDIO_CTRL1					        (uint32)(&hwp_rda2720mGlobal->AUDIO_CTRL0)
#define ANA_CHGR_CTL0               			(uint32)(&hwp_rda2720mGlobal->CHGR_CTRL0)
#define ANA_CHGR_CTL1               			(uint32)(&hwp_rda2720mGlobal->CHGR_CTRL1)
#define ANA_CHGR_CTL2               			(uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)   ///
#define ANA_CHGR_STATUS               			(uint32)(&hwp_rda2720mGlobal->CHGR_STATUS)
#define ANA_MIXED_CTRL0               			(uint32)(&hwp_rda2720mGlobal->OVLO_CTRL)
#define ANA_MIXED_CTRL1               			(uint32)(&hwp_rda2720mGlobal->MIXED_CTRL)
#define ANA_POR_RST_MONITOR               	    (uint32)(&hwp_rda2720mGlobal->POR_RST_MONITOR)
#define ANA_WDG_RST_MONITOR               	    (uint32)(&hwp_rda2720mGlobal->WDG_RST_MONITOR)
#define ANA_POR_PIN_RST_MONITOR                 (uint32)(&hwp_rda2720mGlobal->POR_PIN_RST_MONITOR)
#define ANA_POR_SRC_FLAG               		    (uint32)(&hwp_rda2720mGlobal->POR_SRC_FLAG)
#define ANA_POR_7S_CTRL               			(uint32)(&hwp_rda2720mGlobal->POR_7S_CTRL)
#define ANA_HWRST_RTC               			(uint32)(&hwp_rda2720mGlobal->HWRST_RTC)
#define ANA_ARCH_EN               				(uint32)(&hwp_rda2720mGlobal->ARCH_EN)
#define ANA_MCU_WR_PROT_VALUE               	(uint32)(&hwp_rda2720mGlobal->MCU_WR_PROT_VALUE)
#define ANA_PWR_WR_PROT_VALUE               	(uint32)(&hwp_rda2720mGlobal->PWR_WR_PROT_VALUE)
#define ANA_SMPL_CTRL0               			(uint32)(&hwp_rda2720mGlobal->SMPL_CTRL0)
#define ANA_SMPL_CTRL1               			(uint32)(&hwp_rda2720mGlobal->SMPL_CTRL1)
#define ANA_RTC_RST0               				(uint32)(&hwp_rda2720mGlobal->RTC_RST0)
#define ANA_RTC_RST1               				(uint32)(&hwp_rda2720mGlobal->RTC_RST1)
#define ANA_RTC_RST2               				(uint32)(&hwp_rda2720mGlobal->RTC_RST2)
#define ANA_RTC_CLK_STOP               		    (uint32)(&hwp_rda2720mGlobal->RTC_CLK_STOP)
#define ANA_VBAT_DROP_CNT               		(uint32)(&hwp_rda2720mGlobal->VBAT_DROP_CNT)
#define ANA_SWRST_CTRL0               			(uint32)(&hwp_rda2720mGlobal->SWRST_CTRL0)
#define ANA_SWRST_CTRL1               			(uint32)(&hwp_rda2720mGlobal->SWRST_CTRL1)
#define ANA_LOW_PWR_CLK32K_CTRL                 (uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)    //really no 
#define ANA_LOW_PWR_CLK32K_TEST0                (uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)
#define ANA_LOW_PWR_CLK32K_TEST1                (uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)
#define ANA_LOW_PWR_CLK32K_TEST2                (uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)
#define ANA_LOW_PWR_CLK32K_TEST3                (uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)
#define ANA_DCDC_WPA_REG0             		    (uint32)(&hwp_rda2720mGlobal->DCDC_WPA_REG0)
#define ANA_DCDC_WPA_REG1             		    (uint32)(&hwp_rda2720mGlobal->DCDC_WPA_REG1)
#define ANA_DCDC_WPA_REG2             		    (uint32)(&hwp_rda2720mGlobal->DCDC_WPA_REG2)
#define ANA_DCDC_WPA_VOL             		    (uint32)(&hwp_rda2720mGlobal->DCDC_WPA_VOL)

#define ANA_BLTC_PD_CTRL                        (uint32)(&hwp_rda2720mBltc->BLTC_PD_CTRL)
#define ANA_RG_RGB_V0                           (uint32)(&hwp_rda2720mBltc->RG_RGB_V0)
#define ANA_RG_RGB_V1                           (uint32)(&hwp_rda2720mBltc->RG_RGB_V1)
#define ANA_RG_RGB_V2                           (uint32)(&hwp_rda2720mBltc->RG_RGB_V2)
#define ANA_RG_RGB_V3                           (uint32)(&hwp_rda2720mBltc->RG_RGB_V3)
#define ANA_BLTC_CTRL                           (uint32)(&hwp_rda2720mBltc->BLTC_CTRL)

#define ANA_INTC_MASK_STATUS                    (uint32)(&hwp_rda2720mInt->INT_MASK_STATUS)
#define ANA_INTC_RAW_STATUS                     (uint32)(&hwp_rda2720mInt->INT_RAW_STATUS)
#define ANA_INTC_EN                             (uint32)(&hwp_rda2720mInt->INT_EN)

#define ANA_LDO_CORE_REG1    (uint32)(&hwp_rda2720mGlobal->RESERVED_REG18)//
#define ANA_CHIP_ID_P0  ANA_CHIP_ID_HIGH
#define ANA_CHIP_ID_P1  ANA_CHIP_ID_LOW
/*******************************************************************************
 ** Register Name        : ANA_CHIP_ID_LOW
 ** Register Offset      : 0x0000
 ** Register Description : Chip ID low part for this analog dice.
 ** Other Reset      : 
 ** Protect Register :
 ** Register field description:
 ** CHIP_ID_LOW               : [15:0 ] (RO)Chip ID low part for this analog    
 **                                    dice.
 **************************   Fields Definition *******************************/
#define    CHIP_ID_L16_SHIFT                (0)
#define    CHIP_ID_L16_MSK                  (0xFFFFUL << CHIP_ID_L16_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_CHIP_ID_HIGH
 ** Register Offset      : 0x0004
 ** Register Description : Chip ID high part for this analog dice.
 ** Register field description:
 ** CHIP_ID_HIGH               : [15:0 ] (RO)Chip ID high part for this analog   
 **                                    die.
 **************************   Fields Definition *******************************/
#define    CHIP_ID_H16_SHIFT                (0)
#define    CHIP_ID_H16_MSK                  (0xFFFFUL << CHIP_ID_H16_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_MODULE_EN0
 ** Register Offset      : 0x0008
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register :
 ** Register field description:
 ** PINREG_EN		   : [7] (RW)PINREG module enable,0: Disable the PCLK  of pin registers,1: Enable  the PCLK  of pin registers 
 ** PWM_EN               	   : [6] (RW)PWM module enable,0: Disable the PCLK  of PWM,1: Enable  the PCLK  of PWM                
 ** ADC_EN               	   : [5] (RW)AUXADC module enable,0: Disable the PCLK  of AUXADC,1: Enable  the PCLK  of AUXADC
 ** AUD_EN               	   : [4] (RW)Audio  module enable,0: Disable the PCLK  of Audio,1: Enable  the PCLK  of Audio
 ** EIC_EN               	   : [3] (RW)EIC module enable,0: Disable the PCLK  of EIC,1: Enable  the PCLK  of EIC
 ** WDG_EN                   : [2] (RW)WDG module enable,0: Disable the PCLK  of watchdog,1: Enable  the PCLK  of watchdog
 ** RTC_EN           	   : [1] (RW)RTC module enable,0: Disable the PCLK  of RTC,1: Enable  the PCLK  of RTC
 ** CAL_EN        		   : [0] (RW)CAL module enable,0: Disable the PCLK  of CAL,1: Enable  the PCLK  of CAL
 **************************   Fields Definition *******************************/
#define    ANA_TYPEC_EN	    	(BIT_14)
#define    ANA_TMR_EN	    	(BIT_12)
#define    ANA_BLTC_EN	    	(BIT_9)
#define    ANA_PINREG_EN		(BIT_8)
#define    ANA_FGU_EN			(BIT_7)
#define    ANA_EFS_EN			(BIT_6)
#define    ANA_ADC_EN			(BIT_5)
#define    ANA_AUD_EN			(BIT_4)
#define    ANA_EIC_EN			(BIT_3)
#define    ANA_WDG_EN			(BIT_2)
#define    ANA_RTC_EN			(BIT_1)
#define    ANA_CAL_EN			(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_ARM_CLK_EN0
 ** Register Offset      : 0x000C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register :
 ** Register field description:
 ** CLK_AUD_SCLK_EN		   	   : [7] (RW)Analog clock enable,0: disable CLK_AUD_SCLK,1: enable CLK_AUD_SCLK
 ** CLK_AUXAD_EN               	   : [6] (RW)AUXAD clock enable, the clock is connected to AUXADC converter,0: disable AUXAD_CLK,1: enable AUXAD_CLK
 ** CLK_AUXADC_EN               	   : [5] (RW)AUXADC module work clock enable,0: disable clk_adc,1: enable clk_adc
 ** CLK_CAL_EN				   : [2] (RW)CLK_CAL eanble,0: disable clk_cal,1: enable clk_cal
 ** CLK_AUD_IF_6P5M_EN             : [1] (RW)Audio 6.5M clock enable,0: disable clk_aud_6p5m_rx  and clk_aud_6p5m_tx,1: enable clk_aud_6p5m_rx  and clk_aud_6p5m_tx
 ** CLK_AUD_IF_EN                      : [0] (RW)Audio IF clock enable,0: disable CLK_AUDIF,1: enable CLK_AUDIF
 **************************   Fields Definition *******************************/
#define    ANA_CLK_AUD_SCLK_EN				(BIT_7)    //really no
#define    ANA_CLK_AUXAD_EN					(BIT_6)
#define    ANA_CLK_AUXADC_EN				(BIT_5)
#define    ANA_CLK_CAL_EN					(BIT_2)
#define    ANA_CLK_AUD_IF_6P5M_EN			(BIT_1)
#define    ANA_CLK_AUD_IF_EN				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_RTC_CLK_EN0
 ** Register Offset      : 0x0010
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register :
 ** Register field description:
 ** RTC_PWM_EN		   	   : [4] (RW)The RTC clock enable of PWM module
 ** RTC_EIC_EN               	   : [3] (RW)EIC RTC clock soft enable,0: Disable the RTC clock of EIC,1: Enable RTC clock of EIC
 ** RTC_WDG_EN               	   : [2] (RW)Watchdog RTC clock soft enable,0: Disable the RTC clock of Watchdog,1: Enable RTC clock of Watchdo
 ** RTC_RTC_EN			   : [1] (RW)RTC RTC clock soft enable,0: Disable the RTC clock of RTC,1: Enable RTC clock of RTC
 ** RTC_ARCH_EN			   : [0] (RW)ARCH RTC clock soft enable,0: Disable the RTC clock of ARCH,1: Enable RTC clock of ARCH
 **************************   Fields Definition *******************************/
#define    ANA_RTC_TYPEC_EN				(BIT_15)
#define    ANA_RTC_TMR_EN				(BIT_13)
#define    ANA_RTC_FLASH_EN				(BIT_12)
#define    ANA_RTC_EFS_EN				(BIT_11)
#define    ANA_RTC_BLTC_EN				(BIT_7)
#define    ANA_RTC_FGU_EN				(BIT_6)
#define    ANA_RTC_EIC_EN				(BIT_3)
#define    ANA_RTC_WDG_EN				(BIT_2)
#define    ANA_RTC_RTC_EN				(BIT_1)
#define    ANA_RTC_ARCH_EN				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_SOFT_RST0
 ** Register Offset      : 0x0014
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register :
 ** Register field description:
 ** AUDRX_SOFT_RST		   	   : [8] (RW)AUD RX soft reset
 ** AUDTX_SOFT_RST		   	   : [7] (RW)AUD TX soft reset
 ** AUD_IF_SOFT_RST		   	   : [6] (RW)Audio IF soft reset
 ** ADC_SOFT_RST				   : [5] (RW)Auxadc soft reset
 ** PWM_SOFT_RST		   	   : [4] (RW)PWM soft reset
 ** EIC_SOFT_RST               	   : [3] (RW)EIC soft reset
 ** WDG_SOFT_RST               	   : [2] (RW)Watchdog soft reset
 ** RTC_SOFT_RST				   : [1] (RW)RTC soft reset
 ** CAL_SOFT_RST				   : [0] (RW)CAL soft reset
 **************************   Fields Definition *******************************/
#define    ANA_AUDRX_SOFT_RST			(BIT_8)
#define    ANA_AUDTX_SOFT_RST			(BIT_7)
#define    ANA_AUD_IF_SOFT_RST			(BIT_6)
#define    ANA_ADC_SOFT_RST				(BIT_5)
#define    ANA_PWM_SOFT_RST			(BIT_4)
#define    ANA_EIC_SOFT_RST				(BIT_3)
#define    ANA_WDG_SOFT_RST			(BIT_2)
#define    ANA_RTC_SOFT_RST				(BIT_1)
#define    ANA_CAL_SOFT_RST				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_POWER_PD_SW
 ** Register Offset      : 0x0018
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : PWR_WR_PROT_VALUE.PWR_WR_PROT
 ** Register field description:
 ** LDO_DCXO_PD		   	   : [5] (RW)LDO_DCXO power down,1: power down,0: power on
 ** LDO_CORE_PD		   	   : [4] (RW)DCDC_CORE power down,1: power down,0: power on
 ** LDO_SF_PD		   	   : [3] (RW)DCDC_ARM1 power down,1: power down,0: power on
 ** LDO_VDD18_PD		   : [2] (RW)LDO_AVDD18  power down,1: power down,0: power on
 ** LDO_VDD28_PD		   : [1] (RW)LDO_VDD28 power down,1: power down,0: power on
 ** BG_PD               	   	   : [0] (RW)Bandgap power down,1: power down,0: power on
 **************************   Fields Definition *******************************/
#define    LDO_DCXO_PD				(BIT_5)
#define    LDO_CORE_PD				(BIT_4)
#define    LDO_SF_PD					(BIT_3)
#define    LDO_VDD18_PD				(BIT_2)
#define    LDO_VDD28_PD				(BIT_1)
#define    BG_PD						(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_POWER_PD_HW
 ** Register Offset      : 0x001C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : PWR_WR_PROT_VALUE.PWR_WR_PROT
 ** Register field description:
 ** PWR_OFF_SEQ_EN		   	   : [0] (RW)Power off_sequence enable that is protected by reg PWR_WR_PROT_VALUE.PWR_WR_PROT
 **************************   Fields Definition *******************************/
#define    PWR_OFF_SEQ_EN			(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CAMA_REG0
 ** Register Offset      : 0x0038
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register :   ** Register field description:
 ** LDO_CAMA_LP_EN_SW		   : [8] (RW)LDO_CAMA low power mode enable software control,0: depends on SLP_LDO_CAMA_LP_EN control,1: Enable
 ** LDO_CAMA_SHPT_PD		   : [1] (RW)LDO short protection power down
 ** LDO_CAMA_PD               	   : [0] (RW)LDO_CAMA  power down,1: power down,0: power on   
 **************************   Fields Definition *******************************/
#define    LDO_CAMA_LP_EN_SW		(BIT_8)
#define    LDO_CAMA_SHPT_PD		(BIT_1)
#define    LDO_CAMA_PD				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CAMA_REG1
 ** Register Offset      : 0x003C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CAMA_V		   : [7:0] (RW)CAMA1 LDO program bits: 8'b0 as 1.2V output, 10mV/step,default 2.8v  
 **************************   Fields Definition *******************************/
#define    LDO_CAMA_V_SHIFT             (0)
#define    LDO_CAMA_V_MSK               (0x7FUL << LDO_CAMA_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SIM0_REG0
 ** Register Offset      : 0x0040
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SIM0_LP_EN_SW		   : [8] (RW)LDO_SIM0 low power mode enable software control,0: depends on SLP_LDO_SIM0_LP_EN control,1: Enable
 ** LDO_SIM0_SHPT_PD		   : [1] (RW)LDO short protection power down
 **************************   Fields Definition *******************************/
#define    LDO_SIM0_LP_EN_SW		(BIT_8)
#define    LDO_SIM0_SHPT_PD			(BIT_1)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SIM0_REG1
 ** Register Offset      : 0x0044
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SIM0_V		   : [7:0] (RW)VDDSIM0 LDO program bits: 8'b0 as 1.2V output, 10mV/step, 1.8V default
 **************************   Fields Definition *******************************/
#define    LDO_SIM0_V_SHIFT             (0)
#define    LDO_SIM0_V_MSK               (0xFFUL << LDO_SIM0_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SIM0_REG2
 ** Register Offset      : 0x0048
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SIM0_PD		   : [0] (RW)LDO_SIM0  power down,1: power down,0: power on   
 **************************   Fields Definition *******************************/
#define    LDO_SIM0_PD				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SIM1_REG0
 ** Register Offset      : 0x004C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SIM1_LP_EN_SW		   : [8] (RW)LDO_SIM1 low power mode enable software control,0: depends on SLP_LDO_SIM1_LP_EN control,1: Enable
 ** LDO_SIM1_SHPT_PD		   : [1] (RW)LDO short protection power down
 **************************   Fields Definition *******************************/
#define    LDO_SIM1_LP_EN_SW		(BIT_8)
#define    LDO_SIM1_SHPT_PD			(BIT_1)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SIM1_REG1
 ** Register Offset      : 0x0050
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SIM1_V		   : [7:0] (RW)VDDSIM1 LDO program bits: 8'b0 as 1.2V output, 10mV/step, 1.8V default
 **************************   Fields Definition *******************************/
#define    LDO_SIM1_V_SHIFT             (0)
#define    LDO_SIM1_V_MSK               (0xFFUL << LDO_SIM1_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SIM1_PD_REG
 ** Register Offset      : 0x0054
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SIM1_PD_REG		   : [0] (RW)LDO_SIM1  power down,1: power down,0: power on  
 **************************   Fields Definition *******************************/
#define    LDO_SIM1_PD_REG			(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SD_REG0
 ** Register Offset      : 0x0058
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SD_LP_EN_SW		   	   : [8] (RW)LDO_SD low power mode enable software control,0: depends on SLP_LDO_SD_LP_EN control,1: Enable
 ** LDO_SD_SHPT_PD		   	   : [1] (RW)LDO short protection power down
 **************************   Fields Definition *******************************/
#define    LDO_SD_LP_EN_SW			(BIT_8)
#define    LDO_SD_SHPT_PD			(BIT_1)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SD_REG1
 ** Register Offset      : 0x005C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SD_V		   : [7:0] (RW)Connectivity SD LDO program bits: 8'b0 as 1.2V output, 10mV/step,default 3.0v
 **************************   Fields Definition *******************************/
#define    LDO_SD_V_SHIFT             (0)
#define    LDO_SD_V_MSK               (0x7FUL << LDO_SD_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SD_REG2
 ** Register Offset      : 0x0060
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SD_PD		   : [0] (RW)LDO_SD  power down,1: power down,0: power on  
 **************************   Fields Definition *******************************/
#define    LDO_SD_PD					(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_VDD28_REG0
 ** Register Offset      : 0x0064
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_VDD28_LP_EN_SW		   	   : [8] (RW)LDO_VDD28 low power mode enable software control,0: depends on SLP_LDO_VDD28_LP_EN control,1: Enable
 ** LDO_VDD28_SHPT_PD		   	   : [1] (RW)LDO short protection power down
 **************************   Fields Definition *******************************/
#define    LDO_VDD28_LP_EN_SW			(BIT_8)
#define    LDO_VDD28_SHPT_PD			(BIT_1)

/*******************************************************************************
 ** Register Name        : ANA_LDO_VDD28_REG1
 ** Register Offset      : 0x0068
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_VDD28_V		   : [7:0] (RW)2.8V LDO program bits: 1.8~3.3 ,10mV/step, 2.8V default
 **************************   Fields Definition *******************************/
#define    LDO_VDD28_V_SHIFT             (0)
#define    LDO_VDD28_V_MSK               (0xFFUL << LDO_VDD28_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_BTPA_REG0
 ** Register Offset      : 0x006C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_BTPA_BYPASS		   : [9] (RW)LDO BTPA BYPASS,"1" Output short to VBAT,"0" LDO mode 
 ** LDO_BTPA_SHPT_PD	   : [1] (RW)LDO short protection power down
 ** LDO_BTPA_PD		   	   : [0] (RW)LDO_BTPA  power down,1: power down,0: power on     
 **************************   Fields Definition *******************************/
#define    LDO_BTPA_BYPASS			(BIT_9)
#define    LDO_BTPA_SHPT_PD			(BIT_1)
#define    LDO_BTPA_PD				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_BTPA_REG1
 ** Register Offset      : 0x0070
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_BTPA_V		   : [7:0] (RW)BT PA LDO program bits: 8'b0 as 1.2V output, 10mV/step,default 3.3v
 **************************   Fields Definition *******************************/
#define    LDO_BTPA_V_SHIFT             (0)
#define    LDO_BTPA_V_MSK               (0xFFUL << LDO_BTPA_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_DCXO_REG0
 ** Register Offset      : 0x0074
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_DCXO_LP_EN_SW		   : [8] (RW)LDO_DCXO low power mode enable software control,0: depends on SLP_LDO_DCXO_LP_EN control,1: Enable
 ** LDO_DCXO_SHPT_PD	   	   : [1] (RW)LDO short protection power down 
 **************************   Fields Definition *******************************/
#define    LDO_DCXO_LP_EN_SW			(BIT_8)
#define    LDO_DCXO_SHPT_PD			(BIT_1)

/*******************************************************************************
 ** Register Name        : ANA_LDO_DCXO_REG1
 ** Register Offset      : 0x0078
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_DCXO_V		   : [7:0] (RW)DCXO LDO program bits: 8'b0 as 1.2V output, 10mV/step,default 1.8v
 **************************   Fields Definition *******************************/
#define    LDO_DCXO_V_SHIFT             (0)
#define    LDO_DCXO_V_MSK               (0xFFUL << LDO_DCXO_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_USB_REG0
 ** Register Offset      : 0x007C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_USB_LP_EN_SW		: [8] (RW)LDO_DCXO low power mode enable software control,0: depends on SLP_LDO_DCXO_LP_EN control,1: Enable
 ** LDO_USB_SHPT_PD	   	   	: [1] (RW)LDO short protection power down 
 ** LDO_USB_PD		   		: [0] (RW)LDO_USB33  power down,1: power down,0: power on  
 **************************   Fields Definition *******************************/
#define    LDO_USB_LP_EN_SW		(BIT_8)
#define    LDO_USB_SHPT_PD			(BIT_1)
#define    LDO_USB_PD				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_USB_REG1
 ** Register Offset      : 0x0080
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_USB_V		   : [7:0] (RW)USB33 LDO program bits: 8'b0 as 1.2V output, 10mV/step,default 3.3v
 **************************   Fields Definition *******************************/
#define    LDO_USB_V_SHIFT             (0)
#define    LDO_USB_V_MSK               (0xFFUL << LDO_USB_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CAMD_REG0
 ** Register Offset      : 0x0084
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CAMD_LP_EN_SW		: [8] (RW)LDO_CAMD low power mode enable software control,0: depends on SLP_LDO_CAMD_LP_EN control,1: Enable
 ** LDO_CAMD_SHPT_PD	   	: [1] (RW)LDO short protection power down
 ** LDO_CAMD_PD		   		: [0] (RW)LDO_CAMD  power down,1: power down,0: power on  
 **************************   Fields Definition *******************************/
#define    LDO_CAMD_LP_EN_SW		(BIT_8)
#define    LDO_CAMD_SHPT_PD		(BIT_1)
#define    LDO_CAMD_PD				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CAMD_REG1
 ** Register Offset      : 0x0088
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CAMD_V		   : [7:0] (RW)CAMD LDO program bits: 8'b0 as 1.2V output, 10mV/step, 1.8V default
 **************************   Fields Definition *******************************/
#define    LDO_CAMD_V_SHIFT             (0)
#define    LDO_CAMD_V_MSK               (0x3FUL << LDO_CAMD_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CAMIO_REG0
 ** Register Offset      : 0x0084
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CAMD_LP_EN_SW		: [8] (RW)LDO_CAMD low power mode enable software control,0: depends on SLP_LDO_CAMD_LP_EN control,1: Enable
 ** LDO_CAMD_SHPT_PD	   	: [1] (RW)LDO short protection power down
 ** LDO_CAMD_PD		   		: [0] (RW)LDO_CAMD  power down,1: power down,0: power on  
 **************************   Fields Definition *******************************/
#define    LDO_CAMIO_LP_EN_SW		(BIT_8)
#define    LDO_CAMIO_SHPT_PD		(BIT_6)
#define    LDO_CAMIO_PD				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CAMIO_REG1
 ** Register Offset      : 0x0088
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CAMD_V		   : [7:0] (RW)CAMD LDO program bits: 8'b0 as 1.2V output, 10mV/step, 1.8V default
 **************************   Fields Definition *******************************/
#define    LDO_CAMIO_V_SHIFT             (0)
#define    LDO_CAMIO_V_MSK               (0x3FUL << LDO_CAMIO_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CAMMOT_REG0
 ** Register Offset      : 0x0084
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CAMD_LP_EN_SW		: [8] (RW)LDO_CAMD low power mode enable software control,0: depends on SLP_LDO_CAMD_LP_EN control,1: Enable
 ** LDO_CAMD_SHPT_PD	   	: [1] (RW)LDO short protection power down
 ** LDO_CAMD_PD		   		: [0] (RW)LDO_CAMD  power down,1: power down,0: power on  
 **************************   Fields Definition *******************************/
#define    LDO_CAMMOT_LP_EN_SW		(BIT_8)
#define    LDO_CAMMOT_SHPT_PD		(BIT_6)
#define    LDO_CAMMOT_PD			(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CAMIO_REG1
 ** Register Offset      : 0x0088
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CAMD_V		   : [7:0] (RW)CAMD LDO program bits: 8'b0 as 1.2V output, 10mV/step, 1.8V default
 **************************   Fields Definition *******************************/
#define    LDO_CAMMOT_V_SHIFT             (0)
#define    LDO_CAMMOT_V_MSK               (0x7FUL << LDO_CAMMOT_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SF_REG0
 ** Register Offset      : 0x008C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SF_V_SEL		   	: [10] (RW)LDO SF voltage select:0: from bonding option,1: from software register
 ** LDO_SF_OPTION_O		: [9] (RO)VDDSF LDO Output voltage select signal,0:1.8V(default),1:3.0V
 ** LDO_SF_LP_EN_SW		: [8] (RW)LDO_SF low power mode enable software control,0: depends on SLP_LDO_SF_LP_EN control,1: Enable
 ** LDO_SF_SHPT_PD	   	: [1] (RW)LDO short protection power down
 **************************   Fields Definition *******************************/
#define    LDO_SF_V_SEL			(BIT_10)
#define    LDO_SF_OPTION_O		(BIT_9)
#define    LDO_SF_LP_EN_SW		(BIT_8)
#define    LDO_SF_SHPT_PD		(BIT_1)

#define LDO_SF_VOLT_3000  (LDO_SF_OPTION_O)
#define LDO_SF_VOLT_1800  (0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_SF_REG1
 ** Register Offset      : 0x0090
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_SF_V		   : [7:0] (RW)VDDSF LDO program bits: , 1.8V-3.3V, 10mV/step,  1.2V default
 **************************   Fields Definition *******************************/
#define    LDO_SF_V_SHIFT             (0)
#define    LDO_SF_V_MSK               (0xFFUL << LDO_SF_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_VDD18_REG0
 ** Register Offset      : 0x0094
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_VDD18_LP_EN_SW		: [8] (RW)LDO_VDD18 low power mode enable software control,0: depends on SLP_LDO_VDD18_LP_EN control,1: Enable
 ** LDO_VDD18_SHPT_PD	   	: [1] (RW)LDO short protection power down
 **************************   Fields Definition *******************************/
#define    LDO_VDD18_LP_EN_SW		(BIT_8)
#define    LDO_VDD18_SHPT_PD		(BIT_1)

/*******************************************************************************
 ** Register Name        : ANA_LDO_VDD18_REG1
 ** Register Offset      : 0x0098
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_VDD18_V		   : [7:0] (RW)VDD18 LDO program bits: 1.8~3.3 , 10mV/step, 1.8V default
 **************************   Fields Definition *******************************/
#define    LDO_VDD18_V_SHIFT             (0)
#define    LDO_VDD18_V_MSK               (0xFFUL << LDO_VDD18_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_RF_REG0
 ** Register Offset      : 0x009C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_RF_LP_EN_SW		: [8] (RW)LDO_RF low power mode enable software control,0: depends on SLP_LDO_RF_LP_EN control,1: Enable
 ** LDO_RF_SHPT_PD	   	: [1] (RW)LDO short protection power down
 ** LDO_RF_PD	   		: [0] (RW)LDO_RF power down,1: power down,0: power on
 **************************   Fields Definition *******************************/
#define    LDO_RF_LP_EN_SW		(BIT_8)
#define    LDO_RF_SHPT_PD		(BIT_1)
#define    LDO_RF_PD				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_RF_REG1
 ** Register Offset      : 0x00A0
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_RF_V		   : [6:0] (RW)VDDSF LDO program bits: 1.2~2.0V, 6.25mV/step,1.8V default
 **************************   Fields Definition *******************************/
#define    LDO_RF_V_SHIFT             (0)
#define    LDO_RF_V_MSK               (0x7FUL << LDO_RF_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CORE_REG0
 ** Register Offset      : 0x00A4
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CORE_LP_EN_SW		: [8] (RW)LDO_CORE low power mode enable software control,0: depends on SLP_LDO_CORE_LP_EN control,1: Enable
 ** LDO_CORE_SHPT_PD	   	: [1] (RW)LDO short protection power down
 **************************   Fields Definition *******************************/
#define    LDO_CORE_LP_EN_SW		(BIT_8)
#define    LDO_CORE_SHPT_PD		(BIT_1)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CORE_REG1
 ** Register Offset      : 0x00A8
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CORE_V		   : [5:0] (RW)VDDCORE LDO program bits: output, 0.75~1.35V, 10mV/step, 1.1V default
 **************************   Fields Definition *******************************/
#define    LDO_CORE_V_SHIFT             (0)
#define    LDO_CORE_V_MSK               (0x3FUL << LDO_CORE_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CH_CTRL
 ** Register Offset      : 0x00C4
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** EFS_PROG_SUP_PD		: [14] (RW)Ddie efuse program mode power supply switch on/off control signal,"1" switch off,"0" switch on
 ** VBATBK_V				: [13:12] (RW)Backup battery output program bits,
 								00:2.6,
 								01:2.8 (Default),
 								10:3.0,
 								11:3.2
 ** LDOB_CAL_SEL			: [7:4] (RW)VBAT1 LDO TRIM CONTROL BITS:
								0000: cal disable (default)
								0001: LDO VDDCORE cal enable;
								0010: LDO VDDRF cal enable;
								0011: LDO VDDBTPA cal enable;
								0100: LDO VDDDCXO cal enable;
								0101: LDO VDD18 cal enable;
								0110: LDO VDD28 cal enable;
								0111: LDO_AVSS;
								1000: LDO_AVSS;
								1001: LDOB REF cal enable; 
								1010: LDODCXO REF cal enable; 
								others: none 
 ** LDOA_CAL_SEL  	   	: [3:0] (RW)VBAT1 LDO TRIM CONTROL BITS:
								0000: cal disable (default)
								0001: LDO VDDSD cal enable;
								0010: LDO VDDCAMA cal enable;
								0011: LDO VDDCAMD cal enable;
								0100: LDO VDDVIBR cal enable;
								0101: LDO VDDSIM0 cal enable;
								0110: LDO VDDSIM1 cal enable;
								0111: LDO VDDSF cal enable;                                                                  
								1000: LDO VDDUSB cal enable;                                                              
								1001: LDOA REF cal enable; 
								others: none  
 **************************   Fields Definition *******************************/
#define    EFS_PROG_SUP_PD				(BIT_14)
#define    VBATBK_V_CH_SHIFT             	(12)
#define    VBATBK_V_CH_MSK               		(0x3UL << VBATBK_V_CH_SHIFT)
#define    LDOB_CAL_SEL_CH_SHIFT            (4)
#define    LDOB_CAL_SEL_CH_MSK               (0xFUL << LDOB_CAL_SEL_CH_SHIFT)
#define    LDOA_CAL_SEL_CH_SHIFT            (0)
#define    LDOA_CAL_SEL_CH_MSK               (0xFUL << LDOA_CAL_SEL_CH_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_REFTRIM_REG
 ** Register Offset      : 0x00C8
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDODCXO_CAL_SEL		: [14:10] (RW)LDOBTOP DCXOTRIM program bits: 8mV/step 1.2V default
 ** LDOB_CAL_SEL		: [9:5] (RW)LDOBTOP TRIM program bits: 8mV/step 1.2V default
 ** LDOA_CAL_SEL  	   	: [4:0] (RW)LDOATOP TRIM program bits: 8mV/step 1.2V default
 **************************   Fields Definition *******************************/
#define    LDODCXO_CAL_SEL_REFTRIM_SHIFT            (10)
#define    LDODCXO_CAL_SEL_REFTRIM_MSK              (0x1FUL << LDODCXO_CAL_SEL_REFTRIM_SHIFT)
#define    LDOB_CAL_SEL_REFTRIM_SHIFT            	(5)
#define    LDOB_CAL_SEL_REFTRIM_MSK               	(0x1FUL << LDOB_CAL_SEL_REFTRIM_SHIFT)
#define    LDOA_CAL_SEL_REFTRIM_SHIFT            	(0)
#define    LDOA_CAL_SEL_REFTRIM_MSK               	(0x1FUL << LDOA_CAL_SEL_REFTRIM_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_SLP_CTRL
 ** Register Offset      : 0x00DC
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_XTL_EN			: [3] (RW)LDO and DCDC can be controlled by external device if this bit is set
 ** SLP_IO_EN			: [1] (RW)IO PAD sleep enable in deep sleep mode
 ** SLP_LDO_PD_EN  	   	: [0] (RW)ALL LDO and DCDC power down enable in deep sleep mode
 **************************   Fields Definition *******************************/
#define    LDO_XTL_EN		(BIT_3)
#define    SLP_IO_EN			(BIT_1)
#define    SLP_LDO_PD_EN	(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_SLP_LDO_PD_CTRL0
 ** Register Offset      : 0x00E0
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** SLP_LDORF_PD_EN			: [15] (RW)LDO RF power down enable in deep sleep mode
 ** SLP_LDOSF_PD_EN  	   	: [14] (RW)LDO SF power down enable in deep sleep mode
 ** SLP_LDODCXO_PD_EN		: [13] (RW)LDO DCXO power down enable in deep sleep mode
 ** SLP_LDOBTPA_PD_EN		: [12] (RW)LDO BTPA power down enable in deep sleep mode
 ** SLP_LDOVDD28_PD_EN  	   	: [11] (RW)LDO VDD28 power down enable in deep sleep mode
 ** SLP_LDOSD_PD_EN			: [10] (RW)LDO SD power down enable in deep sleep mode
 ** SLP_LDOUSB_PD_EN		: [8] (RW)LDO USB power down enable in deep sleep mode
 ** SLP_LDOCAMD_PD_EN  	   	: [5] (RW)LDO CAMD power down enable in deep sleep mode
 ** SLP_LDOCAMA_PD_EN		: [3] (RW)LDO CAMA power down enable in deep sleep mode
 ** SLP_LDOSIM1_PD_EN		: [1] (RW)LDO SIM1 power down enable in deep sleep mode
 ** SLP_LDOSIM0_PD_EN  	   	: [0] (RW)LDO SIM0 power down enable in deep sleep mode
 **************************   Fields Definition *******************************/
#define    SLP_LDORF_PD_EN			(BIT_15)
#define    SLP_LDOSF_PD_EN			(BIT_14)
#define    SLP_LDODCXO_PD_EN		(BIT_13)
#define    SLP_LDOBTPA_PD_EN		(BIT_12)
#define    SLP_LDOVDD28_PD_EN		(BIT_11)
#define    SLP_LDOSD_PD_EN			(BIT_10)
#define    SLP_LDOUSB_PD_EN			(BIT_8)
#define    SLP_LDOCAMD_PD_EN		(BIT_5)
#define    SLP_LDOCAMA_PD_EN		(BIT_3)
#define    SLP_LDOSIM1_PD_EN		(BIT_1)
#define    SLP_LDOSIM0_PD_EN		(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_SLP_LDO_LP_CTRL0
 ** Register Offset      : 0x00E4
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** SLP_LDORF_LP_EN			: [15] (RW)LDO RF low power mode enable in deep sleep mode
 ** SLP_LDOSF_LP_EN  	   		: [14] (RW)LDO SF low power mode enable in deep sleep mode
 ** SLP_LDODCXO_LP_EN		: [13] (RW)LDO DCXO low power mode enable in deep sleep mode
 ** SLP_LDOVDD28_LP_EN  	   	: [11] (RW)LDO VDD28 low power mode enable in deep sleep mode
 ** SLP_LDOSD_LP_EN			: [10] (RW)LDO SD low power mode enable in deep sleep mode,0: Disable,1: Enable
 ** SLP_LDOVDD18_LP_EN		: [9] (RW)LDO VDD18 low power mode enable in deep sleep mode
 ** SLP_LDOUSB_LP_EN			: [8] (RW)LDO USB low power mode enable in deep sleep mode,0: Disable,1: Enable
 ** SLP_LDOCAMD_LP_EN  	   	: [5] (RW)LDO CAMD low power mode enable in deep sleep mode,0: Disable,1: Enable
 ** SLP_LDOCAMA_LP_EN		: [3] (RW)LDO CAMA low power mode enable in deep sleep mode,0: Disable,1: Enable
 ** SLP_LDOCORE_LP_EN		: [2] (RW)LDO CORE low power mode enable in deep sleep mode,0: Disable,1: Enable
 ** SLP_LDOSIM1_LP_EN		: [1] (RW)LDO SIM1 low power mode enable in deep sleep mode,0: Disable,1: Enable
 ** SLP_LDOSIM0_LP_EN  	   	: [0] (RW)LDO SIM0 low power mode enable in deep sleep mode,0: Disable,1: Enable
 **************************   Fields Definition *******************************/
#define    SLP_LDORF_LP_EN			(BIT_15)
#define    SLP_LDOSF_LP_EN			(BIT_14)
#define    SLP_LDODCXO_LP_EN		(BIT_13)
#define    SLP_LDOVDD28_LP_EN		(BIT_11)
#define    SLP_LDOSD_LP_EN			(BIT_10)
#define    SLP_LDOVDD18_LP_EN		(BIT_9)
#define    SLP_LDOUSB_LP_EN			(BIT_8)
#define    SLP_LDOCAMD_LP_EN		(BIT_5)
#define    SLP_LDOCAMA_LP_EN		(BIT_3)
#define    SLP_LDOCORE_LP_EN		(BIT_2)
#define    SLP_LDOSIM1_LP_EN		(BIT_1)
#define    SLP_LDOSIM0_LP_EN		(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CORE_SLP_CTRL0
 ** Register Offset      : 0x00E8
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** SLP_LDOCORE_DROP_EN				: [2] (RW)LDO CORE power drop enable in deep sleep mode
 ** LDO_CORE_SLP_OUT_STEP_EN		: [1] (RW)LDO CORE wakeup voltage step enable
 ** LDO_CORE_SLP_IN_STEP_EN  	   	: [0] (RW)LDO CORE sleep  voltage step enable
 **************************   Fields Definition *******************************/
#define    SLP_LDOCORE_DROP_EN				(BIT_2)
#define    LDO_CORE_SLP_OUT_STEP_EN		(BIT_1)
#define    LDO_CORE_SLP_IN_STEP_EN			(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CORE_SLP_CTRL1
 ** Register Offset      : 0x00EC
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CORE_V_SLP_STEP1		: [11:6] (RW)LDO CORE sleep control of step1
 ** LDO_CORE_V_DS  	   			: [5:0] (RW)LDO CORE control bits in deep sleep mode
 **************************   Fields Definition *******************************/
#define    LDO_CORE_V_SLP_STEP1_SHIFT            	(6)
#define    LDO_CORE_V_SLP_STEP1_MSK               	(0x3FUL << LDO_CORE_V_SLP_STEP1_SHIFT)
#define    LDO_CORE_V_DS_SHIFT            			(0)
#define    LDO_CORE_V_DS_MSK               			(0x3FUL << LDO_CORE_V_DS_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CORE_SLP_CTRL2
 ** Register Offset      : 0x00F0
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CORE_V_SLP_STEP3		: [11:6] (RW)LDO CORE sleep control of step3
 ** LDO_CORE_V_SLP_STEP2  	   	: [5:0] (RW)LDO CORE sleep control of step2
 **************************   Fields Definition *******************************/
#define    LDO_CORE_V_SLP_STEP3_SHIFT            	(6)
#define    LDO_CORE_V_SLP_STEP3_MSK               	(0x3FUL << LDO_CORE_V_SLP_STEP3_SHIFT)
#define    LDO_CORE_V_SLP_STEP2_SHIFT            	(0)
#define    LDO_CORE_V_SLP_STEP2_MSK               	(0x3FUL << LDO_CORE_V_SLP_STEP2_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_CORE_SLP_CTRL3
 ** Register Offset      : 0x00F4
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_CORE_V_SLP_STEP5		: [11:6] (RW)LDO CORE sleep control of step5
 ** LDO_CORE_V_SLP_STEP4  	   	: [5:0] (RW)LDO CORE sleep control of step4
 **************************   Fields Definition *******************************/
#define    LDO_CORE_V_SLP_STEP5_SHIFT            	(6)
#define    LDO_CORE_V_SLP_STEP5_MSK               	(0x3FUL << LDO_CORE_V_SLP_STEP5_SHIFT)
#define    LDO_CORE_V_SLP_STEP4_SHIFT            	(0)
#define    LDO_CORE_V_SLP_STEP4_MSK               	(0x3FUL << LDO_CORE_V_SLP_STEP4_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LDO_XTL_EN0
 ** Register Offset      : 0x00F8
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_DCXO_EXT_XTL_EN			: [15] (RW)LDO DCXO can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_DCXO_XTL_BUF_EN  	   	: [14] (RW)LDO DCXO can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_CORE_EXT_XTL_EN			: [13] (RW)LDO CORE can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_CORE_XTL_BUF_EN  	   	: [12] (RW)LDO CORE can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_SIM0_EXT_XTL_EN  	   	: [11] (RW)LDO SIM0 can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_SIM0_XTL_BUF_EN			: [10] (RW)LDO SIM0 can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_SIM1_EXT_XTL_EN			: [9] (RW)LDO SIM1 can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_SIM1_XTL_BUF_EN			: [8] (RW)LDO SIM1 can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_CAMA_EXT_XTL_EN  	   	: [7] (RW)LDO CAMA can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_CAMA_XTL_BUF_EN  	   	: [6] (RW)LDO CAMA can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_CAMD_EXT_XTL_EN  	   	: [5] (RW)LDO CAMD can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_CAMD_XTL_BUF_EN			: [4] (RW)LDO CAMD can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_VDD18_EXT_XTL_EN		: [3] (RW)LDO VDD18 can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_VDD18_XTL_BUF_EN		: [2] (RW)LDO VDD18 can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_VDD28_EXT_XTL_EN		: [1] (RW)LDO VDD28 can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_VDD28_XTL_BUF_EN  	   	: [0] (RW)LDO VDD28 can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 **************************   Fields Definition *******************************/
#define    LDO_DCXO_EXT_XTL_EN			(BIT_15)
#define    LDO_DCXO_XTL_BUF_EN			(BIT_14)
#define    LDO_CORE_EXT_XTL_EN			(BIT_13)
#define    LDO_CORE_XTL_BUF_EN			(BIT_12)
#define    LDO_SIM0_EXT_XTL_EN			(BIT_11)
#define    LDO_SIM0_XTL_BUF_EN			(BIT_10)
#define    LDO_SIM1_EXT_XTL_EN			(BIT_9)
#define    LDO_SIM1_XTL_BUF_EN			(BIT_8)
#define    LDO_CAMA_EXT_XTL_EN			(BIT_7)
#define    LDO_CAMA_XTL_BUF_EN			(BIT_6)
#define    LDO_CAMD_EXT_XTL_EN			(BIT_5)
#define    LDO_CAMD_XTL_BUF_EN			(BIT_4)
#define    LDO_VDD18_EXT_XTL_EN		(BIT_3)
#define    LDO_VDD18_XTL_BUF_EN		(BIT_2)
#define    LDO_VDD28_EXT_XTL_EN		(BIT_1)
#define    LDO_VDD28_XTL_BUF_EN		(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LDO_XTL_EN1
 ** Register Offset      : 0x00FC
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** LDO_RF_EXT_XTL_EN		: [15] (RW)LDO RF can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_RF_XTL_BUF_EN  	   	: [14] (RW)LDO RF can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_SF_EXT_XTL_EN		: [13] (RW)LDO SF can be cotrolled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_SF_XTL_BUF_EN  	   	: [12] (RW)LDO SF can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_VIBR_EXT_XTL_EN  	: [7] (RW)LDO VIBR can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_VIBR_XTL_BUF_EN  	: [6] (RW)LDO VIBR can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_USB_EXT_XTL_EN  	   	: [5] (RW)LDO USB can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_USB_XTL_BUF_EN		: [4] (RW)LDO USB can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_BTPA_EXT_XTL_EN		: [3] (RW)LDO BTPA can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_BTPA_XTL_BUF_EN		: [2] (RW)LDO BTPA can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 ** LDO_SD_EXT_XTL_EN		: [1] (RW)LDO SD can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** LDO_SD_XTL_BUF_EN  	   	: [0] (RW)LDO SD can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 **************************   Fields Definition *******************************/
#define    LDO_RF_EXT_XTL_EN			(BIT_15)
#define    LDO_RF_XTL_BUF_EN			(BIT_14)
#define    LDO_SF_EXT_XTL_EN			(BIT_13)
#define    LDO_SF_XTL_BUF_EN			(BIT_12)
#define    LDO_VIBR_EXT_XTL_EN			(BIT_7)
#define    LDO_VIBR_XTL_BUF_EN			(BIT_6)
#define    LDO_USB_EXT_XTL_EN			(BIT_5)
#define    LDO_USB_XTL_BUF_EN			(BIT_4)
#define    LDO_BTPA_EXT_XTL_EN			(BIT_3)
#define    LDO_BTPA_XTL_BUF_EN			(BIT_2)
#define    LDO_SD_EXT_XTL_EN			(BIT_1)
#define    LDO_SD_XTL_BUF_EN			(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_XO_BG_XTL_EN
 ** Register Offset      : 0x0100
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** DCXO_EXT_XTL_EN			: [15] (RW)DCXO can be controlled by EXT_XTL_EN(from PAD) if this bit is set
 ** DCXO_XTL_BUF_EN  	   	: [14] (RW)DCXO can be controlled by XTL_BUF_EN(from PAD) if this bit is set
 **************************   Fields Definition *******************************/
#define    DCXO_EXT_XTL_EN			(BIT_15)
#define    DCXO_XTL_BUF_EN			(BIT_14)

/*******************************************************************************
 ** Register Name        : ANA_CLK32KLESS_CTRL0
 ** Register Offset      : 0x011C
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** RC_MODE_WR_ACK_FLAG  	   	: [14] (RO)RC_MODE write ack flag
 ** XO_LOW_CUR_FLAG			: [13] (RO)DCXO sleep low current mode flag, active if DCXO falls into low current mode in chip sleep. 
 ** XO_LOW_CUR_FRC_RTCSET  	   	: [12] (RW)Force DCXO into low current mode RTC SET
 ** XO_LOW_CUR_FRC_RTCCLR  	   	: [11] (RW)Force DCXO into low current mode RTC CLR
 ** RC_MODE_WR_ACK_FLAG_CLR  	: [10] (WC)RC_MODE write ack flag clear, high effective
 ** XO_LOW_CUR_FLAG_CLR  	   	: [9] (WC)DCXO low current mode flag clear, high effective
 ** XO_LOW_CUR_CNT_CLR  	   	: [8] (WC)DCXO low current counter clear, high effective
 ** LDO_DCXO_LP_EN_RTCSET  		: [7] (RW)Low power LDO_DCXO power down set in RTC
 ** LDO_DCXO_LP_EN_RTCCLR  		: [6] (RW)Low power LDO_DCXO power down clear in RTC
 ** SLP_XO_LOW_CUR_EN  	   		: [5] (RW)DCXO low current mode enable in chip sleep mode
 ** XO_LOW_CUR_EN				: [3] (RW)Force DCXO into low current mode
 ** RC_32K_SEL					: [1] (RW)32K clock select in 32K crystal removal option,0: From XO,1: From RC
 ** RC_32K_EN  	   				: [0] (RW)RC 32K oscillator enable
 **************************   Fields Definition *******************************/
#define    RC_MODE_WR_ACK_FLAG			(BIT_14)
#define    XO_LOW_CUR_FLAG					(BIT_13)
#define    XO_LOW_CUR_FRC_RTCSET			(BIT_12)
#define    XO_LOW_CUR_FRC_RTCCLR			(BIT_11)
#define    RC_MODE_WR_ACK_FLAG_CLR		(BIT_10)
#define    XO_LOW_CUR_FLAG_CLR				(BIT_9)
#define    XO_LOW_CUR_CNT_CLR				(BIT_8)
#define    LDO_DCXO_LP_EN_RTCSET			(BIT_7)
#define    LDO_DCXO_LP_EN_RTCCLR			(BIT_6)
#define    SLP_XO_LOW_CUR_EN				(BIT_5)
#define    XO_LOW_CUR_EN					(BIT_3)
#define    RC_32K_SEL						(BIT_1)
#define    RC_32K_EN							(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_CLK32KLESS_CTRL1
 ** Register Offset      : 0x0120
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** RC_MODE		: [15:0] (RW)RC 32K mode in battery drop case:16'h95A5: RC oscillator stop working.Others: RC oscillator keep working.
 **************************   Fields Definition *******************************/
#define    RC_MODE_SHIFT            	(0)
#define    RC_MODE_MSK               	(0xFFFFUL << RC_MODE_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_CLK32KLESS_CTRL2
 ** Register Offset      : 0x0124
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** XO_LOW_CUR_CNT_LOW		: [15:0] (RW)Low 16bits of DCXO low current mode counter to indicate how long DCXO falls into this mode. The unit is 30.5us.
 **************************   Fields Definition *******************************/
#define    XO_LOW_CUR_CNT_LOW_SHIFT            	(0)
#define    XO_LOW_CUR_CNT_LOW_MSK               	(0xFFFFUL << XO_LOW_CUR_CNT_LOW_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_CLK32KLESS_CTRL3
 ** Register Offset      : 0x0128
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** XO_LOW_CUR_CNT_HIGH		: [15:0] (RW)High 16bits of DCXO low current mode counter to indicate how long DCXO falls into this mode. The unit is 30.5us.
 **************************   Fields Definition *******************************/
#define    XO_LOW_CUR_CNT_HIGH_SHIFT            	(0)
#define    XO_LOW_CUR_CNT_HIGH_MSK               	(0xFFFFUL << XO_LOW_CUR_CNT_HIGH_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_CLK32KLESS_CTRL4
 ** Register Offset      : 0x012C
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** RC1M_CAL		: [3:0] (RW)RC calibration bit
 **************************   Fields Definition *******************************/
#define    RC1M_CAL_SHIFT            	(0)
#define    RC1M_CAL_MSK               	(0xFUL << RC1M_CAL_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_AUXAD_CTL
 ** Register Offset      : 0x0130
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** AUXAD_RESERVED  	   		: [14:11] (RW)AUX_RESERVED[3]: reserved bit
									AUX_RESERVED[2]: auxadc current mode, current source selection, 0: current from current driver (default); 1: current from auxadc internal DAC
									AUX_RESERVED[1:0]: auxadc NTC buffer input mux
													00: channel0 NTC buffer +/- input open from VBAT_DET and SENSP (default)
													01: channel0 NTC buffer input+ connect to VBAT_DET, input- connect to SENSEP 
													10: channel 0 NTC buffer input+ connect to ADCI1 channel, input- connect to auxadc ground;
													11: channel 0 NTC buffer input+ connect to ADCI1 channel, input- connect to SENSEP
 ** AUXAD_REF_SEL  			: [7] (RW)ADC reference source selection, 
									0: from ADC inner reference gen by current from bandgap (default)
									1: from bandgap 1.2V voltage reference
 ** AUXAD_CURRENTSEN_EN  	: [6] (RW)Aux ADC current sense enable signal, active high
 **************************   Fields Definition *******************************/
#define    AUXAD_RESERVED_SHIFT            	(11)
#define    AUXAD_RESERVED_MSK               	(0xFUL << AUXAD_RESERVED_SHIFT)
#define    AUXAD_REF_SEL					(BIT_7)
#define    AUXAD_CURRENTSEN_EN			(BIT_6)

/*******************************************************************************
 ** Register Name        : ANA_XTL_WAIT_CTRL
 ** Register Offset      : 0x0134
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** AUDIO_FORCE_ON  		: [15] (RW)audio 26M path force on,1: force on,0:Not force on
 ** XTL_BUF_FOR_26M_EN  	: [11] (RW)26M wake up enable by ext_xtl1_en
 ** EXT_XTL_FOR_26M_EN  	: [10] (RW)26M wake up enable by ext_xtl0_en
 ** SLP_XTLBUF_PD_EN  	: [9] (RW)26MHz crystal oscillator power down enable in deep sleep mode
 ** XTL_EN  				: [8] (RW)26MHz crystal oscillator output enable
 ** XTL_WAIT  			: [7:0] (RW)26MHz crystal oscillator  wait cycles
 **************************   Fields Definition *******************************/
#define    AUDIO_FORCE_ON			(BIT_15)
#define    XTL_BUF_FOR_26M_EN		(BIT_11)
#define    EXT_XTL_FOR_26M_EN		(BIT_10)
#define    SLP_XTLBUF_PD_EN			(BIT_9)
#define    XTL_EN						(BIT_8)
#define    ANA_XTL_WAIT_SHIFT           (0)
#define    ANA_XTL_WAIT_MSK             	(0xFFUL << XTL_WAIT_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_IB_CTRL
 ** Register Offset      : 0x0138
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** IB_CUR_EN  		: [12] (RW)current mode enable,0: disable (default),1:enable (default)
 ** IB_CUR_I  		: [11:9] (RW)set current level in current mode,bit3~bit1  effective, bit0 not used,1.25/2.5/5/10/20/40/80/160uA 7step
 ** IB_CAL_EN  		: [8] (RW)IB trim test mode enable singnal
 ** IB_TRIM  			: [7:2] (RW)Internal resistor for sink current calibration bit.
 **************************   Fields Definition *******************************/
#define    IB_CUR_EN				(BIT_12)
#define    IB_CUR_I_SHIFT            	(9)
#define    IB_CUR_I_MSK               	(0x7UL << IB_CUR_I_SHIFT)
#define    IB_CAL_EN				(BIT_8)
#define    IB_TRIM_SHIFT            	(2)
#define    IB_TRIM_MSK               	(0x3FUL << IB_TRIM_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_WHTLED_CTRL
 ** Register Offset      : 0x013C
 ** Register Description : 
 ** Other Reset      : rst_d1
 ** Protect Register : 
 ** Register field description:
 ** WHTLED_PD_SEL  	: [7] (RW)White LED driver power down control selection,0: From Software Register,1: From Hareware Auto Control
 ** WHTLED_V  		: [4:1] (RW)White LED current control bit. 64 steps.Min current: 5mA ("000000")One step is 0.625mA
 ** WHTLED_PD  		: [0] (RW)White LED driver power down,"1"power down ,"0"enable
							PWM signal when WHTLED_V<5:0> = 6'b0
							Min current: 0.156mA
							One step is 0.156mA
 **************************   Fields Definition *******************************/
#define    WHTLED_PD_SEL			(BIT_7)
#define    WHTLED_V_SHIFT           (0)
#define    WHTLED_V_MSK             (0x3FUL << WHTLED_V_SHIFT)
#define    WHTLED_PD				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_KPLED_CTRL
 ** Register Offset      : 0x0140
 ** Register Description :
 ** Other Reset      : rst_d1
 ** Protect Register : 
 ** Register field description:
 ** KPLED_V  			: [15:12] (RW)KPLED driver current control bit. 16 steps
							Min current: 5mA ("0000")
							Max current: 80mA("1111")
 ** KPLED_PD  		: [11] (RW)KPLED driver power down
 **************************   Fields Definition *******************************/
#define    KPLED_V_SHIFT            	(12)
#define    KPLED_V_MSK               	(0xFUL << KPLED_V_SHIFT)
#define    KPLED_PD				        (BIT_11)

/*******************************************************************************
 ** Register Name        : ANA_VIBR_CTRL
 ** Register Offset      : 0x0144
 ** Register Description : 
 ** Other Reset      : rst_d1
 ** Protect Register : 
 ** Register field description:
 ** SLP_LDOVIBR_PD_EN  		: [9] (RW)Vibrator LDO sleep power down enable
 ** LDO_VIBR_PD  			: [8] (RW)VIBR LDO power down signal
 ** LDO_VIBR_V  				: [7:0] (RW)VIBR LDO program bits: 8'b0 as 1.2V output, 10mV/step,Default output 3.0V
 **************************   Fields Definition *******************************/
#define    SLP_LDOVIBR_PD_EN		(BIT_9)
#define    LDO_VIBR_PD				(BIT_8)
#define    LDO_VIBR_V_SHIFT            	(0)
#define    LDO_VIBR_V_MSK               	(0xFFUL << LDO_VIBR_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_AUDIO_CTRL0
 ** Register Offset      : 0x0148
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** AUD_SLP_APP_RST_EN  			: [15] (RW)audio sleep reset eanble . 0: chip_deep_slp can not effect the audio,1: chip_deep_slp can reset clk_aud_hbd and clk_aud_hid domain
 ** CLK_AUD_HBD_DIV  			: [12:8] (RW)clk_aud_hbd divider
 ** CLK_AUD_LOOP_INV_EN  		: [4] (RW)Whether Adie audio controller use inverse of clk_aud_loop,0: No,1: Yes
 ** CLK_AUD_IF_TX_INV_EN  		: [3] (RW)Whether Adie use inverse of clk_audif to sample Ddie tx data,0: No,1: Yes
 ** CLK_AUD_IF_RX_INV_EN  		: [2] (RW)Whether Adie use inverse of clk_aud6m5 to send rx data to Ddie,0: No,1: Yes
 ** CLK_AUD_IF_6P5M_TX_INV_EN  	: [1] (RW)Whether Adie audio controller use inverse of clk_aud6m5 in tx path,0: No,1: Yes
 ** CLK_AUD_IF_6P5M_RX_INV_EN  	: [0] (RW)Whether Adie audio controller use inverse of clk_aud6m5 in rx path,0: No,1: Yes
 **************************   Fields Definition *******************************/
#define    CLK_AUD_HBD_DIV_SHIFT            	(8)
#define    CLK_AUD_HBD_DIV_MSK              	 	(0x1FUL << CLK_AUD_HBD_DIV_SHIFT)
#define    CLK_AUD_LOOP_INV_EN				(BIT_4)
#define    CLK_AUD_IF_TX_INV_EN				(BIT_3)
#define    CLK_AUD_IF_RX_INV_EN				(BIT_2)
#define    CLK_AUD_IF_6P5M_TX_INV_EN		(BIT_1)
#define    CLK_AUD_IF_6P5M_RX_INV_EN		(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_AUDIO_CTRL1
 ** Register Offset      : 0x014C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** HEAD_INSERT_EIC_EN  		: [6] (RW)EIC[5] input select,0: AUDIO_HEAD_INSERT2,1: AUDIO_HEAD_INSERT
 ** AUDIO_CHP_CLK_DIV_EN  	: [5] (RW)AUDIO_CHP_CLK divide enable
 ** AUDIO_CHP_CLK_DIV  		: [4:0] (RW)AUDIO_CHP_CLK divide parameter
 **************************   Fields Definition *******************************/
#define    HEAD_INSERT_EIC_EN				(BIT_6)
#define    AUDIO_CHP_CLK_DIV_EN			(BIT_5)
#define    AUDIO_CHP_CLK_DIV_SHIFT            	(0)
#define    AUDIO_CHP_CLK_DIV_MSK              	(0x1FUL << AUDIO_CHP_CLK_DIV_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_CHGR_CTL0
 ** Register Offset      : 0x0150
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** CHGR_CV_V  		: [11:6] (RW)Charger battery sense DAC (CC-CV trans-point control)
 ** CHGR_END_V  		: [5:4] (RW)Charger battery charging end voltage
        						00: Vend=4.2V
        						01: Vend=4.3V
        						10: Vend=4.4V
        						11: Vend=4.5V
 ** CHGR_PD  		: [0] (RW)Charger power down
 **************************   Fields Definition *******************************/
#define    CHGR_SW_POINT_SHIFT            	(6)
#define    CHGR_SW_POINT_MSK              	(0x3FUL << CHGR_SW_POINT_SHIFT)
#define    CHGR_END_V_SHIFT            	(4)
#define    CHGR_END_V_MSK              		(0x3UL << CHGR_END_V_SHIFT)
#define    CHGR_PD					(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_CHGR_CTL1
 ** Register Offset      : 0x0154
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** CHGR_CC_I  		: [14:10] (RW)CC mode charging current
								00000:300mA        00001 : 350
								00010: 400mA       00011 : 450
								00100: 500mA       00101 :550
								00110: 600mA       00111: 650
								01000: 700mA       01001: 750
								01010: 800mA       01011: 850
								01100: 900mA       01101: 950
								01110: 1000mA     01111: 1050
								10000: 1100mA     10001: 1150
								10010: 1200mA     10011: 1250
								10100: 1300mA     10101: 1350
								10110: 1400mA     10111: 1500
								11000: 1600mA     11001: 1700
								11010: 1800mA     11011: 1900
								11100: 2000mA     11101: 2100
								11110: 2200mA     11111: 2300
								Default5’b0
 ** VBAT_OVP_V  		: [9:6] (RW)control bits of over voltage protection for VBAT. When VBAT is above some level set by these 4 bits, charger power down and VBAT_OVI becomes high.
							        0000: 4.25V(protection voltage)
							        0001: 4.275V
							        0010: 4.3V
							        0011: 4.325V
							        0100: 4.35V
							        0101: 4.375V
							        0110: 4.4V
							        0111: 4.425V
							        1000:  4.45V
							        1001: 4.475V
							        1010: 4.5V
							        1011: 4.525V
							        1100: 4.55V
							        1101: 4.575V
							        1110: 4.6V
							        1111: 4.625V
								Default4'b0
 ** VCHG_OVP_V  		: [5:0] (RW)control bits of over voltage protection for VCHG. When VCHG is above some level set by these 6 bits, charger power down and CHGR_OVI becomes high.
							      000000: 5.6V(protection voltage)   000001: 5.7V
							      000010: 5.8V                                  000011: 5.9V
							      000100: 6.0V                                  000101: 6.1V
							      000110: 6.2V                                  000111: 6.3V
							      001000: 6.4V                                  001001: 6.5V
							      001010: 6.6V                                  001011: 6.7V
							      001100: 6.8V                                  001101: 6.9V
							      001110: 7.0V                                  001111: 7.1V
							      010000: 7.2V                                  010001: 7.3V
							      010010: 7.4V                                  010011: 7.5V
							      010100: 7.6V                                  010101: 7.7V
							      010110: 7.8V                                  010111: 7.9V
							      011000: 8.0V                                  011001: 8.1V
							      011010: 8.2V                                  011011: 8.3V
							      011100: 8.4V                                  011101: 8.5V
							      011110: 8.6V                                  011111: 8.7V
							      100000: 8.8V                                  100001: 8.9V
							      100010: 9.0V                                  100011: 9.1V
							      100100: 9.2V                                  100101: 9.3V
							      100110: 9.4V                                  100111: 9.5V
							      101000: 9.6V                                  101001: 9.7V
							      101010: 9.7V
							      ......
							      111111: 9.7V
								Default 6'001110
 **************************   Fields Definition *******************************/
#define    CHGR_CHG_CUR_SHIFT            	(10)
#define    CHGR_CHG_CUR_MSK              	(0x1FUL << CHGR_CHG_CUR_SHIFT)
#define    VBAT_OVP_V_SHIFT            		(6)
#define    VBAT_OVP_V_MSK              		(0xFUL << VBAT_OVP_V_SHIFT)
#define    VCHG_OVP_V_SHIFT               	(0)
#define    VCHG_OVP_V_MSK              		(0x3FUL << VCHG_OVP_V_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_CHGR_CTL2
 ** Register Offset      : 0x0158
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** CHGR_DRV  		: [7] (RW)CHGR_DRV
 ** CHGR_OSC  		: [6] (RW)CHGR_OSC
 ** CHGR_DPM  		: [5:4] (RW)CHGR_DPM
 ** CHGR_ITERM  		: [3:2] (RW)CHGR_ITERM
 									Termination charger current programmable bits
									00:cc*0.9
									01:cc*0.4
									10:cc*0.2
									11:cc*0.1
 ** CHGR_CC_EN  		: [1] (RW)Charger CC mode enable, high effective
 ** RECHG  			: [0] (RW)Charging software start up flag, high effective
 **************************   Fields Definition *******************************/
#define    CHGR_DRV					(BIT_7)
#define    CHGR_OSC					(BIT_6)
#define    CHGR_DPM_SHIFT            	(4)
#define    CHGR_DPM_MSK              	(0x3UL << CHGR_DPM_SHIFT)
#define    CHGR_ITERM_SHIFT            	(2)
#define    CHGR_ITERM_MSK              	(0x3UL << CHGR_ITERM_SHIFT)
#define    CHGR_CC_EN				(BIT_1)
#define    CHGR_RECHG				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_CHGR_STATUS
 ** Register Offset      : 0x015C
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** CHGR_CV_STATUS  	: [4] (RO)Flag when charging current below some level(0.5*full current) in CV mode High effective
 ** CHGR_ON  		: [3] (RO)Charger voltage ready indicator, high effective:When VCHG<4.1V: "0",When VCHG>4.3V: "1"
 ** CHGR_INT  		: [2] (RO)Charger present indicator, high effective:When VCHG<3.1V: "0",When VCHG>3.3V: "1"
 ** VBAT_OVI  		: [1] (RO)VBAT over voltage(programmable) flag:When VBAT higher than some voltage set by VBAT_OVP_V<3:0> and lasts 2mS, VBAT_OVI="1",The hysteresis voltage is 25mV.
 ** VCHG_OVI  		: [0] (RO)VCHG over voltage(programmable) flag:When VCHG higher than some voltage set by VCHG_OVP_V<5:0> and lasts 2mS, CHGR_OVI=1,The hysteresis voltage is 600mV.
 **************************   Fields Definition *******************************/
#define    CHGR_CV_STATUS		(BIT_4)
#define    CHGR_ON				(BIT_3)
#define    CHGR_INT				(BIT_2)
#define    VBAT_OVI				(BIT_1)
#define    VCHG_OVI				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_MIXED_CTRL0
 ** Register Offset      : 0x0160
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** PTEST_PD_RTCSET  		: [15] (RW)PTEST PD mode set
 ** OPT_AUTO_PWON_O  	: [12] (RO)Power on mode,0:no self test when vbat ready(default),1:self test when vbat ready
 ** VBAT_CRASH_V  		: [11:10] (RW)Battery crash voltage level setting:
								00: 1.9V (default)
								01: 2.2V
								10: 2.3V
								11: 2.5V
 ** OVLO_EN  			: [9] (RW)Over voltage locked-out enable (high effective),Default"1"
 ** OVLO_V  				: [3:2] (RW)Over voltage locked-out detecting time 
								00 : 4.8V (default)
								01 : 5.0V 
								10 : 4.6V
								11 : 4.2V
 ** OVLO_T  				: [1:0] (RW)Over voltage locked-out detecting time 
								00 : 1ms (default)
								01 : 0.5ms
								10 : 0.25ms
								11 : 2ms
 **************************   Fields Definition *******************************/
#define    PTEST_PD_RTCSET			(BIT_15)
#define    OPT_AUTO_PWON_O		(BIT_12)
#define    VBAT_CRASH_V_SHIFT           (10)
#define    VBAT_CRASH_V_MSK              (0x3UL << VBAT_CRASH_V_SHIFT)
#define    OVLO_EN					(BIT_9)
#define    OVLO_V_SHIFT            		(2)
#define    OVLO_V_MSK              		(0x3UL << OVLO_V_SHIFT)
#define    OVLO_T_SHIFT            		(0)
#define    OVLO_T_MSK              		(0x3UL << OVLO_T_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_MIXED_CTRL1
 ** Register Offset      : 0x0164
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** BATON_T  		: [11:10] (RW)Control bit of de-glitch time for battery remove:"00" 32us,"01" 64us,"10" 128us,"11" no de-glitch,default"00"
 ** BATDET_OK  		: [9] (RO)Battery presence flag to SW and POCV, so need RTC domain,"0" no battery,"1" battery presence
 ** ALL_GPI_DEB  		: [3] (RW)ALL GPI source debug 
 ** GPI_DEBUG_EN  	: [2] (RW)GPI debug enable
 ** ALL_INT_DEB  		: [1] (RW)ALL_INT debug, if 1, interrupt will be sent
 ** INT_DEBUG_EN  	: [0] (RW)Interupt debug enable
 **************************   Fields Definition *******************************/
#define    BATON_T_SHIFT           		(10)
#define    BATON_T_MSK              		(0x3UL << BATON_T_SHIFT)
#define    OVLO_EN					(BIT_9)
#define    ALL_GPI_DEB				(BIT_3)
#define    GPI_DEBUG_EN				(BIT_2)
#define    ALL_INT_DEB				(BIT_1)
#define    INT_DEBUG_EN				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_POR_RST_MONITOR
 ** Register Offset      : 0x0168
 ** Register Description :
 ** Other Reset      : por_rst_n
 ** Protect Register : 
 ** Register field description:
 ** POR_RST_MONITOR  		: [15:0] (RW)When POR reset active, this register is reset to 0
 **************************   Fields Definition *******************************/
#define    POR_RST_MONITOR_SHIFT           		(0)
#define    POR_RST_MONITOR_MSK              		(0xFFFFUL << POR_RST_MONITOR_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_WDG_RST_MONITOR
 ** Register Offset      : 0x016C
 ** Register Description : 
 ** Other Reset      : wdg_rst_in_n
 ** Protect Register : 
 ** Register field description:
 ** WDG_RST_MONITOR  		: [15:0] (RW)When WDG reset active, this register is reset to 0
 **************************   Fields Definition *******************************/
#define    WDG_RST_MONITOR_SHIFT           		(0)
#define    WDG_RST_MONITOR_MSK              		(0xFFFFUL << WDG_RST_MONITOR_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_POR_PIN_RST_MONITOR
 ** Register Offset      : 0x0170
 ** Register Description : 
 ** Other Reset      : rst_d1
 ** Protect Register : 
 ** Register field description:
 ** POR_PIN_RST_MONITOR  		: [15:0] (RW)When POR_EXT_RST active, this register is reset to 0
 **************************   Fields Definition *******************************/
#define    POR_PIN_RST_MONITOR_SHIFT           		(0)
#define    POR_PIN_RST_MONITOR_MSK              		(0xFFFFUL << POR_PIN_RST_MONITOR_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_POR_SRC_FLAG
 ** Register Offset      : 0x0174
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** POR_SW_FORCE_ON  		: [15] (RW)Setting this bit could disable the 1S debouncing time of power key after boot.
 ** POR_SRC_FLAG  			: [14:0] (RO)Power on source flag:
								[0]: Debounced PBINT signal, set when PBINT=0 >50ms, clear when PBINT=1>50ms.
								[1]: PBINT initiating power-up hardware flag, set when PBINT=0>1s, clear after power down.
								[2]: Debounced PBINT2 signal, set when PBINT2=0 >50ms, clear when PBINT2=1>50ms.
								[3]: PBINT2 initiating power-up hardware flag, set when PBINT2=0>1s, clear after power down.
								[4]: Debounced CHGR_INT signal, set when VCHG=1 >50ms, clear when VCHG=0>50ms.
								[5]: Charger plug-in initiating power-up hardware flag, set when VCHG=1>1s, clear after power down.
								[6]: RTC alarm initiating power-up hardware flag
								[7]: Long pressing power key reboot hardware flag, set when PBINT=0>PBINT_7S_THRESHOLD, clear after power down.
								[8]: PBINT initiating power-up software flag, set when PBINT=0>1s, clear by pbint_flag_clr.
								[9]: PBINT2 initiating power-up software flag, set when PBINT2=0>1s, clear by pbint2_flag_clr.
								[10]: Charger plug-in initiating power-up software flag, set when VCHG=1>1s, clear by chgr_int_flag_clr.
								[11]: External pin reset reboot software flag, set when EXTRSTN=0>30ms, clear by ext_rstn_flag_clr.
								[12]: Long pressing power key reboot software flag, set when PBINT=0>PBINT_7S_THRESHOLD, clear by pbint_7s_flag.
					        		[14:13]: Reserved
 **************************   Fields Definition *******************************/
#define    POR_SW_FORCE_ON				(BIT_15)
#define    POR_PBINT_7S_SW_FLAG         (BIT_12)
#define    POR_EXT_RST_FLAG				(BIT_11)
#define    POR_PBINT_7S_HW_FLAG         (BIT_7)
#define    POR_SRC_FLAG_SHIFT           	(0)
#define    POR_SRC_FLAG_MSK              	(0x7FFFUL << POR_SRC_FLAG_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_POR_7S_CTRL
 ** Register Offset      : 0x0178
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** PBINT_7S_FLAG_CLR  	   		: [15] (RW)Write 1'b1 to this bit will clear pbint_7s_flag.
 ** EXT_RSTN_FLAG_CLR			: [14] (RW)Write 1'b1 to this bit will clear ext_rstn_flag.
 ** CHGR_INT_FLAG_CLR  	   		: [13] (RW)Write 1'b1 to this bit will clear chgr_int_flag.
 ** PBINT2_FLAG_CLR  	   			: [12] (RW)Write 1'b1 to this bit will clear pbint2_flag.
 ** PBINT_FLAG_CLR  				: [11] (RW)Write 1'b1 to this bit will clear pbint_flag.
 ** KEY2_7S_RST_EN  	   			: [9] (RW)1: One-key Reset Mode;0: Two-key Reset Mode;
 ** PBINT_7S_RST_SWMODE  	   	: [8] (RW)0: long reset;1: short reset;
 ** PBINT_7S_RST_THRESHOLD  		: [7:4] (RW)The power key long pressing time threshold:
										0~1: 2S
										2: 3S
										3: 4S
										4: 5S
										5: 6S
										6: 7S
										7: 8S
										8: 9S
										9: 10S
										10:11S
										11:12S
										12: 13S
										13:14S
										14:15S
										15:16S
 ** EXT_RSTN_MODE  				: [3] (RW)EXT_RSTN PIN function mode when 1key 7S reset,0: EXT_INT,1: RESET
 ** PBINT_7S_AUTO_ON_EN  	   	: [2] (RW)RTC register PBINT_7S_AUTO_ON_EN 
 ** PBINT_7S_RST_DISABLE			: [1] (RW)0: enable 7s reset function;1: disable 7s reset function;
 ** PBINT_7S_RST_MODE  	   		: [0] (RW)0: software reset;1: hardware reset;
 **************************   Fields Definition *******************************/
#define    PBINT_7S_FLAG_CLR					(BIT_15)
#define    EXT_RSTN_FLAG_CLR					(BIT_14)
#define    CHGR_INT_FLAG_CLR					(BIT_13)
#define    PBINT2_FLAG_CLR						(BIT_12)
#define    PBINT_FLAG_CLR						(BIT_11)
#define    KEY2_7S_RST_EN						(BIT_9)
#define    PBINT_7S_RST_SWMODE					(BIT_8)
#define    PBINT_7S_RST_THRESHOLD_SHIFT           	(4)
#define    PBINT_7S_RST_THRESHOLD_MSK              	(0xFUL << PBINT_7S_RST_THRESHOLD_SHIFT)
#define    EXT_RSTN_MODE						(BIT_3)
#define    PBINT_7S_AUTO_ON_EN					(BIT_2)
#define    PBINT_7S_RST_DISABLE					(BIT_1)
#define    PBINT_7S_RST_MODE					(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_HWRST_RTC
 ** Register Offset      : 0x017C
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** HWRST_RTC_REG_STS  	   		: (RO)[15:8] RTC status register, set by HWRST_RTC_SET.
 ** HWRST_RTC_REG_SET			: (RW)[7:0] Software set this register to test VBAT and RTC power status.
 **************************   Fields Definition *******************************/
#define    HWRST_RTC_REG_STS_SHIFT           	(8)
#define    HWRST_RTC_REG_STS_MSK              	(0xFFUL << HWRST_RTC_REG_STS_SHIFT)
#define    HWRST_RTC_REG_SET_SHIFT           	(0)
#define    HWRST_RTC_REG_SET_MSK              	(0xFFUL << HWRST_RTC_REG_SET_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_ARCH_EN
 ** Register Offset      : 0x0180
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : MCU_WR_PROT_VALUE.MCU_WR_PROT
 ** Register field description:
 ** ARCH_EN  	   		: [0] (RW)PCLK_arch enable
 **************************   Fields Definition *******************************/
#define    ARCH_EN					(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_MCU_WR_PROT_VALUE
 ** Register Offset      : 0x0184
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** MCU_WR_PROT  	   		: [15] (RO)Arch_en write protect bit status.When mcu_wr_prot_value==16'h3c4d,the bit is "1",else "0"
 ** MCU_WR_PROT_VALUE  	   	: [14:0] (WO)Arch_en write protect value
 **************************   Fields Definition *******************************/
#define    MCU_WR_PROT						(BIT_15)
#define    MCU_WR_PROT_VALUE_SHIFT           	(0)
#define    MCU_WR_PROT_VALUE_MSK              	(0x7FFFUL << MCU_WR_PROT_VALUE_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_PWR_WR_PROT_VALUE
 ** Register Offset      : 0x0188
 ** Register Description :
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** PWR_WR_PROT  	   		: [15] (RO)All power which default on write protect bit status.When mcu_wr_prot_value==16'h6e7f,the bit is "1",else "0"
 ** PWR_WR_PROT_VALUE  	   	: [14:0] (WO)Arch_en write protect value
 **************************   Fields Definition *******************************/
#define    PWR_WR_PROT						(BIT_15)
#define    PWR_WR_PROT_VALUE_SHIFT           	(0)
#define    PWR_WR_PROT_VALUE_MSK              	(0x7FFFUL << PWR_WR_PROT_VALUE_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_SMPL_CTRL0
 ** Register Offset      : 0x018C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** SMPL_MODE  	   	: [15:0] (RW)SMPL mode:
							[15:13]: SMPL timer threshold
						                0: 0.25s
						                1: 0.5s
						                2: 0.75s
						                 ......
						                 7: 2s
							[12:0]: SMPL enable
						               13'h1935: enable
						                Others:     disable
 **************************   Fields Definition *******************************/
#define    SMPL_MODE_SHIFT           	(0)
#define    SMPL_MODE_MSK              	(0xFFFFUL << SMPL_MODE_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_SMPL_CTRL1
 ** Register Offset      : 0x0190
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** SMPL_PWR_ON_FLAG  	   			: [15] (RO)Set once SMPL timer not expired. 
 ** SMPL_MODE_WR_ACK_FLAG  	   		: [14] (RO)Set once SMPL mode write finish 
 ** SMPL_PWR_ON_FLAG_CLR  	   		: [13] (WC)Clear SMPL_PWR_ON_FLAG
 ** SMPL_MODE_WR_ACK_FLAG_CLR  	   	: [12] (WC)Clear SMPL_MODE_WR_ACK
 ** SMPL_PWR_ON_SET  	   			: [11] (RO)Set once SMPL timer not expired, 
 ** SMPL_EN  	   						: [0] (RO)SMPL enable indication
 **************************   Fields Definition *******************************/
#define    SMPL_PWR_ON_FLAG					(BIT_15)
#define    SMPL_MODE_WR_ACK_FLAG				(BIT_14)
#define    SMPL_PWR_ON_FLAG_CLR				(BIT_13)
#define    SMPL_MODE_WR_ACK_FLAG_CLR			(BIT_12)
#define    SMPL_PWR_ON_SET						(BIT_11)
#define    SMPL_EN								(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_RTC_RST0
 ** Register Offset      : 0x0194
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** RTC_CLK_FLAG_SET  	   			: [15:0] (RW)RTC register flag
 **************************   Fields Definition *******************************/
#define    RTC_CLK_FLAG_SET_SHIFT           	(0)
#define    RTC_CLK_FLAG_SET_MSK              	(0xFFFFUL << RTC_CLK_FLAG_SET_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_RTC_RST1
 ** Register Offset      : 0x0198
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** RTC_CLK_FLAG_CLR  	   			: [15:0] (RW)RTC register flag
 **************************   Fields Definition *******************************/
#define    RTC_CLK_FLAG_CLR_SHIFT           	(0)
#define    RTC_CLK_FLAG_CLR_MSK              	(0xFFFFUL << RTC_CLK_FLAG_CLR_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_RTC_RST2
 ** Register Offset      : 0x019C
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** RTC_CLK_FLAG_RTC  	   			: [15:0] (RO)RTC register flag, reset by RTC_RST, default is 16'hA596
 **************************   Fields Definition *******************************/
#define    RTC_CLK_FLAG_RTC_SHIFT           	(0)
#define    RTC_CLK_FLAG_RTC_MSK              	(0xFFFFUL << RTC_CLK_FLAG_RTC_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_RTC_CLK_STOP
 ** Register Offset      : 0x01A0
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** RTC_CLK_STOP_FLAG  	   			: [7] (RO)rtc time over thresthold value
 ** RTC_CLK_STOP_THRESHOLD  	   		: [6:0] (RW)set reset rtc cnt time,default 16s
 **************************   Fields Definition *******************************/
#define    RTC_CLK_STOP_FLAG						(BIT_7)
#define    RTC_CLK_STOP_THRESHOLD_SHIFT           	(0)
#define    RTC_CLK_STOP_THRESHOLD_MSK              	(0x7FUL << RTC_CLK_STOP_THRESHOLD_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_VBAT_DROP_CNT
 ** Register Offset      : 0x01A4
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** VBAT_DROP_CNT  	   		: [11:0] (RO)VBAT Drop Time Count
 **************************   Fields Definition *******************************/
#define    VBAT_DROP_CNT_SHIFT           	(0)
#define    VBAT_DROP_CNT_MSK              	(0xFFFUL << VBAT_DROP_CNT_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_SWRST_CTRL0
 ** Register Offset      : 0x01A8
 ** Register Description :
 ** Other Reset      : por_rst_n
 ** Protect Register : 
 ** Register field description:
 ** EXT_RSTN_PD_EN  	   		: [10] (RW)Software reset certain power enable when ext_rstn valid
 ** PB_7S_RST_PD_EN  	   	: [9] (RW)Software reset certain power enable when pb_7s_rst valid
 ** WDG_RST_PD_EN  	   		: [6] (RW)Software reset certain power enable when wdg_rst valid
 ** SW_RST_PD_THRESHOLD  	: [3:0] (RW)reset LDO to normal mode threshold time 8ms/step,default 8ms
 **************************   Fields Definition *******************************/
 #define    EXT_RSTN_PD_EN				(BIT_10)
 #define    PB_7S_RST_PD_EN				(BIT_9)
 #define    WDG_RST_PD_EN				(BIT_6)
 #define    SW_RST_PD_THRESHOLD_SHIFT           	(0)
 #define    SW_RST_PD_THRESHOLD_MSK              	(0xFUL << VBAT_DROP_CNT_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_SWRST_CTRL1
 ** Register Offset      : 0x01AC
 ** Register Description : 
 ** Other Reset      : por_rst_n
 ** Protect Register : 
 ** Register field description:
 ** SW_RST_USB_PD_EN  	   	: [3] (RW)Software reset LDO_USB_PD enable when global reset valid
 ** SW_RST_SF_PD_EN  	   	: [2] (RW)Software reset LDO_SF_PD enable when global reset valid
 ** SW_RST_RF_PD_EN  	   	: [1] (RW)Software reset LDO_RF_PD enable when global reset valid
 ** SW_RST_VDD28_PD_EN  	: [0] (RW)Software reset LDO_VDD28_PD enable when global reset valid
 **************************   Fields Definition *******************************/
 #define    SW_RST_USB_PD_EN				(BIT_3)
 #define    SW_RST_SF_PD_EN					(BIT_2)
 #define    SW_RST_RF_PD_EN					(BIT_1)
 #define    SW_RST_VDD28_PD_EN				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_OTP_CTRL
 ** Register Offset      : 0x01B0
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** OTPINT_OUT  	   	: [6] (RO)OTP int flag
 ** OTP_OP  	   		: [2:1] (RW)OTP threshold option, 00 120, 01 130,  10 140, 11 150;
 ** OTP_EN  			: [0] (RW)OTP function enable control bit
 **************************   Fields Definition *******************************/
#define    OTPINT_OUT			(BIT_6)
#define    OTP_OP_SHIFT           	(1)
#define    OTP_OP_MSK              	(0x3UL << OTP_OP_SHIFT)
#define    OTP_EN				(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LOW_PWR_CLK32K_CTRL
 ** Register Offset      : 0x01BC
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** DCXO_EXTRA_LOW_CUR_MODE_EN  	: [15] (RW)extra low current mode DCXO for auto calibration,0:  disable,1:  enable
 ** DCXO_LOW_CUR_MODE_EN  	   		: [14] (RW)Low current mode DCXO enable for auto calibration,0: use normal mode DCXO for calibration,1:use low current mode DCXO for calibration
 ** LDO_DCXO_LP_AUTO_EN  			: [13] (RW)LDO DCXO low power enable for auto calibration,0: normal mode when auto calibration,1: low power mode when auto calibration
 ** TIME_BETWEEN_CALIBRATION 		: [12:9] (RW)0000: 20S
											0001: 21S
											0010: 22S
											0011: 23S
											0100: 24S
											0101: 25S
											0110: 26S
											0111: 27S
											1000: 28S
											1001: 29S
											1010: 210S
											1011: 211S
											1100: 212S
											1101: 213S
											1110: 214S
											1111: 215S
 ** TIME_FOR_CALIBRATION  			: [8:6] (RW)000: 30ms
											001: 60ms
											010: 120ms
											011: 240ms
											100: 480ms
											101: 960ms
											110: 2s
											111: 4s
 ** TIME_FOR_DCXO_STABLE  			: [5:2] (RW)0000: 2ms
		 									0001: 4ms
		 									0010: 6ms
		 									0011: 8ms
		 									0100: 10ms
		 									0101: 12ms
		 									0110: 14ms
		 									0111: 16ms
		 									1000: 18ms
		 									1001: 20ms
		 									1010: 22ms
		 									1011: 24ms
		 									1100: 26ms
		 									1101: 28ms
		 									1110: 30ms
		 									1111: 32ms
 ** LOW_PWR_CLK32K_EN_DBG  		: [1] (RW)Enable low power of clock 32k for debug,0: disable,1: enable
 ** LOW_PWR_CLK32K_EN  				: [0] (RW)Enable low power of clock 32k,0: disable,1: enable
 **************************   Fields Definition *******************************/
#define    DCXO_EXTRA_LOW_CUR_MODE_EN			(BIT_15)
#define    DCXO_LOW_CUR_MODE_EN					(BIT_14)
#define    LDO_DCXO_LP_AUTO_EN						(BIT_13)
#define    TIME_BETWEEN_CALIBRATION_SHIFT           	(9)
#define    TIME_BETWEEN_CALIBRATION_MSK              	(0xFUL << TIME_BETWEEN_CALIBRATION_SHIFT)
#define    TIME_FOR_CALIBRATION_SHIFT           		(6)
#define    TIME_FOR_CALIBRATION_MSK              		(0x7UL << TIME_FOR_CALIBRATION_SHIFT)
#define    TIME_FOR_DCXO_STABLE_SHIFT           		(2)
#define    TIME_FOR_DCXO_STABLE_MSK              		(0xFUL << TIME_FOR_DCXO_STABLE_SHIFT)
#define    LOW_PWR_CLK32K_EN_DBG					(BIT_1)
#define    LOW_PWR_CLK32K_EN						(BIT_0)

/*******************************************************************************
 ** Register Name        : ANA_LOW_PWR_CLK32K_TEST0
 ** Register Offset      : 0x01C0
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** cal_result_1  			: [15:0] (RO)register for test
 **************************   Fields Definition *******************************/
#define    CAL_RESULT_1_SHIFT		(0)
#define    CAL_RESULT_1_MSK              	(0xFFFFUL << CAL_RESULT_0_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LOW_PWR_CLK32K_TEST1
 ** Register Offset      : 0x01C0
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** cal_result_0  			: [15:0] (RO)register for test
 **************************   Fields Definition *******************************/
#define    CAL_RESULT_0_SHIFT			(0)
#define    CAL_RESULT_0_MSK              	(0xFFFFUL << CAL_RESULT_0_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LOW_PWR_CLK32K_TEST2
 ** Register Offset      : 0x01C0
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** div_fac_frac  			: [15:0] (RO)register for test
 **************************   Fields Definition *******************************/
#define    DIV_FAC_FRAC_SHIFT			(0)
#define    DIV_FAC_FRAC_MSK              	(0xFFFFUL << DIV_FAC_FRAC_SHIFT)

/*******************************************************************************
 ** Register Name        : ANA_LOW_PWR_CLK32K_TEST3
 ** Register Offset      : 0x01C0
 ** Register Description : 
 ** Other Reset      : 
 ** Protect Register : 
 ** Register field description:
 ** div_fac_int  			: [3:0] (RO)register for test
 **************************   Fields Definition *******************************/
#define    DIV_FAC_INT_SHIFT			(0)
#define    DIV_FAC_INT_MSK              	(0xFUL << DIV_FAC_INT_SHIFT)

/*******************************************************************************
 **                      Compiler Flag                                         *
 *******************************************************************************/
#ifdef __cplusplus
}
#endif


#endif //TO_BE_REPLACED
