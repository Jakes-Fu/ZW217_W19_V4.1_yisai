/******************************************************************************
 ** File Name:    int_reg_v5.h                                                *
 ** Author:       Yuhua.Shi                                                   *
 ** DATE:         6/14/2008                                                   *
 ** Copyright:    2008 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 6/14/2008     Yuhua.Shi       Create.                                     *
 ******************************************************************************/
#ifndef _INT_REG_V5_H_
#define _INT_REG_V5_H_
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


//The corresponding bit of all INT_CTL registers.
#define INTCTL_SPECIAL_LATCH_IRQ        (1 << 0)
#define INTCTL_SOFT_IRQ                 (1 << 1)
#define INTCTL_COMMRX                   (1 << 2)
#define INTCTL_COMMTX                   (1 << 3)
#define INTCTL_TIMER0_3_IRQ             (1 << 4)
#define INTCTL_TIMER1_4_IRQ             (1 << 5)
#define INTCTL_GPIO_EICDBC_IRQ          (1 << 6)
#define INTCTL_EIC_SYNC_ASYNC_IRQ       (1 << 7)
#define INTCTL_KPD_IRQ                  (1 << 8)
#define INTCTL_I2C_IRQ                  (1 << 9)
#define INTCTL_SIM0_IRQ                 (1 << 10)
#define INTCTL_UART0_IRQ                (1 << 11)
#define INTCTL_UART1_IRQ                (1 << 12)
#define INTCTL_DSP_IRQ                  (1 << 13)
#define INTCTL_LCDC_IRQ                 (1 << 14)
#define INTCTL_GEA_IRQ                  (1 << 15)
#define INTCTL_SYST_IRQ                 (1 << 16)
#define INTCTL_SPI0_IRQ                 (1 << 17)
#define INTCTL_SPI1_IRQ                 (1 << 18)
#define INTCTL_BM_IRQ                   (1 << 19)
#define INTCTL_DMA_IRQ                  (1 << 20)
#define INTCTL_VBC_IRQ                  (1 << 21)
#define INTCTL_ADI_IRQ                  (1 << 22)
#define INTCTL_TIMER2_5_IRQ             (1 << 23)
#define INTCTL_ANA_IRQ                  (1 << 24)
#define INTCTL_USB_IRQ                  (1 << 25)
#define INTCTL_DCAM_IRQ                 (1 << 26)
#define INTCTL_IIS_IRQ                  (1 << 27)
#define INTCTL_SDIO_IRQ                 (1 << 28)
#define INTCTL_RESERVED_IRQ             (1 << 29)
#define INTCTL_MIDI_FM_ZIP_IRQ          (1 << 30)
#define INTCTL_SFC_IRQ                  (1UL << 31)

#define INTCTL_SIM1_IRQ                 INTCTL_SIM0_IRQ
#define INTCTL_PCM_IRQ                  INTCTL_IIS_IRQ
//#define INTCTL_ICLR_ALL                 (0x4FF7FFFF)
#define INTCTL_SIM_IRQ                  INTCTL_SIM0_IRQ
/*----------Analog Die Interupt Status Registers----------*/

///#define ANA_INTC_BASE                0x82000380
#define ANA_INT_STATUS             (ANA_INTC_BASE + 0x00)
#define ANA_INT_RAW                (ANA_INTC_BASE + 0x04)
#define ANA_INT_EN                 (ANA_INTC_BASE + 0x08)

#define ANA_INT_STATUS_SYNC        (ANA_INTC_BASE + 0x0C)

#define ANA_MAX_INT_NUM            (5)
//#define ANA_INT_MSK              0x1F


#define ANA_IFA_FIFO_ERR_IRQ       BIT_8
#define ANA_AUDIFA_IRQ             BIT_7
#define ANA_CHGRWDG_IRQ            BIT_6
#define ANA_EIC_IRQ                BIT_5
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
#endif
// End
