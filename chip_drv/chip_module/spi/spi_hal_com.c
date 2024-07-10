/******************************************************************************
 ** File Name:      spi_hal.c                                                 *
 ** Author:         Jimmy.Jia                                                 *
 ** DATE:           10/28/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Jimmy.Jia        Create.                                   *
 ** 04/09/2007     David.Jia        Change from DMA to arm loop in SPI_DmaRead.
 ** 08/26/2009     Xuepeng.Zhang    Clean up
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "spi_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Mcaro Definitions                                 *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
/*                   Local Variable Declaration                              */
/*****************************************************************************/
LOCAL uint32 s_cs[MAX_SPI_PORT_NUM];        // SPI controller
extern const SPI_PHY_CFG __spi_phy_cfg[SPI_ID_MAX];

/**---------------------------------------------------------------------------*
 **                      Function Declaration                                 *
 **---------------------------------------------------------------------------*/
/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL uint32 __look_up_mode (BOOLEAN rx_shift_edge,BOOLEAN tx_shift_edge,BOOLEAN sck_reverse)
{
    uint32 mode;

    if (SCI_FALSE == sck_reverse)
    {
        if (SCI_FALSE == rx_shift_edge)
        {
            mode = 0;
        }
        else
        {
            mode = 1;
        }
    }
    else
    {
        if (SCI_FALSE == rx_shift_edge)
        {
            mode = 2;
        }
        else
        {
            mode = 3;
        }
    }

    return mode;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL uint32 __look_up_logic_id (uint32 phy_port, uint32 cs_id)
{
    uint32 i;
    uint32 logic_id = 0xffffffff;

    for (i=0; i<SPI_ID_MAX; i++)
    {
        if ( (phy_port == __spi_phy_cfg[i].phy_id) && (cs_id == __spi_phy_cfg[i].cs_id))
        {
            logic_id = __spi_phy_cfg[i].logic_id;
            break;
        }

    }

    return logic_id;
}
/******************************************************************************
**  Description:    This function Open spi port
**  Author:         Jimmy.Jia
**  08/26/2009      Xuepeng.Zhang    Clean up
*******************************************************************************/
PUBLIC uint32 SPI_Open (
    uint32          phy_port,   // port number of spi
    SPI_CONFIG_T    *spi_config // spi config data
)
{
    SPI_DEV dev;

    SCI_ASSERT (phy_port < (uint32) MAX_SPI_PORT_NUM);/*assert verified*/

    dev.id = __look_up_logic_id (phy_port, spi_config->cs_id);
    dev.mode = __look_up_mode (spi_config->rx_shift_edge,spi_config->tx_shift_edge,spi_config->sck_reverse);
    dev.tx_bit_length =  spi_config->tx_bit_length;
    dev.freq =  spi_config->freq;
    dev.spi_cs_cb = PNULL;
    dev.spi_tx_cb = PNULL;
    dev.spi_rx_cb = PNULL;
    s_cs[phy_port] = dev.id;

    SPI_HAL_Open (&dev);

    return SCI_TRUE;
}

/******************************************************************************
**  Description:    This function close spi port
**  Author:         Jimmy.Jia
**    Note:         Close spi port
**  08/26/2009      Xuepeng.Zhang    Clean up
*******************************************************************************/
PUBLIC void SPI_Close (
    uint32      phy_port    // port number of spi
)
{
    SCI_ASSERT (phy_port < (uint32) MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_Close (s_cs[phy_port]);
}

/******************************************************************************
**  Description:    This function initialize after open SPI port.
**  Author:         Jimmy.Jia
**  Note:           Save cs bits, set clock of spi,initial hardware
**  08/26/2009      Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_Init (
    uint32     phy_port     // port number of spi
)
{
    return SCI_TRUE;
}

/******************************************************************************
**  Description:    This function deinitial spi hardware.
**  Author:         Xuepeng.Zhang
**    Note:
******************************************************************************/
PUBLIC uint32 SPI_Deinit (
    uint32      phy_port    // port number of spi
)
{
    return SCI_TRUE;
}

/******************************************************************************
**  Description:    This function set cs of SPI port.
**  Author:         Xuepeng.Zhang
**    Note:         cs_e
**                         SPI_CS_HIGH : all cs set high level
**                         SPI_CS_LOW  : seleted cs bit in config data set low
**  08/26/2009      Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC void SPI_SetCSSignal (
    uint32      phy_port,    // port number of spi
    SPI_CSHL_E  cs_e         // cs status
)
{
    SCI_ASSERT (phy_port < (uint32) MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_Ioctl (s_cs[phy_port], SPI_CTL_S_CS, (uint32*)&cs_e);
}

/******************************************************************************
**  Description:    Set SPI Dividor according to bus_clk
**  Author:         Nick.Zhao
**    Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC void SPI_SetClkFreq (
    uint32      phy_port,     // port number of spi
    uint32      bus_clk       // Frequency of spi clock
)
{
    SCI_ASSERT (phy_port < (uint32) MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_Ioctl (s_cs[phy_port], SPI_CTL_S_FREQ, &bus_clk);
}

/******************************************************************************
**  Description:    Get SPI BUS freq of spi clock
**  Author:         Nick.Zhao
**    Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_GetClkFreq (
    uint32      phy_port    // port number of spi
)
{
    uint32 bus_clk=0;             // return frequency of spi clock

    SCI_ASSERT (phy_port < (uint32) MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_Ioctl (s_cs[phy_port], SPI_CTL_G_FREQ, &bus_clk);

    return bus_clk;
}

/******************************************************************************
**  Description:    Set SPI bit length
**  Author:         Nick.Zhao
**    Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC void SPI_SetBitLen (
    uint32      phy_port,     // port number of spi
    uint32      bit_len       // bit length:8,16,32
)
{
    SCI_ASSERT (phy_port < (uint32) MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_Ioctl (s_cs[phy_port], SPI_CTL_S_BITLEN, &bit_len);
}

/******************************************************************************
**  Description:    Get SPI BUS freq of spi clock
**  Author:         Nick.Zhao
**    Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_GetBitLen (
    uint32      phy_port    // port number of spi
)
{
    uint32 bit_len=0;

    SCI_ASSERT (phy_port < (uint32) MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_Ioctl (s_cs[phy_port], SPI_CTL_G_BITLEN, &bit_len);

    return bit_len;
}

/******************************************************************************
**  Description:    Receive the char or chars by SPI
**  Author:         hanjun.liu
**  Note:
**  08/26/2009     Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_Read (
    uint32      phy_port,     // port number of spi
    uint8       *buffer_ptr,  // pointer of buffer of read data
    uint32      len           // byte length to read
)
{
    SCI_ASSERT (phy_port < MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_Read (s_cs[phy_port], buffer_ptr, len);

    return SCI_TRUE;
}

/******************************************************************************
**  Description:    Send the char or chars by SPI
**  Author:         hanjun.liu
**  Note:
**  08/26/2009      Xuepeng.Zhang    Clean up
******************************************************************************/
PUBLIC uint32 SPI_Write (
    uint32          phy_port,   // port number of spi
    uint8           *buffer_ptr,// pointer of buffer of write data
    uint32          len         // byte length to write
)
{
    SCI_ASSERT (phy_port < MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_Write (s_cs[phy_port], buffer_ptr, len);

    return SCI_TRUE;
}


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
)
{
    SCI_ASSERT (phy_port < MAX_SPI_PORT_NUM);/*assert verified*/

    SPI_HAL_RW (s_cs[phy_port], din_buffer_ptr, dout_buffer_ptr, len);

    return SCI_TRUE;
}

/*EOF of spi_hal.c*/
