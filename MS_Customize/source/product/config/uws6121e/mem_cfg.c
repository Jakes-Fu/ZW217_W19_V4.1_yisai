/******************************************************************************
 ** File Name:      mem_cfg.c                                                 *
 ** Author:         Lin.liu                                                   *
 ** DATE:           2004-04-19                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the memory config                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/19/2004     Lin.liu          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 #include "sci_types.h"
 #include "threadx_os.h"
 #include "os_api.h"
 #include "emc_drv.h"
 #include "sc_reg.h"
 #include "chng_freq.h"
 #include "mem_dump.h"
 #include "resparam.h"
  #include "lcd_cfg.h"
 #include  "mmu.h"
 #include "spiflash_spec.h" 
  #include "mem_cfg.h" 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

extern uint32 Image$$RAM_EXEC_CORE_CODE$$Limit;
extern uint32 Image$$RAM_RW$$Base;

#pragma arm section zidata = "MEM_CFG_LIST_AREA"
__align (4)  uint8 g_mem_cfg_list_start[MEM_RAM_MEM_CFG_LIST_SIZE];
#pragma arm section zidata

#pragma arm section zidata = "CP_ASS_REG_AREA"
__align (4)  uint8 g_cp_ass_reg_start[MEM_RAM_CP_ASS_REG_SIZE];
#pragma arm section zidata

#pragma arm section zidata = "CP_NV_AREA"
__align (4)  uint8 g_cp_nv_start[MEM_RAM_CP_NV_SIZE];
#pragma arm section zidata

#pragma arm section zidata = "RF_CALIB_AREA"
__align (4)  uint8 g_rf_calib_start[MEM_RAM_RF_CALIB_SIZE];
#pragma arm section zidata

#pragma arm section zidata = "RISCV_BIN_AREA"
__align (4)  uint8 g_riscv_bin_start[MEM_RAM_RSICV_BIN_SIZE];
#pragma arm section zidata

#pragma arm section zidata = "IPC_SHMEM_AREA"
__align (4)  uint8 g_ipc_shmem_start[MEM_RAM_IPC_SHMEM_SIZE];
#pragma arm section zidata

#pragma arm section zidata = "AUDIO_SMEM_AREA"
__align (4)  uint8 g_audio_shmem_start[MEM_RAM_AUDIO_SHMEM_SIZE];
#pragma arm section zidata

#pragma arm section zidata = "CP_AREA"
__align (4)  uint8 g_cp_start[MEM_RAM_CP_SIZE];
#pragma arm section zidata

CONST uint32 g_mmu_tlb_l1_size = MMU_TLB_L1_SIZE;
CONST uint32 g_mmu_tlb_l2_size = MMU_TLB_L2_SIZE;

#pragma arm section zidata = "MMU_TLB"
__align (4096)  uint32 g_mmu_table_start[(MMU_TLB_SIZE)>>2];
#pragma arm section zidata


__align(256) uint16 s_main_lcd_buffer[((LCD_WIDTH+15)/16*16)*((LCD_HEIGHT+7)/8*8)];
__align(32) uint16 s_sub_lcd_buffer[SUBLCD_WIDTH * SUBLCD_HEIGHT];

CONST uint32   g_sram_start	        = PHY_RAM_BASE_ADDR;
CONST uint32	g_sram_size		        = MEM_RAM_SIZE;

//have the compiler error with "Image$$RAM_RW$$Base". 
//use the MEM_RAM_AP_BASE_ADDR instead 
CONST uint32 mem_rw_start_add   = MEM_RAM_AP_BASE_ADDR; //&Image$$RAM_RW$$Base;
CONST uint32 mem_rw_size        =   MEM_RAM_END_ADDR - MEM_RAM_AP_BASE_ADDR;//(MEM_RAM_END_ADDR - (uint32)&Image$$RAM_RW$$Base);

CONST uint32 mem_ro_start_add   = (MEM_RAM_AP_BASE_ADDR);
CONST uint32 mem_ro_size        = (&Image$$RAM_EXEC_CORE_CODE$$Limit -MEM_RAM_AP_BASE_ADDR) ;

CONST uint32 mem_iram_start_add = PHY_IRAM_BASE_ADDR;
CONST uint32 mem_iram_size      = MEM_IRAM_SIZE;

CONST uint32 mem_irom_start_add = PHY_IROM_BASE_ADDR;
CONST uint32 mem_irom_size      = MEM_IROM_SIZE;

CONST uint32 mem_vm_start_addr       = VIR_IMAGE_START_ADDR;
CONST uint32 mem_vm_end_addr         = VIR_IMAGE_SIZE;



// PSRAM base address and size
CONST uint32 mem_ap_sram_start_add   = (PHY_RAM_BASE_ADDR+RAM_CFG_AP_OFFSET);
CONST uint32 mem_ap_sram_size        = MEM_RAM_AP_SIZE+MEM_RAM_MMU_SIZE;

#if  defined( MULTI_FLASH_SUPPORT)

CONST uint32 mem_ro2_start_add    = PHY_FLASH_START_ADDR_B;  
CONST uint32 mem_ro2_size         = PHY_FLASH_SIZE_B;            
#endif


CONST uint32 system_stack_size  = (16 * 1024);  //16K
CONST uint32 g_fiq_stack_size		= 16;
CONST uint32 g_irq_stack_size		= 2048;
CONST uint32 g_undefine_stack_size  = 2048;
CONST uint32 g_abort_stack_size		= 2048;
CONST uint32 g_sys_stack_size		= 32; 
CONST uint32 g_timer_stack_size		= 2048;

#if 0
CONST uint32 g_vfp_saved_regs_size  = 136*16;
#pragma arm section zidata = "VFP_SAVED_REG_AREA"
    uint8 g_vfp_reg_list[g_vfp_saved_regs_size];
#pragma arm section zidata	
#endif

#pragma arm section zidata = "SYS_STACK_AREA"
    uint8 g_system_stack[system_stack_size];
#pragma arm section zidata	

#pragma arm section zidata = "FIQ_STACK_AREA"
    uint8 g_fiq_stack[g_fiq_stack_size];
#pragma arm section zidata	

//CONST uint32    g_sram_start	        = HW_SRAM_START_ADDR;
//CONST uint32	g_sram_size		        = SRAM_TOTAL_SIZE;

//the system memory allocation define,the allocation map defeine is different when
//chip,module and the flash chip used in the module is different for the production 
//                                                  // Typical Define 
CONST uint32    g_svc_stack_limit_add 	= (uint32)&g_system_stack[system_stack_size];
//CONST uint32 	const_RAM_LimitOffset	= (uint32)g_system_stack;    

//the ZW and ZI data should not excced the RAM LIMIT,and RAM after RAM_LIMIT
//is used as stack of exceptions as FIQ,IRQ,ABORT and so on
CONST uint32    g_mem_limit 			= (uint32)g_system_stack;
CONST uint32    g_sys_stack_base        = (uint32)g_system_stack;
CONST uint32 	g_fiq_stack_base 		= (uint32)g_fiq_stack;   
    

/* Nv itme max number defined by customer */
#ifdef NV_PRTITION_TINY
CONST uint16 NV_MAX_ID = 1500;  
#else
CONST uint16 NV_MAX_ID = 3529;  
#endif

// Each block is 1024byte(defined in efs_config.h), so the total file sytem space is 
//  1024 * g_file_system_data_block_num
CONST uint32 g_file_system_data_block_num = 1;  /*200->1*/
// The max file number that support.
CONST uint32 g_file_system_file_num       = 1; /*32->1*/

CONST uint32 g_nvitem_cache_enabled		  = 1;

//SFS memory config Start Jason.wu
uint32 g_sfs_bufMgr_min_count_of_clusBuf  = 3;	//the counter of memory block in device memory manager.value range is >=1,
uint32 g_sfs_bufMgr_min_buf_size          = 10;	//  minimal  size of buf block.unit is sector.the range is [1,128]
uint32 g_sfs_bufMgr_max_idle_write_size   = 20;	//unit is sector.when handset is In idle, SFS will write a number of block data to physical storage.the value range is >= 1
//SFS memory config End



// Be careful, the array below must be 4 bytes alignment.
// Byte heap size
//@Ivan.yin for GPRS 2004-1-13
#ifdef WIN32
	#define BYTE_HEAP_SIZE          (1700 * 1024) //   //bruce, 1700->1200

#else
          #define     SCI_STATIC_HEAP_SIZE_THRESHOD     0x14000   //80KB
	 #define     SCI_APP_HEAP_SIZE_THRESHOD           0x10000   //64KB
	
	 #define     SCI_SYSTEM_SPACE_ALLOC_THRESHOD   0x4000      //16KB
	 #define     SCI_SYSTEM_MMAP_THRESHOLD         0xC000     //48KB

         #define  	MAX_STATIC_SPACE_SIZE             (400*1024 ) //SFS decrease memory 300K, Alarm decrease memory 30K
         
         
         
         #if defined(BZP_SUPPORT)
         
         	#define   MAX_SYSTEM_SPACE_SIZE             (2380 * 1024)
         #else
         
         	#define   MAX_SYSTEM_SPACE_SIZE             (3100 * 1024)
         #endif

         #define    	MAX_BASE_SPACE_SIZE  	  (50 * 1024) 
         #define    	MAX_APP_SPACE_SIZE  	          (380* 1024)
         

	#define 	BYTE_HEAP_SIZE    			      (MAX_STATIC_SPACE_SIZE  + MAX_SYSTEM_SPACE_SIZE + MAX_BASE_SPACE_SIZE + MAX_APP_SPACE_SIZE)
#endif


	/*Define the memory threshod size in the byte heap, when the allocation memory size less than 
	  the threshod, it will be allocated from the heap low address, or it will be allocated from the
	  heap high address*/
CONST uint32  g_static_heap_size_threshod = SCI_STATIC_HEAP_SIZE_THRESHOD;
CONST uint32  g_app_heap_size_threshod    = SCI_APP_HEAP_SIZE_THRESHOD;
CONST uint32  g_system_space_alloc_threshod    = SCI_SYSTEM_SPACE_ALLOC_THRESHOD;
CONST uint32  g_system_mmap_threshod      = SCI_SYSTEM_MMAP_THRESHOLD;


CONST uint32 max_static_space_size  =  MAX_STATIC_SPACE_SIZE;
CONST uint32 max_system_space_size  =  MAX_SYSTEM_SPACE_SIZE;
CONST uint32 max_app_space_size     =  MAX_APP_SPACE_SIZE;
CONST uint32 byte_head_size     	=  BYTE_HEAP_SIZE;
CONST uint32 max_base_space_size    =  MAX_BASE_SPACE_SIZE;

#pragma arm section zidata = "BYTE_HEAP_AREA"
uint32    BYTE_HEAP_ADDR[ byte_head_size >> 2 ];
#pragma arm section zidata 



#if defined(BZP_SUPPORT)

#define  VIR_RESOURCE_STRAT_ADDR       (VIR_IMAGE_START_ADDR + VIR_IMAGE_SIZE)
#define  VIR_RESOURCE_SIZE                      (PHY_FLASH_SIZE >> 1)  /* !should be larger than the Resource LENGTH*/

#define  PHY_RES_SWAP_MEM_SIZE             (32*1024) 
#define  PHY_IMAGE_SWAP_MEM_SIZE         (448*1024)

#define PHY_TOTAL_SWAP_MEM_SIZE          (PHY_RES_SWAP_MEM_SIZE + PHY_IMAGE_SWAP_MEM_SIZE)

#pragma arm section zidata = "PHY_DP_SWAP"
__align (4096) uint32    g_mem_swap_buffer[PHY_TOTAL_SWAP_MEM_SIZE >> 2 ];
#pragma arm section zidata

#define  PHY_IMAGE_SWAP_MEM_START_ADDR  &g_mem_swap_buffer[0]
#define  PHY_RES_SWAP_MEM_START_ADDR       &g_mem_swap_buffer[PHY_IMAGE_SWAP_MEM_SIZE >>4]

#else

#define PHY_TOTAL_SWAP_MEM_SIZE          (0)

#endif



//rw,ro region need align to 4kB(1pages)
//For every demand paging region pages need > 3 (page tyes:1k or 4k).
const static VM_REGION vm_region[] = 
{

    /*--vir base addr-----size----- phy base addr--------type------access----control---- */
    {
        PHY_IROM_BASE_ADDR,
        MEM_IROM_SIZE,
        PHY_IROM_BASE_ADDR,
        VM_OTHER_PAGE_4K,
        (VM_PROT_NC_R),
        VM_CB_UNCACHED
      },
      
    /* flash1, 16MB */
    {
        PHY_FLASH_START_ADDR,
        PHY_FLASH_SIZE,
        PHY_FLASH_START_ADDR,
        VM_USE_RTCODE,
        (VM_PROT_RW),
        VM_CB_WB
      },

#if  defined( MULTI_FLASH_SUPPORT)
    {/*{0x00000000, 0x00400000,  0x00000000,  VM_USE_RTCODE, 0,    0}*/
        PHY_FLASH_START_ADDR_B,
        PHY_FLASH_SIZE_B,
        PHY_FLASH_START_ADDR_B,
        VM_USE_RTCODE,
        (VM_PROT_RW),
        VM_CB_WB
	},
  #endif

    {
        PHY_RAM_BASE_ADDR,
        (MEM_RAM_AP_BASE_ADDR - PHY_RAM_BASE_ADDR),
        PHY_RAM_BASE_ADDR,
        VM_OTHER_PAGE_4K,
        (VM_PROT_RW),
        VM_CB_UNCACHED
	},

    {
        (MEM_RAM_AP_BASE_ADDR),
        (MEM_RAM_AP_END_ADDR - MEM_RAM_AP_BASE_ADDR),
        (MEM_RAM_AP_BASE_ADDR),
        (VM_OTHER_PAGE_4K),
        (VM_PROT_RW),
        (VM_CB_WB)
    },


 /* page table and pfn */
    {
        (MEM_RAM_MMU_BASE_ADDR),
        (MEM_RAM_MMU_SIZE), //The physic Memory size Only For PGD Table and all PTE Table 
        (MEM_RAM_MMU_BASE_ADDR),
        (VM_USE_PAGETABLE),
        (PHY_TOTAL_SWAP_MEM_SIZE),//total physic memory swap  size
        (0)
    },
#ifdef BZP_SUPPORT
    /*
	   if user image increaed, so pay attention to modify here in condition "VM_SUPPORT == TRUE"
	*/
    {   
        (VIR_IMAGE_START_ADDR),//vir start
        (VIR_IMAGE_SIZE), //vir size
        (PHY_IMAGE_SWAP_MEM_START_ADDR),//demand paging phy start addr
        (VM_USE_DPVIR), //used for demandpaging region 
        (PHY_IMAGE_SWAP_MEM_SIZE),//page_size 1024,4096
        (PAGE_SIZE_4K)
    },
    {
        (VIR_RESOURCE_STRAT_ADDR), //vir start
        (VIR_RESOURCE_SIZE), //vir size
        (PHY_RES_SWAP_MEM_START_ADDR),//demand paging phy start addr
        (VM_USE_DPVIR), //used for demandpaging region 
        (PHY_RES_SWAP_MEM_SIZE),//phy size :at least > 2 pagesize
        (PAGE_SIZE_4K)//page_size 1024,4096
    },
#endif    
};

void MEM_GetVmRegion(const VM_REGION **region, uint32 *num)
{
    *region = vm_region;
    *num = sizeof(vm_region) / sizeof(vm_region[0]);
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
