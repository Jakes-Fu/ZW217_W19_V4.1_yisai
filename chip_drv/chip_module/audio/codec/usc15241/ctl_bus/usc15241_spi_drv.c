/******************************************************************************
 ** File Name:      usc15241_spi_drv.c                                                 *
 ** Author:         ken.kuang                                                 *
 ** DATE:           09/08/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    usc15241 control spi bus driver                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/08/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/


#include "sci_types.h"
#include "os_api.h"

#include "chip_plf_export.h"

#include "usc15241_spi_drv.h"


#define SPI_CLK_DELAY    2
#define SPI_READ_REG    0x3F


LOCAL __inline void spi_delay(uint32 us)
{
    volatile uint32 i = 0;
    for(i=0;i<us;i++)
    {}
}

#if 1 //GPIO

#define ARM_GPIO_BASE0				0x8A000000
#define ARM_GPIO_BASE1				0x8A000080
#define ARM_GPIO_BASE2				0x8A000100
#define ARM_GPIO_BASE3				0x8A000180
#define ARM_GPIO_BASE4				0x8A000200
#define ARM_GPIO_BASE5				0x8A000280
#define ARM_GPIO_BASE6				0x8A000300
#define ARM_GPIO_BASE7				0x8A000380
#define ARM_GPIO_BASE8				0x8A000400
#define ARM_GPIO_BASE9				0x8A000480
#define ARM_GPIO_BASE10				0x8A000500



typedef struct arm_gpio_tag
{
	volatile uint32 gpio_data;		    //GPIO Data
	volatile uint32 gpio_dmsk;		    //GPIO Data Mask
	volatile uint32 gpio_dir;			//GPIO Direction
	volatile uint32 gpio_is;			//GPIO Interrupt Sense Select
	volatile uint32 gpio_ibe;			//GPIO Interrupt on Both Edge
	volatile uint32 gpio_iev;			//GPIO Interrupt Event
	volatile uint32 gpio_ie;			//GPIO Interrupt Mask
	volatile uint32 gpio_ris;			//GPIO Raw Interrupt Status
	volatile uint32 gpio_mis;			//GPIO Masked Interrupt Status
	volatile uint32 gpio_ic;			//GPIO Interrupt Clear
}ARM_GPIO_S;


#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
LOCAL void GPIO_En()
{
    volatile uint32 i = 0;
    //Access Enable
    *(volatile uint32 *)(APB_EB0_SET) = (GPIO_EB_SET);
    
    //Soft Reset
    *(volatile uint32 *)(APB_RST0_SET) = GPIO_SOFT_RST_SET;
    for(i = 0; i < 10; i++){}
    *(volatile uint32 *)(APB_RST0_CLR) = GPIO_SOFT_RST_CLR;
}
#endif

LOCAL void GPIO_SET_DIR(uint8 port,uint8 dir)
{
    volatile ARM_GPIO_S *gpio_ctrl = (volatile ARM_GPIO_S *)(ARM_GPIO_BASE0);
    if( dir )
    {
        gpio_ctrl->gpio_dir  |= 1<<((uint32)(port-ARM_GPIO_BASE_CNT));        // OUTPUT MODE.
    }
    else
    {
        gpio_ctrl->gpio_dir  &= ~((uint32)(1<<((uint32)(port-ARM_GPIO_BASE_CNT))));     // INPUT MODE.
    }
    gpio_ctrl->gpio_dmsk |= (1<<(uint32)((port-ARM_GPIO_BASE_CNT)));     // OUTPUT MODE.
}


LOCAL void GPIO_OUT_COMBO(uint32 sclk,uint32 sen,uint32 dat)
{
    volatile ARM_GPIO_S *gpio_ctrl = (volatile ARM_GPIO_S *)(ARM_GPIO_BASE0);
    uint32 read_dat ;
    uint32 reg_set = 0;
    if( sclk )
    {
        reg_set |= SPI_USC15241_SCK_GPIO_MASK;
    }
    if( sen )
    {
        reg_set |= SPI_USC15241_SEN_GPIO_MASK;
    }
    if( dat )
    {
        reg_set |= SPI_USC15241_DAT_GPIO_MASK;
    }
    SCI_DisableIRQ();
    read_dat = gpio_ctrl->gpio_data;
    read_dat &= ~(SPI_USC15241_SCK_GPIO_MASK | SPI_USC15241_SEN_GPIO_MASK | SPI_USC15241_DAT_GPIO_MASK);
    read_dat |= reg_set;
    gpio_ctrl->gpio_data = read_dat;
    SCI_RestoreIRQ();
}


LOCAL void GPIO_OUT(uint8 port,uint8 data)
{
    volatile ARM_GPIO_S *gpio_ctrl = (volatile ARM_GPIO_S *)(ARM_GPIO_BASE0);
    if( data )
    {
        gpio_ctrl->gpio_data |= (1<<(uint32)((port-ARM_GPIO_BASE_CNT)));  // set 1.
    }
    else
    {
        gpio_ctrl->gpio_data &= ~((uint32)(1<<(uint32)((port-ARM_GPIO_BASE_CNT)))); // set 0.
    }
}

LOCAL uint8 GPIO_IN(uint8 port)
{
    volatile ARM_GPIO_S *gpio_ctrl = (volatile ARM_GPIO_S *)(ARM_GPIO_BASE0);
    uint32 data ;
    data = gpio_ctrl->gpio_data;
    if( data & (1<<(port-ARM_GPIO_BASE_CNT)) )
    {
        return 1;
    }
    return 0;
}


#endif


PUBLIC void __usc15241_spi_init(void)
{
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
	GPIO_En();
#endif

    GPIO_SET_DIR(SPI_USC15241_SCK_GPIO_PIN,1);
    GPIO_SET_DIR(SPI_USC15241_SEN_GPIO_PIN,1);
    GPIO_SET_DIR(SPI_USC15241_DAT_GPIO_PIN,1);

    GPIO_OUT(SPI_USC15241_SCK_GPIO_PIN,0);
    GPIO_OUT(SPI_USC15241_SEN_GPIO_PIN,1);
    GPIO_OUT(SPI_USC15241_DAT_GPIO_PIN,1);
}


PUBLIC void __spi_reg_w(uint32 reg,uint32 val)
{
    int32 i;
    uint32 send;
    send = (val<<15) | (reg & 0x7fff);
    
    GPIO_SET_DIR(SPI_USC15241_SCK_GPIO_PIN,1);
    GPIO_SET_DIR(SPI_USC15241_SEN_GPIO_PIN,1);
    GPIO_SET_DIR(SPI_USC15241_DAT_GPIO_PIN,1);
    GPIO_OUT_COMBO(0,1,1);
    
    GPIO_OUT_COMBO(1,1,1);
    for(i=30;i>=0;i--)
    {
        GPIO_OUT_COMBO(0,0,send & (1<<i));
        spi_delay(SPI_CLK_DELAY);
        GPIO_OUT(SPI_USC15241_SCK_GPIO_PIN,1);
    }
    spi_delay(1);
    GPIO_OUT_COMBO(0,1,1);
}

PUBLIC uint32 __spi_reg_r(uint32 reg)
{
    int32 i;
    uint32 recv;

    __spi_reg_w(SPI_READ_REG,reg);
    
    GPIO_SET_DIR(SPI_USC15241_DAT_GPIO_PIN,0);
    recv= 0;

    GPIO_OUT(SPI_USC15241_SCK_GPIO_PIN,1);
    spi_delay(SPI_CLK_DELAY);
    GPIO_OUT(SPI_USC15241_SCK_GPIO_PIN,0);
    
    for(i=15;i>=0;i--)
    {
        spi_delay(SPI_CLK_DELAY);
        GPIO_OUT(SPI_USC15241_SCK_GPIO_PIN,1);
        recv <<= 1;
        spi_delay(SPI_CLK_DELAY);
        recv |= GPIO_IN(SPI_USC15241_DAT_GPIO_PIN);
        GPIO_OUT(SPI_USC15241_SCK_GPIO_PIN,0);
    }

    return recv;
}


