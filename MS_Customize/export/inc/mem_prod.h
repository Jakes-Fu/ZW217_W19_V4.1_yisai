/******************************************************************************
 ** File Name:      mem_prod.h                                                *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           2008-03-07                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the memory config                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/07/2008     jiexia.yu        Create.                                   *
 ** 11/03/2010    	fei.zhang	       Add DSP USB Log function       * 
 *****************************************************************************/
 #ifndef _MEM_PROD_H_
 #define _MEM_PROD_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **--------------------------------------------------------------------------*/
 #include "sci_types.h"
 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*----------------------------------------------------------------------------*
 **                         Dependencies                                     **
 **------------------------------------------------------------------------- */
typedef enum 
{
    MEM_RO_REGION = 0,
    MEM_RW_REGION, 
    MEM_ZI_REGION,
    MEM_ATTRIBUTE_MAX  
}MEM_ATTRIBUTE_E;

typedef struct mem_region_tag
{
    uint32*  start_add;
    uint32   size;
    uint32*  prev;
    uint32*  next;
    MEM_ATTRIBUTE_E   attribute;
}MEM_REGION_T;

/* use */
#define VM_USE_UNKNOWN          0
#define VM_USE_RTCODE           1   /* region for real-time code */
#define VM_USE_DPVIR            2   /* virtual address space for demand paging code */
#define VM_USE_PAGETABLE            3   /* phy address for demand paging and page table */
#define VM_HIGH_VECTOR          4
#define VM_OTHER_PAGE_1K     5
#define VM_OTHER_PAGE_4K     6
/* protection */
#define VM_PROT_NA              0   /* no access */
#define VM_PROT_RO              1   /* read-only */
#define VM_PROT_RW              2   /* read-write */
#define VM_PROT_NC_R            3   /* read-only,no execute, nocache */

/* cache and buffer */
#define VM_CB_UNCACHED          0   /* un-cached and un-buffered */
#define VM_CB_BUFFERED          1   /* un-cached and buffered */
#define VM_CB_WT                2   /* write through */
#define VM_CB_WB                3   /* write back */

typedef struct _VM_REGION
{
    uint32 base; /* base must be aligned on 1MB boundary */
    uint32 size;
	uint32 phy_base;
    uint32 use;
    uint32 prot;
    uint32 cb;
} VM_REGION;

/**----------------------------------------------------------------------------*
**                          Macro Define                                      **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                          Data Prototype                                    **
**----------------------------------------------------------------------------*/
extern const uint32  g_sram_size;
extern const uint32  g_svc_stack_limit_add;
extern const uint32  g_mem_limit;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    MEM_GetTVBuffer
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void *MEM_GetTVBuffer(void);

/*****************************************************************************/
//  Description:    Get all system memory size from address 0x0.
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config .
/*****************************************************************************/
PUBLIC uint32 MEM_GetAllMemSize (void);

/*****************************************************************************/
//  Description:    PROD_GetResMemAdd
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided to MMI. 
/*****************************************************************************/
PUBLIC uint8 * PROD_GetResMemAdd (void);

/*****************************************************************************/
//  Description:    Return practical start address of Demandon OS image partition
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC uint8* MEM_GetUserImageAddress(void);

/*****************************************************************************/
//  Description:    Return practical start address of Demandon OS image partition
//	Global resource dependence: 
//  Author:         
//	Note:            
/*****************************************************************************/
PUBLIC uint32 MEM_GetUserImageSize(void);

/*****************************************************************************/
//  Description:    Get large buffer address
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC uint32* MEM_GetLargeBuf(void);

/*****************************************************************************/
//  Description:    Get large buffer size
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC uint32 MEM_GetLargeBufSize(void);


/*****************************************************************************/
//  Description:    Get the AP ram size 
//	Global resource dependence: 
//  Author:         guangqiao.she
//	Note:           This function is provided from product mem config .
/*****************************************************************************/
PUBLIC uint32 MEM_GetAPSramSize(void);

/*****************************************************************************/
//  Description:    The function is used to get base address of AP ram.
//	Global resource dependence: 
//  Author:        guangqiao.she
//	Note:           
/*****************************************************************************/
PUBLIC uint32 MEM_GetAPSramBaseAddr(void);


/*****************************************************************************/
//  Description:    The function is used to get base address of dump memory.
//	Global resource dependence: 
//  Author:         kevin.wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 MEM_GetDumpBaseAddr(void);

/*****************************************************************************/
//  Description:    The function is used to get length of dump memory.
//	Global resource dependence: 
//  Author:         kevin.wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 MEM_GetDumpLen(void);

/*****************************************************************************/
//  Description:    MEM_GetMainLCDBuffer
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void* MEM_GetMainLCDBuffer(void);

/*****************************************************************************/
//  Description:    MEM_FreeMainLCDBuffer
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_FreeMainLCDBuffer(void);

/*****************************************************************************/
//  Description:    MEM_GetSubLCDBuffer
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void* MEM_GetSubLCDBuffer(void);

/*****************************************************************************/
//  Description:    MEM_GetVBDecOutPutBuf
//	Global resource dependence: 
//  Author:         Liangwen.zhen
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
//PUBLIC void *MEM_GetVBDecOutPutBuf(uint32 * lenght_ptr);

/*****************************************************************************/
//  Description:    MEM_GetBMRoSectionSize
//	Global resource dependence: 
//  Author:         Liangwen.zhen
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC uint32 MEM_GetRoMonitorSize(void);

/*****************************************************************************/
//  Description:    This function is provided from product mem config .
//	Global resource dependence: 
//  Author:         jiexia.yu
//	Note:            
/*****************************************************************************/
PUBLIC BOOLEAN MEM_IsRWRegion(uint32 data_addr);

/*****************************************************************************/
//  Description:    This function is provided to check whether the scatter loading rw overflow the max ram size
//	Global resource dependence: 
//  Author:       
//	Note:            call in init to indicate user
/*****************************************************************************/
PUBLIC BOOLEAN MEM_IsRWRegionOverflow(void);

/*****************************************************************************/
//  Description:    This function is used to check RO Data section.
//	Global resource dependence: 
//  Author:         jiexia.yu
//	Note:            
/*****************************************************************************/
PUBLIC BOOLEAN MEM_IsRORegion(uint32 data_addr);

/*****************************************************************************/
//  Description:    MEM_GetCodeRegion
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetCodeRegion (MEM_REGION_T* code_region);

/*****************************************************************************/
//  Description:    MEM_GetMMURegion
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetMMURegion (MEM_REGION_T* mmu_region);

/*****************************************************************************/
//  Description:    MEM_GetRORegionSize
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetRORegion (MEM_REGION_T* ro_region);

/*****************************************************************************/
//  Description:    MEM_GetRWRegion
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetRWRegion (MEM_REGION_T* rw_region);

/*****************************************************************************/
//  Description:    MEM_GetVMRegion
//	Global resource dependence: 
//  Author:         
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetVmRegion(const VM_REGION **region, uint32 *num);


/******************************************************************************/
// Description:   MEM_HasLoadImage
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MEM_HasLoadImage(void);

/*****************************************************************************/
//  Description:    MEM_GetIRAMRegionSize
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function is provided from product mem config . 
/*****************************************************************************/
PUBLIC void MEM_GetIRAMRegion (MEM_REGION_T* iram_region);

PUBLIC uint32 MEM_GetRamTotalSize(void);

#ifdef VM_SUPPORT

PUBLIC uint32 MEM_GetVMStartAddr(void);

#endif

#ifdef DSP_USB_LOG
/*****************************************************************************/
// Description: get dsp log buffer information
// Global resource dependence: 
// Author: fei.zhang        
// Note: This function is provided from product mem config . 		       
/*****************************************************************************/
PUBLIC uint32 MEM_GetDSPLogBuffRegion (	//If successful, returns SCI_TRUE
			MEM_REGION_T* DSPLogBuff_region	//[IN]used to store dsp log buffer information
			);
#endif

#ifdef CHIP_VER_UWS6121E	
PUBLIC uint32 PROD_GetApnMemSection(uint8** addr);
PUBLIC uint32 PROD_GetConfigNVMemSection(uint8** addr);
PUBLIC uint32 PROD_GetImsMemSection(uint8** addr);
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif  // _MEM_PROD_H_
