/******************************************************************************
 ** File Name:      uart_phy_r3p0.c                                             *
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
#include "sio.h"
#include "com_drvapi.h"
#include "chip_plf_export.h"
#include "uart_drvapi.h"
#include "../uart_phy.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "uart_reg_uix8910.h"
#include "gpio_drvapi.h"
#include "../../eic/eic_cfg.h"
#include "eic_hal.h"
#include "../../sys_mail/uix8910/mailbox_reg_uix8910.h"

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
struct HAL_IP_SUSPEND_RESUME_T            
{
    char module_name[10];
    void (*reg_suspend)();                       //func addr of reg save
    void (*reg_resume)();                        //func addr of reg resume
    struct HAL_IP_SUSPEND_RESUME_T *next;        //next node addr
};

typedef struct
{   
    uint32	SavedUART_BAUD; 
    uint32	SavedUART_RXTRIG; 
    uint32	SavedUART_TXTRIG; 
    uint32	SavedUART_DELAY;  
    uint32	SavedUART_CONF;
}HAL_SAVED_UARTREGS_T;

HAL_SAVED_UARTREGS_T g_uart_regs_data[3];
static struct HAL_IP_SUSPEND_RESUME_T g_uart_node[3];
uint32 g_sysmail31_value;

typedef void (*pfunc_void)(void);
void UART_PHY_SuspendResumeLogin(uint32 port);
void UART_PHY_SuspendResumeLogout(uint32 port);
void UART_PHY_Port3RegSave(void);

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
//define DMA req_id_addr for UART module
#define UART1_RX_REQ          (0)
#define UART1_TX_REQ          (1)
#define UART2_RX_REQ          (2)
#define UART2_TX_REQ          (3)
#define UART3_RX_REQ          (4)
#define UART3_TX_REQ          (5)

#define ARM_UART0_BASE        (REG_UART1_BASE)
#define ARM_UART1_BASE        (REG_UART2_BASE)
#define ARM_UART2_BASE        (REG_UART3_BASE)
#define ZSP_UART_BASE           (REG_UART4_BASE)

#define UART0_FIFO_SIZE        (120)
#define UART1_FIFO_SIZE        (120)
#define UART2_FIFO_SIZE        (120)  
#define ZSPUART_FIFO_SIZE    (120)

#define US1_RX_FIFOCNT     0xff
#define US1_TX_FIFOCNT     0xff

/* UART sts1 register bit position definition */
#define US1_RX_FIFOCNT_SHIFT 0x0
#define US1_TX_FIFOCNT_SHIFT 0x0

#define UART_TRANSFER_TIME_OUT 1000 //1000 ms

#define RECEIVE_TIMEOUT    0xFF
#define TX_DELAY           0x0

#if 0 //defined(FPGA_SUPPORT_UIX8910)
#define  UART_BAUD_BASE_CLK    48000000
#define  UART_BAUD_3M_NSET     8
#define  UART_BAUD_3M_DIV      1
#define  UART_BAUD_3M25_NSET     8
#define  UART_BAUD_3M25_DIV      0
#define  UART_BAUD_4M_NSET     6
#define  UART_BAUD_4M_DIV      1
#define  UART_BAUD_6M_NSET     4
#define  UART_BAUD_6M_DIV      1
#define  UART_BAUD_8M_NSET     6
#define  UART_BAUD_8M_DIV      0
#else
#if 0   //26MHz
#define  UART_BAUD_BASE_CLK    26000000
#define  UART_BAUD_3M25_NSET     8
#define  UART_BAUD_3M25_DIV      0
#define  UART_BAUD_4M_NSET     6
#define  UART_BAUD_4M_DIV      0
#define  UART_BAUD_8M_NSET     6
#define  UART_BAUD_8M_DIV      1
#else
#define  UART_BAUD_BASE_CLK    96000000
#define  UART_BAUD_3M25_NSET     16
#define  UART_BAUD_3M25_DIV      1  
#define  UART_BAUD_4M_NSET     12
#define  UART_BAUD_4M_DIV      1
#define  UART_BAUD_8M_NSET     6
#define  UART_BAUD_8M_DIV      1
#endif
#endif

#define  UART_SAVE_REG_NUM     (5)
/*---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
//global control differents
static __inline void UART_GLOBAL_Reg (uint32 port, BOOLEAN enable)
{
#if 0
    switch (port)
    {
        case UART_COM0:
            {
                if (enable)
                {
                    CHIP_REG_SET (APB_EB0_SET, UART0_EB_SET);
                }
                else
                {
                    CHIP_REG_SET (APB_EB0_CLR, UART0_EB_CLR);
                }

                break;
            }
        case UART_COM1:
            {
                if (enable)
                {
                    CHIP_REG_SET (APB_EB0_SET, UART1_EB_SET);
                }
                else
                {
                    CHIP_REG_SET (APB_EB0_CLR, UART1_EB_CLR);
                }

                break;
            }

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
                break;
            }
    }
#endif
}

/*****************************************************************************/
//  Description:   Get uart request address                                  *
//  Author:        changde                                                   *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint32 UART_PHY_GetTxReqAddr (uint32 port)
{
#if 0
    SCI_ASSERT (port < UART_MAX);/*assert verified*/

    switch (port)
    {
        case UART_COM0:
            {
                return (UART0_TX_REQ_ADDR);//lint !e778 confirmed by windy.wang
            }
        case UART_COM1:
            {
                return (UART1_TX_REQ_ADDR);
            }

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
                break;
            }
    }
#endif
    return 0;//lint !e527 for Coverity & Pclint tools
}

PUBLIC uint32 UART_PHY_GetRxReqAddr (uint32 port)
{
#if 0
    SCI_ASSERT (port < UART_MAX);/*assert verified*/

    switch (port)
    {
        case UART_COM0:
            {
                return (UART0_RX_REQ_ADDR);
            }
        case UART_COM1:
            {
                return (UART1_RX_REQ_ADDR);
            }
        default:
            {
                SCI_ASSERT (0);/*assert verified*/
                break;
            }
    }
#endif
    return 0;//lint !e527 for Coverity & Pclint tools
}

//
LOCAL ISR_EXE_T uart0_port_irq (uint32 port);
LOCAL ISR_EXE_T uart1_port_irq (uint32 port);
LOCAL ISR_EXE_T uart2_port_irq (uint32 port);
LOCAL ISR_EXE_T uartzsp_port_irq (uint32 port);

PUBLIC UART_HW_CONFIG_T hw_config =
{
    {ARM_UART0_BASE, ARM_UART1_BASE, ARM_UART2_BASE, ZSP_UART_BASE},
    {UART0_FIFO_SIZE, UART1_FIFO_SIZE, UART2_FIFO_SIZE, ZSPUART_FIFO_SIZE},
    {   {TB_UART0_INT, uart0_port_irq},
        {TB_UART1_INT, uart1_port_irq},
        {TB_UART2_INT, uart2_port_irq},
        {TB_ZSPUART_INT, uartzsp_port_irq},
    },
    4,
    32
};
UART_INFO_S s_uart_port[4];

PUBLIC void UART_InitUartLowLevel (uint32 port, uint32 baudrate)
{
    uint32 uart_base = 0;
    HWP_ARM_UART_T *p;

    uart_base = hw_config.baseAddr[port-UART_COM0];
    p  = (HWP_ARM_UART_T*)uart_base;
    UART_GLOBAL_Reg (port, TRUE);

    p->uart_status = 0xFF;
    p->uart_delay = (0xFF | 0 << 8);
    p->uart_conf = (ARM_UART_TXRST | ARM_UART_RXRST);
    UART_PHY_BaudSet(port, p, baudrate);
}

INPUT_BUFFER_INIT (int32,3)
PUBLIC void  Uart_CallbackHisr (uint32 data, void *pData)
{
    uint32 mask_sts;
    uint32 raw_sts;
    uint32 tmp_char = 0;
    int32 port = 0;
    HWP_ARM_UART_T *uart_reg_ptr = NULL;

    while (!threadReadBuffer (&port))
    {
        SCI_ASSERT (port < 3);/*assert verified*/

        uart_reg_ptr = (HWP_ARM_UART_T*) hw_config.baseAddr[port-UART_COM0];
        SCI_ASSERT (uart_reg_ptr != NULL);/*assert verified*/

        mask_sts = uart_reg_ptr->uart_status;

		SCI_TraceLow("Uart_CallbackHisr: line=%d", __LINE__);

        // clear irq status first, protect from clearing successive two irqs at one time
        uart_reg_ptr->uart_status = mask_sts;

        if (s_uart_port[port].callback != NULL)
        {
            if (mask_sts & (uint32) ARM_UART_TX_INT)
            {
                //tx fifo empty interrupt disable, it will be reset when UART_Write is called
                uart_reg_ptr->uart_conf &= ~ (ARM_UART_TX_IE);
                (s_uart_port[port].callback) (EVENT_WRITE_COMPLETE);
            }

            if ( (mask_sts & (uint32) ARM_UART_RX_INT) || (mask_sts & (uint32) ARM_UART_TIMEOUT_INT)) //notify having data to read
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

LOCAL ISR_EXE_T uartzsp_port_irq (uint32 port)
{
    CHIPDRV_DisableIRQINT (hw_config.inter[UART_COM3].int_num);
    IsrWriteBuffer (UART_COM3);
    return CALL_HISR;
}

LOCAL int UART_PHY_BaudSet(uint32 port, HWP_ARM_UART_T *puart, uint32 baudrate)
{    
    uint32 n_set, n_temp;    
    uint32 baud_div,baud_div_temp;    
    uint32 baudrate_real,diff,diff_temp;    
    uint32 remain;    
    uint32 uart_base_clk;

    if (UART_COM0 == port)    /*uart1 only 26MHz base_clk*/
    {
        uart_base_clk = 26000000;
    }
    else
    {
        uart_base_clk = UART_BAUD_BASE_CLK;
    }
    if(baudrate<=2000000)    
    {       
        diff = baudrate;        
        for(n_temp=6;n_temp<=16;n_temp++)       
        {           
            baud_div_temp = uart_base_clk/(n_temp*baudrate);           
            remain = uart_base_clk%(n_temp*baudrate);          
            if(remain > (n_temp*baudrate/2))            
            {               
                baud_div_temp++;            
            }                       
            baudrate_real = uart_base_clk/(n_temp * baud_div_temp);            
            if(baudrate_real > baudrate)            
            {               
                diff_temp = baudrate_real - baudrate;           
            }           
            else            
            {               
                diff_temp = baudrate - baudrate_real;           
            }                       
            if(diff_temp < diff)            
            {               
                diff = diff_temp;               
                n_set = n_temp;             
                baud_div = baud_div_temp - 1;           
            }       
        }  
	if (UART_COM3 == port)
	{
           puart->uart_baud = ((n_set-1) << 10) + baud_div;     
	}
	else
	{
           puart->uart_baud = ((n_set-1) << 16) + baud_div;     
	}
        return 0;   
    }    
    switch(baudrate)    
    {
        case 3250000:     
        {
            if (UART_COM0 == port)    /*uart1 only 26MHz base_clk*/
            {
                n_set = 8;            
                baud_div = 0;            
            }
            else
            {
                n_set = UART_BAUD_3M25_NSET;            
                baud_div = UART_BAUD_3M25_DIV;            
            }
            break;    
        }
        case 4000000:            
            n_set = UART_BAUD_4M_NSET;            
            baud_div = UART_BAUD_4M_DIV;            
            break;  
        case 8000000:            
            n_set = UART_BAUD_8M_NSET;            
            baud_div = UART_BAUD_8M_DIV;            
            break;        
        default:            return -1;  //不支持的串口速率    
    }        
	if (UART_COM3 == port)
	{
	       puart->uart_baud = ((n_set-1) << 10) + baud_div;     
	}
	else
	{
	       puart->uart_baud = ((n_set-1) << 16) + baud_div;     
	}
    return 0;
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
    uint32 type, config = 0;
    uint16 ctl0 = 0;

    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    UART_GLOBAL_Reg (port, TRUE);

#if 0   /*暂时不考虑sleep*/
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
                            EIC_LATCH_RegCallback ( EIC_LATCH_ID(LATCH_UART0_RXD), 
                                                    GPIO_INT_LEVEL_LOW, 
                                                    SCI_TRUE, 
                                                    (EIC_CALLBACK) UART0_RXD_DSlp_Handler
                                                   );

                        }
                        else if (type == DS_WAKEUP_BY_CTS)
                        {
                            EIC_LATCH_RegCallback ( EIC_LATCH_ID(LATCH_UART0_CTS), 
                                                    GPIO_INT_LEVEL_LOW, 
                                                    SCI_TRUE, 
                                                    (EIC_CALLBACK) UART0_CTS_DSlp_Handler
                                                   );
                        
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
                            EIC_LATCH_RegCallback ( EIC_LATCH_ID(LATCH_UART1_RXD), 
                                                    GPIO_INT_LEVEL_LOW, 
                                                    SCI_TRUE, 
                                                    (EIC_CALLBACK) UART1_RXD_DSlp_Handler
                                                   );                        
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
#endif

    p->uart_conf &= ~ (ARM_UART_TOUT_IE | ARM_UART_TX_IE | ARM_UART_RX_IE);

    if (para->parity_enable != PARITY_DISABLE)
    {
        config |= (ARM_UART_CHECK | (para->parity << 1));
    }

    config |= ((ONE_AND_HALF_BIT == para->stop_bits) || (TWO_STOP_BIT == para->stop_bits)) ? (ARM_UART_STOP_BIT) : (0);

    //set water mark
    p->uart_txtrig = ARM_UART_TX_TRIG(para->tx_watermark);
    p->uart_rxtrig = ARM_UART_RX_TRIG(para->rx_watermark);
    
    if (HW_FLOW_CONTROL == para->flow_control) //flow enable
    {
        //config |= (ARM_UART_TOUT_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_HWFC);     // enable Rx/Tx hardware_flow control
        config |= (ARM_UART_RX_TRIG_HWFC | ARM_UART_HWFC);     // enable Rx/Tx hardware_flow control
    }
    else if (SW_FLOW_CONTROL == para->flow_control)
    {   /*uix8910 no sw_flow*/
    }
    else
    {
        config &= ~(ARM_UART_TOUT_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_HWFC);     // enable Rx/Tx hardware_flow control
    }

    UART_SetBaudSpeed (port, para->baud_rate);

    config |= (ARM_UART_TX_IE | ARM_UART_RX_IE | ARM_UART_TOUT_IE);

    p->uart_conf = config;

    if (UART_COM2 == port)
    {
        UART_PHY_Port3RegSave();
    }
    UART_PHY_SuspendResumeLogin(port);
    
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    UART_GLOBAL_Reg (port, FALSE);
    p->uart_conf &= ~ (ARM_UART_TOUT_IE | ARM_UART_TX_IE | ARM_UART_RX_IE);
    /* clear uart deepsleep wakeup control bit */
#if 0
    switch(port)
    {
        case UART_COM0:
        {
            EIC_HAL_EnableInt(EIC_LATCH_ID(LATCH_UART0_RXD), SCI_FALSE);  
            EIC_HAL_EnableInt(EIC_LATCH_ID(LATCH_UART0_CTS), SCI_FALSE);
            break;
        }
        case UART_COM1:
        {
            EIC_HAL_EnableInt(EIC_LATCH_ID(LATCH_UART1_RXD), SCI_FALSE);  
            break;
        }
        default:
        {
            SCI_ASSERT(0);/*assert to do*/
            break;
        }
    }
#endif
#if 0
    CHIP_REG_AND (INT_UINT_CTL, ~ ( (uint32) UART_INT_BIT_MASK << (port * 3)));
#endif /* 0 */
    return UART_SUCCESS;
}

LOCAL BOOLEAN UART_PHY_IsHwFlowControl (HWP_ARM_UART_T *p)
{
    BOOLEAN condition = FALSE;

    if (p->uart_conf & (ARM_UART_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_TOUT_HWFC))
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    i = p->uart_rxfifo_stat & US1_RX_FIFOCNT;

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
        *buffer++ = p->uart_rx;
        length--;
        count++;
    }

    while (length > 0 && (p->uart_rxfifo_stat & US1_RX_FIFOCNT))
    {
        *buffer++ = p->uart_rx;
        length--;
        count++;

        if (!UART_PHY_IsHwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK ( (length > 0 && (p->uart_rxfifo_stat & US1_RX_FIFOCNT)));/*assert verified*/
        }
    }
    p->uart_status |=  (ARM_UART_TIMEOUT_INT | ARM_UART_RX_INT);
 
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    if (0 == length)
    {
        return 0;
    }

    i = hw_config.fifoSize[port-UART_COM0] - ( ( (p->uart_txfifo_stat) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT);

    if (length < i)
    {
        i = length;
    }

    for (; i > 0; i--)
    {
        p->uart_tx = *buffer++;
        length--;
        count++;
    }

    while (length > 0 && ( ( (p->uart_txfifo_stat) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT) < hw_config.fifoSize[port-UART_COM0])
    {
        p->uart_tx = *buffer++;
        length--;
        count++;

        if (!UART_PHY_IsHwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK ( (length > 0 && ( ( (p->uart_txfifo_stat) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT) < hw_config.fifoSize[port-UART_COM0]));/*assert verified*/
        }
    }

    if (irq_en)
    {
        p->uart_conf |= (ARM_UART_TX_IE);
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

#if 0   /*不知道这个什么意义?*/
    status = p->sts0;
    if (status)
    {
        /* the char is invalid, try next  */
        p->iclr |= UART_ICLR_MASK;
    }
#endif
    if (! (p->uart_rxfifo_stat & US1_RX_FIFOCNT))
    {
        return -1;
    }

    nchar = (uint8) ( (p)->uart_rx);

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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    /* tx fifo is empty, so try send char  */
    UART_PHY_WaiTxFifoEmpty (port, hw_config.fifoSize[port-UART_COM0] - 1);

    /* write to tx_fifo */
    p->uart_tx = (uint8) (nchar);
}

/*****************************************************************************/
//  Description:   This function set baudrate parameter directly             *
//  Author:        Tao.Zhou                                                  *
//  Return value:  NONE                                                      *
//  Note:          NONE                                                      *
/*****************************************************************************/
void UART_PHY_SetBaudSpeed (uint32 port, uint32 baudrate)
{
    uint32 apbclk, baudrate_tmp;
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    apbclk = CHIP_GetAPBClk();

    switch(baudrate)
    {
	case BAUD_9600:
	{
		baudrate_tmp = 9600;
		break;
	}
        case BAUD_115200:
	 {
             baudrate_tmp = 115200;
	      break;
	  }
        case BAUD_921600:
	 {
             baudrate_tmp = 921600;
	      break;
	  }
        case BAUD_2000000:
	 {
             baudrate_tmp = 2000000;
	      break;
	  }
        case BAUD_3250000:
	 {
             baudrate_tmp = 3250000;
	      break;
	  }
        case BAUD_8000000:
	 {
          baudrate_tmp = 8000000;
	      break;
	  }
	default:
       {
             baudrate_tmp = 115200;
	      break;
	}		
    }
    UART_PHY_BaudSet(port, p, baudrate_tmp);
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    
    ctl0 = (p->uart_conf & ~ARM_UART_PARITY);
    ctl0 |= (parity << 1);
    p->uart_conf = ctl0;
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->uart_conf & ~ARM_UART_CHECK);
    ctl0 |= (is_enable << 0);
    p->uart_conf = ctl0;
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->uart_conf & ARM_UART_CHECK) >> 0;

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
#if 0
    uint32 ctl0;
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->ctl & ~UC_BYTE_SIZE);
    ctl0 |= (byte_size << UC_BYTE_SIZE_SHIFT);
    p->ctl = ctl0;
#endif
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    ctl0 = (p->uart_conf & ~ARM_UART_STOP_BIT);
    ctl0 |= ((ONE_AND_HALF_BIT == stop_bits) || (TWO_STOP_BIT == stop_bits)) ? (ARM_UART_STOP_BIT) : (0);
    p->uart_conf = ctl0;
}

/*****************************************************************************/
//  Description:   This function set uart tx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetTxWaterMark (uint32 port, uint16 tx_watermark)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    p->uart_txtrig = ARM_UART_TX_TRIG(tx_watermark);
}

/*****************************************************************************/
//  Description:   This function get uart tx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint16 UART_PHY_GetTxWaterMark (uint32 port)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    return (p->uart_txtrig);
}

/*****************************************************************************/
//  Description:   This function set uart rx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetRxWaterMark (uint32 port, uint16 rx_watermark)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    p->uart_rxtrig = ARM_UART_RX_TRIG(rx_watermark);
}

/*****************************************************************************/
//  Description:   This function get uart rx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint16 UART_PHY_GetRxWaterMark (uint32 port)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    return (p->uart_rxtrig);
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    return ARM_UART_TX_FIFO_CNT(p->uart_txfifo_stat); 
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    return ARM_UART_RX_FIFO_CNT(p->uart_rxfifo_stat);
}

/*****************************************************************************/
//  Description:   uart hardware flow control enable/disable directly        *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetFlowCtl (uint32 port, UART_FLOW_CTR_E is_enable)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    if (HW_FLOW_CONTROL == is_enable)
    {
        //p->uart_conf |= (uint32) (ARM_UART_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_TOUT_HWFC);
        p->uart_conf |= (uint32) (ARM_UART_HWFC | ARM_UART_RX_TRIG_HWFC);
    }
    else
    {
        p->uart_conf &= ~ (uint32) (ARM_UART_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_TOUT_HWFC);
    }
}

PUBLIC void UART_PHY_SendBreak (uint32 port, BOOLEAN is_enable)
{
#if 0
    uint32 ctl0;
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

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
#endif
}
/*****************************************************************************/
//  Description:   uart fifo control by DAM enable/disable directly          *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetDmaMode (uint32 port, BOOLEAN is_enable)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    if (is_enable)
    {
        //p->uart_conf &= ~ (uint32) (ARM_UART_RX_IE | ARM_UART_TX_IE | ARM_UART_TOUT_IE);
        p->uart_conf &= ~ (uint32) (ARM_UART_TX_IE);
    }
    else
    {
        p->uart_conf |= (uint32) (ARM_UART_RX_IE | ARM_UART_TX_IE | ARM_UART_TOUT_IE);
    }
}

/*****************************************************************************/
//  Description:   enable/disable self loop mode directly                    *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetSelfLoop (uint32 port, BOOLEAN is_enable)
{
#if 0
    uint32 ctl1;
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

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
#endif
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

    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    while ( (fifo_size) < ( ( (p)->uart_txfifo_stat & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT))
    {
        if (!UART_PHY_IsHwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK ( ( (fifo_size) < ( ( p->uart_txfifo_stat & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT)));/*assert verified*/
        }
#if defined (PLATFORM_UWS6121E)
        else {
                next_tick = SCI_GetTickCount();
                if ((next_tick - cur_tick) > (UART_TRANSFER_TIME_OUT ))
                {
                    break;
                }
        }
#endif
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
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    while ( (fifo_size) < ( ( p->uart_rxfifo_stat & US1_RX_FIFOCNT) >> US1_RX_FIFOCNT_SHIFT))
    {
        if (!UART_PHY_IsHwFlowControl (p))
        {
            nchar = (int) ( p->uart_rx);
            UART_TRANSFER_TIME_OUT_CHECK ( ( (fifo_size) < ( ( p->uart_rxfifo_stat & US1_RX_FIFOCNT) >> US1_RX_FIFOCNT_SHIFT)));/*assert verified*/
        }
#if defined (PLATFORM_UWS6121E)
        else {
            nchar = (int) ( p->uart_rx);
            UART_TRANSFER_TIME_OUT_CHECK ( ( (fifo_size) < ( ( p->uart_rxfifo_stat & US1_RX_FIFOCNT) >> US1_RX_FIFOCNT_SHIFT)));/*assert verified*/
        }
#endif
    }
}

PUBLIC void UART_PHY_TxInEnable (uint32 port, BOOLEAN is_enable)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    if (is_enable)
    {
        p->uart_conf |= ARM_UART_TX_IE;
    }
    else
    {
        p->uart_conf &= ~ ARM_UART_TX_IE;
    }
}

PUBLIC void UART_PHY_RxIntEnable (uint32 port, BOOLEAN is_enable)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    if (is_enable)
    {
        p->uart_conf |= (ARM_UART_RX_IE | ARM_UART_TOUT_IE);
    }
    else
    {
        p->uart_conf &= ~ (ARM_UART_RX_IE | ARM_UART_TOUT_IE);
    }
}

PUBLIC uint8 UART_PHY_CheckTxStatusForDma (uint32 port)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    if ((p->uart_txfifo_stat & ARM_UART_TX_FIFO_CNT(0xFF)) <= (p->uart_txtrig & ARM_UART_TX_TRIG(0xFF)))
    {
        p->uart_status = ARM_UART_TX_INT;
        return 1;
    }
    else
    {
        return 0;
    }
}

PUBLIC uint32 UART_PHY_GetDmaTXChannel (uint32 port)
{
    //avoid compiloring
    uint32 channel =0;
    return (channel);
}

PUBLIC void UART_PHY_Arbitrate (uint32 port_id, UART_CONTROL_TYPE_E control_type)
{
#if 0
	switch (control_type)
	{
		case UART_CONTROL_BY_MCU:
		{
			if (UART_COM1 == port_id)
			{
				* (volatile uint32 *) APB_CLK_CFG1 &= (~CLK_UART1_DSP_CTL_SEL);//CLK source is switched to arm
				REG32 (APB_PERI_CTL0) &= ~U1_CM4_SEL;
				REG32 (APB_PERI_CTL0) &= ~UART1_MUX_SEL;
			}
			else
			{
				SCI_ASSERT (0);/*assert verified*/
			}

			break;
		}
		case UART_CONTROL_BY_DSP:
		{
			if (UART_COM1 == port_id)
			{
				* (volatile uint32 *) APB_CLK_CFG1 |= CLK_UART1_DSP_CTL_SEL;//CLK source is switched to DSP
				REG32 (APB_PERI_CTL0) &= ~U1_CM4_SEL;
				REG32 (APB_PERI_CTL0) |= UART1_MUX_SEL;
			}
			else
			{
				SCI_ASSERT (0);/*assert verified*/
			}

			break;
		}
		case UART_CONTROL_BY_CM4:
		{
			if (UART_COM1 == port_id)
			{
				* (volatile uint32 *) APB_CLK_CFG1 &= (~CLK_UART1_DSP_CTL_SEL);//CLK source is switched to arm
				REG32 (APB_PERI_CTL0) |= U1_CM4_SEL;
			}
			else
			{
				SCI_ASSERT (0);/*assert verified*/
			}

			break;
		}

		default:
		{
			SCI_ASSERT (0);/*assert verified*/
			break;
		}
	}
#endif
}

PUBLIC uint32 GetUART0BaseAddr (void)
{
    return ARM_UART0_BASE;
}

PUBLIC uint32 getRMUartBaseAddr (void)
{
    return ARM_UART2_BASE;
}

PUBLIC ISR_EXE_T UART0_RXD_DSlp_Handler (uint32 param)
{
    param = param; // avoid compiler warning

    if (UARTCOM_DslpFuncSts_Get (UART_COM0))
    {
        //SCI_ASSERT(0);
        DPSLP_UART_EnableApbSleep (UART0_E,DISABLE_APB_SLEEP);
    }
    return ISR_DONE;
}

PUBLIC ISR_EXE_T UART0_CTS_DSlp_Handler (uint32 param)
{
    param = param; // avoid compiler warning
    /* To Do */
    return ISR_DONE;
}

PUBLIC ISR_EXE_T UART1_RXD_DSlp_Handler (uint32 param)
{
    param = param; // avoid compiler warning
    if (UARTCOM_DslpFuncSts_Get (UART_COM1))
    {
        //SCI_ASSERT(0);                  
        DPSLP_UART_EnableApbSleep (UART1_E,DISABLE_APB_SLEEP);
    }
    return ISR_DONE;
}


PUBLIC ISR_EXE_T UartDSlp_Handler (uint32 param)
{
#if 0
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
                SIO_SetUartRxWakeup (UART_COM1, SCI_TRUE);
                DPSLP_UART_EnableApbSleep (UART1_E,DISABLE_APB_SLEEP);
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
#endif
    return ISR_DONE;
}

PUBLIC uint32 UART_PHY_SetTxDelay (uint32 port, uint8 delay_cnt)
{
    uint32 delay_value;
    
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    delay_value = p->uart_delay;
    delay_value &= ~(0xF00);
    delay_value |= ARM_UART_TWO_TX_DELAY(delay_cnt);
    p->uart_delay = delay_value;
    return delay_value;
}

PUBLIC void UART_PHY_SetFifoClr (uint32 port, uint32 clear_flag)
{
    HWP_ARM_UART_T *p  = (HWP_ARM_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    if (clear_flag & PURGE_TX_CLEAR)
    {
        p->uart_conf |= ARM_UART_TXRST;
    }
    if (clear_flag & PURGE_RX_CLEAR)
    {
        p->uart_conf |= ARM_UART_RXRST;
    }
}

static void UART_PHY_WaitTxOver(HWP_ARM_UART_T *uart_ip)
{
    uint32 cur_tick, next_tick;
    
    if (uart_ip->uart_conf & ARM_UART_HWFC)
    {
        cur_tick = next_tick = SCI_GetTickCount();
        while(uart_ip->uart_txfifo_stat & 0xFF)
        {
            //Timeout
            if ( (next_tick - cur_tick) > 10)
            {
                SCI_PASSERT(0, ("uart%x_save tx Timeout!", (uint32)uart_ip));   /*assert to do*/
            }
            next_tick = SCI_GetTickCount();
         }
    }
    else
    {
        while(uart_ip->uart_txfifo_stat & 0xFF);
    }
}

void UART_PHY_Port1RegSave(void)
{    
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =  
    {    
        ARM_UART0_BASE+0x08,        
        ARM_UART0_BASE+0x10,     
        ARM_UART0_BASE+0x14,    
        ARM_UART0_BASE+0x18,         
        ARM_UART0_BASE+0x0C,       
    };     
    HWP_ARM_UART_T *p = (HWP_ARM_UART_T*) (hw_config.baseAddr[0]);
    
    /*Step1: wait tx_fifo empty*/  
    UART_PHY_WaitTxOver(p); 
    
    /*Step2: save uart regs*/   
    Slp_LpsRegsSave((uint32*)&g_uart_regs_data[0], uart_regs_protect_table, UART_SAVE_REG_NUM);
}

void UART_PHY_Port2RegSave(void)
{    
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =  
    {    
        ARM_UART1_BASE+0x08,           
        ARM_UART1_BASE+0x10,        
        ARM_UART1_BASE+0x14,     
        ARM_UART1_BASE+0x18,       
        ARM_UART1_BASE+0x0C,     
    };     
    HWP_ARM_UART_T *p = (HWP_ARM_UART_T*) (hw_config.baseAddr[1]);
    
    /*Step1: wait tx_fifo empty*/  
    UART_PHY_WaitTxOver(p); 
    
    /*Step2: save uart regs*/   
    Slp_LpsRegsSave((uint32*)&g_uart_regs_data[1], uart_regs_protect_table, UART_SAVE_REG_NUM);
}

void UART_PHY_Port3RegSave(void)
{    
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =  
    {    
        ARM_UART2_BASE+0x08,           
        ARM_UART2_BASE+0x10,        
        ARM_UART2_BASE+0x14,     
        ARM_UART2_BASE+0x18,       
        ARM_UART2_BASE+0x0C,     
    };     
    HWP_ARM_UART_T *p = (HWP_ARM_UART_T*) (hw_config.baseAddr[2]);
    
    /*Step1: wait tx_fifo empty*/  
    UART_PHY_WaitTxOver(p); 
    
    /*Step2: save uart regs*/   
    Slp_LpsRegsSave((uint32*)&g_uart_regs_data[2], uart_regs_protect_table, UART_SAVE_REG_NUM);

   /*step3: 8910m BT_test_use:need save sysmail31 reg value*/
   g_sysmail31_value = hwp_mailbox->SYSMAIL31;
   
}

void UART_PHY_Port1RegResume(void)
{   
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =  
    {      
        ARM_UART0_BASE+0x08,          
        ARM_UART0_BASE+0x10,          
        ARM_UART0_BASE+0x14,          
        ARM_UART0_BASE+0x18,            
        ARM_UART0_BASE+0x0C,                 
    };	
    Slp_LpsRegsResume((uint32*)&g_uart_regs_data[0], uart_regs_protect_table, UART_SAVE_REG_NUM);
}

void UART_PHY_Port2RegResume(void)
{   
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =  
    {      
        ARM_UART1_BASE+0x08,          
        ARM_UART1_BASE+0x10,          
        ARM_UART1_BASE+0x14,          
        ARM_UART1_BASE+0x18,            
        ARM_UART1_BASE+0x0C,                 
    };	
    Slp_LpsRegsResume((uint32*)&g_uart_regs_data[1], uart_regs_protect_table, UART_SAVE_REG_NUM);
}

void UART_PHY_Port3RegResume(void)
{   
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =  
    {      
        ARM_UART2_BASE+0x08,          
        ARM_UART2_BASE+0x10,          
        ARM_UART2_BASE+0x14,          
        ARM_UART2_BASE+0x18,            
        ARM_UART2_BASE+0x0C,                 
    };	
    Slp_LpsRegsResume((uint32*)&g_uart_regs_data[2], uart_regs_protect_table, UART_SAVE_REG_NUM);

    hwp_mailbox->SYSMAIL31 = g_sysmail31_value;
}

void UART_PHY_SuspendResumeLogin(uint32 port)
{   
    if (port > 2)
    {
        return ;
    }
    else if (UART_COM0 == port)
    {
        strcpy(g_uart_node[port].module_name, "UART1");    
        g_uart_node[port].reg_suspend = (pfunc_void)UART_PHY_Port1RegSave;    
        g_uart_node[port].reg_resume = (pfunc_void)UART_PHY_Port1RegResume;     
        Slp_LpsSuspendLogin(&g_uart_node[port]); 
    }
    else if (UART_COM1 == port)
    {
        strcpy(g_uart_node[port].module_name, "UART2");    
        g_uart_node[port].reg_suspend = (pfunc_void)UART_PHY_Port2RegSave;    
        g_uart_node[port].reg_resume = (pfunc_void)UART_PHY_Port2RegResume;     
        Slp_LpsSuspendLogin(&g_uart_node[port]); 
    }
    else
    {
        strcpy(g_uart_node[port].module_name, "UART3");    
        g_uart_node[port].reg_suspend = (pfunc_void)UART_PHY_Port3RegSave;    
        g_uart_node[port].reg_resume = (pfunc_void)UART_PHY_Port3RegResume;     
        Slp_LpsSuspendLogin(&g_uart_node[port]); 
    }
        
}   

void UART_PHY_SuspendResumeLogout(uint32 port)
{ 
    struct HAL_IP_SUSPEND_RESUME_T *p_uart_node; 
    
    if (port > 2)
    {
        return ;
    }
    p_uart_node = &g_uart_node[port];   
    Slp_LpsSuspendLogout(p_uart_node);
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
