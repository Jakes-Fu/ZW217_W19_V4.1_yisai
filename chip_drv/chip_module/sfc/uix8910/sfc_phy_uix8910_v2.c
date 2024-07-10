/******************************************************************************
 ** File Name:      sfc_drv.c
 ** Author:         Fei.Zhang
 ** DATE:           01/15/2011
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.
 ** Description:    This file describe operation of SFC.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION
 ** 01/15/2011     Fei.Zhang        Create.
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "os_api.h"
#include "arm_reg.h"
#include "tb_hal.h"
#include "sfc_reg_v7.h"
#include "sfc_reg_uix8910.h"
#include "sfc_drvapi.h"

#define CMD_PAGE_PROGRAM        0x02
#define CMD_SECTOR_ERASE        0x20
#define CMD_GIGA_BE32K    0x52    /* Block Erase 32K */
#define CMD_GIGA_BE64K    0xD8    /* Block Erase 64K */
#define CMD_GIGA_QUAD_PP       0x32    /* Quad Page Program */
#define CMD_WRITE_STATUS        0x01
#define CMD_READ_STATUS2        0x35

#define SPI_FLASH_TX_COMMAND_BIT    0
#define SPI_FLASH_TX_ADDRESS_BIT    8
#define SPI_FLASH_TX_CMD_SPD_ID_BIT     0
#define SPI_FLASH_TX_COMMAND_BIT_SPD_MODE   8
#define SPI_FLASH_TX_CMD_RW_FLAG_BIT    16
#define SPI_FLASH_TX_SPI_MODE_FLAG_BIT      17
#define SPI_FLASH_CMD_ADDR_REG_RESERVED_BIT     18

HWP_SPI_FLASH_T *hwp_spiFlash_cs[SFC_MAX_CS];

#pragma arm section code = "SFC_DRV_CODE" 

extern void *_memcpy(void * dst, const void *src, uint32 count);//defined in MS_REF

PUBLIC void wait_flash_busy(int cs)
{    
	while (hwp_spiFlash_cs[cs]->spi_status & SPI_FLASH_SPI_FLASH_BUSY)    
	{        
	}    
	return ;
}

void push_fifo_data(int cs, const uint8 data_array[], uint32 data_cnt, uint8 quard_flag, uint8 clr_flag)
{   
	uint32 data_tmp_32 = 0;    
	uint32 addr_tmp;  
	uint32 i=0;   
	if(clr_flag)  
	{         
		hwp_spiFlash_cs[cs]->spi_fifo_control = SPI_FLASH_TX_FIFO_CLR;
		while((hwp_spiFlash_cs[cs]->spi_status & SPI_FLASH_TX_FIFO_EMPTY) == 0);
	}   
	while (hwp_spiFlash_cs[cs]->spi_status & SPI_FLASH_TX_FIFO_FULL);     

	for(i = 0; i < data_cnt; i++)   
	{       
		data_tmp_32 = (uint32)data_array[i];       
		if(quard_flag)       
			data_tmp_32 = data_tmp_32 | SPI_FLASH_SPI_SEND_TYPE;
		hwp_spiFlash_cs[cs]->spi_data_fifo = data_tmp_32;
       }    
}

/*****************************************************************************/
//  Description:  read sfc status register
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      sfc status value: SFC_INT_STATUS/SFC_IDLE_STATUS
//  Note:
/*****************************************************************************/
PUBLIC uint32 SFCDRV_GetStatus(int cs)
{
	if (hwp_spiFlash_cs[cs]->spi_status & SPI_FLASH_SPI_FLASH_BUSY)
	{
		return SFC_INT_STATUS;
	}
	else
	{
		return SFC_IDLE_STATUS;
	}
}

/*****************************************************************************/
//  Description:  Soft request to sfc for write command or program
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
void SFCDRV_CmdNoRx(int cs, uint8 cmd, uint32 address, const uint8 *data, size_t size)
{
    REG_SPI_FLASH_SPI_FIFO_CONTROL_T fifo_control;
    REG_SPI_FLASH_SPI_CMD_ADDR_T cmd_addr;
    
    if (cmd == 0)
        return;

    wait_flash_busy(cs);
    
    fifo_control.b.rx_fifo_clr = 0;
    fifo_control.b.tx_fifo_clr = 1;
    hwp_spiFlash_cs[cs]->spi_fifo_control = fifo_control.v;
    
    push_fifo_data(cs, data, size, 1, 0);
    
    cmd_addr.b.spi_tx_cmd = cmd;
    cmd_addr.b.spi_address = address;
    hwp_spiFlash_cs[cs]->spi_cmd_addr = cmd_addr.v;

    wait_flash_busy(cs);
}

uint32 SFCDRV_CmdWithRx(int cs, uint8 cmd, uint32 address, const uint8 *txdata, size_t txsize, size_t rxsize)
{
    REG_SPI_FLASH_SPI_FIFO_CONTROL_T fifo_control;
    REG_SPI_FLASH_SPI_CMD_ADDR_T cmd_addr;
    REG_SPI_FLASH_SPI_BLOCK_SIZE_T block_size;
    REG_SPI_FLASH_SPI_CONFIG_T config;
    uint32 result;
    
    wait_flash_busy(cs);

    fifo_control.b.rx_fifo_clr = 1;
    fifo_control.b.tx_fifo_clr = 1;
    hwp_spiFlash_cs[cs]->spi_fifo_control = fifo_control.v;

    block_size.v = hwp_spiFlash_cs[cs]->spi_block_size;
    block_size.b.spi_rw_blk_size = rxsize;
    hwp_spiFlash_cs[cs]->spi_block_size = block_size.v;

    config.v = hwp_spiFlash_cs[cs]->spi_config;
    config.b.tx_rx_size = rxsize == 1 ? 0 : rxsize == 2 ? 1 : rxsize == 3 ? 3 : 2;
    hwp_spiFlash_cs[cs]->spi_config = config.v;

    push_fifo_data(cs, txdata, txsize, 1, 0);

    cmd_addr.b.spi_tx_cmd = cmd;
    cmd_addr.b.spi_address = 0;
    hwp_spiFlash_cs[cs]->spi_cmd_addr = cmd_addr.v;

    wait_flash_busy(cs);

    result = hwp_spiFlash_cs[cs]->rx_status >> ((4 - rxsize) * 8);
    //uint32_t result = hwp_spiFlash_cs[cs]->rx_status;
    
    block_size.b.spi_rw_blk_size = 1;
    hwp_spiFlash_cs[cs]->spi_block_size = block_size.v;
    return result;
}

void SFCDRV_ReadModeSet(int cs, uint8 mode)
{
    REG_SPI_FLASH_SPI_CONFIG_T config;
    
    config.v = hwp_spiFlash_cs[cs]->spi_config;
    config.b.quad_mode = (mode == 0 ? 0 : 1);
    hwp_spiFlash_cs[cs]->spi_config = config.v;
}

/*****************************************************************************/
//  Description:  set clock config register
//  Author: Fei.zhang
//  Param:
//      value: clock configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_ClkCfg (int cs, uint32 value)
{
    hwp_spiFlash_cs[cs]->spi_config &= ~(SPI_FLASH_SAMPLE_DELAY_MASK | SPI_FLASH_CLK_DIVIDER_MASK);
    hwp_spiFlash_cs[cs]->spi_config |= value;
}

/*****************************************************************************/
//  Description:  set CS config register
//  Author: Fei.zhang
//  Param:
//      value: CS configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_CSCfg (uint32 value)
{
    if(value == 0)
    {
        hwp_spiFlash_cs[value] = ((HWP_SPI_FLASH_T*) REG_ACCESS_ADDRESS(REG_SPI_FLASH_BASE));
    }   
    else if (value == 1)
    {
        hwp_spiFlash_cs[value] = ((HWP_SPI_FLASH_T*) REG_ACCESS_ADDRESS(REG_SPI_FLASH1_BASE));
    }
}

#pragma arm section code

#pragma arm section code = "SFC_INIT_IN_FLASH"

/*****************************************************************************/
//  Description:  set cs0 space size register
//  Author: Yong.Li
//  Param:
//      start_addr: flash cs0 space size
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC uint32 SFCDRV_SetCS0SpaceSize(uint32 flash_size)
{
    switch(flash_size)
    {
        case 0x00100000:
            *(volatile uint32 *)(SFC_CS0_SPACE) = 0x1; 
        break;

        case 0x00200000:
            *(volatile uint32 *)(SFC_CS0_SPACE) = 0x2; 
        break;

        case 0x00400000:
            *(volatile uint32 *)(SFC_CS0_SPACE) = 0x4; 
        break;

        case 0x00800000:
            *(volatile uint32 *)(SFC_CS0_SPACE) = 0x8; 
        break;

        case 0x01000000:
            *(volatile uint32 *)(SFC_CS0_SPACE) = 0x10; 
        break;

        case 0x02000000:
            *(volatile uint32 *)(SFC_CS0_SPACE) = 0x20; 
        break;

        case 0x04000000:
            *(volatile uint32 *)(SFC_CS0_SPACE) = 0x40; 
        break;

        case 0x08000000:
            *(volatile uint32 *)(SFC_CS0_SPACE) = 0x80; 
        break;

        default:
            return SCI_FALSE;
    }

    return SCI_TRUE;
}
#pragma arm section code

