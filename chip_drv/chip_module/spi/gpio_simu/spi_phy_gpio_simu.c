/******************************************************************************
 ** File Name:      spi_gpio.c                                                 *
 ** Author:         Wuding.Yang                                               *
 ** Date:           2010.2                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the gpio  file for spi protocol           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2010.22        yun.wang     Create                                    *
 ** 2010.11.29       wuding.yang   porting to 6800H                    *
 ******************************************************************************/

#include "sci_types.h"
#include "sci_api.h"
//#include "spi_drv.h"
#include "gpio_prod_api.h"
#include "gpio_drvapi.h"
#include "spi_gpio.h"

LOCAL GPIO_SPI_PORT_T g_gpio_spi_func[MAX_VIR_SPI_NUM]=
{
    {
        {
            {0,0,0},
            {0,0,0},
            {0,0,0},
            {0,0,0}
        },
        FALSE,
        FALSE
    },
    {
        {
            {0,0,0},
            {0,0,0},
            {0,0,0},
            {0,0,0}
        },
        FALSE,
        FALSE
    }
};

/*******************************************************************************************************************
** 函数名称: void SPI_GPIO_Init()                   Name:     void SD_Power()
** 功能描述: 初始化GPIO_SPI接口             Function: turn off the card's power, and turn on
** 输　  入: 无                                 Input:    NULL
** 输 　 出: 无                                 Output:   NULL
********************************************************************************************************************/
PUBLIC int32 SPI_GPIO_Init(uint32 spi_port)
{
    uint8 vir_spi_port_id = VIR_PORT_SPI0;

    if (SPI2_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI0;
    }
    else if (SPI3_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI1;
    }
    else
    {
        SCI_ASSERT(0);/*assert verified*/
    }

    /*配置GPIO的地址和bit位*/
    if (SPI2_0_CS0 == spi_port)
    {
        g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id = GPIO_SPI_id_get (VIR_SPI0_CLK_PIN_ID);
        if ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id) <128)
        {
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr = GPIO_BASE + ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id) >>4) * 0x80;
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset= (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id) & 0xF;
        }

        g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id = GPIO_SPI_id_get (VIR_SPI0_CS_PIN_ID);
        if ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id) <128)
        {
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_addr = GPIO_BASE + ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id) >>4) * 0x80;
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_bitoffset= (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id) & 0xF;
        }

        g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id = GPIO_SPI_id_get (VIR_SPI0_DOUT_PIN_ID);
        if ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id) <128)
        {
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr = GPIO_BASE + ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id) >>4) * 0x80;
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset= (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id) & 0xF;
        }

        g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id = GPIO_SPI_id_get (VIR_SPI0_DIN_PIN_ID);
        if ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id) <128)
        {
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_addr = GPIO_BASE + ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id) >>4) * 0x80;
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_bitoffset= (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id) & 0xF;
        }
    }
    else if (SPI3_0_CS0 == spi_port)
    {
        g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id = GPIO_SPI_id_get (VIR_SPI1_CLK_PIN_ID);
        if ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id) <128)
        {
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr = GPIO_BASE + ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id) >>4) * 0x80;
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset= (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id) & 0xF;
        }

        g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id = GPIO_SPI_id_get (VIR_SPI1_CS_PIN_ID);
        if ( (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id) <128)
        {
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_addr = GPIO_BASE + ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id) >>4) * 0x80;
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_bitoffset= (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id) & 0xF;
        }

        g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id = GPIO_SPI_id_get (VIR_SPI1_DOUT_PIN_ID);
        if ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id) <128)
        {
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr = GPIO_BASE + ( (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id) >>4) * 0x80;
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset= (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id) & 0xF;
        }

        g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id = GPIO_SPI_id_get (VIR_SPI1_DIN_PIN_ID);
        if ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id) <128)
        {
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_addr = GPIO_BASE + ((g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id) >>4) * 0x80;
            g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_bitoffset= (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id) & 0xF;
        }
    }

    return ERR_SPI_NONE;
}

/******************************************************************************
**  Description:    This function config hardware after Init SPI port.
**                  set config reg
**  Author:         wuding.yang
**  Note: 
******************************************************************************/
PUBLIC int32 SPI_GPIO_Cfg (
    uint32 phy_id,
    uint32 mode,
    uint32 tx_bit_length,
    uint32 freq
)
{
    uint8 vir_spi_port_id = VIR_PORT_SPI0;
    BOOLEAN sck_reverse;
    BOOLEAN tx_shift_edge;

    /*set mode*/
    switch (mode)
    {
        case 0:
            sck_reverse = 0;
            tx_shift_edge = 1;
            break;
        case 1:
            sck_reverse = 0;
            tx_shift_edge = 0;
            break;
        case 2:
            sck_reverse = 1;
            tx_shift_edge = 1;
            break;
        case 3:
            sck_reverse = 1;
            tx_shift_edge = 0;
            break;
        default:
            SCI_ASSERT (SCI_FALSE);/*assert to do*/
            break;
    }

    g_gpio_spi_func[vir_spi_port_id].sck_reverse = sck_reverse;
    g_gpio_spi_func[vir_spi_port_id].tx_shift_edge = tx_shift_edge;

    if (GPIO_PROD_NUM_INVALID != g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id)
    {
        //SPI_SCK_GPIO (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset);
        //SPI_SCK_OUT (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset);
        GPIO_SetDataMask(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,TRUE);
        GPIO_SetDirection(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,TRUE);

        //if(TRUE == g_gpio_spi_func[spi_port-REAL_SPI_PORT_NUM].spi_port_ptr->sck_reverse)
        if (g_gpio_spi_func[vir_spi_port_id].sck_reverse)
        {
            //SPI_SCK_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset);   /* SCK  引脚置低   set SCK to zero */
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,1);
        }
        else
        {
            //SPI_SCK_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset);   /* SCK  引脚置低   set SCK to zero */
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,0);
        }
    }

    if (GPIO_PROD_NUM_INVALID != g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id)
    {
        //SPI_DOUT_GPIO (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset);
        //SPI_DOUT_OUT (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset);
        //SPI_DOUT_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset); /* DOUT 引脚置低   set MOSI to zero */
        GPIO_SetDataMask(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id,TRUE);
        GPIO_SetDirection(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id,TRUE);
        GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id,0);
    }

    if (GPIO_PROD_NUM_INVALID != g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id)
    {
        //SPI_CS_GPIO (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_bitoffset);
        //SPI_CS_OUT (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_bitoffset);
        //SPI_CS_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_bitoffset); /* CS 引脚置高  set CS to zero */
        GPIO_SetDataMask(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id,TRUE);
        GPIO_SetDirection(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id,TRUE);
        GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id,1);
    }

    if (GPIO_PROD_NUM_INVALID != g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id)
    {
        //SPI_DIN_GPIO (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_bitoffset);
        //SPI_DIN_IN (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_bitoffset);
        GPIO_SetDataMask(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id,TRUE);
        GPIO_SetDirection(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id,FALSE);
    }

    return ERR_SPI_NONE;
}

/*******************************************************************************************************************
** 函数名称: void SPI_Clk400k()                 Name:     void SPI_Clk400k()
** 功能描述: 设置SPI的时钟小于400kHZ            Function: set the clock of SPI less than 400kHZ
** 输　  入: 无                                 Input:    NULL
** 输 　 出: 无                                 Output:   NULL
********************************************************************************************************************/
void SPI_GPIO_clk_set (void)
{
#if 0
    SPI_SPCCR = 128;                            /* 设置SPI时钟分频值为128  Set the value of dividing frequency to 128 */
#endif
}

/*******************************************************************************************************************
** 函数名称: void __SPI_SendByte()              Name:     void __SPI_SendByte()
** 功能描述: 通过SPI接口发送一个字节            Function: send a byte by SPI interface
** 输　  入: INT8U byte: 发送的字节             Input:    INT8U byte: the byte that will be send
** 输 　 出: 无                                 Output:   NULL
********************************************************************************************************************/
static void __SPI_SendByte (uint32 spi_port,uint8 byte)
{
    uint8 vir_spi_port_id = VIR_PORT_SPI0;
    unsigned char i = 0;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if (SPI2_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI0;
    }
    else if (SPI3_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI1;
    }
    else
    {
        SCI_ASSERT(0);/*assert verified*/
    }
#else
    vir_spi_port_id = spi_port-REAL_SPI_PORT_NUM;
#endif

    if (GPIO_PROD_NUM_INVALID == g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id)
    {
        return;
    }

    if (1 == ((g_gpio_spi_func[vir_spi_port_id].sck_reverse) ^ (g_gpio_spi_func[vir_spi_port_id].tx_shift_edge))) /*两者异或为1则CLK为下降沿采样*/
    {
        //SCI_TRACE_LOW("SIM SPI_SendByte--1 spi_port=%d, byte=%d",spi_port,byte);
        SCI_DISABLE_IRQ

        for (i = 0; i < 8; i++)
        {
            if ( ( (byte >> (7-i)) & 0x01) ==0x01)
            {
                //SPI_DOUT_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset); //set  spi dataout gpio high
                GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id,1);
            }
            else
            {
                //SPI_DOUT_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset); //set  spi dataout gpio low
                GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id,0);
            }

            //SPI_SCK_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset); //set  spi clk gpio high
            //SPI_SCK_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset); //set  spi clk gpio low
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,1);
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,0);
        }

        SCI_RESTORE_IRQ

        if (g_gpio_spi_func[vir_spi_port_id].sck_reverse)
        {
            //SPI_SCK_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset);   /* SCK  引脚置低   set SCK to zero */
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,1);
        }
    }
    else  /*两者相同则CLK为上升沿采样*/
    {
        //SCI_TRACE_LOW("SIM SPI_SendByte--2 spi_port=%d, byte=%d",spi_port,byte);

        SCI_DISABLE_IRQ

        for (i = 0; i < 8; i++)
        {
            if ( ( (byte >> (7-i)) & 0x01) ==0x01)
            {
                //SPI_DOUT_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset); //set  spi dataout gpio high
                GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id,1);
            }
            else
            {
                //SPI_DOUT_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset); //set  spi dataout gpio low
                GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id,1);
            }

            //SPI_SCK_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset); //set  spi clk gpio low
            //SPI_SCK_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset); //set  spi clk gpio high
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,0);
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,1);
        }

        SCI_RESTORE_IRQ

        if (!g_gpio_spi_func[vir_spi_port_id].sck_reverse)
        {
            //SPI_SCK_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset);   /* SCK  引脚置低   set SCK to zero */
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,0);
        }
    }

    //SPI_DOUT_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_bitoffset); //set spi dataout gpio low
    GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id,0);
}

/*******************************************************************************************************************
** 函数名称: INT8U __SPI_RecByte()              Name:     INT8U __SPI_RecByte()
** 功能描述: 从SPI接口接收一个字节              Function: receive a byte from SPI interface
** 输　  入: 无                                 Input:    NULL
** 输 　 出: 收到的字节                         Output:   the byte that be received
********************************************************************************************************************/
static uint8 __SPI_RecByte (uint32 spi_port)
{
    uint8 vir_spi_port_id = VIR_PORT_SPI0;
    unsigned char temp = 0, i = 0;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if (SPI2_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI0;
    }
    else if (SPI3_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI1;
    }
    else
    {
        SCI_ASSERT(0);/*assert verified*/
    }
#else
    vir_spi_port_id = spi_port-REAL_SPI_PORT_NUM;
#endif

    if (GPIO_PROD_NUM_INVALID == g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id)
    {
        return 0;
    }

    if (1 == ( (g_gpio_spi_func[vir_spi_port_id].sck_reverse) ^ (g_gpio_spi_func[vir_spi_port_id].tx_shift_edge))) /*两者异或为1则CLK为下降沿采样*/
    {
        SCI_DISABLE_IRQ

        for (i = 0; i < 8; i++)
        {
            //SPI_SCK_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset); //set  spi clk gpio low
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,0);

            //if (SPI_DIN_READ (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_bitoffset) & 1) //判断GPIODIN是否为高 ?
            if (GPIO_GetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id)) //Is GPIODIN HIGH?
            {
                temp = (temp << 1) + 1;
            }
            else
            {
                temp = (temp << 1) + 0;
            }

            //SPI_SCK_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset); //set  spi clk gpio high
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,1);
        }

        if (!g_gpio_spi_func[vir_spi_port_id].sck_reverse)
        {
            //SPI_SCK_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset);   /* SCK  引脚置低   set SCK to zero */
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,0);
        }

        SCI_RESTORE_IRQ
    }
    else
    {
        SCI_DISABLE_IRQ

        for (i = 0; i < 8; i++)
        {
            //SPI_SCK_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset); //set  spi clk gpio low
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,1);

            //if (SPI_DIN_READ (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_bitoffset) & 1) //判断GPIODIN是否为高 ?
            if (GPIO_GetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id)) //Is GPIODIN HIGH?
            {
                temp = (temp << 1) + 1;
            }
            else
            {
                temp = (temp << 1) + 0;
            }

            //SPI_SCK_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset); //set  spi clk gpio high
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,0);
        }

        if (g_gpio_spi_func[vir_spi_port_id].sck_reverse)
        {
            //SPI_SCK_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_bitoffset);   /* SCK  引脚置低   set SCK to zero */
            GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sck_pin.gpio_id,1);
        }

        SCI_RESTORE_IRQ
    }

    return (temp);
}

/*******************************************************************************************************************
** 函数名称: void SPI_GPIO_CS_SET()             Name:     void SPI_CS_Assert()
** 功能描述: 片选SPI从机                        Function: select the SPI slave
** 输　  入: 无                                 Input:    cs_id
** 输 　 出: 无                                 Output:   NULL
********************************************************************************************************************/
PUBLIC int32 SPI_GPIO_CS_SET (uint32 spi_port,uint32 cs_id)
{
    uint8 vir_spi_port_id = VIR_PORT_SPI0;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if (SPI2_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI0;
    }
    else if (SPI3_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI1;
    }
    else
    {
        SCI_ASSERT(0);/*assert verified*/
    }
#else
    vir_spi_port_id = spi_port-REAL_SPI_PORT_NUM;
#endif

    if (GPIO_PROD_NUM_INVALID == g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id)
    {
        return 0;
    }

    //SCI_TRACE_LOW("SIM SPI_GPIO_CS_SET spi_port=%d, cs_id=%d",spi_port,cs_id);
    if (0xF == cs_id)
    {
        //SPI_CS_SET (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_bitoffset);
        GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id,1);
    }
    else
    {
        //SPI_CS_CLR (g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_addr,g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_bitoffset);
        GPIO_SetValue(g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.scs_pin.gpio_id,0);
    }

    return 1;
}

PUBLIC int32 SPI_GPIO_Write (uint32 spi_port,uint8 *buf, uint32 len)
{
    uint8 vir_spi_port_id = VIR_PORT_SPI0;
    uint32 i;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if (SPI2_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI0;
    }
    else if (SPI3_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI1;
    }
    else
    {
        SCI_ASSERT(0);/*assert verified*/
    }
#else
    vir_spi_port_id = spi_port-REAL_SPI_PORT_NUM;
#endif

    if (GPIO_PROD_NUM_INVALID == g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdout_pin.gpio_id)
    {
        return ERR_SPI_NO_MUTEX;
    }

    for (i = 0; i < len; i++)
    {
        __SPI_SendByte (spi_port,* (buf+i));
    }

    return ERR_SPI_NONE;

}

PUBLIC int32 SPI_GPIO_Read (uint32 spi_port,uint8 *buf, uint32 len)
{
    uint8 vir_spi_port_id = VIR_PORT_SPI0;
    uint32 i;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if (SPI2_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI0;
    }
    else if (SPI3_0_CS0 == spi_port)
    {
        vir_spi_port_id = VIR_PORT_SPI1;
    }
    else
    {
        return ERR_SPI_INT_TIMEOUT;
    }
#else
    vir_spi_port_id = spi_port-REAL_SPI_PORT_NUM;
#endif

    if (GPIO_PROD_NUM_INVALID == g_gpio_spi_func[vir_spi_port_id].gpio_spi_pin.sdin_pin.gpio_id)
    {
        return ERR_SPI_NO_MUTEX;
    }

    for (i = 0; i < len; i++)
    {
        * (buf+i) = __SPI_RecByte (spi_port);
    }

    return ERR_SPI_NONE;

}

PUBLIC SPI_PHY_FUN spi_phy_fun_simu =
{
    SPI_GPIO_Init,
    SPI_GPIO_Cfg,
    SPI_GPIO_Write,
    SPI_GPIO_Read,
    PNULL, /*rw*/
    PNULL,
    PNULL,
    PNULL,
    SPI_GPIO_CS_SET,
    PNULL, /*set mode*/
    PNULL, /*get mode*/
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};
