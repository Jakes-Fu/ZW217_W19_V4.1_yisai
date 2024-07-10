/******************************************************************************
 ** File Name:      spi_drvapi.h                                                 *
 ** Author:         Jimmy.Jia                                                 *
 ** DATE:           10/28/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************/

#ifndef _SPI_DRVAPI_OLD_H_
#define _SPI_DRVAPI_OLD_H_

// SPI Device type
typedef enum
{
    SPI_SDCARD,
    SPI_DEV_TYPE_MAX=0x7FFFFFFF
} SPI_DEV_TYPE_E;

// defined SPI Chip Select pin number
typedef enum
{
    SPI_CS_ID0,
    SPI_CS_ID1,
    SPI_CS_ID2,
    SPI_CS_ID3
} SPI_CSID_E;

//SPI configuration
typedef struct
{
    BOOLEAN        rx_shift_edge; // "1" Enable Rx data shift in at clock neg-edge
    BOOLEAN        tx_shift_edge; // "1" Enable Tx data shift out at clock neg-edge
    BOOLEAN        sck_reverse;   // "1" reverse the sck
    BOOLEAN        lsb_first;     // "1" enable data transmit/receive from LSB
    uint8          tx_bit_length; // Transmit data bit number
    SPI_DEV_TYPE_E dev_type;
    SPI_CSID_E     cs_id;
    uint32         freq;          // SPI bus clock
} SPI_CONFIG_T;


/******************************************************************************
**  Description:    This function Open spi port
**  Author:         Jimmy.Jia
**  08/26/2009      Xuepeng.Zhang    Clean up
*******************************************************************************/
PUBLIC uint32 SPI_Open (
    uint32          phy_port,    // port number of spi
    SPI_CONFIG_T    *spi_config  // spi config data
);

/******************************************************************************
**  Description:    This function close spi port
**  Author:         Jimmy.Jia
**    Note:         Close spi port
**  08/26/2009      Xuepeng.Zhang    Clean up
*******************************************************************************/
PUBLIC void SPI_Close (
    uint32      phy_port    // port number of spi
);

/******************************************************************************
**  Description:    This function initialize after open SPI port.
**  Author:         Jimmy.Jia
**  Note:           Save cs bits, set clock of spi,initial hardware
**  08/26/2009      Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_Init (
    uint32      phy_port    // port number of spi
);

/******************************************************************************
**  Description:    This function deinitial spi hardware.
**  Author:         Xuepeng.Zhang
**    Note:
******************************************************************************/
PUBLIC uint32 SPI_Deinit (
    uint32      phy_port    // port number of spi
);

/******************************************************************************
**  Description:    This function set cs signal of SPI port.
**  Author:         Xuepeng.Zhang
**    Note:         cs_e
**                         SPI_CS_HIGH : all cs set high level
**                         SPI_CS_LOW  : seleted cs bit in config data set low
**  08/26/2009      Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC void SPI_SetCSSignal (
    uint32      phy_port,    // port number of spi
    SPI_CSHL_E    cs_e       // cs status
);

/******************************************************************************
**  Description:    This function set cs id of SPI port.
**  Author:         Xuepeng.Zhang
**    Note:
******************************************************************************/
PUBLIC void SPI_SetCSID (
    uint32      phy_port,    // port number of spi
    SPI_CSID_E  cs_id        // cs id
);

/******************************************************************************
**  Description:    Set SPI Dividor according to bus_clk
**  Author:         Nick.Zhao
**    Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC void SPI_SetClkFreq (
    uint32      phy_port,     // port number of spi
    uint32      bus_clk       // Frequency of spi clock
);

/******************************************************************************
**  Description:    Get SPI BUS freq of spi clock
**  Author:         Nick.Zhao
**    Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_GetClkFreq (
    uint32      phy_port      // port number of spi
);

/******************************************************************************
**  Description:    Receive the char or chars by SPI
**  Author:         hanjun.liu
**  Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_Read (
    uint32      phy_port,     // port number of spi
    uint8       *buffer,      // pointer of buffer of read data
    uint32      len           // byte length to read
);

/******************************************************************************
**  Description:    Send the char or chars by SPI
**  Author:         hanjun.liu
**  Note:
**  08/26/2009      Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_Write (
    uint32          phy_port,   // port number of spi
    uint8           *buffer,    // pointer of buffer of write data
    uint32          len         // byte length to write
);

/******************************************************************************
**  Description:    Set SPI bit length
**  Author:         Nick.Zhao
**    Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC void SPI_SetBitLen (
    uint32      phy_port,     // port number of spi
    uint32      bit_len       // bit length:8,16,32
);

/******************************************************************************
**  Description:    Get SPI BUS freq of spi clock
**  Author:         Nick.Zhao
**    Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_GetBitLen (
    uint32      phy_port    // port number of spi
);
/******************************************************************************
**  Description:    Send and receive the char or chars by SPI (SPI duplex mode)
**  Author:         Wenjun.Shi
**  Note:
**
******************************************************************************/
PUBLIC uint32 SPI_Read_Write (
    uint32          phy_port,   // port number of spi
    uint8           *din_buffer_ptr,// pointer of buffer of read data
    uint8           *dout_buffer_ptr,// pointer of buffer of write data
    uint32          len         // byte length to write
);

#endif    //_SPI_DRVAPI_H_
