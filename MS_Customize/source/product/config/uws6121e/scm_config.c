/******************************************************************************
 ** File Name:      scm_config.c                                              *
 ** Author:                                                                   *
 ** DATE:           04/18/2008                                                *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product configurable content of SCM * 
 **                 module                                                    *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/18/2008     	                Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "card_sdio.h"
#include "scm_api.h"
#include "sci_api.h"


#include "bsd.h"
//xingyun.he test
#include "xsr_partition.h"
//#include "STL.h"
#include "ldo_drv.h"
#include "scm_config.h"
#include "gpio_prod_api.h"
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
 


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define SCM_CFG_PRINT(x) SCI_TRACE_LOW x

#define SCM_SDIO_PIN_CMD		0x5010c020
#define SCM_SDIO_PIN_D0			0x5010c024
#define SCM_SDIO_PIN_D1			0x5010c028
#define SCM_SDIO_PIN_D2			0x5010c02c
#define SCM_SDIO_PIN_D3			0x5010c030
#define SCM_SDIO_PIN_CLK		0x5010c01c
#define SCM_APB_CLK_CFG0		0x50100148	//6800H:[3:2] 00:XTL_Clock  01:MPLL_DIV8(48Mhz)  1x:MPLL_DIV4(96Mhz)
#define SCM_GR_MPLL_MN			0x50109040	//6800H:0x8B00001C		[11:0]MPLL_N		[20:16]MPLL_M		F=26Mhz*N/M
												//8800H:0x8B000024	[4:0]MPLL_M		[27:16]MPLL_N
												//8800G:0x8B000024	[11:0]MPLL_N		[21:16]MPLL_M
//#define SCM_GR_CLK_GEN5			0x8B00007C	//8800H:[18:17] CLK_SDIOPLL_SEL 00:MPLL  01:VPLL  1x:TDPLL  [9:5] CLK_SDIO_DIV F = f(pll)/(n+1)
 												//8800G:[18:17] PLL source select for clk_sdio 00:96Mhz 01:64Mhz 10:48Mhz 11:26Mhz
//#define SCM_GR_CLK_EN				0x8B000074	//8800H: bit12 CLK_SDIO enable

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
#if 1
PUBLIC uint32 SCM_TASK_STACK_SIZE = 2048;
LOCAL uint32 SCM_TASK_QUEUE_NUM = 40;
PUBLIC uint32 SCM_TASK_TASK_PRIO = 26;
#endif

LOCAL BOOLEAN change_mode = FALSE;		//FALSE: Default use High Speed Mode    TRUE: Change to Normal Speed Mode

#ifdef DUAL_TCARD_SUPPORT
LOCAL CARD_SDIO_HANDLE s_cardHandle1;
LOCAL BOOLEAN change_mode1 = FALSE;		//FALSE: Default use High Speed Mode    TRUE: Change to Normal Speed Mode
#endif
/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/
#define SD_SUPORT_DIAG // support SD diag simulate
#ifdef  SD_SUPORT_DIAG
#include "diag.h"
#define SDCARD_SUPPORT_NUM 1

typedef enum
{
	PLUGIN,
	PLUGOUT,
	CARD_ERR,
	ENTER_ATEST_MODE,
	EXIT_ATEST_MODE,
	SCM_MSG_MAX
}SCM_MSG_E;

typedef struct
{
    SCM_SLOT_NAME_E slot_name;
    SCM_MSG_E  msg;    
}SDCARD_MSG_T;

LOCAL uint8 s_atest_mode[SDCARD_SUPPORT_NUM] = {0}; // ATest Mode
LOCAL uint8 s_sd_card_status[SDCARD_SUPPORT_NUM] = {0};  // ATest Mode	

LOCAL uint32 Slot0_GetCapacity(void);
#endif 


#if 1 // used for Slot config
LOCAL CARD_SDIO_HANDLE s_cardHandle;

LOCAL BOOLEAN slot0_OpenProtocol()
{
	s_cardHandle = CARD_SDIO_Open(CARD_SDIO_SLOT_0);

	if(0 != s_cardHandle)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LOCAL BOOLEAN slot0_CloseProtocol(void)
{
	CARD_SDIO_Close(s_cardHandle);
	return TRUE;
}

/******************************************************************************/
// Description: support SD diag simulate 
// Note:
/******************************************************************************/
#ifdef  SD_SUPORT_DIAG
LOCAL uint32 _SD_diag_handler (
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{	
    SDCARD_MSG_T sdcard_msg, * sdcard_msg_ptr = PNULL;
	
    sdcard_msg_ptr   = (SDCARD_MSG_T *) (src_ptr + sizeof (MSG_HEAD_T));
    sdcard_msg.slot_name = sdcard_msg_ptr->slot_name; 
    sdcard_msg.msg = sdcard_msg_ptr->msg;	
    //SCI_TRACE_LOW:"SCM: DIAG, slot_name = %d, msg = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_145_112_2_18_0_26_32_38,(uint8*)"dd", sdcard_msg.slot_name, sdcard_msg.msg);
	
    // Build response to channel server
    *dest_ptr = SCI_ALLOC_APP (sizeof (MSG_HEAD_T));
    *dest_len_ptr = sizeof (MSG_HEAD_T);
    memcpy (*dest_ptr, src_ptr, sizeof (MSG_HEAD_T));
    ( (MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
    
#ifndef WIN32
	switch(sdcard_msg.msg)
	{
		case ENTER_ATEST_MODE:		// Enter ATest Mode
		{
			s_atest_mode[sdcard_msg.slot_name] = 1;
		}
		break;

		case EXIT_ATEST_MODE:		// Enter ATest Mode
		{
			s_atest_mode[sdcard_msg.slot_name] = 0;
		}
		break;
		
		case PLUGIN:		// SD Card plug-in
		{
			//Send message to SCM
			s_sd_card_status[sdcard_msg.slot_name] = PLUGIN;
			SCM_PlugIn(sdcard_msg.slot_name);
		}
		break;

		case PLUGOUT:	// SD Card plug-out
		{
			//Send message to SCM
			s_sd_card_status[sdcard_msg.slot_name] = PLUGOUT;
			SCM_PlugOut(sdcard_msg.slot_name);
		}
		break;

		case CARD_ERR:	 // SD Card error
		{
			//Send message to SCM
			s_sd_card_status[sdcard_msg.slot_name] = CARD_ERR;
			if(SCM_SLOT_0 == sdcard_msg.slot_name)
			{
				Slot0_GetCapacity();
			}
			else
			{
				//SCM_CFG_PRINT:"SCM: ATest Mode slot_name not support!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_194_112_2_18_0_26_32_39,(uint8*)"");
			 	return SCI_FALSE;	
			}
		}
		break;

		default:
		{
			//SCM_CFG_PRINT:"SCM: ATest Mode MSG error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_202_112_2_18_0_26_32_40,(uint8*)"");
			return SCI_FALSE;
		}
		//break;
	}

#endif//WIN32

	return SCI_TRUE;

}
#endif

LOCAL BOOLEAN slot0_Init(void)
{
	BOOLEAN ret = FALSE;
	
        //SCM_CFG_PRINT:"[Init]SCM_SDIO_PIN_CMD = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_219_112_2_18_0_26_32_41,(uint8*)"d", * (volatile uint32 *) SCM_SDIO_PIN_CMD);
        //SCM_CFG_PRINT:"[Init]SCM_SDIO_PIN_D0 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_220_112_2_18_0_26_32_42,(uint8*)"d", * (volatile uint32 *) SCM_SDIO_PIN_D0);
        //SCM_CFG_PRINT:"[Init]SCM_SDIO_PIN_D1 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_221_112_2_18_0_26_32_43,(uint8*)"d", * (volatile uint32 *) SCM_SDIO_PIN_D1);
        //SCM_CFG_PRINT:"[Init]SCM_SDIO_PIN_D2 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_222_112_2_18_0_26_32_44,(uint8*)"d",  * (volatile uint32 *)SCM_SDIO_PIN_D2);
        //SCM_CFG_PRINT:"[Init]SCM_SDIO_PIN_D3 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_223_112_2_18_0_26_32_45,(uint8*)"d", * (volatile uint32 *) SCM_SDIO_PIN_D3);
        //SCM_CFG_PRINT:"[Init]SCM_SDIO_PIN_CLK = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_224_112_2_18_0_26_32_46,(uint8*)"d", * (volatile uint32 *)SCM_SDIO_PIN_CLK);
        //SCM_CFG_PRINT:"[Init]SCM_APB_CLK_CFG0 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_225_112_2_18_0_26_32_47,(uint8*)"d", * (volatile uint32 *)SCM_APB_CLK_CFG0);
        //SCM_CFG_PRINT:"[Init]SCM_GR_MPLL_MN = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_226_112_2_18_0_26_32_48,(uint8*)"d", * (volatile uint32 *)SCM_GR_MPLL_MN);

#ifdef SD_SUPORT_DIAG
        //DIAG_RegisterCmdRoutine(DIAG_SDCARD_TEST_F, _SD_diag_handler);
#endif

	if(!change_mode)
	{
		ret = CARD_SDIO_InitCard(s_cardHandle, HIGH_SPEED_MODE);
	}
	else
	{
		ret = CARD_SDIO_InitCard(s_cardHandle, NORMAL_SPEED_MODE);
	}

	return ret;	
}

LOCAL BOOLEAN slot0_IsExist(void)
{
	if(0 == s_atest_mode[0])	//normal mode
	{
    		return TRUE; //GPIO_CheckSDCardStatus();
	}
	else		//ATest mode
	{
        	if(1==s_sd_card_status[0]) // Plug Out	
        	{
			//SCM_CFG_PRINT:"ATest Mode PO -- Plug out"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_254_112_2_18_0_26_32_49,(uint8*)"");
			return FALSE;
		}
		else if(0==s_sd_card_status[0]) // Plug In
		{
			//SCM_CFG_PRINT:"ATest Mode PI -- Plug in"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_259_112_2_18_0_26_32_50,(uint8*)"");
			return TRUE;
		}
		else		//ATEST CARD_ERR
		{
			return TRUE;
		}
	}
}

#ifdef DUAL_TCARD_SUPPORT
LOCAL BOOLEAN SDIO0_Pwr(SCM_SLOT_NAME_E slotNO,SLOT_PWR_SWITCH_E pwrOn)
{
	static int32 iSlot0_PowerLogicStatus=0;//o:not need power, 1:need power, 
	static int32 iSlot1_PowerLogicStatus=0;

	if(SCM_SLOT_0 == slotNO)
	{
		if(SLOT_PWRON == pwrOn)
		{
			CARD_SDIO_PwrCtl(s_cardHandle,TRUE);
			iSlot0_PowerLogicStatus=1;
			return TRUE/* open power*/;
			
		}
		else if(SLOT_PWROFF == pwrOn)
		{
			if(iSlot1_PowerLogicStatus==0)
			{
				CARD_SDIO_PwrCtl(s_cardHandle,FALSE);
			}
			iSlot0_PowerLogicStatus = 0;
			return TRUE/* close power*/;
		}
		else
		{
			//SCI_ASSERT(0);
			//SCI_TRACE_LOW:"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_296_112_2_18_0_26_32_51,(uint8*)"" );
			return FALSE;	/*lint !e527*/
		}
	}

	if(SCM_SLOT_1 == slotNO)
	{
		if(SLOT_PWRON == pwrOn)
		{
			CARD_SDIO_PwrCtl(s_cardHandle1,TRUE);
			iSlot1_PowerLogicStatus=1;
			return TRUE/* open power*/;
			
		}
		else if(SLOT_PWROFF == pwrOn)
		{
			if(iSlot0_PowerLogicStatus==0)
			{
				CARD_SDIO_PwrCtl(s_cardHandle1,FALSE);
			}
			iSlot1_PowerLogicStatus = 0;
			return TRUE/* close power*/;
		}
		else
		{
			//SCI_ASSERT(0);
			//SCI_TRACE_LOW:"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_322_112_2_18_0_26_32_52,(uint8*)"" );
			return FALSE;	/*lint !e527*/
		}
	}
	/* else ....*/
	else
	{
        //SCI_ASSERT(0);
		//SCI_TRACE_LOW:"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_330_112_2_18_0_26_32_53,(uint8*)"" );
		return FALSE;	/*lint !e527*/
	}
}
#else
LOCAL BOOLEAN SDIO0_Pwr(SCM_SLOT_NAME_E slotNO,SLOT_PWR_SWITCH_E pwrOn)
{
	if(SCM_SLOT_0 == slotNO)
	{
		if(SLOT_PWRON == pwrOn)
		{
			CARD_SDIO_PwrCtl(s_cardHandle,TRUE);
			return TRUE/* open power*/;
			
		}
		else if(SLOT_PWROFF == pwrOn)
		{
			CARD_SDIO_PwrCtl(s_cardHandle,FALSE);
			return TRUE/* close power*/;
		}
		else
		{
			//SCI_ASSERT(0);
			//SCI_TRACE_LOW:"[DRV_SCM]<ERROR> val:pwrOn is wrong\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_353_112_2_18_0_26_32_54,(uint8*)"" );
			return FALSE;  /*lint !e527 */
		}
	}
	/* else ....*/
	else
	{
		//SCI_ASSERT(0);
		//SCI_TRACE_LOW:"[DRV_SCM]<ERROR> val:slotNO is wrong\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_361_112_2_18_0_26_32_55,(uint8*)"" );
		return FALSE;  /*lint !e527 */
	}
}
#endif


//#define CHIP_SDIO_FIX

#ifdef SRAM_SIZE_16MBIT
#define CHIPSDIOFIXBUFSIZE 16
#elif SRAM_SIZE_32MBIT/*lint !e553*/
#define CHIPSDIOFIXBUFSIZE 32

#else
#define CHIPSDIOFIXBUFSIZE 32   /* 64  */
#endif
__align(32) LOCAL uint8 s_chipFixBuf[CHIPSDIOFIXBUFSIZE*512];

#if 1


void   q(void){};
#endif
#define CHIPSDIOFIX_MEMCPY SCI_MEMCPY

#define ADDR_ALIGN_MASK_32  0x1f
#define ADDR_ALIGN_MASK_4   0x03

LOCAL BOOLEAN slot0_Read(uint32 startBlock,uint32 num,uint8* buf)
{
#ifdef CHIP_SDIO_FIX
	uint32 i,tmpStartBlock;

	if(0!= (ADDR_ALIGN_MASK_32&((uint32)buf)))
	{
		if(1 == num)
		{
			if(FALSE == CARD_SDIO_ReadSingleBlock(s_cardHandle, startBlock, s_chipFixBuf))
			{
				return FALSE;
			}
			CHIPSDIOFIX_MEMCPY(buf,s_chipFixBuf,512);
			return TRUE;
		}
		else
		{
			tmpStartBlock = startBlock;
			for(i = 0; i < num/CHIPSDIOFIXBUFSIZE; i++)
			{
				if(FALSE == CARD_SDIO_ReadMultiBlock(s_cardHandle, tmpStartBlock, CHIPSDIOFIXBUFSIZE, s_chipFixBuf))
				{
					return FALSE;
				}
				CHIPSDIOFIX_MEMCPY(buf+((tmpStartBlock-startBlock)*512),s_chipFixBuf,512*CHIPSDIOFIXBUFSIZE);
				tmpStartBlock += CHIPSDIOFIXBUFSIZE;
			}
			if(0 == (num%CHIPSDIOFIXBUFSIZE))
			{
				return TRUE;
			}
			if(FALSE == CARD_SDIO_ReadMultiBlock(s_cardHandle, tmpStartBlock, num%CHIPSDIOFIXBUFSIZE, s_chipFixBuf))
			{
				return FALSE;
			}
			CHIPSDIOFIX_MEMCPY(buf+((tmpStartBlock-startBlock)*512),s_chipFixBuf,512*(num%CHIPSDIOFIXBUFSIZE));
			return TRUE;
		}
	}
	else
	{
#endif
	if(1 == num)
	{
		return CARD_SDIO_ReadSingleBlock(s_cardHandle, startBlock, buf);
	}
	else
	{
		return CARD_SDIO_ReadMultiBlock(s_cardHandle, startBlock, num, buf);
	}
#ifdef CHIP_SDIO_FIX
	}
#endif
}

LOCAL BOOLEAN slot0_Write(uint32 startBlock,uint32 num,uint8* buf)
{
#ifdef CHIP_SDIO_FIX
	uint32 i,tmpStartBlock;

	if(0!= (ADDR_ALIGN_MASK_32&((uint32)buf)))
	{
		if(1 == num)
		{
			CHIPSDIOFIX_MEMCPY(s_chipFixBuf,buf,512);
			if(FALSE == CARD_SDIO_WriteSingleBlock(s_cardHandle, startBlock, s_chipFixBuf))
			{
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			tmpStartBlock = startBlock;
			for(i = 0; i < num/CHIPSDIOFIXBUFSIZE; i++)
			{
				CHIPSDIOFIX_MEMCPY(s_chipFixBuf,buf+((tmpStartBlock-startBlock)*512),512*CHIPSDIOFIXBUFSIZE);
				if(FALSE == CARD_SDIO_WriteMultiBlock(s_cardHandle, tmpStartBlock, CHIPSDIOFIXBUFSIZE, s_chipFixBuf))
				{
					return FALSE;
				}
				tmpStartBlock += CHIPSDIOFIXBUFSIZE;
			}
			if(0 == (num%CHIPSDIOFIXBUFSIZE))
			{
				return TRUE;
			}
			CHIPSDIOFIX_MEMCPY(s_chipFixBuf,buf+((tmpStartBlock-startBlock)*512),512*(num%CHIPSDIOFIXBUFSIZE));
			if(FALSE == CARD_SDIO_WriteMultiBlock(s_cardHandle, tmpStartBlock, num%CHIPSDIOFIXBUFSIZE, s_chipFixBuf))
			{
				return FALSE;
			}
			return TRUE;
		}
	}
	else
	{
#endif
	if(1 == num)
	{
		return CARD_SDIO_WriteSingleBlock(s_cardHandle, startBlock, buf);
	}
	else
	{
		return CARD_SDIO_WriteMultiBlock(s_cardHandle, startBlock, num, buf);
	}
#ifdef CHIP_SDIO_FIX
	}
#endif
}

LOCAL BOOLEAN slot0_ReadExt(uint32 startBlock,SCM_SCTBUF_INFO_T* sctInfo)
{
	BOOLEAN ret_val = SCI_FALSE;
	uint32 i ,blk;

	blk = startBlock;
	for(i = 0; i < sctInfo->cnt; i++)
	{
		if(1 == sctInfo->item[i].num)
		{
			ret_val = CARD_SDIO_ReadSingleBlock(s_cardHandle, blk, sctInfo->item[i].buf);
		}
		else
		{
			ret_val = CARD_SDIO_ReadMultiBlock(s_cardHandle, blk, sctInfo->item[i].num, sctInfo->item[i].buf);
		}
		blk += sctInfo->item[i].num;
		if(FALSE == ret_val)
		{
			return FALSE;
		}
	}
	return TRUE;

}

LOCAL BOOLEAN slot0_WriteExt(uint32 startBlock,SCM_SCTBUF_INFO_T* sctInfo)
{
	BOOLEAN ret_val = SCI_FALSE;
	uint32 i ,blk;

	blk = startBlock;
	for(i = 0; i < sctInfo->cnt; i++)
	{
		if(1 == sctInfo->item[i].num)
		{
			ret_val = CARD_SDIO_WriteSingleBlock(s_cardHandle, blk, sctInfo->item[i].buf);
		}
		else
		{
			ret_val = CARD_SDIO_WriteMultiBlock(s_cardHandle, blk, sctInfo->item[i].num, sctInfo->item[i].buf);
		}
		blk += sctInfo->item[i].num;
		if(FALSE == ret_val)
		{
			return FALSE;
		}
	}
	return TRUE;

}

LOCAL BOOLEAN slot0_ERASE(uint32 startBlock,uint32 endBlock)
{
#if 0
/*
	return CARD_SDIO_Erase(s_cardHandle, startBlock, endBlock);
*/
#else
	return TRUE;
#endif
}

LOCAL uint32 slot0_GetCapcity()
{
	if((1==s_atest_mode[0]) && (CARD_ERR==s_sd_card_status[0]))  //ATest mode, error case
	{
		return 0;
	}
	else
	{
		return CARD_SDIO_GetCapacity(s_cardHandle);
	}
}

#if defined(SPRD_SUPPORT_MCEX)

#error
LOCAL BOOLEAN slot0_EnterMcex(BOOLEAN* ifEnterMcex)
{
	return CARD_SDIO_EnterMcex(s_cardHandle,ifEnterMcex );
}
LOCAL BOOLEAN slot0_ResetTrm(void)
{
	return CARD_SDIO_CtlTrm(s_cardHandle);
}

LOCAL BOOLEAN slot0_SendPsi(uint32 arg,uint8* rspBuf)
{
	return CARD_SDIO_SendPsi(s_cardHandle,arg, rspBuf);
}

LOCAL BOOLEAN slot0_ReadSecCmd(uint8* rspBuf)
{
	return CARD_SDIO_ReadSecCmd(s_cardHandle, rspBuf);
}

LOCAL BOOLEAN slot0_WriteSecCmd(uint8* rspBuf)
{
	return CARD_SDIO_WriteSecCmd(s_cardHandle, rspBuf);
}

LOCAL BOOLEAN slot0_ExitMcex()
{
	return CARD_SDIO_ExitMcex(s_cardHandle);
}

#endif

LOCAL BOOLEAN slot0_ErrProc(SCM_FUN_E cmd,SCM_PARAM_T* param)
{
	uint32 i;
	uint32 retry = 1;
	CARD_SPEED_MODE speedmode = HIGH_SPEED_MODE;
  	static uint32 proc_count_HighMode = 0;
	static uint32 proc_count_NorMode = 0;
	static uint32 time1 = 0;
	static uint32 time2 = 0;

	if((1 == s_atest_mode[0]) && (CARD_ERR==s_sd_card_status[0]))	//ATest mode, error case
	{	
		return FALSE;
	}
	
/*Below solution is used to solve time is too long when format bad T-Cards. */
/*For these T-Cards, errproc will succeed, but in fact the data not written to T-Card.*/
/* @CR207499 */
	if(CARD_SDIO_IsEnterHighSpeed(s_cardHandle))
	{
		if(0 == proc_count_HighMode)
		{
	       	time1 = SCI_GetTickCount();
		     	proc_count_HighMode++;
		}
		else
		{
			time2 = SCI_GetTickCount();
	        	proc_count_HighMode++;
			if(time2 - time1 >= 10000)
			{
				if(proc_count_HighMode > 2)
				{
					speedmode = NORMAL_SPEED_MODE;
					retry = 0;
					change_mode = TRUE;
					//SCM_CFG_PRINT:"SCM ErrProc too many in HighSpeed mode! Use normal mode!"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_633_112_2_18_0_26_33_56,(uint8*)"");
				}
				else
				{
					proc_count_HighMode = 0;
				}
			}
	       }
	}
	else
	{
		speedmode = NORMAL_SPEED_MODE;
		retry = 0;
		if(0 == proc_count_NorMode)
		{
	       	time1 = SCI_GetTickCount();
		     	proc_count_NorMode++;
		}
		else
		{
			time2 = SCI_GetTickCount();
	        	proc_count_NorMode++;
			if(time2 - time1 >= 10000)
			{
				if(proc_count_NorMode > 2)
				{
					//SCM_CFG_PRINT:"SCM ErrProc too many in normal mode! FALSE!"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_659_112_2_18_0_26_33_57,(uint8*)"");
					return FALSE;
				}
				else
				{
					proc_count_NorMode = 0;
				}
			}
	       }
	}
	
        //SCM_CFG_PRINT:"[ErrProc]SCM_SDIO_PIN_CMD = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_670_112_2_18_0_26_33_58,(uint8*)"d", * (volatile uint32 *) SCM_SDIO_PIN_CMD);
        //SCM_CFG_PRINT:"[ErrProc]SCM_SDIO_PIN_D0 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_671_112_2_18_0_26_33_59,(uint8*)"d", * (volatile uint32 *) SCM_SDIO_PIN_D0);
        //SCM_CFG_PRINT:"[ErrProc]SCM_SDIO_PIN_D1 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_672_112_2_18_0_26_33_60,(uint8*)"d", * (volatile uint32 *) SCM_SDIO_PIN_D1);
        //SCM_CFG_PRINT:"[ErrProc]SCM_SDIO_PIN_D2 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_673_112_2_18_0_26_33_61,(uint8*)"d",  * (volatile uint32 *)SCM_SDIO_PIN_D2);
        //SCM_CFG_PRINT:"[ErrProc]SCM_SDIO_PIN_D3 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_674_112_2_18_0_26_33_62,(uint8*)"d", * (volatile uint32 *) SCM_SDIO_PIN_D3);
        //SCM_CFG_PRINT:"[ErrProc]SCM_SDIO_PIN_CLK = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_675_112_2_18_0_26_33_63,(uint8*)"d", * (volatile uint32 *)SCM_SDIO_PIN_CLK);
        //SCM_CFG_PRINT:"[ErrProc]SCM_APB_CLK_CFG0 = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_676_112_2_18_0_26_33_64,(uint8*)"d", * (volatile uint32 *) SCM_APB_CLK_CFG0);
        //SCM_CFG_PRINT:"[ErrProc]SCM_GR_MPLL_MN = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_677_112_2_18_0_26_33_65,(uint8*)"d", * (volatile uint32 *)SCM_GR_MPLL_MN);

	do
	{
		//SCM_CFG_PRINT:"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_681_112_2_18_0_26_33_66,(uint8*)"dd", speedmode, retry);
		
		switch (cmd)
		{

			case SCM_FUNC_REINIT:
			{
				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle, speedmode))
					{
						return TRUE;
					}
				}
			}
				break;
				
			case SCM_FUNC_READ:
			{
				BOOLEAN result;
		//---
		        result = slot0_Read(param->readParam.startBlock, param->readParam.num, param->readParam.buf);
#if 0 //ken.kuang repace by local funtion.
                if(1 == param->readParam.num)
				{
					result = CARD_SDIO_ReadSingleBlock(s_cardHandle, param->readParam.startBlock, param->readParam.buf);
				}
				else
				{
					result = CARD_SDIO_ReadMultiBlock(s_cardHandle, param->readParam.startBlock, param->readParam.num, param->readParam.buf);
				}
#endif
				if(TRUE == result)
				{
					return TRUE;
				}

				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle, speedmode))
					{
                        result = slot0_Read(param->readParam.startBlock, param->readParam.num, param->readParam.buf);
#if 0 //ken.kuang repace by local funtion.
						if(1 == param->readParam.num)
						{
							result = CARD_SDIO_ReadSingleBlock(s_cardHandle, param->readParam.startBlock, param->readParam.buf);
						}
						else
						{
							result = CARD_SDIO_ReadMultiBlock(s_cardHandle, param->readParam.startBlock, param->readParam.num, param->readParam.buf);
						}
#endif
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
		//---
			}
				break;
				
			case SCM_FUNC_WRITE:
			{
				BOOLEAN result;
		//---
#if 0 //ken.kuang move this to below (A)
		        result = slot0_Write(param->writeParam.startBlock,param->writeParam.num,param->writeParam.buf);
#if 0 //ken.kuang repace by local funtion.
				if(1 == param->writeParam.num)
				{
					result = CARD_SDIO_WriteSingleBlock(s_cardHandle, param->writeParam.startBlock, param->writeParam.buf);
				}
				else
				{
					result = CARD_SDIO_WriteMultiBlock(s_cardHandle, param->writeParam.startBlock, param->writeParam.num, param->writeParam.buf);
				}
#endif
				if(TRUE == result)
				{
					return TRUE;
				}
#endif

				for(i = 0; i < 2; i++) // from (A) to here!
				{
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle, speedmode))
					{
		                result = slot0_Write(param->writeParam.startBlock,param->writeParam.num,param->writeParam.buf);
#if 0 //ken.kuang repace by local funtion.
						if(1 == param->writeParam.num)
						{
							result = CARD_SDIO_WriteSingleBlock(s_cardHandle, param->writeParam.startBlock, param->writeParam.buf);
						}
						else
						{
							result = CARD_SDIO_WriteMultiBlock(s_cardHandle, param->writeParam.startBlock, param->writeParam.num, param->writeParam.buf);
						}
#endif
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
		//---
			}
				break;
				
			case SCM_FUNC_ERASE:
			{
				return FALSE;
			}
//				break; for pclint
				
			case SCM_FUNC_GETCAPCITY:
			{
				return FALSE;
			}
//				break; for pclint
				
#if defined(SPRD_SUPPORT_MCEX)
			case SCM_FUNC_ENTERMCEX:
			{
				BOOLEAN result;

				result = CARD_SDIO_EnterMcex(s_cardHandle,param->enterMcex.ifEnterMcex);
				if(TRUE == result)
				{
					return TRUE;
				}
				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle, speedmode))
					{
						result = CARD_SDIO_EnterMcex(s_cardHandle,param->enterMcex.ifEnterMcex);
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
			}
				break;
				
			case SCM_FUNC_RESETTRM:
			{
				BOOLEAN result;

				result = CARD_SDIO_CtlTrm(s_cardHandle);
				if(TRUE == result)
				{
					return TRUE;
				}
				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle, speedmode))
					{
						result = CARD_SDIO_CtlTrm(s_cardHandle);
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
			}
				break;
			
			case SCM_FUNC_READ_REC_CMD:
			case SCM_FUNC_WRITE_REC_CMD:
			case SCM_FUNC_SEND_PSI:
			{
				for(i = 0; i < 1; i++)
				{
					BOOLEAN result;
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_0,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle, speedmode))
					{
						result = CARD_SDIO_EnterMcex(s_cardHandle,param->enterMcex.ifEnterMcex);
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
			}
				break;
#endif
			default:
			    //SCI_ASSERT(0);
			    //SCI_TRACE_LOW:"[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
			    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_882_112_2_18_0_26_33_67,(uint8*)"" );
				return FALSE;	/*lint !e527*/
		//		break; for pclint

		}

		//SCM_CFG_PRINT:"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_888_112_2_18_0_26_33_68,(uint8*)"dd", speedmode, retry);
		speedmode = NORMAL_SPEED_MODE;
		change_mode = TRUE;

		if(retry > 0)
		{
			SDIO0_Pwr(SCM_SLOT_0,SLOT_PWROFF);
			SDIO0_Pwr(SCM_SLOT_0,SLOT_PWRON);
			if(FALSE == CARD_SDIO_InitCard(s_cardHandle, speedmode))
			{
				return FALSE;
			}
		}
		
	}while(retry--);

	return FALSE;				
}


#ifdef DUAL_TCARD_SUPPORT
LOCAL BOOLEAN slot1_OpenProtocol()
{   
	s_cardHandle1 = CARD_SDIO_Open(CARD_SDIO_SLOT_1);

	if(0 != s_cardHandle1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LOCAL BOOLEAN slot1_CloseProtocol(void)
{
	CARD_SDIO_Close(s_cardHandle1);
	return TRUE;
}


LOCAL BOOLEAN slot1_Init(void)
{
	BOOLEAN ret = FALSE;
	
	if(!change_mode1)
	{
		ret = CARD_SDIO_InitCard(s_cardHandle1, HIGH_SPEED_MODE);
	}
	else
	{
		ret = CARD_SDIO_InitCard(s_cardHandle1, NORMAL_SPEED_MODE);
	}

	return ret;	
}

LOCAL BOOLEAN slot1_IsExist(void)
{
	if(0 == s_atest_mode[1])	//normal mode
	{
    		return TRUE;

	}
	else		//ATest mode
	{
        	if(1==s_sd_card_status[1]) // Plug Out	
        	{
			//SCM_CFG_PRINT:"ATest Mode PO -- Plug out"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_957_112_2_18_0_26_34_69,(uint8*)"");
			return FALSE;
		}
		else if(0==s_sd_card_status[1]) // Plug In
		{
			//SCM_CFG_PRINT:"ATest Mode PI -- Plug in"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_962_112_2_18_0_26_34_70,(uint8*)"");
			return TRUE;
		}
		else		//ATEST CARD_ERR
		{
			return TRUE;
		}
	}	
}


LOCAL BOOLEAN slot1_Read(uint32 startBlock,uint32 num,uint8* buf)
{
#ifdef CHIP_SDIO_FIX
	uint32 i,tmpStartBlock;

	if(0!= (ADDR_ALIGN_MASK_32&((uint32)buf)))
	{
		if(1 == num)
		{
			if(FALSE == CARD_SDIO_ReadSingleBlock(s_cardHandle1, startBlock, s_chipFixBuf))
			{
				return FALSE;
			}
			CHIPSDIOFIX_MEMCPY(buf,s_chipFixBuf,512);
			return TRUE;
		}
		else
		{
			tmpStartBlock = startBlock;
			for(i = 0; i < num/CHIPSDIOFIXBUFSIZE; i++)
			{
				if(FALSE == CARD_SDIO_ReadMultiBlock(s_cardHandle1, tmpStartBlock, CHIPSDIOFIXBUFSIZE, s_chipFixBuf))
				{
					return FALSE;
				}
				CHIPSDIOFIX_MEMCPY(buf+((tmpStartBlock-startBlock)*512),s_chipFixBuf,512*CHIPSDIOFIXBUFSIZE);
				tmpStartBlock += CHIPSDIOFIXBUFSIZE;
			}
			if(0 == (num%CHIPSDIOFIXBUFSIZE))
			{
				return TRUE;
			}
			if(FALSE == CARD_SDIO_ReadMultiBlock(s_cardHandle1, tmpStartBlock, num%CHIPSDIOFIXBUFSIZE, s_chipFixBuf))
			{
				return FALSE;
			}
			CHIPSDIOFIX_MEMCPY(buf+((tmpStartBlock-startBlock)*512),s_chipFixBuf,512*(num%CHIPSDIOFIXBUFSIZE));
			return TRUE;
		}
	}
	else
	{
#endif
	if(1 == num)
	{
		return CARD_SDIO_ReadSingleBlock(s_cardHandle1, startBlock, buf);
	}
	else
	{
		return CARD_SDIO_ReadMultiBlock(s_cardHandle1, startBlock, num, buf);
	}
#ifdef CHIP_SDIO_FIX
	}
#endif
}

LOCAL BOOLEAN slot1_Write(uint32 startBlock,uint32 num,uint8* buf)
{
#ifdef CHIP_SDIO_FIX
	uint32 i,tmpStartBlock;

	if(0!= (ADDR_ALIGN_MASK_32&((uint32)buf)))
	{
		if(1 == num)
		{
			CHIPSDIOFIX_MEMCPY(s_chipFixBuf,buf,512);
			if(FALSE == CARD_SDIO_WriteSingleBlock(s_cardHandle1, startBlock, s_chipFixBuf))
			{
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			tmpStartBlock = startBlock;
			for(i = 0; i < num/CHIPSDIOFIXBUFSIZE; i++)
			{
				CHIPSDIOFIX_MEMCPY(s_chipFixBuf,buf+((tmpStartBlock-startBlock)*512),512*CHIPSDIOFIXBUFSIZE);
				if(FALSE == CARD_SDIO_WriteMultiBlock(s_cardHandle1, tmpStartBlock, CHIPSDIOFIXBUFSIZE, s_chipFixBuf))
				{
					return FALSE;
				}
				tmpStartBlock += CHIPSDIOFIXBUFSIZE;
			}
			if(0 == (num%CHIPSDIOFIXBUFSIZE))
			{
				return TRUE;
			}
			CHIPSDIOFIX_MEMCPY(s_chipFixBuf,buf+((tmpStartBlock-startBlock)*512),512*(num%CHIPSDIOFIXBUFSIZE));
			if(FALSE == CARD_SDIO_WriteMultiBlock(s_cardHandle1, tmpStartBlock, num%CHIPSDIOFIXBUFSIZE, s_chipFixBuf))
			{
				return FALSE;
			}
			return TRUE;
		}
	}
	else
	{
#endif
	if(1 == num)
	{
		return CARD_SDIO_WriteSingleBlock(s_cardHandle1, startBlock, buf);
	}
	else
	{
		return CARD_SDIO_WriteMultiBlock(s_cardHandle1, startBlock, num, buf);
	}
#ifdef CHIP_SDIO_FIX
	}
#endif
/*
	if(1 == num)
	{
		return CARD_SDIO_WriteSingleBlock(s_cardHandle1, startBlock, buf);
	}
	else
	{
		return CARD_SDIO_WriteMultiBlock(s_cardHandle1, startBlock, num, buf);
	}
*/
}

LOCAL BOOLEAN slot1_ReadExt(uint32 startBlock,SCM_SCTBUF_INFO_T* sctInfo)
{
	BOOLEAN ret_val = SCI_FALSE;
	uint32 i ,blk;

	blk = startBlock;
	for(i = 0; i < sctInfo->cnt; i++)
	{
		if(1 == sctInfo->item[i].num)
		{
			ret_val = CARD_SDIO_ReadSingleBlock(s_cardHandle1, blk, sctInfo->item[i].buf);
		}
		else
		{
			ret_val = CARD_SDIO_ReadMultiBlock(s_cardHandle1, blk, sctInfo->item[i].num, sctInfo->item[i].buf);
		}
		blk += sctInfo->item[i].num;
		if(FALSE == ret_val)
		{
			return FALSE;
		}
	}
	return TRUE;

}

LOCAL BOOLEAN slot1_WriteExt(uint32 startBlock,SCM_SCTBUF_INFO_T* sctInfo)
{
	BOOLEAN ret_val = SCI_FALSE;
	uint32 i ,blk;

	blk = startBlock;
	for(i = 0; i < sctInfo->cnt; i++)
	{
		if(1 == sctInfo->item[i].num)
		{
			ret_val = CARD_SDIO_WriteSingleBlock(s_cardHandle1, blk, sctInfo->item[i].buf);
		}
		else
		{
			ret_val = CARD_SDIO_WriteMultiBlock(s_cardHandle1, blk, sctInfo->item[i].num, sctInfo->item[i].buf);
		}
		blk += sctInfo->item[i].num;
		if(FALSE == ret_val)
		{
			return FALSE;
		}
	}
	return TRUE;

}

LOCAL BOOLEAN slot1_ERASE(uint32 startBlock,uint32 endBlock)
{
#if 0
/*
	return CARD_SDIO_Erase(s_cardHandle1, startBlock, endBlock);
*/
#else
	return TRUE;
#endif
}

LOCAL uint32 slot1_GetCapcity()
{
	if((1==s_atest_mode[1]) && (CARD_ERR==s_sd_card_status[1]))  //ATest mode, error case
	{
		return 0;
	}
	else
	{
		return CARD_SDIO_GetCapacity(s_cardHandle1);
	}
}

#if defined(SPRD_SUPPORT_MCEX)

LOCAL BOOLEAN slot1_EnterMcex(BOOLEAN* ifEnterMcex)
{
	return CARD_SDIO_EnterMcex(s_cardHandle1,ifEnterMcex );
}
LOCAL BOOLEAN slot1_ResetTrm(void)
{
	return CARD_SDIO_CtlTrm(s_cardHandle1);
}

LOCAL BOOLEAN slot1_SendPsi(uint32 arg,uint8* rspBuf)
{
	return CARD_SDIO_SendPsi(s_cardHandle1,arg, rspBuf);
}

LOCAL BOOLEAN slot1_ReadSecCmd(uint8* rspBuf)
{
	return CARD_SDIO_ReadSecCmd(s_cardHandle1, rspBuf);
}

LOCAL BOOLEAN slot1_WriteSecCmd(uint8* rspBuf)
{
	return CARD_SDIO_WriteSecCmd(s_cardHandle1, rspBuf);
}

LOCAL BOOLEAN slot1_ExitMcex()
{
	return CARD_SDIO_ExitMcex(s_cardHandle1);
}

#endif

LOCAL BOOLEAN slot1_ErrProc(SCM_FUN_E cmd,SCM_PARAM_T* param)
{
	uint32 i;
	uint32 retry = 1;
	CARD_SPEED_MODE speedmode = HIGH_SPEED_MODE;
  	static uint32 proc_count_HighMode = 0;
	static uint32 proc_count_NorMode = 0;
	static uint32 time1 = 0;
	static uint32 time2 = 0;

	if((1 == s_atest_mode[1]) && (CARD_ERR==s_sd_card_status[1]))	//ATest mode, error case
	{	
		return FALSE;
	}
	
/*Below solution is used to solve time is too long when format bad T-Cards. */
/*For these T-Cards, errproc will succeed, but in fact the data not written to T-Card.*/
/* @CR207499 */
	if(CARD_SDIO_IsEnterHighSpeed(s_cardHandle1))
	{
		if(0 == proc_count_HighMode)
		{
	       	time1 = SCI_GetTickCount();
		     	proc_count_HighMode++;
		}
		else
		{
			time2 = SCI_GetTickCount();
	        	proc_count_HighMode++;
			if(time2 - time1 >= 10000)
			{
				if(proc_count_HighMode > 2)
				{
					speedmode = NORMAL_SPEED_MODE;
					retry = 0;
					change_mode1 = TRUE;
					//SCM_CFG_PRINT:"SCM ErrProc too many in HighSpeed mode! Use normal mode!"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1182_112_2_18_0_26_34_71,(uint8*)"");
				}
				else
				{
					proc_count_HighMode = 0;
				}
			}
	       }
	}
	else
	{
		speedmode = NORMAL_SPEED_MODE;
		retry = 0;
		if(0 == proc_count_NorMode)
		{
	       	time1 = SCI_GetTickCount();
		     	proc_count_NorMode++;
		}
		else
		{
			time2 = SCI_GetTickCount();
	        	proc_count_NorMode++;
			if(time2 - time1 >= 10000)
			{
				if(proc_count_NorMode > 2)
				{
					//SCM_CFG_PRINT:"SCM ErrProc too many in normal mode! FALSE!"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1208_112_2_18_0_26_34_72,(uint8*)"");
					return FALSE;
				}
				else
				{
					proc_count_NorMode = 0;
				}
			}
	       }
	}
	
	do
	{
		//SCM_CFG_PRINT:"SCM ErrProc...... speedmode = 0x%x, retry = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1221_112_2_18_0_26_34_73,(uint8*)"dd", speedmode, retry);
		
		switch (cmd)
		{

			case SCM_FUNC_REINIT:
			{
				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle1, speedmode))
					{
						return TRUE;
					}
				}
			}
				break;
				
			case SCM_FUNC_READ:
			{
				BOOLEAN result;
		//---
				if(1 == param->readParam.num)
				{
					result = CARD_SDIO_ReadSingleBlock(s_cardHandle1, param->readParam.startBlock, param->readParam.buf);
				}
				else
				{
					result = CARD_SDIO_ReadMultiBlock(s_cardHandle1, param->readParam.startBlock, param->readParam.num, param->readParam.buf);
				}
				if(TRUE == result)
				{
					return TRUE;
				}

				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle1, speedmode))
					{
						if(1 == param->readParam.num)
						{
							result = CARD_SDIO_ReadSingleBlock(s_cardHandle1, param->readParam.startBlock, param->readParam.buf);
						}
						else
						{
							result = CARD_SDIO_ReadMultiBlock(s_cardHandle1, param->readParam.startBlock, param->readParam.num, param->readParam.buf);
						}
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
		//---
			}
				break;
				
			case SCM_FUNC_WRITE:
			{
				BOOLEAN result;
		//---
				if(1 == param->writeParam.num)
				{
					result = CARD_SDIO_WriteSingleBlock(s_cardHandle1, param->writeParam.startBlock, param->writeParam.buf);
				}
				else
				{
					result = CARD_SDIO_WriteMultiBlock(s_cardHandle1, param->writeParam.startBlock, param->writeParam.num, param->writeParam.buf);
				}
				if(TRUE == result)
				{
					return TRUE;
				}

				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle1, speedmode))
					{
						if(1 == param->writeParam.num)
						{
							result = CARD_SDIO_WriteSingleBlock(s_cardHandle1, param->writeParam.startBlock, param->writeParam.buf);
						}
						else
						{
							result = CARD_SDIO_WriteMultiBlock(s_cardHandle1, param->writeParam.startBlock, param->writeParam.num, param->writeParam.buf);
						}
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
		//---
			}
				break;
				
			case SCM_FUNC_ERASE:
			{
				return FALSE;
			}
			//	break; for pclint
				
			case SCM_FUNC_GETCAPCITY:
			{
				return FALSE;
			}
			//	break; for pclint
				
#if defined(SPRD_SUPPORT_MCEX)
			case SCM_FUNC_ENTERMCEX:
			{
				BOOLEAN result;

				result = CARD_SDIO_EnterMcex(s_cardHandle1,param->enterMcex.ifEnterMcex);
				if(TRUE == result)
				{
					return TRUE;
				}
				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle1, speedmode))
					{
						result = CARD_SDIO_EnterMcex(s_cardHandle1,param->enterMcex.ifEnterMcex);
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
			}
				break;
				
			case SCM_FUNC_RESETTRM:
			{
				BOOLEAN result;

				result = CARD_SDIO_CtlTrm(s_cardHandle1);
				if(TRUE == result)
				{
					return TRUE;
				}
				for(i = 0; i < 1; i++)
				{
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle1, speedmode))
					{
						result = CARD_SDIO_CtlTrm(s_cardHandle1);
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
			}
				break;
			
			case SCM_FUNC_READ_REC_CMD:
			case SCM_FUNC_WRITE_REC_CMD:
			case SCM_FUNC_SEND_PSI:
			{
				for(i = 0; i < 1; i++)
				{
					BOOLEAN result;
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWROFF);
					SDIO0_Pwr(SCM_SLOT_1,SLOT_PWRON);
					if(TRUE == CARD_SDIO_InitCard(s_cardHandle1, speedmode))
					{
						result = CARD_SDIO_EnterMcex(s_cardHandle1,param->enterMcex.ifEnterMcex);
						if(TRUE == result)
						{
							return TRUE;
						}
					}
				}
			}
				break;
#endif
			default:
		        //SCI_ASSERT(0);
			    //SCI_TRACE_LOW:"[DRV_SCM]<ERROR> val:cmd is wrong\r\n"
			    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1408_112_2_18_0_26_35_74,(uint8*)"" );
				return FALSE;	/*lint !e527*/
				//	break; for pclint

		}

		//SCM_CFG_PRINT:"SCM ErrProc failed! speedmode = 0x%x, retry = 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1414_112_2_18_0_26_35_75,(uint8*)"dd", speedmode, retry);
		speedmode = NORMAL_SPEED_MODE;
		change_mode1 = TRUE;
		
		if(retry > 0)
		{
			SDIO0_Pwr(SCM_SLOT_1,SLOT_PWROFF);
			SDIO0_Pwr(SCM_SLOT_1,SLOT_PWRON);
			if(FALSE == CARD_SDIO_InitCard(s_cardHandle1, speedmode))
			{
				return FALSE;
			}
		}
		
	}while(retry--);

	return FALSE;				
}

#endif

#ifdef DUAL_TCARD_SUPPORT
#define SCM_SLOT_NUM 2
#else
#define SCM_SLOT_NUM 1
#endif

PUBLIC uint32 g_slotInformationNum = SCM_SLOT_NUM;
SCM_SLOT_DESCRIPTION_T g_slotInformation[SCM_SLOT_NUM] =
{
	{
		SCM_SLOT_0,
		slot0_OpenProtocol,
		slot0_CloseProtocol,
		slot0_Init,
		slot0_IsExist,
		SDIO0_Pwr,

		slot0_Read,
		slot0_Write,
		slot0_ReadExt,
		slot0_WriteExt,
		slot0_ERASE,
		slot0_GetCapcity,
#if defined(SPRD_SUPPORT_MCEX)
		slot0_EnterMcex,
		slot0_ResetTrm,
		slot0_SendPsi,
		slot0_ReadSecCmd,
		slot0_WriteSecCmd,
		slot0_ExitMcex,//need merge
#endif
		slot0_ErrProc,

		SCM_SDIO_0
	}

#ifdef DUAL_TCARD_SUPPORT
	,
	{
		SCM_SLOT_1,
		slot1_OpenProtocol,
		slot1_CloseProtocol,
		slot1_Init,
		slot1_IsExist,
		SDIO0_Pwr,	//must be same with slot0

		slot1_Read,
		slot1_Write,
		slot1_ReadExt,
		slot1_WriteExt,
		slot1_ERASE,
		slot1_GetCapcity,
#if defined(SPRD_SUPPORT_MCEX)
		slot1_EnterMcex,
		slot1_ResetTrm,
		slot1_SendPsi,
		slot1_ReadSecCmd,
		slot1_WriteSecCmd,
		slot1_ExitMcex,//need merge
#endif
		slot1_ErrProc,

		SCM_SDIO_0	//must be same with slot0
	}
#endif

};
#endif



#if 1 //used for BSD config
/*****************************************/
LOCAL BSD_STATUS Slot0_ReadSector(     // return result. BSD_SUCCESS - success
                                                                    //                 BSD_FAILURE - failure
    BSD_DEVICE *hDev,   // device occupied by SDCard
    uint32 uiOffset,        // the block number
    uint32 uiNumber,    // the read counts
    void* pucBuf       // read buffer
    )
{
	SCM_PARAM_T param;

	param.readParam.startBlock = uiOffset;
	param.readParam.num = uiNumber;
	param.readParam.buf = (uint8*)pucBuf;
	if(TRUE == SCM_IOCTL(SCM_SLOT_0,SCM_FUNC_READ,&param))
	{
		return BSD_SUCCESS;
	}
	else
	{
		//SCM_CFG_PRINT:"SCM Read fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1526_112_2_18_0_26_35_76,(uint8*)"");
		return BSD_ERROR;
	}

}

LOCAL BSD_STATUS Slot0_WriteSector(     // return result. BSD_SUCCESS - success
                                                                    //                 BSD_FAILURE - failure
    BSD_DEVICE *hDev,   // device occupied by SDCard
    uint32 uiOffset,        // the block number
    uint32 uiNumber,    // the read counts
    const void* pucBuf       // read buffer
    )
{
	SCM_PARAM_T param;

	param.writeParam.startBlock = uiOffset;
	param.writeParam.num = uiNumber;
	param.writeParam.buf = (uint8*)pucBuf;
	if(TRUE == SCM_IOCTL(SCM_SLOT_0,SCM_FUNC_WRITE,&param))
	{
		return BSD_SUCCESS;
	}
	else
	{
		//SCM_CFG_PRINT:"SCM Write fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1551_112_2_18_0_26_35_77,(uint8*)"");
		return BSD_ERROR;
	}

}

LOCAL BSD_STATUS Slot0_DeleteSector(                  // return result. BSD_SUCCESS - success
                                                                    //                 BSD_FAILURE - failure
    BSD_DEVICE *hDev,       // the partition id occupied by SDCard
    uint32 uiOffset,            //  the block number
    uint32 uiNumber         // erased counts
    )
{
	SCM_PARAM_T param;

	param.eraseParam.startBlock = uiOffset;
	param.eraseParam.endBlock = uiOffset+uiNumber-1;
	if(TRUE == SCM_IOCTL(SCM_SLOT_0,SCM_FUNC_ERASE,&param))
	{
		return BSD_SUCCESS;
	}
	else
	{
		//SCM_CFG_PRINT:"SCM Erase fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1574_112_2_18_0_26_35_78,(uint8*)"");
		return BSD_ERROR;
	}

}

LOCAL uint32 Slot0_GetCapacity(void)
{
	SCM_PARAM_T param;

	if(TRUE == SCM_IOCTL(SCM_SLOT_0,SCM_FUNC_GETCAPCITY,&param))
	{
		return param.getCapcityParam.capcity;
	}
	else
	{
		//SCM_CFG_PRINT:"SCM GetCap fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1590_112_2_18_0_26_35_79,(uint8*)"");
		return 0;
	}
}
#endif


#ifdef DUAL_TCARD_SUPPORT
LOCAL BSD_STATUS Slot1_ReadSector(     // return result. BSD_SUCCESS - success
                                                                    //                 BSD_FAILURE - failure
    BSD_DEVICE *hDev,   // device occupied by SDCard
    uint32 uiOffset,        // the block number
    uint32 uiNumber,    // the read counts
    void* pucBuf       // read buffer
    )
{
	SCM_PARAM_T param;

	param.readParam.startBlock = uiOffset;
	param.readParam.num = uiNumber;
	param.readParam.buf = (uint8*)pucBuf;
	if(TRUE == SCM_IOCTL(SCM_SLOT_1,SCM_FUNC_READ,&param))
	{
		return BSD_SUCCESS;
	}
	else
	{
		//SCM_CFG_PRINT:"SCM Read fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1617_112_2_18_0_26_35_80,(uint8*)"");
		return BSD_ERROR;
	}

}

LOCAL BSD_STATUS Slot1_WriteSector(     // return result. BSD_SUCCESS - success
                                                                    //                 BSD_FAILURE - failure
    BSD_DEVICE *hDev,   // device occupied by SDCard
    uint32 uiOffset,        // the block number
    uint32 uiNumber,    // the read counts
    const void* pucBuf       // read buffer
    )
{
	SCM_PARAM_T param;

	param.writeParam.startBlock = uiOffset;
	param.writeParam.num = uiNumber;
	param.writeParam.buf = (uint8*)pucBuf;
	if(TRUE == SCM_IOCTL(SCM_SLOT_1,SCM_FUNC_WRITE,&param))
	{
		return BSD_SUCCESS;
	}
	else
	{
		//SCM_CFG_PRINT:"SCM Write fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1642_112_2_18_0_26_35_81,(uint8*)"");
		return BSD_ERROR;
	}

}

LOCAL BSD_STATUS Slot1_DeleteSector(                  // return result. BSD_SUCCESS - success
                                                                    //                 BSD_FAILURE - failure
    BSD_DEVICE *hDev,       // the partition id occupied by SDCard
    uint32 uiOffset,            //  the block number
    uint32 uiNumber         // erased counts
    )
{
	SCM_PARAM_T param;

	param.eraseParam.startBlock = uiOffset;
	param.eraseParam.endBlock = uiOffset+uiNumber-1;
	if(TRUE == SCM_IOCTL(SCM_SLOT_1,SCM_FUNC_ERASE,&param))
	{
		return BSD_SUCCESS;
	}
	else
	{
		//SCM_CFG_PRINT:"SCM Erase fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1665_112_2_18_0_26_35_82,(uint8*)"");
		return BSD_ERROR;
	}

}

LOCAL uint32 Slot1_GetCapacity(void)
{
	SCM_PARAM_T param;

	if(TRUE == SCM_IOCTL(SCM_SLOT_1,SCM_FUNC_GETCAPCITY,&param))
	{
		return param.getCapcityParam.capcity;
	}
	else
	{
		//SCM_CFG_PRINT:"SCM GetCap fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1681_112_2_18_0_26_35_83,(uint8*)"");
		return 0;
	}
}

#endif

PUBLIC BOOLEAN SCM_BSD_REG(SCM_SLOT_NAME_E slotNO)
{
	BSD_DEVICE dev;
	uint32 capcity;

	//SCM_CFG_PRINT:"SCM_BSD_REG"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1693_112_2_18_0_26_35_84,(uint8*)"");

	if(SCM_SLOT_0 == slotNO)
	{
		if(SCM_STATUS_NORMAL != SCM_GetSlotStatus(SCM_SLOT_0))
		{
			//SCM_CFG_PRINT:"SCM slot0 status not normal! Error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1699_112_2_18_0_26_35_85,(uint8*)"");
			return FALSE;
		}

		capcity = Slot0_GetCapacity();
		if(0 == capcity)
		{
			//SCM_CFG_PRINT:"SCM slot0 capacity is 0! Error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1706_112_2_18_0_26_35_86,(uint8*)"");
			return SCI_FALSE;
		}
		
		SCI_MEMSET(&dev, 0x00, sizeof(dev));;
		strcpy(dev.Name, STL_SD_DEVICE_NAME);
		dev.Type = BSD_SD;
		dev.Attribute = BSD_DEVICE_NORMAL;
		dev.SectorSize = 512;
		dev.EraseUnitSize = 1;
		dev.TotScts = capcity;
		dev.Offset = 0;//partition start address
		dev.Read = Slot0_ReadSector;
		dev.Write = Slot0_WriteSector;
		dev.Erase = Slot0_DeleteSector;
		dev.DeviceExtension = NULL;

		if(BSD_SUCCESS == BSD_AddDevice(&dev))
		{
			return SCI_TRUE;
		}
	}
#ifdef DUAL_TCARD_SUPPORT    
	else if(SCM_SLOT_1 == slotNO)
	{
		if(SCM_STATUS_NORMAL != SCM_GetSlotStatus(SCM_SLOT_1))
		{
			//SCM_CFG_PRINT:"SCM slot1 status not normal! Error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1733_112_2_18_0_26_35_87,(uint8*)"");
			return FALSE;
		}

		capcity = Slot1_GetCapacity();
		if(0 == capcity)
		{
			//SCM_CFG_PRINT:"SCM slot1 capacity is 0! Error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1740_112_2_18_0_26_35_88,(uint8*)"");
			return SCI_FALSE;
		}
		
		SCI_MEMSET(&dev, 0x00, sizeof(dev));;
		strcpy(dev.Name, STL_SD_1_DEVICE_NAME);
		dev.Type = BSD_SD;
		dev.Attribute = BSD_DEVICE_NORMAL;
		dev.SectorSize = 512;
		dev.EraseUnitSize = 1;
		dev.TotScts = capcity;
		dev.Offset = 0;//partition start address
		dev.Read = Slot1_ReadSector;
		dev.Write = Slot1_WriteSector;
		dev.Erase = Slot1_DeleteSector;
		dev.DeviceExtension = NULL;

		if(BSD_SUCCESS == BSD_AddDevice(&dev))
		{
			return SCI_TRUE;
		}
	}
#endif    
	else
	{
		//SCM_CFG_PRINT:"SCM slot name error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1765_112_2_18_0_26_35_89,(uint8*)"");
	}
	
	return SCI_FALSE;
}


PUBLIC BOOLEAN SCM_BSD_UNREG(SCM_SLOT_NAME_E slotNO)
{
	//SCM_CFG_PRINT:"SCM_BSD_UNREG"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1774_112_2_18_0_26_35_90,(uint8*)"");

	if(SCM_SLOT_0 == slotNO)
	{	
		if (BSD_SUCCESS == BSD_RemoveDevice(STL_SD_DEVICE_NAME, TRUE))
		{
			return SCI_TRUE;
		}
	}
#ifdef DUAL_TCARD_SUPPORT    
	else if(SCM_SLOT_1 == slotNO)
	{
		if (BSD_SUCCESS == BSD_RemoveDevice(STL_SD_1_DEVICE_NAME, TRUE))
		{
			return SCI_TRUE;
		}
	}
#endif    
	else
	{
		//SCM_CFG_PRINT:"SCM slot name error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_CONFIG_1794_112_2_18_0_26_35_91,(uint8*)"");
	}
	
	return SCI_FALSE;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif

