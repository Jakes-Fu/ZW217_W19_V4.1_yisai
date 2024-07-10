/******************************************************************************
 ** File Name:    sc6531efm_mem_cfg.c                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         07/27/2010                                                  *
 ** Copyright:    2010  Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **07/27/2010     Steve.zhan      Create.                                     *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "sci_types.h"
#include "uix8910_reg_base.h"
#include "../../../MS_Ref/export/inc/iram_mgr.h"
#include "uix8910_mem_cfg.h"
#include "mem_dump.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                        Local variable definition                           **
**----------------------------------------------------------------------------*/
const MEMORY_ACCESS_INFO_T s_memory_acc_table[ ] =
{
    /*------------------------------------------------------------------------------------------------------------
    *start_address              end_address                 acc_condtion                                memory name
    ----------------------------------------------------------------------------------------------------------------*/
    {0x00000000,          0xFFFFFFFF, (READ_ACC|WRITE_ACC|BYTE_ACC|WORD_ACC|DWORD_ACC) },             //the RAM memory
};


LOCAL const IRAM_AREA g_init_iram_area[] = {0x40000000, 0x00002800, SCI_NULL, IRAM_ATTRIBUTE_NORMAL};

/*
    On Chip RAM for DSP 
*/
LOCAL const IRAM_AREA g_iram_dsp[] = {
                                        {0x4001B000, 0x00008000, SCI_NULL, IRAM_ATTRIBUTE_DSP},
                                        {0x40023000, 0x0000C000, SCI_NULL, IRAM_ATTRIBUTE_DSP}
                                     };


// layout the const to global var, so that it can be modifed 
PUBLIC CTL_REG_INFO_T g_CtlRegInfoTab[]=
{
    	{(uint32)&g_CtlRegInfoTab[0],((uint32)&g_CtlRegInfoTab[0]+1024)},  //sizeof(g_CtlRegInfoTab) compiler error,temporarily specified the size to 1024
//AON&RF PWR DOMAIN    	
    {MODEM_RISCV_START, MODEM_RISCV_END  },
   	{MODEM_RFREG_START, MODEM_RFREG_END },   
   	{MODEM_SYSREG_START,MODEM_SYSREG_END},
   	{MODEM_CLKRST_START,MODEM_CLKRST_END},
	{MODEM_MONITOR_START, MODEM_MONITOR_END},
	{MODEM_WD_START, MODEM_WD_END},
	{MODEM_IDLE_START, MODEM_IDLE_END},
	{MODEM_PWRCTRL_START, MODEM_PWRCTRL_END },
	{MODEM_SYSCTRL_START, MODEM_SYSCTRL_END },
	{MODEM_MAILBOX_START , MODEM_MAILBOX_END},	
	{MODEM_TIMER3_START , MODEM_TIMER3_END},		
	{MODEM_ANALOG_REG_START , MODEM_ANALOG_REG_END},	

//AP PWR DOMAIN
   	{MODEM_GIC_START,  MODEM_GIC_END },
	{MODEM_IRQ_START,  MODEM_IRQ_END },
	{MODEM_IRQ1_START,  MODEM_IRQ1_END },
	{MODEM_TIMER1_START,  MODEM_TIMER1_END },
	{MODEM_TIMER2_START,  MODEM_TIMER2_END },
	{MODEM_TIMER4_START,  MODEM_TIMER4_END },
	{MODEM_SCI1_START,  MODEM_SCI1_END },
    {MODEM_SCI2_START,  MODEM_SCI2_END},   
	{MODEM_F8_START,  MODEM_F8_END },
	{MODEM_AXIDMA_START,MODEM_AXIDMA_END},
	{MODEM_SPIFLASH_START,MODEM_SPIFLASH_END},
	{MODEM_SPIFLASH1_START,MODEM_SPIFLASH1_END},
	{MODEM_EFS_START,MODEM_EFS_END},
	{MODEM_GPIO_START, MODEM_GPIO_END},
	
//WCN PWR DOMAIN
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
	{BT_RISCV_RAM_START,BT_RISCV_RAM_END},    	
	{BT_RISCV_EM_START,BT_RISCV_EM_END},
#endif
//ZSP PWR DOMAIN
   	{MODEM_ZAXIDMA_START, MODEM_ZAXIDMA_END },
    {MODEM_ZITCM_START, MODEM_ZITCM_END },
    {MODEM_ZDTCM_START, MODEM_ZDTCM_END },

//LTE PWR DOMAIN
   	{MODEM_RXTX_START, MODEM_RXTX_END },
   	{MODEM_RFADRAM_START, MODEM_RFADRAM_END },
   	{MODEM_COEFF_START , MODEM_COEFF_END },
   	{MODEM_LDTC_START, MODEM_LDTC_END },
   	{MODEM_OTDOA_START, MODEM_OTDOA_END} ,
   	{MODEM_MEASPWR_START, MODEM_MEASPWR_END },
   	{MODEM_IDDET_START , MODEM_IDDET_END },
   	{MODEM_ULFFT_START, MODEM_ULFFT_END },
   	{MODEM_PUSCH_START, MODEM_PUSCH_END },
   	{MODEM_ULPCDCI_START, MODEM_ULPCDCI_END },
   	{MODEM_DLFFT_START , MODEM_DLFFT_END  },
   	{MODEM_CSIRS_START, MODEM_CSIRS_END },
   	{MODEM_LDTC1_START , MODEM_LDTC1_END },
   	{MODEM_RXCAP_START, MODEM_RXCAP_END },

//GGE PWR DOMAIN
   	{MODEM_BBAPB_START, MODEM_BBAPB_END },
   	{BCPU_SRAM_BEGIN, BCPU_SRAM_BEGIN+BCPU_SRAM_SIZE },
    {MODEM_BCPUROM_START,MODEM_BCPUROM_END},
	
    {0xffffffff,0xffffffff}
};

/**----------------------------------------------------------------------------*
**                        Global variable definition                          **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC MEMORY_ACCESS_INFO_T *Mem_getAccTable (uint32 *pSize)
{
    *pSize = sizeof (s_memory_acc_table) /sizeof (s_memory_acc_table[0]);

    return (MEMORY_ACCESS_INFO_T *) s_memory_acc_table;
}

/*****************************************************************************/
//  Description:    Get IRAM information used by DSP
//  Global resource dependence:
//  Author:
//  Note:           
/*****************************************************************************/
PUBLIC IRAM_AREA* MEM_GetIRAMDsp (uint32 *count)
{
    // return registered area num by input param
    *count = sizeof(g_iram_dsp[0]) /sizeof (IRAM_AREA);

    return ( (IRAM_AREA *) g_iram_dsp);
}

/*****************************************************************************/
//  Description:    Get IRAM information used by ARM
//  Global resource dependence:
//  Author:
//  Note:           return num by input param and return the const array pointer
/*****************************************************************************/
PUBLIC IRAM_AREA *GetIRAMSet (uint32 *count)
{
    // return registered area num by input param
    *count = sizeof (g_init_iram_area) /sizeof (IRAM_AREA);

    return ( (IRAM_AREA *) g_init_iram_area);
}

/*****************************************************************************/
// Description :    This function is used to get the number of control registers group.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 CHIP_GetNumCtlRegGrp (void)
{
    uint32 CtlRegInfoTabSize = sizeof (g_CtlRegInfoTab) / sizeof (g_CtlRegInfoTab[0]);
    return CtlRegInfoTabSize - 1;
}

/*****************************************************************************/
// Description :    This function is used to get the infomation of control
//                  registers,including the base address,the length and the
//                  name of External Memory Control Registers and AHB General
//                  Control Registers,DMA Control Register,Digital Camera Space,
//                  USB Device Space,NAND Flash Control and LCM Control Space,
//                  Peripheral Address Space.
// Global resource dependence :g_CtlRegInfoTab
// Author :         kevin.wang
// Note : the unit of length is a word(32bit).
/*****************************************************************************/
PUBLIC void const *CHIP_GetInfoOfCtlReg (void)
{
    return (void *) g_CtlRegInfoTab;
}
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
