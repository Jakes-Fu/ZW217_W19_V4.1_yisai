/******************************************************************************
 ** File Name:      spi_slave_phy_v3.c                                                 *
 ** Author:         wenjun.shi                                                   *
 ** DATE:           12/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of SPI_SLAVE device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/27/2010     wenjun.shi     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "spi_slave_drvapi.h"
#include "../spi_slave_phy.h"
#include "timer_drvapi.h"

static uint32 spi_slave_buffer_zero = 0;
static uint32 spi_slave_buffer_dummy = 0;
static uint32 spi_slave_time_out = SPI_SLAVE_TIME_OUT;
static uint32 spi_slave_txd_flag = 0;
spi_slave_tx_callback spi_slave_tx_cb = PNULL;
spi_slave_rx_callback spi_slave_rx_cb = PNULL;
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
LOCAL void __SPI_SLAVE_DMA_Tx_CallBack (uint32 inttype)
{
    //REG32 (SPI_BASE+SPI_CTL2) &= ~SPI_DMA_EN;
    DMA_HAL_ChnEnable (SPI_SLAVE_TX_CHANNEL, SCI_FALSE);
    //SCI_TRACE_LOW ("[SPI_SLAVE DRV:]__SPI_SLAVE_DMA_Tx_CallBack");
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
LOCAL void __SPI_SLAVE_DMA_Rx_CallBack (uint32 inttype)
{
    REG32 (SPI_BASE+SPI_CTL2) &= ~SPI_DMA_EN;
    DMA_HAL_ChnEnable (SPI_SLAVE_RX_CHANNEL, SCI_FALSE);

    //SCI_TRACE_LOW ("[SPI_SLAVE DRV:]__SPI_SLAVE_DMA_Rx_CallBack");
    if (spi_slave_txd_flag) /*write*/
    {
        if (spi_slave_tx_cb != PNULL)
        {
            spi_slave_tx_cb();
        }
        else
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }
    else/*read*/
    {
        if (spi_slave_rx_cb != PNULL)
        {
            spi_slave_rx_cb();
        }
        else
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }
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
LOCAL __inline void __SPI_SLAVE_Dump_Reg ()
{
    uint32 j;

    for (j = 1; j < 19; j++)
    {
        SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]PHY:reg0x%x=0x%x", (SPI_BASE+j*4), REG32 ( (SPI_BASE+j*4)));
    }
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
LOCAL __inline void __SPI_SLAVE_CFG_MCLK_MAX ()
{
    REG32 (GR_CLK_DLY) &=~ (BIT_26 | BIT_27);
    REG32 (GR_GEN2) &=~ (BIT_21 | BIT_22 | BIT_23);

    //REG32 (GR_CLK_DLY) |= (BIT_26 | BIT_27);
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
LOCAL __inline void __SPI_SLAVE_RST_FIFO ()
{
    REG32 (SPI_BASE+SPI_SLAVE_FIFO_RST) |= (BIT_0);
    REG32 (SPI_BASE+SPI_SLAVE_FIFO_RST) &=~ (BIT_0);
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
LOCAL __inline int32 __SPI_SLAVE_JUDGE_TIME_OUT (uint32 time_tick)
{
    int32 ret = ERR_SPI_SLAVE_NONE;

    if ( (TIMER_GetSystemCounterReg()- time_tick) >= spi_slave_time_out)
    {
        ret = -ERR_SPI_SLAVE_TIMEOUT;
    }

    return ret;
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
PUBLIC void SPI_SLAVE_PHY_Init (uint32 mode, uint32 tx_bit_length, spi_slave_tx_callback tx_callback, spi_slave_rx_callback rx_callback)
{
    uint32 rtl0 = 0;

    /*adjust SPI working clock*/
    __SPI_SLAVE_CFG_MCLK_MAX();

    /*reset SPI*/
    REG32 (GR_SOFT_RST) |= BIT_14;
    REG32 (GR_SOFT_RST) &= ~BIT_14;

    /*enable SPI clock*/
    REG32 (GR_GEN0) |= GEN0_SPI_EN;

    /*set mode*/
    switch (mode)
    {
        case 0:
            rtl0 |= BIT_1;
            break;
        case 1:
            rtl0 |= BIT_0;
            break;
        case 2:
            rtl0 |= BIT_13;
            rtl0 |= BIT_1;
            break;
        case 3:
            rtl0 |= BIT_13;
            rtl0 |= BIT_0;
            break;
        default:
            SCI_ASSERT (SCI_FALSE);/*assert verified*/
            break;
    }

    /*set bit len*/
    rtl0 |= (tx_bit_length & 0x1F) << 2;

    /*set cs high*/
    rtl0 |= (0xF << 8);

    /*set ctl0*/
    REG32 (SPI_BASE+SPI_SLAVE_CTL0) = rtl0;

    /*set duplex mode*/
    REG32 (SPI_BASE+SPI_SLAVE_CTL1) |= (BIT_12 | BIT_13);

    /*set slave mode*/
    REG32 (SPI_BASE+SPI_SLAVE_CTL2) |= BIT_5;

    /*set recv threshold*/
    REG32 (SPI_BASE+SPI_SLAVE_CTL3) = SPI_SLAVE_RX_WATERMARK;
    //REG32 (SPI_BASE+SPI_SLAVE_CTL3) &= ~BIT_4;
    //REG32 (SPI_BASE+SPI_SLAVE_CTL3) |= BIT_0;

    /*set fast mode and delay*/
    REG32 (SPI_BASE+SPI_SLAVE_CTL4) = SPI_SLAVE_FAST_MODE;

    // transmit data interval clk
    REG32 (SPI_BASE+SPI_SLAVE_CTL5) = SPI_SLAVE_ITVL_NUM;

    /*set tx threshold*/
    REG32 (SPI_BASE+SPI_SLAVE_CTL6) = SPI_SLAVE_TX_WATERMARK;

    /*Disable interrupt here*/
    REG32 (SPI_BASE+SPI_SLAVE_INT_IEN) = 0;

    spi_slave_tx_cb = tx_callback;
    spi_slave_rx_cb = rx_callback;
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
PUBLIC void SPI_SLAVE_PHY_Exit (void)
{
    /*disable SPI_SLAVE*/
    REG32 (GR_GEN0) &= ~GEN0_SPI_EN;
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
LOCAL int32 SPI_SLAVE_PHY_WaitForBusIdle (void)
{
    uint32 time_out_cnt = 0;
    int32 ret = ERR_SPI_SLAVE_NONE;

    /*check really empey flag*/
    time_out_cnt = TIMER_GetSystemCounterReg();

    while (! (REG32 (SPI_BASE+SPI_SLAVE_STS2) & ( (uint32) SPI_SLAVE_TX_FIFO_REALLY_EMPTY)))
    {
        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }
    }

    /*wait few clocks skip the gap: max 2*clk_spi_slave+pclk*/
    /*dummy read, make sure spi_slave bus is idle*/
    REG32 (SPI_BASE+SPI_SLAVE_STS2);


    /*check shifter chain is not busy*/
    time_out_cnt = TIMER_GetSystemCounterReg();

    while (REG32 (SPI_BASE+SPI_SLAVE_STS2) & ( (uint32) SPI_SLAVE_TX_BUSY))
    {
        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }
    }

    return ret;
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
PUBLIC int32 SPI_SLAVE_PHY_WriteBytes (uint8 *buffer_ptr, uint32 len)
{
    uint32 bitlen;
    uint32 time_out_cnt = 0;
    uint32 write_bytes_cnt = 0;
    int32 ret = ERR_SPI_SLAVE_NONE;

    __SPI_SLAVE_RST_FIFO();

    bitlen = (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_CTL0) >>2) &0x1F;

    time_out_cnt = TIMER_GetSystemCounterReg();

    while (write_bytes_cnt < len)
    {
        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }

        if ( (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_STS2) & ( (uint32) SPI_SLAVE_TX_FIFO_FULL)) == 0)
        {
            switch (bitlen)
            {
                case 8:
                    REG32 (SPI_BASE+SPI_SLAVE_TXD) = * ( (uint8 *) (&buffer_ptr[write_bytes_cnt]));
                    write_bytes_cnt +=1;
                    break;
                case 16:
                    REG32 (SPI_BASE+SPI_SLAVE_TXD) = * ( (uint16 *) (&buffer_ptr[write_bytes_cnt]));
                    write_bytes_cnt +=2;
                    break;
                case 0: /*32bit mode*/
                    REG32 (SPI_BASE+SPI_SLAVE_TXD) = * ( (uint32 *) (&buffer_ptr[write_bytes_cnt]));
                    write_bytes_cnt +=4;
                    break;
                default:
                    SCI_ASSERT (0);/*assert to do*/
                    break;
            }
        }
    }

    /*wait for all of data send out*/
    ret = SPI_SLAVE_PHY_WaitForBusIdle ();

    return ret;
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
PUBLIC int32 SPI_SLAVE_PHY_ReadBytes (uint8 *buffer_ptr, uint32 len)
{
    uint32 bitlen;
    uint32 time_out_cnt;
    uint32 read_bytes_cnt = 0;
    int32 ret = ERR_SPI_SLAVE_NONE;

    bitlen = (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_CTL0) >>2) &0x1F;

    /*set receive only mode*/
    time_out_cnt = TIMER_GetSystemCounterReg();

    while (read_bytes_cnt < len)
    {
        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }

        if ( (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_STS2) & SPI_SLAVE_RX_FIFO_REALLY_EMPTY) == 0)
        {
            switch (bitlen)
            {
                case 8:
                    * ( (uint8 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    read_bytes_cnt +=1;
                    break;
                case 16:
                    * ( (uint16 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    read_bytes_cnt +=2;
                    break;
                case 0: /*32bit mode*/
                    * ( (uint32 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    read_bytes_cnt +=4;
                    break;
                default:
                    SCI_ASSERT (0);/*assert to do*/
                    break;
            }

        }
    }

    return ret;
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
PUBLIC int32 SPI_SLAVE_PHY_ReadWriteBytes (uint8 *buffer_ptr, uint32 len, uint32 mode)
{
    uint32 bitlen;
    uint32 time_out_cnt;
    uint32 read_bytes_cnt = 0;
    uint32 write_bytes_cnt = 0;
    int32 ret = ERR_SPI_SLAVE_NONE;

    __SPI_SLAVE_RST_FIFO();

    bitlen = (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_CTL0) >>2) &0x1F;
    time_out_cnt = TIMER_GetSystemCounterReg();

    while (write_bytes_cnt < len)
    {
        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }

        if ( (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_STS2) & ( (uint32) SPI_SLAVE_TX_FIFO_FULL)) == 0)
        {
            switch (bitlen)
            {
                case 8:

                    if (mode)
                    {
                        REG32 (SPI_BASE+SPI_SLAVE_TXD) = * ( (uint8 *) (&buffer_ptr[write_bytes_cnt]));
                    }
                    else
                    {
                        REG32 (SPI_BASE+SPI_SLAVE_TXD) = 0x0;
                    }

                    write_bytes_cnt +=1;
                    break;
                case 16:

                    if (mode)
                    {
                        REG32 (SPI_BASE+SPI_SLAVE_TXD) = * ( (uint16 *) (&buffer_ptr[write_bytes_cnt]));
                    }
                    else
                    {
                        REG32 (SPI_BASE+SPI_SLAVE_TXD) = 0x0;
                    }

                    write_bytes_cnt +=2;
                    break;
                case 0:     /*32bit mode*/

                    if (mode)
                    {
                        REG32 (SPI_BASE+SPI_SLAVE_TXD) = * ( (uint32 *) (&buffer_ptr[write_bytes_cnt]));
                    }
                    else
                    {
                        REG32 (SPI_BASE+SPI_SLAVE_TXD) = 0x0;
                    }

                    write_bytes_cnt +=4;
                    break;
                default:
                    SCI_ASSERT (0);/*assert to do*/
                    break;
            }
        }

        if ( (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_STS2) & SPI_SLAVE_RX_FIFO_REALLY_EMPTY) == 0)
        {
            switch (bitlen)
            {
                case 8:

                    if (!mode)
                    {
                        * ( (uint8 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }
                    else
                    {
                        CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }

                    read_bytes_cnt +=1;
                    break;
                case 16:

                    if (!mode)
                    {
                        * ( (uint16 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }
                    else
                    {
                        CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }

                    read_bytes_cnt +=2;
                    break;
                case 0:     /*32bit mode*/

                    if (!mode)
                    {
                        * ( (uint32 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }
                    else
                    {
                        CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }

                    read_bytes_cnt +=4;
                    break;
                default:
                    SCI_ASSERT (0);/*assert to do*/
                    break;
            }
        }
    }

    time_out_cnt = TIMER_GetSystemCounterReg();

    while (read_bytes_cnt < len)
    {
        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }

        if ( (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_STS2) & SPI_SLAVE_RX_FIFO_REALLY_EMPTY) == 0)
        {
            switch (bitlen)
            {
                case 8:

                    if (!mode)
                    {
                        * ( (uint8 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }
                    else
                    {
                        CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }

                    read_bytes_cnt +=1;
                    break;
                case 16:

                    if (!mode)
                    {
                        * ( (uint16 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }
                    else
                    {
                        CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }

                    read_bytes_cnt +=2;
                    break;
                case 0:     /*32bit mode*/

                    if (!mode)
                    {
                        * ( (uint32 *) (&buffer_ptr[read_bytes_cnt])) = CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }
                    else
                    {
                        CHIP_REG_GET (SPI_BASE+SPI_SLAVE_TXD);
                    }

                    read_bytes_cnt +=4;
                    break;
                default:
                    SCI_ASSERT (0);/*assert verified*/
                    break;
            }
        }
    }

    return ret;
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
PUBLIC int32 SPI_SLAVE_PHY_DmaWrite (uint8 *buffer_ptr, uint32 dma_len)
{
    uint32 time_out_cnt;
    uint32 tx_src_datawidth;
    uint32 tx_dest_datawidth;
    uint32 tx_src_step;
    DMA_CHNCFG_T s_tx_dma_cfg = {0};
    volatile uint32 i;
    uint32 bitlen;
    uint32 burst_size;
    DMA_CAHCECONFIG_T cacheConfig[1];
    int32 ret = ERR_SPI_SLAVE_NONE;

    __SPI_SLAVE_RST_FIFO();

    bitlen = (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_CTL0) >>2) &0x1F;

    switch (bitlen)
    {
        case 8:
            tx_dest_datawidth = DMA_DATAWIDTH_BYTE;
            burst_size = SPI_SLAVE_BURST_SIZE/4;
            break;
        case 16:
            tx_dest_datawidth = DMA_DATAWIDTH_HALFWORD;
            burst_size = SPI_SLAVE_BURST_SIZE/2;
            break;
        case 0:
            tx_dest_datawidth = DMA_DATAWIDTH_WORD;
            burst_size = SPI_SLAVE_BURST_SIZE;
            break;
        default:
            SCI_ASSERT (SCI_FALSE);/*assert verified*/
            break;
    }

    if (0 == ( ( (uint32) buffer_ptr) % 2))
    {
        if (0 == ( ( (uint32) buffer_ptr) % 4))
        {
            tx_src_datawidth = DMA_DATAWIDTH_WORD;
            tx_src_step = 4;
        }
        else
        {
            tx_src_datawidth = DMA_DATAWIDTH_HALFWORD;
            tx_src_step = 2;
        }
    }
    else
    {
        tx_src_datawidth = DMA_DATAWIDTH_BYTE;
        tx_src_step = 1;
    }

    DMA_HAL_ChnIntConfig (SPI_SLAVE_TX_CHANNEL, DMA_CHN_INT_TYPE_NONE, NULL);
    //DMA_HAL_ChnIntConfig (SPI_SLAVE_TX_CHANNEL, DMA_CHN_INT_TYPE_TRANSDONE, __SPI_SLAVE_DMA_Tx_CallBack);

    s_tx_dma_cfg.type           = DMA_CHN_TYPE_HARD;
    s_tx_dma_cfg.priority       = DMA_CHN_PRIORITY_1;
    s_tx_dma_cfg.workmode       = DMA_CHN_WORKMODE_NORMAL;
    s_tx_dma_cfg.src_datawidth  = tx_src_datawidth;
    s_tx_dma_cfg.dest_datawidth = tx_dest_datawidth;
    s_tx_dma_cfg.src_address    = (uint32) buffer_ptr;
    s_tx_dma_cfg.dest_address   = (uint32) (SPI_BASE+SPI_TXD);
    s_tx_dma_cfg.requestmode    = DMA_CHN_REQMODE_NORMAL;
    s_tx_dma_cfg.block_length   = burst_size;
    s_tx_dma_cfg.total_length   = dma_len;
    s_tx_dma_cfg.src_step       = tx_src_step;
    s_tx_dma_cfg.dest_step      = 0;
    s_tx_dma_cfg.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    s_tx_dma_cfg.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;
#ifdef CHIP_ENDIAN_LITTLE
    //s_tx_dma_cfg.endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#endif
    DMA_HAL_ChnConfig (SPI_SLAVE_TX_CHANNEL, &s_tx_dma_cfg);

    cacheConfig[0].bufferAddr = (uint32) s_tx_dma_cfg.src_address;
    cacheConfig[0].bufferSize = s_tx_dma_cfg.total_length;
    cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
    DMA_HAL_ChnEnable_Ex (SPI_SLAVE_TX_CHANNEL, SCI_TRUE, cacheConfig, 1);
    //DMA_HAL_ChnEnable (SPI_SLAVE_TX_CHANNEL, SCI_TRUE);

    REG32 (SPI_BASE+SPI_CTL2) |= SPI_DMA_EN;

    time_out_cnt = TIMER_GetSystemCounterReg();

    while (!DMA_HAL_PollingTransDone (SPI_SLAVE_TX_CHANNEL))
    {
        for (i=0; i<100; i++) {};

        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }
    }

    DMA_HAL_ChnEnable (SPI_SLAVE_TX_CHANNEL, SCI_FALSE);
    REG32 (SPI_BASE+SPI_CTL2) &= ~SPI_DMA_EN;

    /*wait untill the Tx FIFO is empty*/
    SPI_SLAVE_PHY_WaitForBusIdle ();

    return ret;
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
PUBLIC int32 SPI_SLAVE_PHY_DmaRead (uint8 *buffer_ptr, uint32 dma_len)
{
    uint32 time_out_cnt;
    volatile uint32 i;
    DMA_CHNCFG_T s_rx_dma_cfg = {0};
    uint32 bitlen;
    uint32 rx_src_datawidth;
    uint32 burst_size;
    DMA_CAHCECONFIG_T cacheConfig[1];
    int32 ret = ERR_SPI_SLAVE_NONE;

    //__SPI_SLAVE_RST_FIFO();

    bitlen = (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_CTL0) >>2) &0x1F;

    switch (bitlen)
    {
        case 8:
            rx_src_datawidth = DMA_DATAWIDTH_BYTE;
            burst_size = SPI_SLAVE_BURST_SIZE/4;
            break;
        case 16:
            rx_src_datawidth = DMA_DATAWIDTH_HALFWORD;
            burst_size = SPI_SLAVE_BURST_SIZE/2;
            break;
        case 0:     /*32bit mode*/
            rx_src_datawidth = DMA_DATAWIDTH_WORD;
            burst_size = SPI_SLAVE_BURST_SIZE;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    DMA_HAL_ChnIntConfig (SPI_SLAVE_RX_CHANNEL, DMA_CHN_INT_TYPE_NONE, NULL);
    //DMA_HAL_ChnIntConfig (SPI_SLAVE_RX_CHANNEL, DMA_CHN_INT_TYPE_TRANSDONE, __SPI_SLAVE_DMA_Rx_CallBack);

    s_rx_dma_cfg.type           = DMA_CHN_TYPE_HARD;
    s_rx_dma_cfg.priority       = DMA_CHN_PRIORITY_3;
    s_rx_dma_cfg.workmode       = DMA_CHN_WORKMODE_NORMAL;
    s_rx_dma_cfg.src_datawidth  = rx_src_datawidth;
    s_rx_dma_cfg.dest_datawidth = DMA_DATAWIDTH_WORD;
    s_rx_dma_cfg.src_address    = (uint32) (SPI_BASE+SPI_TXD);
    s_rx_dma_cfg.dest_address   = (uint32) buffer_ptr;
    s_rx_dma_cfg.requestmode    = DMA_CHN_REQMODE_NORMAL;
    s_rx_dma_cfg.block_length   = burst_size;
    s_rx_dma_cfg.total_length   = dma_len;
    s_rx_dma_cfg.src_step       = 0;
    s_rx_dma_cfg.dest_step      = 4;
    s_rx_dma_cfg.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    s_rx_dma_cfg.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;
#ifdef CHIP_ENDIAN_LITTLE
    //s_rx_dma_cfg.endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#endif

    DMA_HAL_ChnConfig (SPI_SLAVE_RX_CHANNEL, &s_rx_dma_cfg);

    cacheConfig[0].bufferAddr = (uint32) buffer_ptr;
    cacheConfig[0].bufferSize = dma_len;
    cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
    DMA_HAL_ChnEnable_Ex (SPI_SLAVE_RX_CHANNEL, SCI_TRUE, cacheConfig, 1);
    //DMA_HAL_ChnEnable (SPI_SLAVE_RX_CHANNEL, SCI_TRUE);

    REG32 (SPI_BASE+SPI_CTL2) |= BIT_6;

    time_out_cnt = TIMER_GetSystemCounterReg();

    while (!DMA_HAL_PollingTransDone (SPI_SLAVE_RX_CHANNEL))
    {
        for (i=0; i<100; i++) {}

        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }
    }

    DMA_HAL_ChnEnable (SPI_SLAVE_RX_CHANNEL, SCI_FALSE);

    REG32 (SPI_BASE+SPI_CTL2) &= ~SPI_DMA_EN;

    return ret;
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
PUBLIC int32 SPI_SLAVE_PHY_DmaReadWrite (uint8 *buffer_ptr, uint32 dma_len, uint32 mode)
{
    uint32 tx_src_step;
    uint32 tx_des_step;
    uint32 rx_src_step;
    uint32 rx_des_step;
    DMA_CHNCFG_T s_tx_dma_cfg = {0};
    DMA_CHNCFG_T s_rx_dma_cfg = {0};
    DMA_CAHCECONFIG_T cacheConfig[1];
    uint8 *din_buffer_ptr;
    uint8 *dout_buffer_ptr;
    int32 ret = ERR_SPI_SLAVE_NONE;
    uint32 bitlen;
    uint32 burst_size;
    uint32 rx_src_datawidth;
    uint32 tx_des_datawidth;
#if 0
    volatile uint32 i;
    uint32 time_out_cnt;
#endif

    __SPI_SLAVE_RST_FIFO();

    bitlen = (CHIP_REG_GET (SPI_BASE+SPI_SLAVE_CTL0) >>2) &0x1F;

    switch (bitlen)
    {
        case 8:
            tx_des_datawidth = rx_src_datawidth = DMA_DATAWIDTH_BYTE;
            burst_size = SPI_SLAVE_BURST_SIZE/4;
            break;
        case 16:
            tx_des_datawidth = rx_src_datawidth = DMA_DATAWIDTH_HALFWORD;
            burst_size = SPI_SLAVE_BURST_SIZE/2;
            break;
        case 0:     /*32bit mode*/
            tx_des_datawidth = rx_src_datawidth = DMA_DATAWIDTH_WORD;
            burst_size = SPI_SLAVE_BURST_SIZE;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    spi_slave_txd_flag = mode;

    if (mode) /*write*/
    {
        dout_buffer_ptr = buffer_ptr;
        din_buffer_ptr = (uint8 *) (&spi_slave_buffer_dummy);
        tx_src_step = 4;
        tx_des_step = 0;
        rx_src_step = 0;
        rx_des_step = 0;
    }
    else/*read*/
    {
        dout_buffer_ptr = (uint8 *) (&spi_slave_buffer_zero);
        din_buffer_ptr = buffer_ptr;
        tx_src_step = 0;
        tx_des_step = 0;
        rx_src_step = 0;
        rx_des_step = 4;
    }

    /*config SPI TX DMA*/
    //DMA_HAL_ChnIntConfig (SPI_SLAVE_TX_CHANNEL, DMA_CHN_INT_TYPE_NONE, NULL);
    DMA_HAL_ChnIntConfig (SPI_SLAVE_TX_CHANNEL, DMA_CHN_INT_TYPE_TRANSDONE, __SPI_SLAVE_DMA_Tx_CallBack);

    s_tx_dma_cfg.type           = DMA_CHN_TYPE_HARD;
    s_tx_dma_cfg.priority       = DMA_CHN_PRIORITY_1;
    s_tx_dma_cfg.workmode       = DMA_CHN_WORKMODE_NORMAL;
    s_tx_dma_cfg.src_datawidth  = DMA_DATAWIDTH_WORD;
    s_tx_dma_cfg.dest_datawidth = tx_des_datawidth;
    s_tx_dma_cfg.src_address    = (uint32) dout_buffer_ptr;
    s_tx_dma_cfg.dest_address   = (uint32) (SPI_BASE+SPI_TXD);
    s_tx_dma_cfg.requestmode    = DMA_CHN_REQMODE_NORMAL;
    s_tx_dma_cfg.block_length   = burst_size;
    s_tx_dma_cfg.total_length   = dma_len;
    s_tx_dma_cfg.src_step       = tx_src_step;
    s_tx_dma_cfg.dest_step      = tx_des_step;
    s_tx_dma_cfg.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    s_tx_dma_cfg.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;
#ifdef CHIP_ENDIAN_LITTLE
    //s_tx_dma_cfg.endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#endif
    DMA_HAL_ChnConfig (SPI_SLAVE_TX_CHANNEL, &s_tx_dma_cfg);

    /*config SPI RX DMA*/
    //DMA_HAL_ChnIntConfig (SPI_SLAVE_RX_CHANNEL, DMA_CHN_INT_TYPE_NONE, NULL);
    DMA_HAL_ChnIntConfig (SPI_SLAVE_RX_CHANNEL, DMA_CHN_INT_TYPE_TRANSDONE, __SPI_SLAVE_DMA_Rx_CallBack);

    s_rx_dma_cfg.type           = DMA_CHN_TYPE_HARD;
    s_rx_dma_cfg.priority       = DMA_CHN_PRIORITY_3;
    s_rx_dma_cfg.workmode       = DMA_CHN_WORKMODE_NORMAL;
    s_rx_dma_cfg.src_datawidth  = rx_src_datawidth;
    s_rx_dma_cfg.dest_datawidth = DMA_DATAWIDTH_WORD;
    s_rx_dma_cfg.src_address    = (uint32) (SPI_BASE+SPI_TXD);
    s_rx_dma_cfg.dest_address   = (uint32) din_buffer_ptr;
    s_rx_dma_cfg.requestmode    = DMA_CHN_REQMODE_NORMAL;
    s_rx_dma_cfg.block_length   = burst_size;
    s_rx_dma_cfg.total_length   = dma_len;
    s_rx_dma_cfg.src_step       = rx_src_step;
    s_rx_dma_cfg.dest_step      = rx_des_step;
    s_rx_dma_cfg.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    s_rx_dma_cfg.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;
#ifdef CHIP_ENDIAN_LITTLE
    //s_rx_dma_cfg.endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#endif
    DMA_HAL_ChnConfig (SPI_SLAVE_RX_CHANNEL, &s_rx_dma_cfg);

    cacheConfig[0].bufferAddr = (uint32) s_tx_dma_cfg.src_address;
    cacheConfig[0].bufferSize = s_tx_dma_cfg.total_length;
    cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
    DMA_HAL_ChnEnable_Ex (SPI_SLAVE_TX_CHANNEL, SCI_TRUE, cacheConfig, 1);
    cacheConfig[0].bufferAddr = (uint32) s_rx_dma_cfg.dest_address;
    cacheConfig[0].bufferSize = s_rx_dma_cfg.total_length;
    cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
    DMA_HAL_ChnEnable_Ex (SPI_SLAVE_RX_CHANNEL, SCI_TRUE, cacheConfig, 1);
    //DMA_HAL_ChnEnable (SPI_SLAVE_TX_CHANNEL, SCI_TRUE);
    //DMA_HAL_ChnEnable (SPI_SLAVE_RX_CHANNEL, SCI_TRUE);

    REG32 (SPI_BASE+SPI_CTL2) |= SPI_DMA_EN;

#if 0
    time_out_cnt = TIMER_GetSystemCounterReg();

    while (!DMA_HAL_PollingTransDone (SPI_SLAVE_TX_CHANNEL))
    {
        for (i=0; i<100; i++) {};

        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }
    }

    time_out_cnt = TIMER_GetSystemCounterReg();

    while (!DMA_HAL_PollingTransDone (SPI_SLAVE_RX_CHANNEL))
    {
        for (i=0; i<100; i++) {};

        if ( (ret = __SPI_SLAVE_JUDGE_TIME_OUT (time_out_cnt)) < 0)
        {
            break;
        }
    }

    DMA_HAL_ChnEnable (SPI_SLAVE_RX_CHANNEL, SCI_FALSE);
    DMA_HAL_ChnEnable (SPI_SLAVE_TX_CHANNEL, SCI_FALSE);

    REG32 (SPI_BASE+SPI_CTL2) &= ~SPI_DMA_EN;
#endif

    return ret;
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
PUBLIC void SPI_SLAVE_PHY_S_TIME_OUT (uint32 tick)
{
    spi_slave_time_out = tick;
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
PUBLIC void SPI_SLAVE_RST_FIFO ()
{
    __SPI_SLAVE_RST_FIFO();
}

PUBLIC SPI_SLAVE_PHY_FUN spi_slave_phy_fun =
{
    SPI_SLAVE_PHY_Init,
    SPI_SLAVE_PHY_Exit,
    SPI_SLAVE_PHY_ReadWriteBytes,
    SPI_SLAVE_PHY_WriteBytes,
    SPI_SLAVE_PHY_ReadBytes,
    SPI_SLAVE_PHY_DmaReadWrite,
    SPI_SLAVE_PHY_DmaRead,
    SPI_SLAVE_PHY_DmaWrite,
    SPI_SLAVE_PHY_S_TIME_OUT,
    SPI_SLAVE_RST_FIFO
};

