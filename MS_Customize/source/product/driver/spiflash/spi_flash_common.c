/******************************************************************************
 ** File Name:      spi_flash_common.c                                        *
 ** Author:         changde.li                                                *
 ** DATE:           08/15/2012                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 interface for spi flash common operation                  * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08-15-2012     changde.li       Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "arm_reg.h"
#include "sc_reg.h" 
#include "sci_types.h"
#include "common.h"
//#include "malloc.h"
#include "spi.h"
#include "spi_flash.h"
#include "sfc_drvapi.h"
#include "spi_flash_internal.h"
#include "spiflash.h"
#include "flash_drvapi.h"

/*
    Define the Lock pattern for different serial flash.
    The enum has the lock start address and its lock size.
*/
static const struct spi_flash_lock_pattern sf_lock_pattern[] = 
{
    {SFXXM_L0X00000000_0X001000,  0X00000000, 0X001000 }, /* 4K */
    {SFXXM_L0X00000000_0X002000,  0X00000000, 0X002000 }, /* 8K */
    {SFXXM_L0X00000000_0X004000,  0X00000000, 0X004000 }, /* 16K */
    {SFXXM_L0X00000000_0X008000,  0X00000000, 0X008000 }, /* 32K */
    {SFXXM_L0X00000000_0X010000,  0X00000000, 0X010000 }, /* 64KB */
    {SFXXM_L0X00000000_0X020000,  0X00000000, 0X020000 }, /* 128KB */
    {SFXXM_L0X00000000_0X040000,  0X00000000, 0X040000 }, /* 256KB */
    {SFXXM_L0X00000000_0X080000,  0X00000000, 0X080000 }, /* 512KB */
    {SFXXM_L0X00000000_0X100000,  0X00000000, 0X100000 }, /* 1MB */
    {SFXXM_L0X00000000_0X200000,  0X00000000, 0X200000 }, /* 2MB */
    {SFXXM_L0X00000000_0X400000,  0X00000000, 0X400000 }, /* 4MB */
    {SFXXM_L0X00000000_0X800000,  0X00000000, 0X800000 }, /* 8MB */
    {SFXXM_L0X00000000_0X1000000, 0X00000000, 0X1000000}, /* 16MB */
    {SFXXM_L0X00000000_0X2000000, 0X00000000, 0X2000000}, /* 32MB/256Mbit */
    {SFXXM_L0X00000000_0X4000000, 0X00000000, 0X4000000}, /* 64MB/512Mbit */
    {SFXXM_L0X00000000_0X8000000, 0X00000000, 0X8000000}, /* 128MB/1024Mbit */

    /* Flash 32Mb(4MB) pattern,Start from bottom */ 
    {SFXXM_L0X00000000_0X300000, 0X00000000, 0X300000}, /* 2MB+1MB */ 
    {SFXXM_L0X00000000_0X380000, 0X00000000, 0X380000}, /* 3MB+512KB */ 
    {SFXXM_L0X00000000_0X3C0000, 0X00000000, 0X3C0000}, /* 3MB+512KB+256K */ 
    {SFXXM_L0X00000000_0X3E0000, 0X00000000, 0X3E0000}, /* 3MB+512KB+256K+128KB */
    {SFXXM_L0X00000000_0X3F0000, 0X00000000, 0X3F0000}, /* 3MB+512KB+256K+128KB+64KB */
    /* Flash 64Mb(8MB) pattern,Start from bottom */
    {SFXXM_L0X00000000_0X600000, 0X00000000, 0X600000}, /* 4MB+2MB */ 
    {SFXXM_L0X00000000_0X700000, 0X00000000, 0X700000}, /* 6MB+1MB */ 
    {SFXXM_L0X00000000_0X780000, 0X00000000, 0X780000}, /* 7MB+512KB */ 
    {SFXXM_L0X00000000_0X7C0000, 0X00000000, 0X7C0000}, /* 7MB+512KB+256K */ 
    {SFXXM_L0X00000000_0X7E0000, 0X00000000, 0X7E0000}, /* 7MB+512KB+256K+128KB */
    {SFXXM_L0X00000000_0X7F0000, 0X00000000, 0X7F0000}, /* 7MB+512KB+256K+128KB+64KB */
    /* Flash 128Mb(16MB) pattern,Start from bottom */
    {SFXXM_L0X00000000_0XF00000, 0X00000000, 0XF00000}, /* 15MB */

    /* Serial Flash 32Mbit(4MB) Lock range. Start from Top */
    {SF32M_L0X003FF000_0X001000, 0X003FF000, 0X001000}, /* 4K */
    {SF32M_L0X003FE000_0X002000, 0X003FE000, 0X002000}, /* 8K */
    {SF32M_L0X003FC000_0X004000, 0X003FC000, 0X004000}, /* 16K */
    {SF32M_L0X003F8000_0X008000, 0X003F8000, 0X008000}, /* 32K */
    {SF32M_L0X003F0000_0X010000, 0X003F0000, 0X010000}, /* 64KB */
    {SF32M_L0X003E0000_0X020000, 0X003E0000, 0X020000}, /* 128KB */
    {SF32M_L0X003C0000_0X040000, 0X003C0000, 0X040000}, /* 256KB */
    {SF32M_L0X00380000_0X080000, 0X00380000, 0X080000}, /* 512KB */
    {SF32M_L0X00300000_0X100000, 0X00300000, 0X100000}, /* 1MB */
    {SF32M_L0X00200000_0X200000, 0X00200000, 0X200000}, /* 2MB */
    {SF32M_L0X00000000_0X400000, 0X00000000, 0X400000}, /* 4MB,All Chip */

    /* Serial Flash 64Mbit(8MB) Lock range. Start from Top */
    {SF64M_L0X007FF000_0X001000, 0X007FF000, 0X001000}, /* 4K */
    {SF64M_L0X007FE000_0X002000, 0X007FE000, 0X002000}, /* 8K */
    {SF64M_L0X007FC000_0X004000, 0X007FC000, 0X004000}, /* 16K */
    {SF64M_L0X007F8000_0X008000, 0X007F8000, 0X008000}, /* 32K */
    {SF64M_L0X007F0000_0X010000, 0X007F0000, 0X010000}, /* 64K */
    {SF64M_L0X007E0000_0X020000, 0X007E0000, 0X020000}, /* 128K */
    {SF64M_L0X007C0000_0X040000, 0X007C0000, 0X040000}, /* 256K */
    {SF64M_L0X00780000_0X080000, 0X00780000, 0X080000}, /* 512k */
    {SF64M_L0X00700000_0X100000, 0X00700000, 0X100000}, /* 1M */
    {SF64M_L0X00600000_0X200000, 0X00600000, 0X200000}, /* 2M */
    {SF64M_L0X00400000_0X400000, 0X00400000, 0X400000}, /* 4M */
    {SF64M_L0X00000000_0X800000, 0X00000000, 0X800000}, /* 8M,AllChip */

    /* Serial Flash 128Mbit(16MB) Lock range. Start from Top */
    {SF128M_L0X00FFF000_0X001000, 0X00FFF000, 0X001000}, /* 4K */
    {SF128M_L0X00FFE000_0X002000, 0X00FFE000, 0X002000}, /* 8K */
    {SF128M_L0X00FFC000_0X004000, 0X00FFC000, 0X004000}, /* 16K */
    {SF128M_L0X00FF8000_0X008000, 0X00FF8000, 0X008000}, /* 32K */
    {SF128M_L0X00FF0000_0X010000, 0X00FF0000, 0X010000}, /* 64K */
    {SF128M_L0X00FE0000_0X020000, 0X00FE0000, 0X020000}, /* 128K */ 
    {SF128M_L0X00FC0000_0X040000, 0X00FC0000, 0X040000}, /* 256K */
    {SF128M_L0X00F80000_0X080000, 0X00F80000, 0X080000}, /* 512k */
    {SF128M_L0X00F00000_0X100000, 0X00F00000, 0X100000}, /* 1M */
    {SF128M_L0X00E00000_0X200000, 0X00E00000, 0X200000}, /* 2M */
    {SF128M_L0X00C00000_0X400000, 0X00C00000, 0X400000}, /* 4M */
    {SF128M_L0X00800000_0X800000, 0X00800000, 0X800000}, /* 8M */
    {SF128M_L0X00000000_0X1000000,0X00000000, 0X1000000},/* 16M */

    /* Serial Flash 256Mbit(32MB) Lock range. Start from Top */

    {SF256M_L0X01FFF000_0X001000, 0X01FFF000, 0X01000}, /* 4KB */
    {SF256M_L0X01FFE000_0X002000, 0X01FFE000, 0X02000}, /* 8KB */
    {SF256M_L0X01FFC000_0X004000, 0X01FFC000, 0X04000}, /* 16KB */
    {SF256M_L0X01FF8000_0X008000, 0X01FF8000, 0X08000}, /* 32KB */
	
    {SF256M_L0X01FF0000_0X010000, 0X01FF0000, 0X010000}, /* 64KB */
    {SF256M_L0X01FE0000_0X020000, 0X01FE0000, 0X020000}, /* 128KB */
    {SF256M_L0X01FC0000_0X040000, 0X01FC0000, 0X040000}, /* 256KB */
    {SF256M_L0X01F80000_0X080000, 0X01F80000, 0X080000}, /* 512KB */
    {SF256M_L0X01F00000_0X100000, 0X01F00000, 0X100000}, /* 1MB */
    {SF256M_L0X01E00000_0X200000, 0X01E00000, 0X200000}, /* 2MB */
    {SF256M_L0X01C00000_0X400000, 0X01C00000, 0X400000}, /* 4MB */
    {SF256M_L0X01800000_0X800000, 0X01800000, 0X800000}, /* 8MB */
    {SF256M_L0X01000000_0X1000000,0X01000000, 0X1000000},/* 16MB */
    {SF256M_L0X00000000_0X2000000,0X00000000, 0X2000000},/* 32MB */

    /* Serial Flash 256Mbit(32MB) Lock range. Start from Top */
    /* Example, winbond: WPS=0,CMP=1 */
    {SFXXM_L0X00000000_0X02000000, 0X00000000, 0X02000000}, /* 32M,ALL CHIP */
    {SFXXM_L0X00000000_0X01FF0000, 0X00000000, 0X01FF0000}, /* 32704KB */
    {SFXXM_L0X00000000_0X01FE0000, 0X00000000, 0X01FE0000}, /* 32640KB */
    {SFXXM_L0X00000000_0X01FC0000, 0X00000000, 0X01FC0000}, /* 32512KB */
    {SFXXM_L0X00000000_0X01F80000, 0X00000000, 0X01F80000}, /* 32256KB */
    {SFXXM_L0X00000000_0X01F00000, 0X00000000, 0X01F00000}, /* 31MB */
    {SFXXM_L0X00000000_0X01E00000, 0X00000000, 0X01E00000}, /* 30MB */
    {SFXXM_L0X00000000_0X01C00000, 0X00000000, 0X01C00000}, /* 28MB */
    {SFXXM_L0X00000000_0X01800000, 0X00000000, 0X01800000}, /* 24MB */
    {SFXXM_L0X00000000_0X01000000, 0X00000000, 0X01000000}, /* 16MB */

    {SF256M_L0X00010000_0X01FF0000, 0X00010000, 0X01FF0000}, /* 32704KB */
    {SF256M_L0X00020000_0X01FE0000, 0X00020000, 0X01FE0000}, /* 32640KB */
    {SF256M_L0X00040000_0X01FC0000, 0X00040000, 0X01FC0000}, /* 32512KB */
    {SF256M_L0X00080000_0X01F80000, 0X00080000, 0X01F80000}, /* 32256KB */
    {SF256M_L0X00100000_0X01F00000, 0X00100000, 0X01F00000}, /* 31MB */
    {SF256M_L0X00200000_0X01E00000, 0X00200000, 0X01E00000}, /* 30MB */
    {SF256M_L0X00400000_0X01C00000, 0X00400000, 0X01C00000}, /* 28MB */
    {SF256M_L0X00800000_0X01800000, 0X00800000, 0X01800000}, /* 24MB */
    {SF256M_L0X01000000_0X01000000, 0X01000000, 0X01000000}, /* 16MB */
};


static uint32 get_timer( )
{
    volatile uint32 tmp_tick1;
    volatile uint32 tmp_tick2;

    tmp_tick1 = SYSTEM_CURRENT_CLOCK;
    tmp_tick2 = SYSTEM_CURRENT_CLOCK;

    while (tmp_tick1 != tmp_tick2)
    {
        tmp_tick1 = tmp_tick2;
        tmp_tick2 = SYSTEM_CURRENT_CLOCK;
    }

    return tmp_tick1;
}

/*
 * Adjust 24bits/32bits address fomat because of endian support.
 * Note:
 *     return the enum type of BYTE_NUM_E
 */
 BYTE_NUM_E spi_flash_addr(uint32* addr, BOOLEAN support_4addr)
{
    uint8 cmd[4] = {0};
    uint32 address =*addr;  
    /* send address in byte0-byte1-byte2-byte3 */
    cmd[0] = ((address>>0)&(0xFF));
    cmd[1] = ((address>>8)&(0xFF));
    cmd[2] = ((address>>16)&(0xFF));
    cmd[3] = ((address>>24)&(0xFF));

    /* controller specified */
    if(support_4addr == SCI_TRUE) 
    {
    #ifdef BIG_ENDIAN
        *addr = (cmd[3]<<0)|(cmd[2]<<8)|(cmd[1]<<16)|(cmd[0]<<24);
    #else   
        *addr = (cmd[3]<<0)|(cmd[2]<<8)|(cmd[1]<<16)|(cmd[0]<<24);
    #endif
        return (BYTE_NUM_4);
    }
    else  //BYTE_NUM_3
    {
      /* Send mode_1 [27:00]*/
    #ifdef BIG_ENDIAN   
        *addr = (cmd[3]<<0)|(cmd[2]<<8)|(cmd[1]<<16)|(cmd[0]<<32);
    #else   
        *addr = (cmd[2]<<0)|(cmd[1]<<8)|(cmd[0]<<16);
    #endif
        return (BYTE_NUM_3);    
    }
}

/*
 * Get spi flash lock pattern.
 * Occasionally the size does not match the LOCK_TABLE,and we will select 
 * a region that close to this size!(samller than size)!
 * But the start_addr must match the pattern!
 */
 LOCK_PATTERN_E spiflash_get_lock_pattern(uint32 start_addr, uint32 size,
                                          const struct spi_flash_lock_desc* lock_table,
                                          uint32 lock_table_size)
{
    LOCK_PATTERN_E lock_pattern=LOCK_PATTERN_MAX;
    uint32 i=0,j=0;
    uint32 lock_match_index=0xFFFF;
    uint32 delta_size=0xFFFFFFFF;
    
    for(i=0; i<lock_table_size; i++)
    {
        lock_pattern = (LOCK_PATTERN_E)(lock_table[i].lock_pattern);
    
        /* Refer to sf_lock_pattern table, compare the size with sf_lock_pattern[i].size */
        for(j=0; j<ARRAY_SIZE(sf_lock_pattern); j++)
        {
            if((lock_pattern == sf_lock_pattern[j].lock_pattern)&&
               (start_addr == sf_lock_pattern[j].start_addr) &&
               (size >= sf_lock_pattern[j].size) )
            {
                /* Record the index of lock pattern table if it's smaller than delta_size  */
                if(size-sf_lock_pattern[j].size < delta_size)
                {
                    delta_size = size-sf_lock_pattern[j].size;
                    lock_match_index=j;
                }
            }
        }
    }

    /* Invalid index, we will return LOCK_PATTERN_MAX */
    if(0xFFFF == lock_match_index)
    {
        return LOCK_PATTERN_MAX;
    }

    return (LOCK_PATTERN_E)(sf_lock_pattern[lock_match_index].lock_pattern);
}

/*
 * Change Flash controller according to flash freq configuration.
 * ClockUnit: Mhz
 */
 int spiflash_set_clock(struct spi_flash *flash, uint32 freq_mhz)
{
    /* SC6530's SFC use AHB freq, so we don't support change frequency */
    return 0;
    /* SC6531 SFC use async clock independently! */
}/*lint !e533*/

/*
 * Send Cmd with data parameter,then get the response data from the flash.
 * Note:Cmd and data with the same bit_mode and send_mode
 *      and cmd_len is not longer than 1Byte.
 *      data_len is not longer than 8Bytes.
 */
 void spiflash_cmd_read(struct spi_flash *flash, const u8 *cmd, size_t cmd_len,
        const void *data_in, size_t data_len)
{
    SFC_CMD_DES_T cmd_desc[4];
    BIT_MODE_E bitmode = BIT_MODE_1;
    BYTE_NUM_E cmd_byte_num =0;
    BYTE_NUM_E data_byte_num =0;
    uint8  cmd_length=2;
    uint32* data_ptr32=(uint32*)(data_in);

    SCI_ASSERT(cmd_len <= 4);  /*assert verified*/   
    SCI_ASSERT(data_len <= 8);  /*assert verified*/  
    if(QPI_MODE == flash->work_mode)
    {
        bitmode = BIT_MODE_4;
    } 

    switch(cmd_len)
    {
        case 1: cmd_byte_num=BYTE_NUM_1; break;
        case 2: cmd_byte_num=BYTE_NUM_2; break;
        case 3: cmd_byte_num=BYTE_NUM_3; break;
        case 4: cmd_byte_num=BYTE_NUM_4; break;
        default: break;
    }
    CREATE_CMD_(cmd_desc[0], cmd[0], cmd_byte_num, CMD_MODE_WRITE, bitmode);
    
    if(data_len > 4)
    {
        switch(data_len%4)
        {
            case 1: data_byte_num=BYTE_NUM_1; break;
            case 2: data_byte_num=BYTE_NUM_2; break;
            case 3: data_byte_num=BYTE_NUM_3; break;
            case 0: data_byte_num=BYTE_NUM_4; break;
            default: break;
        }
        CREATE_CMD_(cmd_desc[1], NULL, BYTE_NUM_4, CMD_MODE_READ, bitmode);
        CREATE_CMD_(cmd_desc[2], NULL, data_byte_num, CMD_MODE_READ, bitmode);
        cmd_length=3;
    }
    else if(data_len > 0)
    {
        switch(data_len)
        {
            case 1: data_byte_num=BYTE_NUM_1; break;
            case 2: data_byte_num=BYTE_NUM_2; break;
            case 3: data_byte_num=BYTE_NUM_3; break;
            case 4: data_byte_num=BYTE_NUM_4; break;
            default: break;
        }
        CREATE_CMD_(cmd_desc[1], NULL, data_byte_num, CMD_MODE_READ, bitmode);
        cmd_length=2;    
    }
    else
    {
        cmd_length=1;
    }
    
    spiflash_read_write(flash->spi->cs, cmd_desc, cmd_length, data_ptr32);    
}

/*
 * Send Cmd with data parameter,or only send one command. 
 *      use to write data to control register. 
 * Note:Cmd and data with the same bit_mode and send_mode
 *      and cmd_len is not longer than 4Bytes.
 *      data_len is not longer than 4Bytes.
 *      little endian support only.
 */
 void spiflash_cmd_write(struct spi_flash *flash, const u8 *cmd, size_t cmd_len,
        const void *data_out, size_t data_len)
{
    SFC_CMD_DES_T cmd_desc[4];
    BIT_MODE_E bitmode = BIT_MODE_1;
    BYTE_NUM_E cmd_byte_num =0;
    BYTE_NUM_E data_byte_num =0;
    uint32 cmd_data = 0;
    uint32 i=0;
    uint8* data_ptr8= (uint8*)(data_out);
    uint8  cmd_length=2;

    SCI_ASSERT(cmd_len <= 4); /*assert verified*/     
    SCI_ASSERT(data_len <= 4); /*assert verified*/   
    for(i=0; i<data_len; i++)
    {
        cmd_data = cmd_data|((*data_ptr8)<<(i*8));
        data_ptr8++;
    }
   
    if(QPI_MODE == flash->work_mode)
    {
        bitmode = BIT_MODE_4;
    } 

    switch(cmd_len)
    {
        case 1: cmd_byte_num=BYTE_NUM_1; break;
        case 2: cmd_byte_num=BYTE_NUM_2; break;
        case 3: cmd_byte_num=BYTE_NUM_3; break;
        case 4: cmd_byte_num=BYTE_NUM_4; break;
        default: break;
    }
    switch(data_len)
    {
        case 1: data_byte_num=BYTE_NUM_1; break;
        case 2: data_byte_num=BYTE_NUM_2; break;
        case 3: data_byte_num=BYTE_NUM_3; break;
        case 4: data_byte_num=BYTE_NUM_4; break;
        default: break;
    }
    
    if(data_len)
    {
        CREATE_CMD_(cmd_desc[0], cmd[0], cmd_byte_num, CMD_MODE_WRITE, bitmode);
        CREATE_CMD_(cmd_desc[1], cmd_data, data_byte_num, CMD_MODE_WRITE, bitmode);
        cmd_length=2;
    }
    else
    {
        CREATE_CMD_(cmd_desc[0], cmd[0], cmd_byte_num, CMD_MODE_WRITE, bitmode);
        cmd_length=1;
    }
    
    spiflash_read_write(flash->spi->cs, cmd_desc, cmd_length, NULL);    
}

/*
 * Send cmd and wait the response byte,
 * wait the poll_bit of response data turn to bit_value before timeout.
 * Note:
 *     return: 0:Ok, other:Error
 */
 int spiflash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
               uint8 cmd, uint32 poll_bit, uint32 bit_value)
{
    uint32 status =0;
    uint32 timebase =0;

    timebase = get_timer();
    do{
        spiflash_cmd_read(flash, &cmd,1, &status, 1);
        if(bit_value)
        {
            if ((status & poll_bit))
                return 0;
        } else //bit_value==0
        {
            if ((status & poll_bit) == 0)
                return 0;
        }
    } while (get_timer()-timebase < timeout);

       /* Timed out */
    debug("SF: time out!\n");
    //SCI_PASSERT(0,("spiflash write enable latch timeout!"));    
    return -1;
}

/*
 * Wait WIP bit to be 0 in status register.
 * WIP will set to 1 statte when the device is executing Page Programe, 
 * Sector Erase,Block Erase,Chip Erase or Write Status Register instruction. 
 * During this time the device will ignore further instruction except for the Read
 * status register and Erase Suspend instruction.
 * When those instruction has completed, the bit will be cleared to a 0 state
 * indicating the device is ready for further instructions.
 */
 int spiflash_cmd_wait_ready(struct spi_flash *flash, unsigned long timeout)
{
#if 0
    /*WIP bit sets to 1,means the device is busy*/
    return spiflash_cmd_poll_bit(flash, timeout,CMD_READ_STATUS, STATUS_WIP, 0);
#else
    extern PUBLIC int SPIFLASH_CheckStatus(struct spi_flash *flash, uint  max_tick);
    /* SPIFLASH_CheckStatus的返回值1表示success，0表示fail
       而spiflash_cmd_wait_read的返回值0表示成功，非零表示失败类型*/
    if(SPIFLASH_CheckStatus(flash, timeout))
        return 0;
    else
        return -1;
#endif
}

 int spiflash_cmd_enter_qpi(struct spi_flash *flash, u8 cmd)
{
    int   ret = 0;

    SCI_ASSERT(QPI_MODE != flash->work_mode);   /*assert verified*/

    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);    
  
    flash->work_mode = QPI_MODE;
    return ret;
}

 int spiflash_cmd_exit_qpi(struct spi_flash *flash, u8 cmd)
{
    int   ret = 0;

    SCI_ASSERT(QPI_MODE == flash->work_mode);  /*assert verified*/
 
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);    

    flash->work_mode = SPI_MODE;
    return ret;    
}

 int spiflash_cmd_erase(struct spi_flash *flash, u8 erase_cmd, u32 offset)
{   
    uint32 erase_size;
    uint32 addr;
    int ret=0;
    SFC_CMD_DES_T cmd_desc[2];
    BIT_MODE_E bitmode = BIT_MODE_1;
    BYTE_NUM_E addr_byte_num =BYTE_NUM_3;
         
    if(QPI_MODE == flash->work_mode)
    {
        bitmode = BIT_MODE_4;
    }
        
    erase_size = flash->sector_size;
    if (offset % erase_size) {
        debug("SF: Erase offset/length not multiple of erase size\n");
        return -1;
    }

    ret = spiflash_write_enable (flash);
    if (ret)
        goto out;

    addr = offset;
#if !defined(PLATFORM_UWS6121E)
    addr_byte_num =spi_flash_addr(&addr, flash->support_4addr);     
#endif

    CREATE_CMD_(cmd_desc[0], erase_cmd, BYTE_NUM_1, CMD_MODE_WRITE, bitmode);
    CREATE_CMD_(cmd_desc[1], addr, addr_byte_num, CMD_MODE_WRITE, bitmode);
    spiflash_read_write(flash->spi->cs, cmd_desc, 2, SCI_NULL);       


    ret = spiflash_cmd_wait_ready(flash, SPI_FLASH_CHIP_ERASE_TIMEOUT);
    if (ret)
        goto out;

    debug("SF: Successfully erased  @%x \n", addr);

 out:
    spi_release_bus(flash->spi);
    return ret;

}

/*
 * Set read operation according to the cmd_read with dummy_bytes and spi_rw_mode
 * for specified flash, then AHB will use those setting to access spi flash.
 */
 /*lint -save -e533 */
 int spiflash_cmd_spi_read(struct spi_flash *flash, u8 cmd_read, u8 dummy_bytes)
{
    SFC_CMD_DES_T cmd_desc[5];
    uint32 cmd_desc_size=0;
    uint32 dummy[2] = {0};      

    BIT_MODE_E bitmode_cmd=BIT_MODE_1;
    BIT_MODE_E bitmode_addr=BIT_MODE_1;
    BIT_MODE_E bitmode_dummy=BIT_MODE_1;
    BIT_MODE_E bitmode_data=BIT_MODE_1;
     
    SCI_ASSERT(dummy_bytes <= 8); /*assert verified*/
    SCI_ASSERT(flash->work_mode == SPI_MODE); /*assert verified*/

    if(RD_CMD_4BIT == (flash->spi_rw_mode&RD_CMD_MSK)) {
        bitmode_cmd = BIT_MODE_4;
    } else if(RD_CMD_2BIT == (flash->spi_rw_mode&RD_CMD_MSK)) {
        bitmode_cmd = BIT_MODE_2;
    } else {
        bitmode_cmd = BIT_MODE_1;
    }

    if(RD_ADDR_4BIT == (flash->spi_rw_mode&RD_ADDR_MSK)) {
        bitmode_addr = BIT_MODE_4;
    } else if(RD_ADDR_2BIT == (flash->spi_rw_mode&RD_ADDR_MSK)) {
        bitmode_addr = BIT_MODE_2;
    } else {
        bitmode_addr = BIT_MODE_1;
    }        

    if(RD_DUMY_4BIT == (flash->spi_rw_mode&RD_DUMY_MSK)) {
        bitmode_dummy = BIT_MODE_4;
    } else if(RD_DUMY_2BIT == (flash->spi_rw_mode&RD_DUMY_MSK)) {
        bitmode_dummy = BIT_MODE_2;
    } else {
        bitmode_dummy = BIT_MODE_1;
    }   

    if(RD_DATA_4BIT == (flash->spi_rw_mode&RD_DATA_MSK)) {
        bitmode_data = BIT_MODE_4;
    } else if(RD_DATA_2BIT == (flash->spi_rw_mode&RD_DATA_MSK)) {
        bitmode_data = BIT_MODE_2;
    } else {
        bitmode_data = BIT_MODE_1;
    }   

    CREATE_CMD_(cmd_desc[0], cmd_read, BYTE_NUM_1, CMD_MODE_WRITE, bitmode_cmd);
    /* 24bits/32bits address support */
    if(SCI_TRUE == flash->support_4addr) {
        CREATE_CMD_(cmd_desc[1], NULL, BYTE_NUM_4, CMD_MODE_WRITE, bitmode_addr);
    } else {
        CREATE_CMD_(cmd_desc[1], NULL, BYTE_NUM_3, CMD_MODE_WRITE, bitmode_addr);
    }

    if(dummy_bytes > 4) {
        dummy[0] = BYTE_NUM_4;
        switch(dummy_bytes%4)
        {
            case 1: dummy[1] = BYTE_NUM_1; break;
            case 2: dummy[1] = BYTE_NUM_2; break;
            case 3: dummy[1] = BYTE_NUM_3; break;
            case 0: dummy[1] = BYTE_NUM_4; break;
            default: break;
        }
        
        CREATE_CMD_(cmd_desc[2], NULL, dummy[0], CMD_MODE_WRITE, bitmode_dummy);
        CREATE_CMD_(cmd_desc[3], NULL, dummy[1], CMD_MODE_WRITE, bitmode_dummy);
        cmd_desc_size =4;
    } else if (dummy_bytes > 0) {
        switch(dummy_bytes)
        {
            case 1: dummy[0] = BYTE_NUM_1; break;
            case 2: dummy[0] = BYTE_NUM_2; break;
            case 3: dummy[0] = BYTE_NUM_3; break;
            case 4: dummy[0] = BYTE_NUM_4; break;
            default: break;
        }
        
        CREATE_CMD_(cmd_desc[2], NULL, dummy[0], CMD_MODE_WRITE, bitmode_dummy);        
        cmd_desc_size =3;       
    } else {
        cmd_desc_size =2;       
    }
    
    spiflash_set_xip(flash->spi->cs, cmd_desc, cmd_desc_size, bitmode_data);
    return 0;
}

/*
 * Set read operation according to the cmd_read with dummy_bytes in QPI_MODE.
 */
 int spiflash_cmd_qpi_read(struct spi_flash *flash, u8 cmd_read, u8 dummy_bytes)
{
    SFC_CMD_DES_T cmd_desc[5];
    BIT_MODE_E bitmode = BIT_MODE_4;  
    uint32 cmd_desc_size=0;
    uint32 dummy[2] = {0};      

    SCI_ASSERT(dummy_bytes <= 8); /*assert verified*/
    SCI_ASSERT(flash->work_mode == QPI_MODE); /*assert verified*/

    CREATE_CMD_(cmd_desc[0], cmd_read, BYTE_NUM_1, CMD_MODE_WRITE, bitmode);
    /* 24bits/32bits address support */
    if(SCI_TRUE == flash->support_4addr) {
        CREATE_CMD_(cmd_desc[1], NULL, BYTE_NUM_4, CMD_MODE_WRITE, bitmode);
    } else {
        CREATE_CMD_(cmd_desc[1], NULL, BYTE_NUM_3, CMD_MODE_WRITE, bitmode);
    }

    if(dummy_bytes > 4) {
        dummy[0] = BYTE_NUM_4;
        switch(dummy_bytes%4)
        {
            case 1: dummy[1] = BYTE_NUM_1; break;
            case 2: dummy[1] = BYTE_NUM_2; break;
            case 3: dummy[1] = BYTE_NUM_3; break;
            case 0: dummy[1] = BYTE_NUM_4; break;
            default: break;
        }
        
        CREATE_CMD_(cmd_desc[2], NULL, dummy[0], CMD_MODE_WRITE, bitmode);
        CREATE_CMD_(cmd_desc[3], NULL, dummy[1], CMD_MODE_WRITE, bitmode);
        cmd_desc_size =4;
    } else if (dummy_bytes > 0) {
        switch(dummy_bytes)
        {
            case 1: dummy[0] = BYTE_NUM_1; break;
            case 2: dummy[0] = BYTE_NUM_2; break;
            case 3: dummy[0] = BYTE_NUM_3; break;
            case 4: dummy[0] = BYTE_NUM_4; break;
            default: break;
        }
        
        CREATE_CMD_(cmd_desc[2], NULL, dummy[0], CMD_MODE_WRITE, bitmode);      
        cmd_desc_size =3;       
    } else {
        cmd_desc_size =2;       
    }
    
    spiflash_set_xip(flash->spi->cs, cmd_desc, cmd_desc_size, bitmode);
    return 0;
}


 /*
  * Send command sequence to the device followed by (optional) data.
  * Used for programming the flash page, etc.
  *---------------------------------------------------------------------------*
  * Note: cmd_des_ptr[0].cmd has the content of cmd.
  *     : cmd_des_ptr[1].cmd has the content of address
  *     : data_len is shorter than page size.
  *---------------------------------------------------------------------------*
  */
#if !defined(PLATFORM_UWS6121E)
static int spiflash_write_page(struct spi_flash *flash, 
         SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_len, 
         const void *data, size_t data_len)
 {
     uint32 i,j;
     uint32 piece_bytes_max=(12-2)*4;
     uint32 cmd_write = cmd_des_ptr[0].cmd;
     uint32 addr      = cmd_des_ptr[1].cmd;
     uint32 dest_addr = addr;
     uint32* data_ptr32 = (uint32*)(data);
     uint32 cs =flash->spi->cs;
     int ret=0;

     BIT_MODE_E bitmode_cmd=BIT_MODE_1;
     BIT_MODE_E bitmode_addr=BIT_MODE_1;
     BIT_MODE_E bitmode_data=BIT_MODE_1;
     SFC_CMD_DES_T cmd_desc[12];
 
     SCI_ASSERT(data_len <= flash->page_size); /*assert verified*/

     /* Set cmd/addr/data bitmode according to different cmdmode */
     if(QPI_MODE == flash->work_mode)
     {
         bitmode_cmd = BIT_MODE_4;
         bitmode_addr = BIT_MODE_4;
         bitmode_data=BIT_MODE_4;
     }
     else /* SPI_MODE*/
     {
         if(WR_CMD_4BIT == (flash->spi_rw_mode&WR_CMD_MSK)) {
             bitmode_cmd = BIT_MODE_4;
         } else if(WR_CMD_2BIT == (flash->spi_rw_mode&WR_CMD_MSK)) {
             bitmode_cmd = BIT_MODE_2;
         } else {
             bitmode_cmd = BIT_MODE_1;
         }

         if(WR_ADDR_4BIT == (flash->spi_rw_mode&WR_ADDR_MSK)) {
             bitmode_addr = BIT_MODE_4;
         } else if(WR_ADDR_2BIT == (flash->spi_rw_mode&WR_ADDR_MSK)) {
             bitmode_addr = BIT_MODE_2;
         } else {
             bitmode_addr = BIT_MODE_1;
         }       

         if(WR_DATA_4BIT == (flash->spi_rw_mode&WR_DATA_MSK)) {
             bitmode_data = BIT_MODE_4;
         } else if(WR_DATA_2BIT == (flash->spi_rw_mode&WR_DATA_MSK)) {
             bitmode_data = BIT_MODE_2;
         } else {
             bitmode_data = BIT_MODE_1;
         }              
     }
 
     for(i=0; i<data_len; ) 
     {
         BYTE_NUM_E addr_byte_num =BYTE_NUM_3;
         uint32 buffer_cnt=0;
         uint32 piece_cnt = MIN(piece_bytes_max, data_len-i);
 
         ret=spiflash_write_enable (flash);
         if (ret) {
            debug("SF: enabling write failed\n");
            break;
         }
         
         /* send this piece of data to flash */
         dest_addr = addr;
         addr_byte_num =spi_flash_addr(&dest_addr, flash->support_4addr); 
         
         CREATE_CMD_(cmd_desc[buffer_cnt], cmd_write, BYTE_NUM_1, CMD_MODE_WRITE, bitmode_cmd);
         buffer_cnt++;
         CREATE_CMD_(cmd_desc[buffer_cnt], dest_addr, addr_byte_num, CMD_MODE_WRITE, bitmode_addr);
         buffer_cnt++;
         for(j=0; j<piece_cnt; ) 
         {
             /* process address with different data_length */
             if((piece_cnt-j) >=  4)
             {
                 CREATE_CMD_(cmd_desc[buffer_cnt], *data_ptr32, BYTE_NUM_4, CMD_MODE_WRITE, bitmode_data);
                 buffer_cnt++;
                 data_ptr32++;
                 j = j+4;
             } 
             /* tail_bytes send for len%4 */
             else if(((piece_cnt-j)<4)&&((piece_cnt-j)%4))   
             {
                 /* Little endian, get data byte by byte. */
                 uint32 tail_bytes=piece_cnt-j, byte_number=BYTE_NUM_1;
                 switch(tail_bytes)
                 {
                     case 1: byte_number=BYTE_NUM_1; break;
                     case 2: byte_number=BYTE_NUM_2; break;
                     case 3: byte_number=BYTE_NUM_3; break;
                     default: break;
                 }
                 CREATE_CMD_(cmd_desc[buffer_cnt], *data_ptr32, byte_number, CMD_MODE_WRITE, bitmode_data);
                 buffer_cnt++;

                 /* set j equal with piece_cnt, end the data transfer. */
                 j = piece_cnt;
             }
         }
         spiflash_read_write(cs, cmd_desc, buffer_cnt, SCI_NULL);       
         /* end of sending a piece */

         ret=spiflash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);         
         if (ret < 0) {
            debug("SF: enabling write failed\n");
            break;
         }
         i = i+piece_cnt;
         addr = addr+piece_cnt;
     }
     return ret;
 }
#else
static int spiflash_write_page(struct spi_flash *flash, 
         SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_len, 
         const void *data, size_t data_len)
 {
     uint32 i,j;
     uint32 piece_bytes_max=(12-2)*4;
     uint32 cmd_write = cmd_des_ptr[0].cmd;
     uint32 addr      = cmd_des_ptr[1].cmd;
     uint32 dest_addr = addr;
     uint32* data_ptr32 = (uint32*)(data);
     uint32 cs =flash->spi->cs;
     int ret=0;

     BIT_MODE_E bitmode_data=BIT_MODE_1;
     SFC_CMD_DES_T cmd_desc[12];
 
     SCI_ASSERT(data_len <= flash->page_size); /*assert verified*/

     /* Set cmd/addr/data bitmode according to different cmdmode */
     if(QPI_MODE == flash->work_mode)
     {
         bitmode_data=BIT_MODE_4;
     }
     else /* SPI_MODE*/
     {
         if(WR_DATA_4BIT == (flash->spi_rw_mode&WR_DATA_MSK)) {
             bitmode_data = BIT_MODE_4;
         } else if(WR_DATA_2BIT == (flash->spi_rw_mode&WR_DATA_MSK)) {
             bitmode_data = BIT_MODE_2;
         } else {
             bitmode_data = BIT_MODE_1;
         }              
     }

     CREATE_CMD_(cmd_des_ptr[2], (uint32)data, BYTE_NUM_4, CMD_MODE_WRITE, bitmode_data);
     CREATE_CMD_(cmd_des_ptr[3], (uint32)data_len, BYTE_NUM_4, CMD_MODE_WRITE, bitmode_data);
     spiflash_read_write(cs, cmd_des_ptr, 4, SCI_NULL);       

     return ret;
 }
#endif

 int spiflash_cmd_program(struct spi_flash *flash, u32 offset,
                    size_t len, const void *buf, u8 cmd)
{
    unsigned long page_addr, byte_addr, page_size;
    size_t chunk_len, actual;
#if !defined(PLATFORM_UWS6121E)
    SFC_CMD_DES_T cmd_desc[2];
#else
    SFC_CMD_DES_T cmd_desc[4];
#endif
    BIT_MODE_E bitmode = BIT_MODE_1;
    int ret=0;
    
    page_size = flash->page_size;
    page_addr = offset / page_size;
    byte_addr = offset % page_size;

    for (actual = 0; actual < len; actual += chunk_len) 
    {
        chunk_len = MIN(len - actual, page_size - byte_addr);

        ret = spiflash_write_enable (flash);
        if (ret < 0) {
            debug("SF: enabling write failed\n");
            break;
        }
        
        CREATE_CMD_(cmd_desc[0], cmd, BYTE_NUM_1, CMD_MODE_WRITE, bitmode);    
        CREATE_CMD_(cmd_desc[1], (uint32)(page_addr*page_size+byte_addr), BYTE_NUM_4, CMD_MODE_WRITE, bitmode);

        debug("PP: => cmd = { 0x%02x 0x%x } chunk_len = %d\n",
              cmd_desc[0].cmd, cmd_desc[1].cmd, chunk_len);

        ret = spiflash_write_page(flash, cmd_desc, 2,
                      ((char*)buf + actual), chunk_len);
        if (ret < 0) {
            debug("SF: write failed\n");
            break;
        }

        ret = spiflash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
        if (ret)
            break;

        page_addr++;
        byte_addr = 0;
    }

    debug("SF: program %s %zu bytes @ %#x\n",
          ret ? "failure" : "success", len, offset);

    spi_release_bus(flash->spi);
    return ret;
}


/*
 * Set Write Enable Latch (WEL) bit in the status register to a 1.
 * The WEL bit must be set prior to every Page Programe,Sector Erasee, 
 * Block Erase,Chip Erase and Write Status Register instruction.
 */
  int spiflash_write_enable (struct spi_flash *flash)
{
    uint32 timeout = SPI_FLASH_WEL_TIMEOUT;
    uint8 cmd = CMD_WRITE_ENABLE;
    
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);   

    /*Polling WEL turn to 1 */
    return spiflash_cmd_poll_bit(flash, timeout,CMD_READ_STATUS, STATUS_WEL, 1);
}

/*
 * Set Write Enable Latch (WEL) bit in the status register to a 0.
 * The WEL bit must be set prior to every Page Programe,Sector Erasee, 
 * Block Erase,Chip Erase and Write Status Register instruction.
 */
 int spiflash_write_disable(struct spi_flash *flash)
{
    SFC_CMD_DES_T cmd_desc[2];
    BIT_MODE_E bitmode = BIT_MODE_1;

    
    if(QPI_MODE == flash->work_mode)
    {
        bitmode = BIT_MODE_4;
    }

    CREATE_CMD_(cmd_desc[0], CMD_WRITE_DISABLE, BYTE_NUM_1, CMD_MODE_WRITE, bitmode);
    spiflash_read_write(flash->spi->cs, cmd_desc, 1, SCI_NULL);
    return 0;
}

/*
 * Send CMD_RSTEN&CMD_RST instruction to soft reset the flash.
 * Notes:
 *     Only it can be used if the device support this instruction.
 */
 int spiflash_reset(struct spi_flash *flash)
{
    int   ret = TRUE;
    uint8 cmd = 0;

    cmd = CMD_RSTEN;
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);  

    cmd = CMD_RST;
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);  

	FLASH_DELAY_US(30);

    return (BOOLEAN)ret;    
}

 int spiflash_reset_anyway(uint32 cs)
{
    int   ret = TRUE; 
    SFC_CMD_DES_T cmd_desc[2];
    BIT_MODE_E bitmode = BIT_MODE_1;

    /* reset in qpi mode */
    bitmode = BIT_MODE_4;
    CREATE_CMD_(cmd_desc[0], CMD_RSTEN, BYTE_NUM_1, CMD_MODE_WRITE, bitmode);
    spiflash_read_write(cs, cmd_desc, 1, NULL);   
    CREATE_CMD_(cmd_desc[0], CMD_RST, BYTE_NUM_1, CMD_MODE_WRITE, bitmode);
    spiflash_read_write(cs, cmd_desc, 1, NULL);
    
    /* reset in spi mode */
    bitmode = BIT_MODE_1;
    CREATE_CMD_(cmd_desc[0], CMD_RSTEN, BYTE_NUM_1, CMD_MODE_WRITE, bitmode);
    spiflash_read_write(cs, cmd_desc, 1, NULL);   
    CREATE_CMD_(cmd_desc[0], CMD_RST, BYTE_NUM_1, CMD_MODE_WRITE, bitmode);
    spiflash_read_write(cs, cmd_desc, 1, NULL); 

	FLASH_DELAY_US(30);

    return (BOOLEAN)ret;    
}

 int spiflash_enter_read(struct spi_flash *flash)
{ 
    u8 cmd = CMD_NORMAL_READ;
    flash->spi_rw_mode = RD_CMD_1BIT|RD_ADDR_1BIT|RD_DUMY_1BIT|RD_DATA_1BIT;
    return spiflash_cmd_spi_read(flash, cmd,  0);    
}

 int spiflash_write(struct spi_flash *flash, u32 offset,
                    size_t len, const void *buf)
{
    int ret=0;
    ret=spiflash_cmd_program(flash, offset, len, buf, CMD_PAGE_PROGRAM);
    return ret;
}

 int spiflash_erase(struct spi_flash *flash, u32 offset, size_t len)
{
    uint32 i, sectors_nr;
    int ret=0;

    sectors_nr = len/flash->sector_size;
    for(i=0; i<sectors_nr; i++) {
        ret=spiflash_cmd_erase(flash, CMD_SECTOR_ERASE, offset);
        if(ret){
            break;
        }
        offset += flash->sector_size;
    }
    return ret;
}
 
/*
 * The Erase/Program suspend will allow system to interrupt a Secotr or Block Erase
 * operation or a Page Program operation and then read/erase/program data later.
 * During suspend, read instruction and RESUME can be accepted.
 */
 int spiflash_suspend(struct spi_flash *flash)
{ 
    uint8 cmd = CMD_PE_SUSPEND;
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0); 
    return 0;
}
/*
 * The Erase/Program resume instruction can resume the Sector or Block Erase operation
 * or the Page Program operation after an Erase/Program Suspend operation.
 */
 int spiflash_resume(struct spi_flash *flash)
{  
    uint8 cmd = CMD_PE_RESUME;
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);
    return 0;
}

 /*
  * Erase all memory in the chip to the erased state as 0xFF.
  */
 int spiflash_erase_chip(struct spi_flash *flash)
{
    int   ret = TRUE;
    uint8 cmd = CMD_CHIP_ERASE;
 
    spiflash_write_enable (flash);   
 
    spiflash_cmd_write(flash, &cmd, 1, NULL, 0);
       
    spiflash_cmd_wait_ready(flash, SPI_FLASH_CHIP_ERASE_TIMEOUT); 
 
    return (BOOLEAN)ret;
}

 int spiflash_read_common(struct spi_flash *flash, u32 offset,
        size_t len, void *buf)
{
    uint32 i=0;
    uint8* din_ptr8 =(uint8*)(offset);
    uint8* dout_ptr8=(uint8*)(buf);
    
    for(i=0; i<len; i++)
    {
        *dout_ptr8 = *din_ptr8;
        dout_ptr8++;
        din_ptr8++;
    }
    return 0;
}
/*lint -restore*/
/*
 * Init the struct flash to default operation in the second level.
*/
 void spi_flash_common_operation(struct spi_flash *flash)
{
    SCI_ASSERT(flash != NULL); /*assert verified*/
/*lint -esym(613, flash) */
    if(flash->work_mode != QPI_MODE)  flash->work_mode = SPI_MODE; 

    if(NULL == flash->read) flash->read=spiflash_read_common;
    if(NULL == flash->write)flash->write=spiflash_write;
    if(NULL == flash->erase)SCI_ASSERT(0);
    if(NULL == flash->erase_chip)flash->erase_chip = spiflash_erase_chip;  

    if(NULL == flash->reset)  flash->reset = spiflash_reset;
    if(NULL == flash->suspend)flash->suspend= spiflash_suspend;
    if(NULL == flash->resume) flash->resume = spiflash_resume;

    if(NULL == flash->lock)       flash->lock= NULL;  
    if(NULL == flash->unlock)     flash->unlock= NULL;   
    if(NULL == flash->enter_4read)flash->enter_4read= NULL;  
    if(NULL == flash->enter_read) flash->enter_read= spiflash_enter_read;

    debug("SF: Init spi_flash object with common operation.\n");
}

/*
 * JEDEC devices should be ordered in the table such that
 * the probe functions with best detection algorithms come first.
 *
 * Several matching entries are permitted, they will be tried
 * in sequence until a probe function returns non NULL.
 *
 * Probe functions will be given the idcode buffer starting at their
 * manu id byte (the "idcode" in the table below).  In other words,
 * all of the continuation bytes will be skipped (the "shift" below).
 */
struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
        unsigned int max_hz, unsigned int spi_mode)
{
    SFC_CMD_DES_T cmd_desc[2];
    struct spi_slave *spi;
    struct spi_flash *flash = NULL;
    uint8 idcode[5];
    int ret;

    spi = spi_setup_slave(bus, cs, max_hz, spi_mode);
    if (!spi) {
        debug("SF: Failed to set up slave\n");
        return NULL;
    }

    ret = spi_claim_bus(spi);
    if (ret) {
        debug("SF: Failed to claim SPI bus: %d\n", ret);
        goto err_claim_bus;
    }

    /* Read the ID codes */
    CREATE_CMD_(cmd_desc[0], CMD_READ_ID, BYTE_NUM_1, CMD_MODE_WRITE, BIT_MODE_1);
    CREATE_CMD_(cmd_desc[1], SCI_NULL, BYTE_NUM_3, CMD_MODE_READ, BIT_MODE_1);
    spiflash_read_write(cs, cmd_desc, 2, idcode);/*lint !e64*/

    debug("SF: Got idcode %02x %02x %02x %02x %02x\n", idcode[0],
            idcode[1], idcode[2], idcode[3], idcode[4]);

    switch (idcode[0]) {
#if defined (CONFIG_SPI_FLASH_ATMEL) || !defined (CONFIG_SPI_FLASH)
    case 0x1F:
        //flash = spi_flash_probe_atmel(spi, idcode);
        break;
#endif
#if defined (CONFIG_SPI_FLASH_MACRONIX) || !defined (CONFIG_SPI_FLASH)
    case 0xC2:
        flash = spi_flash_probe_macronix(spi, idcode);
        break;
#endif
#if defined (CONFIG_SPI_FLASH_WINBOND) || !defined (CONFIG_SPI_FLASH)
    case 0xef:
        flash = spi_flash_probe_winbond(spi, idcode);
        break;
#endif
    case 0x20:
    case 0xff: /* Let the stmicro func handle non-JEDEC ids */
        //flash = spi_flash_probe_stmicro(spi, idcode);
        break;
#if defined (CONFIG_SPI_FLASH_GIGA) || !defined (CONFIG_SPI_FLASH)
    case 0xC8:
        flash = spi_flash_probe_giga(spi, idcode);
        break;
#endif
#if defined (CONFIG_SPI_FLASH_FIDELIX) || !defined (CONFIG_SPI_FLASH)
    case 0xF8:
    	if((0x42 == idcode[1] && 0x18 == idcode[2]) || (0x43 == idcode[1] && 0x17 == idcode[2]))
    	{
    		flash = spi_flash_probe_fm25m(spi, idcode);
    	}
        break;
#endif
#if defined (CONFIG_SPI_FLASH_BERG) || !defined (CONFIG_SPI_FLASH)
case 0xe0:
	flash = spi_flash_probe_berg(spi, idcode);
	break;
#endif		
    default:
        debug("SF: Unsupported manufacturer %02X\n", idcode[0]);
        flash = NULL;
        break;
    }

    if (!flash)
        goto err_manufacturer_probe;

    spi_release_bus(spi);

    return flash;

err_manufacturer_probe:
    spi_release_bus(spi);
err_claim_bus:
    spi_free_slave(spi);
    return NULL;
}

