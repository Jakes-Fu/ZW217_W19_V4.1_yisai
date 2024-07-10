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
#include "sc_reg.h" 
#include "sfc_drvapi.h"


#define SFC_PRINT(x)
//#define SFC_PRINT(x) SCI_TRACE_LOW x

LOCAL SFC_REG_T *reg_sfc = (SFC_REG_T *) SFC_REG_BASE ;



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
    
       if(cs == 0)
	{
		reg_sfc->cmd_cfg.mBits.rdata_bit_mode = bitmode;
		reg_sfc->cmd_cfg.mBits.sts_ini_addr_sel = iniAddSel;
	}
	else
	{
		reg_sfc->cs1_cmd_cfg.mBits.rdata_bit_mode = bitmode;
		reg_sfc->cs1_cmd_cfg.mBits.sts_ini_addr_sel = iniAddSel;
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
PUBLIC void SFCDRV_SoftReq(void)
{
	reg_sfc->soft_req.mBits.soft_req = BIT_0;
}

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
	else
	{
		reg_sfc->cs1_cmd_buf0.dwValue = 0;
		reg_sfc->cs1_cmd_buf1.dwValue = 0;
		reg_sfc->cs1_cmd_buf2.dwValue = 0;
		reg_sfc->cs1_cmd_buf3.dwValue = 0;
		reg_sfc->cs1_cmd_buf4.dwValue = 0;
		reg_sfc->cs1_cmd_buf5.dwValue = 0;
		reg_sfc->cs1_cmd_buf6.dwValue = 0;
		reg_sfc->cs1_cmd_buf7.dwValue = 0;
		reg_sfc->cs1_cmd_buf8.dwValue = 0;
		reg_sfc->cs1_cmd_buf9.dwValue = 0;
		reg_sfc->cs1_cmd_buf10.dwValue = 0;
		reg_sfc->cs1_cmd_buf11.dwValue = 0;  	
	}
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
	if(cs == 0)
	{
		reg_sfc->tbuf_clr.mBits.tbuf_clr = BIT_0;
	}
	else
	{
		reg_sfc->cs1_tbuf_clr.mBits.tbuf_clr = BIT_0;
	}		
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
	if(cs == 0)
	{
		reg_sfc->int_clr.mBits.int_clr = BIT_0;
	}
	else
	{
		reg_sfc->int_clr.mBits.int_clr1 = BIT_0;
	}	
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
	return reg_sfc->status.dwValue;
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
	if(cs == 0)
	{
		reg_sfc->cs_timing_cfg.dwValue = value;
	}
	else
	{
		reg_sfc->cs1_timing_cfg.dwValue = value;
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
	if(cs == 0)
	{
		reg_sfc->rd_timing_cfg.dwValue = value;
	}
	else
	{
		reg_sfc->cs1_rd_timing_cfg.dwValue = value;
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
	if(cs == 0)
	{
		reg_sfc->clk_cfg.dwValue = value;
	}
	else
	{
		reg_sfc->cs1_clk_cfg.dwValue = value;
	}	
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
	reg_sfc->cs_cfg.dwValue = value;
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
	if(cs == 0)
	{
		reg_sfc->endian_cfg.dwValue = value;
	}
	else
	{
		reg_sfc->cs1_endian_cfg.dwValue = value;
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
	if(cs == 0)
	{
		switch (index)
		{

			case CMD_BUF_0:
				reg_sfc->cmd_buf0.mBits.cmd_buf0 = value;
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
				SCI_ASSERT(0); /*assert to do*/
				break;					
		}
	}
	else
	{
		switch (index)
		{
			case CMD_BUF_0:
				reg_sfc->cs1_cmd_buf0.mBits.cmd_buf0 = value;
				break;

			case CMD_BUF_1:
				reg_sfc->cs1_cmd_buf1.mBits.cmd_buf1 = value;
				break;

			case CMD_BUF_2:
				reg_sfc->cs1_cmd_buf2.mBits.cmd_buf2 = value;
				break;

			case CMD_BUF_3:
				reg_sfc->cs1_cmd_buf3.mBits.cmd_buf3 = value;
				break;
				
			case CMD_BUF_4:
				reg_sfc->cs1_cmd_buf4.mBits.cmd_buf4 = value;
				break;

			case CMD_BUF_5:
				reg_sfc->cs1_cmd_buf5.mBits.cmd_buf5 = value;
				break;

			case CMD_BUF_6:
				reg_sfc->cs1_cmd_buf6.mBits.cmd_buf6 = value;
				break;

			case CMD_BUF_7:
				reg_sfc->cs1_cmd_buf7.mBits.cmd_buf7 = value;
				break;			

			case CMD_BUF_8:
				reg_sfc->cs1_cmd_buf8.mBits.cmd_buf8 = value;
				break;

			case CMD_BUF_9:
				reg_sfc->cs1_cmd_buf9.mBits.cmd_buf9 = value;
				break;

			case CMD_BUF_10:
				reg_sfc->cs1_cmd_buf10.mBits.cmd_buf10 = value;
				break;

			case CMD_BUF_11:
				reg_sfc->cs1_cmd_buf11.mBits.cmd_buf11 = value;
				break;		
			default:
				SCI_ASSERT(0); /*assert to do*/
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
	SCI_ASSERT((index+ count) <= (CMD_BUF_MAX*4)); /*assert to do*/
	
	if(cs == 0)
	{
		cmd_buf = (SFC_CMD_BUF0_U *)((uint32)&reg_sfc->cmd_buf0 + index*sizeof(SFC_CMD_BUF0_U));
	}
	else
	{
		cmd_buf = (SFC_CMD_BUF0_U *)((uint32)&reg_sfc->cs1_cmd_buf0 + index*sizeof(SFC_CMD_BUF0_U));
	}	

	_memcpy((uint8*)&cmd_buf->dwValue, buf, count);
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
	if(cs == 0)
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
		}
	}
	else
	{
		switch (index)
		{
			case CMD_BUF_0:
				value = reg_sfc->cs1_cmd_buf0.mBits.cmd_buf0;
				break;

			case CMD_BUF_1:
				value = reg_sfc->cs1_cmd_buf1.mBits.cmd_buf1;
				break;

			case CMD_BUF_2:
				value = reg_sfc->cs1_cmd_buf2.mBits.cmd_buf2;
				break;

			case CMD_BUF_3:
				value = reg_sfc->cs1_cmd_buf3.mBits.cmd_buf3;
				break;
				
			case CMD_BUF_4:
				value = reg_sfc->cs1_cmd_buf4.mBits.cmd_buf4;
				break;

			case CMD_BUF_5:
				value = reg_sfc->cs1_cmd_buf5.mBits.cmd_buf5;
				break;

			case CMD_BUF_6:
				value = reg_sfc->cs1_cmd_buf6.mBits.cmd_buf6;
				break;

			case CMD_BUF_7:
				value = reg_sfc->cs1_cmd_buf7.mBits.cmd_buf7;
				break;			

			case CMD_BUF_8:
				value = reg_sfc->cs1_cmd_buf8.mBits.cmd_buf8;
				break;

			case CMD_BUF_9:
				value = reg_sfc->cs1_cmd_buf9.mBits.cmd_buf9;
				break;

			case CMD_BUF_10:
				value = reg_sfc->cs1_cmd_buf10.mBits.cmd_buf10;
				break;

			case CMD_BUF_11:
				value = reg_sfc->cs1_cmd_buf11.mBits.cmd_buf11;
				break;
			default:
				SCI_ASSERT(0); /*assert to do*/
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
	if(cs == 0)
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
		}		
	}	
	else
	{
		switch (index)
		{
			case CMD_BUF_0:
				reg_sfc->cs1_type_buf0.mBits.valid0 = BIT_0;
				reg_sfc->cs1_type_buf0.mBits.bit_mode0 = bitmode;
				reg_sfc->cs1_type_buf0.mBits.byte_num0 = bytenum;
				reg_sfc->cs1_type_buf0.mBits.operation_status0 = cmdmode;
				reg_sfc->cs1_type_buf0.mBits.byte_send_mode0 = sendmode;
				break;

			case CMD_BUF_1:
				reg_sfc->cs1_type_buf0.mBits.valid1 = BIT_0;
				reg_sfc->cs1_type_buf0.mBits.bit_mode1 = bitmode;
				reg_sfc->cs1_type_buf0.mBits.byte_num1 = bytenum;
				reg_sfc->cs1_type_buf0.mBits.operation_status1 = cmdmode;
				reg_sfc->cs1_type_buf0.mBits.byte_send_mode1 = sendmode;
				break;

			case CMD_BUF_2:
				reg_sfc->cs1_type_buf0.mBits.valid2 = BIT_0;
				reg_sfc->cs1_type_buf0.mBits.bit_mode2 = bitmode;
				reg_sfc->cs1_type_buf0.mBits.byte_num2 = bytenum;
				reg_sfc->cs1_type_buf0.mBits.operation_status2 = cmdmode;
				reg_sfc->cs1_type_buf0.mBits.byte_send_mode2 = sendmode;
				break;

			case CMD_BUF_3:
				reg_sfc->cs1_type_buf0.mBits.valid3 = BIT_0;
				reg_sfc->cs1_type_buf0.mBits.bit_mode3 = bitmode;
				reg_sfc->cs1_type_buf0.mBits.byte_num3 = bytenum;
				reg_sfc->cs1_type_buf0.mBits.operation_status3 = cmdmode;
				reg_sfc->cs1_type_buf0.mBits.byte_send_mode3 = sendmode;
				break;
				
			case CMD_BUF_4:
				reg_sfc->cs1_type_buf1.mBits.valid4 = BIT_0;
				reg_sfc->cs1_type_buf1.mBits.bit_mode4 = bitmode;
				reg_sfc->cs1_type_buf1.mBits.byte_num4 = bytenum;
				reg_sfc->cs1_type_buf1.mBits.operation_status4 = cmdmode;
				reg_sfc->cs1_type_buf1.mBits.byte_send_mode4 = sendmode;
				break;

			case CMD_BUF_5:
				reg_sfc->cs1_type_buf1.mBits.valid5 = BIT_0;
				reg_sfc->cs1_type_buf1.mBits.bit_mode5 = bitmode;
				reg_sfc->cs1_type_buf1.mBits.byte_num5 = bytenum;
				reg_sfc->cs1_type_buf1.mBits.operation_status5 = cmdmode;
				reg_sfc->cs1_type_buf1.mBits.byte_send_mode5 = sendmode;
				break;

			case CMD_BUF_6:
				reg_sfc->cs1_type_buf1.mBits.valid6 = BIT_0;
				reg_sfc->cs1_type_buf1.mBits.bit_mode6 = bitmode;
				reg_sfc->cs1_type_buf1.mBits.byte_num6 = bytenum;
				reg_sfc->cs1_type_buf1.mBits.operation_status6 = cmdmode;
				reg_sfc->cs1_type_buf1.mBits.byte_send_mode6 = sendmode;
				break;

			case CMD_BUF_7:
				reg_sfc->cs1_type_buf1.mBits.valid7 = BIT_0;
				reg_sfc->cs1_type_buf1.mBits.bit_mode7 = bitmode;
				reg_sfc->cs1_type_buf1.mBits.byte_num7 = bytenum;
				reg_sfc->cs1_type_buf1.mBits.operation_status7 = cmdmode;
				reg_sfc->cs1_type_buf1.mBits.byte_send_mode7 = sendmode;
				break;			

			case CMD_BUF_8:
				reg_sfc->cs1_type_buf2.mBits.valid8 = BIT_0;
				reg_sfc->cs1_type_buf2.mBits.bit_mode8 = bitmode;
				reg_sfc->cs1_type_buf2.mBits.byte_num8 = bytenum;
				reg_sfc->cs1_type_buf2.mBits.operation_status8 = cmdmode;
				reg_sfc->cs1_type_buf2.mBits.byte_send_mode8 = sendmode;
				break;

			case CMD_BUF_9:
				reg_sfc->cs1_type_buf2.mBits.valid9 = BIT_0;
				reg_sfc->cs1_type_buf2.mBits.bit_mode9 = bitmode;
				reg_sfc->cs1_type_buf2.mBits.byte_num9 = bytenum;
				reg_sfc->cs1_type_buf2.mBits.operation_status9 = cmdmode;
				reg_sfc->cs1_type_buf2.mBits.byte_send_mode9 = sendmode;
				break;

			case CMD_BUF_10:
				reg_sfc->cs1_type_buf2.mBits.valid10 = BIT_0;
				reg_sfc->cs1_type_buf2.mBits.bit_mode10 = bitmode;
				reg_sfc->cs1_type_buf2.mBits.byte_num10 = bytenum;
				reg_sfc->cs1_type_buf2.mBits.operation_status10 = cmdmode;
				reg_sfc->cs1_type_buf2.mBits.byte_send_mode10 = sendmode;
				break;

			case CMD_BUF_11:
				reg_sfc->cs1_type_buf2.mBits.valid11 = BIT_0;
				reg_sfc->cs1_type_buf2.mBits.bit_mode11 = bitmode;
				reg_sfc->cs1_type_buf2.mBits.byte_num11 = bytenum;
				reg_sfc->cs1_type_buf2.mBits.operation_status11 = cmdmode;
				reg_sfc->cs1_type_buf2.mBits.byte_send_mode11 = sendmode;
				break;			
			default:
				SCI_ASSERT(0); /*assert to do*/
				break;	
		}
	}
}

/*****************************************************************************/
//  Description:  set cs1 start address register
//  Author: Yong.Li
//  Param:
//      start_addr: flash cs1 start address
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC uint32 SFCDRV_SetCS1StartAddr(uint32 start_addr)
{
    switch(start_addr)
    {
        case 0x00100000:
            reg_sfc->cs1_start_addr.mBits.cs1_start_address = 0x1; 
        break;

        case 0x00200000:
            reg_sfc->cs1_start_addr.mBits.cs1_start_address = 0x2; 
        break;

        case 0x00400000:
            reg_sfc->cs1_start_addr.mBits.cs1_start_address = 0x4; 
        break;

        case 0x00800000:
            reg_sfc->cs1_start_addr.mBits.cs1_start_address = 0x8; 
        break;

        case 0x01000000:
            reg_sfc->cs1_start_addr.mBits.cs1_start_address = 0x10; 
        break;

        case 0x02000000:
            reg_sfc->cs1_start_addr.mBits.cs1_start_address = 0x20; 
        break;

        case 0x04000000:
            reg_sfc->cs1_start_addr.mBits.cs1_start_address = 0x40; 
        break;

        case 0x08000000:
            reg_sfc->cs1_start_addr.mBits.cs1_start_address = 0x80; 
        break;

        default:
            return SCI_FALSE;
    }

    return SCI_TRUE;
}

