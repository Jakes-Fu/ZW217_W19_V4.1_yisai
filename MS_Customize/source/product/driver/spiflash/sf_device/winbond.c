/*
 * Copyright 2008, Network Appliance Inc.
 * Author: Jason McMullan <mcmullan <at> netapp.com>
 * Licensed under the GPL-2 or later.
 */
#include "common.h"
//#include <malloc.h>
#include "os_api.h"
#include "spi_flash.h"
#include "spi_flash_internal.h"


#if defined (CONFIG_SPI_FLASH_WINBOND) || !defined (CONFIG_SPI_FLASH)


/* M25Pxx-specific commands */
#define CMD_W25_WREN        0x06    /* Write Enable */
#define CMD_W25_WRDI        0x04    /* Write Disable */
#define CMD_W25_RDSR        0x05    /* Read Status Register */
#define CMD_W25_WRSR        0x01    /* Write Status Register */
#define CMD_W25_RDSR1        0x05
#define CMD_W25_WRSR1        0x01
#define CMD_W25_RDSR2        0x35
#define CMD_W25_WRSR2        0x31

#define CMD_W25_RDSR3       0x15
#define CMD_W25_WRSR3        0x11


#define CMD_W25_READ        0x03    /* Read Data Bytes */
#define CMD_W25_FAST_READ   0x0b    /* Read Data Bytes at Higher Speed */
#define CMD_W25_PP      0x02    /* Page Program */
#define CMD_W25_SE      0x20    /* Sector (4K) Erase */
#define CMD_W25_BE32  0x52   /* Block (32K) Erase */
#define CMD_W25_BE64      0xd8    /* Block (64K) Erase */
#define CMD_W25_CE      0xc7    /* Chip Erase */
#define CMD_W25_DP      0xb9    /* Deep Power-down */
#define CMD_W25_RES     0xab    /* Release from DP, and Read Signature */

#define CMD_W25_SETBURST        0xC0
#define CMD_W25_QPI_4READ       0xeb    /* qpi read */

/* 256Mb */
#define CMD_4BYTEADDREN     0xB7
#define CMD_4BYTEADDRDI     0xE9

#define WINBOND_ID_W25X16       0x3015
#define WINBOND_ID_W25X32       0x3016
#define WINBOND_ID_W25X64       0x3017

/* Status1/Status2 register bit map */
#define WINBOND_SR_WIP      (1 << 0)    /* Write-in-Progress */

#define WINBOND_SR_QE       (1 << 9)    /* Quad-enable */

/* Read dummy bytes for QPI_READ */
#define DUMMYCLK_2_FREQ_50MHZ           (0x0)
#define DUMMYCLK_4_FREQ_80MHZ           (0x10)
#define DUMMYCLK_6_FREQ_104MHZ          (0x20)
#define DUMMYCLK_8_FREQ_104MHZ          (0x30)

#define WINBOND_TIMEOUT_CMD    100


struct winbond_spi_flash_params {
    u16    id;
    /* Log2 of page size in power-of-two mode */
    u8     l2_page_size;
    u16    pages_per_sector;
    u16    sectors_per_block;
    u16    nr_blocks;
    const char  *name;

    u8     support_qpi;
    u8     read_freq_max;  /*Unit:Mhz*/
    u8     dummy_clocks;    
};

/* spi_flash needs to be first so upper layers can free() it */
struct winbond_spi_flash {
    struct spi_flash flash;
    const struct winbond_spi_flash_params *params;
};

static __inline struct winbond_spi_flash *
to_winbond_spi_flash(struct spi_flash *flash)
{
    return container_of(flash, struct winbond_spi_flash, flash); /*lint !e413*/
}

#define WINBOND_REG1_LOCK_MSK 0xFC  //SRP0,SEC,TB,BP2,BP1,BP0
#define WINBOND_REG2_LOCK_MSK 0x41   //CMP,SRP1
#define WINBOND_REG3_LOCK_MSK 0x4  //WPS


static const struct spi_flash_lock_desc winbond32mbit_lock_table[] = {
/*S7:spr0, S6:sec, S5:tb, S4:bp2, S3:bp1, S2:bp0, S1:wel, S0:busy,*/
/* CMP=0 */
/* Serial Flash 32Mbit(4MB) Lock range. Start from Top */
    {0xFF, 0x04, SF32M_L0X003F0000_0X010000}, /* 64KB */
    {0xFF, 0x08, SF32M_L0X003E0000_0X020000}, /* 128KB */
    {0xFF, 0x0C, SF32M_L0X003C0000_0X040000}, /* 256KB */
    {0xFF, 0x10, SF32M_L0X00380000_0X080000}, /* 512KB */
    {0xFF, 0x14, SF32M_L0X00300000_0X100000}, /* 1MB */
    {0xFF, 0x18, SF32M_L0X00200000_0X200000}, /* 2MB */
    
/* Serial Flash 32Mbit(4MB) Lock range. Start from Bottom */    
    {0xFF, 0x24, SFXXM_L0X00000000_0X010000}, /* 64KB */
    {0xFF, 0x28, SFXXM_L0X00000000_0X020000}, /* 128KB */
    {0xFF, 0x2C, SFXXM_L0X00000000_0X040000}, /* 256KB */
    {0xFF, 0x30, SFXXM_L0X00000000_0X080000}, /* 512KB */
    {0xFF, 0x34, SFXXM_L0X00000000_0X100000}, /* 1MB */
    {0xFF, 0x38, SFXXM_L0X00000000_0X200000}, /* 2MB */ 
    {0xFF, 0x3C, SFXXM_L0X00000000_0X400000}, /* 4M,ALL CHIP */
    
    {0xFF, 0x44, SF32M_L0X003FF000_0X001000}, /* 4K */
    {0xFF, 0x48, SF32M_L0X003FE000_0X002000}, /* 8K */
    {0xFF, 0x4C, SF32M_L0X003FC000_0X004000}, /* 16K */
    {0xFF, 0x50, SF32M_L0X003F8000_0X008000}, /* 32K */
    
    {0xFF, 0x64, SFXXM_L0X00000000_0X001000}, /* 4K */
    {0xFF, 0x68, SFXXM_L0X00000000_0X002000}, /* 8K */
    {0xFF, 0x6C, SFXXM_L0X00000000_0X004000}, /* 16K */
    {0xFF, 0x70, SFXXM_L0X00000000_0X008000}, /* 32K */
};

static const struct spi_flash_lock_desc winbond64mbit_lock_table[] = {
/*S7:spr0, S6:sec, S5:tb, S4:bp2, S3:bp1, S2:bp0, S1:wel, S0:busy,*/
/* CMP=0 */
    /* Serial Flash 64Mbit(8MB) Lock range. Start from Top */
    {0xFF, 0x04, SF64M_L0X007E0000_0X020000}, /* 128K */
    {0xFF, 0x08, SF64M_L0X007C0000_0X040000}, /* 256K */
    {0xFF, 0x0C, SF64M_L0X00780000_0X080000}, /* 512k */
    {0xFF, 0x10, SF64M_L0X00700000_0X100000}, /* 1M */
    {0xFF, 0x14, SF64M_L0X00600000_0X200000}, /* 2M */
    {0xFF, 0x18, SF64M_L0X00400000_0X400000}, /* 4M */
    /* Serial Flash 64Mbit(8MB) Lock range. Start from Bottom */    
    {0xFF, 0x24, SFXXM_L0X00000000_0X020000}, /* 128K */
    {0xFF, 0x28, SFXXM_L0X00000000_0X040000}, /* 256K */
    {0xFF, 0x2C, SFXXM_L0X00000000_0X080000}, /* 512k */
    {0xFF, 0x30, SFXXM_L0X00000000_0X100000}, /* 1M */
    {0xFF, 0x34, SFXXM_L0X00000000_0X200000}, /* 2M */
    {0xFF, 0x38, SFXXM_L0X00000000_0X400000}, /* 4M */   
    {0xFF, 0x3C, SFXXM_L0X00000000_0X800000}, /* 8M,ALL CHIP */ 

    {0xFF, 0x44, SF64M_L0X007FF000_0X001000}, /* 4K */
    {0xFF, 0x48, SF64M_L0X007FE000_0X002000}, /* 8K */
    {0xFF, 0x4C, SF64M_L0X007FC000_0X004000}, /* 16K */
    {0xFF, 0x50, SF64M_L0X007F8000_0X008000}, /* 32K */
    
    {0xFF, 0x64, SFXXM_L0X00000000_0X001000}, /* 4K */
    {0xFF, 0x68, SFXXM_L0X00000000_0X002000}, /* 8K */
    {0xFF, 0x6C, SFXXM_L0X00000000_0X004000}, /* 16K */
    {0xFF, 0x70, SFXXM_L0X00000000_0X008000}, /* 32K */
};

static const struct spi_flash_lock_desc winbond128mbit_lock_table[] = {
/*S7:spr0, S6:sec, S5:tb, S4:bp2, S3:bp1, S2:bp0, S1:wel, S0:busy,*/
/* CMP=0 */
    /* Serial Flash 128Mbit(16MB) Lock range. Start from Top */
    {0xFF, 0x04, SF128M_L0X00FC0000_0X040000}, /* 256K */
    {0xFF, 0x08, SF128M_L0X00F80000_0X080000}, /* 512k */
    {0xFF, 0x0C, SF128M_L0X00F00000_0X100000}, /* 1M */
    {0xFF, 0x10, SF128M_L0X00E00000_0X200000}, /* 2M */
    {0xFF, 0x14, SF128M_L0X00C00000_0X400000}, /* 4M */
    {0xFF, 0x18, SF128M_L0X00800000_0X800000}, /* 8M */
    /* Serial Flash 128Mbit(16MB) Lock range. Start from Bottom */    
    {0xFF, 0x24, SFXXM_L0X00000000_0X040000}, /* 256K */
    {0xFF, 0x28, SFXXM_L0X00000000_0X080000}, /* 512k */
    {0xFF, 0x2C, SFXXM_L0X00000000_0X100000}, /* 1M */
    {0xFF, 0x30, SFXXM_L0X00000000_0X200000},/* 2M */
    {0xFF, 0x34, SFXXM_L0X00000000_0X400000},/* 4M */
    {0xFF, 0x38, SFXXM_L0X00000000_0X800000},/* 8M */
    {0xFF, 0x3C, SFXXM_L0X00000000_0X1000000}, /* 16M,ALL CHIP */ 

    {0xFF, 0x44, SF128M_L0X00FFF000_0X001000}, /* 4K */
    {0xFF, 0x48, SF128M_L0X00FFE000_0X002000}, /* 8K */
    {0xFF, 0x4C, SF128M_L0X00FFC000_0X004000}, /* 16K */
    {0xFF, 0x50, SF128M_L0X00FF8000_0X008000}, /* 32K */
    
    {0xFF, 0x64, SFXXM_L0X00000000_0X001000}, /* 4K */
    {0xFF, 0x68, SFXXM_L0X00000000_0X002000}, /* 8K */
    {0xFF, 0x6C, SFXXM_L0X00000000_0X004000}, /* 16K */
    {0xFF, 0x70, SFXXM_L0X00000000_0X008000}, /* 32K */
};

static const struct spi_flash_lock_desc winbond256mbit_lock_table[] = {
/*S07:spr0, S06:tb, S05:bp3, S04:bp2, S03:bp1, S02:bp0, S01:wel, S00:busy,*/
/*S15:sus , S14:cmp S13:lb3, S12:lb2, S11:lb1, S10:(R), S09:qe , S08:spr1,*/

/* WPS=0,CMP=0 */
    /* Serial Flash 256Mbit(32MB) Lock range. Start from Top */
    {0x00, 0x04, SF256M_L0X01FF0000_0X010000}, /* 64KB */
    {0x00, 0x08, SF256M_L0X01FE0000_0X020000}, /* 128KB */
    {0x00, 0x0C, SF256M_L0X01FC0000_0X040000}, /* 256KB */
    {0x00, 0x10, SF256M_L0X01F80000_0X080000}, /* 512KB */
    {0x00, 0x14, SF256M_L0X01F00000_0X100000}, /* 1MB */
    {0x00, 0x18, SF256M_L0X01E00000_0X200000}, /* 2MB */
    {0x00, 0x1C, SF256M_L0X01C00000_0X400000}, /* 4MB */
    {0x00, 0x20, SF256M_L0X01800000_0X800000}, /* 8MB */
    {0x00, 0x24, SF256M_L0X01000000_0X1000000}, /* 16MB */
    /* Serial Flash 256Mbit(32MB) Lock range. Start from Bottom */
    {0x00, 0x44, SFXXM_L0X00000000_0X010000}, /* 64KB */
    {0x00, 0x48, SFXXM_L0X00000000_0X020000}, /* 128KB */
    {0x00, 0x4C, SFXXM_L0X00000000_0X040000}, /* 256KB */
    {0x00, 0x50, SFXXM_L0X00000000_0X080000}, /* 512KB */
    {0x00, 0x54, SFXXM_L0X00000000_0X100000}, /* 1MB */
    {0x00, 0x58, SFXXM_L0X00000000_0X200000}, /* 2MB */
    {0x00, 0x5C, SFXXM_L0X00000000_0X400000}, /* 4MB */
    {0x00, 0x60, SFXXM_L0X00000000_0X800000}, /* 8MB */
    {0x00, 0x64, SFXXM_L0X00000000_0X1000000,}, /* 16MB */
    {0x00, 0x30, SFXXM_L0X00000000_0X2000000,}, /* 32M,ALL CHIP */ 

/* WPS=0,CMP=1 */
    {0x40, 0x00, SFXXM_L0X00000000_0X02000000}, /* 32M,ALL CHIP */
    {0x40, 0x04, SFXXM_L0X00000000_0X01FF0000}, /* 32704KB */
    {0x40, 0x08, SFXXM_L0X00000000_0X01FE0000}, /* 32640KB */
    {0x40, 0x0C, SFXXM_L0X00000000_0X01FC0000}, /* 32512KB */
    {0x40, 0x10, SFXXM_L0X00000000_0X01F80000}, /* 32256KB */
    {0x40, 0x14, SFXXM_L0X00000000_0X01F00000}, /* 31MB */
    {0x40, 0x18, SFXXM_L0X00000000_0X01E00000}, /* 30MB */
    {0x40, 0x1C, SFXXM_L0X00000000_0X01C00000}, /* 28MB */
    {0x40, 0x20, SFXXM_L0X00000000_0X01800000}, /* 24MB */
    {0x40, 0x24, SFXXM_L0X00000000_0X01000000}, /* 16MB */

    {0x40, 0x44, SF256M_L0X00010000_0X01FF0000}, /* 32704KB */
    {0x40, 0x48, SF256M_L0X00020000_0X01FE0000}, /* 32640KB */
    {0x40, 0x4C, SF256M_L0X00040000_0X01FC0000}, /* 32512KB */
    {0x40, 0x50, SF256M_L0X00080000_0X01F80000}, /* 32256KB */
    {0x40, 0x54, SF256M_L0X00100000_0X01F00000}, /* 31MB */
    {0x40, 0x58, SF256M_L0X00200000_0X01E00000}, /* 30MB */
    {0x40, 0x5C, SF256M_L0X00400000_0X01C00000}, /* 28MB */
    {0x40, 0x60, SF256M_L0X00800000_0X01800000}, /* 24MB */
    {0x40, 0x64, SF256M_L0X01000000_0X01000000}, /* 16MB */
};

static const struct winbond_spi_flash_params winbond_spi_flash_table[] = {
    {
        0x3015,8,16,16,32,"W25X16", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x3016,8,16,16,64,"W25X32", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x3017,8,16,16,128,"W25X64", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    
    {
        0x6016,8,16,16,64,"W25Q32DW", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x6017,8,16,16,128,"W25Q64FW", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x6018,8,16,16,256,"W25Q128FW", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    
    {
        0x4015,8,16,16,32,"W25Q16", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x4016,8,16,16,64,"W25Q32FV", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x4017,8,16,16,128,"W25Q64FV", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x4018,8,16,16,256,"W25Q128FV", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },      
    {
        0x4019,8,16,16,512,"W25Q256FV", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },  
};

static int dummy_bytes[2] =0;
static int winbond_qpi_4read(struct spi_flash *flash)
{ 
    u8 cs = flash->spi->cs;
    SCI_ASSERT(flash->work_mode == QPI_MODE);/*assert verified*/
    
    return spiflash_cmd_qpi_read(flash, CMD_W25_QPI_4READ,  dummy_bytes[cs]);   
}

static int winbond_spi_write(struct spi_flash *flash, u32 offset,
                    size_t len, const void *buf)
{ 
    u8 cmd = 0x32; // CMD_4PP
    flash->spi_rw_mode = WR_CMD_1BIT|WR_ADDR_1BIT|WR_DATA_4BIT;
    spiflash_cmd_program(flash, offset,len, buf, cmd);
    return 0;
}

static int winbond_spi_read(struct spi_flash *flash)
{ 
    u8 cs = flash->spi->cs;
    u8 cmd = 0xeB; //CMD_4READ
    flash->spi_rw_mode = RD_CMD_1BIT|RD_ADDR_4BIT|RD_DUMY_4BIT|RD_DATA_4BIT;
    return spiflash_cmd_spi_read(flash, cmd,  dummy_bytes[cs]);    
}

static int winbond_rst_sts_reg(struct spi_flash *flash)
{
    struct winbond_spi_flash *sfd = to_winbond_spi_flash(flash);
    int ret=0;
    u8 cmd=0;
    u32 dout;
	uint32 status_reg_val[3] = {0x00, 0x02, 0x60};
	uint32 cmd_write_reg[3] = {CMD_W25_WRSR, CMD_W25_WRSR2, CMD_W25_WRSR3};
    u8 cs = flash->spi->cs;
	uint32 i = 0;

	for(i = 0; i < 3; i++)
	{
	    spiflash_write_enable (flash);
	    
	    cmd = cmd_write_reg[i];
	    dout = status_reg_val[i];
	    spiflash_cmd_write(flash, &cmd,1, &dout,1);
	    spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
	}

    return ret;
}   

static int winbond_enter_4io(struct spi_flash *flash)
{
    struct winbond_spi_flash *sfd = to_winbond_spi_flash(flash);
    int ret=0;
    u8 status1=0,status2=0;
    u8 cmd=0;
    u32 dout;
    u8 para=0;
    u8 dummy_clocks = sfd->params->dummy_clocks;
    u8 cs = flash->spi->cs;
    
    switch(dummy_clocks) 
    {
        case DUMMY_2CLOCKS: para=DUMMYCLK_2_FREQ_50MHZ; dummy_bytes[cs]=1; break;
        case DUMMY_4CLOCKS: para=DUMMYCLK_4_FREQ_80MHZ; dummy_bytes[cs]=2; break;
        case DUMMY_6CLOCKS: para=DUMMYCLK_6_FREQ_104MHZ;dummy_bytes[cs]=3; break;
        case DUMMY_8CLOCKS: para=DUMMYCLK_8_FREQ_104MHZ;dummy_bytes[cs]=4; break;
        default: break;
    }

    cmd = CMD_W25_RDSR1; spiflash_cmd_read(flash, &cmd,1, &status1,1);
    cmd = CMD_W25_RDSR2; spiflash_cmd_read(flash, &cmd,1, &status2,1);    
    debug("SF: winbond_enter_qpi:sts1=0x%x,sts2=0x%x\n", status1,status2);

    spiflash_write_enable (flash);
    
    cmd = CMD_W25_WRSR;
    dout = (status2<<8)|(status1)|(WINBOND_SR_QE);
    spiflash_cmd_write(flash, &cmd,1, &dout,2);
    spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);

    return ret;
}   

static int winbond_enter_qpi(struct spi_flash *flash)
{
    struct winbond_spi_flash *sfd = to_winbond_spi_flash(flash);
    int ret=0;
    u8 cmd=0;
    u8 para=0;
    u8 dummy_clocks = sfd->params->dummy_clocks;
    u8 cs = flash->spi->cs;

    switch(dummy_clocks) 
    {
        case DUMMY_2CLOCKS: para=DUMMYCLK_2_FREQ_50MHZ; dummy_bytes[cs]=1; break;
        case DUMMY_4CLOCKS: para=DUMMYCLK_4_FREQ_80MHZ; dummy_bytes[cs]=2; break;
        case DUMMY_6CLOCKS: para=DUMMYCLK_6_FREQ_104MHZ;dummy_bytes[cs]=3; break;
        case DUMMY_8CLOCKS: para=DUMMYCLK_8_FREQ_104MHZ;dummy_bytes[cs]=4; break;
        default: break;
    }

    winbond_enter_4io(flash);

    /* Enable the flash in qpi mode */
    spiflash_cmd_enter_qpi(flash, CMD_ENTER_QPI);  

    /* Specified: you must enter qpi first,then can use _SETBURST */
    cmd = CMD_W25_SETBURST;
    spiflash_cmd_write(flash, &cmd,1, &para,1);
    spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);

    return (ret);
}
/*lint -save -e533 */   
static int winbond_exit_qpi(struct spi_flash *flash)
{
    int ret=0;
    ret=spiflash_cmd_exit_qpi(flash, CMD_EXIT_QPI);//return ret==true 
    return ret;    
}

static int winbond_work_configure(struct spi_flash *flash)
{
    int ret=0;
    struct winbond_spi_flash *sfd = to_winbond_spi_flash(flash);

    /* Support flash with density exceed 128Mbit, enter 32bit addr */
    if(flash->size > (0x8000000>>3)) {
        uint8 cmd = CMD_4BYTEADDREN;
        spiflash_cmd_write(flash, &cmd,1, NULL,0);        
        flash->support_4addr=SCI_TRUE;
    }
    
    if(QPI_MODE == sfd->params->support_qpi) {
        ret=winbond_enter_qpi(flash);
    } else {
        ret=winbond_enter_4io(flash);
    }

    debug("SF: winbond_work_cofigure: work_mode=%d\n", flash->work_mode);
    return ret;
}
/*lint -restore*/
#if 0
int winbond_erase(struct spi_flash *flash, u32 offset, size_t len)
{
    struct winbond_spi_flash *sfd = to_winbond_spi_flash(flash);
    u32 sector_size,block_size;
    u32 page_shift;
    u32 blocks_64k_nr,blocks_32k_nr, sectors_nr;
    u32 i=0;
    int ret=0;

    /*
     * speed things up by using bulk erase when possible.
     */
    page_shift = sfd->params->l2_page_size;
    sector_size = (1 << page_shift) * sfd->params->pages_per_sector;
    block_size  = sector_size*sfd->params->sectors_per_block;

    if (offset % sector_size || len % sector_size) {
        debug("SF: Erase offset/length not multiple of sector size\n");
        return -1;
    }

    /* Split the len as bolcks and the sectors */
    debug("SF: winbond_erase: len=0x%x as blocks=%d sectors=%d\n",\
          len, blocks_nr, sectors_nr);
    
	if(!(offset%0x10000))
	{
		blocks_64k_nr = len/0x10000;	
		blocks_32k_nr = (len%0x10000)/0x8000;
		sectors_nr = (len%0x8000)/sector_size;
	}
	else if(!(offset%0x8000))
	{
		blocks_64k_nr = 0;
		blocks_32k_nr = len/0x8000;
		sectors_nr = (len%0x8000)/sector_size;
	}
	else
	{	
		blocks_64k_nr = 0;
		blocks_32k_nr = 0;
		sectors_nr = len/sector_size;
	}

    for(i=0; i<blocks_64k_nr; i++) {
        ret = spiflash_cmd_erase(flash, CMD_W25_BE64, offset);
        offset += block_size;       
    }

    for(i=0; i<blocks_32k_nr; i++) {
        ret = spiflash_cmd_erase(flash, CMD_W25_BE32, offset);
        offset += 0x8000;       
    }

    for(i=0; i<sectors_nr; i++) {
        ret = spiflash_cmd_erase(flash, CMD_W25_SE, offset);
        offset += sector_size;
    }

//out:
    spi_release_bus(flash->spi);
    return ret;
}
#else
int winbond_erase(struct spi_flash *flash, u32 offset, size_t len)
{
	int ret=0;
	u32 block_64k_align_addr = 0,block_64k_unalign_len = 0;
	u32 block_32k_align_addr = 0,block_32k_unalign_len = 0;
    /*
     * speed things up by using bulk erase when possible.
     */    
	if(offset != (offset & (~(SECTOR_SIZE - 1))))	{
        debug("SF: Erase offset/length not multiple of sector size\n");
        return -1;
    }
	
	if(len<SECTOR_SIZE){
        debug("SF: Erase length not multiple of sector size\n");
        return -1;
    }
	
	if(len != (len & (~(SECTOR_SIZE - 1)))){
        debug("SF: Erase length not multiple of sector size\n");
        return -1;
    }	

 	block_64k_align_addr = (offset & (~(BLOCK_64K_SIZE - 1)));
	
	if(block_64k_align_addr != offset)
	{
		if(len >= (block_64k_align_addr+BLOCK_64K_SIZE-offset))
		{	
			block_64k_unalign_len = block_64k_align_addr+BLOCK_64K_SIZE-offset;
		}
		else
		{
			block_64k_unalign_len = len;
		}	
		
 		block_32k_align_addr = (offset & (~(BLOCK_32K_SIZE - 1)));	
		
		if(block_32k_align_addr != offset)
		{	
			if(len >= (block_32k_align_addr+BLOCK_32K_SIZE-offset))
			{
				block_32k_unalign_len = block_32k_align_addr+BLOCK_32K_SIZE-offset;
			}
			else
			{	
				block_32k_unalign_len = len;
			}
			
			if(block_64k_unalign_len == block_32k_unalign_len)
			{
				while(block_32k_unalign_len>=SECTOR_SIZE)
				{	
			        ret = spiflash_cmd_erase(flash, CMD_W25_SE, offset);
					if(ret)
					{
						goto out;
					}
			        offset += SECTOR_SIZE;
					block_32k_unalign_len -= SECTOR_SIZE;
					len -= SECTOR_SIZE;
				}
			}
			else
			{
				while(block_32k_unalign_len>=SECTOR_SIZE)
				{	
			        ret = spiflash_cmd_erase(flash, CMD_W25_SE, offset);
					if(ret)
					{
						goto out;
					}
			        offset += SECTOR_SIZE;
					block_32k_unalign_len -= SECTOR_SIZE;
					len -= SECTOR_SIZE;
				}
				
				if(len>=BLOCK_32K_SIZE)
				{
			        ret = spiflash_cmd_erase(flash, CMD_W25_BE32, offset);
					if(ret)
					{
						goto out;
					}
			        offset += BLOCK_32K_SIZE;
					len -= BLOCK_32K_SIZE;
				}
				else
				{
					while(len>=SECTOR_SIZE)
					{	
				        ret = spiflash_cmd_erase(flash, CMD_W25_SE, offset);
						if(ret)
						{
							goto out;
						}
				        offset += SECTOR_SIZE;
						len -= SECTOR_SIZE;
					}
				}
			}
		}
		else
		{	
			if(len>=BLOCK_32K_SIZE)
			{
		        ret = spiflash_cmd_erase(flash, CMD_W25_BE32, offset);
				if(ret)
				{
					goto out;
				}
		        offset += BLOCK_32K_SIZE;
				len -= BLOCK_32K_SIZE;			
			}
			else
			{
			    while(len>=SECTOR_SIZE)
			    {
			        ret = spiflash_cmd_erase(flash, CMD_W25_SE, offset);
					if(ret)
					{
						goto out;
					}
			        offset += SECTOR_SIZE;
					len -= SECTOR_SIZE;
			    }
			}
		}

	}

	while(len>=BLOCK_64K_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_W25_BE64, offset);
		if(ret)
		{
			goto out;
		}
        offset += BLOCK_64K_SIZE;  
		len -= BLOCK_64K_SIZE;
	}

	while(len>=BLOCK_32K_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_W25_BE32, offset);
		if(ret)
		{
			goto out;
		}
        offset += BLOCK_32K_SIZE;  
		len -= BLOCK_32K_SIZE;
	}

	while(len>=SECTOR_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_W25_SE, offset);
		if(ret)
		{
			goto out;
		}
        offset += SECTOR_SIZE;  
		len -= SECTOR_SIZE;
	}
out:
    spi_release_bus(flash->spi);
    return ret;    
}

#endif
 int winbond_lock(struct spi_flash *flash, u32 offset, size_t len)
{
    u8 cmd=0;
    u8 status=0;
    u8 status2=0;
	u8 status3 = 0;

    u8 table_size=0;
    const struct spi_flash_lock_desc *lock_desc =NULL;
    const struct spi_flash_lock_desc *lock_table=NULL;
    unsigned int i;
    LOCK_PATTERN_E lock_pattern = 0;
    
    /* Select lock table according to flash size MByte */
    if(SPIFLASH_SIZE_32Mbit == flash->size) {
        lock_table = winbond32mbit_lock_table;
        table_size = ARRAY_SIZE(winbond32mbit_lock_table);
    } else if(SPIFLASH_SIZE_64Mbit == flash->size) {
        lock_table = winbond64mbit_lock_table;
        table_size = ARRAY_SIZE(winbond64mbit_lock_table);
    } else if (SPIFLASH_SIZE_128Mbit == flash->size) {
        lock_table = winbond128mbit_lock_table;
        table_size = ARRAY_SIZE(winbond128mbit_lock_table);
    } else if (SPIFLASH_SIZE_256Mbit == flash->size) {
        lock_table = winbond256mbit_lock_table;
        table_size = ARRAY_SIZE(winbond256mbit_lock_table);
    } else {
        debug("SF: Unsupported with Winbond Size 0x%x\n", flash->size);
        return -1;
    }

    lock_pattern = spiflash_get_lock_pattern(offset, len,lock_table,table_size);
    
    /* Get lock information according to the LOCK_PATTERN*/
    for (i = 0; i < table_size; i++) {
        lock_desc = &lock_table[i];
        if (lock_pattern == lock_desc->lock_pattern)
            break;
    }
    if(lock_desc == NULL) {
        return -2;
    }

    if (i == table_size) {
        debug("SF: Unsupported Winbond Lock @0x%x len=0x%x\n", offset, len);
        return -1;
    }

    /* Write those control bit to the register */
    if(flash->size > 0x1000000) {
        /* 256Mbit or larger */
        cmd = CMD_W25_RDSR1;
        spiflash_cmd_read(flash, &cmd,1, &status, 1);
        cmd = CMD_W25_RDSR2;
        spiflash_cmd_read(flash, &cmd,1, &status2, 1);
		cmd = CMD_W25_RDSR3;
		spiflash_cmd_read(flash, &cmd,1, &status3, 1);
	//step1,check status register_1,and set status register_1 	
        if ((status & WINBOND_REG1_LOCK_MSK) != ((lock_desc->reg1_value)& 0xFF))
        {
	        status &= (~WINBOND_REG1_LOCK_MSK);
	        status |= ((lock_desc->reg1_value)&0xFF);
	        spiflash_write_enable (flash);
	        cmd = CMD_W25_WRSR1; 
	        spiflash_cmd_write(flash, &cmd,1, &status, 1);      
	        spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
			
        }
	//step2,check status register_2,and set status register_2
        if ((status2 & WINBOND_REG2_LOCK_MSK) != ((lock_desc->reg2_value)& 0xFF))
        {
	        status2 &= (~WINBOND_REG2_LOCK_MSK);
	        status2 |= ((lock_desc->reg2_value)&0xFF);
	        spiflash_write_enable (flash);
	        cmd = CMD_W25_WRSR2; 
	        spiflash_cmd_write(flash, &cmd,1, &status2, 1);      
	        spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
			
        }
	//step3,check status register_3,and set status register_3

		if((status3&WINBOND_REG3_LOCK_MSK) == WINBOND_REG3_LOCK_MSK)/*lint !e774 -e774*///check WPS	
		{
			status3 &= (~WINBOND_REG3_LOCK_MSK);
	        spiflash_write_enable (flash);
            cmd = CMD_W25_WRSR3; 
            spiflash_cmd_write(flash, &cmd,1, &status3, 1);     
            spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
		}
    } else {
        cmd = CMD_W25_RDSR1;
        spiflash_cmd_read(flash, &cmd,1, &status, 1);
        cmd = CMD_W25_RDSR2;
        spiflash_cmd_read(flash, &cmd,1, &status2, 1);
		
		if(0x17 != (flash->id & 0xff) ) //W25Q64FV not support RDSR3
		{
			cmd = CMD_W25_RDSR3;
			spiflash_cmd_read(flash, &cmd,1, &status3, 1);
		}
				
       
        if ((status & WINBOND_REG1_LOCK_MSK) != ((lock_desc->reg1_value)& 0xFF))
        {
		    status &= (~WINBOND_REG1_LOCK_MSK);
            status |= ((lock_desc->reg1_value)&0xFF);
			spiflash_write_enable (flash);
    
        	cmd = CMD_W25_WRSR; 
        	spiflash_cmd_write(flash, &cmd,1, &status, 1);
        	spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
		}
		
	    if ((status2 & WINBOND_REG2_LOCK_MSK) != 0)
        {
           
	        
            status2 &= (~ WINBOND_REG2_LOCK_MSK);
			spiflash_write_enable (flash);
            cmd = CMD_W25_WRSR2; 
            spiflash_cmd_write(flash, &cmd,1, &status2, 1);     
            spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);

        }

		if(((status3&WINBOND_REG3_LOCK_MSK) == WINBOND_REG3_LOCK_MSK) && (0x17 != (flash->id & 0xff)))//check WPS	
		{
			status3 &= (~WINBOND_REG3_LOCK_MSK);
            spiflash_write_enable (flash);
            cmd = CMD_W25_WRSR3; 
            spiflash_cmd_write(flash, &cmd,1, &status3, 1);     
            spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
		}
    }
    return 0;
}

/* 
 * For spiflash, we will unlock all chip 
 * no matter which region was locked before! 
 * The param of offset/size is invalid here! 
 */
 int winbond_unlock(struct spi_flash *flash, u32 offset, size_t len)
{
    u8 cmd=0;
    u8 status=0;
    
    // Write those control bit to the register 
    if(flash->size > 0x1000000) {
        // 256Mbit or larger 
        u8 status2=0;
        cmd = CMD_W25_RDSR1;
        spiflash_cmd_read(flash, &cmd,1, &status, 1);
        cmd = CMD_W25_RDSR2;
        spiflash_cmd_read(flash, &cmd,1, &status2, 1);

        spiflash_write_enable (flash);
        
        status &= (~WINBOND_REG1_LOCK_MSK);
        status2&= (~WINBOND_REG2_LOCK_MSK);

        cmd = CMD_W25_WRSR1; 
        spiflash_cmd_write(flash, &cmd,1, &status, 1);      
        spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
        cmd = CMD_W25_WRSR2; 
        spiflash_cmd_write(flash, &cmd,1, &status2, 1);     
        spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
    } else {
        cmd = CMD_W25_RDSR;
        spiflash_cmd_read(flash, &cmd,1, &status, 1);
        
        status &= (~WINBOND_REG1_LOCK_MSK);
        
        spiflash_write_enable (flash);
    
        cmd = CMD_W25_WRSR; 
        spiflash_cmd_write(flash, &cmd,1, &status, 1);
        spiflash_cmd_wait_ready(flash, WINBOND_TIMEOUT_CMD);
    }
	
    return 0;
}

struct winbond_spi_flash stm_winbond_obj[2] = {0};
struct spi_flash *spi_flash_probe_winbond(struct spi_slave *spi, u8 *idcode)
{
    const struct winbond_spi_flash_params *params;
    struct winbond_spi_flash *sfd=NULL;
    u32 page_size;
    u32 i;

    for (i = 0; i < ARRAY_SIZE(winbond_spi_flash_table); i++) {
        params = &winbond_spi_flash_table[i];
        if (params->id == ((idcode[1] << 8) | idcode[2]))
            break;
    }

    if (i == ARRAY_SIZE(winbond_spi_flash_table)) {
        debug("SF: Unsupported Winbond ID %02x%02x\n",
                idcode[1], idcode[2]);
        return NULL;
    }

    //stm = SCI_ALLOC(sizeof(struct winbond_spi_flash));
    if(spi->cs == 0)
        sfd = &stm_winbond_obj[0];
    else if(spi->cs == 1)
        sfd = &stm_winbond_obj[1];    
    
    if (!sfd) {
        debug("SF: Failed to allocate memory\n");
        return NULL;
    }

    sfd->params = params;
    sfd->flash.spi = spi;
    sfd->flash.name = params->name;
    sfd->flash.id = idcode[2] | (idcode[1]<<8) | (idcode[0]<<16);

    sfd->flash.work_mode = SPI_MODE;
    sfd->flash.support_4addr= SCI_FALSE;

    /* Assuming power-of-two page size initially. */
    page_size = 1 << params->l2_page_size;

    if(QPI_MODE == sfd->params->support_qpi)
        sfd->flash.write = spiflash_write;
    else
        sfd->flash.write = winbond_spi_write;
    
    sfd->flash.erase = winbond_erase;
    sfd->flash.read = spiflash_read_common;
    sfd->flash.reset = spiflash_reset;

    sfd->flash.erase_chip = spiflash_erase_chip;    
    sfd->flash.enter_4read= winbond_qpi_4read;  
    sfd->flash.enter_read= winbond_spi_read;  
    sfd->flash.lock= winbond_lock;  
    sfd->flash.unlock= winbond_unlock;
    sfd->flash.page_size = page_size;
    sfd->flash.sector_size = page_size * params->pages_per_sector;
    sfd->flash.size = page_size * params->pages_per_sector
                * params->sectors_per_block
                * params->nr_blocks;

    debug("SF: Detected %s with page size %u, total ",
           params->name, page_size);

    sfd->flash.reset(&(sfd->flash));

	//ATE test causes winbond flash been locked, so we do flash reset here during flash initialization
	winbond_rst_sts_reg(&(sfd->flash));
    
    /* Set flash in proper work mode */
    winbond_work_configure(&(sfd->flash));

    spiflash_set_clock(&(sfd->flash), sfd->params->read_freq_max);
    return &sfd->flash;
}

#endif //#if defined (CONFIG_SPI_FLASH_WINBOND) || !defined (CONFIG_SPI_FLASH)

