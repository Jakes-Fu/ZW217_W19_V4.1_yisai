/******************************************************************************
 ** File Name:    spi_reg_v5.h                                            *
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
#ifndef _SPI_REG_V5_H_
#define _SPI_REG_V5_H_
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
#define SPI_TXD                         0x0000
#define SPI_CLKD                        0x0004
#define SPI_CTL0                        0x0008
#define SPI_CTL1                        0x000c
#define SPI_CTL2                        0x0010
#define SPI_CTL3                        0x0014
#define SPI_CTL4                        0x0018
#define SPI_CTL5                        0x001c
#define SPI_INT_IEN                     0x0020
#define SPI_INT_CLR                     0x0024
#define SPI_INT_RAW                     0x0028
#define SPI_INT_STS                     0x002c
#define SPI_STS1                        0x0030
#define SPI_STS2                        0x0034
#define SPI_DSP_WAIT                    0x0038
#define SPI_STS3                        0x003c
#define SPI_CTL6                        0x0040
#define SPI_STS4                        0x0044
#define SPI_FIFO_RST                    0x0048
#define SPI_CTL7			  0x4c	
//#define SPI_STS5              0x50  
#define SPI_CTL8			  0x54
#define SPI_CTL9			  0x58
#define SPI_CTL10			  0x5c
#define SPI_CTL11			  0x60
#define SPI_CTL12			  0x64

// Bit define for register STS2
#define SPI_RX_FIFO_FULL            BIT_0
#define SPI_RX_FIFO_EMPTY           BIT_1
#define SPI_TX_FIFO_FULL            BIT_2
#define SPI_TX_FIFO_EMPTY           BIT_3
#define SPI_RX_FIFO_REALLY_FULL     BIT_4
#define SPI_RX_FIFO_REALLY_EMPTY    BIT_5
#define SPI_TX_FIFO_REALLY_FULL     BIT_6
#define SPI_TX_FIFO_REALLY_EMPTY    BIT_7
#define SPI_TX_BUSY                 BIT_8
// Bit define for register ctr1
#define SPI_RX_MODE                 BIT_12
#define SPI_TX_MODE                 BIT_13
// Bit define for register ctr2
#define SPI_DMA_EN                  BIT_6
// Bit define for register ctr4
#define SPI_START_RX                BIT_9

#define DUMY_LEN_MASK    0x3F    //[09:04]
#define DATA_LEN_H_MASK  0xF    //[03:00]
#define DATA_LEN_L_MASK  0xFFFF  //[15:00]


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
