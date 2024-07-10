/*
 * Base on winbond.c
 * Fidelix FM25M
 */
#include "common.h"
//#include <malloc.h>
#include "os_api.h"
#include "spi_flash.h"
#include "spi_flash_internal.h"

#if defined (CONFIG_SPI_FLASH_FM25M) || !defined (CONFIG_SPI_FLASH)


/* FM25M -specific commands */
#define CMD_FM25M_WREN        0x06    /* Write Enable */
#define CMD_FM25M_WRDI        0x04    /* Write Disable */
#define CMD_FM25M_RDSR        0x05    /* Read Status Register */
#define CMD_FM25M_WRSR        0x01    /* Write Status Register */
#define CMD_FM25M_RDSR1        0x05
#define CMD_FM25M_WRSR1        0x01
#define CMD_FM25M_RDSR2        0x35
#define CMD_FM25M_WRSR2        0x31

#define CMD_FM25M_READ        0x03    /* Read Data Bytes */
#define CMD_FM25M_FAST_READ   0x0b    /* Read Data Bytes at Higher Speed */
#define CMD_FM25M_PP      0x02    /* Page Program */
#define CMD_FM25M_SE      0x20    /* Sector (4K) Erase */
#define CMD_FM25M_BE32  0x52   /* Block (32K) Erase */
#define CMD_FM25M_BE64      0xd8    /* Block (64K) Erase */
#define CMD_FM25M_CE      0xc7    /* Chip Erase */
#define CMD_FM25M_DP      0xb9    /* Deep Power-down */
#define CMD_FM25M_RES     0xab    /* Release from DP, and Read Signature */

#define CMD_FM25M_SETBURST        0xC0
#define CMD_FM25M_QPI_4READ       0xeb    /* qpi read */

/* 256Mb */
#define CMD_4BYTEADDREN     0xB7
#define CMD_4BYTEADDRDI     0xE9

/* Status1/Status2 register bit map */
#define FM25M_SR_WIP      (1 << 0)    /* Write-in-Progress */

#define FM25M_SR_QE       (1 << 9)    /* Quad-enable */

/* Read dummy bytes for QPI_READ */
#define DUMMYCLK_2_FREQ_50MHZ           (0x0)
#define DUMMYCLK_4_FREQ_80MHZ           (0x10)
#define DUMMYCLK_6_FREQ_104MHZ          (0x20)
#define DUMMYCLK_8_FREQ_104MHZ          (0x30)

#define FM25M_TIMEOUT_CMD    100


struct fm25m_spi_flash_params {
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
struct fm25m_spi_flash {
    struct spi_flash flash;
    const struct fm25m_spi_flash_params *params;
};

static __inline struct fm25m_spi_flash *
to_fm25m_spi_flash(struct spi_flash *flash)
{
    return container_of(flash, struct fm25m_spi_flash, flash); /*lint !e413*/
}

#define FM25M_REG1_LOCK_MSK 0xFC  //SRP0,SEC,TB,BP2,BP1,BP0
#define FM25M_REG2_LOCK_MSK 0x41   //CMP,SRP1
#define FM25M_REG3_LOCK_MSK 0x4  //WPS


static const struct spi_flash_lock_desc fm25m32mbit_lock_table[] = {
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

static const struct spi_flash_lock_desc fm25m64mbit_lock_table[] = {
/*S7:spr0, S6:sec, S5:tb, S4:bp2, S3:bp1, S2:bp0, S1:wel, S0:busy,*/
/* CMP=0 */
    /* Serial Flash 64Mbit(8MB) Lock range. Start from Top */
    {0x0F, 0x04, SF64M_L0X007E0000_0X020000}, /* 128K */
    {0x0F, 0x08, SF64M_L0X007C0000_0X040000}, /* 256K */
    {0x0F, 0x0C, SF64M_L0X00780000_0X080000}, /* 512k */
    {0x0F, 0x10, SF64M_L0X00700000_0X100000}, /* 1M */
    {0x0F, 0x14, SF64M_L0X00600000_0X200000}, /* 2M */
    {0x0F, 0x18, SF64M_L0X00400000_0X400000}, /* 4M */
    /* Serial Flash 64Mbit(8MB) Lock range. Start from Bottom */    
    {0x0F, 0x24, SFXXM_L0X00000000_0X020000}, /* 128K */
    {0x0F, 0x28, SFXXM_L0X00000000_0X040000}, /* 256K */
    {0x0F, 0x2C, SFXXM_L0X00000000_0X080000}, /* 512k */
    {0x0F, 0x30, SFXXM_L0X00000000_0X100000}, /* 1M */
    {0x0F, 0x34, SFXXM_L0X00000000_0X200000}, /* 2M */
    {0x0F, 0x38, SFXXM_L0X00000000_0X400000}, /* 4M */   
    {0x0F, 0x3C, SFXXM_L0X00000000_0X800000}, /* 8M,ALL CHIP */ 

    {0x0F, 0x44, SF64M_L0X007FF000_0X001000}, /* 4K */
    {0x0F, 0x48, SF64M_L0X007FE000_0X002000}, /* 8K */
    {0x0F, 0x4C, SF64M_L0X007FC000_0X004000}, /* 16K */
    {0x0F, 0x50, SF64M_L0X007F8000_0X008000}, /* 32K */
    
    {0x0F, 0x64, SFXXM_L0X00000000_0X001000}, /* 4K */
    {0x0F, 0x68, SFXXM_L0X00000000_0X002000}, /* 8K */
    {0x0F, 0x6C, SFXXM_L0X00000000_0X004000}, /* 16K */
    {0x0F, 0x70, SFXXM_L0X00000000_0X008000}, /* 32K */
};

static const struct spi_flash_lock_desc fm25m128mbit_lock_table[] = {
/*S7:spr0, S6:sec, S5:tb, S4:bp2, S3:bp1, S2:bp0, S1:wel, S0:busy,*/
/* CMP=0 */
    /* Serial Flash 128Mbit(16MB) Lock range. Start from Top */
    {0x0F, 0x04, SF128M_L0X00FC0000_0X040000}, /* 256K */
    {0x0F, 0x08, SF128M_L0X00F80000_0X080000}, /* 512k */
    {0x0F, 0x0C, SF128M_L0X00F00000_0X100000}, /* 1M */
    {0x0F, 0x10, SF128M_L0X00E00000_0X200000}, /* 2M */
    {0x0F, 0x14, SF128M_L0X00C00000_0X400000}, /* 4M */
    {0x0F, 0x18, SF128M_L0X00800000_0X800000}, /* 8M */
    /* Serial Flash 128Mbit(16MB) Lock range. Start from Bottom */    
    {0x0F, 0x24, SFXXM_L0X00000000_0X040000}, /* 256K */
    {0x0F, 0x28, SFXXM_L0X00000000_0X080000}, /* 512k */
    {0x0F, 0x2C, SFXXM_L0X00000000_0X100000}, /* 1M */
    {0x0F, 0x30, SFXXM_L0X00000000_0X200000},/* 2M */
    {0x0F, 0x34, SFXXM_L0X00000000_0X400000},/* 4M */
    {0x0F, 0x38, SFXXM_L0X00000000_0X800000},/* 8M */
    {0x4F, 0x0C, SFXXM_L0X00000000_0XF00000}, /* 15M */
    {0x0F, 0x3C, SFXXM_L0X00000000_0X1000000}, /* 16M,ALL CHIP */ 

    {0x0F, 0x44, SF128M_L0X00FFF000_0X001000}, /* 4K */
    {0x0F, 0x48, SF128M_L0X00FFE000_0X002000}, /* 8K */
    {0x0F, 0x4C, SF128M_L0X00FFC000_0X004000}, /* 16K */
    {0x0F, 0x50, SF128M_L0X00FF8000_0X008000}, /* 32K */
    
    {0x0F, 0x64, SFXXM_L0X00000000_0X001000}, /* 4K */
    {0x0F, 0x68, SFXXM_L0X00000000_0X002000}, /* 8K */
    {0x0F, 0x6C, SFXXM_L0X00000000_0X004000}, /* 16K */
    {0x0F, 0x70, SFXXM_L0X00000000_0X008000}, /* 32K */
};

static const struct spi_flash_lock_desc fm25m256mbit_lock_table[] = {
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

static const struct fm25m_spi_flash_params fm25m_spi_flash_table[] = {
    {
        0x4317,8,16,16,128,"FM25M64C", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },  
    {
        0x4218,8,16,16,256,"FM25M4AA", QPI_MODE,READ_FREQ_104M,DUMMY_6CLOCKS,
    },  

};

static int dummy_bytes[2] =0;
static int fm25m_qpi_4read(struct spi_flash *flash)
{ 
    u8 cs = flash->spi->cs;
    SCI_ASSERT(flash->work_mode == QPI_MODE);/*assert verified*/
    
    return spiflash_cmd_qpi_read(flash, CMD_FM25M_QPI_4READ,  dummy_bytes[cs]);   
}

static int fm25m_spi_write(struct spi_flash *flash, u32 offset,
                    size_t len, const void *buf)
{ 
    u8 cmd = 0x32; // CMD_4PP
    flash->spi_rw_mode = WR_CMD_1BIT|WR_ADDR_1BIT|WR_DATA_4BIT;
    spiflash_cmd_program(flash, offset,len, buf, cmd);    
    return 0;
}

static int fm25m_spi_read(struct spi_flash *flash)
{ 
    u8 cs = flash->spi->cs;
    u8 cmd = 0xeB; //CMD_4READ
    flash->spi_rw_mode = RD_CMD_1BIT|RD_ADDR_4BIT|RD_DUMY_4BIT|RD_DATA_4BIT;
    return spiflash_cmd_spi_read(flash, cmd,  dummy_bytes[cs]);    
}

static int fm25m_enter_4io(struct spi_flash *flash)
{
    struct fm25m_spi_flash *sfd = to_fm25m_spi_flash(flash);
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

    cmd = CMD_FM25M_RDSR1; spiflash_cmd_read(flash, &cmd,1, &status1,1);
    cmd = CMD_FM25M_RDSR2; spiflash_cmd_read(flash, &cmd,1, &status2,1);    
    debug("SF: fm25m_enter_qpi:sts1=0x%x,sts2=0x%x\n", status1,status2);

    spiflash_write_enable (flash);
    
    cmd = CMD_FM25M_WRSR;
    dout = (status2<<8)|(status1)|(FM25M_SR_QE);
    spiflash_cmd_write(flash, &cmd,1, &dout,2);
    spiflash_cmd_wait_ready(flash, FM25M_TIMEOUT_CMD);

    return ret;
}   

static int fm25m_enter_qpi(struct spi_flash *flash)
{
    struct fm25m_spi_flash *sfd = to_fm25m_spi_flash(flash);
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

    fm25m_enter_4io(flash);

    /* Enable the flash in qpi mode */
    spiflash_cmd_enter_qpi(flash, CMD_ENTER_QPI);  

    /* Specified: you must enter qpi first,then can use _SETBURST */
    cmd = CMD_FM25M_SETBURST;
    spiflash_cmd_write(flash, &cmd,1, &para,1);
    spiflash_cmd_wait_ready(flash, FM25M_TIMEOUT_CMD);

    return (ret);
}
/*lint -save -e533 */   
static int fm25m_exit_qpi(struct spi_flash *flash)
{
    spiflash_cmd_exit_qpi(flash, CMD_EXIT_QPI);  
    return 0;
}

static int fm25m_work_configure(struct spi_flash *flash)
{
    struct fm25m_spi_flash *sfd = to_fm25m_spi_flash(flash);

    /* Support flash with density exceed 128Mbit, enter 32bit addr */
    if(flash->size > (0x8000000>>3)) {
        uint8 cmd = CMD_4BYTEADDREN;
        spiflash_cmd_write(flash, &cmd,1, NULL,0);        
        flash->support_4addr=SCI_TRUE;
    }
    
    if(QPI_MODE == sfd->params->support_qpi) {
        fm25m_enter_qpi(flash);
    } else {
        fm25m_enter_4io(flash);
    }

    debug("SF: fm25m_work_cofigure: work_mode=%d\n", flash->work_mode);      
    return 0;
}
/*lint -restore*/
#if 0
int fm25m_erase(struct spi_flash *flash, u32 offset, size_t len)
{
    struct fm25m_spi_flash *sfd = to_fm25m_spi_flash(flash);
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
    debug("SF: fm25m_erase: len=0x%x as blocks=%d sectors=%d\n",\
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
        ret = spiflash_cmd_erase(flash, CMD_FM25M_BE64, offset);
        offset += block_size;       
    }

    for(i=0; i<blocks_32k_nr; i++) {
        ret = spiflash_cmd_erase(flash, CMD_FM25M_BE32, offset);
        offset += 0x8000;       
    }

    for(i=0; i<sectors_nr; i++) {
        ret = spiflash_cmd_erase(flash, CMD_FM25M_SE, offset);
        offset += sector_size;
    }

//out:
    spi_release_bus(flash->spi);
    return ret;
}
#else
int fm25m_erase(struct spi_flash *flash, u32 offset, size_t len)
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
			        ret = spiflash_cmd_erase(flash, CMD_FM25M_SE, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_FM25M_SE, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_FM25M_BE32, offset);
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
				        ret = spiflash_cmd_erase(flash, CMD_FM25M_SE, offset);
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
		        ret = spiflash_cmd_erase(flash, CMD_FM25M_BE32, offset);
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
			        ret = spiflash_cmd_erase(flash, CMD_FM25M_SE, offset);
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
        ret = spiflash_cmd_erase(flash, CMD_FM25M_BE64, offset);
		if(ret)
		{
			goto out;
		}
        offset += BLOCK_64K_SIZE;  
		len -= BLOCK_64K_SIZE;
	}

	while(len>=BLOCK_32K_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_FM25M_BE32, offset);
		if(ret)
		{
			goto out;
		}
        offset += BLOCK_32K_SIZE;  
		len -= BLOCK_32K_SIZE;
	}

	while(len>=SECTOR_SIZE)
	{
        ret = spiflash_cmd_erase(flash, CMD_FM25M_SE, offset);
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

 int fm25m_lock(struct spi_flash *flash, u32 offset, size_t len)
{
    u8 cmd=0;
    u8 status=0;
    u8 status2=0;
    u16 status_all = 0;

    u8 table_size=0;
    const struct spi_flash_lock_desc *lock_desc =NULL;
    const struct spi_flash_lock_desc *lock_table=NULL;
    unsigned int i;
    LOCK_PATTERN_E lock_pattern = 0;
    
    /* Select lock table according to flash size MByte */
    if(SPIFLASH_SIZE_32Mbit == flash->size) {
        lock_table = fm25m32mbit_lock_table;
        table_size = ARRAY_SIZE(fm25m32mbit_lock_table);
    } else if(SPIFLASH_SIZE_64Mbit == flash->size) {
        lock_table = fm25m64mbit_lock_table;
        table_size = ARRAY_SIZE(fm25m64mbit_lock_table);
    } else if (SPIFLASH_SIZE_128Mbit == flash->size) {
        lock_table = fm25m128mbit_lock_table;
        table_size = ARRAY_SIZE(fm25m128mbit_lock_table);
    } else if (SPIFLASH_SIZE_256Mbit == flash->size) {
        lock_table = fm25m256mbit_lock_table;
        table_size = ARRAY_SIZE(fm25m256mbit_lock_table);
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
        debug("SF: Unsupported Fidelix Lock @0x%x len=0x%x\n", offset, len);
        return -1;
    }

    //lock_desc = &fm25m128mbit_lock_table[11];
    
    cmd = CMD_FM25M_RDSR;
    spiflash_cmd_read(flash, &cmd,1, &status, 1);
    cmd = CMD_FM25M_RDSR2;
    spiflash_cmd_read(flash, &cmd,1, &status2, 1);

    if ((status & FM25M_REG1_LOCK_MSK) != ((lock_desc->reg1_value) & FM25M_REG1_LOCK_MSK))
    {
        status &= (~FM25M_REG1_LOCK_MSK);
        status |= ((lock_desc->reg1_value) & (FM25M_REG1_LOCK_MSK));
        status2 &= (~FM25M_REG2_LOCK_MSK);
        status2 |= ((lock_desc->reg2_value) & (FM25M_REG2_LOCK_MSK));
        status_all = ((u16)status2 << 8) | ((u16)status);

        spiflash_write_enable (flash);
    
        cmd = CMD_FM25M_WRSR; 
        spiflash_cmd_write(flash, &cmd, 1, &status_all, 2);
        spiflash_cmd_wait_ready(flash, FM25M_TIMEOUT_CMD);
    }

    return 0;
}

/* 
 * For spiflash, we will unlock all chip 
 * no matter which region was locked before! 
 * The param of offset/size is invalid here! 
 */
 int fm25m_unlock(struct spi_flash *flash, u32 offset, size_t len)
{
    u8 cmd=0;
    u8 status=0;
    
    // Write those control bit to the register 
    if(flash->size > 0x1000000) {
        // 256Mbit or larger 
        u8 status2=0;
        cmd = CMD_FM25M_RDSR1;
        spiflash_cmd_read(flash, &cmd,1, &status, 1);
        cmd = CMD_FM25M_RDSR2;
        spiflash_cmd_read(flash, &cmd,1, &status2, 1);

        spiflash_write_enable (flash);
        
        status &= (~FM25M_REG1_LOCK_MSK);
        status2&= (~FM25M_REG2_LOCK_MSK);

        cmd = CMD_FM25M_WRSR1; 
        spiflash_cmd_write(flash, &cmd,1, &status, 1);      
        spiflash_cmd_wait_ready(flash, FM25M_TIMEOUT_CMD);
        cmd = CMD_FM25M_WRSR2; 
        spiflash_cmd_write(flash, &cmd,1, &status2, 1);     
        spiflash_cmd_wait_ready(flash, FM25M_TIMEOUT_CMD);
    } else {
        cmd = CMD_FM25M_RDSR;
        spiflash_cmd_read(flash, &cmd,1, &status, 1);
        
        status &= (~FM25M_REG1_LOCK_MSK);
        
        spiflash_write_enable (flash);
    
        cmd = CMD_FM25M_WRSR; 
        spiflash_cmd_write(flash, &cmd,1, &status, 1);
        spiflash_cmd_wait_ready(flash, FM25M_TIMEOUT_CMD);
    }
	
    return 0;
}

struct fm25m_spi_flash stm_fm25m_obj[2] = {0};
struct spi_flash *spi_flash_probe_fm25m(struct spi_slave *spi, u8 *idcode)
{
    const struct fm25m_spi_flash_params *params;
    struct fm25m_spi_flash *sfd=NULL;
    u32 page_size;
    u32 i;

    for (i = 0; i < ARRAY_SIZE(fm25m_spi_flash_table); i++) {
        params = &fm25m_spi_flash_table[i];
        if (params->id == ((idcode[1] << 8) | idcode[2]))
            break;
    }

    if (i == ARRAY_SIZE(fm25m_spi_flash_table)) {
        debug("SF: Unsupported Fidelix ID %02x%02x\n",
                idcode[1], idcode[2]);
        return NULL;
    }

    //stm = SCI_ALLOC(sizeof(struct fm25m_spi_flash));
    if(spi->cs == 0)
        sfd = &stm_fm25m_obj[0];
    else if(spi->cs == 1)
        sfd = &stm_fm25m_obj[1];    
    
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
        sfd->flash.write = fm25m_spi_write;
    
    sfd->flash.erase = fm25m_erase;
    sfd->flash.read = spiflash_read_common;
    sfd->flash.reset = spiflash_reset;

    sfd->flash.erase_chip = spiflash_erase_chip;    
    sfd->flash.enter_4read= fm25m_qpi_4read;  
    sfd->flash.enter_read= fm25m_spi_read;  
    sfd->flash.lock= fm25m_lock;  
    sfd->flash.unlock= fm25m_unlock;
    sfd->flash.page_size = page_size;
    sfd->flash.sector_size = page_size * params->pages_per_sector;
    sfd->flash.size = page_size * params->pages_per_sector
                * params->sectors_per_block
                * params->nr_blocks;

    debug("SF: Detected %s with page size %u, total ",
           params->name, page_size);

    sfd->flash.reset(&(sfd->flash));
    
    /* Set flash in proper work mode */
    fm25m_work_configure(&(sfd->flash));

    spiflash_set_clock(&(sfd->flash), sfd->params->read_freq_max);
    return &sfd->flash;
}

#endif //#if defined (CONFIG_SPI_FLASH_FM25M) || !defined (CONFIG_SPI_FLASH)

