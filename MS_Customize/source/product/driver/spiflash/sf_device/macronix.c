/*
 * Copyright 2009(C) Macronix(MXIC) International Ltd. and its affiliates
 * Prafulla Wadaskar <prafulla@marvell.com>
 * Based on drivers/mtd/spi/micron.c
 *
 */
#include "common.h"
#include "os_api.h"
#include "spi_flash.h"
#include "spi_flash_internal.h"


#if defined (CONFIG_SPI_FLASH_MACRONIX) || !defined (CONFIG_SPI_FLASH)


/* MX25xx-specific commands */
#define CMD_MX25XX_WREN     0x06    /* Write Enable */
#define CMD_MX25XX_WRDI     0x04    /* Write Disable */
#define CMD_MX25XX_RDSR     0x05    /* Read Status Register */
#define CMD_MX25XX_WRSR     0x01    /* Write Status Register */
#define CMD_MX25XX_READ     0x03    /* Read Data Bytes */
#define CMD_MX25XX_FAST_READ    0x0b    /* Read Data Bytes at Higher Speed */
#define CMD_MX25XX_PP       0x02    /* Page Program */
#define CMD_MX25XX_SE       0x20    /* Sector Erase */
#define CMD_MX25XX_BE32K       0x52    /* 32K Block Erase */
#define CMD_MX25XX_BE64K       0xD8    /* 64K Block Erase */
#define CMD_MX25XX_CE       0xc7    /* Chip Erase */
#define CMD_MX25XX_DP       0xb9    /* Deep Power-down */
#define CMD_MX25XX_RES      0xab    /* Release from DP, and Read Signature */

#define CMD_MX_RDSR            0x05    /* Read Status Register */
#define CMD_MX_RDCR            0x15    /* Read Configuration Register */
#define CMD_MX_WRSR            0x01    /* Write Status Register */
#define CMD_MX_ENTER_QPI            0x35    /* enable qpi */
#define CMD_MX_EXIT_QPI            0xF5    /* exit qpi */

#define CMD_MX_QPI_4READ       0xeb  /* qpi read */
#define CMD_MX_SUSPEND         0xB0
#define CMD_MX_RESUME          0x30
#define MACRONIX_SR_WIP     (1 << 0)    /* Write-in-Progress */
#define MACRONIX_SR_WEL     (1 << 1)    /* Write-enable-latch */
#define MACRONIX_SR_QE      (1 << 6)    /* Quad-enable */
#define MACRONIX_SR_SRWD    (1 << 7)    /* Status-register-write-disable */


#define MX_DUMMYCLK_4_FREQ_80MHZ      (0x0)
#define MX_DUMMYCLK_6_FREQ_104MHZ     (0x10)
#define MX_DUMMYCLK_6_FREQ_80MHZ      (0x60) 
#define MX_DUMMYCLK_10_FREQ_104MHZ    (0xa0)
#define MX_VCR_DUMMYCLK_MASK          (0xf0)

struct macronix_spi_flash_params {
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

struct macronix_spi_flash {
    struct spi_flash flash;
    const struct macronix_spi_flash_params *params;
};

static __inline struct macronix_spi_flash *to_macronix_spi_flash(struct spi_flash
                                   *flash)
{
    return container_of(flash, struct macronix_spi_flash, flash); /*lint !e413*/
}

#define MACRONIX_REG1_LOCK_MSK 0xBC
#define MACRONIX_REG2_LOCK_MSK 0x08
/*Configuration Register Table:
 b07:DC1, b06:DC0, b05:reserved, b04:reserved, b03:TB, b02:OSD2, b01:OSD1, b00:OSD0,*/

/*Status Register Table:
 S07:spwd, S06:qe, S05:bp3, S04:bp2, S03:bp1, S02:bp0, S01:wel, S00:wip,*/
static const struct spi_flash_lock_desc macronix64mbit_lock_table[] = {
/*S07:spwd, S06:qe, S05:bp3, S04:bp2, S03:bp1, S02:bp0, S01:wel, S00:wip,*/
/* CMP=0 */
    /* Serial Flash 64Mbit(8MB) Lock range. Start from Top */
    {0xFF, 0x04, SF64M_L0X007F0000_0X010000}, /* 64K */
    {0xFF, 0x08, SF64M_L0X007E0000_0X020000}, /* 128K */
    {0xFF, 0x0C, SF64M_L0X007C0000_0X040000}, /* 256K */
    {0xFF, 0x10, SF64M_L0X00780000_0X080000}, /* 512k */
    {0xFF, 0x14, SF64M_L0X00700000_0X100000}, /* 1M */
    {0xFF, 0x18, SF64M_L0X00600000_0X200000}, /* 2M */
    {0xFF, 0x1C, SF64M_L0X00400000_0X400000}, /* 4M */
    
    /* Serial Flash 64Mbit(8MB) Lock range. Start from Bottom */    
    {0xFF, 0x20, SFXXM_L0X00000000_0X400000}, /* 4M */
    {0xFF, 0x24, SFXXM_L0X00000000_0X600000}, /* 4M+2M */
    {0xFF, 0x28, SFXXM_L0X00000000_0X700000}, /* 6M+1M */
    {0xFF, 0x2C, SFXXM_L0X00000000_0X780000}, /* 7M+512K */
    {0xFF, 0x30, SFXXM_L0X00000000_0X7C0000}, /* 7M+512K+256K */
    {0xFF, 0x34, SFXXM_L0X00000000_0X7E0000}, /* 7M+512K+256K+128K */
    {0xFF, 0x38, SFXXM_L0X00000000_0X7F0000}, /* 7M+512K+256K+128K+64K */
    {0xFF, 0x3C, SFXXM_L0X00000000_0X800000}, /* 8M,ALL CHIP */ 

};

static const struct spi_flash_lock_desc macronix128mbit_lock_table[] = {
/* tb=0 */
    /* Serial Flash 128Mbit(16MB) Lock range. Start from Top */
    {0x00, 0x04, SF128M_L0X00FF0000_0X010000}, /* 64K */
    {0x00, 0x08, SF128M_L0X00FE0000_0X020000}, /* 128K */
    {0x00, 0x0C, SF128M_L0X00FC0000_0X040000}, /* 256K */
    {0x00, 0x10, SF128M_L0X00F80000_0X080000}, /* 512k */
    {0x00, 0x14, SF128M_L0X00F00000_0X100000}, /* 1M */
    {0x00, 0x18, SF128M_L0X00E00000_0X200000}, /* 2M */
    {0x00, 0x1C, SF128M_L0X00C00000_0X400000}, /* 4M */
    {0x00, 0x20, SF128M_L0X00800000_0X800000}, /* 8M */
    {0x00, 0x24, SF128M_L0X00000000_0X1000000}, /* 16M */
    /* Serial Flash 128Mbit(16MB) Lock range. Start from Bottom */    
    {0x08, 0x04, SFXXM_L0X00000000_0X010000}, /* 64K */
    {0x08, 0x08, SFXXM_L0X00000000_0X020000}, /* 128K */
    {0x08, 0x0C, SFXXM_L0X00000000_0X040000}, /* 256K */
    {0x08, 0x10, SFXXM_L0X00000000_0X080000}, /* 512k */
    {0x08, 0x14, SFXXM_L0X00000000_0X100000}, /* 1M */
    {0x08, 0x18, SFXXM_L0X00000000_0X200000}, /* 2M */
    {0x08, 0x1C, SFXXM_L0X00000000_0X400000}, /* 4M */
    {0x08, 0x20, SFXXM_L0X00000000_0X800000}, /* 8M */
    {0x08, 0x24, SFXXM_L0X00000000_0X1000000}, /* 16M */
};

static const struct macronix_spi_flash_params macronix_spi_flash_table[] = {
    {
        0x2015,256,16,16,32,"MX25L1605D", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x2016,256,16,16,64,"MX25L3205D", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x2017,256,16,16,128,"MX25L6405D",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x2018,256,16,16,256,"MX25L12805D",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x2618,256,16,16,256,"MX25L12855E",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },

    {
        0x2538,256,16,16,256,"MX25U12835E",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x2537,256,16,16,128,"MX25U6435E",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },
    {
        0x2536,256,16,16,64,"MX25U3235E",QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },    
};
/*lint -save -e533 */
 int macronix_suspend(struct spi_flash *flash)
{  
    u8 cmd = CMD_MX_SUSPEND;
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);
    return 0;
}

 int macronix_resume(struct spi_flash *flash)
{
    u8 cmd = CMD_MX_RESUME;
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);
    return 0;
}
#if 1
int macronix_erase(struct spi_flash *flash, u32 offset, size_t len)
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
			        ret = spiflash_cmd_erase(flash, CMD_MX25XX_SE, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_MX25XX_SE, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_MX25XX_BE32K, offset);
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
				        ret = spiflash_cmd_erase(flash, CMD_MX25XX_SE, offset);
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
		        ret = spiflash_cmd_erase(flash, CMD_MX25XX_BE32K, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_MX25XX_SE, offset);
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
        ret = spiflash_cmd_erase(flash, CMD_MX25XX_BE64K, offset);
		if(ret)
		{
			goto out;
		}
        offset += BLOCK_64K_SIZE;  
		len -= BLOCK_64K_SIZE;
	}

	while(len>=BLOCK_32K_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_MX25XX_BE32K, offset);
		if(ret)
		{
			goto out;
		}
        offset += BLOCK_32K_SIZE;  
		len -= BLOCK_32K_SIZE;
	}

	while(len>=SECTOR_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_MX25XX_SE, offset);
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
#else

int macronix_erase(struct spi_flash *flash, u32 offset, size_t len)
{
    struct macronix_spi_flash *sfd = to_macronix_spi_flash(flash);

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
        spiflash_cmd_erase(flash, CMD_MX25XX_BE64K, offset);
        offset += 0x10000;      
    }

    for(i=0; i<blocks_32k_nr; i++) {
        spiflash_cmd_erase(flash, CMD_MX25XX_BE32K, offset);  
        offset += 0x8000;
    }
 
    for(i=0; i<sectors_nr; i++) {
        ret = spiflash_cmd_erase(flash, CMD_MX25XX_SE, offset);
        offset += sector_size;
    }

    debug("SF: winbond_erase: len=0x%x as blk64k_nr=%d, blk32k_nr=%d, sector=%d\n",
            len, blocks_64k_nr, blocks_32k_nr, sectors_nr);   
//out:
    spi_release_bus(flash->spi);

    return ret;    
}

#endif 
static int macronix_enter_qpi(struct spi_flash *flash)
{
    /* Set QE in the status register(S6) that enable Quad spi operation */
    u8 status=0;
    u8 cmd=0;
    u32 dout;
    int ret=0;

    cmd = CMD_MX_RDSR; spiflash_cmd_read(flash, &cmd,1, &status,1);   
    debug("SF: macronix_enter_qpi:sts=0x%x,sts2=0x%x\n", status);

    ret=spiflash_write_enable (flash);
    cmd = CMD_MX_WRSR;
    dout = (status)|(MACRONIX_SR_QE);
    spiflash_cmd_write(flash, &cmd,1, &dout,1);
    
    /* Wait write param done */
    spiflash_cmd_wait_ready(flash, SPI_FLASH_MAX_TIMEOUT);

    /* Enable the flash in qpi mode */
    spiflash_cmd_enter_qpi(flash, CMD_MX_ENTER_QPI);
    return ret;
}


static u8 dummy_bytes[2] =0;
static u8 paras[2] =0;
static int macronix_qpi_4read(struct spi_flash *flash)
{ 
    u8 cs = flash->spi->cs;
    SCI_ASSERT(flash->work_mode == QPI_MODE);/*assert verified*/
    spiflash_cmd_qpi_read(flash, CMD_MX_QPI_4READ,  dummy_bytes[cs]);
    return 0;
}

 int macronix_lock(struct spi_flash *flash, u32 offset, size_t len)
{
    u8 cmd=0;
    u8 table_size=0;
    const struct spi_flash_lock_desc *lock_desc=NULL;
    const struct spi_flash_lock_desc *lock_table=NULL;
    u32 status=0;
    u32 i;
    LOCK_PATTERN_E lock_pattern = 0;
    
    /* Select lock table according to flash size MByte */
    if(SPIFLASH_SIZE_64Mbit == flash->size) {
        lock_table = macronix64mbit_lock_table;
        table_size = ARRAY_SIZE(macronix64mbit_lock_table);
    } else if (SPIFLASH_SIZE_128Mbit == flash->size) {
        lock_table = macronix128mbit_lock_table;
        table_size = ARRAY_SIZE(macronix128mbit_lock_table);
    }  else {
        debug("SF: Unsupported with Maxronix Size 0x%x\n", flash->size);
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

    /* Lock the region according to register value with pattern */
    cmd = CMD_MX_RDSR;
    spiflash_cmd_read(flash, &cmd,1, &status, 1);

    if(lock_table->reg2_value != 0xFF) {
        u32 status2= 0;
        cmd = CMD_MX_RDCR;
        spiflash_cmd_read(flash, &cmd,1, &status2, 1);

        if((status2&MACRONIX_REG2_LOCK_MSK) != ((lock_desc->reg2_value)&0xFF)
        || ((status&MACRONIX_REG1_LOCK_MSK) != ((lock_desc->reg1_value)&0xFF)))
        {
            status &= (~MACRONIX_REG1_LOCK_MSK);
		    status |= ((lock_desc->reg1_value)&0xFF);
	        status2 &= (~MACRONIX_REG2_LOCK_MSK);
	        status2 |= ((lock_desc->reg2_value)&0xFF);        
	        status  |= (status2<<8);
        
	        spiflash_write_enable(flash);       
	        cmd = CMD_MX_WRSR; 
	        spiflash_cmd_write(flash, &cmd,1, &status, 2);
	        spiflash_cmd_wait_ready(flash, SPI_FLASH_WIP_TIMEOUT);  			
		}
    }
	else if((status&MACRONIX_REG1_LOCK_MSK) != ((lock_desc->reg1_value)&0xFF))
	{
	    status &= (~MACRONIX_REG1_LOCK_MSK);
	    status |= ((lock_desc->reg1_value)&0xFF);
        spiflash_write_enable(flash);       
        cmd = CMD_MX_WRSR; 
        spiflash_cmd_write(flash, &cmd,1, &status, 1);
        spiflash_cmd_wait_ready(flash, SPI_FLASH_WIP_TIMEOUT);  
    }
	return 0;
}

 int macronix_unlock(struct spi_flash *flash, u32 offset, size_t len)
{
    u8 cmd=0;
    u8 status1=0;
    u8 status2=0;
    u32 dout= 0;
    int ret=0;

    cmd = CMD_MX_RDSR;
    spiflash_cmd_read(flash, &cmd,1, &status1, 1);
    cmd = CMD_MX_RDCR;
    spiflash_cmd_read(flash, &cmd,1, &status2, 1);

    dout = ((status2&MACRONIX_REG2_LOCK_MSK)<<8)|(status1&MACRONIX_REG1_LOCK_MSK);

   ret= spiflash_write_enable (flash);
    cmd = CMD_MX_WRSR; 
    spiflash_cmd_write(flash, &cmd,1, &dout, 2);
    spiflash_cmd_wait_ready(flash, SPI_FLASH_WIP_TIMEOUT);
    return ret;
 }
 
static int macronix_work_configure(struct spi_flash *flash)
{
    int ret=0;
    struct macronix_spi_flash *sfd = to_macronix_spi_flash(flash);
  //  u32 dout=0;
   // u8 cmd = 0;
  //  u8 status=0;
    u8 dummy_clocks = sfd->params->dummy_clocks;
    u8 cs = flash->spi->cs;
/*
    switch(dummy_clocks) 
    {
        case DUMMY_2CLOCKS: paras=MX_DUMMYCLK_4_FREQ_80MHZ; dummy_bytes=2; break;
        case DUMMY_6CLOCKS: paras=MX_DUMMYCLK_6_FREQ_104MHZ;dummy_bytes=3; break;
        case DUMMY_10CLOCKS: paras=MX_DUMMYCLK_10_FREQ_104MHZ;dummy_bytes=5; break;
        default: break;
    }
    */
    switch(dummy_clocks) 
    {
        case DUMMY_2CLOCKS: paras[cs]=MX_DUMMYCLK_4_FREQ_80MHZ;dummy_bytes[cs]=2; break;
        case DUMMY_6CLOCKS: paras[cs]=MX_DUMMYCLK_10_FREQ_104MHZ;dummy_bytes[cs]=3; break;
        case DUMMY_10CLOCKS: paras[cs]=MX_DUMMYCLK_10_FREQ_104MHZ;dummy_bytes[cs]=5; break;
        default: break;
    }

    /* Enable the flash in qpi mode */
    ret=macronix_enter_qpi(flash);  
/*
    cmd = CMD_MX_RDVCR; 
    spiflash_cmd_read(flash, &cmd,1, &status,1);    

    status &= ~MX_VCR_DUMMYCLK_MASK;
    status |= paras[cs];

    spiflash_write_enable (flash);
    
    cmd = CMD_MX_WRVCR;
    dout = status;
    spiflash_cmd_write(flash, &cmd,1, &dout,1);
  
    spiflash_cmd_wait_ready(flash, TIMEOUT_WRVCR);  
*/
    debug("SF: macronix_work_cofigure:qpi mode, work_mode=%d\n", flash->work_mode);
    return ret;
}
/*lint -restore*/

struct macronix_spi_flash mx_obj[2]={0};
struct spi_flash *spi_flash_probe_macronix(struct spi_slave *spi, u8 *idcode)
{
    const struct macronix_spi_flash_params *params;
    struct macronix_spi_flash *sfd =NULL;
    unsigned int i;
    u16 id = idcode[2] | idcode[1] << 8;


    for (i = 0; i < ARRAY_SIZE(macronix_spi_flash_table); i++) {
        params = &macronix_spi_flash_table[i];
        if (params->idcode == id)
            break;
    }

    if (i == ARRAY_SIZE(macronix_spi_flash_table)) {
        debug("SF: Unsupported Macronix ID %04x\n", id);
        return NULL;
    }

    //mx = SCI_ALLOC(sizeof(*mx));
    if(spi->cs == 0)
        sfd = &mx_obj[0];
    else if(spi->cs == 1)
        sfd = &mx_obj[1];  
    
    if (!sfd) {
        debug("SF: Failed to allocate memory\n");
        return NULL;
    }

    sfd->params = params;
    sfd->flash.spi = spi;
    sfd->flash.name = params->name;
    sfd->flash.id = idcode[2] | (idcode[1]<<8) | (idcode[0]<<16);

    sfd->flash.write = spiflash_write;
    sfd->flash.erase = macronix_erase;
    sfd->flash.read = spiflash_read_common;

    sfd->flash.enter_4read= macronix_qpi_4read; 
    sfd->flash.suspend = macronix_suspend;
    sfd->flash.resume  = macronix_resume;
    sfd->flash.lock  = macronix_lock;
    sfd->flash.unlock= macronix_unlock;

    sfd->flash.page_size = params->page_size;
    sfd->flash.sector_size = params->page_size * params->pages_per_sector;       
    sfd->flash.size = params->page_size * params->pages_per_sector
        * params->sectors_per_block * params->nr_blocks;

    debug("SF: Detected %s with page size %u, total ",
           params->name, params->page_size);
    debug("mx->flash.size is %d\n",sfd->flash.size);

    /* Set flash in proper work mode */
    if(QPI_MODE == sfd->params->support_qpi)
        macronix_work_configure(&(sfd->flash));

    spiflash_set_clock(&(sfd->flash), sfd->params->read_freq_max);
    return &sfd->flash;
}

#endif //#if defined (CONFIG_SPI_FLASH_MACRONIX) || !defined (CONFIG_SPI_FLASH)