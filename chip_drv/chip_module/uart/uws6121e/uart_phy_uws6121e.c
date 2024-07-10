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
#include "uart_reg_uws6121e.h"
#include "gpio_drvapi.h"
#include "../../eic/eic_cfg.h"
#include "eic_hal.h"
#include "mailbox_reg_uws6121e.h"
#include "sys_ctrl.h"
#include "ap_apb.h"             //w217
#include "ap_clk.h"             //w217
#include "lps_apb.h"            //w217

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

#if 0
typedef struct
{
    uint32    SavedUART_BAUD;
    uint32    SavedUART_RXTRIG;
    uint32    SavedUART_TXTRIG;
    uint32    SavedUART_DELAY;
    uint32    SavedUART_CONF;
}HAL_SAVED_UARTREGS_T;
#endif

/*
        ARM_UART0_BASE+0x00, //ctrl
        ARM_UART0_BASE+0x0C, //irq_mask
        ARM_UART0_BASE+0x10, //irq_cause
        ARM_UART0_BASE+0x14, //triggers
        ARM_UART0_BASE+0x18, //cmd_set
        ARM_UART0_BASE+0x1C, //cmd_clr
        ARM_UART0_BASE+0x20, //auto_ratio
        ARM_UART0_BASE+0x24, //xon
        ARM_UART0_BASE+0x28, //xoff

*/


typedef struct
{
    uint32    SavedUART_ctrl; //改这里
    uint32    SavedUART_irq_mask;
    uint32    SavedUART_irq_cause;
    uint32    SavedUART_triggers;
    uint32    SavedUART_cmd_set;
    uint32    SavedUART_cmd_clr;
    uint32    SavedUART_auto_ratio;
    uint32    SavedUART_xon;
    uint32    SavedUART_xoff;
}HAL_SAVED_UARTREGS_T;

//HAL_SAVED_UARTREGS_T g_uart_regs_data[3];
HAL_SAVED_UARTREGS_T g_uart_regs_data[6];
static struct HAL_IP_SUSPEND_RESUME_T g_uart_node[6];
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
#define ZSP_UART_BASE         (REG_UART4_BASE)

#define UART0_FIFO_SIZE        (16) //TX=16B RX=127B
#define UART1_FIFO_SIZE        (16)
#define UART2_FIFO_SIZE        (16)
#define ZSPUART_FIFO_SIZE      (16)

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

//#define  UART_SAVE_REG_NUM     (5)
#define  UART_SAVE_REG_NUM     (9)
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
    0
};
UART_INFO_S s_uart_port[4];


//----IMPORT w217 CODE ------
#define DIVISOR_MODE_16 (0)
#define DIVISOR_MODE_4 (1)

#define UART1_SRC_FREQ (26000000)
#define UART2_SRC_FREQ (26000000)
#define UART3_SRC_FREQ (26000000)
#define UART4_SRC_FREQ (26000000)
#define UART5_SRC_FREQ (26000000)
#define UART6_SRC_FREQ (26000000)

typedef unsigned short uint16_t;
typedef unsigned long long uint64_t;

typedef struct
{
    uint16_t num;
    uint16_t denom;
} romFracDividerValue_t;

int builtin_clz(unsigned int x)
{
    int n = 0;
    x |=1;
    while(!(x & 0x80000000))
    {
        n++;
        x <<= 1;
    }
    return n;
}

romFracDividerValue_t romCalcFracDivider(unsigned input, unsigned output,
                                         unsigned num_bits,
                                         unsigned denom_bits)
{

    int delta = -1U;
    int rnum = 0, rdenom = 0;
    romFracDividerValue_t divider;
    int inavail_bits;
    unsigned int in_up=0;
    int num=0;
    unsigned int denom;
    unsigned int out_up;
    unsigned int diff;


    divider.denom = 0;
    divider.num = 0;
    inavail_bits = builtin_clz(input);

    if (input == 0 || output == 0 || output > input)
        return divider;

    // Ensure no overflow: input*num, output*denom

    if (inavail_bits < num_bits + 1)
    {
        input >>= (num_bits + 1 - inavail_bits);
        output >>= (num_bits + 1 - inavail_bits);
    }


    for (num = 1; num < (1 << num_bits); num++)
    {
        in_up = (unsigned int)input * num;
        denom = (in_up + output / 2) / output;
        if (denom >= (1 << denom_bits))
            break;

        // Maybe a little over-complex. If the difference is
        // calculated by (input*num/denom), sub-optimized value
        // may be choosen when output is small.
        out_up = (unsigned int)output * denom;
        //diff = OSI_ABS(int, out_up - in_up);
        diff = (out_up - in_up)>0 ? (out_up - in_up):(in_up-out_up);
        diff = ((uint64_t)diff << denom_bits) / denom;
        if (diff < delta)
        {
            delta = diff;
            rnum = num;
            rdenom = denom;
        }
        if (diff == 0)
            break;
    }

    if (delta == -1U)
        return divider;

    divider.num = rnum;
    divider.denom = rdenom;
    return divider;
}

void uart5_SetBuad(unsigned int Buad)
{
    REG_AP_APB_CFG_CLK_UART5_T AP_APB_CFG_CLK_UART5;
    romFracDividerValue_t divider = romCalcFracDivider(UART5_SRC_FREQ / 4, Buad, 13, 17);

    AP_APB_CFG_CLK_UART5.v = hwp_apApb->cfg_clk_uart5;
    AP_APB_CFG_CLK_UART5.b.uart5_div_num = divider.num;
    AP_APB_CFG_CLK_UART5.b.uart5_div_denom = divider.denom;
    hwp_apApb->cfg_clk_uart5 = AP_APB_CFG_CLK_UART5.v;
}

void uart6_SetBuad(unsigned int Buad)
{
    REG_AP_APB_CFG_CLK_UART6_T AP_APB_CFG_CLK_UART6;
    romFracDividerValue_t divider = romCalcFracDivider(UART6_SRC_FREQ / 4, Buad, 13, 17);

    AP_APB_CFG_CLK_UART6.v = hwp_apApb->cfg_clk_uart6;
    AP_APB_CFG_CLK_UART6.b.uart6_div_num = divider.num;
    AP_APB_CFG_CLK_UART6.b.uart6_div_denom = divider.denom;
    hwp_apApb->cfg_clk_uart6 = AP_APB_CFG_CLK_UART6.v;
}

void uart4_SetBuad(unsigned int Buad)
{
    REG_AP_APB_CFG_CLK_UART4_T AP_APB_CFG_CLK_UART4;
    romFracDividerValue_t divider = romCalcFracDivider(UART4_SRC_FREQ / 4, Buad, 13, 17);

    AP_APB_CFG_CLK_UART4.v = hwp_apApb->cfg_clk_uart4;
    AP_APB_CFG_CLK_UART4.b.uart4_div_num = divider.num;
    AP_APB_CFG_CLK_UART4.b.uart4_div_denom = divider.denom;
    hwp_apApb->cfg_clk_uart4 = AP_APB_CFG_CLK_UART4.v;
}

void uart3_SetBuad(unsigned int Buad)
{
    REG_SYS_CTRL_CFG_CLK_UART3_T SYS_CTRL_CFG_CLK_UART3;
    romFracDividerValue_t divider = romCalcFracDivider(UART3_SRC_FREQ / 4, Buad, 10, 14);

    SYS_CTRL_CFG_CLK_UART3.v = hwp_sysCtrl->cfg_clk_uart3;
    SYS_CTRL_CFG_CLK_UART3.b.cfg_clk_uart3_num = divider.num;
    SYS_CTRL_CFG_CLK_UART3.b.cfg_clk_uart3_demod = divider.denom;
    hwp_sysCtrl->cfg_clk_uart3 = SYS_CTRL_CFG_CLK_UART3.v;
}

void uart2_SetBuad(unsigned int Buad)
{
    REG_SYS_CTRL_CFG_CLK_UART2_T SYS_CTRL_CFG_CLK_UART2;
    romFracDividerValue_t divider = romCalcFracDivider(UART2_SRC_FREQ / 4, Buad, 10, 14);

    SYS_CTRL_CFG_CLK_UART2.v = hwp_sysCtrl->cfg_clk_uart2;
    SYS_CTRL_CFG_CLK_UART2.b.cfg_clk_uart2_num = divider.num;
    SYS_CTRL_CFG_CLK_UART2.b.cfg_clk_uart2_demod = divider.denom;
    hwp_sysCtrl->cfg_clk_uart2 = SYS_CTRL_CFG_CLK_UART2.v;
}

void uart1_SetBuad(unsigned int Buad)
{
    REG_LPS_APB_CFG_CLK_UART1_T LPS_APB_CFG_CLK_UART1;
    romFracDividerValue_t divider = romCalcFracDivider(UART1_SRC_FREQ / 4, Buad, 10, 14);

    LPS_APB_CFG_CLK_UART1.v = hwp_lpsApb->cfg_clk_uart1;
    LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_num = divider.num;
    LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_demod = divider.denom;
    LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_update = 1;
    hwp_lpsApb->cfg_clk_uart1 = LPS_APB_CFG_CLK_UART1.v;
}


void uart2_io_init(void)
{
    // UART2: set iomux
    hwp_iomux_6121e->pad_uart_2_rxd &= ~(IOMUX_PAD_UART_2_RXD_DRV(0xf));
    hwp_iomux_6121e->pad_uart_2_rxd |= IOMUX_PAD_UART_2_RXD_DRV(2);
    hwp_iomux_6121e->pad_uart_2_rxd &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_2_rxd |= IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_2_txd &= ~(IOMUX_PAD_UART_2_TXD_DRV(0xf));
    hwp_iomux_6121e->pad_uart_2_txd |= IOMUX_PAD_UART_2_TXD_DRV(2);
}

void uart3_io_init(void)
{
    // UART3 is not default IO
    hwp_iomux_6121e->gpio_17 = 0x4; // UART_3_RX
    hwp_iomux_6121e->gpio_16 = 0x4; // UART_3_TX
    hwp_iomux_6121e->pad_gpio_17 &= ~(IOMUX_PAD_GPIO_17_DRV(0xf));
    hwp_iomux_6121e->pad_gpio_17 |= IOMUX_PAD_GPIO_17_DRV(2);
    hwp_iomux_6121e->pad_gpio_17 &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_gpio_17 |= IOMUX_WPU;
    hwp_iomux_6121e->pad_gpio_16 &= ~(IOMUX_PAD_GPIO_16_DRV(0xf));
    hwp_iomux_6121e->pad_gpio_16 |= IOMUX_PAD_GPIO_16_DRV(2);
}

void uart5_io_init(void)
{
    // UART5 is not default IO
    hwp_iomux_6121e->sdmmc1_data_0 = 0x14; // UART_5_RX
    hwp_iomux_6121e->sdmmc1_data_1 = 0x14; // UART_5_TX
    hwp_iomux_6121e->pad_sdmmc1_data_0 &= ~(IOMUX_PAD_SDMMC1_DATA_0_DRV(0xf));
    hwp_iomux_6121e->pad_sdmmc1_data_0 |= IOMUX_PAD_SDMMC1_DATA_0_DRV(2);
    hwp_iomux_6121e->pad_sdmmc1_data_0 &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_sdmmc1_data_0 |= IOMUX_WPU;
    hwp_iomux_6121e->pad_sdmmc1_data_1 &= ~(IOMUX_PAD_SDMMC1_DATA_1_DRV(0xf));
    hwp_iomux_6121e->pad_sdmmc1_data_1 |= IOMUX_PAD_SDMMC1_DATA_1_DRV(2);
}

void uart6_io_init(void)
{
    // UART6 is not default IO
    hwp_iomux_6121e->sdmmc1_clk = 0x18; // UART_6_RX
    hwp_iomux_6121e->sdmmc1_cmd = 0x18; // UART_6_TX
    hwp_iomux_6121e->pad_sdmmc1_clk &= ~(IOMUX_PAD_SDMMC1_CLK_DRV(0xf));
    hwp_iomux_6121e->pad_sdmmc1_clk |= IOMUX_PAD_SDMMC1_CLK_DRV(2);
    hwp_iomux_6121e->pad_sdmmc1_clk &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_sdmmc1_clk |= IOMUX_WPU;
    hwp_iomux_6121e->pad_sdmmc1_cmd &= ~(IOMUX_PAD_SDMMC1_CMD_DRV(0xf));
    hwp_iomux_6121e->pad_sdmmc1_cmd |= IOMUX_PAD_SDMMC1_CMD_DRV(2);
}


void uart4_io_init(void)
{
    // UART4 is not default IO
    hwp_iomux_6121e->uart_2_cts = 0x10; // UART_4_RX
    hwp_iomux_6121e->uart_2_rts = 0x10; // UART_4_TX
    hwp_iomux_6121e->pad_uart_2_cts &= ~(IOMUX_PAD_UART_2_CTS_DRV(0x3));
    hwp_iomux_6121e->pad_uart_2_cts |= IOMUX_PAD_UART_2_CTS_DRV(2);
    hwp_iomux_6121e->pad_uart_2_cts &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_2_cts |= IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_2_rts &= ~(IOMUX_PAD_UART_2_RTS_DRV(0x3));
    hwp_iomux_6121e->pad_uart_2_rts |= IOMUX_PAD_UART_2_RTS_DRV(2);
}

void uart1_io_init(void)
{
    // UART1: set iomux
    hwp_iomux_6121e->pad_uart_1_rxd &= ~(IOMUX_PAD_UART_1_RXD_DRV(0xf));
    hwp_iomux_6121e->pad_uart_1_rxd |= IOMUX_PAD_UART_1_RXD_DRV(7); //rx_drving = 2
    hwp_iomux_6121e->pad_uart_1_rxd &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_1_rxd |= IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_1_txd &= ~(IOMUX_PAD_UART_1_TXD_DRV(0xf));
    hwp_iomux_6121e->pad_uart_1_txd |= IOMUX_PAD_UART_1_TXD_DRV(7); //rx_drving = 2
}


//---------------------------

#if 0//for test
void UART_PHY_SetBaudSpeed (uint32 port, uint32 baudrate);
void UART_PHY_WaitRxFifoEmpty(uint32 port, uint32 fifo_size, int nchar);

void UART_KERNEL_Test(uint32 port, uint32 baudrate) //test
{
    uint8 Read_Buff[10];
    char CH = 0;
    UART_InitUartLowLevel (port, baudrate);
    UART_PHY_SetBaudSpeed (port, baudrate);

    UART_PHY_WriteData (port,"\nUART_PHY_WriteData...\n",24, FALSE);
    UART_PHY_WriteData (port,"\nUART_PHY_WriteData...\n",24, FALSE);
    UART_PHY_WriteData (port,"\nUART_PHY_WriteData...\n",24, FALSE);
    UART_PHY_WriteData (port,"\nUART_PHY_WriteData...\n",24, FALSE);
    UART_PHY_WriteData (port,"\nUART_PHY_WriteData...\n",24, FALSE);
    UART_PHY_WriteData (port,"\nUART_PHY_WriteData...\n",24, FALSE);
    UART_PHY_WriteData (port,"\nUART_PHY_WriteData...\n",24, FALSE);

    //UART_PHY_ReadData (port, Read_Buff, 10);
/*
    UART_PHY_WaitRxFifoEmpty(port,0,'a');

    CH = (char)UART_PHY_GetChar(port)&0x00ff;
    UART_PHY_WriteData (port,"\nRecvData:",10, FALSE);
    UART_PHY_WriteData (port,&CH,1, FALSE);

    CH = (char)UART_PHY_GetChar(port)&0x00ff;
    UART_PHY_WriteData (port,"\nRecvData:",10, FALSE);
    UART_PHY_WriteData (port,&CH,1, FALSE);
*/

    UART_PHY_WriteData (port,"\nUART_PHY_Close\n",17, FALSE);
    UART_PHY_Close (port);
}
#endif


PUBLIC void UART_Clk_Enable(uint32 port)  //for CP uart clk enable
{
    REG_AP_APB_CLK_AP_EN0_T clken456;
    REG_LPS_APB_LPS_CLK_EN_T clken1;
    REG_SYS_CTRL_AON_CLOCK_EN0_T clken23;

    clken23.v = hwp_sysCtrl->aon_clock_en0;
    clken456.v = hwp_apApb->clk_ap_en0;
    clken1.v = hwp_lpsApb->lps_clk_en;

    switch(port)
    {
        case 0:
            clken1.b.uart1_en = 1;
        break;
        case 1:
            clken23.b.uart2_en = 1;
        break;
        case 2:
            clken23.b.uart3_en = 1;
        break;
        case 3:
            clken456.b.clk_en_uart4_bf_div_fr = 1;
        break;
        case 4:
            clken456.b.clk_en_uart5_bf_div_fr = 1;
        break;
        case 5:
            clken456.b.clk_en_uart6_bf_div_fr = 1;
        break;
        default:
            //assert
        break;
    }
    hwp_apApb->clk_ap_en0 = clken456.v;
    hwp_lpsApb->lps_clk_en= clken1.v;
    hwp_sysCtrl->aon_clock_en0 = clken23.v;
}


PUBLIC void UART_InitUartLowLevel (uint32 port, uint32 baudrate)
{
    uint32 uart_base = 0;
    HWP_UART_T *p;
    REG_UART_CTRL_T ctrl;
    //REG_UART_CMD_SET_T cmd_set;
    REG_AP_APB_CLK_AP_EN0_T clken456;
    REG_LPS_APB_LPS_CLK_EN_T clken1;
    REG_SYS_CTRL_AON_CLOCK_EN0_T clken23;

    uart_base = hw_config.baseAddr[port-UART_COM0];
    p  = (HWP_UART_T*)uart_base;
    UART_GLOBAL_Reg (port, TRUE);

    clken23.v = hwp_sysCtrl->aon_clock_en0;
    clken456.v = hwp_apApb->clk_ap_en0;
    clken1.v = hwp_lpsApb->lps_clk_en;

    switch(port)
    {
        case 0:
            clken1.b.uart1_en = 1;
            //uart1_io_init();
            uart1_SetBuad(baudrate);
        break;
        case 1:
            clken23.b.uart2_en = 1;
            //uart2_io_init();
            uart2_SetBuad(baudrate);
        break;
        case 2:
            clken23.b.uart3_en = 1;
            //uart3_io_init();
            uart3_SetBuad(baudrate);
        break;
        case 3:
            clken456.b.clk_en_uart4_bf_div_fr = 1;
            //uart4_io_init();
            uart4_SetBuad(baudrate);
        break;
        case 4:
            clken456.b.clk_en_uart5_bf_div_fr = 1;
            //uart5_io_init();
            uart5_SetBuad(baudrate);
        break;
        case 5:
            clken456.b.clk_en_uart6_bf_div_fr = 1;
            //uart6_io_init();
            uart6_SetBuad(baudrate);
        break;
        default:
            //assert
        break;
    }

    hwp_apApb->clk_ap_en0 = clken456.v;
    hwp_lpsApb->lps_clk_en= clken1.v;
    hwp_sysCtrl->aon_clock_en0 = clken23.v;

    //uart4_SetBuad(115200);

    p->ctrl = 0XF0080003;
    p->irq_mask = 0X00000000;
    p->irq_cause = 0X00040000;
    p->triggers = 0X00000000;
    p->cmd_set = 0X00000000;
    p->cmd_clr = 0X00000000;
    p->auto_ratio = 0X00000000;
    p->xon = 0X00000011;
    p->xoff= 0X00000013;
}


INPUT_BUFFER_INIT (int32,3)
PUBLIC void  Uart_CallbackHisr (uint32 data, void *pData)
{
    //uint32 mask_sts;
    uint32 raw_sts;
    //uint32 tmp_char = 0;
    int32 port = 0;
    HWP_UART_T *uart_reg_ptr = NULL;
    //REG_UART_IRQ_MASK_T mask_sts;
    REG_UART_IRQ_CAUSE_T cause;


    while (!threadReadBuffer (&port))
    {
        SCI_ASSERT (port < 4);/*assert verified*/

        uart_reg_ptr = (HWP_UART_T*) hw_config.baseAddr[port-UART_COM0];
        SCI_ASSERT (uart_reg_ptr != NULL);/*assert verified*/

        //mask_sts = uart_reg_ptr->uart_status; //读取中断状态
        //mask_sts = (uart_reg_ptr->irq_mask); //读取中断状态(读取后中断是否自动清零(待确认))

        cause.v = (uart_reg_ptr->irq_cause); //获取中断原因
        (uart_reg_ptr->irq_cause) = cause.v; //清除中断

        SCI_TraceLow("Uart_CallbackHisr: port=%d cause:0x%x", port,(uint32)cause.v);

        // clear irq status first, protect from clearing successive two irqs at one time
        //uart_reg_ptr->uart_status = mask_sts;

        //(uart_reg_ptr->irq_mask) = mask_sts;

        if (s_uart_port[port].callback != NULL)
        {
            if ((cause.b.tx_data_needed)||
                (cause.b.tx_dma_done)||
                (cause.b.uart_dma_tx_adone)) //如果是发送完成中断
            {
                //tx fifo empty interrupt disable, it will be reset when UART_Write is called
                //uart_reg_ptr->uart_conf &= ~ (ARM_UART_TX_IE);
                (s_uart_port[port].callback) (EVENT_WRITE_COMPLETE);
            }


            if ((cause.b.rx_data_available)||
                (cause.b.rx_dma_done)||
                (cause.b.uart_dma_rx_adone)||
                (cause.b.rx_timeout))
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

LOCAL int UART_PHY_BaudSet(uint32 port, HWP_UART_T *puart, uint32 baudrate)
{
    switch(port)
    {
        case 0:uart1_SetBuad(baudrate); break;
        case 1:uart2_SetBuad(baudrate); break;
        case 2:uart3_SetBuad(baudrate); break;
        case 3:uart4_SetBuad(baudrate); break;
        case 4:uart5_SetBuad(baudrate); break;
        case 5:uart6_SetBuad(baudrate); break;
        default:
            //assert
        break;
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
    //uint32 type, config = 0;
    //uint16 ctl0 = 0;

    REG_UART_TRIGGERS_T uart_triggers;
    REG_UART_CTRL_T uart_ctrl;
    REG_UART_IRQ_MASK_T uart_irq_mask;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    UART_GLOBAL_Reg (port, TRUE);

    //p->uart_conf &= ~ (ARM_UART_TOUT_IE | ARM_UART_TX_IE | ARM_UART_RX_IE); //关闭tout/TX/RX中断
    uart_irq_mask.v = p->irq_mask;
    uart_irq_mask.b.tx_dma_done = 0;
    uart_irq_mask.b.uart_dma_tx_adone = 0;
    uart_irq_mask.b.rx_timeout = 0;
    uart_irq_mask.b.uart_dma_rx_adone = 0;
    uart_irq_mask.b.rx_dma_timeout = 0;
    uart_irq_mask.b.rx_data_available = 0;
    p->irq_mask = uart_irq_mask.v;


    uart_ctrl.v = p->ctrl;  //以下都是配置ctrl寄存器
    if (para->parity_enable != PARITY_DISABLE) //使能parity
    {
        //config |= (ARM_UART_CHECK | (para->parity << 1));
        uart_ctrl.v = p->ctrl;
        uart_ctrl.b.parity_enable = 1;
        uart_ctrl.b.parity_select = para->parity;
        //p->ctrl = uart_ctrl.v;
    }

    //config |= ((ONE_AND_HALF_BIT == para->stop_bits) || (TWO_STOP_BIT == para->stop_bits)) ? (ARM_UART_STOP_BIT) : (0);
    uart_ctrl.b.tx_stop_bits = ((ONE_AND_HALF_BIT == para->stop_bits) || (TWO_STOP_BIT == para->stop_bits)) ? (1) : (0);

    //set water mark
    //p->uart_txtrig = ARM_UART_TX_TRIG(para->tx_watermark);
    //p->uart_rxtrig = ARM_UART_RX_TRIG(para->rx_watermark);

    uart_triggers.v = p->triggers;
    uart_triggers.b.rx_trigger = (para->rx_watermark)&0X7F;
    uart_triggers.b.tx_trigger = (para->tx_watermark)*0X0F;
    p->triggers = uart_triggers.v;


    if (HW_FLOW_CONTROL == para->flow_control) //flow enable
    {
        //config |= (ARM_UART_TOUT_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_HWFC);     // enable Rx/Tx hardware_flow control
        //config |= (ARM_UART_RX_TRIG_HWFC | ARM_UART_HWFC);     // enable Rx/Tx hardware_flow control
        /*w217 no hw_flow*/
    }
    else if (SW_FLOW_CONTROL == para->flow_control)
    {
        uart_ctrl.b.soft_flow_ctrl_enable = 1;
        uart_ctrl.b.auto_flow_control = 1;
    }
    else
    {
        //config &= ~(ARM_UART_TOUT_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_HWFC);     // enable Rx/Tx hardware_flow control
        uart_ctrl.b.soft_flow_ctrl_enable = 0;
        uart_ctrl.b.auto_flow_control = 0;
    }

    UART_SetBaudSpeed (port, para->baud_rate);

    //config |= (ARM_UART_TX_IE | ARM_UART_RX_IE | ARM_UART_TOUT_IE); //中断使能

    uart_irq_mask.v = p->irq_mask;
    uart_irq_mask.b.tx_dma_done = 1;
    uart_irq_mask.b.uart_dma_tx_adone = 1;
    uart_irq_mask.b.rx_timeout = 1;
    uart_irq_mask.b.uart_dma_rx_adone = 1;
    uart_irq_mask.b.rx_dma_timeout = 1;
    uart_irq_mask.b.rx_data_available = 1;
    p->irq_mask = uart_irq_mask.v;


    //p->uart_conf = config;
    p->ctrl = uart_ctrl.v;

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
    REG_UART_IRQ_MASK_T uart_irq_mask;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    UART_GLOBAL_Reg (port, TRUE);

    //p->uart_conf &= ~ (ARM_UART_TOUT_IE | ARM_UART_TX_IE | ARM_UART_RX_IE); //关闭tout/TX/RX中断
    uart_irq_mask.v = p->irq_mask;
    uart_irq_mask.b.tx_dma_done = 0;
    uart_irq_mask.b.uart_dma_tx_adone = 0;
    uart_irq_mask.b.rx_timeout = 0;
    uart_irq_mask.b.uart_dma_rx_adone = 0;
    uart_irq_mask.b.rx_dma_timeout = 0;
    p->irq_mask = uart_irq_mask.v;

    return UART_SUCCESS;
}

LOCAL BOOLEAN UART_PHY_IsHwFlowControl (HWP_UART_T *p)
{
    BOOLEAN condition = FALSE;
    //w217 is not support hw flow ctrl
    return condition;
}

LOCAL BOOLEAN UART_PHY_IsSwFlowControl (HWP_UART_T *p)
{
    BOOLEAN condition = FALSE;
    REG_UART_CTRL_T uart_ctrl;

    uart_ctrl.v =  p->ctrl;
    if(uart_ctrl.b.soft_flow_ctrl_enable)
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
/*
        uint32_t rx_fifo_level : 8;        // [7:0], read only
        uint32_t tx_fifo_space : 5;        // [12:8], read only
        uint32_t at_match_flag : 1;        // [13], read only
        uint32_t tx_active : 1;            // [14], read only
        uint32_t rx_active : 1;            // [15], read only
        uint32_t rx_overflow_err : 1;      // [16], read only
        uint32_t tx_overflow_err : 1;      // [17], read only
        uint32_t rx_parity_err : 1;        // [18], read only
        uint32_t rx_framing_err : 1;       // [19], read only
        uint32_t rx_break_int : 1;         // [20], read only
        uint32_t character_miscompare : 1; // [21], read only
        uint32_t auto_baud_locked : 1;     // [22], read only
        uint32_t __23_23 : 1;              // [23]
        uint32_t dcts : 1;                 // [24], read only
        uint32_t cts : 1;                  // [25], read only
        uint32_t auto_ratio_flag : 1;      // [26], read only
        uint32_t mask_tx_enable_flag : 1;  // [27], read only
        uint32_t dtr : 1;                  // [28], read only
        uint32_t __30_29 : 2;              // [30:29]
        uint32_t clk_enabled : 1;          // [31], read only
    } b;
} REG_UART_STATUS_T;

*/


uint32 UART_PHY_ReadData (uint32 port, uint8 *buffer, uint32 length)
{
    uint32 count = 0;
    uint32   cur_tick     = SCI_GetTickCount();
    uint32   next_tick   = SCI_GetTickCount();
    uint32 i;
    REG_UART_IRQ_MASK_T uart_irq_mask;
    REG_UART_STATUS_T uart_status;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    //i = p->uart_rxfifo_stat & US1_RX_FIFOCNT;
    uart_status.v = p->status;
    i = uart_status.b.rx_fifo_level;

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
        //*buffer++ = p->uart_rx;
        *buffer++ = ((p->rxtx_buffer) & 0xff);
        length--;
        count++;
    }

    //while (length > 0 && (p->uart_rxfifo_stat & US1_RX_FIFOCNT))
    while (length > 0)
    {
        do
        {
            uart_status.v = p->status;
            if (!UART_PHY_IsSwFlowControl (p)) //sw flowctrl
            {
                UART_TRANSFER_TIME_OUT_CHECK ( (length > 0 && (uart_status.b.rx_fifo_level)));/*assert verified*/
            }
        }while(uart_status.b.rx_fifo_level == 0);

        *buffer++ = ((p->rxtx_buffer) & 0xff);
        length--;
        count++;
#if 0
        if (!UART_PHY_IsHwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK ( (length > 0 && (p->uart_rxfifo_stat & US1_RX_FIFOCNT)));/*assert verified*/
        }
#endif
    }
    //p->uart_status |=  (ARM_UART_TIMEOUT_INT | ARM_UART_RX_INT); //清除中断标志，待完善
    uart_irq_mask.v = p->irq_mask;
    uart_irq_mask.b.rx_timeout = 0;
    uart_irq_mask.b.uart_dma_rx_adone = 0;
    uart_irq_mask.b.rx_dma_timeout = 0;
    if(port==COM0)
    uart_irq_mask.b.rx_data_available = 1;
    p->irq_mask = uart_irq_mask.v;

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
    REG_UART_IRQ_MASK_T uart_irq_mask;
    REG_UART_STATUS_T uart_status;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    if (0 == length)
    {
        return 0;
    }

    //先判断TX FIFO剩余空间 uart_status.b.tx_fifo_space
    uart_status.v = p->status;
    //i = hw_config.fifoSize[port-UART_COM0] - ( ( (p->uart_txfifo_stat) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT);
    //i = hw_config.fifoSize[port-UART_COM0] - (uart_status.b.tx_fifo_space);
    i = (uart_status.b.tx_fifo_space);

    if (length < i)
    {
        i = length;
    }

    for (; i > 0; i--)
    {
        //p->uart_tx = *buffer++;
        p->rxtx_buffer = *buffer++;
        length--;
        count++;
    }

    //while (length > 0 && ( ( (p->uart_txfifo_stat) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT) < hw_config.fifoSize[port-UART_COM0])
    while (length > 0)
    {

        do{
            uart_status.v = p->status;
            if (!UART_PHY_IsSwFlowControl (p)) //sw flowctrl
            {
                UART_TRANSFER_TIME_OUT_CHECK ( (length > 0 && (uart_status.b.tx_fifo_space == 0)));/*assert verified*/
            }
        }while(uart_status.b.tx_fifo_space == 0);

        p->rxtx_buffer = *buffer++;
        length--;
        count++;
#if 0
        if (!UART_PHY_IsSwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK ( (length > 0 && ( ( (p->uart_txfifo_stat) & US1_TX_FIFOCNT) >> US1_TX_FIFOCNT_SHIFT) < hw_config.fifoSize[port-UART_COM0]));/*assert verified*/
        }
#endif
    }

    if (irq_en)
    {
        //p->uart_conf |= (ARM_UART_TX_IE); //使能发送完成中断
        uart_irq_mask.v = p->irq_mask;
        uart_irq_mask.b.tx_data_needed = 1;
        if(port == COM3)
        uart_irq_mask.b.tx_data_needed = 0;
        p->irq_mask = uart_irq_mask.v;
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
    REG_UART_STATUS_T uart_status;
    uint32 old_tick;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_status.v = p->status;
    old_tick =  SCI_GetTickCount();
    do
    {
        uart_status.v = p->status;
        if((SCI_GetTickCount() - old_tick) >= 2000)    //2s timeout
            return -1;
    }while(uart_status.b.rx_fifo_level == 0);

    return ((p->rxtx_buffer) & 0xff);
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
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    /* tx fifo is empty, so try send char  */
    UART_PHY_WaiTxFifoEmpty (port, hw_config.fifoSize[port-UART_COM0] - 1);

    /* write to tx_fifo */
    //p->uart_tx = (uint8) (nchar);
    p->rxtx_buffer = nchar;
}

/*****************************************************************************/
//  Description:   This function set baudrate parameter directly             *
//  Author:        Tao.Zhou                                                  *
//  Return value:  NONE                                                      *
//  Note:          NONE                                                      *
/*****************************************************************************/
void UART_PHY_SetBaudSpeed (uint32 port, uint32 baudrate)
{
    uint32 baudrate_tmp;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    //apbclk = CHIP_GetAPBClk();
    switch(baudrate)
    {
        case BAUD_115200:
        baudrate_tmp = 115200;
        break;

        case BAUD_921600:
        baudrate_tmp = 921600;
        break;

        default:
        baudrate_tmp = baudrate;
        break;
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
    //uint32 ctl0;
    REG_UART_CTRL_T uart_ctrl;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_ctrl.v = p->ctrl;
    uart_ctrl.b.parity_select = parity;
    p->ctrl = uart_ctrl.v;

#if 0
    ctl0 = (p->uart_conf & ~ARM_UART_PARITY); //
    ctl0 |= (parity << 1);
    p->uart_conf = ctl0;
#endif
}

/*****************************************************************************/
//  Description:   uart parity enable/disable directly                       *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
void UART_PHY_SetParityEnable (uint32 port, UART_PARITY_SET_E is_enable)
{
    //uint32 ctl0;
    REG_UART_CTRL_T uart_ctrl;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_ctrl.v = p->ctrl;
    uart_ctrl.b.parity_enable = is_enable > 0 ? 1:0;
    p->ctrl = uart_ctrl.v;

#if 0
    ctl0 = (p->uart_conf & ~ARM_UART_CHECK);
    ctl0 |= (is_enable << 0);
    p->uart_conf = ctl0;
#endif
}

/*****************************************************************************/
//  Description:   get uart parity enable/disable directly                   *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
UART_PARITY_SET_E UART_PHY_GetParityEnable (uint32 port)
{
    //uint32 ctl0;
    REG_UART_CTRL_T uart_ctrl;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_ctrl.v = p->ctrl;
    if(uart_ctrl.b.parity_enable)
    {
        return PARITY_ENABLE;
    }
    else
    {
        return PARITY_DISABLE;
    }

#if 0
    ctl0 = (p->uart_conf & ARM_UART_CHECK) >> 0;

    if (ctl0)
    {
        return PARITY_ENABLE;
    }
    else
    {
        return PARITY_DISABLE;
    }
#endif
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
    //uint32 ctl0;
    REG_UART_CTRL_T uart_ctrl;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_ctrl.v = p->ctrl;
    uart_ctrl.b.tx_stop_bits = ((ONE_AND_HALF_BIT == stop_bits) || (TWO_STOP_BIT == stop_bits)) ? (1) : (0);
    p->ctrl = uart_ctrl.v;

#if 0
    ctl0 = (p->uart_conf & ~ARM_UART_STOP_BIT);
    ctl0 |= ((ONE_AND_HALF_BIT == stop_bits) || (TWO_STOP_BIT == stop_bits)) ? (ARM_UART_STOP_BIT) : (0);
    p->uart_conf = ctl0;
#endif
}

/*****************************************************************************/
//  Description:   This function set uart tx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetTxWaterMark (uint32 port, uint16 tx_watermark)
{
    REG_UART_TRIGGERS_T uart_triggers;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    //p->uart_txtrig = ARM_UART_TX_TRIG(tx_watermark);
    uart_triggers.v = p->triggers;
    //uart_triggers.b.rx_trigger = (para->rx_watermark)&0X7F;
    uart_triggers.b.tx_trigger = (tx_watermark)*0X0F;
    p->triggers = uart_triggers.v;

}

/*****************************************************************************/
//  Description:   This function get uart tx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint16 UART_PHY_GetTxWaterMark (uint32 port)
{
    REG_UART_TRIGGERS_T uart_triggers;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    uart_triggers.v = p->triggers;

    return (uart_triggers.b.tx_trigger)&0X0F;
}

/*****************************************************************************/
//  Description:   This function set uart rx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetRxWaterMark (uint32 port, uint16 rx_watermark)
{
    REG_UART_TRIGGERS_T uart_triggers;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    //p->uart_rxtrig = ARM_UART_RX_TRIG(rx_watermark);
    uart_triggers.v = p->triggers;
    uart_triggers.b.rx_trigger = (rx_watermark)&0X7F;
    //uart_triggers.b.tx_trigger = (para->tx_watermark)*0X0F;
    p->triggers = uart_triggers.v;
}

/*****************************************************************************/
//  Description:   This function get uart rx water mark directly             *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC uint16 UART_PHY_GetRxWaterMark (uint32 port)
{
    REG_UART_TRIGGERS_T uart_triggers;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);
    uart_triggers.v = p->triggers;

    return (uart_triggers.b.rx_trigger)&0X7F;
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
    REG_UART_STATUS_T uart_status;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_status.v = p->status;
    //返回TX FIFO中已有数据个数
    //return ARM_UART_TX_FIFO_CNT(p->uart_txfifo_stat);
    return (16 - ((uart_status.b.tx_fifo_space)&0x1F));
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
    REG_UART_STATUS_T uart_status;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_status.v = p->status;
    //return ARM_UART_RX_FIFO_CNT(p->uart_rxfifo_stat);
    return (uart_status.b.rx_fifo_level);
}

/*****************************************************************************/
//  Description:   uart hardware flow control enable/disable directly        *
//  Global resource dependence : NONE                                        *
//  Author:        Tao.Zhou                                                  *
//  Note:          NONE                                                      *
/*****************************************************************************/
PUBLIC void UART_PHY_SetFlowCtl (uint32 port, UART_FLOW_CTR_E is_enable)
{
    REG_UART_CTRL_T uart_ctrl;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_ctrl.v = p->ctrl;

    if (HW_FLOW_CONTROL == is_enable)
    {
        //p->uart_conf |= (uint32) (ARM_UART_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_TOUT_HWFC);
        //p->uart_conf |= (uint32) (ARM_UART_HWFC | ARM_UART_RX_TRIG_HWFC);
    }
    else if (SW_FLOW_CONTROL == is_enable)
    {
        //p->uart_conf &= ~ (uint32) (ARM_UART_HWFC | ARM_UART_RX_TRIG_HWFC | ARM_UART_TOUT_HWFC);

        uart_ctrl.b.soft_flow_ctrl_enable = 1;
        uart_ctrl.b.auto_flow_control = 1;
    }
    else //no usr flowctrl
    {
        uart_ctrl.b.soft_flow_ctrl_enable = 0;
        uart_ctrl.b.auto_flow_control = 0;
    }

    p->ctrl = uart_ctrl.v;
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
    REG_UART_CTRL_T uart_ctrl;
    REG_UART_IRQ_MASK_T uart_irq_mask;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_ctrl.v = p->ctrl;
    uart_irq_mask.v = p->irq_mask;

    //原始代码仅仅配置了DMA中断
#if 0
    if (is_enable)
    {
        //p->uart_conf &= ~ (uint32) (ARM_UART_RX_IE | ARM_UART_TX_IE | ARM_UART_TOUT_IE);
        p->uart_conf &= ~ (uint32) (ARM_UART_TX_IE);
    }
    else
    {
        p->uart_conf |= (uint32) (ARM_UART_RX_IE | ARM_UART_TX_IE | ARM_UART_TOUT_IE);
    }
#endif
    //使能DMA模式、并只保留DMA相关中断
    if (is_enable)
    {
        uart_ctrl.b.dma_mode = 1;
        uart_irq_mask.b.rx_data_available = 0;
        uart_irq_mask.b.tx_data_needed = 0;
        uart_irq_mask.b.rx_timeout = 0;

        uart_irq_mask.b.uart_dma_rx_adone = 1;
        uart_irq_mask.b.uart_dma_tx_adone = 1;
        uart_irq_mask.b.tx_dma_done = 1;
        uart_irq_mask.b.rx_dma_done = 1;
        uart_irq_mask.b.rx_dma_timeout = 1;
    }
    else
    {
        uart_ctrl.b.dma_mode = 0;
        uart_ctrl.b.dma_mode = 1;
        uart_irq_mask.b.rx_data_available = 1;
        uart_irq_mask.b.tx_data_needed = 1;
        uart_irq_mask.b.rx_timeout = 1;

        uart_irq_mask.b.uart_dma_rx_adone = 0;
        uart_irq_mask.b.uart_dma_tx_adone = 0;
        uart_irq_mask.b.tx_dma_done = 0;
        uart_irq_mask.b.rx_dma_done = 0;
        uart_irq_mask.b.rx_dma_timeout = 0;
    }
    p->ctrl = uart_ctrl.v;
    p->irq_mask = uart_irq_mask.v;
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

    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);


#if 0
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
#endif
    while ( (fifo_size) < (UART_PHY_GetTxFifoCnt (port)))
    {
        if (!UART_PHY_IsSwFlowControl (p))
        {
            UART_TRANSFER_TIME_OUT_CHECK((fifo_size) < (UART_PHY_GetTxFifoCnt (port)));/*assert verified*/
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
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);


#if 0
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
#endif
    while ( (fifo_size) < (UART_PHY_GetRxFifoCnt (port)))
    {
        if (!UART_PHY_IsSwFlowControl (p))
        {
            nchar = (int)((p->rxtx_buffer) & 0xff);
            UART_TRANSFER_TIME_OUT_CHECK ( (fifo_size) < (UART_PHY_GetRxFifoCnt (port)));/*assert verified*/
        }
#if defined (PLATFORM_UWS6121E)
        else {
             nchar = (int)((p->rxtx_buffer) & 0xff);
            UART_TRANSFER_TIME_OUT_CHECK ( (fifo_size) < (UART_PHY_GetRxFifoCnt (port)));/*assert verified*/
        }
#endif
    }
}

PUBLIC void UART_PHY_TxInEnable (uint32 port, BOOLEAN is_enable)
{
    REG_UART_IRQ_MASK_T uart_irq_mask;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_irq_mask.v = p->irq_mask;
    if (is_enable)
    {
        //p->uart_conf |= ARM_UART_TX_IE;
        uart_irq_mask.b.tx_data_needed = 1;
    }
    else
    {
        //p->uart_conf &= ~ ARM_UART_TX_IE;
        uart_irq_mask.b.tx_data_needed = 0;
    }
    p->irq_mask = uart_irq_mask.v;
}

PUBLIC void UART_PHY_RxIntEnable (uint32 port, BOOLEAN is_enable)
{
    REG_UART_IRQ_MASK_T uart_irq_mask;

    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    uart_irq_mask.v = p->irq_mask;
    if (is_enable)
    {
        //p->uart_conf |= (ARM_UART_RX_IE | ARM_UART_TOUT_IE);
        uart_irq_mask.b.rx_data_available = 1;
        uart_irq_mask.b.rx_timeout = 1;
    }
    else
    {
        //p->uart_conf &= ~ (ARM_UART_RX_IE | ARM_UART_TOUT_IE);
        uart_irq_mask.b.rx_data_available = 0;
        uart_irq_mask.b.rx_timeout = 0;
    }
    p->irq_mask = uart_irq_mask.v;
}

PUBLIC uint8 UART_PHY_CheckTxStatusForDma (uint32 port)
{
    REG_UART_IRQ_CAUSE_T cause;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

#if 0
    if ((p->uart_txfifo_stat & ARM_UART_TX_FIFO_CNT(0xFF)) <= (p->uart_txtrig & ARM_UART_TX_TRIG(0xFF)))
    {
        p->uart_status = ARM_UART_TX_INT;
        return 1;
    }
    else
    {
        return 0;
    }
#endif

    if (UART_PHY_GetTxFifoCnt (port) <= UART_PHY_GetTxWaterMark(port))
    {
        //p->uart_status = ARM_UART_TX_INT; //clr tx中断
        cause.v = 0;
        cause.b.tx_data_needed = 1; //clr tx_data_needed
        p->irq_cause = cause.v;
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
    return ARM_UART0_BASE;
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

PUBLIC uint32 UART_PHY_SetTxDelay (uint32 port, uint8 delay_cnt) //这里
{
    uint32 delay_value=0;

#if 0
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    delay_value = p->uart_delay;
    delay_value &= ~(0xF00);
    delay_value |= ARM_UART_TWO_TX_DELAY(delay_cnt);
    p->uart_delay = delay_value;
#endif
    //w217 has no reg of uart_delay.
    return delay_value;
}

PUBLIC void UART_PHY_SetFifoClr (uint32 port, uint32 clear_flag)
{
    REG_UART_CMD_SET_T cmd_set;
    HWP_UART_T *p  = (HWP_UART_T*) (hw_config.baseAddr[port-UART_COM0]);

    cmd_set.v = p->cmd_set;

    if (clear_flag & PURGE_TX_CLEAR)
    {
        //p->uart_conf |= ARM_UART_TXRST;
        cmd_set.b.tx_fifo_reset = 1;
    }
    if (clear_flag & PURGE_RX_CLEAR)
    {
        //p->uart_conf |= ARM_UART_RXRST;
        cmd_set.b.rx_fifo_reset = 1;
    }
    p->cmd_set = cmd_set.v;
}

static void UART_PHY_WaitTxOver(HWP_UART_T *uart_ip)
{
    uint32 cur_tick, next_tick;

#if 0
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
#endif

    REG_UART_CTRL_T uart_ctrl;
    REG_UART_STATUS_T uart_status;


    uart_ctrl.v = uart_ip->ctrl;
    uart_status.v = uart_ip->status;

    if((uart_ctrl.b.soft_flow_ctrl_enable)) //如果开了流控
    {
        cur_tick = next_tick = SCI_GetTickCount();
        while((16 - ((uart_status.b.tx_fifo_space)&0x1F)))
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
        while((16 - ((uart_status.b.tx_fifo_space)&0x1F))) //w217 tx fifo cap(16byte)
        {
        }
    }
}


/*

    hwp_uart->ctrl = 0XF0080003;
    //hwp_uart->status = 0X82001000;
    //hwp_uart->rxtx_buffer = 0X0000006F;
    hwp_uart->irq_mask = 0X00000000;
    hwp_uart->irq_cause = 0X00040000;
    hwp_uart->triggers = 0X00000000;
    hwp_uart->cmd_set = 0X00000000;
    hwp_uart->cmd_clr = 0X00000000;
    hwp_uart->auto_ratio = 0X00000000;
    hwp_uart->xon = 0X00000011;
    hwp_uart->xoff= 0X00000013;

    uint32_t ctrl;        // 0x00000000
    uint32_t status;      // 0x00000004
    uint32_t rxtx_buffer; // 0x00000008
    uint32_t irq_mask;    // 0x0000000c
    uint32_t irq_cause;   // 0x00000010
    uint32_t triggers;    // 0x00000014
    uint32_t cmd_set;     // 0x00000018
    uint32_t cmd_clr;     // 0x0000001c
    uint32_t auto_ratio;  // 0x00000020
    uint32_t xon;         // 0x00000024
    uint32_t xoff;        // 0x00000028

*/
void UART_PHY_Port1RegSave(void)
{
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =
    {
        ARM_UART0_BASE+0x00, //ctrl
        ARM_UART0_BASE+0x0C, //irq_mask
        ARM_UART0_BASE+0x10, //irq_cause
        ARM_UART0_BASE+0x14, //triggers
        ARM_UART0_BASE+0x18, //cmd_set
        ARM_UART0_BASE+0x1C, //cmd_clr
        ARM_UART0_BASE+0x20, //auto_ratio
        ARM_UART0_BASE+0x24, //xon
        ARM_UART0_BASE+0x28, //xoff
    };
    HWP_UART_T *p = (HWP_UART_T*) (hw_config.baseAddr[0]);

    /*Step1: wait tx_fifo empty*/
    UART_PHY_WaitTxOver(p);

    /*Step2: save uart regs*/
    Slp_LpsRegsSave((uint32*)&g_uart_regs_data[0], uart_regs_protect_table, UART_SAVE_REG_NUM);
}

void UART_PHY_Port2RegSave(void)
{
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =
    {
        ARM_UART1_BASE+0x00, //ctrl
        ARM_UART1_BASE+0x0C, //irq_mask
        ARM_UART1_BASE+0x10, //irq_cause
        ARM_UART1_BASE+0x14, //triggers
        ARM_UART1_BASE+0x18, //cmd_set
        ARM_UART1_BASE+0x1C, //cmd_clr
        ARM_UART1_BASE+0x20, //auto_ratio
        ARM_UART1_BASE+0x24, //xon
        ARM_UART1_BASE+0x28, //xoff
    };
    HWP_UART_T *p = (HWP_UART_T*) (hw_config.baseAddr[1]);

    /*Step1: wait tx_fifo empty*/
    UART_PHY_WaitTxOver(p);

    /*Step2: save uart regs*/
    Slp_LpsRegsSave((uint32*)&g_uart_regs_data[1], uart_regs_protect_table, UART_SAVE_REG_NUM);
}

void UART_PHY_Port3RegSave(void)
{
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =
    {
        ARM_UART2_BASE+0x00, //ctrl
        ARM_UART2_BASE+0x0C, //irq_mask
        ARM_UART2_BASE+0x10, //irq_cause
        ARM_UART2_BASE+0x14, //triggers
        ARM_UART2_BASE+0x18, //cmd_set
        ARM_UART2_BASE+0x1C, //cmd_clr
        ARM_UART2_BASE+0x20, //auto_ratio
        ARM_UART2_BASE+0x24, //xon
        ARM_UART2_BASE+0x28, //xoff
    };
    HWP_UART_T *p = (HWP_UART_T*) (hw_config.baseAddr[2]);

    /*Step1: wait tx_fifo empty*/
    UART_PHY_WaitTxOver(p);

    /*Step2: save uart regs*/
    Slp_LpsRegsSave((uint32*)&g_uart_regs_data[2], uart_regs_protect_table, UART_SAVE_REG_NUM);

   /*step3: 8910m BT_test_use:need save sysmail31 reg value*/
   //g_sysmail31_value = hwp_mailbox->SYSMAIL31;

}

void UART_PHY_Port1RegResume(void)
{
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =
    {
        ARM_UART0_BASE+0x00, //ctrl
        ARM_UART0_BASE+0x0C, //irq_mask
        ARM_UART0_BASE+0x10, //irq_cause
        ARM_UART0_BASE+0x14, //triggers
        ARM_UART0_BASE+0x18, //cmd_set
        ARM_UART0_BASE+0x1C, //cmd_clr
        ARM_UART0_BASE+0x20, //auto_ratio
        ARM_UART0_BASE+0x24, //xon
        ARM_UART0_BASE+0x28, //xoff
    };
    Slp_LpsRegsResume((uint32*)&g_uart_regs_data[0], uart_regs_protect_table, UART_SAVE_REG_NUM);
}

void UART_PHY_Port2RegResume(void)
{
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =
    {
        ARM_UART1_BASE+0x00, //ctrl
        ARM_UART1_BASE+0x0C, //irq_mask
        ARM_UART1_BASE+0x10, //irq_cause
        ARM_UART1_BASE+0x14, //triggers
        ARM_UART1_BASE+0x18, //cmd_set
        ARM_UART1_BASE+0x1C, //cmd_clr
        ARM_UART1_BASE+0x20, //auto_ratio
        ARM_UART1_BASE+0x24, //xon
        ARM_UART1_BASE+0x28, //xoff
    };
    Slp_LpsRegsResume((uint32*)&g_uart_regs_data[1], uart_regs_protect_table, UART_SAVE_REG_NUM);
}

void UART_PHY_Port3RegResume(void)
{
    uint32 uart_regs_protect_table[UART_SAVE_REG_NUM] =
    {
        ARM_UART2_BASE+0x00, //ctrl
        ARM_UART2_BASE+0x0C, //irq_mask
        ARM_UART2_BASE+0x10, //irq_cause
        ARM_UART2_BASE+0x14, //triggers
        ARM_UART2_BASE+0x18, //cmd_set
        ARM_UART2_BASE+0x1C, //cmd_clr
        ARM_UART2_BASE+0x20, //auto_ratio
        ARM_UART2_BASE+0x24, //xon
        ARM_UART2_BASE+0x28, //xoff
    };
    Slp_LpsRegsResume((uint32*)&g_uart_regs_data[2], uart_regs_protect_table, UART_SAVE_REG_NUM);

    //hwp_mailbox->SYSMAIL31 = g_sysmail31_value;
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
