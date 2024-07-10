/*
 * Interface to SPI flash
 *
 * Copyright (C) 2008 Atmel Corporation
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_
#include "./common.h"
#include "spi.h"
#include "sci_types.h"

struct spi_flash_region {
    unsigned int    count;
    unsigned int    size;
};

struct spi_flash {
    struct spi_slave *spi;

    const char  *name;
    uint32 id;

    /* Total flash size */
    u32     size;
    /* Write (page) size */
    u32     page_size;
    /* Erase (sector) size */
    u32     sector_size;

    /* SPI/QPI work mode */
    u8     work_mode;
    /* Support flash with density exceed 128Mbit, with 4Byte addr */
    u8      support_4addr;
    /* Read and Write mode in SPI work mode only */
    /* BitMap for spi_rw_mode
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
     | WR_CMD_2BIT |WR_ADDR_2BIT |             |WR_DATA_2BIT |
     | WR_CMD_4BIT |WR_ADDR_4BIT |             |WR_DATA_4BIT |   
     */
    int      spi_rw_mode;

    int     (*read)(struct spi_flash *flash, u32 offset, size_t len, void *buf);
    int     (*write)(struct spi_flash *flash, u32 offset, size_t len, const void *buf);
    int     (*erase)(struct spi_flash *flash, u32 offset, size_t len);
    int     (*erase_chip)(struct spi_flash *flash);

    /* Chip-supported reset or suspend/resume */    
    int     (*reset)(struct spi_flash *flash);    
    int     (*suspend)(struct spi_flash *flash);
    int     (*resume)(struct spi_flash *flash);
    
    /* Chip-supported device locking */
    /* 
     * For unlock(), we will unlock all chip,no matter which region was locked before! 
     * The param of offset/size is invalid here! 
     */    
    int     (*lock) (struct spi_flash *flash, u32 offset, size_t len);
    int     (*unlock) (struct spi_flash *flash, u32 offset, size_t len);   
    int     (*enter_4read)(struct spi_flash *flash);
    int     (*enter_read)(struct spi_flash *flash);

    /* reserved for other private use */
    void* priv;
};

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
        unsigned int max_hz, unsigned int spi_mode);
void spi_flash_free(struct spi_flash *flash);

#endif /* _SPI_FLASH_H_ */
