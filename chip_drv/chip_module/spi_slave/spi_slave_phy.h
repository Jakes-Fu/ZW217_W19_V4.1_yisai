/******************************************************************************
 ** File Name:      spi_phy.h                                             *
 ** Author:         hanjun.liu                                                *
 ** DATE:           7/8/2009                                                  *
 ** Copyright:      20049 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file declearate the prototype of the physical level  *
 **                 function of serrial device.                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 7/8/2009       hanjun.liu        Create.                                  *
 **  08/26/2009    Xuepeng.Zhang     Clean up                                 *
 ******************************************************************************/
#ifndef _SPI_SLAVE_PHY_H_
#define _SPI_SLAVE_PHY_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
typedef void (*_spi_salve_init) (uint32 spi_mode, uint32 tx_bit_len, spi_slave_tx_callback tx_callback, spi_slave_rx_callback rx_callback);
typedef void (*_spi_salve_exit) (void);
typedef int32 (*_spi_slave_rw) (uint8 *buffer_ptr, uint32 len, uint32 mode);
typedef int32 (*_spi_slave_write) (uint8 *buffer_ptr, uint32 len);
typedef int32 (*_spi_slave_read) (uint8 *buffer_ptr, uint32 len);
typedef int32 (*_spi_slave_dma_rw) (uint8 *buffer_ptr, uint32 len, uint32 mode);
typedef int32 (*_spi_slave_dma_read) (uint8 *buffer_ptr, uint32 len);
typedef int32 (*_spi_slave_dma_write) (uint8 *buffer_ptr, uint32 len);
typedef void (*_spi_slave_s_time_out) (uint32 tick);
typedef  void (*_spi_slave_rst_fifo) (void);

typedef struct
{
    _spi_salve_init init;
    _spi_salve_exit exit;
    _spi_slave_rw rw;
    _spi_slave_write write;
    _spi_slave_read read;
    _spi_slave_dma_rw dma_rw;
    _spi_slave_dma_read dma_r;
    _spi_slave_dma_write dma_w;
    _spi_slave_s_time_out time_out;
    _spi_slave_rst_fifo rst_fifo;
} SPI_SLAVE_PHY_FUN;

#endif

