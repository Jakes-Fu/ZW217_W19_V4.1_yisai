/******************************************************************************
 ** File Name:      TF_load.c                                               *
 ** Author:         tao.feng                                              *
 ** DATE:           16/01/2011                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 16/01/2011         tao.feng          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "tf_load.h"
#include "sci_types.h"
#include "lcm_cfg.h"
#include "tb_hal.h"
#include "arm_reg.h"
#include "ref_outport.h"
#include "tf_cfg.h"
#include "mmu_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro  Definition                                     *
 **---------------------------------------------------------------------------*/
 /*arm diable int bit control*/
#define TF_LOAD_DISABLE_INT		    0xc0
/*tfboot.bin load address, it is defined in XXXX_project.scf, in most cases no need to change this definition*/
#define TF_LOAD_BASE_ADDR           0x800
/*notice: define sram start address 0x04010000, not  0x04000000, avoid conflict with sys stack when call copy func
and jump func, after enter tfboot.bin, we can safly use sram address start with 0x04000000, in most cases no need
to change this definition*/
#define  TF_SDRAM_START_ADDR	           0x010000
#define  TF_SRAM_START_ADDR		           0x80000000

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 // KEYPAD Interface Control.
typedef struct kpd_tag
{
    VOLATILE uint32    ctl;
    VOLATILE uint32    int_en;
    VOLATILE uint32    int_raw_status;
    VOLATILE uint32    int_mask_status;
    VOLATILE uint32    int_clr;
    VOLATILE uint32    reserved;    ///reserved
    VOLATILE uint32    polarity;
    VOLATILE uint32    debounce_cnt;
    VOLATILE uint32    long_key_cnt;
    VOLATILE uint32    sleep_cnt;
    VOLATILE uint32    clk_divide_cnt;
    VOLATILE uint32    key_status;
    VOLATILE uint32    sleep_status;
    VOLATILE uint32    debug_status_1;  //only for debug
    VOLATILE uint32    debug_status_2;  //only for debug
} kpd_s;

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL  TF_UPDATE_INFO_PTR s_tf_update_info_ptr=PNULL;

#ifdef TF_LOAD_PROTECT
#pragma arm section rodata = "TF_BOOTCODE"
LOCAL __align(4) const uint8 tfboot[] =
#include "tfboot.h"
#pragma arm section code
#endif


/**---------------------------------------------------------------------------*
 **                         extern func                                *
 **---------------------------------------------------------------------------*/
extern void TF_JumpToTarget(uint32 addr);
extern unsigned int  _tx_thread_interrupt_control(unsigned int);
#ifndef NANDBOOT_SUPPORT
#ifndef FLASH_TYPE_SPI
PUBLIC void FLASH_GetID (uint32 *pManu_id, uint32 *pDev_id, uint32 *pExtend_id);
#endif
void FLASH_GetFixedNvitemAddr (uint32 *start_addr, uint32 *end_addr);
#endif
/**---------------------------------------------------------------------------*
 **                          Functions    Definition                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Function name:	TF_ExitSys
//  Description:This function is used to exit system
//  Global resource dependence1:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL  void TF_ExitSys(void)
{
	//uint32 i=0;
	//stop watchdog timer
	HAL_WDG_STOP();
	// disable the fiq and irq.
	_tx_thread_interrupt_control(TF_LOAD_DISABLE_INT);
	//*(volatile uint32 *)(INT_FIQ_DISABLE) = 0xFFFFFFFF;
	//*(volatile uint32 *)(INT_IRQ_DISABLE) = 0xFFFFFFFF;
	SCI_DisableIRQ();
	SCI_DisableFIQ();

	MMU_CacheOff(TRUE, TRUE);
    MMU_UseNorDOM();	//Set MMU to Manager Access Permission
    //MMU_Disable();
	//WAIT dma transfor done
	//while((*(volatile uint32*)DMA_CFG)  & BIT_3);
	//for(i=0x0; i<0x1000; i++);
	OS_TickDelay(1000);
}


/*****************************************************************************/
//  Function name:	TF_ParamInit
//  Description:This function is used to fill TF_UPDATE_INFO whick will export to tf bootloader.bin
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL  void TF_ParamInit(void)
{
#ifdef NANDBOOT_SUPPORT
	s_tf_update_info_ptr = (TF_UPDATE_INFO *)TFLOAD_SHARE_RAM_ADDRESS;
	s_tf_update_info_ptr->tf_update_magic = TF_UPDATE_TYPE_MAGIC;
	/*to be implemented*/
#else

	uint32 fixnv_start_addr, fixnv_end_addr;

#ifndef FLASH_TYPE_SPI
    uint32 manu_id, dev_id, ext_id;
	FLASH_GetID(&manu_id, &dev_id, &ext_id);
	s_tf_update_info_ptr->tf_update_flash.manu_id   = manu_id;
	s_tf_update_info_ptr->tf_update_flash.dev_id      = dev_id;
	s_tf_update_info_ptr->tf_update_flash.ext_id       = ext_id;
#endif
	FLASH_GetFixedNvitemAddr(&fixnv_start_addr, &fixnv_end_addr);

	s_tf_update_info_ptr = (TF_UPDATE_INFO *)TFLOAD_SHARE_RAM_ADDRESS;
	s_tf_update_info_ptr->tf_update_flash.fixnv_addr = fixnv_start_addr;
	s_tf_update_info_ptr->tf_update_magic = TF_UPDATE_TYPE_MAGIC;
	/*to be implemented*/
#endif
}

/*****************************************************************************/
//  Function name:	TF_HwInit
//  Description:This function is used to hw init
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL  void TF_HwInit(void)
{
#ifdef PLATFORM_SC6800H
	*(volatile uint32 *)(GR_MISC0) |= BIT_27|BIT_28;	// 24KB XRAM switch to ARM
#endif
	/*to be implemented*/
}


/*****************************************************************************/
//  Function name:	TF_GetDiabsleAssertFlag
//  Description:This function is used to get allow disbale assert flasg
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN   TF_GetDiabsleAssertFlag(void)
{
	if(s_tf_update_info_ptr->tf_update_magic != TF_UPDATE_TYPE_MAGIC)
	{
		return SCI_FALSE;
	}
	else
	{
		return SCI_TRUE;
	}
}

/*****************************************************************************/
//  Function name:	TF_GetBootExecAddress
//  Description:This function is used to get tf bootloader.bin start exec address.
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 TF_GetBootExecAddress(void)
{
	uint32 load_base;

#ifdef NANDBOOT_SUPPORT
	load_base = TF_SDRAM_START_ADDR;
#else
	load_base = TF_SRAM_START_ADDR;
#endif
	return load_base;
}

/*****************************************************************************/
//  Function name:	TF_LoadInit
//  Description:This function is used to do tf update  init
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void  TF_LoadInit(void)
{
	TF_ExitSys();
	TF_ParamInit();
	TF_HwInit();
}

/*****************************************************************************/
//  Function name:	TF_Jump
//  Description:This function is used to jump to target
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void  TF_Jump(void)
{
	TF_JumpToTarget(TF_GetBootExecAddress());
}

/*****************************************************************************/
//  Discription:TF  load string compare
//  Global resource dependence: none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int TF_LoadWstrCMP(
                             const unsigned short * string1,
                             const unsigned short * string2
                             )
{
	int ret = 0 ;

	if( string1 == PNULL && string2 == PNULL )
	{
		return 0;
	}
	else if( string1 == PNULL && string2 != PNULL )
	{
		return -1;
	}
	else if( string1 != PNULL && string2 == PNULL )
	{
		return 1;
	}
	else
	{
		/*lint -save -e613 */
		ret = *(unsigned short *)string1 - *(unsigned short *)string2;/*lint !e613 */

		while( (!ret) && *string2 && *string1)/*lint !e613 */
		{
			++string1;/*lint !e613 */
			++string2;
			ret = *(unsigned short *)string1 - *(unsigned short *)string2;
		}
		/*lint -restore */
		return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
	}
}

#ifdef TF_LOAD_PROTECT
/*
	Important notice: Behind funcs not allowed  to call system func!!!
*/
/*****************************************************************************/
//  Function name:	TF_Delay
//  Description:This function is used to delay somtime
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void TF_Delay(uint32 ms)
{
	uint32 i;
	for(i=0; i<ms*2000; i++) {}
}

/*****************************************************************************/
//  Function name:	TF_EnterVerify
//  Description:This function is used to enter TF hot update verify.
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN  TF_EnterVerify(void)
{
	uint32 i;
	kpd_s   *kpd_ptr = (kpd_s *)(KPD_BASE);

#ifdef PLATFORM_SC8800G
	//Reset Keypad
	*(volatile uint32 *)(GR_SOFT_RST) |= BIT_1;
	//Delay some time
	 for(i=0;i<10;i++){}
	//exist keypad soft reset
	*(volatile uint32 *)(GR_SOFT_RST) &= ~BIT_1;
	//Enable KPD
	*(volatile uint32 *)(GR_GEN0) |= BIT_8|BIT_26;
#endif

#ifdef PLATFORM_SC6800H//6800H:
	//Reset Keypad
	*(volatile uint32 *)(GR_SOFT_RST) |= BIT_0;
	//Delay some time
	 for(i=0;i<10;i++) {};
	//exist keypad soft reset
	*(volatile uint32 *)(GR_SOFT_RST) &= ~BIT_0;
	//Enable KPD
	*(volatile uint32 *)(GR_GEN0) |= BIT_5|BIT_10;
#endif

#ifdef PLATFORM_SC6530//6800H:
	//Reset Keypad
	*(volatile uint32 *)(APB_RST0_SET) |= BIT_0;
	//Delay some time
	 for(i=0;i<10;i++) {};
	//exist keypad soft reset
	*(volatile uint32 *)(APB_RST0_CLR) |= BIT_0;
	//Enable KPD
	*(volatile uint32 *)(APB_EB0_SET) |= BIT_6|BIT_19;
#endif


#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E) //6800H:
	//Reset Keypad
	*(volatile uint32 *)(APB_RST0_SET) |= BIT_0;
	//Delay some time
	 for(i=0;i<10;i++) {};
	//exist keypad soft reset
	*(volatile uint32 *)(APB_RST0_CLR) |= BIT_0;
	//Enable KPD
	*(volatile uint32 *)(APB_EB0_SET) |= BIT_6|BIT_19;
#endif

	//Clear Keypad interrupt
	kpd_ptr->int_clr = 0xFFF;
	//Set Keypad row colomn polarity
	kpd_ptr->polarity = 0xFFFF;
	//Set Keypad clock div
	kpd_ptr->clk_divide_cnt = 0x0;
	//Set Keypad debounce time
	kpd_ptr->debounce_cnt = 0xf;
	//Enable Keypad module
	kpd_ptr->ctl |= BIT_0;
	//Wait 20ms for KPD debouncing
	//TF_Delay(20); // waste about 4s in 6530 during power on

	//If key in1_out1 & key_in2_out2 is pressed, enter
	if((kpd_ptr->int_raw_status & 0x3) &&\
			((((kpd_ptr->key_status & 0x77) == 0x11) && ((kpd_ptr->key_status & (0x77<<8)) == (0x22<<8))) ||\
				(((kpd_ptr->key_status & 0x77) == 0x22) && ((kpd_ptr->key_status & (0x77<<8)) == (0x11<<8)))))
	{
		//Clear Keypad interrupt
		kpd_ptr->int_clr = 0xFFF;
#ifdef PLATFORM_SC8800G
		//Disable KPD
		*(volatile uint32 *)(GR_GEN0) &= ~BIT_8;
#endif

#ifdef PLATFORM_SC6800H//6800H:
		*(volatile uint32 *)(GR_GEN0) &= ~BIT_5;
#endif

#ifdef PLATFORM_SC6530//6800H:
		*(volatile uint32 *)(APB_EB0_CLR) = BIT_6;
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		*(volatile uint32 *)(APB_EB0_CLR) = BIT_6;
#endif

		return SCI_TRUE;
	}

	return SCI_FALSE;
}


/*****************************************************************************/
//  Function name:	TF_CopyBin2Ram
//  Description:This function is used to copy bin to sram
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL  void  TF_CopyBin2Ram(void)
{
	uint32 i, base_addr;
	uint8 *pTfboot = (uint8 *)tfboot;
	uint32 value = *(uint32 *)tfboot;

	base_addr = TF_GetBootExecAddress();
	if(value != 0x0)
	{
		for(i=0x0; i<sizeof(tfboot)/sizeof(tfboot[0]); i++)
		{
			*(volatile uint8*)(base_addr +i) = *(volatile uint8*)(pTfboot + i);
		}
	}
}

/*****************************************************************************/
//  Function name:	TF_EnterInit
//  Description:This function is used to exit system
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC  void   TF_EnterInit(void)
{
	BOOLEAN ret = SCI_FALSE;

	if(s_tf_update_info_ptr->tf_update_magic != TF_UPDATE_TYPE_MAGIC)
	{
		/*开机时进入此流程判断*/
		ret = TF_EnterVerify();
	}
	else
	{
		/*工程模式中进入此模式*/
		ret = SCI_TRUE;
	}

	if(ret)
	{
		//tf hot update start!!!
		//TF_CopyBin2Ram();
		{
	uint32 i, base_addr;
	uint8 *pTfboot = (uint8 *)tfboot;
	uint32 value = *(uint32 *)tfboot;

	base_addr = TF_GetBootExecAddress();
	if(value != 0x0)
	{
		for(i=0x0; i<sizeof(tfboot)/sizeof(tfboot[0]); i++)
		{
			*(volatile uint8*)(base_addr +i) = *(volatile uint8*)(pTfboot + i);
		}
	}
}
		//while(1) {} /*lint !e716*/ //should never enter here!!!!
		TF_Jump();
		while(1) {} /*lint !e716*/ //should never enter here!!!!
	}
}
#endif


#ifdef   __cplusplus
    }

#endif  // End of tf_load.c


