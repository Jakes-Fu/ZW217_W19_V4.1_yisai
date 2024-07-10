/******************************************************************************
 ** File Name:    uws6121e_module_config.h                                      *
 ** Author:       Yong.Li                                                     *
 ** DATE:         04/11/2012                                                  *
 ** Copyright:    2011 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 04/11/2012    Yong.Li         Create.                                      *
 ******************************************************************************/
#ifndef _UWS6121E_MODULE_CONFIG_H_
#define _UWS6121E_MODULE_CONFIG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                               Macro Define for Watch Dog                   **
**---------------------------------------------------------------------------*/
#define WDG_INIT_COUNT      (0x1E0000) //uint : 1/32768    = 60s
#define WDG_IRQ_COUNT       (0x020000) //uint : 1/32768    =4s   (60-4 = 56s isue the interrupte)

/**---------------------------------------------------------------------------*
**                               Macro Define for bus monitor                       **
**---------------------------------------------------------------------------*/
#define MAX_BM_POINTS_NUM   1

/**---------------------------------------------------------------------------*
**                               Macro Define for kpd                        **
**---------------------------------------------------------------------------*/
#define KPD_ROW_NUM         5           // KPD_ROW_MIN_NUM <= value <= KPD_ROW_MAX_NUM   keyout
#define KPD_COL_NUM         8           // KPD_COL_MIN_NUM <= value <= KPD_COL_MAX_NUM   keyin

#define CFG_ROW_POLARITY    (0x00FF & KPDPOLARITY_ROW)
#define CFG_COL_POLARITY    (0xFF00 & KPDPOLARITY_COL)

#define CFG_CLK_DIV         1
#define MAX_MUL_KEY_NUM     4


/**---------------------------------------------------------------------------*
**                               Macro Define for pwm                        **
**---------------------------------------------------------------------------*/
#define PWM_NUM         1
#define PWMx_BASE(x)    (PWM_BASE + (x)*0x20 )

#define GEN_SET_PWM         APB_EB0_SET
#define GEN_SET_PWMx(x)     (PWM_EB_SET << (x))
#define GEN_CLR_PWM         APB_EB0_CLR
#define GEN_CLR_PWMx(x)     (PWM_EB_CLR << (x))

#define GCLK_PWM        APB_CLK_CFG0
#define GCLK_PWMx(x)    (CLK_PWM_SEL << (x))

// the prescale = 0, and the mod = 10
#define   PWM_MAX_FREQ( _pwm_clk )   ( (_pwm_clk) / 10 )
#define   PWM_MIN_FREQ( _pwm_clk )   ( (_pwm_clk) / 256 / 256 )

/**---------------------------------------------------------------------------*
**                               Macro Define for int                        **
**---------------------------------------------------------------------------*/

#define FIQ_MAX_INT_NUM            0x20


/**---------------------------------------------------------------------------*
**                               Macro Define for Charger                    **
**---------------------------------------------------------------------------*/
#if defined(CHGMNG_PSE_EVT_SUPPORT)
#define ADC_CHANNEL_TEMP_DET    (4)
#else
#define ADC_CHANNEL_BAT_DET    (0)
#endif

#define ADC_CHANNEL_ADCI1      (1)
#define ADC_CHANNEL_ADCI2      (2)  //NTC
#define ADC_CHANNEL_VBAT    (11)
#define ADC_CHANNEL_VCHG    (14)
#define ADC_CHANNEL_PROG    (16)

#define CHGMNG_STOP_VPROG	300 //100//0x70  // Isense stop point,current value
#define CHGMNG_SWITCH_CV_VPROG	120 //100 //100//0x70              // Isense stop point ,current value

/**---------------------------------------------------------------------------*
**                               Macro Define for GPIO                   **
**---------------------------------------------------------------------------*/

#define GPIO_INVALID_ID             0x0FF


#define GPIO_MAX_PIN_NUM            (32+16)   //16 in GPIO1, and 32 in GPIO2


#define GPIO_MAX_REC_NUM            10


/**---------------------------------------------------------------------------*
**                               Micro Define for SDIO                    **
**---------------------------------------------------------------------------*/
#define     SDIO_CLK_48M            48000000        // 48   MHz, 1 div for 48MHz or 2 div for 96MHz
#define     SDIO_CLK_24M            32000000        // 24  MHz
#define     SDIO_CLK_16M            16000000        // 16  MHz
#define     SDIO_CLK_12M            12500000        // 12   MHz, 4 div for 50MHz
#define     SDIO_CLK_8M             8000000         // 8   MHz
#define     SDIO_CLK_6M             6000000         // 6   MHz, 8 div for 50MHz
#define     SDIO_CLK_4M             4000000         // 4   MHz
#define     SDIO_CLK_3M             3000000         // 3  MHz, 16 div for 50MHz
#define     SDIO_CLK_2M             2000000         // 2   MHz
#define     SDIO_CLK_1M             1000000         // 1   MHz
#define     SDIO_CLK_500K           500000          // 500 KHz, 64 div for 50MHz
#define     SDIO_CLK_250K           250000          // 250 KHz
#define     SDIO_CLK_125K           125000          // 125 KHz
#define     SDIO_MAX_CLK            SDIO_CLK_48M  // 48MHz


#define     SDIO_BASE_CLK_104M      104000000        // 100 MHz
#define     SDIO_BASE_CLK_100M      100000000        // 100 MHz
#define     SDIO_BASE_CLK_96M       96000000        // 96 MHz
#define     SDIO_BASE_CLK_90M       90000000        // 90 MHz
#define     SDIO_BASE_CLK_78M       78000000        // 90 MHz
#define     SDIO_BASE_CLK_48M       48000000        // 48 MHz
#define     SDIO_BASE_CLK_45M       45000000        // 45 MHz
#define     SDIO_BASE_CLK_26M       26000000        // 26 MHz

#define     SDIO_BASE_CLK           SDIO_BASE_CLK_96M    // 96MHz
#define     SDIO_SD_MAX_CLK         SDIO_CLK_48M          //SDIO_CLK_50M//

//---

//=====
//define transfer mode and command mode...
//command mode
#define SDIO_CMD_TYPE_ABORT         (3<<22)
#define SDIO_CMD_TYPE_RESUME        (2<<22)
#define SDIO_CMD_TYPE_SUSPEND       (1<<22)
#define SDIO_CMD_TYPE_NML           (0<<22)

#define SDIO_CMD_DATA_PRESENT       BIT_21

#define SDIO_CMD_INDEX_CHK          BIT_20
#define SDIO_CMD_CRC_CHK            BIT_19
#define SDIO_CMD_NO_RSP             (0x00<<16)
#define SDIO_CMD_RSP_136            (0x01<<16)
#define SDIO_CMD_RSP_48             (0x02<<16)
#define SDIO_CMD_RSP_48_BUSY        (0x03<<16)

#define SDIO_NO_RSP                 0x0;
#define SDIO_R1     ( SDIO_CMD_RSP_48 | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R2     ( SDIO_CMD_RSP_136 | SDIO_CMD_CRC_CHK )
#define SDIO_R3     SDIO_CMD_RSP_48
#define SDIO_R4     SDIO_CMD_RSP_48
#define SDIO_R5     ( SDIO_CMD_RSP_48 | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R6     ( SDIO_CMD_RSP_48 | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R7     ( SDIO_CMD_RSP_48 | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R1B    ( SDIO_CMD_RSP_48_BUSY | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R5B    ( SDIO_CMD_RSP_48_BUSY | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
//transfer mode
#define SDIO_TRANS_COMP_ATA         BIT_6
#define SDIO_TRANS_MULTIBLK         BIT_5
#define SDIO_TRANS_DIR_READ         BIT_4
#define SDIO_TRANS_AUTO_CMD12_EN    BIT_2
#define SDIO_TRANS_BLK_CNT_EN       BIT_1
#define SDIO_TRANS_DMA_EN           BIT_0

//=====
//define normal and error sts index...
#define SDIO_VENDOR_SPEC_ERR        (BIT_29|BIT_30|BIT_31)
#define SDIO_TARGET_RESP_ERR        (BIT_28)
#define SDIO_AUTO_CMD12_ERR         (BIT_24)
#define SDIO_CURRENT_LMT_ERR        (BIT_23)
#define SDIO_DATA_ENDBIT_ERR        (BIT_22)
#define SDIO_DATA_CRC_ERR           (BIT_21)
#define SDIO_DATA_TMOUT_ERR         (BIT_20)
#define SDIO_CMD_INDEX_ERR          (BIT_19)
#define SDIO_CMD_ENDBIT_ERR         (BIT_18)
#define SDIO_CMD_CRC_ERR            (BIT_17)
#define SDIO_CMD_TMOUT_ERR          (BIT_16)
#define SDIO_ERROR_INT              (BIT_15)
#define SDIO_CARD_INT               (BIT_8)
#define SDIO_CARD_REMOVAL           (BIT_7)
#define SDIO_CARD_INSERTION         (BIT_6)
#define SDIO_BUF_READ_RDY           (BIT_5)
#define SDIO_BUF_WRITE_RDY          (BIT_4)
#define SDIO_DMA_INT                (BIT_3)
#define SDIO_BLK_GAP_EVT            (BIT_2)
#define SDIO_TRANSFER_CMPLETE       (BIT_1)
#define SDIO_CMD_CMPLETE            (BIT_0)

//=============================================
// tmp code, will be deleted at next step
#define SDIO_PIN_CMD                    (PIN_CTL_BASE + 0x00f0)
#define SDIO_PIN_D0                 (PIN_CTL_BASE + 0x00f4)
#define SDIO_PIN_D1                 (PIN_CTL_BASE + 0x00f8)
#define SDIO_PIN_D2                 (PIN_CTL_BASE + 0x00fc)
#define SDIO_PIN_D3                 (PIN_CTL_BASE + 0x0100)

#define SDIO_PINMAP_VALUE            0x384
//=============================================

#define USB_FIFO_MAX_WORD               16
#define USB_FIFO_MAX_BYTE               64
#define USB_MAX_TRANSFER_SIZE           (32*1024)

//=============================================


/* Global Ctl macro define*/

#define MAX_SIM0_PORT  3
#define MAX_UART0_PORT 2
#define MAX_UART1_PORT 3
#define MAX_I2C0_PORT  4

/* end of Global Ctl macro define*/


#include "sci_types.h"
#include "chng_freq.h"
#include "uws6121e_int_cfg.h"
#include "uws6121e_gpio_cfg.h"
#include "uws6121e_i2c_cfg.h"
#include "sc6531efm_spi_cfg.h"
#include "uws6121e_mem_cfg.h"
#include "sc6531efm_freq_cfg.h"
#include "uws6121e_dma_cfg.h"
#include "uws6121e_sleep_cfg.h"
#include "sc6531efm_audio_cfg.h"
#include "uws6121e_timer_cfg.h"
#include "uws6121e_eic_cfg.h"
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
