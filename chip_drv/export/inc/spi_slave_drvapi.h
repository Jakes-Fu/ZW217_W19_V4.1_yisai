/******************************************************************************
 ** File Name:      spi_drvapi.h                                                 *
 ** Author:         hao.liu                                                 *
 ** DATE:           12/20/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************/

#ifndef _SPI_SLAVE_DRVAPI_H_
#define _SPI_SLAVE_DRVAPI_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

/* SPI_SLAVE IOCTL */
#define SPI_SLAVE_CTL_G_MODE 0x30 /*get mode*/
#define SPI_SLAVE_CTL_S_MODE 0x31 /*set mode*/
#define SPI_SLAVE_CTL_G_BITLEN 0x32 /*get bitlen*/
#define SPI_SLAVE_CTL_S_BITLEN 0x33 /*set bitlen*/
#define SPI_SLAVE_CTL_S_TIMEOUT 0x34 /*set timeout*/

/*SPI_SLAVE error code define*/
typedef enum
{
    ERR_SPI_SLAVE_NONE = 0,
    ERR_SPI_SLAVE_TIMEOUT,
    ERR_SPI_SLAVE_NO_MUTEX
}
ERR_SPI_SLAVE_E;

typedef void (*spi_slave_tx_callback) (void);
typedef void (*spi_slave_rx_callback) (void);

typedef struct
{
    uint32 mode;
    uint32 tx_bit_length;
    spi_slave_tx_callback tx_callback;
    spi_slave_rx_callback rx_callback;
} SPI_SLAVE_DEV;

PUBLIC void SPI_SLAVE_HAL_Open (SPI_SLAVE_DEV *dev);
PUBLIC void SPI_SLAVE_HAL_Close (void);
PUBLIC uint32 SPI_SLAVE_HAL_Read (uint8 *buffer_ptr, uint32 len);
PUBLIC uint32 SPI_SLAVE_HAL_Write (uint8 *buffer_ptr, uint32 len);
PUBLIC uint32 SPI_SLAVE_HAL_Ioctl (uint32 cmd, uint32 *arg);

#ifdef   __cplusplus
}
#endif

#endif    //_SPI_SLAVE_DRVAPI_H_
