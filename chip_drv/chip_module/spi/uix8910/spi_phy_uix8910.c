/******************************************************************************
 ** File Name:      spi_phy_v0.c                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of SPI device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.
 ** 12/27/2010     wenjun.shi     Modify.                                     *
 ** 05/30/2011     wenjun.shi     Add Interrupt mode interfaces, include:     *
 **                                      SPI_PHY_WriteBytes_Int()             *
 **                                      SPI_PHY_ReadBytes_Int()              *
 **                                      SPI_PHY_ReadWriteBytes_Int()         *
 **                                      SPI_PHY_DmaReadWrite_Int()           *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "spi_drvapi.h"
#include "../spi_phy.h"
//#include "dma_drv_internal.h"
#include "clock_drvapi.h"
#include "gpio_drvapi.h"
#include "ldo_drvapi.h"
#include "spi_reg_uix8910.h"
#include "spi_phy_uix8910.h"
#include "apifc_drvapi.h"

#define  SPI_THRESHOLD_1DATA       0
#define  SPI_THRESHOLD_4DATA       1
#define  SPI_THRESHOLD_8DATA       2
#define  SPI_THRESHOLD_12DATA      3

#ifndef GPIO_SIMULATE_SPI_SUPPORT
LOCAL DEVICE_HANDLE s_dev_spi[SPI_BUS_MAX] = {SCI_NULL};
#else
LOCAL DEVICE_HANDLE s_dev_spi[SPI_BUS_MAX] = {SCI_NULL,SCI_NULL};
#endif
LOCAL uint32 spi_buffer_zero = 0;
LOCAL uint32 spi_buffer_dummy = INVALID_U32; // For CSR:  need MOSI to be HIGH during all read operations. Perhaps a configuration parameter can be introduced here?

LOCAL SCI_TIMER_PTR spi_rx_timer = SCI_NULL;
PUBLIC _spi_callback spi_tx_callback = PNULL;
PUBLIC _spi_callback spi_rx_callback = PNULL;

/* spi dma context */
PUBLIC SPI_RW_CTX_T s_spi_dma_ctx =
{
    INVALID_U32, //bus_id
    0,          //spi_rw_remain_size
    0,          //spi_rw_last_size
    PNULL,      //spi_rw_tx_buf_ptr
    PNULL,      //spi_rw_rx_buf_ptr
    RTX_NONE,   //spi_txd_flag
    0           //bitlen
};
/* spi irq context */
PUBLIC SPI_RW_CTX_T s_spi_irq_ctx =
{
    INVALID_U32, //bus_id
    0,          //spi_rw_remain_size
    0,          //spi_rw_last_size
    PNULL,      //spi_rw_tx_buf_ptr
    PNULL,      //spi_rw_rx_buf_ptr
    RTX_NONE,   //spi_txd_flag
    0           //bitlen
};


/* Structure for storing cached data about the SPI controller. This has two purposes:
- Enable indexed access to the SPI_BASE_INFO associated with the SPI controller.
- Allow caching of values for the control registers so they do not need to be read before modifying. */
typedef struct
{
    SPI_BASE_INFO *phy_info;
    uint32 ctrl;
    uint32 cfg;
    uint32 irq;
} SPI_PHY_INST;

#if 0
// =============================================================================
// HAL_SPI_ACTIVE_T
// -----------------------------------------------------------------------------
/// Type used to remember the activation state of the SPI.
// =============================================================================
typedef enum
{    
/// Active CS0 to CS3 are the first one to   
/// keep compatibility with the HAL_SPI_CS_T type.  
HAL_SPI_ACTIVE_CS_0,    
HAL_SPI_ACTIVE_CS_1,  
HAL_SPI_ACTIVE_CS_2,   
HAL_SPI_ACTIVE_CS_3,   
HAL_SPI_ACTIVE_NONE,  
/// To avoid race and several things done simultaneously. 
HAL_SPI_OPERATION_IN_PROGRESS,  
HAL_SPI_ACTIVE_QTY
} HAL_SPI_ACTIVE_T;
// =============================================================================
// HAL_SPI_SETTINGS_T
// -----------------------------------------------------------------------------
/// Type used to remember the SPI configuration for each Chip Select.
// =============================================================================
typedef VOLATILE struct
{   
UINT32       ctrl;   
UINT32       cfg;   
HAL_SPI_TRANSFERT_MODE_T rxMode; 
///< Reception transfer mode   
HAL_SPI_TRANSFERT_MODE_T txMode;
///< Emission transfer mode   
/// Desired freq  
UINT32      freq;  
/// Number of bytes per frame. Used to check that we only   
/// send complete frames.   
UINT32      bytesPerFrame;  
HAL_SPI_IRQ_HANDLER_T irqHandler;  
/// Image of the irq mask register  
UINT32      irqMask;

}HAL_SPI_SETTINGS_T;

// =============================================================================
// HAL_SPI_PROP_T
// -----------------------------------------------------------------------------
/// This type is used to remember of all the properties of each SPI present in
/// the system, and its driver status.
// =============================================================================
typedef struct
{ 
/// Configuration per CS  
HAL_SPI_SETTINGS_T          cfg[HAL_SPI_CS_MAX_QTY];  
/// Control Register picture   
UINT32                      ctrl;   
VOLATILE UINT8              rxIfcCh;  
VOLATILE UINT8              txIfcCh;  
HAL_SPI_TRANSFERT_MODE_T    rxMode;  
HAL_SPI_TRANSFERT_MODE_T    txMode;  
// The SPI availability is represented by the activeStatus variable.   
// A CS Activation must use a critical section to check and set   
// (if possible) the activeStatus and thus become activated.   
// The Deactivation mechanism relies on the same mechanism.  
HAL_SPI_ACTIVE_T            activeStatus;   
// Used to store the user irq handler   
HAL_SPI_IRQ_HANDLER_T       irqHandler;  
// Used to store required frequency  
//HAL_SYS_FREQ_T              requiredSysFreq;
} HAL_SPI_PROP_T;
#endif

CONST HAL_APIFC_REQUEST_ID_T gc_halSpiIfcTxId[HAL_SPI_QTY] =
{
    HAL_IFC_SPI1_TX,   
    HAL_IFC_SPI2_TX,
};
CONST HAL_APIFC_REQUEST_ID_T gc_halSpiIfcRxId[HAL_SPI_QTY] =
{
    HAL_IFC_SPI1_RX,  
    HAL_IFC_SPI2_RX,
};

/*List of SPI_PHY_INST device instances, indexed by phy_id.*/
static SPI_PHY_INST spi_phy_inst[SPI_BUS_MAX];
/*Macro to initialize the base value properly*/
#define __SPI_PHY_SetPhyInstBase(phy_id, phy_inf) (spi_phy_inst[(phy_id)].phy_info = (phy_inf))
/*Macro to access the SPI PHY instance by index*/
#define __SPI_PHY_GetPhyInst(phy_id) (&spi_phy_inst[(phy_id)])

extern const SPI_PAD_INFO __spi_pad_func_info[];
extern SPI_BASE_INFO __spi_base_info[SPI_BUS_MAX];


/*********************************************************************************************************
** Function name:
** Descriptions: Perform byte reversal on an array of longword.
**               0xABCD => 0xDCBA, corresponding to DMA Full Switch Mode
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL __inline void __ByteSwap32 (
    uint8 *x, uint32 xlen
)
{
    uint8 a;
    uint32 i;

    for (i = 0; i < xlen; i+=4)
    {
        a = x[i];
        x[i] = x[i+3];
        x[i+3] = a;
        a = x[i+1];
        x[i+1] = x[i+2];
        x[i+2] = a;
    }
}
/*********************************************************************************************************
** Function name:
** Descriptions: Perform byte reversal on an array of longword.
**               0xABCD => 0xBADC, corresponding to DMA Switch Mode 0
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL __inline void __ByteSwap16 (
    uint8 *x, uint32 xlen
)
{
    uint8 a;
    uint32 i;

    for (i = 0; i < xlen; i+=2)
    {
        a = x[i];
        x[i] = x[i+1];
        x[i+1] = a;
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
PUBLIC __inline int32 __SPI_PHY_GetBase (
    SPI_BASE_INFO *phy_info
)
{
    int32 i;

    for (i = 0; i < SPI_BUS_MAX; i++)
    {
        if (phy_info->phy_id == (uint32) __spi_base_info[i].phy_id)
        {
            __SPI_PHY_SetPhyInstBase (phy_info->phy_id, &__spi_base_info[i]); //Index the found device in the SPI PHY instance
            phy_info->base_addr = (uint32) __spi_base_info[i].base_addr;
            phy_info->tx_channel = (uint16) __spi_base_info[i].tx_channel;
            phy_info->rx_channel = (uint16) __spi_base_info[i].rx_channel;
            break;
        }
    }

    return ERR_SPI_NONE;
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
PUBLIC  uint32  __SPI_SetFifoThreshold (uint32 phy_id, SPI_THRESHOLD_PTR spi_threshold_ptr)
{
	SPI_PHY_INST *phy_inst;
	HWP_SPI_T *p_hwp_spi;

	/*Retrieve register cache*/
	phy_inst = __SPI_PHY_GetPhyInst (phy_id);
	p_hwp_spi = (HWP_SPI_T *)(phy_inst->phy_info->base_addr);

	/*set spi rx threshold*/
	phy_inst->irq = p_hwp_spi->irq;
	phy_inst->irq &= ~(SPI_TX_THRESHOLD(3));
	phy_inst->irq |= SPI_TX_THRESHOLD(spi_threshold_ptr->tx_threshold);
	/*set spi tx threshold*/
	phy_inst->irq &= ~(SPI_RX_THRESHOLD(3));
	phy_inst->irq |= SPI_RX_THRESHOLD(spi_threshold_ptr->rx_threshold);
	p_hwp_spi->irq = phy_inst->irq;

	return ERR_SPI_NONE;
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
LOCAL __inline int32 __SPI_Dump_Reg ()
{
    uint32 j;

    for (j = 1; j < 16; j++)
    {
        SPI_PRINT ( ("[SPI DRV:]PHY:reg0x%x=0x%x", (0x8e000000+j*4), CHIP_REG_GET ( (0x8e000000+j*4))));
    }

    return ERR_SPI_NONE;
}


/*********************************************************************************************************
** Function name:
** Descriptions: write words to tx fifo
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL __inline int32 __SPI_PHY_WBytes (
    uint32          phy_id,              // port number of spi
    uint8           *dout_buffer_ptr,    // pointer of buffer of send data
    uint32          len,                  // byte-length to send
    SPI_RW_CHECK_MODE_E rw_chk_mode          // check mode when read/write
)
{

    uint32 bitlen;
    uint32 time_out_cnt   =0;
    uint32 write_bytes_cnt=0;           // byte-conter to send
    uint32 i;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
	
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    p_hwp_spi = (HWP_SPI_T *)phy_inst->phy_info->base_addr;
    bitlen = SPI_PHY_GetBitLen (phy_id);
    SPI_PRINT ( ("__SPI_PHY_WBytes: phy_id=%d, dout_buffer_ptr=%d, len=%d",
                 phy_id, dout_buffer_ptr, len));

    while (write_bytes_cnt < len)
    {
        time_out_cnt++;

        if (time_out_cnt > SPI_PHY_POLL_TIMEOUT)
        {
            SCI_PASSERT (0, ("ctrl=0x%x,cfg=0x%x,status=0x%x",/*assert verified*/
                             p_hwp_spi->ctrl, p_hwp_spi->cfg, p_hwp_spi->status));
        }

        /*check is not TX fifo full?*/
        if ( (RW_NOCHECK == rw_chk_mode) || ( p_hwp_spi->status & SPI_TX_OVF) == 0)
        {
            if (PNULL == dout_buffer_ptr) // spi read only
            {
                switch (bitlen)
                {
                    case BITLEN_8:     // 8bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            p_hwp_spi->rxtx_buffer = * ( (uint8 *) (&spi_buffer_dummy));
                            write_bytes_cnt +=INCREMENT_BYTE;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_BYTE; i++)
                            {
                                p_hwp_spi->rxtx_buffer = * ( (uint8 *) (&spi_buffer_dummy));
                                write_bytes_cnt +=INCREMENT_BYTE;
                            }
                        }

                        break;
                    case BITLEN_16:    // 16bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            p_hwp_spi->rxtx_buffer = * ( (uint16 *) (&spi_buffer_dummy));
                            write_bytes_cnt +=INCREMENT_HALFWORD;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_HALFWORD; i++)
                            {
                                p_hwp_spi->rxtx_buffer = * ( (uint16 *) (&spi_buffer_dummy));
                                write_bytes_cnt +=INCREMENT_HALFWORD;
                            }
                        }

                        break;
                    case BITLEN_32:     // 32bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            p_hwp_spi->rxtx_buffer = * ( (uint32 *) (&spi_buffer_dummy));
                            write_bytes_cnt +=INCREMENT_WORD;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_WORD; i++)
                            {
                                p_hwp_spi->rxtx_buffer = * ( (uint32 *) (&spi_buffer_dummy));
                                write_bytes_cnt +=INCREMENT_WORD;
                            }
                        }

                        break;
                    default:
                        SCI_ASSERT (0);/*assert to do*/
                        break;

                }
            }
            else
            {
                switch (bitlen)
                {
                    case BITLEN_8:     // 8bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            p_hwp_spi->rxtx_buffer = * ( (uint8 *) (&dout_buffer_ptr[write_bytes_cnt]));
                            write_bytes_cnt +=INCREMENT_BYTE;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_BYTE; i++)
                            {
                                p_hwp_spi->rxtx_buffer = * ( (uint8 *) (&dout_buffer_ptr[write_bytes_cnt]));
                                write_bytes_cnt +=INCREMENT_BYTE;
                            }
                        }

                        break;
                    case BITLEN_16:    // 16bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
#ifdef CHIP_ENDIAN_LITTLE
                            __ByteSwap16 (&dout_buffer_ptr[write_bytes_cnt], INCREMENT_HALFWORD);
#endif
                            p_hwp_spi->rxtx_buffer = * ( (uint16 *) (&dout_buffer_ptr[write_bytes_cnt]));
                            write_bytes_cnt +=INCREMENT_HALFWORD;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
#ifdef CHIP_ENDIAN_LITTLE
                            __ByteSwap16 (&dout_buffer_ptr[write_bytes_cnt], len);
#endif

                            for (i = 0; i < len/INCREMENT_HALFWORD; i++)
                            {
                                p_hwp_spi->rxtx_buffer = * ( (uint16 *) (&dout_buffer_ptr[write_bytes_cnt]));
                                write_bytes_cnt +=INCREMENT_HALFWORD;
                            }
                        }

                        break;
                    case BITLEN_32:     // 32bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
#ifdef CHIP_ENDIAN_LITTLE
                            __ByteSwap32 (&dout_buffer_ptr[write_bytes_cnt], INCREMENT_WORD);
#endif
                            p_hwp_spi->rxtx_buffer = * ( (uint32 *) (&dout_buffer_ptr[write_bytes_cnt]));
                            write_bytes_cnt +=INCREMENT_WORD;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
#ifdef CHIP_ENDIAN_LITTLE
                            __ByteSwap32 (&dout_buffer_ptr[write_bytes_cnt], len);
#endif

                            for (i = 0; i < len/INCREMENT_WORD; i++)
                            {
                                p_hwp_spi->rxtx_buffer = * ( (uint32 *) (&dout_buffer_ptr[write_bytes_cnt]));
                                write_bytes_cnt +=INCREMENT_WORD;
                            }
                        }

                        break;
                    default:
                        SCI_ASSERT (0);/*assert to do*/
                        break;
                }
            }
        }
    }

    return ERR_SPI_NONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions: read words from rx fifo
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL __inline int32 __SPI_PHY_RBytes (
    uint32          phy_id,              // port number of spi
    uint8           *din_buffer_ptr,     // pointer of buffer of send data
    uint32          len,                  // byte-length to send
    SPI_RW_CHECK_MODE_E rw_chk_mode          // check mode when read/write
)
{
    uint32 spi_addr;
    uint32 bitlen;
    uint32 time_out_cnt   =0;
    uint32 read_bytes_cnt=0;           // byte count to receive
    uint32 i;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    p_hwp_spi = (HWP_SPI_T *)phy_inst->phy_info->base_addr;
    bitlen = SPI_PHY_GetBitLen (phy_id);
    SPI_PRINT ( ("__SPI_PHY_RBytes: phy_id=%d, din_buffer_ptr=%d, len=%d",
                 phy_id, din_buffer_ptr, len));

    while (read_bytes_cnt < len)
    {
        time_out_cnt++;

        if (time_out_cnt > SPI_PHY_POLL_TIMEOUT)
        {
            SCI_PASSERT (0, ("ctrl=0x%x,cfg=0x%x,status=0x%x",/*assert verified*/
                             p_hwp_spi->ctrl, p_hwp_spi->cfg, p_hwp_spi->status));
        }

        /*check if RX_FIFO real empty, to prevent invalid read*/
        if ( (RW_NOCHECK == rw_chk_mode) || ( (p_hwp_spi->status & SPI_RX_LEVEL_MASK) != 0))
        {
            if (PNULL == din_buffer_ptr) // spi write only
            {
                switch (bitlen)
                {
                    case BITLEN_8:     // 8bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            * ( (uint8 *) (&spi_buffer_zero)) = p_hwp_spi->rxtx_buffer;
                            read_bytes_cnt +=INCREMENT_BYTE;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_BYTE; i++)
                            {
                                * ( (uint8 *) (&spi_buffer_zero)) = p_hwp_spi->rxtx_buffer;
                                read_bytes_cnt +=INCREMENT_BYTE;
                            }
                        }

                        break;
                    case BITLEN_16:    // 16bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            * ( (uint16 *) (&spi_buffer_zero)) = p_hwp_spi->rxtx_buffer;
                            read_bytes_cnt +=INCREMENT_HALFWORD;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_HALFWORD; i++)
                            {
                                * ( (uint16 *) (&spi_buffer_zero)) = p_hwp_spi->rxtx_buffer;
                                read_bytes_cnt +=INCREMENT_HALFWORD;
                            }
                        }

                        break;
                    case BITLEN_32:     // 32bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            * ( (uint32 *) (&spi_buffer_zero)) = p_hwp_spi->rxtx_buffer;
                            read_bytes_cnt +=INCREMENT_WORD;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_WORD; i++)
                            {
                            * ( (uint32 *) (&spi_buffer_zero)) = p_hwp_spi->rxtx_buffer;
                                read_bytes_cnt +=INCREMENT_WORD;
                            }
                        }

                        break;
                    default:
                        SCI_ASSERT (0);/*assert to do*/
                        break;
                }
            }
            else
            {
                switch (bitlen)
                {
                    case BITLEN_8:     // 8bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            * ( (uint8 *) (&din_buffer_ptr)) = p_hwp_spi->rxtx_buffer;
                            read_bytes_cnt +=INCREMENT_BYTE;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_BYTE; i++)
                            {
                                * ( (uint8 *) (&din_buffer_ptr)) = p_hwp_spi->rxtx_buffer;
                                read_bytes_cnt +=INCREMENT_BYTE;
                            }
                        }

                        break;
                    case BITLEN_16:    // 16bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            * ( (uint16 *) (&din_buffer_ptr)) = p_hwp_spi->rxtx_buffer;
#ifdef CHIP_ENDIAN_LITTLE
                            __ByteSwap16 (&din_buffer_ptr[read_bytes_cnt], INCREMENT_HALFWORD);
#endif
                            read_bytes_cnt +=INCREMENT_HALFWORD;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_HALFWORD; i++)
                            {
                                * ( (uint16 *) (&din_buffer_ptr)) = p_hwp_spi->rxtx_buffer;
                                read_bytes_cnt +=INCREMENT_HALFWORD;
                            }

#ifdef CHIP_ENDIAN_LITTLE
                            __ByteSwap16 (&din_buffer_ptr[read_bytes_cnt - len], len);
#endif
                        }

                        break;
                    case BITLEN_32:     // 32bit mode

                        if (RW_CHECK == rw_chk_mode)
                        {
                            * ( (uint32 *) (&din_buffer_ptr)) = p_hwp_spi->rxtx_buffer;
#ifdef CHIP_ENDIAN_LITTLE
                            __ByteSwap32 (&din_buffer_ptr[read_bytes_cnt], INCREMENT_WORD);
#endif
                            read_bytes_cnt +=INCREMENT_WORD;
                        }
                        else if (RW_NOCHECK == rw_chk_mode)
                        {
                            for (i = 0; i < len/INCREMENT_WORD; i++)
                            {
                                * ( (uint32 *) (&din_buffer_ptr)) = p_hwp_spi->rxtx_buffer;
                                read_bytes_cnt +=INCREMENT_WORD;
                            }

#ifdef CHIP_ENDIAN_LITTLE
                            __ByteSwap32 (&din_buffer_ptr[read_bytes_cnt - len], len);
#endif
                        }

                        break;
                    default:
                        SCI_ASSERT (0);/*assert to do*/
                        break;
                }
            }
        }
    }

    return ERR_SPI_NONE;
}


/*********************************************************************************************************
** Function name:
** Descriptions: If words reside in rx fifo couldn't reach rx full watermark,  a timer will be activated  to
**               read the remaining words in rx fifo. This function will be called when timer get time out.
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __SPI_RxTimerHandle (uint32 arg)
{
#if 0
    uint32 spi_addr;
    uint32 rx_fifo_addr;
    uint32 rx_r_addr = 0;
    uint32 rx_w_addr = 0;
    SPI_PHY_INST *phy_inst;

    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (s_spi_irq_ctx.bus_id);
    spi_addr = phy_inst->phy_info->base_addr;
    SCI_DeactiveTimer (spi_rx_timer);
    {
        BOOLEAN active          = 0;
        uint32  remaining_time  = 0;
        uint32  reschedule_time = 0;
        char    name[SCI_MAX_NAME_SIZE];

        SCI_GetTimerInfo (spi_rx_timer, name, &active, &remaining_time,
                          &reschedule_time);
    }
    rx_fifo_addr =  CHIP_REG_GET (spi_addr + SPI_STS1);
    rx_r_addr = rx_fifo_addr & SPI_FIFO_ADDR_MASK;
    rx_w_addr = (rx_fifo_addr >> SPI_FIFO_RX_W_ADDR_OFFSET) & SPI_FIFO_ADDR_MASK;
    if ( ( ( (rx_w_addr >= rx_r_addr? rx_w_addr:rx_w_addr+SPI_FIFO_SIZE) - rx_r_addr) * (s_spi_irq_ctx.bitlen) /8)
            == s_spi_irq_ctx.spi_rw_remain_size)
    {
        /*read remain words from rx fifo */
        __SPI_PHY_RBytes (s_spi_irq_ctx.bus_id, s_spi_irq_ctx.spi_rw_tx_buf_ptr, s_spi_irq_ctx.spi_rw_remain_size, RW_NOCHECK);
        s_spi_irq_ctx.spi_rw_tx_buf_ptr += s_spi_irq_ctx.spi_rw_remain_size;
        s_spi_irq_ctx.spi_rw_remain_size = 0;
        (spi_rx_callback) (PNULL);
    }
    else
    {
        SCI_ChangeTimer (spi_rx_timer, __SPI_RxTimerHandle, SPI_RX_INTERVAL_MS);
        SCI_ActiveTimer (spi_rx_timer);
    }
#endif
}
/*********************************************************************************************************
** Function name:
** Descriptions: This function will be called when spi rx dma complete.
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __SPI_DmaRxHandle (uint32 inttype)
{
    uint32 spi_addr;
    uint32 tx_channel;                    // dma chaanel number for spi transmmit
    uint32 rx_channel;                    // dma chaanel number for spi receive

    uint32 rx_fifo_addr;
    uint32 rx_r_addr;
    uint32 rx_w_addr;
    SPI_PHY_INST *phy_inst;

    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (s_spi_dma_ctx.bus_id);
    spi_addr = phy_inst->phy_info->base_addr;
    rx_channel = phy_inst->phy_info->rx_channel;
    tx_channel = phy_inst->phy_info->tx_channel;

    if (s_spi_dma_ctx.spi_txd_flag & TX_MODE) /*write*/
    {
        if (s_spi_dma_ctx.spi_rw_remain_size > 0)
        {
            SPI_PRINT ( ("__SPI_DmaRxHandle: tx_buf_ptr:%d, remain_size:%d", s_spi_dma_ctx.spi_rw_tx_buf_ptr, s_spi_dma_ctx.spi_rw_remain_size));
            SPI_PHY_WriteBytes_Int (s_spi_dma_ctx.bus_id, s_spi_dma_ctx.spi_rw_tx_buf_ptr, s_spi_dma_ctx.spi_rw_remain_size);
            s_spi_dma_ctx.spi_rw_remain_size = 0;
            s_spi_dma_ctx.spi_rw_tx_buf_ptr = NULL;
        }
        else
        {
            (spi_tx_callback) (PNULL);
        }
    }
    else if (s_spi_dma_ctx.spi_txd_flag & RX_MODE) /*read*/
    {
        if (s_spi_dma_ctx.spi_rw_remain_size > 0)
        {
            //SPI_PRINT(("__SPI_DmaRxHandle: rx_buf_ptr:%d, remain_size:%d", s_spi_dma_ctx.spi_rw_rx_buf_ptr, s_spi_dma_ctx.spi_rw_remain_size));
            //rx_fifo_addr =  CHIP_REG_GET (spi_addr + SPI_STS1);
            
            //rx_r_addr = rx_fifo_addr & SPI_FIFO_ADDR_MASK;
            //rx_w_addr = (rx_fifo_addr >> SPI_FIFO_RX_W_ADDR_OFFSET) & SPI_FIFO_ADDR_MASK;
            //SPI_PRINT ( ("__SPI_DmaRxHandle: remain_size:%d, rx_raddr:%d, rx_waddr:%d", s_spi_dma_ctx.spi_rw_remain_size , rx_r_addr, rx_w_addr));
            SPI_PHY_ReadBytes_Int (s_spi_dma_ctx.bus_id, s_spi_dma_ctx.spi_rw_rx_buf_ptr, s_spi_dma_ctx.spi_rw_remain_size);
            s_spi_dma_ctx.spi_rw_remain_size = 0;
            s_spi_dma_ctx.spi_rw_rx_buf_ptr = NULL;
        }
        else
        {
            (spi_rx_callback) (PNULL);
        }
    }
}


/*********************************************************************************************************
** Function name:
** Descriptions: This function will be called when spi rw transfer complete.
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ISR_EXE_T __SPI_RWIrqHandle (uint32 irqnum)
{
    uint32 spi_addr;
    SPI_THRESHOLD_T spi_threshold;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (s_spi_irq_ctx.bus_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    
    do
    {
        /*read words from rx fifo*/
        __SPI_PHY_RBytes (s_spi_irq_ctx.bus_id, s_spi_irq_ctx.spi_rw_rx_buf_ptr, s_spi_irq_ctx.spi_rw_last_size, RW_CHECK);
        s_spi_irq_ctx.spi_rw_remain_size -= s_spi_irq_ctx.spi_rw_last_size;
        SCI_ASSERT (s_spi_irq_ctx.spi_rw_remain_size < INVALID_U16); /*assert to do*/

        if (0 == s_spi_irq_ctx.spi_rw_remain_size)
        {
            /*spi write*/
            if ( (s_spi_irq_ctx.spi_rw_tx_buf_ptr != PNULL) && (spi_tx_callback != PNULL))
            {
                (spi_tx_callback) (PNULL);
            }

            /*spi read*/
            if ( (s_spi_irq_ctx.spi_rw_rx_buf_ptr != PNULL) && (spi_rx_callback != PNULL))
            {
                (spi_rx_callback) (PNULL);
            }

            break;
        }

        if (s_spi_irq_ctx.spi_rw_tx_buf_ptr != PNULL)
        {
            s_spi_irq_ctx.spi_rw_tx_buf_ptr += s_spi_irq_ctx.spi_rw_last_size;
        }

        if (s_spi_irq_ctx.spi_rw_rx_buf_ptr != PNULL)
        {
            s_spi_irq_ctx.spi_rw_rx_buf_ptr += s_spi_irq_ctx.spi_rw_last_size;
        }

        /*calculate word counts to xfer*/
        s_spi_irq_ctx.spi_rw_last_size = s_spi_irq_ctx.spi_rw_remain_size  > (SPI_THRHOLD_CNT * (s_spi_irq_ctx.bitlen/BITLEN_8)) ?
                                         (SPI_THRHOLD_CNT * (s_spi_irq_ctx.bitlen/BITLEN_8)) : s_spi_irq_ctx.spi_rw_remain_size;
        /*set rx, tx threshold*/
        spi_threshold.rx_threshold  = s_spi_irq_ctx.spi_rw_last_size / (s_spi_irq_ctx.bitlen/BITLEN_8);
        spi_threshold.tx_threshold = 0;
        __SPI_SetFifoThreshold (s_spi_irq_ctx.bus_id, &spi_threshold);

        /*enable rx fifo full int*/
        p_hwp_spi->irq |= SPI_MASK_RX_TH_IRQ;
        __SPI_PHY_WBytes (s_spi_irq_ctx.bus_id, s_spi_irq_ctx.spi_rw_tx_buf_ptr, s_spi_irq_ctx.spi_rw_last_size, RW_CHECK);
    }
    while (0);

    SPI_PRINT ( ("__SPI_RWIrqHandle: bus_id:%d, tx_buf_ptr:%d, rx_buf_ptr:%d, last_size:%d, remain_size:%d",
                 s_spi_irq_ctx.bus_id, s_spi_irq_ctx.spi_rw_tx_buf_ptr,
                 s_spi_irq_ctx.spi_rw_rx_buf_ptr, s_spi_irq_ctx.spi_rw_last_size,
                 s_spi_irq_ctx.spi_rw_remain_size));

    return ISR_DONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions: This function will be called when spi tx fifo empty.
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ISR_EXE_T __SPI_TxIrqHandle (uint32 irqnum)
{
    uint32 cur_cnt;
    uint32 spi_addr;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (s_spi_irq_ctx.bus_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    
    do
    {
        SCI_ASSERT (s_spi_irq_ctx.spi_rw_remain_size > 0); /*assert to do*/
        /*have at least one more tx empty interrupt*/
        if (s_spi_irq_ctx.spi_rw_remain_size > (SPI_THRHOLD_CNT_CMPT * (s_spi_irq_ctx.bitlen/BITLEN_8)))
        {
            cur_cnt = (SPI_THRHOLD_CNT_CMPT * (s_spi_irq_ctx.bitlen/BITLEN_8));
            __SPI_PHY_WBytes (s_spi_irq_ctx.bus_id, s_spi_irq_ctx.spi_rw_tx_buf_ptr, cur_cnt, RW_NOCHECK);
            s_spi_irq_ctx.spi_rw_tx_buf_ptr  += cur_cnt;
            s_spi_irq_ctx.spi_rw_remain_size -= cur_cnt;
            /*enable tx fifo empty int*/
            p_hwp_spi->irq |= SPI_MASK_TX_TH_IRQ;
            break;
        }
        /*no need tx empty interrupt*/
        else
        {
            cur_cnt = s_spi_irq_ctx.spi_rw_remain_size;
            __SPI_PHY_WBytes (s_spi_irq_ctx.bus_id, s_spi_irq_ctx.spi_rw_tx_buf_ptr, cur_cnt, RW_NOCHECK);
            s_spi_irq_ctx.spi_rw_tx_buf_ptr  += cur_cnt;
            s_spi_irq_ctx.spi_rw_remain_size -= cur_cnt;
            (spi_tx_callback) (PNULL);
            break;
        }
    }
    while (0);

    return ISR_DONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions: This function will be called when spi rx fifo full.
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ISR_EXE_T __SPI_RxIrqHandle (uint32 irqnum)
{
    uint32 cur_cnt;
    uint32 spi_addr;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (s_spi_irq_ctx.bus_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    SPI_PRINT ( ("__SPI_RxIrqHandle: bus_id:%d, tx_buf_ptr:%d, rx_buf_ptr:%d, last_size:%d, remain_size:%d",
                 s_spi_irq_ctx.bus_id, s_spi_irq_ctx.spi_rw_tx_buf_ptr,
                 s_spi_irq_ctx.spi_rw_rx_buf_ptr, s_spi_irq_ctx.spi_rw_last_size,
                 s_spi_irq_ctx.spi_rw_remain_size));
    
    cur_cnt = SPI_THRHOLD_CNT* (s_spi_irq_ctx.bitlen/BITLEN_8);

    do
    {
        SCI_ASSERT (s_spi_irq_ctx.spi_rw_remain_size > 0); /*assert to do*/
        /* read SPI_THRHOLD_CNT's size of data */
        __SPI_PHY_RBytes (s_spi_irq_ctx.bus_id, s_spi_irq_ctx.spi_rw_tx_buf_ptr, cur_cnt, RW_NOCHECK);
        s_spi_irq_ctx.spi_rw_tx_buf_ptr += cur_cnt;
        s_spi_irq_ctx.spi_rw_remain_size -= cur_cnt;

        if (s_spi_irq_ctx.spi_rw_remain_size  == 0)
        {
            (spi_rx_callback) (PNULL);
            break;
        }

        if (s_spi_irq_ctx.spi_rw_remain_size >= (SPI_THRHOLD_CNT * (s_spi_irq_ctx.bitlen/BITLEN_8)))
        {
            /*enable rx fifo full int*/
            p_hwp_spi->irq |= SPI_MASK_RX_TH_IRQ;
        }
        else
        {
            SPI_PRINT ( ("__SPI_RxIrqHandle: SCI_ActiveTimer, remain_size:%d", s_spi_irq_ctx.spi_rw_remain_size));
            SCI_ChangeTimer (spi_rx_timer, __SPI_RxTimerHandle, SPI_RX_INTERVAL_MS);
            SCI_ActiveTimer (spi_rx_timer);

        }
    }
    while (0);


    return ISR_DONE;
}

/******************************************************************************
**  Description:    This function bind DMA channel and SPI tx/rx request.
**
**  Author:         Xuepeng.Zhang
**  Note:
******************************************************************************/

LOCAL void __SPI_PHY_BindingDmaChannel (uint32 channel, uint32 uid)
{
#if 0
#if !defined(PLATFORM_UWS6121E)
    uint32 reqx_cid_baseaddr = DMA_CTL_REG_BASE + 0x2000 + (uid) *0x04;
#else
    uint32 reqx_cid_baseaddr = REG_DMA_BASE + 0x40 + (uid) *0x04;
#endif
    CHIP_REG_SET (reqx_cid_baseaddr, channel);
#endif
}

// =============================================================================
// hal_SpiIrqHandler
// -----------------------------------------------------------------------------
/// Handler called in case of SPI interrupt
////// @param interruptId  Will be spi Hw module interrupt
/// id when used by the IRQ driver.
// =============================================================================
VOID SPI_PHY_IRQHandler(uint32 interruptId)
{   
    // Under the hypothesis that several SPI    
    // would have their IRQ lines in a consecutive   
    // order ... (First id is 0, potential SPI2 is 1)  
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    uint8 id; 
    union  
    {        
        UINT32 reg;       
        HAL_SPI_IRQ_STATUS_T bitfield; 
    } cause;  
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (s_spi_irq_ctx.bus_id);
    p_hwp_spi = (HWP_SPI_T *)phy_inst->phy_info->base_addr;
    
    id = interruptId - TB_SPI0_INT;   
    SCI_PASSERT(id < HAL_SPI_QTY, ("Improper SPI id %d ! \n"  " Your system only has %d SPI", id, HAL_SPI_QTY));   

    // cause.reg contains a copy of the status register  
    cause.reg = p_hwp_spi->status;   
    // clear all cause  
    p_hwp_spi->status = cause.reg;  
    // cause.reg is masked and shift to contain only  
    // the IRQ cause bitfield 
    cause.reg = GET_BITFIELD(cause.reg, SPI_IRQ_CAUSE);   
    if (cause.bitfield.rxDmaDone)  
    {     
        phy_inst->phy_info->rx_channel = HAL_UNKNOWN_CHANNEL;  
        /* Disable rx DMA channel*/
        hal_ApIfcChannelRelease(gc_halSpiIfcRxId[id], phy_inst->phy_info->rx_channel);
        __SPI_DmaRxHandle(interruptId);
    }   
    else if (cause.bitfield.rxTh)
    {
        p_hwp_spi->irq &= ~SPI_MASK_RX_TH_IRQ;
        __SPI_RxIrqHandle(interruptId);
    }
    if (cause.bitfield.txDmaDone)  
    {     
        phy_inst->phy_info->tx_channel = HAL_UNKNOWN_CHANNEL;   
        /* Disable tx DMA channel*/
        hal_ApIfcChannelRelease(gc_halSpiIfcTxId[id], phy_inst->phy_info->tx_channel);
        __SPI_DmaRxHandle(interruptId);
    }
    else if (cause.bitfield.txTh)
    {
        p_hwp_spi->irq &= ~SPI_MASK_TX_TH_IRQ;
        __SPI_TxIrqHandle(interruptId);
    }
    #if 0
    // Call the user IRQ handler function. 
    if (g_halSpiPropArray[id].irqHandler)  
    {     
        g_halSpiPropArray[id].irqHandler(cause.bitfield);  
    }
    #endif
}
/******************************************************************************
**  Description:    This function initialize hardware after open SPI port.
**                  Select clock source, enable the SPI model, set config reg
**  Author:         Xuepeng.Zhang
**  Note:           In this function select 26M clock as source of SPI
******************************************************************************/
PUBLIC int32 SPI_PHY_Init (
    uint32 phy_id
)
{
    uint32 spi_addr;
    SPI_BASE_INFO phy_info;
    SPI_THRESHOLD_T spi_threshold;
    SPI_PHY_INST *phy_inst;

    phy_info.phy_id = phy_id;
    __SPI_PHY_GetBase (&phy_info);
    spi_addr = phy_info.base_addr;
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);

    /*Get SPI device handle, enable spi*/
    switch (phy_id)
    {
        case SPI_BUS_ID_0:

            if (CLOCK_SUCCESS != DEVICE_Find ("SPI0", & (s_dev_spi[phy_id])))
            {
                SCI_ASSERT (0); /*assert to do*/
            }

            break;
        case SPI_BUS_ID_1:

            if (CLOCK_SUCCESS != DEVICE_Find ("SPI1", & (s_dev_spi[phy_id])))
            {
                SCI_ASSERT (0); /*assert to do*/
            }

            break;
        default:
            SCI_ASSERT (0); /*assert to do*/
    }

    DEVICE_Open (s_dev_spi[phy_id]);

    /*recv threshold, tx threshold, dma disable*/
    spi_threshold.rx_threshold = SPI_THRESHOLD_8DATA;
    spi_threshold.tx_threshold  = SPI_THRESHOLD_1DATA;
    __SPI_SetFifoThreshold (phy_id, &spi_threshold);

    spi_rx_timer = SCI_CreateTimer ("SPI_RX_TIMER",
                                    __SPI_RxTimerHandle, 0, SPI_RX_INTERVAL_MS, SCI_NO_ACTIVATE);
    return ERR_SPI_NONE;
}

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
)
{
    SPI_BASE_INFO phy_info;
    //SPI_THRESHOLD_T spi_threshold;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;

    phy_info.phy_id = phy_id;
    __SPI_PHY_GetBase (&phy_info);
    p_hwp_spi = (HWP_SPI_T *)phy_info.base_addr;

    /*Initialize cache of control registers. These registers will from now on be referenced through the phy_inst structure.*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);

    phy_inst->ctrl = p_hwp_spi->ctrl;
    phy_inst->cfg = p_hwp_spi->cfg;
    phy_inst->irq = p_hwp_spi->irq;
    //reserve cs bits, in case of spi controller multiplexing
    //phy_inst->ctl0 &= (SPI_CS_MASK << SPI_CS_OFFSET);

    /*set mode*/
    //CPOL && CPHA mode
    phy_inst->ctrl &= ~(SPI_CLOCK_POLARITY | SPI_CLOCK_DELAY(3) | SPI_DO_DELAY(3) | SPI_DI_DELAY(3) | SPI_CS_DELAY(3));
    switch (mode)
    {
        case 0:
            phy_inst->ctrl |= SPI_CLOCK_DELAY(2) |SPI_DO_DELAY(1) | SPI_DI_DELAY(0) | SPI_CS_DELAY(1);
            break;
        case 1:
            phy_inst->ctrl |= SPI_CLOCK_POLARITY | SPI_CLOCK_DELAY(2) |SPI_DO_DELAY(1) | SPI_DI_DELAY(0) | SPI_CS_DELAY(1);
            break;
        case 2:
            phy_inst->ctrl |= SPI_CLOCK_DELAY(1) |SPI_DO_DELAY(1) | SPI_DI_DELAY(1) | SPI_CS_DELAY(0);
            break;
        case 3:
            phy_inst->ctrl |= SPI_CLOCK_POLARITY | SPI_CLOCK_DELAY(1) |SPI_DO_DELAY(1) | SPI_DI_DELAY(1) | SPI_CS_DELAY(0);
            break;
        default:
            SCI_ASSERT (SCI_FALSE);/*assert to do*/
            break;
    }

    /*set bit len*/
    phy_inst->ctrl |= SPI_FRAME_SIZE(tx_bit_length - 1);

    /*Disable interrupt here*/
    p_hwp_spi->irq &= ~SPI_IRQ_MASK_MASK;
	
    /*set ctl*/
    p_hwp_spi->ctrl = phy_inst->ctrl;	

    /*set idle mode*/
    //phy_inst->ctl1 &= ~ (SPI_RX_MODE | SPI_TX_MODE);
    //CHIP_REG_SET (spi_addr+SPI_CTL1, phy_inst->ctl1);
    phy_inst->ctrl &= ~SPI_ENABLE;
    
    //phy_inst->ctl4 = 0x8000;
    //CHIP_REG_SET (spi_addr+SPI_CTL4, phy_inst->ctl4);

    /*transmit data interval 0 clk*/
    //CHIP_REG_SET (spi_addr+SPI_CTL5, 0);

    /*config clock*/
    SPI_PHY_SetBusClk (phy_id, freq);

    return ERR_SPI_NONE;
}


/******************************************************************************
**  Description:    This function deinitial SPI hardware.
**  Author:         Xuepeng.Zhang
**    Note:         1.Disable SPI irq
**                  2.Reset the SPI model
**                  3.Disable the SPI clock
**
******************************************************************************/
PUBLIC int32 SPI_PHY_Deinit (uint32 phy_id)
{
    uint32 i;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
	
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    p_hwp_spi = (HWP_SPI_T *)phy_inst->phy_info->base_addr;

    /*Disable irq*/
    p_hwp_spi->irq &= ~SPI_IRQ_MASK_MASK;
	
    /* Reset spi*/
    switch (phy_id)
    {
        case SPI_BUS_ID_0:
		hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SPI1;
            for (i=0; i<10; i++)
            {
                ;
            }
		hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SPI1;
            break;
        case SPI_BUS_ID_1:
		hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SPI2;

            for (i=0; i<10; i++)
            {
                ;
            }

		hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SPI2;
            break;
        default:
            SCI_ASSERT (0); /*assert to do*/
            break;
    }

    /*Disable spi*/
    DEVICE_Close (s_dev_spi[phy_id]);
    /* Set SPI_CS, SPI_DO to GPIO, output '0'*/

    /* de-binding DMA channel and REQ id */
    __SPI_PHY_BindingDmaChannel (0, phy_inst->phy_info->tx_req_id);
    __SPI_PHY_BindingDmaChannel (0, phy_inst->phy_info->rx_req_id);

    DMA_HAL_FreeChn (phy_inst->phy_info->tx_channel);
    DMA_HAL_FreeChn (phy_inst->phy_info->rx_channel);

    SCI_DeleteTimer (spi_rx_timer);
    spi_rx_timer = SCI_NULL;

    return ERR_SPI_NONE;
}

/******************************************************************************
**  Description:    This function will be wait until SPI become to Idle status.
**  Author:         Xuepeng.Zhang
**    Note:         1. check really empey flag
**                  2. wait few clocks skip the gap
**                  3. check shifte register chain is not busy
******************************************************************************/
LOCAL int32 SPI_PHY_WaitForBusIdle (uint32 spi_add)
{
    uint32 time_out_cnt = 0;
    volatile uint32 dummy;
    HWP_SPI_T *p_hwp_spi;

    p_hwp_spi = (HWP_SPI_T *)spi_add;
#if 0
    if(CHIP_REG_GET(spi_add+SPI_CTL7) & BIT_15)
    {
        while(1) {/*lint !e716*/
            if(CHIP_REG_GET (spi_add+SPI_INT_RAW) & BIT_8)
            {
                CHIP_REG_OR (spi_add+SPI_INT_CLR, BIT_8);
                CHIP_REG_AND(spi_add+SPI_CTL7, ~BIT_15);
                break;
            }
        }
    }
#endif

    /*check really empey flag*/
    while (p_hwp_spi->status & SPI_TX_SPACE_MASK)
    {
        time_out_cnt++;

        if (time_out_cnt > (uint32) SPI_PHY_POLL_TIMEOUT)
        {
            SCI_PASSERT (0, ("SPI TIME OUT!"));/*assert verified*/
            break; /*lint !e527 comfirmed by xuepeng*/
        }
    }

    /*wait few clocks skip the gap: max 2*clk_spi+pclk*/
    /*dummy read, make sure spi bus is idle*/
    //dummy = CHIP_REG_GET (spi_add+SPI_STS2);

    /* check shifter chain is not busy*/
    while (p_hwp_spi->status & SPI_ACTIVE_STATUS)
    {
        time_out_cnt++;

        if (time_out_cnt > (uint32) SPI_PHY_POLL_TIMEOUT)
        {
            SCI_PASSERT (0, ("SPI TIME OUT!"));/*assert verified*/
            break; /*lint !e527 comfirmed by xuepeng*/
        }
    }

    return ERR_SPI_NONE;
}


/******************************************************************************
**  Description:    Send data byte by byte at SPI port, using Polling mode
**  Author:         Xuepeng.Zhang
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_WriteBytes (
    uint32 phy_id,
    uint8 *buffer_ptr,
    uint32 len
)
{

    uint32 spi_addr;
    uint32 bitlen;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
	spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;

    bitlen = SPI_PHY_GetBitLen (phy_id);
    /*wait for all of last data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SPI_PRINT ( ("SPI_PHY_WriteBytes: phy_id=%d, buffer_ptr=%d, len=%d",
                 phy_id, buffer_ptr , len));

    /*set transmit only write mode*/
    phy_inst->ctrl = p_hwp_spi->ctrl;
    phy_inst->ctrl &= (~SPI_INPUT_MODE);
    p_hwp_spi->ctrl = phy_inst->ctrl;

    __SPI_PHY_WBytes (phy_id, buffer_ptr, len, RW_CHECK);

    /* wait for all of data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);

    /*set idle mode*/
    phy_inst->ctrl &= ~ SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    return ERR_SPI_NONE;
}


/******************************************************************************
**  Description:    Receive data byte by byte at SPI port, using Polling mode
**  Author:         Xuepeng.Zhang
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_ReadBytes (
    uint32 phy_id,
    uint8 *buffer_ptr,
    uint32 len
)
{
    uint32 bitlen;
    uint32 num;

    uint32 spi_addr;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    
    /* wait for all of last data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SPI_PRINT ( ("SPI_PHY_ReadBytes: phy_id=%d, buffer_ptr=%d, len=%d",
                 phy_id, buffer_ptr , len));

    bitlen = SPI_PHY_GetBitLen (phy_id);
    num = len/ (bitlen/BITLEN_8);
    //SCI_PASSERT ( (num <= SPI_MAX_RECEIVE_WORD), ("read %d words has exceeded the max receive num:256 word",num));/*assert verified*/

    /*set receive only mode*/
    phy_inst->ctrl = p_hwp_spi->ctrl;
    phy_inst->ctrl |= SPI_INPUT_MODE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    /*read max num words due to readnum limit*/
    while (num > SPI_MAX_RECEIVE_WORD)
    {
        __SPI_PHY_RBytes (phy_id, buffer_ptr, SPI_MAX_RECEIVE_WORD* (bitlen/BITLEN_8), RW_CHECK);

        //phy_inst->ctl4 &= ~SPI_START_RX;
        num -= SPI_MAX_RECEIVE_WORD;
        len -= SPI_MAX_RECEIVE_WORD* (bitlen/BITLEN_8);
        buffer_ptr += SPI_MAX_RECEIVE_WORD* (bitlen/BITLEN_8);
    }

    /*set idle mode*/
    phy_inst->ctrl &= ~SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;
    return ERR_SPI_NONE;
}
/******************************************************************************
**  Description:    Receive data at SPI port and use DMA trans
**  Author:         Xuepeng.Zhang
**  Note:           The address of buffer must be aligned by 4 bytes
******************************************************************************/
PUBLIC int32 SPI_PHY_DmaRead (
    uint32   phy_id,       // port number of spi
    uint8    *buffer_ptr,    // pointer of receive data buffer
    uint32   dma_len         // byte-length of receive
)
{
    uint32 rx_channel;
    volatile uint32 time_out_cnt;
    volatile uint32 i;
    uint32 ctl1;
    SPI_THRESHOLD_T spi_threshold;
    DMA_CHNCFG_T s_rx_dma_cfg = {0};
    uint32 spi_addr;
    uint32 rx_src_datawidth = 0;
    uint32 block_length = 0;
    uint32 rx_dest_step = 0;
    uint32 endian_switchmode = 0;
    uint32 num = 0;
    uint32 bitlen;
    uint32 rx_src_address = 0;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    HAL_IFC_MODE_T ifc_mode;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    
    /* wait for all of data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SPI_PRINT ( ("SPI_PHY_DmaRead: phy_id=%d, buffer_ptr=0x%x, dma_len=%d", phy_id, buffer_ptr, dma_len));
    rx_channel = phy_inst->phy_info->rx_channel;
    SCI_ASSERT (INVALID_U16 != rx_channel); /*assert to do*/
    SCI_PASSERT ( (dma_len <= (SPI_MAX_RECEIVE_BYTE)), ("read %d bytes has exceeded the max receive num:256 word", dma_len)); /*assert to do*/
    /* storage registers*/
    ctl1 = phy_inst->ctrl;


    bitlen = SPI_PHY_GetBitLen (phy_id);

    switch (bitlen)
    {
        case BITLEN_8:     // 8bit mode
            rx_src_datawidth  = DMA_DATAWIDTH_BYTE;
            block_length      = ( (SPI_THRHOLD_CNT) << 0);
            rx_dest_step      = INCREMENT_BYTE;
            endian_switchmode = DMA_ENDIANSWITCHMODE_UN;
            num               = dma_len/INCREMENT_BYTE;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 3;
#endif
            ifc_mode          = HAL_IFC_SIZE_8_MODE_AUTO;
            break;
        case BITLEN_16:    // 16bit mode
            rx_src_datawidth  = DMA_DATAWIDTH_HALFWORD;
            block_length      = ( (SPI_THRHOLD_CNT) << 1);
            rx_dest_step      = INCREMENT_HALFWORD;
            endian_switchmode = DMA_ENDIANSWITCHMODE_MODE0;
            num               = dma_len/INCREMENT_HALFWORD;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 2;
#endif
            ifc_mode          = HAL_IFC_SIZE_16_MODE_AUTO;
            break;
        case BITLEN_32:     // 32bit mode
            rx_src_datawidth  = DMA_DATAWIDTH_WORD;
            block_length      = ( (SPI_THRHOLD_CNT) << 2);
            rx_dest_step      = INCREMENT_WORD;
            endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
            num               = dma_len/INCREMENT_WORD;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 0;
#endif
            ifc_mode          = HAL_IFC_SIZE_32_MODE_AUTO;
            break;
        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    phy_inst->ctrl = p_hwp_spi->ctrl;
    phy_inst->ctrl |= SPI_INPUT_MODE;
    p_hwp_spi->ctrl = phy_inst->ctrl;
    
    /* set water mark of reveive FIFO*/
    spi_threshold.rx_threshold  = (SPI_THRHOLD_CNT);
    spi_threshold.tx_threshold  = 0;
    __SPI_SetFifoThreshold (phy_id, &spi_threshold);

    /*enable spi dma*/
    //phy_inst->ctl2 |= SPI_DMA_EN;
    //CHIP_REG_SET (spi_addr+SPI_CTL2, phy_inst->ctl2);
    //DMA_HAL_ChnIntConfig (rx_channel, DMA_CHN_INT_TYPE_NONE, NULL);

    phy_inst->phy_info->rx_channel = hal_ApIfcTransferStart(gc_halSpiIfcRxId[phy_id], buffer_ptr, dma_len, ifc_mode);   
    //g_halSpiRxIfcCh = 0;      
    // get IFC channel and start if any available      
    if (phy_inst->phy_info->rx_channel == HAL_UNKNOWN_CHANNEL)      
    {        
        // No channel available        
        // No data received         
        SCI_PASSERT ( 1, ("No channel available")); /*assert to do*/
    }       

    /* Start send clk*/
    phy_inst->ctrl |= SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    time_out_cnt = 0;

    while (hal_ApIfcGetRunStatus(gc_halSpiIfcRxId[phy_id], phy_inst->phy_info->rx_channel))
    {
        for (i=0; i<100; i++) {}

        time_out_cnt++;

        if (time_out_cnt > (uint32) SPI_DMA_TIME_OUT)
        {
            SCI_PASSERT (FALSE, ("DMA all transfer time out!"));/*assert verified*/
        }
    }

    /* Disable rx DMA channel*/
    hal_ApIfcChannelRelease(gc_halSpiIfcRxId[phy_id], phy_inst->phy_info->rx_channel);

    /* restorage registers*/
    phy_inst->ctrl &= ~SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;
    
    /*set idle mode*/
    //phy_inst->ctl1 &= ~ (BIT_12 | BIT_13);
    //CHIP_REG_SET (spi_addr+SPI_CTL1, phy_inst->ctl1);

    return ERR_SPI_NONE;
}

/******************************************************************************
**  Description:    Send data at SPI port and use DMA trans
**  Author:         Xuepeng.Zhang
**  Note:           The address of buffer must be aligned by 4 bytes
******************************************************************************/
PUBLIC int32 SPI_PHY_DmaWrite (
    uint32          phy_id,     // tx channel for SPI module tansmite data
    uint8           *buffer_ptr,    // pointer of send data buffer
    uint32          dma_len         // byte-length of send data
)
{
    uint32 ctl1;
    volatile uint32 time_out_cnt;
    volatile uint32 i;
    DMA_CHNCFG_T s_tx_dma_cfg = {0};
    uint32 spi_addr;
    uint32 tx_channel;
    SPI_THRESHOLD_T spi_threshold;
    uint32 tx_dest_datawidth = 0;
    uint32 block_length = 0;
    uint32 tx_src_step = 0;
    uint32 endian_switchmode = 0;
    uint32 bitlen;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    HAL_IFC_MODE_T ifc_mode;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    /* wait for all of data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SPI_PRINT ( ("SPI_PHY_DmaWrite: phy_id=%d, buffer_ptr=0x%x, dma_len=%d", phy_id, buffer_ptr, dma_len));

    tx_channel = phy_inst->phy_info->tx_channel;
    SCI_ASSERT (INVALID_U16 != tx_channel); /*assert to do*/
    /* storage registers*/
    ctl1 = phy_inst->ctrl;
    bitlen = SPI_PHY_GetBitLen (phy_id);

    switch (bitlen)
    {
        case BITLEN_8:     // 8bit mode
            tx_dest_datawidth  = DMA_DATAWIDTH_BYTE;
            block_length      = ( (SPI_THRHOLD_CNT) << 0);
            tx_src_step      = INCREMENT_BYTE;
            endian_switchmode = DMA_ENDIANSWITCHMODE_UN;
            ifc_mode          = HAL_IFC_SIZE_8_MODE_AUTO;
            break;
        case BITLEN_16:    // 16bit mode
            tx_dest_datawidth  = DMA_DATAWIDTH_HALFWORD;
            block_length      = ( (SPI_THRHOLD_CNT) << 1);
            tx_src_step      = INCREMENT_HALFWORD;
            endian_switchmode = DMA_ENDIANSWITCHMODE_MODE0;
            ifc_mode          = HAL_IFC_SIZE_16_MODE_AUTO;
            break;
        case BITLEN_32:     // 32bit mode
            tx_dest_datawidth  = DMA_DATAWIDTH_WORD;
            block_length      = ( (SPI_THRHOLD_CNT) << 2);
            tx_src_step      = INCREMENT_WORD;
            endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
            ifc_mode          = HAL_IFC_SIZE_32_MODE_AUTO;
            break;
        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    phy_inst->ctrl &= ~SPI_INPUT_MODE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    /* set water mark of reveive FIFO*/
    spi_threshold.rx_threshold  = 0;
    spi_threshold.tx_threshold = 4;
    __SPI_SetFifoThreshold (phy_id, &spi_threshold);

    phy_inst->phy_info->tx_channel = hal_ApIfcTransferStart(gc_halSpiIfcTxId[phy_id], buffer_ptr, dma_len, ifc_mode);   
    //g_halSpiRxIfcCh = 0;      
    // get IFC channel and start if any available      
    if (phy_inst->phy_info->tx_channel == HAL_UNKNOWN_CHANNEL)      
    {        
        // No channel available        
        // No data received         
        SCI_PASSERT ( 1, ("No channel available")); /*assert to do*/
    }       

    /* Start send clk*/
    phy_inst->ctrl |= SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    time_out_cnt = 0;

    while (hal_ApIfcGetRunStatus(gc_halSpiIfcTxId[phy_id], phy_inst->phy_info->tx_channel))
    {
        for (i=0; i<100; i++) {}

        time_out_cnt++;

        if (time_out_cnt > (uint32) SPI_DMA_TIME_OUT)
        {
            SCI_PASSERT (FALSE, ("DMA all transfer time out!"));/*assert verified*/
        }
    }

    /* Disable tx DMA channel*/
    hal_ApIfcChannelRelease(gc_halSpiIfcTxId[phy_id], phy_inst->phy_info->tx_channel);

    /* restorage registers*/
    phy_inst->ctrl &= ~SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    /*set idle mode*/
    //phy_inst->ctl1 &= ~ (BIT_12 | BIT_13);
    //CHIP_REG_SET (spi_addr+SPI_CTL1, phy_inst->ctl1);

    return ERR_SPI_NONE;
}

/******************************************************************************
**  Description:    Send and receive data by byte at SPI port
**  Author:         Wenjun.Shi
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_ReadWriteBytes (
    uint32          phy_id,       // port number of spi
    uint8           *din_buffer_ptr,    // pointer of buffer of received data
    uint8           *dout_buffer_ptr,    // pointer of buffer of send data
    uint32          len             // byte-length to send
)
{
    uint32 read_bytes_cnt=0;            // byte-conter of receive
    uint32 spi_addr;
    uint32 cur_cnt;                     // byte-count per xfer
    uint32 bitlen;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    bitlen = SPI_PHY_GetBitLen (phy_id);

    /* wait for all of last data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    /* len must be exactly divided by word_width*/
    SCI_ASSERT (len% (bitlen/BITLEN_8) == 0); /*assert to do*/

    /*set transmit and receive mode*/
    phy_inst->ctrl |= SPI_INPUT_MODE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    /*write data bytes*/
    while (read_bytes_cnt < len)
    {
        /*spi polling mode*/
        /*calculate byte counts to xfer*/
        cur_cnt = (len - read_bytes_cnt) > (SPI_FIFO_SIZE * (bitlen/BITLEN_8)) ?
                  (SPI_FIFO_SIZE * (bitlen/BITLEN_8)) : (len - read_bytes_cnt);

        SPI_PRINT ( ("SPI_PHY_ReadWriteBytes: phy_id=%d, din_buffer_ptr=%d, dout_buffer_ptr=%d, len=%d,  cur_cnt=%d",
                     phy_id, din_buffer_ptr, dout_buffer_ptr, len,  cur_cnt));
        /* reset spi fifo*/
        //CHIP_REG_SET (spi_addr+SPI_FIFO_RST, 0x1);
        //CHIP_REG_SET (spi_addr+SPI_FIFO_RST, 0x0);

        __SPI_PHY_WBytes (phy_id, dout_buffer_ptr, cur_cnt, RW_CHECK);


        /*spi polling mode*/
        /* wait for all of data send out*/
        SPI_PHY_WaitForBusIdle (spi_addr);

        __SPI_PHY_RBytes (phy_id, din_buffer_ptr, cur_cnt, RW_CHECK);

        read_bytes_cnt  += cur_cnt;

        if (dout_buffer_ptr != PNULL)
        {
            dout_buffer_ptr += cur_cnt;
        }

        if (din_buffer_ptr != PNULL)
        {
            din_buffer_ptr += cur_cnt;
        }

        SPI_PRINT ( ("SPI_PHY_ReadWriteBytes: read_bytes_cnt=%d,write_bytes_cnt=%d ",
                     read_bytes_cnt, write_bytes_cnt));
    }

    /*set idle mode*/
    //phy_inst->ctl1 &= ~ (BIT_12 | BIT_13);
    //CHIP_REG_SET (spi_addr+SPI_CTL1, phy_inst->ctl1);
    return ERR_SPI_NONE;
}


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
)
{
    volatile uint32 time_out_cnt;
    uint32 ctl1;
    uint32 tx_channel;                    // dma chaanel number for spi transmmit
    uint32 rx_channel;                    // dma chaanel number for spi receive
    DMA_CHNCFG_T s_tx_dma_cfg = {0};
    DMA_CHNCFG_T s_rx_dma_cfg = {0};
    uint32 spi_addr;
    SPI_THRESHOLD_T spi_threshold;
    uint32 rx_src_datawidth;
    uint32 tx_dest_datawidth = 0;
    uint32 block_length = 0;
    uint32 rx_dest_step = 0;
    uint32 tx_src_step = 0;
    uint32 endian_switchmode = 0;
    uint32 bitlen;
    uint32 rx_src_address;
    uint32 i;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    HAL_IFC_MODE_T ifc_mode;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    /* wait for all of data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SPI_PRINT ( ("SPI_PHY_DmaReadWrite: phy_id=%d, din_buffer_ptr=0x%x,\
        dout_buffer_ptr=0x%x, dma_len=%d", phy_id, din_buffer_ptr,
                 dout_buffer_ptr, dma_len));
    rx_channel = phy_inst->phy_info->rx_channel;
    tx_channel = phy_inst->phy_info->tx_channel;
    SCI_ASSERT ( (INVALID_U16 != rx_channel) && (INVALID_U16 != tx_channel)); /*assert to do*/
    /* storage registers*/
    ctl1 = phy_inst->ctrl;
    /* reset spi fifo*/
    p_hwp_spi->status = SPI_FIFO_FLUSH;
    bitlen = SPI_PHY_GetBitLen (phy_id);

    switch (bitlen)
    {
        case BITLEN_8:     // 8bit mode
            tx_dest_datawidth = rx_src_datawidth = DMA_DATAWIDTH_BYTE;
            block_length      = ( (SPI_THRHOLD_CNT) << 0);
            rx_dest_step      = (PNULL == din_buffer_ptr ? INCREMENT_NONE:INCREMENT_BYTE);
            tx_src_step       = (PNULL == dout_buffer_ptr? INCREMENT_NONE:INCREMENT_BYTE);
            din_buffer_ptr    = (PNULL == din_buffer_ptr ? (uint8 *) (&spi_buffer_zero) : din_buffer_ptr);
            dout_buffer_ptr   = (PNULL == dout_buffer_ptr? (uint8 *) (&spi_buffer_dummy) :dout_buffer_ptr);
            endian_switchmode = DMA_ENDIANSWITCHMODE_UN;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 3;
#endif
            ifc_mode          = HAL_IFC_SIZE_8_MODE_AUTO;
            break;
        case BITLEN_16:    // 16bit mode
            tx_dest_datawidth = rx_src_datawidth = DMA_DATAWIDTH_HALFWORD;
            block_length      = ( (SPI_THRHOLD_CNT) << 1);
            rx_dest_step      = (PNULL == din_buffer_ptr ? INCREMENT_NONE:INCREMENT_HALFWORD);
            tx_src_step       = (PNULL == dout_buffer_ptr? INCREMENT_NONE:INCREMENT_HALFWORD);
            din_buffer_ptr    = (PNULL == din_buffer_ptr ? (uint8 *) (&spi_buffer_zero) : din_buffer_ptr);
            dout_buffer_ptr   = (PNULL == dout_buffer_ptr? (uint8 *) (&spi_buffer_dummy) :dout_buffer_ptr);
            endian_switchmode = DMA_ENDIANSWITCHMODE_MODE0;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 2;
#endif
            ifc_mode          = HAL_IFC_SIZE_16_MODE_AUTO;
            break;
        case BITLEN_32:     // 32bit mode
            tx_dest_datawidth = rx_src_datawidth = DMA_DATAWIDTH_WORD;
            block_length      = ( (SPI_THRHOLD_CNT) << 2);
            rx_dest_step      = (PNULL == din_buffer_ptr ? INCREMENT_NONE:INCREMENT_WORD);
            tx_src_step       = (PNULL == dout_buffer_ptr? INCREMENT_NONE:INCREMENT_WORD);
            din_buffer_ptr    = (PNULL == din_buffer_ptr ? (uint8 *) (&spi_buffer_zero) : din_buffer_ptr);
            dout_buffer_ptr   = (PNULL == dout_buffer_ptr? (uint8 *) (&spi_buffer_dummy) :dout_buffer_ptr);
            endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 0;
#endif
            ifc_mode          = HAL_IFC_SIZE_32_MODE_AUTO;
            break;
        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    phy_inst->ctrl |= SPI_INPUT_MODE;
    p_hwp_spi->ctrl = phy_inst->ctrl;


    /* set water mark of reveive FIFO*/
    spi_threshold.rx_threshold  = (SPI_THRHOLD_CNT);
    spi_threshold.tx_threshold  = (SPI_THRHOLD_CNT);
    __SPI_SetFifoThreshold (phy_id, &spi_threshold);

    phy_inst->phy_info->tx_channel = hal_ApIfcTransferStart(gc_halSpiIfcTxId[phy_id], dout_buffer_ptr, dma_len, ifc_mode);   
    //g_halSpiRxIfcCh = 0;      
    // get IFC channel and start if any available      
    if (phy_inst->phy_info->tx_channel == HAL_UNKNOWN_CHANNEL)      
    {        
        // No channel available        
        // No data received         
        SCI_PASSERT ( 1, ("No channel available")); /*assert to do*/
    }       

    phy_inst->phy_info->rx_channel = hal_ApIfcTransferStart(gc_halSpiIfcRxId[phy_id], din_buffer_ptr, dma_len, ifc_mode);   
    //g_halSpiRxIfcCh = 0;      
    // get IFC channel and start if any available      
    if (phy_inst->phy_info->rx_channel == HAL_UNKNOWN_CHANNEL)      
    {        
        // No channel available        
        // No data received         
        SCI_PASSERT ( 1, ("No channel available")); /*assert to do*/
    }       

    /* Start send clk*/
    phy_inst->ctrl |= SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    time_out_cnt = 0;

    while (hal_ApIfcGetRunStatus(gc_halSpiIfcTxId[phy_id], phy_inst->phy_info->tx_channel))
    {
        for (i=0; i<100; i++) {}

        time_out_cnt++;

        if (time_out_cnt > (uint32) SPI_DMA_TIME_OUT)
        {
            SCI_PASSERT (FALSE, ("DMA all transfer time out!"));/*assert verified*/
        }
    }

    while (hal_ApIfcGetRunStatus(gc_halSpiIfcRxId[phy_id], phy_inst->phy_info->rx_channel))
    {
        for (i=0; i<100; i++) {}

        time_out_cnt++;

        if (time_out_cnt > (uint32) SPI_DMA_TIME_OUT)
        {
            SCI_PASSERT (FALSE, ("DMA all transfer time out!"));/*assert verified*/
        }
    }
    
    /* wait untill the Tx FIFO is empty*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    
    /* Disable tx DMA channel*/
    hal_ApIfcChannelRelease(gc_halSpiIfcTxId[phy_id], phy_inst->phy_info->tx_channel);

    /* Disable tx DMA channel*/
    hal_ApIfcChannelRelease(gc_halSpiIfcRxId[phy_id], phy_inst->phy_info->rx_channel);

    /* restorage registers*/
    phy_inst->ctrl &= ~SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;
    
    /*set idle mode*/
    //phy_inst->ctl1 &= ~ (BIT_12 | BIT_13);
    //CHIP_REG_SET (spi_addr+SPI_CTL1, phy_inst->ctl1);
    return ERR_SPI_NONE;
}

/******************************************************************************
**  Description:    Send data byte by byte at SPI port, using Interrupt mode
**  Author:         Xuepeng.Zhang
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_WriteBytes_Int (
    uint32 phy_id,
    uint8 *buffer_ptr,
    uint32 len
)
{

    uint32 spi_addr;
    uint32 bitlen;
    SPI_THRESHOLD_T spi_threshold;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    bitlen = SPI_PHY_GetBitLen (phy_id);
    /* wait for all of last data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SPI_PRINT ( ("SPI_PHY_WriteBytes_Int: phy_id=%d, buffer_ptr=%d, len=%d",
                 phy_id, buffer_ptr , len));

    /*set transmit only mode*/
    phy_inst->ctrl &= ~SPI_INPUT_MODE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    /*set tx threshold*/
    spi_threshold.tx_threshold = SPI_THRHOLD_CNT;
    __SPI_SetFifoThreshold (phy_id, &spi_threshold);

    /*initialize irq context*/
    s_spi_irq_ctx.bus_id = phy_id;
    s_spi_irq_ctx.bitlen = bitlen;
    s_spi_irq_ctx.spi_rw_remain_size = len;
    s_spi_irq_ctx.spi_rw_tx_buf_ptr = buffer_ptr;

    /*necessary to issue spi tx empty interrupt*/
    if (len > SPI_FIFO_SIZE* (bitlen/BITLEN_8))
    {
        __SPI_PHY_WBytes (phy_id, buffer_ptr, SPI_FIFO_SIZE* (bitlen/BITLEN_8), RW_CHECK);
        s_spi_irq_ctx.spi_rw_remain_size -= SPI_FIFO_SIZE* (bitlen/BITLEN_8);
        s_spi_irq_ctx.spi_rw_tx_buf_ptr += SPI_FIFO_SIZE* (bitlen/BITLEN_8);
        ISR_UnRegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT);
        //ISR_RegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT, (TB_ISR) __SPI_TxIrqHandle);
        ISR_RegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT, (TB_ISR) SPI_PHY_IRQHandler);
        CHIPDRV_EnableIRQINT ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT);
        /*enable tx empty  int*/
        //CHIP_REG_OR (spi_addr+SPI_INT_IEN, SPI_TXF_EMPTY_INT_EN);
    }
    /* no need tx empty intterrupt*/
    else
    {
        __SPI_PHY_WBytes (phy_id, buffer_ptr, len, RW_CHECK);
        (spi_tx_callback) (PNULL);
    }

    return ERR_SPI_NONE;
}

/******************************************************************************
**  Description:    Receive data byte by byte at SPI port, using Interrupt mode
**  Author:         Xuepeng.Zhang
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_ReadBytes_Int (
    uint32 phy_id,
    uint8 *buffer_ptr,
    uint32 len
)
{
    uint32 read_bytes_cnt;          // byte-conter of receive
    uint32 bitlen;
    uint32 num;

    uint32 spi_addr;
    SPI_THRESHOLD_T spi_threshold;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    
    /* wait for all of last data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SPI_PRINT ( ("SPI_PHY_ReadBytes_Int: phy_id=%d, buffer_ptr=%d, len=%d",
                 phy_id, buffer_ptr , len));
    bitlen = SPI_PHY_GetBitLen (phy_id);

    num = len/ (bitlen/BITLEN_8);
    SCI_PASSERT ( (num <= ( (SPI_MAX_RECEIVE_BYTE) >> 2)), ("read %d words has exceeded the max receive num:256 word",num)); /*assert verified*/

    /*set receive only mode*/
    phy_inst->ctrl |= SPI_INPUT_MODE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    /*set total byte-length to send*/
    //phy_inst->ctl4 = 0x8000+ num;
    //CHIP_REG_SET (spi_addr+SPI_CTL4, phy_inst->ctl4);       // set 32-bit length of receive

    read_bytes_cnt = 0;

    /*set rx threshold*/
    spi_threshold.rx_threshold  = SPI_THRHOLD_CNT;
    __SPI_SetFifoThreshold (phy_id, &spi_threshold);

    /*initialize irq context*/
    s_spi_irq_ctx.bus_id = phy_id;
    s_spi_irq_ctx.bitlen = bitlen;
    s_spi_irq_ctx.spi_rw_remain_size = len;
    s_spi_irq_ctx.spi_rw_tx_buf_ptr = buffer_ptr;

    /*have at least one spi rx fifo full interrupt*/
    if (len >= SPI_THRHOLD_CNT* (bitlen/BITLEN_8))
    {
        ISR_UnRegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT);
        //ISR_RegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT, (TB_ISR) __SPI_RxIrqHandle);
        ISR_RegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT, (TB_ISR) SPI_PHY_IRQHandler);
        CHIPDRV_EnableIRQINT ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT);
        /*enable rx full int*/
        //CHIP_REG_OR (spi_addr+SPI_INT_IEN, SPI_RXF_FULL_INT_EN);
    }
    /* no rx fifo full intterrupt*/
    else
    {
        SPI_PRINT ( ("SPI_PHY_ReadBytes_Int: SCI_ActiveTimer, SCI_InThreadContext:%d", SCI_InThreadContext()));
        SCI_ChangeTimer (spi_rx_timer, __SPI_RxTimerHandle, SPI_RX_INTERVAL_MS);
        SCI_ActiveTimer (spi_rx_timer);
    }

    /* set receive data only mode and start to fire clock for receiving data*/
    //phy_inst->ctl4 |= SPI_START_RX;
    //CHIP_REG_SET (spi_addr+SPI_CTL4, phy_inst->ctl4);
    return ERR_SPI_NONE;
}

/******************************************************************************
**  Description:    Send and receive data by byte at SPI port
**  Author:         Wenjun.Shi
**  Note:
******************************************************************************/
PUBLIC int32 SPI_PHY_ReadWriteBytes_Int (
    uint32          phy_id,       // port number of spi
    uint8           *din_buffer_ptr,    // pointer of buffer of received data
    uint8           *dout_buffer_ptr,    // pointer of buffer of send data
    uint32          len             // byte-length to send
)
{
    uint32 read_bytes_cnt=0;            // byte-conter of receive
    uint32 write_bytes_cnt=0;           // byte-conter to send
    uint32 spi_addr;
    uint32 cur_cnt=0;                     // byte-count per xfer
    uint32 bitlen;
    SPI_THRESHOLD_T spi_threshold;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    bitlen = SPI_PHY_GetBitLen (phy_id);

    /* wait for all of last data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SCI_ASSERT (len% (bitlen/BITLEN_8) == 0); /*assert to do*/

    /* set transmit and receive mode*/
    phy_inst->ctrl |= SPI_INPUT_MODE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

    /*write data bytes*/
    while (read_bytes_cnt < len)
    {
        /*spi interrupt mode*/
        if ( (PNULL != spi_tx_callback) || (PNULL != spi_rx_callback))
        {
            s_spi_irq_ctx.bus_id = phy_id;
            s_spi_irq_ctx.bitlen = bitlen;
            /*calculate word counts to xfer*/
            cur_cnt = (len - write_bytes_cnt) > (SPI_THRHOLD_CNT * (bitlen/BITLEN_8)) ?
                      (SPI_THRHOLD_CNT * (bitlen/BITLEN_8)) : (len - write_bytes_cnt);

            /*set rx, tx threshold*/
            spi_threshold.rx_threshold  = cur_cnt/ (bitlen/BITLEN_8);
            spi_threshold.tx_threshold = 0;
            __SPI_SetFifoThreshold (phy_id, &spi_threshold);

            ISR_UnRegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT);
            //ISR_RegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT, (TB_ISR) __SPI_RWIrqHandle);
            ISR_RegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT, (TB_ISR) SPI_PHY_IRQHandler);
            CHIPDRV_EnableIRQINT ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT);
            /*enable rx fifo full int*/
            //CHIP_REG_OR (spi_addr+SPI_INT_IEN, SPI_RXF_FULL_INT_EN);
        }

        SPI_PRINT ( ("SPI_PHY_ReadWriteBytes: phy_id=%d, din_buffer_ptr=%d,\
            dout_buffer_ptr=%d, len=%d,  cur_cnt=%d",
                     phy_id, din_buffer_ptr, dout_buffer_ptr,
                     len,  cur_cnt));

        /*reset spi fifo*/
        p_hwp_spi->status = SPI_FIFO_FLUSH;
        
        /*initialize irq context*/
        s_spi_irq_ctx.spi_rw_remain_size = len;
        SCI_ASSERT (s_spi_irq_ctx.spi_rw_remain_size != 0); /*assert to do*/
        s_spi_irq_ctx.spi_rw_last_size = cur_cnt;
        s_spi_irq_ctx.spi_rw_rx_buf_ptr = din_buffer_ptr;
        s_spi_irq_ctx.spi_rw_tx_buf_ptr = dout_buffer_ptr;

        __SPI_PHY_WBytes (phy_id, dout_buffer_ptr, cur_cnt, RW_CHECK);

        /*spi interrupt mode*/
        if ( (PNULL != spi_tx_callback) || (PNULL != spi_rx_callback))
        {

            return ERR_SPI_NONE;
        }

    }

    /*set idle mode*/
    //CHIP_REG_AND (spi_addr+SPI_CTL1,  ~ (BIT_12 | BIT_13));
    return ERR_SPI_NONE;
}
/******************************************************************************
**  Description:    Send and receive data at SPI port and use DMA trans
**  Author:         Wenjun.Shi
**  Note:           The address of buffer must be aligned by 4 bytes
******************************************************************************/
PUBLIC int32 SPI_PHY_DmaReadWrite_Int (
    uint32          phy_id,     // port number of spi
    uint8           *din_buffer_ptr,    // pointer of received data buffer
    uint8           *dout_buffer_ptr,    // pointer of send data buffer
    uint32          dma_len         // byte-length of send data
)
{
    uint32 ctl1;
    uint32 tx_channel;                    // dma chaanel number for spi transmmit
    uint32 rx_channel;                    // dma chaanel number for spi receive
    DMA_CHNCFG_T s_tx_dma_cfg = {0};
    DMA_CHNCFG_T s_rx_dma_cfg = {0};
    uint32 spi_addr;
    SPI_THRESHOLD_T spi_threshold;
    uint32 rx_src_datawidth;
    uint32 tx_dest_datawidth = 0;
    uint32 block_length = 0;
    uint32 rx_dest_step;
    uint32 tx_src_step = 0;
    uint32 endian_switchmode = 0;
    uint32 bitlen;
    uint32 rx_src_address;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    HAL_IFC_MODE_T ifc_mode;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    /* wait for all of data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SPI_PRINT ( ("SPI_PHY_DmaReadWrite: phy_id=%d, din_buffer_ptr=0x%x,\
        dout_buffer_ptr=0x%x, dma_len=%d", phy_id, din_buffer_ptr,
                 dout_buffer_ptr, dma_len));

    rx_channel = phy_inst->phy_info->rx_channel;
    tx_channel = phy_inst->phy_info->tx_channel;
    SCI_ASSERT ( (INVALID_U16 != rx_channel) && (INVALID_U16 != tx_channel)); /*assert to do*/
    s_spi_dma_ctx.bus_id = phy_id;
    /* storage registers*/
    ctl1 = phy_inst->ctrl;

    s_spi_dma_ctx.spi_txd_flag = (PNULL == din_buffer_ptr? 0:1) | ( (PNULL == dout_buffer_ptr? 0:1) <<1);
    bitlen = SPI_PHY_GetBitLen (phy_id);

    switch (bitlen)
    {
        case BITLEN_8:     // 8bit mode
            tx_dest_datawidth = rx_src_datawidth = DMA_DATAWIDTH_BYTE;
            block_length      = ( (SPI_THRHOLD_CNT) << 0);
            rx_dest_step      = (PNULL == din_buffer_ptr ? INCREMENT_NONE:INCREMENT_BYTE);
            /* Note: in SC6530, src and dest step can't be fixed at the same time.
            *  Fortunately, data in tx_src is only used for read, so it will not ruin data
            *  when in RX_MODE.
            */
            tx_src_step       = INCREMENT_BYTE;//( PNULL == dout_buffer_ptr? INCREMENT_NONE:INCREMENT_BYTE );
            din_buffer_ptr    = (PNULL == din_buffer_ptr ? (uint8 *) (&spi_buffer_zero) : din_buffer_ptr);
            dout_buffer_ptr   = (PNULL == dout_buffer_ptr? (uint8 *) (&spi_buffer_dummy) :dout_buffer_ptr);
            endian_switchmode = DMA_ENDIANSWITCHMODE_UN;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 3;
#endif
            ifc_mode          = HAL_IFC_SIZE_8_MODE_AUTO;
            break;
        case BITLEN_16:    // 16bit mode
            tx_dest_datawidth = rx_src_datawidth = DMA_DATAWIDTH_HALFWORD;
            block_length      = ( (SPI_THRHOLD_CNT) << 1);
            rx_dest_step      = (PNULL == din_buffer_ptr ? INCREMENT_NONE:INCREMENT_HALFWORD);
            tx_src_step       = INCREMENT_HALFWORD;//( PNULL == dout_buffer_ptr? INCREMENT_NONE:INCREMENT_HALFWORD );
            din_buffer_ptr    = (PNULL == din_buffer_ptr ? (uint8 *) (&spi_buffer_zero) : din_buffer_ptr);
            dout_buffer_ptr   = (PNULL == dout_buffer_ptr? (uint8 *) (&spi_buffer_dummy) :dout_buffer_ptr);
            endian_switchmode = DMA_ENDIANSWITCHMODE_MODE0;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 2;
#endif
            ifc_mode          = HAL_IFC_SIZE_16_MODE_AUTO;
            break;
        case BITLEN_32:     // 32bit mode
            tx_dest_datawidth = rx_src_datawidth = DMA_DATAWIDTH_WORD;
            block_length      = ( (SPI_THRHOLD_CNT) << 2);
            rx_dest_step      = (PNULL == din_buffer_ptr ? INCREMENT_NONE:INCREMENT_WORD);
            tx_src_step       = INCREMENT_WORD;//( PNULL == dout_buffer_ptr? INCREMENT_NONE:INCREMENT_WORD );
            din_buffer_ptr    = (PNULL == din_buffer_ptr ? (uint8 *) (&spi_buffer_zero) : din_buffer_ptr);
            dout_buffer_ptr   = (PNULL == dout_buffer_ptr? (uint8 *) (&spi_buffer_dummy) :dout_buffer_ptr);
            endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#ifdef CHIP_ENDIAN_LITTLE
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer);
#else
            rx_src_address       = (uint32) (&p_hwp_spi->rxtx_buffer) + 0;
#endif
            ifc_mode          = HAL_IFC_SIZE_32_MODE_AUTO;
            break;
        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    ISR_UnRegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT);
    ISR_RegHandler ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT, (TB_ISR) SPI_PHY_IRQHandler);
    CHIPDRV_EnableIRQINT ( (phy_id == SPI_BUS_ID_0) ? TB_SPI0_INT : TB_SPI1_INT);

    /*Only run duplex for RX operations. This reduces TX overhead due to reduced DMA setup overhead*/
    if (s_spi_dma_ctx.spi_txd_flag & RX_MODE)
    {
        phy_inst->ctrl |= SPI_INPUT_MODE;
        p_hwp_spi->irq |= SPI_MASK_RX_DMA_IRQ;
    }
    else if (s_spi_dma_ctx.spi_txd_flag & TX_MODE)
    {
        phy_inst->ctrl &= (~SPI_INPUT_MODE);
        p_hwp_spi->irq |= SPI_MASK_TX_DMA_IRQ;
    }
    else if (s_spi_dma_ctx.spi_txd_flag & RTX_MODE)
    {
        phy_inst->ctrl |= SPI_INPUT_MODE;
        p_hwp_spi->irq |= SPI_MASK_RX_DMA_IRQ | SPI_MASK_TX_DMA_IRQ;
    }
    else
    {
        phy_inst->ctrl &= (~SPI_INPUT_MODE);
    }
    p_hwp_spi->ctrl = phy_inst->ctrl;
    
    /* set water mark of reveive FIFO*/
    spi_threshold.rx_threshold  = (SPI_THRHOLD_CNT);
    spi_threshold.tx_threshold  = (SPI_THRHOLD_CNT);
    __SPI_SetFifoThreshold (phy_id, &spi_threshold);

    /*config SPI TX DMA*/
    /*Configure TX DMA callback in case only TX channel is to be used.*/
    phy_inst->phy_info->tx_channel = hal_ApIfcTransferStart(gc_halSpiIfcTxId[phy_id], dout_buffer_ptr, dma_len, ifc_mode);   
    //g_halSpiRxIfcCh = 0;      
    // get IFC channel and start if any available      
    if (phy_inst->phy_info->tx_channel == HAL_UNKNOWN_CHANNEL)      
    {        
        // No channel available        
        // No data received         
        SCI_PASSERT ( 1, ("No channel available")); /*assert to do*/
    }       
    
    if (s_spi_dma_ctx.spi_txd_flag & RX_MODE)
    {
        //DMA_HAL_ChnIntConfig (tx_channel, DMA_CHN_INT_TYPE_NONE, NULL);
    }
    else
    {
        //DMA_HAL_ChnIntConfig (tx_channel, DMA_CHN_INT_TYPE_TRANSDONE, __SPI_DmaRxHandle);
    }

    /*config SPI RX DMA*/
    /*Only configure RX DMA channel in RX mode*/
    phy_inst->phy_info->rx_channel = hal_ApIfcTransferStart(gc_halSpiIfcRxId[phy_id], din_buffer_ptr, dma_len, ifc_mode);   
    //g_halSpiRxIfcCh = 0;      
    // get IFC channel and start if any available      
    if (phy_inst->phy_info->rx_channel == HAL_UNKNOWN_CHANNEL)      
    {        
        // No channel available        
        // No data received         
        SCI_PASSERT ( 1, ("No channel available")); /*assert to do*/
    }       
    if (s_spi_dma_ctx.spi_txd_flag & RX_MODE)
    {
        //DMA_HAL_ChnIntConfig (rx_channel, DMA_CHN_INT_TYPE_TRANSDONE, __SPI_DmaRxHandle);

    }

    /* Start send clk*/
    phy_inst->ctrl |= SPI_ENABLE;
    p_hwp_spi->ctrl = phy_inst->ctrl;

#if 0
    //phy_inst->ctl2 |= SPI_DMA_EN;
    //CHIP_REG_SET (spi_addr+SPI_CTL2, phy_inst->ctl2);

    {
        DMA_CAHCECONFIG_T cacheConfig[1];

        if (s_spi_dma_ctx.spi_txd_flag & RX_MODE)
        {
            cacheConfig[0].bufferAddr = (uint32) s_rx_dma_cfg.dest_address;
            cacheConfig[0].bufferSize = s_rx_dma_cfg.total_length;
            cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
            DMA_HAL_ChnEnable_Ex (rx_channel, SCI_TRUE, cacheConfig, 1);
        }

        cacheConfig[0].bufferAddr = (uint32) s_tx_dma_cfg.src_address;
        cacheConfig[0].bufferSize = s_tx_dma_cfg.total_length;
        cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
        DMA_HAL_ChnEnable_Ex (tx_channel, SCI_TRUE, cacheConfig, 1);


    }
#endif

    return ERR_SPI_NONE;
}
/*****************************************************************************/
// Description :    This function is used to get SPI source clock frequency
//                  from registers.
// Global resource dependence : GR_GEN2
// Author :         hanjun.liu
//         2009/10/26 Xuepeng.Zhang clean up
// Note :
/*****************************************************************************/
LOCAL uint32 SPI_PHY_GetSrcClk (
    uint32 phy_id    // port number of spi
)
{
    uint32 clk_spi_div = 0;
    uint32 clk_src = 0;
    clk_src = DEVICE_GetClock (s_dev_spi[phy_id]);
    clk_spi_div = DEVICE_GetClockDiv (s_dev_spi[phy_id]);
    clk_src = clk_src / (clk_spi_div + 1);
    return clk_src;
}

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
)
{
    uint32 spi_addr;
    uint32 apb_clk;
    uint32 spi_clkd;
    SPI_PHY_INST *phy_inst;

    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    /* wait for all of data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    /*get frequency of source clock*/
    apb_clk = SPI_PHY_GetSrcClk (phy_id);
    spi_clkd = ( (apb_clk / (2 * bus_clk)) > 0? (apb_clk / (2 * bus_clk)) : 1) - 1;

    //CHIP_REG_SET (spi_addr+SPI_CLKD, spi_clkd);
    return ERR_SPI_NONE;
}

/*****************************************************************************/
// Description :    This function get SPI source clock frequency by getting
//                      SPI clock devider value
// Global resource dependence : GR_GEN2
// Author :         Xuepeng.Zhang
// Note :
/*****************************************************************************/
PUBLIC int32 SPI_PHY_GetBusClk (
    uint32  phy_id                    // port number of spi
)
{
    uint32 spi_addr;  // base addresse of spi control registers
    uint32 source_clk;                      // frequency of source clock
    uint32 spi_clk = 0;                         // frequency of spi clock
    SPI_PHY_INST *phy_inst;

    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;

    source_clk = SPI_PHY_GetSrcClk (phy_id);
    //spi_clk = source_clk / (2 * (CHIP_REG_GET (spi_addr+SPI_CLKD) + 1));

    return spi_clk;
}

/*****************************************************************************/
//  Description:    This function set the CS to actived or inactived
//  Author:         xuepeng.zhang
//  Note:
/*****************************************************************************/
PUBLIC int32 SPI_PHY_SetCS (uint32  phy_id, uint32 cs_id, uint32 cs_level)
{
    uint32  spi_addr;  // base addresse of spi control registers
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    /* wait for all of data send out*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    /*wait till spi module idle*/
    SPI_PHY_WaitForBusIdle (spi_addr);

    phy_inst->ctrl = p_hwp_spi->ctrl;
    phy_inst->ctrl &= ~SPI_CS_SEL_MASK;
    switch (cs_id)
    {
        case SPI_CS_ID0:
            break;
        case SPI_CS_ID1:
            phy_inst->ctrl |= SPI_CS_SEL(1);
            break;
        case SPI_CS_ID2:
            phy_inst->ctrl |= SPI_CS_SEL(2);
            break;
        case SPI_CS_ID3:
            phy_inst->ctrl |= SPI_CS_SEL(3);
            break;
        default:
            break;
    }
    p_hwp_spi->ctrl = phy_inst->ctrl;

    phy_inst->cfg = p_hwp_spi->cfg;
    if (cs_level == SPI_CS_HIGH)
    {
        phy_inst->cfg &= ~(1 << cs_id);
    }
    else
    {
        phy_inst->cfg |= (1 << cs_id);
    }

    p_hwp_spi->cfg = phy_inst->cfg;
    return ERR_SPI_NONE;
}

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
)
{
    uint32 bitlen;
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
	
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    p_hwp_spi = (HWP_SPI_T *)phy_inst->phy_info->base_addr;

    bitlen =  (p_hwp_spi->ctrl & SPI_FRAME_SIZE_MASK) >> SPI_FRAME_SIZE_SHIFT ;

    if (0 == bitlen)
    {
        return 32;
    }
    else
    {
        return bitlen;
    }
}

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
)
{
    uint32 spi_addr;  // base addresse of spi control registers
    SPI_PHY_INST *phy_inst;
    HWP_SPI_T *p_hwp_spi;
    
    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;
    p_hwp_spi = (HWP_SPI_T *)spi_addr;
    
    /*wait till spi module idle*/
    SPI_PHY_WaitForBusIdle (spi_addr);
    SCI_ASSERT (7 == bit_len ||8 == bit_len || 9 == bit_len ||
                10 == bit_len || 11 == bit_len || 12 == bit_len || 16 == bit_len ||
                32 == bit_len);/*assert to do*/

    phy_inst->ctrl &= ~SPI_FRAME_SIZE_MASK;
    phy_inst->ctrl |= SPI_FRAME_SIZE(bit_len - 1);
    p_hwp_spi->ctrl = phy_inst->ctrl;
    return ERR_SPI_NONE;
}

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
PUBLIC int32 __SPI_PHY_GetPAD_GPIOID (SPI_PAD_NAME_E pad_name)
{
    uint32 i = 0;
    uint32 gpio_id = INVALID_U32;

    for (;;)
    {
        if (SPI_PAD_MAX == __spi_pad_func_info[i].spi_pad_name)
        {
            break;
        }

        if (pad_name == __spi_pad_func_info[i].spi_pad_name)
        {
            gpio_id = __spi_pad_func_info[i].gpio_id;
            break;
        }

        i++;
    }

    return gpio_id;
}


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
PUBLIC int32 SPI_PHY_SetPAD (SPI_PAD_NAME_E pad_name, uint32 pad_fun)
{
    uint32 pin_addr = 0;
    uint32 pin_val = 0;
    uint32 pin_func_val = 0;
    uint32 i = 0;

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    if (PAD_SPI1_3_CLK_ALTERNATE == pad_name)
    {
        /* select LCMD14 as SPI1_3_CLK*/
        CHIP_REG_OR (APB_PIN_CTL1, SPI1_3_CLK_PIN_SEL);
    }
    else if (PAD_SPI1_3_CLK == pad_name)
    {
        /* select SPI1_0_CS0 as SPI1_3_CLK*/
        CHIP_REG_AND (APB_PIN_CTL1, ~SPI1_3_CLK_PIN_SEL);
    }
#endif

    for (;;)
    {
        if (SPI_PAD_MAX == __spi_pad_func_info[i].spi_pad_name)
        {
            break;
        }

        if (pad_name == __spi_pad_func_info[i].spi_pad_name)
        {
            pin_addr = __spi_pad_func_info[i].addr;

            if (SPI_PAD_SPI == pad_fun)
            {
                pin_func_val = __spi_pad_func_info[i].value_spi;
            }
            else
            {
                pin_func_val = __spi_pad_func_info[i].value_gpio;
            }

            break;
        }

        i++;
    }

    SPI_PRINT ( ("[SPI DRV:]__SPI_PHY_SetPAD: pad_name=0x%x value=0x%x",
                 pin_addr, pin_func_val));

    SCI_ASSERT (pin_addr); /*assert verified*/
    pin_val = CHIP_REG_GET (pin_addr);
    pin_val &= (~SPI_PAD_FUNC_MASK);
    pin_val |= pin_func_val;
    CHIP_REG_SET (pin_addr,  pin_val);
    return ERR_SPI_NONE;
}

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
PUBLIC int32 SPI_PHY_SetPAD_with_GPIO (SPI_PAD_NAME_E pad_name, uint32 pad_fun, uint32 output)
{
    uint32 gpio_id;
    SPI_PHY_SetPAD (pad_name, pad_fun);
    gpio_id = __SPI_PHY_GetPAD_GPIOID (pad_name);
    SCI_ASSERT (gpio_id != INVALID_U32); /*assert verified*/

    if (SPI_PAD_GPIO == pad_fun)
    {
        GPIO_Enable (gpio_id);
        GPIO_SetDirection (gpio_id, 1);
        GPIO_SetValue (gpio_id, output);
    }
    else
    {
        GPIO_Disable (gpio_id);
    }

    return ERR_SPI_NONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
** pingid: pin group id, specify pins used with spi controller
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL int32 SPI_PHY_PinGrp_Sel (uint32 phy_id, uint32 pingid)
{
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    uint32 i;
    uint32 pin_sel_reg;
    SCI_ASSERT (phy_id < SPI_BUS_MAX); /*assert to do*/

    pin_sel_reg = CHIP_REG_GET (APB_PIN_CTL0);

    if (phy_id == 0)
    {
        switch (pingid)
        {
            case 0: //SPI0_0
                /*power on SPI0_0*/
                LDO_TurnOnLDO (LDO_LDO_SDIO);
                pin_sel_reg &= ~ (SPI0_SEL_MSK);//lint !e648 -e648
                break;
            case 1: //SPI0_1
                pin_sel_reg &= ~ (SPI0_SEL_MSK);
                pin_sel_reg |= (0x1 << SPI0_SEL_SHIFT);
                break;
            case 2: //SPI0_2
                pin_sel_reg &= ~ (SPI0_SEL_MSK);
                pin_sel_reg |= (0x2 << SPI0_SEL_SHIFT);
                break;
	    case 3: //SPI0_3
	       pin_sel_reg &= ~ (SPI0_SEL_MSK);
	       pin_sel_reg |= (0x3 << SPI0_SEL_SHIFT);
	       break;
		
            default:
                SCI_ASSERT (0); /*assert to do*/
                break;
        }
    }
    else if (phy_id == 1)
    {
        switch (pingid)
        {
            case 0: //SPI1_0
                /*power on SPI1_0*/
                #if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
                LDO_TurnOnLDO (LDO_LDO_SIM2);
                #endif
                pin_sel_reg &= ~ (SPI1_SEL_MSK);
                break;
            case 1: //SPI1_1
                pin_sel_reg &= ~ (SPI1_SEL_MSK);
                pin_sel_reg |= (0x1 << SPI1_SEL_SHIFT);
                break;
            case 2: //SPI1_2
                pin_sel_reg &= ~ (SPI1_SEL_MSK);
                pin_sel_reg |= (0x2 << SPI1_SEL_SHIFT);
                break;
            case 3: //SPI1_3
                pin_sel_reg &= ~ (SPI1_SEL_MSK);
                pin_sel_reg |= (0x3 << SPI1_SEL_SHIFT);
                break;
            default:
                SCI_ASSERT (0); /*assert to do*/
                break;
        }
    }

    CHIP_REG_SET (APB_PIN_CTL0, pin_sel_reg);

    for (i = 0; i < 100; i++)
    {
        ;
    }
#endif
    return ERR_SPI_NONE;
}

LOCAL int32 SPI_PHY_SetLcdWorkMode (uint32 phy_id, SPI_LCD_MODE_E mode)
{
#if 0
    uint32 spi_addr;
    SPI_PHY_INST *phy_inst;

    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;

    /*wait for last operation complete*/
    SPI_PHY_WaitForBusIdle (spi_addr);

    switch (mode)
    {
        case SPI_3WIRE_9BIT_SDA:
            REG32 (spi_addr + SPI_CTL7) &= ~ (0x7 << 3);
            REG32 (spi_addr + SPI_CTL7) |= 0x1 << 3;
            break;

        case SPI_3WIRE_9BIT_SDIO:
            REG32 (spi_addr + SPI_CTL7) &= ~ (0x7 << 3);
            REG32 (spi_addr + SPI_CTL7) |= 0x2 << 3;
            break;

        case SPI_4WIRE_8BIT_SDA:
            REG32 (spi_addr + SPI_CTL7) &= ~ (0x7 << 3);
            REG32 (spi_addr + SPI_CTL7) |= 0x3 << 3;
            break;

        case SPI_4WIRE_8BIT_SDIO:
            REG32 (spi_addr + SPI_CTL7) &= ~ (0x7 << 3);
            REG32 (spi_addr + SPI_CTL7) |= 0x4 << 3;
            break;
#ifdef CHIP_VER_6531EFM
        case SPI_3WIRE_9BIT_RW:
            REG32 (spi_addr + SPI_CTL7) &= ~ (0x7 << 3);
            REG32 (spi_addr + SPI_CTL7) |= 0x5 << 3;
            break;


        case SPI_4WIRE_8BIT_RW:
            REG32 (spi_addr + SPI_CTL7) &= ~ (0x7 << 3);
            REG32 (spi_addr + SPI_CTL7) |= 0x6 << 3;
            break;
#endif //CHIP_VER_6531EFM

        default:
            SCI_ASSERT (SCI_FALSE);/*assert to do*/
            break;

    }
#endif
    return ERR_SPI_NONE;
}

LOCAL int32 SPI_PHY_SetCdLevel (uint32 phy_id, BOOLEAN is_high)
{
#if 0
    uint32 spi_addr;
    SPI_PHY_INST *phy_inst;

    /*Retrieve register cache*/
    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;

    if (is_high)
    {
        REG32 (spi_addr + SPI_CTL8) |= BIT_15;
    }
    else
    {
        REG32 (spi_addr + SPI_CTL8) &= ~BIT_15;
    }
#endif
    return ERR_SPI_NONE;
}

LOCAL int32 SPI_PHY_GetDataPort (uint32 phy_id, uint32 *reg_base)
{
    uint32 spi_addr;
    SPI_PHY_INST *phy_inst;

    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;

    *reg_base = spi_addr;

    return ERR_SPI_NONE;
}

LOCAL int32 SPI_PHY_SetTransMode (uint32 phy_id, SPI_TXRX_MODE_E mode)
{
#if 0
    uint32 spi_addr;
    SPI_PHY_INST *phy_inst;

    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;

    /*wait for last operation complete*/
    SPI_PHY_WaitForBusIdle (spi_addr);

    switch (mode)
    {
        case SPI_RX_ONLY:
            REG32 (spi_addr + SPI_CTL1) &= ~ (0x3 << 12);
            REG32 (spi_addr + SPI_CTL1) |= 0x1 << 12;
            break;

        case SPI_TX_ONLY:
            REG32 (spi_addr + SPI_CTL1) &= ~ (0x3 << 12);
            REG32 (spi_addr + SPI_CTL1) |= 0x2 << 12;
            break;

        case SPI_RX_AND_TX:
            REG32 (spi_addr + SPI_CTL1) &= ~ (0x3 << 12);
            REG32 (spi_addr + SPI_CTL1) |= 0x3 << 12;
            break;

        case SPI_IDLE:
            REG32 (spi_addr + SPI_CTL1) &= ~ (0x3 << 12);
            break;

        default:
            SCI_ASSERT (SCI_FALSE);/*assert to do*/
            break;

    }
#endif
    return ERR_SPI_NONE;
}

/*
 *Note: The unit of data_len is identical with buswidth
*/
LOCAL int32 SPI_PHY_LCD_Read (uint32 phy_id, uint32 *data_buf, uint32 data_len, uint32 dummy_len)
{
#if 0
    uint32 read_len;
    uint32 spi_addr;
    uint32 bit_len;
    uint32 datawidth_mask;
    uint32 reg_val;
    SPI_PHY_INST *phy_inst;

    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;

    bit_len = (REG32 (spi_addr + SPI_CTL0) >>  SPI_BITLEN_OFFSET) & SPI_BITLEN_MASK;

    if (0x0 != bit_len)
    {
        datawidth_mask = (0x1 << bit_len) - 1;
    }
    else
    {
        /*bit_len is 32 bits*/
        datawidth_mask = 0xffffffff;
    }

    SPI_PHY_WaitForBusIdle (spi_addr);

    /*set data len and dummy len*/
    reg_val = REG32 (spi_addr + SPI_CTL10);
    reg_val &= ~ (DUMY_LEN_MASK << 4 | DATA_LEN_H_MASK);
    reg_val |= (dummy_len << 4) | (data_len >> 16);
    REG32 (spi_addr + SPI_CTL10) = reg_val;
    REG32 (spi_addr + SPI_CTL11) = data_len & DATA_LEN_L_MASK;

    /*send read request*/
    REG32 (spi_addr + SPI_CTL12) |= BIT_0;

    for (read_len = 0; read_len < data_len; read_len++)
    {
        /*check the rx fifo is empty*/
        while (REG32 (spi_addr + SPI_STS2) & SPI_RX_FIFO_REALLY_EMPTY) {};

        data_buf[read_len] = REG32 (spi_addr + SPI_TXD) & datawidth_mask;
    }
    return read_len;
#else
    return 0;
#endif
}

/*
 *Note: The unit of data_len is identical with buswidth
*/
LOCAL int32 SPI_PHY_LCD_Write (uint32 phy_id, uint32 *data_buf, uint32 data_len, uint32 dummy_len)
{
#if 0
    uint32 write_len;
    uint32 spi_addr;
    //uint32 read_data;
    uint32 reg_val;
    SPI_PHY_INST *phy_inst;

    phy_inst = __SPI_PHY_GetPhyInst (phy_id);
    spi_addr = phy_inst->phy_info->base_addr;

    SPI_PHY_WaitForBusIdle (spi_addr);

    /*set data len and dummy len*/
    reg_val = REG32 (spi_addr + SPI_CTL8);
    reg_val &= ~ ( (DUMY_LEN_MASK<<4) | DATA_LEN_H_MASK);
    reg_val |= (dummy_len << 4) | (data_len >> 16);
    REG32 (spi_addr + SPI_CTL8) = reg_val;
    REG32 (spi_addr + SPI_CTL9) = data_len & DATA_LEN_L_MASK;

    /*send write request*/
    REG32 (spi_addr + SPI_CTL12) |= BIT_1;

    for (write_len = 0; write_len < data_len; write_len++)
    {
        /*check the tx fifo is full*//*Pls don't*/
        //while (REG32(spi_addr + SPI_STS2) & SPI_TX_FIFO_REALLY_FULL);
        REG32 (spi_addr + SPI_TXD) = data_buf[write_len];
    }

    SPI_PHY_WaitForBusIdle (spi_addr);

    return ERR_SPI_NONE;
#else
    return 0;
#endif
}

PUBLIC SPI_PHY_FUN spi_phy_fun_v5 =
{
    SPI_PHY_Init,
    SPI_PHY_Cfg,
    SPI_PHY_WriteBytes,
    SPI_PHY_ReadBytes,
    SPI_PHY_ReadWriteBytes, /*rw*/
    SPI_PHY_DmaWrite,
    SPI_PHY_DmaRead,
    SPI_PHY_DmaReadWrite,
    SPI_PHY_SetCS,
    PNULL, /*set mode*/
    PNULL, /*get mode*/
    SPI_PHY_SetBitLen,
    SPI_PHY_GetBitLen,
    SPI_PHY_SetBusClk,
    SPI_PHY_GetBusClk,
    SPI_PHY_Deinit,
    SPI_PHY_SetPAD_with_GPIO,
    SPI_PHY_WriteBytes_Int,
    SPI_PHY_ReadBytes_Int,
    SPI_PHY_ReadWriteBytes_Int,
    SPI_PHY_DmaReadWrite_Int,
    SPI_PHY_PinGrp_Sel,
#if  defined(MAINLCM_INTERFACE_SPI) || defined(SUBLCM_INTERFACE_SPI)
    SPI_PHY_SetLcdWorkMode,
    SPI_PHY_SetCdLevel,
    SPI_PHY_GetDataPort,
    SPI_PHY_SetTransMode,
    SPI_PHY_LCD_Read,
    SPI_PHY_LCD_Write
#endif
};

