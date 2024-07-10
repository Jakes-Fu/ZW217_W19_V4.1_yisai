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

#if defined(CHIP_VER_UWS6121E)
HWP_SPI_FLASH_T *hwp_spiFlash_cs[SFC_MAX_CS];
#endif

#pragma arm section code = "SFC_DRV_CODE" 

#define SFC_PRINT(x)
#define SFC_CMDBUFF_MUX     11
LOCAL SFC_REG_T reg_sfc_soft;
LOCAL SFC_REG_T *reg_sfc = (SFC_REG_T *) ((uint32)&reg_sfc_soft) ;
uint8 g_sfc_cmd_rec[SFC_CMDBUFF_MUX];
uint8 g_sfc_cmd_index = 0;

extern void *_memcpy(void * dst, const void *src, uint32 count);//defined in MS_REF
#ifdef SFC_DEBUG
uint32 s_sfc_save_cmd0_index = 0;
uint16 s_sfc_save_cmd0[SFC_DEBUG_BUF_LEN] = {0};
#endif

PUBLIC uint32    cmd_buf_index =0;
PUBLIC uint32    read_buf_index =7;

#if defined(CHIP_VER_UWS6121E)
PUBLIC void wait_flash_busy(int cs)
{    
	while (hwp_spiFlash_cs[cs]->spi_status & SPI_FLASH_SPI_FLASH_BUSY)    
	{        
	}    
	return ;
}

void start_flash_operation(int cs, uint8 cmd, uint32 addr, uint8 opmode, uint16 blocksize)
{    
	uint32 data_tmp_32;    
	#if 0
	if((g_spiflash_ID.manufacturerID == MEMD_FLASH_EON) && (cmd == g_dif_opcode_high_p_mode) ) 
		return;   
	if((g_spiflash_ID.manufacturerID == MEMD_FLASH_MICRON) && (cmd == g_dif_opcode_high_p_mode) ) 
		return;       
	#endif
	wait_flash_busy(cs);   
	if(blocksize!=0)    
	{        
		data_tmp_32 = hwp_spiFlash_cs[cs]->spi_block_size;
		data_tmp_32 = (data_tmp_32 & (~0x1ff00)) | SPI_FLASH_SPI_RW_BLK_SIZE(blocksize);    
		hwp_spiFlash_cs[cs]->spi_block_size = data_tmp_32;
		wait_flash_busy(cs);  
	}  
	
	if (blocksize >= 3)
	{
		data_tmp_32 = hwp_spiFlash_cs[cs]->spi_config;
		data_tmp_32 |= SPI_FLASH_TX_RX_SIZE(3);
		hwp_spiFlash_cs[cs]->spi_config = data_tmp_32;
		wait_flash_busy(cs);  
	}
	
	if(opmode==0)  
	{	       
		data_tmp_32 = ((addr << SPI_FLASH_TX_ADDRESS_BIT) & 0xffffff00) | (cmd << SPI_FLASH_TX_COMMAND_BIT);  
	}   
	else if(opmode==1)  
	{       
		data_tmp_32 = (((cmd << SPI_FLASH_TX_COMMAND_BIT_SPD_MODE) |(1 << SPI_FLASH_TX_CMD_RW_FLAG_BIT)) & 0x0003ff00);  
	}  
	else if(opmode==2)  
	{     
		data_tmp_32 = (((cmd << SPI_FLASH_TX_COMMAND_BIT_SPD_MODE) |(1 << SPI_FLASH_TX_CMD_RW_FLAG_BIT) | (1 << SPI_FLASH_TX_SPI_MODE_FLAG_BIT)) & 0x0003ff00);  
	}  
	else if(opmode==3) 
	{     
		data_tmp_32 = ((cmd << SPI_FLASH_TX_COMMAND_BIT_SPD_MODE) & 0x0003ff00);    
	}   
	else    
	{      
		data_tmp_32 = ((cmd << SPI_FLASH_TX_COMMAND_BIT_SPD_MODE) & 0x0003ff00);    
	}   
	hwp_spiFlash_cs[cs]->spi_cmd_addr = data_tmp_32;
	wait_flash_busy(cs);  
	
	if (blocksize >= 3)
	{
		data_tmp_32 = hwp_spiFlash_cs[cs]->spi_config;
		data_tmp_32 &= ~(SPI_FLASH_TX_RX_SIZE(3));
		hwp_spiFlash_cs[cs]->spi_config = data_tmp_32;
	}

	return;
}

void push_fifo_data(int cs, uint8 data_array[], uint32 data_cnt, uint8 quard_flag, uint8 clr_flag)
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
#else
PUBLIC void wait_flash_busy(void)
{    
	while (hwp_spiFlash->spi_status & SPI_FLASH_SPI_FLASH_BUSY)    
	{        
	}    
	return ;
}

void start_flash_operation(uint8 cmd, uint32 addr, uint8 opmode, uint16 blocksize)
{    
	uint32 data_tmp_32;    
	#if 0
	if((g_spiflash_ID.manufacturerID == MEMD_FLASH_EON) && (cmd == g_dif_opcode_high_p_mode) ) 
		return;   
	if((g_spiflash_ID.manufacturerID == MEMD_FLASH_MICRON) && (cmd == g_dif_opcode_high_p_mode) ) 
		return;       
	#endif
	wait_flash_busy();   
	if(blocksize!=0)    
	{        
		data_tmp_32 = hwp_spiFlash->spi_block_size;
		data_tmp_32 = (data_tmp_32 & (~0x1ff00)) | SPI_FLASH_SPI_RW_BLK_SIZE(blocksize);    
		hwp_spiFlash->spi_block_size = data_tmp_32;
		wait_flash_busy();  
	}  
	
	if (blocksize >= 3)
	{
		data_tmp_32 = hwp_spiFlash->spi_config;
		data_tmp_32 |= SPI_FLASH_TX_RX_SIZE(3);
		hwp_spiFlash->spi_config = data_tmp_32;
		wait_flash_busy();  
	}
	
	if(opmode==0)  
	{	       
		data_tmp_32 = ((addr << SPI_FLASH_TX_ADDRESS_BIT) & 0xffffff00) | (cmd << SPI_FLASH_TX_COMMAND_BIT);  
	}   
	else if(opmode==1)  
	{       
		data_tmp_32 = (((cmd << SPI_FLASH_TX_COMMAND_BIT_SPD_MODE) |(1 << SPI_FLASH_TX_CMD_RW_FLAG_BIT)) & 0x0003ff00);  
	}  
	else if(opmode==2)  
	{     
		data_tmp_32 = (((cmd << SPI_FLASH_TX_COMMAND_BIT_SPD_MODE) |(1 << SPI_FLASH_TX_CMD_RW_FLAG_BIT) | (1 << SPI_FLASH_TX_SPI_MODE_FLAG_BIT)) & 0x0003ff00);  
	}  
	else if(opmode==3) 
	{     
		data_tmp_32 = ((cmd << SPI_FLASH_TX_COMMAND_BIT_SPD_MODE) & 0x0003ff00);    
	}   
	else    
	{      
		data_tmp_32 = ((cmd << SPI_FLASH_TX_COMMAND_BIT_SPD_MODE) & 0x0003ff00);    
	}   
	hwp_spiFlash->spi_cmd_addr = data_tmp_32;
	wait_flash_busy();  
	
	if (blocksize >= 3)
	{
		data_tmp_32 = hwp_spiFlash->spi_config;
		data_tmp_32 &= ~(SPI_FLASH_TX_RX_SIZE(3));
		hwp_spiFlash->spi_config = data_tmp_32;
	}

	return;
}

void push_fifo_data(uint8 data_array[], uint32 data_cnt, uint8 quard_flag, uint8 clr_flag)
{   
	uint32 data_tmp_32 = 0;    
	uint32 addr_tmp;  
	uint32 i=0;   
	if(clr_flag)  
	{         
		hwp_spiFlash->spi_fifo_control = SPI_FLASH_TX_FIFO_CLR;
		while((hwp_spiFlash->spi_status & SPI_FLASH_TX_FIFO_EMPTY) == 0);
	}   
	while (hwp_spiFlash->spi_status & SPI_FLASH_TX_FIFO_FULL);     

	for(i = 0; i < data_cnt; i++)   
	{       
		data_tmp_32 = (uint32)data_array[i];       
		if(quard_flag)       
			data_tmp_32 = data_tmp_32 | SPI_FLASH_SPI_SEND_TYPE;
		hwp_spiFlash->spi_data_fifo = data_tmp_32;
       }    
}
#endif

/*****************************************************************************/
//  Description:  set cmd_cfg register
//  Author: Fei.zhang
//  Param:
//      cmdmode: read or write operation
//      bitmode: AHB read data bit mode, 1bit ,2bit and 4bit canbe used
//      iniAddSel: Read back status information initial address selection
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetCMDCfgReg (int cs, CMD_MODE_E cmdmode, BIT_MODE_E bitmode, INI_ADD_SEL_E iniAddSel)
{
    reg_sfc->cmd_cfg.mBits.cmd_set = cmdmode;
    
      // if(cs == 0)
    {
        reg_sfc->cmd_cfg.mBits.rdata_bit_mode = bitmode;
        reg_sfc->cmd_cfg.mBits.sts_ini_addr_sel = iniAddSel;
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
#if defined(CHIP_VER_UWS6121E)
PUBLIC void SFCDRV_SoftReq(int cs)
{
	uint8 idx;
	uint8 cmd = 0;
	uint32 addr;
	uint8 opmode;
	uint16 blocksize = 0;
	uint8 cmd_resp_buf;
	uint32 *type_info, *read_cmd_Buf;
	uint8 buff_type_info;
	
	uint8 *data_buff = NULL;
	uint32  data_cnt;
	uint8 quard_flag;

	cmd_resp_buf = reg_sfc->cmd_cfg.mBits.sts_ini_addr_sel;
	cmd_resp_buf = 7 - cmd_resp_buf;
    
	type_info = (uint32 *)(&reg_sfc->type_buf0);
       if (cmd_resp_buf > 7)
       {
           SCI_PASSERT(0, ("sfc:cmd_resp_buf is error"));/*assert verified*/
       }
       else if (cmd_resp_buf > 3)
       {
	    type_info = (uint32 *)(&reg_sfc->type_buf1);
       }
       buff_type_info = *type_info >> ((cmd_resp_buf % 4) * 8);
       
       switch(cmd_resp_buf)
       {
            case CMD_BUF_0:
	         read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf0);
                break;

            case CMD_BUF_1:
	         read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf1);
                break;

            case CMD_BUF_2:
                read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf2);
                break;

            case CMD_BUF_3:
                read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf3);
                break;
                
            case CMD_BUF_4:
                read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf4);
                break;

            case CMD_BUF_5:
                read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf5);
                break;

            case CMD_BUF_6:
                read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf6);
                break;

            case CMD_BUF_7:
                read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf7);
                break;          

            default:
                read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf7);
                break;                  
       }
	
	addr = 0x0;
	for (idx = 0; idx < g_sfc_cmd_index; idx++)
	{	
		switch(g_sfc_cmd_rec[idx])
		{
			case CMD_BUF_0:
			{
				cmd = reg_sfc->cmd_buf0.dwValue;
				break;
			}
			case CMD_BUF_1:
			{
				if ((cmd == CMD_SECTOR_ERASE) || (cmd == CMD_GIGA_BE32K) || (cmd == CMD_GIGA_BE64K)  || (cmd == CMD_PAGE_PROGRAM) || (cmd == CMD_GIGA_QUAD_PP) )
				{
					addr = reg_sfc->cmd_buf1.dwValue;
				}
				else
				{
					data_buff = (uint8 *)&reg_sfc->cmd_buf1.dwValue;
					data_cnt = reg_sfc->type_buf0.mBits.byte_num1 + 1;
					//quard_flag = (BIT_MODE_4 == reg_sfc->type_buf0.mBits.bit_mode1) ? (1) : (0);
					quard_flag = 0;
					push_fifo_data(cs, data_buff, data_cnt, quard_flag, 1);
				}
				break;
			}
			case CMD_BUF_2:
			{
				data_buff = (uint8 *)(reg_sfc->cmd_buf2.dwValue);
				break;
			}
			case CMD_BUF_3:
			{
				data_cnt = reg_sfc->cmd_buf3.dwValue;
				//quard_flag = (BIT_MODE_4 == reg_sfc->type_buf0.mBits.bit_mode3) ? (1) : (0);
				quard_flag = 0;
                            if (data_buff == NULL)
                            {
                                SCI_PASSERT(0, ("sfc:buff is null"));/*assert verified*/
                            }
				push_fifo_data(cs, data_buff, data_cnt, quard_flag, 1);
				break;
			}
			default:
               		break;                  
		}
	}
	if (buff_type_info & BIT_0)
	{
		blocksize = ((buff_type_info & 0x18) >> 3) + 1;
	}
	//opmode = buff_type_info & 0x06;
	if ((cmd == CMD_SECTOR_ERASE) || (cmd == CMD_GIGA_BE32K) || (cmd == CMD_GIGA_BE64K)  || (cmd == CMD_PAGE_PROGRAM) || (cmd == CMD_GIGA_QUAD_PP) )
	{
        blocksize = 1;
    }
    
      opmode = 0;
        if (cmd == 0)
        {
            SCI_PASSERT(0, ("sfc:cmd is 0"));/*assert verified*/
        }
        else
        {
	    start_flash_operation(cs, cmd, addr, opmode, blocksize);
        }
	wait_flash_busy(cs);

	*read_cmd_Buf = hwp_spiFlash_cs[cs]->rx_status;
	if (cmd == CMD_WRITE_STATUS)
	{
                if (data_buff == NULL)
                {
                    SCI_PASSERT(0, ("sfc:buff is null"));/*assert verified*/
                }
                
		if (data_buff[1] & 0x02)
		{
			hwp_spiFlash_cs[cs]->spi_config |= SPI_FLASH_QUAD_MODE;
		}
		else
		{
			hwp_spiFlash_cs[cs]->spi_config &= ~SPI_FLASH_QUAD_MODE;
		}
	}
	g_sfc_cmd_index = 0;
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
    //return reg_sfc->status.dwValue;
	if (hwp_spiFlash_cs[cs]->spi_status & SPI_FLASH_SPI_FLASH_BUSY)
	{
		return SFC_INT_STATUS;
	}
	else
	{
		return SFC_IDLE_STATUS;
	}
}

PUBLIC void SFCDRV_WaitCmdDone(int cs)
{
    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs))) {}
}

#else
PUBLIC void SFCDRV_SoftReq(void)
{
#if 0
    reg_sfc->soft_req.mBits.soft_req = BIT_0;
#endif
	uint8 idx;
	uint8 cmd;
	uint32 addr;
	uint8 opmode;
	uint16 blocksize = 0;
	uint8 cmd_resp_buf;
	uint32 *type_info, *read_cmd_Buf;
	uint8 buff_type_info;
	
	uint8 *data_buff;
	uint32  data_cnt;
	uint8 quard_flag;
	
	cmd_resp_buf = reg_sfc->cmd_cfg.mBits.sts_ini_addr_sel;
	cmd_resp_buf = 7 - cmd_resp_buf;
	type_info = (uint32 *)(&reg_sfc->type_buf0);
	type_info += (cmd_resp_buf / 4);
	buff_type_info = *type_info >> ((cmd_resp_buf % 4) * 8);
	read_cmd_Buf = (uint32 *)(&reg_sfc->cmd_buf0);
	read_cmd_Buf += cmd_resp_buf;
	
	addr = 0x0;
	for (idx = 0; idx < g_sfc_cmd_index; idx++)
	{	
		switch(g_sfc_cmd_rec[idx])
		{
			case CMD_BUF_0:
			{
				cmd = reg_sfc->cmd_buf0.dwValue;
				break;
			}
			case CMD_BUF_1:
			{
				if ((cmd == CMD_SECTOR_ERASE) || (cmd == CMD_GIGA_BE32K) || (cmd == CMD_GIGA_BE64K)  || (cmd == CMD_PAGE_PROGRAM) || (cmd == CMD_GIGA_QUAD_PP) )
				{
					addr = reg_sfc->cmd_buf1.dwValue;
				}
				else
				{
					data_buff = (uint8 *)&reg_sfc->cmd_buf1.dwValue;
					data_cnt = reg_sfc->type_buf0.mBits.byte_num1 + 1;
					//quard_flag = (BIT_MODE_4 == reg_sfc->type_buf0.mBits.bit_mode1) ? (1) : (0);
					quard_flag = 0;
					push_fifo_data(data_buff, data_cnt, quard_flag, 1);
				}
				break;
			}
			case CMD_BUF_2:
			{
				data_buff = (uint8 *)(reg_sfc->cmd_buf2.dwValue);
				break;
			}
			case CMD_BUF_3:
			{
				data_cnt = reg_sfc->cmd_buf3.dwValue;
				//quard_flag = (BIT_MODE_4 == reg_sfc->type_buf0.mBits.bit_mode3) ? (1) : (0);
				quard_flag = 0;
				push_fifo_data(data_buff, data_cnt, quard_flag, 1);
				break;
			}
			default:
               		break;                  
		}
	}
	if (buff_type_info & BIT_0)
	{
		blocksize = ((buff_type_info & 0x18) >> 3) + 1;
	}
	//opmode = buff_type_info & 0x06;
	if ((cmd == CMD_SECTOR_ERASE) || (cmd == CMD_GIGA_BE32K) || (cmd == CMD_GIGA_BE64K)  || (cmd == CMD_PAGE_PROGRAM) || (cmd == CMD_GIGA_QUAD_PP) )
	{
        blocksize = 1;
    }
      opmode = 0;
	start_flash_operation(cmd, addr, opmode, blocksize);
	wait_flash_busy();

	*read_cmd_Buf = hwp_spiFlash->rx_status;
	if (cmd == CMD_WRITE_STATUS)
	{
		if (data_buff[1] & 0x02)
		{
			hwp_spiFlash->spi_config |= SPI_FLASH_QUAD_MODE;
		}
		else
		{
			hwp_spiFlash->spi_config &= ~SPI_FLASH_QUAD_MODE;
		}
	}
	g_sfc_cmd_index = 0;
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
PUBLIC uint32 SFCDRV_GetStatus(void)
{
    //return reg_sfc->status.dwValue;
	if (hwp_spiFlash->spi_status & SPI_FLASH_SPI_FLASH_BUSY)
	{
		return SFC_INT_STATUS;
	}
	else
	{
		return SFC_IDLE_STATUS;
	}
}

PUBLIC void SFCDRV_WaitCmdDone( void )
{
    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus())) {}
}
#endif

/*****************************************************************************/
//  Description:  clear all cmd buffer register
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_CMDBufClr (int cs)
{
#if defined(CHIP_VER_UWS6121E)
	hwp_spiFlash_cs[cs]->spi_fifo_control = SPI_FLASH_RX_FIFO_CLR | SPI_FLASH_TX_FIFO_CLR;
#else
#if 0
    if(cs == 0)
    {
        reg_sfc->cmd_buf0.dwValue = 0;
        reg_sfc->cmd_buf1.dwValue = 0;
        reg_sfc->cmd_buf2.dwValue = 0;
        reg_sfc->cmd_buf3.dwValue = 0;
        reg_sfc->cmd_buf4.dwValue = 0;
        reg_sfc->cmd_buf5.dwValue = 0;
        reg_sfc->cmd_buf6.dwValue = 0;
        reg_sfc->cmd_buf7.dwValue = 0;
        reg_sfc->cmd_buf8.dwValue = 0;
        reg_sfc->cmd_buf9.dwValue = 0;
        reg_sfc->cmd_buf10.dwValue = 0;
        reg_sfc->cmd_buf11.dwValue = 0;     
    }
#endif
	hwp_spiFlash->spi_fifo_control = SPI_FLASH_RX_FIFO_CLR | SPI_FLASH_TX_FIFO_CLR;
#endif
}

/*****************************************************************************/
//  Description:  clear all type buffer register
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_TypeBufClr (int cs)
{
#if defined(CHIP_VER_UWS6121E)
    hwp_spiFlash_cs[cs]->spi_fifo_control = SPI_FLASH_RX_FIFO_CLR | SPI_FLASH_TX_FIFO_CLR;
#else
#if 0
    if(cs == 0)
    {
        reg_sfc->tbuf_clr.mBits.tbuf_clr = BIT_0;
    }       
#endif
	hwp_spiFlash->spi_fifo_control = SPI_FLASH_RX_FIFO_CLR | SPI_FLASH_TX_FIFO_CLR;
#endif
}

/*****************************************************************************/
//  Description:  clear sfc interrupt request signal
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_IntClr (int cs)
{
#if 0
    if(cs == 0)
    {
        reg_sfc->int_clr.mBits.int_clr = BIT_0;
    }
#endif
}

/*****************************************************************************/
//  Description:  set cs_timing_cfg register
//  Author: Fei.zhang
//  Param:
//      value: cs timing configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_CSTimingCfg (int cs, uint32 value)
{
    //if(cs == 0)
    {
        reg_sfc->cs_timing_cfg.dwValue = value;
    }
}

/*****************************************************************************/
//  Description:  set rd_timing_cfg register
//  Author: Fei.zhang
//  Param:
//      value: rd timing configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_RDTimingCfg (int cs, uint32 value)
{
    //if(cs == 0)
    {
        reg_sfc->rd_timing_cfg.dwValue = value;
    }  
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
#if defined(CHIP_VER_UWS6121E)
    reg_sfc->clk_cfg.dwValue = value;
    hwp_spiFlash_cs[cs]->spi_config &= ~(SPI_FLASH_SAMPLE_DELAY_MASK | SPI_FLASH_CLK_DIVIDER_MASK);
    hwp_spiFlash_cs[cs]->spi_config |= value;
#else
    if(cs == 0)
    {
        reg_sfc->clk_cfg.dwValue = value;
        hwp_spiFlash->spi_config &= ~(SPI_FLASH_SAMPLE_DELAY_MASK | SPI_FLASH_CLK_DIVIDER_MASK);
        hwp_spiFlash->spi_config |= value;
    }   
#endif
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
#if defined(CHIP_VER_UWS6121E)
    if(value == 0)
    {
        hwp_spiFlash_cs[value] = ((HWP_SPI_FLASH_T*) REG_ACCESS_ADDRESS(REG_SPI_FLASH_BASE));
    }   
    else if (value == 1)
    {
        hwp_spiFlash_cs[value] = ((HWP_SPI_FLASH_T*) REG_ACCESS_ADDRESS(REG_SPI_FLASH1_BASE));
    }
#else
#if 0
    reg_sfc->cs_cfg.dwValue = value;
#endif
#endif
}

/*****************************************************************************/
//  Description:  set endian config register
//  Author: Fei.zhang
//  Param:
//      value: endian configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_EndianCfg (int cs, uint32 value)
{
   // if(cs == 0)
    {
        reg_sfc->endian_cfg.dwValue = value;
    }   
}

/*****************************************************************************/
//  Description:  set cmd buffer register
//  Author: Fei.zhang
//  Param:
//      index: cmd buffer index
//      value: cmd buffer configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetCMDBuf (int cs, CMD_BUF_INDEX_E index, uint32 value)
{
    //if(cs == 0)
    {
        g_sfc_cmd_rec[g_sfc_cmd_index++] = index;
        switch (index)
        {
            case CMD_BUF_0:
                reg_sfc->cmd_buf0.mBits.cmd_buf0 = value;
                            #ifdef SFC_DEBUG
                            s_sfc_save_cmd0[s_sfc_save_cmd0_index++] = value;
                            if(s_sfc_save_cmd0_index == SFC_DEBUG_BUF_LEN)
                            {
                                s_sfc_save_cmd0_index = 0;
                            }
                            #endif
                break;

            case CMD_BUF_1:
                reg_sfc->cmd_buf1.mBits.cmd_buf1 = value;
                break;

            case CMD_BUF_2:
                reg_sfc->cmd_buf2.mBits.cmd_buf2 = value;
                break;

            case CMD_BUF_3:
                reg_sfc->cmd_buf3.mBits.cmd_buf3 = value;
                break;
                
            case CMD_BUF_4:
                reg_sfc->cmd_buf4.mBits.cmd_buf4 = value;
                break;

            case CMD_BUF_5:
                reg_sfc->cmd_buf5.mBits.cmd_buf5 = value;
                break;

            case CMD_BUF_6:
                reg_sfc->cmd_buf6.mBits.cmd_buf6 = value;
                break;

            case CMD_BUF_7:
                reg_sfc->cmd_buf7.mBits.cmd_buf7 = value;
                break;          

            case CMD_BUF_8:
                reg_sfc->cmd_buf8.mBits.cmd_buf8 = value;
                break;

            case CMD_BUF_9:
                reg_sfc->cmd_buf9.mBits.cmd_buf9 = value;
                break;

            case CMD_BUF_10:
                reg_sfc->cmd_buf10.mBits.cmd_buf10 = value;
                break;

            case CMD_BUF_11:
                reg_sfc->cmd_buf11.mBits.cmd_buf11 = value;
                break;  
            default:
                break;                  
        }
    }
}

/*****************************************************************************/
//  Description:  set cmd buffer register
//  Author: Fei.zhang
//  Param:
//      index: cmd buffer index
//      buf: src data
//      count: bytes count
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetCMDBufEx(int cs, CMD_BUF_INDEX_E index, const uint8 *buf,  uint32 count)
{
    SFC_CMD_BUF0_U *cmd_buf;
    //SCI_ASSERT((index+ count) <= (CMD_BUF_MAX*4));    /*assert to do*/ 
    
    //if(cs == 0)
    {
    #if 0
        cmd_buf = (SFC_CMD_BUF0_U *)((uint32)&reg_sfc->cmd_buf0 + index*sizeof(SFC_CMD_BUF0_U));
        _memcpy((uint8*)&cmd_buf->dwValue, buf, count);    
     #else
        cmd_buf = (SFC_CMD_BUF0_U *)((uint32)&reg_sfc->cmd_buf0 + index*sizeof(SFC_CMD_BUF0_U));
        cmd_buf->dwValue = (uint32)buf;
        cmd_buf++;
        cmd_buf->dwValue = count;

        g_sfc_cmd_rec[g_sfc_cmd_index++] = CMD_BUF_2;
        g_sfc_cmd_rec[g_sfc_cmd_index++] = CMD_BUF_3;
    #endif     
    }

    //_memcpy((uint8*)&cmd_buf->dwValue, buf, count);
}

/*****************************************************************************/
//  Description:  get cmd buffer register
//  Author: Fei.zhang
//  Param:
//      index: cmd buffer index
//  Return:
//      cmd buffer configure value
//  Note:
/*****************************************************************************/
PUBLIC uint32 SFCDRV_GetCMDBuf (int cs, CMD_BUF_INDEX_E index)
{
    uint32 value = 0; 
    //if(cs == 0)
    {   
        switch (index)
        {
            case CMD_BUF_0:
                value = reg_sfc->cmd_buf0.mBits.cmd_buf0;
                break;

            case CMD_BUF_1:
                value = reg_sfc->cmd_buf1.mBits.cmd_buf1;
                break;

            case CMD_BUF_2:
                value = reg_sfc->cmd_buf2.mBits.cmd_buf2;
                break;

            case CMD_BUF_3:
                value = reg_sfc->cmd_buf3.mBits.cmd_buf3;
                break;
                
            case CMD_BUF_4:
                value = reg_sfc->cmd_buf4.mBits.cmd_buf4;
                break;

            case CMD_BUF_5:
                value = reg_sfc->cmd_buf5.mBits.cmd_buf5;
                break;

            case CMD_BUF_6:
                value = reg_sfc->cmd_buf6.mBits.cmd_buf6;
                break;

            case CMD_BUF_7:
                value = reg_sfc->cmd_buf7.mBits.cmd_buf7;
                break;          

            case CMD_BUF_8:
                value = reg_sfc->cmd_buf8.mBits.cmd_buf8;
                break;

            case CMD_BUF_9:
                value = reg_sfc->cmd_buf9.mBits.cmd_buf9;
                break;

            case CMD_BUF_10:
                value = reg_sfc->cmd_buf10.mBits.cmd_buf10;
                break;

            case CMD_BUF_11:
                value = reg_sfc->cmd_buf11.mBits.cmd_buf11;
                break;
            default:
                break;  
        }
    }
    
    return value;
}

/*****************************************************************************/
//  Description:  set cmd buffer register
//  Author: Fei.zhang
//  Param:
//      index: cmd buffer index
//      bitmode: bit mode
//      bytenum: byte number
//      cmdmode: read or write
//      sendmode: send mode
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetTypeInfBuf (int cs, CMD_BUF_INDEX_E index, BIT_MODE_E bitmode, BYTE_NUM_E bytenum, CMD_MODE_E cmdmode, SEND_MODE_E sendmode)
{
    //if(cs == 0)
    {
        switch (index)
        {
            case CMD_BUF_0:
                reg_sfc->type_buf0.mBits.valid0 = BIT_0;
                reg_sfc->type_buf0.mBits.bit_mode0 = bitmode;
                reg_sfc->type_buf0.mBits.byte_num0 = bytenum;
                reg_sfc->type_buf0.mBits.operation_status0 = cmdmode;
                reg_sfc->type_buf0.mBits.byte_send_mode0 = sendmode;
                break;

            case CMD_BUF_1:
                reg_sfc->type_buf0.mBits.valid1 = BIT_0;
                reg_sfc->type_buf0.mBits.bit_mode1 = bitmode;
                reg_sfc->type_buf0.mBits.byte_num1 = bytenum;
                reg_sfc->type_buf0.mBits.operation_status1 = cmdmode;
                reg_sfc->type_buf0.mBits.byte_send_mode1 = sendmode;
                break;

            case CMD_BUF_2:
                reg_sfc->type_buf0.mBits.valid2 = BIT_0;
                reg_sfc->type_buf0.mBits.bit_mode2 = bitmode;
                reg_sfc->type_buf0.mBits.byte_num2 = bytenum;
                reg_sfc->type_buf0.mBits.operation_status2 = cmdmode;
                reg_sfc->type_buf0.mBits.byte_send_mode2 = sendmode;
                break;

            case CMD_BUF_3:
                reg_sfc->type_buf0.mBits.valid3 = BIT_0;
                reg_sfc->type_buf0.mBits.bit_mode3 = bitmode;
                reg_sfc->type_buf0.mBits.byte_num3 = bytenum;
                reg_sfc->type_buf0.mBits.operation_status3 = cmdmode;
                reg_sfc->type_buf0.mBits.byte_send_mode3 = sendmode;
                break;
                
            case CMD_BUF_4:
                reg_sfc->type_buf1.mBits.valid4 = BIT_0;
                reg_sfc->type_buf1.mBits.bit_mode4 = bitmode;
                reg_sfc->type_buf1.mBits.byte_num4 = bytenum;
                reg_sfc->type_buf1.mBits.operation_status4 = cmdmode;
                reg_sfc->type_buf1.mBits.byte_send_mode4 = sendmode;
                break;

            case CMD_BUF_5:
                reg_sfc->type_buf1.mBits.valid5 = BIT_0;
                reg_sfc->type_buf1.mBits.bit_mode5 = bitmode;
                reg_sfc->type_buf1.mBits.byte_num5 = bytenum;
                reg_sfc->type_buf1.mBits.operation_status5 = cmdmode;
                reg_sfc->type_buf1.mBits.byte_send_mode5 = sendmode;
                break;

            case CMD_BUF_6:
                reg_sfc->type_buf1.mBits.valid6 = BIT_0;
                reg_sfc->type_buf1.mBits.bit_mode6 = bitmode;
                reg_sfc->type_buf1.mBits.byte_num6 = bytenum;
                reg_sfc->type_buf1.mBits.operation_status6 = cmdmode;
                reg_sfc->type_buf1.mBits.byte_send_mode6 = sendmode;
                break;

            case CMD_BUF_7:
                reg_sfc->type_buf1.mBits.valid7 = BIT_0;
                reg_sfc->type_buf1.mBits.bit_mode7 = bitmode;
                reg_sfc->type_buf1.mBits.byte_num7 = bytenum;
                reg_sfc->type_buf1.mBits.operation_status7 = cmdmode;
                reg_sfc->type_buf1.mBits.byte_send_mode7 = sendmode;
                break;          

            case CMD_BUF_8:
                reg_sfc->type_buf2.mBits.valid8 = BIT_0;
                reg_sfc->type_buf2.mBits.bit_mode8 = bitmode;
                reg_sfc->type_buf2.mBits.byte_num8 = bytenum;
                reg_sfc->type_buf2.mBits.operation_status8 = cmdmode;
                reg_sfc->type_buf2.mBits.byte_send_mode8 = sendmode;
                break;

            case CMD_BUF_9:
                reg_sfc->type_buf2.mBits.valid9 = BIT_0;
                reg_sfc->type_buf2.mBits.bit_mode9 = bitmode;
                reg_sfc->type_buf2.mBits.byte_num9 = bytenum;
                reg_sfc->type_buf2.mBits.operation_status9 = cmdmode;
                reg_sfc->type_buf2.mBits.byte_send_mode9 = sendmode;
                break;

            case CMD_BUF_10:
                reg_sfc->type_buf2.mBits.valid10 = BIT_0;
                reg_sfc->type_buf2.mBits.bit_mode10 = bitmode;
                reg_sfc->type_buf2.mBits.byte_num10 = bytenum;
                reg_sfc->type_buf2.mBits.operation_status10 = cmdmode;
                reg_sfc->type_buf2.mBits.byte_send_mode10 = sendmode;
                break;

            case CMD_BUF_11:
                reg_sfc->type_buf2.mBits.valid11 = BIT_0;
                reg_sfc->type_buf2.mBits.bit_mode11 = bitmode;
                reg_sfc->type_buf2.mBits.byte_num11 = bytenum;
                reg_sfc->type_buf2.mBits.operation_status11 = cmdmode;
                reg_sfc->type_buf2.mBits.byte_send_mode11 = sendmode;
                break;
            default:
                break;  
        }       
    }   
}

/*****************************************************************************/
//  Description:  Set/Get start addr for read back.
//  Author: lichd
//  Param:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetInitAddr(int cs, INI_ADD_SEL_E start_addr)
{
    //if(cs == 0)
    {
        reg_sfc->cmd_cfg.mBits.sts_ini_addr_sel = start_addr;
    }
}

PUBLIC uint32 SFCDRV_GetInitAddr(int cs)
{
    uint32 start_addr =0;
    //if(cs == 0)
    {
        start_addr = reg_sfc->cmd_cfg.mBits.sts_ini_addr_sel;
    }

    switch(start_addr)
    {
        case INI_CMD_BUF_7: 
            start_addr=7; 
            break;
        case INI_CMD_BUF_6: 
            start_addr=6; 
            break;
        case INI_CMD_BUF_5: 
            start_addr=5; 
            break;
        case INI_CMD_BUF_4: 
            start_addr=4; 
            break;
        default: 
            start_addr=7; 
            break;
    }
    return (start_addr);
}

/*****************************************************************************/
//  Description:  Send cmd and data to spi flash.
//  Author:       lichd
//  Param:
//      cs         : cs0/cs1
//      cmd_des_ptr: cmd to be send including cmd_mode/bit_mode/send_mode.
//      cmd_flag   : SPI_CMD_DATA_BEGIN/NULL/SPI_CMD_DATA_END
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
/*
    Set cmd and data to buffer.
*/
PUBLIC void SFCDRV_SetCmdData(int cs, SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_flag)
{
    //SCI_ASSERT((cmd_buf_index<=12)&&(read_buf_index<=12));

    if(cmd_flag == SPI_CMD_DATA_BEGIN)
    {
        //clear all cmd buffer register and type cfg register.
        SFCDRV_CMDBufClr(cs);
        SFCDRV_TypeBufClr(cs);
        cmd_buf_index =0;
        read_buf_index=SFCDRV_GetInitAddr(cs);
    }

    //Add the cmd to the cmd_buffer and type_cfg_buffer
    if(cmd_des_ptr != SCI_NULL)
    {
        SFCDRV_SetCMDBuf(cs, cmd_buf_index, cmd_des_ptr->cmd);
        SFCDRV_SetTypeInfBuf(cs, cmd_buf_index, 
                             cmd_des_ptr->bit_mode, 
                             cmd_des_ptr->cmd_byte_len, 
                             cmd_des_ptr->cmd_mode,
                             cmd_des_ptr->send_mode);   
        cmd_buf_index++;    
    }

    if(cmd_flag == SPI_CMD_DATA_END)
    {  
        /* clear int status */
        SFCDRV_IntClr(cs);    
#if defined(CHIP_VER_UWS6121E)
        SFCDRV_SoftReq(cs);
        
        //Wait for cmd finish.
        while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs))) {} 
#else
        SFCDRV_SoftReq( );
        
        //Wait for cmd finish.
        while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus())) {} 
#endif 
    }
}

PUBLIC void SFCDRV_SetReadBuf(int cs, SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_flag)
{
    if(cmd_flag == SPI_CMD_DATA_BEGIN)
    {
        //clear all cmd buffer register and type cfg register.
        SFCDRV_CMDBufClr(cs);
        SFCDRV_TypeBufClr(cs);
        cmd_buf_index =0;
        read_buf_index=SFCDRV_GetInitAddr(cs);        
    }

    //Add the cmd to the cmd_buffer and type_cfg_buffer
    if(cmd_des_ptr != SCI_NULL)
    {
        SFCDRV_SetCMDBuf(cs, read_buf_index, cmd_des_ptr->cmd);
        SFCDRV_SetTypeInfBuf(cs, read_buf_index, 
                             cmd_des_ptr->bit_mode, 
                             cmd_des_ptr->cmd_byte_len, 
                             cmd_des_ptr->cmd_mode,
                             cmd_des_ptr->send_mode);   
        read_buf_index++;   
    }

    if(cmd_flag == SPI_CMD_DATA_END)
    {  
        /* clear int status */
        SFCDRV_IntClr(cs);   
#if defined(CHIP_VER_UWS6121E)
        SFCDRV_SoftReq(cs);
#else
        SFCDRV_SoftReq( );
#endif
    }

    //Wait for cmd finish.
#if defined(CHIP_VER_UWS6121E)
    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs))) {} 
#else
    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus())) {} 
#endif
}

PUBLIC void SFCDRV_GetReadBuf(int cs,uint32* buffer, uint32 word_cnt)
{
    uint32   i   =0;
    uint32   cnt =0;
    uint32   _start_index = SFCDRV_GetInitAddr(cs);
    uint32   data_in=0;
    uint8*   data_ptr8 = (uint8*)(&data_in);

    for(cnt= _start_index; (cnt<read_buf_index)&&(i<word_cnt); cnt++)
    {
        data_in = SFCDRV_GetCMDBuf(cs, cnt);
        
        /* Little endian support only */
        //buffer[i++] = (data_ptr8[0]<<24)|(data_ptr8[1]<<16)|(data_ptr8[2]<<8)|(data_ptr8[3]<<0);
        buffer[i++] = (data_ptr8[0]<<24)|(data_ptr8[3]<<16)|(data_ptr8[2]<<8)|(data_ptr8[1]<<0);    /*cmd buf resp diff*/
    }
}

PUBLIC void SFCDRV_ResetAllBuf(uint32 cs)
{
    //clear all cmd buffer register and type cfg register.
    SFCDRV_CMDBufClr(cs);
    SFCDRV_TypeBufClr(cs);
    cmd_buf_index =0;
    read_buf_index=SFCDRV_GetInitAddr(cs);    
}

PUBLIC void SFCDRV_Req(uint32 cs, uint32 time_out)
{
    /* clear int status */
    SFCDRV_IntClr(cs);    
#if defined(CHIP_VER_UWS6121E)
    SFCDRV_SoftReq(cs);

    //Wait for cmd finish.
    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs))) {} 
#else
    SFCDRV_SoftReq( );

    //Wait for cmd finish.
    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus())) {} 
#endif
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

