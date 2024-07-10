/*
 * Copyright (C) 2007 Spreadtrum Corporation
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
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
#include "os_api.h"
#include "arm_reg.h"
#include "spi.h"
#include "sfc_drvapi.h"
//#include "malloc.h"

/*lint -emacro(413,container_of)*/
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

struct sprd_spi_slave {
    struct spi_slave slave;
    
    BIT_MODE_E  bit_mode;   //SPI/DPI/QPI
    SEND_MODE_E send_mode;  //0--byte 0123 order; 1--byte 3210 order
};

static __inline struct sprd_spi_slave *to_sprd_spi(struct spi_slave *slave)
{
    return container_of(slave, struct sprd_spi_slave, slave);
}

void spi_init(void)
{

}

/*
    Define dummy funtion to avoid compile error.
*/
/* ----------------------------------------------------------------------- */
void spi_free_slave(struct spi_slave *slave)
{
    //SCI_FREE(slave);
    return;
}

int spi_claim_bus(struct spi_slave *slave)
{
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
	if( (!slave) || (slave->cs > 1))
#else
#if defined(CHIP_VER_UWS6121E)
	if( (!slave) || (slave->cs > 1))
#else
    if( (!slave) || (slave->cs > 0))	//only one cs
#endif
#endif
        return -1;
    else
        return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
}
/* ----------------------------------------------------------------------- */
struct sprd_spi_slave   sprd_sfc[2];
struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
            unsigned int max_hz, unsigned int mode)
{
    sprd_sfc[cs].slave.bus = bus;
    sprd_sfc[cs].slave.cs = cs;

    // Use mode to set bit_mode/send_mode
    sprd_sfc[cs].bit_mode = 0;   //spi
    sprd_sfc[cs].send_mode= 0;   //
    return &sprd_sfc[cs].slave;
}


/*----------------------------------------------------------------------------
 * SPI transfer
 *
 * This writes "bitlen" bits out the SPI MOSI port and simultaneously clocks
 * "bitlen" bits in the SPI MISO port.  That's just the way SPI works.
 *
 * But
 * SFC can only work in dout or din at different time and 
   #cs is not controlled dependently, see sfc_drv.c for detail.
 * Send bitlen bits of *dout in SDO.
 * Get bitlen bits in SDI and stroe to *din.
 * With flags 
 *----------------------------------------------------------------------------*/
PUBLIC void SFCDRV_GetReadBuf(int cs,uint32* buffer, uint32 buf_cnt);
PUBLIC void SFCDRV_SetReadBuf(int cs, SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_flag);
PUBLIC void SFCDRV_SetCmdData(int cs, SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_flag);
PUBLIC void SFCDRV_ResetAllBuf(uint32 cs);
PUBLIC void SFCDRV_Req(uint32 cs, uint32 time_out);

/*
 * Send a multi-byte command to the device and read the response. 
 * Example:
 */
PUBLIC void spiflash_read_write(int cs, SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_len, uint32* din)
{
    uint32 i;
    uint32* read_ptr = din;


    SCI_ASSERT(cmd_len <= 12);/*assert verified*/  
    SFCDRV_CSCfg(cs);
    SFCDRV_ResetAllBuf(cs);

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, SCI_NULL, INI_CMD_BUF_7);
    for(i=0; i<cmd_len; i++)
    {
        cmd_des_ptr[i].is_valid = SCI_TRUE;
        cmd_des_ptr[i].send_mode= SEND_MODE_0;
        if(cmd_des_ptr[i].cmd_mode == CMD_MODE_WRITE)
            SFCDRV_SetCmdData(cs, &(cmd_des_ptr[i]), SCI_NULL);
        else if(cmd_des_ptr[i].cmd_mode == CMD_MODE_READ)
            SFCDRV_SetReadBuf(cs, &(cmd_des_ptr[i]), SCI_NULL);
        else
            SCI_ASSERT(0);/*assert verified*/
    }
    SFCDRV_Req(cs, SCI_NULL);

    /* Get the response data */
    for(i=0; i<cmd_len; i++)
    {
       if(cmd_des_ptr[i].cmd_mode == CMD_MODE_READ)
       {
           SFCDRV_GetReadBuf(cs, read_ptr++, 1);
       }
    }
}


/*
 * Xip support, set the read cmd for this feature.
 * Example:
 * Note: You must set address send_mode as SEND_MODE_1, byte3-2-1-0 as chip specified.
 *       ------------Attention! ----------Attention!!------------------------------//
 */
PUBLIC void spiflash_set_xip(int cs, SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_len, BIT_MODE_E bit_mode)
{
    uint32 i;
        
    SFCDRV_CSCfg(cs);        
    SFCDRV_ResetAllBuf(cs);

    /* set read command and read bit_mode */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_READ, bit_mode, INI_CMD_BUF_7);
    for(i=0; i<cmd_len; i++)
    {
        cmd_des_ptr[i].is_valid = SCI_TRUE;
       
        if(i==1)
            cmd_des_ptr[i].send_mode= SEND_MODE_1;
        else
            cmd_des_ptr[i].send_mode= SEND_MODE_0;

        SFCDRV_SetCmdData(cs, &(cmd_des_ptr[i]), SCI_NULL);
        
    }

    //Execute read data from ahb bus
}


