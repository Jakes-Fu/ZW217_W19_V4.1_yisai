/******************************************************************************
 ** File Name:      uartcom_drv.c                                             *
 ** Author:         Tao.Zhou                                                  *
 ** DATE:           09/01/2008                                                *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages various operations             *
 **                 related to chip.                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/09/2008     Tao.Zhou         Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_api.h"
#include "com_drvapi.h"
#include "chip_plf_export.h"
#include "uart_drvapi.h"
#include "../uart_phy.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
//#include "uart_reg_v0.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         variable section                                  *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
//global control differents
static __inline void UART_GLOBAL_Reg (uint32 port, BOOLEAN enable)
{
    switch (port)
    {
        case UART_COM0:
            {
                if (enable)
                {
                    REG32 (GR_GEN1) |= BIT_1;
                }
                else
                {
                    REG32 (GR_GEN1) &= ~BIT_1;
                }

                break;
            }
        case UART_COM1:
            {
                if (enable)
                {
                    REG32 (GR_GEN1) |= BIT_2;
                }
                else
                {
                    REG32 (GR_GEN1) &= ~BIT_2;
                }

                break;
            }
        case UART_COM2:
            {
                if (enable)
                {
                    REG32 (GR_GEN1) |= BIT_3;
                    REG32 (GR_SOFT_RST)  |=  SWRST_UART2_RST;
                    {
                        uint32 t = 0;

                        for (t = 0; t < 20; t++) {}
                    }
                    REG32 (GR_SOFT_RST)  &=  ~SWRST_UART2_RST;
                }
                else
                {
                    REG32 (GR_GEN1) &= ~BIT_3;
                }

                break;
            }
        default:
            {
                SCI_ASSERT (0);/*assert verified*/
                break;
            }
    }
}

//
LOCAL ISR_EXE_T uart0_port_irq (uint32 port);
LOCAL ISR_EXE_T uart1_port_irq (uint32 port);
LOCAL ISR_EXE_T uart2_port_irq (uint32 port);

PUBLIC UART_HW_CONFIG_T hw_config =
{
    {ARM_UART0_BASE, ARM_UART1_BASE, ARM_UART2_BASE, 0},
    {UART0_FIFO_SIZE, UART1_FIFO_SIZE, UART2_FIFO_SIZE, 0},
    {   {TB_UART0_INT, uart0_port_irq}, {TB_UART1_INT, uart1_port_irq},
        {TB_UART2_INT, uart2_port_irq}, {0, NULL}
    },
    3,
    48
};
UART_INFO_S s_uart_port[3];

PUBLIC void UART_InitUartLowLevel (uint32 port, uint32 baudrate)
{
    uint32 uart_base = 0;

    uart_base = hw_config.baseAddr[port-UART_COM0];
    UART_GLOBAL_Reg (port, TRUE);

    REG32 (uart_base + ARM_UART_IEN)   = (uint32) 0x00;
    REG32 (uart_base + ARM_UART_CTL0)  = (uint32) 0x1C;
    REG32 (uart_base + ARM_UART_CTL1)  = (uint32) 0x00;
    REG32 (uart_base + ARM_UART_CLKD0) = (uint32) ( (CHIP_GetAPBClk() + (baudrate / 2)) / (baudrate));
    REG32 (uart_base + ARM_UART_CLKD1) = (uint32) 0x00;
}

INPUT_BUFFER_INIT (int32,3)
PUBLIC void  Uart_CallbackHisr (uint32 data, void *pData)
{
    uint32 mask_sts;
    uint32 raw_sts;
    uint32 tmp_char = 0;
    int32 port = 0;
    volatile UART_REG_S *uart_reg_ptr = NULL;

    while (!threadReadBuffer (&port))
    {
        SCI_ASSERT (port < 3);/*assert verified*/

        uart_reg_ptr = (volatile UART_REG_S *) hw_config.baseAddr[port-UART_COM0];
        SCI_ASSERT (uart_reg_ptr != NULL);/*assert verified*/

        mask_sts = uart_reg_ptr->masked_sts;
        raw_sts  = uart_reg_ptr->sts0;

        if (raw_sts & US_BREAK)
        {
            uart_reg_ptr->iclr |= US_BREAK;
            UART_PHY_WaitRxFifoEmpty (port, 0, tmp_char);
        }

        // clear irq status first, protect from clearing successive two irqs at one time
        uart_reg_ptr->iclr |= UART_ICLR_MASK;

        if (s_uart_port[port].callback != NULL)
        {
            if (mask_sts & (uint32) US_TX_EMPTY)
            {
                //tx fifo empty interrupt disable, it will be reset when UART_Write is called
                uart_reg_ptr->ien &= ~ ( (US_TX_EMPTY) &UART_IEN_MASK);
                (s_uart_port[port].callback) (EVENT_WRITE_COMPLETE);
            }

            if ( (mask_sts & (uint32) US_RX_FULL) || (mask_sts & (uint32) US_RXT_OUT)) //notify having data to read
            {
                (s_uart_port[port].callback) (EVENT_DATA_TO_READ);
            }
        }

        CHIPDRV_EnableIRQINT (hw_config.inter[port].int_num);
    }

}

/*****************************************************************************/
//  Description:   uart0 irq handler                                         *
//  Author:        weihua.wang                                               *
//  Note:          NONE                                                      *
/*****************************************************************************/
LOCAL ISR_EXE_T uart0_port_irq (uint32 port)
{
    CHIPDRV_DisableIRQINT (hw_config.inter[UART_COM0].int_num);
    IsrWriteBuffer (UART_COM0);
    return CALL_HISR;
}

/*****************************************************************************/
//  Description:   uart1 irq handler                                         *
//  Author:        weihua.wang                                               *
//  Note:          NONE                                                      *
/*****************************************************************************/
LOCAL ISR_EXE_T uart1_port_irq (uint32 port)
{
    CHIPDRV_DisableIRQINT (hw_config.inter[UART_COM1].int_num);
    IsrWriteBuffer (UART_COM1);
    return CALL_HISR;
}

LOCAL ISR_EXE_T uart2_port_irq (uint32 port)
{
    CHIPDRV_DisableIRQINT (hw_config.inter[UART_COM2].int_num);
    IsrWriteBuffer (UART_COM2);
    return CALL_HISR;
}

/*****************************************************************************/
//  Description:   This function initilizes uart port, configure uart
//                   registers, interrupt service, and register callback
//                   function for notifying the status of uart read and write.
//  Param:         uint32 port: usb virtual com num USB_COM0/USB_COM1
//                 UART_INIT_PARA_T* :include param for uart, only useful for uart
//                 UartCallback func :uplayer callback
//  Note:          NONE
/*****************************************************************************/
PUBLIC uint32 UART_PHY_Initilize (uint32 port,
                                  UART_INIT_PARA_T *para)
{
    uint32 type;
    uint16 ctl0 = 0;

    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);
    UART_GLOBAL_Reg (port, TRUE);

    if (DS_WAKEUP_ENABLE == para->ds_wakeup_en)
    {
        type = para->ds_wakeup_type;
        SCI_ASSERT ( (type > DS_WAKEUP_TYPE_START) && (type < DS_WAKEUP_TYPE_MAX));/*assert verified*/

        switch (port)
        {
            case UART_COM0:
                {
                    if (type == DS_WAKEUP_BY_RXD)
                    {
                        REG32 (INT_UINT_CTL) |= (UART0_RXD_SPC_ENABLE | UART0_RXD_SPC_ENABLE<<16);
                    }
                    else if (type == DS_WAKEUP_BY_CTS)
                    {
                        REG32 (INT_UINT_CTL) |= (UART0_CTSN_SPC_ENABLE | UART0_CTSN_SPC_ENABLE<<16);
                    }
                    else
                    {
                        SCI_ASSERT (0);/*assert to do*/
                    }

                    break;
                }
            case UART_COM1:
                {
                    if (type == DS_WAKEUP_BY_RXD)
                    {
                        REG32 (INT_UINT_CTL) |= (UART1_RXD_SPC_ENABLE  | UART1_RXD_SPC_ENABLE<<16);
                    }
                    else
                    {
                        SCI_ASSERT (0);/*assert to do*/
                    }

                    break;
                }
            case UART_COM2:
                {
                    if (type == DS_WAKEUP_BY_RXD)
                    {
                        REG32 (INT_UINT_CTL) |= (UART2_RXD_SPC_ENABLE | UART2_RXD_SPC_ENABLE<<16);
                    }
                    else
                    {
                        SCI_ASSERT (0);/*assert to do*/
                    }

                    break;
                }
            default:
                {
                    SCI_ASSERT (0);/*assert to do*/
                    break;
                }
        }

        UARTCOM_DslpFuncSts_Set (port, SCI_TRUE);
    }
    else
    {
        UARTCOM_DslpFuncSts_Set (port, SCI_FALSE);
    }

    p->ien &= ~ (UART_IEN_MASK);

    if (para->parity_enable != PARITY_DISABLE)
    {
        ctl0 |= para->parity;
        ctl0 |= UC_PARITY_ENABLE;
    }

    ctl0 |= (para->byte_size << UC_BYTE_SIZE_SHIFT) | (para->stop_bits << UC_STOP_BIT_NUBMER_SHIFT);
    p->ctl = ctl0;

    //set water mark
    p->watermark = (para->rx_watermark << UC_RX_WATERMARK_SHIFT)
                   + (para->tx_watermark << UC_TX_WATERMARK_SHIFT);

    if (HW_FLOW_CONTROL == para->flow_control) //flow enable
    {
        p->flow_ctl = (para->rx_watermark) | RECEIVE_TIMEOUT  // Receive time out value
                      | UC_RX_HW_FLOW_CTL_EN | UC_TX_HW_FLOW_CTL_EN;     // enable Rx/Tx hardware_flow control
    }
    else
    {
        p->flow_ctl = RECEIVE_TIMEOUT | (para->rx_watermark);
    }

    UART_SetBaudSpeed (port, para->baud_rate);

    p->ien |= ( (US_RX_FULL | US_RXT_OUT) & UART_IEN_MASK);

    return UART_SUCCESS;
}
/*****************************************************************************/
//  Description:  This function close uart port
//  Author:       weihua.wang
//  Param :       uint32 port: usb virtual com number
//  Note:         NONE
/*****************************************************************************/
PUBLIC uint32 UART_PHY_Close (uint32 port)
{
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    UART_GLOBAL_Reg (port, FALSE);
    p->ien &= ~ (UART_IEN_MASK);    
    
    /* clear uart deepsleep wakeup control bit */
    switch (port)
    {
        case UART_COM0:
            CHIP_REG_AND (INT_UINT_CTL,~(INTCTL_SPC_UART0_RXD_BIT|INTCTL_SPC_UART0_CTSN_BIT));
            break;
        case UART_COM1:
            CHIP_REG_AND (INT_UINT_CTL, ~INTCTL_SPC_UART1_RXD_BIT);
            break;
        case UART_COM2:
            CHIP_REG_AND (INT_UINT_CTL, ~INTCTL_SPC_UART2_RXD_BIT);
            break;
        default:
            SCI_PASSERT (0, ("[UART]:Invalidate uart com number:%d(should be 0~3)", port));/*assert to do*/
            return UART_FAILURE; /*lint !e527 comfirmed by windy.wang*/
    }
    
    return UART_SUCCESS;
}

LOCAL BOOLEAN UART_PHY_IsHwFlowControl (volatile UART_REG_S *p)
{
    BOOLEAN condition = FALSE;
    uint32 flow_ctl;
    flow_ctl = (p->flow_ctl & UART_CTL1_MASK);

    if (flow_ctl & (UC_RX_HW_FLOW_CTL_EN | UC_TX_HW_FLOW_CTL_EN))
    {
        condition = TRUE;
    }

    return condition;
}
/*****************************************************************************/
//  Description:   This function read data from receiving buffer directly.   *
//  Author:        Tao.Zhou                                                  *
//  Param :        UART_REG_S *p   : uart register base address pointer      *
//                 uint8* buffer   : des buffer header address               *
//                 uint32 length   : size to be read                         *
//  Return:        if success,return size read in effect                     *
//  Note  :        NONE                                                      *
/*****************************************************************************/
uint32 UART_PHY_ReadData (uint32 port, uint8 *buffer, uint32 length)
{
    uint32 count = 0;
    uint32   cur_tick     = SCI_GetTickCount();
    uint32   next_tick   = SCI_GetTickCount();
    uint32 i;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    i = p->fifo_cnt & US1_RX_FIFOCNT;

    if (0 == length || 0 == i)
    {
        return 0;
    }

    if (length < i)
    {
        i = length;
    }

    for (; i > 0; i--)
    {
        *buffer++ = p->rxd;
        length--;
        count++;
    }

    while (length > 0 && (p->fifo_cnt & US1_RX_FIFOCNT))
    {
        *buffer++ = p->rxd;
        length--;
        count++;

        if (!UART_PHY_IsHwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK ( (length > 0 && (p->fifo_cnt & US1_RX_FIFOCNT)));/*assert verified*/
        }
    }

    return count;
}

/******************************************************************************/
//  Description:   This function record data which will be written to         *
//                  tx fifo and enable tx fifo empty interrupt. The length    *
//                  of data should be not larger than UART_FIFO_SIZE(127B).        *
//  Author:        Tao.Zhou                                                   *
//  Param :        UART_REG_S *p   : uart register base address pointer       *
//                 uint8* buffer   : src buffer header address                *
//                 uint32 length   : size to be writed                        *
//  Return:        if success,return size writed in effect                    *
//  Note  :        NONE                                                       *
/******************************************************************************/
uint32 UART_PHY_WriteData (uint32 port, const uint8 *buffer, uint32 length, BOOLEAN irq_en)
{
    uint32 count = 0;
    uint32   cur_tick     = SCI_GetTickCount();
    uint32   next_tick   = SCI_GetTickCount();
    uint32 i;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    if (0 == length)
    {
        return 0;
    }

    i = hw_config.fifoSize[port-UART_COM0] - ( ( (p->fifo_cnt) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT);

    if (length < i)
    {
        i = length;
    }

    for (; i > 0; i--)
    {
        p->txd = *buffer++;
        length--;
        count++;
    }

    while (length > 0 && ( ( (p->fifo_cnt) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT) < hw_config.fifoSize[port-UART_COM0])
    {
        p->txd = *buffer++;
        length--;
        count++;

        if (!UART_PHY_IsHwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK ( (length > 0 && ( ( (p->fifo_cnt) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT) < hw_config.fifoSize[port-UART_COM0]));/*assert verified*/
        }
    }

    if (irq_en)
    {
        p->ien |= ( (US_TX_EMPTY) & UART_IEN_MASK);
    }

    return count;
}

/*****************************************************************************/
//  Description:   This function get char from uart receive register         *
//                   directly.                                               *
//  Author:        Tao.Zhou                                                  *
//  Return value:  Returns a char from uart receive register.                *
//  Note:          return -1 indicates that no char received from fifo       *
/*****************************************************************************/
int32 UART_PHY_GetChar (uint32 port)
{
    uint8 nchar;
    uint32 status;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    status = p->sts0;

    if (status)
    {
        /* the char is invalid, try next  */
        p->iclr |= UART_ICLR_MASK;
    }

    if (! (p->fifo_cnt & US1_RX_FIFOCNT))
    {
        return -1;
    }

    nchar = (uint8) ( (p)->rxd);

    return nchar;
}

/*****************************************************************************/
//  Description:   This function put a char to the uart send register        *
//                   directly                                                *
//  Author:        Tao.Zhou                                                  *
//  Return value:  NONE                                                      *
//  Note:          NONE                                                      *
/*****************************************************************************/
void UART_PHY_DumpPutChar (uint32 port, uint8 nchar)
{
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);
    /* tx fifo is empty, so try send char  */
    UART_PHY_WaiTxFifoEmpty (port, hw_config.fifoSize[port-UART_COM0] - 1);

    /* write to tx_fifo */
    p->txd = (uint8) (nchar);
}

/*****************************************************************************/
//  Description:   This function set baudrate parameter directly             *
//  Author:        Tao.Zhou                                                  *
//  Return value:  NONE                                                      *
//  Note:          NONE                                                      *
/*****************************************************************************/
void UART_PHY_SetBaudSpeed (uint32 port, uint32 baudrate)
{
    uint32 divider;
    uint32 apbclk;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    apbclk = CHIP_GetAPBClk();
    divider = apbclk / ARM_CLK_13M * baudrate;

    p->clk0 = divider & UART_CKD0_MASK;
    p->clk1 = (divider >> 16) & UART_CKD1_MASK;
}

/*****************************************************************************/
//  Description:   This function set uart parity parameter directly          *
//  Author:        Tao.Zhou                                                  *
//  Return value:  NONE                                                      *
//  Note:          NONE                                                      *
/*****************************************************************************/
void UART_PHY_SetParity (uint32 port, UART_PARITY_E parity)
{
    uint32 ctl0;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->ctl & ~UC_PARITY);
    ctl0 |= (parity << UC_PARITY_SHIFT);
    p->ctl = ctl0;
}

/*****************************************************************************/
//  Description:   uart parity enable/disable directly                       *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
void UART_PHY_SetParityEnable (uint32 port, UART_PARITY_SET_E is_enable)
{
    uint32 ctl0;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->ctl & ~UC_PARITY_ENABLE);
    ctl0 |= (is_enable << UC_PARITY_ENABLE_SHIFT);
    p->ctl = ctl0;
}

/*****************************************************************************/
//  Description:   get uart parity enable/disable directly                   *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
UART_PARITY_SET_E UART_PHY_GetParityEnable (uint32 port)
{
    uint32 ctl0;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->ctl & UC_PARITY_ENABLE) >> UC_PARITY_ENABLE_SHIFT;

    if (ctl0)
    {
        return PARITY_ENABLE;
    }
    else
    {
        return PARITY_DISABLE;
    }
}

/*****************************************************************************/
//  Description:   This function set uart byte size directly                 *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetWordSize (uint32 port, UART_BYTE_SIZE_E byte_size)
{
    uint32 ctl0;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->ctl & ~UC_BYTE_SIZE);
    ctl0 |= (byte_size << UC_BYTE_SIZE_SHIFT);
    p->ctl = ctl0;
}

/*****************************************************************************/
//  Description:   This function set uart stop bits directly                 *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetStopBits (uint32 port, UART_STOP_BIT_E stop_bits)
{
    uint32 ctl0;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->ctl & ~UC_STOP_BIT_NUBMER);
    ctl0 |= (stop_bits << UC_STOP_BIT_NUBMER_SHIFT);
    p->ctl = ctl0;
}

/*****************************************************************************/
//  Description:   This function set uart tx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetTxWaterMark (uint32 port, uint16 tx_watermark)
{
    uint32 watermark;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    watermark = (p->watermark & ~UC_TX_WATERMARK);
    watermark |= (tx_watermark << UC_TX_WATERMARK_SHIFT);
    p->watermark = watermark;
}

/*****************************************************************************/
//  Description:   This function get uart tx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint16 UART_PHY_GetTxWaterMark (uint32 port)
{
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    return ( (p->watermark & UC_TX_WATERMARK) >> UC_TX_WATERMARK_SHIFT);
}

/*****************************************************************************/
//  Description:   This function set uart rx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetRxWaterMark (uint32 port, uint16 rx_watermark)
{
    uint32 watermark;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    watermark = (p->watermark & ~UC_RX_WATERMARK);
    watermark |= (rx_watermark << UC_RX_WATERMARK_SHIFT);
    p->watermark = watermark;
}

/*****************************************************************************/
//  Description:   This function get uart rx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint16 UART_PHY_GetRxWaterMark (uint32 port)
{
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);
    return ( (p->watermark & UC_RX_WATERMARK) >> UC_RX_WATERMARK_SHIFT);
}

/*****************************************************************************/
//  Description:    return the fifo count in uart tx fifo directly
//  Global resource dependence:  NONE
//  Author:         Tao.Zhou
//  Param:          UART_REG_S *p: uart register base address pointer
//  Note:           NONE
/*****************************************************************************/
PUBLIC uint32 UART_PHY_GetTxFifoCnt (uint32 port)
{
    uint32 tx_fifo_cnt;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);
    tx_fifo_cnt = (p->fifo_cnt & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT;
    return tx_fifo_cnt;
}

/*****************************************************************************/
//  Description:    return the Tx fifo is busy or idle
//  Global resource dependence:  NONE
//  Author:         Jeff.Li
//  Param:          UART_REG_S *p: uart register base address pointer
//  Note:           NONE
/*****************************************************************************/
PUBLIC BOOLEAN UART_PHY_GetIsTxFifoBusy (uint32 port)
{
    return 0;
}
//  Description:    return the fifo count in uart rx fifo directly
//  Global resource dependence:  NONE
//  Author:         Tao.Zhou
//  Param:          UART_REG_S *p: uart register base address pointer
//  Note:           NONE
/*****************************************************************************/
PUBLIC uint32 UART_PHY_GetRxFifoCnt (uint32 port)
{
    uint32 rx_fifo_cnt;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);
    rx_fifo_cnt = (p->fifo_cnt & US1_RX_FIFOCNT) >> US1_RX_FIFOCNT_SHIFT;

    return rx_fifo_cnt;
}

/*****************************************************************************/
//  Description:   uart hardware flow control enable/disable directly        *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetFlowCtl (uint32 port, UART_FLOW_CTR_E is_enable)
{
    uint32 flow_ctl;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    flow_ctl = (p->flow_ctl & UART_CTL1_MASK);

    if (HW_FLOW_CONTROL == is_enable)
    {
        flow_ctl |= (uint32) (UC_RX_HW_FLOW_CTL_EN | UC_TX_HW_FLOW_CTL_EN);
    }
    else
    {
        flow_ctl &= ~ (uint32) (UC_RX_HW_FLOW_CTL_EN | UC_TX_HW_FLOW_CTL_EN);
    }

    p->flow_ctl = flow_ctl;
}

PUBLIC void UART_PHY_SendBreak (uint32 port, BOOLEAN is_enable)
{
    uint32 ctl0;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->ctl & UART_CTL0_MASK);

    if (is_enable)
    {
        ctl0 |= (uint32) (UC_SEND_BREAK);
    }
    else
    {
        ctl0 &= ~ (uint32) (UC_SEND_BREAK);
    }

    p->ctl = ctl0;
}
/*****************************************************************************/
//  Description:   uart fifo control by DAM enable/disable directly          *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetDmaMode (uint32 port, BOOLEAN is_enable)
{
    uint32 ctl1;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    ctl1 = (p->flow_ctl & UART_CTL1_MASK);

    if (is_enable)
    {
        ctl1 |= (uint32) UC_DMA_ENABLE;
    }
    else
    {
        ctl1 &= ~ (uint32) UC_DMA_ENABLE;
    }

    p->flow_ctl = ctl1;
}

/*****************************************************************************/
//  Description:   enable/disable self loop mode directly                    *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetSelfLoop (uint32 port, BOOLEAN is_enable)
{
    uint32 ctl1;
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    ctl1 = (p->flow_ctl & UART_CTL1_MASK);

    if (is_enable)
    {
        ctl1 |= (uint32) UC_SLEFLOOP_ENABLE;
    }
    else
    {
        ctl1 &= ~ (uint32) UC_SLEFLOOP_ENABLE;
    }

    p->flow_ctl = ctl1;
}
/*****************************************************************************/
//  Description:   Get the FIFO size of UART
//  Global resource dependence : NONE
//  Author:        Tao.Zhou
//  Note:          NONE
/*****************************************************************************/
PUBLIC uint32 UART_PHY_GetFifoSize (uint32 port)
{
    return hw_config.fifoSize[port-UART_COM0];
}
/*****************************************************************************/
//  Description:    Clear send buffer
//  Global resource dependence: NONE
//  Author:         Tao.Zhou
//  Param :         p: uart register pointer, UART_REG_S struct;
//                  fifo_size: the number of data in the fifo
//                  time_out: time out limit
//  Note:           NONE
/*****************************************************************************/
PUBLIC void  UART_PHY_WaiTxFifoEmpty (uint32 port, uint32 fifo_size)
{
    uint32   cur_tick     = SCI_GetTickCount();
    uint32   next_tick   = SCI_GetTickCount();

    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    while ( (fifo_size) < ( ( (p)->fifo_cnt & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT))
    {
        if (!UART_PHY_IsHwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK ( ( (fifo_size) < ( ( (p)->fifo_cnt & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT)));/*assert verified*/
        }
    }
}
/*****************************************************************************/
//  Description:    Clear receive buffer
//  Global resource dependence: NONE
//  Author:         Tao.Zhou
//  Param :         p: uart register pointer, UART_REG_S struct;
//                      fifo_size: the number of data in the fifo
//                  time_out: time out limit
//                  nchar: temp value used reserving the data read out
//  Note:           NONE
/*****************************************************************************/
PUBLIC void UART_PHY_WaitRxFifoEmpty (uint32 port, uint32 fifo_size, int nchar)
{
    uint32   cur_tick     = SCI_GetTickCount();
    uint32   next_tick   = SCI_GetTickCount();
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    while ( (fifo_size) < ( ( (p)->fifo_cnt & US1_RX_FIFOCNT) >> US1_RX_FIFOCNT_SHIFT))
    {
        if (!UART_PHY_IsHwFlowControl (p))
        {
            nchar = (int) ( (p)->rxd);
            UART_TRANSFER_TIME_OUT_CHECK ( ( (fifo_size) < ( ( (p)->fifo_cnt & US1_RX_FIFOCNT) >> US1_RX_FIFOCNT_SHIFT)));/*assert verified*/
        }
    }
}

PUBLIC void UART_PHY_TxInEnable (uint32 port, BOOLEAN is_enable)
{
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    if (is_enable)
    {
        p->ien |= US_TX_EMPTY;
    }
    else
    {
        p->ien &= ~ ( (US_TX_EMPTY) &UART_IEN_MASK);
    }
}

PUBLIC void UART_PHY_RxIntEnable (uint32 port, BOOLEAN is_enable)
{
    volatile UART_REG_S *p  = (volatile UART_REG_S *) (hw_config.baseAddr[port-UART_COM0]);

    if (is_enable)
    {
        p->ien |= ( (US_RX_FULL | US_RXT_OUT) & UART_IEN_MASK);
    }
    else
    {
        p->ien &= ~ ( (US_RX_FULL | US_RXT_OUT) &UART_IEN_MASK);
    }
}

/*****************************************************************************/
//  Description:   Get uart request address                                  *
//  Author:        changde                                                   *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint32 UART_PHY_GetTxReqAddr(uint32 port) 
{
    return 0;
}

PUBLIC uint32 UART_PHY_GetRxReqAddr(uint32 port) 
{
    return 0;
}

PUBLIC uint32 UART_PHY_GetDmaTXChannel (uint32 port)
{
    uint32 ret = 0xFFFFFFFF;

    SCI_ASSERT (port < 3);/*assert verified*/

    switch (port)
    {
        case UART_COM0:
            {
                ret = DMA_UART0_TX;
                break;
            }
        case UART_COM1:
            {
                ret = DMA_UART1_TX;
                break;
            }

        case UART_COM2:
            {
                ret = DMA_UART2_TX;
                break;
            }

        default:
            {
                SCI_PASSERT (0, ("[UART]:Invalidate uart com number:%d(should be 0~3)", port));/*assert verified*/
                break;
            }
    }

    return ret;
}

PUBLIC void UART_PHY_Arbitrate (uint32 pord_id, uint is_ARM)
{
    if (is_ARM)
    {
        if (1 == pord_id)
        {
            REG32 (GR_MISC0) &= ~MISC0_UART1_MUX_SEL;
        }
        else
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }
    else
    {
        if (1 == pord_id)
        {
            REG32 (GR_MISC0) |= MISC0_UART1_MUX_SEL;
        }
        else
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }

}

PUBLIC uint32 GetUART0BaseAddr (void)
{
    return ARM_UART0_BASE;
}

PUBLIC uint32 getRMUartBaseAddr (void)
{
    return ARM_UART1_BASE;
}

PUBLIC ISR_EXE_T UartDSlp_Handler (uint32 param)
{
    uint32 uintsts = REG32 (INT_IRQ_UINT_STS);

    param = param; // avoid compiler warning
    //get which uart i/o pin trigged interrrupt,and awaked the chip.

    if (uintsts & UART_INT_STS_MASK)
    {
        if (uintsts & UART0_INT_STS_MASK)
        {
            if (UARTCOM_DslpFuncSts_Get (UART_COM0))
            {
                DPSLP_UART_EnableApbSleep (UART0_E,DISABLE_APB_SLEEP);
            }
        }

        if (uintsts & UART1_INT_STS_MASK)
        {
            if (UARTCOM_DslpFuncSts_Get (UART_COM1))
            {
                SIO_SetUartRxWakeup(UART_COM1, SCI_TRUE);               
                DPSLP_UART_EnableApbSleep (UART1_E,DISABLE_APB_SLEEP);
            }
        }

        if (uintsts & UART2_INT_STS_MASK)
        {
            if (UARTCOM_DslpFuncSts_Get (UART_COM2))
            {
                DPSLP_UART_EnableApbSleep (UART2_E,DISABLE_APB_SLEEP);
            }
        }

        if (uintsts & USBINPUT_SE0_INT_STS_MASK)
        {
        }

    }

    /* clear Uint_status bits,
       one problem: perhaps once set bit_15,it means you
       disable the uart i/o pin interrrupt,so we add to clear bit_15
    */
    REG32 (INT_UINT_CTL) |= BIT_15;
    REG32 (INT_UINT_CTL) &= ~ (BIT_15); //clear bit_15
    return ISR_DONE;
}


PUBLIC serial_config_funs serial_set_func =
{
    UART_PHY_SetBaudSpeed,
    UART_PHY_SetParity,
    UART_PHY_SetParityEnable,
    UART_PHY_SetWordSize,
    UART_PHY_SetStopBits,
    UART_PHY_SetTxWaterMark,
    UART_PHY_SetRxWaterMark,
    UART_PHY_SetFlowCtl,
    UART_PHY_SetDmaMode,
    UART_PHY_SetSelfLoop,
};

PUBLIC serial_get_funs serial_get_func =
{
    UART_PHY_GetParityEnable,
    UART_PHY_GetTxWaterMark,
    UART_PHY_GetRxWaterMark,
    UART_PHY_GetTxFifoCnt,
    UART_PHY_GetIsTxFifoBusy,
    UART_PHY_GetRxFifoCnt,
    UART_PHY_GetFifoSize,
};

PUBLIC UART_CB_T uart_cb =
{
    (UART_INFO_S *) s_uart_port,
    (UART_HW_CONFIG_T *) (&hw_config),
    &serial_set_func,
    &serial_get_func
};

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/*  End Of File */
