/******************************************************************************
 ** File Name:    pcm_reg_v5.h                                                *
 ** Author:       Steve.Zhan                                                  *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ** 11/06/2010    Jeff.Li         Modify for new chip driver architecture.    *
 ******************************************************************************/
#ifndef _PCM_REG_V5_H_
#define _PCM_REG_V5_H_
#include "pcm_drvapi.h"
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
#define WORD_SIZE               (4)
#define HALF_WORD_SIZE          (2)
#define BYTE_SIZE               (1)

#define PCM_DATA_LEN            120

#define FIFO_FULL_WATERMARK             0x0A
#define FIFO_EMPTY_WATERMARK           0x06

//#define DMA_PCM_RX              DMA_IIS_RX
//#define DMA_PCM_TX              DMA_IIS_TX

#define PCM_DATA_LEN            120

/*----------IIS Interface Control Registers----------*/
#define PCM_FIFO_SIZE_IN_WORD           16

// IIS_CTL0
#define IIS_CTL0_BIT_PER_CHN_SHIFT      (4)
#define IIS_CTL0_BIT_PER_CHN_MASK       (0x3 << IIS_CTL0_BIT_PER_CHN_SHIFT)
#define IIS_CTL0_BIT_PER_CHN_8          0
#define IIS_CTL0_BIT_PER_CHN_16         1
#define IIS_CTL0_BIT_PER_CHN_32         2

#define IIS_CTL0_SLAVE_MODE             BIT_3
#define IIS_CTL0_IISC_SHRTH             BIT_8
#define IIS_CTL0_SYNC_MODE              BIT_9
#define IIS_CTL0_PCM_SCK_REV            BIT_11
#define IIS_CTL0_PCM_DMA_EB             BIT_14
#define IIS_CTL0_PCM_EB                 BIT_15

// IIS_CTL2
#define IIS_CTL2_PCM_SLOT_SHIFT         (0)
#define IIS_CTL2_PCM_SLOT_MASK          (0x7 << IIS_CTL2_PCM_SLOT_SHIFT)

#define IIS_CTL2_PCM_CYCLE_SHIFT        (3)
#define IIS_CTL2_PCM_CYCLE_MASK         (0x7F << IIS_CTL2_PCM_CYCLE_SHIFT)

// IIS_CTL3
#define IIS_CTL3_RX_FULL_THRHLD_SHIFT   (0)
#define IIS_CTL3_RX_FULL_THRHLD_MASK    (0xF << IIS_CTL3_RX_FULL_THRHLD_SHIFT)
#define IIS_CTL3_RX_EMPTY_THRHLD_SHIFT  (4)
#define IIS_CTL3_RX_EMPTY_THRHLD_MASK   (0xF << IIS_CTL3_RX_EMPTY_THRHLD_SHIFT)

#define IIS_CTL3_TX_FULL_THRHLD_SHIFT   (8)
#define IIS_CTL3_TX_FULL_THRHLD_MASK    (0xF << IIS_CTL3_TX_FULL_THRHLD_SHIFT)
#define IIS_CTL3_TX_EMPTY_THRHLD_SHIFT  (12)
#define IIS_CTL3_TX_EMPTY_THRHLD_MASK   (0xF << IIS_CTL3_TX_EMPTY_THRHLD_SHIFT)

// IIS_INT_IEN
#define IIS_INT_IEN_RX_FIFO_FULL_EN     BIT_0
#define IIS_INT_IEN_RX_FIFO_EMPTY_EN    BIT_1
#define IIS_INT_IEN_TX_FIFO_FULL_EN     BIT_2
#define IIS_INT_IEN_TX_FIFO_EMPTY_EN    BIT_3
#define IIS_INT_IEN_RX_OVERRUN_EN       BIT_4
#define IIS_INT_IEN_SLAVE_TIMEOUT_EN    BIT_5
#define IIS_INT_IEN_RX_FULL_EN          BIT_6
#define IIS_INT_IEN_TX_EMPTY_EN         BIT_7

// IIS_INT_CLR
#define IIS_INT_IEN_RX_FIFO_FULL_CLR    BIT_0
#define IIS_INT_IEN_RX_FIFO_EMPTY_CLR   BIT_1
#define IIS_INT_IEN_TX_FIFO_FULL_CLR    BIT_2
#define IIS_INT_IEN_TX_FIFO_EMPTY_CLR   BIT_3
#define IIS_INT_IEN_RX_OVERRUN_CLR      BIT_4
#define IIS_INT_IEN_SLAVE_TIMEOUT_CLR   BIT_5

// IIS_INT_STS
#define IIS_INT_STS_RX_FIFO_FULL        BIT_0
#define IIS_INT_STS_TX_FIFO_FULL        BIT_2
#define IIS_INT_STS_TX_FIFO_EMPTY       BIT_3
#define IIS_INT_STS_SLAVE_TIMEOUT       BIT_5


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
// PCM control register filed definitions
typedef struct
{
    volatile uint32 iis_fifo;               // 0x00, data register
    volatile uint32 iis_clkd;               // 0x04, interrupt status reigster
    volatile uint32 iis_ctl0;               // 0x08, fifo cnt register, bit[5:0] for RX and [13:8] for TX
    volatile uint32 iis_ctl1;               // 0x0c, interrutp enable register
    volatile uint32 iis_ctl2;               // 0x10, control register
    volatile uint32 iis_ctl3;               // 0x14, Receive Data full threshold/Receive Data full threshold
    volatile uint32 iis_ien;                // 0x18, 2-wire mode reigster
    volatile uint32 iis_iclr;               // 0x1c, clock dividor register
    volatile uint32 iis_iraw;               // 0x20, masked interrupt status register
    volatile uint32 iis_ists;               // 0x24, interrupt clear register 0024
    volatile uint32 iis_sts1;               // 0x28, transmit data interval
    volatile uint32 iis_sts2;               // 0x2c, transmit data interval
    volatile uint32 iis_sts3;               // 0x30,
} PCM_CTL_REG_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
#define PCM_SET_RX_WATERMARK(param) do {  \
        volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE;    \
        reg_ptr->iis_ctl3 &=  ~ (IIS_CTL3_RX_FULL_THRHLD_MASK | IIS_CTL3_RX_EMPTY_THRHLD_MASK); \
        reg_ptr->iis_ctl3 |= ((param->rx_watermark << IIS_CTL3_RX_FULL_THRHLD_SHIFT)| \
                              (param->tx_watermark << IIS_CTL3_RX_EMPTY_THRHLD_SHIFT) ); \
    } while (0)

#define PCM_SET_TX_WATERMARK(param) do {  \
        volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE; \
        reg_ptr->iis_ctl3 &=  ~ (IIS_CTL3_TX_FULL_THRHLD_MASK| IIS_CTL3_TX_EMPTY_THRHLD_MASK); \
        reg_ptr->iis_ctl3 |= ((param->rx_watermark << IIS_CTL3_TX_FULL_THRHLD_SHIFT) | \
                              (param->tx_watermark << IIS_CTL3_TX_EMPTY_THRHLD_SHIFT) ); \
    } while (0)

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
