/******************************************************************************
 ** File Name:      mem_prod.c                                                *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           2008-03-04                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the functions used by memory config     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/04/2008     jiexia.yu        Create.                                   *
 ** 11/03/2010    	fei.zhang	       Add DSP USB Log function       * 
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "threadx_os.h"
#include "os_api.h"
#include "sc_reg.h"
#include "chng_freq.h"
#include "mem_dump.h"
#include "mem_prod.h"
#include "mmu.h"/*lint -esym(766, chip_drv\export\inc\outdated\mmu.h)*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
 // NOR + SRAM Solution
#if defined SRAM_SIZE_16MBIT
#define SRAM_TOTAL_SIZE	   0x200000
#elif defined SRAM_SIZE_32MBIT
#define SRAM_TOTAL_SIZE    0x400000
#elif defined SRAM_SIZE_64MBIT
#define SRAM_TOTAL_SIZE    0x800000
#elif defined SRAM_SIZE_128MBIT
#define SRAM_TOTAL_SIZE    0x01000000
#elif defined SRAM_SIZE_256MBIT
#define SRAM_TOTAL_SIZE    0x02000000
#endif

#define MAX_MAIN_LCD_BUF_SIZE   ((((LCD_WIDTH+15)&~0xF)*((LCD_HEIGHT+7)&~0x7))*2)

#define LCD_ALIGN_256(_X_)  ((uint32)((_X_) + 255)& (~0xFF))
/**---------------------------------------------------------------------------*
 **                     External Variables Prototypes                         *
 **---------------------------------------------------------------------------*/
 #include "mem_cfg.c"

LOCAL uint8*  s_main_lcd_buffer_ptr = NULL;

/**---------------------------------------------------------------------------*
 **                     External Function Prototypes                         *
 **---------------------------------------------------------------------------*/
#ifndef BZP_SUPPORT
extern uint32 FLASH_GetMMIResPtr(void);
#endif
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Return practical start address of Demandon OS image partition
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC uint8* MEM_GetUserImageAddress(void)
{
#ifdef NANDBOOT_SUPPORT
	return ((uint8*)USER_IMAGE_BASE);
#else
	return 0;
#endif
}

PUBLIC BOOLEAN MEM_HasLoadImage(void)
{
#ifndef VM_LOAD_IMAGE
#define VM_LOAD_IMAGE TRUE
#endif
	return ((BOOLEAN)VM_LOAD_IMAGE);
}

/*****************************************************************************/
//  Description:    Return practical start address of Demandon OS image partition
//	Global resource dependence: 
//  Author:         
//	Note:            
/*****************************************************************************/
PUBLIC uint32 MEM_GetUserImageSize(void)
{
#ifdef NANDBOOT_SUPPORT
	return ((uint32)USER_IMAGE_LENGTH);	
#else
 	return 0; 	
#endif
}

#ifdef  BZP_SUPPORT
PUBLIC uint32 MEM_GetResBaseAddr (void)
{
    return  VIR_RESOURCE_STRAT_ADDR;
}
#endif

/*****************************************************************************/
//  Description:    PROD_GetResMemAdd
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided to MMI. 
/*****************************************************************************/
PUBLIC uint8* PROD_GetResMemAdd(void)
{
#ifdef  BZP_SUPPORT
#ifdef BZP_NO_RESOURCE
    extern  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config;	
    return s_platform_patitiion_config.mmi_res;
#else
    return (uint8*)MEM_GetResBaseAddr(); 
#endif    
#else
#ifdef BZP_NO_RESOURCE
    extern  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config;	
    return s_platform_patitiion_config.mmi_res;
#else
	return (uint8*)FLASH_GetMMIResPtr();
#endif
#endif	
}

#ifdef CHIP_VER_UWS6121E


PUBLIC uint32 PROD_GetApnMemSection(uint8** addr)
{
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    *addr = (uint8 *)s_platform_patitiion_config.apn_addr;
    return s_platform_patitiion_config.apn_size;
#elif defined (FLASH_SIZE_128MBITX64MBIT_NEW) || defined (FLASH_SIZE_128MBIT)
    return 0;
#endif
}

PUBLIC uint32 PROD_GetConfigNVMemSection(uint8** addr)
{
    *addr = (uint8 *)s_platform_patitiion_config.config_nv_addr;
    return s_platform_patitiion_config.config_nv_size;
}

PUBLIC uint32 PROD_GetImsMemSection(uint8** addr)
{
    *addr = (uint8 *)s_platform_patitiion_config.operator_data_addr;
    return s_platform_patitiion_config.operator_data_size;
}

PUBLIC uint32 PROD_GetImsToggleMemSection(uint8** addr)
{
    *addr = (uint8 *)SPIFLASH_GetImsToggelCfgAddress();
    return SPIFLASH_GetImsToggelCfgSize();
}

#endif


/*****************************************************************************/
//  Description:    Get the AP ram size 
//	Global resource dependence: 
//  Author:         guangqiao.she
//	Note:           This function is provided from product mem config .
/*****************************************************************************/
PUBLIC uint32 MEM_GetAPSramSize(void)
{
    return mem_ap_sram_size;
}

/*****************************************************************************/
//  Description:    The function is used to get base address of AP ram.
//	Global resource dependence: 
//  Author:        guangqiao.she
//	Note:           
/*****************************************************************************/
PUBLIC uint32 MEM_GetAPSramBaseAddr(void)
{
	return mem_ap_sram_start_add;
}


/*****************************************************************************/
//  Description:    The function is used to get base address of dump memory.
//	Global resource dependence: 
//  Author:         kevin.wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 MEM_GetDumpBaseAddr(void)
{
	return g_sram_start;
}


/*****************************************************************************/
//  Description:    The function is used to get length of dump memory.
//	Global resource dependence: 
//  Author:         kevin.wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 MEM_GetDumpLen(void)
{
	return g_sram_size;
}

#ifndef MODEM_PLATFORM
/*****************************************************************************/
//  Description:    MEM_GetMainLCDBuffer
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void* MEM_GetMainLCDBuffer(void)
{   
	return (void*)s_main_lcd_buffer;
}


PUBLIC void MEM_FreeMainLCDBuffer(void)
{
   // if(s_main_lcd_buffer_ptr)
    //{
    //   SCI_FREE(s_main_lcd_buffer_ptr);
    //}
}


/*****************************************************************************/
//  Description:    MEM_GetSubLCDBuffer
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void* MEM_GetSubLCDBuffer(void)
{
	return (void*)s_sub_lcd_buffer;
}
#endif


/*****************************************************************************/
//  Description:    This function is provided from product mem config .
//	Global resource dependence: 
//  Author:         jiexia.yu
//	Note:            
/*****************************************************************************/
PUBLIC BOOLEAN MEM_IsRWRegion(uint32 data_addr)
{
    BOOLEAN is_rw_region = SCI_FALSE;
    
    if(((data_addr >= mem_rw_start_add)&&(data_addr < (mem_rw_start_add+mem_rw_size))) ||
        ((data_addr >= mem_iram_start_add)&&(data_addr < (mem_iram_start_add+mem_iram_size))))
    {
        is_rw_region = SCI_TRUE;
    }
    
    return is_rw_region;
}

/*****************************************************************************/
//  Description:    This function is provided to check whether the scatter loading rw overflow the max ram size
//	Global resource dependence: 
//  Author:       
//	Note:            call in init to indicate user
/*****************************************************************************/
PUBLIC BOOLEAN MEM_IsRWRegionOverflow(void)
{
#if 0 /*realize this func later*/
    BOOLEAN ret = SCI_FALSE;
    
    if(mem_scatter_end_add > (mem_rw_start_add+  mem_rw_size))
    {
        ret = SCI_TRUE;
    }
    
    return ret;
#else
    return SCI_FALSE;
#endif
}

/*****************************************************************************/
//  Description:    This function is used to check RO Data section.
//	Global resource dependence: 
//  Author:         jiexia.yu
//	Note:            
/*****************************************************************************/
PUBLIC BOOLEAN MEM_IsRORegion(uint32 data_addr)
{
    BOOLEAN is_ro_region = SCI_FALSE;

    if(((data_addr >= mem_ro_start_add)&&(data_addr < (mem_ro_start_add + mem_ro_size))) ||   /*lint !e685 !e568*/
        ((data_addr >= mem_iram_start_add)&&(data_addr < (mem_iram_start_add + mem_iram_size))) ||
        ((data_addr >= mem_irom_start_add)&&(data_addr < (mem_irom_start_add + mem_irom_size))) ||

#ifdef MULTI_FLASH_SUPPORT        
        ((data_addr >= mem_ro2_start_add) && (data_addr < (mem_ro2_start_add + mem_ro2_size)))  ||
#endif

        ((data_addr >= mem_rw_start_add) && (data_addr < (mem_rw_start_add + mem_rw_size)))
#ifdef VM_SUPPORT
        || ((data_addr >= mem_vm_start_addr) && (data_addr < mem_vm_end_addr))
#endif
        )
    {
        is_ro_region = SCI_TRUE;
    }
    
    return is_ro_region;
}

#ifdef VM_SUPPORT

/*****************************************************************************/
//  Description:    This function is used to get the virture spac start address.
//	Global resource dependence: 
//  Author:        
//	Note:            
/*****************************************************************************/
PUBLIC uint32 MEM_GetVMStartAddr(void)
{
     return mem_vm_start_addr;
}

#endif

#if 0 
/*****************************************************************************/
//  Description:    MEM_GetCodeRegion
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetCodeRegion (MEM_REGION_T* code_region)
{
    code_region->start_add = (uint32*)mem_code_start_add;
    code_region->size      = mem_code_size;
    code_region->prev      = NULL;
    code_region->next      = NULL;
    code_region->attribute = MEM_RO_REGION;
}

/*****************************************************************************/
//  Description:    MEM_GetMMURegion
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetMMURegion (MEM_REGION_T* mmu_region)
{
#ifdef PRODUCT_DRIVER_SC6800
    mmu_region->start_add = (uint32*)mem_mmu_tlb_start_add;
    mmu_region->size      = mem_mmu_tlb_size;
    mmu_region->prev      = NULL;
    mmu_region->next      = NULL;
    mmu_region->attribute = MEM_RO_REGION;
#endif	
}

/*****************************************************************************/
//  Description:    MEM_GetRORegionSize
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetRORegion (MEM_REGION_T* ro_region)
{
    ro_region->start_add = (uint32*)mem_ro_start_add;
    ro_region->size      = mem_ro_size;
    ro_region->prev      = NULL;
    ro_region->next      = NULL;
    ro_region->attribute = MEM_RO_REGION;
}

/*****************************************************************************/
//  Description:    MEM_GetRWRegion
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetRWRegion (MEM_REGION_T* rw_region)
{
    rw_region->start_add = (uint32*)mem_rw_start_add;
    rw_region->size      = mem_rw_size;
    rw_region->prev      = NULL;
    rw_region->next      = NULL;
    rw_region->attribute = MEM_RW_REGION;
}

/*****************************************************************************/
//  Description:    MEM_GetIRAMRegionSize
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetIRAMRegion (MEM_REGION_T* iram_region)
{
    iram_region->start_add = (uint32*)mem_iram_start_add;
    iram_region->size      = mem_iram_size;
    iram_region->prev      = NULL;
    iram_region->next      = NULL;
    iram_region->attribute = MEM_RW_REGION;
}

#endif
/*****************************************************************************/
//  Description:    MEM_GetRamTotalSize
//	Global resource dependence: 
//  Author:         
//	Note:           . 
/*****************************************************************************/
PUBLIC uint32 MEM_GetRamTotalSize(void)
{
    return (g_sram_size);
}





/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
