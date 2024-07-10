/******************************************************************************
 ** File Name:    spi_reg_v0.h                                            *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ******************************************************************************/
#ifndef _SPI_SLAVE_REG_V3_H_
#define _SPI_SLAVE_REG_V3_H_
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
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
/*----------SPI Interface Control Registers----------*/
#define SPI_SLAVE_TXD                         0x0000
#define SPI_SLAVE_CLKD                        0x0004
#define SPI_SLAVE_CTL0                        0x0008
#define SPI_SLAVE_CTL1                        0x000c
#define SPI_SLAVE_CTL2                        0x0010
#define SPI_SLAVE_CTL3                        0x0014
#define SPI_SLAVE_CTL4                        0x0018
#define SPI_SLAVE_CTL5                        0x001c
#define SPI_SLAVE_INT_IEN                     0x0020
#define SPI_SLAVE_INT_CLR                     0x0024
#define SPI_SLAVE_INT_RAW                     0x0028
#define SPI_SLAVE_INT_STS                     0x002c
#define SPI_SLAVE_STS1                        0x0030
#define SPI_SLAVE_STS2                        0x0034
#define SPI_SLAVE_DSP_WAIT                    0x0038
#define SPI_SLAVE_STS3                        0x003c
#define SPI_SLAVE_CTL6                        0x0040
#define SPI_SLAVE_STS4                        0x0044
#define SPI_SLAVE_FIFO_RST                    0x0048

// Bit define for register STS2
#define SPI_SLAVE_RX_FIFO_FULL            BIT_0
#define SPI_SLAVE_RX_FIFO_EMPTY           BIT_1
#define SPI_SLAVE_TX_FIFO_FULL            BIT_2
#define SPI_SLAVE_TX_FIFO_EMPTY           BIT_3
#define SPI_SLAVE_RX_FIFO_REALLY_FULL     BIT_4
#define SPI_SLAVE_RX_FIFO_REALLY_EMPTY    BIT_5
#define SPI_SLAVE_TX_FIFO_REALLY_FULL     BIT_6
#define SPI_SLAVE_TX_FIFO_REALLY_EMPTY    BIT_7
#define SPI_SLAVE_TX_BUSY                 BIT_8
// Bit define for register ctr1
#define SPI_SLAVE_RX_MODE                 BIT_12
#define SPI_SLAVE_TX_MODE                 BIT_13
// Bit define for register ctr2
#define SPI_SLAVE_DMA_EN                  BIT_6
// Bit define for register ctr4
#define SPI_SLAVE_START_RX                BIT_9


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
