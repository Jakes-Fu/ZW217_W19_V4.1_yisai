/**
***********************************************************************************************************************
* Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
*
* @file        flash.c
*
* @brief
*
* @revision
* Date         Author          verison
* 2022-03-16   OneOS Team      First Version
***********************************************************************************************************************
*/
#include "os_flash.h"
#include "flash_drvapi.h"
#include "flash_drv.h"

#define OS_FLASH_LOG        rprintf

/*should be equal to s_platform_patitiion_config.sect_size*/
#define OS_FLASH_BLOCK_SIZE 0x10000

static unsigned char line_buff[OS_FLASH_BLOCK_SIZE];

extern const NOR_FLASH_CONFIG_T s_platform_patitiion_config;

int nor_flash_read(unsigned int addr, unsigned char *buf, unsigned int size)
{
    unsigned int len = FDL_FlashRead(addr, buf, size);
    //OS_FLASH_LOG("nor flash read addr[%08x], size[%x],ret = %x\r\n",addr, size, len);
    if (len == size)
    {
        return len;
    }
    else
    {
        return -1;
    }
}

int nor_flash_write(unsigned int addr, unsigned char *buf, unsigned int size)
{
    unsigned int len = FDL_FlashWriteArea(addr, buf, size);
    OS_FLASH_LOG("nor flash write addr[%08x], buf[%08x] size[%08x],ret = %08x\r\n",addr, buf, size, len);
    if (len ==size)
    {
        return len;
    }
    else
    {
        return -1;
    }
}

int nor_flash_erase(unsigned int addr, unsigned int size)
{
    unsigned int ret = 0;
    unsigned int remain_size = size;
    unsigned int block_size = s_platform_patitiion_config.sect_size;
    OS_FLASH_LOG("nor flash erase addr[%08x], size[%08x]\r\n",addr, size);

    unsigned int offset = addr & (block_size - 1);
    unsigned int aligned_addr = addr  - offset;
    OS_FLASH_LOG("aligned_addr[%08x]\r\n", aligned_addr);

    ret = FDL_FlashErase_Block(aligned_addr, block_size);
    if (ret == 1)
    {
        OS_FLASH_LOG("nor flash erase addr ok[%08x], size[%08x]\r\n",aligned_addr, block_size);
        return 0;
    }
    else
    {
        OS_FLASH_LOG("nor flash erase addr error[%08x], size[%08x]\r\n",aligned_addr, block_size);
        return -1;
    }

    OS_FLASH_LOG("nor flash erase addr[%08x], size[%x],ret = %x\r\n",addr, size, ret);

    return 0;
}

int onchip_flash_read(unsigned int addr, unsigned char *buf, unsigned int size)
{
    unsigned int len = size;
    memcpy(buf, addr, size);
    
    return len;
}
int onchip_flash_write(unsigned int addr, unsigned char *buf, unsigned int size)
{
    unsigned int len = size;
    memcpy(addr, buf, size);
    
    return len;
}
int onchip_flash_erase(unsigned int addr, unsigned int size)
{
    unsigned int len = size;
    memset(addr, 0xFF, size);

    return 0;
}

