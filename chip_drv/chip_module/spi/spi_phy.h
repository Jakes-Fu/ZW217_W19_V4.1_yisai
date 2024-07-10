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
#ifndef _SPI_PHY_H_
#define _SPI_PHY_H_
#include "spi_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#define SPI_NOT_COMPLETE    0
#define SPI_COMPLETE        1

#define SPI_TX_COMPLETE        2
#define SPI_RX_COMPLETE        4

#define RTX_NONE  0x0
#define RX_MODE   0x1
#define TX_MODE   0x2
#define RTX_MODE  0x3

/*available spi bit length*/
#define BITLEN_8 8
#define BITLEN_16 16
#define BITLEN_32 32

#define INCREMENT_NONE     0
#define INCREMENT_BYTE     1
#define INCREMENT_HALFWORD 2
#define INCREMENT_WORD     4

#define INVALID_U16 0xffff
#define INVALID_U32 0xffffffff
/* SPI BUS ID*/
typedef enum 
{
    SPI_BUS_ID_0 = 0,
    SPI_BUS_ID_1,
    SPI_BUS_ID_2,
    SPI_BUS_ID_3,
    SPI_BUS_ID_4,
    SPI_BUS_ID_MAX
}SPI_BUS_ID_E;

/* SPI CS ID */
typedef enum 
{
    SPI_CS_ID_0 = 0,
    SPI_CS_ID_1,
    SPI_CS_ID_2,
    SPI_CS_ID_MAX
}SPI_CS_ID_E;

typedef enum
{
   SPI_PIN_GID_0 = 0, 
   SPI_PIN_GID_1, 
   SPI_PIN_GID_2,
   SPI_PIN_GID_3
}SPI_PIN_GID_E;

typedef int32 (*_spi_init) (uint32 phy_id);
typedef int32 (*_spi_cfg) (uint32 phy_id, uint32 spi_mode, uint32 tx_bit_len, uint32 freq);
typedef int32 (*_spi_write) (uint32 phy_id, uint8 *buffer_ptr, uint32 len);
typedef int32 (*_spi_read) (uint32 phy_id, uint8 *buffer_ptr, uint32 len);
typedef int32 (*_spi_rw) (uint32 phy_id, uint8 *s_buffer_ptr, uint8 *d_buffer_ptr, uint32 len);
typedef int32 (*_spi_dma_write) (uint32 phy_id, uint8 *buffer_ptr, uint32 len);
typedef int32 (*_spi_dma_read) (uint32 phy_id, uint8 *buffer_ptr, uint32 len);
typedef int32 (*_spi_dma_rw) (uint32 phy_id, uint8 *s_buffer_ptr, uint8 *d_buffer_ptr, uint32 len);
typedef int32 (*_spi_set_cs) (uint32 phy_id, uint32 cs, uint32 cs_level);
typedef int32 (*_spi_set_mode) (uint32 phy_id, uint32 mode);
typedef int32 (*_spi_get_mode) (uint32 phy_id, uint32 mode);
typedef int32 (*_spi_set_tx_bit_len) (uint32 phy_id, uint32 bit_len);
typedef int32 (*_spi_get_tx_bit_len) (uint32 phy_id);
typedef int32 (*_spi_set_freq) (uint32 phy_id, uint32 freq);
typedef int32 (*_spi_get_freq) (uint32 phy_id);
typedef int32 (*_spi_deinit) (uint32 phy_id);
typedef int32 (*_spi_set_pad) (SPI_PAD_NAME_E pin_name, uint32 pad_fun, uint32 output);
typedef int32 (*_spi_pingrp_select) (uint32 phy_id, uint32 pin_gid);
typedef int32 (*_spi_set_lcd_mode) (uint32 phy_id, uint32 mode);
typedef int32(*_spi_set_cd_level)(uint32 phy_id, BOOLEAN is_high);
typedef int32 (*_spi_get_data_port)(uint32 phy_id, uint32 *reg_base);
typedef int32 (*_spi_set_transmode) (uint32 phy_id, uint32 trans_mode);
typedef int32 (*_spi_lcd_read)(uint32 phy_id, uint32 *data_buf, uint32 data_len, uint32 dummy_len);
typedef int32 (*_spi_lcd_write)(uint32 phy_id, uint32 *data_buf, uint32 data_len, uint32 dummy_len);

typedef struct
{
    _spi_init init;
    _spi_cfg cfg;
    _spi_write write;
    _spi_read read;
    _spi_rw rw;
    _spi_dma_write dma_write;
    _spi_dma_read dma_read;
    _spi_dma_rw dma_rw;
    _spi_set_cs set_cs;
    _spi_set_mode set_mode;
    _spi_get_mode get_mode;
    _spi_set_tx_bit_len set_tx_bit_len;
    _spi_get_tx_bit_len get_tx_bit_len;
    _spi_set_freq set_freq;
    _spi_get_freq get_freq;
    _spi_deinit deinit;
    _spi_set_pad set_pad;
    _spi_write write_int;   //spi write in interrupt mode
    _spi_read read_int;     //spi read in interrupt mode
    _spi_rw rw_int;         //spi rw in interrupt mode
    _spi_dma_rw dma_rw_int; //spi dma_rw in interrupt mode
    _spi_pingrp_select pingrp_select;
#if  defined(MAINLCM_INTERFACE_SPI) || defined(SUBLCM_INTERFACE_SPI)
   _spi_set_lcd_mode set_lcd_workmode;
   _spi_set_cd_level set_cd_level;   
   _spi_get_data_port get_data_port;
   _spi_set_transmode set_trans_mode;
   _spi_lcd_read lcd_read;
   _spi_lcd_write lcd_write;
#endif	
} SPI_PHY_FUN;

//SPI error code define
typedef enum
{
    ERR_SPI_NONE = 0,
    ERR_SPI_TRX_TIMEOUT,
    ERR_SPI_INT_TIMEOUT,
    ERR_SPI_NO_MUTEX
} ERR_SPI_E;

#if defined(PLATFORM_UWS6121E)
typedef struct
{
    uint32 tx_threshold;
    uint32 rx_threshold;
} SPI_THRESHOLD_T;
#else
typedef struct
{
    uint32 tx_empty_threshold;
    uint32 tx_full_threshold;
    uint32 rx_empty_threshold;
    uint32 rx_full_threshold;
} SPI_THRESHOLD_T;
#endif

typedef struct
{
    uint32 bus_id;
    uint32 spi_rw_remain_size;
    uint32 spi_rw_last_size;
    uint8* spi_rw_tx_buf_ptr;
    uint8* spi_rw_rx_buf_ptr;
    uint32 spi_txd_flag;
    uint32 bitlen;
}SPI_RW_CTX_T;

typedef SPI_THRESHOLD_T *SPI_THRESHOLD_PTR;

/*in order to improve spi performance, we separate spi read/write 
* into two ways.
* RW_NOCHECK: 
* write a given number of words directly
* read a given number of words directly
* RW_CHECK:
* every time before spi read/write is performed, 
* check if rx/tx fifo is empty/full.
*/
typedef enum 
{
    RW_CHECK,
    RW_NOCHECK
}SPI_RW_CHECK_MODE_E;

/******************************************************************************
**  Description:    This function initialize hardware after open SPI port.
**                  Select clock source, enable the SPI model, set config reg
**  Author:         Xuepeng.Zhang
**  Note:           In this function select 26M clock as source of SPI
******************************************************************************/
PUBLIC int32 SPI_PHY_Init (
    uint32 phy_id
);

/******************************************************************************
**  Description:    This function config hardware after open SPI port.
**                  set config reg
**  Author:         Xuepeng.Zhang
**  Note: 
******************************************************************************/
PUBLIC int32 SPI_PHY_Cfg (
    uint32 phy_id,
    uint32 mode,
    uint32 tx_bit_length,
    uint32 freq
);


/******************************************************************************
**  Description:    This function deinitial SPI hardware.
**  Author:         Xuepeng.Zhang
**    Note:         1.Disable SPI irq
**                  2.Reset the SPI model
**                  3.Disable the SPI clock
**                  
******************************************************************************/
PUBLIC int32 SPI_PHY_Deinit (uint32 phy_id);



 /******************************************************************************
 **  Description:    Send data byte by byte at SPI port
 **  Author:         Xuepeng.Zhang
 **  Note:
 ******************************************************************************/
 PUBLIC int32 SPI_PHY_WriteBytes (
     uint32 phy_id,
     uint8 *buffer_ptr,
     uint32 len
 );


/******************************************************************************
**  Description:    Receive data byte by byte at SPI port
**  Author:         Xuepeng.Zhang
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_ReadBytes (
    uint32 phy_id,
    uint8 *buffer_ptr,
    uint32 len
);


/******************************************************************************
**  Description:    Receive data at SPI port and use DMA trans
**  Author:         Xuepeng.Zhang
**  Note:           The address of buffer must be aligned by 4 bytes
******************************************************************************/
PUBLIC int32 SPI_PHY_DmaRead (
    uint32   phy_id,       // port number of spi
    uint8    *buffer_ptr,    // pointer of receive data buffer
    uint32   dma_len         // byte-length of receive
);


/******************************************************************************
**  Description:    Send data at SPI port and use DMA trans
**  Author:         Xuepeng.Zhang
**  Note:           The address of buffer must be aligned by 4 bytes
******************************************************************************/
PUBLIC int32 SPI_PHY_DmaWrite (
    uint32          phy_id,     // tx channel for SPI module tansmite data
    uint8           *buffer_ptr,    // pointer of send data buffer
    uint32          dma_len         // byte-length of send data
);



/******************************************************************************
**  Description:    Send and receive data by byte at SPI port
**  Author:         Wenjun.Shi
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_ReadWriteBytes (
    uint32          phy_id,       // port number of spi
    uint8           *din_buffer_ptr,    // pointer of buffer of received data
    uint8           *dout_buffer_ptr,    // pointer of buffer of send data
    uint32          len            // byte-length to send
);



/******************************************************************************
**  Description:    Send and receive data at SPI port and use DMA trans
**  Author:         Wenjun.Shi
**  Note:           The address of buffer must be aligned by 4 bytes
******************************************************************************/
PUBLIC int32 SPI_PHY_DmaReadWrite (
    uint32          phy_id,     // port number of spi
    uint8           *din_buffer_ptr,    // pointer of received data buffer
    uint8           *dout_buffer_ptr,    // pointer of send data buffer
    uint32          dma_len         // byte-length of send data
);



/*****************************************************************************/
// Description :    This function set SPI source clock frequency by setting
//                      SPI clock devider value
// Global resource dependence : GR_GEN2
// Author :         Xuepeng.Zhang
// Note :
/*****************************************************************************/
PUBLIC int32 SPI_PHY_SetBusClk (
    uint32  phy_id,    // port number of spi
    uint32  bus_clk      // frequency of spi clock
);


/*****************************************************************************/
// Description :    This function get SPI source clock frequency by getting
//                      SPI clock devider value
// Global resource dependence : GR_GEN2
// Author :         Xuepeng.Zhang
// Note :
/*****************************************************************************/
PUBLIC int32 SPI_PHY_GetBusClk (
    uint32  phy_id                    // port number of spi
);


/*****************************************************************************/
//  Description:    This function set the CS to actived or inactived
//  Author:         xuepeng.zhang
//  Note:
/*****************************************************************************/
PUBLIC int32 SPI_PHY_SetCS (uint32  phy_id, uint32 cs_id, uint32 cs_level);



/*****************************************************************************/
// Description :    This function is used to get SPI bit length
//                  from registers.
// Global resource dependence : GR_GEN2
// Author :         hanjun.liu
//         2009/10/26 Xuepeng.Zhang clean up
// Note :
/*****************************************************************************/
PUBLIC int32 SPI_PHY_GetBitLen (
    uint32 phy_id    // port number of spi
);


/*****************************************************************************/
// Description :    This function is used to Set SPI bit length
//                  to registers.
// Global resource dependence : GR_GEN2
// Author :         hanjun.liu
//         2009/10/26 Xuepeng.Zhang clean up
// Note :
/*****************************************************************************/
PUBLIC int32 SPI_PHY_SetBitLen (
    uint32 phy_id,    // port number of spi
    uint32 bit_len      // bit length:8,16,32
);

/*********************************************************************************************************
** Function name:
** Descriptions: Switch between SPI PAD Functions(SPI/GPIO) respectively
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
PUBLIC int32 SPI_PHY_SetPAD (SPI_PAD_NAME_E pad_name, uint32 pad_fun);


/*********************************************************************************************************
** Function name:
** Descriptions: Switch between SPI PAD Functions(SPI/GPIO) respectively, if GPIO, output '0'
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
PUBLIC int32 SPI_PHY_SetPAD_with_GPIO (SPI_PAD_NAME_E pad_name, uint32 pad_fun, uint32 output);


/******************************************************************************
**  Description:    Receive data byte by byte at SPI port, using Interrupt mode
**  Author:         Xuepeng.Zhang
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_ReadBytes_Int (
    uint32 phy_id,
    uint8 *buffer_ptr,
    uint32 len
);

 /******************************************************************************
 **  Description:    Send data byte by byte at SPI port, using Interrupt mode

 **  Author:         Xuepeng.Zhang
 **  Note:
 ******************************************************************************/
 PUBLIC int32 SPI_PHY_WriteBytes_Int (
     uint32 phy_id,
     uint8 *buffer_ptr,
     uint32 len
 );
#endif

