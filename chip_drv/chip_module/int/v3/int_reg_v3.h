/******************************************************************************
 ** File Name:    int_reg_v3.h                                           *
 ** Author:       Yuhua.Shi                                                   *
 ** DATE:         6/14/2008                                                   *
 ** Copyright:    2008 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/14/2008    Yuhua.Shi       Create.                                     *
 ** 05/05/2010    Mingwei.Zhang   Modified for SC8800G                        *
 ******************************************************************************/
#ifndef _INT_REG_V3_H_
#define _INT_REG_V3_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
/*----------Interrupt Control Registers----------*/

#define INT_IRQ_BASE                    (INT_REG_BASE + 0x0000)
#define INT_IRQ_STS                     (INT_REG_BASE + 0x0000) //Interrupt status after masked by irq_enable.
#define INT_IRQ_RAW_STS                 (INT_REG_BASE + 0x0004) //Interrupt status from different interrupt source.
#define INT_IRQ_EN                      (INT_REG_BASE + 0x0008) //Enable bits  for the corresponding interrupt sources. 
#define INT_IRQ_DISABLE                 (INT_REG_BASE + 0x000C)
#define INT_IRQ_SOFT                    (INT_REG_BASE + 0x0010)
#define INT_IRQ_TEST_SRC                (INT_REG_BASE + 0x0014)
#define INT_IRQ_TEST_SEL                (INT_REG_BASE + 0x0018)
#define INT_IRQ_UINT_STS                (INT_REG_BASE + 0x001C)
#define INT_FIQ_STS                     (INT_REG_BASE + 0x0020)
#define INT_FIQ_RAW_STS                 (INT_REG_BASE + 0x0024)
#define INT_FIQ_EN                      (INT_REG_BASE + 0x0028)
#define INT_FIQ_DISABLE                 (INT_REG_BASE + 0x002C)
#define INT_FIQ_SOFT                    (INT_REG_BASE + 0x0030)
#define INT_FIQ_TEST_SRC                (INT_REG_BASE + 0x0034)
#define INT_FIQ_TEST_SEL                (INT_REG_BASE + 0x0038)
#define INT_UINT_CTL                    (INT_REG_BASE + 0x003C)


#define INTCTL_SPECIAL_LATCH_IRQ            BIT_0
#define INTCTL_SOFT_IRQ                     BIT_1
#define INTCTL_UART0_IRQ                    BIT_2
#define INTCTL_UART1_IRQ                    BIT_3
#define INTCTL_UART2_IRQ                    BIT_4
#define INTCTL_TIMER0_IRQ                   BIT_5
#define INTCTL_TIMER1_IRQ                   BIT_6
#define INTCTL_TIMER2_IRQ                   BIT_7
#define INTCTL_COMMTX                       BIT_7
#define INTCTL_GPIO_IRQ                     BIT_8
#define INTCTL_SPI_IRQ                      BIT_9
#define INTCTL_KPD_IRQ                      BIT_10
#define INTCTL_I2C_IRQ                      BIT_11
#define INTCTL_SIM0_IRQ                     BIT_12
#define INTCTL_SIM1_IRQ                     BIT_12
#define INTCTL_PIU_SER_INT_IRQ              BIT_13
#define INTCTL_PIU_CR_HINT_IRQ              BIT_14
#define INTCTL_DSP_IRQ0                     BIT_15
#define INTCTL_DSP_IRQ1                     BIT_16
#define INTCTL_SYST_IRQ                     BIT_17
#define INTCTL_EPT_IRQ                      BIT_18
#define INTCTL_IIS_IRQ                      BIT_19
#define INTCTL_DSP_INT_OR_IRQ               BIT_20
#define INTCTL_DMA_IRQ                      BIT_21
#define INTCTL_VBC_IRQ                      BIT_22
#define INTCTL_VSP_IRQ                      BIT_23
#define INTCTL_ANA_DIE_IRQ                  BIT_24
#define INTCTL_ADI_IRQ                      BIT_25
#define INTCTL_USB_IRQ                      BIT_26
#define INTCTL_DCAM_IRQ                     BIT_27
#define INTCTL_NFC_IRQ                      BIT_28
#define INTCTL_LCDC_IRQ                     BIT_29
#define INTCTL_DRM_IRQ                      BIT_30
#define INTCTL_SDIO_IRQ                     BIT_30
#define INTCTL_BUS_MON_IRQ                  BIT_31
#define INTCTL_BUS_MON0_IRQ                 BIT_31
#define INTCTL_BUS_MON1_IRQ                 BIT_31
#define INTCTL_COMMRX_IRQ                   BIT_31


#define INTCTL_PCM_IRQ                      INTCTL_IIS_IRQ
#define INTCTL_ICLR_ALL                     (0xFFFFFFFF)
#define INTCTL_SPC_UART0_RXD_BIT        (BIT_0)
#define INTCTL_SPC_UART0_CTSN_BIT       (BIT_1)
#define INTCTL_SPC_UART1_RXD_BIT        (BIT_2)
#define INTCTL_SPC_UART2_RXD_BIT        (BIT_3)
#define INTCTL_SPC_USB_INPUT_SE0_BIT    (BIT_4)


/*----------Analog Die Interupt Status Registers----------*/

///#define ANA_INTC_BASE                0x82000380
#define ANA_INT_STATUS             (ANA_INTC_BASE + 0x00)
#define ANA_INT_RAW                (ANA_INTC_BASE + 0x04)
#define ANA_INT_EN                 (ANA_INTC_BASE + 0x08)

#define ANA_INT_STATUS_SYNC        (ANA_INTC_BASE + 0x0C)

#define ANA_MAX_INT_NUM            (5)
//#define ANA_INT_MSK              0x1F

#define ANA_TPC_IRQ                BIT_4
#define ANA_WDG_IRQ                BIT_3
#define ANA_RTC_IRQ                BIT_2
#define ANA_GPIO_IRQ               BIT_1
#define ANA_ADC_IRQ                BIT_0


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_INT_REG_V3_H_
// End

