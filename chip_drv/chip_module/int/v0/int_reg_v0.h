/******************************************************************************
 ** File Name:    int_reg_v0.h                                                *
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
#ifndef _INT_REG_V0_H_
#define _INT_REG_V0_H_
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
#define INTCTL_UART_SLEEP_IRQ           (1 << 0)
#define INTCTL_SOFT_IRQ                 (1 << 1)
#define INTCTL_COMMRX                   (1 << 2)
#define INTCTL_COMMTX                   (1 << 3)
#define INTCTL_CNT1_IRQ                 (1 << 4)
#define INTCTL_CNT2_IRQ                 (1 << 5)
#define INTCTL_CNT3_IRQ                 (1 << 6)
#define INTCTL_GPIO_IRQ                 (1 << 7)
#define INTCTL_RTC_IRQ                  (1 << 8)
#define INTCTL_KPD_IRQ                  (1 << 9)
#define INTCTL_I2C_IRQ                  (1 << 10)
#define INTCTL_SIM0_IRQ                 (1 << 11)
#define INTCTL_SIM1_IRQ                 (1 << 12)
#define INTCTL_UART0_IRQ                (1 << 13)
#define INTCTL_UART1_IRQ                (1 << 14)
#define INTCTL_UART2_IRQ                (1 << 15)
#define INTCTL_DSP_IRQ                  (1 << 16)
#define INTCTL_ADC_IRQ                  (1 << 17)
#define INTCTL_GEA_POOL_IRQ             (1 << 18)
#define INTCTL_SYST_IRQ                 (1 << 19)
#define INTCTL_SPI_IRQ                  (1 << 20)
#define INTCTL_DMA_IRQ                  (1 << 21)
#define INTCTL_VBC_IRQ                  (1 << 22)
#define INTCTL_BM_IRQ                   (1 << 23)
#define INTCTL_TPC_IRQ                  (1 << 24)
#define INTCTL_USBD_IRQ                 (1 << 25)
#define INTCTL_DCAM_IRQ                 (1 << 26)
#define INTCTL_IIS_IRQ                  (1 << 27)
#define INTCTL_WDG_IRQ                  (1 << 28)
#define INTCTL_LCDC_IRQ                 (1 << 29)
#define INTCTL_SDIO_IRQ                 (1 << 30)
#define INTCTL_NLC_IRQ                  (1 << 31)

#define INTCTL_PCM_IRQ                  INTCTL_IIS_IRQ
#define INTCTL_ICLR_ALL                 (0x4FF7FFFF)
#define INTCTL_SIM_IRQ                  INTCTL_SIM0_IRQ
#define INTCTL_SPC_UART0_RXD_BIT        (BIT_0)
#define INTCTL_SPC_UART0_CTSN_BIT       (BIT_1)
#define INTCTL_SPC_UART1_RXD_BIT        (BIT_2)
#define INTCTL_SPC_UART2_RXD_BIT        (BIT_3)
#define INTCTL_SPC_USB_INPUT_SE0_BIT    (BIT_4)

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
