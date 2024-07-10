/*
 * Copyright 2009(C) Giga Device Ltd. 
 *
 * Based on drivers/mtd/spi/stmicro.c
 *
 */
#include "common.h"
#include "sc_reg.h"
#include "os_api.h"
#include "spi_flash.h"
#include "spi_flash_internal.h"


#if defined (CONFIG_SPI_FLASH_GIGA) || !defined (CONFIG_SPI_FLASH)


/* GIGA25xx-specific commands */
#define CMD_GIGA_WREN     0x06    /* Write Enable */
#define CMD_GIGA_WRDI     0x04    /* Write Disable */
#define CMD_GIGA_RDSR     0x05    /* Read Status Register */
#define CMD_GIGA_RDSR2     0x35    /* Read Status Register 2 */
#define CMD_GIGA_WRSR     0x01    /* Write Status Register */
#define CMD_GIGA_READ     0x03    /* Read Data Bytes */
#define CMD_GIGA_FAST_READ    0x0b    /* Read Data Bytes at Higher Speed */
#define CMD_GIGA_QPI_4READ       0xeb  /* qpi read */
#define CMD_GIGA_PP       0x02    /* Page Program */
#define CMD_GIGA_SE       0x20    /* Sector Erase */
#define CMD_GIGA_BE32K    0x52    /* Block Erase 32K */
#define CMD_GIGA_BE64K    0xD8    /* Block Erase 64K */

#define CMD_GIGA_BE       0xD8    /* Block Erase */
#define CMD_GIGA_CE       0xc7    /* Chip Erase */
#define CMD_GIGA_DP       0xb9    /* Deep Power-down */
#define CMD_GIGA_RES      0xab    /* Release from DP, and Read Signature */
#define CMD_GIGA_ENTER_QPI     0x38  /* enable qpi */
#define CMD_GIGA_EXIT_QPI     0xFF  /* exit qpi */

#define CMD_GIGA_QUAD_PP       0x32    /* Quad Page Program */
#define CMD_GIGA_DOUTPUT_FAST_READ    0x3B
#define CMD_GIGA_DIO_FAST_READ    0xBB
#define CMD_GIGA_QOUTPUT_FAST_READ    0x6B
#define CMD_GIGA_QIO_FAST_READ    0xEB

#define GIGA_SR_WIP     (1 << 0)    /* Write-in-Progress */
#define GIGA_SR_QE      (1 << 9)    /* Quad-enable */

#define GD_DUMMYCLK_4_FREQ_80MHZ           (0x0)
#define GD_DUMMYCLK_6_FREQ_104MHZ         (0x10)
#define GD_LQ32M_DUMMYCLK_6_FREQ_104MHZ   (0x20)
#define GD_LQ64M_DUMMYCLK_6_FREQ_104MHZ   (0x20)
#define GD_DUMMYCLK_8_FREQ_120MHZ         (0x20)
#define GD_DUMMYCLK_12_FREQ_140MHZ       (0x30)

struct giga_spi_flash_params {
    u16 idcode;
    u16 page_size;
    u16 pages_per_sector;
    u16 sectors_per_block;
    u16 nr_blocks;
    const char *name;

    u8     support_qpi;
    u8     read_freq_max;  /*Unit:Mhz*/
    u8     dummy_clocks;
};

struct giga_spi_flash {
    struct spi_flash flash;
    const struct giga_spi_flash_params *params;
};

static __inline struct giga_spi_flash *to_giga_spi_flash(struct spi_flash
                                   *flash)
{
    return container_of(flash, struct giga_spi_flash, flash); /*lint !e413*/
}

#define GIGA_REG1_LOCK_MSK 0xFC
#define GIGA_REG2_LOCK_MSK 0x41
/*S15:sus1, S14:cmp, S13:lb3, S12:lb2, S11:lb1, S10:sus2 S09:qe , S08:spr1,*/
/*S07:spr0, S06:bp4, S05:bp3, S04:bp2, S03:bp1, S02:bp0, S01:wel, S00:wip,*/
static const struct spi_flash_lock_desc giga16mbit_lock_table[] = {
/* CMP=0 */
/* Serial Flash 16Mbit(2MB) Lock range. Start from Top */
    {0xFF, 0x04, SF16M_L0X001F0000_0X010000}, /* 64KB */
    {0xFF, 0x08, SF16M_L0X001E0000_0X020000}, /* 128KB */
    {0xFF, 0x0C, SF16M_L0X001C0000_0X040000}, /* 256KB */
    {0xFF, 0x10, SF16M_L0X00180000_0X080000}, /* 512KB */
    {0xFF, 0x14, SF16M_L0X00100000_0X100000}, /* 1MB */
    
/* Serial Flash 16Mbit(2MB) Lock range. Start from Bottom */    
    {0xFF, 0x24, SFXXM_L0X00000000_0X010000}, /* 64KB */
    {0xFF, 0x28, SFXXM_L0X00000000_0X020000}, /* 128KB */
    {0xFF, 0x2C, SFXXM_L0X00000000_0X040000}, /* 256KB */
    {0xFF, 0x30, SFXXM_L0X00000000_0X080000}, /* 512KB */
    {0xFF, 0x34, SFXXM_L0X00000000_0X100000}, /* 1MB */
    {0xFF, 0x38, SFXXM_L0X00000000_0X200000}, /* 2MB,ALL CHIP */
    
    {0xFF, 0x44, SF16M_L0X001FF000_0X001000}, /* 4K */
    {0xFF, 0x48, SF16M_L0X001FE000_0X002000}, /* 8K */
    {0xFF, 0x4C, SF16M_L0X001FC000_0X004000}, /* 16K */
    {0xFF, 0x50, SF16M_L0X001F8000_0X008000}, /* 32K */
    
    {0xFF, 0x64, SFXXM_L0X00000000_0X001000}, /* 4K */
    {0xFF, 0x68, SFXXM_L0X00000000_0X002000}, /* 8K */
    {0xFF, 0x6C, SFXXM_L0X00000000_0X004000}, /* 16K */
    {0xFF, 0x70, SFXXM_L0X00000000_0X008000}, /* 32K */
};

static const struct spi_flash_lock_desc giga32mbit_lock_table[] = {
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
static const struct spi_flash_lock_desc giga64mbit_lock_table[] = {
/* CMP=0 */
/* bp4-bp3-bp2-bp1-bp0*/
    {0xFF, 0x04, SF64M_L0X007E0000_0X020000}, /* 128K */
    {0xFF, 0x08, SF64M_L0X007C0000_0X040000}, /* 256K */
    {0xFF, 0x0C, SF64M_L0X00780000_0X080000}, /* 512k */
    {0xFF, 0x10, SF64M_L0X00700000_0X100000}, /* 1M */
    {0xFF, 0x14, SF64M_L0X00600000_0X200000}, /* 2M */
    {0xFF, 0x18, SF64M_L0X00400000_0X400000}, /* 4M */
    
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

static const struct spi_flash_lock_desc giga128mbit_lock_table[] = {
/* CMP=0 */
/* bp4-bp3-bp2-bp1-bp0*/
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


static const struct spi_flash_lock_desc giga256mbit_lock_table[] = {
/* CMP=0 */
/* bp4-bp3-bp2-bp1-bp0*/
    /* Serial Flash 256Mbit(32MB) Lock range. Start from Top */

     {0xFF, 0x04, SF256M_L0X01F80000_0X080000}, /* 512K */
    {0xFF, 0x08, SF256M_L0X01F00000_0X100000}, /* 1M */
    {0xFF, 0x0C, SF256M_L0X01E00000_0X200000}, /* 2M */
    {0xFF, 0x10, SF256M_L0X01C00000_0X400000}, /* 4M */
    {0xFF, 0x14, SF256M_L0X01800000_0X800000}, /* 8M */
    {0xFF, 0x18, SF256M_L0X01000000_0X1000000}, /* 16M */
    /* Serial Flash 256Mbit(32MB) Lock range. Start from Bottom */    
    {0xFF, 0x24, SFXXM_L0X00000000_0X080000}, /* 512k */
    {0xFF, 0x28, SFXXM_L0X00000000_0X100000}, /* 1M */
    {0xFF, 0x2C, SFXXM_L0X00000000_0X200000}, /* 2M */
    {0xFF, 0x30, SFXXM_L0X00000000_0X400000},/* 4M */
    {0xFF, 0x34, SFXXM_L0X00000000_0X800000},/* 8M */
    {0xFF, 0x38, SFXXM_L0X00000000_0X1000000},/* 16M */
    {0xFF, 0x3C, SFXXM_L0X00000000_0X2000000}, /* 32M,ALL CHIP */ 

    {0xFF, 0x44, SF256M_L0X01FFF000_0X001000}, /* 4K */
    {0xFF, 0x48, SF256M_L0X01FFE000_0X002000}, /* 8K */
    {0xFF, 0x4C, SF256M_L0X01FFC000_0X004000}, /* 16K */
    {0xFF, 0x50, SF256M_L0X01FF8000_0X008000}, /* 32K */
    {0xFF, 0x58, SF256M_L0X01FF8000_0X008000}, /* 32K */ //this one is special
    
    {0xFF, 0x64, SFXXM_L0X00000000_0X001000}, /* 4K */
    {0xFF, 0x68, SFXXM_L0X00000000_0X002000}, /* 8K */
    {0xFF, 0x6C, SFXXM_L0X00000000_0X004000}, /* 16K */
    {0xFF, 0x70, SFXXM_L0X00000000_0X008000}, /* 32K */
    {0xFF, 0x78, SFXXM_L0X00000000_0X008000}, /* 32K */  //this one is special
};


static const struct giga_spi_flash_params giga_spi_flash_table[] = {
    {
        0x6015, 256, 16, 16, 32, "25LQ16",QPI_MODE,READ_FREQ_104M,DUMMY_4CLOCKS,
    },  
    {
        0x6016, 256, 16, 16, 64, "25LQ32B",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },  
#if defined(CHIP_VER_UWS6121E)
    {
        0x6017, 256, 16, 16, 128, "25LQ64B",SPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    }, 
#else
    {
        0x6017, 256, 16, 16, 128, "25LQ64B",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    }, 
#endif
#if !defined(PLATFORM_UWS6121E)
    {
        0x6018, 256, 16, 16, 256, "25LQ128",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
#else
    {
        0x6018, 256, 16, 16, 256, "25LQ128",SPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
#endif
    {
        0x6019, 256, 16, 16, 512, "25LQ256",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,   //??
    },
		
    {
        0x4017, 256, 16, 16, 128, "25Q64B",SPI_MODE,READ_FREQ_104M,DUMMY_8CLOCKS,
    },
    {
        0x4016, 256, 16, 16, 64, "25Q32B",SPI_MODE,READ_FREQ_104M,DUMMY_8CLOCKS,
    },
};
/*lint -save -e533 */
static u8 dummy_bytes[2] ={0};
static u8 paras[2] = {0};
static int giga_qpi_4read(struct spi_flash *flash)
{ 
    u8 cs = flash->spi->cs;
   return spiflash_cmd_qpi_read(flash, CMD_GIGA_FAST_READ,  dummy_bytes[cs]);    
}
static int giga_spi_read(struct spi_flash *flash)
{ 
    u8 cs = flash->spi->cs;
    u8 cmd = CMD_GIGA_QOUTPUT_FAST_READ;
    flash->spi_rw_mode = RD_CMD_1BIT|RD_ADDR_1BIT|RD_DUMY_1BIT|RD_DATA_4BIT;
    return spiflash_cmd_spi_read(flash, cmd,  dummy_bytes[cs]);
}

static int giga_spi_write(struct spi_flash *flash, u32 offset,
                    size_t len, const void *buf)
{ 
    u8 cmd = CMD_GIGA_QUAD_PP; // CMD_4PP
    flash->spi_rw_mode = WR_CMD_1BIT|WR_ADDR_1BIT|WR_DATA_4BIT;
    return spiflash_cmd_program(flash, offset,len, buf, cmd);    
}

 static void giga_enter_4io(struct spi_flash *flash)
{
    u8 status1=0,status2=0;
    u8 cmd=0;
    u32 dout;

    cmd = CMD_READ_STATUS1; spiflash_cmd_read(flash, &cmd,1, &status1,1);
    cmd = CMD_READ_STATUS2; spiflash_cmd_read(flash, &cmd,1, &status2,1);    
    debug("SF: giga_enter_4io:sts1=0x%x,sts2=0x%x\n", status1,status2);

    spiflash_write_enable (flash);
    
    cmd = CMD_WRITE_STATUS;
    dout = (status2<<8)|(status1)|(GIGA_SR_QE);
    spiflash_cmd_write(flash, &cmd,1, &dout,2);
    spiflash_cmd_wait_ready(flash, SPI_FLASH_MAX_TIMEOUT);
}

static int giga_work_configure(struct spi_flash *flash)
{
    //struct giga_spi_flash *sfd = to_giga_spi_flash(flash);
    //const struct giga_spi_flash_params *params= sfd->params;
    u8 cmd = CMD_SETBURST;

    giga_enter_4io(flash);

    /* Enable the flash in qpi mode */
    spiflash_cmd_enter_qpi(flash, CMD_GIGA_ENTER_QPI);
    
    spiflash_cmd_write(flash, &cmd,1, &paras,1);

    debug("SF: giga_work_cofigure:qpi mode, work_mode=%d\n", flash->work_mode);
    return 0;
}

#if 0
int giga_erase(struct spi_flash *flash, u32 offset, size_t len)
{
    struct giga_spi_flash *sfd = to_giga_spi_flash(flash);

    unsigned long sector_size;
    u32 blocks_64k_nr, blocks_32k_nr,sectors_nr;
    u32 i=0;
    int ret=0;

    /*
     * speed things up by using bulk erase when possible.
     */
    sector_size = (sfd->params->page_size)*(sfd->params->pages_per_sector);

    if (offset % sector_size || len % sector_size) {
        debug("SF: Erase offset/length not multiple of sector size\n");
        return -1;
    }

    /* Split the len as bolcks and the sectors of tail */
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
        spiflash_cmd_erase(flash, CMD_GIGA_BE64K, offset);
        offset += 0x10000;      
    }

    for(i=0; i<blocks_32k_nr; i++) {
        spiflash_cmd_erase(flash, CMD_GIGA_BE32K, offset);  
        offset += 0x8000;
    }
    
    for(i=0; i<sectors_nr; i++) {
        ret = spiflash_cmd_erase(flash, CMD_GIGA_SE, offset);
        offset += sector_size;
    }
    debug("SF: winbond_erase: len=0x%x as blk64k_nr=%d, blk32k_nr=%d, sector=%d\n",
            len, blocks_64k_nr, blocks_32k_nr, sectors_nr);   
    
//out:
    spi_release_bus(flash->spi);
    return ret;    
}
#else 
int giga_erase(struct spi_flash *flash, u32 offset, size_t len)
{
   //struct giga_spi_flash *sfd = to_giga_spi_flash(flash);

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
			        ret = spiflash_cmd_erase(flash, CMD_GIGA_SE, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_GIGA_SE, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_GIGA_BE32K, offset);
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
				        ret = spiflash_cmd_erase(flash, CMD_GIGA_SE, offset);
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
		        ret = spiflash_cmd_erase(flash, CMD_GIGA_BE32K, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_GIGA_SE, offset);
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
        ret = spiflash_cmd_erase(flash, CMD_GIGA_BE64K, offset);
		if(ret)
		{
			goto out;
		}
        offset += BLOCK_64K_SIZE;  
		len -= BLOCK_64K_SIZE;
	}

	while(len>=BLOCK_32K_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_GIGA_BE32K, offset);
		if(ret)
		{
			goto out;
		}
        offset += BLOCK_32K_SIZE;  
		len -= BLOCK_32K_SIZE;
	}

	while(len>=SECTOR_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_GIGA_SE, offset);
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

 int giga_lock(struct spi_flash *flash, u32 offset, size_t len)
{
    unsigned int i;
    u8 cmd=0;
    u8 status=0;
	u8 status2=0;
	u16 status_all = 0;
    u8 table_size=0;
    const struct spi_flash_lock_desc *lock_desc =NULL;
    const struct spi_flash_lock_desc *lock_table=NULL;
    LOCK_PATTERN_E lock_pattern = 0;
    
    /* Select lock table according to flash size MByte */
    if(SPIFLASH_SIZE_16Mbit == flash->size) {
        lock_table = giga16mbit_lock_table;
        table_size = ARRAY_SIZE(giga16mbit_lock_table);
    } else if(SPIFLASH_SIZE_32Mbit == flash->size) {
        lock_table = giga32mbit_lock_table;
        table_size = ARRAY_SIZE(giga32mbit_lock_table);
    } else if(SPIFLASH_SIZE_64Mbit == flash->size) {
        lock_table = giga64mbit_lock_table;
        table_size = ARRAY_SIZE(giga64mbit_lock_table);
    } else if (SPIFLASH_SIZE_128Mbit == flash->size) {
        lock_table = giga128mbit_lock_table;
        table_size = ARRAY_SIZE(giga128mbit_lock_table);
    } else if(SPIFLASH_SIZE_256Mbit == flash->size){
        lock_table = giga256mbit_lock_table;
        table_size = ARRAY_SIZE(giga256mbit_lock_table);
    }else {
        debug("SF: Unsupported with Giga Size 0x%x\n", flash->size);
        return -1;
    }

    lock_pattern = spiflash_get_lock_pattern(offset, len,lock_table,table_size);
    
    /* Get lock information */
    for (i = 0; i < table_size; i++) {
        lock_desc = &lock_table[i];
        if (lock_pattern == lock_desc->lock_pattern)
            break;
    }
    if(lock_desc == NULL) {
        return -2;
    }

    if (i == table_size) {
        debug("SF: Unsupported Giga Lock @0x%x len=0x%x\n", offset, len);
        return NULL;
    }

    /* Write those control bit to the register */
    cmd = CMD_GIGA_RDSR; 
    spiflash_cmd_read(flash, &cmd,1, &status, 1);
    cmd = CMD_GIGA_RDSR2; 
    spiflash_cmd_read(flash, &cmd,1, &status2, 1);

	
    if((status2&GIGA_REG2_LOCK_MSK) != 0)//lint !e774
    {
        status &= (~GIGA_REG1_LOCK_MSK);
	    status |= ((lock_desc->reg1_value)&0xFF);
    	status2 = status2&(~GIGA_REG2_LOCK_MSK);
		status_all = ((u16)status2 << 8) | ((u16)status);
		
	    spiflash_write_enable (flash);
	    cmd = CMD_GIGA_WRSR;    
	    spiflash_cmd_write(flash, &cmd,1, &status_all, 2);
	    spiflash_cmd_wait_ready(flash, SPI_FLASH_WIP_TIMEOUT);
    }
	else if ((status&GIGA_REG1_LOCK_MSK) != ((lock_desc->reg1_value)&0xFF))
	{
	    status &= (~GIGA_REG1_LOCK_MSK);
	    status |= ((lock_desc->reg1_value)&0xFF);
	    status_all = ((u16)status2 << 8) | ((u16)status);

	    spiflash_write_enable (flash);
	    cmd = CMD_GIGA_WRSR;    
	    spiflash_cmd_write(flash, &cmd,1, &status_all, 2);
	    spiflash_cmd_wait_ready(flash, SPI_FLASH_WIP_TIMEOUT);
	}

	return 0;
}

 int giga_unlock(struct spi_flash *flash, u32 offset, size_t len)
{
    u8 cmd=0;
    u8 status=0;
    int ret=0;
    u8 status2=0;
    u16 status_all = 0;

    /* Write those control bit to the register */
    cmd = CMD_GIGA_RDSR; 
    spiflash_cmd_read(flash, &cmd,1, &status, 1);
    cmd = CMD_GIGA_RDSR2; 
    spiflash_cmd_read(flash, &cmd,1, &status2, 1);

    status &= (~GIGA_REG1_LOCK_MSK);
    status_all = ((u16)status2 << 8) | ((u16)status);
	
    ret=spiflash_write_enable (flash);

    cmd = CMD_GIGA_WRSR;    
    spiflash_cmd_write(flash, &cmd,1, &status_all, 2);
    spiflash_cmd_wait_ready(flash, SPI_FLASH_WIP_TIMEOUT);
    return ret;
}
/*lint -restore*/
struct giga_spi_flash giga_obj[2]={0};
struct spi_flash *spi_flash_probe_giga(struct spi_slave *spi, u8 *idcode)
{
    const struct giga_spi_flash_params *params;
    struct giga_spi_flash *sfd =NULL;
    unsigned int i;
    u16 id = idcode[2] | idcode[1] << 8;
    u8 dummy_clocks =0;

    for (i = 0; i < ARRAY_SIZE(giga_spi_flash_table); i++) {
        params = &giga_spi_flash_table[i];
        if (params->idcode == id)
            break;
    }

    if (i == ARRAY_SIZE(giga_spi_flash_table)) {
        debug("SF: Unsupported Giga device ID %04x\n", id);
        return NULL;
    }

    if(spi->cs == 0)
        sfd = &giga_obj[0];
    else if(spi->cs == 1)
        sfd = &giga_obj[1];
    
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

    sfd->flash.erase = giga_erase;
    sfd->flash.read = spiflash_read_common;
    sfd->flash.reset = spiflash_reset; 

    sfd->flash.enter_4read= giga_qpi_4read; 
    sfd->flash.enter_read= giga_spi_read; 
    sfd->flash.lock= giga_lock; 
    sfd->flash.unlock= giga_unlock; 
    
    sfd->flash.page_size = params->page_size;
    sfd->flash.sector_size = params->page_size * params->pages_per_sector;    
    sfd->flash.size = params->page_size * params->pages_per_sector
        * params->sectors_per_block * params->nr_blocks;

    debug("SF: Detected %s with page size %u, total ",
           params->name, params->page_size);
    debug("giga->flash.size is %d\n",sfd->flash.size);

    dummy_clocks = sfd->params->dummy_clocks;
    switch(dummy_clocks) 
    {
        case DUMMY_4CLOCKS:
        {

            if(((sfd->flash.id &0xFF) == 0x15) ||
               ((sfd->flash.id &0xFF) == 0x16) ||
               ((sfd->flash.id &0xFF) == 0x17) ) {   
                paras[spi->cs]=GD_DUMMYCLK_6_FREQ_104MHZ;
            } else {
                paras[spi->cs]=GD_DUMMYCLK_4_FREQ_80MHZ; 
            }
            
            dummy_bytes[spi->cs]=2; break;
        }
        case DUMMY_6CLOCKS: 
        {
			if((sfd->flash.id &0xFF) == 0x16) {
                paras[spi->cs]=GD_LQ32M_DUMMYCLK_6_FREQ_104MHZ; //for GDLQ32
			} 
			else if((sfd->flash.id &0xFF) == 0x17) {
				paras[spi->cs]=GD_LQ64M_DUMMYCLK_6_FREQ_104MHZ;
			}			
			else {
            	paras[spi->cs]=GD_DUMMYCLK_6_FREQ_104MHZ;
			}
            dummy_bytes[spi->cs]=3; break;
        }
        case DUMMY_2CLOCKS: 
		{
			dummy_bytes[spi->cs]=1; break;
        }
        default: dummy_bytes[spi->cs]=3; break;
    }
    /* Set flash in proper work mode */
    if(sfd->params->support_qpi == QPI_MODE)
    {
        sfd->flash.write = spiflash_write;
        giga_work_configure(&(sfd->flash));
    }
    else
    {
        sfd->flash.write = giga_spi_write;
        giga_enter_4io(&(sfd->flash));
    }

    spiflash_set_clock(&(sfd->flash), sfd->params->read_freq_max);    
    return &sfd->flash;
}

#endif //#if defined (CONFIG_SPI_FLASH_GIGA) || !defined (CONFIG_SPI_FLASH)
