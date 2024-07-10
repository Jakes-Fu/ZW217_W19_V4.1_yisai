/******************************************************************************
 ** File Name:      spi_drvapi.h                                                 *
 ** Author:         hao.liu                                                 *
 ** DATE:           12/20/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************/

#ifndef _SPI_DRVAPI_H_
#define _SPI_DRVAPI_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

/* SPI LOGIC PORT */
/* SPI(x)   _   x   _   CS(x)
*      |        |          |  
*   bus_id   pin_gid    cs_num
*/
#define SPI0_0_CS0 0
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#ifdef GPIO_SIMULATE_SPI_SUPPORT
#define SPI1_0_CS0 1
#else
#define SPI1_0_CS0 1
#define SPI1_0_CD 2
#endif
#else
#ifdef GPIO_SIMULATE_SPI_SUPPORT
#define SPI0_0_CS1 1
#define SPI2_0_CS0 2
#define SPI3_0_CS0 3
#define SPI0_1_CS0 4
#define SPI0_1_CS1 5
#define SPI0_2_CS0 6
#define SPI0_2_CS1 7
#define SPI1_0_CS0 8
#define SPI1_0_CS1 9
#define SPI1_1_CS0 10
#define SPI1_1_CS1 11
#define SPI1_2_CS0 12
#define SPI1_2_CS1 13
#define SPI1_3_CS0 14
#define SPI1_3_CS1 15
#else
#define SPI0_0_CS1 1
#define SPI0_1_CS0 2
#define SPI0_1_CS1 3
#define SPI0_2_CS0 4
#define SPI0_2_CS1 5
#define SPI1_0_CS0 6
#define SPI1_0_CS1 7
#define SPI1_1_CS0 8
#define SPI1_1_CS1 9
#define SPI1_2_CS0 10
#define SPI1_2_CS1 11
#define SPI1_3_CS0 12
#define SPI1_3_CS1 13
#define SPI0_3_CS0 14
#define SPI0_3_CS1 15

#define SPI0_0_CD 16
#define SPI0_1_CD 17
#define SPI0_2_CD 18
#define SPI1_0_CD 19
#define SPI1_1_CD 20
#define SPI1_2_CD 21
#define SPI1_3_CD 22
#endif
#endif


/* SPI IOCTL */
#define SPI_CTL_G_MODE 0x30 /*get mode*/
#define SPI_CTL_S_MODE 0x31 /*set mode*/
#define SPI_CTL_G_BITLEN 0x32 /*get bitlen*/
#define SPI_CTL_S_BITLEN 0x33 /*set bitlen*/
#define SPI_CTL_G_FREQ 0x34 /*get freq*/
#define SPI_CTL_S_FREQ 0x35 /*set freqn*/
#define SPI_CTL_S_CS 0x36 /*set cs*/
#define SPI_CTL_S_MS 0x37 /*set master or slave mode*/
#define SPI_CTL_S_PAD 0x38 /*set pad switch*/
#define SPI_CTL_S_CS_NO_MUTEX 0x39 /*set cs, when do this, no mutex to get or put*/
#define SPI_CTL_S_MUTEX 0x40 /*get or put mutex*/

#define SPI_CTL_S_CDLEVEL 0x41 /*set CD level*/
#define SPI_CTL_G_DATAPORT 0x42 /*get spi data port*/
#define SPI_CTL_S_LCDWORKMODE 0x43 /*set spi tx and tx mode*/
#define SPI_CTL_S_TRANSMODE 0x44 /*set the spi as rx mode, tx mode, rxtx mode and idle mode*/

// SPI CS
typedef enum
{
    SPI_CS_HIGH,
    SPI_CS_LOW
} SPI_CSHL_E;

// SPI MUTEX
typedef enum
{
    SPI_MUTEX_PUT,
    SPI_MUTEX_GET
} SPI_MUTEX_E;

// SPI PAD
typedef enum
{
    SPI_PAD_SPI,
    SPI_PAD_GPIO
} SPI_PAD_E;

typedef int32 (*_spi_callback) (uint32 *arg);

/*spi mode*/
typedef enum
{
    CPOL0_CPHA0 = 0,    //sampling on rising edge, clk idle '0'
    CPOL0_CPHA1,        //sampling on falling edge, clk idle '0'
    CPOL1_CPHA0,        //sampling on falling edge, clk idle '1'
    CPOL1_CPHA1         //sampling on rising edge, clk idle '1'
}SPI_MODE_E;

/*spi lcd mode*/
typedef enum
{
    SPI_3WIRE_9BIT_SDA = 1,
    SPI_3WIRE_9BIT_SDIO,
    SPI_4WIRE_8BIT_SDA,
    SPI_4WIRE_8BIT_SDIO,
#ifdef CHIP_VER_6531EFM
    SPI_3WIRE_9BIT_RW,
    SPI_4WIRE_8BIT_RW
#endif //CHIP_VER_6531EFM
}SPI_LCD_MODE_E;

/*spi rxtx mode*/
typedef enum
{
    SPI_RX_ONLY = 0,
    SPI_TX_ONLY,
    SPI_RX_AND_TX,
    SPI_IDLE
}SPI_TXRX_MODE_E;

typedef enum
{
   // rgb565, 16bits
    SPI_3W9B1D_BW = 17, // 16+2 -1
    SPI_3W9B2D_BW = 8,  // 16/2
    SPI_4W8B1D_BW = 16, 
    SPI_4W8B2D_BW = 8,  // 16/2
    SPI_DEFAULT_BW = 16
}SPI_TX_BITWIDTH_E;

typedef struct
{
    uint32 id;
    SPI_MODE_E mode;
    uint32 tx_bit_length;
    uint32 freq;
    //uint32 data_mode;
    _spi_callback spi_cs_cb;
    _spi_callback spi_tx_cb;
    _spi_callback spi_rx_cb;
} SPI_DEV;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
// SPI PAD NAME
typedef enum
{
    /* PAD_SPI(BUS_ID)_GID_ROLE */
    PAD_SPI0_0_CLK = 0,
    PAD_SPI0_0_DI,
    PAD_SPI0_0_DO,
    PAD_SPI0_0_CS0,

    PAD_SPI1_0_CLK,
    PAD_SPI1_0_DI,
    PAD_SPI1_0_DO,
    PAD_SPI1_0_CS0,

    SPI_PAD_MAX
} SPI_PAD_NAME_E;
#else
// SPI PAD NAME
typedef enum
{
    /* PAD_SPI(BUS_ID)_GID_ROLE */
    PAD_SPI0_0_CLK = 0,
    PAD_SPI0_0_DI,
    PAD_SPI0_0_DO,
    PAD_SPI0_0_CS0,
    PAD_SPI0_0_CS1,

    PAD_SPI0_1_CLK,
    PAD_SPI0_1_DI,
    PAD_SPI0_1_DO,
    PAD_SPI0_1_CS0,
    PAD_SPI0_1_CS1,

    PAD_SPI0_2_CLK,
    PAD_SPI0_2_DI,
    PAD_SPI0_2_DO,
    PAD_SPI0_2_CS0,
    PAD_SPI0_2_CS1,

    PAD_SPI1_0_CLK,
    PAD_SPI1_0_DI,
    PAD_SPI1_0_DO,
    PAD_SPI1_0_CS0,
    PAD_SPI1_0_CS1,

    PAD_SPI1_1_CLK,
    PAD_SPI1_1_DI,
    PAD_SPI1_1_DO,
    PAD_SPI1_1_CS0,
    PAD_SPI1_1_CS1,

    PAD_SPI1_2_CLK,
    PAD_SPI1_2_DI,
    PAD_SPI1_2_DO,
    PAD_SPI1_2_CS0,
    PAD_SPI1_2_CS1,

    PAD_SPI1_3_CLK, /*SPI1_0_CS0*/
    PAD_SPI1_3_CLK_ALTERNATE, /*LCMD14*/
    PAD_SPI1_3_DI,
    PAD_SPI1_3_DO,
    PAD_SPI1_3_CS0, /*SPI0_0_CS1*/
    PAD_SPI1_3_CS0_ALTERNATE, /*LCMD15*/
    PAD_SPI1_3_CS1,

    SPI_PAD_MAX
} SPI_PAD_NAME_E;
#endif

// SPI PAD CFG
typedef struct
{
    SPI_PAD_NAME_E pad_name;
    uint32         pad_fun;
    uint32         output; /* GPIO output value, it does not make sense when pad_fun == SPI_PAD_SPI */
} SPI_PAD_CFG_T;

PUBLIC int32 SPI_HAL_Open (SPI_DEV *dev);
PUBLIC int32 SPI_HAL_Close (uint32 logic_id);
PUBLIC uint32 SPI_HAL_Read (uint32 logic_id, uint8 *buffer_ptr, uint32 len);
PUBLIC uint32 SPI_HAL_Write (uint32 logic_id, uint8 *buffer_ptr, uint32 len);
PUBLIC uint32 SPI_HAL_LCDWrite(uint32 logic_id, uint32 *buffer_ptr, uint32 data_len, uint32 dummy_len);
PUBLIC uint32 SPI_HAL_Ioctl (uint32 logic_id, uint32 cmd, uint32 *arg);
PUBLIC uint32 SPI_HAL_RW (uint32 logic_id, uint8 *din_buffer_ptr, uint8 *dout_buffer_ptr, uint32 len);
PUBLIC uint32 SPI_HAL_Read_Async (uint32 logic_id, uint8 *buffer_ptr, uint32 len, _spi_callback rx_cb);
PUBLIC uint32 SPI_HAL_Write_Async (uint32 logic_id, uint8 *buffer_ptr, uint32 len, _spi_callback tx_cb);
/*for compatible*/
#include "spi_drvapi_old.h"
#ifdef   __cplusplus
}
#endif

#endif    //_SPI_DRVAPI_H_
