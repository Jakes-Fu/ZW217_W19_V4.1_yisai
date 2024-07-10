/*
 * SPI flash internal definitions
 *
 * Copyright (C) 2008 Spreadtrum Corporation
 */

/* Common parameters -- kind of high, but they should only occur when there
 * is a problem (and well your system already is broken), so err on the side
 * of caution in case we're dealing with slower SPI buses and/or processors.
 */
#ifndef _SPI_FLASH_INTERNAL_H_
#define _SPI_FLASH_INTERNAL_H_

#include "common.h"
#include "sfc_drvapi.h"  

//#define SF_DUBUG
#ifdef SF_DUBUG
#define debug SCI_TRACE_LOW
#else
#define debug(fmt...)
#endif

/* ---------------------------------------------------------------------------//
 * Common parameters for different Manufacturer-specific flash
 * Common commands set 
 * ---------------------------------------------------------------------------//
 */
#define CMD_READ_ID         0x9f

/* this two cmd sequence is need to reset device, 
   then it will take rRST us to resst            */
#define CMD_RSTEN               0x66
#define CMD_RST                 0x99

#define CMD_PE_SUSPEND          0x75
#define CMD_PE_RESUME           0x7A

#define CMD_ENTER_QPI           0x38
#define CMD_EXIT_QPI            0xFF

#define CMD_WRITE_DISABLE       0x04
#define CMD_WRITE_ENABLE        0x06

#define CMD_NORMAL_READ         0x03
#define CMD_4IO_READ            0xEB    //SPI 4READ/QPI 4READ
#define CMD_FAST_READ           0x0B
#define CMD_WRITE_STATUS        0x01
#define CMD_READ_STATUS         0x05
#define CMD_READ_STATUS1        0x05
#define CMD_READ_STATUS2        0x35

#define CMD_PAGE_PROGRAM        0x02

#define CMD_SECTOR_ERASE        0x20
#define CMD_CHIP_ERASE          0xC7

#define CMD_SETBURST            0xC0


#define SECTOR_SIZE             0x1000
#define BLOCK_32K_SIZE          0x8000
#define BLOCK_64K_SIZE          0x10000

/* Common status */
#define STATUS_WIP          (1 << 0)
#define STATUS_WEL          (1 << 1)


#define CREATE_CMD_(cmd_desc, _cmd, _byte_len, _cmd_mode, _bit_mode) \
{\
    cmd_desc.cmd = _cmd; \
    cmd_desc.cmd_byte_len=_byte_len; \
    cmd_desc.cmd_mode=_cmd_mode; \
    cmd_desc.bit_mode=_bit_mode; \
}

#define SPIFLASH_SIZE_2MB    0x00200000
#define SPIFLASH_SIZE_4MB    0x00400000
#define SPIFLASH_SIZE_8MB    0x00800000
#define SPIFLASH_SIZE_16MB   0x01000000
#define SPIFLASH_SIZE_32MB   0x02000000

#define SPIFLASH_SIZE_16Mbit    SPIFLASH_SIZE_2MB
#define SPIFLASH_SIZE_32Mbit    SPIFLASH_SIZE_4MB
#define SPIFLASH_SIZE_64Mbit    SPIFLASH_SIZE_8MB
#define SPIFLASH_SIZE_128Mbit   SPIFLASH_SIZE_16MB
#define SPIFLASH_SIZE_256Mbit   SPIFLASH_SIZE_32MB

#define SPIFLASH_VOLTAGE_3V  1
#define SPIFLASH_VOLTAGE_1V8 0

#define SPI_MODE            0x0
#define QPI_MODE            0x1 

#define ADDR_24BIT            0x0
#define ADDR_32BIT            0x1 

/* The dummy clocks for fast read in different frequency */
#define DUMMY_0CLOCKS     0
#define DUMMY_2CLOCKS     2
#define DUMMY_4CLOCKS     4
#define DUMMY_6CLOCKS     6
#define DUMMY_8CLOCKS     8
#define DUMMY_10CLOCKS    10

#define READ_FREQ_104M    1
#define READ_FREQ_133M    2

/* The unit of Systimer for time delay is 1ms */
#define CONFIG_SYS_HZ               1000

#define SPI_FLASH_PROG_TIMEOUT      (200 * CONFIG_SYS_HZ)
#define SPI_FLASH_PAGE_ERASE_TIMEOUT    (5 * CONFIG_SYS_HZ)
#define SPI_FLASH_SECTOR_ERASE_TIMEOUT  (10 * CONFIG_SYS_HZ)
#define SPI_FLASH_CHIP_ERASE_TIMEOUT    (125 * CONFIG_SYS_HZ)
#define SPI_FLASH_MAX_TIMEOUT           (200 * CONFIG_SYS_HZ)

#define SPI_FLASH_WEL_TIMEOUT  (2 * CONFIG_SYS_HZ)
#define SPI_FLASH_WIP_TIMEOUT  (2 * CONFIG_SYS_HZ)
#define SPI_FLASH_SFC_INT_TIMEOUT  (2 * CONFIG_SYS_HZ)

#define TIMEOUT_WRSR                    (100)
#define TIMEOUT_WRVCR                    (100)
#define TIMEOUT_WRVECR                    (100)

/* Common commands */
#define CMD_READ_ID         0x9f

#define CMD_READ_ARRAY_SLOW     0x03
#define CMD_READ_ARRAY_FAST     0x0b
#define CMD_READ_ARRAY_LEGACY       0xe8
/* BitMap for spi_rw_mode in struct flash,used in SPI4READ SPI2READ 4PP
/--------------|-------------|-------------|-------------|-/
 |   00 - 01   |   02 - 03   |   04 - 05   |   06 - 07   |
/--------------|-------------|-------------|-------------|-/
 | RD_CMD_1BIT |RD_ADDR_1BIT |RD_DUMY_1BIT |RD_DATA_1BIT |
 | RD_CMD_2BIT |RD_ADDR_2BIT |RD_DUMY_2BIT |RD_DATA_2BIT |
 | RD_CMD_4BIT |RD_ADDR_4BIT |RD_DUMY_4BIT |RD_DATA_4BIT |
/                                                          /
/--------------|-------------|-------------|-------------|-/
 |   08 - 09   |   10 - 11   |   12 - 13   |   14 - 15   |
/--------------|-------------|-------------|-------------|-/
 | WR_CMD_1BIT |WR_ADDR_1BIT |             |WR_DATA_1BIT |
 | WR_CMD_2BIT |WR_ADDR_2BIT |  Reserved   |WR_DATA_2BIT |
 | WR_CMD_4BIT |WR_ADDR_4BIT |             |WR_DATA_4BIT |   
 */
#define RD_CMD_1BIT (0x00<<0)
#define RD_CMD_2BIT (0x01<<0)
#define RD_CMD_4BIT (0x02<<0)
#define RD_CMD_MSK  (0x03<<0)
#define RD_ADDR_1BIT (0x00<<2)
#define RD_ADDR_2BIT (0x01<<2)
#define RD_ADDR_4BIT (0x02<<2)
#define RD_ADDR_MSK  (0x03<<2)
#define RD_DUMY_1BIT (0x00<<4)
#define RD_DUMY_2BIT (0x01<<4)
#define RD_DUMY_4BIT (0x02<<4)
#define RD_DUMY_MSK  (0x03<<4)
#define RD_DATA_1BIT (0x00<<6)
#define RD_DATA_2BIT (0x01<<6)
#define RD_DATA_4BIT (0x02<<6)
#define RD_DATA_MSK  (0x03<<6)

#define WR_CMD_1BIT (0x00<<8)
#define WR_CMD_2BIT (0x01<<8)
#define WR_CMD_4BIT (0x02<<8)
#define WR_CMD_MSK  (0x03<<8)
#define WR_ADDR_1BIT (0x00<<10)
#define WR_ADDR_2BIT (0x01<<10)
#define WR_ADDR_4BIT (0x02<<10)
#define WR_ADDR_MSK  (0x03<<10)
#define WR_DATA_1BIT (0x00<<14)
#define WR_DATA_2BIT (0x01<<14)
#define WR_DATA_4BIT (0x02<<14)
#define WR_DATA_MSK  (0x03<<14)

/*
    Define the Lock pattern for different serial flash.
    The enum has the lock start address and its lock size.
*/
typedef enum lock_pattern {
    /* Serial Flash Lock Range. Start from Bottom */
    SFXXM_L0X00000000_0X001000=1,/* 4K */
    SFXXM_L0X00000000_0X002000, /* 8K */
    SFXXM_L0X00000000_0X004000, /* 16K */
    SFXXM_L0X00000000_0X008000, /* 32K */
    SFXXM_L0X00000000_0X010000, /* 64KB */

    SFXXM_L0X00000000_0X020000, /* 128KB */
    SFXXM_L0X00000000_0X040000, /* 256KB */
    SFXXM_L0X00000000_0X080000, /* 512KB */
    SFXXM_L0X00000000_0X100000, /* 1MB */
    SFXXM_L0X00000000_0X200000, /* 2MB */
    
    SFXXM_L0X00000000_0X300000, /* 2MB+1MB */ 
    SFXXM_L0X00000000_0X380000, /* 3MB+512KB */ 
    SFXXM_L0X00000000_0X3C0000, /* 3MB+512KB+256K */ 
    SFXXM_L0X00000000_0X3E0000, /* 3MB+512KB+256K+128KB */
    SFXXM_L0X00000000_0X3F0000, /* 3MB+512KB+256K+128KB+64KB */
    
    SFXXM_L0X00000000_0X400000, /* 4MB */
    SFXXM_L0X00000000_0X600000, /* 4MB+2MB */ 
    SFXXM_L0X00000000_0X700000, /* 6MB+1MB */ 
    SFXXM_L0X00000000_0X780000, /* 7MB+512KB */ 
    SFXXM_L0X00000000_0X7C0000, /* 7MB+512KB+256K */ 
    
    SFXXM_L0X00000000_0X7E0000, /* 7MB+512KB+256K+128KB */
    SFXXM_L0X00000000_0X7F0000, /* 7MB+512KB+256K+128KB+64KB */
    SFXXM_L0X00000000_0X800000, /* 8MB */
    SFXXM_L0X00000000_0XF00000, /* 15MB */
    SFXXM_L0X00000000_0X1000000, /* 16MB */
    SFXXM_L0X00000000_0X2000000, /* 32MB/256Mbit */
    
    SFXXM_L0X00000000_0X4000000, /* 64MB/512Mbit */
    SFXXM_L0X00000000_0X8000000, /* 128MB/1024Mbit */
    /* Serial Flash 32Mbit(4MB) Lock range. Start from Top */
    SF32M_L0X003FF000_0X001000, /* 4K */
    SF32M_L0X003FE000_0X002000, /* 8K */
    SF32M_L0X003FC000_0X004000, /* 16K */
    
    SF32M_L0X003F8000_0X008000, /* 32K */
    SF32M_L0X003F0000_0X010000, /* 64KB */
    SF32M_L0X003E0000_0X020000, /* 128KB */
    SF32M_L0X003C0000_0X040000, /* 256KB */
    SF32M_L0X00380000_0X080000, /* 512KB */
    
    SF32M_L0X00300000_0X100000, /* 1MB */
    SF32M_L0X00200000_0X200000, /* 2MB */
    SF32M_L0X00000000_0X400000, /* 4MB,All Chip */
    /* Serial Flash 64Mbit(8MB) Lock range. Start from Top */
    SF64M_L0X007FF000_0X001000, /* 4K */
    SF64M_L0X007FE000_0X002000, /* 8K */
    
    SF64M_L0X007FC000_0X004000, /* 16K */
    SF64M_L0X007F8000_0X008000, /* 32K */
    SF64M_L0X007F0000_0X010000, /* 64K */
    SF64M_L0X007E0000_0X020000, /* 128K */
    SF64M_L0X007C0000_0X040000, /* 256K */
    
    SF64M_L0X00780000_0X080000, /* 512k */
    SF64M_L0X00700000_0X100000, /* 1M */
    SF64M_L0X00600000_0X200000, /* 2M */
    SF64M_L0X00400000_0X400000, /* 4M */
    SF64M_L0X00000000_0X800000, /* 8M,AllChip */

    /* Serial Flash 128Mbit(16MB) Lock range. Start from Top */
    SF128M_L0X00FFF000_0X001000, /* 4K */
    SF128M_L0X00FFE000_0X002000, /* 8K */
    SF128M_L0X00FFC000_0X004000, /* 16K */
    SF128M_L0X00FF8000_0X008000, /* 32K */
    SF128M_L0X00FF0000_0X010000, /* 64K */
    
    SF128M_L0X00FE0000_0X020000, /* 128K */
    SF128M_L0X00FC0000_0X040000, /* 256K */
    SF128M_L0X00F80000_0X080000, /* 512k */
    SF128M_L0X00F00000_0X100000, /* 1M */
    SF128M_L0X00E00000_0X200000, /* 2M */
    
    SF128M_L0X00C00000_0X400000, /* 4M */
    SF128M_L0X00800000_0X800000, /* 8M */
    SF128M_L0X00000000_0X1000000,/* 16M */
    /* Serial Flash 256Mbit(32MB) Lock range. Start from Top */
    SF256M_L0X01FFF000_0X001000,/* 4KB */
    SF256M_L0X01FFE000_0X002000,/* 8KB */
    SF256M_L0X01FFC000_0X004000,/* 16KB */
    SF256M_L0X01FF8000_0X008000,/* 32KB */
 	
    SF256M_L0X01FF0000_0X010000, /* 64KB */
    SF256M_L0X01FE0000_0X020000, /* 128KB */
    
    SF256M_L0X01FC0000_0X040000, /* 256KB */
    SF256M_L0X01F80000_0X080000, /* 512KB */
    SF256M_L0X01F00000_0X100000, /* 1MB */
    SF256M_L0X01E00000_0X200000, /* 2MB */
    SF256M_L0X01C00000_0X400000, /* 4MB */
    
    SF256M_L0X01800000_0X800000, /* 8MB */
    SF256M_L0X01000000_0X1000000, /* 16MB */
    SF256M_L0X00000000_0X2000000, /* 32MB */
    /* Serial Flash 256Mbit(32MB) Lock range. Start from Top */
    /* Example, winbond: WPS=0,CMP=1 */
    SFXXM_L0X00000000_0X02000000, /* 32M,ALL CHIP */
    SFXXM_L0X00000000_0X01FF0000, /* 32704KB */
    
    SFXXM_L0X00000000_0X01FE0000, /* 32640KB */
    SFXXM_L0X00000000_0X01FC0000, /* 32512KB */
    SFXXM_L0X00000000_0X01F80000, /* 32256KB */
    SFXXM_L0X00000000_0X01F00000, /* 31MB */
    SFXXM_L0X00000000_0X01E00000, /* 30MB */
    
    SFXXM_L0X00000000_0X01C00000, /* 28MB */
    SFXXM_L0X00000000_0X01800000, /* 24MB */
    SFXXM_L0X00000000_0X01000000, /* 16MB */
    SF256M_L0X00010000_0X01FF0000, /* 32704KB */
    SF256M_L0X00020000_0X01FE0000, /* 32640KB */
    
    SF256M_L0X00040000_0X01FC0000, /* 32512KB */
    SF256M_L0X00080000_0X01F80000, /* 32256KB */
    SF256M_L0X00100000_0X01F00000, /* 31MB */
    SF256M_L0X00200000_0X01E00000, /* 30MB */
    SF256M_L0X00400000_0X01C00000, /* 28MB */
    
    SF256M_L0X00800000_0X01800000, /* 24MB */
    SF256M_L0X01000000_0X01000000, /* 16MB */   
/* Serial Flash 16Mbit(2MB) Lock range. Start from Top */
    SF16M_L0X001F0000_0X010000, /* 64KB */
    SF16M_L0X001E0000_0X020000, /* 128KB */
    SF16M_L0X001C0000_0X040000, /* 256KB */
    SF16M_L0X00180000_0X080000, /* 512KB */
    SF16M_L0X00100000_0X100000, /* 1MB */
    SF16M_L0X001FF000_0X001000, /* 4K */
    SF16M_L0X001FE000_0X002000, /* 8K */
    SF16M_L0X001FC000_0X004000, /* 16K */
    SF16M_L0X001F8000_0X008000, /* 32K */

    LOCK_PATTERN_MAX,
} LOCK_PATTERN_E;

struct spi_flash_lock_desc {
    u8  reg2_value; /*0xFF mean that this register is not exist! */
    u8  reg1_value; /*0xFF mean that this register is not exist! */
    
    u8  lock_pattern;
};

struct spi_flash_lock_pattern {
    u8  lock_pattern;

    u32 start_addr;
    u32 size;
};

/*
 * Send a multi-byte command to the device and read the response. Used
 * for flash array reads, etc.
 */
 void spiflash_cmd_read(struct spi_flash *flash, const u8 *cmd, size_t cmd_len,
        const void *data_in, size_t data_len);

/*
 * Send Cmd with data parameter,or only send one command. 
 *      use to write data to control register. 
 */
 void spiflash_cmd_write(struct spi_flash *flash, const u8 *cmd, size_t cmd_len,
        const void *data_out, size_t data_len);


/* Manufacturer-specific probe functions */
struct spi_flash *spi_flash_probe_spansion(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_atmel(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_macronix(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_micron(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_sst(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_stmicro(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_winbond(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_giga(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_fidelix(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_esmt(struct spi_slave *spi, u8 *idcode);

/* Generic method for spi flash operation */
LOCK_PATTERN_E spiflash_get_lock_pattern(uint32 start_addr, uint32 size,
                                         const struct spi_flash_lock_desc* lock_table,
                                         uint32 lock_table_size);
#if !defined(PLATFORM_UWS6121E)
BYTE_NUM_E spi_flash_addr(uint32* addr, BOOLEAN support_4addr);
#endif
 int spiflash_set_clock(struct spi_flash *flash, uint32 freq_mhz);
int spiflash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
                          uint8 cmd, uint32 poll_bit, uint32 bit_value);
int spiflash_cmd_wait_ready(struct spi_flash *flash, unsigned long timeout);
int spiflash_cmd_enter_qpi(struct spi_flash *flash, u8 cmd);
int spiflash_cmd_exit_qpi(struct spi_flash *flash, u8 cmd);
int spiflash_cmd_erase(struct spi_flash *flash, u8 erase_cmd, u32 offset);
int spiflash_cmd_program(struct spi_flash *flash, u32 offset,size_t len, const void *buf, u8 cmd);
int spiflash_cmd_spi_read(struct spi_flash *flash, u8 cmd_read, u8 dummy_bytes);
int spiflash_cmd_qpi_read(struct spi_flash *flash, u8 cmd_read, u8 dummy_bytes);    

int spiflash_write_enable (struct spi_flash *flash);
int spiflash_write_disable(struct spi_flash *flash);
int spiflash_write(struct spi_flash *flash, u32 offset, size_t len, const void *buf);
int spiflash_erase_chip(struct spi_flash *flash);
int spiflash_suspend(struct spi_flash *flash);
int spiflash_resume(struct spi_flash *flash);
int spiflash_reset(struct spi_flash *flash);
int spiflash_reset_anyway(uint32 cs);

int spiflash_read_common(struct spi_flash *flash, u32 offset,
       size_t len, void *buf);

void spi_flash_common_operation(struct spi_flash *flash);
 
#endif //_SPI_FLASH_INTERNAL_H_
