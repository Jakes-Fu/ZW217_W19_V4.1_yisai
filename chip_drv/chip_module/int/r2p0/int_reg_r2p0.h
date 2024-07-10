/******************************************************************************
 ** File Name:    SC6530_REG_int.h                                            *
 ** Author:       Yong.Li                                                     *
 ** DATE:         07/28/2011                                                  *
 ** Copyright:    2008 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 07/28/2011    Yong.Li         Create.                                     *
 ** 04/12/2012    Yong.Li         Modified.                                   *
 ******************************************************************************/
#ifndef _INT_REG_R2P0_H_
#define _INT_REG_R2P0_H_
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
#define INT_IRQ_BASE                     (INT_REG_BASE + 0x0000)
#define INT_IRQ_STS                      (INT_REG_BASE + 0x0000) //Interrupt status after masked by irq_enable.
#define INT_IRQ_RAW_STS                  (INT_REG_BASE + 0x0004) //Interrupt status from different interrupt source.
#define INT_IRQ_EN                       (INT_REG_BASE + 0x0008) //Enable bits  for the corresponding interrupt sources. 
#define INT_IRQ_DISABLE                  (INT_REG_BASE + 0x000C)
#define INT_IRQ_SOFT                     (INT_REG_BASE + 0x0010)
#define INT_IRQ_TEST_SRC                 (INT_REG_BASE + 0x0014)
#define INT_IRQ_TEST_SEL                 (INT_REG_BASE + 0x0018)
#define INT_IRQ_UINT_STS                 (INT_REG_BASE + 0x001C)
#define INT_FIQ_STS                      (INT_REG_BASE + 0x0020)
#define INT_FIQ_RAW_STS                  (INT_REG_BASE + 0x0024)
#define INT_FIQ_EN                       (INT_REG_BASE + 0x0028)
#define INT_FIQ_DISABLE                  (INT_REG_BASE + 0x002C)
#define INT_FIQ_SOFT                     (INT_REG_BASE + 0x0030)
#define INT_FIQ_TEST_SRC                 (INT_REG_BASE + 0x0034)
#define INT_FIQ_TEST_SEL                 (INT_REG_BASE + 0x0038)
#define INT_UINT_CTL                     (INT_REG_BASE + 0x003C)


#define INT0_IRQ_BASE                    (INT0_REG_BASE + 0x0000)
#define INT0_IRQ_STS                     (INT0_REG_BASE + 0x0000) //Interrupt status after masked by irq_enable.
#define INT0_IRQ_RAW_STS                 (INT0_REG_BASE + 0x0004) //Interrupt status from different interrupt source.
#define INT0_IRQ_EN                      (INT0_REG_BASE + 0x0008) //Enable bits  for the corresponding interrupt sources. 
#define INT0_IRQ_DISABLE                 (INT0_REG_BASE + 0x000C)
#define INT0_IRQ_SOFT                    (INT0_REG_BASE + 0x0010)
#define INT0_IRQ_TEST_SRC                (INT0_REG_BASE + 0x0014)
#define INT0_IRQ_TEST_SEL                (INT0_REG_BASE + 0x0018)
#define INT0_IRQ_UINT_STS                (INT0_REG_BASE + 0x001C)
#define INT0_FIQ_STS                     (INT0_REG_BASE + 0x0020)
#define INT0_FIQ_RAW_STS                 (INT0_REG_BASE + 0x0024)
#define INT0_FIQ_EN                      (INT0_REG_BASE + 0x0028)
#define INT0_FIQ_DISABLE                 (INT0_REG_BASE + 0x002C)
#define INT0_FIQ_SOFT                    (INT0_REG_BASE + 0x0030)
#define INT0_FIQ_TEST_SRC                (INT0_REG_BASE + 0x0034)
#define INT0_FIQ_TEST_SEL                (INT0_REG_BASE + 0x0038)
#define INT0_UINT_CTL                    (INT0_REG_BASE + 0x003C)


#define INT1_IRQ_BASE                    (INT1_REG_BASE + 0x0000)
#define INT1_IRQ_STS                     (INT1_REG_BASE + 0x0000) //Interrupt status after masked by irq_enable.
#define INT1_IRQ_RAW_STS                 (INT1_REG_BASE + 0x0004) //Interrupt status from different interrupt source.
#define INT1_IRQ_EN                      (INT1_REG_BASE + 0x0008) //Enable bits  for the corresponding interrupt sources. 
#define INT1_IRQ_DISABLE                 (INT1_REG_BASE + 0x000C)
#define INT1_IRQ_SOFT                    (INT1_REG_BASE + 0x0010)
#define INT1_IRQ_TEST_SRC                (INT1_REG_BASE + 0x0014)
#define INT1_IRQ_TEST_SEL                (INT1_REG_BASE + 0x0018)
#define INT1_IRQ_UINT_STS                (INT1_REG_BASE + 0x001C)
#define INT1_FIQ_STS                     (INT1_REG_BASE + 0x0020)
#define INT1_FIQ_RAW_STS                 (INT1_REG_BASE + 0x0024)
#define INT1_FIQ_EN                      (INT1_REG_BASE + 0x0028)
#define INT1_FIQ_DISABLE                 (INT1_REG_BASE + 0x002C)
#define INT1_FIQ_SOFT                    (INT1_REG_BASE + 0x0030)
#define INT1_FIQ_TEST_SRC                (INT1_REG_BASE + 0x0034)
#define INT1_FIQ_TEST_SEL                (INT1_REG_BASE + 0x0038)
#define INT1_UINT_CTL                    (INT1_REG_BASE + 0x003C)

#define INTCTL_SPECIAL_LATCH_IRQ        BIT_0
#define INTCTL_SOFT_IRQ                 BIT_1
#define INTCTL_COMMRX                   BIT_2
#define INTCTL_COMMTX                   BIT_3
#define INTCTL_TIMER0_IRQ               BIT_4
#define INTCTL_TIMER1_IRQ               BIT_5
#define INTCTL_TIMER2_IRQ               BIT_6
#define INTCTL_TIMER3_IRQ               BIT_7
#define INTCTL_TIMER4_IRQ               BIT_8
#define INTCTL_TIMER5_IRQ               BIT_9
#define INTCTL_GPIO_EICDBC_IRQ          BIT_10
#define INTCTL_EIC_SYNC_ASYNC_IRQ     BIT_11
#define INTCTL_KPD_IRQ                  BIT_12
#define INTCTL_I2C0_IRQ                 BIT_13
#define INTCTL_I2C1_IRQ                 BIT_14
#if 0 //!defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910)  && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A)
#define INTCTL_I2C2_IRQ                 BIT_15
#endif
#define INTCTL_AUD_IRQ                  BIT_16
#define INTCTL_SIM0_IRQ                 BIT_17
#define INTCTL_UART0_IRQ                BIT_18
#define INTCTL_UART1_IRQ                BIT_19
#if  0 //!defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A)
#define INTCTL_UART2_IRQ                BIT_20
#endif
#define INTCTL_DSP_IRQ                  BIT_21
#define INTCTL_LCDC_IRQ                 BIT_22
#define INTCTL_GEA_IRQ                  BIT_23
#define INTCTL_SYST_IRQ                 BIT_24
#define INTCTL_SPI0_IRQ                 BIT_25
#define INTCTL_SPI1_IRQ                 BIT_26
#define INTCTL_BM0_IRQ                  BIT_27
#if 0 //!defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A)
#define INTCTL_BM1_IRQ                  BIT_28
#define INTCTL_BM2_IRQ                  BIT_29
#define INTCTL_BM3_IRQ                  BIT_30
#else
#define INTCTL_CM4_TO_ARM9_IRQ2                  BIT_28
#define INTCTL_CM4_TO_ARM9_IRQ1                  BIT_29
#define INTCTL_CM4_TO_ARM9_IRQ0                  BIT_30
#endif
#define INTCTL_DMA_IRQ                  BIT_31

#define INTCTL_VBC_IRQ                  BIT_2  //BIT_34
#define INTCTL_ADI_IRQ                  BIT_3  //BIT_35
#define INTCTL_ANA_IRQ                  BIT_4  //BIT_36
#define INTCTL_USB_IRQ                  BIT_5  //BIT_37
#define INTCTL_DCAM_IRQ                 BIT_6  //BIT_38
#define INTCTL_IIS_IRQ                  BIT_7  //BIT_39
#define INTCTL_SDIO_IRQ                 BIT_8  //BIT_40
#define INTCTL_ARM7_IRQ                 BIT_9  //BIT_41
#define INTCTL_ARM7_TIMER_IRQ           BIT_10 //BIT_42
#if 0 //!defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A)
#define INTCTL_MIDI_IRQ                 BIT_11 //BIT_43
#endif
#define INTCTL_FM_IRQ                   BIT_12 //BIT_44
#define INTCTL_ZIP0_IRQ                 BIT_13 //BIT_45
#if 0 //!defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A)
#define INTCTL_ZIP1_IRQ                 BIT_14 //BIT_46
#else
#define INTCTL_RF_MASTER_IRQ                 BIT_14 //BIT_46
#endif
#define INTCTL_SFC_IRQ                  BIT_15 //BIT_47
#if  1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
#define INTCTL_ARM7_WDG_IRQ                  BIT_16 //BIT_48
#endif
#define INTCTL_ICLR_ALL                     (0xFFFFFFFF)

#define INTCTL_SIM1_IRQ                 INTCTL_SIM0_IRQ
#define INTCTL_PCM_IRQ                  INTCTL_IIS_IRQ
#define INTCTL_SIM_IRQ                  INTCTL_SIM0_IRQ
#define INTCTL_BM_IRQ                   INTCTL_BM0_IRQ
#define INTCTL_MIDI_FM_ZIP_IRQ          INTCTL_ZIP0_IRQ
#define INTCTL_TIMER0_3_IRQ             INTCTL_TIMER0_IRQ
#define INTCTL_TIMER1_4_IRQ             INTCTL_TIMER1_IRQ
#define INTCTL_TIMER2_5_IRQ             INTCTL_TIMER2_IRQ
#define INTCTL_I2C_IRQ                  INTCTL_I2C0_IRQ

/*----------Analog Die Interupt Status Registers----------*/
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
#define ANA_IRQ_BASE               ANA_INTC_BASE + 0x20
#define ANA_IRQ_STATUS             (ANA_IRQ_BASE + 0x00)
#define ANA_IRQ_RAW                (ANA_IRQ_BASE + 0x04)
#define ANA_IRQ_EN                 (ANA_IRQ_BASE + 0x08)

#define ANA_FIQ_BASE               ANA_INTC_BASE + 0x0
#define ANA_FIQ_STATUS             (ANA_FIQ_BASE + 0x00)
#define ANA_FIQ_RAW                (ANA_FIQ_BASE + 0x04)
#define ANA_FIQ_EN                 (ANA_FIQ_BASE + 0x08)

#define ANA_INT_STATUS             (ANA_IRQ_BASE + 0x00)
#define ANA_INT_RAW                (ANA_IRQ_BASE + 0x04)
#define ANA_INT_EN                 (ANA_IRQ_BASE + 0x08)
#else
#define ANA_INT_STATUS             (ANA_INTC_BASE + 0x00)
#define ANA_INT_RAW                (ANA_INTC_BASE + 0x04)
#define ANA_INT_EN                 (ANA_INTC_BASE + 0x08)
#endif

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
#define ANA_INT_ADC                  0
#define ANA_INT_GPIO                 1
#define ANA_INT_RTC                  2
#define ANA_INT_WDG                  3
#define ANA_INT_TPC                  4
#define ANA_INT_EIC                  5
#define ANA_INT_CHGRWDG              6
#define ANA_INT_AUDIFA               7
#define ANA_INT_ANAFA_FIFO_ERR       8
#define ANA_INT_MAX                  9
#define ANA_MAX_INT_NUM            (9)
#else
#define ANA_INT_ADC                  0
#define ANA_INT_RTC                  2
#define ANA_INT_WDG                  3
#define ANA_INT_EIC                  5
#define ANA_INT_AUDPROTECT           7
#define ANA_INT_CAL               	 8
#define ANA_INT_MAX                  9
#define ANA_MAX_INT_NUM            (9)
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E) 
#define ANA_CAL_IRQ       BIT_8
#define ANA_AUDPROTECT_IRQ             BIT_7
#define ANA_EIC_IRQ                BIT_5
#define ANA_WDG_IRQ                BIT_3
#define ANA_RTC_IRQ                BIT_2
#define ANA_ADC_IRQ                BIT_0
#else
#define ANA_IFA_FIFO_ERR_IRQ       BIT_8
#define ANA_AUDIFA_IRQ             BIT_7
#define ANA_CHGRWDG_IRQ            BIT_6
#define ANA_EIC_IRQ                BIT_5
#define ANA_TPC_IRQ                BIT_4
#define ANA_WDG_IRQ                BIT_3
#define ANA_RTC_IRQ                BIT_2
#define ANA_GPIO_IRQ               BIT_1
#define ANA_ADC_IRQ                BIT_0
#endif

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
#endif //_INT_REG_R2P0_H_
// End

